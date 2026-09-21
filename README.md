# Precision Time Synchronization POC

Phase-first layout for the BLE time-sync proof of concept. Design source: [BLE_Time_Synchronization_POC_Roadmap(2).md](BLE_Time_Synchronization_POC_Roadmap(2).md).

## Layout

```text
precision_time_sync_poc/
  phase_01_ble_transport/          # BLE Periodic Adv transport
    master_si917/
    receiver_1_si917/
    receiver_2_si917/
  phase_02_si917_bg24_event_ref/   # active — Si917 GPIO → BG24 PRS R[N]
    common/
    receiver_si917/
    bg24_capture/
  phase_03_trigger_followup/       # active — trigger + follow-up M[N] <-> R[N]
    common/
    master_si917/
    receiver_si917/
    bg24_capture/
  phase_04_logical_network_time/  # active — BG24 offset + rate network time
  phase_05_scheduled_gpio/         # active — future-timestamp GPIO on BG24
  phase_06_wifi_commands/          # active — Wi-Fi UDP commands; BLE keeps time
  phase_07_stress_validation/      # active — stress matrix; reuse P6 FW
```

Phase 1 boards are three Si917s (1 master + 2 receivers). Phase 2 adds BG24 capture beside a Si917 receiver.

## Phase 1 — BLE synchronization transport

**Goal:** One-to-many Periodic Advertising train; both receivers sync to the same train; measure callback-level GPIO skew on a logic analyzer.

### Official WiSeConnect baselines

| Role | Example | Project in-tree |
|------|---------|-----------------|
| Master | [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral) | `phase_01_ble_transport/master_si917/pts_master_si917_soc.slcp` |
| Receiver 1 | [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central) | `phase_01_ble_transport/receiver_1_si917/pts_receiver_1_si917_soc.slcp` |
| Receiver 2 | same | `phase_01_ble_transport/receiver_2_si917/pts_receiver_2_si917_soc.slcp` |

Shared: `phase_01_ble_transport/common/` (payload + debug GPIO).

- Examples index: https://docs.silabs.com/wiseconnect/latest/wiseconnect-examples/
- AE APIs: https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-ble/bt-low-energy1
- Build notes and references: [phase_01_ble_transport/IMPORT.md](phase_01_ble_transport/IMPORT.md)

### Exit criteria

- [ ] Master transmits seq `N` at ~20–25 ms on Periodic Advertising
- [ ] Both receivers establish sync to the same advertising train
- [ ] Logs: sequence numbers, misses, RSSI, callback timing
- [ ] Debug GPIO toggled on each sync reception (both RX boards)
- [ ] Logic-analyzer skew between RX GPIO edges recorded (callback baseline)

## Phase 2 — Si917-to-BG24 BLE event reference

**Goal:** Hardware-capture `R[N]` on BG24 from Si917 event GPIO; associate delayed host identity frames with captures.

Details and references: [phase_02_si917_bg24_event_ref/README.md](phase_02_si917_bg24_event_ref/README.md).

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

## Later phases (stubs only)

| Phase | Folder | Roadmap focus |
|-------|--------|---------------|
| 3 | `phase_03_trigger_followup` | Section 5.2 trigger + follow-up `M[N]` ↔ `R[N]` — see that folder's README |
| 4 | `phase_04_logical_network_time` | Section 5.3 logical network time on BG24 — see that folder's README |
| 5 | `phase_05_scheduled_gpio` | Future-timestamp hardware GPIO execution — see that folder's README |
| 6 | `phase_06_wifi_commands` | Wi-Fi command transport; BLE keeps time — see that folder's README |
| 7 | `phase_07_stress_validation` | Drift, loss, restart, coexistence; skew &lt; 200 µs — see that folder's README |
