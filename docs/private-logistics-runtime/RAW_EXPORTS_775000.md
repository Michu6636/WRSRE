# Raw Ghidra Exports - FUN_140775000

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON
**All addresses are RVAs** (file-based = 0x140 + RVA)
**Generated:** 2026-10-29 via scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21)

---

## 1. Full Decompilation of FUN_140775000 (0x140775000)

```c
// ============================================================
// Function: FUN_140775000 at 140775000
// Target address checked: 0x140775000
// ============================================================

void FUN_140775000(undefined8 param_1,float *param_2,float *param_3,longlong param_4)

{
  longlong lVar1;
  float fVar2;
  wchar_t *pwVar3;
  longlong lVar4;
  longlong lVar5;
  uint uVar6;
  ulonglong uVar7;
  float fVar8;
  float local_res10 [2];
  float *local_res18;
  float local_res20 [2];
  wchar_t *pwVar9;
  
  fVar2 = DAT_14090aa0c;
  lVar1 = *(longlong *)(param_4 + 0x240);
  local_res20[0] = *param_2;
  local_res10[0] = *param_3;
  if ((*(longlong *)(lVar1 + 0x318) != 0) &&
     (*(int *)(*(longlong *)(lVar1 + 0x318) + 0x360) == 0x69)) {
    *param_2 = local_res20[0];
    *param_3 = local_res10[0];
    return;
  }
  lVar4 = *(longlong *)(lVar1 + 0x978) - *(longlong *)(lVar1 + 0x970);
  uVar6 = 0;
  lVar5 = lVar4 >> 0x3f;
  if (lVar4 / 0xe0 + lVar5 == lVar5) {
    *param_2 = local_res20[0];
    *param_3 = local_res10[0];
  }
  else {
    uVar7 = 0;
    local_res18 = param_3;
    do {
      lVar5 = uVar7 * 0xe0 + *(longlong *)(lVar1 + 0x970);
      if (*(int *)(lVar5 + 0x88) - 5U < 2) {
        fVar8 = DAT_140992088 * 0.0;
        pwVar3 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x10a8);
        pwVar9 = L"%ls:";
        C3D_FONTMANAGER::PrintLeftUnicode
                  ((C3D_FONTMANAGER *)&DAT_140996fb0,DAT_140995220,-NAN,4.49691,
                   (ulong)(ulonglong)(uint)(fVar8 + local_res20[0]),
                   (wchar_t *)(ulonglong)(uint)(fVar8 + local_res10[0]),pwVar3);
        local_res10[0] = local_res10[0] + DAT_140992088 * fVar2;
        FUN_140706c00(param_1,param_4,lVar5,local_res20,local_res10,
                      (ulonglong)pwVar9 & 0xffffffff00000000,1,0,0xffffffff,
                      *(undefined4 *)(lVar1 + 0x9e8),uVar6,0,0,0,1,0);
      }
      uVar6 = uVar6 + 1;
      uVar7 = (ulonglong)uVar6;
    } while (uVar7 < (ulonglong)
                     ((*(longlong *)(lVar1 + 0x978) - *(longlong *)(lVar1 + 0x970)) / 0xe0));
    *param_2 = local_res20[0];
    *local_res18 = local_res10[0];
  }
  return;
}




```

---

## 2. Full Disassembly of FUN_140775000 (0x140775000-0x140775294)

```asm
; Range: 0x140775000 to 0x140775294
140775000:  48 8B C4                 MOV RAX,RSP
140775003:  4C 89 40 18              MOV qword ptr [RAX + 0x18],R8
140775007:  55                       PUSH RBP
140775008:  57                       PUSH RDI
140775009:  41 54                    PUSH R12
14077500b:  41 56                    PUSH R14
14077500d:  41 57                    PUSH R15
14077500f:  48 81 EC D0 00 00 00     SUB RSP,0xd0
140775016:  49 8B B9 40 02 00 00     MOV RDI,qword ptr [R9 + 0x240]
14077501d:  4D 8B F9                 MOV R15,R9
140775020:  F3 0F 10 02              MOVSS XMM0,dword ptr [RDX]
140775024:  4C 8B F2                 MOV R14,RDX
140775027:  F3 41 0F 10 08           MOVSS XMM1,dword ptr [R8]
14077502c:  4C 8B E1                 MOV R12,RCX
14077502f:  F3 0F 11 40 20           MOVSS dword ptr [RAX + 0x20],XMM0
140775034:  F3 0F 11 48 10           MOVSS dword ptr [RAX + 0x10],XMM1
140775039:  48 8B 87 18 03 00 00     MOV RAX,qword ptr [RDI + 0x318]
140775040:  48 85 C0                 TEST RAX,RAX
140775043:  74 22                    JZ 0x140775067
140775045:  83 B8 60 03 00 00 69     CMP dword ptr [RAX + 0x360],0x69
14077504c:  75 19                    JNZ 0x140775067
14077504e:  F3 0F 11 02              MOVSS dword ptr [RDX],XMM0
140775052:  F3 41 0F 11 08           MOVSS dword ptr [R8],XMM1
140775057:  48 81 C4 D0 00 00 00     ADD RSP,0xd0
14077505e:  41 5F                    POP R15
140775060:  41 5E                    POP R14
140775062:  41 5C                    POP R12
140775064:  5F                       POP RDI
140775065:  5D                       POP RBP
140775066:  C3                       RET
140775067:  48 8B 8F 78 09 00 00     MOV RCX,qword ptr [RDI + 0x978]
14077506e:  48 2B 8F 70 09 00 00     SUB RCX,qword ptr [RDI + 0x970]
140775075:  48 89 B4 24 C8 00 00 00  MOV qword ptr [RSP + 0xc8],RSI
14077507d:  33 F6                    XOR ESI,ESI
14077507f:  4C 89 AC 24 C0 00 00 00  MOV qword ptr [RSP + 0xc0],R13
140775087:  49 BD 25 49 92 24 49 92 24 49 MOV R13,0x4924924924924925
140775091:  49 8B C5                 MOV RAX,R13
140775094:  48 F7 E9                 IMUL RCX
140775097:  48 C1 FA 06              SAR RDX,0x6
14077509b:  48 8B C2                 MOV RAX,RDX
14077509e:  48 C1 E8 3F              SHR RAX,0x3f
1407750a2:  48 03 D0                 ADD RDX,RAX
1407750a5:  0F 84 BF 01 00 00        JZ 0x14077526a
1407750ab:  48 89 9C 24 00 01 00 00  MOV qword ptr [RSP + 0x100],RBX
1407750b3:  48 8D 2D F6 95 18 00     LEA RBP,[0x1408fe6b0]
1407750ba:  0F 29 BC 24 A0 00 00 00  MOVAPS xmmword ptr [RSP + 0xa0],XMM7
1407750c2:  45 33 C0                 XOR R8D,R8D
1407750c5:  44 0F 29 84 24 90 00 00 00 MOVAPS xmmword ptr [RSP + 0x90],XMM8
1407750ce:  0F 57 FF                 XORPS XMM7,XMM7
1407750d1:  F3 44 0F 10 05 32 59 19 00 MOVSS XMM8,dword ptr [0x14090aa0c]
1407750da:  0F 29 B4 24 B0 00 00 00  MOVAPS xmmword ptr [RSP + 0xb0],XMM6
1407750e2:  49 69 D8 E0 00 00 00     IMUL RBX,R8,0xe0
1407750e9:  48 03 9F 70 09 00 00     ADD RBX,qword ptr [RDI + 0x970]
1407750f0:  8B 83 88 00 00 00        MOV EAX,dword ptr [RBX + 0x88]
1407750f6:  83 E8 05                 SUB EAX,0x5
1407750f9:  83 F8 01                 CMP EAX,0x1
1407750fc:  0F 87 F1 00 00 00        JA 0x1407751f3
140775102:  F3 0F 10 35 7E CF 21 00  MOVSS XMM6,dword ptr [0x140992088]
14077510a:  48 8D 0D 7F 24 22 00     LEA RCX,[0x140997590]
140775111:  BA A8 10 00 00           MOV EDX,0x10a8
140775116:  F3 0F 59 F7              MULSS XMM6,XMM7
14077511a:  FF 15 70 77 0F 00        CALL qword ptr [0x14086c890]
140775120:  48 8B 15 F9 00 22 00     MOV RDX,qword ptr [0x140995220]
140775127:  48 8D 0D 82 1E 22 00     LEA RCX,[0x140996fb0]
14077512e:  0F 28 DE                 MOVAPS XMM3,XMM6
140775131:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140775136:  F3 0F 58 B4 24 18 01 00 00 ADDSS XMM6,dword ptr [RSP + 0x118]
14077513f:  F3 0F 58 9C 24 08 01 00 00 ADDSS XMM3,dword ptr [RSP + 0x108]
140775148:  48 89 6C 24 28           MOV qword ptr [RSP + 0x28],RBP
14077514d:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
140775155:  0F 28 D6                 MOVAPS XMM2,XMM6
140775158:  66 41 0F 7E D0           MOVD R8D,XMM2
14077515d:  66 41 0F 7E D9           MOVD R9D,XMM3
140775162:  FF 15 18 77 0F 00        CALL qword ptr [0x14086c880]
140775168:  F3 0F 10 8C 24 08 01 00 00 MOVSS XMM1,dword ptr [RSP + 0x108]
140775171:  4C 8D 8C 24 18 01 00 00  LEA R9,[RSP + 0x118]
140775179:  F3 0F 10 05 07 CF 21 00  MOVSS XMM0,dword ptr [0x140992088]
140775181:  4C 8B C3                 MOV R8,RBX
140775184:  C6 44 24 78 00           MOV byte ptr [RSP + 0x78],0x0
140775189:  49 8B D7                 MOV RDX,R15
14077518c:  C6 44 24 70 01           MOV byte ptr [RSP + 0x70],0x1
140775191:  49 8B CC                 MOV RCX,R12
140775194:  F3 0F 11 7C 24 68        MOVSS dword ptr [RSP + 0x68],XMM7
14077519a:  F3 0F 11 7C 24 60        MOVSS dword ptr [RSP + 0x60],XMM7
1407751a0:  F3 0F 11 7C 24 58        MOVSS dword ptr [RSP + 0x58],XMM7
1407751a6:  89 74 24 50              MOV dword ptr [RSP + 0x50],ESI
1407751aa:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1407751af:  F3 0F 58 C8              ADDSS XMM1,XMM0
1407751b3:  F3 0F 11 8C 24 08 01 00 00 MOVSS dword ptr [RSP + 0x108],XMM1
1407751bc:  8B 87 E8 09 00 00        MOV EAX,dword ptr [RDI + 0x9e8]
1407751c2:  89 44 24 48              MOV dword ptr [RSP + 0x48],EAX
1407751c6:  48 8D 84 24 08 01 00 00  LEA RAX,[RSP + 0x108]
1407751ce:  C7 44 24 40 FF FF FF FF  MOV dword ptr [RSP + 0x40],0xffffffff
1407751d6:  F3 0F 11 7C 24 38        MOVSS dword ptr [RSP + 0x38],XMM7
1407751dc:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
1407751e1:  C7 44 24 28 00 00 00 00  MOV dword ptr [RSP + 0x28],0x0
1407751e9:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1407751ee:  E8 0D 1A F9 FF           CALL 0x140706c00
1407751f3:  48 8B 8F 78 09 00 00     MOV RCX,qword ptr [RDI + 0x978]
1407751fa:  FF C6                    INC ESI
1407751fc:  48 2B 8F 70 09 00 00     SUB RCX,qword ptr [RDI + 0x970]
140775203:  49 8B C5                 MOV RAX,R13
140775206:  48 F7 E9                 IMUL RCX
140775209:  44 8B C6                 MOV R8D,ESI
14077520c:  48 C1 FA 06              SAR RDX,0x6
140775210:  48 8B C2                 MOV RAX,RDX
140775213:  48 C1 E8 3F              SHR RAX,0x3f
140775217:  48 03 D0                 ADD RDX,RAX
14077521a:  4C 3B C2                 CMP R8,RDX
14077521d:  0F 82 BF FE FF FF        JC 0x1407750e2
140775223:  48 8B AC 24 10 01 00 00  MOV RBP,qword ptr [RSP + 0x110]
14077522b:  F3 0F 10 84 24 18 01 00 00 MOVSS XMM0,dword ptr [RSP + 0x118]
140775234:  F3 0F 10 8C 24 08 01 00 00 MOVSS XMM1,dword ptr [RSP + 0x108]
14077523d:  44 0F 28 84 24 90 00 00 00 MOVAPS XMM8,xmmword ptr [RSP + 0x90]
140775246:  0F 28 BC 24 A0 00 00 00  MOVAPS XMM7,xmmword ptr [RSP + 0xa0]
14077524e:  0F 28 B4 24 B0 00 00 00  MOVAPS XMM6,xmmword ptr [RSP + 0xb0]
140775256:  48 8B 9C 24 00 01 00 00  MOV RBX,qword ptr [RSP + 0x100]
14077525e:  F3 41 0F 11 06           MOVSS dword ptr [R14],XMM0
140775263:  F3 0F 11 4D 00           MOVSS dword ptr [RBP],XMM1
140775268:  EB 0A                    JMP 0x140775274
14077526a:  F3 41 0F 11 06           MOVSS dword ptr [R14],XMM0
14077526f:  F3 41 0F 11 08           MOVSS dword ptr [R8],XMM1
140775274:  48 8B B4 24 C8 00 00 00  MOV RSI,qword ptr [RSP + 0xc8]
14077527c:  4C 8B AC 24 C0 00 00 00  MOV R13,qword ptr [RSP + 0xc0]
140775284:  48 81 C4 D0 00 00 00     ADD RSP,0xd0
14077528b:  41 5F                    POP R15
14077528d:  41 5E                    POP R14
14077528f:  41 5C                    POP R12
140775291:  5F                       POP RDI
140775292:  5D                       POP RBP
140775293:  C3                       RET


```

---



---

## 3. Analysis - Evidence and Interpretation

### 3.1 Known Inputs (from caller FUN_1406F4B40 at 0x1406F4BE4)

Per x64 calling convention:
- RCX = param_1 = context (undefined8)
- RDX = param_2 = &horizontal layout value (float*)
- R8 = param_3 = &vertical layout value (float*)
- R9 = param_4 = window (longlong)

Call condition: `window + 0x248 == 0`.

### 3.2 Function Signature (verbatim from decompilation)

`void FUN_140775000(undefined8 param_1, float *param_2, float *param_3, longlong param_4)`

### 3.3 Offsets Accessed

**On param_4 (window):**
- `window + 0x240` -> associated-object UNION pointer (`lVar1`)

**On `*(window + 0x240)` (associated object `lVar1`):**
- `lVar1 + 0x318` -> pointer checked for NULL and type
- `lVar1 + 0x318 + 0x360` -> int compared to 0x69 (early-return gate)
- `lVar1 + 0x970` -> list base address
- `lVar1 + 0x978` -> list end address
- `lVar1 + 0x9e8` -> undefined4 passed as argument to FUN_140706c00

**On list entries (`lVar1 + 0x970 + i * 0xe0`):**
- `entry + 0x88` -> int compared: `*(int *)(entry + 0x88) - 5U < 2` matches types 5 and 6

### 3.4 Callee Delegation

- `FUN_140706c00(param_1, param_4, entry_ptr, &horizontal, &vertical, format_ptr, 1, 0, 0xffffffff, *(undefined4 *)(lVar1 + 0x9e8), index, 0, 0, 0, 1, 0)` â€” called for each matching entry; the last argument `index` increments per match (0, 1, 2, ...)
- `C3D_LANGUAGE::GetString(0x10a8)` -> string for rendering label
- `C3D_FONTMANAGER::PrintLeftUnicode` -> renders `L"%ls:"` label at computed position

### 3.5 Role Assessment

**Role:** Renders a vertical list of associated-object child entries whose type field at `entry + 0x88` is 5 or 6. For each matching entry, it:
1. Renders a text label (`L"%ls:"`) using a string from `GetString(0x10a8)` at the current vertical position
2. Delegates full rendering of the entry to `FUN_140706c00`, passing the entry pointer, horizontal/vertical layout pointers, window, an associated-object field at `+0x9e8`, and a sequential index
3. Advances the vertical position by `DAT_140992088 * DAT_14090aa0c` per entry

### 3.6 Vehicle Enumeration / Selection Assessment

**Neither vehicles nor vehicle selection are present.**

- The list at `lVar1 + 0x970/0x978` is iterated with stride 0xe0 â€” this is an associated-object child list, not a vehicle fleet (DO+0xC70/C78 is the fleet; DO+0xD38/D40 is the task list).
- Entry type check at `entry + 0x88` matches types 5 or 6 â€” no vehicle pointer origin is traced.
- No mouse hit-test, no click-edge flag, no click handling exists in this function.
- No action consumes a vehicle pointer.
- Behavior is delegated to `FUN_140706c00` for per-entry rendering; that callee is not investigated here per task instructions.

### 3.7 Conclusion

**Confirmed:** `FUN_140775000` is a list renderer for associated-object child entries (types 5 and 6). It does not handle vehicles, does not perform click handling, and does not trace vehicle pointer origins. The exact next callee is `FUN_140706c00`, called with the entry pointer, layout pointers, window, associated-object field `+0x9e8`, and a sequential index.

**Unknown:** Whether `FUN_140706c00` handles vehicles or click conditions. Not investigated per task scope.

### 3.8 Tools and Limitations

- Tools: `scratch/ghidra_run.bat` (Ghidra 12.1.4, JDK 21), `DecompAt.java`, `DisasmMany.java`
- Errors/limitations: None. Decompilation and disassembly both completed successfully.
- No runtime code, hooks, configuration, or billing was modified.