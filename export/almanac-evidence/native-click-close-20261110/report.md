# Native row-click observation — bounded static assessment

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0), `C:\WRSRE\plugins\ui_panels\ui_panels.cpp` (WidgetButtonImplEx), sections 39a–52, 51
**Method:** Static only. No Ghidra started while locked (stale `.lock` left untouched; no ghidra process running). No game execution, no hooks, no writes.

## 1. Requirement

The Almanac button sits beside the eight native Economy & Trade icons. All eight native icons must remain visible and clickable. The Almanac panel must close when the player clicks **any** native row icon — including one **already selected** — without consuming the native click latch (`0xA54E91`) or changing native click behavior.

## 2. Verified click paths — instruction-level trace

### 2a. FUN_140373EA0 click path (verified)

| VA | Instruction | Bytes | Purpose |
|----|------------|-------|---------|
| `0x1403742a4` | `CALL qword ptr [0x14086c850]` | 6 | Collision(mouse, rect) — AL = hit |
| `0x1403742aa` | `TEST AL,AL` | 2 | Check hit |
| `0x1403742ac` | `JZ 0x140374366` | 2 | No hit → skip click handling (overlay) |
| `0x1403742b2` | `MOV EDX,[R13+0x40]` | 4 | Get text ID |
| ... | ... | ... | Copy string |
| `0x1403742f2` | `CMP byte ptr [0x140a54e91],CL` | 6 | Latch check |
| `0x1403742f8` | `JZ 0x140374366` | 2 | No latch → skip click handling |
| `0x1403742fa` | `CMP byte ptr [RSI+0x14],CL` | 4 | Live mode check |
| `0x1403742fd` | `JNZ 0x140374347` | 2 | Not live → skip sound, go to selection write |
| `0x14037430d` | `CALL qword ptr [0x14086cd10]` | 6 | Sound call (live mode only) |
| `0x140374313` | `TEST AL,AL` | 2 | Sound result |
| `0x140374315` | `JZ 0x140374347` | 2 | Sound failed → skip to selection write |
| `0x140374317` | `CALL qword ptr [0x14086cd18]` | 6 | Second sound |
| `0x140374347` | `CMP dword ptr [RSI+0xc65c],EBX` | 6 | Compare selection with this button's index |
| `0x14037434d` | `JZ 0x140374356` | 2 | Already selected → jump to store |
| `0x14037434f` | `MOV byte ptr [RSI+0x1039a],0x0` | 7 | Dirty flag clear — ONLY if selection changed |
| `0x140374356` | `MOV dword ptr [RSI+0xc65c],EBX` | 6 | **game+0xC65C = this button's index** |
| `0x14037435c` | `MOV dword ptr [R14+0x10be8],0xffffffff` | 12 | **game+0x10BE8 = -1 — ALWAYS on click** |

**Registers at counter write (`0x14037435c`):** R14 = game object, RSI = game object, EBX = this button's sub-entry index.

**Already-selected click at this path:** `game+0xC65C` unchanged (no-op write), `game+0x1039A` NOT written, `game+0x10BE8 = -1` (may already be -1). Sound plays but is not state-observable.

**Input gates (all must pass):** Collision hit (AL=1), latch `0xA54E91` set, live mode (or skip sound). All three must be true for click handling to reach the counter write.

### 2b. FUN_140361BB0 click path (verified)

| VA | Instruction | Bytes | Purpose |
|----|------------|-------|---------|
| `0x1403620c1` | `CALL qword ptr [0x14086c850]` | 6 | Collision(mouse, rect) |
| `0x1403620c7` | `TEST AL,AL` | 2 | Check hit |
| `0x1403620c9` | `JZ 0x1403621a5` | 2 | No hit → skip click handling |
| `0x1403620cf` | `MOV EDX,[R15+RAX*1+0x40]` | 5 | Get text ID |
| ... | ... | ... | Copy string |
| `0x14036213b` | `CALL qword ptr [0x14086c8b8]` | 6 | Sound/latch-related call |
| `0x140362141` | `TEST AL,AL` | 2 | Check result |
| `0x140362143` | `JZ 0x1403621a5` | 2 | No result → skip click handling |
| `0x140362145` | `CMP byte ptr [R14+0x14],0x0` | 4 | Live mode check |
| `0x14036214a` | `JNZ 0x140362193` | 2 | Not live → skip sound, go to selection write |
| `0x14036214c` | `LEA RBX,[R14+0xfd10]` | 7 | Sound setup |
| `0x140362153` | `LEA RDX,[0x1408b87b8]` | 7 | Sound setup |
| `0x14036215a` | `MOV RCX,RBX` | 3 | Sound setup |
| `0x14036215d` | `CALL qword ptr [0x14086cd10]` | 6 | Sound call |
| `0x140362163` | `TEST AL,AL` | 2 | Sound result |
| `0x140362165` | `JZ 0x1403621a5` | 2 | Sound failed → skip to selection write |
| `0x140362167` | `CALL qword ptr [0x14086d2a8]` | 6 | Second sound |
| `0x140362193` | `MOV dword ptr [R14+0xc65c],ESI` | 7 | **game+0xC65C = this button's index** |
| `0x14036219a` | `MOV dword ptr [R14+0x10be8],0xffffffff` | 13 | **game+0x10BE8 = -1 — ALWAYS on click** |

**Registers at counter write (`0x14036219a`):** R14 = game object, ESI = this button's sub-entry index.

**Already-selected click:** Same as FUN_140373EA0 — `game+0xC65C` no-op write, `game+0x10BE8 = -1` (may already be -1).

## 3. Displaced-instruction analysis (14-byte inline patch constraint)

The project's hook requires a contiguous span of **14 whole instructions** (verified in section 51.2). Neither click-write instruction sits at a 14-byte whole-instruction boundary.

### FUN_140373EA0 candidate sites

| VA | Instruction | Size | 14-byte boundary? | Issue |
|----|------------|------|-------------------|-------|
| `0x140374347` | `CMP [RSI+0xc65c],EBX` | 6 | No | 6+2+7=15 bytes to next whole-instr boundary; cuts into MOV dirty flag |
| `0x14037434d` | `JZ 0x140374356` | 2 | No | 2+7+6=15 bytes; cuts into MOV selection |
| `0x14037434f` | `MOV [RSI+0x1039a],0x0` | 7 | No | 7+6=13 bytes; need 1 more, cuts into counter write |
| `0x140374356` | `MOV [RSI+0xc65c],EBX` | 6 | No | 6+7=13 bytes; need 1 more, cuts into counter write |
| `0x14037435c` | `MOV [R14+0x10be8],-1` | 12 | No | 12 bytes; need 2 more, cuts into next CMP instruction |

**Result:** No 14-byte whole-instruction boundary at any click-write site in FUN_140373EA0.

### FUN_140361BB0 candidate sites

| VA | Instruction | Size | 14-byte boundary? | Issue |
|----|------------|------|-------------------|-------|
| `0x140362193` | `MOV [R14+0xc65c],ESI` | 7 | No | 7+13=20 bytes; 14 cuts into counter write |
| `0x14036219a` | `MOV [R14+0x10be8],-1` | 13 | No | 13 bytes; need 1 more, cuts into next CMP |

**Result:** No 14-byte whole-instruction boundary at any click-write site in FUN_140361BB0 either.

### RIP-relative check
Neither counter write uses RIP-relative addressing (both use R14-relative: `C7 86 E8 0B 01 00 FF FF FF FF` and `41 C7 86 E8 0B 01 00 FF FF FF FF`). So relocation is not an issue — but instruction-boundary alignment is.

## 4. Candidate observation points — comparison

| Candidate | VA | Fired on already-selected click? | Whole-instr 14-byte boundary? | Reads button index? | Touches game state? |
|-----------|-----|----------------------------------|-------------------------------|---------------------|---------------------|
| Counter write (FUN_140373EA0) | `0x14037435c` | Yes | **No** (12 bytes) | Yes (EBX) | Yes (writes -1 — original must execute) |
| Counter write (FUN_140361BB0) | `0x14036219a` | Yes | **No** (13 bytes) | Yes (ESI) | Yes (writes -1 — original must execute) |
| Selection write (FUN_140373EA0) | `0x140374356` | Yes | **No** (6 bytes) | Yes (EBX) | Yes (writes index — may be no-op) |
| Selection write (FUN_140361BB0) | `0x140362193` | Yes | **No** (7 bytes) | Yes (ESI) | Yes (writes index — may be no-op) |
| Sound call (both) | `0x140374325` / `0x14036218d` | Yes (live mode only) | **No** (6 bytes) | No | No (but indirect CALL via IAT) |
| Dirty flag write (FUN_140373EA0) | `0x14037434f` | **No** (only on change) | **No** (7 bytes) | No | Yes (writes 0) |

**Key finding:** No candidate observation point satisfies all four requirements: (a) fired on already-selected click, (b) 14-byte whole-instruction boundary, (c) reads button index, (d) doesn't change native behavior.

## 5. Candidate/blocker table

| Candidate | Blocker | Resolvable statically? |
|-----------|---------|----------------------|
| Hook at counter write `0x14037435c` / `0x14036219a` | Not a 14-byte whole-instruction boundary (12-13 bytes; including next instruction cuts it) | No — instruction boundary mismatch is fundamental |
| Hook at selection write `0x140374356` / `0x140362193` | Too small alone (6-7 bytes); combining with counter write exceeds 14 bytes | No — size mismatch |
| Hook at sound call `0x140374325` / `0x14036218d` | Only fires in live mode; indirect CALL via IAT (6 bytes too small); not every click produces sound | Partial — live mode gap |
| Read `game+0x10BE8` after native handler | May already be -1 on already-selected click; no transition observable | No — no transition on already-selected click |
| Read `game+0xC65C` after native handler | Unchanged on already-selected click | No — no transition |
| Mouse position recording (GetCursorPos + ScreenToClient) | Requires hook before/after native handler — no safe hook point identified; frame-timing race | No — no hook point |
| Latch `0xA54E91` read | Racy (cleared elsewhere, not by button loop); consuming it breaks native clicks | No — consumption breaks natives; reading is racy |

## 6. What each candidate would need to preserve

If a hook were placed at the counter write (the best candidate despite the boundary issue), the mod would need to:

1. **Preserve all registers:** R14/RSI (game object), EBX/ESI (button index), XMM registers (used by subsequent overlay code), RSP/RBP (stack frame)
2. **Execute the original instruction:** `MOV [R14/RSI+0x10be8], -1` must still write -1
3. **Jump back to the instruction after:** `0x140374366` / `0x1403621a5` (overlay CMP)
4. **Read button index before original executes:** EBX/ESI identifies which native icon was clicked
5. **Record click in mod-owned state:** Set mod-owned "Almanac should close" flag
6. **Not consume latch `0xA54E91`:** Natives need it for their own click handling

## 7. Shared-state risks

| Shared state | Native use | Risk if mod touches |
|-------------|-----------|-------------------|
| `game+0xC65C` (selected index) | Written by native click handler | Writing synthetic index selects wrong icon |
| `game+0xC650` (panel pointer) | Written by `FUN_14007C400` on section switch | Arbitrary panel object — unsafe |
| `game+0x10BE8` (counter) | Written -1 on every click | Writing could mask native click detection |
| `0xA54E91` (click latch) | Checked at click handling; cleared elsewhere | Consuming breaks native icon clicks |
| `game+0x1039A` (dirty flag) | Cleared only if selection changed | Writing could affect native state tracking |

## 8. Conclusion: NOT YET

A native-click observation point that satisfies all constraints (fired on already-selected click, 14-byte whole-instruction boundary, reads button index, doesn't change native behavior) does **not exist** at any statically identified location in either handler.

**Blockers:**
1. **No 14-byte whole-instruction boundary** at any click-write instruction in either handler (counter writes are 12-13 bytes, selection writes are 6-7 bytes, combining exceeds 14)
2. **Already-selected click:** No state-field transition observable (`game+0xC65C` unchanged, `game+0x10BE8` may already be -1, `game+0x1039A` not written)
3. **Latch consumption breaks natives:** `0xA54E91` is required by native click handling at `0x1403742f2` / `ch_tab23_full.asm:575`
4. **No clean observation point** that fires on every click without touching native state

**What would change the assessment:**
- A 14-byte whole-instruction boundary found within the click path of either handler (before or at the counter write)
- Identification of where latch `0xA54E91` is cleared (could observe clear instead of set)
- Confirmation that `game+0x10BE8` writers are only the click handlers (would make read-only observation viable)
- An authorized integration test measuring latch state and `game+0x10BE8` across already-selected clicks

## 9. Supporting instruction references

| Purpose | VA | File |
|---------|-----|------|
| FUN_140373EA0: Collision call | `0x1403742a4` | `140373ea0_1403829fd_disasm.asm:217` |
| FUN_140373EA0: Latch check | `0x1403742f2` | `140373ea0_1403829fd_disasm.asm:233` |
| FUN_140373EA0: Live mode check | `0x1403742fa` | `140373ea0_1403829fd_disasm.asm:236` |
| FUN_140373EA0: Sound call | `0x14037430d` | `140373ea0_1403829fd_disasm.asm:239` |
| FUN_140373EA0: Selection CMP | `0x140374347` | `140373ea0_1403829fd_disasm.asm:251` |
| FUN_140373EA0: Selection write | `0x140374356` | `140373ea0_1403829fd_disasm.asm:255` |
| FUN_140373EA0: Counter write | `0x14037435c` | `140373ea0_1403829fd_disasm.asm:256` |
| FUN_140361BB0: Collision call | `0x1403620c1` | `export/ch_tab23_full.asm:252` |
| FUN_140361BB0: Latch/sound call | `0x14036213b` | `export/ch_tab23_full.asm:278` |
| FUN_140361BB0: Live mode check | `0x140362145` | `export/ch_tab23_full.asm:280` |
| FUN_140361BB0: Sound call | `0x14036215d` | `export/ch_tab23_full.asm:286` |
| FUN_140361BB0: Selection write | `0x140362193` | `export/ch_tab23_full.asm:297` |
| FUN_140361BB0: Counter write | `0x14036219a` | `export/ch_tab23_full.asm:298` |
| WidgetButtonImplEx latch logic | `ui_panels.cpp` | `s_wasLButtonDown`, `s_clickConsumed`, `0xA54E91`, `GetAsyncKeyState(VK_LBUTTON)` |

**Evidence:** `C:\WRSRE\export\almanac-evidence\native-click-close-20261110/report.md` — this report updates prior findings. All static — nothing executed, no Ghidra started while locked, no hooks, no writes.
