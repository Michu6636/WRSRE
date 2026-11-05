"""Self-contained unittest coverage for Almanac UX synthetic fixtures.

These tests validate fixture schema and expected classifications only. They
do not import production modules and do not claim any runtime acceptance.
"""

from __future__ import annotations

import sys
import json
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from fixture_validation import (
    SCHEMA_VERSION,
    all_fixture_files,
    causation_errors,
    envelope_errors,
    fixture_path,
    load_fixture,
    record_errors,
    totals_mixing_errors,
    truncated_is_incomplete,
)

ROOT = Path(__file__).resolve().parent
FIXTURES = ROOT / "fixtures"

EXPECTED_FIXTURES = {
    "actual_flows.json",
    "dry_run_output.json",
    "estimates.json",
    "mixed_currencies.json",
    "truncated_diagnostics.json",
    "incomplete_residuals.json",
    "causation_provenance.json",
}


class AlmanacFixtureSchemaTests(unittest.TestCase):
    def test_expected_fixture_files_exist(self):
        present = {p.name for p in all_fixture_files()}
        self.assertTrue(EXPECTED_FIXTURES.issubset(present), present)

    def test_all_fixtures_are_valid_json(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                with path.open(encoding="utf-8") as handle:
                    data = json.load(handle)
                self.assertIsInstance(data, dict)

    def test_all_fixtures_have_valid_envelope(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                data = load_fixture(path.name)
                self.assertEqual(envelope_errors(data), [], path.name)

    def test_all_records_have_valid_schema(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                data = load_fixture(path.name)
                errors = []
                for i, record in enumerate(data.get("records", [])):
                    errors.extend(record_errors(record, i))
                self.assertEqual(errors, [], path.name)

    def test_totals_do_not_mix_classes_currencies_or_units(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                data = load_fixture(path.name)
                self.assertEqual(totals_mixing_errors(data), [], path.name)

    def test_truncated_diagnostics_are_incomplete_evidence(self):
        data = load_fixture("truncated_diagnostics.json")
        self.assertEqual(truncated_is_incomplete(data), [])

    def test_causation_fixture_schema_and_provenance(self):
        data = load_fixture("causation_provenance.json")
        self.assertEqual(causation_errors(data), [])
        expected = data["expected"]
        edge_by_id = {e["id"]: e for e in data["edges"]}
        for edge_id in expected["supported_edges"]:
            self.assertTrue(edge_by_id[edge_id]["provenance"], edge_id)
        for edge_id in expected["unsupported_edges"]:
            edge = edge_by_id[edge_id]
            self.assertTrue(
                edge["relation"] != "causes" or not edge.get("provenance"),
                edge_id,
            )
        self.assertEqual(
            {m["id"] for m in data["missing_parents"]},
            set(expected["missing_parents"]),
        )

    def test_value_classes_are_distinct_fixtures(self):
        self.assertEqual(load_fixture("actual_flows.json")["value_class"], "ACTUAL")
        self.assertEqual(load_fixture("dry_run_output.json")["value_class"], "DRY_RUN")
        self.assertEqual(load_fixture("estimates.json")["value_class"], "ESTIMATED")

    def test_dry_run_records_are_not_posted(self):
        data = load_fixture("dry_run_output.json")
        for record in data["records"]:
            self.assertFalse(record.get("posted", False))
            self.assertEqual(record["value_class"], "DRY_RUN")

    def test_fixtures_are_compact(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                self.assertLess(path.stat().st_size, 8000, path.name)

    def test_schema_version_constant(self):
        for path in all_fixture_files():
            with self.subTest(fixture=path.name):
                data = load_fixture(path.name)
                self.assertEqual(data["schema_version"], SCHEMA_VERSION)


if __name__ == "__main__":
    unittest.main()
