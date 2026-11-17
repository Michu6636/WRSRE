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
// Installs only the creation observer; no existing HWND is required. Selection
// occurs after successful creation, using matching-build caller/window checks.
// Binary hashes are a caller precondition, not enforced by this function.
// Drawing permission is independent of chain observation and defaults false.
bool InstallGraphicsAdapter(const TsmHost* host, bool developmentOptIn);
// Host must deny while native modal/input ownership is unsafe. Defaults false.
// Thread-safe permission only; never touches ImGui from the caller's thread.
void SetGraphicsAdapterPermission(bool allowed);
}
