/**
 * @file pts_prs_timer_capture.h
 * @brief BG24: GPIO edge -> async PRS -> TIMER0 CC0 hardware capture R[].
 */
#ifndef PTS_PRS_TIMER_CAPTURE_H
#define PTS_PRS_TIMER_CAPTURE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PTS_CAPTURE_RING_SIZE 64u

typedef struct {
  uint32_t r_ticks;   /**< TIMER0 capture value R */
  uint32_t irq_seq;   /**< Monotonic capture index (not BLE seq) */
} pts_capture_sample_t;

/** Init GPIO input, async PRS ch0, TIMER0 CC0 capture on rising edge. */
void pts_prs_timer_capture_init(void);

/** Pop oldest unmatched capture. Returns false if empty. */
bool pts_prs_timer_capture_pop(pts_capture_sample_t *out);

/** Number of captures waiting for identity association. */
uint32_t pts_prs_timer_capture_pending(void);

/** Overflow / drop counters for diagnostics. */
uint32_t pts_prs_timer_capture_overflows(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_PRS_TIMER_CAPTURE_H */
