/**
 * @file pts_schedule_gpio.c
 * @brief Convert future M to TIMER0 CC1; toggle PC02 from the compare IRQ.
 */
#include "pts_schedule_gpio.h"
#include "pts_network_time.h"
#include "../common/pts_schedule_protocol.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

#include <stdio.h>

#ifndef PTS_DEMO_GPIO_PORT
#define PTS_DEMO_GPIO_PORT gpioPortC
#endif
#ifndef PTS_DEMO_GPIO_PIN
#define PTS_DEMO_GPIO_PIN  2u
#endif

/** Reject if local target is fewer than this many TIMER ticks ahead. */
#define PTS_SCHED_MIN_LEAD_TICKS  2000u

static uint32_t s_last_cmd_id;
static uint8_t s_have_last_cmd;
static uint8_t s_armed;
static uint32_t s_armed_cmd;
static uint32_t s_exec_count;
static uint32_t s_dup_count;
static uint32_t s_reject_count;

void pts_schedule_gpio_init(void)
{
  TIMER_InitCC_TypeDef cc_init = TIMER_INITCC_DEFAULT;

  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(PTS_DEMO_GPIO_PORT, PTS_DEMO_GPIO_PIN, gpioModePushPull, 0);

  /* TIMER0 already running from capture init. CC1 = compare. */
  cc_init.mode = timerCCModeCompare;
  TIMER_InitCC(TIMER0, 1, &cc_init);
  TIMER_IntDisable(TIMER0, TIMER_IEN_CC1);
  TIMER_IntClear(TIMER0, TIMER_IF_CC1);
}

int pts_schedule_gpio_arm(uint32_t cmd_id, uint8_t cmd_type, uint32_t exec_m)
{
  uint32_t r_target = 0;
  uint32_t now_r;
  int32_t lead;

  if (cmd_type != PTS_SCHED_TYPE_GPIO_TOGGLE) {
    s_reject_count++;
    return -1;
  }
  if (s_have_last_cmd && (cmd_id == s_last_cmd_id)) {
    s_dup_count++;
    return -1;
  }
  if (s_armed && (cmd_id == s_armed_cmd)) {
    s_dup_count++;
    return -1;
  }
  if (pts_network_time_to_local(exec_m, &r_target) != 0) {
    s_reject_count++;
    return -1;
  }

  now_r = TIMER_CounterGet(TIMER0);
  lead = (int32_t)(r_target - now_r);
  if (lead < (int32_t)PTS_SCHED_MIN_LEAD_TICKS) {
    s_reject_count++;
    printf("PTS sched reject cmd=%lu lead=%ld (too soon or past)\n",
           (unsigned long)cmd_id, (long)lead);
    return -1;
  }

  TIMER_IntDisable(TIMER0, TIMER_IEN_CC1);
  TIMER_CompareSet(TIMER0, 1, r_target);
  TIMER_IntClear(TIMER0, TIMER_IF_CC1);
  TIMER_IntEnable(TIMER0, TIMER_IEN_CC1);

  s_armed = 1u;
  s_armed_cmd = cmd_id;
  printf("PTS sched arm cmd=%lu exec_m=%lu R=%lu lead=%ld\n",
         (unsigned long)cmd_id,
         (unsigned long)exec_m,
         (unsigned long)r_target,
         (long)lead);
  return 0;
}

void pts_schedule_gpio_on_cc1(void)
{
  if (!s_armed) {
    TIMER_IntDisable(TIMER0, TIMER_IEN_CC1);
    return;
  }
  GPIO_PinOutToggle(PTS_DEMO_GPIO_PORT, PTS_DEMO_GPIO_PIN);
  s_last_cmd_id = s_armed_cmd;
  s_have_last_cmd = 1u;
  s_armed = 0u;
  s_exec_count++;
  TIMER_IntDisable(TIMER0, TIMER_IEN_CC1);
}

uint32_t pts_schedule_gpio_exec_count(void)
{
  return s_exec_count;
}

uint32_t pts_schedule_gpio_dup_count(void)
{
  return s_dup_count;
}

uint32_t pts_schedule_gpio_reject_count(void)
{
  return s_reject_count;
}
