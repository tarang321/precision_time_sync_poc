# Master Si917 — Phase 1 (code ready)

**Role:** Periodic Advertising time-master (seq `N` @ ~20–25 ms).

**Studio project:** `pts_master_si917_soc.slcp`  
**Base:** WiSeConnect [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral)

## Sources

| File | Purpose |
|------|---------|
| `app.c` | AE + periodic enable, seq refresh timer, payload update |
| `ble_config.h` | Random addr `11:44:33:AA:BB:CC` (must match RX `REM_ADDR`) |
| `../common/pts_sync_protocol.h` | Seq payload encode |
| `../common/pts_debug_gpio.c` | Optional LA GPIO toggle on seq tick |

## Dependencies (`.slcp`)

WiSeConnect 3.5.2 + Simplicity SDK 2025.6.1: `ble`, `wifi`, `freertos`, `sl_gpio`, network/wifi stack components (same as stock AE peripheral).

## Open in Studio

1. File → Open → `pts_master_si917_soc.slcp` (WiSeConnect extension required).
2. Select your Si917 board / part.
3. Build, flash, UART: look for `set ae periodic adv enable success` and `PTS master seq=…`.
