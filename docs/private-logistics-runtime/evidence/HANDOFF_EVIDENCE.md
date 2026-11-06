# HANDOFF_EVIDENCE.md
## Road DO Truck Observation Chain — Final Machine Evidence

**Branch:** DO-fuel-cost-calculatotr | **Commit:** ede8985 | **Date:** 2026-10-24
**Target:** SOVIET64.exe v1.1.1.9, base 0x140000000, ASLR ON. All addresses RVAs.

---

## 1. Pointer Provenance at Fuel Deduction Point

### 1.1 Register State at Burn Subtraction (`0x14016F6E0`) and Clamp (`0x14016F794`)

From `scratch/asm_burn_vehicle_base.txt` and `scratch/asm_burn_full.txt`:

| Instruction | Address | Register | Value | Meaning |
|-------------|---------|----------|-------|---------|
| `MOV R8, [RAX + RCX*8]` | `0x14016F67A` | `R8` | `*(order_entry+0x38)[inner]` | **Cargo vector begin** = `veh+0x3B8` |
| `MOV R9, [R8]` | `0x14016F69D` | `R9` | `*(veh+0x3B8)` | Cargo vector begin (copy) |
| `SHL RAX, 4` / `ADD RAX, R9` | `0x14016F6B0/6B4` | `RAX` | `veh+0x3B8 + slot_index*0x10` | **Fuel slot address** (interior pointer) |
| `MOVSS XMM0, [RAX+8]` | `0x14016F6BC` | `XMM0` | `slot.tons` | Authoritative fuel tonnes |
| `SUBSS XMM0, XMM8` | `0x14016F6E0` | `XMM0` | `tons - consumption` | Burn deduction |
| `MOVSS [RAX+8], XMM0` | `0x14016F6E5` | `[RAX+8]` | `slot.tons` | Store new tonnes |
| `MOV [RAX+8], 0` | `0x14016F794` | `[RAX+8]` | `0` | Empty-tank clamp |

### 1.2 Vehicle Base vs. Cargo Vector vs. Slot Array

| Entity | Address Expression | At Deduction Point |
|--------|-------------------|-------------------|
| **Vehicle base** (`veh*`) | `*(bld+0xC70)[idx]` | **NOT in any register** — only available earlier at `0x14016F66B-67A` via fleet vector |
| **Cargo vector member** | `veh+0x3B8` | `R8` = `veh+0x3B8` (loaded from order entry) |
| **Slot array begin** | `*(veh+0x3B8)` | `R9` = allocated slot-array begin pointer |
| **Individual fuel slot** | `*(veh+0x3B8) + slot_index*0x10` | `RAX` = interior pointer to `slot+0x08` |

**Provenance chain:**
```
DO building (param_2 = RBX)
  → bld+0xC70 (fleet vector)
  → vehicle* = *(bld+0xC70)[lVar20]  (at 0x14016F514, RCX = vehicle*)
  → vehicle+0x3B8 = cargo vector begin  (at 0x14016F67A, R8 = veh+0x3B8)
  → *(veh+0x3B8) = slot array begin     (at 0x14016F69D, R9)
  → slot_index*0x10 + R9 = slot address (at 0x14016F6B4, RAX)
  → slot+0x08 = authoritative fuel tonnes
```

**At the exact deduction instructions (`0x14016F6E0/6E5/794`):**
- Vehicle base (`veh*`) is **not live** in any register or stack slot
- Only `RAX` (fuel slot address) and `XMM0/XMM8` (tonnes/consumption) are live
- Vehicle identity must be reconstructed by walking back: `RAX → slot → veh+0x3B8 → order_entry[0x38][inner] → vehicle*`

---

## 2. State Gate — Corrected Interpretation

### 2.1 Actual Instructions (`scratch/asm_state_gate.txt`, `0x14016F518-0x14016F53F`)

```
14016f518:  MOVSXD R10, dword ptr [RCX + 0x90]   ; R10 = state (int)
14016f51f:  LEA EAX, [R10 - 1]                   ; EAX = state - 1
14016f523:  TEST EAX, 0xFFFFFFEE                 ; (state-1) & 0xFFFFFFEE == 0 ?
14016f528:  JNZ 0x14016F632                      ; SKIP if non-zero
14016f52e:  CMP R10D, 0x12                       ; state == 18 ?
14016f532:  JZ 0x14016F632                       ; SKIP if state == 18
14016f538:  CMP dword ptr [RCX + 0x88], 9        ; mode == 9 ?
14016f53f:  JNZ 0x14016F595                      ; SKIP if mode != 9
```

### 2.2 Mask Logic: `(state - 1) & 0xFFFFFFEE == 0`

| state | state-1 | (state-1) & 0xFFFFFFEE | Pass? |
|-------|---------|------------------------|-------|
| 1     | 0       | 0                      | ✓     |
| 2     | 1       | 0                      | ✓     |
| 3     | 2       | 2                      | ✗     |
| ...   | ...     | ...                    | ✗     |
| 17    | 16      | 16                     | ✗     |
| 18    | 17      | 16                     | ✗ (also explicit CMP) |
| 19+   | ≥18     | ≠0                     | ✗     |

**Result:** State ∈ **{1, 2} only**. State 17 is excluded by the mask (16 & 0xFFFFFFEE = 16 ≠ 0). The explicit `CMP R10D, 0x12` excludes state 18 redundantly.

**Mode gate:** `veh+0x88 == 9` (mode 9 required).

---

## 3. Cargo Boundaries — Verified and Unresolved

### 3.1 Verified: Pickup (Vehicle Cargo Increase)
**Call site:** `0x694F40` (in `FUN_14068B330`) — mode 0, tons < 0
**Paired vehicle-side:** `0x6953C4` — mode 1, tons > 0
**Arguments at `0x6953C4` (`scratch/asm_6953c4.asm`):**
- `RDX` = `[R14+0x390]` (vehicle)
- `R8` = 1 (mode 1)
- `XMM6` = positive tons
- `R9` = resource
- **Mutation:** `vehicle_cargo_slot += tons` (cargo increases)

### 3.2 Verified: Delivery Building-Side (Building Receives)
**Call site:** `0x1B2898` (in `FUN_1401B1290`) — mode 1, tons > 0
**Arguments (`scratch/asm_road_delivery_full.txt`):**
- `RDX` = `RDI` = vehicle
- `R8` = 1 (mode 1)
- `XMM6` = positive tons (clamped delivery amount)
- `R9` = `R12` = resource
- **Mutation:** `vehicle_cargo_slot += tons` (cargo **increases**)
- **Post-call:** Building accounting at `[RSI+0xE90]` manual, not via transfer

### 3.3 Unresolved: Vehicle-Side Delivery (Vehicle Cargo Decrease)
**Required:** mode 1, tons < 0 transfer for road truck unloading
**Searched:** Full `FUN_1401B1290` (0x1401B1290–0x1401B2900) — **only one transfer call at `0x1B2898`**
**Repaired report 16 call sites:** Road truck vehicle-side delivery should be among `0x1B484B`, `0x1B5EC0`, `0x1B653B`, `0x1B6833`, `0x1B75ED`, `0x1B7981` — **function not identified**
**Sea/rail equivalent:** `0x68F7F9` in `FUN_14068B330` (mode 1, -tons)

**BLOCKED:** Road truck cargo-decrease transfer site **not located**. Cannot verify complete unloading boundary for road DO truck.

---

## 4. Fuel Observation Mechanism

### 4.1 Burn Function Entry — Inline Hook Viable
`FUN_14016E3B0` prologue (`scratch/asm_burn_entry.txt`, `0x14016E3B0-0x14016E41F`):
```
14016e3b0:  MOV RAX, RSP
14016e3b3:  MOVSS [RAX+18], XMM2
14016e3b8:  PUSH RBP
14016e3b9:  PUSH RSI
14016e3ba:  PUSH RDI
14016e3bb:  PUSH R12
14016e3bd:  PUSH R13
14016e3bf:  PUSH R14
14016e3c1:  PUSH R15
14016e3c3:  LEA RBP, [RAX-0xF8]
14016e3ca:  SUB RSP, 0x1C0
... (XMM save area)
```
**Prologue length: ~0x70 bytes (>14)** — **inline hook at entry IS viable**.

### 4.2 Mid-Function Instrumentation — NOT Supported by Loader
The deduction points (`0x14016F6E0/6E5/794`) are **mid-function**. The repository's hook API (`installInlineHook`, `patchIAT`, call-site patching) supports:
- Function-entry inline hooks (≥14 stolen bytes)
- Call-site patching (16 transfer sites, all `CALL rel32`)
- **No API for mid-function code caves or instruction relocation**

### 4.3 Concrete Observation Design Options

| Option | Mechanism | Vehicle Attribution | Status |
|--------|-----------|---------------------|--------|
| **A. Inline hook at `0x16E3B0` entry** | Hook function entry, save vehicle* from `RCX` (fleet iteration), instrument deduction sites via single-step or shadow stack | Vehicle* available at entry (`RCX` from fleet vector) | **Viable** but requires mid-function instrumentation not in API |
| **B. Call-site patch at transfer `0x420350`** | Patch 16 transfer sites (already done by `private_logistics`) | Vehicle in `RDX` at mode 1 | **Cannot observe burn** — burn doesn't call transfer |
| **C. Polling via sim_clock daily callback** | Read `veh+0x3B8` fuel slot daily, delta = burned | Per-vehicle, via `bld+0xC70` fleet walk | **Works** but coarse (daily, not per-trip) |
| **D. Per-delivery fuel delta (BurnProbe)** | At delivery (`0x1B2898`), read fuel slot, delta from last delivery | Per-vehicle, via `RDX` = vehicle | **Implemented in `private_logistics`** but conflates trips, invalidated by refills/fleet changes |

### 4.4 Requirements for Per-Trip Burn Observation
To observe **actual deductions on both paths** for one road truck:
1. **Hook `FUN_14016E3B0` at entry** (inline, ≥14 bytes available)
2. **Preserve vehicle* across iteration** — at entry, `RCX` = vehicle* (from fleet vector `bld+0xC70[idx]`)
3. **Instrument two deduction sites:**
   - `0x14016F6E5` (normal store): capture `RAX+8` before/after, or `XMM0` (new) and `XMM8` (consumption)
   - `0x14016F794` (clamp store): capture `RAX+8` before (was ≤ consumption), record actual = old tonnes
4. **Machine state preservation:** Save/restore all volatile registers (RAX, RCX, RDX, R8-R11, XMM0-XMM5) per x64 ABI
5. **Vehicle attribution:** At hook entry, `RCX` = current vehicle*. Store in thread-local or per-vehicle array indexed by vehicle*.

**BLOCKED:** No loader API for mid-function instrumentation. Inline hook at entry only gives access to function start, not the specific deduction instructions 0x310 bytes later.

---

## 5. Summary: What Is Verified vs. BLOCKED

| Observation | Status | Evidence |
|-------------|--------|----------|
| **DO membership** (type 43, fleet at `bld+0xC70`) | ✅ Verified | 3 independent readers (`AGENT_REPORT.md:164-174`) |
| **Vehicle kind** (`veh+0x50C == 0x42`) | ✅ Verified | `asm_burn_fuel.txt:446`, `private_logistics.cpp:3376` |
| **State gate** (`veh+0x90 ∈ {1,2}`, `veh+0x88 == 9`) | ✅ Verified | `asm_state_gate.txt:4-11` |
| **Authoritative fuel store** (`veh+0x3B8` slot matching `game+0xC3F0`, `slot+0x08`) | ✅ Verified | `asm_burn_fuel.txt:75-83` |
| **Burn deduction** (`SUBSS [slot+8], consumption`) | ✅ Verified | `asm_burn_fuel.txt:91-94` |
| **Empty-tank clamp** (`MOV [slot+8], 0`) | ✅ Verified | `asm_burn_fuel.txt:126-127` |
| **Pickup complete** (vehicle cargo increase at `0x6953C4`) | ✅ Verified | `asm_6953c4.asm`, paired with `0x694F40` |
| **Delivery building-side** (building accounting at `0x1B2898`) | ✅ Verified | `asm_road_delivery_full.txt` |
| **Vehicle base at deduction point** | ❌ BLOCKED | Not in register at `0x14016F6E0/794` |
| **Per-trip burn observation (both paths)** | ❌ BLOCKED | No mid-function hook API; inline hook at entry only |
| **Road truck cargo decrease (vehicle unload)** | ❌ BLOCKED | Transfer site not located in road path |
| **Complete unloading boundary verified** | ❌ BLOCKED | Requires vehicle cargo decrease observation |

---

## 6. Integrator Guidance

### Safe to Implement (with current loader):
- **Daily fuel delta per vehicle** via sim_clock callback + fleet walk (`bld+0xC70`) + `FuelSlotTons()` — coarse, not per-trip
- **Per-delivery fuel delta** via call-site patch at `0x1B2898` (vehicle in `RDX`) — conflates trips, invalidated by refills
- **Pickup verification** via call-site patch at `0x6953C4` (vehicle in `RDX`)

### Requires New Loader Capability:
- **Per-trip burn observation** → needs mid-function instrumentation API or inline hook with instruction-relocation support at `0x14016F6E5/794`
- **Complete unloading verification** → needs road truck vehicle-side delivery call site (mode 1, tons<0) located and patched

### Unresolved (No Workaround):
- Road truck vehicle-side delivery transfer site — must be found in `0x1Bxxxx` range
- Vehicle identity at exact burn deduction — requires mid-function register preservation

---

## 7. Evidence Files (Committed)

| File | Commit | Purpose |
|------|--------|---------|
| `ROAD_PATH_EVIDENCE_COMPLETE.md` | 6c276e1 | Full evidence report (superseded in part) |
| `ROAD_PATH_EVIDENCE_ADDENDUM.md` | 91c68ea | Focused corrections |
| `MACHINE_EVIDENCE_FOCUSED.md` | ede8985 | Direct machine evidence |
| `HANDOFF_EVIDENCE.md` | (this) | Final handoff |

All under `docs/private-logistics-runtime/evidence/`. No production edits.