"""Offline C++17 integration tests for the Almanac root composition."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins' / 'almanac'
HARNESS = Path(__file__).with_name('almanac_composition_test.cpp')


class AlmanacCompositionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            for candidate in [r'C:\w64devkit\bin\g++.exe',
                              r'C:\msys64\ucrt64\bin\g++.exe']:
                if Path(candidate).exists():
                    cls.compiler = candidate
                    break
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'almanac_composition'
        sources = [
            HARNESS,
            PLUGIN / 'almanac_composition.cpp',
            PLUGIN / 'almanac_page_providers.cpp',
            PLUGIN / 'almanac_shell.cpp',
            PLUGIN / 'almanac_pages.cpp',
            PLUGIN / 'host_validation.cpp',
            PLUGIN / 'statistics_history.cpp',
            PLUGIN / 'causation_ledger.cpp',
            PLUGIN / 'fiscal_policy.cpp',
        ]
        includes = [PLUGIN, ROOT / 'src', ROOT / 'plugins' / 'republic_bank']
        command = [cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                   '-Werror', '-pthread', *[f'-I{path}' for path in includes],
                   *[str(source) for source in sources], '-o', str(cls.exe)]
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def test_composition(self):
        result = subprocess.run([str(self.exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('ALL TESTS PASSED', result.stdout)


if __name__ == '__main__':
    unittest.main()
