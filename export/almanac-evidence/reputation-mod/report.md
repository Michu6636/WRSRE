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

## 6. The sidebar_item path (Economy & Trade row item) — traced

**Config** (`reputation.ini` lines 1628–1631):
- `sidebar_item = 0` (OFF by default; shipped off because it is "a SECOND door into the same room" — the mod already has its own left-sidebar button).
- `sidebar_item_panel = trade`, `sidebar_item_name = reputation`. Requires `sidebar = 1`.
- **Config claim (not runtime-observed):** "the tab appears in Economy and trade, the game draws and highlights it, and clicking it opens our window."
- **Config claim (texture):** icon `media_soviet/editor/trade_reputation.png`, 96x96; renderer binds the texture **without a null check** → missing file crashes; item taken back out if texture does not arrive.
- **Config claim (vector):** "It also writes into a game vector, which should be a deliberate choice."

**Insertion** (verified instructions, `TsmPluginStart`, `rep_TsmPluginStart.asm`):
- Reads DLL global `0x1800c3090` (the `sidebar_item` enabled flag). If clear, skips the whole path.
- Looks up/creates the item through the TesmioLoader API (`[RAX+0x40]`, handler `0x18008e968`, name `0x18008e958`) and stores the item's function pointer at DLL global `0x1800d9498`.
- Registers four mod callbacks into four item-vtable function-pointer slots via `[RAX+0x48]`:
  - `FUN_180068a20` → `[0x180187dd0]` (item create/draw; calls `[0x180187dd0]` then `0x1800303e0`)
  - `FUN_180068a80` → `[0x180187dc8]` (item draw/label; calls `[0x180187dc8]` then `0x1800303e0`)
  - `FUN_180068ae0` → `[0x180187de0]` (is-selected; returns nonzero else `0x1800303e0`)
  - `FUN_180068b40` → `[0x180187dd8]` (open/activate; returns nonzero else `0x1800303e0`)
- Verified disassembly: `rep_sidebar_item_asm.asm` (98 instructions, VA `0x180068a20`–`0x180068bd9`).

**Click / selection detection** (verified instructions, fallback `FUN_1800303e0`, `rep_fallback_303e0.asm`, 210 instructions VA `0x1800303e0`–`0x180030710`):
- Entry `(RCX=item name/path, RDX=context)`. First checks `sidebar_item` flag `0x1800c3090` — if 0, returns without handling (so the path is fully gated on the config flag).
- Scans the item name for `\`/`/` path separators to extract the basename, then compares it against known strings via strcmp-like `0x18006daf0` (`0x180188090`, `0x180090790`, `0x1800907a8`).
- On match with `0x180090790`: calls `0x180064790` (panel manager), then `0x180007ed0` with `EDX=1` (open/switch panel).
- On match with `0x1800907a8`: stores `EAX` into selection-index global `0x1801a3f90`, calls `0x180064790`, then `0x180007fd0`.
- Length guard: names longer than `0x102` (258) bytes are rejected.
- Falls through to cleanup logging via `[0x18008d0c0]`.

**Opening the panel:** `0x180064790` → panel manager → `0x180007ed0`/`0x180007fd0` (open/switch functions). The mod does not reimplement the row renderer; it pushes an item into the game's `trade` vector and notices the click.

**Close / return** (verified instructions, `FUN_180068ba0`, `rep_sidebar_item_asm.asm` lines 86–99):
- When `EDX==0` (item not selected / close): resets selection flag `0x1800c3e80 = 0`, XCHGs selection atom `0x180448f5c`, swaps panel pointer `0x180448f30` (old → RAX, new → RDX), and if the old panel is non-null calls virtual `[0x18008d2e8]` on it (cleanup/release). Returns 1 (handled).

**Native Economy & Trade content when the item is selected:**
- The game's generic renderer draws items from the vector (one button each, icon from `item+0x48`), highlights on hover, and writes the clicked item INDEX to a global. The mod only notices and opens its own window — it does not suppress or replace native content.
- The mod's separate left-sidebar section (`sidebar = 1`) is a distinct button; the `sidebar_item` row item is explicitly "clutter in a window that is not ours" and is shipped disabled.

**Comparison with WRSRE trade-handler evidence** (`docs/almanac-investigation.md`, `docs/ghidra-index/373EA0.md`, `docs/almanac-export-review.md`):
- WRSRE row drawer `FUN_140373EA0` reads `game+0xC650` (selected panel entry), walks its sub-entry vector (stride `0x50`, begin/end at `[P+0x48]`/`[P+0x50]`), draws each sub-entry's texture at `sub_entry+0x48` with a 64x64 backdrop, tracks selected sub-tab in `game+0xC65C`. Tile positions written at `0x14037423c`–`0x14037424f`; `0x140445eb0` receives `game+0xADB0`.
- WRSRE selects between **two** trade handlers based on a game-state flag; `FUN_140358A20` (importexport) is effectively dead because no panel is named "importexport".
- Reputation's `+0x48` icon offset matches WRSRE's `sub_entry+0x48` texture offset — both reuse the game's generic row rendering. Reputation pushes into the game vector; WRSRE's row is built by `FUN_140296110` from a data table (`game+0xC2B0`, stride `0x340`).
- WRSRE unknowns that apply here too: exact panel width/height/clip write location (candidate: `game+0xADB0`), hit-test bounds vs visual clipping (separate unknowns), runtime count/visibility unverified.

**Risks relevant to a safe WRSRE design:**
- **Texture null-check crash:** missing icon file crashes rather than draws blank (config claim; uncorroborated in binary).
- **Vector ownership:** mod writes into the game's `trade` vector — if the vector reallocates or the mod unloads, dangling pointers; no cleanup path verified in the DLL for the `sidebar_item` vector write beyond `FUN_180068ba0` panel swap.
- **Handler ambiguity:** WRSRE's two trade handlers mean the same-row button must work under whichever handler is active; `FUN_140358A20` is dead but `FUN_140373EA0` is the verified row drawer.
- **Close/return:** `FUN_180068ba0` swaps panel pointer and calls a virtual cleanup — but whether native panel state is fully restored is unverified.

## 7. What remains unknown / limits

- Whether the `sidebar_item` path actually renders a ninth Economy & Trade icon at runtime (vector capacity / `FUN_140296110` data-table contents not traced).
- Whether both WRSRE trade handlers can display and select the item, or only `FUN_140373EA0` (the verified row drawer).
- Exact instruction(s) that write the panel width/height/clip fields (`game+0xADB0` candidate; hit-test bounds vs visual clipping are separate unknowns).
- Whether `FUN_1800303e0` fallback is reached at runtime, or the mod's registered vtable handlers handle the click first.
- Whether native panel state is fully restored on close (`FUN_180068ba0` cleanup unverified).
- In-game read-only snapshot of trade row geometry and selection state is still needed (not authorized to execute).
- Headless Ghidra decompilation remains unavailable (`DecompileInterface` not on the script classpath); only disassembly captured.

## 8. Evidence files preserved (non-ignored; DLL itself NOT copied)

Folder: `C:\WRSRE\export\almanac-evidence\reputation-mod\`
- `report.md` — this report.
- `reputation_peinfo.txt` — full PE header, sections, exports, imports, image base, and interesting-string excerpts with RVAs.
- `rep_funcs.txt` — Ghidra function list (1154 functions) for `reputation.dll`.
- `rep_TsmPluginStart.asm` — verified instruction-level disassembly of `TsmPluginStart` (VA `0x180068bf0`–`0x180069c40`), including build-stamp check, hook targets `0x31e478`/`0x31e509`/`0x31f03c`, title-string swap, and the `sidebar_item` callback registration block.
- `rep_sidebar_item_asm.asm` — verified disassembly of the four `sidebar_item` vtable callbacks and close handler (VA `0x180068a20`–`0x180068bd9`).
- `rep_fallback_303e0.asm` — verified disassembly of the click/selection/panel-open fallback `FUN_1800303e0` (VA `0x1800303e0`–`0x180030710`).
- The DLL binary was **not** copied into the repository (per the `*.dll` ignore rule and redistribution caution); only hashes and small excerpts are preserved.

## Bottom line

The Reputation mod demonstrates, at both config and verified-instruction level, two patterns relevant to the Almanac:

1. **Left-sidebar button** (`sidebar = 1`): a new button beside Economy & Trade opening a self-drawn panel, push-button open/close, GetString label override (`0x187A` → "Diplomacy"), PE-timestamp version gate — verified in `TsmPluginStart` disassembly (`rep_TsmPluginStart.asm`) on SOVIET64.exe 1.1.1.9 via fixed hook addresses (`exe+0x31E478/+0x31E509`) and `g_ui` panel-rect offsets (`+0xAE50/+0xAE54/+0xAE58`).

2. **Same-row Economy & Trade item** (`sidebar_item = 1`, shipped OFF): the mod pushes an item into the game's `trade` panel vector; the game's generic renderer draws it (icon at `item+0x48`) and highlights/clicks it; the mod intercepts via four item-vtable callbacks (`FUN_180068a20/a80/ae0/b40` → slots `[0x180187dd0/dc8/de0/dd8]`) with `FUN_1800303e0` as the click/selection/panel-open fallback, and `FUN_180068ba0` as the close/return handler that swaps the panel pointer and calls virtual cleanup.

This is directly relevant evidence for a ninth same-row button, but the `sidebar_item` path is shipped disabled, texture loading lacks a null check (crash risk), vector-ownership cleanup is unverified, and WRSRE's two trade handlers (only `FUN_140373EA0` verified as the row drawer) mean same-row placement must work under the active handler. The sidebar button and the same-row item are two separate doors; the Almanac must pick one and verify the other independently. Remaining work: runtime read-only snapshot of trade row geometry/selection, clip/write-location confirmation, and whether both trade handlers can select the item — none authorized to execute.
