"""D32/D34: an unpriced shop sale is free, counted and loud.

The Windows checkout hook uses MSVC SEH and cannot run here, so the numeric
decision is compiled from the production policy header. The rest is a source
contract check. No game process is launched.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/citizen_retail'
SOURCE = PLUGIN / 'citizen_retail.cpp'


class CheckoutPolicyTest(unittest.TestCase):
    def test_missing_price_is_not_turned_into_min_cost(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        harness = r'''
#include "checkout_policy.h"
#include <cassert>
#include <cmath>
using wrsre::retail::QuoteSale;

int main()
{
    // No native price -> UNPRICED, and min_cost must not manufacture a charge.
    auto q = QuoteSale(0.0f, 1.0f, 0.50f);
    assert(!q.priced && q.cost == 0.0f);
    q = QuoteSale(-4.0f, 2.0f, 99.0f);
    assert(!q.priced && q.cost == 0.0f);
    q = QuoteSale(NAN, 1.0f, 0.50f);
    assert(!q.priced && q.cost == 0.0f);

    // A priced sale still honours the configured floor.
    q = QuoteSale(10.0f, 0.01f, 0.50f);
    assert(q.priced && std::fabs(q.cost - 0.50f) < 1e-6f);

    // A normal sale is amount x native price.
    q = QuoteSale(10.0f, 2.0f, 0.50f);
    assert(q.priced && std::fabs(q.cost - 20.0f) < 1e-6f);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'checkout.cpp'
            exe = Path(directory) / 'checkout'
            cpp.write_text(harness, encoding='utf-8')
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            '-I', str(PLUGIN), '-I', str(ROOT / 'src'),
                            str(cpp), '-o', str(exe)],
                           check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)

    def test_gate_logs_and_counts_unpriced_sales(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('NoteUnpricedSale', source)
        self.assertIn('InterlockedIncrement(&g_unpricedSales)', source)
        self.assertIn('GetUnpricedSaleCountImpl', source)
        self.assertIn('TSM_CITIZEN_RETAIL_VERSION_V2', source)
        # The gate must return before the interceptor or the state charge.
        gate = source.split('static int CheckoutAffordabilityGate(', 1)[1]
        gate = gate.split('// v2.4: installs the 18-byte inline hook', 1)[0]
        unpriced = gate.index('NoteUnpricedSale')
        self.assertLess(unpriced, gate.index('g_interceptor'))

    def test_no_default_cpi_basket_remains(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertNotIn('2.15f', source)
        self.assertNotIn('c = 2.15', source)
        self.assertIn('ERROR CPI UNAVAILABLE', source)
        self.assertIn('return 0.0f;', source)
        self.assertIn('TSM_RESOURCE_PRICING_VERSION', source)


if __name__ == '__main__':
    unittest.main()
