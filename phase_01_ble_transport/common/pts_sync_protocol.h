/**
 * @file pts_sync_protocol.h
 * @brief Phase 1 Periodic Advertising payload layout (master <-> receiver).
 *
 * AD structure (manufacturer-specific):
 *   [0] length = PTS_SYNC_AD_LEN - 1
 *   [1] AD type = 0xFF
 *   [2..3] company ID (little-endian), Silicon Labs 0x02FF placeholder for POC
 *   [4..7] magic 'P''T''S''1'
 *   [8..11] sequence N (little-endian uint32)
 */
#ifndef PTS_SYNC_PROTOCOL_H
#define PTS_SYNC_PROTOCOL_H

#include <stdint.h>
#include <string.h>

#define PTS_SYNC_AD_TYPE           0xFFu
#define PTS_SYNC_COMPANY_ID        0x02FFu /* Silabs CID; replace if required */
#define PTS_SYNC_MAGIC0            'P'
#define PTS_SYNC_MAGIC1            'T'
#define PTS_SYNC_MAGIC2            'S'
#define PTS_SYNC_MAGIC3            '1'
#define PTS_SYNC_PAYLOAD_LEN       12u
#define PTS_SYNC_AD_LEN            (PTS_SYNC_PAYLOAD_LEN) /* length field + rest */

/* Periodic interval: N * 1.25 ms. 0x10=20ms, 0x14=25ms. */
/* BLE units: N * 1.25 ms. Lock min=max so seq timer can track air time. */
#define PTS_PERIODIC_INTERVAL_MIN  0x10u /* 20 ms */
#define PTS_PERIODIC_INTERVAL_MAX  0x10u /* 20 ms */
/* Master seq refresh must be >= PA interval or intermediate seqs never air. */
#define PTS_SEQ_TIMER_MS           25u

/* Master random address — receivers must sync to this (see ble_config.h REM_ADDR). */
#define PTS_MASTER_RAND_ADDR       "11:44:33:AA:BB:CC"
#define PTS_MASTER_LOCAL_NAME      "PTS_MASTER"
#define PTS_MASTER_ADV_SID         0x00u

static inline void pts_sync_encode(uint8_t out[PTS_SYNC_PAYLOAD_LEN], uint32_t seq)
{
  out[0] = (uint8_t)(PTS_SYNC_PAYLOAD_LEN - 1u);
  out[1] = PTS_SYNC_AD_TYPE;
  out[2] = (uint8_t)(PTS_SYNC_COMPANY_ID & 0xFFu);
  out[3] = (uint8_t)((PTS_SYNC_COMPANY_ID >> 8) & 0xFFu);
  out[4] = PTS_SYNC_MAGIC0;
  out[5] = PTS_SYNC_MAGIC1;
  out[6] = PTS_SYNC_MAGIC2;
  out[7] = PTS_SYNC_MAGIC3;
  out[8]  = (uint8_t)(seq & 0xFFu);
  out[9]  = (uint8_t)((seq >> 8) & 0xFFu);
  out[10] = (uint8_t)((seq >> 16) & 0xFFu);
  out[11] = (uint8_t)((seq >> 24) & 0xFFu);
}

static inline int pts_sync_decode(const uint8_t *data, uint8_t data_len, uint32_t *seq_out)
{
  if ((data == NULL) || (seq_out == NULL) || (data_len < PTS_SYNC_PAYLOAD_LEN)) {
    return -1;
  }
  if ((data[0] != (uint8_t)(PTS_SYNC_PAYLOAD_LEN - 1u)) || (data[1] != PTS_SYNC_AD_TYPE)
      || (data[2] != (uint8_t)(PTS_SYNC_COMPANY_ID & 0xFFu))
      || (data[3] != (uint8_t)((PTS_SYNC_COMPANY_ID >> 8) & 0xFFu))
      || (data[4] != PTS_SYNC_MAGIC0) || (data[5] != PTS_SYNC_MAGIC1)
      || (data[6] != PTS_SYNC_MAGIC2) || (data[7] != PTS_SYNC_MAGIC3)) {
    return -1;
  }
  *seq_out = (uint32_t)data[8] | ((uint32_t)data[9] << 8) | ((uint32_t)data[10] << 16)
             | ((uint32_t)data[11] << 24);
  return 0;
}

#endif /* PTS_SYNC_PROTOCOL_H */
