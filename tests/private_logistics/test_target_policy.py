"""Run extracted production daily reconciliation with host services/append stubs.

Also execute the production append preflight before native calls. These checks
are not native panel, allocator, save/load or dispatch validation.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
SOURCE = PLUGIN / 'private_logistics.cpp'


def extracted_bodies():
    source = SOURCE.read_text()
    signature = 'static void AutoTargetsOnDay(int day, int closedDay)\n{'
    start = source.index(signature)
    daily = source[start:source.index('\n}\n', start) + 3]
    start = source.index('static bool AutoAppendNativeTarget(void* office, void* target, unsigned directions)')
    end = source.index('    typedef void* (__fastcall *TTaskCfgCtor)', start)
    prefix = source[start:end]
    prefix = prefix.replace('AutoAppendNativeTarget(void* office, void* target, unsigned directions)',
                            'AutoAppendPrefix(void* office, void*, unsigned)')
    return daily, prefix + '    return true;\n}\n'


class TargetPolicyTest(unittest.TestCase):
    def exercise(self, sanitized):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        daily, guard = extracted_bodies()
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'production_daily.inc').write_text(daily)
            (path / 'production_append_guard.inc').write_text(guard)
            flags = ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie'] if sanitized else ['-O2']
            command = [compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic', *flags,
                       '-I', str(PLUGIN), '-I', str(path),
                       str(Path(__file__).with_name('target_policy_runtime_test.cpp')),
                       '-o', str(path / 'target_policy')]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            env = dict(os.environ)
            if sanitized:
                env.update(ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',
                           UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
            result = subprocess.run([str(path / 'target_policy')], capture_output=True, text=True, env=env)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertEqual(result.stderr, '')
            self.assertIn('6 extracted daily-reconciliation groups PASS', result.stdout)
            self.assertIn('extracted live append preflight PASS', result.stdout)

    def test_optimized_production_reconciliation(self):
        self.exercise(False)

    def test_sanitized_production_reconciliation(self):
        self.exercise(True)

    def test_live_guard_precedes_native_allocation(self):
        source = SOURCE.read_text()
        body = source.split('static bool AutoAppendNativeTarget(', 1)[1].split('\nstatic void AutoPersistenceEvent', 1)[0]
        guard = body.index('!AutoCanAppendTarget(officeType, oldCount)')
        self.assertLess(guard, body.index('::operator new'))
        self.assertLess(body.index('AutoReadTargetVector'), guard)
        self.assertLess(body.index('AutoReadBuildingType'), guard)
        self.assertIn('#include "auto_target_policy.h"', source)
        self.assertNotIn('AUTO_DO_SAFE_TARGET_COUNT', source)


if __name__ == '__main__':
    unittest.main()
