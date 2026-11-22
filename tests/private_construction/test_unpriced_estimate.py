"""D33: a construction estimate with an unpriced material is refused whole.

The native phase walk reads game memory and cannot run on the host. Its
portable pricing helper is extracted from the production source and executed
with stubbed native records; the rest is a source contract check.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'plugins/private_construction/private_construction.cpp'
RUNTIME_SOURCE = ROOT / 'plugins/private_logistics/private_logistics.cpp'


class PriceOneTest(unittest.TestCase):
    def test_unpriced_material_refuses_the_estimate(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        source = SOURCE.read_text(encoding='utf-8')
        start = source.index('#define MAX_UNPRICED_MATERIALS')
        end = source.index('\nstatic double CostFromPanelMats', start)
        price_one = source[start:end]
        harness = r'''
#include "price_guards.h"
#include <cassert>
#include <cstring>

enum Mode { OK, NO_RECORD, UNPRICED };
static Mode mode = OK;
static int logs = 0;
static void Logf(const char*, ...) { ++logs; }
static int TsmCurrencyActive(void*) { return 0; }

static void* ByName(const char* name)
{
    (void)name;
    return (mode == NO_RECORD) ? nullptr : reinterpret_cast<void*>(1);
}
static float Price(void*, int)
{
    return (mode == UNPRICED) ? 0.0f : 12.5f;
}
struct Pricing {
    void* (*getByName)(const char*);
    float (*getPrice)(void*, int);
};
static const Pricing pricing = { ByName, Price };
static const Pricing* g_pricing = &pricing;
static void* g_currency = nullptr;
''' + price_one + r'''
int main()
{
    double out = -1.0;
    mode = OK;
    assert(PriceOne("steel", 2.0f, &out) && out == 25.0);
    assert(PriceOne("steel", 0.0f, &out) && out == 0.0);

    // No price -> the whole estimate is refused. Not zero-filled, not partial.
    mode = UNPRICED;
    out = -1.0;
    assert(!PriceOne("steel", 2.0f, &out) && out == 0.0);
    assert(logs >= 1);

    // No record is also a refusal.
    mode = NO_RECORD;
    out = -1.0;
    logs = 0;
    assert(!PriceOne("bricks", 2.0f, &out) && out == 0.0);
    assert(logs == 1);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'price_one.cpp'
            exe = Path(directory) / 'price_one'
            cpp.write_text(harness, encoding='utf-8')
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                            '-I', str(ROOT / 'src'), str(cpp), '-o', str(exe)],
                           check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)


class EstimateSourceContract(unittest.TestCase):
    def test_panel_and_native_paths_refuse_unpriced_materials(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('TsmNativePriceUsable', source)
        self.assertIn('"unpriced construction material"', source)
        self.assertIn('"unpriced material in the panel estimate"', source)
        self.assertIn('the whole estimate is refused', source)
        # The old "a raw price beats refusing" rule is gone.
        self.assertNotIn('raw price still beats refusing', source)
        self.assertNotIn('its fallback table', source)

    def test_b2b_freight_refuses_the_whole_delivery(self):
        source = RUNTIME_SOURCE.read_text(encoding='utf-8')
        self.assertIn('anyUnpriced', source)
        self.assertIn('the whole ', source)
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('g_b2bUnpricedCount', source)
        self.assertIn('unpriced=%u', source)


if __name__ == '__main__':
    unittest.main()
