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

#include <cstdarg>
#include <cstddef>
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
unsigned       g_toggleKey = VK_F8;
bool           g_autoShow  = true;

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

    if (!g_panel.Render(swap, keyDown)) {
        g_panelFailed = true;
        Log("frame failed (device removed or back buffer unavailable); drawing "
            "stopped, Present result unchanged");
        return;
    }
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
    g_toggleKey = toggleKey ? toggleKey : static_cast<unsigned>(VK_F8);
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
