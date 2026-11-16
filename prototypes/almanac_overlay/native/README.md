# Isolated D3D11 panel candidate — NOT a working plugin yet

Cross-subsystem entry point: [Almanac engineering status](../../../docs/almanac-engineering-status.md). PR #82 merged this preparation into Almanac; its final adapter source review passed, but this is not Windows/runtime or hook-install acceptance. The follow-on [Windows build and standalone diagnostic](WINDOWS_CHECK.md) now provides an isolated CMake target and F8 test-window source; it is not a game DLL and has not been Windows-built/run. This new work has not yet been independently reviewed.

`d3d11_panel.{h,cpp}` prepares the actual ImGui DX11 draw path, following Earthworks' real-swap-chain architecture without copying its monolithic interception/input code. It is outside normal plugin discovery, has no exports/DllMain and is not built by the host runner. It installs no IAT/Present/WndProc hook. No Windows compilation, GPU test, deployment or game execution has been performed. Independent review is outstanding. Do not drop this source into `plugins/` or enable it in a live game.

## Implemented source

- Explicit caller-supplied swap-chain initialization; no first-Present guessing, native globals or dummy swap chain.
- Owned ImGui context and COM references, initialization rollback on ordinary failure/C++ unwinding, context restoration, explicit same-owner shutdown.
- Earthworks-derived paper/ink styling and the synthetic read-only Almanac view.
- DX11 backend frames with dimensions from the back buffer and clamped elapsed time. No Win32 input backend, input capture, cursor changes, widgets or live Finance providers.
- Existing tested visibility policy accepts a physical F8 down sample and permission each tick. Hiding retains resources; focus loss/minimization hides/disarms.
- Per-draw back-buffer/RTV references are released before return. All eight OM render targets and depth view are saved/restored in addition to upstream backend state handling.

## Explicit adapter candidate (still no plugin entry point)

`graphics_adapter.{h,cpp}` implements callable creation-import/Present interception and physical F8 sampling. It is NOT automatically invoked or included in a normal build. `InstallGraphicsAdapter(host, developmentOptIn)` requires explicit development opt-in and a matching engine/host, but no pre-existing HWND. After a successful original creation call it checks the matching engine return RVA `0x148C2`, a top-level current-process window with the executable's HINSTANCE and exact class `Workers & Resources: Soviet Republic`, and agreement with the returned swap chain's OutputWindow. It does not gate on the mutable title or assume visibility during creation. Binary hash verification remains a caller prerequisite. Unknown callers/windows are forwarded without selecting a chain.

Observation is independent of drawing permission, which defaults false. The caller must update permission for development/modal/readiness safety before enabling drawing; it need not grant permission to capture startup creation. ImGui initializes only on the selected chain's Present callback, not during plugin Start or creation. Late installation still has no fallback. The changed installer contract and creation checks require new source review and Windows compilation; earlier results apply only to their recorded revisions.

The installer rejects a creation IAT target that is already replaced, pins its containing DLL before publishing any detour, and uses expected-target compare/exchange rather than overwriting a competing slot change. It saves/forwards the prior Present function without Present1 substitution, suppresses nested overlay drawing, serializes drawing, rejects render-owner migration, and disables rendering after failures. Protection-restoration failure leaves any installed hook resident and disables drawing. It retains process-lifetime chain/panel references deliberately: this experiment has no unload or automatic replacement path. A shared Present vtable can affect other chains; those forward without drawing. This is source-level preparation, NOT verified coexistence or race safety.

The physical F8 high-bit sample now feeds the tested toggle in the Present callback. It does not consume native input. Existing budget HUD F8 remains unchanged and will also toggle; a single shared activation owner remains a later integration decision. There is still no DLL entry point/build/deployment and no user-visible F8 panel yet. Windows compilation, hook review, startup/chain evidence and disposable-copy testing remain required.

## Adapter integration contract (do not confuse source preparation with integration)

A reviewed native adapter must supply the verified game chain, serialize calls with rendering/resize/device teardown, and always forward the original Present. Call Initialize on its verified render owner, then Draw on that same owner every eligible tick even while hidden. Sample F8 with the high/down bit (not the low transition bit); feed the same sampled state from a single owner rather than installing competing toggles. Development opt-in, focus/modal/native input safety and backend readiness must gate permission. The existing `ui_overlay` HUD is a separate F8 consumer; resolve coexistence explicitly without silently replacing budget rendering. The new adapter supplies that sample when explicitly installed; no plugin invokes it yet.

Do not call Draw for DXGI_PRESENT_TEST, foreign chains or an unverified owner. If callbacks pause, reset visibility/owner state before resuming. Draw failure means bypass overlay, not return fake Present success. A C++ exception/native fault is NOT proven recoverable by this code: disable further overlay entry according to a reviewed policy; do not blindly retry a possibly unfinished ImGui frame. Shutdown must be serialized on the render owner; do not rely on a global destructor during DLL unload.

Device removal/foreign chains are rejected; automatic replacement/recovery is not implemented. Shutdown/reinitialize must be explicit and must not race the hook. DPI policy is currently back-buffer pixels with default font/style: no claim of game-DPI correctness. OM/UAV, backend pipeline coverage, resize behavior, context affinity and Steam/other-overlay coexistence still need native review/acceptance. Backend state saving alone is not a complete GPU safety proof.

## Dependencies and checks remaining

Use upstream Dear ImGui v1.91.9b, commit `f5befd2d29e66809cd1110a152e375a7f1981f06`: core files and `backends/imgui_impl_dx11.{h,cpp}`. The prototype pin is not Earthworks' unavailable RepublicMP-pinned ImGui. Include ImGui root plus WRSRE `src` headers, the Finance drawing source and `earthworks/panel_style.cpp`. MSVC/Windows SDK/WRL and D3D11/DXGI/D3DCompiler libraries are required. This is a dependency description, not a tested Windows build recipe. Preserve upstream ImGui MIT text and the Earthworks GPL notices; combined release obligations remain unresolved.

Before any execution: independent source review, explicit integration scope, Windows compile/link, disposable-copy authorization and GPU/input/lifecycle acceptance. See `docs/almanac-overlay-s0-reconciliation.md` for the corrected S0 argument map and unresolved startup ordering.
