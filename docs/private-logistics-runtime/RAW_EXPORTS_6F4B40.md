# Raw Ghidra Exports — FUN_1406F4B40

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON  
**All addresses are RVAs** (file-based = 0x140 + RVA)  
**Generated:** 2026-10-29 via scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21)

---

## 1. Full Decompilation of FUN_1406F4B40 (0x1406F4B40)

```c
// ============================================================
// Function: FUN_1406f4b40 at 1406f4b40
// Target address checked: 0x1406F4B40
// ============================================================

void FUN_1406f4b40(longlong param_1,longlong param_2,undefined8 param_3,float *param_4)

{
  longlong lVar1;
  float fVar2;
  longlong lVar3;
  bool bVar4;
  uint uVar5;
  float local_res8 [2];
  ulonglong uVar6;
  
  lVar1 = param_2 + 0x580;
  FUN_140042de0(lVar1,param_4);
  fVar2 = *param_4;
  FUN_140772400(param_1,param_3,param_4,param_2);
  if (*param_4 != fVar2) {
    FUN_140042de0(lVar1,param_4);
  }
  fVar2 = *param_4;
  FUN_1407752a0(param_1,param_3,param_4,param_2);
  if (*param_4 != fVar2) {
    FUN_140042de0(lVar1,param_4);
  }
  fVar2 = *param_4;
  if (*(int *)(param_2 + 0x248) == 0) {
    FUN_140775000(param_1,param_3,param_4,param_2);
  }
  if (*param_4 != fVar2) {
    FUN_140042de0(lVar1,param_4);
  }
  fVar2 = *param_4;
  if ((((*(longlong *)(param_2 + 0x240) != 0) && (*(int *)(param_2 + 0x248) == 0)) &&
      (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x240) + 0x318), lVar3 != 0)) &&
     (((*(float *)(lVar3 + 0x77c) != 0.0 || (*(float *)(lVar3 + 0x780) != 0.0)) ||
      ((*(uint *)(lVar3 + 0x360) < 0x24 &&
       ((0x980080000U >> ((longlong)(int)*(uint *)(lVar3 + 0x360) & 0x3fU) & 1) != 0)))))) {
    FUN_140702730(param_1,param_2,param_3,param_4);
  }
  if (*param_4 != fVar2) {
    FUN_140042de0(lVar1,param_4);
  }
  fVar2 = *param_4;
  bVar4 = true;
  if ((*(int *)(param_2 + 0x248) == 0) &&
     (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x240) + 0x318), lVar3 != 0)) {
    bVar4 = true;
    if (*(int *)(lVar3 + 0x360) == 0x22) {
      bVar4 = false;
    }
  }
  if (((*(int *)(param_1 + 0x5b4) != 0) && (bVar4)) && (*(int *)(param_2 + 0x248) == 0)) {
    lVar3 = *(longlong *)(param_2 + 0x240);
    local_res8[0] = *(float *)(param_2 + 0x28) + *(float *)(param_2 + 4);
    uVar6 = (*(longlong *)(lVar3 + 0x978) - *(longlong *)(lVar3 + 0x970)) / 0xe0;
    if ((int)uVar6 != 0) {
      do {
        uVar5 = (int)uVar6 - 1;
        uVar6 = (ulonglong)uVar5;
        if (*(int *)(uVar6 * 0xe0 + 0x90 + *(longlong *)(lVar3 + 0x970)) == 0xf) {
          FUN_1406f4db0(param_1,param_2,lVar3,local_res8,param_4);
          break;
        }
      } while (uVar5 != 0);
    }
  }
  if (*param_4 != fVar2) {
    FUN_140042de0(lVar1,param_4);
  }
  if (*(int *)(param_2 + 0x24c) == 2) {
    FUN_1407a2a60(param_1,param_3,param_4,param_2);
  }
  return;
}



```

---

## 2. Full Disassembly of FUN_1406F4B40 (0x1406F4B40–0x1406F4DAC)

```asm
; Range: 0x1406F4B40 to 0x1406F4DAC
1406f4b40:  48 89 5C 24 10           MOV qword ptr [RSP + 0x10],RBX
1406f4b45:  48 89 6C 24 18           MOV qword ptr [RSP + 0x18],RBP
1406f4b4a:  56                       PUSH RSI
1406f4b4b:  57                       PUSH RDI
1406f4b4c:  41 56                    PUSH R14
1406f4b4e:  48 83 EC 40              SUB RSP,0x40
1406f4b52:  48 8D AA 80 05 00 00     LEA RBP,[RDX + 0x580]
1406f4b59:  0F 29 74 24 30           MOVAPS xmmword ptr [RSP + 0x30],XMM6
1406f4b5e:  48 8B FA                 MOV RDI,RDX
1406f4b61:  48 8B F1                 MOV RSI,RCX
1406f4b64:  48 8B CD                 MOV RCX,RBP
1406f4b67:  49 8B D1                 MOV RDX,R9
1406f4b6a:  49 8B D9                 MOV RBX,R9
1406f4b6d:  4D 8B F0                 MOV R14,R8
1406f4b70:  E8 6B E2 94 FF           CALL 0x140042de0
1406f4b75:  F3 0F 10 33              MOVSS XMM6,dword ptr [RBX]
1406f4b79:  4C 8B CF                 MOV R9,RDI
1406f4b7c:  4C 8B C3                 MOV R8,RBX
1406f4b7f:  49 8B D6                 MOV RDX,R14
1406f4b82:  48 8B CE                 MOV RCX,RSI
1406f4b85:  E8 76 D8 07 00           CALL 0x140772400
1406f4b8a:  F3 0F 10 03              MOVSS XMM0,dword ptr [RBX]
1406f4b8e:  0F 2E C6                 UCOMISS XMM0,XMM6
1406f4b91:  7A 02                    JP 0x1406f4b95
1406f4b93:  74 0B                    JZ 0x1406f4ba0
1406f4b95:  48 8B D3                 MOV RDX,RBX
1406f4b98:  48 8B CD                 MOV RCX,RBP
1406f4b9b:  E8 40 E2 94 FF           CALL 0x140042de0
1406f4ba0:  F3 0F 10 33              MOVSS XMM6,dword ptr [RBX]
1406f4ba4:  4C 8B CF                 MOV R9,RDI
1406f4ba7:  4C 8B C3                 MOV R8,RBX
1406f4baa:  49 8B D6                 MOV RDX,R14
1406f4bad:  48 8B CE                 MOV RCX,RSI
1406f4bb0:  E8 EB 06 08 00           CALL 0x1407752a0
1406f4bb5:  F3 0F 10 03              MOVSS XMM0,dword ptr [RBX]
1406f4bb9:  0F 2E C6                 UCOMISS XMM0,XMM6
1406f4bbc:  7A 02                    JP 0x1406f4bc0
1406f4bbe:  74 0B                    JZ 0x1406f4bcb
1406f4bc0:  48 8B D3                 MOV RDX,RBX
1406f4bc3:  48 8B CD                 MOV RCX,RBP
1406f4bc6:  E8 15 E2 94 FF           CALL 0x140042de0
1406f4bcb:  83 BF 48 02 00 00 00     CMP dword ptr [RDI + 0x248],0x0
1406f4bd2:  F3 0F 10 33              MOVSS XMM6,dword ptr [RBX]
1406f4bd6:  75 11                    JNZ 0x1406f4be9
1406f4bd8:  4C 8B CF                 MOV R9,RDI
1406f4bdb:  4C 8B C3                 MOV R8,RBX
1406f4bde:  49 8B D6                 MOV RDX,R14
1406f4be1:  48 8B CE                 MOV RCX,RSI
1406f4be4:  E8 17 04 08 00           CALL 0x140775000
1406f4be9:  F3 0F 10 03              MOVSS XMM0,dword ptr [RBX]
1406f4bed:  0F 2E C6                 UCOMISS XMM0,XMM6
1406f4bf0:  7A 02                    JP 0x1406f4bf4
1406f4bf2:  74 0B                    JZ 0x1406f4bff
1406f4bf4:  48 8B D3                 MOV RDX,RBX
1406f4bf7:  48 8B CD                 MOV RCX,RBP
1406f4bfa:  E8 E1 E1 94 FF           CALL 0x140042de0
1406f4bff:  48 8B 87 40 02 00 00     MOV RAX,qword ptr [RDI + 0x240]
1406f4c06:  F3 0F 10 33              MOVSS XMM6,dword ptr [RBX]
1406f4c0a:  48 85 C0                 TEST RAX,RAX
1406f4c0d:  74 66                    JZ 0x1406f4c75
1406f4c0f:  83 BF 48 02 00 00 00     CMP dword ptr [RDI + 0x248],0x0
1406f4c16:  75 5D                    JNZ 0x1406f4c75
1406f4c18:  48 8B 80 18 03 00 00     MOV RAX,qword ptr [RAX + 0x318]
1406f4c1f:  48 85 C0                 TEST RAX,RAX
1406f4c22:  74 51                    JZ 0x1406f4c75
1406f4c24:  F3 0F 10 80 7C 07 00 00  MOVSS XMM0,dword ptr [RAX + 0x77c]
1406f4c2c:  0F 57 C9                 XORPS XMM1,XMM1
1406f4c2f:  0F 2E C1                 UCOMISS XMM0,XMM1
1406f4c32:  7A 30                    JP 0x1406f4c64
1406f4c34:  75 2E                    JNZ 0x1406f4c64
1406f4c36:  F3 0F 10 80 80 07 00 00  MOVSS XMM0,dword ptr [RAX + 0x780]
1406f4c3e:  0F 2E C1                 UCOMISS XMM0,XMM1
1406f4c41:  7A 21                    JP 0x1406f4c64
1406f4c43:  75 1F                    JNZ 0x1406f4c64
1406f4c45:  48 63 88 60 03 00 00     MOVSXD RCX,dword ptr [RAX + 0x360]
1406f4c4c:  83 F9 23                 CMP ECX,0x23
1406f4c4f:  77 24                    JA 0x1406f4c75
1406f4c51:  48 8B C1                 MOV RAX,RCX
1406f4c54:  48 B9 00 00 08 80 09 00 00 00 MOV RCX,0x980080000
1406f4c5e:  48 0F A3 C1              BT RCX,RAX
1406f4c62:  73 11                    JNC 0x1406f4c75
1406f4c64:  4C 8B CB                 MOV R9,RBX
1406f4c67:  4D 8B C6                 MOV R8,R14
1406f4c6a:  48 8B D7                 MOV RDX,RDI
1406f4c6d:  48 8B CE                 MOV RCX,RSI
1406f4c70:  E8 BB DA 00 00           CALL 0x140702730
1406f4c75:  F3 0F 10 03              MOVSS XMM0,dword ptr [RBX]
1406f4c79:  0F 2E C6                 UCOMISS XMM0,XMM6
1406f4c7c:  7A 02                    JP 0x1406f4c80
1406f4c7e:  74 0B                    JZ 0x1406f4c8b
1406f4c80:  48 8B D3                 MOV RDX,RBX
1406f4c83:  48 8B CD                 MOV RCX,RBP
1406f4c86:  E8 55 E1 94 FF           CALL 0x140042de0
1406f4c8b:  8B 97 48 02 00 00        MOV EDX,dword ptr [RDI + 0x248]
1406f4c91:  B1 01                    MOV CL,0x1
1406f4c93:  F3 0F 10 33              MOVSS XMM6,dword ptr [RBX]
1406f4c97:  85 D2                    TEST EDX,EDX
1406f4c99:  75 23                    JNZ 0x1406f4cbe
1406f4c9b:  48 8B 87 40 02 00 00     MOV RAX,qword ptr [RDI + 0x240]
1406f4ca2:  4C 8B 80 18 03 00 00     MOV R8,qword ptr [RAX + 0x318]
1406f4ca9:  4D 85 C0                 TEST R8,R8
1406f4cac:  74 10                    JZ 0x1406f4cbe
1406f4cae:  33 C0                    XOR EAX,EAX
1406f4cb0:  0F B6 C9                 MOVZX ECX,CL
1406f4cb3:  41 83 B8 60 03 00 00 22  CMP dword ptr [R8 + 0x360],0x22
1406f4cbb:  0F 44 C8                 CMOVZ ECX,EAX
1406f4cbe:  83 BE B4 05 00 00 00     CMP dword ptr [RSI + 0x5b4],0x0
1406f4cc5:  0F 84 99 00 00 00        JZ 0x1406f4d64
1406f4ccb:  84 C9                    TEST CL,CL
1406f4ccd:  0F 84 91 00 00 00        JZ 0x1406f4d64
1406f4cd3:  85 D2                    TEST EDX,EDX
1406f4cd5:  0F 85 89 00 00 00        JNZ 0x1406f4d64
1406f4cdb:  4C 8B 97 40 02 00 00     MOV R10,qword ptr [RDI + 0x240]
1406f4ce2:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
1406f4cec:  F3 0F 10 47 28           MOVSS XMM0,dword ptr [RDI + 0x28]
1406f4cf1:  F3 0F 58 47 04           ADDSS XMM0,dword ptr [RDI + 0x4]
1406f4cf6:  49 8B 8A 78 09 00 00     MOV RCX,qword ptr [R10 + 0x978]
1406f4cfd:  49 2B 8A 70 09 00 00     SUB RCX,qword ptr [R10 + 0x970]
1406f4d04:  48 F7 E9                 IMUL RCX
1406f4d07:  F3 0F 11 44 24 60        MOVSS dword ptr [RSP + 0x60],XMM0
1406f4d0d:  48 C1 FA 06              SAR RDX,0x6
1406f4d11:  48 8B C2                 MOV RAX,RDX
1406f4d14:  48 C1 E8 3F              SHR RAX,0x3f
1406f4d18:  48 03 D0                 ADD RDX,RAX
1406f4d1b:  85 D2                    TEST EDX,EDX
1406f4d1d:  74 45                    JZ 0x1406f4d64
1406f4d1f:  4D 8B 82 70 09 00 00     MOV R8,qword ptr [R10 + 0x970]
1406f4d26:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
1406f4d30:  FF CA                    DEC EDX
1406f4d32:  8B C2                    MOV EAX,EDX
1406f4d34:  48 69 C8 E0 00 00 00     IMUL RCX,RAX,0xe0
1406f4d3b:  42 83 BC 01 90 00 00 00 0F CMP dword ptr [RCX + R8*0x1 + 0x90],0xf
1406f4d44:  74 06                    JZ 0x1406f4d4c
1406f4d46:  85 D2                    TEST EDX,EDX
1406f4d48:  75 E6                    JNZ 0x1406f4d30
1406f4d4a:  EB 18                    JMP 0x1406f4d64
1406f4d4c:  4C 8D 4C 24 60           LEA R9,[RSP + 0x60]
1406f4d51:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
1406f4d56:  4D 8B C2                 MOV R8,R10
1406f4d59:  48 8B D7                 MOV RDX,RDI
1406f4d5c:  48 8B CE                 MOV RCX,RSI
1406f4d5f:  E8 4C 00 00 00           CALL 0x1406f4db0
1406f4d64:  F3 0F 10 03              MOVSS XMM0,dword ptr [RBX]
1406f4d68:  0F 2E C6                 UCOMISS XMM0,XMM6
1406f4d6b:  7A 02                    JP 0x1406f4d6f
1406f4d6d:  74 0B                    JZ 0x1406f4d7a
1406f4d6f:  48 8B D3                 MOV RDX,RBX
1406f4d72:  48 8B CD                 MOV RCX,RBP
1406f4d75:  E8 66 E0 94 FF           CALL 0x140042de0
1406f4d7a:  83 BF 4C 02 00 00 02     CMP dword ptr [RDI + 0x24c],0x2
1406f4d81:  75 11                    JNZ 0x1406f4d94
1406f4d83:  4C 8B CF                 MOV R9,RDI
1406f4d86:  4C 8B C3                 MOV R8,RBX
1406f4d89:  49 8B D6                 MOV RDX,R14
1406f4d8c:  48 8B CE                 MOV RCX,RSI
1406f4d8f:  E8 CC DC 0A 00           CALL 0x1407a2a60
1406f4d94:  48 8B 5C 24 68           MOV RBX,qword ptr [RSP + 0x68]
1406f4d99:  48 8B 6C 24 70           MOV RBP,qword ptr [RSP + 0x70]
1406f4d9e:  0F 28 74 24 30           MOVAPS XMM6,xmmword ptr [RSP + 0x30]
1406f4da3:  48 83 C4 40              ADD RSP,0x40
1406f4da7:  41 5E                    POP R14
1406f4da9:  5F                       POP RDI
1406f4daa:  5E                       POP RSI
1406f4dab:  C3                       RET

```

---

## 3. Analysis — Evidence and Interpretation

### 3.1 Call Site

Called by FUN_140741170 at VA 0x140745060 / RVA 0x745060.  
Register setup per x64 calling convention:  
- RCX = param_1 = context  
- RDX = param_2 = window  
- R8  = param_3 = &horizontal layout value (float*)  
- R9  = param_4 = &vertical layout value (float*)  

### 3.2 Direct Evidence (verbatim from exports)

**Function signature:** oid FUN_1406f4b40(longlong param_1, longlong param_2, undefined8 param_3, float *param_4)  

**Offsets accessed on param_2 (window):**  
- param_2 + 0x580 ? lVar1, passed to FUN_140042de0  
- param_2 + 0x248 ? compared to 0 (multiple branches)  
- param_2 + 0x240 ? associated-object UNION pointer  
- param_2 + 0x24c ? compared to 2  
- param_2 + 0x28 and param_2 + 4 ? summed into local_res8[0]  

**Offsets accessed on *(param_2 + 0x240) (associated object):**  
- + 0x318 ? pointer to associated-object data  
- + 0x77c ? float compared to 0.0  
- + 0x780 ? float compared to 0.0  
- + 0x360 ? uint compared to 0x23, bit-test against 0x980080000  
- + 0x970 ? list base address  
- + 0x978 ? list end address  

**Offsets accessed on list entries (+ 0x970 + index * 0xe0):**  
- + 0x90 ? compared to 0xf  

**Callees (direct calls):**  
- FUN_140042de0(lVar1, param_4) — called 7 times, after each potential change to *param_4  
- FUN_140772400(param_1, param_3, param_4, param_2) — context, &horizontal, &vertical, window  
- FUN_1407752a0(param_1, param_3, param_4, param_2) — context, &horizontal, &vertical, window  
- FUN_140775000(param_1, param_3, param_4, param_2) — gated on window + 0x248 == 0  
- FUN_140702730(param_1, param_2, param_3, param_4) — gated on associated-object conditions  
- FUN_1406f4db0(param_1, param_2, lVar3, local_res8, param_4) — called when list entry type == 0xf  
- FUN_1407a2a60(param_1, param_3, param_4, param_2) — gated on window + 0x24c == 2  

### 3.3 Dataflow — List Iteration (disassembly lines 114–143)

`	ext
window + 0x240 ? associated object pointer (R10)
associated_object + 0x970 ? list base (R8)
associated_object + 0x978 ? list end (RCX)
count = (list_end - list_base) / 0xe0
for i from count-1 down to 0:
    entry = list_base + i * 0xe0
    if *(int *)(entry + 0x90) == 0xf:
        FUN_1406f4db0(context, window, associated_object, &sum, &vertical)
        break
``n
### 3.4 Role Assessment

**This function does not display vehicles.** It does not perform mouse hit-testing, does not check click-edge flags, and does not reference any vehicle rendering path.  

**What it does:** A layout/coordinate adjustment helper. It:  
1. Saves and restores *param_4 (vertical layout value) via FUN_140042de0 after each callee that may modify it  
2. Calls three layout-adjustment callees (FUN_140772400, FUN_1407752a0, FUN_140775000) that modify *param_4  
3. Conditionally calls FUN_140702730 when window + 0x240 != 0, window + 0x248 == 0, *window+0x240+0x318 != 0, and float/type conditions at +0x77c, +0x780, +0x360 are met  
4. Conditionally iterates a list at window + 0x240 + 0x970/+0x978 with stride 0xe0, looking for entry type  xf, then calls FUN_1406f4db0  
5. Conditionally calls FUN_1407a2a60 when window + 0x24c == 2  

### 3.5 Conclusion

**Confirmed:** FUN_1406F4B40 is a layout/coordinate adjustment helper called from FUN_140741170. It modifies vertical layout values through helper callees and conditionally dispatches to FUN_140702730, FUN_1406f4db0, or FUN_1407a2a60.  

**Unknown:** None of FUN_140042de0, FUN_140772400, FUN_1407752a0, FUN_140775000, FUN_140702730, FUN_1406f4db0, or FUN_1407a2a60 have been analyzed. No vehicle pointer origin, mouse-hit condition, click condition, or vehicle action call has been traced through this function.  

**Vehicle path:** Not found. This function does not display vehicles or handle clicks. No actionable vehicle path exists through FUN_1406F4B40 based on this export alone.  

### 3.6 Tools and Limitations

- Tools: scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21), ListFuncs.java, DecompAt.java, DisasmMany.java  
- Errors/limitations: None. Decompilation and disassembly both completed successfully.  
- FUN_140769300 is CLOSED (task percentages) and was not reinvestigated.  
- No runtime code, hooks, configuration, or billing was modified.  


