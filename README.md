# Precision Time Synchronization POC

Phase-first BLE time-sync POC. Design: [BLE_Time_Synchronization_POC_Roadmap(2).md](BLE_Time_Synchronization_POC_Roadmap(2).md).

## Topology (all phases) — board constraint

| Boards | Role |
|--------|------|
| **1× Si917 SoC** | Master (Periodic Adv + optional Wi-Fi stamp) |
| **1× Si917 SoC** | Receiver (sync, `R[N]`, network time, scheduled GPIO) |

**No BG24.** Si917 master + Si917 RX only.

**Trade-off:** `R[N]` and schedule live on the RX Si917 callback/timer path — no PRS hardware capture.

## SDK pins (Si917 `.slcp`)

| Package | Version |
|---------|---------|
| WiSeConnect (`wiseconnect3_sdk`) | **4.1.1** |
| Simplicity SDK | **2026.6.1** |

Flash NWP from the **same** WiSeConnect 4.1.1 package.

## Layout

```text
precision_time_sync_poc/
  phase_01_ble_transport/          # 1M + 1RX Periodic Adv + GPIO baseline
  phase_02_si917_bg24_event_ref/   # RX local R[N]
  phase_03_trigger_followup/       # M[N] ↔ R[N] on RX
  phase_04_logical_network_time/   # Network time on RX Si917
  phase_05_scheduled_gpio/         # Scheduled GPIO on RX
  phase_06_wifi_commands/          # Wi-Fi UDP commands; BLE keeps time
  phase_07_stress_validation/      # Stress; reuse Phase 6 Si917 firmware
```

LA: master LED0 vs RX LED0 (`ULP_GPIO_2`, P1+); RX schedule LED1 from P5.

## Phase index

| Phase | Goal | Details |
|-------|------|---------|
| 1 | Periodic Adv; master↔RX GPIO delay | [IMPORT.md](phase_01_ble_transport/IMPORT.md) |
| 2 | RX samples `R[N]` at AE callback | [README](phase_02_si917_bg24_event_ref/README.md) |
| 3 | Follow-up pairs `M[N]` ↔ `R[N]` on RX | [README](phase_03_trigger_followup/README.md) |
| 4 | Logical network time on RX Si917 | [README](phase_04_logical_network_time/README.md) |
| 5 | Schedule GPIO at future network time | [README](phase_05_scheduled_gpio/README.md) |
| 6 | Wi-Fi UDP `PTS6`; BLE = time only | [README](phase_06_wifi_commands/README.md) |
| 7 | Stress / accuracy &lt; 200 µs | [README](phase_07_stress_validation/README.md) |

## Phase 1 — BLE synchronization transport

**Goal:** Master PA train; one RX syncs; LA master↔RX GPIO6 delay.

| Role | Project |
|------|---------|
| Master | `phase_01_ble_transport/master_si917/pts_master_si917_soc.slcp` |
| Receiver | `phase_01_ble_transport/receiver_1_si917/pts_receiver_1_si917_soc.slcp` |

Base: [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral) / [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central). Build: [IMPORT.md](phase_01_ble_transport/IMPORT.md).

### Exit criteria

- [ ] Master seq `N` @ ~20 ms Periodic Adv
- [ ] RX sync established; seq/miss/RSSI logs
- [ ] GPIO toggle master + RX
- [ ] LA master GPIO6 vs RX GPIO6 delay recorded

## Phases 2–7

Same boards. Receiver owns time model and schedule. Open each phase README.

**Phase 2 flash:** keep Phase 1 master; flash `phase_02_si917_bg24_event_ref/receiver_si917` only.  
**Phase 3 flash:** both `phase_03_trigger_followup/{master,receiver}_si917` (PTS3 payload; no BG24).

| Phase | One-line |
|-------|----------|
| 2 | `R[N]` = RX SysTimer at AE report + GPIO pulse |
| 3 | Follow-up `M`; RX logs `M[N]<->R[N]` |
| 4 | Offset + rate network time on RX |
| 5 | Arm local schedule; toggle RX GPIO at `exec_m` |
| 6 | Host UDP → RX; BLE PTS4 sync only |
| 7 | Stress matrix on Phase 6 Si917 binaries |
