// wrsre_entry.cpp - single-DLL entry point for WRSRE (`wrsre.dll`).
//
// Links all core WRSRE plugin modules into one DLL loaded by tesmioloader.dll,
// eliminating cross-DLL /MT CRT boundaries between WRSRE subsystems and
// enforcing an explicit, deterministic Init/Start dependency order without
// modifying tesmioloader.cpp.

#include "tesmio_plugin.h"

#define WRSRE_DECLARE_MOD(mod)                                                 \
    extern "C" int WrsreInit_##mod(const TsmHost* host, TsmPluginInfo* info);  \
    extern "C" int WrsreStart_##mod(void)

// Level 0: infrastructure
WRSRE_DECLARE_MOD(sim_clock);
WRSRE_DECLARE_MOD(persistence);
WRSRE_DECLARE_MOD(currency);

// Level 1: pricing, enterprises, banking, fiscal & ledger
WRSRE_DECLARE_MOD(resource_pricing);
WRSRE_DECLARE_MOD(enterprise_base);
WRSRE_DECLARE_MOD(republic_bank);
WRSRE_DECLARE_MOD(taxes);
WRSRE_DECLARE_MOD(state_ledger);

// Level 2: labor, retail, shops, logistics, construction
WRSRE_DECLARE_MOD(citizen_labor);
WRSRE_DECLARE_MOD(citizen_retail);
WRSRE_DECLARE_MOD(private_shops);
WRSRE_DECLARE_MOD(private_logistics);
WRSRE_DECLARE_MOD(private_construction);

// UI
WRSRE_DECLARE_MOD(ui_panels);
WRSRE_DECLARE_MOD(ui_overlay);
#ifdef WRSRE_WITH_ALMANAC
WRSRE_DECLARE_MOD(almanac_overlay_dev);
#endif

struct WrsreModule {
    const char*      name;
    TsmPluginInitFn  init;
    TsmPluginStartFn start;
    int              initialized;
};

#define WRSRE_MOD_ENTRY(mod) { #mod, WrsreInit_##mod, WrsreStart_##mod, 0 }

static WrsreModule g_modules[] = {
    // Level 0
    WRSRE_MOD_ENTRY(sim_clock),
    WRSRE_MOD_ENTRY(persistence),
    WRSRE_MOD_ENTRY(currency),
    // Level 1
    WRSRE_MOD_ENTRY(resource_pricing),
    WRSRE_MOD_ENTRY(enterprise_base),
    WRSRE_MOD_ENTRY(republic_bank),
    WRSRE_MOD_ENTRY(taxes),
    WRSRE_MOD_ENTRY(state_ledger),
    // Level 2
    WRSRE_MOD_ENTRY(citizen_labor),
    WRSRE_MOD_ENTRY(citizen_retail),
    WRSRE_MOD_ENTRY(private_shops),
    WRSRE_MOD_ENTRY(private_logistics),
    WRSRE_MOD_ENTRY(private_construction),
    // UI
    WRSRE_MOD_ENTRY(ui_panels),
    WRSRE_MOD_ENTRY(ui_overlay),
#ifdef WRSRE_WITH_ALMANAC
    WRSRE_MOD_ENTRY(almanac_overlay_dev),
#endif
};

static const int kModuleCount = (int)(sizeof(g_modules) / sizeof(g_modules[0]));

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "wrsre";
    info->version = "1.0-monolithic";

    int initOk = 0;
    for (int i = 0; i < kModuleCount; i++) {
        WrsreModule* m = &g_modules[i];
        m->initialized = 0;

        if (host->configInt &&
            host->configInt("tesmioloader.ini", "plugins", m->name, 1) == 0)
        {
            Logf("wrsre    %-20s off in tesmioloader.ini [plugins]", m->name);
            continue;
        }

        TsmPluginInfo subInfo;
        memset(&subInfo, 0, sizeof(subInfo));
        int rc = 1;
        __try { rc = m->init(host, &subInfo); }
        __except (FaultFilter("wrsre module init", GetExceptionInformation())) {
            rc = -1;
        }

        if (rc == 0) {
            m->initialized = 1;
            initOk++;
            Logf("wrsre    init  %-20s %s",
                 subInfo.name ? subInfo.name : m->name,
                 subInfo.version ? subInfo.version : "");
        } else {
            Logf("wrsre    skip  %-20s init returned %d", m->name, rc);
        }
    }

    if (initOk == 0) {
        Logf("wrsre    no modules initialized - declining load");
        return 1;
    }
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    int started = 0;
    for (int i = 0; i < kModuleCount; i++) {
        WrsreModule* m = &g_modules[i];
        if (!m->initialized || !m->start) continue;

        int rc = 0;
        __try { rc = m->start(); }
        __except (FaultFilter("wrsre module start", GetExceptionInformation())) {
            rc = -1;
        }


        if (rc == 0) {
            started++;
        } else {
            Logf("wrsre    start %-20s returned %d - inactive", m->name, rc);
        }
    }
    Logf("wrsre    %d/%d modules active", started, kModuleCount);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
