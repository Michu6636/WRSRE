// almanac_panel.cpp - ImGui + D3D11 drawing for the Almanac panel.
//
// Four things have to be true before a frame is drawn, and each one is checked
// per frame rather than assumed once:
//
//   1. we own the context        the presenting thread is the one that set it up
//   2. the swap chain is ours    Present was called through our hook for it
//   3. there is a back buffer    GetBuffer(0) succeeded for this frame
//   4. ImGui is between frames   NewFrame only after the previous Render
//
// Anything unexpected stops drawing for the session instead of retrying into a
// device the game may have torn down. There is no device-loss recovery: the
// game's own ResizeBuffers path is not intercepted, so a resized or replaced
// chain simply gets a fresh render target view on the next frame and a removed
// device ends drawing.
//
// Nothing here reads game memory, calls a game function or writes game state.
#include "almanac_panel.h"

#include <windowsx.h>

#include "almanac_panel_view.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"

#include <atomic>
#include <cfloat>
#include <cstdio>
#include <mutex>

namespace almanac {
namespace {

// Exactly one ImGui context may be current at a time, and the game is not the
// only plugin that may use ImGui. Every entry saves what was current and puts it
// back, so this panel can never make another overlay draw into the wrong
// context or leave ImGui in a half-finished frame.
class ContextGuard {
public:
    explicit ContextGuard(ImGuiContext* mine)
        : previous_(ImGui::GetCurrentContext())
    {
        ImGui::SetCurrentContext(mine);
    }
    ~ContextGuard() { ImGui::SetCurrentContext(previous_); }
    ContextGuard(const ContextGuard&) = delete;
    ContextGuard& operator=(const ContextGuard&) = delete;

private:
    ImGuiContext* previous_;
};

// The caller's output-merger state is not ours to keep. ImGui_ImplDX11 saves and
// restores its own pipeline state; the render-target bindings are additional and
// are restored here. No reference to a target survives the frame.
class TargetsGuard {
public:
    explicit TargetsGuard(ID3D11DeviceContext* context) : context_(context)
    {
        context_->OMGetRenderTargets(
            D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets_, &depth_);
    }
    ~TargetsGuard()
    {
        context_->OMSetRenderTargets(
            D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets_, depth_);
        for (auto*& target : targets_) {
            if (target) { target->Release(); target = nullptr; }
        }
        if (depth_) { depth_->Release(); depth_ = nullptr; }
    }
    TargetsGuard(const TargetsGuard&) = delete;
    TargetsGuard& operator=(const TargetsGuard&) = delete;

private:
    ID3D11DeviceContext*   context_;
    ID3D11RenderTargetView* targets_[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
    ID3D11DepthStencilView* depth_ = nullptr;
};

double NowSeconds()
{
    static LARGE_INTEGER frequency{};
    if (!frequency.QuadPart) QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER counter{};
    QueryPerformanceCounter(&counter);
    return static_cast<double>(counter.QuadPart) /
           static_cast<double>(frequency.QuadPart);
}

// Scoped WndProc state for the floating launcher button and interactive panel.
// Shared between the window message thread and the D3D11 present thread.
std::mutex        g_launcherMutex;
LauncherRect      g_launcherRect{};
LauncherRect      g_panelRect{};
bool              g_panelVisible  = false;
unsigned          g_surfaceWidth  = 0;
unsigned          g_surfaceHeight = 0;
std::atomic<bool> g_launcherHovered{false};
std::atomic<bool> g_pendingButtonClick{false};
std::atomic<bool> g_panelDragging{false};
std::atomic<bool> g_mouseDownLeft{false};
std::atomic<bool> g_mouseDownRight{false};
std::mutex        g_inputMutex;
float             g_pendingWheelY = 0.0f;
WNDPROC           g_originalWndProc = nullptr;
HWND              g_subclassedWindow = nullptr;
int               g_activeTab = 0;
PanelLayout       g_savedLayout{};

constexpr const char* kLayoutIni = "plugins\\almanac_overlay_dev.ini";

void LoadSavedLayout()
{
    const int x = GetPrivateProfileIntA("layout", "panel_x", -1, kLayoutIni);
    const int y = GetPrivateProfileIntA("layout", "panel_y", -1, kLayoutIni);
    const int w = GetPrivateProfileIntA("layout", "panel_w", -1, kLayoutIni);
    const int h = GetPrivateProfileIntA("layout", "panel_h", -1, kLayoutIni);
    const int t = GetPrivateProfileIntA("layout", "active_tab", 0, kLayoutIni);

    if (x >= 0 && y >= 0) {
        g_savedLayout.hasSavedPos = true;
        g_savedLayout.x = static_cast<float>(x);
        g_savedLayout.y = static_cast<float>(y);
    }
    if (w >= 420 && h >= 240) {
        g_savedLayout.hasSavedSize = true;
        g_savedLayout.w = static_cast<float>(w);
        g_savedLayout.h = static_cast<float>(h);
    }
    if (t >= 0 && t <= 2) {
        g_activeTab = t;
    }
}

void MaybeSaveLayout(const LauncherRect& rect, int tab, bool dragging)
{
    if (!rect.valid || dragging) return;
    static int       s_lastX = -1, s_lastY = -1, s_lastW = -1, s_lastH = -1, s_lastTab = -1;
    static ULONGLONG s_lastWriteMs = 0;

    const int x = static_cast<int>(rect.left + 0.5f);
    const int y = static_cast<int>(rect.top + 0.5f);
    const int w = static_cast<int>((rect.right - rect.left) + 0.5f);
    const int h = static_cast<int>((rect.bottom - rect.top) + 0.5f);
    if (x == s_lastX && y == s_lastY && w == s_lastW && h == s_lastH && tab == s_lastTab) {
        return;
    }
    const ULONGLONG now = GetTickCount64();
    if (s_lastWriteMs != 0 && (now - s_lastWriteMs) < 1000ULL) return;
    s_lastWriteMs = now;
    s_lastX = x; s_lastY = y; s_lastW = w; s_lastH = h; s_lastTab = tab;

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%d", x);
    WritePrivateProfileStringA("layout", "panel_x", buf, kLayoutIni);
    std::snprintf(buf, sizeof(buf), "%d", y);
    WritePrivateProfileStringA("layout", "panel_y", buf, kLayoutIni);
    std::snprintf(buf, sizeof(buf), "%d", w);
    WritePrivateProfileStringA("layout", "panel_w", buf, kLayoutIni);
    std::snprintf(buf, sizeof(buf), "%d", h);
    WritePrivateProfileStringA("layout", "panel_h", buf, kLayoutIni);
    std::snprintf(buf, sizeof(buf), "%d", tab);
    WritePrivateProfileStringA("layout", "active_tab", buf, kLayoutIni);
}

bool PointInRect(const LauncherRect& rect, float x, float y)
{
    return rect.valid && x >= rect.left && x <= rect.right &&
           y >= rect.top && y <= rect.bottom;
}

bool GetSurfaceCursorPos(HWND hwnd, unsigned surfW, unsigned surfH,
                         float& outX, float& outY)
{
    if (!hwnd || !surfW || !surfH) return false;
    POINT pt{};
    if (!GetCursorPos(&pt)) return false;
    if (!ScreenToClient(hwnd, &pt)) return false;
    RECT client{};
    if (!GetClientRect(hwnd, &client)) return false;
    const float clientW = static_cast<float>(client.right - client.left);
    const float clientH = static_cast<float>(client.bottom - client.top);
    if (!(clientW > 0.0f) || !(clientH > 0.0f)) return false;
    outX = static_cast<float>(pt.x) * (static_cast<float>(surfW) / clientW);
    outY = static_cast<float>(pt.y) * (static_cast<float>(surfH) / clientH);
    return true;
}

bool ClientLParamToSurface(HWND hwnd, LPARAM lParam, unsigned surfW, unsigned surfH,
                           float& outX, float& outY)
{
    if (!surfW || !surfH) return false;
    RECT client{};
    if (!GetClientRect(hwnd, &client)) return false;
    const float clientW = static_cast<float>(client.right - client.left);
    const float clientH = static_cast<float>(client.bottom - client.top);
    if (!(clientW > 0.0f) || !(clientH > 0.0f)) return false;
    outX = static_cast<float>(GET_X_LPARAM(lParam)) * (static_cast<float>(surfW) / clientW);
    outY = static_cast<float>(GET_Y_LPARAM(lParam)) * (static_cast<float>(surfH) / clientH);
    return true;
}

bool PointInLauncherButton(HWND hwnd, LPARAM lParam)
{
    LauncherRect rect{};
    unsigned surfW = 0, surfH = 0;
    {
        std::lock_guard<std::mutex> lock(g_launcherMutex);
        rect  = g_launcherRect;
        surfW = g_surfaceWidth;
        surfH = g_surfaceHeight;
    }
    if (!rect.valid) return false;
    float mx = 0.0f, my = 0.0f;
    if (!ClientLParamToSurface(hwnd, lParam, surfW, surfH, mx, my)) return false;
    return PointInRect(rect, mx, my);
}

bool PointInOpenPanel(HWND hwnd, LPARAM lParam)
{
    LauncherRect rect{};
    bool visible = false;
    unsigned surfW = 0, surfH = 0;
    {
        std::lock_guard<std::mutex> lock(g_launcherMutex);
        rect    = g_panelRect;
        visible = g_panelVisible;
        surfW   = g_surfaceWidth;
        surfH   = g_surfaceHeight;
    }
    if (!visible || !rect.valid) return false;
    float mx = 0.0f, my = 0.0f;
    if (!ClientLParamToSurface(hwnd, lParam, surfW, surfH, mx, my)) return false;
    return PointInRect(rect, mx, my);
}

bool CursorInOpenPanel(HWND hwnd)
{
    LauncherRect rect{};
    bool visible = false;
    unsigned surfW = 0, surfH = 0;
    {
        std::lock_guard<std::mutex> lock(g_launcherMutex);
        rect    = g_panelRect;
        visible = g_panelVisible;
        surfW   = g_surfaceWidth;
        surfH   = g_surfaceHeight;
    }
    if (!visible || !rect.valid) return false;
    float mx = 0.0f, my = 0.0f;
    if (!GetSurfaceCursorPos(hwnd, surfW, surfH, mx, my)) return false;
    return PointInRect(rect, mx, my);
}

LRESULT CALLBACK AlmanacWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_MOUSEMOVE:
        g_launcherHovered.store(PointInLauncherButton(hwnd, lParam),
                                std::memory_order_relaxed);
        if (g_panelDragging.load(std::memory_order_relaxed)) {
            return 0; // keep camera/map from panning while dragging or resizing panel
        }
        break;
    case WM_LBUTTONDOWN:
        if (PointInLauncherButton(hwnd, lParam)) {
            g_pendingButtonClick.store(true, std::memory_order_release);
            return 0; // swallow click so the game world does not react behind the button
        }
        if (PointInOpenPanel(hwnd, lParam)) {
            g_panelDragging.store(true, std::memory_order_relaxed);
            g_mouseDownLeft.store(true, std::memory_order_relaxed);
            return 0; // swallow click and route to ImGui
        }
        break;
    case WM_LBUTTONUP:
        if (g_panelDragging.exchange(false, std::memory_order_relaxed)) {
            g_mouseDownLeft.store(false, std::memory_order_relaxed);
            return 0;
        }
        if (PointInLauncherButton(hwnd, lParam) || PointInOpenPanel(hwnd, lParam)) {
            g_mouseDownLeft.store(false, std::memory_order_relaxed);
            return 0;
        }
        break;
    case WM_RBUTTONDOWN:
        if (PointInOpenPanel(hwnd, lParam)) {
            g_mouseDownRight.store(true, std::memory_order_relaxed);
            return 0;
        }
        break;
    case WM_RBUTTONUP:
        if (g_mouseDownRight.exchange(false, std::memory_order_relaxed) ||
            PointInOpenPanel(hwnd, lParam)) {
            return 0;
        }
        break;
    case WM_MOUSEWHEEL:
        if (CursorInOpenPanel(hwnd)) {
            const float delta =
                static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) /
                static_cast<float>(WHEEL_DELTA);
            std::lock_guard<std::mutex> lock(g_inputMutex);
            g_pendingWheelY += delta;
            return 0; // swallow scroll so the game camera does not zoom
        }
        break;
    case WM_MOUSELEAVE:
        g_launcherHovered.store(false, std::memory_order_relaxed);
        break;
    case WM_KILLFOCUS:
    case WM_CAPTURECHANGED:
        g_panelDragging.store(false, std::memory_order_relaxed);
        g_mouseDownLeft.store(false, std::memory_order_relaxed);
        g_mouseDownRight.store(false, std::memory_order_relaxed);
        break;
    default:
        break;
    }
    WNDPROC orig = g_originalWndProc;
    return orig ? CallWindowProcW(orig, hwnd, msg, wParam, lParam)
                : DefWindowProcW(hwnd, msg, wParam, lParam);
}

void InstallLauncherWndProc(HWND hwnd)
{
    if (!hwnd || g_subclassedWindow) return;
    WNDPROC prev = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC,
                          reinterpret_cast<LONG_PTR>(&AlmanacWndProc)));
    if (prev && prev != &AlmanacWndProc) {
        g_originalWndProc  = prev;
        g_subclassedWindow = hwnd;
    }
}

void PublishOverlayRects(const LauncherRect& btnRect,
                         const LauncherRect& panelRect,
                         bool panelVisible,
                         unsigned width, unsigned height)
{
    std::lock_guard<std::mutex> lock(g_launcherMutex);
    g_launcherRect  = btnRect;
    g_panelRect     = panelRect;
    g_panelVisible  = panelVisible;
    g_surfaceWidth  = width;
    g_surfaceHeight = height;
}

const char* HotkeyDisplayName(int vk)
{
    switch (vk) {
    case VK_F8:  return "F8";
    case VK_F9:  return "F9";
    case VK_F10: return "F10";
    case VK_F11: return "F11";
    case VK_F12: return "F12";
    default:     return "hotkey";
    }
}

} // namespace

Panel::~Panel()
{
    // Deliberately does not call Shutdown(): the destructor may run during
    // process teardown on a thread that is not the owner. Shutdown() is called
    // explicitly by the hook owner, and a process-lifetime panel is never freed
    // on a live path.
}

bool Panel::Initialize(IDXGISwapChain* swap, const PanelSettings& settings)
{
    if (context_ && backend_) return true;      // already up
    if (!swap || owner_ != 0) return false;     // failed before; do not retry

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swap->GetDesc(&desc))) return false;
    if (!desc.OutputWindow || !IsWindow(desc.OutputWindow)) return false;

    // Only draw over a window this process owns. A launcher or browser window
    // sharing the same chain would otherwise get our panel painted on it.
    DWORD processId = 0;
    GetWindowThreadProcessId(desc.OutputWindow, &processId);
    if (processId != GetCurrentProcessId()) return false;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceCtx = nullptr;
    if (FAILED(swap->GetDevice(__uuidof(ID3D11Device),
                               reinterpret_cast<void**>(&device))) || !device) {
        return false;
    }
    device->GetImmediateContext(&deviceCtx);
    if (!deviceCtx) {
        device->Release();
        return false;
    }

    if (!CreateFontsAndContext(device, deviceCtx)) {
        deviceCtx->Release();
        device->Release();
        return false;
    }

    device_    = device;          // reference kept for the session
    deviceCtx_ = deviceCtx;
    bound_     = swap;
    window_    = desc.OutputWindow;
    owner_     = GetCurrentThreadId();
    settings_  = settings;
    toggle_    = OverlayToggle{};
    toggle_.SetAutoShow(settings.autoShow);
    lastTime_  = NowSeconds();
    LoadSavedLayout();
    InstallLauncherWndProc(window_);
    return true;
}

void Panel::ApplyStyle() const
{
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding    = 10.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 5.0f;
    style.WindowPadding     = ImVec2(14.0f, 12.0f);
    style.FramePadding      = ImVec2(10.0f, 5.0f);
    style.ItemSpacing       = ImVec2(8.0f, 7.0f);
    style.WindowBorderSize  = 1.0f;
    style.WindowTitleAlign  = ImVec2(0.0f, 0.5f);
    // Paper and ink. Colour names are ImGui's roles, not this panel's meanings.
    style.Colors[ImGuiCol_WindowBg]          = ImVec4(0.91f, 0.85f, 0.74f, 0.97f);
    style.Colors[ImGuiCol_Border]            = ImVec4(0.36f, 0.25f, 0.16f, 0.85f);
    style.Colors[ImGuiCol_TitleBg]           = ImVec4(0.40f, 0.10f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]     = ImVec4(0.48f, 0.14f, 0.11f, 1.00f);
    style.Colors[ImGuiCol_Text]              = ImVec4(0.22f, 0.17f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]    = ImVec4(0.80f, 0.62f, 0.34f, 0.60f);
    style.Colors[ImGuiCol_TableRowBgAlt]     = ImVec4(0.30f, 0.22f, 0.14f, 0.06f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.36f, 0.25f, 0.16f, 0.45f);
    style.Colors[ImGuiCol_TableBorderLight]  = ImVec4(0.36f, 0.25f, 0.16f, 0.25f);
}

bool Panel::CreateFontsAndContext(ID3D11Device* device,
                                  ID3D11DeviceContext* deviceCtx)
{
    ImGuiContext* previous = ImGui::GetCurrentContext();
    ImGuiContext* mine     = ImGui::CreateContext();
    if (!mine) {
        ImGui::SetCurrentContext(previous);
        return false;
    }

    // Failures below must not leave a half-initialized context behind for the
    // game or another overlay to inherit.
    bool ok = false;
    ImGui::SetCurrentContext(mine);
    do {
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;     // never write imgui.ini into the game dir
        io.LogFilename = nullptr;
        io.ConfigFlags &= ~(ImGuiConfigFlags_NavEnableKeyboard |
                            ImGuiConfigFlags_NavEnableGamepad);
        // Cursor shape belongs to the game window procedure, not to us.
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        // The default embedded font: no asset files, nothing to install.
        ImFontConfig fontConfig;
        fontConfig.SizePixels = 16.0f * settings_.fontScale;
        fontConfig.OversampleH = 2;
        fontConfig.OversampleV = 1;
        fontConfig.PixelSnapH  = true;
        if (!io.Fonts->AddFontDefault(&fontConfig)) break;

        if (!ImGui_ImplDX11_Init(device, deviceCtx)) break;
        backend_ = true;

        // Build the atlas now. ImGui refuses to start a frame without it, and
        // the DX11 backend only does it lazily on its first NewFrame, which is
        // one frame too late to report failure from here.
        if (!ImGui_ImplDX11_CreateDeviceObjects()) break;
        if (!io.Fonts->IsBuilt()) break;

        // Style lives in the context, so it cannot leak into another overlay's
        // context and does not need re-applying on later frames.
        ApplyStyle();

        ok = true;
    } while (false);

    if (ok) {
        context_ = mine;
        ImGui::SetCurrentContext(previous);
        return true;
    }

    // Roll back in dependency order, then restore whoever was here before us.
    if (backend_) {
        ImGui_ImplDX11_Shutdown();
        backend_ = false;
    }
    ImGui::DestroyContext(mine);
    ImGui::SetCurrentContext(previous);
    return false;
}

bool Panel::AcquireBackBuffer(IDXGISwapChain* swap)
{
    ReleaseBackBuffer();

    // In Direct3D 11, buffer 0 is always the writable back buffer (even on
    // flip-model DX11.1 swap chains; GetCurrentBackBufferIndex is D3D12-only
    // and passing index > 0 to GetBuffer returns DXGI_ERROR_INVALID_CALL).
    ID3D11Texture2D* buffer = nullptr;
    if (FAILED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D),
                               reinterpret_cast<void**>(&buffer))) || !buffer) {
        return false;
    }

    D3D11_TEXTURE2D_DESC desc{};
    buffer->GetDesc(&desc);
    if (!desc.Width || !desc.Height) {
        buffer->Release();
        return false;
    }

    if (FAILED(device_->CreateRenderTargetView(buffer, nullptr, &target_)) ||
        !target_) {
        buffer->Release();
        return false;
    }
    bufferWidth_  = desc.Width;
    bufferHeight_ = desc.Height;
    buffer->Release();
    return true;
}

void Panel::ReleaseBackBuffer()
{
    if (target_) {
        target_->Release();
        target_ = nullptr;
    }
    bufferWidth_  = 0;
    bufferHeight_ = 0;
}

bool Panel::Render(IDXGISwapChain* swap, bool keyDown, bool worldLoaded,
                   const PanelModel* liveModel, PanelAction* outAction)
{
    if (outAction) *outAction = PanelAction{};
    static std::mutex s_renderMutex;
    std::lock_guard<std::mutex> renderLock(s_renderMutex);

    if (!context_ || !backend_ || !device_ || !deviceCtx_) return false;

    // The chain we bound must still be the chain presenting. Anything else is a
    // different window's Present reaching our hook; never draw on it.
    if (swap != bound_) return false;

    if (FAILED(device_->GetDeviceRemovedReason())) return false;

    // Eligibility: the overlay only exists inside a loaded game save over the
    // foreground, unminimised game window.
    bool eligible = worldLoaded && IsWindow(window_) && !IsIconic(window_);
    if (eligible && !settings_.drawWhenUnfocused)
        eligible = GetForegroundWindow() == window_;

    if (!eligible) {
        PublishOverlayRects(LauncherRect{}, LauncherRect{}, false, 0, 0);
        g_launcherHovered.store(false, std::memory_order_relaxed);
        g_pendingButtonClick.store(false, std::memory_order_relaxed);
        g_panelDragging.store(false, std::memory_order_relaxed);
        g_mouseDownLeft.store(false, std::memory_order_relaxed);
        g_mouseDownRight.store(false, std::memory_order_relaxed);
        toggle_.Frame(keyDown, false, false);
        return true;
    }

    const bool buttonClicked =
        g_pendingButtonClick.exchange(false, std::memory_order_acq_rel);
    const bool show = toggle_.Frame(keyDown, true, buttonClicked);

    // A transient back-buffer failure (e.g. during a resize or mode switch)
    // skips this frame rather than permanently stopping drawing; real device
    // removal is already caught above by GetDeviceRemovedReason().
    if (!AcquireBackBuffer(swap)) return true;

    // Another overlay may have made its own context current since the last
    // frame; this puts ours back for the duration of the frame and restores
    // theirs on the way out.
    ContextGuard current(context_);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(bufferWidth_),
                            static_cast<float>(bufferHeight_));
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    float mx = 0.0f, my = 0.0f;
    const bool hasCursor = GetSurfaceCursorPos(window_, bufferWidth_,
                                               bufferHeight_, mx, my);
    if (hasCursor) {
        io.AddMousePosEvent(mx, my);
    } else {
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }

    // Feed WndProc-captured mouse buttons and scroll wheel to ImGui.
    const bool dragging = g_panelDragging.load(std::memory_order_relaxed);
    io.AddMouseButtonEvent(0, show && g_mouseDownLeft.load(std::memory_order_relaxed));
    io.AddMouseButtonEvent(1, show && g_mouseDownRight.load(std::memory_order_relaxed));
    float wheelY = 0.0f;
    {
        std::lock_guard<std::mutex> lock(g_inputMutex);
        wheelY = g_pendingWheelY;
        g_pendingWheelY = 0.0f;
    }
    if (show && wheelY != 0.0f) {
        io.AddMouseWheelEvent(0.0f, wheelY);
    }

    // Workers & Resources renders its own software cursor before Present, so
    // anything drawn in Present paints over it. Draw ImGui's own cursor on top
    // whenever the pointer is over our launcher button or open panel.
    LauncherRect prevBtnRect{};
    LauncherRect prevPanelRect{};
    {
        std::lock_guard<std::mutex> lock(g_launcherMutex);
        prevBtnRect   = g_launcherRect;
        prevPanelRect = g_panelRect;
    }
    const bool overButton = hasCursor && PointInRect(prevBtnRect, mx, my);
    const bool overPanel  = hasCursor && show && PointInRect(prevPanelRect, mx, my);
    io.MouseDrawCursor = overButton || overPanel || (show && dragging);

    const double now = NowSeconds();
    float delta = static_cast<float>(now - lastTime_);
    lastTime_ = now;
    if (!(delta > 0.0f) || delta > 0.25f) delta = 1.0f / 30.0f;  // tab-out, hitch
    io.DeltaTime = delta;

    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();

    const char* keyName = HotkeyDisplayName(settings_.toggleKey);
    const bool hovered  = overButton || g_launcherHovered.load(std::memory_order_relaxed);
    const LauncherRect btnRect = DrawAlmanacLauncherButton(show, hovered, keyName);

    LauncherRect panelRect{};
    if (show) {
        PanelModel model = liveModel ? *liveModel : PanelModel{};
        char buildNotice[64];
        std::snprintf(buildNotice, sizeof(buildNotice), "v%s",
                      ALMANAC_PANEL_VERSION);
        model.buildNotice   = buildNotice;
        model.drawnFrames   = drawnFrames_;
        model.processId     = static_cast<int>(GetCurrentProcessId());
        model.autoShow      = settings_.autoShow;
        model.toggleKeyName = keyName;
        DrawAlmanacPanel(model, &panelRect, &g_activeTab, outAction, &g_savedLayout);
        MaybeSaveLayout(panelRect, g_activeTab, dragging);
        ++drawnFrames_;
    }
    PublishOverlayRects(btnRect, panelRect, show, bufferWidth_, bufferHeight_);

    ImGui::Render();
    {
        TargetsGuard restore(deviceCtx_);
        ID3D11RenderTargetView* target = target_;
        deviceCtx_->OMSetRenderTargets(1, &target, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    // Never hold a back-buffer RTV across Present so the game's ResizeBuffers
    // calls always see zero outstanding references from this plugin.
    ReleaseBackBuffer();

    return true;
}

void Panel::Shutdown()
{
    if (!context_) return;
    if (owner_ && owner_ != GetCurrentThreadId()) return;  // wrong thread

    ReleaseBackBuffer();
    ImGuiContext* previous = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(context_);
    if (backend_) ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext(context_);
    ImGui::SetCurrentContext(previous == context_ ? nullptr : previous);

    context_   = nullptr;
    backend_   = false;
    owner_     = 0;
    bound_     = nullptr;
    window_    = nullptr;
    if (deviceCtx_) { deviceCtx_->Release(); deviceCtx_ = nullptr; }
    if (device_)    { device_->Release();    device_    = nullptr; }
}

} // namespace almanac
