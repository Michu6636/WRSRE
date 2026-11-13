# Native row-click hookpoint proof-of-concept test plan

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out\140373ea0_1403829fd_disasm.asm` (FUN_140373EA0), `C:\WRSRE\export\ch_tab23_full.asm` (FUN_140361BB0)
**Method:** Static test plan only. No game execution, no hook installation, no game modification, no deployment. This plan describes what a test would look like; it is not executed.

## 1. Purpose

Validate whether a minimal observer hook at either candidate click-path span can reliably detect native row icon clicks — including clicks on an already-selected icon — without breaking native click behavior. All eight native icons must remain visible and clickable. The Almanac button sits beside them, not covering them.

This test plan does **not** solve native-content replacement, Almanac placement, or return behavior. Those are separate unsolved problems.

## 2. Candidates

| Candidate | Span | Handler | Size | Button index register |
|-----------|------|---------|------|----------------------|
| A | `0x140374356–0x140374365` | FUN_140373EA0 | 16 bytes | EBX |
| B | `0x140362193–0x1403621A4` | FUN_140361BB0 | 18 bytes | ESI |

### Instruction bytes (for hook stub calculation)

**Candidate A:**
```
140374356: 89 9E 5C C6 00 00     MOV dword ptr [RSI+0xc65c],EBX    ; 6 bytes
14037435c: C7 86 E8 0B 01 00     MOV dword ptr [RSI+0x10be8],-1    ; 10 bytes
```
Total: 16 bytes. Both stores use RSI (game object pointer). EBX = clicked button's sub-entry index.

**Candidate B:**
```
140362193: 41 89 B6 5C C6 00 00  MOV dword ptr [R14+0xc65c],ESI    ; 7 bytes
14036219a: C7 86 E8 0B 01 00     MOV dword ptr [R14+0x10be8],-1    ; 11 bytes
```
Total: 18 bytes. R14 = game object pointer. ESI = clicked button's sub-entry index.

### Verified incoming branches

**Candidate A (`0x140374356`):**
- `0x14037434d` — `JZ 0x140374356` (already selected: selection == EBX)
- `0x14037434f` — fall-through (selection changed: dirty flag written first)
- `0x140374356` — sequential to `0x14037435C`

**Candidate B (`0x140362193`):**
- `0x14036214a` — `JNZ 0x140362193` (not live mode)
- `0x140362165` — `JZ 0x140362193` (sound call returned AL=0)
- `0x140362167` — fall-through (sound succeeded)
- `0x140362193` — sequential to `0x14036219A`

**Not verified:** Exhaustive xrefs to confirm no other code jumps into span interiors. Not exhaustive.

## 3. Minimal observer — what it preserves vs records

### Preserves (must not modify)

| Item | How preserved |
|------|---------------|
| Original stores | Both MOV instructions in span execute unmodified |
| Registers | All non-volatile (RBX/RSI/RBP/R12-R15 per candidate) restored; volatile (RAX/RCX/RDX/R8-R11) per x64 ABI |
| Flags (EFLAGS) | MOV doesn't affect flags; hook code must not leave flags clobbered unexpectedly |
| Stack | 32-byte shadow space + 16-byte alignment before any hook-side CALL |
| Latch `0xA54E91` | Read-only observation; hook does NOT consume it |
| `game+0xC65C` | Native write executes unmodified |
| `game+0x10BE8` | Native write executes unmodified |
| `game+0xC650` | Not touched by hook |
| Native sound calls | Execute unmodified (they precede the hook span) |

### Records (mod-owned, read-only with respect to game state)

| Observation | Where captured | Trigger condition |
|-------------|---------------|-------------------|
| Click occurred | Hook entry at span start | Every time hook span executes |
| Button index | EBX (Candidate A) / ESI (Candidate B) read before original stores | Per-click |
| Already-selected vs other-icon | Compare EBX/ESI against previously recorded index | Per-click |
| Click count | Mod-owned counter incremented per hook entry | Per-click |
| Timestamp/frame | Mod-owned frame counter | Per-click |

### What the observer does NOT do

- Does not write `game+0xC65C`, `game+0xC650`, `game+0x10BE8`, `game+0x1039A`
- Does not consume latch `0xA54E91`
- Does not write to any game memory
- Does not close/open any panel (mod-owned flag only; actual close decision is separate)

## 4. Test matrix — repeated clicks and other icons

All clicks performed manually by a human tester (no simulated input). Each click follows a full frame cycle.

| Test | Click # | Icon clicked | Expected observer result | Expected native result |
|------|---------|-------------|-------------------------|------------------------|
| T1 | 1 | Icon 0 (first) | Observer flag set, EBX/ESI = 0 | `game+0xC65C` = 0, `game+0x10BE8` = -1 |
| T2 | 2 | Icon 0 (already selected) | Observer flag set again, EBX/ESI = 0 | `game+0xC65C` = 0 (unchanged), `game+0x10BE8` = -1 |
| T3 | 3 | Icon 1 | Observer flag set, EBX/ESI = 1 | `game+0xC65C` = 1, `game+0x10BE8` = -1 |
| T4 | 4 | Icon 1 (already selected) | Observer flag set again, EBX/ESI = 1 | `game+0xC65C` = 1 (unchanged), `game+0x10BE8` = -1 |
| T5 | 5 | Icon 7 (last) | Observer flag set, EBX/ESI = 7 | `game+0xC65C` = 7, `game+0x10BE8` = -1 |
| T6 | 6 | Icon 0 then Icon 3 (rapid) | Observer flag set twice, EBX/ESI = 0 then 3 | `game+0xC65C` = 3, `game+0x10BE8` = -1 |
| T7 | 7 | No click (hover only) | Observer flag NOT set | `game+0xC65C` unchanged, `game+0x10BE8` unchanged |
| T8 | 8 | Click outside row | Observer flag NOT set | `game+0xC65C` unchanged |

### Verification method (read-only where possible)

- **Observer flag:** Read mod-owned DLL memory (read-only from game's perspective — the mod writes its own memory, not game memory)
- **Native `game+0xC65C`:** Read via read-only process memory (PROCESS_VM_READ) — does not modify game
- **Native `game+0x10BE8`:** Read via read-only process memory
- **Native icon visibility/clickability:** Observed visually by tester; no memory modification

### Read-only vs game-modification actions

| Action | Read-only? | Modifies game? | Deploys hook? |
|--------|-----------|---------------|---------------|
| Read `game+0xC65C` via PROCESS_VM_READ | Yes | No | No |
| Read `game+0x10BE8` via PROCESS_VM_READ | Yes | No | No |
| Read mod-owned DLL memory | Yes | No | No |
| Build DLL with observer hook | Yes (build artifact) | No | No |
| Install hook DLL into game folder | No | Yes | Yes |
| Launch game with hooked DLL | No | Yes | Yes |
| Click native icons manually | Yes (user input) | No | No |

**Note:** Build-only (no deployment) is read-only with respect to the running game. Installing and launching with the hook requires game modification/deployment and is **not** part of this static plan.

## 5. Build verification

Before any deployment, the observer DLL must pass:

| Check | Method | Pass criterion |
|-------|--------|---------------|
| Hook span size matches | Disassemble built DLL | Exactly 16 bytes (A) or 18 bytes (B) at hook entry |
| Original stores preserved | Disassemble | Both MOV instructions present and unmodified |
| Register preservation | Disassemble hook prologue/epilogue | All non-volatile regs (RBX/RSI/RBP/R12-R15 per candidate) saved/restored |
| Flag preservation | Disassemble | No FLAGS-modifying ops left unbalanced (PUSHF/POPF or save/restore) |
| Stack alignment | Disassemble before any hook-side CALL | 16-byte aligned + 32-byte shadow space |
| Jump target | Disassemble hook epilogue | Jumps to `0x140374366` (A) or `0x1403621A5` (B) |
| No latch consumption | Disassemble | No write to `0xA54E91` |
| No game memory writes | Disassemble | No writes to `game+0xC65C`, `game+0xC650`, `game+0x10BE8`, `game+0x1039A` |

## 6. Failure/rollback steps

### Failure categories

| Failure | Detection | Rollback |
|---------|-----------|----------|
| Hook span wrong size | Build-time disassembly mismatch | Rebuild with corrected span; do not deploy |
| Original store missing | Build-time disassembly | Rebuild; do not deploy |
| Register not saved/restored | Build-time disassembly | Fix prologue/epilogue; rebuild |
| Jump target wrong | Build-time disassembly | Fix epilogue; rebuild |
| Game crash on launch | Tester observes no window / crash | Uninstall hook DLL; revert to vanilla game |
| Native click broken (icon not selected) | Tester observes click does nothing | Uninstall hook DLL; revert |
| Latch consumed (double-click not detected) | Tester observes already-selected click not registered | Uninstall hook DLL; revert |
| Observer flag not set on click | Mod-owned memory read shows no change | Uninstall hook DLL; rebuild observer logic |

### Rollback procedure

1. Close game if running
2. Remove observer DLL from game folder (restore original file set)
3. Verify game launches and runs vanilla
4. Rebuild observer with fixes
5. Repeat build verification before redeploy

## 7. Pass/fail criteria

### Pass (all required)

| # | Criterion | How verified |
|---|-----------|-------------|
| P1 | Observer flag set on every native icon click (including already-selected) | Mod-owned memory read after each click |
| P2 | Observer flag NOT set on hover-only or off-row click | Mod-owned memory read unchanged |
| P3 | `game+0xC65C` equals clicked icon index after every click | PROCESS_VM_READ after each click |
| P4 | `game+0x10BE8` = -1 after every click | PROCESS_VM_READ after each click |
| P5 | All eight native icons remain visible and clickable | Visual observation by tester |
| P6 | Game launches and runs without crash with hook installed | Tester observation |
| P7 | Hook span matches expected 16-byte (A) or 18-byte (B) sequence | Build-time disassembly |
| P8 | No game memory modified by hook (except mod-owned DLL memory) | Disassembly + memory diff |

### Fail (any one)

| # | Criterion |
|---|-----------|
| F1 | Observer flag not set on any native icon click |
| F2 | Observer flag set on hover-only or off-row click |
| F3 | `game+0xC65C` incorrect after any click |
| `F4` | `game+0x10BE8` not -1 after any click |
| F5 | Any native icon invisible or unclickable |
| F6 | Game crashes or fails to launch with hook |
| F7 | Hook span size or content mismatch |
| F8 | Any game memory (other than mod-owned DLL) modified by hook |

**Pass requires ALL of P1–P8. Any single F1–F8 is a failure.**

## 8. What this test does NOT solve

This test plan is narrowly scoped to one question: **can a minimal observer detect native row icon clicks — including already-selected — without breaking native click behavior?**

It explicitly does **not** address:

- **Native-content replacement:** How the Almanac replaces native sidebar content below the row (see `sidebar-content-replace-20261110/` — Boundary 2 is the leading candidate but unresolved)
- **Almanac placement:** Where the Almanac button sits relative to the eight native icons (Approach B — separate additive button — is the leading candidate but placement math unverified)
- **Return behavior:** How the Almanac closes and native content returns (see `sidebar-return-20261109/` — observed but mechanism not implemented)
- **Clip context:** `game+0xADB0` dimensions not set in any traced function (see `trade-handler-trace/` and `sidebar-write-trace-20261109/`)
- **Handler selector:** Which trade handler runs at runtime (`game+0x10398` gate unverified at runtime)

## 9. Decision gate

If the test passes (all P1–P8), the observer hook is a **viable candidate** for Almanac close-on-native-click. It does **not** prove the Almanac design is complete — it only proves one interaction primitive works.

If the test fails (any F1–F8), the observer hook approach is rejected for both candidates, and an alternative detection mechanism must be found (e.g., mouse-position recording, latch observation at a different site).

## 10. Evidence

Folder: `C:\WRSRE\export\almanac-evidence\native-click-hookpoints-20261110/`
- `report.md` — hookpoint assessment (14-byte minimum, both candidates viable but unproven)
- `report.md` (this file) — test plan

All sources: existing exports only. No Ghidra started while locked. No game execution. No hooks installed. No game modified.
