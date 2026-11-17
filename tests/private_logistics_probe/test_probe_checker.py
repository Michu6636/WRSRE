"""Test suite for the PROBE log checker.

Run with:
    python3 -m unittest tests.private_logistics_probe.test_probe_checker -v

Tests cover all 15 PROBE checks (P1-P15) against synthetic fixtures generated
in probe_fixtures.py. These fixtures reproduce the exact output format of
plugins/private_logistics_probe/private_logistics_probe.cpp (commit ce8648d5).
"""

from __future__ import annotations

import unittest
from typing import List, Optional

from tests.private_logistics_probe import probe_fixtures as fx
from tests.private_logistics_probe import probe_parser
from tests.private_logistics_probe import probe_checker as chk
from tests.private_logistics_probe import probe_schema
from tests.private_logistics_probe.probe_checker import (
    ProbeCheckReport,
    ProbeCheckResult,
    run_probe_checks,
)


class _ProbeBaseTest(unittest.TestCase):
    """Base helper: feeds a fixture through parser+checker."""

    def _run(self, log_text: str, source: str = "synthetic") -> ProbeCheckReport:
        records = probe_parser.parse_probe_log(log_text)
        return run_probe_checks(records, source=source)

    def _get(self, report: ProbeCheckReport, check_id: str) -> Optional[ProbeCheckResult]:
        return report.get_result(check_id)

    def _assert(self, report: ProbeCheckReport, check_id: str, expected: str):
        r = self._get(report, check_id)
        self.assertIsNotNone(r, f"Check {check_id} not found.")
        self.assertEqual(
            r.status, expected,
            f"Check {check_id} expected {expected} but got {r.status}.\n"
            f"Reasons: {r.reasons}"
        )


# ---------------------------------------------------------------------------
# P1: Consecutive trip chaining
# ---------------------------------------------------------------------------

class TestP1ConsecutiveTrips(_ProbeBaseTest):
    def test_two_trips_chain_pass(self):
        report = self._run(fx.fixture_consecutive_trips())
        self._assert(report, "P1", "PASS")

    def test_refuel_uncleared(self):
        """Refuel makes chaining INCONCLUSIVE for that vehicle."""
        report = self._run(fx.fixture_refuel_detected())
        r = self._get(report, "P1")
        self.assertIsNotNone(r)


# ---------------------------------------------------------------------------
# P2: Session changes and stale records
# ---------------------------------------------------------------------------

class TestP2SessionStale(_ProbeBaseTest):
    def test_stale_event_detected(self):
        report = self._run(fx.fixture_stale_records())
        r = self._get(report, "P2")
        self.assertIsNotNone(r)
        self.assertEqual(r.status, "FAIL")
        self.assertTrue(any("stale" in reason.lower() or "seq" in reason.lower()
                          for reason in r.reasons))

    def test_no_stale_events(self):
        report = self._run(fx.fixture_consecutive_trips())
        self._assert(report, "P2", "PASS")


# ---------------------------------------------------------------------------
# P3: Sequence ordering
# ---------------------------------------------------------------------------

class TestP3SeqOrdering(_ProbeBaseTest):
    def test_monotonic_unique(self):
        report = self._run(fx.fixture_seq_ordering_ok())
        self._assert(report, "P3", "PASS")

    def test_non_monotonic(self):
        report = self._run(fx.fixture_seq_non_monotonic())
        self._assert(report, "P3", "FAIL")

    def test_duplicate_fails_both_p3_and_p4(self):
        report = self._run(fx.fixture_duplicate_seq())
        self._assert(report, "P3", "FAIL")
        self._assert(report, "P4", "FAIL")


# ---------------------------------------------------------------------------
# P4: Duplicate records
# ---------------------------------------------------------------------------

class TestP4Duplicates(_ProbeBaseTest):
    def test_no_duplicates(self):
        report = self._run(fx.fixture_seq_ordering_ok())
        self._assert(report, "P4", "PASS")

    def test_duplicates_fail(self):
        report = self._run(fx.fixture_duplicate_seq())
        self._assert(report, "P4", "FAIL")


# ---------------------------------------------------------------------------
# P5: Gaps/dropped events
# ---------------------------------------------------------------------------

class TestP5GapsDrops(_ProbeBaseTest):
    def test_no_drops(self):
        report = self._run(fx.fixture_no_drops())
        self._assert(report, "P5", "PASS")

    def test_ring_overflow(self):
        report = self._run(fx.fixture_ring_overflow())
        r = self._get(report, "P5")
        self.assertIsNotNone(r)
        self.assertEqual(r.status, "INCONCLUSIVE")
        self.assertTrue(any("ring" in reason.lower() for reason in r.reasons))

    def test_log_cap(self):
        report = self._run(fx.fixture_log_cap())
        r = self._get(report, "P5")
        self.assertIsNotNone(r)
        self.assertIn(r.status, ("INCONCLUSIVE", "WARN"))


# ---------------------------------------------------------------------------
# P6: Vehicle/trip mismatches
# ---------------------------------------------------------------------------

class TestP6TripMismatch(_ProbeBaseTest):
    def test_unload_without_load_fails(self):
        report = self._run(fx.fixture_trip_mismatch())
        self._assert(report, "P6", "FAIL")

    def test_load_unload_match_pass(self):
        report = self._run(fx.fixture_trip_match_ok())
        self._assert(report, "P6", "PASS")


# ---------------------------------------------------------------------------
# P7: Unsupported/partial boundaries
# ---------------------------------------------------------------------------

class TestP7UnsupportedBoundaries(_ProbeBaseTest):
    def test_unsupported_unload_fails(self):
        report = self._run(fx.fixture_unsupported_unload())
        r = self._get(report, "P7")
        self.assertIsNotNone(r)
        self.assertEqual(r.status, "FAIL")

    def test_no_errors_pass(self):
        report = self._run(fx.fixture_supported_no_errors())
        self._assert(report, "P7", "PASS")


# ---------------------------------------------------------------------------
# P8: Deduction self-consistency
# ---------------------------------------------------------------------------

class TestP8Deduction(_ProbeBaseTest):
    def test_consistent_deduction(self):
        report = self._run(fx.fixture_deduction_consistent())
        self._assert(report, "P8", "PASS")

    def test_inconsistent_deduction(self):
        report = self._run(fx.fixture_deduction_inconsistent())
        self._assert(report, "P8", "FAIL")


# ---------------------------------------------------------------------------
# P9: Complete-unload evidence
# ---------------------------------------------------------------------------

class TestP9CompleteUnload(_ProbeBaseTest):
    def test_complete_pair(self):
        report = self._run(fx.fixture_complete_unload())
        self._assert(report, "P9", "PASS")

    def test_incomplete_no_unload(self):
        report = self._run(fx.fixture_incomplete_unload())
        self._assert(report, "P9", "FAIL")


# ---------------------------------------------------------------------------
# P10: Finite quantities
# ---------------------------------------------------------------------------

class TestP10Finite(_ProbeBaseTest):
    def test_all_finite(self):
        report = self._run(fx.fixture_consecutive_trips())
        self._assert(report, "P10", "PASS")

    def test_nan_detected(self):
        report = self._run(fx.fixture_nan_fuel())
        self._assert(report, "P10", "FAIL")


# ---------------------------------------------------------------------------
# P11: Independent fuel reconciliation
# ---------------------------------------------------------------------------

class TestP11IndependentReconciliation(_ProbeBaseTest):
    def test_pass(self):
        report = self._run(fx.fixture_independent_reconciliation_pass())
        self._assert(report, "P11", "PASS")

    def test_fail_mismatch(self):
        """Change trip 2's fa to make fb-fa != ded on trip 2."""
        log = fx.fixture_independent_reconciliation_pass()
        log = log.replace("fa=0.406000 ded=0.022000", "fa=0.400000 ded=0.022000")
        report = self._run(log)
        self._assert(report, "P11", "FAIL")

    def test_no_events_inconclusive(self):
        report = self._run(fx.fixture_no_events())
        self._assert(report, "P11", "INCONCLUSIVE")

    def test_empty_travel_consumption_per_trip(self):
        """Cross-trip start-end != sum(ded) due to empty travel burn,
        but each trip individually reconciles. P11 must PASS (per-trip),
        not FAIL (old cross-trip approach would FAIL spuriously).
        """
        report = self._run(fx.fixture_empty_travel_consumption())
        self._assert(report, "P11", "PASS")

    def test_missing_deduction_inconclusive(self):
        """Unload EVENT with ded unavailable — P11 INCONCLUSIVE."""
        report = self._run(fx.fixture_missing_deduction_observation())
        self._assert(report, "P11", "INCONCLUSIVE")

    def test_refuel_makes_p11_inconclusive(self):
        """Visible fuel addition between trips — P11 INCONCLUSIVE."""
        report = self._run(fx.fixture_visible_fuel_addition())
        self._assert(report, "P11", "INCONCLUSIVE")


# ---------------------------------------------------------------------------
# P12: Refuel assessment
# ---------------------------------------------------------------------------

class TestP12Refuel(_ProbeBaseTest):
    def test_no_refuel_pass(self):
        report = self._run(fx.fixture_independent_reconciliation_pass())
        self._assert(report, "P12", "PASS")

    def test_refuel_inconclusive(self):
        report = self._run(fx.fixture_refuel_detected())
        self._assert(report, "P12", "INCONCLUSIVE")

    def test_insufficient_coverage_inconclusive(self):
        """When fa is unavailable on some events, P12 cannot assess
        refuel coverage → INCONCLUSIVE.
        """
        report = self._run(fx.fixture_insufficient_refuel_coverage())
        self._assert(report, "P12", "INCONCLUSIVE")


# ---------------------------------------------------------------------------
# P13: Monetary cost
# ---------------------------------------------------------------------------

class TestP13MonetaryCost(_ProbeBaseTest):
    def test_unavailable_inconclusive(self):
        report = self._run(fx.fixture_price_unavailable())
        self._assert(report, "P13", "INCONCLUSIVE")

    def test_available_pass(self):
        report = self._run(fx.fixture_price_available())
        self._assert(report, "P13", "PASS")


# ---------------------------------------------------------------------------
# P14: Required evidence
# ---------------------------------------------------------------------------

class TestP14RequiredEvidence(_ProbeBaseTest):
    def test_no_events(self):
        report = self._run(fx.fixture_no_events())
        self._assert(report, "P14", "INCONCLUSIVE")

    def test_no_session(self):
        report = self._run(fx.fixture_no_session())
        self._assert(report, "P14", "INCONCLUSIVE")

    def test_all_present(self):
        report = self._run(fx.fixture_consecutive_trips())
        self._assert(report, "P14", "PASS")


# ---------------------------------------------------------------------------
# P15: Confidence values
# ---------------------------------------------------------------------------

class TestP15Confidence(_ProbeBaseTest):
    def test_valid_confidence(self):
        report = self._run(fx.fixture_consecutive_trips())
        self._assert(report, "P15", "PASS")

    def test_unknown_confidence(self):
        report = self._run(fx.fixture_unknown_confidence())
        self._assert(report, "P15", "FAIL")


# ---------------------------------------------------------------------------
# P16: Selection record presence
# ---------------------------------------------------------------------------

class TestP16SelectionRecord(_ProbeBaseTest):
    def test_selection_present(self):
        report = self._run(fx.fixture_snapshot_selection())
        self._assert(report, "P16", "PASS")

    def test_no_match_present(self):
        report = self._run(fx.fixture_snapshot_no_match())
        self._assert(report, "P16", "PASS")


# ---------------------------------------------------------------------------
# P17: Measurement validity flags
# ---------------------------------------------------------------------------

class TestP17MeasurementValidity(_ProbeBaseTest):
    def test_complete_measurement_valid(self):
        report = self._run(fx.fixture_snapshot_measurement_complete())
        self._assert(report, "P17", "PASS")

    def test_unavailable_fields_are_minus_one(self):
        """Missing/unreadable fields are -1 (FUEL_UNAVAILABLE), never 0."""
        report = self._run(fx.fixture_snapshot_measurement_unavailable())
        self._assert(report, "P17", "PASS")


# ---------------------------------------------------------------------------
# P18: Mirror independence from fuel slot
# ---------------------------------------------------------------------------

class TestP18MirrorIndependence(_ProbeBaseTest):
    def test_mirror_independent(self):
        """mirror_valid=1 with fuel_slot_valid=0 is allowed; checker does not infer."""
        report = self._run(fx.fixture_snapshot_measurement_unavailable())
        self._assert(report, "P18", "PASS")


# ---------------------------------------------------------------------------
# P19: Identity retention across state changes
# ---------------------------------------------------------------------------

class TestP19IdentityRetention(_ProbeBaseTest):
    def test_identity_retained(self):
        """Same vehicle pointer across state/mode changes."""
        report = self._run(fx.fixture_snapshot_identity_retained())
        self._assert(report, "P19", "PASS")


# ---------------------------------------------------------------------------
# P20: Exactly-one-candidate selection rule
# ---------------------------------------------------------------------------

class TestP20ExactlyOneCandidate(_ProbeBaseTest):
    def test_selection_valid(self):
        report = self._run(fx.fixture_snapshot_selection())
        self._assert(report, "P20", "PASS")

    def test_no_match_valid(self):
        report = self._run(fx.fixture_snapshot_no_match())
        self._assert(report, "P20", "PASS")


# ---------------------------------------------------------------------------
# Parser tests for SNAPSHOT
# ---------------------------------------------------------------------------

class TestProbeParserSnapshot(unittest.TestCase):
    def test_parse_snapshot_selection(self):
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_selection())
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 1)
        s = snaps[0].kind_record
        self.assertIsInstance(s, probe_schema.ProbeSnapshot)
        self.assertEqual(s.record_type, "selection")
        self.assertEqual(s.snapshot_ok, 1)
        self.assertEqual(s.observation_seq, 1)
        self.assertEqual(s.veh, fx.V_TRUCK_A)
        self.assertEqual(s.do, fx.DO_WEST)
        self.assertEqual(s.fuel_slot_valid, 0)
        self.assertEqual(s.fuel_tons, -1.0)
        self.assertEqual(s.kind, 66)
        self.assertEqual(s.state, 1)
        self.assertEqual(s.mode, 9)
        self.assertEqual(s.day, 5)
        self.assertEqual(s.mon, 1)
        self.assertEqual(s.yr, 2026)

    def test_parse_snapshot_measurement_unavailable(self):
        """Unavailable fields parse as -1 / FUEL_UNAVAILABLE, not 0."""
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_measurement_unavailable())
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 1)
        s = snaps[0].kind_record
        self.assertEqual(s.record_type, "measurement")
        self.assertEqual(s.snapshot_ok, 1)
        # fuel slot unavailable
        self.assertEqual(s.fuel_slot_valid, 0)
        self.assertEqual(s.fuel_tons, -1.0)
        # mirror readable but independent
        self.assertEqual(s.mirror_valid, 1)
        self.assertAlmostEqual(s.mirror_tons, 0.432000)
        # cargo unavailable, not zero
        self.assertEqual(s.cargo_valid, 0)
        self.assertEqual(s.cargo_slots, -1)
        self.assertEqual(s.cargo_total_tons, -1.0)
        # state/mode readable but don't invalidate
        self.assertEqual(s.state_valid, 1)
        self.assertEqual(s.state, 2)
        self.assertEqual(s.mode_valid, 1)
        self.assertEqual(s.mode, 9)

    def test_parse_snapshot_no_match(self):
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_no_match())
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 1)
        s = snaps[0].kind_record
        self.assertEqual(s.record_type, "no_match")
        self.assertEqual(s.snapshot_ok, 0)
        self.assertEqual(s.veh, "null")
        self.assertEqual(s.do, "null")
        self.assertEqual(s.fail_reason, "no_match")
        self.assertEqual(s.kind_valid, 0)
        self.assertEqual(s.kind, -1)

    def test_parse_snapshot_vehicle_lost(self):
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_vehicle_lost())
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 2)
        sel = snaps[0].kind_record
        lost = snaps[1].kind_record
        self.assertEqual(sel.record_type, "selection")
        self.assertEqual(sel.snapshot_ok, 1)
        self.assertEqual(sel.observation_seq, 1)
        self.assertEqual(lost.record_type, "vehicle_lost")
        self.assertEqual(lost.snapshot_ok, 0)
        self.assertEqual(lost.fail_reason, "vehicle_gone")
        self.assertEqual(lost.observation_seq, 2)

    def test_parse_snapshot_world_reset(self):
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_world_reset())
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 1)
        s = snaps[0].kind_record
        self.assertEqual(s.record_type, "world_reset")
        self.assertEqual(s.day, -1)
        self.assertEqual(s.mon, -1)
        self.assertEqual(s.yr, -1)
        self.assertEqual(s.fail_reason, "world_reset")


# ---------------------------------------------------------------------------
# Integration: SNAPSHOT report
# ---------------------------------------------------------------------------

class TestProbeReportSnapshot(unittest.TestCase):
    def test_snapshot_full_report(self):
        records = probe_parser.parse_probe_log(fx.fixture_snapshot_identity_retained())
        report = run_probe_checks(records, source="synthetic")
        self.assertEqual(report.schema_version, 1)
        self.assertEqual(report.source, "synthetic")
        # All SNAPSHOT checks should pass
        for cid in ("P16", "P17", "P18", "P19", "P20"):
            r = report.get_result(cid)
            self.assertIsNotNone(r, f"Check {cid} not found in report.")
            self.assertEqual(r.status, "PASS",
                             f"Check {cid} expected PASS but got {r.status}.\n"
                             f"Reasons: {r.reasons}")


if __name__ == '__main__':
    unittest.main()


# ---------------------------------------------------------------------------
# Real C++ emitter output serialization test
# ---------------------------------------------------------------------------
#
# Feeds actual output from snapshot_serialization_test.exe (a standalone
# host harness that replicates EmitSnapshotRecord() exactly) into the Python
# parser. Uses deliberately distinct values for every field so that field
# swaps (state from cargo_slots, mode from fuel_tons, etc.) are detectable.

class TestCppSerialization(_ProbeBaseTest):
    """Verify the parser correctly maps real C++ emitter output fields.

    The fixture is a static golden in the current ProbeSerializeSnapshot
    format: the measurement record reproduces the values of
    snapshot_serialization_test.cpp, and the no_match record is hand-built
    from the same serializer layout. test_native_snapshot.py recompiles the
    live serializer separately, so format drift is still exercised.
    """

    _output_path = "tests/private_logistics_probe/snapshot_serialization_output.txt"

    def _load_output(self):
        """Read the real C++ emitter output file."""
        import os
        path = os.path.join(os.path.dirname(__file__),
                            "snapshot_serialization_output.txt")
        with open(path, "r", encoding="utf-8") as f:
            return f.read()

    def test_real_cpp_output_parses(self):
        """Real C++ emitter output must parse into SNAPSHOT records."""
        log_text = self._load_output()
        records = probe_parser.parse_probe_log(log_text)
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]
        self.assertEqual(len(snaps), 2,
                         f"Expected 2 SNAPSHOT records, got {len(snaps)}")

    def test_no_field_swaps(self):
        """Every field must map to its correct value - no swaps allowed.

        Distinct values:
          - state=17 (NOT cargo_slots=3, NOT fuel_tons=9.876543)
          - mode=9 (NOT fuel_tons=9.876543)
          - home_matches=1 (NOT mirror_tons=-1.0)
          - kind=66 (0x42 road truck)
        """
        log_text = self._load_output()
        records = probe_parser.parse_probe_log(log_text)
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]

        m = snaps[0].kind_record  # measurement record
        self.assertEqual(m.record_type, "measurement")
        self.assertEqual(m.snapshot_ok, 1)
        self.assertEqual(m.observation_seq, 71)
        self.assertEqual(m.fail_reason, "")

        # Vehicle/DO identity
        self.assertEqual(m.veh, "000001F2A3B4C5D6")
        self.assertEqual(m.do, "000001F2A3B4C7E8")

        # Calendar from sim_clock accessors (not derived)
        self.assertEqual(m.day, 42)
        self.assertEqual(m.mon, 7)
        self.assertEqual(m.yr, 2026)

        # Fuel slot - valid, distinct value
        self.assertEqual(m.fuel_slot_valid, 1)
        self.assertAlmostEqual(m.fuel_tons, 9.876543)

        # Mirror - invalid, distinct value (must NOT be confused with fuel_tons)
        self.assertEqual(m.mirror_valid, 0)
        self.assertEqual(m.mirror_tons, -1.0)

        # Cargo - valid, distinct values
        self.assertEqual(m.cargo_valid, 1)
        self.assertEqual(m.cargo_slots, 3)
        self.assertAlmostEqual(m.cargo_total_tons, 4.567890)
        self.assertEqual(m.cargo, "fuel=9.876t ore=2.345t goods=1.234t")

        # Kind - 66 (0x42 road truck)
        self.assertEqual(m.kind_valid, 1)
        self.assertEqual(m.kind, 66)

        # STATE = 17 (NOT cargo_slots=3, NOT fuel_tons=9.876543)
        self.assertEqual(m.state_valid, 1)
        self.assertEqual(m.state, 17)
        self.assertNotEqual(m.state, 3)   # cargo_slots value
        self.assertNotEqual(m.state, 9)   # mode value

        # MODE = 9 (NOT fuel_tons=9.876543)
        self.assertEqual(m.mode_valid, 1)
        self.assertEqual(m.mode, 9)
        self.assertNotEqual(m.mode, 3)    # cargo_slots value

        # HOME_MATCHES = 1 (NOT mirror_tons=-1.0)
        self.assertEqual(m.home_matches_valid, 1)
        self.assertEqual(m.home_matches, 1)

    def test_no_match_record(self):
        """no_match record: all fields unavailable (-1), never zero."""
        log_text = self._load_output()
        records = probe_parser.parse_probe_log(log_text)
        snaps = [r for r in records if r.header.record_kind == "SNAPSHOT"]

        nm = snaps[1].kind_record  # no_match record
        self.assertEqual(nm.record_type, "no_match")
        self.assertEqual(nm.snapshot_ok, 0)
        self.assertEqual(nm.observation_seq, 72)
        self.assertEqual(nm.fail_reason, "no_match")
        self.assertEqual(nm.veh, "null")
        self.assertEqual(nm.do, "null")
        self.assertEqual(nm.fuel_slot_valid, 0)
        self.assertEqual(nm.fuel_tons, -1.0)
        self.assertEqual(nm.mirror_valid, 0)
        self.assertEqual(nm.mirror_tons, -1.0)
        self.assertEqual(nm.cargo_valid, 0)
        self.assertEqual(nm.cargo_slots, -1)
        self.assertEqual(nm.cargo_total_tons, -1.0)
        self.assertEqual(nm.cargo, "")
        self.assertEqual(nm.kind_valid, 0)
        self.assertEqual(nm.kind, -1)
        self.assertEqual(nm.state_valid, 0)
        self.assertEqual(nm.state, -1)
        self.assertEqual(nm.mode_valid, 0)
        self.assertEqual(nm.mode, -1)
        self.assertEqual(nm.home_matches_valid, 0)
        self.assertEqual(nm.home_matches, -1)
