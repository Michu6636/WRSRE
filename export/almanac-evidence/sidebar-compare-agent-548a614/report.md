# Trade handler content-call comparison (bounded) — Almanac design evidence

**Source:** `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0, verified by text search), `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), sections 39a–52
**Method:** Static only. No Ghidra analysis started while project locked; existing `.lock` file left untouched (stale, no ghidra process running). No game execution, no hooks, no writes.

## 1. Verified call at 0x1403628b8 (FUN_140361BB0)

Confirmed in `C:\WRSRE\export\ch_tab23_full.asm` at line 655:

```
1403628a3:  41 83 BE 5C C6 00 00 01  CMP dword ptr [R14 + 0xc65c],0x1   ; selected <= 1?
1403628ab:  77 29                    JA 0x1403628d6                      ; skip if > 1 (unsigned)
1403628ad:  4C 8D 45 E0              LEA R8,[RBP + -0x20]              ; arg3
1403628b1:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]             ; arg2
1403628b5:  49 8B CE                 MOV RCX,R14                       ; arg1 = game
1403628b8:  E8 23 CB FF FF           CALL 0x14035f3e0                   ; FUN_14035F3E0(game, &Y, &X)
1403628bd:  0F B6 DB                 MOVZX EBX,BL
1403628c0:  84 C0                    TEST AL,AL                         ; return value
1403628c2:  41 0F 45 DC              CMOVNZ EBX,R12D                   ; if AL != 0, EBX = R12D
1403628c6:  89 5D 80                 MOV dword ptr [RBP + -0x80],EBX   ; store local output
```

**Guards before the call:** line 649 `TEST R15,R15` / `JNZ 0x1403628d6` (skip if R15 nonzero), then line 650-651 `CMP [R14+0xc65c],0x1` / `JA` (skip if selection > 1).

**Return-value handling:** `AL` = click indicator; if nonzero, `EBX` = `R12D`; stored at `[RBP-0x80]`. This local output is used downstream (section 51 noted `[RSP+0x40]` equivalent handling in FUN_140373EA0).

## 2. Selection-dependent call comparison (indices 0–6)

### FUN_140373EA0 (verified, `140373ea0_1403829fd_disasm.asm`)

| Selection | Calls FUN_14035F3E0? | Instruction | Notes |
|-----------|---------------------|-------------|-------|
| 0 | Yes | `CMP [RSI+0xc65c],0x1` / `JA 0x14037461b` skips if >1 (`0x1403745f1`-`0x140374606`) | 0 <= 1, calls |
| 1 | Yes | Same guard | 1 <= 1, calls |
| >1 | No | `JA` taken at `0x1403745f8` | Content block runs without sub-page |
| 3,4,5,6 | No (same as >1) | No selection-specific calls beyond guard | No index-specific dispatch |

### FUN_140361BB0 (verified, `ch_tab23_full.asm`)

| Selection | Behavior | Instructions |
|-----------|----------|--------------|
| 0 | Calls FUN_14035F3E0 | `CMP [R14+0xc65c],0x1` / `JA` skips if >1 (`ch_tab23_full.asm:650-655`) | 0 <= 1, calls |
| 1 | Calls FUN_14035F3E0 | Same guard | 1 <= 1, calls |
| 2 | Complex branch | `CMP [R14+0xc65c],0x2` / `JNZ` skips (`ch_tab23_full.asm:1593-1594`) | Enters panel-list iteration block (lines 1593-1614+) |
| 3 | Calls `0x1404acf50`, then JMP out | `LEA EAX,[RCX-0x3]` / `CMP EAX,0x1` / `JA` skips if >1 (`ch_tab23_full.asm:476-481`) | `3-3=0 <= 1`, calls `0x1404acf50` |
| 4 | Calls `0x1404acf50`, then JMP out | Same (`4-3=1 <= 1`) | Calls `0x1404acf50` |
| 5 | Calls `0x1404b0e20` | `CMP ECX,0x5` / `JNZ` skips (`ch_tab23_full.asm:482-486`) | Selection == 5 |
| 6 | Calls `0x1404b24a0` | `CMP ECX,0x6` / `JNZ` skips (`ch_tab23_full.asm:492-498`) | Selection == 6 |
| >1 (general) | Skips FUN_14035F3E0 | `JA` at `ch_tab23_full.asm:651` | Content block runs without sub-page |

### Key difference
FUN_140373EA0 treats all selections >1 identically (no FUN_14035F3E0, no index-specific dispatch). FUN_140361BB0 has **selection-specific calls for indices 3, 4, 5, 6** that route to different functions (`0x1404acf50`, `0x1404b0e20`, `0x1404b24a0`) and a complex branch for selection 2. Any replacement boundary in FUN_140361BB0 must account for these additional paths.

## 3. Output locals and state writes (both handlers)

### FUN_140373EA0 (verified)
- `game+0xC65C` ← EBX (new selected index) — `0x140374356`
- `game+0x10BE8` ← -1 (dirty counter reset) — `0x14037435c`
- `game+0x1039A` ← 0 (dirty flag, ONLY if selection changed) — `0x14037434f`
- `game+0xAE58` ← panel height — master dispatch `0x14031ee02`
- `FUN_14035F3E0` return `AL` → `[RSP+0x40]` flag — `0x140374612`-`0x14037461B`

### FUN_140361BB0 (verified)
- `game+0xC65C` ← ESI — `ch_tab23_full.asm:297` (`MOV [R14+0xc65c],ESI`)
- `[RBP-0x80]` ← FUN_14035F3E0 return-derived value — `ch_tab23_full.asm:659`
- Various stack locals zeroed after call (`ch_tab23_full.asm:662-699`, XMM8 stores)
- Selection 2: panel-list iteration block (`ch_tab23_full.asm:1593-1614+`)
- Selection 3/4/5/6: respective function calls with JMP out (no fallthrough to shared tail)

## 4. Candidate replacement boundaries

### Boundary A — FUN_140373EA0 at `0x140374606` (replace FUN_14035F3E0 call)
- **Preserves:** native row drawing, `game+0xC65C`/`game+0x10BE8`/`game+0x1039A` writes, hit-test + latch consumption, panel height computation
- **Replaces:** content sub-view only (when selection <= 1)
- **Skips:** `FUN_14035F3E0` return-value handling (`AL` → `[RSP+0x40]`), `game+0x10F20`/`game+0x5E8`/`0x5EC`/`0x5E6`/`0x5F0`/`game+0xADB8`/`game+0xADC8` writes, sub-page navigation `FUN_1403613B0`, `gui_click` sound
- **Valid for:** selections 0 and 1 only. Selection >1 already skips `FUN_14035F3E0` natively.

### Boundary B — FUN_140361BB0 at `0x1403628b8` (replace FUN_14035F3E0 call)
- **Preserves:** same as above for this handler
- **Valid for:** selections 0 and 1 only (guard at `0x1403628a3`-`0x1403628ab`)
- **Additional complexity:** selection 2 branches to panel-list iteration; selections 3/4/5/6 call different functions entirely and JMP out (never reach the content block). A single boundary here does NOT cover all selections.

## 5. What remains after suppressing content (input, cleanup, output)

| Concern | Status | Evidence |
|---------|--------|----------|
| Native input underneath | NOT accounted | Latch `DAT_0x140A54E91` consumed at `0x1403742F2` (FUN_140373EA0) but ownership/clear site not traced; already-selected click produces no unique state transition (section 51.1) |
| Click detection for already-selected button | NOT proven | `game+0x10BE8` set to -1 on every click but may already be -1; no transition observable; `game+0x1039A` only written if selection changed |
| Return value `AL` / `[RSP+0x40]` | Partially preserved | Must be replicated by replacement (FUN_140373EA0: `0x140374612`-`0x14037461B`; FUN_140361BB0: `0x1403628bd`-`0x1403628c6`) |
| Content state writes skipped | Unverified readers | `game+0x10F20`, `game+0x5E8/0x5EC/0x5E6/0x5F0`, `game+0xADB8/0xADC8` — no traced readers outside content block |
| Sub-page navigation (`FUN_1403613B0`) | Skipped | Disabled when `FUN_14035F3E0` replaced; game-state consistency unverified |
| Panel height `game+0xAE58` | Preserved | Written by master dispatch after handler returns (`0x14031ee02`), not inside content block |
| Cleanup / return-to-vanilla | NOT established | No explicit "restore native panel" instruction in any traced path; closing Almanac requires not-drawing replacement content, but native state may be inconsistent |

## 6. go / not yet assessment

| Aspect | Assessment | Reason |
|--------|-----------|--------|
| Native row drawing preserved | go | Button loop runs before content block; separable at instruction level |
| Selection state (`game+0xC65C`) preserved | go | Written in button loop, before content block |
| Click handling preserved | go | Native loop handles clicks + latch before content block |
| Boundary valid for selection >1 | go (FUN_140373EA0) / n/a (FUN_140361BB0) | >1 already skips FUN_14035F3E0 natively in FUN_140373EA0; FUN_140361BB0 has index-specific calls |
| Boundary valid for selections 0/1 | go (both handlers) | Both call FUN_14035F3E0 under `selection <= 1` guard |
| Return value `AL` replicated | not yet | Must be handled by replacement code; not verified safe |
| Content state corruption | not yet | Readers of `game+0x10F20` etc. not traced |
| Already-selected click detection | not yet | No unique state transition; latch ownership unclear |
| Cleanup / return-to-vanilla | not yet | No explicit restore instruction; state consistency unverified |
| Clip context / overlap | not yet | `game+0xADB0` dimensions not set in any traced function |

## 7. Conclusion: NOT YET

A replacement boundary is **not yet** safe to implement because:

1. **Input not accounted:** The already-selected-button click produces no observable state transition (latch consumption site not identified as clear point; `game+0x10BE8` may already be -1). Native content underneath could still receive clicks.
2. **Native content not suppressed at the right layer:** Boundary 2 only replaces `FUN_14035F3E0` (content below the row); the native row buttons still draw and remain clickable. If the Almanac overlay doesn't fully cover the row, native input bleeds through.
3. **Cleanup not established:** No return-to-vanilla path, no panel-pointer restore, no state-consistency guarantee for skipped content writes.
4. **FUN_140361BB0 has additional selection-specific calls** (indices 2–6) that a single boundary cannot cover uniformly.

## 8. Supporting instruction references

| Purpose | VA | File |
|---------|-----|------|
| FUN_140361BB0: selection<=1 guard | `1403628a3`-`1403628ab` | `export/ch_tab23_full.asm:650-651` |
| FUN_140361BB0: FUN_14035F3E0 call | `1403628b8` | `export/ch_tab23_full.asm:655` |
| FUN_140361BB0: return-value handling | `1403628bd`-`1403628c6` | `export/ch_tab23_full.asm:656-659` |
| FUN_140361BB0: selection==2 branch | `140363bba` | `export/ch_tab23_full.asm:1593` |
| FUN_140361BB0: selection==3/4 call `0x1404acf50` | `14036253d` | `export/ch_tab23_full.asm:480` |
| FUN_140361BB0: selection==5 call `0x1404b0e20` | `14036254f` | `export/ch_tab23_full.asm:485` |
| FUN_140361BB0: selection==6 call `0x1404b24a0` | `14036258f` | `export/ch_tab23_full.asm:498` |
| FUN_140361BB0: writes game+0xC65C | `140362193` | `export/ch_tab23_full.asm:297` |
| FUN_140373EA0: selection<=1 guard | `0x1403745f1`-`0x1403745f8` | `scratch/out/140373ea0_1403829fd_disasm.asm` |
| FUN_140373EA0: FUN_14035F3E0 call | `0x140374606` | `scratch/out/140373ea0_1403829fd_disasm.asm` |
| FUN_140373EA0: selection write `game+0xC65C` | `0x140374356` | `scratch/out/140373ea0_1403829fd_disasm.asm` |
| FUN_140373EA0: counter reset `game+0x10BE8` | `0x14037435c` | `scratch/out/140373ea0_1403829fd_disasm.asm` |

**Evidence:** `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0), `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), sections 39a–52. All static — nothing executed, nothing modified, no Ghidra analysis started while locked.
