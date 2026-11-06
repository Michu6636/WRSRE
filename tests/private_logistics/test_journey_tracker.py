"""Native tests of the production C++ journey core, never a duplicate model.

Only synthetic event streams. No offsets, hooks, or game lifecycle are validated.
"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
HARNESS = ROOT / 'tests/private_logistics/journey_tracker_test.cpp'


class JourneyTrackerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.compiler = compiler
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.executable = Path(cls.directory.name) / 'journey_tracker_test'
        result = subprocess.run([
            compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic',
            '-I', str(ROOT / 'plugins/private_logistics'),
            str(HARNESS), '-o', str(cls.executable),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def test_runtime_wiring_remains_deferred(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        self.assertIn('#include "journey_tracker.h"', source)
        self.assertNotIn('private_logistics_journey::Tracker', source)
        self.assertNotIn('private_logistics_journey::FormatSummary', source)
        header = (ROOT / 'plugins/private_logistics/journey_tracker.h').read_text()
        for forbidden in ('windows.h', 'currency_api', 'game_world.h', 'reinterpret_cast'):
            self.assertNotIn(forbidden, header)

    def test_windows_min_max_macro_compatibility(self):
        # Emulate macro pollution from windows.h; this is not a Windows DLL build.
        source = Path(self.directory.name) / 'windows_macros.cpp'
        source.write_text('''#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#include "journey_tracker.h"
int main() { private_logistics_journey::Tracker tracker; return 0; }
''')
        result = subprocess.run([
            self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
            '-I', str(ROOT / 'plugins/private_logistics'), str(source),
            '-o', str(Path(self.directory.name) / 'windows_macros'),
        ], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def run_case(self, case):
        result = subprocess.run([str(self.executable), case], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + case, result.stdout)


def native_case(case):
    def run(self):
        self.run_case(case)
    return run


# Register every case in the C++ dispatch table, not a separately maintained list.
for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name != 'name':  # Ignore the macro definition itself.
        setattr(JourneyTrackerTest, 'test_' + name, native_case(name))


if __name__ == '__main__':
    unittest.main()
