"""D8: the Almanac has no sample rows and shows NO DATA when a service is off.

Source contract only. The drawing test in run_host_tests.py still needs a real
ImGui checkout and is skipped on CI when that dependency is absent.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/almanac_overlay_dev'


class NoSyntheticDataTest(unittest.TestCase):
    def test_sample_tables_are_gone(self):
        view = (PLUGIN / 'almanac_panel_view.cpp').read_text(encoding='utf-8')
        for removed in ('kEconomyRows', 'kLabourRows', 'kLogisticsRows',
                        'kTabRows', 'kTabRowCounts', 'DrawTable('):
            self.assertNotIn(removed, view)
        for sample in ('1,234,567.00', '18,402', '23,118', '1,209',
                       '76 open', '412.60'):
            self.assertNotIn(sample, view)

    def test_missing_service_shows_no_data(self):
        view = (PLUGIN / 'almanac_panel_view.cpp').read_text(encoding='utf-8')
        self.assertIn('NO DATA: %s unavailable', view)
        self.assertIn('DrawNoData(kTabServiceNames[tab])', view)
        self.assertIn('kWarn', view)
        header = (PLUGIN / 'almanac_panel_view.h').read_text(encoding='utf-8')
        self.assertIn('NO DATA: live service unavailable', header)
        self.assertNotIn('Synthetic placeholder values', header)

    def test_unpriced_sales_are_an_error_fact(self):
        hook = (PLUGIN / 'graphics_hook.cpp').read_text(encoding='utf-8')
        self.assertIn('row->warn = unpriced > 0', hook)
        header = (PLUGIN / 'almanac_panel_view.h').read_text(encoding='utf-8')
        self.assertIn('bool warn = false', header)
        view = (PLUGIN / 'almanac_panel_view.cpp').read_text(encoding='utf-8')
        self.assertIn('data.rows[i].warn ? kWarn : kInk', view)
        self.assertIn('data.rows[i].warn ? kWarn : kQuiet', view)

    def test_testing_doc_no_longer_accepts_synthetic_rows(self):
        testing = (PLUGIN / 'TESTING.md').read_text(encoding='utf-8')
        self.assertIn('NO DATA: <service> unavailable', testing)
        self.assertIn('Treat any sample, synthetic or placeholder value on screen as a failure.', testing)
        self.assertNotIn('SYNTHETIC`/placeholder rows when live data is missing', testing)


if __name__ == '__main__':
    unittest.main()
