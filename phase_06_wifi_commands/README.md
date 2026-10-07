# Phase 6 — Wi-Fi command transport

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** BLE keeps network time. Wi-Fi delivers schedule commands to the receiver. Measure GPIO schedule accuracy with Wi-Fi traffic active.

```text
Linux PC  --UDP-->  Master :5005   TIME / SCHED
Linux PC  --UDP-->  RX :5006       PTS6,<cmd_id>,<type>,<exec_m>,0
Receiver            arms local schedule GPIO at exec_m
```

BLE Periodic Adv stays **PTS4 sync only** (no schedule AD required on air once Wi-Fi path is used).

## Setup

1. Edit SSID/PSK in:
   - `master_si917/config/sl_net_default_values.h`
   - `receiver_si917/config/sl_net_default_values.h`
2. Flash Si917 `.rps` only (do **not** flash `bg24_capture/`).
3. Note IPv4 from VCOM (`PTS wifi IP a.b.c.d`).
4. Linux host on same AP/LAN.

## Linux interface

```bash
cd phase_06_wifi_commands/host
python3 pts_wifi_cmd.py \
  --master 192.168.1.50 \
  --receivers 192.168.1.51 \
  --lead-ms 100
```

1. `TIME` → master → `m`, SysTimer Hz  
2. `exec_m = m + hz * lead_ms / 1000`  
3. `PTS6,...` UDP → receiver (repeated)  
4. RX arms schedule GPIO  

### Manual UDP

```bash
echo -n 'TIME' | nc -u -w1 192.168.1.50 5005
echo -n 'PTS6,42,1,<exec_m>,0' | nc -u -w1 192.168.1.51 5006
```

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p6.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p6.slcp` | `.rps` |

**Parked:** `bg24_capture/` (UART→BG24 schedule path unused).

## Exit criteria

- [ ] Both Si917s join AP; VCOM shows IP + UDP port
- [ ] `TIME` / `PTS6` → RX schedule GPIO edge
- [ ] LA accuracy with Wi-Fi traffic vs Phase 5

## Trade-off

UDP is lossy; script repeats. Wi-Fi/BLE coexistence on the same Si917 is intentional load for Phase 6/7.
