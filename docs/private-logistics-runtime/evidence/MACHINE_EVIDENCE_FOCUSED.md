# MACHINE_EVIDENCE_FOCUSED.md
## Direct Machine Evidence for Road DO Truck Observation Chain

**Target:** SOVIET64.exe v1.1.1.9, base 0x140000000, ASLR ON. All addresses RVAs.
**Date:** 2026-10-24

---

## 1. RCX/Base Provenance at Fuel Subtraction and Clamp

### 1.1 Burn Function Entry and Vehicle Base Register
From `scratch/asm_burn_full.txt` lines 429-446 (function `FUN_14016E3B0`):

```
14016f66b:  49 63 C4                 MOVSXD RAX,R12D          ; R12D = vehicle index
14016f66e:  48 6B C0 68              IMUL RAX,RAX,0x68        ; RAX = index * 0x68 (order stride)
14016f672:  49 63 CD                 MOVSXD RCX,R13D          ; R13D = inner index
14016f675:  4A 8B 44 00 38           MOV RAX,qword ptr [RAX + R8*0x1 + 0x38]  ; RAX = order_entry[0x38]
14016f67a:  4C 8B 04 C8              MOV R8,qword ptr [RAX + RCX*0x8]  ; R8 = order_entry[0x38][inner] = veh+0x3B8
```

**At line 14016f67a: `R8 = veh+0x3B8` (cargo vector begin pointer)**

### 1.2 Cargo Vector Access and Slot Iteration
```
14016f687:  33 C9                    XOR ECX,ECX              ; slot_index = 0
14016f689:  49 8B 50 08              MOV RDX,qword ptr [R8 + 0x8]  ; RDX = veh+0x3C0 (cargo vector end)
14016f68d:  49 2B 10                 SUB RDX,qword ptr [R8]        ; RDX = end - begin
14016f690:  48 C1 FA 04              SAR RDX,0x4                    ; RDX = slot count (stride 0x10)
14016f694:  48 85 D2                 TEST RDX,RDX
14016f697:  0F 84 3A 03 00 00        JZ 0x14016f9d7               ; no slots → skip
14016f69d:  4D 8B 08                 MOV R9,qword ptr [R8]        ; R9 = cargo vector begin (veh+0x3B8)
14016f6a0:  4D 8B 87 F0 C3 00 00     MOV R8,qword ptr [R15 + 0xc3f0]  ; R8 = game+0xC3F0 (fuel resource)
```

### 1.3 Fuel Slot Match and Address Calculation
```
14016f6b0:  48 C1 E0 04              SHL RAX,0x4                ; RAX = slot_index * 0x10
14016f6b4:  49 03 C1                 ADD RAX,R9                 ; RAX = R9 + slot_index*0x10 = &cargo_slot[slot_index]
14016f6b7:  4C 39 00                 CMP qword ptr [RAX],R8     ; CMP slot.Resource*, game+0xC3F0
14016f6ba:  75 0B                    JNZ 0x14016f6c7            ; not fuel → next slot
14016f6bc:  F3 0F 10 40 08           MOVSS XMM0,dword ptr [RAX + 0x8]  ; XMM0 = slot.tons
```

**At line 14016f6b4: `RAX = &cargo_slot[slot_index]` = `veh+0x3B8 + slot_index*0x10`**
**At line 14016f6bc: `RAX + 0x8` = fuel slot tonnes field (authoritative store)**

### 1.4 Burn Subtraction (Normal Path)
```
14016f6d6:  41 0F 2F C0              COMISS XMM0,XMM8           ; compare slot.tons vs consumption
14016f6da:  0F 86 B0 00 00 00        JBE 0x14016f790            ; if tons <= consumption → empty tank
14016f6e0:  F3 41 0F 5C C0           SUBSS XMM0,XMM8            ; XMM0 = tons - consumption  ← BURN
14016f6e5:  F3 0F 11 40 08           MOVSS dword ptr [RAX + 0x8],XMM0  ; store back to slot+0x08
```

### 1.5 Empty-Tank Clamp Path
```
14016f790:  44 0F 28 C0              MOVAPS XMM8,XMM0           ; XMM8 = current tons (≤ consumption)
14016f794:  C7 40 08 00 00 00 00     MOV dword ptr [RAX + 0x8],0x0  ; slot.tons = 0  ← CLAMP
```

### 1.6 Vehicle Base vs Cargo Vector Member — Separation
| Register | Value | Role |
|----------|-------|------|
| `R8` (at 14016f67a) | `veh+0x3B8` | Cargo vector **begin** pointer (member) |
| `R9` (at 14016f69d) | `veh+0x3B8` | Cargo vector **begin** pointer (copy) |
| `RAX` (at 14016f6b4) | `veh+0x3B8 + slot_index*0x10` | **Fuel slot address** (interior pointer) |
| `RAX + 0x8` | `slot+0x08` | **Fuel tonnes field** (authoritative store) |
| `RCX` (loop) | N/A | Not used as vehicle base here |

**The vehicle base pointer (`veh*`) is NOT in a register at the subtraction point.** The burn loop operates on the cargo vector (`veh+0x3B8`) directly, which was loaded from the order structure at `order+0x38[inner]`. The vehicle base (`veh*`) is only available earlier in the call chain (at `0x14016F66B-0x14016F67A` where `R8 = veh+0x3B8` is derived from order entry).

**BLOCKED:** Direct vehicle base identity at the exact subtraction/clamp instructions is not in register state — only the cargo vector member (`veh+0x3B8`) and interior fuel slot address (`RAX`) are live.

---

## 2. Burn Observation Mechanism

### 2.1 Exact Deduction Points to Observe
| Event | Address | Instruction | Mutation |
|-------|---------|-------------|----------|
| Normal burn | `0x14016F6E0` | `SUBSS XMM0,XMM8` | `slot.tons -= consumption` |
| Normal store | `0x14016F6E5` | `MOVSS [RAX+8],XMM0` | Write new tonnes |
| Empty-tank clamp | `0x14016F794` | `MOV [RAX+8],0` | `slot.tons = 0` |

### 2.2 Observation Mechanism Requirements
To record **actual deductions on both paths**, an observation mechanism must:

1. **Intercept at `0x14016F6E5` (normal store)** — Read `RAX+8` before and after, or capture `XMM0` (new tonnes) and `XMM8` (consumption)
2. **Intercept at `0x14016F794` (clamp store)** — Read `RAX+8` before (was `XMM0` ≤ `XMM8`), record actual deduction = old tonnes
3. **Correlate to vehicle** — Need `veh*` identity. At these instructions, only `RAX = &slot` is available. Vehicle base requires walking back: `slot → veh+0x3B8 → order_entry[0x38][inner] → vehicle*`

### 2.3 Concrete Mechanism (Call-Site Patch Not Applicable)
The burn is **inline in `FUN_14016E3B0`**, not a call site. A call-site patch at `0x420350` (transfer) **cannot observe burn deductions** — burn does not call transfer.

**Required:** Inline hook or code cave at `0x14016F6E5` and `0x14016F794` inside `FUN_14016E3B0`.
- Prologue of `FUN_14016E3B0` at `0x14016E3B0` — check length for inline hook viability
- If <14 bytes: must patch callers of `FUN_14016E3B0` (unknown number)

**BLOCKED:** No call-site patch available for burn observation. Inline hook requires ≥14 stolen bytes at function entry — unverified. Caller patching requires enumerating all callers of `FUN_14016E3B0`.

---

## 3. Cargo Mutation at Delivery (Mode 1)

### 3.1 Delivery Call Site — Arguments
From `scratch/asm_delivery_full.txt` lines 14-22:
```
1401b2874:  4C 89 7C 24 40           MOV [RSP+0x40],R15        ; param_9 = NULL
1401b2879:  44 88 7C 24 38           MOV [RSP+0x38],R15B       ; flag = 0
1401b287e:  4C 89 7C 24 28           MOV [RSP+0x28],R15        ; out = R15 (RSI value)
1401b2883:  F3 0F 11 74 24 20        MOVSS [RSP+0x20],XMM6     ; tons = XMM6 (>0)
1401b2889:  4D 8B CC                 MOV R9,R12                ; resource = R12
1401b288c:  41 B8 01 00 00 00        MOV R8D,1                 ; mode = 1
1401b2892:  48 8B D7                 MOV RDX,RDI               ; vehicle = RDI
1401b2895:  49 8B CE                 MOV RCX,R14               ; game = R14
1401b2898:  E8 B3 DA 26 00           CALL 0x140420350          ; transfer
```

**At call: `RDX = RDI = vehicle*` (verified: predecessor accesses `veh+0x1708`, `veh+0x3B8`, `veh+0x3C0`)**

### 3.2 Transfer Function Mode 1 Mutation (from repaired report Section 5.7)
`FUN_140420350` mode 1 iterates vehicle cargo vector at `param_2 + 0x3B8` to `param_2 + 0x3C0`:
```
For each slot (stride 0x10):
  slot.Resource* = *(slot + 0x00)
  slot.tons = *(slot + 0x08)
  if (slot.Resource* == param_4) {  // resource match
      pfVar18 = slot + 0x08        // mutable tonnes field
      // ... capacity check ...
      *pfVar18 = *pfVar18 + param_5  // param_5 = positive tons (delivery)
      if (exceeds capacity) clamp
  }
```

**Mutation direction:** `param_5 > 0` (positive) → `*pfVar18 = *pfVar18 + param_5` → **cargo INCREASES**

### 3.3 Reconciliation: Mode 1 Positive Tons = Cargo Increases
| Parameter | Value | Meaning |
|-----------|-------|---------|
| `mode` (R8) | `1` | Vehicle cargo slot |
| `tons` ([RSP+0x20]) | `XMM6 > 0` | Positive = **add to vehicle** |
| Mutation | `slot.tons += tons` | **Cargo increases** (vehicle receives goods) |

**My prior claim "cargo decreasing" was incorrect.** Mode 1 with positive tons **increases** vehicle cargo (delivery INTO vehicle). The goods leave the destination building via a separate mode 0 call.

### 3.4 Complete Unloading Boundary — Two Transfers
From repaired report Section 9.5 (paired calls):
```
0x68F792  mode 0  +tons   BUILDING receives goods (destination storage)
0x68F7F9  mode 1  -tons   VEHICLE loses goods (vehicle cargo decreases)
```

**Complete unloading requires observing BOTH:**
1. Mode 0 at destination building (`0x68F792` or similar) — building storage increases
2. Mode 1 at vehicle (`0x1B2898` for road) — vehicle cargo decreases (param_5 < 0)

**At `0x1B2898`: mode=1, tons>0 → vehicle cargo INCREASES.** This is NOT the unload-from-vehicle transfer. The unload-from-vehicle is a different call site with mode=1, tons<0.

**BLOCKED:** `0x1B2898` is NOT the vehicle unload transfer. The vehicle unload (cargo decrease) occurs at a different call site (likely `0x68F7F9` family). Road truck unload site not yet identified.

---

## 4. State-Mask Interpretation Correction

### 4.1 Actual Branch Conditions (from `scratch/asm_burn_full.txt` lines 429-438)
```
14016f518:  4C 63 91 90 00 00 00     MOVSXD R10,dword ptr [RCX + 0x90]  ; RCX = vehicle*
14016f51f:  41 8D 42 FF              LEA EAX,[R10 - 1]
14016f523:  A9 EE FF FF FF           TEST EAX,0xFFFFFFEE
14016f528:  0F 85 04 01 00 00        JNZ 0x14016f632              ; SKIP if (state-1) & 0xFFFFFFEE != 0
14016f52e:  41 83 FA 12              CMP R10D,0x12
14016f532:  0F 84 FA 00 00 00        JZ 0x14016f632               ; SKIP if state == 0x12
14016f538:  83 B9 88 00 00 00 09     CMP dword ptr [RCX + 0x88],0x9
14016f53f:  75 54                    JNZ 0x14016f595              ; SKIP if mode != 9
```

### 4.2 Corrected State Gate
| Offset | Field | Condition | Meaning |
|--------|-------|-----------|---------|
| `veh+0x90` | state (int) | `(state - 1) & 0xFFFFFFEE == 0` | **state ∈ {1, 2}** |
| `veh+0x90` | state (int) | `state != 0x12` | **state ≠ 18** (explicit exclusion) |
| `veh+0x88` | mode (int) | `mode == 9` | **mode == 9** |

**The mask `0xFFFFFFEE` tests that bits 1,2,3... are clear after subtracting 1.**
- `state=1`: `(1-1)=0` → `0 & 0xFFFFFFEE = 0` ✓
- `state=2`: `(2-1)=1` → `1 & 0xFFFFFFEE = 0` ✓
- `state=3`: `(3-1)=2` → `2 & 0xFFFFFFEE = 2` ✗
- `state=0x12 (18)`: explicitly excluded by `CMP R10D,0x12`

**Corrected:** State must be **1 or 2** (working states), **not 18**, and mode must be **9**.

---

## 5. Summary of BLOCKED Observations

| # | Observation | Status | Blocker |
|---|-------------|--------|---------|
| 1 | Vehicle base identity at burn subtraction/clamp | **BLOCKED** | Only cargo vector member (`veh+0x3B8`) and interior slot address (`RAX`) live at deduction point; vehicle base not in register |
| 2 | Burn deduction observation (both paths) | **BLOCKED** | Burn is inline in `FUN_14016E3B0`, not a call site; requires inline hook or caller patching (unverified) |
| 3 | Complete unloading (vehicle cargo decrease) | **BLOCKED** | `0x1B2898` is mode 1, tons>0 → cargo **increases**; vehicle unload (tons<0) at different unidentified call site |
| 4 | State gate interpretation | **RESOLVED** | State ∈ {1,2}, ≠ 18; mode == 9 (corrected from prior mask claim) |

---

## 6. Minimal Verified Evidence Chain (What Is Established)

| Link | Evidence |
|------|----------|
| DO → vehicle | `bld+0xC70` vector → `vehicle*` → `veh+0x4F8 == bld` |
| Vehicle kind | `veh+0x50C == 0x42` (road truck) |
| Vehicle state gate | `veh+0x90 ∈ {1,2}`, `veh+0x90 ≠ 0x12`, `veh+0x88 == 9` |
| Authoritative fuel | Cargo slot at `veh+0x3B8` matching `game+0xC3F0`, field `slot+0x08` |
| Burn deduction | `SUBSS [slot+8], consumption` at `0x14016F6E0`; clamp `MOV [slot+8],0` at `0x14016F794` |
| Pickup (vehicle cargo increase) | `0x694F40`: mode 0, tons<0 → building loses; paired mode 1 at `0x6953C4`: tons>0 → vehicle gains |
| Delivery (building receives) | Mode 0 at destination building (site TBD) — NOT `0x1B2898` |
| Vehicle unload (cargo decrease) | Mode 1, tons<0 — site NOT `0x1B2898` (that one increases cargo) |