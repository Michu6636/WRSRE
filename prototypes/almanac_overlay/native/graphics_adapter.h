// Explicit development experiment; not exported or installed automatically.
#pragma once
#include "d3d11_panel.h"
#include "../../../src/tesmio_api.h"
namespace almanac {
// Install only after binary identity/startup checks and explicit development
// permission. Engine is the verified matching C3DDLL64 module. Startup ordering
// must still be observed: late installation intentionally stays unavailable.
// Once installation is attempted successfully, DLL residency is process-long.
// No uninstall, deployment, input suppression or live Finance connection.
bool InstallGraphicsAdapter(const TsmHost* host, HWND verifiedGameWindow, bool developmentOptIn);
// Host must deny while native modal/input ownership is unsafe. Defaults false.
// Thread-safe permission only; never touches ImGui from the caller's thread.
void SetGraphicsAdapterPermission(bool allowed);
}
