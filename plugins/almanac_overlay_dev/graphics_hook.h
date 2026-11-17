// graphics_hook.h - getting called once per rendered game frame.
//
// The panel needs a Present call it can draw immediately before. Two documented
// Tesmio mechanisms get one, tried in this order:
//
//   1. findIatSlot/patchIat on C3DDLL64's import of
//      D3D11CreateDeviceAndSwapChain. The engine's own creation call is
//      observed, its real swap chain adopted, and that chain's Present slot
//      replaced. No game RVA is involved, so a game update cannot shift it.
//
//   2. The shared IDXGISwapChain vtable, discovered by creating a throwaway
//      chain of our own. Used only when (1) could not be installed, because the
//      import was already hooked by something else. Less desirable - it touches
//      a vtable every chain in the process shares - so it is the fallback and it
//      forwards everything it does not own.
//
// Both are installed from a worker thread, never from TsmPluginStart: Start runs
// inside DllMain, under the loader lock. See graphics_hook.cpp for why that
// matters and what is deliberately not done under it.
#pragma once

#include "../../src/tesmio_api.h"

namespace almanac {

// Spawns the installer thread. Called from TsmPluginStart. Returns false only
// when the thread could not be created. A hook that then fails is logged and
// leaves the plugin loaded and inert, which is safe: nothing here removes a
// hook, so the DLL stays resident for the session either way.
bool StartGraphicsHook(const TsmHost* host, unsigned toggleKey, bool autoShow);

} // namespace almanac
