"""Execute read-only snapshots and actual production logging with host stubs.

This verifies logging transparency, not native timer consumption or dispatch.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


def audit_harness():
    source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
    setter = source.split('static bool AutoSetTargetDirections(', 1)[1].split('\nstatic void AutoLogNativeTarget', 1)[0]
    setter = 'static bool AutoSetTargetDirections(' + setter
    office = source.split('static void AutoLogOfficePlanState(', 1)[1].split('\nstatic void AutoTargetsOnDay', 1)[0]
    office = 'static void AutoLogOfficePlanState(' + office
    return r'''
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "site_permissions.h"
#include "task_audit.h"
using BYTE = unsigned char;
#define AUTO_TARGET_ENTRY_SIZE 0x18
#define AUTO_DO_DIRTY 0xD50
#define AUTO_DO_UI_TIMER 0xFC0
static unsigned g_autoLogLines;
// Explicit 20% fixture preserves earlier regressions; the final section
// switches to the new 10% profile. Real defaults are tested against INI/source.
static int g_autoTargetLogLimit=80, g_autoSourcePercent=20, g_autoDestinationPercent=30;
static bool readable=true;
static bool ReadablePtr(const void* p, size_t) { return p && readable; }
static bool AutoTaskConfigValid(void* p) { return p && readable; }
static std::vector<std::string> logs;
static void Logf(const char* fmt, ...) {
    char line[2048]; va_list args; va_start(args,fmt);
    vsnprintf(line,sizeof(line),fmt,args); va_end(args); logs.emplace_back(line);
}
''' + setter + office + r'''
int main() {
    alignas(8) BYTE entry[0x18]={}, cfg[0x198]={};
    void* ptr=cfg; memcpy(entry+16,&ptr,sizeof(ptr));
    BYTE ec[sizeof(entry)], cc[sizeof(cfg)];
    memcpy(ec,entry,sizeof(entry)); memcpy(cc,cfg,sizeof(cfg));
    const auto a=AutoReadTaskAudit(entry,cfg);
    assert(memcmp(ec,entry,sizeof(entry))==0 && memcmp(cc,cfg,sizeof(cfg))==0);
    assert(AutoTaskAuditChanges(a,a)==0);
    auto b=a; b.enabled=1; assert(AutoTaskAuditChanges(a,b)==1);
    b=a; b.loadAll=1; assert(AutoTaskAuditChanges(a,b)==2);
    b=a; b.unloadAll=1; assert(AutoTaskAuditChanges(a,b)==4);
    b=a; b.loadBits=0x3DCCCCCD; assert(AutoTaskAuditChanges(a,b)==8);
    assert(AutoAuditPercent(b.loadBits)==(double)0.1f);
    b=a; b.unloadBits=0x3E99999A; assert(AutoTaskAuditChanges(a,b)==16);
    b=a; b.loadEnd=16; assert(AutoTaskAuditChanges(a,b)==32);
    b=a; b.unloadEnd=16; assert(AutoTaskAuditChanges(a,b)==64);
    b=a; b.loadBegin=16; assert(AutoTaskAuditChanges(a,b)==32);
    b=a; b.unloadBegin=16; assert(AutoTaskAuditChanges(a,b)==64);
    // The actual production setter has identical effects with audit enabled
    // or exhausted. All direction masks and repeated reconciliation covered.
    for (int budget : {0,1,80}) for (unsigned mask=0;mask<4;++mask) {
        memset(cfg,0,sizeof(cfg)); entry[0]=0;
        BYTE expected[sizeof(cfg)]; memcpy(expected,cfg,sizeof(cfg));
        BYTE expectedEntry[sizeof(entry)]; memcpy(expectedEntry,entry,sizeof(entry));
        const bool c=AutoApplyTaskDirections(expected,mask,20,30);
        const bool e=AutoApplyEntryActivation(expectedEntry,mask);
        g_autoLogLines=0; g_autoTargetLogLimit=budget; logs.clear();
        assert(AutoSetTargetDirections(entry,mask)==(c||e));
        assert(memcmp(cfg,expected,sizeof(cfg))==0);
        assert(memcmp(entry,expectedEntry,sizeof(entry))==0);
        assert(g_autoLogLines<=(unsigned)budget);
        assert(!AutoSetTargetDirections(entry,mask));
    }
    // Distinguish a 10% incoming value from our normalized 20%, without
    // treating a native clamp as established or changing the intended policy.
    AutoApplyTaskDirections(cfg,3); entry[0]=1;
    float incoming=0.1f; memcpy(cfg+4,&incoming,4);
    logs.clear(); g_autoLogLines=0; g_autoTargetLogLimit=80;
    assert(AutoSetTargetDirections(entry,3));
    assert(logs.size()==2 && logs[1].find("changed_fields=0x08")!=std::string::npos);
    assert(logs[1].find("load_bits=3DCCCCCD->3E4CCCCD")!=std::string::npos);
    // Actual captured regression: source-only 20%, denied unload normalized
    // to 10% by native code. Daily reconciliation MUST be a no-op, so the
    // caller does not request another dirty/countdown reset.
    AutoApplyTaskDirections(cfg,1,20,30); entry[0]=1;
    float nativeDisabled=0.1f; memcpy(cfg+0xCC,&nativeDisabled,4);
    logs.clear(); g_autoLogLines=0;
    for (unsigned day=0;day<100;++day) {
        assert(!AutoSetTargetDirections(entry,1));
        assert(cfg[0]==1 && cfg[0xC8]==0);
        assert(AutoReadTaskAudit(entry,cfg).unloadBits==0x3DCCCCCD);
    }
    assert(logs.empty());
    // Office-wide native edits may change a DENIED load percentage too.
    // No work is permitted merely because its unused threshold is nonzero.
    AutoApplyTaskDirections(cfg,2);
    nativeDisabled=0.3f; memcpy(cfg+4,&nativeDisabled,4);
    logs.clear(); g_autoLogLines=0;
    assert(!AutoSetTargetDirections(entry,2));
    assert(cfg[0]==0 && cfg[0xC8]==1 && logs.empty());
    // Permission revocation still disables wildcards and empties filters,
    // without moving cargo, freeing allocation or changing unused thresholds.
    uintptr_t begin=0x1000, end=0x1020;
    memcpy(cfg+8,&begin,8); memcpy(cfg+16,&end,8); cfg[0]=1;
    const auto prior=AutoReadTaskAudit(entry,cfg);
    assert(AutoSetTargetDirections(entry,2));
    const auto denied=AutoReadTaskAudit(entry,cfg);
    assert(denied.loadAll==0 && denied.loadEnd==denied.loadBegin);
    assert(denied.loadBits==prior.loadBits);
    assert(!AutoSetTargetDirections(entry,2));
    // Re-authorizing source restores company 20%, not the retained 30%.
    assert(AutoSetTargetDirections(entry,3));
    assert(AutoReadTaskAudit(entry,cfg).loadBits==0x3E4CCCCD);
    // New default profile: a task starts with 10% without a UI click.
    // A legacy 20% source converges once; an observed manual 10% value is
    // no longer undone by our setter. This does NOT simulate native dispatch.
    g_autoSourcePercent=10; logs.clear(); g_autoLogLines=0;
    AutoApplyTaskDirections(cfg,1,20,30); entry[0]=1;
    nativeDisabled=0.1f; memcpy(cfg+0xCC,&nativeDisabled,4);
    const auto oldTwenty=AutoReadTaskAudit(entry,cfg);
    assert(AutoSetTargetDirections(entry,1));
    const auto newTen=AutoReadTaskAudit(entry,cfg);
    assert(AutoTaskAuditChanges(oldTwenty,newTen)==8);
    assert(newTen.loadBits==0x3DCCCCCD && newTen.unloadBits==0x3DCCCCCD);
    assert(newTen.loadAll==1 && newTen.unloadAll==0);
    const size_t afterConvergence=logs.size();
    for (unsigned day=0;day<100;++day) assert(!AutoSetTargetDirections(entry,1));
    assert(logs.size()==afterConvergence);
    // The actual default writer used by new-task setup also produces 10/30.
    alignas(8) BYTE fresh[0x198]={};
    assert(AutoApplyTaskDirections(fresh,3));
    assert(AutoReadTaskAudit(entry,fresh).loadBits==0x3DCCCCCD);
    assert(AutoReadTaskAudit(entry,fresh).unloadBits==0x3E99999A);
    assert(!AutoApplyTaskDirections(fresh,3));
    // Explicit overrides are still supported; do not silently force 10%.
    g_autoSourcePercent=20;
    assert(AutoSetTargetDirections(entry,1));
    assert(AutoReadTaskAudit(entry,cfg).loadBits==0x3E4CCCCD);
    // Office snapshots must not write scheduling fields, including when
    // unreadable. Missing reads are explicitly marked, not interpreted as 0.
    alignas(8) BYTE building[0x1000]={}, original[0x1000];
    building[AUTO_DO_DIRTY]=1; float t=3; memcpy(building+AUTO_DO_UI_TIMER,&t,4);
    memcpy(original,building,sizeof(building));
    logs.clear(); g_autoLogLines=0;
    AutoLogOfficePlanState(building,123,7,"before-notify",true);
    assert(logs.back().find("dirty=1 timer_readable=1 countdown=3")!=std::string::npos);
    readable=false;
    AutoLogOfficePlanState(building,123,7,"after-notify",true);
    assert(logs.back().find("dirty=-1 timer_readable=0")!=std::string::npos);
    readable=true;
    assert(memcmp(original,building,sizeof(building))==0);
    g_autoLogLines=80; const size_t n=logs.size();
    AutoLogOfficePlanState(building,123,7,"before-reconcile",false);
    assert(logs.size()==n);
}
'''


class TaskAuditTest(unittest.TestCase):
    def test_production_logging_is_bounded_and_transparent(self):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            self.skipTest('C++ compiler required')
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / 'audit.cpp'
            exe = Path(directory) / 'audit'
            cpp.write_text(audit_harness())
            result = subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-I', str(ROOT / 'plugins/private_logistics'), str(cpp), '-o', str(exe)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(exe)], check=True, capture_output=True)

    def test_office_observations_surround_existing_notification(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        start = source.index('"before-notify", doAdded > 0 || doConfigChanged)')
        end = source.index('"after-notify", doAdded > 0 || doConfigChanged)', start)
        body = source[start:end]
        self.assertIn('if (doAdded > 0 || doConfigChanged)', body)
        self.assertEqual(body.count('*((BYTE*)office.building + AUTO_DO_DIRTY) = 1;'),1)
        self.assertEqual(body.count('*(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;'),1)


if __name__ == '__main__':
    unittest.main()
