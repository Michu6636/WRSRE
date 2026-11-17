# Almanac ImGui panel smoke test

This guide builds and stages the separate `almanac_overlay_dev.dll`—the actual Almanac ImGui/D3D11 test panel. It draws clearly labelled synthetic, read-only Finance values; it does not load Earthworks' terrain/gameplay plugin, connect live Finance, or change the native sidebar. The attached `tesmioloader-4.log` loaded only `almanac_button.dll`, so that run never loaded this panel.

## Build on Windows

Prerequisites: Git, CMake 3.24+, Visual Studio 2022 x64 C++ tools and Windows SDK. From the repository root, fetch pinned Dear ImGui v1.91.9b and build the explicit DLL target:

```powershell
git -c core.autocrlf=false clone --depth 1 --branch v1.91.9b https://github.com/ocornut/imgui.git .cache/almanac-imgui-host-tests
$imgui = (Resolve-Path .cache/almanac-imgui-host-tests).Path
cmake -S prototypes/almanac_overlay/native -B build/almanac_native -G "Visual Studio 17 2022" -A x64 "-DALMANAC_IMGUI_DIR=$imgui"
if ($LASTEXITCODE -ne 0) { throw "Almanac configure failed" }
cmake --build build/almanac_native --config Debug --target almanac_overlay_dev
if ($LASTEXITCODE -ne 0) { throw "Almanac DLL build failed; do not stage a stale DLL" }
```

CMake verifies the ImGui commit and source bytes. The DLL output is `build/almanac_native/Debug/almanac_overlay_dev.dll`. This isolated target is not built by `build.bat`.

## Stage and check a disposable copy

1. Close the game/loader. Use the disposable game copy and save; point at the exact Tesmio runtime directory containing the `tesmioloader.ini` used by its launcher.
2. Copy `build/almanac_native/Debug/almanac_overlay_dev.dll` into that directory's `plugins` folder. Copy `prototypes/almanac_overlay/native/almanac_overlay_dev.ini` there as `almanac_overlay_dev.ini` (or preserve an existing copy with both settings enabled).
3. In that runtime's `tesmioloader.ini`, under `[plugins]`, set `almanac_overlay_dev=1`. Leave other plugin settings alone.
4. If the old `almanac_button.dll` is present, disable its unrelated ninth-sidebar experiment only in this disposable copy: set `enabled = 0` under `[almanac_button]` in `plugins/almanac_button.ini`. It is not the ImGui panel.
5. Launch the game through Tesmio yourself. The synthetic panel should appear on the first eligible foreground game render. F8 hides/reopens it. Do not click through it, use planning/gameplay actions, or make terrain/save changes; it does not capture input and may cover native dialogs.
6. Verify the log contains `plugin ... almanac_overlay_dev.dll`, `almanac dev: observer installed`, `almanac graphics: creation observed`, and `almanac graphics: synthetic ImGui panel visible`. The plugin deliberately declines on a game/engine hash or creation-call mismatch; do not bypass those checks. If the plugin entry is absent, verify the DLL is in this exact loader's `plugins` folder and enabled in this exact loader's INI.

The build and copy steps do not launch the game. Runtime visual confirmation remains the user's step on the disposable copy; no Windows build, staging or game launch has been performed in this workspace.
