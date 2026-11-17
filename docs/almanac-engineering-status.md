# Almanac engineering status and handoff

**Start here for current implementation status.** Baseline: `Almanac` merge `e912dcf12c1ec32930f785255f6473389a453849` ([PR #87](https://github.com/Michu6636/WRSRE/pull/87)). Follow-on development adds an auto-visible synthetic ImGui panel to the isolated Almanac development DLL; Windows build, staging and in-game visual acceptance remain outstanding. Older Almanac plans/reviews remain historical evidence; their sidebar approach and implementation-status statements are not the current delivery plan.

## What the user wants / immediate milestone

The user has redirected the immediate task from testing Earthworks itself to getting a **simple in-game Almanac ImGui panel visible**. Reuse the existing Earthworks-inspired D3D11/ImGui renderer and synthetic Almanac view; do not add Earthworks terrain/gameplay hooks, connect live Finance, or change the native sidebar. This test panel is read-only and clearly labels its values synthetic. It auto-shows on the first eligible foreground render; F8 hides/reopens it. It is a visual smoke test, not product/runtime acceptance.

The previous turn targeted Earthworks' own panel, which was the wrong deliverable. The user's `tesmioloader-4.log` confirms the test run loaded `almanac_button.dll` (the separate native ninth-tile prototype); it contains no `almanac_overlay_dev.dll` or Earthworks startup/render messages. The attempted Almanac UI was therefore not loaded, and no ImGui render path was reached. The ImGui DLL is a separate isolated CMake target, not built by `build.bat`; merging source alone does not install it. See the corrected [`test_menu/README.md`](../prototypes/almanac_overlay/test_menu/README.md).

Keep the runtime check on a disposable game copy/save. Use the actual `almanac_overlay_dev.dll` output and its loader/config entries; do not mistake the old `almanac_button` hook for the ImGui panel. No Windows build, staging, or Almanac game test has occurred in this workspace. The user launches and visually checks the game; do not modify or launch a live installation.

Preserve the eight native sidebar buttons' order, artwork and click behavior. Do not re-enable the old ninth-button prototype as a shortcut. Native controls remain appropriate for supported native-window extensions; Almanac itself uses an overlay inside the game window, not a desktop popup.

## Implemented versus connected

| Area | Source / contract | Actual state |
|---|---|---|
| Native-button groundwork | `src/ui_button_core.h`, private rectangle work in `plugins/ui_panels` | Earlier shared groundwork, NOT a working Almanac launcher. Existing native path still needs Windows/regression acceptance. |
| Finance data/view/editor | `src/almanac_finance_{snapshot,view,editor}.h`; `tests/almanac_button/FINANCE_*.md` | Host-tested policy and view code; not wired to the overlay. Taxes v3 configured-policy snapshot exists; do not invent a second tax authority. |
| Lifecycle/session/transport | `src/almanac_panel_state.h`, `almanac_finance_{session,executor}.h`, `almanac_overlay_{bridge,controller}.h` | Serialized host policy and tests. No real game safe-phase scheduler connection. |
| Synthetic panel | `prototypes/almanac_overlay/almanac_overlay_view.{h,cpp}` | Headless-tested. Use `DrawOverlayReadOnlyPrototype` for first native candidate. Older `DrawOverlayPrototype` has interactive Close/window controls and is not interchangeable. |
| Earthworks styling | `prototypes/almanac_overlay/earthworks/` | Copied paper/ink style, GPL-3.0-only with attribution/full license. No native textures, fonts or gameplay code copied. |
| Toggle policy | `prototypes/almanac_overlay/almanac_debug_toggle.h` | Host-tested: auto-shows once on the first eligible foreground frame, hides/disarms on permission loss, then requires a fresh F8 press to hide/reopen. Visibility is not GPU-resource ownership. |
| DX11 drawing component | `prototypes/almanac_overlay/native/d3d11_panel.{h,cpp}` | Windows-only source candidate; NOT Windows-compiled or GPU-tested. Own context/COM resources; supplied chain, auto-visible synthetic drawing, target restoration. |
| Creation/Present/F8 adapter | `prototypes/almanac_overlay/native/graphics_adapter.{h,cpp}` | Invoked by the isolated `almanac_overlay_dev` Init/Start source; not built by normal discovery or Windows-validated. No runtime acceptance. |
| Windows compilation/diagnostic | `prototypes/almanac_overlay/native/CMakeLists.txt`, `panel_window.cpp`, [Windows check](../prototypes/almanac_overlay/native/WINDOWS_CHECK.md) | Explicit x64 MSVC build target and standalone WARP-DX11 F8 test-window source added; not Windows-compiled/run. Compiles adapter source but does not install hooks. |
| Plugin integration | `native/dev_plugin.cpp`, `sha256.h`, default-on settings and isolated CMake `ALMANAC_BUILD_DEV_PLUGIN` | Development Init/Start source and DLL target remain isolated from `build.bat`; build output must be manually copied as `almanac_overlay_dev.dll` and enabled in Tesmio. Not Windows-built/deployed. Verifies backing-file hashes/call bytes; synthetic drawing defaults on without modal tracking. See [development DLL contract](../prototypes/almanac_overlay/native/DEV_PLUGIN.md). |
| Interaction/live Finance | No native overlay input arbitration or provider/safe-phase hookup | **Missing.** Do not call host Finance services from Present simply because host tests pass. |

All overlay prototype/native candidate code intentionally lives outside `plugins/` and `build.bat` discovery. The previous test log listed only `almanac_button.dll`, so it did not activate this overlay candidate. The corrected playtest guide names the exact separate DLL, loader setting and copy location.

## What we discovered (evidence and limits)

### Earthworks route

[Port map](almanac-earthworks-port-map.md) traces the accessible Earthworks snapshot to startup, creation observation, Present, context setup, input and cleanup. Its graphics bootstrap is primarily **symbol/interface based**, not a collection of game RVAs: engine import `d3d11.dll!D3D11CreateDeviceAndSwapChain`, then `IDXGISwapChain` vtable slot 8 for Present. Slot 8 is an interface index, not an address to relocate.

Reuse the architecture, not its monolithic runtime: Earthworks also installs terrain/facility/native-input/DirectInput/underlay machinery that Almanac does not need. Source inspection found initialization rollback gaps, direct untracked vtable writes, manual cleanup paths and special Present1 re-entry behavior. Our candidate does not copy that re-entry fallback. The full native port still requires review and runtime evidence; existing mods are not compatibility proof.

### Matching-binary S0

Beam published static exports in [docs/evidence/almanac-overlay-s0](evidence/almanac-overlay-s0/MANIFEST.md), commit `4589d3a`. **Read the raw assembly together with [Arena's correction](almanac-overlay-s0-reconciliation.md); the S0 findings report contains incorrect output-argument labels.**

Operator-reported identities (not independently rehashed in Arena):

- `SOVIET64.exe`: SHA256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`.
- `C3DDLL64.dll`: SHA256 `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516`.

Raw exports support engine creation import RVA `0x16B750`, called at engine RVA `0x148BC` within `Initialize` at RVA `0x145C0`. The corrected Win64 call setup identifies outputs at engine RVAs **`0x1D7730` (swap chain), `0x1D76C0` (device), `0x1D76C8` (immediate context)**. These are static output destinations, not guaranteed lifetime-safe runtime accessors. The candidate reads none of these globals.

The post-success vtable call at offset `0x128` is through the device, consistent with GetFeatureLevel, NOT proof of a swap-chain method. Static xrefs do not establish plugin Start versus graphics creation ordering. That timing remains unresolved. Hash output and full PE import-table dumps were not retained for all report claims; distinguish selected-slot evidence from the report's broader tables.

### Loader and input lessons

- Existing `src/tesmioloader.cpp::PatchIat` saves the current slot target but does not check ownership or offer unpatch. Protection-restoration failure is ignored even after modification. A detour DLL must remain resident while reachable; failed/partial installation does not make unloading safe.
- `plugins/ui_overlay/ui_overlay.cpp` already samples F8 for the **native-text budget/HUD**, after `C3D_TERRAIN::Render`. This is not an ImGui backend. The candidate Present adapter separately samples F8's physical high bit; it leaves existing HUD behavior unchanged, so both would toggle if enabled. Resolve that coexistence explicitly, not by silently removing budget behavior.
- NoInputs flags alone do not make an arbitrary shared ImGui context input-safe. A headless test with global navigation enabled observed keyboard capture from ImGui's fallback window. Initial mode therefore uses a dedicated draw-only context, navigation disabled and no Win32 input backend. This is not proof of real game input isolation.
- Native `ui_panels` extends existing windows; it does not create a global popup, and widget helpers cannot be used outside their widget pass. `game+0x5E6` is not visual content suppression. Do not revive speculative sidebar/window-creator patches; see the historical evidence links below.

## Native candidate contract and open risks

See [native README](../prototypes/almanac_overlay/native/README.md) and source, not just this summary. Installation now needs explicit development opt-in and a verified matching engine/host, **not a pre-existing HWND**. Creation-time selection checks engine return RVA `0x148C2`, current-process top-level window ownership, executable HINSTANCE, exact evidenced class and returned swap-chain OutputWindow. The new development entry point checks matching module backing-file SHA256 values and loaded creation-call bytes before calling the installer. The installer alone still assumes verified binary identity; this is not a security/in-memory integrity guarantee. Drawing permission defaults false and no longer gates observation; missed creation still has no fallback. This contract update requires fresh native review/compilation.

The adapter uses expected-target CAS, rejects an already replaced creation-import target, and pins its DLL before publishing reachable hooks. It saves/forwards the previous Present result/arguments, bypasses foreign chains/test presents, serializes drawing, disables drawing on failure and rejects render-owner migration. These are source intentions, not validated race/coexistence guarantees. Shared vtable effects, pointer publication, re-entry chains, permission races and protection changes require review.

Chain/panel references are deliberately process-long. No hot unload, full device replacement, recovery or hook removal is implemented. The renderer releases its per-frame RTV/back-buffer references, but resize/device lifetime and complete OM/UAV/pipeline restoration remain unproven. C++ exception handling is not native access-violation recovery. DPI currently uses back-buffer pixels/default font, not verified game scaling. Modal/readiness permission has no real game producer yet. Do not simply hard-code permission true to claim input safety.

## Verification and review status

Follow-up reports: Gemini published startup evidence `d67cf4d` and corrections `d36333d` on Almanac; Arena fetched/read them. Normal completed-injection ordering supports early observation, but timeout/direct-launch/post-window claims must retain the qualifications in `native/WINDOWS_CHECK.md`. Beam reports a successful x64 MSVC 19.51 (VS 2026, not the documented VS 2022) build, standalone launch/survival and clean close of **`8facb6e`**, with visual F8/resize/focus checks NOT TESTED. That report is not a test of the later adapter changes. Raw Windows logs were not yet available on fetched Almanac when checked; treat it as an operator report pending artifact verification. Gemini's separate source review raised no confirmed defect after source triage: clearing an RTV does not require OM binding, LONG_PTR is pointer-sized on x64, PUBLIC includes propagate, and lack of device-loss recovery is documented.

The following bullets record earlier runs/publication history, not a claim that the current native revision was Windows-tested:

- Recorded implementation runs passed **13 checks** normally and with ASan/UBSan: 11 strict C++ host suites, tax extraction and optional headless ImGui drawing. A later normal run also passed after renderer preparation. These are prior run results, not rerun by this docs-only update.
- Headless tests cover synthetic/unavailable rows, window bounds after simulated viewport changes, noninteractive flags, toggle policy, style isolation and draw output. They do not visually inspect a GPU panel or exercise any Windows native adapter.
- **No Windows compile/link, native/GPU/input test, deployment or Almanac game execution has occurred for this candidate.** No working DLL/artifact hash can be supplied yet.
- PR #80 merged groundwork; #81 merged the completed-result stale-ack fix and consolidated test runner. #82 merged drawing/toggle/style/renderer/adapter preparation as `6e3d61f`.
- Kilo passed PR #82 head `0aade50` (panel/renderer preparation). The adapter commit `3a964e6` started a new check, still running at merge and the original handoff update. It has now completed **SUCCESS**, and its published review reports no issues in the adapter diff. That review explicitly does NOT claim Windows compilation, game integration or hook/install acceptance. It also does not cover the new Windows build/diagnostic changes on this branch.
- No independent child reviewer is exposed in this Arena session; no independent approval of this documentation update is claimed. Beam S0 likewise reported no independent reviewer.

Reproduce host checks (offline by default) using [HOST_TESTS.md](../tests/almanac_button/HOST_TESTS.md):

```sh
python3 tests/almanac_button/run_host_tests.py
python3 tests/almanac_button/run_host_tests.py --sanitize
# With an explicitly acquired unmodified ImGui checkout at the documented pin:
python3 tests/almanac_button/run_host_tests.py --imgui-dir PATH_TO_IMGUI
```

Default run: 12 checks, optional drawing skipped. With ImGui: 13. Pin: upstream v1.91.9b, commit `f5befd2d29e66809cd1110a152e375a7f1981f06`. The runner verifies used core dependency bytes, not Windows backend compilation. Ignored caches/build summaries may not survive a new workspace; acquire dependencies explicitly and retain the tested source revision with results.

## Concrete next work / completion evidence

1. **Independent review:** no child-review capability is exposed in this workspace, so the native adapter/visibility change is not independently reviewed or accepted. Do not infer runtime safety from host tests or a passing code-review bot.
2. **Matching-binary operator:** establish loader Start versus graphics creation timing and how to identify the correct HWND early enough. Source tracing now shows loader DllMain → Init → LoadPlugins → StartPlugins, so Start is under the loader lock, not a safe graphics/window initialization callback. See the [startup blocker](../prototypes/almanac_overlay/native/WINDOWS_CHECK.md). Provide exact commands, identities, raw exports/observations, thread/order evidence and explicit unknowns. Existing S0 is static only; any runtime diagnostics need separately scoped authorization. Do not invent a dummy chain/global-pointer fallback if installation is late.
3. **Windows builder:** build the exact `almanac_overlay_dev` CMake target at the pinned ImGui revision; verify exports and retain the output log/hash. The current log shows only `almanac_button.dll`, so staging the separate Almanac DLL is mandatory. Do not relax its module-hash or creation-call guards.
4. **Optional renderer check:** the standalone WARP test window uses the same panel, auto-show and F8 hide/reopen policy. It can isolate a rendering failure without the game, but does not validate Present interception, loader startup timing or game coexistence.
5. **Disposable-copy operator:** stage only the built `almanac_overlay_dev.dll` in the loader directory that produced the log; enable its matching `[plugins]` entry. Launch Tesmio manually and verify the startup, creation-observed and panel-visible log lines plus a screenshot. Confirm auto-show, F8 hide/reopen, focus loss and resize; do not make save/policy or terrain changes. The previous log was from `almanac_button.dll` only and is not a panel test.
6. **Then:** add reviewed interaction/input ownership and recovery; connect real read-only Finance through a verified safe-phase adapter; editing follows session/world/revision safety proof. Final player launcher is unresolved. Audit full Almanac-to-main changes and regressions before a product merge; branch synchronization alone is not release readiness.

## Reading map / licensing

- [Prototype README](../prototypes/almanac_overlay/README.md), [native candidate README](../prototypes/almanac_overlay/native/README.md): source usage and limits.
- [Earthworks port map](almanac-earthworks-port-map.md), [S0 raw manifest](evidence/almanac-overlay-s0/MANIFEST.md), [S0 corrections](almanac-overlay-s0-reconciliation.md): renderer evidence.
- [Overlay design](almanac-overlay-design.md), [review reconciliation](almanac-overlay-review-reconciliation.md): ownership and separate S0/D0/V0/I0/F0 gates. A merged preparation PR does not close native acceptance gates.
- [Host tests](../tests/almanac_button/HOST_TESTS.md) and adjacent Finance/session/executor/bridge/controller/lifecycle contracts: existing host safety work. In particular stale completed acknowledgements remain sticky-invalidated after context changes without rewriting the real execution result.
- [UI direction](almanac-ui-direction-20261114.md): overlay decision and earlier native-sidebar evidence corrections. Older `almanac-plan.md`, implementation spec and sidebar investigations are historical requirements/research, not an instruction to resurrect rejected native patches.
- [Earthworks NOTICE](../prototypes/almanac_overlay/earthworks/NOTICE.md): exact local source hash, provenance and GPL-3.0-only extraction. ImGui is MIT. Preserve both notices/source obligations; combined-binary distribution treatment remains unresolved. No repository-wide license was assigned. The Earthworks Tesmio API 4 header hashes have been corrected against the pinned SDK, but Windows compilation is unverified. GitHub returned 404 for the RepublicMP-pinned checkout; no cause is assumed. Its provenance notice remains; Earthworks and the separate Almanac candidate use separately pinned upstream Dear ImGui v1.91.9b.

No separate repository migration has been performed. The user explicitly deferred that operation. Local-agent prompts belong in chat, not repository prompt files.
