# Phase 2 — Receiver local event reference `R[N]`

**Topology:** 1× Si917 SoC master (Phase 1) + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Best-effort local timestamp `R[N]` on the receiver at the Periodic Adv report, plus debug GPIO for LA. No second MCU.

## Active project

| Role | Project |
|------|---------|
| Master | `phase_01_ble_transport/master_si917/` (reuse) |
| Receiver | `receiver_si917/pts_receiver_si917_p2.slcp` |

**Parked:** `bg24_capture/` (original PRS/TIMER path — not used).

## Behavior (Si917-only)

```text
AE report N → sample R[N] (osKernelGetSysTimerCount or equiv)
            → toggle ULP_GPIO6 (EXP16)   [PTS_GPIO_SANITY_TOGGLE=0]
            → log seq ↔ R

Sanity (default): PTS_GPIO_SANITY_TOGGLE=1 → free-run ~2 Hz on EXP16, no BLE.
```

Constraints (unchanged):

- Periodic Adv report: no controller RX timestamp.
- SiWG917 GPIO Timestamp Controller removed (`GPIO_TIMESTAMP_E301`).
- Closest app-visible edge: start of AE report callback.

## Wiring / LA

| Signal | Board | Notes |
|--------|-------|-------|
| Event edge | RX **EXP16** (`ULP_GPIO_6`) | Toggle (not µs pulse) |
| Optional | Master breakout P19 / GPIO6 | Phase 1 baseline |
| GND | EXP1 or nearby | Required |

## Exit criteria

- [ ] Sanity: EXP16 toggles ~2 Hz with `PTS_GPIO_SANITY_TOGGLE=1` (no master)
- [ ] Phase2: set `PTS_GPIO_SANITY_TOGGLE=0`; toggle on each AE report
- [ ] Logs show `seq` ↔ `R[N]` (SysTimer ticks, every 10 seq)
- [ ] LA optional: EXP16 edges align with report cadence (~20 ms)
- [ ] No dependency on BG24 / UART identity path

## Flash

| Role | Firmware |
|------|----------|
| Master | Reuse `phase_01_ble_transport/master_si917` |
| Receiver | `receiver_si917/pts_receiver_si917_p2.slcp` |

## Soft-UART / host TX

`pts_host_tx.*` / `pts_event_ref_protocol.h` remain in `common/` but are **not linked** (parked BG24 path). Active path is VCOM `seq`/`R` logs.

## Next

Phase 3: correlate master `M[N]` with this `R[N]` on the same RX.

## References

| Used for | Reference |
|----------|-----------|
| Roadmap Phase 2 (Si917-only) | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) |
| RX base | Fork of `phase_01_ble_transport/receiver_1_si917/` |
| Payload | `phase_01_ble_transport/common/pts_sync_protocol.h` |
| No Si917 GPIO timestamp HW | [SiWG917 errata `GPIO_TIMESTAMP_E301`](https://www.silabs.com/documents/public/errata/siwg917-soc-ic-errata.pdf) |
