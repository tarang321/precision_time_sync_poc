/**
 * @file pts_schedule_gpio.h
 * @brief Arm local SysTimer → set LED1 level (ON/OFF) at exec time.
 */
#ifndef PTS_SCHEDULE_GPIO_H
#define PTS_SCHEDULE_GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t cmd_id;
  uint32_t exec_m;
  uint32_t r_target;
  uint32_t now_at_arm;
  int32_t lead_ms; /* remaining at ARM; negative = already past */
  uint8_t cmd_type;
  uint8_t late;    /* 1 if fired after >1 ms late */
} pts_sched_arm_info_t;

int pts_schedule_gpio_init(void);

/**
 * RX: convert network exec_m → local R and arm.
 * cmd_type: PTS_SCHED_TYPE_GPIO_ON / OFF.
 * Returns 0 armed, 1 duplicate, -1 fail, -2 late skip (no fire).
 */
int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m);

/**
 * Master: arm at exec_m (COMP=0). Return -2 if target already past (no fire).
 */
int pts_schedule_gpio_arm_local(uint32_t cmd_id, uint8_t cmd_type, uint32_t local_ticks);

void pts_schedule_gpio_poll(void);

/** 1 if FIRE happened; fills cmd/exec; late_out optional. */
int pts_schedule_gpio_take_fire(uint32_t *cmd_id_out, uint32_t *exec_m_out, uint8_t *late_out);

/** 1 if a new ARM happened since last take; fills info for VCOM. */
int pts_schedule_gpio_take_arm_info(pts_sched_arm_info_t *out);

#ifdef __cplusplus
}
#endif

#endif /* PTS_SCHEDULE_GPIO_H */
