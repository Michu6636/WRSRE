# WRSRE Economy & Trade handler trace — Almanac evidence

**Source:** `SOVIET64.exe` disassembly evidence in `C:\WRSRE\scratch\out/` and `C:\WRSRE\docs/ghidra-index/`.
**Method:** Static only. No game execution, no debugger, no hooks. All claims trace to instruction-level disassembly or decompilation artifacts.

## 1. Dispatch: which handler runs for Economy & Trade

Master function `FUN_140317DB0` (RVA `0x317DB0`) dispatches the selected sidebar entry by name. Relevant branches (from `14031eb5f_14031ec80_disasm.asm`):

| Entry name compared | Handler called | Call site RVA |
|---------------------|---------------|---------------|
| "system" | `FUN_140337450` | `0x31EAF8` |
| "infrastructure" | `FUN_140395C90` | `0x31EB5F` |
| "building" | `FUN_140389F10` | `0x31EB7F` |
| "workshopbuildings" | `FUN_140388140` (EDX=0) | `0x31EBA1` |
| "editorbuildings" | `FUN_140388140` (EDX=R13D) | `0x31EBC4` |
| "const_offi" | `FUN_1405F00D0` | `0x31ECFA` |
| "veh_replace" | `FUN_1405F00D0` | `0x31ED76` |
| "mtlist" | `FUN_1404A12E0` | `0x31ECAF` |
| **"trade"** | **`FUN_140373EA0`** | **`0x31EDF3`** (gated by `game+0x10398 != 0`) |
| **"importexport" / other** | **`FUN_140358A20`** | **`0x31EE76`** (fallback) |

**Verified:** "trade" reaches `FUN_140373EA0` only when `game+0x10398` is nonzero. The zero branch calls `361BB0` (not traced here). "importexport" is not in the explicit comparison list and reaches the fallback `FUN_140358A20`. Neither path is runtime-proven as the active Economy & Trade handler.

## 2. Handler 1: FUN_140373EA0 (trade branch) — verified instruction trace

**Artifact:** `140373ea0_1403829fd_disasm.asm` (VA `0x140373EA0`–`0x1403829FD`, 55,231 bytes). **Artifact:** `140373ea0_calls.txt` (188 calls). **Artifact:** `140373ea0_strings.txt` (36 strings).

### 2a. Early gate (VA `0x140373FCC`–`0x140374035`)

```
140373FCC: MOV EAX,dword ptr [RSI + 0x10be8]   ; EAX = game+0x10be8 (dirty counter)
140373FD2: CMP RAX,RDX                           ; compare with computed count
140373FD5: JNC 0x14037403A                       ; if counter >= count, skip to reset
...
14037403A: MOV dword ptr [RSI + 0x10be8],0xFFFFFFFF   ; game+0x10be8 = -1
```

If `game+0x10be8 >= count`, the function jumps to `0x1403826F7` (near end) — an early return path. Otherwise it sets `game+0x10be8 = -1` and proceeds.

### 2b. Sub-entry vector copy (VA `0x14037404D`–`0x1403740CF`)

Copies the sub-entry vector from `game+0xC7F0` (begin/end pointers) into a local buffer at `RBP+0x50`. Vector element size appears to be 16 bytes (copied via `MOVUPS`).

### 2c. Dirty-flag / soft-close handling (VA `0x1403740D0`–`0x14037410F`)

```
1403740D0: CMP byte ptr [RSI + 0x10970],0x0
1403740D7: JZ 0x140374114
...
1403740FA: MOV byte ptr [RSI + 0x10970],0x0     ; clear dirty flag
140374101: MOV qword ptr [RSI + 0x10978],R14    ; null a pointer
140374108: LEA RCX,[RSI + 0x10980]
14037410F: CALL 0x140201CC0                     ; FUN_140201CC0 (cleanup?)
```

If `game+0x10970` is set, it is cleared and `FUN_140201CC0` is called. This is the closest thing to a "close" path found statically, but it is not explicitly a panel-close handler.

### 2d. Section-button row loop (VA `0x14037413D`–`0x1403743E6`)

**Vector setup** (lines 145–158):
- `game+0xC650` = selected panel entry pointer
- `*(entry)+0x48` = sub-entry vector begin, `*(entry)+0x50` = end
- Count = `(end - begin) / 96` (computed via magic-number divide: `0x6666666666666667` then `SAR RDX,0x5`, i.e. divide by 96)
  - **Note:** stride is 96 bytes (0x60), not 0x50 (80) as previously documented. This needs verification against runtime layout.

**Per-iteration index reordering** (lines 159–179, VA `0x1403741A2`–`0x1403741CF`):

```
1403741A2: CMP EDI,0x2        ; EDI = loop index (0..count-1)
1403741A5: JGE 0x1403741AB    ; if EDI >= 2, special handling
1403741A7: MOV EBX,EDI        ; EDI 0 or 1: EBX = EDI (identity)
1403741CF: ...               ; fall through to draw
1403741AB: JNZ 0x1403741CC    ; EDI >= 2 and EDI != 2
1403741AD: ... count-1 path   ; EDI == 2: EBX = count - 1 (last item)
1403741CC: LEA EBX,[RDI + -0x1]  ; EDI > 2: EBX = EDI - 1
```

**Mapping summary:** EDI → EBX (actual sub-entry index drawn at this screen position):
| EDI (screen pos) | EBX (sub-entry index) |
|------------------|----------------------|
| 0 | 0 |
| 1 | 1 |
| 2 | count - 1 (last) |
| 3 | 2 |
| 4 | 3 |
| ... | ... |
| count-1 | count - 2 |

This is **not a linear row**. Item at logical index 2 is always drawn last (screen position 2 = vector index count-1). Items at logical indices 3..count-1 are drawn at screen positions 3..count-1 (mapped to vector indices 2..count-2).

**Per-button draw** (lines 184–192):
```
1403741F6: MOVSXD RAX,EBX
1403741F9: LEA R13,[RAX + RAX*0x4]    ; R13 = EBX * 5
1403741FD: SHL R13,0x4                ; R13 = EBX * 80 (0x50 stride)
140374201: ADD R13,qword ptr [R8]     ; R13 = &sub_entry[EBX]
140374204: MOV RCX,qword ptr [R13 + 0x48]   ; drawable pointer at sub_entry+0x48
140374208: MOV RAX,qword ptr [RCX]
140374214: CALL qword ptr [RAX + 0x70]  ; virtual draw call
```

**Click handling** (lines 217–255, VA `0x14037429D`–`0x14037435C`):

```
1403742A4: CALL qword ptr [0x14086C850]   ; Collision(mouse, button rect)
1403742AA: TEST AL,AL
1403742AC: JZ 0x140374366                 ; no hit, skip
1403742B2: MOV EDX,dword ptr [R13 + 0x40] ; text ID at sub_entry+0x40
1403742BD: CALL qword ptr [0x14086C890]   ; GetString(text ID)
1403742C3–1403742E2:                       ; copy string to game+0xD5A0
1403742F2: CMP byte ptr [0x140A54E91],CL   ; latch check (global at 0x140A54E91)
1403742F8: JZ 0x140374366                 ; latch not set, skip
1403742FA: CMP byte ptr [RSI + 0x14],CL    ; game+0x14 live-mode check
1403742FD: JNZ 0x140374347                 ; not live mode, skip state write
1403742FF–140374341:                       ; GetValueExist/GetValueString/PlaySound2D
140374347: CMP dword ptr [RSI + 0xC65C],EBX ; if selection changed
14037434D: JZ 0x140374356
14037434F: MOV byte ptr [RSI + 0x1039A],0x0   ; game+0x1039A = 0 (dirty flag clear)
140374356: MOV dword ptr [RSI + 0xC65C],EBX   ; game+0xC65C = EBX (selected sub-tab index)
14037435C: MOV dword ptr [RSI + 0x10BE8],0xFFFFFFFF  ; game+0x10BE8 = -1 (reset counter)
```

**Click effects (verified):**
- `game+0xC65C` ← clicked sub-entry index (EBX)
- `game+0x10BE8` ← -1 (counter reset)
- `game+0x1039A` ← 0 (dirty flag cleared) only if selection changed
- Sound `gui_subtab_click` played
- Selected overlay drawn when `game+0xC65C == EBX`

**Selected overlay** (lines 256–267):
```
140374366: CMP dword ptr [RSI + 0xC65C],EBX
14037436C: JNZ 0x14037437C     ; if not selected, skip overlay
14037436E: MOVUPS [0x1409BE30C], xmmword ptr [0x14090C4C0]   ; selected color
1403743A2: CALL qword ptr [0x14086C860]   ; Draw (selected overlay)
```

### 2e. Content sub-view call (VA `0x1403745F1`–`0x140374606`)

```
1403745F1: CMP dword ptr [RSI + 0xC65C],0x1   ; if selected sub-tab <= 1
1403745F8: JA 0x14037461B                      ; skip if > 1
1403745FA–140374603:                           ; set up args
140374606: CALL 0x14035F3E0                    ; FUN_14035F3E0(game, ...)
14037460B: MOVSS XMM10,[RSP + 0x70]
140374612: TEST AL,AL                          ; AL = return value (cVar4)
140374614: MOV byte ptr [RSP + 0x40],0x0
140374619: JNZ 0x140374620                     ; if AL != 0, skip
14037461B: MOV byte ptr [RSP + 0x40],0x1       ; else set flag
```

`FUN_14035F3E0` is called **only when `game+0xC65C <= 1`** (selected sub-tab is index 0 or 1). For sub-tabs 2+, this call is skipped entirely. The return value `AL` controls a flag at `[RSP+0x40]`.

### 2f. End of function (VA `0x1403826F7`–`0x1403829FA`)

The function ends with panel draw calls, collision checks, and shader setup. No explicit "close panel" or "return to native" handler is visible in the traced range.

## 3. Handler 2: FUN_140358A20 (fallback / importexport branch)

**Artifact:** `140358a20_decomp.c` (1,131 lines decompiled), `140358a20_calls.txt` (188 calls), `140358a20_xrefs.txt`.

**XRefs** (`140358a20_xrefs.txt`):
- Called only from `FUN_140317DB0` at RVA `0x31EE76` (the fallback branch)
- No other callers

**Decompiled structure** (`140358a20_decomp.c`):
- Draws a row of buttons using `FUN_1404461f0` for layout and `CALL qword ptr [RAX + 0x70]` for draw
- Click handling uses `GetMouseSolid` + `Collision` + latch at `0x140A54E91`
- Writes `game+0xC65C` and `game+0x10BE8` on click (same pattern as `FUN_140373EA0`)
- Calls `FUN_140497050`, `FUN_140494F60`, `FUN_140497140` for content sub-pages
- Calls `FUN_1406EE2A0` and `FUN_14003AAA0` (repeatedly)
- **No native slider or checkbox calls** in its call list

**Key difference from `FUN_140373EA0`:**
- Geometry: 92x92 backdrop, 85 pitch, 7px overlap, wrap-at-8 (vs `FUN_140373EA0`'s 64x64 backdrop, 64 pitch, no overlap)
- Reaches only when entry name is NOT "trade" (i.e., "importexport" or any other unmatched name)
- Superseded for the Economy & Trade row: the active handler is `FUN_140373EA0` when `game+0x10398 != 0`

## 4. FUN_14035F3E0 — content sub-view (6 sub-items) and stale-AL concern

**Artifact:** `14035f3e0_decomp.c` (737 lines). Called from `FUN_140373EA0` at VA `0x140374606` when `game+0xC65C <= 1`.

### 4a. What it draws (verified from decompilation)

- 6 sub-items in a loop (`iVar15 = 0..5`)
- String IDs: `0x68B`–`0x690` for indices 0–4, `0x2B6` for index 5
- Each item: `PrintLeftUnicode` label + `DrawFrom9Patch` 9-patch button + click handling
- Click test: `C3D_INPUT::GetMouseSolid` + `Collision` + latch `DAT_140A54E91`

### 4b. Click effects (verified from decompilation)

When a sub-item is clicked (latch `DAT_140A54E91 != 0`, live mode `*(char *)(param_1 + 0x14) == 0`):

| Sub-item index | `FUN_1403613b0` arg | `game+0x10F20` |
|---------------|-------------------|----------------|
| 0 | `0xFFFFFFFE` (-2) | 4 |
| 1 | `1` | 4 |
| 2 | `2` | 4 |
| 3 | `0xFFFFFFFD` (-3) | 4 |
| 4 | `1` (with `local_148`) | 4 |
| 5 | `2` (with `local_148`) | 4 |

- `FUN_1403FFAD0(param_1, "gui_click", 1)` — play click sound
- `FUN_1402FD840(param_1, iVar15, 0)` — sub-item state setter
- `game+0x10F20` = 4 (sub-page state)

### 4c. Return value (stale-AL concern)

```
char cVar4 = '\0';                          // line 177
...
if ((*(char *)(param_1 + 0x5E6) != '\0') &&   // line 178
    (cVar5 = FUN_140335BC0(), cVar4 = '\0', cVar5 != '\0')) {  // line 179
    cVar4 = '\x01';                           // line 180
}
return cVar4;                               // line 733
```

`cVar4` is `'\0'` (0) unless both: `game+0x5E6 != 0` AND `FUN_140335BC0()` returns non-zero. In the caller `FUN_140373EA0`:

```
140374612: TEST AL,AL          ; AL = return value of FUN_14035F3E0
140374614: MOV byte ptr [RSP + 0x40],0x0
140374619: JNZ 0x140374620     ; if AL != 0, skip
14037461B: MOV byte ptr [RSP + 0x40],0x1   ; if AL == 0, set flag to 1
```

**Stale-AL concern:** `AL` is the actual return value, not stale. However, **skipping the `FUN_14035F3E0` call is NOT safe** because:
1. It draws the selected sub-page content (6 sub-items with labels and buttons)
2. It handles sub-item clicks (calls `FUN_1403613b0` to switch sub-pages, sets `game+0x10F20 = 4`)
3. It plays `gui_click` sound on sub-item clicks
4. Without it, sub-tabs 0 and 1 show no content

## 5. Selection change, close, and return-to-native — what is known vs unknown

### 5a. Selecting an existing section button (verified)

Click on row button → `game+0xC65C` ← EBX (new sub-tab index), `game+0x10BE8` ← -1, `game+0x1039A` ← 0 if changed. Next frame, the row loop re-runs with the new selection, drawing the selected overlay on the clicked button and calling `FUN_14035F3E0` (if new index <= 1) to draw the corresponding sub-page content.

### 5b. Selecting another sidebar section (NOT verified in this handler)

Switching sidebar sections is handled by `FUN_14007C400` (RVA `0x1400843F3`), which writes `game+0xC650` (selected entry pointer). `FUN_140317DB0` then re-dispatches to a different handler based on the new entry name. `FUN_140373EA0` itself does not handle sidebar section switching — it only handles sub-tab selection within the current entry.

### 5c. Closing the panel / returning to native content (NOT established)

No explicit "close panel" or "return to native" instruction exists in the traced path of `FUN_140373EA0`. The closest candidates:
- `game+0x10BE8 = -1` (counter reset) — happens on every draw, not specifically on close
- `game+0x1039A = 0` (dirty flag clear) — happens only when selection changes
- `game+0x10970 = 0` + `FUN_140201CC0` call — dirty flag clear with possible cleanup, but not identified as panel-close

**Unknown:** What closes the Economy & Trade panel, what restores native sidebar content, and what happens to `game+0xC65C` on close. These require runtime observation (read-only snapshot).

## 6. Appended item analysis — what happens if a ninth item is added

The row loop draws from the sub-entry vector `*(game+0xC650)+0x48/+0x50`. Count = `(end - begin) / 96`. If a ninth item is appended:

| Screen pos (EDI) | Vector index (EBX), count=8 | Vector index (EBX), count=9 |
|------------------|----------------------------|----------------------------|
| 0 | 0 | 0 |
| 1 | 1 | 1 |
| 2 | 7 (last) | 8 (last) |
| 3 | 2 | 2 |
| 4 | 3 | 3 |
| 5 | 4 | 4 |
| 6 | 5 | 5 |
| 7 | 6 | 6 |
| 8 | — | 7 |

**Critical finding:** Appending a ninth item does NOT simply add it at the end. The item at logical index 2 moves from screen position 7 (count=8) to screen position 8 (count=9). The new item at index 8 appears at screen position 7. This reordering means "alongside the eight visible icons" is not achieved by simple vector append — the layout logic must be understood first.

**Also unknown:** Vector capacity. If the vector is fixed-size (e.g., exactly 8 entries), appending a ninth item would overflow. No bounds check is visible in the traced loop — it trusts the count from `(end - begin) / 96`.

## 7. Comparison summary

| Aspect | FUN_140373EA0 (trade) | FUN_140358A20 (fallback/importexport) |
|--------|----------------------|--------------------------------------|
| Dispatch condition | entry == "trade" AND `game+0x10398 != 0` | entry != any explicit name (incl. "trade") |
| Button backdrop | 64x64 (DPI * DAT_0x90AAA0) | 92x92 |
| Button pitch | 64 (DPI * 64.0), no overlap | 85 pitch, 7px overlap |
| Wrap | none visible | wrap at 8 |
| Index mapping | reordering (item 2 → last) | not traced in decomp |
| Content sub-view | `FUN_14035F3E0` (if sub-tab <= 1) | `FUN_140497050`/`FUN_140494F60` |
| Slider/checkbox calls | none | none |
| Active for Economy & Trade | Yes (when `game+0x10398 != 0`) | No (only for "importexport" or unmatched) |

## 8. What remains unknown (requires read-only in-game snapshot)

1. **Runtime dispatch:** Which handler is actually active — `FUN_140373EA0` (requires `game+0x10398 != 0`) or `FUN_140358A20` (fallback)? The `game+0x10398` gate is unverified at runtime.

2. **Vector capacity:** Can the sub-entry vector hold 9 entries, or is it fixed at 8? No bounds check visible in the loop.

3. **Stride verification:** Is the sub-entry stride 96 bytes (per magic-number division) or 80 bytes (per earlier docs)? This affects count calculation.

4. **Close/return path:** What closes the panel and restores native content? No close handler traced statically.

5. **Sidebar section switching:** How does `FUN_14007C400` write `game+0xC650` connect to mouse clicks on section buttons? The click→write path is not shown in the traced window.

6. **Appended item rendering:** If a ninth item is appended, does the reordering logic place it acceptably? Static analysis cannot confirm visual result.

## 9. Evidence files preserved (non-ignored; no EXE copied)

Folder: `C:\WRSRE\export\almanac-evidence\trade-handler-trace/`
- `report.md` — this report
- No new disassembly files created (all evidence is from existing `scratch/out/` artifacts cited above)

**Pre-existing evidence artifacts referenced (not overwritten):**
- `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` — FUN_140373EA0 disassembly
- `C:\WRSRE\scratch\out\140373ea0_calls.txt` — FUN_140373EA0 call list
- `C:\WRSRE\scratch\out\140373ea0_strings.txt` — FUN_140373EA0 string refs
- `C:\WRSRE\scratch\out\140358a20_decomp.c` — FUN_140358A20 decompilation
- `C:\WRSRE\scratch\out\140358a20_calls.txt` — FUN_140358A20 call list
- `C:\WRSRE\scratch\out\140358a20_xrefs.txt` — FUN_140358A20 cross-references
- `C:\WRSRE\scratch\out\14035f3e0_decomp.c` — FUN_14035F3E0 decompilation
- `C:\WRSRE\scratch\out\14031eb5f_14031ec80_disasm.asm` — master dispatch around 0x31EB5F–0x31EC80

**Bottom line:** The Economy & Trade row is drawn by `FUN_140373EA0` (when `game+0x10398 != 0`), which iterates the sub-entry vector with a non-linear index reordering (item 2 → last position), draws section buttons with click-to-select (`game+0xC65C`), and conditionally calls `FUN_14035F3E0` for sub-page content (only when sub-tab <= 1). The fallback `FUN_140358A20` handles "importexport"/unmatched entries with different geometry. Neither handler's suitability for a ninth appended item, nor the close/return-to-native path, is established statically. A read-only in-game snapshot of `game+0xC650`, `game+0xC65C`, `game+0x10BE8`, `game+0x1039A`, `game+0x10398`, and the sub-entry vector contents is required to resolve these questions.
