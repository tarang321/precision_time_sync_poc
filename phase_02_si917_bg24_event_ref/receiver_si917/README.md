# Phase 2 Si917 receiver (no BG24)

**Role:** Sample local `R[N]` at AE Periodic Adv report + LED0 (`ULP_GPIO_2`) toggle + VCOM `seq`/`R` logs.

**GPIO sanity (opt-in):** set `PTS_GPIO_SANITY_TOGGLE=1` in `pts_event_gpio.h` → free-run ~2 Hz on LED0, BLE off. Default is `0` (AE path).

**Topology:** Phase 1 master + this receiver.

## Behavior

1. `R[N] = osKernelGetSysTimerCount()` at AE report callback entry
2. `pts_event_gpio_toggle()` on LED0 / `ULP_GPIO_2` (when sanity=0)
3. Ring stores payload + `R`; VCOM: `RX1 seq=… R=…` every 10 seq

## Build / flash

1. Sanity check (optional): set `PTS_GPIO_SANITY_TOGGLE=1`; probe LED0 (~2 Hz).
2. Phase 2: sanity=0; keep Phase 1 master running; flash; expect `seq`/`R` logs.
3. Open `pts_receiver_si917_p2.slcp` → Generate → Build → Flash `.rps`.

## Pass

- Sanity: VCOM `PTS GPIO sanity…`; LED0 square ~2 Hz
- Phase2: periodic sync; `seq` ↔ `R`; optional LA ~20 ms edges on LED0
