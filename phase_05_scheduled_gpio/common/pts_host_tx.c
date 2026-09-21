/**
 * @file pts_host_tx.c
 * @brief Bit-bang UART TX @ PTS_EVENT_REF_BAUD (stand-in for NCP host message).
 */
#include "pts_host_tx.h"
#include "pts_followup_protocol.h"
#include "pts_schedule_protocol.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"

#ifndef PTS_HOST_TX_GPIO_PORT
#define PTS_HOST_TX_GPIO_PORT SL_SI91X_GPIO_7_PORT
#endif
#ifndef PTS_HOST_TX_GPIO_PIN
#define PTS_HOST_TX_GPIO_PIN GPIO_PIN_NUMBER7
#endif

static sl_si91x_gpio_pin_config_t s_tx = {
  { PTS_HOST_TX_GPIO_PORT, PTS_HOST_TX_GPIO_PIN },
  GPIO_OUTPUT
};
static uint8_t s_ready;

/* ~115200 bit time @ ~180 MHz HP core: tune if frames corrupt. */
#ifndef PTS_HOST_TX_BIT_LOOPS
#define PTS_HOST_TX_BIT_LOOPS  156u
#endif

static void bit_delay(void)
{
  for (volatile uint32_t i = 0; i < PTS_HOST_TX_BIT_LOOPS; i++) {
  }
}

static void tx_bit(uint8_t level)
{
  if (level) {
    (void)sl_gpio_driver_set_pin(&s_tx.port_pin);
  } else {
    (void)sl_gpio_driver_clear_pin(&s_tx.port_pin);
  }
  bit_delay();
}

static void tx_byte(uint8_t b)
{
  tx_bit(0); /* start */
  for (uint8_t i = 0; i < 8u; i++) {
    tx_bit((uint8_t)((b >> i) & 1u));
  }
  tx_bit(1); /* stop */
}

static int tx_line(const char *line, int n)
{
  if (!s_ready || (line == NULL) || (n <= 0)) {
    return -1;
  }
  for (int i = 0; i < n; i++) {
    tx_byte((uint8_t)line[i]);
  }
  return 0;
}

int pts_host_tx_init(void)
{
  sl_status_t status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return -1;
  }
  status = sl_gpio_set_configuration(s_tx);
  if (status != SL_STATUS_OK) {
    return -1;
  }
  (void)sl_gpio_driver_set_pin(&s_tx.port_pin); /* idle high */
  s_ready = 1u;
  return 0;
}

int pts_host_tx_send(uint8_t board_id, uint32_t trigger_seq, int8_t rssi,
                     uint32_t fu_seq, uint32_t m_ticks, int fu_valid, uint32_t session)
{
  char line[PTS_FU_LINE_MAX];
  int n = pts_fu_format_line(line, sizeof(line), board_id, trigger_seq, rssi, fu_seq, m_ticks,
                             fu_valid, session);
  return tx_line(line, n);
}

int pts_host_tx_send_sched(uint8_t board_id, uint32_t cmd_id, uint8_t cmd_type,
                           uint32_t exec_m, uint8_t status)
{
  char line[PTS_SCHED_LINE_MAX];
  int n = pts_sched_format_line(line, sizeof(line), board_id, cmd_id, cmd_type, exec_m, status);
  return tx_line(line, n);
}
