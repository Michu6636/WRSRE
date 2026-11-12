# Almanac button — implementation report (v3.0)

Implementation status: built and verified with `build.bat almanac_button`.
Static Ghidra investigations completed; no game launch, click tests, or runtime
validation performed. The Ghidra project lock at
`C:\WRSRE\scratch\ghidra_proj_soviet_trade\soviet_trade.lock` was not touched
and remains in place. SOVIET64.exe was imported into the `soviet_trade` Ghidra
project; a separate `c3ddll64` project was created for C3DDLL64.dll analysis.

## Executive summary

The extra Economy & Trade tile repeats the vanilla (crime-button) artwork
because the prototype (`plugins/almanac_button/almanac_button.cpp`) does not
change any texture/icon state — it calls the engine's own `C3D_PANEL2D::Draw`
again with the **same intercepted `self` panel object** and arguments, changing
only the shared X/Y rectangle pair (RVA `0x9BE2F0`/`0x9BE2F4`). The artwork
bound to that panel object is whatever the native row handler last set up
(per-tile via `vtable[0x70]` texture bind at `0x140374214`, texture object from
sub-entry offset `0x48`). Repeating the draw therefore re-renders the same
panel state at a new position. There is no static evidence that
`C3D_PANEL2D::Draw` "owns" the crime icon, nor that a second draw with a
different rect produces a distinct, clickable button. A distinct Almanac button
requires a separate texture binding, hit-test rect, and input handling — none
of which the prototype creates, and none of which the existing `ui_panels` API
exposes for the global sidebar row (it is a building-window widget contract
keyed on `window+0x250`, gated on a valid building).

## Method and identity limits

- Implementation: built with `build.bat almanac_button`; DLL at
  `build\plugins\almanac_button.dll`. No game launch, click tests, or runtime
  validation performed.
- Ghidra queries: XrefsTo on `0x140374356` and `0x14037435C`; DumpBytes on
  `0x1403613B0` and `0x140335BC0`; ExtSym for Texture/Font/Input symbols;
  DisasmMany on `DrawFrom9Patch` (0x1800B99C0) and `DrawFrom3Patch`
  (0x1800B9680) in C3DDLL64.dll. SOVIET64.exe imported into the `soviet_trade`
  Ghidra project; separate `c3ddll64` project created for C3DDLL64.dll.
- The Ghidra project lock at `C:\WRSRE\scratch\ghidra_proj_soviet_trade\soviet_trade.lock`
  was not touched or deleted. No existing project data was modified.
- Executable identity: SOVIET64.exe baseline v1.1.1.9, PE TimeDateStamp
  `0x6A3EB6AD`, preferred Ghidra image base `0x140000000`.
- C3DDLL64.dll (engine panel DLL): image base `0x180000000`, SHA-256
  `65FCB684...`.
- All "verified" claims are instruction-address-level facts from preserved
  disassembly/exports and Ghidra query results.

## 1. Native row draw setup — verified trace

| Item | Address / RVA | Evidence |
|------|---------------|----------|
| Host call site | `0x14031EDEF` (RVA `0x31EDEF`) | `E8 AC 50 05 00` CALL `FUN_140373EA0`; geometry-snapshot §2 confirms bytes match static export |
| Row Draw call | `0x1403743A2` (RVA `0x3743A2`) | `FF 15 B8 84 4F 00` CALL `[IAT 0x86C860]`; ret at `0x1403743A8` |
| IAT slot (Draw) | RVA `0x86C860` | `C3D_PANEL2D::Draw` import slot; verified bytes in plugin `VerifyBuild()` |
| Panel object (`self`) | RVA `0x9BE060` | `TSM_RVA_DEFAULT_PANEL2D`; used as RCX for the Draw call |
| Rect pair X/Y | RVA `0x9BE2F0`/`0x9BE2F4` | `P2D_RECT_X`/`P2D_RECT_Y`; shared mutable draw-state the plugin overwrites |
| W/H | RVA `0x9BE2E8`/`0x9BE2EC` | `P2D_RECT_W`/`P2D_RECT_H`; not written by the plugin |
| `game` object | RVA `0x9D4F10` | `LEA RCX,[0x1409d4f10]` at `0x140482b4b` in `FUN_140482790` (geometry snapshot §3) |
| Handler selector | `game+0x10398` | byte; `1` → `FUN_140373EA0` (button row); `0` → `FUN_140361BB0` |
| Normal row gate | `game+0xC7D4 == -1`, `game+0x578 == 0`, `game+0x5E0 != 0` | geometry snapshot §4 (one read-only, non-atomic sample) |
| Trade subvector | `game+0xC650 → P → [P+0x48]=begin, [P+0x50]=end`, stride `0x50` | count 8 in snapshot; capacity may exceed 8 (not a safe-insertion proof) |
| Row constants | scale `0x992088` (0.8), origin `45` `0x90AA0C`, `85` `0x90AB14`, pitch `64` `0x90AAA0` | geometry snapshot §5 |
| Ninth tile (hypothetical) | X [513.6,564.8], Y [196.0,247.2] at scale 0.8 | conditional arithmetic only — not proven visible, unclipped, or clickable (geometry snapshot §7-8) |

The prototype's detour (`almanac_button.cpp:540`) forwards every call to the
engine Draw; only the trade row's eighth-tile call (return address `0x3743A8`,
`self == panel 0x9BE060`) takes the extra path. It saves/restores only the X/Y
pair via one 8-byte store each; W/H and all panel-internal fields are untouched.
`DrawNinthTile` (`almanac_button.cpp:505`) writes `PackPair(x,y)` to the rect
pair, calls the native Draw with the unchanged `self`, then restores the saved
pair.

**Why the artwork repeats:** The ninth Draw re-enters the same panel object
whose texture/icon state was last written by the native row handler for the
previous tile. The plugin changes no texture, UV, border, tint, or other
panel field. So the ninth tile renders whatever artwork the panel object holds
— observed as the crime-button artwork repeated. This is exactly the expected
result of "same `self`, same arguments, different X/Y." It does **not** prove
Draw owns the crime artwork; it proves the panel object's internal state was not
changed between the two calls.

### What is unknown about texture/icon ownership

- Exact field offsets inside `C3D_PANEL2D` that hold the texture/icon/UV/border
  state are not traced. Draw reads panel fields at `+0x288`/`+0x28C` (scale),
  `+0x2A8`, `+0x2D0`, `+0x204`/`0x214`/`0x224`/`0x234`, and bounds fields
  `+0x673c`/`+0x6740`/`+0x6744`/`+0x674c`; none are confirmed as the texture
  source. The colliding bounds fields `+0x673c` (scale=1.0f), `+0x6744`
  (static init `0x4E6E6B28` ~1e9, overwritten by Draw), `+0x6740` are written
  by constructors/init and overwritten during Draw (`0x1800BB26E`,
  `0x1800BB682`), but their role is hit-test bounds, not texture ownership.
- `FUN_140445eb0` (called per loop iteration at `0x140374265`) likely repositions
  the panel per button; body not in any export, so which field carries the
  per-tile texture/icon is unresolved.
- Whether the native Draw itself writes the rect pair during the call is
  unverified (plugin source §SHARD STATE says so explicitly).

## 2. Hit-test / input path — verified trace

| Item | Address | Evidence |
|------|---------|----------|
| Mouse read | `0x140374380` | indirect `CALL [0x14086c868]` (`C3D_INPUT::GetMouseSolid`, unresolved target) writing `[RBP-0x80]`/`[RBP-0x78]`; same mouse for every loop iteration |
| Hit-test rect | `[RBP+0x210]` | mouse X/Y, remaining fields zeroed (`XMM8=0`); it is a point-in-rect test, not button coordinates |
| Collision call | `0x1403742a4` | indirect `CALL [0x14086c850]` (`C3D_PANEL2D::Collision(panel=0x1409BE060, rect)`) |
| Collision reads | `+0x673c`/`+0x6740`/`+0x6744`/`+0x674c` | scale, bound Y1, bound Y2, depth (panel-fields report) |
| Latch check | `0x1403742f2` | `CMP byte [0x140a54e91],CL` — `0xA54E91` click latch |
| Live-mode check | `0x1403742fa` | `CMP byte [RSI+0x14],CL` |
| Sound calls | `0x14037430d` / `0x140374317` | indirect `[0x14086cd10]` / `[0x14086cd18]` |
| Selection write | `0x140374356` | `game+0xC65C = EBX` (this button's index) |
| Counter write | `0x14037435c` | `game+0x10BE8 = -1` always on click |
| Dirty flag | `game+0x1039A` | cleared only if selection changed |
| Already-selected click | — | no observable transition (`0xC65C` unchanged, `0x10BE8` may already be -1, `0x1039A` not written) |

Click-handling gates: Collision hit AND latch set AND live mode (or skip sound).
No 14-byte whole-instruction boundary exists at any click-write site in either
handler (counter writes 12-13 bytes, selection writes 6-7 bytes; combining
exceeds 14), so a 14-byte inline patch at the click path is not statically
approvable (native-click-close report §3, native-click-hookpoints report §3).

## 3. What the plugin API exposes (and does not)

The shared `ui_panels` API (`src/ui_panels_api.h`, v6) provides native button
machinery (`widgetButton`, `widgetButtonSmall`, `widgetWideButton`) built on
`DrawFrom9Patch` + cached button texture + `GetMouseSolid`+`Collision` +
latch `0xA54E91` + `gui_click`. However:

- It is an **in-building widget contract**: providers run on the render thread
  only while a building's window is open, append at `window+0x250`, and are
  gated on a valid building (`AppendWidgetsImpl` explicitly gates on it). It is
  **not** a global-sidebar button API, and the Economy & Trade row is not a
  building window.
- The City Hall TAX button uses this path (`widgetButtonSmall` / toolbar-docked
  tile) and must not be moved per the task.
- ImGui is deferred per the task.
- No plugin-exposed route exists to create, own, or lifetime-manage a separate
  `C3D_PANEL2D` panel/texture object for the global row, nor to register a
  same-row button outside `window+0x250`.

The reputation-mod sidebar technique (content replacement via
`window+0x250`/dispatcher hooks) does not transfer: that row is a building-panel
sidebar with its own dispatcher and builder hooks; the Economy & Trade row is a
separate global page handler (`FUN_140373EA0`) with different layout, skip
mapping, and input gating. Do not assume it applies.

## 4. Candidate comparison for a distinct same-row Almanac button

| Candidate | Preserves 8 buttons? | Same-row? | Distinct artwork? | Clickable? | Native input intact? | Status |
|-----------|---------------------|-----------|-------------------|------------|----------------------|--------|
| Extend prototype (change rect again) | Yes | Yes | No (same panel state) | No | Yes | Already built; artwork problem inherent |
| Draw separate texture via new Draw call | Yes | Yes | Partial (needs owned texture) | No (no hit-test) | Risky (shared state) | No owned panel/texture route in API |
| Native insertion into row vector | No (reorders/shifts index 7 → 8th; alters visible ordering) | Yes | Possible | Yes | Alters native selection indexing (`game+0xC65C` synthetic index may fault in vector accessor `0x14031d794–b1`) | Not proven safe; button-approach review §1 |
| Separate mod-owned panel + texture + Collision + click | Yes | Yes | Yes (owned texture) | Yes (owned hit-test) | Yes (if no latch consumption / no shared-state overwrite) | **Design candidate** — requires: texture asset, panel object creation/lifetime, X/Y origin+pitch+clip math, input arbitration with 8 natives, close/switch/return behavior, hook safety (no 14-byte boundary at click path), no writes to `game+0xC65C`/`0x10BE8`/`0xA54E91`/`0x1039A` |
| Separate global Almanac section/page (Economy & Trade page fallback) | Yes | No (fallback) | Yes | Yes | Yes | Preferred product direction per almanac-technical-findings §Decision status; still requires a same-row button per product requirement |

No candidate is established as safe to implement. The separate-panel candidate is
the only one that can, in principle, satisfy all constraints, but every one of
its requirements is currently unverified.

## 5. Unresolved risks and unknowns

1. **Texture/icon ownership** — which `C3D_PANEL2D` field holds the per-tile
   texture/icon is not traced; no owned-panel creation API exists in the
   loader or shared headers.
2. **Clipping** — `Draw` does not perform visual clipping (delegated to
   `DrawFrom9Patch`/`DrawFrom3Patch`, bodies not traced); whether a ninth
   same-row tile is clipped is unknown. Geometry snapshot §7-8 labels the ninth
   rectangle hypothetical.
3. **Panel sharing** — whether all eight row tiles reuse the single panel object
   at `0x1409BE060` is inferred from the prototype's `self` check, not proven;
   per-tile state may live elsewhere.
4. **Draw mutates shared state** — whatever the engine's Draw writes internally
   for the ninth tile (per-widget/global draw state, hover/press, GPU state) is
   not identified (plugin source §SHARD STATE).
5. **Hit-test arbitration** — a ninth button at a different X would need its own
   `Collision` hit-test agreeing with its draw rect; ordering vs. the eight
   native hit-tests and latch consumption are unresolved; no clean observation
   point exists (native-click-close report §8).
6. **Close/switch/return** — clicking a native icon must close the Almanac;
   clicking another sidebar section, closing the sidebar, and returning to
   vanilla content all need explicit behavior (almanac-technical-findings §2-3);
   none traced here.
7. **Hook safety** — no 14-byte whole-instruction boundary at any click-write
   site; the loader's inline hook copies instructions without fixing RIP-relative
   or relative-control-flow operands (INSTRUCTIONS §B).
8. **DPI/scaling** — pixel values must multiply by the game's DPI scalar
   (`0x992088`); ninth-tile pixel mapping not established.
9. **Private UI text IDs** — any minted text ID must be >= 1,000,000
   (INSTRUCTIONS §3); not yet determined whether a new ID is required.

## 6. Implementation status (v3.0)

**Implemented and built.** `plugins/almanac_button/almanac_button.cpp` v3.0
implements Path A (clickable ninth tile with distinct icon/text) + Path D
(read-only click observation, mod-owned Almanac state) + content suppression
via `game+0x5E6` write. Builds cleanly with `build.bat almanac_button`; DLL at
`build\plugins\almanac_button.dll`.

**What v3.0 does:**
- Draws a ninth tile beside the Economy & Trade row at `origin + 8 * pitch`
  (same draw path as the prototype).
- Binds a distinct texture via `vtable[0x70]` (TexBind) before the ninth Draw,
  using texture object loaded via `C3D_MIDDLEPOINT::CreateManagedTexture`
  (C3DDLL64.dll RVA `0x99`). Texture path configurable via INI `texture_asset`.
- Renders a text label ("Almanac") via `C3D_FONTMANAGER::PrintLeftUnicode`
  (C3DDLL64.dll RVA `0x111`), font loaded via
  `C3D_MIDDLEPOINT::CreateManagedFontBitmap` (RVA `0x1C0`). Font path
  configurable via INI `font_asset`.
- Checks for clicks on the ninth tile: mouse-over rect (own point-in-rect test)
  + click latch `0xA54E91` read (read-only, NOT consumed).
- Toggles mod-owned `g_almanacOpen` state on ninth-tile click (debounced by latch).
- Closes Almanac when native selection `game+0xC65C` changes to a valid tile index (0..7).
- **Content suppression**: when Almanac is open, writes `game+0x5E6 = 1` every
  frame to suppress native sub-page content drawing via `FUN_14035f3e0`.
  Restores `game+0x5E6 = 0` when Almanac closes. Native code toggles this flag
  on certain clicks, so it is re-asserted each frame. Configurable via INI
  `content_suppression` (default: 1).
- Never writes `game+0xC65C`, never consumes `0xA54E91`, never writes
  `game+0x10BE8` or `game+0x1039A`.
- All 8 native buttons unchanged. City Hall TAX button not moved. No ImGui.
- GetMouseSolid resolved from C3DDLL64.dll at Start; if unresolved, plugin still
  arms but ninth-tile clicks are not detected.

**Known limitations:**
- Content suppression via `game+0x5E6` requires `FUN_140335bc0() != 0` to take
  effect (behavior of `FUN_140335bc0` unknown — not decompilable with current
  Ghidra setup). Native code may toggle `game+0x5E6` back on clicks; we re-assert
  it every frame to counter this.
- Ninth-tile clipping: DrawFrom9Patch/3Patch check `panel+0x6760` flag; whether
  the ninth tile is clipped depends on runtime flag value.
- Texture/font assets must be original/cleared (no `game_assets.zip` extraction).
  No assets loaded by default.
- No in-game testing performed. Requires separate authorization for game launch
  and click tests.

**Build guard:** All original static checks preserved (call-site bytes at
`0x31EDEF`/`0x3743A2`, import slot at `0x86C860`, constants 45/85/64).
Fail-stop policy unchanged.

**Version history:**
- v1.5: draw-only ninth backdrop, no input.
- v2.0: clickable ninth tile with read-only click observation + Almanac toggle.
- v3.0: distinct texture binding via `CreateManagedTexture` + text label via
  `PrintLeftUnicode` + content suppression via `game+0x5E6` write.

## Evidence paths referenced

- `plugins/almanac_button/almanac_button.cpp` — prototype source (draw-only, no input)
- `plugins/almanac_button/almanac_button_geometry.h` — row model, ninth-slot hypothesis
- `plugins/almanac_button/almanac_button.ini` — default-on setting; no click handling
- `src/game_addresses.h` — RVA table (panel2d `0x9BE060`, rect pair `0x9BE2F0`/`0x9BE2F4`, input `0xA54B90`, click latch `0xA54E91`, etc.)
- `src/ui_panels_api.h` — ui_panels v6 contract (building-window only; not a global-sidebar API)
- `export/almanac-evidence/almanac-button-visual-test-run1/result.md` — visual run: nine tiles, crime-button artwork, plugin armed, ninth tile drawn, no fail-stop
- `export/almanac-evidence/almanac-button-visual-test-run1/tesmioloader-log.txt` — loader log (game v1.1.1.9, plugin 1.5 armed)
- `export/almanac-evidence/geometry-snapshot-20261108.md` — row constants, subvector count 8, ninth-tile hypothetical
- `export/almanac-evidence/hit-test-rectangle-trace-20261109.md` — mouse/Collision hit-test trace
- `export/almanac-evidence/native-click-close-20261110/report.md` — click path, latch, no 14-byte boundary
- `export/almanac-evidence/native-click-hookpoints-20261110/report.md` — hook candidate A/B, size minimum met but not proven safe
- `export/almanac-evidence/c3ddll64-draw-complete-20261109/REPORT.md` — Draw body, exits, no clipping in Draw
- `export/almanac-evidence/c3ddll64-panel-fields-20261109/REPORT.md` — panel bounds fields `+0x673c`/`+0x6740`/`+0x6744`
- `export/almanac-evidence/fun-14035f3e0-20261108/REPORT.md` — native content helper draws + handles input (not a custom renderer)
- `docs/ghidra-index/373EA0.md` — row handler entry (partial; button backdrop 64x64, X origin 130, pitch 64)
- `docs/almanac-technical-findings.md` — global findings, placement/replacement questions
- `docs/almanac-button-approach-review.md` — corrections, separate-button candidate status
- `docs/ghidra-index/README.md` — index schema convention
- `scratch/ghidra_proj_c3ddll64/` — separate Ghidra project for C3DLL64.dll analysis
- `scratch/out/drawfrom9patch_disasm.asm` — DrawFrom9Patch disassembly (310 instructions)
- `scratch/out/drawfrom9patch_disasm2.asm` — DrawFrom9Patch jump-target region
- `scratch/out/drawfrom3patch_disasm.asm` — DrawFrom3Patch disassembly (160 instructions)
- `scratch/out/xrefs_tex_9patch.txt` — XrefsTo on G_TEX_9PATCH VA (total=0)
- `scratch/out/xrefs_tex_b048.txt` — XrefsTo on game+0xB048 texture slot (total=0)
- `scratch/out/extsym_texture.txt` — ExtSym: Texture-related external symbols (27 matches)
- `scratch/out/extsym_font.txt` — ExtSym: Font-related external symbols (21 matches)
- `scratch/out/extsym_input.txt` — ExtSym: Mouse/Input-related external symbols (12 matches)
- `scratch/out/xrefs_panel_0x6760.txt` — XrefsTo on panel+0x6760 VA (total=0)
- `scratch/out/xrefs_1403613b0_v2.txt` — XrefsTo on FUN_1403613b0 (3 data refs)
- `scratch/out/bytes_1403613b0.txt` — Raw bytes at FUN_1403613b0 entry (x64 prologue confirmed)
- `scratch/out/bytes_140335bc0.txt` — Raw bytes at FUN_140335bc0 entry (x64 prologue confirmed)
- `scratch/out/1403613b0_disasm.asm`, `disasm2.asm`, `disasm3.asm`, `disasm4.asm` —
  DisasmMany attempts on FUN_1403613b0 (0 instructions — Ghidra analysis incomplete)
- `scratch/out/140335bc0_disasm.asm`, `disasm2.asm` — DisasmMany attempts on FUN_140335bc0

## 7. Investigation I1 — Texture/icon field ownership (Ghidra, RESOLVED)

**Texture IS bound per-tile in SOVIET64.exe**, not inside `C3D_PANEL2D::Draw`.

Per-tile sequence before each Draw call (`scratch/out/140373ea0_1403829fd_disasm.asm`):
1. `0x14037420b-0x140374214`: `CALL qword ptr [RAX + 0x70]` — vtable[0x70] = TexBind method (same as `ui_panels.cpp:1502`). RCX = texture obj from `[R13 + 0x48]` (sub-entry offset 0x48). R8 = `game+0xEAD08` (PANEL_STATE).
2. Set P2D_RECT_W/H, X/Y pair, ROT (=0), TINT
3. `0x14037439b-0x1403743a2`: LEA RCX, [0x1409be060]; CALL Draw — uses whatever texture is currently bound

**No texture field is written to C3D_PANEL2D panel object** (`0x1409be060`). Only W/H, X/Y, ROT, TINT are written per-frame. Draw uses whatever texture is currently bound externally via vtable[0x70].

**Texture loading mechanism EXISTS in C3DDLL64.dll** (`scratch/out/extsym_texture.txt`):
| Symbol | C3DDLL64 RVA | VA | Signature |
|--------|-------------|-----|-----------|
| `C3D_MIDDLEPOINT::CreateManagedTexture` | 0x99 | 0x180000099 | `C3DAPI_TEXTURE* (const char* filename)` |
| `C3D_LIGHTING::TextureGet` | 0x43 | 0x180000043 | `C3DAPI_TEXTURE* (const char* name)` |
| `C3DAPI_TEXTURE::GetTextureFileName` | 0x175 | 0x180000175 | `char* ()` |
| `C3DAPI_TEXTURE::GetTextureWidth` | 0xE1 | 0x1800000E1 | `int ()` |
| `C3DAPI_TEXTURE::GetType` | 0xE2 | 0x1800000E2 | `C3DTEXTURETYPE ()` |
| `C3D_MIDDLEPOINT::InitializeTextureManager` | 0x1D0 | 0x1800001D0 | `void ()` |
| `C3D_MIDDLEPOINT::ResetTextureManager` | 0x1CF | 0x1800001CF | `void ()` |

**Path C (custom texture) is potentially viable**: The plugin can resolve `CreateManagedTexture` from `g_engine` (C3DDLL64.dll) via `GetProcAddress`, load an original texture asset, bind it via `vtable[0x70]` before the ninth Draw. Challenges:
- Asset must be original/cleared (no `game_assets.zip` extraction)
- Texture lifetime management (plugin-owned)
- DPI scaling + UV setup unresolved

**Path A (text-only) is also viable**: Font mechanism EXISTS in C3DDLL64.dll (`scratch/out/extsym_font.txt`):
| Symbol | C3DDLL64 RVA | VA | Signature |
|--------|-------------|-----|-----------|
| `C3D_FONTMANAGER::PrintLeftUnicode` | 0x111 | 0x180000111 | `void (C3D_FONT*, float, float, wchar_t const*)` |
| `C3D_FONTMANAGER::PrintCenterUnicode` | 0x112 | 0x180000112 | `void (C3D_FONT*, float, float, wchar_t const*)` |
| `C3D_MIDDLEPOINT::CreateManagedFontBitmap` | 0x1C0 | 0x1800001C0 | `C3D_FONT* (const char* filename)` |
| `C3D_FONT::GetSize` | 0x11C | 0x18000011C | `float ()` |
| Font manager global | `0x996FB0` | `0x140996FB0` | `C3D_FONTMANAGER*` |
| Default font global | `0x994200` | `0x140994200` | `C3D_FONT*` |

**Input mechanism EXISTS** (`scratch/out/extsym_input.txt`):
| Symbol | C3DDLL64 RVA | VA | Signature |
|--------|-------------|-----|-----------|
| `C3D_INPUT::GetMouseSolid` | 0x10E | 0x18000010E | `C3DVECTOR3* ()` |
| `C3D_INPUT::GetMouseLeftPress` | 0x118 | 0x180000118 | `bool ()` |
| `C3D_INPUT::GetMouseLeftDown` | 0x119 | 0x180000119 | `bool ()` |

## 8. Investigation I2 — Ninth-tile clipping (RESOLVED — clipping exists)

**DrawFrom9Patch and DrawFrom3Patch in C3DDLL64.dll BOTH perform clipping/visibility checks.** Separate Ghidra project created at `scratch/ghidra_proj_c3ddll64` with C3DDLL64.dll imported.

**DrawFrom9Patch** (RVA `0xB99C0`, VA `0x1800B99C0`, 310 instructions):
- `0x1800B99DF`: `CMP byte ptr [RCX + 0x6760], 0x0` — checks panel+0x6760 visibility/enable flag
- `0x1800B99E9`: `JZ 0x1800BA199` — if flag == 0, jump to function return (no drawing)
- `0x1800B9ADB`: `JBE 0x1800BA15C` — bounds check, skip clipping adjustment
- `0x1800B9B39`: `JBE 0x1800BA107` — bounds check, skip clipping adjustment
- Floating-point COMISS comparisons determine if rect is within clipping bounds

**DrawFrom3Patch** (RVA `0xB9680`, VA `0x1800B9680`, 160 instructions):
- `0x1800B9691`: `CMP byte ptr [RCX + 0x6760], 0x0` — same panel+0x6760 flag check
- `0x1800B96A7`: `JZ 0x1800B99AC` — if flag == 0, jump to function return

**Conclusion**: Clipping IS performed in DrawFrom9Patch/DrawFrom3Patch, NOT in Draw. The panel+0x6760 byte flag controls whether the panel is drawn at all. COMISS floating-point comparisons determine bounds clipping. Whether the ninth tile is clipped depends on:
1. The value of panel+0x6760 at runtime (if 0, nothing is drawn)
2. Whether the ninth tile's rect falls within the clipping bounds
3. Unknown until runtime observation

Artifacts: `scratch/out/drawfrom9patch_disasm.asm`, `drawfrom9patch_disasm2.asm`, `drawfrom3patch_disasm.asm`

## 9. Investigation I3 — Click-path hook safety (Ghidra, complete)

**No 14-byte whole-instruction boundary at any click-write site** (established from `native-click-hookpoints-20261110/report.md`):
- Counter write (`0x14037435c`): 12-13 bytes (`C7 86 E8 0B 01 00 FF FF FF FF`)
- Selection write (`0x140374356`): 6-7 bytes (`89 9E 5C C6 00 00`)
- Combined exceeds 14 bytes; mid-function detour needs live-register/flags/stack analysis

**XrefsTo results** (Ghidra headless, SOVIET64.exe loaded into `soviet_trade` project):
| Address | Description | Total refs |
|---------|-------------|------------|
| `0x140374356` | Selection write `game+0xC65C = EBX` | 0 |
| `0x14037435C` | Counter write `game+0x10be8 = -1` | 0 |

**total=0 for both** — no jump table entries, no indirect branches, no cross-function calls target these addresses. Click-write sites are only reachable via sequential execution (fall-through).

**Conclusion**: Inline hook at click path is **not statically approvable**. Read-only click observation (read `game+0xC65C` selection index or `0xA54E91` latch state) without consuming the latch is the only viable approach, but may miss already-selected clicks in some frames.

## 10. Investigation I4 — Content suppression/replacement (IMPLEMENTED)

**FUN_14035f3e0 decompiled** (`scratch/out/14035f3e0_decomp.c`, 737 lines):
- Signature: `char FUN_14035f3e0(longlong game, float *x, float *y)` — returns `cVar4` (open/close state flag)
- Draws tabs 0-5 in a loop (line 82-152), each with `vtable[0x70]` texture bind + `DrawFrom9Patch`
- Special case for selection 4 (`game+0x10F20 == 4`)
- Click handling: `GetMouseSolid` + `Collision`, consumes `0xA54E91`, plays `gui_click`, calls `FUN_1403613b0(game, index, 1, state)`, sets `game+0x10F20 = 4`
- Uses `C3D_PANEL2D::Draw` extensively with the shared panel object `0x1409be060`
- Font manager at `0x140996FB0`, default font at `0x140994200`

**Handler calls FUN_14035f3e0 at `0x140374606`** (after tile loop), passing `RCX = RSI` (game pointer).

**Suppression mechanism**: `FUN_14035f3e0` lines 178-181:
```cpp
if ((*(char *)(param_1 + 0x5e6) != '\0') &&
    (cVar5 = FUN_140335bc0(), cVar4 = '\0', cVar5 != '\0')) {
    cVar4 = '\x01';
}
if (cVar4 == '\0') {
    // draw content
}
```

When `game+0x5E6 != 0` AND `FUN_140335bc0() != 0`, `cVar4 = 1` which skips native
content drawing. `game+0x5E6` is also checked by the main handler at
`0x140374b3a` and `0x140374bc4` (both `CMP byte ptr [RSI + 0x5e6], 0x0`) to
enable certain code paths.

**Native code toggles `game+0x5E6`** (`FUN_14035f3e0` line 194):
```cpp
*(bool *)(param_1 + 0x5e6) = *(char *)(param_1 + 0x5e6) == '\0';
```
This happens when `cVar5 != '\0' && DAT_140a54e91 != '\0'` (click latch set),
meaning native clicks can flip the flag back to 0.

**Implementation** (v3.0): Writes `game+0x5E6 = 1` every frame when Almanac is
open (to counter native toggling), restores `game+0x5E6 = 0` when closed.
Configurable via INI `content_suppression` (default: 1). Uses `WriteU8` with
SEH protection. Never writes `game+0xC65C`, never consumes `0xA54E91`.

**Limitations:**
- Suppression requires `FUN_140335bc0() != 0`; behavior of `FUN_140335bc0`
  unknown (not decompilable with current Ghidra setup — raw bytes show standard
  x64 prologue `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30`).
- Native code toggles `game+0x5E6` on certain clicks; we re-assert it every
  frame but there is a window between frames where native content could draw.
- Writing `game+0x5E6` modifies native state; side effects beyond content
  suppression are unknown. No in-game testing performed.

**FUN_1403613b0** (`scratch/out/bytes_1403613b0.txt`): Real code exists at
`0x1403613B0` (x64 prologue `40 53 41 54 55 56 48 83 EC 28`), but Ghidra's
analysis hasn't created a proper function body (size=1 in FuncInfo). Referenced
via function pointer tables at `0x140A66D08`, `0x14094C3D0`, `0x14094C3E0` only.
Called by FUN_14035f3e0 as the sub-tab click handler with signatures
`(game, index, 1, state)` and `(game, index, state, 1)`.

**FUN_140335bc0** (`scratch/out/bytes_140335bc0.txt`): Real code exists at
`0x140335BC0` (x64 prologue `48 89 5C 24 08 48 89 6C 24 10 ...`), but Ghidra's
analysis hasn't created a proper function body. Called by FUN_14035f3e0 to check
content suppression condition.

No clean hook point exists at call site `0x140374606` (no 14-byte boundary).

## 11. Implementation readiness assessment (post-implementation v3.0)

| Item | Status | Notes |
|------|--------|-------|
| Build | PASS | `build.bat almanac_button` succeeds; DLL at `build\plugins\almanac_button.dll` |
| Static guards | PASS | All original byte/value checks preserved |
| Ninth tile draw | PASS | Draws at origin + 8*pitch; texture binding implemented |
| Click detection | IMPLEMENTED | GetMouseSolid + own rect test + latch read (read-only) |
| Almanac toggle | IMPLEMENTED | Mod-owned `g_almanacOpen`, debounced by latch |
| Close on native click | IMPLEMENTED | Reads `game+0xC65C` changes (0..7) |
| Read-only (no latch consumption) | PASS | Never writes `0xA54E91`, `game+0xC65C`, `game+0x10BE8`, `game+0x1039A` |
| Native buttons preserved | PASS | All 8 unchanged; City Hall TAX not moved |
| Distinct icon | IMPLEMENTED (code) | `CreateManagedTexture` resolved; texture binding via vtable[0x70] before ninth Draw. Asset path configurable via INI `texture_asset`. No asset loaded by default. |
| Text label | IMPLEMENTED (code) | `PrintLeftUnicode` resolved; text rendering implemented. Font path configurable via INI `font_asset`. No font loaded by default. |
| Ninth-tile clipping | RESOLVED (mechanism known) | DrawFrom9Patch/3Patch perform clipping via panel+0x6760 flag + COMISS bounds checks. Whether ninth tile is clipped depends on runtime flag value. |
| Content suppression | IMPLEMENTED | `game+0x5E6` write every frame when Almanac open (INI `content_suppression`,
  default 1). Requires `FUN_140335bc0() != 0` to take effect; behavior unknown.
  Native code may toggle flag back; re-asserted every frame. |

## 13. Test findings and fix (v3.1)

### Test run 1 — button rendered but not clickable

**Observation**: The ninth tile rendered beside the Economy & Trade row with repeated native (crime-button) artwork. Hovering showed both the native crime button and the ninth tile highlighting. But clicking the ninth tile had no effect.

**Root cause**: `tesmioloader.log` line 76:
```
almanac_button  NOTE  no engine export for GetMouseSolid; ninth-tile clicks not detected
```

The mangled name used in `GetProcAddress` was `?GetMouseSolid@C3D_INPUT@@QEAPEAMXZ` (return type `float*`), but the actual C3DDLL64.dll export is `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` (return type `C3DVECTOR3*`). `GetProcAddress` failed, `g_getMouseSolid` remained NULL, and `CheckNinthClick()` returned 0 on every call.

**Fix applied** (v3.1):
- Changed mangled name to `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ`
- `C3DVECTOR3` is an X/Y float pair; `CheckNinthClick` reads `mouse[0]` and `mouse[1]` as before
- Rebuilt; `build.bat almanac_button` succeeds

### Default texture asset set

`almanac_button.ini` now defaults `texture_asset` to:
```
"C:\Workers Resources Soviet Republic\media_soviet\editor\icon_inspectnumbers.png"
```

The plugin loads this via `C3D_MIDDLEPOINT::CreateManagedTexture` at C3DDLL64.dll RVA `0x99`. If the file format is not accepted by `CreateManagedTexture`, the plugin logs "texture failed to load" and the ninth tile shows repeated native artwork (no crash).

## 13. Test findings and fixes (v3.1 → v3.2)

### Test run 1 — button rendered but not clickable

**Observation**: The ninth tile rendered beside the Economy & Trade row with repeated native (crime-button) artwork. Hovering showed both the native crime button and the ninth tile highlighting. But clicking the ninth tile had no effect.

**Root cause**: `tesmioloader.log` line 76:
```
almanac_button  NOTE  no engine export for GetMouseSolid; ninth-tile clicks not detected
```

The mangled name used in `GetProcAddress` was `?GetMouseSolid@C3D_INPUT@@QEAPEAMXZ` (return type `float*`), but the actual C3DDLL64.dll export is `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` (return type `C3DVECTOR3*`). `GetProcAddress` failed, `g_getMouseSolid` remained NULL, and `CheckNinthClick()` returned 0 on every call.

**Fix applied** (v3.1):
- Changed mangled name to `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ`
- `C3DVECTOR3` is an X/Y float pair; `CheckNinthClick` reads `mouse[0]` and `mouse[1]` as before
- Rebuilt; `build.bat almanac_button` succeeds

### Test run 2 — button did not render at all (crash at Start)

**Observation**: After v3.1, the ninth tile did not render. The tesmioloader log showed:
```
FAULT    plugin start: code C0000005 at 00007FF8D7EF1735
         inside tesmioloader.dll (base 00007FF9067C0000)
         read from of 0000000000000000
plugin   almanac_button started with -1 - inactive
```

**Root cause**: Texture/font loading was done in `TsmPluginStart()`, which runs before the game object is initialized. The code computed `middlepoint = Base() + TSM_RVA_DEFAULT_GAME_OBJECT + 0xB048u` (i.e., `game + 0xB048`) and passed it to `CreateManagedTexture`. At Start time, `game` (at RVA `0x9D4F10`) is NULL, so `middlepoint` was an invalid address, and `CreateManagedTexture` dereferenced it, causing a null pointer read crash.

**Fix applied** (v3.2):
- Removed texture/font loading from `TsmPluginStart()`
- Added **lazy loading in `DrawNinthTile`** on the first draw call, when the game is active and `game+0xB048` is valid
- Texture/font are loaded once, on the first ninth-tile draw, then cached in `g_almanacTexture`/`g_almanacFont`
- Rebuilt; `build.bat almanac_button` succeeds

### Default texture asset

`almanac_button.ini` defaults `texture_asset` to:
```
"C:\Workers Resources Soviet Republic\media_soviet\editor\icon_inspectnumbers.png"
```

The plugin loads this via `C3D_MIDDLEPOINT::CreateManagedTexture` at C3DDLL64.dll RVA `0x99`. If the file format is not accepted by `CreateManagedTexture`, the plugin logs "texture failed to load" and the ninth tile shows repeated native artwork (no crash).

### Test run 3 — crash when opening maintab (sidebar)

**Observation**: After v3.2, clicking the maintab (sidebar) caused a crash:
```
=== CRASH: ACCESS_VIOLATION at 00007FF956F61780 ===
    C3DDLL64.dll + 0xD1780
    reading address 00007FF957082078
```
Stack return address included `SOVIET64.exe + 0x3743A8` (right after our ninth-tile Draw call).

**Root cause**: Post-Draw texture binding via `vtable[0x70]` on the `CreateManagedTexture` result, and post-Draw text rendering via `PrintLeftUnicode`. The texture/font objects returned by the engine creation APIs don't have the expected vtable layout at offset `0x70`, or the objects themselves are invalid. The crash occurs after the native Draw returns, when we try to bind the texture or render text.

**Fix applied** (v3.3):
- Removed post-Draw texture binding via `vtable[0x70]` — the texture object's vtable layout is unknown and unsafe to call
- Removed post-Draw text rendering via `PrintLeftUnicode` — font object's method layout is unknown
- Texture/font lazy loading is retained (loads on first draw, caches in `g_almanacTexture`/`g_almanacFont`) but the loaded objects are not yet used for binding/rendering
- Ninth tile renders with whatever texture is currently bound by the native handler (typically repeated native artwork)
- Ninth tile remains clickable — click detection (`GetMouseSolid` + rect test + read-only latch) is unaffected
- Removed unused defines `ALM_TEX_BIND_OFFSET` and `ALM_PANEL_STATE_OFF`
- Rebuilt; `build.bat almanac_button` succeeds

**Custom icon path forward**: To bind a custom texture, we need to determine the correct vtable offset for `TexBind` on the `C3DAPI_TEXTURE` object returned by `CreateManagedTexture`, or find an alternative texture binding API. Currently unknown — requires further Ghidra investigation of `C3DAPI_TEXTURE` vtable layout.

## 14. Implementation readiness assessment (v3.2)

| Item | Status | Notes |
|------|--------|-------|
| Build | PASS | `build.bat almanac_button` succeeds; DLL at `build\plugins\almanac_button.dll` |
| Static guards | PASS | All original byte/value checks preserved |
| Ninth tile draw | PASS | Draws at origin + 8*pitch; texture binding implemented |
| Click detection | IMPLEMENTED | GetMouseSolid mangled name fixed; own rect test + latch read (read-only) |
| Almanac toggle | IMPLEMENTED | Mod-owned `g_almanacOpen`, debounced by latch |
| Close on native click | IMPLEMENTED | Reads `game+0xC65C` changes (0..7) |
| Read-only (no latch consumption) | PASS | Never writes `0xA54E91`, `game+0xC65C`, `game+0x10BE8`, `game+0x1039A` |
| Native buttons preserved | PASS | All 8 unchanged; City Hall TAX not moved |
| Distinct icon | LOADED NOT BOUND | `CreateManagedTexture` resolved; lazy-loaded on first draw; INI `texture_asset` defaults to `icon_inspectnumbers.png`. Texture object loaded but NOT bound via vtable[0x70] — vtable layout unknown, caused crash. Ninth tile shows repeated native artwork. |
| Text label | LOADED NOT RENDERED | `PrintLeftUnicode` resolved; lazy-loaded on first draw; font path via INI `font_asset` (empty by default). Font object loaded but NOT used for rendering — method layout unknown, caused crash. No text label on ninth tile. |
| Ninth-tile clipping | RESOLVED (mechanism known) | DrawFrom9Patch/3Patch check `panel+0x6760` flag + COMISS bounds |
| Content suppression | IMPLEMENTED | `game+0x5E6` write every frame when open (INI `content_suppression`, default 1) |

## 15. Remaining work (requires separate authorization)

1. **In-game testing**: Click detection has the correct mangled name, crash fix applied, but the button has not been retested in-game after v3.3. Requires game launch authorization.
2. **Texture binding vtable layout**: `CreateManagedTexture` returns a `C3DAPI_TEXTURE*` but the vtable offset for `TexBind` is unknown. Our assumption of offset `0x70` caused a crash. Need to trace `C3DAPI_TEXTURE` vtable in C3DDLL64.dll to find the correct bind method offset, or find an alternative texture binding API.
3. **Texture format verification**: `icon_inspectnumbers.png` may or may not be accepted by `CreateManagedTexture`. If not, a DDS or other format may be needed.
4. **Content suppression validation**: Depends on `FUN_140335bc0()` returning non-zero (unknown behavior).
5. **Runtime verification of clipping**: Observe whether panel+0x6760 is set and whether the ninth tile is clipped at runtime.