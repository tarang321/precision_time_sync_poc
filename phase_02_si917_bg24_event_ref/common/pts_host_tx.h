/**
 * @file pts_host_tx.h
 * @brief Si917 soft-UART TX of event-identity frames to BG24.
 */
#ifndef PTS_HOST_TX_H
#define PTS_HOST_TX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Init soft-UART TX pin (idle high). Returns 0 on success. */
int pts_host_tx_init(void);

/** Send PTS2 identity frame for sequence N. Not ISR-safe (bit-bang delays). */
int pts_host_tx_send(uint8_t board_id, uint32_t seq, int8_t rssi);

#ifdef __cplusplus
}
#endif

#endif /* PTS_HOST_TX_H */
