# Read-only Almanac drawing prototype

This directory is intentionally outside `plugins/` and the normal plugin build. It provides actual Dear ImGui panel drawing code, not a deployable plugin. It installs NO Present, WndProc, DirectInput, native Draw or sidebar hook. No temporary shortcut is implemented yet. Do not enable the old experimental `almanac_button` plugin to test it.

`DrawOverlayPrototype` draws a labelled synthetic-data Finance table and Close control. Its caller must own the ImGui context, frame boundaries, rendering resources, coordinate/font scaling and input routing. Call only while the panel is logically open; return actions.close to the lifecycle owner rather than destroying resources from inside drawing. It does not call services, execute edits or infer game readiness. No live Finance controller connection is made here.

`SyntheticOverlayView` reports a fixed invented RUB treasury, with policy fields deliberately unavailable. The synthetic-data banner must remain visible in any development integration. This is not the user's republic. Window anchoring/sizing uses viewport-relative ratios and contents use a flow table. Window drawing does not establish correct DPI behavior, native-modal arbitration or click-through protection.

## Dependency and verification

Original drawing code was written for this prototype; no Earthworks/RepublicMP source was copied. Dear ImGui itself is MIT licensed (Omar Cornut and contributors); retain its LICENSE.txt with any future vendored/distributed copy.

The Earthworks-pinned RepublicMP header fetch returned HTTP 404 from GitHub's contents API. No cause (private, unavailable or bad path) is assumed. An initial syntax check consequently failed with `imgui.h: No such file or directory`. Rather than claim that unavailable dependency worked, this prototype was checked against upstream Dear ImGui **v1.91.9b**, Git tag commit `f5befd2d29e66809cd1110a152e375a7f1981f06`. This is a prototype-only dependency selection, not a decision to change Earthworks or the production SDK.

Fetched through GitHub's contents API into ignored `.cache/almanac-imgui-headers`, not vendored or installed in the game:

- imgui.h SHA256 `2d0180859ff85e7fe66ed9999b0f05f2fc45176773046a8f86b3b6bd190b0765`
- imconfig.h SHA256 `0884061cc43e27e1d346fe18dcd1425792eb2532d3d16610a82eb5ac751f7594`
- Core implementation: imgui.cpp, imgui_draw.cpp, imgui_tables.cpp, imgui_widgets.cpp, imgui_internal.h, imstb_rectpack.h, imstb_textedit.h, imstb_truetype.h, plus LICENSE.txt, all from that tag. No platform/GPU backends used.

Strict C++17 syntax checking passed. The headless smoke test creates an ImGui context and font atlas, draws multiple frames at 1280x720, 1920x1080 and 800x600, verifies synthetic/unavailable data and asserts valid nonempty draw output. It passed. This does NOT visually inspect the panel, test clicking Close, verify layout after live resize, prove game input isolation or test GPU rendering. No OS window or game is created.

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

## Next native integration gate

Before adding graphics or input hooks, independent review must establish hook ownership/coexistence, safe state restoration, lifecycle/thread routing, partial initialization and device/window recovery, pinned dependency/license packaging and a safe development activation path. Existing repository rules disallow speculative native hooks; no child-review capability is exposed here, so independent review is outstanding. This prototype remains deliberately unconnected until that gate is satisfied. Windows build, deployment, visible in-game tests and disposable-copy acceptance have not occurred.
