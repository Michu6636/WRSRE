"""Frozen log schema for the private-logistics runtime diagnostic.

SCHEMA VERSION: private-logistics-runtime-log-v1.0
FROZEN: 2026-10-24
SOURCE: plugins/private_logistics/private_logistics.cpp (v0.17, 3641 lines)
        src/tesmioloader.cpp (Logf timestamp prefix)

This schema is derived directly from the production plugin's Logf() call sites.
It is frozen: no production log line format is changed to satisfy a checker.
The schema maps each [TAG] line the plugin already emits to a structured record.

Agent B's proposed association-adapter schema (association-adapter-contract.md)
defines a per-call observer for a future reader that does not yet exist in the
binary. That schema is a proposal, not a runtime log format. This schema freezes
the ACTUAL production output so the checker parses what the plugin already writes.

A v0.18 plan proposes new [TRIP FUEL] lines, but those lines do not exist in the
shipping binary. If/when they ship, they will be SCHEMA-2.0 (additive only,
v1.0 fields remain unchanged).
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

SCHEMA_VERSION = "private-logistics-runtime-log-v1.0"
SCHEMA_FROZEN_DATE = "2026-10-24"
SCHEMA_SOURCE = "plugins/private_logistics/private_logistics.cpp v0.17"


# ---------------------------------------------------------------------------
# Reconciliation tolerances
# ---------------------------------------------------------------------------

# The game stores fuel in tonnes as a float (veh+0x5F0 mirror) or in the cargo
# slot as a float. The engine clamps the slot to zero. The ReadTank() reader
# uses 0.02 t as its refill detection threshold (private_logistics.cpp:3266:
# `level > prev + 0.02`). The tank is sampled once per in-game day at close.
#
# tick/boundary uncertainty:
#   - Burn is per-tick (FUN_14016E3B0). The day close happens at a fixed tick
#     within the day, but the exact sub-tick position varies, so a single day's
#       burn can shift by a fraction of a tick's consumption between runs.
#   - The 0.02 t refill threshold means sub-threshold noise is silently
#     classified as either burn or no-change.
#   - Float32 precision at ~0.4 t full scale gives ~1e-7 relative error.
#
# DEFAULT_TOLERANCE_T is set to 0.02 (the production refill threshold) so that
# single-day boundary noise does not cause a FAIL. For multi-day runs, a tight
# tolerance (0.005) can be specified explicitly.
DEFAULT_TOLERANCE_T = 0.02


# ---------------------------------------------------------------------------
# Record dataclasses
# ---------------------------------------------------------------------------

@dataclass
class TankReading:
    """Per-vehicle daily tank reading.

    Source: [TANK] day %d   v=%p fuel %.4f t | burned %.4f t | refills %u
    Location: private_logistics.cpp:3429
    """
    day: int
    vehicle_addr: str
    fuel: float           # current fuel level in tonnes
    burned: float         # burn since previous close, tonnes
    refills: int          # refill count since previous close
    raw: str = ""


@dataclass
class TankCrossCheck:
    """Per-vehicle slot-vs-mirror cross-check (secondary evidence).

    Source: [TANK] day %d   v=%p slot %.4f t | mirror +0x5F0 %.4f t | drift %+.4f t | %s
    Location: private_logistics.cpp:3278
    """
    day: int
    vehicle_addr: str
    slot_level: float     # authoritative level from cargo slot
    mirror_level: float   # +0x5F0 mirror value
    drift: float
    status: str           # "agree" or "MIRROR HAS DRIFTED"
    raw: str = ""


@dataclass
class TankRunTotal:
    """Run-level tank burn summary (fleet-wide).

    Source: [TANK] run: burned %.4f t = %.1f L | ...
    Location: private_logistics.cpp:1843
    """
    burned_tons: float
    burned_litres: float
    lp_trip: float
    deliveries: int
    raw: str = ""


@dataclass
class Payment:
    """Individual payment event.

    Source: [PAY] %s %s %+.2f (%s %.3ft) paid by the state
            [PAX] would %s %s %+.2f (%s %.3ft) - dry_run
    Location: private_logistics.cpp:1123, 1093
    """
    dry_run: bool
    kind: str             # "freight", "factory", "load", "unload", "fuel", "buy", "?"
    building_name: str
    amount: float
    resource_name: str
    tons: float
    raw: str = ""


@dataclass
class PaymentDay:
    """Daily payment summary.

    Source: [PAY] day %d closed: %u payment(s), %u capped | skipped: ...
    Location: private_logistics.cpp:1682
    """
    day: int
    payments: int
    capped: int
    skip_no_payee: int
    skip_no_price: int
    skip_too_small: int
    skip_treasury: int
    factory_total: float
    carrier_total: float
    live: bool            # True if "PAID by the state treasury", False if "DRY RUN"
    raw: str = ""


@dataclass
class BurnProbe:
    """Measured per-delivery burn probe (fleet-level).

    Source: [BURN] run: measured %.4f t = %.1f L between %u delivery pair(s) = ...
    Location: private_logistics.cpp:1872
    """
    measured_tons: float
    measured_litres: float
    delivery_pairs: int
    lp_trip: float
    skipped: int
    configured_lp_trip: float
    action: str
    raw: str = ""


@dataclass
class TreasuryBalance:
    """Treasury balance at day close (baseline or movement).

    Source: [BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid %.2f | ...
            [BAL] day %d closed: treasury %.2f (baseline ...) | paid %.2f
    Location: private_logistics.cpp:1736, 1732
    """
    day: int
    before: Optional[float]
    after: Optional[float]
    delta: Optional[float]
    plugin_paid: float
    is_baseline: bool
    raw: str = ""


@dataclass
class TariffDelivery:
    """Per-delivery tariff breakdown.

    Source: [TARIFF] %s %.3ft @%.2f = %.2f -> %s: %.2f = fuel %.2f x%.2f + wear %.2f (index %s) | charged %.2f by the state
    Location: private_logistics.cpp:1405
    """
    resource_name: str
    tons: float
    unit_price: float
    cargo_value: float
    building_name: str
    tariff: float
    fuel_cost: float
    multiplier: float
    wear_cost: float
    index: str             # "fuel", "cpi", "none"
    charged: float         # 0 if dry_run, else equals tariff
    raw: str = ""


@dataclass
class TariffDay:
    """Daily tariff summary.

    Source: [TARIFF] day %d closed: %u delivery(ies), %s %.2f = fuel %.2f x%.2f + wear %.2f, mean %.2f | ...
    Location: private_logistics.cpp:1696
    """
    day: int
    deliveries: int
    status: str            # "charged" or "model"
    total: float
    fuel_total: float
    multiplier: float
    wear_total: float
    mean: float
    indexed_fuel: int
    indexed_cpi: int
    unindexed: int
    raw: str = ""


@dataclass
class B2BTransfer:
    """Per-transfer attribution record.

    Source: [B2B] #%u %-6s bld=%p "%s" priv=%d res=%s %.2ft @%.2f = %.2f (site 0x%06X, mode %u)
    Location: private_logistics.cpp:1447
    """
    ship_id: int
    kind: str
    building_addr: str
    building_name: str
    private: int
    resource: str
    tons: float
    unit_price: float
    value: float
    site_rva: str
    mode: int
    raw: str = ""


@dataclass
class B2BDay:
    """Daily transfer summary.

    Source: [B2B] day %d closed: %u calls, %u committed, %u no-op | ...
    Location: private_logistics.cpp:1665
    """
    day: int
    calls: int
    committed: int
    noop: int
    unknown: int
    staging: int
    non_private: int
    raw: str = ""


@dataclass
class B2BSite:
    """Per-call site log record.

    Source: [B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u p2=%p "%s" priv=%d p9=%p vhome=%p class=%s
    Location: private_logistics.cpp:1318
    """
    call_site_rva: str
    mode: int
    sign: str
    tons: float
    committed: int
    flag: int
    p2: str
    building_name: str
    private: int
    p9: str
    vhome: str
    site_class: str
    raw: str = ""


@dataclass
class VehicleCensus:
    """Fleet census summary for one enterprise.

    Source: [VEH] day %d %s fleet: %u vehicle(s)
    Location: private_logistics.cpp:3356
    """
    day: int
    building_name: str
    vehicle_count: int
    raw: str = ""


@dataclass
class VehicleDetail:
    """Per-vehicle census detail.

    Source: [VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) kind=%d ...
    Location: private_logistics.cpp:3412
    """
    day: int
    vehicle_addr: str
    home: str
    cargo_tons: float
    slots: int
    kind: int
    state: int
    mode: int
    active: bool
    raw: str = ""


@dataclass
class VehicleCapacity:
    """Per-vehicle tank capacity (first sighting).

    Source: [VEH] day %d   v=%p tank capacity %.4f t (%.0f L at 1 kg/L)
    Location: private_logistics.cpp:3408
    """
    day: int
    vehicle_addr: str
    capacity_tons: float
    capacity_litres: float
    raw: str = ""


@dataclass
class FuelMeter:
    """Game's measured diesel bill (cat=2 foreign cost).

    Source: [FUEL] day %d   %s diesel %.2f (measured, cat=2) = %.2f L at %.2f/%s
    Location: private_logistics.cpp:1806
    """
    day: int
    building_name: str
    diesel_rub: float
    litres: float
    fuel_price: float
    currency: str
    raw: str = ""


@dataclass
class FuelIndex:
    """Daily fuel price / CPI index resolution.

    Source: [IDX] day %d closed: fuel '%s' = %.2f/%s (%s) | cpi basket %.2f/day | margin %.0f%%, wear %.2f L
    Location: private_logistics.cpp:1893
    """
    day: int
    fuel_resource: str
    fuel_price: float
    currency: str
    price_status: str
    cpi_basket: float
    margin_pct: float
    wear_litres: float
    raw: str = ""


@dataclass
class PluginBanner:
    """Plugin startup configuration banner.

    Source: private_logistics  vX.Y - billing ...
            private_logistics  live vX.Y: billing=N dry_run=N ...
    Location: private_logistics.cpp:3471, 3574
    """
    version: str
    billing: bool
    dry_run: bool
    live: bool
    raw: str = ""
