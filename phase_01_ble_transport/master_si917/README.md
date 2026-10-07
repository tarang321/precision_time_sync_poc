# Master Si917 — Phase 1 (SoC)

**Role:** Periodic Advertising time-master (seq `N` @ 20 ms). Pair with **one** SoC receiver (`receiver_1_si917`).

**Studio project:** `pts_master_si917_soc.slcp`  
**Base:** WiSeConnect [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral)

## Sources

| File | Purpose |
|------|---------|
| `app.c` | AE + periodic enable, seq refresh timer, payload update, GPIO6 on seq tick |
| `ble_config.h` | Random addr `11:44:33:AA:BB:CC` (must match RX `REM_ADDR`) |
| `../common/pts_sync_protocol.h` | Seq payload encode |
| `../common/pts_debug_gpio.c` | LA GPIO toggle on seq tick |

## Dependencies (`.slcp`)

WiSeConnect **4.1.1** + Simplicity SDK **2026.6.1**: `ble`, `wifi`, `freertos`, `sl_gpio`, network/wifi stack components.

## Open in Studio

1. File → Open → `pts_master_si917_soc.slcp`.
2. Select Si917 board / part (MCU SoC debug).
3. Build, flash (NWP 4.1.1 first if Connectivity FW unknown).
4. UART: `set ae periodic adv enable success` and `PTS master seq=…`.
