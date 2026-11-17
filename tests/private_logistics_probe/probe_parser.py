"""Parser for [PROBE ...] structured log records emitted by private_logistics_probe.

Each logical record spans multiple physical lines:
  Line 1: header — `[PROBE <RECKIND>] seq=N ts=X.XXXX thr=HEX kind=NAME schema=1 committed=N load=N unload=N complete=N gap=N error=N synth=N`
  Lines 2+: continuation — ` key=value key=value ...` (space-prefixed, no timestamp)

The header line also carries the Loader timestamp prefix [HH:MM:SS.mmm] on
the first physical line.

The `kind=` field in the header carries the actual record kind in
lowercase (the same name as the `[PROBE <RECKIND>]` prefix). The parser
uses the prefix as authoritative and treats `kind=` as redundant.

Confidence strings (from schema + emitter):
  verified, partial, unverified, synthetic, unsupported
Unknown values are never upgraded to "verified".
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Dict, List, Optional

from tests.private_logistics_probe.probe_schema import (
    DEFAULT_TOLERANCE_T,
    FUEL_UNAVAILABLE,
    PROBE_SCHEMA_COMMIT,
    PROBE_SCHEMA_VERSION,
    ProbeDrop,
    ProbeEvent,
    ProbeHeader,
    ProbeInterp,
    ProbeRecord,
    ProbeSession,
    ProbeSnapshot,
    ProbeTrip,
    ProbeDiscrepancy,
)


# ---------------------------------------------------------------------------
# Regex patterns for PROBE records
# ---------------------------------------------------------------------------

_TIMESTAMP_RE = re.compile(r'^\[(\d{2}:\d{2}:\d{2})\.(\d{3})\]')

# Header line: [PROBE <RECKIND>] seq=N ts=X.XXXX thr=HEX kind=NAME schema=1 ...
PROBE_HEADER_RE = re.compile(
    r'^\[PROBE\s+'
    r'(SESSION|EVENT|INTERP|TRIP|DROP|SYNTH|SNAPSHOT)'  # RECKIND prefix (authoritative)
    r'\]\s+'
    r'seq=(\d+)\s+'
    r'ts=([+-]?\d+\.\d+)\s+'
    r'thr=(\S+)\s+'
    r'kind=\S+\s+'                # actual record kind, redundant with prefix
    r'schema=(\d+)\s+'
    r'committed=(\d)\s+'
    r'load=(\d)\s+'
    r'unload=(\d)\s+'
    r'complete=(\d)\s+'
    r'gap=(\d)\s+'
    r'error=(\d)\s+'
    r'synth=(\d)'
)


# ---------------------------------------------------------------------------
# Key=value parsing helpers
# ---------------------------------------------------------------------------

def _parse_kv_pairs(text: str) -> Dict[str, str]:
    """Parse space-separated key=value pairs, handling quoted values."""
    result: Dict[str, str] = {}
    i = 0
    s = text
    while i < len(s):
        while i < len(s) and s[i] in ' \t':
            i += 1
        if i >= len(s):
            break
        key_start = i
        while i < len(s) and s[i] not in ' =\t':
            i += 1
        key = s[key_start:i]
        if i >= len(s) or s[i] != '=':
            while i < len(s) and s[i] not in ' \t':
                i += 1
            continue
        i += 1  # skip '='
        if i < len(s) and s[i] == '"':
            i += 1
            val_start = i
            while i < len(s) and s[i] != '"':
                i += 1
            val = s[val_start:i]
            if i < len(s):
                i += 1
        else:
            val_start = i
            while i < len(s) and s[i] not in ' \t':
                i += 1
            val = s[val_start:i]
        result[key] = val
    return result


def _parse_f64(val: str) -> Optional[float]:
    """Parse a float, returning None for 'unavailable' or invalid tokens."""
    if not val or val == "unavailable":
        return None
    try:
        return float(val)
    except ValueError:
        return None


def _or_unavail(val: Optional[float]) -> float:
    """Coerce None to FUEL_UNAVAILABLE; preserves 0.0 and NaN."""
    return FUEL_UNAVAILABLE if val is None else val


# ---------------------------------------------------------------------------
# Record assembly
# ---------------------------------------------------------------------------

@dataclass
class RawProbeLine:
    """A single physical log line in the PROBE log."""
    timestamp: Optional[str]
    text: str
    raw_line: str


@dataclass
class AssembledProbeRecord:
    """A complete PROBE record assembled from multiple physical lines."""
    header_line: str
    continuation_fields: Dict[str, str]
    header_fields: Dict[str, str]
    timestamp: Optional[str]
    raw: str


def _split_log_lines(log_text: str) -> List[RawProbeLine]:
    """Split log text into physical lines, extracting timestamps."""
    lines: List[RawProbeLine] = []
    for raw_line in log_text.splitlines():
        stripped = raw_line.rstrip('\n\r')
        ts_match = _TIMESTAMP_RE.match(stripped)
        if ts_match:
            timestamp = f"{ts_match.group(1)}.{ts_match.group(2)}"
            text = stripped[ts_match.end():].lstrip()
        else:
            timestamp = None
            text = stripped.lstrip()
        lines.append(RawProbeLine(timestamp=timestamp, text=text, raw_line=raw_line))
    return lines


def _assemble_records(lines: List[RawProbeLine]) -> List[AssembledProbeRecord]:
    """Group physical lines into logical PROBE records."""
    records: List[AssembledProbeRecord] = []
    current: Optional[AssembledProbeRecord] = None

    for line in lines:
        if line.text.startswith('[PROBE '):
            if current is not None:
                records.append(current)
            header_fields = _parse_kv_pairs_from_header(line.text)
            current = AssembledProbeRecord(
                header_line=line.text,
                continuation_fields={},
                header_fields=header_fields,
                timestamp=line.timestamp,
                raw=line.raw_line,
            )
        elif current is not None and not line.text.startswith('['):
            kvs = _parse_kv_pairs(line.text)
            current.continuation_fields.update(kvs)
            current.raw += '\n' + line.raw_line

    if current is not None:
        records.append(current)

    return records


def _parse_kv_pairs_from_header(text: str) -> Dict[str, str]:
    """Parse the header line which starts with [PROBE KIND]."""
    m = re.match(r'^\[PROBE\s+(\w+)\]\s+(.*)', text)
    if not m:
        return {}
    kvs = {'record_kind': m.group(1)}
    kvs.update(_parse_kv_pairs(m.group(2)))
    return kvs


# ---------------------------------------------------------------------------
# Typed record builders
# ---------------------------------------------------------------------------

def _build_header(hf: Dict[str, str]) -> ProbeHeader:
    kind = hf.get('record_kind', '?')
    return ProbeHeader(
        record_kind=kind,
        seq=int(hf.get('seq', '0')),
        ts=float(hf.get('ts', '0')),
        thr=hf.get('thr', 'null'),
        kind_field=hf.get('kind', '?'),
        schema_version=int(hf.get('schema', '0')),
        committed=int(hf.get('committed', '0')),
        load=int(hf.get('load', '0')),
        unload=int(hf.get('unload', '0')),
        complete=int(hf.get('complete', '0')),
        gap=int(hf.get('gap', '0')),
        error=int(hf.get('error', '0')),
        synth=int(hf.get('synth', '0')),
    )


def build_probe_record(ra: AssembledProbeRecord) -> ProbeRecord:
    hf = ra.header_fields
    cf = ra.continuation_fields
    header = _build_header(hf)

    if header.record_kind == "SESSION":
        kind_record = ProbeSession(
            header=header,
            session_id=cf.get('session_id', ''),
            event=cf.get('event', ''),
            day=int(cf.get('day', '-1')),
            mon=int(cf.get('mon', '-1')),
            yr=int(cf.get('yr', '-1')),
            note=cf.get('note', ''),
            raw=ra.raw,
        )
    elif header.record_kind == "EVENT":
        kind_record = ProbeEvent(
            header=header,
            site=cf.get('site', ''),
            mode=int(cf.get('mode', '0')),
            veh=cf.get('veh', 'null'),
            do=cf.get('do', 'null'),
            res=cf.get('res', '?'),
            load_site=cf.get('load_site', 'null'),
            unload_site=cf.get('unload_site', 'null'),
            fb=_or_unavail(_parse_f64(cf.get('fb'))),
            fa=_or_unavail(_parse_f64(cf.get('fa'))),
            ded=_or_unavail(_parse_f64(cf.get('ded'))),
            cb=float(cf.get('cb', '0')),
            ca=float(cf.get('ca', '0')),
            conf=cf.get('conf', 'unverified'),
            day=int(cf.get('day', '-1')),
            raw=ra.raw,
        )
    elif header.record_kind == "INTERP":
        kind_record = ProbeInterp(
            header=header,
            veh=cf.get('veh', 'null'),
            what=cf.get('what', ''),
            fb=_or_unavail(_parse_f64(cf.get('fb'))),
            fa=_or_unavail(_parse_f64(cf.get('fa'))),
            ded=_or_unavail(_parse_f64(cf.get('ded'))),
            complete=int(cf.get('complete', '0')),
            gap=int(cf.get('gap', '0')),
            error=int(cf.get('error', '0')),
            raw=ra.raw,
        )
    elif header.record_kind == "TRIP":
        kind_record = ProbeTrip(
            header=header,
            veh=cf.get('veh', 'null'),
            fuel_before=_or_unavail(_parse_f64(cf.get('fuel_before'))),
            fuel_after=_or_unavail(_parse_f64(cf.get('fuel_after'))),
            deduction=_or_unavail(_parse_f64(cf.get('deduction'))),
            trips=int(cf.get('trips', '0')),
            completed=int(cf.get('completed', '0')),
            replayed=int(cf.get('replayed', '0')),
            unsupported=int(cf.get('unsupported', '0')),
            dup_loads=int(cf.get('dup_loads', '0')),
            fuel_price=cf.get('fuel_price', 'unavailable'),
            currency=cf.get('currency', 'unavailable'),
            note=cf.get('note', ''),
            raw=ra.raw,
        )
    elif header.record_kind == "DROP":
        kind_record = ProbeDrop(
            header=header,
            reason=cf.get('reason', ''),
            ring_drops=int(cf.get('ring_drops', '0')),
            today_drops=int(cf.get('today_drops', '0')),
            raw=ra.raw,
        )
    elif header.record_kind == "SNAPSHOT":
        kind_record = ProbeSnapshot(
            header=header,
            day=int(cf.get('day', '-1')),
            mon=int(cf.get('mon', '-1')),
            yr=int(cf.get('yr', '-1')),
            tod=float(cf.get('tod', '0')),
            veh=cf.get('veh', 'null'),
            do=cf.get('do', 'null'),
            record_type=cf.get('record_type', ''),
            snapshot_ok=int(cf.get('snapshot_ok', '0')),
            fail_reason=cf.get('fail_reason', ''),
            fuel_slot_valid=int(cf.get('fuel_slot_valid', '0')),
            fuel_tons=_or_unavail(_parse_f64(cf.get('fuel_tons'))),
            mirror_valid=int(cf.get('mirror_valid', '0')),
            mirror_tons=_or_unavail(_parse_f64(cf.get('mirror_tons'))),
            cargo_valid=int(cf.get('cargo_valid', '0')),
            cargo_slots=int(cf.get('cargo_slots', '-1')),
            cargo_total_tons=_or_unavail(_parse_f64(cf.get('cargo_total_tons'))),
            cargo=cf.get('cargo', ''),
            kind_valid=int(cf.get('kind_valid', '0')),
            kind=int(cf.get('kind', '-1')),
            state_valid=int(cf.get('state_valid', '0')),
            state=int(cf.get('state', '-1')),
            mode_valid=int(cf.get('mode_valid', '0')),
            mode=int(cf.get('mode', '-1')),
            home_matches_valid=int(cf.get('home_matches_valid', '0')),
            home_matches=int(cf.get('home_matches', '-1')),
            raw=ra.raw,
        )
    else:
        kind_record = None

    return ProbeRecord(
        header=header,
        fields={**hf, **cf},
        kind_record=kind_record,
        raw=ra.raw,
    )


# ---------------------------------------------------------------------------
# Discrepancy reporting
# ---------------------------------------------------------------------------

DISCREPANCIES: List[ProbeDiscrepancy] = [
    ProbeDiscrepancy(
        discrepancy_id="D-THR-FORMAT",
        description=(
            "thr= is formatted as hex pointer (%p) by PtrStr(), "
            "not decimal u64 as schema states."
        ),
        severity="cosmetic",
        location="private_logistics_probe.cpp:PtrStr + LogHeader",
        schema_field="line_format.thr",
    ),
    ProbeDiscrepancy(
        discrepancy_id="D-RES-UNRESOLVED",
        description=(
            "Resource pointer is resolved via TsmResourceName only for valid "
            "game pointers; synthetic/unresolvable pointers yield '?'."
        ),
        severity="doc",
        location="private_logistics_probe.cpp:ResourceNameSafe",
        schema_field="EVENT.fields[].res",
    ),
    ProbeDiscrepancy(
        discrepancy_id="D-TS-PRECISION",
        description=(
            "ts is formatted with %.4f (4 decimal places). Schema says <f64>."
        ),
        severity="doc",
        location="private_logistics_probe.cpp:LogHeader",
        schema_field="line_format.ts",
    ),
]


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def parse_probe_log(log_text: str) -> List[ProbeRecord]:
    """Parse a full log text and return typed PROBE records."""
    lines = _split_log_lines(log_text)
    assembled = _assemble_records(lines)
    return [
        build_probe_record(ra)
        for ra in assembled
        if PROBE_HEADER_RE.match(ra.header_line) is not None
    ]


def iter_probe_records(log_text: str) -> List[ProbeRecord]:
    """Alias for parse_probe_log — yields typed PROBE records only."""
    return parse_probe_log(log_text)
