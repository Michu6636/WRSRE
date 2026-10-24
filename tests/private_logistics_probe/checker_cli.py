"""CLI entrypoint for the private-logistics offline log checker.

Usage:
    python3 -m tests.private_logistics_probe.checker_cli <path-to-tesmioloader.log>
    python3 -m tests.private_logistics_probe.checker_cli --synthetic

With --synthetic, runs all fixtures through the checker and prints the summary
for each. This demonstrates the checker on the synthetic test suite without
needing a test framework.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from tests.private_logistics_probe import fixtures, parser, checker


def check_file(path: Path, source: str = "runtime") -> checker.CheckReport:
    """Parse and run checks against a real or captured log file."""
    log_text = path.read_text(encoding="utf-8")
    parsed = parser.parse_log(log_text)
    return checker.run_checks(parsed, source=source)


def check_synthetic() -> list[checker.CheckReport]:
    """Run all 16 fixtures through the checker."""
    fixture_funcs = [
        ("F1 normal_two_trip", fixtures.fixture_normal_two_trip),
        ("F2 refuel_mid_trip", fixtures.fixture_refuel_mid_trip),
        ("F3 duplicate_ship_ids", fixtures.fixture_duplicate_ship_ids),
        ("F4 day_gap", fixtures.fixture_day_gap),
        ("F5 burn_mismatch", fixtures.fixture_burn_mismatch),
        ("F6 negative_burn", fixtures.fixture_negative_burn),
        ("F7 single_reading", fixtures.fixture_single_reading),
        ("F8 no_tank_lines", fixtures.fixture_no_tank_lines),
        ("F9 dry_run", fixtures.fixture_dry_run),
        ("F10 partial_unload", fixtures.fixture_partial_unload),
        ("F11 nan_fuel", fixtures.fixture_nan_fuel),
        ("F12 treasury_mismatch", fixtures.fixture_treasury_mismatch),
        ("F13 mirror_drift", fixtures.fixture_mirror_drift),
        ("F14 only_refuel_vehicle", fixtures.fixture_only_refuel_vehicle),
        ("F15 multi_vehicle", fixtures.fixture_multi_vehicle),
        ("F16 baseline_only", fixtures.fixture_baseline_only),
    ]

    reports = []
    for name, func in fixture_funcs:
        log = func()
        parsed = parser.parse_log(log)
        report = checker.run_checks(parsed, source="synthetic")
        print(f"{name}: overall={report.overall_status}")
        for r in report.results:
            print(f"  {r.check_id} [{r.status}] {r.name}")
        print()
        reports.append(report)

    return reports


def main():
    ap = argparse.ArgumentParser(
        prog="checker_cli",
        description="Offline log checker for the private-logistics plugin."
    )
    ap.add_argument("logfile", nargs="?", help="Path to tesmioloader.log")
    ap.add_argument("--synthetic", action="store_true",
                     help="Run all synthetic fixtures instead of a real file")
    ap.add_argument("--source", default="runtime",
                     choices=["runtime", "synthetic"],
                     help="Mark the log source for evidence provenance")
    args = ap.parse_args()

    if args.synthetic:
        reports = check_synthetic()
        passed = sum(1 for r in reports if r.overall_status != "FAIL")
        failed = sum(1 for r in reports if r.overall_status == "FAIL")
        print(f"=== Summary: {passed} non-failing, {failed} failing fixtures ===")
        sys.exit(0 if failed == 0 else 1)

    if not args.logfile:
        ap.error("logfile is required (or use --synthetic)")

    path = Path(args.logfile)
    if not path.exists():
        print(f"ERROR: file not found: {path}", file=sys.stderr)
        sys.exit(2)

    report = check_file(path, source=args.source)
    print(report.summary)
    for r in report.results:
        print(f"  {r.check_id} [{r.status}] {r.name}")
        for reason in r.reasons:
            print(f"    {reason}")

    sys.exit(0 if report.overall_status != "FAIL" else 1)


if __name__ == "__main__":
    main()
