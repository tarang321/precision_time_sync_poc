/**
 * @file pts_event_gpio.h
 * @brief Si917 BLE-event reference GPIO for logic-analyzer (Phase 2).
 *
 * Default pin: LED0 = ULP_GPIO_2 (BRD4338A kit LED).
 * Set PTS_GPIO_SANITY_TOGGLE=1 for free-running toggle (no BLE needed).
 */
#ifndef PTS_EVENT_GPIO_H
#define PTS_EVENT_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/** 1 = free-run ~2 Hz toggle on LED0 (pin check). 0 = toggle only on AE report. */
#ifndef PTS_GPIO_SANITY_TOGGLE
#define PTS_GPIO_SANITY_TOGGLE 0
#endif

/** Init event-ref output pin. Returns 0 on success. */
int pts_event_gpio_init(void);

/**
 * Toggle LED0 / ULP_GPIO_2 (idle state undefined after first call).
 * AE-report path when PTS_GPIO_SANITY_TOGGLE=0.
 */
void pts_event_gpio_toggle(void);

/** Alias kept for call sites; same as toggle (no short pulse). */
void pts_event_gpio_pulse(void);

#ifdef __cplusplus
}
#endif

#endif /* PTS_EVENT_GPIO_H */
