/**
 * @file pts_host_rx.c
 * @brief USART0 RX @ 115200 for PTS3,<board>,<seq>,<rssi>,<fu_seq>,<M> lines.
 *
 * Default route: PC01 RX (wire from Si917 GPIO7 soft-UART TX).
 * Remap PTS_BG24_HOST_RX_PORT/PIN before generate if EXP differs.
 */
#include "pts_host_rx.h"
#include "../common/pts_followup_protocol.h"

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

#define PTS_HOST_ID_RING 32u

static volatile char s_line[PTS_FU_LINE_MAX];
static volatile uint8_t s_line_len;
static volatile pts_host_id_t s_id_ring[PTS_HOST_ID_RING];
static volatile uint32_t s_id_head;
static volatile uint32_t s_id_tail;

void USART0_RX_IRQHandler(void)
{
  if ((USART0->IF & USART_IF_RXDATAV) == 0u) {
    return;
  }
  char c = (char)USART_Rx(USART0);
  if ((c == '\n') || (c == '\r')) {
    if (s_line_len > 0u) {
      char tmp[PTS_FU_LINE_MAX];
      uint8_t n = s_line_len;
      if (n >= PTS_FU_LINE_MAX) {
        n = (uint8_t)(PTS_FU_LINE_MAX - 1u);
      }
      for (uint8_t i = 0; i < n; i++) {
        tmp[i] = s_line[i];
      }
      tmp[n] = '\0';
      s_line_len = 0u;

      pts_host_id_t id;
      int fu_valid = 0;
      if (pts_fu_parse_line(tmp, &id.board_id, &id.trigger_seq, &id.rssi,
                            &id.fu_seq, &id.m_ticks, &fu_valid, &id.session) == 0) {
        id.fu_valid = (uint8_t)fu_valid;
        uint32_t next = (s_id_head + 1u) % PTS_HOST_ID_RING;
        if (next != s_id_tail) {
          s_id_ring[s_id_head] = id;
          s_id_head = next;
        }
      }
    }
    return;
  }
  if (s_line_len < (PTS_FU_LINE_MAX - 1u)) {
    s_line[s_line_len++] = c;
  } else {
    s_line_len = 0u; /* resync */
  }
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
