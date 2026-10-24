"""Native execution of the production classifier, migration and cfg writer.

No duplicated TaskCfg implementation; no Windows/game planner claim.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

HARNESS = r'''
#include <cassert>
#include <cstring>
#include "site_permissions.h"
static uintptr_t Ptr(const unsigned char* p) { uintptr_t v; memcpy(&v,p,8); return v; }
static float Float(const unsigned char* p) { float v; memcpy(&v,p,4); return v; }
static void SetPtr(unsigned char* p, uintptr_t v) { memcpy(p,&v,8); }
int main(int argc, char**) {
    assert(argc==1);
    // Same numeric type, different verified asset: consumer vs warehouse.
    assert(AutoSiteCapabilities(3,"MIRRORZ_DLC3_small_store")==2);
    assert(AutoSiteCapabilities(3,"2119881153/sklep3")==2);
    assert(AutoSiteCapabilities(3,"warehouse_covered_combined")==3);
    assert(AutoSiteCapabilities(3,"path/warehouse_open")==3);
    assert(AutoSiteCapabilities(5,"modded_storage")==3);
    assert(AutoSiteCapabilities(6,"modded_factory")==3);
    assert(AutoSiteCapabilities(23,"modded_pub")==2);
    assert(AutoSiteCapabilities(43,"muddy_distribution")==0);
    assert(AutoSiteCapabilities(20,"customs")==0);
    assert(AutoSiteCapabilities(0,"substation")==0);
    assert(AutoSiteCapabilities(999,"something_warehouse_ish")==0);
    assert(AutoSiteCapabilities(999,nullptr)==0);
    // Save-format migration: old explicit OFF can never become UNLOAD.
    for (unsigned i=0; i<256; ++i) {
        assert(AutoMigratePermission((unsigned char)i,true)==(i==1 ? 1:0));
        assert(AutoMigratePermission((unsigned char)i,false)==(i<=3 ? i:0));
    }
    for (unsigned caps=0; caps<4; ++caps)
        for (unsigned mask=0; mask<4; ++mask) {
            assert(AutoEffectiveDirections(true,mask,caps)==caps);
            assert(AutoEffectiveDirections(false,mask,caps)==(mask&caps));
            assert(AutoPermissionToggle(mask,1,1,caps)==((mask|1)&caps));
            assert(AutoPermissionToggle(mask,2,0,caps)==((mask&~2u)&caps));
        }
    // Existing native resource lists: deny must clear the selector's END,
    // not just set percentage=0 or disable wildcard while retaining a list.
    for (unsigned mask=0; mask<4; ++mask) {
        alignas(8) unsigned char cfg[0x198]; memset(cfg,0xA5,sizeof(cfg));
        for (unsigned side=0; side<2; ++side) {
            const unsigned off=side ? 0xC8:0;
            cfg[off]=1;
            float f=0.75f; memcpy(cfg+off+4,&f,4);
            SetPtr(cfg+off+8,0x1000);
            SetPtr(cfg+off+16,0x1020);
            SetPtr(cfg+off+24,0x1100);
        }
        unsigned char before[sizeof(cfg)]; memcpy(before,cfg,sizeof(cfg));
        assert(AutoApplyTaskDirections(cfg,mask));
        assert(!AutoApplyTaskDirections(cfg,mask)); // idempotent reconciliation
        for (unsigned side=0; side<2; ++side) {
            const unsigned off=side ? 0xC8:0;
            const bool enabled=(mask & (side ? 2:1))!=0;
            assert(cfg[off]==(enabled ? 1:0));
            assert(Float(cfg+off+4)==(enabled ? 1.0f:0.0f));
            assert(Ptr(cfg+off+8)==0x1000 && Ptr(cfg+off+24)==0x1100);
            assert(Ptr(cfg+off+16)==(enabled ? 0x1020:0x1000));
        }
        // No unrelated byte changes (native active/validated flags included).
        for (unsigned i=0;i<sizeof(cfg);++i) {
            const bool writable=(i==0 || (i>=4 && i<8) || (i>=16 && i<24) ||
                                 i==0xC8 || (i>=0xCC && i<0xD0) || (i>=0xD8 && i<0xE0));
            if (!writable) assert(cfg[i]==before[i]);
        }
        AutoApplyTaskDirections(cfg,0);
        AutoApplyTaskDirections(cfg,3);
        assert(cfg[0]==1 && cfg[0xC8]==1); // re-allow without resurrecting old filters
        assert(Ptr(cfg+16)==Ptr(cfg+8) && Ptr(cfg+0xD8)==Ptr(cfg+0xD0));
    }
}
'''


class SitePermissionsTest(unittest.TestCase):
    def test_native_policy_and_taskcfg(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'permissions.cpp'
            exe = Path(directory) / 'permissions'
            cpp.write_text(HARNESS)
            result = subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-I', str(ROOT / 'plugins/private_logistics'), str(cpp), '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, capture_output=True)

    def test_production_integration_guards(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        self.assertIn('memcpy(data, "PLSTOR18", 8)', source)
        self.assertIn('AutoMigratePermission(*((BYTE*)raw + at++), legacy)', source)
        self.assertIn('AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH)', source)
        self.assertNotIn('g_stateStorageForceOn', source)
        self.assertNotIn('target.statePermission != 0', source)  # default OFF must also be enforced
        self.assertNotIn('candidateCount == 0', source)  # still revoke old entries when no allowed candidates
        self.assertIn('doConfigChanged |= AutoSetTargetDirections(entry, directions)', source)
        wrapper = source.split('static bool AutoSetTargetDirections(',1)[1].split('static void AutoTargetsOnDay(int day, int closedDay)\n{',1)[0]
        self.assertIn('AutoTaskConfigValid(cfg)', wrapper)
        self.assertNotIn('*(BYTE*)entry = 1', wrapper)
