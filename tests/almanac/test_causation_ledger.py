"""Offline C++17 tests for causation_ledger engine."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins' / 'almanac'
HARNESS = Path(__file__).with_name('causation_ledger_test.cpp')

def find_compiler():
    c = shutil.which('g++') or shutil.which('clang++')
    if c:
        return c
    for cand in [r'C:\w64devkit\bin\g++.exe', r'C:\msys64\ucrt64\bin\g++.exe']:
        if Path(cand).exists():
            return cand
    return shutil.which('cl')

class CausationLedgerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = find_compiler()
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'causation_ledger'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(PLUGIN / 'causation_ledger.cpp'), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, name):
        result = subprocess.run([str(self.exe), name], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + name, result.stdout)

for name in re.findall(r'if \(std::strcmp\(name, "(\w+)"\) == 0\)', HARNESS.read_text()):
    def run(self, case=name):
        self.run_case(case)
    setattr(CausationLedgerTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
