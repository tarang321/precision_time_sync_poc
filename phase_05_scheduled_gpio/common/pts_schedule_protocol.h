/**
 * @file pts_schedule_protocol.h
 * @brief Phase 5 future-timestamp GPIO command (roadmap scheduled-execution).
 *
 * Appended after the PTS4 follow-up AD on Periodic Advertising when a
 * schedule is active. Same command id is repeated; receivers execute once.
 *
 * AD (manufacturer-specific), 20 bytes:
 *   [0]      length = 19
 *   [1]      0xFF
 *   [2..3]   company id 0x02FF
 *   [4..7]   'P''T''S''5'
 *   [8..11]  command id
 *   [12]     command type (1 = GPIO toggle)
 *   [13]     status (0 = valid)
 *   [14..17] future execution network time M (SysTimer counts)
 *   [18..19] reserved
 *
 * Host line (Si917 -> BG24 UART):
 *   PTS5,<board>,<cmd_id>,<cmd_type>,<exec_m>,<status>\n
 */
#ifndef PTS_SCHEDULE_PROTOCOL_H
#define PTS_SCHEDULE_PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define PTS_SCHED_AD_TYPE        0xFFu
#define PTS_SCHED_COMPANY_ID     0x02FFu
#define PTS_SCHED_MAGIC0         'P'
#define PTS_SCHED_MAGIC1         'T'
#define PTS_SCHED_MAGIC2         'S'
#define PTS_SCHED_MAGIC3         '5'
#define PTS_SCHED_PAYLOAD_LEN    20u

#define PTS_SCHED_TYPE_GPIO_TOGGLE 1u
#define PTS_SCHED_STATUS_VALID     0u

#define PTS_SCHED_LINE_MAX       80u
#define PTS_SCHED_PREFIX         "PTS5"
#define PTS_SCHED_LEAD_MS        100u
#define PTS_SCHED_REPEAT_COUNT   4u
/** Packets between new command starts (~500 ms at 20 ms interval). */
#define PTS_SCHED_PERIOD_PKTS    25u
/** Packets to wait after boot before first schedule (allow lock). */
#define PTS_SCHED_BOOT_DELAY_PKTS 50u

static inline void pts_sched_encode(uint8_t out[PTS_SCHED_PAYLOAD_LEN],
                                    uint32_t cmd_id,
                                    uint8_t cmd_type,
                                    uint8_t status,
                                    uint32_t exec_m)
{
  out[0] = (uint8_t)(PTS_SCHED_PAYLOAD_LEN - 1u);
  out[1] = PTS_SCHED_AD_TYPE;
  out[2] = (uint8_t)(PTS_SCHED_COMPANY_ID & 0xFFu);
  out[3] = (uint8_t)((PTS_SCHED_COMPANY_ID >> 8) & 0xFFu);
  out[4] = PTS_SCHED_MAGIC0;
  out[5] = PTS_SCHED_MAGIC1;
  out[6] = PTS_SCHED_MAGIC2;
  out[7] = PTS_SCHED_MAGIC3;
  out[8]  = (uint8_t)(cmd_id & 0xFFu);
  out[9]  = (uint8_t)((cmd_id >> 8) & 0xFFu);
  out[10] = (uint8_t)((cmd_id >> 16) & 0xFFu);
  out[11] = (uint8_t)((cmd_id >> 24) & 0xFFu);
  out[12] = cmd_type;
  out[13] = status;
  out[14] = (uint8_t)(exec_m & 0xFFu);
  out[15] = (uint8_t)((exec_m >> 8) & 0xFFu);
  out[16] = (uint8_t)((exec_m >> 16) & 0xFFu);
  out[17] = (uint8_t)((exec_m >> 24) & 0xFFu);
  out[18] = 0u;
  out[19] = 0u;
}

static inline int pts_sched_decode(const uint8_t *data, uint8_t data_len,
                                   uint32_t *cmd_id,
                                   uint8_t *cmd_type,
                                   uint8_t *status,
                                   uint32_t *exec_m)
{
  if ((data == NULL) || (cmd_id == NULL) || (cmd_type == NULL) || (status == NULL)
      || (exec_m == NULL) || (data_len < PTS_SCHED_PAYLOAD_LEN)) {
    return -1;
  }
  if ((data[1] != PTS_SCHED_AD_TYPE) || (data[4] != PTS_SCHED_MAGIC0) || (data[5] != PTS_SCHED_MAGIC1)
      || (data[6] != PTS_SCHED_MAGIC2) || (data[7] != PTS_SCHED_MAGIC3)) {
    return -1;
  }
  *cmd_id   = (uint32_t)data[8] | ((uint32_t)data[9] << 8) | ((uint32_t)data[10] << 16)
              | ((uint32_t)data[11] << 24);
  *cmd_type = data[12];
  *status   = data[13];
  *exec_m   = (uint32_t)data[14] | ((uint32_t)data[15] << 8) | ((uint32_t)data[16] << 16)
              | ((uint32_t)data[17] << 24);
  return 0;
}

static inline int pts_sched_format_line(char *out, size_t out_len,
                                        uint8_t board_id,
                                        uint32_t cmd_id,
                                        uint8_t cmd_type,
                                        uint32_t exec_m,
                                        uint8_t status)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  return snprintf(out, out_len, "%s,%u,%lu,%u,%lu,%u\n",
                  PTS_SCHED_PREFIX,
                  (unsigned)board_id,
                  (unsigned long)cmd_id,
                  (unsigned)cmd_type,
                  (unsigned long)exec_m,
                  (unsigned)status);
}

static inline int pts_sched_parse_line(const char *line,
                                       uint8_t *board_id,
                                       uint32_t *cmd_id,
                                       uint8_t *cmd_type,
                                       uint32_t *exec_m,
                                       uint8_t *status)
{
  unsigned bid = 0;
  unsigned long cid = 0;
  unsigned ctype = 0;
  unsigned long em = 0;
  unsigned st = 0;

  if ((line == NULL) || (board_id == NULL) || (cmd_id == NULL) || (cmd_type == NULL)
      || (exec_m == NULL) || (status == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if (sscanf(line, "PTS5,%u,%lu,%u,%lu,%u", &bid, &cid, &ctype, &em, &st) != 5) {
    return -1;
  }
  *board_id = (uint8_t)bid;
  *cmd_id   = (uint32_t)cid;
  *cmd_type = (uint8_t)ctype;
  *exec_m   = (uint32_t)em;
  *status   = (uint8_t)st;
  return 0;
}

#endif /* PTS_SCHEDULE_PROTOCOL_H */
