# Phase 2 Si917 receiver

Fork of Phase 1 AE central with:

1. `pts_event_gpio_pulse()` at AE periodic-adv report callback entry
2. Delayed `pts_host_tx_send()` of `PTS2,<board>,<seq>,<rssi>` after decode

Open `pts_receiver_si917_p2.slcp`, Generate, Build, Flash. Use Phase 1 master.
