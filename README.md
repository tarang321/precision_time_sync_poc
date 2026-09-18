# Precision Time Synchronization POC

Phase-first layout for the BLE time-sync proof of concept. Design source: [BLE_Time_Synchronization_POC_Roadmap(2).md](BLE_Time_Synchronization_POC_Roadmap(2).md).

## Layout

```text
precision_time_sync_poc/
  phase_01_ble_transport/          # active — BLE Periodic Adv transport
    master_si917/
    receiver_1_si917/
    receiver_2_si917/
  phase_02_si917_bg24_event_ref/   # stub
  phase_03_trigger_followup/       # stub
  phase_04_logical_network_time/   # stub
  phase_05_scheduled_gpio/         # stub
  phase_06_wifi_commands/          # stub
  phase_07_stress_validation/      # stub
```

Phase 1 boards are three Si917s (1 master + 2 receivers). BG24 work starts in Phase 2.

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
- Build notes: [phase_01_ble_transport/IMPORT.md](phase_01_ble_transport/IMPORT.md)

### Exit criteria

- [ ] Master transmits seq `N` at ~20–25 ms on Periodic Advertising
- [ ] Both receivers establish sync to the same advertising train
- [ ] Logs: sequence numbers, misses, RSSI, callback timing
- [ ] Debug GPIO toggled on each sync reception (both RX boards)
- [ ] Logic-analyzer skew between RX GPIO edges recorded (callback baseline)

Open item (Phase 2): Si917 controller-level BLE TX/RX timestamps / radio-event GPIO.

## Later phases (stubs only)

| Phase | Folder | Roadmap focus |
|-------|--------|---------------|
| 2 | `phase_02_si917_bg24_event_ref` | Si917 GPIO edge → BG24 PRS timer capture `R[N]` |
| 3 | `phase_03_trigger_followup` | Section 5.2 trigger + follow-up `M[N]` ↔ `R[N]` |
| 4 | `phase_04_logical_network_time` | Section 5.3 logical network time on BG24 |
| 5 | `phase_05_scheduled_gpio` | Future-timestamp hardware GPIO execution |
| 6 | `phase_06_wifi_commands` | Wi-Fi command transport; BLE keeps time |
| 7 | `phase_07_stress_validation` | Drift, loss, restart, coexistence; skew &lt; 200 µs |
