// graphics_hook.cpp - installing the per-frame callback and driving the panel.
//
// WHY A WORKER THREAD
//   TsmPluginStart runs from LoadPlugins, which runs from DllMain under the
//   loader lock. Anything that loads a DLL, creates a window or waits on the
//   graphics path can deadlock or stall the game's own start from there. The
//   install work is therefore handed to a short-lived thread that installs its
//   hooks and exits. Nothing on that thread touches D3D11 or ImGui: the panel is
//   created later, on the game's own present thread, inside the first Present we
//   are called from.
//
// WHY PRESENT
//   Present is the last thing the game does with a frame, and it runs on the
//   thread that owns the D3D11 immediate context. Drawing there means the panel
//   is composited into the game's own back buffer, inside the game window, with
//   no second window, no second swap chain and nothing for the compositor to
//   fight with.
//
// WHAT IS NOT HOOKED
//   ResizeBuffers, Present1 and the device creation of any chain that is not the
//   game's. The panel rebuilds its render-target view from GetBuffer(0) every
//   frame, so a resize needs no interception, and a device removal stops drawing
//   rather than guessing.
//
// FAILURE POLICY
//   Each step logs and either continues in a weaker mode or stops drawing
//   permanently. Nothing is ever un-hooked, because the game may be inside a
//   callback that we installed; the DLL therefore stays resident for the
//   process, as the host requires once anything has been patched.
#include "graphics_hook.h"

#include "almanac_panel.h"
#include "almanac_panel_view.h"
#include "../../src/b2b_logistics_api.h"
#include "../../src/citizen_labor_api.h"
#include "../../src/currency_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/republic_bank_api.h"
#include "../../src/sim_clock_api.h"
#include "../../src/taxes_api.h"
#include "../../src/ui_overlay_api.h"

#include <atomic>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace almanac {
namespace {

constexpr const char* kLabel = "almanac overlay";

// Present's index in the IDXGISwapChain vtable. A fixed part of the DXGI
// interface: IUnknown's three slots, then SetPrivateData, SetPrivateDataInterface,
// GetPrivateData, GetParent, GetDevice, Present.
constexpr int kPresentVtableIndex = 8;

// Published before any detour that reads them becomes reachable, and never
// mutated afterwards.
const TsmHost* g_host      = nullptr;
unsigned       g_toggleKey = VK_F9;
bool           g_autoShow  = false;

using CreateAndSwapChainFn = HRESULT(WINAPI*)(IDXGIAdapter*, D3D_DRIVER_TYPE,
    HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT,
    const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**,
    D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
using PresentFn = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);

CreateAndSwapChainFn g_realCreate  = nullptr;   // path 1
PresentFn            g_realPresent = nullptr;   // whichever path installed

IDXGISwapChain* g_chain          = nullptr;     // one reference held
bool            g_presentHooked  = false;
int             g_path           = 0;           // 0 none, 1 creation, 2 vtable

// Set once when our Present slot is found to have been replaced by something
// else after we installed it. Our detour then stays reachable only through
// whatever that something else chains to, so drawing is no longer guaranteed
// and saying so beats a panel that silently stops appearing.
bool g_slotLost = false;

Panel g_panel;
bool  g_panelFailed = false;

// One line per session rather than one per frame.
bool g_loggedShown = false;

void Log(const char* fmt, ...)
{
    if (!g_host || !g_host->log) return;
    char body[512];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(body, sizeof(body), fmt, ap);
    va_end(ap);
    body[sizeof(body) - 1] = '\0';
    g_host->log("%s  %s", kLabel, body);
}

// The panel may only be bound to a top-level window of this process. A helper or
// foreign window would put our drawing somewhere we are not allowed to draw; a
// child window would hand us geometry that does not match the back buffer.
bool IsOurTopLevelWindow(HWND window)
{
    if (!window || !IsWindow(window)) return false;
    if (GetAncestor(window, GA_ROOT) != window) return false;
    DWORD processId = 0;
    GetWindowThreadProcessId(window, &processId);
    return processId == GetCurrentProcessId();
}

bool IsD3D11Chain(IDXGISwapChain* swap)
{
    ID3D11Device* device = nullptr;
    if (!swap || FAILED(swap->GetDevice(__uuidof(ID3D11Device),
                                        reinterpret_cast<void**>(&device)))) {
        return false;
    }
    if (device) device->Release();
    return true;
}

// A physical key level, sampled once per frame. The panel's toggle policy turns
// it into one edge per press; this is the only input this plugin reads.
bool ToggleKeyDown()
{
    return (GetAsyncKeyState(g_toggleKey) & 0x8000) != 0;
}

std::atomic<ULONGLONG> g_lastTerrainFrameMs{0};

float AlmanacWorldHeartbeat(void*, float, float y, float)
{
    g_lastTerrainFrameMs.store(GetTickCount64(), std::memory_order_relaxed);
    return y;
}

// Detect whether a game save/world is currently loaded.
// Uses a lightweight ui_overlay drawer (C3D_TERRAIN::Render heartbeat), which
// runs every frame in a loaded save (paused or unpaused) and never runs on the
// splash screen, loading screen, or main menu. Falls back to sim_clock if the
// HUD is hidden, or defaults to true if neither service is present.
bool IsWorldLoaded()
{
    static const TsmUiOverlayApi* s_uiOverlay = nullptr;
    static const TsmSimClockApi*  s_simClock  = nullptr;
    static bool s_lookedUp = false;

    if (!s_lookedUp && g_host && g_host->consume) {
        s_uiOverlay = static_cast<const TsmUiOverlayApi*>(
            g_host->consume(TSM_SERVICE_UI_OVERLAY, TSM_UI_OVERLAY_VERSION));
        if (s_uiOverlay && s_uiOverlay->registerDrawer) {
            s_uiOverlay->registerDrawer(&AlmanacWorldHeartbeat, nullptr);
        }
        s_simClock = static_cast<const TsmSimClockApi*>(
            g_host->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION));
        s_lookedUp = true;
    }

    if (s_uiOverlay) {
        const ULONGLONG last = g_lastTerrainFrameMs.load(std::memory_order_relaxed);
        if (last != 0 && (GetTickCount64() - last) < 1000ULL) {
            return true;
        }
    }

    if (s_simClock && s_simClock->getYear && s_simClock->getYear() >= 0) {
        return true;
    }

    return !s_uiOverlay && !s_simClock;
}

void AppendRow(TabData& tab, const char* label, const char* fmt, ...)
{
    if (tab.count >= TabData::kMaxRows) return;
    TabRow& row = tab.rows[tab.count++];
    std::snprintf(row.label, sizeof(row.label), "%s", label ? label : "");
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(row.value, sizeof(row.value), fmt, ap);
    va_end(ap);
    row.value[sizeof(row.value) - 1] = '\0';
}

const TsmSimClockApi*       g_svcClock      = nullptr;
const TsmCurrencyApi*       g_svcCurrency   = nullptr;
const TsmTaxesApi*          g_svcTaxes      = nullptr;
const TsmRepublicBankApi*   g_svcBank       = nullptr;
const TsmCitizenLaborApi*   g_svcLabor      = nullptr;
const TsmEnterpriseBaseApi* g_svcEnterprise = nullptr;
const TsmB2bLogisticsApi*   g_svcB2b        = nullptr;
bool                        g_servicesLookedUp = false;
ULONGLONG                   g_lastSampleMs     = 0;

void EnsureServicesLookedUp()
{
    if (g_servicesLookedUp || !g_host || !g_host->consume) return;
    g_svcClock = static_cast<const TsmSimClockApi*>(
        g_host->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION));

    g_svcCurrency = static_cast<const TsmCurrencyApi*>(
        g_host->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION));
    if (!g_svcCurrency) {
        g_svcCurrency = static_cast<const TsmCurrencyApi*>(
            g_host->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3));
    }

    g_svcTaxes = static_cast<const TsmTaxesApi*>(
        g_host->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION));
    if (!g_svcTaxes) {
        g_svcTaxes = static_cast<const TsmTaxesApi*>(
            g_host->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V2));
    }
    if (!g_svcTaxes) {
        g_svcTaxes = static_cast<const TsmTaxesApi*>(
            g_host->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V1));
    }

    g_svcBank = static_cast<const TsmRepublicBankApi*>(
        g_host->consume(TSM_SERVICE_REPUBLIC_BANK, TSM_REPUBLIC_BANK_VERSION));

    g_svcLabor = static_cast<const TsmCitizenLaborApi*>(
        g_host->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION));
    if (!g_svcLabor) {
        g_svcLabor = static_cast<const TsmCitizenLaborApi*>(
            g_host->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V4));
    }
    if (!g_svcLabor) {
        g_svcLabor = static_cast<const TsmCitizenLaborApi*>(
            g_host->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V3));
    }
    if (!g_svcLabor) {
        g_svcLabor = static_cast<const TsmCitizenLaborApi*>(
            g_host->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V2));
    }

    g_svcEnterprise = static_cast<const TsmEnterpriseBaseApi*>(
        g_host->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION));
    if (!g_svcEnterprise) {
        g_svcEnterprise = static_cast<const TsmEnterpriseBaseApi*>(
            g_host->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION_V7));
    }

    g_svcB2b = static_cast<const TsmB2bLogisticsApi*>(
        g_host->consume(TSM_SERVICE_B2B_LOGISTICS, TSM_B2B_LOGISTICS_VERSION));

    g_servicesLookedUp = true;
}

void ApplyPanelAction(const PanelAction& action)
{
    if (action.kind == PanelAction::Kind::None) return;
    EnsureServicesLookedUp();

    switch (action.kind) {
    case PanelAction::Kind::SetTaxEnabled:
        if (TsmTaxesHasPolicy(g_svcTaxes)) {
            g_svcTaxes->setTaxEnabled(action.index, action.flag ? 1 : 0);
        }
        break;
    case PanelAction::Kind::SetTaxRate:
        if (TsmTaxesHasPolicy(g_svcTaxes)) {
            g_svcTaxes->setTaxRate(action.index, action.value);
        }
        break;
    case PanelAction::Kind::SettleTaxesNow:
        if (g_svcTaxes && g_svcTaxes->settleNow) {
            g_svcTaxes->settleNow();
        }
        break;
    case PanelAction::Kind::SetBankRate:
        if (g_svcBank && g_svcBank->setAnnualInterestRate) {
            g_svcBank->setAnnualInterestRate(static_cast<double>(action.value));
        }
        break;
    case PanelAction::Kind::SetSectorWage:
        if (TsmLaborHasV4(g_svcLabor)) {
            g_svcLabor->setSectorWage(action.index, action.value);
        }
        break;
    case PanelAction::Kind::None:
        break;
    }

    // Force immediate telemetry refresh on the next frame.
    g_lastSampleMs = 0;
}

// Sample live read-only data from WRSRE services (throttled to ~4 Hz).
// Native C3D_LANGUAGE::GetString resolver (verified via Ghidra FUN_1403329E0):
//   - Language singleton: exeBase + 0x997590 (DAT_140997590)
//   - Export in C3DDLL64.dll: "?GetString@C3D_LANGUAGE@@QEAAPEB_WK@Z"
//     (also present at IAT slot exeBase + 0x86C890)
//   - Building+0x108 -> NamePoint* -> +0x10: wchar_t[128] city/town name
//   - Building+0x110 -> int32 instance index (when > 0, display appends " <n+1>")
//   - Building+0x318 -> TypeDescriptor*:
//       +0x200: int32 localized string ID ($NAME or $NAME_STR custom ID)
//       +0x204: int32 optional suffix string ID (used only when >= 0)
using LanguageGetStringFn = const wchar_t*(__fastcall*)(void* langThis, unsigned int id);

constexpr size_t kRvaLanguageSingleton = 0x997590;
constexpr size_t kRvaGetStringIat      = 0x86C890;

LanguageGetStringFn ResolveLanguageGetString()
{
    static LanguageGetStringFn s_fn = nullptr;
    static bool s_resolved = false;
    if (s_resolved) return s_fn;
    s_resolved = true;

    if (!g_host) return nullptr;
    if (g_host->engineModule) {
        s_fn = reinterpret_cast<LanguageGetStringFn>(
            GetProcAddress(static_cast<HMODULE>(g_host->engineModule),
                           "?GetString@C3D_LANGUAGE@@QEAAPEB_WK@Z"));
    }
    if (!s_fn && g_host->exeBase &&
        (!g_host->readablePtr ||
         g_host->readablePtr(g_host->exeBase + kRvaGetStringIat, sizeof(void*)))) {
        s_fn = *reinterpret_cast<LanguageGetStringFn const*>(
            g_host->exeBase + kRvaGetStringIat);
    }
    return s_fn;
}

bool IsLegitNameChar(wchar_t c)
{
    return (c >= 0x0020 && c <= 0x007E) ||
           (c >= 0x00C0 && c <= 0x024F) ||
           (c >= 0x0400 && c <= 0x04FF);
}

bool IsLetterChar(wchar_t c)
{
    return (c >= L'A' && c <= L'Z') ||
           (c >= L'a' && c <= L'z') ||
           (c >= 0x00C0 && c <= 0x024F) ||
           (c >= 0x0400 && c <= 0x04FF);
}

bool ValidateWideName(const wchar_t* ws, size_t maxScan, size_t* outLen)
{
    size_t len = 0;
    size_t letters = 0;
    while (len < maxScan) {
        const wchar_t c = ws[len];
        if (c == 0) break;
        if (!IsLegitNameChar(c)) return false;
        if (IsLetterChar(c)) ++letters;
        ++len;
    }
    if (len < 2 || len >= maxScan || ws[len] != 0 || letters < 2) return false;
    *outLen = len;
    return true;
}

// Reads the verified city/town name from bld+0x108 (NamePoint*) -> +0x10 (wchar_t[128]).
size_t ReadBuildingCityName(void* bldg, wchar_t* out, size_t maxChars)
{
    if (!bldg || !out || maxChars < 2) return 0;
    out[0] = L'\0';
    const uintptr_t addr = reinterpret_cast<uintptr_t>(bldg);
    if (addr < 0x10000ULL || addr > 0x00007FFFFFFEFFFFULL) return 0;
    if (g_host && g_host->readablePtr && !g_host->readablePtr(bldg, 0x110)) return 0;

    __try {
        const BYTE* b = static_cast<const BYTE*>(bldg);
        const void* np = *reinterpret_cast<void* const*>(b + 0x108);
        const uintptr_t npAddr = reinterpret_cast<uintptr_t>(np);
        if (npAddr < 0x10000ULL || npAddr > 0x00007FFFFFFEFFFFULL) return 0;
        if (g_host && g_host->readablePtr && !g_host->readablePtr(np, 0x110)) return 0;

        const wchar_t* ws = reinterpret_cast<const wchar_t*>(static_cast<const BYTE*>(np) + 0x10);
        size_t len = 0;
        if (!ValidateWideName(ws, 128, &len)) return 0;
        const size_t cp = (len < maxChars - 1) ? len : (maxChars - 1);
        for (size_t k = 0; k < cp; ++k) out[k] = ws[k];
        out[cp] = L'\0';
        return cp;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        out[0] = L'\0';
        return 0;
    }
}

void AppendWide(wchar_t* dst, size_t maxChars, size_t* pos, const wchar_t* src)
{
    if (!dst || !pos || !src || maxChars == 0) return;
    size_t i = *pos;
    while (*src && i + 1 < maxChars) {
        dst[i++] = *src++;
    }
    dst[i] = L'\0';
    *pos = i;
}

void AppendWideInt(wchar_t* dst, size_t maxChars, size_t* pos, int value)
{
    if (!dst || !pos || maxChars == 0) return;
    wchar_t tmp[16]{};
    int n = 0;
    unsigned int v = (value < 0) ? static_cast<unsigned int>(-value)
                                 : static_cast<unsigned int>(value);
    if (value < 0 && *pos + 1 < maxChars) {
        dst[(*pos)++] = L'-';
    }
    do {
        tmp[n++] = static_cast<wchar_t>(L'0' + (v % 10u));
        v /= 10u;
    } while (v > 0 && n < 15);
    while (n > 0 && *pos + 1 < maxChars) {
        dst[(*pos)++] = tmp[--n];
    }
    dst[*pos] = L'\0';
}

// Mirrors FUN_1403329E0 read-only:
//   "<City> <GetString(TD+0x200)>[ <GetString(TD+0x204)>][ <Building+0x110 + 1>]"
bool ReadNativeBuildingDisplayName(void* bldg, const wchar_t* cityName, size_t cityLen,
                                   wchar_t* out, size_t maxChars,
                                   int* outNameId, int* outSuffixId, int* outInstNum)
{
    if (!bldg || !out || maxChars < 2) return false;
    out[0] = L'\0';
    if (outNameId) *outNameId = -1;
    if (outSuffixId) *outSuffixId = -1;
    if (outInstNum) *outInstNum = 0;

    if (!g_host || !g_host->exeBase) return false;
    const LanguageGetStringFn getString = ResolveLanguageGetString();
    if (!getString) return false;

    const uintptr_t addr = reinterpret_cast<uintptr_t>(bldg);
    if (addr < 0x10000ULL || addr > 0x00007FFFFFFEFFFFULL) return false;
    if (g_host->readablePtr && !g_host->readablePtr(bldg, 0x320)) return false;

    void* langThis = g_host->exeBase + kRvaLanguageSingleton;
    if (g_host->readablePtr && !g_host->readablePtr(langThis, 0x10)) return false;

    __try {
        const BYTE* b = static_cast<const BYTE*>(bldg);
        const void* td = *reinterpret_cast<void* const*>(b + 0x318);
        const uintptr_t tdAddr = reinterpret_cast<uintptr_t>(td);
        if (tdAddr < 0x10000ULL || tdAddr > 0x00007FFFFFFEFFFFULL) return false;
        if (g_host->readablePtr && !g_host->readablePtr(td, 0x208)) return false;

        const BYTE* tdBytes = static_cast<const BYTE*>(td);
        const int32_t nameId   = *reinterpret_cast<const int32_t*>(tdBytes + 0x200);
        const int32_t suffixId = *reinterpret_cast<const int32_t*>(tdBytes + 0x204);
        const int32_t instNum  = *reinterpret_cast<const int32_t*>(b + 0x110);

        if (outNameId) *outNameId = static_cast<int>(nameId);
        if (outSuffixId) *outSuffixId = static_cast<int>(suffixId);
        if (outInstNum) *outInstNum = static_cast<int>(instNum);

        const wchar_t* typeStr = getString(langThis, static_cast<unsigned int>(nameId));
        if (!typeStr) return false;
        const uintptr_t tsAddr = reinterpret_cast<uintptr_t>(typeStr);
        if (tsAddr < 0x10000ULL || tsAddr > 0x00007FFFFFFEFFFFULL) return false;
        if (g_host->readablePtr && !g_host->readablePtr(typeStr, sizeof(wchar_t) * 2)) return false;
        if (typeStr[0] == L'\0') return false;

        const wchar_t* suffixStr = nullptr;
        if (suffixId >= 0) {
            const wchar_t* s = getString(langThis, static_cast<unsigned int>(suffixId));
            const uintptr_t sAddr = reinterpret_cast<uintptr_t>(s);
            if (s && sAddr >= 0x10000ULL && sAddr <= 0x00007FFFFFFEFFFFULL &&
                (!g_host->readablePtr || g_host->readablePtr(s, sizeof(wchar_t) * 2)) &&
                s[0] != L'\0') {
                suffixStr = s;
            }
        }

        size_t pos = 0;
        if (cityName && cityLen > 0) {
            AppendWide(out, maxChars, &pos, cityName);
            AppendWide(out, maxChars, &pos, L" ");
        }
        AppendWide(out, maxChars, &pos, typeStr);
        if (suffixStr) {
            AppendWide(out, maxChars, &pos, L" ");
            AppendWide(out, maxChars, &pos, suffixStr);
        }
        if (instNum > 0) {
            AppendWide(out, maxChars, &pos, L" ");
            AppendWideInt(out, maxChars, &pos, static_cast<int>(instNum + 1));
        }
        return pos > 0;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        out[0] = L'\0';
        return false;
    }
}

void FormatHumanizedObjectName(const char* objName, char* out, size_t outSize)
{
    if (!out || outSize == 0) return;
    out[0] = '\0';
    if (!objName || !objName[0]) return;

    const char* start = objName;
    for (const char* p = objName; *p; ++p) {
        if (*p == '/' || *p == '\\') start = p + 1;
    }

    size_t len = 0;
    while (start[len] != '\0') ++len;
    if (len > 4 && start[len - 4] == '.' &&
        (start[len - 3] == 'i' || start[len - 3] == 'I') &&
        (start[len - 2] == 'n' || start[len - 2] == 'N') &&
        (start[len - 1] == 'i' || start[len - 1] == 'I')) {
        len -= 4;
    }

    size_t w = 0;
    bool prevSpace = true;
    for (size_t i = 0; i < len && w + 1 < outSize; ++i) {
        char c = start[i];
        if (c == '_' || c == '-') c = ' ';
        if (c == ' ') {
            if (!prevSpace) {
                out[w++] = ' ';
                prevSpace = true;
            }
            continue;
        }
        if (w == 0 && c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
        out[w++] = c;
        prevSpace = false;
    }
    while (w > 0 && out[w - 1] == ' ') --w;
    out[w] = '\0';
}

const PanelModel* SampleLiveModel(bool worldLoaded)
{
    static PanelModel s_model{};

    if (!worldLoaded || !g_host || !g_host->consume) return nullptr;

    const ULONGLONG now = GetTickCount64();
    if (g_lastSampleMs != 0 && (now - g_lastSampleMs) < 250ULL && s_model.hasLiveData) {
        return &s_model;
    }
    g_lastSampleMs = now;

    EnsureServicesLookedUp();

    const TsmSimClockApi*       s_clock      = g_svcClock;
    const TsmCurrencyApi*       s_currency   = g_svcCurrency;
    const TsmTaxesApi*          s_taxes      = g_svcTaxes;
    const TsmRepublicBankApi*   s_bank       = g_svcBank;
    const TsmCitizenLaborApi*   s_labor      = g_svcLabor;
    const TsmEnterpriseBaseApi* s_enterprise = g_svcEnterprise;
    const TsmB2bLogisticsApi*   s_b2b        = g_svcB2b;

    const bool anyService = s_clock || s_currency || s_taxes || s_bank ||
                            s_labor || s_enterprise || s_b2b;
    if (!anyService) return nullptr;

    PanelModel m{};
    m.hasLiveData = true;
    m.dataNotice  = "Live republic telemetry & controls";

    const char* sym = TsmCurrencySymbol(s_currency);
    std::snprintf(m.controls.currencySymbol, sizeof(m.controls.currencySymbol),
                  "%s", sym);

    // --- Tab 0: Economy ---
    TabData& econ = m.tabs[0];
    if (s_clock && s_clock->getYear && s_clock->getMonth && s_clock->getDay) {
        const int y = s_clock->getYear();
        const int mo = s_clock->getMonth();
        const int d = s_clock->getDay();
        const bool paused = s_clock->isPaused && s_clock->isPaused() != 0;
        if (y >= 0) {
            AppendRow(econ, "Republic date", "Year %d, Month %d, Day %d%s",
                      y, mo, d + 1, paused ? " (paused)" : "");
        } else {
            AppendRow(econ, "Republic date", "Paused (waiting for first tick)");
        }
    }
    if (s_currency) {
        AppendRow(econ, "Active currency", "%s", sym);
        if (s_currency->getTreasuryBalance) {
            AppendRow(econ, "Treasury (RUB)", "%.2f RUB",
                      s_currency->getTreasuryBalance(TSM_CURRENCY_RUB));
            AppendRow(econ, "Treasury (USD)", "%.2f USD",
                      s_currency->getTreasuryBalance(TSM_CURRENCY_USD));
        }
        if (s_currency->getTotalWalletBalance) {
            AppendRow(econ, "Citizen wallets total", "%.2f %s",
                      s_currency->getTotalWalletBalance(), sym);
        }
    }
    if (s_taxes) {
        TsmTaxPolicySnapshot snap{};
        if (TsmTaxesHasPolicySnapshot(s_taxes) &&
            s_taxes->getPolicySnapshot(&snap, sizeof(snap))) {
            AppendRow(econ, "VAT", "%.1f%% (%s)",
                      snap.configured[TSM_TAX_VAT] * 100.0f,
                      snap.enabled[TSM_TAX_VAT] ? "enabled" : "suspended");
            AppendRow(econ, "Profit tax", "%.1f%% (%s)",
                      snap.configured[TSM_TAX_PROFIT] * 100.0f,
                      snap.enabled[TSM_TAX_PROFIT] ? "enabled" : "suspended");
            AppendRow(econ, "Property tax base", "%.2f %s (%s, %s)",
                      snap.configured[TSM_TAX_PROPERTY], sym,
                      snap.propertyLandValue ? "land value" : "flat",
                      snap.enabled[TSM_TAX_PROPERTY] ? "enabled" : "suspended");
            if (TsmTaxesHasPolicy(s_taxes)) {
                m.controls.hasTaxControls  = true;
                m.controls.vatEnabled      = snap.enabled[TSM_TAX_VAT] != 0;
                m.controls.vatRate         = snap.configured[TSM_TAX_VAT];
                m.controls.profitEnabled   = snap.enabled[TSM_TAX_PROFIT] != 0;
                m.controls.profitRate      = snap.configured[TSM_TAX_PROFIT];
                m.controls.propertyEnabled = snap.enabled[TSM_TAX_PROPERTY] != 0;
                m.controls.propertyBase    = snap.configured[TSM_TAX_PROPERTY];
            }
        } else {
            if (s_taxes->getVatRate) {
                AppendRow(econ, "VAT", "%.1f%%", s_taxes->getVatRate() * 100.0f);
            }
            if (s_taxes->getProfitTaxRate) {
                AppendRow(econ, "Profit tax", "%.1f%%", s_taxes->getProfitTaxRate() * 100.0f);
            }
        }
    }
    if (s_bank && s_bank->getAnnualInterestRate) {
        const double rate = s_bank->getAnnualInterestRate();
        AppendRow(econ, "Republic bank rate", "%.2f%% / yr", rate * 100.0);
        if (s_bank->setAnnualInterestRate) {
            m.controls.hasBankControl = true;
            m.controls.bankRate       = rate;
        }
    }

    // --- Tab 1: Labour ---
    TabData& labour = m.tabs[1];
    if (s_labor) {
        if (s_labor->getDefaultDailyWage) {
            AppendRow(labour, "Default daily wage", "%.2f %s / day",
                      s_labor->getDefaultDailyWage(), sym);
        }
        if (TsmLaborHasV3(s_labor)) {
            AppendRow(labour, "Statutory minimum wage", "%.2f %s / day",
                      s_labor->getMinimumWage(), sym);
            static const char* const kSectors[TSM_SECTOR_COUNT] = {
                "Sector: Heavy industry",
                "Sector: Infrastructure",
                "Sector: Care & Education",
                "Sector: Light industry",
                "Sector: Services",
                "Sector: Agriculture",
            };
            m.controls.minWage = s_labor->getMinimumWage();
            if (TsmLaborHasV4(s_labor)) {
                m.controls.hasSectorWages = true;
            }
            for (int s = 0; s < TSM_SECTOR_COUNT; ++s) {
                const float w = s_labor->getSectorWage(s);
                m.controls.sectorWages[s] = w;
                if (w > 0.0f) {
                    AppendRow(labour, kSectors[s], "%.2f %s / day", w, sym);
                }
            }
        }
        if (s_labor->getUnemployedWelfare) {
            AppendRow(labour, "Unemployed welfare", "%.2f %s / day",
                      s_labor->getUnemployedWelfare(), sym);
        }
        if (s_labor->getStudentStipend) {
            AppendRow(labour, "Student stipend", "%.2f %s / day",
                      s_labor->getStudentStipend(), sym);
        }
        if (s_labor->getRetireePension) {
            AppendRow(labour, "Retiree pension", "%.2f %s / day",
                      s_labor->getRetireePension(), sym);
        }
        if (TsmLaborHasV5(s_labor)) {
            AppendRow(labour, "Dynamic wage mobility", "%s",
                      s_labor->isWageMobilityHookInstalled() ? "active" : "static INI");
        }
    }

    // --- Tab 2: Logistics & Private Sector ---
    TabData& logi = m.tabs[2];
    if (s_enterprise) {
        if (s_enterprise->getEnterpriseCount) {
            AppendRow(logi, "Private enterprises", "%zu",
                      s_enterprise->getEnterpriseCount());
        }
        if (s_enterprise->getPrivateInvestmentPool) {
            AppendRow(logi, "Private investment pool", "%.2f %s",
                      s_enterprise->getPrivateInvestmentPool(), sym);
        }
        if (s_enterprise->getMonthlyPoolInflow) {
            AppendRow(logi, "Pool inflow (this month)", "%.2f %s",
                      s_enterprise->getMonthlyPoolInflow(), sym);
        }
        if (s_enterprise->getMonthlyEconomySummary) {
            double sales = 0.0, wages = 0.0, taxes = 0.0, inflow = 0.0;
            s_enterprise->getMonthlyEconomySummary(&sales, &wages, &taxes, &inflow);
            AppendRow(logi, "Last month private sales", "%.2f %s", sales, sym);
            AppendRow(logi, "Last month private wages", "%.2f %s", wages, sym);
            AppendRow(logi, "Last month private taxes", "%.2f %s", taxes, sym);
        }
        if (s_enterprise->getMonthlyEvictionCount) {
            AppendRow(logi, "Evictions (this month)", "%zu",
                      s_enterprise->getMonthlyEvictionCount());
        }
        if (s_enterprise->structSize >=
                offsetof(TsmEnterpriseBaseApi, getResidentialRentMonthly) +
                    sizeof(s_enterprise->getResidentialRentMonthly) &&
            s_enterprise->getResidentialRentMonthly) {
            AppendRow(logi, "Base residential rent", "%.2f %s / month",
                      s_enterprise->getResidentialRentMonthly(), sym);
        }
    }
    if (s_b2b) {
        AppendRow(logi, "B2B counterparty billing", "%s",
                  TsmB2bCounterpartyActive(s_b2b) ? "active" : "inactive");
    }

    // --- Tab 3: Enterprises Directory ---
    if (s_enterprise && s_enterprise->getEnterpriseCount &&
        s_enterprise->getEnterpriseByIndex && s_enterprise->describeEnterprise) {
        const size_t total = s_enterprise->getEnterpriseCount();
        m.enterprises.totalCount = static_cast<int>(total);
        const size_t limit = (total < static_cast<size_t>(EnterpriseList::kMaxEnterprises))
                                 ? total
                                 : static_cast<size_t>(EnterpriseList::kMaxEnterprises);
        for (size_t i = 0; i < limit; ++i) {
            void* bldg = s_enterprise->getEnterpriseByIndex(i);
            if (!bldg) continue;
            TsmEnterpriseInfo info{};
            info.structSize = sizeof(info);
            if (!s_enterprise->describeEnterprise(bldg, &info) || !info.isPrivate) {
                continue;
            }
            EnterpriseRow& row = m.enterprises.rows[m.enterprises.count++];
            row.solvent  = info.solvent != 0;
            row.capital  = info.capital;
            row.monthNet = info.monthNet;
            row.markup   = info.markup;
            row.debt     = 0.0;

            bool gotName = false;
            int nameId = -1;
            int suffixId = -1;
            int instNum = 0;
            wchar_t cityW[64]{};
            const size_t cityLen = ReadBuildingCityName(bldg, cityW, 64);
            char cityUtf8[48]{};
            if (cityLen > 0) {
                WideCharToMultiByte(CP_UTF8, 0, cityW, -1, cityUtf8,
                                    static_cast<int>(sizeof(cityUtf8)), nullptr, nullptr);
            }

            wchar_t wbuf[96]{};
            if (s_enterprise->getBuildingCustomName &&
                s_enterprise->getBuildingCustomName(bldg, wbuf, 96) > 0 && wbuf[0]) {
                const int n = WideCharToMultiByte(
                    CP_UTF8, 0, wbuf, -1, row.name,
                    static_cast<int>(sizeof(row.name)), nullptr, nullptr);
                if (n > 1) gotName = true;
            }
            if (!gotName &&
                ReadNativeBuildingDisplayName(bldg, cityW, cityLen, wbuf, 96,
                                              &nameId, &suffixId, &instNum)) {
                const int n = WideCharToMultiByte(
                    CP_UTF8, 0, wbuf, -1, row.name,
                    static_cast<int>(sizeof(row.name)), nullptr, nullptr);
                if (n > 1) gotName = true;
            }
            const char* objName = s_enterprise->getBuildingObjectName
                                      ? s_enterprise->getBuildingObjectName(bldg)
                                      : nullptr;
            if (!gotName && objName && objName[0]) {
                char prettyObj[48]{};
                FormatHumanizedObjectName(objName, prettyObj, sizeof(prettyObj));
                const char* typeLabel = prettyObj[0] ? prettyObj : objName;
                if (cityUtf8[0]) {
                    std::snprintf(row.name, sizeof(row.name), "%s %s", cityUtf8, typeLabel);
                } else {
                    std::snprintf(row.name, sizeof(row.name), "%s", typeLabel);
                }
                gotName = true;
            }
            if (!gotName) {
                std::snprintf(row.name, sizeof(row.name), "Enterprise #%zu", i + 1);
            }
            static int s_nameProbeCount = 0;
            if (s_nameProbeCount < 3) {
                ++s_nameProbeCount;
                Log("enterprise name probe: city=\"%s\" obj=\"%s\" nameId=%d suffixId=%d inst=%d resolved=\"%s\"",
                    cityUtf8[0] ? cityUtf8 : "(none)",
                    (objName && objName[0]) ? objName : "(none)",
                    nameId, suffixId, instNum, row.name);
            }

            if (s_bank && s_bank->getEnterpriseDebt) {
                double principal = 0.0, monthlyInt = 0.0;
                int missed = 0;
                s_bank->getEnterpriseDebt(bldg, &principal, &monthlyInt, &missed);
                row.debt = principal;
            }
        }
    }

    s_model = m;
    return &s_model;
}

// Drawing for one frame. Owns the "is the panel ready, and did it draw" state so
// both hook paths behave identically.
void DrawFrame(IDXGISwapChain* swap)
{
    if (g_panelFailed || !swap) return;

    const bool keyDown = ToggleKeyDown();
    if (!g_panel.Ready()) {
        PanelSettings settings;
        settings.autoShow  = g_autoShow;
        settings.toggleKey = static_cast<int>(g_toggleKey);
        if (!g_panel.Initialize(swap, settings)) {
            g_panelFailed = true;
            Log("panel initialization failed (device, window or font atlas); "
                "no further drawing this session");
            return;
        }
        Log("panel initialized; waiting for the first visible frame");
    }

    const bool worldLoaded = IsWorldLoaded();
    const PanelModel* liveModel = SampleLiveModel(worldLoaded);
    PanelAction action{};
    if (!g_panel.Render(swap, keyDown, worldLoaded, liveModel, &action)) {
        g_panelFailed = true;
        Log("frame failed (device removed or back buffer unavailable); drawing "
            "stopped, Present result unchanged");
        return;
    }
    ApplyPanelAction(action);
    if (g_panel.Visible() && !g_loggedShown) {
        g_loggedShown = true;
        Log("panel visible; key 0x%X hides and reopens it", g_toggleKey);
    }
}

// Path 2's stand-in for a creation event. One chain, one time; the reference is
// held for the session so the chain cannot be destroyed under the hook.
void AdoptOnFirstPresent(IDXGISwapChain* swap)
{
    if (g_chain || g_panelFailed || !swap || !IsD3D11Chain(swap)) return;

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swap->GetDesc(&desc))) return;
    if (!IsOurTopLevelWindow(desc.OutputWindow)) {
        Log("present on window %p ignored (not a top-level window of this "
            "process)", static_cast<void*>(desc.OutputWindow));
        return;
    }

    swap->AddRef();
    g_chain = swap;
    g_path  = 2;
    Log("adopted the presenting swap chain (hwnd %p)",
        static_cast<void*>(desc.OutputWindow));
}

// Declared here because the helpers below compare a slot against its address
// before the detour's body is reached.
HRESULT STDMETHODCALLTYPE HookPresent(IDXGISwapChain* swap, UINT sync, UINT flags);

// Write our detour into the chain's Present slot and remember the exact value it
// replaced. Used by both hook paths so the ownership check below has something
// definite to compare against.
bool InstallPresentSlot(IDXGISwapChain* swap, HWND window)
{
    void** vtable = *reinterpret_cast<void***>(swap);
    if (!vtable || !vtable[kPresentVtableIndex]) return false;

    // Store the previous target before publishing the replacement, so the
    // watchdog can tell "still ours" from "someone else took it".
    void* previous = vtable[kPresentVtableIndex];
    if (previous == reinterpret_cast<void*>(&HookPresent)) return true;   // ours

    g_realPresent = reinterpret_cast<PresentFn>(previous);
    DWORD protect = 0;
    if (!VirtualProtect(&vtable[kPresentVtableIndex], sizeof(void*),
                        PAGE_READWRITE, &protect)) {
        Log("Present slot on hwnd %p is not writable", static_cast<void*>(window));
        return false;
    }
    InterlockedExchangePointer(
        reinterpret_cast<void* volatile*>(&vtable[kPresentVtableIndex]),
        reinterpret_cast<void*>(&HookPresent));
    DWORD ignored = 0;
    VirtualProtect(&vtable[kPresentVtableIndex], sizeof(void*), protect, &ignored);

    g_presentHooked = true;
    return true;
}

// Is the slot still holding our detour?
//
// A vtable slot is a plain pointer that anything can write: another overlay
// installed after us, a graphics-hook library, or the driver's own wrapper may
// replace it, and nothing tells us when it happens. The visible result would be
// the panel disappearing with no log entry to explain it, which is the kind of
// silence this plugin exists to remove. One compare every frame is cheaper than
// that.
void CheckPresentSlotOwnership(IDXGISwapChain* swap)
{
    if (g_slotLost || !swap) return;

    void** vtable = *reinterpret_cast<void***>(swap);
    if (!vtable) return;
    void* current = vtable[kPresentVtableIndex];
    if (current == reinterpret_cast<void*>(&HookPresent)) return;

    g_slotLost = true;
    if (g_host && g_host->log) {
        Log("Present slot was taken over by %p after we installed; our hook is "
            "now only reached if that hook chains to it. The panel may stop "
            "appearing or flicker. Nothing here un-hooks or fights for the slot.",
            current);
    }
}

// Runs inside the game's Present, on the game's present thread, before the frame
// is handed to the driver. The original result and arguments are passed through
// untouched, so the game's own present handling and any other overlay see exactly
// what they would have seen. A drawing failure stops drawing; it never
// swallows or fakes a Present result.
HRESULT STDMETHODCALLTYPE HookPresent(IDXGISwapChain* swap, UINT sync, UINT flags)
{
    if (!(flags & DXGI_PRESENT_TEST)) {
        try {
            // The vtable path has no creation event to learn the chain from, so
            // the first chain that presents from a top-level window of this
            // process is taken as the game's. Later presents on it are ours;
            // everything else is forwarded untouched.
            if (!g_chain) AdoptOnFirstPresent(swap);
            if (swap == g_chain) {
                CheckPresentSlotOwnership(swap);
                DrawFrame(swap);
            }
        } catch (...) {
            g_panelFailed = true;
            Log("drawing threw; no further drawing this session");
        }
    }
    return g_realPresent(swap, sync, flags);
}

// Adopt `swap` if it is the game's, then replace its Present slot.
bool AdoptChain(IDXGISwapChain* swap)
{
    if (g_chain) return g_chain == swap;          // first one wins
    if (g_presentHooked) return false;
    if (!IsD3D11Chain(swap)) return false;

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swap->GetDesc(&desc))) return false;
    if (!IsOurTopLevelWindow(desc.OutputWindow)) {
        Log("chain on window %p ignored (not a top-level window of this "
            "process)", static_cast<void*>(desc.OutputWindow));
        return false;
    }

    swap->AddRef();                               // pinned for the session
    g_chain = swap;
    if (!InstallPresentSlot(swap, desc.OutputWindow)) {
        swap->Release();
        g_chain = nullptr;
        return false;
    }
    Log("Present hooked on the game's swap chain (hwnd %p)",
        static_cast<void*>(desc.OutputWindow));
    return true;
}

HRESULT WINAPI HookCreateDeviceAndSwapChain(
    IDXGIAdapter* adapter, D3D_DRIVER_TYPE driverType, HMODULE software,
    UINT flags, const D3D_FEATURE_LEVEL* levels, UINT levelCount, UINT sdkVersion,
    const DXGI_SWAP_CHAIN_DESC* desc, IDXGISwapChain** swapOut,
    ID3D11Device** deviceOut, D3D_FEATURE_LEVEL* levelOut,
    ID3D11DeviceContext** contextOut)
{
    HRESULT result = g_realCreate(adapter, driverType, software, flags, levels,
                                  levelCount, sdkVersion, desc, swapOut,
                                  deviceOut, levelOut, contextOut);
    if (SUCCEEDED(result) && swapOut && *swapOut) {
        try {
            AdoptChain(*swapOut);
        } catch (...) {
            Log("chain adoption threw; carrying on without the panel");
        }
    }
    return result;
}

// Path 1. Requires the engine's import slot to still hold the real d3d11.dll
// export: if something else already replaced it, that hook owns the call and
// taking it over would break it.
bool InstallCreationHook()
{
    void** slot = g_host->findIatSlot(g_host->engineModule, "d3d11.dll",
                                      "D3D11CreateDeviceAndSwapChain");
    if (!slot) {
        Log("C3DDLL64 does not import D3D11CreateDeviceAndSwapChain");
        return false;
    }

    // The real export address, from the real DLL, so an already-replaced slot is
    // detected rather than overwritten.
    HMODULE d3d11 = GetModuleHandleW(L"d3d11.dll");
    if (!d3d11) d3d11 = LoadLibraryW(L"d3d11.dll");
    void* expected = d3d11 ? reinterpret_cast<void*>(
        GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain")) : nullptr;
    if (!expected) {
        Log("cannot resolve the real d3d11.dll export; refusing to patch blind");
        return false;
    }
    if (*slot != expected) {
        Log("engine creation import already replaced by another mod; not "
            "stealing it");
        return false;
    }

    if (!g_host->patchIat(g_host->engineModule, "d3d11.dll",
                          "D3D11CreateDeviceAndSwapChain",
                          reinterpret_cast<void*>(&HookCreateDeviceAndSwapChain),
                          reinterpret_cast<void**>(&g_realCreate),
                          "Almanac overlay: graphics creation")) {
        Log("creation import hook failed");
        return false;
    }
    g_path = 1;
    Log("creation import hooked; waiting for the engine's swap chain");
    return true;
}

// Path 2. The vtable is shared by every IDXGISwapChain in the process, so this
// touches a slot the D3D runtime also owns. A throwaway chain is created to
// READ the slot, then destroyed; nothing is drawn on it and it is never
// presented. The creation call is made on the installer thread and under the
// same loader lock as everything else it does, which is why this runs from
// InstallThread and not from Start directly.
bool InstallVtableHook()
{
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferCount       = 1;
    desc.BufferDesc.Width  = 8;               // smallest legal back buffer
    desc.BufferDesc.Height = 8;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow      = GetConsoleWindow();   // null is allowed and unused
    desc.SampleDesc.Count  = 1;
    desc.Windowed          = TRUE;
    desc.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* throwaway = nullptr;

    // D3D_DRIVER_TYPE_NULL needs no adapter and no display, so this works on any
    // machine, including one where the game's own device is on another adapter.
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_NULL, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &desc, &throwaway, &device, nullptr, &context);
    if (FAILED(hr) || !throwaway) {
        Log("could not create a throwaway chain to locate Present (hr 0x%08lX)",
            static_cast<unsigned long>(hr));
        if (context) context->Release();
        if (device) device->Release();
        return false;
    }

    void** vtable  = *reinterpret_cast<void***>(throwaway);
    void*  present = vtable ? vtable[kPresentVtableIndex] : nullptr;
    if (!present) {
        throwaway->Release();
        if (context) context->Release();
        if (device) device->Release();
        Log("throwaway chain has no Present slot");
        return false;
    }

    // Write the slot while the scratch objects are still alive, then release
    // them. The vtable belongs to the D3D runtime's interface, not to any one
    // chain, which is exactly why this works for the game's chain too.
    const bool installed = InstallPresentSlot(throwaway, nullptr);

    throwaway->Release();
    if (context) context->Release();
    if (device) device->Release();

    if (!installed) {
        Log("Present vtable slot is not writable");
        return false;
    }
    g_path = 2;
    Log("Present hooked through the shared DXGI vtable at slot %d; the "
        "ownership check reports if anything replaces that slot later",
        kPresentVtableIndex);
    return true;
}

DWORD WINAPI InstallThread(LPVOID)
{
    // The engine imports d3d11.dll itself, so it is normally already loaded -
    // but "normally" is not a guarantee, and a patch attempted against a missing
    // module fails for a reason that looks like a different bug.
    for (int waited = 0; waited < 20 && !GetModuleHandleW(L"d3d11.dll"); ++waited)
        Sleep(250);

    // Path 1: follow the engine's own creation call. It needs no vtable write,
    // so it is both the more precise and the less invasive of the two.
    bool hooked = InstallCreationHook();

    if (hooked) {
        // Give the engine a few seconds to create its device. If it does, the
        // chain is ours and nothing else is needed. If it does not, it created
        // the device before this plugin was injected, and the vtable path is the
        // only remaining route at the moment something next presents.
        for (int waited = 0; waited < 20 && !g_chain && !g_panelFailed; ++waited)
            Sleep(250);
        if (g_chain) {
            Log("ready: creation hook adopted the engine's chain");
            return 0;
        }
        Log("the engine's device was created before this plugin was injected; "
            "falling back to the shared Present slot");
    }

    // Path 2: no cooperation needed from the engine, only from DXGI.
    hooked = InstallVtableHook();

    if (!hooked) {
        Log("no Present hook could be installed; the panel will not appear. This "
            "plugin stays loaded and does nothing.");
        return 0;
    }
    Log("ready: shared Present slot; the panel appears on the next presented "
        "frame of a top-level window in this process");
    return 0;
}

} // namespace

bool StartGraphicsHook(const TsmHost* host, unsigned toggleKey, bool autoShow)
{
    if (!host || !host->patchIat || !host->findIatSlot || !host->engineModule)
        return false;

    g_host      = host;
    g_toggleKey = toggleKey ? toggleKey : static_cast<unsigned>(VK_F9);
    g_autoShow  = autoShow;

    HANDLE thread = CreateThread(nullptr, 0, &InstallThread, nullptr, 0, nullptr);
    if (!thread) {
        Log("could not create the installer thread; nothing installed");
        return false;
    }
    CloseHandle(thread);
    return true;
}

} // namespace almanac
