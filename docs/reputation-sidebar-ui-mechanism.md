# Reputation DLL: how the sidebar button and panel are placed (static analysis)

**Target:** `Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll`, SHA-256 `fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a` (same as the earlier review). Image base `0x180000000`; addresses below are DLL VAs unless marked `exe+` (game EXE RVA, relative to `exeBase` supplied by the loader).

**Method:** Static only. The DLL was read as data with `objdump`, `pefile` (installed in a git-ignored venv under `scratch/`) and Python helpers. No game, loader or DLL was run, and no Ghidra project was used. Annotated excerpts are preserved in [`export/reputation-sidebar-ui-20261110/`](../export/reputation-sidebar-ui-20261110/).

**Status of claims:** Each statement below is tied to code addresses. Where behaviour is inferred from log strings or config comments rather than traced end to end, it is labelled as such. Nothing here has been confirmed at runtime.

---

## 1. Short answer

The mod does **not** build a panel, and it does not draw its own sidebar button from scratch. It works by reusing the game's own UI objects:

1. **Button.** It copies one existing record from the game's left-sidebar vector, changes the name, icon and string id, and appends the copy with the game's own copy constructor. The game then draws and hit-tests the new entry like any other.
2. **Panel.** It does not create the panel. When the player selects the entry, the game builds its own sidebar panel. The mod hooks the `C3D_PANEL2D` methods the game calls on that panel, reads the rectangle the game gives it, and resizes it to fit the mod's windows.
3. **Content.** The mod draws its own text and widgets inside that rectangle. It does this from detours on the engine's panel draw passes and its per-frame terrain render call, not from the game's UI code.

The sidebar and panel path installs its hooks by replacing the EXE's import slots for `C3DDLL64.dll` functions (`patchIat`), with no code patching. The DLL does also install five inline (code-byte) hooks, but they belong to the border, customs and visa logic (`0x18002e410`, for example "tourist count (visa gate)" and "customs trade"), not to the sidebar or the panel. They are outside this report.

---

## 2. Hooks: how the mod gets control

Installed from `0x1800690fc`–`0x180069a2f` (exports and config are read first; see §6). Each call is `host->patchIat(module, "C3DDLL64.dll", <mangled name>, detour, &original, label)`, where `module` is the EXE handle held in DLL global `0x1800c4d78`. The host struct offsets are from `src/tesmio_api.h`: `+0x38` log, `+0x48` `patchIat`, `+0x70` `configInt`, `+0x78` `configString`.

| Engine method (import from `C3DDLL64.dll`) | Detour (DLL VA) | Saved original | Role |
|---|---|---|---|
| `C3D_PANEL2D::SetDimensions` | `0x180067fa0` | `0x1802860b8` | Captures the size the game gives a panel. Only acts when the **caller** is `exe+0x31E478` or `exe+0x31E4B2`. |
| `C3D_PANEL2D::SetPosition` | `0x180068090` | `0x1802860c0` | Captures the position. Only acts when the caller is `exe+0x31E509`. |
| `C3D_PANEL2D::SetReallyRender` | `0x180068140` | `0x180286108` | "Interface pass" hook (hook stage 1). |
| `C3D_PANEL2D::EndDraw` | `0x180067bf0` | `0x180286110` | Hook stage 2. Calls the body after the original `EndDraw`. |
| `C3D_PANEL2D::Draw` | `0x180067da0` | `0x180286118` | Hook stage 3. Calls the body after the original `Draw`. |
| `C3D_TERRAIN::Render` | `0x180068180` | `0x180187dc0` | The per-frame placement and clone loop. The block at `0x180287ac0`–`0x180287b20` holds other EXE function pointers computed as `exeBase + offset`; these were not analysed. |
| `C3D_FONTMANAGER::PrintAllTexts` | `0x180067f10` | `0x1802860e8` | "Fill" hook, optional. |

Key points:

- The detours check the **return address** (`[rsp+0x48]` in `SetDimensions`, `[rsp+0x28]` in `SetPosition`) against fixed EXE offsets. The mod therefore reacts only to the game's own call sites that set up the sidebar-panel rectangle, not to every panel.
- Which pass draws the body is chosen by the config key `panel_hook` (stored in `0x180280b0c`): 1 = `SetReallyRender`, 2 = `EndDraw`, 3 = `Draw`. The detour calls the body `0x180039400` only when the stage matches.
- If an import is missing, the installer logs "hook 2 unavailable" / "hook 3 unavailable" (or the equivalent for `SetReallyRender`). If that hook was the selected `panel_hook`, the code sets `panel_hook` to 0, so the body is not drawn through that pass. It does **not** switch to another pass automatically; the log text only advises trying a different value.

Evidence: [`02_panel_hook_install_IAT_patches.asm`](../export/reputation-sidebar-ui-20261110/02_panel_hook_install_IAT_patches.asm), [`03_panel_detours_…asm`](../export/reputation-sidebar-ui-20261110/03_panel_detours_EndDraw_Draw_SetDimensions_SetPosition_SetReallyRender.asm).

---

## 3. The button: cloning a sidebar entry

**Function:** `0x180056b70` ("side appending … cloned from entry %llu"). It is called every frame from the per-frame hook `0x180068180` at `0x1800687f7`, before `0x180057230`. Evidence: [`05_sidebar_entry_clone_0x180056b70.asm`](../export/reputation-sidebar-ui-20261110/05_sidebar_entry_clone_0x180056b70.asm).

### 3.1 The game's sidebar vector

Read from the EXE, with `exeBase` from the loader:

| Field | Offset | Notes |
|---|---|---|
| vector begin | `exe+0x9E1518` | |
| vector end | `exe+0x9E1520` | incremented by `0x68` on append |
| vector capacity | `exe+0x9E1528` | |
| selected record pointer | `exe+0x9E1560` | set by the mod in §4 (`0x1800593b0`); cleared by helper `0x1800571C0` |
| selected index (DWORD) | `exe+0x9E156C` | compared in §4 |

Each record is `0x68` bytes. The fields the code touches are:

| Record offset | Use |
|---|---|
| `+0x00` | byte flag; donors must have `1` |
| `+0x01` | name, up to `0x17` chars (`"reputation"` is copied here) |
| `+0x30` | back-pointer to `g_ui` (`exe+0x9D4F10`); donors must match it |
| `+0x44` | label/string id (DWORD) |
| `+0x48`, `+0x50`, `+0x58` | sub-list begin / end / capacity; donors must be null |
| `+0x60` | texture pointer |

`g_ui = exe+0x9D4F10`. This is consistent with the config's `g_ui+0xAE50` naming, because `0x9D4F10 + 0xAE50 = 0x9DFD60`. The record layout is inferred from the accesses above; it is not confirmed against the game's type definitions.

### 3.2 Gating

The clone runs only if all of these hold:

- `0x180280d80` is set. This is the config key `sidebar`.
- `0x180280b24` is set. The build check in `TsmPluginStart` (`0x180068f30`) sets it only when the EXE's PE timestamp equals the `build_stamp` in the config. Otherwise the code logs "this is not the build these addresses were read off. Declined."
- `0x180287b38` is clear. It is a done/failed latch.

The search for the entry runs once every 240 frames (counter `0x180287b3c`). After four failed searches (counter `0x180287b40` above 3) the mod logs "giving up rather than retrying forever" and sets the done latch.

### 3.3 Donor selection and cloning

1. **Existing entry.** The loop at `0x180056c58` scans the vector for a name equal to `"reputation"`. If one is found, it only refreshes its string id at `+0x44` (logged as "label id … -> …") and does not add a copy.
2. **Vector full.** If `end == capacity`, the code declines. Appending would reallocate the vector and invalidate the selection pointer.
3. **Donor.** Scanning from the end, the first record that satisfies all of these is the donor: `+0x00 == 1`, `+0x30 == g_ui`, and `+0x48`, `+0x50`, `+0x58` all null. The donor's sub-list is therefore empty and the clone shares no allocation with it.
4. **Build the copy on the stack.** The donor's bytes `0..0x67` are copied to a local. The sub-list fields are zeroed, the name is replaced with `"reputation"`, and the string id is set from the global `0x1800c3308`. The donor's texture pointer is kept as a fallback (the config comment says the same); it is replaced in step 6 if the new icon loads.
5. **Insert.** `call [exe+0x459530]` is the game's record copy constructor. It copies the local record into `exe+0x9E1520` (the current end), and then `exe+0x9E1520 += 0x68`.
6. **Icon.** The mod formats `editor/maintab_%s.png` with `"reputation"` (`0x18009e7d8`), loads it with `0x18003ae90`, and stores the texture pointer at record `+0x60`. The log message says the icon "keeps the picture it was cloned with" if loading fails.
7. **Failure rollback.** If the texture pointer is still null, the mod removes the record again (`end -= 0x68`) and logs "has no texture - taken back out before the game could draw it." The config comment says the same thing, and it matches the code.
8. **Success.** `0x1800583e0` runs. It is a diagnostic dump of the sidebar items, and it does nothing unless its debug flag `0x1800d799c` is set. The success path sets `0x180280d84` ("entry present").

### 3.4 Why the game draws it

The record is a copy of a native sidebar entry, so the game's own code draws the icon, tooltip and selection state. The mod's only additions are the name, the string id and the texture. The string id is registered separately through `C3D_LANGUAGE::GetString` and `CustomStringAdd` for the tooltip and the panel heading (config `engine_strings`, `panel_title`); that path is not traced here.

---

## 4. Selection: what happens when the entry is clicked

The DLL does not hook the game's sidebar click itself. The selection-change handler `0x1800575c9`–`0x180057780` is not reached by a direct `call` in the DLL. Its entry point is therefore reached indirectly (a stored pointer or a loader hook), which was not traced. The handler does this, in order:

1. Calls `0x180057e40` (trade-row insertion; see §7).
2. Calls `0x180058310`. This returns true when the selected entry is `trade` and the selected index at `exe+0x9E156C` equals the DLL's stored index `0x1800c3c5c`.
3. **Correction (second pass, `0x1800576c4`, listing `D05`).** `0x1800593b0` runs only when **all** of these hold: `in_panel` (`0x1800c32ec`) is set; the reputation entry is **not** the current selection (`0x180057d50` returns 0); the state flag `0x180411bc8` is clear; the predicate `0x180001c80` is true; and the trade-row check `0x180058310` is false. `0x180001c80` returns 1 when **any** of the seven DLL window flags is set (listing `D12`). So the write re-selects the reputation entry while one of the mod's windows is open. It finds the record named `reputation` and writes its address into the game's **selected record pointer** `exe+0x9E1560`. The earlier wording "the entry is ours" was wrong: the write happens when the entry is **not** selected.
4. When `sidebar_hides_panel` (`0x1800c3c74`) is **0** (latching mode), and the selection moves to a different entry, it clears the mod's own state (`0x180286080`, `0x180286084`, `0x1801cda1c`, `0x180184eb4`, `0x1801cda14`, `0x1801cda18`, `0x1800c3158`) and logs "another sidebar entry chosen - panel closed". These are the mod's own globals, not game state. The branch runs only when the value is 0. The code path that makes the button a push-button when the value is 1 was not identified with certainty.

Evidence: [`07_sidebar_selection_write_0x1800593b0.asm`](../export/reputation-sidebar-ui-20261110/07_sidebar_selection_write_0x1800593b0.asm), [`06_trade_row_selected_check_0x180058310.asm`](../export/reputation-sidebar-ui-20261110/06_trade_row_selected_check_0x180058310.asm), [`17_selected_is_reputation_0x180057d50.asm`](../export/reputation-sidebar-ui-20261110/17_selected_is_reputation_0x180057d50.asm).

**Interpretation (inference).** Setting `exe+0x9E1560` to our record is presumably how the mod makes the game treat the entry as selected, so that the game opens its own panel for it. The config describes `sidebar_hides_panel` as changing what a later click does; the one branch I traced for that setting is the 0 (latching) case in step 4. The game-side consumer of `exe+0x9E1560` was not traced.

---

## 5. Placement and drawing inside the panel

### 5.1 Per-frame driver

`0x180068180` (the `C3D_TERRAIN::Render` detour) runs every frame. In order it does the following, subject to the flags shown:

- Calls `0x180058c20`, the placement watchdog, at `0x180068685`.
- Calls `0x180060e20` (the mod's own window dispatcher) when `in_panel` is set and the entry is present (`0x180280d84`). `0x180280d88` also appears in the gate, but I did not fully decode the skip target. The dispatcher's contents are covered in the earlier contracts-window review.
- Calls the sidebar clone `0x180056b70` at `0x1800687f7` (§3).
- Calls `0x180057230` and `0x18005abb0` (not analysed here).

Evidence: [`04_terrain_render_frame_hook_placement_and_clone_call.asm`](../export/reputation-sidebar-ui-20261110/04_terrain_render_frame_hook_placement_and_clone_call.asm).

### 5.2 Size and position of the game's panel

The `SetDimensions` and `SetPosition` detours copy the arguments the game passes when it lays out the sidebar panel:

| Captured value | Source | Stored at |
|---|---|---|
| panel width | `SetDimensions` arg 1, caller `exe+0x31E478` or `+0x31E4B2` | `0x180280d5c` |
| panel height | `SetDimensions` arg 2, same callers | `0x180280d60` |
| panel x | `SetPosition` vector x, caller `exe+0x31E509` | `0x180280d64` |
| panel y | `SetPosition` vector y, same caller | `0x180280d68` |
| (not a vector component) | constant from DLL global `0x1801a3fd0`, same caller | `0x180280d6c` |

The detours also store the raw width and height at `0x1801b4868`/`0x1801b486c`, and the raw position at `0x1801b4870`/`0x1801b4874`. The capture is gated by `0x180280d74` (the raw `in_panel` config value) and `0x180280d78` (a runtime flag set by the placement code, not by config).

### 5.3 Resizing the panel

When `SetDimensions` is called from `exe+0x31E478` and the entry is ours (`0x180057d50`), the detour tail-calls the sizer `0x180058930` (gated by `in_panel` `0x1800c32ec`). The sizer reads the three floats at `g_ui+0xAE50` and then:

- writes the **right edge** to `g_ui+0xAE54`, conditionally, when the computed value is positive. The value comes from DLL global `0x1800c32f0`, which the config loader fills from the key `in_panel_w` (`0x1800484a4`–`0x1800484ee`; the file's initial value is 1280.0, matching the shipped config). The same global is also nudged at `0x18003b0c0` by adding 25.0 (constant at `0x1800af67c`), which is the live-nudge step the config comment describes for `CTRL+SHIFT+LEFT/RIGHT`. I did not trace the caller of that nudge;
- writes the **bottom** to `g_ui+0xAE58`. The value is computed from the captured geometry and DLL globals (`0x1800c32fc`, `0x1800c3158`, and a constant), clamped between two bounds. The exact formula was not decoded.

For the `exe+0x31E478` caller only (listing `D13`), the detour also calls `0x1800280e0`, which writes the same two fields (`+0xAE54`, `+0xAE58`) as a clamp against `g_ui+0xAE50`. The `exe+0x31E4B2` caller does not reach the clamp. The sizer logs "panel sized: right edge … bottom … top …".

Separately, the placement path at `0x180059323` writes the float at DLL global `0x180411bac` to `g_ui + [debug] panel_width_at`. The shipped config sets `panel_width_at = 44628` (`0xAE54`), so this targets the right-edge field. The write is logged as "writing … to g_ui+0x… while our panel is open". The config comment for `in_panel_w` says the same thing: it moves the panel rather than setting its width.

`g_ui = exe+0x9D4F10`, so `g_ui+0xAE50` = `exe+0x9DFD60`, `g_ui+0xAE54` = `exe+0x9DFD64`, and `g_ui+0xAE58` = `exe+0x9DFD68`. These are the writes that appear in the direct-write list at `0x180028128`, `0x180028149`, `0x180058b23`, `0x180058b2b`.

Evidence: [`08_panel_size_writer_0x180058930.asm`](../export/reputation-sidebar-ui-20261110/08_panel_size_writer_0x180058930.asm).

### 5.4 Placing the mod's content

`0x180058e77`–`0x18005905d` is the in-panel layout (log: "in-panel: screen … The game drew its panel background W x H centred on X,Y"). It reads the captured rectangle from `0x180280d5c`–`0x180280d68`. It treats the position as the panel's centre, and places the mod's windows inside that rectangle. The config comments say the padding keys (`in_panel_pad_x`, `in_panel_pad_y`, `in_panel_foot`) feed this, but I did not confirm that in the layout code. The layout is inferred from the log text and the offsets read; the math was not decoded line by line.

`0x180058c20` is the watchdog that decides when the layout is trustworthy. Its log messages describe the fallback behaviour:

- "a window has been open for four seconds and the panel has not drawn it - the windows come back to the terrain pass";
- "the panel is drawing again - the windows go back inside it";
- "180 frames selected and the panel background has not been seen being drawn - in_panel has nothing to put our content in".

So the in-panel mode degrades to drawing on the terrain pass if the panel's own draw is not seen. The detour logic is consistent with that.

Evidence: [`09_placement_watchdog_0x180058c20.asm`](../export/reputation-sidebar-ui-20261110/09_placement_watchdog_0x180058c20.asm), [`10_inpanel_layout_0x180058e77.asm`](../export/reputation-sidebar-ui-20261110/10_inpanel_layout_0x180058e77.asm).

### 5.5 Drawing the body

The body is reached from the `Draw`/`EndDraw` detour (or `SetReallyRender`, depending on `panel_hook`):

1. `0x180039400`: guard. It returns early on the re-entry marker `0x180287abc`, on `0x180280b10`, on `0x180280b00` ("panel off after a fault"), or if `0x180280afc` is clear. It requires `panel_stage` (`0x180280af8`) to be at least 1, sets `0x180280b10` and `0x180287abc`, and calls the body inside a `__try`. On a fault it logs "panel faulted inside the interface pass - panel off" and sets `0x180280b00`.
2. `0x180038f30`: dispatch on `panel_stage`. `1` returns. `2` goes to `0x180039d90`. `3` goes to `0x18003a260`. Any other value goes to `0x180039960`.
3. Stage 2 (`0x180039d90`): logs "calibration card at …  ticks every 50px" and draws a calibration grid. This is a layout aid, not content. (Inferred from strings and calls.)
4. Stage 3 (`0x18003a260`): draws the `REPUTATION` heading and a line of numbers labelled `soviet` and `west`. It also handles the mouse: it reports whether the cursor is over the panel, and logs "CLICK taken on the panel - the game did not see it" when the mod consumes a click. (Inferred from strings and calls.)
5. Stage 4 (`0x180039960`): draws the heading and calls the engine's own `C3D_PANEL2D` methods through the saved originals (`0x180286090`–`0x1802860f8`, including `0x1802860b8` for `SetDimensions` and `0x1802860c0` for `SetPosition`). On a fault it logs "stage 4 faulted - back to stage 3" and sets `panel_stage` to 3.

The text helper `0x180022c50` is called for almost every label. It is the mod's own text-and-coordinates routine; its internals were not analysed in this pass.

Evidence: [`11_panel_body_guard_0x180039400.asm`](../export/reputation-sidebar-ui-20261110/11_panel_body_guard_0x180039400.asm), [`12_panel_body_dispatch_0x180038f30.asm`](../export/reputation-sidebar-ui-20261110/12_panel_body_dispatch_0x180038f30.asm), [`13_stage2_renderer_0x180039d90.asm`](../export/reputation-sidebar-ui-20261110/13_stage2_renderer_0x180039d90.asm), [`14_stage3_renderer_0x18003a260.asm`](../export/reputation-sidebar-ui-20261110/14_stage3_renderer_0x18003a260.asm), [`15_stage4_renderer_0x180039960.asm`](../export/reputation-sidebar-ui-20261110/15_stage4_renderer_0x180039960.asm).

## 6. Config keys and the DLL globals they drive

Read by the INI loader at `0x180047d40`–`0x180049295` (config accessed through `host->configInt`/`configString`). Only the keys that control the UI are listed.

| Key (`reputation.ini`) | DLL global | Effect in code |
|---|---|---|
| `sidebar` | `0x180280D80` | Enables the clone (§3.2). |
| `build_stamp` (check in `TsmPluginStart`) | `0x180280B24` | Set only when the EXE's PE timestamp equals `build_stamp`; required by the clone and by other EXE-address paths. |
| `in_panel_w` | `0x1800C32F0` (float) | Panel position value; the sizer reads it as the right-edge input (§5.3). Also nudged live by +25.0 at `0x18003b0c0`. |
| `in_panel_bottom` | `0x1800C32FC` (float) | Sizer bottom input (§5.3). The config loader writes it at `0x1800485a8`. |
| `sidebar_hides_panel` | `0x1800C3C74` | Used by the selection path. The latching (0) branch is the one traced (§4, step 4). |
| `sidebar_item` | `0x180287B48` | Enables the trade-row path (§7). |
| `in_panel` (raw) | `0x180280D74` | Forced to 0 when `sidebar` is off (in the same branch that clears the effective flag). |
| `in_panel` (effective) | `0x1800C32EC` | Gates the sizer, the layout and the body. |
| `panel_hook` | `0x180280B0C` | Selects the draw pass: 1 `SetReallyRender`, 2 `EndDraw`, 3 `Draw`. |
| `panel_stage` | `0x180280AF8` | Selects the body renderer (§5.5). |
| `[debug] panel_width_at` | `0x1800C3C68` | Offset from `g_ui` where the width float is written (default `0xAE54`). |
| (none) | `0x180280D84` | "Entry present": set after the clone finds or adds the record. |

Config keys read from `plugins\reputation.ini` (`0x18008de18`). Several keys in the shipped config are written in comments as off by default, but their values are `1`. This was already noted in the earlier review and was not re-checked for this pass.

---

## 7. Optional trade-row path (`sidebar_item`, off by default)

Covered in the earlier [review](almanac-reputation-review.md) and not re-traced here. The summary is:

- `0x180057e40` searches the sidebar records for `trade` and appends one 0x50-byte entry to its sub-list at record `+0x48`, using the game helper `exe+0x4492F0`.
- `0x180058310` checks whether that item is the selected one (§4, step 2).
- `0x18002F1E0` is a dispatcher for the mod's own panel states. The seven globals it changes are DLL-local, not established as game state.

This path writes into the game's trade vector. The config says that is a deliberate choice. The `sidebar_item` flag `0x180287B48` is checked in the selection path (`0x180058310`); I did not re-verify the gate on the insert itself.

**Correction (second pass; see [row-item analysis](almanac-row-item-vs-button-analysis.md)).** Insertion is called from the selection handler at `0x18005761E`, gated by `0x180287B48`, `0x180280D80`, `0x180280B24` and the decline latch `0x180287B4C`. It retries every 240 invocations (`0x180287B50`). The DLL does **not** check vector capacity before the native push. When the trade row item is clicked, the handler does not re-point `0x9E1560`, and the dispatch call `0x18002F1E0` takes its index from `0x180411BCC`, which is stored only in the case loop. The result is a DLL-state change (on the first click, `0x1800C3158` becomes 2), not a verified window opening. The log text "opening the reputation window" is printed without that being shown.

---

## 8. Corrections to the config comments

The shipped `reputation.ini` says that the only place the plugin writes to a game global is the `sidebar_hides_panel` path. The disassembly does not support that. Direct writes with an EXE displacement (the scan used `exe+0x9xxxxx` patterns) include:

| Written location | Where | Purpose |
|---|---|---|
| `exe+0x9E1520` (vector end) | `0x180056EE3` (`add`), `0x180057010` (rollback) | Append or undo the sidebar clone (§3.3). |
| `exe+0x9E1560` (selected record) | `0x18005948C` (set), `0x1800571F1` (clear) | Selection (§4). |
| `exe+0x9DFD64`, `exe+0x9DFD68` (`g_ui+0xAE54/58`) | `0x180028128`, `0x180028149`, `0x180058B23`, `0x180058B2B` | Panel right edge and bottom (§5.3). |
| `g_ui + panel_width_at` | `0x180059323` | Width write (§5.3). |
| `exe+0x9E17F0`, `exe+0x9E17E0` | `0x18005B100`, `0x18005B10E`, `0x18005BC34`, `0x18005C897` | Not analysed. |
| `exe+0x9BE30C`–`0x9BE318` (base register not confirmed as `exeBase`) | `0x18003913A`–`0x180039158` (in `0x180038F80`) | Constants written before drawing. Not analysed. |

The config comment is therefore incomplete. The selection write (`0x18005948C`) is not gated by `sidebar_hides_panel` at all; it is gated by `in_panel`. I have not edited the `.ini`, because changing it is outside this task.

---

## 9. Not established

- **Runtime behaviour.** Nothing was run, so the layout math, the stage renderers and the click handling are inferred from code structure and log strings.
- **The consumer of `exe+0x9E1560`.** The game code that reads the selected record was not located, so how the game opens the panel for it is not traced.
- **The selection handler's entry point.** `0x1800575C9` has no direct caller in the DLL, so it is reached indirectly.
- **The bottom-edge formula** in the sizer (§5.3). The source of `0x1800C32F0` is now known (config key `in_panel_w`); the caller of the `0x18003b0c0` nudge is not traced.
- **The text helper** `0x180022C50` and the layout helpers `0x180039330`, `0x180039490`, `0x180060E20` internals.
- **The record layout.** Offsets are inferred from the mod's accesses, not from the game's type information. The repo's `docs/engine/` layouts were not compared.
- **Build dependence.** Every EXE offset above is specific to the build whose PE timestamp is `0x6A3EB6AD`. The clone is refused on other builds, but the placement hooks are not gated by a separate check, so a different build could misplace content.
- **Scan coverage.** The direct-write scan uses exe displacements only. Writes through another base register would be missed.

---

## 10. Runtime checks that would close the gaps

If the game is available and this is authorised, these reads would settle the open points, all without changing game state:

1. After selecting the `reputation` entry, read `exe+0x9E1560` and confirm it points at a record whose name is `reputation`.
2. Log the return addresses that reach `SetDimensions` and `SetPosition` (they should be `exe+0x31E478`, `+0x31E4B2` and `+0x31E509`).
3. Read `exe+0x9DFD60`–`0x9DFD68` before and after the sizer, and compare them with the logged "panel sized" values.
4. Trace the game's selection consumer by setting a read watchpoint on `exe+0x9E1560`.

These belong to the existing "read-only snapshot" workflow in `KILO_AGENT_WORKFLOW.md`. Check that workflow's authorisation rules before running anything against the game.


