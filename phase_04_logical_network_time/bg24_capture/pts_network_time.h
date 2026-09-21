/**
 * @file pts_network_time.h
 * @brief Phase 4 logical network time on the BG24 timer domain.
 *
 * Network time = M0 + rate * (local TIMER0 - R0)
 * The running timer is never reset. M and R are different tick units;
 * rate is master-counts per BG24 tick, held as rate_num/rate_den.
 */
#ifndef PTS_NETWORK_TIME_H
#define PTS_NETWORK_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  PTS_SYNC_UNLOCKED = 0,
  PTS_SYNC_ACQUIRING,
  PTS_SYNC_LOCKED,
  PTS_SYNC_HOLDOVER
} pts_sync_quality_t;

void pts_network_time_init(void);

/** Drop the fit when the master session id changes. */
void pts_network_time_on_session(uint32_t session);

/**
 * One accepted follow-up pair. Returns 0 if the sample updated the model,
 * -1 if it was rejected.
 */
int pts_network_time_add_pair(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks);

/** Move locked -> holdover when pairs stop arriving. */
void pts_network_time_poll(void);

/** 0 if locked or holdover and *out is set. -1 otherwise. */
int pts_network_time_now(uint32_t *out);

pts_sync_quality_t pts_network_time_quality(void);
uint32_t pts_network_time_session(void);
uint32_t pts_network_time_rejects(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_NETWORK_TIME_H */
