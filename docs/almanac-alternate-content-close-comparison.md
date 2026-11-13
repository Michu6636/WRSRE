# Static comparison: native insertion vs. separate same-row button (revised draft)

**Scope.** Compares two ways of putting an Almanac entry on the Economy & Trade row, for three questions: the alternate row handler, native content, and close/return. This is a comparison only. Nothing is implemented. V1 has not been run. The game and the mod were not modified.

**Status.** Revised draft, not committed. It supersedes the earlier uncommitted draft. The committed reports are unchanged. Their statements that this draft corrects are listed in §9.

**Basis.** Commit `46679f2` on `arena/aea12c74-wrsre` holds [`almanac-row-item-vs-button-analysis.md`](almanac-row-item-vs-button-analysis.md), the mechanism report, and the raw excerpts in `export/almanac-row-item-analysis-20261110/`. The DLL disassembly used for this revision is `scratch/rep/disasm.txt`, produced from the DLL whose hash is `fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a`.

**Approaches**

- **A. Native insertion (row item).** The DLL appends a 0x50-byte sub-entry to the trade record's sub-list. This is the `sidebar_item` option, shipped as `0`.
- **B. Separate same-row button.** The mod draws and hit-tests its own button beside the native tiles. It does not write the native vector.

**Labels**

- **[D]** DLL bytes or disassembly. Addresses are given so they can be checked.
- **[N]** Native listing preserved in the repo (`N01`–`N08`). The game binary is not in the workspace, so these are not verified against the installed game.
- **[C]** Configuration value or author claim in `reputation.ini`.
- **[I]** Inference from the labelled evidence.
- **[STATE]** Depends on runtime state. The static evidence does not fix which state applies.
- **[OPEN]** Not determined by the static evidence.
- **[RT-n]** A runtime assumption, listed in §6. It has not been checked.
- **[DESIGN]** A constraint on any later implementation. It is not a finding about the game.

## 1. Shared facts

### 1.1 The literal setting and what it says

- **[C]** `reputation.ini` sets `sidebar = 1`, `sidebar_hides_panel = 1`, `in_panel = 1`, `in_panel_w = 1280`, and `sidebar_item = 0`.
- **[I]** The config comment treats `sidebar_hides_panel` as a plain setting. The code does not treat it that way when `in_panel` is on (§1.2).

### 1.2 Effective value of `sidebar_hides_panel` at load (DLL 0x180048CC2–0x180048D14)

The block is in the config loader that starts at `0x180047D40`. Its caller is at `0x180068C91`; the caller's function was not identified in this pass.

1. **[D]** It reads the effective `in_panel` (`0x1800C32EC`) and `sidebar_hides_panel` (`0x1800C3C74`), and copies the hides value to a saved copy at `0x1801A3FD4` (`0x180048CC2`–`0x180048CCE`).
2. **[D]** If effective `in_panel` is 0, it jumps past the block (`0x180048CD6`).
3. **[D]** If `sidebar` (`0x180280D80`) is 0, it logs "in_panel needs [ui] sidebar - in_panel off, the windows float" (string `0x1800ABB20`), sets the raw `in_panel` (`0x180280D74`) and effective `in_panel` to 0, and skips the rest (`0x180048CE1`–`0x180048D00`).
4. **[D]** If `sidebar` is nonzero and the hides value is nonzero, it logs "in_panel needs the game's panel on screen, so sidebar_hides_panel is off for this run" (string `0x1800ABB70`) and **writes 0 to `0x1800C3C74`** (`0x180048D02`–`0x180048D0D`).
5. **[D]** If effective `in_panel` is still nonzero and `menu_dock` (`0x1800C3780`) is 0, it sets `menu_dock` to 1 and logs "in_panel puts every window in one panel, so menu_dock is on for this run" (`0x180048D1F`–`0x180048D3D`).

**Reading.** **[D]** With `in_panel` on and `sidebar` on, the literal value 1 is replaced by 0 at this point in the loader. This is the code's own statement, in its log text, that the flag is "off for this run". Whether the loader runs once per session, and whether its timing is before any selection, was not established [OPEN].

### 1.3 Other writers of `sidebar_hides_panel` (DLL, found in this pass)

| Address | What it does | Condition | Evidence |
|---|---|---|---|
| `0x180048454` | Config assignment (the literal value). | Loader. | [I] from the write's position in the loader |
| `0x180048D0D` | Forces 0. | `in_panel` and `sidebar` on, value was nonzero (§1.2). | [D] |
| `0x18002DA61` | Restores the saved copy from `0x1801A3FD4`. Same function also clears effective `in_panel` and `0x1801A3FA8`. | Called from at least eight sites, including `0x180038F0A`, `0x180058B82`, `0x180058EF0`, `0x180059202`, `0x18005949C`, `0x180068693`, `0x180068825`, `0x18006982F`. | [D]. The caller conditions were not traced. |
| `0x18005925C` | Restores the saved copy. | Path in the function containing `0x180059230`, on a change to effective `in_panel`. | [D]. Conditions partly traced. |
| `0x18005717F` | Forces 0. Logged as "clearing the selection faulted - left alone from here, and the button latches instead of pushing" (string `0x18009EBF0`). | A fault path. | [D] |
| `0x1800577FC` | Conditional code observation: forces 0 when the counter `0x1804DE988` is at least 8 and the flag is nonzero. The counter is incremented by the reputation-select transition, not by physical clicks (§1.5). The log string at `0x18009F220` is quoted only as text in the binary; it is not evidence about input or intent. | The reputation-select path (§1.5). | [D] for the code path. [I] for the counter's meaning. The log text's meaning for the flag is not resolved. |

**Reading.** **[STATE]** The effective value of `sidebar_hides_panel` is not fixed by the config. It is 0 when the load-time block applies, and it can be restored or forced by the other writers. The value during a session is not determined by this pass. Any statement that depends on it is conditional on the state named in that statement.

### 1.4 Gates that read `sidebar_hides_panel`

- **[D] Close block** (`0x180057710`): on a selected-to-unselected transition, the close block runs only when the value is 0 (§4.2).
- **[D] Reputation-select dispatch** (`0x18005779E`): the branch on this value decides between two paths on the reputation-select transition (§1.5).
- **[D] Re-select rule** (`0x1800576C4`): does **not** read this value. I checked the instructions from `0x18005769C` to `0x1800576C4` (§4.1).

### 1.5 Dispatch sites for `0x18002F1E0` and the reputation-select transition

- **[D]** `0x18002F1E0` has four call sites: `0x180057678` (trade-row change), `0x1800577B0` (reputation select, see below), `0x18002F7A1` and `0x18005C7AA`. The last two were not traced.
- **[D] Reputation-select transition** (`0x180057799`, reached when the state flag `0x180411BC8` becomes nonzero at `0x18005770A`). The branch on `sidebar_hides_panel` at `0x18005779E` gives four cases, keyed on the window flags (`0x180001C80`):

  | `sidebar_hides_panel` | Window flags | Code | Result |
  |---|---|---|---|
  | 0 | all zero | dispatch with `0x180411BCC` (`0x1800577AA`–`0x1800577B0`) | dispatcher runs |
  | 0 | any set | nothing (`0x1800577A6`–`0x1800577A8`) | none |
  | nonzero | all zero | dispatch with `0x180411BCC` (`0x18005781C`, then `0x1800577AA`) | dispatcher runs |
  | nonzero | any set | case loop stores the first passing index into `0x180411BCC`, then clears `0x180286080`, `0x180286084`, `0x1800C4C50`, `0x1801CDA1C`, `0x180184EB4`, `0x1801CDA14`, `0x1801CDA18`, `0x1800C3158`, and sets `0x1800C3170` to 1 (`0x180057820`–`0x180057883`) | flags cleared |

  **Reading.** **[D]** The dispatch runs on this path whenever the flags are all zero, for either value of `sidebar_hides_panel`. The earlier committed report describes the dispatch only on the trade-row path. This path is an addition.
- **[D] Counter (conditional code observation).** On the reputation-select path, the counter `0x1804DE988` is incremented (`0x1800577D2`–`0x1800577DA`) when the gap between `0x1801A3FD0` and `0x1804DE984` is under 30, and reset to 0 (`0x180057888`) otherwise. When it reaches 8 and `sidebar_hides_panel` is nonzero, `0x1800577FC` writes 0 to that flag (§1.3). This is a code path that a reputation-select transition can take. It is **not** shown to prevent re-selection: the re-select rule (§4.1) does not read the counter or the flag, and the counter's own stores do not clear the window flags. The reset path also calls `0x180057110`, which was not traced. It is **not** a count of physical clicks: it increments on state transitions, gated by a frame-gap value.

### 1.6 Alternate handler and native click, from the preserved listings

- **[N, N01]** The host picks the alternate handler when `[game+0x10398] == 0` (`0x14031EDE6`–`0x14031EE13`), and the normal handler otherwise (`0x14031EDEF`). R15 is zeroed at `0x14031EAD5`. The runtime value of `[game+0x10398]` on the trade page was not established.
- **[N, N05–N06]** In the examined alternate handler (`0x140361BB0`), the loop variable `ESI` runs while it is below `count-1` (`CMP ESI,EAX` at `0x140362243`, `JL` at `0x140362245`). The bound is recomputed on each iteration. Entries `0..count-2` are drawn, and the entry at `count-1` is not drawn and not clicked. This is a statement about this handler and this vector only.
- **[N, N02–N04]** In the examined normal handler (`0x140373EA0`), the loop runs while `EDI < EDX` (`0x1403743DE`–`0x1403743E0`). The mapping is: `EDI < 2` gives `EBX = EDI`; `EDI == 2` gives `EBX = count-1`; `EDI > 2` gives `EBX = EDI-1`. The entry at `count-1` is drawn in third position.
- **[N, N04, N07]** A click on a native tile writes the item index to `[g_ui+0xC65C]` (`exe+0x9E156C`). Normal handler: the mapped index (`0x140374356`). Alternate handler: the vector index (`0x140362193`).
- **[N, N02, N06]** Skip tests in the normal handler: `[game+0x578]` (mapped 3 and 4), and `[game+0x5E0]` (mapped 6). The alternate handler's test on `[R14+0x578]` is at `0x140361FC0`. Its test on `[R14+0x5E0]` was not decoded [OPEN].

### 1.7 Scope of the alternate-handler statement

**[N] with [STATE].** The statement "the alternate handler does not draw the last entry" is established for the examined handler and for a vector whose count is known. It does **not** establish what the player currently sees. With eight vector entries the examined handler draws indices 0–6, and with nine it draws 0–7. The live trade page may show a different count because of the active handler (RT-1), the skip tests (§1.6), and the runtime vector. No runtime evidence in this pass shows a missing icon on the player's current view.

## 2. Alternate row handler

| Question | A. Native insertion | B. Separate button |
|---|---|---|
| Examined alternate handler, eight vector entries | Draws indices 0–6 [N, bound `count-1`]. Index 7 is not drawn. | Same native handler and vector. Native behaviour unchanged by B. |
| Examined alternate handler, nine vector entries (Almanac appended as index 8) | Draws indices 0–7. The Almanac (index 8) is not drawn and not clickable [N]. The DLL still records the insertion as successful [D, D01/D04]. | Native behaviour unchanged by B. |
| Examined normal handler, nine entries | The Almanac is drawn in third position and native 2–7 shift right [N, N02]. | Native behaviour unchanged by B. |
| Which handler is active on the live trade page | [RT-1, OPEN]. Not established. | Same. B's placement may depend on it (below). |
| Position of the button | Native layout decides it. | Must be computed. If placed after the last drawn tile, the position depends on the active handler. A fixed coordinate avoids that dependency. The row origin and pitch were not decoded [OPEN]. |
| Can the mod tell which handler is active | No check exists in the insertion path [D]. | Possible only if the mod reads `[game+0x10398]`, which is not established as readable without a runtime check [RT-1, RT-9]. |

**Reading.**

- **[STATE]** With the examined alternate handler, approach A's appended entry is silently absent. This is conditional on that handler being active (RT-1).
- **[N, D]** Under the examined alternate handler, native index 7 is not drawn when the vector has eight entries. Whether the player's current view shows seven or eight icons is **not established** by this pass. It needs a runtime observation (V1, §6).
- **[I]** The requirement that all eight native icons and the Almanac are visible and clickable cannot be checked statically for either approach. Both approaches depend on the active handler and the skip tests.
- **[OPEN]** What sets `[game+0x10398]` was not traced. It may decide the choice between the approaches more than any other item.

## 3. Native content

**Traced.**

- **[D, D05, D13]** The DLL's reputation body and layout run only while the reputation test `0x180057D50` is true. A row click does not make it true.
- **[D, D09–D11]** The row-click dispatch sets DLL flags for case 0. It does not draw content in the traced path.
- **[N, N04, N07]** A click on a native tile writes the item index. Under the normal handler the appended item's index is 8. The DLL's trade predicate reads the same index (`0x180058310`, D06). The selection identity is therefore consistent between the native click and the DLL check.

**Not traced.** The following drawing paths were not traced. The row-item content is therefore **UNRESOLVED**.

1. The native trade panel's own draw path for the selected index 8. It may show nothing, or reuse content for another index [RT-4].
2. The draw functions for the DLL window states that the dispatcher can enable. Only the case-0 effect is decoded. The cases beyond 0, and the functions they call, were not decoded [OPEN, RT-5].
3. Any other `0x18002F1E0` call site (`0x18002F7A1`, `0x18005C7AA`).

**Consequence.** **[UNRESOLVED]** No statement is made here about what appears on screen for the row item, for either approach.

| Question | A. Native insertion | B. Separate button |
|---|---|---|
| Content for the Almanac item | UNRESOLVED (above). | Must be built. Nothing in the DLL draws it yet. |
| Reuse of the reputation body | Not as it stands: the body's gate is the reputation selection, which a row click does not set [D]. Changing that gate would change the sidebar behaviour too, since the same test drives layout and resizing [D, D13]. | Possibly. A new gate on the mod's own state would be needed. The body guard `0x180039400` and dispatch `0x180038F30` were only partly decoded [OPEN]. |
| Native trade content suppression | Not required for native content. Whether the mod's own window would be drawn over the native panel is [OPEN]. | Required. The mechanism for suppressing native trade content on the trade page is [OPEN]. The `ui_panels` precedent covers building windows only [C]. |
| Panel geometry | Native trade panel geometry, which the DLL does not resize on this path [D, D13, I]. | Must be defined. `in_panel_w` and `in_panel_bottom` set the sidebar panel, not the trade page [C, `reputation-sidebar-ui-20261110/19_in_panel_w_bottom_sources.txt`]. |

## 4. Close and return

### 4.1 Re-select rule (`0x1800576C4`)

**[D]** It runs only when all of these hold:

1. Effective `in_panel` (`0x1800C32EC`) is nonzero (`0x18005769C`).
2. `0x180057D50` returns 0, meaning reputation is not selected now (`0x1800576A6`–`0x1800576A8`).
3. The **previous** state flag `0x180411BC8` is 0 (`0x1800576AA`). The state flag is written later, at `0x180057702`, so this check reads the previous call's value.
4. `0x180001C80` is nonzero, meaning some window flag is set (`0x1800576B2`–`0x1800576B9`).
5. The trade predicate `0x180058310` is 0 (`0x1800576BB`–`0x1800576C2`).

The instructions from `0x18005769C` to `0x1800576C4` do **not** read `sidebar_hides_panel`. The re-select rule is therefore independent of that value.

### 4.2 Close block (1→0 transition)

**[D]** It runs only when the state flag goes from nonzero to zero and `sidebar_hides_panel` is 0 (`0x180057710`). Then, if a window flag is set and the trade predicate is false, it clears `0x180286080` and `0x180286084` (if `0x180286080` was set), logs "another sidebar entry chosen - panel closed" (string `0x18009F1E8`), clears the six flags `0x1800C4C50`, `0x1801CDA1C`, `0x180184EB4`, `0x1801CDA14`, `0x1801CDA18`, and `0x1800C3158`, and sets `0x1800C3170` to 1 (`0x180057745`–`0x180057769`).

With `sidebar_hides_panel` nonzero the close block is skipped. Under the literal config value 1, no close happens. Under the value the loader forces to 0 (§1.2), it can happen. The effective value is state-dependent (§1.3).

### 4.3 H1: re-select override after a native click (state-dependent hypothesis)

**H1, as a hypothesis.** After a native sidebar click on an entry other than reputation or trade, the DLL may write the reputation record into `exe+0x9E1560` (§4.1). That write could override the click for that handler call.

**Preconditions (all [D] unless marked).**

- (i) Previous state flag 0 (§4.1 item 3).
- (ii) Reputation not selected now (§4.1 item 2).
- (iii) Effective `in_panel` nonzero (§4.1 item 1).
- (iv) At least one window flag set (§4.1 item 4).
- (v) Trade predicate false (§4.1 item 5).
- (vi) The selection handler runs on that click [RT-3].
- (vii) The native consumer of `exe+0x9E1560` honours the write [RT-6, OPEN].

`sidebar_hides_panel` is **not** a precondition (§4.1).

**How the window flags get set.** Two routes, both [D]:

- **Approach A.** The row-item dispatch, when all flags are zero (`0x180057678`).
- **Baseline mod.** The reputation-select dispatch, when all flags are zero (`0x1800577B0`), for either value of `sidebar_hides_panel` (§1.5).

So H1 is **not specific to approach A**. It is a property of the existing flag mechanism, reachable whenever the flags are set and the other preconditions hold.

**How the window flags get cleared.** [D]

- **Close block** (§4.2), on a 1→0 transition, only when `sidebar_hides_panel` is 0 and the trade predicate is false.
- **Reputation-select path** (§1.5), when the value is nonzero and a flag is set. It clears all seven flags.

No other clearing path was found. This pass did not trace every writer of the flags.

**Consequences, by state.**

- **[STATE] Baseline, effective `sidebar_hides_panel` 0.** Leaving reputation (1→0) clears the flags, so H1 cannot follow from that exit alone.
- **[STATE] Baseline, effective `sidebar_hides_panel` nonzero (including the literal 1).** Leaving reputation does not clear the flags. A later 0→0 click can then meet H1's preconditions.
- **[I, STATE] Approach A.** A row click sets the flags. Clicking another native entry afterwards is a 0→0 transition, and the trade predicate is false, so H1's preconditions can hold **regardless of the hides value**. Only the clearing paths above remove the flags.
- **[I, RT-6] Repetition.** If an override happens, the state flag becomes 1. The next click away is a 1→0 transition. With the value nonzero, the flags persist, and the following 0→0 click can be overridden again. This could alternate. Its visible effect depends on RT-6.
- **[I] Counter (§1.5), conditional.** The counter path exists in the DLL and can write the hides value. Its own stores write the counter and the hides value, not the window flags or the selection. The reset path also calls `0x180057110`, which was not traced. The re-select rule does not read the counter. It is therefore not evidence that H1 is prevented or limited. Whether the path is reached in practice, and how often, was not established [RT-3, RT-7].

### 4.4 Events

| Event | A. Native insertion | B. Separate button |
|---|---|---|
| Row-item click (A) or button click (B), flags clear | Trade-row dispatch with the stale index (0 if never stored). `0x1800C3158` becomes 2 and `0x1800C3170` becomes 1 [D, D10]. The log prints [D, D05]. Whether a window appears is [OPEN]. | Writes no DLL flags [DESIGN]. Its state is its own. |
| Click another native entry (0→0) with flags set | H1 candidate (§4.3). Applies after a row click regardless of the hides value [I, RT-3, RT-6]. | Does not apply if B sets no DLL flags [DESIGN]. |
| Reputation selected (0→1), flags clear | Stale-index dispatch on the reputation path (both hides values) [D]. Same as baseline. | Same as baseline. |
| Reputation selected, flags set | Hides 0: no change. Hides nonzero: clears all seven flags and sets `0x1800C3170` to 1 [D]. | Same, as baseline. |
| Leave reputation for a non-reputation entry (1→0) | Hides 0 with a flag set and the trade predicate false: close block, flags cleared, log [D]. Hides nonzero: no close [D]. | Same, as baseline. |
| Return to trade by a native trade tile | The predicate is true only if the selected record is `trade` and the index is 8 [D, D06]. The close block is not reached by a 0→0 click [D]. | The mod must detect the change itself [DESIGN, RT-8]. |
| Leave trade by another native tab | Same as a 0→0 click (see the H1 row above) [D, I]. | The mod must hide its own content when the native page changes [DESIGN]. |

## 5. Summary

| Question | A. Native insertion | B. Separate button |
|---|---|---|
| Examined alternate handler | If active, the Almanac entry is never drawn or clicked [N]. | Native handler is unchanged. Whether native index 7 is visible depends on the active handler and the skip tests [STATE, RT-1]. |
| Native content | Native for the tile. The Almanac window content is **UNRESOLVED** (§3). | Must be built. Native trade content suppression is **OPEN** (§3). |
| Close and return | H1 applies after a row click regardless of the hides value [I]. Baseline H1 depends on the effective hides value [STATE]. | Avoids H1 only if it sets no DLL flags [DESIGN]. Needs its own change detection [RT-8]. |
| Native order | Changes under the normal handler [N]. | Preserved. |
| Native vector | Mutated by an append with no DLL capacity check [D]. | Not touched. |

**Overall reading.** Neither approach can be chosen from the static evidence. Both depend on the active handler, which is a runtime fact (RT-1). A is handler-sensitive, and its failure under the alternate handler is silent. B needs new work for content, suppression, and change detection. The close/return hazard H1 is a property of the existing flag mechanism. Approach A adds a route to it through the row click. Approach B avoids that route only if it sets no DLL flags.

## 6. V1 and the runtime assumptions register

### 6.1 V1 is a live modification test

**V1 is a live modification test. It is not read-only.** It installs the change under test (`sidebar_item = 1` in a test configuration) into a running game. It therefore changes the runtime state, and it must run on a disposable save copy with separate authorisation. It has **not** been run.

**Pass criteria.** A pass requires all of the following:

1. All eight native icons and the Almanac icon are visible on the trade page at the same time.
2. None of the nine icons overlaps another, and none is clipped.
3. Each of the nine icons is clickable and produces the expected selection. Each click is verified separately, including the Almanac icon.
4. The handler in use is recorded (RT-1).
5. No fault appears in the game or the DLL log.
6. Leaving and returning to the trade page leaves native content and sidebar state intact.
7. No save is written during the test.

Any missing, overlapping, clipped, or unclickable icon is a failure.

**Note on the committed report.** The committed analysis report describes V1 as "one bounded, read-only, non-pausing session" (its §1 item 5 and §6.2). That wording is superseded by this section. The committed file is unchanged, as instructed.

### 6.2 Runtime assumptions

Each assumption must be checked before it is relied on. None has been checked.

| ID | Assumption | Basis | Affects | Check that would test it (not run) |
|---|---|---|---|---|
| RT-1 | On the trade page, `[game+0x10398] != 0` (normal handler). | Screenshot inference only [I]. | Alternate-handler behaviour, both approaches | Read the field during V1. |
| RT-2 | A click on a tile writes the index as in N04/N07. | Listings, not verified against the binary [N]. | Click identity, both | Compare the binary bytes at those addresses. |
| RT-3 | The DLL selection handler runs after a native selection change. | The trigger is not identified (no DLL caller found) [OPEN]. | H1; B's change detection | Observe the handler's calls during V1. |
| RT-4 | Native trade content for index 8 draws something. | Not traced [OPEN]. | A's content | Observe the native panel after the appended tile is clicked. |
| RT-5 | Setting `0x1800C3158` or other flags opens a visible window. | Dispatch cases beyond 0 not decoded [OPEN]. | A's content and visible state | Observe whether a window appears. |
| RT-6 | The native sidebar honours the DLL's re-select write to `exe+0x9E1560`. | The consumer of that address is not identified [OPEN]. | H1 severity | Observe the selection after another sidebar click. |
| RT-7 | The effective `sidebar_hides_panel` value during a session. | State-dependent (§1.2–§1.3). The literal config is 1. | Close block, reputation-select path, H1 | Read the field at several points during V1. |
| RT-8 | The per-frame terrain hook runs while the trade page is shown. | Earlier pass [D]. | B's change detection | Observe the hook's activity. |
| RT-9 | Nine tiles fit the panel. | Row origin and pitch not decoded [OPEN]. | Both approaches' layout | Measure the tile rectangles during V1. |
| RT-10 | The stale dispatch index on the reputation-select path is 0 in a session that has not stored one. | Initial value of `0x180411BCC` (earlier pass); the store happens only with a flag set [D]. | Whether the first reputation select sets `0x1800C3158` | Read `0x180411BCC` and the flags during V1. |

## 7. Open items that could change the comparison

1. **What sets `[game+0x10398]`.** It decides whether the normal handler is active.
2. **The native consumer of `exe+0x9E1560`.** It decides how serious H1 is.
3. **The draw functions for the DLL window states** that the dispatcher can enable.
4. **The native content path for index 8** on the trade page.
5. **The row origin, pitch, and panel width**, to check whether nine tiles fit.
6. **Every writer of the seven DLL flags and of `sidebar_hides_panel`.** §1.3 lists the writers found, not all of them.
7. **The caller of the config loader** (`0x180068C91`) and when it runs relative to the first selection.
8. **The conditions of the `0x18002DA30` callers** (§1.3). They decide when the saved hides value is restored.
9. **The skip test on `[R14+0x5E0]`** in the examined alternate handler.
10. **The other `0x18002F1E0` call sites** `0x18002F7A1` and `0x18005C7AA`.
11. **The meaning of `0x1801A3FD0`** as a frame counter (§1.5).

## 8. Constraints for any later implementation (not implemented)

- **[DESIGN] Approach B must not set any of the seven DLL flags.** Setting one enables the re-select rule (§4.1, §4.3).
- **[DESIGN] Approach A must account for H1 after a row click**, whatever the hides value.
- **[DESIGN] No change may rely on the literal `sidebar_hides_panel`** without first reading its effective value (§1.2–§1.3).
- **[DESIGN] Any implementation must check the active handler** before it claims that all eight native icons are visible.
- **[DESIGN] No native vector or selection write** should be made without the runtime checks in §6.

## 9. Corrections to committed reports (committed files unchanged)

The committed files are preserved as they are. The statements below are superseded by this draft.

1. **`almanac-row-item-vs-button-analysis.md`, §2.2 close rule.** It says the shipped value 1 means "the close block is not reached". That holds for the literal value only. The effective value is state-dependent (§1.2–§1.3, §4.2).
2. **Same file, §1 item 5, §6.2, and the V1 description.** It calls V1 "read-only, non-pausing". V1 is a live modification test (§6.1).
3. **Same file, §1 item 3, §4.4, and §5 table.** It states that native index 7 is omitted in the alternate handler, in the present tense and as a property of the live view. The statement is scoped in §1.6–§1.7 and §2 to the examined handler and vector. The live view is not established.
4. **Same file, §3.4.** It describes the dispatch only on the trade-row path. The reputation-select transition is a second dispatch site (`0x1800577B0`), with its own state dependence (§1.5).
5. **`reputation-sidebar-ui-mechanism.md`, §4 step 4.** It describes the latching branch as the one traced. The effective hides value is set by the loader and the other writers listed in §1.2–§1.3, which this file does not describe.

## 10. Status

- Static comparison only. Nothing is implemented. V1 has not been run.
- The game and the mod were not modified. No hook was installed.
- This revision is **not committed**. Commit `46679f2` contains the earlier report and the raw evidence, and it is unchanged.
