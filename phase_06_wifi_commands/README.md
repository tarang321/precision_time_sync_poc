# Phase 6 — Wi-Fi command transport

**Goal:** BLE keeps network time. Wi-Fi delivers schedule commands. Measure GPIO skew with Wi-Fi traffic active.

```text
Linux PC  --UDP-->  Master :5005   TIME / SCHED  (stamp exec_m in SysTimer domain)
Linux PC  --UDP-->  RX :5006       PTS6,<cmd_id>,<type>,<exec_m>,0
Receiver  --UART--> BG24           same as Phase 5 PTS5 host line
BG24                TIMER0 CC1     toggle PC02 at exec_m
```

BLE Periodic Adv stays **PTS4 only** (no schedule AD on air).

## Setup

1. Edit SSID/PSK in both:
   - `master_si917/config/sl_net_default_values.h`
   - `receiver_si917/config/sl_net_default_values.h`
2. Flash `.rps` (Si917) / `.hex` (BG24). Wiring unchanged (GPIO6→PC00, GPIO7→PC01, probe PC02).
3. Note each board’s IPv4 from VCOM (`PTS wifi IP a.b.c.d`).
4. Put a Linux host on the same AP/LAN.

## Linux interface (after firmware)

Script: `host/pts_wifi_cmd.py` (stdlib only).

```bash
cd phase_06_wifi_commands/host
python3 pts_wifi_cmd.py \
  --master 192.168.1.50 \
  --receivers 192.168.1.51,192.168.1.52 \
  --lead-ms 100
```

What it does:

1. `TIME` → master → gets `m` and SysTimer Hz  
2. `exec_m = m + hz * lead_ms / 1000`  
3. Sends `PTS6,...` UDP to each receiver (repeated)  
4. Receiver soft-UART → BG24 arms CC1  

Optional: `--via-master-sched` uses master `SCHED,<id>,1,<lead_ms>` for the stamp, then still fans out `PTS6` from the PC.

### Manual UDP (no script)

```bash
# Query master time
echo -n 'TIME' | nc -u -w1 192.168.1.50 5005

# Deliver absolute command to a receiver
echo -n 'PTS6,42,1,<exec_m>,0' | nc -u -w1 192.168.1.51 5006
```

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p6.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p6.slcp` | `.rps` |
| BG24 | `bg24_capture/pts_bg24_wifi_cmd.slcp` | `.hex` (same schedule path as Phase 5) |

## Exit criteria

- [ ] Boards join AP; VCOM shows IP and UDP listen port
- [ ] Linux `TIME` / `PTS6` produces BG24 `PTS sched arm` and PC02 edge
- [ ] LA skew with Wi-Fi traffic recorded (vs Phase 5 baseline)

## Trade-off

UDP is lossy; script repeats. Wi-Fi join/DHCP adds coexistence load next to BLE AE — that is intentional for Phase 6/7 measurement.
