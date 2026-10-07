/**
 * @file pts_event_ref_protocol.h
 * @brief Parked BG24 identity-frame format (unused in Si917-only Phase 2).
 *
 * Active Phase 2 path: sample R[N] = osKernelGetSysTimerCount() on RX,
 * pulse GPIO6, log seq <-> R on VCOM.
 *
 * Legacy wire ASCII (115200 8N1), kept for reference:
 *   PTS2,<board_id>,<seq>,<rssi>\n
 */
#ifndef PTS_EVENT_REF_PROTOCOL_H
#define PTS_EVENT_REF_PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define PTS_EVENT_REF_BAUD           115200u
#define PTS_EVENT_REF_LINE_MAX       48u
#define PTS_EVENT_REF_PREFIX         "PTS2"

static inline int pts_event_ref_format(char *out, size_t out_len,
                                       uint8_t board_id, uint32_t seq, int8_t rssi)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  return snprintf(out, out_len, "%s,%u,%lu,%d\n",
                  PTS_EVENT_REF_PREFIX,
                  (unsigned)board_id,
                  (unsigned long)seq,
                  (int)rssi);
}

static inline int pts_event_ref_parse(const char *line,
                                      uint8_t *board_id_out,
                                      uint32_t *seq_out,
                                      int8_t *rssi_out)
{
  unsigned bid = 0;
  unsigned long seq = 0;
  int rssi = 0;

  if ((line == NULL) || (board_id_out == NULL) || (seq_out == NULL) || (rssi_out == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if (sscanf(line, "PTS2,%u,%lu,%d", &bid, &seq, &rssi) != 3) {
    return -1;
  }
  *board_id_out = (uint8_t)bid;
  *seq_out      = (uint32_t)seq;
  *rssi_out     = (int8_t)rssi;
  return 0;
}

#endif /* PTS_EVENT_REF_PROTOCOL_H */
