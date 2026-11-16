# Overlay backend: pre-implementation review packet

## Frozen scope

The user chose in-game overlays for Almanac/macro panels and native controls inside supported native windows. The first visible candidate must be READ-ONLY and SYNTHETIC, with a conspicuous synthetic-data banner. No live Finance provider calls, native sidebar modification, terrain/facility hook, save operation or game-owned object write is in scope. The old experimental almanac_button is not a test launcher and must not be enabled or built as part of this work.

Existing drawing source: `prototypes/almanac_overlay/almanac_overlay_view.{h,cpp}`. It expects an owned ImGui frame/context and returns a close action. It does not install hooks, route OS input or establish game readiness. Backend design: `docs/almanac-overlay-design.md`. Host policies and their tests are included in the accompanying archive, but the first visible candidate does not need to wire real Finance execution.

## Decisions required before native implementation

1. **Graphics observation:** verify the matching engine's D3D11 creation import and plugin-start timing. Earthworks' patch of D3D11CreateDeviceAndSwapChain followed by swap-chain Present slot 8 is a reference, not an approved chain. State behavior if creation already happened; no early dummy swap chain. Default to unavailable rather than inventing a second interception route.
2. **Swap-chain identity:** identify the game's output window/process/device/chain and ownership. Reject auxiliary chains. Specify how minimized/hidden windows, a replacement chain, device removal and window recreation are handled. Unsupported recovery must suspend interaction and rendering safely, not retain invalid resources.
3. **Hook chain and residence:** retain original pointers; no duplicate self-hook installation. Establish coexistence with other overlays and existing import-slot owners. Specify partial installation failure; once callbacks point into the DLL, failure must not invite unloading it. Do not promise hot unload. Thread/callback quiescence cannot be inferred from a flag alone.
4. **Render state:** audit the exact pinned Win32/DX11 backends and all extra custom state changes. Restore previous ImGui context and pipeline/target state on every exit, not only successful frames. Back-buffer references must not break ResizeBuffers. Distinguish supported HRESULT failures from C++/SEH handling.
5. **Input:** choose one owner for panel lifecycle and ImGui state. WndProc and Present may run on different threads; define bounded event marshalling, overflow behavior and gesture-release ownership. No concurrent ImGui access or controller mutation. Identify all game input paths needed to prevent click-through/typing shortcuts. WndProc swallowing alone is not proof. Do not copy Earthworks' DirectInput/native-latch patches without matching-target evidence.
6. **Development trigger:** disabled by default, explicitly configured, no live policy writes. A shortcut is a candidate ONLY after text/native-modal/readiness gates and key ownership are verified. Otherwise report it blocked; do not silently bypass those gates or auto-open during menus/load. Production launcher remains independently unresolved.
7. **Layout/scaling:** one policy for game scale, Windows DPI, client/framebuffer conversion and font/style sizing. Hit testing and drawing must share transforms. Prototype ratios do not prove DPI correctness or bounds after resolution changes.
8. **Dependency/legal:** upstream ImGui v1.91.9b commit f5befd2d29e66809cd1110a152e375a7f1981f06 was used for headless prototype checks; its MIT notice must be preserved. Earthworks-adapted overlay/underlay code has GPL/RepublicMP provenance. Do not copy that implementation until attribution/distribution obligations are handled. The Earthworks SDK hash discrepancy remains documented; API 4 numbering alone is not a complete compatibility check.

## Required reviewer output

Return a bounded go/no-go on the first synthetic visible backend, not another sidebar-geometry report:

- Chosen implementation route and exact source/binary evidence references.
- Per-item status above: established, design-only or blocked; identify smallest missing evidence.
- Hook target identity/chain/ABI and whole-instruction relocation audit if any inline hook is proposed. Prefer no new executable mid-function interception.
- Input ownership proof or an explicit block on interactive testing. Noninteractive drawing does not authorize a clickable panel with leaky controls.
- Specific file ownership and patch scope for the worker; no broad copying of Earthworks runtime.
- Disposable-copy test checklist with expected failure behavior; do not perform gameplay tests.

Read-only review: no source edits, builds, dependency installers, deployment, game execution or Ghidra project changes. Serialize queries through the designated operator. Do not approve unavailable evidence from a manifest alone. The review result is still required; this packet is not an approval.

## Current checks and limits

The existing headless drawing test was rerun successfully while preparing this packet. Previous host unit/sanitizer checks are documented in their contracts; they were not all rerun in this packaging pass. No child-review tool is exposed in Arena, so no independent review has occurred here. The Earthworks scratch archive used earlier is no longer present in `.cache`; its committed source remains accessible via Git, not that ephemeral path. Retrieve the referenced source from the recorded commit rather than assuming an ignored cache persists.
