"""Configuration contract only; no claim of Windows/game validation."""
import configparser
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_shops'


class RestockDefaultsTest(unittest.TestCase):
    def test_shipped_ini_opts_out_of_direct_foreign_restock(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_shops.ini')
        self.assertEqual(config.getint('private_shops', 'foreign_restock_enabled'), 0)

    def test_missing_config_also_opts_out_and_explicit_opt_in_remains(self):
        source = (PLUGIN / 'private_shops.cpp').read_text()
        self.assertRegex(source, r'g_foreignEnabled\s*=\s*0;')
        self.assertIn('H->configInt(ini, "private_shops", "foreign_restock_enabled", 0)', source)
        self.assertRegex(source, re.compile(
            r'static void RunForeignRestock\([^)]*\)\s*\{\s*if \(!g_foreignEnabled\) return;', re.S))


if __name__ == '__main__':
    unittest.main()
