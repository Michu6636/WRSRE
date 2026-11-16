# Almanac Overlay Windows Check — Evidence Report (Commit 093856d)

## Scope and authorization

Isolated Windows build and standalone DX11 renderer diagnostic for the Almanac
overlay prototype at merged commit `093856d1604d1fac783f011275496df6a6d946ee`.
NOT game integration, NOT plugin deployment, NOT runtime hook testing. No game
run, no DLL deployed, no game config changed, no Ghidra access.

Source revision tested: `093856d1604d1fac783f011275496df6a6d946ee`
Branch: `Almanac` (isolated worktree at `C:\WRSRE-almanac-0938`)
Local diff: none (clean checkout).

## Environment and dependency identities

| Component | Identity |
|---|---|
| OS | Windows (win32) |
| MSVC compiler | 19.51.36257 (Visual Studio 18 Community 2026) |
| Windows SDK | 10.0.26100.0 |
| CMake | 4.4.0 (w64devkit) |
| Git | 2.56.0.windows.4 |
| Python | 3.14.7 |
| Host C++ compiler | g++ (GCC) 16.1.0 (w64devkit) |
| ImGui dependency | commit `f5befd2d29e66809cd1110a152e375a7f1981f06` (v1.91.9b), verified |

Toolchain deviation: WINDOWS_CHECK.md specifies VS 2022; that installation is
empty on this host. Used VS 18 (2026) / MSVC 19.51.36257. No source changes.

## Build results

### 1. Standalone diagnostic (almanac_panel_window.exe)

Configure: PASS (see configure.log)
Build: PASS (see build_panel.log)
- Static lib `almanac_imgui` (ImGui core + DX11 backend): OK
- Static lib `almanac_native_candidate` (d3d11_panel, graphics_adapter,
  almanac_overlay_view, panel_style): OK with /W4 /WX
- Executable `almanac_panel_window.exe`: OK

### 2. Development DLL (almanac_overlay_dev.dll)

Build: PASS (see build_dev.log)
- Target: `almanac_overlay_dev` (SHARED), opt-in via ALMANAC_BUILD_DEV_PLUGIN=ON
- CRT: static (/MT) per loader contract
- All native-candidate sources compiled into the DLL

### 3. Launcher (tesmiolauncher.exe)

Build: PASS (see build_launcher.log)
- Compiled from src/tesmiolauncher.cpp with cl.exe /MT /O2 /W3
- Note: src/tesmiolauncher.rc could NOT be compiled — missing logo.ico
  (not present in repository). Launcher built without icon resource.
  This is a pre-existing repository gap, not a source change.

## DLL export verification

dumpbin /exports confirms exactly 3 exports (see dumpbin_exports.log):
- TsmPluginApiVersion
- TsmPluginInit
- TsmPluginStart

## Artifact SHA256 values

| Artifact | SHA256 | Size |
|---|---|---|
| almanac_panel_window.exe | `34EE3DF053A3D3426348A57375F5B924F386A77D32A997DDABA55F8694BA1860` | — |
| almanac_overlay_dev.dll | `BD7E2B1EEFAD3FEF6ADE2BF92A07F50240AD8E561C0A5565C334B0D65A0C4810` | — |
| tesmiolauncher.exe | `28D7992B9FD21387FAA032480ED6996A26C559687BF4AE7F2CF806631DBE4892` | — |

## Dependency guard tests

11 checks passed (see dependency_guard.log).

## Host tests

All 13 checks passed (see host_tests_summary.json).

## Standalone diagnostic execution

Executable launched successfully, survived 5+ seconds, graceful close → exit
code 0. No crash dialog.

### Interactive visual checks

| # | Check | Status |
|---|---|---|
| 1 | F8 toggle (dark → panel on press, hide on next press, no flicker on hold) | NOT TESTED |
| 2 | Synthetic-data banner, invented RUB balance, unavailable policy values, no editable/Close controls | NOT TESTED |
| 3 | Resize including small sizes; re-anchor; no crash/ResizeBuffers error | PARTIALLY TESTED (survived runtime; resize not automated) |
| 4 | Alt-tab/minimize hides/disarms; F8-held return does not reopen | NOT TESTED |
| 5 | Close while shown and hidden; shutdown on render owner; nonzero exit = failure | PARTIALLY TESTED (graceful close → exit 0) |

Automated interaction not performed (P/Invoke unavailable in this shell).
Process-level evidence does not constitute visual acceptance.

## Explicit non-claims

A standalone WARP-DX11 pass does NOT prove game startup ordering, Present-hook
safety, Steam coexistence, game input ownership, or game-DPI correctness. The
dev DLL linked but was not loaded into any process. No hook was installed.
No game was run.

## Failures

None for build/configure/test/launch. One pre-existing gap: missing logo.ico
prevents .rc compilation (icon only, no functional impact).

## Independent review

Not performed; reported as not independently reviewed.
