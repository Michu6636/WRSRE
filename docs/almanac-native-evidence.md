# Almanac Native Evidence: FUN_140395C90 Widget Path

This is a small, committed evidence excerpt for the Almanac investigation. It is not a full export.

## Dispatch evidence for the corrected Economy & Trade path

Source artifacts:
- `scratch/out/14031eac0_14031eb30_disasm.asm`
- `scratch/out/14031eb5f_14031ec80_disasm.asm`
- `scratch/out/14031ec80_14031ecb0_disasm.asm`
- `scratch/out/14031ece0_14031ed20_disasm.asm`
- `scratch/out/14031ed50_14031ed70_disasm.asm`
- `scratch/out/14031edaf_14031ede0_disasm.asm`
- `scratch/out/14031eddf_14031edf5_disasm.asm`
- `scratch/out/14031ee1d_14031ee30_disasm.asm`
- `scratch/out/14031ee70_14031ee80_disasm.asm`

Relevant dispatch in `FUN_140317DB0` (RVA `0x317DB0`):

```asm
14031eac3: MOV byte ptr [R14 + 0xc912], 0x1
14031eacb: MOV RBX, qword ptr [R14 + 0xc650]
14031ead2: INC RBX
14031ead5: XOR R15D, R15D
14031ead8: MOV ECX, R15D
14031eadb: LEA RDX, [0x140879164]   ; "system"
...
14031eaf5: MOV RCX, R14
14031eaf8: CALL 0x140337450          ; system handler
14031eafd: JMP 0x14031f018
...
14031eb49: LEA RDX, [0x1408b2108]   ; "infrastructure"
...
14031eb5c: MOV RCX, R14
14031eb5f: CALL 0x140395c90          ; infrastructure handler
14031eb64: JMP 0x14031f018
...
14031eb69: LEA RDX, [0x1408b20f8]   ; "building"
...
14031eb7c: MOV RCX, R14
14031eb7f: CALL 0x140389f10          ; building handler
...
14031eb89: LEA RDX, [0x1408b20e0]   ; "workshopbuildings"
...
14031eb9c: XOR EDX, EDX
14031eb9e: MOV RCX, R14
14031eba1: CALL 0x140388140
...
14031ebab: LEA RDX, [0x1408b2060]   ; "editorbuildings"
...
14031ebbe: MOV EDX, R13D
14031ebc1: MOV RCX, R14
14031ebc4: CALL 0x140388140
...
14031ece0: LEA RDX, [0x1408b2080]   ; "const_offi"
...
14031ecf3: MOV RCX, qword ptr [R14 + 0xd448]
14031ecfa: CALL 0x1405f00d0
...
14031ed5c: LEA RDX, [0x1408b21a0]   ; "veh_replace"
...
14031ed6f: MOV RCX, qword ptr [R14 + 0xd440]
14031ed76: CALL 0x1405f00d0
...
14031ec89: MOV RCX, R15
14031ec8c: LEA RDX, [0x140879134]   ; "mtlist"
...
14031eca6: XOR EDX, EDX
14031eca8: MOV RCX, qword ptr [R14 + 0xd410]
14031ecaf: CALL 0x1404a12e0
...
14031edaf: MOV RCX, R15
14031edb2: LEA RDX, [0x1408b2194]   ; "trade"
...
14031edd3: MOV dword ptr [RBP + 0x48], R15D
14031edd7: MOV dword ptr [RBP - 0x10], R15D
14031eddb: LEA R8, [RBP - 0x10]
14031eddf: LEA RDX, [RBP + 0x48]
14031ede3: MOV RCX, R14
14031ede6: CMP byte ptr [R14 + 0x10398], R15B
14031eded: JZ 0x14031ee10
14031edef: CALL 0x140373ea0
...
14031ee1d: MOV RCX, R15
14031ee20: LEA RDX, [0x1408b218c]   ; "list"
...
14031ee73: MOV RCX, R14
14031ee76: CALL 0x140358a20          ; reached for names not matched above, including "importexport"
14031ee7b: JMP 0x14031f018
```

Interpretation:
- An entry named "importexport" is not matched by the explicit comparisons above, so it reaches `FUN_140358A20` at RVA `0x358A20`.
- This is static dispatch evidence, not proof that the entry is selected at runtime or that the handler is suitable.
- `FUN_140358A20` has no slider or checkbox calls in its reviewed call list. Its own suitability remains unresolved.

## Widget-path evidence

Source artifacts: `scratch/out/140396900_140396ac0_disasm.asm` and `scratch/out/140396a80_140396ae0_disasm.asm` (uncommitted scratch output).

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

Claim status:
- Supported: `FUN_140395C90` calls the slider and checkbox in at least one path.
- Supported: RSI is the game pointer in `FUN_140395C90`; R15 is 0 on the observed master path.
- Supported: the slider path requires the construction/building type values listed above.
- Unresolved: whether an Economy & Trade/importexport entry reaches this path.
- Unresolved: whether `FUN_140395C90` is a safe host for an Almanac page.

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

## Selected-entry write at RVA 0x843F3 in `FUN_14007C400`

Source artifact: `scratch/out/14007c400_140084754_disasm.asm` (uncommitted scratch output).
Referenced strings verified in `.rdata`: `0x14087bfd8` = "objectives", `0x140871344`–`0x140871346` = "qa" + null, `0x14087c000` = "makecity".

```asm
1400843bc: CMP AL,byte ptr [0x140871344]      ; [RBX+0] == 'q'?
1400843c2: JNZ 0x1400843dc                   ; if not, compare to "objectives"
1400843c4: MOVZX EAX,byte ptr [RBX + 0x1]    ; [RBX+1]
1400843c8: CMP AL,byte ptr [0x140871345]      ; == 'a'?
1400843ce: JNZ 0x1400843dc                   ; if not, compare to "objectives"
1400843d0: MOVZX EAX,byte ptr [RBX + 0x2]    ; [RBX+2]
1400843d4: CMP AL,byte ptr [0x140871346]      ; == 0x00 (name == "qa")?
1400843da: JZ 0x14008440a                     ; if "qa", go to "makecity" compare
1400843dc: LEA RDX,[0x14087bfd8]              ; "objectives"
1400843e3: MOV RCX,RBX                        ; RCX = entry name to compare
1400843e6: CALL 0x14084f520                   ; strcmp(entry_name, "objectives")
1400843eb: TEST EAX,EAX
1400843ed: JZ 0x14008440a                     ; if "objectives", go to "makecity" compare
1400843ef: MOV qword ptr [R13],RDI
1400843f3: MOV qword ptr [R14 + 0xc650],RDI   ; SELECTED ENTRY WRITE
1400843fa: MOV dword ptr [R14 + 0xae54],0xc47a0000
140084405: JMP 0x140084641
14008440a: LEA RDX,[0x14087c000]              ; "makecity"
140084411: MOV RCX,RBX
140084414: CALL 0x14084f520                   ; strcmp(entry_name, "makecity")
140084419: TEST EAX,EAX
14008441b: JNZ 0x140084641
```

Registers/objects:
- `R14` = `game` pointer (set at `0x14007c47e`: `MOV R14,RCX`).
- `[R14 + 0xc650]` = global selected entry pointer (the object being changed at 0x1400843F3).
- `RBX` = pointer to an entry name being compared against fixed strings.
- `RDI` = candidate entry pointer written into `game+0xC650` and `[R13]`.

Interpretation:
- `FUN_14007C400` is called by `FUN_140317DB0` at RVA `0x31BB22` during sidebar drawing, after setting panel dimensions (`0x14031BB1F`–`0x14031BB27`).
- `FUN_14007C400` reads `game+0xC650` and conditionally writes it at `0x1400843F3`.
- The write is guarded by entry-name string comparisons ("qa", "objectives", "makecity"). It fires for entry names that do not match those special cases.
- This is part of the sub-tab/button-row renderer path, but the click→write connection (mouse latch/collision) is NOT shown in this window.

Claim status:
- Supported: `FUN_14007C400` writes `game+0xC650` (selected entry) at RVA `0x843F3` (preferred VA `0x1400843F3`) when called from the master sidebar function.
- Supported: `R14` is the game pointer; `[R14 + 0xc650]` is the selected entry pointer being changed; `RDI` is the new selected entry.
- Supported: the write is reachable only through the string-comparison gates shown above.
- Unresolved: whether `RBX` iteration corresponds to clicking visible section buttons (no click latch/collision shown in this window).
- Unresolved: what `RDI` points to and how entries are enumerated/counted before this write.
- Unresolved: whether adding a new button/entry name is safe (button capacity, vector ownership, array bounds not established).
- Unresolved: whether `FUN_14007C400` alone can host a separate Almanac page without covering vanilla content.

## Claim status summary

- Supported: Economy & Trade ("importexport") is not matched by the explicit dispatch branches and reaches `FUN_140358A20`.
- Supported: `FUN_140358A20` has no native slider/checkbox calls.
- Supported: `FUN_14007C400` is the master-called button-row renderer and writes the selected entry.
- Unresolved: button count/capacity and safe extensibility.
- Unresolved: whether `FUN_140358A20` is a safe host for an Almanac page.