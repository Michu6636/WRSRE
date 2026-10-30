"""Production reader with synthetic memory; source guards are NOT a DLL/game test."""
from pathlib import Path
import configparser
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = ROOT / 'tests/private_logistics/panel_fuel_observer_test.cpp'


class PanelFuelObserverTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'panel_test'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, case):
        result = subprocess.run([str(self.exe), case], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + case, result.stdout)

    def test_production_runtime_with_host_shim(self):
        exe = Path(self.directory.name) / 'runtime_test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(HARNESS.with_name('panel_fuel_runtime_test.cpp')),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('8 host-shim scenarios PASS', result.stdout)

    def test_matching_test_defaults_and_no_tracker_wiring(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        self.assertEqual(config.getint('private_logistics', 'panel_fuel_observer'), 1)
        self.assertIn('H->configInt(ini, "private_logistics", "panel_fuel_observer", 1)', cpp)
        self.assertEqual(config.getint('private_logistics', 'billing'), 0)
        self.assertEqual(config.getint('private_logistics', 'dry_run'), 1)
        self.assertEqual(config.getint('private_logistics', 'auto_source_percent'), 10)
        self.assertEqual(config.getint('private_logistics', 'auto_destination_percent'), 30)
        self.assertNotIn('private_logistics_journey::Tracker', cpp)
        self.assertIn('PanelObserverStart();', cpp)
        self.assertRegex(cpp, r'if \(event == TSM_SAVE_LOADING\) \{\s*'
                         r'InterlockedIncrement\(&g_fuelDiagnosticEpoch\);\s*PanelObserverLoading\(\);')

    def test_hook_fingerprint_and_full_instruction_boundary(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        expect = runtime.split('static const BYTE expect[] = {', 1)[1].split('};', 1)[0]
        actual = bytes(int(x, 16) for x in re.findall(r'0x([0-9A-Fa-f]{2})', expect))
        # Pinned first 20 bytes from d00eaae, ending exactly after LEA RBP.
        self.assertEqual(actual, bytes.fromhex('40 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 50 D1 FF FF'))
        self.assertIn('TimeDateStamp != 0x6A3EB6ADu', runtime)
        self.assertIn('0x00, 0x00, 0x7A, 0x44', runtime)
        self.assertIn('memcmp(g_exeBase + 0x8F3E98', runtime)
        self.assertIn('&g_panelTrampoline, expect, sizeof(expect)', runtime)
        self.assertIn('!g_persistenceHandle', runtime)
        self.assertIn('if (g_billing ||', runtime)

    def test_render_path_is_bounded_nonblocking_no_io(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        render = runtime.split('static void PanelCapture(', 1)[1].split('static const char* PanelStatus(', 1)[0]
        for forbidden in ('Logf(', 'EnterCriticalSection(', 'Sleep(', 'malloc(', 'new ', 'std::vector', 'std::string'):
            self.assertNotIn(forbidden, render.replace('TryEnterCriticalSection(', 'TryLock('))
        for required in ('TryEnterCriticalSection(', 'g_panelSamples >= 240', 'now - g_panelLastMs < 1000',
                         'g_panelCount == 16', '__except', 'epoch == InterlockedCompareExchange'):
            self.assertIn(required, render)
        hook = render.split('static void __fastcall PanelRendererDetour(', 1)[1]
        self.assertLess(hook.index('((PanelRenderer)g_panelTrampoline)'), hook.index('PanelCapture(context, window)'))

    def test_reader_has_no_native_calls_or_settlements(self):
        header = (PLUGIN / 'panel_fuel_observer.h').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        for forbidden in ('windows.h', 'currency_api', 'modifyCompanyCapital', 'FuelSlotTons(',
                          'FormatSummary(', 'Consume(', 'WriteProcessMemory'):
            self.assertNotIn(forbidden, header + runtime)
        self.assertIn('physical_fuel=unverified exact_burn=0 billing=off', runtime)
        self.assertIn('unresolved_7c58', runtime)

    def test_windows_min_max_macros(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstddef>\n#include <cstdint>\n#include <cmath>\n'
                        '#define max(a,b) ((a)>(b)?(a):(b))\n#define min(a,b) ((a)<(b)?(a):(b))\n'
                        '#include "panel_fuel_observer.h"\nint main(){return 0;}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)


for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name != 'name':
        def run(self, case=name):
            self.run_case(case)
        setattr(PanelFuelObserverTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
