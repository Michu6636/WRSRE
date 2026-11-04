// map_expander - scales the vanilla 20 km world-size constants in SOVIET64.exe
// so maps with a larger $WORLDSIZE behave consistently.
//
// What it patches (RVAs into the exe image; ASLR-safe via g_exeBase):
//   0x90B1A4  20000.0f -> W              (world size)
//   0x90B180  10000.0f -> W / 2          (half world)
//   0x90B178  +9900.0f -> +(W/2 - 100)   (pollution emitter gate hi)
//   0x90BD2C  -9900.0f -> -(W/2 - 100)   (pollution emitter gate lo)
//   0x90B164  +8000.0f -> +(W/2 - 2000)  (terrain processing margin hi)
//   0x90BD20  -8000.0f -> -(W/2 - 2000)  (terrain processing margin lo)
//
// Step 1 semantic audit (scratch/audit_halfworld_*.asm): every sampled
// consumer of the shared 10000.0f is spatial - COMISS min/max clamp pairs
// (FUN_14010e1f0), grid-index-to-world math IMUL x0x2710 + ADDSS [10000]
// (FUN_1404b9140), terrain scatter col*20000/gridW - 10000 (FUN_14024ca10),
// editor bounds (FUN_1404fcaf0, x17 refs). No money/loan use: the treasury
// is a pair of doubles at game+0x580/0x588, wallets are small floats, and
// the loan UI constants live at 0x1408ADxxx - nowhere near 0x90B180.
// Global patching of 0x90B180 is therefore approved.
//
// Known residual (deliberately NOT patched): inline MOV 0x461C4000
// immediates in .text (FUN_140007d20 / FUN_140007ed0 emissive-map ortho
// bounds). A shared-constant patch cannot reach them; they keep 20 km
// ortho bounds until a follow-up .text patch. Gameplay-safe.

#include "../../src/tesmio_plugin.h"

#define RVA_WORLD_SIZE  (g_tsmAddr.world_size)
#define RVA_HALF_WORLD  (g_tsmAddr.half_world)
#define RVA_GATE_HI     (g_tsmAddr.gate_hi)
#define RVA_GATE_LO     (g_tsmAddr.gate_lo)
#define RVA_MARGIN_HI   (g_tsmAddr.margin_hi)
#define RVA_MARGIN_LO   (g_tsmAddr.margin_lo)

#define VANILLA_WORLD   20000.0f
#define VANILLA_HALF    10000.0f
#define VANILLA_GATE     9900.0f
#define VANILLA_MARGIN   8000.0f

static int   g_enabled   = 1;
static int   g_probe     = 1;
static float g_worldSize = 30000.0f;

static void ReadSettings()
{
    const char* ini = "plugins\\map_expander.ini";
    char v[32];

    g_enabled = H->configInt(ini, "map_expander", "enabled", 1);
    g_probe   = H->configInt(ini, "map_expander", "probe", 1);
    if (H->configString(ini, "map_expander", "world_size",
                        v, sizeof(v), "30000.0") && v[0])
    {
        char* end = nullptr;
        float w = strtof(v, &end);
        if (end == v || w < VANILLA_WORLD)
        {
            Logf("map_expander  WARN  world_size = '%s' is invalid (minimum %.1f) - clamped to %.1f",
                 v, (double)VANILLA_WORLD, (double)VANILLA_WORLD);
            w = VANILLA_WORLD;
        }
        else if (w > 40000.0f)
        {
            Logf("map_expander  WARN  world_size = %.1f exceeds 40000: projection far planes "
                 "(25000/15000) and the camera Y clamp (4544) stay vanilla",
                 (double)w);
        }
        g_worldSize = w;
    }
}

static bool ExpectFloat(DWORD rva, float want, const char* label)
{
    float got = *(const float*)(g_exeBase + rva);
    if (got != want)
    {
        Logf("map_expander  rva 0x%X is %.4f, expected %.4f (%s) - refusing (wrong game build?)",
             rva, (double)got, (double)want, label);
        return false;
    }
    return true;
}

struct Patch
{
    DWORD rva;
    float value;
    float oldValue;
    DWORD savedProt;
};

static int ApplyPatches(Patch* list, int n)
{
    // Phase 1: unprotect every site first. If any site refuses, restore the
    // ones already taken and write nothing - a half-patched world (W scaled
    // but HalfW vanilla) is worse than no patch at all.
    int i = 0;
    for (; i < n; i++)
    {
        BYTE* p = g_exeBase + list[i].rva;
        if (!ReadablePtr(p, sizeof(float)))
        {
            Logf("map_expander  rva 0x%X is not readable - refusing, nothing written",
                 list[i].rva);
            break;
        }
        DWORD prot = 0;
        if (!VirtualProtect(p, sizeof(float), PAGE_READWRITE, &prot))
        {
            Logf("map_expander  FAILED  VirtualProtect(%lu) at rva 0x%X - refusing, nothing written",
                 GetLastError(), list[i].rva);
            break;
        }
        list[i].savedProt = prot;
    }
    if (i < n)
    {
        for (int j = 0; j < i; j++)
        {
            DWORD tmp = 0;
            VirtualProtect(g_exeBase + list[j].rva, sizeof(float),
                           list[j].savedProt, &tmp);
        }
        return 0;
    }

    // Phase 2: every site is writable - commit the values.
    for (i = 0; i < n; i++)
    {
        float* p = (float*)(g_exeBase + list[i].rva);
        list[i].oldValue = *p;
        *p = list[i].value;
    }

    // Phase 3: restore protections. A restore failure is logged, not fatal -
    // the values are already committed.
    for (i = 0; i < n; i++)
    {
        DWORD tmp = 0;
        if (!VirtualProtect(g_exeBase + list[i].rva, sizeof(float),
                            list[i].savedProt, &tmp))
            Logf("map_expander  WARN  protection restore failed at rva 0x%X (%lu)",
                 list[i].rva, GetLastError());
    }
    return 1;
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "map_expander";
    info->version = "1.0";

    ReadSettings();
    if (!g_enabled)
    {
        Logf("map_expander  disabled");
        return 1;
    }
    if (!TsmCheckExeBuild(host, "map_expander"))
        return 1;
    if (!RVA_WORLD_SIZE || !RVA_HALF_WORLD || !RVA_GATE_HI ||
        !RVA_GATE_LO || !RVA_MARGIN_HI || !RVA_MARGIN_LO)
    {
        Logf("map_expander  required RVAs disabled in addresses.ini - inactive");
        return 1;
    }

    if (!ReadablePtr(g_exeBase + RVA_WORLD_SIZE, sizeof(float)) ||
        !ReadablePtr(g_exeBase + RVA_MARGIN_LO, sizeof(float)))
    {
        Logf("map_expander  patch region not readable - refusing (wrong game build?)");
        return 1;
    }

    // Fingerprint: every shared constant must still hold its vanilla value
    // before we touch anything. Strangers (another build) refuse here.
    if (!ExpectFloat(RVA_WORLD_SIZE, VANILLA_WORLD, "world size") ||
        !ExpectFloat(RVA_HALF_WORLD, VANILLA_HALF, "half world") ||
        !ExpectFloat(RVA_GATE_HI,    VANILLA_GATE, "emission gate hi") ||
        !ExpectFloat(RVA_GATE_LO,   -VANILLA_GATE, "emission gate lo") ||
        !ExpectFloat(RVA_MARGIN_HI,  VANILLA_MARGIN, "terrain margin hi") ||
        !ExpectFloat(RVA_MARGIN_LO, -VANILLA_MARGIN, "terrain margin lo"))
        return 1;

    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    float W     = g_worldSize;
    float halfW = W / 2.0f;
    float gate  = halfW - 100.0f;
    float marg  = halfW - 2000.0f;

    Patch list[6] = {
        { RVA_WORLD_SIZE, W,     0, 0 },
        { RVA_HALF_WORLD, halfW, 0, 0 },
        { RVA_GATE_HI,    gate,  0, 0 },
        { RVA_GATE_LO,   -gate,  0, 0 },
        { RVA_MARGIN_HI,  marg,  0, 0 },
        { RVA_MARGIN_LO, -marg,  0, 0 },
    };

    if (g_probe)
        Logf("map_expander  probe  world=%.1f half=%.1f gate=+-%.1f margin=+-%.1f",
             (double)W, (double)halfW, (double)gate, (double)marg);

    if (!ApplyPatches(list, 6))
    {
        Logf("map_expander  patch aborted, nothing written - inactive");
        return 1;
    }

    if (g_probe)
    {
        Logf("map_expander  patch  0x%X: %.1f -> %.1f", RVA_WORLD_SIZE,
             (double)list[0].oldValue, (double)list[0].value);
        Logf("map_expander  patch  0x%X: %.1f -> %.1f", RVA_HALF_WORLD,
             (double)list[1].oldValue, (double)list[1].value);
        Logf("map_expander  patch  0x%X/0x%X: +-%.1f -> +-%.1f", RVA_GATE_HI, RVA_GATE_LO,
             (double)VANILLA_GATE, (double)gate);
        Logf("map_expander  patch  0x%X/0x%X: +-%.1f -> +-%.1f", RVA_MARGIN_HI, RVA_MARGIN_LO,
             (double)VANILLA_MARGIN, (double)marg);
        Logf("map_expander  NOTE  inline MOV 0x461C4000 in .text "
             "(FUN_140007d20/ed0 ortho bounds) NOT patched - emissive-map init keeps 20 km bounds");
    }

    Logf("map_expander  live v1.0: world %.1f (half %.1f, gate +-%.1f, margin +-%.1f)",
         (double)W, (double)halfW, (double)gate, (double)marg);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
