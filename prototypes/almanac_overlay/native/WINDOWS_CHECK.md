# Windows compilation and standalone renderer check

This is a diagnostic **desktop test window**, not the Almanac product UI. The product remains an in-game overlay. This check uses the same `D3D11Panel` and synthetic Earthworks-styled view, without a game, loader, hook installation or Finance services. It lets a Windows engineer check actual DX11 rendering before risking game integration. Nothing is installed/copied into a game directory.

## Build (not yet executed on Windows)

Prerequisites: Visual Studio 2022 x64 C++ tools/Windows SDK, CMake 3.24+, Git. From the repository root, explicitly acquire the existing prototype dependency (skip clone if already available and unmodified):

```powershell
git -c core.autocrlf=false clone --depth 1 --branch v1.91.9b https://github.com/ocornut/imgui.git .cache/almanac-imgui-host-tests
$imgui = (Resolve-Path .cache/almanac-imgui-host-tests).Path
cmake -S prototypes/almanac_overlay/native -B build/almanac_native -G "Visual Studio 17 2022" -A x64 "-DALMANAC_IMGUI_DIR=$imgui"
if ($LASTEXITCODE -ne 0) { throw "Almanac configure failed" }
cmake --build build/almanac_native --config Debug --target almanac_panel_window
if ($LASTEXITCODE -ne 0) { throw "Almanac build failed; do not run a stale executable" }
```

CMake requires exact ImGui HEAD `f5befd2d29e66809cd1110a152e375a7f1981f06` and checks raw bytes of all used core/header/license/DX11-backend files against that commit. It does not fetch dependencies. Wrong platform, missing dependency or modified inputs fail configuration. The pin guard also runs before compilation on every build, including incremental builds, so changing dependencies after configuration does not silently bypass it. First builds must retain output: warnings-as-errors is enabled for candidate/harness source, not upstream ImGui.

Outputs are an isolated static native-candidate library and `build/almanac_native/Debug/almanac_panel_window.exe`. The adapter source is compiled into the library, but the diagnostic never calls its installer. The separate `almanac_overlay_dev` DLL target is on by default in the isolated CMake project; this target-specific command does not build it. This is **not full adapter DLL link or hook testing**. No automatic CTest run is provided, and normal `build.bat` behavior is unchanged.

## Manual diagnostic acceptance (Windows operator)

Only run after a successful build/source review. Launch the executable directly, NOT through the game. It creates its own WARP software-DX11 device/swap chain and an obviously labelled window. A successful process exit does not automatically certify the following visual checks:

1. Initially only a dark background appears. Release F8, then press it: paper/ink synthetic Finance panel appears. Holding F8 must not flicker/toggle repeatedly; next fresh press hides it.
2. Confirm the synthetic-data banner, invented RUB balance, unavailable policy values and absence of editable/Close controls. This is not your republic's data.
3. Resize repeatedly, including small sizes. Panel re-anchors; extremely small/minimized work areas omit drawing. No crash or ResizeBuffers error should occur. Clipping at tiny sizes is not final usability acceptance.
4. Alt-tab/minimize with the panel visible: it hides/disarms. Returning while F8 is held must not reopen it; release and press again. Neither ImGui mouse capture nor input suppression is implemented.
5. Close the OS window while shown and hidden. Panel shutdown must occur on the rendering owner before device/window destruction. Any failure dialog/nonzero exit is a failure; retain evidence, do not keep retrying blindly.

Record source commit/diff, MSVC/SDK/CMake/dependency identities, configure/build output, executable SHA256, screenshots and exact steps/results. Run a Debug configuration first. This diagnostic does NOT establish native sidebar/modal behavior, Steam coexistence, GPU-driver behavior (it uses WARP), correct game DPI, or original-Present chaining.

## Startup blocker discovered in current loader source

`src/tesmioloader.cpp::DllMain(DLL_PROCESS_ATTACH)` calls `Init()`, which calls `LoadPlugins()`, which calls `StartPlugins()`. Thus current plugin Start runs under the loader lock. This is source ordering, NOT proof of timing versus engine device creation. Do not start a graphics/window creation routine, wait for a worker thread, or assume a game HWND already exists in Start.

The original adapter required an externally verified HWND at install time, making it unsuitable for normal pre-window plugin Start. Gemini's published startup packet (`d67cf4d`, addendum `d36333d`, on Almanac) supports moving validation to creation time. The current source now installs without an HWND and validates the creation caller/window/returned chain before selection; see native README. This is NOT a tested automatic plugin entry point, and binary-identity enforcement is still a caller prerequisite. Do not silently take the first arbitrary window/chain or read unverified globals.

Normal completed launcher injection supports Start-before-window ordering. The launcher's unchecked wait timeout can instead resume the main thread while injection is incomplete; it does not establish which game instructions then execute concurrently. No universal ordering, direct-launch attachment rule or post-window interception impossibility is inferred. The old raw packet's `SovietClass` annotation is superseded by the addendum. Beam's reported Windows build/launch at `8facb6e` does not compile/test the new adapter revision; obtain a fresh Windows compile after review.

## Checks actually performed in Arena

- All 13 existing host/extraction/headless checks passed again normally.
- `tests/almanac_button/run_native_dependency_tests.py` passed 11 guard checks: original pin, disposable copy with spaces in its path, absent/nonexistent dependency, modified core/license/DX11 source/header, missing core header, wrong Git HEAD, and restored fixture. Tests modify only a disposable file copy, not the shared dependency. Run separately with `python tests/almanac_button/run_native_dependency_tests.py --imgui-dir PATH_TO_IMGUI --cmake PATH_TO_CMAKE`. These 11 checks are separate from the 13 host-suite count.
- Native CMake configuration rejected the Linux/GNU toolchain as intended.
- No Windows compiler/SDK exists in this sandbox: no Windows compile/link, standalone window execution, game run or deployment was performed. Independent child review remains unavailable.

Licensing: this diagnostic links the GPL-derived Earthworks style with the native candidate and MIT ImGui. Preserve attribution, GPL source obligations and ImGui MIT notice with any distribution; this is not permission to relicense unrelated WRSRE content or publish a release package.
