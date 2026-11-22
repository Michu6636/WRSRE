"""D34: the City Hall CPI readout says UNAVAILABLE, never a default basket.

Source contract check. The readout is drawn by a Windows-only callback.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'plugins/taxes/taxes.cpp'


class CpiReadoutTest(unittest.TestCase):
    def test_readout_is_unavailable_without_cpi(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertIn('UNAVAILABLE (no CPI)', source)
        self.assertNotIn('n/a (no CPI)', source)
        self.assertNotIn('2.15', source)
        self.assertNotIn('default 2.15 fallback', source)
        # Both the Cost of Living and Poverty Rate rows use the same text.
        self.assertGreaterEqual(source.count('UNAVAILABLE (no CPI)'), 2)


if __name__ == '__main__':
    unittest.main()
