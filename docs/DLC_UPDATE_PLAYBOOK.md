# DLC & `SOVIET64.exe` Update Playbook

Whenever 3Division ships a patch or DLC for *Workers & Resources: Soviet Republic*, ASLR-relative `.text`, `.rdata`, and `.data` RVAs inside `SOVIET64.exe` shift on recompile, whereas `CBuilding` / `CPerson` struct member offsets (`src/game_world.h`) typically stay stable unless fields were inserted inside core classes.

WRSRE uses a three-layer **De-Brittling Shield** so a Steam update never causes silent memory corruption or a desktop crash:

1. **PE `TimeDateStamp` Build Gate (`src/game_addresses.h` + `tesmiolauncher.cpp` + `tesmioloader.cpp`):**
   - Baseline defaults target `SOVIET64.exe` **v1.1.1.9** (`TimeDateStamp = 0x6A3EB6AD`).
   - On an unrecognized `SOVIET64.exe` build with no matching `addresses.ini`, `tesmiolauncher.exe` blocks injection up front, and even if `--ignore-version` is passed, `g_tsmAddr.exeVerified` stays `0`—so `TsmGameObject()`, `TsmCallableCodePtr()`, and raw RVA readers/hooks pause cleanly with one log line instead of faulting.
2. **Runtime `addresses.ini` Override Table (`src/game_addresses.h`):**
   - `tesmiolauncher.exe`, `tesmioloader.dll`, and plugins read `<install_dir>\addresses.ini` at startup (`TSM_API_VERSION = 4` preserved; no loader ABI break).
   - When `[build] target_stamp` matches the installed `SOVIET64.exe`'s PE `TimeDateStamp`, relocated RVAs are applied **without recompiling C++**.
   - Setting any optional RVA to `0x0` disables *only* that single sub-feature (e.g., `abandon_building = 0x0` skips physical ruin collapse while keeping enterprises, wages, retail, taxes, and construction live).
3. **Automated Relocation & Drift Scanner (`tools/scan_exe_update.py`):**
   - Pure Python 3 stdlib tool that parses PE32+ headers, `C3DDLL64.dll` IAT call sites, `.pdata` function bounds, `.rdata` IEEE-754 float clusters, string xrefs (`"gui_click"`, version strings), and masked hook prologues in ~2 seconds.

---

## Step-by-Step Patch-Day Workflow

### 1. Run the Automated Scanner Against the New `SOVIET64.exe`

```bash
python tools/scan_exe_update.py "C:\Program Files (x86)\Steam\steamapps\common\SovietRepublic\media_soviet\SOVIET64.exe" --write-ini addresses.ini
```

Copy the generated `addresses.ini` next to `tesmiolauncher.exe` / `tesmioloader.dll`.

### 2. Check the Scanner Output

1. **Struct-Offset Drift Checks (`src/game_world.h`):**
   - Confirm all 6 canary offsets show `[PASS]` (`+0x360`, `+0x12D0`, `+0x121C`, `+0x11420`, `+0x580/+0x588`, `+0x274`).
   - If any offset shows `STRUCT OFFSET DRIFT` or `[WARN]`, **do not launch on existing saves** until `src/game_world.h` (and any inline stub displacement in `citizen_labor.cpp` / `citizen_retail.cpp`) is updated and rebuilt.
2. **Relocated RVAs (`[OK]` vs `[MISS]`):**
   - Core anchors (`game_object`, `sim_timer`, `daytick_power`, `day_length`, `fontmgr`, `panel2d`, `input`, `world_size`, `window_dispatch`, panel builders, `labor_mobility_hook`, `retail_checkout_hook`, `play_gui_sound`) relocate automatically via IAT/string/`.pdata` signatures.
   - Leaf/internal native helpers without unique string/IAT xrefs (`relocate_person`, `abandon_building`, `eject_worker`, `ui_slider`, `ui_checkbox`, `clip_push`) default to `0x0` (`[MISS]`) until filled in from Ghidra if needed; WRSRE runs safely with them at `0x0` (degrading gracefully to no native slider/checkbox/relocate call until set).
3. **Other Active-Branch Plugins (`private_logistics`):**
   - `private_logistics` carries its own internal `0x6A3EB6AD` stamp check in `panel_fuel_runtime.h` and `TSM_RVA_TRANSFER` (`logistics_transfer` in `scan_exe_update.py`'s report). Update its header constant when rebasing/rebuilding `private_logistics` for a new executable.

### 3. Verify Scanner Self-Test Anytime

```bash
python3 tools/scan_exe_update.py selftest
```

### 4. Promoting a New Build to the Compiled-In Baseline

Once a new game patch becomes the standard Steam version:
1. Copy the verified RVAs and `TSM_BASELINE_EXE_STAMP` from `addresses.ini` into `src/game_addresses.h` (`TSM_RVA_DEFAULT_*`) and `BASELINE_RVAS` in `tools/scan_exe_update.py`.
2. Add the new `{ {major, minor, patch, build}, L"x.y.z.w", 0xSTAMP }` entry to `kSupportedVersions[]` in `src/tesmiolauncher.cpp`.
3. Update `font` / `panel_builder_rva` defaults in `plugins/ui_panels/ui_panels.ini` and `plugins/private_construction/private_construction.ini`, then run `build.bat all`.
