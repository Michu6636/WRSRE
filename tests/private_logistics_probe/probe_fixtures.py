"""Synthetic [PROBE ...] log generators for the private-logistics probe checker.

Each generator produces raw log text in the EXACT format emitted by
plugins/private_logistics_probe/private_logistics_probe.cpp (commit ce8648d5).

Key emitter behaviors to reproduce:
  - Records span multiple lines (header + continuation lines prefixed with space)
  - kind= field is buggy: ConfName(seq % 6 + 1), not the real kind
  - thr= formatted as hex pointer (%p style)
  - res=? for synthetic/unresolvable pointers
  - ts formatted with 4 decimal places
  - day=-1, mon=1, yr=1970 when sim day unavailable
  - fuel_price=unavailable, currency=unavailable in Phase 1
  - synth=1 on all selftest records
"""

from __future__ import annotations

# ---------------------------------------------------------------------------
# Constants — must match the emitter's synthetic selftest
# ---------------------------------------------------------------------------

PROBE_SCHEMA_VERSION = 1
V_TRUCK_A = "00000000AAAA0001"   # synthetic vehicle pointer
V_TRUCK_B = "00000000AAAA0002"
R_FUEL = "fuel"
SITE_LOAD_1 = "00000000CCCC0001"
SITE_LOAD_2 = "00000000CCCC0002"
SITE_UNLOAD_1 = "00000000CCCC0002"
SITE_UNLOAD_2 = "00000000CCCC0003"
DO_WEST = "000001F2A3B4C5D6"    # real DO pointer (for session notes)


def _ts(minute: int, second: int, ms: int) -> str:
    """Generate [HH:MM:SS.mmm] timestamp."""
    return f"[00:{minute:02d}:{second:02d}.{ms:03d}]"


# ---------------------------------------------------------------------------
# Emitter behavior reproduction helpers
# ---------------------------------------------------------------------------

def _buggy_kind(seq: int) -> str:
    """Reproduce the emitter's kind= bug: ConfName(seq % 6 + 1)."""
    names = ["session", "event", "interp", "trip", "drop", "synth"]
    return names[seq % 6]


def _probe_header(record_kind: str, seq: int, ts: float, flags: int, synth: int) -> str:
    """Generate a PROBE header line with the buggy kind= field."""
    committed = 1 if (flags & 0x01) else 0
    load = 1 if (flags & 0x02) else 0
    unload = 1 if (flags & 0x04) else 0
    complete = 1 if (flags & 0x08) else 0
    gap = 1 if (flags & 0x10) else 0
    error = 1 if (flags & 0x20) else 0
    return (
        f"[PROBE {record_kind}] seq={seq} ts={ts:.4f} thr=00000000000007F1 "
        f"kind={_buggy_kind(seq)} schema={PROBE_SCHEMA_VERSION} "
        f"committed={committed} load={load} unload={unload} "
        f"complete={complete} gap={gap} error={error} synth={synth}"
    )


# ---------------------------------------------------------------------------
# F-P1: Two consecutive trips for one vehicle (Trip 1 + Trip 2)
# Matches the emitter's selftest exactly.
# ---------------------------------------------------------------------------

def fixture_consecutive_trips() -> str:
    """Two complete trips on the same vehicle, chaining correctly."""
    lines = [
        # SESSION
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f" session_id=AAAA0000BBBB0000 event=armed day=-1 mon=1 yr=1970 note=\"probe enabled\"",
        # SELFTEST banner
        f"{_ts(0,0,2)} private_logistics_probe  selftest: labelled synthetic accumulator exercise begins",
        # Trip 1: load
        f"{_ts(0,0,3)} {_probe_header('EVENT', 2, 0.0110, 0x42, 1)}",  # LOAD|SYNTH
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site={SITE_LOAD_1} unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: INTERP trip_open_load
        f"{_ts(0,0,4)} {_probe_header('INTERP', 3, 0.0150, 0x40, 1)}",  # SYNTH
        f' veh={V_TRUCK_A} what=\"trip_open_load\" fb=0.450000 fa=-1.000000 ded=-1.000000 complete=0 gap=0 error=0',
        # Trip 1: unload
        f"{_ts(0,0,5)} {_probe_header('EVENT', 4, 0.0200, 0x45, 1)}",  # COMMITTED|UNLOAD|SYNTH
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site={SITE_LOAD_1} unload_site={SITE_UNLOAD_1}",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: INTERP trip_complete
        f"{_ts(0,0,6)} {_probe_header('INTERP', 5, 0.0250, 0x48, 1)}",  # COMPLETE|SYNTH
        f' veh={V_TRUCK_A} what=\"trip_complete\" fb=0.450000 fa=0.428000 ded=0.022000 complete=1 gap=0 error=0',
        # Trip 2: load (fb=0.4280, chains from trip 1's fa)
        f"{_ts(0,0,7)} {_probe_header('EVENT', 6, 0.0300, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site={SITE_LOAD_1} unload_site=null",
        f" fb=0.428000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload (fa=0.4060, ded=0.0220)
        f"{_ts(0,0,8)} {_probe_header('EVENT', 7, 0.0350, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site={SITE_LOAD_1} unload_site={SITE_UNLOAD_1}",
        f" fb=0.428000 fa=0.406000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: INTERP trip_complete_consecutive
        f"{_ts(0,0,9)} {_probe_header('INTERP', 8, 0.0400, 0x48, 1)}",
        f' veh={V_TRUCK_A} what=\"trip_complete_consecutive\" fb=0.428000 fa=0.406000 ded=0.022000 complete=1 gap=0 error=0',
        # TRIP summary
        f"{_ts(0,0,10)} {_probe_header('TRIP', 9, 0.0450, 0x08, 0)}",  # COMPLETE only
        f" veh={V_TRUCK_A} fuel_before=0.450000 fuel_after=0.406000 deduction=0.044000 trips=2 completed=2 replayed=0 unsupported=0 dup_loads=0 fuel_price=unavailable currency=unavailable note=phase1_no_price_source",
        # RESET + final log
        f"{_ts(0,0,11)} private_logistics_probe  selftest: reset issued; stale open trips invalidated",
        f"{_ts(0,0,12)} private_logistics_probe  selftest: completed=2 replayed=0 unsupported=0 dup_loads=0",
        f"{_ts(0,0,13)} private_logistics_probe  selftest: labelled synthetic accumulator exercise ends",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P2: Session boundary with stale records
# ---------------------------------------------------------------------------

def fixture_stale_records() -> str:
    """An event with seq below the session seq — should be flagged."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 100, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=-1 mon=1 yr=1970 note=\"armed\"',
        # Event with seq <= session seq (stale)
        f"{_ts(0,0,2)} {_probe_header('EVENT', 50, 0.0100, 0x40, 0)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site={SITE_LOAD_1} unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P3: Sequence ordering (monotonic + unique)
# ---------------------------------------------------------------------------

def fixture_seq_ordering_ok() -> str:
    """Events with monotonic, unique seq values."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_seq_non_monotonic() -> str:
    """Events with non-monotonic seq (3 before 2)."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P4: Duplicate records
# ---------------------------------------------------------------------------

def fixture_duplicate_seq() -> str:
    """Two records with the same seq."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 2, 0.0150, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P5: Gap/dropped events
# ---------------------------------------------------------------------------

def fixture_no_drops() -> str:
    """No DROP records."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_ring_overflow() -> str:
    """DROP record with ring_overflow reason."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('DROP', 3, 0.0150, 0x20, 0)}",
        f' reason=\"ring_overflow\" ring_drops=1 today_drops=1',
    ]
    return "\n".join(lines) + "\n"


def fixture_log_cap() -> str:
    """DROP record with log_cap reason."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('DROP', 2, 0.0050, 0x20, 0)}",
        f' reason=\"log_cap\" ring_drops=0 today_drops=1',
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P6: Vehicle/trip mismatches (unload without load)
# ---------------------------------------------------------------------------

def fixture_trip_mismatch() -> str:
    """Unload event with no preceding load for the same vehicle."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x45, 1)}",  # UNLOAD|SYNTH
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_trip_match_ok() -> str:
    """Load followed by unload — valid trip."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P7: Unsupported/partial boundaries
# ---------------------------------------------------------------------------

def fixture_unsupported_unload() -> str:
    """Unload with no preceding load (unsupported boundary)."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x41, 1)}",  # LOAD|UNLOAD|SYNTH|GAP
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=null unload_site=def",
        f" fb=-1.000000 fa=0.400000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=unsupported day=-1",
        f"{_ts(0,0,3)} {_probe_header('INTERP', 3, 0.0150, 0x60, 1)}",  # GAP|ERROR|SYNTH
        f' veh={V_TRUCK_A} what=\"unsupported_unload\" fb=-1.000000 fa=0.400000 ded=-1.000000 complete=0 gap=1 error=1',
    ]
    return "\n".join(lines) + "\n"


def fixture_supported_no_errors() -> str:
    """All INTERP records have no errors or gaps."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('INTERP', 2, 0.0100, 0x48, 1)}",  # COMPLETE|SYNTH
        f' veh={V_TRUCK_A} what=\"trip_complete\" fb=0.450000 fa=0.428000 ded=0.022000 complete=1 gap=0 error=0',
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P8: Deduction self-consistency
# ---------------------------------------------------------------------------

def fixture_deduction_consistent() -> str:
    """All ded values match fb - fa."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_deduction_inconsistent() -> str:
    """ded does NOT match fb - fa (emitter arithmetic bug)."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.050000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P9: Complete-unload evidence
# ---------------------------------------------------------------------------

def fixture_complete_unload() -> str:
    """Load + unload pair — complete evidence."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_incomplete_unload() -> str:
    """Load without unload — incomplete evidence."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P10: NaN/Inf in numeric fields
# ---------------------------------------------------------------------------

def fixture_nan_fuel() -> str:
    """EVENT with nan fuel value."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=nan fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P11: Independent fuel reconciliation (PASS scenario)
# ---------------------------------------------------------------------------

def fixture_independent_reconciliation_pass() -> str:
    """No-refuel: start_tank - end_tank == sum(deductions)."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        # Trip 1: load fb=0.4500
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload fa=0.4280, ded=0.0220
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load fb=0.4280 (chains from trip 1)
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.428000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload fa=0.4060, ded=0.0220
        f"{_ts(0,0,5)} {_probe_header('EVENT', 5, 0.0400, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.428000 fa=0.406000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P12: Refuel detected (INCONCLUSIVE)
# ---------------------------------------------------------------------------

def fixture_refuel_detected() -> str:
    """After unload, the next load's fb > fa — refuel detected, not observable."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        # Trip 1: load fb=0.4500
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload fa=0.4280
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load fb=0.4700 > fa 0.4280 — refuel detected
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.470000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P13: Monetary cost unavailable
# ---------------------------------------------------------------------------

def fixture_price_unavailable() -> str:
    """TRIP records with fuel_price=unavailable, currency=unavailable."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('TRIP', 2, 0.0100, 0x08, 0)}",
        f" veh={V_TRUCK_A} fuel_before=0.450000 fuel_after=0.406000 deduction=0.044000 trips=2 completed=2 replayed=0 unsupported=0 dup_loads=0 fuel_price=unavailable currency=unavailable note=phase1_no_price_source",
    ]
    return "\n".join(lines) + "\n"


def fixture_price_available() -> str:
    """TRIP records with actual fuel_price and currency."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('TRIP', 2, 0.0100, 0x08, 0)}",
        f" veh={V_TRUCK_A} fuel_before=0.450000 fuel_after=0.406000 deduction=0.044000 trips=2 completed=2 replayed=0 unsupported=0 dup_loads=0 fuel_price=6.90 currency=RUB note=resolved",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P14: Missing required evidence
# ---------------------------------------------------------------------------

def fixture_no_events() -> str:
    """No EVENT records."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
    ]
    return "\n".join(lines) + "\n"


def fixture_no_session() -> str:
    """No SESSION records."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('EVENT', 1, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P15: Unknown confidence value
# ---------------------------------------------------------------------------

def fixture_unknown_confidence() -> str:
    """EVENT with an unknown confidence string."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=bogus day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P16: Replay guard — completed trip cannot replay
# ---------------------------------------------------------------------------

def fixture_replay_guard() -> str:
    """Second unload on a closed trip does not increase completed count."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note=\"disabled\"',
        # Trip 1 complete
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site=\"synthetic:load\" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site=\"synthetic:unload\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Replay attempt — same vehicle, second unload without a new load
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x45, 1)}",
        f' site=\"synthetic:replay\" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.428000 fa=0.428000 ded=0.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # TRIP summary showing completed=2 but replayed=0
        f"{_ts(0,0,5)} {_probe_header('TRIP', 5, 0.0350, 0x08, 0)}",
        f" veh={V_TRUCK_A} fuel_before=0.450000 fuel_after=0.428000 deduction=0.022000 trips=2 completed=1 replayed=0 unsupported=1 dup_loads=0 fuel_price=unavailable currency=unavailable note=phase1_no_price_source",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P16: Regression fixtures for reconciliation corrections
# ---------------------------------------------------------------------------

def fixture_empty_travel_consumption() -> str:
    """Two trips separated by empty-travel consumption.

    Trip 1: fb=0.450, fa=0.428, ded=0.022 → per-trip: 0.450-0.428=0.022 ✓
    Trip 2: fb=0.420, fa=0.402, ded=0.018 → per-trip: 0.420-0.402=0.018 ✓
    (empty travel burned 0.008 t between trips — OUTSIDE per-trip scope)

    Cross-trip start-end = 0.450-0.402 = 0.048, but Σded = 0.040.
    The old cross-trip approach would FAIL here spuriously.
    Per-trip reconciliation correctly PASSES.
    """
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note="disabled"',
        # Trip 1: load
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload (ded=0.022)
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load (fb=0.420, LOWER than trip 1's fa=0.428 — empty travel burn)
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.420000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload (ded=0.018)
        f"{_ts(0,0,5)} {_probe_header('EVENT', 5, 0.0400, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.420000 fa=0.402000 ded=0.018000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_visible_fuel_addition() -> str:
    """Visible fuel addition between trips (refuel) — P12 should INCONCLUSIVE.

    Trip 1: fa=0.428, Trip 2: fb=0.450 > fa — fuel was added (refuel).
    P11 should also INCONCLUSIVE (refuel detected without independent observation).
    """
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note="disabled"',
        # Trip 1: load fb=0.450
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload fa=0.428
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load fb=0.450 > fa=0.428 — refuel detected
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload fa=0.428 (no consumption — vehicle was refueled)
        f"{_ts(0,0,5)} {_probe_header('EVENT', 5, 0.0400, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_missing_deduction_observation() -> str:
    """Unload EVENT has ded=-1 (unavailable) — P11 INCONCLUSIVE.

    The emitter did not record a fuel-deduction observation on the unload.
    Without ded, per-trip reconciliation cannot verify fb-fa ≈ ded.
    """
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note="disabled"',
        # Trip 1: load
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload but ded is unavailable (FUEL_UNAVAILABLE = -1.0)
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.450000 fa=0.428000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.428000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload (ded available)
        f"{_ts(0,0,5)} {_probe_header('EVENT', 5, 0.0400, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.428000 fa=0.406000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_insufficient_refuel_coverage() -> str:
    """Trip where fb/fa unavailable on some events — P12 INCONCLUSIVE.

    P12 requires both fa (from previous unload) and fb (from next load)
    to detect refuel. If either is unavailable, cannot determine coverage.
    """
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=disabled day=-1 mon=1 yr=1970 note="disabled"',
        # Trip 1: load (fb available)
        f"{_ts(0,0,2)} {_probe_header('EVENT', 2, 0.0100, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.450000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 1: unload (fa unavailable — sensor gap)
        f"{_ts(0,0,3)} {_probe_header('EVENT', 3, 0.0200, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=-1.000000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: load (fb available)
        f"{_ts(0,0,4)} {_probe_header('EVENT', 4, 0.0300, 0x42, 1)}",
        f' site="synthetic:load" mode=0',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=null",
        f" fb=0.428000 fa=-1.000000 ded=-1.000000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
        # Trip 2: unload (fa available)
        f"{_ts(0,0,5)} {_probe_header('EVENT', 5, 0.0400, 0x45, 1)}",
        f' site="synthetic:unload" mode=1',
        f" veh={V_TRUCK_A} do=null res=? load_site=abc unload_site=def",
        f" fb=0.428000 fa=0.406000 ded=0.022000 cb=0.000000 ca=0.000000 conf=synthetic day=-1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F-P16: SNAPSHOT fixtures
# ---------------------------------------------------------------------------

def fixture_snapshot_selection() -> str:
    """Selection record: exactly one eligible road DO truck found."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        # Selection record: vehicle selected, no measurements yet
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0100, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=12.5000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=selection snapshot_ok=1 fail_reason=\"\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=1",
        f" mode_valid=1 mode=9",
        f" home_matches_valid=1 home_matches=1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_measurement_complete() -> str:
    """Measurement record: all fields valid."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0200, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=12.5000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=measurement snapshot_ok=1 fail_reason=\"\"",
        f" fuel_slot_valid=1 fuel_tons=0.430000",
        f" mirror_valid=1 mirror_tons=0.432000",
        f" cargo_valid=1 cargo_slots=2 cargo_total_tons=1.000000 cargo=\"fuel=0.430t goods=0.570t\"",
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=1",
        f" mode_valid=1 mode=9",
        f" home_matches_valid=1 home_matches=1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_measurement_unavailable() -> str:
    """Measurement record: some fields unavailable (validity flags = 0, values = -1).
    Missing/unreadable cargo is unavailable, not zero."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0300, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=13.0000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=measurement snapshot_ok=1 fail_reason=\"\"",
        # fuel slot unavailable
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        # mirror readable but does NOT validate slot
        f" mirror_valid=1 mirror_tons=0.432000",
        # cargo unavailable
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        # state/mode readable but don't invalidate
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=2",
        f" mode_valid=1 mode=9",
        f" home_matches_valid=1 home_matches=1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_no_match() -> str:
    """No-match record: no eligible candidate found."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0100, 0x20, 0)}",
        f' day=5 mon=1 yr=2026 tod=12.5000',
        f" veh=null do=null",
        f" record_type=no_match snapshot_ok=0 fail_reason=\"no_match\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=0 kind=-1",
        f" state_valid=0 state=-1",
        f" mode_valid=0 mode=-1",
        f" home_matches_valid=0 home_matches=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_vehicle_lost() -> str:
    """Vehicle lost record: previously selected vehicle no longer valid."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        # Selection first
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0100, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=12.5000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=selection snapshot_ok=1 fail_reason=\"\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=1",
        f" mode_valid=1 mode=9",
        f" home_matches_valid=1 home_matches=1",
        # Vehicle lost
        f"{_ts(0,0,3)} {_probe_header('SNAPSHOT', 3, 0.0400, 0x40, 0)}",
        f' day=5 mon=1 yr=2026 tod=13.0000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=vehicle_lost snapshot_ok=0 fail_reason=\"vehicle_gone\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=0 kind=-1",
        f" state_valid=0 state=-1",
        f" mode_valid=0 mode=-1",
        f" home_matches_valid=0 home_matches=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_world_reset() -> str:
    """World reset record: world unloaded, selection reset."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0100, 0x40, 0)}",
        f' day=-1 mon=-1 yr=-1 tod=0.0000',
        f" veh=null do=null",
        f" record_type=world_reset snapshot_ok=0 fail_reason=\"world_reset\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=0 kind=-1",
        f" state_valid=0 state=-1",
        f" mode_valid=0 mode=-1",
        f" home_matches_valid=0 home_matches=-1",
    ]
    return "\n".join(lines) + "\n"


def fixture_snapshot_identity_retained() -> str:
    """Identity retention: same vehicle pointer across state changes (mode 1→2→9)."""
    lines = [
        f"{_ts(0,0,1)} {_probe_header('SESSION', 1, 0.0001, 0, 0)}",
        f' session_id=AAAA0000BBBB0000 event=armed day=5 mon=1 yr=2026 note=\"armed\"',
        # Selection
        f"{_ts(0,0,2)} {_probe_header('SNAPSHOT', 2, 0.0100, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=12.5000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=selection snapshot_ok=1 fail_reason=\"\"",
        f" fuel_slot_valid=0 fuel_tons=-1.000000",
        f" mirror_valid=0 mirror_tons=-1.000000",
        f" cargo_valid=0 cargo_slots=-1 cargo_total_tons=-1.000000 cargo=\"\"",
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=1",
        f" mode_valid=1 mode=0",
        f" home_matches_valid=1 home_matches=1",
        # Measurement: same vehicle, state changed to 2, mode to 9 — identity retained
        f"{_ts(0,0,3)} {_probe_header('SNAPSHOT', 3, 0.0200, 0x08, 0)}",
        f' day=5 mon=1 yr=2026 tod=13.0000',
        f" veh={V_TRUCK_A} do={DO_WEST}",
        f" record_type=measurement snapshot_ok=1 fail_reason=\"\"",
        f" fuel_slot_valid=1 fuel_tons=0.428000",
        f" mirror_valid=1 mirror_tons=0.430000",
        f" cargo_valid=1 cargo_slots=1 cargo_total_tons=0.500000 cargo=\"goods=0.500t\"",
        f" kind_valid=1 kind=66",
        f" state_valid=1 state=2",
        f" mode_valid=1 mode=9",
        f" home_matches_valid=1 home_matches=1",
    ]
    return "\n".join(lines) + "\n"
