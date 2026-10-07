# Phase 4 — Logical network time on receiver Si917

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Turn Phase 3 pairs into a conversion owned by the **receiver**. Do not reset the free-running local counter.

```text
Network time = M0 + (rate_num / rate_den) × (local_now − R0)
```

`M` and `R` are both Si917 SysTimer (or chosen local) counts in this topology. `rate_num/rate_den` is the smoothed ratio. New master session id clears the fit.

## Model

| State | Meaning |
|-------|---------|
| unlocked | No anchor, or session just changed |
| acquiring | One pair stored, rate not set |
| locked | Rate accepted; `pts_network_time_now()` valid |
| holdover | No accepted pair for ~eight intervals; still extrapolates |

Reject samples when deltas are not forward, seq does not advance, or ratio is far off the smoothed rate.

## Packet

Phase 3 layout plus session id. Magic `PTS4`.

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p4.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p4.slcp` | `.rps` |

**Parked:** `bg24_capture/`.

## Exit criteria

- [ ] RX reaches `quality=locked`
- [ ] Session change clears model
- [ ] Holdover then re-lock after brief miss

## References

| Used for | Reference |
|----------|-----------|
| Section 5.3 | [Roadmap](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 4 |
| Pairs | Phase 3 |
