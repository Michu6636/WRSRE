"""Guard the enabled-by-default deployment contract (not runtime evidence)."""
import configparser
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics_probe'


class DefaultConfigTest(unittest.TestCase):
    def test_shipped_ini_is_enabled_without_synthetic_selftest(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics_probe.ini')
        self.assertEqual(config.getint('private_logistics_probe', 'enabled'), 1)
        self.assertEqual(config.getint('private_logistics_probe', 'selftest'), 0)

    def test_missing_ini_defaults_on_and_explicit_disable_is_preserved(self):
        source = (PLUGIN / 'private_logistics_probe.cpp').read_text()
        self.assertRegex(source, r'g_enabled\s*=\s*1;')
        self.assertIn('H->configInt(ini, "private_logistics_probe", "enabled", 1)', source)
        self.assertRegex(source, re.compile(
            r'if \(!g_enabled\) \{\s*LogSession\("disabled".*?return 0;', re.S))


if __name__ == '__main__':
    unittest.main()
