#!/usr/bin/env python3
import socket
import time

N, LEAD_MS, GAP_S = 20, 500, 2.0


def probe():
    found = []
    for i in range(1, 31):
        a = f"172.20.10.{i}"
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.settimeout(0.35)
        try:
            s.sendto(b"TIME", (a, 5005))
            d, _ = s.recvfrom(256)
            if d.decode(errors="replace").startswith("TIME"):
                found.append(a)
        except Exception:
            pass
        finally:
            s.close()
    return found


def udp(ip, msg, timeout=4.0):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.settimeout(timeout)
    try:
        s.sendto(msg.encode(), (ip, 5005))
        d, _ = s.recvfrom(256)
        return d.decode(errors="replace").strip()
    finally:
        s.close()


def main():
    found = probe()
    print(f"FOUND={found}", flush=True)
    if not found:
        raise SystemExit("no master — WiFi not joined")
    ip = "172.20.10.6" if "172.20.10.6" in found else found[0]
    print(f"USING {ip}", flush=True)
    print("LA ARM — 3s then GO", flush=True)
    time.sleep(3.0)
    print(f"GO {N}", flush=True)
    ok = 0
    t0 = time.time()
    for i in range(N):
        verb = "ON" if (i % 2) == 0 else "OFF"
        try:
            r = udp(ip, f"{verb},{LEAD_MS}")
            ok += 1 if r.startswith("OK") else 0
            print(f"{i+1:02d} {verb} {r}", flush=True)
        except Exception as e:
            print(f"{i+1:02d} {verb} FAIL {e}", flush=True)
        if i + 1 < N:
            time.sleep(GAP_S)
    print(f"DONE udp_ok={ok}/{N} elapsed={time.time()-t0:.1f}s", flush=True)


if __name__ == "__main__":
    main()
