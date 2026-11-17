// Standalone DX11 renderer check, NOT a game plugin or injection tool.
// Links the real panel component but never calls InstallGraphicsAdapter.
#include "d3d11_panel.h"

namespace {
struct WindowState { bool closing = false; };
LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    if (message == WM_NCCREATE) {
        const auto* create = reinterpret_cast<CREATESTRUCTW*>(lparam);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
    }
    auto* state = reinterpret_cast<WindowState*>(GetWindowLongPtrW(window, GWLP_USERDATA));
    if (message == WM_CLOSE && state) { state->closing = true; return 0; }
    if (message == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProcW(window, message, wparam, lparam);
}

bool RunPanel(HWND window, WindowState& state) {
    using Microsoft::WRL::ComPtr;
    ComPtr<IDXGISwapChain> swap;
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2;
    desc.OutputWindow = window;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    // Software DX11 keeps this check independent of a particular GPU driver.
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
        0, nullptr, 0, D3D11_SDK_VERSION, &desc, swap.GetAddressOf(),
        device.GetAddressOf(), nullptr, context.GetAddressOf()))) return false;
    almanac::D3D11Panel panel;
    if (!panel.Initialize(swap.Get())) return false;
    UINT width = 0, height = 0;
    while (!state.closing) {
        MSG message{};
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) { state.closing = true; break; }
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
        if (state.closing) break;
        RECT client{};
        if (!GetClientRect(window, &client)) return false;
        const UINT nextWidth = static_cast<UINT>(client.right - client.left);
        const UINT nextHeight = static_cast<UINT>(client.bottom - client.top);
        if (IsIconic(window) || !nextWidth || !nextHeight) {
            if (!panel.Draw(swap.Get(), false, false)) return false;
            WaitMessage();
            continue;
        }
        if (width != nextWidth || height != nextHeight) {
            // The previous loop retained no back buffer/RTV references.
            context->OMSetRenderTargets(0, nullptr, nullptr);
            if (FAILED(swap->ResizeBuffers(0, nextWidth, nextHeight, DXGI_FORMAT_UNKNOWN, 0))) return false;
            width = nextWidth;
            height = nextHeight;
        }
        {
            ComPtr<ID3D11Texture2D> buffer;
            ComPtr<ID3D11RenderTargetView> target;
            if (FAILED(swap->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()))) ||
                FAILED(device->CreateRenderTargetView(buffer.Get(), nullptr, target.GetAddressOf()))) return false;
            const float clear[] = {.08f, .10f, .13f, 1.f};
            context->ClearRenderTargetView(target.Get(), clear);
        }
        const bool down = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
        if (!panel.Draw(swap.Get(), true, down)) return false;
        if (FAILED(swap->Present(1, 0))) return false;
        Sleep(1);
    }
    panel.Shutdown(); // same owner, before window/device destruction
    return true;
}
} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    WNDCLASSW cls{};
    cls.lpfnWndProc = WindowProc;
    cls.hInstance = instance;
    cls.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    cls.lpszClassName = L"AlmanacStandalonePanelCheck";
    if (!RegisterClassW(&cls)) return 1;
    WindowState state;
    HWND window = CreateWindowExW(0, cls.lpszClassName,
        L"Almanac renderer check (not the game) - F8 toggles synthetic panel",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, instance, &state);
    if (!window) { UnregisterClassW(cls.lpszClassName, instance); return 1; }
    ShowWindow(window, show);
    bool success = false;
    try { success = RunPanel(window, state); }
    catch (...) { success = false; }
    if (!success) MessageBoxW(window, L"DX11 panel check failed. No game was accessed.",
        L"Almanac renderer check", MB_OK | MB_ICONERROR);
    DestroyWindow(window);
    UnregisterClassW(cls.lpszClassName, instance);
    return success ? 0 : 1;
}
