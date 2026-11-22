"""No fallback prices, no default basket (economy direction D8/D34, step 2).

Host-only: the Windows provider is checked by reading its source and by
compiling the portable API/price-guard headers. Nothing here launches the game.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/resource_pricing'
SOURCE = PLUGIN / 'resource_pricing.cpp'
INI = PLUGIN / 'resource_pricing.ini'


def active_ini_lines(text):
    """Lines that take effect: comments and blanks removed."""
    return [line for line in text.splitlines()
            if line.strip() and not line.lstrip().startswith(';')]


class FallbackRemovalTest(unittest.TestCase):
    def test_ini_has_no_fallback_price_or_divisor(self):
        lines = active_ini_lines(INI.read_text(encoding='utf-8'))
        joined = '\n'.join(lines)
        self.assertNotIn('[fallback_prices', joined)
        self.assertNotIn('usd_divisor', joined)
        self.assertNotIn('\ndefault ', '\n' + joined + '\n')
        self.assertNotIn('\ndefault_usd', '\n' + joined)
        self.assertNotIn('default = 500', joined)

    def test_source_has_no_fallback_table_or_default_price(self):
        source = SOURCE.read_text(encoding='utf-8')
        for removed in ('LookupFallback', 'g_fallbackCount', 'g_defaultPrice',
                        'g_defaultPriceUsd', 'g_usdDivisor',
                        'fallback_prices', 'LoadFallbacks'):
            self.assertNotIn(removed, source)
        # The 2.15 RUB/day default basket is gone.
        self.assertNotIn('2.15', source)
        # A missing price is explicitly 0.0f = UNPRICED.
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('return 0.0f;', source)
        # The CPI basket is unavailable rather than defaulted.
        self.assertIn('ERROR CPI UNAVAILABLE', source)

    def test_cpi_helper_answers_zero_when_the_tail_is_missing(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        harness = r'''
#include "resource_pricing_api.h"
#include "price_guards.h"
#include <cassert>
#include <cmath>

static float NeverCalled(float) { return 12345.0f; }

int main()
{
    // Missing provider -> UNAVAILABLE, never the old 2.15 default.
    assert(TsmPricingCpiBasket(nullptr, 0.0f) == 0.0f);

    // A v1 prefix (no CPI tail) -> UNAVAILABLE.
    TsmResourcePricingApi v1{};
    v1.structSize = (unsigned)offsetof(TsmResourcePricingApi, getCpiDailyBasket);
    assert(!TsmPricingHasCpi(&v1));
    assert(TsmPricingCpiBasket(&v1, 0.0f) == 0.0f);

    // A v6 provider that reports "no basket" stays 0; a priced one survives.
    TsmResourcePricingApi api{};
    api.structSize = sizeof(api);
    api.getCpiDailyBasket = NeverCalled;
    assert(std::fabs(TsmPricingCpiBasket(&api, 0.0f) - 12345.0f) < 1e-3);

    // The shared UNPRICED predicate: 0, negative, NaN and Inf are unusable.
    assert(!TsmNativePriceUsable(0.0f));
    assert(!TsmNativePriceUsable(-1.0f));
    assert(!TsmNativePriceUsable(NAN));
    assert(!TsmNativePriceUsable(INFINITY));
    assert(TsmNativePriceUsable(0.02f));
    assert(TsmNativePriceUsable(350.0f));
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'cpi.cpp'
            exe = Path(directory) / 'cpi'
            cpp.write_text(harness, encoding='utf-8')
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            '-I', str(ROOT / 'src'), str(cpp), '-o', str(exe)],
                           check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)


if __name__ == '__main__':
    unittest.main()
