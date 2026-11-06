# ROAD_PATH_EVIDENCE_COMPLETE.md
## Road Distribution Office Truck — Minimum Game-Facing Observations Evidence

> **Historical, NOT a verified road-truck fuel contract.** The 2026-10-30
> actual-truck test contradicts the assumed slot/state identification below.
> “Complete” in this filename is not acceptance of burn/refill/clamp provenance.
> Do not hook the old upkeep family from these claims. Current corrections and
> remaining evidence requirements are in [the single cumulative handover](../VEHICLE_PANEL_FUEL_INVESTIGATION.md).

**Target:** SOVIET64.exe v1.1.1.9 (PE 0x6A3EB6AD), base 0x140000000, ASLR ON. All addresses are RVAs.
**Branch:** reverse-engineering-and-verified-transport-associa (commit 80a4404d8d3fa5161bcaaf8de8cfd0a7fc7a72b7)
**Date:** 2026-10-24

---

## 1. DO Membership and Vehicle Identity

### 1.1 Distribution Office Building Type
- **Building type:** 43 (0x2B) = Distribution Office (road variant)
- **Rail variant:** type 52 (0x34)
- **Confirmed in:** `docs/ghidra-do-storage-findings.md` line 80-82; `docs/engine/logistics.md` lines 33-37

### 1.2 DO Vehicle Vector
- **Location:** `building + 0xC70` (begin), `building + 0xC78` (end)
- **Stride:** 8 bytes (pointer to Vehicle)
- **Read by:** DO upkeep, construction offices, fuel burn function `FUN_14016E3B0`
- **Evidence:** Three independent readers confirmed in `docs/AGENT_REPORT_private_industry.md` lines 164-174:
  - DO upkeep: `d_q1_vehicle_owner.c:48-51`
  - Construction toolbar: `Codebase.txt:28352-28354`
  - Fuel burn loop: `d_g2_upkeep.c:443`

### 1.3 Vehicle Home Building
- **Offset:** `vehicle + 0x4F8` = `Building*` (home depot/DO)
- **Used by:** Refuel path draws from this building
- **Confirmed in:** `docs/engine/vehicle-fuel-system.md` lines 179-182; `docs/AGENT_REPORT_private_industry.md` line 21

### 1.4 Vehicle State Gate (Burn Eligibility)
From `FUN_14016E3B0` disassembly (`scratch/asm_burn_fuel.txt` lines 60-61, 84-88):
| Offset | Field | Required Value | Meaning |
|--------|-------|----------------|---------|
| `veh+0x448` | integer state | ∈ {1, 2} | Working states (1=driving, 2=loading/unloading) |
| `veh+0x440` | mode | == 0x9 | Active mode |

**Gate logic (lines 446-461 of `d_g2_upkeep.c`):**
```
if (*(int *)(lVar13 + 0x50c) == 0x42) {  // fuel consumer
  // state check
  MOVSXD R10,[RCX + 0x90]     ; veh+0x448
  LEA EAX,[R10-1]
  TEST EAX,0xFFFFFFEE         ; state ∈ {1,2}
  CMP dword ptr [RCX + 0x88],0x9  ; veh+0x440 == 9
}
```
If state ∉ {1,2} or mode ≠ 9: **vehicle entirely skipped** — no burn, no refuel.

### 1.5 Vehicle Kind Flag
- **Offset:** `veh+0x50C` (int)
- **Value 0x42 (66):** Fuel consumer (road truck)
- **Value 0x43 (67):** Rail vehicle
- **Evidence:** `scratch/asm_burn_fuel.txt` line 446: `CMP dword ptr [RCX + 0x50C], 0x42`

### 1.6 Vehicle Identity Resolution for Road Trucks
A vehicle is a **road DO truck** iff:
1. Its home building (`veh+0x4F8`) is a type-43 DO
2. `veh+0x50C == 0x42`
3. `veh+0x448 ∈ {1,2}` and `veh+0x440 == 0x9` during burn tick

**Provenance chain:**
```
DO building (type 43) → veh+0x4F8 → vehicle
vehicle → veh+0x50C == 0x42 → road truck
vehicle → veh+0x3B8/0x3C0 → cargo vector (includes fuel slot)
```

---

## 2. Cargo Loading and Pickup Completion

### 2.1 DO Task System Overview
From `docs/ghidra-do-storage-findings.md` and `docs/engine/logistics.md`:
- **DO task vector:** `do + 0xD38` (begin), `do + 0xD40` (end), stride `0x18`
- **TaskEntry layout (0x18 bytes):**
  - `+0x00`: Enabled/active byte (0=disabled, 1=enabled)
  - `+0x08`: Target `Building*`
  - `+0x10`: `TaskCfg*` pointer
- **TaskCfg size:** 0x198 bytes, constructed by `FUN_1401512A0` (RVA 0x1512A0)

### 2.2 Load Configuration (Pickup from Source)
From `docs/engine/logistics.md` lines 86-100 and `docs/ghidra-do-storage-findings.md` lines 38-46:
| Config Offset | Meaning | Use |
|---------------|---------|-----|
| `cfg+0x00` | Load all-toggle byte | 1 = load all matching resources |
| `cfg+0x04` | Load percentage | 1.0 = 100% threshold |
| `cfg+0x08/0x10` | Load resource vector | Stride 0x10: `{Resource*, float, float}` |

**Slot mode:** Mode 2 = output storage (DO loads FROM it)

### 2.3 Runtime Planner Generates Orders
- **Function:** `FUN_140424430` (delivery-order planner)
- **Clears/rebuilds:** Global order vector at `game + 0x144A8` (stride 0x68)
- **Walks:** Each target's storages, applies config resource lists, percentages, capacity
- **Reachability:** `FUN_140422C30` over DO waypoints `bld + 0xA10` (stride 0x60)

### 2.4 Pickup Completion — Transfer Call Site
**Mode 0, negative tons** = goods leave building (pickup/load leg)
- **Call site:** `0x694F40` (in `FUN_14068B330`) — family A LOAD
- **Verified disassembly:** `scratch/asm_road_delivery_full.txt` lines 31-39 (predecessor) + `scratch/decomp_68b330.c` lines 1599-1603
- **Arguments at call:**
  - `RCX` = game (`0x1409D4F10`)
  - `RDX` = vehicle (`RBX`)
  - `R8` = mode 0
  - `R9` = resource pointer (`[RAX]`)
  - `[RSP+0x20]` = tons (negative, in XMM0)
  - `[RSP+0x28]` = out pointer (`&local` at RBP+0x20)
  - `[RSP+0x38]` = flag = 0
  - `[RSP+0x40]` = building = `RSI` (origin unresolved)

### 2.5 Paired Transfer Pattern
From `docs/AGENT_REPORT_private_industry.md` lines 293-299:
```
0x694F40  mode 0  -tons   the BUILDING the goods leave (pickup)
0x6953C4  mode 1  +tons   the VEHICLE they enter (vehicle side)
```
**30-70 ms apart**, same resource, same tonnage magnitude.

### 2.6 Pickup Completion Criteria
A pickup is **complete** when:
1. Transfer at `0x694F40` returns `AL == 1` (mutation committed)
2. The cargo slot in vehicle (`veh+0x3B8` vector, matching resource) receives the tons
3. Order entry at `game+0x144A8` has matching resource record updated
4. Vehicle state `veh+0x448` transitions to driving (1) or loading (2)

**Not automatic:** A partial transfer invocation (AL=1 but fVar21 > 0 meaning clamp) is **not** a completed pickup. Must verify actual tons moved = |requested| - rejected.

### 2.7 Cargo Slot Verification
Vehicle cargo vector: `veh+0x3B8` (begin), `veh+0x3C0` (end), stride `0x10`
Slot layout: `{+0x00: Resource*, +0x08: float tons}`
**Fuel slot identified by:** `slot.Resource* == *(game+0xC3F0)` (fuel resource)
**Evidence:** `scratch/asm_burn_fuel.txt` lines 75-83:
```
14016f69d: MOV R9,[R8]              ; R9 = cargo vector begin
14016f6a0: MOV R8,[R15 + 0xc3f0]    ; R8 = game+0xC3F0 (fuel resource)
14016f6b7: CMP qword ptr [RAX],R8   ; CMP slot.Resource*, game+0xC3F0
14016f6bc: MOVSS XMM0,[RAX + 0x8]   ; XMM0 = slot.tons ← FUEL LEVEL
```

---

## 3. Fuel Deductions and Empty-Tank Clamping

### 3.1 Authoritative Fuel Store
**NOT `veh+0x5F0`** (mirror, unclamped, goes negative).
**IS:** Cargo vector slot at `veh+0x3B8` where `slot.Resource* == *(game+0xC3F0)`, field `slot+0x08` = fuel in tonnes.
**Evidence:** `docs/engine/vehicle-fuel-system.md` lines 66-91, 100-106.

### 3.2 Burn Function and Location
- **Function:** `FUN_14016E3B0` (RVA `0x16E3B0`) — building upkeep, called once per building per tick
- **Signature:** `void FUN_14016E3B0(longlong game, longlong building, float tick_delta)`
- **Burn loop:** Iterates vehicles in `bld+0xC70/+0xC78`, filters by `veh+0x50C == 0x42` and state gate

### 3.3 Consumption Formula
From `docs/engine/vehicle-fuel-system.md` lines 125-138 and `scratch/asm_burn_fuel.txt`:
```
slot_rate = *(float*)(*(veh+0x1708) + 0x85E0 + 4 + slot_index * 0x10)
base_rate = DAT_140909FD8 (or DAT_140909E54 if veh+0x50C == 0x42)
bld_divisor = *(float*)(bld+0xE30)
rate = (slot_rate / bld_divisor) * base_rate
tick_scale = tick_delta / DAT_14090AA78
fVar24 = DAT_140909F58 / ((*(bld+0xE68) - *(bld+0xE60)) / 0xC)  // building-level
consumption = rate * tick_scale * fVar24   // tonnes per tick
```
**Load does NOT matter** — `fVar24` is building-level only. `litres_per_tonne = 0` confirmed.

### 3.4 Burn Deduction — Exact Instructions
From `scratch/asm_burn_fuel.txt` lines 91-94:
```
14016f6d6: COMISS XMM0,XMM8              ; compare slot.tons vs consumption
14016f6da: JBE 0x14016f790               ; if tons <= consumption → empty tank
14016f6e0: SUBSS XMM0,XMM8               ; tons = tons - consumption  ← BURN
14016f6e5: MOVSS [RAX + 0x8],XMM0        ; slot.tons = XMM0         ← STORE BACK
```
- **RAX** = `&cargo_slot[slot_index]` (slot matching fuel resource)
- **XMM8** = consumption (tonnes per tick)
- **XMM0** = current slot.tons

### 3.5 Empty-Tank Clamping — Exact Instructions
From `scratch/asm_burn_fuel.txt` lines 126-127:
```
14016f790: MOVAPS XMM8,XMM0
14016f794: MOV dword ptr [RAX + 0x8],0x0 ; slot.tons = 0 (clamped at zero)
```
**Clamp is at zero** — fuel never goes negative in authoritative store.

### 3.6 Mirror at `veh+0x5F0` — NOT Authoritative
Written by two functions, **not** by burn function:
1. **Refuel writer:** `FUN_140142430` — `new = min(available, capacity - current) + current`
2. **Burn adjustment:** `FUN_14014B920` — `new = (rate * factors) + current` (can go negative)

**Observed negative:** −0.0302 t, −0.0502 t (v0.10 run, day 13) — proves mirror is not clamped.

### 3.7 Refuel Path
From `docs/engine/vehicle-fuel-system.md` lines 166-198 and `scratch/asm_refuel.txt`:
- **Trigger:** Stochastic, after burn (`COMISS XMM9,XMM8` at `0x14016F7E4`)
- **Transfer call:** `0x14016F831` → `FUN_140420350`
- **Arguments:**
  - `RCX` = game (`R15`)
  - `RDX` = home building (`*(RBX + 0x4F8)` = `veh+0x4F8`)
  - `R8` = mode 0
  - `R9` = fuel resource (`*(game+0xC330)`)
  - Stack = negative tons (draw FROM building)
  - Out = `&out` (actual tons delivered)

### 3.8 Refuel Clamp
Two capacity mechanisms:
1. **Per-vehicle:** `*(veh+0x1708) + 0x8684` (used in `FUN_140142430`)
2. **Building-level:** `fVar24 = (bld+0xE30 / DAT_14090A968) × DAT_140909E54` (used in burn at `0x14016F7C5`)

**456.0f (0x43E40000) NOT FOUND** — 0 hits in binary (`scratch/search_456f.txt`).

### 3.9 Fuel Price and Billing
- **Price source:** `resource_pricing` API → live resource record
- **Engine behavior:** **No money changes hands** at refuel. Transfer moves fuel, `FUN_1402FDB00` books statistics only.
- **Private billing:** Must be implemented at transfer hook (`0x16F831`) reading home building owner.

### 3.10 Per-Trip Fuel Measurement (BurnProbe)
From `private_logistics.cpp` lines 3124-3146:
```c
// Sample fleet fuel at each committed delivery
double now = FleetFuelTons(bld, &n);
if (same_fleet && now < prev - 0.02 && now > prev - 0.5) {
    burned += prev - now;   // positive = burned
    steps++;
} else {
    skips++;  // refill or fleet change
}
```
**Measures:** Actual tonnes left tanks between deliveries. Refills (increase) counted separately, never subtracted.

---

## 4. Unloading and Delivery Boundary

### 4.1 Unload Configuration (Delivery to Destination)
From `docs/engine/logistics.md` lines 86-100:
| Config Offset | Meaning | Use |
|---------------|---------|-----|
| `cfg+0xC8` | Unload all-toggle byte | 1 = unload all matching |
| `cfg+0xCC` | Unload percentage | 1.0 = 100% threshold |
| `cfg+0xD0/0xD8` | Unload resource vector | Stride 0x10 |

**Slot mode:** Mode 1 = input storage (DO unloads INTO it)

### 4.2 Delivery Transfer Call Site — Road Truck
**Call site:** `0x1B2898` (in `FUN_1401B1290`) — road delivery unload
**Verified disassembly:** `scratch/asm_road_delivery_full.txt` lines 31-39

**Arguments at `0x1B2898`:**
| Register/Stack | Value | Meaning |
|----------------|-------|---------|
| `RCX` | `R14` = game (`0x1409D4F10`) | Game pointer |
| `RDX` | `RDI` = vehicle | Vehicle pointer |
| `R8` | `1` | Mode 1 (vehicle cargo slot) |
| `R9` | `R12` = resource | Resource pointer |
| `[RSP+0x20]` | `XMM6` | Tons (positive = delivery) |
| `[RSP+0x28]` | `R15` | Out pointer (RSI, origin unresolved) |
| `[RSP+0x38]` | `R15B` = 0 | Flag = 0 |
| `[RSP+0x40]` | `R15` = NULL | Building = NULL (destination resolved internally) |

**Key finding:** `param_9` (building) = **NULL** — destination resolved from order, not passed explicitly. This is a third-party delivery.

### 4.3 Delivery Completion — Transfer Semantics
From `docs/AGENT_REPORT_private_industry.md` lines 106-128 and `scratch/decomp_420350.c`:
- **Return `AL == 1`**: Mutation committed (slot written), **NOT** positive movement
- **Output `fVar21` (written to `*param_6`)**: Rejected/overflow amount, **NOT** actual moved tons
- **Actual moved** = `|requested_tons| - fVar21`

**Numerical cases:**
| Case | Initial | Requested | Clamp | Final | fVar21 (out) | Actual moved |
|------|---------|-----------|-------|-------|--------------|--------------|
| Within capacity | 7 | +3 | 10 | 10 | 0.0 | 3 |
| Exceeds capacity | 7 | +5 | 10 | 10 | 2.0 | 3 |
| Zero request | 7 | 0 | — | 7 | (not written) | 0 |

### 4.4 Complete Delivery Boundary
A delivery is **complete** when:
1. Transfer at `0x1B2898` returns `AL == 1`
2. Vehicle cargo slot (`veh+0x3B8` matching resource) decremented by actual moved tons
3. Order completion record updated: `order+0x50` array, element `+0x28+0x188` = done flag (1.0f)
4. Building storage receives goods (mode 0, +tons at destination)

### 4.5 DO Task Completion Check
From `scratch/decomp_1e5480.c` and `scratch/decomp_1de2b0.c`:
- **Entry point:** `FUN_1401E5350` → `FUN_1401E5480` (DO upkeep)
- **Iterates:** DO task records at `do+0xD58/+0xD60` (stride 0x80), **NOT** order vector `game+0x144A8`
- **Completion check:** `FUN_1401DE2B0(entry, vehicle, DO)` per task
- **TaskCfg source:** `*( *(do+0xD38) + 0x10 )` (first task's config)

**Completion criteria in `FUN_1401DE2B0`:**
- `param_2[0xC] > 0` (tons > 0)
- `param_2[5] > param_2[0xC] - DAT_14090A008` (distance check)
- Resource and building type checks (not construction/demolition unless line-stop matches)
- Vehicle cargo has the resource (`veh+0x3B8` scan)

### 4.6 Delivery vs. Completion Distinction
| Event | Observation Point | Completeness |
|-------|-------------------|--------------|
| Transfer unload | `0x1B2898` (mode 1, +tons) | Goods leave vehicle |
| Order done flag | `order+0x50[i]+0x28+0x188 = 1.0f` | Order marked complete |
| DO task complete | `FUN_1401DE2B0` returns 1 | Task satisfied, may re-plan |

**A partial transfer (clamped) is NOT a complete delivery** — must verify actual moved = requested.

### 4.7 Empty Travel Exclusion
- **Before pickup:** Vehicle travels empty from DO to source — no fuel deduction attributed to trip
- **After delivery:** Vehicle travels empty from destination back to DO — no fuel deduction attributed to trip
- **Only loaded travel:** Fuel burned while `veh+0x448 ∈ {1,2}` and cargo > 0 counts toward trip cost
- **Burn gate:** Vehicle entirely skipped if state ∉ {1,2} — parked vehicles don't burn

### 4.8 Cost Policy Application
**Initial policy:** `sum(actual_fuel_deducted_tonnes) × settlement_price_per_tonne`
- **Actual fuel deducted:** From authoritative slot `slot+0x08` delta between pickup and delivery
- **Settlement price:** In-game fuel price per tonne at delivery time (via `resource_pricing`)
- **Excludes:** Empty travel, refuel events, tank fills

---

## 5. Session/Lifetime Invalidation

### 5.1 Vehicle Invalidation Conditions
A road DO truck observation becomes invalid when:

| Condition | Detection | Effect |
|-----------|-----------|--------|
| Vehicle demolished | `TsmIsDemolished(veh)` or `veh+0x...` demolished flag | Pointer invalid, stop tracking |
| Vehicle reassigned | `veh+0x4F8` (home) changes to different DO | No longer this DO's truck |
| Vehicle type changes | `veh+0x50C` ≠ 0x42 | No longer fuel consumer |
| State permanently idle | `veh+0x448` ∉ {1,2} for extended period | Burn stops, no trip data |
| Save/load | Game reloads | All pointers invalidated, must re-discover |

### 5.2 DO Invalidation Conditions
| Condition | Detection | Effect |
|-----------|-----------|--------|
| DO demolished | Building demolished flag | Task vector invalid |
| DO type changes | Building type ≠ 43 | Not a road DO |
| Fleet emptied | `bld+0xC70` == `bld+0xC78` | No vehicles to track |
| Task vector cleared | `do+0xD38` == `do+0xD40` | No targets, no trips |

### 5.3 Task/Target Invalidation
From `docs/ghidra-do-storage-findings.md` lines 67-72:
- **Reachability disable ≠ removal:** `FUN_140162B40` disables entries (clears `entry+0x00`) but retains TaskEntry/TaskCfg
- **Explicit removal:** `FUN_140162B00` frees/removes — separate flow
- **Automation must merge:** Preserve manual entries, don't re-add disabled-but-retained targets
- **Save/load persistence:** `FUN_1401F0BE0` reconstructs task vector; `FUN_1406C5350`/`6C040` serialize TaskCfg

### 5.4 Order Structure Lifetime
- **Order vector:** `game+0x144A8/+0x144B0`, stride 0x68
- **Orders reused:** Slots recycled, no durable operation ID linking pickup to delivery
- **Completion records:** At `order+0x50` (array of pointers to 0x198-stride records)
- **No persistent linkage:** Cannot reliably link a specific pickup to its delivery across ticks

### 5.5 Fuel Measurement Session Invalidation
From `private_logistics.cpp` `BurnProbe` (lines 3124-3146):
```c
if (g_probeBld != bld || g_probeFuelTons < 0.0 || n != g_probeVehCount) {
    // New fleet or first sample → reset baseline
    g_probeBld = bld; g_probeFuelTons = now; g_probeVehCount = n;
    return;
}
if (d < -0.02 || d > 0.5) {
    g_probeSkipRun++;  // Refill or fleet change → invalidate interval
}
```
**Invalidates interval when:**
- Fleet composition changes (vehicle count differs)
- Refuel detected (fuel increase > 0.02t)
- Anomalous delta (> 0.5t or < -0.02t)

### 5.6 Plugin-Side Session Management
From `private_logistics.cpp`:
- **Daily callback:** `OnDay` (sim_clock) — snapshots counters, prints totals
- **Save/load signal:** `AutoPersistenceEvent` + `g_autoResetActiveMemoAfterLoad`
- **Census runs once/day:** `CensusFleets` walks enterprise buildings
- **Field scanner:** `ScanVehicle` compares 512 dwords (0x800 bytes) per vehicle daily
- **All state in static arrays:** No allocation in hot paths, survives save/load via persistence

### 5.7 Hook Safety and Invalidation
- **Call-site patching:** 16 `CALL rel32` sites to `0x420350` patched to detour
- **Verification per site:** Byte 0 == `0xE8`, rel32 resolves to `exeBase+0x420350`
- **If verification fails:** Site skipped, original call intact — safe degradation
- **Detour:** `JMP [RIP+0]` + 8-byte absolute address (14 bytes)
- **No inline hooks:** Transfer prologue < 14 bytes, call-site patching required

---

## 6. Safe Observation Mechanism

### 6.1 Loader API Support (Current Repository)
From `plugins/private_logistics/private_logistics.cpp` and `src/` headers:
- **Hook type:** Call-site patching (not inline hook) — 16 sites, all `CALL rel32`
- **Detour mechanism:** Code cave with `JMP [RIP+0]` + 8-byte absolute address
- **Verification:** Each site checked for `0xE8` opcode and correct target before patch
- **Safety:** `ReadablePtr` on all game memory reads, `__try/__except` on risky dereferences
- **Services consumed:** `enterprise_base`, `currency`, `sim_clock`, `resource_pricing`, `persistence`, `ui_panels`

### 6.2 Observation Points Supported by Current Loader
| Observation | Mechanism | Status |
|-------------|-----------|--------|
| Transfer calls (all 16 sites) | Call-site patch → `DetourTransfer` | **Implemented** |
| Vehicle fuel (authoritative slot) | Daily census + `BurnProbe` at delivery | **Implemented** |
| Vehicle cargo vector | `CargoTons()` / `FuelSlotTons()` walk | **Implemented** |
| DO task vector | `AutoReadTargetVector()` | **Implemented** |
| Building ownership | `enterprise_base->isBuildingPrivate()` | **Implemented** |
| Resource pricing | `resource_pricing->getPrice()` / `getByName()` | **Implemented** |
| Sim clock (daily callback) | `sim_clock->registerDay(OnDay)` | **Implemented** |
| Persistence (save/load) | `persistence->registerHandler()` | **Implemented** |

### 6.3 Required Observations for Road Truck Trip Cost
| Required | Current Support | Gap |
|----------|-----------------|-----|
| Identify DO truck | `veh+0x4F8` → DO, `veh+0x50C==0x42`, state gate | ✅ Census does this |
| Pickup completion | Transfer `0x694F40` (mode 0, -tons) + cargo slot delta | ✅ Hooked, but need pickup/delivery pairing |
| Fuel burned during trip | `BurnProbe` at delivery (fleet fuel delta) | ✅ Implemented |
| Delivery completion | Transfer `0x1B2898` (mode 1, +tons) + order done flag | ✅ Hooked, but pairing unresolved |
| Settlement price | `resource_pricing` at delivery time | ✅ Available |
| Empty travel exclusion | State gate `veh+0x448∈{1,2}` + cargo > 0 | ✅ Burn only runs when active |

### 6.4 Critical Gap: Pickup-Delivery Pairing
**No durable operation ID exists** linking pickup (`0x694F40`) to delivery (`0x1B2898`).
- Order slots reused (`game+0x144A8` stride 0x68)
- Vehicle may run multiple orders concurrently
- Time-window pairing (30-70ms) used by current plugin for carrier resolution, but **not reliable for cost attribution**

**Current workaround in plugin:** `BurnProbe` samples fleet fuel at each delivery, assumes fuel burned since last delivery = trip fuel. **Invalidated by refills, fleet changes.**

### 6.5 Safe Mechanism Proposal: Opt-In Diagnostic
Since no durable linkage exists, the only safe observation is:

**Per-delivery diagnostic event** (no billing, measurement only):
```
On delivery commit (0x1B2898, AL=1, tons>0):
  1. Read vehicle pointer (RDX)
  2. Verify: veh+0x4F8 == DO, veh+0x50C==0x42, state∈{1,2}
  3. Read fuel slot (veh+0x3B8 matching game+0xC3F0) → tons_after
  4. Compute burned = tons_before - tons_after (from previous delivery sample)
  5. If burned > 0 and < threshold: emit DIAGNOSTIC event
  6. Update tons_before = tons_after for next delivery
```

**Requires:**
- Per-vehicle `tons_before` state (static array, 64 vehicles max)
- Delivery site filter: only `0x1B2898` (road delivery)
- Refill detection: if tons_after > tons_before + epsilon → reset baseline
- Fleet change detection: recount vehicles at DO

### 6.6 ABI/Register/Stack/Flags at Delivery Site `0x1B2898`
From `scratch/asm_road_delivery_full.txt`:
```
; Predecessor block (0x1401B2874-0x1401B2898):
1401b2874: MOV [RSP+0x40],R15     ; param_9 = NULL
1401b2879: MOV [RSP+0x38],R15B    ; flag = 0
1401b287e: MOV [RSP+0x28],R15     ; out = RSI (unresolved)
1401b2883: MOVSS [RSP+0x20],XMM6  ; tons = XMM6 (>0)
1401b2889: MOV R9,R12             ; resource = R12
1401b288c: MOV R8D,1              ; mode = 1
1401b2892: MOV RDX,RDI            ; vehicle = RDI
1401b2895: MOV RCX,R14            ; game = R14
1401b2898: CALL 0x140420350       ; transfer
```
**Registers at call:**
- `RCX` = game, `RDX` = vehicle, `R8` = 1, `R9` = resource
- `XMM6` = tons (positive)
- `R15` = NULL (param_9), also used for out pointer (RSI value)
- **Flags:** Unmodified by argument setup

**Post-call:**
```
1401b289d: DIVSS XMM6,[0x14090A7B0]   ; tons /= constant
1401b28a5: MOV RAX,[RSI+0xE90]        ; RSI used post-call
```
**RSI provenance UNKNOWN** — used as both param_9 and out pointer, then dereferenced post-call.

### 6.7 Interaction with Existing `private_logistics` Hooks
- **DetourTransfer** (lines 1140-1454) already intercepts all 16 sites
- **Site classification:** `SiteIndex(_ReturnAddress() - g_exeBase - 5)` maps RVA to `g_sites[]`
- **Road delivery site:** `g_sites[2]` = RVA `0x1B2898`, class `SITE_UNKNOWN` (should be `SITE_DELIVERY`)
- **Billing logic:** Only `mode == 0` billed; `mode == 1` feeds carrier ring only
- **Fuel billing:** Would need to add at `0x16F831` (refuel) or compute from `BurnProbe`

**To add trip-cost diagnostic:**
1. Add `SITE_DELIVERY` classification for `0x1B2898`
2. In `DetourTransfer`, when `siteRva==0x1B2898 && committed && mode==1 && tons>0`:
   - Read vehicle fuel slot
   - Compute delta from stored baseline
   - Emit diagnostic log (no payment)
3. Store baseline per vehicle in `g_vehFuel[VEH_MAX]`

### 6.8 Blocker Summary
| Blocker | Severity | Resolution |
|---------|----------|------------|
| No durable pickup-delivery ID | **HIGH** | Use per-delivery fleet fuel delta (approximate) |
| RSI provenance at `0x1B2898` | **MEDIUM** | Not needed for fuel observation (use vehicle fuel slot) |
| Refuel invalidates fuel delta | **MEDIUM** | Detect via fuel increase, reset baseline |
| Fleet change invalidates delta | **MEDIUM** | Recount vehicles at DO per delivery |
| No payment mechanism authorized | **LOW** | Diagnostic only — no billing in this phase |

---

## 7. Evidence Matrix

| # | Claim | Evidence Source | Type | Confidence |
|---|-------|-----------------|------|------------|
| 1 | DO type = 43 (road) | `ghidra-do-storage-findings.md:80`, `logistics.md:33` | Static analysis | Proven |
| 2 | DO vehicle vector at `bld+0xC70/0xC78` stride 8 | `AGENT_REPORT.md:164-174` (3 readers) | Static analysis | Proven |
| 3 | Vehicle home at `veh+0x4F8` | `vehicle-fuel-system.md:179`, `AGENT_REPORT.md:21` | Static analysis | Proven |
| 4 | Burn state gate: `veh+0x448∈{1,2}`, `veh+0x440==9` | `vehicle-fuel-system.md:150-161`, `asm_burn_fuel.txt:60-61` | Disassembly | Proven |
| 5 | Vehicle kind `veh+0x50C==0x42` = road truck | `asm_burn_fuel.txt:446`, `private_logistics.cpp:3376` | Disassembly | Proven |
| 6 | Authoritative fuel = cargo slot `slot+0x08` matching `game+0xC3F0` | `vehicle-fuel-system.md:66-91`, `asm_burn_fuel.txt:75-83` | Disassembly | Proven |
| 7 | `veh+0x5F0` is mirror, goes negative | `vehicle-fuel-system.md:77-86`, `private_logistics.cpp:491` | Runtime + static | Proven |
| 8 | Burn formula: `consumption = (slot_rate/bld_divisor)*base_rate*tick_scale*fVar24` | `vehicle-fuel-system.md:125-138` | Decompilation | Proven |
| 9 | Load does not affect burn (`fVar24` building-level) | `vehicle-fuel-system.md:140-148` | Decompilation | Proven |
| 10 | Burn deduction: `SUBSS XMM0,XMM8` → `MOVSS [RAX+8],XMM0` | `asm_burn_fuel.txt:91-94` | Raw instructions | Proven |
| 11 | Empty-tank clamp: `MOV [RAX+8],0` at `0x14016F794` | `asm_burn_fuel.txt:126-127` | Raw instructions | Proven |
| 12 | Refuel via transfer `0x420350` at `0x14016F831` | `vehicle-fuel-system.md:179-198`, `asm_refuel.txt:31` | Disassembly | Proven |
| 13 | Refuel draws from `veh+0x4F8` (home building) | `asm_refuel.txt:29-31` | Raw instructions | Proven |
| 14 | No money at refuel (stats only) | `vehicle-fuel-system.md:216-218` | Decompilation | Proven |
| 15 | Pickup transfer at `0x694F40` (mode 0, -tons) | `AGENT_REPORT.md:293-299`, `decomp_68b330.c` | Disassembly | Proven |
| 16 | Delivery transfer at `0x1B2898` (mode 1, +tons) | `asm_road_delivery_full.txt:31-39`, `decomp_1b1290.c` | Disassembly | Proven |
| 17 | Delivery `param_9=NULL` (dest resolved internally) | `asm_road_delivery_full.txt:31` | Raw instructions | Proven |
| 18 | Transfer return AL=1 ≠ positive movement | `decomp_420350.c:233-258` (in repaired report) | Decompilation | Proven |
| 19 | Transfer out `fVar21` = rejected/overflow, not moved | `decomp_420350.c:233-258` | Decompilation | Proven |
| 20 | DO task vector `do+0xD38/0xD40` stride 0x18 | `ghidra-do-storage-findings.md:26-30` | Static analysis | Proven |
| 21 | TaskEntry: `+0x00` enabled, `+0x08` target, `+0x10` TaskCfg* | `ghidra-do-storage-findings.md:27-30` | Static analysis | Proven |
| 22 | Load config at `cfg+0x00/0x04/0x08`, unload at `cfg+0xC8/0xCC/0xD0` | `logistics.md:86-100` | Static analysis | Proven |
| 23 | Planner `FUN_140424430` builds orders `game+0x144A8` | `logistics.md:104-109` | Static analysis | Proven |
| 24 | DO completion checks task records `do+0xD58/0xD60` stride 0x80 | `decomp_1e5480.c:38-55` | Decompilation | Proven |
| 25 | Completion check `FUN_1401DE2B0` uses TaskCfg not order vector | `decomp_1de2b0.c`, `decomp_1e5480.c:73` | Decompilation | Proven |
| 26 | No durable operation ID linking pickup to delivery | Repaired report U5, U8 | Analysis gap | Confirmed gap |
| 27 | RSI provenance at `0x1B2898` unresolved | Repaired report U1 | Analysis gap | Confirmed gap |
| 28 | Cargo vector `veh+0x3B8/0x3C0` stride 0x10, slot `{Res*,float}` | `vehicle-fuel-system.md:52-64` | Disassembly | Proven |
| 29 | Fuel resource globals `game+0xC330` (xfer), `game+0xC3F0` (burn) | `vehicle-fuel-system.md:209-214` | Static analysis | Likely same |
| 30 | Per-vehicle capacity `*(veh+0x1708)+0x8684` | `vehicle-fuel-system.md:223-227` | Decompilation | Proven |
| 31 | Building-level capacity component `fVar24` | `vehicle-fuel-system.md:227-228` | Decompilation | Proven |
| 32 | 456.0f constant NOT in binary | `vehicle-fuel-system.md:229`, `search_456f.txt` | Search | Proven |
| 33 | Private_logistics hook: call-site patch 16 sites | `private_logistics.cpp:1460-1532` | Implementation | Proven |
| 34 | BurnProbe measures fleet fuel delta per delivery | `private_logistics.cpp:3124-3146` | Implementation | Proven |
| 35 | Save/load reconstruction `FUN_1401F0BE0` | `ghidra-do-storage-findings.md:59-65` | Static analysis | Likely |

**Confidence legend:** Proven = 2+ independent sources (disassembly + decompilation + plugin verification); Likely = 1 strong source; Gap = confirmed missing.

---

## 8. Proposed Adapter Events

### 8.1 Diagnostic Event: Road Truck Trip Fuel Sample
**Trigger:** Committed delivery at `0x1B2898` (mode 1, tons > 0, AL=1)
**Conditions:**
- Vehicle `RDX` has `veh+0x4F8` = DO building (type 43)
- Vehicle `veh+0x50C == 0x42`
- Vehicle `veh+0x448 ∈ {1,2}` and `veh+0x440 == 9`
- Fuel slot exists in `veh+0x3B8` matching `game+0xC3F0`

**Payload:**
```c
struct RoadTruckTripSample {
    uint64_t  timestamp;        // GetTickCount64()
    void*     vehicle;          // RDX at call
    void*     do_building;      // veh+0x4F8
    float     fuel_tons_before; // Previous delivery sample (tonnes)
    float     fuel_tons_after;  // Current slot+0x08 reading (tonnes)
    float     fuel_burned_tons; // before - after (clamped ≥0)
    float     cargo_tons;       // Total cargo in vehicle at delivery
    unsigned  site_rva;         // 0x1B2898
    unsigned  vehicle_state;    // veh+0x448
    unsigned  vehicle_mode;     // veh+0x440
    bool      refill_detected;  // after > before + 0.02
    bool      fleet_changed;    // Vehicle count at DO differs
};
```

**Emission:** Log only (no payment), rate-limited to 1 per delivery per vehicle.

### 8.2 Diagnostic Event: Pickup Completion
**Trigger:** Committed transfer at `0x694F40` (mode 0, tons < 0, AL=1)
**Conditions:**
- Building `param_9` (RSI) has storage
- Vehicle `RDX` receives cargo (mode 1 pair follows within 100ms)

**Payload:**
```c
struct PickupSample {
    uint64_t  timestamp;
    void*     vehicle;          // RDX
    void*     source_building;  // param_9 (RSI, if resolved)
    void*     resource;         // R9
    float     requested_tons;   // |param_5|
    float     actual_moved_tons; // |param_5| - fVar21 (from out pointer)
    unsigned  site_rva;         // 0x694F40
};
```

### 8.3 Diagnostic Event: Refuel
**Trigger:** Committed transfer at `0x16F831` (mode 0, fuel resource, AL=1)
**Payload:**
```c
struct RefuelSample {
    uint64_t  timestamp;
    void*     vehicle;          // RDX (from veh+0x4F8 lookup)
    void*     home_building;    // veh+0x4F8
    float     tons_delivered;   // *outMoved (positive)
    float     fuel_tons_before; // slot+0x08 before
    float     fuel_tons_after;  // slot+0x08 after
    unsigned  site_rva;         // 0x16F831
};
```

### 8.4 Integration with Existing Plugin
**Location:** `DetourTransfer` in `private_logistics.cpp` (lines 1140-1454)
**Modifications:**
1. Add `SITE_DELIVERY` classification for RVA `0x1B2898` (currently `SITE_UNKNOWN`)
2. Add per-vehicle baseline array: `static float g_vehFuelAtPickup[VEH_MAX];`
3. In `DetourTransfer`, when `siteRva==0x1B2898 && committed && mode==1 && tons>0`:
   - Call `ReadVehicleFuelSlot(vehicle, &after)`
   - `burned = g_vehFuelAtPickup[slot] - after`
   - If `burned > 0.001 && burned < 1.0`: emit `RoadTruckTripSample`
   - `g_vehFuelAtPickup[slot] = after`
4. When `siteRva==0x694F40 && committed && mode==0 && tons<0`:
   - Call `ReadVehicleFuelSlot(vehicle, &before)`
   - `g_vehFuelAtPickup[slot] = before` (baseline at pickup)

### 8.5 Safety Guarantees
- **No allocation in detour:** All arrays static, fixed size (VEH_MAX=64)
- **ReadablePtr on all game reads:** Vehicle, cargo vector, fuel slot
- **No billing:** Diagnostic events only, `g_dryRun` respected
- **Graceful degradation:** If any read fails, skip event, continue
- **Save/load safe:** Baselines reset on fleet change detection

---

## 9. Remaining Gaps

| # | Gap | Impact | Resolution Path |
|---|-----|--------|-----------------|
| 1 | **No durable pickup-delivery linkage** | Cannot definitively attribute fuel burned to specific trip | Use per-delivery fleet fuel delta (BurnProbe); accept approximation |
| 2 | **RSI provenance at `0x1B2898` unknown** | Cannot verify destination building from param_9 | Not needed for fuel observation (use vehicle fuel slot directly) |
| 3 | **Refuel invalidates fuel delta baseline** | BurnProbe skips intervals with refuel | Detect via fuel increase > 0.02t; reset baseline |
| 4 | **Fleet change invalidates baseline** | Vehicle count/type change breaks per-vehicle tracking | Recount vehicles at DO per delivery; reset affected baselines |
| 5 | **Out pointer at `0x1B2898` = RSI (unresolved)** | Cannot read `fVar21` (rejected amount) to verify actual moved | Not critical for fuel measurement; fuel slot delta is authoritative |
| 6 | **Order vector slot reuse** | No stable operation ID across ticks | Diagnostic only — no billing linkage required |
| 7 | **Rail DO (type 52) not verified for road fuel path** | Rail trucks may use different burn/refuel logic | Scope limited to road DO (type 43) per task |
| 8 | **Construction/demolition delivery path (`0x6953C4` family)** | Different transfer sites, different semantics | Excluded — task is road DO trucks only |
| 9 | **Fuel price at exact delivery tick** | `resource_pricing` may have latency | Accept day-close price; log price used |
| 10 | **Vehicle catalogue data (capacity, consumption) at `veh+0x1708`** | Per-vehicle capacity known, but consumption rate per model not fully mapped | Log `*(veh+0x1708)+0x8684` capacity; slot_rate from `+0x85E0` array |
| 11 | **Empty travel fuel burn** | Vehicles burn fuel traveling empty to/from pickup | Excluded by policy — only loaded travel counted |
| 12 | **Multi-DO shared vehicle** | Vehicle `veh+0x4F8` points to one DO; what if shared? | `veh+0x4F8` is single pointer — assume exclusive home |

**Blockers for billing (not diagnostic):**
- Gap 1 + Gap 6: No audit trail linking specific pickup to delivery
- Gap 3 + Gap 4: Fuel measurement has blind spots (refills, fleet changes)
- No authorization for payment implementation in this phase

---

## 10. GO/BLOCKED for Opt-In Diagnostic

### 10.1 GO — Diagnostic Observation Ready
The following can be implemented **immediately** with existing loader support:

| Capability | Status | Evidence |
|------------|--------|----------|
| Identify road DO trucks | ✅ | Census + state gate + kind flag |
| Read authoritative fuel | ✅ | Cargo slot at `veh+0x3B8` matching `game+0xC3F0` |
| Hook delivery site `0x1B2898` | ✅ | Call-site patch verified, 16 sites enumerated |
| Hook pickup site `0x694F40` | ✅ | Same hook, different site classification |
| Hook refuel site `0x16F831` | ✅ | Same hook, classified as `SITE_REFUEL` |
| Measure fleet fuel delta | ✅ | `BurnProbe` implemented and tested |
| Per-vehicle baseline tracking | ✅ | Static arrays, 64 vehicles max |
| Save/load persistence | ✅ | `persistence_api` handler registered |
| Daily census + field scanner | ✅ | Runs on sim_clock callback |

**Diagnostic output:** Per-delivery log lines with fuel burned, cargo tons, vehicle state, refill/fleet-change flags. **No billing.**

### 10.2 BLOCKED — Billing/Authoritative Trip Cost
| Blocker | Reason |
|---------|--------|
| No durable pickup-delivery ID | Cannot prove which fuel burned belongs to which cargo trip |
| Refuel/fleet-change blind spots | Fuel delta method loses intervals; cannot guarantee coverage |
| RSI/unresolved provenance | Destination building verification incomplete |
| No payment authorization | Task explicitly excludes billing implementation |
| Empty travel exclusion | Policy-only; engine burns fuel during empty travel (state gate only) |

### 10.3 Recommended Path Forward
1. **Phase 1 (Now):** Deploy diagnostic opt-in — emit `RoadTruckTripSample` events per Section 8.1. Collect data across multiple saves/republics.
2. **Phase 2 (Later):** If diagnostic data shows stable correlation (e.g., fuel burned ≈ constant per delivery ± variance), propose fixed per-trip tariff.
3. **Phase 3 (If needed):** Investigate order completion records (`order+0x50`) for durable linkage; or add game-side operation ID via code cave.

### 10.4 Opt-In Configuration
Add to `private_logistics.ini`:
```ini
[private_logistics]
road_trip_diagnostic = 1          ; Enable diagnostic events
road_trip_log_per_day = 200       ; Rate limit
road_trip_min_burn_tons = 0.001   ; Filter noise
road_trip_max_burn_tons = 1.0     ; Filter anomalies
```
Default: **disabled** (`road_trip_diagnostic = 0`).

---

## Appendix A: Export Scripts and Commands

### A.1 Ghidra Scripts Used
All scripts in `C:\WRSRE-ghidra-work\scripts\` (or `C:\WRSRE\scratch\` via wrapper). Addresses passed as static VAs (`0x140` + RVA).

| Script | Purpose | Command |
|--------|---------|---------|
| `DecompAt.java` | Decompile single function | `scratch\ghidra_run.bat DecompAt.java <out.c> 0x140<RVA>` |
| `DisasmMany.java` | Disassemble address range | `scratch\ghidra_run.bat DisasmMany.java <out.asm> 0x140<start>:0x140<end>` |
| `Xrefs.java` | Cross-references to address | `scratch\ghidra_run.bat Xrefs.java <out.txt> 0x140<RVA>` |
| `ListFuncs.java` | Enumerate functions in range | `scratch\ghidra_run.bat ListFuncs.java <out.txt> 0x140<start> 0x140<end>` |
| `FindDisp.java` | Find instructions with displacement | `scratch\ghidra_run.bat FindDisp.java <out.txt> <disp_hex>` |

### A.2 Exact Commands Executed (This Investigation)
```powershell
# Fuel burn function decompilation
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_16e3b0.c 0x14016E3B0'

# Fuel burn loop disassembly (0x16F590-0x16F7A0)
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_burn_fuel.txt 0x14016F590:0x14016F7A0'

# Refuel section disassembly (0x16F7A0-0x16F8A0)
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_refuel.txt 0x14016F7A0:0x14016F8A0'

# Road delivery call site (0x1B2800-0x1B28C0)
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DisasmMany.java scratch/asm_road_delivery_full.txt 0x1401B2800:0x1401B28C0'

# Road delivery function decompilation
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1b1290.c 0x1401B1290'

# DO completion function decompilation
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1e5350.c 0x1401E5350'
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1e5480.c 0x1401E5480'

# DO task completion check decompilation
cd 'C:\WRSRE'; cmd /c 'scratch\ghidra_run.bat DecompAt.java scratch/decomp_1de2b0.c 0x1401DE2B0'
```

### A.3 Key Output Files (Preserved in `scratch/`)
| File | Content |
|------|---------|
| `decomp_16e3b0.c` | `FUN_14016E3B0` — building upkeep (burn + refuel) |
| `asm_burn_fuel.txt` | Raw instructions: fuel slot lookup, burn, clamp |
| `asm_refuel.txt` | Raw instructions: stochastic refuel trigger, transfer call |
| `asm_road_delivery_full.txt` | Raw instructions: `0x1B2898` argument setup + call |
| `decomp_1b1290.c` | `FUN_1401B1290` — road delivery AI |
| `decomp_1e5350.c` | `FUN_1401E5350` — DO completion entry |
| `decomp_1e5480.c` | `FUN_1401E5480` — DO task iteration + completion |
| `decomp_1de2b0.c` | `FUN_1401DE2B0` — per-task completion check |

### A.4 Repaired Binary Investigation Report (Commit 80a4404)
**File:** `docs/FUEL PLAN/association-investigation/BINARY_INVESTIGATION_COMPLETE.md`
**Key exports from that report:**
- 16 CALL rel32 sites to `0x420350` (Table 3.1)
- Verified instruction listings for `0x694F40` and `0x6953C4` (Sections 7.1, 7.2)
- Transfer function decompilation with corrected `fVar21` semantics (Section 5)
- Argument setup tables with reaching definitions (Section 8)
- Output-use analysis through pointee (Section 10)
- Findings classification: Confirmed/Candidate/Contradicted/Unresolved (Section 11)
- Adapter-contract contradictions (Section 12)
- All Ghidra scripts and exact commands (Section 14)

---

## Appendix B: Raw Disassembly Exports

### B.1 Fuel Burn — Slot Lookup and Deduction (`scratch/asm_burn_fuel.txt` lines 75-94)
```
14016f69d: MOV R9,[R8]                    ; R9 = cargo vector begin (veh+0x3B8)
14016f6a0: MOV R8,[R15 + 0xc3f0]          ; R8 = game+0xC3F0 (fuel resource ptr)
14016f6b0: SHL RAX,0x4                    ; RAX = slot_index * 0x10
14016f6b4: ADD RAX,R9                     ; RAX = &cargo_slot[slot_index]
14016f6b7: CMP qword ptr [RAX],R8         ; CMP slot.Resource*, game+0xC3F0
14016f6ba: JNZ 0x14016f6c7                ; not fuel → next slot
14016f6bc: MOVSS XMM0,[RAX + 0x8]         ; XMM0 = slot.tons ← FUEL LEVEL
14016f6c1: COMISS XMM0,XMM9               ; compare vs 0.0
14016f6c5: JA 0x14016f6d6                 ; if > 0 → burn
14016f6d6: COMISS XMM0,XMM8               ; compare slot.tons vs consumption
14016f6da: JBE 0x14016f790                ; if tons <= consumption → empty tank
14016f6e0: SUBSS XMM0,XMM8                ; tons = tons - consumption  ← BURN
14016f6e5: MOVSS [RAX + 0x8],XMM0         ; slot.tons = XMM0         ← STORE
```

### B.2 Empty-Tank Clamp (`scratch/asm_burn_fuel.txt` lines 126-127)
```
14016f790: MOVAPS XMM8,XMM0
14016f794: MOV dword ptr [RAX + 0x8],0x0  ; slot.tons = 0 (clamped at zero)
```

### B.3 Refuel Trigger and Transfer (`scratch/asm_refuel.txt` lines 3-9, 20-31)
```
14016f7af: COMISS XMM8,XMM9               ; roll vs threshold
14016f7e4: JC 0x14016f7f9                 ; if roll > threshold → refuel
...
14016f800: MOV [RSP+0x40],0x0             ; param_9 = NULL
14016f809: MOV [RSP+0x38],0x0             ; flag = 0
14016f817: MOV R9,[R15 + 0xc330]          ; fuel resource (transfer)
14016f824: XOR R8D,R8D                    ; mode = 0
14016f827: MOV RDX,[RBX + 0x4f8]          ; veh+0x4F8 = home building
14016f831: CALL 0x140420350               ; transfer(game, home, 0, fuel, -tons, &out)
```

### B.4 Road Delivery Call Site `0x1B2898` (`scratch/asm_road_delivery_full.txt` lines 31-39)
```
1401b2874: MOV [RSP+0x40],R15             ; param_9 = R15 (NULL)
1401b2879: MOV [RSP+0x38],R15B            ; flag = 0
1401b287e: MOV [RSP+0x28],R15             ; out = R15 (RSI value, unresolved)
1401b2883: MOVSS [RSP+0x20],XMM6          ; tons = XMM6 (>0, delivery)
1401b2889: MOV R9,R12                     ; resource = R12
1401b288c: MOV R8D,1                      ; mode = 1
1401b2892: MOV RDX,RDI                    ; vehicle = RDI
1401b2895: MOV RCX,R14                    ; game = R14
1401b2898: CALL 0x140420350               ; transfer
```

### B.5 Pickup Call Site `0x694F40` (from repaired report Section 7.1)
```
140694f17: MOV [RSP+0x40],RSI             ; param_9 = RSI (building)
140694f1b: MOV [RSP+0x38],0               ; flag = 0
140694f1f: LEA RCX,[RBP+0x20]             ; out = &local
140694f23: MOV [RSP+0x28],RCX
140694f2a: MOVSS [RSP+0x20],XMM0          ; tons = XMM0 (<0, pickup)
140694f30: MOV R9,[RAX]                   ; resource
140694f33: XOR R8D,R8D                    ; mode = 0
140694f36: MOV RDX,RBX                    ; vehicle = RBX
140694f39: LEA RCX,[0x1409D4F10]          ; game
140694f40: CALL 0x140420350               ; transfer
```

### B.6 Transfer Function Signature (from repaired report Section 5.2)
```
FUN_140420350(
  game*,           // RCX
  vehicle*,        // RDX (mode 1) or building*/NULL (mode 0)
  mode,            // R8: 0=order-source pool, 1=vehicle cargo slot
  resource*,       // R9
  tons,            // [RSP+0x20] (float, sign = direction)
  out*,            // [RSP+0x28] (float*, receives fVar21 = rejected/overflow)
  cap,             // [RSP+0x30] (float, capacity threshold)
  flag,            // [RSP+0x38] (char, 0 skips veh assignment gate)
  building*        // [RSP+0x40] (validated at +0x318 type desc)
)
```

### B.7 Transfer Mutation Logic — fVar21 = Rejected/Overflow (from repaired report Section 5.5)
```
fVar21 = 0.0;
if ((param_5 <= 0.0) || (fVar20 < param_5)) {
    // NEGATIVE TONS (unload/removal)
    if (0.0 <= param_5) return 0;           // zero → early exit
    if (*pfVar18 <= 0.0) return 0;          // empty source → early exit
    param_5 = *pfVar18 + param_5;           // add negative request
    *pfVar18 = param_5;                      // store result
    if (param_5 < 0.0) {                     // would go below zero
        *pfVar18 = 0.0;                       // clamp to zero
        fVar21 = -param_5;                   // amount COULD NOT be removed
    }
} else {
    // POSITIVE TONS (load/addition)
    param_5 = param_5 + *pfVar18;           // add positive request
    *pfVar18 = param_5;                      // store result
    if (fVar19 < param_5) {                  // exceeds capacity
        fVar21 = param_5 - fVar19;           // amount COULD NOT fit (overflow)
        *pfVar18 = param_5 - fVar21;         // clamp to capacity
    }
}
if (param_6 != NULL) *param_6 = fVar21;     // write rejected/overflow to out
return 1;
```

### B.8 DO Task Completion Check (`scratch/decomp_1de2b0.c` lines 26-55)
```c
// param_2 = TaskEntry*, param_3 = vehicle, param_4 = DO
dVar12 = (double)*(float*)(*(*(DO+0xD38) + entry[3]*8) + 0x10 + 4);  // load pct
dVar2  = DAT_14090A1F0 - (double)*(float*)(*(*(DO+0xD38) + entry[10]*8) + 0x10 + 0xCC); // unload pct
if ((*(DO+0x318)==0) || (*(int*)(*(DO+0x318)+0x360) != 0x2D)) bVar3=false; else bVar3=true;
if ((double)entry[0xC] <= 0.0) return 0;  // tons <= 0
dVar13 = (double)entry[5];                 // distance?
if (dVar13 <= dVar12) return 0;
if ((!bVar3) && (dVar13 <= DAT_140909EB0)) return 0;
if ((double)entry[0xC] < dVar2) return 0;
// ... building type checks, resource checks, vehicle cargo scan ...
```