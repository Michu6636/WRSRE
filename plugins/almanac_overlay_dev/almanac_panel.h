// almanac_panel.h - the ImGui/D3D11 panel and its Present callback.
//
// One per process. Owned by graphics_hook.cpp, driven from the game's Present
// call, never from a thread of its own: D3D11 immediate-context use, ImGui
// context use and the back buffer all belong to the thread that presents.
#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include "overlay_toggle.h"
#include "almanac_version.h"

struct ImGuiContext;

namespace almanac {

struct PanelSettings {
    bool  autoShow     = false;  // wait for launcher click or F9 by default
    int   toggleKey    = VK_F9;  // launcher hotkey
    float fontScale    = 1.0f;   // 0.75 .. 2.5
    bool  drawWhenUnfocused = false;
};

class Panel {
public:
    Panel() = default;
    ~Panel();
    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;

    // Binds ImGui to the device behind `swap` and builds the font atlas, once.
    // Must run on the presenting thread. Returns false and stays inert on any
    // failure; a second call after success is a no-op returning true.
    bool Initialize(IDXGISwapChain* swap, const PanelSettings& settings);
    bool Ready() const { return context_ != nullptr; }

    // Draws one frame into the swap chain's current back buffer. `keyDown` is
    // the physical key level. Returns false if the panel had to give up (device
    // removed, back buffer unavailable); the caller then stops calling it.
    bool Render(IDXGISwapChain* swap, bool keyDown, bool worldLoaded = true);

    // Drops the GPU objects and the ImGui context. Only from the owner thread,
    // and only while no Present callback can be running.
    void Shutdown();

    bool Visible() const { return toggle_.Visible(); }


    // Synthetic readout shown in the panel footer.
    int DrawnFrames() const { return drawnFrames_; }

private:
    // Paper-and-ink colours, applied once on our own context.
    void ApplyStyle() const;
    bool CreateFontsAndContext(ID3D11Device* device, ID3D11DeviceContext* context);
    bool AcquireBackBuffer(IDXGISwapChain* swap);
    void ReleaseBackBuffer();

    ImGuiContext*           context_   = nullptr;
    ID3D11Device*           device_    = nullptr;
    ID3D11DeviceContext*    deviceCtx_ = nullptr;
    ID3D11RenderTargetView* target_    = nullptr;
    IDXGISwapChain*         bound_     = nullptr;
    HWND                    window_    = nullptr;
    DWORD                   owner_     = 0;
    bool                    backend_   = false;
    int                     drawnFrames_ = 0;
    unsigned                bufferWidth_  = 0;
    unsigned                bufferHeight_ = 0;
    double                  lastTime_   = 0.0;
    PanelSettings           settings_{};
    OverlayToggle           toggle_{};
};

} // namespace almanac
