"""Synthetic test suite for the private-logistics offline log checker.

Run with:
    python3 -m unittest tests.private_logistics_probe.test_checker -v

These tests consume ONLY synthetic fixtures (fixtures.py) — no game binary,
no runtime log capture, no network. They verify that the parser and checker
correctly process the frozen SCHEMA-1.0 log format.

Each test maps to a fixture (F1..F16) and asserts the expected PASS/FAIL/
INCONCLUSIVE verdict for one or more reconciliation checks (C1..C8).
"""

from __future__ import annotations

import math
import unittest
from typing import List, Optional, Tuple

from tests.private_logistics_probe import fixtures
from tests.private_logistics_probe import checker
from tests.private_logistics_probe import parser
from tests.private_logistics_probe.checker import run_checks, CheckReport, CheckResult


class _BaseCheckTestCase(unittest.TestCase):
    """Base helper: feeds a fixture through parser+checker and asserts check status."""

    fixture_func = None  # set per subclass
    expected_source = "synthetic"

    def _run(self, log_text: str) -> Tuple[List, CheckReport]:
        parsed = parser.parse_log(log_text)
        report = run_checks(parsed, source=self.expected_source)
        return parsed, report

    def _get(self, report: CheckReport, check_id: str) -> Optional[CheckResult]:
        return report.get_result(check_id)

    def _assert_status(self, report: CheckReport, check_id: str, expected: str):
        r = self._get(report, check_id)
        self.assertIsNotNone(r, f"Check {check_id} not found in report.")
        self.assertEqual(
            r.status, expected,
            f"Check {check_id} expected {expected} but got {r.status}.\n"
            f"Reasons: {r.reasons}"
        )


# ---------------------------------------------------------------------------
# C1 tests (no-refuel tank reconciliation)
# ---------------------------------------------------------------------------

class TestC1NormalTwoTrip(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C1", "PASS")

    def test_report_overall_uses_c1(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        c1 = self._get(report, "C1")
        self.assertIn("0.0440", c1.reasons[0])


class TestC1SingleReading(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_single_reading())
        self._assert_status(report, "C1", "INCONCLUSIVE")


class TestC1NoTankLines(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_no_tank_lines())
        self._assert_status(report, "C1", "INCONCLUSIVE")


class TestC1BurnMismatch(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_burn_mismatch())
        self._assert_status(report, "C1", "FAIL")


class TestC1OnlyRefuelVehicle(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_only_refuel_vehicle())
        self._assert_status(report, "C1", "INCONCLUSIVE")


class TestC1MultiVehicle(_BaseCheckTestCase):
    def test_truck_a_passes(self):
        _, report = self._run(fixtures.fixture_multi_vehicle())
        self._assert_status(report, "C1", "PASS")

    def test_truck_b_passes_c2(self):
        _, report = self._run(fixtures.fixture_multi_vehicle())
        self._assert_status(report, "C2", "PASS")


# ---------------------------------------------------------------------------
# C2 tests (refuel tank reconciliation)
# ---------------------------------------------------------------------------

class TestC2RefuelMidTrip(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_refuel_mid_trip())
        self._assert_status(report, "C2", "PASS")


class TestC2NoRefuelVehicles(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C2", "INCONCLUSIVE")


# ---------------------------------------------------------------------------
# C3 tests (duplicate B2B ship IDs)
# ---------------------------------------------------------------------------

class TestC3DuplicateShipIds(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_duplicate_ship_ids())
        self._assert_status(report, "C3", "FAIL")


class TestC3NoDuplicates(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C3", "PASS")


# ---------------------------------------------------------------------------
# C4 tests (day-sequence completeness)
# ---------------------------------------------------------------------------

class TestC4DayGap(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_day_gap())
        self._assert_status(report, "C4", "FAIL")


class TestC4ContiguousDays(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C4", "PASS")


# ---------------------------------------------------------------------------
# C5 tests (invalid numerics)
# ---------------------------------------------------------------------------

class TestC5NegativeBurn(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_negative_burn())
        self._assert_status(report, "C5", "FAIL")


class TestC5NaNFuel(_BaseCheckTestCase):
    def test_parser_handles_nan(self):
        parsed = parser.parse_log(fixtures.fixture_nan_fuel())
        # Find the tank reading record
        from tests.private_logistics_probe.parser import UnrecognizedLine
        from tests.private_logistics_probe.schema import TankReading
        tank_records = [
            pl.record for pl in parsed
            if isinstance(pl.record, TankReading)
        ]
        self.assertEqual(len(tank_records), 1)
        # NaN is a float in Python
        self.assertTrue(
            math.isnan(tank_records[0].fuel),
            f"Expected NaN fuel, got {tank_records[0].fuel}"
        )

    def test_checker_catches_nan(self):
        _, report = self._run(fixtures.fixture_nan_fuel())
        r = self._get(report, "C5")
        self.assertIsNotNone(r)
        # NaN should appear in reasons
        self.assertTrue(
            any("NaN" in reason or "nan" in str(reason).lower() or "Inf" in reason
                for reason in r.reasons),
            f"Expected NaN detection in C5 reasons, got: {r.reasons}"
        )
        # Status should be FAIL
        self.assertEqual(r.status, "FAIL")


class TestC5NormalValues(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C5", "PASS")


# ---------------------------------------------------------------------------
# C6 tests (dry-run / payment mode detection)
# ---------------------------------------------------------------------------

class TestC6LiveBilling(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C6", "PASS")


class TestC6DryRun(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_dry_run())
        self._assert_status(report, "C6", "INCONCLUSIVE")


class TestC6NoBanner(_BaseCheckTestCase):
    def test_inconclusive(self):
        log = f"{fixtures._timestamp(1,0,0)} [TANK] day 1   v={fixtures.V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0\n"
        _, report = self._run(log)
        self._assert_status(report, "C6", "INCONCLUSIVE")


# ---------------------------------------------------------------------------
# C7 tests (treasury movement consistency)
# ---------------------------------------------------------------------------

class TestC7LiveBillingMatch(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C7", "PASS")


class TestC7DryRun(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_dry_run())
        self._assert_status(report, "C7", "INCONCLUSIVE")


class TestC7TreasuryMismatch(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_treasury_mismatch())
        self._assert_status(report, "C7", "FAIL")


class TestC7BaselineOnly(_BaseCheckTestCase):
    def test_inconclusive(self):
        _, report = self._run(fixtures.fixture_baseline_only())
        self._assert_status(report, "C7", "INCONCLUSIVE")


# ---------------------------------------------------------------------------
# C8 tests (tank cross-check slot vs mirror)
# ---------------------------------------------------------------------------

class TestC8Agree(_BaseCheckTestCase):
    def test_pass(self):
        _, report = self._run(fixtures.fixture_normal_two_trip())
        self._assert_status(report, "C8", "PASS")


class TestC8MirrorDrift(_BaseCheckTestCase):
    def test_fail(self):
        _, report = self._run(fixtures.fixture_mirror_drift())
        self._assert_status(report, "C8", "FAIL")


# ---------------------------------------------------------------------------
# Parser tests
# ---------------------------------------------------------------------------

class TestParser(unittest.TestCase):
    def test_parse_banner(self):
        log = fixtures.banner_live()
        parsed = parser.parse_log(log)
        banners = [pl.record for pl in parsed
                   if hasattr(pl.record, 'version')]
        self.assertEqual(len(banners), 2)
        self.assertEqual(banners[1].version, "0.17")
        self.assertTrue(banners[1].billing)
        self.assertFalse(banners[1].dry_run)

    def test_parse_tank_reading(self):
        log = f"{fixtures._timestamp(1,0,0)} [TANK] day 1   v={fixtures.V_TRUCK_A} fuel 0.4300 t | burned 0.0220 t | refills 0\n"
        parsed = parser.parse_log(log)
        from tests.private_logistics_probe.schema import TankReading
        tanks = [pl.record for pl in parsed if isinstance(pl.record, TankReading)]
        self.assertEqual(len(tanks), 1)
        self.assertEqual(tanks[0].day, 1)
        self.assertEqual(tanks[0].vehicle_addr, fixtures.V_TRUCK_A)
        self.assertAlmostEqual(tanks[0].fuel, 0.4300)
        self.assertAlmostEqual(tanks[0].burned, 0.0220)
        self.assertEqual(tanks[0].refills, 0)

    def test_parse_payment_dry_run(self):
        log = f"{fixtures._timestamp(1,1,0)} [PAY] would freight DO West +23.03 (fuel 1.000ft) - dry_run\n"
        parsed = parser.parse_log(log)
        from tests.private_logistics_probe.schema import Payment
        pays = [pl.record for pl in parsed if isinstance(pl.record, Payment)]
        self.assertEqual(len(pays), 1)
        self.assertTrue(pays[0].dry_run)
        self.assertEqual(pays[0].kind, "freight")
        self.assertEqual(pays[0].building_name, "DO West")
        self.assertAlmostEqual(pays[0].amount, 23.03)
        self.assertEqual(pays[0].resource_name, "fuel")
        self.assertAlmostEqual(pays[0].tons, 1.000)

    def test_parse_b2b_transfer(self):
        log = f"{fixtures._timestamp(1,2,0)} [B2B] #1001 freight bld={fixtures.V_TRUCK_A} \"{fixtures.B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)\n"
        parsed = parser.parse_log(log)
        from tests.private_logistics_probe.schema import B2BTransfer
        b2bs = [pl.record for pl in parsed if isinstance(pl.record, B2BTransfer)]
        self.assertEqual(len(b2bs), 1)
        self.assertEqual(b2bs[0].ship_id, 1001)
        self.assertEqual(b2bs[0].kind, "freight")
        self.assertEqual(b2bs[0].building_name, fixtures.B_DO_WEST)
        self.assertEqual(b2bs[0].mode, 1)

    def test_unrecognized_lines_counted(self):
        log = (
            f"{fixtures._timestamp(0,0,0)} tesmioloader v1.0 starting...\n"
            f"{fixtures._timestamp(1,0,0)} [TANK] day 1   v={fixtures.V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0\n"
            f"{fixtures._timestamp(1,1,0)} some weird line that matches nothing\n"
        )
        parsed = parser.parse_log(log)
        from tests.private_logistics_probe.parser import UnrecognizedLine
        unrec = [pl for pl in parsed if isinstance(pl.record, UnrecognizedLine)]
        self.assertEqual(len(unrec), 2)


# ---------------------------------------------------------------------------
# Integration: full report from fixture
# ---------------------------------------------------------------------------

class TestReportIntegration(unittest.TestCase):
    def test_normal_two_trip_report(self):
        parsed = parser.parse_log(fixtures.fixture_normal_two_trip())
        report = run_checks(parsed, source="synthetic")
        self.assertEqual(report.source, "synthetic")
        self.assertEqual(report.schema_version, "private-logistics-runtime-log-v1.0")
        self.assertEqual(report.plugin_version, "0.17")
        self.assertTrue(report.billing)
        self.assertFalse(report.dry_run)

    def test_dry_run_report(self):
        parsed = parser.parse_log(fixtures.fixture_dry_run())
        report = run_checks(parsed, source="synthetic")
        self.assertEqual(report.plugin_version, "0.17")
        self.assertFalse(report.billing)
        self.assertTrue(report.dry_run)

    def test_synthetic_tag_present(self):
        report = run_checks(
            parser.parse_log(fixtures.fixture_normal_two_trip()),
            source="synthetic"
        )
        c1 = report.get_result("C1")
        # Synthetic source should appear in reasons
        self.assertTrue(any("synthetic" in r for r in c1.reasons))


if __name__ == '__main__':
    unittest.main()
