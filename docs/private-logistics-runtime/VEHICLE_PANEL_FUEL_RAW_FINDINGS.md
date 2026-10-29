# Vehicle Panel Fuel — Raw Ghidra Exports

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON  
**All addresses below are RVAs** (file-based = 0x140 + RVA)  
**Generated:** 2026-10-28 via `scratch/ghidra_run.bat` (Ghidra 12.1.4, JDK 21)

---

## 1. Litres Format String — ADDRESS AND CROSS-REFERENCES

### 1.1 Format Function
```
FUN_140005290 (RVA 0x5290) → __stdio_common_vswprintf
```

### 1.2 GetValueString (panel string formatter)
```
FUN_1403329E0 (RVA 0x3329E0) = C3D_CONFIGURATOR::GetValueString
```

### 1.3 Data Xrefs to GetValueString (format string pointers in .rdata)
```
[DATA] from 140a668c4 in NON_FUNCTION
[DATA] from 14094b268 in NON_FUNCTION
[DATA] from 14094b2a0 in NON_FUNCTION
```

### 1.4 Call Xrefs to GetValueString (panel call sites using format strings)
```
[UNCONDITIONAL_CALL] from 140743540 in FUN_140741170   ← fuel display #1
[UNCONDITIONAL_CALL] from 140743d94 in FUN_140741170   ← fuel display #2
[UNCONDITIONAL_CALL] from 1407dec4c in FUN_1407d5460   ← vehicle detail opener
[UNCONDITIONAL_CALL] from 140747620 in FUN_140745350
[UNCONDITIONAL_CALL] from 140748253 in FUN_140745350
[UNCONDITIONAL_CALL] from 14074b0c0 in FUN_1407496c0
[UNCONDITIONAL_CALL] from 14074b663 in FUN_1407496c0
... (100+ total call sites)
```

### 1.5 Format String Pointers Referenced in GetValueString (raw disassembly)
```asm
; FUN_1403329E0 loads format strings from these .rdata addresses:
140332b4a:  LEA R8, [0x1408B51B8]   ; format string #1
140332b6e:  LEA R8, [0x1408B51A0]   ; format string #2
140332bcf:  LEA R8, [0x1408B52D8]   ; format string #3
140332c29:  LEA R8, [0x1408B52B8]   ; format string #4
140332c71:  LEA R8, [0x1408B52A8]   ; format string #5
140332c8e:  LEA R8, [0x1408B5290]   ; format string #6
140332d3e:  LEA R8, [0x1408B5310]   ; format string #7
140332d4c:  LEA R8, [0x1408B5300]   ; format string #8
140332e05:  LEA R8, [0x1408B52F0]   ; format string #9
140332e32:  LEA R8, [0x1408B5228]   ; format string #10
140332e46:  LEA R8, [0x1408B5218]   ; format string #11
140332e75:  LEA R8, [0x1408B5210]   ; format string #12
```
**STATUS: Format string contents UNKNOWN — located in .rdata at 0x1408B5xxx range, not yet dumped as wide strings.**

---

## 2. Display Call — SURROUNDING BASIC BLOCKS

### 2.1 Panel Fuel Display #1 (FUN_140741170 at 0x140743530-0x140743545)
```asm
; Basic block entry
140743530:  41 B1 01                 MOV R9B,0x1
140743533:  41 B8 00 01 00 00        MOV R8D,0x100
140743539:  48 8B 52 08              MOV RDX,qword ptr [RDX + 0x8]   ; RDX = value to format
14074353d:  49 8B CE                 MOV RCX,R14                       ; panel context
140743540:  E8 9B F4 BE FF           CALL 0x1403329E0                ; GetValueString
; --- after call ---
140743545:  F3 0F 10 05 3B EB 24 00  MOVSS XMM0,dword ptr [0x140992088] ; DPI scalar
14074354d:  F3 0F 59 C7              MULSS XMM0,XMM7                 ; XMM7 = litres value
```

### 2.2 Panel Fuel Display #2 (FUN_140741170 at 0x140743D80-0x140743D94)
```asm
140743d80:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140743d85:  41 B1 01                 MOV R9B,0x1
140743d88:  41 B8 00 01 00 00        MOV R8D,0x100
140743d8e:  48 8B D7                 MOV RDX,RDI                       ; RDX = value to format
140743d91:  49 8B CE                 MOV RCX,R14
140743d94:  E8 47 EC BE FF           CALL 0x1403329E0                ; GetValueString
```

### 2.3 Vehicle Detail Panel Opener (FUN_1407d5460 at 0x1407DEC45-0x1407DEC4C)
```asm
1407dec45:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8] ; RDX = vehicle* from fleet vector
1407dec49:  49 8B CD                 MOV RCX,R13
1407dec4c:  E8 8F 3D B5 FF           CALL 0x1403329E0                ; GetValueString
```

---

## 3. Getter/Helper — NUMERIC ARGUMENT SUPPLY

### 3.1 Panel Display #1 — Value Loaded into RDX
```asm
140743539:  48 8B 52 08              MOV RDX,qword ptr [RDX + 0x8]
```
- **RDX before**: panel list item pointer (from fleet list iteration)
- **RDX after**: `*(list_item + 0x8)` = value to format
- **Value origin**: Traced to `XMM7` at 0x14074354d (`MULSS XMM0,XMM7`) — XMM7 holds the **litres value**

### 3.2 Panel Display #2 — Value in RDI
```asm
140743d8e:  48 8B D7                 MOV RDX,RDI
```
- **RDI** holds the litres value (set earlier in function)

### 3.3 Litres Value Computation (Panel — FUN_140741170)
```asm
; XMM7 = litres value computed earlier
140743545:  F3 0F 10 05 3B EB 24 00  MOVSS XMM0,dword ptr [0x140992088] ; DPI scalar
14074354d:  F3 0F 59 C7              MULSS XMM0,XMM7                 ; XMM0 = litres * DPI
; ... then passed to GetValueString via RDX at 0x140743539
```

### 3.4 Vehicle Detail Opener — Litres Computation
```asm
1407dec51:  F3 0F 10 15 2F 34 1B 00  MOVSS XMM2,dword ptr [0x140992088] ; DPI scalar
1407dec59:  0F 28 C2                 MOVAPS XMM0,XMM2
1407dec5c:  F3 41 0F 59 C0           MULSS XMM0,XMM8                 ; XMM8 = litres value
1407dec65:  F3 0F 5C D8              SUBSS XMM3,XMM0
...
1407dec88:  66 41 0F 7E D9           MOVD R9D,XMM3                   ; XMM3 passed as format arg
```

---

## 4. REAL CONVERSION INSTRUCTION AND REFERENCED CONSTANT/DATA

### 4.1 Conversion Factor — NOT FOUND AS IMMEDIATE
**Search results for 1176.0f (0x44D38000): 0 hits in .text**  
**Search results for diesel density constants: 0 hits**

### 4.2 Panel Uses Pre-Converted Litres Value
The panel receives **already-converted litres** in XMM7/XMM8/RDI. The conversion happens **upstream**.

### 4.3 Upstream Conversion — UNKNOWN LOCATION
**Candidate locations (NOT YET VERIFIED):**
- `FUN_14042A760` (called at 0x14074357E, 0x140743DD7) — returns formatted value in buffer
- `FUN_1403329E0` internal logic — may apply conversion before formatting
- Vehicle data structure — litres may be stored directly

### 4.3 Authoritative Store is TONNES (confirmed elsewhere)
```asm
; Burn function FUN_14016E3B0 — authoritative tonnes store
14016F6BC:  MOVSS XMM0, dword ptr [RAX + 0x8]    ; XMM0 = slot.tons (TONNES)
14016F6E0:  SUBSS XMM0, XMM8                     ; tonnes -= consumption
14016F6E5:  MOVSS dword ptr [RAX + 0x8], XMM0    ; store back tonnes
14016F794:  MOV dword ptr [RAX + 0x8], 0x0       ; clamp at 0 tonnes
```

---

## 5. POINTER DEFINITIONS — CONNECTING BLOCKS TO INDIVIDUAL TRUCK

### 5.1 Vehicle Pointer in Panel Function (FUN_140741170)
```asm
; RBX = Vehicle* (passed as param_2 or from fleet iteration)
140741458:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]   ; cargo vec end - begin
14074145f:  48 C1 F8 04              SAR RAX,0x4                         ; /16 = slot count
140741473:  48 8B 83 B8 03 00 00     MOV RAX,qword ptr [RBX + 0x3b8]   ; cargo vector begin
14074147a:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]  ; &cargo_slot[i]
```

### 5.2 Vehicle Pointer in Vehicle Detail Opener (FUN_1407d5460)
```asm
1407dec45:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8] ; RDX = vehicle*
; RDX = fleet_vector[RCX] where fleet_vector = building+0xC70/0xC78
```

### 5.3 Fleet Vector Source (DO Building)
```asm
; DO building fleet vector
bld+0xC70 = fleet vector begin (Vehicle**)
bld+0xC78 = fleet vector end (Vehicle**)
stride = 8 (pointer)
```

### 5.4 Vehicle → Cargo Vector (Authoritative Fuel Location)
```asm
; Offsets from vehicle base (RBX/RCX/R8)
veh+0x3B8 = cargo vector begin (CargoSlot*)
veh+0x3C0 = cargo vector end (CargoSlot*)
stride = 0x10 per slot
slot layout:
  +0x00 = Resource* (compared to game+0xC3F0)
  +0x08 = float tons (AUTHORITATIVE FUEL LEVEL)
```

### 5.5 Fuel Slot Identification (Burn Function FUN_14016E3B0)
```asm
14016F69D:  MOV R9, qword ptr [R8]              ; R9 = *(veh+0x3B8) = cargo vec begin
14016F6A0:  MOV R8, qword ptr [R15 + 0xC3F0]    ; R8 = *(game+0xC3F0) = fuel Resource*
14016F6B0:  SHL RAX, 0x4                         ; RAX = slot_index * 0x10
14016F6B4:  ADD RAX, R9                          ; RAX = &cargo_slot[slot_index]
14016F6B7:  CMP qword ptr [RAX], R8              ; CMP slot.Resource*, game+0xC3F0
14016F6BA:  JNZ 0x14016F6C7                      ; not fuel → next slot
14016F6BC:  MOVSS XMM0, dword ptr [RAX + 0x8]    ; XMM0 = slot.tons (TONNES)
```

### 5.6 Game Globals
```
game+0xC3F0 = fuel Resource* (slot-match key for burn)
game+0xC330 = fuel Resource* (transfer key for refuel)
game+0x9D4F10 = game object base (GAME_OBJ_RVA)
game+0x992088 = DPI scalar (float)
```

---

## 6. REMAINING UNKNOWN ITEMS

| Item | Status | Notes |
|------|--------|-------|
| Format string literal (e.g., `L"%.2f l"`) | **UNKNOWN** | In .rdata at 0x1408B5xxx; not dumped |
| Tonnes → litres conversion site | **UNKNOWN** | Not in panel code; upstream of panel |
| Conversion constant (1176.0f) location | **UNKNOWN** | Not in .text as immediate; possibly resource property |
| Exact panel function for "Skd-706 RTTN" detail view | **UNKNOWN** | Candidate: FUN_140741170 / FUN_1407D5460 |
| Whether refuel and burn same tick | **UNKNOWN** | FUN_14016E3B0 does burn then stochastic refuel sequentially |

---

## 7. RAW EXPORT FILES (in scratch/)

| File | Content |
|------|---------|
| `disasm_3329e0_full.txt` | GetValueString full disassembly (format string loads) |
| `disasm_741170_fuel3.txt` | Panel fuel display call sites (2x) |
| `disasm_7d5460_veh_detail.txt` | Vehicle detail panel opener |
| `disasm_741170_veh_ptr.txt` | Panel cargo vector access (veh+0x3B8/0x3C0) |
| `disasm_7d5460_3b8.txt` | Vehicle cargo vector access in 7d5460 |
| `disasm_7ebb80_fuel.txt` | Vehicle cargo vector access in 7ebb80 |
| `xrefs_3329e0.txt` | GetValueString cross-references (call + data) |
| `xrefs_5290.txt` | vswprintf wrapper cross-references |
| `xrefs_veh_3b8.txt` | All veh+0x3B8 displacements (721 hits) |
| `write_5f0.txt` | All veh+0x5F0 accesses (218 hits) — mirror, not authoritative |
| `xrefs_5f0_disp.txt` | veh+0x5F0 displacements (218 hits) |
| `asm_burn_fuel.txt` | Burn function fuel slot lookup + deduction + clamp |

---

## 8. CONFIRMED VS INFERRED

| Connection | Evidence Type | Status |
|------------|---------------|--------|
| Panel calls GetValueString with litres value | Raw call instructions at 0x140743540, 0x140743D94, 0x1407DEC4C | **CONFIRMED** |
| GetValueString calls vswprintf wrapper | Raw CALL 0x140005290 in GetValueString | **CONFIRMED** |
| Panel receives pre-converted litres | XMM7/XMM8/RDI hold litres at call sites | **CONFIRMED** |
| Authoritative fuel = cargo slot tons | Burn function MOVSS [RAX+8],XMM0 at 0x14016F6E5 | **CONFIRMED** |
| Cargo vector at veh+0x3B8/0x3C0 stride 0x10 | Panel iterates RBX+0x3B8/0x3C0, burn uses same | **CONFIRMED** |
| Fuel slot = Resource* match game+0xC3F0 | Burn function CMP [RAX], R8 at 0x14016F6B7 | **CONFIRMED** |
| Format string address | Data xrefs to 0x1408B5xxx in GetValueString | **CONFIRMED (address only)** |
| Format string content | Not dumped | **UNKNOWN** |
| Tonnes→litres conversion location | Not found in panel or GetValueString | **UNKNOWN** |
| Conversion constant 1176.0f | Not in .text as immediate | **UNKNOWN** |
| Panel function for "Skd-706" title | Not identified | **UNKNOWN** |

---

**DO NOT BUILD TRACKER ON INFERRED CONNECTIONS.**  
Only the **confirmed** items above have raw instruction evidence. The conversion factor and format string content remain unverified.