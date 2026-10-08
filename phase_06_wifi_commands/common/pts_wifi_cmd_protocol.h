/**
 * @file pts_wifi_cmd_protocol.h
 * @brief Phase 6 Wi-Fi command lines (ASCII over UDP).
 *
 * Host -> master :5005
 *   TIME
 *   ON[,<lead_ms>]
 *   OFF[,<lead_ms>]
 *   SCHED,<cmd_id>,<cmd_type>,<lead_ms>   (type 2=ON 3=OFF)
 *     -> OK,<cmd_id>,<exec_m>
 *
 * Host/master -> RX :5006
 *   PTS6,<cmd_id>,<cmd_type>,<exec_m>,0
 */
#ifndef PTS_WIFI_CMD_PROTOCOL_H
#define PTS_WIFI_CMD_PROTOCOL_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include "pts_schedule_protocol.h"

#define PTS_WIFI_MASTER_PORT     5005u
#define PTS_WIFI_RX_PORT         5006u
#define PTS_WIFI_LINE_MAX        96u
#define PTS_WIFI_JOIN_TIMEOUT_MS 25000u

static inline int pts_wifi_format_time_reply(char *out, size_t out_len, uint32_t m, uint32_t hz)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  return snprintf(out, out_len, "TIME,%lu,%lu\n", (unsigned long)m, (unsigned long)hz);
}

static inline int pts_wifi_format_ok(char *out, size_t out_len, uint32_t cmd_id, uint32_t exec_m)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  return snprintf(out, out_len, "OK,%lu,%lu\n", (unsigned long)cmd_id, (unsigned long)exec_m);
}

static inline int pts_wifi_format_pts6(char *out, size_t out_len,
                                       uint32_t cmd_id, uint8_t cmd_type,
                                       uint32_t exec_m, uint8_t status)
{
  if ((out == NULL) || (out_len < 16u)) {
    return -1;
  }
  return snprintf(out, out_len, "PTS6,%lu,%u,%lu,%u\n",
                  (unsigned long)cmd_id,
                  (unsigned)cmd_type,
                  (unsigned long)exec_m,
                  (unsigned)status);
}

static inline int pts_wifi_parse_pts6(const char *line,
                                      uint32_t *cmd_id,
                                      uint8_t *cmd_type,
                                      uint32_t *exec_m,
                                      uint8_t *status)
{
  unsigned long cid = 0;
  unsigned ctype = 0;
  unsigned long em = 0;
  unsigned st = 0;

  if ((line == NULL) || (cmd_id == NULL) || (cmd_type == NULL) || (exec_m == NULL) || (status == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if (sscanf(line, "PTS6,%lu,%u,%lu,%u", &cid, &ctype, &em, &st) != 4) {
    return -1;
  }
  *cmd_id   = (uint32_t)cid;
  *cmd_type = (uint8_t)ctype;
  *exec_m   = (uint32_t)em;
  *status   = (uint8_t)st;
  return 0;
}

static inline int pts_wifi_parse_sched(const char *line,
                                       uint32_t *cmd_id,
                                       uint8_t *cmd_type,
                                       uint32_t *lead_ms)
{
  unsigned long cid = 0;
  unsigned ctype = 0;
  unsigned long lead = 0;

  if ((line == NULL) || (cmd_id == NULL) || (cmd_type == NULL) || (lead_ms == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if (sscanf(line, "SCHED,%lu,%u,%lu", &cid, &ctype, &lead) != 3) {
    return -1;
  }
  *cmd_id   = (uint32_t)cid;
  *cmd_type = (uint8_t)ctype;
  *lead_ms  = (uint32_t)lead;
  return 0;
}

/** Parse ON / OFF / ON,<lead> / OFF,<lead>. Returns 0 ok. */
static inline int pts_wifi_parse_onoff(const char *line, uint8_t *cmd_type, uint32_t *lead_ms)
{
  unsigned long lead = PTS_SCHED_LEAD_MS;

  if ((line == NULL) || (cmd_type == NULL) || (lead_ms == NULL)) {
    return -1;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  if ((strncmp(line, "ON", 2) == 0) && ((line[2] == '\0') || (line[2] == ',') || isspace((unsigned char)line[2]))) {
    *cmd_type = PTS_SCHED_TYPE_GPIO_ON;
    if (line[2] == ',') {
      if (sscanf(line + 3, "%lu", &lead) != 1) {
        return -1;
      }
    }
    *lead_ms = (uint32_t)lead;
    return 0;
  }
  if ((strncmp(line, "OFF", 3) == 0) && ((line[3] == '\0') || (line[3] == ',') || isspace((unsigned char)line[3]))) {
    *cmd_type = PTS_SCHED_TYPE_GPIO_OFF;
    if (line[3] == ',') {
      if (sscanf(line + 4, "%lu", &lead) != 1) {
        return -1;
      }
    }
    *lead_ms = (uint32_t)lead;
    return 0;
  }
  return -1;
}

static inline int pts_wifi_is_time_req(const char *line)
{
  if (line == NULL) {
    return 0;
  }
  while ((*line == ' ') || (*line == '\t')) {
    line++;
  }
  return (strncmp(line, "TIME", 4) == 0) ? 1 : 0;
}

#endif /* PTS_WIFI_CMD_PROTOCOL_H */
