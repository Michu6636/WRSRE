"""Execute the production wholesale settlement tail, including its delta guard.

Native game APIs are stubbed; this does not test Windows hooks or game rendering.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class WholesaleDebtTest(unittest.TestCase):
    def test_deliveries_preserve_goods_and_charge_into_debt_once(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        source = (ROOT / 'plugins/private_shops/private_shops.cpp').read_text()
        start = source.index('    float delta = content - snap.content;', source.index('static void WholesaleSlot('))
        end = source.index('\nstatic void RunWholesaleBilling(', start)
        tail = source[start:end]
        harness = r'''
#include <cassert>
#include <cmath>
#include <initializer_list>
using BYTE = unsigned char;
static double balance, treasury;
static unsigned charges;
static float price=10.0f;
static float g_wholesaleRatio=0.7f;
static int g_currency;
static int TsmCurrencyActive(int) { return 0; }
static void TsmPricingRecordDemand(void*, void*, double) {}
static float Price(void*, int) { return price; }
struct Pricing { float (*getPrice)(void*, int); } pricing{Price};
static Pricing* g_pricing=&pricing;
static void Charge(void*, double cost) {
    balance-=cost; treasury+=cost; ++charges;
}
struct Enterprise { void (*chargeWholesaleDelivery)(void*, double); } enterprise{Charge};
static Enterprise* g_enterprise=&enterprise;
struct Snapshot { void* resource; float content; };
struct Context { void* shop; double capital; double charged; unsigned billed; };
static void Settle(float content, Snapshot& snap, Context* ctx) {
    bool isExport=false, isImport=false;
    void* res=snap.resource;
''' + tail + r'''
static bool Near(double a, double b) { return std::abs(a-b)<0.001; }
int main() {
    for (double initial : {100.0, 5.0, 0.0, -10.0}) {
        balance=initial; treasury=0; charges=0;
        Snapshot snap{reinterpret_cast<void*>(1), 1.0f};
        Context ctx{reinterpret_cast<void*>(2), initial, 0, 0};
        // Inventory grew from 1 to 5 t: all 4 t cost 28. No affordability cap.
        float shelf=5.0f;
        Settle(shelf,snap,&ctx);
        assert(shelf==5.0f && snap.content==5.0f);
        assert(Near(balance,initial-28.0) && Near(ctx.capital,balance));
        assert(Near(treasury,28.0) && Near(ctx.charged,28.0));
        assert(charges==1 && ctx.billed==1);
        // Repeated tick with unchanged stock does not bill again.
        Settle(shelf,snap,&ctx);
        assert(charges==1 && Near(treasury,28.0));
        // Consumption is not another delivery.
        shelf=3.0f; Settle(shelf,snap,&ctx);
        assert(charges==1 && snap.content==3.0f);
        // A later fragment is billed only for the new tonnage, still on debt.
        shelf=3.5f; Settle(shelf,snap,&ctx);
        assert(charges==2 && Near(treasury,31.5));
        assert(Near(balance,initial-31.5) && shelf==3.5f);
    }
    // Missing/zero price does not modify inventory or create a charge.
    price=0; charges=0;
    Snapshot snap{reinterpret_cast<void*>(1),0}; Context ctx{nullptr,0,0,0};
    Settle(4,snap,&ctx);
    assert(charges==0 && snap.content==4);
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'wholesale.cpp'
            exe = Path(directory) / 'wholesale'
            cpp.write_text(harness)
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', str(cpp), '-o', str(exe)], check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)

    def test_live_caller_cannot_select_inventory_rollback(self):
        source = (ROOT / 'plugins/private_shops/private_shops.cpp').read_text()
        wholesale = source.split('static void WholesaleSlot(', 1)[1].split('// ---------------------------------------------------------------- service fees', 1)[0]
        self.assertNotIn('TSM_SLOT_CONTENT', wholesale)
        self.assertNotIn('allowDebt', wholesale)
        self.assertIn('RunWholesaleBilling(b, *st);', source)


if __name__ == '__main__':
    unittest.main()
