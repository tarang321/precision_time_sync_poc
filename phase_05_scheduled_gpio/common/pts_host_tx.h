/**
 * @file pts_host_tx.h
 * @brief Si917 soft-UART TX of event-identity and schedule frames to BG24.
 */
#ifndef PTS_HOST_TX_H
#define PTS_HOST_TX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Init soft-UART TX pin (idle high). Returns 0 on success. */
int pts_host_tx_init(void);

/**
 * Send PTS4 line: trigger seq plus optional follow-up M[fu_seq].
 * Not ISR-safe (bit-bang delays).
 */
int pts_host_tx_send(uint8_t board_id, uint32_t trigger_seq, int8_t rssi,
                     uint32_t fu_seq, uint32_t m_ticks, int fu_valid, uint32_t session);

/** Send PTS5 schedule line. Not ISR-safe. */
int pts_host_tx_send_sched(uint8_t board_id, uint32_t cmd_id, uint8_t cmd_type,
                           uint32_t exec_m, uint8_t status);

#ifdef __cplusplus
}
#endif

#endif /* PTS_HOST_TX_H */
