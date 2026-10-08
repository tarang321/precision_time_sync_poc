/**
 * @file pts_schedule_protocol.h
 * @brief Phase 6: Wi-Fi command on master → BLE PTS6 AD to RX (like P5).
 *
 * AD after PTS5 follow-up on Periodic Advertising, 20 bytes, magic PTS6.
 * cmd_type: 2=GPIO ON, 3=GPIO OFF (active-low LED1).
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
#define PTS_SCHED_MAGIC3         '6'
#define PTS_SCHED_PAYLOAD_LEN    20u

#define PTS_SCHED_TYPE_GPIO_TOGGLE 1u
#define PTS_SCHED_TYPE_GPIO_ON     2u
#define PTS_SCHED_TYPE_GPIO_OFF    3u
#define PTS_SCHED_STATUS_VALID     0u

/** Lead before exec — must cover PTS6 air repeats + Wi-Fi/BLE coex + miss. */
#define PTS_SCHED_LEAD_MS          500u
#define PTS_SCHED_REPEAT_COUNT     8u

/**
 * Master fires at exec_m (network clock = master SysTimer). Keep 0.
 * RX subtracts PATH_COMP from to_local() to cancel one-way (set_ae_data → RX cb).
 * RX-early → lower PATH_COMP (fire later). RX-late → raise it.
 * 30 ms over-corrected (RX a few ms early); 26 ms is one PA minus ~4 ms.
 */
#ifndef PTS_SCHED_MASTER_COMP_MS
#define PTS_SCHED_MASTER_COMP_MS   0u
#endif
#ifndef PTS_SCHED_PATH_COMP_MS
#define PTS_SCHED_PATH_COMP_MS     26u
#endif

/** ms → SysTimer ticks without uint32 overflow (freq ~180 MHz). */
static inline uint32_t pts_ms_to_systimer_ticks(uint32_t ms, uint32_t freq_hz)
{
  return (uint32_t)(((uint64_t)freq_hz * (uint64_t)ms) / 1000ull);
}

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

static inline int pts_sched_decode(const uint8_t *data,
                                   uint8_t data_len,
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

#endif /* PTS_SCHEDULE_PROTOCOL_H */
