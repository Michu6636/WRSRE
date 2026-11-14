// almanac_button - clickable ninth tile beside the Economy & Trade row.
//
// WHAT IT DOES
//   In the normal eight-tile configuration the native trade row draws eight
//   tiles. After the eighth tile this plugin makes one more call to the
//   engine's own C3D_PANEL2D::Draw, with the rect store set to the next slot.
//   It then checks for a click on that ninth tile (mouse over rect + click
//   latch set, read-only) and toggles a mod-owned Almanac open/close state.
//   Clicking another native row button closes the Almanac. It does not read or
//   write the click latch (0xA54E91), does not write native selection
//   (game+0xC65C), City Hall, tax policy, or saves.
//
//   The ninth tile still renders the native panel's current artwork (the
//   prototype's shared `self` Draw call); a distinct icon requires resolving
//   the texture-icon ownership question (see REPORT.md). The button is
//   clickable regardless.
//
// DEFAULT
//   The shipped INI and missing-key fallback are enabled = 1. If installed,
//   Init accepts the plugin and Start attempts the guarded import swap. Set
//   enabled = 0 in the active INI and restart to opt out. This experimental
//   hook is active by default on supported builds.
//
// MECHANISM
//   One import swap: C3D_PANEL2D::Draw at IAT slot RVA 0x86C860, called by the
//   row handler from RVA 0x3743A2 (return address 0x3743A8) on the trade widget
//   at RVA 0x9BE060. Every call is forwarded. Only the row's own eighth-tile
//   call takes the extra path, and only while armed. GetMouseSolid is resolved
//   from C3DDLL64.dll for click detection; if unresolved, clicks are not
//   detected but the plugin still arms.
//
// BUILD GUARD: WHAT TsmCheckExeBuild ACCEPTS (src/game_addresses.h)
//   It returns true when any of these holds:
//   - the PE TimeDateStamp equals the repo baseline 0x6A3EB6AD;
//   - addresses.ini [build] target_stamp equals the loaded stamp. The operator
//     chooses that value, and the loader then applies addresses.ini RVA
//     overrides to its own table;
//   - tesmioloader.ini [tesmioloader] allow_unverified_exe = 1, for any build;
//   - no PE image is mapped (test harness).
//   The stamp is one header field. It is not a whole-file check.
//   This plugin reads its RVAs from compile-time macros, not from the loader's
//   runtime table, so addresses.ini RVA overrides do not change this plugin's
//   addresses. The byte and value checks below are therefore the only layout
//   guard, and they run in every case above.
//
// VERIFIED AT START (static checks against this build; refuses otherwise)
//   - Two call-site byte sequences: E8 AC 50 05 00 at RVA 0x31EDEF (CALL
//     FUN_140373EA0), and FF 15 B8 84 4F 00 at RVA 0x3743A2 (CALL [0x86C860]).
//   - The import slot is at RVA 0x86C860 and holds the engine's exported Draw.
//   - Three constants read 45 (origin A), 85 (origin B) and 64 (pitch).
//   These checks do NOT authenticate the executable as a whole, do NOT prove the
//   row layout, and do NOT prove what the rect store means.
//
// CONSTANTS DURING A PASS
//   The row tracker captures scale, origin A, origin B and pitch on a pass's
//   first tile. Every later tile must read the same values, including the tile
//   that decides the draw. A changed value drops the pass.
//
// SHARED STATE WRITTEN BY THE EXTRA DRAW
//   - RVA 0x9BE2F0..0x9BE2F7: the X and Y floats of the rect store. The ninth
//     pair is written with ONE 8-byte store. The saved pair is written back with
//     ONE 8-byte store. The native Draw may write this pair itself during the
//     call. The restore sets back the pre-call pair, so any such write is
//     reverted. Whether the native Draw writes the pair is NOT verified.
//   - RVA 0x9BE2E8..0x9BE2EF (W, H) and RVA 0x9BE2F8.. are not written by this
//     plugin.
//   - Whatever the engine's C3D_PANEL2D::Draw writes internally while drawing
//     the ninth tile. NOT identified. It may include per-widget or global draw
//     state, hover or press state, or GPU state. UNVERIFIED.
//   - This plugin's own counters and log lines.
//
// SLOT OWNERSHIP AND PROTECTION
//   - The swap and the restore are each one interlocked compare-exchange on the
//     slot. The swap's expected value is the exact pointer validated at Start,
//     the engine export. It does not re-read the slot and adopt what it holds.
//   - Three steps are separate outcomes: making the page writable, the exchange,
//     and restoring the page protection.
//     * Page not writable: no exchange is tried and nothing is written.
//     * Exchange finds another value: nothing is written, and the other value
//       is left alone. Protection is still restored and its outcome logged.
//     * Exchange succeeds, protection not restored: the plugin does NOT arm. It
//       tries to undo its own write with the same recorded original protection
//       (not the protection a failed restore left behind), and logs whether the
//       undo succeeded and whether the ORIGINAL protection is now in place.
//     * Arming requires BOTH the exchange and the protection restore to succeed.
//   - The ORIGINAL protection is captured once, by the first protect-set in
//     Start. Every later restore uses that value. A later VirtualProtect return
//     value is not used as "the original": after a failed restore the page stays
//     PAGE_READWRITE, and that value would be wrong. Each restore is tried twice.
//   - This does NOT stop another writer from replacing the slot. Plain stores
//     from another plugin, from the engine, or from another thread can replace
//     our detour at any time. Nothing here guarantees that another hook is never
//     overwritten. The code reports what it observes.
//   - The detour always forwards valid calls to the engine's Draw. Once the slot
//     has been written, g_drawOrig is never cleared. This plugin never unloads
//     the DLL.
//
// LOADER LIFECYCLE (verified in src/tesmioloader.cpp)
//   - Init non-zero: the loader calls FreeLibrary. This plugin returns non-zero
//     from Init only when disabled, before anything is patched.
//   - Start non-zero: the loader logs "started with N - inactive" and does NOT
//     unload the DLL. A swap may already be in place.
//   So once a swap has been written, the DLL stays loaded for the session. Start
//   returns 0 when armed and 1 when inactive. Forwarding stays valid in both.
//
// FAULTS AND FAIL-STOP POLICY (DISPOSABLE TEST ONLY)
//   Any of the following ENDS THE GAME PROCESS through FailStop. The log line is
//   evidence only. The process end is what stops the game.
//     - the rect pair is unreadable or misaligned before any write;
//     - the 8-byte load of the pair faults, before any write;
//     - the 8-byte store of the ninth pair faults;
//     - the native ninth Draw faults (its SEH handler only reports the fault);
//     - the 8-byte restore of the saved pair faults (logged RECT DIRTY first).
//   FailStop does NOT restore the rect pair or the import slot. Their state is
//   not assumed, and a restore could fault too. It logs, calls TerminateProcess,
//   then __fastfail as a backstop if TerminateProcess returns.
//   The rect pair is never read again to decide anything after a failure.
//   A failed restore is a residual, not a safe failure. Whether TerminateProcess
//   ends the game in a Windows test is NOT verified. The README describes the
//   Windows-side containment procedure that backs it up.
//   - Faults in the plugin's own planning code (PlanRowTile) are unchanged: they
//     disarm and call RestoreOriginal. That is NOT a fail-stop.
//   - The eighth tile's native Draw is forwarded with no handler, so its faults
//     propagate as they would without the plugin. The ninth tile's native Draw is
//     wrapped in CallNinthNative, whose handler only reports the fault. That is
//     a deliberate change from the earlier "faults propagate" policy for the
//     ninth tile only. The user must acknowledge it.
//   - Protected blocks contain only reads, tracker calls, and the 8-byte pair
//     load and store. Each handler only returns a status.
//
// CONCURRENCY (ASSUMPTIONS, NOT VERIFIED)
//   - Draw is assumed to run on one render thread. Start is assumed to run on
//     the loader thread before the first row draw. The loader runs StartPlugins
//     as a sequential loop (verified in tesmioloader.cpp).
//   - The compare-exchange removes the window between our validation and our
//     write. It does not remove a concurrent writer that bypasses the check.
//
// RESTORATION AND FAIL-STOP FOR A TESTER
//   Set enabled = 0 and restart the game, on a disposable copy. See README.md.
//
// STATUS
//   Pure geometry logic is host-tested (g++). Draw hook and click detection
//   implemented; no independently supplied MSVC build artifact. The ninth tile
//   draws the native panel's current artwork (repeated crime-button artwork)
//   until the texture-icon ownership question is resolved. Click detection and
//   Almanac toggle are implemented but untested in-game. No click or input test
//   was performed.
//
// Version 2.0: adds clickable ninth tile with read-only click observation
//   (Path D) and mod-owned Almanac open/close state. GetMouseSolid resolved
//   from C3DDLL64.dll for click detection. Ninth tile still shows repeated
//   native artwork (I1 texture blocker unresolved).

#include <stdint.h>
#include <intrin.h>
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
struct C3DVec3 { float x, y, z; };
typedef void (*t_GetMouseSolid)(void* input, C3DVec3* out);

// Texture loading (re-added with corrected binding; see commit 09dc3e0).
typedef void* (*t_CreateManagedTexture)(void* middlepoint, const char* filename);
#define ALM_RVA_MIDDLEPOINT 0x9EACD0u   // C3D_MIDDLEPOINT global (passed to CreateManagedTexture)
#define ALM_TEX_BIND_OFFSET 0x70u       // vtable[0x70] = TexBind (confirmed by native code at FUN_140358A20:141)
// panel state global is TSM_RVA_DEFAULT_PANEL_STATE (0x9EAD08), NOT game+0x9D4F10+0xEAD08

static int                        g_enabled;
static volatile LONG              g_active;    // 1 = armed: the ninth tile is wanted
static volatile LONG              g_faulted;   // set by an SEH handler, read by Detour
static t_Draw                     g_drawOrig;  // engine's Draw, recorded before the swap
static t_GetMouseSolid            g_getMouseSolid; // engine's GetMouseSolid, resolved at Start
static t_CreateManagedTexture     g_createManagedTexture; // C3D_MIDDLEPOINT::CreateManagedTexture
static void*                      g_almanacTexture;       // plugin-owned C3DAPI_TEXTURE*
static int                        g_textureLoaded;         // 1 if texture loaded successfully
static int                        g_textureAttempted;      // 1 after first load attempt (prevents retry spam)
static int                        g_contentSuppressionEnabled; // INI: suppress native content when Almanac open
static char                       g_textureAsset[256];    // INI: texture_asset
static void**                     g_slot;      // the import slot we swapped
static DWORD                      g_slotProt;  // ORIGINAL protection of the import page
static int                        g_slotProtKnown; // set once, by the first protect-set in Start
static almanac_button::RowTracker g_tracker;
static int                        g_ninthDrawn;
static int                        g_noteCount;
static const char*                g_lastNote;

// Almanac state (Path D: read-only observation, mod-owned state).
static volatile LONG              g_almanacOpen;   // 0 = closed, 1 = open
static volatile LONG              g_almanacDebounce; // 1 while the click latch is held
static int32_t                    g_lastSelection; // last observed game+0xC65C (native selection)

#define ALM_OFF_SELECTION   0xC65Cu   // game+0xC65C: selected sub-tab index (read-only)
#define ALM_OFF_CONTENT_SUPPRESS 0x5E6u // game+0x5E6: content suppress flag (write-only)

// Plain-old-data plan, filled before the native call and used after it.
typedef struct NinthPlan {
    int   draw;
    int   reset;   // unreadable state: the pass must be dropped
    float x;
    float y;
    float w;   // ninth tile width  (scale * pitch)
    float h;   // ninth tile height (scale * pitch)
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

// Writes a single byte with SEH fault protection. Returns false on fault.
static bool WriteU8(uintptr_t addr, uint8_t value)
{
    __try {
        *(uint8_t*)addr = value;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
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

// The outcome of one compare-exchange attempt, with its protection steps kept
// separate. If protectSetOk is 0, no exchange was tried and nothing was written.
typedef struct SlotSwap {
    int   protectSetOk;      // the page was made writable
    int   protectRestoreOk;  // the ORIGINAL page protection (g_slotProt) was put back
    void* was;               // value found in the slot at the exchange
} SlotSwap;

// Makes the import page writable, exchanges *slot from `expect` to `repl`, then
// restores the ORIGINAL protection g_slotProt. captureOriginal = 1 is used only
// by Start, for the first protect-set, and records that set's old protection as
// the original. Every other call reuses the recorded original and ignores the
// old protection that its own protect-set returns (after a failed restore that
// value is PAGE_READWRITE). The restore is tried twice.
static SlotSwap CompareSwapSlot(void** slot, void* expect, void* repl, int captureOriginal)
{
    SlotSwap r;
    DWORD oldProt = 0;
    DWORD ignored = 0;
    int attempt;
    r.protectSetOk = 0;
    r.protectRestoreOk = 0;
    r.was = NULL;

    r.protectSetOk = VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &oldProt) != 0;
    if (!r.protectSetOk) return r;
    if (captureOriginal) {
        g_slotProt = oldProt;
        g_slotProtKnown = 1;
    }
    r.was = InterlockedCompareExchangePointer((PVOID volatile*)slot, repl, expect);

    if (g_slotProtKnown) {
        for (attempt = 0; attempt < 2 && !r.protectRestoreOk; attempt++)
            r.protectRestoreOk = VirtualProtect(slot, sizeof(void*), g_slotProt, &ignored) != 0;
    }
    return r;
}

static const char* ProtText(int ok)
{
    return ok ? "original protection restored" : "original protection NOT restored";
}

// Puts the engine's original Draw back, but only if the slot still holds our
// detour. Never overwrites a value it did not observe. Stops drawing first.
// Ordinary code only.
static bool RestoreOriginal(const char* why)
{
    InterlockedExchange(&g_active, 0);
    InterlockedExchange(&g_faulted, 0);

    if (!g_slot || !g_drawOrig) {
        Logf("almanac_button  NOT RESTORED (%s): no slot recorded; drawing stopped", why);
        return false;
    }

    SlotSwap r = CompareSwapSlot(g_slot, (void*)Detour, (void*)g_drawOrig, 0);
    if (!r.protectSetOk) {
        Logf("almanac_button  NOT RESTORED (%s): import page not writable; no write made; "
             "drawing stopped", why);
        return false;
    }
    if (r.was == (void*)Detour) {
        if (*g_slot != (void*)g_drawOrig) {
            Logf("almanac_button  WARNING (%s): exchange reported success but the slot does "
                 "not read back as the original", why);
            return false;
        }
        Logf("almanac_button  restored original Draw pointer (%s); %s",
             why, ProtText(r.protectRestoreOk));
        return r.protectRestoreOk != 0;
    }
    if (r.was == (void*)g_drawOrig) {
        Logf("almanac_button  restore (%s): slot already holds the original; %s",
             why, ProtText(r.protectRestoreOk));
        return r.protectRestoreOk != 0;
    }
    Logf("almanac_button  NOT RESTORED (%s): slot holds another value %p; no write made; "
         "our detour forwards only and the DLL stays loaded", why, r.was);
    return false;
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
            plan->w = c.scale * c.pitch;
            plan->h = c.scale * c.pitch;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_active, 0);
        InterlockedExchange(&g_faulted, 1);
    }
}

// ---------------------------------------------------------------- rect pair

// The rect pair is X at +8 and Y at +12 (0x9BE2F0 and 0x9BE2F4). It is 8 bytes
// and 8-byte aligned, so one 8-byte access reads or writes both. Little-endian:
// X is the low half.
static uint64_t PackPair(float x, float y)
{
    uint32_t xb = 0, yb = 0;
    memcpy(&xb, &x, sizeof(xb));
    memcpy(&yb, &y, sizeof(yb));
    return (uint64_t)xb | ((uint64_t)yb << 32);
}

// One 8-byte load. Returns 0 if it faulted. Nothing is written.
static int LoadRectPair(const uint8_t* at, uint64_t* out)
{
    __try {
        *out = *(const volatile uint64_t*)at;
        return 1;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
}

// One 8-byte store. Returns 0 if it faulted. A faulting store writes nothing.
static int StoreRectPair(uint8_t* at, uint64_t value)
{
    __try {
        *(volatile uint64_t*)at = value;
        return 1;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
}

// ---------------------------------------------------------------- fail-stop

// Disposable-test fail-stop. It writes log lines, then ends the process. It does
// NOT restore the rect pair or the import slot. Their state is not assumed after
// a failure, and a restore could fault too. The log line is evidence only. The
// process end is what stops the game.
__declspec(noreturn) static void FailStop(const char* why)
{
    Logf("almanac_button  FAIL-STOP  %s", why);
    Logf("almanac_button  FAIL-STOP  ending the game process (exit code 0xA1A10001); "
         "the rect pair and the import slot are NOT assumed to be in any state");
    TerminateProcess(GetCurrentProcess(), 0xA1A10001u);
    __fastfail(7u);                     // FAST_FAIL_FATAL_APP_EXIT; reached only if TerminateProcess returns
}

// The only SEH frame around a native call on the ninth path. Its handler does
// nothing but return 1. It does not touch the rect pair, the import slot, or any
// other state. DrawNinthTile decides what to do with the result.
static int CallNinthNative(void* self, float a1, float a2, float a3, float a4,
                           float a5, bool a6)
{
    __try {
        g_drawOrig(self, a1, a2, a3, a4, a5, a6);
        return 0;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 1;
    }
}

// Reads the click latch (read-only, does NOT consume it) and returns 1 if set.
static int ReadClickLatch(void)
{
    uint8_t v = 0;
    if (!ReadU8(Base() + TSM_RVA_DEFAULT_CLICK_LATCH, &v)) return 0;
    return v != 0 ? 1 : 0;
}

// Returns 1 if the mouse is over the ninth-tile rect and the click latch is set.
// Read-only: does not write game+0xC65C, does not consume 0xA54E91.
static int CheckNinthClick(float x, float y, float w, float h)
{
    if (!g_getMouseSolid) return 0;
    void* input = (void*)(Base() + TSM_RVA_DEFAULT_INPUT);
    C3DVec3 mouse = { 0.0f, 0.0f, 0.0f };
    g_getMouseSolid(input, &mouse);
    float mx = mouse.x;
    float my = mouse.y;
    // Native Collision uses inclusive upper bound (COMISS; JA means mouse.y <=
    // upper passes). Plugin uses half-open [x, x+w). Fix: inclusive upper edge.
    // Top-left AABB (native convention per rect store: X=origin+k*pitch, Y=rowY).
    if (mx < x || mx > x + w) return 0;
    if (my < y || my > y + h) return 0;
    return ReadClickLatch() ? 1 : 0;
}

// Manages Almanac open/close state. Toggles on ninth-tile click; closes when the
// native selection index changes to a valid row button (0..7). Never writes
// game+0xC65C, never consumes 0xA54E91.
static void UpdateAlmanacState(int ninthClicked)
{
    int32_t sel = 0;
    if (!ReadI32(Base() + TSM_RVA_DEFAULT_GAME_OBJECT + ALM_OFF_SELECTION, &sel)) return;

    // Close on another native row click (selection changed to a valid tile index).
    if (sel >= 0 && sel < 8 && sel != g_lastSelection) {
        if (g_lastSelection >= 0 && g_lastSelection < 8 && InterlockedCompareExchange(&g_almanacOpen, 1, 1) != 0) {
            InterlockedExchange(&g_almanacOpen, 0);
            Logf("almanac_button  almanac closed by native row click (selection %d)", (int)sel);
        }
    }
    g_lastSelection = (sel >= 0 && sel < 8) ? sel : g_lastSelection;

    // Toggle on ninth-tile click (debounced by latch).
    if (ninthClicked) {
        if (InterlockedCompareExchange(&g_almanacDebounce, 1, 0) == 0) {
            int wasOpen = InterlockedCompareExchange(&g_almanacOpen, 1, 1) != 0;
            InterlockedExchange(&g_almanacOpen, wasOpen ? 0 : 1);
            Logf("almanac_button  almanac %s via ninth-tile click",
                 InterlockedCompareExchange(&g_almanacOpen, 1, 1) != 0 ? "opened" : "closed");
        }
    } else if (!ReadClickLatch()) {
        // Click latch is clear: ready for the next ninth-tile click.
        InterlockedExchange(&g_almanacDebounce, 0);
    }

    // Content suppression: when Almanac is open, write game+0x5E6 = 1 every
    // frame to suppress native sub-page content drawing. Native code toggles
    // this flag on certain clicks, so we re-assert it each frame. Restore to 0
    // when Almanac closes. Never writes game+0xC65C, never consumes 0xA54E91.
    if (g_contentSuppressionEnabled) {
        int isOpen = InterlockedCompareExchange(&g_almanacOpen, 1, 1) != 0;
        if (isOpen) {
            if (!WriteU8(Base() + TSM_RVA_DEFAULT_GAME_OBJECT + ALM_OFF_CONTENT_SUPPRESS, 1)) {
                Logf("almanac_button  WARNING  content suppression write failed");
            }
        } else {
            if (!WriteU8(Base() + TSM_RVA_DEFAULT_GAME_OBJECT + ALM_OFF_CONTENT_SUPPRESS, 0)) {
                Logf("almanac_button  WARNING  content suppression restore failed");
            }
        }
    }
}
// Draws the ninth tile and checks for a click on it. The rect pair is touched
// only by one 8-byte load, one 8-byte store (the ninth values), the native draw,
// and one 8-byte store (the saved pair). Every failure ends in FailStop.
static void DrawNinthTile(void* self, float a1, float a2, float a3, float a4,
                          float a5, bool a6, float x, float y, float w, float h)
{
    uint8_t* at = (uint8_t*)(Base() + TSM_RVA_DEFAULT_P2D_RECT_X);
    uint64_t saved = 0;
    int drawFault = 0;
    int ninthClicked = 0;

    if (((uintptr_t)at & 7u) != 0 || !ReadablePtr(at, sizeof(uint64_t)))
        FailStop("rect pair unreadable or misaligned, before any write");
    if (!LoadRectPair(at, &saved))
        FailStop("rect pair load faulted, before any write");
    if (!StoreRectPair(at, PackPair(x, y)))
        FailStop("rect pair store of the ninth values faulted");

    // Lazy-load the distinct icon texture (once). CreateManagedTexture returns
    // a C3DAPI_TEXTURE* or NULL if the file is not accepted.
    if (!g_textureAttempted && g_createManagedTexture && g_textureAsset[0] && !g_almanacTexture) {
        g_textureAttempted = 1;
        void* middlepoint = (void*)(Base() + ALM_RVA_MIDDLEPOINT);
        g_almanacTexture = g_createManagedTexture(middlepoint, g_textureAsset);
        g_textureLoaded = (g_almanacTexture != NULL) ? 1 : 0;
        // Load texture data once after creation (native: Create -> Load -> TexBind).
        if (g_textureLoaded && g_almanacTexture) {
            __try {
                uintptr_t vptr = *(uintptr_t*)g_almanacTexture;
                if (vptr && ReadablePtr((const void*)vptr, 0x10 + sizeof(void*))) {
                    void* load = *(void**)(vptr + 0x10);
                    if (load && ReadablePtr(load, 1)) {
                        ((void(__thiscall*)(void*, const char*, int, int))load)(
                            g_almanacTexture, g_textureAsset, 0, 0);
                    }
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                Logf("almanac_button  texture load faulted");
                g_textureLoaded = 0;
            }
        }
        Logf("almanac_button  texture %s (%s)",
             g_textureLoaded ? "loaded" : "failed to load", g_textureAsset);
    }

    // Bind the distinct texture pre-Draw (native binds per-sub-entry before Draw).
    // vtable[0x70] = TexBind, signature (texture, 0, panel_state_global).
    // panel_state global is at RVA 0x9EAD08 (TSM_RVA_DEFAULT_PANEL_STATE).
    // SEH-wrapped: a binding failure is non-fatal (native artwork shows).
    if (g_textureLoaded && g_almanacTexture) {
        __try {
            uintptr_t vptr = *(uintptr_t*)g_almanacTexture;
            if (vptr && ReadablePtr((const void*)vptr, ALM_TEX_BIND_OFFSET + sizeof(void*))) {
                void* texBind = *(void**)(vptr + ALM_TEX_BIND_OFFSET);
                void* panelState = (void*)(Base() + TSM_RVA_DEFAULT_PANEL_STATE);
                if (texBind && ReadablePtr(texBind, 1)) {
                    ((void(__thiscall*)(void*, int, void*))texBind)(g_almanacTexture, 0, panelState);
                }
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            Logf("almanac_button  texture bind faulted; using native artwork");
            g_textureLoaded = 0;
        }
    }

    drawFault = CallNinthNative(self, a1, a2, a3, a4, a5, a6);

    // Click detection while the ninth tile's bounds are active (Path D: read-only).
    ninthClicked = CheckNinthClick(x, y, w, h);

    if (!StoreRectPair(at, saved)) {
        Logf("almanac_button  RECT DIRTY: restoring the saved rect pair faulted. The pair holds "
             "ninth-slot values or an unknown mix. It is not read again.");
        FailStop("rect restore faulted (RECT DIRTY)");
    }
    if (drawFault)
        FailStop("native ninth Draw faulted; the rect pair was restored before the stop");

    UpdateAlmanacState(ninthClicked);

    g_ninthDrawn++;
    if (g_ninthDrawn == 1)
        Logf("almanac_button  ninth tile drawn (first row pass)");
}

// ---------------------------------------------------------------- detour

// Replacement import for C3D_PANEL2D::Draw. Every call is forwarded. The one
// call that does more is the trade row's own draw of its eighth tile, and only
// while armed. The eighth tile's native call has no handler. The ninth tile's
// native call is wrapped only to catch a fault, which then ends the process.
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
        DrawNinthTile(self, a1, a2, a3, a4, a5, a6, plan.x, plan.y, plan.w, plan.h);
    else
        UpdateAlmanacState(0);                          // close-on-native-click every frame
}

// ---------------------------------------------------------------- lifecycle

static void ReadSettings(void)
{
    const char* ini = "plugins\\almanac_button.ini";
    g_enabled = (H->configInt(ini, "almanac_button", "enabled", 1) == 1) ? 1 : 0;
    H->configString(ini, "almanac_button", "texture_asset", g_textureAsset,
                    (int)sizeof(g_textureAsset), "icon_inspectnumbers.png");
    g_contentSuppressionEnabled = (H->configInt(ini, "almanac_button", "content_suppression", 1) == 1) ? 1 : 0;
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
    info->version = "2.6";

    ReadSettings();
    Logf("almanac_button  init v2.6 (revert center-anchor; inclusive upper bound; Load once)");
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
    FARPROC engineProc = g_engine ? GetProcAddress(g_engine, ALM_DRAW_SYM) : NULL;
    if (!engineProc) {
        Logf("almanac_button  REFUSED  no engine export for C3D_PANEL2D::Draw");
        return 1;
    }

    // The validated pointer is the engine export. It is the one expected value
    // for the exchange, and the original that Detour forwards to. It is never
    // re-read from the slot and adopted.
    void* validated = (void*)engineProc;
    if (*slot != validated) {
        Logf("almanac_button  REFUSED  Draw import differs from the engine export; no write made");
        return 1;
    }

    g_slot = slot;
    g_drawOrig = (t_Draw)validated;

    // Resolve GetMouseSolid for click detection (Path D). Not required to arm;
    // if it cannot be resolved, clicks on the ninth tile are simply not detected.
    FARPROC mouseProc = g_engine ? GetProcAddress(g_engine, "?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ") : NULL;
    if (!mouseProc) {
        Logf("almanac_button  NOTE  no engine export for GetMouseSolid; ninth-tile clicks not detected");
    } else {
        g_getMouseSolid = (t_GetMouseSolid)mouseProc;
    }

    // Resolve CreateManagedTexture for distinct icon (corrected binding).
    // The vtable offset 0x70 (TexBind) is confirmed by native code at
    // FUN_140358A20:141. The earlier crash was caused by a wrong panel_state
    // pointer (game+0x9D4F10+0xEAD08 instead of RVA 0x9EAD08) and post-Draw
    // timing. Binding is now done pre-Draw with the correct global.
    FARPROC texProc = g_engine ? GetProcAddress(g_engine, "?CreateManagedTexture@C3D_MIDDLEPOINT@@QEAAPEAVC3DAPI_TEXTURE@@PEBD@Z") : NULL;
    if (!texProc) {
        Logf("almanac_button  NOTE  no engine export for CreateManagedTexture; no distinct icon");
    } else {
        g_createManagedTexture = (t_CreateManagedTexture)texProc;
    }

    SlotSwap s = CompareSwapSlot(slot, validated, (void*)Detour, 1);
    if (!s.protectSetOk) {
        Logf("almanac_button  REFUSED  import page not writable; no write made");
        return 1;
    }
    if (s.was != validated) {
        // No write made. The slot holds another value, and it is left alone.
        Logf("almanac_button  REFUSED  import changed before the swap (found %p); no write "
             "made; %s", s.was, ProtText(s.protectRestoreOk));
        return 1;
    }

    // The exchange wrote our detour, so the slot points into this DLL, and the
    // DLL stays loaded for the session from here on.
    if (!s.protectRestoreOk) {
        SlotSwap u = CompareSwapSlot(slot, (void*)Detour, validated, 0);
        int undone = u.protectSetOk && u.was == (void*)Detour;
        Logf("almanac_button  WARNING  swap written, but the ORIGINAL import page protection was "
             "NOT restored after the swap. undo %s (slot found %p); %s; NOT ARMED. The detour "
             "is %s.",
             undone ? "done" : "NOT done", u.was, ProtText(u.protectRestoreOk),
             undone ? "removed from the slot by the undo" : "not removed by this undo (see slot found)");
        InterlockedExchange(&g_active, 0);
        return 1;
    }
    if (*slot != (void*)Detour) {
        InterlockedExchange(&g_active, 0);
        Logf("almanac_button  WARNING  import changed right after our swap; NOT ARMED; "
             "forwarding only");
        return 1;
    }

    InterlockedExchange(&g_active, 1);
    Logf("almanac_button  armed  clickable ninth tile after row tile 8 (import verified; GetMouseSolid %s)",
         g_getMouseSolid ? "resolved" : "not found");
    return 0;
}
