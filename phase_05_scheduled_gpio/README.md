# Phase 5 — Scheduled GPIO execution

**Goal:** Receive a future network timestamp, convert it to a local TIMER0 compare, and toggle a demo GPIO from the timer IRQ — not from the receive path.

```text
Master:   PA carries PTS4 sync + PTS5 schedule (exec_m ≈ now + 100 ms)
Receiver: GPIO6 edge for R[N]; UART forwards PTS4 and PTS5 to BG24
BG24:     keep Phase 4 clock model; arm TIMER0 CC1; toggle PC02 on match
```

Command is buffered and executed at the specified logical network time. Repeated broadcasts with the same `cmd_id` execute once.

## Schedule command

Appended AD after PTS4 when a schedule is active. Magic `PTS5`. 20 bytes.

| Field | Meaning |
|-------|---------|
| cmd_id | Monotonic; dedupe key |
| cmd_type | `1` = GPIO toggle |
| status | `0` = valid |
| exec_m | Future network time (Si917 SysTimer counts) |

Host line:

```text
PTS5,<board>,<cmd_id>,<cmd_type>,<exec_m>,<status>
```

Master emits a new command about every 25 sync intervals (~500 ms), repeats it 4 times, with ~100 ms lead. First command waits ~50 intervals so BG24 can lock.

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p5.slcp` | `pts_master_si917_p5.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p5.slcp` | `pts_receiver_si917_p5.rps` |
| BG24 | `bg24_capture/pts_bg24_scheduled_gpio.slcp` | `pts_bg24_scheduled_gpio.hex` |

Wiring: GPIO6 → PC00 (capture), GPIO7 → PC01 (UART), probe **PC02** for scheduled edges. Common GND.

## Exit criteria

- [ ] Master log: `PTS sched cmd=... exec_m=... lead_ms=100`
- [ ] BG24 log: `PTS sched arm cmd=...` then GPIO edge on PC02
- [ ] Duplicate `cmd_id` does not re-arm
- [ ] Logic-analyzer skew across boards' PC02 edges recorded (target later: &lt; 200 µs)

## References

| Used for | Reference |
|----------|-----------|
| Section Phase 5 | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) |
| Network time model | Phase 4 `phase_04_logical_network_time/` |
| TIMER0 CC1 compare | SiSDK `TIMER_CompareSet()` / `em_timer.h` |
