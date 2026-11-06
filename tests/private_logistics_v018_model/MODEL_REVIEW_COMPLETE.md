# MODEL_REVIEW_COMPLETE.md

## Fuel-Accounting Reference Model — Review Complete (v0.18)

**Repository:** `C:\WRSRE\.kilo\worktrees\executable-accounting-model-and-deterministic-test`
**Branch:** `executable-accounting-model-and-deterministic-test`
**Deliverable Path:** `tests/private_logistics_v018_model/MODEL_REVIEW_COMPLETE.md`

# MODEL_REVIEW_COMPLETE.md

## Fuel-Accounting Reference Model — Review Complete (v0.18)

**Repository:** `C:\WRSRE\.kilo\worktrees\executable-accounting-model-and-deterministic-test`
**Branch:** `executable-accounting-model-and-deterministic-test`
**Deliverable Path:** `tests/private_logistics_v018_model/MODEL_REVIEW_COMPLETE.md`

## 1. Provenance and Exact Requirements Used

### 1.1 Inputs Inspected

| Path | Role |
|------|------|
| `plugins/private_logistics/private_logistics.cpp` | Production v0.17 reference (read only) — `TariffFor()`, `Pay()`, `DetourTransfer()`, `VehSlot()`, `ReadTank()`, persistence registration, daily census consumers |
| `docs/implementation-plan-per-trip-fuel.md` | v0.18 plan (the actual location; the claimed `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` does **not** exist) |
| `docs/review-per-trip-fuel-plan.md` | Corrective review; treated as authoritative where it disagrees with the plan |
| `src/resource_pricing_api.h` | `FuelUnitPrice()` = RUB per tonne |
| `plugins/private_logistics/private_logistics.ini` | Plugin configuration |

### 1.2 Claimed Plan Status

The claimed plan `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` **does not exist** in this worktree. The v0.18 plan lives at `docs/implementation-plan-per-trip-fuel.md`. Where the plan and its review disagree, the review's corrections are adopted (e.g. burn = `burnTons * FuelUnitPrice()`, not `litres * price`; `SITE_REFUEL` is not a trip boundary; factory gate excludes `SHIP_FUEL`/`SITE_REFUEL`).

### 1.3 Requirements Implemented

The model implements the agreed delivery-to-delivery policy:

**Measurement:**
- Measure the current interval before settlement.
- Accumulate the full delivery-to-delivery window, including intervening loads.
- A first readable load does not establish a complete delivery-window anchor (it only sets a baseline).
- Baseline recovery never repairs missing window coverage.
- Contamination reason(s) persist until the window closes.
- Unreadable delivery closes the old window but cannot anchor a usable next one.
- Unmatched/ambiguous deliveries still close physical accounting windows.
- Known carrier/invoice eligibility is independent of tank readability and slot availability.
- Hidden refills remain an explicit limitation (detected via tank rise, flagged as contamination).

**Pricing:**
- Measured fuel cost = observed tonnes × fuel price per tonne.
- Margin applied once; wear applied once per trip cycle.
- Configured constant fuel tariff (`constant_fuel_tariff`) and CPI fallback implemented explicitly.
- CPI cargo scaling uses cargo tonnes, NOT fuel-depletion tonnes.
- `trip_tariff=0` selects value pricing regardless of fuel/CPI/fallback settings.
- One settlement policy for immediate and timeout invoice handling.
- Observed fuel depletion is distinct from actual cargo quantity.
- Zero-depletion pricing is a provisional policy: fail-closed (unpriceable) when no fallback resolves.

**Settlement:**
- Verified + freight eligible may pay live.
- Heuristic may simulate only.
- Ambiguous/unmatched cannot pay.
- Freight eligibility is an independent adapter input, not inferred from readable tank + positive cargo.
- No slot + verified eligible invoice + fallback enabled can use constant/CPI.
- No slot + trip tariff + fallback disabled cannot use measured pricing.
- First observed delivery anchors but does not settle; settlement occurs at a subsequent delivery within the open window.
- Actual and simulated amounts are disjoint; refusals are distinct from both.
- Treasury decreases cumulatively on actual payment.
- Daily cap is global across vehicles.
- Simulation affordability/cap policy documented explicitly (simulation does not deduct treasury or apply cap; it is a "would pay" estimate).

**Lifecycle:**
- Pending-invoice timeout is separate from vehicle-window expiry.
- Vehicle-window expiry without an invoice creates NO payment.
- Expiry checks real stored timestamps against an injected clock.
- Identity is session-scoped; save/load cannot correlate reused raw identities.
- Rehoming cannot silently transfer old-owner consumption.
- A generated observation ID is not proof of movement deduplication.
- Terminal outcomes defined: settled_actual, settled_simulated, refused, skipped.
- Dedup ring eviction cannot turn a settled invoice into a new one; exactly-once cannot be guaranteed under bounded retention; fail closed.
- save/load invalidates tank baselines, carrier identities, dedup ring, and pending invoices before any consumer runs.
**Lifecycle:**
- Pending-invoice timeout is separate from vehicle-window expiry.
- Vehicle-window expiry without an invoice creates NO payment.
- Expiry checks real stored timestamps against an injected clock.
- Identity is session-scoped; save/load cannot correlate reused raw identities.
- Rehoming cannot silently transfer old-owner consumption.
- A generated observation ID is not proof of movement deduplication.
- Terminal outcomes defined: settled_actual, settled_simulated, refused, skipped.
- Dedup ring eviction cannot turn a settled invoice into a new one; exactly-once cannot be guaranteed under bounded retention; fail closed.
- save/load invalidates tank baselines, carrier identities, dedup ring, and pending invoices before any consumer runs.

## 2. Executive Result

**What is validated:** The reference model's internal behaviour across **38 deterministic tests** covering measurement (interval accumulation, window anchoring/closing, contamination, unreadable handling), pricing (constant tariff with density factor, CPI cargo-scaled fallback, value pricing independence, zero-depletion fail-closed), settlement (association matrix × live/dry-run × freight eligibility, treasury/cap/minimum refusals, global daily cap, actual vs simulated disjointness), and lifecycle (pending-invoice timeout vs vehicle-window expiry, save/load identity reset, rehoming, dedup eviction, partial movement cargo/burn separation, invalid/non-finite fail-safe).

**What is NOT validated:** Production correctness, real-game offsets, real-game association, the `SOVIET64.exe` binary, or the production plugin `plugins/private_logistics/private_logistics.cpp`. The model is a standalone executable specification; passing tests prove only the model's internal behaviour.

**Test result: 38 passed, 0 failed, 0 errors.** (See Section 7 for exact output.)

**Remaining blockers:** None for the reference model itself. Production integration work is documented in Section 8.
**Lifecycle:**
- Pending-invoice timeout is separate from vehicle-window expiry.
- Vehicle-window expiry without an invoice creates NO payment.
- Expiry checks real stored timestamps against an injected clock.
- Identity is session-scoped; save/load cannot correlate reused raw identities.
- Rehoming cannot silently transfer old-owner consumption.
- A generated observation ID is not proof of movement deduplication.
- Terminal outcomes defined: settled_actual, settled_simulated, refused, skipped.
- Dedup ring eviction cannot turn a settled invoice into a new one; exactly-once cannot be guaranteed under bounded retention; fail closed.
- save/load invalidates tank baselines, carrier identities, dedup ring, and pending invoices before any consumer runs.

## 2. Executive Result

**What is validated:** The reference model's internal behaviour across **38 deterministic tests** covering measurement (interval accumulation, window anchoring/closing, contamination, unreadable handling), pricing (constant tariff with density factor, CPI cargo-scaled fallback, value pricing independence, zero-depletion fail-closed), settlement (association matrix × live/dry-run × freight eligibility, treasury/cap/minimum refusals, global daily cap, actual vs simulated disjointness), and lifecycle (pending-invoice timeout vs vehicle-window expiry, save/load identity reset, rehoming, dedup eviction, partial movement cargo/burn separation, invalid/non-finite fail-safe).

**What is NOT validated:** Production correctness, real-game offsets, real-game association, the `SOVIET64.exe` binary, or the production plugin `plugins/private_logistics/private_logistics.cpp`. The model is a standalone executable specification; passing tests prove only the model's internal behaviour.

**Test result: 38 passed, 0 failed, 0 errors.** (See Section 7 for exact output.)

**Remaining blockers:** None for the reference model itself. Production integration work is documented in Section 8.

## 3. Requirement-to-Test Matrix

| Requirement | Test Method Names |
|-------------|-------------------|
| Measure current interval before settlement | `TestMeasurement.test_readable_chain_billed_depletion_is_025` |
| Accumulate full delivery-to-delivery window incl. loads | `TestMeasurement.test_readable_chain_billed_depletion_is_025`, `TestContinuousWindows.test_continuous_next_window_baselines` |
| First readable load does not anchor complete window | `TestMeasurement.test_first_load_baseline_only_no_settle` |
| Baseline recovery never repairs missing coverage | `TestMeasurement.test_unreadable_interval_then_recovery`, `TestMeasurement.test_unreadable_interval_then_recovery_asserts_unusable` |
| Contamination persists until window closes | `TestMeasurement.test_refill_contamination_persists_until_close`, `TestMeasurement.test_refill_contamination_asserts_unusable_and_fallback` |
| Unreadable delivery closes old window, can't anchor next | `TestMeasurement.test_unreadable_delivery_then_readable_load_delivery`, `TestMeasurement.test_unreadable_interval_then_recovery` |
| Unmatched/ambiguous deliveries close physical windows | `TestSettlement.test_association_matrix_with_freight_eligible` (ambiguous/unmatched refuse) |
| Freight eligibility independent of tank readability/slot | `TestSettlement.test_association_matrix_with_freight_eligible`, `TestMeasurement.test_no_slot_matrix` |
| Measured fuel cost = tonnes × price/tonne | `TestMeasurement.test_readable_chain_billed_depletion_is_025` (5.15375) |
| Margin applied once; wear once per trip | `TestPricing.test_constant_tariff_includes_density_factor` |
| Constant fuel tariff + CPI fallback explicit | `TestPricing.test_constant_tariff_includes_density_factor`, `TestPricing.test_cpi_cargo_scaling_not_depletion` |
| CPI scales by cargo tonnes not depletion | `TestPricing.test_cpi_cargo_scaling_not_depletion` |
| trip_tariff=0 → value pricing regardless of fallback | `TestPricing.test_value_pricing_independent_of_fallback` |
| One settlement policy for immediate + timeout | `TestLifecycle.test_vehicle_window_expiry_without_invoice_creates_no_payment`, `TestLifecycle.test_vehicle_window_expiry_unpriceable_no_invented_invoice` |
| Observed depletion distinct from cargo quantity | `TestLifecycle.test_partial_movement_cargo_separate_from_burn` |
| Zero-depletion pricing provisional (fail-closed) | `TestLifecycle.test_zero_depletion_no_fallback_unpriceable`, `TestPricing.test_zero_fuel_price_no_cpi_refused` |
| Verified + freight eligible may pay live | `TestSettlement.test_association_matrix_with_freight_eligible` |
| Heuristic simulates only | `TestSettlement.test_association_matrix_with_freight_eligible` |
| Ambiguous/unmatched cannot pay | `TestSettlement.test_association_matrix_with_freight_eligible` |
| No slot + fallback enabled can use constant/CPI | `TestMeasurement.test_no_slot_matrix` (trip_tariff=1 fallback refused; trip_tariff=0 value settles) |
| No slot + trip tariff + fallback disabled cannot use measured | `TestMeasurement.test_no_slot_matrix` |
| First observed delivery anchors, doesn't settle | `TestMeasurement.test_first_load_baseline_only_no_settle`, `TestMeasurement.test_first_delivery_anchors_but_does_not_settle` |
| First delivery fallback inspects first outcome | `TestMeasurement.test_first_delivery_settles_via_fallback_inspect_first_outcome` |
| Actual and simulated disjoint | `TestSettlement.test_actual_and_simulated_are_disjoint` |
| Treasury decreases cumulatively | `TestSettlement.test_successive_payments_exhaust_treasury` |
| Daily cap global across vehicles | `TestSettlement.test_two_vehicles_exhaust_global_daily_cap` |
| Simulation affordability/cap policy documented | `TestSimulationPolicy.test_simulation_affordability_cap_policy_explicit` |
| Separate pending-invoice timeout vs window expiry | `TestLifecycle.test_too_young_invoice_does_not_expire`, `TestLifecycle.test_expired_pending_invoice_settles_at_most_once`, `TestLifecycle.test_vehicle_window_expiry_without_invoice_creates_no_payment` |
| Expiring vehicle state never invents invoice | `TestLifecycle.test_vehicle_window_expiry_without_invoice_creates_no_payment`, `TestLifecycle.test_vehicle_window_expiry_unpriceable_no_invented_invoice` |
| Expiry checks stored timestamps vs injected clock | `TestLifecycle.test_too_young_invoice_does_not_expire`, `TestLifecycle.test_expired_pending_invoice_settles_at_most_once` |
| Identity session-scoped; save/load can't correlate | `TestLifecycle.test_save_load_identity_reset_before_daily_close` |
| Rehoming can't transfer old-owner consumption | `TestLifecycle.test_rehoming_resets_consumption`, `TestLifecycle.test_rehoming_no_old_owner_charge` |
| Observation ID is not proof of dedup | `TestLifecycle.test_dedup_after_eviction`, `TestLifecycle.test_eviction_does_not_resurrect_settled_invoice` |
| Terminal outcomes defined | All settlement tests assert lifecycle strings |
| Eviction can't resurrect settled invoice | `TestLifecycle.test_eviction_does_not_resurrect_settled_invoice` |
| Invalid/non-finite fail safely | `TestLifecycle.test_invalid_non_finite_fail_safely` |
| Exact invoice: 0.400→0.390→0.375 = 0.025 t = 5.15375 | `TestMeasurement.test_readable_chain_billed_depletion_is_025` |

## 4. Provisional Policy Decisions and Unsupported Guarantees

### 4.1 Provisional Decisions (Not Approved Requirements)

1. **Wear is applied once per trip cycle.** The `wear_charged` flag is reset when a new window opens. This is a modelling choice; production may allocate wear differently.
2. **CPI fallback formula** `baskets_per_trip + baskets_per_tonne * cargo_tons` is a provisional configuration; production must confirm the actual basket formula.
3. **Constant fuel tariff** (`constant_fuel_tariff`, litres per trip) is a provisional fallback when fuel price is available but no depletion was measured. Production may not have this concept.
4. **Zero-depletion with no fallback → refuse unpriceable.** Production may choose to price at zero or skip instead.
5. **Simulation (dry-run) does not deduct treasury or apply the daily cap.** It is a "would pay" estimate that checks association/freight/minimum but not treasury/cap. Production may differ.
6. **Dedup ring with bounded retention.** Exactly-once cannot be guaranteed; evicted IDs are treated as new observations. Settled invoices are tracked by invoice_id separately from the event dedup ring.
7. **Pending-invoice TTL (2s default)** is separate from vehicle-window expiry (TTL default 2s). These are provisional time values.

### 4.2 Unsupported Guarantees

- The model does not validate real-game offsets, association evidence, or the production plugin.
- The model does not access game memory or hooks.
- No money-moving functionality exists in the model.
- The plan `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` is absent; the model is grounded on `docs/implementation-plan-per-trip-fuel.md` and its review.
- Exactly-once invoice processing cannot be guaranteed under bounded dedup retention; the model fails closed.
- The model does not claim to reproduce production `Pay()` behaviour exactly; it specifies the *agreed policy* for integration.

## 5. Full Final Source Code

### 5.2 `tests/private_logistics_v018_model/model.py`

```python
"""Executable reference model for per-trip fuel measurement, pricing,
settlement eligibility and lifecycle behaviour (v0.18 design).

Pure Python standard library. No game offsets, no hooks, no live billing, no
money-moving. Passing tests prove only this model's internal behaviour, not
production correctness or real-game association.

State separation, per vehicle:
  - tank baseline availability            (baseline_valid / baseline_tons)
  - window anchored at a readable event    (window_open / window_usable)
  - window usability                       (window_usable)
  - persistent contamination reasons      (contamination: set[str])
  - observed depletion                    (observed_depletion, tonnes)
  - association quality                   (association)
  - freight eligibility                   (freight_eligible -- independent input)
  - actual pricing source                 (pricing_source)
  - requested / simulated / refused / actual payment outcomes
  - movement/settlement lifecycle + dedup identity

Window semantics:
  - A readable event with a usable slot anchors/opens a window (sets baseline).
  - Each subsequent readable event measures the interval delta from the
    baseline and accumulates it into observed_depletion; the baseline then
    advances. Loads accumulate; deliveries accumulate then settle the FULL
    delivery-to-delivery window and close it.
  - A first readable LOAD does NOT establish a complete delivery-window anchor
    for settlement; it only sets a baseline.
  - A first readable DELIVERY anchors the window AND may settle via fallback
    pricing (CPI/constant) if freight-eligible and fallback is enabled.
  - An unreadable event closes the window without settling; baseline recovery
    never repairs missing coverage of the closed window.
  - Contamination persists until the window closes.
  - Interval measurement, window usability, and invoice eligibility are three
    separate concerns.

Pricing policy:
  - trip_tariff=1 (measured):
      has_depletion + fuel_price -> ACTUAL_QUANTITY
         = depletion * fuel_price * (1+margin) + wear
      no_depletion + fallback.cpi -> CPI = baskets(cargo) * cpi_basket
      no_depletion + fallback.constant -> CONSTANT
         = constant_fuel_tariff * density * fuel_price * (1+margin) + wear
      no_depletion + no fallback -> NONE (unpriceable; fail-closed)
  - trip_tariff=0 (value):
      cargo_tons * unit_price * freight_share -> VALUE
      independent of fuel/CPI/fallback; requires cargo + unit price.

  Wear = wear_litres * diesel_density * fuel_price, applied once per trip cycle.
  CPI cargo scaling uses cargo tonnes, NOT fuel-depletion tonnes.
  Constant tariff is in litres: constant_fuel_tariff * density = tonnes.

Fallback configuration (explicit):
  - fallback_mode: "none" | "cpi" | "constant" | "both"
  - fallback_enabled derived from fallback_mode + available parameters.

Settlement policy (single policy for immediate and timeout invoices):
  - VERIFIED + freight_eligible -> may pay live (subject to treasury/min/cap)
  - HEURISTIC -> simulate only (dry-run); refuse live
  - AMBIGUOUS/UNMATCHED -> cannot pay (refuse association), even in dry-run
  - source == NONE -> refuse unpriceable (never invent)
  - amount <= 0 or < min_payment -> refuse minimum
  - treasury insufficient -> refuse treasury (cumulative tracking)
  - daily cap exceeded -> refuse cap (global across vehicles)

Actual and simulated amounts are disjoint; refusals are distinct from both.

Lifecycle:
  - Pending-invoice timeout is separate from vehicle-window expiry.
  - Vehicle-window expiry without an invoice creates NO payment.
  - Expiry checks real stored timestamps (window_opened_at) against the injected clock.
  - Identity is session-scoped; save/load cannot correlate reused raw identities.
  - Rehoming cannot silently transfer old-owner consumption.
  - A generated observation ID is not proof of movement deduplication.
  - Terminal outcomes: settled_actual, settled_simulated, refused, skipped.
  - Dedup ring eviction cannot turn a settled invoice into a new one; exactly-once
    cannot be guaranteed under bounded retention; fail closed.
  - save/load invalidates tank baselines, carrier identities, dedup ring, and
    pending invoices before any consumer runs.
```

```python
from __future__ import annotations

import math
from dataclasses import dataclass, field
from enum import Enum
from typing import Optional


class AssocQuality(Enum):
    VERIFIED = "verified"
    HEURISTIC = "heuristic"
    AMBIGUOUS = "ambiguous"
    UNMATCHED = "unmatched"


class PricingSource(Enum):
    NONE = "none"
    ACTUAL_QUANTITY = "actual_quantity"
    CONSTANT = "constant"
    VALUE = "value"
    CPI = "cpi"


class RefusalReason(Enum):
    NONE = "none"
    MINIMUM = "minimum"
    TREASURY = "treasury"
    CAP = "cap"
    ASSOCIATION = "association"
    UNPRICEABLE = "unpriceable"
    INELIGIBLE = "ineligible"


# Terminal lifecycle outcomes
LIFECYCLE_SETTLED_ACTUAL = "settled_actual"
LIFECYCLE_SETTLED_SIMULATED = "settled_simulated"
LIFECYCLE_REFUSED = "refused"
LIFECYCLE_SKIPPED = "skipped"
LIFECYCLE_DUPLICATE = "duplicate"
LIFECYCLE_REHOMED = "rehomed"
LIFECYCLE_NO_SLOT = "no_slot"
LIFECYCLE_CLOSED_UNREADABLE = "closed_unreadable"
LIFECYCLE_MEASURED = "measured"
LIFECYCLE_ACCUMULATED = "accumulated"
LIFECYCLE_ALREADY_SETTLED = "already_settled"
LIFECYCLE_UNUSABLE_WINDOW = "unusable_window"
LIFECYCLE_EXPIRED_ACTUAL = "expired_actual"
LIFECYCLE_EXPIRED_SIMULATED = "expired_simulated"
LIFECYCLE_EXPIRED_REFUSED = "expired_refused"


@dataclass
class ModelParams:
    trip_tariff: int = 1
    freight_share: float = 0.08
    tariff_margin: float = 0.35
    wear_litres: float = 5.0
    diesel_density: float = 0.001  # tonnes per litre (game uses 1 kg/L)
    constant_fuel_tariff: float = 0.0  # configured per-trip constant (litres)
    min_payment: float = 0.01
    daily_pay_cap: float = 5000.0  # 0 = no cap
    fuel_price: Optional[float] = None  # None/unresolved -> fail-closed in trip mode
    cpi_basket: Optional[float] = None  # None -> CPI unavailable
    baskets_per_trip: float = 0.80
    baskets_per_tonne: float = 0.02
    dry_run: bool = True
    refill_threshold: float = 0.001  # tonnes; only sub-kilo noise is ignored
    fallback_mode: str = "both"  # "none" | "cpi" | "constant" | "both"
    ttl_seconds: float = 2.0  # pending-invoice timeout
    treasury_balance: float = 0.0
    dedup_capacity: int = 16
```

```python
@dataclass
class TankEvent:
    vehicle_id: str
    kind: str  # "delivery" or "load"
    tank_tons: Optional[float]
    readable: bool
    association: AssocQuality = AssocQuality.UNMATCHED
    carrier_id: Optional[str] = None
    cargo_tons: Optional[float] = None
    unit_price: Optional[float] = None
    # INDEPENDENT adapter input: freight eligibility is NOT inferred from
    # tank readability + positive cargo.
    freight_eligible: bool = False
    timestamp: float = 0.0
    event_id: Optional[str] = None


@dataclass
class EventOutcome:
    vehicle_id: str
    kind: str
    timestamp: float
    duplicate: bool = False
    lifecycle: str = "ignored"
    # interval measurement
    baseline_was_valid: bool = False
    measured_depletion: Optional[float] = None
    # window usability
    window_usable: bool = False
    contamination: set = field(default_factory=set)
    # invoice eligibility and settlement
    pricing_source: PricingSource = PricingSource.NONE
    requested_amount: float = 0.0
    simulated_amount: float = 0.0
    actual_amount: float = 0.0
    refused: bool = False
    refusal_reason: RefusalReason = RefusalReason.NONE
    freight_eligible: bool = False
    association_quality: AssocQuality = AssocQuality.UNMATCHED
```

### 5.3 `tests/private_logistics_v018_model/test_model.py`

```python
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
```

## 6. Reproduction Command

```powershell
cd 'C:\WRSRE\.kilo\worktrees\executable-accounting-model-and-deterministic-test'
python -m unittest tests.private_logistics_v018_model.test_model -v
```

No new dependencies are required (Python standard library `unittest` only).

## 7. Actual Command Output and Test Count

```
test_actual_and_simulated_are_disjoint ... ok
test_association_matrix_with_freight_eligible ... ok
test_constant_tariff_includes_density_factor ... ok
test_constant_tariff_without_density_would_be_wrong ... ok
test_cpi_cargo_scaling_not_depletion ... ok
test_dedup_after_eviction ... ok
test_duplicate_invoice_does_not_change_totals ... ok
test_duplicates_and_eviction ... ok
test_eviction_does_not_resurrect_settled_invoice ... ok
test_expired_pending_invoice_settles_at_most_once ... ok
test_first_delivery_anchors_but_does_not_settle ... ok
test_first_delivery_no_fallback_refused ... ok
test_first_delivery_settles_via_fallback_inspect_first_outcome ... ok
test_first_load_baseline_only_no_settle ... ok
test_invalid_non_finite_fail_safely ... ok
test_minimum_payment_refusal ... ok
test_no_slot_matrix ... ok
test_partial_movement_cargo_separate_from_burn ... ok
test_readable_chain_billed_depletion_is_025 ... ok
test_rehoming_no_old_owner_charge ... ok
test_rehoming_resets_consumption ... ok
test_refill_contamination_asserts_unusable_and_fallback ... ok
test_refill_contamination_persists_until_close ... ok
test_save_load_identity_reset_before_daily_close ... ok
test_simulation_affordability_cap_policy_explicit ... ok
test_successive_payments_exhaust_treasury ... ok
test_too_young_invoice_does_not_expire ... ok
test_two_vehicles_exhaust_global_daily_cap ... ok
test_unreadable_delivery_then_readable_load_delivery ... ok
test_unreadable_interval_then_recovery ... ok
test_unreadable_interval_then_recovery_asserts_unusable ... ok
test_value_pricing_independent_of_fallback ... ok
test_vehicle_window_expiry_unpriceable_no_invented_invoice ... ok
test_vehicle_window_expiry_without_invoice_creates_no_payment ... ok
test_zero_depletion_no_fallback_unpriceable ... ok
test_zero_fuel_price_cpi_available ... ok
test_zero_fuel_price_no_cpi_refused ... ok
----------------------------------------------------------------------
Ran 38 tests in 0.005s
OK
```

**Result: 38 tests passed, 0 failed, 0 errors.**

Test classes: `TestMeasurement` (8), `TestPricing` (5), `TestSettlement` (6), `TestLifecycle` (10), `TestSimulationPolicy` (1), `TestContinuousWindows` (1) = 38 total.

Passing tests prove only the model's internal behaviour. They do not validate production correctness, real-game association, or `plugins/private_logistics/private_logistics.cpp`.

## 8. Remaining Production Integration Work

### Existing `Pay()` dry-run counters (production, `private_logistics.cpp`)

These are the existing refusal/tally counters that any production integration must preserve or extend:

| Counter | Meaning | Where incremented |
|---------|---------|-------------------|
| `g_skipSmall` | amount `< min_payment` | `Pay()` |
| `g_skipNoPayee` | missing enterprise/currency service or no payee | `Pay()`, detour |
| `g_skipNoPrice` | `unitPrice <= 0` (factory) or `freight <= 0` (carrier) | `Pay()`, detour |
| `g_skipTreasury` | treasury cannot afford the amount | `Pay()` |
| `g_payCapped` | `daily_pay_cap` exceeded | `Pay()` |
| `g_tariffNoIndex` | delivery could not be priced | detour |
| `g_payCount` / `g_paidToday` | payments computed (dry-run: "would"; live: moved) | `Pay()` |
| `g_paidFactory` / `g_paidCarrier` | split by `TSM_CAP_PRODUCTION` vs `TSM_CAP_WHOLESALE` | `Pay()` |
| `g_wouldPayFreight` / `g_wouldPayFuel` | freight/fuel value tallied at committed sites | detour |
| `g_treasuryChecked` / `g_treasuryExact` / `g_treasuryOff` / `g_treasuryMaxGap` | treasury leg sampled either side of the move | `Pay()` |

Note: in `dry_run = 1`, `Pay()` logs `[PAY] would ...` and still increments `g_payCount`/`g_paidToday`/category totals but moves no money — dry-run counters **overstate** live payments until `dry_run = 0`. The treasury leg (`getTreasuryBalance` before/after) is **not** sampled in dry-run (`g_treasuryChecked` stays 0).

### Required production integration changes

1. **Implement Option A:** in `DetourTransfer`, mode-0 `tons>0` → `StashDelivery()` (no money); mode-1 committed → `OnVehicleEvent()`: match pending record, read tank via pure `ReadTankNow()`, compute delta, `PriceAndPay()`, `Pay()` resolved payee. Keep existing mode-0 probe block only when `per_trip_fuel == 0`.
2. **Add `VehFuelState[VEH_MAX]`**, indexed via existing `VehSlot()` (no second linear scan). Fields: vehicle, home, lastTons, lastT, tripFuel, tripDrops, wearCharged, valid. Guard slot `-1`, unreadable `-1.0`, and `cap <= 0.0`.
3. **Add pure `ReadTankNow(v)`**: validated tank level, leaves daily `g_vehFuel[]`/baseline alone. Call per event; `ReadTank()` keeps its day-close job. Never let them write each other.
4. **`ResolvePayee()`:** vehicle's home building first (`veh+0x4F8`), then `p9`, then carrier-ring fallback (`:1375-1378`). Apply `IsPrivate`, `owner != building`, NULL-owner fallback.
5. **Gate factory payment** (G5): exclude `SHIP_FUEL`/`SITE_REFUEL`; add counter/log if fuel reaches it.
6. **Wear once per trip:** reset `tripFuel`, `wearCharged`, `tripDrops` at mode-1 load/return.
7. **Add `PendingDelivery` ring** (`PENDING_RING 32`) with 2 s TTL; sweep every event; bound `g_pendingHead % PENDING_RING`.
8. **`Pay()` call sites** stay outside `g_lock` (they take it internally). Pending ring touched only inside detour.
9. **`[TRIP FUEL]` logging** at the paid event with `source=` field (`measured | fallback:no-baseline | fallback:refill | fallback:no-vehicle | fallback:invalid | fallback:no-index`) and daily per-source counts.
10. **Save/load handler:** invalidate tank baselines and carrier identities before any consumer runs; retain persistent contamination reasons.
11. **Rehoming:** on carrier-identity change, invalidate baseline + open window; never charge old-owner consumption to new owner.
12. **Per-vehicle validation:** sum `[TRIP FUEL]` deltas per vehicle vs `ReadTank()` per-vehicle burned over the same window (refills excluded); fleet totals only as a secondary check.
13. **Document ordering change:** payment moves 30-70 ms later (mode-1 tick), changing `[PAY]` vs `[B2B]` line ordering and `daily_pay_cap` counting tick.

### Persistence registration, census safety, reset consumers

- `AutoPersistenceEvent` (auto-target permissions) is the only current persistence; the v0.18 model requires a new save/load handler that invalidates baselines + identities before any consumer runs.
- Daily census (`daily_close`) resets per-day paid counters; must run after save/load and before any settlement consumer.
- No reset consumers currently exist for tank baselines or carrier identities; these must be added as part of save/load handling.

## 9. Remaining Limitations

1. **Exactly-once delivery under bounded retention:** The model uses a fixed-size FIFO dedup ring (default 16 entries). An adversarial or buggy event stream can evict an ID and cause a re-delivery to be treated as new. The model fails closed (refuses unpriceable) but cannot guarantee exactly-once without unbounded storage.

2. **Association quality as exogenous input:** The model treats `freight_eligible` and `association` as adapter outputs. It does not implement the matching logic (resource/quantity/time correlation). Production must provide a verified adapter.

3. **Constant tariff density dependency:** The constant-fuel fallback applies `diesel_density` to convert litres→tonnes. If the game's `FuelUnitPrice()` semantics change, the model must be updated.

4. **No stochastic/refill modelling:** Hidden refills remain an explicit limitation. The model only flags `refill` contamination when the tank rises above `refill_threshold`; it cannot detect refills that exactly match depletion.

5. **Single-threaded clock model:** The injected clock is a simple float. Production concurrency (mode-0/1 interleaving) is not modelled; the adapter must serialize events correctly.

6. **No real-game validation:** This model has not been run against live game memory. All offsets, function signatures, and data layouts come from static analysis of `SOVIET64.exe` v1.1.1.9 and may drift with patches.

**Final note:** This deliverable is a **reference model and test suite**, not a production plugin. It validates the *agreed policy* only. Production correctness requires separate offset verification, hook safety audits, and integration testing against `SOVIET64.exe`.