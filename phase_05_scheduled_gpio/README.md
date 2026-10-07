# Phase 5 — Scheduled GPIO on receiver Si917

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Receive a future network timestamp, convert to local schedule, toggle a demo GPIO from the timer path — not from the BLE receive path.

```text
Master:   PA carries PTS4 sync + PTS5 schedule (exec_m ≈ now + 100 ms)
Receiver: keep Phase 4 clock model; arm local compare/schedule; toggle GPIO
```

Same `cmd_id` executes once.

## Schedule command

Magic `PTS5`. Fields: `cmd_id`, `cmd_type` (1 = GPIO toggle), `status`, `exec_m`.

Master emits with ~100 ms lead after RX can lock.

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p5.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p5.slcp` | `.rps` |

**Parked:** `bg24_capture/`.

LA: probe RX schedule GPIO (and optionally master GPIO6). Common GND. No PC02.

## Exit criteria

- [ ] Master log: `PTS sched cmd=... exec_m=...`
- [ ] RX log: arm then GPIO edge at schedule
- [ ] Duplicate `cmd_id` does not re-arm
- [ ] LA schedule accuracy vs lead / master reference (target &lt; 200 µs)

## References

| Used for | Reference |
|----------|-----------|
| Roadmap Phase 5 | [Roadmap](../BLE_Time_Synchronization_POC_Roadmap(2).md) |
| Network time | Phase 4 |
