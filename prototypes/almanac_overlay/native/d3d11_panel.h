// Development candidate only. No hook installation or normal plugin build.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <chrono>
#include "../almanac_debug_toggle.h"
struct ImGuiContext;
namespace almanac {
// Original wrapper around upstream ImGui, following Earthworks' real-chain
// drawing architecture. Uses GPL-derived panel styling; see ../earthworks/NOTICE.md.
// All calls, including destruction, MUST run on the same verified render owner.
// Caller must serialize with native resize/device teardown and other context users.
class D3D11Panel {
public:
    D3D11Panel() = default;
    ~D3D11Panel();
    D3D11Panel(const D3D11Panel&) = delete;
    D3D11Panel& operator=(const D3D11Panel&) = delete;

    // Explicit initialization only on an externally verified game swap chain.
    // Never guesses from first Present or reads Beam's reported global pointers.
    bool Initialize(IDXGISwapChain* swap);
    // Must be called even while hidden to track release/permission loss.
    // f8Down is PHYSICAL down state, not GetAsyncKeyState's unreliable low bit.
    // allowed includes development opt-in, focus/modal/readiness permission.
    // False means unavailable/fault: adapter must bypass overlay, not skip Present.
    bool Draw(IDXGISwapChain* swap, bool allowed, bool f8Down);
    void Shutdown(); // no hook removal; caller retains interception ownership
private:
    Microsoft::WRL::ComPtr<IDXGISwapChain> swap_;
    Microsoft::WRL::ComPtr<ID3D11Device> device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
    ImGuiContext* gui_ = nullptr;
    HWND window_ = nullptr;
    DWORD owner_ = 0;
    bool backend_ = false;
    DebugOverlayToggle toggle_;
    std::chrono::steady_clock::time_point last_{};
};
}
