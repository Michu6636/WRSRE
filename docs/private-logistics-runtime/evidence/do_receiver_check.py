#!/usr/bin/env python3
"""Verify receiver pairing and observed assignment targets in a Tesmioloader log."""

import collections
import re
import sys
from pathlib import Path


def capture(pattern, text, label):
    match = re.search(pattern, text)
    if not match:
        raise ValueError(f"line missing {label}: {text}")
    return match.group(1)


def is_committed_leg(line, mode, sign):
    return (
        capture(r"mode=(\d+)", line, "mode") == str(mode)
        and capture(r"sign=([+-])", line, "sign") == sign
        and capture(r"commit=(\d+)", line, "commit") == "1"
    )


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: python do_receiver_check.py <tesmioloader-log>")

    log_path = Path(sys.argv[1])
    lines = log_path.read_text(encoding="utf-8", errors="replace").splitlines()
    open_asg = {}
    receivers = collections.defaultdict(collections.Counter)
    outside = collections.Counter()
    trips = []
    pairs = adjacent = tonnage_matched = owner_ok = 0

    for i, line in enumerate(lines):
        if "[FLEET ASSIGNMENT]" in line:
            root = capture(r"root=([0-9A-F]+)", line, "root")
            key = (root, capture(r" id=(\d+)", line, "assignment id"))
            if "event=observed_close" in line:
                trip = next(
                    (item for item in reversed(lines[max(0, i - 3):i])
                     if "[DO TRIP]" in item),
                    None,
                )
                if trip:
                    trips.append((
                        key,
                        capture(r'buyer="([^"]*)"', trip, "trip buyer"),
                        capture(r"reason=(\w+)", trip, "trip reason"),
                        float(capture(r"charge=([\d.]+)", trip, "trip charge")),
                    ))
                open_asg.pop(root, None)
            else:
                open_asg[root] = key
        elif "[B2B SITE] rva=0x68F792" in line:
            if not is_committed_leg(line, 0, "+"):
                continue
            tons = float(capture(r"tons=([-+0-9.eE]+)", line, "building tons"))
            if tons <= 0.0:
                continue
            pairs += 1
            building = capture(r'"([^"]*)"', line, "building")
            owner = capture(r"p9=([0-9A-F]+)", line, "building-side p9")
            next_site = next(
                (item for item in lines[i + 1:i + 3] if "[B2B SITE]" in item),
                "",
            )
            if "rva=0x68F7F9" not in next_site:
                continue
            if not is_committed_leg(next_site, 1, "-"):
                continue
            vehicle_tons = float(
                capture(r"tons=([-+0-9.eE]+)", next_site, "vehicle-side tons")
            )
            if vehicle_tons >= 0.0:
                continue
            adjacent += 1
            vehicle_magnitude = abs(vehicle_tons)
            tolerance = (
                vehicle_magnitude * 0.005
                if vehicle_magnitude > 0.01
                else 0.0005
            )
            if abs(tons - vehicle_magnitude) > tolerance:
                continue
            tonnage_matched += 1
            vehicle = capture(r"p2=([0-9A-F]+)", next_site, "vehicle-side p2")
            vehicle_home = capture(r"vhome=([0-9A-F]+)", next_site, "vehicle home")
            if owner != vehicle_home:
                continue
            owner_ok += 1
            if vehicle in open_asg:
                receivers[open_asg[vehicle]][building] += 1
            else:
                outside[building] += 1

    same = sum(
        1 for key, buyer, _, _ in trips
        if list(receivers.get(key, {})) == [buyer]
    )
    print(
        f"0x68F792 committed_deliveries={pairs} "
        f"adjacent_committed_0x68F7F9_pairs={adjacent} "
        f"tonnage_matched_pairs={tonnage_matched} "
        f"p9==vehicle_home={owner_ok}"
    )
    print("deliveries with no open assignment for that truck:", dict(outside))
    print(f"DO trips={len(trips)} exactly-one-receiver-equal-to-trip-target={same}")
    for key, buyer, reason, charge in trips:
        print(
            f"  truck ..{key[0][-5:]} id {key[1]:>2} "
            f"target={buyer:28s} receivers={dict(receivers.get(key, {}))} "
            f"{reason} charge={charge:.2f}"
        )


if __name__ == "__main__":
    main()
