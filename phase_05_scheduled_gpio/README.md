# Phase 5 — Scheduled GPIO on receiver Si917

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Receive a future network timestamp, convert to local schedule, toggle demo GPIO (LED0) from a timer thread — not from the BLE receive path.

```text
Master:   PA = PTS5 sync + PTS6 schedule; arm LED1 at exec_m (M == SysTimer)
Receiver: lock → convert exec_m → local R; arm LED1 at same network time
```

Both boards pulse **LED1** (idle high → low pulse) at the same network time.
Master adds `PTS_SCHED_MASTER_COMP_MS` (default **7 ms**, LA-tuned) on local arm so push-time M matches air-time R.

| LED | Pin | Role |
|-----|-----|------|
| LED0 | ULP_GPIO_2 | BT / PA / seq edge only |
| LED1 | HP GPIO_10 | Schedule pulse (both boards, same polarity) |

Same `cmd_id` executes once.

## Packets

| AD | Magic | Role |
|----|-------|------|
| Sync / follow-up | `PTS5` (33 B) | K=4 M history + session (from P4 hard-pass) |
| Schedule | `PTS6` (20 B) | `cmd_id`, type=GPIO toggle, `exec_m` |

Master appends PTS6 for 4 PA events per command, every ~25 packets (~750 ms at 30 ms).

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p5.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p5.slcp` | `.rps` |

**Parked:** `bg24_capture/`.

LA: probe RX LED0 (BT) + LED1 (schedule). Optionally master LED0. Common GND.

## Exit criteria

- [ ] Master: `PTS sched cmd=...` + `master sched FIRE` (LED1)
- [ ] RX: `sched ARM` then `sched FIRE` (~100 ms); LED1
- [ ] LA: both LED1 edges align (target &lt; 200 µs)
- [ ] Duplicate `cmd_id` does not re-arm
- [ ] Lock quality holds while scheduling

## References

| Used for | Reference |
|----------|-----------|
| Roadmap Phase 5 | [Roadmap](../BLE_Time_Synchronization_POC_Roadmap(2).md) |
| Network time | Phase 4 hard-pass |
