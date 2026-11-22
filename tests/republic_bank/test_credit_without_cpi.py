"""D34: no CPI means no economic index and no new credit.

The scale function is portable (it reads the resource-pricing API and one
reference constant), so the test executes the production source with a stubbed
provider. The refusal path itself is checked in the source.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'plugins/republic_bank/republic_bank.cpp'
INI = ROOT / 'plugins/republic_bank/republic_bank.ini'


class EconomicScaleTest(unittest.TestCase):
    def test_unavailable_cpi_answers_zero(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        source = SOURCE.read_text(encoding='utf-8')
        start = source.index('static double CurrentEconomicScale(void) {')
        end = source.index('\n}\n', start) + 3
        function = source[start:end]
        harness = r'''
#include "resource_pricing_api.h"
#include <cassert>
#include <cmath>

static double g_referenceCpiRub = 2.15;
static float g_cpi = 0.0f;
static float Basket(float) { return g_cpi; }
static TsmResourcePricingApi g_api{};
static const TsmResourcePricingApi* g_pricing = nullptr;
''' + function + r'''
int main()
{
    // No provider / no CPI tail -> UNAVAILABLE (0), not a neutral 1.0.
    assert(CurrentEconomicScale() == 0.0);

    // A provider that cannot price the basket -> UNAVAILABLE.
    g_api.structSize = sizeof(g_api);
    g_api.getCpiDailyBasket = Basket;
    g_pricing = &g_api;
    g_cpi = 0.0f;
    assert(CurrentEconomicScale() == 0.0);

    // A real basket keeps the documented reference indexing.
    g_cpi = 4.30f;
    assert(std::fabs(CurrentEconomicScale() - 2.0) < 1e-6);
    g_cpi = 1.075f;
    assert(std::fabs(CurrentEconomicScale() - 0.5) < 1e-6);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'scale.cpp'
            exe = Path(directory) / 'scale'
            cpp.write_text(harness, encoding='utf-8')
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            '-I', str(ROOT / 'src'), str(cpp), '-o', str(exe)],
                           check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)


class CreditRefusalContract(unittest.TestCase):
    def test_new_credit_is_refused_when_scale_is_unavailable(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertIn('NoteNoCreditWithoutCpi', source)
        self.assertIn('ERROR CPI UNAVAILABLE', source)
        draw = source.split('static double requestCreditDrawImpl', 1)[1]
        draw = draw.split('static void repayPrincipalImpl', 1)[0]
        check = draw.index('if (!(scale > 0.0))')
        self.assertLess(check, draw.index('requestCreditDraw('))

        daily = source.split('static void onDay(', 1)[1]
        daily = daily.split('\n}\n', 1)[0]
        self.assertIn('if (!(scale > 0.0)) return;', daily)

    def test_no_usd_divisor_fallback_in_the_bank(self):
        source = SOURCE.read_text(encoding='utf-8')
        ledger = (ROOT / 'plugins/republic_bank/bank_ledger.h').read_text(encoding='utf-8')
        ini = INI.read_text(encoding='utf-8')
        active = '\n'.join(line for line in ini.splitlines()
                           if line.strip() and not line.lstrip().startswith(';'))
        self.assertNotIn('usd_divisor', source)
        self.assertNotIn('usd_divisor', active)
        self.assertNotIn('fallbackScale', ledger)
        self.assertNotIn('return fallback', ledger)


if __name__ == '__main__':
    unittest.main()
