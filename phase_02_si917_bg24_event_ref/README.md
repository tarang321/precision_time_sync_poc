# Phase 2 — Si917-to-BG24 BLE event reference

**Goal:** Deterministic path `Si917 BLE event → GPIO edge → BG24 PRS → TIMER capture R[N]`, then delayed identity association over a host link (UART stand-in for NCP message).

## Layout

```text
phase_02_si917_bg24_event_ref/
  common/
    pts_event_ref_protocol.h   # PTS2,<board>,<seq>,<rssi> framing
    pts_event_gpio.c/.h        # Si917 rising-edge pulse
    pts_host_tx.c/.h           # Si917 soft-UART TX @ 115200
  receiver_si917/              # Phase1 AE central + Phase2 edge/host
    pts_receiver_si917_p2.slcp
  bg24_capture/                # PRS + TIMER0 capture + USART host RX
    pts_bg24_capture.slcp
```

Master still comes from Phase 1: `phase_01_ble_transport/master_si917/`.

## Wiring (defaults — remap in sources if needed)

| Signal | Si917 (BRD4338A) | BG24 (BRD4187C) | Notes |
|--------|------------------|-----------------|-------|
| Event edge | HP **GPIO6** | **PC00** input | Rising pulse; also LA probe |
| Host identity | HP **GPIO7** soft-UART TX | **PC01** USART0 RX | `PTS2,...` lines @ 115200 |
| GND | common | common | Required |

Series-2 GPIO→PRS producers are **pin-number** based (PIN0–PIN7). Prefer capture on pin 0–7 of the chosen port.

## Constraints (verified)

- Periodic Adv report: payload / sync handle / RSSI / data status — **no controller RX timestamp**.
- SiWG917 **GPIO Timestamp Controller removed** (`GPIO_TIMESTAMP_E301`).
- Closest app-visible edge: pulse at start of AE periodic-adv **report callback** (before RTOS wake).
- Host frame delay must **not** define `R[N]`; only identifies which BLE `seq` matches the already-captured edge.

## Build

### Si917 receiver

1. Open `receiver_si917/pts_receiver_si917_p2.slcp` in Studio (WiSeConnect 4.1.0 / SiSDK 2026.6.0).
2. Generate → Build → Flash.
3. Run Phase 1 master first.

### BG24 capture

1. Open `bg24_capture/pts_bg24_capture.slcp` (BRD4187C / MG24).
2. Generate → Build → Flash.
3. VCOM log should show `R[N] pair seq=... R=...` once wires and master train are live.

## Exit criteria

- [ ] Si917 pulses GPIO6 on each Periodic Adv report
- [ ] BG24 TIMER0 ISR records `R` without waiting for host UART
- [ ] Host frame arrives later; BG24 logs `seq` ↔ `R` pairs
- [ ] LA: Si917 GPIO6 rising edge aligns with BG24 capture activity
- [ ] Orphan / overflow counters stay low under ~20–25 ms train

## Soft-UART bit timing

If host frames are corrupt, tune `PTS_HOST_TX_BIT_LOOPS` in `common/pts_host_tx.c` for your Si917 HP clock.

## Next

Phase 3: correlate master `M[N]` with this `R[N]` (trigger + follow-up).

## References

No in-tree PTS capture project existed. Code was written from these sources.

| Used for | Reference |
|----------|-----------|
| Phase 2 behavior (GPIO edge, PRS `R[N]`, delayed identity) | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 2 |
| Si917 BLE receiver | Fork of `phase_01_ble_transport/receiver_1_si917/` (`app.c`, `.slcp`) |
| Periodic Adv payload | `phase_01_ble_transport/common/pts_sync_protocol.h` |
| Si917 GPIO pulse | Same `sl_gpio` calls as Phase 1 `pts_debug_gpio.c` |
| BG24 `.slcp` / `main.c` / BRD4187C | Workspace `Elevator_capacity_controller`, `bt_soc_blinky_demos` |
| TIMER input capture | [series2 `timer_input_capture_interrupt`](https://github.com/SiliconLabsSoftware/peripheral_examples_gsdk/blob/master/series2/timer/timer_input_capture_interrupt/src/main.c) |
| PRS → TIMER0 CC0 | Local SiSDK 2026.6.0 `em_prs.h`, `efr32mg24_prs_signals.h` (`PRS_ConnectSignal`, `PRS_ASYNC_GPIO_PINx`, `prsConsumerTIMER0_CC0`) |
| TIMER / PRS API | [TIMER](https://docs.silabs.com/gecko-platform/latest/platform-emlib-efr32xg24/timer), [PRS](https://docs.silabs.com/gecko-platform/latest/platform-emlib-efr32xg24/prs) |
| No controller RX timestamp | WiSeConnect Periodic Advertising report fields (payload, sync handle, RSSI, data status) |
| No Si917 GPIO timestamp HW | [SiWG917 errata `GPIO_TIMESTAMP_E301`](https://www.silabs.com/documents/public/errata/siwg917-soc-ic-errata.pdf) |

SDK pins in the `.slcp` files: WiSeConnect 4.1.0, Simplicity SDK 2026.6.0.
