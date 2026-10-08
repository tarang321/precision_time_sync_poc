/**
 * @file pts_wifi_cmd.h
 * @brief Phase 6: join AP + UDP command socket (master only; RX ignores).
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
 * Master: arm local LED1 at exec_m. Wi-Fi layer then forwards PTS6 to RX.
 */
typedef int (*pts_wifi_fanout_fn)(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m);

/**
 * Receiver: unused in Phase 6 (BLE schedule only).
 */
typedef void (*pts_wifi_on_sched_fn)(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m, uint8_t status);

/**
 * Join AP, bind UDP, start async RX. Call after PA + seq timer are running.
 * Returns 0 on success.
 */
int pts_wifi_cmd_start(pts_wifi_role_t role,
                       pts_wifi_fanout_fn fanout,
                       pts_wifi_on_sched_fn on_sched);

/** Non-zero after IP config done. */
int pts_wifi_cmd_ready(void);

/** Print IPv4 once ready (for VCOM). */
void pts_wifi_cmd_log_ip(void);

/**
 * App-thread: stamp exec_m + arm/fanout for cmds queued from UDP CB.
 * Call from master main loop.
 */
void pts_wifi_cmd_poll(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_WIFI_CMD_H */
