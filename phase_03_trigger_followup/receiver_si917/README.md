# Phase 3 Si917 receiver (no BG24)

**Role:** Sample `R[N]` at AE report; match follow-up `M[fu]` → log `M[N]<->R[N]` on VCOM.

## Build / flash

1. Flash Phase 3 master first (`../master_si917`).
2. Open `pts_receiver_si917_p3.slcp` → Generate → Build → Flash `.rps`.
3. Expect: `M[N]<->R[N] seq=… M=… R=… pairs=…`

Do **not** flash `bg24_capture/` or use soft-UART host TX.
