"""Offline C++17 test for republic_bank bank_ledger engine."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class LedgerTest(unittest.TestCase):
    def test_ledger(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        cpp = ROOT / 'tests/republic_bank/test_ledger.cpp'
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'ledger'
            result = subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                     '-I', str(ROOT / 'plugins/republic_bank'), str(cpp),
                                     '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            run = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stderr + run.stdout)

if __name__ == '__main__':
    unittest.main()