/**
 * @file pts_schedule_gpio.c
 * @brief Schedule LED1 level (active-low: ON=low, OFF=high) at SysTimer target.
 *
 * Master fires at exec_m. RX fires at to_local(exec_m) - PATH_COMP.
 * ARM of a past target is skipped (no catch-up fire).
 * Arm/poll share state under osKernelLock.
 */
#include "pts_schedule_gpio.h"
#include "pts_network_time.h"
#include "pts_schedule_protocol.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"
#include "cmsis_os2.h"

#ifndef PTS_SCHED_BUSY_LEAD_MS
#define PTS_SCHED_BUSY_LEAD_MS 2u
#endif
#ifndef PTS_SCHED_LATE_LOG_MS
#define PTS_SCHED_LATE_LOG_MS 1u
#endif

#ifndef PTS_SCHED_GPIO_PORT
#define PTS_SCHED_GPIO_PORT SL_SI91X_GPIO_10_PORT
#endif
#ifndef PTS_SCHED_GPIO_PIN
#define PTS_SCHED_GPIO_PIN SL_SI91X_GPIO_10_PIN
#endif

static sl_si91x_gpio_pin_config_t s_led1 = {
  { PTS_SCHED_GPIO_PORT, PTS_SCHED_GPIO_PIN },
  GPIO_OUTPUT
};
static uint8_t s_led1_ready;

static uint8_t s_armed;
static uint8_t s_have_last_cmd;
static uint32_t s_last_cmd_id;
static uint32_t s_cmd_id;
static uint32_t s_r_target;
static uint32_t s_exec_m;
static uint8_t s_cmd_type;
static uint8_t s_fire_pending;
static uint32_t s_fire_cmd;
static uint8_t s_fire_late;

static uint8_t s_arm_info_pending;
static pts_sched_arm_info_t s_arm_info;

static void led1_apply_level(uint8_t cmd_type)
{
  if (!s_led1_ready) {
    return;
  }
  if (cmd_type == PTS_SCHED_TYPE_GPIO_ON) {
    (void)sl_gpio_driver_clear_pin(&s_led1.port_pin);
  } else if (cmd_type == PTS_SCHED_TYPE_GPIO_OFF) {
    (void)sl_gpio_driver_set_pin(&s_led1.port_pin);
  }
}

static const osThreadAttr_t s_sched_attr = {
  .name       = "pts_sched",
  .stack_size = 1024,
  .priority   = osPriorityAboveNormal,
};

static void pts_schedule_thread(void *arg)
{
  (void)arg;
  for (;;) {
    pts_schedule_gpio_poll();
    if (!s_armed) {
      (void)osDelay(1u);
    }
  }
}

static int32_t remaining_ms(uint32_t r_target, uint32_t now, uint32_t freq)
{
  int32_t left = (int32_t)(r_target - now);
  uint32_t ms_ticks;

  if (freq < 1000u) {
    return left;
  }
  ms_ticks = freq / 1000u;
  return left / (int32_t)ms_ticks;
}

int pts_schedule_gpio_init(void)
{
  sl_status_t status;

  s_armed            = 0u;
  s_have_last_cmd    = 0u;
  s_last_cmd_id      = 0u;
  s_fire_pending     = 0u;
  s_fire_late        = 0u;
  s_arm_info_pending = 0u;
  s_led1_ready       = 0u;
  s_cmd_type         = PTS_SCHED_TYPE_GPIO_OFF;

  status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return -1;
  }
  status = sl_gpio_set_configuration(s_led1);
  if (status != SL_STATUS_OK) {
    return -1;
  }
  (void)sl_gpio_driver_set_pin(&s_led1.port_pin);
  s_led1_ready = 1u;

  if (osThreadNew(pts_schedule_thread, NULL, &s_sched_attr) == NULL) {
    return -1;
  }
  return 0;
}

void pts_schedule_gpio_reset(void)
{
  (void)osKernelLock();
  s_armed         = 0u;
  s_have_last_cmd = 0u;
  s_last_cmd_id   = 0u;
  (void)osKernelUnlock();
}

static int arm_common(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m, uint32_t r_target)
{
  uint32_t now;
  uint32_t freq;

  if ((cmd_type != PTS_SCHED_TYPE_GPIO_ON) && (cmd_type != PTS_SCHED_TYPE_GPIO_OFF)) {
    return -1;
  }

  (void)osKernelLock();
  if (s_have_last_cmd && (cmd_id == s_last_cmd_id)) {
    (void)osKernelUnlock();
    return 1;
  }
  (void)osKernelUnlock();

  now  = osKernelGetSysTimerCount();
  freq = osKernelGetSysTimerFreq();

  s_arm_info.cmd_id     = cmd_id;
  s_arm_info.exec_m     = exec_m;
  s_arm_info.r_target   = r_target;
  s_arm_info.now_at_arm = now;
  s_arm_info.lead_ms    = remaining_ms(r_target, now, freq);
  s_arm_info.cmd_type   = cmd_type;
  s_arm_info.late       = 0u;
  s_arm_info_pending    = 1u;

  /* Already past — do not catch-up fire; do not consume cmd_id. */
  if ((int32_t)(r_target - now) < 0) {
    s_arm_info.late = 1u;
    return -2;
  }

  (void)osKernelLock();
  s_cmd_id        = cmd_id;
  s_cmd_type      = cmd_type;
  s_exec_m        = exec_m;
  s_r_target      = r_target;
  s_last_cmd_id   = cmd_id;
  s_have_last_cmd = 1u;
  s_armed         = 1u;
  (void)osKernelUnlock();
  return 0;
}

int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m)
{
  uint32_t r_target = 0;
  uint32_t path;
  int32_t path_ms;

  if (pts_network_time_to_local(exec_m, &r_target) != 0) {
    return -1;
  }
  path_ms = (int32_t)PTS_SCHED_PATH_COMP_MS;
  if (path_ms >= 0) {
    path = pts_ms_to_systimer_ticks((uint32_t)path_ms, osKernelGetSysTimerFreq());
    r_target -= path;
  } else {
    path = pts_ms_to_systimer_ticks((uint32_t)(-path_ms), osKernelGetSysTimerFreq());
    r_target += path;
  }
  return arm_common(cmd_id, cmd_type, exec_m, r_target);
}

int pts_schedule_gpio_arm_local(uint32_t cmd_id, uint8_t cmd_type, uint32_t local_ticks)
{
  return arm_common(cmd_id, cmd_type, local_ticks, local_ticks);
}

int pts_schedule_gpio_take_fire(uint32_t *cmd_id_out, uint32_t *exec_m_out, uint8_t *late_out)
{
  uint8_t pending;
  uint32_t fire_cmd;
  uint32_t exec_m;
  uint8_t late;

  (void)osKernelLock();
  pending  = s_fire_pending;
  fire_cmd = s_fire_cmd;
  exec_m   = s_exec_m;
  late     = s_fire_late;
  if (pending) {
    s_fire_pending = 0u;
  }
  (void)osKernelUnlock();

  if (!pending) {
    return 0;
  }
  if (cmd_id_out != NULL) {
    *cmd_id_out = fire_cmd;
  }
  if (exec_m_out != NULL) {
    *exec_m_out = exec_m;
  }
  if (late_out != NULL) {
    *late_out = late;
  }
  return 1;
}

int pts_schedule_gpio_take_arm_info(pts_sched_arm_info_t *out)
{
  if ((out == NULL) || !s_arm_info_pending) {
    return 0;
  }
  *out               = s_arm_info;
  s_arm_info_pending = 0u;
  return 1;
}

static void do_fire(uint8_t late)
{
  led1_apply_level(s_cmd_type);
  s_fire_cmd     = s_cmd_id;
  s_fire_late    = late;
  s_fire_pending = 1u;
  s_armed        = 0u;
}

void pts_schedule_gpio_poll(void)
{
  uint32_t now;
  uint32_t left;
  uint32_t freq;
  uint32_t busy_lead;
  uint32_t late_lim;
  uint32_t r_target;
  uint8_t armed;

  (void)osKernelLock();
  armed    = s_armed;
  r_target = s_r_target;
  (void)osKernelUnlock();

  if (!armed) {
    return;
  }

  now  = osKernelGetSysTimerCount();
  left = (uint32_t)(r_target - now);
  freq = osKernelGetSysTimerFreq();
  late_lim = pts_ms_to_systimer_ticks(PTS_SCHED_LATE_LOG_MS, freq);
  if (late_lim == 0u) {
    late_lim = 1u;
  }

  /* Target in the past (signed). */
  if (left > 0x80000000u) {
    uint32_t late_ticks = (uint32_t)(now - r_target);
    (void)osKernelLock();
    if (s_armed && (s_r_target == r_target)) {
      do_fire((late_ticks > late_lim) ? 1u : 0u);
    }
    (void)osKernelUnlock();
    return;
  }

  busy_lead = (freq / 1000u) * PTS_SCHED_BUSY_LEAD_MS;
  if (busy_lead == 0u) {
    busy_lead = 1u;
  }

  if (left > busy_lead) {
    uint32_t sleep_ms = ((left - busy_lead) / (freq / 1000u));
    if (sleep_ms > 1u) {
      if (sleep_ms > 20u) {
        sleep_ms = 20u;
      }
      (void)osDelay(sleep_ms);
      return;
    }
  }

  while ((int32_t)(osKernelGetSysTimerCount() - r_target) < 0) {
  }
  (void)osKernelLock();
  if (s_armed && (s_r_target == r_target)) {
    do_fire(0u);
  }
  (void)osKernelUnlock();
}
