#!/usr/bin/env python3
"""
Phase 6 Linux host: stamp and deliver schedule commands over Wi-Fi UDP.

Usage (same LAN as the Si917 boards — 1 master + 1 receiver):
  python3 pts_wifi_cmd.py --master 192.168.1.50 \\
      --receivers 192.168.1.51 --lead-ms 100

Protocol:
  -> master:5005  TIME
  <- TIME,<m_ticks>,<sys_timer_hz>
  -> master:5005  SCHED,<cmd_id>,1,<lead_ms>   (optional stamp helper)
  <- OK,<cmd_id>,<exec_m>
  -> RX:5006 PTS6,<cmd_id>,1,<exec_m>,0
"""

from __future__ import annotations

import argparse
import socket
import sys
import time


MASTER_PORT = 5005
RX_PORT = 5006
TIMEOUT_S = 2.0


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


def main() -> int:
    p = argparse.ArgumentParser(description="PTS Phase 6 Wi-Fi schedule command")
    p.add_argument("--master", required=True, help="Master Si917 IPv4")
    p.add_argument(
        "--receivers",
        required=True,
        help="Comma-separated receiver Si917 IPv4 list",
    )
    p.add_argument("--lead-ms", type=int, default=100, help="Lead time before exec (ms)")
    p.add_argument("--cmd-id", type=int, default=0, help="0 = use unix time as id")
    p.add_argument("--cmd-type", type=int, default=1, help="1 = GPIO toggle")
    p.add_argument(
        "--via-master-sched",
        action="store_true",
        help="Ask master to stamp with SCHED (still fan-out PTS6 from this host)",
    )
    p.add_argument("--repeat", type=int, default=3, help="Repeat PTS6 to each RX")
    args = p.parse_args()

    receivers = [x.strip() for x in args.receivers.split(",") if x.strip()]
    if not receivers:
        print("no receivers", file=sys.stderr)
        return 2

    cmd_id = args.cmd_id if args.cmd_id else int(time.time()) & 0x7FFFFFFF

    if args.via_master_sched:
        reply = udp_exchange(
            args.master,
            MASTER_PORT,
            f"SCHED,{cmd_id},{args.cmd_type},{args.lead_ms}\n",
            expect_prefix="OK,",
        )
        print(f"master {reply}")
        # OK,<cmd_id>,<exec_m>
        parts = reply.split(",")
        exec_m = int(parts[2])
    else:
        reply = udp_exchange(args.master, MASTER_PORT, "TIME\n", expect_prefix="TIME,")
        print(f"master {reply}")
        _, m_s, hz_s = reply.split(",", 2)
        m = int(m_s)
        hz = int(hz_s)
        exec_m = m + (hz * args.lead_ms) // 1000
        print(f"computed exec_m={exec_m} (lead_ms={args.lead_ms})")

    pts6 = f"PTS6,{cmd_id},{args.cmd_type},{exec_m},0\n"
    for _ in range(max(1, args.repeat)):
        for rx in receivers:
            udp_send(rx, RX_PORT, pts6)
            print(f"-> {rx}:{RX_PORT} {pts6.strip()}")
        time.sleep(0.02)

    print("done")
    return 0


if __name__ == "__main__":
    sys.exit(main())
