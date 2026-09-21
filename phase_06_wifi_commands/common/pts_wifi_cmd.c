/**
 * @file pts_wifi_cmd.c
 * @brief Join STA + UDP TIME/SCHED (master) or PTS6 (receiver).
 *
 * Pattern: wifi_ble_power_save wifi_app.c + twt_use_case_remote_app UDP server.
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
#define PTS_WIFI_PSK_STR  DEFAULT_WIFI_CLIENT_CREDENTIAL
#endif

static pts_wifi_role_t s_role;
static pts_wifi_fanout_fn s_fanout;
static pts_wifi_on_sched_fn s_on_sched;
static int s_sock = -1;
static uint8_t s_ready;
static struct sockaddr_in s_peer; /* last peer for replies */

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
  if (n >= PTS_WIFI_LINE_MAX) {
    n = PTS_WIFI_LINE_MAX - 1u;
  }
  memcpy(line, buffer, n);
  line[n] = '\0';
  /* strip CR/LF */
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

  if (s_role == PTS_WIFI_ROLE_MASTER) {
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
      uint32_t cmd_id = 0;
      uint32_t lead_ms = 0;
      uint8_t cmd_type = 0;
      if (pts_wifi_parse_sched(line, &cmd_id, &cmd_type, &lead_ms) == 0) {
        uint32_t hz = osKernelGetSysTimerFreq();
        uint32_t lead_ticks = (hz / 1000u) * lead_ms;
        uint32_t exec_m = osKernelGetSysTimerCount() + lead_ticks;
        char reply[PTS_WIFI_LINE_MAX];
        int rn = pts_wifi_format_ok(reply, sizeof(reply), cmd_id, exec_m);
        if (rn > 0) {
          (void)sl_si91x_sendto(s_sock, (const uint8_t *)reply, (size_t)rn, 0,
                                (const struct sockaddr *)&s_peer, sizeof(s_peer));
        }
        printf("PTS wifi SCHED cmd=%lu lead_ms=%lu exec_m=%lu\n",
               (unsigned long)cmd_id, (unsigned long)lead_ms, (unsigned long)exec_m);
        if (s_fanout != NULL) {
          (void)s_fanout(cmd_id, cmd_type, exec_m);
        }
        return;
      }
    }
  }

  {
    uint32_t cmd_id = 0;
    uint32_t exec_m = 0;
    uint8_t cmd_type = 0;
    uint8_t status = 0;
    if (pts_wifi_parse_pts6(line, &cmd_id, &cmd_type, &exec_m, &status) == 0) {
      printf("PTS wifi PTS6 cmd=%lu type=%u exec_m=%lu\n",
             (unsigned long)cmd_id, (unsigned)cmd_type, (unsigned long)exec_m);
      if (s_on_sched != NULL) {
        s_on_sched(cmd_id, cmd_type, exec_m, status);
      }
    }
  }
}

static int join_and_dhcp(void)
{
  sl_status_t status;
  sl_wifi_client_configuration_t ap = { 0 };
  sl_net_ip_configuration_t ipcfg = { 0 };
  sl_wifi_credential_id_t id = SL_NET_DEFAULT_WIFI_CLIENT_CREDENTIAL_ID;

  status = sl_net_set_credential(id, SL_NET_WIFI_PSK, PTS_WIFI_PSK_STR, strlen(PTS_WIFI_PSK_STR));
  if (status != SL_STATUS_OK) {
    printf("PTS wifi set_credential fail 0x%lX\n", (unsigned long)status);
    return -1;
  }

  ap.ssid.length = (uint8_t)strlen(PTS_WIFI_SSID_STR);
  memcpy(ap.ssid.value, PTS_WIFI_SSID_STR, ap.ssid.length);
  ap.security      = SL_WIFI_WPA2;
  ap.encryption    = SL_WIFI_DEFAULT_ENCRYPTION;
  ap.credential_id = id;

  printf("PTS wifi joining SSID=%s\n", PTS_WIFI_SSID_STR);
  status = sl_wifi_connect(SL_WIFI_CLIENT_2_4GHZ_INTERFACE, &ap, PTS_WIFI_JOIN_TIMEOUT_MS);
  if (status != SL_STATUS_OK) {
    printf("PTS wifi connect fail 0x%lX\n", (unsigned long)status);
    return -1;
  }

  ipcfg.type      = SL_IPV4;
  ipcfg.mode      = SL_IP_MANAGEMENT_DHCP;
  ipcfg.host_name = NULL;
  status = sl_si91x_configure_ip_address(&ipcfg, SL_SI91X_WIFI_CLIENT_VAP_ID);
  if (status != SL_STATUS_OK) {
    printf("PTS wifi DHCP fail 0x%lX\n", (unsigned long)status);
    return -1;
  }

  printf("PTS wifi IP %u.%u.%u.%u\n",
         ipcfg.ip.v4.ip_address.bytes[0],
         ipcfg.ip.v4.ip_address.bytes[1],
         ipcfg.ip.v4.ip_address.bytes[2],
         ipcfg.ip.v4.ip_address.bytes[3]);
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
  rc = sl_si91x_bind(s_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0) {
    printf("PTS wifi bind fail errno=%d\n", errno);
    return -1;
  }
  printf("PTS wifi UDP listen port=%u sock=%d\n", (unsigned)port, s_sock);
  return 0;
}

int pts_wifi_cmd_start(pts_wifi_role_t role,
                       pts_wifi_fanout_fn fanout,
                       pts_wifi_on_sched_fn on_sched)
{
  uint16_t port;

  s_role     = role;
  s_fanout   = fanout;
  s_on_sched = on_sched;
  s_ready    = 0u;

  if (join_and_dhcp() != 0) {
    return -1;
  }
  port = (role == PTS_WIFI_ROLE_MASTER) ? PTS_WIFI_MASTER_PORT : PTS_WIFI_RX_PORT;
  if (open_udp(port) != 0) {
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
  /* IP already printed at join. */
}
