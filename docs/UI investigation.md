# UI Investigation — interactive menus, buttons, and the Economy & Trade sidebar

> **Status:** investigation only. No plugin/header/source was modified for this
> document. Target binary `SOVIET64.exe` v1.1.1.9 (build stamp `0x6A3EB6AD`),
> engine `C3DDLL64.dll`. Ghidra project loaded at file-base `0x140000000`; a
> query address is `0x140` + RVA.
>
> Log: 2026-10-04 draft (hotkeys vs C3D_GUI hypothesis). **2026-10-11 Ghidra
> session: DisasmMany + DecompAt + Xrefs on `0x2727F0` (slider),
> `0x3956F0` (labelled checkbox), `0x3959D0` wrapper, `0x7CEF60` (city window
> head), `0x265500` (options-menu candidate). Findings in §11 supersede
> §§0/1/4/5 wherever they conflict.**
>
> **2026-10-11 session 2 — PROBE P0 CLOSED (§11.9).** The stats/tools sidebar
> ("Economy & Trade" family) is drawn by **`FUN_140296110` (RVA `0x296110`,
> 42,071 bytes)**. Two new tools made this possible: `scratch/strrva.py`
> (needle → RVA; `peinfo.py strings` gives contents but no addresses) and
> `scratch/StrAddr.java` (data address + code xrefs in one run). Raw outputs
> `scratch/ui_stradd_*.txt`, `ui_xrefs_trade*.txt`, `ui_funcs_side.txt`,
> `ui_probe_sidebar.asm`, `ui_probe_sidetabs.asm`, `ui_xrefs_sidebar.txt`.
>
> Everything marked **verified** was read from disassembly, an export table, or
> a shipped data file. Everything marked **PROBE** is a hypothesis that still
> needs a Ghidra run before it is trusted.

---

## 0. TL;DR — what is and is not possible today

| Goal | Verdict | Mechanism |
|---|---|---|
| Add real clickable buttons to a **building window** (City Hall / shop / factory) | **Possible**, but we draw + hit-test them ourselves | `ui_panels` post-hook draws button rects via `C3D_PANEL2D::DrawFrom9Patch`; a mouse read (`GetAsyncKeyState(VK_LBUTTON)` or `C3D_INPUT`) detects the click |
| Direct numeric entry (type a tax rate / wage) | **Possible** via engine textboxes | `C3D_GUI::AddTextBox` + `GetElementText`/`GetElementTextFloat` — but only inside a `C3D_GUI` menu, **not** a building panel window (see §3) |
| Sliders for +/- adjustments | **Possible** | `C3D_GUI::AddSlider` + `GetSliderPercent`, or draw our own +/- button rects |
| Put the tax dashboard in the **stats & tools sidebar (Economy & Trade)** as its own section | **Yes — builder found (§11.9)** | `FUN_140296110` @ RVA `0x296110`, called every frame from `FUN_14028c770`. Takes `game` in RCX (not a window), so `window+0x250` does **not** apply — but the §11.1/§11.2 widgets take `game` too, so a post-hook here can call them directly. Visibility gate + row origin still PROBE (§11.10) |
| Toggle "all buildings vs. this building" | **Possible today, ship as a native checkbox** | exe-internal `0x3956F0` labelled checkbox, string label, no text id needed (§11) |

**CORRECTION (2026-10-11 Ghidra session, supersedes the two-system claim
below):** building panels are **not** "text rows only with no native widget to
piggyback on". The exe ships panel-native widget functions that draw,
hit-test, **and mutate the value themselves** and that panel builders call
directly: the `0..100` slider with −/+ buttons at **`0x2727F0`** and the
labelled checkbox at **`0x3956F0`** (both verified by DisasmMany + DecompAt +
Xrefs this session — §11). `cities.cpp` (TesmioLoader reference plugin, in
`C:\Users\EricM\Downloads\Other mods by other people\Tesmio\plugins\cities\cities.cpp`)
already calls both from inside a `window+0x250` post-hook on the live game —
`o_Slider(game, x, y, -1, &pct, dpi, live, step, live, NULL)` +
`o_Checkbox(game, x, y, &sq, L"Square city area", colour, NULL, size, 0, live,
1, 0)` — with `live = window+0x20`, `game+0xAD74 == -1` (no widget busy),
`DAT_0xA54E91` click latch, and `game+0xADB0` widget-draw context. **So the
fastest route to real buttons is neither "draw + hit-test ourselves" nor
"C3D_GUI": it is calling the same two exe functions from our own
`ui_panels` post-hook.** The old §§1/4/5 `C3D_GUI`-toolkit path (engine
`AddButton`/`AddSlider`/`AddTextBox`) is demoted to a stretch goal — the exe
imports **zero** `C3D_GUI` symbols and no live `C3D_GUI*` instance has been
found (the 2026-10-04 draft's own import count was misread; see §11.5). The
`C3D_INPUT::GetMouseSolid` + `C3D_PANEL2D::Collision` + latch pattern (§4 opt
2) stays valid, but as the engine's own idiom *inside* these widget calls, not
as custom code we write.

**The single most important architectural fact:** the game has **two unrelated
UI systems**, and the difference decides every approach below.

1. **Building panel windows** (shop `0x7272F0`, residence `0x761300`, pub
   `0x723950`, factory `0x799630`, cityhall/fallback `0x6F4740`). These are
   drawn **imperatively** — `C3D_LANGUAGE::GetString` for the label, then
   `C3D_FONTMANAGER::PrintLeftUnicode` for the row, `y += DPI*35`. **They do
   NOT use the `C3D_GUI` widget toolkit.** This is what `ui_panels` hooks and
   appends text rows to. There is no native button here to piggyback on.

2. **The `C3D_GUI` widget toolkit** (`AddButton`, `AddSlider`, `AddTextBox`,
   `AddCheckBox`, `AddComboBox`, `AddSelector`, `AddRadioPoint`, `AddText`,
   `AddLayer` …). This is the engine's real button/slider/textbox system, with
   skins declared in `media_soviet\gui\default.ini` (`$BUTTON`, `$CHECKBOX`,
   `$TEXTBOX`, `$SLIDER`, `$COMBO`, `$SELECTOR`). **It is engine-internal** —
   the exe does not import it (verified: 0 `C3D_GUI` entries in
   `SOVIET64_exe.imports.txt`), so a plugin reaches it only via
   `GetProcAddress(C3DDLL64.dll, "?AddButton@C3D_GUI@@…")` **and** a live
   `C3D_GUI*` instance. It is used by the game's own menu windows, not by the
   building panels.

Consequence: **the City Hall fiscal panel we shipped (Task 4) lives in system 1
(text rows + hotkeys). To give it real buttons we either (a) draw our own
button rectangles into that panel and hit-test clicks ourselves, or (b) move
the dashboard into a real `C3D_GUI` menu window (system 2) where
`AddButton`/`AddSlider`/`AddTextBox` are native.** Both are laid out below.

---

## 1. Verified engine surface — the native widget toolkit (`C3D_GUI`)

All of the following are **exported from `C3DDLL64.dll`** (verified in
`tools/peinfo-out/C3DDLL64_dll.exports.txt`). `C3DVECTOR3` is the
position/size argument.

### Widgets you can create
| Export | RVA | Mangled | What it gives us |
|---|---|---|---|
| `AddButton` | `0x6E7F0` | `?AddButton@C3D_GUI@@QEAAXPEADPEA_WVC3DVECTOR3@@@Z` | clickable button `(id, label, pos/size)` |
| `AddCheckBox` | `0x6EA30` | `…PEADPEA_WVC3DVECTOR3@@@Z` | checkbox (ON/OFF) |
| `AddSlider` | `0x6EF80` | `…PEADPEA_WVC3DVECTOR3@@@Z` | slider (0..1 percent) |
| `AddComboBox` | `0x6F420` | `…PEADVC3DVECTOR3@@@Z` | dropdown |
| `AddSelector` | `0x6F650` | `…PEADVC3DVECTOR3@@@Z` | selector list |
| `AddRadioPoint` | `0x6EC80` | `…PEADPEA_WVC3DVECTOR3@@H@Z` | radio point (grouped toggle) |
| `AddText` | `0x6F1D0` | `…PEADPEA_WVC3DVECTOR3@@@Z` | static label |
| `AddTextBox` | `0x6F880` | `…PEADVC3DVECTOR3@@_N2@Z` | **text/number entry** |
| `AddElementItem` | `0x6FC00`/`0x6FC80` | `…PEAD0@Z` / `…PEADPEA_W@Z` | list item for combo/selector |
| `AddLayer` | `0x6E100` | `…PEAD@Z` | layer/group for layout |
| `OffsetLayer` | `0x6E3A0` | `…PEADMM@Z` | move a layer |

### State you can read back (this is the "button was pressed" mechanism)
| Export | RVA | Use |
|---|---|---|
| `GetElementStateValue` | `0x6FDB0` | button/checkbox pressed state |
| `GetElementStateValueChange` | `0x6FE10` | **edge-trigger** (pressed *this* poll) |
| `GetElementStateSelectedItem` | `0x6FF00` | selected index of combo/selector |
| `GetElementStateSelectedItemChanged` | `0x6FFB0` | selection changed edge |
| `GetSliderPercent` | `0x703C0` | slider 0..1 |
| `GetElementText` | `0x702E0` | textbox contents (wchar) |
| `GetElementTextFloat` | `0x70330` | textbox parsed to float |
| `GetElementTextFloatChanged` | `0x70370` | text changed edge |
| `IsSliderInProgress` / `IsTextboxInProgress` / `IsAnyTexboxInProgress` | `0x70430`/`0x70C10`/`0x70C00` | drag/edit in progress |
| `GetTexboxInProgressText` | `0x6B8C0` | live text while editing |
| `FindElement` / `RemoveElement` / `ClearElements` / `ClearElementItems` | `0x6C7E0`/`0x6FAD0`/`0x6FB00`/`0x6FD50` | manage elements |
| `Refresh` / `Render` | `0x6D8A0`/`0x6DC60` | lifecycle |
| `Initialize` | `0x6C8D0` | `Initialize(MIDDLEPOINT*, C3D_INPUT*, const char*)` |

**Read-back model is polling, not callbacks.** You create a widget with a
string `id`, then each frame poll `GetElementStateValueChange(id)` (button),
`GetSliderPercent(id)` (slider), `GetElementTextFloatChanged(id, &f)`
(textbox). No function-pointer callback is registered — this is exactly the
shape our hotkey polling already uses, just with widgets instead of keys.

### Skins are data-driven (verified)
`media_soviet\gui\default.ini` declares the look of every widget type, so a
`C3D_GUI` menu drawn through the toolkit is **pixel-consistent with stock UI**:

```
$BUTTON   SIZE 100 20 … TEXTUREON gui/default/button.dds
$CHECKBOX SIZE 20 20 … TEXTUREON gui/default/checkbox_on.dds
$RADIO    SIZE 20 20 … radiobutton_on/off.dds
$TEXTBOX  SIZE 120 20 … textbox.dds
$SLIDER   SIZE 18 120 5 … slider.dds / sliderpoint.dds
$COMBO    SIZE 120 20 120 18 … combodrop/combopoint/comboarrow…
$SELECTOR SIZE 120 160 18 …
$TEXT     FONT font1 0xffffffff …
```

### Blocker for building windows (verified)
`SOVIET64_exe.imports.txt` shows **0** `C3D_GUI` imports. The building panels
never touch this toolkit. So `AddButton` inside a shop/City Hall window would
mean (1) obtaining a `C3D_GUI*` instance the exe does not expose to us, and
(2) hoping it renders into the same panel — neither is safe to assume. **For
building windows, the realistic button path is draw-our-own (§4).** The
`C3D_GUI` toolkit is the natural fit only for a **standalone menu window**
(§6) — which is exactly the Economy & Trade sidebar the request wants.


---

## 2. The building-panel window contract (what `ui_panels` already uses)

From `docs/Archive dont read/17-ui-and-integration.md` §2 and re-verified in
`18-ghidra-followup.md`. This is the system our Task 4 City Hall panel and the
enterprise/wage rows live in.

| Window offset | Contents |
|---|---|
| `+0x01` | non-zero while the panel is **not** drawn (builder returns immediately) |
| `+0x04`, `+0x08` | panel position (float x, y) |
| `+0x28`, `+0x2C` | panel scroll/offset |
| `+0x240` | the building the window is about |
| `+0x250` | **the running Y — the panel's bottom edge** |

Draw with the game's own font:
`C3D_FONTMANAGER::PrintLeftUnicodeNoArg(&[0x996FB0], [0x995220], x, y, 0xFF990000, L"…")`.
One row is `DPI * 35` (`DPI` at `0x992088`). The append pattern is a post-hook
on the builder: run the original, read `window+0x250`, draw rows, write the new
bottom edge back.

Hooked builders today (verified prologues, `ui_panels.cpp`):
shop `0x7272F0`, residence `0x761300`, pub `0x723950`, factory `0x799630`,
cityhall/fallback `0x6F4740` (added for Task 4).

**Key limitation:** rows here are **text only**. There is no per-row button
object to clone (the panels are imperative `GetString`+`Print`, per
`Labor_and_Salary_Overhaul_Plan.md` §2: "panels are imperative … no row object
to clone").

---

## 3. The stats & tools sidebar / Economy & Trade menu (the target)

The sidebar the request describes is **not** a `ui_panels` panel builder — it
is the game's own tabbed menu. Verified evidence in the exe:

- **Main-tab textures** (`editor/maintab_*.png`, verified unique set):
  `list, population, qa, resources, storage, sun, temperature, wind, highlight`
  → `editor/maintab_%s.png`, `editor/maintab_highlight.png`.
- **Sub-tab + click sounds:** `gui_maintab_click`, `gui_subtab_click`,
  `gui_subtab_cancel`, `gui_window_open`, `gui_window_change`.
- **Bottom menu:** `editor/bottomtab_%s.png`, `BOTTOMMENU_EDITOR`,
  `BOTTOMMENU_WORKSHOP`, `BOTTOM_MENU_LEVEL_1/2_SCALE`.
- **Window manager props** (how the game flags a window's behaviour):
  `WINDOW_AUTOCLOSING`, `WINDOW_MINIMIZE`, `WINDOW_PAUSE`,
  `WINDOW_OPEN_ON_LEFT`, `USE_MENU_ZOOM`, `close_all_windows`,
  `close_windows_other_than`, `pin_window_for`, `miniwindow_goods`,
  `miniwindow_citizens`.
- **`C3D_PANEL2D`** is imported by the exe (verified) — `BeginDraw`, `Draw`,
  `DrawFrom9Patch`, `DrawFrom3Patch`, `EndDraw`, `Collision`,
  `CollisionRect`. These are how the game draws window chrome and any
  tappable rect. `Collision*` is the engine's own hit-test we can reuse.

**What is NOT yet pinned (PROBE):** the exact builder function that lays out
the statistics / Economy & Trade window, and the string id of the
"Economy & Trade" tab. The maintab textures above are the *map-overlay view
modes*; the *statistics/graphs* window (the one with economic indicators and a
category sidebar) is a separate builder in the `0x78xxxx–0x7Axxxx` panel
family noted in `17-ui-and-integration.md` §10 (`funcs_panels.txt` enumerates
the ~20 big builders there). That is the first Ghidra probe (§8).

Two viable routes into it, once the builder is found:
- **Post-hook the stats-window builder** (same recipe as `ui_panels`, but a new
  hook on a new RVA): append our tax section as rows at `window+0x250`, and
  draw +/- / toggle button rects with `C3D_PANEL2D::DrawFrom9Patch`.
- **`GetString` label-retag** (the `reputation`-mod route, verified): hook
  `C3D_LANGUAGE::GetString` (`?GetString@C3D_LANGUAGE@@QEAAPEA_WH@Z`, object
  inline at `0x997590`) and rewrite a specific label id to carry our numbers.
  Cheapest "feels native" integration, but read-only display — not buttons.

---

## 4. Path A — real buttons inside a building window (draw + hit-test ourselves)

Because building panels are imperative text (§2), a genuine clickable button
means we draw the button and detect the click. This is fully within the
render-thread post-hook `ui_panels` already runs.

**Draw the button** (verified exports on `C3D_PANEL2D`, imported by the exe):
- Background: `DrawFrom9Patch(rect, …)` or `DrawFrom3Patch` with the same
  button texture the game uses (`gui/default/button.dds` via the `$BUTTON`
  skin), or a solid 9-patch from `C3D_PANEL2D`.
- Label: `PrintLeftUnicodeNoArg(…, x, y, colour, L"VAT +1%")`.

**Detect the click** — three options, cheapest first:
1. **`GetAsyncKeyState(VK_LBUTTON)` + cursor-rect test.** Same polling shape as
   `citizen_labor`/`taxes` hotkeys (already proven in this repo). Get the mouse
   position via `GetCursorPos` (screen) — need the window→panel mapping, or use
   the engine's `C3D_INPUT` mouse read. Foreground-gate on the game window
   (`GetForegroundWindow` + pid match), exactly as `PollWageKeys`/`PollTaxKeys`
   do. **No new hook.** Simplest; needs screen↔panel coordinate mapping (PROBE
   — `C3D_INPUT` or `window+0x04/+0x08` + DPI).
2. **Hook `C3D_INPUT`** to read the engine's own mouse state
   (`CommonInterfaceGetButtonPress`, `CommonInterfaceGetButtonPressed` — both
   exported). Cleaner, but a new inline hook on `C3D_INPUT`.
3. **Reuse `C3D_PANEL2D::CollisionRect`** for the rect hit-test against the
   engine's cursor position, so coordinates match the engine exactly.

**State:** keep pressed-state per (building, button) under `g_lock`, update the
taxes/wage value through the existing service API (`TsmTaxesApi::setTaxRate`,
`TsmCitizenLaborApi` wage setters). Poll edges (`pressed since last frame`) so
one click = one step, mirroring the `LSB & 1` edge trick already used for `T`
and `Space`.

**Pros:** stays in the window players already open; no new builder RVA to
maintain. **Cons:** we own hit-testing and button art; a game update that moves
row geometry can misplace buttons (same fragility as the panel rows today, and
the loader's prologue memcmp already degrades safely to "no rows").


---

## 5. Path B — move the dashboard into a real `C3D_GUI` menu (native widgets)

If we want **native** `AddButton`/`AddSlider`/`AddTextBox` (§1), the dashboard
must live in a `C3D_GUI` menu window, not a building panel. Concretely:

1. **PROBE:** find how the game opens a `C3D_GUI` menu and get a `C3D_GUI*`.
   The toolkit is engine-internal; likely reached by hooking a menu-builder and
   borrowing the `this` pointer, or a global `C3D_GUI` singleton. This is the
   critical unknown and the reason Path A is the safer first build.
2. Create our widgets by id (`tax_vat`, `tax_profit`, `tax_property`,
   `wage_plus`, `wage_minus`, `scope_toggle`).
3. Each frame poll `GetElementStateValueChange(id)` / `GetSliderPercent(id)` /
   `GetElementTextFloatChanged(id,&f)` and push into the service APIs.
4. `AddTextBox` gives **direct numeric entry** for free — type `22` and it
   parses to `22.0` via `GetElementTextFloat`. This is the clean answer to
   "type the value directly".

**Scope toggle (all buildings vs. this building):** a `C3D_GUI` radio group
(`AddRadioPoint`, grouped by the `int` arg) or a `AddSelector` list
("Workplace Only" / "Whole Sector") maps directly onto `citizen_labor`'s
existing scope model.

**Verdict:** native and robust *if* we can obtain the `C3D_GUI*`. Until that
probe lands, Path A is the deliverable and Path B is the stretch goal.

---

## 6. Putting the tax dashboard in the Economy & Trade sidebar (the request)

The user's preferred home is "the main sidebar in the stats and tools menu,
Economy & Trade, its own section." That is the stats-window menu from §3, not
a building window. Recommended sequence:

1. **PROBE (Ghidra):** locate the statistics-window builder. Start from the
   `0x78xxxx–0x7Axxxx` panel family (`funcs_panels.txt`), xref the
   `editor/maintab_*.png` / `gui_window_open` strings, and find the function
   that builds the category sidebar. Confirm it reads `window+0x240` (or the
   stats object) and writes `window+0x250`.
2. **Post-hook it** (new `ui_panels`-style hook, prologue-memcmp-guarded like
   the existing five) and append the fiscal section: live rates, [ON/OFF], and
   +/- / toggle button rects (Path A drawing).
3. **Alternative / complement:** `GetString` retag (§3, the `reputation` route)
   to surface a one-line "Fiscal Policy: VAT 20% · Profit 15% · Property
   250" inside an existing Economy & Trade row, with zero layout risk.

**Why a post-hook and not the `C3D_GUI` toolkit here:** the stats window is
part of the imperative panel family like the building windows (it uses
`GetString`+`Print`, and `C3D_PANEL2D` for chrome) — same as §2 — so the
draw-our-own approach is consistent and needs no `C3D_GUI*`.

---

## 7. What each existing plugin route buys us (recap of §17 doc)

| Route | Mechanism | Proven by | Gives buttons? |
|---|---|---|---|
| HUD overlay | `ui_overlay` drawer chain on `C3D_TERRAIN::Render` | every plugin here | text only |
| Panel row append | post-hook a panel builder, draw at `window+0x250` | `ui_panels`, `depletion`, `cities` | only if we draw+hit-test (§4) |
| Import/name hook | chain `C3D_LANGUAGE::GetString`, rewrite by id | `reputation` (vehicle list) | no — display only |
| `C3D_GUI` toolkit | `AddButton`/`AddSlider`/`AddTextBox` + poll getters | engine export, skins in `gui/default.ini` | **yes, native** — needs a `C3D_GUI*` (§5) |

`ui_overlay` (verified, `plugins/ui_overlay/ui_overlay.cpp`) gives us
`drawText`, `drawTextOutlined`, `getDPI`, `getColumnX`, `isVisible`,
`setVisible`, `registerDrawer` — a per-frame drawer bus. Useful for a HUD
fiscal readout, but it is text and lives over the world, not in a window.


---

## 8. Ghidra probe queue (sequential, one run at a time, `0x140`+RVA)

Per `.clinerules`: one `analyzeHeadless` at a time (project lock); use `.java`
scripts; addresses are `0x140`+RVA.

1. **P0 — the stats/Economy & Trade builder.** `ListFuncs 0x140780000–0x1407A0000`
   (the big panel family), then `Xrefs` on the `editor/maintab_%s.png` /
   `gui_window_open` string addresses to find the menu builder. `DisasmMany`
   its head to confirm the `window+0x240` / `+0x250` contract and a ≥14-byte
   PI prologue for a safe post-hook.
2. **P1 — a live `C3D_GUI*`.** Xref `?Initialize@C3D_GUI` / find a global
   `C3D_GUI` instance the game holds, to judge whether Path B (native widgets)
   is reachable. If a singleton exists, `AddButton` becomes first-class.
3. **P2 — mouse/cursor coordinates.** How `C3D_INPUT` exposes the cursor in
   panel space (for Path A hit-testing), or whether `GetCursorPos` + window
   rect is sufficient. Xref `CommonInterfaceGetButtonPress`.
4. **P3 — the "Economy & Trade" string id.** Decode `sovietEnglish.btf` (the
   `tools/assets/btf.py` referenced in §17; not present in this checkout —
   rebuild or hand-parse) to get the label id, enabling the `GetString` retag
   route for a zero-risk display line.
5. **P4 — window chrome rects.** `C3D_PANEL2D::DrawFrom9Patch` argument order
   from a caller, so our drawn buttons match stock `$BUTTON` art.

---

## 9. Safety rules that apply to any of this

- **Never modify main files for the investigation** — this document only.
- Inline hooks need ≥14 position-independent stolen bytes; if the prologue is
  13 bytes then a RIP-relative `mov rax,[rip+…]`, patch the caller `call rel32`
  instead (rule 2). The loader memcmps prologue bytes and refuses on mismatch,
  so a game update degrades to "no rows/buttons", never a crash.
- **Render thread only.** Panel/menu drawing and widget polling run on the
  render thread. No large allocations, no blocking, no loader re-entry inside a
  provider. Bracket shared state with `g_lock`.
- Every game-memory dereference guarded by `TsmIsValidGamePtr()` +
  `ReadablePtr()`.
- Preserve ABI: new API fields appended at the end with a `structSize` guard
  (`TsmTaxesApi`, `TsmCitizenLaborApi` — do not reorder).
- Currency neutrality: `TsmCurrencyActive()` + dynamic `RUB`/`USD` symbol.
- `.ini` edits stay UTF-8 **without** BOM.

---

## 10. Recommended next step (smallest useful win)

Ship **native exe widgets first (§11)**: call slider `0x2727F0` + labelled
checkbox `0x3956F0` from the existing `ui_panels` post-hooks — no new builder
RVA, no `C3D_GUI*`, no custom hit-testing. Then run probe **P0** for the
Economy & Trade sidebar section reusing the same two calls. Full order §11.8.

---

## 11. 2026-10-11 Ghidra session — panel-native widgets (SUPERSEDES §§0/1/4/5 where in conflict)

One run at a time per `.clinerules` (project lock). All addresses file-based
(`0x140`+RVA). Raw outputs in `scratch/ui_probe_*`.

### 11.1 Slider `0x2727F0` — verified callable from a panel post-hook

- `DisasmMany 0x1402727F0:0x140272860` → `scratch/ui_probe_slider.asm`:
  `48 8B C4 / 48 89 58 08 / PUSH RBP RSI RDI R12–R15 / SUB RSP,0x110` —
  ordinary panel-widget frame; `MOVSS XMM14,[RSP+0x178]`,
  `MULSS XMM14,[0x14090A6A8]` (DPI-scaled geometry follows).
- `DecompAt 0x1402727F0 0x1403956F0` → `scratch/ui_probe_widgets.c`
  (`FUN_1402727f0`): `(game, x, y, textId, uint* value, scale, enabled,
  step, drag, hover) -> changed`. Verified: optional `GetString` label
  when `textId != -1` (font `0x995220`); **two −/+ buttons** via
  `C3D_INPUT::GetMouseSolid(0xA54B90)` +
  `C3D_PANEL2D::Collision(0x9BE060)` gated on `game+0xAD74 == -1`,
  `enabled`, click latch `0xA54E91` (consumed per click, `gui_click`
  sound); `*value ∓= step` (clamp ≥ 0); **0–100 drag bar**; returns 1 on
  the changed frame. Draw context `game+0xADB0/0xB498/0xB4A0`.
- `Xrefs 0x1402727F0` → `scratch/ui_xrefs_slider.txt`: **called from inside
  the panel builders we already hook** — `FUN_1407838E0` (construction, 2
  sites), `FUN_140799630` (factory), plus `0x270840` (6 sites), `0x389F10`,
  `0x395C90`, `0x233890`, `0x07C400`. Calling it from our post-hook is the
  same call the game makes one frame earlier.
- Reference call (Tesmio `cities.cpp:1011-1013`, live on v1.1.1.9):
  `o_Slider(game, left + dpi*g_widgetX, y, -1, &pct, dpi, live, g_step,
  live, NULL)`; `pct` 0–100; `live = window+0x20`. `pct != was` → commit.
  Typedef `cities.cpp:300-301`.

### 11.2 Labelled checkbox `0x3956F0` — verified, string label, no text id

- `DisasmMany 0x1403956F0:0x140395760` → `scratch/ui_probe_checkbox.asm`:
  `48 8B C4 / 48 89 58 18 / 48 89 70 20 / PUSH R14 / SUB RSP,0x90`, DPI
  `MOVSS XMM3,[0x140992088]`, draw-context `ADD RCX,0xADB0`, shared rect
  globals `0x9BE2E8/0x9BE2F0`.
- Decomp (`FUN_1403956f0` in `scratch/ui_probe_widgets.c`): `(game, x, y,
  uint* value, label wstr, colour, font, size, labelDy, live, p11, p12)
  -> hovered`. Verified: hit-test `GetMouseSolid+Collision`, gate `live
  (= window+0x20)`; on unconsumed latch plays `gui_click` and flips
  `*value = (*value == 0)`; draws box + `PrintLeftUnicode` label at
  `x + DPI*0x90A968` (null font → default `0x994200`). **Why this one, not
  `0x273AF0`:** per `cities.cpp:147-160` the plain checkbox is also gated
  on `game+0xAD74 == -1` **and consumes the latch**, so an earlier slider
  starves it; the labelled one needs only `hit && live` and leaves the
  latch alone.
- `Xrefs 0x1403956F0` → `scratch/ui_xrefs_checkbox.txt` (~140 sites incl.
  **every builder we hook**: residence `0x761300`, pub `0x723950`, factory
  `0x799630`, construction `0x7838E0`, city window `0x7CEF60`, plus menu
  candidate `0x265500`). Panel-native by construction.
- Companion `0x3959D0` (`scratch/ui_probe_textbox.asm`): thin wrapper —
  `CALL 0x3956F0`, then `SETNZ CL / MOV [RBX],CL` (1-byte bool adapter).
  We call `0x3956F0` directly with a `uint`.
- Reference call (`cities.cpp:1030-1034`): `o_Checkbox(game,
  left + dpi*g_labelX, y + dpi*g_checkDy, &sq, L"Square city area",
  ROW_COLOUR, NULL, LayoutF(0x90A928 /*20*/), 0.0f, live, 1, 0)`; the
  widget **draws ~25 units above the `y` handed in** (`0x90A968`), hence
  `g_checkDy = 25.0` pushes it back onto its row. Typedef §300-308.

### 11.3 What this unlocks for taxes + wages (no new hook, no C3D_GUI)

- **Wage −/+:** one slider row per building window, `uint pct` mapped to
  `wage = min + pct/100 * (max - min)` (or step-count onto
  `g_wageAdjustStep`), committed via existing
  `Api_SetWorkplaceWageOverride` / `Api_SetSectorWage`. Replaces `[`/`]`;
  drag + −/+ buttons come free.
- **Tax −/+ per category:** same slider per tax row (VAT/profit/property),
  committed via existing `SetTaxRateImpl`. Replaces `[`/`]` + `T` cursor.
- **Scope + tax on/off:** labelled-checkbox rows (`L"Whole Sector"` /
  `L"VAT enabled"` …), committed via `g_scopeMode` / `SetTaxEnabledImpl`.
  Takes a **wide string**, so no `≥1,000,000` private text id is needed for
  the widget itself (rule 4 still applies to any `GetString` ids we mint).
- **Rows/hook budget:** each widget consumes one `DPI*35` row + a
  `window+0x250` advance — the existing `AppendRowsImpl` flow. City Hall
  already renders 6 rows; check `TSM_PANEL_MAX_ROWS = 12` headroom before
  adding 3 sliders + 3 checkboxes (or page behind the `T` cursor:
  selected-category slider + master enable checkbox = 2 rows).
- **Gating recipe (copy `cities`):** `live = window+0x20`; skip while
  `game+0xAD74 != -1`; commit-on-change only (`pct != was`); defer
  expensive applies until `P_MOUSE_DOWN (0xA54E90) == 0`. Render thread,
  `g_lock`, `ReadablePtr` on every game dereference.

### 11.4 City-window head `0x7CEF60` — prologue re-verified for the record

`DisasmMany 0x1407CEF60:0x1407CEFC0` → `scratch/ui_probe_citywin.asm`:
`48 8B C4 / PUSH RBP RSI RDI R12–R15 / LEA RBP,[RAX-0x158] /
SUB RSP,0x220` — matches `cities.cpp` `kWindowPrologue`
(`48 8B C4 55 56 57 41 54 41 55 41 56 41 57`) on the steal. Direct
`call`-by-RVA (as `cities` does for slider/checkbox) needs no prologue
steal; only a future *post-hook* on a new builder would.

### 11.5 Correction: the exe imports zero `C3D_GUI` symbols (draft misread)

The 2026-10-04 draft's "0 `C3D_GUI` entries" line was correct but then
contradicted by §1's "verified imported" `C3D_PANEL2D` paragraph. Verified
this session: `Select-String -Pattern 'C3D_GUI' SOVIET64_exe.imports.txt`
→ **no matches** (the earlier `Measure-Object` count echoed the PowerShell
pipeline, not a match count). What the exe **does** import (verified):
`C3D_PANEL2D::Collision/CollisionForSize/BeginDraw/Draw/EndDraw/...`,
`C3D_INPUT::GetMouse*`/`CommonInterface*`,
`C3D_LANGUAGE::GetString`,
`C3D_FONTMANAGER::PrintLeftUnicode(NoArg)`. `C3D_GUI::AddButton/AddSlider/
AddTextBox/...` live **only** in `C3DDLL64_dll.exports.txt`
(engine-internal, skins in `media_soviet/gui/default.ini`), with no exe
import and no located live instance. Path B stays a stretch goal behind
probe P1; **the ship path is §11.1–11.3.**

### 11.6 Sidebar / Economy & Trade — narrowed, not yet closed

- `DecompAt 0x140265500` → `scratch/ui_probe_menucand.c` (1270 lines): an
  options-style menu calling the same checkbox `0x3956F0` repeatedly with
  `GetString` labels `0x9C60/0x9CA7–0x9CA9/0x9EFF`, the same
  `GetMouseSolid + Collision(0x9BE060)` + latch `0xA54E91` idiom, and
  hover-copy `GetString(0x266/0x12D/0x12E/0x26A/0x26B/…)`. It proves the
  widget idiom outside building panels — but its ids sit in the `0x9Cxx`
  options block, **not** the Economy/Trade tab, so it is *not* the stats
  builder.
- The stats/Economy & Trade builder is still PROBE P0: start from the
  `trade_productionconsumption / trade_importexport / trade_tourism /
  trade_crime` texture strings (§6 evidence) and xref their data addresses
  to the builder, then confirm `window+0x240/+0x250` + ≥14-byte PI
  prologue. The `GetString`-retag fallback needs the tab's text id, still
  PROBE P3 (`sovietEnglish.btf` decode).
- "Another mod did the sidebar" is **unconfirmed**: `reputation` v1.38 is
  overlay (F9/F10/F11) + `GetString` vehicle-list retag — no sidebar hook;
  Construction-Market-Estimate is a tooltip overlay (`0x7ABD80`
  post-hook). No third-party sidebar-section precedent was found in
  `Other mods by other people\`.

### 11.7 Typed numeric entry — verdict: no panel-native textbox found

`ListFuncs 0x140395000–0x140398000` → `scratch/ui_funcs_widgets.txt`: only
`0x3956F0` (checkbox), `0x3959D0` (bool-wrapper), `0x395A70` (second small
wrapper), and the 55 KB `0x395C90` panel function. **No textbox/line-edit
widget in the panel family.** The engine `C3D_GUI::AddTextBox +
GetElementTextFloat` path needs the still-unproven live `C3D_GUI*` (P1).
Ship options today: (a) slider + checkboxes (§11.3 — no typing); (b) typed
entry via **HUD overlay + keyboard capture** (`ui_overlay` drawer +
`GetAsyncKeyState` digit accumulation, Enter commits, Esc cancels —
`reputation` F9-window precedent); (c) defer typing to Path-B once P1
lands. (a) covers +/− and toggles; typing stays a follow-up, not a
blocker.

### 11.8 Revised build order (supersedes §10)

1. **City Hall fiscal panel → native widgets (no new hook).** One slider
   row for the `T`-selected tax + one master `[ON/OFF]` checkbox row (2
   rows, fits `max_rows`), or full 3×(slider+checkbox) if
   `TSM_PANEL_MAX_ROWS` headroom allows. Reuses `cityhall 0x6F4740` hook.
2. **Wage panel → same treatment.** One slider (workplace/sector-resolved
   wage) + scope checkbox; keep `T` as keyboard mirror. Reuses existing
   shop/pub/factory hooks.
3. **P0 sidebar probe** (stats/Economy & Trade builder) → tax section as a
   post-hook reusing §11.1–11.3 widget calls.
4. **Typing (follow-up):** overlay digit-capture or Path-B `C3D_GUI`
   `AddTextBox` once P1 lands — not required for buttons/sidebar.

### 11.9 PROBE P0 CLOSED — the sidebar builder is `FUN_140296110`

**How it was found (repeatable recipe).** `peinfo.py strings` proves a string
exists but prints no address, so it cannot be xref'd. Two new tools close that
gap:

- **`scratch/strrva.py <exe> <needle...>`** — maps a needle's file offset back
  to an RVA through the section table, and prints the Ghidra query address
  (`0x140`+RVA). Prints the *containing* string too, so a prefix shared by
  several strings can be disambiguated.
- **`scratch/StrAddr.java <out> <needle...>`** — Ghidra memory search for the
  needle in **both ASCII and UTF-16LE**, printing each data address plus its
  code references and containing function.

**Verified chain (one Ghidra run per line, sequential):**

| Step | Tool | Result |
|---|---|---|
| Resolve addresses | `strrva.py` | `editor/trade_importexport.png` → `0x14089AE00` (and `…AE20`); `editor/trade_productionconsumption.png` → `0x14089AEC0` (and `…AEE8`); `editor/maintab_%s.png` → `0x140899BD0`; `maintab_%s` → `0x1408B23D8` |
| Xref the **string starts** | `Xrefs 0x14089AEC0 0x14089AE00 0x140899BD0 0x1408B23D8` → `scratch/ui_xrefs_trade2.txt` | all four → **`FUN_140296110`** (from `0x140297cf1`, `0x140297caf`, `0x140296317`) |
| Bound it | `ListFuncs 0x140294000–0x14029A000` → `ui_funcs_side.txt` | `0x140296110` size **42,071**; neighbours `0x294C00` (4,814), `0x295ED0` (372), `0x296050` (187) |
| Prologue | `DisasmMany 0x140296110:0x1402961C0` → `ui_probe_sidebar.asm` | see below — **16 clean PI bytes** |
| Tab textures | `DisasmMany 0x140297C60:0x140297D80` → `ui_probe_sidetabs.asm` | see below |
| Callers | `Xrefs 0x140296110` → `ui_xrefs_sidebar.txt` | `FUN_14028c770` @ `0x140294092`, `FUN_140094290` @ `0x1400970a0` |

> **Trap worth recording:** xref'ing the *needle offset* (`0x14089AEC7`, i.e.
> `trade_…` inside `editor/trade_…`) returned **no references**. Code takes the
> address of the string **start**. `strrva.py` prints both so the start is used.

**Prologue — safe to hook (16-byte steal, rule 2 satisfied):**

```
140296110: 4C 8B DC                MOV R11,RSP              (3)
140296113: 55 53 56 57             PUSH RBP RBX RSI RDI     (4)  -> 7
140296117: 41 55                   PUSH R13                 (2)  -> 9
140296119: 49 8D AB 18 FC FF FF    LEA RBP,[R11-0x3E8]      (7)  -> 16
140296120: 48 81 EC C0 04 00 00    SUB RSP,0x4C0            <- steal ends here
140296127: 48 8B 05 1A BF 6F 00    MOV RAX,[0x140992048]    <- /GS cookie, AFTER
14029612e: 48 33 C4                XOR RAX,RSP
```

Expect bytes: `4C 8B DC 55 53 56 57 41 55 49 8D AB 18 FC FF FF`. All
position-independent, no RIP-relative operand, and the cookie load sits at
`+0x17` — outside the window. Ends exactly on an instruction boundary.

**Signature and the data it owns (from the prologue):**

```
FUN_140296110(void* game)          ; RCX -> saved to RSI, used all function
  count = (*(void**)(game+0xC2B8) - *(void**)(game+0xC2B0)) / 0x348
                                 ; IMUL magic 0x4EC4EC4EC4EC4EC5 + SAR 8 = signed /840
  for (i = -2; i < count; ++i)    ; -2 and -1 are aggregate entries, then per record
```

So the sidebar is **game-object driven, not window driven**. It walks a
840-byte record vector at `game+0xC2B0` and prepends two aggregate entries
(`-2`, `-1`) — consistent with a statistics/category menu.

**Tab textures are cached in an array at `game+0xB650`, 8-byte stride**
(`ui_probe_sidetabs.asm`):

| Offset | Texture loaded |
|---|---|
| `game+0xB650` | `editor/maintab_storage.png` |
| `game+0xB658` | `editor/trade_importexport.png` |
| `game+0xB660` | `editor/trade_productionconsumption.png` |
| `game+0xB668` | `editor/trade_tourism.png` |
| `game+0xB670` | (next entry) |

Per-entry idiom, verified twice over: store the texture pointer at
`[RSI+0xB65x]`, then a virtual `CALL [R10+0x10]` with `RDX` = the path string,
then `LEA RCX,[0x1409EACD0]` (`C3D_MIDDLEPOINT`) with `CALL [0x14086C4C0]`
through the IAT. Adjacent `.rdata` (`dumprva.py 0x89AE00`) confirms the sibling
string block: `trade_importexport`, `maintab_storage`, `trade_tourism`,
`trade_productionconsumption`, `icon_subscribed`, `icon_wip`.

**Why this changes the sidebar plan.** The `window+0x240` / `window+0x250`
append contract in §2 **does not apply** — there is no window pointer here.
But that costs nothing: the two proven widgets take `game` as their first
parameter as well (`o_Slider(game, …)`, `o_Checkbox(game, …)`), so a post-hook
on `0x296110` has the exact pointer both need. A post-hook is the right shape;
only the geometry source has to be rediscovered.

### 11.10 Still open before the sidebar can be built

Two unknowns, both cheap `DisasmMany` runs inside `0x296110` (42 KB, so
**`DecompAt` will likely hit the 120 s budget** — use `DisasmMany` on ranges,
per `.clinerules`):

1. **P5 — the visibility gate.** Which byte/field says the stats menu is
   actually open, so the section is drawn only then and not over the map.
   Find it by locating the `GetMouseSolid`/`Collision` cluster in the tab-draw
   loop and reading what guards it.
2. **P6 — the row origin.** Where the sidebar's own rows start (the `x`/`y`
   the game uses for its list), so the fiscal section can be placed inside the
   panel rather than at a guessed offset. Look for the `PrintLeftUnicode` /
   `PrintCenterUnicode` call sites and the running-Y they advance.
3. **P7 — tab click handling.** How a tab press is latched, so a new
   "Fiscal Policy" tab (or an entry under Economy & Trade) can be made to
   select without stealing a vanilla tab. Note the `gui_maintab_click` string
   at `0x14087FAE8` is referenced by `FUN_14009DC80` — that is the sound/latch
   path, not the layout.

**Do not** attempt to widen the `game+0xB650` texture array by guesswork: the
stride is 8 bytes and the array's bound is unknown, so writing past its end
would corrupt neighbouring game state.

### 11.11 Revised build order (supersedes §11.8)

1. **City Hall fiscal panel → native widgets (no new hook).** Unchanged from
   §11.8 — slider + checkbox rows, reusing `cityhall 0x6F4740`. Still the
   fastest shippable win.
2. **Wage panel → same treatment**, reusing shop/pub/factory hooks.
3. **Sidebar section (P5/P6 first).** Post-hook `FUN_140296110`
   (`0x296110`, 16-byte prologue, `game` in RCX), append the fiscal rows and
   call `0x2727F0` / `0x3956F0` with the same `game`. This is now a bounded
   task rather than an open question.
4. **Typing (follow-up):** overlay digit-capture or Path-B `C3D_GUI`
   `AddTextBox` once P1 lands — not required for buttons/sidebar.

### 11.12 `FUN_140296110` is a texture LOADER — correcting §11.9

`CallsIn.java` over the whole function (`scratch/ui_calls_sidebar.txt`, 1,263
calls) settles what §11.9 could only guess at:

| Target | Calls |
|---|---|
| `CreateManagedTexture` | **619** |
| `sprintf` | 9 |
| `GetSound2DForLoop` | 3 |
| `strstr` | 3 |
| `C3DHelp_CheckIfFileExist` | 2 |
| `LoadFromFile` / `CreateManagedMesh` / `ControlImageInitialize` | 1 each |

There is **no** `Print*`, no `Collision`, no `GetMouseSolid`, no `Draw`. So
`FUN_140296110` does not draw the sidebar — it **loads the GUI textures** into
the array at `game+0xB650` (hence 619 calls: hundreds of slots, not eight
tabs). §11.9's "called every frame from `FUN_14028c770`" remains true as an
xref fact, but the function is a loader that the frame update guards, not the
per-frame draw. The hook target must be whatever *reads* the array.

### 11.13 P5 + P6 CLOSED — the consumer is `FUN_14072DBF0`, and it is a window panel

A struct field is named by a displacement, so `Xrefs.java` cannot find its
users. **`scratch/FindDisp.java`** scans `.text` for the disp32 and keeps hits
that land inside a real instruction:

```
FindDisp b650  ->  scratch/ui_finddisp_b650.txt   (2 hits)
  0x140297C74  in FUN_140296110   MOV qword ptr [RSI + 0xb650],RAX  <- loader WRITE
  0x14072EC27  in FUN_14072DBF0   MOV RCX,qword ptr [RDI + 0xb650]   <- consumer READ
```

Exactly one reader: **`FUN_14072DBF0` (RVA `0x72DBF0`, 5,845 bytes)**.
`CallsIn` over it (`scratch/ui_calls_tabbar.txt`, 57 calls) shows a real
interactive panel: `GetString` x18, `PrintLeftUnicode` x3,
`PrintCenterUnicode` x1, `C3D_PANEL2D::Draw` x2, and **`GetMouseSolid` +
`Collision`** for hit-testing.

**It uses the standard window contract our hooks already implement**
(`scratch/ui_probe_tabbar2.asm`, `ui_probe_tabbar3.asm`):

```
14072dbf0: 4C 8B DC                MOV R11,RSP           (3)
14072dbf3: 55 56 57                PUSH RBP RSI RDI      (3) -> 6
14072dbf6: 41 54 41 57             PUSH R12 R15          (4) -> 10
14072dbfa: 49 8D AB 98 F5 FF FF    LEA RBP,[R11-0xA68]   (7) -> 17  <- steal ends
14072dc01: 48 81 EC 40 0B 00 00    SUB RSP,0xB40
14072dc08: MOV RAX,[0x140992048]                         <- /GS cookie, AFTER

14072dc19: CMP byte ptr [RDX+0x1],0x0 / JNZ 0x14072f2a7  <- P5 GATE: window+0x01
14072dc21: MOV R15,[RDX+0x240]                           <- window+0x240 = building
14072dc32: MOV R12,[R15+0x318]                           <- building+0x318 = typedesc
14072dc28: MOV RSI,RDX                                   <- window
14072dc2f: MOV RDI,RCX                                   <- game
14072dcb9: MOVSS XMM14,[RDX+0x4]                         <- window+0x04 = x
```

**17 clean position-independent bytes**
(`4C 8B DC 55 56 57 41 54 41 57 49 8D AB 98 F5 FF FF`), cookie load at `+0x18`,
outside the window. Rule 2 satisfied.

**P6 - the row origin is byte-identical to `ui_panels`:**

```
MOVSS XMM14,[RDX+0x4]                            ; window+0x04  (W_X)
ADDSS XMM14,[RDX+0x28]                           ; + window+0x28 (W_OFFX)
MULSS XMM0,[0x14090A9CC] / ADDSS XMM14,XMM0      ; + DPI*35 (LAYOUT_35)
MOVSS XMM0,[RDX+0x2c] / ADDSS XMM0,[RDX+0x8]     ; W_OFFY + W_Y
MULSS XMM1,[0x14090AB98]                         ; DPI*110 (indent)
```

That is the same `window[4] + window[0x28] + DPI*(35 + 110 - 15)` expression
`ui_panels.cpp` already implements. **No new geometry model is needed** - the
existing `LayoutF(RVA_ROW_STEP / RVA_X_INDENT / RVA_X_BACK)` helpers apply
unchanged.

Also verified here: `live = window+0x20` (`CMP byte ptr [RSI+0x20],0x0 / JZ`),
tooltip buffer at `window+0xD5A0`, clip context `game+0xADB0` via
`FUN_1404461F0`, and hover labels picked by an `EBX` selector through
`GetString(0x4B0 / 0x514 / 0x5E1 / 0x5E7)`. It walks `building+0xBC0`
(`LEA R13,[R15+0xBC0]` = `TSM_BLD_RESIDENTS_BEGIN`) and pushes rows into a
vector at `window+0x580`.

**Consequence:** the panel that displays the sidebar/tab texture is an ordinary
building window following the exact contract of the five hooks already
installed. Appending a fiscal section here needs **no new geometry and no
`C3D_GUI*`** - same post-hook, same `window+0x250` bottom edge, same two widget
calls with `game`.

**Not yet isolated:** the building-type filter (`typedesc` is loaded into `R12`
but the type compare was outside the ranges read), so which building type this
panel serves is unconfirmed. That gate must be known before adding rows, or
they would appear on every building routed through this builder.

### 11.14 New tools added this session

| Tool | Purpose | Why it was needed |
|---|---|---|
| `scratch/strrva.py` | needle -> file offset -> RVA -> `0x140`+RVA query address | `peinfo.py strings` gives contents but no addresses, so nothing could be xref'd |
| `scratch/dumprva.py` | hexdump around an RVA, ASCII-decoded | recognises a pointer table vs a run of sibling strings |
| `scratch/StrAddr.java` | Ghidra memory search, **ASCII + UTF-16LE**, with code xrefs | `maintab_*` paths are stored differently than `gui_*` strings; one run now covers both |
| `scratch/CallsIn.java` | every call in a range, target named (direct **and** IAT) | `DecompAt` times out on 42 KB; this answers "what does it call" in ~5 s |
| `scratch/FindDisp.java` | find users of a struct-field **displacement** | `Xrefs.java` cannot find `game+0xB650`; a field has no address |

Two API traps cost a run each and are worth knowing: `Instruction` has **no**
`getReferences()` (use `ReferenceManager.getReferencesFrom(addr)`), and
`ExternalManager.getExternalFunction(Address)` does not exist — but
`FunctionManager.getFunctionAt(externalAddress)` returns the external function,
whose name is the full decorated C++ symbol. `FindDisp` must use
`getInstructionContaining`, because a displacement sits *after* the opcode, and
must emit the disp32 **little-endian** (`0xB650` -> `50 B6 00 00`).

### 11.15 Probe status

| Probe | Status |
|---|---|
| P0 sidebar builder | **Closed** — `FUN_140296110` (loader) + `FUN_14072DBF0` (the panel that draws it) |
| P5 visibility gate | **Closed** — `window+0x01`, plus `window+0x20` as the input gate |
| P6 row origin | **Closed** — identical to `ui_panels`: `window+0x04 + window+0x28 + DPI*35`, indent `DPI*110` |
| P7 tab click latch | Open — `EBX` selector + `GetString` hover ids found; the write that changes the selected tab was not isolated |
| Type filter of `0x72DBF0` | Open — must be known before adding rows there |
| P1 live `C3D_GUI*` | Open — still the blocker for typed entry |

### 11.16 CLOSED — the type filter for `FUN_14072DBF0` is BUILDINGTYPE 39, City Hall

**Route taken: the caller, not the callee.** `Xrefs 0x14072DBF0`
(`scratch/ui_xrefs_tabpanel.txt`) gives exactly one calling function and two
sites, both inside the central window dispatcher:

```
[UNCONDITIONAL_CALL] from 1406ea8ef in FUN_1406e9b80
[UNCONDITIONAL_CALL] from 1406eafc7 in FUN_1406e9b80
```

`DisasmMany` around both (`scratch/ui_probe_dispatch.asm`) shows the guard is
a plain compare on `ECX`:

```
1406eaf8b: CMP ECX,0x4    JNZ 0x1406eafae     ; type 4  -> 0x14072F2D0
1406eafae: CMP ECX,0x27   JNZ 0x1406eafd4     ; type 39 -> 0x1406F8CC0, then
1406eafc7: CALL 0x14072dbf0                   ;           0x14072DBF0  <-- ours
```

**`ECX` is the building type — proven from the same dispatcher, not assumed.**
Widening the dump (`scratch/ui_probe_dispatch2.asm`) exposes sibling compares
whose targets are builders this project already names:

| Compare | Type | Builder called | Known as |
|---|---|---|---|
| `CMP ECX,0x3` | 3 | `0x1407272F0` | **shop** — `ui_panels.cpp` hook `{ "shop", 0x7272F0 }` |
| `CMP ECX,0x17` | 23 | `0x140723950` | **pub** — `ui_panels.cpp` hook `{ "pub", 0x723950 }`; and `citizen_labor.cpp` v1.5: *"Building type 23 is a pub"* |
| `CMP ECX,0x27` | 39 | `0x14072DBF0` | **City Hall / Party HQ** |

`CMP ECX,0x17` (23) landing on the pub builder matches `citizen_labor.cpp`'s
independently-derived "type 23 is a pub" exactly, so the register is settled
beyond doubt.

**Answer:** `FUN_14072DBF0` is the **City Hall / Party HQ** panel,
**BUILDINGTYPE 39 (`0x27`)**. Name from
`media_soviet\scripts\SOVIETInstructions.txt:1798` —
`defineVariable(int, BUILDINGTYPE_CITYHALL);`

Three independent confirmations of 39 = City Hall:

1. this dispatcher compare (`0x27`);
2. `plugins/taxes/taxes.cpp` — `#define TSM_TYPE_CITYHALL 39`, *"City Hall /
   Party HQ (building type 39)"*, the filter the shipped fiscal panel uses;
3. Tesmio `cities.cpp:86-87` — *"a working `$TYPE_CITYHALL` (building type
   `0x27`) in the city makes the dispatcher draw that building's own panel
   inside the city window as well"*, which is precisely the **second** call
   site: the branch at `0x1406EA8BD` walks a window array
   (`MOV R14,[R11+R10*8]`), sets `window+0x240 = R14` and `window+0x248 = 0`,
   then calls `0x14072DBF0`.

**Consequence — the sidebar texture and the tax dashboard are the same window.**
`FUN_14072DBF0` is the panel that reads `game+0xB650` (§11.13) *and* it is the
City Hall window *and* it is where `taxes.cpp` already renders its fiscal
dashboard through `ui_panels`. So adding slider/checkbox rows there puts native
controls directly into the panel the user already opens for taxes.

**Flagged discrepancy — worth verifying before any code change.**
`ui_panels.cpp` currently registers its fiscal rows against
`{ "cityhall", 0x6F4740 }` (from docs/18 G4, described as a generic fallback
drawing ~60 types, type-filtered to 39 in software). But the dispatcher routes
type 39 to **`0x72DBF0`**, and `CallsIn` over `0x72DBF0`
(`scratch/ui_calls_tabbar.txt`) shows **no call to `0x6F4740`**. The two are
different functions. Either `0x6F4740` is reached by another path for this
window, or the existing `cityhall` hook is aimed at the wrong builder. This is
recorded as an open question, **not** as a bug — no runtime observation has
been made, and the shipped panel may well work via a path not yet traced.
`0x6F8CC0`, called immediately before each of these builders, is a shared
helper (window chrome), not a type filter.

---

## 12. Pivot — the global Economy & Trade sidebar (session 2026-10-11)

City Hall (§11.16) is a *building* popup, so it is the wrong home for national
fiscal policy. This section records what the pivot established, including two
of the three starting clues that turned out to be **wrong**.

### 12.1 The bottom toolbar / sidebar tab system — found and mapped

`FUN_1404d72c0` (**RVA `0x1404D72C0`, size 7,553**) is the bottom toolbar tab
bar. `CallsIn` (`scratch/ui_calls_toolbar.txt`, 99 calls) shows exactly the
tab-bar idiom, repeated per tab:

```
GetMouseSolid + Collision          hit-test the tab rect
GetValueExist + GetValueString     look up the click sound
PlaySound2D                        gui_maintab_click / gui_subtab_click
Draw                               the tab background
PrintLeftUnicode / PrintAllTexts   the tab caption
CreateManagedTexture               the tab icon
GetValueInteger                    config reads
```

It references `gui_maintab_click` six times (`0x1408DA040/58/A0/D0/F0/180`).
On a click it calls `FUN_1404d9050`.

### 12.2 Tab state object layout — from `FUN_1404d9050` (167 bytes)

`DisasmMany 0x1404D9050:0x1404D9100` (`scratch/ui_probe_tabhelpers.asm`) is the
whole tab-switch setter:

```
MOV  R11,RCX                     ; the tab-bar object
MOV  RCX,[RCX + 0x38]            ; -> array of panel pointers
;   scan 10 slots (CMP R9,0xa) for RDX and zero the match
MOV  [RAX + RCX*0x8], RDX        ; store the panel at index R8D
MOV  byte  [R11], 0x1            ; active flag
MOV  dword [R11 + 0x68], R8D     ; CURRENT TAB INDEX
```

| Field | Meaning |
|---|---|
| `obj+0x00` | active flag (1 while the bar is in use) |
| `obj+0x38` | pointer to a **10-element array of panel pointers** |
| `obj+0x68` | **current tab index**, 0..9 |

**This is the "main sidebar" the request describes, and it holds at most ten
panels.** `FUN_1404d92c0` (755 B) is *not* a builder — it reads
`window+0x248 == 1`, `window+0x240` (a building), `typedesc+0x360 == 2`
(`BUILDINGTYPE_LIVING`) and counts residents, i.e. a tooltip evaluator.

### 12.3 Loans are load-time state, not a per-frame UI flow

Localization keys verified in the exe and resolved with `strrva.py`:

| Key | Address |
|---|---|
| `$LoanStart` | `0x1408AD3E8` |
| `$Loan_Ballance_USD` | `0x1408AD8E8` |
| `$Loan_Interest_USD` | `0x1408AD920` |

(`$Loan_Ballance_USD %f` / `$Loan_Interest_USD %f` do **not** NUL-match as whole
strings — the `%f` is a separate literal. Searching the bare key works; the
sibling `$YearInterestRate` and `$LoanSubType` are not stored as standalone
NUL-terminated copies in this build.)

All three are referenced by exactly one function, `FUN_1402ff840`
(`0x1402FF840`), and `Xrefs 0x1402FF840` gives exactly one caller:

```
[UNCONDITIONAL_CALL] from 140433faa in FUN_140430fc0
```

`FUN_140430FC0` is the **treasury reset** (§F6) — world load / new game, plus
vtable callbacks at `0x140954090` and `0x140A69B64`. So loans are initialised
and settled as state; **the loans UI reads persisted fields and does not call
the loan logic per frame.** Chasing the loan strings leads to the model, not
the window.

### 12.4 REFUTED — the Economy & Trade window does not read `game+0x580/0x588`

The starting brief asserted the dashboard reads the treasury doubles directly.
The complete reader set is the 11 functions in `scratch/v3_xrefs_treasury2.txt`
(80 xref lines, grouped by function):

| Reads | Function | What it is |
|---|---|---|
| 22 | `FUN_14068B330` | revenue **crediter** — it *writes* the treasury |
| 10 | `FUN_14060B8E0` | build/contract affordability gate |
| 7 / 6 / 6 / 3 | `FUN_14060E4D0` / `14060E940` / `14060F700` / `14060C450` | same gate family |
| 7 / 5 / 4 / 4 / 2 | `140334620` (seeder) / `140430FC0`, `1401AAD70`, `1404FCAF0`, `140430690` | load, reset, purchase paths |

**None is a dashboard builder.** Every treasury read is either a purchase
affordability check, a reset/seeder, or the crediter that adds revenue. So the
`game+0x580/0x588` clue does not point at the Economy & Trade window, and a
fiscal readout there would have to use the derived stats instead
(`0x1409E63C0/C4` session spend, `0x1409E2280`).

### 12.5 REFUTED — the trade crediter's callers are not the trade windows

`Xrefs 0x14068B330` gives 8 code callers. `CallsIn` on the four largest shows
they are simulation/3D code, not UI:

| Callee | Size | Dominant calls | Actually is |
|---|---|---|---|
| `FUN_140699420` | 41,279 | `C3DVECTOR3` x130, `GetMatrix`/`GetMatrixInv`, `C3DVec3TransformCoord`, `CollisionBox` x18, `SpawnParticles` | placement / geometry |
| `FUN_1406A7520` | 20,919 | `PowerTime` x21, `Power`, `SetVolume`, `Stop`, `Refresh`, `GetOrientation` | vehicle / power / audio |
| `FUN_1406DE4A0` | 13,441 | `C3D_NODE` x16, `CreateFromPositionRotationScale`, `GetMatrix` | 3D transform |

§F6's note that all ten call sites are "in the `0x66xxxx-0x6Exxxx` window/UI
family" is **not** supported — that range also holds simulation code (the
window dispatcher `0x1406E9B80` merely sits at its top). The crediter is a
money helper used by many flows.

### 12.6 Honest status — the dashboard builder is NOT yet pinned

The deliverable asked for the RVA, size and prologue of the national Economy &
Trade window builder. **That was not reached, and two of the three supplied
clues were refuted above rather than followed.** What *is* established is the
container the panel lives in (§12.1–12.2): a ten-slot tab bar whose current
index is `obj+0x68`.

Remaining leads, in the order worth trying:

1. **Find the tab dispatcher.** `FUN_1404d9050` *writes* `obj+0x68`; something
   *reads* it and calls the matching panel builder. The tab object arrives in
   `RCX` (heap), so `FindDisp 0x68` is too noisy to be useful. Instead
   `Xrefs 0x1404D72C0` to find the per-frame owner, then `CallsIn` that owner
   and look for the branch on the tab index.
2. **The `trade_*` sub-tab strings.** `trade_productionconsumption`,
   `trade_importexport`, `trade_tourism`, `trade_crime` are referenced *only* by
   the texture loader `FUN_140296110` (§11.12), so their data addresses will not
   find the panel. The panel selects them by index. Resolve the sub-tab *sound*
   `gui_subtab_click` and xref it instead — that is fired from the panel that
   owns the sub-tabs.
3. **`FUN_14066D530` (6,302 B)** and `FUN_1406C07E0` (3,635 B) — the two
   crediter callers not yet inspected. Small for a dashboard, but cheap to rule
   out with one `CallsIn` each.

### 12.7 Strategic note for the actual goal

The tax dashboard does not have to live inside a vanilla panel at all. §12.2
shows the sidebar is a **ten-element array of panel pointers** at `obj+0x38`
that `FUN_1404d9050` writes into by index. If a plugin can register its own
panel into that array, it gets a first-class sidebar tab holding the fiscal
controls — which is what the request actually asks for, and it avoids
depending on the vanilla Economy layout at all. That route needs the owner of
the tab object (lead 1) before it can be judged.

Everything in §11.1–11.3 still stands independently of this pivot: the native
slider `0x2727F0` and labelled checkbox `0x3956F0` are callable from any
post-hook that has `game`, so the *controls* are solved even while the *host
panel* is still open.

### 12.8 Raw outputs from this pivot

| File | Content |
|---|---|
| `scratch/ui_xrefs_loan.txt`, `ui_xrefs_loan2.txt` | xrefs to the `$Loan_*` keys |
| `scratch/ui_xrefs_loanlogic.txt` | xrefs to `FUN_1402ff840` (one caller) |
| `scratch/ui_xrefs_crediter.txt` | the 8 callers of `FUN_14068B330` |
| `scratch/ui_funcs_trade.txt` | 344 functions, `0x140660000–0x1406E0000` |
| `scratch/ui_calls_econ.txt` | `FUN_140699420` — 674 calls (3D placement) |
| `scratch/ui_calls_trade2.txt` | `FUN_1406A7520` — 185 calls (vehicle/power) |
| `scratch/ui_calls_trade3.txt` | `FUN_1406DE4A0` — 140 calls (3D transform) |
| `scratch/ui_funcs_toolbar.txt`, `ui_calls_toolbar.txt` | tab bar `FUN_1404d72c0` |
| `scratch/ui_probe_tabhelpers.asm` | `FUN_1404d9050` + `FUN_1404d92c0` |

---

## 13. Tab dispatcher & sub-tab system (session 2026-10-11, leads 1 + 2)

**Headline: the sidebar is not ten panel builders. It is one 84,735-byte
function** that draws the tab bar, the tab icons and the sub-tabs itself.

### 13.1 Lead 1 — who owns the tab bar

```
Xrefs 0x1404D72C0  (tab bar)   -> ONE code caller: FUN_140317db0 @ 0x140319E42
Xrefs 0x1404D9050  (setter)    -> FUN_1404d6df0 x10, FUN_1404d72c0 x2
```

`FUN_1404d6df0` is the **hotkey -> tab mapper**. `DisasmMany 0x1404D6F80:
0x1404D70E0` (`scratch/ui_probe_register.asm`) is an unrolled chain of ten:

```
CMP byte ptr [0x140a54c92],SIL / JZ next / XOR R8D,R8D  / CALL 0x1404d9050  ; idx 0
CMP byte ptr [0x140a54c93],SIL / JZ next / MOV R8D,0x1  / CALL 0x1404d9050  ; idx 1
...
CMP byte ptr [0x140a54c9b],SIL / JZ next / MOV R8D,0x9  / CALL 0x1404d9050  ; idx 9
```

| Global | Meaning |
|---|---|
| `0x140A54C92 .. 0x140A54C9B` | **ten key-state bytes, one per tab index 0..9** |
| `0x1409E2338` | the panel pointer handed to the setter (`RDX`) |
| `0x140A54CCF` | the cancel/close key |

Close path: `CMP byte [0x140a54ccf],SIL` -> `MOV dword [RBX+0x68],0xffffffff`
(index **-1** = nothing selected).

### 13.2 The tab-name loop — index to icon, and the panel struct

`DisasmMany 0x14031FF40:0x140320060` (`scratch/ui_probe_tabnames.asm`), inside
`FUN_140317db0`:

```
MOV RAX,[RBP+0x30]            ; the 10-slot panel array
MOV R8,[RAX + RSI*0x8]        ; panel[RSI]        <- RSI IS the tab index
INC R8                        ; skip +0x00 (the active flag)
LEA RDX,[0x1408b23d8]         ; "maintab_%s"
CALL 0x140005340              ; sprintf -> "maintab_<name>"
...
CMP qword ptr [R14+0xc650],RCX ; game+0xC650 == the SELECTED panel?
JNZ skip                       ; not selected -> no highlight
```

So each panel is a tiny struct: **`+0x00` active flag, `+0x01` a NUL-terminated
name**, and its icon is `editor/maintab_<name>.png`. `game+0xC650` holds the
currently selected panel pointer.

Confirmed name literals in the exe (standalone ASCII): `population`,
`resources`, `importexport` — i.e. `maintab_population.png`,
`maintab_resources.png`, `trade_importexport.png`.

**There is no switch/jump table over ten builders.** `FindDisp 0x68` returned
219 hits binary-wide and **none inside `FUN_140317db0`** — it does not branch on
a stored tab index; it iterates the array and draws whichever matches
`game+0xC650`.

### 13.3 Lead 2 — the sub-tab system

`gui_subtab_click` exists as **eight** `.rdata` copies; seven are referenced by
one function, and `gui_subtab_cancel` by another:

| String | Addresses | Referenced by |
|---|---|---|
| `gui_subtab_click` | `0x14087B270/320/3B8/428/498/4C0/4E8` | **`FUN_14007c400`** (7 sites) |
| `gui_subtab_click` | `0x14087B778` | *(no references)* |
| `gui_subtab_cancel` | `0x1408B8208`, `0x1408B8248` | `FUN_140358a20` |

`FUN_14007c400` is the **generic sub-tab widget** (draw + hit-test + click
sound). `Xrefs 0x14007C400` gives exactly **one code caller**:

```
[UNCONDITIONAL_CALL] from 14031bb22 in FUN_140317db0
```

So the sub-tabbed panel and the tab-bar panel are the same function. The call
site (`scratch/ui_probe_masterui.asm`):

```
LEA RCX,[0x1409be060] / CALL [0x14086c8f8]     ; C3D_PANEL2D method
MOV RCX,R14 / CALL 0x140629a70
LEA RDX,[0x1408b1b48]                          ; L"Basic"  (UTF-16)
LEA RCX,[0x140996fb0] / CALL [0x14086c870]     ; FONTMANAGER::PrintAllTexts
XOR R8D,R8D / XOR EDX,EDX / LEA RCX,[0x1409be060] / CALL [0x14086c918]
MOV RCX,R14 / CALL 0x14007c400                 ; THE SUB-TAB WIDGET
CMP byte ptr [R14 + 0xfba],0x0 / JZ skip       ; <- gate
TEST R12B,R12B / JNZ skip
```

Nearby `.rdata` format strings (UTF-16): `%.0f%%`, `%ls`, `%s (%s)`, and the
label `Basic` — the "label (value)" row shape.

### 13.4 The builder — deliverable

**`FUN_140317db0` — RVA `0x140317DB0`, size 84,735 bytes.**

```
140317db0: 40 55                    PUSH RBP                  (2)
140317db2: 41 54 41 55 41 56 41 57  PUSH R12 R13 R14 R15      (8) -> 10
140317dba: 48 8D AC 24 D0 D4 FF FF  LEA RBP,[RSP-0x2B30]      (8) -> 18
140317dc2: B8 40 2C 00 00           MOV EAX,0x2C40            (5) -> 23
140317dc7: E8 84 76 53 00           CALL 0x14084f450   <- _alloca, steal must stop before
140317dcc: 48 2B E0                 SUB RSP,RAX
...
140317e4a: MOV RAX,[0x140992048]    <- /GS cookie, +0x9A
140317e5b: MOV R14,RCX              <- RCX = game
140317e5e: LEA RBX,[RCX + 0xadb0]   <- the widget draw context
140317e68: CALL 0x1404465a0
```

- **Prologue: 18 clean position-independent bytes**
  `40 55 41 54 41 55 41 56 41 57 48 8D AC 24 D0 D4 FF FF`.
- A **23-byte** steal is equally safe (`MOV EAX,0x2C40` is position-independent)
  and ends immediately before `CALL _alloca` — the same shape and the same
  caution as the residence builder in docs/17 s10.
- **Signature `(void* game)`** — `RCX` saved to `R14`, and `RBX = game+0xADB0`,
  the *same* widget-draw context the slider `0x2727F0` and checkbox `0x3956F0`
  use. A post-hook here therefore has everything those calls need.
- **Visibility gate candidate: `game+0xFBA`** (`CMP byte [R14+0xfba],0`).
- It is called from the per-frame update `FUN_14028c770`, so it runs every frame
  while the sidebar is up.

`DecompAt` will time out here (84 KB) — use `DisasmMany` on ranges, per
`.clinerules`.

### 13.5 What is still NOT mapped

- **Which of indices 0..9 is Economy & Trade.** The index->name binding is
  runtime data: the name lives at `panel+0x01` in each panel struct, and the
  array order is built at load. I confirmed the *literals* exist
  (`population`, `resources`, `importexport`) but not their slot order. To close
  it, read `[RBP+0x30]` (the array) and each `panel+0x01` in a live game, or
  find the static initialiser that fills the array.
- `game+0xFBA` is a **candidate** gate, not a proven one — it guards the block
  after the sub-tab widget, and may be a narrower flag than "sidebar open".
- `FUN_140358a20` owns `gui_subtab_cancel`; its relationship to
  `FUN_14007c400` is untraced.
- The 18 small functions immediately before `FUN_140317db0`
  (`0x140317900..0x140317D70`) are **not** panel builders — each is a
  config-getter (`GetValueInteger` then `MULSS`/`DIVSS`, or a `GetString` id
  pick between `0x36xx` labels). They are scale/label helpers inlined ahead of
  the master function.

### 13.6 Consequence for the tax dashboard

The target for a native fiscal section is now concrete: post-hook
**`FUN_140317db0`** (`0x140317DB0`, 18/23-byte prologue, `game` in `RCX`,
`game+0xADB0` already set up), gated on `game+0xFBA`, and place rows using the
same geometry as `ui_panels` (`game+0xADB0` context, `DPI*35` rows). The
slider `0x2727F0` and checkbox `0x3956F0` calls need no adaptation — the
signature matches exactly. Because this one function owns both the tab strip and
the sub-tab strip, a fiscal section added here appears inside the real global
sidebar rather than inside a building popup.

### 13.7 Raw outputs from this session

| File | Content |
|---|---|
| `scratch/ui_xrefs_tabbar_owner.txt` | callers of the tab bar and the setter |
| `scratch/ui_funcs_dispatch.txt` | 19 functions around `FUN_140317db0` |
| `scratch/ui_probe_thunks.asm` | the 18 helper functions before the master UI |
| `scratch/ui_finddisp_68.txt` | 219 disp-`0x68` hits (none in `FUN_140317db0`) |
| `scratch/ui_probe_register.asm` | the 10-way hotkey -> tab-index chain |
| `scratch/ui_xrefs_subtab.txt` | xrefs to all `gui_subtab_click` / `_cancel` |
| `scratch/ui_xrefs_subtabhelper.txt` | `FUN_14007c400` -> one caller |
| `scratch/ui_probe_masterui.asm` | master UI prologue + sub-tab call site |
| `scratch/ui_probe_tabnames.asm` | the tab-name loop and `game+0xC650` |

---

## 14. Visibility gate & panel-name verification (session 2026-10-11)

### 14.1 The gate is `game + 0xFCE` — verified

`DisasmMany 0x140317E60:0x140318300` (`scratch/ui_probe_gate.asm`). After the
unconditional setup (screen scalars written to `0x1409EACB4/B8/C0` and
`0x140A55238/40/44`), the first conditional branch in `FUN_140317db0` is:

```
140317f20: MOV  RAX,qword ptr [R14 + 0xc650]   ; read the selected panel
140317f27: MOV  qword ptr [RBP + -0x70],RAX    ; cache it
140317f2b: CMP  byte ptr [R14 + 0xfce],R15B    ; R15B == 0
140317f32: JNZ  0x140318024                    ; SET   -> take the draw branch
140317f38: ...                                ; CLEAR -> input/tab handling
...
140317f8a: CMP  byte ptr [R14 + 0xfce],R15B    ; same test again
140317f91: JNZ  0x140318024
```

**`game+0xFCE` is the flag the function itself branches on, and `0x140318024`
is the drawing path.** So a post-hook should gate on `game+0xFCE != 0`.

Two things this does **not** establish, recorded honestly:

- There is **no unconditional early `RET` at the top.** The prologue and setup
  always run; `game+0xFCE` selects between a later draw block and input
  handling, it does not abort the function. A post-hook therefore still costs a
  call per frame even when closed — so the hook must test the flag itself.
- **`game+0xC650` was NOT observed becoming NULL.** `FindDisp c650`
  (`scratch/ui_finddisp_c650.txt`, 141 hits) shows only **nine** write sites —
  `FUN_14007c400`, `FUN_14030d1a0` (per-frame update), `FUN_140317db0` x3,
  `FUN_140337450` x2, `FUN_140387320`, `FUN_14048d010` — and none is in the
  close-key path. `FUN_1404d6df0` writes only the tab object
  (`MOV dword [RBX+0x68],0xffffffff`), never `game+0xC650`.

`game+0xC650` is also read 32 times inside `FUN_140317db0` and 20 times in
`FUN_140389f10`, so it behaves as a **shared "current selection" field** rather
than a sidebar-only pointer. Treat the name check as advisory and the flag as
authoritative:

```c
BYTE*  game = (BYTE*)game;
if (!*(BYTE*)(game + 0xFCE)) return;              // authoritative: closed
void*  active = *(void**)(game + 0xC650);
if (!active) return;                              // defensive
// optional narrowing: strcmp((char*)active + 1, "<name>")
```

### 14.2 The panel-name table — located, and the Economy names read

The names are a contiguous NUL-terminated ASCII block in `.rdata`, verified with
`dumprva.py` around `0x8A7B80`. Tab-caption names appear here **grouped**:

```
0x008A7E00  list          <- the toolbar tab set starts
0x008A7E08  resources
0x008A7E13  crime
0x008A7E19  tourism
0x008A7E21  airplanes
0x008A7E2B  ships
0x008A7E31  vehicles
```

and the **Economy & Trade sub-tab set** sits ~0x150 earlier:

```
0x008A7D10  importexport
0x008A7D1D  trade
0x008A7D23  trees            (editor overlay, interleaved)
0x008A7D29  loanusd
0x008A7D30  loanrub
0x008A7D38  productionconsumption
0x008A7D4E  warehouse
0x008A7D58  various
```

`importexport` has **exactly one** copy in the whole binary (`0x1408A7D10`),
while `population`/`resources` have 5-7 copies as ordinary words — so
`importexport` is a unique identifier, consistent with `panel+0x01`.

**Corroboration that this block is the economy caption set:** `loanusd` and
`loanrub` sit immediately beside `importexport`/`trade`, matching the verified
`$Loan_Ballance_USD` / `$Loan_Ballance_RUB` localization keys (§12.3). The
Economy & Trade panel is the one that shows loans.

Adjacent earlier entries confirm the same family: `population` (`0x8A7B80`),
`storage`, `conveyor`, `pedestrian`, `factoryconnection`, `rails`, `graphic`,
`controls`, `objectives`, `cheatsystem`, `equipment`, `residental`,
`construction`, `building`, `food`, `tourism`, `education`, `eletric_low/high`,
`pipes`, `airplanes`, `heating`, `cableway`.

### 14.3 What is still NOT established

- **The 10-slot array is not statically populated.** `[RBP+0x30]` is a *local*
  in `FUN_140317db0`, loaded from the caller's stack, and the names live at
  `panel+0x01` inside runtime-allocated entries. The `.rdata` block in §14.2 is
  the **literal pool the entries are built from**, not the array itself. So the
  index -> panel mapping is still runtime data.
- **No `strcmp`-based name check can be confirmed as the shipped mechanism.** The
  one site I found that compares a name against a literal
  (`0x14031AC82`, `LEA RDX,[0x1408b1bd0]`) resolves to the input-binding name
  `const_offi` — that is `C3D_INPUT` plumbing, **not** panel selection. I had
  briefly read it as panel selection and it is not.
- `game+0xFCE` is verified as *the flag the function branches on*. Whether it is
  exactly "sidebar open" (versus a narrower "toolbar visible") is inferred from
  its position, not proven.
- `game+0xC608` appeared in the same region as a table of `0x68`-byte entries
  with names at `+0x01`; its relationship to the 10-slot toolbar array is
  untraced.

### 14.4 Practical guidance for the post-hook

Gate on the flag, do not rely on the name:

```c
static void __stdcall h_MasterSidebar(void* game)
{
    o_MasterSidebar(game);                       // run the original first
    if (!game) return;
    BYTE* g = (BYTE*)game;
    if (!*(BYTE*)(g + 0xFCE)) return;            // sidebar closed -> draw nothing
    // fiscal rows here, using the game+0xADB0 widget context the original
    // already set up, and the ui_panels geometry (DPI*35 rows).
}
```

- Hook site: `FUN_140317db0`, RVA `0x140317DB0`, size 84,735.
- Steal: **18** clean PI bytes (`40 55 41 54 41 55 41 56 41 57 48 8D AC 24 D0 D4
  FF FF`), or **23** ending immediately before `CALL _alloca`. `/GS` cookie at
  `+0x9A`. §13.4 has the full prologue.
- A name check may be added later as a narrowing filter once the runtime array
  order is read from a live game; `importexport` (`0x1408A7D10`) is the unique
  candidate for the trade tab and `loanusd`/`loanrub` (`0x8A7D29/D30`) confirm
  the economy family.

### 14.5 Raw outputs from this session

| File | Content |
|---|---|
| `scratch/ui_probe_gate.asm` | `FUN_140317db0` first ~100 instructions; the `game+0xFCE` gate |
| `scratch/ui_finddisp_c650.txt` | 141 disp-`0xC650` hits; 9 write sites, none in the close path |
| `scratch/ui_xrefs_c650.txt` | xrefs to `0x1409E2338` (shared selection global) |
| `scratch/ui_probe_closekey.asm` | `0x14031AC60-0x14031AD00`; input-binding compare, not panel select |

---

## 15. `game + 0xC608` panel vector (session 2026-10-11)

> Labor/job-selection findings that were originally under this heading moved to
> `docs/Labor_Mobility_Investigation.md` on 2026-10-11.

### 15.1 `game + 0xC608` IS the panel array — settled

`FindDisp c608` (`scratch/ui_finddisp_c608.txt`, 27 hits) shows the classic
`std::vector` idiom repeated in `FUN_140296110`, `FUN_140337450`,
`FUN_14048d600` and `FUN_14030d1a0`:

```
MOV RCX,[RSI+0xc610]        ; end
SUB RCX,[RSI+0xc608]        ; end - begin
IMUL RCX / SAR RDX,0x5      ; / 0x68   (magic 0x6666666666666667)
MOV R8,[RSI+0xc608]         ; begin
```

and inside `FUN_140296110` (`scratch/ui_probe_vec.asm`) the loop that builds
each tab icon:

```
140296310: IMUL R14,R9,0x68           ; entry stride = 0x68 (104 bytes)
140296314: INC  R8                    ; begin+1  -> skip the +0x00 flag
140296317: LEA  RDX,[0x140899bd0]     ; "maintab_%s"
14029631e: ADD  R8,R14                ; + i*0x68
140296326: CALL 0x140005340           ; sprintf -> "maintab_<name>"
14029633e: CALL [0x14086c4c0]         ; CreateManagedTexture
```

**Confirmed layout:**

| Field | Meaning |
|---|---|
| `game+0xC608` | `std::vector<Entry>::begin` |
| `game+0xC610` | `end` — count is `(end-begin)/0x68` |
| `Entry+0x00` | active flag |
| `Entry+0x01` | NUL-terminated ASCII name (`importexport`, `trade`, …) |
| `Entry` size | `0x68` = 104 bytes |
| `game+0xC650` | pointer to the **selected** entry |

`FUN_1404d9050`'s `[RCX+0x38]` and `FUN_140317db0`'s `[RBP+0x30]` are both this
same `begin` pointer, passed down. So §12.2's ten-slot array and §14.2's literal
pool are two halves of one structure, and `game+0xC608` is the array itself.

**How a post-hook knows which sub-tab is active:**

```c
void* sel = *(void**)((BYTE*)game + 0xC650);   // selected Entry, or garbage
if (sel) {
    const char* name = (const char*)sel + 1;   // e.g. "importexport"
    // strcmp(name,"importexport") -> Trade ; "loanusd"/"loanrub" -> Loans
}
```

Because `game+0xC650` was never observed being nulled (§14.1), validate it: the
pointer must lie inside `[begin, end)` of the vector at `game+0xC608`, and the
name must be one of the known literals. That is a sound check without a live
dump:

```c
BYTE*  g    = (BYTE*)game;
BYTE*  beg  = *(BYTE**)(g + 0xC608);
BYTE*  end  = *(BYTE**)(g + 0xC610);
BYTE*  sel  = *(BYTE**)(g + 0xC650);
if (!*(BYTE*)(g + 0xFCE)) return;                 // sidebar closed
if (sel < beg || sel >= end) return;              // not a live entry
if (strcmp((char*)sel + 1, "importexport") != 0) return;   // not the trade tab
```

`importexport` (`0x1408A7D10`) is the unique trade-tab identifier; `loanusd` /
`loanrub` (`0x8A7D29` / `0x8A7D30`) are the loan sub-tabs.

### 15.2 Still untraced (UI)

- Whether `game+0xC650` is written when a tab is *clicked*, versus only by
  `FUN_140317db0`'s own cycling at `0x14031ACD9`. `FUN_1404d9050` writes the tab
  object (`obj+0x38` array, `obj+0x68` index); the hand-off to `game+0xC650` was
  not followed.

> Labor/job-selection findings that were here have moved to
> `docs/Labor_Mobility_Investigation.md`.

