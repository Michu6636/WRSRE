"""Self-contained validation helpers for Almanac UX synthetic fixtures.

These helpers validate fixture *schema and classification* only. They do not
import or exercise any production module, and they never assert that a
deferred runtime/native acceptance criterion has passed.
"""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, List

FIXTURE_DIR = Path(__file__).resolve().parent / "fixtures"

SCHEMA_VERSION = "almanac-fixture-v1"
VALUE_CLASSES = {"ACTUAL", "DRY_RUN", "ESTIMATED"}
CURRENCIES = {"RUB", "USD", "NONE"}
UNITS = {"money", "tons", "count", "percent", "days", "mixed"}


def fixture_path(name: str) -> Path:
    return FIXTURE_DIR / name


def load_fixture(name: str) -> Dict[str, Any]:
    with fixture_path(name).open("r", encoding="utf-8") as handle:
        return json.load(handle)


def all_fixture_files() -> List[Path]:
    return sorted(FIXTURE_DIR.glob("*.json"))


def envelope_errors(data: Dict[str, Any]) -> List[str]:
    errors: List[str] = []
    required = {"fixture_id", "schema_version", "scenario"}
    for key in required:
        if key not in data:
            errors.append(f"missing envelope key: {key}")
    has_records = isinstance(data.get("records"), list) and bool(data.get("records"))
    has_events = isinstance(data.get("events"), list) and bool(data.get("events"))
    if not has_records and not has_events:
        errors.append("records or events must be a non-empty list")
    if data.get("schema_version") != SCHEMA_VERSION:
        errors.append(
            f"bad schema_version: {data.get('schema_version')!r} != {SCHEMA_VERSION!r}"
        )
    if "records" in data and not (isinstance(data.get("records"), list) and data.get("records")):
        errors.append("records must be a non-empty list when present")
    return errors


def record_errors(record: Any, index: int, prefix: str = "record") -> List[str]:
    errors: List[str] = []
    if not isinstance(record, dict):
        return [f"{prefix}[{index}] is not an object"]
    if "id" not in record:
        errors.append(f"{prefix}[{index}] missing id")
    if "value_class" in record and record["value_class"] not in VALUE_CLASSES:
        errors.append(
            f"{prefix}[{index}] bad value_class: {record['value_class']!r}"
        )
    if "currency" in record and record["currency"] not in CURRENCIES:
        errors.append(f"{prefix}[{index}] bad currency: {record['currency']!r}")
    if "unit" in record and record["unit"] not in UNITS:
        errors.append(f"{prefix}[{index}] bad unit: {record['unit']!r}")
    return errors


def totals_mixing_errors(data: Dict[str, Any]) -> List[str]:
    """A total must not silently combine value classes, currencies or units."""
    errors: List[str] = []
    totals = data.get("totals", [])
    if not isinstance(totals, list):
        return ["totals must be a list when present"]
    for i, total in enumerate(totals):
        if not isinstance(total, dict):
            errors.append(f"totals[{i}] is not an object")
            continue
        for key in ("value_class", "currency", "unit"):
            if key not in total:
                errors.append(f"totals[{i}] missing {key}")
        if total.get("value_class") not in VALUE_CLASSES:
            errors.append(f"totals[{i}] bad value_class")
        if total.get("currency") not in CURRENCIES:
            errors.append(f"totals[{i}] bad currency")
        if total.get("unit") not in UNITS:
            errors.append(f"totals[{i}] bad unit")
        components = total.get("components")
        if components is not None:
            if not isinstance(components, list):
                errors.append(f"totals[{i}] components must be a list")
            else:
                # Components must all share the total's class/currency/unit.
                by_id = {r.get("id"): r for r in data.get("records", [])
                         if isinstance(r, dict)}
                for cid in components:
                    rec = by_id.get(cid)
                    if rec is None:
                        errors.append(f"totals[{i}] unknown component {cid!r}")
                        continue
                    for key in ("value_class", "currency", "unit"):
                        if key in rec and rec.get(key) != total.get(key):
                            errors.append(
                                f"totals[{i}] mixes {key}: total={total.get(key)!r} "
                                f"vs component {cid}={rec.get(key)!r}"
                            )
    # Also reject any record explicitly claiming a combined/mixed total.
    for i, rec in enumerate(data.get("records", [])):
        if isinstance(rec, dict) and rec.get("combined_total") is True:
            errors.append(f"records[{i}] declares an illegal combined_total")
    return errors


def truncated_is_incomplete(data: Dict[str, Any]) -> List[str]:
    errors: List[str] = []
    for i, rec in enumerate(data.get("records", [])):
        if not isinstance(rec, dict):
            continue
        if rec.get("truncated") is True:
            if rec.get("complete") is not False:
                errors.append(f"records[{i}] truncated but not marked incomplete")
            if rec.get("evidence_status") != "incomplete":
                errors.append(
                    f"records[{i}] truncated record must have evidence_status=incomplete"
                )
            if rec.get("phantom_income_claimed") is True:
                errors.append(f"records[{i}] truncated diagnostics imply phantom income")
    return errors


def causation_errors(data: Dict[str, Any]) -> List[str]:
    errors: List[str] = []
    events = data.get("events", [])
    edges = data.get("edges", [])
    if not isinstance(events, list) or not events:
        errors.append("causation fixture needs non-empty events")
    if not isinstance(edges, list):
        errors.append("edges must be a list")
        return errors
    event_ids = {e.get("id") for e in events if isinstance(e, dict)}
    for i, edge in enumerate(edges):
        if not isinstance(edge, dict):
            errors.append(f"edges[{i}] is not an object")
            continue
        for key in ("id", "from", "to", "relation", "provenance"):
            if key not in edge:
                errors.append(f"edges[{i}] missing {key}")
        if edge.get("from") not in event_ids:
            errors.append(f"edges[{i}] unknown source event {edge.get('from')!r}")
        if edge.get("to") not in event_ids:
            errors.append(f"edges[{i}] unknown target event {edge.get('to')!r}")
        if edge.get("relation") == "causes" and not edge.get("provenance"):
            errors.append(f"edges[{i}] causal claim without provenance")
    # Missing parents must be declared, not silently dropped.
    missing = data.get("missing_parents", [])
    if not isinstance(missing, list):
        errors.append("missing_parents must be a list when present")
    return errors
