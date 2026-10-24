"""Log parser for the private-logistics runtime diagnostic.

Parses lines from tesmioloader.log (the shared log that the Loader writes to
via Logf) into structured records defined by schema.py.

Each record carries a `tag` attribute so checker.py can dispatch by tag
without isinstance checks.

Log line format
---------------
Every line from tesmioloader is prefixed by the Loader's timestamp:
  [HH:MM:SS.mmm] <message>

This parser strips the timestamp prefix and routes <message> to the
matching record parser based on the [TAG] it starts with.

Lines that do not match any known tag are returned as UnrecognizedLine
records, not silently dropped. The checker uses these to report unknown
content rather than assuming silence means success.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Iterator, List, Optional, Union

from tests.private_logistics_probe import schema
from tests.private_logistics_probe.schema import (
    DEFAULT_TOLERANCE_T,
    SCHEMA_VERSION,
)
from tests.private_logistics_probe.schema import (
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
# Regex patterns  — frozen to SCHEMA_VERSION v1.0
# ---------------------------------------------------------------------------

_TIMESTAMP_RE = r'^\[(\d{2}:\d{2}:\d{2})\.(\d{3})\]\s*'

# [TANK] day 1   v=000001F2A3B4C5D6 fuel 0.4300 t | burned 0.0220 t | refills 0
# Also accepts nan/inf tokens for invalid-numerics detection (C5).
_FLT = r'(?:nan|inf|-inf|\+inf|-?\d+\.\d+)'
TANK_READING_RE = re.compile(
    r'\[TANK\]\s+day\s+(\d+)\s+v=(\S+)\s+'
    r'fuel\s+(' + _FLT + r')\s+t\s+\|\s+'
    r'burned\s+(' + _FLT + r')\s+t\s+\|\s+'
    r'refills\s+(\d+)'
)

# [TANK] day 1   v=ADDR slot X.XXXX t | mirror +0x5F0 Y.YYYY t | drift %+.4f t | agree|MIRROR HAS DRIFTED
TANK_CROSSCHECK_RE = re.compile(
    r'\[TANK\]\s+day\s+(\d+)\s+v=(\S+)\s+'
    r'slot\s+([+-]?\d+\.\d+)\s+t\s+\|\s+'
    r'mirror\s+\+0x5F0\s+([+-]?\d+\.\d+)\s+t\s+\|\s+'
    r'drift\s+([+-]?\d+\.\d+)\s+t\s+\|\s+'
    r'(agree|MIRROR HAS DRIFTED)'
)

# [TANK] run: burned X.XXXX t = Y.Y L | ... (long line, capture key fields)
TANK_RUN_RE = re.compile(
    r'\[TANK\]\s+run:\s+'
    r'burned\s+([+-]?\d+\.\d+)\s+t\s+=\s+([+-]?\d+\.\d+)\s+L'
)

# [PAY] freight DO West +5.15 (fuel 1.000ft) paid by the state
# [PAY] would factory "BuildingName" -5.15 (fuel -1.000ft) - dry_run
PAYMENT_RE = re.compile(
    r'\[PAY\]\s+'
    r'(would\s+)?'
    r'(\w+)'                                   # kind
    r'\s+'
    r'(.+?)'                                   # building name (greedy up to amount)
    r'\s+'
    r'([+-]\d+\.\d+)'                          # amount
    r'\s+'
    r'\(([^)]+)\)'                             # (resname tonsft)
    r'\s+'
    r'(paid by the state|- dry_run)'
)

# [PAY] day 5 closed: 10 payment(s), 0 capped | skipped: no payee 0, no price 0, too small 0, treasury 0 | factory 0.00, carrier 11.05 | PAID by the state treasury
PAYMENT_DAY_RE = re.compile(
    r'\[PAY\]\s+day\s+(\d+)\s+closed:\s+'
    r'(\d+)\s+payment\(s\),\s+'
    r'(\d+)\s+capped\s+\|\s+'
    r'skipped:\s+no payee\s+(\d+),\s+'
    r'no price\s+(\d+),\s+'
    r'too small\s+(\d+),\s+'
    r'treasury\s+(\d+)'
    r'\s+\|\s+'
    r'factory\s+([+-]?\d+\.\d+),\s+'
    r'carrier\s+([+-]?\d+\.\d+)'
    r'\s+\|\s+'
    r'(.+)'
)

# [BURN] run: measured 0.0450 t = 450.0 L between 2 delivery pair(s) = 225.0 L/trip | skipped 0 | configured 225.0 | apply
BURN_RUN_RE = re.compile(
    r'\[BURN\]\s+run:\s+'
    r'measured\s+([+-]?\d+\.\d+)\s+t\s+=\s+([+-]?\d+\.\d+)\s+L\s+'
    r'between\s+(\d+)\s+delivery\s+pair\(s\)'
)

# [BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid %.2f | ...
BAL_MOVEMENT_RE = re.compile(
    r'\[BAL\]\s+day\s+(\d+)\s+closed:\s+'
    r'treasury\s+([+-]?\d+\.\d+)\s+->\s+([+-]?\d+\.\d+)\s+'
    r'\(([+-][0-9.]+)\)\s+\|\s+'
    r'this plugin paid\s+([+-]?\d+\.\d+)'
)

# [BAL] day %d closed: treasury %.2f (baseline %s) | paid %.2f
BAL_BASELINE_RE = re.compile(
    r'\[BAL\]\s+day\s+(\d+)\s+closed:\s+'
    r'treasury\s+([+-]?\d+\.\d+)\s+\(baseline\s+\w+\)\s+\|\s+'
    r'paid\s+([+-]?\d+\.\d+)'
)

# [TARIFF] res tonsft @price = value -> bname: tariff = fuel X xY + wear Z (index fuel|cpi|none) | charged W by the state
TARIFF_DELIVERY_RE = re.compile(
    r'\[TARIFF\]\s+'
    r'(\S+)\s+'                       # resource name
    r'([+-]?\d+\.\d+)ft\s+'           # tons
    r'@([+-]?\d+\.\d+)\s+=\s+'       # unit price
    r'([+-]?\d+\.\d+)\s+'            # cargo value
    r'->\s+'
    r'([^:]+?)\s*:\s+'               # building name
    r'([+-]?\d+\.\d+)\s+=\s+'        # tariff
    r'fuel\s+([+-]?\d+\.\d+)\s+'     # fuel cost
    r'x([+-]?\d+\.\d+)\s+'          # multiplier
    r'\+\s+wear\s+([+-]?\d+\.\d+)\s+'
    r'\(index\s+(\w+)\)\s+\|\s+'
    r'charged\s+([+-]?\d+\.\d+)'
)

# [TARIFF] day %d closed: %u delivery(ies), charged|model %.2f = fuel %.2f x%.2f + wear %.2f, mean %.2f | indexed by fuel %d, by cpi %d, unindexed %d
TARIFF_DAY_RE = re.compile(
    r'\[TARIFF\]\s+day\s+(\d+)\s+closed:\s+'
    r'(\d+)\s+delivery\(ies\),\s+'
    r'(charged|model)\s+'
    r'([+-]?\d+\.\d+)\s+=\s+'
    r'fuel\s+([+-]?\d+\.\d+)\s+'
    r'x([+-]?\d+\.\d+)\s+'
    r'\+\s+wear\s+([+-]?\d+\.\d+)'
    r',\s+mean\s+([+-]?\d+\.\d+)'
    r'\s+\|\s+'
    r'indexed by fuel\s+(\d+),\s+'
    r'by cpi\s+(\d+),\s+'
    r'unindexed\s+(\d+)'
)

# [B2B] #123 freight bld=000001F2A3B4C5D6 "DO West" priv=1 res=fuel 1.00ft @0.00 = 0.00 (site 0x168EECA, mode 0)
B2B_RE = re.compile(
    r'\[B2B\]\s+'
    r'#(\d+)\s+'                     # ship id
    r'(\w+)\s+'                      # kind
    r'bld=(\S+)\s+'                  # building addr
    r'"([^"]*)"\s+'                  # building name
    r'priv=(\d+)\s+'
    r'res=(\S+)\s+'                  # resource name
    r'([+-]?\d+\.\d+)ft\s+'         # tons
    r'@([+-]?\d+\.\d+)\s+=\s+'      # unit price = value
    r'([+-]?\d+\.\d+)'              # value
    r'\s+\(site\s+0x([0-9A-Fa-f]+),\s+'
    r'mode\s+(\d+)\)'
)

# [B2B] day 5 closed: 10 calls, 8 committed, 2 no-op | staging 0, non-private 0
B2B_DAY_RE = re.compile(
    r'\[B2B\]\s+day\s+(\d+)\s+closed:\s+'
    r'(\d+)\s+calls,\s+'
    r'(\d+)\s+committed,\s+'
    r'(\d+)\s+no-op'
    r'(?:\s+\|\s+staging\s+(\d+),\s+'
    r'non-private\s+(\d+))?'
)

# [B2B SITE] rva=0x001B2898 mode=0 sign=+ tons=1.00ft commit=1 flag=1 p2=000001... "DO West" priv=1 p9=000001... vhome=000001... class=carrier
B2B_SITE_RE = re.compile(
    r'\[B2B\s+SITE\]\s+'
    r'rva=0x([0-9A-Fa-f]+)\s+'
    r'mode=(\d+)\s+'
    r'sign=(\S)\s+'
    r'tons=([+-]?\d+\.\d+)ft\s+'
    r'commit=(\d)\s+'
    r'flag=(\d)\s+'
    r'p2=(\S+)\s+'
    r'"([^"]*)"\s+'
    r'priv=(\d)\s+'
    r'p9=(\S+)\s+'
    r'vhome=(\S+)\s+'
    r'class=(\w+)'
)

# [VEH] day 1   v=000001F2A3B4C5D6 tank capacity 0.4300 t (430 L at 1 kg/L)
VEH_CAPACITY_RE = re.compile(
    r'\[VEH\]\s+day\s+(\d+)\s+'
    r'v=(\S+)\s+tank\s+capacity\s+'
    r'([+-]?\d+\.\d+)\s+t\s+'
    r'\((\d+)\s+L\s+at\s+1\s+kg/L\)'
)

# [VEH] day 1   v=ADDR home=000001... cargo 0.000ft in 1 slot(s) kind=0 ... mode=0 ... ACTIVE|idle
# This is a simplified match; we extract the key fields.
VEH_DETAIL_RE = re.compile(
    r'\[VEH\]\s+day\s+(\d+)\s+'
    r'v=(\S+)\s+'
    r'home=(\S+)\s+'
    r'cargo\s+([+-]?\d+\.\d+)ft\s+in\s+(\d+)\s+slot\(s\)\s+'
    r'kind=(\d+)'
    r'.*?'
    r'mode=(\d+)'
    r'.*?'
    r'(ACTIVE|idle)'
)

# [VEH] day 5 "DO West" fleet: 4 vehicle(s)
VEH_FLEET_RE = re.compile(
    r'\[VEH\]\s+day\s+(\d+)\s+'
    r'"([^"]*)"\s+fleet:\s+'
    r'(\d+)\s+vehicle\(s\)'
)

# [FUEL] day 1   "DO West" diesel 862.50 (measured, cat=2) = 12500.0 L at 6.90/RUB
FUEL_METER_RE = re.compile(
    r'\[FUEL\]\s+day\s+(\d+)\s+'
    r'"([^"]*)"\s+'
    r'diesel\s+([+-]?\d+\.\d+)\s+'
    r'\(measured,\s+cat=2\)\s+=\s+'
    r'([+-]?\d+\.\d+)\s+L\s+'
    r'at\s+([+-]?\d+\.\d+)/(\w+)'
)

# [IDX] day 5 closed: fuel 'fuel' = 6.90/RUB (resolved) | cpi basket 0.00/day | margin 90%, wear 0.52 L
IDX_DAY_RE = re.compile(
    r'\[IDX\]\s+day\s+(\d+)\s+closed:\s+'
    r"fuel\s+'([^']*)'\s+=\s+"
    r'([+-]?\d+\.\d+)/(\w+)\s+'
    r'\((\w+)\)\s+\|\s+'
    r'cpi basket\s+([+-]?\d+\.\d+)/day\s+\|\s+'
    r'margin\s+(\d+)%,\s+wear\s+([+-]?\d+\.\d+)\s+L'
)

# private_logistics  v0.17 - billing LIVE (moves money)
BANNER_V_RE = re.compile(
    r'private_logistics\s+v(\d+\.\d+)\s+-\s+billing\s+(\w+)'
    r'(?:\s+\(([^)]+)\))?'
)

# private_logistics  live v0.17: billing=1 dry_run=0 factory=1 carrier=1, ...
BANNER_LIVE_RE = re.compile(
    r'private_logistics\s+live\s+v(\d+\.\d+):\s+'
    r'billing=(\d)\s+'
    r'dry_run=(\d)'
)

# ---------------------------------------------------------------------------
# Unrecognized line wrapper
# ---------------------------------------------------------------------------

@dataclass
class UnrecognizedLine:
    """A log line that matched no known [TAG]."""
    raw: str


# ---------------------------------------------------------------------------
# Parser
# ---------------------------------------------------------------------------

@dataclass
class ParsedLine:
    """A single parsed log line: timestamp + record (or unrecognized)."""
    timestamp: Optional[str]    # "HH:MM:SS.mmm"
    record: Union[object, UnrecognizedLine]


def _strip_resource_tons(field: str) -> tuple[str, float]:
    """Parse 'resname 1.000ft' or 'resname -1.000ft' into (resname, tons)."""
    m = re.match(r'^(\S+)\s+([+-]?\d+\.\d+)ft$', field.strip())
    if not m:
        return field.strip(), 0.0
    return m.group(1), float(m.group(2))


def parse_line(line: str) -> Optional[ParsedLine]:
    """Parse a single log line.

    Returns a ParsedLine with timestamp and record, or None if the line is
    blank or is a non-plugin line that we can identify (e.g., the Loader's
    own header lines that don't belong to any plugin).
    """
    stripped = line.rstrip('\n\r')
    if not stripped.strip():
        return None

    # Extract timestamp prefix
    ts_match = re.match(_TIMESTAMP_RE, stripped)
    timestamp = None
    message = stripped
    if ts_match:
        timestamp = f"{ts_match.group(1)}.{ts_match.group(2)}"
        message = stripped[ts_match.end():]

    # Try each record parser
    record = _parse_message(message)
    return ParsedLine(timestamp=timestamp, record=record)


def _parse_message(msg: str) -> Union[object, UnrecognizedLine]:
    """Route a timestamp-stripped message to the appropriate record parser."""

    # Try banner first (no [TAG] prefix)
    m = BANNER_V_RE.match(msg)
    if m:
        return PluginBanner(
            version=m.group(1),
            billing=(m.group(2) == 'LIVE'),
            dry_run=False,
            live=(m.group(2) == 'LIVE'),
            raw=msg,
        )
    m = BANNER_LIVE_RE.match(msg)
    if m:
        return PluginBanner(
            version=m.group(1),
            billing=(m.group(2) == '1'),
            dry_run=(m.group(3) == '1'),
            live=True,
            raw=msg,
        )

    # All remaining tags start with [
    if not msg.startswith('['):
        return UnrecognizedLine(raw=msg)

    # Tank readings and cross-checks (both start with [TANK])
    m = TANK_CROSSCHECK_RE.match(msg)
    if m:
        return TankCrossCheck(
            day=int(m.group(1)),
            vehicle_addr=m.group(2),
            slot_level=float(m.group(3)),
            mirror_level=float(m.group(4)),
            drift=float(m.group(5)),
            status=m.group(6),
            raw=msg,
        )
    m = TANK_READING_RE.match(msg)
    if m:
        return TankReading(
            day=int(m.group(1)),
            vehicle_addr=m.group(2),
            fuel=float(m.group(3)),
            burned=float(m.group(4)),
            refills=int(m.group(5)),
            raw=msg,
        )
    m = TANK_RUN_RE.match(msg)
    if m:
        return TankRunTotal(
            burned_tons=float(m.group(1)),
            burned_litres=float(m.group(2)),
            lp_trip=0.0,
            deliveries=0,
            raw=msg,
        )

    # Payments
    m = PAYMENT_RE.match(msg)
    if m:
        res_name, tons = _strip_resource_tons(m.group(5))
        return Payment(
            dry_run=(m.group(1) is not None),
            kind=m.group(2),
            building_name=m.group(3).strip().strip('"'),
            amount=float(m.group(4)),
            resource_name=res_name,
            tons=tons,
            raw=msg,
        )
    m = PAYMENT_DAY_RE.match(msg)
    if m:
        return PaymentDay(
            day=int(m.group(1)),
            payments=int(m.group(2)),
            capped=int(m.group(3)),
            skip_no_payee=int(m.group(4)),
            skip_no_price=int(m.group(5)),
            skip_too_small=int(m.group(6)),
            skip_treasury=int(m.group(7)),
            factory_total=float(m.group(8)),
            carrier_total=float(m.group(9)),
            live=('PAID by the state treasury' in m.group(10)),
            raw=msg,
        )

    # Burn probe
    m = BURN_RUN_RE.match(msg)
    if m:
        return BurnProbe(
            measured_tons=float(m.group(1)),
            measured_litres=float(m.group(2)),
            delivery_pairs=int(m.group(3)),
            lp_trip=0.0,
            skipped=0,
            configured_lp_trip=0.0,
            action='apply',
            raw=msg,
        )

    # Treasury balance
    m = BAL_MOVEMENT_RE.match(msg)
    if m:
        return TreasuryBalance(
            day=int(m.group(1)),
            before=float(m.group(2)),
            after=float(m.group(3)),
            delta=float(m.group(4)),
            plugin_paid=float(m.group(5)),
            is_baseline=False,
            raw=msg,
        )
    m = BAL_BASELINE_RE.match(msg)
    if m:
        return TreasuryBalance(
            day=int(m.group(1)),
            before=float(m.group(2)),
            after=None,
            delta=None,
            plugin_paid=float(m.group(3)),
            is_baseline=True,
            raw=msg,
        )

    # Tariff
    m = TARIFF_DELIVERY_RE.match(msg)
    if m:
        return TariffDelivery(
            resource_name=m.group(1),
            tons=float(m.group(2)),
            unit_price=float(m.group(3)),
            cargo_value=float(m.group(4)),
            building_name=m.group(5).strip().strip('"'),
            tariff=float(m.group(6)),
            fuel_cost=float(m.group(7)),
            multiplier=float(m.group(8)),
            wear_cost=float(m.group(9)),
            index=m.group(10),
            charged=float(m.group(11)),
            raw=msg,
        )
    m = TARIFF_DAY_RE.match(msg)
    if m:
        return TariffDay(
            day=int(m.group(1)),
            deliveries=int(m.group(2)),
            status=m.group(3),
            total=float(m.group(4)),
            fuel_total=float(m.group(5)),
            multiplier=float(m.group(6)),
            wear_total=float(m.group(7)),
            mean=float(m.group(8)),
            indexed_fuel=int(m.group(9)),
            indexed_cpi=int(m.group(10)),
            unindexed=int(m.group(11)),
            raw=msg,
        )

    # B2B
    m = B2B_SITE_RE.match(msg)
    if m:
        return B2BSite(
            call_site_rva=f"0x{m.group(1)}",
            mode=int(m.group(2)),
            sign=m.group(3),
            tons=float(m.group(4)),
            committed=int(m.group(5)),
            flag=int(m.group(6)),
            p2=m.group(7),
            building_name=m.group(8),
            private=int(m.group(9)),
            p9=m.group(10),
            vhome=m.group(11),
            site_class=m.group(12),
            raw=msg,
        )
    m = B2B_RE.match(msg)
    if m:
        return B2BTransfer(
            ship_id=int(m.group(1)),
            kind=m.group(2),
            building_addr=m.group(3),
            building_name=m.group(4),
            private=int(m.group(5)),
            resource=m.group(6),
            tons=float(m.group(7)),
            unit_price=float(m.group(8)),
            value=float(m.group(9)),
            site_rva=m.group(10),
            mode=int(m.group(11)),
            raw=msg,
        )
    m = B2B_DAY_RE.match(msg)
    if m:
        return B2BDay(
            day=int(m.group(1)),
            calls=int(m.group(2)),
            committed=int(m.group(3)),
            noop=int(m.group(4)),
            unknown=0,
            staging=int(m.group(5)) if m.group(5) else 0,
            non_private=int(m.group(6)) if m.group(6) else 0,
            raw=msg,
        )

    # Vehicle census
    m = VEH_CAPACITY_RE.match(msg)
    if m:
        return VehicleCapacity(
            day=int(m.group(1)),
            vehicle_addr=m.group(2),
            capacity_tons=float(m.group(3)),
            capacity_litres=float(m.group(4)),
            raw=msg,
        )
    m = VEH_FLEET_RE.match(msg)
    if m:
        return VehicleCensus(
            day=int(m.group(1)),
            building_name=m.group(2),
            vehicle_count=int(m.group(3)),
            raw=msg,
        )
    m = VEH_DETAIL_RE.match(msg)
    if m:
        return VehicleDetail(
            day=int(m.group(1)),
            vehicle_addr=m.group(2),
            home=m.group(3),
            cargo_tons=float(m.group(4)),
            slots=int(m.group(5)),
            kind=int(m.group(6)),
            state=0,
            mode=int(m.group(7)),
            active=(m.group(8) == 'ACTIVE'),
            raw=msg,
        )

    # Fuel meter
    m = FUEL_METER_RE.match(msg)
    if m:
        return FuelMeter(
            day=int(m.group(1)),
            building_name=m.group(2),
            diesel_rub=float(m.group(3)),
            litres=float(m.group(4)),
            fuel_price=float(m.group(5)),
            currency=m.group(6),
            raw=msg,
        )

    # Fuel index
    m = IDX_DAY_RE.match(msg)
    if m:
        return FuelIndex(
            day=int(m.group(1)),
            fuel_resource=m.group(2),
            fuel_price=float(m.group(3)),
            currency=m.group(4),
            price_status=m.group(5),
            cpi_basket=float(m.group(6)),
            margin_pct=float(m.group(7)),
            wear_litres=float(m.group(8)),
            raw=msg,
        )

    return UnrecognizedLine(raw=msg)


def parse_log(log_text: str) -> List[ParsedLine]:
    """Parse a full log file's text into ParsedLine records.

    Blank lines and non-plugin header lines return None and are filtered out.
    Lines that do not match any known tag are returned as UnrecognizedLine
    records inside the ParsedLine.
    """
    results: List[ParsedLine] = []
    for line in log_text.splitlines():
        pl = parse_line(line)
        if pl is not None:
            results.append(pl)
    return results


def iter_records(log_text: str) -> Iterator[object]:
    """Yield parsed records (not UnrecognizedLine) from log text."""
    for pl in parse_log(log_text):
        if not isinstance(pl.record, UnrecognizedLine):
            yield pl.record
