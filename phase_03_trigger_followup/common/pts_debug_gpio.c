/**
 * @file pts_debug_gpio.c
 * @brief Si917 LED0 toggle for Phase 3 master LA / visual edge.
 *
 * BRD4338A LED0 = ULP_GPIO_2.
 */
#include "pts_debug_gpio.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"

#ifndef PTS_DEBUG_GPIO_PORT
#define PTS_DEBUG_GPIO_PORT SL_SI91X_ULP_GPIO_2_PORT
#endif
#ifndef PTS_DEBUG_GPIO_PIN
#define PTS_DEBUG_GPIO_PIN SL_SI91X_ULP_GPIO_2_PIN
#endif

static sl_si91x_gpio_pin_config_t s_pin_cfg = { { PTS_DEBUG_GPIO_PORT, PTS_DEBUG_GPIO_PIN }, GPIO_OUTPUT };
static uint8_t s_ready;

int pts_debug_gpio_init(void)
{
  sl_status_t status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return -1;
  }
  status = sl_gpio_set_configuration(s_pin_cfg);
  if (status != SL_STATUS_OK) {
    return -1;
  }
  s_ready = 1u;
  return 0;
}

void pts_debug_gpio_toggle(void)
{
  if (!s_ready) {
    return;
  }
  (void)sl_gpio_driver_toggle_pin(&s_pin_cfg.port_pin);
}
