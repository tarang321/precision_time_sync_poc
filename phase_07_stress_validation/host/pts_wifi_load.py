#!/usr/bin/env python3
"""
Standalone UDP flood for Phase 7 coexistence (T7/T8).

Prefer: pts_stress.py --scenario wifi-load
Use this when you want flood without scheduling.
"""

from __future__ import annotations

import argparse
import socket
import time


def main() -> int:
    p = argparse.ArgumentParser(description="PTS Phase 7 Wi-Fi UDP load")
    p.add_argument(
        "--targets",
        required=True,
        help="Comma list host:port (e.g. 192.168.1.50:5005,192.168.1.51:5006)",
    )
    p.add_argument("--hz", type=float, default=200.0)
    p.add_argument("--duration-s", type=float, default=60.0)
    p.add_argument("--size", type=int, default=72, help="Payload bytes")
    args = p.parse_args()

    targets = []
    for item in args.targets.split(","):
        item = item.strip()
        if not item:
            continue
        host, port_s = item.rsplit(":", 1)
        targets.append((host, int(port_s)))
    if not targets:
        print("no targets")
        return 2

    payload = b"PTSLOAD" + bytes(max(0, args.size - 7))
    period = 1.0 / max(args.hz, 1.0)
    socks = [socket.socket(socket.AF_INET, socket.SOCK_DGRAM) for _ in targets]
    t_end = time.monotonic() + args.duration_s
    n = 0
    try:
        while time.monotonic() < t_end:
            for sock, (host, port) in zip(socks, targets):
                sock.sendto(payload, (host, port))
                n += 1
            time.sleep(period)
    finally:
        for sock in socks:
            sock.close()
    print(f"sent {n} datagrams")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
