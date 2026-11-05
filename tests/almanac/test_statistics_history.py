"""Offline C++17 test for almanac statistics/history core."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class StatisticsHistoryTest(unittest.TestCase):
    def test_statistics_history(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        cpp = ROOT / 'tests/almanac/statistics_history_test.cpp'
        impl = ROOT / 'plugins/almanac/statistics_history.cpp'
        inc = ROOT / 'plugins/almanac'
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'statistics_history'
            result = subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
                                     '-I', str(inc), str(cpp), str(impl),
                                     '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            run = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stderr + run.stdout)
            self.assertIn('ALL TESTS PASSED', run.stdout)

if __name__ == '__main__':
    unittest.main()
