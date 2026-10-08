/**
 * @file pts_schedule_gpio.c
 * @brief Convert future M → local R; pulse LED1 from timer path (not BLE CB).
 *
 * Master push-time M vs RX air-time R: to_local() embeds that bias.
 * Master adds PTS_SCHED_MASTER_COMP_MS so LED1 matches RX (LA-tuned;
 * full PA=30 ms overshot to ~23 ms → use 7 ms).
 */
#include "pts_schedule_gpio.h"
#include "pts_network_time.h"
#include "pts_schedule_protocol.h"
#include "pts_sync_protocol.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"
#include "cmsis_os2.h"

#ifndef PTS_SCHED_BUSY_LEAD_MS
#define PTS_SCHED_BUSY_LEAD_MS 2u
#endif
/* Visible ON time after sync edge (active-low LED: low = lit). */
#ifndef PTS_SCHED_PULSE_MS
#define PTS_SCHED_PULSE_MS 80u
#endif
/* Master-only: delay local fire vs advertised exec_m (push→air trim). */
#ifndef PTS_SCHED_MASTER_COMP_MS
#define PTS_SCHED_MASTER_COMP_MS 7u
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
static uint8_t s_fire_pending;
static uint32_t s_fire_cmd;

/**
 * Sync edge = falling (both boards). Hold low PTS_SCHED_PULSE_MS so LED1
 * is visibly ON (active-low), then return idle high.
 */
static void led1_fire_pulse(void)
{
  if (!s_led1_ready) {
    return;
  }
  (void)sl_gpio_driver_clear_pin(&s_led1.port_pin); /* falling edge = fire / LED on */
  (void)osDelay(PTS_SCHED_PULSE_MS);
  (void)sl_gpio_driver_set_pin(&s_led1.port_pin); /* idle / LED off */
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

int pts_schedule_gpio_init(void)
{
  sl_status_t status;

  s_armed         = 0u;
  s_have_last_cmd = 0u;
  s_last_cmd_id   = 0u;
  s_fire_pending  = 0u;
  s_led1_ready    = 0u;

  status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return -1;
  }
  status = sl_gpio_set_configuration(s_led1);
  if (status != SL_STATUS_OK) {
    return -1;
  }
  /* Idle high — both boards same resting state (active-low LED = off). */
  (void)sl_gpio_driver_set_pin(&s_led1.port_pin);
  s_led1_ready = 1u;

  if (osThreadNew(pts_schedule_thread, NULL, &s_sched_attr) == NULL) {
    return -1;
  }
  return 0;
}

static int arm_common(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m, uint32_t r_target)
{
  if (cmd_type != PTS_SCHED_TYPE_GPIO_TOGGLE) {
    return -1;
  }
  if (s_have_last_cmd && (cmd_id == s_last_cmd_id)) {
    return 1;
  }
  s_cmd_id        = cmd_id;
  s_exec_m        = exec_m;
  s_r_target      = r_target;
  s_armed         = 1u;
  s_last_cmd_id   = cmd_id;
  s_have_last_cmd = 1u;
  return 0;
}

int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m)
{
  uint32_t r_target = 0;

  if (pts_network_time_to_local(exec_m, &r_target) != 0) {
    return -1;
  }
  return arm_common(cmd_id, cmd_type, exec_m, r_target);
}

int pts_schedule_gpio_arm_local(uint32_t cmd_id, uint8_t cmd_type, uint32_t local_ticks)
{
  uint32_t comp =
    pts_ms_to_systimer_ticks(PTS_SCHED_MASTER_COMP_MS, osKernelGetSysTimerFreq());
  return arm_common(cmd_id, cmd_type, local_ticks, local_ticks + comp);
}

int pts_schedule_gpio_take_fire(uint32_t *cmd_id_out, uint32_t *exec_m_out)
{
  if (!s_fire_pending) {
    return 0;
  }
  if (cmd_id_out != NULL) {
    *cmd_id_out = s_fire_cmd;
  }
  if (exec_m_out != NULL) {
    *exec_m_out = s_exec_m;
  }
  s_fire_pending = 0u;
  return 1;
}

void pts_schedule_gpio_poll(void)
{
  uint32_t now;
  uint32_t left;
  uint32_t freq;
  uint32_t busy_lead;

  if (!s_armed) {
    return;
  }

  now  = osKernelGetSysTimerCount();
  left = (uint32_t)(s_r_target - now);
  if (left > 0x80000000u) {
    led1_fire_pulse();
    s_fire_cmd     = s_cmd_id;
    s_fire_pending = 1u;
    s_armed        = 0u;
    return;
  }

  freq      = osKernelGetSysTimerFreq();
  busy_lead = (freq / 1000u) * PTS_SCHED_BUSY_LEAD_MS;
  if (busy_lead == 0u) {
    busy_lead = 1u;
  }

  if (left > busy_lead) {
    uint32_t sleep_ms = ((left - busy_lead) / (freq / 1000u));
    if (sleep_ms > 1u) {
      if (sleep_ms > 20u) {
        sleep_ms = 20u; /* tighter re-check near deadline */
      }
      (void)osDelay(sleep_ms);
      return;
    }
  }

  while ((int32_t)(osKernelGetSysTimerCount() - s_r_target) < 0) {
  }
  led1_fire_pulse();
  s_fire_cmd     = s_cmd_id;
  s_fire_pending = 1u;
  s_armed        = 0u;
}
