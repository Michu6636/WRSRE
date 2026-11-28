"""Actual portable ledger/codec/native adapter/runtime via synthetic services."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'

class DryAssignmentLedgerTest(unittest.TestCase):
    def test_production_runtime(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('g++ or clang++ host compiler required; Linux CI runs this suite')
        with tempfile.TemporaryDirectory() as directory:
            exe = Path(directory) / 'dry'
            source = str(Path(__file__).with_name('dry_assignment_runtime_test.cpp'))
            result = subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                     '-Werror', '-pedantic', '-I', str(PLUGIN), source,
                                     '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('PASS ScenarioStep3bTripPayerAndBilling', result.stdout)
            self.assertIn('PASS ScenarioStep3bDryRunNeverPaid', result.stdout)
            self.assertIn('PASS ScenarioStep3bReceiptCapacityFailsClosed', result.stdout)
            self.assertIn('PASS ScenarioStep3bReceiverIdentityAndSaveBoundaries', result.stdout)
            self.assertIn('PASS ScenarioStep3bProofOverflowFailsClosed', result.stdout)
            self.assertIn('18 dry-ledger scenarios PASS', result.stdout)
            self.assertIn('29 fleet scenarios PASS', result.stdout)

    def test_no_economic_hooks_or_pointer_wire_format(self):
        headers = [PLUGIN / f'dry_assignment_{part}.h'
                   for part in ('ledger', 'codec', 'native', 'runtime')]
        source = '\n'.join(p.read_text() for p in headers)
        for forbidden in ('modifyCompanyCapital(', 'TsmCurrencyCredit(', 'TsmCurrencyDebit(',
                          'InstallInlineHook(', 'g_realTransfer(', 'fopen('):
            self.assertNotIn(forbidden, source)
        codec = headers[1].read_text()
        for forbidden in ('uintptr_t', 'sizeof(Record)', 'sizeof(Book)', '.root', '.epoch'):
            self.assertNotIn(forbidden, codec)
        self.assertIn('payer_verified=0', source)
        self.assertIn('existing_file_retained=1', source)
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        self.assertIn('DryCostStart();', cpp)
        self.assertIn('snapshot', source)

if __name__ == '__main__':
    unittest.main()
