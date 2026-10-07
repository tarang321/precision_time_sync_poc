/**
 * @file pts_prs_timer_capture.c
 * @brief Series-2 xG24: GPIO -> PRS async -> TIMER0 input capture.
 *
 * Refs:
 * - Silabs peripheral_examples series2 timer_input_capture_interrupt
 * - em_prs PRS_ConnectConsumer(prsConsumerTIMER0_CC0)
 * - TIMER_InitCC with prsInput + timerPrsInputAsyncPulse
 */
#include "pts_prs_timer_capture.h"
#include "pts_schedule_gpio.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_timer.h"

#ifndef PTS_BG24_CAPTURE_PORT
#define PTS_BG24_CAPTURE_PORT gpioPortC
#endif
#ifndef PTS_BG24_CAPTURE_PIN
#define PTS_BG24_CAPTURE_PIN  0u
#endif

#ifndef PTS_PRS_CHANNEL
#define PTS_PRS_CHANNEL       0u
#endif

static volatile pts_capture_sample_t s_ring[PTS_CAPTURE_RING_SIZE];
static volatile uint32_t s_head;
static volatile uint32_t s_tail;
static volatile uint32_t s_irq_seq;
static volatile uint32_t s_overflows;

void TIMER0_IRQHandler(void)
{
  uint32_t flags = TIMER_IntGet(TIMER0);
  TIMER_IntClear(TIMER0, flags);

  if ((flags & TIMER_IF_CC1) != 0u) {
    pts_schedule_gpio_on_cc1();
  }

  if ((flags & TIMER_IF_CC0) == 0u) {
    return;
  }

  uint32_t r = TIMER_CaptureGet(TIMER0, 0);
  uint32_t next = (s_head + 1u) % PTS_CAPTURE_RING_SIZE;
  if (next == s_tail) {
    s_overflows++;
    return;
  }
  s_ring[s_head].r_ticks = r;
  s_ring[s_head].irq_seq = s_irq_seq++;
  s_head = next;
}

void pts_prs_timer_capture_init(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PRS, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);

  /* Capture input from Si917 event GPIO wire. */
  GPIO_PinModeSet(PTS_BG24_CAPTURE_PORT, PTS_BG24_CAPTURE_PIN, gpioModeInput, 0);

  /*
   * Series 2: GPIO pin interrupts also feed async PRS GPIO_PINn producers.
   * Configure rising-edge sense; IRQ disabled — PRS alone drives TIMER.
   */
  unsigned int int_no = PTS_BG24_CAPTURE_PIN & 0xFu;
  GPIO_ExtIntConfig(PTS_BG24_CAPTURE_PORT,
                    PTS_BG24_CAPTURE_PIN,
                    int_no,
                    true,  /* rising */
                    false, /* falling */
                    false  /* IRQ off */);

  /* Async PRS: GPIO_PINx -> channel -> TIMER0 CC0 consumer. */
  {
    static const uint32_t gpio_prs_sig[] = {
      PRS_ASYNC_GPIO_PIN0, PRS_ASYNC_GPIO_PIN1, PRS_ASYNC_GPIO_PIN2, PRS_ASYNC_GPIO_PIN3,
      PRS_ASYNC_GPIO_PIN4, PRS_ASYNC_GPIO_PIN5, PRS_ASYNC_GPIO_PIN6, PRS_ASYNC_GPIO_PIN7
    };
    uint32_t pin_ix = PTS_BG24_CAPTURE_PIN & 0x7u;
    PRS_ConnectSignal(PTS_PRS_CHANNEL, prsTypeAsync, (PRS_Signal_t)gpio_prs_sig[pin_ix]);
  }
  PRS_ConnectConsumer(PTS_PRS_CHANNEL, prsTypeAsync, prsConsumerTIMER0_CC0);

  TIMER_Init_TypeDef timer_init = TIMER_INIT_DEFAULT;
  timer_init.enable   = false;
  timer_init.prescale = timerPrescale1; /* full HF resolution for R[N] */

  TIMER_InitCC_TypeDef cc_init = TIMER_INITCC_DEFAULT;
  cc_init.eventCtrl    = timerEventEveryEdge;
  cc_init.edge         = timerEdgeRising;
  cc_init.mode         = timerCCModeCapture;
  cc_init.prsInput     = true;
  cc_init.prsInputType = timerPrsInputAsyncPulse;
  cc_init.prsSel       = (TIMER_PRSSEL_TypeDef)PTS_PRS_CHANNEL;

  TIMER_Init(TIMER0, &timer_init);
  TIMER_InitCC(TIMER0, 0, &cc_init);

  /*
   * CC1 CFG (compare mode) needs TIMER EN=0 on Series 2, so it is set here
   * before the counter starts. pts_schedule_gpio owns CC1 CTRL/OC and the
   * pin route; it must not call TIMER_InitCC on a running counter.
   */
  {
    TIMER_InitCC_TypeDef cc1_init = TIMER_INITCC_DEFAULT;
    cc1_init.mode = timerCCModeCompare;
    cc1_init.cmoa = timerOutputActionNone;
    TIMER_InitCC(TIMER0, 1, &cc1_init);
  }

  TIMER_IntEnable(TIMER0, TIMER_IEN_CC0);
  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  /* Highest priority: capture ring and CC1 bookkeeping must not queue behind UART. */
  NVIC_SetPriority(TIMER0_IRQn, 0u);
  NVIC_EnableIRQ(TIMER0_IRQn);

  TIMER_Enable(TIMER0, true);
}

bool pts_prs_timer_capture_pop(pts_capture_sample_t *out)
{
  if (out == NULL) {
    return false;
  }
  if (s_tail == s_head) {
    return false;
  }
  *out = s_ring[s_tail];
  s_tail = (s_tail + 1u) % PTS_CAPTURE_RING_SIZE;
  return true;
}

uint32_t pts_prs_timer_capture_pending(void)
{
  uint32_t h = s_head;
  uint32_t t = s_tail;
  if (h >= t) {
    return h - t;
  }
  return PTS_CAPTURE_RING_SIZE - t + h;
}

uint32_t pts_prs_timer_capture_overflows(void)
{
  return s_overflows;
}
