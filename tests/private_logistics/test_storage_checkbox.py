"""Execute the production two-direction widget handler with a stub native UI.

This tests logical polarity, visibility by capability, and persistence, not
Windows rendering. The stub explicitly models raw 0=visually checked,
raw 1=unchecked, as observed in the supplied gameplay test. The provider's ownership/role gate has source checks below.
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
        start = source.index('static void AutoDrawStatePermissions(')
        end = source.index('\nstatic void StateStorageWidgetProvider(', start)
        handler = source[start:end]
        harness = r'''
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include "site_permissions.h"
static unsigned char mode;
static unsigned calls, drawn[2], desired[2], writes;
static bool click[2];
static unsigned char AutoStatePermissionGet(uint64_t) { return mode; }
static bool AutoStatePermissionSet(uint64_t, unsigned char m) { mode=m; ++writes; return true; }
static void Logf(const char*, ...) {}
static int Checkbox(unsigned* value, const wchar_t*, unsigned long) {
    const unsigned index=calls++;
    drawn[index]=*value;
    if (!click[index]) return 0;
    *value=desired[index]; return 1;
}
struct Api { int (*widgetCheckbox)(unsigned*, const wchar_t*, unsigned long); };
static Api api{Checkbox};
static Api* g_uiPanels=&api;
''' + handler + r'''
int main() {
    // Real widget polarity from the reported game run: 0=visual tick, 1=no tick.
    // Fresh defaults must DRAW unchecked, without relying on connection existence.
    mode=0; calls=writes=0; click[0]=click[1]=false;
    AutoDrawStatePermissions(123,AUTO_ALLOW_BOTH);
    assert(calls==2 && drawn[0]==1 && drawn[1]==1 && mode==0 && writes==0);
    // Clicking a visible tick must authorize (raw output 0), not revoke.
    calls=0; click[0]=true; desired[0]=0;
    AutoDrawStatePermissions(123,AUTO_ALLOW_UNLOAD);
    assert(mode==2 && writes==1);
    calls=writes=0; click[0]=false;
    AutoDrawStatePermissions(123,AUTO_ALLOW_UNLOAD);
    assert(drawn[0]==0 && mode==2 && writes==0);
    // Storage/factory: independently toggle either permission, preserve the other.
    for (unsigned char initial : {0,1,2,3}) {
        mode=initial; calls=writes=0; click[0]=click[1]=false;
        AutoDrawStatePermissions(123,AUTO_ALLOW_BOTH);
        assert(calls==2 && drawn[0]==(initial&1 ? 0u:1u));
        assert(drawn[1]==(initial&2 ? 0u:1u) && writes==0 && mode==initial);
        calls=0; click[0]=true; desired[0]=(initial&1) ? 1u:0u;
        AutoDrawStatePermissions(123,AUTO_ALLOW_BOTH);
        assert(mode==(initial^1) && writes==1);
        calls=writes=0; click[0]=false; click[1]=true; desired[1]=(mode&2) ? 1u:0u;
        const unsigned before=mode;
        AutoDrawStatePermissions(123,AUTO_ALLOW_BOTH);
        assert(mode==(before^2) && writes==1);
    }
    // Retail has exactly ONE delivery checkbox, never loading; old LOAD bit sanitized.
    mode=3; calls=writes=0; click[0]=click[1]=false;
    AutoDrawStatePermissions(123,AUTO_ALLOW_UNLOAD);
    assert(calls==1 && drawn[0]==0 && mode==2);
    calls=writes=0; click[0]=true; desired[0]=1;
    AutoDrawStatePermissions(123,AUTO_ALLOW_UNLOAD);
    assert(calls==1 && mode==0 && writes==1);
    // Reopen / no click: no bogus error, no write.
    calls=writes=0; click[0]=false;
    AutoDrawStatePermissions(123,AUTO_ALLOW_UNLOAD);
    assert(calls==1 && drawn[0]==1 && writes==0);
}
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'checkbox.cpp'
            exe = Path(directory) / 'checkbox'
            cpp.write_text(harness)
            result = subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-I', str(ROOT / 'plugins/private_logistics'), str(cpp), '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, capture_output=True)

    def test_private_and_unknown_buildings_do_not_show_widgets(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        body = source.split('static void StateStorageWidgetProvider(', 1)[1].split('static bool AutoHasNativeStorage(void* b)\n{', 1)[0]
        self.assertIn('g_enterprise->isBuildingPrivate(building)) return;', body)
        self.assertIn('if (!capabilities) return;', body)


if __name__ == '__main__':
    unittest.main()
