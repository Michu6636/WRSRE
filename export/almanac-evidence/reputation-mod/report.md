# Reputation mod (v2.0) static investigation — Almanac evidence

**Source:** `C:\WRSRE\Mods for analysis\Diplomacy\` (workshop item 3797587125, "WRSR Reputation - diplomacy, espionage and the people")
**Method:** Static only. No installer, script, DLL, or game was executed; no debugger, hook, or game-file modification. Ghidra was used read-only (headless import + analysis of the DLL in a separate scratch project).

## 1. Inventory

| Item | Path | Size | SHA-256 | Notes |
|------|------|------|---------|-------|
| Mod DLL | `tesmioloader/build/plugins/reputation.dll` | 825,344 bytes | `FD7D94590B98C7EB80FA14EF95F2BBDF4B5E058F68D07DFA97D0DB8DCBFBCB6A` | Only DLL; Ghidra MD5 `29c3a47765b8925b794447121a117236` |
| Plugin config | `tesmioloader/build/plugins/reputation.ini` | 421,069 bytes | — | All UI/hook/version settings; UTF-8 no BOM required |
| Resources config | `tesmioloader/build/plugins/resources.ini` | 22,641 bytes | — | Adds small_arms/large_arms/munitions resources |
| Example events | `tesmioloader/build/plugins/reputation.d/example-mod-events.ini` | 4,581 bytes | — | Per-mod add-on directory |
| Icons | `tesmioloader/vfs/media_soviet/resources/{small_arms,large_arms,munitions}.png` | 149/193/201 B | — | 96x96 sidebar/build-tab icons expected at `media_soviet/editor/maintab_reputation.png` and `bottomtab_reputation.png` |
| Docs | `README.txt`, `workshopconfig.ini`, `ADD-TO-resources.ini.txt` | — | — | Read as text only |

**No separate loader binary ships.** `reputation.dll` is a TesmioLoader plugin loaded by `tesmiolauncher.exe` (not Steam's Play button). It depends on the `resources` plugin (`resources.dll`) and on `SOVIET64.exe` 1.1.1.9.

## 2. reputation.dll PE summary

- **Arch:** PE32+ x64 DLL, image base `0x180000000`, timestamp `0x6ab18b03`, characteristics `0x2022` (DLL).
- **Entry point rva:** `0x6a4b0` (VA `0x18006a4b0`).
- **Exports (TesmioLoader plugin API):** `TsmPluginApiVersion`, `TsmPluginInit` (VA `0x180068be0`, size 829), `TsmPluginStart` (VA `0x180068bf0`, size 2815).
- **Imports:** only `USER32.dll` (GetAsyncKeyState, hook/proc enumeration, window metrics) and `KERNEL32.dll` (heap, file, thread, VirtualProtect, GetProcAddress). **No SOVIET64.exe imports** — the mod calls game code at fixed absolute addresses and reads game memory at fixed RVAs.
- **Sections:** `.text` `.rdata` `.data` `.pdata` `.fptable` `.reloc`.

Full PE/string dump: `reputation_peinfo.txt` (same folder). Function list: `rep_funcs.txt` (1154 functions).

## 3. How the mod adds its sidebar tab — verified instructions

Analyzed in Ghidra project `C:\WRSRE\scratch\ghidra_proj_reputation3\reputation_dll`. `TsmPluginStart` disassembled at `rep_TsmPluginStart.asm` (691 instructions, VA `0x180068bf0`–`0x180069c40`). Key verified code:

**(a) Build-stamp version gate** (`rep_TsmPluginStart.asm` lines 33–68, VA `0x180068fd3`–`0x180069064`):
- Reads the SOVIET64.exe module base (`[0x1800c4d80]`), validates `MZ` then `PE` signatures.
- Reads the PE `TimeDateStamp` (PE header + 8) and `CMP`s it against `EDI`, which is loaded from DLL global `0x1801a15c0` — the configured `[sites] build_stamp`.
- On mismatch it logs "this is not the build the addresses were read off. Declined" and aborts UI setup.
- **Verified:** the mod enforces SOVIET64.exe 1.1.1.9 (timestamp `0x6A3EB6AD`) before installing any hook.

**(b) Panel-rectangle hooks** (`rep_TsmPluginStart.asm` lines 269–272, VA `0x18006944e`–`0x180069462`):
- `MOV EDX,0x31e478` / `MOV R8D,0x31e509` → hook targets at `SOVIET64.exe+0x31E478` and `+0x31E509` (the `SetDimensions`/`SetPosition` callsites the config names as the panel-rectangle hooks). A second hook at `exe+0x31F03C` appears at line 433 (`MOV EDX,0x31f03c`).

**(c) Panel-title string swap** (`rep_TsmPluginStart.asm` lines 212–216, VA `0x180069317`–`0x18006932f`):
- `CMP dword ptr [0x1801a3fdc],0x187a` then `MOV EDX,0x187a` / `LEA RCX,[0x18008ef40]` ("Diplomacy") — overrides the GetString result so id `0x187A` ("Secret police") renders as "Diplomacy" for the panel title.

**(d) Callback registration** — repeated `CALL qword ptr [RAX+0x48]` / `[RAX+0x38]` through the TesmioLoader API pointer table (`[0x1800c4d70]`), registering helper functions (e.g. `0x180067fa0`, `0x180068090`, `0x180068140`, `0x180067bf0`, `0x180067da0`, `0x180067f10`) and strings (`0x18008e958` format). These helpers implement the sidebar clone, tab append, click handling, and in-panel drawing (not individually disassembled — see unknowns below).

From `reputation.ini` (configuration-level, corroborated by the disassembly strings above):

- **Sidebar entry** (`sidebar = 1`): a button in the LEFT column beside Settings/Population/Economy. The entry is CLONED from one of the game's own 32 sidebar records (eight fields; three undecoded values byte-identical across all entries; donor has an empty sub-list), then only the name, string-id and icon are changed. Icon: `media_soviet/editor/maintab_reputation.png`, 96x96 — the game reads its texture pointer **without a null check** (missing file = crash).
- **sidebar_hides_panel = 1**: press-to-open / press-to-close push-button; clears the sidebar selection so the game draws no empty panel; described as "the ONLY place this plugin writes to a game global; guarded and switches itself off on a fault."
- **in_panel = 1**: draws the mod's windows INSIDE the game's sidebar panel. Panel rect read off `g_ui`: `+0xAE50` top (pixels), `+0xAE54` right edge (design units), `+0xAE58` bottom (pixels); the plugin detours `SetDimensions`/`SetPosition` and reads the real rectangle off the two calls the game draws the background with. `in_panel_w = 1280` positions the panel flush with the sidebar column.
- **Bottom build-bar tab** (`tab_enable = 1`, `tab_name = reputation`): appends to the tab vector; the last slot (`editor_civ`) is never drawn, so the mod swaps back one place (`tab_swap_last = 1`). Icon `editor/bottomtab_reputation.png`; `tab_string_id` registered via the engine's `CustomStringAdd` so tooltips read too.
- **Sidebar item inside Economy & Trade** (`sidebar_item = 0`, OFF by default): pushes an item into the `trade` panel's item vector; the game itself draws, highlights and clicks it (icon from `item+0x48`, click writes item index to a global). Config warns: do NOT use `system` (hardcodes count of five, would not draw a ninth item).
- **Sub-tabs / click:** a strip of labels (Standing, Contracts, Blueprints CMEA/NATO, Cheats, Debug) hit-tested by the mod itself using the mouse from `C3D_INPUT` and the left-press byte at `C3D_INPUT+0x301`; the same byte is consumed as a click shield so it does not reach the world.
- **[sites] hook targets:** `game_new = 0x84DECC`, `notification_new = 0x4C4880`, `notification_push = 0x4C49E0`; `notification_gate = 0x13178`, `notification_list = 0xD1F0` (offsets inside the world object). `language = 0x997590`, `language_custom_strings = 0x26720`, `font_manager = 0x996FB0`, `font = 0x995220`, `resource_vector = 0x9E11C0`, `dpi = 0x992088`, `world = 0x9D4F10`.

## 4. Dependencies and version-gating

- Tied to `SOVIET64.exe` **1.1.1.9 DX11 x64** (PE timestamp `0x6A3EB6AD`); the build-stamp check in `TsmPluginStart` aborts on any other build.
- Requires TesmioLoader (`tesmiolauncher.exe`) and the `resources` plugin (`resources.dll`).
- All hook addresses (`exe+0x31E478`, `+0x31E509`, `+0x31F03C`) and `g_ui` offsets are fixed to that one build.

## 5. Relevance to the WRSRE global Almanac

This is the **closest useful analogue yet** to the Almanac goal: a new button alongside the existing left-sidebar entries (Settings / Population / Economy & Trade), opening a panel whose content the mod draws itself, with push-button open/close and a GetString label override — all version-gated to a single build.

Transferable design ideas (verified where marked):
- Sidebar button cloned from a real entry (only name/string-id/icon changed) — preserves the three undecoded fields byte-identical across all 32 entries. **(config claim; clone mechanics not yet traced in disassembly)**
- Push-button open/close by clearing the sidebar selection, so no empty native panel appears; **ONLY game-global write**, guarded. **(config claim)**
- Content drawn inside the game's panel via `in_panel`, reading the real rect from `SetDimensions`/`SetPosition` calls (`g_ui +0xAE50/+0xAE54/+0xAE58`). **(partially verified: hook targets `exe+0x31E478/+0x31E509` confirmed in disassembly)**
- `GetString` panel-title override (id `0x187A` -> "Diplomacy"). **(verified in disassembly)**
- Version gate via PE TimeDateStamp comparison. **(verified in disassembly)**

Key distinction (important): the Reputation mod's sidebar button sits in the **left column**, not among the **eight visible Economy & Trade icons in the same row**. Its optional `sidebar_item` path puts an item inside the Economy & Trade panel (config `sidebar_item = 0`, OFF), where the game draws items generically ("one button each, icon from item+0x48") — but that door is shipped closed and is described as "a SECOND door into the same room ... clutter in a window that is not ours." Do not assume the Almanac button uses that exact row.

## 6. What remains unknown / limits

- The sidebar **vector location and record layout** (8 fields, 3 undecoded constants) — not independently verified in the binary.
- The **helper functions** registered via `[RAX+0x48]` (e.g. `0x180067fa0`, `0x180068090`, `0x180068140`, `0x180067bf0`, `0x180067da0`, `0x180067f10`) that implement the sidebar clone, tab append, click detection, and in-panel drawing were **not disassembled**.
- Exact **click detection** for the sidebar button: `C3D_INPUT+0x301` is a config claim; not yet traced in disassembly.
- Whether the eight Economy & Trade icons use the same vector/mechanism as the left sidebar (config asserts they do, uncorroborated in the DLL).
- Decompilation via headless Ghidra script was **unavailable**: `DecompAt.java`/`DecompAt2.java` fail to compile (`ghidra.app.decompiler.DecompileInterface` not on the headless script classpath); `ListFuncs.java` and `DisasmMany.java` work only when combined with `-import` into a fresh project (re-import into an already-imported project conflicts). Only disassembly was captured, not decompilation.

## 7. Evidence files preserved (non-ignored; DLL itself NOT copied)

Folder: `C:\WRSRE\export\almanac-evidence\reputation-mod\`
- `report.md` — this report.
- `reputation_peinfo.txt` — full PE header, sections, exports, imports, image base, and interesting-string excerpts with RVAs.
- `rep_funcs.txt` — Ghidra function list (1154 functions) for `reputation.dll`.
- `rep_TsmPluginStart.asm` — verified instruction-level disassembly of `TsmPluginStart` (VA `0x180068bf0`–`0x180069c40`), including build-stamp check, hook targets `0x31e478`/`0x31e509`/`0x31f03c`, and title-string swap.
- The DLL binary was **not** copied into the repository (per the `*.dll` ignore rule and redistribution caution); only hashes and small excerpts are preserved.

## Bottom line

The Reputation mod demonstrates, at both config and verified-instruction level, the exact pattern the Almanac needs: a new left-sidebar button beside Economy & Trade, opening a panel the mod fills itself, push-button open/close, GetString label override, and a PE-timestamp version gate — all on SOVIET64.exe 1.1.1.9 via fixed hook addresses (`exe+0x31E478/+0x31E509`) and `g_ui` panel-rect offsets (`+0xAE50/+0xAE54/+0xAE58`). It is directly relevant evidence. Remaining work: confirm the sidebar vector/record layout, click detection, and whether the eight Economy & Trade icons share that vector; those were not reached in this static pass.
