/**
 * @file pts_host_rx.c
 * @brief USART0 RX @ 115200 for PTS4 identity and PTS5 schedule lines.
 *
 * Default route: PC01 RX (wire from Si917 GPIO7 soft-UART TX).
 */
#include "pts_host_rx.h"
#include "../common/pts_followup_protocol.h"
#include "../common/pts_schedule_protocol.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#ifndef PTS_BG24_HOST_RX_PORT
#define PTS_BG24_HOST_RX_PORT gpioPortC
#endif
#ifndef PTS_BG24_HOST_RX_PIN
#define PTS_BG24_HOST_RX_PIN  1u
#endif

#define PTS_HOST_ID_RING    32u
#define PTS_HOST_SCHED_RING 16u
#define PTS_HOST_LINE_MAX   PTS_FU_LINE_MAX
/* Raw lines land here from the ISR; sscanf runs in pts_host_rx_process(). */
#define PTS_HOST_RAW_RING   8u

typedef struct {
  char    text[PTS_HOST_LINE_MAX];
  uint8_t len;
} pts_raw_line_t;

static volatile char s_line[PTS_HOST_LINE_MAX];
static volatile uint8_t s_line_len;
static pts_raw_line_t s_raw_ring[PTS_HOST_RAW_RING];
static volatile uint32_t s_raw_head;
static volatile uint32_t s_raw_tail;
static volatile uint32_t s_raw_drops;
static pts_host_id_t s_id_ring[PTS_HOST_ID_RING];
static uint32_t s_id_head;
static uint32_t s_id_tail;
static pts_host_sched_t s_sched_ring[PTS_HOST_SCHED_RING];
static uint32_t s_sched_head;
static uint32_t s_sched_tail;

void USART0_RX_IRQHandler(void)
{
  if ((USART0->IF & USART_IF_RXDATAV) == 0u) {
    return;
  }
  char c = (char)USART_Rx(USART0);
  if ((c == '\n') || (c == '\r')) {
    if (s_line_len > 0u) {
      uint32_t next = (s_raw_head + 1u) % PTS_HOST_RAW_RING;
      if (next != s_raw_tail) {
        pts_raw_line_t *dst = &s_raw_ring[s_raw_head];
        uint8_t n = s_line_len;
        for (uint8_t i = 0; i < n; i++) {
          dst->text[i] = s_line[i];
        }
        dst->text[n] = '\0';
        dst->len = n;
        s_raw_head = next;
      } else {
        s_raw_drops++;
      }
      s_line_len = 0u;
    }
    return;
  }
  if (s_line_len < (PTS_HOST_LINE_MAX - 1u)) {
    s_line[s_line_len++] = c;
  } else {
    s_line_len = 0u; /* resync */
  }
}

void pts_host_rx_process(void)
{
  while (s_raw_tail != s_raw_head) {
    const char *text = s_raw_ring[s_raw_tail].text;
    pts_host_id_t id;
    int fu_valid = 0;

    if (pts_fu_parse_line(text, &id.board_id, &id.trigger_seq, &id.rssi,
                          &id.fu_seq, &id.m_ticks, &fu_valid, &id.session) == 0) {
      id.fu_valid = (uint8_t)fu_valid;
      uint32_t next = (s_id_head + 1u) % PTS_HOST_ID_RING;
      if (next != s_id_tail) {
        s_id_ring[s_id_head] = id;
        s_id_head = next;
      }
    } else {
      pts_host_sched_t sch;
      if (pts_sched_parse_line(text, &sch.board_id, &sch.cmd_id, &sch.cmd_type,
                               &sch.exec_m, &sch.status) == 0) {
        uint32_t next = (s_sched_head + 1u) % PTS_HOST_SCHED_RING;
        if (next != s_sched_tail) {
          s_sched_ring[s_sched_head] = sch;
          s_sched_head = next;
        }
      }
    }
    s_raw_tail = (s_raw_tail + 1u) % PTS_HOST_RAW_RING;
  }
}

uint32_t pts_host_rx_raw_drops(void)
{
  return s_raw_drops;
}

void pts_host_rx_init(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);

  GPIO_PinModeSet(PTS_BG24_HOST_RX_PORT, PTS_BG24_HOST_RX_PIN, gpioModeInputPull, 1);

  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
  init.baudrate = PTS_FU_BAUD;
  init.enable   = usartDisable;
  USART_InitAsync(USART0, &init);

  GPIO->USARTROUTE[0].RXROUTE = (PTS_BG24_HOST_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (PTS_BG24_HOST_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN;

  USART_IntEnable(USART0, USART_IEN_RXDATAV);
  NVIC_ClearPendingIRQ(USART0_RX_IRQn);
  /* Below TIMER0 (0): a UART byte must never delay capture/compare handling. */
  NVIC_SetPriority(USART0_RX_IRQn, 3u);
  NVIC_EnableIRQ(USART0_RX_IRQn);

  USART_Enable(USART0, usartEnableRx);
}

bool pts_host_rx_pop(pts_host_id_t *out)
{
  if (out == NULL) {
    return false;
  }
  if (s_id_tail == s_id_head) {
    return false;
  }
  *out = s_id_ring[s_id_tail];
  s_id_tail = (s_id_tail + 1u) % PTS_HOST_ID_RING;
  return true;
}

bool pts_host_rx_pop_sched(pts_host_sched_t *out)
{
  if (out == NULL) {
    return false;
  }
  if (s_sched_tail == s_sched_head) {
    return false;
  }
  *out = s_sched_ring[s_sched_tail];
  s_sched_tail = (s_sched_tail + 1u) % PTS_HOST_SCHED_RING;
  return true;
}
