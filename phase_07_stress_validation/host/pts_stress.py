#!/usr/bin/env python3
"""
Phase 7 stress runner — drives Phase 6 Wi-Fi schedule UDP (1 master + 1 RX).

Scenarios: baseline | drift | wifi-load | lead-sweep
Schedule accuracy is measured on the logic analyzer; this script only logs commands.
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
RX_PORT = 5006
TIMEOUT_S = 2.0

LEAD_SWEEP_MS = (15, 25, 50, 75, 100, 150, 200, 500)


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


def stamp_exec(master: str, lead_ms: int) -> tuple[int, int, int]:
    """Return (m, hz, exec_m)."""
    reply = udp_exchange(master, MASTER_PORT, "TIME\n", expect_prefix="TIME,")
    _, m_s, hz_s = reply.split(",", 2)
    m = int(m_s)
    hz = int(hz_s)
    exec_m = m + (hz * lead_ms) // 1000
    return m, hz, exec_m


def deliver_pts6(
    receivers: list[str],
    cmd_id: int,
    cmd_type: int,
    exec_m: int,
    repeat: int,
) -> None:
    pts6 = f"PTS6,{cmd_id},{cmd_type},{exec_m},0\n"
    for _ in range(max(1, repeat)):
        for rx in receivers:
            udp_send(rx, RX_PORT, pts6)
        time.sleep(0.02)


def flood_loop(targets: list[tuple[str, int]], stop: threading.Event, rate_hz: float) -> None:
    """Best-effort UDP spam for coexistence (T7/T8)."""
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
        "lead_ms",
        "m",
        "hz",
        "exec_m",
        "receivers",
        "notes",
        "skew_us",
    ]
    w = csv.DictWriter(fh, fieldnames=fieldnames)
    if new_file:
        w.writeheader()
    return w, fh


def one_shot(
    master: str,
    receivers: list[str],
    lead_ms: int,
    cmd_type: int,
    repeat: int,
    writer: csv.DictWriter | None,
    scenario: str,
    notes: str = "",
) -> int:
    m, hz, exec_m = stamp_exec(master, lead_ms)
    cmd_id = int(time.time() * 1000) & 0x7FFFFFFF
    deliver_pts6(receivers, cmd_id, cmd_type, exec_m, repeat)
    print(
        f"[{scenario}] cmd_id={cmd_id} lead_ms={lead_ms} "
        f"m={m} hz={hz} exec_m={exec_m}"
    )
    if writer is not None:
        writer.writerow(
            {
                "unix_s": f"{time.time():.3f}",
                "scenario": scenario,
                "cmd_id": cmd_id,
                "lead_ms": lead_ms,
                "m": m,
                "hz": hz,
                "exec_m": exec_m,
                "receivers": ";".join(receivers),
                "notes": notes,
                "skew_us": "",  # fill from LA
            }
        )
    return cmd_id


def run_periodic(
    master: str,
    receivers: list[str],
    lead_ms: int,
    cmd_type: int,
    repeat: int,
    duration_s: float,
    period_s: float,
    writer: csv.DictWriter | None,
    scenario: str,
    notes: str = "",
) -> None:
    t_end = time.monotonic() + duration_s
    n = 0
    while time.monotonic() < t_end:
        t0 = time.monotonic()
        try:
            one_shot(
                master, receivers, lead_ms, cmd_type, repeat, writer, scenario, notes
            )
            n += 1
        except (OSError, RuntimeError, ValueError) as exc:
            print(f"error: {exc}", file=sys.stderr)
            if writer is not None:
                writer.writerow(
                    {
                        "unix_s": f"{time.time():.3f}",
                        "scenario": scenario,
                        "cmd_id": "",
                        "lead_ms": lead_ms,
                        "m": "",
                        "hz": "",
                        "exec_m": "",
                        "receivers": ";".join(receivers),
                        "notes": f"FAIL {exc}",
                        "skew_us": "",
                    }
                )
        elapsed = time.monotonic() - t0
        time.sleep(max(0.0, period_s - elapsed))
    print(f"[{scenario}] issued {n} commands in {duration_s:.0f}s")


def main() -> int:
    p = argparse.ArgumentParser(description="PTS Phase 7 stress runner")
    p.add_argument("--master", required=True)
    p.add_argument("--receivers", required=True, help="Comma-separated RX IPv4")
    p.add_argument(
        "--scenario",
        choices=("baseline", "drift", "wifi-load", "lead-sweep"),
        default="baseline",
    )
    p.add_argument("--lead-ms", type=int, default=100)
    p.add_argument("--cmd-type", type=int, default=1)
    p.add_argument("--repeat", type=int, default=3)
    p.add_argument("--duration-s", type=float, default=300.0)
    p.add_argument("--period-s", type=float, default=2.0)
    p.add_argument("--flood-hz", type=float, default=200.0, help="UDP flood rate (wifi-load)")
    p.add_argument("--results", type=Path, default=None, help="Append CSV path")
    args = p.parse_args()

    receivers = [x.strip() for x in args.receivers.split(",") if x.strip()]
    if not receivers:
        print("no receivers", file=sys.stderr)
        return 2

    writer = None
    fh = None
    if args.results is not None:
        writer, fh = open_csv(args.results)

    flood_stop: threading.Event | None = None
    flood_thread: threading.Thread | None = None

    try:
        if args.scenario == "baseline":
            one_shot(
                args.master,
                receivers,
                args.lead_ms,
                args.cmd_type,
                args.repeat,
                writer,
                "baseline",
            )
        elif args.scenario == "drift":
            run_periodic(
                args.master,
                receivers,
                args.lead_ms,
                args.cmd_type,
                args.repeat,
                args.duration_s,
                args.period_s,
                writer,
                "drift",
            )
        elif args.scenario == "wifi-load":
            flood_stop = threading.Event()
            targets = [(args.master, MASTER_PORT)] + [(r, RX_PORT) for r in receivers]
            flood_thread = threading.Thread(
                target=flood_loop,
                args=(targets, flood_stop, args.flood_hz),
                daemon=True,
            )
            flood_thread.start()
            run_periodic(
                args.master,
                receivers,
                args.lead_ms,
                args.cmd_type,
                args.repeat,
                args.duration_s,
                args.period_s,
                writer,
                "wifi-load",
                notes=f"flood_hz={args.flood_hz}",
            )
        elif args.scenario == "lead-sweep":
            for lead in LEAD_SWEEP_MS:
                try:
                    one_shot(
                        args.master,
                        receivers,
                        lead,
                        args.cmd_type,
                        args.repeat,
                        writer,
                        "lead-sweep",
                        notes=f"lead={lead}",
                    )
                except (OSError, RuntimeError, ValueError) as exc:
                    print(f"lead={lead} FAIL {exc}", file=sys.stderr)
                # Wait past lead + margin so edges do not pile up
                time.sleep(max(0.5, (lead / 1000.0) + 0.3))
    finally:
        if flood_stop is not None:
            flood_stop.set()
        if flood_thread is not None:
            flood_thread.join(timeout=2.0)
        if fh is not None:
            fh.close()

    print("done — fill skew_us from LA into results CSV")
    return 0


if __name__ == "__main__":
    sys.exit(main())
