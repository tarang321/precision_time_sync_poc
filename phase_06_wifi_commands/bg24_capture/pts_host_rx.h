/**
 * @file pts_host_rx.h
 * @brief BG24 USART RX of Si917 PTS4 identity and PTS5 schedule frames.
 */
#ifndef PTS_HOST_RX_H
#define PTS_HOST_RX_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t  board_id;
  uint32_t trigger_seq;
  int8_t   rssi;
  uint32_t fu_seq;
  uint32_t m_ticks;
  uint32_t session;
  uint8_t  fu_valid;
} pts_host_id_t;

typedef struct {
  uint8_t  board_id;
  uint32_t cmd_id;
  uint8_t  cmd_type;
  uint32_t exec_m;
  uint8_t  status;
} pts_host_sched_t;

void pts_host_rx_init(void);

/** Non-blocking: parse completed PTS4 lines from RX ISR buffer. */
bool pts_host_rx_pop(pts_host_id_t *out);

/** Non-blocking: parse completed PTS5 schedule lines. */
bool pts_host_rx_pop_sched(pts_host_sched_t *out);

#ifdef __cplusplus
}
#endif

#endif /* PTS_HOST_RX_H */
