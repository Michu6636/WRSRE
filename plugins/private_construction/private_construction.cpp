// private_construction - STUB (2026-10-22) - restores correct service identity
// The file on origin/main (f38de86) is a byte-identical copy of private_logistics.cpp
// (3567 lines, PL_VERSION 0.17). That duplicate causes:
//   private_construction.dll advertises service "private_logistics v0.17" and patches 16 sites
//   private_logistics.dll   advertises same service and then fails 0/16 with ERROR no sites patched
// and double [AUTO DO][B2B] logs. The ini is correct (v3.5.2 behaviours), only the cpp is wrong.
// This stub prevents the collision so the logistics plugin can be tested in isolation.
// It does NOT implement autonomous building - that must be restored from the Windows checkout
// (C:\Users\EricM\Downloads\WRSRE\plugins\private_construction\private_construction.cpp).
// See docs/DEVELOPMENT_README.md: private_construction v3.5.2 native phase pricing (typedesc+0x370).

#include "../../src/tesmio_plugin.h"

static int g_enabled = 1;

static void ReadSettings() {
    g_enabled = H->configInt("plugins\\private_construction.ini", "private_construction", "enabled", 1);
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void) { return TSM_API_VERSION; }

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info) {
    TsmBind(host);
    info->name    = "private_construction";
    info->version = "3.5.2-STUB";
    ReadSettings();
    if (!g_enabled) { Logf("private_construction  disabled in ini"); return 1; }
    Logf("private_construction  STUB v3.5.2-STUB: original cpp was duplicate of private_logistics; stub prevents double-patch collision");
    Logf("private_construction  to restore: copy the real cpp from your Windows checkout and rebuild with build.bat private_construction");
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void) {
    if (!g_enabled) return 1;
    Logf("private_construction  live v3.5.2-STUB (no building logic - placeholder)");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
