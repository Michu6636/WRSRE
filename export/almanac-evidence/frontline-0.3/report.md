# Frontline mod (v0.3) static investigation — Almanac evidence

**Source:** `C:\Users\EricM\Downloads\Other mods to reverse engineer\Frontline, a Soviet Republic combat mod`
**Method:** Static only. No installer, script, DLL, or game was executed; no debugger, hook, or game-file modification. Ghidra was used read-only (headless import + analysis of the DLL in a separate scratch project).

## 1. Inventory

| Item | Path | Size | SHA-256 | Notes |
|------|------|------|---------|-------|
| Mod DLL | `dinput8.dll` | 4,635,648 bytes | `A8C9265D41F85E4DB854B14EC71B91919756640C691991717A6AFB9BA0383620` | Matches `BUILD.txt`. Only DLL in the package. |
| Installer | `install.bat` / `tools/install.ps1` | small | — | Copies `dinput8.dll` into the game folder (next to `SOVIET64.exe`), copies `data\*`, `sounds\`, `music\` into `SRWar\`, runs save migration. |
| Uninstaller | `uninstall.bat` | small | — | Calls `install.ps1 -Uninstall`. |
| Workshop config | `workshopconfig.ini` | small | — | Steam item ID `3835680832`, type `WORKSHOP_ITEMTYPE_SCRIPT`. |
| Descriptions | `descEng.txt`, `descRu.txt` | small | — | "turns a peaceful city builder into a wargame"; open beta. |
| Build manifest | `BUILD.txt` | 2 lines | — | `Frontline 0.3; public; SRWAR_PUBLIC=1`, DLL SHA-256 listed above. |
| Data | `data\defence.ini`, `data\units.ini`, `data\eras.ini`, `data\lang_en.txt`, `data\migration_0_3.json`, `data\defence_tab*.png` | various | — | Building catalogue, unit catalogue, eras, translations, migration table. |
| Build tools | `tools\{install,preflight,paths,language,migration,fl,defence,restore_migration}.ps1` | various | — | PowerShell; read as text only. |
| Build artefacts | `previewimage.png`, `log.html` | small | — | `log.html` is nearly empty (`setfocus`). |

**No separate loader binary ships in the package.** `dinput8.dll` is both the loader (a `dinput8` COM-proxy DLL sideloaded next to `SOVIET64.exe`) and the mod code; there is no distinct loader DLL to distinguish. `tools/*.ps1` are installer/build helpers, not game code.

## 2. dinput8.dll PE summary

- **Arch:** PE32+ x64 DLL, image base `0x180000000`, timestamp `0x6af1d90f`, characteristics `0x2022` (DLL).
- **Sections:** `.text` `.rdata` `.data` `.pdata` `.idata` `.tls` `.00cfg` `.reloc`.
- **Exports (3):** `DirectInput8Create` (ord 0, addr rva `0x00000000` — proxy stub/forwarder), `DllCanUnloadNow` (ord 1, rva `0x00005592` → VA `0x180005592`), `DllGetClassObject` (ord 2, rva `0x000137c8` → VA `0x1800137c8`).
- **Imports:** `USER32.dll` (windowing/input/DPI), `d3d11.dll` (`D3D11CreateDeviceAndSwapChain`), `ole32.dll` (COM), `KERNEL32.dll` (`GetProcAddress`, heap, file), `SHELL32.dll` (`ShellExecuteW`), `IMM32.dll`, `D3DCOMPILER_47.dll` (`D3DCompile`), `GDI32.dll`, `dwmapi.dll`.

Full PE/string dump: `dinput8_peinfo.txt` (same folder). MD5 of the DLL per Ghidra: `7617ec2a807fbc55c15f1989037ed38e`.

## 3. What the mod demonstrably does (evidence-backed)

Raw string excerpts from `dinput8.dll` (RVA = file-relative virtual address; add image base `0x180000000` for VA):

**Defence build tab (build menu, NOT the Economy & Trade sidebar):**
- `0x392988` "buildmenu: no srwar_* building types loaded, no defence tab"
- `0x3929d0` "buildmenu: tab %s added as #%zu with %d buildings"
- `0x392a40` "buildmenu: GetString not hooked, the tab shows no label"
- `0x392a90` "buildmenu: menu builder call not found (other game version), no defence tab"
- `0x392af0` "buildmenu: no memory near the exe for the stub"
- `0x392ba0` "buildmenu: menu builder call redirected"

Interpretation: the mod adds a "Defence" tab to the game's build menu by (a) locating a native "menu builder" call site, (b) redirecting it via an inline hook with a stub/code-cave allocated in memory near the executable image, and (c) hooking `GetString` so the tab label resolves. The tab only appears when the menu-builder call site is found — i.e. it is **game-version-specific**.

**UI rendering (Dear ImGui):**
- `third_party\imgui\imgui.h/.cpp`, `imgui_impl_dx11.cpp`, `imgui_impl_win32.cpp` paths present → the mod draws its own UI with Dear ImGui over DX11/Win32.
- `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` (`0x39ad78`), `?SetColor@C3D_PANEL2D@@...` (`0x392be8`), `?Collision2DBBox@C3D_BUILDING@@...` (`0x391c80`) → uses the same `C3D_PANEL2D` / `C3D_BUILDING` native control machinery referenced in the WRSRE Almanac findings.

**Loader / render hooking:**
- Exports `DirectInput8Create`/`DllGetClassObject`/`DllCanUnloadNow` (classic `dinput8` COM-proxy sideload).
- Imports `D3D11CreateDeviceAndSwapChain` and `D3DCompile` → DX11 device/swap-chain hooking (render-path interception).
- `0x39f4a8` "Frontline active" / `0x39f4c0` "Frontline " / `0x39f4f0` "menu: Frontline %s active indicator drawn" → main-menu "Frontline active" indicator.
- `0x397458` "debug button", `0x39f230` "debug panel: %d" → debug tools (Ctrl+Shift+D per description).

**Gameplay / economy:**
- `0x39d560` "trade with the West cut for the war", `0x39d590` "trade with the West restored", `0x39da28`/`0x39da38`/`0x39da50` "trade_cut" / "trade_factor" → the mod cuts/restores trade with the West (economy gameplay, not UI).

**Activation / navigation (per description, not binary):**
- Pressing `F8` switches between managing the republic and commanding the army; main menu shows "Frontline active" + version. (The `F8` key itself was not observed as a standalone string in the DLL.)

**Save migration (installer, PowerShell):**
- `tools/install.ps1` + `tools/migration.ps1` migrate `srwar.ini` / `vehicles.bin` saves, require `SOVIET64.exe` SHA-256 `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` (the supported 1.1.1.9 DX11 x64 build), back up saves to `SRWar\migration_backups`.

**Build provenance (string):** `0x3e10b4` `C:\SR_Frontline\mod\build\dinput8.pdb`.

## 4. Relevance to the WRSRE global Almanac

**Transferable design ideas (demonstrated by this mod, version 0.3, on SR 1.1.1.9):**
1. A mod can add its own button/tab to an existing native menu row by hooking the native menu-builder call and redirecting it through an inline hook + stub/code-cave allocated near the executable image.
2. The tab label can be resolved by hooking the native `GetString`.
3. The mod's own UI can be rendered with Dear ImGui (`imgui` + `imgui_impl_dx11` + `imgui_impl_win32`) hooked into the DX11 render path.
4. The same `C3D_PANEL2D` / `C3D_BUILDING` native panel machinery the Almanac docs discuss is already used by a shipping mod for its own UI.

**What does NOT transfer / key distinction (important):**
- The Frontline "Defence" category is a **build-menu tab**, not a button in the **Economy & Trade sidebar row** that the Almanac targets. The mod hooks the build-menu builder; nothing in this package shows how to place a button in the sidebar row, how the sidebar's input routing/clipping/selection behaves, or how to replace/suppress the sidebar body. Do **not** assume the Defence category uses the same sidebar row as the intended Almanac button (per the investigation constraint).
- The Almanac needs a global page that opens without selecting a building and returns to native sidebar content predictably; Frontline's model is a build tab + an F8 game-mode switch, which is a different interaction model.

## 5. Dependencies and version-gating

- Tied to `SOVIET64.exe` **1.1.1.9 DX11 x64** (installer preflight verifies the exe SHA-256; aborts otherwise).
- The menu-builder call site is version-specific: "menu builder call not found (other game version), no defence tab" — the hook address is not stable across builds.
- The `dinput8` proxy-loader and DX11 swap-chain hook depend on the game loading `dinput8.dll` and using DX11.
- GetString hooking and the stub/cave allocation depend on the native binary layout at the chosen call site.

## 6. What remains unknown / limits

- Exact hook call-site address, stolen-instruction span, trampoline/stub layout, and instruction boundaries were **not** recovered. Ghidra imported and analyzed `dinput8.dll` successfully in project `C:\WRSRE\scratch\ghidra_proj_frontline\frontline_dll` (program `/dinput8.dll`, image base `0x180000000`), but the `DecompAt.java` decompile post-script could not resolve the Ghidra decompiler API classes in this headless "no program context" invocation (script-classpath environment issue). Instruction-level tracing of the hook path remains as future work requiring a working Ghidra decompile script or GUI session on that project.
- Whether the Economy & Trade sidebar row uses the same "menu builder" call site as the build menu — unproven; likely different UI code path.
- Input routing, clipping, selection state, and clean return-to-native behavior for a mod-added button in the sidebar row — not addressed by this package.
- `F8` key handling was described in the docs but not confirmed as a binary string.

## 7. Evidence files preserved (non-ignored, shareable; DLL itself NOT copied)

Folder: `C:\WRSRE\export\almanac-evidence\frontline-0.3\`
- `report.md` — this report.
- `dinput8_peinfo.txt` — full PE header, sections, exports, imports, and interesting-string excerpts with RVAs.
- The DLL binary itself was **not** copied into the repository (per the `*.dll` ignore rule and redistribution caution); only hashes and small string excerpts are preserved. A Ghidra decompile of `DllGetClassObject` was attempted but the `DecompAt.java` script could not resolve the Ghidra decompiler API classes in this headless invocation, so no decompile output file was produced.

## Bottom line

Frontline demonstrates that a mod can inject its own tab/button into a native SR menu row via an inline-hooked menu-builder call + stub cave, resolve labels via `GetString`, and render its own UI with Dear ImGui over DX11 — all version-gated to SR 1.1.1.9 and delivered through a `dinput8` proxy DLL. That is useful **design evidence** for the Almanac's same-row button, but the Defence tab lives in the build menu, not the Economy & Trade sidebar, so the sidebar-specific integration questions (placement in the row, input/clipping, content suppression, return-to-native) remain open and must be verified independently for WRSRE.
