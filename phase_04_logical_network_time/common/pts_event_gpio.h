/**
 * @file pts_event_gpio.h
 * @brief Si917 BLE-event reference GPIO edge for BG24 PRS capture.
 */
#ifndef PTS_EVENT_GPIO_H
#define PTS_EVENT_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/** Init event-ref output pin. Returns 0 on success. */
int pts_event_gpio_init(void);

/**
 * Emit a rising-then-falling pulse (idle low).
 * Call from AE periodic-adv report callback for earliest available edge.
 * Safe enough for BLE stack callback context (GPIO driver only).
 */
void pts_event_gpio_pulse(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_EVENT_GPIO_H */
