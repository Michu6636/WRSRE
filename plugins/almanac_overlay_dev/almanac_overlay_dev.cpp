// almanac_overlay_dev - the Almanac panel, drawn with ImGui over the game.
//
// WHAT IT IS
//   One D3D11/ImGui window, drawn into the game's own back buffer through the
//   game's own Present call, so it appears inside the game window rather than as
//   a separate desktop window. F8 shows and hides it. The values in it are
//   placeholders: nothing here reads or writes the republic.
//
// WHY THE EARTHWORKS PANEL NEVER APPEARED
//   Not a rendering bug and not an ImGui bug. There were two independent
//   blockers, and either one alone was enough to keep the panel off screen:
//
//   1. The DLL was never in the loader's plugin folder. The ImGui panel lived in
//      an isolated CMake target outside `plugins\`, so ordinary `build.bat`
//      never produced it, and the log from the test run lists `almanac_button.dll`
//      only. No DLL means no Init, no hook and no drawing.
//   2. Even when built and staged, its installer refused to run on this install.
//      Init declined unless SOVIET64.exe and C3DDLL64.dll hashed to two exact
//      files, and the creation hook then required the engine call site at RVA
//      0x148BC to still read FF 15 8E 6E 15 00. Those are properties of game
//      build 1.1.1.13, not of the install the test ran on, so the chain was never
//      adopted and Present was never hooked. Nothing logged a panel because
//      nothing ever tried to draw one.
//
//   This plugin keeps the working half of that design - ImGui over the game's
//   own swap chain - and drops both blockers: it is an ordinary plugin that
//   `build.bat` builds by name, and it identifies the game window and swap chain
//   at runtime instead of matching file hashes or code bytes.
//
// WHAT IT DOES NOT DO
//   No game memory reads or writes, no sidebar changes, no services consumed, no
//   imgui.ini written into the game directory, no input capture, and no hook
//   removal: once Present is replaced, this DLL stays loaded for the session.
#include "../../src/tesmio_api.h"

#include "almanac_panel.h"
#include "almanac_version.h"
#include "graphics_hook.h"

#include <cstdarg>
#include <cstddef>
#include <cstdio>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace {

const TsmHost* g_host = nullptr;
bool g_started        = false;

// Read once in Init. Defaults mirror the shipped INI.
int  g_enabled    = 1;
int  g_autoShow   = 0;
int  g_toggleKey  = VK_F9;
int  g_fontScale  = 100;      // percent
int  g_unfocused  = 0;        // draw while the game is not the foreground window

void Log(const char* fmt, ...)
{
    if (!g_host || !g_host->log) return;
    char body[512];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(body, sizeof(body), fmt, ap);
    va_end(ap);
    body[sizeof(body) - 1] = '\0';
    g_host->log("almanac overlay  %s", body);
}

constexpr const char* kIni = "plugins\\almanac_overlay_dev.ini";

void ReadSettings(const TsmHost* host)
{
    g_enabled   = host->configInt(kIni, "almanac_overlay_dev", "enabled", 1);
    g_autoShow  = host->configInt(kIni, "almanac_overlay_dev", "show_on_start", 0);
    g_toggleKey = host->configInt(kIni, "almanac_overlay_dev", "toggle_key", VK_F9);
    g_fontScale = host->configInt(kIni, "almanac_overlay_dev", "font_scale_percent", 100);
    g_unfocused = host->configInt(kIni, "almanac_overlay_dev", "draw_when_unfocused", 0);

    if (g_toggleKey < 0x08 || g_toggleKey > 0xFE) g_toggleKey = VK_F9;
    if (g_fontScale < 75)  g_fontScale = 75;
    if (g_fontScale > 250) g_fontScale = 250;
}

} // namespace

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    if (!host || !info || host->apiVersion != TSM_API_VERSION) return 1;
    // findIatSlot and patchIat are the last v3 fields this file reads; anything
    // smaller is an older host table than this plugin was compiled against.
    if (host->structSize < offsetof(TsmHost, configString) +
                            sizeof(host->configString)) {
        return 1;
    }
    // Only what this plugin actually uses. installInlineHook is deliberately
    // not required: the frame source is Present, reached through an import or
    // the DXGI vtable, and demanding a code-patching capability it never calls
    // would refuse to load on a host that forbids it.
    if (!host->log || !host->configInt || !host->findIatSlot ||
        !host->patchIat || !host->exeModule || !host->engineModule) {
        return 1;
    }

    g_host = host;
    info->name    = "almanac_overlay_dev";
    info->version = ALMANAC_PANEL_VERSION;

    ReadSettings(host);
    if (!g_enabled) {
        Log("disabled in %s", kIni);
        return 1;                       // nothing patched yet: safe to unload
    }

    Log("init: key 0x%X, show_on_start %d, font %d%%, host API %u",
        g_toggleKey, g_autoShow, g_fontScale, host->apiVersion);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_host || g_started) return 1;
    g_started = true;

    if (!almanac::StartGraphicsHook(g_host, static_cast<unsigned>(g_toggleKey),
                                    g_autoShow != 0)) {
        return 1;
    }
    Log("startup handed to the installer thread; the panel appears with the "
        "game's first rendered frame");
    return 0;
}
