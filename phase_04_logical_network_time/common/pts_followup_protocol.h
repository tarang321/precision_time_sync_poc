/**
 * @file pts_followup_protocol.h
 * @brief Phase 3 trigger + follow-up (roadmap section 5.2).
 *
 * Periodic Adv packet N is the trigger. It also carries the follow-up for
 * the previous trigger: referenced seq and master timestamp M.
 *
 * M[N] is sampled on the Si917 in the app event that calls
 * rsi_ble_set_ae_data(), via osKernelGetSysTimerCount(). That is the
 * firmware schedule boundary, not a controller TX timestamp. Si917 does
 * not expose one. A fixed offset vs the radio edge can be calibrated;
 * jitter in this boundary cannot.
 *
 * Phase 4 adds a master session id so BG24 can drop its clock model on restart.
 *
 * AD (manufacturer-specific), 24 bytes:
 *   [0]      length = 23
 *   [1]      0xFF
 *   [2..3]   company id 0x02FF
 *   [4..7]   'P''T''S''4'
 *   [8..11]  trigger sequence N
 *   [12..15] follow-up sequence, or PTS_FU_SEQ_NONE
 *   [16..19] M[follow-up] SysTimer counts, little-endian
 *   [20..23] master session id
 *
 * Host line (after the GPIO edge, does not define R[N]):
 *   PTS4,<board>,<trigger_seq>,<rssi>,<fu_seq>,<M>,<session>\n
 * fu_seq is 4294967295 when the packet has no follow-up.
 */
#ifndef PTS_FOLLOWUP_PROTOCOL_H
#define PTS_FOLLOWUP_PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define PTS_FU_AD_TYPE       0xFFu
#define PTS_FU_COMPANY_ID    0x02FFu
#define PTS_FU_MAGIC0        'P'
#define PTS_FU_MAGIC1        'T'
#define PTS_FU_MAGIC2        'S'
#define PTS_FU_MAGIC3        '4'
#define PTS_FU_PAYLOAD_LEN   24u
#define PTS_FU_SEQ_NONE      0xFFFFFFFFu

#define PTS_FU_LINE_MAX      96u
#define PTS_FU_PREFIX        "PTS4"
#define PTS_FU_BAUD          115200u

static inline void pts_fu_encode(uint8_t out[PTS_FU_PAYLOAD_LEN],
                                 uint32_t trigger_seq,
                                 uint32_t fu_seq,
                                 uint32_t m_ticks,
                                 int fu_valid,
                                 uint32_t session)
{
  out[0] = (uint8_t)(PTS_FU_PAYLOAD_LEN - 1u);
  out[1] = PTS_FU_AD_TYPE;
  out[2] = (uint8_t)(PTS_FU_COMPANY_ID & 0xFFu);
  out[3] = (uint8_t)((PTS_FU_COMPANY_ID >> 8) & 0xFFu);
  out[4] = PTS_FU_MAGIC0;
  out[5] = PTS_FU_MAGIC1;
  out[6] = PTS_FU_MAGIC2;
  out[7] = PTS_FU_MAGIC3;
  out[8]  = (uint8_t)(trigger_seq & 0xFFu);
  out[9]  = (uint8_t)((trigger_seq >> 8) & 0xFFu);
  out[10] = (uint8_t)((trigger_seq >> 16) & 0xFFu);
  out[11] = (uint8_t)((trigger_seq >> 24) & 0xFFu);
  if (!fu_valid) {
    fu_seq  = PTS_FU_SEQ_NONE;
    m_ticks = 0u;
  }
  out[12] = (uint8_t)(fu_seq & 0xFFu);
  out[13] = (uint8_t)((fu_seq >> 8) & 0xFFu);
  out[14] = (uint8_t)((fu_seq >> 16) & 0xFFu);
  out[15] = (uint8_t)((fu_seq >> 24) & 0xFFu);
  out[16] = (uint8_t)(m_ticks & 0xFFu);
  out[17] = (uint8_t)((m_ticks >> 8) & 0xFFu);
  out[18] = (uint8_t)((m_ticks >> 16) & 0xFFu);
  out[19] = (uint8_t)((m_ticks >> 24) & 0xFFu);
  out[20] = (uint8_t)(session & 0xFFu);
  out[21] = (uint8_t)((session >> 8) & 0xFFu);
  out[22] = (uint8_t)((session >> 16) & 0xFFu);
  out[23] = (uint8_t)((session >> 24) & 0xFFu);
}

static inline int pts_fu_decode(const uint8_t *data, uint8_t data_len,
                                uint32_t *trigger_seq,
                                uint32_t *fu_seq,
                                uint32_t *m_ticks,
                                int *fu_valid,
                                uint32_t *session)
{
  uint32_t seq;
  uint32_t fu;
  uint32_t m;

  if ((data == NULL) || (trigger_seq == NULL) || (fu_seq == NULL) || (m_ticks == NULL)
      || (fu_valid == NULL) || (session == NULL) || (data_len < PTS_FU_PAYLOAD_LEN)) {
    return -1;
  }
  if ((data[1] != PTS_FU_AD_TYPE) || (data[4] != PTS_FU_MAGIC0) || (data[5] != PTS_FU_MAGIC1)
      || (data[6] != PTS_FU_MAGIC2) || (data[7] != PTS_FU_MAGIC3)) {
    return -1;
  }
  seq = (uint32_t)data[8] | ((uint32_t)data[9] << 8) | ((uint32_t)data[10] << 16) | ((uint32_t)data[11] << 24);
  fu  = (uint32_t)data[12] | ((uint32_t)data[13] << 8) | ((uint32_t)data[14] << 16) | ((uint32_t)data[15] << 24);
  m   = (uint32_t)data[16] | ((uint32_t)data[17] << 8) | ((uint32_t)data[18] << 16) | ((uint32_t)data[19] << 24);
  *trigger_seq = seq;
  *fu_seq      = fu;
  *m_ticks     = m;
  *fu_valid    = (fu != PTS_FU_SEQ_NONE) ? 1 : 0;
  *session     = (uint32_t)data[20] | ((uint32_t)data[21] << 8) | ((uint32_t)data[22] << 16)
                 | ((uint32_t)data[23] << 24);
  return 0;
}

static inline int pts_fu_format_line(char *out, size_t out_len,
                                     uint8_t board_id, uint32_t trigger_seq, int8_t rssi,
                                     uint32_t fu_seq, uint32_t m_ticks, int fu_valid,
                                     uint32_t session)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  if (!fu_valid) {
    fu_seq  = PTS_FU_SEQ_NONE;
    m_ticks = 0u;
  }
  return snprintf(out, out_len, "%s,%u,%lu,%d,%lu,%lu,%lu\n",
                  PTS_FU_PREFIX,
                  (unsigned)board_id,
                  (unsigned long)trigger_seq,
                  (int)rssi,
                  (unsigned long)fu_seq,
                  (unsigned long)m_ticks,
                  (unsigned long)session);
}

static inline int pts_fu_parse_line(const char *line,
                                    uint8_t *board_id,
                                    uint32_t *trigger_seq,
                                    int8_t *rssi,
                                    uint32_t *fu_seq,
                                    uint32_t *m_ticks,
                                    int *fu_valid,
                                    uint32_t *session)
{
  unsigned bid = 0;
  unsigned long seq = 0;
  unsigned long fu = 0;
  unsigned long m = 0;
  int rssi_i = 0;

  unsigned long sess = 0;

  if ((line == NULL) || (board_id == NULL) || (trigger_seq == NULL) || (rssi == NULL)
      || (fu_seq == NULL) || (m_ticks == NULL) || (fu_valid == NULL) || (session == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if (sscanf(line, "PTS4,%u,%lu,%d,%lu,%lu,%lu", &bid, &seq, &rssi_i, &fu, &m, &sess) != 6) {
    return -1;
  }
  *board_id    = (uint8_t)bid;
  *trigger_seq = (uint32_t)seq;
  *rssi        = (int8_t)rssi_i;
  *fu_seq      = (uint32_t)fu;
  *m_ticks     = (uint32_t)m;
  *session     = (uint32_t)sess;
  *fu_valid    = ((uint32_t)fu != PTS_FU_SEQ_NONE) ? 1 : 0;
  return 0;
}

#endif /* PTS_FOLLOWUP_PROTOCOL_H */
