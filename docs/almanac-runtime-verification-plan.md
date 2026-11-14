# Almanac v2.12 Runtime Verification Plan

**Status:** Plan only — no game launch, build, deploy, or source edit authorized.
**Target:** SOVIET64.exe v1.1.1.9 (PE TimeDateStamp 0x6A3EB6AD), plugin almanac_button v2.12.
**Source commit:** f857619 (Almanac branch).
**Plugin DLL:** build/plugins/almanac_button.dll (built 2026-11-14, not yet deployed to game directory).
**INI:** plugins/almanac_button.ini (enabled=1, texture_asset=icon_inspectnumbers.png, content_suppression=1).

---

## 1. Prerequisites and identity verification

### 1.1 Game executable identity
- **Expected:** SOVIET64.exe v1.1.1.9, PE TimeDateStamp 0x6A3EB6AD, preferred image base 0x140000000.
- **Verify:** Record the actual file's SHA-256, size, and PE TimeDateStamp from the game directory. Compare against the documented value 0x6A3EB6AD. If they differ, stop — the binary is a different version and all RVAs may be wrong.
- **Record:** File path, size, SHA-256, PE timestamp, ASLR base address (from a debugger or process inspection).

### 1.2 Plugin DLL identity
- **Do NOT** use the checked-in DLL hash as proof of which DLL was loaded. The checked-in hash (65FCB684... for C3DDLL64.dll) only proves the tracked file matches its analysis report, not that it was the DLL loaded during the run.
- **Verify:** After deploying, record the deployed DLL's SHA-256 and size from the game's plugin directory. Compare against the build output DLL. If they differ, the deployed DLL is stale or corrupted.
- **Record:** Deployed DLL path, size, SHA-256, build timestamp. Compare with build/plugins/almanac_button.dll.

### 1.3 TesmioLoader log
- **Location:** `<game_dir>/build/tesmioloader.log` (or configured log path).
- **Verify:** The log shows "almanac_button init v2.12" and "armed clickable ninth tile after row tile 8". If an older version string appears, the deployed DLL is stale.
- **Record:** Full log file (copy to evidence directory before testing).

### 1.4 INI verification
- **Verify:** `[System.IO.File]::ReadAllBytes($ini)[0..2]` is NOT `239 187 191` (no BOM).
- **Verify:** INI contains `enabled = 1`, `texture_asset = icon_inspectnumbers.png`, `content_suppression = 1`.
- **Do not modify the INI during testing.**

### 1.5 Disposable test setup
- Use a disposable game profile/copy with cloud sync OFF.
- Do not alter saves, time, taxes, policy, or game state beyond the approved test.
- Back up the save file before testing.
- Test on a fresh game load (not a loaded save with existing state).

---

## 2. Test procedure

### 2.1 Appearance and texture verification

**Observed facts to record:**
- Screenshot of the Economy & Trade page with the ninth tile visible.
- Whether the ninth tile shows a distinct icon or repeated native artwork.
- Whether the ninth tile is visually aligned with the eight native tiles (same row, same height, same spacing).
- Whether the ninth tile is clipped or partially off-screen at the current resolution.

**Expected results (hypothesis, not proven):**
- The ninth tile should show the icon from `icon_inspectnumbers.png` (if CreateManagedTexture + Load + TexBind all succeed).
- If the texture failed to load, the ninth tile shows repeated native artwork (fallback).
- The ninth tile should be at X = origin + 8*pitch (scale*642 handler units), Y = rowY.

**Pass criteria:**
- Ninth tile is visible and not clipped.
- If distinct icon: texture Load and TexBind succeeded.
- If repeated artwork: texture failed (check log for "texture failed to load" or "texture load faulted").

**Failure diagnosis:**
- If no ninth tile appears: check log for "FAIL-STOP" or "RECT DIRTY" (rect-pair corruption).
- If ninth tile appears but no icon: check log for "texture failed to load" (CreateManagedTexture returned NULL) or "texture load faulted" (SEH fault in Load).
- If ninth tile appears with icon but wrong position: record the visual position and compare with expected X/Y.

### 2.2 Open/close toggling

**Test cases:**

| # | Action | Expected (hypothesis) | Observed |
|---|--------|----------------------|----------|
| 1 | Single click on ninth tile | Almanac opens (log: "almanac opened via ninth-tile click") | |
| 2 | Single click again | Almanac closes (log: "almanac closed via ninth-tile click") | |
| 3 | Click and hold (mouse down, hold 2s, release) | Opens once, does not toggle again while held | |
| 4 | Rapid clicks (5 clicks in 1s) | Toggles on first click, ignores subsequent while latch held | |
| 5 | Hover over ninth tile (no click) | No toggle, no state change | |
| 6 | Click ninth tile, drag mouse off, release | Opens on click; drag does not trigger additional toggle | |
| 7 | Click ninth tile, then click native tile 0-7 | Almanac closes (log: "almanac closed by native row click") | |
| 8 | Click native tile 0-7 while Almanac closed | Native selection changes normally; Almanac stays closed | |

**Pass criteria:**
- Toggle works on single click.
- No double-toggle on held click.
- No toggle on hover.
- Native row click closes Almanac.
- Native buttons retain normal behavior.

**Failure diagnosis:**
- If double-toggle on held click: debounce logic issue (check g_almanacDebounce reset).
- If no toggle on click: CheckNinthClick returning 0 (hitbox mismatch or latch not set).
- If toggle on hover: latch read is returning true without click (latch semantics wrong).

### 2.3 Hitbox verification

**Procedure:**
1. Open the Economy & Trade page.
2. Move mouse slowly across the ninth tile from left to right, top to bottom.
3. Record the mouse coordinates (from a debugger or screen capture with cursor position) at:
   - Left edge of the tile
   - Right edge of the tile
   - Top edge of the tile
   - Bottom edge of the tile
   - Center of the tile
4. Click at each of these positions and record whether the Almanac toggles.
5. Click just outside each edge (1-2 pixels) and record whether it toggles.
6. Repeat at different DPI scales (if available: 1.0, 1.25, 1.5, 2.0).

**Expected (hypothesis):**
- Clicks inside the tile (centered bounds [x-w/2, x+w/2) x [y, y+h]) should toggle.
- Clicks outside should not toggle.
- The hitbox should match the visible tile position.

**Pass criteria:**
- Clicks on the visible tile toggle the Almanac.
- Clicks just outside the visible tile do not toggle.
- No double-trigger with native tile 8 (shared boundary).

**Failure diagnosis:**
- If clicks on the visible tile don't toggle: hitbox is misaligned (record mouse coords and compare with expected X/Y).
- If clicks outside toggle: hitbox is too large (check w/h calculation).
- If both native tile 8 and ninth tile trigger on the same click: boundary overlap (X inclusivity issue).

**Record:** Mouse coordinates (x, y) at each test point, expected bounds, actual toggle result.

### 2.4 Native button preservation

**Procedure:**
1. Open the Economy & Trade page.
2. Click each native button (0-7) in order.
3. Verify each opens its corresponding sub-page.
4. Verify the button order matches vanilla (trade_importexport, trade_productionconsumption, trade_tourism, trade_crime, trade_loanrub, trade_loanusd, trade_prices, trade_resources).
5. Verify TAX (City Hall) has not moved.
6. Verify native selections and close behavior are unchanged.

**Pass criteria:**
- All 8 native buttons work in their original order.
- TAX is in its original position.
- No native button is missing or duplicated.

### 2.5 game+0x5E6 characterization

**Procedure:**
1. With Almanac closed, read game+0x5E6 (should be 0 or native value).
2. Open Almanac (click ninth tile).
3. Read game+0x5E6 (should be 1 if content_suppression=1).
4. Move mouse over the content area and observe visual changes.
5. Close Almanac.
6. Read game+0x5E6 (should be 0).

**Observed facts to record:**
- Value of game+0x5E6 before/after open/close.
- Whether native content is visually hidden or still visible when Almanac is open.
- Whether native content click handling is disabled when Almanac is open.
- Whether a tint change is visible (the "small window in the corner" artifact).
- Whether the corner artifact correlates with mouse position (FUN_140335BC0 hit test).

**Expected (from static analysis):**
- game+0x5E6 = 1 when Almanac open, 0 when closed.
- Native content is NOT visually hidden (Draw still executes).
- Native click handling may be disabled when mouse is in a specific rectangle.
- Tint change may be visible (artifact).

**Pass criteria (for the partial input effect):**
- game+0x5E6 toggles correctly with Almanac state.
- No crash or fault when writing the flag.

**Failure diagnosis:**
- If game+0x5E6 doesn't toggle: WriteU8 failed (check log for "WARNING content suppression write failed").
- If native content is still fully interactive: the 0x5E6 effect is not sufficient for exclusive visibility (expected per C43-C44).

---

## 3. Required evidence captures

### 3.1 Logs
- Full tesmioloader.log (copy before and after test).
- Filter for "almanac_button" lines.
- Record: version string, "armed" message, texture load result, click events, any FAIL-STOP/RECT DIRTY/warnings.

### 3.2 Screenshots
- Economy & Trade page with ninth tile (normal state).
- Economy & Trade page with Almanac open (showing native content still visible).
- Mouse at each hitbox test position (center, edges, off-target).
- Native buttons 0-7 in order.
- Corner artifact (if visible).

### 3.3 State captures
- game+0x5E6 value (before open, after open, after close).
- game+0xC65C (selected sub-tab) before/after native clicks.
- Mouse coordinates (from GetMouseSolid or debugger) at each test point.
- Drawn bounds (rect store 0x9BE2E8-0x9BE2F4: W, H, X, Y) for the ninth tile.
- DLL hash of deployed almanac_button.dll.
- Game executable hash and PE timestamp.

### 3.4 Pass/fail criteria summary

| Criterion | Pass | Fail |
|-----------|------|------|
| Plugin loads and arms | Log shows "armed clickable ninth tile" | No "armed" message or version mismatch |
| Texture loads | Log shows "texture loaded" | "texture failed to load" or "texture load faulted" |
| Click toggles Almanac | Single click opens/closes | No toggle or double-toggle |
| Hitbox matches visible tile | Clicks on tile toggle; clicks outside don't | Misaligned hitbox |
| Native buttons preserved | All 8 work in order | Missing/duplicated/moved buttons |
| No crash | No FAIL-STOP, no crash | Any crash or fault |
| 0x5E6 toggles | Value = 1 when open, 0 when closed | Value doesn't change |

---

## 4. What would be needed to diagnose a failure

| Failure | Required evidence | Next step |
|---------|------------------|-----------|
| Hitbox misaligned | Mouse coords + drawn bounds (rect store) at each test point | Compare mouse space vs rect store space; check DPI scaling |
| Texture not displaying | Log ("texture loaded/failed"), deployed DLL hash | Verify Load call succeeded; check if icon_inspectnumbers.png exists in game media |
| Double-toggle on held click | Log click events, debounce state | Check g_almanacDebounce reset logic |
| Native button broken | Screenshot of button row, log | Check if Detour is interfering with native Draw |
| Corner artifact | Screenshot with mouse position, 0x5E6 value | Determine if artifact is 0x5E6 tint or ui_overlay |
| Crash | Full log, crash dump | Check SEH fault location; verify pointer validity |

---

## 5. Authorization requirements

The following require explicit user authorization before execution:
1. **Launching the game** — not authorized in this session.
2. **Deploying the DLL** — not authorized (build output exists but not deployed).
3. **Click testing** — not authorized.
4. **Debugger attachment** — not authorized.
5. **Modifying saves/state** — not authorized.

The following are authorized (already performed):
- Static code analysis and documentation.
- Building the plugin (`cmd /c build.bat almanac_button`).
- Host geometry tests.
- Reading cached Ghidra exports and disassembly.

---

## 6. Missing prerequisites

1. **Game executable hash** — not recorded in this session; must be measured from the actual game directory.
2. **Deployed DLL hash** — not recorded; must be measured after deployment.
3. **Mouse coordinate capture** — no debugger or coordinate-logging mechanism is currently in the plugin. Would require either a debugger or adding debug logging (which is a source change requiring authorization).
4. **Drawn bounds capture** — the plugin logs "ninth tile drawn" but not the actual rect store values. Would require adding debug logging (source change).
5. **DPI scale value** — not recorded; must be read from game+0x992088 at runtime.
6. **icon_inspectnumbers.png existence** — not verified in the game's media directory.

---

## 7. Conclusion

This plan covers all aspects of runtime verification for the Almanac button v2.12. The static investigation has identified the most credible hypotheses (center-anchor, coordinate-space mismatch, 0x5E6 partial input effect) and the plan provides specific test cases to confirm or refute each. All runtime steps require explicit authorization before execution. No tests were run in preparing this plan.
