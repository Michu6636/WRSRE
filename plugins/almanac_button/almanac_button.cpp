// almanac_button - draw-only ninth backdrop beside the Economy & Trade row.
//
// WHAT IT DOES
//   In the normal eight-tile configuration the native trade row draws eight
//   tiles. After the eighth tile this plugin makes one more call to the
//   engine's own C3D_PANEL2D::Draw, with the rect store set to the next slot.
//   It has no click handling, and it does not read or write the click latch
//   (0xA54E91). It does not write native selection, City Hall, tax policy, or
//   saves.
//
//   This is NOT a change-free draw. "Draw only" means no input is handled. The
//   extra engine call can change shared UI state (see SHARED STATE below).
//
// DEFAULT
//   plugins\almanac_button.ini: enabled = 0. Init then returns non-zero, and
//   the loader calls FreeLibrary (the only unload path). Nothing is patched.
//
// MECHANISM
//   One import swap: C3D_PANEL2D::Draw at IAT slot RVA 0x86C860, called by the
//   row handler from RVA 0x3743A2 (return address 0x3743A8) on the trade widget
//   at RVA 0x9BE060. Every call is forwarded. Only the row's own eighth-tile
//   call takes the extra path, and only while armed.
//
// VERIFIED AT START (static checks against this build; refuses otherwise)
//   - The PE TimeDateStamp equals the repo baseline (TsmCheckExeBuild). That is
//     one header field, NOT a whole-file check. tesmioloader.ini
//     allow_unverified_exe = 1 bypasses it.
//   - Two call-site byte sequences: E8 AC 50 05 00 at RVA 0x31EDEF (CALL
//     FUN_140373EA0), and FF 15 B8 84 4F 00 at RVA 0x3743A2 (CALL [0x86C860]).
//   - The import slot is at RVA 0x86C860 and holds the engine's exported Draw.
//   - Three constants read 45 (origin A), 85 (origin B) and 64 (pitch).
//   These checks do NOT authenticate the executable as a whole, and they do
//   NOT prove the row layout.
//
// CONSTANTS DURING A PASS
//   The row tracker captures scale, origin A, origin B and pitch on a pass's
//   first tile. Every later tile must read the same values, including the tile
//   that decides the draw. A changed value drops the pass.
//
// SHARED STATE WRITTEN BY THE EXTRA DRAW
//   - RVA 0x9BE2E8..0x9BE2FB (rect store, 20 bytes). The ninth X and Y are
//     written for the one call, and the saved bytes are restored afterwards.
//     The native Draw may write this store itself, and the restore does not
//     undo writes made during that call.
//   - Whatever the engine's C3D_PANEL2D::Draw writes internally while drawing
//     the ninth tile. NOT identified. It may include per-widget or global draw
//     state, hover or press state, or GPU state. UNVERIFIED.
//   - This plugin's own counters and log lines.
//   Known limitation: the ninth tile reuses the eighth tile's hover state.
//
// SLOT OWNERSHIP AND RESTORE
//   - Swap and restore are each one interlocked compare-exchange on the slot.
//     The write happens only if the slot still holds the value we observed.
//     This plugin never overwrites a value it did not see.
//   - This does NOT stop another writer from replacing the slot. Plain stores
//     from another plugin, from the engine, or from another thread can replace
//     our detour at any time. Nothing here guarantees that another hook is
//     never overwritten. The code reports what it observes.
//   - Our detour may still be reachable through another hook's chain, so it
//     always forwards valid calls to the engine's Draw, and the DLL is never
//     unloaded by this plugin.
//   - RestoreOriginal runs from ordinary code, never inside an exception
//     handler. It logs what it observed and what it did.
//
// LOADER LIFECYCLE (verified in src/tesmioloader.cpp)
//   - Init non-zero: the loader calls FreeLibrary. This plugin returns non-zero
//     from Init only when disabled, before anything is patched.
//   - Start non-zero: the loader logs "started with N - inactive" and does NOT
//     unload the DLL. A swap may already be in place.
//   So once a swap has succeeded, the DLL stays loaded for the session. Start
//   returns 0 when armed and 1 when the plugin is inactive. Forwarding stays
//   valid in both states.
//
// FAULTS
//   - Faults in this plugin's own code are caught: the snapshot reads and the
//     tracker (PlanRowTile), and the rect-store write (WriteNinthSlot). Those
//     handlers only set interlocked flags. Ordinary code then resets the
//     tracker, restores the store, and calls RestoreOriginal.
//   - Faults inside the native Draw, eighth or ninth, are NOT caught here. They
//     propagate as they would without this plugin. Continuing after a fault in
//     engine code can leave engine state inconsistent, so this plugin does not
//     swallow them.
//
// CONCURRENCY (ASSUMPTIONS, NOT VERIFIED)
//   - Draw is assumed to run on one render thread. Start is assumed to run on
//     the loader thread before the first row draw. The loader runs StartPlugins
//     as a sequential loop (verified in tesmioloader.cpp).
//   - The compare-exchange removes the window between our check and our write.
//     It does not remove a concurrent writer that bypasses the check.
//
// RESTORATION FOR A TESTER
//   Set enabled = 0 and restart the game.
//
// STATUS
//   Only the pure geometry logic is host-tested (g++). This Windows glue is NOT
//   compiled or run in this session. The ninth position, its visibility, and its
//   clipping are NOT established.
//
// Version 1.2: compare-exchange swap and restore; constants captured per pass;
// native ninth draw outside exception handlers; corrected lifecycle notes.

#include <stdint.h>
#include "../../src/tesmio_plugin.h"
#include "almanac_button_geometry.h"

#define ALM_DRAW_SYM "?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z"   // same as ui_panels

#define ALM_RVA_HOST_CALL  0x31EDEFu   // E8 AC 50 05 00     CALL FUN_140373EA0
#define ALM_RVA_ROW_CALL   0x3743A2u   // FF 15 B8 84 4F 00  CALL [IAT 0x86C860]
#define ALM_RVA_ROW_RET    0x3743A8u   // the instruction after that CALL
#define ALM_RVA_IAT_DRAW   0x86C860u   // C3DDLL64 import slot, C3D_PANEL2D::Draw
#define ALM_RVA_K_ORIGIN_A 0x90AA0Cu   // float, expected 45
#define ALM_RVA_K_ORIGIN_B 0x90AB14u   // float, expected 85
#define ALM_RVA_K_PITCH    0x90AAA0u   // float, expected 64
#define ALM_OFF_SUBVEC     0xC650u     // game+0xC650: pointer to trade subvector
#define ALM_OFF_SELECTOR   0x10398u    // ONE BYTE: CMP byte ptr [R14+0x10398],R15B
#define ALM_OFF_C7D4       0xC7D4u     // dword
#define ALM_OFF_SKIP578    0x578u      // dword
#define ALM_OFF_SKIP5E0    0x5E0u      // dword

typedef void (*t_Draw)(void* self, float a1, float a2, float a3,
                       float a4, float a5, bool a6);

static int                        g_enabled;
static volatile LONG              g_active;    // 1 = armed: the ninth tile is wanted
static volatile LONG              g_faulted;   // set by an SEH handler, read by Detour
static t_Draw                     g_drawOrig;  // engine's Draw, recorded before the swap
static void**                     g_slot;      // the import slot we swapped
static almanac_button::RowTracker g_tracker;
static int                        g_ninthDrawn;
static int                        g_noteCount;
static const char*                g_lastNote;

// Plain-old-data plan, filled before the native call and used after it.
typedef struct NinthPlan {
    int   draw;
    int   reset;   // unreadable state: the pass must be dropped
    float x;
    float y;
} NinthPlan;

__declspec(noinline) static void Detour(void* self, float a1, float a2, float a3,
                                        float a4, float a5, bool a6);

static uintptr_t Base(void) { return (uintptr_t)g_exeBase; }

static bool IsArmed(void) { return InterlockedCompareExchange(&g_active, 1, 1) != 0; }

static bool ReadU8(uintptr_t addr, uint8_t* out)
{
    if (!ReadablePtr((const void*)addr, sizeof(uint8_t))) return false;
    *out = *(const uint8_t*)addr;
    return true;
}

static bool ReadF32(uintptr_t addr, float* out)
{
    if (!ReadablePtr((const void*)addr, sizeof(float))) return false;
    *out = *(const float*)addr;
    return true;
}

static bool ReadI32(uintptr_t addr, int32_t* out)
{
    if (!ReadablePtr((const void*)addr, sizeof(int32_t))) return false;
    *out = *(const int32_t*)addr;
    return true;
}

static bool ReadPtr(uintptr_t addr, uintptr_t* out)
{
    if (!ReadablePtr((const void*)addr, sizeof(uintptr_t))) return false;
    *out = *(const uintptr_t*)addr;
    return true;
}

// Logs a changed reason, capped so a busy session cannot flood the log.
static void Note(const char* reason)
{
    if (!reason || reason == g_lastNote) return;
    g_lastNote = reason;
    if (strcmp(reason, "none") == 0) return;
    if (g_noteCount++ < 40)
        Logf("almanac_button  row  %s", reason);
}

static bool ReadRowConstants(almanac_button::RowConstants* c)
{
    return ReadF32(Base() + TSM_RVA_DEFAULT_DPI, &c->scale) &&
           ReadF32(Base() + ALM_RVA_K_ORIGIN_A, &c->originA) &&
           ReadF32(Base() + ALM_RVA_K_ORIGIN_B, &c->originB) &&
           ReadF32(Base() + ALM_RVA_K_PITCH, &c->pitch);
}

// Game-object fields. The count is (end - begin) / 0x50, with begin at
// [P+0x48] and end at [P+0x50], where P = [game+0xC650]. The handler's own
// arithmetic matches this (ADD R8,0x48; SUB RCX,[R8]; /0x50 by multiply-high
// and SAR 5), so the layout is supported by the export. The live values were
// read only once. This check also rejects any span that is not a multiple of
// 0x50, which is stricter than the handler's floor division.
static bool ReadGate(almanac_button::RowGate* g)
{
    uintptr_t game = Base() + TSM_RVA_DEFAULT_GAME_OBJECT;
    uintptr_t sub = 0, begin = 0, end = 0;
    if (!ReadPtr(game + ALM_OFF_SUBVEC, &sub) || sub == 0) return false;
    if (!ReadPtr(sub + 0x48, &begin) || !ReadPtr(sub + 0x50, &end)) return false;
    if (end < begin) return false;
    uintptr_t span = end - begin;
    if (span % 0x50 != 0) return false;
    uintptr_t count = span / 0x50;
    if (count > 64) return false;
    g->count = (int32_t)count;

    uint8_t selector = 0;
    if (!ReadU8(game + ALM_OFF_SELECTOR, &selector)) return false;
    g->selector = selector;

    return ReadI32(game + ALM_OFF_C7D4, &g->c7d4) &&
           ReadI32(game + ALM_OFF_SKIP578, &g->skip578) &&
           ReadI32(game + ALM_OFF_SKIP5E0, &g->skip5e0);
}

// Tile position the handler stored in the global rect store just before the
// row's Draw call: {X at 0x9BE2F0, Y at 0x9BE2F4}.
static bool ReadTileRect(float* x, float* y)
{
    return ReadF32(Base() + TSM_RVA_DEFAULT_P2D_RECT_X, x) &&
           ReadF32(Base() + TSM_RVA_DEFAULT_P2D_RECT_Y, y);
}

static bool VerifyBytes(uintptr_t rva, const unsigned char* expect,
                        size_t n, const char* label)
{
    const void* p = (const void*)(Base() + rva);
    if (!ReadablePtr(p, n) || memcmp(p, expect, n) != 0) {
        Logf("almanac_button  REFUSED  bytes at %s do not match", label);
        return false;
    }
    return true;
}

static bool VerifyFloat(uintptr_t rva, float expect, const char* label)
{
    float v = 0.0f;
    if (!ReadF32(Base() + rva, &v) || v != expect) {
        Logf("almanac_button  REFUSED  %s is not %.1f", label, (double)expect);
        return false;
    }
    return true;
}

// Byte and value checks for this exact build. These are static checks on three
// call sites and three constants. They are not a whole-executable check, and
// they do not prove the row layout. Nothing is patched unless all of them pass.
static bool VerifyBuild(void)
{
    static const unsigned char kHostCall[5] = {0xE8, 0xAC, 0x50, 0x05, 0x00};
    static const unsigned char kRowCall[6]  = {0xFF, 0x15, 0xB8, 0x84, 0x4F, 0x00};

    if (!VerifyBytes(ALM_RVA_HOST_CALL, kHostCall, sizeof(kHostCall), "host call 0x31EDEF")) return false;
    if (!VerifyBytes(ALM_RVA_ROW_CALL, kRowCall, sizeof(kRowCall), "row Draw call 0x3743A2")) return false;
    if (ALM_RVA_ROW_CALL + 6u != ALM_RVA_ROW_RET) return false;

    if (!VerifyFloat(ALM_RVA_K_ORIGIN_A, 45.0f, "origin constant A")) return false;
    if (!VerifyFloat(ALM_RVA_K_ORIGIN_B, 85.0f, "origin constant B")) return false;
    if (!VerifyFloat(ALM_RVA_K_PITCH, 64.0f, "pitch constant")) return false;
    return true;
}

// ---------------------------------------------------------------- slot swap

// Replaces *slot with `repl` only if it still holds `expect`, in one interlocked
// compare-exchange. Returns the value found in the slot. The caller compares it
// with `expect`: equal means the write happened, and anything else means no
// write was made. *protectOk is false if the page could not be made writable.
static void* CompareSwapSlot(void** slot, void* expect, void* repl, bool* protectOk)
{
    DWORD oldProt = 0;
    *protectOk = VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &oldProt) != 0;
    if (!*protectOk) return NULL;
    void* was = InterlockedCompareExchangePointer((PVOID volatile*)slot, repl, expect);
    DWORD ignored = 0;
    if (!VirtualProtect(slot, sizeof(void*), oldProt, &ignored))
        Logf("almanac_button  WARNING  could not restore page protection on the import slot");
    return was;
}

// Puts the engine's original Draw back, but only if the slot still holds our
// detour. Never overwrites a value it did not observe. Stops drawing first.
// Runs from ordinary code only.
static bool RestoreOriginal(const char* why)
{
    InterlockedExchange(&g_active, 0);
    InterlockedExchange(&g_faulted, 0);

    if (!g_slot || !g_drawOrig) {
        Logf("almanac_button  NOT RESTORED (%s): no slot recorded; drawing stopped", why);
        return false;
    }

    bool protectOk = false;
    void* was = CompareSwapSlot(g_slot, (void*)Detour, (void*)g_drawOrig, &protectOk);
    if (!protectOk) {
        Logf("almanac_button  NOT RESTORED (%s): import slot not writable; drawing stopped", why);
        return false;
    }
    if (was == (void*)g_drawOrig) {
        Logf("almanac_button  restore (%s): slot already holds the original", why);
        return true;
    }
    if (was != (void*)Detour) {
        Logf("almanac_button  NOT RESTORED (%s): slot holds another value %p; no write made; "
             "our detour forwards only and the DLL stays loaded", why, was);
        return false;
    }
    if (*g_slot != (void*)g_drawOrig) {
        Logf("almanac_button  WARNING (%s): compare-exchange reported a swap but the slot "
             "does not read back as the original", why);
        return false;
    }
    Logf("almanac_button  restored original Draw import (%s)", why);
    return true;
}

// ---------------------------------------------------------------- row logic

// Reads the row state and advances the tracker. Runs before the native eighth
// tile. The protected block contains only reads and tracker calls. On a fault it
// sets interlocked flags and returns. Detour does the cleanup in ordinary code.
static void PlanRowTile(NinthPlan* plan)
{
    plan->draw = 0;
    plan->reset = 0;
    plan->x = 0.0f;
    plan->y = 0.0f;
    __try {
        float tx = 0.0f, ty = 0.0f;
        almanac_button::RowConstants c;
        almanac_button::RowGate g;
        if (!ReadTileRect(&tx, &ty) || !ReadRowConstants(&c) || !ReadGate(&g)) {
            plan->reset = 1;            // an unreadable frame cannot continue a pass
            return;
        }
        almanac_button::Decision d = g_tracker.OnRowTile(tx, ty, c, g);
        Note(d.reason);
        if (d.action == almanac_button::Action::DrawNinth) {
            plan->draw = 1;
            plan->x = d.x;
            plan->y = d.y;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_active, 0);
        InterlockedExchange(&g_faulted, 1);
    }
}

// Writes the ninth X and Y into the rect store. The only protected code is the
// two stores, which lie in the same 4 KB page (0x9BE000..0x9BEFFF). A faulting
// store therefore changed nothing, so the caller does not write back.
// Returns 0 if a store faulted.
static int WriteNinthSlot(uint8_t* rect, float x, float y)
{
    __try {
        *(float*)(rect + 8)  = x;   // 0x9BE2F0
        *(float*)(rect + 12) = y;   // 0x9BE2F4
        return 1;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
}

// Draws the ninth backdrop tile. Ordinary code: the native call is not inside
// any handler. The rect store is read, changed for the one call, and restored.
static void DrawNinthTile(void* self, float a1, float a2, float a3, float a4,
                          float a5, bool a6, float x, float y)
{
    uint8_t* rect = (uint8_t*)(Base() + TSM_RVA_DEFAULT_P2D_RECT_W);
    uint8_t saved[20];
    if (!ReadablePtr(rect, sizeof(saved))) {
        RestoreOriginal("rect store unreadable");
        return;
    }
    memcpy(saved, rect, sizeof(saved));
    if (!WriteNinthSlot(rect, x, y)) {
        // Nothing changed (see WriteNinthSlot). Stop drawing and restore the slot.
        RestoreOriginal("rect store write faulted");
        return;
    }
    g_drawOrig(self, a1, a2, a3, a4, a5, a6);         // native ninth draw
    memcpy(rect, saved, sizeof(saved));
    g_ninthDrawn++;
    if (g_ninthDrawn == 1)
        Logf("almanac_button  ninth tile drawn (first row pass)");
}

// ---------------------------------------------------------------- detour

// Replacement import for C3D_PANEL2D::Draw. Every call is forwarded. The one
// call that does more is the trade row's own draw of its eighth tile, and only
// while armed. Both native calls are outside any exception handler.
__declspec(noinline) static void Detour(void* self, float a1, float a2, float a3,
                                        float a4, float a5, bool a6)
{
    void* ret = _ReturnAddress();
    if (!IsArmed() || !g_drawOrig ||
        ret != (void*)(Base() + ALM_RVA_ROW_RET) ||
        self != (void*)(Base() + TSM_RVA_DEFAULT_PANEL2D)) {
        if (g_drawOrig) g_drawOrig(self, a1, a2, a3, a4, a5, a6);
        return;
    }

    NinthPlan plan;
    PlanRowTile(&plan);
    if (InterlockedCompareExchange(&g_faulted, 0, 0) != 0) {
        g_tracker.Reset();
        RestoreOriginal("fault while planning the row tile");
        plan.draw = 0;
    } else if (plan.reset) {
        g_tracker.Reset();
    }

    g_drawOrig(self, a1, a2, a3, a4, a5, a6);          // native eighth tile

    if (plan.draw && IsArmed())
        DrawNinthTile(self, a1, a2, a3, a4, a5, a6, plan.x, plan.y);
}

// ---------------------------------------------------------------- lifecycle

static void ReadSettings(void)
{
    const char* ini = "plugins\\almanac_button.ini";
    g_enabled = (H->configInt(ini, "almanac_button", "enabled", 0) == 1) ? 1 : 0;
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "almanac_button";
    info->version = "1.2";

    ReadSettings();
    if (!g_enabled) {
        Logf("almanac_button  disabled (enabled = 0)");
        return 1;                       // loader calls FreeLibrary; nothing is patched
    }
    return 0;
}

// Returns 0 when armed and 1 when inactive. The loader logs a non-zero result
// as "started with N - inactive" and keeps the DLL loaded either way.
extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;
    if (!TsmCheckExeBuild(H, "almanac_button")) return 1;
    if (!VerifyBuild()) return 1;

    void** slot = FindIatSlot(g_exe, DLL_ENGINE, ALM_DRAW_SYM);
    if (!slot) {
        Logf("almanac_button  REFUSED  no C3D_PANEL2D::Draw import");
        return 1;
    }
    if ((uintptr_t)slot != Base() + ALM_RVA_IAT_DRAW) {
        Logf("almanac_button  REFUSED  Draw import is not at RVA 0x86C860");
        return 1;
    }
    FARPROC engine = g_engine ? GetProcAddress(g_engine, ALM_DRAW_SYM) : NULL;
    if (!engine || (void*)engine != *slot) {
        Logf("almanac_button  REFUSED  Draw import differs from the engine export");
        return 1;
    }

    // Record the original first, so Detour can forward as soon as the slot
    // holds it. Stays inactive until the last step.
    t_Draw original = (t_Draw)*slot;
    g_slot = slot;
    g_drawOrig = original;

    bool protectOk = false;
    void* was = CompareSwapSlot(slot, (void*)original, (void*)Detour, &protectOk);
    if (!protectOk) {
        g_slot = NULL;
        g_drawOrig = NULL;
        Logf("almanac_button  REFUSED  import slot not writable; nothing written");
        return 1;
    }
    if (was != (void*)original) {
        g_slot = NULL;
        g_drawOrig = NULL;
        Logf("almanac_button  REFUSED  import slot changed before the swap (now %p); "
             "no write made", was);
        return 1;
    }
    if (*slot != (void*)Detour) {
        InterlockedExchange(&g_active, 0);
        Logf("almanac_button  WARNING  slot changed right after our swap; not armed; "
             "forwarding only");
        return 1;
    }

    InterlockedExchange(&g_active, 1);
    Logf("almanac_button  armed  draw-only ninth backdrop after row tile 8 (import verified)");
    return 0;
}
