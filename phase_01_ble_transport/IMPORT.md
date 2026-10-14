# Phase 1 — Project structure and build

**Topology (current):** 2× Si917 SoC — 1 master + 1 receiver. **No BG24.**

## Layout

```text
phase_01_ble_transport/
  common/
    pts_sync_protocol.h   # seq payload encode/decode
    pts_debug_gpio.h/.c   # Si917 GPIO6 toggle (sl_gpio)
  master_si917/
    pts_master_si917_soc.slcp
    app.c  ble_config.h  app.h  README.md
  receiver_1_si917/          # active receiver
    pts_receiver_1_si917_soc.slcp
    app.c  ble_config.h  app.h  README.md
```

## Dependencies (active `.slcp` files)

| Component | From |
|-----------|------|
| `ble`, `wifi`, `wiseconnect3_common`, buffers, network mgr | wiseconnect3_sdk **4.1.1** |
| `sl_gpio` | wiseconnect3_sdk (debug pin) |
| `freertos`, `sl_main` | simplicity_sdk **2026.6.1** |

Base examples: [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral), [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central).

## Already applied in code (vs stock)

**Master**

- `PERIODIC_ADV_EN=1`, interval fixed `0x10` (20 ms)
- Non-connectable AE props `0x00`, `ADV_SET2=0`
- PTS seq payload + CMSIS-RTOS timer refreshing periodic ADV data
- Random addr `11:44:33:AA:BB:CC`
- GPIO6 toggle on each seq update (LA TX edge)

**Receiver (`receiver_1_si917`)**

- Sync to master addr (random) + SID 0
- Report callback copies payload; event path logs seq/miss/RSSI + GPIO toggle
- `BLE_AE_OPTIONS=0` (address-based sync, reporting on)

## Studio steps

1. Match WiSeConnect **4.1.1** / Simplicity SDK **2026.6.1** to the `.slcp` pins. Flash NWP from the same 4.1.1 package.
2. Open `master_si917/pts_master_si917_soc.slcp` and `receiver_1_si917/pts_receiver_1_si917_soc.slcp` (keep `../common` relative path intact).
3. Select board part → Generate → Build → Flash.
4. Run **master first**, then **one receiver**. LA: common GND; probe **GPIO6 on master** and **GPIO6 on RX**.

## Config knobs

| Symbol | Where | Meaning |
|--------|-------|---------|
| `PTS_MASTER_RAND_ADDR` | `common/pts_sync_protocol.h` | Must match master `RSI_BLE_SET_RAND_ADDR` and RX `REM_ADDR` |
| `PTS_PERIODIC_INTERVAL_*` | same | Periodic interval (1.25 ms units) |
| `PTS_DEBUG_GPIO_*` | `pts_debug_gpio.c` | LA probe pin (default GPIO6) |
| `PTS_RX_BOARD_ID` | RX `.slcp` | Log tag (1 for active RX) |

## References

| Used for | Reference |
|----------|-----------|
| Phase 1 behavior (Periodic Adv train, callback GPIO) | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 1 |
| Master app | [ble_ae_peripheral](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_peripheral) |
| Receiver app | [ble_ae_central](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/snippets/ble/ble_ae_central) |
| Example index | [WiSeConnect examples](https://docs.silabs.com/wiseconnect/latest/wiseconnect-examples/) |
| AE APIs | [BLE API guide](https://docs.silabs.com/wiseconnect/latest/wiseconnect-api-reference-guide-ble/bt-low-energy1) |
| Debug GPIO | [sl_si91x_gpio_example](https://github.com/SiliconLabs/wiseconnect/tree/master/examples/si91x_soc/peripheral/sl_si91x_gpio_example) |
