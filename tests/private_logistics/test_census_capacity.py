"""Execute actual legacy census/slot bodies with synthetic services, not native proof."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'plugins/private_logistics/private_logistics.cpp'

def production_bodies(source):
    def function(signature):
        start = source.index(signature + '\n{')
        return source[start:source.index('\n}', start) + 2] + '\n'
    return function('static int VehSlot(void* v)') + function('static void CensusFleets(int closedDay)')

class CensusCapacityTest(unittest.TestCase):
    def exercise(self, sanitized):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'production_census.inc').write_text(production_bodies(SOURCE.read_text()))
            flags = ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie'] if sanitized else ['-O2']
            command = [compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic', *flags,
                       '-I', str(path), str(Path(__file__).with_name('census_capacity_test.cpp')), '-o', str(path / 'census')]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            env = dict(os.environ)
            if sanitized:
                env.update(ASAN_OPTIONS='detect_leaks=1:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
            result = subprocess.run([str(path / 'census')], capture_output=True, text=True, env=env)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertEqual(result.stderr, '')
            self.assertIn('8 extracted census capacity scenarios PASS', result.stdout)

    def test_optimized_production_census(self):
        self.exercise(False)

    def test_sanitized_production_census(self):
        self.exercise(True)

    def test_guard_dominates_slot_access_and_day_reconciliation_continues(self):
        source = SOURCE.read_text()
        body = production_bodies(source).split('static void CensusFleets', 1)[1]
        guard = body.index('if (s < 0 || s >= VEH_MAX)')
        self.assertLess(guard, body.index('CargoDump(s, v, closedDay)'))
        self.assertLess(guard, body.index('g_vehFuelValid[s]'))
        self.assertEqual(body.count('VehSlot(v)'), 1)
        self.assertIn('++omittedVisits;\n                continue;', body)
        self.assertEqual(body.count('legacy_census_capacity_exhausted=1'), 1)
        self.assertIn('#define VEH_MAX 64', source)
        day = source.split('static void OnDay(void*, int day, int month, int year)\n{', 1)[1].split('\n// ---', 1)[0]
        self.assertLess(day.index('CensusFleets(closedDay);'), day.index('AutoTargetsOnDay(day, closedDay);'))

if __name__ == '__main__':
    unittest.main()
