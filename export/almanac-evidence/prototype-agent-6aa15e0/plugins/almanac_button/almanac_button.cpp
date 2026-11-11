// almanac_button - draw-only ninth backdrop beside the Economy & Trade row.
//
// WHAT IT DOES
//   In the verified normal configuration the native trade row draws eight
//   tiles. This plugin draws one more game-style tile backdrop after the
//   eighth, at the next slot. Nothing else. It has no click handling, no input
//   observer, no reads or writes of the click latch (0xA54E91), no writes to
//   native selection, no panel and no content. The ninth tile is NOT
//   interactive.
//
// DEFAULT
//   plugins\almanac_button.ini: enabled = 0. With the default, Init declines,
//   the loader unloads the DLL, and nothing is patched.
//
// MECHANISM
//   One import swap. The row handler (FUN_140373EA0) calls
//   C3D_PANEL2D::Draw through the import slot at RVA 0x86C860, from the call
//   at RVA 0x3743A2. The detour forwards every call unchanged. It acts only
//   when the return address is 0x3743A8 (the row's own call) and `this` is the
//   trade widget at RVA 0x9BE060. The tile position is read from the rect
//   store at RVA 0x9BE2F0/0x9BE2F4. Row decisions come from
//   almanac_button_geometry.h.
//
// VERIFIED AT START (refuses with a log line otherwise)
//   - exe stamp matches the repo baseline (TsmCheckExeBuild);
//   - bytes at RVA 0x31EDEF are E8 AC 50 05 00 (CALL FUN_140373EA0);
//   - bytes at RVA 0x3743A2 are FF 15 B8 84 4F 00 (CALL [slot 0x86C860]);
//   - the import slot is at RVA 0x86C860 and holds the engine's exported
//     C3D_PANEL2D::Draw;
//   - the origin and pitch constants read as 45, 85 and 64.
//
// RESTORATION
//   The loader has no shutdown hook for IAT swaps. On any fault the detour
//   restores the original import pointer and stops drawing for the session.
//   To turn the feature off: set enabled = 0, then restart the game.
//
// STATUS
//   Only the pure geometry logic is host-tested (g++). This Windows glue is
//   NOT compiled or run in this session. The ninth position, its visibility
//   and its clipping are NOT established.
//
// Version 1.0: first draw-only prototype.

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
#define ALM_OFF_SELECTOR   0x10398u
#define ALM_OFF_C7D4       0xC7D4u
#define ALM_OFF_SKIP578    0x578u
#define ALM_OFF_SKIP5E0    0x5E0u

typedef void (*t_Draw)(void* self, float a1, float a2, float a3,
                       float a4, float a5, bool a6);

static int                        g_enabled;
static volatile LONG              g_active;      // 0 = pure forwarder
static t_Draw                     g_drawOrig;    // the engine's Draw
static almanac_button::RowTracker g_tracker;
static int                        g_ninthDrawn;
static int                        g_noteCount;
static const char*                g_lastNote;

// Plain-old-data plan, filled before the native call and used after it.
typedef struct NinthPlan {
    int   draw;
    float x;
    float y;
} NinthPlan;

static uintptr_t Base(void) { return (uintptr_t)g_exeBase; }

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

// Game-object fields. The count is derived from the trade subvector the
// handler uses: (end - begin) / 0x50, with begin at +0x48 and end at +0x50.
// Reading this layout is an assumption from the static trace, so a bad read
// yields a count other than 8 and the tracker stays idle (fails safe).
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
    return ReadI32(game + ALM_OFF_SELECTOR, &g->selector) &&
           ReadI32(game + ALM_OFF_C7D4, &g->c7d4) &&
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

// Byte and value checks for this exact build. Nothing is patched unless all
// of them pass.
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

// ---------------------------------------------------------------- fault path

// Puts the engine's Draw back in the import slot and stops all drawing. Called
// only from exception handlers. Idempotent.
static void DisarmAfterFault(const char* where)
{
    if (InterlockedExchange(&g_active, 0) == 0) return;
    void* dummy = NULL;
    PatchIat(g_exe, DLL_ENGINE, ALM_DRAW_SYM, (void*)g_drawOrig, &dummy,
             "restore C3D_PANEL2D::Draw");
    Logf("almanac_button  FAULT in %s - original Draw restored, drawing stopped", where);
}

// ---------------------------------------------------------------- row logic

// Decides for the row tile about to be drawn. Reads only; no native state is
// written. Runs before the native draw, which does not change the row state it
// reads. SEH function: uses only POD locals.
static void PlanRowTile(NinthPlan* plan)
{
    plan->draw = 0;
    plan->x = 0.0f;
    plan->y = 0.0f;
    __try {
        float tx = 0.0f, ty = 0.0f;
        almanac_button::RowConstants c;
        almanac_button::RowGate g;
        if (!ReadTileRect(&tx, &ty) || !ReadRowConstants(&c) || !ReadGate(&g)) {
            Note("row state unreadable; idle");
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
        DisarmAfterFault("PlanRowTile");
        plan->draw = 0;
    }
}

// Draws the ninth backdrop tile through the engine's own Draw, with the rect
// store set to the ninth slot for that one call. The store range
// 0x9BE2E8..0x9BE2FB (20 bytes) is saved first and restored afterwards.
// Limitation: the ninth tile reuses the eighth tile's hover state.
static void DrawNinthTile(void* self, float a1, float a2, float a3, float a4,
                          float a5, bool a6, float x, float y)
{
    uint8_t saved[20];
    uint8_t* rect = (uint8_t*)(Base() + TSM_RVA_DEFAULT_P2D_RECT_W);
    int savedOk = 0;
    __try {
        if (!ReadablePtr(rect, sizeof(saved))) return;
        memcpy(saved, rect, sizeof(saved));
        savedOk = 1;
        *(float*)(rect + 8)  = x;   // 0x9BE2F0
        *(float*)(rect + 12) = y;   // 0x9BE2F4
        g_drawOrig(self, a1, a2, a3, a4, a5, a6);
        memcpy(rect, saved, sizeof(saved));
        g_ninthDrawn++;
        if (g_ninthDrawn == 1)
            Logf("almanac_button  ninth tile drawn (first row pass)");
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (savedOk) memcpy(rect, saved, sizeof(saved));
        DisarmAfterFault("DrawNinthTile");
    }
}

// ---------------------------------------------------------------- detour

// Replacement import for C3D_PANEL2D::Draw. Every call is forwarded. The one
// call that does more is the trade row's own draw of its eighth tile.
__declspec(noinline) static void Detour(void* self, float a1, float a2, float a3,
                                        float a4, float a5, bool a6)
{
    void* ret = _ReturnAddress();
    if (InterlockedCompareExchange(&g_active, 1, 1) == 0 || !g_drawOrig ||
        ret != (void*)(Base() + ALM_RVA_ROW_RET) ||
        self != (void*)(Base() + TSM_RVA_DEFAULT_PANEL2D)) {
        if (g_drawOrig) g_drawOrig(self, a1, a2, a3, a4, a5, a6);
        return;
    }
    NinthPlan plan;
    PlanRowTile(&plan);
    g_drawOrig(self, a1, a2, a3, a4, a5, a6);          // native tile, always
    if (plan.draw && InterlockedCompareExchange(&g_active, 1, 1) != 0)
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
    info->version = "1.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("almanac_button  disabled (enabled = 0)");
        return 1;                       // loader unloads; nothing is patched
    }
    return 0;
}

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

    if (!PatchIat(g_exe, DLL_ENGINE, ALM_DRAW_SYM, (void*)Detour,
                  (void**)&g_drawOrig, "C3D_PANEL2D::Draw")) {
        Logf("almanac_button  REFUSED  could not swap the Draw import");
        return 1;
    }
    InterlockedExchange(&g_active, 1);
    Logf("almanac_button  armed  draw-only ninth backdrop after row tile 8");
    return 0;
}
