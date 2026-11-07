# Almanac Native Evidence: FUN_140395C90 Widget Path

This is a small, committed evidence excerpt for the Almanac investigation. It is not a full export.

## Function and path

- Host function: `FUN_140317DB0`, RVA `0x317DB0` (preferred VA `0x140317DB0`).
- Fallback handler: `FUN_140395C90`, RVA `0x395C90` (preferred VA `0x140395C90`), size 55,231 bytes.
- `FUN_140395C90` is called from `FUN_140317DB0` at RVA `0x31EB5F` when the selected entry name is not one of "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "timelapse", "notifi_tab", "objectives", or "scripts".
- The Economy & Trade entry is commonly identified by the unique string "importexport", which is not in that explicit list. Therefore it is likely, but not runtime-proven, that an Economy & Trade entry reaches `FUN_140395C90`.

## Slider call site

Source artifact: `scratch/out/140396a80_140396ae0_disasm.asm` (uncommitted scratch output).

```asm
140396a80:  ...
140396a82:  MOV EAX, EDX
140396a84:  MOV dword ptr [RSP + 0x38], 0x5
140396a8c:  SHR EAX, 0x1f
140396a8f:  ADDSS XMM1, XMM12
140396a94:  ADD EDX, EAX
140396a96:  MOV byte ptr [RSP + 0x30], 0x1
140396a9b:  IMUL EAX, EDX, 0x64
140396a9e:  MOVSS dword ptr [RSP + 0x28], XMM0
140396aa4:  SUB ECX, EAX
140396aa6:  LEA RAX, [RBP + -0x70]
140396aaa:  MOV dword ptr [RBP + -0x70], ECX
140396aad:  MOV RCX, RSI
140396ab0:  MOV qword ptr [RSP + 0x20], RAX
140396ab5:  CALL 0x1402727F0
```

## Gate for the slider call

Source artifact: `scratch/out/140396900_140396ac0_disasm.asm`.

```asm
140396909: CMP byte ptr [RSI + 0x1090], R15B
140396910: JNZ 0x140396d2e
140396916: MOV RCX, qword ptr [RSI + 0xd428]
14039691d: TEST RCX, RCX
140396920: JZ 0x140396d2e
140396926: MOV RCX, qword ptr [RCX + 0x48]
14039692a: TEST RCX, RCX
14039692d: JZ 0x140396d2e
140396933: MOV EAX, dword ptr [RCX + 0x360]
140396939: CMP EAX, 0x3
14039693c: JZ 0x14039699e
14039693e: CMP EAX, 0x22
140396941: JZ 0x14039699e
140396943: CMP EAX, 0x2d
140396946: JNZ 0x14039695a
140396948: CMP dword ptr [RCX + 0x820], R15D
14039694f: JLE 0x14039695a
140396951: CMP dword ptr [RSI + 0x5b0], 0x2
140396958: JZ 0x14039699e
14039695a: TEST EAX, EAX
14039695c: JNZ 0x140396977
14039695e: MOV EDX, dword ptr [RCX + 0x364]
140396964: CMP EDX, 0x13
140396967: JZ 0x14039696e
140396969: CMP EDX, 0x21
14039696c: JNZ 0x140396977
14039696e: CMP dword ptr [RSI + 0x5b0], 0x2
140396975: JZ 0x14039699e
140396977: CMP EAX, 0x1
14039697a: JNZ 0x140396995
14039697c: MOV EDX, dword ptr [RCX + 0x364]
140396982: CMP EDX, 0x13
140396985: JZ 0x14039698c
140396987: CMP EDX, 0x21
14039698a: JNZ 0x140396995
14039698c: CMP dword ptr [RSI + 0x5b0], 0x2
140396993: JZ 0x14039699e
140396995: CMP EAX, 0x17
140396998: JNZ 0x140396d2e
14039699e: ...
140396a11: CALL 0x1403956f0
...
140396ab5: CALL 0x1402727f0
```

## Claim status

- Supported: `FUN_140395C90` can call the slider and checkbox in at least one path.
- Supported: the slider call at RVA `0x396AB5` is gated by the conditions shown above.
- Supported: RSI is the game pointer in `FUN_140395C90`; R15 is 0 on the observed master path to `FUN_140395C90`.
- Supported: the allowed type values are construction/building-oriented (`0x03`, `0x22`, `0x2D`, `0x00` with subtype `0x13`/`0x21`, `0x01` with subtype `0x13`/`0x21`, or `0x17`).
- Unresolved: whether an Economy & Trade/importexport entry has the field values required by those gates.
- Unresolved: whether `FUN_140395C90` is a safe or suitable host for an Almanac page.
- Unresolved: content rectangle, clipping, scrolling, close behavior, and input safety.