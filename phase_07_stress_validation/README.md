# Phase 7 — Stress testing and validation

**Goal:** Prove Phase 6 stack under adverse conditions. Pass if multi-board PC02 edge skew stays **&lt; 200 µs** on the same logic analyzer.

**Firmware:** reuse Phase 6 binaries (no P7 board tree). Host tooling lives here.

```text
Flash:  phase_06_wifi_commands/{master,receiver,bg24}_*
Drive:  phase_07_stress_validation/host/pts_stress.py
Probe:  LA on each BG24 PC02 (+ optional RX GPIO6)
```

## Pass / fail

| Metric | Pass |
|--------|------|
| Skew (max − min PC02 edge, same `cmd_id`) | &lt; 200 µs |
| Late / reject storm under nominal lead | no sustained `PTS sched reject` once locked |
| Sync recovery | BG24 returns `quality=locked` after loss / restart / late join |

Record skew in `results/` (CSV + LA capture notes). Host only stamps/delivers; skew is LA-measured.

## Test matrix

| ID | Stress | How | What to watch |
|----|--------|-----|----------------|
| T1 | Baseline | `pts_stress.py --scenario baseline` | Skew vs Phase 5/6 |
| T2 | Long drift | `--scenario drift --duration-s 3600` | Skew over time; holdover exits |
| T3 | Missed BLE | Attenuate / brief master TX off | `MISS` logs; quality holdover→locked |
| T4 | Sync loss/recover | Power-cycle one RX Si917+BG24 | Re-lock; next SCHED executes |
| T5 | Master restart | Reset master | New `session`; RX re-sync; schedule resumes |
| T6 | Late join | Start one RX after train running | Locks; participates in next cmd |
| T7 | Wi-Fi coexist | `--scenario wifi-load` (UDP flood + SCHED) | Skew vs T1 |
| T8 | RTOS/IRQ load | Same as T7 + VCOM spam / Wi-Fi join churn | Arm latency / rejects |
| T9 | Timer rollover | Run &gt; TIMER0 wrap (~110 s @ 39 MHz) or long drift | No stuck schedule; int32 deltas OK |
| T10 | Sync interval | Rebuild P6 master: `PTS_PERIODIC_INTERVAL_*` (e.g. 0x10 / 0x20 / 0x40) | Skew vs interval |
| T11 | Lead sweep | `--scenario lead-sweep` | Min workable lead; rejects below it |

Manual: T3–T6, T9 (partial), T10. Scripted: T1, T2, T7, T8, T11.

## Host usage

```bash
cd phase_07_stress_validation/host

# T1 baseline — one command, lead 100 ms
python3 pts_stress.py --master 192.168.1.50 \
  --receivers 192.168.1.51,192.168.1.52 \
  --scenario baseline --results ../results/t1.csv

# T2 long drift — command every 2 s for 1 h
python3 pts_stress.py --master 192.168.1.50 \
  --receivers 192.168.1.51,192.168.1.52 \
  --scenario drift --duration-s 3600 --period-s 2 \
  --results ../results/t2.csv

# T7 coexistence — background UDP flood while scheduling
python3 pts_stress.py --master 192.168.1.50 \
  --receivers 192.168.1.51,192.168.1.52 \
  --scenario wifi-load --duration-s 300 --period-s 1 \
  --results ../results/t7.csv

# T11 lead sweep
python3 pts_stress.py --master 192.168.1.50 \
  --receivers 192.168.1.51,192.168.1.52 \
  --scenario lead-sweep --results ../results/t11.csv
```

`pts_stress.py` imports the Phase 6 UDP protocol (`TIME` / `PTS6`). Optional flood: `pts_wifi_load.py`.

## Interval rebuild (T10)

In `phase_06_wifi_commands/common/pts_sync_protocol.h`:

| Target period | `PTS_PERIODIC_INTERVAL_MIN/MAX` (1.25 ms units) |
|---------------|--------------------------------------------------|
| ~20 ms | `0x10` / `0x14` (default) |
| ~40 ms | `0x20` / `0x20` |
| ~80 ms | `0x40` / `0x40` |

Rebuild + reflash **master** (and keep RX sync timeout tolerant). Note interval in `results/*.csv` `notes` column.

## Rollover note (T9)

BG24 TIMER0 is free-running 32-bit. Model uses `int32` deltas (`pts_network_time.c`) — wrap is fine if true gap &lt; ~2^31 ticks (~55 s @ 39 MHz between accepted pairs). Stress: keep sync alive across at least one wrap; schedule during/after wrap.

## Wiring / flash

Unchanged from Phase 6. See [phase_06_wifi_commands/README.md](../phase_06_wifi_commands/README.md).

## Exit criteria

- [ ] T1 skew recorded (baseline)
- [ ] T2 ≥ 1 h drift run; skew trend noted
- [ ] T3–T6 recovery observed (logs + next GPIO edge)
- [ ] T7 Wi-Fi-on skew vs T1
- [ ] T11 lead floor documented
- [ ] At least one condition shows multi-board skew **&lt; 200 µs** under load (prefer T1 and T7)

## Trade-offs

- Host CSV is command log, not skew — LA is ground truth.
- Short lead reduces Wi-Fi margin; long lead hides coexistence jitter.
- Interval rebuild is invasive; do not change RX firmware for interval-only tests.
