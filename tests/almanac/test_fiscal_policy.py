"""Offline C++17 test for almanac fiscal policy adapter."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins' / 'almanac'
HARNESS = Path(__file__).with_name('fiscal_policy_test.cpp')

class FiscalPolicyTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            for cand in [r'C:\w64devkit\bin\g++.exe', r'C:\msys64\ucrt64\bin\g++.exe']:
                if Path(cand).exists():
                    cls.compiler = cand
                    break
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'fiscal_policy'
        srcs = [PLUGIN / 'fiscal_policy.cpp', PLUGIN / 'almanac_shell.cpp',
                PLUGIN / 'almanac_pages.cpp', PLUGIN / 'host_validation.cpp', HARNESS]
        incs = [PLUGIN, ROOT / 'src', ROOT / 'plugins' / 'republic_bank']
        cmd = [cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
               *[f'-I{p}' for p in incs], *[str(s) for s in srcs],
               '-o', str(cls.exe)]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self):
        result = subprocess.run([str(self.exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('ALL TESTS PASSED', result.stdout)

    def test_fiscal_policy(self):
        self.run_case()

if __name__ == '__main__':
    unittest.main()
