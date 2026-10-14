#!/usr/bin/env python3
"""Capture master/RX UART, send ON/OFF to master, print schedule analysis."""

from __future__ import annotations

import re
import socket
import sys
import time
from collections import defaultdict

import serial

MASTER_COM = "COM11"
RX_COM = "COM10"
BAUD = 115200
UDP_PORT = 5005


def open_port(name: str, port: str) -> serial.Serial:
    ser = serial.Serial(port, BAUD, timeout=0.05)
    ser.reset_input_buffer()
    print(f"OPEN {name} {port}", flush=True)
    return ser


def drain(sers: dict, duration_s: float) -> dict:
    t0 = time.time()
    lines = defaultdict(list)
    while time.time() - t0 < duration_s:
        for name, ser in sers.items():
            n = ser.in_waiting
            if not n:
                continue
            chunk = ser.read(n).decode("utf-8", errors="replace")
            for raw in chunk.splitlines():
                line = raw.strip()
                if not line:
                    continue
                line_safe = line.encode("ascii", errors="replace").decode("ascii")
                lines[name].append(line_safe)
                print(f"[{name}] {line_safe}", flush=True)
        time.sleep(0.02)
    return lines


def find_ip(lines: list[str]) -> str | None:
    for line in lines:
        m = re.search(r"PTS wifi IP\s+(\d+\.\d+\.\d+\.\d+)", line)
        if m:
            return m.group(1)
    return None


def udp_cmd(ip: str, msg: str, timeout: float = 5.0) -> str:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(timeout)
    try:
        sock.sendto(msg.encode("ascii"), (ip, UDP_PORT))
        data, _ = sock.recvfrom(256)
        return data.decode("ascii", errors="replace").strip()
    finally:
        sock.close()


def parse_cmd(line: str) -> int | None:
    m = re.search(r"cmd=(\d+)", line)
    return int(m.group(1)) if m else None


def parse_exec(line: str) -> int | None:
    m = re.search(r"exec_m=(\d+)", line)
    return int(m.group(1)) if m else None


def parse_type(line: str) -> int | None:
    m = re.search(r"type=(\d+)", line)
    return int(m.group(1)) if m else None


def analyze(all_lines: dict) -> str:
    out = []
    m = all_lines.get("master", [])
    r = all_lines.get("rx", [])

    def grab(lines, substr):
        return [x for x in lines if substr in x]

    m_wifi = grab(m, "PTS wifi IP")
    m_cmd = grab(m, "PTS wifi CMD")
    m_arm = grab(m, "sched ARM")
    m_fire = grab(m, "sched FIRE")
    m_seq = [x for x in m if x.startswith("PTS master seq=")]
    m_fail = grab(m, "seq update failed")
    m_recov = grab(m, "seq update recovered")

    r_est = grab(r, "Periodic sync established")
    r_lost = grab(r, "sync LOST")
    r_reacq = grab(r, "reacq")
    r_arm = grab(r, "sched ARM")
    r_fire = grab(r, "sched FIRE")
    r_alive = grab(r, "alive")
    r_seq = [x for x in r if x.startswith("RX1 seq=")]
    r_q = grab(r, "PTS net quality=")

    last_rx = r_seq[-1] if r_seq else None
    last_m = m_seq[-1] if m_seq else None

    m_arms = {parse_cmd(x): x for x in m_arm if parse_cmd(x) is not None}
    r_arms = {parse_cmd(x): x for x in r_arm if parse_cmd(x) is not None}
    m_fires = {parse_cmd(x): x for x in m_fire if parse_cmd(x) is not None}
    r_fires = {parse_cmd(x): x for x in r_fire if parse_cmd(x) is not None}
    cmds = sorted(set(m_arms) | set(r_arms) | set(m_fires) | set(r_fires))

    out.append("=== SUMMARY ===")
    out.append(f"master_lines={len(m)} rx_lines={len(r)}")
    out.append(
        f"wifi_ip_logs={len(m_wifi)} wifi_cmd={len(m_cmd)} "
        f"master_ARM={len(m_arm)} master_FIRE={len(m_fire)}"
    )
    out.append(
        f"rx_ARM={len(r_arm)} rx_FIRE={len(r_fire)} sync_est={len(r_est)} "
        f"sync_lost={len(r_lost)} reacq={len(r_reacq)}"
    )
    out.append(f"master_seq_fail={len(m_fail)} master_recovered={len(m_recov)}")
    if last_m:
        out.append(f"last_master: {last_m}")
    if last_rx:
        out.append(f"last_rx: {last_rx}")
    if r_q:
        out.append(f"last_quality: {r_q[-1]}")
    if r_alive:
        out.append(f"last_alive: {r_alive[-1]}")

    out.append("=== SCHEDULE MATCH ===")
    if not cmds:
        out.append("NO ARM/FIRE seen on either UART")
    for c in cmds:
        ma, ra = m_arms.get(c), r_arms.get(c)
        mf, rf = m_fires.get(c), r_fires.get(c)
        mex = parse_exec(ma) if ma else (parse_exec(mf) if mf else None)
        rex = parse_exec(ra) if ra else (parse_exec(rf) if rf else None)
        typ = parse_type(ma) if ma else (parse_type(ra) if ra else None)
        same = mex is not None and rex is not None and mex == rex
        out.append(
            f"cmd={c} type={typ} exec_match={same} "
            f"M_arm={'Y' if ma else 'n'} R_arm={'Y' if ra else 'n'} "
            f"M_fire={'Y' if mf else 'n'} R_fire={'Y' if rf else 'n'} "
            f"exec_m={mex if mex is not None else '-'} / {rex if rex is not None else '-'}"
        )
        if ma:
            out.append(f"  M: {ma}")
        if ra:
            out.append(f"  R: {ra}")
        if mf:
            out.append(f"  Mfire: {mf}")
        if rf:
            out.append(f"  Rfire: {rf}")

    out.append("=== VERDICT ===")
    ok_sched = any(
        (
            c in m_arms
            and c in r_arms
            and c in m_fires
            and c in r_fires
            and parse_exec(m_arms[c]) == parse_exec(r_arms[c])
        )
        for c in cmds
    )
    locked = (last_rx and "q=locked" in last_rx) or any("q=locked" in x for x in r_q[-3:])
    if m_fail and not m_recov:
        out.append("FAIL: master seq updates failing without recovery")
    elif r_lost and not r_est:
        out.append("FAIL: RX sync lost, no re-establish in capture")
    elif ok_sched and locked:
        out.append("PASS: locked + both sides ARM/FIRE same exec_m")
    elif ok_sched:
        out.append("PARTIAL: schedule matched; lock not confirmed in capture window")
    elif locked:
        out.append("PARTIAL: locked but no complete ON/OFF ARM/FIRE pair in capture")
    else:
        out.append("FAIL: no complete schedule path observed")
    return "\n".join(out)


def main() -> int:
    sers = {
        "master": open_port("master", MASTER_COM),
        "rx": open_port("rx", RX_COM),
    }
    all_lines: dict = defaultdict(list)

    print("\n--- baseline 6s ---", flush=True)
    base = drain(sers, 6.0)
    for k, v in base.items():
        all_lines[k].extend(v)

    ip = find_ip(all_lines["master"])
    if not ip:
        for cand in (
            "172.20.10.2",
            "172.20.10.4",
            "172.20.10.5",
            "172.20.10.6",
            "172.20.10.7",
            "172.20.10.8",
        ):
            try:
                rep = udp_cmd(cand, "TIME\n", timeout=1.0)
                if rep.startswith("TIME,"):
                    ip = cand
                    print(f"FOUND master via TIME probe: {ip} -> {rep}", flush=True)
                    break
            except Exception:
                pass

    if not ip:
        print("NO_MASTER_IP", flush=True)
        print(analyze(all_lines))
        for s in sers.values():
            s.close()
        return 2

    print(f"\n--- master IP {ip} : ON ---", flush=True)
    try:
        print(f"UDP_REPLY {udp_cmd(ip, 'ON,500\n')}", flush=True)
    except Exception as e:
        print(f"UDP_ON_FAIL {e}", flush=True)

    after_on = drain(sers, 3.0)
    for k, v in after_on.items():
        all_lines[k].extend(v)

    print(f"\n--- master IP {ip} : OFF ---", flush=True)
    try:
        print(f"UDP_REPLY {udp_cmd(ip, 'OFF,500\n')}", flush=True)
    except Exception as e:
        print(f"UDP_OFF_FAIL {e}", flush=True)

    after_off = drain(sers, 3.0)
    for k, v in after_off.items():
        all_lines[k].extend(v)

    print("\n--- post 4s ---", flush=True)
    post = drain(sers, 4.0)
    for k, v in post.items():
        all_lines[k].extend(v)

    for s in sers.values():
        s.close()

    print("\n" + analyze(all_lines), flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
