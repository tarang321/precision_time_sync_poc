# Receiver 2 Si917 — Phase 1 (code ready)

**Role:** Second receiver on the same train (LA channel 2).

**Studio project:** `pts_receiver_2_si917_soc.slcp`  
Same code as receiver 1; `PTS_RX_BOARD_ID=2`; local rand addr ends in `:57`.

Open `pts_receiver_2_si917_soc.slcp` → build/flash → probe GPIO6 alongside RX1 for callback skew.
