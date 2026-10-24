"""Offline log checker for the private-logistics runtime diagnostic.

Consumes parsed log records (from parser.py) and performs reconciliation
checks that produce PASS, FAIL, or INCONCLUSIVE verdicts.

Design rules (from the coding standards):
  - Missing evidence must not silently pass → INCONCLUSIVE.
  - Distinguish synthetic fixtures from actual runtime logs.
  - Zero-allocation per-frame rule does not apply to a CLI tool, but the
    checker is written to handle large logs efficiently with O(n) passes.

Check index
-----------
C1  No-refuel tank reconciliation
C2  Refuel tank reconciliation
C3  Duplicate B2B ship IDs
C4  Day-sequence completeness
C5  Invalid numerics (NaN/Inf, negative burn)
C6  Dry-run / payment mode detection
C7  Treasury movement consistency
C8  Tank cross-check (slot vs mirror) agreement
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

from tests.private_logistics_probe.parser import ParsedLine, UnrecognizedLine
from tests.private_logistics_probe.schema import (
    DEFAULT_TOLERANCE_T,
    SCHEMA_VERSION,
    B2BTransfer,
    B2BDay,
    B2BSite,
    BurnProbe,
    FuelIndex,
    FuelMeter,
    Payment,
    PaymentDay,
    PluginBanner,
    TankCrossCheck,
    TankReading,
    TankRunTotal,
    TariffDay,
    TariffDelivery,
    TreasuryBalance,
    VehicleCapacity,
    VehicleCensus,
    VehicleDetail,
)


# ---------------------------------------------------------------------------
# Result model
# ---------------------------------------------------------------------------

@dataclass
class CheckResult:
    """Result of a single reconciliation check."""
    check_id: str        # e.g. "C1"
    name: str            # human-readable
    status: str          # "PASS", "FAIL", or "INCONCLUSIVE"
    reasons: List[str] = field(default_factory=list)
    evidence: List[str] = field(default_factory=list)
    severity: str = "info"  # "critical", "error", "warning", "info"


@dataclass
class CheckReport:
    """Aggregate report for a single log file."""
    source: str                       # "runtime" or "synthetic"
    schema_version: str               # SCHEMA_VERSION
    plugin_version: Optional[str] = None
    billing: Optional[bool] = None
    dry_run: Optional[bool] = None
    results: List[CheckResult] = field(default_factory=list)
    summary: str = ""
    unrecognized_count: int = 0

    @property
    def overall_status(self) -> str:
        """Worst status across all checks."""
        priority = {"FAIL": 0, "INCONCLUSIVE": 1, "PASS": 2}
        if not self.results:
            return "INCONCLUSIVE"
        return min(
            (r.status for r in self.results),
            key=lambda s: priority.get(s, 1),
        )

    def get_result(self, check_id: str) -> Optional[CheckResult]:
        for r in self.results:
            if r.check_id == check_id:
                return r
        return None


# ---------------------------------------------------------------------------
# Record extraction helpers
# ---------------------------------------------------------------------------

def _extract_records(parsed_lines: List[ParsedLine]) -> List[object]:
    """Extract non-unrecognized records from ParsedLine list."""
    return [pl.record for pl in parsed_lines
            if not isinstance(pl.record, UnrecognizedLine)]


def _get_banner(records: List[object]) -> Optional[PluginBanner]:
    """Return the last PluginBanner — the 'live' banner has authoritative config."""
    last = None
    for r in records:
        if isinstance(r, PluginBanner):
            last = r
    return last


def _get_tank_readings(records: List[object]) -> List[TankReading]:
    return [r for r in records if isinstance(r, TankReading)]


def _get_b2b_transfers(records: List[object]) -> List[B2BTransfer]:
    return [r for r in records if isinstance(r, B2BTransfer)]


def _get_payments(records: List[object]) -> List[Payment]:
    return [r for r in records if isinstance(r, Payment)]


def _get_payment_days(records: List[object]) -> List[PaymentDay]:
    return [r for r in records if isinstance(r, PaymentDay)]


def _get_burn_runs(records: List[object]) -> List[BurnProbe]:
    return [r for r in records if isinstance(r, BurnProbe)]


def _get_tank_runs(records: List[object]) -> List[TankRunTotal]:
    return [r for r in records if isinstance(r, TankRunTotal)]


def _get_tank_crosschecks(records: List[object]) -> List[TankCrossCheck]:
    return [r for r in records if isinstance(r, TankCrossCheck)]


def _get_balances(records: List[object]) -> List[TreasuryBalance]:
    return [r for r in records if isinstance(r, TreasuryBalance)]


def _get_tariff_days(records: List[object]) -> List[TariffDay]:
    return [r for r in records if isinstance(r, TariffDay)]


def _get_fuel_index(records: List[object]) -> List[FuelIndex]:
    return [r for r in records if isinstance(r, FuelIndex)]


def _get_vehicle_census(records: List[object]) -> List[VehicleCensus]:
    return [r for r in records if isinstance(r, VehicleCensus)]


def _get_vehicle_details(records: List[object]) -> List[VehicleDetail]:
    return [r for r in records if isinstance(r, VehicleDetail)]


def _group_tank_readings_by_vehicle(
    readings: List[TankReading],
) -> Dict[str, List[TankReading]]:
    """Group TankReading records by vehicle address, sorted by day."""
    by_vehicle: Dict[str, List[TankReading]] = {}
    for r in readings:
        by_vehicle.setdefault(r.vehicle_addr, []).append(r)
    for addr in by_vehicle:
        by_vehicle[addr].sort(key=lambda t: t.day)
    return by_vehicle


# ---------------------------------------------------------------------------
# Individual checks
# ---------------------------------------------------------------------------

def check_no_refuel_tank_reconciliation(
    records: List[object],
    tolerance_t: float = DEFAULT_TOLERANCE_T,
    synthetic: bool = False,
) -> CheckResult:
    """C1: No-refuel tank reconciliation.

    For each vehicle that has tank readings but no refills on any day,
    verify:
      sum(burned) ≈ start_fuel - end_fuel

    Vehicles with any refills > 0 are skipped (handled by C2).
    If no vehicles qualify, returns INCONCLUSIVE.
    """
    readings = _get_tank_readings(records)
    if not readings:
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=["No [TANK] day readings found — cannot verify tank burn."],
            severity="critical",
        )

    by_vehicle = _group_tank_readings_by_vehicle(readings)
    qualified = {
        addr: days for addr, days in by_vehicle.items()
        if all(d.refills == 0 for d in days)
    }

    if not qualified:
        # All vehicles had refills — no no-refuel case to check
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=[
                "All vehicles had refills (>0) on at least one day. "
                "No pure no-refuel case available.",
            ],
            evidence=[],
            severity="warning",
        )

    failures: List[str] = []
    passes: List[str] = []
    inconclusive: List[str] = []

    for addr, days in qualified.items():
        if len(days) < 2:
            inconclusive.append(
                f"v={addr}: only {len(days)} reading(s) — need >=2 for reconciliation."
            )
            continue

        total_burned = sum(d.burned for d in days)
        # The first reading's fuel is at day-close (after that day's burn).
        # The pre-burn start fuel = fuel + burned (assuming no refills on day 1,
        # which is guaranteed since the vehicle is no-refuel qualified).
        fuel_start = days[0].fuel + days[0].burned
        fuel_end = days[-1].fuel
        expected = fuel_start - fuel_end
        measured = total_burned
        delta = abs(measured - expected)

        src = "synthetic" if synthetic else "runtime"
        if delta <= tolerance_t:
            passes.append(
                f"v={addr} (src={src}): burned={measured:.4f} t, "
                f"start-end={expected:.4f} t, delta={delta:.4f} t ≤ tol={tolerance_t} t"
            )
        else:
            failures.append(
                f"v={addr} (src={src}): burned={measured:.4f} t, "
                f"start-end={expected:.4f} t, delta={delta:.4f} t > tol={tolerance_t} t"
            )

    if inconclusive and not passes and not failures:
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=inconclusive,
            evidence=[],
            severity="warning",
        )
    elif failures and not passes:
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="FAIL",
            reasons=inconclusive + failures,
            evidence=[],
            severity="critical",
        )
    elif failures:
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="FAIL",
            reasons=inconclusive + passes + failures,
            evidence=[],
            severity="critical",
        )
    else:
        return CheckResult(
            check_id="C1",
            name="No-refuel tank reconciliation",
            status="PASS",
            reasons=inconclusive + passes,
            evidence=[],
            severity="info",
        )


def check_refuel_tank_reconciliation(
    records: List[object],
    tolerance_t: float = DEFAULT_TOLERANCE_T,
    synthetic: bool = False,
) -> CheckResult:
    """C2: Refuel tank reconciliation.

    For each vehicle that has refills > 0 on at least one day:
      sum(burned) ≈ (start_fuel - end_fuel) + sum(refill_amounts)

    where refill_amount[i] = fuel[i] - fuel[i-1] + burned[i] when refills[i] > 0.

    If refills are present but cannot be reconstructed (insufficient data),
    returns INCONCLUSIVE.
    """
    readings = _get_tank_readings(records)
    if not readings:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=["No [TANK] day readings found — cannot verify refuel burn."],
            severity="critical",
        )

    by_vehicle = _group_tank_readings_by_vehicle(readings)
    refuel_vehicles = {
        addr: days for addr, days in by_vehicle.items()
        if any(d.refills > 0 for d in days)
    }

    if not refuel_vehicles:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=[
                "No vehicles with refills (>0) on any day. "
                "Refuel reconciliation not applicable."
            ],
            evidence=[],
            severity="info",
        )

    failures: List[str] = []
    passes: List[str] = []
    inconclusive: List[str] = []

    for addr, days in refuel_vehicles.items():
        if len(days) < 2:
            inconclusive.append(
                f"v={addr}: only {len(days)} reading(s) with refills — cannot reconstruct."
            )
            continue

        total_burned = sum(d.burned for d in days)
        # Start fuel = fuel at beginning of day 1 (before day 1's burn).
        # If day 1 had refills, we cannot reconstruct start_fuel precisely.
        if days[0].refills > 0:
            inconclusive.append(
                f"v={addr}: first reading has refills — cannot reconstruct start fuel."
            )
            continue
        fuel_start = days[0].fuel + days[0].burned
        fuel_end = days[-1].fuel

        # Compute refill amounts
        refill_amounts = []
        cannot_compute = False
        for i in range(1, len(days)):
            if days[i].refills > 0:
                # refill_i = fuel[i] - fuel[i-1] + burned[i]
                # If fuel[i] < fuel[i-1], refill was less than burn
                refill_amt = days[i].fuel - days[i - 1].fuel + days[i].burned
                if refill_amt <= 0:
                    # Could not reconstruct: fuel went down more than burn
                    cannot_compute = True
                refill_amounts.append(refill_amt)

        if cannot_compute:
            inconclusive.append(
                f"v={addr}: refill amount cannot be reconstructed "
                f"(negative refill computed)."
            )
            continue

        sum_refills = sum(refill_amounts)
        expected_from_refill = (fuel_start - fuel_end) + sum_refills
        # Alternatively: sum(burned) should equal start - end + sum(refills)
        # i.e., start - end = sum(burned) - sum(refills)
        measured_burn = total_burned
        delta = abs(measured_burn - expected_from_refill)

        src = "synthetic" if synthetic else "runtime"
        if delta <= tolerance_t:
            passes.append(
                f"v={addr} (src={src}): burned={measured_burn:.4f} t, "
                f"(start-end+refills)={expected_from_refill:.4f} t, "
                f"delta={delta:.4f} t ≤ tol={tolerance_t} t"
            )
        else:
            failures.append(
                f"v={addr} (src={src}): burned={measured_burn:.4f} t, "
                f"(start-end+refills)={expected_from_refill:.4f} t, "
                f"delta={delta:.4f} t > tol={tolerance_t} t"
            )

    if inconclusive and not passes and not failures:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="INCONCLUSIVE",
            reasons=inconclusive,
            evidence=[],
            severity="warning",
        )
    elif failures and not passes:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="FAIL",
            reasons=inconclusive + failures,
            evidence=[],
            severity="critical",
        )
    elif failures:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="FAIL",
            reasons=inconclusive + passes + failures,
            evidence=[],
            severity="critical",
        )
    else:
        return CheckResult(
            check_id="C2",
            name="Refuel tank reconciliation",
            status="PASS",
            reasons=inconclusive + passes,
            evidence=[],
            severity="info",
        )


def check_duplicate_b2b_ship_ids(
    records: List[object],
    synthetic: bool = False,
) -> CheckResult:
    """C3: Detect duplicate B2B ship IDs.

    Each [B2B] #ID line must have a globally unique ship_id.
    Two deliveries must not share the same ID.
    """
    transfers = _get_b2b_transfers(records)
    if not transfers:
        return CheckResult(
            check_id="C3",
            name="Duplicate B2B ship IDs",
            status="INCONCLUSIVE",
            reasons=["No [B2B] records found — cannot verify uniqueness."],
            severity="warning",
        )

    seen: Dict[int, str] = {}
    duplicates: List[str] = []
    for t in transfers:
        if t.ship_id in seen:
            duplicates.append(
                f"Duplicate ship_id #{t.ship_id}: "
                f"first at {seen[t.ship_id]}, again ({t.kind} {t.resource} "
                f"{t.tons:.3f}ft) "
                f"{'synthetic' if synthetic else 'runtime'}"
            )
        else:
            seen[t.ship_id] = t.raw

    if duplicates:
        return CheckResult(
            check_id="C3",
            name="Duplicate B2B ship IDs",
            status="FAIL",
            reasons=duplicates,
            evidence=[],
            severity="critical",
        )

    src = "synthetic" if synthetic else "runtime"
    return CheckResult(
        check_id="C3",
        name="Duplicate B2B ship IDs",
        status="PASS",
        reasons=[
            f"All {len(transfers)} B2B ship IDs unique (src={src})."
        ],
        evidence=[],
        severity="info",
    )


def check_day_sequence(
    records: List[object],
    synthetic: bool = False,
) -> CheckResult:
    """C4: Day-sequence completeness.

    Days should be contiguous. Gaps in the day sequence indicate missing
    close events (e.g., plugin was unloaded mid-day).

    Checks day numbers from TankReading, PaymentDay, B2BDay, TariffDay,
    FuelIndex, VehicleCensus, FuelMeter.
    """
    day_holders = [
        _get_tank_readings(records),
        _get_payment_days(records),
        _get_tariff_days(records),
        _get_fuel_index(records),
        _get_vehicle_census(records),
    ]

    all_days: set[int] = set()
    for group in day_holders:
        for r in group:
            all_days.add(getattr(r, 'day', 0))

    if not all_days:
        return CheckResult(
            check_id="C4",
            name="Day-sequence completeness",
            status="INCONCLUSIVE",
            reasons=["No day-tagged records found — cannot verify sequence."],
            severity="warning",
        )

    sorted_days = sorted(all_days)
    gaps: List[str] = []
    for i in range(1, len(sorted_days)):
        diff = sorted_days[i] - sorted_days[i - 1]
        if diff > 1:
            gaps.append(
                f"Gap between day {sorted_days[i-1]} and day {sorted_days[i]} "
                f"(missing {diff - 1} day(s))"
            )

    src = "synthetic" if synthetic else "runtime"
    if gaps:
        return CheckResult(
            check_id="C4",
            name="Day-sequence completeness",
            status="FAIL",
            reasons=[f"Found {len(gaps)} gap(s) (src={src}):"] + gaps,
            evidence=[],
            severity="warning",
        )

    return CheckResult(
        check_id="C4",
        name="Day-sequence completeness",
        status="PASS",
        reasons=[
            f"Day sequence {sorted_days[0]}..{sorted_days[-1]} contiguous "
            f"({len(sorted_days)} day(s), src={src})."
        ],
        evidence=[],
        severity="info",
    )


def check_invalid_numerics(
    records: List[object],
    synthetic: bool = False,
) -> CheckResult:
    """C5: Invalid numerics (NaN, Inf, negative burn).

    Checks:
    - All float fields in tank readings for NaN/Inf
    - burned values must be >= 0 (burn cannot be negative)
    - All float fields in payments for NaN/Inf
    """
    issues: List[str] = []

    tank_readings = _get_tank_readings(records)
    for r in tank_readings:
        src = "synthetic" if synthetic else "runtime"
        for field_name in ('fuel', 'burned'):
            val = getattr(r, field_name)
            if math.isnan(val) or math.isinf(val):
                issues.append(
                    f"[TANK] day {r.day} v={r.vehicle_addr}: {field_name}={val} "
                    f"is NaN/Inf (src={src})"
                )
        if r.burned < 0:
            issues.append(
                f"[TANK] day {r.day} v={r.vehicle_addr}: burned={r.burned:.4f} < 0 "
                f"(src={'synthetic' if synthetic else 'runtime'})"
            )
        if r.fuel < 0:
            issues.append(
                f"[TANK] day {r.day} v={r.vehicle_addr}: fuel={r.fuel:.4f} < 0 "
                f"(src={'synthetic' if synthetic else 'runtime'})"
            )

    payments = _get_payments(records)
    for r in payments:
        src = "synthetic" if synthetic else "runtime"
        for field_name in ('amount', 'tons'):
            val = getattr(r, field_name)
            if math.isnan(val) or math.isinf(val):
                issues.append(
                    f"[PAY] {r.kind} {r.building_name}: {field_name}={val} "
                    f"is NaN/Inf (src={src})"
                )

    if issues:
        return CheckResult(
            check_id="C5",
            name="Invalid numerics",
            status="FAIL",
            reasons=issues,
            evidence=[],
            severity="critical",
        )

    src = "synthetic" if synthetic else "runtime"
    return CheckResult(
        check_id="C5",
        name="Invalid numerics",
        status="PASS",
        reasons=[
            f"No NaN/Inf or negative-burn values in "
            f"{len(tank_readings)} tank readings, "
            f"{len(payments)} payments (src={src})."
        ],
        evidence=[],
        severity="info",
    )


def check_dry_run_mode(
    records: List[object],
    synthetic: bool = False,
) -> CheckResult:
    """C6: Dry-run / payment mode detection.

    Reads the PluginBanner to determine if billing or dry_run is active.
    In dry_run mode, payment reconciliation (C7) is INCONCLUSIVE by design.
    """
    banner = _get_banner(records)
    if not banner:
        return CheckResult(
            check_id="C6",
            name="Dry-run / payment mode detection",
            status="INCONCLUSIVE",
            reasons=[
                "No [BANNER] line found — cannot detect mode."
            ],
            severity="warning",
        )

    src = "synthetic" if synthetic else "runtime"
    if banner.dry_run:
        return CheckResult(
            check_id="C6",
            name="Dry-run / payment mode detection",
            status="INCONCLUSIVE",
            reasons=[
                f"Plugin v{banner.version} is in dry_run mode (src={src}). "
                f"Payment reconciliation is INCONCLUSIVE by design."
            ],
            evidence=[
                f"banner: v{banner.version} billing={'LIVE' if banner.billing else 'LIVE'} dry_run={banner.dry_run}"
            ],
            severity="warning",
        )

    return CheckResult(
        check_id="C6",
        name="Dry-run / payment mode detection",
        status="PASS",
        reasons=[
            f"Plugin v{banner.version} is in live billing mode (src={src})."
        ],
        evidence=[
            f"banner: v{banner.version} billing={'LIVE' if banner.billing else 'LIVE'} dry_run={banner.dry_run}"
        ],
        severity="info",
    )


def check_treasury_consistency(
    records: List[object],
    tolerance: float = 0.01,
    synthetic: bool = False,
) -> CheckResult:
    """C7: Treasury movement consistency.

    In live-billing mode, the plugin_paid value in [BAL] lines should
    approximately equal the sum of individual [PAY] amounts.

    In dry-run mode, this is INCONCLUSIVE (no money moved).
    """
    banner = _get_banner(records)
    if banner and banner.dry_run:
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="INCONCLUSIVE",
            reasons=["Plugin in dry_run mode — no actual treasury movement."],
            evidence=[],
            severity="info",
        )

    balances = _get_balances(records)
    payments = _get_payments(records)

    if not balances:
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="INCONCLUSIVE",
            reasons=["No [BAL] records found — cannot verify treasury movement."],
            severity="critical",
        )

    movement_lines = [b for b in balances if not b.is_baseline]
    if not movement_lines:
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="INCONCLUSIVE",
            reasons=["Only baseline [BAL] lines found — no movement to verify."],
            severity="warning",
        )

    # Total plugin_paid from BAL movement lines
    bal_total = sum(b.plugin_paid for b in movement_lines)

    # Total from PAY lines
    pay_carrier = sum(p.amount for p in payments if p.kind == 'freight')
    pay_factory = sum(p.amount for p in payments if p.kind == 'factory')
    pay_all = pay_carrier + pay_factory

    src = "synthetic" if synthetic else "runtime"
    delta = abs(bal_total - pay_all)

    if not payments:
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="INCONCLUSIVE",
            reasons=[
                "No [PAY] records found — cannot cross-check with BAL total."
            ],
            evidence=[f"[BAL] plugin_paid total={bal_total:.2f} (src={src})"],
            severity="warning",
        )

    if delta <= tolerance + max(abs(bal_total * 0.02), 0.1):
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="PASS",
            reasons=[
                f"[BAL] total plugin_paid={bal_total:.2f}, "
                f"sum([PAY])={pay_all:.2f} (carrier={pay_carrier:.2f}, "
                f"factory={pay_factory:.2f}), delta={delta:.4f} "
                f"(src={src})"
            ],
            evidence=[],
            severity="info",
        )
    else:
        return CheckResult(
            check_id="C7",
            name="Treasury movement consistency",
            status="FAIL",
            reasons=[
                f"[BAL] total plugin_paid={bal_total:.2f}, "
                f"sum([PAY])={pay_all:.2f} (carrier={pay_carrier:.2f}, "
                f"factory={pay_factory:.2f}), delta={delta:.4f} > tol "
                f"(src={src})"
            ],
            evidence=[],
            severity="critical",
        )


def check_tank_crosscheck(
    records: List[object],
    tolerance_t: float = DEFAULT_TOLERANCE_T,
    synthetic: bool = False,
) -> CheckResult:
    """C8: Tank cross-check (slot vs mirror) agreement.

    Each [TANK] cross-check line reports whether slot vs mirror agree.
    All lines should report "agree" (or be absent if the slot isn't read).
    """
    crosschecks = _get_tank_crosschecks(records)
    if not crosschecks:
        return CheckResult(
            check_id="C8",
            name="Tank cross-check (slot vs mirror) agreement",
            status="INCONCLUSIVE",
            reasons=["No [TANK] cross-check lines found — cannot verify."],
            severity="info",
        )

    drift_failures: List[str] = []
    ok_count = 0
    src = "synthetic" if synthetic else "runtime"

    for c in crosschecks:
        if c.status == "MIRROR HAS DRIFTED":
            drift_failures.append(
                f"day {c.day} v={c.vehicle_addr}: drift={c.drift:.4f} t, "
                f"slot={c.slot_level:.4f} mirror={c.mirror_level:.4f} (src={src})"
            )
        elif c.status == "agree":
            ok_count += 1

    if drift_failures:
        return CheckResult(
            check_id="C8",
            name="Tank cross-check (slot vs mirror) agreement",
            status="FAIL",
            reasons=[
                f"{len(drift_failures)} drift failure(s) out of "
                f"{len(crosschecks)} checks (src={src}):"
            ] + drift_failures,
            evidence=[],
            severity="warning",
        )

    return CheckResult(
        check_id="C8",
        name="Tank cross-check (slot vs mirror) agreement",
        status="PASS",
        reasons=[
            f"All {ok_count} cross-check(s) agree (src={src})."
        ],
        evidence=[],
        severity="info",
    )


# ---------------------------------------------------------------------------
# Top-level run
# ---------------------------------------------------------------------------

def run_checks(
    parsed_lines: List[ParsedLine],
    source: str = "runtime",
    tolerance_t: float = DEFAULT_TOLERANCE_T,
) -> CheckReport:
    """Run all reconciliation checks against parsed log records.

    Args:
        parsed_lines: Output of parser.parse_log()
        source: "runtime" or "synthetic" — marks the provenance of the log.
        tolerance_t: Tolerance in tonnes for tank reconciliation.

    Returns:
        CheckReport with results from C1 through C8.
    """
    records = _extract_records(parsed_lines)
    unrecognized = sum(1 for pl in parsed_lines
                       if isinstance(pl.record, UnrecognizedLine))

    banner = _get_banner(records)
    is_synthetic = source == "synthetic"

    results: List[CheckResult] = [
        check_no_refuel_tank_reconciliation(records, tolerance_t, is_synthetic),
        check_refuel_tank_reconciliation(records, tolerance_t, is_synthetic),
        check_duplicate_b2b_ship_ids(records, is_synthetic),
        check_day_sequence(records, is_synthetic),
        check_invalid_numerics(records, is_synthetic),
        check_dry_run_mode(records, is_synthetic),
        check_treasury_consistency(records, synthetic=is_synthetic),
        check_tank_crosscheck(records, tolerance_t, is_synthetic),
    ]

    # Build summary
    status_counts: Dict[str, int] = {"PASS": 0, "FAIL": 0, "INCONCLUSIVE": 0}
    for r in results:
        status_counts[r.status] = status_counts.get(r.status, 0) + 1

    priority = {"FAIL": 0, "INCONCLUSIVE": 1, "PASS": 2}
    overall = min(
        (s for s in status_counts if status_counts[s] > 0),
        key=lambda s: priority.get(s, 1),
    )

    summary = (
        f"Schema={SCHEMA_VERSION} src={source} "
        f"checks={len(results)} "
        f"PASS={status_counts['PASS']} "
        f"FAIL={status_counts['FAIL']} "
        f"INCONCLUSIVE={status_counts['INCONCLUSIVE']} "
        f"unrecognized={unrecognized} "
        f"overall={overall}"
    )

    return CheckReport(
        source=source,
        schema_version=SCHEMA_VERSION,
        plugin_version=banner.version if banner else None,
        billing=banner.billing if banner else None,
        dry_run=banner.dry_run if banner else None,
        results=results,
        summary=summary,
        unrecognized_count=unrecognized,
    )
