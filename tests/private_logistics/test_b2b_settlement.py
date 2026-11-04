"""B2B settlement unit tests."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'

class B2bSettlementTest(unittest.TestCase):
    def test_production_runtime(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            # try cl
            compiler = shutil.which('cl')
        if not compiler:
            self.skipTest('C++ compiler required')
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'b2b'
            if compiler.endswith('cl') or compiler.endswith('cl.exe'):
                result = subprocess.run([compiler, '/std:c++17', '/EHsc', '/W4',
                                         f'/I{PLUGIN}', str(Path(__file__).with_name('b2b_settlement_test.cpp')),
                                         f'/Fe:{exe}'], capture_output=True, text=True)
            else:
                result = subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                         '-Werror', '-pedantic', '-I', str(PLUGIN),
                                         str(Path(__file__).with_name('b2b_settlement_test.cpp')),
                                         '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('ALL B2B SCENARIOS PASS', result.stdout)

if __name__ == '__main__':
    unittest.main()
