"""Frozen PROBE log schema v1.0 for the private-logistics diagnostic probe.

SCHEMA VERSION: private-logistics-probe-schema-v1.0
FROZEN: 2026-10-24
SOURCE: docs/private-logistics-runtime/probe/probe_schema.json (commit ce8648d5)
        plugins/private_logistics_probe/private_logistics_probe.cpp (commit ce8648d5)

The JSON schema (probe_schema.json) defines the contract. This module
implements the Python-side record types that match the emitter's actual
output, including known discrepancies from the schema.

KNOWN DISCREPANCIES (emitter vs schema):
  1. `thr=` is formatted as a hex pointer (`%p`) by the emitter, not a
     decimal u64 as the schema states. Parser accepts both hex and decimal.
  2. `res=` is "?" for synthetic/unresolvable pointers (schema says
     `res=<string>`). Parser accepts "?" as valid (unresolved).
  3. `ts=` is formatted with `%.4f` (4 decimal places). Schema says `<f64>`.
  4. Records span multiple physical log lines (header + continuation lines).
     Continuation lines have no timestamp prefix.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

PROBE_SCHEMA_VERSION = 1
PROBE_SCHEMA_NAME = "private-logistics-probe-schema-v1.0"
PROBE_SCHEMA_COMMIT = "ce8648d51f9a9a7d0a87158f2cb0b7c0ffe37459"

# ---------------------------------------------------------------------------
# Reconciliation tolerances
# ---------------------------------------------------------------------------

# The emitter formats fuel as %.6f (6 decimal places = 1e-6 t precision).
# Float64 representation error at ~0.4 t scale is ~1e-15 t (negligible).
# Observation timing: fuel_before and fuel_after are read at different engine
# ticks, so a single tick of engine idle consumption may differ.
# Tolerance for PROBE fuel reconciliation. The emitter logs fuel values with
# %.6f precision (6 decimal places). The tolerance is set to 0.001 t, which
# corresponds to the 4th decimal place of the logged value. This is a parser
# matching assumption, NOT a statement about engine physics or measurement
# accuracy. The tolerance is not derived from typical consumption or trip
# timing — those values vary by vehicle and route and are not modeled here.
DEFAULT_TOLERANCE_T = 0.001

# Sentinel for "unavailable" fuel values
FUEL_UNAVAILABLE = -1.0


# ---------------------------------------------------------------------------
# Record dataclasses
# ---------------------------------------------------------------------------

@dataclass
class ProbeHeader:
    """Header fields common to all PROBE records (from LogHeader)."""
    record_kind: str        # "SESSION", "EVENT", "INTERP", "TRIP", "DROP"
    seq: int                # monotonically increasing event sequence
    ts: float              # monotonic seconds since session start
    thr: str               # thread id as hex string (emitter uses %p)
    kind_field: str        # record kind, lowercase (redundant with prefix)
    schema_version: int    # always 1
    committed: int         # 0|1
    load: int              # 0|1
    unload: int            # 0|1
    complete: int          # 0|1
    gap: int               # 0|1
    error: int            # 0|1
    synth: int             # 0|1


@dataclass
class ProbeSession:
    """SESSION record: session start / armed / disabled."""
    header: ProbeHeader
    session_id: str
    event: str              # "start" | "armed" | "disabled"
    day: int               # sim day or -1
    mon: int               # sim month or -1-derived
    yr: int                # sim year
    note: str
    raw: str = ""


@dataclass
class ProbeEvent:
    """EVENT record: raw observation of a transfer/movement call."""
    header: ProbeHeader
    site: str               # observation site, e.g. "synthetic:load"
    mode: int              # 0=building side, 1=vehicle side
    veh: str               # vehicle identity (hex pointer or "null")
    do: str                # distribution office / building identity (or "null")
    res: str               # resource name or "?"
    load_site: str         # load site pointer (or "null")
    unload_site: str       # unload site pointer (or "null")
    fb: float              # fuel_before (tonnes), -1 = unavailable
    fa: float              # fuel_after (tonnes), -1 = unavailable
    ded: float             # deduction = fb - fa, -1 = gap
    cb: float              # cargo_before (tonnes)
    ca: float              # cargo_after (tonnes)
    conf: str              # evidence confidence: verified | partial | unsupported | synthetic
    day: int               # sim day at observation
    raw: str = ""


@dataclass
class ProbeInterp:
    """INTERP record: interpreted trip boundary / gap / error."""
    header: ProbeHeader
    veh: str
    what: str               # "trip_open_load" | "trip_complete" | etc.
    fb: float
    fa: float
    ded: float
    complete: int          # 0|1
    gap: int              # 0|1
    error: int           # 0|1
    raw: str = ""


@dataclass
class ProbeTrip:
    """TRIP record: trip summary with settlement-time fuel price and currency."""
    header: ProbeHeader
    veh: str
    fuel_before: float
    fuel_after: float
    deduction: float
    trips: int            # trips tracked for this vehicle
    completed: int
    replayed: int         # always 0: closed trips cannot replay
    unsupported: int
    dup_loads: int        # additional loads that were not duplicates
    fuel_price: str       # "unavailable" in Phase 1
    currency: str         # "unavailable" in Phase 1
    note: str            # "phase1_no_price_source"
    raw: str = ""


@dataclass
class ProbeDrop:
    """DROP record: dropped observation (ring overflow / log cap)."""
    header: ProbeHeader
    reason: str           # "ring_overflow" | "log_cap" | ...
    ring_drops: int       # ring buffer drops total
    today_drops: int      # drops today
    raw: str = ""


@dataclass
class ProbeSnapshot:
    """SNAPSHOT record: sampled vehicle snapshot (exploratory telemetry only).

    NOT accumulated fuel burn, NOT trip-boundary verification, NOT trip-cost
    validation. Every measurement field carries a *_valid flag; missing or
    unreadable values are FUEL_UNAVAILABLE (-1.0), never zero or inferred.
    A readable mirror does NOT validate the authoritative fuel slot.
    """
    header: ProbeHeader
    observation_seq: int     # independent snapshot counter (drain thread), starts at 1
    day: int                 # sim day (from sim_clock getDay, -1 when no world)
    mon: int                 # sim month (from sim_clock getMonth, -1 when no world)
    yr: int                  # sim year (from sim_clock getYear, -1 when no world)
    tod: float               # time of day fraction (from sim_clock getTimeOfDay)
    veh: str                 # selected vehicle identity (null when no match / lost)
    do: str                  # selected DO building identity (null when no match / lost)
    record_type: str         # "selection" | "measurement" | "no_match" | "vehicle_lost" | "world_reset"
    snapshot_ok: int         # 0|1 — snapshot capture succeeded
    fail_reason: str         # "no_match" | "vehicle_gone" | "selected" | "" | "world_reset"
    fuel_slot_valid: int     # 0|1 — fuel slot found and readable
    fuel_tons: float         # fuel slot tonnes; FUEL_UNAVAILABLE if unavailable
    mirror_valid: int        # 0|1 — fuel mirror readable (does NOT validate slot)
    mirror_tons: float       # fuel mirror tonnes; FUEL_UNAVAILABLE if unavailable
    cargo_valid: int         # 0|1 — cargo vector readable
    cargo_slots: int         # non-empty cargo slots; -1 if unavailable
    cargo_total_tons: float  # total cargo tonnes; FUEL_UNAVAILABLE if unavailable
    cargo: str               # bounded cargo listing "res=tons ..."; "" if unavailable
    kind_valid: int          # 0|1 — vehicle kind readable
    kind: int                # vehicle kind raw value; -1 if unavailable
    state_valid: int         # 0|1 — vehicle state readable
    state: int               # vehicle state raw value; -1 if unavailable (does not invalidate)
    mode_valid: int          # 0|1 — vehicle mode readable
    mode: int                # vehicle mode raw value; -1 if unavailable (does not invalidate)
    home_matches_valid: int  # 0|1 — home depot match readable
    home_matches: int        # vehicle home depot == selected DO; -1 if unavailable
    raw: str = ""


@dataclass
class ProbeRecord:
    """Container for a complete PROBE record (header + continuation fields)."""
    header: ProbeHeader
    fields: dict          # kind-specific fields as key=value
    kind_record: object   # typed record (ProbeSession | ProbeEvent | etc.)
    raw: str = ""         # full raw text of all lines for this record


@dataclass
class ProbeDiscrepancy:
    """A known discrepancy between the emitter and the schema."""
    discrepancy_id: str
    description: str
    severity: str         # "bug" | "cosmetic" | "doc"
    location: str         # emitter source location
    schema_field: str     # field in probe_schema.json that is affected
