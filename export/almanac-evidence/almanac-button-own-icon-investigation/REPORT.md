# Almanac button — artwork ownership and path to a distinct clickable button

Static investigation only. No source edits, builds, game launches, hooks,
deployments, save edits, or click tests. Existing evidence preserved unchanged.
The Ghidra project lock at `C:\WRSRE\scratch\ghidra_proj_soviet_trade\soviet_trade.lock`
was not touched and remains in place.

## Executive summary

The extra Economy & Trade tile repeats the vanilla (crime-button) artwork
because the prototype (`plugins/almanac_button/almanac_button.cpp`) does not
change any texture/icon state — it calls the engine's own `C3D_PANEL2D::Draw`
again with the **same intercepted `self` panel object** and arguments, changing
only the shared X/Y rectangle pair (RVA `0x9BE2F0`/`0x9BE2F4`). The artwork
bound to that panel object is whatever the native row handler last set up
(per-tile via `FUN_140445eb0`, body not traced). Repeating the draw therefore
re-renders the same panel state at a new position. There is no static evidence
that `C3D_PANEL2D::Draw` "owns" the crime icon, nor that a second draw with a
different rect produces a distinct, clickable button. A distinct Almanac button
requires a separate panel/texture object with its own texture, hit-test rect,
and input handling — none of which the prototype creates, and none of which the
existing `ui_panels` API exposes for the global sidebar row (it is a
building-window widget contract keyed on `window+0x250`, gated on a valid
building).

## Method and identity limits

- Static analysis only. Reads preserved exports, source, and docs. No Ghidra
  query was run in this session; the `soviet_trade` Ghidra project is locked
  (`.lock` present) and was not opened.
- Executable identity: SOVIET64.exe baseline v1.1.1.9, PE TimeDateStamp
  `0x6A3EB6AD`, preferred Ghidra image base `0x140000000`. The visual-test run
  reported game v1.1.1.9 and plugin `almanac_button` 1.5 armed, but the log does
  not record the DLL SHA-256 or exact source commit, so the tested binary cannot
  be independently identified (result.md). Addresses below are preferred-image
  RVAs/VA; runtime VA is ASLR-relocated.
- C3DDLL64.dll (engine panel DLL): image base `0x180000000`, SHA-256
  `65FCB684...`. `C3D_PANEL2D::Draw` RVA `0xBAF40` (VA `0x1800BAF40`),
  `Collision` RVA `0xBC690` (VA `0x1800BC690`), `DrawFrom9Patch` RVA `0xB99C0`,
  `DrawFrom3Patch` RVA `0xB9680` — bodies not fully traced.
- All "verified" claims below are instruction-address-level facts from preserved
  disassembly/exports; runtime path (which handler executed, which tile wrote the
  panel state) is **not** established.

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

## 6. Recommended next gate

**Do not implement a hook.** The evidence does not establish a defensible design
for a distinct, clickable, same-row Almanac button. Recommended next gate
(separately authorized):

1. **Confirm Ghidra project availability/lock.** The `soviet_trade` project is
   locked (`.lock` present). Do not bypass or delete. If a query is needed,
   route through the designated Ghidra agent per the one-run-at-a-time rule.
2. **Trace texture/icon ownership in `C3D_PANEL2D`** — identify the field(s)
   that hold the per-tile texture/UV/border and whether a plugin can construct a
   separate panel object with its own texture (addresses: panel fields
   `+0x673c`/`+0x6740`/`+0x6744`/`+0x674c`, Draw RVA `0xBAF40`,
   `DrawFrom9Patch` `0xB99C0`, `DrawFrom3Patch` `0xB9680`).
3. **Determine whether a same-row ninth tile is clipped** and whether a distinct
   hit-test rect can agree with a distinct draw rect without interfering with
   the eight native hit-tests.
4. **Only if (2)-(3) are resolved**, propose a design: owned texture asset,
   panel lifetime, DPI-scaled origin/pitch/clip math, input arbitration,
   close/switch/return behavior, and a hook-safe observation point (none
   currently exists at the click path). This design, not the existing backdrop
   hook, is the artifact to review before any implementation.

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