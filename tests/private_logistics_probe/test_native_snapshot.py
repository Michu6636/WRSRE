"""Compile the shared production serializer and consume AFTER producer exit.

This executes portable C++ on the host. It does NOT build the Windows DLL or
validate game offsets, Windows threading or gameplay observations.
"""
import pathlib
import shutil
import subprocess
import tempfile
import unittest
from tests.private_logistics_probe.probe_parser import parse_probe_log
from tests.private_logistics_probe.probe_schema import ProbeSnapshot

ROOT = pathlib.Path(__file__).resolve().parents[2]

class TestNativeSnapshot(unittest.TestCase):
    def test_owned_payload_and_production_serializer_roundtrip(self):
        compiler = shutil.which("g++") or shutil.which("clang++")
        if not compiler:
            self.skipTest("Host C++ compiler unavailable; native test not executed")
        with tempfile.TemporaryDirectory() as temp:
            binary = pathlib.Path(temp) / "snapshot_test"
            subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                            str(ROOT / "tests/private_logistics_probe/snapshot_serialization_test.cpp"),
                            "-o", str(binary)], check=True, capture_output=True, text=True)
            output = subprocess.check_output([str(binary)], text=True)
        parsed = parse_probe_log(output)
        snapshots = [r.kind_record for r in parsed if isinstance(r.kind_record, ProbeSnapshot)]
        self.assertEqual(len(snapshots), 1, repr(parsed))
        r = snapshots[0]
        self.assertEqual(r.header.seq, 1001)
        self.assertEqual(r.record_type, "measurement")
        self.assertEqual(r.state, 17)
        self.assertEqual(r.mode, 9)
        self.assertEqual(r.kind, 66)
        self.assertEqual(r.cargo_slots, 3)
        self.assertAlmostEqual(r.fuel_tons, 9.876543)
        self.assertAlmostEqual(r.cargo_total_tons, 4.567890)
        self.assertEqual(r.cargo, "fuel=9.876t ore=2.345t goods=1.234t")
        self.assertEqual(r.mirror_valid, 0)
        self.assertEqual(r.home_matches, 1)
        self.assertIn(" observation_seq=71 ", output)
