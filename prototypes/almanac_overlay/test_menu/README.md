# Earthworks panel-spawn test

This step tests **only the real Earthworks panel**. It does not build, copy, or enable the synthetic Almanac DLL, and it does not change the native sidebar. Earthworks is a full gameplay plugin; the panel is not a separately loadable component.

## Build Earthworks on Windows

From the repository root, in PowerShell with Git for Windows and Visual Studio 2022's required C++ components installed, run:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\Mods for analysis\wrsr-Earthworks-main\scripts\Fetch-Dependencies.ps1" -BuildOnly
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\Mods for analysis\wrsr-Earthworks-main\scripts\Build.ps1"
```

The build script checks the x64 DLL and Tesmio exports. The output is:

`Mods for analysis/wrsr-Earthworks-main/build/windows-x64/bin/EarthworksPlanning.dll`

The build uses the GPL-3.0-only Earthworks source; retain its notices and source obligations. No game is staged or launched by these commands.

## Show the panel in the disposable copy

1. Use the **disposable game copy and disposable save** you confirmed earlier. Earthworks records WRSR **1.1.1.13 / Steam build 24922914** as its baseline. If your copy is 1.1.1.9, this is exploratory only; do not bypass its hook/preflight checks.
2. Copy `EarthworksPlanning.dll` to the Tesmio `plugins` folder in that copy. In that copy's existing `tesmioloader.ini`, under `[plugins]`, set:

   ```ini
   EarthworksPlanning=1
   almanac_overlay_dev=0
   ```

   Leave every other plugin entry alone. The second line keeps the old Almanac development DLL off if it is present.
3. Run TesmioLauncher from the copy and click **Launch** yourself. Once the first ready world and camera are available, the panel should open automatically. Earthworks' own floating **Earthworks** button closes/reopens it; this does not add or alter a native sidebar button.
4. This is only a panel-spawn check: do not edit terrain, use planning actions, or change Earthworks settings. Stop if its hook/preflight checks report an error. Earthworks also stores preferences at `%LOCALAPPDATA%\EarthworksPlanning\settings.ini`, outside the game copy; do not change those settings for this test.

The disposable-copy requirement is because Earthworks is a full gameplay plugin that installs hooks and can perform gameplay edits—the DLL cannot be loaded as a visuals-only panel. The user launches and tests the game; no game was staged or launched in this workspace.
