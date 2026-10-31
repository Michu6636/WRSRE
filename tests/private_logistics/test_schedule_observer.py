"""Host/source tests for sampled schedule observations, NOT native job evidence."""
from pathlib import Path
import configparser
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = Path(__file__).with_name('schedule_observer_test.cpp')

class ScheduleObserverTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'schedule_test'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, name):
        result = subprocess.run([str(self.exe), name], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + name, result.stdout)

    def test_production_runtime_enabled(self):
        exe = Path(self.directory.name) / 'runtime_test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(HARNESS.with_name('schedule_runtime_test.cpp')),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('16 schedule runtime host-shim scenarios PASS', result.stdout)

    def test_native_fingerprints_match_existing_raw_bytes(self):
        raw = (ROOT / 'docs/private-logistics-runtime/RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md').read_bytes().decode('utf-16')
        instructions = {}
        for line in raw.splitlines():
            m = re.match(r'^(140[0-9a-f]+):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+', line)
            if m:
                instructions[int(m[1], 16)] = bytes.fromhex(m[2])
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        records = re.findall(r'\{(0x[0-9A-F]+), (\d+), \{([^}]+)\}\}', runtime)
        self.assertEqual(len(records), 13)
        for rva, size, values in records:
            expected = bytes(int(x, 16) for x in re.findall(r'0x[0-9A-F]{2}', values))
            address = int(rva, 16) + 0x140000000
            end = address + int(size)
            actual = b''
            while address < end:
                chunk = instructions[address]
                actual += chunk
                address += len(chunk)
            self.assertEqual(address, end)  # whole instructions
            self.assertEqual(len(expected), int(size))
            self.assertEqual(actual, expected)

    def test_defaults_and_read_only_separation(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        header = (PLUGIN / 'schedule_observer.h').read_text()
        self.assertEqual(config.getint('private_logistics', 'schedule_observer'), 1)
        self.assertIn('static int g_scheduleObserverEnabled = 1;', cpp)
        self.assertIn('H->configInt(ini, "private_logistics", "schedule_observer", 1)', cpp)
        for forbidden in ('modifyCompanyCapital', 'private_logistics_journey::Tracker',
                          'WriteProcessMemory', 'FuelSlotTons(', 'reinterpret_cast', 'new ',
                          'std::vector', 'std::string'):
            self.assertNotIn(forbidden, header)
        self.assertIn('if (s.status != private_logistics_schedule::Status::Complete) return;', runtime)
        self.assertIn('native_job_id=unverified buyer=unverified', runtime)
        self.assertIn('completion=unverified exact_burn=0 billing=off', runtime)
        self.assertIn('g_scheduleObserverEnabled && ScheduleTargetMatches()', runtime)
        self.assertEqual(runtime.count('InstallInlineHook('), 1)  # same existing hook

    def test_gap_endpoint_source_contract(self):
        header = (PLUGIN / 'schedule_observer.h').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        history = header.split('class History {', 1)[1]
        self.assertNotIn('m.Get(', history)  # cached-value comparisons only
        self.assertEqual(header.count('m.Get('), 13)  # native read sites unchanged
        self.assertIn('0.18.11-assignment-ledger', cpp)
        self.assertIn('if (d.endpointCompared && d.endpointAcrossGap)', runtime)
        self.assertIn('compared_across_gap=1 continuity_valid=0', runtime)
        self.assertIn('native_event_time=unknown', runtime)
        self.assertIn('[PANEL SCHEDULE PREVIOUS ROW]', runtime)
        self.assertIn('record.sequence, record.milliseconds', runtime)

    def test_windows_min_max_macros(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstddef>\n#include <cstdint>\n#include <cmath>\n'
                        '#define max(a,b) ((a)>(b)?(a):(b))\n#define min(a,b) ((a)<(b)?(a):(b))\n'
                        '#include "schedule_observer.h"\nint main(){return 0;}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    def run(self, case=name):
        self.run_case(case)
    setattr(ScheduleObserverTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
