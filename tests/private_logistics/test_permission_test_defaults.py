"""Ship the actual next-test profile in both INI and missing-key fallbacks."""
import configparser
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class PermissionTestDefaults(unittest.TestCase):
    def test_ini_and_fallback_match_permissions_test(self):
        plugin = ROOT / 'plugins/private_logistics'
        config = configparser.ConfigParser()
        config.read(plugin / 'private_logistics.ini')
        source = (plugin / 'private_logistics.cpp').read_text()
        for key, value in {'enabled': 1, 'billing': 0, 'dry_run': 1, 'auto_targets_enabled': 1, 'auto_targets_dry_run': 0, 'auto_source_percent': 10, 'auto_destination_percent': 30, 'auto_targets_log_limit': 80}.items():
            with self.subTest(key=key):
                self.assertEqual(config.getint('private_logistics', key), value)
                self.assertIn(f'H->configInt(ini, "private_logistics", "{key}", {value})', source)
        self.assertIn('permission-test profile:', source)
        self.assertIn('static int g_autoSourcePercent = 10;', source)
        self.assertIn('source threshold override=', source)

    def test_probe_defaults_match_required_capture_budget(self):
        plugin = ROOT / 'plugins/private_logistics_probe'
        config = configparser.ConfigParser()
        config.read(plugin / 'private_logistics_probe.ini')
        source = (plugin / 'private_logistics_probe.cpp').read_text()
        for key, value in {'enabled': 1, 'tick_interval': 30, 'max_snapshots': 1000, 'max_queue': 256, 'selftest': 0}.items():
            with self.subTest(key=key):
                self.assertEqual(config.getint('private_logistics_probe', key), value)
                self.assertIn(f'H->configInt(ini, "private_logistics_probe", "{key}", {value})', source)

    def test_manually_added_customs_is_not_rewritten_as_unsupported(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        start = source.index('if (AutoReadBuildingType(targetBuilding, &targetType) && AutoPreserveManualCustoms(targetType))')
        end = source.index('const int targetIndex = AutoPointerIndex(targetBuilding);', start)
        branch = source[start:end]
        self.assertIn('continue;', branch)
        self.assertNotIn('AutoSetTargetDirections', branch)
        self.assertNotIn('AutoApply', branch)
        self.assertLess(end, source.index('doConfigChanged |= AutoSetTargetDirections(entry, directions)', start))

    def test_new_and_present_targets_request_activation(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        append = source.split('static bool AutoAppendNativeTarget(', 1)[1].split('static void AutoPersistenceEvent(', 1)[0]
        self.assertIn('AutoApplyEntryActivation(entry, directions)', append)
        self.assertNotIn('*(BYTE*)entry = 0;', append)
        self.assertIn('AutoApplyEntryActivation(entry, directions)', source.split('static bool AutoSetTargetDirections(', 1)[1])
        self.assertIn('entry_enabled=%u', source)


if __name__ == '__main__':
    unittest.main()
