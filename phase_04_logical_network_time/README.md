# Phase 4 — Logical network time on receiver Si917

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Turn Phase 3 pairs into a conversion owned by the **receiver**. Do not reset the free-running local counter.

```text
Network time = M0 + (rate_num / rate_den) × (local_now − R0)
```

`M` and `R` are both Si917 SysTimer counts. `rate_num/rate_den` is the long-baseline smoothed ratio. New master session id clears the fit.

## Model

| State | Meaning |
|-------|---------|
| unlocked | No anchor, or session just changed |
| acquiring | One pair stored, rate not set |
| locked | Rate accepted; `pts_network_time_now()` valid |
| holdover | No RX activity for ~32 intervals; still extrapolates |

Reject samples when deltas are not forward, seq does not advance, or ratio is far off the smoothed rate. Rate fits use a ~1 s baseline (`PTS_NT_RATE_MIN_DSEQ=32`) so M-sample jitter stays small.

## Packet

Magic `PTS5`, 33 bytes. Trigger seq N + session + M[N-1]..M[N-4] + valid_mask. A pair needs `R[k]` (from hearing trigger k) and `M[k]` from any later packet’s history — survives up to 3 lost follow-up carriers.

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p4.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p4.slcp` | `.rps` |

**Parked:** `bg24_capture/` (model lives on RX Si917: `common/pts_network_time.*`).

## Air / seq pacing

PA fixed **30 ms** (`0x18`); seq timer **30 ms**; seq commits only after successful `set_ae_data`; RX ignores duplicate seq. ~5% raw `miss/seq` on this bench is RF/NWP (BLE PA has no retransmission). Pairing uses K=4 history so effective pair loss (`fu_miss`) should stay near 0.

## Loss instrumentation (every 200 seq)

RX line includes: `rpt` (reports), `bad` (callback status≠0), `inc` (data_status incomplete), `dec` (decode fail), `rssi_min` / `avg`.

| Pattern | Meaning |
|---------|---------|
| High `dec` / `inc` | Truncated or corrupt payload |
| Clean reports, high `miss` | Packet never received (RF / radio schedule) |
| `rpt` rate ≪ 33/s | RX skipping PA events |

Master logs `push_ok` / `fail` every 200 seq.

## RF experiments (if miss stays high and counters show real loss)

1. Bring boards closer / fix orientation; keep TX 18 dBm.
2. Optional: set `PTS_COEX_BLE_ONLY` to `1` in both `app.c` files — **hypothesis only**; SDK notes BLE-only may require `bt_stack_bypass`. Revert if boot/AE fails.

## Exit criteria (hard)

- [ ] `quality=locked` ≥2 min (no holdover spam)
- [ ] Every 200 seq: `Rdrop=0`, `fit_ok/pairs` ≥ 0.95, `rej/pairs` ≤ 0.02, **`fu_miss/seq` ≤ 0.01**
- [ ] Raw air loss `miss/seq` reported as RF metric; bench target < 0.05 (not a hard 0)
- [ ] Master reboot → `session A->B unlocked (reboot gate)` then re-lock
- [ ] Brief RF block → `q=holdover` / sync LOST holdover gate; clear → locked

## References

| Used for | Reference |
|----------|-----------|
| Section 5.3 | [Roadmap](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 4 |
| Pairs | Phase 3 |
