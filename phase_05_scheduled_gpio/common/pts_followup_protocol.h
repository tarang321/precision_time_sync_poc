/**
 * @file pts_followup_protocol.h
 * @brief Phase 4 trigger + K-history follow-up (loss-tolerant pairing).
 *
 * Periodic Adv packet N is the trigger. It carries M for seq N-1..N-K so a
 * pair survives up to K-1 consecutive air losses.
 *
 * M[N] is sampled on the Si917 in the app event that calls
 * rsi_ble_set_ae_data(), via osKernelGetSysTimerCount(). Firmware schedule
 * boundary — not a controller TX timestamp.
 *
 * AD (manufacturer-specific), 33 bytes:
 *   [0]      length = 32
 *   [1]      0xFF
 *   [2..3]   company id 0x02FF
 *   [4..7]   'P''T''S''5'
 *   [8..11]  trigger sequence N
 *   [12..15] master session id
 *   [16..19] M[N-1]  (0 if invalid)
 *   [20..23] M[N-2]
 *   [24..27] M[N-3]
 *   [28..31] M[N-4]
 *   [32]     valid_mask bit i => M[N-1-i] valid
 */
#ifndef PTS_FOLLOWUP_PROTOCOL_H
#define PTS_FOLLOWUP_PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define PTS_FU_AD_TYPE       0xFFu
#define PTS_FU_COMPANY_ID    0x02FFu
#define PTS_FU_MAGIC0        'P'
#define PTS_FU_MAGIC1        'T'
#define PTS_FU_MAGIC2        'S'
#define PTS_FU_MAGIC3        '5'
#define PTS_FU_HIST_K        4u
#define PTS_FU_PAYLOAD_LEN   33u
#define PTS_FU_SEQ_NONE      0xFFFFFFFFu

#define PTS_FU_LINE_MAX      96u
#define PTS_FU_PREFIX        "PTS5"
#define PTS_FU_BAUD          115200u

typedef struct {
  uint32_t seq;
  uint32_t m_ticks;
  uint8_t valid;
} pts_fu_hist_entry_t;

static inline void pts_fu_put_u32(uint8_t *p, uint32_t v)
{
  p[0] = (uint8_t)(v & 0xFFu);
  p[1] = (uint8_t)((v >> 8) & 0xFFu);
  p[2] = (uint8_t)((v >> 16) & 0xFFu);
  p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

static inline uint32_t pts_fu_get_u32(const uint8_t *p)
{
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

/**
 * Encode trigger N plus history of prior M samples.
 * hist_seq[i] / hist_m[i] are the i-th older sample (i=0 => N-1).
 * hist_valid_mask bit i set when that slot is valid.
 */
static inline void pts_fu_encode(uint8_t out[PTS_FU_PAYLOAD_LEN],
                                 uint32_t trigger_seq,
                                 const uint32_t hist_m[PTS_FU_HIST_K],
                                 uint8_t hist_valid_mask,
                                 uint32_t session)
{
  uint8_t i;

  memset(out, 0, PTS_FU_PAYLOAD_LEN);
  out[0] = (uint8_t)(PTS_FU_PAYLOAD_LEN - 1u);
  out[1] = PTS_FU_AD_TYPE;
  out[2] = (uint8_t)(PTS_FU_COMPANY_ID & 0xFFu);
  out[3] = (uint8_t)((PTS_FU_COMPANY_ID >> 8) & 0xFFu);
  out[4] = PTS_FU_MAGIC0;
  out[5] = PTS_FU_MAGIC1;
  out[6] = PTS_FU_MAGIC2;
  out[7] = PTS_FU_MAGIC3;
  pts_fu_put_u32(&out[8], trigger_seq);
  pts_fu_put_u32(&out[12], session);
  for (i = 0u; i < PTS_FU_HIST_K; i++) {
    uint32_t m = 0u;
    if ((hist_valid_mask & (uint8_t)(1u << i)) != 0u) {
      m = hist_m[i];
    }
    pts_fu_put_u32(&out[16u + (i * 4u)], m);
  }
  out[32] = hist_valid_mask;
}

static inline int pts_fu_decode(const uint8_t *data,
                                uint8_t data_len,
                                uint32_t *trigger_seq,
                                uint32_t *session,
                                pts_fu_hist_entry_t hist[PTS_FU_HIST_K])
{
  uint8_t i;
  uint8_t mask;
  uint32_t seq;

  if ((data == NULL) || (trigger_seq == NULL) || (session == NULL) || (hist == NULL)
      || (data_len < PTS_FU_PAYLOAD_LEN)) {
    return -1;
  }
  if ((data[0] != (uint8_t)(PTS_FU_PAYLOAD_LEN - 1u)) || (data[1] != PTS_FU_AD_TYPE)
      || (data[2] != (uint8_t)(PTS_FU_COMPANY_ID & 0xFFu))
      || (data[3] != (uint8_t)((PTS_FU_COMPANY_ID >> 8) & 0xFFu))
      || (data[4] != PTS_FU_MAGIC0) || (data[5] != PTS_FU_MAGIC1)
      || (data[6] != PTS_FU_MAGIC2) || (data[7] != PTS_FU_MAGIC3)) {
    return -1;
  }
  seq          = pts_fu_get_u32(&data[8]);
  *trigger_seq = seq;
  *session     = pts_fu_get_u32(&data[12]);
  mask         = data[32];
  for (i = 0u; i < PTS_FU_HIST_K; i++) {
    hist[i].valid   = ((mask & (uint8_t)(1u << i)) != 0u) ? 1u : 0u;
    hist[i].m_ticks = pts_fu_get_u32(&data[16u + (i * 4u)]);
    if (hist[i].valid && (seq > (uint32_t)i)) {
      hist[i].seq = seq - 1u - (uint32_t)i;
    } else {
      hist[i].valid = 0u;
      hist[i].seq   = PTS_FU_SEQ_NONE;
    }
  }
  return 0;
}

static inline int pts_fu_format_line(char *out,
                                     size_t out_len,
                                     uint8_t board_id,
                                     uint32_t trigger_seq,
                                     int8_t rssi,
                                     uint32_t fu_seq,
                                     uint32_t m_ticks,
                                     int fu_valid,
                                     uint32_t session)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  if (!fu_valid) {
    fu_seq  = PTS_FU_SEQ_NONE;
    m_ticks = 0u;
  }
  return snprintf(out,
                  out_len,
                  "%s,%u,%lu,%d,%lu,%lu,%lu\n",
                  PTS_FU_PREFIX,
                  (unsigned)board_id,
                  (unsigned long)trigger_seq,
                  (int)rssi,
                  (unsigned long)fu_seq,
                  (unsigned long)m_ticks,
                  (unsigned long)session);
}

#endif /* PTS_FOLLOWUP_PROTOCOL_H */
