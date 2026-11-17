# Read-only Almanac drawing prototype

Current cross-subsystem handoff: [Almanac engineering status](../../docs/almanac-engineering-status.md).

The current Almanac panel-spawn check is documented in [`test_menu/README.md`](test_menu/README.md). It builds and manually stages the separate `almanac_overlay_dev.dll` into a disposable Tesmio copy; it does not use Earthworks' gameplay DLL or alter the native sidebar.

This directory remains outside `plugins/` and normal `build.bat` plugin discovery. It contains synthetic drawing code, a development DLL entry point and creation/Present adapter under `native/`. The explicit `build.bat almanac_overlay_dev` target invokes the isolated CMake project and copies the DLL/config to this checkout's `build/plugins`; it is not part of the ordinary full build and does not deploy to another game directory. The panel auto-shows on its first eligible foreground render and F8 hides/reopens it. Windows compilation, runtime staging and in-game visual acceptance are still outstanding. `almanac_button.dll` is a separate native-sidebar prototype; its load log is not evidence that the ImGui panel loaded.

`DrawOverlayReadOnlyPrototype` is the new draw-only candidate: it has no Close button, titlebar controls, movement, resize, navigation or mouse interaction. Its window uses NoInputs/NoDecoration/NoMove/NoFocusOnAppearing/NoBringToFrontOnFocus flags and re-anchors/resizes each frame against the viewport work area. It skips invalid/minimized/tiny work areas instead of retaining stale visible bounds. Its banner wraps at the window edge. Visibility is controlled by its caller, not a hotkey or click.

`DrawOverlayPrototype` retains the older interactive drawing experiment and Close action for host testing; it is NOT approved for native integration. The caller of either function owns the context, frame boundaries, resources and scaling. For the read-only candidate use a dedicated draw-only context with no platform input backend, global keyboard/gamepad navigation disabled and no other interactive windows. Do not force global capture flags off in a shared interactive context. An initial test with global keyboard navigation enabled showed ImGui's fallback debug window claiming Tab despite the read-only window's NoInputs flags; disabling global navigation in the test's dedicated draw-only context resolved that. Window flags alone are not a general input-isolation guarantee.

Neither path calls services, executes edits or infers game readiness. No live Finance controller connection is made here. A future backend must still satisfy its own readiness, graphics-state, input and lifetime review gates.

`SyntheticOverlayView` reports a fixed invented RUB treasury, with policy fields deliberately unavailable. The synthetic-data banner must remain visible in any development integration. This is not the user's republic. Window anchoring/sizing uses viewport-relative ratios and contents use a flow table. Window drawing does not establish correct DPI behavior, native-modal arbitration or click-through protection.

## F8 development visibility policy

`almanac_debug_toggle.h` supplies a host-only `DebugOverlayToggle`. The current dev panel auto-shows once on the first allowed foreground frame. F8 then toggles it once per fresh press; held/repeated samples do not flicker it. Permission loss hides/disarms it, and focus restoration with F8 held cannot reopen it. Hiding does not create/destroy resources or change the Finance/panel lifecycle.

The current development adapter samples physical F8 on the selected swap chain's Present callback and applies this policy. Its synthetic-drawing permission now defaults on in the development DLL, but there is no native modal-state provider, so it may cover native dialogs. The adapter checks foreground/visible/not-minimized state; it captures no input and does not consume game keys. Use only for disposable-copy visual tests and do not click through the panel or dialogs. This class itself does not poll Windows, install hooks, assign a final player launcher or prove that F8 is unused; the existing budget HUD also uses F8.

When the development DLL is manually built and loaded for a test, it draws the synthetic read-only view only when permission and `Visible()` are true. It does not connect to live Finance or modify resources. Host tests cover startup-held keys, repeat, alternating presses, permission loss/restoration, releases while blocked and explicit hiding. Native F8 operation remains untested.

## Dependency and verification

The Finance drawing code was written for this prototype. The separate `earthworks/panel_style.cpp` now copies Earthworks' paper/ink style block under GPL-3.0-only, with provenance and full license in that directory. The headless test applies it to an owned context; no native graphics/input code or game textures were copied. See `docs/almanac-earthworks-port-map.md` for the backend source comparison and remaining gates. This adds GPL-derived prototype source; it does not license the rest of the repository or authorize a combined binary release. Dear ImGui itself is MIT licensed (Omar Cornut and contributors); retain its LICENSE.txt with any future vendored/distributed copy.

The pinned RepublicMP checkout used to supply Earthworks' ImGui sources was unavailable from this environment (GitHub returned 404 for the pinned commit); no cause is assumed. Earthworks now builds against separately pinned upstream Dear ImGui **v1.91.9b**, commit `f5befd2d29e66809cd1110a152e375a7f1981f06`. CMake checks the commit and raw bytes of all linked core/Win32/DX11 inputs at configure and build time. This fixes the dependency path only; Earthworks remains GPL-derived and its runtime compatibility is not established by building.

Fetched through GitHub's contents API into ignored `.cache/almanac-imgui-headers`, not vendored or installed in the game:

- imgui.h SHA256 `2d0180859ff85e7fe66ed9999b0f05f2fc45176773046a8f86b3b6bd190b0765`
- imconfig.h SHA256 `0884061cc43e27e1d346fe18dcd1425792eb2532d3d16610a82eb5ac751f7594`
- Core implementation: imgui.cpp, imgui_draw.cpp, imgui_tables.cpp, imgui_widgets.cpp, imgui_internal.h, imstb_rectpack.h, imstb_textedit.h, imstb_truetype.h, plus LICENSE.txt, all from that tag. No platform/GPU backends used.

The updated headless test covers the original interactive smoke path and the separate draw-only path. It checks NoInputs/control flags, absence of a titlebar Close control, re-anchoring/window bounds after synthetic viewport changes at 1280x720, 800x600, 1920x1080 and 320x240, valid nonempty draw output, and no capture requests in the dedicated draw-only context while synthetic clicks/Tab events are injected. Zero/tiny viewport cases omit drawing. These are headless coordinate/input checks, NOT visual inspection, native window resizing, real keyboard/mouse isolation or GPU rendering.

All 13 host/extraction/headless suites (including the added debug toggle) passed via the consolidated runner normally and with ASan/UBSan after the style extraction. Tests also verify style values, repeat application without cumulative scaling, and isolation from a second ImGui context. The dependency was obtained explicitly at the recorded pin in ignored cache; it is not vendored. No OS window or game was created. Independent child review is unavailable here and is not claimed.

Reproduce after obtaining the dependency at the specified pin:

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Isrc \
  -Iprototypes/almanac_overlay -isystem .cache/almanac-imgui-headers \
  tests/almanac_button/almanac_overlay_view_test.cpp \
  prototypes/almanac_overlay/almanac_overlay_view.cpp \
  .cache/almanac-imgui-headers/imgui{,_draw,_tables,_widgets}.cpp \
  -o build/almanac_overlay_view_test
build/almanac_overlay_view_test
```

## D3D11 drawing candidate

`native/d3d11_panel.{h,cpp}` contains the Windows/DX11 draw component for the synthetic Earthworks-styled panel. It takes the selected swap chain and physical F8 state; it installs no interception itself. It is wired to the isolated development DLL, which auto-shows it on the first eligible foreground frame. Neither the DLL nor renderer has been Windows-compiled or GPU-tested. See `native/README.md` for the owner/lifetime contract and remaining acceptance gates, and `docs/almanac-overlay-s0-reconciliation.md` for the corrected Beam evidence. Host tests do not cover these Windows-only files.

## Acceptance gate

The isolated `almanac_overlay_dev` source now wires the existing ImGui panel to the creation/Present adapter, but remains an unbuilt development candidate. Before production integration, interaction or live Finance, independent review must establish hook ownership/coexistence, safe state restoration, lifecycle/thread routing, partial initialization and device/window recovery, pinned dependency/license packaging and modal/input behavior. Existing repository rules disallow speculative native hooks; no child-review capability is exposed here, so independent review is outstanding. The Windows DLL build, disposable-copy staging and visual in-game check have not occurred.
