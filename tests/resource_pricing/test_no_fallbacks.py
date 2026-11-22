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
        # v1 is not published: its old non-zero contract cannot coexist with
        # UNPRICED 0.0f without a stale consumer giving goods away.
        self.assertNotIn('kApiV1', source)
        self.assertNotIn('TSM_RESOURCE_PRICING_VERSION_V1, &', source)
        # The 2.15 RUB/day default basket is gone.
        self.assertNotIn('2.15', source)
        # A missing price is explicitly 0.0f = UNPRICED.
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('return 0.0f;', source)
        # The CPI basket is unavailable rather than defaulted.
        self.assertIn('ERROR CPI UNAVAILABLE', source)

    def test_rent_keys_are_required_and_equal(self):
        ini = INI.read_text(encoding='utf-8')
        values = {}
        for line in active_ini_lines(ini):
            if '=' in line:
                key, value = line.split('=', 1)
                values[key.strip()] = value.strip()
        self.assertEqual(values.get('default_daily_rent'), '0.33')
        self.assertEqual(values.get('default_daily_rent_usd'), '0.33')
        self.assertIn('0 is rejected; there is no "no rent"', ini)

        source = SOURCE.read_text(encoding='utf-8')
        # No compiled-in RUB default, and 0 is not accepted for either key.
        self.assertIn('g_cpiDefaultRent    = 0.0f;', source)
        self.assertIn('g_cpiDefaultRentUsd = 0.0f;', source)
        self.assertNotIn('g_cpiDefaultRent = 0.33f', source)
        self.assertEqual(source.count('f > 0.0f && f <= 10000.0f && f == f'), 2)

        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        harness = r"""
#include "currency_api.h"
#include "cpi_rent.h"
#include <cassert>
#include <cstring>
#include <limits>

using wrsre::pricing::ChooseCpiRent;

static void expectMissing(int currency, float rub, float usd, const char* key)
{
    const auto r = ChooseCpiRent(currency, 0.0f, rub, usd);
    assert(!r.available);
    assert(r.missingKey != nullptr && std::strcmp(r.missingKey, key) == 0);
    assert(r.value == 0.0f);
}

int main()
{
    // Injected real rent wins in either currency, even when a key is invalid.
    assert(ChooseCpiRent(TSM_CURRENCY_RUB, 0.20f, 0.0f, 0.0f).value == 0.20f);
    assert(ChooseCpiRent(TSM_CURRENCY_USD, 0.20f, 0.0f, 0.0f).value == 0.20f);

    // Each currency uses its own required INI constant, undivided.
    const auto rub = ChooseCpiRent(TSM_CURRENCY_RUB, 0.0f, 0.33f, 0.44f);
    assert(rub.available && rub.value == 0.33f && rub.missingKey == nullptr);
    const auto usd = ChooseCpiRent(TSM_CURRENCY_USD, 0.0f, 0.33f, 0.44f);
    assert(usd.available && usd.value == 0.44f && usd.missingKey == nullptr);

    // Missing keys behave the same in both currencies.
    expectMissing(TSM_CURRENCY_RUB, 0.0f, 0.33f, "default_daily_rent");
    expectMissing(TSM_CURRENCY_USD, 0.33f, 0.0f, "default_daily_rent_usd");

    // 0 is not a "no rent" option in either currency.
    expectMissing(TSM_CURRENCY_RUB, 0.0f, 0.33f, "default_daily_rent");
    expectMissing(TSM_CURRENCY_USD, 0.33f, 0.0f, "default_daily_rent_usd");

    // Negative, NaN and implausible values are invalid like a missing key.
    const float nan = std::numeric_limits<float>::quiet_NaN();
    expectMissing(TSM_CURRENCY_RUB, -1.0f, 0.33f, "default_daily_rent");
    expectMissing(TSM_CURRENCY_USD, 0.33f, -1.0f, "default_daily_rent_usd");
    expectMissing(TSM_CURRENCY_RUB, nan, 0.33f, "default_daily_rent");
    expectMissing(TSM_CURRENCY_USD, 0.33f, nan, "default_daily_rent_usd");
    expectMissing(TSM_CURRENCY_RUB, 20000.0f, 0.33f, "default_daily_rent");
    expectMissing(TSM_CURRENCY_USD, 0.33f, 20000.0f, "default_daily_rent_usd");
    return 0;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'cpi_rent.cpp'
            exe = Path(directory) / 'cpi_rent'
            cpp.write_text(harness, encoding='utf-8')
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            '-I', str(ROOT / 'src'),
                            '-I', str(PLUGIN), str(cpp), '-o', str(exe)],
                           check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)

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
