/**
 * @file pts_network_time.c
 * @brief Offset + rate model. Integer only. No timer reset.
 *
 * Deltas use int32 subtraction so a uint32 rollover is fine as long as the
 * true interval fits in signed 32 bits (~55 s at 39 MHz).
 *
 * Rate: two-point slope over a long baseline (ref pair -> newest pair),
 * smoothed 7/8. Per-pair edge jitter divides by the baseline, so a 4 s
 * window turns ~100 us of callback jitter into ~25 ppm instead of the
 * ~1000 ppm a 20 ms pair-to-pair slope gives. The ref slides forward via a
 * checkpoint so the baseline stays within [1x, 2x] target and inside int32.
 *
 * Offset: the anchor is not jumped to each raw pair. The new pair is
 * predicted from the current model and the anchor moves by residual/4.
 * A single late callback therefore cannot land whole in to_local().
 */
#include "pts_network_time.h"

#include "em_cmu.h"
#include "em_timer.h"
#include <stdio.h>

/** Rate baseline target, seconds of TIMER0 time. 2x must stay < 2^31 ticks. */
#ifndef PTS_NET_RATE_BASELINE_S
#define PTS_NET_RATE_BASELINE_S   4u
#endif

/** Reject if |M - predicted M| > predicted dM since anchor >> this shift. */
#ifndef PTS_NET_RESID_GATE_SHIFT
#define PTS_NET_RESID_GATE_SHIFT  4   /* 6.25 % of one interval */
#endif

/** Anchor smoothing: anchor moves by residual / 2^shift per accepted pair. */
#ifndef PTS_NET_ANCHOR_GAIN_SHIFT
#define PTS_NET_ANCHOR_GAIN_SHIFT 2
#endif

static uint32_t s_session;
static uint8_t s_have_session;
static uint8_t s_have_anchor;
static uint8_t s_have_rate;
static uint8_t s_have_ckpt;
static uint32_t s_anchor_m;
static uint32_t s_anchor_r;
static uint32_t s_ref_m;
static uint32_t s_ref_r;
static uint32_t s_ckpt_m;
static uint32_t s_ckpt_r;
static uint32_t s_last_seq;
static uint32_t s_last_pair_r;
static int64_t s_last_dr;
static int64_t s_rate_num;
static int64_t s_rate_den;
static uint32_t s_baseline_ticks;
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

static void reset_fit(void)
{
  s_have_anchor = 0u;
  s_have_rate = 0u;
  s_have_ckpt = 0u;
  s_last_dr = 0;
  s_rate_num = 0;
  s_rate_den = 1;
}

/** Model prediction of network time at local tick r. Requires s_have_rate. */
static int64_t predict_m(uint32_t r_ticks)
{
  int64_t dr = (int32_t)(r_ticks - s_anchor_r);
  return (int64_t)s_anchor_m + ((dr * s_rate_num) / s_rate_den);
}

void pts_network_time_init(void)
{
  s_quality = PTS_SYNC_UNLOCKED;
  s_rate_den = 1;
  s_baseline_ticks = CMU_ClockFreqGet(cmuClock_TIMER0) * PTS_NET_RATE_BASELINE_S;
}

void pts_network_time_on_session(uint32_t session)
{
  if (s_have_session && (session == s_session)) {
    return;
  }
  s_session = session;
  s_have_session = 1u;
  reset_fit();
  set_quality(PTS_SYNC_UNLOCKED);
  printf("PTS net session=%lu reset\n", (unsigned long)session);
}

int pts_network_time_add_pair(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks)
{
  int64_t dm_ref;
  int64_t dr_ref;
  int64_t dr_pair;
  int64_t pred = 0;
  int64_t resid = 0;
  uint8_t had_rate = s_have_rate;

  if (!s_have_anchor) {
    s_anchor_m = m_ticks;
    s_anchor_r = r_ticks;
    s_ref_m = m_ticks;
    s_ref_r = r_ticks;
    s_last_seq = seq;
    s_last_pair_r = r_ticks;
    s_have_anchor = 1u;
    set_quality(PTS_SYNC_ACQUIRING);
    return 0;
  }

  dm_ref  = (int32_t)(m_ticks - s_ref_m);
  dr_ref  = (int32_t)(r_ticks - s_ref_r);
  dr_pair = (int32_t)(r_ticks - s_last_pair_r);
  if ((dm_ref <= 0) || (dr_ref <= 0) || (dr_pair <= 0) || ((int32_t)(seq - s_last_seq) <= 0)) {
    s_rejects++;
    return -1;
  }

  if (s_have_rate) {
    int64_t gate;
    pred  = predict_m(r_ticks);
    resid = (int32_t)(m_ticks - (uint32_t)pred);
    gate  = (pred - (int64_t)s_anchor_m) >> PTS_NET_RESID_GATE_SHIFT;
    if (gate < 1) {
      gate = 1;
    }
    if ((resid > gate) || (resid < -gate)) {
      s_rejects++;
      return -1;
    }
    s_rate_num = (s_rate_num * 7) + dm_ref;
    s_rate_den = (s_rate_den * 7) + dr_ref;
    renorm();
  } else {
    s_rate_num = dm_ref;
    s_rate_den = dr_ref;
    renorm();
    s_have_rate = 1u;
  }

  /* Slide the rate reference so the baseline stays within [1x, 2x] target. */
  if (!s_have_ckpt && (dr_ref >= (int64_t)s_baseline_ticks)) {
    s_ckpt_m = m_ticks;
    s_ckpt_r = r_ticks;
    s_have_ckpt = 1u;
  }
  if (dr_ref >= (int64_t)s_baseline_ticks * 2) {
    if (s_have_ckpt) {
      s_ref_m = s_ckpt_m;
      s_ref_r = s_ckpt_r;
    } else {
      s_ref_m = m_ticks;
      s_ref_r = r_ticks;
    }
    s_have_ckpt = 0u;
  }

  if (had_rate) {
    /* Division truncates toward zero, so +/- residuals are treated alike. */
    s_anchor_m = (uint32_t)(pred + (resid / (int64_t)(1 << PTS_NET_ANCHOR_GAIN_SHIFT)));
  } else {
    s_anchor_m = m_ticks;
  }
  s_anchor_r = r_ticks;
  s_last_seq = seq;
  s_last_pair_r = r_ticks;
  s_last_dr = dr_pair;
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
  if ((out == NULL) || !s_have_rate) {
    return -1;
  }
  if ((s_quality != PTS_SYNC_LOCKED) && (s_quality != PTS_SYNC_HOLDOVER)) {
    return -1;
  }
  *out = (uint32_t)predict_m(TIMER_CounterGet(TIMER0));
  return 0;
}

int pts_network_time_to_local(uint32_t m_ticks, uint32_t *r_out)
{
  int64_t dm;
  int64_t dr;

  if ((r_out == NULL) || !s_have_rate || (s_rate_num == 0)) {
    return -1;
  }
  if ((s_quality != PTS_SYNC_LOCKED) && (s_quality != PTS_SYNC_HOLDOVER)) {
    return -1;
  }
  dm = (int32_t)(m_ticks - s_anchor_m);
  dr = (dm * s_rate_den) / s_rate_num;
  *r_out = s_anchor_r + (uint32_t)dr;
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
