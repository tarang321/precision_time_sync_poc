# BLE Time-Synchronization POC Roadmap

## Purpose

POC for BLE time sync on **SiWG917 SoC only**, under current board constraints.

- **Section 5.2 — Trigger plus follow-up synchronization**
- **Section 5.3 — Logical network time**
- Receiver Si917 owns and maintains logical network time
- Future-timestamp-based command execution on the receiver GPIO

**Topology (all phases):** **1× Si917 SoC master + 1× Si917 SoC receiver.**  
No BG24. In-tree `bg24_capture/` trees are **parked** (not required). Multi-RX is out of scope (`receiver_2_si917/` parked).

**Trade-off:** Without BG24 PRS/TIMER capture, `R[N]` is sampled in the AE report callback (`osKernelGetSysTimerCount()` or equivalent). Fixed offsets can be calibrated; callback jitter cannot. LA on master GPIO6 vs RX GPIO6 (and later RX schedule GPIO) is ground truth.

## Proposed Architecture

```text
Master Si917 (SoC)
    |
    | BLE Periodic Adv: trigger N + follow-up M[N-1]
    v
Receiver Si917 (SoC)
    |
    | Sample R[N] at AE report callback (+ debug GPIO edge)
    | Pair M[N] <-> R[N] from follow-up
    | Maintain logical network time (offset + rate)
    | Arm local timer / schedule GPIO at future network time
```

The master does **not** own the receiver clock. It emits sync/follow-up (and optional Wi-Fi stamps). The receiver maps local time to master time and executes scheduled edges.

## Phase 1 — Validate the BLE Synchronization Transport

- One Si917 SoC master, one Si917 SoC receiver.
- BLE Periodic Advertising (~20 ms).
- Confirm RX sync; log seq / miss / RSSI.
- Toggle GPIO on master seq tick and RX sync report.
- LA: master GPIO6 vs RX GPIO6 delay (callback baseline).

### Relevant WiSeConnect APIs

Master: `rsi_ble_set_ae_*`, periodic AE params/enable, `rsi_ble_set_ae_data()`.  
Receiver: AE scan + `rsi_ble_ae_set_periodic_sync()` + report callbacks.

## Phase 2 — Receiver local event reference `R[N]`

Establish the best available local timestamp on the **receiver Si917** at the BLE event:

1. AE periodic-adv report for sequence `N`.
2. Sample `R[N]` immediately (SysTimer / free-running counter).
3. Pulse debug GPIO for LA.
4. Log `seq` ↔ `R[N]` (no second MCU).

No controller RX timestamp and no Si917 GPIO timestamp HW (`GPIO_TIMESTAMP_E301`) — callback edge is the baseline.

## Phase 3 — Trigger plus follow-up

1. Master transmits trigger `N` and samples `M[N]` at its TX/update boundary.
2. Receiver samples `R[N]` on the corresponding report.
3. Later packet carries follow-up `M` for prior `N`.
4. Receiver logs `M[N] <-> R[N]`.

Follow-up delay must not redefine `R[N]`.

## Phase 4 — Logical network time on receiver Si917

```text
Network time = M0 + rate × (local_now − R0)
```

Offset + rate on the receiver. Reject bad samples; holdover on short loss; session id on master restart. Do not reset the free-running local counter.

## Phase 5 — Scheduled GPIO on receiver

- Command carries future network execution time (~100 ms lead).
- Receiver converts to local timer compare (or best available schedule path).
- Toggle demo GPIO from the timer path, not from the receive path.
- LA: schedule accuracy vs master reference / expected lead (&lt; 200 µs target).

## Phase 6 — Wi-Fi command transport

- Both Si917s join Wi-Fi AP.
- BLE maintains network time (PTS4-class sync only).
- Host UDP delivers schedule (`TIME` / `PTS6`) to the receiver.
- LA with Wi-Fi coexistence load.

## Phase 7 — Stress / validation

Same 1M+1RX stack. Stress matrix: drift, miss, loss/recover, master restart, late join, Wi-Fi load, interval/lead sweeps. Pass: schedule accuracy **&lt; 200 µs** on LA.

## Implementation Order

```text
P1 Periodic Adv + master↔RX GPIO delay
        |
        v
P2 RX local R[N] at callback
        |
        v
P3 M[N] <-> R[N] follow-up
        |
        v
P4 RX network-time model
        |
        v
P5 RX scheduled GPIO
        |
        v
P6 Wi-Fi command path
        |
        v
P7 Stress
```

## Parked / future

- BG24 PRS capture + HW compare (original design) — restore when a BG24 kit is available.
- Second RX / multi-board skew.
- BIG/BIS, TWS-style filters — after base Si917 path works.

## Items for Review

- Best local counter for `R[N]` / schedule on Si917 SoC (SysTimer vs GPTIMER / other).
- Whether master can get a true TX-event timestamp for follow-up `M[N]`.
- Schedule path: HW compare vs software timer (accuracy vs complexity).
