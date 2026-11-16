// Original experimental wrapper; architecture reference: Earthworks overlay.cpp.
// No Present, IAT, WndProc, DirectInput or game-address patches in this file.
#include "d3d11_panel.h"
#include "../almanac_overlay_view.h"
#include "../earthworks/panel_style.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include <algorithm>
#include <cassert>

namespace almanac {
namespace {
// Rolls back partial backend/context initialization and restores the host
// context on normal returns AND C++ unwinding. Not an SEH recovery mechanism.
struct InitializationScope {
    ImGuiContext* previous;
    ImGuiContext* candidate;
    bool committed = false;
    ~InitializationScope() {
        if (!committed) {
            ImGui::SetCurrentContext(candidate);
            if (ImGui::GetIO().BackendRendererUserData) ImGui_ImplDX11_Shutdown();
            ImGui::DestroyContext(candidate);
        }
        ImGui::SetCurrentContext(previous);
    }
};
struct ContextScope {
    ImGuiContext* previous = ImGui::GetCurrentContext();
    explicit ContextScope(ImGuiContext* selected) { ImGui::SetCurrentContext(selected); }
    ~ContextScope() { ImGui::SetCurrentContext(previous); }
};
// Backend restores most pipeline state; the caller's OM targets are additional.
// Save ALL RTV slots, not just slot zero. No RTV survives the Draw call.
struct TargetsScope {
    ID3D11DeviceContext* context;
    ID3D11RenderTargetView* targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
    ID3D11DepthStencilView* depth = nullptr;
    explicit TargetsScope(ID3D11DeviceContext* value) : context(value) {
        context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, &depth);
    }
    ~TargetsScope() {
        context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, depth);
        for (auto* target : targets) if (target) target->Release();
        if (depth) depth->Release();
    }
};
}
D3D11Panel::~D3D11Panel() { Shutdown(); }

bool D3D11Panel::Initialize(IDXGISwapChain* swap) {
    if (gui_ || !swap) return false;
    DXGI_SWAP_CHAIN_DESC desc{};
    DWORD process = 0;
    if (FAILED(swap->GetDesc(&desc)) || !IsWindow(desc.OutputWindow)) return false;
    GetWindowThreadProcessId(desc.OutputWindow, &process);
    if (process != GetCurrentProcessId()) return false;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    if (FAILED(swap->GetDevice(IID_PPV_ARGS(device.GetAddressOf())))) return false;
    device->GetImmediateContext(context.GetAddressOf());
    if (!context) return false;
    ImGuiContext* previous = ImGui::GetCurrentContext();
    ImGuiContext* candidate = ImGui::CreateContext();
    if (!candidate) { ImGui::SetCurrentContext(previous); return false; }
    InitializationScope transaction{previous, candidate};
    ImGui::SetCurrentContext(candidate);
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags &= ~(ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad);
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ApplyEarthworksPanelStyle();
    if (!ImGui_ImplDX11_Init(device.Get(), context.Get())) return false;
    gui_ = candidate;
    backend_ = true;
    swap_ = swap;
    device_ = device;
    context_ = context;
    window_ = desc.OutputWindow;
    owner_ = GetCurrentThreadId();
    last_ = std::chrono::steady_clock::now();
    transaction.committed = true;
    return true;
}

bool D3D11Panel::Draw(IDXGISwapChain* swap, bool allowed, bool f8Down) {
    if (!gui_ || owner_ != GetCurrentThreadId()) return false;
    if (swap != swap_.Get() || FAILED(device_->GetDeviceRemovedReason())) {
        toggle_.Hide();
        return false; // adapter must explicitly retire/reinitialize on its owner
    }
    allowed = allowed && IsWindowVisible(window_) && !IsIconic(window_) &&
              GetForegroundWindow() == window_;
    toggle_.Update(f8Down, allowed);
    const auto now = std::chrono::steady_clock::now();
    const float elapsed = std::chrono::duration<float>(now - last_).count();
    last_ = now;
    if (!toggle_.Visible()) return true;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
    if (FAILED(swap->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()))) ||
        FAILED(device_->CreateRenderTargetView(buffer.Get(), nullptr, target.GetAddressOf()))) {
        toggle_.Hide();
        return false;
    }
    D3D11_TEXTURE2D_DESC desc{};
    buffer->GetDesc(&desc);
    if (!desc.Width || !desc.Height) { toggle_.Hide(); return false; }
    ContextScope selected(gui_);
    auto& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(desc.Width), static_cast<float>(desc.Height));
    io.DisplayFramebufferScale = ImVec2(1, 1);
    io.DeltaTime = std::max(.001f, std::min(elapsed, .1f));
    // Intentionally NO Win32 input backend, message dispatch or mouse events.
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
    DrawOverlayReadOnlyPrototype(SyntheticOverlayView());
    ImGui::Render();
    TargetsScope restoreTargets(context_.Get());
    ID3D11RenderTargetView* output = target.Get();
    context_->OMSetRenderTargets(1, &output, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return true;
}

void D3D11Panel::Shutdown() {
    assert(!gui_ || owner_ == GetCurrentThreadId());
    toggle_.Hide();
    if (gui_) {
        ImGuiContext* previous = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(gui_);
        if (backend_) ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext(gui_);
        ImGui::SetCurrentContext(previous == gui_ ? nullptr : previous);
    }
    gui_ = nullptr;
    backend_ = false;
    window_ = nullptr;
    owner_ = 0;
    context_.Reset();
    device_.Reset();
    swap_.Reset();
}
} // namespace almanac
