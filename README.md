# Precision Time Synchronization POC

Phase-first BLE time-sync proof of concept. Design source: [BLE_Time_Synchronization_POC_Roadmap(2).md](BLE_Time_Synchronization_POC_Roadmap(2).md).

## SDK pins (all `.slcp`)

| Package | Version | Used by |
|---------|---------|---------|
| WiSeConnect (`wiseconnect3_sdk`) | **4.1.1** | Si917 projects only |
| Simplicity SDK | **2026.6.1** | Si917 + BG24 |

Open each `.slcp` in Simplicity Studio → Generate → Build. Use NWP firmware from the **same** WiSeConnect 4.1.1 package.

## Layout

```text
precision_time_sync_poc/
  phase_01_ble_transport/          # BLE Periodic Adv transport (3× Si917)
  phase_02_si917_bg24_event_ref/   # Si917 GPIO → BG24 PRS R[N]
  phase_03_trigger_followup/       # Trigger + follow-up M[N] ↔ R[N]
  phase_04_logical_network_time/   # BG24 offset + rate network time
  phase_05_scheduled_gpio/         # Future-timestamp GPIO on BG24 (BLE PTS5)
  phase_06_wifi_commands/          # Wi-Fi UDP commands; BLE keeps time
  phase_07_stress_validation/      # Stress matrix; reuse Phase 6 firmware
```

Phase 1: three Si917s (1 master + 2 receivers). Phase 2+: add BG24 capture beside each Si917 receiver (GPIO6→PC00, GPIO7→PC01; probe PC02 from Phase 5).

## Phase index

| Phase | Goal | Details |
|-------|------|---------|
| 1 | Periodic Adv train; callback GPIO skew baseline | [IMPORT.md](phase_01_ble_transport/IMPORT.md) |
| 2 | Hardware `R[N]` via PRS/TIMER0; delayed UART identity | [README](phase_02_si917_bg24_event_ref/README.md) |
| 3 | Pair master `M[N]` with stored `R[N]` (follow-up AD) | [README](phase_03_trigger_followup/README.md) |
| 4 | Logical network time on BG24 (offset + rate) | [README](phase_04_logical_network_time/README.md) |
| 5 | Schedule GPIO at future network time (PTS5 on BLE) | [README](phase_05_scheduled_gpio/README.md) |
| 6 | Same schedule path; commands over Wi-Fi UDP (`PTS6`) | [README](phase_06_wifi_commands/README.md) |
| 7 | Stress / skew &lt; 200 µs; host scripts only | [README](phase_07_stress_validation/README.md) |

## Phase 1 — BLE synchronization transport

**Goal:** One-to-many Periodic Advertising; both receivers sync to the same train; measure callback-level GPIO skew on a logic analyzer.

### Official WiSeConnect baselines

| Role | Example | Project in-tree |
|------|---------|-----------------|
| Master | [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral) | `phase_01_ble_transport/master_si917/pts_master_si917_soc.slcp` |
| Receiver 1 | [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central) | `phase_01_ble_transport/receiver_1_si917/pts_receiver_1_si917_soc.slcp` |
| Receiver 2 | same | `phase_01_ble_transport/receiver_2_si917/pts_receiver_2_si917_soc.slcp` |

Shared: `phase_01_ble_transport/common/` (payload + debug GPIO).

- Examples: https://docs.silabs.com/wiseconnect/latest/wiseconnect-examples/
- AE APIs: https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-ble/bt-low-energy1
- Build notes: [phase_01_ble_transport/IMPORT.md](phase_01_ble_transport/IMPORT.md)

### Exit criteria

- [ ] Master transmits seq `N` at ~20–25 ms on Periodic Advertising
- [ ] Both receivers establish sync to the same advertising train
- [ ] Logs: sequence numbers, misses, RSSI, callback timing
- [ ] Debug GPIO toggled on each sync reception (both RX boards)
- [ ] Logic-analyzer skew between RX GPIO edges recorded (callback baseline)

## Phase 2 — Si917-to-BG24 BLE event reference

**Goal:** Hardware-capture `R[N]` on BG24 from Si917 event GPIO; associate delayed host identity frames with captures.

Details: [phase_02_si917_bg24_event_ref/README.md](phase_02_si917_bg24_event_ref/README.md).

| Role | Project |
|------|---------|
| Master (reuse P1) | `phase_01_ble_transport/master_si917/` |
| Receiver Si917 | `phase_02_si917_bg24_event_ref/receiver_si917/pts_receiver_si917_p2.slcp` |
| BG24 capture | `phase_02_si917_bg24_event_ref/bg24_capture/pts_bg24_capture.slcp` |

Path: Si917 AE callback GPIO pulse → BG24 async PRS → TIMER0 capture `R` → later `PTS2` UART frame binds `seq`.

### Exit criteria

- [ ] Event GPIO rising edge on each Periodic Adv report
- [ ] BG24 records `R` in TIMER ISR (independent of UART latency)
- [ ] Logs show `seq` ↔ `R` pairs
- [ ] Confirmed: no Si917 controller RX timestamp / GPIO timestamp HW (callback edge is best available)

## Phases 3–7 (active)

Not stubs — full firmware (3–6) or host tooling (7). Open the phase README for projects, wiring, and exit criteria.

| Phase | One-line |
|-------|----------|
| 3 | Follow-up carries prior `M`; BG24 logs `M[N]<->R[N]` |
| 4 | Convert pairs → network time model on BG24 |
| 5 | Arm TIMER0 CC1; toggle PC02 at `exec_m` (BLE schedule AD) |
| 6 | BLE = PTS4 only; Linux/host stamps via UDP `TIME` / `PTS6` |
| 7 | Stress matrix on Phase 6 binaries; LA skew is ground truth |
