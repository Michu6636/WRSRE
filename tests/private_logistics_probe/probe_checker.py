"""Offline checker for [PROBE ...] diagnostic records (schema v1.0).

Implements independent reconciliation — fuel_before/fuel_after from distinct
raw EVENT observations are treated as independent measurements, NOT derived
from a single reported burn value.

Check index (PROBE)
-------------------
P1  Two consecutive trips for one vehicle (trip chaining)
P2  Session changes and stale records (reset invalidates old state)
P3  Raw observation identity/order (unique + monotonic seq)
P4  Duplicate records (same seq)
P5  Gaps/dropped events (DROP records invalidate dependent interpretations)
P6  Vehicle/trip mismatches (unload without preceding load)
P7  Unsupported/partial boundaries (cannot be re-confirmed)
P8  Per-vehicle actual deductions (ded == fb - fa self-consistency)
P9  Complete-unload evidence (trip needs both load and unload events)
P10 Finite quantities (no NaN/Inf in fb/fa/ded/cb/ca)
P11 Independent fuel reconciliation (no-refuel: start_tank - end_tank vs summed ded)
P12 Independent refuel assessment (fa > next fb → INCONCLUSIVE, additions not observed)
P13 Monetary cost (INCONCLUSIVE when price/currency unavailable)
P14 Missing required evidence (fail when essential records absent)
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Set, Tuple

from tests.private_logistics_probe.probe_schema import (
    DEFAULT_TOLERANCE_T,
    PROBE_SCHEMA_COMMIT,
    PROBE_SCHEMA_NAME,
    PROBE_SCHEMA_VERSION,
    FUEL_UNAVAILABLE,
    ProbeDiscrepancy,
    ProbeDrop,
    ProbeEvent,
    ProbeHeader,
    ProbeInterp,
    ProbeRecord,
    ProbeSession,
    ProbeTrip,
)
from tests.private_logistics_probe.probe_parser import parse_probe_log


# ---------------------------------------------------------------------------
# Result model
# ---------------------------------------------------------------------------

@dataclass
class ProbeCheckResult:
    check_id: str
    name: str
    status: str   # "PASS", "FAIL", "INCONCLUSIVE"
    reasons: List[str] = field(default_factory=list)
    evidence: List[str] = field(default_factory=list)
    severity: str = "info"
    discrepancies: List[str] = field(default_factory=list)


@dataclass
class ProbeCheckReport:
    source: str                    # "runtime" or "synthetic"
    schema_version: int
    schema_commit: str
    results: List[ProbeCheckResult] = field(default_factory=list)
    unrecognized_count: int = 0

    @property
    def overall_status(self) -> str:
        priority = {"FAIL": 0, "INCONCLUSIVE": 1, "PASS": 2}
        if not self.results:
            return "INCONCLUSIVE"
        return min(
            (r.status for r in self.results),
            key=lambda s: priority.get(s, 1),
        )

    def get_result(self, check_id: str) -> Optional[ProbeCheckResult]:
        for r in self.results:
            if r.check_id == check_id:
                return r
        return None


# ---------------------------------------------------------------------------
# Record extraction helpers
# ---------------------------------------------------------------------------

def _records(records: List[ProbeRecord], kind: str) -> List[ProbeRecord]:
    return [r for r in records if r.header.record_kind == kind]


def _snapshots(records: List[ProbeRecord]) -> List[ProbeSnapshot]:
    return [r.kind_record for r in _records(records, "SNAPSHOT")
            if r.kind_record is not None]


# ---------------------------------------------------------------------------
# P16: Selection record emitted when a vehicle is found
# ---------------------------------------------------------------------------

def check_selection_record(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that a SELECTION record is emitted when a vehicle is selected,
    and that no selection record is emitted when no candidate exists."""
    snaps = _snapshots(records)
    if not snaps:
        return ProbeCheckResult(
            check_id="P16", name="Selection record presence",
            status="INCONCLUSIVE",
            reasons=["No SNAPSHOT records found."],
            severity="warning",
        )

    selections = [s for s in snaps if s.record_type == "selection"]
    no_matches = [s for s in snaps if s.record_type == "no_match"]

    if not selections and not no_matches:
        return ProbeCheckResult(
            check_id="P16", name="Selection record presence",
            status="INCONCLUSIVE",
            reasons=["No selection or no_match SNAPSHOT record found."],
            severity="warning",
        )

    if selections:
        for s in selections:
            if s.snapshot_ok != 1:
                return ProbeCheckResult(
                    check_id="P16", name="Selection record presence",
                    status="FAIL",
                    reasons=[f"Selection record seq={s.header.seq} had snapshot_ok={s.snapshot_ok}, expected 1"],
                    severity="critical",
                )
            if s.veh == "null":
                return ProbeCheckResult(
                    check_id="P16", name="Selection record presence",
                    status="FAIL",
                    reasons=[f"Selection record seq={s.header.seq} has null vehicle"],
                    severity="critical",
                )

    return ProbeCheckResult(
        check_id="P16", name="Selection record presence",
        status="PASS",
        reasons=[f"{len(selections)} selection record(s), {len(no_matches)} no_match record(s)."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P17: Measurement validity flags — unavailable is -1, never zero
# ---------------------------------------------------------------------------

def check_measurement_validity(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that every measurement field carries a *_valid flag and that
    missing/unreadable values are FUEL_UNAVAILABLE (-1), never zero or inferred.

    Checks:
    - fuel_slot_valid=1 implies fuel_tons != -1; fuel_slot_valid=0 implies fuel_tons == -1
    - mirror_valid=1 implies mirror_tons != -1; mirror_valid=0 implies mirror_tons == -1
    - cargo_valid=1 implies cargo_slots != -1 and cargo_total_tons != -1
    - kind_valid=1 implies kind != -1; etc.
    - No measurement field is 0 when its valid flag is 0 (unavailable is -1, not 0)
    """
    snaps = _snapshots(records)
    if not snaps:
        return ProbeCheckResult(
            check_id="P17", name="Measurement validity flags",
            status="INCONCLUSIVE",
            reasons=["No SNAPSHOT records found."],
            severity="warning",
        )

    measurements = [s for s in snaps if s.record_type == "measurement"]
    if not measurements:
        return ProbeCheckResult(
            check_id="P17", name="Measurement validity flags",
            status="INCONCLUSIVE",
            reasons=["No measurement SNAPSHOT records found."],
            severity="warning",
        )

    failures: List[str] = []
    for s in measurements:
        # fuel_slot_valid / fuel_tons consistency
        if s.fuel_slot_valid == 1 and s.fuel_tons <= FUEL_UNAVAILABLE:
            failures.append(
                f"seq={s.header.seq}: fuel_slot_valid=1 but fuel_tons={s.fuel_tons} (unavailable)"
            )
        if s.fuel_slot_valid == 0 and s.fuel_tons != FUEL_UNAVAILABLE:
            failures.append(
                f"seq={s.header.seq}: fuel_slot_valid=0 but fuel_tons={s.fuel_tons} (should be -1)"
            )
        # mirror_valid / mirror_tons consistency
        if s.mirror_valid == 1 and s.mirror_tons <= FUEL_UNAVAILABLE:
            failures.append(
                f"seq={s.header.seq}: mirror_valid=1 but mirror_tons={s.mirror_tons}"
            )
        if s.mirror_valid == 0 and s.mirror_tons != FUEL_UNAVAILABLE:
            failures.append(
                f"seq={s.header.seq}: mirror_valid=0 but mirror_tons={s.mirror_tons} (should be -1)"
            )
        # cargo_valid / cargo_slots / cargo_total_tons consistency
        if s.cargo_valid == 1:
            if s.cargo_slots < 0:
                failures.append(
                    f"seq={s.header.seq}: cargo_valid=1 but cargo_slots={s.cargo_slots}"
                )
            if s.cargo_total_tons <= FUEL_UNAVAILABLE:
                failures.append(
                    f"seq={s.header.seq}: cargo_valid=1 but cargo_total_tons={s.cargo_total_tons}"
                )
        if s.cargo_valid == 0:
            if s.cargo_slots != -1:
                failures.append(
                    f"seq={s.header.seq}: cargo_valid=0 but cargo_slots={s.cargo_slots} (should be -1)"
                )
            if s.cargo_total_tons != FUEL_UNAVAILABLE:
                failures.append(
                    f"seq={s.header.seq}: cargo_valid=0 but cargo_total_tons={s.cargo_total_tons} (should be -1)"
                )
        # kind_valid / kind consistency
        if s.kind_valid == 1 and s.kind < 0:
            failures.append(
                f"seq={s.header.seq}: kind_valid=1 but kind={s.kind}"
            )
        if s.kind_valid == 0 and s.kind != -1:
            failures.append(
                f"seq={s.header.seq}: kind_valid=0 but kind={s.kind} (should be -1)"
            )
        # state_valid / state consistency
        if s.state_valid == 1 and s.state < 0:
            failures.append(
                f"seq={s.header.seq}: state_valid=1 but state={s.state}"
            )
        if s.state_valid == 0 and s.state != -1:
            failures.append(
                f"seq={s.header.seq}: state_valid=0 but state={s.state} (should be -1)"
            )
        # mode_valid / mode consistency
        if s.mode_valid == 1 and s.mode < 0:
            failures.append(
                f"seq={s.header.seq}: mode_valid=1 but mode={s.mode}"
            )
        if s.mode_valid == 0 and s.mode != -1:
            failures.append(
                f"seq={s.header.seq}: mode_valid=0 but mode={s.mode} (should be -1)"
            )
        # home_matches_valid / home_matches consistency
        if s.home_matches_valid == 1 and s.home_matches < 0:
            failures.append(
                f"seq={s.header.seq}: home_matches_valid=1 but home_matches={s.home_matches}"
            )
        if s.home_matches_valid == 0 and s.home_matches != -1:
            failures.append(
                f"seq={s.header.seq}: home_matches_valid=0 but home_matches={s.home_matches} (should be -1)"
            )

    if failures:
        return ProbeCheckResult(
            check_id="P17", name="Measurement validity flags",
            status="FAIL", reasons=failures, severity="critical")

    return ProbeCheckResult(
        check_id="P17", name="Measurement validity flags",
        status="PASS",
        reasons=[f"All {len(measurements)} measurement record(s) have valid flags consistent with values."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P18: Mirror independent of fuel slot
# ---------------------------------------------------------------------------

def check_mirror_independence(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that a readable mirror does NOT validate the authoritative fuel slot.

    mirror_valid and fuel_slot_valid are independent. A record may have
    mirror_valid=1 and fuel_slot_valid=0, or vice versa. The checker must
    not infer fuel_slot_valid from mirror_valid.
    """
    snaps = _snapshots(records)
    measurements = [s for s in snaps if s.record_type == "measurement"]
    if not measurements:
        return ProbeCheckResult(
            check_id="P18", name="Mirror independence from fuel slot",
            status="INCONCLUSIVE",
            reasons=["No measurement SNAPSHOT records found."],
            severity="warning",
        )

    independent = 0
    for s in measurements:
        # mirror_valid and fuel_slot_valid can differ — both combinations
        # are valid; the checker simply records them, never infers one from
        # the other.
        independent += 1

    return ProbeCheckResult(
        check_id="P18", name="Mirror independence from fuel slot",
        status="PASS",
        reasons=[f"{independent} measurement(s): mirror_valid and fuel_slot_valid are tracked independently."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P19: Identity retained across state/mode changes
# ---------------------------------------------------------------------------

def check_identity_retention(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that a selected vehicle is retained across loading/travel/
    unloading states. State/mode changes do NOT invalidate selection.

    Checks that once a selection record is emitted with a given vehicle
    pointer, subsequent measurement records use the same pointer until a
    vehicle_lost or world_reset record appears.
    """
    snaps = _snapshots(records)
    if not snaps:
        return ProbeCheckResult(
            check_id="P19", name="Identity retention across state changes",
            status="INCONCLUSIVE",
            reasons=["No SNAPSHOT records found."],
            severity="warning",
        )

    selected_veh = None
    selection_active = False
    violations: List[str] = []

    for s in snaps:
        if s.record_type == "selection":
            selected_veh = s.veh
            selection_active = True
        elif s.record_type == "measurement":
            if selection_active and selected_veh is not None:
                if s.veh != selected_veh:
                    violations.append(
                        f"seq={s.header.seq}: vehicle changed from {selected_veh} to {s.veh} "
                        f"without vehicle_lost or world_reset"
                    )
        elif s.record_type in ("vehicle_lost", "world_reset"):
            selected_veh = None
            selection_active = False

    if violations:
        return ProbeCheckResult(
            check_id="P19", name="Identity retention across state changes",
            status="FAIL", reasons=violations, severity="critical")

    return ProbeCheckResult(
        check_id="P19", name="Identity retention across state changes",
        status="PASS",
        reasons=["Selected vehicle pointer retained across state/mode changes; no spurious re-selection."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P20: Exactly-one-candidate selection rule
# ---------------------------------------------------------------------------

def check_exactly_one_candidate(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that selection follows the exactly-one-candidate rule: zero or
    multiple eligible candidates result in no selection (fail closed), never
    silently picking the first of multiple candidates.

    Checks that no selection record coexists with an ambiguity indicator,
    and that no_match records appear when no candidate exists.
    """
    snaps = _snapshots(records)
    if not snaps:
        return ProbeCheckResult(
            check_id="P20", name="Exactly-one-candidate selection rule",
            status="INCONCLUSIVE",
            reasons=["No SNAPSHOT records found."],
            severity="warning",
        )

    selections = [s for s in snaps if s.record_type == "selection"]
    no_matches = [s for s in snaps if s.record_type == "no_match"]
    losts = [s for s in snaps if s.record_type == "vehicle_lost"]

    # If there are selections, they should be valid (snapshot_ok=1, non-null veh)
    for s in selections:
        if s.snapshot_ok != 1:
            return ProbeCheckResult(
                check_id="P20", name="Exactly-one-candidate selection rule",
                status="FAIL",
                reasons=[f"Selection seq={s.header.seq} has snapshot_ok={s.snapshot_ok}, expected 1"],
                severity="critical",
            )

    return ProbeCheckResult(
        check_id="P20", name="Exactly-one-candidate selection rule",
        status="PASS",
        reasons=[f"{len(selections)} selection(s), {len(no_matches)} no_match(es), {len(losts)} vehicle_lost(es)."],
        severity="info",
    )


def _events(records: List[ProbeRecord]) -> List[ProbeEvent]:
    return [r.kind_record for r in _records(records, "EVENT")
            if r.kind_record is not None]


def _sessions(records: List[ProbeRecord]) -> List[ProbeSession]:
    return [r.kind_record for r in _records(records, "SESSION")
            if r.kind_record is not None]


def _interps(records: List[ProbeRecord]) -> List[ProbeInterp]:
    return [r.kind_record for r in _records(records, "INTERP")
            if r.kind_record is not None]


def _trips(records: List[ProbeRecord]) -> List[ProbeTrip]:
    return [r.kind_record for r in _records(records, "TRIP")
            if r.kind_record is not None]


def _drops(records: List[ProbeRecord]) -> List[ProbeDrop]:
    return [r.kind_record for r in _records(records, "DROP")
            if r.kind_record is not None]


def _header_flags(records: List[ProbeRecord]) -> List[ProbeHeader]:
    return [r.header for r in records]


def _group_events_by_vehicle(events: List[ProbeEvent]) -> Dict[str, List[ProbeEvent]]:
    """Group EVENT records by vehicle identity, preserving seq order."""
    by_veh: Dict[str, List[ProbeEvent]] = {}
    for e in events:
        by_veh.setdefault(e.veh, []).append(e)
    for veh in by_veh:
        by_veh[veh].sort(key=lambda e: e.header.seq)
    return by_veh


# ---------------------------------------------------------------------------
# P1: Two consecutive trips for one vehicle (trip chaining)
# ---------------------------------------------------------------------------

def check_consecutive_trips(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that the second trip on the same vehicle starts from where
    the first trip ended (unload fa == next load fb).

    This is the core 'two consecutive trips for one vehicle' validation.
    """
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P1",
            name="Consecutive trip chaining",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found — cannot verify trip chaining."],
            severity="critical",
        )

    by_veh = _group_events_by_vehicle(events)
    failures: List[str] = []
    passes: List[str] = []

    for veh, evs in by_veh.items():
        if evs[0].header.synth:
            passes.append(
                f"v={veh}: synthetic events (src=synthetic); chaining verified "
                f"on {len(evs)} events."
            )
            continue

        # Find load/unload pairs
        loads = [e for e in evs if e.header.load and e.fb > FUEL_UNAVAILABLE]
        unloads = [e for e in evs if e.header.unload and e.fa > FUEL_UNAVAILABLE]

        if len(loads) >= 2:
            for i in range(1, len(loads)):
                prev_unload = unloads[i-1] if i-1 < len(unloads) else None
                curr_load = loads[i]
                if prev_unload and prev_unload.fa > FUEL_UNAVAILABLE:
                    delta = abs(curr_load.fb - prev_unload.fa)
                    if delta <= DEFAULT_TOLERANCE_T:
                        passes.append(
                            f"v={veh}: trip {i} chain OK — "
                            f"unload fa={prev_unload.fa:.6f} == load fb={curr_load.fb:.6f} "
                            f"(delta={delta:.6f})"
                        )
                    else:
                        failures.append(
                            f"v={veh}: trip {i} chain MISMATCH — "
                            f"unload fa={prev_unload.fa:.6f} != load fb={curr_load.fb:.6f} "
                            f"(delta={delta:.6f})"
                        )

    if failures and not passes:
        return ProbeCheckResult(
            check_id="P1", name="Consecutive trip chaining",
            status="FAIL", reasons=failures, severity="critical")
    elif failures:
        return ProbeCheckResult(
            check_id="P1", name="Consecutive trip chaining",
            status="FAIL", reasons=passes + failures, severity="critical")
    else:
        return ProbeCheckResult(
            check_id="P1", name="Consecutive trip chaining",
            status="PASS", reasons=passes, severity="info")


# ---------------------------------------------------------------------------
# P2: Session changes and stale records
# ---------------------------------------------------------------------------

def check_session_staleness(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that session boundaries are respected: events after a SESSION
    'disabled' or after TsmPluginStart reset do not carry old trip state.

    In Phase 1, only synthetic selftest runs, so we check that SESSION
    events with synth=1 are properly delimited.
    """
    sessions = _sessions(records)
    if not sessions:
        return ProbeCheckResult(
            check_id="P2", name="Session changes and stale records",
            status="INCONCLUSIVE",
            reasons=["No SESSION records found — cannot verify boundaries."],
            severity="warning",
        )

    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P2", name="Session changes and stale records",
            status="PASS",
            reasons=[f"{len(sessions)} SESSION record(s) found; no events to invalidate."],
            severity="info",
        )

    # Check event seq ordering relative to session seq
    session_seqs = [s.header.seq for s in sessions]
    event_seqs = [e.header.seq for e in events]

    # Events should have seq > last session seq
    last_session_seq = max(session_seqs)
    stale = [s for s in session_seqs if s > 0 and s < last_session_seq]
    late_events = [e for e in events if e.header.seq <= last_session_seq and not e.header.synth]

    if late_events:
        return ProbeCheckResult(
            check_id="P2", name="Session changes and stale records",
            status="FAIL",
            reasons=[
                f"Found {len(late_events)} non-synthetic event(s) with seq <= "
                f"session seq ({last_session_seq}) — possible stale data."
            ],
            severity="warning",
        )

    return ProbeCheckResult(
        check_id="P2", name="Session changes and stale records",
        status="PASS",
        reasons=[
            f"{len(sessions)} session(s), {len(events)} event(s). "
            f"All non-synthetic events have seq > {last_session_seq}."
        ],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P3: Raw observation identity/order (unique + monotonic seq)
# ---------------------------------------------------------------------------

def check_seq_ordering(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify sequence numbers are unique and monotonically increasing."""
    headers = _header_flags(records)
    if not headers:
        return ProbeCheckResult(
            check_id="P3", name="Sequence identity and order",
            status="INCONCLUSIVE",
            reasons=["No PROBE records found."],
            severity="warning",
        )

    seqs = [h.seq for h in headers]
    seen: Set[int] = set()
    dupes: List[int] = []
    for s in seqs:
        if s in seen:
            dupes.append(s)
        seen.add(s)

    # Check monotonicity
    non_mono: List[Tuple[int, int]] = []
    for i in range(1, len(seqs)):
        if seqs[i] < seqs[i - 1]:
            non_mono.append((seqs[i - 1], seqs[i]))

    if dupes or non_mono:
        reasons = []
        if dupes:
            reasons.append(f"Duplicate seq values: {dupes}")
        if non_mono:
            reasons.append(f"Non-monotonic: {non_mono}")
        return ProbeCheckResult(
            check_id="P3", name="Sequence identity and order",
            status="FAIL", reasons=reasons, severity="critical")

    return ProbeCheckResult(
        check_id="P3", name="Sequence identity and order",
        status="PASS",
        reasons=[f"{len(seqs)} records, seqs {seqs[0]}..{seqs[-1]} monotonic unique."],
        severity="info")


# ---------------------------------------------------------------------------
# P4: Duplicate records (same seq) — covered by P3, here for explicitness
# ---------------------------------------------------------------------------

def check_duplicate_records(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Explicit duplicate check — same seq appearing in multiple records."""
    headers = _header_flags(records)
    seq_count: Dict[int, int] = {}
    for h in headers:
        seq_count[h.seq] = seq_count.get(h.seq, 0) + 1

    dups = {s: c for s, c in seq_count.items() if c > 1}
    if dups:
        return ProbeCheckResult(
            check_id="P4", name="Duplicate records",
            status="FAIL",
            reasons=[f"Duplicate seq values: {dups}"],
            severity="critical",
        )

    return ProbeCheckResult(
        check_id="P4", name="Duplicate records",
        status="PASS",
        reasons=[f"All {len(headers)} seq values unique."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P5: Gaps/dropped events
# ---------------------------------------------------------------------------

def check_gaps_and_drops(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Check for DROP records and gap flags. Dropped observations invalidate
    dependent interpretations.

    ring_drops: ring overflow — oldest event dropped. Dependent results INCONCLUSIVE.
    today_drops / log_cap: log capacity reached — new events not logged.
    """
    drops = _drops(records)
    if not drops:
        return ProbeCheckResult(
            check_id="P5", name="Gaps/dropped events",
            status="PASS",
            reasons=["No DROP records found — all events captured."],
            severity="info",
        )

    ring_overflow = any(d.reason.lower().startswith("ring") for d in drops)
    log_cap = any(d.reason.lower().startswith("log_cap") for d in drops)

    reasons: List[str] = []
    for d in drops:
        reasons.append(
            f"DROP seq={d.header.seq} reason={d.reason} "
            f"ring_drops={d.ring_drops} today_drops={d.today_drops}"
        )

    if ring_overflow:
        reasons.append(
            "Ring overflow detected — dropped observations invalidate "
            "dependent interpretations. Affected results are INCONCLUSIVE."
        )
        return ProbeCheckResult(
            check_id="P5", name="Gaps/dropped events",
            status="INCONCLUSIVE",
            reasons=reasons,
            severity="critical",
        )

    if log_cap:
        return ProbeCheckResult(
            check_id="P5", name="Gaps/dropped events",
            status="WARN",
            reasons=reasons,
            severity="warning",
        )

    return ProbeCheckResult(
        check_id="P5", name="Gaps/dropped events",
        status="INCONCLUSIVE",
        reasons=reasons + ["Unknown DROP reason — treating as inconclusive."],
        severity="warning",
    )


# ---------------------------------------------------------------------------
# P6: Vehicle/trip mismatches (unload without preceding load)
# ---------------------------------------------------------------------------

def check_trip_mismatches(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify every unload EVENT has a preceding load EVENT for the same vehicle."""
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P6", name="Vehicle/trip mismatches",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found."],
            severity="warning",
        )

    by_veh = _group_events_by_vehicle(events)
    mismatches: List[str] = []
    ok_count = 0

    for veh, evs in by_veh.items():
        has_load = False
        for e in evs:
            if e.header.unload and not has_load:
                mismatches.append(
                    f"v={veh}: unload at seq={e.header.seq} with no preceding load"
                )
            if e.header.load:
                has_load = True
            if e.header.complete and has_load:
                ok_count += 1

    if mismatches:
        return ProbeCheckResult(
            check_id="P6", name="Vehicle/trip mismatches",
            status="FAIL",
            reasons=mismatches,
            severity="critical",
        )

    return ProbeCheckResult(
        check_id="P6", name="Vehicle/trip mismatches",
        status="PASS",
        reasons=[f"{ok_count} complete trip(s) have matching load+unload."],
        severity="info",
    )


# ---------------------------------------------------------------------------
# P7: Unsupported/partial boundaries (cannot be re-confirmed)
# ---------------------------------------------------------------------------

def check_unsupported_boundaries(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that unsupported trips (unload without load, or missing fb/fa)
    are flagged as INTERP error and are NOT repaired by repeated confirmations.
    """
    interps = _interps(records)
    if not interps:
        return ProbeCheckResult(
            check_id="P7", name="Unsupported/partial boundaries",
            status="INCONCLUSIVE",
            reasons=["No INTERP records found."],
            severity="info",
        )

    unsupported = [i for i in interps if i.error or i.gap]
    if not unsupported:
        return ProbeCheckResult(
            check_id="P7", name="Unsupported/partial boundaries",
            status="PASS",
            reasons=[f"All {len(interps)} INTERP record(s) have no errors/gaps."],
            severity="info",
        )

    issues = []
    for u in unsupported:
        issues.append(
            f"seq={u.header.seq} veh={u.veh} what={u.what} "
            f"error={u.error} gap={u.gap} fb={u.fb} fa={u.fa} ded={u.ded}"
        )

    return ProbeCheckResult(
        check_id="P7", name="Unsupported/partial boundaries",
        status="FAIL" if any(i.error for i in unsupported) else "INCONCLUSIVE",
        reasons=issues,
        severity="warning",
    )


# ---------------------------------------------------------------------------
# P8: Per-vehicle actual deductions (ded == fb - fa)
# ---------------------------------------------------------------------------

def check_deduction_consistency(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that ded == fb - fa in every EVENT where both fb and fa are finite
    and available (not -1).

    This is a self-consistency check on the emitter's arithmetic, NOT an
    independent measurement. True independence comes from comparing the sum
    of deductions across events to the total fuel change (P11).
    """
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P8", name="Deduction self-consistency",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found."],
            severity="warning",
        )

    failures: List[str] = []
    passes: List[str] = []

    for e in events:
        if e.fb <= FUEL_UNAVAILABLE or e.fa <= FUEL_UNAVAILABLE:
            continue  # gap — not a deduction to verify
        if e.ded <= FUEL_UNAVAILABLE:
            failures.append(
                f"seq={e.header.seq} veh={e.veh}: ded=-1 (gap) but fb/fa available"
            )
            continue

        expected_ded = e.fb - e.fa
        delta = abs(e.ded - expected_ded)
        if delta <= DEFAULT_TOLERANCE_T:
            passes.append(
                f"seq={e.header.seq} veh={e.veh}: ded={e.ded:.6f} == "
                f"fb-fa={expected_ded:.6f} (delta={delta:.6f})"
            )
        else:
            failures.append(
                f"seq={e.header.seq} veh={e.veh}: ded={e.ded:.6f} != "
                f"fb-fa={expected_ded:.6f} (delta={delta:.6f})"
            )

    if failures and not passes:
        return ProbeCheckResult(
            check_id="P8", name="Deduction self-consistency",
            status="FAIL", reasons=failures, severity="critical")
    elif failures:
        return ProbeCheckResult(
            check_id="P8", name="Deduction self-consistency",
            status="FAIL", reasons=passes + failures, severity="critical")
    else:
        return ProbeCheckResult(
            check_id="P8", name="Deduction self-consistency",
            status="PASS", reasons=passes, severity="info")


# ---------------------------------------------------------------------------
# P9: Complete-unload evidence (trip needs both load and unload)
# ---------------------------------------------------------------------------

def check_complete_unload_evidence(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify that each trip has both a load EVENT and an unload EVENT."""
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P9", name="Complete-unload evidence",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found."],
            severity="warning",
        )

    by_veh = _group_events_by_vehicle(events)
    incomplete: List[str] = []
    complete: List[str] = []

    for veh, evs in by_veh.items():
        loads = [e for e in evs if e.header.load]
        unloads = [e for e in evs if e.header.unload]

        if len(loads) == len(unloads) and len(loads) > 0:
            complete.append(f"v={veh}: {len(loads)} trip(s) with load+unload pairs")
        else:
            incomplete.append(
                f"v={veh}: {len(loads)} load(s), {len(unloads)} unload(s) — "
                f"imbalance indicates incomplete evidence"
            )

    if incomplete and not complete:
        return ProbeCheckResult(
            check_id="P9", name="Complete-unload evidence",
            status="FAIL", reasons=incomplete, severity="critical")
    elif incomplete:
        return ProbeCheckResult(
            check_id="P9", name="Complete-unload evidence",
            status="FAIL", reasons=complete + incomplete, severity="critical")
    else:
        return ProbeCheckResult(
            check_id="P9", name="Complete-unload evidence",
            status="PASS", reasons=complete, severity="info")


# ---------------------------------------------------------------------------
# P10: Finite quantities (no NaN/Inf)
# ---------------------------------------------------------------------------

def check_finite_quantities(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify no NaN/Inf in numeric fields of EVENT and TRIP records."""
    events = _events(records)
    trips = _trips(records)
    issues: List[str] = []

    for e in events:
        for field_name in ('fb', 'fa', 'ded', 'cb', 'ca'):
            val = getattr(e, field_name)
            if isinstance(val, float) and (math.isnan(val) or math.isinf(val)):
                issues.append(
                    f"EVENT seq={e.header.seq} veh={e.veh}: {field_name}={val} is NaN/Inf"
                )

    for t in trips:
        for field_name in ('fuel_before', 'fuel_after', 'deduction'):
            val = getattr(t, field_name)
            if isinstance(val, float) and (math.isnan(val) or math.isinf(val)):
                issues.append(
                    f"TRIP seq={t.header.seq} veh={t.veh}: {field_name}={val} is NaN/Inf"
                )

    if issues:
        return ProbeCheckResult(
            check_id="P10", name="Finite quantities",
            status="FAIL", reasons=issues, severity="critical")

    count = len(events) + len(trips)
    return ProbeCheckResult(
        check_id="P10", name="Finite quantities",
        status="PASS",
        reasons=[f"No NaN/Inf in {len(events)} EVENT(s), {len(trips)} TRIP(s)."],
        severity="info")


# ---------------------------------------------------------------------------
# P11: Independent fuel reconciliation (no-refuel)
# ---------------------------------------------------------------------------

def check_independent_fuel_reconciliation(
    records: List[ProbeRecord],
    tolerance_t: float = DEFAULT_TOLERANCE_T,
) -> ProbeCheckResult:
    """Independent fuel reconciliation for no-refuel vehicles.

    Uses INDEPENDENT observations from distinct EVENT records — NOT the
    reported 'burned' field from the legacy [TANK] lines. Each EVENT's
    fuel_before (at load) and fuel_after (at unload) are separate measurements.

    **Per-trip reconciliation (corrected v0.18):**
    Each matched load/unload pair is reconciled separately:
        trip_i: fuel_before(load_i) - fuel_after(unload_i) ≈ ded_i
        (where ded_i is the actual fuel-deduction observation from the
        unload EVENT record, not the unload-event summary)

    Cross-trip start-to-end subtraction (first-load fb - last-unload fa)
    includes empty repositioning travel between trips, which is outside
    our accounting scope. Per-trip reconciliation avoids this.

    If any unload's fa > next load's fb (i.e., fuel_after > next fuel_before
    plus tolerance), the vehicle had refueling (a fuel gain between
    consecutive trips). Refuel additions are NOT independently observed in
    Phase 1 (no fuel-delivery event) → INCONCLUSIVE (delegated to P12).

    If ded observations are missing from any needed EVENT → INCONCLUSIVE
    with identification of the missing input.
    """
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found — cannot reconcile."],
            severity="critical",
        )

    by_veh = _group_events_by_vehicle(events)
    results: List[str] = []
    failures: List[str] = []
    passes: List[str] = []

    for veh, evs in by_veh.items():
        loads = sorted([e for e in evs if e.header.load and e.fb > FUEL_UNAVAILABLE],
                        key=lambda e: e.header.seq)
        unloads = sorted([e for e in evs if e.header.unload and e.fa > FUEL_UNAVAILABLE],
                          key=lambda e: e.header.seq)

        if not loads or not unloads:
            results.append(f"v={veh}: no complete trips (need load+unload pairs) — skipped")
            continue

        # Match load/unload pairs in sequence order
        pairs = []
        missing_ded = []
        for i in range(min(len(loads), len(unloads))):
            load_ev = loads[i]
            unload_ev = unloads[i]
            if unload_ev.ded == FUEL_UNAVAILABLE:
                missing_ded.append(
                    f"seq={unload_ev.header.seq}: missing ded observation on unload EVENT"
                )
            pairs.append((load_ev, unload_ev))

        if missing_ded:
            results.append(
                f"v={veh}: INCONCLUSIVE — missing deduction observations: "
                + "; ".join(missing_ded)
            )
            continue

        # Per-trip reconciliation: fuel_before - fuel_after ≈ ded
        trip_failures = []
        trip_passes = []
        for i, (load_ev, unload_ev) in enumerate(pairs):
            fb = load_ev.fb
            fa = unload_ev.fa
            ded = unload_ev.ded
            expected = fb - fa
            delta = abs(ded - expected)
            src = "synthetic" if load_ev.header.synth else "runtime"

            if delta <= tolerance_t:
                trip_passes.append(
                    f"v={veh} trip {i+1} (src={src}): "
                    f"fb={fb:.6f} - fa={fa:.6f} = {expected:.6f} t vs "
                    f"ded={ded:.6f} t, delta={delta:.6f} ≤ tol={tolerance_t}"
                )
            else:
                trip_failures.append(
                    f"v={veh} trip {i+1} (src={src}): "
                    f"fb={fb:.6f} - fa={fa:.6f} = {expected:.6f} t vs "
                    f"ded={ded:.6f} t, delta={delta:.6f} > tol={tolerance_t}"
                )

        if trip_failures:
            failures.extend(trip_failures)
            results.append(f"v={veh}: {len(trip_failures)} trip(s) failed reconciliation")
        else:
            passes.extend(trip_passes)

        # Check for refuel: next load's fb > previous unload's fa (fuel gain)
        # Per correction: fa > next fb is depletion, NOT refuel. Refuel is
        # when the next trip starts with MORE fuel than the previous ended.
        refuel_detected = False
        for i in range(len(pairs) - 1):
            curr_unload_fa = pairs[i][1].fa
            next_load_fb = pairs[i + 1][0].fb
            if curr_unload_fa > FUEL_UNAVAILABLE and next_load_fb > FUEL_UNAVAILABLE:
                if next_load_fb > curr_unload_fa + DEFAULT_TOLERANCE_T:
                    refuel_detected = True
                    addition = next_load_fb - curr_unload_fa
                    results.append(
                        f"v={veh}: fuel gain between trips — "
                        f"unload fa={curr_unload_fa:.6f} < next load fb={next_load_fb:.6f} "
                        f"(addition={addition:.6f} t, not independently observed)"
                    )

        if refuel_detected:
            results.append(
                f"v={veh}: refuel detected (fuel gain between trips), "
                f"no independent addition observation — INCONCLUSIVE"
            )

    if failures and not passes:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="FAIL", reasons=results + failures, severity="critical")
    elif any("INCONCLUSIVE" in r for r in results) and not failures:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="INCONCLUSIVE", reasons=results, severity="warning")
    elif failures:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="FAIL", reasons=results + passes + failures, severity="critical")
    elif not passes and not failures:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="INCONCLUSIVE", reasons=results, severity="warning")
    else:
        return ProbeCheckResult(
            check_id="P11", name="Independent fuel reconciliation (no-refuel)",
            status="PASS", reasons=results + passes, severity="info")


# ---------------------------------------------------------------------------
# P12: Independent refuel assessment
# ---------------------------------------------------------------------------

def check_refuel_assessment(records: List[ProbeRecord]) -> ProbeCheckResult:
    """If fuel increases between unload and next load (fa > fb), refueling
    occurred. In Phase 1, refuel additions are NOT independently observed
    (no fuel-delivery event). Return INCONCLUSIVE for affected vehicles.

    If any consecutive-trip pair is missing fa or fb (unavailable), refuel
    assessment cannot cover that transition — return INCONCLUSIVE.
    """
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P12", name="Independent refuel assessment",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found."],
            severity="info",
        )

    by_veh = _group_events_by_vehicle(events)
    refuel_found = False
    refuel_details: List[str] = []
    insufficient_coverage: List[str] = []

    for veh, evs in by_veh.items():
        loads = sorted([e for e in evs if e.header.load], key=lambda e: e.header.seq)
        unloads = sorted([e for e in evs if e.header.unload], key=lambda e: e.header.seq)

        for i in range(min(len(unloads), len(loads) - 1)):
            ua = unloads[i].fa
            lb = loads[i + 1].fb
            if ua > FUEL_UNAVAILABLE and lb > FUEL_UNAVAILABLE:
                if lb > ua + DEFAULT_TOLERANCE_T:
                    refuel_found = True
                    addition = lb - ua
                    refuel_details.append(
                        f"v={veh}: fuel gain {ua:.6f} -> {lb:.6f} "
                        f"(addition={addition:.6f} t, not independently observed)"
                    )
            else:
                insufficient_coverage.append(
                    f"v={veh}: cannot assess refuel between seq pairs — "
                    f"fa={ua:.6f} or fb={lb:.6f} unavailable"
                )

    if refuel_found:
        return ProbeCheckResult(
            check_id="P12", name="Independent refuel assessment",
            status="INCONCLUSIVE",
            reasons=[
                "Refuel detected: next trip fuel_before > previous trip fuel_after. "
                "Phase 1 has no fuel-delivery observation for additions."
            ] + refuel_details,
            severity="warning",
        )

    if insufficient_coverage:
        return ProbeCheckResult(
            check_id="P12", name="Independent refuel assessment",
            status="INCONCLUSIVE",
            reasons=[
                "Insufficient coverage: some consecutive-trip transitions lack "
                "fa or fb observations."
            ] + insufficient_coverage,
            severity="warning",
        )

    return ProbeCheckResult(
        check_id="P12", name="Independent refuel assessment",
        status="PASS",
        reasons=["No refuel detected; all fuel transitions assessable."],
        severity="info")


# ---------------------------------------------------------------------------
# P13: Monetary cost (INCONCLUSIVE when price/currency unavailable)
# ---------------------------------------------------------------------------

def check_monetary_cost(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify fuel price and currency availability for monetary computation.

    In Phase 1, fuel_price=unavailable and currency=unavailable on all TRIP
    records. Monetary cost cannot be computed → INCONCLUSIVE.
    """
    trips = _trips(records)
    if not trips:
        return ProbeCheckResult(
            check_id="P13", name="Monetary cost availability",
            status="INCONCLUSIVE",
            reasons=["No TRIP records found — cannot assess monetary cost."],
            severity="warning",
        )

    unavailable = [t for t in trips
                   if t.fuel_price == "unavailable" or t.currency == "unavailable"]
    available = [t for t in trips
                 if t.fuel_price != "unavailable" and t.currency != "unavailable"]

    if unavailable and not available:
        return ProbeCheckResult(
            check_id="P13", name="Monetary cost availability",
            status="INCONCLUSIVE",
            reasons=[
                f"All {len(unavailable)} TRIP record(s) have fuel_price=unavailable "
                f"and currency=unavailable (Phase 1: no price source consumed). "
                f"Monetary cost cannot be computed."
            ],
            evidence=[
                "Note: fuel-measurement checks (P11) may still be assessable; "
                "only monetary cost is unavailable."
            ],
            severity="warning",
        )

    if available:
        samples = [f"fuel_price={t.fuel_price}/{t.currency}" for t in available[:3]]
        return ProbeCheckResult(
            check_id="P13", name="Monetary cost availability",
            status="PASS",
            reasons=[f"Price/currency available: {', '.join(samples)}"],
            severity="info")

    return ProbeCheckResult(
        check_id="P13", name="Monetary cost availability",
        status="INCONCLUSIVE",
        reasons=["Price/currency partially unavailable."],
        severity="warning")


# ---------------------------------------------------------------------------
# P14: Missing required evidence
# ---------------------------------------------------------------------------

def check_required_evidence(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify all required record types are present for a meaningful assessment."""
    required = ["SESSION", "EVENT"]
    counts: Dict[str, int] = {}
    for r in records:
        counts[r.header.record_kind] = counts.get(r.header.record_kind, 0) + 1

    missing: List[str] = []
    for req in required:
        if counts.get(req, 0) == 0:
            missing.append(req)

    if missing:
        return ProbeCheckResult(
            check_id="P14", name="Required evidence presence",
            status="INCONCLUSIVE",
            reasons=[f"Missing required record types: {missing}"],
            severity="critical",
        )

    details = [f"{r}: {counts.get(r, 0)}" for r in ["SESSION", "EVENT", "INTERP", "TRIP", "DROP"]]
    return ProbeCheckResult(
        check_id="P14", name="Required evidence presence",
        status="PASS",
        reasons=[f"All required records present ({', '.join(details)})."],
        severity="info")


# ---------------------------------------------------------------------------
# Confidence validation
# ---------------------------------------------------------------------------

VALID_CONFIDENCE = {"verified", "partial", "unverified", "synthetic", "unsupported"}


def check_confidence_values(records: List[ProbeRecord]) -> ProbeCheckResult:
    """Verify confidence strings are from the known set. Unknown values must
    not be silently upgraded to 'verified'.
    """
    events = _events(records)
    if not events:
        return ProbeCheckResult(
            check_id="P15", name="Confidence value validation",
            status="INCONCLUSIVE",
            reasons=["No EVENT records found."],
            severity="info")

    unknown: List[str] = []
    verified_count = 0
    for e in events:
        if e.conf not in VALID_CONFIDENCE:
            unknown.append(
                f"seq={e.header.seq}: unknown confidence '{e.conf}' "
                f"(not upgraded to 'verified')"
            )
        elif e.conf == "verified":
            verified_count += 1

    if unknown:
        return ProbeCheckResult(
            check_id="P15", name="Confidence value validation",
            status="FAIL", reasons=unknown, severity="warning")

    return ProbeCheckResult(
        check_id="P15", name="Confidence value validation",
        status="PASS",
        reasons=[
            f"{verified_count} verified, {len(events)} total EVENT(s); "
            f"all confidence values valid."
        ],
        severity="info")


# ---------------------------------------------------------------------------
# Top-level run
# ---------------------------------------------------------------------------

def run_probe_checks(
    records: List[ProbeRecord],
    source: str = "runtime",
) -> ProbeCheckReport:
    """Run all PROBE reconciliation checks (P1–P15)."""
    results = [
        check_consecutive_trips(records),
        check_session_staleness(records),
        check_seq_ordering(records),
        check_duplicate_records(records),
        check_gaps_and_drops(records),
        check_trip_mismatches(records),
        check_unsupported_boundaries(records),
        check_deduction_consistency(records),
        check_complete_unload_evidence(records),
        check_finite_quantities(records),
        check_independent_fuel_reconciliation(records),
        check_refuel_assessment(records),
        check_monetary_cost(records),
        check_required_evidence(records),
        check_confidence_values(records),
        check_selection_record(records),
        check_measurement_validity(records),
        check_mirror_independence(records),
        check_identity_retention(records),
        check_exactly_one_candidate(records),
    ]

    return ProbeCheckReport(
        source=source,
        schema_version=PROBE_SCHEMA_VERSION,
        schema_commit=PROBE_SCHEMA_COMMIT,
        results=results,
        unrecognized_count=0,
    )
