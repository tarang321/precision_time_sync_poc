# Phase 1 — Project structure and build

## Layout

```text
phase_01_ble_transport/
  common/
    pts_sync_protocol.h   # seq payload encode/decode
    pts_debug_gpio.h/.c   # Si917 GPIO6 toggle (sl_gpio)
  master_si917/
    pts_master_si917_soc.slcp
    app.c  ble_config.h  app.h  README.md
  receiver_1_si917/
    pts_receiver_1_si917_soc.slcp
    app.c  ble_config.h  app.h  README.md
  receiver_2_si917/
    pts_receiver_2_si917_soc.slcp
    app.c  ble_config.h  app.h  README.md
```

## Dependencies (all three `.slcp`)

| Component | From |
|-----------|------|
| `ble`, `wifi`, `wiseconnect3_common`, buffers, network mgr | wiseconnect3_sdk 3.5.2 |
| `sl_gpio` | wiseconnect3_sdk (debug pin) |
| `freertos`, `sl_main` | simplicity_sdk 2025.6.1 |

Base examples: [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral), [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central).

## Already applied in code (vs stock)

**Master**

- `PERIODIC_ADV_EN=1`, interval `0x10`–`0x14` (~20–25 ms)
- Non-connectable AE props `0x00`, `ADV_SET2=0`
- PTS seq payload + 20 ms CMSIS-RTOS timer refreshing periodic ADV data
- Random addr `11:44:33:AA:BB:CC`

**Receivers**

- Sync to master addr (random) + SID 0
- Report callback copies payload; event path logs seq/miss/RSSI + GPIO toggle
- `BLE_AE_OPTIONS=0` (address-based sync, reporting on)

## Studio steps

1. Install WiSeConnect 3 extension matching `.slcp` (3.5.2) / SDK 2025.6.1 — adjust versions in `.slcp` if your Studio install differs.
2. Open each `*_soc.slcp` from the board folder (keep `../common` relative path intact).
3. Select board part → Generate → Build → Flash.
4. Run master first, then both receivers; LA on GPIO6 both RX boards.

## Config knobs

| Symbol | Where | Meaning |
|--------|-------|---------|
| `PTS_MASTER_RAND_ADDR` | `common/pts_sync_protocol.h` | Must match master `RSI_BLE_SET_RAND_ADDR` and RX `REM_ADDR` |
| `PTS_PERIODIC_INTERVAL_*` | same | Periodic interval (1.25 ms units) |
| `PTS_DEBUG_GPIO_*` | `pts_debug_gpio.c` | LA probe pin (default GPIO6) |
| `PTS_RX_BOARD_ID` | RX `.slcp` | Log tag 1 vs 2 |

## References

| Used for | Reference |
|----------|-----------|
| Phase 1 behavior (Periodic Adv train, callback GPIO, LA skew) | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 1 |
| Master app | [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral) |
| Receiver apps | [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central) |
| Example index | [WiSeConnect examples](https://docs.silabs.com/wiseconnect/latest/wiseconnect-examples/) |
| AE APIs (`rsi_ble_set_ae_*`, `rsi_ble_ae_set_periodic_sync`, report callback) | [BLE API guide](https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-ble/bt-low-energy1) |
| Debug GPIO (`sl_gpio_driver_toggle_pin`) | [sl_si91x_gpio_example](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/si91x_soc/peripheral/sl_si91x_gpio_example) |

SDK pins in the three `.slcp` files: WiSeConnect 4.1.0, Simplicity SDK 2026.6.0. The dependency table above still lists the older 3.5.2 / 2025.6.1 pair from the first import.
