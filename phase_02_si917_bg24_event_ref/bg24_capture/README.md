# Phase 2 BG24 capture

Hardware path: GPIO rising edge → async PRS ch0 → TIMER0 CC0 capture → ring buffer.

Identity path: USART0 RX parses `PTS2,...` lines; FIFO-associates with oldest unmatched `R`.

Open `pts_bg24_capture.slcp` on BRD4187C (or matching MG24 + WPK). Generate required before first build.

Default pins: capture **PC00**, host RX **PC01**. Change `PTS_BG24_CAPTURE_*` / `PTS_BG24_HOST_RX_*` in the `.c` files if your EXP wiring differs.
