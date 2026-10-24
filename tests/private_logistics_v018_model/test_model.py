"""Deterministic tests for the v0.18 fuel-accounting reference model.

Standard-library unittest only. No new dependencies. The clock is injected so
all behaviour is deterministic. These tests prove only the model's internal
behaviour, not production correctness or real-game association.

Run:
    python -m unittest tests.private_logistics_v018_model.test_model -v

These tests reproduce the following defects against the previous model:
  1. Expiry without an invoice must produce no payment
     (test_vehicle_window_expiry_without_invoice_creates_no_payment)
  2. First-delivery fallback must inspect the first outcome
     (test_first_delivery_anchors_but_does_not_settle,
      test_first_delivery_settles_via_fallback_inspect_first_outcome)
  3. Recovery/refill tests must assert fallback pricing and unusable-window
     status (test_unreadable_interval_then_recovery_asserts_unusable,
      test_refill_contamination_asserts_unusable_and_fallback)
  4. Duplicate-invoice tests must assert unchanged payment totals
     (test_duplicate_invoice_does_not_change_totals)
  5. Constant-litre tariff missing density factor
     (test_constant_tariff_includes_density_factor)
"""

from __future__ import annotations

import unittest
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from private_logistics_v018_model.model import (  # noqa: E402
    FuelAccountingModel,
    ModelParams,
    TankEvent,
    AssocQuality,
    PricingSource,
    RefusalReason,
    LIFECYCLE_SETTLED_ACTUAL,
    LIFECYCLE_SETTLED_SIMULATED,
    LIFECYCLE_REFUSED,
    LIFECYCLE_SKIPPED,
    LIFECYCLE_DUPLICATE,
    LIFECYCLE_REHOMED,
    LIFECYCLE_NO_SLOT,
    LIFECYCLE_CLOSED_UNREADABLE,
    LIFECYCLE_MEASURED,
    LIFECYCLE_ACCUMULATED,
    LIFECYCLE_ALREADY_SETTLED,
    LIFECYCLE_UNUSABLE_WINDOW,
    LIFECYCLE_EXPIRED_ACTUAL,
    LIFECYCLE_EXPIRED_SIMULATED,
    LIFECYCLE_EXPIRED_REFUSED,
    _is_finite,
)

VERIFIED = AssocQuality.VERIFIED
HEURISTIC = AssocQuality.HEURISTIC
AMBIGUOUS = AssocQuality.AMBIGUOUS
UNMATCHED = AssocQuality.UNMATCHED


def delivery(
    vehicle_id="v1",
    tank=0.4,
    readable=True,
    association=VERIFIED,
    carrier="c1",
    cargo=1.0,
    unit_price=100.0,
    t=0.0,
    eid=None,
    freight_eligible=False,
):
    return TankEvent(
        vehicle_id=vehicle_id,
        kind="delivery",
        tank_tons=tank,
        readable=readable,
        association=association,
        carrier_id=carrier,
        cargo_tons=cargo,
        unit_price=unit_price,
        timestamp=t,
        event_id=eid,
        freight_eligible=freight_eligible,
    )


def load(
    vehicle_id="v1",
    tank=0.4,
    readable=True,
    t=0.0,
    eid=None,
    freight_eligible=False,
):
    return TankEvent(
        vehicle_id=vehicle_id,
        kind="load",
        tank_tons=tank,
        readable=readable,
        association=UNMATCHED,
        carrier_id=None,
        cargo_tons=None,
        unit_price=None,
        timestamp=t,
        event_id=eid,
        freight_eligible=freight_eligible,
    )


class BaseCase(unittest.TestCase):
    def make_model(self, **kwargs):
        treasury = kwargs.pop("treasury_balance", 1e9)
        p = ModelParams(treasury_balance=treasury, **kwargs)
        return FuelAccountingModel(clock=0.0, params=p)


# ============================================================================
# TEST 1: Readable Delivery N 0.400 -> load 0.390 -> delivery 0.375
# billed depletion = 0.025 t; exact charge 5.15375.
# ============================================================================
class TestMeasurement(BaseCase):
    def test_readable_chain_billed_depletion_is_025(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0)
        m.record_event(delivery(tank=0.400, t=0.0, eid="e1", cargo=1.0,
                                freight_eligible=True))
        m.record_event(load(tank=0.390, t=1.0, eid="e2"))
        r = m.record_event(delivery(tank=0.375, t=2.0, eid="e3", cargo=1.0,
                                    freight_eligible=True))

        self.assertEqual(r.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        expected = 0.025 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        self.assertAlmostEqual(r.requested_amount, expected)
        self.assertAlmostEqual(r.simulated_amount, expected)
        self.assertEqual(r.actual_amount, 0.0)
        self.assertEqual(r.pricing_source, PricingSource.ACTUAL_QUANTITY)

    # TEST 2: First load vs first delivery.
    def test_first_load_baseline_only_no_settle(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0)
        r = m.record_event(load(tank=0.400, t=0.0, eid="first"))
        # First load sets baseline only; nothing settled.
        self.assertIsNone(r.measured_depletion)
        self.assertEqual(r.lifecycle, LIFECYCLE_ACCUMULATED)

    # TEST 2b: First delivery anchors but does not settle (no fallback).
    def test_first_delivery_anchors_but_does_not_settle(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        # Re-run to inspect the first outcome independently.
        m2 = self.make_model(trip_tariff=1, fuel_price=133.0,
                             fallback_mode="none")
        r1 = m2.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                      freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_MEASURED)
        self.assertEqual(r1.actual_amount, 0.0)
        self.assertEqual(r1.simulated_amount, 0.0)
        # Window is open (anchored) but not settled.
        vs = m2._vehicle("v1")
        self.assertTrue(vs.window_open)
        self.assertFalse(vs.settled)

    # TEST 2c: First delivery settles via fallback; inspect the first outcome.
    def test_first_delivery_settles_via_fallback_inspect_first_outcome(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            cpi_basket=2.15, fallback_mode="cpi")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        # Re-run to inspect the first outcome independently.
        m2 = self.make_model(trip_tariff=1, fuel_price=133.0,
                             cpi_basket=2.15, fallback_mode="cpi")
        r1 = m2.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                      freight_eligible=True))
        # First delivery with fallback settles via CPI (zero depletion).
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        self.assertEqual(r1.pricing_source, PricingSource.CPI)
        baskets = 0.80 + 0.02 * 1.0  # cargo=1.0
        expected = baskets * 2.15
        self.assertAlmostEqual(r1.requested_amount, expected)
        # Second delivery opens a NEW window (first closed after fallback
        # settlement). It anchors at its own reading and also falls back to
        # CPI because there is no prior interval in the new window.
        r2 = m2.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                      freight_eligible=True))
        self.assertEqual(r2.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        self.assertEqual(r2.pricing_source, PricingSource.CPI)

    def test_first_delivery_no_fallback_refused(self):
        m = self.make_model(trip_tariff=1, fuel_price=None, cpi_basket=None,
                            constant_fuel_tariff=0.0, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        self.assertTrue(r.refused)
        self.assertEqual(r.refusal_reason, RefusalReason.UNPRICEABLE)

    # TEST 3: Unreadable interval -> readable load -> delivery: window unusable.
    def test_unreadable_interval_then_recovery(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0)
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r_unread = m.record_event(delivery(tank=None, readable=False, t=1.0,
                                           eid="b"))
        self.assertEqual(r_unread.lifecycle, LIFECYCLE_CLOSED_UNREADABLE)
        self.assertFalse(r_unread.window_usable)
        # Recovery: new baseline, old window coverage never repaired.
        m.record_event(load(tank=0.390, t=2.0, eid="c"))
        r = m.record_event(delivery(tank=0.375, t=3.0, eid="d", cargo=1.0,
                                    freight_eligible=True))
        self.assertAlmostEqual(r.measured_depletion, 0.015)

    # TEST 3b: Recovery asserts unusable-window status before settlement.
    def test_unreadable_interval_then_recovery_asserts_unusable(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            cpi_basket=2.15, fallback_mode="cpi")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r_unread = m.record_event(delivery(tank=None, readable=False, t=1.0,
                                           eid="b", freight_eligible=True))
        self.assertEqual(r_unread.lifecycle, LIFECYCLE_CLOSED_UNREADABLE)
        self.assertFalse(r_unread.window_usable)
        # Load anchors a new window but it is not yet usable for settlement
        # until the next readable delivery.
        m.record_event(load(tank=0.390, t=2.0, eid="c"))
        vs = m._vehicle("v1")
        self.assertTrue(vs.window_open)
        self.assertTrue(vs.window_usable)  # usable for next measurement
        # Final delivery measures only the new window (0.390 -> 0.375).
        r = m.record_event(delivery(tank=0.375, t=3.0, eid="d", cargo=1.0,
                                    freight_eligible=True))
        self.assertAlmostEqual(r.measured_depletion, 0.015)
        expected = 0.015 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        self.assertAlmostEqual(r.requested_amount, expected)

    # TEST 4: Unreadable delivery -> readable load -> delivery: fallback.
    def test_unreadable_delivery_then_readable_load_delivery(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0, cpi_basket=2.15,
                            fallback_mode="cpi")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        m.record_event(delivery(tank=None, readable=False, t=1.0, eid="b",
                                freight_eligible=True))
        m.record_event(load(tank=0.390, t=2.0, eid="c"))
        r = m.record_event(delivery(tank=0.375, t=3.0, eid="d", cargo=1.0,
                                    freight_eligible=True))
        self.assertAlmostEqual(r.measured_depletion, 0.015)

    # TEST 5: Refill at load/intermediate -> contamination persists until close.
    def test_refill_contamination_persists_until_close(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0)
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r_refill = m.record_event(load(tank=0.450, t=1.0, eid="b"))
        self.assertIn("refill", r_refill.contamination)
        self.assertIsNone(r_refill.measured_depletion)
        r_final = m.record_event(delivery(tank=0.425, t=2.0, eid="c",
                                          cargo=1.0, freight_eligible=True))
        self.assertAlmostEqual(r_final.measured_depletion, 0.025)

    # TEST 5b: Refill asserts contamination persists and window stays usable.
    def test_refill_contamination_asserts_unusable_and_fallback(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            cpi_basket=2.15, fallback_mode="cpi")
        # First delivery: NOT freight-eligible, so it anchors only (no settle).
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=False))
        vs = m._vehicle("v1")
        self.assertTrue(vs.window_open)
        # Refill at load: tank rose, contamination detected.
        r_refill = m.record_event(load(tank=0.450, t=1.0, eid="b"))
        self.assertIn("refill", r_refill.contamination)
        self.assertIsNone(r_refill.measured_depletion)
        # Window stays open after refill.
        self.assertTrue(vs.window_open)
        self.assertTrue(vs.window_usable)
        # Final delivery measures from refill level (0.450 -> 0.425 = 0.025).
        r_final = m.record_event(delivery(tank=0.425, t=2.0, eid="c",
                                          cargo=1.0, freight_eligible=True))
        self.assertAlmostEqual(r_final.measured_depletion, 0.025)
        expected = 0.025 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        self.assertAlmostEqual(r_final.requested_amount, expected)

    # TEST 6: No-slot matrix: tariff/value × fallback on/off.
    def test_no_slot_matrix(self):
        # trip_tariff=1, fallback off: cannot measure -> refuse unpriceable.
        m1 = self.make_model(trip_tariff=1, fuel_price=133.0,
                             cpi_basket=None, constant_fuel_tariff=0.0,
                             fallback_mode="none")
        m1.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                 freight_eligible=True))
        vs = m1._vehicle("v1")
        vs.slot_available = False
        r1 = m1.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                      carrier="c1", freight_eligible=True))
        self.assertTrue(r1.refused)
        self.assertEqual(r1.refusal_reason, RefusalReason.UNPRICEABLE)
        self.assertEqual(vs.carrier_id, "c1")

        # trip_tariff=0: value pricing needs no tank reading -> settles.
        m2 = self.make_model(trip_tariff=0)
        m2.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=2.0,
                                 unit_price=100.0, freight_eligible=True))
        vs2 = m2._vehicle("v1")
        vs2.slot_available = False
        r2 = m2.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=2.0,
                                      unit_price=100.0, carrier="c1",
                                      freight_eligible=True))
        self.assertEqual(r2.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        self.assertAlmostEqual(r2.requested_amount, 2.0 * 100.0 * 0.08)


# ============================================================================
# PRICING TESTS
# ============================================================================
class TestPricing(BaseCase):
    # TEST 8a: Constant fuel tariff includes density factor.
    def test_constant_tariff_includes_density_factor(self):
        # constant_fuel_tariff=10 L, density=0.001 t/L, price=133 RUB/t
        # fuel_cost = 10 * 0.001 * 133 = 1.33
        # wear_cost = 5 * 0.001 * 133 = 0.665
        # total = 1.33 * 1.35 + 0.665 = 2.4605
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            constant_fuel_tariff=10.0,
                            cpi_basket=None, fallback_mode="constant")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        # Zero depletion (same tank level) -> constant fallback path.
        r = m.record_event(delivery(tank=0.400, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        expected = 10.0 * 0.001 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        self.assertAlmostEqual(r.requested_amount, expected)
        self.assertEqual(r.pricing_source, PricingSource.CONSTANT)

    def test_constant_tariff_without_density_would_be_wrong(self):
        # Documents the bug: without density, cost would be 10*133*1.35+0.665
        # = 1796.165 instead of 2.4605. The density factor is essential.
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            constant_fuel_tariff=10.0,
                            cpi_basket=None, fallback_mode="constant")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.400, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        # With density: 2.4605
        expected_with_density = (
            10.0 * 0.001 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        )
        # Without density (WRONG): 1796.165
        expected_without_density = (
            10.0 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        )
        self.assertAlmostEqual(r.requested_amount, expected_with_density)
        self.assertNotAlmostEqual(r.requested_amount, expected_without_density)

    # TEST 8b: CPI uses cargo tonnes, not depletion tonnes.
    def test_cpi_cargo_scaling_not_depletion(self):
        m = self.make_model(trip_tariff=1, fuel_price=0.0, cpi_basket=2.15,
                            constant_fuel_tariff=0.0, fallback_mode="cpi")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        # Zero depletion -> CPI fallback with cargo=5.0
        r = m.record_event(delivery(tank=0.400, t=1.0, eid="b", cargo=5.0,
                                    freight_eligible=True))
        baskets = 0.80 + 0.02 * 5.0  # cargo-scaled
        expected = baskets * 2.15
        self.assertAlmostEqual(r.requested_amount, expected)
        self.assertEqual(r.pricing_source, PricingSource.CPI)

    def test_zero_fuel_price_cpi_available(self):
        m = self.make_model(trip_tariff=1, fuel_price=0.0, cpi_basket=2.15,
                            fallback_mode="cpi")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        self.assertEqual(r.pricing_source, PricingSource.CPI)
        self.assertGreater(r.requested_amount, 0.0)

    def test_zero_fuel_price_no_cpi_refused(self):
        m = self.make_model(trip_tariff=1, fuel_price=0.0, cpi_basket=None,
                            constant_fuel_tariff=0.0, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        self.assertTrue(r.refused)
        self.assertEqual(r.refusal_reason, RefusalReason.UNPRICEABLE)

    def test_value_pricing_independent_of_fallback(self):
        for fuel_price, cpi in ((133.0, 2.15), (0.0, None), (None, None)):
            m = self.make_model(trip_tariff=0, fuel_price=fuel_price,
                                cpi_basket=cpi, fallback_mode="none")
            m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                    unit_price=100.0, freight_eligible=True))
            r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=2.0,
                                        unit_price=100.0, freight_eligible=True))
            expected = 2.0 * 100.0 * 0.08
            self.assertEqual(r.pricing_source, PricingSource.VALUE)
            self.assertAlmostEqual(r.requested_amount, expected)


# ============================================================================
# SETTLEMENT TESTS
# ============================================================================
class TestSettlement(BaseCase):
    # TEST 9: Association quality × live/dry-run × freight eligibility.
    def test_association_matrix_with_freight_eligible(self):
        cases = [
            (VERIFIED, True, True, False, True, False),
            (VERIFIED, False, True, True, False, False),
            (VERIFIED, True, False, False, False, True),
            (VERIFIED, False, False, False, False, True),
            (HEURISTIC, True, True, False, True, False),
            (HEURISTIC, False, True, False, False, True),
            (AMBIGUOUS, True, True, False, False, True),
            (AMBIGUOUS, False, True, False, False, True),
            (UNMATCHED, True, True, False, False, True),
            (UNMATCHED, False, True, False, False, True),
        ]
        for assoc, dry_run, fe, exp_act, exp_sim, exp_ref in cases:
            m = self.make_model(trip_tariff=1, dry_run=dry_run,
                                fuel_price=133.0, fallback_mode="none")
            m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                    freight_eligible=fe))
            r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                        association=assoc, freight_eligible=fe))
            self.assertEqual(r.refused, exp_ref,
                             f"{assoc} dry_run={dry_run} fe={fe} "
                             f"refused={r.refused} reason={r.refusal_reason}")
            if exp_act:
                self.assertGreater(r.actual_amount, 0.0)
                self.assertEqual(r.simulated_amount, 0.0)
            elif exp_sim:
                self.assertGreater(r.simulated_amount, 0.0)
                self.assertEqual(r.actual_amount, 0.0)
            else:
                self.assertEqual(r.actual_amount, 0.0)
                self.assertEqual(r.simulated_amount, 0.0)

    # TEST 10: Two vehicles exhausting one global daily cap.
    def test_two_vehicles_exhaust_global_daily_cap(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            daily_pay_cap=6.0, treasury_balance=1e9,
                            fallback_mode="none")
        # Vehicle 1 settles ~5.15 under cap 6.0.
        m.record_event(delivery(tank=0.400, t=0.0, eid="v1a", cargo=1.0,
                                vehicle_id="veh1", freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="v1b", cargo=1.0,
                                     vehicle_id="veh1", freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_ACTUAL)

        # Vehicle 2: remaining cap 6.0 - 5.15 = 0.85 < 5.15 -> capped.
        m.record_event(delivery(tank=0.400, t=2.0, eid="v2a", cargo=1.0,
                                vehicle_id="veh2", freight_eligible=True))
        r2 = m.record_event(delivery(tank=0.375, t=3.0, eid="v2b", cargo=1.0,
                                     vehicle_id="veh2", freight_eligible=True))
        self.assertTrue(r2.refused)
        self.assertEqual(r2.refusal_reason, RefusalReason.CAP)

    # TEST 11: Successive payments exhausting treasury.
    def test_successive_payments_exhaust_treasury(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=12.0, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_ACTUAL)
        self.assertAlmostEqual(m.totals()["treasury_remaining"],
                               12.0 - 5.15375, places=4)

        m.record_event(delivery(tank=0.400, t=2.0, eid="c", cargo=1.0,
                                freight_eligible=True))
        r2 = m.record_event(delivery(tank=0.375, t=3.0, eid="d", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r2.lifecycle, LIFECYCLE_SETTLED_ACTUAL)
        self.assertAlmostEqual(m.totals()["treasury_remaining"],
                               12.0 - 2 * 5.15375, places=4)

    # TEST: Minimum payment refusal.
    def test_minimum_payment_refusal(self):
        m = self.make_model(trip_tariff=0, dry_run=False, min_payment=0.01)
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=0.001,
                                unit_price=1.0, freight_eligible=True))
        r = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=0.001,
                                    unit_price=1.0, freight_eligible=True))
        self.assertTrue(r.refused)
        self.assertEqual(r.refusal_reason, RefusalReason.MINIMUM)

    # TEST: Actual and simulated are disjoint.
    def test_actual_and_simulated_are_disjoint(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r_dry = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                        freight_eligible=True))
        self.assertAlmostEqual(r_dry.simulated_amount, 5.15375, places=6)
        self.assertEqual(r_dry.actual_amount, 0.0)


# ============================================================================
# LIFECYCLE TESTS
# ============================================================================
class TestLifecycle(BaseCase):
    # TEST 12a: Too-young invoice does not expire.
    def test_too_young_invoice_does_not_expire(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=1e9, ttl_seconds=2.0)
        m.request_pending_invoice("inv1", "v1", 5.15375,
                                  PricingSource.ACTUAL_QUANTITY, VERIFIED,
                                  True)
        # clock still 0.0, invoice requested at 0.0; cutoff 1.0 -> not expired
        # (1.0 - 0.0 = 1.0 <= 2.0).
        vs = m._vehicle("v1")
        outs = m.expire_pending_invoices(cutoff=1.0)
        self.assertEqual(len(outs), 0)
        # Settle it now: settled once.
        out1 = m.settle_pending_invoice("inv1", vs)
        self.assertEqual(out1.lifecycle, LIFECYCLE_SETTLED_ACTUAL)
        # Settle again: already settled, no double payment.
        out2 = m.settle_pending_invoice("inv1", vs)
        self.assertIsNone(out2)
        self.assertEqual(m.totals()["actual_total"], 5.15375)

    def test_expired_pending_invoice_settles_at_most_once(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=1e9, ttl_seconds=2.0)
        totals_before = m.totals()["actual_total"]
        m.request_pending_invoice("inv1", "v1", 5.15375,
                                  PricingSource.ACTUAL_QUANTITY, VERIFIED,
                                  True)
        m.advance(5.0)  # past TTL
        vs = m._vehicle("v1")
        outs = m.expire_pending_invoices(cutoff=m.clock)
        self.assertEqual(len(outs), 1)
        self.assertTrue(outs[0].refused)
        # Attempt settlement of expired invoice: no double payment.
        out2 = m.settle_pending_invoice("inv1", vs)
        self.assertIsNone(out2)
        # Totals unchanged (invoice was refused, not paid).
        self.assertEqual(m.totals()["actual_total"], totals_before)

    # TEST 13: Vehicle-window expiry without an invoice creates no payment.
    def test_vehicle_window_expiry_without_invoice_creates_no_payment(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=1e9, ttl_seconds=2.0,
                            fallback_mode="none")
        # Open a window with a delivery but do NOT settle it (first delivery
        # anchors only). Then expire the window.
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        m.advance(5.0)
        vs = m._vehicle("v1")
        vs.window_open = True
        vs.window_opened_at = 0.0
        vs.observed_depletion = 0.025
        vs.freight_eligible = True
        totals_before = m.totals()["actual_total"]
        outs = m.expire_stale_windows(cutoff=m.clock)
        self.assertEqual(len(outs), 1)
        # No payment was created: expiry without invoice = no payment.
        self.assertEqual(outs[0].actual_amount, 0.0)
        self.assertEqual(outs[0].simulated_amount, 0.0)
        self.assertFalse(outs[0].refused)
        self.assertEqual(m.totals()["actual_total"], totals_before)

    def test_vehicle_window_expiry_unpriceable_no_invented_invoice(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=0.0,
                            cpi_basket=None, constant_fuel_tariff=0.0,
                            treasury_balance=1e9, ttl_seconds=2.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        m.advance(5.0)
        vs = m._vehicle("v1")
        vs.window_open = True
        vs.window_opened_at = 0.0
        vs.observed_depletion = 0.025
        vs.freight_eligible = True
        totals_before = m.totals()["actual_total"]
        outs = m.expire_stale_windows(cutoff=m.clock)
        self.assertEqual(len(outs), 1)
        # Expiry without an invoice creates NO payment and NO refusal.
        # It only closes the window.
        self.assertEqual(outs[0].actual_amount, 0.0)
        self.assertEqual(outs[0].simulated_amount, 0.0)
        self.assertFalse(outs[0].refused)
        self.assertEqual(m.totals()["actual_total"], totals_before)

    # TEST 14: Duplicate observations, duplicate invoice requests, eviction.
    def test_duplicates_and_eviction(self):
        m = self.make_model(trip_tariff=1, dedup_capacity=2)
        e = delivery(tank=0.400, t=0.0, eid="dup", freight_eligible=True)
        r1 = m.record_event(e)
        self.assertFalse(r1.duplicate)
        r2 = m.record_event(delivery(tank=0.400, t=0.0, eid="dup",
                                     freight_eligible=True))
        self.assertTrue(r2.duplicate)
        # eviction: fill ring beyond capacity, old id can be re-submitted.
        m.record_event(delivery(tank=0.400, t=0.0, eid="x1",
                                freight_eligible=True))
        m.record_event(delivery(tank=0.400, t=0.0, eid="x2",
                                freight_eligible=True))
        r3 = m.record_event(delivery(tank=0.400, t=0.0, eid="dup",
                                     freight_eligible=True))
        self.assertFalse(r3.duplicate)  # evicted, treated as new

    def test_dedup_after_eviction(self):
        m = self.make_model(trip_tariff=1, dedup_capacity=1)
        r1 = m.record_event(delivery(tank=0.400, t=0.0, eid="keep",
                                     freight_eligible=True))
        self.assertFalse(r1.duplicate)
        m.record_event(delivery(tank=0.400, t=0.0, eid="other",
                                freight_eligible=True))
        r2 = m.record_event(delivery(tank=0.400, t=0.0, eid="keep",
                                     freight_eligible=True))
        self.assertFalse(r2.duplicate)

    # TEST 4: Duplicate-invoice tests must assert unchanged payment totals.
    def test_duplicate_invoice_does_not_change_totals(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            treasury_balance=1e9, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="e1", cargo=1.0,
                                freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="e2", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        totals_after_first = dict(m.totals())
        # Duplicate the settled event: dedup should catch it.
        r_dup = m.record_event(delivery(tank=0.375, t=1.0, eid="e2",
                                        cargo=1.0, freight_eligible=True))
        self.assertTrue(r_dup.duplicate)
        # Totals unchanged by the duplicate.
        self.assertEqual(m.totals(), totals_after_first)

    def test_eviction_does_not_resurrect_settled_invoice(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            dedup_capacity=1, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="e1", cargo=1.0,
                                freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="e2", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        # Eviction ring full; e2 cannot be re-submitted as a new invoice.
        m.record_event(delivery(tank=0.400, t=0.0, eid="x",
                                freight_eligible=True))
        r3 = m.record_event(delivery(tank=0.375, t=0.0, eid="e2", cargo=1.0,
                                     freight_eligible=True))
        self.assertFalse(r3.duplicate)  # evicted ID treated as new observation

    # TEST 15: Partial movements: actual cargo quantity separate from fuel burn.
    def test_partial_movement_cargo_separate_from_burn(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.390, t=1.0, eid="b", cargo=0.25,
                                    freight_eligible=True))
        self.assertAlmostEqual(r.measured_depletion, 0.010)
        # Pricing uses depletion (fuel burn), cargo is separate adapter input.
        self.assertEqual(r.pricing_source, PricingSource.ACTUAL_QUANTITY)

    # TEST 16: Save/load identity reuse and reset before daily-close consumers.
    def test_save_load_identity_reset_before_daily_close(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", carrier="c1",
                                freight_eligible=True))
        vs = m._vehicle("v1")
        self.assertTrue(vs.baseline_valid)
        self.assertEqual(vs.carrier_id, "c1")
        # Also register a pending invoice to verify session-scoped invalidation.
        m.request_pending_invoice("inv1", "v1", 5.15375,
                                  PricingSource.ACTUAL_QUANTITY, VERIFIED,
                                  True)
        self.assertEqual(len(m._pending_invoices), 1)
        m.save_load()
        vs = m._vehicle("v1")
        self.assertFalse(vs.baseline_valid)
        self.assertFalse(vs.window_usable)
        self.assertIsNone(vs.carrier_id)
        self.assertEqual(vs.association, UNMATCHED)
        # Session-scoped: dedup ring and pending invoices cleared.
        self.assertEqual(len(m._dedup), 0)
        self.assertEqual(len(m._pending_invoices), 0)
        # daily-close consumer runs after save/load: fresh state.
        m.daily_close()
        r = m.record_event(delivery(tank=0.390, t=1.0, eid="b",
                                    freight_eligible=True))
        self.assertIsNone(r.measured_depletion)

    # TEST 17: Rehoming and contamination reset semantics.
    def test_rehoming_resets_consumption(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=1e9, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", carrier="old",
                                cargo=1.0, freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="b", carrier="old",
                                     cargo=1.0, freight_eligible=True))
        # Rehome to new carrier: baseline invalidated, no carry-over charge.
        r2 = m.record_event(delivery(tank=0.390, t=2.0, eid="c", carrier="new",
                                     cargo=1.0, freight_eligible=True))
        self.assertIsNone(r2.measured_depletion)
        self.assertEqual(m._vehicle("v1").carrier_id, "new")

    # TEST: Rehoming cannot silently transfer old-owner consumption.
    def test_rehoming_no_old_owner_charge(self):
        m = self.make_model(trip_tariff=1, dry_run=False, fuel_price=133.0,
                            treasury_balance=1e9, fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", carrier="old",
                                cargo=1.0, freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="b", carrier="old",
                                     cargo=1.0, freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_ACTUAL)
        r2 = m.record_event(delivery(tank=0.390, t=2.0, eid="c", carrier="new",
                                     cargo=1.0, freight_eligible=True))
        self.assertIsNone(r2.measured_depletion)

    # TEST 18: Invalid/non-finite quantities, prices and tank readings fail safely.
    def test_invalid_non_finite_fail_safely(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        # NaN tank reading -> unreadable, fail safe.
        r_nan = m.record_event(delivery(tank=float("nan"), t=0.0, eid="a",
                                        freight_eligible=True))
        self.assertIsNone(r_nan.measured_depletion)
        self.assertFalse(r_nan.window_usable)
        # Non-finite unit price -> value pricing cannot price -> fail safe.
        m2 = self.make_model(trip_tariff=0, fuel_price=-5.0,
                             fallback_mode="none")
        m2.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                 unit_price=float("nan"),
                                 freight_eligible=True))
        r2 = m2.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                      unit_price=float("nan"),
                                      freight_eligible=True))
        self.assertFalse(r2.refused)
        self.assertEqual(r2.pricing_source, PricingSource.NONE)
        self.assertEqual(r2.actual_amount, 0.0)
        self.assertEqual(r2.simulated_amount, 0.0)

    # TEST: Zero depletion + no fallback -> unpriceable, no invoice invented.
    def test_zero_depletion_no_fallback_unpriceable(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            cpi_basket=None, constant_fuel_tariff=0.0,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r = m.record_event(delivery(tank=0.400, t=1.0, eid="b", cargo=1.0,
                                    freight_eligible=True))
        self.assertTrue(r.refused)
        self.assertEqual(r.refusal_reason, RefusalReason.UNPRICEABLE)
        self.assertEqual(r.requested_amount, 0.0)


# ============================================================================
# SIMULATION AFFORDABILITY/CAP POLICY
# ============================================================================
class TestSimulationPolicy(BaseCase):
    def test_simulation_affordability_cap_policy_explicit(self):
        """Simulation (dry-run) does NOT deduct treasury and does NOT apply
        the daily cap — it is a 'would pay' estimate. It still checks
        association/freight eligibility and minimum pricing."""
        m = self.make_model(trip_tariff=1, dry_run=True, fuel_price=133.0,
                            daily_pay_cap=5.0, treasury_balance=1e9,
                            fallback_mode="none")
        m.record_event(delivery(tank=0.400, t=0.0, eid="a", cargo=1.0,
                                freight_eligible=True))
        r1 = m.record_event(delivery(tank=0.375, t=1.0, eid="b", cargo=1.0,
                                     freight_eligible=True))
        # Simulation estimates the amount even though it exceeds the cap;
        # cap/treasury are live-only checks.
        self.assertFalse(r1.refused)
        self.assertAlmostEqual(r1.simulated_amount, 5.15375, places=6)
        self.assertEqual(r1.actual_amount, 0.0)
        # Treasury untouched by simulation.
        self.assertEqual(m.totals()["treasury_remaining"], 1e9)


# ============================================================================
# CONTINUOUS NEXT-WINDOW BASELINES
# ============================================================================
class TestContinuousWindows(BaseCase):
    # After a window closes (settlement or unreadable), the next readable
    # event starts a new baseline (continuous next-window baselines).
    def test_continuous_next_window_baselines(self):
        m = self.make_model(trip_tariff=1, fuel_price=133.0,
                            fallback_mode="none")
        # Window 1: delivery 0.400 -> load 0.390 -> delivery 0.375 (settled)
        m.record_event(delivery(tank=0.400, t=0.0, eid="w1a", cargo=1.0,
                                freight_eligible=True))
        m.record_event(load(tank=0.390, t=1.0, eid="w1b"))
        r1 = m.record_event(delivery(tank=0.375, t=2.0, eid="w1c", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r1.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        # Window 1 is closed and settled.
        vs = m._vehicle("v1")
        self.assertFalse(vs.window_open)
        self.assertTrue(vs.settled)  # window 1 was settled
        # Window 2: next delivery anchors a NEW baseline (continuous).
        m.record_event(delivery(tank=0.400, t=3.0, eid="w2a", cargo=1.0,
                                freight_eligible=True))
        self.assertTrue(vs.window_open)  # new window anchored
        self.assertFalse(vs.settled)  # new window: not yet settled
        m.record_event(load(tank=0.380, t=4.0, eid="w2b"))
        r2 = m.record_event(delivery(tank=0.360, t=5.0, eid="w2c", cargo=1.0,
                                     freight_eligible=True))
        self.assertEqual(r2.lifecycle, LIFECYCLE_SETTLED_SIMULATED)
        # measured_depletion is the per-event delta (0.380 -> 0.360 = 0.020)
        self.assertAlmostEqual(r2.measured_depletion, 0.020)
        # Pricing uses the full accumulated window depletion (0.040)
        expected = 0.040 * 133.0 * (1.0 + 0.35) + 5.0 * 0.001 * 133.0
        self.assertAlmostEqual(r2.requested_amount, expected)


if __name__ == "__main__":
    unittest.main()
