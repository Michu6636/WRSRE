# Native row-click hookpoint assessment — bounded static analysis

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0)
**Method:** Static only. No Ghidra started while locked (stale `.lock` left untouched; no ghidra process running). No game execution, no hooks, no writes.

**Correction:** The loader requires **at least 14 bytes**, not exactly 14. Both candidate spans meet this minimum:
- `0x140374356–0x140374365` (16 bytes) in FUN_140373EA0
- `0x140362193–0x1403621A4` (18 bytes) in FUN_140361BB0

These meet the size requirement, but that does **not** prove hook safety. This report assesses each candidate fully.

## 1. Candidate A — FUN_140373EA0 span `0x140374356–0x140374365` (16 bytes)

### Instructions in span

| VA | Instruction | Bytes | Purpose |
|----|------------|-------|---------|
| `0x140374356` | `MOV dword ptr [RSI+0xc65c],EBX` | 6 | game+0xC65C = this button's index (EBX) |
| `0x14037435C` | `MOV dword ptr [R14+0x10be8],0xffffffff` | 10 | game+0x10BE8 = -1 (always, on every click) |

### Incoming branches

| Source VA | Instruction | Target | Condition |
|-----------|------------|--------|-----------|
| `0x14037434d` | `JZ 0x140374356` | 0x140374356 | Already selected (selection index == EBX) |
| `0x14037434f` | fall-through | 0x140374356 | Selection changed (dirty flag written first) |
| `0x140374356` | fall-through | 0x14037435C | Sequential execution |

Both paths converge at `0x140374356`: already-selected jumps directly here; changed-selection falls through from the dirty-flag write. `0x14037435C` is reached only sequentially from `0x140374356`. No other incoming branches identified in cached disassembly — but this is a partial view; there may be jump-table or cross-function edges not captured.

### Registers and flags at hook point

| Register | Value | Volatile? | Notes |
|----------|-------|-----------|-------|
| RSI/RSI | game object pointer | Non-volatile | Base for selection write |
| R14/R14 | game object pointer | Non-volatile | Base for counter write |
| EBX | clicked button index | Non-volatile (RBX low 32) | Read-only at hook point |
| Flags | ZF set by preceding CMP | N/A | MOV does not affect flags |

**MOV instructions do not affect EFLAGS.** At `0x140374356`, flags reflect the preceding `CMP [RSI+0xc65c],EBX`. At `0x14037435C`, flags reflect the selection write (MOV doesn't change them). A hook must preserve all registers and flags it touches.

### Stack/ABI (Windows x64)

- Non-volatile: RBX, RBP, RDI, RSI, R12-R15 (hook must restore if modified)
- Volatile: RAX, RCX, RDX, R8-R11 (caller-saved)
- Shadow space: 32 bytes required before any CALL
- Stack must be 16-byte aligned before CALL

EBX (RBX low 32) is non-volatile — the hook must preserve RBX if it modifies it. ESI/RSI (non-volatile) holds game object pointer — must preserve. Since MOV doesn't modify flags, flags are preserved as long as hook code saves/restores them.

### Original stores (must still execute)

1. `game+0xC65C = EBX` — selects this button's index (no-op if already selected)
2. `game+0x10BE8 = -1` — resets dirty counter, always on click

**A hook must execute both original stores.** Skipping either would change native behavior.

### Notification opportunity

At `0x14037435C`, EBX holds the clicked button index and `game+0xC65C` has just been written with it. A hook could:
- Read EBX to learn which native icon was clicked
- Set a mod-owned "Almanac should close" flag in DLL memory
- Execute both original stores
- Restore registers/flags and jump to `0x140374366` (overlay CMP)

This fires on **every** click including already-selected icons — exactly the coverage needed.

### Side effects of notifying

- Mod sets its own flag in DLL memory — no impact on native state
- Native click processing is already complete (selection write done, counter write about to execute)
- Latch `0xA54E91` is not consumed (hook only reads the fact that we reached the counter write)
- `game+0xC65C` write still executes (native selection tracking intact)

### Risks / unknowns

| Risk | Status |
|------|--------|
| Incoming branches other than JZ/fall-through not identified | Not traced — need full xrefs to rule out jump-table/other edges |
| Register preservation in hook code | Hook must save/restore RBX, RSI, R14, flags |
| Stack alignment for any CALL in hook | Hook must maintain 16-byte alignment + 32-byte shadow space |
| EBX may not equal the visual icon index (reordering) | FUN_140373EA0 maps screen pos → vector index non-linearly; EBX is the vector index, not screen position |

## 2. Candidate B — FUN_140361BB0 span `0x140362193–0x1403621A4` (18 bytes)

### Instructions in span

| VA | Instruction | Bytes | Purpose |
|----|------------|-------|---------|
| `0x140362193` | `MOV dword ptr [R14+0xc65c],ESI` | 7 | game+0xC65C = this button's index (ESI) |
| `0x14036219A` | `MOV dword ptr [R14+0x10be8],0xffffffff` | 11 | game+0x10BE8 = -1 (always, on every click) |

### Incoming branches

| Source VA | Instruction | Target | Condition |
|-----------|------------|--------|-----------|
| `0x14036214a` | `JNZ 0x140362193` | 0x140362193 | Not live mode |
| `0x140362165` | `JZ 0x140362193` | 0x140362193 | Sound call returned 0 (AL=0) |
| `0x140362167` | fall-through | 0x140362193 | Sound succeeded → sequential |
| `0x140362193` | fall-through | 0x14036219A | Sequential execution |

Multiple paths converge at `0x140362193`: not-live-mode jumps directly here; sound-failed jumps here; sound-succeeded falls through here. `0x14036219A` reached only sequentially. Same caveat about possible other incoming branches.

### Registers and flags at hook point

| Register | Value | Volatile? | Notes |
|----------|-------|-----------|-------|
| R14 | game object pointer | Non-volatile | Base for both writes |
| ESI | clicked button index | Non-volatile (RSI low 32) | Read-only at hook point |
| Flags | MOV doesn't affect flags | N/A | Same as Candidate A |

### Original stores (must still execute)

1. `game+0xC65C = ESI` — selects this button's index
2. `game+0x10BE8 = -1` — resets dirty counter

### Notification opportunity

Same as Candidate A: read ESI for button index, set mod-owned flag, execute both stores, jump to `0x1403621A5` (overlay CMP). Fires on every click including already-selected.

### Risks / unknowns

Same categories as Candidate A, plus:
- ESI (part of RSI) is non-volatile — hook must preserve RSI
- FUN_140361BB0 has additional selection-specific calls for indices 2-6 (different code paths) — hook at counter write fires for ALL selections uniformly

## 3. Candidate comparison

| Aspect | Candidate A (FUN_140373EA0) | Candidate B (FUN_140361BB0) |
|--------|----------------------------|----------------------------|
| Span | `0x140374356–0x140374365` (16 bytes) | `0x140362193–0x1403621A4` (18 bytes) |
| Meets 14-byte minimum | Yes | Yes |
| Whole-instruction boundary | Yes (start of MOV, end of MOV) | Yes (start of MOV, end of MOV) |
| Button index register | EBX (non-volatile RBX) | ESI (non-volatile RSI) |
| Already-selected click fires? | Yes | Yes |
| Latch consumed? | No (read-only observation) | No (read-only observation) |
| Native state modified by hook? | No (if only writes mod-owned memory) | No (if only writes mod-owned memory) |
| Incoming branches fully mapped? | No (partial view) | No (partial view) |
| RIP-relative operands in stolen span? | No | No |
| Flag-modifying instructions in span? | No | No |

## 4. What a hook at either candidate must preserve

| Item | Requirement |
|------|-------------|
| Original stores | Both `MOV [game+0xc65c], index` and `MOV [game+0x10be8], -1` must execute |
| Registers | RBX (Candidate A) / RSI (Candidate B) must be preserved; all non-volatile regs preserved |
| Flags | MOV doesn't affect flags, but hook code must not clobber them unexpectedly |
| Stack | 32-byte shadow space + 16-byte alignment if hook calls anything |
| Return target | Jump to `0x140374366` (A) or `0x1403621A5` (B) — the overlay CMP after the counter write |
| Latch `0xA54E91` | Not consumed — natives check it for their own processing |
| `game+0xC65C` | Not overwritten by hook — native selection tracking intact |
| `game+0x10BE8` | Not overwritten by hook — native dirty counter intact |

## 5. Candidate/blocker table

| Candidate | Size (bytes) | Fires on already-selected click? | Button index available? | Latch consumed? | Native state modified? | 14-byte boundary? | Verdict |
|-----------|-------------|----------------------------------|------------------------|-----------------|---------------------|-------------------|---------|
| A: `0x140374356–0x140374365` | 16 | Yes | EBX | No | No (if hook only writes mod memory) | Yes | Possible candidate — needs incoming-branch verification |
| B: `0x140362193–0x1403621A4` | 18 | Yes | ESI | No | No (if hook only writes mod memory) | Yes | Possible candidate — needs incoming-branch verification |
| Mouse position recording | N/A | Yes | No | No | No | N/A | Possible — needs hook before/after native handler |
| Latch `0xA54E91` read | N/A | Yes (racy) | No | No | No | N/A | Racy — cleared elsewhere |

**No candidate is proven safe.** Both A and B meet the size requirement and fire on already-selected clicks, but neither has verified incoming-branch coverage. Treating instruction alignment alone as hook safety would be incorrect.

## 6. What needs an authorized integration test

1. **Full xrefs to `0x140374356` / `0x140362193`** — confirm no other code jumps into the middle of the span (e.g., jump tables, exception handlers)
2. **Full xrefs to `0x14037435C` / `0x14036219A`** — confirm only sequential entry
3. **Hook proof-of-concept** — verify register/flag preservation, stack alignment, and that original stores execute correctly
4. **Already-selected click notification** — verify the mod's flag is set when clicking an already-selected icon
5. **Native click integrity** — verify native icon clicks still work after hook installation (selection writes, counter reset, sound, overlay)
6. **Almanac close timing** — verify the mod's flag is set at the right moment (after native processing, before overlay draw)

## 7. Evidence files preserved

Folder: `C:\WRSRE\export\almanac-evidence\native-click-hookpoints-20261110/`
- `report.md` — this report

**No new disassembly files produced.** All evidence from existing sources:
- `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0: `:217-256`)
- `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0: `:245-298`)
- `C:\WRSRE\plugins\ui_panels\ui_panels.cpp` (WidgetButtonImplEx latch logic)
- sections 39a–52, 51

**Bottom line:** Both candidate spans meet the 14-byte minimum size requirement and fire on every click including already-selected icons. However, neither is proven safe: incoming branches are not fully mapped, register/flag preservation in hook code is unverified, and stack alignment for any hook-side CALL is untested. Instruction alignment alone is not hook safety. The assessment remains **NOT YET** — but both candidates are now viable for an authorized hook-point proof-of-concept, whereas they were previously rejected on size grounds.
