/**
 * @file pts_host_rx.h
 * @brief BG24 USART RX of Si917 PTS3 trigger/follow-up frames.
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

void pts_host_rx_init(void);

/** Non-blocking: parse completed lines from RX ISR buffer. */
bool pts_host_rx_pop(pts_host_id_t *out);

#ifdef __cplusplus
}
#endif

#endif /* PTS_HOST_RX_H */
