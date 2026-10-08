/**
 * @file pts_network_time.h
 * @brief Phase 4 logical network time on Si917 SysTimer domain.
 *
 * Network time = M0 + (rate_num/rate_den) * (local_now - R0)
 * Local clock is never reset. M and R are both osKernelGetSysTimerCount().
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
 * Note that an AE report was processed (any decode). Keeps holdover from
 * firing during residual rejects while the PA train is still alive.
 */
void pts_network_time_note_rx(uint32_t r_ticks);

/**
 * One follow-up pair. Returns 0 if model updated (accept or anchor slide),
 * -1 if rejected.
 */
int pts_network_time_add_pair(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks);

/** Move locked -> holdover when RX activity stops. */
void pts_network_time_poll(void);

/** 0 if locked or holdover and *out is set. -1 otherwise. */
int pts_network_time_now(uint32_t *out);

pts_sync_quality_t pts_network_time_quality(void);
const char *pts_network_time_quality_name(pts_sync_quality_t q);
uint32_t pts_network_time_session(void);
uint32_t pts_network_time_rejects(void);
uint32_t pts_network_time_accepts(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_NETWORK_TIME_H */
