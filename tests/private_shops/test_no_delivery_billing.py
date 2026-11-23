"""Step 3 contract: private_shops charges services only.

This is a source/INI contract test. It does not run the Windows plugin.
"""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_shops'


def code_only(text):
    return '\n'.join(line for line in text.splitlines()
                     if not line.lstrip().startswith('//'))


class NoDeliveryBillingTest(unittest.TestCase):
    def test_plugin_has_no_wholesale_or_foreign_path(self):
        source = code_only((PLUGIN / 'private_shops.cpp').read_text())
        for removed in ('g_wholesaleRatio', 'WholesaleSlot', 'WholesaleContext',
                        'RunWholesaleBilling', 'chargeWholesaleDelivery',
                        'g_foreignEnabled', 'g_foreignTargetRatio',
                        'g_foreignMarkup', 'g_restockReorderPoint',
                        'RunForeignRestock', 'RestockSlot', 'RestockContext',
                        'TSM_SLOT_CONTENT', 'NoteUnpricedShop',
                        'ForEachSlot', 'SlotSnapshot', 'ShopState'):
            with self.subTest(removed=removed):
                self.assertNotIn(removed, source)

    def test_only_service_fees_move_money(self):
        source = code_only((PLUGIN / 'private_shops.cpp').read_text())
        tick = source.split('static void RunShopTick()', 1)[1].split(
            '// ---------------------------------------------------------------- persistence', 1)[0]
        self.assertIn('GuardedService(b, type, *fee, day);', tick)
        self.assertNotIn('GuardedRestock', tick)
        self.assertNotIn('GuardedWholesale', tick)
        self.assertIn('static void RunServiceCharge(', source)
        self.assertIn('chargeCitizenPartial', source)

    def test_removed_ini_keys_are_not_read(self):
        source = (PLUGIN / 'private_shops.cpp').read_text()
        ini = (PLUGIN / 'private_shops.ini').read_text()
        for key in ('wholesale_ratio', 'foreign_restock_enabled',
                    'foreign_restock_target', 'foreign_restock_markup',
                    'restock_reorder_point', 'restock_types'):
            with self.subTest(key=key):
                self.assertNotIn(f'"{key}"', source)
                self.assertNotRegex(ini, re.compile(rf'^\s*{key}\s*=', re.M))

    def test_service_fee_defaults_survive(self):
        ini = (PLUGIN / 'private_shops.ini').read_text()
        source = (PLUGIN / 'private_shops.cpp').read_text()
        self.assertRegex(ini, re.compile(r'^service_fees\s*=\s*23:3:3\.5,\s*24:3:3\.5$', re.M))
        self.assertIn('info->version = "3.8";', source)
        self.assertIn('no delivery bill, no foreign restock', source)


if __name__ == '__main__':
    unittest.main()
