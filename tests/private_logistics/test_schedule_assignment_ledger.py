"""Production observation ledger tests: never native job/burn/payment proof."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = Path(__file__).with_name('schedule_assignment_ledger_test.cpp')

class AssignmentLedgerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'assignment_ledger'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, name):
        result = subprocess.run([str(self.exe), name], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + name, result.stdout)

    def test_runtime_and_money_separation(self):
        header = (PLUGIN / 'schedule_assignment_ledger.h').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        for forbidden in ('m.Get(', 'Capture(', 'currency_api', 'modifyCompanyCapital',
                          'private_logistics_journey::Tracker', 'std::vector', 'std::string',
                          'malloc(', 'new ', 'windows.h', 'reinterpret_cast'):
            self.assertNotIn(forbidden, header)
        self.assertIn('ScheduleAssignmentLog(record, d, changes, home);\n    if (!d.emit) return;', runtime)
        self.assertIn('g_scheduleLedger.Break();', runtime)
        self.assertIn('id_scope=process_local_selected_panel native_boundary=unverified', runtime)
        self.assertIn('exact_burn=0 actual_cost=not_computed settlement=none money_moved=0', runtime)
        self.assertEqual(runtime.count('InstallInlineHook('), 1)

    def test_windows_macro_compatibility(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstdint>\n#include <cmath>\n'
                        '#define min(a,b) ((a)<(b)?(a):(b))\n#define max(a,b) ((a)>(b)?(a):(b))\n'
                        '#include "schedule_assignment_ledger.h"\n'
                        'int main(){private_logistics_assignment::Ledger ledger;return ledger.Active();}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name == 'name':
        continue
    def run(self, case=name):
        self.run_case(case)
    setattr(AssignmentLedgerTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
