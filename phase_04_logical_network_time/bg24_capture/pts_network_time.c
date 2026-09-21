/**
 * @file pts_network_time.c
 * @brief Offset + rate model. Integer only. No timer reset.
 *
 * Deltas use int32 subtraction so a uint32 rollover is fine as long as the
 * true interval fits in signed 32 bits (~55 s at 39 MHz).
 */
#include "pts_network_time.h"

#include "em_timer.h"
#include <stdio.h>

static uint32_t s_session;
static uint8_t s_have_session;
static uint8_t s_have_anchor;
static uint8_t s_have_rate;
static uint32_t s_anchor_m;
static uint32_t s_anchor_r;
static uint32_t s_last_seq;
static uint32_t s_last_pair_r;
static int64_t s_last_dr;
static int64_t s_rate_num;
static int64_t s_rate_den;
static uint32_t s_rejects;
static pts_sync_quality_t s_quality;

static const char *quality_name(pts_sync_quality_t q)
{
  switch (q) {
    case PTS_SYNC_ACQUIRING: return "acquiring";
    case PTS_SYNC_LOCKED:    return "locked";
    case PTS_SYNC_HOLDOVER:  return "holdover";
    default:                 return "unlocked";
  }
}

static void set_quality(pts_sync_quality_t q)
{
  if (s_quality == q) {
    return;
  }
  s_quality = q;
  printf("PTS net quality=%s session=%lu\n", quality_name(q), (unsigned long)s_session);
}

static void renorm(void)
{
  while ((s_rate_num > 0x3FFFFFFF) || (s_rate_den > 0x3FFFFFFF)) {
    s_rate_num >>= 1;
    s_rate_den >>= 1;
  }
  if (s_rate_den == 0) {
    s_rate_den = 1;
  }
}

void pts_network_time_init(void)
{
  s_quality = PTS_SYNC_UNLOCKED;
  s_rate_den = 1;
}

void pts_network_time_on_session(uint32_t session)
{
  if (s_have_session && (session == s_session)) {
    return;
  }
  s_session = session;
  s_have_session = 1u;
  s_have_anchor = 0u;
  s_have_rate = 0u;
  s_last_dr = 0;
  s_rate_num = 0;
  s_rate_den = 1;
  set_quality(PTS_SYNC_UNLOCKED);
  printf("PTS net session=%lu reset\n", (unsigned long)session);
}

int pts_network_time_add_pair(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks)
{
  int64_t dm;
  int64_t dr;

  if (!s_have_anchor) {
    s_anchor_m = m_ticks;
    s_anchor_r = r_ticks;
    s_last_seq = seq;
    s_last_pair_r = r_ticks;
    s_have_anchor = 1u;
    set_quality(PTS_SYNC_ACQUIRING);
    return 0;
  }

  dm = (int32_t)(m_ticks - s_anchor_m);
  dr = (int32_t)(r_ticks - s_anchor_r);
  if ((dm <= 0) || (dr <= 0) || ((int32_t)(seq - s_last_seq) <= 0)) {
    s_rejects++;
    return -1;
  }

  if (s_have_rate) {
    int64_t expect = dr * s_rate_num;
    int64_t got = dm * s_rate_den;
    int64_t err = got - expect;
    if (err < 0) {
      err = -err;
    }
    /* 12.5% residual against the smoothed rate. */
    if ((expect > 0) && (err > (expect / 8))) {
      s_rejects++;
      return -1;
    }
    s_rate_num = (s_rate_num * 7) + dm;
    s_rate_den = (s_rate_den * 7) + dr;
    renorm();
  } else {
    s_rate_num = dm;
    s_rate_den = dr;
    renorm();
    s_have_rate = 1u;
  }

  s_anchor_m = m_ticks;
  s_anchor_r = r_ticks;
  s_last_seq = seq;
  s_last_pair_r = r_ticks;
  s_last_dr = dr;
  set_quality(PTS_SYNC_LOCKED);
  return 0;
}

void pts_network_time_poll(void)
{
  int32_t gap;

  if ((s_quality != PTS_SYNC_LOCKED) || (s_last_dr <= 0)) {
    return;
  }
  gap = (int32_t)(TIMER_CounterGet(TIMER0) - s_last_pair_r);
  /* About eight sync intervals with no accepted pair. */
  if ((gap > 0) && ((int64_t)gap > (s_last_dr * 8))) {
    set_quality(PTS_SYNC_HOLDOVER);
  }
}

int pts_network_time_now(uint32_t *out)
{
  int64_t dr;
  int64_t m;

  if ((out == NULL) || !s_have_rate) {
    return -1;
  }
  if ((s_quality != PTS_SYNC_LOCKED) && (s_quality != PTS_SYNC_HOLDOVER)) {
    return -1;
  }
  dr = (int32_t)(TIMER_CounterGet(TIMER0) - s_anchor_r);
  m = (int64_t)s_anchor_m + ((dr * s_rate_num) / s_rate_den);
  *out = (uint32_t)m;
  return 0;
}

pts_sync_quality_t pts_network_time_quality(void)
{
  return s_quality;
}

uint32_t pts_network_time_session(void)
{
  return s_session;
}

uint32_t pts_network_time_rejects(void)
{
  return s_rejects;
}
