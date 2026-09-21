# Manual procedure checklist (T3–T6, T9–T10)

Print or copy. Flash Phase 6 first. LA on all BG24 PC02.

## Preflight

- [ ] Master + N receivers + BG24s on air; VCOM shows IPs
- [ ] `quality=locked` on each BG24
- [ ] One baseline SCHED edge visible on LA

## T3 — Missed BLE packets

1. Attenuate master antenna or briefly disable PA (reset mid-train without Wi-Fi reset if possible).
2. Confirm RX `MISS` / sync-lost logs; BG24 may enter `holdover`.
3. Restore path; wait for `locked`.
4. Issue one SCHED; measure skew.

## T4 — RX sync loss / recovery

1. Power-cycle one RX Si917 + its BG24 only.
2. Wait re-sync + lock.
3. SCHED; confirm that board’s PC02 participates; skew vs survivors.

## T5 — Master restart

1. Reset master; note new `session` in PTS4 lines.
2. Wait all RX re-establish sync + BG24 lock.
3. SCHED; record skew.

## T6 — Late join

1. Start train with N−1 receivers.
2. Power on remaining RX after ≥30 s.
3. Confirm lock; SCHED; skew includes late joiner.

## T9 — Timer rollover

1. Leave locked ≥ 2 min continuous (TIMER0 wrap ~110 s @ 39 MHz).
2. Issue SCHED immediately after a wrap window.
3. Confirm arm + edge; no stuck `reject` from wrap math.

## T10 — Sync interval

1. Change `PTS_PERIODIC_INTERVAL_*` in P6 `pts_sync_protocol.h`.
2. Rebuild/flash master only.
3. Re-lock; run T1; note interval + skew in CSV `notes`.
