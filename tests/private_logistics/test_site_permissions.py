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
#include <initializer_list>
#include "site_permissions.h"
static uintptr_t Ptr(const unsigned char* p) { uintptr_t v; memcpy(&v,p,8); return v; }
static float Float(const unsigned char* p) { float v; memcpy(&v,p,4); return v; }
static void SetPtr(unsigned char* p, uintptr_t v) { memcpy(p,&v,8); }
int main(int argc, char**) {
    assert(argc==1);
    assert(AutoNativeCheckboxValue(false)==1 && AutoNativeCheckboxValue(true)==0);
    assert(AutoNativeCheckboxChecked(0) && !AutoNativeCheckboxChecked(1));
    assert(AutoPreserveManualCustoms(20));
    assert(!AutoPreserveManualCustoms(3) && !AutoPreserveManualCustoms(5));
    // Activation is separate from selectors; no entry metadata may change.
    for (unsigned mask=0;mask<4;++mask) {
        unsigned char entry[0x18]; memset(entry,0xA5,sizeof(entry));
        assert(AutoApplyEntryActivation(entry,mask));
        assert(entry[0]==(mask ? 1:0));
        assert(!AutoApplyEntryActivation(entry,mask));
        for (unsigned i=1;i<sizeof(entry);++i) assert(entry[i]==0xA5);
        AutoApplyEntryActivation(entry,0); assert(entry[0]==0);
        AutoApplyEntryActivation(entry,mask); assert(entry[0]==(mask ? 1:0));
    }
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
    // Under construction: no load, no unload, whatever the permission says;
    // finished buildings keep exactly their effective directions.
    for (unsigned d=0; d<8; ++d) {
        assert(AutoDirectionsForBuildState(d,false)==0);
        assert(AutoDirectionsForBuildState(d,true)==(d&3u));
    }
    // Legacy 100%/90% or player-adjusted values converge to company defaults,
    // then daily reconciliation is idempotent instead of fighting native clamp.
    alignas(8) unsigned char legacy[0x198] = {};
    legacy[0]=legacy[0xC8]=1;
    for (float old : {1.0f,0.90f,0.19f}) {
        memcpy(legacy+4,&old,4); memcpy(legacy+0xCC,&old,4);
        assert(AutoApplyTaskDirections(legacy,3));
        assert(Float(legacy+4)==0.10f && Float(legacy+0xCC)==0.30f);
        assert(!AutoApplyTaskDirections(legacy,3));
    }
    for (int load : {-100,0,20,90,100,10000})
        for (int unload : {-100,0,10,30,100,10000})
            for (unsigned mask=0; mask<4; ++mask) {
                const float beforeLoad=Float(legacy+4), beforeUnload=Float(legacy+0xCC);
                AutoApplyTaskDirections(legacy,mask,load,unload);
                assert(Float(legacy+4)==((mask&1) ? AutoClampSourcePercent(load)/100.0f : beforeLoad));
                assert(Float(legacy+0xCC)==((mask&2) ? AutoClampDestinationPercent(unload)/100.0f : beforeUnload));
                assert(!AutoApplyTaskDirections(legacy,mask,load,unload));
            }
    assert(AutoClampSourcePercent(-1)==0 && AutoClampSourcePercent(100)==90);
    assert(AutoClampDestinationPercent(-1)==10 && AutoClampDestinationPercent(101)==100);
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
            assert(Float(cfg+off+4)==(enabled ? (side ? 0.30f:0.10f):0.75f));
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
        self.assertEqual(source.count('AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent)'), 2)
        self.assertNotIn('g_stateStorageForceOn', source)
        self.assertNotIn('target.statePermission != 0', source)  # default OFF must also be enforced
        self.assertNotIn('candidateCount == 0', source)  # still revoke old entries when no allowed candidates
        self.assertIn('doConfigChanged |= AutoSetTargetDirections(entry, directions)', source)
        wrapper = source.split('static bool AutoSetTargetDirections(',1)[1].split('static void AutoTargetsOnDay(int day, int closedDay)\n{',1)[0]
        self.assertIn('AutoTaskConfigValid(cfg)', wrapper)
        self.assertIn('AutoApplyEntryActivation(entry, directions)', wrapper)

    def test_under_construction_targets_are_gated(self):
        """Owner bug 2026-11-22: private DOs kept unloading at an unfinished
        privately funded site (302 x commit=0) and each trip was charged."""
        plugin = ROOT / 'plugins/private_logistics'
        source = (plugin / 'private_logistics.cpp').read_text()
        helper = source.split('static bool AutoBuildingFinished(void* building)', 1)[1].split('\n}\n', 1)[0]
        self.assertIn('ReadablePtr((BYTE*)building + TSM_BLD_BUILT, sizeof(float))', helper)
        self.assertIn('TsmIsFinished(building)', helper)
        discovery = source.split('unsigned entityCount = 0, officeCount = 0, candidateCount = 0', 1)[1].split('AutoProbeStateInfrastructure(closedDay, entityCount);', 1)[0]
        self.assertIn('if (isOffice && !finished) continue;', discovery)
        self.assertIn('.hasStorage = (isOffice || !finished) ? 0 : 1;', discovery)
        self.assertIn('else if (finished) ++candidateCount;', discovery)
        self.assertIn('e.hasStorage = site.permission != 0 && finished;', source)
        self.assertIn('AutoDirectionsForBuildState(\n                AutoEffectiveDirections(isPrivate, permissions, capabilities), targetFinished)', source)
        runtime = (plugin / 'dry_assignment_runtime.h').read_text()
        self.assertIn('!AutoBuildingFinished((void*)token.receiver)', runtime)
        self.assertIn('reason="receiver_unfinished"', runtime)


if __name__ == '__main__':
    unittest.main()
