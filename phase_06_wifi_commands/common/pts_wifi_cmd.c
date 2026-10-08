/**
 * @file pts_wifi_cmd.c
 * @brief Master-only: join STA + UDP ON/OFF/SCHED. Schedule rides BLE (not Wi-Fi).
 *
 * UDP CB: stamp exec_m + OK reply (sendto must stay in CB on Si91x).
 * App thread pts_wifi_cmd_poll(): arm LED1 + BLE PTS6 fanout.
 */
#include "pts_wifi_cmd.h"
#include "pts_wifi_cmd_protocol.h"
#include "pts_schedule_protocol.h"

#include "cmsis_os2.h"
#include "sl_status.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "sl_net.h"
#include "sl_net_si91x.h"
#include "sl_net_wifi_types.h"
#include "sl_net_default_values.h"
#include "sl_si91x_driver.h"
#include "sl_si91x_socket.h"
#include "sl_si91x_socket_types.h"
#include "socket.h"
#include "errno.h"

#include <stdio.h>
#include <string.h>

#ifndef PTS_WIFI_SSID_STR
#define PTS_WIFI_SSID_STR DEFAULT_WIFI_CLIENT_PROFILE_SSID
#endif
#ifndef PTS_WIFI_PSK_STR
#define PTS_WIFI_PSK_STR DEFAULT_WIFI_CLIENT_CREDENTIAL
#endif
#ifndef PTS_WIFI_CONNECT_TIMEOUT_MS
#define PTS_WIFI_CONNECT_TIMEOUT_MS 120000u
#endif
#ifndef PTS_WIFI_CONNECT_RETRIES
#define PTS_WIFI_CONNECT_RETRIES 3u
#endif

#define PTS_WIFI_CMD_Q_DEPTH 8u

typedef struct {
  uint32_t cmd_id;
  uint32_t exec_m;
  uint8_t cmd_type;
  uint8_t valid;
} pts_wifi_cmd_q_t;

static pts_wifi_role_t s_role;
static pts_wifi_fanout_fn s_on_cmd;
static pts_wifi_on_sched_fn s_on_sched;
static int s_sock = -1;
static uint8_t s_ready;
static struct sockaddr_in s_peer;
static uint32_t s_cmd_seq;

static pts_wifi_cmd_q_t s_q[PTS_WIFI_CMD_Q_DEPTH];
static volatile uint8_t s_q_wr;
static volatile uint8_t s_q_rd;

static int q_push(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m)
{
  uint8_t wr   = s_q_wr;
  uint8_t next = (uint8_t)((wr + 1u) % PTS_WIFI_CMD_Q_DEPTH);

  if (next == s_q_rd) {
    return -1;
  }
  s_q[wr].cmd_id   = cmd_id;
  s_q[wr].cmd_type = cmd_type;
  s_q[wr].exec_m   = exec_m;
  s_q[wr].valid    = 1u;
  s_q_wr           = next;
  return 0;
}

static int q_pop(pts_wifi_cmd_q_t *out)
{
  uint8_t rd = s_q_rd;

  if (rd == s_q_wr) {
    return 0;
  }
  *out   = s_q[rd];
  s_q_rd = (uint8_t)((rd + 1u) % PTS_WIFI_CMD_Q_DEPTH);
  return 1;
}

/** Stamp + OK in CB; queue arm/fanout for app thread. */
static void master_stamp_reply_queue(uint32_t cmd_id, uint8_t cmd_type, uint32_t lead_ms)
{
  uint32_t hz         = osKernelGetSysTimerFreq();
  uint32_t lead_ticks = pts_ms_to_systimer_ticks(lead_ms, hz);
  uint32_t exec_m     = osKernelGetSysTimerCount() + lead_ticks;
  char reply[PTS_WIFI_LINE_MAX];
  int rn;

  rn = pts_wifi_format_ok(reply, sizeof(reply), cmd_id, exec_m);
  if (rn > 0) {
    (void)sl_si91x_sendto(s_sock, (const uint8_t *)reply, (size_t)rn, 0,
                          (const struct sockaddr *)&s_peer, sizeof(s_peer));
  }
  printf("PTS wifi CMD cmd=%lu type=%u lead_ms=%lu exec_m=%lu (BLE schedule)\n",
         (unsigned long)cmd_id,
         (unsigned)cmd_type,
         (unsigned long)lead_ms,
         (unsigned long)exec_m);

  if (q_push(cmd_id, cmd_type, exec_m) != 0) {
    printf("PTS wifi CMD queue full (drop arm)\n");
  }
}

static void udp_rx_cb(uint32_t sock_no,
                      uint8_t *buffer,
                      uint32_t length,
                      const sl_si91x_socket_metadata_t *meta)
{
  char line[PTS_WIFI_LINE_MAX];
  uint32_t n = length;

  (void)sock_no;
  if ((buffer == NULL) || (length == 0u) || (s_sock < 0)) {
    return;
  }
  if (s_role != PTS_WIFI_ROLE_MASTER) {
    return;
  }
  if (n >= PTS_WIFI_LINE_MAX) {
    n = PTS_WIFI_LINE_MAX - 1u;
  }
  memcpy(line, buffer, n);
  line[n] = '\0';
  for (uint32_t i = 0; i < n; i++) {
    if ((line[i] == '\r') || (line[i] == '\n')) {
      line[i] = '\0';
      break;
    }
  }

  if (meta != NULL) {
    memset(&s_peer, 0, sizeof(s_peer));
    s_peer.sin_family = AF_INET;
    s_peer.sin_port   = meta->dest_port;
    memcpy(&s_peer.sin_addr.s_addr, meta->dest_ip_addr.ipv4_address, 4);
  }

  if (pts_wifi_is_time_req(line)) {
    char reply[PTS_WIFI_LINE_MAX];
    int rn = pts_wifi_format_time_reply(reply, sizeof(reply),
                                        osKernelGetSysTimerCount(),
                                        osKernelGetSysTimerFreq());
    if (rn > 0) {
      (void)sl_si91x_sendto(s_sock, (const uint8_t *)reply, (size_t)rn, 0,
                            (const struct sockaddr *)&s_peer, sizeof(s_peer));
    }
    return;
  }
  {
    uint8_t cmd_type = 0;
    uint32_t lead_ms = 0;
    if (pts_wifi_parse_onoff(line, &cmd_type, &lead_ms) == 0) {
      s_cmd_seq++;
      master_stamp_reply_queue(s_cmd_seq, cmd_type, lead_ms);
      return;
    }
  }
  {
    uint32_t cmd_id  = 0;
    uint32_t lead_ms = 0;
    uint8_t cmd_type = 0;
    if (pts_wifi_parse_sched(line, &cmd_id, &cmd_type, &lead_ms) == 0) {
      if (cmd_id == 0u) {
        s_cmd_seq++;
        cmd_id = s_cmd_seq;
      } else if (cmd_id > s_cmd_seq) {
        s_cmd_seq = cmd_id;
      }
      master_stamp_reply_queue(cmd_id, cmd_type, lead_ms);
    }
  }
  (void)s_on_sched;
}

void pts_wifi_cmd_poll(void)
{
  pts_wifi_cmd_q_t item;

  if (!s_ready || (s_role != PTS_WIFI_ROLE_MASTER)) {
    return;
  }
  while (q_pop(&item) != 0) {
    if (s_on_cmd != NULL) {
      (void)s_on_cmd(item.cmd_id, item.cmd_type, item.exec_m);
    }
  }
}

static int try_connect(sl_wifi_security_t sec)
{
  sl_status_t status;
  sl_wifi_client_configuration_t ap = { 0 };
  sl_wifi_credential_id_t id        = SL_NET_DEFAULT_WIFI_CLIENT_CREDENTIAL_ID;

  ap.ssid.length = (uint8_t)strlen(PTS_WIFI_SSID_STR);
  memcpy(ap.ssid.value, PTS_WIFI_SSID_STR, ap.ssid.length);
  ap.security      = sec;
  ap.encryption    = SL_WIFI_DEFAULT_ENCRYPTION;
  ap.credential_id = id;

  status = sl_wifi_connect(SL_WIFI_CLIENT_2_4GHZ_INTERFACE, &ap, PTS_WIFI_CONNECT_TIMEOUT_MS);
  return (status == SL_STATUS_OK) ? 0 : (int)status;
}

static int join_and_dhcp(void)
{
  sl_status_t status;
  sl_net_ip_configuration_t ipcfg = { 0 };
  sl_wifi_credential_id_t id      = SL_NET_DEFAULT_WIFI_CLIENT_CREDENTIAL_ID;
  uint8_t attempt;
  int rc = -1;

  status = sl_net_set_credential(id, SL_NET_WIFI_PSK, PTS_WIFI_PSK_STR, strlen(PTS_WIFI_PSK_STR));
  if (status != SL_STATUS_OK) {
    printf("PTS wifi set_credential fail 0x%lX\n", (unsigned long)status);
    return -1;
  }

  printf("PTS wifi joining SSID=%s (master only; 2.4GHz)\n", PTS_WIFI_SSID_STR);

  for (attempt = 0u; attempt < PTS_WIFI_CONNECT_RETRIES; attempt++) {
    rc = try_connect(SL_WIFI_WPA3_TRANSITION);
    if (rc != 0) {
      printf("PTS wifi WPA3_TRANSITION fail 0x%X try=%u\n", (unsigned)rc, (unsigned)(attempt + 1u));
      rc = try_connect(SL_WIFI_WPA2);
    }
    if (rc == 0) {
      break;
    }
    printf("PTS wifi WPA2 fail 0x%X try=%u\n", (unsigned)rc, (unsigned)(attempt + 1u));
    (void)osDelay(1000u);
  }
  if (rc != 0) {
    printf("PTS wifi connect fail 0x%X (0x10003=NO_AP_FOUND)\n", (unsigned)rc);
    return -1;
  }

  ipcfg.type      = SL_IPV4;
  ipcfg.mode      = SL_IP_MANAGEMENT_DHCP;
  ipcfg.host_name = NULL;
  status          = sl_si91x_configure_ip_address(&ipcfg, SL_SI91X_WIFI_CLIENT_VAP_ID);
  if (status != SL_STATUS_OK) {
    printf("PTS wifi DHCP fail 0x%lX\n", (unsigned long)status);
    return -1;
  }

  printf("PTS wifi IP %u.%u.%u.%u UDP :%u\n",
         ipcfg.ip.v4.ip_address.bytes[0],
         ipcfg.ip.v4.ip_address.bytes[1],
         ipcfg.ip.v4.ip_address.bytes[2],
         ipcfg.ip.v4.ip_address.bytes[3],
         (unsigned)PTS_WIFI_MASTER_PORT);
  return 0;
}

static int open_udp(uint16_t port)
{
  struct sockaddr_in addr = { 0 };
  int rc;

  s_sock = sl_si91x_socket_async(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &udp_rx_cb);
  if (s_sock < 0) {
    printf("PTS wifi socket fail errno=%d\n", errno);
    return -1;
  }
  addr.sin_family = AF_INET;
  addr.sin_port   = port;
  rc              = sl_si91x_bind(s_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0) {
    printf("PTS wifi bind fail errno=%d\n", errno);
    return -1;
  }
  printf("PTS wifi UDP listen port=%u\n", (unsigned)port);
  return 0;
}

int pts_wifi_cmd_start(pts_wifi_role_t role,
                       pts_wifi_fanout_fn fanout,
                       pts_wifi_on_sched_fn on_sched)
{
  s_role     = role;
  s_on_cmd   = fanout;
  s_on_sched = on_sched;
  s_ready    = 0u;
  s_cmd_seq  = 0u;
  s_q_wr     = 0u;
  s_q_rd     = 0u;
  memset(s_q, 0, sizeof(s_q));

  if (role != PTS_WIFI_ROLE_MASTER) {
    printf("PTS wifi: receiver role ignored (BLE schedule only)\n");
    return 0;
  }
  if (join_and_dhcp() != 0) {
    return -1;
  }
  if (open_udp(PTS_WIFI_MASTER_PORT) != 0) {
    return -1;
  }
  s_ready = 1u;
  return 0;
}

int pts_wifi_cmd_ready(void)
{
  return s_ready ? 1 : 0;
}

void pts_wifi_cmd_log_ip(void)
{
}
