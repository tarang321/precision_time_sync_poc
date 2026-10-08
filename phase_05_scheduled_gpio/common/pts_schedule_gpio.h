/**
 * @file pts_schedule_gpio.h
 * @brief Arm local SysTimer schedule → toggle demo GPIO (not BLE RX path).
 */
#ifndef PTS_SCHEDULE_GPIO_H
#define PTS_SCHEDULE_GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int pts_schedule_gpio_init(void);

/**
 * Arm one-shot at network time exec_m (RX: convert M→R via clock model).
 * Same cmd_id is ignored. Returns 0 armed, 1 duplicate, -1 fail.
 */
int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m);

/**
 * Arm one-shot at local SysTimer count (master: M domain == SysTimer).
 * Same cmd_id rules as arm().
 */
int pts_schedule_gpio_arm_local(uint32_t cmd_id, uint8_t cmd_type, uint32_t local_ticks);

/** Poll from schedule thread: coarse wait then busy-wait to target + toggle. */
void pts_schedule_gpio_poll(void);

/** 1 if a fire just happened; fills cmd/exec once. */
int pts_schedule_gpio_take_fire(uint32_t *cmd_id_out, uint32_t *exec_m_out);

#ifdef __cplusplus
}
#endif

#endif /* PTS_SCHEDULE_GPIO_H */
