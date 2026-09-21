/**
 * @file pts_schedule_gpio.h
 * @brief Phase 5: network-time command -> TIMER0 CC1 compare -> demo GPIO.
 */
#ifndef PTS_SCHEDULE_GPIO_H
#define PTS_SCHEDULE_GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Init demo GPIO (PC02) and TIMER0 CC1 compare. Call after capture init. */
void pts_schedule_gpio_init(void);

/**
 * Arm one GPIO toggle at network time exec_m.
 * Duplicate cmd_id is ignored. Returns 0 if armed, -1 if rejected.
 */
int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m);

/** Called from TIMER0_IRQHandler when CC1 matches. */
void pts_schedule_gpio_on_cc1(void);

uint32_t pts_schedule_gpio_exec_count(void);
uint32_t pts_schedule_gpio_dup_count(void);
uint32_t pts_schedule_gpio_reject_count(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_SCHEDULE_GPIO_H */
