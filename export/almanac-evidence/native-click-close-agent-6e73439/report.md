# Almanac close-on-native-click — interaction investigation

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0), `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), `C:\WRSRE\plugins\ui_panels\ui_panels.cpp` (WidgetButtonImplEx), `C:\WRSRE\src\ui_panels_api.h`, sections 39a–52, 51
**Method:** Static only. No Ghidra started while locked (stale `.lock` left untouched; no ghidra process running). No game execution, no hooks, no writes.

## 1. Requirement restatement

The Almanac button must sit beside the eight native Economy & Trade icons. All eight native icons must remain **visible and clickable**. The Almanac panel must **close** when the player clicks **any** native row icon — including one **already selected** — without consuming the wrong click (i.e., without breaking native click handling or stealing the latch).

## 2. Native row handler click behavior — verified instructions

### 2a. FUN_140373EA0 (verified, `140373ea0_1403829fd_disasm.asm`)

Click handling path (lines 244–256):

```
140374347:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX   ; selected == this?
14037434d:  74 07                    JZ 0x140374356                      ; JUMPS TO the store when equal (already selected)
14037434f:  C6 86 9A 03 01 00 00     MOV byte ptr [RSI + 0x1039a],0x0   ; dirty flag clear — ONLY if selection changed
140374356:  89 9E 5C C6 00 00        MOV dword ptr [RSI + 0xc65c],EBX   ; game+0xC65C = EBX (always written, no-op if same)
14037435c:  C7 86 E8 0B 01 00 FF FF  MOV dword ptr [RSI + 0x10be8],-1  ; counter = -1 — ALWAYS on click, even if already selected
140374366:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX   ; selected overlay
14037436e:  ...MOVUPS [0x1409be30c],XMM0                            ; selected overlay color
```

**Already-selected click behavior:**
- `game+0xC65C` unchanged (write is a no-op)
- `game+0x1039A` NOT written (stays at previous value)
- `game+0x10BE8` = -1 **always** written — but may already be -1 from a previous already-selected click, so **no observable transition**
- Sound `gui_subtab_click` played — not observable via state reads

**Latch handling:**
```
1403742f2:  CMP byte ptr [0x140a54e91],CL   ; latch check
1403742f8:  JZ 0x140374366                  ; skip if no latch
```
The latch `DAT_0x140A54E91` is **checked but NOT cleared** by the button loop (section 51.1). It is cleared elsewhere (input system or master function — not identified statically).

### 2b. FUN_140361BB0 (verified, `export/ch_tab23_full.asm`)

Same pattern:
```
14036273f:  84 C0                    TEST AL,AL                    ; Collision result
140362741:  0F 84 F0 00 00 00        JZ 0x140362837                ; no hit
...
1403627b1:  84 C0                    TEST AL,AL                    ; latch
1403627b3:  74 52                    JZ 0x140362807                  ; skip if no latch
```
And at line 650:
```
1403628a3:  41 83 BE 5C C6 00 00 01  CMP dword ptr [R14 + 0xc65c],0x1
1403628ab:  77 29                    JA 0x1403628d6                  ; skip FUN_14035F3E0 if >1
```
Both handlers share the same click-latch idiom: `GetMouseSolid` + `Collision`, latch checked, state written on hit.

### 2c. Already-selected click — summary of observable effects

| Field | Already-selected click | Transition observable? |
|-------|----------------------|----------------------|
| `game+0xC65C` | unchanged (write is no-op) | No |
| `game+0x1039A` | NOT written | No |
| `game+0x10BE8` | = -1 (always) | Only if it wasn't already -1 |
| Sound `gui_subtab_click` | played | Not state-observable |
| Latch `0xA54E91` | checked (set by input system) | Racy; cleared elsewhere |

**Conclusion:** No state-field transition reliably detects a click on an already-selected native icon.

## 3. WidgetButtonImplEx latch/dedup logic — comparison (from `ui_panels.cpp`)

| Aspect | WidgetButtonImplEx (ui_panels.cpp) | Native row handlers |
|--------|-----------------------------------|---------------------|
| Hit-test | Engine `GetMouseSolid` + `Collision` pair (agrees with drawn tile) | Same idiom (`GetMouseSolid` + `Collision` at IAT `[0x14086c850]`) |
| Click latch | `0xA54E91` consumed on press + `GetAsyncKeyState(VK_LBUTTON)` edge fallback | `0xA54E91` checked but **not cleared** by button loop |
| Returns | 1 on exact frame of click (edge), 0 otherwise | State writes + sound (no explicit return) |
| Physical edge latch | `s_wasLButtonDown` + `s_clickConsumed` (v3.7) | None — relies on latch byte |
| Latch byte eaten | `0xA54E91` (confirm/click) — **different** from `0xA54C91` (ESC) | `0xA54E91` (confirm/click) |
| ESC interaction | Does NOT block ESC (different byte) | Same latch byte; ESC uses `0xA54C91` |

**Key distinction:** WidgetButtonImplEx owns its OWN tile and consumes `0xA54E91` for itself. A global Almanac button sitting beside native icons must NOT consume `0xA54E91` — natives need it for their own click handling. Consuming the shared latch would break native icon clicks.

## 4. Candidate observation/coordination points

### Candidate A — Mouse position recording (most evidence-backed)

Record cursor position (GetCursorPos + ScreenToClient, as WidgetButtonImplEx v3.3 does as fallback) **before** the native handler runs; after the handler, test if the position falls within any native button's 64×64 rect (X = 130 + n×64, Y = 15 + game+0xAE50, for n in rendered order).

**Pros:** Detects ALL clicks including already-selected; doesn't consume the latch; doesn't touch `game+0xC65C`.
**Cons:** Requires reading mouse state around the native handler — needs a hook before/after the function or reading mouse state independently. Timing is racy (mouse could move between native click and Almanac read).

### Candidate B — `game+0x10BE8` counter

Written to -1 on every click in both handlers. Almanac could record its previous value and check if it became -1.

**Cons:** If already -1 from a previous already-selected click, no transition. UNRELIABLE for already-selected clicks (section 51.1 explicitly debunks this).

### Candidate C — `game+0x1039A` dirty flag

Only written if selection changed (`0x14037434f`, conditional on `JZ` not taken).

**Cons:** NOT written on already-selected click. UNRELIABLE.

### Candidate D — Latch `DAT_0x140A54E91`

The Almanac could observe the latch being set.

**Cons:** Timing is racy (cleared elsewhere, not by button loop). Consuming it would break native click handling. Not a clean observation point.

## 5. Shared-state risks

| Shared state | Native use | Almanac risk if touched |
|--------------|-----------|------------------------|
| `0xA54E91` (click latch) | Natives check it at `0x1403742f2` / `0x14036273f`; cleared elsewhere | Consuming it breaks native icon clicks |
| `game+0xC65C` (selected index) | Natives write it on click | Writing synthetic index selects a native icon the user didn't click (unsafe) |
| `game+0xC650` (panel pointer) | `FUN_14007C400` writes it on section switch | Manually writing it targets arbitrary panel object (unsafe) |
| `game+0x10BE8` (counter) | Written -1 on every click | Writing it could mask native click detection |

## 6. What each approach closes Almanac on

| Observation point | Detects already-selected click? | Consumes native latch? | Touches game state? |
|-------------------|-------------------------------|----------------------|---------------------|
| Mouse position recording | Yes | No | No |
| `game+0x10BE8` transition | No (may already be -1) | No | No (read only) |
| `game+0x1039A` change | No (only if selection changed) | No | No (read only) |
| Latch `0xA54E91` observation | Yes (racy) | No (if read-only) | No (read only) |
| Latch `0xA54E91` consumption | Yes | YES — breaks natives | Yes |

## 7. What needs an authorized integration test

None of these can be confirmed statically:

1. **Does the latch `0xA54E91` get set for already-selected clicks?** Sound plays and state writes happen, suggesting yes — but the latch is checked at `0x1403742f2` before those writes, so if the latch weren't set, none of the click handling would run. This is inferable but not directly observable statically.

2. **Is `game+0x10BE8` reliably -1 only after clicks?** Other code paths may set it too. Tracing writers of `game+0x10BE8` is needed.

3. **What clears the latch?** The input system or master function — not identified statically. Timing of latch set vs. clear relative to a mod's read is racy.

4. **Mouse position recording feasibility:** Does reading `GetCursorPos` + `ScreenToClient` around the native handler reliably catch a click? Depends on frame timing; needs live measurement.

5. **Would Almanac closing on any native click (including already-selected) be the desired UX?** This is a design question, not a static one.

## 7. Evidence files preserved

Folder: `C:\WRSRE\export\almanac-evidence\native-click-close-20261110/`
- `report.md` — this report

**No new disassembly files produced.** All evidence from existing sources:
- `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0: `ch_tab23_full.asm:575-600`, `:650-651`, `:1593`)
- `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0: `:244-256`)
- `C:\WRSRE\plugins\ui_panels\ui_panels.cpp` (WidgetButtonImplEx: `s_wasLButtonDown`, `s_clickConsumed`, latch `0xA54E91`, `GetAsyncKeyState(VK_LBUTTON)` edge)
- `C:\WRSRE\src\ui_panels_api.h` (widgetButton contract: returns 1 on click frame, engine `GetMouseSolid`+`Collision` idiom)
- sections 39a–52, 51

**Bottom line:** NOT YET. The Almanac cannot safely close on a native icon click (including already-selected) because:
- Already-selected click: `game+0xC65C` unchanged, `game+0x10BE8` may already be -1, `game+0x1039A` unchanged — no observable state transition
- Latch `0xA54E91` is checked but not cleared by the button loop; consuming it would break native click handling
- Mouse position recording requires a hook before/after the native handler (not yet identified as safe) and has frame-timing race
- No clean, shared-state-free observation point exists statically

**Do not:** cover the icon row (all 8 icons + Almanac button must stay visible and clickable), consume `0xA54E91`, write synthetic `game+0xC65C`, or manually write `game+0xC650`.
