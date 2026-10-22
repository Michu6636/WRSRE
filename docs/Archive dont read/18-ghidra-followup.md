# 18 - Ghidra follow-up (2026-10-05): construction hook safety + M3 prologues

Target: SOVIET64.exe v1.1.1.9. All addresses RVAs; Ghidra file-base
0x140000000 (pass 0x140+RVA). One run at a time (project lock).
Raw outputs in `scratch/g_*.asm`, `scratch/g_funcs_*.txt`.

## G1 PASS - construction builder 0x7838E0 is safe to hook (B14 decided)

`DisasmMany 0x1407838E0:0x140783940` (`scratch/g_construction_7838E0.asm`):

```
1407838e0: MOV RAX,RSP / PUSH RBP / PUSH RSI / PUSH RDI
            PUSH R12 / PUSH R13 / PUSH R14 / PUSH R15
1407838ee: LEA RBP,[RAX-0x9b8] / SUB RSP,0xa90 / ...
```

First 14 bytes = `48 8B C4 55 56 57 41 54 41 55 41 56 41 57` -
exactly the INI `panel_builder_expect`. Count: 3+1+1+1+2+2+2+2 = 14,
ends on an instruction boundary immediately before the LEA.
All stolen bytes register-only, no RIP-relative operand, no
`__security_cookie` load in range (next ~0x60 bytes are all
RAX/RBP-relative MOVAPS/MOV). Meets rule 2 (>= 14, PI).

Decision: INI socket is CORRECT. Enabling the private_construction
panel-builder hook is safe; the `pdata.py` / empty-socket comments in the
INI are stale (there is no `tools/pe/` in this checkout). Keep stack scan
as fallback, fix comments when source edits reopen.

## G2 CONFIRMED - pub 0x723950 matches the family + follows append contract

`DisasmMany 0x140723950:0x140723990` (`scratch/g_pub_723950.asm`):

```
140723950: MOV RAX,RSP / PUSH RBP / RSI / RDI / R12-R15 (14 bytes,
           identical to construction/shop prefix)
14072395e: LEA RBP,[RAX-0xa98] / SUB RSP,0xb70 / MOV [RBP-0x58],-2 / ...
```

First 14 = same `48 8B C4 ... 41 57`. LEA differs only in offset
(`68 F5 FF FF` vs shop `58 F5 FF FF`), still position-independent.
21-byte steal (14 + 7 LEA) is also safe and matches the shop/mine
pattern; 14-byte minimal steal is safe too.

`ListFuncs 0x140720000-0x140730000` (`scratch/g_funcs_pub.txt`):
FUN_140723950 size 6111 - a full panel builder, consistent with
pub assignment in docs/17 s10. Neighbours 0x723190 / 0x725130 are
separate builders, not fragments.

`DecompAt 0x140723950` (`scratch/g_pub_723950.c`, 633 lines):

- `(game=param_1, window=param_2)`; `lVar4 = *(param_2+0x240)`
  (building); `*(char*)(param_2+1)` gate with early return.
- DPI `DAT_140992088`, row layout `DAT_14090a9cc/90a9f0/90ab98`
  (same constants as shop/residence/mine); fontmgr `DAT_140996fb0`,
  fonts via `DAT_140994200/140994208`; single exit writes
  `*(float*)(param_2+0x250) = local_b18`.
- Uses `C3D_LANGUAGE::GetString` + `C3D_FONTMANAGER` prints -
  the same imperative row pattern, so `AppendRows` applies unchanged.
- Cookie note: decompiler flags `security_check_cookie` but the
  prologue disassembly shows the cookie XOR far past the steal
  (`local_b8 = DAT_140992048 ^ ...` materializes later); the first
  14 (or 21 incl. LEA) stolen bytes contain no cookie load.
  Same shape as shop/residence - safe.

Hook bytes for ui_panels table (from `scratch/g_pub_723950.asm`):
`48 8B C4 55 56 57 41 54 41 55 41 56 41 57` (14, boundary-safe),
21-byte variant appends `48 8D A8 68 F5 FF FF` (LEA, PI).
Recommend 21 to match shop/mine precedent.

## G3 - factory 0x799630 CONFIRMED same family (prologue + contract markers)

`DisasmMany 0x140799630` (`scratch/g_factory_799630.asm`): same 14-byte
prefix `48 8B C4 ... 41 57`, then `LEA RBP,[RAX-0xc08]` (PI) - safe at
14 or 21 bytes. `DecompAt` (`scratch/g_factory_799630.c`):
`lVar4 = *(param_2+0x240)`, `*(char*)(param_2+1)` gate, DPI/layout
constants, `GetString` row pattern. Full +0x250 exit scan pending but all
markers match pub/shop/residence.

## G4 - fallback 0x6F4740 is a DIFFERENT shape: DO NOT inline-hook, use call-patch or dispatcher route

`DisasmMany 0x1406F4740` (`scratch/g_fallback_6F4740.asm`):

```
48 89 5C 24 08   MOV [RSP+8],RBX   (5)
57               PUSH RDI          (1)  -> 6
48 83 EC 70      SUB RSP,0x70      (4)  -> 10
80 7A 01 00      CMP byte [RDX+1],0 (4) -> 14
```

14 bytes ends mid-`JNZ` sense exactly on the CMP boundary, but the shape
is the `/GS`-adjacent prologue the rules warn about: only 10 bytes before
first real work, `MOV [RSP+8]` + tiny frame, and the window gate
(`window+0x01`, game=RCX, window=RDX) inline at +10. No 14-byte
register-only prefix exists here. Per rule 2: patch the caller
`call rel32` instead, or cover these windows via the dispatcher
`obj+0xCAC0` walk after each builder writes `window+0x250`.

## Raw file index (append)

- `scratch/g_factory_799630.asm` / `.c` - G3
- `scratch/g_fallback_6F4740.asm` - G4

