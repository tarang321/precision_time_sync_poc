# Phase 6 — Wi-Fi command → BLE schedule

**Topology:** 1× Si917 master + 1× Si917 RX. **No BG24.**

**Motto:** Only **master** joins Wi-Fi. Host sends `ON`/`OFF` to master. Master stamps `exec_m`, arms its LED1, and airs **PTS6 on BLE PA** (same schedule path as Phase 5). RX has **no Wi-Fi** — locks on BLE and arms LED1 via `to_local(exec_m)`.

```text
PC --UDP ON/OFF--> Master :5005
                     |-- arm LED1 @ exec_m (+PA like P5)
                     +-- BLE PA: PTS5 sync + PTS6 schedule AD
RX  <--- BLE only ---'  to_local → arm LED1
Both fire at exec_m
```

| LED | Pin | Role |
|-----|-----|------|
| LED0 | ULP_GPIO_2 | BT / seq edge |
| LED1 | GPIO_10 | Scheduled ON/OFF level |

## Setup

1. SSID/PSK in **master only**: `master_si917/config/sl_net_default_values.h`
   (`YOUR_SSID` / `YOUR_PSK` placeholders — set real values before flash)
2. iPhone hotspot: **Maximize Compatibility** (2.4 GHz)
3. Flash both `.rps`. PC on same hotspot as master.
4. RX: wait `q=locked` (BLE only — no Wi-Fi join). Master: `PTS wifi IP …`
5. Host `OK` means queued; `ERR,queue_full` / `ERR,bad_type` means rejected.

## Host

```bash
cd phase_06_wifi_commands/host
python pts_wifi_cmd.py --master <master_ip> on
python pts_wifi_cmd.py --master <master_ip> off
```

## Exit criteria

- [ ] Master joins AP; RX does **not** attempt Wi-Fi
- [ ] `on`/`off` → both LED1 same level after lead
- [ ] LA LED1 edges under Wi-Fi+BLE coex on master
