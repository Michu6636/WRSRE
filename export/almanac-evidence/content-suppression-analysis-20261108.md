# Almanac: content-suppression analysis after 0x140373ea0 normal row

Static analysis only. No debugger, hooks, implementation, or game changes.
Preferred-image VAs (image base 0x140000000). Source exports:
`export/140373ea0_1403829fd_disasm.asm` (handler) and
`export/140317db0_full_disasm.asm` (host). Branch not switched; working tree
untouched. No commit or push performed.

## 1. Outputs that must be preserved if an Almanac page replaces vanilla content after 0x1403745b6

### 1.1 Handler output writes (normal path)

After the row loop and content drawing, the handler writes two outputs at
0x140374c3d–0x140374c54 before jumping to cleanup:

| Address | Instruction | Meaning |
|---|---|---|
| 0x140374c42 | MOV RAX, [RBP+0x30] | RAX = saved RDX = output-X pointer (arg 2) |
| 0x140374c46 | MOVSS [RAX], XMM0 | *output_X = [RBP-0x4c] (final X) |
| 0x140374c50 | MOV RAX, [RBP+0x28] | RAX = saved R8 = output-Y pointer (arg 3) |
| 0x140374c54 | MOVSS [RAX], XMM1 | *output_Y = [RSP+0x70] (final Y) |
| 0x140374c58 | JMP 0x1403826ca | fall into normal cleanup |

At function entry:
- 0x140373f21: MOV [RBP+0x28], R8 (save output-Y ptr)
- 0x140373f28: MOV [RBP+0x30], RDX (save output-X ptr)
- RSI = RCX = game pointer; RDI = R8 = output-Y; RBX = RDX = output-X

[XMM0 = [RBP-0x4c] and XMM1 = [RSP+0x70] are set within the content-drawing
branches at 0x140374aa5/0x140374aba (sub-tab 0–4), 0x140374b88/0x140374b9d
(sub-tab 5), or 0x140374c5d/default path.]

### 1.2 Host consumption of outputs (0x14031edf4–0x14031ee02)

| Address | Instruction | Meaning |
|---|---|---|
| 0x14031edf4 | MOVSS XMM0, [RBP-0x10] | XMM0 = handler's Y output (host stored RDI target here) |
| 0x14031edf9 | SUBSS XMM0, [R14+0xae50] | XMM0 -= game+0xae50 |
| 0x14031ee02 | MOVSS [R14+0xae58], XMM0 | game+0xae58 = handler_Y − game+0xae50 |

R14 = game pointer (0x140317e5b: MOV R14, RCX). [RBP-0x10] in the host is the
location the host passed as RDI (output-Y pointer) to the handler. The host
then computes `game+0xae58 = handler_Y − game+0xae50`.

**Preservation requirement:** If vanilla content is replaced by an Almanac
page after 0x1403745b6, the handler must still write both outputs
(*output_X = [RBP-0x4c], *output_Y = [RSP+0x70]) at 0x140374c46/0x140374c54,
and execution must reach 0x1403826ca (cleanup) then 0x1403826f7 (stack cookie
+ epilogue). Skipping the output writes would leave game+0xae58 uncomputed,
breaking the host's downstream logic.

## 2. Early return path vs normal cleanup: initialized locals and cleanup requirements

### 2.1 Early return path (0x14037401d–0x140374035)

This path is taken when `game+0x10be8 <= sub-entry count` (0x140373fd5 JNC).
It is independent of a row click and writes both outputs before exiting:

| Address | Instruction |
|---|---|
| 0x14037401d | CALL 0x1404b79c0 (RCX=RSI=game, RDX=RBP-0x4c, R8=RSP+0x70) |
| 0x140374022 | XMM0 = [RBP-0x4c] |
| 0x140374027 | MOVSS [RBX], XMM0  (*output_X = X origin) |
| 0x14037402b | XMM1 = [RSP+0x70] |
| 0x140374031 | MOVSS [RDI], XMM1  (*output_Y = Y position) |
| 0x140374035 | JMP 0x1403826f7 |

The jump at 0x140374035 lands at **0x1403826f7**, the stack-cookie/XOR
epilogue, **bypassing all four cleanup calls** at 0x1403826d1–0x1403826f2.

At this point the only cleanup-relevant local known initialized is
[RBP+0x50] (zeroed at 0x140374050, before the row loop). The review's
correction notes this early path "does not prove that skipping the same
cleanup from after the row is safe."

### 2.2 Normal cleanup (0x1403826ca–0x1403826f7)

| Address | Instruction | Cleanup target |
|---|---|---|
| 0x1403826ca | LEA RCX,[RBP+0x90]; CALL 0x1400ff7a0 | object at RBP+0x90 |
| 0x1403826d7 | LEA RCX,[RBP+0x510]; CALL 0x1400ff840 | object at RBP+0x510 |
| 0x1403826e4 | LEA RCX,[RBP-0x70]; CALL 0x1401071e0 | object at RBP-0x70 |
| 0x1403826ee | LEA RCX,[RBP+0x50]; CALL 0x14000fed0 | object at RBP+0x50 |
| 0x1403826f7 | MOV RCX,[RBP+0xd30]; XOR RCX,RSP; CALL 0x14084dea0 | stack cookie + epilogue |

### 2.3 Initialization status of cleanup targets at 0x1403745b6 (normal row exit)

| Cleanup target | Init instruction | Init address | Initialized at 0x1403745b6? |
|---|---|---|---|
| [RBP+0x50] | MOVDQU [RBP+0x50], XMM0 (zero) | 0x140374050 | **Yes** (before row loop) |
| [RBP-0x70] | MOVDQU [RBP-0x70], XMM0 (zero) | 0x14037464f | **No** (after row exit) |
| [RBP+0x510] | MOV word [RBP+0x510], 0x0 | 0x140374752 | **No** (after row exit) |
| [RBP+0x90] | LEA RCX,[RBP+0x90]; CALL 0x14025b720 | 0x1403749f2 | **No** (after row exit) |

**Conclusion:** Jumping from 0x1403745b6 directly to cleanup at 0x1403826ca
would pass uninitialized locals to cleanup helpers for RBP+0x90, RBP+0x510,
and RBP-0x70. The review states this "would pass later, uninitialized locals
to those helpers." Only [RBP+0x50] is initialized at the row exit.

## 3. Earliest plausible content-suppression boundary

### 3.1 Flow after 0x1403745b6 (normal row exit)

| Address | Instruction | Notes |
|---|---|---|
| 0x1403749f2 | CALL 0x14025b720 (RCX=RBP+0x90) | Initializes RBP+0x90 (cleanup target 1) |
| 0x140374a69 | LEA RDX,[RSI+0x108a8]; MOV RCX,R12; CALL 0x1400e5650 | Purpose unresolved (subvector op) |
| 0x140374acc | CALL 0x1404b3230 | Content drawing (sub-tab 0–4) |
| 0x140374b35 | CALL 0x1404b5d20 | Content drawing (sub-tab 5) |
| 0x140374c25 | CALL 0x140335cc0 | Content drawing (default/other) |
| 0x140374c3d | MOVSS XMM0,[RBP-0x4c] | Output writing begins |
| 0x140374c46 | MOVSS [output_X], XMM0 | Write output X |
| 0x140374c54 | MOVSS [output_Y], XMM1 | Write output Y |
| 0x1403826ca | CALL 0x1400ff7a0 | Cleanup 1 (RBP+0x90) |
| ... | ... | Cleanup 2–4 |
| 0x1403826f7 | stack cookie + epilogue | Function exit |

### 3.2 Candidate suppression point

The earliest point at which **all cleanup targets are initialized** is
immediately after `0x1403749f2` (CALL 0x14025b720 sets RBP+0x90), since
RBP+0x510 (0x140374752), RBP-0x70 (0x14037464f), and RBP+0x50 (0x140374050)
are all initialized before that.

A candidate boundary: intercept after 0x1403749f2, skip the content-drawing
calls (0x140374a69 through 0x140374c25), jump to output writing at
0x140374c3d, then let normal cleanup proceed at 0x1403826ca.

### 3.3 Unresolved requirements

| Requirement | Status |
|---|---|
| Is CALL 0x1400e5650 (0x140374a69) content setup or skippable? | **Unresolved** — body not examined |
| Are [RBP-0x4c] and [RSP+0x70] valid without content drawing on every sub-tab path? | **Partially unresolved** — default path (0x140374c5d) does not visibly write [RSP+0x70] before the merge at 0x140374c3d |
| What do cleanup helpers 0x1400ff7a0, 0x1400ff840, 0x1401071e0, 0x14000fed0 actually do? | **Unresolved** — bodies not examined |
| Does suppressing content affect host's game+0xae58 computation? | **Unresolved** — depends on [RSP+0x70] being valid |
| Does CALL 0x140335cc0 (0x140374c25) modify output-relevant state? | **Unresolved** |
| Are there incoming edges to 0x140374c3d from other paths that must be preserved? | **Unresolved** |

### 3.4 Planning candidate (not a patch proposal)

If the above are resolved, a suppression design would:

1. Intercept after 0x1403749f2 (all cleanup locals initialized).
2. Skip content-drawing calls (0x140374a69–0x140374c25).
3. Jump to 0x140374c3d to write outputs (*output_X = [RBP-0x4c],
   *output_Y = [RSP+0x70]).
4. Fall into normal cleanup at 0x1403826ca, then epilogue at 0x1403826f7.
5. Render the Almanac page separately (mod-owned draw), without disturbing
   game+0xC65C or game+0x10be8 unless intended.

This is an investigation candidate, not an implementation. No hook is
specified or authorized. The existing 17-byte inline hook helper cannot be
used blindly here (it does not relocate RIP-relative operands), as noted in
the review.

## 4. Fact vs assumption summary

**Facts (from exports):**
- Output X/Y written at 0x140374c46/0x140374c54; host computes game+0xae58 at
  0x14031edf4–0x14031ee02.
- Early return (0x14037401d–0x140374035) writes outputs then JMPs to
  0x1403826f7, skipping all 4 cleanup calls.
- Normal cleanup: 4 calls at 0x1403826d1–0x1403826f2 targeting RBP+0x90,
  RBP+0x510, RBP-0x70, RBP+0x50.
- Cleanup-local init order: RBP+0x50 (0x140374050), RBP-0x70 (0x14037464f),
  RBP+0x510 (0x140374752), RBP+0x90 (0x1403749f2). All after 0x1403745b6
  except RBP+0x50.

**Assumptions (not proven):**
- [RBP-0x4c] and [RSP+0x70] hold correct output values without content
  drawing.
- CALL 0x1400e5650 and CALL 0x140335cc0 are not required for output
  correctness.
- Cleanup helpers are safe to call with initialized locals.

## 5. Recommended next read-only observation

Serialized Ghidra decompile of the four cleanup helpers
(0x1400ff7a0, 0x1400ff840, 0x1401071e0, 0x14000fed0) and of
0x1400e5650 / 0x140335cc0 to determine whether they touch output-relevant
state or cleanup targets. Use `DecompAt.java` with preferred VAs, one run at a
time, output to `scratch/out/<rva>_decomp.c`. Do not run concurrently with
any other Ghidra invocation.
