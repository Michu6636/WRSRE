"""Source-contract guards, not native reachability or in-game behaviour tests."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class AutomaticReconnectionTest(unittest.TestCase):
    def setUp(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        self.sync = source.split('static void AutoTargetsOnDay(int day, int closedDay)\n{', 1)[1].split('\n// ---', 1)[0]

    def test_missing_generated_target_is_not_a_permanent_veto(self):
        self.assertNotIn('memo.state = 2', self.sync)
        self.assertNotIn('g_autoMemo[memoIndex].state == 2', self.sync)
        self.assertNotIn('suppressing retry', self.sync)
        self.assertIn('memo.state = 3;', self.sync)
        self.assertIn('eligible for automatic reconnection', self.sync)

    def test_existing_targets_are_still_deduplicated(self):
        self.assertIn('if (target.targetPresent)', self.sync)
        self.assertIn('++existing; ++doExisting; continue;', self.sync)
        self.assertIn('g_autoMemo[memoIndex].state = 1;', self.sync)

    def test_state_opt_in_and_native_capacity_guards_are_not_removed(self):
        self.assertIn('e.hasStorage = site.permission != 0;', self.sync)
        self.assertIn('if (!target.hasStorage || target.building == office.building) continue;', self.sync)
        self.assertIn('entryCountAtStart + projectedAdds >= AUTO_DO_SAFE_TARGET_COUNT', self.sync)


if __name__ == '__main__':
    unittest.main()
