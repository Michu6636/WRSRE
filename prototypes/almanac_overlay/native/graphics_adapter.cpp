// Original, opt-in candidate. NOT included by build.bat or plugin discovery.
#include "graphics_adapter.h"
#include <atomic>
#include <cstddef>
#include <mutex>
#include <new>

namespace almanac {
namespace {
using CreateFn = decltype(&D3D11CreateDeviceAndSwapChain);
using PresentFn = HRESULT (STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
CreateFn originalCreate = nullptr;
PresentFn originalPresent = nullptr;
std::atomic<bool> permission{false};
std::atomic<bool> healthy{true};
std::mutex installMutex, renderMutex;
bool installed = false;
HWND verifiedWindow = nullptr;
const TsmHost* host = nullptr;
// Process-lifetime references: no unsafe global ImGui/COM destructor at unload.
// There is deliberately no unload/recovery contract in this experiment yet.
IDXGISwapChain* selectedChain = nullptr;
D3D11Panel* panel = nullptr;
DWORD renderOwner = 0;
bool initializationAttempted = false;
thread_local bool drawing = false;

// Caller serializes writes. Output is published before the replacement becomes
// reachable. Expected-target CAS rejects a competing write, rather than stealing
// ownership. Restoration failure leaves the hook live but disables rendering.
bool ReplaceSlot(void** slot, void* expected, void* replacement, void** saved) {
    DWORD protection = 0;
    if (!VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &protection)) return false;
    *saved = expected;
    void* observed = InterlockedCompareExchangePointer(
        reinterpret_cast<PVOID volatile*>(slot), replacement, expected);
    DWORD ignored = 0;
    if (!VirtualProtect(slot, sizeof(void*), protection, &ignored)) healthy.store(false);
    return observed == expected;
}

HRESULT STDMETHODCALLTYPE PanelPresent(IDXGISwapChain* swap, UINT interval, UINT flags) {
    // The saved function is immutable after publication. Never fake success or
    // substitute Present1. The original result and arguments pass through.
    const bool nested = drawing;
    struct EntryScope {
        bool previous;
        ~EntryScope() { drawing = previous; }
    } entry{nested};
    drawing = true;
    if (!nested && !(flags & DXGI_PRESENT_TEST) && healthy.load()) {
        std::unique_lock<std::mutex> lock(renderMutex, std::try_to_lock);
        if (lock.owns_lock() && swap == selectedChain) {
            try {
                if (!initializationAttempted) {
                    initializationAttempted = true;
                    renderOwner = GetCurrentThreadId();
                    if (host->log) host->log("almanac graphics: first game Present thread=%lu chain=%p",
                        renderOwner, static_cast<void*>(swap));
                    panel = new D3D11Panel;
                    if (!panel->Initialize(swap)) healthy.store(false);
                }
                if (healthy.load() && GetCurrentThreadId() == renderOwner) {
                    // High bit is physical state. Existing HUD F8 remains
                    // unchanged; it does not consume this high-bit sample.
                    const bool down = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
                    if (!panel->Draw(swap, permission.load(), down)) healthy.store(false);
                } else if (GetCurrentThreadId() != renderOwner) {
                    healthy.store(false); // no owner migration guess
                }
            } catch (...) {
                // No retry/unload/destruction after a possibly partial frame.
                healthy.store(false);
                permission.store(false);
            }
        }
    }
    return originalPresent(swap, interval, flags);
}

HRESULT WINAPI ObserveCreation(IDXGIAdapter* adapter, D3D_DRIVER_TYPE type,
    HMODULE software, UINT flags, const D3D_FEATURE_LEVEL* levels, UINT count,
    UINT sdk, const DXGI_SWAP_CHAIN_DESC* desc, IDXGISwapChain** swap,
    ID3D11Device** device, D3D_FEATURE_LEVEL* level, ID3D11DeviceContext** context) {
    const HRESULT result = originalCreate(adapter, type, software, flags, levels,
        count, sdk, desc, swap, device, level, context);
    if (FAILED(result) || !swap || !*swap || !desc || !healthy.load()) return result;
    try {
    // Select only the externally verified HWND; it may still be hidden during
    // creation. Draw separately gates focus/visibility at Present time.
    DWORD process = 0;
    GetWindowThreadProcessId(desc->OutputWindow, &process);
    if (!permission.load() || desc->OutputWindow != verifiedWindow || process != GetCurrentProcessId())
        return result;
    std::lock_guard<std::mutex> lock(renderMutex);
    if (selectedChain) return result;
    auto** table = *reinterpret_cast<void***>(*swap);
    void* expected = table[8];
    if (!expected || expected == reinterpret_cast<void*>(PanelPresent)) return result;
    // Keep chain alive before publishing a reachable hook.
    selectedChain = *swap;
    selectedChain->AddRef();
    if (!ReplaceSlot(&table[8], expected, reinterpret_cast<void*>(PanelPresent),
                     reinterpret_cast<void**>(&originalPresent))) {
        selectedChain->Release();
        selectedChain = nullptr;
        healthy.store(false);
    }
    if (host->log) host->log("almanac graphics: creation observed; Present installed=%d healthy=%d",
        selectedChain != nullptr, healthy.load());
    } catch (...) {
        healthy.store(false);
        permission.store(false);
    }
    return result;
}
} // namespace

void SetGraphicsAdapterPermission(bool allowed) { permission.store(allowed); }

bool InstallGraphicsAdapter(const TsmHost* api, HWND verifiedGameWindow, bool developmentOptIn) {
    std::lock_guard<std::mutex> lock(installMutex);
    if (installed || !healthy.load() || !IsWindow(verifiedGameWindow) || !developmentOptIn || !api ||
        api->apiVersion != TSM_API_VERSION ||
        api->structSize < offsetof(TsmHost, findIatSlot) + sizeof(api->findIatSlot) ||
        !api->findIatSlot || !api->engineModule) return false;
    void** slot = api->findIatSlot(api->engineModule, "d3d11.dll", "D3D11CreateDeviceAndSwapChain");
    HMODULE d3d = GetModuleHandleW(L"d3d11.dll");
    void* expected = d3d ? reinterpret_cast<void*>(GetProcAddress(d3d, "D3D11CreateDeviceAndSwapChain")) : nullptr;
    if (!slot || !expected || *slot != expected) return false;
    // Pin BEFORE any detour is reachable. A later failure may still leave this
    // DLL pinned, intentionally. Never return an unload-safe claim after writing.
    HMODULE pinned = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
        reinterpret_cast<LPCWSTR>(&InstallGraphicsAdapter), &pinned)) return false;
    host = api;
    verifiedWindow = verifiedGameWindow;
    installed = ReplaceSlot(slot, expected, reinterpret_cast<void*>(ObserveCreation),
                            reinterpret_cast<void**>(&originalCreate));
    if (host->log) host->log("almanac graphics: creation observer installed=%d healthy=%d; late load has no fallback",
        installed, healthy.load());
    return installed; // true means reachable, NOT drawing-ready or safe to unload
}
} // namespace almanac
