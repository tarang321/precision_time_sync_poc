# Phase 3 — Trigger plus follow-up

**Goal:** For event `N`, pair master timestamp `M[N]` with the BG24 capture `R[N]` from Phase 2. The follow-up may arrive later. It must not define `R[N]`.

## How the pair is formed

Packet `N` on Periodic Advertising is the trigger. The receiver Si917 pulses GPIO6 in the AE report callback (same Phase 2 edge). BG24 TIMER0 stores `R[N]`.

That same packet carries the follow-up for the **previous** trigger: sequence `N-1` and `M[N-1]`.

`M` is `osKernelGetSysTimerCount()` sampled on the master in the app event that calls `rsi_ble_set_ae_data()`. That is the firmware schedule boundary, not a controller TX timestamp. The Si917 does not expose one. A fixed offset can be calibrated later. Jitter in this boundary cannot.

```text
Trigger N:     master samples M[N], air packet carries seq N
               receiver edge -> BG24 R[N]
Follow-up:     packet N+1 carries fu_seq=N and M[N]
               BG24 looks up stored R[N] and logs M[N] <-> R[N]
```

Host line (GPIO7 → BG24 PC01), after the edge:

```text
PTS3,<board>,<trigger_seq>,<rssi>,<fu_seq>,<M>
```

`fu_seq` is `4294967295` on the first packet (no previous `M`).

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p3.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p3.slcp` | `.rps` |
| BG24 | `bg24_capture/pts_bg24_followup.slcp` | `.hex` / `.s37` |

Wiring is unchanged from Phase 2: GPIO6 → PC00, GPIO7 → PC01, common GND.

## Exit criteria

- [ ] Master log shows `seq` and `M`
- [ ] Receiver pulses GPIO6 per report and forwards `PTS3`
- [ ] BG24 log shows `M[N]<->R[N] seq=... M=... R=...`
- [ ] `R` is the earlier capture, not the follow-up arrival

## References

| Used for | Reference |
|----------|-----------|
| Section 5.2 trigger + follow-up | [BLE_Time_Synchronization_POC_Roadmap(2).md](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 3 |
| Event edge and `R[N]` | Phase 2 `phase_02_si917_bg24_event_ref/` |
| Periodic Adv transport | Phase 1 master / `ble_ae_peripheral` |
| Master timebase | CMSIS-RTOS2 `osKernelGetSysTimerCount()` in the SiSDK FreeRTOS port |
| No controller TX timestamp | Same Periodic Adv limitation as Phase 2; [SiWG917 `GPIO_TIMESTAMP_E301`](https://www.silabs.com/documents/public/errata/siwg917-soc-ic-errata.pdf) |
