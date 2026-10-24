# Fuel-accounting reference model & deterministic tests (v0.18 design)

Location: `tests/private_logistics_v018_model/`

This directory holds a small **executable reference model** and a deterministic
test suite for the per-trip fuel measurement, pricing, settlement eligibility
and lifecycle behaviour described in the v0.18 design.

## Scope (what this is)

* A pure-Python (standard library only, `unittest`) reference model of the
  fuel-accounting state machine: tank baselines, measurement windows, pricing
  sources, settlement eligibility, refusals, dry-run vs live, save/load,
  rehoming and wall-clock expiry.
* Deterministic tests covering every minimum-test case in the task brief plus
  9 additional regression tests (25 tests total) across 7 test classes:
  `TestMeasurement`, `TestWindowAccumulation`, `TestPricing`, `TestSettlement`,
  `TestGlobalCap`, `TestTreasurySpending`, `TestLifecycle`.
* `findings.md`: remaining design defects and required production integration
  changes, including the current `Pay()` dry-run counters.
* `transition.md`: a proposed transition table and association-adapter input
  requirements.

## Scope (what this is NOT)

* It is **not** the production plugin. It does not read game memory, it does
  not hook `SOVIET64.exe`, and it does not move money.
* Passing tests prove only the **model's internal behaviour**. They do not
  validate production correctness, real-game association, or the offsets in
  `plugins/private_logistics/private_logistics.cpp`.
* No production source is modified by anything in this directory.

## Files

| File | Purpose |
|------|---------|
| `model.py` | Executable reference model (`FuelAccountingModel`) |
| `test_model.py` | Deterministic `unittest` suite |
| `findings.md` | Design defects + production integration changes |
| `transition.md` | Proposed transition table + adapter input requirements |

## Commands

Run the full suite (Python 3, no new dependencies):

```
cd 'C:\WRSRE\.kilo\worktrees\executable-accounting-model-and-deterministic-test'
python -m unittest tests.private_logistics_v018_model.test_model -v
```

Or run a single test class:

```
python -m unittest tests.private_logistics_v018_model.test_model.TestMeasurement -v
```

A summary line is printed on success; failures list the failing assertions.

## Assumptions (provisional, not approved requirements)

These are the explicit assumptions used to build the model. Where the design
is unresolved, `findings.md` records the alternatives.

1. **Claimed plan absent.** `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md`
   does not exist in this worktree. The model is instead grounded on
   `docs/implementation-plan-per-trip-fuel.md` (v0.18 plan), its review
   `docs/review-per-trip-fuel-plan.md`, `private_logistics.cpp` v0.17, and
   `private_logistics.ini`. Where the plan and the review disagree (the review
   is the later, corrective document), the review's corrections are adopted
   (e.g. burn = `burnTons * FuelUnitPrice()` not `litres * price`, `SITE_REFUEL`
   is not a trip boundary, factory gate excludes `SHIP_FUEL`/`SITE_REFUEL`).

2. **Verified association, actual quantity and carrier identity are inputs**
   from a future association adapter. The model treats them as event fields;
   it does not perform resource/quantity/time matching and does not invent
   verified identity.

3. **Pricing unit.** `FuelUnitPrice()` is RUB per TONNE (per
   `src/resource_pricing_api.h` and the plan review B3). Trip-tariff fuel cost
   is `burn_tons * fuel_price`, wear is `wear_litres * diesel_density *
   fuel_price`. `trip_tariff = 0` selects value pricing
   `cargo_tons * unit_price * freight_share`, independent of fuel/CPI/fallback.

4. **Fail-closed.** If neither a live fuel price nor a CPI basket resolves in
   trip-tariff mode, the delivery is unpriceable and refused (reason
   `unpriceable`) — no figure is invented.

5. **Association gating.** Verified → live payment (subject to
   treasury/minimum/cap). Heuristic → simulated (dry-run) only, **never**
   live. Ambiguous/unmatched → no payment at all (even in dry-run).

6. **Deterministic clock.** The model accepts an injected monotonic clock
   (a float seconds value). Tests advance it explicitly; there is no real
   wall clock and no `time.time()` in hot paths.

7. **Save/load.** Invalidates tank baselines and carrier identities before any
   consumer runs; persistent contamination reasons are retained.

8. **Rehoming.** A carrier-identity change invalidates the baseline and the
   open window so old-owner consumption is never charged to the new owner.
