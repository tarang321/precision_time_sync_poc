# Phase 3 — Trigger plus follow-up

**Topology:** 1× Si917 SoC master + 1× Si917 SoC receiver. **No BG24.**

**Goal:** Pair master timestamp `M[N]` with receiver-local `R[N]`. Follow-up may arrive later; it must not redefine `R[N]`.

## How the pair is formed

Packet `N` is the trigger. RX samples `R[N]` in the AE report callback (Phase 2) and toggles LED0 (`ULP_GPIO_2`).

The same packet (or next) carries follow-up for the previous trigger: `fu_seq` and `M`.

`M` is `osKernelGetSysTimerCount()` on the master at the app event that calls `rsi_ble_set_ae_data()` — firmware schedule boundary, not a controller TX timestamp.

```text
Trigger N:     master samples M[N], air packet carries seq N
               RX samples R[N] + GPIO edge
Follow-up:     later packet carries fu_seq=N and M[N]
               RX looks up R[N] and logs M[N] <-> R[N]
```

## Projects

| Role | Project | Flash |
|------|---------|-------|
| Master | `master_si917/pts_master_si917_p3.slcp` | `.rps` |
| Receiver | `receiver_si917/pts_receiver_si917_p3.slcp` | `.rps` |

**Parked:** `bg24_capture/` (no soft-UART / host TX). Pairing is on the RX VCOM.

LA / visual: master LED0 vs RX LED0 (`ULP_GPIO_2`). Optional LA on same pin if probed.

## Flash order

1. Master `pts_master_si917_p3.slcp` → `.rps`
2. Receiver `pts_receiver_si917_p3.slcp` → `.rps`

## Exit criteria

- [ ] Master log: `PTS master seq=… M=…`
- [ ] RX pulses GPIO6 per report and stores first `R[N]` only
- [ ] RX log: `M[N]<->R[N] seq=… M=… R=…`
- [ ] `R` is trigger-callback sample, not follow-up arrival time

## References

| Used for | Reference |
|----------|-----------|
| Section 5.2 | [Roadmap](../BLE_Time_Synchronization_POC_Roadmap(2).md) Phase 3 |
| `R[N]` | Phase 2 |
| No controller TX timestamp | Same as Phase 2; [GPIO_TIMESTAMP_E301](https://www.silabs.com/documents/public/errata/siwg917-soc-ic-errata.pdf) |
