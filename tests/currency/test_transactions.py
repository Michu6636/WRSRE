"""Compile and execute currency's production transaction headers with native stubs."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class CurrencyTransactions(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            cls.exe = None
            return
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'currency_transactions'
        result = subprocess.run([
            compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pthread',
            str(ROOT / 'tests/currency/transaction_test.cpp'), '-o', str(cls.exe),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stdout + result.stderr)

    def run_case(self, case):
        if self.exe is None:
            self.skipTest('g++/clang++ C++17 compiler required for portable harness')
        result = subprocess.run([str(self.exe), str(case)], capture_output=True, text=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_full_charge_and_partial_charge_contracts(self):
        self.run_case(1)

    def test_paired_transfers_and_active_currency(self):
        self.run_case(2)

    def test_nonfinite_and_nonpositive_amounts(self):
        self.run_case(3)

    def test_unavailable_memory_and_invalid_recipients(self):
        self.run_case(4)

    def test_overflow_and_corrupt_balances(self):
        self.run_case(5)

    def test_observers_only_report_committed_treasury_changes(self):
        self.run_case(6)

    def test_concurrent_full_charges_cannot_overdraw(self):
        self.run_case(7)

    @unittest.skipUnless(os.name == 'nt' and shutil.which('cl'), 'Windows MSVC required for real SEH')
    def test_real_native_write_faults(self):
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'native_fault.exe'
            result = subprocess.run([
                'cl', '/nologo', '/std:c++17', '/EHsc', '/MT', '/W4', '/WX',
                str(ROOT / 'tests/currency/native_fault_test.cpp'),
                f'/Fe:{exe}', f'/Fo:{Path(directory) / "native_fault.obj"}',
            ], cwd=directory, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            run = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stdout + run.stderr)

    def test_state_checkout_does_not_collect_partial_payment_before_aborting(self):
        source = (ROOT / 'plugins/citizen_retail/citizen_retail.cpp').read_text()
        gate = source.split('static int CheckoutAffordabilityGate(', 1)[1].split(
            'static bool InstallCheckoutHook(', 1)[0]
        self.assertIn('if (!g_currency->citizenToState(person, cost, "state_store_sale")) return 0;', gate)
        self.assertNotIn('g_currency->citizenToStatePartial(', gate)

    def test_plugin_uses_tested_implementation_and_preserves_abi_versions(self):
        source = (ROOT / 'plugins/currency/currency.cpp').read_text()
        for name in ('native', 'wallet', 'treasury'):
            self.assertIn(f'#include "transaction_{name}.h"', source)
        self.assertIn('TSM_CURRENCY_VERSION_V3, &kApiV3', source)
        self.assertIn('TSM_CURRENCY_VERSION, &kApiMut', source)


if __name__ == '__main__':
    unittest.main()
