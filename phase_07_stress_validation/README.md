# Phase 7 — Stress testing and validation

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Stress Phase 6 Si917 stack. Pass if schedule accuracy stays **&lt; 200 µs** on LA (master/RX reference vs RX schedule GPIO).

**Firmware:** Phase 6 Si917 binaries only. Host tooling here.

```text
Flash:  phase_06_wifi_commands/{master,receiver}_si917 only
Drive:  phase_07_stress_validation/host/pts_stress.py
Probe:  LA on master LED1 vs RX LED1 (schedule)
```

## Pass / fail

| Metric | Pass |
|--------|------|
| Schedule accuracy (LA) | &lt; 200 µs |
| Late / reject storm under nominal lead | no sustained rejects once locked |
| Sync recovery | RX returns locked after loss / restart / late join |

## Test matrix

| ID | Stress | How | Automate |
|----|--------|-----|----------|
| T1 | Baseline | `pts_stress.py --scenario baseline` | Host |
| T2 | Long drift | `--scenario drift --duration-s 3600` | Host + LA |
| T3 | Missed BLE | Attenuate / block path | Manual |
| T4 | Sync loss/recover | Power-cycle RX | Manual |
| T5 | Master restart | Reset master | Manual |
| T6 | Late join | Start RX after train ≥30 s | Manual |
| T7 | Wi-Fi coexist | `--scenario wifi-load` | Host + LA |
| T8 | RTOS/IRQ load | T7 + VCOM spam / join churn | Manual+T7 |
| T9 | Timer rollover | ≥30 s locked then ON/OFF | Manual |
| T10 | Sync interval | Rebuild P6 `PTS_PERIODIC_INTERVAL_*` | Rebuild |
| T11 | Lead sweep | `--scenario lead-sweep` (≥200 ms) | Host + LA |

## Host usage

P6: host talks to **master only** (`ON`/`OFF` → BLE PTS6). RX has no Wi-Fi.

```bash
cd phase_07_stress_validation/host

python pts_stress.py --master 172.20.10.6 \
  --scenario baseline --results ../results/t1.csv
```

Same for `drift` / `wifi-load` / `lead-sweep`. Default `--lead-ms 500`.

LA: master LED1 vs RX LED1. Fill `skew_us` in the CSV after capture.

## Exit criteria

- [ ] T1 accuracy recorded
- [ ] T2 ≥ 1 h drift; trend noted
- [ ] T3–T6 recovery observed
- [ ] T7 vs T1 under Wi-Fi load
- [ ] T11 lead floor documented
- [ ] At least one condition &lt; 200 µs (prefer T1 and T7)

## Wiring / flash

Phase 6 Si917 only. See [phase_06_wifi_commands/README.md](../phase_06_wifi_commands/README.md). Do not flash parked `bg24_capture/`.
