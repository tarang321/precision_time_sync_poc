# Receiver 1 Si917 — Phase 1 (code ready)

**Role:** Sync to master Periodic Adv train; log seq/miss/RSSI; toggle debug GPIO.

**Studio project:** `pts_receiver_1_si917_soc.slcp`  
**Base:** WiSeConnect [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central)

## Sources

| File | Purpose |
|------|---------|
| `app.c` | Periodic sync create, report parse, miss detect, GPIO |
| `ble_config.h` | `REM_ADDR` = master `11:44:33:AA:BB:CC`; `BLE_AE_OPTIONS=0` |
| `../common/*` | Shared protocol + GPIO |

`PTS_RX_BOARD_ID=1` via `.slcp` define.

## Open in Studio

Open `pts_receiver_1_si917_soc.slcp` → build/flash → confirm `Periodic sync estbl` then `RX1 seq=…`.
