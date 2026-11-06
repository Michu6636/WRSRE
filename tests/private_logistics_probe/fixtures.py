"""Synthetic log-line generators for the private-logistics checker tests.

Each generator produces raw log text in the EXACT format that the production
plugin writes to tesmioloader.log, including the Loader's timestamp prefix
[HH:MM:SS.mmm] and the [TAG] content.

These fixtures are the ONLY form of evidence the checker consumes in the
test suite — no game binary, no runtime log capture, no network.

All fixture data is self-consistent with the production physics:
  - Each daily [TANK] reading reports fuel at day-close (AFTER that day's burn)
  - burned = fuel consumed since previous day-close
  - For no-refuel vehicles: fuel[i-1] - fuel[i] = burned[i] (consecutive days)
  - For refuel vehicles: fuel[i] - fuel[i-1] + burned[i] = refill_amount

Reconciliation formula (C1/C2):
  sum(burned) ≈ (fuel_1 + burned_1) - fuel_N  [+ sum(refill_amounts)]
  where (fuel_1 + burned_1) = fuel at start of day 1 (before first burn)
"""

from __future__ import annotations


def _ts(minute: int, second: int, ms: int) -> str:
    """Generate [HH:MM:SS.mmm] timestamp."""
    return f"[00:{minute:02d}:{second:02d}.{ms:03d}]"


_timestamp = _ts  # backward-compat alias


# ---------------------------------------------------------------------------
# Vehicle addresses and constants used across fixtures
# ---------------------------------------------------------------------------

V_TRUCK_A = "000001F2A3B4C5D6"
V_TRUCK_B = "000001F2A3B4C5D7"
B_DO_WEST = "DO West"
B_FACTORY_N = "Factory North"
R_FUEL = "fuel"


# ---------------------------------------------------------------------------
# Banner helpers
# ---------------------------------------------------------------------------

def banner_live() -> str:
    """Production: plugin v0.17 in live billing mode."""
    lines = [
        f"{_ts(0,0,0)} private_logistics  v0.17 - billing LIVE (moves money)",
        f"{_ts(0,0,1)} private_logistics  live v0.17: billing=1 dry_run=0 factory=1 carrier=1, 16 sites, cap 5000/day, price = TRIP TARIFF, factory share 50",
    ]
    return "\n".join(lines) + "\n"


def banner_dryrun() -> str:
    """Production: plugin v0.17 in dry-run mode (billing=0 dry_run=1)."""
    lines = [
        f"{_ts(0,0,0)} private_logistics  v0.17 - billing LIVE (moves money)",
        f"{_ts(0,0,1)} private_logistics  live v0.17: billing=0 dry_run=1 factory=0 carrier=1, 16 sites, cap 5000/day, price = TRIP TARIFF, factory share 50",
    ]
    return "\n".join(lines) + "\n"


def banner_nobilling() -> str:
    """Production: plugin v0.17 with billing disabled entirely."""
    lines = [
        f"{_ts(0,0,0)} private_logistics  v0.17 - billing OFF (no money moves)",
        f"{_ts(0,0,1)} private_logistics  live v0.17: billing=0 dry_run=1 factory=0 carrier=0, 16 sites, cap 5000/day, price = TRIP TARIFF, factory share 50",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F1: Normal two-trip delivery, no refuel, live billing
#     Expected: C1 PASS, C3 PASS, C4 PASS, C5 PASS, C6 PASS, C7 PASS, C8 PASS
#
#     Physics:
#       Day 1: start_fuel=0.4720, burn=0.0220 → close_fuel=0.4500
#       Day 2: start_fuel=0.4500, burn=0.0220 → close_fuel=0.4280
#       sum(burned) = 0.0440 = (0.4500+0.0220) - 0.4280 = 0.0440 ✓
# ---------------------------------------------------------------------------

def fixture_normal_two_trip() -> str:
    lines = [
        banner_live(),
        # Day 1 setup
        f"{_ts(0,1,0)} [VEH] day 1   v={V_TRUCK_A} tank capacity 0.4720 t (472 L at 1 kg/L)",
        f"{_ts(0,1,1)} [VEH] day 1 \"{B_DO_WEST}\" fleet: 1 vehicle(s)",
        # Day 1 close: fuel=0.4500, burned=0.0220 (from start 0.4720)
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4500 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4500 t | mirror +0x5F0 0.4500 t | drift +0.0000 t | agree",
        # Day 1 tariff
        f"{_ts(1,1,0)} [TARIFF] fuel 1.000ft @6.90 = 6.90 -> {B_DO_WEST}: 23.03 = fuel 15.93 x1.35 + wear 7.10 (index fuel) | charged 23.03 by the state",
        # Day 1 payment
        f"{_ts(1,1,1)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        # Day 1 B2B
        f"{_ts(1,2,0)} [B2B] #1001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        # Day 1 burn run
        f"{_ts(1,3,0)} [BURN] run: measured 0.0440 t = 440.0 L between 2 delivery pair(s) = 220.0 L/trip | skipped 0 | configured 220.0 | apply",
        f"{_ts(1,3,1)} [TANK] run: burned 0.0440 t = 440.0 L | lp-trip 220.0 | pair 2 | deliveries 2 | fleet 0.0220/day x2 | truck 0.0220/day x2 | active 0.0220/day x1 | idle 0 | refills 0 | cfg 220.0",
        # Day 1 BAL
        f"{_ts(1,4,0)} [BAL] day 1 closed: treasury 10000.00 -> 9976.97 (-23.03) | this plugin paid 23.03 | the rest of the movement is the rest of the state's day",
        # Day 1 PAY day close
        f"{_ts(1,5,0)} [PAY] day 1 closed: 1 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 23.03 | PAID by the state treasury",
        # Day 1 TARIFF day close
        f"{_ts(1,5,1)} [TARIFF] day 1 closed: 2 delivery(ies), charged 46.06 = fuel 31.86 x1.35 + wear 14.20, mean 23.03 | indexed by fuel 2, by cpi 0, unindexed 0",
        # Day 1 FUEL meter
        f"{_ts(1,6,0)} [FUEL] day 1   \"{B_DO_WEST}\" diesel 150.00 (measured, cat=2) = 22000.0 L at 6.90/RUB",
        # Day 1 IDX
        f"{_ts(1,7,0)} [IDX] day 1 closed: fuel 'fuel' = 6.90/RUB (resolved) | cpi basket 0.00/day | margin 90%, wear 0.52 L",
        # Day 1 B2B day close
        f"{_ts(1,8,0)} [B2B] day 1 closed: 2 calls, 2 committed, 0 no-op | staging 0, non-private 0",
        # Day 1 VEH fleet
        f"{_ts(1,9,0)} [VEH] day 1 \"{B_DO_WEST}\" fleet: 1 vehicle(s)",
        # Day 2 close: fuel=0.4280, burned=0.0220 (from 0.4500 - 0.0220)
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.4280 t | burned 0.0220 t | refills 0",
        f"{_ts(2,0,1)} [TANK] day 2   v={V_TRUCK_A} slot 0.4280 t | mirror +0x5F0 0.4280 t | drift +0.0000 t | agree",
        f"{_ts(2,1,0)} [TARIFF] fuel 1.000ft @6.90 = 6.90 -> {B_DO_WEST}: 23.03 = fuel 15.93 x1.35 + wear 7.10 (index fuel) | charged 23.03 by the state",
        f"{_ts(2,1,1)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        f"{_ts(2,2,0)} [B2B] #1002 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(2,3,0)} [BURN] run: measured 0.0440 t = 440.0 L between 2 delivery pair(s) = 220.0 L/trip | skipped 0 | configured 220.0 | apply",
        f"{_ts(2,3,1)} [TANK] run: burned 0.0440 t = 440.0 L | lp-trip 220.0 | pair 2 | deliveries 2 | fleet 0.0220/day x2 | truck 0.0220/day x2 | active 0.0220/day x1 | idle 0 | refills 0 | cfg 220.0",
        f"{_ts(2,4,0)} [BAL] day 2 closed: treasury 9976.97 -> 9953.94 (-23.03) | this plugin paid 23.03 | the rest of the movement is the rest of the state's day",
        f"{_ts(2,5,0)} [PAY] day 2 closed: 1 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 23.03 | PAID by the state treasury",
        f"{_ts(2,5,1)} [TARIFF] day 2 closed: 2 delivery(ies), charged 46.06 = fuel 31.86 x1.35 + wear 14.20, mean 23.03 | indexed by fuel 2, by cpi 0, unindexed 0",
        f"{_ts(2,8,0)} [B2B] day 2 closed: 2 calls, 2 committed, 0 no-op | staging 0, non-private 0",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F2: Refuel during loaded leg, live billing
#     Expected: C1 INCONCLUSIVE (vehicle has refills), C2 PASS
#
#     Physics:
#       Day 1: start_fuel=0.4300, burn=0.0220 → close_fuel=0.4080 (no refuel)
#       Day 2: start_fuel=0.4080, burn=0.0150, refill=0.0370 → close_fuel=0.4300
#       refill_amt = 0.4300 - 0.4080 + 0.0150 = 0.0370
#       start_fuel = 0.4080 + 0.0220 = 0.4300
#       expected = (0.4300 - 0.4300) + 0.0370 = 0.0370
#       sum(burned) = 0.0220 + 0.0150 = 0.0370 ✓
# ---------------------------------------------------------------------------

def fixture_refuel_mid_trip() -> str:
    lines = [
        banner_live(),
        # Day 1: no refuel
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4080 t | mirror +0x5F0 0.4080 t | drift +0.0000 t | agree",
        # Day 1 payments
        f"{_ts(1,1,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        f"{_ts(1,2,0)} [B2B] #2001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(1,3,0)} [BURN] run: measured 0.0370 t = 370.0 L between 2 delivery pair(s) = 185.0 L/trip | skipped 0 | configured 220.0 | apply",
        f"{_ts(1,4,0)} [BAL] day 1 closed: treasury 10000.00 -> 9976.97 (-23.03) | this plugin paid 23.03 | the rest of the movement is the rest of the state's day",
        f"{_ts(1,5,0)} [PAY] day 1 closed: 1 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 23.03 | PAID by the state treasury",
        # Day 2: refuel happened (refills=1)
        # start_fuel = 0.4080 + 0.0220 = 0.4300
        # refill = 0.4300 - 0.4080 + 0.0150 = 0.0370
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.4300 t | burned 0.0150 t | refills 1",
        f"{_ts(2,0,1)} [TANK] day 2   v={V_TRUCK_A} slot 0.4300 t | mirror +0x5F0 0.4300 t | drift +0.0000 t | agree",
        f"{_ts(2,1,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        f"{_ts(2,2,0)} [B2B] #2002 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(2,3,0)} [BURN] run: measured 0.0370 t = 370.0 L between 2 delivery pair(s) = 185.0 L/trip | skipped 0 | configured 220.0 | apply",
        f"{_ts(2,4,0)} [BAL] day 2 closed: treasury 9976.97 -> 9953.94 (-23.03) | this plugin paid 23.03 | the rest of the movement is the rest of the state's day",
        f"{_ts(2,5,0)} [PAY] day 2 closed: 1 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 23.03 | PAID by the state treasury",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F3: Duplicate B2B ship IDs
#     Expected: C3 FAIL
# ---------------------------------------------------------------------------

def fixture_duplicate_ship_ids() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,1,0)} [B2B] #3001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(1,1,1)} [B2B] #3001 freight bld={V_TRUCK_B} \"{B_FACTORY_N}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(1,2,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F4: Day gap (missing day 2)
#     Expected: C4 FAIL
# ---------------------------------------------------------------------------

def fixture_day_gap() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4500 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4500 t | mirror +0x5F0 0.4500 t | drift +0.0000 t | agree",
        f"{_ts(3,0,0)} [TANK] day 3   v={V_TRUCK_A} fuel 0.4060 t | burned 0.0220 t | refills 0",
        f"{_ts(3,0,1)} [TANK] day 3   v={V_TRUCK_A} slot 0.4060 t | mirror +0x5F0 0.4060 t | drift +0.0000 t | agree",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F5: Wrong vehicle / burn mismatch
#     Expected: C1 FAIL
#
#     Physics:
#       Day 1: start=0.4520, burn=0.0220 → close=0.4300
#       Day 2: start=0.4300, burn reported 0.0200 → close SHOULD BE 0.4100
#       But fixture says fuel=0.3850 (drop of 0.0450, not 0.0200)
#       sum(burned) = 0.0420, start-end = 0.4520-0.3850 = 0.0670
#       delta = 0.0250 > 0.02 → FAIL
# ---------------------------------------------------------------------------

def fixture_burn_mismatch() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4300 t | burned 0.0220 t | refills 0",
        # Day 2: burned 0.0200 but fuel drops by 0.0450 (inconsistent)
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.3850 t | burned 0.0200 t | refills 0",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F6: Invalid numerics — negative burn
#     Expected: C5 FAIL
# ---------------------------------------------------------------------------

def fixture_negative_burn() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned -0.0200 t | refills 0",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F7: Single reading (stale/reset)
#     Expected: C1 INCONCLUSIVE (need >=2 readings)
# ---------------------------------------------------------------------------

def fixture_single_reading() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4300 t | burned 0.0220 t | refills 0",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F8: Missing tank readings entirely
#     Expected: C1 INCONCLUSIVE
# ---------------------------------------------------------------------------

def fixture_no_tank_lines() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,1,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        f"{_ts(1,2,0)} [B2B] #4001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F9: Dry-run mode (payments show "would ... - dry_run")
#     Expected: C6 INCONCLUSIVE, C7 INCONCLUSIVE
# ---------------------------------------------------------------------------

def fixture_dry_run() -> str:
    lines = [
        banner_dryrun(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4080 t | mirror +0x5F0 0.4080 t | drift +0.0000 t | agree",
        f"{_ts(1,1,0)} [PAY] would freight {B_DO_WEST} +23.03 (fuel 1.000ft) - dry_run",
        f"{_ts(1,2,0)} [B2B] #5001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(1,3,0)} [BURN] run: measured 0.0220 t = 220.0 L between 1 delivery pair(s) = 220.0 L/trip | skipped 0 | configured 220.0 | apply",
        f"{_ts(1,4,0)} [BAL] day 1 closed: treasury 10000.00 (baseline initial) | paid 0.00",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F10: Partial unload — burner independent of cargo tons
#     Expected: C1 PASS (burn reconciles regardless of cargo tons)
#
#     Physics:
#       Day 1: start=0.4500, burn=0.0220 → close=0.4280
#       Day 2: start=0.4280, burn=0.0220 → close=0.4060
#       sum(burned) = 0.0440 = 0.4500 - 0.4060 ✓
# ---------------------------------------------------------------------------

def fixture_partial_unload() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4280 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4280 t | mirror +0x5F0 0.4280 t | drift +0.0000 t | agree",
        # Full load delivery (1.000ft)
        f"{_ts(1,2,0)} [B2B] #6001 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 1.00ft @6.90 = 6.90 (site 0x168EECA, mode 1)",
        f"{_ts(1,3,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        # Day 2: same burn, but partial cargo (0.500ft)
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.4060 t | burned 0.0220 t | refills 0",
        f"{_ts(2,0,1)} [TANK] day 2   v={V_TRUCK_A} slot 0.4060 t | mirror +0x5F0 0.4060 t | drift +0.0000 t | agree",
        f"{_ts(2,2,0)} [B2B] #6002 freight bld={V_TRUCK_A} \"{B_DO_WEST}\" priv=1 res=fuel 0.50ft @6.90 = 3.45 (site 0x168EECA, mode 1)",
        f"{_ts(2,3,0)} [PAY] freight {B_DO_WEST} +11.51 (fuel 0.500ft) paid by the state",
        # BAL: total payments = 23.03 + 11.51 = 34.54
        f"{_ts(2,4,0)} [BAL] day 1 closed: treasury 10000.00 -> 9965.46 (-34.54) | this plugin paid 34.54 | the rest of the movement is the rest of the state's day",
        f"{_ts(2,5,0)} [PAY] day 2 closed: 2 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 34.54 | PAID by the state treasury",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F11: NaN fuel value
#     Expected: C5 FAIL (NaN detected)
# ---------------------------------------------------------------------------

def fixture_nan_fuel() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel nan t | burned 0.0220 t | refills 0",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F12: Treasury mismatch (BAL says paid 100 but PAY sums to 23.03)
#     Expected: C7 FAIL
# ---------------------------------------------------------------------------

def fixture_treasury_mismatch() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,1,0)} [PAY] freight {B_DO_WEST} +23.03 (fuel 1.000ft) paid by the state",
        f"{_ts(1,2,0)} [BAL] day 1 closed: treasury 10000.00 -> 9900.00 (-100.00) | this plugin paid 100.00 | the rest of the movement is the rest of the state's day",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F13: Mirror drift
#     Expected: C8 FAIL
# ---------------------------------------------------------------------------

def fixture_mirror_drift() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4500 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4280 t | mirror +0x5F0 0.4300 t | drift +0.0020 t | MIRROR HAS DRIFTED",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F14: Only refuel vehicle (no pure no-refuel case)
#     Expected: C1 INCONCLUSIVE (all vehicles have refills)
# ---------------------------------------------------------------------------

def fixture_only_refuel_vehicle() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_A} slot 0.4080 t | mirror +0x5F0 0.4080 t | drift +0.0000 t | agree",
        # Day 2: this vehicle refueled
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.4300 t | burned 0.0150 t | refills 1",
        f"{_ts(2,0,1)} [TANK] day 2   v={V_TRUCK_A} slot 0.4300 t | mirror +0x5F0 0.4300 t | drift +0.0000 t | agree",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F15: Multi-vehicle, one with no-refuel, one with refuel
#     Expected: C1 PASS for truck A, C2 PASS for truck B
#
#     Truck A (no refuel):
#       Day 1: start=0.4700, burn=0.0200 → close=0.4500
#       Day 2: start=0.4500, burn=0.0200 → close=0.4300
#       sum(burned) = 0.0400 = (0.4500+0.0200) - 0.4300 = 0.0400 ✓
#
#     Truck B (refuel on day 2):
#       Day 1: start=0.4500, burn=0.0200 → close=0.4300
#       Day 2: start=0.4300, burn=0.0150, refill=0.0350 → close=0.4500
#       refill = 0.4500 - 0.4300 + 0.0150 = 0.0350
#       start_fuel = 0.4300 + 0.0200 = 0.4500
#       expected = (0.4500 - 0.4500) + 0.0350 = 0.0350
#       sum(burned) = 0.0350 ✓
# ---------------------------------------------------------------------------

def fixture_multi_vehicle() -> str:
    lines = [
        banner_live(),
        # Truck A: no refuel
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4500 t | burned 0.0200 t | refills 0",
        f"{_ts(2,0,0)} [TANK] day 2   v={V_TRUCK_A} fuel 0.4300 t | burned 0.0200 t | refills 0",
        # Truck B: refuel on day 2
        f"{_ts(1,0,1)} [TANK] day 1   v={V_TRUCK_B} fuel 0.4300 t | burned 0.0200 t | refills 0",
        f"{_ts(2,0,1)} [TANK] day 2   v={V_TRUCK_B} fuel 0.4500 t | burned 0.0150 t | refills 1",
    ]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# F16: Only baseline BAL (no movement)
#     Expected: C7 INCONCLUSIVE
# ---------------------------------------------------------------------------

def fixture_baseline_only() -> str:
    lines = [
        banner_live(),
        f"{_ts(1,0,0)} [TANK] day 1   v={V_TRUCK_A} fuel 0.4080 t | burned 0.0220 t | refills 0",
        f"{_ts(1,1,0)} [BAL] day 1 closed: treasury 10000.00 (baseline initial) | paid 0.00",
    ]
    return "\n".join(lines) + "\n"
