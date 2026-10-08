/**
 * @file pts_rx_ring.h
 * @brief SPSC ring for AE reports + local R[N] (callback -> app thread).
 */
#ifndef PTS_RX_RING_H
#define PTS_RX_RING_H

#include <stdint.h>
#include <string.h>
#include "pts_followup_protocol.h"

#ifndef PTS_RX_RING_DEPTH
#define PTS_RX_RING_DEPTH 128u
#endif

#define PTS_RX_RING_DATA_MAX PTS_FU_PAYLOAD_LEN

typedef struct {
  uint8_t data[PTS_RX_RING_DATA_MAX];
  uint8_t data_len;
  uint8_t data_status;
  int8_t rssi;
  uint16_t cb_status;
  uint32_t r_ticks;
} pts_rx_slot_t;

typedef struct {
  volatile uint16_t head;
  volatile uint16_t tail;
  volatile uint32_t drops;
  pts_rx_slot_t slot[PTS_RX_RING_DEPTH];
} pts_rx_ring_t;

static inline void pts_rx_ring_init(pts_rx_ring_t *r)
{
  r->head  = 0u;
  r->tail  = 0u;
  r->drops = 0u;
}

static inline uint16_t pts_rx_ring_next(uint16_t i)
{
  return (uint16_t)((i + 1u) % PTS_RX_RING_DEPTH);
}

static inline int pts_rx_ring_push(pts_rx_ring_t *r,
                                   const uint8_t *data,
                                   uint8_t data_len,
                                   int8_t rssi,
                                   uint32_t r_ticks,
                                   uint8_t data_status,
                                   uint16_t cb_status)
{
  uint16_t h = r->head;
  uint16_t n = pts_rx_ring_next(h);
  if (n == r->tail) {
    r->drops++;
    return -1;
  }
  uint8_t ncopy = data_len;
  if (ncopy > PTS_RX_RING_DATA_MAX) {
    ncopy = PTS_RX_RING_DATA_MAX;
  }
  if ((data != NULL) && (ncopy > 0u)) {
    memcpy(r->slot[h].data, data, ncopy);
  }
  r->slot[h].data_len    = ncopy;
  r->slot[h].data_status = data_status;
  r->slot[h].cb_status   = cb_status;
  r->slot[h].rssi        = rssi;
  r->slot[h].r_ticks     = r_ticks;
  r->head                 = n;
  return 0;
}

static inline int pts_rx_ring_pop(pts_rx_ring_t *r, pts_rx_slot_t *out)
{
  uint16_t t = r->tail;
  if (t == r->head) {
    return -1;
  }
  *out    = r->slot[t];
  r->tail = pts_rx_ring_next(t);
  return 0;
}

#endif /* PTS_RX_RING_H */
