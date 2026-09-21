# Phase 4 — Logical network time on BG24

**Goal:** Turn Phase 3 pairs into one conversion owned by the BG24. The hardware timer is not reset.

```text
Network time = M0 + (rate_num / rate_den) × (TIMER0 now − R0)
```

`M` is Si917 SysTimer counts. `R` is BG24 TIMER0 ticks. `rate_num/rate_den` is the smoothed ratio. A new master session id clears the fit.

## Model

| State | Meaning |
|-------|---------|
| unlocked | No anchor, or session just changed |
| acquiring | One pair stored, rate not set |
| locked | Rate accepted. `pts_network_time_now()` is valid |
| holdover | No accepted pair for about eight intervals. Still extrapolates |

A sample is rejected when the master or local delta is not forward, the sequence does not advance, or the new ratio is more than 12.5% off the smoothed rate. Deltas use `int32` subtraction so a `uint32` rollover is valid if the true gap fits in signed 32 bits.

## Packet

Phase 3 layout plus a session id. Magic is `PTS4`. 24-byte AD. Host line:

```text
PTS4,<board>,<trigger_seq>,<rssi>,<fu_seq>,<M>,<session>
```

Session is `osKernelGetSysTimerCount() | 1` once per master boot.

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p4.slcp` | `pts_master_si917_p4.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p4.slcp` | `pts_receiver_si917_p4.rps` |
| BG24 | `bg24_capture/pts_bg24_network_time.slcp` | `pts_bg24_network_time.hex` |

Wiring is unchanged: GPIO6 → PC00, GPIO7 → PC01.

## References

| Used for | Reference |
|----------|-----------|
| Section 5.3 formula and rules | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 4 |
| Pairs `M[N]` ↔ `R[N]` | Phase 3 `phase_03_trigger_followup/` |
| TIMER0 free-running counter | `TIMER_CounterGet()` in SiSDK `em_timer.h` |
