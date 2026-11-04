"""Offline C++17 tests for ledger_reconcile.h."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins' / 'state_ledger'
HARNESS = Path(__file__).with_name('ledger_reconcile_test.cpp')

class LedgerReconcileTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'ledger_reconcile'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, name):
        result = subprocess.run([str(self.exe), name], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + name, result.stdout)

for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name == 'name':
        continue
    def run(self, case=name):
        self.run_case(case)
    setattr(LedgerReconcileTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
