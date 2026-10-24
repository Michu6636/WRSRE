"""Execute the production checkbox handler tail with a stubbed native widget.

Tests logical polarity/persistence, not Windows rendering or native task enforcement.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class StorageCheckboxTest(unittest.TestCase):
    def test_production_handler(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        start = source.index('    const unsigned char oldMode = AutoStatePermissionGet(key);')
        end = source.index('\nstatic bool AutoHasNativeStorage(void* b)\n{', start)
        handler = source[start:end]  # includes the production function closing brace
        harness = r'''
#include <cassert>
#include <cstdint>
static unsigned char mode;
static unsigned drawn, desired;
static bool click;
static unsigned writes, logs;
static unsigned char AutoStatePermissionGet(uint64_t) { return mode; }
static bool AutoStatePermissionSet(uint64_t, unsigned char m) { mode=m; ++writes; return true; }
static void Logf(const char*, ...) { ++logs; }
static int Checkbox(unsigned* value, const wchar_t*, unsigned long) {
    drawn=*value;
    if (!click) return 0;
    *value=desired;
    return 1;
}
struct Api { int (*widgetCheckbox)(unsigned*, const wchar_t*, unsigned long); };
static Api api{Checkbox};
static Api* g_uiPanels=&api;
static void Handler(uint64_t key) {
''' + handler + r'''
int main() {
    for (unsigned char initial : {0, 1, 2}) {
        mode=initial; writes=logs=0; click=false;
        Handler(123);
        assert(drawn == (initial == 1 ? 1u : 0u));
        assert(mode == initial && writes == 0 && logs == 0);
        click=true; desired=(initial == 1 ? 0u : 1u);
        Handler(123);
        assert(mode == (desired ? 1 : 2) && writes == 1);
        click=false; writes=0;
        Handler(123);
        assert(drawn == desired && writes == 0);
    }
}
'''
        harness = '#include <initializer_list>\n' + harness
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'checkbox.cpp'
            exe = Path(directory) / 'checkbox'
            cpp.write_text(harness)
            subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', str(cpp), '-o', str(exe)], check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)


if __name__ == '__main__':
    unittest.main()
