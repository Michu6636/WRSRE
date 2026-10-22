// ui_overlay - the HUD drawing and toggle service.
//
// See ui_overlay_api.h for the contract. This file is the implementation:
// one hook on C3D_TERRAIN::Render, one import lookup for the non-variadic
// text printer, one DPI read per frame, a drawer table that fires in
// registration order, and a hotkey that decides whether any of it is
// drawn.

#include "../../src/tesmio_plugin.h"
#include "../../src/ui_overlay_api.h"

#pragma comment(lib, "user32.lib")

// ---------------------------------------------------------------- sites

#define RVA_FONTMGR    0x996FB0
#define RVA_FONT       0x995220
#define RVA_DPI        0x992088

#define SYM_TERRAIN_RENDER "?Render@C3D_TERRAIN@@QEAAX_NPEAVC3D_CAMERA@@0HH@Z"
#define SYM_PRINT_NOARG    "?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z"

// ---------------------------------------------------------------- types

typedef void (*t_TerrainRender)(void* self, bool a, void* cam1, void* cam2,
                                int b, int c);
typedef void (*t_PrintNoArg)(void* fontMgr, void* font,
                             float x, float y, unsigned long color,
                             const wchar_t* text);

static t_TerrainRender o_TerrainRender;
static t_PrintNoArg    o_Print;

// ---------------------------------------------------------------- config

static int   g_enabled        = 1;
static int   g_probe          = 1;
static int   g_visible        = 1;
static int   g_toggleKey      = 0x77;   // F8
static float g_columnX        = 28.0f;
static float g_columnY        = 85.0f;
static float g_lineHeight     = 17.5f;

// ---------------------------------------------------------------- drawers

struct Drawer {
    TsmHudDrawer cb;
    void*        userData;
    int          active;
};

#define MAX_DRAWERS 32
static Drawer g_drawers[MAX_DRAWERS];
static int    g_drawerCount;

static int RegisterDrawer(TsmHudDrawer cb, void* userData)
{
    if (!cb) return 0;
    if (g_drawerCount >= MAX_DRAWERS) {
        Logf("ui_overlay  drawer table full (%d) - refusing", MAX_DRAWERS);
        return 0;
    }
    int i = g_drawerCount++;
    g_drawers[i].cb       = cb;
    g_drawers[i].userData = userData;
    g_drawers[i].active   = 1;
    return i + 1;
}

static void UnregisterDrawer(int handle)
{
    if (handle < 1 || handle > g_drawerCount) return;
    g_drawers[handle - 1].active = 0;
}

// ---------------------------------------------------------------- text

static void DrawTextImpl(float x, float y, unsigned long color,
                         const wchar_t* text)
{
    if (!o_Print || !text) return;

    void* font = *(void**)(g_exeBase + RVA_FONT);
    if (!font) return;

    o_Print(g_exeBase + RVA_FONTMGR, font, x, y, color, text);
}

static void DrawTextOutlinedImpl(float x, float y, unsigned long color,
                                 const wchar_t* text)
{
    if (!o_Print || !text) return;

    void* font = *(void**)(g_exeBase + RVA_FONT);
    if (!font) return;

    float dpi = *(float*)(g_exeBase + RVA_DPI);
    if (!(dpi > 0.2f && dpi < 8.0f)) dpi = 1.0f;

    const float o = 1.6f * dpi;
    void* fm = g_exeBase + RVA_FONTMGR;

    o_Print(fm, font, x - o, y,     0xFF000000u, text);
    o_Print(fm, font, x + o, y,     0xFF000000u, text);
    o_Print(fm, font, x,     y - o, 0xFF000000u, text);
    o_Print(fm, font, x,     y + o, 0xFF000000u, text);
    o_Print(fm, font, x,     y,     color,       text);
}

static float GetDPIImpl(void)
{
    float d = *(float*)(g_exeBase + RVA_DPI);
    if (!(d > 0.2f && d < 8.0f)) return 1.0f;
    return d;
}

static float GetColumnXImpl(void) { return g_columnX; }

// ---------------------------------------------------------------- visibility

static int  IsVisibleImpl(void)            { return g_visible; }
static void SetVisibleImpl(int visible)    { g_visible = visible ? 1 : 0; }

// ---------------------------------------------------------------- the hook

static void h_TerrainRender(void* self, bool a, void* cam1, void* cam2,
                            int b, int c)
{
    o_TerrainRender(self, a, cam1, cam2, b, c);

    if (!g_enabled) return;

    if (g_toggleKey > 0) {
        SHORT s = GetAsyncKeyState(g_toggleKey);
        if ((s & 0x8000) && (s & 0x0001)) {
            g_visible = !g_visible;
            if (g_probe)
                Logf("ui_overlay  HUD %s", g_visible ? "shown" : "hidden");
        }
    }

    if (!g_visible) return;

    float dpi  = GetDPIImpl();
    float x    = g_columnX * dpi;
    float y    = g_columnY * dpi;
    float step = g_lineHeight * dpi;

    __try
    {
        for (int i = 0; i < g_drawerCount; i++) {
            Drawer* d = &g_drawers[i];
            if (!d->active) continue;
            __try { y = d->cb(d->userData, x, y, step); }
            __except (FaultFilter("ui_overlay drawer",
                                  GetExceptionInformation()))
            {
                Logf("ui_overlay  a drawer faulted; disabling it for "
                     "this session");
                d->active = 0;
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        Logf("ui_overlay  the drawer walk faulted - skipped this frame");
    }
}

// ---------------------------------------------------------------- the API

static const TsmUiOverlayApi kApi = {
    sizeof(TsmUiOverlayApi),
    DrawTextImpl,
    DrawTextOutlinedImpl,
    GetDPIImpl,
    GetColumnXImpl,
    IsVisibleImpl,
    SetVisibleImpl,
    RegisterDrawer,
    UnregisterDrawer,
};

// ---------------------------------------------------------------- settings

static void ReadSettings(void)
{
    const char* ini = "plugins\\ui_overlay.ini";
    char v[64];

    g_enabled = H->configInt(ini, "ui_overlay", "enabled", 1);
    g_probe   = H->configInt(ini, "ui_overlay", "probe",   1);

    H->configString(ini, "ui_overlay", "show_by_default",
                    v, sizeof(v), "1");
    g_visible = atoi(v) != 0;

    H->configString(ini, "ui_overlay", "toggle_key",
                    v, sizeof(v), "0x77");
    g_toggleKey = (int)strtol(v, NULL, 0);
    if (g_toggleKey < 0)     g_toggleKey = 0;
    if (g_toggleKey > 0xFE)  g_toggleKey = 0xFE;

    H->configString(ini, "ui_overlay", "column_x",
                    v, sizeof(v), "28.0");
    g_columnX = (float)atof(v);
    H->configString(ini, "ui_overlay", "column_y",
                    v, sizeof(v), "85.0");
    g_columnY = (float)atof(v);
    H->configString(ini, "ui_overlay", "line_height",
                    v, sizeof(v), "17.5");
    g_lineHeight = (float)atof(v);

    if (!(g_columnX    >= 0.0f && g_columnX    <= 2000.0f)) g_columnX    = 28.0f;
    if (!(g_columnY    >= 0.0f && g_columnY    <= 2000.0f)) g_columnY    = 85.0f;
    if (!(g_lineHeight >  0.0f && g_lineHeight <=   50.0f)) g_lineHeight = 17.5f;
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "ui_overlay";
    info->version = "1.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("ui_overlay  disabled");
        return 1;
    }

    H->provide(TSM_SERVICE_UI_OVERLAY, TSM_UI_OVERLAY_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    o_Print = (t_PrintNoArg)GetProcAddress(g_engine, SYM_PRINT_NOARG);
    if (!o_Print) {
        void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_PRINT_NOARG);
        if (slot) o_Print = (t_PrintNoArg)*slot;
    }
    if (!o_Print) {
        Logf("ui_overlay  no PrintLeftUnicodeNoArg - no HUD text this "
             "session");
        return 1;
    }

    if (!ReadablePtr(g_exeBase + RVA_FONT, sizeof(void*))) {
        Logf("ui_overlay  the panel font handle is not where this build "
             "puts it - inactive");
        return 1;
    }

    if (!PatchIat(g_exe, DLL_ENGINE, SYM_TERRAIN_RENDER,
                  (void*)h_TerrainRender, (void**)&o_TerrainRender,
                  "C3D_TERRAIN::Render"))
        return 1;

    Logf("ui_overlay  live: toggle 0x%02X, column (%.0f, %.0f), line %.1f",
         g_toggleKey, g_columnX, g_columnY, g_lineHeight);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }