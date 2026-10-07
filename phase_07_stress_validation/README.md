# Phase 7 — Stress testing and validation

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Stress Phase 6 Si917 stack. Pass if schedule accuracy stays **&lt; 200 µs** on LA (master/RX reference vs RX schedule GPIO).

**Firmware:** Phase 6 Si917 binaries only. Host tooling here.

```text
Flash:  phase_06_wifi_commands/{master,receiver}_si917 only
Drive:  phase_07_stress_validation/host/pts_stress.py
Probe:  LA on RX schedule GPIO (+ optional master/RX GPIO6)
```

## Pass / fail

| Metric | Pass |
|--------|------|
| Schedule accuracy (LA) | &lt; 200 µs |
| Late / reject storm under nominal lead | no sustained rejects once locked |
| Sync recovery | RX returns locked after loss / restart / late join |

## Test matrix

| ID | Stress | How |
|----|--------|-----|
| T1 | Baseline | `pts_stress.py --scenario baseline` |
| T2 | Long drift | `--scenario drift --duration-s 3600` |
| T3 | Missed BLE | Attenuate / brief master TX off |
| T4 | Sync loss/recover | Power-cycle RX |
| T5 | Master restart | Reset master |
| T6 | Late join | Start RX after train running |
| T7 | Wi-Fi coexist | `--scenario wifi-load` |
| T8 | RTOS/IRQ load | T7 + VCOM spam / join churn |
| T9 | Timer rollover | Long run across counter wrap |
| T10 | Sync interval | Rebuild P6 master intervals |
| T11 | Lead sweep | `--scenario lead-sweep` |

## Host usage

```bash
cd phase_07_stress_validation/host

python3 pts_stress.py --master 192.168.1.50 \
  --receivers 192.168.1.51 \
  --scenario baseline --results ../results/t1.csv
```

Same pattern for drift / wifi-load / lead-sweep (single RX IP).

## Exit criteria

- [ ] T1 accuracy recorded
- [ ] T2 ≥ 1 h drift; trend noted
- [ ] T3–T6 recovery observed
- [ ] T7 vs T1 under Wi-Fi load
- [ ] T11 lead floor documented
- [ ] At least one condition &lt; 200 µs (prefer T1 and T7)

## Wiring / flash

Phase 6 Si917 only. See [phase_06_wifi_commands/README.md](../phase_06_wifi_commands/README.md). Do not flash parked `bg24_capture/`.
