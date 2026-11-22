"""D8/D34: the destitute bread-day never substitutes a missing food price.

Source contract check; the rent pass itself needs the Windows game.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'plugins/enterprise_base/enterprise_base.cpp'


class DestituteBreadTest(unittest.TestCase):
    def test_unpriced_food_skips_the_stage_loudly(self):
        source = SOURCE.read_text(encoding='utf-8')
        self.assertIn('ERROR UNPRICED', source)
        self.assertIn('TsmNativePriceUsable(pFood)', source)
        self.assertIn('the destitute bread-day is unavailable', source)
        # The old silent "0 disables the stage" wording must not hide UNPRICED.
        self.assertNotIn('v9.20: no market multiplier. 0 disables the stage.', source)


if __name__ == '__main__':
    unittest.main()
