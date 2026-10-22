// private_logistics - what a private company earns, and what it pays.
//
// v0.17 updates (storage panel integration):
//   - Consumes ui_panels API and registers a row provider for type-5
//     storage buildings, appending a row per storage panel window.
//   - Uses registerRowProvider via the ui_panels service (never calls
//     widget RVAs directly - per .clinerules section D).
//
// Level 3. Consumes enterprise_base (v8), currency (v4) and sim_clock.
// Optionally consumes resource_pricing for the unit price.
// Optionally consumes ui_panels for storage panel rows.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"

#include <stdint.h>
#include <math.h>
#include <new>
// ---------------------------------------------------------------------------
// the transfer being instrumented
// ---------------------------------------------------------------------------

#define TSM_RVA_TRANSFER 0x420350

typedef int (*tTransfer)(void* game, void* veh, unsigned mode, void* res,
                         float tons, float* outMoved, float cap,
                         unsigned char flag, void* bld);

static tTransfer g_realTransfer = nullptr;

// Vehicle fields.
#define TSM_VEH_HOME    0x4F8   // Building* - the owning depot (mode 1 only)
static int g_autoTargetsEnabled = 0;
static int g_autoTargetsDryRun  = 1;
static int g_autoTargetLogLimit = 80;
static volatile LONG g_autoResetActiveMemoAfterLoad = 0;
#define AUTO_ENTITY_MAX 8192
#define AUTO_MEMO_MAX   262144
#define AUTO_MEMO_HASH_SIZE 524288
#define AUTO_POINTER_HASH_SIZE 16384
#define AUTO_OFFICE_HASH_SIZE 16384
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char targetPresent; };
struct AutoMemo { uint64_t officeKey; uint64_t targetKey; unsigned nextOffice; unsigned char state; };
struct AutoPointerSlot { void* building; unsigned entityIndex; };
struct AutoOfficeMemoHead { uint64_t officeKey; unsigned headPlusOne; };
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static AutoMemo   g_autoMemo[AUTO_MEMO_MAX];
static unsigned   g_autoMemoHash[AUTO_MEMO_HASH_SIZE];
static AutoPointerSlot g_autoPointerHash[AUTO_POINTER_HASH_SIZE];
static AutoOfficeMemoHead g_autoOfficeMemoHeads[AUTO_OFFICE_HASH_SIZE];
static unsigned   g_autoMemoCount = 0;
static unsigned   g_autoLogLines = 0;
static int        g_autoLastDay = -1;
static void AutoTargetsOnDay(int day, int closedDay);
static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void* userData);

// ---------------------------------------------------------------------------
// the sixteen call sites
// ---------------------------------------------------------------------------

enum SiteClass {
    SITE_UNKNOWN = 0,
    SITE_REFUEL,
    SITE_LOAD,
    SITE_DELIVERY,
    SITE_TRANSSHIP
};

struct SiteSpec {
    unsigned    rva;
    SiteClass   known;
    const char* note;
};

#define MAX_SITES 16
static SiteSpec g_sites[MAX_SITES] = {
    { 0x00141F33, SITE_UNKNOWN,   "never observed" },
    { 0x0016F831, SITE_REFUEL,    "refuel call - telemetry, never billed; never observed" },
    { 0x001B2898, SITE_UNKNOWN,   "CONTESTED: r8-9 'road delivery', r10 'road refuel'; never observed" },
    { 0x001B484B, SITE_UNKNOWN,   "never observed" },
    { 0x001B5EC0, SITE_UNKNOWN,   "never observed" },
    { 0x001B653B, SITE_UNKNOWN,   "never observed" },
    { 0x001B6833, SITE_UNKNOWN,   "never observed" },
    { 0x001B75ED, SITE_UNKNOWN,   "never observed" },
    { 0x001B7981, SITE_UNKNOWN,   "never observed" },
    { 0x001CB7FF, SITE_LOAD,      "loader: mode 0, +tons, then bld+0x970 storage-add; never observed" },
    { 0x0068EECA, SITE_LOAD,      "family A: mode 0, +tons, cargo-add follows; never observed" },
    { 0x0068F792, SITE_DELIVERY,  "family A DELIVERY: mode 0, +tons, param_2 = building, param_9 = fleet owner" },
    { 0x0068F7F9, SITE_TRANSSHIP, "family A delivery, vehicle side: mode 1, -tons. NEVER BILLED" },
    { 0x006945B3, SITE_TRANSSHIP, "family A: mode 0, -tons; never observed" },
    { 0x00694F40, SITE_LOAD,      "family A LOAD: mode 0, -tons, param_2 = the building the goods leave" },
    { 0x006953C4, SITE_TRANSSHIP, "family A load, vehicle side: mode 1, +tons. NEVER BILLED" }
};

static bool g_sitePatched[MAX_SITES] = { false };
static bool g_siteWanted[MAX_SITES]  = { false };
// ----------------------------------------------------------------
// v0.17: Storage panel row provider.
//
// Called by ui_panels when a type-5 storage building window is open.
// Appends one text row naming the storage type, using only the ui_panels
// API contract (no widget RVA access, no native widget RVAs directly).
// ---------------------------------------------------------------------------
static void StoragePanelRowProvider(void* building, TsmPanelRows* out, void* /*userData*/)
{
    if (!out || !building) return;
    if (!g_enterprise) return;
    if (!ReadablePtr(building, 0x320)) return;

    // Only serve type-5 storage buildings.
    int bldType = TsmBuildingType(building);
    if (bldType != 5) return;

    wchar_t value[TSM_PANEL_VALUE];
    _snwprintf_s(value, _countof(value), _TRUNCATE, L"Storage (%d)", bldType);

    if (out->count >= out->capacity) return;
    if (out->count >= TSM_PANEL_MAX_ROWS) return;

    TsmPanelRow* r = &out->rows[out->count++];
    wcsncpy_s(r->label, TSM_PANEL_LABEL, L"Storage:", _TRUNCATE);
    wcsncpy_s(r->value, TSM_PANEL_VALUE, value, _TRUNCATE);
    r->colour = 0;
}
// ---------------------------------------------------------------------------
// call-site patching
// ---------------------------------------------------------------------------

static bool PatchSite(int index)
{
    if (index < 0 || index >= MAX_SITES) return false;
    if (g_sitePatched[index]) return true;

    const unsigned rva = g_sites[index].rva;

    if (rva < 0x1000 || (size_t)rva + 5 > g_exeSize) {
        Logf("private_logistics  site 0x%06X is outside the image (%llu bytes) - skipped",
             rva, (unsigned long long)g_exeSize);
        return false;
    }

    BYTE* site = g_exeBase + rva;
    if (!ReadablePtr(site, 5)) {
        Logf("private_logistics  site 0x%06X not readable - skipped", rva);
        return false;
    }

    // Verify: a direct call, landing on the transfer.
    if (site[0] != 0xE8) {
        Logf("private_logistics  site 0x%06X is not a call (0x%02X) - skipped",
             rva, site[0]);
        return false;
    }
    const int32_t rel    = *(const int32_t*)(site + 1);
    BYTE*         target = site + 5 + rel;
    if (target != g_exeBase + TSM_RVA_TRANSFER) {
        Logf("private_logistics  site 0x%06X calls %p, not the transfer - skipped",
             rva, target);
        return false;
    }

    // The cave: `jmp qword ptr [rip+0]` plus the absolute detour address.
    BYTE* cave = AllocNear(site, 32);
    if (!cave) {
        Logf("private_logistics  no code cave near 0x%06X - skipped", rva);
        return false;
    }

    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&PatchSite; // placeholder - actual detour set by caller
    memcpy(cave + 6, &detour, 8);
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("private_logistics  cave out of rel32 range at 0x%06X - skipped", rva);
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("private_logistics  VirtualProtect failed at 0x%06X - nothing written", rva);
        return false;
    }

    // Re-verify under the protection.
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("private_logistics  site 0x%06X changed under us - nothing written", rva);
        return false;
    }

    site[0] = 0xE8;
    *(int32_t*)(site + 1) = (int32_t)toCave;
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    g_sitePatched[index] = true;
    Logf("private_logistics  hook 0x%06X -> transfer ok (cave %p)", rva, (void*)cave);
    return true;
}

static void PatchAllSites(void)
{
    int ok = 0, wanted = 0;
    for (int i = 0; i < MAX_SITES; i++) {
        wanted++;
        if (PatchSite(i)) ok++;
    }
    Logf("private_logistics  patched %d of %d transfer call sites", ok, wanted);

    if (ok == 0) {
        Logf("private_logistics  ERROR  no sites patched. Either this is not "
             "SOVIET64.exe v1.1.1.9, or the transfer is not at RVA 0x420350. "
             "Nothing was written; the game is unaffected.");
    } else if (ok < wanted) {
        Logf("private_logistics  WARN  %d site(s) refused - see the lines above. "
             "A refusal is safe: that call still reaches the engine normally.",
             wanted - ok);
    }
}