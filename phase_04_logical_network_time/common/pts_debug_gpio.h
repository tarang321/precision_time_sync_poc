/**
 * @file pts_debug_gpio.h
 * @brief Phase 1 debug GPIO for logic-analyzer skew (callback path).
 */
#ifndef PTS_DEBUG_GPIO_H
#define PTS_DEBUG_GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Init HP GPIO output used for sync-edge toggle. Returns 0 on success. */
int pts_debug_gpio_init(void);

/** Toggle debug pin (ISR/callback-safe if driver allows; prefer from event context). */
void pts_debug_gpio_toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_DEBUG_GPIO_H */
