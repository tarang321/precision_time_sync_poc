/**
 * @file pts_event_gpio.c
 * @brief Si917 HP GPIO pulse: BLE event reference for BG24 capture.
 *
 * Note: WiSeConnect Periodic Adv report has no controller RX timestamp.
 * GPIO Timestamp Controller is removed on SiWG917 (GPIO_TIMESTAMP_E301).
 * Closest deterministic app-visible edge is AE report callback entry.
 */
#include "pts_event_gpio.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"

#ifndef PTS_EVENT_GPIO_PORT
#define PTS_EVENT_GPIO_PORT SL_SI91X_GPIO_6_PORT
#endif
#ifndef PTS_EVENT_GPIO_PIN
#define PTS_EVENT_GPIO_PIN GPIO_PIN_NUMBER6
#endif

static sl_si91x_gpio_pin_config_t s_pin_cfg = {
  { PTS_EVENT_GPIO_PORT, PTS_EVENT_GPIO_PIN },
  GPIO_OUTPUT
};
static uint8_t s_ready;

/*
 * Direct HP EGPIO bit-load register. Same write sl_gpio_driver_set_pin()
 * ends in, minus NULL/port/pin validation and the port branch, so the edge
 * is as close to callback entry as the M4 can place it.
 */
#define PTS_EVENT_BIT_LOAD \
  (GPIO->PIN_CONFIG[((uint32_t)PTS_EVENT_GPIO_PORT * MAX_GPIO_PORT_PIN) + PTS_EVENT_GPIO_PIN].BIT_LOAD_REG)

int pts_event_gpio_init(void)
{
  sl_status_t status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return -1;
  }
  status = sl_gpio_set_configuration(s_pin_cfg);
  if (status != SL_STATUS_OK) {
    return -1;
  }
  (void)sl_gpio_driver_clear_pin(&s_pin_cfg.port_pin);
  s_ready = 1u;
  return 0;
}

void pts_event_gpio_pulse(void)
{
  if (!s_ready) {
    return;
  }
  /* Rising edge is the capture trigger on BG24. */
  PTS_EVENT_BIT_LOAD = 1u;
  /* Hold high briefly so async PRS + TIMER capture can settle. */
  for (volatile uint32_t i = 0; i < 32u; i++) {
  }
  PTS_EVENT_BIT_LOAD = 0u;
}
