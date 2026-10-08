/**
 * @file pts_network_time.c
 * @brief Offset + rate model on Si917 SysTimer. Integer only. No timer reset.
 *
 * Hardening:
 * - Holdover keyed off last RX activity, not only accepted fits.
 * - uint32 wrap-safe int64 deltas (avoids int32 cast break ~12 s @ 180 MHz).
 * - Rate fit against long-baseline anchor (~1 s / PTS_NT_RATE_MIN_DSEQ).
 * - Separate offset slide for pts_network_time_now().
 * - After a seq gap, residual failure slides offset — does not poison rate.
 */
#include "pts_network_time.h"
#include "cmsis_os2.h"

#ifndef PTS_NT_HOLDOVER_MULT
#define PTS_NT_HOLDOVER_MULT 32
#endif
#ifndef PTS_NT_GAP_SLIDE
#define PTS_NT_GAP_SLIDE 8
#endif
/* ~1 s at 30 ms seq — rate updates use this span to dilute M-sample jitter. */
#ifndef PTS_NT_RATE_MIN_DSEQ
#define PTS_NT_RATE_MIN_DSEQ 32
#endif
/* Residual |dm*den - dr*num| > expect/N fails. 4 = 25% (SW M/R jitter). */
#ifndef PTS_NT_RESID_DIV
#define PTS_NT_RESID_DIV 4
#endif

static uint32_t s_session;
static uint8_t s_have_session;
static uint8_t s_have_anchor;
static uint8_t s_have_rate;
static uint8_t s_rate_confirmed;
static uint32_t s_offset_m;
static uint32_t s_offset_r;
static uint32_t s_rate_m0;
static uint32_t s_rate_r0;
static uint32_t s_rate_seq0;
static uint32_t s_last_seq;
static uint32_t s_last_pair_r;
static uint32_t s_last_rx_r;
static int64_t s_last_dr;
static int64_t s_rate_num;
static int64_t s_rate_den;
static uint32_t s_rejects;
static uint32_t s_accepts;
static pts_sync_quality_t s_quality;

/** Unsigned 32-bit difference as non-negative int64 (wrap-safe). */
static int64_t u32_delta(uint32_t newer, uint32_t older)
{
  return (int64_t)(uint32_t)(newer - older);
}

/** Signed wrap-safe Δ: reinterpret uint32 subtract as int32, widen to int64. */
static int64_t u32_signed_delta(uint32_t a, uint32_t b)
{
  return (int64_t)(int32_t)(uint32_t)(a - b);
}

const char *pts_network_time_quality_name(pts_sync_quality_t q)
{
  switch (q) {
    case PTS_SYNC_ACQUIRING:
      return "acquiring";
    case PTS_SYNC_LOCKED:
      return "locked";
    case PTS_SYNC_HOLDOVER:
      return "holdover";
    default:
      return "unlocked";
  }
}

static void set_quality(pts_sync_quality_t q)
{
  s_quality = q;
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

static void slide_offset(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks)
{
  s_offset_m    = m_ticks;
  s_offset_r    = r_ticks;
  s_last_seq    = seq;
  s_last_pair_r = r_ticks;
  s_last_rx_r   = r_ticks;
}

/** 1 if |dm/dr - num/den| within 1/PTS_NT_RESID_DIV. */
static int residual_ok(int64_t dm, int64_t dr)
{
  int64_t expect;
  int64_t got;
  int64_t err;

  if ((dm <= 0) || (dr <= 0) || (s_rate_den == 0)) {
    return 0;
  }
  expect = dr * s_rate_num;
  got    = dm * s_rate_den;
  err    = got - expect;
  if (err < 0) {
    err = -err;
  }
  if (expect <= 0) {
    return 0;
  }
  return (err <= (expect / (int64_t)PTS_NT_RESID_DIV)) ? 1 : 0;
}

void pts_network_time_init(void)
{
  s_quality      = PTS_SYNC_UNLOCKED;
  s_have_session = 0u;
  s_have_anchor     = 0u;
  s_have_rate       = 0u;
  s_rate_confirmed  = 0u;
  s_rate_num        = 0;
  s_rate_den     = 1;
  s_rejects      = 0u;
  s_accepts      = 0u;
  s_last_dr      = 0;
  s_session      = 0u;
  s_offset_m     = 0u;
  s_offset_r     = 0u;
  s_rate_m0      = 0u;
  s_rate_r0      = 0u;
  s_rate_seq0    = 0u;
  s_last_seq     = 0u;
  s_last_pair_r  = 0u;
  s_last_rx_r    = 0u;
}

void pts_network_time_on_session(uint32_t session)
{
  if (s_have_session && (session == s_session)) {
    return;
  }
  s_session      = session;
  s_have_session = 1u;
  s_have_anchor    = 0u;
  s_have_rate      = 0u;
  s_rate_confirmed = 0u;
  s_last_dr        = 0;
  s_rate_num     = 0;
  s_rate_den     = 1;
  s_accepts      = 0u;
  set_quality(PTS_SYNC_UNLOCKED);
}

void pts_network_time_note_rx(uint32_t r_ticks)
{
  s_last_rx_r = r_ticks;
  if (s_quality == PTS_SYNC_HOLDOVER && s_rate_confirmed) {
    set_quality(PTS_SYNC_LOCKED);
  }
}

int pts_network_time_add_pair(uint32_t seq, uint32_t m_ticks, uint32_t r_ticks)
{
  int64_t dm;
  int64_t dr;
  int32_t dseq;

  s_last_rx_r = r_ticks;

  if (!s_have_anchor) {
    slide_offset(seq, m_ticks, r_ticks);
    s_rate_m0     = m_ticks;
    s_rate_r0     = r_ticks;
    s_rate_seq0   = seq;
    s_have_anchor = 1u;
    s_accepts++;
    set_quality(PTS_SYNC_ACQUIRING);
    return 0;
  }

  dseq = (int32_t)(seq - s_last_seq);
  if (dseq <= 0) {
    s_rejects++;
    return -1;
  }

  dm = u32_delta(m_ticks, s_offset_m);
  dr = u32_delta(r_ticks, s_offset_r);
  if ((dm <= 0) || (dr <= 0)) {
    s_rejects++;
    return -1;
  }

  if (!s_have_rate) {
    int32_t dseq_rate = (int32_t)(seq - s_rate_seq0);
    int64_t dm_r      = u32_delta(m_ticks, s_rate_m0);
    int64_t dr_r      = u32_delta(r_ticks, s_rate_r0);

    if ((dseq_rate <= 0) || (dm_r <= 0) || (dr_r <= 0)) {
      s_rejects++;
      return -1;
    }
    s_rate_num  = dm_r;
    s_rate_den  = dr_r;
    renorm();
    s_have_rate = 1u;
    slide_offset(seq, m_ticks, r_ticks);
    s_last_dr = dr;
    s_accepts++;
    if (dseq_rate >= (int32_t)PTS_NT_RATE_MIN_DSEQ) {
      s_rate_confirmed = 1u;
      set_quality(PTS_SYNC_LOCKED);
    } else {
      set_quality(PTS_SYNC_ACQUIRING);
    }
    return 0;
  }

  {
    int32_t dseq_rate = (int32_t)(seq - s_rate_seq0);
    int64_t dm_r      = u32_delta(m_ticks, s_rate_m0);
    int64_t dr_r      = u32_delta(r_ticks, s_rate_r0);

    if ((dseq_rate >= (int32_t)PTS_NT_RATE_MIN_DSEQ) && (dm_r > 0) && (dr_r > 0)) {
      if (!residual_ok(dm_r, dr_r)) {
        s_rejects++;
        if (!s_rate_confirmed) {
          /* 30 ms seed was junk — replace with this ~1 s window. */
          s_rate_num   = dm_r;
          s_rate_den   = dr_r;
          renorm();
          s_rate_m0    = m_ticks;
          s_rate_r0    = r_ticks;
          s_rate_seq0  = seq;
          slide_offset(seq, m_ticks, r_ticks);
          set_quality(PTS_SYNC_ACQUIRING);
          return 0;
        }
        if (dseq >= (int32_t)PTS_NT_GAP_SLIDE) {
          slide_offset(seq, m_ticks, r_ticks);
        }
        return -1;
      }
      s_rate_num = (s_rate_num * 15) + dm_r;
      s_rate_den = (s_rate_den * 15) + dr_r;
      renorm();
      s_rate_m0        = m_ticks;
      s_rate_r0        = r_ticks;
      s_rate_seq0      = seq;
      s_rate_confirmed = 1u;
    } else {
      if (!residual_ok(dm, dr)) {
        s_rejects++;
        return -1;
      }
    }
  }

  slide_offset(seq, m_ticks, r_ticks);
  s_last_dr = dr;
  s_accepts++;
  if (s_rate_confirmed) {
    set_quality(PTS_SYNC_LOCKED);
  } else {
    set_quality(PTS_SYNC_ACQUIRING);
  }
  return 0;
}

void pts_network_time_poll(void)
{
  int64_t gap;
  int64_t lim;

  if (((s_quality != PTS_SYNC_LOCKED) && (s_quality != PTS_SYNC_HOLDOVER)) || (s_last_dr <= 0)) {
    return;
  }
  gap = u32_delta(osKernelGetSysTimerCount(), s_last_rx_r);
  lim = s_last_dr * (int64_t)PTS_NT_HOLDOVER_MULT;
  if ((gap > 0) && (gap > lim)) {
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
  dr   = u32_delta(osKernelGetSysTimerCount(), s_offset_r);
  m    = (int64_t)s_offset_m + ((dr * s_rate_num) / s_rate_den);
  *out = (uint32_t)m;
  return 0;
}

int pts_network_time_to_local(uint32_t m_ticks, uint32_t *r_out)
{
  int64_t dm;
  int64_t dr;
  uint32_t m0;
  uint32_t r0;

  if ((r_out == NULL) || !s_have_rate || (s_rate_num == 0)) {
    return -1;
  }
  if ((s_quality != PTS_SYNC_LOCKED) && (s_quality != PTS_SYNC_HOLDOVER)) {
    return -1;
  }
  /* Use ~1 s rate origin after lock — last pair jitters a few ms. */
  if (s_rate_confirmed) {
    m0 = s_rate_m0;
    r0 = s_rate_r0;
  } else {
    m0 = s_offset_m;
    r0 = s_offset_r;
  }
  dm     = u32_signed_delta(m_ticks, m0);
  dr     = (dm * s_rate_den) / s_rate_num;
  *r_out = r0 + (uint32_t)dr;
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

uint32_t pts_network_time_accepts(void)
{
  return s_accepts;
}
