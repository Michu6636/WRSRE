# INVESTIGATION_REPORT.md
## Road DO Truck — Missing Evidence for Diagnostic Probe

**Branch:** DO-fuel-cost-calculatotr | **Date:** 2026-10-24
**Target:** SOVIET64.exe v1.1.1.9, base 0x140000000, ASLR ON. All addresses RVAs.

---

## 1. PRIORITY 1 — Road Truck Cargo-Decrease Path (UNLOADING)

### 1.1 Sea/Rail Reference Pattern (Verified)
**Site:** `0x68F7F9` in `FUN_14068B330` (construction/demolition)
**Disassembly** (`scratch/asm_68f7f9_pre.txt`, `scratch/asm_68f7f9.txt`):
```
14068f7dc:  F3 0F 11 6C 24 20        MOVSS [RSP+0x20],XMM5     ; tons (negative)
14068f7e2:  4C 8B CF                 MOV R9,RDI                  ; resource
14068f7e5:  41 B8 01 00 00 00        MOV R8D,1                   ; mode = 1
14068f7eb:  48 8B 91 90 03 00 00     MOV RDX,[RCX+0x390]         ; vehicle = building+0x390
14068f7f2:  48 8D 0D 17 57 34 00     LEA RCX,[0x1409D4F10]       ; game
14068f7f9:  E8 52 0B D9 FF           CALL 0x140420350            ; transfer
```
**Key:** `XMM5 = -XMM6` via `XORPS XMM5,XMM11` at `0x14068F7B7`. Mode 1, negative tons → vehicle cargo DECREASES.

### 1.2 Road DO Truck Call Sites Examined (All 0x1Bxxxx Transfer Calls)

| Site RVA | Function | Mode | Tons Sign | Vehicle Source | Cargo Effect |
|----------|----------|------|-----------|----------------|--------------|
| `0x1B2898` | `FUN_1401B1290` | 1 | **+** (XMM6) | `RDI` (from fleet +0xC70) | **INCREASE** |
| `0x1B484B` | `FUN_1401B3CE0` | 1 | **+** (XMM6) | `RDI` (from fleet +0xC70) | **INCREASE** |
| `0x1B5EC0` | `FUN_1401B5420` | 0 | ± | `RBX` (building) | Building-side |
| `0x1B653B` | `FUN_1401B5420` | 0 | ± | `[RDX+R11+0x30]` | Building-side |
| `0x1B6833` | `FUN_1401B5420` | 0 | ± | `[RDX+R10+0x30]` | Building-side |
| `0x1B75ED` | `FUN_1401B6A20` | 0 | ± | `RBX` (building) | Building-side |
| `0x1B7981` | `FUN_1401B6A20` | 0 | ± | `[R9+RDI*8]` | Building-side |

**All 7 road transfer sites in 0x1Bxxxx range are either:**
- Mode 1 with **positive** tons (vehicle cargo **increases** — origin loading)
- Mode 0 (building-side)

### 1.3 Missing Road Unload Site
**No transfer call with mode=1 and negative tons exists in any road DO truck function.**

**Search performed:**
- All 16 transfer call sites enumerated via Xrefs to `0x420350`
- All 7 sites in 0x1Bxxxx range disassembled with predecessors
- All 4 road delivery functions decompiled (`0x1B1290`, `0x1B3CE0`, `0x1B5420`, `0x1B6A20`)
- No mode=1, negative-tons call found

**Hypothesis:** Road truck cargo decrease at destination may use:
1. Direct write to cargo slot (bypassing transfer function)
2. Different function not using `FUN_140420350`
3. Destination building's upkeep (not DO's code) — but would still need a transfer call, which isn't in the 16 sites

**BLOCKED:** Cannot verify complete unloading boundary for road DO truck without locating the cargo-decrease instruction.

---

## 2. PRIORITY 2 — Actual Fuel Deduction Observation

### 2.1 Vehicle Attribution at Deduction Point

**Burn Function:** `FUN_14016E3B0` (RVA `0x16E3B0`)
**Entry Signature** (from `scratch/asm_burn_entry.txt`):
```
14016e3b0:  48 89 44 24 40           MOV [RSP+0x40],RAX
14016e3b4:  4C 89 4C 24 38           MOV [RSP+0x38],R9
14016e3b9:  4C 89 44 24 30           MOV [RSP+0x30],R8
14016e3be:  48 89 54 24 28           MOV [RSP+0x28],RDX
14016e3c3:  48 89 4C 24 20           MOV [RSP+0x20],RCX
; param_1=RCX=game, param_2=RDX=building, param_3=XMM2=tick_delta
```

**Fleet Iteration** (`scratch/asm_burn_vehicle_base.txt`, `0x14016F66B-0x14016F67A`):
```
14016f66b:  49 63 C4                 MOVSXD RAX,R12D          ; R12D = vehicle index
14016f66e:  48 6B C0 68              IMUL RAX,RAX,0x68        ; order stride
14016f672:  49 63 CD                 MOVSXD RCX,R13D          ; inner index
14016f675:  4A 8B 44 00 38           MOV RAX,[RAX+R8+0x38]    ; order_entry[0x38]
14016f67a:  4C 8B 04 C8              MOV R8,[RAX+RCX*8]       ; R8 = veh+0x3B8 (cargo vector)
```

**At Deduction Point** (`scratch/asm_burn_full.txt`, `0x14016F6B0-0x14016F794`):
| Register | Value | Meaning |
|----------|-------|---------|
| `R8` | `veh+0x3B8` | Cargo vector **member** (pointer) |
| `R9` | `*(veh+0x3B8)` | **Slot array begin** (allocated buffer) |
| `RAX` | `*(veh+0x3B8) + slot_index*0x10` | **Fuel slot address** (interior) |
| `RAX+0x8` | `slot+0x08` | **Authoritative fuel tonnes** |

**Corrected slot address:** `*(vehicle + 0x3B8) + slot_index*0x10` (not `vehicle + 0x3B8 + ...`)

**Vehicle base (`veh*`) is NOT live** at `0x14016F6E0` (SUBSS) or `0x14016F794` (clamp). Only `RAX` (slot address) and `XMM0/XMM8` are live. Vehicle identity must be reconstructed from `RAX → slot → veh+0x3B8 → order_entry[0x38][inner] → vehicle*`.

### 2.2 Deduction Instructions (Exact)

| Event | Address | Bytes | Instruction | Effect |
|-------|---------|-------|-------------|--------|
| Normal burn | `0x14016F6E0` | `F3 41 0F 5C C0` | `SUBSS XMM0,XMM8` | `XMM0 = tonnes - consumption` |
| Normal store | `0x14016F6E5` | `F3 0F 11 40 08` | `MOVSS [RAX+8],XMM0` | Write new tonnes |
| Empty-tank clamp | `0x14016F794` | `C7 40 08 00 00 00 00` | `MOV [RAX+8],0` | Clamp to zero |

### 2.3 Loader Inline Hook Capability (`src/tesmioloader.cpp`)

**`InstallInlineHook`** (lines 214-270):
- Writes **14-byte absolute JMP** (`FF 25 00 00 00 00` + 8-byte address)
- **Requires ≥14 stolen bytes** (enforced at line 224-229)
- Copies stolen bytes to trampoline, appends return jump
- **No mid-function instrumentation** — only function-entry hooks
- **No instruction relocation** — stolen bytes must be position-independent

**Burn function prologue** (`0x14016E3B0-0x14016E41F`, ~0x70 bytes): **Viable for entry hook** (>14 bytes).

**Deduction sites are mid-function** (0x310 bytes from entry): **NOT supported** by current API.

### 2.4 Observation Mechanism Options

| Mechanism | Vehicle Attribution | Deduction Coverage | Loader Support |
|-----------|---------------------|-------------------|----------------|
| Entry hook at `0x16E3B0` | Vehicle* in `RCX` at entry (fleet iteration) | Requires mid-function instrumentation to reach `0x16F6E0/794` | ❌ No API |
| Call-site patch at `0x420350` | Vehicle in `RDX` at mode 1 | **Cannot observe burn** — burn doesn't call transfer | ✅ Implemented |
| Daily poll (sim_clock) | Fleet walk `bld+0xC70` | Coarse (daily), misses per-trip | ✅ Works |
| Per-delivery delta at `0x1B2898` | Vehicle in `RDX` | Conflates trips, invalidated by refills | ✅ Implemented |

**Concrete proposal:** None viable for **per-trip, both-paths** observation with current loader. Entry hook gives vehicle identity but cannot reach deduction instructions without mid-function support.

**BLOCKED:** No safe mechanism to observe both normal deduction and empty-tank clamp per-trip with vehicle attribution.

---

## 3. CORRECTED INTERPRETATIONS

### 3.1 State Mask (Corrected)
**Instructions** (`scratch/asm_state_gate.txt`, `0x14016F518-0x14016F53F`):
```
14016f518:  4C 63 91 90 00 00 00     MOVSXD R10,[RCX+0x90]   ; state
14016f51f:  41 8D 42 FF              LEA EAX,[R10-1]           ; state-1
14016f523:  A9 EE FF FF FF           TEST EAX,0xFFFFFFEE       ; (state-1) & 0xFFFFFFEE
14016f528:  0F 85 04 01 00 00        JNZ 0x14016F632           ; SKIP if non-zero
14016f52e:  41 83 FA 12              CMP R10D,0x12             ; state == 18
14016f532:  0F 84 FA 00 00 00        JZ 0x14016F632            ; SKIP if 18
14016f538:  83 B9 88 00 00 00 09     CMP [RCX+0x88],9          ; mode == 9
14016f53f:  75 54                    JNZ 0x14016F595           ; SKIP if not 9
```

**Mask logic:** `(state - 1) & 0xFFFFFFEE == 0`
- `0xFFFFFFEE = 1111...11101110` (bits 1,2,3... set)
- `state=1`: `(0) & mask = 0` ✓
- `state=2`: `(1) & mask = 0` ✓
- `state=17`: `(16=0x10) & mask = 0x10 ≠ 0` ✗ **EXCLUDED**
- `state=18`: explicit `CMP` excludes

**Valid states: {1, 2} only.** State 17 is excluded by mask (16 & 0xFFFFFFEE = 16 ≠ 0). Prior claim of {1,2,17} was incorrect.

### 3.2 Slot Address (Corrected)
**Wrong:** `vehicle + 0x3B8 + slot_index*0x10`
**Correct:** `*(vehicle + 0x3B8) + slot_index*0x10`

**Evidence** (`scratch/asm_burn_vehicle_base.txt`):
```
14016f67a:  4C 8B 04 C8              MOV R8,[RAX+RCX*8]       ; R8 = veh+0x3B8 (member)
14016f69d:  4D 8B 08                 MOV R9,[R8]              ; R9 = *(veh+0x3B8) (array base)
14016f6b0:  48 C1 E0 04              SHL RAX,0x4              ; index*0x10
14016f6b4:  49 03 C1                 ADD RAX,R9               ; RAX = array_base + index*0x10
```

`veh+0x3B8` is a **pointer member**; the slot array is at `*(veh+0x3B8)`.

### 3.3 R8 Overwritten
At `0x14016F6A0`: `MOV R8, [R15+0xC3F0]` — R8 overwritten with **fuel resource pointer** (`game+0xC3F0`). Original `R8 = veh+0x3B8` preserved in `R9`.

### 3.4 Function Entry Argument
`FUN_14016E3B0(RCX=game, RDX=building, XMM2=tick_delta)` — **not** vehicle*. Vehicle* only appears inside fleet iteration loop, not as function parameter.

---

## 4. SUMMARY OF EVIDENCE STATUS

| Link | Status | Evidence |
|------|--------|----------|
| DO membership (type 43, fleet +0xC70) | ✅ Verified | 3 readers, `asm_burn_vehicle_base.txt` |
| Vehicle kind (`veh+0x50C==0x42`) | ✅ Verified | `asm_burn_full.txt:446` |
| State gate (`veh+0x90∈{1,2}`, `veh+0x88==9`) | ✅ Verified | `asm_state_gate.txt` |
| Authoritative fuel (`*(veh+0x3B8)+idx*0x10+8` matching `game+0xC3F0`) | ✅ Verified | `asm_burn_full.txt:75-83` |
| Burn deduction (`SUBSS [slot+8], consumption`) | ✅ Verified | `asm_burn_full.txt:91-94` |
| Empty-tank clamp (`MOV [slot+8], 0`) | ✅ Verified | `asm_burn_full.txt:126-127` |
| Pickup complete (vehicle cargo increase) | ✅ Verified | `0x6953C4` mode 1, +tons |
| **Road truck cargo decrease (unload)** | ❌ **BLOCKED** | No mode=1, negative-tons transfer in 0x1Bxxxx |
| Vehicle base at deduction point | ❌ **BLOCKED** | Not live in registers at `0x16F6E0/794` |
| Per-trip burn observation (both paths) | ❌ **BLOCKED** | No mid-function hook API; entry hook can't reach deduction sites |

---

## 5. NEXT EXPERIMENTS NEEDED

### For Road Unloading (Priority 1)
1. **Search for direct cargo slot writes** in vehicle AI / destination building upkeep:
   - Pattern: `MOVSS [RAX+8], XMM0` where `RAX = *(veh+0x3B8) + idx*0x10` and resource ≠ fuel
   - Check vehicle per-frame update functions
2. **Trace vehicle arrival logic** — find function that triggers when road truck reaches destination
3. **Check destination building upkeep** for road-consumer buildings (factories, warehouses) for transfer calls

### For Fuel Observation (Priority 2)
1. **Enumerate callers of `FUN_14016E3B0`** — if few, caller patching may work
2. **Evaluate single-stepping from entry hook** — save vehicle* at entry, step to deduction sites (complex, fragile)
3. **Extend loader API** — add mid-function code cave support (requires loader modification)

---

## 6. COMMANDS EXECUTED (This Session)

```powershell
# Loader inspection
cd 'C:\WRSRE'; type src\tesmioloader.cpp

# Transfer XREFs
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat Xrefs.java scratch/xrefs_420350.txt 0x140420350'

# Road delivery functions
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1b3ce0.c 0x1401B3CE0'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1b5420.c 0x1401B5420'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1b6a20.c 0x1401B6A20'

# Road call sites
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b484b.txt 0x1401B4830:0x1401B4880'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b484b_pre.txt 0x1401B4800:0x1401B4850'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b5ec0.txt 0x1401B5EB0:0x1401B5F10'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b653b.txt 0x1401B6520:0x1401B6570'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b6833.txt 0x1401B6820:0x1401B6870'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b75ed.txt 0x1401B75D0:0x1401B7620'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1b7981.txt 0x1401B7970:0x1401B79C0'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_1cb7ff.txt 0x1401CB7E0:0x1401CB830'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_141f33.txt 0x140141F20:0x140141F70'

# Sea/rail reference
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_68f7f9.txt 0x14068F7E0:0x14068F830'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_68f7f9_pre.txt 0x14068F7A0:0x14068F800'

# Burn function details
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_burn_vehicle_base.txt 0x14016F660:0x14016F6E0'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_burn_entry.txt 0x14016E3B0:0x14016E420'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_state_gate.txt 0x14016F510:0x14016F550'

# Helper functions
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_689290.c 0x140689290'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_67cb30.c 0x14067CB30'
```

---

## 7. DELIVERABLE FILES (Committed)

- `docs/private-logistics-runtime/evidence/ROAD_PATH_EVIDENCE_COMPLETE.md` (6c276e1)
- `docs/private-logistics-runtime/evidence/ROAD_PATH_EVIDENCE_ADDENDUM.md` (91c68ea)
- `docs/private-logistics-runtime/evidence/MACHINE_EVIDENCE_FOCUSED.md` (ede8985)
- `docs/private-logistics-runtime/evidence/HANDOFF_EVIDENCE.md` (5b3ea4a)
- `docs/private-logistics-runtime/evidence/INVESTIGATION_REPORT.md` (this)