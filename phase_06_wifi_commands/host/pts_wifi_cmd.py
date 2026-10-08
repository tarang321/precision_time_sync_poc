#!/usr/bin/env python3
"""
Phase 6: command MASTER only over Wi-Fi. Master schedules + sends PTS6 on BLE.

  python pts_wifi_cmd.py --master 172.20.10.2 on
  python pts_wifi_cmd.py --master 172.20.10.2 off --lead-ms 500
"""

from __future__ import annotations

import argparse
import socket
import sys

MASTER_PORT = 5005
TIMEOUT_S = 5.0
DEFAULT_LEAD_MS = 500


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


def main() -> int:
    p = argparse.ArgumentParser(description="PTS Phase 6 — Wi-Fi cmd to master → BLE schedule")
    p.add_argument("--master", required=True, help="Master Si917 IPv4")
    p.add_argument("--lead-ms", type=int, default=DEFAULT_LEAD_MS,
                   help=f"Lead before exec (ms), default {DEFAULT_LEAD_MS}")
    p.add_argument("action", choices=("on", "off", "time"))
    args = p.parse_args()

    if args.action == "time":
        print(udp_exchange(args.master, MASTER_PORT, "TIME\n", expect_prefix="TIME,"))
        return 0

    msg = f"{args.action.upper()},{args.lead_ms}\n"
    reply = udp_exchange(args.master, MASTER_PORT, msg, expect_prefix="OK,")
    print(reply)
    print("master arms LED1 + airs PTS6 on BLE; RX arms via to_local; both fire @ exec_m")
    return 0


if __name__ == "__main__":
    sys.exit(main())
