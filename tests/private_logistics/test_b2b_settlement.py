"""B2B settlement unit tests."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'

class B2bSettlementTest(unittest.TestCase):
    def test_simplified_settlement_has_no_legacy_payment_paths(self):
        source = (PLUGIN / 'private_logistics.cpp').read_text()
        config = (PLUGIN / 'private_logistics.ini').read_text()
        for retired in ('wholesale_factor', 'freight_share', 'factory_share',
                        'trip_tariff', 'tariff_margin', 'wear_litres',
                        'upkeep_per_vehicle_day', 'daily_pay_cap',
                        'pay_factory', 'pay_carrier'):
            self.assertNotIn(retired, source)
            self.assertNotIn(retired, config)
        for helper in ('FreightFor(', 'TariffFor(', 'static void Pay('):
            self.assertNotIn(helper, source)
        self.assertIn('global market quote locked at loading', (PLUGIN / 'b2b_settlement.h').read_text())
        self.assertIn('sampled positive fuel decrease', source)

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
