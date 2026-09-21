/**
 * @file pts_wifi_cmd.h
 * @brief Phase 6: join AP + UDP command socket (master or receiver role).
 */
#ifndef PTS_WIFI_CMD_H
#define PTS_WIFI_CMD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  PTS_WIFI_ROLE_MASTER = 0,
  PTS_WIFI_ROLE_RECEIVER
} pts_wifi_role_t;

/**
 * Optional: when master receives SCHED, call this to fan-out PTS6.
 * Return 0 on success. May be NULL (Linux sends PTS6 to receivers itself).
 */
typedef int (*pts_wifi_fanout_fn)(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m);

/**
 * Receiver: when PTS6 arrives, call this (typically UART forward to BG24).
 */
typedef void (*pts_wifi_on_sched_fn)(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m, uint8_t status);

/**
 * Join AP (SSID/PSK from pts_wifi_cmd_protocol.h / sl_net_default_values.h),
 * bind UDP, start async RX. Call after sl_wifi_init() / BLE bring-up.
 * Returns 0 on success.
 */
int pts_wifi_cmd_start(pts_wifi_role_t role,
                       pts_wifi_fanout_fn fanout,
                       pts_wifi_on_sched_fn on_sched);

/** Non-zero after IP config done. */
int pts_wifi_cmd_ready(void);

/** Print IPv4 once ready (for VCOM). */
void pts_wifi_cmd_log_ip(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_WIFI_CMD_H */
