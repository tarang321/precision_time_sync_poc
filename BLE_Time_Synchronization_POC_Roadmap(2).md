# BLE Time-Synchronization POC Roadmap

## Purpose

After reviewing the time-synchronization design document and exploring the relevant SiWG917 and WiSeConnect capabilities, I have arrived at the following implementation approach for the initial proof of concept (POC).

The primary implementation will be based on:

- **Section 5.2 — Trigger plus follow-up synchronization**
- **Section 5.3 — Logical network time**
- Hardware-assisted BLE event-timing transfer from Si917 to BG24
- BG24 owns and maintains logical network time**
- Future-timestamp-based command execution

TWS and Auracast mechanisms were explored only as additional reference architectures. They are not part of the initial POC. Their continuous synchronization and one-to-many broadcast concepts can be evaluated after the base synchronization method is working.

## Proposed Architecture

```text
Master Si917
    |
    | BLE trigger N followed by master timestamp M[N]
    v
Receiver Si917 devices
    |
    | Event-related GPIO edge
    | Trigger/follow-up data over NCP
    |
    v
BG24 PRS + timer capture
    |
    | Match M[N] with captured BG24 time R[N]
    | Calculate logical network time and clock rate
    v
BG24 hardware timer compare and synchronized GPIO execution
```

In this architecture, the Si917 does **not** calculate or own the receiver's logical network clock. Its timing responsibilities are limited to BLE communication, obtaining or exposing an accurate BLE-event reference, and forwarding the associated trigger and follow-up information to BG24.

The BG24 captures the event reference with PRS and a hardware timer, calculates its direct local-timer-to-master-time mapping, and uses that same timer domain to schedule the final GPIO action. This avoids an additional Si917-network-time-to-BG24-time synchronization stage.

## Phase 1 — Validate the BLE Synchronization Transport

- Configure one Si917 as the time master.
- Configure two Si917 devices as receivers.
- Use BLE Periodic Advertising as the initial one-to-many transport.
- Transmit a synchronization sequence number at a fixed interval, initially 20–25 ms.
- Confirm that both receivers synchronize with the same advertising train.
- Log received sequence numbers, missed packets, RSSI, and callback timing.
- Toggle a debug GPIO when each receiver receives a synchronization event.
- Measure callback-level skew using a common logic analyzer.

This phase will validate the BLE transport and establish the timing variation introduced by the existing NCP and callback path.

### Relevant WiSeConnect APIs

Master:

```text
rsi_ble_set_ae_params()
rsi_ble_set_ae_data()
rsi_ble_app_set_periodic_ae_params()
rsi_ble_app_set_periodic_ae_enable()
rsi_ble_start_ae_advertising()
```

Receivers:

```text
rsi_ble_adv_ext_events_register_callbacks()
rsi_ble_ae_set_scan_params()
rsi_ble_ae_set_scan_enable()
rsi_ble_ae_set_periodic_sync()
```

## Phase 2 — Establish the Si917-to-BG24 BLE Event Reference

Before implementing the complete clock model, establish a deterministic timestamp path from the Si917 BLE event to the BG24 timer:

1. Receiver Si917 identifies the relevant BLE trigger event `N` as close as possible to the controller or radio-event boundary.
2. Si917 generates an event-related GPIO edge.
3. BG24 routes that input edge through PRS to a timer-capture channel.
4. The timer-capture channel records BG24 local timer value `R[N]` without application callback latency.
5. Si917 forwards the event sequence number and trigger/follow-up data to BG24 through NCP.
6. BG24 associates the captured timer value with sequence number `N`.

```text
Si917 BLE event N
        |
        v
Si917 event GPIO edge
        |
        v
BG24 PRS -> timer capture -> R[N]
```

The NCP message may arrive later because it is used only to identify the captured event. Its SPI, interrupt, and task-scheduling delay must not determine `R[N]`.

One item requiring confirmation is whether Si917 firmware exposes an accurate BLE RX event timestamp, a hardware radio-event signal, or a firmware hook that can generate a deterministic GPIO edge. The published Periodic Advertising report appears to provide payload, synchronization handle, RSSI, and data status, but not a controller timestamp.

If only the normal BG24 application callback is available, it can be used for the Phase 1 baseline.

## Phase 3 — Implement Section 5.2: Trigger Plus Follow-Up

For synchronization event `N`:

1. The master transmits a trigger containing sequence number `N`.
2. The master records timestamp `M[N]` at the defined BLE transmit-event boundary.
3. Every receiver Si917 generates an event reference for the same trigger.
4. Every receiver BG24 captures its own timer value `R[N]` using PRS and timer capture.
5. A later packet carries the master timestamp and referenced sequence number `N`.
6. Each BG24 matches master timestamp `M[N]` with its captured timestamp `R[N]`.

```text
Trigger event N:
Master captures M[N]
Receiver BG24 captures R[N]

Follow-up:
Referenced event = N
Master timestamp = M[N]

Timestamp pair:
M[N] <-> R[N]
```

The follow-up packet does not need to arrive at the same time on every receiver. It describes the earlier trigger event `N`; therefore, each BG24 can correlate the delayed follow-up with the hardware-captured `R[N]` value.

Both sides must timestamp clearly defined equivalent event boundaries. Any fixed difference may be calibrated, but variable delay or jitter cannot be removed using constant-delay calibration.

## Phase 4 — Implement Section 5.3: Logical Network Time on BG24

Using multiple timestamp pairs, every receiver BG24 will calculate the relationship between its own hardware timer and the master timer:

```text
Network time = M0 + rate x (BG24 local time - R0)
```

The offset represents the current time difference, while the rate compensates for clock drift.

The physical counters do not have to contain identical values. Every BG24 only needs a conversion that maps its local counter to the common master time.

The clock model will:

- Use multiple timestamp samples.
- Estimate both offset and relative clock rate.
- Reject invalid or excessively delayed samples.
- Filter measurement noise.
- Continue extrapolating during short synchronization-packet losses.
- Handle timer rollover.
- Avoid directly resetting the running hardware timer.
- Maintain a master-session identifier so receivers can detect a master restart.
- Report synchronization quality and enter holdover mode during short packet losses.

Because BG24 owns the logical network clock, there is no separate phase that transfers calculated network time from Si917 to BG24. The transferred reference is the earlier BLE event itself, not a completed Si917 network-time model.

## Phase 5 — Scheduled-Execution POC

Once every BG24 can calculate logical network time directly from its own captured timer:

- Send a command containing a future network execution timestamp.
- Initially use a lead time of approximately 100 ms.
- Convert the network execution time into the corresponding BG24 timer value.
- Program a hardware timer compare.
- Toggle the demo GPIO directly from the timer event.
- Measure the GPIO-edge skew across all three boards.

The command will include:

```text
Command ID
Command type
Future execution network time
Validity/status information
```

Commands should be repeated if transported over a broadcast mechanism. The command ID will prevent duplicate execution.

The important principle is that a command is not executed when it is received. It is received in advance, buffered, and executed at the specified logical network time.

## Phase 6 — Add Wi-Fi Command Transport

After BLE synchronization and scheduled GPIO execution are stable:

- Connect the Si917 devices to the Wi-Fi access point.
- Deliver commands through Wi-Fi.
- Include the future logical network timestamp in every command.
- Keep BLE responsible only for maintaining network time.
- Keep Wi-Fi responsible only for command delivery.
- Repeat logic-analyzer measurements while Wi-Fi traffic is active.
- Evaluate the effect of Wi-Fi/BLE coexistence on synchronization.

## Phase 7 — Stress Testing and Validation

Test the following conditions:

- Long-duration clock drift
- Missed BLE synchronization packets
- Receiver synchronization loss and recovery
- Master restart
- Receiver joining after synchronization has started
- Wi-Fi traffic active during BLE synchronization
- RTOS and interrupt load
- Timer rollover
- Different synchronization intervals
- Different command lead times

The initial execution-skew objective will be **below 200 us**, measured between the GPIO edges of all participating boards using the same logic analyzer.

## Additional Exploration After the Primary POC

TWS and Auracast synchronization concepts were reviewed as possible future improvements.

TWS systems generally separate packet-reception time from presentation time. Audio is received in advance, buffered, and played at a common future instant. This is comparable to receiving a command in advance and executing it at a shared future network time.

Auracast similarly uses a one-to-many broadcast structure and common scheduled radio events. However, BIG/BIS is not the primary implementation path because the publicly documented Si917 APIs clearly expose Periodic Advertising, while the availability of raw BIG/BIS and controller-event timestamp APIs still needs confirmation.

After Sections 5.2 and 5.3 are successfully implemented, the following concepts can be evaluated:

- A higher-rate continuous time stream
- TWS-style filtered clock tracking
- Command buffering similar to audio presentation buffering
- Event-number-based execution
- BIG/BIS as an alternative continuous broadcast transport, if supported by the Si917 firmware and SDK
- Repeated broadcast commands for packet-loss tolerance

## Implementation Order

```text
Validate Periodic Advertising and callback skew
            |
            v
Si917 BLE-event edge -> BG24 PRS capture
            |
            v
Trigger + follow-up timestamp correlation on BG24
            |
            v
BG24 logical network-time model
            |
            v
Future-timestamp hardware GPIO execution
            |
            v
Wi-Fi command integration
            |
            v
TWS/Auracast-inspired enhancements
```

## Items for Review

Please review this approach and let me know if any changes are required, particularly regarding:

- Availability of controller-level BLE TX/RX timestamps on Si917
- Availability of a radio-event-related GPIO signal or deterministic firmware hook
- Whether the master can obtain the actual transmit-event timestamp needed for the follow-up
- Use of Periodic Advertising for the initial transport
- Proposed 20–25 ms synchronization interval
- Any existing internal APIs or firmware hooks that should be used for event timestamping
