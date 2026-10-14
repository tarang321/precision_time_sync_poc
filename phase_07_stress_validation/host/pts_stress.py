#!/usr/bin/env python3
"""
Phase 7 stress runner — drives Phase 6 master Wi-Fi UDP (BLE schedule to RX).

P6 contract:
  Host -> master :5005  ON|OFF[,lead_ms]  -> OK,<cmd_id>,<exec_m>
  Master arms LED1 + airs PTS6 on BLE; RX is BLE-only (no UDP :5006).

Scenarios: baseline | drift | wifi-load | lead-sweep
Schedule accuracy is measured on the logic analyzer; this script logs commands.
"""

from __future__ import annotations

import argparse
import csv
import socket
import sys
import threading
import time
from pathlib import Path

MASTER_PORT = 5005
TIMEOUT_S = 3.0

# Match P6 defaults (pts_schedule_protocol.h)
DEFAULT_LEAD_MS = 500
CMD_ON = 2
CMD_OFF = 3

# Floor 200 ms — matches PTS_WIFI_LEAD_MIN_MS / P6 air+coex budget.
LEAD_SWEEP_MS = (200, 300, 500, 750, 1000)


def udp_exchange(addr: str, port: int, msg: str, expect_prefix: str | None = None) -> str:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(TIMEOUT_S)
    try:
        sock.sendto(msg.encode("ascii"), (addr, port))
        data, _ = sock.recvfrom(256)
        text = data.decode("ascii", errors="replace").strip()
        if expect_prefix and not text.startswith(expect_prefix):
            raise RuntimeError(f"unexpected reply from {addr}: {text!r}")
        return text
    finally:
        sock.close()


def udp_send(addr: str, port: int, msg: str) -> None:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.sendto(msg.encode("ascii"), (addr, port))
    finally:
        sock.close()


def cmd_onoff(master: str, on: bool, lead_ms: int) -> tuple[int, int]:
    """Send ON/OFF. Returns (cmd_id, exec_m)."""
    verb = "ON" if on else "OFF"
    reply = udp_exchange(master, MASTER_PORT, f"{verb},{lead_ms}\n")
    if reply.startswith("ERR,"):
        raise RuntimeError(reply)
    if not reply.startswith("OK,"):
        raise RuntimeError(f"unexpected reply: {reply!r}")
    parts = reply.split(",")
    if len(parts) < 3:
        raise RuntimeError(f"bad OK line: {reply!r}")
    return int(parts[1]), int(parts[2])


def query_time(master: str) -> tuple[int, int]:
    reply = udp_exchange(master, MASTER_PORT, "TIME\n", expect_prefix="TIME,")
    _, m_s, hz_s = reply.split(",", 2)
    return int(m_s), int(hz_s)


def flood_loop(targets: list[tuple[str, int]], stop: threading.Event, rate_hz: float) -> None:
    payload = b"PTSLOAD" + b"\x00" * 64
    period = 1.0 / max(rate_hz, 1.0)
    socks = [socket.socket(socket.AF_INET, socket.SOCK_DGRAM) for _ in targets]
    try:
        while not stop.is_set():
            for sock, (addr, port) in zip(socks, targets):
                try:
                    sock.sendto(payload, (addr, port))
                except OSError:
                    pass
            time.sleep(period)
    finally:
        for sock in socks:
            sock.close()


def open_csv(path: Path) -> tuple[csv.DictWriter, object]:
    path.parent.mkdir(parents=True, exist_ok=True)
    new_file = not path.exists() or path.stat().st_size == 0
    fh = path.open("a", newline="", encoding="utf-8")
    fieldnames = [
        "unix_s",
        "scenario",
        "cmd_id",
        "cmd",
        "lead_ms",
        "m",
        "hz",
        "exec_m",
        "notes",
        "skew_us",
    ]
    w = csv.DictWriter(fh, fieldnames=fieldnames)
    if new_file:
        w.writeheader()
    return w, fh


def one_shot(
    master: str,
    lead_ms: int,
    on: bool,
    writer: csv.DictWriter | None,
    scenario: str,
    notes: str = "",
) -> int:
    m, hz = query_time(master)
    cmd_id, exec_m = cmd_onoff(master, on, lead_ms)
    verb = "ON" if on else "OFF"
    print(f"[{scenario}] {verb} cmd_id={cmd_id} lead_ms={lead_ms} m={m} hz={hz} exec_m={exec_m}")
    if writer is not None:
        writer.writerow(
            {
                "unix_s": f"{time.time():.3f}",
                "scenario": scenario,
                "cmd_id": cmd_id,
                "cmd": verb,
                "lead_ms": lead_ms,
                "m": m,
                "hz": hz,
                "exec_m": exec_m,
                "notes": notes,
                "skew_us": "",  # fill from LA
            }
        )
    return cmd_id


def run_periodic(
    master: str,
    lead_ms: int,
    duration_s: float,
    period_s: float,
    writer: csv.DictWriter | None,
    scenario: str,
    notes: str = "",
) -> None:
    t_end = time.monotonic() + duration_s
    n = 0
    on = True
    while time.monotonic() < t_end:
        t0 = time.monotonic()
        try:
            one_shot(master, lead_ms, on, writer, scenario, notes)
            n += 1
            on = not on
        except (OSError, RuntimeError, ValueError) as exc:
            print(f"error: {exc}", file=sys.stderr)
            if writer is not None:
                writer.writerow(
                    {
                        "unix_s": f"{time.time():.3f}",
                        "scenario": scenario,
                        "cmd_id": "",
                        "cmd": "",
                        "lead_ms": lead_ms,
                        "m": "",
                        "hz": "",
                        "exec_m": "",
                        "notes": f"FAIL {exc}",
                        "skew_us": "",
                    }
                )
        elapsed = time.monotonic() - t0
        time.sleep(max(0.0, period_s - elapsed))
    print(f"[{scenario}] issued {n} commands in {duration_s:.0f}s")


def main() -> int:
    p = argparse.ArgumentParser(description="PTS Phase 7 stress runner (P6 Wi-Fi → BLE)")
    p.add_argument("--master", required=True, help="Master Si917 IPv4")
    p.add_argument(
        "--scenario",
        choices=("baseline", "drift", "wifi-load", "lead-sweep"),
        default="baseline",
    )
    p.add_argument("--lead-ms", type=int, default=DEFAULT_LEAD_MS)
    p.add_argument("--duration-s", type=float, default=300.0)
    p.add_argument("--period-s", type=float, default=2.0)
    p.add_argument("--flood-hz", type=float, default=200.0, help="UDP flood rate (wifi-load)")
    p.add_argument("--results", type=Path, default=None, help="Append CSV path")
    p.add_argument(
        "--receivers",
        default="",
        help="Ignored (P6 RX is BLE-only). Kept for CLI compat.",
    )
    args = p.parse_args()

    writer = None
    fh = None
    if args.results is not None:
        writer, fh = open_csv(args.results)

    flood_stop: threading.Event | None = None
    flood_thread: threading.Thread | None = None

    try:
        # Probe master first
        m, hz = query_time(args.master)
        print(f"master TIME m={m} hz={hz}")

        if args.scenario == "baseline":
            one_shot(args.master, args.lead_ms, True, writer, "baseline", notes="T1 ON")
            time.sleep(max(0.8, args.lead_ms / 1000.0 + 0.3))
            one_shot(args.master, args.lead_ms, False, writer, "baseline", notes="T1 OFF")
        elif args.scenario == "drift":
            run_periodic(
                args.master,
                args.lead_ms,
                args.duration_s,
                args.period_s,
                writer,
                "drift",
            )
        elif args.scenario == "wifi-load":
            flood_stop = threading.Event()
            targets = [(args.master, MASTER_PORT)]
            flood_thread = threading.Thread(
                target=flood_loop,
                args=(targets, flood_stop, args.flood_hz),
                daemon=True,
            )
            flood_thread.start()
            run_periodic(
                args.master,
                args.lead_ms,
                args.duration_s,
                args.period_s,
                writer,
                "wifi-load",
                notes=f"flood_hz={args.flood_hz}",
            )
        elif args.scenario == "lead-sweep":
            on = True
            for lead in LEAD_SWEEP_MS:
                try:
                    one_shot(
                        args.master,
                        lead,
                        on,
                        writer,
                        "lead-sweep",
                        notes=f"lead={lead}",
                    )
                    on = not on
                except (OSError, RuntimeError, ValueError) as exc:
                    print(f"lead={lead} FAIL {exc}", file=sys.stderr)
                time.sleep(max(0.8, (lead / 1000.0) + 0.4))
    finally:
        if flood_stop is not None:
            flood_stop.set()
        if flood_thread is not None:
            flood_thread.join(timeout=2.0)
        if fh is not None:
            fh.close()

    print("done — fill skew_us from LA (master LED1 vs RX LED1) into results CSV")
    return 0


if __name__ == "__main__":
    sys.exit(main())
