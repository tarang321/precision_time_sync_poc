# Manual procedure checklist (T3–T6, T8–T10)

Flash Phase 6 Si917 first. LA: **master LED1 vs RX LED1**. Host: `ON`/`OFF` to master only.

## Preflight

- [ ] Master VCOM: `PTS wifi IP …` + PA running
- [ ] RX VCOM: `q=locked`, `fit_ok` high vs `pairs` (not rej storm)
- [ ] One baseline ON/OFF edge on LA (both LED1)

## T3 — Missed BLE packets

1. Attenuate path or briefly power-cycle master RF (or block LOS).
2. Confirm RX `miss` / `sync LOST` / `holdover`.
3. Restore; wait `q=locked`.
4. Host: `python pts_stress.py --master <IP> --scenario baseline`
5. LA skew; note recovery time.

## T4 — RX sync loss / recovery

1. Power-cycle **RX only**.
2. Wait re-sync + `q=locked`.
3. Baseline ON/OFF; LA skew.

## T5 — Master restart

1. Reset master; note new `session` on RX VCOM.
2. Confirm `pts_schedule_gpio_reset` path (dedupe cleared); wait lock.
3. Baseline ON/OFF; LA skew.

## T6 — Late join

1. Start master alone ≥30 s (PA + Wi-Fi up).
2. Power on RX; wait lock.
3. Baseline ON/OFF; LA skew.

## T8 — RTOS / IRQ load

1. Run T7 (`wifi-load`) while watching VCOM (high log rate).
2. Optional: repeated Wi-Fi join churn on master (SSID toggle / hotspot bounce).
3. Compare skew vs T1.

## T9 — Timer rollover

1. Leave locked ≥ **30 s** (SysTimer u32 wrap ≈ **23.8 s** @ 180 MHz).
2. Issue ON/OFF across a wrap.
3. Confirm ARM/FIRE; no stuck rejects from wrap math.

## T10 — Sync interval

1. Change `PTS_PERIODIC_INTERVAL_*` in P6 `pts_sync_protocol.h` (keep min=max).
2. Match `PTS_SEQ_TIMER_MS` ≥ PA interval.
3. Rebuild/flash **master** (and RX if shared headers differ).
4. Re-lock; run T1; note interval + skew in CSV `notes`.
