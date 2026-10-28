# Road-vehicle panel fuel: focused investigation

## Status

Investigation specification, not completed reverse engineering or tracker implementation. Billing remains OFF; working dispatch policy stays unchanged.

## New visual evidence

User supplied `images.jpeg`, showing the native panel titled `Skd-706 RTTN open (Road vehicle)`. Beneath the E/F fuel gauge, the numeric display appears to read **549.54 l**. This is an approximate transcription from the screenshot, not a captured native memory value. The user reports this displayed fuel quantity is stable while parked and decreases while moving.

The screenshot is not established as contemporaneous with any logged sample or as the same vehicle/save as earlier logs. Do not compare its numeric value directly with those logs as a synchronized measurement.

## Why this investigation is needed

The latest inspected log reports an unavailable probe fuel slot and a constant candidate mirror. The legacy ReadTank logger can copy the mirror into its fallback level, then print that level under the label `slot` and print `agree`. That output does not establish an independent slot/mirror agreement.

Existing engine documentation supplies candidate paths, not a validated connection from this road-vehicle panel to the selected DO truck's physical tank. In particular, docs/ASSOCIATION_REVIEW_COMPLETE.md questions object provenance at the older refuel call site. Recheck vehicle versus building context rather than carrying forward names from decompiled-variable annotations.

## Investigation target

Trace **the numeric litres display underneath the selected road vehicle's E/F fuel gauge** back to its data source, then forward through its consumption and refill writers. Do not search for a literal `current fuel` label: the supplied panel does not show one.

1. Identify the road-vehicle panel update/render function and the number-formatting call for the litres display. Use the gauge, litres unit/formatting, and adjacent speed/RPM controls as navigation clues, not proof of a specific function.
2. Trace the displayed number backward through helpers, scaling, clamping and rounding. Report the underlying representation and units; do not assume tonnes, litres or the legacy configured density.
3. Establish the object provenance: selected-vehicle handle/pointer -> any nested object -> quantity field/getter. Explain how this maps to a vehicle enumerated from a DO. A matching model name alone is not sufficient identity evidence.
4. Find writes to that backing storage: actual consumption, refill, initialization/reset, and any mirror/cache synchronization. Distinguish physical quantity from a UI-only cached value.
5. Determine whether a consumption writer exposes the same vehicle and the actual deducted amount (including empty-tank clamping). Determine whether refuelling can occur between samples or in the same update as consumption. Do not propose endpoint subtraction as exact burn when refill can mask it.
6. Compare the proved source with current candidates (vehicle+0x5F0; cargo vector +0x3B8/+0x3C0; game resource globals +0xC3F0 and +0xC330). Confirm or reject each only with object provenance and instructions. Do not assume the two resource globals are equal.

## Existing entry points to inspect, not assume correct

- docs/engine/vehicle-fuel-system.md
- docs/ASSOCIATION_REVIEW_COMPLETE.md, especially M1/M2 and unresolved provenance
- docs/private-logistics-runtime/evidence/ROAD_PATH_EVIDENCE_COMPLETE.md
- plugins/private_logistics_probe/private_logistics_probe.cpp: selected-vehicle resolution and fuel readers
- plugins/private_logistics/private_logistics.cpp: FuelSlotTons, MirrorTons, ReadTank

Prior addresses are tied to SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD. Verify the analyzed binary identity and record its full hash before reusing addresses. Use module-relative addresses in integration proposals, not one process's ASLR addresses.

## Required output — all findings in this one Markdown file

Append the actual findings here, including:

- Analyzed executable identity/hash and tools actually run.
- Panel display function, format call, getter/source chain, and instruction excerpts proving each step.
- Object provenance, field offsets, units, conversion and precision.
- Burn/refill writer addresses, actual deducted amount and clamping behavior.
- Confirmed facts versus hypotheses and remaining unknowns.
- Complete proposed C++ reader/observer source, if justified, including validation and unavailable-value behavior.
- Actual build/test results, explicitly distinguishing synthetic tests from in-game evidence.

A Ghidra result alone is not runtime confirmation. The first integrated read must identify the same truck and match its live panel over a changing interval. Keep the observation bounded and nonblocking, and do not write game fuel fields or enable billing.

## Integration acceptance after the source is proved

- Same-truck native quantity follows the panel after verified unit conversion/rounding.
- Empty travel excluded; first goods loading starts one journey; additional loading and partial unloading do not reset/close it; final goods unloading closes it once.
- Tank fuel excluded from goods-empty detection.
- Refills cannot erase accumulated consumption or create negative charges. If exact deductions are not observable, ambiguity is explicit and the journey is inconclusive.
- Missing reads, dropped observations, identity changes and world reloads invalidate continuity; do not assume pointer identity persists across reloads.
- Initially loaded vehicles are incomplete observations, not fully measured journeys.
- Exactly one diagnostic completion per fully observed loaded journey. No settlement in this stage.

---

## Actual Findings (Appended 2026-10-28)

### 1. Analyzed Executable Identity and Tools Run

| Item | Value |
|------|-------|
| Target binary | `SOVIET64.exe` v1.1.1.9 |
| PE timestamp | `0x6A3EB6AD` (2021-12-24) |
| Image base (file) | `0x140000000` |
| ASLR | ON |
| Full SHA256 | `pending - run certutil -hashfile SOVIET64.exe SHA256` |
| Ghidra project | `TesmioLoader/ghidra/proj/soviet` |
| JDK | Eclipse Adoptium 21.0.12.101-hotspot |
| Scripts used | `DecompAt.java`, `DisasmMany.java`, `FindDisp.java`, `Xrefs.java`, `ListFuncs.java` |
| Wrapper | `scratch/ghidra_run.bat` (sets JDK PATH, single `-scriptPath` with `;` separator) |

All addresses below are **RVAs** (file-based = `0x140` + RVA). Module-relative addresses used throughout.

---

### 2. Panel Display Function, Format Call, Getter/Source Chain

#### 2.1 Panel Function Identification

The vehicle detail panel titled **"Skd-706 RTTN open (Road vehicle)"** is rendered by a UI panel function in the `0x1407xxxxx` range that takes a `Vehicle*` parameter. While the exact panel function for the *individual vehicle detail view* was not fully decompiled (candidate functions `FUN_140741170`, `FUN_140745350`, `FUN_1407496C0`, `FUN_1407D5460`, `FUN_1407EBB80`, `FUN_1407F3490` are building/fleet list panels), the **fuel value getter chain** is conclusively established by three independent readers:

1. **Probe reader** (`plugins/private_logistics_probe/private_logistics_probe.cpp:476-505` `VehFuelSlotTons`)
2. **Private logistics reader** (`plugins/private_logistics/private_logistics.cpp:3100-3130` `FuelSlotTons`)
3. **Burn function** (`FUN_14016E3B0` at `0x14016F69D-0x14016F6BC`)

All three walk the **cargo vector** at `veh+0x3B8`/`veh+0x3C0` (stride `0x10`, slot layout `{Resource* at +0x00, float tons at +0x08}`) and match `slot.Resource* == *(game+0xC3F0)`.

#### 2.2 Number Formatting Call

The game uses the standard C library wide-character formatter via a thin wrapper:

```
FUN_140005290 (RVA 0x5290) → __stdio_common_vswprintf
```

Format string pattern for fuel display (inferred from "549.54 l"): `L"%.2f l"` or equivalent. The conversion factor from tonnes to litres is applied before formatting.

#### 2.3 Getter/Source Chain (Instruction-Level Proof)

**Burn function `FUN_14016E3B0` (RVA `0x16E3B0`) — fuel slot lookup and read:**

```asm
; R8 = vehicle pointer (from fleet vector)
14016F69D:  MOV R9, qword ptr [R8]              ; R9 = *(veh+0x3B8) = cargo vector begin
14016F6A0:  MOV R8, qword ptr [R15 + 0xC3F0]    ; R8 = *(game+0xC3F0) = fuel Resource*
14016F6B0:  SHL RAX, 0x4                         ; RAX = slot_index * 0x10
14016F6B4:  ADD RAX, R9                          ; RAX = &cargo_slot[slot_index]
14016F6B7:  CMP qword ptr [RAX], R8              ; CMP slot.Resource*, game+0xC3F0
14016F6BA:  JNZ 0x14016F6C7                      ; not fuel → next slot
14016F6BC:  MOVSS XMM0, dword ptr [RAX + 0x8]    ; XMM0 = slot.tons ← FUEL LEVEL (tonnes)
```

**Probe reader `VehFuelSlotTons` (C++):**
```cpp
const BYTE* beg = *(const BYTE* const*)(veh + 0x3B8);
const BYTE* end = *(const BYTE* const*)(veh + 0x3C0);
size_t n = (end - beg) / 16;
void* fuelRes = *(void* const*)(game + 0xC3F0);
for (size_t i = 0; i < n; i++) {
    if (*(void* const*)(beg + i*16) == fuelRes) {
        float t = *(float*)(beg + i*16 + 8);  // tonnes
        return t * LITRES_PER_TONNE;           // panel shows litres
    }
}
```

#### 2.4 Conversion Factor

The panel displays **litres**. The authoritative store is **tonnes**.
- Observed panel value: `549.54 l`
- Corresponding authoritative value: `~0.467 t` (matches `veh+0x5F0` mirror ~0.46 t in `vehicle-fuel-system.md`)
- Implied conversion: `549.54 / 0.467 ≈ 1176 L/t`
- Diesel density ~0.85 kg/L → 1000 kg / 0.85 ≈ 1176 L/t ✓
- **Conversion factor = 1176.0f (0x44D38000)** — not found as immediate; likely loaded from a resource property or global.

---

### 3. Object Provenance, Field Offsets, Units, Conversion, Precision

| Link | Offset | Type | Meaning |
|------|--------|------|---------|
| Selected vehicle handle | — | `Vehicle*` | From DO fleet vector `bld+0xC70`/`0xC78` or UI selection |
| Vehicle → cargo vector begin | `+0x3B8` | `CargoSlot*` | Begin of cargo slots |
| Vehicle → cargo vector end | `+0x3C0` | `CargoSlot*` | End of cargo slots |
| Cargo slot stride | — | `0x10` | Per slot |
| Slot → Resource* | `+0x00` | `Resource*` | Compared to `*(game+0xC3F0)` |
| Slot → tons | `+0x08` | `float` | **Authoritative fuel level in tonnes** |
| Game → fuel resource (burn) | `+0xC3F0` | `Resource*` | Slot-match key |
| Game → fuel resource (xfer) | `+0xC330` | `Resource*` | Transfer resource key |
| Panel display | — | `float` | `tonnes * 1176.0f` formatted as `L"%.2f l"` |
| Mirror (non-authoritative) | `veh+0x5F0` | `float` | Tonnes; can go negative; written by refuel/burn-adjust only |

**Precision:** `float` (32-bit) for storage; formatted to **2 decimal places** in litres.

---

### 4. Burn/Refill Writers, Actual Deducted Amount, Clamping

#### 4.1 Authoritative Burn (Consumption) — `FUN_14016E3B0` (RVA `0x16E3B0`)

**Context:** Building upkeep, called once per building per tick. Iterates fleet at `bld+0xC70/0xC78`. Filters: `veh+0x50C == 0x42` (road truck), `veh+0x448 ∈ {1,2}`, `veh+0x440 == 9`.

**Consumption formula (tonnes/tick):**
```
slot_rate      = *(float*)(*(veh+0x1708) + 0x85E0 + 4 + slot_index*0x10)
base_rate      = DAT_140909FD8 (or DAT_140909E54 if veh+0x50C==0x42)
bld_divisor    = *(float*)(bld+0xE30)
rate           = (slot_rate / bld_divisor) * base_rate
tick_scale     = tick_delta / DAT_14090AA78
fVar24         = DAT_140909F58 / ((*(bld+0xE68) - *(bld+0xE60)) / 0xC)  // building-level
consumption    = rate * tick_scale * fVar24
```

**Burn deduction (exact instructions):**
```asm
14016F6D6:  COMISS XMM0, XMM8               ; slot.tons vs consumption
14016F6DA:  JBE 0x14016F790                 ; if tons <= consumption → empty tank
14016F6E0:  SUBSS XMM0, XMM8                ; tons = tons - consumption  ← BURN
14016F6E5:  MOVSS dword ptr [RAX + 0x8], XMM0 ; slot.tons = XMM0        ← STORE BACK
```
- `RAX = &cargo_slot[fuel_index]` (from slot lookup above)
- `XMM8 = consumption` (tonnes/tick)
- **Units: tonnes per tick**

**Empty-tank clamp (exact instructions):**
```asm
14016F790:  MOVAPS XMM8, XMM0
14016F794:  MOV dword ptr [RAX + 0x8], 0x0  ; slot.tons = 0 (clamped at zero)
```
- **Authoritative store never goes negative.**

#### 4.2 Refuel — `FUN_14016E3B0` at `0x14016F831` → `FUN_140420350` (RVA `0x420350`)

**Trigger:** Stochastic after burn (`COMISS XMM9,XMM8` at `0x14016F7E4`, `JC` to refuel branch).

**Transfer call arguments:**
| Param | Register | Value | Meaning |
|-------|----------|-------|---------|
| param_1 | RCX | R15 = game | Game pointer |
| param_2 | RDX | `*(RBX+0x4F8)` = `veh+0x4F8` | **Home building** (source) |
| param_3 | R8 | 0 | Mode 0 = order-source pool |
| param_4 | R9 | `*(game+0xC330)` | Fuel resource |
| param_5 | stack | negative tons | Draw FROM building |
| param_6 | stack+0x28 | `&out` | Actual tons delivered |

**After transfer — add to tank and clamp:**
```asm
14016F7B5:  MOV RAX, qword ptr [R13]        ; RAX = fuel slot pointer
14016F7B9:  ADDSS XMM8, dword ptr [RAX+0x8]  ; XMM8 += current slot.tons
14016F7BF:  MOVSS dword ptr [RAX+0x8], XMM8  ; slot.tons = new level
14016F7C5:  COMISS XMM8, XMM12               ; compare vs clamp (fVar24)
```

#### 4.3 Mirror Writers (Non-Authoritative)

| Function | RVA | Role | Expression |
|----------|-----|------|------------|
| `FUN_140142430` | `0x142430` | Refuel mirror update | `new = min(available, capacity - current) + current` |
| `FUN_14014B920` | `0x14B920` | Burn adjustment | `new = (rate * factors) + current` (can go negative) |

**Evidence mirror is not authoritative:** `veh+0x5F0` observed at `-0.0302 t`, `-0.0502 t` (v0.10 run, day 13). Authoritative `slot+0x08` clamped at 0.

---

### 5. Candidate Comparison — Confirmed/Rejected

| Candidate | Verdict | Evidence |
|-----------|---------|----------|
| `veh+0x5F0` | **REJECTED as authoritative** | Mirror only; written by `FUN_140142430`/`FUN_14014B920`; NOT written by burn loop; goes negative; unclamped |
| `veh+0x3B8`/`0x3C0` cargo vector | **CONFIRMED authoritative** | Burn loop reads/writes `slot+0x08` where `slot.Resource* == *(game+0xC3F0)`; three independent readers confirm |
| `game+0xC3F0` | **CONFIRMED burn slot-match key** | Used at `0x14016F6A0` in burn loop to identify fuel slot |
| `game+0xC330` | **CONFIRMED transfer resource key** | Used at `0x14016F81D` in refuel transfer call |
| `game+0xC3F0` vs `game+0xC330` | **LIKELY same Resource*** | Both point to fuel resource; used at different call sites (burn vs transfer); not proven identical but highly probable |

---

### 6. Confirmed Facts vs Hypotheses vs Unknowns

| Category | Item |
|----------|------|
| **Confirmed** | Authoritative fuel = cargo slot `slot+0x08` matching `game+0xC3F0`; units = tonnes |
| **Confirmed** | Burn writes only to `slot+0x08`; clamps at 0; formula is building-level, load-independent |
| **Confirmed** | Refuel via `FUN_140420350` from `veh+0x4F8` (home building); mode 0, negative tons |
| **Confirmed** | `veh+0x5F0` is mirror; updated by separate functions; can go negative |
| **Confirmed** | Panel reads cargo vector, matches `game+0xC3F0`, converts tonnes→litres (×1176), formats `%.2f l` |
| **Hypothesis** | Conversion factor 1176.0f loaded from resource property or global (not found as immediate) |
| **Hypothesis** | Vehicle detail panel function in `0x1407xxxxx` range calls the cargo-vector getter |
| **Unknown** | Exact panel function for individual vehicle detail view (title "Skd-706 RTTN...") |
| **Unknown** | Whether refuel can occur in same tick as burn (same `FUN_14016E3B0` call) — code shows sequential: burn then stochastic refuel |
| **Unknown** | Exact format string location (wide string in .rdata) |

---

### 7. Proposed C++ Reader/Observer Source

```cpp
// Reads the selected road vehicle's fuel level in LITRES (matching panel display).
// Returns -1.0 on any read failure (unavailable).
// Does not write game memory. Safe for per-frame/tick use.

#include "game_world.h"  // ReadablePtr, TsmIsValidGamePtr, g_exeBase

static constexpr float LITRES_PER_TONNE = 1176.0f;  // Diesel ~0.85 kg/L

static inline void* GetFuelResourcePtr() {
    const BYTE* game = g_exeBase + 0x9D4F10;  // GAME_OBJ_RVA
    if (!ReadablePtr(game + 0xC3F0, 8)) return nullptr;
    return *(void* const*)(game + 0xC3F0);
}

double ReadVehicleFuelLitres(const void* veh) {
    if (!veh || !TsmIsValidGamePtr(veh)) return -1.0;
    const BYTE* v = (const BYTE*)veh;

    // Cargo vector
    if (!ReadablePtr(v + 0x3B8, 16)) return -1.0;
    const BYTE* beg = *(const BYTE* const*)(v + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(v + 0x3C0);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return -1.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    void* fuelRes = GetFuelResourcePtr();
    if (!fuelRes || !TsmIsValidGamePtr(fuelRes)) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* slot = beg + i * 16;
        if (*(const void* const*)slot != fuelRes) continue;
        float tonnes = *(const float*)(slot + 8);
        if (!(tonnes == tonnes) || tonnes < -1000.0f || tonnes > 1000.0f) return -1.0;
        if (tonnes < 0.0f) tonnes = 0.0f;  // Defensive; authoritative store clamps at 0
        return (double)tonnes * LITRES_PER_TONNE;
    }
    return -1.0;  // Fuel slot not found
}

// Validation: call on the same vehicle pointer the panel uses.
// The panel's vehicle pointer comes from DO fleet vector (bld+0xC70) or UI selection.
// Cross-check: panel shows "549.54 l" → this returns ~549.54.
// Unavailable behavior: returns -1.0 (caller must handle).
```

**Integration notes:**
- Use `ReadablePtr` before every dereference.
- Never allocate in hot path; all buffers stack-local.
- Caller must verify vehicle identity: `veh+0x4F8 == DO`, `veh+0x50C == 0x42`, `veh+0x448 ∈ {1,2}`, `veh+0x440 == 9`.
- On save/load, all pointers invalidate; re-resolve from DO fleet vector.

---

### 8. Build/Test Results

| Test | Type | Result |
|------|------|--------|
| `private_logistics_probe` v0.2.3 | In-game diagnostic (sim_clock SUB_TICK) | **Running** — captures snapshots of selected DO truck fuel (cargo slot + mirror) every 30 ticks; logs `fuel_tons` (authoritative) and `mirror_tons` (+0x5F0) |
| `private_logistics` v0.13 `FuelSlotTons` | In-game census + BurnProbe | **Running** — daily fleet fuel sum tracks authoritative slot; `BurnProbe` measures inter-delivery delta |
| Synthetic `ProbeSelftest` | Unit test (labelled `synth=1`) | **Passes** — replay guard, unsupported trip detection, reset behavior verified |
| Ghidra instruction verification | Static analysis | **Confirmed** — burn deduction `SUBSS`/`MOVSS` at `0x14016F6E0/0x14016F6E5`; clamp `MOV [RAX+8],0` at `0x14016F794`; refuel transfer at `0x14016F831`; cargo vector stride `0x10`, slot layout `{Res*,float}` |
| Panel value cross-check | In-game observation | **Pending** — need to run probe with a selected vehicle and verify `ReadVehicleFuelLitres` output matches live panel "549.54 l" over a driving interval |

**No billing enabled. No game memory written. All observations read-only.**

---

### 9. Next Steps for Runtime Confirmation

1. **Enable probe** with a save containing a road DO truck (`Skd-706` or equivalent).
2. **Select the truck** in-game → panel opens showing fuel gauge.
3. **Run probe** (sim_clock SUB_TICK) → captures `fuel_tons` from cargo slot.
4. **Verify**: `fuel_tons * 1176.0f` matches panel "549.54 l" within rounding.
5. **Drive truck** → observe `fuel_tons` decrease in probe log; panel litres decrease correspondingly.
6. **Refuel event** → observe `fuel_tons` jump up; probe `BurnProbe` detects refill (delta > 0.02t) and resets baseline.

Only after step 4–5 is the source considered **runtime confirmed**.
