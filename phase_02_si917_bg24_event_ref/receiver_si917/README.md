# Phase 2 Si917 receiver (no BG24)

**Role:** Sample local `R[N]` at AE Periodic Adv report + EXP16 (`ULP_GPIO_6`) toggle + VCOM `seq`/`R` logs.

**GPIO sanity (default on):** `PTS_GPIO_SANITY_TOGGLE=1` in `pts_event_gpio.h` → free-run ~2 Hz on EXP16, BLE off. Probe EXP16 + GND. Set to `0` for Phase 2 AE path.

**Topology:** Phase 1 master + this receiver. Do not flash `bg24_capture/`.

## Behavior

1. `R[N] = osKernelGetSysTimerCount()` at AE report callback entry
2. `pts_event_gpio_toggle()` on EXP16 / `ULP_GPIO_6` (when sanity=0)
3. Ring stores payload + `R`; VCOM: `RX1 seq=… R=…` every 10 seq

## Build / flash

1. Sanity check: flash with default `PTS_GPIO_SANITY_TOGGLE=1`; probe EXP16 (~2 Hz).
2. Phase 2: set sanity=0; keep Phase 1 master running; flash; expect `seq`/`R` logs.
3. Open `pts_receiver_si917_p2.slcp` → Generate → Build → Flash `.rps`.

## Pass

- Sanity: VCOM `PTS GPIO sanity…`; EXP16 square ~2 Hz
- Phase2: periodic sync; `seq` ↔ `R`; optional LA ~20 ms edges on EXP16
