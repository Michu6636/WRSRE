"""B2B settlement unit tests."""
from pathlib import Path
import configparser
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'

class B2bSettlementTest(unittest.TestCase):
    def test_live_billing_is_the_shipped_default(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        source = (PLUGIN / 'private_logistics.cpp').read_text()
        self.assertEqual(config.getint('private_logistics', 'billing'), 1)
        self.assertEqual(config.getint('private_logistics', 'dry_run'), 0)
        self.assertIn('H->configInt(ini, "private_logistics", "billing", 1)', source)
        self.assertIn('H->configInt(ini, "private_logistics", "dry_run", 0)', source)
        self.assertIn('static int   g_billing         = 1;', source)
        self.assertIn('static int   g_dryRun          = 0;', source)
        self.assertIn('0.19.1-live-billing', source)

    def test_customs_export_has_no_state_buy_leg(self):
        source = (PLUGIN / 'private_logistics.cpp').read_text()
        header = (PLUGIN / 'b2b_settlement.h').read_text()
        self.assertIn('#define AUTO_CUSTOMS_TYPE 20', source)
        self.assertIn('TsmBuildingType(b) == AUTO_CUSTOMS_TYPE', source)
        self.assertIn('goodsValue, buyerCustoms)', source)
        self.assertIn('if (plan.customsExport)', source)
        self.assertIn('no state payment', source)
        self.assertIn('if(plan.buyerIsCustoms)', header)
        self.assertIn('plan.customsExport=true;', header)

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
        self.assertIn('global native price locked at loading', (PLUGIN / 'b2b_settlement.h').read_text())
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
