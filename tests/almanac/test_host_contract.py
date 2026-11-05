"""Offline C++17 test for almanac host contract validation helpers."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class HostContractTest(unittest.TestCase):
    def test_host_contract(self):
        sources = [
            ROOT / 'tests/almanac/host_contract_test.cpp',
            ROOT / 'plugins/almanac/host_validation.cpp',
        ]
        include = ROOT / 'plugins/almanac'
        with tempfile.TemporaryDirectory() as directory:
            directory = Path(directory)
            exe = directory / 'host_contract'
            compiler = shutil.which('g++') or shutil.which('clang++')
            if compiler:
                cmd = [compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                       '-I', str(include), *[str(s) for s in sources],
                       '-o', str(exe)]
                result = subprocess.run(cmd, capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stderr)
            else:
                cl = shutil.which('cl') or shutil.which('cl.exe')
                if not cl:
                    # search common VS install
                    for cand in Path('C:/Program Files/Microsoft Visual Studio').glob('*/Community/VC/Tools/MSVC/*/bin/Hostx64/x64/cl.exe'):
                        cl = str(cand)
                        break
                if not cl:
                    self.skipTest('C++ compiler required')
                vcvars = 'C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/Build/vcvars64.bat'
                cmd_str = ('call "{}" && cl /std:c++17 /EHsc /W4 /WX '
                           '/I "{}" {} /Fe"{}"').format(
                    vcvars, include, ' '.join('"' + str(s) + '"' for s in sources), exe)
                result = subprocess.run(cmd_str, shell=True, cwd=str(directory),
                                       capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            run = subprocess.run([str(exe)], capture_output=True, text=True, cwd=str(directory))
            self.assertEqual(run.returncode, 0, run.stderr + run.stdout)

if __name__ == '__main__':
    unittest.main()
