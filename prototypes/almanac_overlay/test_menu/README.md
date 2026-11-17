# Almanac ImGui panel smoke test

This guide builds and stages the separate `almanac_overlay_dev.dll`—the actual Almanac ImGui/D3D11 test panel. It draws clearly labelled synthetic, read-only Finance values; it does not load Earthworks' terrain/gameplay plugin, connect live Finance, or change the native sidebar. The recent `tesmioloader-5.log` lists `almanac_button.dll` but no `almanac_overlay_dev.dll`; therefore the ImGui plugin was not present in the loader's plugin directory and no draw was attempted.

## Build on Windows

Use a disposable Tesmio copy, not a live install. Prerequisites: Git, CMake 3.24+, Visual Studio 2022 x64 C++ tools and Windows SDK. From the repository root, run the new explicit target:

```cmd
build.bat almanac_overlay_dev
```

This target is deliberately excluded from ordinary `build.bat` runs. It fetches Dear ImGui v1.91.9b into the ignored `.cache` directory if `ALMANAC_IMGUI_DIR` is not set, verifies the exact dependency commit/source bytes, builds the DLL, and copies it plus its INI to `build\plugins`. A successful command must end with `almanac_overlay_dev.dll and config staged in build\plugins`; do not use an old DLL if the command fails. To use an already-pinned checkout, set `ALMANAC_IMGUI_DIR` to its path before running the command.

## Stage and check a disposable copy

1. Close the game/loader. Identify the exact Tesmio runtime directory containing the `tesmioloader.ini` used by its launcher. The log's `vfs root` is a useful clue, but the actual plugin folder is always `<directory containing tesmioloader.dll>\plugins`.
2. Copy `build\plugins\almanac_overlay_dev.dll` and `build\plugins\almanac_overlay_dev.ini` into that runtime's `plugins` folder. If `build\plugins` already is that exact disposable runtime folder, do not copy again.
3. In that runtime's `tesmioloader.ini`, under `[plugins]`, set `almanac_overlay_dev=1`. Leave other plugin settings alone.
4. If the old `almanac_button.dll` is present, disable its unrelated ninth-sidebar experiment only in this disposable copy: set `enabled = 0` under `[almanac_button]` in `plugins\almanac_button.ini`. It is not the ImGui panel.
5. Launch the game through Tesmio yourself. The synthetic panel should appear on the first eligible foreground game render. F8 hides/reopens it. Do not click through it, use planning/gameplay actions, or make terrain/save changes; it does not capture input and may cover native dialogs.
6. First confirm the loader log has `plugin ... almanac_overlay_dev.dll`. Then confirm `almanac dev: observer installed`, `almanac graphics: creation observed`, and `almanac graphics: synthetic ImGui panel visible`. If the first line is absent, the DLL is still missing from the directory that this loader scans. If Init reports a hash/call mismatch or the observer declines, do not bypass those checks; preserve the exact log and stop.

The build and copy steps do not launch the game. Runtime visual confirmation remains outstanding until the panel is seen in the disposable copy; no Windows build, staging or game launch has been performed in this workspace.
