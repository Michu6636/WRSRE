"""Offline C++17 test for almanac UI shell."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins' / 'almanac'
HARNESS = Path(__file__).with_name('almanac_shell_test.cpp')

def find_compiler():
    c = shutil.which('g++') or shutil.which('clang++')
    if c:
        return c
    for cand in [r'C:\w64devkit\bin\g++.exe', r'C:\msys64\ucrt64\bin\g++.exe']:
        if Path(cand).exists():
            return cand
    return shutil.which('cl')

class AlmanacShellTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = find_compiler()
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'almanac_shell'
        srcs = [PLUGIN / 'almanac_shell.cpp', PLUGIN / 'almanac_pages.cpp',
                PLUGIN / 'host_validation.cpp', HARNESS]
        incs = [PLUGIN, ROOT / 'src']
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

    def test_almanac_shell(self):
        self.run_case()

if __name__ == '__main__':
    unittest.main()
