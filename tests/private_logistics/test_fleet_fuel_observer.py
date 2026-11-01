"""Panel-independent production observer; synthetic host tests, not game proof."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = Path(__file__).with_name('fleet_fuel_runtime_test.cpp')

class FleetFuelObserverTest(unittest.TestCase):
    def test_production_reader_and_runtime(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'fleet'
            result = subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                     '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                     '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('16 fleet scenarios PASS', result.stdout)

    def test_integration_and_no_economic_or_hook_writes(self):
        core = (PLUGIN / 'fleet_fuel_observer.h').read_text()
        runtime = (PLUGIN / 'fleet_fuel_runtime.h').read_text()
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        for forbidden in ('InstallInlineHook(', 'modifyCompanyCapital(', 'g_realTransfer(',
                          'TsmCurrencyCredit(', 'TsmCurrencyDebit(', 'std::vector', 'new ',
                          'malloc(', 'writeMemory(', 'PanelCapture(', 'PanelRendererDetour('):
            self.assertNotIn(forbidden, core + runtime)
        self.assertIn('FleetObserverStart();', cpp)
        self.assertIn('registerTick(FleetObserverTick,nullptr)', runtime)
        self.assertIn('!g_persistenceHandle', runtime)
        self.assertIn('if(g_billing ||', runtime)
        self.assertIn('PanelTargetMatches()', runtime)
        self.assertIn('ScheduleTargetMatches()', runtime)
        self.assertIn('panel_required=0', runtime)
        self.assertIn('exact_burn=0', runtime)
        self.assertIn('original_target_resolved=0', runtime)

if __name__ == '__main__':
    unittest.main()
