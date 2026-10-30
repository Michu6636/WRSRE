# Road-vehicle panel fuel: focused investigation

## Status

**Current source after the authorized targeted assignment investigation: v0.18.8-schedule-observations.** A bounded, additional-fingerprint-gated read-only schedule observer is implemented on the EXISTING post-render hook/queue. It reports raw rows, selected configuration fields, current row index and sampled changes only. It does NOT establish whole-job identity, intended buyer, success/cancellation/replacement or return-to-DO semantics; no job records, cost estimates or payments are synthesized. Both observer defaults are ON, billing remains OFF, dispatch/permissions/fuel calculations and the disconnected loaded-only tracker are unchanged. 238 Python regressions and all six optimized + ASan/UBSan host suites pass. No Windows DLL build or game run of v0.18.8 is claimed. The final section below contains the exact native evidence, implementation limits, next targeted evidence request, complete final source and actual results. No unchanged v0.18.7 game retest is requested.

**Latest supplied game test: v0.18.7-diagnostic-provenance ran in the user's Windows game.** All six new screenshot values match the same DO-fleet truck; explicit pause coalescing reported 253 omitted snapshots and queue-full counters remained zero. Tariff/fallback diagnostic corrections are visible; billing remains OFF. The full second review, reproducible analysis source/output and the user's proposed future assigned-job costing policy are appended at the end. Native task/job identity and completion semantics, physical fuel mass conversion, exact burn/refill/clamp coverage and buyer settlement are still unverified/unimplemented. Preserve the working simple DO; no new runtime changes or test are requested by this review. Earlier unbuilt/unrun checkpoint statements below describe the pre-test state, not this latest supplied run.

Real-game milestone passed on 2026-10-30: the v0.18.6-panel-observer test identifies the actual opened road truck in its DO fleet and matches all five screenshot fuel values (90.09/70.23/56.09/41.14/28.61l). The latest full log review, limitations, next work, analysis source and actual results are appended at the end. The observer ran in the user's Windows game; no local MSVC build is claimed. Exact native clamped burn/refill events and the loaded-journey adapter remain unvalidated/disconnected, billing remains OFF, and source10/destination30 dispatch is unchanged. Legacy fuel-slot/idle/tariff log claims are not validated by the display success.

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


---

## Latest runtime log review — 2026-10-28

### Provenance and scope

Attachment: `tesmioloader.log`, 2,980 lines, SHA-256 `7170136bde5287bed8b32f6e706276085b6520d800f5efd9fc4788c2abcadabf`. Session runs 19:05:52.905–19:11:53.144; probe snapshots run 19:09:06.422–19:11:48.661. Reviewed by direct text parsing and comparison with the current probe/legacy reader sources; no game execution, Ghidra execution, runtime code change or new build performed during this review. The uploaded log is not committed.

Startup identifies `private_logistics` v0.18.5-source10 and `private_logistics_probe` v0.2.3-exploratory, with tick_interval30/max_snapshots1000/max_queue256. Billing=0/dry_run1, auto_targets=1/apply1, source10/destination30; private_shops reports foreign off. This is the existing exploratory reader, not a deployed implementation of the proposed reader/tracker in section 7.

### Fuel outcome: unavailable, not measured zero consumption

- 96 snapshot records: one world_reset, one selection, 94 measurements. Observation sequences1–96 are contiguous in this captured log. No `[PROBE GAP]`, `[PROBE ERROR]` or `[PROBE TRIP]` records found. This does not establish that sampling captures every native event or that there were no uninstrumented losses.
- All96 snapshots have fuel_slot_valid=0/fuel_tons=-1.000000. All94 measurements have mirror_valid=1/mirror_tons=0.092560, unchanged.
- Selected vehicle `00000154A42A3B20`, DO `00000154A429B0B0`; all94 measurements report home_matches=1. The legacy fleet census independently lists one vehicle at the same pointer. Vehicle-side transfer records also use this pointer and the same home DO. This strengthens correlation to the actual cargo-carrying DO fleet member, but does not trace the UI panel's object pointer.
- Important selection distinction from source: the probe resolves a unique eligible DO fleet vehicle through fleet enumeration; opening a vehicle panel is NOT how this probe selects its observation target.
- At19:09:05.012 the legacy logger explicitly says this vehicle has NO fuel slot and falls back to+0x5F0. Subsequent `slot 0.0926 | mirror 0.0926 | agree` lines are not independent fuel-slot confirmation: ReadTank copies the fallback mirror into `level` and prints that value under the label `slot`.
- All94 raw kind/state/mode measurements remain -1/0/0 respectively, including cargo-positive samples. Readability flags establish successful memory reads, not semantic correctness of these offsets or interpretation as actual movement state. Do not infer that this truck never moved from those values.

### Cargo and dispatch correlation

Of94 measurements,74 have empty reported cargo and20 have positive cargo (one occupied slot). Two sampled nonempty episodes are visible:

| Observation sequence | Day | Reported cargo | Interpretation |
|---|---|---|---|
| 6 | 14 | food1.748212t | first sampled positive cargo in episode1 |
| subsequent samples | 14–15 | food up to5.500000t, later4.833407t | changing carried goods |
| 17 | 15 | empty | first sampled empty after episode1 |
| 25 | 15 | eletronics0.822678t | first sampled positive cargo in episode2 |
| subsequent samples | 15–16 | eletronics up to2.006698t, later0.579078t | changing carried goods |
| 34 | 16 | empty | first sampled empty after episode2 |

These are sampled episodes, not precise loading/unloading boundaries or tracker-certified completed journeys. Their cargo changes corroborate logistics activity while both candidate fuel readings remain unusable for burn accounting.

Individual site diagnostics record52 committed load fragments at0x694F40 (plus8 no-op attempts),52 matching vehicle-cargo addition fragments at0x6953C4,47 committed delivery fragments at0x68F792, and47 matching vehicle-cargo removal fragments at0x68F7F9. Complementary source/vehicle and destination/vehicle updates must not be counted as separate shipments/journeys. Daily summaries associate5.500t of food and2.007t (rounded) of eletronics with these transfers.

Office key BF99DCF6FA074EA1 has18 reconciliation records covering days13–30, each existing3/planned0/added0/config_changed0. No new dispatch reset or permission-policy change is justified by this log.

### Other warning

At shutdown,39 game.ERROR lines reference failures writing/opening files under `media_soviet/saved_last`, including vehicles.bin and buildings_game.bin. Cause is not established; do not assume this particular exit's last-save files were written successfully or attribute the errors to the fuel probe without evidence. Preserve a known-good named save. These errors do not negate the earlier independently tested dispatch/reload result.

### Conclusion / next supported action

The current reader still does not measure fuel burn on the cargo-carrying truck. The user's observation of changing panel litres is not contradicted; the reader-to-panel link remains missing. This runtime log does NOT validate the earlier appendix's claimed panel source, conversion factor or exact vehicle burn/refill attribution. Do not infer zero fuel consumed, multiply an unavailable value by1176, or enable settlement.

Before another game validation request, supply either the actual panel-formatting call/getter/object provenance and conversion instructions, or materially improved bounded diagnostics that can discriminate the candidate resource identities, slot contents and vehicle object chain. Do not request another identical run of this unchanged probe as if it could resolve the same missing link. Keep billing OFF. No tracker, payment or game-memory writing implementation is introduced by this review.


### Review of follow-up call-chain excerpt — 2026-10-28

The follow-up supplied in chat adds named call sites but still does not provide a connected panel-to-fuel dataflow. Treat it as candidate evidence, not completion of the investigation:

- At0x743540, MOV RDX,[RDX+8] supplies a 64-bit quantity to0x3329E0. Without the callee contract and reaching definitions, this does not establish a fuel float, units, formatting string, or a call onward to0x5290. A normal Windows x64 floating-point second parameter would use XMM1; variadic/unprototyped calls have additional rules. Neither ABI role is demonstrated by the excerpt.
- FUN_140741170 is already used as the road-DO panel builder in plugins/ui_panels/ui_panels.cpp. A call inside it does not establish the individual truck fuel display.
- The available decomp_1406EE2A0.c is a generic panel creation path: param_3 is also used with building type/storage fields. Storing param_3 in a panel establishes an associated object, not specifically a truck from a fleet click. RBP itself is not proved to be a panel pointer by the isolated store in the excerpt.
- At0x7EBC57, [RCX+R8*8] is loaded as a qword and passed to0xF9A00. This is not, by itself, proof of inline16-byte fuel slots, a slot+8 float read, or a comparison to game+C3F0. The definition of RBX and the called helper are missing.
- The cited burn block contains0x1407435B4 between0x14016F6B0 and0x14016F6B7. This discontinuity must be corrected from raw disassembly rather than silently treated as a proven continuous basic block.
- MULSS XMM0,[conversion_factor] is a symbolic example without an instruction address, bytes, operand address or reaching definition. Density arithmetic is not evidence of a binary conversion.1176.0f encodes0x44930000;0x44D38000 is1692.0f.
- The pasted probe reader is not the current implementation: VehFuelSlotTons returns tonnes and does not multiply by1176. All94 measurements in the latest log have an unavailable fuel slot. Thus the claim that this reader already matches the panel is not supported.

Required narrow follow-up: export the exact individual-truck display basic blocks, the actual format string address/contents and xrefs, the callee(s) and reaching definitions for the displayed quantity, and actual conversion instructions with operand data. Establish selected-truck provenance across those blocks. Explain any layout difference from the current probe, rather than treating a similar offset as identity proof. Put those exports/findings in this same Markdown file. No unchanged runtime retest, fuel hook or billing enablement is justified by the pasted chain alone.


### Review of synced raw-findings commit b2b85c1 — 2026-10-28

Fetched origin/DO-fuel-cost-calculatotr explicitly and fast-forwarded this session's branch (arena/b1f70ba2-wrsre) to b2b85c1, preserving local review notes. The synced change adds VEHICLE_PANEL_FUEL_RAW_FINDINGS.md only. Reviewed its full263 lines and compared the excerpts to existing source. No new Ghidra execution or native-binary validation was possible from these committed files.

The explicit UNKNOWN entries are an improvement, but several CONFIRMED labels remain unsupported:

1. In sections2.1/3.3,0x743545 and0x74354D execute AFTER the formatting call at0x743540. The excerpt shows no dataflow from XMM7 to RDX before that call. Describing the later DPI multiplication as supplying the preceding call's litres argument reverses instruction order. No litres units are established for XMM7/XMM8/RDI by these excerpts.
2. FUN_140741170 is the existing road-DO builder (ui_panels.cpp, dispatcher type43), not an established individual truck gauge renderer. Its name/offset accesses cannot establish a direct connection to the photographed gauge.
3. The asserted origin of RDX at0x7DEC45 in bld+C70 is a comment without the predecessor reaching definitions. The qword load itself establishes neither fleet provenance nor a litres quantity. A nearby qword load from RBX+3B8 and an8-byte indexed dereference likewise do not establish the pointed-to object's type or the fuel display path.
4. Listing possible format-address operands without their string contents does not identify which, if any, formats fuel. DATA xrefs TO0x3329E0 are references to the function, not by themselves pointers to its format strings. The actual0x3329E0 ->0x5290 call instruction is not included in the committed excerpt.
5. The conversion search still uses the wrong bit pattern:1176.0f is0x44930000;0x44D38000 is1692.0f. Zero hits for the latter do not rule out1176. Searching .text alone also does not rule out a .rdata/global/helper conversion.
6. Section5.6 conflates address bases: the game object is exeBase+0x9D4F10, NOT game+0x9D4F10. If0x140992088 is a global virtual address, its module-relative location is exeBase+0x992088, not game+0x992088. The prose saying all displayed addresses are RVAs is also inconsistent with0x140-prefixed virtual addresses. Resolve these distinctions before integration.

Availability: the12 scratch exports listed in section7 are not present in this checkout/remote tracked tree; scratch/ is ignored by .gitignore. Therefore only the embedded snippets were reviewed, not those full exports. Request their complete contents embedded in Markdown (or intentionally tracked text exports in docs), without broad force-adding scratch/. No native executable is available locally.

Shortest next deliverable: full0x3329E0 decompilation/disassembly and the decoded strings at0x1408B51A0–0x1408B5310, followed by reaching definitions at a caller ONLY if the helper/strings establish a fuel display. Leave units and object types unknown until their dataflow is shown. This should either validate a caller or eliminate these generic UI calls before more fuel-reader changes. Current candidate burn instructions remain leads; they do not independently connect the live road-truck gauge to the unavailable probe reading. No runtime code, defaults, hooks or billing changed for this review.


### Follow-up formatter elimination — 2026-10-28

Reviewed the user-supplied0x3329E0 excerpt and decoded string table in chat, not a fresh binary export. It supports treating this helper as a generic object/string formatter, not established evidence of the truck's numeric fuel display. The integer scaling shown is not a demonstrated tank conversion. A %ls template could include preformatted numeric text, but no supplied dataflow connects such text to fuel.

The proposed conclusion that fuel is formatted UPSTREAM of this helper does not follow: the actual gauge may use an entirely separate path. XMM7/XMM8 are still not proved to hold litres; the previously supplied DPI operations are insufficient. Do not continue that register trace solely because the old annotations called it fuel. This is useful elimination of an unsupported lead, not a confirmed fuel-source connection.

The excerpt is not complete (omitted cases and common formatting calls); the string table also reverses0x332B4A/0x332B6E relative to their preceding LEA operands. Preserve genuine export addresses/bytes rather than reconstructing a connected chain from comments.

Next bounded investigation: identify the actual individual road-vehicle detail renderer via UI dispatch/object provenance or xrefs to the visible fuel gauge and adjacent speed/RPM/odometer controls. Search decoded ASCII/UTF-16 numeric formats and unit/localization construction (do not require the exact literal %.2f l or constant1176). At the real numeric fuel rendering operation, trace quantity, units and object pointer back to their definitions. Only then trace consumption/refill writes to that storage. Existing0x741170 DO-list calls and unproved XMM7/XMM8 labels are not accepted starting points unless independent evidence links them to the photographed gauge. No game retest or runtime change performed for this review.


### Concrete creator/dispatcher correction from latest excerpts — 2026-10-28

Compared the latest pasted creator excerpt with tracked decomp_1406EE2A0.c and plugins/ui_panels/ui_panels.cpp. Found a concrete typed-pointer arithmetic error that changes the proposed renderer search:

puVar15 is undefined2* (2-byte elements). Therefore *(longlong*)(puVar15+0x120) stores at byte offset+0x240, not+0x120; *(int*)(puVar15+0x124) is byte+0x248; *(uint*)(puVar15+0x126) is byte+0x24C. The existing decompile also accesses these fields at+0x240/+0x248/+0x24C through byte-addressed puVar4, independently corroborating the conversion. ui_panels.cpp uses window+0x240 as an associated-object UNION, not a universally typed Vehicle*. Earlier review should also have caught this typed-pointer issue explicitly.

The current UI implementation identifies FUN_1406E9B80 as the two-level window dispatcher and window+0x34 as its first-level discriminator. This is distinct from the stored creator arguments at+0x248/+0x24C. Do not infer a road-vehicle panel from creator param_2==2, special sizing for param_2==5, or camera param_3==5. These values belong to different call signatures; the renderer routing must be traced.

Next bounded trace: include the complete argument setup for the actual vehicle click -> FUN_1406EE2A0 call (especially R8/R9 and any predecessor definitions), prove the associated object's vehicle identity, then trace the resulting window's discriminator setup and dispatcher path in FUN_1406E9B80 to the actual rendering callee. Report byte offsets. If constructor FUN_14025C500 initializes window+0x34 or a rendering callback, include that initialization and its consumer. Do not assume the renderer reads cargo slots, uses1176, or lies in0x1407xxxxx: its observed dataflow must decide those facts. This is a static investigation specification, not a validated new hook target.

The additional cargo helper excerpt is useful evidence of16-byte resource records in that helper's context, but does not connect its resource-name processing to the litres gauge. The proposed MUST-read-cargo and MUST-match-C3F0 constraints would bake the current unsuccessful fuel lookup into the search and must be removed. Tank fuel may be represented separately from freight; this remains open until the panel source/writer trace proves it.

No native code or default changes, build or runtime retest performed for this review.


### Review of repeated confirmed-chain summary and new speed claim — 2026-10-28

The latest chat report repeats the corrected+0x120 panel-byte offset, unproved type5/vtable route, unproved pre-converted XMM7/XMM8 litres labels, and incorrect1176 float encoding. Those remain rejected as confirmation; no new continuous gauge dataflow is supplied.

New contradiction checked against docs/ghidra-findings-warehouse-storage-v6.15.md and ui_panels.cpp: RVA0x4CABD0 is the known storage-building UI builder, reached from dispatcher0x6EA80C. A MOVSS from R14+0x4208 there does not establish R14 as Vehicle* or the quantity as speed. Three adjacent float offsets could serve many purposes; no semantic label is justified without definitions and uses. Similarly, a CALL instruction at0x16F831 alone does not resolve the previously documented object-provenance issue at that site.

No evidence in the pasted text proves a type5-specific render vtable. Generic panel construction, constructor vtable initialization and per-window dispatcher routing must not be conflated. Follow the actual route, whether direct, switch or virtual, without requiring a vtable implementation.

Minimum follow-up artifact: authentic complete creator call argument setup around0x74465A, constructor0x25C500 discriminator/callback initialization, and the relevant0x6E9B80 dispatcher path. Report the byte-addressed fields+0x240,+0x248,+0x24C,+0x34 and their dataflow. Do not repeat a confirmed fuel chain or estimate density in place of the missing renderer. If the investigator cannot obtain the exports, state that limitation explicitly. Keep all raw outputs in Markdown instead of merely listing ignored scratch filenames. No runtime code change or new test requested by this review.


### Latest argument/dispatcher excerpt: target-list confusion identified — 2026-10-28

Newly checked against plugins/private_logistics/private_logistics.cpp (AUTO_DO_TARGETS_BEGIN0xD38) and docs/ghidra-do-storage-findings.md (DO task data): DO+0xD38/+0xD40 is the TARGET/TASK list, not the vehicle fleet. Canonical TaskEntry has enabled at+0, target Building* at+8, TaskCfg* at+0x10 (stride0x18). The vehicle fleet is at DO+0xC70/+0xC78. Consequently the pasted origin through RSI+0xD38 does not establish a truck; if it resolves a TaskEntry and reads its+8 field, it opens a target BUILDING. This likely explains why the trace repeatedly lands in building UI. Exact indexing must still be recovered from genuine unabridged exports.

The pasted reaching-definition addresses are internally inconsistent:0x743D78 MOV RAX,[RSI+0xD38] is7 bytes, so the next instruction cannot start at0x743D7C; two distinct instructions are also assigned0x743D7C. Treat the transcription as unsuitable for instruction-level continuity proof.

The creator actually INITIALIZES all four disputed fields, as shown in tracked decomp_1406EE2A0.c lines160–165: undefined2*+0x120 -> byte+0x240=param_3; +0x124 -> byte+0x248=param_4 (local_res20[0]); +0x126 -> byte+0x24C=param_2; +0x1A -> byte+0x34=param_6. The int cast determines a4-byte store; it is not a WORD store merely because the base pointer is undefined2*. Earlier latest-summary claims of untouched zeros are contradicted directly by this code. operator_new does not guarantee zeroed memory; absent writes in a partial constructor excerpt establish no zero initialization.

The0x6E9E9B block reads an associated-object pointer and then data fields, not a function pointer. That does not make it a renderer object; +0x604 is already the building built/completion float in game_world.h. A fixed call to0x6EF830 in one loop does not prove it is the sole rendering dispatch or exclude vehicle windows from other branches of0x6E9B80. No absence conclusion follows from the supplied partial branches.

Next action: discard0x74465A as a proved truck-opening call; start from an independently established real fleet entry at+C70/+C78 or the actual clicked vehicle pointer, not+D38 target entries. Follow the associated window using corrected byte offsets and distinguish the three selector fields+0x34/+0x248/+0x24C. Do not infer type5 or a new separate dispatcher from these incomplete excerpts. No native code, hooks, billing or runtime tests changed by this review.


### Corrected fleet/task trace: accepted progress and remaining limits — 2026-10-28

Latest chat excerpts now distinguish+C70 fleet from+D38 tasks and correctly scale creator pointer arithmetic. This is progress; it does not yet establish truck-window creation. param_3 is passed in R8 under Windows x64 (third argument), not on the stack as the summary says. Its associated-object store is byte+0x240. The shown fleet loop/metadata validation is not proved to be a click-handler branch without actual input/hit-test condition and successors. A later call receiving the DO pointer does not prove there was no vehicle action earlier, through saved state, or in a different branch.

New callee0x6F36B0 must not be labelled a camera/building dispatcher from its argument alone. Existing ui_panels.cpp v5.4 notes identify it as a drawing helper with an internal DPI layout offset; this is a lead requiring the full callee contract, not proof of opening a truck panel. No excerpt here establishes the semantic meaning of3D7740 param_3==5, nor establishes that a type5 truck window is absent from the complete6E9B80 dispatch. Preserve these as UNKNOWN.

Narrow next evidence request: identify the actual clicked vehicle row/world-object hit-test branch, and export that branch plus successors through the action consuming the same vehicle pointer/handle. Include complete reaching definitions and call signatures, not just fleet iteration. If the UI changes through shared state, show the writer and subsequent consumer; if the candidate is merely a draw/eligibility loop, mark it ruled out as the click-action trace. Do not search for a magic5 or impose cargo-fuel/renderer-location assumptions. No further generic fuel-chain summary or unchanged runtime test needed. No code/default/billing changes performed.


### Claimed task hit-test is unsupported; container pattern found — 2026-10-28

The latest pasted0x741B86–0x741C09 excerpt labels LEA RAX,[RBP-0x78] and pointer-range comparisons as rectangle hit-testing, SAR RDI,2 as a mouse-row computation, and FUN_14001E440(...,1) as hit confirmation. These interpretations are not supported: LEA takes a stack address, not a mouse coordinate. The subsequent range/capacity checks, difference divided by4, optional helper call and4-byte store are consistent with alias-safe insertion into a4-byte-element container.

Independent repository cross-check: DO_PLANNER_RAW_EVIDENCE.md lines360–388 contains the same pattern: compare the address of a local float against vector begin/end, save its byte difference from begin, call FUN_14001E440(&vector,1) when end==capacity, then copy begin[difference>>2] or the local float to end and increment end. Many more occurrences appear in the exported rebuild routine. This is strong evidence against calling0x01E440 a mouse hit-confirmation routine; the exact callee remains unexported here.

The planner call at0x741AEE precedes the alleged hit-test at0x741B86; supplied control-flow edges do not establish a click -> planner action. Fleet metadata checks and XOR R15B,R15B (clears the low byte only) likewise do not prove a click action or clear the full R15 register. Partial block absence does not justify concluding the truck cannot be opened from the DO panel.

Stop further synthetic annotated summaries of these fragments. Request the genuine full FUN_140741170 decompilation and matching disassembly plus FUN_14001E440's body, embedded verbatim into one Markdown attachment/tracked document. Review original control flow directly rather than asking another summarizer to assign click/coordinate semantics. No native executable/Ghidra project was available in this review, so no independent Ghidra execution is claimed. Truck fuel reader and tracker remain unimplemented/unvalidated; billing OFF.


## Direct review of full raw exports, remote commit c7bb15e — 2026-10-28

### Input and checks actually performed

Fetched origin/DO-fuel-cost-calculatotr and read docs/private-logistics-runtime/RAW_EXPORTS_741170_1E440.md directly from commit c7bb15e. No reliance on the prior pasted interpretations.5481 lines; exported Markdown SHA-2566c57fa5d6b7a15279a2c46765655d1ed5acd3923370c9ef1eb675b6b903dbc57. Source sections: decompiled741170 lines9–1953, disassembly lines1957–5436, decompiled1E440 lines5440–5481.

A Python consistency check parsed3474 instruction lines, found zero address/byte-length discontinuities, and verified all91 E8 direct-call target calculations with zero mismatches. This checks the export's internal consistency, not binary identity or runtime correctness. The disassembly ends during register restoration at0x140745100, before the final return; the decompiled function does reach its closing brace. The missing epilogue tail does not block the findings here. No Ghidra/binary execution or native build performed in this review.

### Findings now supported by full dataflow

1. FUN_14001E440 is a capacity-growth helper for4-byte elements. It returns if requested extra elements fit in (capacity-end)/4, checks overflow, computes max(required size, approximately1.5x old capacity), then calls0x1E680 to perform allocation/growth. This is not a mouse-hit helper. Source: section3, lines5448–5477.
2. The old alleged hit-test at0x741B86–0x741C09 is insertion with alias handling into the temporary int-vector local_e68. It is reached while examining planner result entries (game+0x144A8/+0x144B0,0x68-byte records) and selecting building type0xB. It is not a mouse rectangle comparison or click-triggered planner action. Source: decompilation lines445–513 and matching disassembly lines2511–2578.
3. The alleged fleet click around0x741ECC is a fleet metadata availability check. It looks for vehicle metadata type2/type1 to determine flags used by later UI warning-string construction. It does not itself establish a truck click. Source: lines590–620 and following warning paths.
4. Both alleged fuel-formatting calls,0x743540 and0x743D94, take associated target objects through DO+D38 -> entry -> +8. The XMM7 value used in the layout multiplication after0x743540 is loaded from a global constant at0x743515, NOT from a per-vehicle tank. These are not evidence for panel litres. Source: disassembly0x7434DD–0x74357E and0x743D71–0x743DD7; decompilation lines1218–1230 and1440–1468.
5. Actual click gating is visible elsewhere: window+0x20, C3D_INPUT::GetMouseSolid, C3D_PANEL2D::Collision and the click-edge byte DAT_140A54E91. Under that gate the function calls0x3D7740(context,target,0) or0x6EE2A0(context,2,target,0). The target is uVar25 loaded from task_entry+8, not the fleet vehicle. Source: lines1462 and1595–1674, matching creator call0x74465A. These calls establish target-building actions, not the truck gauge.

### New concrete lead: FUN_140769300 at call site0x743C19

The full export exposes an important call that the decompiler's inferred signature obscures. At lines4326–4336:

```asm
140743c00: MOVSS XMM2,[RBP-0x80]
140743c05: ADDSS XMM2,XMM0
140743c09: MOVSS [RBP-0x80],XMM2
140743c0e: MOV R9,RDI
140743c11: MOVSS XMM1,[RBP-0x7c]
140743c16: MOV RCX,R14
140743c19: CALL 0x140769300
140743c1e: MOVSS XMM1,[RBP-0x80]
140743c23: ADDSS XMM1,XMM0
140743c27: MOVSS [RBP-0x80],XMM1
```

R14 is the game/context argument; RDI is the original window (saved from incoming RDX at0x7411EE/0x7411F1 and restored from[RBP-0x68] at0x743895). XMM1/XMM2 receive the layout values. XMM0 returned by the callee is added to the running vertical layout value. A working candidate signature is float helper(context,float x,float y,window), pending the callee body. The decompiler displays only FUN_140769300(param_1); that incomplete inferred signature must not hide the additional register arguments. The call occurs before the task-target rows.

This is a candidate shared UI subsection that receives the DO WINDOW and may itself resolve/draw actionable fleet rows. It is NOT yet identified as a fleet helper, vehicle opener or gauge renderer. Its complete body is the next bounded export to inspect; trace fleet selection/input inside it if present. Do not label its values litres or assume a type5 result. No new helper hook is justified yet.

A second shared-window helper is0x6F4B40, called at0x745060 with RCX=context,RDX=window,R8=&layout_x,R9=&layout_y (decompilation line1930). Its role remains unknown and it can be inspected if0x769300 does not resolve the actionable vehicle UI. This prevents the unsupported conclusion that no truck click can exist merely because741170 does not contain a direct fleet->creator call: relevant behavior can be in these callees.

### Integration outcome

The newly supplied full export is usable and resolves the previous hit-test/click misclassifications. It does not yet identify the truck fuel gauge, tank field, conversion or burn/refill attribution. Continue with the precise0x769300 callee and its real arguments, not another restatement of the old cargo-slot chain. Current log fuel_slot_valid=0 remains unaddressed. No executable source/defaults, game memory, dispatch policy or billing changed. No game retest requested.


## Direct review of RAW_EXPORTS_769300.md, remote commit4425f9e — 2026-10-28

Fetched origin/DO-fuel-cost-calculatotr and read the actual1169-line file from4425f9e. Markdown SHA-25621fadcaa83fc82a41364a0c1c07adcc357de6875db2d1d0a13e62b1d95c17de2. Python consistency check:754 instruction lines, zero address/byte-length gaps,8 direct E8 targets all consistent with their bytes. The export reaches RET at0x14076A16C. This is internal export validation, not independent execution of Ghidra or the game.

### Resolved role

FUN_140769300 is the office-wide loading/unloading task-threshold UI helper. Its full signature confirms context in RCX, layout coordinates in XMM1/XMM2, and window in R9. It resolves the associated DO through window+0x240, obtains current threshold values via0x76A3A0 into local_120/local_138, draws the greater-than/less-than percentage controls, handles collision/click gating, and writes the adjusted values across target TaskCfgs.

The task-object chain is DO+D38 -> entry pointer -> entry+0x10 -> TaskCfg. entry+8 is the target building. Target building type0x14 is excluded from the four batch-write loops. DO building type0x2D changes fallback handling of an unavailable unloading value; do not call it a vehicle metadata check or assign a new building role without independent evidence.

### Exact stores and input gating

| Instruction address | Destination | Role in helper |
|---|---|---|
|0x1407697D9|TaskCfg+0x04|first loading adjustment control|
|0x140769A88|TaskCfg+0x04|second loading adjustment control|
|0x140769DD8|TaskCfg+0xCC|first unloading adjustment control|
|0x14076A0A8|TaskCfg+0xCC|second unloading adjustment control|

Each store follows target filtering and UI collision, click-edge DAT_140A54E91, and window+0x20 gating. Numeric step sizes/bounds come from globals; their decimal values were not extracted here and must not be guessed. These stored values are task thresholds, NOT vertical positions or layout values. Layout uses param_2/param_3 and local_11c/local_114; the function returns consumed UI height. Reuse of fVar24/fVar25 for multiple local computations is not evidence of shared units.

No truck identity, per-truck selection, tank quantity, litres conversion or fuel burn observation is established by this function. No direct call to the panel creator or camera opener appears. The whole-function observation rules out769300 as the previously sought direct truck opener/fuel gauge renderer; it does not rule out unrelated behavior inside unexported callees by assertion.

### Scope decision

Close the769300 lead for fuel investigation; no need to export its threshold-aggregation helper0x76A3A0 for the fuel task. This finding agrees with current loading/unloading threshold policy but does not justify changing the working10/30 settings, timers or permissions. Earlier candidate shared helper0x6F4B40 (called at0x745060 with context,window,&x,&y) remains unreviewed; it is a bounded candidate only, not a confirmed fleet helper. Prefer a directly proved truck-window route over an unconstrained search for generic UI functions.

No executable code, INI, hook, billing or dispatch change. No runtime test/build rerun for this documentation-only review. Truck fuel reader and tracker remain unimplemented/unvalidated.


## Direct review of RAW_EXPORTS_6F4B40.md, remote commit034a85a — 2026-10-29

Fetched the actual remote commit and read the351-line export. SHA-2560e7f7a5820a4fe5032acd036464f459ebe5d18b50d32f8300acb986752442901. Byte/address check:164 instructions,620 bytes through RET, zero address gaps; all12 direct E8 call encodings match their printed targets. This is export consistency only, not independent binary/Ghidra/runtime validation.

Supported classification: shared UI subsection orchestration with vertical-layout change detection, not proven to be only coordinate adjustment. It passes context/layout/window to0x772400 and0x7752A0 unconditionally, and to0x775000 when window+0x248==0. Other conditional section calls are0x702730,0x6F4DB0 and0x7A2A60. The direct+C970/+978 scan is building storage at associatedObject+0x970/+0x978 with stride0xE0, not a vehicle fleet. Descriptor+0x77C/+0x780/+0x360 are read through associatedObject+0x318, not directly from associatedObject.

Important scope correction: no direct mouse/fleet/fuel code in this wrapper does NOT prove there is no vehicle UI in its unexplored callees. The supplied conclusion excludes the transitive path without analyzing those bodies. Keep that path unresolved rather than closed. Calling all children layout-adjustment helpers is likewise unsupported: substantive UI sections commonly update the running Y coordinate. FUN_140042DE0's body is unavailable, so save/restore semantics are not established. There are six actual calls to042DE0 (initial plus five conditional), not seven as the report says.

Next bounded artifact: full verbatim decompilation and disassembly of the two unconditional UI section callees0x772400 and0x7752A0, collected in one Markdown. Identify each role and report object provenance, any fleet enumeration, actual input/click condition and selected-object action if present. Classify only those two functions; no recursive export of all descendants. If either delegates the relevant action, include exact call arguments and identify the next callee without declaring its role proved. No need to re-export6F4B40 or investigate042DE0 for fuel at this point. No assumed panel type, cargo tank, unit conversion or renderer location. No runtime code/configuration/hook/payment changes, and no game retest requested.


## Direct review of RAW_EXPORTS_772400_7752A0.md, remote commitdcaf22d — 2026-10-29

Fetched the actual synced commit and reviewed both bodies and corresponding disassembly. The file contains2372 newline-counted lines; SHA-256dd6057e13c2c0ec496e62ca96b96a76c82df49150a81b4608d80d92e2bf48c84. Consistency checks:772400 has142 instructions/545 bytes/4 direct E8 calls;7752A0 has1372 instructions/6804 bytes/44 direct E8 calls. Both reach RET, have zero address/byte-length gaps and zero direct-call-target mismatches. These checks validate export consistency, not native executable identity or runtime behavior.

### Supported roles

-772400: gate window+0x248==0; obtain target object from window+0x240; scan context+0x11B98/+0x11BA0 entries and their task lists for entry+8==target; collect referencing objects and pass them to766940 with a localized heading. This is reverse task association, not a truck-fleet enumeration. The delegated renderer remains unreviewed, so avoid universal claims about every descendant.
-7752A0: the actual signature uses float* for both layout parameters, NOT float values. It is a shared building-operation section, gated by context+0x5BC, window+0x248==0, and associatedObject+0xEB2. Explicit ReplaceMachines/FundReplaceMachines tutorial strings, construction_fund/construction_fundusd actions, building flags+0x608/+0x609 and resource/funding calculations support machinery replacement/reconstruction-related UI, not a specifically DO/fuel panel. The exact meaning of every percentage/operation still requires localized labels/callee evidence and is not guessed here.

The1A8D70/6EE2A0 actions receive the same associated building, not a newly selected truck. No direct vehicle fleet enumeration or truck identity is present in either supplied function body. Close these bodies as direct truck-gauge/opener leads; do not recursively investigate reconstruction callees for fuel.

### Debug strings are not fuel evidence

7752A0's L:/F: debug outputs are argument-traceable in the assembly:
-0x776A46/0x776A54 load doubles from associated building+0xEB8/+0xED0 for the L: line at0x776A96.
-0x776AAE/0x776ABC load doubles from associated building+0xEC0/+0xED8 for the F: line at0x776AFE.

The labels do not prove litres/load/fuel. No truck pointer, units, or tank write is supplied for these building fields. Preserve them as unknown-semantics building debug quantities; do not promote F: to actual vehicle fuel.

### Remaining bounded wrapper child

FUN_140775000 is the remaining unreviewed member of the wrapper's initial three UI sections.6F4B40 calls it at0x6F4BE4 when window+0x248==0, with context,&x,&y,window. Next bounded artifact may export775000 alone (complete body plus disassembly), classify its role, and report a selected-vehicle path only if actual pointer/input/action dataflow exists. No recursive search, no re-export of closed sections, no new fuel-source claims from variable/label names. If unrelated, stop and reassess the route rather than opening a broad descendant search. No runtime code/default/hook/payment changes or new game test.


## Direct review of RAW_EXPORTS_775000.md, remote commitc9c1f53 — 2026-10-29

Fetched the synced commit and reviewed the complete294-line export. SHA-256ad921c7b5e4c1769192c9ccccc13c96f7506c1101dc40a9abfc586529be283a3. Consistency check:131 instructions/660 bytes through RET, zero address gaps, the one direct E8 target matches its bytes. No independent Ghidra/binary/runtime execution performed.

The actual signature has float* layout arguments. The associated building's+0x970/+0x978 vector, stride0xE0, agrees with existing TSM_BLD_STORAGES_BEGIN/END and TSM_STORAGE_STRIDE in game_world.h. This is building STORAGE-entry rendering, not merely an unidentified child-object list. It filters storage entries by entry+0x88 value5/6, then calls706C00 at0x7751EE with that storage-entry pointer. The per-entry index advances for every storage entry, not only matches. The+0x318 check dereferences a descriptor before reading descriptor+0x360; it is not a direct base+0x678 read.

Close775000 as a direct vehicle/gauge lead. The unreviewed706C00 cannot be declared universally vehicle-free, but this caller supplies a storage entry, not a truck. No further706C00 export is justified solely by this path. The malformed PrintLeftUnicode decompilation (NaN/odd signature) also reinforces using assembly for variadic argument interpretation, not inferred prototypes.

### Strategy checkpoint

All three initial section callees of6F4B40 now lead to reverse target associations, building-operation UI or storage-entry rendering. Stop this DO-wrapper branch for the fuel task rather than recursively collecting generic helpers. The missing anchor remains the actual individual road-vehicle gauge, not dispatch policy or task thresholds.

Next bounded investigation should be a global decoded-string/reference pass over initialized executable data for numeric fuel-unit formats and neighboring speed/RPM/odometer formats, preserving raw decoded strings, addresses, bytes and callers. Previous absence claims were restricted to the generic formatter's8B5xxx strings and do not establish global absence. Do not assume exact %.2f l spelling, internal units,1176 or a renderer address range. Use combined numeric fuel/speed/odometer evidence to select a genuine vehicle UI caller before deeper tracing. If units are assembled from localization, trace that construction; if no anchor is found, report exact search coverage and stop. Keep scripts/results in one Markdown. No code/hook/config/billing changes or unchanged game retest.


## Review of42c4767 string search and implemented replacement — 2026-10-29

Fetched remote commit42c4767 and inspected the full403-line VEHICLE_GAUGE_STRING_ANCHORS.md, including StrAddrNeedles.java. Reviewed-export SHA-256e6233329f07166cef611675be0344baae9c528a6cdca62498dbe5cb68a65a763. No native binary or Ghidra project is available here, so the actual SOVIET64.exe search was NOT rerun in Arena.

### Why the negative conclusion is not established

1. StrAddrNeedles only accepts a needle immediately followed by a null terminator. Thus needles Fuel, %.2f and %.2f l all fail against the legitimate example `Fuel: %.2f l remaining`. This is suffix matching, not the advertised general substring/string search. The included script also searches leading-space ` Fuel`/` fuel`, not every case the prose describes.
2. It stops at12 hits per needle/encoding. The UTF-16 %.2f table contains exactly12 entries. Later candidates cannot be ruled out; no truncation flag or complete count was emitted.
3. It reconstructs a longer string starting at s but asks for xrefs at i, the needle's location. A code reference to the full string start will be missed when the needle is interior. Both addresses matter because compilers may also share suffix strings.
4. It scans every initialized memory block, including code, contrary to the report's narrower data-section description; many reported tiny matches are instruction bytes. Actual block coverage/skip details are not emitted. Blocks above64MiB are silently skipped.
5. Wide-string terminator checking can read past the buffer near its end. No full actual output or source for the separate StrAddr.java runs was embedded, so those runs cannot independently establish broader coverage here.
6. A large renderer called from a frame loop is not evidence against an instrument subsection inside that renderer. Do not exclude09DC80/59F1F0 solely by function size or caller scope.

Supported outcome: no fuel gauge was identified by the reported run. Unsupported stronger outcome: no relevant string/reference anchor exists in the executable data. Localization remains possible but is not proved by an incomplete negative search.

### Implemented bounded correction

Added tools/vehicle_gauge_strings.py: dependency-free offline PE scanner for file-backed initialized non-executable data. It reports exact section coverage, binary SHA-256/PE identity, complete candidate strings/bytes, start VAs and interior format/unit anchors, encoding and termination status. It supports substring matches, uncapped candidate counts, general floating-point printf formats, and ASCII/BMP UTF-16LE (both alignments). Full scan results and the exact scanner source are written into one Markdown. Candidate labels do not establish physical units or vehicle UI. Executable sections, runtime/localization resources and surrogate-pair Unicode are explicitly outside full coverage.

Added tests/vehicle_gauge_strings/test_scanner.py. Actual results in this environment:

```text
python3 -m unittest discover -s tests/vehicle_gauge_strings -v
Ran 14 tests in 0.041s
OK

python3 -m py_compile tools/vehicle_gauge_strings.py tests/vehicle_gauge_strings/test_scanner.py
PASS

git diff --check
PASS
```

The14 synthetic tests cover suffix-filter regression, more than12 candidates, full-start/interior VA mapping, localized and odd-aligned UTF-16, precision/format variations, escaped percent tokens, standalone unit candidates, nontermination reporting, PE32/PE32+ mapping, section coverage, malformed/truncated rejection and CLI Markdown/source output. These tests do NOT locate or validate real truck fuel.

### Next bounded run (once the tool is available in the investigator checkout)

```text
python tools/vehicle_gauge_strings.py "PATH_TO_ACTUAL_SOVIET64.exe" --output docs/private-logistics-runtime/VEHICLE_GAUGE_STRING_SEARCH_FIXED.md
```

Use the same executable analyzed in Ghidra; compare the generated hash/identity. Keep all candidate results, including counts above12. In Ghidra, inspect references to string_va AND interior_anchor_vas, with raw caller blocks for plausible numeric/unit candidates. Do not eliminate a large caller by size alone. If no direct unit anchor exists after this corrected pass, report that measured outcome and exact coverage rather than assuming localization. No further DO-helper descent. No runtime game test or fuel hook justified yet.

The replacement is implemented locally on the Arena session branch and has NOT been pushed by this review. No game binary, runtime plugin, INI, dispatch policy, treasury or billing changed. Complete new source and tests follow in this same Markdown for transfer.

### Complete source: tools/vehicle_gauge_strings.py

```python
"""Offline PE string-anchor inventory; no game execution or external dependencies.

Run on the SAME executable used by Ghidra:
  python tools/vehicle_gauge_strings.py SOVIET64.exe --output gauge-search.md

All candidates are leads, not identification of a vehicle instrument. The
Markdown includes full JSON results and this source. Import the listed VAs into
Ghidra and inspect references to string starts AND the reported interior anchors.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

# Tokenize escaped percent signs too, rather than treating %%f as a conversion.
FORMAT = re.compile(
    r"%%|%(?:\d+\$)?[-+ #0]*(?:\d+|\*)?(?:\.(?:\d+|\*))?"
    r"(?:I64|I32|hh|ll|[hlLjzt])?[diuoxXfFeEgGaAcCsSpn]"
)
PATTERNS = {
    "fuel_word": re.compile(r"fuel", re.I),
    "litre_word": re.compile(r"lit(?:re|er)s?", re.I),
    "speed_unit": re.compile(r"km\s*/\s*h|\bmph\b", re.I),
    "rpm": re.compile(r"\brpm\b", re.I),
    "distance": re.compile(r"odometer|distance|\bkm\b|\bmi\b", re.I),
}


def anchors(text: str) -> tuple[list[str], list[int]]:
    reasons, offsets = [], set()
    formats = [m for m in FORMAT.finditer(text)
               if m.group() != "%%" and m.group()[-1] in "fFeEgGaA"]
    if formats:
        reasons.append("float_format")
        offsets.update(m.start() for m in formats)
    for label, pattern in PATTERNS.items():
        matches = list(pattern.finditer(text))
        if matches:
            reasons.append(label)
            offsets.update(m.start() for m in matches)
    # Separately assembled units need not share a literal with the number.
    if text.strip() in {"l", "L", "л", "Л"}:
        reasons.append("standalone_litre_candidate")
        offsets.add(text.index(text.strip()))
    return reasons, sorted(offsets)


def decoded_runs(data: bytes):
    """Yield printable ASCII and BMP UTF-16LE runs, including embedded needles.

    UTF-16 is checked at both byte alignments. Nonterminated runs are reported
    as such, not silently assumed to be genuine string literals. No hit caps.
    Non-ASCII BMP text is retained so localized suffixes are not discarded.
    """
    for match in re.finditer(rb"[\x20-\x7e\t\r\n]+", data):
        yield (match.start(), match.end(), match.group().decode("ascii"),
               "ASCII", match.end() < len(data) and data[match.end()] == 0)
    for parity in (0, 1):
        start, chars = 0, []
        for offset in range(parity, len(data) - 1, 2):
            unit = data[offset] | data[offset + 1] << 8
            char = chr(unit)
            if char.isprintable() or char in "\t\r\n":
                if not chars:
                    start = offset
                chars.append(char)
            else:
                if chars:
                    yield start, offset, "".join(chars), "UTF-16LE", unit == 0
                    chars = []
        if chars:
            yield start, offset + 2, "".join(chars), "UTF-16LE", False


def scan_data(data: bytes, base_va: int = 0, file_base: int = 0) -> list[dict]:
    results = []
    for start, end, text, encoding, terminated in decoded_runs(data):
        reasons, positions = anchors(text)
        if not reasons:
            continue
        width = 1 if encoding == "ASCII" else 2
        results.append({
            "string_va": hex(base_va + start),
            "file_offset": hex(file_base + start),
            "encoding": encoding,
            "terminated": terminated,
            "text": text,
            "bytes_hex": data[start:end].hex(),
            "reasons": reasons,
            "interior_anchor_vas": [hex(base_va + start + p * width)
                                    for p in positions],
        })
    return results


def pe_sections(data: bytes):
    """Read PE image base and section table, rejecting truncated input."""
    def need(offset, length):
        if offset < 0 or offset + length > len(data):
            raise ValueError(f"Truncated PE range {offset:#x}+{length:#x}")

    need(0, 64)
    if data[:2] != b"MZ":
        raise ValueError("Not a DOS/PE image")
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    need(pe, 24)
    if data[pe:pe + 4] != b"PE\0\0":
        raise ValueError("Missing PE signature")
    machine, count, timestamp = struct.unpack_from("<HHI", data, pe + 4)
    opt_size = struct.unpack_from("<H", data, pe + 20)[0]
    opt = pe + 24
    need(opt, opt_size)
    if opt_size < 32:
        raise ValueError("Optional header too short")
    magic = struct.unpack_from("<H", data, opt)[0]
    if magic == 0x20B:
        image_base = struct.unpack_from("<Q", data, opt + 24)[0]
    elif magic == 0x10B:
        image_base = struct.unpack_from("<I", data, opt + 28)[0]
    else:
        raise ValueError(f"Unsupported optional-header magic {magic:#x}")
    table = opt + opt_size
    need(table, count * 40)
    sections = []
    for index in range(count):
        pos = table + index * 40
        name = data[pos:pos + 8].split(b"\0", 1)[0].decode("ascii", "replace")
        virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, pos + 8)
        flags = struct.unpack_from("<I", data, pos + 36)[0]
        # Scan initialized non-executable data; explicitly report other sections.
        scan = bool(flags & 0x40) and not bool(flags & 0x20000000) and raw_size > 0
        span = min(raw_size, virtual_size) if virtual_size else raw_size
        if scan:
            need(raw_offset, raw_size)
        sections.append({"name": name, "rva": hex(rva),
                         "raw_offset": raw_offset, "raw_size": raw_size,
                         "scan_bytes": span if scan else 0,
                         "flags": hex(flags), "scanned": scan})
    return {"image_base": hex(image_base), "machine": hex(machine),
            "pe_timestamp": hex(timestamp)}, sections


def inventory(data: bytes) -> dict:
    identity, sections = pe_sections(data)
    results = []
    for section in sections:
        if not section["scanned"]:
            continue
        offset, size = section["raw_offset"], section["scan_bytes"]
        candidates = scan_data(data[offset:offset + size],
                               int(identity["image_base"], 16) + int(section["rva"], 16),
                               offset)
        for candidate in candidates:
            candidate["section"] = section["name"]
        results.extend(candidates)
        section["candidate_count"] = len(candidates)
    return {"sha256": hashlib.sha256(data).hexdigest(), **identity,
            "scope": "All file-backed initialized non-executable PE sections; "
                     "ASCII and BMP UTF-16LE runs; no candidate cap. "
                     "Not runtime/localization-resource analysis. "
                     "Surrogate-pair Unicode strings may be split.",
            "sections": sections, "candidate_count": len(results),
            "candidates": results}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        result = inventory(args.executable.read_bytes())
    except (OSError, ValueError) as error:
        parser.exit(2, f"Search failed: {error}\n")
    markdown = (
        "# Vehicle gauge string candidates — offline PE inventory\n\n"
        "These are candidates, NOT confirmed vehicle UI or fuel semantics.\n"
        "VAs use the PE preferred image base, not runtime ASLR addresses.\n"
        "Inspect Ghidra references to string_va AND interior_anchor_vas.\n"
        "No references found at one address does not establish no callers.\n\n"
        "## Complete results and section coverage\n\n```json\n"
        + json.dumps(result, ensure_ascii=False, indent=2)
        + "\n```\n\n## Search source used\n\n```python\n"
        + Path(__file__).read_text(encoding="utf-8") + "\n```\n"
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(markdown, encoding="utf-8")
    print(f"Wrote {result['candidate_count']} candidates to {args.output}")


if __name__ == "__main__":
    main()

```

### Complete source: tests/vehicle_gauge_strings/test_scanner.py

```python
import importlib.util
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("scanner", ROOT / "tools/vehicle_gauge_strings.py")
scanner = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(scanner)


def fake_pe(payload=b"Fuel: %.2f l remaining\0", pe64=True):
    """Small synthetic PE: one initialized data and one executable section."""
    data = bytearray(0x600)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    opt_size = 0xF0 if pe64 else 0xE0
    struct.pack_into("<HHI", data, 0x84, 0x8664 if pe64 else 0x14C, 2, 0x6A3EB6AD)
    struct.pack_into("<H", data, 0x94, opt_size)
    struct.pack_into("<H", data, 0x98, 0x20B if pe64 else 0x10B)
    struct.pack_into("<Q" if pe64 else "<I", data, 0x98 + (24 if pe64 else 28),
                     0x140000000 if pe64 else 0x400000)
    table = 0x98 + opt_size
    for i, (name, flags, off, rva) in enumerate([
        (b".rdata", 0x40000040, 0x200, 0x2000),
        (b".text", 0x60000020, 0x400, 0x1000),
    ]):
        pos = table + i * 40
        data[pos:pos + 8] = name.ljust(8, b"\0")
        struct.pack_into("<IIII", data, pos + 8, 0x200, rva, 0x200, off)
        struct.pack_into("<I", data, pos + 36, flags)
    data[0x200:0x200 + len(payload)] = payload
    data[0x400:0x40A] = b"RPM %.2f\0\0"
    return bytes(data)


class ScannerTests(unittest.TestCase):
    def ascii_candidates(self, data):
        return [x for x in scanner.scan_data(data) if x["encoding"] == "ASCII"]

    def test_embedded_match_with_suffix(self):
        text = b"Fuel: %.2f l remaining\0"
        self.assertEqual(self.ascii_candidates(text)[0]["text"], text[:-1].decode())

    def test_original_suffix_filter_would_miss(self):
        text = b"Fuel: %.2f l remaining\0"
        for needle in (b"Fuel", b"%.2f", b"%.2f l"):
            pos = text.index(needle)
            self.assertNotEqual(text[pos + len(needle)], 0)
        self.assertEqual(len(self.ascii_candidates(text)), 1)

    def test_more_than_twelve_hits(self):
        data = b"\0".join(f"candidate {i}: %.2f l".encode() for i in range(25)) + b"\0"
        self.assertEqual(len(self.ascii_candidates(data)), 25)

    def test_prefix_preserved_and_interior_address(self):
        result = scanner.scan_data(b"\0prefix %.2f l\0", 0x140002000, 0x200)[0]
        self.assertEqual(result["string_va"], "0x140002001")
        self.assertIn("0x140002008", result["interior_anchor_vas"])
        self.assertEqual(result["file_offset"], "0x201")

    def test_utf16_localized_suffix(self):
        text = "Паливо: %.2f л"
        data = b"\0\0" + text.encode("utf-16le") + b"\0\0"
        results = scanner.scan_data(data, 0x140000000)
        result = next(x for x in results if x["text"] == text)
        self.assertEqual(result["encoding"], "UTF-16LE")
        self.assertEqual(result["string_va"], "0x140000002")
        self.assertTrue(result["terminated"])
        self.assertIn(hex(0x140000002 + text.index("%") * 2), result["interior_anchor_vas"])

    def test_utf16_odd_alignment(self):
        text = "Fuel %.3f %ls"
        data = b"\0" + text.encode("utf-16le") + b"\0\0"
        self.assertTrue(any(x["text"] == text for x in scanner.scan_data(data)))

    def test_precision_and_separate_unit_formats(self):
        for text in ("%.*f %ls", "%.3f", "%8.2f litres", "%g", "%.2lf l"):
            self.assertIn("float_format", scanner.anchors(text)[0])

    def test_escaped_percent_is_not_float(self):
        self.assertNotIn("float_format", scanner.anchors("%%f")[0])
        self.assertIn("float_format", scanner.anchors("%%%f")[0])

    def test_standalone_unit(self):
        for unit in ("l", "L", "л"):
            self.assertIn("standalone_litre_candidate", scanner.anchors(unit)[0])

    def test_nonterminated_reported(self):
        result = self.ascii_candidates(b"Fuel %.2f l")[0]
        self.assertFalse(result["terminated"])

    def test_pe64_coverage_and_va(self):
        result = scanner.inventory(fake_pe())
        self.assertEqual(result["image_base"], "0x140000000")
        self.assertEqual(result["pe_timestamp"], "0x6a3eb6ad")
        self.assertEqual([s["scanned"] for s in result["sections"]], [True, False])
        self.assertEqual(result["candidates"][0]["string_va"], "0x140002000")
        self.assertFalse(any(x["text"].startswith("RPM") for x in result["candidates"]))

    def test_pe32(self):
        result = scanner.inventory(fake_pe(pe64=False))
        self.assertEqual(result["image_base"], "0x400000")
        self.assertEqual(result["candidates"][0]["string_va"], "0x402000")

    def test_invalid_and_truncated_rejected(self):
        for data in (b"", b"MZ", b"x" * 128, fake_pe()[:0x250]):
            with self.assertRaises(ValueError):
                scanner.inventory(data)

    def test_cli_embeds_results_and_source(self):
        with tempfile.TemporaryDirectory() as temp:
            source, output = Path(temp) / "synthetic.exe", Path(temp) / "report.md"
            source.write_bytes(fake_pe())
            result = subprocess.run([sys.executable, str(ROOT / "tools/vehicle_gauge_strings.py"),
                                     str(source), "--output", str(output)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            report = output.read_text()
            self.assertIn('"string_va": "0x140002000"', report)
            self.assertIn('Fuel: %.2f l remaining', report)
            self.assertIn('def inventory(data: bytes)', report)


if __name__ == "__main__":
    unittest.main()

```


## Direct review and local CFG trace of 5c6f8b8 / RAW_EXPORTS_34D590.md — 2026-10-29

Fetched the actual commit and decoded its UTF-16LE Markdown without modifying the supplied export. Artifact SHA-256: `154afba88d9d5f9c369ac5580ec2cce667d78ffefafe72f316cfb198f888c5b3`. Embedded executable SHA-256 remains `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8`. Native executable/project unavailable here; analysis below is of the supplied addressed instruction bytes. Decompiler timeout is explicitly accepted; a retry is unnecessary for this normal-flow trace.

### Export consistency audit (actually executed)

The first disassembly block has **9,042** instruction lines INCLUDING the three-byte instruction at the next function (0x1403586F0); **9,041** belong before that boundary. Their byte sum excluding the boundary is **45,395 = 0xB153**, not 0xB163. There are two one-byte holes, 0x14035268A and 0x140353340, immediately after calls to 0x14084E124, plus 11 unlisted bytes after RET at 0x1403586E4 and before the next function. The stated continuous range through 0x1403586EF spans 45,408 bytes. Therefore "9,044 instructions/no gaps" is not literal coverage proof. The holes are not traversed by the backward slices below; their contents are not inferred here.

All encoded direct call and branch targets agree with the listed targets. All checked absolute RIP-relative operand targets also agree after accounting for trailing immediate operands. No indirect JMP or direct branch outside the exported address set was found. Addresses printed as 140... are VAs, not RVAs; the heading and next-function RVA annotation are incorrect (next RVA is 0x3586F0, not 0x586F0).

### R14 reaching definition resolved locally

Constructed instruction-level normal-flow predecessor edges from direct branches and fallthrough; CALL is modeled as returning and preserving the Windows x64 nonvolatile register R14. Exceptional unwinding, arbitrary register-clobbering nonstandard callees and runtime patching are outside this model. A backward walk stops at explicit R14/R14D/R14W/R14B writes.

At **0x140353D72**, the unique reaching explicit definition in this supplied normal-flow graph is:

```asm
140353420: 48 8B 86 38 CC 00 00  MOV RAX,qword ptr [RSI + 0xcc38]
140353427: 4E 8B 34 E0           MOV R14,qword ptr [RAX + R12*0x8]
```

RSI was set from RCX at 0x14034D63B. The vector end is context+CC40, count is `(end-begin)/8`, indexed by R12; loop back-edge at 0x140354AA3 targets 0x140353420. Between this load and the disputed numeric read there is no explicit R14 write. The four MOV R14,RAX examples from 350E14/35132A/35182C/351ACC are not the reaching definitions for this read. Thus the base pointer is a row object from a **CC38/CC40 pointer vector**, not directly the earlier C650+48 80-byte-entry structure. Concrete road-truck/DO membership is still NOT established.

The numeric block at 0x140353D60 has exactly one explicit predecessor: JNZ at 0x140353772, following `CMP byte ptr [RSI+0xCCDD],0`. The fallthrough preceding D60 is actually an unconditional JMP at 0x140353D5B to 0x140353F24. Therefore `[row+1A48]/365.25` belongs to the CCDD-nonzero alternative display. Unit ID57001 remains unresolved; age/years remains a hypothesis, not a new established label. No fuel tonnes or runtime-patched divisor is established.

### A distinct, structurally fuel-related display was found

In the CCDD-zero path (the other side of that branch), 0x140353ACB–0x140353B62:

- Initialize two float accumulators to the zero value held in XMM12.
- Start signed RDX at -1, so first RCX=row (R14); subsequent iterations select pointers from row+3A0/3A8.
- Read capacity-like float via `*(object+1708)+8684` at 0x140353B0F, matching the existing documented native refuel-capacity access.
- For positive capacity, add capacity; load float **object+5F0** at **0x140353B21**, adding positive values to the other accumulator.
- Divide the accumulated levels by accumulated capacities, multiply by the float at 0x14090AB70, convert to integer and format through string 0x1408B7E78.

This is an aggregate level/capacity display over the row and its linked objects, not the `[row+1A48]/365.25` block. The multiplier/string contents are not present as raw data in this export, so do not silently assume their exact values. It corroborates a native UI use of the legacy **+5F0** reader beside the known capacity access; it does NOT supersede the real-game evidence that the previously observed DO truck's mirror was constant/unusable, establish physical storage or exact burn, or identify the screenshot's numeric litres gauge. Do NOT deploy the same reader again unchanged as a validated fix. No C3F0 reader appears in this exported function.

### The same row pointer is passed to the native window creator

The local normal-flow trace also uniquely resolves R14 at 0x140354334 to 0x140353427. At the row-click path:

```asm
140354318: 33 C0                 XOR EAX,EAX
14035431A: 48 89 44 24 40        MOV qword ptr [RSP+0x40],RAX
14035431F: 48 89 44 24 38        MOV qword ptr [RSP+0x38],RAX
140354324: 89 44 24 30           MOV dword ptr [RSP+0x30],EAX
140354328: 89 44 24 28           MOV dword ptr [RSP+0x28],EAX
14035432C: 89 44 24 20           MOV dword ptr [RSP+0x20],EAX
140354330: 44 8D 48 01           LEA R9D,[RAX+1]
140354334: 4D 8B C6              MOV R8,R14
140354337: 8D 50 02              LEA EDX,[RAX+2]
14035433A: 48 8B CE              MOV RCX,RSI
14035433D: E8 5E 9F 39 00        CALL 0x1406EE2A0
```

Cross-checking existing `decomp_1406EE2A0.c`: param3 -> window+240, param4 -> +248, param2 -> +24C, param6 -> +34, param7 -> +38. Thus this request supplies **window+240=row, +248=1, +24C=2, +34=0, +38=0** for a newly created window (creator may instead focus/reuse an existing one). This is an actual row-to-native-panel route, not a guessed camera enum or a guessed association from two matching offsets. It does not yet prove a particular DO truck or identify the panel renderer. In particular, do not infer primary window kind=1 from param4=1; these are distinct fields.

### Next bounded investigation

No more export of 34D590 or R14 CFG handoff is needed. Starting from the row-click request above, trace **FUN_1406E9B80 for window+34=0, +248=1, +24C=2**, with +240 carrying the row object, to the actual detail-panel renderer. Do not assume 7D5460 without following the branch. Inspect that proven renderer for the E/F gauge's numeric formatting/getter and capacity access. If it delegates rendering, follow only the argument-preserving gauge path. Resolve units through actual lookup entries/conversion data, not adjacent bytes. Establish whether the same object is the road truck in the DO fleet before integration. Keep actual writers/burn/refill instrumentation deferred until physical quantity is identified. No unchanged game test. Billing OFF; no tracker implemented.

### Complete executed analysis source

Cached inputs can be reconstructed with `git show 5c6f8b8:docs/private-logistics-runtime/RAW_EXPORTS_34D590.md` into `.cache/RAW_EXPORTS_34D590.md`, then decode that file with Python `read_bytes().decode('utf-16')` into `.cache/raw34.txt`. The full read-only audit used is below. It is an artifact-level CFG check, not an x86 emulator or an independent scan of the game binary.

```python
from pathlib import Path
import re,collections,hashlib
s=Path('.cache/raw34.txt').read_text();body=s.split('```asm',1)[1].split('```',1)[0]
rows=[]
for m in re.finditer(r'^([0-9a-f]{9}):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+([A-Z][^\r\n]*)',body,re.M):
 rows.append((int(m[1],16),bytes.fromhex(m[2]),m[3]))
print('source SHA256',hashlib.sha256(Path('.cache/RAW_EXPORTS_34D590.md').read_bytes()).hexdigest())
print('instruction count',len(rows),'sum bytes',sum(len(b) for a,b,t in rows),'span',rows[-1][0]+len(rows[-1][1])-rows[0][0], 'body excl boundary',len(rows)-1)
gaps=[(hex(a),hex(a+len(b)),hex(rows[i+1][0])) for i,(a,b,t) in enumerate(rows[:-1]) if a+len(b)!=rows[i+1][0]];print('gaps',gaps[:30],len(gaps))
by={a:(b,t) for a,b,t in rows};edges={};pred=collections.defaultdict(set);missing=[];bad=[];ripbad=[]
for a,b,t in rows:
 op=t.split()[0];n=a+len(b);outs=[]
 if op.startswith('J'):
  m=re.fullmatch(r'J[A-Z]+ (0x[0-9a-f]+)',t)
  if m:
   dst=int(m[1],16);outs.append(dst)
   disp=int.from_bytes(b[-(1 if len(b)==2 else 4):],'little',signed=True)
   if a+len(b)+disp!=dst:bad.append(hex(a))
  else: missing.append((hex(a),t))
 if op not in ('JMP','RET','INT3') and n in by:outs.append(n)
 for dst in outs:
  if dst in by:pred[dst].add(a)
  else:missing.append((hex(a),hex(dst)))
 edges[a]=[x for x in outs if x in by]
 if b[0]==0xe8:
  m=re.match(r'CALL (0x[0-9a-f]+)',t)
  if m and a+len(b)+int.from_bytes(b[1:5],'little',signed=True)!=int(m[1],16):bad.append(hex(a))
 m=re.search(r'\[(0x[0-9a-f]+)\]',t)
 if m:
  imm=4 if b[:2]==bytes.fromhex('c705') else 1 if b[:2]==bytes.fromhex('803d') else 0
  disp=b[-4-imm:len(b)-imm]
  if a+len(b)+int.from_bytes(disp,'little',signed=True)!=int(m[1],16):ripbad.append((hex(a),t))
print('direct jump/call mismatch',bad,'missing/indirect edges',missing,'RIP mismatches',ripbad[:15],len(ripbad))
# Backward predecessor walk stopping at an explicit write to R14/R14D/W/B.
# Calls follow normal Windows x64 nonvolatile-register convention; exceptional
# unwind edges and self-modifying code are not modeled.
def writes(t):
 return bool(re.match(r'(?!CMP |TEST |PUSH )(?:[A-Z0-9]+) R14(?:D|W|B)?(?:,|$)',t)) or bool(re.match(r'XCHG [^,]+,R14(?:D|W|B)?$',t))
for site in (0x140353d72,0x140353b21,0x1403548eb,0x140354334):
 seen=set();defs=set();unknown=[];q=list(pred[site])
 while q:
  a=q.pop()
  if a in seen:continue
  seen.add(a);b,t=by[a]
  if writes(t):defs.add(a);continue
  if not pred[a]:unknown.append(hex(a))
  q.extend(pred[a])
 print('site',hex(site),'visited',len(seen),'reaching definitions',[(hex(a),by[a][1]) for a in sorted(defs)],'roots',unknown)
print('predecessors numeric block',[(hex(a),by[a][1]) for a in pred[0x140353d60]])
print('R14 writes before read since vector load',[(hex(a),t) for a,b,t in rows if 0x140353427<=a<0x140353d72 and writes(t)])

```

### Actual output

```text
source SHA256 154afba88d9d5f9c369ac5580ec2cce667d78ffefafe72f316cfb198f888c5b3
instruction count 9042 sum bytes 45398 span 45411 body excl boundary 9041
gaps [('0x140352685', '0x14035268a', '0x14035268b'), ('0x14035333b', '0x140353340', '0x140353341'), ('0x1403586e4', '0x1403586e5', '0x1403586f0')] 3
direct jump/call mismatch [] missing/indirect edges [] RIP mismatches [] 0
site 0x140353d72 visited 162 reaching definitions [('0x140353427', 'MOV R14,qword ptr [RAX + R12*0x8]')] roots []
site 0x140353b21 visited 350 reaching definitions [('0x140353427', 'MOV R14,qword ptr [RAX + R12*0x8]')] roots []
site 0x1403548eb visited 1039 reaching definitions [('0x140353427', 'MOV R14,qword ptr [RAX + R12*0x8]')] roots []
site 0x140354334 visited 747 reaching definitions [('0x140353427', 'MOV R14,qword ptr [RAX + R12*0x8]')] roots []
predecessors numeric block [('0x140353772', 'JNZ 0x140353d60')]
R14 writes before read since vector load [('0x140353427', 'MOV R14,qword ptr [RAX + R12*0x8]')]
```

No runtime code, hooks, configuration, dispatch or billing changes were made in this review.


## Direct review of 851bfbf / RAW_EXPORTS_VEHICLE_DETAIL_ROUTE.md — 2026-10-29

Fetched the actual target branch and read all 427 lines of the supplied report. Artifact SHA-256: `bdf5b84d704b677998bac5349f73145c2ee05a2c920ee2234a0641166cbd3fdd`. No new binary/Ghidra execution here. Preserved local tracker-core work and previous review notes; did not merge over them.

### Supported evidence and audit limits

The report contains 15 addressed instruction lines total, including repeated short excerpts. The call at 0x14035433D decodes to 0x1406EE2A0; the renderer-entry call at 0x1407D555E decodes to 0x1406C27E0. The latter reads R15 from window+240 before calling the helper. These support a receiver-side row-object extraction **if** the requested dispatcher route is taken.

The dispatcher route itself is given as a prose summary of decompiler line numbers, not the actual dispatcher C or addressed instruction bytes. Conditional arithmetic on `(window+23C)-9U` is consistent with the claimed else path for zero, but does not independently show the branch context, call site, or arguments. Retain 7D5460 as the reported target, not a newly byte-verified dispatcher target in this review. The constructor can focus/reuse an existing window; do not assume it is always newly allocated.

### Creator-field claims are wrong for this request

Cross-checked tracked `decomp_1406EE2A0.c`, especially lines 283–327. `local_res20[0] = param_4`, which is 1 for the established row-click call. The entire row+1310/storage/1318 copy block is guarded by `local_res20[0] == 0`. It is therefore SKIPPED for this param4=1 request. These are not established inputs to the requested vehicle gauge.

Additionally, puVar15 is `undefined2*`, so the proposed destination fields again confuse element indices with byte offsets:

```text
puVar15 + 0x2AA -> byte offset +0x554 (not +0x2AA)
puVar15 + 0x2AC -> byte offset +0x558 (not +0x2AC)
puVar15 + 700   -> byte offset +0x578 (not +0x2BC)
```

The storage expression uses the already documented building +970/+978 vector with E0-byte entries. Do not restart that building-storage branch as a truck-gauge search. DAT_14090AA78 need not be read for this skipped path.

### Register index and helper interpretation corrections

R14's actual row load is `*(contextCC38Begin + R12*8)`, not `array[count]`. The earlier `(end-begin)/8` in R14 is a count/limit before the register is overwritten. The report's explicit formula substitutes that count for the index and implies a one-past-end element; it is not the supplied read.

C650 is the selected-sidebar-entry association in prior code, not an established fleet base. Distinct CC38 and DO+C70/C78 containers may still hold the same object pointers; differing vector addresses establish neither equality nor disjointness. Static analysis can sometimes establish the object relationship; actual same-instance runtime correlation may still be needed.

The supplied 6C27E0 C excerpt starts its index at -1 and explicitly checks the row itself BEFORE its +3A0/+3A8 children. It returns the first nonzero +1AD0 value from that traversal, not automatically a fuel quantity. The excerpt omits lVar1's declaration, so it is not a complete verbatim decompiler source as written. There is no justification to chase its writers for fuel merely because this is the first helper called.

### Gauge investigation remains missing, not a verified negative

Only four renderer-entry instructions are provided for the 91,932-byte 7D5460 function. The assertion that it never reads fuel numerically is unsupported by this export. Returning strings from GetValueString does not exclude prior numeric formatting, another branch or delegated drawing. The cited RAW_EXPORTS_772400_7752A0.md exports different functions, not this renderer. The unit ID57001 comes from 34D590's alternative display, not from a proven gauge in 7D5460; keep it unresolved and do not make it a prerequisite for following the actual gauge.

### Next bounded request

Obtain raw dispatcher branch/call evidence for +34=0, +248=1, +24C=2, +23C=0, with +240=row. Then export the actual selected renderer's complete instruction body and exact ranges (7D5460 if the branch confirms that target), plus decompiler output if it succeeds; a timeout is acceptable. No ellipses, no summary-only assertions of no fuel reads, no next-function boundary instruction, and distinguish body holes/padding. Include exact commands/results and complete export-script source in ONE Markdown. Do not rescan strings or revisit the skipped creator building-storage fields. This is the same raw-evidence approach that let the local 34D590 CFG trace resolve R14 without a decompiler. Once raw rendering code is available, perform the numeric/gauge trace here rather than another offset interpretation loop.

### Parallel implementation status

Unlike older historical review entries, a C++ journey core now EXISTS in `plugins/private_logistics/journey_tracker.h` with 30 native synthetic scenarios (including 1,000 generated journeys); full implementation and actual regression results are in JOURNEY_TRACKER_CORE.md. It remains disconnected from native callbacks and cannot measure physical fuel. This report does not justify wiring it. Billing stays OFF; dispatch and all runtime settings remain unchanged. No game test requested.

Actual checks in this review: decoded both supplied E8 call targets, counted the 15 instruction lines, compared the creator gate/typed-pointer stores against existing source, and computed the three byte offsets above using integer multiplication. These are report/source checks, not native runtime validation.

---

## Direct review of d00eaae: numeric litres and gauge trace (2026-10-29)

### Outcome: the actual numeric litres path is found

I fetched the committed raw file and analyzed its instruction bytes locally, rather than accepting the report summary. **7D5460 contains both the `%.2fl` numeric display and a gauge-value calculation using the same aggregate.** Its input is +5F0 on the panel's +240 object and qualifying +3A0/+3A8 linked objects. This disproves the earlier assertion that the renderer never reads fuel numerically. It establishes a display-source chain, NOT an authoritative physical fuel/burn source or live equality with the selected DO fleet entry.

No runtime integration, INI/default change, billing, commit, push or PR was performed in this review. Existing disconnected C++ core and working dispatch are preserved.

### Artifact, tools and independently checked boundary

- Source: `d00eaae:docs/private-logistics-runtime/RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md`, retrieved with `git fetch origin DO-fuel-cost-calculatotr` and `git show` into `.cache/detail-renderer-original.md`.
- Actual Markdown SHA-256: `2f3c638236c3c7d2e26cd97fe46d570748b497b1a69272c7358cac7877ce20e0`.
- Supplied executable identity: v1.1.1.9, PE timestamp 6A3EB6AD, image base 140000000, SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`. This identifies the reported target; I cannot independently authenticate the absent executable here.
- Local analysis: Python 3.11, Capstone 5.0.7 decoding of EVERY supplied instruction in the actual function, direct call/jump target checks, RIP-relative operand checks, instruction-level CFG predecessor walks and the prior complete string-inventory JSON. No local Ghidra or game run. The supplied 120-second decompiler timeout is acceptable; no retry needed.
- Actual renderer: **1407D5460 through RET at 1407EBB7B**, end-exclusive 1407EBB7C. **18,611 instructions, 91,932 bytes = 0x1671C; zero holes.** Every row decodes to exactly one instruction of the supplied length. Zero direct-transfer/RIP-relative textual target mismatches.
- The larger export has 18,893 instruction rows, including **282 instructions from the next function starting 1407EBB80**. It ends partway through that function: its final instruction starts 1407EBFBA and extends through 1407EBFBD. Do not analyze that following function under the renderer's register/frame meanings. The report's 18,895 / 0x1669C / inclusive 7EBFBC metadata are not accurate, but the relevant renderer itself IS complete and continuous.
- Addresses prefixed 140... are preferred-image virtual addresses, not RVAs as the report header claims. Subtract 140000000: renderer RVA 7D5460, formatting-call RVA 7E013A, constant RVA 90B07C.

### Dispatcher and same-panel-object provenance

1. At 6EA4EE/4F3, window+34==0 selects 6EA7E6. There +248==1 eventually reaches 6EA93B. With +23C==0, subtracting 9 and the unsigned comparison do not take the exceptional 9/10 submode branch. The ordinary call at **6EA958 targets 7D5460**.
2. **RCX is context (RDI); RDX is window (RSI)** at 6EA946/94C. The report's swapped labels are wrong. Renderer entry confirms R12=RDX/window at 7D54FD, R13=RCX/context at 7D5504.
3. **Window+4F8!=0 is NOT a prerequisite for this renderer call.** Its zero branch at 6EA485 skips an earlier draw, and both paths converge at 6EA4DA before primary/subkind dispatch. +24C==2 selects that earlier draw-test path, not a different receiver layout.
4. **7D5550 loads R15=[window+240]**; 7D5557 saves it in [RBP+40]. The previous verified creator chain from the clicked CC38 row supplies exactly this +240 value.
5. R15 is reused temporarily elsewhere in this large function; I did not assume it retains its initial meaning everywhere. Backwards CFG walks at the quantity read (7DF743) and format call (7E013A) find only three reaching definitions: 7D5550, 7DDB22 and 7DDECA. The latter two restore [RBP+40]. Backwards memory-definition walks at both restores find only 7D5557, the original save. Later reuses of that stack slot at 7E813F/8195 do not reach these sites. There are no indirect jumps in this function's supplied bytes.
6. Therefore this numeric block operates on the original **window+240 object plus its linked objects**, not a pointer guessed from a reused register. This is a static intra-function provenance result, under the normal x64 nonvolatile-register/stack-frame ABI. It does not prove that this object is an entry in a particular DO+C70/C78 vector; that identity comparison remains outstanding.

### Aggregation feeding the gauge: 7DF6CE–7DF7F8

XMM13 is zero: its sole reaching definition at aggregation initialization is XORPS XMM13,XMM13 at 7D568C. XMM11 (current), XMM8 (capacity) and XMM15 (another quantity) are initialized from it at 7DF6CE/D2/D6.

The iterator starts at -1: the negative index selects R15 itself (7DF710–723), then indices 0..count-1 select pointers from [R15+3A0], with count ([R15+3A8]-[R15+3A0])/8. This is root PLUS linked objects, not just the vector and not an arbitrary fleet sum.

For the current/capacity pair:

- 7DF6F8–707 obtains root metadata at [root+1708] and its +8684 capacity (XMM1).
- 7DF723/727 requires that ROOT capacity to be positive.
- 7DF729–73C additionally requires the CURRENT traversed object's metadata +8684 to be positive.
- 7DF73E adds that object's capacity to XMM8.
- **7DF743 reads float [object+5F0]. 7DF74B/74F accepts only values greater than zero; 7DF751 adds those to XMM11.** Negative values and unordered comparisons do not contribute. This display filtering must not become a rule that silently converts an invalid runtime measurement into valid zero.
- 7DF797–7DF7F8 can scale XMM8 further: if the +19E0/+19E8 pointer vector has at least one entry, the vector count divided by `FUN_1401FFB00(rootMetadata+7C78)` multiplies capacity. XMM11 is NOT scaled here. The callee's domain meaning remains untyped; do not assume this adjustment applies to every road truck.

The separate root metadata +8688 gate/additions produce XMM15; it is used in another iteration (ESI=3), not the numeric litres quantity. Between 7DF6CE and 7E013A the ONLY writes to XMM11 are its zero initialization and the +5F0 addition at 7DF751.

### Exact numeric formatting and gauge use: ESI=2

The ESI loop runs 0..3 (7DF86E–7E052E), with eligibility checks. In particular context+5B0 must be >=2 to process ESI>=2; root metadata+294==7 skips the loop, and the capacity/flag tests choose different presentations. These are not all road-vehicle type declarations.

At 7E00E6–7E013A, for ESI==2, XMM8>0 and ordinary presentation byte BL==0:

```asm
1407e00fd: 41 0F 28 C3                 MOVAPS XMM0,XMM11
1407e0101: F3 41 0F 5E C0              DIVSS XMM0,XMM8
1407e0106: 41 0F 28 FD                 MOVAPS XMM7,XMM13
1407e010a: F3 0F 5F F8                 MAXSS XMM7,XMM0
1407e010e: 41 0F 28 CD                 MOVAPS XMM1,XMM13
1407e0112: F3 41 0F 5F CB              MAXSS XMM1,XMM11
1407e0117: F3 0F 59 CE                 MULSS XMM1,XMM6
1407e011b: 0F 57 DB                    XORPS XMM3,XMM3
1407e011e: F3 0F 5A D9                 CVTSS2SD XMM3,XMM1
1407e0122: 66 49 0F 7E D9              MOVQ R9,XMM3
1407e0127: 4C 8D 05 6A 3D 11 00        LEA R8,[0x1408f3e98]
1407e012e: BA 00 01 00 00              MOV EDX,0x100
1407e0133: 48 8D 8D 00 28 00 00        LEA RCX,[RBP + 0x2800]
1407e013a: E8 51 51 82 FF              CALL 0x140005290
```

The already-exported terminated UTF-16LE literal at **1408F3E98** is **`%.2fl`**, bytes `25 00 2e 00 32 00 66 00 6c 00` followed by its terminator (inventory file offset 8F2E98). This is a genuine literal lowercase-l suffix, NOT the earlier ASCII false hit inside wide `%ls`. The numeric float32 multiplication is promoted to double for the variadic formatting argument. Precision is two decimal places.

XMM6's only reaching definitions at 7E0117 are MOVSS at **7DF848** and **7E0519**, both loading **float [14090B07C]**. Thus:

```text
current = sum of qualifying positive float(object+5F0), root plus linked objects
capacity = aggregate positive metadata+8684, with the optional adjustment above
gauge_ratio = max(0, current/capacity)
displayed_litres = float32(current * float_at_14090B07C), formatted "%.2fl"
```

**The address of the conversion factor is proved; its four data bytes/value are NOT supplied by this instruction export.** Do not silently substitute 1,000, density 0.84, or any earlier configured tonnes conversion. In particular the display's `l` does not by itself prove the underlying +5F0 unit is physical tonnes.

Special presentations must be kept separate: BL derives from rootMetadata+8681 when ESI==2. DIL derives from rootMetadata+7C58 via 02EBC0 / rootMetadata+8682. BL==1 or DIL==1 selects 7E017E–7E01CC, which can overwrite the text with **UNSCALED** max(0,XMM11) and localized unit ID0244 or024E using `%.2f %ls` at 8F3E48. These are not the prior unproven unit57001. The ordinary literal-litres text remains the final display when BL==0 and DIL!=1.

The numeric buffer [RBP+2800] is passed as a stack argument to the text draw at **7E033D**. In the ordinary presentation, **7E0353–7E036D transforms the same XMM7 ratio with constants at 90A678/90A32C and passes it to the UI operation through [14086C960]**; 7E039F–3AA resets that UI value to zero afterward. This ties the quantity to the gauge's drawing transform rather than finding an unrelated unit string. The transform constants, imported method names and face artwork are not dumped here; no claim about their exact angle/label semantics is necessary. The numeric branch establishes the sought litres display without relying on identifying an E/F text literal.

### A renderer-side +5F0 write, not a burn event

At **7D71FD**, the supplied bytes also store immediate `3C23D70A` to [R15+5F0], approximately **0.01f**. The local gate is context byte +9!=0 plus nonzero results from UI/input imports [86C6F8] with EDX=1D and [86C8C8] with EDX=21 (7D71CA–7D71FB). This is a conditional UI-side assignment, not an observed simulation deduction, refill amount or physical-tank clamp. Without imported function identity I do not name its keys or call it a confirmed cheat. Its existence further prevents treating every +5F0 change as actual burned fuel.

### What is proved, and what is still not proved

| Question | Result after this review |
|---|---|
| Actual selected renderer | 7D5460, supported by dispatcher bytes |
| Numeric litres formatting | 7E013A / format 8F3E98 `%.2fl` |
| Quantity feeding it | Positive +5F0 aggregate on window+240 root and qualifying linked objects |
| Gauge quantity relationship | Same aggregate/capacity ratio flows into the drawing transform |
| Conversion | Multiply by float at 90B07C; its value still needs a four-byte data read |
| Same clicked row object | Creator +240 chain and renderer CFG establish this statically |
| Same DO-fleet truck instance | Not yet established; compare actual panel root/linked pointers with DO+C70/C78 entries |
| Physical tank authority / exact burn | NOT established by a UI display path |
| Live screenshot matching | No contemporaneous panel/memory sample; cannot compare 549.54l with earlier logs |
| Cargo fuel resource C330 vs C3F0 | Still unresolved; renderer trace does not read either |
| Billing / native journey integration | OFF / disconnected, unchanged |

Older engine documentation calls +5F0 a derived mirror and proposes cargo resource slots. Those source/representation claims still need receiver provenance for this DO truck; the failed C3F0 logs cannot be repaired by merely declaring the now-proved UI quantity authoritative. The new result explains exactly what the gauge displays, not why the previous candidate object remained constant.

### Next work: bounded verification, not another renderer export

The complete renderer request is CLOSED. No new full render/decompiler export or general string scan is needed. The next evidence should resolve the remaining ambiguity rather than rerun the unchanged fuel reader:

1. Read exactly four bytes at preferred-image VA **14090B07C** (RVA90B07C; expected PE raw offset90A07C if the existing .rdata mapping still holds). Report actual address-to-file mapping, bytes, float32 value and binary hash. This is data absent from the supplied export, not an unperformed local arithmetic calculation. Also retain the format literal/terminator as a small cross-check if exporting data.
2. A meaningful logging-only runtime diagnostic must capture the actual **opened panel's +240 root**, its qualifying linked objects, raw +5F0 values, capacity/flags and the scale, and compare those object pointers directly against the DO fleet for the truck the user opened. A name or the only apparently eligible truck is insufficient. Record the ordinary/special branch so a generic-unit presentation is not misreported as litres. This requires a verified observer in the existing plugin; no such observer is installed by this documentation review.
3. Once that identity is verified, trace the actual simulation write receiver and clamped deducted amount for that SAME vehicle; correlate refills separately. Use the existing burn/refuel candidate evidence, but do not promote +5F0 samples, UI clamp/rounding or this 0.01 assignment to actual-burn events. No money or exact-journey claim until the actual event/coverage requirements are met.

For a remote data check, a sufficient bounded request is: "In the already analyzed executable, export the PE section mapping and four raw bytes at VA14090B07C/RVA90B07C, interpreted as little-endian float32; cross-check UTF-16LE VA1408F3E98 through its terminator. Give the executable hash, exact commands and actual output in ONE Markdown. Do not export 7D5460 again, rerun its decompiler, infer 1,000, or declare +5F0 physical tank authority."

### Complete local audit source and actual output

Environment setup: the initial system-wide pip attempt was rejected by the externally-managed-environment guard; no override was used. `python3 -m pip install --quiet --target .cache/python-audit capstone` succeeded (5.0.7). Actual audit command: `PYTHONPATH=.cache/python-audit python3 .cache/audit_detail.py`. The code below is the COMPLETE script actually executed, including the register/stack reaching-definition checks; its cache files are scratch, not production dependencies. The input is the exact fetched Markdown named above, and the prior inventory already embedded in VEHICLE_GAUGE_STRING_SEARCH_FIXED.md. This is static export validation, not a native executable/game test.

```python
from pathlib import Path
import re, collections, hashlib, json, struct
import capstone as cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_RIP
raw = Path('.cache/detail-renderer-original.md').read_bytes()
s = raw.decode('utf-16') if raw[:2] in (b'\xff\xfe', b'\xfe\xff') else raw.decode('utf-8-sig')
body = s.split('### 2.2', 1)[1].split('```asm', 1)[1].split('```', 1)[0]
rows = [(int(m[1],16), bytes.fromhex(m[2]), m[3].strip()) for m in re.finditer(
    r'^([0-9a-f]{9}):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+([^\r\n]+)', body, re.M)]
core = [(a,b,t) for a,b,t in rows if a <= 0x1407ebb7b]
print('Artifact SHA256',hashlib.sha256(raw).hexdigest())
print('Whole export',len(rows),'instructions; renderer through RET',len(core),'instructions',sum(len(b) for a,b,t in core),'bytes')
print('Through RET span',core[-1][0]+len(core[-1][1])-core[0][0])
print('Gaps',[(hex(a+len(b)),hex(core[i+1][0]),core[i+1][0]-a-len(b)) for i,(a,b,t) in enumerate(core[:-1]) if a+len(b)!=core[i+1][0]])
print('Extraneous following-function instructions',len(rows)-len(core))
md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_64);md.detail=True
by={};bad=[];ripbad=[];branchbad=[]
for a,b,t in core:
 ins=list(md.disasm(b,a))
 if len(ins)!=1 or ins[0].size!=len(b):bad.append(hex(a));continue
 ins=ins[0];by[a]=ins
 lit=re.search(r'\[(0x[0-9a-f]+)\]',t)
 for o in ins.operands:
  if o.type==X86_OP_MEM and o.mem.base==X86_REG_RIP and lit:
   if a+len(b)+o.mem.disp!=int(lit[1],16):ripbad.append(hex(a))
 if (ins.group(cs.CS_GRP_JUMP) or ins.group(cs.CS_GRP_CALL)) and ins.operands[0].type==X86_OP_IMM:
  lit=re.search(r'\b(0x[0-9a-f]+)\b',t)
  if lit and int(lit[1],16)!=ins.operands[0].imm:branchbad.append(hex(a))
print('Decode-size failures',bad,'RIP mismatches',ripbad,'direct transfer mismatches',branchbad)
pred=collections.defaultdict(set);unresolved=[]
for a,i in by.items():
 outs=[]
 if i.group(cs.CS_GRP_JUMP):
  if i.operands[0].type==X86_OP_IMM:outs.append(i.operands[0].imm)
  else:unresolved.append(hex(a))
 if i.mnemonic not in ('jmp','ret','int3'):outs.append(a+i.size)
 for dst in outs:
  if dst in by:pred[dst].add(a)
print('Indirect JMP sites',unresolved)
def reaching(site,reg):
 q=list(pred[site]);seen=set();defs=set();roots=set()
 while q:
  a=q.pop()
  if a in seen:continue
  seen.add(a);i=by[a]
  aliases={reg,reg+'d',reg+'w',reg+'b'}
  if aliases & {i.reg_name(r) for r in i.regs_access()[1]}:
   defs.add(a);continue
  if not pred[a]:roots.add(a)
  q.extend(pred[a])
 print('Reaching',hex(site),reg,[(hex(a),by[a].mnemonic,by[a].op_str) for a in sorted(defs)],'roots',[hex(x) for x in roots])
for site in (0x1407df743,0x1407e013a):reaching(site,'r15')
print('RBP+40 writes',[(hex(a),t) for a,b,t in core if re.match(r'(MOV|LEA) (?:qword ptr )?\[RBP \+ 0x40\],',t)])
print('XMM11 writes in gauge block',[(hex(a),i.mnemonic,i.op_str) for a,i in by.items() if 0x1407df6ce<=a<=0x1407e013a and 'xmm11' in {i.reg_name(r) for r in i.regs_access()[1]}])
print('Scalar data write',hex(0x1407d71fd),struct.unpack('<f',bytes.fromhex('0ad7233c'))[0])
# Inspect the previously supplied JSON inventory, not native memory.
v=Path('docs/private-logistics-runtime/VEHICLE_GAUGE_STRING_SEARCH_FIXED.md').read_text(encoding='utf-8-sig')
a=v.index('```json\n')+8;v=json.loads(v[a:v.index('\n```',a)])
print('Gauge literal',[c for c in v['candidates'] if c['string_va']=='0x1408f3e98' and c['encoding']=='UTF-16LE'])
def reaching_slot(site,disp):
 q=list(pred[site]);seen=set();defs=set()
 while q:
  a=q.pop()
  if a in seen:continue
  seen.add(a);i=by[a]
  if i.operands and i.operands[0].type==X86_OP_MEM and i.reg_name(i.operands[0].mem.base)=='rbp' and i.operands[0].mem.disp==disp and i.operands[0].access & cs.CS_AC_WRITE:
   defs.add(a);continue
  q.extend(pred[a])
 print('Reaching stack',hex(site),hex(disp),[(hex(a),by[a].mnemonic,by[a].op_str) for a in sorted(defs)])
for site in (0x1407ddb22,0x1407ddeca):reaching_slot(site,0x40)
reaching(0x1407df6ce,'xmm13')
reaching(0x1407e0117,'xmm6')
print('Correct hex size',hex(91932))
```

Actual output:

```text
Artifact SHA256 2f3c638236c3c7d2e26cd97fe46d570748b497b1a69272c7358cac7877ce20e0
Whole export 18893 instructions; renderer through RET 18611 instructions 91932 bytes
Through RET span 91932
Gaps []
Extraneous following-function instructions 282
Decode-size failures [] RIP mismatches [] direct transfer mismatches []
Indirect JMP sites []
Reaching 0x1407df743 r15 [('0x1407d5550', 'mov', 'r15, qword ptr [rdx + 0x240]'), ('0x1407ddb22', 'mov', 'r15, qword ptr [rbp + 0x40]'), ('0x1407ddeca', 'mov', 'r15, qword ptr [rbp + 0x40]')] roots []
Reaching 0x1407e013a r15 [('0x1407d5550', 'mov', 'r15, qword ptr [rdx + 0x240]'), ('0x1407ddb22', 'mov', 'r15, qword ptr [rbp + 0x40]'), ('0x1407ddeca', 'mov', 'r15, qword ptr [rbp + 0x40]')] roots []
RBP+40 writes [('0x1407d5557', 'MOV qword ptr [RBP + 0x40],R15'), ('0x1407e813f', 'MOV qword ptr [RBP + 0x40],RCX'), ('0x1407e8195', 'MOV qword ptr [RBP + 0x40],RBX')]
XMM11 writes in gauge block [('0x1407df6ce', 'movaps', 'xmm11, xmm13'), ('0x1407df751', 'addss', 'xmm11, xmm0')]
Scalar data write 0x1407d71fd 0.009999999776482582
Gauge literal [{'string_va': '0x1408f3e98', 'file_offset': '0x8f2e98', 'encoding': 'UTF-16LE', 'terminated': True, 'text': '%.2fl', 'bytes_hex': '25002e00320066006c00', 'reasons': ['float_format'], 'interior_anchor_vas': ['0x1408f3e98'], 'section': '.rdata'}]
Reaching stack 0x1407ddb22 0x40 [('0x1407d5557', 'mov', 'qword ptr [rbp + 0x40], r15')]
Reaching stack 0x1407ddeca 0x40 [('0x1407d5557', 'mov', 'qword ptr [rbp + 0x40], r15')]
Reaching 0x1407df6ce xmm13 [('0x1407d568c', 'xorps', 'xmm13, xmm13')] roots []
Reaching 0x1407e0117 xmm6 [('0x1407df848', 'movss', 'xmm6, dword ptr [rip + 0x12b82c]'), ('0x1407e0519', 'movss', 'xmm6, dword ptr [rip + 0x12ab5b]')] roots []
Correct hex size 0x1671c
```

---

## a38a6ae PE-data review and implemented panel observer (2026-10-29)

### Handover / current result

**No further broad renderer prompt is needed.** I fetched and read `a38a6ae:docs/private-logistics-runtime/RAW_PE_DATA_READ.md` directly. The ordinary literal-litres display is now:

`format("%.2fl", double(float32(positive qualifying +5F0 aggregate * 1000.0f)))`.

This is a display-unit conversion, not independent proof of physical tonnes or exact fuel deductions. To test the remaining identity problem, I implemented the next bounded logging-only observer INSIDE `private_logistics`, not in a detached model or another plugin. Source version is **0.18.6-panel-observer**. It is compiled into the existing plugin and installs its verified renderer hook at Start when prerequisites pass. The journey core still has no runtime consumer and billing remains OFF.

**Not yet a tested Windows DLL:** the environment has native Linux g++ but no MSVC/Windows compiler or game. The reader and production queue/control code are tested synthetically below; no DLL build, hook execution in game, live panel match, dispatch regression in game or native burn-event validation is claimed. No commit, push, PR or deployment was done in this turn; these files are saved on arena/15fdac3e-wrsre. A VS Code pull cannot fetch this unpushed observer yet. Publishing the checkpoint and obtaining a Windows build is the next delivery step, before requesting the game run.

### Direct review of the new report

- Retrieved with `git fetch origin DO-fuel-cost-calculatotr`, then `git show a38a6ae:docs/private-logistics-runtime/RAW_PE_DATA_READ.md`.
- Actual Markdown SHA-256: **db2ebe8375e59bfbfe771a62ff3fb92010e70ce12537ac0fa001404ced2ab83c**.
- Reported binary hash matches the earlier renderer target: `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`.
- Independently computed `struct.unpack('<f', bytes.fromhex('00007a44'))[0] == 1000.0`.
- Independently computed `86B000 + (90B07C - 86C000) == 90A07C` and `86B000 + (8F3E98 - 86C000) == 8F2E98`.
- Literal `%.2fl` and terminator agree with the earlier complete string inventory.
- Evidence limitation: the report's source snippet hardcodes the section mapping and leaves the string-reading code as a comment; it is NOT a complete executable reproduction of all reported output or a fresh PE-header parse. The bytes/results are internally consistent, but I do not independently possess/hash the executable. The report also labels its generation date 2026-10-30, ahead of this session's local 2026-10-29 date; that is recorded as supplied metadata, not treated as an independent timestamp.
- The observer verifies the scale bytes and the full literal including terminator in the actual loaded module before patching. It additionally checks PE architecture/timestamp and exact renderer prologue; this is a fingerprint, NOT a recomputed full SHA-256.

The complete supplied 86-line report is preserved below as received, so its actual commands/omissions remain visible.

### Production observer behavior and safety

Files added:

- `plugins/private_logistics/panel_fuel_observer.h`: actual read-only capture/aggregation/membership implementation, tested with synthetic memory through a read callback.
- `plugins/private_logistics/panel_fuel_runtime.h`: actual Windows host adapter, verified hook installer, fixed queue, epoch reset, deferred logger and diagnostic limits. Private to this plugin; no cross-plugin internal includes.
- `tests/private_logistics/panel_fuel_observer_test.cpp`: 16 native scenarios of the production reader, including failed reads, root/child eligibility, exact fleet membership, overflow and no writes.
- `tests/private_logistics/panel_fuel_runtime_test.cpp`: five host-shim scenarios of the actual production queue/control header, not a duplicated queue model. It includes the reader-test fixture with its entry point renamed. The shim substitutes Windows types/locks/interlocked calls/SEH syntax; it does NOT validate Windows ABI, real SEH, patch execution or real thread races.
- `tests/private_logistics/test_panel_fuel_observer.py`: native compiler runner plus source/default/fingerprint guards.

Existing plugin changes are narrowly scoped: version and update comment; default-enabled observer setting; one load-epoch signal at the beginning of the existing persistence callback; include the private runtime adapter; call its Start after the existing persistence registration. No dispatch, target percentages, task activation, money routines, persistence file format or save permission logic was changed. The previously added journey-core include remains; there is still no Tracker instance/Consume call.

Hook verification:

- Renderer RVA **7D5460**. Detour signature is `(context, window)` on x64, matching dispatcher RCX/RDX and renderer entry. The dispatcher does not use a renderer return value.
- **20 stolen bytes**, complete pushes and `LEA RBP,[RSP-2EB0]`, before the stack-allocation helper call. No relative branch or RIP-relative operand in these bytes. Capstone independently decoded all eight instructions and their exact boundary.
- Full expected-byte array passed to `InstallInlineHook`; both the plugin and host refuse mismatch. No manual code cave/partial-instruction patch.
- The original trampoline is called exactly once before capture. Capture is explicitly **post-render**, not an atomic sample at the numeric-format instruction; UI actions or simultaneous simulation changes can cause a difference. The observer does not write game data, call the untyped metadata helpers, draw UI, change original renderer arguments, or feed the journey tracker.
- Fingerprint mismatch, billing enabled, missing sim_clock tick or missing existing persistence-load subscription -> observer refuses installation with a reason. A conflicting existing renderer patch fails the prologue check safely.

Bounded sampling/logging:

- One GLOBAL eligible-window sample per **1,000 ms**, at most **240 snapshots per load epoch**, **16 queued records**, root plus at most **eight** linked objects, and at most **256** entries per candidate fleet. Too many links do NOT produce a truncated/partial aggregate.
- Close other vehicle-detail windows for this test: with multiple eligible windows, the first rendered window can consume the global sample budget. Ineligible/hidden windows do NOT consume the time slot (covered by the host-shim scenario).
- Render capture uses `TryEnterCriticalSection`, preallocated storage, guarded read callback and SEH. No render-side allocations, blocking lock acquisition, disk output or Logf calls. A guarded access fault disables further capture for the process and is reported later; do not rely on a reload to re-enable a faulted observer.
- The sim_clock tick drains one snapshot per callback, releasing the queue lock before formatting/logging. Paused captures queue and are logged after unpausing, preserving capture milliseconds. Queue-full attempts are counted; they do not become an exact event stream. At the 240-snapshot cap the logger reports the cap; a restart/load starts another budget, not a continuous journey.
- Persistence loading only increments an interlocked diagnostic epoch. Old queued records are discarded; a capture crossing that signal is not enqueued under a new epoch. No captured pointer/identity is persisted. This is diagnostic generation tagging, not proof of a complete native lifecycle or a durable vehicle identity. The permission callback's existing file logic is unchanged.

### What each record means — do not over-read it

`[PANEL FUEL]` includes capture epoch/sequence/uptime milliseconds, window/root pointers, read status, aggregate_valid, raw current sum, **litres_candidate**, unadjusted capacity, optional-adjustment flags, ratio_valid/ratio, context mode, metadata kind, raw presentation flags and queue-full attempt count. It explicitly ends `physical_fuel=unverified exact_burn=0 billing=off`.

`[PANEL OBJECT]` identifies root versus linked index, exact object/metadata pointers, quantity_read, raw +5F0 (including negatives), metadata capacity, candidate-home pointer, membership result, fleet index and duplicate occurrence count. On an invalid snapshot, validity/status fields govern: default zeros are NOT valid measurements. Negative +5F0 is retained in the raw log even though the verified display sum excludes nonpositive contributors.

Membership is deliberately narrow and explicit:

1. Read **that actual captured object's +4F8** as an office CANDIDATE, not an automatic proof of its type/ownership.
2. Verify candidate building descriptor +318 -> type +360 == **43 (road DO)**, matching existing canonical building offsets/type evidence.
3. Read its +C70/+C78 fleet and require exact object-pointer equality with an actual fleet slot.
4. Log `exact_fleet_pointer` only after all reads of the bounded vector succeed. Count duplicate occurrences rather than silently treating them as one unique entry.

This does NOT enumerate every DO or private enterprise: `absent_from_candidate_home` is not proof that no other DO owns the object. It does NOT guess a truck from model name or unique eligible selection. It deliberately logs `private_status=not_classified`; selecting the known private office's truck is part of the controlled test. This observer is not yet a private-fleet journey eligibility adapter.

Presentation/capacity limits:

- `litres_candidate` reproduces the ordinary literal-litres arithmetic, not the special unscaled localized-unit branch.
- Nonzero +8681/+8682 establishes an alternate presentation if that gauge is shown. Both zero still require the native metadata+7C58 helper result, so the observer logs **presentation=unresolved_7c58**, not a guessed ordinary mode. It does not call that untyped helper. Context mode/metadata kind and raw flags are logged to expose omitted or special presentations.
- If +19E0/+19E8 is nonempty, native capacity adjustment is required. The raw current/litres candidate is still available, but ratio_valid is FALSE rather than guessing the divisor/helper result. Unreadable adjustment metadata likewise invalidates the ratio, not silently assuming an empty vector.
- Thus a live visual comparison is still required. A matching litres candidate, actual screenshot and exact membership are new evidence; a mismatch is INCONCLUSIVE about physical fuel and must be diagnosed using the raw object/branch data. No burn/refill amount or price is derived from sampled differences.

### Actual checks run in this turn

```text
python3 -m unittest discover -s tests/private_logistics -v
Ran 67 tests ... OK
python3 -m unittest discover -s tests/private_logistics_probe -v
Ran 93 tests ... OK
python3 -m unittest discover -s tests/private_logistics_v018_model -v
Ran 38 tests ... OK
python3 -m unittest discover -s tests/private_shops -v
Ran 4 tests ... OK
python3 -m unittest discover -s tests/vehicle_gauge_strings -v
Ran 14 tests ... OK
TOTAL: 216 regression tests PASS (67+93+38+4+14)
```

Actual optimized native compile/run:

```text
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -I plugins/private_logistics tests/private_logistics/panel_fuel_observer_test.cpp -o .cache/panel-observer-release
.cache/panel-observer-release
16 native scenarios PASS
```

Actual sanitizer compile/runs (both produced no sanitizer diagnostics):

```text
g++ -std=c++17 -O1 -g -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -I plugins/private_logistics tests/private_logistics/panel_fuel_observer_test.cpp -o .cache/panel-observer-sanitizers
ASAN_OPTIONS=detect_leaks=1 .cache/panel-observer-sanitizers
16 native scenarios PASS

g++ -std=c++17 -O1 -g -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -I plugins/private_logistics tests/private_logistics/panel_fuel_runtime_test.cpp -o .cache/panel-runtime-sanitizers
ASAN_OPTIONS=detect_leaks=1 .cache/panel-runtime-sanitizers
5 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)
```

The exact native scenario outputs are embedded below. The Python runner also compiles/tests production-header compatibility with Windows-style min/max macros, and checks INI/source defaults, hook bytes, nonblocking render path, load signal and deferred tracker wiring. These source guards are not execution of the Windows hook. `git diff --check` passes. No new full Windows DLL or actual game test was possible here.

### Next delivery and proposed game test (AFTER Windows build/deployment approval)

1. Publish a checkpoint/PR from the fixed Arena branch so the changes are available to the user's Windows checkout. **Not pushed yet: do not tell the user to pull an observer that is not on a remote.** Preserve the other pending tracker/investigation files when preparing that checkpoint.
2. Once published/merged into the user's working branch, update that branch in VS Code; do not merge unrelated investigator code into the runtime. From the Windows repository, run `cmd /c build.bat private_logistics`. Confirm the compiler finishes successfully and inspect `Get-Item .\build\plugins\private_logistics.dll | Select-Object LastWriteTime,Length`. This DLL build is still required; report any actual MSVC error before trying the game.
3. Confirm the game's SOVIET64.exe hash with `Get-FileHash 'C:\Workers Resources Soviet Republic\SOVIET64.exe' -Algorithm SHA256`; it must match the hash above. With the game CLOSED, back up the deployed private_logistics DLL AND INI, then deploy the newly built pair only after approval. build.bat copies the shipped INI into build/plugins; it does not by itself prove deployment to the game.
4. The SHIPPED next-test defaults are already **panel_fuel_observer=1, billing=0, dry_run=1**, in INI and missing-key fallbacks; no instruction to manually disable an otherwise-live billing default. Working **auto_source_percent=10 / auto_destination_percent=30** and auto-apply settings are preserved. Verify the deployed INI was actually replaced, because an explicit old key can override fallbacks. Missing observer prerequisites or a prologue mismatch should be reported, not worked around by forcing a patch.
5. Use a COPY of the known working save and ONE selected private road-DO truck. Close other vehicle detail panels, then open that truck's actual road-vehicle panel. Confirm `[PANEL FUEL] installed ... scale=1000 ... cap=240` in the log and plugin version `0.18.6-panel-observer`. If the observer reports unavailable/refused, stop and return the reason rather than running the unchanged old reader test.
6. Keep the panel open. Record the visible litres value while parked, drive/let the truck work while keeping the panel open, then record it parked again. Use normal speed and a short 1–3 minute run within the 240-snapshot budget. No need to wait a whole day. If paused for a screenshot, unpause briefly afterward to drain queued snapshots; capture milliseconds distinguish them from logger/drain time. Do not manipulate fuel/cargo to force an expected result.
7. Return the COMPLETE log for this short run (including startup/version, unavailable/fault/cap lines, all `[PANEL FUEL]` and `[PANEL OBJECT]` records) plus screenshots/video with visible parked/moving values and which office/truck was opened. Do not filter out errors or invalid samples. Primary questions: exact root/linked membership versus known DO, candidate value versus actual panel, and which raw value changes while driving.
8. This test may establish same-truck DISPLAY correlation. It does NOT validate actual clamped burn or completed journeys. Keep settlement OFF and the journey core disconnected; trace real deduction/refill receivers only after the identity/source correlation is evaluated. Source/quantity/presentation mismatches are diagnostic results, not permission to charge from endpoints.

Rollback: with the game closed, restore the backed-up DLL+INI. Alternatively an already built observer can be disabled with `panel_fuel_observer=0` and restart, leaving the working dispatch code intact. This is a rollback option, not the shipped next-test profile.

### Complete new report as received

````markdown
﻿# Raw PE Data Read — RVA 0x90B07C and RVA 0x8F3E98

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON
**SHA-256:** 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
**Generated:** 2026-10-30 via PowerShell direct file read

---

## 1. Command Source


```powershell
$p = "C:\Workers Resources Soviet Republic\SOVIET64.exe"
$data = [System.IO.File]::ReadAllBytes($p)
$hash = Get-FileHash -Path $p -Algorithm SHA256
# .rdata section: VA=0x86C000, RO=0x86B000
$rva1 = 0x90B07C
$fo1 = 0x86B000 + ($rva1 - 0x86C000)
$b1 = $data[$fo1..($fo1+3)]
[BitConverter]::ToSingle($b1, 0)
$rva2 = 0x8F3E98
$fo2 = 0x86B000 + ($rva2 - 0x86C000)
# read UTF-16LE through terminator
```

## 2. Executable Identity

| Field | Value |
|-------|-------|
| Path | C:\Workers Resources Soviet Republic\SOVIET64.exe |
| SHA-256 | 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8 |
| Image base | 0x140000000 |
| PE timestamp | 0x6A3EB6AD |
| Machine | 0x8664 (AMD64) |

## 3. Section Mapping

| Property | Value |
|----------|-------|
| RVA | 0x90B07C |
| Section | .rdata |
| Section VA range | 0x86C000 – 0x991C62 |
| Section raw offset | 0x86B000 |
| File offset | 0x86B000 + (0x90B07C − 0x86C000) = 0x90A07C |

## 4. Four Bytes at RVA 0x90B07C

| Format | Value |
|--------|-------|
| Raw bytes (LE) | 00-00-7A-44 |
| As uint32 LE | 0x447A0000 |
| As IEEE-754 float32 LE | 1000.0 |

## 5. UTF-16LE String at RVA 0x8F3E98

| Property | Value |
|----------|-------|
| RVA | 0x8F3E98 |
| File offset | 0x86B000 + (0x8F3E98 − 0x86C000) = 0x8F2E98 |
| Decoded string | "%.2fl" |
| Terminator | 00-00 |

## 6. Actual Command Output

### 6.1 SHA-256

```text
SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
```

### 6.2 Float32 at RVA 0x90B07C

```text
FO1: 0x90A07C
BYTES: 00-00-7A-44
FLOAT: 1000
```

### 6.3 UTF-16LE at RVA 0x8F3E98

```text
FO2: 0x8F2E98
STR: %.2fl
TERM: 00-00
```

````

### Actual complete native outputs

.cache/panel-observer-release.txt

```text
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS
```

.cache/panel-observer-sanitizers.txt

```text
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS
```

.cache/panel-runtime-sanitizers.txt

```text
PASS runtime_install_fingerprint
PASS runtime_post_render_throttle_and_deferred_log
PASS runtime_queue_bound_trylock_and_load_discard
PASS runtime_inactive_does_not_starve_capture_and_cap
PASS runtime_changed_data_refused
5 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)
```

### Complete final source for this implementation

The files below are embedded verbatim. Shared `src/` APIs, site_permissions.h and task_audit.h are unchanged repository dependencies; the journey-core header is included here because the current plugin includes it, but it has NO runtime consumer. Prior journey-core tests/results remain in JOURNEY_TRACKER_CORE.md. The runtime host-shim test includes the reader test source above it by relative filename.

#### plugins/private_logistics/panel_fuel_observer.h

SHA-256: `3392f202151bddd4d4946da1fd9d0c46d85b2768f68fe08ed6d515a432e2da12`

```cpp
// Read-only diagnostic of the verified 7D5460 panel quantity. NOT a burn meter.
// No native calls, allocations, logging, persistence, tracker or money here.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace private_logistics_panel {
using Address = std::uintptr_t;
using Read = bool (*)(Address, void*, std::size_t, void*);
constexpr unsigned MaxObjects = 9; // root + at most eight links; never partial totals
constexpr unsigned MaxFleet = 256;
struct Memory {
    Read read;
    void* user;
    template<class T> bool Get(Address base, Address offset, T& out) const {
        if (!read || !base || offset > UINTPTR_MAX - base ||
            sizeof(T) > UINTPTR_MAX - (base + offset)) return false;
        return read(base + offset, &out, sizeof(T), user);
    }
};
enum class Match { Unavailable, NotRoadOffice, InvalidFleet, Absent, Found };
enum class Status { Unreadable, TooManyLinks, NonFinite, Overflow, Complete };
struct Object {
    Address pointer = 0, metadata = 0, homeCandidate = 0;
    float raw = 0, capacity = 0;
    bool quantityRead = false;
    Match match = Match::Unavailable;
    unsigned fleetIndex = 0, occurrences = 0;
};
struct Snapshot {
    Address context = 0, window = 0, root = 0;
    Status status = Status::Unreadable;
    unsigned count = 0, links = 0;
    Object objects[MaxObjects]{};
    int contextMode = -1, metadataKind = -1;
    std::uint8_t flag8681 = 0, flag8682 = 0;
    bool flagsRead = false, adjustmentRead = false, adjustmentNeeded = false;
    bool aggregateValid = false, ratioValid = false;
    float current = 0, unadjustedCapacity = 0, litresCandidate = 0, ratio = 0;
};
// This tests ONLY object+4F8 as an office candidate. An absent match is NOT a
// search of all DOs. Type 43 and literal fleet-slot pointer equality are needed.
inline void FindMembership(const Memory& m, Object& o) {
    if (!m.Get(o.pointer, 0x4F8, o.homeCandidate) || !o.homeCandidate) return;
    Address descriptor = 0, begin = 0, end = 0;
    int type = -1;
    if (!m.Get(o.homeCandidate, 0x318, descriptor) || !m.Get(descriptor, 0x360, type)) return;
    if (type != 43) { o.match = Match::NotRoadOffice; return; }
    if (!m.Get(o.homeCandidate, 0xC70, begin) || !m.Get(o.homeCandidate, 0xC78, end)) return;
    if (end < begin || (end - begin) % sizeof(Address) ||
        (end - begin) / sizeof(Address) > MaxFleet || (!begin && end)) {
        o.match = Match::InvalidFleet; return;
    }
    const unsigned count = static_cast<unsigned>((end - begin) / sizeof(Address));
    for (unsigned i = 0; i < count; ++i) {
        Address entry = 0;
        if (!m.Get(begin, i * sizeof(Address), entry)) { o.match = Match::Unavailable; return; }
        if (entry == o.pointer) {
            if (!o.occurrences) o.fleetIndex = i;
            ++o.occurrences;
        }
    }
    o.match = o.occurrences ? Match::Found : Match::Absent;
}
// false = not an eligible visible detail window. true may contain an INVALID
// snapshot: callers must check status/aggregateValid, not interpret defaults as 0.
inline bool Capture(const Memory& m, Address context, Address window, Snapshot& s) {
    s = Snapshot{};
    s.context = context; s.window = window;
    std::uint8_t hidden = 1, live = 0;
    int primary = -1, subkind = -1, origin = -1, submode = -1;
    if (!m.Get(window, 1, hidden) || !m.Get(window, 0x20, live) || hidden || !live ||
        !m.Get(window, 0x34, primary) || !m.Get(window, 0x248, subkind) ||
        !m.Get(window, 0x24C, origin) || !m.Get(window, 0x23C, submode) ||
        primary != 0 || subkind != 1 || origin != 2 || submode != 0) return false;
    if (!m.Get(window, 0x240, s.root) || !s.root) return true;
    Address begin = 0, end = 0;
    if (!m.Get(s.root, 0x3A0, begin) || !m.Get(s.root, 0x3A8, end) ||
        end < begin || (end - begin) % sizeof(Address) || (!begin && end)) return true;
    const Address links = (end - begin) / sizeof(Address);
    if (links >= MaxObjects) { s.status = Status::TooManyLinks; return true; }
    s.links = static_cast<unsigned>(links); s.count = s.links + 1;
    for (unsigned i = 0; i < s.count; ++i) {
        Object& o = s.objects[i];
        o.pointer = s.root;
        if (i && !m.Get(begin, (i - 1) * sizeof(Address), o.pointer)) return true;
        FindMembership(m, o); // independent of quantity-read validity
        if (!m.Get(o.pointer, 0x1708, o.metadata) ||
            !m.Get(o.metadata, 0x8684, o.capacity) || !m.Get(o.pointer, 0x5F0, o.raw)) return true;
        o.quantityRead = true;
        if (!std::isfinite(o.capacity) || !std::isfinite(o.raw)) {
            s.status = Status::NonFinite; return true;
        }
    }
    m.Get(context, 0x5B0, s.contextMode);
    m.Get(s.objects[0].metadata, 0x294, s.metadataKind);
    s.flagsRead = m.Get(s.objects[0].metadata, 0x8681, s.flag8681) &&
                  m.Get(s.objects[0].metadata, 0x8682, s.flag8682);
    Address adjustBegin = 0, adjustEnd = 0;
    s.adjustmentRead = m.Get(s.root, 0x19E0, adjustBegin) &&
                       m.Get(s.root, 0x19E8, adjustEnd) && adjustEnd >= adjustBegin &&
                       (adjustEnd - adjustBegin) % sizeof(Address) == 0 &&
                       (adjustBegin || !adjustEnd);
    s.adjustmentNeeded = s.adjustmentRead && adjustEnd != adjustBegin;
    if (s.objects[0].capacity > 0) {
        for (unsigned i = 0; i < s.count; ++i) {
            const Object& o = s.objects[i];
            if (o.capacity <= 0) continue;
            s.unadjustedCapacity += o.capacity;
            if (o.raw > 0) s.current += o.raw;
        }
    }
    // The actual PE constant is 00 00 7A 44 = 1000.0f. Verify it at install.
    s.litresCandidate = s.current * 1000.0f;
    if (!std::isfinite(s.current) || !std::isfinite(s.unadjustedCapacity) ||
        !std::isfinite(s.litresCandidate)) { s.status = Status::Overflow; return true; }
    s.aggregateValid = true; s.status = Status::Complete;
    // Do not call the untyped native capacity adjustment or guess its result.
    s.ratioValid = s.adjustmentRead && !s.adjustmentNeeded && s.unadjustedCapacity > 0;
    if (s.ratioValid) s.ratio = s.current / s.unadjustedCapacity;
    if (!std::isfinite(s.ratio)) { s.ratioValid = false; }
    // flag8681/8682 can prove an alternate presentation, but both zero cannot
    // prove literal-litres mode without the native +7C58 helper's result.
    return true;
}
} // namespace private_logistics_panel
#endif
```

#### plugins/private_logistics/panel_fuel_runtime.h

SHA-256: `6028edbe6f5f500f12e6f9694c63e1de6901888b43fffb4a09687c82c8785598`

```cpp
// Private to this plugin. Included after its services/globals are defined.
// Process-lifetime hooks, like the plugin's existing transfer hooks.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#include "panel_fuel_observer.h"

static CRITICAL_SECTION g_panelQueueLock;
static volatile LONG g_panelEpoch = 1, g_panelStarted = 0, g_panelFault = 0;
static LONG g_panelQueueEpoch = 0;
static unsigned g_panelHead = 0, g_panelCount = 0, g_panelSamples = 0, g_panelDropped = 0;
static ULONGLONG g_panelLastMs = 0;
static bool g_panelCapLogged = false, g_panelFaultLogged = false;
struct PanelRecord {
    private_logistics_panel::Snapshot snapshot;
    LONG epoch;
    unsigned sequence;
    ULONGLONG milliseconds;
};
static PanelRecord g_panelQueue[16];
using PanelRenderer = void (__fastcall*)(void*, void*);
static void* g_panelTrampoline = nullptr;

static void PanelObserverLoading(void)
{
    // File thread: no game reads, queue lock, retained identities or persistence.
    InterlockedIncrement(&g_panelEpoch);
}
static bool PanelRead(uintptr_t address, void* out, size_t size, void*)
{
    if (address < 0x10000 || !ReadablePtr((const void*)address, size)) return false;
    memcpy(out, (const void*)address, size);
    return true;
}
static void PanelResetQueueLocked(LONG epoch)
{
    if (g_panelQueueEpoch == epoch) return;
    g_panelQueueEpoch = epoch;
    g_panelHead = g_panelCount = g_panelSamples = g_panelDropped = 0;
    g_panelLastMs = 0;
    g_panelCapLogged = false;
}
static void PanelCapture(void* context, void* window)
{
    if (!InterlockedCompareExchange(&g_panelStarted, 0, 0) ||
        InterlockedCompareExchange(&g_panelFault, 0, 0) ||
        !TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    const ULONGLONG now = GetTickCount64();
    if (g_panelSamples >= 240 || (g_panelLastMs && now - g_panelLastMs < 1000)) {
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    if (g_panelCount == 16) {
        g_panelLastMs = now;
        if (g_panelDropped < 0x7FFFFFFFu) ++g_panelDropped;
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    const unsigned slot = (g_panelHead + g_panelCount) % 16;
    bool captured = false;
    __try {
        captured = private_logistics_panel::Capture({PanelRead, nullptr},
            (uintptr_t)context, (uintptr_t)window, g_panelQueue[slot].snapshot);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_panelFault, 1); // no logging/I/O in the render hook
    }
    if (captured && epoch == InterlockedCompareExchange(&g_panelEpoch, 0, 0)) {
        g_panelLastMs = now; // inactive windows must not consume the sample slot
        g_panelQueue[slot].epoch = epoch;
        g_panelQueue[slot].sequence = ++g_panelSamples;
        g_panelQueue[slot].milliseconds = now;
        ++g_panelCount;
    }
    LeaveCriticalSection(&g_panelQueueLock);
}
static void __fastcall PanelRendererDetour(void* context, void* window)
{
    ((PanelRenderer)g_panelTrampoline)(context, window);
    PanelCapture(context, window); // POST render, not an atomic read at the format call
}
static const char* PanelStatus(private_logistics_panel::Status s)
{
    using private_logistics_panel::Status;
    switch (s) {
    case Status::Complete: return "complete";
    case Status::TooManyLinks: return "too_many_links";
    case Status::NonFinite: return "nonfinite";
    case Status::Overflow: return "overflow";
    default: return "unreadable";
    }
}
static const char* PanelMatch(private_logistics_panel::Match m)
{
    using private_logistics_panel::Match;
    switch (m) {
    case Match::Found: return "exact_fleet_pointer";
    case Match::Absent: return "absent_from_candidate_home";
    case Match::NotRoadOffice: return "candidate_home_not_road_do";
    case Match::InvalidFleet: return "invalid_candidate_fleet";
    default: return "unavailable";
    }
}
static void PanelObserverTick(void*, float, float)
{
    PanelRecord record{};
    bool have = false, cap = false, fault = false;
    unsigned dropped = 0;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    if (g_panelCount) {
        record = g_panelQueue[g_panelHead];
        g_panelHead = (g_panelHead + 1) % 16; --g_panelCount; have = true;
    }
    dropped = g_panelDropped;
    cap = g_panelSamples >= 240 && !g_panelCapLogged;
    if (cap) g_panelCapLogged = true;
    fault = InterlockedCompareExchange(&g_panelFault, 0, 0) && !g_panelFaultLogged;
    if (fault) g_panelFaultLogged = true;
    LeaveCriticalSection(&g_panelQueueLock);
    if (fault) Logf("[PANEL FUEL] observer disabled after guarded read fault; no game fuel writes or billing");
    if (cap) Logf("[PANEL FUEL] epoch=%ld capture cap=240 reached; restart/load for another bounded capture", epoch);
    if (!have || record.epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    const auto& s = record.snapshot;
    Logf("[PANEL FUEL] epoch=%ld sample=%u ms=%llu phase=post_render window=%p root=%p "
         "status=%s aggregate_valid=%d current_raw=%.9g litres_candidate=%.2f "
         "capacity_unadjusted=%.9g ratio_valid=%d ratio=%.9g links=%u "
         "mode=%d metadata_kind=%d flags_read=%d flag8681=%u flag8682=%u "
         "adjustment_read=%d adjustment_needed=%d presentation=%s dropped=%u "
         "physical_fuel=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)s.window, (void*)s.root, PanelStatus(s.status), s.aggregateValid ? 1 : 0,
         (double)s.current, (double)s.litresCandidate, (double)s.unadjustedCapacity,
         s.ratioValid ? 1 : 0, (double)s.ratio, s.links, s.contextMode, s.metadataKind,
         s.flagsRead ? 1 : 0, (unsigned)s.flag8681, (unsigned)s.flag8682,
         s.adjustmentRead ? 1 : 0, s.adjustmentNeeded ? 1 : 0,
         s.flagsRead && (s.flag8681 || s.flag8682) ? "alternate_if_shown" : "unresolved_7c58",
         dropped);
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& o = s.objects[i];
        Logf("[PANEL OBJECT] epoch=%ld sample=%u index=%u role=%s object=%p metadata=%p "
             "quantity_read=%d raw5f0=%.9g capacity8684=%.9g candidate_home=%p "
             "membership=%s fleet_index=%u occurrences=%u private_status=not_classified",
             record.epoch, record.sequence, i, i ? "linked" : "root", (void*)o.pointer,
             (void*)o.metadata, o.quantityRead ? 1 : 0, (double)o.raw, (double)o.capacity,
             (void*)o.homeCandidate, PanelMatch(o.match), o.fleetIndex, o.occurrences);
    }
}
static bool PanelTargetMatches(void)
{
    // Fingerprint, not a recomputed SHA-256. Operator must verify the file hash.
    if (!g_exeBase || g_exeSize < 0x90B080 || !ReadablePtr(g_exeBase, sizeof(IMAGE_DOS_HEADER))) return false;
    const auto* dos = (const IMAGE_DOS_HEADER*)g_exeBase;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 || dos->e_lfanew > 0x100000) return false;
    const auto* nt = (const IMAGE_NT_HEADERS64*)(g_exeBase + dos->e_lfanew);
    if (!ReadablePtr(nt, sizeof(*nt)) || nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        nt->FileHeader.TimeDateStamp != 0x6A3EB6ADu ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;
    static const BYTE scale[] = {0x00, 0x00, 0x7A, 0x44};
    static const BYTE format[] = {0x25,0,0x2E,0,0x32,0,0x66,0,0x6C,0,0,0};
    return ReadablePtr(g_exeBase + 0x90B07C, sizeof(scale)) &&
        ReadablePtr(g_exeBase + 0x8F3E98, sizeof(format)) &&
        memcmp(g_exeBase + 0x90B07C, scale, sizeof(scale)) == 0 &&
        memcmp(g_exeBase + 0x8F3E98, format, sizeof(format)) == 0;
}
static void PanelObserverStart(void)
{
    if (!g_panelObserverEnabled) { Logf("[PANEL FUEL] disabled by INI"); return; }
    if (g_billing || !g_simClock || !g_simClock->registerTick || !g_persistenceHandle) {
        Logf("[PANEL FUEL] unavailable: requires billing=0, sim_clock tick and registered load signal"); return;
    }
    if (!PanelTargetMatches()) { Logf("[PANEL FUEL] target/data fingerprint mismatch; no hook installed"); return; }
    // 20 complete bytes through LEA RBP,[RSP-2EB0]; no RIP-relative operand.
    static const BYTE expect[] = {0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,
        0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    if (!ReadablePtr(g_exeBase + 0x7D5460, sizeof(expect)) ||
        memcmp(g_exeBase + 0x7D5460, expect, sizeof(expect))) {
        Logf("[PANEL FUEL] renderer prologue mismatch; no hook installed"); return;
    }
    InitializeCriticalSection(&g_panelQueueLock);
    if (!g_simClock->registerTick(PanelObserverTick, nullptr)) {
        Logf("[PANEL FUEL] tick registration failed; no hook installed"); return;
    }
    if (!InstallInlineHook(g_exeBase + 0x7D5460, (void*)PanelRendererDetour,
                          &g_panelTrampoline, expect, sizeof(expect), "private_logistics panel fuel observer")) {
        Logf("[PANEL FUEL] renderer hook refused; capture unavailable"); return;
    }
    InterlockedExchange(&g_panelStarted, 1);
    Logf("[PANEL FUEL] installed renderer=0x7D5460 stolen=20 scale=1000 format=%%.2fl "
         "interval_ms=1000 cap=240 queue=16; logs drain on unpaused ticks; "
         "candidate_home fleet comparison only; no exact burn, tracker or billing");
}
#endif
```

#### tests/private_logistics/panel_fuel_observer_test.cpp

SHA-256: `7730a4b3523580f3ae37990a4503286d8662bbbbab0795eedd7b4cffb1a74869`

```cpp
#include "panel_fuel_observer.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <vector>
#include <string>
#include <utility>
using namespace private_logistics_panel;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
struct Region { Address address; std::vector<unsigned char> data; };
struct Fixture {
    std::vector<Region> regions;
    Address window=0x10000, context=0x20000, root=0x30000, meta=0x40000;
    Address home=0x50000, descriptor=0x60000, fleet=0x70000, links=0x80000;
    std::size_t reads = 0;
    std::vector<std::pair<Address,std::size_t>> denied;
    void Add(Address a, std::size_t n) { regions.push_back({a,std::vector<unsigned char>(n)}); }
    template<class T> void Set(Address a, Address off, T v) {
        for(auto& r:regions) if(a+off>=r.address && a+off+sizeof(T)<=r.address+r.data.size()) {
            std::memcpy(r.data.data()+a+off-r.address,&v,sizeof(T));return;
        }
        CHECK(false);
    }
    static bool Reader(Address a, void* out, std::size_t n, void* user) {
        auto& f=*static_cast<Fixture*>(user); ++f.reads;
        for(auto d:f.denied) if(a<d.first+d.second && d.first<a+n) return false;
        for(auto& r:f.regions) if(a>=r.address && a+n>=a && a+n<=r.address+r.data.size()) {
            std::memcpy(out,r.data.data()+a-r.address,n); return true;
        }
        return false;
    }
    Memory Mem() { return {Reader,this}; }
    Fixture() {
        Add(window,0x500);Add(context,0x600);Add(root,0x2000);Add(meta,0x9000);
        Add(home,0x1000);Add(descriptor,0x400);Add(fleet,MaxFleet*8);Add(links,0x100);
        Set(window,0x20,std::uint8_t(1)); Set(window,0x248,1);Set(window,0x24C,2);
        Set(window,0x240,root);Set(context,0x5B0,2);
        Set(root,0x1708,meta);Set(root,0x5F0,0.54954f);Set(meta,0x8684,0.7f);
        Set(root,0x4F8,home);Set(home,0x318,descriptor);Set(descriptor,0x360,43);
        Set(home,0xC70,fleet);Set(home,0xC78,fleet+8);Set(fleet,0,root);
    }
    Snapshot CaptureNow() { Snapshot s; CHECK(Capture(Mem(),context,window,s));return s; }
    void Link(float raw, float capacity) {
        Address child=0x90000, childMeta=0xA0000;
        Add(child,0x2000);Add(childMeta,0x9000);
        Set(root,0x3A0,links);Set(root,0x3A8,links+8);Set(links,0,child);
        Set(child,0x1708,childMeta);Set(child,0x5F0,raw);Set(childMeta,0x8684,capacity);
        Set(child,0x4F8,home);Set(fleet,8,child);Set(home,0xC78,fleet+16);
    }
};
#define TEST(name) static void name()
TEST(literal_litres_and_identity) {
    Fixture f;auto s=f.CaptureNow();CHECK(s.status==Status::Complete && s.aggregateValid);
    CHECK(s.current==0.54954f && s.litresCandidate==0.54954f*1000.0f);
    char text[32];std::snprintf(text,sizeof(text),"%.2fl",double(s.litresCandidate));CHECK(std::string(text)=="549.54l");
    CHECK(s.count==1 && s.objects[0].match==Match::Found && s.objects[0].fleetIndex==0);
    CHECK(s.objects[0].occurrences==1 && s.ratioValid);
}
TEST(root_plus_child) {
    Fixture f; f.Link(0.1f,0.2f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.count==2 && s.current==0.54954f+0.1f);
    CHECK(s.objects[1].match==Match::Found && s.objects[1].fleetIndex==1);
}
TEST(nonpositive_capacity_excludes_current) {
    Fixture f;f.Link(500.0f,0.0f);auto s=f.CaptureNow();CHECK(s.current==0.54954f);
    f.Set(f.meta,0x8684,0.0f);s=f.CaptureNow();CHECK(s.current==0 && s.litresCandidate==0 && !s.ratioValid);
}
TEST(negative_raw_is_preserved_not_burn) {
    Fixture f;f.Set(f.root,0x5F0,-0.03f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.current==0 && s.objects[0].raw==-0.03f);
}
TEST(nonfinite_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::quiet_NaN());auto s=f.CaptureNow();
    CHECK(s.status==Status::NonFinite && !s.aggregateValid);
    f.Set(f.root,0x5F0,1.0f);f.Set(f.meta,0x8684,std::numeric_limits<float>::infinity());
    CHECK(f.CaptureNow().status==Status::NonFinite);
}
TEST(arithmetic_overflow_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::max());auto s=f.CaptureNow();
    CHECK(s.status==Status::Overflow && !s.aggregateValid);
}
TEST(optional_capacity_adjustment_is_not_guessed) {
    Fixture f;f.Set(f.root,0x19E0,f.links);f.Set(f.root,0x19E8,f.links+8);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.adjustmentNeeded && !s.ratioValid);
    f.denied.push_back({f.root+0x19E0,16});s=f.CaptureNow();CHECK(s.aggregateValid && !s.adjustmentRead && !s.ratioValid);
}
TEST(window_gates_and_4f8_not_required) {
    Fixture f;Snapshot s;CHECK(Capture(f.Mem(),f.context,f.window,s));
    f.Set(f.window,0x4F8,std::uint8_t(0));CHECK(Capture(f.Mem(),f.context,f.window,s));
    for(Address off:{Address(0x34),Address(0x23C)}) {f.Set(f.window,off,9);CHECK(!Capture(f.Mem(),f.context,f.window,s));f.Set(f.window,off,0);}
    f.Set(f.window,0x248,3);CHECK(!Capture(f.Mem(),f.context,f.window,s));f.Set(f.window,0x248,1);
    f.Set(f.window,1,std::uint8_t(1));CHECK(!Capture(f.Mem(),f.context,f.window,s));
    f.Set(f.window,1,std::uint8_t(0));f.Set(f.window,0x20,std::uint8_t(0));CHECK(!Capture(f.Mem(),f.context,f.window,s));
}
TEST(invalid_links_never_produce_partial_total) {
    Fixture f;f.Set(f.root,0x3A0,f.links);f.Set(f.root,0x3A8,f.links+MaxObjects*8);
    auto s=f.CaptureNow();CHECK(s.status==Status::TooManyLinks && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links+1);s=f.CaptureNow();CHECK(s.status==Status::Unreadable && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links-8);CHECK(!f.CaptureNow().aggregateValid);
}
TEST(unreadable_current_keeps_identity_but_not_total) {
    Fixture f;f.denied.push_back({f.root+0x5F0,4});auto s=f.CaptureNow();
    CHECK(!s.aggregateValid && s.status==Status::Unreadable && !s.objects[0].quantityRead);
    CHECK(s.objects[0].match==Match::Found);
}
TEST(home_pointer_alone_is_not_membership) {
    Fixture f;f.Set(f.fleet,0,Address(0xDEADBEEF));auto s=f.CaptureNow();
    CHECK(s.objects[0].homeCandidate==f.home && s.objects[0].match==Match::Absent);
    f.Set(f.descriptor,0x360,52);CHECK(f.CaptureNow().objects[0].match==Match::NotRoadOffice);
}
TEST(fleet_bounds_and_duplicate_membership) {
    Fixture f;f.Set(f.home,0xC78,f.fleet+(MaxFleet+1)*8);
    CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+1);CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+16);f.Set(f.fleet,8,f.root);auto s=f.CaptureNow();
    CHECK(s.objects[0].match==Match::Found && s.objects[0].occurrences==2);
    f.denied.push_back({f.fleet+8,8});CHECK(f.CaptureNow().objects[0].match==Match::Unavailable);
}
TEST(snapshot_reuse_clears_old_pointer_and_quantity) {
    Fixture f;Snapshot s=f.CaptureNow();CHECK(s.aggregateValid);
    f.Set(f.window,0x240,Address(0));CHECK(Capture(f.Mem(),f.context,f.window,s));
    CHECK(!s.aggregateValid && s.root==0 && s.count==0 && s.objects[0].pointer==0);
}
TEST(address_overflow_does_not_call_reader) {
    Fixture f;float value=0;CHECK(!f.Mem().Get(UINTPTR_MAX-1,8,value));CHECK(f.reads==0);
    CHECK(!f.Mem().Get(0,0,value));CHECK(f.reads==0);
}
TEST(flags_are_evidence_not_literal_mode_proof) {
    Fixture f;f.Set(f.meta,0x8681,std::uint8_t(1));auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.flagsRead && s.flag8681==1 && s.flag8682==0);
    f.denied.push_back({f.meta+0x8682,1});CHECK(!f.CaptureNow().flagsRead);
}
TEST(capture_is_read_only_and_bounded) {
    Fixture f;auto before=f.regions;auto s=f.CaptureNow();CHECK(s.aggregateValid && f.reads<100);
    CHECK(before.size()==f.regions.size());for(std::size_t i=0;i<before.size();++i) CHECK(before[i].data==f.regions[i].data);
}
struct Case {const char* name;void(*run)();};
static const Case cases[]={
#define CASE(name) {#name,name}
CASE(literal_litres_and_identity),CASE(root_plus_child),CASE(nonpositive_capacity_excludes_current),
CASE(negative_raw_is_preserved_not_burn),CASE(nonfinite_is_unavailable),CASE(arithmetic_overflow_is_unavailable),
CASE(optional_capacity_adjustment_is_not_guessed),CASE(window_gates_and_4f8_not_required),
CASE(invalid_links_never_produce_partial_total),CASE(unreadable_current_keeps_identity_but_not_total),
CASE(home_pointer_alone_is_not_membership),CASE(fleet_bounds_and_duplicate_membership),
CASE(snapshot_reuse_clears_old_pointer_and_quantity),CASE(address_overflow_does_not_call_reader),
CASE(flags_are_evidence_not_literal_mode_proof),CASE(capture_is_read_only_and_bounded)
};
int main(int argc,char**argv) {
    unsigned count=0;for(const auto& c:cases)if(argc==1 || std::strcmp(argv[1],c.name)==0) {c.run();std::printf("PASS %s\n",c.name);++count;}
    CHECK(count>0);std::printf("%u native scenarios PASS\n",count);return 0;
}
```

#### tests/private_logistics/panel_fuel_runtime_test.cpp

SHA-256: `c7511ad5f4d6a2d8170ad7f2d96bef62d2a384629f89c95c8d9f18420ce62519`

```cpp
// Native host shim exercises the production queue/control code. NOT Windows ABI,
// SEH, patch execution, real threads, or native game lifecycle validation.
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include <cstdarg>

using BYTE = unsigned char;
using LONG = long; // host-shim log ABI; real Windows LONG is 32-bit
using ULONGLONG = unsigned long long;
struct CRITICAL_SECTION { bool busy=false; };
static bool TryEnterCriticalSection(CRITICAL_SECTION* p) { if(p->busy)return false;p->busy=true;return true; }
static void LeaveCriticalSection(CRITICAL_SECTION* p) { CHECK(p->busy);p->busy=false; }
static void InitializeCriticalSection(CRITICAL_SECTION* p) { p->busy=false; }
static LONG InterlockedCompareExchange(volatile LONG* p,LONG value,LONG expected) { LONG old=*p;if(old==expected)*p=value;return old; }
static LONG InterlockedExchange(volatile LONG* p,LONG value) { LONG old=*p;*p=value;return old; }
static LONG InterlockedIncrement(volatile LONG* p) { return ++*p; }
static ULONGLONG clockMs=1000;
static ULONGLONG GetTickCount64() { return clockMs; }
static std::vector<BYTE> image(0x90B080);
static BYTE* g_exeBase=image.data();
static std::size_t g_exeSize=image.size();
static bool ReadablePtr(const void* p,std::size_t n) {
    const auto a=(uintptr_t)p,b=(uintptr_t)g_exeBase;
    return a>=b && a-b<=image.size() && n<=image.size()-(a-b);
}
constexpr unsigned IMAGE_DOS_SIGNATURE=0x5A4D,IMAGE_NT_SIGNATURE=0x4550;
constexpr unsigned IMAGE_FILE_MACHINE_AMD64=0x8664,IMAGE_NT_OPTIONAL_HDR64_MAGIC=0x20B;
struct IMAGE_DOS_HEADER { unsigned e_magic; int e_lfanew; };
struct IMAGE_NT_HEADERS64 {
    unsigned Signature;
    struct { unsigned Machine, TimeDateStamp; } FileHeader;
    struct { unsigned Magic; } OptionalHeader;
};
static std::vector<std::string> logs;
static void Logf(const char* format,...) {
    char line[2048];va_list args;va_start(args,format);std::vsnprintf(line,sizeof(line),format,args);va_end(args);logs.emplace_back(line);
}
static int g_panelObserverEnabled=1,g_billing=0,g_persistenceHandle=1;
static unsigned installed=0,originalCalls=0;
static void Original(void*,void*) { ++originalCalls; }
static bool InstallInlineHook(void*,void*,void** trampoline,const BYTE*,std::size_t n,const char*) {
    CHECK(n==20);*trampoline=(void*)Original;++installed;return true;
}
using Tick=void(*)(void*,float,float);
static Tick tick=nullptr;
static int RegisterTick(Tick fn,void*) { tick=fn;return 1; }
struct ClockApi { int(*registerTick)(Tick,void*); };
static ClockApi api{RegisterTick};
static ClockApi* g_simClock=&api;
#define __fastcall
#define __try try
#define __except(x) catch(...)
#include "panel_fuel_runtime.h"

int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    PanelObserverStart();CHECK(installed==1 && tick && g_panelStarted);
    std::puts("PASS runtime_install_fingerprint");
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    const auto logged=logs.size();PanelRendererDetour(context,window);
    CHECK(originalCalls==1 && g_panelCount==1 && logs.size()==logged);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    tick(nullptr,1,1);CHECK(g_panelCount==0 && logs.size()==logged+2);
    CHECK(logs[logged].find("litres_candidate=549.54")!=std::string::npos);
    CHECK(logs[logged+1].find("exact_fleet_pointer")!=std::string::npos);
    std::puts("PASS runtime_post_render_throttle_and_deferred_log");
    for(unsigned i=0;i<17;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==16 && g_panelDropped==1);
    g_panelQueueLock.busy=true;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelCount==16);
    g_panelQueueLock.busy=false;
    PanelObserverLoading();const auto oldLogs=logs.size();tick(nullptr,1,1);
    CHECK(g_panelCount==0 && g_panelSamples==0 && logs.size()==oldLogs);
    std::puts("PASS runtime_queue_bound_trylock_and_load_discard");
    clockMs+=1000;PanelCapture(context,nullptr);CHECK(g_panelLastMs==0);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    g_panelSamples=239;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    std::puts("PASS runtime_inactive_does_not_starve_capture_and_cap");
    g_exeBase[0x90B07C]=1;CHECK(!PanelTargetMatches());g_exeBase[0x90B07C]=0;
    g_exeBase[0x8F3E98]=0;CHECK(!PanelTargetMatches());
    std::puts("PASS runtime_changed_data_refused");
    std::puts("5 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}
```

#### tests/private_logistics/test_panel_fuel_observer.py

SHA-256: `334f7d352ba123b146d3fdc9918764b0dd0b1ce442a377e77db153622cba60c9`

```python
"""Production reader with synthetic memory; source guards are NOT a DLL/game test."""
from pathlib import Path
import configparser
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = ROOT / 'tests/private_logistics/panel_fuel_observer_test.cpp'


class PanelFuelObserverTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'panel_test'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, case):
        result = subprocess.run([str(self.exe), case], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + case, result.stdout)

    def test_production_runtime_with_host_shim(self):
        exe = Path(self.directory.name) / 'runtime_test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(HARNESS.with_name('panel_fuel_runtime_test.cpp')),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('5 host-shim scenarios PASS', result.stdout)

    def test_matching_test_defaults_and_no_tracker_wiring(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        self.assertEqual(config.getint('private_logistics', 'panel_fuel_observer'), 1)
        self.assertIn('H->configInt(ini, "private_logistics", "panel_fuel_observer", 1)', cpp)
        self.assertEqual(config.getint('private_logistics', 'billing'), 0)
        self.assertEqual(config.getint('private_logistics', 'dry_run'), 1)
        self.assertEqual(config.getint('private_logistics', 'auto_source_percent'), 10)
        self.assertEqual(config.getint('private_logistics', 'auto_destination_percent'), 30)
        self.assertNotIn('private_logistics_journey::Tracker', cpp)
        self.assertIn('PanelObserverStart();', cpp)
        self.assertIn('if (event == TSM_SAVE_LOADING) PanelObserverLoading();', cpp)

    def test_hook_fingerprint_and_full_instruction_boundary(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        expect = runtime.split('static const BYTE expect[] = {', 1)[1].split('};', 1)[0]
        actual = bytes(int(x, 16) for x in re.findall(r'0x([0-9A-Fa-f]{2})', expect))
        # Pinned first 20 bytes from d00eaae, ending exactly after LEA RBP.
        self.assertEqual(actual, bytes.fromhex('40 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 50 D1 FF FF'))
        self.assertIn('TimeDateStamp != 0x6A3EB6ADu', runtime)
        self.assertIn('0x00, 0x00, 0x7A, 0x44', runtime)
        self.assertIn('memcmp(g_exeBase + 0x8F3E98', runtime)
        self.assertIn('&g_panelTrampoline, expect, sizeof(expect)', runtime)
        self.assertIn('!g_persistenceHandle', runtime)
        self.assertIn('if (g_billing ||', runtime)

    def test_render_path_is_bounded_nonblocking_no_io(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        render = runtime.split('static void PanelCapture(', 1)[1].split('static const char* PanelStatus(', 1)[0]
        for forbidden in ('Logf(', 'EnterCriticalSection(', 'Sleep(', 'malloc(', 'new ', 'std::vector', 'std::string'):
            self.assertNotIn(forbidden, render.replace('TryEnterCriticalSection(', 'TryLock('))
        for required in ('TryEnterCriticalSection(', 'g_panelSamples >= 240', 'now - g_panelLastMs < 1000',
                         'g_panelCount == 16', '__except', 'epoch == InterlockedCompareExchange'):
            self.assertIn(required, render)
        hook = render.split('static void __fastcall PanelRendererDetour(', 1)[1]
        self.assertLess(hook.index('((PanelRenderer)g_panelTrampoline)'), hook.index('PanelCapture(context, window)'))

    def test_reader_has_no_native_calls_or_settlements(self):
        header = (PLUGIN / 'panel_fuel_observer.h').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        for forbidden in ('windows.h', 'currency_api', 'modifyCompanyCapital', 'FuelSlotTons(',
                          'FormatSummary(', 'Consume(', 'WriteProcessMemory'):
            self.assertNotIn(forbidden, header + runtime)
        self.assertIn('physical_fuel=unverified exact_burn=0 billing=off', runtime)
        self.assertIn('unresolved_7c58', runtime)

    def test_windows_min_max_macros(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstddef>\n#include <cstdint>\n#include <cmath>\n'
                        '#define max(a,b) ((a)>(b)?(a):(b))\n#define min(a,b) ((a)<(b)?(a):(b))\n'
                        '#include "panel_fuel_observer.h"\nint main(){return 0;}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)


for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name != 'name':
        def run(self, case=name):
            self.run_case(case)
        setattr(PanelFuelObserverTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
```

#### plugins/private_logistics/private_logistics.cpp

SHA-256: `5ced5d51c0736ccf79b7385b920c422e211ca507ebbacc8dcf8a2d3eb27c340c`

```cpp
// private_logistics - what a private company earns, and what it pays.
//
// ---------------------------------------------------------------------------
// v0.18.6 updates: bounded, read-only panel-object/fleet observer.
// Verified 7D5460 prologue, scale and format fingerprints; one sample/second,
// at most 240 per load epoch. No exact burn/tracker/billing integration.
//
// v0.18-permissions: independent state-site directions; consumer sites never
// supply goods; state default deny applies to existing/manual targets too.
// Missing eligible connections are recreated by the company. No fuel/billing
// policy changes in this version. Native direction semantics need game testing.
//
// Historical v0.17 updates (state-storage source opt-in):
//   - Register a storage-panel checkbox through ui_panels' widget API; each
//     public type-5 storage defaults OFF and its permission is persisted by a
//     stable type/name/position key, never by retaining a Building pointer.
//   - When opted in, append a native DO target with load enabled and unload
//     enable/percentage cleared. Turning the checkbox off disables both task
//     directions on that state-storage target; unrelated DO targets are untouched.
//   - The storage builder is hooked by the verified dispatcher call site in
//     ui_panels. Customs remains diagnostic-only; no inventory is manipulated.
// ---------------------------------------------------------------------------
// v0.16 updates (coordinate probe correction):
//   - v0.15 logged a state storage position of (0,0) and customs distance 0.0m.
//     The position helper had read bld+0x08/+0x10 instead of the C3D_NODE
//     translation at bld+0x320+0x30/+0x38 (with placement-bbox fallback).
//     Stable identity keys and nearest-customs diagnostics now use the actual
//     position helper. No customs/state-storage target writes were added.
//
// v0.15 updates (state-storage/customs discovery probe; no public-target writes):
//   - Walk the game-wide building vector to count state storage buildings and
//     report each private road/rail DO's geometrically nearest customs house.
//     This is diagnostic only; neither is appended as a target in this version.
//   - Use game_world.h's canonical demolished flag rather than an unverified
//     raw offset in private-building discovery.
//
// v0.14 updates (automatic private DO targets; staged and disabled by default):
//   1. Once per in-game day, discover registered private DOs (road 43 / rail 52)
//      and private buildings with native storage, using enterprise_base.
//   2. Preserve existing native target entries. Missing eligible targets are
//      reported in dry-run mode by default; optional write mode appends a
//      TaskEntry and mirrors the native add-path TaskCfg defaults (wildcard,
//      100% thresholds, active/validated flags). The owner reports separate
//      load/unload checkboxes in the DO panel; their backing fields and dataflow
//      still need verification before assigning state storage as load-only.
//   3. Generated entries are enabled, deduplicated, and trigger the verified DO
//      dirty flag. Existing entries/configs are never rewritten or removed.
//   4. A session-local identity ledger tracks generated targets by stable
//      keys. Missing eligible targets are recreated; manual removal is not a veto.
//   5. This is a daily callback, not a per-frame hook. Save/load and manual-removal
//      UI behavior remain runtime validation items; write mode ships off.
// ---------------------------------------------------------------------------
// Level 3. Consumes enterprise_base (v8), currency (v4) and sim_clock.
// Optionally consumes resource_pricing for the unit price.
//
// ---------------------------------------------------------------------------
// v0.10 - THE TANK WAS FOUND, AND THE TARIFF WAS FOUR TIMES TOO CHEAP.
//
// veh+0x5F0 is fuel in tonnes. v0.9's field scanner found it by watching which
// words move, and the v0.9 run confirmed it five ways: it falls while the truck
// works, it runs to zero and jumps at a refill, it tops out at 0.46 (the
// player's own 456 LITRE tank at 1 kg/L), the refills reconcile to the diesel
// the company paid for, and nothing else in the vehicle moves that way.
//
// With the tank readable, the burn stops being a guess:
//
//     2.05 t over 51 days, two active vehicles, 139 deliveries
//     = 0.01475 t per delivery = 14.75 L at 1 kg/L
//
// v0.9 shipped 3.30 L/trip, derived from PURCHASES over trips. The company was
// being paid 1.06 per delivery against a real fuel cost of 1.96 - it lost money
// on every trip and the tariff, not the economy, was doing it.
//
// v0.10 reads every vehicle's tank at every close and reports
//
//     [TANK] day N   v=... fuel 0.4300 t | burned 0.0220 t | refills 0
//     [TANK] run: burned 2.0500 t = 2050.0 L over 139 delivery(ies) = 14.75 L/trip
//
// Refills are counted, never subtracted from burn. This is the first figure in
// the whole feature that is neither a purchase nor a model.
//
// ALSO: diesel_density 0.00084 -> 0.00100. The game stores 1 kg per litre; the
// old constant was real-world diesel and made every litre 16 % too cheap.
//
// ---------------------------------------------------------------------------
// v0.9 - FIND THE TANK.
//
// The player asked the right question: how do we know how much fuel a trip
// burnt? We do not. Two runs measured it by dividing the company's diesel
// PURCHASES by its trips and got 3.27 L/trip over 1,100 trips and 7.14 L/trip
// over 214 trips - a factor of two, because fuel sits in tanks between
// purchases. Purchases are not burn, and no amount of averaging makes them so.
//
// What is needed is the fuel going INTO a vehicle, or the level inside it. The
// vehicle record is reachable (bld+0xC70, proven) and the transfer hook never
// shows a fuel movement, so the tank is a FIELD inside the vehicle, not an
// event we can already see.
//
// This build finds it empirically instead of guessing:
//   - the first sighting of a vehicle is stored as a baseline and not logged;
//   - every day after, the same 512 dwords at veh+0x000..0x800 are compared,
//     and ONLY the words that changed are logged, as offset, old, new.
//
// A tank shows up as a word that falls while the truck drives and jumps when
// it refuels. Position, speed and timers will show up too - the offsets tell
// them apart. Nothing is priced from this; it is how the next tariff stops
// being a fleet average.
//
// ALSO IN v0.9:
//   - the [FUEL] run line now says PURCHASES and not burn, because that is
//     what it measures.
//   - the ini learned its section header back. v0.7's rewrite dropped
//     [private_logistics], so the host read nothing and every key fell back to
//     its compiled default - which is why the last two runs were dry runs.
//
// ---------------------------------------------------------------------------
// v0.8 - SEE THE VEHICLES BEFORE PRICING THEM.
//
// v0.7's trip tariff charged every delivery the same litres. That is one
// arbitrary vehicle imposed on a whole fleet, and it cannot survive: vehicles
// differ in capacity, speed, price and consumption, and a tariff that ignores
// it will overpay a scooter and bankrupt a heavy hauler. H41.
//
// The v0.7 run shows the transfer hook cannot supply the difference: 3,065
// site lines resolve to exactly TWO vehicle-side pointers, one of them NULL.
// The fleet has to be read where the game keeps it - bld+0xC70/+0xC78, proven
// by three independent readers (Part 12) - and each vehicle's own record read
// from there:
//
//     veh+0x4F8  Building*  home building                      proven
//     veh+0x3B8  vec        cargo, stride 0x10 (Res*, float t)  proven
//     veh+0x510  ?          candidate catalogue entry           UNPROVEN
//
// This build adds the census: once a day, every private enterprise's fleet is
// walked, each vehicle is logged with what it carries and what it can carry at
// peak, and the first sighting of each vehicle dumps 32 bytes at the pointer
// the game gives for its catalogue entry. That is the lead for capacity,
// speed, price and consumption - the four numbers the final model needs and
// the four this build refuses to invent.
//
// The tariff is unchanged from v0.7. It stays a fallback until the census
// says what the fleet actually is.
//
// ALSO IN v0.8:
//   - litres_per_trip 5.00 -> 3.30. The v0.7 meter measured the DO's diesel
//     bill against its own deliveries: 406.40 RUB over 1,100 trips =
//     3.267 L/trip at ~128/RUB. Not a guess, and not 8.63 either - that
//     figure counted tank fill, exactly as the player said it would.
//   - the [FUEL] run line no longer claims "model and bill agree" when the
//     bill is zero and there is nothing to compare.
//
// ---------------------------------------------------------------------------
// v0.6 - THE TRIP IS PAID FOR BY THE TRIP, AND THE SHARE IS ONLY A SHARE.
//
// Session 7 ran v0.5 with fuel switched on and produced the number the whole
// feature needed: the private DO's account reconciles exactly to
//
//     10,000.00 + 738.99 freight - 445.37 fuel - 0.02 = 10,293.60
//
// so the game DOES charge a private distribution office for diesel, and it
// arrives as a cat=2 (FOREIGN) debit on its own company capital. H37 closed.
// H33 closed too: 444 sampled payments, all moving the treasury to the kopeck.
//
// The same log indicted the payment model. Per delivery, against a measured
// fuel cost of 0.34 RUB:
//
//     plants     282 deliveries   0.29 each   0.9x the fuel bill
//     food       127 deliveries   2.13        6.3x
//     meat        16 deliveries   2.37        7.0x
//     fabric      19 deliveries   3.80       11.2x
//     chemicals   45 deliveries   6.13       18.0x
//
// 58 % of the DO's work was selling at a LOSS and 9 % was paying for it. A
// percentage of cargo value is not a tariff, it is a lottery. Real freight is
// cost recovery plus a margin; only the handling of valuable cargo is priced
// ad valorem, and it is a small part of the bill.
//
// v0.6 therefore models the tariff as what it has to be:
//
//     tariff = fuel_cost x (1 + margin) + tariff_share x cargo_value
//
// Cost recovery pays the trip, the share pays for the cargo being worth
// carrying. litre counts come from session 7 (see litres_per_trip in the ini).
//
// Still probe only. Nothing here is charged. The 8 % share keeps running so
// the two can be read side by side for one more session.
//
// Also fixed: the index and the treasury were read at plugin start, before the
// world is loaded, so every log opened with "fuel NOT FOUND" and "treasury
// opens at 0.00" that were both false a second later. Both are now re-read and
// printed at every day close.
//
// ---------------------------------------------------------------------------
// v0.5 - MEASURE THE COST BEFORE PRICING IT.
//
// v0.4 moved money and the money arrived (RE_DIGEST Part 19). What the same
// session also showed is that the private DO has NO COST SIDE: its company
// account took 646 credits over seven days and paid out nothing at all. Fuel,
// upkeep and vehicles never touched its books. So the 8 % freight is not a
// tariff, it is a subsidy, and "is 8 % enough to cover fuel" cannot be asked
// until a cost exists to compare it against.
//
// This build therefore charges nothing new and measures two things:
//
//   1. THE TARIFF MODEL. A price built the way real freight is built - cost
//      recovery plus a margin - and indexed to a live price, never to a
//      rouble constant. A constant is exactly what inflation eats:
//
//          litres  = litres_per_trip + litres_per_tonne x tonnes
//          cost    = litres x diesel_density x fuel_price      (live)
//          tariff  = cost x (1 + margin)
//
//      If no fuel price resolves (fuel_resource unknown, resource_pricing
//      absent), the same cost is expressed in daily survival baskets via the
//      CPI tail, so the fallback is indexed too. If neither resolves the
//      probe reports "unindexed" and prices nothing - it never guesses a
//      figure in currency units.
//
//   2. THE MONEY PATH, both sides of it. [BAL] prints the treasury and every
//      carrier's company capital at each day close. v0.4 proved the company
//      side to the kopeck but never read the payer (H33); the delta between
//      two closes is that proof, and the difference between a carrier's
//      capital change and the freight it was paid is its fuel bill (H37).
//
// Nothing here charges the probe. `tariff_probe = 1` logs, it does not pay.
// The 8 % share keeps running exactly as v0.4 left it, so a session produces
// three numbers side by side: modelled tariff, freight actually paid, and the
// vehicle's real fuel consumption as the game charged it.
//
// v0.4 also carried a stale banner - it opened every log with "v0.3" - which
// cost the last reader a minute. Version now comes from PL_VERSION once.
//
// ---------------------------------------------------------------------------
// v0.4 - FREIGHT IS A SHARE OF CARGO VALUE, AND THE MONEY MOVES.
//
// v0.3 paid a flat per-tonne rate (1.50 RUB/t). Measured against the live
// session that is 0.31 % of what the cargo is worth - a private distribution
// office could not cover a truck, and FreightRateFor() ignored the resource
// anyway, so the liquid and goods rates were dead keys.
//
// v0.4 replaces the rate with freight_share, a fraction of the cargo's own
// value:
//
//     freight = tons x unitPrice x freight_share          (default 0.08)
//
// A typical load is ~0.12 t of fabric worth ~58 RUB, so this pays ~4.70 RUB
// instead of ~0.18. It scales with what is actually moved, it needs no
// commodity-class table, and when Ship 2 makes the BUYER pay instead of the
// state, the same formula becomes the wholesaler's margin with no rewrite.
//
// v0.4 also:
//   - refuses a payment the treasury cannot afford (getTreasuryBalance),
//     because moveTreasury takes a signed amount and will happily go negative;
//   - splits `skipped` into the four things it was conflating;
//   - labels the daily line with the day it actually covers (it was one
//     behind - the callback fires at the START of the day).
//
// ---------------------------------------------------------------------------
// v0.3 - THE FIRST MONEY.
//
// `dry_run = 1` by default: this build COMPUTES every payment and logs it as
// "[PAY] would", but moves nothing. Set dry_run = 0 to actually pay.
// Nothing else in the plugin changes when you flip it.
//
// The money model (PLAN_TECHNICAL.md sections 6 and 7):
//
//   H1 - A BUILDING IS PAID FOR GOODS THAT LEAVE IT
//        at a mode-0 call with tons < 0, param_2 is the source Building.
//        If that building is a private company, it is credited
//        |tons| x price x factory_share, and the treasury pays.
//
//   H2 - A CARRIER IS PAID FREIGHT FOR A DELIVERY
//        at a mode-0 call with tons > 0, param_2 is the destination Building
//        and param_9 is the fleet owner. If the owner is a private company
//        and is not the destination, it is credited
//        tons x price x freight_share.
//
//   Nothing is billed at a mode-1 site: those are the vehicle side of the
//   same movement and would double-charge it.
//
// ---------------------------------------------------------------------------
// What the two live sessions established (RE_DIGEST Parts 10, 12 and 13)
// ---------------------------------------------------------------------------
//
//   - THE COMMIT TEST IS `(unsigned char)r == 1`. The transfer returns a bool
//     in AL; the upper 24 bits of EAX are register garbage. v0.1 tested the
//     whole int and discarded exactly half the committed calls.
//
//   - `param_2` IS A Building* AT MODE 0 AND A Vehicle* AT MODE 1. That is
//     the whole of attribution. At mode 0 the value also appears in
//     enterprise_base's "[CAP] ... by=wholesale" lines, which only ever take
//     a building; vehicle offsets read off it return garbage.
//
//   - `param_9` IS THE FLEET OWNER, not an endpoint: constant across two
//     different destination buildings, and equal to the mode-1 vehicle's
//     `home` field in every call.
//
//   - `(veh+0x4F0)` IS NOT AN ENDPOINT. It was NULL in all 342 calls v0.1
//     measured and all 373 of v0.2's, because at mode 0 we were reading
//     vehicle fields off a building. Nothing here uses it.
//
//   - A MOVEMENT IS TWO CALLS 30-70 ms APART, same resource, same tonnage:
//       0x694F40  mode 0  -tons   the BUILDING the goods leave
//       0x6953C4  mode 1  +tons   the VEHICLE they enter
//       0x68F792  mode 0  +tons   the BUILDING they enter
//       0x68F7F9  mode 1  -tons   the VEHICLE they leave
//     The load leg's mode-0 call has param_9 == NULL, so its carrier is
//     resolved from the paired mode-1 call through a short time ring.
//
// ---------------------------------------------------------------------------
// The hook is a call-site patch, NOT an inline hook
// ---------------------------------------------------------------------------
//
// FUN_140420350 has a five-byte prologue, so the loader's installInlineHook
// (which requires >= 14 position-independent bytes) cannot take it. All
// sixteen invocations are direct `call rel32` with no vtable, thunk or
// indirect path, so retargeting the call sites sees every invocation.
//
//   site before:  E8 <rel32>          call FUN_140420350
//   site after:   E8 <rel32>          call cave          <- SAME opcode
//   cave:         FF 25 00 00 00 00   jmp qword ptr [rip+0]
//                 <8-byte detour address>
//
// The opcode deliberately stays E8. A jmp does not push a return address, so
// a detour entered that way both reads garbage from _ReturnAddress() and
// executes `ret` into the enclosing function's caller. Verified on x86-64.
//
// Safety properties:
//   - Nothing is written unless the site verifies: byte 0 must be E8 AND its
//     rel32 must resolve to exeBase+0x420350.
//   - Every game-memory read is behind ReadablePtr.
//   - The ledger is a fixed ring; nothing allocates in the detour.
//   - The lock covers counters and the rings only. Game-memory reads, calls
//     into other plugins and logging all happen outside it.
//   - A payment needs BOTH services. If enterprise_base or currency is
//     missing, billing is skipped entirely and says so once.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "site_permissions.h"
#include "task_audit.h"
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"

#include <stdint.h>
#include <math.h>
#include <new>

// ---------------------------------------------------------------------------
// the transfer being instrumented
// ---------------------------------------------------------------------------

#define TSM_RVA_TRANSFER 0x420350

typedef int (*tTransfer)(void* game, void* veh, unsigned mode, void* res,
                         float tons, float* outMoved, float cap,
                         unsigned char flag, void* bld);

static tTransfer g_realTransfer = nullptr;

// Vehicle fields. See PLAN_TECHNICAL.md section 3.1.
#define TSM_VEH_HOME    0x4F8   // Building* - the owning depot (mode 1 only)

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static int   g_enabled         = 1;
static int   g_billing         = 0; // Permissions-test default: no freight payments.
static int   g_dryRun          = 1;      // SAFE DEFAULT: compute, log, do not pay
static int   g_payFactory      = 1;
static int   g_payCarrier      = 1;
static int   g_logCapPerDay    = 400;
static int   g_ledgerCapacity  = 512;
static int   g_siteLog         = 1;
// v0.4: ONE number, a share of the cargo's value. The three per-tonne rates
// are gone: the class table never existed (FreightRateFor ignored its
// argument) and a player cannot reason about "RUB per tonne of coal".
static float g_freightShareRetired = 0.0f; // set when the ini still carries it
static float g_freightShare    = 0.08f;  // v0.7: RETIRED. Kept only so the
                                         // ini key can be recognised and warned about.
static float g_factoryShare    = 0.90f;  // w - what a buyer pays a factory

// v0.7: THE TRIP TARIFF REPLACES THE VALUE SHARE.
//
// v0.6 measured the value share against the DO's real fuel bill and lost:
// 14,038.20 RUB of freight against 511.92 of diesel over 82 days. The share
// is 27x the cost it was supposed to cover, and it is value-blind - a truck
// hauling plants was paid 1.46 for the trip, one hauling chemicals 71.12.
//
// v0.7 charges the model that was only probed in v0.6; v0.6's behaviour is
// one key away (`trip_tariff = 0`), so both can be compared in one session.
static int    g_tripTariff      = 1;
static double gWearLitres       = 5.0;   // maintenance reserve, in litres of diesel
static int    g_litresAuto      = 0;     // re-tune litres_per_trip from the fuel bill

// v0.7: the measured fuel bill. This is the only figure in the plugin that
// comes from the game instead of from arithmetic - the [TARIFF] model is an
// assumption, the DO's monthly foreign-cost total is the bill.
//
// Fuel is bought in bulk and stored, so a day's purchases are NOT a day's
// burn. Over a run the tank delta washes out; the average is the burn.
// v0.8: the vehicle census. A republic has tens of vehicles, not thousands;
// 64 slots is four times the largest fleet seen so far and keeps the table in
// static storage, as the render/tick rules require.
#define VEH_MAX 64
static void*    g_vehPtr[VEH_MAX];
static double   g_vehPeakTons[VEH_MAX];
static unsigned g_vehSamples[VEH_MAX];
static int      g_vehCount   = 0;
static int      g_censusLog  = 1;
// v0.9: the field scanner. 512 dwords (0x800 bytes) per vehicle, compared once
// a day; only changes are logged. Static storage, no allocation.
#define VEH_SCAN_WORDS 512
static unsigned g_vehSnap[VEH_MAX][VEH_SCAN_WORDS];
static bool     g_vehSnapValid[VEH_MAX];
// v0.10: THE TANK. veh+0x5F0 is fuel, in TONNES, and the v0.9 field scanner
// found it the honest way - by watching which words move.
//
// The evidence, from the v0.9 run (51 days, two active vehicles):
//   - the word falls every day the truck works: 0.4527 -> 0.4307 -> 0.4073 ...
//   - it runs to ~0 and then JUMPS: 0.0024 -> 0.4527  (+0.4503, a refill)
//   - three refills per vehicle, each +0.26 to +0.48, and the tanks top out at
//     ~0.46 - which is the player's own reported 456 LITRE tank at 1 kg/L.
//   - the refills over the run (2.66 t) match the diesel the company PAID for
//     (389.23 RUB = 2.99 t at 133/RUB), less what was left in the tanks.
//
// That is five independent agreements, so the field is fuel and the unit is
// tonnes. Nothing else in the vehicle moved like that.
static double   g_vehFuel[VEH_MAX];          // last reading, tonnes
static bool     g_vehFuelValid[VEH_MAX];
static double   g_burnTonsRun   = 0.0;       // burned this run, tonnes
static unsigned g_refillsRun    = 0;
// v0.11: the denominator the physics actually has. Burn is per TICK, so the
// honest rate is litres per fleet-day, not litres per delivery - and it scales
// with HOW MANY TRUCKS and HOW LONG, never with how much they carry.
static unsigned g_tankDaysRun    = 0;        // closes with at least one tank read
static unsigned g_tankVehDaysRun = 0;        // (vehicle, close) pairs
static int      g_cargoDump      = 1;        // [CARGO] - settle where fuel lives
static int      g_tankCrossLog   = 40;       // v0.13: [TANK] slot-vs-mirror lines
static unsigned char g_vehNoSlotWarned[VEH_MAX];
static unsigned char g_vehCapValid[VEH_MAX];
static double        g_vehCap[VEH_MAX];      // per-vehicle tank capacity, tonnes

// v0.13: THE ACTIVITY GATE, straight out of the follow-up report.
//
// The engine skips a vehicle entirely unless veh+0x448 is 1 or 2 (and +0x440
// is 9): no burn, no refuel, nothing. That means the honest denominator for a
// fuel rate is not "fleet-day" but "TRUCK-DAY ACTIVE", and it means the ~40 L
// this fleet burned on days with no private deliveries was burned doing work
// that was not ours.
//
static unsigned g_activeVehDaysRun = 0;
static unsigned g_idleVehDaysRun   = 0;

// v0.11: THE PER-TRIP FUEL PROBE.
//
// The player's question was exact: "You were able to see the fuel used for
// every trip?" No. Every number this plugin has ever printed about fuel came
// from PURCHASES divided by trips, and purchases are a stock movement, not a
// burn. With 4,558 L bought and 4,157 L pumped into tanks in the v0.7 run, the
// ratio could have been anywhere between 7.0 and 8.7 L/trip.
//
// The fix costs one pointer walk: at every committed delivery, total the tanks
// of the fleet that did it. The difference from the previous delivery is the
// fuel burned between the two - no purchases, no storage, no assumptions. A
// refill (tank goes UP) or a change in fleet size invalidates that one
// interval, and those are counted separately rather than averaged in.
static const int VEH_FUEL_OFF = 0x5F0;     // the tank, in tonnes (see ReadTank)

static void*    g_probeBld      = nullptr;
static double   g_probeFuelTons = -1.0;
static int      g_probeVehCount = -1;
static double   g_probeBurnRun  = 0.0;
static unsigned g_probeStepsRun = 0;
static unsigned g_probeSkipRun  = 0;
static double   FleetFuelTons(void* b, int* outCount);
static void     BurnProbe(void* b);
static int      g_vehScan          = 1;
static int      g_vehScanLogPerDay = 40;
static void     CensusFleets(int closedDay);   // v0.8, defined with the census
static void     ScanVehicle(int slot, void* v, int closedDay);   // v0.9
static void     ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills);   // v0.10

static double             g_fuelPaidRun  = 0.0;
static double             g_fuelLitresRun = 0.0;
static unsigned long long g_fuelTripsRun = 0;

// v0.11 updates:
//   1. litres_per_tonne ships at 0. Load does not enter the burn formula -
//      Ghidra, FUN_14016E3B0 - and the v0.10 log agrees (r = -0.03).
//   2. [TANK] run prints LITRES PER FLEET-DAY and per truck-day beside L/trip.
//      L/trip is a ratio of two unrelated things; L/day is the physics.
//   3. [CARGO] dumps the raw cargo vector, uncapped, to settle where fuel
//      actually lives. The Ghidra report says veh+0x3B8; our own census says
//      the trucks carry 0 slots while holding 0.29 t in the tank. One of those
//      is wrong and a dump costs nothing.
//   4. A NEGATIVE tank reading is read as empty, not as extra burn. Day 13 of
//      the v0.10 run read -0.0302 t; +0x5F0 is derived, not the level.
//   5. BURN PROBE: the fleet's tanks are totalled at every committed delivery,
//      so the next run reports diesel that actually left the tanks BETWEEN two
//      deliveries. That is the player's question answered with a measurement
//      instead of purchases divided by trips.
//
// v0.12 updates:
//   1. The cargo census no longer hides vectors longer than eight slots. That
//      cap is what made this plugin report "0 slot(s)" on trucks holding fuel,
//      and I used that as evidence. It was not evidence - it was a bug here.
//   2. [CARGO] dumps BOTH storage vectors, 0x3A0 and 0x3B8, every slot.
//   3. [VEH] now prints kind=, the int at +0x50C that the report calls the
//      fuel-consumer flag. +0x510 was never a pointer; that is why it printed
//      nonsense.
//
// v0.13 updates (follow-up disassembly):
//   1. THE TANK IS THE CARGO SLOT, not +0x5F0. The slot at veh+0x3B8 whose
//      resource pointer is diesel's, +0x08 tonnes, clamped at zero.
//   2. +0x5F0 is an UNCLAMPED MIRROR written by the refuel and drive paths.
//      That is why it went negative; it is now a cross-check, not the source.
//   3. The fleet's tank can be read per vehicle in two places - the slot for
//      the level, and *(veh+0x1708)+0x8684 for that vehicle's CAPACITY. The
//      capacity is logged once per vehicle, so no arbitrary vehicle stands in
//      for the fleet.
//   4. [VEH] prints state (veh+0x448) and mode (veh+0x440). The engine skips a
//      vehicle entirely unless the state is 1 or 2, so "litres per ACTIVE
//      truck-day" is the only fuel rate that means anything; it is now on the
//      [TANK] run line.
//
// v0.5: the cost side, in physical units. No rouble constant may appear here:
// prices move, litres and ratios do not.
static int    g_tariffProbe     = 1;       // compute + log the tariff, never charge
static int    g_balanceLog      = 1;       // [BAL] treasury + carrier capital per day
static int    g_tariffLogPerDay = 60;      // per-delivery [TARIFF] lines, then totals only
static double gLitresPerTrip    = 14.75;   // v0.10: measured from the TANKS, not purchases
// v0.11: ZERO - and this retires a term I invented.
//
// Ghidra on FUN_14016E3B0 (the per-tick upkeep function) shows the burn is
//
//   consumption = (slot_rate / *(float*)(bld+0xE30)) * base_rate
//                 * (tick_delta / DAT_14090AA78) * fVar24
//   fVar24      = DAT_140909F58 / ((bld+0xE68 - bld+0xE60) / 0xC)
//
// fVar24 is a BUILDING-level constant and cargo weight never enters it. A full
// truck burns what an empty one burns. `litres_per_tonne` was my invention and
// it is falsified by the code as well as by the v0.10 log (daily burn vs daily
// deliveries: correlation -0.03 over 51 days).
//
// Kept as a key so the claim can be re-tested, but it ships at zero.
static double gLitresPerTonne   = 0.0;     // FALSIFIED - load does not burn
static double gTariffMargin     = 0.35;    // dimensionless: 35 % over fuel cost
static double gDieselDensity    = 0.00100; // t/L - v0.10: the game uses 1 kg/L
                                            // (a 456 L tank reads 0.456 t), not
                                            // real diesel's 0.84
static char   gFuelResource[64] = "fuel";  // resolved through resource_pricing
static double gBasketsPerTrip   = 0.80;    // CPI fallback, in daily survival baskets
static double gBasketsPerTonne  = 0.02;

#define PL_VERSION "0.18.6-panel-observer"
static int g_panelObserverEnabled = 1;
static void PanelObserverLoading(void);
static float g_minPayment      = 0.01f;
static double g_dailyPayCap    = 0.0;    // 0 = no cap

static const TsmEnterpriseBaseApi*  g_enterprise = nullptr;
static const TsmSimClockApi*        g_simClock   = nullptr;
static const TsmCurrencyApi*        g_currency   = nullptr;
static const TsmResourcePricingApi* g_pricing    = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persistenceHandle = 0;
static const TsmUiPanelsApi*         g_uiPanels = nullptr;
static int                           g_stateStorageWidgetHandle = 0;

// Automatic DO-target generation. Fixed buffers only; discovery runs from the
// once-per-day sim_clock callback, never from a per-frame/render hook.
static int g_autoTargetsEnabled = 1;
static int g_autoTargetsDryRun  = 0;
static int g_autoSourcePercent = 10;
static int g_autoDestinationPercent = 30;
static int g_autoTargetLogLimit = 80;
static volatile LONG g_autoResetActiveMemoAfterLoad = 0;
#define AUTO_ENTITY_MAX 8704
#define AUTO_MEMO_MAX   262144
#define AUTO_MEMO_HASH_SIZE 524288
#define AUTO_POINTER_HASH_SIZE 32768
#define AUTO_OFFICE_HASH_SIZE 16384
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char isStateStorage; unsigned char statePermission; unsigned char targetPresent; };
struct AutoMemo { uint64_t officeKey; uint64_t targetKey; unsigned nextOffice; unsigned char state; };
struct AutoPointerSlot { void* building; unsigned entityIndex; };
struct AutoOfficeMemoHead { uint64_t officeKey; unsigned headPlusOne; };
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static AutoMemo   g_autoMemo[AUTO_MEMO_MAX];
static unsigned   g_autoMemoHash[AUTO_MEMO_HASH_SIZE]; // index + 1; zero means empty
static AutoPointerSlot g_autoPointerHash[AUTO_POINTER_HASH_SIZE];
static AutoOfficeMemoHead g_autoOfficeMemoHeads[AUTO_OFFICE_HASH_SIZE];
static unsigned   g_autoMemoCount = 0;
static unsigned   g_autoLogLines = 0;
static int        g_autoLastDay = -1;
static void AutoTargetsOnDay(int day, int closedDay);
static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void* userData);

// ---------------------------------------------------------------------------
// the sixteen call sites
// ---------------------------------------------------------------------------
//
// `known` records what the RE rounds established. The instrument does not act
// on the label - every site is logged the same way - but billing only ever
// fires at a MODE 0 site, and `known` is what makes the log readable.
//
// After the first two sessions the FUN_14068B330 family is no longer
// described as "sea/rail": it carries ordinary factory traffic.

enum SiteClass {
    SITE_UNKNOWN = 0,
    SITE_REFUEL,
    SITE_LOAD,          // draws goods out of a building
    SITE_DELIVERY,      // puts goods into a building
    SITE_TRANSSHIP      // intra-vehicle / return leg
};

struct SiteSpec {
    unsigned    rva;
    SiteClass   known;
    const char* note;
};

#define MAX_SITES 16

static SiteSpec g_sites[MAX_SITES] = {
    { 0x00141F33, SITE_UNKNOWN,   "never observed" },
    { 0x0016F831, SITE_REFUEL,    "refuel call - telemetry, never billed; never observed" },
    { 0x001B2898, SITE_UNKNOWN,   "CONTESTED: r8-9 'road delivery', r10 'road refuel'; never observed" },
    { 0x001B484B, SITE_UNKNOWN,   "never observed" },
    { 0x001B5EC0, SITE_UNKNOWN,   "never observed" },
    { 0x001B653B, SITE_UNKNOWN,   "never observed" },
    { 0x001B6833, SITE_UNKNOWN,   "never observed" },
    { 0x001B75ED, SITE_UNKNOWN,   "never observed" },
    { 0x001B7981, SITE_UNKNOWN,   "never observed" },
    { 0x001CB7FF, SITE_LOAD,      "loader: mode 0, +tons, then bld+0x970 storage-add; never observed" },
    { 0x0068EECA, SITE_LOAD,      "family A: mode 0, +tons, cargo-add follows; never observed" },
    { 0x0068F792, SITE_DELIVERY,  "family A DELIVERY: mode 0, +tons, param_2 = building, param_9 = fleet owner" },
    { 0x0068F7F9, SITE_TRANSSHIP, "family A delivery, vehicle side: mode 1, -tons. NEVER BILLED" },
    { 0x006945B3, SITE_TRANSSHIP, "family A: mode 0, -tons; never observed" },
    { 0x00694F40, SITE_LOAD,      "family A LOAD: mode 0, -tons, param_2 = the building the goods leave" },
    { 0x006953C4, SITE_TRANSSHIP, "family A load, vehicle side: mode 1, +tons. NEVER BILLED" }
};

static bool g_sitePatched[MAX_SITES] = { false };
static bool g_siteWanted[MAX_SITES]  = { false };   // filled in by ReadSettings
static bool g_siteFilterActive       = false;       // false = every site

static int  g_siteCount = MAX_SITES;

// ---------------------------------------------------------------------------
// the ledger
// ---------------------------------------------------------------------------

enum ShipKind { SHIP_LOAD = 0, SHIP_DELIVERY, SHIP_FREIGHT, SHIP_FUEL, SHIP_PURCHASE };

struct Shipment {
    unsigned id;
    unsigned seq;
    void*    building;
    void*    counterparty;
    void*    vehicle;
    void*    resource;
    float    tons;
    double   unitPrice;
    double   value;
    unsigned short site;
    unsigned char  kind;
    unsigned char  mode;
    double   t;
};

#define LEDGER_MAX 2048
static Shipment g_ledger[LEDGER_MAX];
static int      g_ledgerHead  = 0;
static unsigned g_ledgerSeq   = 0;
static unsigned g_ledgerTotal = 0;

// ---------------------------------------------------------------------------
// the carrier ring
// ---------------------------------------------------------------------------
//
// The load leg's mode-0 call does not carry the carrier: param_9 is NULL
// there (91 of 91 calls). The vehicle is named 30-70 ms later by the paired
// mode-1 call. This ring is how the two are joined.
//
// A match needs the same resource, the same tonnage within 0.5 %, and an age
// under 2 s. If nothing matches, the payment is skipped and counted - never
// guessed.

#define CARRIER_RING 32

struct CarrierNote {
    void*  res;
    float  tons;      // absolute
    void*  home;      // the fleet owner
    double t;
};

static CarrierNote g_carrier[CARRIER_RING];
static int         g_carrierHead = 0;

static double NowSeconds(void)
{
    // GetTickCount64 is monotonic and needs no CRT.
    return (double)GetTickCount64() / 1000.0;
}

// ---------------------------------------------------------------------------
// counters
// ---------------------------------------------------------------------------

static unsigned g_calls            = 0;
static unsigned g_callsCommitted   = 0;
static unsigned g_callsNoop        = 0;
static unsigned g_unknownBuilding  = 0;
static unsigned g_staging          = 0;
static unsigned g_nonPrivate       = 0;
static unsigned g_siteLoggedToday  = 0;
static unsigned g_attrLoggedToday  = 0;
static unsigned g_payLoggedToday   = 0;
static unsigned g_siteSuppressed   = 0;
static unsigned g_attrSuppressed   = 0;
static unsigned g_paySuppressed    = 0;

static unsigned g_payCount         = 0;
// v0.4: `skipped` used to be one number covering four unrelated causes. In
// the fourth session it read 159 in a day purely because the distribution
// office was state-owned - alarming to read, and meaningless.
static unsigned g_skipNoPayee      = 0;   // no private owner on the leg
static unsigned g_skipNoPrice      = 0;   // resource_pricing gave no price
static unsigned g_skipSmall        = 0;   // below min_payment
static unsigned g_skipTreasury     = 0;   // the state could not afford it

// v0.5: the tariff probe's own tallies, and which index it managed to use.
// An unindexed delivery is NOT a delivery worth 0.00 - it is one this build
// could not price, and the daily line says so rather than folding it in.
static unsigned g_tariffCount      = 0;
static unsigned g_tariffNoIndex    = 0;
static unsigned g_tariffFuelIndex  = 0;
static unsigned g_tariffCpiIndex   = 0;
static double   g_tariffCostSum    = 0.0; // the fuel-recovery half
static double   g_tariffShareSum   = 0.0; // the ad valorem half
static unsigned g_tariffLoggedToday= 0;
static double   g_tariffSum        = 0.0;

// v0.5 (H33): the payer leg, sampled per payment. A daily treasury delta is
// useless as proof - wages, taxes and store sales move the same balance all
// day long. Sampling either side of OUR OWN move isolates it completely: the
// difference must be exactly minus the amount, or the books do not close.
// v0.5: the refuel site (0x16F831) has never fired in any session. If the
// game is going to start charging for fuel, this is where we see it first.
static unsigned g_fuelEvents       = 0;
static unsigned g_treasuryChecked  = 0;
static unsigned g_treasuryExact    = 0;
static unsigned g_treasuryOff      = 0;
static double   g_treasuryMaxGap   = 0.0;

// v0.5: the money path, both ends. The treasury is sampled at each close; the
// carriers we paid are remembered so their capital can be printed beside it.
#define BAL_CARRIERS 4
static double g_treasuryAtClose    = 0.0;
static bool   g_treasuryHaveClose  = false;
static void*  g_balCarrier[BAL_CARRIERS]     = { nullptr, nullptr, nullptr, nullptr };
static double g_balCarrierPrev[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 };
static double g_balCarrierIn[BAL_CARRIERS]   = { 0.0, 0.0, 0.0, 0.0 };
static double g_balCarrierFuel[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 }; // v0.7 meter,
                                                                    // -1 = no baseline yet
static int    g_balCarrierCount    = 0;
static void*  g_fuelRes            = nullptr;   // cached lookup, re-resolved daily

// v0.4: which day the accumulated tallies belong to. OnDay fires at the START
// of `day`, so the window it reports is the day that just ENDED.
static int g_summaryDay            = -1;
static unsigned g_payCapped        = 0;
static double   g_paidFactory      = 0.0;
static double   g_paidCarrier      = 0.0;
static double   g_paidToday        = 0.0;

static double   g_wouldPayFreight  = 0.0;
static double   g_wouldPayFuel     = 0.0;

static int      g_lastMonth        = -1;

// Per-site tallies, so "never fired" is distinguishable from "never logged".
struct SiteTally {
    unsigned calls;
    unsigned committed;
    double   tonsIn;      // sum of +tons
    double   tonsOut;     // sum of |−tons|
    double   value;
    unsigned sawBld;      // param_9 non-NULL
    unsigned sawCur;      // *(veh+0x4F0) non-NULL
    unsigned sawHome;     // *(veh+0x4F8) non-NULL
};

static SiteTally g_tally[MAX_SITES];

static int SiteIndex(unsigned rva)
{
    for (int i = 0; i < g_siteCount; i++)
        if (g_sites[i].rva == rva) return i;
    return -1;
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static bool IsPrivate(void* b)
{
    if (!b || !g_enterprise) return false;
    if (!TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return false;
    return g_enterprise->isBuildingPrivate(b) != 0;
}

static const char* KindName(unsigned char k)
{
    switch (k) {
        case SHIP_LOAD:     return "load";
        case SHIP_DELIVERY: return "unload";
        case SHIP_FREIGHT:  return "freight";
        case SHIP_FUEL:     return "fuel";
        case SHIP_PURCHASE: return "buy";
        default:            return "?";
    }
}

static const char* ClassName(SiteClass c)
{
    switch (c) {
        case SITE_REFUEL:    return "refuel";
        case SITE_LOAD:      return "load";
        case SITE_DELIVERY:  return "delivery";
        case SITE_TRANSSHIP: return "transship";
        default:             return "unknown";
    }
}

// The resource record's name lives at its head. A bad pointer yields "?".
static void ResourceName(void* res, char* out, size_t n)
{
    out[0] = 0;
    if (!res || !TsmIsValidGamePtr(res) || !ReadablePtr(res, 128)) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    const char* s = (const char*)res;
    size_t i = 0;
    for (; i + 1 < n; i++) {
        const char c = s[i];
        if (!c) break;
        if ((unsigned char)c < 32 || (unsigned char)c > 126) {
            _snprintf_s(out, n, _TRUNCATE, "?");
            return;
        }
        out[i] = c;
    }
    out[i] = 0;
    if (!out[0]) _snprintf_s(out, n, _TRUNCATE, "?");
}

// The building's type-descriptor name, so the log never again asks the reader
// to guess which object a pointer is.
static void BuildingName(void* b, char* out, size_t n)
{
    out[0] = 0;
    if (!b || !g_enterprise || !TsmIsValidGamePtr(b)) {
        _snprintf_s(out, n, _TRUNCATE, "-");
        return;
    }
    const char* nm = nullptr;
    __try { nm = g_enterprise->getBuildingObjectName(b); }
    __except (EXCEPTION_EXECUTE_HANDLER) { nm = nullptr; }
    if (!nm || !nm[0]) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    size_t i = 0;
    for (; i + 1 < n && nm[i]; i++) {
        const char c = nm[i];
        out[i] = ((unsigned char)c < 32 || (unsigned char)c > 126) ? '?' : c;
    }
    out[i] = 0;
}

// The suite's single price function. Everything this plugin charges is
// expressed against THIS number, and every pay line names it, so a change of
// price source can never silently halve or double a payment.
//
// NOTE: resource_pricing itself may be returning its shipped fallbacks
// (RE_DIGEST Part 12 section 3). That is fine and consistent - what must not
// happen is this plugin mixing a raw Res+0x58 read with a priced figure.
static double UnitPrice(void* res)
{
    if (!res || !TsmIsValidGamePtr(res)) return 0.0;

    if (g_pricing) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        const float p = g_pricing->getPrice(res, cur);
        if (p == p && p > 0.01f && p < 1e9f) return (double)p;
    }

    // Fallback: the raw record field, only if resource_pricing is absent.
    if (!ReadablePtr((const BYTE*)res + TSM_RES_PRICE_RUB, 4)) return 0.0;
    const float f = *(const float*)((const BYTE*)res + TSM_RES_PRICE_RUB);
    if (!(f == f) || f < 0.0f || f > 1e9f) return 0.0;
    return (double)f;
}

// v0.4: freight as a share of the cargo's value.
//
// `tons` is positive on this leg (the delivery). A zero or nonsense unit
// price yields zero rather than a guess - the caller counts it as a skip.
static double FreightFor(double tons, double unitPrice)
{
    const double value = tons * unitPrice;
    if (!(value > 0.0)) return 0.0;
    const double share = (double)g_freightShare;
    if (!(share > 0.0)) return 0.0;
    return value * share;
}

// One guard, used everywhere the treasury is read: an older currency host may
// carry a shorter struct, and getTreasuryBalance is past the end of it.
static bool TreasuryReadable(void)
{
    return g_currency && g_currency->getTreasuryBalance &&
           g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                     sizeof(g_currency->getTreasuryBalance);
}

// v0.5: the fuel price, resolved by NAME through resource_pricing so the
// index moves with the game's own economy. Cached for the day and dropped at
// each close, because a save load can replace the resource table under us.
static double FuelUnitPrice(void)
{
    if (!g_pricing || !g_pricing->getByName) return 0.0;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    if (!g_fuelRes) return 0.0;

    const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
    const float p = g_pricing->getPrice(g_fuelRes, cur);
    if (!(p == p) || !(p > 0.0f) || !(p < 1e9f)) return 0.0;
    return (double)p;
}

// v0.7: THE TARIFF. Fuel plus a maintenance reserve, both denominated in
// litres and both priced at the live fuel price:
//
//   work    = litres_per_trip + litres_per_tonne x tonnes
//   fuel    = work x diesel_density x fuel_price        <- the index
//   wear    = wear_litres x diesel_density x fuel_price <- sinking fund
//   tariff  = fuel x (1 + margin) + wear
//
// Two rules are enforced here, not in prose. Nothing is a rouble constant, so
// inflation cannot eat it; and nothing depends on what the cargo is WORTH, so
// a truck of plants pays for its trip the same as a truck of chemicals.
// The wear term exists because fuel x 1.35 can never buy a truck: a carrier
// has to recover vehicles over their life, and that is a per-trip cost.
//
// `*outIndex` is 1 for the fuel index, 2 for the CPI fallback, 0 for neither.
// A 0 return means "this build cannot price this delivery", not "it is free".
static double TariffFor(double tons, double* outFuel, double* outWear,
                        int* outIndex)
{
    if (outFuel)  *outFuel  = 0.0;
    if (outWear)  *outWear  = 0.0;
    if (outIndex) *outIndex = 0;
    if (!(tons > 0.0)) return 0.0;

    double fuel = 0.0, wear = 0.0;
    const double work = gLitresPerTrip + gLitresPerTonne * tons;
    const double fp   = FuelUnitPrice();

    if (fp > 0.0) {
        fuel = work * gDieselDensity * fp;
        wear = gWearLitres * gDieselDensity * fp;
        if (outIndex) *outIndex = 1;
    } else if (g_pricing) {
        const float basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        if (basket == basket && basket > 0.0f && basket < 1e9f) {
            fuel = (gBasketsPerTrip + gBasketsPerTonne * tons) * (double)basket;
            // The reserve expressed in the fallback unit: the litres ratio is
            // dimensionless, so it survives the switch unchanged.
            const double ratio =
                (gLitresPerTrip > 0.0) ? (gWearLitres / gLitresPerTrip) : 0.0;
            wear = fuel * ratio;
            if (outIndex) *outIndex = 2;
        }
    }

    if (outFuel) *outFuel = fuel;
    if (outWear) *outWear = wear;

    const double total = fuel * (1.0 + gTariffMargin) + wear;
    return (total > 0.0) ? total : 0.0;
}

// v0.5: remember who was paid, so the close can show their cash position.
// Four slots: a republic has a handful of private logistics companies, not a
// hundred, and the balance line is about seeing money arrive and leave.
// v0.6: how much of the day's freight went to whom, so the close can say
// "freight in X, spent Y" instead of making the reader subtract.
static void NoteCarrierIncome(void* b, double amount)
{
    if (!b || !(amount > 0.0)) return;
    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { g_balCarrierIn[i] += amount; break; }
    LeaveCriticalSection(&g_lock);
}

static void TrackCarrier(void* b)
{
    if (!b) return;
    EnterCriticalSection(&g_lock);
    bool known = false;
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { known = true; break; }
    if (!known && g_balCarrierCount < BAL_CARRIERS) {
        g_balCarrier[g_balCarrierCount]     = b;
        g_balCarrierPrev[g_balCarrierCount] = -1.0;   // -1 = no baseline yet
        g_balCarrierFuel[g_balCarrierCount] = -1.0;   // first read is a baseline
        g_balCarrierCount++;
    }
    LeaveCriticalSection(&g_lock);
}

static void AppendShipment(const Shipment& s)
{
    int cap = g_ledgerCapacity;
    if (cap < 1)          cap = 1;
    if (cap > LEDGER_MAX) cap = LEDGER_MAX;

    g_ledger[g_ledgerHead % cap] = s;
    g_ledgerHead = (g_ledgerHead + 1) % cap;
    g_ledgerTotal++;
}

// ---------------------------------------------------------------------------
// paying
// ---------------------------------------------------------------------------
//
// One place that moves money, so there is exactly one place to look when a
// balance changes. dry_run decides whether the call lands.
//
//   payee  - the building whose company is credited
//   amount - positive RUB
//   cat    - what the movement is for (enterprise_base's own classification)
//   label  - appears verbatim in currency's flow ledger

static bool Pay(void* payee, double amount, TsmCapitalCategory cat,
                const char* label, const char* kind, void* res, float tons)
{
    // v0.4: every refusal is counted HERE, where the reason is known, so the
    // daily line can say which one it was instead of lumping them together.
    if (!(amount >= (double)g_minPayment)) {
        EnterCriticalSection(&g_lock); g_skipSmall++;    LeaveCriticalSection(&g_lock);
        return false;
    }
    if (!g_enterprise || !g_currency) {
        EnterCriticalSection(&g_lock); g_skipNoPayee++;  LeaveCriticalSection(&g_lock);
        return false;
    }

    char bname[64], rname[64];
    BuildingName(payee, bname, sizeof(bname));
    ResourceName(res, rname, sizeof(rname));

    if (g_dailyPayCap > 0.0 && g_paidToday + amount > g_dailyPayCap) {
        EnterCriticalSection(&g_lock);
        g_payCapped++;
        LeaveCriticalSection(&g_lock);
        return false;
    }

    // v0.4 AFFORDABILITY GATE.
    //
    // moveTreasury takes a signed amount and has no floor: crediting a
    // private company and debiting the treasury by more than it holds would
    // simply drive the balance negative. The state is the payer until Ship 2,
    // so it has to be able to say no. Reads the ACTIVE currency's treasury
    // (RUB by default), the same one moveTreasury writes.
    {
        double have = 0.0;
        if (g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance))
            have = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        if (have < amount) {
            EnterCriticalSection(&g_lock); g_skipTreasury++; LeaveCriticalSection(&g_lock);
            return false;
        }
    }

    if (g_dryRun) {
        Logf("[PAY] would %s %s %+.2f (%s %.3ft) - dry_run",
             kind, bname, amount, rname, (double)tons);
    } else {
        // v0.5 (H33): the treasury is read on both sides of the move. This is
        // the only isolated proof of the payer leg that exists - it cannot be
        // contaminated by wages or taxes, because nothing else runs between
        // the two reads.
        const int cur   = TsmCurrencyActive(g_currency);
        const bool sample = TreasuryReadable();
        double before = 0.0, after = 0.0;
        if (sample) before = g_currency->getTreasuryBalance(cur);

        g_enterprise->modifyCompanyCapital(payee, amount, cat);
        g_currency->moveTreasury(-amount, label);

        if (sample) {
            after = g_currency->getTreasuryBalance(cur);
            const double gap = (after - before) + amount;   // 0 = exact
            const double mag = (gap < 0.0) ? -gap : gap;
            EnterCriticalSection(&g_lock);
            g_treasuryChecked++;
            if (mag <= 0.005) {
                g_treasuryExact++;
            } else {
                g_treasuryOff++;
                if (mag > g_treasuryMaxGap) g_treasuryMaxGap = mag;
            }
            LeaveCriticalSection(&g_lock);
        }

        Logf("[PAY] %s %s %+.2f (%s %.3ft) paid by the state",
             kind, bname, amount, rname, (double)tons);
    }

    EnterCriticalSection(&g_lock);
    g_paidToday += amount;
    g_payCount++;
    if (cat == TSM_CAP_PRODUCTION) g_paidFactory += amount;
    else                          g_paidCarrier += amount;
    LeaveCriticalSection(&g_lock);
    return true;
}

// ---------------------------------------------------------------------------
// the detour
// ---------------------------------------------------------------------------

static int DetourTransfer(void* game, void* p2, unsigned mode, void* res,
                          float tons, float* outMoved, float cap,
                          unsigned char flag, void* p9)
{
    // ARGUMENT NAMES THAT MATTER:
    //
    //   p2 = param_2. At mode 0 this is the BUILDING; at mode 1 the VEHICLE.
    //        Proven in two sessions (RE_DIGEST Part 13 §4).
    //   p9 = param_9. The FLEET OWNER on the delivery family, NULL on the
    //        load family. NOT an endpoint.
    //
    // Getting these two the wrong way round is the single easiest way to
    // build a payment that pays nobody, so they are named for their
    // parameter position and nothing else.

    // Which site called us. The patched instruction is still a `call`, so the
    // CPU pushed site+5 and that is what _ReturnAddress reads.
    const BYTE* retAddr = (const BYTE*)_ReturnAddress();
    unsigned    siteRva = 0;
    SiteClass   known   = SITE_UNKNOWN;
    int         siteIdx = -1;

    if (retAddr && g_exeBase) {
        const intptr_t off = (intptr_t)retAddr - (intptr_t)(g_exeBase + 5);
        if (off > 0 && off < (intptr_t)g_exeSize) {
            siteRva = (unsigned)off;
            siteIdx = SiteIndex(siteRva);
            if (siteIdx >= 0) known = g_sites[siteIdx].known;
        }
    }

    // At mode 0 p2 is the building. At mode 1 it is the vehicle and its home
    // depot is the fleet owner - read it BEFORE the call, because the callee
    // clobbers registers and the vehicle can be re-homed later in the tick.
    void* building = (mode == 0 && p2 && TsmIsValidGamePtr(p2) &&
                      ReadablePtr(p2, TSM_BLD_MIN_READABLE)) ? p2 : nullptr;
    void* owner    = p9;

    void* vehicleHome = nullptr;
    if (mode != 0 && p2 && TsmIsValidGamePtr(p2) &&
        ReadablePtr((const BYTE*)p2 + TSM_VEH_HOME, 8)) {
        vehicleHome = *(void**)((BYTE*)p2 + TSM_VEH_HOME);
    }

    EnterCriticalSection(&g_lock);
    g_calls++;
    LeaveCriticalSection(&g_lock);

    // Forward. The transfer returns a bool, so only the low byte of the return
    // carries the answer.
    const int  r         = g_realTransfer
                         ? g_realTransfer(game, p2, mode, res, tons,
                                          outMoved, cap, flag, p9)
                         : 0;
    const bool committed = ((unsigned char)r == 1u);

    // ---- classify, outside the lock --------------------------------------
    const bool   isStaging = (mode != 0 && vehicleHome && vehicleHome == p9);
    const double unitPrice = UnitPrice(res);

    Shipment s;
    memset(&s, 0, sizeof(s));
    s.vehicle  = (mode != 0) ? p2 : nullptr;
    s.resource = res;
    s.tons     = tons;
    s.site     = (unsigned short)(siteRva & 0xFFFF);
    s.mode     = (unsigned char)mode;
    s.building = building;

    if (committed) {
        if (isStaging) {
            s.kind = SHIP_LOAD;
        } else if (known == SITE_REFUEL) {
            s.kind      = SHIP_FUEL;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        } else if (tons > 0.0f) {
            s.kind      = SHIP_DELIVERY;
            s.unitPrice = unitPrice;
            s.value     = (double)tons * unitPrice;
        } else {
            s.kind      = SHIP_LOAD;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        }
    }

    // ---- carrier resolution (mode 1 only feeds the ring) -----------------
    bool     emitSite   = false;
    bool     emitAttrib = false;
    unsigned shipId     = 0;
    void*    carrier    = nullptr;
    bool     carrierMatched = false;

    EnterCriticalSection(&g_lock);

    if (committed && mode != 0 && vehicleHome) {
        CarrierNote& n = g_carrier[g_carrierHead % CARRIER_RING];
        n.res  = res;
        n.tons = fabsf(tons);
        n.home = vehicleHome;
        n.t  = NowSeconds();
        g_carrierHead = (g_carrierHead + 1) % CARRIER_RING;
    }

    if (committed && mode == 0 && tons < 0.0f && !owner) {
        // The load leg: param_9 is NULL, so find the vehicle that took these
        // goods from the mode-1 call that follows within a few milliseconds.
        const double now = NowSeconds();
        for (int i = 1; i <= CARRIER_RING; i++) {
            const int k = ((g_carrierHead - i) % CARRIER_RING + CARRIER_RING) % CARRIER_RING;
            const CarrierNote& n = g_carrier[k];
            if (!n.home || n.res != res) continue;
            if (now - n.t > 2.0) continue;
            const float tol = (fabsf(tons) > 0.01f) ? fabsf(tons) * 0.005f : 0.0005f;
            if (fabsf(n.tons - fabsf(tons)) > tol) continue;
            carrier = n.home;
            carrierMatched = true;
            break;
        }
    }

    if (committed) g_callsCommitted++;
    else           g_callsNoop++;

    if (siteIdx >= 0) {
        SiteTally& t = g_tally[siteIdx];
        t.calls++;
        if (committed) {
            t.committed++;
            if (tons >= 0.0f) t.tonsIn  += (double)tons;
            else              t.tonsOut += fabs((double)tons);
            t.value += s.value;
        }
        if (p9)           t.sawBld++;
        if (vehicleHome)  t.sawHome++;
    }

    if (committed) {
        if (mode == 0 && !building)                g_unknownBuilding++;
        if (isStaging)                             g_staging++;
        if (mode == 0 && !IsPrivate(building))     g_nonPrivate++;

        if (s.kind == SHIP_FUEL)                    { g_wouldPayFuel    += s.value;
                                                      g_fuelEvents++; }
        if (s.kind == SHIP_DELIVERY && s.value > 0.0) g_wouldPayFreight += s.value;

        s.id  = g_ledgerSeq++;
        s.seq = g_ledgerTotal;
        AppendShipment(s);
        shipId = s.id;
    }

    // Separate budgets: a busy day can never hide a site behind its own
    // attribution output, and payments get a third budget of their own.
    if (g_siteLog) {
        if (g_siteLoggedToday < (unsigned)g_logCapPerDay) {
            g_siteLoggedToday++;
            emitSite = true;
        } else {
            g_siteSuppressed++;
        }
    }
    if (committed) {
        if (g_attrLoggedToday < (unsigned)g_logCapPerDay) {
            g_attrLoggedToday++;
            emitAttrib = true;
        } else {
            g_attrSuppressed++;
        }
    }

    LeaveCriticalSection(&g_lock);

    // ---- logging, outside the lock ---------------------------------------
    if (emitSite) {
        char bname[64];
        BuildingName(building, bname, sizeof(bname));
        Logf("[B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u "
             "p2=%p \"%s\" priv=%d p9=%p vhome=%p class=%s",
             siteRva, mode, (tons < 0.0f ? '-' : '+'), (double)tons,
             committed ? 1 : 0, (unsigned)flag,
             p2, bname, IsPrivate(building) ? 1 : 0, p9, vehicleHome,
             ClassName(known));
    }

    // ---- billing ---------------------------------------------------------
    //
    // MODE 0 ONLY. A mode-1 call is the vehicle side of a movement a mode-0
    // call has already described; billing both would charge it twice.
    // v0.5: the probe runs even with billing off. It is a measurement, and
    // measuring a cost must not depend on whether money is being moved.
    if ((g_billing || g_tariffProbe || g_tripTariff) && committed && mode == 0 && building) {

        char rname[64];
        ResourceName(res, rname, sizeof(rname));

        double freightPaid   = 0.0;
        double probeTariff   = 0.0;
        double probeCost     = 0.0;
        double probeShare    = 0.0;
        int    probeIndex    = 0;
        bool   probeLine     = false;

        if (tons > 0.0f) {

            // ---- v0.7 THE TRIP TARIFF (this IS the money) -----------------
            // v0.6 computed this and paid the value share anyway. v0.7 pays
            // it. `trip_tariff = 0` puts the value share back untouched.
            if (g_tariffProbe || g_tripTariff) {
                probeTariff = TariffFor((double)tons, &probeCost, &probeShare,
                                        &probeIndex);

                EnterCriticalSection(&g_lock);
                if (probeTariff > 0.0) {
                    g_tariffCount++;
                    g_tariffSum       += probeTariff;
                    g_tariffCostSum   += probeCost * (1.0 + gTariffMargin);
                    g_tariffShareSum  += probeShare;
                    if (probeIndex == 1)      g_tariffFuelIndex++;
                    else if (probeIndex == 2) g_tariffCpiIndex++;
                    if (g_tariffLoggedToday < (unsigned)g_tariffLogPerDay) {
                        g_tariffLoggedToday++;
                        probeLine = true;
                    }
                } else {
                    g_tariffNoIndex++;
                }
                LeaveCriticalSection(&g_lock);
            }

            if (g_billing && g_payCarrier) {
                // H2 - freight on a delivery into `building`.
                // The fleet owner is param_9 on this family; if the site does
                // not carry one, fall back to the ring.
                void* paid = nullptr;
                if (owner && owner != building && IsPrivate(owner))      paid = owner;
                else if (!owner && carrier && carrier != building &&
                         IsPrivate(carrier))                             paid = carrier;

                if (paid) {
                    // v0.7: the trip price. The value share survives only for
                    // the comparison path (trip_tariff = 0).
                    const double freight = g_tripTariff
                        ? probeTariff
                        : FreightFor((double)tons, unitPrice);
                    if (freight <= 0.0) {
                        EnterCriticalSection(&g_lock); g_skipNoPrice++; LeaveCriticalSection(&g_lock);
                    } else if (Pay(paid, freight, TSM_CAP_WHOLESALE,
                                   "b2b_freight", "freight", res, tons)) {
                        freightPaid = freight;
                        TrackCarrier(paid);
                        NoteCarrierIncome(paid, freight);
                        BurnProbe(paid);              // v0.11: per-delivery tank sample
                    }
                } else {
                    EnterCriticalSection(&g_lock); g_skipNoPayee++; LeaveCriticalSection(&g_lock);
                }
            }

            // One line carries both numbers, so the calibration is a
            // subtraction the reader can do in their head.
            if (probeLine) {
                char bname[64];
                BuildingName(building, bname, sizeof(bname));
                Logf("[TARIFF] %s %.3ft @%.2f = %.2f -> %s: %.2f = fuel %.2f x%.2f "
                     "+ wear %.2f (index %s) | charged %.2f by the state",
                     rname, (double)tons, unitPrice, (double)tons * unitPrice, bname,
                     probeTariff, probeCost, 1.0 + (double)gTariffMargin, probeShare,
                     probeIndex == 1 ? "fuel" : (probeIndex == 2 ? "cpi" : "none"),
                     freightPaid);
            }
        }

        if (g_billing && tons < 0.0f && g_payFactory) {
            // H1 - the private building the goods leave is paid for them.
            if (IsPrivate(building)) {
                if (unitPrice > 0.0) {
                    const double value =
                        fabs((double)tons) * unitPrice * (double)g_factoryShare;
                    Pay(building, value, TSM_CAP_PRODUCTION,
                        "b2b_factory", "factory", res, tons);
                } else {
                    // A private building shipping goods this build cannot
                    // price. Count it and say so once per day-cap window:
                    // silence here would look like "nothing happened".
                    char bname[64];
                    BuildingName(building, bname, sizeof(bname));
                    EnterCriticalSection(&g_lock);
                    g_skipNoPrice++;
                    const bool say = (g_payLoggedToday < (unsigned)g_logCapPerDay);
                    if (say) g_payLoggedToday++;
                    LeaveCriticalSection(&g_lock);
                    if (say)
                        Logf("[PAY] skipped %s: no price for %s (unitPrice 0.00) - "
                             "resource_pricing absent or unpriced", bname, rname);
                }
            }
        }

        (void)carrierMatched;
    }

    if (emitAttrib) {
        char bname[64], rname[64];
        BuildingName(building, bname, sizeof(bname));
        ResourceName(res, rname, sizeof(rname));
        Logf("[B2B] #%u %-6s bld=%p \"%s\" priv=%d res=%s %.2ft @%.2f = %.2f "
             "(site 0x%06X, mode %u)",
             shipId, KindName(s.kind), building, bname, IsPrivate(building) ? 1 : 0,
             rname, (double)s.tons, s.unitPrice, s.value, siteRva, mode);
    }

    return r;
}

// ---------------------------------------------------------------------------
// call-site patching
// ---------------------------------------------------------------------------

static bool PatchSite(int index)
{
    if (index < 0 || index >= g_siteCount) return false;
    if (g_sitePatched[index]) return true;

    const unsigned rva = g_sites[index].rva;

    if (rva < 0x1000 || (size_t)rva + 5 > g_exeSize) {
        Logf("private_logistics  site 0x%06X is outside the image (%llu bytes) - skipped",
             rva, (unsigned long long)g_exeSize);
        return false;
    }

    BYTE* site = g_exeBase + rva;
    if (!ReadablePtr(site, 5)) {
        Logf("private_logistics  site 0x%06X not readable - skipped", rva);
        return false;
    }

    // Verify: a direct call, landing on the transfer.
    if (site[0] != 0xE8) {
        Logf("private_logistics  site 0x%06X is not a call (0x%02X) - skipped",
             rva, site[0]);
        return false;
    }
    const int32_t rel    = *(const int32_t*)(site + 1);
    BYTE*         target = site + 5 + rel;
    if (target != g_exeBase + TSM_RVA_TRANSFER) {
        Logf("private_logistics  site 0x%06X calls %p, not the transfer - skipped",
             rva, target);
        return false;
    }

    // The cave: `jmp qword ptr [rip+0]` plus the absolute detour address.
    BYTE* cave = AllocNear(site, 32);
    if (!cave) {
        Logf("private_logistics  no code cave near 0x%06X - skipped", rva);
        return false;
    }

    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&DetourTransfer;
    memcpy(cave + 6, &detour, 8);
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("private_logistics  cave out of rel32 range at 0x%06X - skipped", rva);
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("private_logistics  VirtualProtect failed at 0x%06X - nothing written", rva);
        return false;
    }

    // Re-verify under the protection.
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("private_logistics  site 0x%06X changed under us - nothing written", rva);
        return false;
    }

    site[0] = 0xE8;                              // same opcode: still a call
    *(int32_t*)(site + 1) = (int32_t)toCave;     // new destination
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    g_sitePatched[index] = true;
    Logf("private_logistics  hook 0x%06X -> transfer ok (cave %p)", rva, (void*)cave);
    return true;
}

static void PatchAllSites(void)
{
    if (!g_siteFilterActive)
        for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = true;

    int ok = 0, wanted = 0;
    for (int i = 0; i < g_siteCount; i++) {
        if (!g_siteWanted[i]) continue;
        wanted++;
        if (PatchSite(i)) ok++;
    }

    if (g_siteFilterActive)
        Logf("private_logistics  site_list active: %d of %d sites selected",
             wanted, g_siteCount);

    Logf("private_logistics  patched %d of %d transfer call sites", ok, wanted);

    if (ok == 0) {
        Logf("private_logistics  ERROR  no sites patched. Either this is not "
             "SOVIET64.exe v1.1.1.9, or the transfer is not at RVA 0x420350. "
             "Nothing was written; the game is unaffected.");
    } else if (ok < wanted) {
        Logf("private_logistics  WARN  %d site(s) refused - see the lines above. "
             "A refusal is safe: that call still reaches the engine normally.",
             wanted - ok);
    }
}

// ---------------------------------------------------------------------------
// the daily roll-up
// ---------------------------------------------------------------------------

static void OnDay(void*, int day, int month, int year)
{
    (void)year;

    // Snapshot under the lock, clear everything, print outside it.
    //
    // v0.3 BUGFIX: v0.2 snapshotted these counters without zeroing them, so
    // its "[B2B] day N" line was cumulative since load while the per-site
    // table was genuinely per-day. Both are per-day now.
    static SiteTally snap[MAX_SITES];
    unsigned calls, committed, noop, unknown, stag, nonPriv;
    unsigned siteSupp, attrSupp, paySupp, payCount, payCap, total;
    unsigned skNoPayee, skNoPrice, skSmall, skTreasury;
    unsigned tCount, tNoIndex, tFuelIdx, tCpiIdx;
    unsigned trChecked, trExact, trOff, fuelEvents;
    double   paidF, paidC, freight, fuel, tSum, tCostSum, tShareSum, trMaxGap;
    static double cIn[BAL_CARRIERS];
    static void*  cPt[BAL_CARRIERS];
    int      cCount;
    bool     newMonth;

    EnterCriticalSection(&g_lock);
    memcpy(snap, g_tally, sizeof(snap));
    memset(g_tally, 0, sizeof(g_tally));

    calls      = g_calls;
    committed  = g_callsCommitted;
    noop       = g_callsNoop;
    unknown    = g_unknownBuilding;
    stag       = g_staging;
    nonPriv    = g_nonPrivate;
    siteSupp   = g_siteSuppressed;
    attrSupp   = g_attrSuppressed;
    paySupp    = g_paySuppressed;
    payCount   = g_payCount;
    payCap     = g_payCapped;
    skNoPayee  = g_skipNoPayee;
    skNoPrice  = g_skipNoPrice;
    skSmall    = g_skipSmall;
    skTreasury = g_skipTreasury;
    tCount     = g_tariffCount;
    tNoIndex   = g_tariffNoIndex;
    tFuelIdx   = g_tariffFuelIndex;
    tCpiIdx    = g_tariffCpiIndex;
    tSum       = g_tariffSum;
    trChecked  = g_treasuryChecked;
    trExact    = g_treasuryExact;
    trOff      = g_treasuryOff;
    trMaxGap   = g_treasuryMaxGap;
    fuelEvents = g_fuelEvents;
    tCostSum   = g_tariffCostSum;
    tShareSum  = g_tariffShareSum;
    cCount     = g_balCarrierCount;
    for (int i = 0; i < BAL_CARRIERS; i++) {
        cPt[i] = g_balCarrier[i];
        cIn[i] = g_balCarrierIn[i];
        g_balCarrierIn[i] = 0.0;      // this day's income, spent by the close
    }
    total      = g_ledgerTotal;
    paidF      = g_paidFactory;
    paidC      = g_paidCarrier;
    freight    = g_wouldPayFreight;
    fuel       = g_wouldPayFuel;

    g_calls = g_callsCommitted = g_callsNoop = 0;
    g_unknownBuilding = g_staging = g_nonPrivate = 0;
    g_siteSuppressed = g_attrSuppressed = g_paySuppressed = 0;
    g_payCount = g_payCapped = 0;
    g_skipNoPayee = g_skipNoPrice = g_skipSmall = g_skipTreasury = 0;
    g_tariffCount = g_tariffNoIndex = g_tariffFuelIndex = g_tariffCpiIndex = 0;
    g_tariffCostSum = g_tariffShareSum = 0.0;
    g_tariffSum = 0.0;
    g_treasuryChecked = g_treasuryExact = g_treasuryOff = 0;
    g_treasuryMaxGap = 0.0;
    g_fuelEvents = 0;
    g_fuelRes = nullptr;          // re-resolve the index next day
    g_paidFactory = g_paidCarrier = 0.0;
    g_wouldPayFreight = g_wouldPayFuel = 0.0;
    g_paidToday = 0.0;

    g_siteLoggedToday = 0;
    g_attrLoggedToday = 0;
    g_payLoggedToday  = 0;
    g_tariffLoggedToday = 0;

    newMonth = (month != g_lastMonth);
    g_lastMonth = month;

    // v0.4: this callback fires at the START of `day`, so everything above is
    // the day that just ENDED. Report that day, not this one - the old label
    // was one ahead of its own numbers.
    // A loaded save can rewind the clock; do not report a day in the future.
    if (g_summaryDay > day) g_summaryDay = -1;
    const int closedDay = (g_summaryDay >= 0) ? g_summaryDay : (day - 1);
    g_summaryDay = day;
    LeaveCriticalSection(&g_lock);

    Logf("[B2B] day %d closed: %u calls, %u committed, %u no-op | unknown %u, "
         "staging %u, non-private %u | suppressed site %u attrib %u pay %u",
         closedDay, calls, committed, noop, unknown, stag, nonPriv, siteSupp,
         attrSupp, paySupp);

    // Per-site table. A site that never fired is simply absent - absence is
    // evidence rather than a logging artefact.
    for (int i = 0; i < g_siteCount; i++) {
        const SiteTally& t = snap[i];
        if (t.calls == 0) continue;
        Logf("[B2B SITE] day %d  0x%06X %-9s calls=%u commit=%u in=%.3f out=%.3f "
             "value=%.2f | arg9!=0 %u home!=0 %u",
             closedDay, g_sites[i].rva, ClassName(g_sites[i].known), t.calls,
             t.committed, t.tonsIn, t.tonsOut, t.value, t.sawBld, t.sawHome);
    }

    if (g_billing) {
        Logf("[PAY] day %d closed: %u payment(s), %u capped | skipped: no payee %u, "
             "no price %u, too small %u, treasury %u | factory %.2f, carrier %.2f | %s",
             closedDay, payCount, payCap, skNoPayee, skNoPrice, skSmall,
             skTreasury, paidF, paidC,
             g_dryRun ? "DRY RUN - nothing moved"
                      : "PAID by the state treasury");
    } else {
        Logf("[PAY] day %d closed: billing off", closedDay);
    }

    // v0.5: the model's own total, beside the index it managed to use.
    // `unindexed` is the honest failure: neither a fuel price nor a CPI
    // basket resolved, so the probe refused to invent a figure.
    if (g_tariffProbe || g_tripTariff) {
        Logf("[TARIFF] day %d closed: %u delivery(ies), %s %.2f = fuel %.2f x%.2f + "
             "wear %.2f, mean %.2f | indexed by fuel %u, by cpi %u, unindexed %u",
             closedDay, tCount, g_tripTariff ? "charged" : "model",
             tSum, tCostSum, 1.0 + (double)gTariffMargin, tShareSum,
             tCount ? tSum / (double)tCount : 0.0,
             tFuelIdx, tCpiIdx, tNoIndex);
    }

    // v0.5: THE MONEY PATH, BOTH ENDS (H33, H37).
    //
    // v0.4 proved the payee's account to the kopeck; it never read the payer.
    // Here the treasury is sampled at every close, so consecutive closes give
    // the movement, and `unexplained` is what the [PAY] lines cannot account
    // for - zero when the books close, non-zero when something else touched
    // the treasury in the same day.
    //
    // The carriers' capital is printed beside it. Income minus everything the
    // company spent, fuel included: if the game charges a private DO for
    // diesel, the difference between this delta and the freight it was paid
    // IS that bill - the number Ship 2 has to price against.
    if (g_balanceLog) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(cur);
            have  = true;
        }

        const double paid = paidF + paidC;

        if (!have) {
            Logf("[BAL] day %d closed: treasury unavailable (no currency service)", closedDay);
        } else if (!g_treasuryHaveClose) {
            Logf("[BAL] day %d closed: treasury %.2f (baseline - the next close gives the "
                 "movement) | paid %.2f", closedDay, treas, paid);
        } else {
            const double moved = treas - g_treasuryAtClose;
            Logf("[BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid "
                 "%.2f | the rest of the movement is the rest of the state's day%s",
                 closedDay, g_treasuryAtClose, treas, moved, paid,
                 g_dryRun ? " | DRY RUN: nothing should have moved" : "");
        }
        if (have) { g_treasuryAtClose = treas; g_treasuryHaveClose = true; }

        // v0.5 (H33): the proof, per payment rather than per day.
        if (trChecked > 0 || trOff > 0) {
            Logf("[BAL] day %d treasury leg: %u payment(s) sampled either side, "
                 "%u moved exactly the amount, %u off by any margin (largest %.2f)",
                 closedDay, trChecked, trExact, trOff, trMaxGap);
        }

        // v0.6: the carrier's day in one line. `spent` is the arithmetic
        // difference - freight in, minus the change in capital - and in this
        // save it is the fuel bill, because fuel is the only thing a private
        // DO buys (it arrives as a cat=2 FOREIGN debit on the same account).
        for (int i = 0; i < cCount; i++) {
            void* b = cPt[i];
            if (!b || !g_enterprise || !g_enterprise->getCompanyCapital) continue;
            const double cap = g_enterprise->getCompanyCapital(b);
            char bname[64];
            BuildingName(b, bname, sizeof(bname));
            if (g_balCarrierPrev[i] < 0.0) {
                Logf("[BAL] day %d   %s capital %.2f (baseline) | freight in %.2f",
                     closedDay, bname, cap, cIn[i]);
            } else {
                const double delta = cap - g_balCarrierPrev[i];
                Logf("[BAL] day %d   %s capital %.2f (%+.2f) | freight in %.2f | spent %.2f",
                     closedDay, bname, cap, delta, cIn[i], cIn[i] - delta);
            }

            // v0.7: THE FUEL METER.
            //
            // monthWholesale is where enterprise_base books cat=2 FOREIGN
            // debits, and diesel is the only foreign purchase a distribution
            // office makes - so this is the carrier's real fuel bill for the
            // calendar month, read back from the game rather than assumed.
            //
            // Fuel is bought in bulk and stored, so a day's purchases are not
            // a day's burn. The run average is, because the tank delta washes
            // out of a long run. That is the number the [TARIFF] model is
            // scored against, and the number litres_per_trip should hold.
            if (g_enterprise->describeEnterprise) {
                TsmEnterpriseInfo info;
                memset(&info, 0, sizeof(info));
                info.structSize = sizeof(TsmEnterpriseInfo);
                if (g_enterprise->describeEnterprise(b, &info) == 1) {
                    const double bill = info.monthWholesale;
                    double spent = 0.0;
                    if (g_balCarrierFuel[i] < 0.0) {
                        // First read after a load: monthWholesale already holds
                        // whatever the save carried. Take it as the baseline and
                        // say so - counting it would bill one day for a month.
                        g_balCarrierFuel[i] = bill;
                        Logf("[FUEL] day %d   %s meter baseline: month wholesale %.2f "
                             "(not counted as this day's burn)", closedDay, bname, bill);
                    } else {
                        spent = (bill < g_balCarrierFuel[i])
                              ? bill                      // month boundary: accumulators cleared
                              : (bill - g_balCarrierFuel[i]);
                        g_balCarrierFuel[i] = bill;
                    }
                    g_fuelPaidRun += (spent > 0.0) ? spent : 0.0;
                    const double fprice = FuelUnitPrice();
                    double litres = 0.0;
                    if (fprice > 0.0 && gDieselDensity > 0.0)
                        litres = ((spent > 0.0) ? spent : 0.0) / (gDieselDensity * fprice);
                    g_fuelLitresRun += litres;
                    Logf("[FUEL] day %d   %s diesel %.2f (measured, cat=2) = %.2f L at %.2f/%s",
                         closedDay, bname, (spent > 0.0) ? spent : 0.0, litres, fprice,
                         g_currency ? TsmCurrencySymbol(g_currency) : "RUB");
                }
            }
            g_balCarrierPrev[i] = cap;
        }

        // v0.7: the run total, and the one number to copy into the ini.
        g_fuelTripsRun += tCount;
        if (g_fuelTripsRun > 0) {
            const double perTrip = g_fuelLitresRun / (double)g_fuelTripsRun;
            // v0.9: this counts PURCHASES, not burn. Fuel is bought in bulk
            // and sits in tanks, so a short run and a long run give different
            // answers from the same fleet - 3.27 L/trip over 1,100 trips,
            // 7.14 over 214. Do not treat it as a measurement of the burn.
            Logf("[FUEL] run: %.2f RUB = %.2f L BOUGHT over %llu delivery(ies) = %.3f L/trip "
                 "| configured %.2f L | fuel sits in tanks: this is purchases, not burn",
                 g_fuelPaidRun, g_fuelLitresRun, g_fuelTripsRun, perTrip, gLitresPerTrip);
        }

        // v0.10: THE BURN, from the tanks.
        //
        // This is the number the tariff has been missing since v0.5. It is not
        // purchases (which include tank fill), it is not a model (which was
        // wrong by 4x): it is what actually left the vehicles' tanks while
        // they worked. Refills are counted separately and never subtract.
        if (g_fuelTripsRun > 0 && g_burnTonsRun > 0.0 && gDieselDensity > 0.0) {
            const double litres = g_burnTonsRun / gDieselDensity;
            const double lpt    = litres / (double)g_fuelTripsRun;
            // v0.11: L/DAY is printed beside L/trip because the physics is
            // per-tick. L/trip moves whenever the shops order differently -
            // the same fleet gave 3.27 and 7.14 L/trip in two runs - while
            // L per fleet-day is the same quantity in both. Read this one.
            const double lpd = g_tankDaysRun ? litres / (double)g_tankDaysRun : 0.0;
            const double lvd = g_tankVehDaysRun ? litres / (double)g_tankVehDaysRun : 0.0;
            const double lva = g_activeVehDaysRun ? litres / (double)g_activeVehDaysRun : 0.0;
            Logf("[TANK] run: burned %.4f t = %.1f L | %.2f L/trip over %llu delivery(ies) "
                 "| %.2f L per fleet-day over %u close(s) | %.2f L per truck-day over %u "
                 "| %.2f L per ACTIVE truck-day over %u active / %u idle "
                 "| %u refill(s) | configured %.2f L/trip",
                 g_burnTonsRun, litres, lpt, g_fuelTripsRun, lpd, g_tankDaysRun, lvd,
                 g_tankVehDaysRun, lva, g_activeVehDaysRun, g_idleVehDaysRun,
                 g_refillsRun, gLitresPerTrip);
            if (g_litresAuto) {
                const double before = gLitresPerTrip;
                gLitresPerTrip = before * 0.70 + lpt * 0.30;
                if (gLitresPerTrip < 0.5)   gLitresPerTrip = 0.5;
                if (gLitresPerTrip > 500.0) gLitresPerTrip = 500.0;
                if (fabs(gLitresPerTrip - before) > 0.01)
                    Logf("[TANK] auto-tune: litres_per_trip %.3f -> %.3f (tank says %.3f)",
                         before, gLitresPerTrip, lpt);
            }
        }
    }

    // v0.11: THE MEASURED BURN, per delivery rather than per day.
    //
    // This is the number the player asked for: diesel that actually left the
    // tanks between one delivery and the next. It needs no purchases, no
    // storage, and no guess about how far a truck drives. Once it has enough
    // intervals it replaces the configured figure - one fleet, one republic,
    // measured where it is used.
    if (g_probeStepsRun > 0 && gDieselDensity > 0.0) {
        const double litres = g_probeBurnRun / gDieselDensity;
        const double lpt    = litres / (double)g_probeStepsRun;
        Logf("[BURN] run: measured %.4f t = %.1f L between %u delivery pair(s) "
             "= %.2f L/trip | %u interval(s) skipped (refill or fleet change) "
             "| configured %.2f L/trip | %s",
             g_probeBurnRun, litres, g_probeStepsRun, lpt, g_probeSkipRun,
             gLitresPerTrip,
             (g_probeStepsRun >= 20 && fabs(lpt - gLitresPerTrip) > 0.5)
                 ? "SET litres_per_trip TO THE MEASURED FIGURE"
                 : "no action");
    }

    // v0.8: who does this company actually own, and what can they carry?
    CensusFleets(closedDay);

    // v0.6: re-read the index at every close. At plugin start the world has
    // not loaded, so v0.5's "fuel NOT FOUND" and "treasury opens at 0.00" were
    // both true for about a second and then false forever. Printing them here
    // is the honest place.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        Logf("[IDX] day %d closed: fuel '%s' = %.2f/%s (%s) | cpi basket %.2f/day | "
             "margin %.0f%%, wear %.2f L",
             closedDay, gFuelResource, fp,
             g_currency ? TsmCurrencySymbol(g_currency) : "RUB",
             fp > 0.0 ? "resolved" : "NOT RESOLVED",
             (double)basket, gTariffMargin * 100.0, gWearLitres);
    }

    // v0.5: fuel telemetry. The refuel site has been silent in every session
    // so far; a count of 0 here after fuel is switched on is itself a result.
    Logf("[FUEL] day %d refuel sites: %u call(s), %.2f RUB of fuel moved through "
         "the hooked sites", closedDay, fuelEvents, fuel);

    if (newMonth) {
        Logf("[B2B] month %d: %u shipments recorded, %.2f RUB would have moved "
             "(freight %.2f, fuel %.2f)",
             month, total, freight + fuel, freight, fuel);
    }

    AutoTargetsOnDay(day, closedDay);
}

// ---------------------------------------------------------------------------
// automatic private DO targets and infrastructure discovery (v0.16)
// ---------------------------------------------------------------------------

#define AUTO_RVA_VECTOR_PUSH_BACK 0x08E050
#define AUTO_RVA_TASKCFG_CTOR     0x1512A0
#define AUTO_DO_TARGETS_BEGIN     0x0D38
#define AUTO_DO_TARGETS_END       0x0D40
#define AUTO_DO_DIRTY             0x0D50
#define AUTO_DO_UI_TIMER          0x0FC0
// Conservative plugin-side bound: native add-flow supports five selections,
// but panel+0x310 per-target control-array capacity above five is unverified.
// This limits new auto targets without deleting/purging any manual entries.
#define AUTO_DO_SAFE_TARGET_COUNT 5
#define AUTO_DO_ROAD_TYPE         43
#define AUTO_DO_RAIL_TYPE         52
#define AUTO_STORAGE_BUILDING_TYPE 5
#define AUTO_CUSTOMS_TYPE         20
#define AUTO_CUSTOMS_MAX          512
#define AUTO_STATE_STORAGE_MAX    512
#define AUTO_STATE_PERMISSION_MAX 512
#define AUTO_STATE_PREF_FILE "private_logistics_state.bin"
struct AutoCustomSite { void* building; float x; float z; char name[64]; };
// permission bits: 1=load FROM site, 2=deliver INTO site; zero=deny both.
struct AutoStateStorageSite { void* building; uint64_t key; float x; float z; char name[64]; unsigned char permission; };
struct AutoStatePermission { uint64_t key; unsigned char mode; };
static AutoCustomSite g_autoCustomSites[AUTO_CUSTOMS_MAX];
static AutoStateStorageSite g_autoStateStorageSites[AUTO_STATE_STORAGE_MAX];
static unsigned g_autoStateStorageCount = 0;
static AutoStatePermission g_statePermissions[AUTO_STATE_PERMISSION_MAX];
static unsigned g_statePermissionCount = 0;
#define AUTO_TARGET_ENTRY_SIZE    0x18
#define AUTO_TASKCFG_SIZE         0x198
#define AUTO_STORAGE_STRIDE       0xE0
#define AUTO_MAX_TARGETS_READ     65536

static uint64_t AutoHashMix(uint64_t h, uint8_t byte)
{
    return (h ^ byte) * 1099511628211ull;
}

static bool AutoReadBuildingType(void* b, int* outType)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return false;
    void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td) || !ReadablePtr((BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
        return false;
    *outType = *(int*)((BYTE*)td + TSM_TD_TYPE_NUM);
    return true;
}

static bool AutoBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // The building's position is the translation row of its embedded
    // C3D_NODE, not bld+0x08/+0x10 (those yielded zeros in the v0.15 probe).
    const BYTE* node = (const BYTE*)b + TSM_BLD_NODE;
    if (ReadablePtr(node, 0x40)) {
        const float x = *(const float*)(node + 0x30);
        const float z = *(const float*)(node + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            if (outX) *outX = x;
            if (outZ) *outZ = z;
            return true;
        }
    }

    // Some unfinished sites can lack a usable node transform; use the
    // verified placement-bbox fallback used by enterprise_base.
    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                const float x = mn[1], z = mn[3];
                if (x == x && z == z && fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
                    if (outX) *outX = x;
                    if (outZ) *outZ = z;
                    return true;
                }
            }
        }
    }
    return false;
}

// Stable session/save identity: native object name + type + quantized position.
// No raw Building* is retained in the retry ledger.
static bool AutoBuildingKey(void* b, uint64_t* outKey, int* outType)
{
    int type = -1;
    float x = 0.0f, z = 0.0f;
    if (!AutoReadBuildingType(b, &type) || !AutoBuildingPosition(b, &x, &z))
        return false;

    uint64_t h = 14695981039346656037ull;
    const char* objectName = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { objectName = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { objectName = nullptr; }
    }
    if (objectName) {
        for (size_t i = 0; i < 128; ++i) {
            if (!ReadablePtr(objectName + i, 1)) return false;
            const uint8_t c = (uint8_t)objectName[i];
            h = AutoHashMix(h, c);
            if (c == 0) break;
        }
    }
    const int32_t qx = (int32_t)(x * 10.0f + (x >= 0.0f ? 0.5f : -0.5f));
    const int32_t qz = (int32_t)(z * 10.0f + (z >= 0.0f ? 0.5f : -0.5f));
    const uint32_t values[3] = { (uint32_t)type, (uint32_t)qx, (uint32_t)qz };
    const uint8_t* bytes = (const uint8_t*)values;
    for (size_t i = 0; i < sizeof(values); ++i) h = AutoHashMix(h, bytes[i]);
    // H1 FIX: mix raw float bits to break 0.1m quantized collisions (two clones within 10cm)
    // keeps qx/qz for backward compat but adds sub-decimeter entropy; one-time permission reset for old saves
    uint32_t fx = 0, fz = 0;
    memcpy(&fx, &x, sizeof(fx));
    memcpy(&fz, &z, sizeof(fz));
    const uint8_t* fxB = (const uint8_t*)&fx; for (size_t i = 0; i < sizeof(fx); ++i) h = AutoHashMix(h, fxB[i]);
    const uint8_t* fzB = (const uint8_t*)&fz; for (size_t i = 0; i < sizeof(fz); ++i) h = AutoHashMix(h, fzB[i]);
    *outKey = h ? h : 1;
    if (outType) *outType = type;
    return true;
}

static void AutoBuildingName(void* b, char* out, size_t capacity)
{
    if (!out || capacity == 0) return;
    out[0] = 0;
    const char* name = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { name = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { name = nullptr; }
    }
    if (name) {
        size_t i = 0;
        for (; i + 1 < capacity; ++i) {
            if (!ReadablePtr(name + i, 1)) break;
            out[i] = name[i];
            if (out[i] == 0) return;
        }
        out[i] = 0;
        if (i > 0) return;
    }
    const char fallback[] = "unknown";
    size_t j = 0;
    while (j + 1 < capacity && fallback[j]) { out[j] = fallback[j]; ++j; }
    out[j] = 0;
}

static unsigned char AutoStatePermissionGet(uint64_t key)
{
    if (!key) return 0;
    unsigned char mode = 0;
    EnterCriticalSection(&g_lock);
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { mode = g_statePermissions[i].mode; break; }
    }
    LeaveCriticalSection(&g_lock);
    return mode;
}

static bool AutoStatePermissionSet(uint64_t key, unsigned char mode)
{
    if (!key || mode > AUTO_ALLOW_BOTH) return false;
    EnterCriticalSection(&g_lock);
    unsigned found = g_statePermissionCount;
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { found = i; break; }
    }
    if (mode == 0) {
        if (found < g_statePermissionCount) {
            g_statePermissions[found] = g_statePermissions[g_statePermissionCount - 1];
            --g_statePermissionCount;
        }
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (found < g_statePermissionCount) {
        g_statePermissions[found].mode = mode;
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (g_statePermissionCount >= AUTO_STATE_PERMISSION_MAX) {
        LeaveCriticalSection(&g_lock);
        return false;
    }
    g_statePermissions[g_statePermissionCount].key = key;
    g_statePermissions[g_statePermissionCount].mode = mode;
    ++g_statePermissionCount;
    LeaveCriticalSection(&g_lock);
    return true;
}

static bool AutoHasNativeStorage(void* b);

static unsigned AutoBuildingCapabilities(void* building)
{
    int type = -1;
    if (!AutoReadBuildingType(building, &type) || !AutoHasNativeStorage(building)) return 0;
    char asset[128];
    AutoBuildingName(building, asset, sizeof(asset));
    return AutoSiteCapabilities(type, asset);
}

static void AutoDrawStatePermissions(uint64_t key, unsigned capabilities)
{
    const unsigned char oldMode = AutoStatePermissionGet(key);
    unsigned char newMode = (unsigned char)(oldMode & capabilities);
    if (capabilities & AUTO_ALLOW_LOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_LOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO loading (take goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_LOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (capabilities & AUTO_ALLOW_UNLOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_UNLOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO deliveries (receive goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_UNLOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (newMode != oldMode) {
        if (!AutoStatePermissionSet(key, newMode)) {
            Logf("[AUTO DO] state permission table full; change not retained");
            return;
        }
        Logf("[AUTO DO] state permissions key=%016llX load=%u unload=%u",
             (unsigned long long)key, (newMode & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (newMode & AUTO_ALLOW_UNLOAD) ? 1u : 0u);
    }
}

static void StateStorageWidgetProvider(void* building, void*)
{
    if (!g_autoTargetsEnabled || !g_uiPanels || !TsmUiPanelsHasWidgets(g_uiPanels) ||
        !building || !TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE) || !g_enterprise ||
        !g_enterprise->isBuildingPrivate || g_enterprise->isBuildingPrivate(building)) return;
    const unsigned capabilities = AutoBuildingCapabilities(building);
    uint64_t key = 0;
    if (!AutoBuildingKey(building, &key, nullptr)) return;
    static uint64_t lastPanelKey = 0;
    if (lastPanelKey != key) {
        lastPanelKey = key;
        int type = -1; AutoReadBuildingType(building, &type);
        char asset[128]; AutoBuildingName(building, asset, sizeof(asset));
        const unsigned effective = AutoStatePermissionGet(key) & capabilities;
        Logf("[AUTO DO] state panel key=%016llX type=%d asset=%s capabilities=%u permissions=%u native_load=%u native_unload=%u (native 0=checked,1=unchecked)%s",
             (unsigned long long)key, type, asset, capabilities, effective,
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_LOAD) != 0),
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_UNLOAD) != 0),
             capabilities ? "" : " (unsupported role: controls hidden, automatic access denied)");
    }
    if (!capabilities) return; // unknown roles do not get a misleading loading control
    AutoDrawStatePermissions(key, capabilities);
}

static bool AutoHasNativeStorage(void* b)
{
    if (!b || !TsmIsValidGamePtr(b) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_STORAGES_BEGIN, 16)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
    const uintptr_t end = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_END);
    if (!begin || end <= begin || ((end - begin) % AUTO_STORAGE_STRIDE) != 0) return false;
    const uintptr_t count = (end - begin) / AUTO_STORAGE_STRIDE;
    return count <= 4096 && ReadablePtr((const void*)begin, count * AUTO_STORAGE_STRIDE);
}

static bool AutoReadTargetVector(void* office, void*** outBegin, size_t* outCount)
{
    if (!office || !TsmIsValidGamePtr(office) ||
        !ReadablePtr((BYTE*)office + AUTO_DO_TARGETS_BEGIN, 24)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_BEGIN);
    const uintptr_t end   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END);
    const uintptr_t cap   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END + 8);
    if (!begin && !end && !cap) { *outBegin = nullptr; *outCount = 0; return true; }
    if (!begin || !end || !cap || end < begin || cap < end || ((end - begin) & 7) != 0)
        return false;
    const size_t count = (size_t)((end - begin) / sizeof(void*));
    if (count > AUTO_MAX_TARGETS_READ ||
        !ReadablePtr((const void*)begin, count * sizeof(void*))) return false;
    *outBegin = (void**)begin; *outCount = count;
    return true;
}

static unsigned AutoOfficeHash(uint64_t officeKey)
{
    officeKey ^= officeKey >> 33; officeKey *= 0xff51afd7ed558ccdull;
    officeKey ^= officeKey >> 33; officeKey *= 0xc4ceb9fe1a85ec53ull;
    officeKey ^= officeKey >> 33;
    return (unsigned)officeKey & (AUTO_OFFICE_HASH_SIZE - 1);
}

static bool AutoBuildOfficeMemoIndex(void)
{
    memset(g_autoOfficeMemoHeads, 0, sizeof(g_autoOfficeMemoHeads));
    for (unsigned i = 0; i < g_autoMemoCount; ++i) {
        AutoMemo& memo = g_autoMemo[i];
        unsigned slot = AutoOfficeHash(memo.officeKey);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
            AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
            if (head.officeKey == 0 || head.officeKey == memo.officeKey) {
                head.officeKey = memo.officeKey;
                memo.nextOffice = head.headPlusOne ? head.headPlusOne - 1 : 0xFFFFFFFFu;
                head.headPlusOne = i + 1;
                inserted = true;
                break;
            }
            slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoOfficeMemoHeadIndex(uint64_t officeKey)
{
    unsigned slot = AutoOfficeHash(officeKey);
    for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
        const AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
        if (head.officeKey == 0) return -1;
        if (head.officeKey == officeKey)
            return head.headPlusOne ? (int)(head.headPlusOne - 1) : -1;
        slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
    }
    return -1;
}

static unsigned AutoPairHash(uint64_t officeKey, uint64_t targetKey)
{
    uint64_t h = officeKey ^ (targetKey + 0x9E3779B97F4A7C15ull + (officeKey << 6) + (officeKey >> 2));
    h ^= h >> 30; h *= 0xBF58476D1CE4E5B9ull;
    h ^= h >> 27; h *= 0x94D049BB133111EBull;
    h ^= h >> 31;
    return (unsigned)h & (AUTO_MEMO_HASH_SIZE - 1);
}

static int AutoMemoFind(uint64_t officeKey, uint64_t targetKey)
{
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        const unsigned stored = g_autoMemoHash[slot];
        if (stored == 0) return -1;
        const unsigned index = stored - 1;
        if (g_autoMemo[index].officeKey == officeKey &&
            g_autoMemo[index].targetKey == targetKey) return (int)index;
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static int AutoPointerIndex(void* building)
{
    if (!building) return -1;
    uintptr_t h = (uintptr_t)building;
    h >>= 4; h ^= h >> 17; h ^= h >> 9;
    unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
    for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
        AutoPointerSlot& item = g_autoPointerHash[slot];
        if (!item.building) return -1;
        if (item.building == building) return (int)item.entityIndex;
        slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoBuildPointerIndex(unsigned entityCount)
{
    memset(g_autoPointerHash, 0, sizeof(g_autoPointerHash));
    for (unsigned i = 0; i < entityCount; ++i) {
        uintptr_t h = (uintptr_t)g_autoEntities[i].building;
        h >>= 4; h ^= h >> 17; h ^= h >> 9;
        unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
            AutoPointerSlot& item = g_autoPointerHash[slot];
            if (!item.building) {
                item.building = g_autoEntities[i].building;
                item.entityIndex = i;
                inserted = true;
                break;
            }
            if (item.building == g_autoEntities[i].building) { inserted = true; break; }
            slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoMemoInsert(uint64_t officeKey, uint64_t targetKey)
{
    if (g_autoMemoCount >= AUTO_MEMO_MAX) return -1;
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        if (g_autoMemoHash[slot] == 0) {
            const unsigned index = g_autoMemoCount++;
            g_autoMemo[index].officeKey = officeKey;
            g_autoMemo[index].targetKey = targetKey;
            g_autoMemo[index].nextOffice = 0xFFFFFFFFu;
            g_autoMemo[index].state = 1;
            g_autoMemoHash[slot] = index + 1;
            return (int)index;
        }
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoAppendNativeTarget(void* office, void* target, unsigned directions)
{
    if (!g_exeBase || g_exeSize <= AUTO_RVA_TASKCFG_CTOR + 1 ||
        g_exeSize <= AUTO_RVA_VECTOR_PUSH_BACK + 1) return false;
    void** oldBegin = nullptr; size_t oldCount = 0;
    if (!AutoReadTargetVector(office, &oldBegin, &oldCount)) return false;
    (void)oldBegin; (void)oldCount;

    typedef void* (__fastcall *TTaskCfgCtor)(void*);
    typedef void (__fastcall *TVectorPushBack)(void*, const void*);
    TTaskCfgCtor ctor = (TTaskCfgCtor)(g_exeBase + AUTO_RVA_TASKCFG_CTOR);
    TVectorPushBack pushBack = (TVectorPushBack)(g_exeBase + AUTO_RVA_VECTOR_PUSH_BACK);
    if (!ReadablePtr((const void*)ctor, 1) || !ReadablePtr((const void*)pushBack, 1)) return false;

    void* entry = ::operator new(AUTO_TARGET_ENTRY_SIZE, std::nothrow);
    void* cfgMem = ::operator new(AUTO_TASKCFG_SIZE, std::nothrow);
    if (!entry || !cfgMem) {
        if (entry) ::operator delete(entry);
        if (cfgMem) ::operator delete(cfgMem);
        return false;
    }
    memset(entry, 0, AUTO_TARGET_ENTRY_SIZE);
    memset(cfgMem, 0, AUTO_TASKCFG_SIZE);
    void* cfg = nullptr;
    try { cfg = ctor(cfgMem); } catch (...) { cfg = nullptr; }
    if (!cfg) { ::operator delete(entry); ::operator delete(cfgMem); return false; }

    // The native UI adds entries OFF, awaiting its activation checkbox. An
    // autonomous company must request activation itself, not only add the row.
    // Native dirty/upkeep/planner paths still validate route compatibility.
    AutoApplyEntryActivation(entry, directions);
    *(void**)((BYTE*)entry + 0x08) = target;
    *(void**)((BYTE*)entry + 0x10) = cfg;
    // Ghidra reports no resource-filter field in this 0x18-byte TaskEntry.
    // The planner's target/connection index is rebuilt in its working area
    // by reachability; do not invent or write a filter at entry+0x0C.
    AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    *((BYTE*)cfg + 0x28) = 1;                    // native active flag
    *((BYTE*)cfg + 0xF0) = 1;                    // native validated flag

    try {
        // FUN_14008E050(vector, &entry) is the native vector<TaskEntry*>::push_back.
        pushBack((BYTE*)office + AUTO_DO_TARGETS_BEGIN, &entry);
    } catch (...) {
        ::operator delete(entry); ::operator delete(cfgMem);
        return false;
    }
    return true;
}

static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void*)
{
    if (event == TSM_SAVE_LOADING) PanelObserverLoading();
    // Persist permission keys only; never retain building pointers. Save/load
    // callbacks run on the file thread, so no game-memory access is performed.
    if (!g_persistence || !dir) return;
    if (event == TSM_SAVE_WRITING) {
        if (!g_persistence->writeAtomic) return;
        BYTE data[12 + AUTO_STATE_PERMISSION_MAX * 9];
        memcpy(data, "PLSTOR18", 8);
        uint32_t count = 0;
        EnterCriticalSection(&g_lock);
        count = g_statePermissionCount;
        memcpy(data + 8, &count, sizeof(count));
        size_t at = 12;
        for (unsigned i = 0; i < g_statePermissionCount; ++i) {
            memcpy(data + at, &g_statePermissions[i].key, sizeof(uint64_t));
            at += sizeof(uint64_t);
            data[at++] = g_statePermissions[i].mode;
        }
        LeaveCriticalSection(&g_lock);
        if (!g_persistence->writeAtomic(dir, AUTO_STATE_PREF_FILE, data, at))
            Logf("[AUTO DO] state permissions were not saved");
    } else if (event == TSM_SAVE_LOADING) {
        if (g_persistence->readFile) {
            size_t len = 0;
            void* raw = g_persistence->readFile(dir, AUTO_STATE_PREF_FILE, &len);
            AutoStatePermission loaded[AUTO_STATE_PERMISSION_MAX];
            unsigned loadedCount = 0;
            const bool legacy = raw && len >= 12 && memcmp(raw, "PLSTOR17", 8) == 0;
            if (raw && len >= 12 && (legacy || memcmp(raw, "PLSTOR18", 8) == 0)) {
                uint32_t count = 0;
                memcpy(&count, (BYTE*)raw + 8, sizeof(count));
                if (count <= AUTO_STATE_PERMISSION_MAX && len == 12 + (size_t)count * 9) {
                    size_t at = 12;
                    for (uint32_t i = 0; i < count; ++i) {
                        uint64_t key = 0;
                        memcpy(&key, (BYTE*)raw + at, sizeof(key));
                        at += sizeof(key);
                        const unsigned char mode = AutoMigratePermission(*((BYTE*)raw + at++), legacy);
                        if (key && mode != 0) {
                            loaded[loadedCount].key = key;
                            loaded[loadedCount].mode = mode;
                            ++loadedCount;
                        }
                    }
                } else {
                    Logf("[AUTO DO] state permission file has invalid length/count");
                }
            } else if (raw) {
                Logf("[AUTO DO] state permission file header invalid");
            }
            if (raw) free(raw);
            EnterCriticalSection(&g_lock);
            memset(g_statePermissions, 0, sizeof(g_statePermissions));
            if (loadedCount) memcpy(g_statePermissions, loaded,
                                    loadedCount * sizeof(loaded[0]));
            g_statePermissionCount = loadedCount;
            LeaveCriticalSection(&g_lock);
        }
        InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 1);
    }
}

static void AutoProbeStateInfrastructure(int closedDay, unsigned entityCount)
{
    g_autoStateStorageCount = 0;
    BYTE* game = g_exeBase ? TsmGameObject(g_exeBase) : nullptr;
    TsmBuildingSpan span;
    if (!game || !TsmGetBuildings(game, &span) ||
        !span.begin || !ReadablePtr(span.begin, span.count * sizeof(void*))) {
        Logf("[AUTO DO] day %d infra probe: world building vector unavailable", closedDay);
        return;
    }

    unsigned stateStorages = 0, privateStorages = 0, customsCount = 0;
    unsigned stateStoragesStored = 0, customsStored = 0;
    for (size_t i = 0; i < span.count; ++i) {
        void* b = span.begin[i];
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b)) continue;
        int type = -1;
        if (!AutoReadBuildingType(b, &type)) continue;
        // Discover storage-bearing sites for enforcement; classification, not
        // mere storage presence, determines allowed directions/visible widgets.
        if (AutoHasNativeStorage(b) && type != AUTO_CUSTOMS_TYPE &&
            type != AUTO_DO_ROAD_TYPE && type != AUTO_DO_RAIL_TYPE) {
            const bool isPrivate = g_enterprise && g_enterprise->isBuildingPrivate &&
                                   g_enterprise->isBuildingPrivate(b);
            if (isPrivate) ++privateStorages;
            else {
                ++stateStorages;
                if (stateStoragesStored < AUTO_STATE_STORAGE_MAX) {
                    AutoStateStorageSite& site = g_autoStateStorageSites[stateStoragesStored++];
                    site.building = b;
                    site.key = 0;
                    AutoBuildingKey(b, &site.key, nullptr);
                    site.permission = site.key ? (unsigned char)(AutoStatePermissionGet(site.key) & AutoBuildingCapabilities(b)) : 0;
                    site.x = 0.0f; site.z = 0.0f;
                    AutoBuildingPosition(b, &site.x, &site.z);
                    AutoBuildingName(b, site.name, sizeof(site.name));
                }
            }
            continue;
        }
        if (type != AUTO_CUSTOMS_TYPE) continue;
        ++customsCount;
        float x = 0.0f, z = 0.0f;
        if (!AutoBuildingPosition(b, &x, &z) || customsStored >= AUTO_CUSTOMS_MAX) continue;
        AutoCustomSite& site = g_autoCustomSites[customsStored++];
        site.building = b; site.x = x; site.z = z;
        AutoBuildingName(b, site.name, sizeof(site.name));
    }

    g_autoStateStorageCount = stateStoragesStored;
    Logf("[AUTO DO] day %d infra probe: world=%llu state_storage=%u private_storage=%u customs=%u customs_indexed=%u",
         closedDay, (unsigned long long)span.count, stateStorages, privateStorages,
         customsCount, customsStored);
    static int previousStateStorages = -1;
    static int previousCustoms = -1;
    if ((int)stateStorages != previousStateStorages || (int)customsCount != previousCustoms) {
        previousStateStorages = (int)stateStorages;
        previousCustoms = (int)customsCount;
        const unsigned show = (stateStoragesStored < 32) ? stateStoragesStored : 32;
        for (unsigned i = 0; i < show; ++i)
            Logf("[AUTO DO] state storage candidate name=%s key=%016llX pos=(%.1f,%.1f) permission=%s",
                 g_autoStateStorageSites[i].name,
                 (unsigned long long)g_autoStateStorageSites[i].key,
                 g_autoStateStorageSites[i].x, g_autoStateStorageSites[i].z,
                 g_autoStateStorageSites[i].permission == AUTO_ALLOW_BOTH ? "LOAD+UNLOAD" :
                 (g_autoStateStorageSites[i].permission == AUTO_ALLOW_LOAD ? "LOAD" :
                  (g_autoStateStorageSites[i].permission == AUTO_ALLOW_UNLOAD ? "UNLOAD" : "DENY")));
        if (stateStoragesStored > show)
            Logf("[AUTO DO] state storage list truncated: %u of %u shown",
                 show, stateStorages);
    }
    for (unsigned i = 0; i < entityCount; ++i) {
        AutoEntity& office = g_autoEntities[i];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        float ox = 0.0f, oz = 0.0f;
        if (!AutoBuildingPosition(office.building, &ox, &oz)) continue;
        int nearest = -1;
        double bestSq = 1.0e100;
        for (unsigned c = 0; c < customsStored; ++c) {
            const double dx = (double)g_autoCustomSites[c].x - ox;
            const double dz = (double)g_autoCustomSites[c].z - oz;
            const double d2 = dx * dx + dz * dz;
            if (d2 < bestSq) { bestSq = d2; nearest = (int)c; }
        }
        if (nearest >= 0)
            Logf("[AUTO DO] day %d nearest customs (geometric, not assigned) office=%016llX customs=%s dist=%.1fm",
                 closedDay, (unsigned long long)office.key,
                 g_autoCustomSites[nearest].name, sqrt(bestSq));
        else
            Logf("[AUTO DO] day %d nearest customs unavailable office=%016llX",
                 closedDay, (unsigned long long)office.key);
    }
}

static bool AutoTaskConfigValid(void* cfg)
{
    if (!cfg || !TsmIsValidGamePtr(cfg) || !ReadablePtr(cfg, AUTO_TASKCFG_SIZE)) return false;
    for (unsigned side = 0; side < 2; ++side) {
        const BYTE* p = (const BYTE*)cfg + (side ? 0xC8 : 0);
        const uintptr_t begin = *(const uintptr_t*)(p + 8);
        const uintptr_t end = *(const uintptr_t*)(p + 16);
        if ((!begin && end) || end < begin || (end - begin) % 16 != 0 ||
            (end - begin) / 16 > 4096) return false;
        if (end != begin && (!TsmIsValidGamePtr((void*)begin) ||
                            !ReadablePtr((void*)begin, end - begin))) return false;
    }
    return true;
}

static bool AutoSetTargetDirections(void* entry, unsigned directions)
{
    if (!entry || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) return false;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) {
        Logf("[AUTO DO] invalid task config; cannot enforce direction mask=%u", directions);
        return false;
    }
    const bool audit = g_autoLogLines < (unsigned)g_autoTargetLogLimit;
    AutoTaskAudit before = {};
    if (audit) before = AutoReadTaskAudit(entry, cfg);
    const bool directionsChanged = AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    const bool activationChanged = AutoApplyEntryActivation(entry, directions);
    const bool changed = directionsChanged || activationChanged;
    if (changed && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        Logf("[AUTO DO] task directions target=%p load=%u unload=%u entry_enabled=%u (native route validation still required)",
             *(void**)((BYTE*)entry + 0x08), (directions & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (directions & AUTO_ALLOW_UNLOAD) ? 1u : 0u, (unsigned)*(BYTE*)entry);
        ++g_autoLogLines;
    }
    if (changed && audit && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        const AutoTaskAudit after = AutoReadTaskAudit(entry, cfg);
        Logf("[AUTO DO AUDIT] target=%p changed_fields=0x%02X enabled=%u->%u load_all=%u->%u unload_all=%u->%u load_pct=%.9g->%.9g unload_pct=%.9g->%.9g load_bits=%08X->%08X unload_bits=%08X->%08X",
             *(void**)((BYTE*)entry + 8), AutoTaskAuditChanges(before, after),
             (unsigned)before.enabled, (unsigned)after.enabled,
             (unsigned)before.loadAll, (unsigned)after.loadAll,
             (unsigned)before.unloadAll, (unsigned)after.unloadAll,
             AutoAuditPercent(before.loadBits), AutoAuditPercent(after.loadBits),
             AutoAuditPercent(before.unloadBits), AutoAuditPercent(after.unloadBits),
             (unsigned)before.loadBits, (unsigned)after.loadBits,
             (unsigned)before.unloadBits, (unsigned)after.unloadBits);
        ++g_autoLogLines;
    }
    return changed;
}

static void AutoLogNativeTarget(void* entry, int type, unsigned requested, bool preserve)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) return; // enforcement path reports invalid configs
    const BYTE* p = (const BYTE*)cfg;
    const uintptr_t lb = *(const uintptr_t*)(p + 8), le = *(const uintptr_t*)(p + 16);
    const uintptr_t ub = *(const uintptr_t*)(p + 0xD0), ue = *(const uintptr_t*)(p + 0xD8);
    Logf("[AUTO DO] native target=%p type=%d policy=%s requested_mask=%u enabled=%u load_all=%u load_pct=%.3f load_selectors=%llu unload_all=%u unload_pct=%.3f unload_selectors=%llu",
         *(void**)((BYTE*)entry + 8), type, preserve ? "preserve-customs" : "managed",
         requested, (unsigned)*(BYTE*)entry, (unsigned)p[0], (double)*(const float*)(p + 4),
         (unsigned long long)((le - lb) / 16), (unsigned)p[0xC8], (double)*(const float*)(p + 0xCC),
         (unsigned long long)((ue - ub) / 16));
    ++g_autoLogLines;
}


// Called only from daily reconciliation; does not arm, clear or advance timers.
static void AutoLogOfficePlanState(void* office, uint64_t key, int day,
                                   const char* phase, bool notificationRequested)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    const BYTE* p = (const BYTE*)office;
    const bool dirtyReadable = ReadablePtr(p + AUTO_DO_DIRTY, 1);
    const bool timerReadable = ReadablePtr(p + AUTO_DO_UI_TIMER, sizeof(float));
    float timer = 0.0f;
    if (timerReadable) memcpy(&timer, p + AUTO_DO_UI_TIMER, sizeof(timer));
    Logf("[AUTO DO AUDIT] day=%d office=%p key=%016llX phase=%s notification_requested=%u dirty=%d timer_readable=%u countdown=%.9g",
         day, office, (unsigned long long)key, phase, notificationRequested ? 1u : 0u,
         dirtyReadable ? (int)p[AUTO_DO_DIRTY] : -1, timerReadable ? 1u : 0u, (double)timer);
    ++g_autoLogLines;
}

static void AutoTargetsOnDay(int day, int closedDay)
{
    if (!g_autoTargetsEnabled || !g_enterprise ||
        !g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex ||
        !g_enterprise->isBuildingPrivate) return;

    const bool afterLoad = (InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 0) != 0) ||
                           (g_autoLastDay >= 0 && day < g_autoLastDay);
    if (afterLoad)
        Logf("[AUTO DO] day %d save/load detected: native TaskEntry/TaskCfg were deserialized; reconciling generated targets after load", closedDay);
    g_autoLastDay = day;

    const size_t n = g_enterprise->getEnterpriseCount();
    if (n > AUTO_ENTITY_MAX) {
        Logf("[AUTO DO] day %d skipped: registered count %llu exceeds fixed discovery capacity %u",
             closedDay, (unsigned long long)n, (unsigned)AUTO_ENTITY_MAX);
        return;
    }

    unsigned entityCount = 0, officeCount = 0, candidateCount = 0;
    for (size_t i = 0; i < n; ++i) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b) ||
            !g_enterprise->isBuildingPrivate(b)) continue;
        uint64_t key = 0; int type = -1;
        if (!AutoBuildingKey(b, &key, &type)) continue;
        const bool isOffice = (type == AUTO_DO_ROAD_TYPE || type == AUTO_DO_RAIL_TYPE);
        const unsigned capabilities = isOffice ? 0 : AutoBuildingCapabilities(b);
        const bool hasStorage = isOffice || capabilities != 0;
        if (!hasStorage) continue;
        g_autoEntities[entityCount].building = b;
        g_autoEntities[entityCount].key = key;
        g_autoEntities[entityCount].type = type;
        g_autoEntities[entityCount].hasStorage = isOffice ? 0 : 1;
        g_autoEntities[entityCount].isStateStorage = 0;
        g_autoEntities[entityCount].statePermission = (unsigned char)capabilities;
        g_autoEntities[entityCount].targetPresent = 0;
        ++entityCount;
        if (isOffice) ++officeCount; else ++candidateCount;
    }

    AutoProbeStateInfrastructure(closedDay, entityCount);

    // Public access defaults deny. Keep denied sites indexed too: existing
    // native/manual entries must obey the same state permissions as new ones.
    for (unsigned si = 0; si < g_autoStateStorageCount && entityCount < AUTO_ENTITY_MAX; ++si) {
        AutoStateStorageSite& site = g_autoStateStorageSites[si];
        if (!site.building || !site.key) continue;
        AutoEntity& e = g_autoEntities[entityCount++];
        e.building = site.building; e.key = site.key; e.type = AUTO_STORAGE_BUILDING_TYPE;
        e.hasStorage = site.permission != 0;
        e.isStateStorage = 1; e.statePermission = site.permission; e.targetPresent = 0;
        if (site.permission != 0) ++candidateCount;
    }

    if (officeCount == 0) return;
    if (!AutoBuildPointerIndex(entityCount)) {
        Logf("[AUTO DO] day %d: pointer index capacity exhausted; scan aborted", closedDay);
        return;
    }
    if (!AutoBuildOfficeMemoIndex()) {
        Logf("[AUTO DO] day %d: office memo index capacity exhausted; scan aborted", closedDay);
        return;
    }

    unsigned planned = 0, added = 0, existing = 0, suppressed = 0, invalid = 0;
    g_autoLogLines = 0;
    for (unsigned oi = 0; oi < entityCount; ++oi) {
        AutoEntity& office = g_autoEntities[oi];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        void** entries = nullptr; size_t entryCount = 0;
        if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
            Logf("[AUTO DO] day %d invalid target vector; skipped office key=%016llX",
                 closedDay, (unsigned long long)office.key);
            ++invalid; continue;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-reconcile", false);
        const size_t entryCountAtStart = entryCount;
        for (unsigned ei = 0; ei < entityCount; ++ei) g_autoEntities[ei].targetPresent = 0;
        bool doConfigChanged = false;
        for (size_t ei = 0; ei < entryCount; ++ei) {
            void* entry = entries[ei];
            if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
            void* targetBuilding = *(void**)((BYTE*)entry + 0x08);
            if (!targetBuilding || !TsmIsValidGamePtr(targetBuilding) ||
                !ReadablePtr(targetBuilding, TSM_BLD_MIN_READABLE)) continue;
            int targetType = -1;
            if (AutoReadBuildingType(targetBuilding, &targetType) && AutoPreserveManualCustoms(targetType)) {
                // Border trade is not a state-enterprise permission. Preserve
                // the user's native customs task, including its resource lists
                // and activation. Customs auto-selection is not implemented.
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] preserving manual customs target=%p entry_enabled=%u",
                         targetBuilding, (unsigned)*(BYTE*)entry);
                    ++g_autoLogLines;
                }
                AutoLogNativeTarget(entry, targetType, 0, true);
                continue;
            }
            const int targetIndex = AutoPointerIndex(targetBuilding);
            if (targetIndex >= 0) g_autoEntities[targetIndex].targetPresent = 1;
            // Enforce all targets of this PRIVATE office, including manually
            // present public entries, unindexed/unknown roles and default OFF.
            const unsigned capabilities = AutoBuildingCapabilities(targetBuilding);
            const bool isPrivate = g_enterprise->isBuildingPrivate(targetBuilding) != 0;
            unsigned permissions = 0;
            if (!isPrivate) {
                uint64_t key = 0;
                permissions = AutoBuildingKey(targetBuilding, &key, nullptr) ?
                    AutoStatePermissionGet(key) : 0;
            }
            const unsigned directions = AutoEffectiveDirections(isPrivate, permissions, capabilities);
            if (!g_autoTargetsDryRun)
                doConfigChanged |= AutoSetTargetDirections(entry, directions);
            AutoLogNativeTarget(entry, targetType, directions, false);
        }

        // Native upkeep can disable an entry in place: keep deduplication for
        // entries still present. Missing generated entries are eligible for
        // recreation regardless of why they disappeared. A private company
        // owns its target list; manual removal is not a persistent veto.
        // State-building permissions remain a separate eligibility gate.
        for (int mi = AutoOfficeMemoHeadIndex(office.key); mi >= 0;
             mi = g_autoMemo[mi].nextOffice == 0xFFFFFFFFu ? -1 : (int)g_autoMemo[mi].nextOffice) {
            AutoMemo& memo = g_autoMemo[mi];
            if (memo.state != 1) continue;
            bool found = false;
            for (size_t ei = 0; ei < entryCount; ++ei) {
                void* entry = entries[ei];
                if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
                void* target = *(void**)((BYTE*)entry + 0x08);
                const int targetIndex = AutoPointerIndex(target);
                if (targetIndex >= 0 && g_autoEntities[targetIndex].key == memo.targetKey) {
                    found = true; break;
                }
            }
            if (!found) {
                memo.state = 3; // preserve hash chain; allow normal eligibility/re-add
                Logf("[AUTO DO] day %d target key=%016llX absent; eligible for automatic reconnection",
                     closedDay, (unsigned long long)memo.targetKey);
            }
        }

        unsigned doPlanned = 0, doAdded = 0, doExisting = 0, doSuppressed = 0;
        size_t projectedAdds = 0;
        // State storages were appended to g_autoEntities after private sites;
        // walk backward so an explicit state-source opt-in gets first access
        // to the five-entry conservative safe budget.
        for (unsigned ti = entityCount; ti > 0; --ti) {
            AutoEntity& target = g_autoEntities[ti - 1];
            if (!target.hasStorage || target.building == office.building) continue;
            if (target.targetPresent) {
                ++existing; ++doExisting; continue;
            }
            const int memoIndex = AutoMemoFind(office.key, target.key);
            if (entryCountAtStart + projectedAdds >= AUTO_DO_SAFE_TARGET_COUNT) {
                ++suppressed; ++doSuppressed;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d target=%016llX omitted: conservative target count %u reached (existing=%llu)",
                         closedDay, (unsigned long long)target.key,
                         (unsigned)AUTO_DO_SAFE_TARGET_COUNT,
                         (unsigned long long)entryCountAtStart);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++planned; ++doPlanned;
            if (g_autoTargetsDryRun) {
                ++projectedAdds;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d DRY RUN office=%016llX target=%016llX type=%d",
                         closedDay, (unsigned long long)office.key,
                         (unsigned long long)target.key, target.type);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (memoIndex < 0 && g_autoMemoCount >= AUTO_MEMO_MAX) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d memo capacity %u reached; target omitted",
                         closedDay, (unsigned)AUTO_MEMO_MAX);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (!AutoAppendNativeTarget(office.building, target.building, target.statePermission)) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d append failed office=%016llX target=%016llX",
                         closedDay, (unsigned long long)office.key, (unsigned long long)target.key);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++projectedAdds;
            if (memoIndex < 0) {
                if (AutoMemoInsert(office.key, target.key) < 0) {
                    ++invalid;
                    Logf("[AUTO DO] day %d memo hash insertion failed after native append", closedDay);
                }
            } else {
                g_autoMemo[memoIndex].state = 1;
            }
            target.targetPresent = 1;
            ++added; ++doAdded;
            entries = nullptr; entryCount = 0;
            if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
                ++invalid; break;
            }
        }

        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-notify", doAdded > 0 || doConfigChanged);
        if (doAdded > 0 || doConfigChanged) {
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_DIRTY, 1))
                *((BYTE*)office.building + AUTO_DO_DIRTY) = 1;
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_UI_TIMER, sizeof(float)))
                *(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "after-notify", doAdded > 0 || doConfigChanged);
        if (doPlanned || doExisting || doSuppressed || doAdded || doConfigChanged)
            Logf("[AUTO DO] day %d office=%016llX type=%d existing=%u planned=%u added=%u config_changed=%u suppressed=%u",
                 closedDay, (unsigned long long)office.key, office.type, doExisting,
                 doPlanned, doAdded, doConfigChanged ? 1u : 0u, doSuppressed);
    }

    Logf("[AUTO DO] day %d summary: private offices=%u candidates=%u planned=%u added=%u existing=%u suppressed=%u invalid=%u mode=%s",
         closedDay, officeCount, candidateCount, planned, added, existing, suppressed, invalid,
         g_autoTargetsDryRun ? "DRY RUN" : "WRITE");
}

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_logistics.ini";
    char v[256];

    g_enabled        = H->configInt(ini, "private_logistics", "enabled", 1);
    g_billing        = H->configInt(ini, "private_logistics", "billing", 0);
    g_panelObserverEnabled = H->configInt(ini, "private_logistics", "panel_fuel_observer", 1);
    g_dryRun         = H->configInt(ini, "private_logistics", "dry_run", 1);
    g_payFactory     = H->configInt(ini, "private_logistics", "pay_factory", 1);
    g_payCarrier     = H->configInt(ini, "private_logistics", "pay_carrier", 1);
    g_logCapPerDay   = H->configInt(ini, "private_logistics", "log_cap_per_day", 400);
    g_ledgerCapacity = H->configInt(ini, "private_logistics", "ledger_capacity", 512);
    g_siteLog        = H->configInt(ini, "private_logistics", "site_log", 1);
    g_autoTargetsEnabled = H->configInt(ini, "private_logistics", "auto_targets_enabled", 1);
    g_autoTargetsDryRun  = H->configInt(ini, "private_logistics", "auto_targets_dry_run", 0);
    g_autoTargetLogLimit = H->configInt(ini, "private_logistics", "auto_targets_log_limit", 80);
    g_autoSourcePercent = AutoClampSourcePercent(H->configInt(ini, "private_logistics", "auto_source_percent", 10));
    g_autoDestinationPercent = AutoClampDestinationPercent(H->configInt(ini, "private_logistics", "auto_destination_percent", 30));
    Logf("[AUTO DO] company thresholds: load above %d%% / deliver below %d%% (managed tasks, reconciled daily)",
         g_autoSourcePercent, g_autoDestinationPercent);
    if (g_autoSourcePercent != 10)
        Logf("[AUTO DO] source threshold override=%d%% (this test ships 10%%); an explicit deployed INI setting overrides the fallback", g_autoSourcePercent);
    Logf("[AUTO DO] permission-test profile: billing=%d auto_targets=%d apply=%d (expected 0/1/1)",
         g_billing, g_autoTargetsEnabled, !g_autoTargetsDryRun);
    if (g_billing || !g_autoTargetsEnabled || g_autoTargetsDryRun)
        Logf("[AUTO DO] WARNING deployed INI overrides the permission-test defaults; rebuild/deploy the shipped INI for this test");
    if (H->configInt(ini, "private_logistics", "state_storage_always_on", 0))
        Logf("[AUTO DO] state_storage_always_on is obsolete and ignored; per-building permissions are required");
    if (g_autoTargetLogLimit < 0) g_autoTargetLogLimit = 0;
    if (g_autoTargetLogLimit > 1000) g_autoTargetLogLimit = 1000;

    if (g_logCapPerDay < 0)            g_logCapPerDay = 0;
    if (g_logCapPerDay > 5000)         g_logCapPerDay = 5000;
    if (g_ledgerCapacity < 1)          g_ledgerCapacity = 1;
    if (g_ledgerCapacity > LEDGER_MAX) g_ledgerCapacity = LEDGER_MAX;

    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), ""))
        g_freightShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "factory_share", v, sizeof(v), ""))
        g_factoryShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "min_payment", v, sizeof(v), ""))
        g_minPayment = (float)atof(v);
    if (H->configString(ini, "private_logistics", "daily_pay_cap", v, sizeof(v), ""))
        g_dailyPayCap = atof(v);

    // v0.5: the tariff model. Every number here is a physical quantity or a
    // ratio - litres, tonnes per litre, a percentage. Not one of them is a
    // price in currency units, which is what makes the model survive a
    // currency event.
    g_tariffProbe     = H->configInt(ini, "private_logistics", "tariff_probe", 1);
    g_balanceLog      = H->configInt(ini, "private_logistics", "balance_log", 1);
    g_tariffLogPerDay = H->configInt(ini, "private_logistics", "tariff_log_per_day", 60);

    if (H->configString(ini, "private_logistics", "litres_per_trip", v, sizeof(v), ""))
        gLitresPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "litres_per_tonne", v, sizeof(v), ""))
        gLitresPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "tariff_margin", v, sizeof(v), ""))
        gTariffMargin = atof(v);
    if (H->configString(ini, "private_logistics", "diesel_density", v, sizeof(v), ""))
        gDieselDensity = atof(v);
    g_tripTariff = H->configInt(ini, "private_logistics", "trip_tariff", 1);
    g_censusLog  = H->configInt(ini, "private_logistics", "census_log", 1);
    g_vehScan    = H->configInt(ini, "private_logistics", "veh_scan", 1);
    g_vehScanLogPerDay = H->configInt(ini, "private_logistics", "veh_scan_log_per_day", 40);
    // v0.11: raw cargo-vector dump, up to three vehicles per close.
    g_cargoDump  = H->configInt(ini, "private_logistics", "cargo_dump", 1);
    if (g_vehScanLogPerDay < 0)   g_vehScanLogPerDay = 0;
    if (g_vehScanLogPerDay > 512) g_vehScanLogPerDay = 512;
    g_litresAuto = H->configInt(ini, "private_logistics", "litres_auto", 0);
    if (H->configString(ini, "private_logistics", "wear_litres", v, sizeof(v), ""))
        gWearLitres = atof(v);

    // v0.7: freight_share is RETIRED. Honouring it silently would make every
    // delivery look mispriced against a tariff that no longer reads it, so a
    // key left in the ini is reported and zeroed rather than obeyed.
    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), "") && v[0]) {
        const float f = (float)atof(v);
        g_freightShareRetired = (f > 0.0f) ? f : 0.0f;
        g_freightShare = 0.0f;
    }
    if (H->configString(ini, "private_logistics", "baskets_per_trip", v, sizeof(v), ""))
        gBasketsPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "baskets_per_tonne", v, sizeof(v), ""))
        gBasketsPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "fuel_resource", v, sizeof(v), "") && v[0])
        strncpy_s(gFuelResource, sizeof(gFuelResource), v, _TRUNCATE);

    // v0.4: a share is a fraction of cargo value. 0 is legal (freight off);
    // more than half would make haulage cost more than the goods.
    if (g_freightShare < 0.0f)   g_freightShare = 0.0f;
    if (g_freightShare > 0.50f)  g_freightShare = 0.50f;
    if (g_tripTariff != 0)       g_tripTariff   = 1;
    if (gWearLitres  < 0.0)      gWearLitres    = 0.0;
    if (gWearLitres  > 500.0)    gWearLitres    = 500.0;
    if (g_litresAuto != 1)       g_litresAuto   = 0;
    if (g_factoryShare  < 0.0f) g_factoryShare  = 0.0f;
    if (g_factoryShare  > 5.0f) g_factoryShare  = 5.0f;
    if (g_minPayment    < 0.0f) g_minPayment    = 0.0f;
    if (g_dailyPayCap   < 0.0)  g_dailyPayCap   = 0.0;

    // v0.5 clamps. Density is the one hard range: a value outside it is a
    // typo, and a typo here would misprice every delivery by orders of
    // magnitude, so it falls back to diesel rather than being honoured.
    if (g_tariffLogPerDay < 0)    g_tariffLogPerDay = 0;
    if (g_tariffLogPerDay > 5000) g_tariffLogPerDay = 5000;
    if (gLitresPerTrip  < 0.0)   gLitresPerTrip  = 0.0;
    if (gLitresPerTonne < 0.0)   gLitresPerTonne = 0.0;
    if (gLitresPerTrip  > 500.0) gLitresPerTrip  = 500.0;
    if (gLitresPerTonne > 50.0)  gLitresPerTonne = 50.0;
    if (gTariffMargin   < 0.0)   gTariffMargin   = 0.0;
    if (gTariffMargin   > 5.0)   gTariffMargin   = 5.0;
    if (!(gDieselDensity > 0.0001) || gDieselDensity > 0.01)
        gDieselDensity = 0.00084;
    if (gBasketsPerTrip  < 0.0)  gBasketsPerTrip  = 0.0;
    if (gBasketsPerTonne < 0.0)  gBasketsPerTonne = 0.0;
    if (gBasketsPerTrip  > 1000.0) gBasketsPerTrip = 1000.0;
    if (gBasketsPerTonne > 100.0)  gBasketsPerTonne = 100.0;

    // site_list, e.g. "0x1CB7FF" or "1CB7FF,68F792". Absent or empty = all.
    if (H->configString(ini, "private_logistics", "site_list", v, sizeof(v), "") && v[0]) {
        bool hit[MAX_SITES] = { false };
        int  matched = 0;

        for (char* p = v; *p; ) {
            while (*p == ' ' || *p == '\t' || *p == ',') p++;
            if (!*p) break;

            char* end = p;
            while (*end && *end != ',' && *end != ' ' && *end != '\t') end++;

            const char save = *end;
            *end = 0;
            const unsigned want = (unsigned)strtoul(p, nullptr, 16);
            *end = save;

            for (int i = 0; i < g_siteCount; i++) {
                if (g_sites[i].rva == want) { hit[i] = true; matched++; break; }
            }
            p = end;
        }

        if (matched > 0) {
            for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = hit[i];
            g_siteFilterActive = true;
        } else {
            Logf("private_logistics  WARN  site_list matched no known site - patching all");
        }
    }
}

// ---------------------------------------------------------------------------
// v0.8: the vehicle census
// ---------------------------------------------------------------------------
//
// Read-only, once a day, on the day callback. Every pointer is validated
// before it is followed, every vector length is capped, and nothing here can
// fail a delivery: a bad read reports "unreadable" and moves on.
//
// The numbers this produces are the ones the final model needs and no formula
// can supply:
//   - how many vehicles a company actually owns
//   - what each one carries at its peak (its capacity, observed not assumed)
//   - which catalogue entry the game attaches to it (+0x510, unproven)

static int VehSlot(void* v)
{
    for (int i = 0; i < g_vehCount; i++)
        if (g_vehPtr[i] == v) return i;
    if (g_vehCount >= VEH_MAX) return -1;
    g_vehPtr[g_vehCount]     = v;
    g_vehPeakTons[g_vehCount] = 0.0;
    g_vehSamples[g_vehCount]  = 0;
    return g_vehCount++;
}

// veh+0x3B8 is a vector of cargo entries, stride 0x10: { Resource* , float tons }.
// Returns the total carried and the number of non-empty slots.
static double CargoTons(void* veh, int* slots)
{
    *slots = 0;
    const BYTE* p = (const BYTE*)veh;
    if (!ReadablePtr(p + 0x3B8, 16)) return 0.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end <= beg) return 0.0;

    // v0.12: the cap was 8, and over 8 it returned 0 SILENTLY - which is how
    // this census came to report "0 slot(s)" on trucks that demonstrably held
    // fuel. A storage vector with a pre-allocated slot per resource type is
    // perfectly plausible, and it would be longer than eight. Raise the cap and
    // stop hiding the case.
    size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return 0.0;

    double t = 0.0;
    for (size_t i = 0; i < n; i++) {
        const float tons = *(const float*)(beg + i * 16 + 8);
        if (tons == tons && tons > 0.0f && tons < 1.0e6f) {
            t += (double)tons;
            (*slots)++;
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// v0.13: WHERE THE FUEL ACTUALLY IS
// ---------------------------------------------------------------------------
//
// The follow-up Ghidra report settled it, and it corrected two of my readings.
//
//   AUTHORITATIVE: the cargo vector at veh+0x3B8, stride 0x10, the slot whose
//                  +0x00 Resource* equals the diesel resource. +0x08 is the
//                  level in tonnes and the engine clamps it at zero.
//   MIRROR:        veh+0x5F0. Written by the refuel function (140142430) as
//                  min(available, capacity - current) + current, and by the
//                  drive function (14014B920) as (rate + current). NOT clamped,
//                  which is why it went NEGATIVE eight times in one run. That
//                  was never a mystery - it was a mirror with no floor.
//
// So the tank we read from now on is the slot, and +0x5F0 is kept only as a
// cross-check that gets printed beside it. If the two ever disagree by more
// than a few kilos, the log says so and we stop trusting the measurement.
static void* FuelResourcePtr(void)
{
    if (!g_pricing || !g_pricing->getByName) return nullptr;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    return g_fuelRes;
}

// Returns the fuel slot's tonnage. *found = 1 when the slot exists.
// -1 when the vector cannot be read at all.
static double FuelSlotTons(void* v, int* found)
{
    *found = 0;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x3B8, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) return -1.0;
    if (n == 0) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    void* fuel = FuelResourcePtr();
    if (!fuel) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* e = beg + i * 16;
        if (*(const void* const*)e != fuel) continue;
        float t = *(const float*)(e + 8);
        if (!(t == t) || t < -1000.0f || t > 1000.0f) return -1.0;
        if (t < 0.0f) t = 0.0f;
        *found = 1;
        return (double)t;
    }
    return 0.0;
}

// The mirror at +0x5F0, and the per-vehicle capacity the refuel code clamps to
// (*(veh+0x1708) + 0x8684). Both are evidence, not the measurement.
static double MirrorTons(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_FUEL_OFF, 4)) return -1.0;
    float f = *(const float*)(p + VEH_FUEL_OFF);
    if (!(f == f) || f < -1000.0f || f > 1000.0f) return -1.0;
    return (double)f;
}

static double TankCapacity(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x1708, 8)) return -1.0;
    const BYTE* meta = *(const BYTE* const*)(p + 0x1708);
    if (!meta || !TsmIsValidGamePtr(meta)) return -1.0;
    if (!ReadablePtr(meta + 0x8684, 4)) return -1.0;
    const float c = *(const float*)(meta + 0x8684);
    if (!(c == c) || c <= 0.0f || c > 1000.0f) return -1.0;
    return (double)c;
}

// v0.11: sum of one building's fleet tanks, in tonnes. -1 = not readable.
static double FleetFuelTons(void* b, int* outCount)
{
    *outCount = 0;
    if (!b || !ReadablePtr((const BYTE*)b + 0xC70, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
    const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 8;
    if (n == 0 || n > VEH_MAX) return -1.0;
    if (!ReadablePtr(beg, n * 8)) return -1.0;

    double t = 0.0;
    int c = 0;
    for (size_t i = 0; i < n; i++) {
        void* v = *(void* const*)(beg + i * 8);
        if (!v) continue;
        int found = 0;
        double lvl = FuelSlotTons(v, &found);
        if (!found) lvl = MirrorTons(v);         // fall back to +0x5F0
        if (lvl < 0.0) continue;
        t += lvl;
        c++;
    }
    *outCount = c;
    return (c > 0) ? t : -1.0;
}

// One sample per committed delivery. Cheap: one vector walk and a few floats.
static void BurnProbe(void* b)
{
    int n = 0;
    const double now = FleetFuelTons(b, &n);
    if (now < 0.0) return;

    if (g_probeBld != b || g_probeFuelTons < 0.0 || n != g_probeVehCount) {
        g_probeBld      = b;                     // first sample, or a new fleet
        g_probeFuelTons = now;
        g_probeVehCount = n;
        return;
    }

    const double d = g_probeFuelTons - now;      // positive = burned
    if (d < -0.02 || d > 0.5) {
        g_probeSkipRun++;                        // refill, or the fleet changed size
    } else {
        g_probeBurnRun  += d;
        g_probeStepsRun++;
    }
    g_probeFuelTons = now;
}

// v0.12: RAW storage-vector dump. Two vectors, every slot, nothing filtered.
//
// Why this exists: the vehicle appears to keep its fuel in a
// {Resource*, float tons} slot, the same shape the transfer function moves
// goods with. If so, one of the vectors below holds diesel and the slot will
// track the +0x5F0 reading day after day.
//
// Two things went wrong in v0.10 and are fixed here:
//   - the census capped the vector at eight slots and returned "0 slot(s)"
//     for anything longer, which is why a truck holding 0.29 t of fuel could
//     report an empty hold. That was OUR bug, not evidence about the game.
//   - only the 0x3B8 vector was read. The trailer vector at 0x3A0 was never
//     looked at, and fuel may live there.
//
// A slot whose tonnage falls every day and jumps at a refill is the tank.
static void DumpVector(void* v, int off, const char* label, int closedDay, int slotIdx)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + off, 16)) return;

    const BYTE* beg = *(const BYTE* const*)(p + off);
    const BYTE* end = *(const BYTE* const*)(p + off + 8);
    if (!beg || !end || end < beg) {
        Logf("[CARGO] day %d s=%d %s unreadable beg=%p end=%p",
             closedDay, slotIdx, label, beg, end);
        return;
    }

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) {
        Logf("[CARGO] day %d s=%d %s n=%llu (implausible)",
             closedDay, slotIdx, label, (unsigned long long)n);
        return;
    }
    if (n == 0) {
        Logf("[CARGO] day %d s=%d %s n=0", closedDay, slotIdx, label);
        return;
    }
    if (!ReadablePtr(beg, n * 16)) return;

    Logf("[CARGO] day %d s=%d %s n=%llu", closedDay, slotIdx, label,
         (unsigned long long)n);
    const size_t show = (n < 16) ? n : 16;
    for (size_t i = 0; i < show; i++) {
        const BYTE* e   = beg + i * 16;
        const void* res = *(const void* const*)e;
        const float t   = *(const float*)(e + 8);
        if (!(t > 0.0f) && !(t < 0.0f)) continue;      // skip empty slots
        Logf("[CARGO] day %d s=%d %s   %llu res=%p %.4f t", closedDay, slotIdx,
             label, (unsigned long long)i, res, (double)t);
    }
}

static void CargoDump(int slot, void* v, int closedDay)
{
    if (!g_cargoDump || slot < 0 || slot > 2) return;   // first three vehicles
    DumpVector(v, 0x3A0, "trailer", closedDay, slot);
    DumpVector(v, 0x3B8, "cargo",   closedDay, slot);
}

// ---------------------------------------------------------------------------
// v0.9: the field scanner
// ---------------------------------------------------------------------------
//
// Compare the vehicle's first 0x800 bytes against yesterday's copy and report
// the words that moved. The baseline is taken silently on first sight so that
// day one does not dump a whole struct.
//
// Known pointer/vector ranges are skipped: they are noise here and the values
// would only ever read as "some address".
//
//   +0x3A0..0x3C8  trailer vector, cargo vector
//   +0x4F0..0x520  current building, home building, the +0x510 integer
//   +0x610..0x620  line stops
static bool VehScanSkip(int off)
{
    return (off >= 0x3A0 && off < 0x3C8) ||
           (off >= 0x4F0 && off < 0x520) ||
           (off >= 0x610 && off < 0x620);
}

// ---------------------------------------------------------------------------
// v0.10: read the tank
// ---------------------------------------------------------------------------
//
// veh+0x5F0 is fuel in tonnes (see the evidence at the globals). This turns it
// into the number the whole tariff has been missing: litres actually BURNED
// per delivery, with no purchases and no tank-fill error.
//
// A refill shows up as an increase and is counted, not charged as negative
// burn. A vehicle seen for the first time only sets a baseline.
static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
{
    if (slot < 0 || slot >= VEH_MAX) return;

    // v0.13: the SLOT is the tank. +0x5F0 is the unclamped mirror, kept as a
    // cross-check - it is the thing that went negative, and now we can show why.
    int found = 0;
    double level = FuelSlotTons(v, &found);
    const double mirror = MirrorTons(v);

    if (!found) {
        if (level < 0.0 && mirror < 0.0) return;
        level = (mirror > 0.0) ? mirror : 0.0;   // no fuel slot: fall back
        if (!g_vehNoSlotWarned[slot]) {
            g_vehNoSlotWarned[slot] = 1;
            Logf("[TANK] day %d   v=%p has NO fuel slot in the cargo vector "
                 "-> falling back to +0x5F0 (%.4f t)", closedDay, v, mirror);
        }
    }

    if (!g_vehFuelValid[slot]) {
        g_vehFuel[slot] = level;
        g_vehFuelValid[slot] = true;
        return;
    }

    const double prev = g_vehFuel[slot];
    if (level > prev + 0.02) {              // a refill, not burn
        if (refills) (*refills)++;
    } else if (prev > level) {
        if (burned) *burned += (prev - level);
    }
    g_vehFuel[slot] = level;

    // v0.13: one line per vehicle per day saying WHICH source was read and how
    // far the mirror has drifted from it. If these two ever part company by
    // more than a few kilos, the measurement is suspect and this says so.
    if (g_tankCrossLog > 0) {
        const double drift = (mirror >= 0.0) ? (mirror - level) : 0.0;
        Logf("[TANK] day %d   v=%p slot %.4f t | mirror +0x5F0 %.4f t | drift %+.4f t | %s",
             closedDay, v, level, mirror, drift,
             (fabs(drift) > 0.01) ? "MIRROR HAS DRIFTED" : "agree");
        g_tankCrossLog--;
    }
}

static void ScanVehicle(int slot, void* v, int closedDay)
{
    if (!g_vehScan || slot < 0 || slot >= VEH_MAX) return;
    if (!ReadablePtr((const BYTE*)v, VEH_SCAN_WORDS * 4)) return;

    const unsigned* src = (const unsigned*)v;

    if (!g_vehSnapValid[slot]) {
        for (int i = 0; i < VEH_SCAN_WORDS; i++) g_vehSnap[slot][i] = src[i];
        g_vehSnapValid[slot] = true;
        return;                                   // baseline, not a delta
    }

    int logged = 0;
    int changed = 0;
    for (int i = 0; i < VEH_SCAN_WORDS; i++) {
        const int off = i * 4;
        if (g_vehSnap[slot][i] == src[i]) continue;
        changed++;
        if (VehScanSkip(off)) { g_vehSnap[slot][i] = src[i]; continue; }
        if (logged < g_vehScanLogPerDay) {
            const unsigned a = g_vehSnap[slot][i];
            const unsigned b = src[i];
            float fa, fb;
            memcpy(&fa, &a, 4);
            memcpy(&fb, &b, 4);
            Logf("[VEH CHG] day %d v=%p +0x%03X %08X -> %08X (f %.4f -> %.4f)",
                 closedDay, v, off, a, b, (double)fa, (double)fb);
            logged++;
        }
        g_vehSnap[slot][i] = src[i];
    }
    if (changed > logged)
        Logf("[VEH CHG] day %d v=%p %d more word(s) changed, not logged",
             closedDay, v, changed - logged);
}

static void CensusFleets(int closedDay)
{
    if (!g_censusLog || !g_enterprise) return;
    if (!g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex) return;

    size_t n = g_enterprise->getEnterpriseCount();
    if (n == 0) return;
    if (n > 32) n = 32;                          // a republic, not a spreadsheet

    // v0.11: one close counted once, not once per building. L per fleet-day is
    // the rate the physics has; it must not double-count a fleet seen from two
    // enterprises.
    const unsigned vehDaysBefore = g_tankVehDaysRun;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr((const BYTE*)b + 0xC70, 16)) continue;

        const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
        const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
        if (!beg || !end || end < beg) continue;

        size_t cnt = (size_t)(end - beg) / sizeof(void*);
        if (cnt > 64) cnt = 64;
        if (!ReadablePtr(beg, cnt * sizeof(void*))) continue;

        char bname[64];
        BuildingName(b, bname, sizeof(bname));

        if (cnt == 0) {
            Logf("[VEH] day %d %s fleet: 0 vehicle(s)", closedDay, bname);
            continue;
        }
        Logf("[VEH] day %d %s fleet: %u vehicle(s)", closedDay, bname, (unsigned)cnt);

        for (size_t k = 0; k < cnt; k++) {
            void* v = ((void* const*)beg)[k];
            if (!v || !TsmIsValidGamePtr(v)) continue;

            void* home = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x4F8, 8))
                home = *(void* const*)((const BYTE*)v + 0x4F8);

            void* cat = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x510, 8))
                cat = *(void* const*)((const BYTE*)v + 0x510);

            // v0.12: the Ghidra report says the vehicle KIND is an int at
            // +0x50C, and that 0x42 marks a fuel consumer. +0x510, which this
            // census has been printing as a pointer, is the next field along -
            // which is why it kept printing nonsense like FFFFFFFF. If every
            // truck reads 66 here, that claim is confirmed; if none does, the
            // report has the wrong offset and we say so.
            int kind = -1;
            if (ReadablePtr((const BYTE*)v + 0x50C, 4))
                kind = *(const int*)((const BYTE*)v + 0x50C);

            int slots = 0;
            const double tons = CargoTons(v, &slots);
            CargoDump(VehSlot(v), v, closedDay);

            const int s = VehSlot(v);
            const bool first = (s >= 0 && g_vehSamples[s] == 0);
            if (s >= 0) {
                g_vehSamples[s]++;
                if (tons > g_vehPeakTons[s]) g_vehPeakTons[s] = tons;
            }

            // v0.13: the state gate the report found. 1 and 2 = working;
            // anything else and the engine does not even look at the truck.
            int state = -1, mode = -1;
            if (ReadablePtr((const BYTE*)v + 0x448, 4))
                state = *(const int*)((const BYTE*)v + 0x448);
            if (ReadablePtr((const BYTE*)v + 0x440, 4))
                mode  = *(const int*)((const BYTE*)v + 0x440);
            const bool active = (state == 1 || state == 2);
            if (active) g_activeVehDaysRun++; else g_idleVehDaysRun++;

            // Per-vehicle tank capacity, from the field the REFUEL code clamps
            // to. This is the "never assume an arbitrary vehicle" requirement
            // answered with a number the game itself uses.
            const double cap = TankCapacity(v);
            if (cap > 0.0 && s >= 0 && !g_vehCapValid[s]) {
                g_vehCapValid[s] = 1;
                g_vehCap[s] = cap;
                Logf("[VEH] day %d   v=%p tank capacity %.4f t (%.0f L at 1 kg/L)",
                     closedDay, v, cap, cap * 1000.0);
            }

            Logf("[VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) kind=%d cat=%p peak %.3ft "
                 "state=%d mode=%d %s",
                 closedDay, v, (home == b) ? "this" : "other",
                 tons, slots, kind, cat, (s >= 0) ? g_vehPeakTons[s] : 0.0,
                 state, mode, active ? "ACTIVE" : "idle");

            // One dump per vehicle, ever. +0x510 is the lead for capacity,
            // speed, price and consumption; this tells us what it actually is
            // before anything is built on it.
            // v0.10: fuel on board, and what left the tank since yesterday.
            double burned = 0.0;
            unsigned refills = 0;
            ReadTank(s, v, closedDay, &burned, &refills);
            g_burnTonsRun += burned;
            g_refillsRun  += refills;
            if (g_vehFuelValid[s]) g_tankVehDaysRun++;
            if (burned > 0.0 || refills > 0)
                Logf("[TANK] day %d   v=%p fuel %.4f t | burned %.4f t | refills %u",
                     closedDay, v, g_vehFuelValid[s] ? g_vehFuel[s] : 0.0,
                     burned, refills);

            ScanVehicle(s, v, closedDay);

            if (first && cat && TsmIsValidGamePtr(cat) && ReadablePtr(cat, 32)) {
                const unsigned* d = (const unsigned*)cat;
                Logf("[VEH] day %d   cat[+0x00] %08X %08X %08X %08X | %08X %08X %08X %08X",
                     closedDay, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
            }
        }
    }

    // v0.11: this close saw at least one tank -> it counts as a fleet-day.
    if (g_tankVehDaysRun > vehDaysBefore) g_tankDaysRun++;
}

// ---------------------------------------------------------------------------
// entry points
// ---------------------------------------------------------------------------

#include "panel_fuel_runtime.h"

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_logistics";
    info->version = PL_VERSION;

    ReadSettings();
    if (!g_enabled) {
        Logf("private_logistics  disabled in the ini");
        return 1;
    }

    g_realTransfer = (tTransfer)(g_exeBase + TSM_RVA_TRANSFER);

    Logf("private_logistics  v%s - billing %s",
         PL_VERSION,
         g_dryRun ? "ARMED IN DRY RUN (computes every payment, moves nothing)"
                  : "LIVE (moves money)");
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_exeBase || g_exeSize == 0) {
        Logf("private_logistics  no executable base - refusing to start");
        return 1;
    }

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK,
                                                   TSM_SIM_CLOCK_VERSION);
    if (!g_simClock)
        Logf("private_logistics  WARN  sim_clock is not installed - "
             "the daily summary will not be printed");

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE,
                                                           TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        Logf("private_logistics  WARN  enterprise_base is not installed - "
             "no building can be identified as private and nothing will be paid");

    if (g_autoTargetsEnabled) {
        g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                       TSM_UI_PANELS_VERSION);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V5);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V4);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V3);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V2);
        if (g_uiPanels && TsmUiPanelsHasWidgets(g_uiPanels) &&
            g_uiPanels->registerWidgetProvider)
            g_stateStorageWidgetHandle =
                g_uiPanels->registerWidgetProvider(StateStorageWidgetProvider, nullptr);
        if (!g_stateStorageWidgetHandle)
            Logf("private_logistics  [AUTO DO] WARN state-storage checkbox unavailable; "
                 "ui_panels v2 widgets are required");
    }

    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                   TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                       TSM_CURRENCY_VERSION_V3);
    if (!g_currency)
        Logf("private_logistics  WARN  currency is not installed - "
             "the treasury leg of every payment is unavailable");

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                         TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                             TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing)
        Logf("private_logistics  WARN  resource_pricing is not installed - "
             "factory payments fall back to the raw Res+0x58 field, which read "
             "0.00 for fabric and plants in two sessions");

    if (g_billing && (!g_enterprise || !g_currency)) {
        g_billing = 0;
        Logf("private_logistics  billing DISABLED: it needs both enterprise_base "
             "and currency. Instrumentation continues normally.");
    }

    if (g_autoTargetsEnabled &&
        (!g_simClock || !g_enterprise || !g_enterprise->getEnterpriseCount ||
         !g_enterprise->getEnterpriseByIndex || !g_enterprise->isBuildingPrivate)) {
        g_autoTargetsEnabled = 0;
        Logf("private_logistics  [AUTO DO] DISABLED: requires sim_clock and "
             "enterprise_base discovery/private checks");
    }

    if (g_autoTargetsEnabled) {
        g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE,
                                                              TSM_PERSISTENCE_VERSION);
        if (g_persistence &&
            g_persistence->structSize >= offsetof(TsmPersistenceApi, registerHandler) +
                                         sizeof(g_persistence->registerHandler) &&
            g_persistence->registerHandler)
            g_persistenceHandle = g_persistence->registerHandler("private_logistics",
                                                                  AutoPersistenceEvent, nullptr);
        if (!g_persistenceHandle)
            Logf("private_logistics  [AUTO DO] WARN persistence load signal unavailable; "
                 "reconciliation after load may wait for date rewind or restart");
    }

    PanelObserverStart();

    PatchAllSites();

    if (g_simClock) g_simClock->registerDay(OnDay, nullptr);

    Logf("private_logistics  live v%s: billing=%d dry_run=%d factory=%d carrier=%d, "
         "%d sites, cap %.0f/day, price = %s, factory share %.2f",
         PL_VERSION, g_billing, g_dryRun, g_payFactory, g_payCarrier,
         g_siteCount, (double)g_dailyPayCap,
         g_tripTariff ? "TRIP TARIFF (fuel x margin + wear)"
                      : "value share (v0.6 path, for comparison)",
         (double)g_factoryShare);

    Logf("private_logistics  [AUTO DO] enabled=%d dry_run=%d cadence=once/day; "
         "private endpoints -> private road/rail DOs; existing entries preserved",
         g_autoTargetsEnabled, g_autoTargetsDryRun);

    if (g_freightShareRetired > 0.0f)
        Logf("private_logistics  WARN freight_share = %.3f is RETIRED in v0.7 and was "
             "ignored - the trip tariff prices the delivery now. Remove the key.",
             (double)g_freightShareRetired);

    // v0.5: name both indices before a single delivery is priced. A probe that
    // silently cannot resolve a price is worse than no probe, so the log says
    // which index is live and what it reads.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);

        Logf("private_logistics  v%s tariff: %.2f L/trip + %.2f L/t, wear %.2f L, "
             "margin %.0f%%, density %.5f t/L, index '%s', auto-tune %s",
             PL_VERSION, gLitresPerTrip, gLitresPerTonne, gWearLitres,
             gTariffMargin * 100.0, gDieselDensity, gFuelResource,
             g_litresAuto ? "on" : "off");

        // The world is not loaded yet at plugin start, so both of these read
        // zero here and are re-read at every day close. Say that rather than
        // printing a scary NOT FOUND that resolves itself a second later.
        Logf("private_logistics  [IDX] at plugin start: fuel '%s' = %.2f, cpi basket %.2f - "
             "the world is still loading; both are re-read at every day close",
             gFuelResource, fp, (double)basket);

        Logf("private_logistics  the tariff IS charged: every delivery is paid the "
             "trip price, indexed to the live fuel price. Cargo value is not a basis.");
    }

    if (g_balanceLog) {
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
            have  = true;
        }
        if (have) {
            g_treasuryAtClose   = treas;
            g_treasuryHaveClose = false;   // first close prints a baseline
            Logf("private_logistics  [BAL] treasury opens at %.2f", treas);
        }
    }

    if (g_billing && g_dryRun)
        Logf("private_logistics  DRY RUN: every [PAY] line says 'would'. "
             "Set dry_run = 0 in the ini and restart to move the money.");
    else if (g_billing)
        Logf("private_logistics  LIVE MONEY: the state treasury pays. Every [PAY] "
             "line reports the amount, the payee and the shortfall gate.");

    Logf("private_logistics  send back every [PAY] line and the daily [PAY] totals");
    return 0;
}
```

#### plugins/private_logistics/private_logistics.ini

SHA-256: `2bb879abbe8ef9e7ff7e5a91eaeb73e698b5aebe50cf2fb6c8c4e1971932b968`

```ini
; ---------------------------------------------------------------------------
; private_logistics.ini - v0.18.5 (source 10% test; preserve disabled thresholds)
;
; WHAT CHANGED IN v0.7, and why.
;
; v0.4-v0.6 paid the carrier 8 % of the cargo's value. The v0.6 run put that
; against the game's own books over 82 days:
;
;   paid to the carrier      14,038.20 RUB
;   diesel the DO bought        511.92 RUB        = 3.6 % of the freight
;   the DO's capital       9,774.99 -> 23,247.13   (+13,472.14)
;
; So the share was 27x the cost it was meant to cover, and it was
; value-blind: 1.46 RUB for a truck of plants, 71.12 for a truck of
; chemicals, same diesel. It is not a tariff, it is a windfall.
;
; v0.7 CHARGES THE TRIP INSTEAD:
;
;   work   = litres_per_trip + litres_per_tonne x tonnes
;   fuel   = work x diesel_density x fuel_price
;   wear   = wear_litres x diesel_density x fuel_price
;   tariff = fuel x (1 + tariff_margin) + wear
;
; Every term is a physical quantity multiplied by a live price. No rouble
; constant, so inflation cannot eat it. No dependence on cargo value, so a
; cheap load still pays for the trip. `wear` is the vehicle sinking fund -
; a carrier has to buy trucks, and 35 % of a fuel bill never buys one.
;
; There is also a METER. enterprise_base books the DO's diesel as a cat=2
; foreign cost, in monthWholesale. Each close reads it back and prints
;
;   [FUEL] day N   <do> diesel X (measured) = Y L at Z/RUB
;   [FUEL] run: ... = Y L/trip | configured W L
;
; That is the game's number, not the model's. Fuel is bought in bulk and
; stored, so ONE DAY's purchases are not one day's burn - the RUN average
; is, because the tank delta washes out. Copy it into litres_per_trip.
; ---------------------------------------------------------------------------

[private_logistics]
enabled             = 1

; PERMISSIONS TEST DEFAULT: no legacy freight payments. Target management
; remains active below. 1 explicitly opts into the old live billing algorithm.
billing             = 0
; 1 = count every payment and move nothing.
dry_run             = 1

; NEW NEXT-TEST DEFAULT: inspect the actual opened detail-panel object and
; candidate-home road-DO fleet membership; read-only, NOT exact burn or billing.
; Requires matching executable bytes, sim_clock and persistence load signal.
; One snapshot/second, 240/load epoch, 16 queued. Paused samples log on unpause.
panel_fuel_observer = 1

; H1: the private building the goods LEAVE is paid = value x factory_share.
; This leg only fires on a privately owned producer. If it reads 0.00 every
; day, no building in the chain is private - check for a [PRIV CLICK] line.
pay_factory         = 1
factory_share       = 0.90

; H2: the fleet owner (the private DO) is paid the trip tariff.
pay_carrier         = 1

; v0.7: 1 = charge the trip tariff (the model above).
;       0 = charge 8 % of cargo value instead - the v0.6 path, kept only so
;           the two can be compared inside one session.
trip_tariff         = 1

; ---------------------------------------------------------------------------
; the tariff, in physical units
; ---------------------------------------------------------------------------

; Diesel a delivery burns, self-calibrating on the [FUEL] run line:
;   litres   = litres_per_trip + litres_per_tonne x tonnes
;   wear     = wear_litres
;
; MEASURED FROM THE VEHICLES' OWN TANKS (v0.10).
;
; veh+0x5F0 is fuel in tonnes. Reading it every day, and summing the drops
; (refills are increases and are counted separately), gives the burn directly:
;
;     v0.9 run: 2.05 t burned over 51 days, 139 deliveries
;               = 0.01475 t/delivery = 14.75 L at 1 kg/L
;
; Three earlier estimates from PURCHASES had given 3.27, 7.14 and 24.8 L/trip -
; they disagreed with each other and with this because fuel sits in tanks
; between purchases. The tank is the only honest source, and [TANK] run prints
; the figure every close.
;
; 14.75 is that measurement FROM THAT FLEET. It is not a property of trucks in
; general: a bigger fleet burns more, a longer route burns more, a different
; vehicle type burns a different rate. The v0.10 run, same office, gave 10.28.
; The number to watch is [TANK] run ... L per fleet-day.
litres_per_trip     = 14.75

; LOAD DOES NOT BURN. Ghidra (FUN_14016E3B0) shows the burn formula takes a
; building-level constant and a per-tick delta; cargo weight never enters it.
; A full truck burns what an empty one burns. This key is kept so the claim can
; be re-tested, and it ships at zero.
litres_per_tonne    = 0.0

; v0.7: the vehicle sinking fund, denominated in litres of diesel so it
; inflates WITH fuel instead of behind it. This is POLICY, not physics: it is
; what you decide a trip should put aside towards the next truck.
;
; Set it from a truck, not from taste:
;     wear_litres = truck_price / trips_per_life / (density x fuel_price)
; A 6,000 RUB truck over 4,000 trips is 1.50 RUB/trip, about 13 L at 133/RUB.
; 5.00 is deliberately conservative - raise it and the DO saves for vehicles.
wear_litres         = 5.00

; 0 = the meter reports and you set litres_per_trip.
; 1 = the plugin re-tunes it itself (85 % old / 15 % measured, each close).
litres_auto         = 0

diesel_density      = 0.00100   ; t per litre. The game stores 1 kg/L: a 456 L
                                ; tank reads 0.456 t at veh+0x5F0. Real diesel is
                                ; 0.84 and using that made every litre 16 % cheap.

; The carrier's profit, as a fraction of fuel cost. Applies to fuel only;
; the wear reserve is a cost and is passed through at cost.
tariff_margin       = 0.35

; The index. 'fuel' is resolved by NAME through resource_pricing every day,
; so the tariff follows the game's own diesel price. If it cannot resolve,
; the same work is priced in daily survival baskets instead, and if neither
; resolves the delivery is reported UNINDEXED and priced at nothing -
; this build never invents a figure in money.
fuel_resource       = fuel
baskets_per_trip    = 0.80
baskets_per_tonne   = 0.02

; Per-delivery [TARIFF] lines; the day total is always printed.
tariff_log_per_day  = 60

; v0.8: the vehicle census. Once a day every private enterprise's fleet is
; walked (bld+0xC70/+0xC78, proven) and each vehicle is logged with what it
; carries now and at its peak. The first sighting of a vehicle also dumps the
; 8 dwords at the pointer the game gives for its catalogue entry (+0x510) -
; that is where capacity, speed, price and consumption live.
;
; Nothing is priced from the census yet. One number cannot describe a fleet,
; and this build refuses to pretend otherwise; the census is how the next one
; stops needing to.
census_log          = 1

; v0.9: the field scanner. Each vehicle's first 0x800 bytes are copied on first
; sight; every day after, only the words that CHANGED are logged:
;
;   [VEH CHG] day 12 v=0000... +0x6D4 000001C8 -> 0000019A (f 456.00 -> 410.00)
;
; A fuel tank is a word that falls while the truck drives and jumps when it
; refuels. 456 above is the kind of value to look for - litres, not roubles.
; Known pointer ranges are skipped, and at most veh_scan_log_per_day words are
; printed per vehicle per day.
veh_scan            = 1
veh_scan_log_per_day = 40

; v0.12: raw storage-vector dump for up to three vehicles per close. BOTH
; vectors - the trailer vector at veh+0x3A0 and the cargo vector at veh+0x3B8 -
; every slot, with the resource pointer and the tonnage. If diesel lives in a
; slot, one slot will fall every day and jump at a refill, exactly like
; [TANK]. Set to 0 once that is settled.
cargo_dump          = 1

; v0.13: where the fuel actually is, from the follow-up disassembly.
;
;   AUTHORITATIVE  the cargo vector at veh+0x3B8, stride 0x10, the slot whose
;                  +0x00 resource pointer equals diesel's. +0x08 = tonnes,
;                  clamped at zero by the engine.
;   MIRROR         veh+0x5F0. Written by the refuel code and by the drive
;                  code, and NOT clamped - which is why it went negative in
;                  the last run. It is read only as a cross-check now.
;
; This many [TANK] lines per run print both, and say whether they agree.
tank_cross_log      = 40

; ---------------------------------------------------------------------------
; instrument
; ---------------------------------------------------------------------------

; [BAL] treasury movement, the carriers' capital and the fuel meter, per day.
; The meter lives here, so balance_log = 0 also disables the measured fuel.
balance_log         = 1
; one [B2B SITE] line per transfer call
site_log            = 1
; cap on [B2B]/[PAY]/[TARIFF] detail lines per day
log_cap_per_day     = 400
; shipment ring buffer
ledger_capacity     = 512

; Absolute payment cap per day, in the active currency. A runaway loop should
; show up as a cap line, not as an emptied treasury.
daily_pay_cap       = 5000.0
; below this, a payment is skipped and counted, never rounded up.
min_payment         = 0.01

; Sites to hook. Empty = all 16. "0x68F792,68F7F9" for a subset.
site_list           =

; Autonomous private DO connections; state site access requires explicit
; per-building direction permissions. State retail: deliveries only. State
; factory/storage: independent loading and deliveries. Unknown roles deny.
; Private endpoints follow their role without player access checkboxes.
; Missing eligible targets are re-added at the daily scan (not instantly).
auto_targets_enabled = 1
; 1 = report plans only (no TaskCfg writes/appends); 0 = apply permissions.
auto_targets_dry_run = 0
; state_storage_always_on is obsolete: any old value is ignored and logged.
; Maximum target-detail lines per daily scan; summary lines are always printed.
auto_targets_log_limit = 80

; ---------------------------------------------------------------------------
; NOT IMPLEMENTED - declared so the intent is on record, not to work yet
; ---------------------------------------------------------------------------
; 0 = the DO pays for diesel through the game's own foreign purchase, which
; is what happens today and what the v0.6 reconciliation measured.
charge_fuel_at_pump      = 0
charge_vehicle_purchase  = 1
upkeep_per_vehicle_day   = 0.0     ; a private DO pays no upkeep: vehicles + fuel

; who pays when the receiving building cannot be identified
unknown_payer            = state

; Company policy for automatic tasks, applied to both new and existing entries
; at daily reconciliation. Native per-DO/per-task edits are overwritten for
; managed targets. Manually assigned customs tasks remain untouched.
; Source 10% matches the working manual setting reported in the latest test.
; This is an empirical test policy, NOT a proven native ceiling or dispatch fix.
; Load when source stock is ABOVE this percentage (policy range 0..90).
auto_source_percent = 10
; Deliver when destination stock is BELOW this percentage (policy range 10..100).
; This is a dispatch trigger, NOT a spending limit or exact final-stock cap.
auto_destination_percent = 30
```

#### plugins/private_logistics/journey_tracker.h

SHA-256: `4b11e07b3099d275d3933b21f6c7367bfc775a49b02f0f7981b8feb65db1ba33`

```cpp
#ifndef PRIVATE_LOGISTICS_JOURNEY_TRACKER_H
#define PRIVATE_LOGISTICS_JOURNEY_TRACKER_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

// Production tracker core only: no game offsets, hooks, IO, persistence or money.
// One instance per vehicle incarnation. The future adapter must serialize calls
// and supply authoritative, fully ordered events, not sampled inferred trips.
namespace private_logistics_journey {

struct Identity {
    // Opaque nonzero adapter tokens, NOT raw pointers or IDs reused after reload.
    std::uint64_t session = 0, vehicle = 0, office = 0;
    bool valid() const { return session && vehicle && office; }
    bool operator==(const Identity& b) const {
        return session == b.session && vehicle == b.vehicle && office == b.office;
    }
};

struct Cargo {
    // Adapter must classify inventory first. Engine fuel is never ordinary goods.
    double goodsTons = 0.0;
    double fuelInventoryTons = 0.0;
};

enum class Kind { ObserveCargo, GoodsTransfer, FuelBurn, Refuel, Gap };
enum class FuelEvidence { ActualClampedDeduction, RequestedOrSampled, Unknown };
enum class FuelCoverage { Unverified, CompleteActualDeductionStream };
enum class Phase { WaitingForEmpty, ArmedEmpty, Loaded };
enum class Reason {
    None, Completed, Reset, InvalidIdentity, SequenceGap, OutOfOrder,
    ExplicitGap, InvalidCargo, CargoMismatch, UnconfirmedCargoChange,
    InvalidFuel, UncertainFuel, NumericOverflow, InvalidEvent
};

inline const char* ReasonName(Reason r) {
    switch (r) {
        case Reason::None: return "none";
        case Reason::Completed: return "completed";
        case Reason::Reset: return "reset";
        case Reason::InvalidIdentity: return "invalid_identity";
        case Reason::SequenceGap: return "sequence_gap";
        case Reason::OutOfOrder: return "out_of_order";
        case Reason::ExplicitGap: return "explicit_gap";
        case Reason::InvalidCargo: return "invalid_cargo";
        case Reason::CargoMismatch: return "cargo_mismatch";
        case Reason::UnconfirmedCargoChange: return "unconfirmed_cargo_change";
        case Reason::InvalidFuel: return "invalid_fuel";
        case Reason::UncertainFuel: return "uncertain_fuel";
        case Reason::NumericOverflow: return "numeric_overflow";
        case Reason::InvalidEvent: return "invalid_event";
    }
    return "invalid_event";
}

struct Event {
    Identity identity;
    // Dense, monotonically increasing PER-TRACKER stream; all relevant events.
    // A global sequence filtered down to one truck is not suitable here.
    std::uint64_t sequence = 0;
    Kind kind = Kind::ObserveCargo;
    Cargo before, after;
    double fuelTons = 0.0;
    FuelEvidence fuelEvidence = FuelEvidence::Unknown;
};

struct Summary {
    Identity identity;
    std::uint64_t journey = 0, startSequence = 0, endSequence = 0;
    bool completed = false;
    bool fuelExact = false;
    double actualBurnTons = 0.0, refillTons = 0.0;
    std::uint64_t burnEvents = 0;
    Reason reason = Reason::None;
};

struct Outcome {
    bool started = false, emitted = false, duplicate = false;
    Reason issue = Reason::None;
    // Start metadata when started; terminal record when emitted. Otherwise empty.
    Summary summary;
};

class Tracker {
public:
    explicit Tracker(Identity identity = Identity(),
                     FuelCoverage coverage = FuelCoverage::Unverified)
        : identity_(identity), coverage_(coverage) {}

    Phase phase() const { return phase_; }

    // Always explicit on world reset/reload, vehicle reuse or office reassignment.
    // No pointer continuity or active journey is restored from a save.
    Outcome Reset(Identity identity, FuelCoverage coverage = FuelCoverage::Unverified) {
        Outcome out = Invalidate(Reason::Reset, lastSequence_);
        identity_ = identity;
        coverage_ = coverage;
        seen_ = false;
        lastSequence_ = 0;
        return out;
    }

    Outcome Consume(const Event& e) {
        if (!identity_.valid() || !e.identity.valid() || !(e.identity == identity_))
            return Invalidate(Reason::InvalidIdentity, lastSequence_);
        if (!e.sequence) return Invalidate(Reason::OutOfOrder, lastSequence_);
        if (seen_ && e.sequence == lastSequence_ && SameEvent(e, lastEvent_)) {
            Outcome out;
            out.duplicate = true;
            return out;
        }
        if (seen_ && e.sequence <= lastSequence_)
            return Invalidate(Reason::OutOfOrder, lastSequence_);
        const bool gap = seen_ && e.sequence - lastSequence_ != 1;
        seen_ = true;
        lastSequence_ = e.sequence;
        lastEvent_ = e;
        // Consume the envelope but not its payload after a gap. A subsequent
        // authoritative empty state is required; never bridge missing events.
        if (gap) return Invalidate(Reason::SequenceGap, e.sequence);

        switch (e.kind) {
            case Kind::ObserveCargo: return Observe(e);
            case Kind::GoodsTransfer: return Transfer(e);
            case Kind::FuelBurn: return Burn(e);
            case Kind::Refuel: return Refill(e);
            case Kind::Gap: return Invalidate(Reason::ExplicitGap, e.sequence);
        }
        return Invalidate(Reason::InvalidEvent, e.sequence);
    }

private:
    Identity identity_;
    FuelCoverage coverage_;
    Phase phase_ = Phase::WaitingForEmpty;
    bool goodsKnown_ = false, seen_ = false;
    double goods_ = 0.0;
    std::uint64_t serial_ = 0, lastSequence_ = 0;
    Event lastEvent_;
    Summary active_;

    static bool Quantity(double x) { return std::isfinite(x) && x >= 0.0; }
    static bool ValidCargo(const Cargo& c) {
        return Quantity(c.goodsTons) && Quantity(c.fuelInventoryTons);
    }
    static bool SameEvent(const Event& a, const Event& b) {
        return a.identity == b.identity && a.sequence == b.sequence &&
            a.kind == b.kind && a.before.goodsTons == b.before.goodsTons &&
            a.before.fuelInventoryTons == b.before.fuelInventoryTons &&
            a.after.goodsTons == b.after.goodsTons &&
            a.after.fuelInventoryTons == b.after.fuelInventoryTons &&
            a.fuelTons == b.fuelTons && a.fuelEvidence == b.fuelEvidence;
    }
    Outcome Invalidate(Reason why, std::uint64_t end) {
        Outcome out;
        out.issue = why;
        if (phase_ == Phase::Loaded) {
            out.emitted = true;
            out.summary = active_;
            out.summary.completed = false;
            out.summary.fuelExact = false;
            out.summary.reason = why;
            out.summary.endSequence = end;
        }
        active_ = Summary();
        phase_ = Phase::WaitingForEmpty;
        goodsKnown_ = false;
        return out;
    }
    void Baseline(const Cargo& c) {
        goodsKnown_ = true;
        goods_ = c.goodsTons;
        phase_ = goods_ == 0.0 ? Phase::ArmedEmpty : Phase::WaitingForEmpty;
    }
    Outcome Observe(const Event& e) {
        if (!ValidCargo(e.after)) return Invalidate(Reason::InvalidCargo, e.sequence);
        if (goodsKnown_ && goods_ != e.after.goodsTons) {
            Outcome out = Invalidate(Reason::UnconfirmedCargoChange, e.sequence);
            Baseline(e.after); // This observation cannot start/complete a trip.
            return out;
        }
        if (phase_ != Phase::Loaded) Baseline(e.after);
        return Outcome();
    }
    Outcome Transfer(const Event& e) {
        if (!ValidCargo(e.before) || !ValidCargo(e.after))
            return Invalidate(Reason::InvalidCargo, e.sequence);
        if (goodsKnown_ && goods_ != e.before.goodsTons) {
            Outcome out = Invalidate(Reason::CargoMismatch, e.sequence);
            Baseline(e.after);
            return out; // No replacement journey from an inconsistent transfer.
        }
        if (!goodsKnown_) Baseline(e.before);
        goodsKnown_ = true;
        goods_ = e.after.goodsTons;
        Outcome out;
        if (phase_ == Phase::Loaded) {
            if (goods_ == 0.0) {
                out.emitted = true;
                out.summary = active_;
                out.summary.completed = true;
                out.summary.endSequence = e.sequence;
                out.summary.reason = active_.fuelExact ? Reason::Completed : Reason::UncertainFuel;
                active_ = Summary();
                phase_ = Phase::ArmedEmpty;
            }
        } else if (phase_ == Phase::ArmedEmpty && goods_ > 0.0) {
            if (serial_ == UINT64_MAX)
                return Invalidate(Reason::NumericOverflow, e.sequence);
            active_ = Summary();
            active_.identity = identity_;
            active_.journey = ++serial_;
            active_.startSequence = e.sequence;
            // Absence of burn events is NOT evidence of zero consumption unless
            // the future adapter explicitly guarantees complete actual events.
            active_.fuelExact = coverage_ == FuelCoverage::CompleteActualDeductionStream;
            phase_ = Phase::Loaded;
            out.started = true;
            out.summary = active_; // Expose the stable key immediately, without emitting completion.
        } else if (goods_ == 0.0) {
            phase_ = Phase::ArmedEmpty;
        }
        return out;
    }
    Outcome Burn(const Event& e) {
        if (!Quantity(e.fuelTons)) return Invalidate(Reason::InvalidFuel, e.sequence);
        if (e.fuelEvidence != FuelEvidence::ActualClampedDeduction &&
            e.fuelEvidence != FuelEvidence::RequestedOrSampled &&
            e.fuelEvidence != FuelEvidence::Unknown)
            return Invalidate(Reason::InvalidEvent, e.sequence);
        if (phase_ != Phase::Loaded) return Outcome(); // Exclude empty legs.
        if (e.fuelEvidence != FuelEvidence::ActualClampedDeduction) {
            active_.fuelExact = false;
            Outcome out;
            out.issue = Reason::UncertainFuel;
            return out; // Never add sampled/requested quantities as actual burn.
        }
        const double sum = active_.actualBurnTons + e.fuelTons;
        if (!Quantity(sum) || active_.burnEvents == UINT64_MAX)
            return Invalidate(Reason::NumericOverflow, e.sequence);
        active_.actualBurnTons = sum;
        ++active_.burnEvents;
        return Outcome();
    }
    Outcome Refill(const Event& e) {
        if (!Quantity(e.fuelTons)) return Invalidate(Reason::InvalidFuel, e.sequence);
        if (phase_ != Phase::Loaded) return Outcome();
        const double sum = active_.refillTons + e.fuelTons;
        if (!Quantity(sum)) return Invalidate(Reason::NumericOverflow, e.sequence);
        active_.refillTons = sum; // Never subtract from, or reset, actual burn.
        return Outcome();
    }
};

// Optional bounded text formatting only; caller owns storage/IO. Never settles.
// False means invalid destination or truncation; partial text is not a record.
inline bool FormatSummary(const Summary& s, char* buffer, std::size_t size) {
    if (!buffer || !size) return false;
    buffer[0] = '\0';
    char record[512];
    const int n = std::snprintf(record, sizeof(record),
        "[JOURNEY_CORE] schema=1 session=%llu vehicle=%llu office=%llu journey=%llu "
        "start_seq=%llu end_seq=%llu completed=%u fuel_exact=%u "
        "actual_burn_t=%.17g refill_t=%.17g burn_events=%llu reason=%s billing=off",
        static_cast<unsigned long long>(s.identity.session),
        static_cast<unsigned long long>(s.identity.vehicle),
        static_cast<unsigned long long>(s.identity.office),
        static_cast<unsigned long long>(s.journey),
        static_cast<unsigned long long>(s.startSequence),
        static_cast<unsigned long long>(s.endSequence),
        s.completed ? 1u : 0u, s.fuelExact ? 1u : 0u, s.actualBurnTons, s.refillTons,
        static_cast<unsigned long long>(s.burnEvents), ReasonName(s.reason));
    if (n < 0 || static_cast<std::size_t>(n) >= sizeof(record) ||
        static_cast<std::size_t>(n) >= size) return false;
    std::memcpy(buffer, record, static_cast<std::size_t>(n) + 1);
    return true;
}

} // namespace private_logistics_journey
#endif
```

---

## Real-game review: tesmioloader-2.log and five truck screenshots (2026-10-30)

### Verdict

**PASS for this test's actual gate: the deployed Windows observer ran, the opened truck is an actual member of the same DO fleet, and the reconstructed fuel number matches ALL FIVE supplied screenshot values.** The previous question of whether the selected DO truck's changing displayed fuel could be read is resolved for this Pan Movic Covered instance/session.

**NOT YET a validated exact loaded-journey fuel meter or settlement implementation.** No burn-deduction hook, refill-event validation or native journey-core adapter was installed by this test. Billing stays OFF. The analysis below does not promote endpoint subtraction or invalid legacy fields into those missing events.

This is a review of the actual user-uploaded log and screenshots, not a synthetic rerun. No runtime source/INI change, commit, push or PR is performed in this review. Current work branch: arena/0a144a79-wrsre.

### Inputs and reproducibility

- Log: user attachment `tesmioloader-2.log`, 315,406 bytes, 2,467 lines; SHA-256 **82e77ba704ba8ce168ea9230801c040cbcf0b3f204fefbfe5a19c7b572bf3e83**. Copied unchanged to ignored `.cache/panel-game-test.log` for local analysis. Raw log/images are not added to Git.
- Current screenshots: `image-2.png` through `image-6.png`, in the supplied order. The user identifies a single truck and two trips in this test save. The screenshots independently show the same model title, task destinations and increasing odometer; pointer identity comes from the log, not the model name.
- Complete Python analysis source and its actual stdout are embedded at the end of this section. It parses the original 11-line PROBE blocks without absorbing fields from the following record; distinguishes committed transfers from zero/no-op site logs; reproduces float32 multiplication/rounding; and reports hashes of precisely these five image attachments. The unrelated older image-1 attachment is not an input.
- All line numbers below refer to this exact raw uploaded log. Logger timestamps are **drain/print time**, not necessarily capture time. The panel record's `ms` is the captured uptime timestamp. Screenshots lack embedded capture timestamps, so matching values do not establish an exact simultaneous frame.

### 1. The Windows plugin and hook actually ran

Startup line135 reports the verified renderer hook installed at RVA7D5460, stolen20, scale1000, format `%.2fl`, interval1000ms, cap240, queue16. Line153 reports `private_logistics live v0.18.6-panel-observer: billing=0 dry_run=1`. ui_panels independently sees the patched dispatcher target at line255 (`6EA958 -> 7D5460`, now beginning with the inline jump), rather than an untouched renderer.

There are 115 complete `[PANEL FUEL]` records and 115 corresponding `[PANEL OBJECT]` records. There is no observer fault-disable or cap-exhaustion message, and the log reaches ordinary loader shutdown at line2467. This is real execution evidence for the observer path and coexistence in this run. It is not a new local MSVC build result, a hash of the installed DLL, a proof of all Windows ABI/SEH paths, or a guarantee against future crashes.

### 2. Same-truck and same-DO identity: PASS

All 115 object records identify:

| Item | Observed value |
|---|---|
| Root vehicle | `0000022219E8A030` |
| Candidate home/DO | `0000022219E6A070` |
| Panel window | `0000022229B326E0` |
| Metadata | `00000221C932A4D0` |
| Membership | `exact_fleet_pointer` |
| Fleet index / occurrences | 0 / 1 |
| Links | 0 |
| Quantity readable | 115/115 |
| Aggregate complete / ratio valid | 115/115 each |
| Epoch | 2 throughout |

The actual pointer was compared with a literal fleet slot; this is not merely "there is only one truck, therefore assume it is ours." The old exploratory probe also records the SAME veh/do pair (selection line800 onward), and the private-enterprise census reports one `muddy_distribution` fleet vehicle with `home=this` (lines1205–1210). AUTO DO AUDIT names that same office pointer/key BF99DCF6FA074EA1 (line1236). The private registry reports one private finished type43 building (lines784–785), consistent with the user's setup.

The observer itself still logs private_status=not_classified; the setup/registry/census provide supporting private-office context. This test does not validate a general private-ownership adapter, exhaustive fleet search, multiple-truck selection, other models, trailers or pointer continuity across reload.

### 3. Every screenshot value matches: PASS

| Attachment / scene | Visible gauge | Matching log sample(s) | Raw +5F0 from first match | First matching log line |
|---|---:|---|---:|---:|
| image-2, first cycle, travelling toward loading, cargo empty / 8.00t gross | 90.09l | 1–8 | 0.0900869071 | 813 |
| image-3, first delivery endpoint, stopped, cargo empty | 70.23l | 28–39 | 0.0702253059 | 1333 |
| image-4, second cycle carrying electronics, 9.99t gross | 56.09l | 54–59 | 0.0560917258 | 1811 |
| image-5, second delivery endpoint, stopped, cargo empty | 41.14l | 81–97 | 0.0411440469 | 2092 |
| image-6, final vehicle state, no listed stops, cargo empty | 28.61l | 109–115 | 0.0286091547 | 2400 |

For example: float32(0.0900869071 ×1000) formatted to two decimals gives **90.09**, and float32(0.0286091547 ×1000) gives **28.61**. The audit reconstructs float32 operations and verifies the logged two-decimal candidate on **115/115** records. The five screenshots provide independent native-UI comparison; all 115 arithmetic checks alone would only verify our logger's internal consistency.

Raw current falls on 60 successive sample transitions and stays equal on54; no captured increase. Capacity stays **0.462799996**, or about **462.80 displayed-equivalent litres**, NOT the current fuel amount. There are no linked objects or capacity adjustments in this truck's records. Both special-presentation flags stay zero, mode2, metadata_kind1. The static +7C58 helper remains untyped, so presentation=unresolved_7c58 is still the conservative logger label. Actual screenshot agreement validates the literal-litres reconstruction for THIS presentation, not every alternate vehicle/unit mode.

This establishes **+5F0 as a live backing quantity of this truck's native fuel display**, rather than a constant candidate unrelated to the opened panel. It does not independently establish authoritative simulation storage, mass units for settlement, empty-tank clamping, or every cause of writes.

### 4. What the two trips actually did

The screenshots and cargo snapshots support two ordinary cargo load-to-empty cycles, not 66 completed journeys:

**Food, warehouse -> big grocery:**

- 17:16:19.817: goods0, empty (probe observation3, line829).
- 17:16:23.611–17:16:25.341: 37 committed vehicle-side loading fragments at site6953C4. Complementary warehouse-side records at694F40.
- Probe cargo grows through 1.039310t / 5.435862t to **5.500000t food** by17:16:26.222.
- 17:16:39.816–17:16:42.356: **40 committed vehicle-side unloading fragments** at68F7F9, paired with40 recipient-side committed deliveries at68F792.
- Partial remaining cargo1.697796t is sampled while unloading; the next empty snapshot is17:16:55.343 (log was paused in between).

**Electronics, warehouse -> small store:**

- Truck is empty between the two loads.
- 17:17:07.024–17:17:07.859: **27 committed vehicle-side loading fragments**, paired with27 warehouse-side records.
- Probe cargo grows from0.206810t to **1.990260t electronics**. The screenshot rounds this to2.0t and gross9.99t, consistent with the8t empty weight.
- 17:17:37.992–17:17:38.824: **26 committed vehicle-side unloading fragments**, paired with26 recipient deliveries.
- Partial remaining cargo1.456907t is observed during unload; the next empty observation is17:18:50.224 after the long pause, and it stays empty in subsequent measurements.

There are also **25 site68F792 records with commit=0 and tons=0** during the first unload. Those have no paired vehicle-side movement and must not be counted as real deliveries, extra journeys or fuel events. The apparent mismatch of91 recipient-site entries versus66 vehicle-side entries is exactly66 committed +25 no-op, NOT missing complementary movement evidence.

Totals from EACH SIDE's logged fragment amounts must not be treated as lossless inventory measurements: tons in these site lines are rounded to three decimals. Their sums are5.501t loaded/5.500t unloaded for food and1.995t loaded/1.992t unloaded for electronics. The measured goods inventories5.500000t and1.990260t are more informative. Do not diagnose vanished/created goods from these few-kilogram rounded-log differences. Do not double-count both sides of a movement.

**Boundary limitation:** these snapshots bracket first loading and final unloading; they do not sample exact cargo-zero transition instants. The runtime journey core is not connected and did not emit two journey completions. We can reconstruct two clear cargo cycles for review; this is not execution of the future confirmed-boundary adapter.

### 5. Fuel changes: useful observations, not an exact loaded-trip invoice

Using the visible rounded screenshot values ONLY:

| Interval | Display change |
|---|---:|
| First pictured cycle, 90.09 ->70.23 | 19.86l decrease |
| Between first endpoint and second start, 70.23 ->56.09 | 14.14l decrease |
| Second pictured loaded interval, 56.09 ->41.14 | 14.95l decrease |
| After second pictured endpoint to final state, 41.14 ->28.61 | 12.53l decrease |
| Full pictured run, 90.09 ->28.61 | 61.48l decrease |

These are differences of rounded DISPLAY endpoints, with up to roughly0.01l rounding uncertainty per difference, and are not exact burn measurements. For example the raw logged values for70.23/56.09 differ by14.1335801l, whereas subtracting the displayed rounded endpoints gives14.14l. Across the entire recorded panel series, the raw-equivalent endpoint decrease is61.4777524l.

**The first screenshot is empty and travelling to load.** Its19.86l interval includes empty approach/loading, not just the user's intended "first confirmed goods loading -> all goods unloaded" billable interval. The second start screenshot already carries electronics, but is not a logged first-positive-goods transition either. The14.14l inter-cycle and12.53l post-delivery reductions demonstrate why endpoint-to-endpoint or whole-run charging would include empty/non-loaded travel. They must not be added to a completed loaded journey. The entire61.48l must NOT be charged as the two loaded journeys' consumption.

No captured fuel increase and the legacy refuel-site summaries report zero calls, so there is **no observed refill** in this test. That is not an exhaustive proof against a refill between observations/on an uninstrumented path. No actual-deduction amount, empty-tank event, simultaneous burn/refill ordering, fuel-resource mass unit or tested refill callback was recorded. A normal-looking falling fuel gauge does not resolve those missing event semantics.

### 6. Sampling gaps: understood limitation, not a failed screenshot match

- Samples1..115 are sequential, but sequential IDs count **accepted records only**. They do not prove dense simulation/event coverage.
- The logger's queue-full-attempt counter ends at **51**. This is reported from global queue state at DRAIN time, not a per-record capture-time drop counter.
- There is an explicit long pause17:17:38.888–17:18:49.888, **71 seconds**. Rendering continues capture attempts while sim_clock does not drain the16-record queue. This explains the observed backlog/queue-full pattern, although a queue-full attempt is not proof of a lost simulation burn event.
- Capture timestamps between samples97 and98 jump from28936375ms to28989656ms: **53.281 seconds**. Both were printed only0.138 seconds apart after resume. Other queued records likewise appear in a rapid burst. This is not sampling at tens of Hz or fuel mysteriously changing during the pause.
- All115 delivered snapshots are readable/complete. The240-record cap was not hit. There is no observer fault-disable line.

This does not invalidate the identity or five screenshot matches. It DOES forbid describing this as a complete fuel-deduction stream. Improving the diagnostic should avoid repeatedly enqueuing identical paused-state values (or use an explicitly discontinuous/coalesced latest-state policy) and emit explicit gap/drop semantics; merely inflating the queue does not turn samples into exact burn events. No need to repeat this unchanged panel test just because it had a long screenshot pause.

### 7. The old readers/log labels are still wrong or misleading

**Cargo fuel-slot probe:** 58 PROBE blocks comprise1 world-reset,1 selection and56 actual measurements. In ALL56 measurements, fuel_slot_valid=0/fuel_tons=-1, while mirror_valid=1, cargo_valid=1 and home_matches=1. The mirror now changes on the same verified truck and follows the gauge. The older game+C3F0 cargo-slot hypothesis is still NOT validated by this run. It must not be promoted based on the new display success.

**`[TANK] agree` is not independent agreement:** line1211 explicitly falls back to+5F0 due to no fuel slot. Line1906 later calls0.0494t "slot" and "mirror" and says agree. Source ReadTank still copies the mirror into level whenever found==0, then compares it to itself. This line does NOT mean the fuel slot started working on day15. The reported0.0213t burned is a daily sampled difference across vehicle activity, not exact loaded-trip burn.

**Legacy state/kind fields:** all56 actual measurements show kind=-1, state=0 and mode=0 while screenshots and decreasing gauge show actual movement. Daily census similarly calls the truck idle. The panel's mode=2 is a DIFFERENT field on a different base, not a contradiction to reconcile by treating names as types. Do not gate the new tracker using the old +448/+440 activity classification. It is not validated as a road-truck activity signal; this test contradicts using state0 to infer no movement/no burn.

**Misleading tariff output, but billing remained OFF:** line153 has billing0/dry_run1; lines1202/1897 say `[PAY] ... billing off`; individual `[TARIFF]` lines say `charged 0.00 by the state`. Nonetheless startup line157 says "the tariff IS charged", and the day15 tariff summary at1898 says40 deliveries charged138.65. Inspection of the actual source shows the legacy tariff accumulators run BEFORE the g_billing gate, and count/price per committed recipient fragment. Thus40 is the first unloading's40 fragments, NOT40 trips, and138.65 is a legacy model tally, not money actually paid by that path. Other economic plugins remain active; this is not a claim that the entire treasury was motionless.

The old14.75L/trip +margin/wear model remains code but was not enabled for settlement. Do not calibrate it from the screenshot deltas, enable it now or mistake the per-fragment tallies for the approved future journey policy. Next cleanup should clearly label model-only totals and distinguish missing-slot/mirror fallback provenance. The correct current observation is display-fuel correlation, not a validated slot or price.

### 8. Dispatch and permissions: no observed regression

Both daily reconciliations identify the same office0000022219E6A070, one private office, three existing targets, config_changed0, added0, suppressed0, invalid0. Raw native targets show:

- Warehouse0000022219E6DFA0: requested_mask1, enabled1, load_all1, load_pct0.100; unloading disabled.
- Small-store00000221F4496810 and grocery00000221F4495300: requested_mask2, enabled1, loading disabled, unload_all1/unload_pct0.300.

The two actual warehouse-to-retail deliveries are positive gameplay evidence that dispatch still operates with this observer. The displayed stop tasks "Load100%" / "Load50%" are truck loading tasks, not a contradiction to source stock threshold10% / destination threshold30%. They do not justify changing the working thresholds.

This run is not a fresh test of reconnection after manual removal, permission toggles, multiple trucks or in-process save/reload continuity. The initial epoch2/world-reset record shows a load signal was received, not that all reload edge cases have been validated.

### 9. Separate save/install warnings — do not ignore

The game logs missing workshop/resource files during load, plus two read errors for replace_history.bin/usedveh.bin. At shutdown, lines2429–2466 contain **38 game.ERROR lines** for saving the saved_last exit-save files. These warnings do not establish a fuel-observer cause; similar missing-path/installation/save issues require separate diagnosis, and I have not tested their cause. The observer still matches the live panel, but the exit-save must NOT be assumed successful.

An autosave3 write is announced at17:18:58.309; an announcement alone is not verification that the save is complete and reloadable. Keep the original working save backed up. Verify a normal manual save in a separate slot and reopen the copy before relying on persistence/exit-save. Do not overwrite/delete the only known-good save to test this. A future reload acceptance test needs an actually successful save, not this failed saved_last write.

### 10. Accepted milestone and next work

**Accepted for this vehicle/session:** actual Windows hook execution; same opened road truck and DO fleet identity; readable changing +5F0 display source; float32×1000 litres conversion matching five UI screenshots; cargo empty/load/partial/unload observations; no observed dispatch regression; billing disabled.

**Still open:** authoritative simulation quantity/unit; actual clamped deduction events; refills/same-update ordering; reliable per-truck activity and goods/fuel classification; exact cargo-boundary event adapter; complete event-stream coverage; multiple vehicles/lifecycle tests; one completion per real loaded journey; settlement. The standalone tested C++ core still has no native consumer.

Next actions should be implementation/targeted receiver work, not another broad gauge prompt or unchanged reader test:

1. Fix misleading diagnostics: label the legacy tariff as model-only/per-transfer while billing is OFF, preserve slot-vs-fallback validity in `[TANK]`, and make paused queue gaps explicit. Do not change dispatch,10/30 thresholds or enable old payments.
2. Trace actual simulation writes for the NOW-VERIFIED road-truck object and identify real deduction/refill storage and clamping. The old building/upkeep candidate family must not be reused solely because it also accesses+5F0 or has "vehicle" labels. The invalid C3F0 reader and legacy idle classification are NOT gates to reinstate. A bounded writer/receiver export may be necessary if existing raw evidence cannot establish that path; no full renderer export/string scan is needed.
3. Build a logging-only native adapter using the actual vehicle-side cargo transfers plus verified full goods state and real fuel deductions. The37/40 and27/26 fragments must fold into TWO loaded journeys, retaining partial unloading. Raw callbacks cannot be counted as trips; no dense renumbering after gaps or sampled decreases promoted to actual deductions.
4. The NEXT changed game test should exercise that adapter: two completed loaded journeys should yield exactly two diagnostic completions, exclude empty approach/inter-trip/return travel, keep exactness false on missing coverage, and never settle money. Separately validate refill and reload/gap behavior before turning on settlement. Do not ask the user to rerun this same panel-only test as if its identity/conversion result were missing.

No broad investigator handoff is needed to re-prove the display. No further identical game run is needed for the milestone just passed. Billing and runtime journey integration remain deliberately unchanged by this analysis.

### Complete analysis source actually executed

Command: `python3 .cache/analyze_panel_game_test.py > .cache/panel-game-analysis.txt`. An optional first argument accepts another raw log path. The full script below is an offline analysis helper, not new plugin code or a new regression suite; its assertions check this exact supplied capture. Image paths are the user-upload locations and can be adjusted when reproducing elsewhere.

```python
from pathlib import Path
import re, json, hashlib, collections, struct, sys
p=Path(sys.argv[1]) if len(sys.argv)>1 else Path('.cache/panel-game-test.log');raw=p.read_bytes();lines=raw.decode('utf-8-sig',errors='replace').splitlines()
def fields(line): return dict(re.findall(r'(\w+)=([^\s]+)',line))
pan=[];objs=[];probes=[];trans=[]
for n,line in enumerate(lines,1):
 d=fields(line);d.update(line=n,time=line[1:13])
 if '[PANEL FUEL]' in line and 'sample=' in line:pan.append(d)
 if '[PANEL OBJECT]' in line:objs.append(d)
 if '[PROBE SNAPSHOT]' in line:
  block='\n'.join(lines[n-1:n+10]);d=fields(block);d.update(line=n,time=line[1:13]);probes.append(d)
 if '[B2B SITE] rva=' in line:
  d.update(text=line);trans.append(d)
print('SHA256',hashlib.sha256(raw).hexdigest(),'lines',len(lines),'bytes',len(raw))
print('Panel count',len(pan),'object count',len(objs),'probe count',len(probes))
for key in ['epoch','root','status','aggregate_valid','links','ratio_valid','mode','metadata_kind','flag8681','flag8682','adjustment_needed','presentation']:
 print('Panel',key,dict(collections.Counter(d[key] for d in pan)))
for key in ['object','candidate_home','membership','occurrences','quantity_read','metadata']:
 print('Object',key,dict(collections.Counter(d[key] for d in objs)))
print('First',pan[0]);print('Last',pan[-1])
seq=[int(d['sample']) for d in pan];print('Sequence gaps',[(a,b) for a,b in zip(seq,seq[1:]) if b!=a+1])
values=[float(d['current_raw'])*1000 for d in pan]
print('Panel value changes',dict(collections.Counter('down' if b<a else 'up' if b>a else 'flat' for a,b in zip(values,values[1:]))))
print('Panel sum decreases',sum(max(0,a-b) for a,b in zip(values,values[1:])),'sum increases',sum(max(0,b-a) for a,b in zip(values,values[1:])))
print('Worst capture gap ms',max(int(b['ms'])-int(a['ms']) for a,b in zip(pan,pan[1:])))
print('Max queue-full attempts',max(int(d['dropped']) for d in pan))
print('Rounded displayed matches:')
for target in [90.09,70.23,56.09,41.14,28.61]:
 exact=[d for d in pan if float(d['litres_candidate'])==target]
 nearest=sorted(pan,key=lambda d:abs(float(d['litres_candidate'])-target))[:2]
 print(target,'exact',[(d['sample'],d['time'],d['line'],d['current_raw']) for d in exact], 'nearest',[(d['sample'],d['litres_candidate']) for d in nearest])
print('Probe validity',collections.Counter((d.get('fuel_slot_valid'),d.get('mirror_valid'),d.get('cargo_valid'),d.get('state'),d.get('mode'),d.get('kind')) for d in probes))
print('Probe cargo transitions:')
last=None
for d in probes:
 state=(d.get('cargo_total_tons'),d.get('cargo_slots'))
 if state!=last:
  print(d['line'],d['time'],'obs',d.get('observation_seq'),'cargo',state,'mirror',d.get('mirror_tons'))
  last=state
print('Transfers grouped by site/resource:')
for key in sorted(set((d.get('rva'),d.get('commit')) for d in trans)):
 group=[d for d in trans if (d.get('rva'),d.get('commit'))==key]
 # res includes one unquoted token; amounts are logged only to 3 decimals.
 print(key,len(group),'sum_rounded_tons',sum(float(d['tons']) for d in group),'range',group[0]['time'],group[-1]['time'])
print('All sample stretches with constant rounded candidate:')
last=None
for d in pan:
 val=d['litres_candidate']
 if val!=last:print(d['line'],d['sample'],d['time'],d['ms'],val,'drops',d['dropped'])
 last=val
out={'panel':pan,'objects':objs,'probes':probes,'transfers':trans}
Path('.cache/panel-game-analysis.json').write_text(json.dumps(out,indent=2))

print('Float32 multiplication/format audit:')
def f32(x): return struct.unpack('<f',struct.pack('<f',x))[0]
assert len(pan)==len(objs)==115
assert [int(d['sample']) for d in pan]==list(range(1,116))
assert all(d['status']=='complete' and d['aggregate_valid']=='1' for d in pan)
assert all(d['membership']=='exact_fleet_pointer' and d['occurrences']=='1' for d in objs)
assert all(format(f32(f32(float(d['current_raw']))*1000.0),'.2f')==d['litres_candidate'] for d in pan)
print('115/115 arithmetic, validity, membership and sequence assertions PASS')
print('Committed transfer groups enriched from immediately following attribution:')
groups=collections.defaultdict(list)
for d in trans:
 i=d['line']-1;res='none'
 for text in lines[i+1:i+8]:
  if '[B2B SITE] rva=' in text: break
  if '[B2B] #' in text and (match:=re.search(r'\bres=(\S+)',text)):
   res=match[1];break
 groups[(d['rva'],d['commit'],res)].append(d)
for key,group in sorted(groups.items()):
 print(key,len(group),round(sum(float(d['tons']) for d in group),6),group[0]['time'],group[-1]['time'])
print('Measurements',sum(d.get('record_type')=='measurement' for d in probes))
print('Shutdown error lines',sum('game.ERROR' in line and line.startswith('[17:19:12') for line in lines))
print('Images:')
for image in [Path('/tmp/arena-coding-uploads') / ('image-%d.png' % i) for i in range(2,7)]:
 print(image.name,hashlib.sha256(image.read_bytes()).hexdigest())
```

### Actual analysis output

```text
SHA256 82e77ba704ba8ce168ea9230801c040cbcf0b3f204fefbfe5a19c7b572bf3e83 lines 2467 bytes 315406
Panel count 115 object count 115 probe count 58
Panel epoch {'2': 115}
Panel root {'0000022219E8A030': 115}
Panel status {'complete': 115}
Panel aggregate_valid {'1': 115}
Panel links {'0': 115}
Panel ratio_valid {'1': 115}
Panel mode {'2': 115}
Panel metadata_kind {'1': 115}
Panel flag8681 {'0': 115}
Panel flag8682 {'0': 115}
Panel adjustment_needed {'0': 115}
Panel presentation {'unresolved_7c58': 115}
Object object {'0000022219E8A030': 115}
Object candidate_home {'0000022219E6A070': 115}
Object membership {'exact_fleet_pointer': 115}
Object occurrences {'1': 115}
Object quantity_read {'1': 115}
Object metadata {'00000221C932A4D0': 115}
First {'epoch': '2', 'sample': '1', 'ms': '28829250', 'phase': 'post_render', 'window': '0000022229B326E0', 'root': '0000022219E8A030', 'status': 'complete', 'aggregate_valid': '1', 'current_raw': '0.0900869071', 'litres_candidate': '90.09', 'capacity_unadjusted': '0.462799996', 'ratio_valid': '1', 'ratio': '0.194656238', 'links': '0', 'mode': '2', 'metadata_kind': '1', 'flags_read': '1', 'flag8681': '0', 'flag8682': '0', 'adjustment_read': '1', 'adjustment_needed': '0', 'presentation': 'unresolved_7c58', 'dropped': '0', 'physical_fuel': 'unverified', 'exact_burn': '0', 'billing': 'off', 'line': 813, 'time': '17:16:19.122'}
Last {'epoch': '2', 'sample': '115', 'ms': '29008921', 'phase': 'post_render', 'window': '0000022229B326E0', 'root': '0000022219E8A030', 'status': 'complete', 'aggregate_valid': '1', 'current_raw': '0.0286091547', 'litres_candidate': '28.61', 'capacity_unadjusted': '0.462799996', 'ratio_valid': '1', 'ratio': '0.0618175343', 'links': '0', 'mode': '2', 'metadata_kind': '1', 'flags_read': '1', 'flag8681': '0', 'flag8682': '0', 'adjustment_read': '1', 'adjustment_needed': '0', 'presentation': 'unresolved_7c58', 'dropped': '51', 'physical_fuel': 'unverified', 'exact_burn': '0', 'billing': 'off', 'line': 2426, 'time': '17:19:09.785'}
Sequence gaps []
Panel value changes {'flat': 54, 'down': 60}
Panel sum decreases 61.4777524 sum increases 0
Worst capture gap ms 53281
Max queue-full attempts 51
Rounded displayed matches:
90.09 exact [('1', '17:16:19.122', 813, '0.0900869071'), ('2', '17:16:19.212', 815, '0.0900869071'), ('3', '17:16:19.267', 817, '0.0900869071'), ('4', '17:16:19.316', 819, '0.0900869071'), ('5', '17:16:19.358', 821, '0.0900869071'), ('6', '17:16:19.417', 823, '0.0900869071'), ('7', '17:16:19.463', 825, '0.0900869071'), ('8', '17:16:19.545', 827, '0.0900869071')] nearest [('1', '90.09'), ('2', '90.09')]
70.23 exact [('28', '17:16:40.696', 1333, '0.0702253059'), ('29', '17:16:41.405', 1426, '0.0702253059'), ('30', '17:16:54.641', 1506, '0.0702253059'), ('31', '17:16:54.706', 1508, '0.0702253059'), ('32', '17:16:54.750', 1510, '0.0702253059'), ('33', '17:16:54.798', 1512, '0.0702253059'), ('34', '17:16:54.842', 1514, '0.0702253059'), ('35', '17:16:54.885', 1516, '0.0702253059'), ('36', '17:16:54.923', 1518, '0.0702253059'), ('37', '17:16:54.969', 1520, '0.0702253059'), ('38', '17:16:55.043', 1522, '0.0702253059'), ('39', '17:16:55.072', 1524, '0.0702253059')] nearest [('28', '70.23'), ('29', '70.23')]
56.09 exact [('54', '17:17:15.786', 1811, '0.0560917258'), ('55', '17:17:15.851', 1813, '0.0560917258'), ('56', '17:17:15.887', 1815, '0.0560917258'), ('57', '17:17:15.926', 1817, '0.0560917258'), ('58', '17:17:15.964', 1819, '0.0560917258'), ('59', '17:17:16.001', 1821, '0.0560917258')] nearest [('54', '56.09'), ('55', '56.09')]
41.14 exact [('81', '17:17:38.226', 2092, '0.0411440469'), ('82', '17:18:49.896', 2204, '0.0411440469'), ('83', '17:18:49.934', 2206, '0.0411440469'), ('84', '17:18:49.966', 2208, '0.0411440469'), ('85', '17:18:49.995', 2210, '0.0411440469'), ('86', '17:18:50.022', 2212, '0.0411440469'), ('87', '17:18:50.054', 2214, '0.0411440469'), ('88', '17:18:50.088', 2216, '0.0411440469'), ('89', '17:18:50.122', 2218, '0.0411440469'), ('90', '17:18:50.158', 2220, '0.0411440469'), ('91', '17:18:50.187', 2222, '0.0411440469'), ('92', '17:18:50.224', 2224, '0.0411440469'), ('93', '17:18:50.260', 2237, '0.0411440469'), ('94', '17:18:50.288', 2239, '0.0411440469'), ('95', '17:18:50.320', 2241, '0.0411440469'), ('96', '17:18:50.355', 2243, '0.0411440469'), ('97', '17:18:50.387', 2245, '0.0411440469')] nearest [('81', '41.14'), ('82', '41.14')]
28.61 exact [('109', '17:19:02.577', 2400, '0.0286091547'), ('110', '17:19:09.229', 2405, '0.0286091547'), ('111', '17:19:09.288', 2407, '0.0286091547'), ('112', '17:19:09.313', 2409, '0.0286091547'), ('113', '17:19:09.345', 2411, '0.0286091547'), ('114', '17:19:09.378', 2413, '0.0286091547'), ('115', '17:19:09.785', 2426, '0.0286091547')] nearest [('109', '28.61'), ('110', '28.61')]
Probe validity Counter({('0', '1', '1', '0', '0', '-1'): 56, ('0', '0', '0', '-1', '-1', '-1'): 2})
Probe cargo transitions:
788 17:16:07.545 obs 1 cargo ('-1.000000', '-1') mirror -1.000000
829 17:16:19.817 obs 3 cargo ('0.000000', '0') mirror 0.089202
894 17:16:23.925 obs 6 cargo ('1.039310', '1') mirror 0.084799
1029 17:16:25.336 obs 7 cargo ('5.435862', '1') mirror 0.084799
1046 17:16:26.222 obs 8 cargo ('5.500000', '1') mirror 0.083921
1405 17:16:41.298 obs 19 cargo ('1.697796', '1') mirror 0.070225
1528 17:16:55.343 obs 20 cargo ('0.000000', '0') mirror 0.069939
1684 17:17:07.117 obs 31 cargo ('0.206810', '1') mirror 0.056321
1797 17:17:08.399 obs 32 cargo ('1.990260', '1') mirror 0.056208
2094 17:17:38.226 obs 45 cargo ('1.456907', '1') mirror 0.041144
2226 17:18:50.224 obs 46 cargo ('0.000000', '0') mirror 0.041144
Transfers grouped by site/resource:
('0x68F792', '0') 25 sum_rounded_tons 0.0 range 17:16:40.347 17:16:42.137
('0x68F792', '1') 66 sum_rounded_tons 7.492000000000002 range 17:16:39.815 17:17:38.823
('0x68F7F9', '1') 66 sum_rounded_tons -7.492000000000002 range 17:16:39.816 17:17:38.824
('0x694F40', '1') 64 sum_rounded_tons -7.496000000000001 range 17:16:23.607 17:17:07.858
('0x6953C4', '1') 64 sum_rounded_tons 7.496000000000001 range 17:16:23.611 17:17:07.859
All sample stretches with constant rounded candidate:
813 1 17:16:19.122 28829250 90.09 drops 0
840 9 17:16:19.893 28839015 89.13 drops 0
842 10 17:16:20.894 28840015 87.99 drops 0
855 11 17:16:21.950 28841078 86.76 drops 0
868 12 17:16:22.953 28842093 85.56 drops 0
909 13 17:16:23.990 28843125 84.80 drops 0
1044 15 17:16:25.987 28845125 84.19 drops 0
1057 16 17:16:28.216 28847343 83.51 drops 0
1070 17 17:16:29.238 28848375 82.34 drops 0
1072 18 17:16:30.272 28849406 81.12 drops 0
1086 19 17:16:31.311 28850421 80.09 drops 0
1099 20 17:16:32.301 28851437 78.93 drops 0
1112 21 17:16:33.309 28852437 77.77 drops 0
1125 22 17:16:34.307 28853437 76.65 drops 0
1127 23 17:16:35.324 28854453 75.43 drops 0
1140 24 17:16:36.315 28855453 74.26 drops 0
1153 25 17:16:37.336 28856468 73.07 drops 0
1166 26 17:16:38.373 28857515 71.90 drops 0
1245 27 17:16:39.520 28858515 70.73 drops 0
1333 28 17:16:40.696 28859515 70.23 drops 0
1526 40 17:16:55.143 28874281 70.16 drops 0
1539 41 17:16:56.152 28875281 69.00 drops 0
1552 42 17:16:57.168 28876312 67.90 drops 0
1565 43 17:16:58.234 28877328 66.69 drops 0
1578 44 17:16:59.223 28878343 65.46 drops 0
1591 45 17:17:00.256 28879390 64.23 drops 0
1593 46 17:17:01.263 28880390 63.05 drops 0
1606 47 17:17:02.285 28881421 61.84 drops 0
1619 48 17:17:03.297 28882421 60.63 drops 0
1632 49 17:17:04.282 28883421 59.45 drops 0
1645 50 17:17:05.298 28884421 58.37 drops 0
1670 51 17:17:06.290 28885421 57.19 drops 0
1715 52 17:17:07.295 28886421 56.32 drops 0
1811 54 17:17:15.786 28888453 56.09 drops 0
1823 60 17:17:16.293 28895421 55.48 drops 0
1825 61 17:17:18.098 28897234 55.27 drops 0
1838 62 17:17:19.098 28898234 54.07 drops 0
1852 63 17:17:20.152 28899265 52.94 drops 0
1865 64 17:17:21.157 28900281 51.75 drops 0
1878 65 17:17:22.144 28901281 50.57 drops 0
1945 66 17:17:23.219 28902296 49.38 drops 0
1958 67 17:17:24.167 28903296 48.17 drops 0
1972 68 17:17:25.174 28904312 46.98 drops 0
1987 69 17:17:33.766 28905312 46.15 drops 0
2003 77 17:17:34.205 28913343 45.60 drops 0
2016 78 17:17:35.212 28914343 44.46 drops 0
2031 79 17:17:36.224 28915359 43.22 drops 0
2044 80 17:17:37.226 28916359 42.06 drops 0
2092 81 17:17:38.226 28917359 41.14 drops 0
2247 98 17:18:50.525 28989656 40.95 drops 51
2260 99 17:18:51.526 28990656 39.77 drops 51
2273 100 17:18:52.521 28991656 38.59 drops 51
2286 101 17:18:53.522 28992656 37.41 drops 51
2299 102 17:18:54.555 28993687 36.18 drops 51
2312 103 17:18:55.558 28994687 35.04 drops 51
2325 104 17:18:56.569 28995703 33.84 drops 51
2348 105 17:18:58.541 28997671 32.73 drops 51
2361 106 17:18:59.570 28998703 31.62 drops 51
2374 107 17:19:00.575 28999718 30.44 drops 51
2387 108 17:19:01.582 29000718 29.28 drops 51
2400 109 17:19:02.577 29001718 28.61 drops 51
Float32 multiplication/format audit:
115/115 arithmetic, validity, membership and sequence assertions PASS
Committed transfer groups enriched from immediately following attribution:
('0x68F792', '0', 'none') 25 0.0 17:16:40.347 17:16:42.137
('0x68F792', '1', 'eletronics') 26 1.992 17:17:37.992 17:17:38.823
('0x68F792', '1', 'food') 40 5.5 17:16:39.815 17:16:42.354
('0x68F7F9', '1', 'eletronics') 26 -1.992 17:17:37.992 17:17:38.824
('0x68F7F9', '1', 'food') 40 -5.5 17:16:39.816 17:16:42.356
('0x694F40', '1', 'eletronics') 27 -1.995 17:17:07.022 17:17:07.858
('0x694F40', '1', 'food') 37 -5.501 17:16:23.607 17:16:25.339
('0x6953C4', '1', 'eletronics') 27 1.995 17:17:07.024 17:17:07.859
('0x6953C4', '1', 'food') 37 5.501 17:16:23.611 17:16:25.341
Measurements 56
Shutdown error lines 38
Images:
image-2.png c45e1d7ebdb015b3a07b7f2214be1344bae521147c715d8a99ba88955c69c488
image-3.png c0634990f1182f3db84a0619dad99dd4a0ef27906b136aafc4b7ef6f83f5a7ec
image-4.png 74e710f870d87b801eb1c301031d348df51259307f2adff5147b9950cd710d7a
image-5.png de607cd09aabcf04332a9fa0e3969d41f1aecee741c09b292e371afe1acc07ea
image-6.png 3eba5b7d13ef35611e71a20614ebd942f34c5847e37179dc560585c5ae23e36b
```


---

## 2026-10-30 implementation checkpoint — diagnostic provenance, targeted writer blocker

### Honest acceptance status

| Ordered item | Status in this checkpoint |
|---|---|
| 1. Tariff/tank/paused-queue diagnostic cleanup | **Implemented and host-tested** in the existing plugin; no MSVC/game validation of this new revision yet. |
| 2. Trace verified road-truck simulation deductions/refills and empty-tank clamp | **Blocked, not completed.** Existing raw renderer/route exports establish display identity, not those simulation writers. Cited simulation dumps and the executable/project are absent from this workspace. |
| 3. Logging-only native journey adapter using confirmed cargo boundaries and verified fuel events | **Not connected.** Do not mistake a cargo-only adapter or display-delta consumer for completion. Default unverified coverage and billing OFF remain. |
| Next real-game test | **Not ready for the requested end-to-end adapter test.** No repeat of the old panel/string investigation is requested. Obtain the targeted static artifact below first. |

Work remained on `arena/0a144a79-wrsre`. For this continuation, actual local HEAD and freshly fetched target `FETCH_HEAD` were both `e2f0e9538bbe335be4be08c8a0061dcec8a5736e`; the fetched tree supplies no additional writer dumps. No checkout, branch creation, merge, commit, push, PR or game deployment was performed. Earlier review text/PR provenance is retained as historical session context, not a claim of a new publish operation in this checkpoint.

### Implemented behavior and diagnostic contract

**Tariff:** startup now calls the legacy tariff a **MODEL per transfer fragment**, explicitly prints billing/dry-run/carrier settings, and says native journey settlement is OFF. `[TARIFF MODEL]` daily totals count `priced_transfer_fragment(s)` and state that their sum is **not journeys or payment totals**. Per-fragment records report `payment_returned`, `billing` and `dry_run`; they no longer assert that a model amount was charged by the state. Actual legacy payment outcomes remain in `[PAY]`. The model accumulators still have their original placement and calculation; they are no longer mislabeled as payments.

**Tank candidate provenance:** `ReadTank` uses the production `fuel_diagnostics.h`, not a detached accounting model. A reading explicitly selects `slot_candidate`, `display_5f0_fallback` or `unavailable`. A readable matching slot with an invalid amount cannot be hidden by a valid display fallback. Fallback is never compared to itself: `drift_valid=0` and no `agree` claim. `slot_found=0` means no matching slot was established by this reader; it is not proof of an absent native engine-fuel inventory. `slot_found=1` is a pointer match to the configured resource, **not** proof that the slot is this truck's canonical engine tank. Both quantities remain candidates with `units=unverified`.

The slot reader now preserves negative/nonfinite raw quantities instead of silently inventing zero. Candidate selection rejects them. Valid zero remains valid zero. Invalid reads, source switches and a signaled load epoch reset sample baselines rather than creating a cross-source/cross-reset decrease. These controls do **not** establish native identity lifetime or complete event coverage; samples remain unverified and are not supplied to the journey tracker. Daily/run records say `decrease_raw`, `sample_increases`, `exact_burn=0`, `verified_refill=0`. The existing legacy aggregate counters retain old variable names, but their logs explicitly describe candidate sample changes and configured-density estimates, not physical tonnes burned. Raw activity words and the old catalogue pointer are now labeled as legacy guesses/candidates; metadata capacity is not claimed as a verified refill clamp.

**One intentional safety restriction:** requesting legacy `litres_auto=1` no longer tunes the tariff from unverified sampled decreases. It logs `auto-tune NOT applied`; the existing default remains 0. This is not a new settlement algorithm. Current billing remains 0/dry-run 1; the legacy money-transfer implementation and dispatch policy are not changed. No candidate/negative-clamp guess is promoted into an exact deduction.

**Paused queue:** the renderer remains post-render, bounded, read-only, allocation-free and try-lock-only; fingerprint/prologue/20-byte-hook checks are unchanged. The public `sim_clock.isPaused` getter is documented as safe from any thread (`src/sim_clock_api.h`, getters section); the render hook does no logging. One queued paused record for the same nonzero root/window may be replaced with its latest snapshot. Other roots/windows and unpaused records are preserved. A 72-capture same-truck pause occupies **one** record, records **71** overwritten snapshots, does not consume 72 accepted-record slots, and produces an explicit `[PANEL GAP] reason=paused_snapshot_coalescing` at drain. This is intentional diagnostic coalescing, not proof of zero simulation activity. First and latest capture wall timestamps are printed.

The queue remains 16, interval 1000 ms and cap 240 **accepted records** per epoch; it was not simply enlarged. Capture time is `ms`, while log-line time is drain time. `queue_full_total_at_capture` belongs to the stored/latest coalesced snapshot; `queue_full_total_at_drain` is explicitly the global counter at drain. `[PANEL GAP] reason=queue_full` reports newly skipped qualified snapshot captures with first/last wall timestamps, without pretending that accepted IDs expose loss. Additional explicit gap/stop reasons are `render_trylock_contention`, `capture_cap`, `guarded_read_fault`. Contention is reported as seen with `skipped_render_attempts=unknown`, not a fabricated exact count. An epoch change discards pending old-epoch records; a guarded read fault disables captures for the process lifetime. If a pause getter is unavailable, coalescing is unavailable and bounded overflow is still explicitly reported. All of this is a **sampled diagnostic stream**, never complete native-event telemetry. `sample_ids=accepted_records_only`, `simulation_event_coverage=unverified` and `exact_burn=0` prevent false continuity claims.

Every INI **value** was compared against the pre-edit tracked configuration and is unchanged. Source/INI test defaults remain observer 1, probe 1 where already provided, billing 0, dry-run 1, source 10%, destination 30%. INI comments were corrected so they no longer claim native slot/clamp/weight independence or measured per-journey physics. No independent-permission, retail delivery-only, reconnect, customs, debt, foreign-restock, target-threshold or task-writing policy was modified.

### Targeted native trace: what the available evidence actually permits

This checkpoint read the existing fuel/road-path/association reports, current native wrapper and reader code, and available raw renderer exports. No new string/gauge search or fresh full-renderer export is required.

| Evidence | What it establishes / does not establish |
|---|---|
| Supplied real-game root/home/fleet match and `5F0 × 1000` display correlation | The opened truck is the observed DO-fleet pointer and its panel values match. Does not prove physical tonnes, a canonical simulation store, or a complete deduction/refill stream. |
| Raw renderer RVA `7D71FD` | Writes literal float bits `0x3C23D70A` (approximately 0.01) to `[R15+5F0]` under UI-side branch conditions. This alone is **not** a simulation deduction or verified refill event. |
| Old `142430` snippet | Describes adding `min(source_available, capacity-current)` to a `5F0` field, but the full raw function/receiver/caller evidence is absent here. Road-truck identity, paired source mutation, authoritative units and hook coverage cannot be accepted from a renamed decompiler parameter. |
| Old `14B920` snippet | Adds a product to a `5F0` field; the same report describes it as building upkeep. No verified selected-truck receiver, actual clamped deduction or safe hook ABI. A negative term is not sufficient proof of actual burn. |
| Old `16E3B0`/`16F831`, slot resource globals/state gates | Actual road-truck test did not establish the slot and logged state0/mode0/kind−1. Association review already flags refuel receiver provenance. These cannot serve as the exact adapter's native event family. |
| Cited other `5F0` writers (`664E00`, `68B330`, `6B3B50`, `6C81D0`, `6DE4A0`, plus building/retail family) | Candidate inventory from prose only, not a verified exhaustive writer census. The actual `68B330` transfer sites did participate in the road-truck cargo run despite an older sea/rail label; do not exclude or accept a path from that label alone. Full raw receiver/writer/clamp evidence is still required. |

Small excerpt from the already available raw renderer (no new renderer investigation):

```asm
1407d71e7: BA 21 00 00 00                MOV EDX,0x21
1407d71ec: 48 8D 0D 9D D9 27 00          LEA RCX,[0x140a54b90]
1407d71f3: FF 15 CF 56 09 00             CALL qword ptr [0x14086c8c8]
1407d71f9: 84 C0                         TEST AL,AL
1407d71fb: 74 0B                         JZ 0x1407d7208
1407d71fd: 41 C7 87 F0 05 00 00 0A D7 23 3C  MOV dword ptr [R15+0x5f0],0x3c23d70a
1407d7208: BA 1D 00 00 00                MOV EDX,0x1d
```

No matching executable, Ghidra project or cited `scratch/write_5f0.txt`, `decomp_142430.c`, `decomp_14b920.c`, `disasm_14b920_5f0.txt`, `d_g2_upkeep.c` or relevant full `6B3B50`/`6C81D0` exports were found in the accessible repository/home workspace. `scratch_clean/` contains scripts, not their generated native evidence. The fresh target fetch added none. The old engine report and `ROAD_PATH_EVIDENCE_COMPLETE.md` now have prominent historical/non-acceptance notices pointing here; original findings are preserved, not silently treated as proven.

**Exact unblock request — not another game run:** provide the matching **SOVIET64.exe** (SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`) so the remaining static trace can be done directly, **or** the targeted Ghidra raw writer export below. Do not ask the user to analyze offsets or implement the adapter themselves. Binary/project artifacts are investigation inputs, not files to commit to Git.

If using the existing Ghidra workspace/another RE agent, this is the bounded request:

> Stop broad panel/string/gauge work. Start from the confirmed actual truck root and its panel backing field `+5F0`; the display scale/identity work is already complete. Supply one self-contained raw simulation-writer export for the matching binary: the full existing `write_5f0` inventory with exact instruction bytes, then complete instructions/decompilation and caller/receiver reaching definitions for actual candidate consumption/refill writers (begin by checking `142430`, `14B920`, `6B3B50`, `6C81D0` and the other inventory hits; do not assume any is a truck). Follow any helper/canonical-store synchronization only as needed to establish the real deduction and refill paths. Distinguish initialization, UI writes, synchronization, actual burn and refills. Show normal and insufficient-fuel paths, actual pre/post state, requested versus actually deducted quantity, zero-floor/empty-tank behavior, paired refill-source/vehicle changes, physical unit conversion and same-update burn/refill ordering. Prove that the native receiver is the verified road-truck object, not an unrelated object sharing `5F0`. Return exact hook-site bytes/boundaries, ABI/register/stack/return semantics, all relevant branches/callers and explicit unresolved coverage. No hooks or payments, no broad gauge/string search, no retry of the full `34D590`/renderer export. Missing evidence stays missing. Append raw findings to this cumulative Markdown, not another report declaring the path complete.

After that evidence is available, implement only verified native event hooks, retain upstream gap/identity/reload invalidation, connect cargo-confirmed boundaries in the existing plugin, leave coverage unverified until complete, keep billing OFF, then prepare the changed real-game test. A sampled-display adapter is explicitly not an acceptable substitute.

### Actual tests, limitations and reproduction

Final regression total: **220 PASS** (71 logistics, 93 probe, 4 shop/debt, 38 historical model, 14 existing scanner regressions). Running existing scanner regressions is not new gauge/string investigation. Native harnesses: **8 provenance + 8 queue/control + 16 reader + 30 tracker scenarios**, each run optimized and again under ASan+UBSan. The tracker is still disconnected; its success is synthetic accounting logic only. The Python tests also extract the **actual production** `FuelSlotTons` and `ReadTank` bodies, compile them with a host memory/service shim and printf checks, and exercise fallback, source switches, negative raw slot quantities, invalid continuity, load-generation baselines and sample increases. The test does not rewrite those functions as a duplicate model.

Test development corrected two source guards for the new loading block/comment spelling, and updated the extraction delimiter when an obsolete source comment was replaced. The final complete runs below passed; these edits are not native-memory validation. Host queue tests use a simulated pause getter, lock and fault; they are not real concurrency, Windows SEH, patch execution or ABI validation. No MSVC compiler/game executable is installed here; **no Windows DLL build or new real-game run is claimed**. Guarded fault and cap behavior are source/host tested only. The supplied older game log still contains the previously reported exit-save errors; these diagnostics do not claim to fix or explain them.

Reproduce native runs with C++17, `-Wall -Wextra -Werror -pedantic`; optimized uses `-O2`. Sanitized uses `-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie` and `ASAN_OPTIONS=detect_leaks=0` in this Linux host. Full actual stdout follows. Generated executables and scratch logs remain ignored in `.cache/`.

When a native-adapter build is actually ready, the Windows handover must give the exact published commit/PR and VS Code pull/merge instructions, `build.bat`/DLL copy steps and expected startup configuration. **Do not present this unpushed, unbuilt diagnostic checkpoint as that ready build.** No additional unchanged panel test is needed now.

#### Final complete regression stdout

```text
$ /usr/bin/python -m unittest discover -s tests/private_logistics -p test_*.py
.......................................................................
----------------------------------------------------------------------
Ran 71 tests in 4.959s

OK
$ /usr/bin/python -m unittest discover -s tests/private_logistics_probe -p test_*.py
.............................................................................................
----------------------------------------------------------------------
Ran 93 tests in 0.167s

OK
$ /usr/bin/python -m unittest discover -s tests/private_shops -p test_*.py
....
----------------------------------------------------------------------
Ran 4 tests in 0.288s

OK
$ /usr/bin/python -m unittest discover -s tests/private_logistics_v018_model -p test_*.py
......................................
----------------------------------------------------------------------
Ran 38 tests in 0.003s

OK
$ /usr/bin/python -m unittest discover -s tests/vehicle_gauge_strings -p test_*.py
..............
----------------------------------------------------------------------
Ran 14 tests in 0.063s

OK
TOTAL: 220 Python regression tests PASS
PASS every INI setting value unchanged; source/INI safety defaults tested
git diff --check: PASS
```

#### Optimized native stdout

```text
=== fuel_diagnostics optimized ===
PASS display_fallback_not_self_comparable
PASS independent_candidate_comparison
PASS invalid_negative_and_nonfinite_not_zero_or_hidden
PASS readable_zero_is_valid_invalid_is_not_zero
PASS changes_are_sample_decreases_and_increases_only
PASS source_changes_reset_baseline_in_both_directions
PASS invalid_read_breaks_sample_continuity
PASS load_epoch_breaks_sample_continuity
8 candidate-provenance scenarios PASS (not native fuel validation)
=== panel_fuel_runtime optimized ===
PASS runtime_install_fingerprint
PASS runtime_post_render_throttle_and_deferred_log
PASS runtime_long_pause_coalesces_and_reports_omissions
PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record
PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard
PASS runtime_inactive_does_not_starve_capture_and_cap
PASS runtime_guarded_fault_disables_and_reports_once
PASS runtime_changed_data_refused
8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)
=== panel_fuel_observer optimized ===
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS
=== journey_tracker optimized ===
PASS boundaries
PASS fuel_excluded
PASS two_journeys
PASS initially_loaded
PASS transfer_before_is_baseline
PASS snapshots_not_transfers
PASS partial_snapshots_and_residue
PASS refill_does_not_cancel_burn
PASS uncertain_evidence
PASS coverage_default_is_unverified
PASS duplicate_completion
PASS duplicate_burn_and_equal_values
PASS gap_empty_payload_not_baseline
PASS sequence_exhaustion
PASS conflicting_duplicate
PASS sequence_gap
PASS sequence_order
PASS explicit_gap
PASS identity_changes
PASS reload_reset
PASS mismatched_cargo
PASS invalid_cargo
PASS invalid_fuel
PASS overflow
PASS invalid_enums
PASS zero_deductions
PASS formatted_output
PASS format_incomplete
PASS interleaved_vehicles
PASS randomized_journeys
```

#### ASan + UBSan native stdout

```text
=== fuel_diagnostics ASan + UBSan ===
PASS display_fallback_not_self_comparable
PASS independent_candidate_comparison
PASS invalid_negative_and_nonfinite_not_zero_or_hidden
PASS readable_zero_is_valid_invalid_is_not_zero
PASS changes_are_sample_decreases_and_increases_only
PASS source_changes_reset_baseline_in_both_directions
PASS invalid_read_breaks_sample_continuity
PASS load_epoch_breaks_sample_continuity
8 candidate-provenance scenarios PASS (not native fuel validation)
=== panel_fuel_runtime ASan + UBSan ===
PASS runtime_install_fingerprint
PASS runtime_post_render_throttle_and_deferred_log
PASS runtime_long_pause_coalesces_and_reports_omissions
PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record
PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard
PASS runtime_inactive_does_not_starve_capture_and_cap
PASS runtime_guarded_fault_disables_and_reports_once
PASS runtime_changed_data_refused
8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)
=== panel_fuel_observer ASan + UBSan ===
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS
=== journey_tracker ASan + UBSan ===
PASS boundaries
PASS fuel_excluded
PASS two_journeys
PASS initially_loaded
PASS transfer_before_is_baseline
PASS snapshots_not_transfers
PASS partial_snapshots_and_residue
PASS refill_does_not_cancel_burn
PASS uncertain_evidence
PASS coverage_default_is_unverified
PASS duplicate_completion
PASS duplicate_burn_and_equal_values
PASS gap_empty_payload_not_baseline
PASS sequence_exhaustion
PASS conflicting_duplicate
PASS sequence_gap
PASS sequence_order
PASS explicit_gap
PASS identity_changes
PASS reload_reset
PASS mismatched_cargo
PASS invalid_cargo
PASS invalid_fuel
PASS overflow
PASS invalid_enums
PASS zero_deductions
PASS formatted_output
PASS format_incomplete
PASS interleaved_vehicles
PASS randomized_journeys
```

### Complete final changed code/configuration and tests (v0.18.7)

These complete files supersede earlier embedded versions of the same paths. Unchanged reader/core code remains in the earlier source appendices/core report; no runtime consumer was added. SHA-256 is of the exact workspace file bytes at this checkpoint.

#### `plugins/private_logistics/fuel_diagnostics.h`

SHA-256: `850492a3e58c46a2a4439dbf700676919890f0076082adce217d8ac5b94b2201`

````cpp
// Legacy candidate readers: provenance and sampled changes ONLY, never burn events.
#ifndef PRIVATE_LOGISTICS_FUEL_DIAGNOSTICS_H
#define PRIVATE_LOGISTICS_FUEL_DIAGNOSTICS_H
#include <cmath>
#include <cstdint>
namespace private_logistics_fuel_diagnostics {
enum class Source { Unavailable, SlotCandidate, DisplayFallback };
enum class Change { Invalid, Baseline, SourceChange, Decrease, Increase, Flat };
struct Reading {
    Source source = Source::Unavailable;
    bool valid = false, comparable = false;
    double value = 0.0, drift = 0.0;
};
inline bool Candidate(double value) {
    return std::isfinite(value) && value >= 0.0 && value <= 1000.0;
}
inline Reading Select(bool slotFound, double slotRaw, double displayRaw) {
    Reading r;
    // A found but INVALID slot must not be hidden by a valid fallback.
    if (slotFound) {
        r.source = Source::SlotCandidate;
        r.valid = Candidate(slotRaw);
        if (r.valid) r.value = slotRaw;
        r.comparable = r.valid && Candidate(displayRaw);
        if (r.comparable) r.drift = displayRaw - slotRaw;
    } else if (Candidate(displayRaw)) {
        r.source = Source::DisplayFallback;
        r.valid = true;
        r.value = displayRaw;
        // Never compare a fallback to itself and call it agreement.
    }
    return r;
}
inline const char* SourceName(Source s) {
    switch (s) {
        case Source::SlotCandidate: return "slot_candidate";
        case Source::DisplayFallback: return "display_5f0_fallback";
        default: return "unavailable";
    }
}
inline const char* ChangeName(Change c) {
    switch (c) {
        case Change::Baseline: return "baseline";
        case Change::SourceChange: return "source_change_baseline";
        case Change::Decrease: return "sample_decrease";
        case Change::Increase: return "sample_increase_not_verified_refill";
        case Change::Flat: return "sample_unchanged";
        default: return "invalid";
    }
}
struct Result { Change change = Change::Invalid; double decrease = 0.0; };
class Samples {
    bool known_ = false;
    std::uint64_t generation_ = 0;
    Source source_ = Source::Unavailable;
    double previous_ = 0.0;
public:
    Result Observe(const Reading& r, std::uint64_t generation) {
        Result result;
        if (!r.valid) { known_ = false; return result; }
        if (!known_ || generation != generation_) result.change = Change::Baseline;
        else if (source_ != r.source) result.change = Change::SourceChange;
        else if (r.value < previous_) {
            result.change = Change::Decrease; result.decrease = previous_ - r.value;
        } else result.change = r.value > previous_ ? Change::Increase : Change::Flat;
        generation_ = generation; source_ = r.source; previous_ = r.value; known_ = true;
        return result;
    }
};
}
#endif
````

#### `plugins/private_logistics/panel_fuel_runtime.h`

SHA-256: `3b667a4219f1f027ff57c837a6b2d2e554c11e9a13ee1d22b66332aeec438105`

````cpp
// Private to this plugin. Included after its services/globals are defined.
// Process-lifetime hooks, like the plugin's existing transfer hooks.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#include "panel_fuel_observer.h"

static CRITICAL_SECTION g_panelQueueLock;
static volatile LONG g_panelEpoch = 1, g_panelStarted = 0, g_panelFault = 0, g_panelContention = 0;
static LONG g_panelQueueEpoch = 0;
static unsigned g_panelHead = 0, g_panelCount = 0, g_panelSamples = 0, g_panelDropped = 0;
static ULONGLONG g_panelLastMs = 0;
static unsigned g_panelDroppedReported = 0;
static ULONGLONG g_panelFirstDropMs = 0, g_panelLastDropMs = 0;
static bool g_panelCapLogged = false, g_panelFaultLogged = false;
struct PanelRecord {
    private_logistics_panel::Snapshot snapshot;
    LONG epoch;
    unsigned sequence;
    ULONGLONG milliseconds;
    ULONGLONG firstMilliseconds;
    unsigned queueFullAtCapture, coalesced;
    bool paused;
};
static PanelRecord g_panelQueue[16];
using PanelRenderer = void (__fastcall*)(void*, void*);
static void* g_panelTrampoline = nullptr;

static void PanelObserverLoading(void)
{
    // File thread: no game reads, queue lock, retained identities or persistence.
    InterlockedIncrement(&g_panelEpoch);
}
static bool PanelRead(uintptr_t address, void* out, size_t size, void*)
{
    if (address < 0x10000 || !ReadablePtr((const void*)address, size)) return false;
    memcpy(out, (const void*)address, size);
    return true;
}
static void PanelResetQueueLocked(LONG epoch)
{
    if (g_panelQueueEpoch == epoch) return;
    g_panelQueueEpoch = epoch;
    g_panelHead = g_panelCount = g_panelSamples = g_panelDropped = 0;
    g_panelLastMs = 0;
    g_panelDroppedReported = 0;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    g_panelCapLogged = false;
    InterlockedExchange(&g_panelContention, 0);
}
static void PanelCapture(void* context, void* window)
{
    if (!InterlockedCompareExchange(&g_panelStarted, 0, 0) ||
        InterlockedCompareExchange(&g_panelFault, 0, 0)) return;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) {
        InterlockedExchange(&g_panelContention, 1); return;
    }
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    const ULONGLONG now = GetTickCount64();
    if (g_panelSamples >= 240 || (g_panelLastMs && now - g_panelLastMs < 1000)) {
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    const bool paused = g_simClock && g_simClock->isPaused && g_simClock->isPaused() == 1;
    const unsigned tail = (g_panelHead + g_panelCount + 15) % 16;
    // Only replace a queued paused snapshot of the SAME window/root. Preserve
    // other vehicles and unpaused samples; no extra storage or render-thread I/O.
    private_logistics_panel::Snapshot snapshot{};
    bool captured = false;
    __try {
        captured = private_logistics_panel::Capture({PanelRead, nullptr},
            (uintptr_t)context, (uintptr_t)window, snapshot);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_panelFault, 1);
    }
    if (captured && epoch == InterlockedCompareExchange(&g_panelEpoch, 0, 0)) {
        g_panelLastMs = now; // inactive windows do not consume the capture slot
        const bool coalesce = paused && snapshot.root && g_panelCount && g_panelQueue[tail].paused &&
            g_panelQueue[tail].snapshot.window == snapshot.window &&
            g_panelQueue[tail].snapshot.root == snapshot.root;
        if (coalesce) {
            auto& record = g_panelQueue[tail];
            record.snapshot = snapshot;
            record.milliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            if (record.coalesced < 0x7FFFFFFFu) ++record.coalesced;
        } else if (g_panelCount == 16) {
            if (!g_panelFirstDropMs) g_panelFirstDropMs = now;
            g_panelLastDropMs = now;
            if (g_panelDropped < 0x7FFFFFFFu) ++g_panelDropped;
        } else {
            auto& record = g_panelQueue[(g_panelHead + g_panelCount) % 16];
            record.snapshot = snapshot;
            record.epoch = epoch;
            record.sequence = ++g_panelSamples;
            record.milliseconds = record.firstMilliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            record.coalesced = 0;
            record.paused = paused;
            ++g_panelCount;
        }
    }
    LeaveCriticalSection(&g_panelQueueLock);
}
static void __fastcall PanelRendererDetour(void* context, void* window)
{
    ((PanelRenderer)g_panelTrampoline)(context, window);
    PanelCapture(context, window); // POST render, not an atomic read at the format call
}
static const char* PanelStatus(private_logistics_panel::Status s)
{
    using private_logistics_panel::Status;
    switch (s) {
    case Status::Complete: return "complete";
    case Status::TooManyLinks: return "too_many_links";
    case Status::NonFinite: return "nonfinite";
    case Status::Overflow: return "overflow";
    default: return "unreadable";
    }
}
static const char* PanelMatch(private_logistics_panel::Match m)
{
    using private_logistics_panel::Match;
    switch (m) {
    case Match::Found: return "exact_fleet_pointer";
    case Match::Absent: return "absent_from_candidate_home";
    case Match::NotRoadOffice: return "candidate_home_not_road_do";
    case Match::InvalidFleet: return "invalid_candidate_fleet";
    default: return "unavailable";
    }
}
static void PanelObserverTick(void*, float, float)
{
    PanelRecord record{};
    bool have = false, cap = false, fault = false, contention = false;
    unsigned dropped = 0, newDrops = 0;
    ULONGLONG firstDropMs = 0, lastDropMs = 0;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    if (g_panelCount) {
        record = g_panelQueue[g_panelHead];
        g_panelHead = (g_panelHead + 1) % 16; --g_panelCount; have = true;
    }
    contention = InterlockedExchange(&g_panelContention, 0) != 0;
    dropped = g_panelDropped;
    newDrops = dropped - g_panelDroppedReported;
    g_panelDroppedReported = dropped;
    firstDropMs = g_panelFirstDropMs; lastDropMs = g_panelLastDropMs;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    cap = g_panelSamples >= 240 && !g_panelCapLogged;
    if (cap) g_panelCapLogged = true;
    fault = InterlockedCompareExchange(&g_panelFault, 0, 0) && !g_panelFaultLogged;
    if (fault) g_panelFaultLogged = true;
    LeaveCriticalSection(&g_panelQueueLock);
    if (epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (fault) Logf("[PANEL GAP] epoch=%ld reason=guarded_read_fault observer=disabled exact_burn=0", epoch);
    if (cap) Logf("[PANEL GAP] epoch=%ld reason=capture_cap cap=240 capture=stopped exact_burn=0", epoch);
    if (contention)
        Logf("[PANEL GAP] epoch=%ld reason=render_trylock_contention skipped_render_attempts=unknown "
             "simulation_event_coverage=unverified exact_burn=0", epoch);
    if (newDrops)
        Logf("[PANEL GAP] epoch=%ld reason=queue_full skipped_snapshot_captures=%u "
             "first_ms=%llu last_ms=%llu queue_full_total_at_drain=%u "
             "sample_ids=accepted_records_only simulation_event_coverage=unverified exact_burn=0",
             epoch, newDrops, (unsigned long long)firstDropMs,
             (unsigned long long)lastDropMs, dropped);
    if (!have || record.epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (record.coalesced)
        Logf("[PANEL GAP] epoch=%ld sample=%u reason=paused_snapshot_coalescing "
             "discarded_paused_snapshots=%u first_ms=%llu latest_ms=%llu "
             "policy=keep_latest_same_window_root simulation_event_coverage=unverified exact_burn=0",
             record.epoch, record.sequence, record.coalesced,
             (unsigned long long)record.firstMilliseconds, (unsigned long long)record.milliseconds);
    const auto& s = record.snapshot;
    Logf("[PANEL FUEL] epoch=%ld sample=%u ms=%llu phase=post_render window=%p root=%p "
         "status=%s aggregate_valid=%d current_raw=%.9g litres_candidate=%.2f "
         "capacity_unadjusted=%.9g ratio_valid=%d ratio=%.9g links=%u "
         "mode=%d metadata_kind=%d flags_read=%d flag8681=%u flag8682=%u "
         "adjustment_read=%d adjustment_needed=%d presentation=%s "
         "queue_full_total_at_capture=%u queue_full_total_at_drain=%u "
         "captured_paused=%d coalesced_snapshots=%u sample_ids=accepted_records_only "
         "physical_fuel=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)s.window, (void*)s.root, PanelStatus(s.status), s.aggregateValid ? 1 : 0,
         (double)s.current, (double)s.litresCandidate, (double)s.unadjustedCapacity,
         s.ratioValid ? 1 : 0, (double)s.ratio, s.links, s.contextMode, s.metadataKind,
         s.flagsRead ? 1 : 0, (unsigned)s.flag8681, (unsigned)s.flag8682,
         s.adjustmentRead ? 1 : 0, s.adjustmentNeeded ? 1 : 0,
         s.flagsRead && (s.flag8681 || s.flag8682) ? "alternate_if_shown" : "unresolved_7c58",
         record.queueFullAtCapture, dropped, record.paused ? 1 : 0, record.coalesced);
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& o = s.objects[i];
        Logf("[PANEL OBJECT] epoch=%ld sample=%u index=%u role=%s object=%p metadata=%p "
             "quantity_read=%d raw5f0=%.9g capacity8684=%.9g candidate_home=%p "
             "membership=%s fleet_index=%u occurrences=%u private_status=not_classified",
             record.epoch, record.sequence, i, i ? "linked" : "root", (void*)o.pointer,
             (void*)o.metadata, o.quantityRead ? 1 : 0, (double)o.raw, (double)o.capacity,
             (void*)o.homeCandidate, PanelMatch(o.match), o.fleetIndex, o.occurrences);
    }
}
static bool PanelTargetMatches(void)
{
    // Fingerprint, not a recomputed SHA-256. Operator must verify the file hash.
    if (!g_exeBase || g_exeSize < 0x90B080 || !ReadablePtr(g_exeBase, sizeof(IMAGE_DOS_HEADER))) return false;
    const auto* dos = (const IMAGE_DOS_HEADER*)g_exeBase;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 || dos->e_lfanew > 0x100000) return false;
    const auto* nt = (const IMAGE_NT_HEADERS64*)(g_exeBase + dos->e_lfanew);
    if (!ReadablePtr(nt, sizeof(*nt)) || nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        nt->FileHeader.TimeDateStamp != 0x6A3EB6ADu ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;
    static const BYTE scale[] = {0x00, 0x00, 0x7A, 0x44};
    static const BYTE format[] = {0x25,0,0x2E,0,0x32,0,0x66,0,0x6C,0,0,0};
    return ReadablePtr(g_exeBase + 0x90B07C, sizeof(scale)) &&
        ReadablePtr(g_exeBase + 0x8F3E98, sizeof(format)) &&
        memcmp(g_exeBase + 0x90B07C, scale, sizeof(scale)) == 0 &&
        memcmp(g_exeBase + 0x8F3E98, format, sizeof(format)) == 0;
}
static void PanelObserverStart(void)
{
    if (!g_panelObserverEnabled) { Logf("[PANEL FUEL] disabled by INI"); return; }
    if (g_billing || !g_simClock || !g_simClock->registerTick || !g_persistenceHandle) {
        Logf("[PANEL FUEL] unavailable: requires billing=0, sim_clock tick and registered load signal"); return;
    }
    if (!PanelTargetMatches()) { Logf("[PANEL FUEL] target/data fingerprint mismatch; no hook installed"); return; }
    // 20 complete bytes through LEA RBP,[RSP-2EB0]; no RIP-relative operand.
    static const BYTE expect[] = {0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,
        0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    if (!ReadablePtr(g_exeBase + 0x7D5460, sizeof(expect)) ||
        memcmp(g_exeBase + 0x7D5460, expect, sizeof(expect))) {
        Logf("[PANEL FUEL] renderer prologue mismatch; no hook installed"); return;
    }
    InitializeCriticalSection(&g_panelQueueLock);
    if (!g_simClock->registerTick(PanelObserverTick, nullptr)) {
        Logf("[PANEL FUEL] tick registration failed; no hook installed"); return;
    }
    if (!InstallInlineHook(g_exeBase + 0x7D5460, (void*)PanelRendererDetour,
                          &g_panelTrampoline, expect, sizeof(expect), "private_logistics panel fuel observer")) {
        Logf("[PANEL FUEL] renderer hook refused; capture unavailable"); return;
    }
    InterlockedExchange(&g_panelStarted, 1);
    Logf("[PANEL FUEL] installed renderer=0x7D5460 stolen=20 scale=1000 format=%%.2fl "
         "interval_ms=1000 cap=240 queue=16 paused_coalescing=%s; logs drain on unpaused ticks; "
         "candidate_home fleet comparison only; accepted sample IDs do not prove coverage; "
         "no exact burn, tracker or billing", g_simClock->isPaused ? "keep_latest_same_window_root" : "unavailable");
}
#endif
````

#### `plugins/private_logistics/private_logistics.cpp`

SHA-256: `c5633ddb3ad409caf5da4abc9b1f050f12b0c4295f0b4d9fe91de4ed716159a4`

````cpp
// private_logistics - what a private company earns, and what it pays.
//
// ---------------------------------------------------------------------------
// v0.18.7: diagnostic provenance and explicit sampled-stream gaps.
// Historical fuel claims below are superseded: sampled changes are NOT burn,
// native fuel units/events remain unverified and the journey adapter is unwired.
//
// v0.18.6 updates: bounded, read-only panel-object/fleet observer.
// Verified 7D5460 prologue, scale and format fingerprints; one sample/second,
// at most 240 per load epoch. No exact burn/tracker/billing integration.
//
// v0.18-permissions: independent state-site directions; consumer sites never
// supply goods; state default deny applies to existing/manual targets too.
// Missing eligible connections are recreated by the company. No fuel/billing
// policy changes in this version. Native direction semantics need game testing.
//
// Historical v0.17 updates (state-storage source opt-in):
//   - Register a storage-panel checkbox through ui_panels' widget API; each
//     public type-5 storage defaults OFF and its permission is persisted by a
//     stable type/name/position key, never by retaining a Building pointer.
//   - When opted in, append a native DO target with load enabled and unload
//     enable/percentage cleared. Turning the checkbox off disables both task
//     directions on that state-storage target; unrelated DO targets are untouched.
//   - The storage builder is hooked by the verified dispatcher call site in
//     ui_panels. Customs remains diagnostic-only; no inventory is manipulated.
// ---------------------------------------------------------------------------
// v0.16 updates (coordinate probe correction):
//   - v0.15 logged a state storage position of (0,0) and customs distance 0.0m.
//     The position helper had read bld+0x08/+0x10 instead of the C3D_NODE
//     translation at bld+0x320+0x30/+0x38 (with placement-bbox fallback).
//     Stable identity keys and nearest-customs diagnostics now use the actual
//     position helper. No customs/state-storage target writes were added.
//
// v0.15 updates (state-storage/customs discovery probe; no public-target writes):
//   - Walk the game-wide building vector to count state storage buildings and
//     report each private road/rail DO's geometrically nearest customs house.
//     This is diagnostic only; neither is appended as a target in this version.
//   - Use game_world.h's canonical demolished flag rather than an unverified
//     raw offset in private-building discovery.
//
// v0.14 updates (automatic private DO targets; staged and disabled by default):
//   1. Once per in-game day, discover registered private DOs (road 43 / rail 52)
//      and private buildings with native storage, using enterprise_base.
//   2. Preserve existing native target entries. Missing eligible targets are
//      reported in dry-run mode by default; optional write mode appends a
//      TaskEntry and mirrors the native add-path TaskCfg defaults (wildcard,
//      100% thresholds, active/validated flags). The owner reports separate
//      load/unload checkboxes in the DO panel; their backing fields and dataflow
//      still need verification before assigning state storage as load-only.
//   3. Generated entries are enabled, deduplicated, and trigger the verified DO
//      dirty flag. Existing entries/configs are never rewritten or removed.
//   4. A session-local identity ledger tracks generated targets by stable
//      keys. Missing eligible targets are recreated; manual removal is not a veto.
//   5. This is a daily callback, not a per-frame hook. Save/load and manual-removal
//      UI behavior remain runtime validation items; write mode ships off.
// ---------------------------------------------------------------------------
// Level 3. Consumes enterprise_base (v8), currency (v4) and sim_clock.
// Optionally consumes resource_pricing for the unit price.
//
// ---------------------------------------------------------------------------
// v0.10 - THE TANK WAS FOUND, AND THE TARIFF WAS FOUR TIMES TOO CHEAP.
//
// veh+0x5F0 is fuel in tonnes. v0.9's field scanner found it by watching which
// words move, and the v0.9 run confirmed it five ways: it falls while the truck
// works, it runs to zero and jumps at a refill, it tops out at 0.46 (the
// player's own 456 LITRE tank at 1 kg/L), the refills reconcile to the diesel
// the company paid for, and nothing else in the vehicle moves that way.
//
// With the tank readable, the burn stops being a guess:
//
//     2.05 t over 51 days, two active vehicles, 139 deliveries
//     = 0.01475 t per delivery = 14.75 L at 1 kg/L
//
// v0.9 shipped 3.30 L/trip, derived from PURCHASES over trips. The company was
// being paid 1.06 per delivery against a real fuel cost of 1.96 - it lost money
// on every trip and the tariff, not the economy, was doing it.
//
// v0.10 reads every vehicle's tank at every close and reports
//
//     [TANK] day N   v=... fuel 0.4300 t | burned 0.0220 t | refills 0
//     [TANK] run: burned 2.0500 t = 2050.0 L over 139 delivery(ies) = 14.75 L/trip
//
// Refills are counted, never subtracted from burn. This is the first figure in
// the whole feature that is neither a purchase nor a model.
//
// ALSO: diesel_density 0.00084 -> 0.00100. The game stores 1 kg per litre; the
// old constant was real-world diesel and made every litre 16 % too cheap.
//
// ---------------------------------------------------------------------------
// v0.9 - FIND THE TANK.
//
// The player asked the right question: how do we know how much fuel a trip
// burnt? We do not. Two runs measured it by dividing the company's diesel
// PURCHASES by its trips and got 3.27 L/trip over 1,100 trips and 7.14 L/trip
// over 214 trips - a factor of two, because fuel sits in tanks between
// purchases. Purchases are not burn, and no amount of averaging makes them so.
//
// What is needed is the fuel going INTO a vehicle, or the level inside it. The
// vehicle record is reachable (bld+0xC70, proven) and the transfer hook never
// shows a fuel movement, so the tank is a FIELD inside the vehicle, not an
// event we can already see.
//
// This build finds it empirically instead of guessing:
//   - the first sighting of a vehicle is stored as a baseline and not logged;
//   - every day after, the same 512 dwords at veh+0x000..0x800 are compared,
//     and ONLY the words that changed are logged, as offset, old, new.
//
// A tank shows up as a word that falls while the truck drives and jumps when
// it refuels. Position, speed and timers will show up too - the offsets tell
// them apart. Nothing is priced from this; it is how the next tariff stops
// being a fleet average.
//
// ALSO IN v0.9:
//   - the [FUEL] run line now says PURCHASES and not burn, because that is
//     what it measures.
//   - the ini learned its section header back. v0.7's rewrite dropped
//     [private_logistics], so the host read nothing and every key fell back to
//     its compiled default - which is why the last two runs were dry runs.
//
// ---------------------------------------------------------------------------
// v0.8 - SEE THE VEHICLES BEFORE PRICING THEM.
//
// v0.7's trip tariff charged every delivery the same litres. That is one
// arbitrary vehicle imposed on a whole fleet, and it cannot survive: vehicles
// differ in capacity, speed, price and consumption, and a tariff that ignores
// it will overpay a scooter and bankrupt a heavy hauler. H41.
//
// The v0.7 run shows the transfer hook cannot supply the difference: 3,065
// site lines resolve to exactly TWO vehicle-side pointers, one of them NULL.
// The fleet has to be read where the game keeps it - bld+0xC70/+0xC78, proven
// by three independent readers (Part 12) - and each vehicle's own record read
// from there:
//
//     veh+0x4F8  Building*  home building                      proven
//     veh+0x3B8  vec        cargo, stride 0x10 (Res*, float t)  proven
//     veh+0x510  ?          candidate catalogue entry           UNPROVEN
//
// This build adds the census: once a day, every private enterprise's fleet is
// walked, each vehicle is logged with what it carries and what it can carry at
// peak, and the first sighting of each vehicle dumps 32 bytes at the pointer
// the game gives for its catalogue entry. That is the lead for capacity,
// speed, price and consumption - the four numbers the final model needs and
// the four this build refuses to invent.
//
// The tariff is unchanged from v0.7. It stays a fallback until the census
// says what the fleet actually is.
//
// ALSO IN v0.8:
//   - litres_per_trip 5.00 -> 3.30. The v0.7 meter measured the DO's diesel
//     bill against its own deliveries: 406.40 RUB over 1,100 trips =
//     3.267 L/trip at ~128/RUB. Not a guess, and not 8.63 either - that
//     figure counted tank fill, exactly as the player said it would.
//   - the [FUEL] run line no longer claims "model and bill agree" when the
//     bill is zero and there is nothing to compare.
//
// ---------------------------------------------------------------------------
// v0.6 - THE TRIP IS PAID FOR BY THE TRIP, AND THE SHARE IS ONLY A SHARE.
//
// Session 7 ran v0.5 with fuel switched on and produced the number the whole
// feature needed: the private DO's account reconciles exactly to
//
//     10,000.00 + 738.99 freight - 445.37 fuel - 0.02 = 10,293.60
//
// so the game DOES charge a private distribution office for diesel, and it
// arrives as a cat=2 (FOREIGN) debit on its own company capital. H37 closed.
// H33 closed too: 444 sampled payments, all moving the treasury to the kopeck.
//
// The same log indicted the payment model. Per delivery, against a measured
// fuel cost of 0.34 RUB:
//
//     plants     282 deliveries   0.29 each   0.9x the fuel bill
//     food       127 deliveries   2.13        6.3x
//     meat        16 deliveries   2.37        7.0x
//     fabric      19 deliveries   3.80       11.2x
//     chemicals   45 deliveries   6.13       18.0x
//
// 58 % of the DO's work was selling at a LOSS and 9 % was paying for it. A
// percentage of cargo value is not a tariff, it is a lottery. Real freight is
// cost recovery plus a margin; only the handling of valuable cargo is priced
// ad valorem, and it is a small part of the bill.
//
// v0.6 therefore models the tariff as what it has to be:
//
//     tariff = fuel_cost x (1 + margin) + tariff_share x cargo_value
//
// Cost recovery pays the trip, the share pays for the cargo being worth
// carrying. litre counts come from session 7 (see litres_per_trip in the ini).
//
// Still probe only. Nothing here is charged. The 8 % share keeps running so
// the two can be read side by side for one more session.
//
// Also fixed: the index and the treasury were read at plugin start, before the
// world is loaded, so every log opened with "fuel NOT FOUND" and "treasury
// opens at 0.00" that were both false a second later. Both are now re-read and
// printed at every day close.
//
// ---------------------------------------------------------------------------
// v0.5 - MEASURE THE COST BEFORE PRICING IT.
//
// v0.4 moved money and the money arrived (RE_DIGEST Part 19). What the same
// session also showed is that the private DO has NO COST SIDE: its company
// account took 646 credits over seven days and paid out nothing at all. Fuel,
// upkeep and vehicles never touched its books. So the 8 % freight is not a
// tariff, it is a subsidy, and "is 8 % enough to cover fuel" cannot be asked
// until a cost exists to compare it against.
//
// This build therefore charges nothing new and measures two things:
//
//   1. THE TARIFF MODEL. A price built the way real freight is built - cost
//      recovery plus a margin - and indexed to a live price, never to a
//      rouble constant. A constant is exactly what inflation eats:
//
//          litres  = litres_per_trip + litres_per_tonne x tonnes
//          cost    = litres x diesel_density x fuel_price      (live)
//          tariff  = cost x (1 + margin)
//
//      If no fuel price resolves (fuel_resource unknown, resource_pricing
//      absent), the same cost is expressed in daily survival baskets via the
//      CPI tail, so the fallback is indexed too. If neither resolves the
//      probe reports "unindexed" and prices nothing - it never guesses a
//      figure in currency units.
//
//   2. THE MONEY PATH, both sides of it. [BAL] prints the treasury and every
//      carrier's company capital at each day close. v0.4 proved the company
//      side to the kopeck but never read the payer (H33); the delta between
//      two closes is that proof, and the difference between a carrier's
//      capital change and the freight it was paid is its fuel bill (H37).
//
// Nothing here charges the probe. `tariff_probe = 1` logs, it does not pay.
// The 8 % share keeps running exactly as v0.4 left it, so a session produces
// three numbers side by side: modelled tariff, freight actually paid, and the
// vehicle's real fuel consumption as the game charged it.
//
// v0.4 also carried a stale banner - it opened every log with "v0.3" - which
// cost the last reader a minute. Version now comes from PL_VERSION once.
//
// ---------------------------------------------------------------------------
// v0.4 - FREIGHT IS A SHARE OF CARGO VALUE, AND THE MONEY MOVES.
//
// v0.3 paid a flat per-tonne rate (1.50 RUB/t). Measured against the live
// session that is 0.31 % of what the cargo is worth - a private distribution
// office could not cover a truck, and FreightRateFor() ignored the resource
// anyway, so the liquid and goods rates were dead keys.
//
// v0.4 replaces the rate with freight_share, a fraction of the cargo's own
// value:
//
//     freight = tons x unitPrice x freight_share          (default 0.08)
//
// A typical load is ~0.12 t of fabric worth ~58 RUB, so this pays ~4.70 RUB
// instead of ~0.18. It scales with what is actually moved, it needs no
// commodity-class table, and when Ship 2 makes the BUYER pay instead of the
// state, the same formula becomes the wholesaler's margin with no rewrite.
//
// v0.4 also:
//   - refuses a payment the treasury cannot afford (getTreasuryBalance),
//     because moveTreasury takes a signed amount and will happily go negative;
//   - splits `skipped` into the four things it was conflating;
//   - labels the daily line with the day it actually covers (it was one
//     behind - the callback fires at the START of the day).
//
// ---------------------------------------------------------------------------
// v0.3 - THE FIRST MONEY.
//
// `dry_run = 1` by default: this build COMPUTES every payment and logs it as
// "[PAY] would", but moves nothing. Set dry_run = 0 to actually pay.
// Nothing else in the plugin changes when you flip it.
//
// The money model (PLAN_TECHNICAL.md sections 6 and 7):
//
//   H1 - A BUILDING IS PAID FOR GOODS THAT LEAVE IT
//        at a mode-0 call with tons < 0, param_2 is the source Building.
//        If that building is a private company, it is credited
//        |tons| x price x factory_share, and the treasury pays.
//
//   H2 - A CARRIER IS PAID FREIGHT FOR A DELIVERY
//        at a mode-0 call with tons > 0, param_2 is the destination Building
//        and param_9 is the fleet owner. If the owner is a private company
//        and is not the destination, it is credited
//        tons x price x freight_share.
//
//   Nothing is billed at a mode-1 site: those are the vehicle side of the
//   same movement and would double-charge it.
//
// ---------------------------------------------------------------------------
// What the two live sessions established (RE_DIGEST Parts 10, 12 and 13)
// ---------------------------------------------------------------------------
//
//   - THE COMMIT TEST IS `(unsigned char)r == 1`. The transfer returns a bool
//     in AL; the upper 24 bits of EAX are register garbage. v0.1 tested the
//     whole int and discarded exactly half the committed calls.
//
//   - `param_2` IS A Building* AT MODE 0 AND A Vehicle* AT MODE 1. That is
//     the whole of attribution. At mode 0 the value also appears in
//     enterprise_base's "[CAP] ... by=wholesale" lines, which only ever take
//     a building; vehicle offsets read off it return garbage.
//
//   - `param_9` IS THE FLEET OWNER, not an endpoint: constant across two
//     different destination buildings, and equal to the mode-1 vehicle's
//     `home` field in every call.
//
//   - `(veh+0x4F0)` IS NOT AN ENDPOINT. It was NULL in all 342 calls v0.1
//     measured and all 373 of v0.2's, because at mode 0 we were reading
//     vehicle fields off a building. Nothing here uses it.
//
//   - A MOVEMENT IS TWO CALLS 30-70 ms APART, same resource, same tonnage:
//       0x694F40  mode 0  -tons   the BUILDING the goods leave
//       0x6953C4  mode 1  +tons   the VEHICLE they enter
//       0x68F792  mode 0  +tons   the BUILDING they enter
//       0x68F7F9  mode 1  -tons   the VEHICLE they leave
//     The load leg's mode-0 call has param_9 == NULL, so its carrier is
//     resolved from the paired mode-1 call through a short time ring.
//
// ---------------------------------------------------------------------------
// The hook is a call-site patch, NOT an inline hook
// ---------------------------------------------------------------------------
//
// FUN_140420350 has a five-byte prologue, so the loader's installInlineHook
// (which requires >= 14 position-independent bytes) cannot take it. All
// sixteen invocations are direct `call rel32` with no vtable, thunk or
// indirect path, so retargeting the call sites sees every invocation.
//
//   site before:  E8 <rel32>          call FUN_140420350
//   site after:   E8 <rel32>          call cave          <- SAME opcode
//   cave:         FF 25 00 00 00 00   jmp qword ptr [rip+0]
//                 <8-byte detour address>
//
// The opcode deliberately stays E8. A jmp does not push a return address, so
// a detour entered that way both reads garbage from _ReturnAddress() and
// executes `ret` into the enclosing function's caller. Verified on x86-64.
//
// Safety properties:
//   - Nothing is written unless the site verifies: byte 0 must be E8 AND its
//     rel32 must resolve to exeBase+0x420350.
//   - Every game-memory read is behind ReadablePtr.
//   - The ledger is a fixed ring; nothing allocates in the detour.
//   - The lock covers counters and the rings only. Game-memory reads, calls
//     into other plugins and logging all happen outside it.
//   - A payment needs BOTH services. If enterprise_base or currency is
//     missing, billing is skipped entirely and says so once.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "site_permissions.h"
#include "task_audit.h"
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"
#include "fuel_diagnostics.h"

#include <stdint.h>
#include <math.h>
#include <new>

// ---------------------------------------------------------------------------
// the transfer being instrumented
// ---------------------------------------------------------------------------

#define TSM_RVA_TRANSFER 0x420350

typedef int (*tTransfer)(void* game, void* veh, unsigned mode, void* res,
                         float tons, float* outMoved, float cap,
                         unsigned char flag, void* bld);

static tTransfer g_realTransfer = nullptr;

// Vehicle fields. See PLAN_TECHNICAL.md section 3.1.
#define TSM_VEH_HOME    0x4F8   // Building* - the owning depot (mode 1 only)

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static int   g_enabled         = 1;
static int   g_billing         = 0; // Permissions-test default: no freight payments.
static int   g_dryRun          = 1;      // SAFE DEFAULT: compute, log, do not pay
static int   g_payFactory      = 1;
static int   g_payCarrier      = 1;
static int   g_logCapPerDay    = 400;
static int   g_ledgerCapacity  = 512;
static int   g_siteLog         = 1;
// v0.4: ONE number, a share of the cargo's value. The three per-tonne rates
// are gone: the class table never existed (FreightRateFor ignored its
// argument) and a player cannot reason about "RUB per tonne of coal".
static float g_freightShareRetired = 0.0f; // set when the ini still carries it
static float g_freightShare    = 0.08f;  // v0.7: RETIRED. Kept only so the
                                         // ini key can be recognised and warned about.
static float g_factoryShare    = 0.90f;  // w - what a buyer pays a factory

// v0.7: THE TRIP TARIFF REPLACES THE VALUE SHARE.
//
// v0.6 measured the value share against the DO's real fuel bill and lost:
// 14,038.20 RUB of freight against 511.92 of diesel over 82 days. The share
// is 27x the cost it was supposed to cover, and it is value-blind - a truck
// hauling plants was paid 1.46 for the trip, one hauling chemicals 71.12.
//
// v0.7 charges the model that was only probed in v0.6; v0.6's behaviour is
// one key away (`trip_tariff = 0`), so both can be compared in one session.
static int    g_tripTariff      = 1;
static double gWearLitres       = 5.0;   // maintenance reserve, in litres of diesel
static int    g_litresAuto      = 0;     // re-tune litres_per_trip from the fuel bill

// v0.7: the measured fuel bill. This is the only figure in the plugin that
// comes from the game instead of from arithmetic - the [TARIFF] model is an
// assumption, the DO's monthly foreign-cost total is the bill.
//
// Fuel is bought in bulk and stored, so a day's purchases are NOT a day's
// burn. Run averages also do not establish exact native journey burn.
// v0.8: the vehicle census. A republic has tens of vehicles, not thousands;
// 64 slots is four times the largest fleet seen so far and keeps the table in
// static storage, as the render/tick rules require.
#define VEH_MAX 64
static void*    g_vehPtr[VEH_MAX];
static double   g_vehPeakTons[VEH_MAX];
static unsigned g_vehSamples[VEH_MAX];
static int      g_vehCount   = 0;
static int      g_censusLog  = 1;
// v0.9: the field scanner. 512 dwords (0x800 bytes) per vehicle, compared once
// a day; only changes are logged. Static storage, no allocation.
#define VEH_SCAN_WORDS 512
static unsigned g_vehSnap[VEH_MAX][VEH_SCAN_WORDS];
static bool     g_vehSnapValid[VEH_MAX];
// Legacy candidate sampling. Physical units, native deduction/refill writers
// and activity gates are NOT verified for the selected road truck. Variable
// names retain old units for compatibility; logs explicitly say raw/estimate.
static double   g_vehFuel[VEH_MAX];          // last valid raw candidate
static bool     g_vehFuelValid[VEH_MAX];
static double   g_burnTonsRun   = 0.0;       // sum of candidate sample decreases
static unsigned g_refillsRun    = 0;
// Historical denominators retained for candidate diagnostics, not physics.
static unsigned g_tankDaysRun    = 0;        // closes with at least one tank read
static unsigned g_tankVehDaysRun = 0;        // (vehicle, close) pairs
static int      g_cargoDump      = 1;        // [CARGO] - settle where fuel lives
static int      g_tankCrossLog   = 40;       // v0.13: [TANK] slot-vs-mirror lines
static private_logistics_fuel_diagnostics::Samples g_vehFuelSamples[VEH_MAX];
static volatile LONG g_fuelDiagnosticEpoch = 1;
static unsigned char g_vehCapValid[VEH_MAX];
static double        g_vehCap[VEH_MAX];      // metadata capacity candidate, raw

// Legacy state-word guesses only; never gate native events using these.
static unsigned g_activeVehDaysRun = 0;
static unsigned g_idleVehDaysRun   = 0;

// Legacy fleet endpoint sampling. It cannot prove individual fuel events.
static const int VEH_FUEL_OFF = 0x5F0;     // panel backing candidate; physical units unverified

static void*    g_probeBld      = nullptr;
static double   g_probeFuelTons = -1.0;
static int      g_probeVehCount = -1;
static double   g_probeBurnRun  = 0.0;
static unsigned g_probeStepsRun = 0;
static unsigned g_probeSkipRun  = 0;
static double   FleetFuelTons(void* b, int* outCount);
static void     BurnProbe(void* b);
static int      g_vehScan          = 1;
static int      g_vehScanLogPerDay = 40;
static void     CensusFleets(int closedDay);   // v0.8, defined with the census
static void     ScanVehicle(int slot, void* v, int closedDay);   // v0.9
static void     ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills);   // v0.10

static double             g_fuelPaidRun  = 0.0;
static double             g_fuelLitresRun = 0.0;
static unsigned long long g_fuelTripsRun = 0;

// Earlier slot/clamp/activity attribution is superseded by the real-game review.
// v0.5: the cost side, in physical units. No rouble constant may appear here:
// prices move, litres and ratios do not.
static int    g_tariffProbe     = 1;       // compute + log the tariff, never charge
static int    g_balanceLog      = 1;       // [BAL] treasury + carrier capital per day
static int    g_tariffLogPerDay = 60;      // per-delivery [TARIFF] lines, then totals only
static double gLitresPerTrip    = 14.75;   // historical model parameter, NOT verified journey burn
// Retained legacy model knob; actual cargo-load dependence is unverified.
static double gLitresPerTonne   = 0.0;     // legacy model default; not native physics
static double gTariffMargin     = 0.35;    // dimensionless: 35 % over fuel cost
static double gDieselDensity    = 0.00100; // legacy model t/L, NOT proof of native mass units
static char   gFuelResource[64] = "fuel";  // resolved through resource_pricing
static double gBasketsPerTrip   = 0.80;    // CPI fallback, in daily survival baskets
static double gBasketsPerTonne  = 0.02;

#define PL_VERSION "0.18.7-diagnostic-provenance"
static int g_panelObserverEnabled = 1;
static void PanelObserverLoading(void);
static float g_minPayment      = 0.01f;
static double g_dailyPayCap    = 0.0;    // 0 = no cap

static const TsmEnterpriseBaseApi*  g_enterprise = nullptr;
static const TsmSimClockApi*        g_simClock   = nullptr;
static const TsmCurrencyApi*        g_currency   = nullptr;
static const TsmResourcePricingApi* g_pricing    = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persistenceHandle = 0;
static const TsmUiPanelsApi*         g_uiPanels = nullptr;
static int                           g_stateStorageWidgetHandle = 0;

// Automatic DO-target generation. Fixed buffers only; discovery runs from the
// once-per-day sim_clock callback, never from a per-frame/render hook.
static int g_autoTargetsEnabled = 1;
static int g_autoTargetsDryRun  = 0;
static int g_autoSourcePercent = 10;
static int g_autoDestinationPercent = 30;
static int g_autoTargetLogLimit = 80;
static volatile LONG g_autoResetActiveMemoAfterLoad = 0;
#define AUTO_ENTITY_MAX 8704
#define AUTO_MEMO_MAX   262144
#define AUTO_MEMO_HASH_SIZE 524288
#define AUTO_POINTER_HASH_SIZE 32768
#define AUTO_OFFICE_HASH_SIZE 16384
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char isStateStorage; unsigned char statePermission; unsigned char targetPresent; };
struct AutoMemo { uint64_t officeKey; uint64_t targetKey; unsigned nextOffice; unsigned char state; };
struct AutoPointerSlot { void* building; unsigned entityIndex; };
struct AutoOfficeMemoHead { uint64_t officeKey; unsigned headPlusOne; };
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static AutoMemo   g_autoMemo[AUTO_MEMO_MAX];
static unsigned   g_autoMemoHash[AUTO_MEMO_HASH_SIZE]; // index + 1; zero means empty
static AutoPointerSlot g_autoPointerHash[AUTO_POINTER_HASH_SIZE];
static AutoOfficeMemoHead g_autoOfficeMemoHeads[AUTO_OFFICE_HASH_SIZE];
static unsigned   g_autoMemoCount = 0;
static unsigned   g_autoLogLines = 0;
static int        g_autoLastDay = -1;
static void AutoTargetsOnDay(int day, int closedDay);
static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void* userData);

// ---------------------------------------------------------------------------
// the sixteen call sites
// ---------------------------------------------------------------------------
//
// `known` records what the RE rounds established. The instrument does not act
// on the label - every site is logged the same way - but billing only ever
// fires at a MODE 0 site, and `known` is what makes the log readable.
//
// After the first two sessions the FUN_14068B330 family is no longer
// described as "sea/rail": it carries ordinary factory traffic.

enum SiteClass {
    SITE_UNKNOWN = 0,
    SITE_REFUEL,
    SITE_LOAD,          // draws goods out of a building
    SITE_DELIVERY,      // puts goods into a building
    SITE_TRANSSHIP      // intra-vehicle / return leg
};

struct SiteSpec {
    unsigned    rva;
    SiteClass   known;
    const char* note;
};

#define MAX_SITES 16

static SiteSpec g_sites[MAX_SITES] = {
    { 0x00141F33, SITE_UNKNOWN,   "never observed" },
    { 0x0016F831, SITE_REFUEL,    "refuel call - telemetry, never billed; never observed" },
    { 0x001B2898, SITE_UNKNOWN,   "CONTESTED: r8-9 'road delivery', r10 'road refuel'; never observed" },
    { 0x001B484B, SITE_UNKNOWN,   "never observed" },
    { 0x001B5EC0, SITE_UNKNOWN,   "never observed" },
    { 0x001B653B, SITE_UNKNOWN,   "never observed" },
    { 0x001B6833, SITE_UNKNOWN,   "never observed" },
    { 0x001B75ED, SITE_UNKNOWN,   "never observed" },
    { 0x001B7981, SITE_UNKNOWN,   "never observed" },
    { 0x001CB7FF, SITE_LOAD,      "loader: mode 0, +tons, then bld+0x970 storage-add; never observed" },
    { 0x0068EECA, SITE_LOAD,      "family A: mode 0, +tons, cargo-add follows; never observed" },
    { 0x0068F792, SITE_DELIVERY,  "family A DELIVERY: mode 0, +tons, param_2 = building, param_9 = fleet owner" },
    { 0x0068F7F9, SITE_TRANSSHIP, "family A delivery, vehicle side: mode 1, -tons. NEVER BILLED" },
    { 0x006945B3, SITE_TRANSSHIP, "family A: mode 0, -tons; never observed" },
    { 0x00694F40, SITE_LOAD,      "family A LOAD: mode 0, -tons, param_2 = the building the goods leave" },
    { 0x006953C4, SITE_TRANSSHIP, "family A load, vehicle side: mode 1, +tons. NEVER BILLED" }
};

static bool g_sitePatched[MAX_SITES] = { false };
static bool g_siteWanted[MAX_SITES]  = { false };   // filled in by ReadSettings
static bool g_siteFilterActive       = false;       // false = every site

static int  g_siteCount = MAX_SITES;

// ---------------------------------------------------------------------------
// the ledger
// ---------------------------------------------------------------------------

enum ShipKind { SHIP_LOAD = 0, SHIP_DELIVERY, SHIP_FREIGHT, SHIP_FUEL, SHIP_PURCHASE };

struct Shipment {
    unsigned id;
    unsigned seq;
    void*    building;
    void*    counterparty;
    void*    vehicle;
    void*    resource;
    float    tons;
    double   unitPrice;
    double   value;
    unsigned short site;
    unsigned char  kind;
    unsigned char  mode;
    double   t;
};

#define LEDGER_MAX 2048
static Shipment g_ledger[LEDGER_MAX];
static int      g_ledgerHead  = 0;
static unsigned g_ledgerSeq   = 0;
static unsigned g_ledgerTotal = 0;

// ---------------------------------------------------------------------------
// the carrier ring
// ---------------------------------------------------------------------------
//
// The load leg's mode-0 call does not carry the carrier: param_9 is NULL
// there (91 of 91 calls). The vehicle is named 30-70 ms later by the paired
// mode-1 call. This ring is how the two are joined.
//
// A match needs the same resource, the same tonnage within 0.5 %, and an age
// under 2 s. If nothing matches, the payment is skipped and counted - never
// guessed.

#define CARRIER_RING 32

struct CarrierNote {
    void*  res;
    float  tons;      // absolute
    void*  home;      // the fleet owner
    double t;
};

static CarrierNote g_carrier[CARRIER_RING];
static int         g_carrierHead = 0;

static double NowSeconds(void)
{
    // GetTickCount64 is monotonic and needs no CRT.
    return (double)GetTickCount64() / 1000.0;
}

// ---------------------------------------------------------------------------
// counters
// ---------------------------------------------------------------------------

static unsigned g_calls            = 0;
static unsigned g_callsCommitted   = 0;
static unsigned g_callsNoop        = 0;
static unsigned g_unknownBuilding  = 0;
static unsigned g_staging          = 0;
static unsigned g_nonPrivate       = 0;
static unsigned g_siteLoggedToday  = 0;
static unsigned g_attrLoggedToday  = 0;
static unsigned g_payLoggedToday   = 0;
static unsigned g_siteSuppressed   = 0;
static unsigned g_attrSuppressed   = 0;
static unsigned g_paySuppressed    = 0;

static unsigned g_payCount         = 0;
// v0.4: `skipped` used to be one number covering four unrelated causes. In
// the fourth session it read 159 in a day purely because the distribution
// office was state-owned - alarming to read, and meaningless.
static unsigned g_skipNoPayee      = 0;   // no private owner on the leg
static unsigned g_skipNoPrice      = 0;   // resource_pricing gave no price
static unsigned g_skipSmall        = 0;   // below min_payment
static unsigned g_skipTreasury     = 0;   // the state could not afford it

// v0.5: the tariff probe's own tallies, and which index it managed to use.
// An unindexed delivery is NOT a delivery worth 0.00 - it is one this build
// could not price, and the daily line says so rather than folding it in.
static unsigned g_tariffCount      = 0;
static unsigned g_tariffNoIndex    = 0;
static unsigned g_tariffFuelIndex  = 0;
static unsigned g_tariffCpiIndex   = 0;
static double   g_tariffCostSum    = 0.0; // the fuel-recovery half
static double   g_tariffShareSum   = 0.0; // the ad valorem half
static unsigned g_tariffLoggedToday= 0;
static double   g_tariffSum        = 0.0;

// v0.5 (H33): the payer leg, sampled per payment. A daily treasury delta is
// useless as proof - wages, taxes and store sales move the same balance all
// day long. Sampling either side of OUR OWN move isolates it completely: the
// difference must be exactly minus the amount, or the books do not close.
// v0.5: the refuel site (0x16F831) has never fired in any session. If the
// game is going to start charging for fuel, this is where we see it first.
static unsigned g_fuelEvents       = 0;
static unsigned g_treasuryChecked  = 0;
static unsigned g_treasuryExact    = 0;
static unsigned g_treasuryOff      = 0;
static double   g_treasuryMaxGap   = 0.0;

// v0.5: the money path, both ends. The treasury is sampled at each close; the
// carriers we paid are remembered so their capital can be printed beside it.
#define BAL_CARRIERS 4
static double g_treasuryAtClose    = 0.0;
static bool   g_treasuryHaveClose  = false;
static void*  g_balCarrier[BAL_CARRIERS]     = { nullptr, nullptr, nullptr, nullptr };
static double g_balCarrierPrev[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 };
static double g_balCarrierIn[BAL_CARRIERS]   = { 0.0, 0.0, 0.0, 0.0 };
static double g_balCarrierFuel[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 }; // v0.7 meter,
                                                                    // -1 = no baseline yet
static int    g_balCarrierCount    = 0;
static void*  g_fuelRes            = nullptr;   // cached lookup, re-resolved daily

// v0.4: which day the accumulated tallies belong to. OnDay fires at the START
// of `day`, so the window it reports is the day that just ENDED.
static int g_summaryDay            = -1;
static unsigned g_payCapped        = 0;
static double   g_paidFactory      = 0.0;
static double   g_paidCarrier      = 0.0;
static double   g_paidToday        = 0.0;

static double   g_wouldPayFreight  = 0.0;
static double   g_wouldPayFuel     = 0.0;

static int      g_lastMonth        = -1;

// Per-site tallies, so "never fired" is distinguishable from "never logged".
struct SiteTally {
    unsigned calls;
    unsigned committed;
    double   tonsIn;      // sum of +tons
    double   tonsOut;     // sum of |−tons|
    double   value;
    unsigned sawBld;      // param_9 non-NULL
    unsigned sawCur;      // *(veh+0x4F0) non-NULL
    unsigned sawHome;     // *(veh+0x4F8) non-NULL
};

static SiteTally g_tally[MAX_SITES];

static int SiteIndex(unsigned rva)
{
    for (int i = 0; i < g_siteCount; i++)
        if (g_sites[i].rva == rva) return i;
    return -1;
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static bool IsPrivate(void* b)
{
    if (!b || !g_enterprise) return false;
    if (!TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return false;
    return g_enterprise->isBuildingPrivate(b) != 0;
}

static const char* KindName(unsigned char k)
{
    switch (k) {
        case SHIP_LOAD:     return "load";
        case SHIP_DELIVERY: return "unload";
        case SHIP_FREIGHT:  return "freight";
        case SHIP_FUEL:     return "fuel";
        case SHIP_PURCHASE: return "buy";
        default:            return "?";
    }
}

static const char* ClassName(SiteClass c)
{
    switch (c) {
        case SITE_REFUEL:    return "refuel";
        case SITE_LOAD:      return "load";
        case SITE_DELIVERY:  return "delivery";
        case SITE_TRANSSHIP: return "transship";
        default:             return "unknown";
    }
}

// The resource record's name lives at its head. A bad pointer yields "?".
static void ResourceName(void* res, char* out, size_t n)
{
    out[0] = 0;
    if (!res || !TsmIsValidGamePtr(res) || !ReadablePtr(res, 128)) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    const char* s = (const char*)res;
    size_t i = 0;
    for (; i + 1 < n; i++) {
        const char c = s[i];
        if (!c) break;
        if ((unsigned char)c < 32 || (unsigned char)c > 126) {
            _snprintf_s(out, n, _TRUNCATE, "?");
            return;
        }
        out[i] = c;
    }
    out[i] = 0;
    if (!out[0]) _snprintf_s(out, n, _TRUNCATE, "?");
}

// The building's type-descriptor name, so the log never again asks the reader
// to guess which object a pointer is.
static void BuildingName(void* b, char* out, size_t n)
{
    out[0] = 0;
    if (!b || !g_enterprise || !TsmIsValidGamePtr(b)) {
        _snprintf_s(out, n, _TRUNCATE, "-");
        return;
    }
    const char* nm = nullptr;
    __try { nm = g_enterprise->getBuildingObjectName(b); }
    __except (EXCEPTION_EXECUTE_HANDLER) { nm = nullptr; }
    if (!nm || !nm[0]) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    size_t i = 0;
    for (; i + 1 < n && nm[i]; i++) {
        const char c = nm[i];
        out[i] = ((unsigned char)c < 32 || (unsigned char)c > 126) ? '?' : c;
    }
    out[i] = 0;
}

// The suite's single price function. Everything this plugin charges is
// expressed against THIS number, and every pay line names it, so a change of
// price source can never silently halve or double a payment.
//
// NOTE: resource_pricing itself may be returning its shipped fallbacks
// (RE_DIGEST Part 12 section 3). That is fine and consistent - what must not
// happen is this plugin mixing a raw Res+0x58 read with a priced figure.
static double UnitPrice(void* res)
{
    if (!res || !TsmIsValidGamePtr(res)) return 0.0;

    if (g_pricing) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        const float p = g_pricing->getPrice(res, cur);
        if (p == p && p > 0.01f && p < 1e9f) return (double)p;
    }

    // Fallback: the raw record field, only if resource_pricing is absent.
    if (!ReadablePtr((const BYTE*)res + TSM_RES_PRICE_RUB, 4)) return 0.0;
    const float f = *(const float*)((const BYTE*)res + TSM_RES_PRICE_RUB);
    if (!(f == f) || f < 0.0f || f > 1e9f) return 0.0;
    return (double)f;
}

// v0.4: freight as a share of the cargo's value.
//
// `tons` is positive on this leg (the delivery). A zero or nonsense unit
// price yields zero rather than a guess - the caller counts it as a skip.
static double FreightFor(double tons, double unitPrice)
{
    const double value = tons * unitPrice;
    if (!(value > 0.0)) return 0.0;
    const double share = (double)g_freightShare;
    if (!(share > 0.0)) return 0.0;
    return value * share;
}

// One guard, used everywhere the treasury is read: an older currency host may
// carry a shorter struct, and getTreasuryBalance is past the end of it.
static bool TreasuryReadable(void)
{
    return g_currency && g_currency->getTreasuryBalance &&
           g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                     sizeof(g_currency->getTreasuryBalance);
}

// v0.5: the fuel price, resolved by NAME through resource_pricing so the
// index moves with the game's own economy. Cached for the day and dropped at
// each close, because a save load can replace the resource table under us.
static double FuelUnitPrice(void)
{
    if (!g_pricing || !g_pricing->getByName) return 0.0;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    if (!g_fuelRes) return 0.0;

    const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
    const float p = g_pricing->getPrice(g_fuelRes, cur);
    if (!(p == p) || !(p > 0.0f) || !(p < 1e9f)) return 0.0;
    return (double)p;
}

// v0.7: THE TARIFF. Fuel plus a maintenance reserve, both denominated in
// litres and both priced at the live fuel price:
//
//   work    = litres_per_trip + litres_per_tonne x tonnes
//   fuel    = work x diesel_density x fuel_price        <- the index
//   wear    = wear_litres x diesel_density x fuel_price <- sinking fund
//   tariff  = fuel x (1 + margin) + wear
//
// Two rules are enforced here, not in prose. Nothing is a rouble constant, so
// inflation cannot eat it; and nothing depends on what the cargo is WORTH, so
// a truck of plants pays for its trip the same as a truck of chemicals.
// The wear term exists because fuel x 1.35 can never buy a truck: a carrier
// has to recover vehicles over their life, and that is a per-trip cost.
//
// `*outIndex` is 1 for the fuel index, 2 for the CPI fallback, 0 for neither.
// A 0 return means "this build cannot price this delivery", not "it is free".
static double TariffFor(double tons, double* outFuel, double* outWear,
                        int* outIndex)
{
    if (outFuel)  *outFuel  = 0.0;
    if (outWear)  *outWear  = 0.0;
    if (outIndex) *outIndex = 0;
    if (!(tons > 0.0)) return 0.0;

    double fuel = 0.0, wear = 0.0;
    const double work = gLitresPerTrip + gLitresPerTonne * tons;
    const double fp   = FuelUnitPrice();

    if (fp > 0.0) {
        fuel = work * gDieselDensity * fp;
        wear = gWearLitres * gDieselDensity * fp;
        if (outIndex) *outIndex = 1;
    } else if (g_pricing) {
        const float basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        if (basket == basket && basket > 0.0f && basket < 1e9f) {
            fuel = (gBasketsPerTrip + gBasketsPerTonne * tons) * (double)basket;
            // The reserve expressed in the fallback unit: the litres ratio is
            // dimensionless, so it survives the switch unchanged.
            const double ratio =
                (gLitresPerTrip > 0.0) ? (gWearLitres / gLitresPerTrip) : 0.0;
            wear = fuel * ratio;
            if (outIndex) *outIndex = 2;
        }
    }

    if (outFuel) *outFuel = fuel;
    if (outWear) *outWear = wear;

    const double total = fuel * (1.0 + gTariffMargin) + wear;
    return (total > 0.0) ? total : 0.0;
}

// v0.5: remember who was paid, so the close can show their cash position.
// Four slots: a republic has a handful of private logistics companies, not a
// hundred, and the balance line is about seeing money arrive and leave.
// v0.6: how much of the day's freight went to whom, so the close can say
// "freight in X, spent Y" instead of making the reader subtract.
static void NoteCarrierIncome(void* b, double amount)
{
    if (!b || !(amount > 0.0)) return;
    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { g_balCarrierIn[i] += amount; break; }
    LeaveCriticalSection(&g_lock);
}

static void TrackCarrier(void* b)
{
    if (!b) return;
    EnterCriticalSection(&g_lock);
    bool known = false;
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { known = true; break; }
    if (!known && g_balCarrierCount < BAL_CARRIERS) {
        g_balCarrier[g_balCarrierCount]     = b;
        g_balCarrierPrev[g_balCarrierCount] = -1.0;   // -1 = no baseline yet
        g_balCarrierFuel[g_balCarrierCount] = -1.0;   // first read is a baseline
        g_balCarrierCount++;
    }
    LeaveCriticalSection(&g_lock);
}

static void AppendShipment(const Shipment& s)
{
    int cap = g_ledgerCapacity;
    if (cap < 1)          cap = 1;
    if (cap > LEDGER_MAX) cap = LEDGER_MAX;

    g_ledger[g_ledgerHead % cap] = s;
    g_ledgerHead = (g_ledgerHead + 1) % cap;
    g_ledgerTotal++;
}

// ---------------------------------------------------------------------------
// paying
// ---------------------------------------------------------------------------
//
// One place that moves money, so there is exactly one place to look when a
// balance changes. dry_run decides whether the call lands.
//
//   payee  - the building whose company is credited
//   amount - positive RUB
//   cat    - what the movement is for (enterprise_base's own classification)
//   label  - appears verbatim in currency's flow ledger

static bool Pay(void* payee, double amount, TsmCapitalCategory cat,
                const char* label, const char* kind, void* res, float tons)
{
    // v0.4: every refusal is counted HERE, where the reason is known, so the
    // daily line can say which one it was instead of lumping them together.
    if (!(amount >= (double)g_minPayment)) {
        EnterCriticalSection(&g_lock); g_skipSmall++;    LeaveCriticalSection(&g_lock);
        return false;
    }
    if (!g_enterprise || !g_currency) {
        EnterCriticalSection(&g_lock); g_skipNoPayee++;  LeaveCriticalSection(&g_lock);
        return false;
    }

    char bname[64], rname[64];
    BuildingName(payee, bname, sizeof(bname));
    ResourceName(res, rname, sizeof(rname));

    if (g_dailyPayCap > 0.0 && g_paidToday + amount > g_dailyPayCap) {
        EnterCriticalSection(&g_lock);
        g_payCapped++;
        LeaveCriticalSection(&g_lock);
        return false;
    }

    // v0.4 AFFORDABILITY GATE.
    //
    // moveTreasury takes a signed amount and has no floor: crediting a
    // private company and debiting the treasury by more than it holds would
    // simply drive the balance negative. The state is the payer until Ship 2,
    // so it has to be able to say no. Reads the ACTIVE currency's treasury
    // (RUB by default), the same one moveTreasury writes.
    {
        double have = 0.0;
        if (g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance))
            have = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        if (have < amount) {
            EnterCriticalSection(&g_lock); g_skipTreasury++; LeaveCriticalSection(&g_lock);
            return false;
        }
    }

    if (g_dryRun) {
        Logf("[PAY] would %s %s %+.2f (%s %.3ft) - dry_run",
             kind, bname, amount, rname, (double)tons);
    } else {
        // v0.5 (H33): the treasury is read on both sides of the move. This is
        // the only isolated proof of the payer leg that exists - it cannot be
        // contaminated by wages or taxes, because nothing else runs between
        // the two reads.
        const int cur   = TsmCurrencyActive(g_currency);
        const bool sample = TreasuryReadable();
        double before = 0.0, after = 0.0;
        if (sample) before = g_currency->getTreasuryBalance(cur);

        g_enterprise->modifyCompanyCapital(payee, amount, cat);
        g_currency->moveTreasury(-amount, label);

        if (sample) {
            after = g_currency->getTreasuryBalance(cur);
            const double gap = (after - before) + amount;   // 0 = exact
            const double mag = (gap < 0.0) ? -gap : gap;
            EnterCriticalSection(&g_lock);
            g_treasuryChecked++;
            if (mag <= 0.005) {
                g_treasuryExact++;
            } else {
                g_treasuryOff++;
                if (mag > g_treasuryMaxGap) g_treasuryMaxGap = mag;
            }
            LeaveCriticalSection(&g_lock);
        }

        Logf("[PAY] %s %s %+.2f (%s %.3ft) paid by the state",
             kind, bname, amount, rname, (double)tons);
    }

    EnterCriticalSection(&g_lock);
    g_paidToday += amount;
    g_payCount++;
    if (cat == TSM_CAP_PRODUCTION) g_paidFactory += amount;
    else                          g_paidCarrier += amount;
    LeaveCriticalSection(&g_lock);
    return true;
}

// ---------------------------------------------------------------------------
// the detour
// ---------------------------------------------------------------------------

static int DetourTransfer(void* game, void* p2, unsigned mode, void* res,
                          float tons, float* outMoved, float cap,
                          unsigned char flag, void* p9)
{
    // ARGUMENT NAMES THAT MATTER:
    //
    //   p2 = param_2. At mode 0 this is the BUILDING; at mode 1 the VEHICLE.
    //        Proven in two sessions (RE_DIGEST Part 13 §4).
    //   p9 = param_9. The FLEET OWNER on the delivery family, NULL on the
    //        load family. NOT an endpoint.
    //
    // Getting these two the wrong way round is the single easiest way to
    // build a payment that pays nobody, so they are named for their
    // parameter position and nothing else.

    // Which site called us. The patched instruction is still a `call`, so the
    // CPU pushed site+5 and that is what _ReturnAddress reads.
    const BYTE* retAddr = (const BYTE*)_ReturnAddress();
    unsigned    siteRva = 0;
    SiteClass   known   = SITE_UNKNOWN;
    int         siteIdx = -1;

    if (retAddr && g_exeBase) {
        const intptr_t off = (intptr_t)retAddr - (intptr_t)(g_exeBase + 5);
        if (off > 0 && off < (intptr_t)g_exeSize) {
            siteRva = (unsigned)off;
            siteIdx = SiteIndex(siteRva);
            if (siteIdx >= 0) known = g_sites[siteIdx].known;
        }
    }

    // At mode 0 p2 is the building. At mode 1 it is the vehicle and its home
    // depot is the fleet owner - read it BEFORE the call, because the callee
    // clobbers registers and the vehicle can be re-homed later in the tick.
    void* building = (mode == 0 && p2 && TsmIsValidGamePtr(p2) &&
                      ReadablePtr(p2, TSM_BLD_MIN_READABLE)) ? p2 : nullptr;
    void* owner    = p9;

    void* vehicleHome = nullptr;
    if (mode != 0 && p2 && TsmIsValidGamePtr(p2) &&
        ReadablePtr((const BYTE*)p2 + TSM_VEH_HOME, 8)) {
        vehicleHome = *(void**)((BYTE*)p2 + TSM_VEH_HOME);
    }

    EnterCriticalSection(&g_lock);
    g_calls++;
    LeaveCriticalSection(&g_lock);

    // Forward. The transfer returns a bool, so only the low byte of the return
    // carries the answer.
    const int  r         = g_realTransfer
                         ? g_realTransfer(game, p2, mode, res, tons,
                                          outMoved, cap, flag, p9)
                         : 0;
    const bool committed = ((unsigned char)r == 1u);

    // ---- classify, outside the lock --------------------------------------
    const bool   isStaging = (mode != 0 && vehicleHome && vehicleHome == p9);
    const double unitPrice = UnitPrice(res);

    Shipment s;
    memset(&s, 0, sizeof(s));
    s.vehicle  = (mode != 0) ? p2 : nullptr;
    s.resource = res;
    s.tons     = tons;
    s.site     = (unsigned short)(siteRva & 0xFFFF);
    s.mode     = (unsigned char)mode;
    s.building = building;

    if (committed) {
        if (isStaging) {
            s.kind = SHIP_LOAD;
        } else if (known == SITE_REFUEL) {
            s.kind      = SHIP_FUEL;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        } else if (tons > 0.0f) {
            s.kind      = SHIP_DELIVERY;
            s.unitPrice = unitPrice;
            s.value     = (double)tons * unitPrice;
        } else {
            s.kind      = SHIP_LOAD;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        }
    }

    // ---- carrier resolution (mode 1 only feeds the ring) -----------------
    bool     emitSite   = false;
    bool     emitAttrib = false;
    unsigned shipId     = 0;
    void*    carrier    = nullptr;
    bool     carrierMatched = false;

    EnterCriticalSection(&g_lock);

    if (committed && mode != 0 && vehicleHome) {
        CarrierNote& n = g_carrier[g_carrierHead % CARRIER_RING];
        n.res  = res;
        n.tons = fabsf(tons);
        n.home = vehicleHome;
        n.t  = NowSeconds();
        g_carrierHead = (g_carrierHead + 1) % CARRIER_RING;
    }

    if (committed && mode == 0 && tons < 0.0f && !owner) {
        // The load leg: param_9 is NULL, so find the vehicle that took these
        // goods from the mode-1 call that follows within a few milliseconds.
        const double now = NowSeconds();
        for (int i = 1; i <= CARRIER_RING; i++) {
            const int k = ((g_carrierHead - i) % CARRIER_RING + CARRIER_RING) % CARRIER_RING;
            const CarrierNote& n = g_carrier[k];
            if (!n.home || n.res != res) continue;
            if (now - n.t > 2.0) continue;
            const float tol = (fabsf(tons) > 0.01f) ? fabsf(tons) * 0.005f : 0.0005f;
            if (fabsf(n.tons - fabsf(tons)) > tol) continue;
            carrier = n.home;
            carrierMatched = true;
            break;
        }
    }

    if (committed) g_callsCommitted++;
    else           g_callsNoop++;

    if (siteIdx >= 0) {
        SiteTally& t = g_tally[siteIdx];
        t.calls++;
        if (committed) {
            t.committed++;
            if (tons >= 0.0f) t.tonsIn  += (double)tons;
            else              t.tonsOut += fabs((double)tons);
            t.value += s.value;
        }
        if (p9)           t.sawBld++;
        if (vehicleHome)  t.sawHome++;
    }

    if (committed) {
        if (mode == 0 && !building)                g_unknownBuilding++;
        if (isStaging)                             g_staging++;
        if (mode == 0 && !IsPrivate(building))     g_nonPrivate++;

        if (s.kind == SHIP_FUEL)                    { g_wouldPayFuel    += s.value;
                                                      g_fuelEvents++; }
        if (s.kind == SHIP_DELIVERY && s.value > 0.0) g_wouldPayFreight += s.value;

        s.id  = g_ledgerSeq++;
        s.seq = g_ledgerTotal;
        AppendShipment(s);
        shipId = s.id;
    }

    // Separate budgets: a busy day can never hide a site behind its own
    // attribution output, and payments get a third budget of their own.
    if (g_siteLog) {
        if (g_siteLoggedToday < (unsigned)g_logCapPerDay) {
            g_siteLoggedToday++;
            emitSite = true;
        } else {
            g_siteSuppressed++;
        }
    }
    if (committed) {
        if (g_attrLoggedToday < (unsigned)g_logCapPerDay) {
            g_attrLoggedToday++;
            emitAttrib = true;
        } else {
            g_attrSuppressed++;
        }
    }

    LeaveCriticalSection(&g_lock);

    // ---- logging, outside the lock ---------------------------------------
    if (emitSite) {
        char bname[64];
        BuildingName(building, bname, sizeof(bname));
        Logf("[B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u "
             "p2=%p \"%s\" priv=%d p9=%p vhome=%p class=%s",
             siteRva, mode, (tons < 0.0f ? '-' : '+'), (double)tons,
             committed ? 1 : 0, (unsigned)flag,
             p2, bname, IsPrivate(building) ? 1 : 0, p9, vehicleHome,
             ClassName(known));
    }

    // ---- billing ---------------------------------------------------------
    //
    // MODE 0 ONLY. A mode-1 call is the vehicle side of a movement a mode-0
    // call has already described; billing both would charge it twice.
    // v0.5: the probe runs even with billing off. It is a measurement, and
    // measuring a cost must not depend on whether money is being moved.
    if ((g_billing || g_tariffProbe || g_tripTariff) && committed && mode == 0 && building) {

        char rname[64];
        ResourceName(res, rname, sizeof(rname));

        double freightPaid   = 0.0;
        double probeTariff   = 0.0;
        double probeCost     = 0.0;
        double probeShare    = 0.0;
        int    probeIndex    = 0;
        bool   probeLine     = false;

        if (tons > 0.0f) {

            // ---- v0.7 THE TRIP TARIFF (this IS the money) -----------------
            // v0.6 computed this and paid the value share anyway. v0.7 pays
            // it. `trip_tariff = 0` puts the value share back untouched.
            if (g_tariffProbe || g_tripTariff) {
                probeTariff = TariffFor((double)tons, &probeCost, &probeShare,
                                        &probeIndex);

                EnterCriticalSection(&g_lock);
                if (probeTariff > 0.0) {
                    g_tariffCount++;
                    g_tariffSum       += probeTariff;
                    g_tariffCostSum   += probeCost * (1.0 + gTariffMargin);
                    g_tariffShareSum  += probeShare;
                    if (probeIndex == 1)      g_tariffFuelIndex++;
                    else if (probeIndex == 2) g_tariffCpiIndex++;
                    if (g_tariffLoggedToday < (unsigned)g_tariffLogPerDay) {
                        g_tariffLoggedToday++;
                        probeLine = true;
                    }
                } else {
                    g_tariffNoIndex++;
                }
                LeaveCriticalSection(&g_lock);
            }

            if (g_billing && g_payCarrier) {
                // H2 - freight on a delivery into `building`.
                // The fleet owner is param_9 on this family; if the site does
                // not carry one, fall back to the ring.
                void* paid = nullptr;
                if (owner && owner != building && IsPrivate(owner))      paid = owner;
                else if (!owner && carrier && carrier != building &&
                         IsPrivate(carrier))                             paid = carrier;

                if (paid) {
                    // v0.7: the trip price. The value share survives only for
                    // the comparison path (trip_tariff = 0).
                    const double freight = g_tripTariff
                        ? probeTariff
                        : FreightFor((double)tons, unitPrice);
                    if (freight <= 0.0) {
                        EnterCriticalSection(&g_lock); g_skipNoPrice++; LeaveCriticalSection(&g_lock);
                    } else if (Pay(paid, freight, TSM_CAP_WHOLESALE,
                                   "b2b_freight", "freight", res, tons)) {
                        freightPaid = freight;
                        TrackCarrier(paid);
                        NoteCarrierIncome(paid, freight);
                        BurnProbe(paid);              // v0.11: per-delivery tank sample
                    }
                } else {
                    EnterCriticalSection(&g_lock); g_skipNoPayee++; LeaveCriticalSection(&g_lock);
                }
            }

            // One line carries both numbers, so the calibration is a
            // subtraction the reader can do in their head.
            if (probeLine) {
                char bname[64];
                BuildingName(building, bname, sizeof(bname));
                Logf("[TARIFF MODEL] %s %.3ft @%.2f = %.2f -> %s: %.2f = fuel %.2f x%.2f "
                     "+ wear %.2f (index %s) | transfer_fragment=1 journey=0 "
                     "| payment_returned=%.2f billing=%d dry_run=%d",
                     rname, (double)tons, unitPrice, (double)tons * unitPrice, bname,
                     probeTariff, probeCost, 1.0 + (double)gTariffMargin, probeShare,
                     probeIndex == 1 ? "fuel" : (probeIndex == 2 ? "cpi" : "none"),
                     freightPaid, g_billing, g_dryRun);
            }
        }

        if (g_billing && tons < 0.0f && g_payFactory) {
            // H1 - the private building the goods leave is paid for them.
            if (IsPrivate(building)) {
                if (unitPrice > 0.0) {
                    const double value =
                        fabs((double)tons) * unitPrice * (double)g_factoryShare;
                    Pay(building, value, TSM_CAP_PRODUCTION,
                        "b2b_factory", "factory", res, tons);
                } else {
                    // A private building shipping goods this build cannot
                    // price. Count it and say so once per day-cap window:
                    // silence here would look like "nothing happened".
                    char bname[64];
                    BuildingName(building, bname, sizeof(bname));
                    EnterCriticalSection(&g_lock);
                    g_skipNoPrice++;
                    const bool say = (g_payLoggedToday < (unsigned)g_logCapPerDay);
                    if (say) g_payLoggedToday++;
                    LeaveCriticalSection(&g_lock);
                    if (say)
                        Logf("[PAY] skipped %s: no price for %s (unitPrice 0.00) - "
                             "resource_pricing absent or unpriced", bname, rname);
                }
            }
        }

        (void)carrierMatched;
    }

    if (emitAttrib) {
        char bname[64], rname[64];
        BuildingName(building, bname, sizeof(bname));
        ResourceName(res, rname, sizeof(rname));
        Logf("[B2B] #%u %-6s bld=%p \"%s\" priv=%d res=%s %.2ft @%.2f = %.2f "
             "(site 0x%06X, mode %u)",
             shipId, KindName(s.kind), building, bname, IsPrivate(building) ? 1 : 0,
             rname, (double)s.tons, s.unitPrice, s.value, siteRva, mode);
    }

    return r;
}

// ---------------------------------------------------------------------------
// call-site patching
// ---------------------------------------------------------------------------

static bool PatchSite(int index)
{
    if (index < 0 || index >= g_siteCount) return false;
    if (g_sitePatched[index]) return true;

    const unsigned rva = g_sites[index].rva;

    if (rva < 0x1000 || (size_t)rva + 5 > g_exeSize) {
        Logf("private_logistics  site 0x%06X is outside the image (%llu bytes) - skipped",
             rva, (unsigned long long)g_exeSize);
        return false;
    }

    BYTE* site = g_exeBase + rva;
    if (!ReadablePtr(site, 5)) {
        Logf("private_logistics  site 0x%06X not readable - skipped", rva);
        return false;
    }

    // Verify: a direct call, landing on the transfer.
    if (site[0] != 0xE8) {
        Logf("private_logistics  site 0x%06X is not a call (0x%02X) - skipped",
             rva, site[0]);
        return false;
    }
    const int32_t rel    = *(const int32_t*)(site + 1);
    BYTE*         target = site + 5 + rel;
    if (target != g_exeBase + TSM_RVA_TRANSFER) {
        Logf("private_logistics  site 0x%06X calls %p, not the transfer - skipped",
             rva, target);
        return false;
    }

    // The cave: `jmp qword ptr [rip+0]` plus the absolute detour address.
    BYTE* cave = AllocNear(site, 32);
    if (!cave) {
        Logf("private_logistics  no code cave near 0x%06X - skipped", rva);
        return false;
    }

    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&DetourTransfer;
    memcpy(cave + 6, &detour, 8);
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("private_logistics  cave out of rel32 range at 0x%06X - skipped", rva);
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("private_logistics  VirtualProtect failed at 0x%06X - nothing written", rva);
        return false;
    }

    // Re-verify under the protection.
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("private_logistics  site 0x%06X changed under us - nothing written", rva);
        return false;
    }

    site[0] = 0xE8;                              // same opcode: still a call
    *(int32_t*)(site + 1) = (int32_t)toCave;     // new destination
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    g_sitePatched[index] = true;
    Logf("private_logistics  hook 0x%06X -> transfer ok (cave %p)", rva, (void*)cave);
    return true;
}

static void PatchAllSites(void)
{
    if (!g_siteFilterActive)
        for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = true;

    int ok = 0, wanted = 0;
    for (int i = 0; i < g_siteCount; i++) {
        if (!g_siteWanted[i]) continue;
        wanted++;
        if (PatchSite(i)) ok++;
    }

    if (g_siteFilterActive)
        Logf("private_logistics  site_list active: %d of %d sites selected",
             wanted, g_siteCount);

    Logf("private_logistics  patched %d of %d transfer call sites", ok, wanted);

    if (ok == 0) {
        Logf("private_logistics  ERROR  no sites patched. Either this is not "
             "SOVIET64.exe v1.1.1.9, or the transfer is not at RVA 0x420350. "
             "Nothing was written; the game is unaffected.");
    } else if (ok < wanted) {
        Logf("private_logistics  WARN  %d site(s) refused - see the lines above. "
             "A refusal is safe: that call still reaches the engine normally.",
             wanted - ok);
    }
}

// ---------------------------------------------------------------------------
// the daily roll-up
// ---------------------------------------------------------------------------

static void OnDay(void*, int day, int month, int year)
{
    (void)year;

    // Snapshot under the lock, clear everything, print outside it.
    //
    // v0.3 BUGFIX: v0.2 snapshotted these counters without zeroing them, so
    // its "[B2B] day N" line was cumulative since load while the per-site
    // table was genuinely per-day. Both are per-day now.
    static SiteTally snap[MAX_SITES];
    unsigned calls, committed, noop, unknown, stag, nonPriv;
    unsigned siteSupp, attrSupp, paySupp, payCount, payCap, total;
    unsigned skNoPayee, skNoPrice, skSmall, skTreasury;
    unsigned tCount, tNoIndex, tFuelIdx, tCpiIdx;
    unsigned trChecked, trExact, trOff, fuelEvents;
    double   paidF, paidC, freight, fuel, tSum, tCostSum, tShareSum, trMaxGap;
    static double cIn[BAL_CARRIERS];
    static void*  cPt[BAL_CARRIERS];
    int      cCount;
    bool     newMonth;

    EnterCriticalSection(&g_lock);
    memcpy(snap, g_tally, sizeof(snap));
    memset(g_tally, 0, sizeof(g_tally));

    calls      = g_calls;
    committed  = g_callsCommitted;
    noop       = g_callsNoop;
    unknown    = g_unknownBuilding;
    stag       = g_staging;
    nonPriv    = g_nonPrivate;
    siteSupp   = g_siteSuppressed;
    attrSupp   = g_attrSuppressed;
    paySupp    = g_paySuppressed;
    payCount   = g_payCount;
    payCap     = g_payCapped;
    skNoPayee  = g_skipNoPayee;
    skNoPrice  = g_skipNoPrice;
    skSmall    = g_skipSmall;
    skTreasury = g_skipTreasury;
    tCount     = g_tariffCount;
    tNoIndex   = g_tariffNoIndex;
    tFuelIdx   = g_tariffFuelIndex;
    tCpiIdx    = g_tariffCpiIndex;
    tSum       = g_tariffSum;
    trChecked  = g_treasuryChecked;
    trExact    = g_treasuryExact;
    trOff      = g_treasuryOff;
    trMaxGap   = g_treasuryMaxGap;
    fuelEvents = g_fuelEvents;
    tCostSum   = g_tariffCostSum;
    tShareSum  = g_tariffShareSum;
    cCount     = g_balCarrierCount;
    for (int i = 0; i < BAL_CARRIERS; i++) {
        cPt[i] = g_balCarrier[i];
        cIn[i] = g_balCarrierIn[i];
        g_balCarrierIn[i] = 0.0;      // this day's income, spent by the close
    }
    total      = g_ledgerTotal;
    paidF      = g_paidFactory;
    paidC      = g_paidCarrier;
    freight    = g_wouldPayFreight;
    fuel       = g_wouldPayFuel;

    g_calls = g_callsCommitted = g_callsNoop = 0;
    g_unknownBuilding = g_staging = g_nonPrivate = 0;
    g_siteSuppressed = g_attrSuppressed = g_paySuppressed = 0;
    g_payCount = g_payCapped = 0;
    g_skipNoPayee = g_skipNoPrice = g_skipSmall = g_skipTreasury = 0;
    g_tariffCount = g_tariffNoIndex = g_tariffFuelIndex = g_tariffCpiIndex = 0;
    g_tariffCostSum = g_tariffShareSum = 0.0;
    g_tariffSum = 0.0;
    g_treasuryChecked = g_treasuryExact = g_treasuryOff = 0;
    g_treasuryMaxGap = 0.0;
    g_fuelEvents = 0;
    g_fuelRes = nullptr;          // re-resolve the index next day
    g_paidFactory = g_paidCarrier = 0.0;
    g_wouldPayFreight = g_wouldPayFuel = 0.0;
    g_paidToday = 0.0;

    g_siteLoggedToday = 0;
    g_attrLoggedToday = 0;
    g_payLoggedToday  = 0;
    g_tariffLoggedToday = 0;

    newMonth = (month != g_lastMonth);
    g_lastMonth = month;

    // v0.4: this callback fires at the START of `day`, so everything above is
    // the day that just ENDED. Report that day, not this one - the old label
    // was one ahead of its own numbers.
    // A loaded save can rewind the clock; do not report a day in the future.
    if (g_summaryDay > day) g_summaryDay = -1;
    const int closedDay = (g_summaryDay >= 0) ? g_summaryDay : (day - 1);
    g_summaryDay = day;
    LeaveCriticalSection(&g_lock);

    Logf("[B2B] day %d closed: %u calls, %u committed, %u no-op | unknown %u, "
         "staging %u, non-private %u | suppressed site %u attrib %u pay %u",
         closedDay, calls, committed, noop, unknown, stag, nonPriv, siteSupp,
         attrSupp, paySupp);

    // Per-site table. A site that never fired is simply absent - absence is
    // evidence rather than a logging artefact.
    for (int i = 0; i < g_siteCount; i++) {
        const SiteTally& t = snap[i];
        if (t.calls == 0) continue;
        Logf("[B2B SITE] day %d  0x%06X %-9s calls=%u commit=%u in=%.3f out=%.3f "
             "value=%.2f | arg9!=0 %u home!=0 %u",
             closedDay, g_sites[i].rva, ClassName(g_sites[i].known), t.calls,
             t.committed, t.tonsIn, t.tonsOut, t.value, t.sawBld, t.sawHome);
    }

    if (g_billing) {
        Logf("[PAY] day %d closed: %u payment(s), %u capped | skipped: no payee %u, "
             "no price %u, too small %u, treasury %u | factory %.2f, carrier %.2f | %s",
             closedDay, payCount, payCap, skNoPayee, skNoPrice, skSmall,
             skTreasury, paidF, paidC,
             g_dryRun ? "DRY RUN - nothing moved"
                      : "PAID by the state treasury");
    } else {
        Logf("[PAY] day %d closed: billing off", closedDay);
    }

    // v0.5: the model's own total, beside the index it managed to use.
    // `unindexed` is the honest failure: neither a fuel price nor a CPI
    // basket resolved, so the probe refused to invent a figure.
    if (g_tariffProbe || g_tripTariff) {
        Logf("[TARIFF MODEL] day %d closed: %u priced_transfer_fragment(s), model %.2f = fuel %.2f x%.2f + "
             "wear %.2f, mean %.2f | indexed by fuel %u, by cpi %u, unindexed %u "
             "| NOT journeys or payment totals; billing=%d dry_run=%d",
             closedDay, tCount,
             tSum, tCostSum, 1.0 + (double)gTariffMargin, tShareSum,
             tCount ? tSum / (double)tCount : 0.0,
             tFuelIdx, tCpiIdx, tNoIndex, g_billing, g_dryRun);
    }

    // v0.5: THE MONEY PATH, BOTH ENDS (H33, H37).
    //
    // v0.4 proved the payee's account to the kopeck; it never read the payer.
    // Here the treasury is sampled at every close, so consecutive closes give
    // the movement, and `unexplained` is what the [PAY] lines cannot account
    // for - zero when the books close, non-zero when something else touched
    // the treasury in the same day.
    //
    // The carriers' capital is printed beside it. Income minus everything the
    // company spent, fuel included: if the game charges a private DO for
    // diesel, the difference between this delta and the freight it was paid
    // IS that bill - the number Ship 2 has to price against.
    if (g_balanceLog) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(cur);
            have  = true;
        }

        const double paid = paidF + paidC;

        if (!have) {
            Logf("[BAL] day %d closed: treasury unavailable (no currency service)", closedDay);
        } else if (!g_treasuryHaveClose) {
            Logf("[BAL] day %d closed: treasury %.2f (baseline - the next close gives the "
                 "movement) | paid %.2f", closedDay, treas, paid);
        } else {
            const double moved = treas - g_treasuryAtClose;
            Logf("[BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid "
                 "%.2f | the rest of the movement is the rest of the state's day%s",
                 closedDay, g_treasuryAtClose, treas, moved, paid,
                 g_dryRun ? " | DRY RUN: nothing should have moved" : "");
        }
        if (have) { g_treasuryAtClose = treas; g_treasuryHaveClose = true; }

        // v0.5 (H33): the proof, per payment rather than per day.
        if (trChecked > 0 || trOff > 0) {
            Logf("[BAL] day %d treasury leg: %u payment(s) sampled either side, "
                 "%u moved exactly the amount, %u off by any margin (largest %.2f)",
                 closedDay, trChecked, trExact, trOff, trMaxGap);
        }

        // v0.6: the carrier's day in one line. `spent` is the arithmetic
        // difference - freight in, minus the change in capital - and in this
        // save it is the fuel bill, because fuel is the only thing a private
        // DO buys (it arrives as a cat=2 FOREIGN debit on the same account).
        for (int i = 0; i < cCount; i++) {
            void* b = cPt[i];
            if (!b || !g_enterprise || !g_enterprise->getCompanyCapital) continue;
            const double cap = g_enterprise->getCompanyCapital(b);
            char bname[64];
            BuildingName(b, bname, sizeof(bname));
            if (g_balCarrierPrev[i] < 0.0) {
                Logf("[BAL] day %d   %s capital %.2f (baseline) | freight in %.2f",
                     closedDay, bname, cap, cIn[i]);
            } else {
                const double delta = cap - g_balCarrierPrev[i];
                Logf("[BAL] day %d   %s capital %.2f (%+.2f) | freight in %.2f | spent %.2f",
                     closedDay, bname, cap, delta, cIn[i], cIn[i] - delta);
            }

            // v0.7: THE FUEL METER.
            //
            // monthWholesale is where enterprise_base books cat=2 FOREIGN
            // debits, and diesel is the only foreign purchase a distribution
            // office makes - so this is the carrier's real fuel bill for the
            // calendar month, read back from the game rather than assumed.
            //
            // Fuel is bought in bulk and stored, so a day's purchases are not
            // a day's burn. The run average is, because the tank delta washes
            // out of a long run. That is the number the [TARIFF] model is
            // scored against, and the number litres_per_trip should hold.
            if (g_enterprise->describeEnterprise) {
                TsmEnterpriseInfo info;
                memset(&info, 0, sizeof(info));
                info.structSize = sizeof(TsmEnterpriseInfo);
                if (g_enterprise->describeEnterprise(b, &info) == 1) {
                    const double bill = info.monthWholesale;
                    double spent = 0.0;
                    if (g_balCarrierFuel[i] < 0.0) {
                        // First read after a load: monthWholesale already holds
                        // whatever the save carried. Take it as the baseline and
                        // say so - counting it would bill one day for a month.
                        g_balCarrierFuel[i] = bill;
                        Logf("[FUEL] day %d   %s meter baseline: month wholesale %.2f "
                             "(not counted as this day's burn)", closedDay, bname, bill);
                    } else {
                        spent = (bill < g_balCarrierFuel[i])
                              ? bill                      // month boundary: accumulators cleared
                              : (bill - g_balCarrierFuel[i]);
                        g_balCarrierFuel[i] = bill;
                    }
                    g_fuelPaidRun += (spent > 0.0) ? spent : 0.0;
                    const double fprice = FuelUnitPrice();
                    double litres = 0.0;
                    if (fprice > 0.0 && gDieselDensity > 0.0)
                        litres = ((spent > 0.0) ? spent : 0.0) / (gDieselDensity * fprice);
                    g_fuelLitresRun += litres;
                    Logf("[FUEL] day %d   %s diesel %.2f (measured, cat=2) = %.2f L at %.2f/%s",
                         closedDay, bname, (spent > 0.0) ? spent : 0.0, litres, fprice,
                         g_currency ? TsmCurrencySymbol(g_currency) : "RUB");
                }
            }
            g_balCarrierPrev[i] = cap;
        }

        // v0.7: the run total, and the one number to copy into the ini.
        g_fuelTripsRun += tCount;
        if (g_fuelTripsRun > 0) {
            const double perTrip = g_fuelLitresRun / (double)g_fuelTripsRun;
            // v0.9: this counts PURCHASES, not burn. Fuel is bought in bulk
            // and sits in tanks, so a short run and a long run give different
            // answers from the same fleet - 3.27 L/trip over 1,100 trips,
            // 7.14 over 214. Do not treat it as a measurement of the burn.
            Logf("[FUEL PURCHASE MODEL] run: %.2f RUB = %.2f configured_litres_estimate over %llu transfer_fragment(s) = %.3f estimate_per_fragment "
                 "| configured %.2f L | fuel sits in tanks: this is purchases, not burn",
                 g_fuelPaidRun, g_fuelLitresRun, g_fuelTripsRun, perTrip, gLitresPerTrip);
        }

        // Legacy candidate differences: not native burn and not loaded journeys.
        if (g_fuelTripsRun > 0 && g_burnTonsRun > 0.0 && gDieselDensity > 0.0) {
            const double litres = g_burnTonsRun / gDieselDensity;
            const double lpt    = litres / (double)g_fuelTripsRun;
            // v0.11: L/DAY is printed beside L/trip because the physics is
            // per-tick. L/trip moves whenever the shops order differently -
            // the same fleet gave 3.27 and 7.14 L/trip in two runs - while
            // L per fleet-day is the same quantity in both. Read this one.
            const double lpd = g_tankDaysRun ? litres / (double)g_tankDaysRun : 0.0;
            const double lvd = g_tankVehDaysRun ? litres / (double)g_tankVehDaysRun : 0.0;
            const double lva = g_activeVehDaysRun ? litres / (double)g_activeVehDaysRun : 0.0;
            Logf("[TANK SAMPLE] run: decrease_raw=%.4f configured_density_litres_estimate=%.1f "
                 "| estimate_per_transfer_fragment=%.2f over %llu fragment(s) "
                 "| estimate_per_fleet_day=%.2f over %u close(s) | estimate_per_vehicle_day=%.2f over %u "
                 "| estimate_per_legacy_active_day=%.2f over %u legacy_active / %u legacy_idle "
                 "| sample_increases=%u | configured_model_litres=%.2f "
                 "units=unverified native_activity=unverified exact_burn=0 verified_refill=0",
                 g_burnTonsRun, litres, lpt, g_fuelTripsRun, lpd, g_tankDaysRun, lvd,
                 g_tankVehDaysRun, lva, g_activeVehDaysRun, g_idleVehDaysRun,
                 g_refillsRun, gLitresPerTrip);
            if (g_litresAuto)
                Logf("[TANK SAMPLE] auto-tune NOT applied: sampled changes/units are unverified; "
                     "configured litres_per_trip remains %.3f", gLitresPerTrip);
        }
    }

    // Legacy inter-transfer endpoint samples are not deduction events.
    if (g_probeStepsRun > 0 && gDieselDensity > 0.0) {
        const double litres = g_probeBurnRun / gDieselDensity;
        const double lpt    = litres / (double)g_probeStepsRun;
        Logf("[FUEL SAMPLE] run: net_raw=%.4f configured_density_litres_estimate=%.1f "
             "between %u transfer_fragment_pair(s) = %.2f estimate_per_pair "
             "| %u interval(s) skipped (sample increase or candidate-count change) "
             "| configured_model_litres=%.2f | %s",
             g_probeBurnRun, litres, g_probeStepsRun, lpt, g_probeSkipRun,
             gLitresPerTrip,
             "units=unverified exact_burn=0 journey_count=unknown calibration_input=0");
    }

    // v0.8: who does this company actually own, and what can they carry?
    CensusFleets(closedDay);

    // v0.6: re-read the index at every close. At plugin start the world has
    // not loaded, so v0.5's "fuel NOT FOUND" and "treasury opens at 0.00" were
    // both true for about a second and then false forever. Printing them here
    // is the honest place.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        Logf("[IDX] day %d closed: fuel '%s' = %.2f/%s (%s) | cpi basket %.2f/day | "
             "margin %.0f%%, wear %.2f L",
             closedDay, gFuelResource, fp,
             g_currency ? TsmCurrencySymbol(g_currency) : "RUB",
             fp > 0.0 ? "resolved" : "NOT RESOLVED",
             (double)basket, gTariffMargin * 100.0, gWearLitres);
    }

    // v0.5: fuel telemetry. The refuel site has been silent in every session
    // so far; a count of 0 here after fuel is switched on is itself a result.
    Logf("[FUEL] day %d refuel sites: %u call(s), %.2f RUB of fuel moved through "
         "the hooked sites", closedDay, fuelEvents, fuel);

    if (newMonth) {
        Logf("[B2B] month %d: %u shipments recorded, %.2f RUB would have moved "
             "(freight %.2f, fuel %.2f)",
             month, total, freight + fuel, freight, fuel);
    }

    AutoTargetsOnDay(day, closedDay);
}

// ---------------------------------------------------------------------------
// automatic private DO targets and infrastructure discovery (v0.16)
// ---------------------------------------------------------------------------

#define AUTO_RVA_VECTOR_PUSH_BACK 0x08E050
#define AUTO_RVA_TASKCFG_CTOR     0x1512A0
#define AUTO_DO_TARGETS_BEGIN     0x0D38
#define AUTO_DO_TARGETS_END       0x0D40
#define AUTO_DO_DIRTY             0x0D50
#define AUTO_DO_UI_TIMER          0x0FC0
// Conservative plugin-side bound: native add-flow supports five selections,
// but panel+0x310 per-target control-array capacity above five is unverified.
// This limits new auto targets without deleting/purging any manual entries.
#define AUTO_DO_SAFE_TARGET_COUNT 5
#define AUTO_DO_ROAD_TYPE         43
#define AUTO_DO_RAIL_TYPE         52
#define AUTO_STORAGE_BUILDING_TYPE 5
#define AUTO_CUSTOMS_TYPE         20
#define AUTO_CUSTOMS_MAX          512
#define AUTO_STATE_STORAGE_MAX    512
#define AUTO_STATE_PERMISSION_MAX 512
#define AUTO_STATE_PREF_FILE "private_logistics_state.bin"
struct AutoCustomSite { void* building; float x; float z; char name[64]; };
// permission bits: 1=load FROM site, 2=deliver INTO site; zero=deny both.
struct AutoStateStorageSite { void* building; uint64_t key; float x; float z; char name[64]; unsigned char permission; };
struct AutoStatePermission { uint64_t key; unsigned char mode; };
static AutoCustomSite g_autoCustomSites[AUTO_CUSTOMS_MAX];
static AutoStateStorageSite g_autoStateStorageSites[AUTO_STATE_STORAGE_MAX];
static unsigned g_autoStateStorageCount = 0;
static AutoStatePermission g_statePermissions[AUTO_STATE_PERMISSION_MAX];
static unsigned g_statePermissionCount = 0;
#define AUTO_TARGET_ENTRY_SIZE    0x18
#define AUTO_TASKCFG_SIZE         0x198
#define AUTO_STORAGE_STRIDE       0xE0
#define AUTO_MAX_TARGETS_READ     65536

static uint64_t AutoHashMix(uint64_t h, uint8_t byte)
{
    return (h ^ byte) * 1099511628211ull;
}

static bool AutoReadBuildingType(void* b, int* outType)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return false;
    void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td) || !ReadablePtr((BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
        return false;
    *outType = *(int*)((BYTE*)td + TSM_TD_TYPE_NUM);
    return true;
}

static bool AutoBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // The building's position is the translation row of its embedded
    // C3D_NODE, not bld+0x08/+0x10 (those yielded zeros in the v0.15 probe).
    const BYTE* node = (const BYTE*)b + TSM_BLD_NODE;
    if (ReadablePtr(node, 0x40)) {
        const float x = *(const float*)(node + 0x30);
        const float z = *(const float*)(node + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            if (outX) *outX = x;
            if (outZ) *outZ = z;
            return true;
        }
    }

    // Some unfinished sites can lack a usable node transform; use the
    // verified placement-bbox fallback used by enterprise_base.
    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                const float x = mn[1], z = mn[3];
                if (x == x && z == z && fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
                    if (outX) *outX = x;
                    if (outZ) *outZ = z;
                    return true;
                }
            }
        }
    }
    return false;
}

// Stable session/save identity: native object name + type + quantized position.
// No raw Building* is retained in the retry ledger.
static bool AutoBuildingKey(void* b, uint64_t* outKey, int* outType)
{
    int type = -1;
    float x = 0.0f, z = 0.0f;
    if (!AutoReadBuildingType(b, &type) || !AutoBuildingPosition(b, &x, &z))
        return false;

    uint64_t h = 14695981039346656037ull;
    const char* objectName = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { objectName = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { objectName = nullptr; }
    }
    if (objectName) {
        for (size_t i = 0; i < 128; ++i) {
            if (!ReadablePtr(objectName + i, 1)) return false;
            const uint8_t c = (uint8_t)objectName[i];
            h = AutoHashMix(h, c);
            if (c == 0) break;
        }
    }
    const int32_t qx = (int32_t)(x * 10.0f + (x >= 0.0f ? 0.5f : -0.5f));
    const int32_t qz = (int32_t)(z * 10.0f + (z >= 0.0f ? 0.5f : -0.5f));
    const uint32_t values[3] = { (uint32_t)type, (uint32_t)qx, (uint32_t)qz };
    const uint8_t* bytes = (const uint8_t*)values;
    for (size_t i = 0; i < sizeof(values); ++i) h = AutoHashMix(h, bytes[i]);
    // H1 FIX: mix raw float bits to break 0.1m quantized collisions (two clones within 10cm)
    // keeps qx/qz for backward compat but adds sub-decimeter entropy; one-time permission reset for old saves
    uint32_t fx = 0, fz = 0;
    memcpy(&fx, &x, sizeof(fx));
    memcpy(&fz, &z, sizeof(fz));
    const uint8_t* fxB = (const uint8_t*)&fx; for (size_t i = 0; i < sizeof(fx); ++i) h = AutoHashMix(h, fxB[i]);
    const uint8_t* fzB = (const uint8_t*)&fz; for (size_t i = 0; i < sizeof(fz); ++i) h = AutoHashMix(h, fzB[i]);
    *outKey = h ? h : 1;
    if (outType) *outType = type;
    return true;
}

static void AutoBuildingName(void* b, char* out, size_t capacity)
{
    if (!out || capacity == 0) return;
    out[0] = 0;
    const char* name = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { name = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { name = nullptr; }
    }
    if (name) {
        size_t i = 0;
        for (; i + 1 < capacity; ++i) {
            if (!ReadablePtr(name + i, 1)) break;
            out[i] = name[i];
            if (out[i] == 0) return;
        }
        out[i] = 0;
        if (i > 0) return;
    }
    const char fallback[] = "unknown";
    size_t j = 0;
    while (j + 1 < capacity && fallback[j]) { out[j] = fallback[j]; ++j; }
    out[j] = 0;
}

static unsigned char AutoStatePermissionGet(uint64_t key)
{
    if (!key) return 0;
    unsigned char mode = 0;
    EnterCriticalSection(&g_lock);
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { mode = g_statePermissions[i].mode; break; }
    }
    LeaveCriticalSection(&g_lock);
    return mode;
}

static bool AutoStatePermissionSet(uint64_t key, unsigned char mode)
{
    if (!key || mode > AUTO_ALLOW_BOTH) return false;
    EnterCriticalSection(&g_lock);
    unsigned found = g_statePermissionCount;
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { found = i; break; }
    }
    if (mode == 0) {
        if (found < g_statePermissionCount) {
            g_statePermissions[found] = g_statePermissions[g_statePermissionCount - 1];
            --g_statePermissionCount;
        }
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (found < g_statePermissionCount) {
        g_statePermissions[found].mode = mode;
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (g_statePermissionCount >= AUTO_STATE_PERMISSION_MAX) {
        LeaveCriticalSection(&g_lock);
        return false;
    }
    g_statePermissions[g_statePermissionCount].key = key;
    g_statePermissions[g_statePermissionCount].mode = mode;
    ++g_statePermissionCount;
    LeaveCriticalSection(&g_lock);
    return true;
}

static bool AutoHasNativeStorage(void* b);

static unsigned AutoBuildingCapabilities(void* building)
{
    int type = -1;
    if (!AutoReadBuildingType(building, &type) || !AutoHasNativeStorage(building)) return 0;
    char asset[128];
    AutoBuildingName(building, asset, sizeof(asset));
    return AutoSiteCapabilities(type, asset);
}

static void AutoDrawStatePermissions(uint64_t key, unsigned capabilities)
{
    const unsigned char oldMode = AutoStatePermissionGet(key);
    unsigned char newMode = (unsigned char)(oldMode & capabilities);
    if (capabilities & AUTO_ALLOW_LOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_LOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO loading (take goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_LOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (capabilities & AUTO_ALLOW_UNLOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_UNLOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO deliveries (receive goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_UNLOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (newMode != oldMode) {
        if (!AutoStatePermissionSet(key, newMode)) {
            Logf("[AUTO DO] state permission table full; change not retained");
            return;
        }
        Logf("[AUTO DO] state permissions key=%016llX load=%u unload=%u",
             (unsigned long long)key, (newMode & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (newMode & AUTO_ALLOW_UNLOAD) ? 1u : 0u);
    }
}

static void StateStorageWidgetProvider(void* building, void*)
{
    if (!g_autoTargetsEnabled || !g_uiPanels || !TsmUiPanelsHasWidgets(g_uiPanels) ||
        !building || !TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE) || !g_enterprise ||
        !g_enterprise->isBuildingPrivate || g_enterprise->isBuildingPrivate(building)) return;
    const unsigned capabilities = AutoBuildingCapabilities(building);
    uint64_t key = 0;
    if (!AutoBuildingKey(building, &key, nullptr)) return;
    static uint64_t lastPanelKey = 0;
    if (lastPanelKey != key) {
        lastPanelKey = key;
        int type = -1; AutoReadBuildingType(building, &type);
        char asset[128]; AutoBuildingName(building, asset, sizeof(asset));
        const unsigned effective = AutoStatePermissionGet(key) & capabilities;
        Logf("[AUTO DO] state panel key=%016llX type=%d asset=%s capabilities=%u permissions=%u native_load=%u native_unload=%u (native 0=checked,1=unchecked)%s",
             (unsigned long long)key, type, asset, capabilities, effective,
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_LOAD) != 0),
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_UNLOAD) != 0),
             capabilities ? "" : " (unsupported role: controls hidden, automatic access denied)");
    }
    if (!capabilities) return; // unknown roles do not get a misleading loading control
    AutoDrawStatePermissions(key, capabilities);
}

static bool AutoHasNativeStorage(void* b)
{
    if (!b || !TsmIsValidGamePtr(b) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_STORAGES_BEGIN, 16)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
    const uintptr_t end = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_END);
    if (!begin || end <= begin || ((end - begin) % AUTO_STORAGE_STRIDE) != 0) return false;
    const uintptr_t count = (end - begin) / AUTO_STORAGE_STRIDE;
    return count <= 4096 && ReadablePtr((const void*)begin, count * AUTO_STORAGE_STRIDE);
}

static bool AutoReadTargetVector(void* office, void*** outBegin, size_t* outCount)
{
    if (!office || !TsmIsValidGamePtr(office) ||
        !ReadablePtr((BYTE*)office + AUTO_DO_TARGETS_BEGIN, 24)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_BEGIN);
    const uintptr_t end   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END);
    const uintptr_t cap   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END + 8);
    if (!begin && !end && !cap) { *outBegin = nullptr; *outCount = 0; return true; }
    if (!begin || !end || !cap || end < begin || cap < end || ((end - begin) & 7) != 0)
        return false;
    const size_t count = (size_t)((end - begin) / sizeof(void*));
    if (count > AUTO_MAX_TARGETS_READ ||
        !ReadablePtr((const void*)begin, count * sizeof(void*))) return false;
    *outBegin = (void**)begin; *outCount = count;
    return true;
}

static unsigned AutoOfficeHash(uint64_t officeKey)
{
    officeKey ^= officeKey >> 33; officeKey *= 0xff51afd7ed558ccdull;
    officeKey ^= officeKey >> 33; officeKey *= 0xc4ceb9fe1a85ec53ull;
    officeKey ^= officeKey >> 33;
    return (unsigned)officeKey & (AUTO_OFFICE_HASH_SIZE - 1);
}

static bool AutoBuildOfficeMemoIndex(void)
{
    memset(g_autoOfficeMemoHeads, 0, sizeof(g_autoOfficeMemoHeads));
    for (unsigned i = 0; i < g_autoMemoCount; ++i) {
        AutoMemo& memo = g_autoMemo[i];
        unsigned slot = AutoOfficeHash(memo.officeKey);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
            AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
            if (head.officeKey == 0 || head.officeKey == memo.officeKey) {
                head.officeKey = memo.officeKey;
                memo.nextOffice = head.headPlusOne ? head.headPlusOne - 1 : 0xFFFFFFFFu;
                head.headPlusOne = i + 1;
                inserted = true;
                break;
            }
            slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoOfficeMemoHeadIndex(uint64_t officeKey)
{
    unsigned slot = AutoOfficeHash(officeKey);
    for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
        const AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
        if (head.officeKey == 0) return -1;
        if (head.officeKey == officeKey)
            return head.headPlusOne ? (int)(head.headPlusOne - 1) : -1;
        slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
    }
    return -1;
}

static unsigned AutoPairHash(uint64_t officeKey, uint64_t targetKey)
{
    uint64_t h = officeKey ^ (targetKey + 0x9E3779B97F4A7C15ull + (officeKey << 6) + (officeKey >> 2));
    h ^= h >> 30; h *= 0xBF58476D1CE4E5B9ull;
    h ^= h >> 27; h *= 0x94D049BB133111EBull;
    h ^= h >> 31;
    return (unsigned)h & (AUTO_MEMO_HASH_SIZE - 1);
}

static int AutoMemoFind(uint64_t officeKey, uint64_t targetKey)
{
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        const unsigned stored = g_autoMemoHash[slot];
        if (stored == 0) return -1;
        const unsigned index = stored - 1;
        if (g_autoMemo[index].officeKey == officeKey &&
            g_autoMemo[index].targetKey == targetKey) return (int)index;
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static int AutoPointerIndex(void* building)
{
    if (!building) return -1;
    uintptr_t h = (uintptr_t)building;
    h >>= 4; h ^= h >> 17; h ^= h >> 9;
    unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
    for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
        AutoPointerSlot& item = g_autoPointerHash[slot];
        if (!item.building) return -1;
        if (item.building == building) return (int)item.entityIndex;
        slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoBuildPointerIndex(unsigned entityCount)
{
    memset(g_autoPointerHash, 0, sizeof(g_autoPointerHash));
    for (unsigned i = 0; i < entityCount; ++i) {
        uintptr_t h = (uintptr_t)g_autoEntities[i].building;
        h >>= 4; h ^= h >> 17; h ^= h >> 9;
        unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
            AutoPointerSlot& item = g_autoPointerHash[slot];
            if (!item.building) {
                item.building = g_autoEntities[i].building;
                item.entityIndex = i;
                inserted = true;
                break;
            }
            if (item.building == g_autoEntities[i].building) { inserted = true; break; }
            slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoMemoInsert(uint64_t officeKey, uint64_t targetKey)
{
    if (g_autoMemoCount >= AUTO_MEMO_MAX) return -1;
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        if (g_autoMemoHash[slot] == 0) {
            const unsigned index = g_autoMemoCount++;
            g_autoMemo[index].officeKey = officeKey;
            g_autoMemo[index].targetKey = targetKey;
            g_autoMemo[index].nextOffice = 0xFFFFFFFFu;
            g_autoMemo[index].state = 1;
            g_autoMemoHash[slot] = index + 1;
            return (int)index;
        }
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoAppendNativeTarget(void* office, void* target, unsigned directions)
{
    if (!g_exeBase || g_exeSize <= AUTO_RVA_TASKCFG_CTOR + 1 ||
        g_exeSize <= AUTO_RVA_VECTOR_PUSH_BACK + 1) return false;
    void** oldBegin = nullptr; size_t oldCount = 0;
    if (!AutoReadTargetVector(office, &oldBegin, &oldCount)) return false;
    (void)oldBegin; (void)oldCount;

    typedef void* (__fastcall *TTaskCfgCtor)(void*);
    typedef void (__fastcall *TVectorPushBack)(void*, const void*);
    TTaskCfgCtor ctor = (TTaskCfgCtor)(g_exeBase + AUTO_RVA_TASKCFG_CTOR);
    TVectorPushBack pushBack = (TVectorPushBack)(g_exeBase + AUTO_RVA_VECTOR_PUSH_BACK);
    if (!ReadablePtr((const void*)ctor, 1) || !ReadablePtr((const void*)pushBack, 1)) return false;

    void* entry = ::operator new(AUTO_TARGET_ENTRY_SIZE, std::nothrow);
    void* cfgMem = ::operator new(AUTO_TASKCFG_SIZE, std::nothrow);
    if (!entry || !cfgMem) {
        if (entry) ::operator delete(entry);
        if (cfgMem) ::operator delete(cfgMem);
        return false;
    }
    memset(entry, 0, AUTO_TARGET_ENTRY_SIZE);
    memset(cfgMem, 0, AUTO_TASKCFG_SIZE);
    void* cfg = nullptr;
    try { cfg = ctor(cfgMem); } catch (...) { cfg = nullptr; }
    if (!cfg) { ::operator delete(entry); ::operator delete(cfgMem); return false; }

    // The native UI adds entries OFF, awaiting its activation checkbox. An
    // autonomous company must request activation itself, not only add the row.
    // Native dirty/upkeep/planner paths still validate route compatibility.
    AutoApplyEntryActivation(entry, directions);
    *(void**)((BYTE*)entry + 0x08) = target;
    *(void**)((BYTE*)entry + 0x10) = cfg;
    // Ghidra reports no resource-filter field in this 0x18-byte TaskEntry.
    // The planner's target/connection index is rebuilt in its working area
    // by reachability; do not invent or write a filter at entry+0x0C.
    AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    *((BYTE*)cfg + 0x28) = 1;                    // native active flag
    *((BYTE*)cfg + 0xF0) = 1;                    // native validated flag

    try {
        // FUN_14008E050(vector, &entry) is the native vector<TaskEntry*>::push_back.
        pushBack((BYTE*)office + AUTO_DO_TARGETS_BEGIN, &entry);
    } catch (...) {
        ::operator delete(entry); ::operator delete(cfgMem);
        return false;
    }
    return true;
}

static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void*)
{
    if (event == TSM_SAVE_LOADING) {
        InterlockedIncrement(&g_fuelDiagnosticEpoch);
        PanelObserverLoading();
    }
    // Persist permission keys only; never retain building pointers. Save/load
    // callbacks run on the file thread, so no game-memory access is performed.
    if (!g_persistence || !dir) return;
    if (event == TSM_SAVE_WRITING) {
        if (!g_persistence->writeAtomic) return;
        BYTE data[12 + AUTO_STATE_PERMISSION_MAX * 9];
        memcpy(data, "PLSTOR18", 8);
        uint32_t count = 0;
        EnterCriticalSection(&g_lock);
        count = g_statePermissionCount;
        memcpy(data + 8, &count, sizeof(count));
        size_t at = 12;
        for (unsigned i = 0; i < g_statePermissionCount; ++i) {
            memcpy(data + at, &g_statePermissions[i].key, sizeof(uint64_t));
            at += sizeof(uint64_t);
            data[at++] = g_statePermissions[i].mode;
        }
        LeaveCriticalSection(&g_lock);
        if (!g_persistence->writeAtomic(dir, AUTO_STATE_PREF_FILE, data, at))
            Logf("[AUTO DO] state permissions were not saved");
    } else if (event == TSM_SAVE_LOADING) {
        if (g_persistence->readFile) {
            size_t len = 0;
            void* raw = g_persistence->readFile(dir, AUTO_STATE_PREF_FILE, &len);
            AutoStatePermission loaded[AUTO_STATE_PERMISSION_MAX];
            unsigned loadedCount = 0;
            const bool legacy = raw && len >= 12 && memcmp(raw, "PLSTOR17", 8) == 0;
            if (raw && len >= 12 && (legacy || memcmp(raw, "PLSTOR18", 8) == 0)) {
                uint32_t count = 0;
                memcpy(&count, (BYTE*)raw + 8, sizeof(count));
                if (count <= AUTO_STATE_PERMISSION_MAX && len == 12 + (size_t)count * 9) {
                    size_t at = 12;
                    for (uint32_t i = 0; i < count; ++i) {
                        uint64_t key = 0;
                        memcpy(&key, (BYTE*)raw + at, sizeof(key));
                        at += sizeof(key);
                        const unsigned char mode = AutoMigratePermission(*((BYTE*)raw + at++), legacy);
                        if (key && mode != 0) {
                            loaded[loadedCount].key = key;
                            loaded[loadedCount].mode = mode;
                            ++loadedCount;
                        }
                    }
                } else {
                    Logf("[AUTO DO] state permission file has invalid length/count");
                }
            } else if (raw) {
                Logf("[AUTO DO] state permission file header invalid");
            }
            if (raw) free(raw);
            EnterCriticalSection(&g_lock);
            memset(g_statePermissions, 0, sizeof(g_statePermissions));
            if (loadedCount) memcpy(g_statePermissions, loaded,
                                    loadedCount * sizeof(loaded[0]));
            g_statePermissionCount = loadedCount;
            LeaveCriticalSection(&g_lock);
        }
        InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 1);
    }
}

static void AutoProbeStateInfrastructure(int closedDay, unsigned entityCount)
{
    g_autoStateStorageCount = 0;
    BYTE* game = g_exeBase ? TsmGameObject(g_exeBase) : nullptr;
    TsmBuildingSpan span;
    if (!game || !TsmGetBuildings(game, &span) ||
        !span.begin || !ReadablePtr(span.begin, span.count * sizeof(void*))) {
        Logf("[AUTO DO] day %d infra probe: world building vector unavailable", closedDay);
        return;
    }

    unsigned stateStorages = 0, privateStorages = 0, customsCount = 0;
    unsigned stateStoragesStored = 0, customsStored = 0;
    for (size_t i = 0; i < span.count; ++i) {
        void* b = span.begin[i];
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b)) continue;
        int type = -1;
        if (!AutoReadBuildingType(b, &type)) continue;
        // Discover storage-bearing sites for enforcement; classification, not
        // mere storage presence, determines allowed directions/visible widgets.
        if (AutoHasNativeStorage(b) && type != AUTO_CUSTOMS_TYPE &&
            type != AUTO_DO_ROAD_TYPE && type != AUTO_DO_RAIL_TYPE) {
            const bool isPrivate = g_enterprise && g_enterprise->isBuildingPrivate &&
                                   g_enterprise->isBuildingPrivate(b);
            if (isPrivate) ++privateStorages;
            else {
                ++stateStorages;
                if (stateStoragesStored < AUTO_STATE_STORAGE_MAX) {
                    AutoStateStorageSite& site = g_autoStateStorageSites[stateStoragesStored++];
                    site.building = b;
                    site.key = 0;
                    AutoBuildingKey(b, &site.key, nullptr);
                    site.permission = site.key ? (unsigned char)(AutoStatePermissionGet(site.key) & AutoBuildingCapabilities(b)) : 0;
                    site.x = 0.0f; site.z = 0.0f;
                    AutoBuildingPosition(b, &site.x, &site.z);
                    AutoBuildingName(b, site.name, sizeof(site.name));
                }
            }
            continue;
        }
        if (type != AUTO_CUSTOMS_TYPE) continue;
        ++customsCount;
        float x = 0.0f, z = 0.0f;
        if (!AutoBuildingPosition(b, &x, &z) || customsStored >= AUTO_CUSTOMS_MAX) continue;
        AutoCustomSite& site = g_autoCustomSites[customsStored++];
        site.building = b; site.x = x; site.z = z;
        AutoBuildingName(b, site.name, sizeof(site.name));
    }

    g_autoStateStorageCount = stateStoragesStored;
    Logf("[AUTO DO] day %d infra probe: world=%llu state_storage=%u private_storage=%u customs=%u customs_indexed=%u",
         closedDay, (unsigned long long)span.count, stateStorages, privateStorages,
         customsCount, customsStored);
    static int previousStateStorages = -1;
    static int previousCustoms = -1;
    if ((int)stateStorages != previousStateStorages || (int)customsCount != previousCustoms) {
        previousStateStorages = (int)stateStorages;
        previousCustoms = (int)customsCount;
        const unsigned show = (stateStoragesStored < 32) ? stateStoragesStored : 32;
        for (unsigned i = 0; i < show; ++i)
            Logf("[AUTO DO] state storage candidate name=%s key=%016llX pos=(%.1f,%.1f) permission=%s",
                 g_autoStateStorageSites[i].name,
                 (unsigned long long)g_autoStateStorageSites[i].key,
                 g_autoStateStorageSites[i].x, g_autoStateStorageSites[i].z,
                 g_autoStateStorageSites[i].permission == AUTO_ALLOW_BOTH ? "LOAD+UNLOAD" :
                 (g_autoStateStorageSites[i].permission == AUTO_ALLOW_LOAD ? "LOAD" :
                  (g_autoStateStorageSites[i].permission == AUTO_ALLOW_UNLOAD ? "UNLOAD" : "DENY")));
        if (stateStoragesStored > show)
            Logf("[AUTO DO] state storage list truncated: %u of %u shown",
                 show, stateStorages);
    }
    for (unsigned i = 0; i < entityCount; ++i) {
        AutoEntity& office = g_autoEntities[i];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        float ox = 0.0f, oz = 0.0f;
        if (!AutoBuildingPosition(office.building, &ox, &oz)) continue;
        int nearest = -1;
        double bestSq = 1.0e100;
        for (unsigned c = 0; c < customsStored; ++c) {
            const double dx = (double)g_autoCustomSites[c].x - ox;
            const double dz = (double)g_autoCustomSites[c].z - oz;
            const double d2 = dx * dx + dz * dz;
            if (d2 < bestSq) { bestSq = d2; nearest = (int)c; }
        }
        if (nearest >= 0)
            Logf("[AUTO DO] day %d nearest customs (geometric, not assigned) office=%016llX customs=%s dist=%.1fm",
                 closedDay, (unsigned long long)office.key,
                 g_autoCustomSites[nearest].name, sqrt(bestSq));
        else
            Logf("[AUTO DO] day %d nearest customs unavailable office=%016llX",
                 closedDay, (unsigned long long)office.key);
    }
}

static bool AutoTaskConfigValid(void* cfg)
{
    if (!cfg || !TsmIsValidGamePtr(cfg) || !ReadablePtr(cfg, AUTO_TASKCFG_SIZE)) return false;
    for (unsigned side = 0; side < 2; ++side) {
        const BYTE* p = (const BYTE*)cfg + (side ? 0xC8 : 0);
        const uintptr_t begin = *(const uintptr_t*)(p + 8);
        const uintptr_t end = *(const uintptr_t*)(p + 16);
        if ((!begin && end) || end < begin || (end - begin) % 16 != 0 ||
            (end - begin) / 16 > 4096) return false;
        if (end != begin && (!TsmIsValidGamePtr((void*)begin) ||
                            !ReadablePtr((void*)begin, end - begin))) return false;
    }
    return true;
}

static bool AutoSetTargetDirections(void* entry, unsigned directions)
{
    if (!entry || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) return false;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) {
        Logf("[AUTO DO] invalid task config; cannot enforce direction mask=%u", directions);
        return false;
    }
    const bool audit = g_autoLogLines < (unsigned)g_autoTargetLogLimit;
    AutoTaskAudit before = {};
    if (audit) before = AutoReadTaskAudit(entry, cfg);
    const bool directionsChanged = AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    const bool activationChanged = AutoApplyEntryActivation(entry, directions);
    const bool changed = directionsChanged || activationChanged;
    if (changed && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        Logf("[AUTO DO] task directions target=%p load=%u unload=%u entry_enabled=%u (native route validation still required)",
             *(void**)((BYTE*)entry + 0x08), (directions & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (directions & AUTO_ALLOW_UNLOAD) ? 1u : 0u, (unsigned)*(BYTE*)entry);
        ++g_autoLogLines;
    }
    if (changed && audit && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        const AutoTaskAudit after = AutoReadTaskAudit(entry, cfg);
        Logf("[AUTO DO AUDIT] target=%p changed_fields=0x%02X enabled=%u->%u load_all=%u->%u unload_all=%u->%u load_pct=%.9g->%.9g unload_pct=%.9g->%.9g load_bits=%08X->%08X unload_bits=%08X->%08X",
             *(void**)((BYTE*)entry + 8), AutoTaskAuditChanges(before, after),
             (unsigned)before.enabled, (unsigned)after.enabled,
             (unsigned)before.loadAll, (unsigned)after.loadAll,
             (unsigned)before.unloadAll, (unsigned)after.unloadAll,
             AutoAuditPercent(before.loadBits), AutoAuditPercent(after.loadBits),
             AutoAuditPercent(before.unloadBits), AutoAuditPercent(after.unloadBits),
             (unsigned)before.loadBits, (unsigned)after.loadBits,
             (unsigned)before.unloadBits, (unsigned)after.unloadBits);
        ++g_autoLogLines;
    }
    return changed;
}

static void AutoLogNativeTarget(void* entry, int type, unsigned requested, bool preserve)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) return; // enforcement path reports invalid configs
    const BYTE* p = (const BYTE*)cfg;
    const uintptr_t lb = *(const uintptr_t*)(p + 8), le = *(const uintptr_t*)(p + 16);
    const uintptr_t ub = *(const uintptr_t*)(p + 0xD0), ue = *(const uintptr_t*)(p + 0xD8);
    Logf("[AUTO DO] native target=%p type=%d policy=%s requested_mask=%u enabled=%u load_all=%u load_pct=%.3f load_selectors=%llu unload_all=%u unload_pct=%.3f unload_selectors=%llu",
         *(void**)((BYTE*)entry + 8), type, preserve ? "preserve-customs" : "managed",
         requested, (unsigned)*(BYTE*)entry, (unsigned)p[0], (double)*(const float*)(p + 4),
         (unsigned long long)((le - lb) / 16), (unsigned)p[0xC8], (double)*(const float*)(p + 0xCC),
         (unsigned long long)((ue - ub) / 16));
    ++g_autoLogLines;
}


// Called only from daily reconciliation; does not arm, clear or advance timers.
static void AutoLogOfficePlanState(void* office, uint64_t key, int day,
                                   const char* phase, bool notificationRequested)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    const BYTE* p = (const BYTE*)office;
    const bool dirtyReadable = ReadablePtr(p + AUTO_DO_DIRTY, 1);
    const bool timerReadable = ReadablePtr(p + AUTO_DO_UI_TIMER, sizeof(float));
    float timer = 0.0f;
    if (timerReadable) memcpy(&timer, p + AUTO_DO_UI_TIMER, sizeof(timer));
    Logf("[AUTO DO AUDIT] day=%d office=%p key=%016llX phase=%s notification_requested=%u dirty=%d timer_readable=%u countdown=%.9g",
         day, office, (unsigned long long)key, phase, notificationRequested ? 1u : 0u,
         dirtyReadable ? (int)p[AUTO_DO_DIRTY] : -1, timerReadable ? 1u : 0u, (double)timer);
    ++g_autoLogLines;
}

static void AutoTargetsOnDay(int day, int closedDay)
{
    if (!g_autoTargetsEnabled || !g_enterprise ||
        !g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex ||
        !g_enterprise->isBuildingPrivate) return;

    const bool afterLoad = (InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 0) != 0) ||
                           (g_autoLastDay >= 0 && day < g_autoLastDay);
    if (afterLoad)
        Logf("[AUTO DO] day %d save/load detected: native TaskEntry/TaskCfg were deserialized; reconciling generated targets after load", closedDay);
    g_autoLastDay = day;

    const size_t n = g_enterprise->getEnterpriseCount();
    if (n > AUTO_ENTITY_MAX) {
        Logf("[AUTO DO] day %d skipped: registered count %llu exceeds fixed discovery capacity %u",
             closedDay, (unsigned long long)n, (unsigned)AUTO_ENTITY_MAX);
        return;
    }

    unsigned entityCount = 0, officeCount = 0, candidateCount = 0;
    for (size_t i = 0; i < n; ++i) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b) ||
            !g_enterprise->isBuildingPrivate(b)) continue;
        uint64_t key = 0; int type = -1;
        if (!AutoBuildingKey(b, &key, &type)) continue;
        const bool isOffice = (type == AUTO_DO_ROAD_TYPE || type == AUTO_DO_RAIL_TYPE);
        const unsigned capabilities = isOffice ? 0 : AutoBuildingCapabilities(b);
        const bool hasStorage = isOffice || capabilities != 0;
        if (!hasStorage) continue;
        g_autoEntities[entityCount].building = b;
        g_autoEntities[entityCount].key = key;
        g_autoEntities[entityCount].type = type;
        g_autoEntities[entityCount].hasStorage = isOffice ? 0 : 1;
        g_autoEntities[entityCount].isStateStorage = 0;
        g_autoEntities[entityCount].statePermission = (unsigned char)capabilities;
        g_autoEntities[entityCount].targetPresent = 0;
        ++entityCount;
        if (isOffice) ++officeCount; else ++candidateCount;
    }

    AutoProbeStateInfrastructure(closedDay, entityCount);

    // Public access defaults deny. Keep denied sites indexed too: existing
    // native/manual entries must obey the same state permissions as new ones.
    for (unsigned si = 0; si < g_autoStateStorageCount && entityCount < AUTO_ENTITY_MAX; ++si) {
        AutoStateStorageSite& site = g_autoStateStorageSites[si];
        if (!site.building || !site.key) continue;
        AutoEntity& e = g_autoEntities[entityCount++];
        e.building = site.building; e.key = site.key; e.type = AUTO_STORAGE_BUILDING_TYPE;
        e.hasStorage = site.permission != 0;
        e.isStateStorage = 1; e.statePermission = site.permission; e.targetPresent = 0;
        if (site.permission != 0) ++candidateCount;
    }

    if (officeCount == 0) return;
    if (!AutoBuildPointerIndex(entityCount)) {
        Logf("[AUTO DO] day %d: pointer index capacity exhausted; scan aborted", closedDay);
        return;
    }
    if (!AutoBuildOfficeMemoIndex()) {
        Logf("[AUTO DO] day %d: office memo index capacity exhausted; scan aborted", closedDay);
        return;
    }

    unsigned planned = 0, added = 0, existing = 0, suppressed = 0, invalid = 0;
    g_autoLogLines = 0;
    for (unsigned oi = 0; oi < entityCount; ++oi) {
        AutoEntity& office = g_autoEntities[oi];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        void** entries = nullptr; size_t entryCount = 0;
        if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
            Logf("[AUTO DO] day %d invalid target vector; skipped office key=%016llX",
                 closedDay, (unsigned long long)office.key);
            ++invalid; continue;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-reconcile", false);
        const size_t entryCountAtStart = entryCount;
        for (unsigned ei = 0; ei < entityCount; ++ei) g_autoEntities[ei].targetPresent = 0;
        bool doConfigChanged = false;
        for (size_t ei = 0; ei < entryCount; ++ei) {
            void* entry = entries[ei];
            if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
            void* targetBuilding = *(void**)((BYTE*)entry + 0x08);
            if (!targetBuilding || !TsmIsValidGamePtr(targetBuilding) ||
                !ReadablePtr(targetBuilding, TSM_BLD_MIN_READABLE)) continue;
            int targetType = -1;
            if (AutoReadBuildingType(targetBuilding, &targetType) && AutoPreserveManualCustoms(targetType)) {
                // Border trade is not a state-enterprise permission. Preserve
                // the user's native customs task, including its resource lists
                // and activation. Customs auto-selection is not implemented.
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] preserving manual customs target=%p entry_enabled=%u",
                         targetBuilding, (unsigned)*(BYTE*)entry);
                    ++g_autoLogLines;
                }
                AutoLogNativeTarget(entry, targetType, 0, true);
                continue;
            }
            const int targetIndex = AutoPointerIndex(targetBuilding);
            if (targetIndex >= 0) g_autoEntities[targetIndex].targetPresent = 1;
            // Enforce all targets of this PRIVATE office, including manually
            // present public entries, unindexed/unknown roles and default OFF.
            const unsigned capabilities = AutoBuildingCapabilities(targetBuilding);
            const bool isPrivate = g_enterprise->isBuildingPrivate(targetBuilding) != 0;
            unsigned permissions = 0;
            if (!isPrivate) {
                uint64_t key = 0;
                permissions = AutoBuildingKey(targetBuilding, &key, nullptr) ?
                    AutoStatePermissionGet(key) : 0;
            }
            const unsigned directions = AutoEffectiveDirections(isPrivate, permissions, capabilities);
            if (!g_autoTargetsDryRun)
                doConfigChanged |= AutoSetTargetDirections(entry, directions);
            AutoLogNativeTarget(entry, targetType, directions, false);
        }

        // Native upkeep can disable an entry in place: keep deduplication for
        // entries still present. Missing generated entries are eligible for
        // recreation regardless of why they disappeared. A private company
        // owns its target list; manual removal is not a persistent veto.
        // State-building permissions remain a separate eligibility gate.
        for (int mi = AutoOfficeMemoHeadIndex(office.key); mi >= 0;
             mi = g_autoMemo[mi].nextOffice == 0xFFFFFFFFu ? -1 : (int)g_autoMemo[mi].nextOffice) {
            AutoMemo& memo = g_autoMemo[mi];
            if (memo.state != 1) continue;
            bool found = false;
            for (size_t ei = 0; ei < entryCount; ++ei) {
                void* entry = entries[ei];
                if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
                void* target = *(void**)((BYTE*)entry + 0x08);
                const int targetIndex = AutoPointerIndex(target);
                if (targetIndex >= 0 && g_autoEntities[targetIndex].key == memo.targetKey) {
                    found = true; break;
                }
            }
            if (!found) {
                memo.state = 3; // preserve hash chain; allow normal eligibility/re-add
                Logf("[AUTO DO] day %d target key=%016llX absent; eligible for automatic reconnection",
                     closedDay, (unsigned long long)memo.targetKey);
            }
        }

        unsigned doPlanned = 0, doAdded = 0, doExisting = 0, doSuppressed = 0;
        size_t projectedAdds = 0;
        // State storages were appended to g_autoEntities after private sites;
        // walk backward so an explicit state-source opt-in gets first access
        // to the five-entry conservative safe budget.
        for (unsigned ti = entityCount; ti > 0; --ti) {
            AutoEntity& target = g_autoEntities[ti - 1];
            if (!target.hasStorage || target.building == office.building) continue;
            if (target.targetPresent) {
                ++existing; ++doExisting; continue;
            }
            const int memoIndex = AutoMemoFind(office.key, target.key);
            if (entryCountAtStart + projectedAdds >= AUTO_DO_SAFE_TARGET_COUNT) {
                ++suppressed; ++doSuppressed;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d target=%016llX omitted: conservative target count %u reached (existing=%llu)",
                         closedDay, (unsigned long long)target.key,
                         (unsigned)AUTO_DO_SAFE_TARGET_COUNT,
                         (unsigned long long)entryCountAtStart);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++planned; ++doPlanned;
            if (g_autoTargetsDryRun) {
                ++projectedAdds;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d DRY RUN office=%016llX target=%016llX type=%d",
                         closedDay, (unsigned long long)office.key,
                         (unsigned long long)target.key, target.type);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (memoIndex < 0 && g_autoMemoCount >= AUTO_MEMO_MAX) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d memo capacity %u reached; target omitted",
                         closedDay, (unsigned)AUTO_MEMO_MAX);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (!AutoAppendNativeTarget(office.building, target.building, target.statePermission)) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d append failed office=%016llX target=%016llX",
                         closedDay, (unsigned long long)office.key, (unsigned long long)target.key);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++projectedAdds;
            if (memoIndex < 0) {
                if (AutoMemoInsert(office.key, target.key) < 0) {
                    ++invalid;
                    Logf("[AUTO DO] day %d memo hash insertion failed after native append", closedDay);
                }
            } else {
                g_autoMemo[memoIndex].state = 1;
            }
            target.targetPresent = 1;
            ++added; ++doAdded;
            entries = nullptr; entryCount = 0;
            if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
                ++invalid; break;
            }
        }

        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-notify", doAdded > 0 || doConfigChanged);
        if (doAdded > 0 || doConfigChanged) {
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_DIRTY, 1))
                *((BYTE*)office.building + AUTO_DO_DIRTY) = 1;
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_UI_TIMER, sizeof(float)))
                *(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "after-notify", doAdded > 0 || doConfigChanged);
        if (doPlanned || doExisting || doSuppressed || doAdded || doConfigChanged)
            Logf("[AUTO DO] day %d office=%016llX type=%d existing=%u planned=%u added=%u config_changed=%u suppressed=%u",
                 closedDay, (unsigned long long)office.key, office.type, doExisting,
                 doPlanned, doAdded, doConfigChanged ? 1u : 0u, doSuppressed);
    }

    Logf("[AUTO DO] day %d summary: private offices=%u candidates=%u planned=%u added=%u existing=%u suppressed=%u invalid=%u mode=%s",
         closedDay, officeCount, candidateCount, planned, added, existing, suppressed, invalid,
         g_autoTargetsDryRun ? "DRY RUN" : "WRITE");
}

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_logistics.ini";
    char v[256];

    g_enabled        = H->configInt(ini, "private_logistics", "enabled", 1);
    g_billing        = H->configInt(ini, "private_logistics", "billing", 0);
    g_panelObserverEnabled = H->configInt(ini, "private_logistics", "panel_fuel_observer", 1);
    g_dryRun         = H->configInt(ini, "private_logistics", "dry_run", 1);
    g_payFactory     = H->configInt(ini, "private_logistics", "pay_factory", 1);
    g_payCarrier     = H->configInt(ini, "private_logistics", "pay_carrier", 1);
    g_logCapPerDay   = H->configInt(ini, "private_logistics", "log_cap_per_day", 400);
    g_ledgerCapacity = H->configInt(ini, "private_logistics", "ledger_capacity", 512);
    g_siteLog        = H->configInt(ini, "private_logistics", "site_log", 1);
    g_autoTargetsEnabled = H->configInt(ini, "private_logistics", "auto_targets_enabled", 1);
    g_autoTargetsDryRun  = H->configInt(ini, "private_logistics", "auto_targets_dry_run", 0);
    g_autoTargetLogLimit = H->configInt(ini, "private_logistics", "auto_targets_log_limit", 80);
    g_autoSourcePercent = AutoClampSourcePercent(H->configInt(ini, "private_logistics", "auto_source_percent", 10));
    g_autoDestinationPercent = AutoClampDestinationPercent(H->configInt(ini, "private_logistics", "auto_destination_percent", 30));
    Logf("[AUTO DO] company thresholds: load above %d%% / deliver below %d%% (managed tasks, reconciled daily)",
         g_autoSourcePercent, g_autoDestinationPercent);
    if (g_autoSourcePercent != 10)
        Logf("[AUTO DO] source threshold override=%d%% (this test ships 10%%); an explicit deployed INI setting overrides the fallback", g_autoSourcePercent);
    Logf("[AUTO DO] permission-test profile: billing=%d auto_targets=%d apply=%d (expected 0/1/1)",
         g_billing, g_autoTargetsEnabled, !g_autoTargetsDryRun);
    if (g_billing || !g_autoTargetsEnabled || g_autoTargetsDryRun)
        Logf("[AUTO DO] WARNING deployed INI overrides the permission-test defaults; rebuild/deploy the shipped INI for this test");
    if (H->configInt(ini, "private_logistics", "state_storage_always_on", 0))
        Logf("[AUTO DO] state_storage_always_on is obsolete and ignored; per-building permissions are required");
    if (g_autoTargetLogLimit < 0) g_autoTargetLogLimit = 0;
    if (g_autoTargetLogLimit > 1000) g_autoTargetLogLimit = 1000;

    if (g_logCapPerDay < 0)            g_logCapPerDay = 0;
    if (g_logCapPerDay > 5000)         g_logCapPerDay = 5000;
    if (g_ledgerCapacity < 1)          g_ledgerCapacity = 1;
    if (g_ledgerCapacity > LEDGER_MAX) g_ledgerCapacity = LEDGER_MAX;

    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), ""))
        g_freightShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "factory_share", v, sizeof(v), ""))
        g_factoryShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "min_payment", v, sizeof(v), ""))
        g_minPayment = (float)atof(v);
    if (H->configString(ini, "private_logistics", "daily_pay_cap", v, sizeof(v), ""))
        g_dailyPayCap = atof(v);

    // v0.5: the tariff model. Every number here is a physical quantity or a
    // ratio - litres, tonnes per litre, a percentage. Not one of them is a
    // price in currency units, which is what makes the model survive a
    // currency event.
    g_tariffProbe     = H->configInt(ini, "private_logistics", "tariff_probe", 1);
    g_balanceLog      = H->configInt(ini, "private_logistics", "balance_log", 1);
    g_tariffLogPerDay = H->configInt(ini, "private_logistics", "tariff_log_per_day", 60);

    if (H->configString(ini, "private_logistics", "litres_per_trip", v, sizeof(v), ""))
        gLitresPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "litres_per_tonne", v, sizeof(v), ""))
        gLitresPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "tariff_margin", v, sizeof(v), ""))
        gTariffMargin = atof(v);
    if (H->configString(ini, "private_logistics", "diesel_density", v, sizeof(v), ""))
        gDieselDensity = atof(v);
    g_tripTariff = H->configInt(ini, "private_logistics", "trip_tariff", 1);
    g_censusLog  = H->configInt(ini, "private_logistics", "census_log", 1);
    g_vehScan    = H->configInt(ini, "private_logistics", "veh_scan", 1);
    g_vehScanLogPerDay = H->configInt(ini, "private_logistics", "veh_scan_log_per_day", 40);
    // v0.11: raw cargo-vector dump, up to three vehicles per close.
    g_cargoDump  = H->configInt(ini, "private_logistics", "cargo_dump", 1);
    if (g_vehScanLogPerDay < 0)   g_vehScanLogPerDay = 0;
    if (g_vehScanLogPerDay > 512) g_vehScanLogPerDay = 512;
    g_litresAuto = H->configInt(ini, "private_logistics", "litres_auto", 0);
    if (H->configString(ini, "private_logistics", "wear_litres", v, sizeof(v), ""))
        gWearLitres = atof(v);

    // v0.7: freight_share is RETIRED. Honouring it silently would make every
    // delivery look mispriced against a tariff that no longer reads it, so a
    // key left in the ini is reported and zeroed rather than obeyed.
    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), "") && v[0]) {
        const float f = (float)atof(v);
        g_freightShareRetired = (f > 0.0f) ? f : 0.0f;
        g_freightShare = 0.0f;
    }
    if (H->configString(ini, "private_logistics", "baskets_per_trip", v, sizeof(v), ""))
        gBasketsPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "baskets_per_tonne", v, sizeof(v), ""))
        gBasketsPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "fuel_resource", v, sizeof(v), "") && v[0])
        strncpy_s(gFuelResource, sizeof(gFuelResource), v, _TRUNCATE);

    // v0.4: a share is a fraction of cargo value. 0 is legal (freight off);
    // more than half would make haulage cost more than the goods.
    if (g_freightShare < 0.0f)   g_freightShare = 0.0f;
    if (g_freightShare > 0.50f)  g_freightShare = 0.50f;
    if (g_tripTariff != 0)       g_tripTariff   = 1;
    if (gWearLitres  < 0.0)      gWearLitres    = 0.0;
    if (gWearLitres  > 500.0)    gWearLitres    = 500.0;
    if (g_litresAuto != 1)       g_litresAuto   = 0;
    if (g_factoryShare  < 0.0f) g_factoryShare  = 0.0f;
    if (g_factoryShare  > 5.0f) g_factoryShare  = 5.0f;
    if (g_minPayment    < 0.0f) g_minPayment    = 0.0f;
    if (g_dailyPayCap   < 0.0)  g_dailyPayCap   = 0.0;

    // v0.5 clamps. Density is the one hard range: a value outside it is a
    // typo, and a typo here would misprice every delivery by orders of
    // magnitude, so it falls back to diesel rather than being honoured.
    if (g_tariffLogPerDay < 0)    g_tariffLogPerDay = 0;
    if (g_tariffLogPerDay > 5000) g_tariffLogPerDay = 5000;
    if (gLitresPerTrip  < 0.0)   gLitresPerTrip  = 0.0;
    if (gLitresPerTonne < 0.0)   gLitresPerTonne = 0.0;
    if (gLitresPerTrip  > 500.0) gLitresPerTrip  = 500.0;
    if (gLitresPerTonne > 50.0)  gLitresPerTonne = 50.0;
    if (gTariffMargin   < 0.0)   gTariffMargin   = 0.0;
    if (gTariffMargin   > 5.0)   gTariffMargin   = 5.0;
    if (!(gDieselDensity > 0.0001) || gDieselDensity > 0.01)
        gDieselDensity = 0.00084;
    if (gBasketsPerTrip  < 0.0)  gBasketsPerTrip  = 0.0;
    if (gBasketsPerTonne < 0.0)  gBasketsPerTonne = 0.0;
    if (gBasketsPerTrip  > 1000.0) gBasketsPerTrip = 1000.0;
    if (gBasketsPerTonne > 100.0)  gBasketsPerTonne = 100.0;

    // site_list, e.g. "0x1CB7FF" or "1CB7FF,68F792". Absent or empty = all.
    if (H->configString(ini, "private_logistics", "site_list", v, sizeof(v), "") && v[0]) {
        bool hit[MAX_SITES] = { false };
        int  matched = 0;

        for (char* p = v; *p; ) {
            while (*p == ' ' || *p == '\t' || *p == ',') p++;
            if (!*p) break;

            char* end = p;
            while (*end && *end != ',' && *end != ' ' && *end != '\t') end++;

            const char save = *end;
            *end = 0;
            const unsigned want = (unsigned)strtoul(p, nullptr, 16);
            *end = save;

            for (int i = 0; i < g_siteCount; i++) {
                if (g_sites[i].rva == want) { hit[i] = true; matched++; break; }
            }
            p = end;
        }

        if (matched > 0) {
            for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = hit[i];
            g_siteFilterActive = true;
        } else {
            Logf("private_logistics  WARN  site_list matched no known site - patching all");
        }
    }
}

// ---------------------------------------------------------------------------
// v0.8: the vehicle census
// ---------------------------------------------------------------------------
//
// Read-only, once a day, on the day callback. Every pointer is validated
// before it is followed, every vector length is capped, and nothing here can
// fail a delivery: a bad read reports "unreadable" and moves on.
//
// The numbers this produces are the ones the final model needs and no formula
// can supply:
//   - how many vehicles a company actually owns
//   - what each one carries at its peak (its capacity, observed not assumed)
//   - which catalogue entry the game attaches to it (+0x510, unproven)

static int VehSlot(void* v)
{
    for (int i = 0; i < g_vehCount; i++)
        if (g_vehPtr[i] == v) return i;
    if (g_vehCount >= VEH_MAX) return -1;
    g_vehPtr[g_vehCount]     = v;
    g_vehPeakTons[g_vehCount] = 0.0;
    g_vehSamples[g_vehCount]  = 0;
    return g_vehCount++;
}

// veh+0x3B8 is a vector of cargo entries, stride 0x10: { Resource* , float tons }.
// Returns the total carried and the number of non-empty slots.
static double CargoTons(void* veh, int* slots)
{
    *slots = 0;
    const BYTE* p = (const BYTE*)veh;
    if (!ReadablePtr(p + 0x3B8, 16)) return 0.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end <= beg) return 0.0;

    // v0.12: the cap was 8, and over 8 it returned 0 SILENTLY - which is how
    // this census came to report "0 slot(s)" on trucks that demonstrably held
    // fuel. A storage vector with a pre-allocated slot per resource type is
    // perfectly plausible, and it would be longer than eight. Raise the cap and
    // stop hiding the case.
    size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return 0.0;

    double t = 0.0;
    for (size_t i = 0; i < n; i++) {
        const float tons = *(const float*)(beg + i * 16 + 8);
        if (tons == tons && tons > 0.0f && tons < 1.0e6f) {
            t += (double)tons;
            (*slots)++;
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// Candidate resource-slot and display-field readers, NOT authoritative fuel.
// The old building/upkeep report does not prove this road truck's burn or
// refill writer, canonical store, native clamp, or physical mass conversion.
// ---------------------------------------------------------------------------
static void* FuelResourcePtr(void)
{
    if (!g_pricing || !g_pricing->getByName) return nullptr;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    return g_fuelRes;
}

// Returns raw quantity from a matching resource slot. Units/classification are
// unverified for this road truck. *found = 1 even if the quantity is invalid.
// -1 when the vector cannot be read at all.
static double FuelSlotTons(void* v, int* found)
{
    *found = 0;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x3B8, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) return -1.0;
    if (n == 0) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    void* fuel = FuelResourcePtr();
    if (!fuel) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* e = beg + i * 16;
        if (*(const void* const*)e != fuel) continue;
        float t = *(const float*)(e + 8);
        // Preserve invalid/negative raw values: diagnostics must never invent
        // an empty tank by applying an unverified native clamp.
        *found = 1;
        return (double)t;
    }
    return 0.0;
}

// The panel backing quantity (+0x5F0) and metadata capacity (+0x8684).
// Display correlation alone does not establish native burn/refill semantics.
static double MirrorTons(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_FUEL_OFF, 4)) return -1.0;
    float f = *(const float*)(p + VEH_FUEL_OFF);
    if (!(f == f) || f < -1000.0f || f > 1000.0f) return -1.0;
    return (double)f;
}

static double TankCapacity(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x1708, 8)) return -1.0;
    const BYTE* meta = *(const BYTE* const*)(p + 0x1708);
    if (!meta || !TsmIsValidGamePtr(meta)) return -1.0;
    if (!ReadablePtr(meta + 0x8684, 4)) return -1.0;
    const float c = *(const float*)(meta + 0x8684);
    if (!(c == c) || c <= 0.0f || c > 1000.0f) return -1.0;
    return (double)c;
}

// v0.11: sum of one building's fleet tanks, in tonnes. -1 = not readable.
static double FleetFuelTons(void* b, int* outCount)
{
    *outCount = 0;
    if (!b || !ReadablePtr((const BYTE*)b + 0xC70, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
    const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 8;
    if (n == 0 || n > VEH_MAX) return -1.0;
    if (!ReadablePtr(beg, n * 8)) return -1.0;

    double t = 0.0;
    int c = 0;
    for (size_t i = 0; i < n; i++) {
        void* v = *(void* const*)(beg + i * 8);
        if (!v) continue;
        int found = 0;
        double lvl = FuelSlotTons(v, &found);
        const auto reading = private_logistics_fuel_diagnostics::Select(
            found != 0, lvl, MirrorTons(v));
        if (!reading.valid) continue;
        t += reading.value;
        c++;
    }
    *outCount = c;
    return (c > 0) ? t : -1.0;
}

// One sample per committed delivery. Cheap: one vector walk and a few floats.
static void BurnProbe(void* b)
{
    int n = 0;
    const double now = FleetFuelTons(b, &n);
    if (now < 0.0) return;

    if (g_probeBld != b || g_probeFuelTons < 0.0 || n != g_probeVehCount) {
        g_probeBld      = b;                     // first sample, or a new fleet
        g_probeFuelTons = now;
        g_probeVehCount = n;
        return;
    }

    const double d = g_probeFuelTons - now;      // positive = burned
    if (d < -0.02 || d > 0.5) {
        g_probeSkipRun++;                        // refill, or the fleet changed size
    } else {
        g_probeBurnRun  += d;
        g_probeStepsRun++;
    }
    g_probeFuelTons = now;
}

// v0.12: RAW storage-vector dump. Two vectors, every slot, nothing filtered.
//
// Why this exists: the vehicle appears to keep its fuel in a
// {Resource*, float tons} slot, the same shape the transfer function moves
// goods with. If so, one of the vectors below holds diesel and the slot will
// track the +0x5F0 reading day after day.
//
// Two things went wrong in v0.10 and are fixed here:
//   - the census capped the vector at eight slots and returned "0 slot(s)"
//     for anything longer, which is why a truck holding 0.29 t of fuel could
//     report an empty hold. That was OUR bug, not evidence about the game.
//   - only the 0x3B8 vector was read. The trailer vector at 0x3A0 was never
//     looked at, and fuel may live there.
//
// A slot whose tonnage falls every day and jumps at a refill is the tank.
static void DumpVector(void* v, int off, const char* label, int closedDay, int slotIdx)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + off, 16)) return;

    const BYTE* beg = *(const BYTE* const*)(p + off);
    const BYTE* end = *(const BYTE* const*)(p + off + 8);
    if (!beg || !end || end < beg) {
        Logf("[CARGO] day %d s=%d %s unreadable beg=%p end=%p",
             closedDay, slotIdx, label, beg, end);
        return;
    }

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) {
        Logf("[CARGO] day %d s=%d %s n=%llu (implausible)",
             closedDay, slotIdx, label, (unsigned long long)n);
        return;
    }
    if (n == 0) {
        Logf("[CARGO] day %d s=%d %s n=0", closedDay, slotIdx, label);
        return;
    }
    if (!ReadablePtr(beg, n * 16)) return;

    Logf("[CARGO] day %d s=%d %s n=%llu", closedDay, slotIdx, label,
         (unsigned long long)n);
    const size_t show = (n < 16) ? n : 16;
    for (size_t i = 0; i < show; i++) {
        const BYTE* e   = beg + i * 16;
        const void* res = *(const void* const*)e;
        const float t   = *(const float*)(e + 8);
        if (!(t > 0.0f) && !(t < 0.0f)) continue;      // skip empty slots
        Logf("[CARGO] day %d s=%d %s   %llu res=%p %.4f t", closedDay, slotIdx,
             label, (unsigned long long)i, res, (double)t);
    }
}

static void CargoDump(int slot, void* v, int closedDay)
{
    if (!g_cargoDump || slot < 0 || slot > 2) return;   // first three vehicles
    DumpVector(v, 0x3A0, "trailer", closedDay, slot);
    DumpVector(v, 0x3B8, "cargo",   closedDay, slot);
}

// ---------------------------------------------------------------------------
// v0.9: the field scanner
// ---------------------------------------------------------------------------
//
// Compare the vehicle's first 0x800 bytes against yesterday's copy and report
// the words that moved. The baseline is taken silently on first sight so that
// day one does not dump a whole struct.
//
// Known pointer/vector ranges are skipped: they are noise here and the values
// would only ever read as "some address".
//
//   +0x3A0..0x3C8  trailer vector, cargo vector
//   +0x4F0..0x520  current building, home building, the +0x510 integer
//   +0x610..0x620  line stops
static bool VehScanSkip(int off)
{
    return (off >= 0x3A0 && off < 0x3C8) ||
           (off >= 0x4F0 && off < 0x520) ||
           (off >= 0x610 && off < 0x620);
}

// ---------------------------------------------------------------------------
// v0.10: read the tank
// ---------------------------------------------------------------------------
//
// Legacy daily candidate sampling, NOT a native deduction/refill adapter.
// Reading, provenance and baseline changes are explicit. A negative candidate
// is invalid, not zero; changing source, invalid reads and load reset baselines.
static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
{
    if (slot < 0 || slot >= VEH_MAX) return;
    using namespace private_logistics_fuel_diagnostics;
    int found = 0;
    const double slotRaw = FuelSlotTons(v, &found);
    const double displayRaw = MirrorTons(v);
    const Reading reading = Select(found != 0, slotRaw, displayRaw);
    const Result result = g_vehFuelSamples[slot].Observe(reading,
        (unsigned long long)InterlockedCompareExchange(&g_fuelDiagnosticEpoch, 0, 0));
    g_vehFuelValid[slot] = reading.valid;
    if (reading.valid) g_vehFuel[slot] = reading.value;
    // Legacy accumulators are candidate sample changes ONLY. No tracker feed.
    if (burned) *burned += result.decrease;
    if (refills && result.change == Change::Increase) ++*refills;
    if (g_tankCrossLog > 0) {
        Logf("[TANK] day %d v=%p selected_source=%s source_valid=%d "
             "slot_found=%d slot_candidate_raw=%.9g display_5f0_raw=%.9g "
             "drift_valid=%d drift_raw=%+.9g change=%s decrease_raw=%.9g "
             "units=unverified exact_burn=0 verified_refill=0 billing_input=0",
             closedDay, v, SourceName(reading.source), reading.valid ? 1 : 0,
             found, slotRaw, displayRaw, reading.comparable ? 1 : 0,
             reading.drift, ChangeName(result.change), result.decrease);
        --g_tankCrossLog;
    }
}

static void ScanVehicle(int slot, void* v, int closedDay)
{
    if (!g_vehScan || slot < 0 || slot >= VEH_MAX) return;
    if (!ReadablePtr((const BYTE*)v, VEH_SCAN_WORDS * 4)) return;

    const unsigned* src = (const unsigned*)v;

    if (!g_vehSnapValid[slot]) {
        for (int i = 0; i < VEH_SCAN_WORDS; i++) g_vehSnap[slot][i] = src[i];
        g_vehSnapValid[slot] = true;
        return;                                   // baseline, not a delta
    }

    int logged = 0;
    int changed = 0;
    for (int i = 0; i < VEH_SCAN_WORDS; i++) {
        const int off = i * 4;
        if (g_vehSnap[slot][i] == src[i]) continue;
        changed++;
        if (VehScanSkip(off)) { g_vehSnap[slot][i] = src[i]; continue; }
        if (logged < g_vehScanLogPerDay) {
            const unsigned a = g_vehSnap[slot][i];
            const unsigned b = src[i];
            float fa, fb;
            memcpy(&fa, &a, 4);
            memcpy(&fb, &b, 4);
            Logf("[VEH CHG] day %d v=%p +0x%03X %08X -> %08X (f %.4f -> %.4f)",
                 closedDay, v, off, a, b, (double)fa, (double)fb);
            logged++;
        }
        g_vehSnap[slot][i] = src[i];
    }
    if (changed > logged)
        Logf("[VEH CHG] day %d v=%p %d more word(s) changed, not logged",
             closedDay, v, changed - logged);
}

static void CensusFleets(int closedDay)
{
    if (!g_censusLog || !g_enterprise) return;
    if (!g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex) return;

    size_t n = g_enterprise->getEnterpriseCount();
    if (n == 0) return;
    if (n > 32) n = 32;                          // a republic, not a spreadsheet

    // v0.11: one close counted once, not once per building. L per fleet-day is
    // the rate the physics has; it must not double-count a fleet seen from two
    // enterprises.
    const unsigned vehDaysBefore = g_tankVehDaysRun;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr((const BYTE*)b + 0xC70, 16)) continue;

        const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
        const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
        if (!beg || !end || end < beg) continue;

        size_t cnt = (size_t)(end - beg) / sizeof(void*);
        if (cnt > 64) cnt = 64;
        if (!ReadablePtr(beg, cnt * sizeof(void*))) continue;

        char bname[64];
        BuildingName(b, bname, sizeof(bname));

        if (cnt == 0) {
            Logf("[VEH] day %d %s fleet: 0 vehicle(s)", closedDay, bname);
            continue;
        }
        Logf("[VEH] day %d %s fleet: %u vehicle(s)", closedDay, bname, (unsigned)cnt);

        for (size_t k = 0; k < cnt; k++) {
            void* v = ((void* const*)beg)[k];
            if (!v || !TsmIsValidGamePtr(v)) continue;

            void* home = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x4F8, 8))
                home = *(void* const*)((const BYTE*)v + 0x4F8);

            void* cat = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x510, 8))
                cat = *(void* const*)((const BYTE*)v + 0x510);

            // v0.12: the Ghidra report says the vehicle KIND is an int at
            // +0x50C, and that 0x42 marks a fuel consumer. +0x510, which this
            // census has been printing as a pointer, is the next field along -
            // which is why it kept printing nonsense like FFFFFFFF. If every
            // truck reads 66 here, that claim is confirmed; if none does, the
            // report has the wrong offset and we say so.
            int kind = -1;
            if (ReadablePtr((const BYTE*)v + 0x50C, 4))
                kind = *(const int*)((const BYTE*)v + 0x50C);

            int slots = 0;
            const double tons = CargoTons(v, &slots);
            CargoDump(VehSlot(v), v, closedDay);

            const int s = VehSlot(v);
            const bool first = (s >= 0 && g_vehSamples[s] == 0);
            if (s >= 0) {
                g_vehSamples[s]++;
                if (tons > g_vehPeakTons[s]) g_vehPeakTons[s] = tons;
            }

            // Retained raw words and legacy guess; disproven as road-truck
            // activity evidence in the supplied game test. Never gate on them.
            int state = -1, mode = -1;
            if (ReadablePtr((const BYTE*)v + 0x448, 4))
                state = *(const int*)((const BYTE*)v + 0x448);
            if (ReadablePtr((const BYTE*)v + 0x440, 4))
                mode  = *(const int*)((const BYTE*)v + 0x440);
            const bool active = (state == 1 || state == 2);
            if (active) g_activeVehDaysRun++; else g_idleVehDaysRun++;

            // Metadata capacity candidate used by the panel, NOT proof of a
            // native refill clamp or physical tonnes.
            const double cap = TankCapacity(v);
            if (cap > 0.0 && s >= 0 && !g_vehCapValid[s]) {
                g_vehCapValid[s] = 1;
                g_vehCap[s] = cap;
                Logf("[VEH] day %d v=%p capacity8684_raw=%.9g display_scale1000_estimate=%.2f "
                     "physical_units=unverified refill_clamp=unverified",
                     closedDay, v, cap, cap * 1000.0);
            }

            Logf("[VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) legacy_kind50c=%d catalogue510_candidate=%p peak %.3ft "
                 "legacy_state448=%d legacy_mode440=%d %s native_activity=unverified",
                 closedDay, v, (home == b) ? "this" : "other",
                 tons, slots, kind, cat, (s >= 0) ? g_vehPeakTons[s] : 0.0,
                 state, mode, active ? "legacy_active_guess" : "legacy_idle_guess");

            // One dump per vehicle, ever. +0x510 is the lead for capacity,
            // speed, price and consumption; this tells us what it actually is
            // before anything is built on it.
            // v0.10: fuel on board, and what left the tank since yesterday.
            double burned = 0.0;
            unsigned refills = 0;
            ReadTank(s, v, closedDay, &burned, &refills);
            g_burnTonsRun += burned;
            g_refillsRun  += refills;
            if (g_vehFuelValid[s]) g_tankVehDaysRun++;
            if (burned > 0.0 || refills > 0)
                Logf("[TANK SAMPLE] day %d v=%p candidate_raw=%.4f | decrease_raw=%.4f "
                     "| sample_increases=%u units=unverified exact_burn=0 verified_refill=0",
                     closedDay, v, g_vehFuelValid[s] ? g_vehFuel[s] : 0.0,
                     burned, refills);

            ScanVehicle(s, v, closedDay);

            if (first && cat && TsmIsValidGamePtr(cat) && ReadablePtr(cat, 32)) {
                const unsigned* d = (const unsigned*)cat;
                Logf("[VEH] day %d   cat[+0x00] %08X %08X %08X %08X | %08X %08X %08X %08X",
                     closedDay, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
            }
        }
    }

    // v0.11: this close saw at least one tank -> it counts as a fleet-day.
    if (g_tankVehDaysRun > vehDaysBefore) g_tankDaysRun++;
}

// ---------------------------------------------------------------------------
// entry points
// ---------------------------------------------------------------------------

#include "panel_fuel_runtime.h"

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_logistics";
    info->version = PL_VERSION;

    ReadSettings();
    if (!g_enabled) {
        Logf("private_logistics  disabled in the ini");
        return 1;
    }

    g_realTransfer = (tTransfer)(g_exeBase + TSM_RVA_TRANSFER);

    Logf("private_logistics  v%s - billing %s",
         PL_VERSION,
         g_dryRun ? "ARMED IN DRY RUN (computes every payment, moves nothing)"
                  : "LIVE (moves money)");
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_exeBase || g_exeSize == 0) {
        Logf("private_logistics  no executable base - refusing to start");
        return 1;
    }

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK,
                                                   TSM_SIM_CLOCK_VERSION);
    if (!g_simClock)
        Logf("private_logistics  WARN  sim_clock is not installed - "
             "the daily summary will not be printed");

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE,
                                                           TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        Logf("private_logistics  WARN  enterprise_base is not installed - "
             "no building can be identified as private and nothing will be paid");

    if (g_autoTargetsEnabled) {
        g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                       TSM_UI_PANELS_VERSION);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V5);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V4);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V3);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V2);
        if (g_uiPanels && TsmUiPanelsHasWidgets(g_uiPanels) &&
            g_uiPanels->registerWidgetProvider)
            g_stateStorageWidgetHandle =
                g_uiPanels->registerWidgetProvider(StateStorageWidgetProvider, nullptr);
        if (!g_stateStorageWidgetHandle)
            Logf("private_logistics  [AUTO DO] WARN state-storage checkbox unavailable; "
                 "ui_panels v2 widgets are required");
    }

    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                   TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                       TSM_CURRENCY_VERSION_V3);
    if (!g_currency)
        Logf("private_logistics  WARN  currency is not installed - "
             "the treasury leg of every payment is unavailable");

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                         TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                             TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing)
        Logf("private_logistics  WARN  resource_pricing is not installed - "
             "factory payments fall back to the raw Res+0x58 field, which read "
             "0.00 for fabric and plants in two sessions");

    if (g_billing && (!g_enterprise || !g_currency)) {
        g_billing = 0;
        Logf("private_logistics  billing DISABLED: it needs both enterprise_base "
             "and currency. Instrumentation continues normally.");
    }

    if (g_autoTargetsEnabled &&
        (!g_simClock || !g_enterprise || !g_enterprise->getEnterpriseCount ||
         !g_enterprise->getEnterpriseByIndex || !g_enterprise->isBuildingPrivate)) {
        g_autoTargetsEnabled = 0;
        Logf("private_logistics  [AUTO DO] DISABLED: requires sim_clock and "
             "enterprise_base discovery/private checks");
    }

    if (g_autoTargetsEnabled) {
        g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE,
                                                              TSM_PERSISTENCE_VERSION);
        if (g_persistence &&
            g_persistence->structSize >= offsetof(TsmPersistenceApi, registerHandler) +
                                         sizeof(g_persistence->registerHandler) &&
            g_persistence->registerHandler)
            g_persistenceHandle = g_persistence->registerHandler("private_logistics",
                                                                  AutoPersistenceEvent, nullptr);
        if (!g_persistenceHandle)
            Logf("private_logistics  [AUTO DO] WARN persistence load signal unavailable; "
                 "reconciliation after load may wait for date rewind or restart");
    }

    PanelObserverStart();

    PatchAllSites();

    if (g_simClock) g_simClock->registerDay(OnDay, nullptr);

    Logf("private_logistics  live v%s: billing=%d dry_run=%d factory=%d carrier=%d, "
         "%d sites, cap %.0f/day, price = %s, factory share %.2f",
         PL_VERSION, g_billing, g_dryRun, g_payFactory, g_payCarrier,
         g_siteCount, (double)g_dailyPayCap,
         g_tripTariff ? "LEGACY FRAGMENT MODEL (fuel x margin + wear), NOT journey settlement"
                      : "value share (v0.6 path, for comparison)",
         (double)g_factoryShare);

    Logf("private_logistics  [AUTO DO] enabled=%d dry_run=%d cadence=once/day; "
         "private endpoints -> private road/rail DOs; existing entries preserved",
         g_autoTargetsEnabled, g_autoTargetsDryRun);

    if (g_freightShareRetired > 0.0f)
        Logf("private_logistics  WARN freight_share = %.3f is RETIRED in v0.7 and was "
             "ignored - the legacy model prices transfer fragments, NOT journeys. Remove the key.",
             (double)g_freightShareRetired);

    // v0.5: name both indices before a single delivery is priced. A probe that
    // silently cannot resolve a price is worse than no probe, so the log says
    // which index is live and what it reads.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);

        Logf("private_logistics  v%s legacy fragment model: %.2f configured L/fragment + %.2f L/t, wear %.2f L, "
             "margin %.0f%%, density %.5f t/L, index '%s', auto-tune %s",
             PL_VERSION, gLitresPerTrip, gLitresPerTonne, gWearLitres,
             gTariffMargin * 100.0, gDieselDensity, gFuelResource,
             g_litresAuto ? "requested_but_blocked_unverified_samples" : "off");

        // The world is not loaded yet at plugin start, so both of these read
        // zero here and are re-read at every day close. Say that rather than
        // printing a scary NOT FOUND that resolves itself a second later.
        Logf("private_logistics  [IDX] at plugin start: fuel '%s' = %.2f, cpi basket %.2f - "
             "the world is still loading; both are re-read at every day close",
             gFuelResource, fp, (double)basket);

        Logf("private_logistics  legacy tariff is a MODEL per transfer fragment, NOT per journey; "
             "billing=%d dry_run=%d carrier_enabled=%d tariff_mode=%s. "
             "Only [PAY] reports payments; native journey settlement OFF.",
             g_billing, g_dryRun, g_payCarrier, g_tripTariff ? "model" : "value_share");
    }

    if (g_balanceLog) {
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
            have  = true;
        }
        if (have) {
            g_treasuryAtClose   = treas;
            g_treasuryHaveClose = false;   // first close prints a baseline
            Logf("private_logistics  [BAL] treasury opens at %.2f", treas);
        }
    }

    if (g_billing && g_dryRun)
        Logf("private_logistics  DRY RUN: every [PAY] line says 'would'. "
             "Set dry_run = 0 in the ini and restart to move the money.");
    else if (g_billing)
        Logf("private_logistics  LIVE MONEY: the state treasury pays. Every [PAY] "
             "line reports the amount, the payee and the shortfall gate.");

    Logf("private_logistics  send back every [PAY] line and the daily [PAY] totals");
    return 0;
}
````

#### `plugins/private_logistics/private_logistics.ini`

SHA-256: `14b19f35acbc6bc9e6efb22ab09e366d931cf57fd83c240f29c90b3dd3f8dcb9`

````ini
; ---------------------------------------------------------------------------
; private_logistics.ini - v0.18.7 diagnostic provenance / paused capture gaps
;
; Billing OFF. Dispatch and independent state permissions are unchanged.
; Native loaded-journey settlement remains OFF; its adapter is not yet wired.
; Existing tariff knobs describe a LEGACY MODEL PER TRANSFER FRAGMENT, not a
; journey or measured fuel cost. [TARIFF MODEL] totals do not report payments.
; Only [PAY] reports legacy payment outcomes if billing is explicitly enabled.
; Model: work = litres_per_trip + litres_per_tonne x fragment tonnes;
; fuel = work x diesel_density x fuel_price; wear = wear_litres x density x price;
; tariff = fuel x (1 + tariff_margin) + wear. No parameter is verified physics.
; Purchases, sampled decreases and increases cannot prove burn/refill events.
; See docs/private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md.
; ---------------------------------------------------------------------------

[private_logistics]
enabled             = 1

; PERMISSIONS TEST DEFAULT: no legacy freight payments. Target management
; remains active below. 1 explicitly opts into the old live billing algorithm.
billing             = 0
; 1 = count every payment and move nothing.
dry_run             = 1

; NEW NEXT-TEST DEFAULT: inspect the actual opened detail-panel object and
; candidate-home road-DO fleet membership; read-only, NOT exact burn or billing.
; Requires matching executable bytes, sim_clock and persistence load signal.
; One snapshot/second, 240 accepted records/load epoch, 16 queued.
; Paused captures keep the latest queued snapshot of the same window/root.
; [PANEL GAP] explicitly reports omitted paused snapshots, queue overflow,
; lock contention and capture stop/fault. Accepted sample IDs are NOT coverage.
; Capture-time and drain-time queue-full counters are separate.
panel_fuel_observer = 1

; H1: the private building the goods LEAVE is paid = value x factory_share.
; This leg only fires on a privately owned producer. If it reads 0.00 every
; day, no building in the chain is private - check for a [PRIV CLICK] line.
pay_factory         = 1
factory_share       = 0.90

; Legacy carrier model: one price per eligible transfer fragment, NOT journey.
pay_carrier         = 1

; 1 = use the legacy fragment tariff model above when billing is enabled.
;       0 = charge 8 % of cargo value instead - the v0.6 path, kept only so
;           the two can be compared inside one session.
trip_tariff         = 1

; ---------------------------------------------------------------------------
; the tariff, in physical units
; ---------------------------------------------------------------------------

; Legacy model estimate retained unchanged for compatibility. It was derived
; from sampled fleet endpoints, NOT verified clamped native deductions.
; Do not calibrate it from [TANK SAMPLE], purchases, or a screenshot difference.
litres_per_trip     = 14.75

; Legacy model default only. Native cargo-load dependence is unverified.
litres_per_tonne    = 0.0

; v0.7: the vehicle sinking fund, denominated in litres of diesel so it
; inflates WITH fuel instead of behind it. This is POLICY, not physics: it is
; what you decide a trip should put aside towards the next truck.
;
; Set it from a truck, not from taste:
;     wear_litres = truck_price / trips_per_life / (density x fuel_price)
; A 6,000 RUB truck over 4,000 trips is 1.50 RUB/trip, about 13 L at 133/RUB.
; 5.00 is deliberately conservative - raise it and the DO saves for vehicles.
wear_litres         = 5.00

; Auto-tune is blocked even if requested: candidate sample changes/units are
; unverified and cannot calibrate physical fuel cost. Default remains OFF.
litres_auto         = 0

diesel_density      = 0.00100   ; legacy model t/L; NOT proof of native mass units.

; The carrier's profit, as a fraction of fuel cost. Applies to fuel only;
; the wear reserve is a cost and is passed through at cost.
tariff_margin       = 0.35

; The index. 'fuel' is resolved by NAME through resource_pricing every day,
; so the tariff follows the game's own diesel price. If it cannot resolve,
; the same work is priced in daily survival baskets instead, and if neither
; resolves the delivery is reported UNINDEXED and priced at nothing -
; this build never invents a figure in money.
fuel_resource       = fuel
baskets_per_trip    = 0.80
baskets_per_tonne   = 0.02

; Per-delivery [TARIFF] lines; the day total is always printed.
tariff_log_per_day  = 60

; v0.8: the vehicle census. Once a day every private enterprise's fleet is
; walked (bld+0xC70/+0xC78, proven) and each vehicle is logged with what it
; carries now and at its peak. The first sighting of a vehicle also dumps the
; 8 dwords at the pointer the game gives for its catalogue entry (+0x510) -
; that is where capacity, speed, price and consumption live.
;
; Nothing is priced from the census yet. One number cannot describe a fleet,
; and this build refuses to pretend otherwise; the census is how the next one
; stops needing to.
census_log          = 1

; v0.9: the field scanner. Each vehicle's first 0x800 bytes are copied on first
; sight; every day after, only the words that CHANGED are logged:
;
;   [VEH CHG] day 12 v=0000... +0x6D4 000001C8 -> 0000019A (f 456.00 -> 410.00)
;
; A fuel tank is a word that falls while the truck drives and jumps when it
; refuels. 456 above is the kind of value to look for - litres, not roubles.
; Known pointer ranges are skipped, and at most veh_scan_log_per_day words are
; printed per vehicle per day.
veh_scan            = 1
veh_scan_log_per_day = 40

; v0.12: raw storage-vector dump for up to three vehicles per close. BOTH
; vectors - the trailer vector at veh+0x3A0 and the cargo vector at veh+0x3B8 -
; every slot, with the resource pointer and the tonnage. If diesel lives in a
; slot, one slot will fall every day and jump at a refill, exactly like
; [TANK]. Set to 0 once that is settled.
cargo_dump          = 1

; Candidate diagnostics only. Each [TANK] record explicitly states selected
; source (slot_candidate / display_5f0_fallback / unavailable), validity, raw
; candidates, independent drift validity and sampled change classification.
; Fallback is NEVER compared to itself. Invalid/negative values are not zero;
; source changes, invalid reads and load epochs break sample continuity.
; No native clamp, actual burn/refill or physical mass unit is claimed.
tank_cross_log      = 40

; ---------------------------------------------------------------------------
; instrument
; ---------------------------------------------------------------------------

; [BAL] treasury movement, the carriers' capital and the fuel meter, per day.
; The meter lives here, so balance_log = 0 also disables the measured fuel.
balance_log         = 1
; one [B2B SITE] line per transfer call
site_log            = 1
; cap on [B2B]/[PAY]/[TARIFF] detail lines per day
log_cap_per_day     = 400
; shipment ring buffer
ledger_capacity     = 512

; Absolute payment cap per day, in the active currency. A runaway loop should
; show up as a cap line, not as an emptied treasury.
daily_pay_cap       = 5000.0
; below this, a payment is skipped and counted, never rounded up.
min_payment         = 0.01

; Sites to hook. Empty = all 16. "0x68F792,68F7F9" for a subset.
site_list           =

; Autonomous private DO connections; state site access requires explicit
; per-building direction permissions. State retail: deliveries only. State
; factory/storage: independent loading and deliveries. Unknown roles deny.
; Private endpoints follow their role without player access checkboxes.
; Missing eligible targets are re-added at the daily scan (not instantly).
auto_targets_enabled = 1
; 1 = report plans only (no TaskCfg writes/appends); 0 = apply permissions.
auto_targets_dry_run = 0
; state_storage_always_on is obsolete: any old value is ignored and logged.
; Maximum target-detail lines per daily scan; summary lines are always printed.
auto_targets_log_limit = 80

; ---------------------------------------------------------------------------
; NOT IMPLEMENTED - declared so the intent is on record, not to work yet
; ---------------------------------------------------------------------------
; 0 = the DO pays for diesel through the game's own foreign purchase, which
; is what happens today and what the v0.6 reconciliation measured.
charge_fuel_at_pump      = 0
charge_vehicle_purchase  = 1
upkeep_per_vehicle_day   = 0.0     ; a private DO pays no upkeep: vehicles + fuel

; who pays when the receiving building cannot be identified
unknown_payer            = state

; Company policy for automatic tasks, applied to both new and existing entries
; at daily reconciliation. Native per-DO/per-task edits are overwritten for
; managed targets. Manually assigned customs tasks remain untouched.
; Source 10% matches the working manual setting reported in the latest test.
; This is an empirical test policy, NOT a proven native ceiling or dispatch fix.
; Load when source stock is ABOVE this percentage (policy range 0..90).
auto_source_percent = 10
; Deliver when destination stock is BELOW this percentage (policy range 10..100).
; This is a dispatch trigger, NOT a spending limit or exact final-stock cap.
auto_destination_percent = 30
````

#### `tests/private_logistics/fuel_diagnostics_test.cpp`

SHA-256: `eec263bfdc42d1f580e6a3432bab7fcf6e12a8ed8056d1b2e0199bfa4b51a3bc`

````cpp
// Candidate provenance tests. Not proof of native offsets, units, or events.
#include "fuel_diagnostics.h"
#include <cassert>
#include <cstdio>
#include <limits>
#include <initializer_list>
using namespace private_logistics_fuel_diagnostics;
int main() {
    const auto fallback=Select(false,-1.0,0.09);
    assert(fallback.valid && fallback.source==Source::DisplayFallback && !fallback.comparable);
    std::puts("PASS display_fallback_not_self_comparable");
    auto slot=Select(true,0.08,0.09);
    assert(slot.valid && slot.source==Source::SlotCandidate && slot.comparable);
    assert(std::fabs(slot.drift-0.01)<1e-12);
    std::puts("PASS independent_candidate_comparison");
    for(double invalid : {-1.0, -0.03, 1001.0, std::numeric_limits<double>::infinity(),
                          std::numeric_limits<double>::quiet_NaN()}) {
        assert(!Select(false,-1.0,invalid).valid);
        assert(!Select(true,invalid,0.09).valid); // found-invalid slot cannot be hidden
    }
    std::puts("PASS invalid_negative_and_nonfinite_not_zero_or_hidden");
    slot=Select(true,0.0,-1.0);assert(slot.valid && !slot.comparable && slot.value==0.0);
    assert(Select(false,0.0,0.0).valid);
    std::puts("PASS readable_zero_is_valid_invalid_is_not_zero");
    Samples samples;
    assert(samples.Observe(fallback,1).change==Change::Baseline);
    auto r=samples.Observe(Select(false,-1.0,0.07),1);
    assert(r.change==Change::Decrease && std::fabs(r.decrease-0.02)<1e-12);
    assert(samples.Observe(Select(false,-1.0,0.08),1).change==Change::Increase);
    assert(samples.Observe(Select(false,-1.0,0.08),1).change==Change::Flat);
    std::puts("PASS changes_are_sample_decreases_and_increases_only");
    r=samples.Observe(Select(true,0.01,0.08),1);
    assert(r.change==Change::SourceChange && r.decrease==0.0);
    r=samples.Observe(Select(false,-1.0,0.005),1);
    assert(r.change==Change::SourceChange && r.decrease==0.0);
    std::puts("PASS source_changes_reset_baseline_in_both_directions");
    assert(samples.Observe(Select(false,-1.0,-0.03),1).change==Change::Invalid);
    r=samples.Observe(Select(false,-1.0,0.001),1);
    assert(r.change==Change::Baseline && r.decrease==0.0);
    std::puts("PASS invalid_read_breaks_sample_continuity");
    r=samples.Observe(Select(false,-1.0,0.0),2);
    assert(r.change==Change::Baseline && r.decrease==0.0);
    std::puts("PASS load_epoch_breaks_sample_continuity");
    std::puts("8 candidate-provenance scenarios PASS (not native fuel validation)");
}
````

#### `tests/private_logistics/test_fuel_diagnostics.py`

SHA-256: `afd1baf9c7dc9dc080c5a206e574f23c41e0bb7874c4d9977021003b36f23e57`

````python
"""Exercise production provenance logic and extracted ReadTank/FuelSlotTons.
These are host tests, never proof of native classification, offsets or hooks.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'


class FuelDiagnosticsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.cpp = (PLUGIN / 'private_logistics.cpp').read_text()

    def compile_and_run(self, path):
        exe = Path(self.directory.name) / 'test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(path),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_candidate_provenance_core(self):
        output = self.compile_and_run(Path(__file__).with_name('fuel_diagnostics_test.cpp'))
        self.assertIn('8 candidate-provenance scenarios PASS', output)

    def test_actual_readtank_and_slot_reader_with_host_memory(self):
        # Extract the actual unchanged production functions, not a test rewrite.
        slot = self.cpp.split('static double FuelSlotTons(void* v, int* found)', 1)[1]
        slot = 'static double FuelSlotTons(void* v, int* found)' + slot.split(
            '// The panel backing quantity', 1)[0]
        read = self.cpp.split('static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)', 1)[1]
        read = 'static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)' + read.split(
            'static void ScanVehicle', 1)[0]
        source = r'''
#include "fuel_diagnostics.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdarg>
using BYTE=unsigned char; using LONG=long;
constexpr int VEH_MAX=1;
static private_logistics_fuel_diagnostics::Samples g_vehFuelSamples[VEH_MAX];
static volatile LONG g_fuelDiagnosticEpoch=1;
static bool g_vehFuelValid[VEH_MAX]{};
static double g_vehFuel[VEH_MAX]{};
static int g_tankCrossLog=100;
static double displayRaw=0.09;
static std::string lastLog;
static int resource;
static bool ReadablePtr(const void* p, size_t) {return p!=nullptr;}
static void* FuelResourcePtr() {return &resource;}
static LONG InterlockedCompareExchange(volatile LONG* p,LONG,LONG) {return *p;}
static double MirrorTons(void*) {return displayRaw;}
static void Logf(const char*,...) __attribute__((format(printf,1,2)));
static void Logf(const char* fmt,...) {
    char line[2048]; va_list args; va_start(args,fmt);
    std::vsnprintf(line,sizeof(line),fmt,args);va_end(args);lastLog=line;
}
''' + slot + read + r'''
int main() {
    alignas(16) BYTE vehicle[0x600]{}, entry[16]{};
    BYTE* begin=entry; BYTE* end=entry+16;
    std::memcpy(vehicle+0x3B8,&begin,sizeof(begin));
    std::memcpy(vehicle+0x3C0,&end,sizeof(end));
    double decrease=0.0;unsigned increases=0;
    ReadTank(0,vehicle,1,&decrease,&increases);
    assert(g_vehFuelValid[0] && decrease==0.0);
    assert(lastLog.find("selected_source=display_5f0_fallback")!=std::string::npos);
    assert(lastLog.find("drift_valid=0")!=std::string::npos);
    displayRaw=0.07;ReadTank(0,vehicle,2,&decrease,&increases);
    assert(std::fabs(decrease-0.02)<1e-12 && increases==0);
    void* fuel=&resource;std::memcpy(entry,&fuel,sizeof(fuel));
    float value=0.01f;std::memcpy(entry+8,&value,4);
    decrease=0.0;ReadTank(0,vehicle,3,&decrease,&increases);
    assert(decrease==0.0 && lastLog.find("source_change_baseline")!=std::string::npos);
    value=-0.03f;std::memcpy(entry+8,&value,4);
    int found=0;assert(FuelSlotTons(vehicle,&found)<0.0 && found==1);
    ReadTank(0,vehicle,4,&decrease,&increases);
    assert(!g_vehFuelValid[0] && decrease==0.0);
    assert(lastLog.find("slot_found=1")!=std::string::npos);
    assert(lastLog.find("source_valid=0")!=std::string::npos);
    value=0.005f;std::memcpy(entry+8,&value,4);
    ReadTank(0,vehicle,5,&decrease,&increases);assert(decrease==0.0);
    value=0.004f;std::memcpy(entry+8,&value,4);++g_fuelDiagnosticEpoch;
    ReadTank(0,vehicle,6,&decrease,&increases);assert(decrease==0.0);
    value=0.01f;std::memcpy(entry+8,&value,4);
    ReadTank(0,vehicle,7,&decrease,&increases);assert(increases==1);
    assert(lastLog.find("verified_refill=0 billing_input=0")!=std::string::npos);
    std::puts("PASS extracted_production_readtank_and_slot_reader");
}
'''
        path = Path(self.directory.name) / 'readtank.cpp'
        path.write_text(source)
        self.assertIn('PASS extracted_production_readtank', self.compile_and_run(path))

    def test_tariff_logs_are_model_fragments_not_charged_totals(self):
        self.assertNotIn('the tariff IS charged', self.cpp)
        self.assertNotIn('g_tripTariff ? "charged" : "model"', self.cpp)
        self.assertIn('priced_transfer_fragment(s)', self.cpp)
        self.assertIn('NOT journeys or payment totals; billing=%d dry_run=%d', self.cpp)
        self.assertIn('payment_returned=%.2f billing=%d dry_run=%d', self.cpp)
        self.assertIn('native journey settlement OFF', self.cpp)

    def test_no_sampled_burn_calibration_or_tracker_feed(self):
        self.assertNotIn('SET litres_per_trip TO THE MEASURED FIGURE', self.cpp)
        self.assertNotIn('gLitresPerTrip = before *', self.cpp)
        self.assertIn('auto-tune NOT applied', self.cpp)
        self.assertNotIn('private_logistics_journey::Tracker', self.cpp)
        self.assertNotIn('has NO fuel slot', self.cpp)
        read = self.cpp.split('static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)', 1)[1].split('static void ScanVehicle', 1)[0]
        self.assertNotIn('"agree"', read)
        self.assertIn('InterlockedCompareExchange(&g_fuelDiagnosticEpoch', read)


if __name__ == '__main__':
    unittest.main()
````

#### `tests/private_logistics/panel_fuel_runtime_test.cpp`

SHA-256: `23c948abab4fadee9f10eff2f50f7f83d33a052a343ce984be7a300b6fffd995`

````cpp
// Native host shim exercises the production queue/control code. NOT Windows ABI,
// SEH, patch execution, real threads, or native game lifecycle validation.
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include <cstdarg>

using BYTE = unsigned char;
using LONG = long; // host-shim log ABI; real Windows LONG is 32-bit
using ULONGLONG = unsigned long long;
struct CRITICAL_SECTION { bool busy=false; };
static bool TryEnterCriticalSection(CRITICAL_SECTION* p) { if(p->busy)return false;p->busy=true;return true; }
static void LeaveCriticalSection(CRITICAL_SECTION* p) { CHECK(p->busy);p->busy=false; }
static void InitializeCriticalSection(CRITICAL_SECTION* p) { p->busy=false; }
static LONG InterlockedCompareExchange(volatile LONG* p,LONG value,LONG expected) { LONG old=*p;if(old==expected)*p=value;return old; }
static LONG InterlockedExchange(volatile LONG* p,LONG value) { LONG old=*p;*p=value;return old; }
static LONG InterlockedIncrement(volatile LONG* p) { return ++*p; }
static ULONGLONG clockMs=1000;
static ULONGLONG GetTickCount64() { return clockMs; }
static std::vector<BYTE> image(0x90B080);
static BYTE* g_exeBase=image.data();
static std::size_t g_exeSize=image.size();
static bool throwOnRead=false;
static bool ReadablePtr(const void* p,std::size_t n) {
    if(throwOnRead) throw 1;
    const auto a=(uintptr_t)p,b=(uintptr_t)g_exeBase;
    return a>=b && a-b<=image.size() && n<=image.size()-(a-b);
}
constexpr unsigned IMAGE_DOS_SIGNATURE=0x5A4D,IMAGE_NT_SIGNATURE=0x4550;
constexpr unsigned IMAGE_FILE_MACHINE_AMD64=0x8664,IMAGE_NT_OPTIONAL_HDR64_MAGIC=0x20B;
struct IMAGE_DOS_HEADER { unsigned e_magic; int e_lfanew; };
struct IMAGE_NT_HEADERS64 {
    unsigned Signature;
    struct { unsigned Machine, TimeDateStamp; } FileHeader;
    struct { unsigned Magic; } OptionalHeader;
};
static std::vector<std::string> logs;
#if defined(__GNUC__) || defined(__clang__)
static void Logf(const char*,...) __attribute__((format(printf,1,2)));
#endif
static void Logf(const char* format,...) {
    char line[2048];va_list args;va_start(args,format);std::vsnprintf(line,sizeof(line),format,args);va_end(args);logs.emplace_back(line);
}
static int g_panelObserverEnabled=1,g_billing=0,g_persistenceHandle=1;
static unsigned installed=0,originalCalls=0;
static void Original(void*,void*) { ++originalCalls; }
static bool InstallInlineHook(void*,void*,void** trampoline,const BYTE*,std::size_t n,const char*) {
    CHECK(n==20);*trampoline=(void*)Original;++installed;return true;
}
using Tick=void(*)(void*,float,float);
static Tick tick=nullptr;
static int RegisterTick(Tick fn,void*) { tick=fn;return 1; }
static bool paused=false;
static int IsPaused() { return paused ? 1 : 0; }
struct ClockApi { int(*registerTick)(Tick,void*); int(*isPaused)(); };
static ClockApi api{RegisterTick,IsPaused};
static ClockApi* g_simClock=&api;
#define __fastcall
#define __try try
#define __except(x) catch(...)
#include "panel_fuel_runtime.h"

int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    PanelObserverStart();CHECK(installed==1 && tick && g_panelStarted);
    std::puts("PASS runtime_install_fingerprint");
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    const auto logged=logs.size();PanelRendererDetour(context,window);
    CHECK(originalCalls==1 && g_panelCount==1 && logs.size()==logged);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    tick(nullptr,1,1);CHECK(g_panelCount==0 && logs.size()==logged+2);
    CHECK(logs[logged].find("litres_candidate=549.54")!=std::string::npos);
    CHECK(logs[logged+1].find("exact_fleet_pointer")!=std::string::npos);
    std::puts("PASS runtime_post_render_throttle_and_deferred_log");
    paused=true;
    const auto pausedLogs=logs.size();
    for(unsigned i=0;i<72;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==1 && g_panelDropped==0 && g_panelSamples==2);
    CHECK(g_panelQueue[g_panelHead].coalesced==71 && logs.size()==pausedLogs);
    paused=false;clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2);tick(nullptr,1,1);
    CHECK(logs[pausedLogs].find("reason=paused_snapshot_coalescing")!=std::string::npos);
    CHECK(logs[pausedLogs].find("discarded_paused_snapshots=71")!=std::string::npos);
    CHECK(logs[pausedLogs+1].find("captured_paused=1")!=std::string::npos);
    tick(nullptr,1,1);CHECK(g_panelCount==0);
    std::puts("PASS runtime_long_pause_coalesces_and_reports_omissions");
    PanelObserverLoading();tick(nullptr,1,1);paused=true;
    clockMs+=1000;PanelCapture(context,window);
    put(fixture.window+0x240,ptr(0x90000)); // another root, no assumed native metadata
    clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2 && g_panelQueue[g_panelHead].snapshot.root==ptr(fixture.root));
    CHECK(g_panelQueue[(g_panelHead+1)%16].snapshot.root==ptr(0x90000));
    std::memcpy(g_exeBase+0xB0000,g_exeBase+fixture.window,0x500);
    clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==3);
    paused=false;clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==4);
    put(fixture.window+0x240,ptr(fixture.root));
    PanelObserverLoading();tick(nullptr,1,1);
    std::puts("PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record");
    for(unsigned i=0;i<17;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==16 && g_panelDropped==1);
    const auto fullLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[fullLogs].find("reason=queue_full")!=std::string::npos);
    CHECK(logs[fullLogs].find("skipped_snapshot_captures=1")!=std::string::npos);
    CHECK(logs[fullLogs+1].find("queue_full_total_at_capture=0 queue_full_total_at_drain=1")!=std::string::npos);
    const auto afterGap=logs.size();tick(nullptr,1,1);
    CHECK(logs[afterGap].find("[PANEL FUEL]")!=std::string::npos); // no duplicate gap
    g_panelQueueLock.busy=true;clockMs+=1000;PanelCapture(context,window);
    g_panelQueueLock.busy=false;const auto contentionLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[contentionLogs].find("reason=render_trylock_contention")!=std::string::npos);
    PanelObserverLoading();const auto oldLogs=logs.size();tick(nullptr,1,1);
    CHECK(g_panelCount==0 && g_panelSamples==0 && logs.size()==oldLogs);
    std::puts("PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard");
    clockMs+=1000;PanelCapture(context,nullptr);CHECK(g_panelLastMs==0);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    g_panelSamples=239;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    const auto capLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[capLogs].find("reason=capture_cap cap=240 capture=stopped")!=std::string::npos);
    tick(nullptr,1,1);const auto drainedLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs.size()==drainedLogs);
    std::puts("PASS runtime_inactive_does_not_starve_capture_and_cap");
    PanelObserverLoading();tick(nullptr,1,1);clockMs+=1000;
    throwOnRead=true;PanelCapture(context,window);throwOnRead=false;
    CHECK(g_panelFault && g_panelCount==0);
    const auto faultLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[faultLogs].find("reason=guarded_read_fault observer=disabled")!=std::string::npos);
    const auto disabledLogs=logs.size();clockMs+=1000;PanelCapture(context,window);tick(nullptr,1,1);
    CHECK(g_panelCount==0 && logs.size()==disabledLogs);
    std::puts("PASS runtime_guarded_fault_disables_and_reports_once");
    g_exeBase[0x90B07C]=1;CHECK(!PanelTargetMatches());g_exeBase[0x90B07C]=0;
    g_exeBase[0x8F3E98]=0;CHECK(!PanelTargetMatches());
    std::puts("PASS runtime_changed_data_refused");
    std::puts("8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}
````

#### `tests/private_logistics/test_panel_fuel_observer.py`

SHA-256: `596acc2369761b1981275677f5e298be02abfae3f0dc2e567b5c7b2c0dd0534d`

````python
"""Production reader with synthetic memory; source guards are NOT a DLL/game test."""
from pathlib import Path
import configparser
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = ROOT / 'tests/private_logistics/panel_fuel_observer_test.cpp'


class PanelFuelObserverTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'panel_test'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, case):
        result = subprocess.run([str(self.exe), case], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + case, result.stdout)

    def test_production_runtime_with_host_shim(self):
        exe = Path(self.directory.name) / 'runtime_test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(HARNESS.with_name('panel_fuel_runtime_test.cpp')),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('8 host-shim scenarios PASS', result.stdout)

    def test_matching_test_defaults_and_no_tracker_wiring(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        self.assertEqual(config.getint('private_logistics', 'panel_fuel_observer'), 1)
        self.assertIn('H->configInt(ini, "private_logistics", "panel_fuel_observer", 1)', cpp)
        self.assertEqual(config.getint('private_logistics', 'billing'), 0)
        self.assertEqual(config.getint('private_logistics', 'dry_run'), 1)
        self.assertEqual(config.getint('private_logistics', 'auto_source_percent'), 10)
        self.assertEqual(config.getint('private_logistics', 'auto_destination_percent'), 30)
        self.assertNotIn('private_logistics_journey::Tracker', cpp)
        self.assertIn('PanelObserverStart();', cpp)
        self.assertRegex(cpp, r'if \(event == TSM_SAVE_LOADING\) \{\s*'
                         r'InterlockedIncrement\(&g_fuelDiagnosticEpoch\);\s*PanelObserverLoading\(\);')

    def test_hook_fingerprint_and_full_instruction_boundary(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        expect = runtime.split('static const BYTE expect[] = {', 1)[1].split('};', 1)[0]
        actual = bytes(int(x, 16) for x in re.findall(r'0x([0-9A-Fa-f]{2})', expect))
        # Pinned first 20 bytes from d00eaae, ending exactly after LEA RBP.
        self.assertEqual(actual, bytes.fromhex('40 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 50 D1 FF FF'))
        self.assertIn('TimeDateStamp != 0x6A3EB6ADu', runtime)
        self.assertIn('0x00, 0x00, 0x7A, 0x44', runtime)
        self.assertIn('memcmp(g_exeBase + 0x8F3E98', runtime)
        self.assertIn('&g_panelTrampoline, expect, sizeof(expect)', runtime)
        self.assertIn('!g_persistenceHandle', runtime)
        self.assertIn('if (g_billing ||', runtime)

    def test_render_path_is_bounded_nonblocking_no_io(self):
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        render = runtime.split('static void PanelCapture(', 1)[1].split('static const char* PanelStatus(', 1)[0]
        for forbidden in ('Logf(', 'EnterCriticalSection(', 'Sleep(', 'malloc(', 'new ', 'std::vector', 'std::string'):
            self.assertNotIn(forbidden, render.replace('TryEnterCriticalSection(', 'TryLock('))
        for required in ('TryEnterCriticalSection(', 'g_panelSamples >= 240', 'now - g_panelLastMs < 1000',
                         'g_panelCount == 16', '__except', 'epoch == InterlockedCompareExchange'):
            self.assertIn(required, render)
        hook = render.split('static void __fastcall PanelRendererDetour(', 1)[1]
        self.assertLess(hook.index('((PanelRenderer)g_panelTrampoline)'), hook.index('PanelCapture(context, window)'))

    def test_reader_has_no_native_calls_or_settlements(self):
        header = (PLUGIN / 'panel_fuel_observer.h').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        for forbidden in ('windows.h', 'currency_api', 'modifyCompanyCapital', 'FuelSlotTons(',
                          'FormatSummary(', 'Consume(', 'WriteProcessMemory'):
            self.assertNotIn(forbidden, header + runtime)
        self.assertIn('physical_fuel=unverified exact_burn=0 billing=off', runtime)
        self.assertIn('unresolved_7c58', runtime)

    def test_windows_min_max_macros(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstddef>\n#include <cstdint>\n#include <cmath>\n'
                        '#define max(a,b) ((a)>(b)?(a):(b))\n#define min(a,b) ((a)<(b)?(a):(b))\n'
                        '#include "panel_fuel_observer.h"\nint main(){return 0;}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)


for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name != 'name':
        def run(self, case=name):
            self.run_case(case)
        setattr(PanelFuelObserverTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()
````


---

## 2026-10-30 second real-game review — v0.18.7 and assigned-job proposal

### Scope and user direction

The user supplied six new screenshots (`image-7.png`–`image-12.png`) and `tesmioloader-3.log`, reported the same single vehicle making two deliveries and ultimately returning to its DO, and clarified that they **did not manually save on exit**. The proposed future optimal arrangement is to use the vehicle's actual assigned task/job lifetime as the trip boundary, record fuel at its start/end, price fuel in game units and have the buyer pay. **For now preserve the simplest working DO.** This is a review/design update, not permission to guess new task offsets or enable the old billing path. No runtime code, INI values, dispatch, settlement or tracker wiring was changed in this turn. Work is on the active `arena/8e9a4f62-wrsre`, based on merged PR31; earlier branch references are historical.

### Actual capture and independent assertions

Uploaded log: **652407 bytes, 4942 lines**, SHA-256 `e04e4c3d1a616ab29c4c18c71c3b7d8a8ee192cc1739440fcc590332a2304d6e`.

- Startup line 135 advertises the new paused coalescing policy; line 153 reports **v0.18.7-diagnostic-provenance, billing=0, dry_run=1**.
- **155 panel + 155 object records**, epoch 2, accepted samples 1–155. Every pair passes matching IDs, valid aggregation, exact fleet-pointer membership, index 0, one occurrence, no links and float32 `raw × 1000` formatting assertions.
- Same root throughout: `000002D3174E8030`; home DO `000002D3174CE9C0`; metadata `000002D2C73234D0`. The first panel window address differs from the remaining 154, but the underlying truck stays the same. Do not use a UI window address as a durable vehicle/job identity.
- **153 probe blocks = world reset + selection + 151 measurements**. Parse exactly **11 lines per block**, including the header, to avoid importing the following panel's mode/kind into the probe. All 151 measurement records have readable cargo/display candidate, unavailable fuel slot, `state=0`, `mode=0`, `kind=-1`; these legacy fields are still not native road-truck activity gates.
- This is a supplied real Windows run of the new diagnostics, not a locally performed MSVC build, independently executed game session or validation of native fuel hooks.

### All six screenshot values matched

| Screenshot | Visible display | Matching accepted sample(s) | First matching log line | First matching drain/log time | Interpretation from image, not a native job event |
|---|---:|---:|---:|---|---|
| image-7 | 91.91 l | 1 | 800 | 17:48:26.765 | Empty; warehouse is highlighted in assigned schedule. User identifies the starting DO phase. |
| image-8 | 84.83 l | 23 | 1816 | 17:48:57.034 | Food 5.5 t; grocery-store unloading stop highlighted. |
| image-9 | 70.52 l | 57–65 | 2329 | 17:49:34.593 | Empty; grocery-store stop still highlighted. Matching fuel spans unloading, so the first fuel match is NOT an exact empty-cargo boundary. |
| image-10 | 56.75 l | 100–102 | 3640 | 17:53:34.200 | Electronics 0.11 t during loading; warehouse load-50% stop highlighted. Not the eventual full load. |
| image-11 | 41.57 l | 129–133 | 4216 | 17:54:05.148 | Empty; small-store unloading stop still highlighted. |
| image-12 | 29.26 l | 155 | 4868 | 17:55:26.408 | Empty, no schedule rows shown in supplied crop; user reports return to DO. No native task-cleared event is logged. |

Capture times are the records' `ms` fields, not the log timestamps: a paused snapshot can be printed much later. Images are visual phase evidence, not exact native assignment start/completion timestamps. The last image's absent rows cannot alone prove an underlying task pointer is null.

Rounded endpoint decrease: **91.91 − 29.26 = 62.65 candidate litres** across the whole observed run. Raw panel endpoint decrease: **62.6529455 candidate litres**. The 154 consecutive sample changes consist of **134 decreases, 20 unchanged, no observed increases**. No observed increase is NOT proof of no hidden refill or a complete deduction stream. These are display/candidate changes, not a new exact burn result or invoice.

Rounded adjacent image differences are **7.08, 14.31, 13.77, 15.18, 12.31 l**. In particular, **70.52 → 56.75** lies between first-delivery unloading and second-delivery loading. Without a logged intervening assignment boundary, that interval cannot be allocated confidently between the two jobs. Do not divide 62.65 by two and claim a measured cost for each trip; do not equate the two loaded-leg image differences with exact native loaded-journey burn.

### Cargo, fragments and logging suppression

The probe independently supports two cargo cycles on this truck:

- Food: empty at observation 17 (17:48:43.461), nonempty at observation 18 (17:48:44.463, 0.435142 t), reaches **5.500000 t** at observation 23 (17:48:50.528). Last sampled nonempty is observation 62 (17:49:39.892, 0.424414 t), first subsequent sampled empty is 63 (17:49:49.041).
- Electronics (`eletronics` in native resource logs): last preceding sampled empty is observation 98 (17:50:23.354), next sampled nonempty is 99 (17:53:34.705, 0.573037 t), reaches **1.962089 t** at observation 101 (17:53:36.621). Last sampled nonempty is 129 (17:54:07.275, 0.044399 t), first sampled empty is 130 (17:54:17.432). The screenshot's **0.11 t is partial loading**, not the final delivery size. The long gap before observation 99 includes a long pause; it is not a three-minute measured loading duration.

Day 14 close independently counts **161 committed food loading fragments per native side**, with 5.500 t source/truck totals. Day 15 close counts **143 food unloading fragments per side**, totaling 5.500 t, and **75 electronics loading fragments per side**, totaling 1.962 t. Printed electronics unloading details show **72 committed fragments per side**, with the independent cargo samples returning empty. These are **fragments, not journeys**. Three-decimal daily totals/six-decimal snapshots have different rounding precision and are not an exact conservation audit.

**Important telemetry limitation:** day 15 reports **4176 suppressed site lines and 36 suppressed attribution lines**, with 4140 no-op calls overall. Most are associated with repeated no-op attempts at the warehouse path; this is not evidence of thousands of trips or fuel charges. Only 4 electronics loading fragments per side appear in printed SITE details, while B2B attribution prints 57 per side and the daily tally counts 75 per side. The 18 additional loading fragments per side account for the 36 suppressed attribution lines. The analysis reports each separately and does not fabricate the missing event records. Do not attempt dense per-vehicle accounting from these capped transfer diagnostics. No cap/default/logging behavior was changed merely to collect a broader trace.

### Diagnostic cleanup: observed result

**Pause/queue:** six `[PANEL GAP]` records explicitly report **11 + 4 + 4 + 184 + 7 + 43 = 253** coalesced paused snapshots. Both queue-full counters are **zero in every accepted record**; no queue-full, contention, guarded-fault or capture-cap gap was logged. Accepted count 155 is below 240. Largest capture spacing is **190922 ms**, corresponding to the long coalesced paused section around sample 100. This demonstrates the new bounded pause behavior in this run, not complete simulation telemetry, thread safety under all races or exact burn coverage. Old contiguous sample IDs no longer hide a misleading global drain-time drop field.

**Tank provenance:** lines 2279/3982 explicitly say `selected_source=display_5f0_fallback source_valid=1 slot_found=0 drift_valid=0`, with `units=unverified exact_burn=0 verified_refill=0 billing_input=0`. Day 15's `decrease_raw=0.0214327276` is correctly labeled as a sample decrease; no fallback-versus-itself `agree` claim remains.

**Tariff:** day 15's **143 priced_transfer_fragment(s), model 478.70** is explicitly marked NOT journeys or payment totals, and both day-close PAY lines say **billing off**. BAL reports this plugin paid 0.00. Startup and detail records identify a legacy fragment model. The 478.70 figure is not an invoice, not two completed trips, and not a charge to a buyer. This run's day-close price lookup resolves `fuel` at **134.37** in the active RUB pricing path, but that is a day-close lookup, not a task-end price capture or proof of litres-to-tonnes conversion. `diesel_density=0.001` is still a legacy configured model parameter, not new native mass evidence.

**Dispatch:** day 14/15 each show one private road DO, three existing targets, `added=0 config_changed=0 suppressed=0 invalid=0`, stable office key `BF99DCF6FA074EA1`. The managed warehouse has loading enabled with threshold 0.100; shops have unloading enabled with threshold 0.300; disabled directions stay disabled. Vehicle schedule entries showing load 100%/50% describe a different per-order quantity/context than the DO's connection eligibility thresholds; they are not evidence that the 10/30 management settings reverted. The actual run supports continuing the working dispatch unchanged. It is not a new manual-removal/reconnection or second reload test.

### Future optimal arrangement: the assigned delivery job

**Accept the user's direction as the future design target:** use the same vehicle's actual assigned **delivery job** as the accounting envelope, instead of making each cargo callback a trip or restricting the future service envelope solely to first-goods/last-goods samples. Keep the working DO simple now. The existing disconnected C++ tracker is still written/tested for the older loaded-only policy; do not silently feed task-based events into it and claim equivalent accounting. A conscious policy/adapter/test update will be needed before implementing the new envelope.

The key distinction is **whole assignment versus current stop**. The screenshots show warehouse loading, buyer unloading and a DO `Nothing` stop within a displayed schedule; the highlighted `>>` row advances during the same service. Closing on every highlighted-stop change would split pickup, loaded travel and possibly return into separate artificial bills. The source/destination/resource/percentage schedule also changes between the food and electronics assignments. Screen text does not establish which native object/index is the whole job or its durable generation.

Suggested future contract (not implemented by this review):

1. **Start:** a verified new DO delivery assignment for that same vehicle is committed/activated. Capture vehicle/session generation, assignment identity/generation, intended buyer/destination and a verified fuel reading. This intentionally can include empty outbound travel before loading, unlike the old loaded-only rule.
2. **Continue:** preserve that assignment across its pickup/delivery stop changes, partial loading/unloading and same-job route updates. Keep cargo observations as completion/cancellation/allocation validation, not necessarily the primary boundary. Do not confuse a shared DO target/config row with a vehicle's active assignment; `DO+D38/D40` is not its fleet or a proven per-vehicle job identity.
3. **Finish once:** on verified successful completion or replacement after completion. A changed/cleared job may also mean cancellation, manual replacement, vehicle deletion or reload; it must not automatically generate a completed-delivery bill. Gaps/identity uncertainty invalidate costing, not dispatch.
4. **Empty return policy:** if the job is cleared at the buyer, its return-to-DO fuel is outside that assignment. If the desired bill includes empty return, retain an explicit return phase until DO arrival; do not assume the visible `Nothing` row necessarily belongs to a still-live job. The six screenshots do not settle the native lifetime or allocate the first job's return versus the second job's outbound fuel. This policy must be explicit before billing.
5. **Fuel:** start/end readings are a useful **estimate for a verified no-refuel/no-other-fuel-change interval** once field units/validity are confirmed. With verified refills and canonical physical units, conservation is `start quantity + fuel added − end quantity`, not just start minus end. Start minus end alone can undercount or even become negative after refueling; native clamping and field synchronization still matter. If refill/quantity provenance is unknown, report the estimate as uncertain/unbillable. The existing diagnostic stream is not sufficient evidence to make a no-refuel guarantee.
6. **Units and price:** convert using a verified game-native litres-to-tonnes mapping, not real-world diesel density or an unproved legacy constant. Multiply by the game's price **per tonne at settlement time**, not commodity cargo value, tank capacity, historical purchase totals or the last available day-close price.
7. **Payment:** charge the identified buyer for that completed service and credit the carrier once, with the existing debt policy if insufficient funds. A state-owned buyer and a private buyer require the correct payer path; vehicle home is not the buyer. Existing legacy `billing=1` pays from the treasury per fragment and is **not** this buyer-invoicing implementation. Leave it OFF. Multi-buyer jobs need explicit allocation before support; do not silently assign all fuel to the final buyer.

This task boundary proposal improves the eventual accounting envelope. It does not by itself supply the native assignment identity/completion signal, reveal hidden refills, prove tonnes, or turn endpoint gauge differences into exact burn. We can keep dispatch working independently while those bounded questions are addressed. No new task offsets, guessed hooks, buyer debits or broad panel/string searches were added or requested here.

### Exit/save clarification

The user explicitly exited without manually saving; accept that. The log nevertheless shows **two automatic save callbacks** (`autosave4` at 17:50:57 and `autosave5` at 17:53:38), and **38 `saved_last` write/open errors at exit**, 17:55:32.203–17:55:32.258 (lines 4904–4941), before shutdown at 17:55:37.845. These are logged attempts by the game, **not evidence that the user selected Save**. There is one separate startup `saved_last/header.bin` enumeration error, making 39 `saved_last` errors total, not 39 exit errors.

There are also earlier missing-content/resource warnings and two read errors for the loaded test save's `replace_history.bin` and `usedveh.bin` at 17:47:57. Causation and the persistence health of an actual reopened manual save remain unverified. Neither the automatic-save announcements nor this test establish successful full save/reload. Do not attribute the errors to the observer without evidence, and do not describe them as a manual save the user performed. Keep the previous backup caution; no automatic deletion, repair or save-policy change was made.

### Disposition / next step

**Pass for this run:** new diagnostic version loaded; six displayed values match the same truck; paused coalescing/counters and honest model/provenance labels work; two cargo cycles and continued working dispatch are supported; billing remained OFF. **Not established:** native assigned-job lifecycle, exact task start/end, actual native clamped deductions/refills, physical mass conversion, return-leg allocation, buyer payment or durable exactly-once settlement.

Keep the working DO unchanged and retain the future assigned-job proposal above. No repeat of this unchanged gauge/diagnostics test is needed. The missing simulation-writer artifact request from the previous checkpoint remains relevant for exact fuel accounting, but the user is not being asked to perform another broad investigation or enable legacy billing. This turn changes documentation only.

### Complete analysis source and actual output

The script is an ignored local review tool, fully embedded here so it is recoverable even when `.cache/` is not distributed. It accepts the supplied log path as an optional first argument and an image directory as an optional second argument; missing images skip hash checks but do not prevent log assertions. Input images are read only for hashes, not OCR. It checks 11-line probe boundaries, every float32 display match, object/panel pairing, same-fleet identity, zero overflow counters, coalescing totals and legacy field validity. It separates capped SITE/B2B detail from daily aggregate evidence, and startup/exit save errors. It does not infer native task boundaries or actual fuel events.

Script SHA-256: `d1e45023a555542581b588994179eed32ba8e89daf69f2a5f02b86e07a7b2c44`

````python
"""Review the supplied 0.18.7 log. Diagnostic evidence, never exact native burn.
PROBE records have 11 lines; following panel fields must not contaminate them.
"""
from pathlib import Path
import collections
import hashlib
import json
import re
import struct
import sys

path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('.cache/panel-game-test-3.log')
raw = path.read_bytes()
lines = raw.decode('utf-8-sig').splitlines()

def fields(text):
    return {k: v.strip('"') for k, v in re.findall(r'(\w+)=("[^"]*"|\S+)', text)}

def row(line, index):
    return dict(fields(line), line=index + 1, time=line[1:13])

def f32(x):
    return struct.unpack('<f', struct.pack('<f', x))[0]

panels, objects, probes, gaps, transfers = [], [], [], [], []
for i, line in enumerate(lines):
    if '[PANEL FUEL]' in line and 'sample=' in line:
        panels.append(row(line, i))
    if '[PANEL OBJECT]' in line:
        objects.append(row(line, i))
    if '[PANEL GAP]' in line:
        gaps.append(row(line, i))
    if '[PROBE SNAPSHOT]' in line:
        block = lines[i:i + 11]
        assert len(block) == 11 and 'home_matches_valid=' in block[-1]
        probes.append(dict(fields('\n'.join(block)), line=i + 1, time=line[1:13]))
    if '[B2B SITE] rva=' in line:
        r = row(line, i)
        r['resource'] = 'unlogged'
        for later in lines[i + 1:i + 6]:
            if '[B2B SITE]' in later:
                break
            if '[B2B] #' in later:
                match = re.search(r'res=(\S+)', later)
                if match:
                    r['resource'] = match[1]
                break
        transfers.append(r)

assert panels and len(panels) == len(objects)
assert [int(p['sample']) for p in panels] == list(range(1, len(panels) + 1))
for p, o in zip(panels, objects):
    assert p['epoch'] == o['epoch'] and p['sample'] == o['sample']
    assert p['root'] == o['object'] and o['membership'] == 'exact_fleet_pointer'
    assert o['occurrences'] == '1' and o['fleet_index'] == '0'
    assert p['status'] == 'complete' and p['aggregate_valid'] == '1'
    assert o['quantity_read'] == '1' and p['links'] == '0'
    assert p['ratio_valid'] == '1' and p['exact_burn'] == '0'
    assert f"{f32(f32(float(p['current_raw'])) * 1000.0):.2f}" == p['litres_candidate']
    assert p['queue_full_total_at_capture'] == '0' and p['queue_full_total_at_drain'] == '0'
assert len({p['root'] for p in panels}) == 1
assert len({o['candidate_home'] for o in objects}) == 1
assert all(g['reason'] == 'paused_snapshot_coalescing' for g in gaps)
assert sum(int(g['discarded_paused_snapshots']) for g in gaps) == sum(int(p['coalesced_snapshots']) for p in panels)
measurements = [p for p in probes if p['record_type'] == 'measurement']
assert measurements
for p in measurements:
    assert p['snapshot_ok'] == p['cargo_valid'] == p['mirror_valid'] == '1'
    assert p['fuel_slot_valid'] == '0' and p['state'] == p['mode'] == '0' and p['kind'] == '-1'
    assert p['veh'] == panels[0]['root'] and p['do'] == objects[0]['candidate_home']

print('INPUT', path, 'SHA256', hashlib.sha256(raw).hexdigest(), 'bytes', len(raw), 'lines', len(lines))
print('COUNTS', dict(panel=len(panels), object=len(objects), probe=len(probes), measurements=len(measurements)))
print('IDENTITY', {k: objects[0][k] for k in ('object', 'metadata', 'candidate_home', 'membership', 'fleet_index', 'occurrences')})
print('WINDOWS', dict(collections.Counter(p['window'] for p in panels)))
print('PASS all float32 display, pairing, validity, exact membership, sequence and probe-block assertions')

matches = []
for name, target in zip(range(7, 13), ('91.91', '84.83', '70.52', '56.75', '41.57', '29.26')):
    found = [p for p in panels if p['litres_candidate'] == target]
    assert found, target
    first, last = found[0], found[-1]
    result = dict(image=f'image-{name}.png', litres=target, count=len(found), first_sample=int(first['sample']),
                  last_sample=int(last['sample']), first_line=first['line'], last_line=last['line'],
                  first_drain_time=first['time'], last_drain_time=last['time'], first_raw=first['current_raw'])
    matches.append(result)
    print('SCREENSHOT', json.dumps(result, sort_keys=True))
    image_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path('/tmp/arena-coding-uploads')
    image = image_dir / result['image']
    if image.is_file():
        print('IMAGE_SHA256', image.name, hashlib.sha256(image.read_bytes()).hexdigest())
    else:
        print('IMAGE_HASH_SKIPPED', image.name, '(image not supplied; log assertions still run)')
values = [float(p['current_raw']) * 1000.0 for p in panels]
print('SAMPLED_CHANGES', dict(collections.Counter('decrease' if b < a else 'increase' if b > a else 'flat' for a, b in zip(values, values[1:]))))
print('NET_DISPLAY_DECREASE_LITRES_CANDIDATE', values[0] - values[-1], 'NOT exact burn')
print('ROUNDED_SCREENSHOT_ADJACENT_DIFFERENCES', [round(float(a['litres']) - float(b['litres']), 2) for a, b in zip(matches, matches[1:])])
print('MAX_CAPTURE_SPACING_MS', max(int(b['ms']) - int(a['ms']) for a, b in zip(panels, panels[1:])))
print('GAP_COUNT', len(gaps), 'COALESCED_SNAPSHOTS', sum(int(g['discarded_paused_snapshots']) for g in gaps), 'QUEUE_FULL_COUNTERS', 0)
for g in gaps:
    print('GAP', json.dumps(g, sort_keys=True))

print('CARGO_PHASE_CHANGES (sample brackets, not native boundaries)')
previous = None
for p in measurements:
    cargo = p['cargo']
    phase = 'empty' if float(p['cargo_total_tons']) == 0 else 'food' if 'food=' in cargo else 'electronics' if 'eletronics=' in cargo or 'electronics=' in cargo else cargo
    if phase != previous:
        index = measurements.index(p)
        if index:
            before = measurements[index - 1]
            print('  PREVIOUS', {k: before[k] for k in ('observation_seq', 'line', 'time', 'cargo_total_tons', 'cargo')})
        print('  ENTER', phase, {k: p[k] for k in ('observation_seq', 'line', 'time', 'cargo_total_tons', 'cargo')})
    previous = phase
for resource in ('food', 'electronics', 'eletronics'):
    group = [p for p in measurements if resource + '=' in p['cargo']]
    if group:
        high = max(group, key=lambda p: float(p['cargo_total_tons']))
        print('CARGO_MAX', resource, {k: high[k] for k in ('observation_seq', 'line', 'time', 'cargo_total_tons')})
print('PRINTED_TRANSFERS (incomplete detail when daily log cap suppresses records)')
for key, count in sorted(collections.Counter((t['rva'], t['resource'], t['commit']) for t in transfers).items()):
    print('  ', key, count)
for label, needle in [('DAILY_TOTAL', '[B2B] day'), ('DAILY_SITE', '[B2B SITE] day'), ('TARIFF_TOTAL', '[TARIFF MODEL] day'),
                      ('TANK', 'selected_source='), ('PRICE', '[IDX] day'), ('PAY', '[PAY] day'), ('AUTO', '[AUTO DO] day')]:
    for i, line in enumerate(lines):
        if needle in line and (label != 'AUTO' or 'summary:' in line):
            print(label, 'line', i + 1, line)
last_panel_line = panels[-1]['line']
saved_last_errors = [(i + 1, line) for i, line in enumerate(lines) if 'game.ERROR' in line and 'saved_last/' in line]
exit_errors = [r for r in saved_last_errors if r[0] > last_panel_line]
print('SAVED_LAST_ERRORS_TOTAL', len(saved_last_errors), 'BEFORE_CAPTURE', len(saved_last_errors) - len(exit_errors), 'AT_EXIT', len(exit_errors))
print('EXIT_ERROR_FIRST_LAST', exit_errors[0], exit_errors[-1])
other_errors = [(i + 1, line) for i, line in enumerate(lines) if 'game.ERROR' in line and 'saved_last/' not in line]
print('OTHER_GAME_ERRORS_COUNT', len(other_errors))
print('LOADED_SAVE_READ_ERRORS', [r for r in other_errors if 'Read error' in r[1]])
print('SAVE_ACTIVITY', [(i + 1, line) for i, line in enumerate(lines) if 'persistence  writing' in line or 'manifest written' in line])
attrs = []
for i, line in enumerate(lines):
    if '[B2B] #' in line:
        m = re.search(r'res=(\S+).*\(site (0x[0-9A-F]+), mode (\d+)\)', line)
        assert m, line
        attrs.append((m[2], m[1]))
print('PRINTED_B2B_ATTRIBUTION_COUNTS', sorted(collections.Counter(attrs).items()))
Path('.cache/diagnostic-game-test-3-analysis.json').write_text(json.dumps(dict(matches=matches, panels=panels, probes=probes, gaps=gaps, transfers=transfers), indent=2))
````

Actual command: `python .cache/analyze_diagnostic_game_test_3.py`

```text
INPUT .cache/panel-game-test-3.log SHA256 e04e4c3d1a616ab29c4c18c71c3b7d8a8ee192cc1739440fcc590332a2304d6e bytes 652407 lines 4942
COUNTS {'panel': 155, 'object': 155, 'probe': 153, 'measurements': 151}
IDENTITY {'object': '000002D3174E8030', 'metadata': '000002D2C73234D0', 'candidate_home': '000002D3174CE9C0', 'membership': 'exact_fleet_pointer', 'fleet_index': '0', 'occurrences': '1'}
WINDOWS {'000002D32802DBC0': 1, '000002D328027AC0': 154}
PASS all float32 display, pairing, validity, exact membership, sequence and probe-block assertions
SCREENSHOT {"count": 1, "first_drain_time": "17:48:26.765", "first_line": 800, "first_raw": "0.0919097364", "first_sample": 1, "image": "image-7.png", "last_drain_time": "17:48:26.765", "last_line": 800, "last_sample": 1, "litres": "91.91"}
IMAGE_SHA256 image-7.png f5d9b02338b9fd591d959aa213daacb5e06a1b33dcef8aef2793183aa330b96c
SCREENSHOT {"count": 1, "first_drain_time": "17:48:57.034", "first_line": 1816, "first_raw": "0.0848258287", "first_sample": 23, "image": "image-8.png", "last_drain_time": "17:48:57.034", "last_line": 1816, "last_sample": 23, "litres": "84.83"}
IMAGE_SHA256 image-8.png c10538acfba3467cbc0d5662a87a6fb99bcd46c78b1625c7b9e9eda6d26264cc
SCREENSHOT {"count": 9, "first_drain_time": "17:49:34.593", "first_line": 2329, "first_raw": "0.0705155358", "first_sample": 57, "image": "image-9.png", "last_drain_time": "17:49:49.041", "last_line": 3062, "last_sample": 65, "litres": "70.52"}
IMAGE_SHA256 image-9.png 2482a94f20c1c1d42a8213e8fab80e3ca0be9bfaa9f4142d538aab5f4e183cc4
SCREENSHOT {"count": 3, "first_drain_time": "17:53:34.200", "first_line": 3640, "first_raw": "0.0567482524", "first_sample": 100, "image": "image-10.png", "last_drain_time": "17:53:36.209", "last_line": 3770, "last_sample": 102, "litres": "56.75"}
IMAGE_SHA256 image-10.png 484d303d4d9d996a8bee5d2671b7883256d25e53eeb38523d5d0352d39901f7e
SCREENSHOT {"count": 5, "first_drain_time": "17:54:05.148", "first_line": 4216, "first_raw": "0.0415683351", "first_sample": 129, "image": "image-11.png", "last_drain_time": "17:54:17.642", "last_line": 4591, "last_sample": 133, "litres": "41.57"}
IMAGE_SHA256 image-11.png c731d63a254454b4ee2129a24ffc1592c0c06f870bbc2a55182e5ab6f0b5b6ad
SCREENSHOT {"count": 1, "first_drain_time": "17:55:26.408", "first_line": 4868, "first_raw": "0.0292567909", "first_sample": 155, "image": "image-12.png", "last_drain_time": "17:55:26.408", "last_line": 4868, "last_sample": 155, "litres": "29.26"}
IMAGE_SHA256 image-12.png 0feedcfc1752497b40db8e531841ad9fdc93b3e9ca0787a3a348aecfcd4dea0e
SAMPLED_CHANGES {'decrease': 134, 'flat': 20}
NET_DISPLAY_DECREASE_LITRES_CANDIDATE 62.6529455 NOT exact burn
ROUNDED_SCREENSHOT_ADJACENT_DIFFERENCES [7.08, 14.31, 13.77, 15.18, 12.31]
MAX_CAPTURE_SPACING_MS 190922
GAP_COUNT 6 COALESCED_SNAPSHOTS 253 QUEUE_FULL_COUNTERS 0
GAP {"discarded_paused_snapshots": "11", "epoch": "2", "exact_burn": "0", "first_ms": "30751312", "latest_ms": "30764796", "line": 799, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "1", "simulation_event_coverage": "unverified", "time": "17:48:26.765"}
GAP {"discarded_paused_snapshots": "4", "epoch": "2", "exact_burn": "0", "first_ms": "30790812", "latest_ms": "30795531", "line": 1815, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "23", "simulation_event_coverage": "unverified", "time": "17:48:57.034"}
GAP {"discarded_paused_snapshots": "4", "epoch": "2", "exact_burn": "0", "first_ms": "30839796", "latest_ms": "30845203", "line": 3057, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "63", "simulation_event_coverage": "unverified", "time": "17:49:46.842"}
GAP {"discarded_paused_snapshots": "184", "epoch": "2", "exact_burn": "0", "first_ms": "30883390", "latest_ms": "31073312", "line": 3639, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "100", "simulation_event_coverage": "unverified", "time": "17:53:34.200"}
GAP {"discarded_paused_snapshots": "7", "epoch": "2", "exact_burn": "0", "first_ms": "31107312", "latest_ms": "31115781", "line": 4577, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "132", "simulation_event_coverage": "unverified", "time": "17:54:16.825"}
GAP {"discarded_paused_snapshots": "43", "epoch": "2", "exact_burn": "0", "first_ms": "31139000", "latest_ms": "31184375", "line": 4867, "policy": "keep_latest_same_window_root", "reason": "paused_snapshot_coalescing", "sample": "155", "simulation_event_coverage": "unverified", "time": "17:55:26.407"}
CARGO_PHASE_CHANGES (sample brackets, not native boundaries)
  ENTER empty {'observation_seq': '3', 'line': 832, 'time': '17:48:29.691', 'cargo_total_tons': '0.000000', 'cargo': ''}
  PREVIOUS {'observation_seq': '17', 'line': 1018, 'time': '17:48:43.461', 'cargo_total_tons': '0.000000', 'cargo': ''}
  ENTER food {'observation_seq': '18', 'line': 1079, 'time': '17:48:44.463', 'cargo_total_tons': '0.435142', 'cargo': 'food=0.435t '}
  PREVIOUS {'observation_seq': '62', 'line': 2999, 'time': '17:49:39.892', 'cargo_total_tons': '0.424414', 'cargo': 'food=0.424t '}
  ENTER empty {'observation_seq': '63', 'line': 3064, 'time': '17:49:49.041', 'cargo_total_tons': '0.000000', 'cargo': ''}
  PREVIOUS {'observation_seq': '98', 'line': 3519, 'time': '17:50:23.354', 'cargo_total_tons': '0.000000', 'cargo': ''}
  ENTER electronics {'observation_seq': '99', 'line': 3672, 'time': '17:53:34.705', 'cargo_total_tons': '0.573037', 'cargo': 'eletronics=0.573t '}
  PREVIOUS {'observation_seq': '129', 'line': 4554, 'time': '17:54:07.275', 'cargo_total_tons': '0.044399', 'cargo': 'eletronics=0.044t '}
  ENTER empty {'observation_seq': '130', 'line': 4580, 'time': '17:54:17.432', 'cargo_total_tons': '0.000000', 'cargo': ''}
CARGO_MAX food {'observation_seq': '23', 'line': 1799, 'time': '17:48:50.528', 'cargo_total_tons': '5.500000'}
CARGO_MAX eletronics {'observation_seq': '101', 'line': 3772, 'time': '17:53:36.621', 'cargo_total_tons': '1.962089'}
PRINTED_TRANSFERS (incomplete detail when daily log cap suppresses records)
   ('0x68F792', 'eletronics', '1') 72
   ('0x68F792', 'food', '1') 143
   ('0x68F792', 'unlogged', '0') 29
   ('0x68F7F9', 'eletronics', '1') 72
   ('0x68F7F9', 'food', '1') 143
   ('0x694F40', 'eletronics', '1') 4
   ('0x694F40', 'food', '1') 161
   ('0x694F40', 'unlogged', '0') 130
   ('0x6953C4', 'eletronics', '1') 4
   ('0x6953C4', 'food', '1') 161
DAILY_TOTAL line 2267 [17:49:33.785] [B2B] day 14 closed: 375 calls, 322 committed, 53 no-op | unknown 0, staging 0, non-private 161 | suppressed site 0 attrib 0 pay 0
DAILY_TOTAL line 3968 [17:53:53.399] [B2B] day 15 closed: 4576 calls, 436 committed, 4140 no-op | unknown 0, staging 143, non-private 218 | suppressed site 4176 attrib 36 pay 0
DAILY_SITE line 2268 [17:49:33.785] [B2B SITE] day 14  0x694F40 load      calls=214 commit=161 in=0.000 out=5.500 value=856.39 | arg9!=0 0 home!=0 0
DAILY_SITE line 2269 [17:49:33.785] [B2B SITE] day 14  0x6953C4 transship calls=161 commit=161 in=5.500 out=0.000 value=856.39 | arg9!=0 0 home!=0 161
DAILY_SITE line 3969 [17:53:53.399] [B2B SITE] day 15  0x68F792 delivery  calls=172 commit=143 in=5.500 out=0.000 value=856.39 | arg9!=0 172 home!=0 0
DAILY_SITE line 3970 [17:53:53.399] [B2B SITE] day 15  0x68F7F9 transship calls=143 commit=143 in=0.000 out=5.500 value=0.00 | arg9!=0 143 home!=0 143
DAILY_SITE line 3971 [17:53:53.399] [B2B SITE] day 15  0x694F40 load      calls=4186 commit=75 in=0.000 out=1.962 value=3549.65 | arg9!=0 0 home!=0 0
DAILY_SITE line 3972 [17:53:53.399] [B2B SITE] day 15  0x6953C4 transship calls=75 commit=75 in=1.962 out=0.000 value=3549.65 | arg9!=0 0 home!=0 75
TARIFF_TOTAL line 2271 [17:49:33.785] [TARIFF MODEL] day 14 closed: 0 priced_transfer_fragment(s), model 0.00 = fuel 0.00 x1.35 + wear 0.00, mean 0.00 | indexed by fuel 0, by cpi 0, unindexed 0 | NOT journeys or payment totals; billing=0 dry_run=1
TARIFF_TOTAL line 3974 [17:53:53.399] [TARIFF MODEL] day 15 closed: 143 priced_transfer_fragment(s), model 478.70 = fuel 382.62 x1.35 + wear 96.08, mean 3.35 | indexed by fuel 143, by cpi 0, unindexed 0 | NOT journeys or payment totals; billing=0 dry_run=1
TANK line 2279 [17:49:33.787] [TANK] day 14 v=000002D3174E8030 selected_source=display_5f0_fallback source_valid=1 slot_found=0 slot_candidate_raw=0 display_5f0_raw=0.0708166063 drift_valid=0 drift_raw=+0 change=baseline decrease_raw=0 units=unverified exact_burn=0 verified_refill=0 billing_input=0
TANK line 3982 [17:53:53.399] [TANK] day 15 v=000002D3174E8030 selected_source=display_5f0_fallback source_valid=1 slot_found=0 slot_candidate_raw=0 display_5f0_raw=0.0493838787 drift_valid=0 drift_raw=+0 change=sample_decrease decrease_raw=0.0214327276 units=unverified exact_burn=0 verified_refill=0 billing_input=0
PRICE line 2280 [17:49:33.787] [IDX] day 14 closed: fuel 'fuel' = 134.37/RUB (resolved) | cpi basket 1.71/day | margin 35%, wear 5.00 L
PRICE line 4020 [17:53:53.399] [IDX] day 15 closed: fuel 'fuel' = 134.37/RUB (resolved) | cpi basket 1.71/day | margin 35%, wear 5.00 L
PAY line 2270 [17:49:33.785] [PAY] day 14 closed: billing off
PAY line 3973 [17:53:53.399] [PAY] day 15 closed: billing off
AUTO line 2311 [17:49:33.828] [AUTO DO] day 14 summary: private offices=1 candidates=3 planned=0 added=0 existing=3 suppressed=0 invalid=0 mode=WRITE
AUTO line 4031 [17:53:53.434] [AUTO DO] day 15 summary: private offices=1 candidates=3 planned=0 added=0 existing=3 suppressed=0 invalid=0 mode=WRITE
SAVED_LAST_ERRORS_TOTAL 39 BEFORE_CAPTURE 1 AT_EXIT 38
EXIT_ERROR_FIRST_LAST (4904, '[17:55:32.203] game.ERROR Cannot open media_soviet/saved_last/header.bin header') (4941, '[17:55:32.258] game.ERROR Cannot open media_soviet/saved_last/windows.bin for save Windows')
OTHER_GAME_ERRORS_COUNT 77
LOADED_SAVE_READ_ERRORS [(736, "[17:47:57.324] game.ERROR Read error (8) - File 'media_soviet/save\\18522 - Testing DO/replace_history.bin' "), (737, "[17:47:57.329] game.ERROR Read error (8) - File 'media_soviet/save\\18522 - Testing DO/usedveh.bin' ")]
SAVE_ACTIVITY [(3625, '[17:50:57.255] persistence  writing -> media_soviet/save/autosave4'), (3634, '[17:50:57.356] save   manifest written to media_soviet/save/autosave4\\tesmioloader.save.ini'), (3796, '[17:53:38.784] persistence  writing -> media_soviet/save/autosave5'), (3805, '[17:53:38.870] save   manifest written to media_soviet/save/autosave5\\tesmioloader.save.ini')]
PRINTED_B2B_ATTRIBUTION_COUNTS [(('0x68F792', 'eletronics'), 72), (('0x68F792', 'food'), 143), (('0x68F7F9', 'eletronics'), 72), (('0x68F7F9', 'food'), 143), (('0x694F40', 'eletronics'), 57), (('0x694F40', 'food'), 161), (('0x6953C4', 'eletronics'), 57), (('0x6953C4', 'food'), 161)]
```


---

# 2026-10-30 targeted assignment trace — v0.18.8 read-only schedule observations

## Outcome and acceptance status

The user's “Proceed” authorized a targeted native assignment trace and logging only where the native fields were verified. The verified portion is now implemented in the existing plugin, **not a detached costing model**. This is a changed diagnostic revision, **0.18.8-schedule-observations**, not a completed whole-assignment logger.

| Requested capability | Actual status |
|---|---|
| Identify the selected truck and its schedule/index/config reads | Verified renderer provenance; bounded read-only observer implemented |
| Distinguish observed stop-index changes from observed row/config changes | Implemented as separate sampled-change flags, not lifecycle events |
| Stable native assignment generation; repeat identical jobs | Unresolved; identical visible fields cannot distinguish unseen reassignment |
| Successful completion versus cancellation/replacement | Unresolved; no success/cancel/replace label is emitted |
| Intended buyer, return-to-DO classification | Unresolved; targets/context remain raw pointers, home is NOT buyer |
| Whole-job start/end fuel estimate and two completed job records | NOT implemented; cannot truthfully promise these from current evidence |
| Exact deductions/refills/clamps, physical tonne conversion | Still separately blocked on missing simulation writers/matching executable |
| Settlement, defaults and working simple DO | Billing OFF; existing dispatch, permissions and loaded-only core unchanged |

No unchanged v0.18.7 retest is requested. No MSVC build or real-game run of v0.18.8 is claimed. Earlier game-test results in this document remain results of their named versions, not validation of these new native reads.

## Native evidence: targeted, not a new gauge/string search

Authority: `RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md`, UTF-16 original, SHA-256 **2f3c638236c3c7d2e26cd97fe46d570748b497b1a69272c7358cac7877ce20e0**. The exported renderer itself is `FUN_1407D5460`. Capstone 5.0.7 independently decodes all **18,611 contiguous exported instructions** in `1407D5460..1407EBB7B`. There are no indirect jumps or branches outside that interval in this exported instruction CFG. This validates decoding/boundaries, not native runtime behavior or every Ghidra operand spelling.

### Selected-root and row-layout evidence

- `1407D5550`: `R15 = [RDX+240]`, the selected window root; saved to `[RBP+40]` at `1407D5557`. R15 is restored from this slot at several later points, including `1407E356E`. There is no intervening R15 reassignment within the investigated schedule region `1407E6904..1407E9C89`; later function epilogue restores and the next function’s setup are outside this claim. **Do not treat `[RBP+40]` itself as a root during the row loop:** `1407E813F` and `1407E8195` repurpose it as row scratch.
- `1407E6904/690B` and `1407E8143/814A`: end minus begin, **root+688/root+680**, shifted right by 3, is the raw target-slot count. Targets are 8-byte values; their pointees are not traversed by the observer.
- `1407E8084/808B` initialize both row counters to zero; `1407E9C74..9C7E` increment both. `1407E815D..8173` reads the indexed target and computes configuration address **[root+638] + row*198**. `+198` is a byte stride, not a task ID.
- `1407E8ED2` reads config byte `+00`; `1407E9273` reads float `+04`; `1407E8EB9` reads byte `+C8`; `1407E8F07` reads float `+CC`. The diagnostic preserves raw field names, not guessed office thresholds or charging rules. These are not the DO's source10/destination30 percentages.
- **Special last row:** `1407E81B3..81C0` checks whether this is the last raw row and root byte `+5FD` is nonzero, then clears R14/RSI. The observer therefore reads the raw target slot but deliberately does **not** read configuration fields for that last row. It does not call this a return-to-DO stop or job completion.
- Null targets use native fallback config `+190` for part of the renderer; vectors at config `+08/+D0` and additional flags also exist. This implementation does not decode those or claim that its selected fields represent the full schedule. Resource/filter-only changes can be invisible.
- DWORD **root+698** is compared with the rendered row at `1407E8438`, `1407E8649`, `1407E870C`, `1407E8A3D`, `1407E8CB3`, `1407E8DDF`. The equality branches drive row presentation. This is an observed native row index, **not a whole-job generation/completion ID**. Out-of-range/negative values are preserved, not given a completion meaning.
- QWORD **root+610** is compared with the row target at `1407E8CC6/8DF2`. QWORD **root+650** is tested and its pointee used in other UI branches. Neither is a verified job ID, buyer or termination reason. The observer reads the pointer values only, never their pointees. Root `+4A0` and byte `+5FD` are similarly logged as raw context.

The complete runnable export audit below performs a local reaching-definition check of R15 and R12, including their direct frame-slot saves/restores and overlapping direct RBP/RSP writes. The prologue gives RBP-RSP=110 in the body; RSP+150 aliases RBP+40. At the selected schedule sites R15 reaches only the selected-root load even after the frame slot becomes scratch. It also confirms R12's window provenance at the two following writer-input sites. **Limits:** calls are assumed to honor the Windows x64 nonvolatile-register ABI; arbitrary escaped stack aliases/callee side effects, native lifetime, hidden threads and executable authenticity are not proved by this local analysis.

### Actual writers found — UI mutation is not successful delivery

The additional direct-store audit found **two** renderer stores to these selected root fields. They must not be mistaken for simulation completion writers:

```asm
1407D9FEB: MOV EAX,dword ptr [R12 + 0x264]
1407D9FF3: MOV dword ptr [R15 + 0x698],EAX
...
1407DA036: MOVSXD RCX,dword ptr [R12 + 0x264]
1407DA03E: MOV RAX,qword ptr [R15 + 0x680]
1407DA045: MOV RCX,qword ptr [RAX + RCX*0x8]
1407DA049: MOV qword ptr [R15 + 0x610],RCX
```

R12 is the window here and R15 the selected root. The surrounding renderer path tests R14D against `5` and root `+528/+530` before choosing these alternatives; subsequent writes include `+668=1` and `+66C=-1.0f`. **The UI command's exact user-facing name and those auxiliary flags are not asserted.** What is established is that the renderer can write the row index/auxiliary target from a window-selected row. A later observed `+698` change therefore cannot uniquely prove natural stop completion, let alone assignment success.

There are also opaque call-mediated mutations/validation paths. At **1407E9D43**, `RCX=R15`, then **1407E9D46 calls FUN_1406BF1D0**, with zeroed RDX/R8/R9. Other renderer calls at `1407D9959/9C51` pass the same root with DL=1. At `1407E9CEF`, `FUN_1403BB640` receives the selected row target and root among its arguments; its predicate meaning remains unverified. The available raw exports contain call sites but no instruction body starting at either `1406BF1D0` or `1403BB640`. **Do not install hooks on these guesses.**

## Implemented read-only adapter and its limits

- New `schedule_observer.h`: typed bounded reads via the existing guarded `Memory` reader. At most **16 raw rows**; oversized tables are rejected, not silently truncated. Invalid arithmetic, reversed/misaligned bounds, missing ordinary-row configs, unreadable or nonfinite selected fields reject the observation. No game calls, allocation, native writes, persistence or payment API.
- Reads the header and selected rows twice; an observed change invalidates the observation. This is **not an atomic native snapshot** and cannot rule out ABA, escaped aliases or unobserved field changes. Readable memory is not proof of allocated container size/lifetime.
- Additional **13 native instruction fingerprints** gate ONLY the new schedule reads. Existing PE/prologue/quantity fingerprints still gate the existing single renderer hook. Schedule mismatch leaves the existing fuel observer available. These fingerprints are not a full executable hash and do not validate simulation lifecycle.
- Eligibility requires that the selected root itself appears **exactly once** in its candidate-home type-43 road-DO fleet. Candidate home is never treated as buyer; private ownership remains unclassified. No linked object is used as a substitute truck. Address reuse/native lifetime remain unverified.
- `PanelRecord` carries fuel and schedule values from the **same queued post-render capture**, including paused-tail replacement. Reads happen after the original renderer; existing nonblocking queue/throttle/cap behavior remains. No file/log I/O on the render path. A schedule-only guarded fault latches schedule reads OFF without disabling the existing fuel observer.
- Simulation ticks emit `[PANEL SCHEDULE]` plus `[PANEL SCHEDULE ROW]` only on first valid observation, relevant selected-field/index/storage/context changes, rebaseline or invalid read. Unchanged successful observations are silent; fuel/object lines retain their prior format/order. Invalid observations never print a partial row table. Invalid/default numeric fields must not be interpreted as a real empty schedule.
- `observed_fields_changed` compares only target, config-read flag and the four selected config fields; `row_index_changed`, `storage_changed`, and `opaque_context_changed` are separate. Storage relocation is not assigned-job generation. A raw list clear/replacement or index advance never becomes a successful-job event.
- A **single last-selected vehicle's value history**, keyed by root/home/epoch, is diagnostic-only. Switching root/home, load epoch, invalid reads and reported gaps causes rebaselining without deltas. Coalescing and capture-side drop-counter advances are handled. It does not retain/dereference old native pointers, accumulate fuel costs, allocate durable per-fleet job IDs or settle once-only invoices.
- Visibility is **panel-only**, at the existing nominal 1000 ms interval, 240 accepted records/load epoch and 16 queued records. The panel must be rendered; closing it, short-lived jobs, missed transitions and repeated identical schedules can be invisible. Log `ms` is host capture uptime, not exact native event time; queued logs can print later. Accepted sample IDs and successive valid reads do not prove event coverage. `[PANEL GAP]` remains the authoritative explicit queue/coalescing/cap/fault diagnostic.
- Every schedule record explicitly says **native_job_id=unverified buyer=unverified completion=unverified exact_burn=0 billing=off**. Raw target/opaque pointers are process-local evidence, not persistent job/buyer identities. Fuel shown alongside is the already-existing sampled display candidate, not exact physical burn. No new job start/end estimate is calculated.

Settings: `schedule_observer=1` in both INI and source fallback, with parent `panel_fuel_observer=1`. Existing INI values are identical to merged PR31: **billing=0, dry_run=1, source=10, destination=30**, existing permission/default rules unchanged. The source changes in `private_logistics.cpp` are only version/comment, new default flag and config read. Native hooks, money transfers, existing dispatch logic, scanner fuel formulas and the disconnected loaded-only tracker were not rewired.

## Actual validation and what remains untested

- Python regression suites: **238 PASS** = logistics89 + probe93 + shops4 + model38 + scanner14. The scanner suite is regression-only; no new broad gauge/string investigation was performed.
- All six strict optimized **and ASan/UBSan** host suites pass: provenance8, existing panel runtime8, panel reader16, journey core30, new schedule reader/history14, new schedule runtime8. These suite counts overlap Python wrappers; they are not additional independent native game evidence.
- New host coverage includes ordinary/special last rows, empty/null/out-of-range values, 16-row limit/overflow, unreadable/nonfinite data, header/row mutation, index-only versus selected-field/storage/context changes, identical visible schedules producing no invented job generation, epoch/identity/gap resets, latest paused schedule carriage, additional-fingerprint refusal without blocking fuel, delayed capture-drop advance and isolated schedule-only fault latching.
- Native fingerprint test verifies each of the 13 byte spans against whole instructions in the original raw export. Capstone export audit results and full stdout appear below.
- `git diff --check` passes. This Linux workspace has g++ but no MSVC/Windows DLL build. The host shim substitutes C++ exceptions/serial locks for Windows SEH/threading and does not execute a patched renderer. **No Windows ABI/SEH/real-thread, live offset/object-lifetime or native assignment validation is claimed.**

## Next evidence/build decision — no unchanged replay

The specific next native artifact is **raw assembly for FUN_1406BF1D0 and its caller list**, because the selected-root call convention is now directly established. Then follow only its actual schedule/index clearing/advancement callees to the simulation writers of the selected truck's `+698`, `+680/+688`, `+638` and any verified assignment identity/termination state. `FUN_1403BB640` can be supplied as the adjacent predicate if needed, but is not itself identified as an assignment terminal handler. This is a narrow missing-body request, not a request for another giant renderer decompilation or a broad string search. A matching executable/project would also permit that work directly; none is available here.

The requested two authoritative delivery-job records remain blocked until those signals can distinguish new assignment, internal stop transition, successful completion, cancellation/replacement and return travel, including repeating identical tasks. Buyer must be resolved from a verified job destination/party mapping, not the home DO. Empty return travel remains separate/unbilled pending allocation policy. Exact accounting still needs verified refill/deduction/clamp behavior and physical units; an eventual endpoint estimate must state its uncertainty and no-refuel assumptions.

For a later Windows build of this **changed diagnostic** source, not the old build:

1. This turn has **not committed, pushed or opened a PR**. Do not expect pulling the target branch to retrieve unpublished edits. If publication is requested, publish this session's fixed branch `arena/8e9a4f62-wrsre` and merge through the normal explicit PR workflow; PR31 is already merged and must not be repeated.
2. After publication/merge, VS Code: fetch, switch to the intended published branch (or the merged target), and pull/synchronize. Confirm source version **0.18.8-schedule-observations** before compiling; do not pull an unrelated historical Arena branch.
3. In the repository terminal on Windows with the installed Visual Studio C++ build tools: `.\build.bat private_logistics`. This builds `build\plugins\private_logistics.dll` and copies `build\plugins\private_logistics.ini`. Deploy both to the normal game plugin location while the game is closed, preserving the tested billing-OFF INI. No loader rebuild is requested.
4. Only after a successful new build and matched startup fingerprint, a schedule-correlation run would add new evidence. It would **not yet be the requested end-to-end two-job costing test**. If run later, use the same truck over the two deliveries and return while keeping its panel visible; retain the log and note user order edits/cancellation. No manual save is required; the prior run exited without saving. Do not repeat unchanged v0.18.7 now.

## Complete final source, audit driver and actual output

The following are exact file contents at this checkpoint, with hashes. Older embedded copies above are historical. Two unchanged fixture/reader dependencies are repeated to make the new portable host tests inspectable. Other unchanged plugin/API dependencies remain in the repository and earlier source sections. The ignored validation driver is included here so `.cache` persistence is not required for reproduction. Run from repository root; optional export audit requires Capstone 5.0.7 (for example install to `.cache/python-packages` and set `PYTHONPATH` as shown in its recorded command). No generated executables/log datasets are added to Git.

### Final source: `plugins/private_logistics/schedule_observer.h`

SHA-256: `5452101790b7ef03cb0b9eb31517ec909b8d9b83721ea2731be200cc19583fea`

````cpp
// Verified renderer schedule reads ONLY. Not a native job identity/completion API.
#ifndef PRIVATE_LOGISTICS_SCHEDULE_OBSERVER_H
#define PRIVATE_LOGISTICS_SCHEDULE_OBSERVER_H
#include "panel_fuel_observer.h"

namespace private_logistics_schedule {
using private_logistics_panel::Address;
using private_logistics_panel::Memory;
static_assert(sizeof(Address)==8,"verified schedule layout is x64 only");
constexpr unsigned MaxRows = 16;
constexpr Address ConfigStride = 0x198;
enum class Status { Disabled, Ineligible, Unreadable, BadBounds, TooManyRows,
                    MissingConfigs, NonFinite, ChangedDuringRead, GuardedFault, Complete };
struct Row {
    Address target = 0;
    bool configRead = false;
    std::uint8_t flag00 = 0, flagC8 = 0;
    float value04 = 0, valueCC = 0;
};
struct Snapshot {
    Status status = Status::Disabled;
    Address begin = 0, end = 0, configs = 0;
    // Opaque renderer context fields: do NOT name these job IDs or buyers.
    Address raw4a0 = 0, raw610 = 0, raw650 = 0;
    std::int32_t index698 = -1;
    std::uint8_t flag5fd = 0;
    unsigned count = 0;
    Row rows[MaxRows]{};
};
inline bool Header(const Memory& m, Address root, Snapshot& s) {
    return m.Get(root,0x680,s.begin) && m.Get(root,0x688,s.end) &&
        m.Get(root,0x638,s.configs) && m.Get(root,0x698,s.index698) &&
        m.Get(root,0x5FD,s.flag5fd) && m.Get(root,0x4A0,s.raw4a0) &&
        m.Get(root,0x610,s.raw610) && m.Get(root,0x650,s.raw650);
}
inline bool SameHeader(const Snapshot& a,const Snapshot& b) {
    return a.begin==b.begin && a.end==b.end && a.configs==b.configs &&
        a.index698==b.index698 && a.flag5fd==b.flag5fd &&
        a.raw4a0==b.raw4a0 && a.raw610==b.raw610 && a.raw650==b.raw650;
}
inline bool SameRow(const Row& a,const Row& b) {
    return a.target==b.target && a.configRead==b.configRead && a.flag00==b.flag00 && a.flagC8==b.flagC8 &&
        a.value04==b.value04 && a.valueCC==b.valueCC;
}
inline bool ReadRow(const Memory& m,const Snapshot& s,unsigned i,Row& r) {
    if(!m.Get(s.begin,Address(i)*8,r.target)) return false;
    // At 7E81B3..7E81C0 the renderer sets RSI=0 for the last row
    // when +5FD is nonzero. Do not invent/read a configuration for it.
    if(s.flag5fd && i+1==s.count) return true;
    const Address offset=Address(i)*ConfigStride;
    r.configRead=m.Get(s.configs,offset,r.flag00) && m.Get(s.configs,offset+4,r.value04) &&
        m.Get(s.configs,offset+0xC8,r.flagC8) && m.Get(s.configs,offset+0xCC,r.valueCC);
    return r.configRead;
}
inline void Capture(const Memory& m,Address root,Snapshot& s) {
    s=Snapshot{};s.status=Status::Unreadable;
    if(!Header(m,root,s)) return;
    if(s.end<s.begin || (s.end-s.begin)%8 || (!s.begin && s.end)) {
        s.status=Status::BadBounds;return;
    }
    const Address count=(s.end-s.begin)/8;
    if(count>MaxRows) {s.status=Status::TooManyRows;return;}
    s.count=static_cast<unsigned>(count);
    if(s.count>(s.flag5fd?1u:0u) && !s.configs) {s.status=Status::MissingConfigs;return;}
    for(unsigned i=0;i<s.count;++i) {
        if(!ReadRow(m,s,i,s.rows[i])) return;
        if(!std::isfinite(s.rows[i].value04) || !std::isfinite(s.rows[i].valueCC)) {
            s.status=Status::NonFinite;return;
        }
    }
    // Recheck header AND selected row fields; reject observed mutation. This
    // is still not an atomic game snapshot (ABA/other fields remain unknown).
    Snapshot after{};
    if(!Header(m,root,after)) return;
    if(!SameHeader(s,after)) {s.status=Status::ChangedDuringRead;return;}
    for(unsigned i=0;i<s.count;++i) {
        Row again{};
        if(!ReadRow(m,s,i,again)) return;
        if(!SameRow(s.rows[i],again)) {s.status=Status::ChangedDuringRead;return;}
    }
    s.status=Status::Complete;
}
inline const char* StatusName(Status s) {
    switch(s) {
        case Status::Disabled:return "disabled";
        case Status::Ineligible:return "not_unique_road_fleet_root";
        case Status::BadBounds:return "bad_bounds";
        case Status::TooManyRows:return "too_many_rows_no_partial_table";
        case Status::MissingConfigs:return "missing_configs";
        case Status::NonFinite:return "nonfinite_config";
        case Status::ChangedDuringRead:return "changed_during_read";
        case Status::GuardedFault:return "guarded_schedule_fault_disabled";
        case Status::Complete:return "complete_selected_fields";
        default:return "unreadable";
    }
}
enum class Baseline { None, First, Epoch, Identity, Gap, Invalid };
inline const char* BaselineName(Baseline b) {
    switch(b) {
        case Baseline::None:return "none";
        case Baseline::Epoch:return "load_epoch";
        case Baseline::Identity:return "vehicle_or_home_change";
        case Baseline::Gap:return "diagnostic_gap";
        case Baseline::Invalid:return "invalid_read";
        default:return "first_observation";
    }
}
struct Delta {
    bool emit=false, fields=false, storage=false, index=false, context=false;
    Baseline baseline=Baseline::None;
};
// One selected vehicle's previous *values*, never a retained native object
// consumer. Windows are transient and are deliberately not identity keys.
class History {
    bool known_=false, gap_=false;
    std::uint64_t epoch_=0;
    Address root_=0,home_=0;
    Snapshot previous_{};
public:
    void Break() {gap_=true;}
    Delta Observe(const Snapshot& s,std::uint64_t epoch,Address root,Address home) {
        Delta d;
        if(s.status!=Status::Complete) {
            known_=false;gap_=true;d.emit=true;d.baseline=Baseline::Invalid;return d;
        }
        if(!known_) d.baseline=gap_?Baseline::Gap:Baseline::First;
        else if(epoch!=epoch_) d.baseline=Baseline::Epoch;
        else if(root!=root_ || home!=home_) d.baseline=Baseline::Identity;
        else if(gap_) d.baseline=Baseline::Gap;
        if(d.baseline==Baseline::None) {
            d.fields=s.count!=previous_.count;
            if(!d.fields) for(unsigned i=0;i<s.count;++i)
                if(!SameRow(s.rows[i],previous_.rows[i])) {d.fields=true;break;}
            d.storage=s.begin!=previous_.begin || s.end!=previous_.end || s.configs!=previous_.configs;
            d.index=s.index698!=previous_.index698;
            d.context=s.raw4a0!=previous_.raw4a0 || s.raw610!=previous_.raw610 ||
                s.raw650!=previous_.raw650 || s.flag5fd!=previous_.flag5fd;
        }
        d.emit=d.baseline!=Baseline::None || d.fields || d.storage || d.index || d.context;
        epoch_=epoch;root_=root;home_=home;previous_=s;known_=true;gap_=false;
        return d;
    }
};
} // namespace private_logistics_schedule
#endif

````

### Final source: `plugins/private_logistics/panel_fuel_runtime.h`

SHA-256: `4d6454454a90ff646625e35448deeef6162d2b9a0244d7a08a5ccba821af2752`

````cpp
// Private to this plugin. Included after its services/globals are defined.
// Process-lifetime hooks, like the plugin's existing transfer hooks.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#include "panel_fuel_observer.h"
#include "schedule_observer.h"

static bool g_scheduleReady = false;
static volatile LONG g_scheduleFault = 0;
static private_logistics_schedule::History g_scheduleHistory; // simulation tick only
static LONG g_scheduleLastEpoch = 0;
static unsigned g_scheduleLastCaptureDrops = 0;
static CRITICAL_SECTION g_panelQueueLock;
static volatile LONG g_panelEpoch = 1, g_panelStarted = 0, g_panelFault = 0, g_panelContention = 0;
static LONG g_panelQueueEpoch = 0;
static unsigned g_panelHead = 0, g_panelCount = 0, g_panelSamples = 0, g_panelDropped = 0;
static ULONGLONG g_panelLastMs = 0;
static unsigned g_panelDroppedReported = 0;
static ULONGLONG g_panelFirstDropMs = 0, g_panelLastDropMs = 0;
static bool g_panelCapLogged = false, g_panelFaultLogged = false;
struct PanelRecord {
    private_logistics_panel::Snapshot snapshot;
    private_logistics_schedule::Snapshot schedule;
    LONG epoch;
    unsigned sequence;
    ULONGLONG milliseconds;
    ULONGLONG firstMilliseconds;
    unsigned queueFullAtCapture, coalesced;
    bool paused;
};
static PanelRecord g_panelQueue[16];
using PanelRenderer = void (__fastcall*)(void*, void*);
static void* g_panelTrampoline = nullptr;

static void PanelObserverLoading(void)
{
    // File thread: no game reads, queue lock, retained identities or persistence.
    InterlockedIncrement(&g_panelEpoch);
}
static bool PanelRead(uintptr_t address, void* out, size_t size, void*)
{
    if (address < 0x10000 || !ReadablePtr((const void*)address, size)) return false;
    memcpy(out, (const void*)address, size);
    return true;
}
static void PanelResetQueueLocked(LONG epoch)
{
    if (g_panelQueueEpoch == epoch) return;
    g_panelQueueEpoch = epoch;
    g_panelHead = g_panelCount = g_panelSamples = g_panelDropped = 0;
    g_panelLastMs = 0;
    g_panelDroppedReported = 0;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    g_panelCapLogged = false;
    InterlockedExchange(&g_panelContention, 0);
}
static void PanelCapture(void* context, void* window)
{
    if (!InterlockedCompareExchange(&g_panelStarted, 0, 0) ||
        InterlockedCompareExchange(&g_panelFault, 0, 0)) return;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) {
        InterlockedExchange(&g_panelContention, 1); return;
    }
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    const ULONGLONG now = GetTickCount64();
    if (g_panelSamples >= 240 || (g_panelLastMs && now - g_panelLastMs < 1000)) {
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    const bool paused = g_simClock && g_simClock->isPaused && g_simClock->isPaused() == 1;
    const unsigned tail = (g_panelHead + g_panelCount + 15) % 16;
    // Only replace a queued paused snapshot of the SAME window/root. Preserve
    // other vehicles and unpaused samples; no extra storage or render-thread I/O.
    private_logistics_panel::Snapshot snapshot{};
    bool captured = false;
    __try {
        captured = private_logistics_panel::Capture({PanelRead, nullptr},
            (uintptr_t)context, (uintptr_t)window, snapshot);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_panelFault, 1);
    }
    private_logistics_schedule::Snapshot schedule{};
    if (captured && g_scheduleReady) {
        schedule.status = private_logistics_schedule::Status::Ineligible;
        // Only the root itself, uniquely present in the verified road-DO fleet.
        // Never walk home as buyer, follow linked vehicles, or read +650's pointee.
        if (snapshot.count && snapshot.objects[0].pointer == snapshot.root &&
            snapshot.objects[0].match == private_logistics_panel::Match::Found &&
            snapshot.objects[0].occurrences == 1) {
            if (!InterlockedCompareExchange(&g_scheduleFault, 0, 0)) {
                __try {
                    private_logistics_schedule::Capture({PanelRead, nullptr}, snapshot.root, schedule);
                } __except (EXCEPTION_EXECUTE_HANDLER) {
                    InterlockedExchange(&g_scheduleFault, 1);
                    schedule.status = private_logistics_schedule::Status::GuardedFault;
                }
            } else schedule.status = private_logistics_schedule::Status::GuardedFault;
        }
    }
    if (captured && epoch == InterlockedCompareExchange(&g_panelEpoch, 0, 0)) {
        g_panelLastMs = now; // inactive windows do not consume the capture slot
        const bool coalesce = paused && snapshot.root && g_panelCount && g_panelQueue[tail].paused &&
            g_panelQueue[tail].snapshot.window == snapshot.window &&
            g_panelQueue[tail].snapshot.root == snapshot.root;
        if (coalesce) {
            auto& record = g_panelQueue[tail];
            record.snapshot = snapshot;
            record.schedule = schedule;
            record.milliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            if (record.coalesced < 0x7FFFFFFFu) ++record.coalesced;
        } else if (g_panelCount == 16) {
            if (!g_panelFirstDropMs) g_panelFirstDropMs = now;
            g_panelLastDropMs = now;
            if (g_panelDropped < 0x7FFFFFFFu) ++g_panelDropped;
        } else {
            auto& record = g_panelQueue[(g_panelHead + g_panelCount) % 16];
            record.snapshot = snapshot;
            record.schedule = schedule;
            record.epoch = epoch;
            record.sequence = ++g_panelSamples;
            record.milliseconds = record.firstMilliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            record.coalesced = 0;
            record.paused = paused;
            ++g_panelCount;
        }
    }
    LeaveCriticalSection(&g_panelQueueLock);
}
static void __fastcall PanelRendererDetour(void* context, void* window)
{
    ((PanelRenderer)g_panelTrampoline)(context, window);
    PanelCapture(context, window); // POST render, not an atomic read at the format call
}
static const char* PanelStatus(private_logistics_panel::Status s)
{
    using private_logistics_panel::Status;
    switch (s) {
    case Status::Complete: return "complete";
    case Status::TooManyLinks: return "too_many_links";
    case Status::NonFinite: return "nonfinite";
    case Status::Overflow: return "overflow";
    default: return "unreadable";
    }
}
static const char* PanelMatch(private_logistics_panel::Match m)
{
    using private_logistics_panel::Match;
    switch (m) {
    case Match::Found: return "exact_fleet_pointer";
    case Match::Absent: return "absent_from_candidate_home";
    case Match::NotRoadOffice: return "candidate_home_not_road_do";
    case Match::InvalidFleet: return "invalid_candidate_fleet";
    default: return "unavailable";
    }
}
static void ScheduleObserverLog(const PanelRecord& record, bool gap)
{
    if (!g_scheduleReady) return;
    if (gap || record.coalesced ||
        (g_scheduleLastEpoch == record.epoch &&
         g_scheduleLastCaptureDrops != record.queueFullAtCapture)) g_scheduleHistory.Break();
    g_scheduleLastEpoch = record.epoch;
    g_scheduleLastCaptureDrops = record.queueFullAtCapture;
    const auto& fuel = record.snapshot;
    const auto& s = record.schedule;
    const uintptr_t home = fuel.count ? fuel.objects[0].homeCandidate : 0;
    const auto d = g_scheduleHistory.Observe(s, (unsigned long long)record.epoch, fuel.root, home);
    if (!d.emit) return;
    Logf("[PANEL SCHEDULE] epoch=%ld sample=%u ms=%llu root=%p candidate_home=%p "
         "status=%s baseline=%s observed_fields_changed=%d storage_changed=%d "
         "row_index_changed=%d opaque_context_changed=%d begin680=%p end688=%p configs638=%p "
         "raw_row_count=%llu captured_rows=%u index698=%d flag5fd=%u "
         "raw4a0=%p raw610=%p raw650=%p fuel_valid=%d fuel_root_raw=%.9g "
         "aggregate_litres_candidate=%.2f visibility=panel_only phase=post_render "
         "native_event_coverage=unverified native_job_id=unverified buyer=unverified "
         "completion=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)fuel.root, (void*)home, private_logistics_schedule::StatusName(s.status),
         private_logistics_schedule::BaselineName(d.baseline), d.fields ? 1 : 0,
         d.storage ? 1 : 0, d.index ? 1 : 0, d.context ? 1 : 0,
         (void*)s.begin, (void*)s.end, (void*)s.configs,
         (unsigned long long)(s.end >= s.begin ? (s.end - s.begin) / 8 : 0),
         s.status == private_logistics_schedule::Status::Complete ? s.count : 0,
         (int)s.index698, (unsigned)s.flag5fd, (void*)s.raw4a0, (void*)s.raw610, (void*)s.raw650,
         fuel.aggregateValid ? 1 : 0, fuel.count ? (double)fuel.objects[0].raw : 0.0,
         (double)fuel.litresCandidate);
    if (s.status != private_logistics_schedule::Status::Complete) return;
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& r = s.rows[i];
        Logf("[PANEL SCHEDULE ROW] epoch=%ld sample=%u row=%u target_raw=%p "
             "config_fields_read=%d flag00=%u value04=%.9g flagc8=%u valuecc=%.9g "
             "target_role=unverified buyer=unverified",
             record.epoch, record.sequence, i, (void*)r.target, r.configRead ? 1 : 0,
             (unsigned)r.flag00, (double)r.value04, (unsigned)r.flagC8, (double)r.valueCC);
    }
}
static void PanelObserverTick(void*, float, float)
{
    PanelRecord record{};
    bool have = false, cap = false, fault = false, contention = false;
    unsigned dropped = 0, newDrops = 0;
    ULONGLONG firstDropMs = 0, lastDropMs = 0;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    if (g_panelCount) {
        record = g_panelQueue[g_panelHead];
        g_panelHead = (g_panelHead + 1) % 16; --g_panelCount; have = true;
    }
    contention = InterlockedExchange(&g_panelContention, 0) != 0;
    dropped = g_panelDropped;
    newDrops = dropped - g_panelDroppedReported;
    g_panelDroppedReported = dropped;
    firstDropMs = g_panelFirstDropMs; lastDropMs = g_panelLastDropMs;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    cap = g_panelSamples >= 240 && !g_panelCapLogged;
    if (cap) g_panelCapLogged = true;
    fault = InterlockedCompareExchange(&g_panelFault, 0, 0) && !g_panelFaultLogged;
    if (fault) g_panelFaultLogged = true;
    LeaveCriticalSection(&g_panelQueueLock);
    if (epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (fault) Logf("[PANEL GAP] epoch=%ld reason=guarded_read_fault observer=disabled exact_burn=0", epoch);
    if (cap) Logf("[PANEL GAP] epoch=%ld reason=capture_cap cap=240 capture=stopped exact_burn=0", epoch);
    if (contention)
        Logf("[PANEL GAP] epoch=%ld reason=render_trylock_contention skipped_render_attempts=unknown "
             "simulation_event_coverage=unverified exact_burn=0", epoch);
    if (newDrops)
        Logf("[PANEL GAP] epoch=%ld reason=queue_full skipped_snapshot_captures=%u "
             "first_ms=%llu last_ms=%llu queue_full_total_at_drain=%u "
             "sample_ids=accepted_records_only simulation_event_coverage=unverified exact_burn=0",
             epoch, newDrops, (unsigned long long)firstDropMs,
             (unsigned long long)lastDropMs, dropped);
    if (g_scheduleReady && (fault || cap || contention || newDrops)) g_scheduleHistory.Break();
    if (!have || record.epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (record.coalesced)
        Logf("[PANEL GAP] epoch=%ld sample=%u reason=paused_snapshot_coalescing "
             "discarded_paused_snapshots=%u first_ms=%llu latest_ms=%llu "
             "policy=keep_latest_same_window_root simulation_event_coverage=unverified exact_burn=0",
             record.epoch, record.sequence, record.coalesced,
             (unsigned long long)record.firstMilliseconds, (unsigned long long)record.milliseconds);
    const auto& s = record.snapshot;
    Logf("[PANEL FUEL] epoch=%ld sample=%u ms=%llu phase=post_render window=%p root=%p "
         "status=%s aggregate_valid=%d current_raw=%.9g litres_candidate=%.2f "
         "capacity_unadjusted=%.9g ratio_valid=%d ratio=%.9g links=%u "
         "mode=%d metadata_kind=%d flags_read=%d flag8681=%u flag8682=%u "
         "adjustment_read=%d adjustment_needed=%d presentation=%s "
         "queue_full_total_at_capture=%u queue_full_total_at_drain=%u "
         "captured_paused=%d coalesced_snapshots=%u sample_ids=accepted_records_only "
         "physical_fuel=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)s.window, (void*)s.root, PanelStatus(s.status), s.aggregateValid ? 1 : 0,
         (double)s.current, (double)s.litresCandidate, (double)s.unadjustedCapacity,
         s.ratioValid ? 1 : 0, (double)s.ratio, s.links, s.contextMode, s.metadataKind,
         s.flagsRead ? 1 : 0, (unsigned)s.flag8681, (unsigned)s.flag8682,
         s.adjustmentRead ? 1 : 0, s.adjustmentNeeded ? 1 : 0,
         s.flagsRead && (s.flag8681 || s.flag8682) ? "alternate_if_shown" : "unresolved_7c58",
         record.queueFullAtCapture, dropped, record.paused ? 1 : 0, record.coalesced);
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& o = s.objects[i];
        Logf("[PANEL OBJECT] epoch=%ld sample=%u index=%u role=%s object=%p metadata=%p "
             "quantity_read=%d raw5f0=%.9g capacity8684=%.9g candidate_home=%p "
             "membership=%s fleet_index=%u occurrences=%u private_status=not_classified",
             record.epoch, record.sequence, i, i ? "linked" : "root", (void*)o.pointer,
             (void*)o.metadata, o.quantityRead ? 1 : 0, (double)o.raw, (double)o.capacity,
             (void*)o.homeCandidate, PanelMatch(o.match), o.fleetIndex, o.occurrences);
    }
    ScheduleObserverLog(record, fault || cap || contention || newDrops);
}
static bool PanelTargetMatches(void)
{
    // Fingerprint, not a recomputed SHA-256. Operator must verify the file hash.
    if (!g_exeBase || g_exeSize < 0x90B080 || !ReadablePtr(g_exeBase, sizeof(IMAGE_DOS_HEADER))) return false;
    const auto* dos = (const IMAGE_DOS_HEADER*)g_exeBase;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 || dos->e_lfanew > 0x100000) return false;
    const auto* nt = (const IMAGE_NT_HEADERS64*)(g_exeBase + dos->e_lfanew);
    if (!ReadablePtr(nt, sizeof(*nt)) || nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        nt->FileHeader.TimeDateStamp != 0x6A3EB6ADu ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;
    static const BYTE scale[] = {0x00, 0x00, 0x7A, 0x44};
    static const BYTE format[] = {0x25,0,0x2E,0,0x32,0,0x66,0,0x6C,0,0,0};
    return ReadablePtr(g_exeBase + 0x90B07C, sizeof(scale)) &&
        ReadablePtr(g_exeBase + 0x8F3E98, sizeof(format)) &&
        memcmp(g_exeBase + 0x90B07C, scale, sizeof(scale)) == 0 &&
        memcmp(g_exeBase + 0x8F3E98, format, sizeof(format)) == 0;
}
// Additional read-only schedule gate. A mismatch disables ONLY schedule reads;
// it never installs another hook or prevents the unchanged fuel observer.
struct ScheduleFingerprint { uintptr_t rva; unsigned size; BYTE bytes[40]; };
static const ScheduleFingerprint g_scheduleFingerprints[] = {
        {0x7D5550, 7, {0x4C,0x8B,0xBA,0x40,0x02,0x00,0x00}},
        {0x7E356E, 4, {0x4C,0x8B,0x7D,0x40}},
        {0x7E6904, 25, {0x49,0x8B,0x8F,0x88,0x06,0x00,0x00,0x49,0x2B,0x8F,0x80,0x06,0x00,0x00,0x48,0xC1,0xF9,0x03,0x41,0x38,0xBF,0xFD,0x05,0x00,0x00}},
        {0x7E693C, 7, {0x49,0x39,0xB7,0xA0,0x04,0x00,0x00}},
        {0x7E6BDB, 8, {0x49,0x83,0xBF,0x50,0x06,0x00,0x00,0x00}},
        {0x7E815D, 32, {0x49,0x8B,0x87,0x80,0x06,0x00,0x00,0x48,0x8B,0x4D,0x38,0x4C,0x8B,0x34,0xC8,0x48,0x69,0xF2,0x98,0x01,0x00,0x00,0x49,0x03,0xB7,0x38,0x06,0x00,0x00,0x4D,0x85,0xF6}},
        {0x7E81B3, 19, {0x41,0x80,0xBF,0xFD,0x05,0x00,0x00,0x00,0x74,0x06,0x4C,0x8B,0xF1,0x48,0x8B,0xF1,0x40,0x32,0xFF}},
        {0x7E8438, 7, {0x41,0x39,0x87,0x98,0x06,0x00,0x00}},
        {0x7E8CC6, 7, {0x4D,0x3B,0xB7,0x10,0x06,0x00,0x00}},
        {0x7E8EB9, 7, {0x0F,0xB6,0x86,0xC8,0x00,0x00,0x00}},
        {0x7E8ED2, 3, {0x80,0x3E,0x00}},
        {0x7E8F07, 8, {0xF3,0x0F,0x10,0x86,0xCC,0x00,0x00,0x00}},
        {0x7E9273, 5, {0xF3,0x0F,0x10,0x46,0x04}},
};
static bool ScheduleTargetMatches(void)
{
    for (const auto& f : g_scheduleFingerprints)
        if (f.rva > g_exeSize || f.size > g_exeSize - f.rva ||
            !ReadablePtr(g_exeBase + f.rva, f.size) ||
            memcmp(g_exeBase + f.rva, f.bytes, f.size)) return false;
    return true;
}
static void PanelObserverStart(void)
{
    if (!g_panelObserverEnabled) { Logf("[PANEL FUEL] disabled by INI"); return; }
    if (g_billing || !g_simClock || !g_simClock->registerTick || !g_persistenceHandle) {
        Logf("[PANEL FUEL] unavailable: requires billing=0, sim_clock tick and registered load signal"); return;
    }
    if (!PanelTargetMatches()) { Logf("[PANEL FUEL] target/data fingerprint mismatch; no hook installed"); return; }
    // 20 complete bytes through LEA RBP,[RSP-2EB0]; no RIP-relative operand.
    static const BYTE expect[] = {0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,
        0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    if (!ReadablePtr(g_exeBase + 0x7D5460, sizeof(expect)) ||
        memcmp(g_exeBase + 0x7D5460, expect, sizeof(expect))) {
        Logf("[PANEL FUEL] renderer prologue mismatch; no hook installed"); return;
    }
    g_scheduleReady = g_scheduleObserverEnabled && ScheduleTargetMatches();
    Logf("[PANEL SCHEDULE] enabled=%d fingerprint=%s mode=read_only_selected_fields "
         "max_rows=16 last_flag5fd_row_config=not_read job_lifecycle=unverified billing=off",
         g_scheduleObserverEnabled ? 1 : 0, g_scheduleReady ? "matched" : "disabled_or_mismatch");
    InitializeCriticalSection(&g_panelQueueLock);
    if (!g_simClock->registerTick(PanelObserverTick, nullptr)) {
        Logf("[PANEL FUEL] tick registration failed; no hook installed"); return;
    }
    if (!InstallInlineHook(g_exeBase + 0x7D5460, (void*)PanelRendererDetour,
                          &g_panelTrampoline, expect, sizeof(expect), "private_logistics panel fuel observer")) {
        Logf("[PANEL FUEL] renderer hook refused; capture unavailable"); return;
    }
    InterlockedExchange(&g_panelStarted, 1);
    Logf("[PANEL FUEL] installed renderer=0x7D5460 stolen=20 scale=1000 format=%%.2fl "
         "interval_ms=1000 cap=240 queue=16 paused_coalescing=%s; logs drain on unpaused ticks; "
         "candidate_home fleet comparison only; accepted sample IDs do not prove coverage; "
         "no exact burn, tracker or billing", g_simClock->isPaused ? "keep_latest_same_window_root" : "unavailable");
}
#endif

````

### Final source: `plugins/private_logistics/private_logistics.cpp`

SHA-256: `9b70a80a7c8745ee329c6728703e0641267672eaccdac7c67ad912b627400684`

````cpp
// private_logistics - what a private company earns, and what it pays.
//
// ---------------------------------------------------------------------------
// v0.18.8: fingerprint-gated read-only schedule observations; job lifecycle unverified.
// Historical fuel claims below are superseded: sampled changes are NOT burn,
// native fuel units/events remain unverified and the journey adapter is unwired.
//
// v0.18.6 updates: bounded, read-only panel-object/fleet observer.
// Verified 7D5460 prologue, scale and format fingerprints; one sample/second,
// at most 240 per load epoch. No exact burn/tracker/billing integration.
//
// v0.18-permissions: independent state-site directions; consumer sites never
// supply goods; state default deny applies to existing/manual targets too.
// Missing eligible connections are recreated by the company. No fuel/billing
// policy changes in this version. Native direction semantics need game testing.
//
// Historical v0.17 updates (state-storage source opt-in):
//   - Register a storage-panel checkbox through ui_panels' widget API; each
//     public type-5 storage defaults OFF and its permission is persisted by a
//     stable type/name/position key, never by retaining a Building pointer.
//   - When opted in, append a native DO target with load enabled and unload
//     enable/percentage cleared. Turning the checkbox off disables both task
//     directions on that state-storage target; unrelated DO targets are untouched.
//   - The storage builder is hooked by the verified dispatcher call site in
//     ui_panels. Customs remains diagnostic-only; no inventory is manipulated.
// ---------------------------------------------------------------------------
// v0.16 updates (coordinate probe correction):
//   - v0.15 logged a state storage position of (0,0) and customs distance 0.0m.
//     The position helper had read bld+0x08/+0x10 instead of the C3D_NODE
//     translation at bld+0x320+0x30/+0x38 (with placement-bbox fallback).
//     Stable identity keys and nearest-customs diagnostics now use the actual
//     position helper. No customs/state-storage target writes were added.
//
// v0.15 updates (state-storage/customs discovery probe; no public-target writes):
//   - Walk the game-wide building vector to count state storage buildings and
//     report each private road/rail DO's geometrically nearest customs house.
//     This is diagnostic only; neither is appended as a target in this version.
//   - Use game_world.h's canonical demolished flag rather than an unverified
//     raw offset in private-building discovery.
//
// v0.14 updates (automatic private DO targets; staged and disabled by default):
//   1. Once per in-game day, discover registered private DOs (road 43 / rail 52)
//      and private buildings with native storage, using enterprise_base.
//   2. Preserve existing native target entries. Missing eligible targets are
//      reported in dry-run mode by default; optional write mode appends a
//      TaskEntry and mirrors the native add-path TaskCfg defaults (wildcard,
//      100% thresholds, active/validated flags). The owner reports separate
//      load/unload checkboxes in the DO panel; their backing fields and dataflow
//      still need verification before assigning state storage as load-only.
//   3. Generated entries are enabled, deduplicated, and trigger the verified DO
//      dirty flag. Existing entries/configs are never rewritten or removed.
//   4. A session-local identity ledger tracks generated targets by stable
//      keys. Missing eligible targets are recreated; manual removal is not a veto.
//   5. This is a daily callback, not a per-frame hook. Save/load and manual-removal
//      UI behavior remain runtime validation items; write mode ships off.
// ---------------------------------------------------------------------------
// Level 3. Consumes enterprise_base (v8), currency (v4) and sim_clock.
// Optionally consumes resource_pricing for the unit price.
//
// ---------------------------------------------------------------------------
// v0.10 - THE TANK WAS FOUND, AND THE TARIFF WAS FOUR TIMES TOO CHEAP.
//
// veh+0x5F0 is fuel in tonnes. v0.9's field scanner found it by watching which
// words move, and the v0.9 run confirmed it five ways: it falls while the truck
// works, it runs to zero and jumps at a refill, it tops out at 0.46 (the
// player's own 456 LITRE tank at 1 kg/L), the refills reconcile to the diesel
// the company paid for, and nothing else in the vehicle moves that way.
//
// With the tank readable, the burn stops being a guess:
//
//     2.05 t over 51 days, two active vehicles, 139 deliveries
//     = 0.01475 t per delivery = 14.75 L at 1 kg/L
//
// v0.9 shipped 3.30 L/trip, derived from PURCHASES over trips. The company was
// being paid 1.06 per delivery against a real fuel cost of 1.96 - it lost money
// on every trip and the tariff, not the economy, was doing it.
//
// v0.10 reads every vehicle's tank at every close and reports
//
//     [TANK] day N   v=... fuel 0.4300 t | burned 0.0220 t | refills 0
//     [TANK] run: burned 2.0500 t = 2050.0 L over 139 delivery(ies) = 14.75 L/trip
//
// Refills are counted, never subtracted from burn. This is the first figure in
// the whole feature that is neither a purchase nor a model.
//
// ALSO: diesel_density 0.00084 -> 0.00100. The game stores 1 kg per litre; the
// old constant was real-world diesel and made every litre 16 % too cheap.
//
// ---------------------------------------------------------------------------
// v0.9 - FIND THE TANK.
//
// The player asked the right question: how do we know how much fuel a trip
// burnt? We do not. Two runs measured it by dividing the company's diesel
// PURCHASES by its trips and got 3.27 L/trip over 1,100 trips and 7.14 L/trip
// over 214 trips - a factor of two, because fuel sits in tanks between
// purchases. Purchases are not burn, and no amount of averaging makes them so.
//
// What is needed is the fuel going INTO a vehicle, or the level inside it. The
// vehicle record is reachable (bld+0xC70, proven) and the transfer hook never
// shows a fuel movement, so the tank is a FIELD inside the vehicle, not an
// event we can already see.
//
// This build finds it empirically instead of guessing:
//   - the first sighting of a vehicle is stored as a baseline and not logged;
//   - every day after, the same 512 dwords at veh+0x000..0x800 are compared,
//     and ONLY the words that changed are logged, as offset, old, new.
//
// A tank shows up as a word that falls while the truck drives and jumps when
// it refuels. Position, speed and timers will show up too - the offsets tell
// them apart. Nothing is priced from this; it is how the next tariff stops
// being a fleet average.
//
// ALSO IN v0.9:
//   - the [FUEL] run line now says PURCHASES and not burn, because that is
//     what it measures.
//   - the ini learned its section header back. v0.7's rewrite dropped
//     [private_logistics], so the host read nothing and every key fell back to
//     its compiled default - which is why the last two runs were dry runs.
//
// ---------------------------------------------------------------------------
// v0.8 - SEE THE VEHICLES BEFORE PRICING THEM.
//
// v0.7's trip tariff charged every delivery the same litres. That is one
// arbitrary vehicle imposed on a whole fleet, and it cannot survive: vehicles
// differ in capacity, speed, price and consumption, and a tariff that ignores
// it will overpay a scooter and bankrupt a heavy hauler. H41.
//
// The v0.7 run shows the transfer hook cannot supply the difference: 3,065
// site lines resolve to exactly TWO vehicle-side pointers, one of them NULL.
// The fleet has to be read where the game keeps it - bld+0xC70/+0xC78, proven
// by three independent readers (Part 12) - and each vehicle's own record read
// from there:
//
//     veh+0x4F8  Building*  home building                      proven
//     veh+0x3B8  vec        cargo, stride 0x10 (Res*, float t)  proven
//     veh+0x510  ?          candidate catalogue entry           UNPROVEN
//
// This build adds the census: once a day, every private enterprise's fleet is
// walked, each vehicle is logged with what it carries and what it can carry at
// peak, and the first sighting of each vehicle dumps 32 bytes at the pointer
// the game gives for its catalogue entry. That is the lead for capacity,
// speed, price and consumption - the four numbers the final model needs and
// the four this build refuses to invent.
//
// The tariff is unchanged from v0.7. It stays a fallback until the census
// says what the fleet actually is.
//
// ALSO IN v0.8:
//   - litres_per_trip 5.00 -> 3.30. The v0.7 meter measured the DO's diesel
//     bill against its own deliveries: 406.40 RUB over 1,100 trips =
//     3.267 L/trip at ~128/RUB. Not a guess, and not 8.63 either - that
//     figure counted tank fill, exactly as the player said it would.
//   - the [FUEL] run line no longer claims "model and bill agree" when the
//     bill is zero and there is nothing to compare.
//
// ---------------------------------------------------------------------------
// v0.6 - THE TRIP IS PAID FOR BY THE TRIP, AND THE SHARE IS ONLY A SHARE.
//
// Session 7 ran v0.5 with fuel switched on and produced the number the whole
// feature needed: the private DO's account reconciles exactly to
//
//     10,000.00 + 738.99 freight - 445.37 fuel - 0.02 = 10,293.60
//
// so the game DOES charge a private distribution office for diesel, and it
// arrives as a cat=2 (FOREIGN) debit on its own company capital. H37 closed.
// H33 closed too: 444 sampled payments, all moving the treasury to the kopeck.
//
// The same log indicted the payment model. Per delivery, against a measured
// fuel cost of 0.34 RUB:
//
//     plants     282 deliveries   0.29 each   0.9x the fuel bill
//     food       127 deliveries   2.13        6.3x
//     meat        16 deliveries   2.37        7.0x
//     fabric      19 deliveries   3.80       11.2x
//     chemicals   45 deliveries   6.13       18.0x
//
// 58 % of the DO's work was selling at a LOSS and 9 % was paying for it. A
// percentage of cargo value is not a tariff, it is a lottery. Real freight is
// cost recovery plus a margin; only the handling of valuable cargo is priced
// ad valorem, and it is a small part of the bill.
//
// v0.6 therefore models the tariff as what it has to be:
//
//     tariff = fuel_cost x (1 + margin) + tariff_share x cargo_value
//
// Cost recovery pays the trip, the share pays for the cargo being worth
// carrying. litre counts come from session 7 (see litres_per_trip in the ini).
//
// Still probe only. Nothing here is charged. The 8 % share keeps running so
// the two can be read side by side for one more session.
//
// Also fixed: the index and the treasury were read at plugin start, before the
// world is loaded, so every log opened with "fuel NOT FOUND" and "treasury
// opens at 0.00" that were both false a second later. Both are now re-read and
// printed at every day close.
//
// ---------------------------------------------------------------------------
// v0.5 - MEASURE THE COST BEFORE PRICING IT.
//
// v0.4 moved money and the money arrived (RE_DIGEST Part 19). What the same
// session also showed is that the private DO has NO COST SIDE: its company
// account took 646 credits over seven days and paid out nothing at all. Fuel,
// upkeep and vehicles never touched its books. So the 8 % freight is not a
// tariff, it is a subsidy, and "is 8 % enough to cover fuel" cannot be asked
// until a cost exists to compare it against.
//
// This build therefore charges nothing new and measures two things:
//
//   1. THE TARIFF MODEL. A price built the way real freight is built - cost
//      recovery plus a margin - and indexed to a live price, never to a
//      rouble constant. A constant is exactly what inflation eats:
//
//          litres  = litres_per_trip + litres_per_tonne x tonnes
//          cost    = litres x diesel_density x fuel_price      (live)
//          tariff  = cost x (1 + margin)
//
//      If no fuel price resolves (fuel_resource unknown, resource_pricing
//      absent), the same cost is expressed in daily survival baskets via the
//      CPI tail, so the fallback is indexed too. If neither resolves the
//      probe reports "unindexed" and prices nothing - it never guesses a
//      figure in currency units.
//
//   2. THE MONEY PATH, both sides of it. [BAL] prints the treasury and every
//      carrier's company capital at each day close. v0.4 proved the company
//      side to the kopeck but never read the payer (H33); the delta between
//      two closes is that proof, and the difference between a carrier's
//      capital change and the freight it was paid is its fuel bill (H37).
//
// Nothing here charges the probe. `tariff_probe = 1` logs, it does not pay.
// The 8 % share keeps running exactly as v0.4 left it, so a session produces
// three numbers side by side: modelled tariff, freight actually paid, and the
// vehicle's real fuel consumption as the game charged it.
//
// v0.4 also carried a stale banner - it opened every log with "v0.3" - which
// cost the last reader a minute. Version now comes from PL_VERSION once.
//
// ---------------------------------------------------------------------------
// v0.4 - FREIGHT IS A SHARE OF CARGO VALUE, AND THE MONEY MOVES.
//
// v0.3 paid a flat per-tonne rate (1.50 RUB/t). Measured against the live
// session that is 0.31 % of what the cargo is worth - a private distribution
// office could not cover a truck, and FreightRateFor() ignored the resource
// anyway, so the liquid and goods rates were dead keys.
//
// v0.4 replaces the rate with freight_share, a fraction of the cargo's own
// value:
//
//     freight = tons x unitPrice x freight_share          (default 0.08)
//
// A typical load is ~0.12 t of fabric worth ~58 RUB, so this pays ~4.70 RUB
// instead of ~0.18. It scales with what is actually moved, it needs no
// commodity-class table, and when Ship 2 makes the BUYER pay instead of the
// state, the same formula becomes the wholesaler's margin with no rewrite.
//
// v0.4 also:
//   - refuses a payment the treasury cannot afford (getTreasuryBalance),
//     because moveTreasury takes a signed amount and will happily go negative;
//   - splits `skipped` into the four things it was conflating;
//   - labels the daily line with the day it actually covers (it was one
//     behind - the callback fires at the START of the day).
//
// ---------------------------------------------------------------------------
// v0.3 - THE FIRST MONEY.
//
// `dry_run = 1` by default: this build COMPUTES every payment and logs it as
// "[PAY] would", but moves nothing. Set dry_run = 0 to actually pay.
// Nothing else in the plugin changes when you flip it.
//
// The money model (PLAN_TECHNICAL.md sections 6 and 7):
//
//   H1 - A BUILDING IS PAID FOR GOODS THAT LEAVE IT
//        at a mode-0 call with tons < 0, param_2 is the source Building.
//        If that building is a private company, it is credited
//        |tons| x price x factory_share, and the treasury pays.
//
//   H2 - A CARRIER IS PAID FREIGHT FOR A DELIVERY
//        at a mode-0 call with tons > 0, param_2 is the destination Building
//        and param_9 is the fleet owner. If the owner is a private company
//        and is not the destination, it is credited
//        tons x price x freight_share.
//
//   Nothing is billed at a mode-1 site: those are the vehicle side of the
//   same movement and would double-charge it.
//
// ---------------------------------------------------------------------------
// What the two live sessions established (RE_DIGEST Parts 10, 12 and 13)
// ---------------------------------------------------------------------------
//
//   - THE COMMIT TEST IS `(unsigned char)r == 1`. The transfer returns a bool
//     in AL; the upper 24 bits of EAX are register garbage. v0.1 tested the
//     whole int and discarded exactly half the committed calls.
//
//   - `param_2` IS A Building* AT MODE 0 AND A Vehicle* AT MODE 1. That is
//     the whole of attribution. At mode 0 the value also appears in
//     enterprise_base's "[CAP] ... by=wholesale" lines, which only ever take
//     a building; vehicle offsets read off it return garbage.
//
//   - `param_9` IS THE FLEET OWNER, not an endpoint: constant across two
//     different destination buildings, and equal to the mode-1 vehicle's
//     `home` field in every call.
//
//   - `(veh+0x4F0)` IS NOT AN ENDPOINT. It was NULL in all 342 calls v0.1
//     measured and all 373 of v0.2's, because at mode 0 we were reading
//     vehicle fields off a building. Nothing here uses it.
//
//   - A MOVEMENT IS TWO CALLS 30-70 ms APART, same resource, same tonnage:
//       0x694F40  mode 0  -tons   the BUILDING the goods leave
//       0x6953C4  mode 1  +tons   the VEHICLE they enter
//       0x68F792  mode 0  +tons   the BUILDING they enter
//       0x68F7F9  mode 1  -tons   the VEHICLE they leave
//     The load leg's mode-0 call has param_9 == NULL, so its carrier is
//     resolved from the paired mode-1 call through a short time ring.
//
// ---------------------------------------------------------------------------
// The hook is a call-site patch, NOT an inline hook
// ---------------------------------------------------------------------------
//
// FUN_140420350 has a five-byte prologue, so the loader's installInlineHook
// (which requires >= 14 position-independent bytes) cannot take it. All
// sixteen invocations are direct `call rel32` with no vtable, thunk or
// indirect path, so retargeting the call sites sees every invocation.
//
//   site before:  E8 <rel32>          call FUN_140420350
//   site after:   E8 <rel32>          call cave          <- SAME opcode
//   cave:         FF 25 00 00 00 00   jmp qword ptr [rip+0]
//                 <8-byte detour address>
//
// The opcode deliberately stays E8. A jmp does not push a return address, so
// a detour entered that way both reads garbage from _ReturnAddress() and
// executes `ret` into the enclosing function's caller. Verified on x86-64.
//
// Safety properties:
//   - Nothing is written unless the site verifies: byte 0 must be E8 AND its
//     rel32 must resolve to exeBase+0x420350.
//   - Every game-memory read is behind ReadablePtr.
//   - The ledger is a fixed ring; nothing allocates in the detour.
//   - The lock covers counters and the rings only. Game-memory reads, calls
//     into other plugins and logging all happen outside it.
//   - A payment needs BOTH services. If enterprise_base or currency is
//     missing, billing is skipped entirely and says so once.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "site_permissions.h"
#include "task_audit.h"
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"
#include "fuel_diagnostics.h"

#include <stdint.h>
#include <math.h>
#include <new>

// ---------------------------------------------------------------------------
// the transfer being instrumented
// ---------------------------------------------------------------------------

#define TSM_RVA_TRANSFER 0x420350

typedef int (*tTransfer)(void* game, void* veh, unsigned mode, void* res,
                         float tons, float* outMoved, float cap,
                         unsigned char flag, void* bld);

static tTransfer g_realTransfer = nullptr;

// Vehicle fields. See PLAN_TECHNICAL.md section 3.1.
#define TSM_VEH_HOME    0x4F8   // Building* - the owning depot (mode 1 only)

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static int   g_enabled         = 1;
static int   g_billing         = 0; // Permissions-test default: no freight payments.
static int   g_dryRun          = 1;      // SAFE DEFAULT: compute, log, do not pay
static int   g_payFactory      = 1;
static int   g_payCarrier      = 1;
static int   g_logCapPerDay    = 400;
static int   g_ledgerCapacity  = 512;
static int   g_siteLog         = 1;
// v0.4: ONE number, a share of the cargo's value. The three per-tonne rates
// are gone: the class table never existed (FreightRateFor ignored its
// argument) and a player cannot reason about "RUB per tonne of coal".
static float g_freightShareRetired = 0.0f; // set when the ini still carries it
static float g_freightShare    = 0.08f;  // v0.7: RETIRED. Kept only so the
                                         // ini key can be recognised and warned about.
static float g_factoryShare    = 0.90f;  // w - what a buyer pays a factory

// v0.7: THE TRIP TARIFF REPLACES THE VALUE SHARE.
//
// v0.6 measured the value share against the DO's real fuel bill and lost:
// 14,038.20 RUB of freight against 511.92 of diesel over 82 days. The share
// is 27x the cost it was supposed to cover, and it is value-blind - a truck
// hauling plants was paid 1.46 for the trip, one hauling chemicals 71.12.
//
// v0.7 charges the model that was only probed in v0.6; v0.6's behaviour is
// one key away (`trip_tariff = 0`), so both can be compared in one session.
static int    g_tripTariff      = 1;
static double gWearLitres       = 5.0;   // maintenance reserve, in litres of diesel
static int    g_litresAuto      = 0;     // re-tune litres_per_trip from the fuel bill

// v0.7: the measured fuel bill. This is the only figure in the plugin that
// comes from the game instead of from arithmetic - the [TARIFF] model is an
// assumption, the DO's monthly foreign-cost total is the bill.
//
// Fuel is bought in bulk and stored, so a day's purchases are NOT a day's
// burn. Run averages also do not establish exact native journey burn.
// v0.8: the vehicle census. A republic has tens of vehicles, not thousands;
// 64 slots is four times the largest fleet seen so far and keeps the table in
// static storage, as the render/tick rules require.
#define VEH_MAX 64
static void*    g_vehPtr[VEH_MAX];
static double   g_vehPeakTons[VEH_MAX];
static unsigned g_vehSamples[VEH_MAX];
static int      g_vehCount   = 0;
static int      g_censusLog  = 1;
// v0.9: the field scanner. 512 dwords (0x800 bytes) per vehicle, compared once
// a day; only changes are logged. Static storage, no allocation.
#define VEH_SCAN_WORDS 512
static unsigned g_vehSnap[VEH_MAX][VEH_SCAN_WORDS];
static bool     g_vehSnapValid[VEH_MAX];
// Legacy candidate sampling. Physical units, native deduction/refill writers
// and activity gates are NOT verified for the selected road truck. Variable
// names retain old units for compatibility; logs explicitly say raw/estimate.
static double   g_vehFuel[VEH_MAX];          // last valid raw candidate
static bool     g_vehFuelValid[VEH_MAX];
static double   g_burnTonsRun   = 0.0;       // sum of candidate sample decreases
static unsigned g_refillsRun    = 0;
// Historical denominators retained for candidate diagnostics, not physics.
static unsigned g_tankDaysRun    = 0;        // closes with at least one tank read
static unsigned g_tankVehDaysRun = 0;        // (vehicle, close) pairs
static int      g_cargoDump      = 1;        // [CARGO] - settle where fuel lives
static int      g_tankCrossLog   = 40;       // v0.13: [TANK] slot-vs-mirror lines
static private_logistics_fuel_diagnostics::Samples g_vehFuelSamples[VEH_MAX];
static volatile LONG g_fuelDiagnosticEpoch = 1;
static unsigned char g_vehCapValid[VEH_MAX];
static double        g_vehCap[VEH_MAX];      // metadata capacity candidate, raw

// Legacy state-word guesses only; never gate native events using these.
static unsigned g_activeVehDaysRun = 0;
static unsigned g_idleVehDaysRun   = 0;

// Legacy fleet endpoint sampling. It cannot prove individual fuel events.
static const int VEH_FUEL_OFF = 0x5F0;     // panel backing candidate; physical units unverified

static void*    g_probeBld      = nullptr;
static double   g_probeFuelTons = -1.0;
static int      g_probeVehCount = -1;
static double   g_probeBurnRun  = 0.0;
static unsigned g_probeStepsRun = 0;
static unsigned g_probeSkipRun  = 0;
static double   FleetFuelTons(void* b, int* outCount);
static void     BurnProbe(void* b);
static int      g_vehScan          = 1;
static int      g_vehScanLogPerDay = 40;
static void     CensusFleets(int closedDay);   // v0.8, defined with the census
static void     ScanVehicle(int slot, void* v, int closedDay);   // v0.9
static void     ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills);   // v0.10

static double             g_fuelPaidRun  = 0.0;
static double             g_fuelLitresRun = 0.0;
static unsigned long long g_fuelTripsRun = 0;

// Earlier slot/clamp/activity attribution is superseded by the real-game review.
// v0.5: the cost side, in physical units. No rouble constant may appear here:
// prices move, litres and ratios do not.
static int    g_tariffProbe     = 1;       // compute + log the tariff, never charge
static int    g_balanceLog      = 1;       // [BAL] treasury + carrier capital per day
static int    g_tariffLogPerDay = 60;      // per-delivery [TARIFF] lines, then totals only
static double gLitresPerTrip    = 14.75;   // historical model parameter, NOT verified journey burn
// Retained legacy model knob; actual cargo-load dependence is unverified.
static double gLitresPerTonne   = 0.0;     // legacy model default; not native physics
static double gTariffMargin     = 0.35;    // dimensionless: 35 % over fuel cost
static double gDieselDensity    = 0.00100; // legacy model t/L, NOT proof of native mass units
static char   gFuelResource[64] = "fuel";  // resolved through resource_pricing
static double gBasketsPerTrip   = 0.80;    // CPI fallback, in daily survival baskets
static double gBasketsPerTonne  = 0.02;

#define PL_VERSION "0.18.8-schedule-observations"
static int g_panelObserverEnabled = 1;
static int g_scheduleObserverEnabled = 1;
static void PanelObserverLoading(void);
static float g_minPayment      = 0.01f;
static double g_dailyPayCap    = 0.0;    // 0 = no cap

static const TsmEnterpriseBaseApi*  g_enterprise = nullptr;
static const TsmSimClockApi*        g_simClock   = nullptr;
static const TsmCurrencyApi*        g_currency   = nullptr;
static const TsmResourcePricingApi* g_pricing    = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persistenceHandle = 0;
static const TsmUiPanelsApi*         g_uiPanels = nullptr;
static int                           g_stateStorageWidgetHandle = 0;

// Automatic DO-target generation. Fixed buffers only; discovery runs from the
// once-per-day sim_clock callback, never from a per-frame/render hook.
static int g_autoTargetsEnabled = 1;
static int g_autoTargetsDryRun  = 0;
static int g_autoSourcePercent = 10;
static int g_autoDestinationPercent = 30;
static int g_autoTargetLogLimit = 80;
static volatile LONG g_autoResetActiveMemoAfterLoad = 0;
#define AUTO_ENTITY_MAX 8704
#define AUTO_MEMO_MAX   262144
#define AUTO_MEMO_HASH_SIZE 524288
#define AUTO_POINTER_HASH_SIZE 32768
#define AUTO_OFFICE_HASH_SIZE 16384
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char isStateStorage; unsigned char statePermission; unsigned char targetPresent; };
struct AutoMemo { uint64_t officeKey; uint64_t targetKey; unsigned nextOffice; unsigned char state; };
struct AutoPointerSlot { void* building; unsigned entityIndex; };
struct AutoOfficeMemoHead { uint64_t officeKey; unsigned headPlusOne; };
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static AutoMemo   g_autoMemo[AUTO_MEMO_MAX];
static unsigned   g_autoMemoHash[AUTO_MEMO_HASH_SIZE]; // index + 1; zero means empty
static AutoPointerSlot g_autoPointerHash[AUTO_POINTER_HASH_SIZE];
static AutoOfficeMemoHead g_autoOfficeMemoHeads[AUTO_OFFICE_HASH_SIZE];
static unsigned   g_autoMemoCount = 0;
static unsigned   g_autoLogLines = 0;
static int        g_autoLastDay = -1;
static void AutoTargetsOnDay(int day, int closedDay);
static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void* userData);

// ---------------------------------------------------------------------------
// the sixteen call sites
// ---------------------------------------------------------------------------
//
// `known` records what the RE rounds established. The instrument does not act
// on the label - every site is logged the same way - but billing only ever
// fires at a MODE 0 site, and `known` is what makes the log readable.
//
// After the first two sessions the FUN_14068B330 family is no longer
// described as "sea/rail": it carries ordinary factory traffic.

enum SiteClass {
    SITE_UNKNOWN = 0,
    SITE_REFUEL,
    SITE_LOAD,          // draws goods out of a building
    SITE_DELIVERY,      // puts goods into a building
    SITE_TRANSSHIP      // intra-vehicle / return leg
};

struct SiteSpec {
    unsigned    rva;
    SiteClass   known;
    const char* note;
};

#define MAX_SITES 16

static SiteSpec g_sites[MAX_SITES] = {
    { 0x00141F33, SITE_UNKNOWN,   "never observed" },
    { 0x0016F831, SITE_REFUEL,    "refuel call - telemetry, never billed; never observed" },
    { 0x001B2898, SITE_UNKNOWN,   "CONTESTED: r8-9 'road delivery', r10 'road refuel'; never observed" },
    { 0x001B484B, SITE_UNKNOWN,   "never observed" },
    { 0x001B5EC0, SITE_UNKNOWN,   "never observed" },
    { 0x001B653B, SITE_UNKNOWN,   "never observed" },
    { 0x001B6833, SITE_UNKNOWN,   "never observed" },
    { 0x001B75ED, SITE_UNKNOWN,   "never observed" },
    { 0x001B7981, SITE_UNKNOWN,   "never observed" },
    { 0x001CB7FF, SITE_LOAD,      "loader: mode 0, +tons, then bld+0x970 storage-add; never observed" },
    { 0x0068EECA, SITE_LOAD,      "family A: mode 0, +tons, cargo-add follows; never observed" },
    { 0x0068F792, SITE_DELIVERY,  "family A DELIVERY: mode 0, +tons, param_2 = building, param_9 = fleet owner" },
    { 0x0068F7F9, SITE_TRANSSHIP, "family A delivery, vehicle side: mode 1, -tons. NEVER BILLED" },
    { 0x006945B3, SITE_TRANSSHIP, "family A: mode 0, -tons; never observed" },
    { 0x00694F40, SITE_LOAD,      "family A LOAD: mode 0, -tons, param_2 = the building the goods leave" },
    { 0x006953C4, SITE_TRANSSHIP, "family A load, vehicle side: mode 1, +tons. NEVER BILLED" }
};

static bool g_sitePatched[MAX_SITES] = { false };
static bool g_siteWanted[MAX_SITES]  = { false };   // filled in by ReadSettings
static bool g_siteFilterActive       = false;       // false = every site

static int  g_siteCount = MAX_SITES;

// ---------------------------------------------------------------------------
// the ledger
// ---------------------------------------------------------------------------

enum ShipKind { SHIP_LOAD = 0, SHIP_DELIVERY, SHIP_FREIGHT, SHIP_FUEL, SHIP_PURCHASE };

struct Shipment {
    unsigned id;
    unsigned seq;
    void*    building;
    void*    counterparty;
    void*    vehicle;
    void*    resource;
    float    tons;
    double   unitPrice;
    double   value;
    unsigned short site;
    unsigned char  kind;
    unsigned char  mode;
    double   t;
};

#define LEDGER_MAX 2048
static Shipment g_ledger[LEDGER_MAX];
static int      g_ledgerHead  = 0;
static unsigned g_ledgerSeq   = 0;
static unsigned g_ledgerTotal = 0;

// ---------------------------------------------------------------------------
// the carrier ring
// ---------------------------------------------------------------------------
//
// The load leg's mode-0 call does not carry the carrier: param_9 is NULL
// there (91 of 91 calls). The vehicle is named 30-70 ms later by the paired
// mode-1 call. This ring is how the two are joined.
//
// A match needs the same resource, the same tonnage within 0.5 %, and an age
// under 2 s. If nothing matches, the payment is skipped and counted - never
// guessed.

#define CARRIER_RING 32

struct CarrierNote {
    void*  res;
    float  tons;      // absolute
    void*  home;      // the fleet owner
    double t;
};

static CarrierNote g_carrier[CARRIER_RING];
static int         g_carrierHead = 0;

static double NowSeconds(void)
{
    // GetTickCount64 is monotonic and needs no CRT.
    return (double)GetTickCount64() / 1000.0;
}

// ---------------------------------------------------------------------------
// counters
// ---------------------------------------------------------------------------

static unsigned g_calls            = 0;
static unsigned g_callsCommitted   = 0;
static unsigned g_callsNoop        = 0;
static unsigned g_unknownBuilding  = 0;
static unsigned g_staging          = 0;
static unsigned g_nonPrivate       = 0;
static unsigned g_siteLoggedToday  = 0;
static unsigned g_attrLoggedToday  = 0;
static unsigned g_payLoggedToday   = 0;
static unsigned g_siteSuppressed   = 0;
static unsigned g_attrSuppressed   = 0;
static unsigned g_paySuppressed    = 0;

static unsigned g_payCount         = 0;
// v0.4: `skipped` used to be one number covering four unrelated causes. In
// the fourth session it read 159 in a day purely because the distribution
// office was state-owned - alarming to read, and meaningless.
static unsigned g_skipNoPayee      = 0;   // no private owner on the leg
static unsigned g_skipNoPrice      = 0;   // resource_pricing gave no price
static unsigned g_skipSmall        = 0;   // below min_payment
static unsigned g_skipTreasury     = 0;   // the state could not afford it

// v0.5: the tariff probe's own tallies, and which index it managed to use.
// An unindexed delivery is NOT a delivery worth 0.00 - it is one this build
// could not price, and the daily line says so rather than folding it in.
static unsigned g_tariffCount      = 0;
static unsigned g_tariffNoIndex    = 0;
static unsigned g_tariffFuelIndex  = 0;
static unsigned g_tariffCpiIndex   = 0;
static double   g_tariffCostSum    = 0.0; // the fuel-recovery half
static double   g_tariffShareSum   = 0.0; // the ad valorem half
static unsigned g_tariffLoggedToday= 0;
static double   g_tariffSum        = 0.0;

// v0.5 (H33): the payer leg, sampled per payment. A daily treasury delta is
// useless as proof - wages, taxes and store sales move the same balance all
// day long. Sampling either side of OUR OWN move isolates it completely: the
// difference must be exactly minus the amount, or the books do not close.
// v0.5: the refuel site (0x16F831) has never fired in any session. If the
// game is going to start charging for fuel, this is where we see it first.
static unsigned g_fuelEvents       = 0;
static unsigned g_treasuryChecked  = 0;
static unsigned g_treasuryExact    = 0;
static unsigned g_treasuryOff      = 0;
static double   g_treasuryMaxGap   = 0.0;

// v0.5: the money path, both ends. The treasury is sampled at each close; the
// carriers we paid are remembered so their capital can be printed beside it.
#define BAL_CARRIERS 4
static double g_treasuryAtClose    = 0.0;
static bool   g_treasuryHaveClose  = false;
static void*  g_balCarrier[BAL_CARRIERS]     = { nullptr, nullptr, nullptr, nullptr };
static double g_balCarrierPrev[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 };
static double g_balCarrierIn[BAL_CARRIERS]   = { 0.0, 0.0, 0.0, 0.0 };
static double g_balCarrierFuel[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 }; // v0.7 meter,
                                                                    // -1 = no baseline yet
static int    g_balCarrierCount    = 0;
static void*  g_fuelRes            = nullptr;   // cached lookup, re-resolved daily

// v0.4: which day the accumulated tallies belong to. OnDay fires at the START
// of `day`, so the window it reports is the day that just ENDED.
static int g_summaryDay            = -1;
static unsigned g_payCapped        = 0;
static double   g_paidFactory      = 0.0;
static double   g_paidCarrier      = 0.0;
static double   g_paidToday        = 0.0;

static double   g_wouldPayFreight  = 0.0;
static double   g_wouldPayFuel     = 0.0;

static int      g_lastMonth        = -1;

// Per-site tallies, so "never fired" is distinguishable from "never logged".
struct SiteTally {
    unsigned calls;
    unsigned committed;
    double   tonsIn;      // sum of +tons
    double   tonsOut;     // sum of |−tons|
    double   value;
    unsigned sawBld;      // param_9 non-NULL
    unsigned sawCur;      // *(veh+0x4F0) non-NULL
    unsigned sawHome;     // *(veh+0x4F8) non-NULL
};

static SiteTally g_tally[MAX_SITES];

static int SiteIndex(unsigned rva)
{
    for (int i = 0; i < g_siteCount; i++)
        if (g_sites[i].rva == rva) return i;
    return -1;
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static bool IsPrivate(void* b)
{
    if (!b || !g_enterprise) return false;
    if (!TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return false;
    return g_enterprise->isBuildingPrivate(b) != 0;
}

static const char* KindName(unsigned char k)
{
    switch (k) {
        case SHIP_LOAD:     return "load";
        case SHIP_DELIVERY: return "unload";
        case SHIP_FREIGHT:  return "freight";
        case SHIP_FUEL:     return "fuel";
        case SHIP_PURCHASE: return "buy";
        default:            return "?";
    }
}

static const char* ClassName(SiteClass c)
{
    switch (c) {
        case SITE_REFUEL:    return "refuel";
        case SITE_LOAD:      return "load";
        case SITE_DELIVERY:  return "delivery";
        case SITE_TRANSSHIP: return "transship";
        default:             return "unknown";
    }
}

// The resource record's name lives at its head. A bad pointer yields "?".
static void ResourceName(void* res, char* out, size_t n)
{
    out[0] = 0;
    if (!res || !TsmIsValidGamePtr(res) || !ReadablePtr(res, 128)) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    const char* s = (const char*)res;
    size_t i = 0;
    for (; i + 1 < n; i++) {
        const char c = s[i];
        if (!c) break;
        if ((unsigned char)c < 32 || (unsigned char)c > 126) {
            _snprintf_s(out, n, _TRUNCATE, "?");
            return;
        }
        out[i] = c;
    }
    out[i] = 0;
    if (!out[0]) _snprintf_s(out, n, _TRUNCATE, "?");
}

// The building's type-descriptor name, so the log never again asks the reader
// to guess which object a pointer is.
static void BuildingName(void* b, char* out, size_t n)
{
    out[0] = 0;
    if (!b || !g_enterprise || !TsmIsValidGamePtr(b)) {
        _snprintf_s(out, n, _TRUNCATE, "-");
        return;
    }
    const char* nm = nullptr;
    __try { nm = g_enterprise->getBuildingObjectName(b); }
    __except (EXCEPTION_EXECUTE_HANDLER) { nm = nullptr; }
    if (!nm || !nm[0]) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    size_t i = 0;
    for (; i + 1 < n && nm[i]; i++) {
        const char c = nm[i];
        out[i] = ((unsigned char)c < 32 || (unsigned char)c > 126) ? '?' : c;
    }
    out[i] = 0;
}

// The suite's single price function. Everything this plugin charges is
// expressed against THIS number, and every pay line names it, so a change of
// price source can never silently halve or double a payment.
//
// NOTE: resource_pricing itself may be returning its shipped fallbacks
// (RE_DIGEST Part 12 section 3). That is fine and consistent - what must not
// happen is this plugin mixing a raw Res+0x58 read with a priced figure.
static double UnitPrice(void* res)
{
    if (!res || !TsmIsValidGamePtr(res)) return 0.0;

    if (g_pricing) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        const float p = g_pricing->getPrice(res, cur);
        if (p == p && p > 0.01f && p < 1e9f) return (double)p;
    }

    // Fallback: the raw record field, only if resource_pricing is absent.
    if (!ReadablePtr((const BYTE*)res + TSM_RES_PRICE_RUB, 4)) return 0.0;
    const float f = *(const float*)((const BYTE*)res + TSM_RES_PRICE_RUB);
    if (!(f == f) || f < 0.0f || f > 1e9f) return 0.0;
    return (double)f;
}

// v0.4: freight as a share of the cargo's value.
//
// `tons` is positive on this leg (the delivery). A zero or nonsense unit
// price yields zero rather than a guess - the caller counts it as a skip.
static double FreightFor(double tons, double unitPrice)
{
    const double value = tons * unitPrice;
    if (!(value > 0.0)) return 0.0;
    const double share = (double)g_freightShare;
    if (!(share > 0.0)) return 0.0;
    return value * share;
}

// One guard, used everywhere the treasury is read: an older currency host may
// carry a shorter struct, and getTreasuryBalance is past the end of it.
static bool TreasuryReadable(void)
{
    return g_currency && g_currency->getTreasuryBalance &&
           g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                     sizeof(g_currency->getTreasuryBalance);
}

// v0.5: the fuel price, resolved by NAME through resource_pricing so the
// index moves with the game's own economy. Cached for the day and dropped at
// each close, because a save load can replace the resource table under us.
static double FuelUnitPrice(void)
{
    if (!g_pricing || !g_pricing->getByName) return 0.0;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    if (!g_fuelRes) return 0.0;

    const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
    const float p = g_pricing->getPrice(g_fuelRes, cur);
    if (!(p == p) || !(p > 0.0f) || !(p < 1e9f)) return 0.0;
    return (double)p;
}

// v0.7: THE TARIFF. Fuel plus a maintenance reserve, both denominated in
// litres and both priced at the live fuel price:
//
//   work    = litres_per_trip + litres_per_tonne x tonnes
//   fuel    = work x diesel_density x fuel_price        <- the index
//   wear    = wear_litres x diesel_density x fuel_price <- sinking fund
//   tariff  = fuel x (1 + margin) + wear
//
// Two rules are enforced here, not in prose. Nothing is a rouble constant, so
// inflation cannot eat it; and nothing depends on what the cargo is WORTH, so
// a truck of plants pays for its trip the same as a truck of chemicals.
// The wear term exists because fuel x 1.35 can never buy a truck: a carrier
// has to recover vehicles over their life, and that is a per-trip cost.
//
// `*outIndex` is 1 for the fuel index, 2 for the CPI fallback, 0 for neither.
// A 0 return means "this build cannot price this delivery", not "it is free".
static double TariffFor(double tons, double* outFuel, double* outWear,
                        int* outIndex)
{
    if (outFuel)  *outFuel  = 0.0;
    if (outWear)  *outWear  = 0.0;
    if (outIndex) *outIndex = 0;
    if (!(tons > 0.0)) return 0.0;

    double fuel = 0.0, wear = 0.0;
    const double work = gLitresPerTrip + gLitresPerTonne * tons;
    const double fp   = FuelUnitPrice();

    if (fp > 0.0) {
        fuel = work * gDieselDensity * fp;
        wear = gWearLitres * gDieselDensity * fp;
        if (outIndex) *outIndex = 1;
    } else if (g_pricing) {
        const float basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        if (basket == basket && basket > 0.0f && basket < 1e9f) {
            fuel = (gBasketsPerTrip + gBasketsPerTonne * tons) * (double)basket;
            // The reserve expressed in the fallback unit: the litres ratio is
            // dimensionless, so it survives the switch unchanged.
            const double ratio =
                (gLitresPerTrip > 0.0) ? (gWearLitres / gLitresPerTrip) : 0.0;
            wear = fuel * ratio;
            if (outIndex) *outIndex = 2;
        }
    }

    if (outFuel) *outFuel = fuel;
    if (outWear) *outWear = wear;

    const double total = fuel * (1.0 + gTariffMargin) + wear;
    return (total > 0.0) ? total : 0.0;
}

// v0.5: remember who was paid, so the close can show their cash position.
// Four slots: a republic has a handful of private logistics companies, not a
// hundred, and the balance line is about seeing money arrive and leave.
// v0.6: how much of the day's freight went to whom, so the close can say
// "freight in X, spent Y" instead of making the reader subtract.
static void NoteCarrierIncome(void* b, double amount)
{
    if (!b || !(amount > 0.0)) return;
    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { g_balCarrierIn[i] += amount; break; }
    LeaveCriticalSection(&g_lock);
}

static void TrackCarrier(void* b)
{
    if (!b) return;
    EnterCriticalSection(&g_lock);
    bool known = false;
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { known = true; break; }
    if (!known && g_balCarrierCount < BAL_CARRIERS) {
        g_balCarrier[g_balCarrierCount]     = b;
        g_balCarrierPrev[g_balCarrierCount] = -1.0;   // -1 = no baseline yet
        g_balCarrierFuel[g_balCarrierCount] = -1.0;   // first read is a baseline
        g_balCarrierCount++;
    }
    LeaveCriticalSection(&g_lock);
}

static void AppendShipment(const Shipment& s)
{
    int cap = g_ledgerCapacity;
    if (cap < 1)          cap = 1;
    if (cap > LEDGER_MAX) cap = LEDGER_MAX;

    g_ledger[g_ledgerHead % cap] = s;
    g_ledgerHead = (g_ledgerHead + 1) % cap;
    g_ledgerTotal++;
}

// ---------------------------------------------------------------------------
// paying
// ---------------------------------------------------------------------------
//
// One place that moves money, so there is exactly one place to look when a
// balance changes. dry_run decides whether the call lands.
//
//   payee  - the building whose company is credited
//   amount - positive RUB
//   cat    - what the movement is for (enterprise_base's own classification)
//   label  - appears verbatim in currency's flow ledger

static bool Pay(void* payee, double amount, TsmCapitalCategory cat,
                const char* label, const char* kind, void* res, float tons)
{
    // v0.4: every refusal is counted HERE, where the reason is known, so the
    // daily line can say which one it was instead of lumping them together.
    if (!(amount >= (double)g_minPayment)) {
        EnterCriticalSection(&g_lock); g_skipSmall++;    LeaveCriticalSection(&g_lock);
        return false;
    }
    if (!g_enterprise || !g_currency) {
        EnterCriticalSection(&g_lock); g_skipNoPayee++;  LeaveCriticalSection(&g_lock);
        return false;
    }

    char bname[64], rname[64];
    BuildingName(payee, bname, sizeof(bname));
    ResourceName(res, rname, sizeof(rname));

    if (g_dailyPayCap > 0.0 && g_paidToday + amount > g_dailyPayCap) {
        EnterCriticalSection(&g_lock);
        g_payCapped++;
        LeaveCriticalSection(&g_lock);
        return false;
    }

    // v0.4 AFFORDABILITY GATE.
    //
    // moveTreasury takes a signed amount and has no floor: crediting a
    // private company and debiting the treasury by more than it holds would
    // simply drive the balance negative. The state is the payer until Ship 2,
    // so it has to be able to say no. Reads the ACTIVE currency's treasury
    // (RUB by default), the same one moveTreasury writes.
    {
        double have = 0.0;
        if (g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance))
            have = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        if (have < amount) {
            EnterCriticalSection(&g_lock); g_skipTreasury++; LeaveCriticalSection(&g_lock);
            return false;
        }
    }

    if (g_dryRun) {
        Logf("[PAY] would %s %s %+.2f (%s %.3ft) - dry_run",
             kind, bname, amount, rname, (double)tons);
    } else {
        // v0.5 (H33): the treasury is read on both sides of the move. This is
        // the only isolated proof of the payer leg that exists - it cannot be
        // contaminated by wages or taxes, because nothing else runs between
        // the two reads.
        const int cur   = TsmCurrencyActive(g_currency);
        const bool sample = TreasuryReadable();
        double before = 0.0, after = 0.0;
        if (sample) before = g_currency->getTreasuryBalance(cur);

        g_enterprise->modifyCompanyCapital(payee, amount, cat);
        g_currency->moveTreasury(-amount, label);

        if (sample) {
            after = g_currency->getTreasuryBalance(cur);
            const double gap = (after - before) + amount;   // 0 = exact
            const double mag = (gap < 0.0) ? -gap : gap;
            EnterCriticalSection(&g_lock);
            g_treasuryChecked++;
            if (mag <= 0.005) {
                g_treasuryExact++;
            } else {
                g_treasuryOff++;
                if (mag > g_treasuryMaxGap) g_treasuryMaxGap = mag;
            }
            LeaveCriticalSection(&g_lock);
        }

        Logf("[PAY] %s %s %+.2f (%s %.3ft) paid by the state",
             kind, bname, amount, rname, (double)tons);
    }

    EnterCriticalSection(&g_lock);
    g_paidToday += amount;
    g_payCount++;
    if (cat == TSM_CAP_PRODUCTION) g_paidFactory += amount;
    else                          g_paidCarrier += amount;
    LeaveCriticalSection(&g_lock);
    return true;
}

// ---------------------------------------------------------------------------
// the detour
// ---------------------------------------------------------------------------

static int DetourTransfer(void* game, void* p2, unsigned mode, void* res,
                          float tons, float* outMoved, float cap,
                          unsigned char flag, void* p9)
{
    // ARGUMENT NAMES THAT MATTER:
    //
    //   p2 = param_2. At mode 0 this is the BUILDING; at mode 1 the VEHICLE.
    //        Proven in two sessions (RE_DIGEST Part 13 §4).
    //   p9 = param_9. The FLEET OWNER on the delivery family, NULL on the
    //        load family. NOT an endpoint.
    //
    // Getting these two the wrong way round is the single easiest way to
    // build a payment that pays nobody, so they are named for their
    // parameter position and nothing else.

    // Which site called us. The patched instruction is still a `call`, so the
    // CPU pushed site+5 and that is what _ReturnAddress reads.
    const BYTE* retAddr = (const BYTE*)_ReturnAddress();
    unsigned    siteRva = 0;
    SiteClass   known   = SITE_UNKNOWN;
    int         siteIdx = -1;

    if (retAddr && g_exeBase) {
        const intptr_t off = (intptr_t)retAddr - (intptr_t)(g_exeBase + 5);
        if (off > 0 && off < (intptr_t)g_exeSize) {
            siteRva = (unsigned)off;
            siteIdx = SiteIndex(siteRva);
            if (siteIdx >= 0) known = g_sites[siteIdx].known;
        }
    }

    // At mode 0 p2 is the building. At mode 1 it is the vehicle and its home
    // depot is the fleet owner - read it BEFORE the call, because the callee
    // clobbers registers and the vehicle can be re-homed later in the tick.
    void* building = (mode == 0 && p2 && TsmIsValidGamePtr(p2) &&
                      ReadablePtr(p2, TSM_BLD_MIN_READABLE)) ? p2 : nullptr;
    void* owner    = p9;

    void* vehicleHome = nullptr;
    if (mode != 0 && p2 && TsmIsValidGamePtr(p2) &&
        ReadablePtr((const BYTE*)p2 + TSM_VEH_HOME, 8)) {
        vehicleHome = *(void**)((BYTE*)p2 + TSM_VEH_HOME);
    }

    EnterCriticalSection(&g_lock);
    g_calls++;
    LeaveCriticalSection(&g_lock);

    // Forward. The transfer returns a bool, so only the low byte of the return
    // carries the answer.
    const int  r         = g_realTransfer
                         ? g_realTransfer(game, p2, mode, res, tons,
                                          outMoved, cap, flag, p9)
                         : 0;
    const bool committed = ((unsigned char)r == 1u);

    // ---- classify, outside the lock --------------------------------------
    const bool   isStaging = (mode != 0 && vehicleHome && vehicleHome == p9);
    const double unitPrice = UnitPrice(res);

    Shipment s;
    memset(&s, 0, sizeof(s));
    s.vehicle  = (mode != 0) ? p2 : nullptr;
    s.resource = res;
    s.tons     = tons;
    s.site     = (unsigned short)(siteRva & 0xFFFF);
    s.mode     = (unsigned char)mode;
    s.building = building;

    if (committed) {
        if (isStaging) {
            s.kind = SHIP_LOAD;
        } else if (known == SITE_REFUEL) {
            s.kind      = SHIP_FUEL;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        } else if (tons > 0.0f) {
            s.kind      = SHIP_DELIVERY;
            s.unitPrice = unitPrice;
            s.value     = (double)tons * unitPrice;
        } else {
            s.kind      = SHIP_LOAD;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        }
    }

    // ---- carrier resolution (mode 1 only feeds the ring) -----------------
    bool     emitSite   = false;
    bool     emitAttrib = false;
    unsigned shipId     = 0;
    void*    carrier    = nullptr;
    bool     carrierMatched = false;

    EnterCriticalSection(&g_lock);

    if (committed && mode != 0 && vehicleHome) {
        CarrierNote& n = g_carrier[g_carrierHead % CARRIER_RING];
        n.res  = res;
        n.tons = fabsf(tons);
        n.home = vehicleHome;
        n.t  = NowSeconds();
        g_carrierHead = (g_carrierHead + 1) % CARRIER_RING;
    }

    if (committed && mode == 0 && tons < 0.0f && !owner) {
        // The load leg: param_9 is NULL, so find the vehicle that took these
        // goods from the mode-1 call that follows within a few milliseconds.
        const double now = NowSeconds();
        for (int i = 1; i <= CARRIER_RING; i++) {
            const int k = ((g_carrierHead - i) % CARRIER_RING + CARRIER_RING) % CARRIER_RING;
            const CarrierNote& n = g_carrier[k];
            if (!n.home || n.res != res) continue;
            if (now - n.t > 2.0) continue;
            const float tol = (fabsf(tons) > 0.01f) ? fabsf(tons) * 0.005f : 0.0005f;
            if (fabsf(n.tons - fabsf(tons)) > tol) continue;
            carrier = n.home;
            carrierMatched = true;
            break;
        }
    }

    if (committed) g_callsCommitted++;
    else           g_callsNoop++;

    if (siteIdx >= 0) {
        SiteTally& t = g_tally[siteIdx];
        t.calls++;
        if (committed) {
            t.committed++;
            if (tons >= 0.0f) t.tonsIn  += (double)tons;
            else              t.tonsOut += fabs((double)tons);
            t.value += s.value;
        }
        if (p9)           t.sawBld++;
        if (vehicleHome)  t.sawHome++;
    }

    if (committed) {
        if (mode == 0 && !building)                g_unknownBuilding++;
        if (isStaging)                             g_staging++;
        if (mode == 0 && !IsPrivate(building))     g_nonPrivate++;

        if (s.kind == SHIP_FUEL)                    { g_wouldPayFuel    += s.value;
                                                      g_fuelEvents++; }
        if (s.kind == SHIP_DELIVERY && s.value > 0.0) g_wouldPayFreight += s.value;

        s.id  = g_ledgerSeq++;
        s.seq = g_ledgerTotal;
        AppendShipment(s);
        shipId = s.id;
    }

    // Separate budgets: a busy day can never hide a site behind its own
    // attribution output, and payments get a third budget of their own.
    if (g_siteLog) {
        if (g_siteLoggedToday < (unsigned)g_logCapPerDay) {
            g_siteLoggedToday++;
            emitSite = true;
        } else {
            g_siteSuppressed++;
        }
    }
    if (committed) {
        if (g_attrLoggedToday < (unsigned)g_logCapPerDay) {
            g_attrLoggedToday++;
            emitAttrib = true;
        } else {
            g_attrSuppressed++;
        }
    }

    LeaveCriticalSection(&g_lock);

    // ---- logging, outside the lock ---------------------------------------
    if (emitSite) {
        char bname[64];
        BuildingName(building, bname, sizeof(bname));
        Logf("[B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u "
             "p2=%p \"%s\" priv=%d p9=%p vhome=%p class=%s",
             siteRva, mode, (tons < 0.0f ? '-' : '+'), (double)tons,
             committed ? 1 : 0, (unsigned)flag,
             p2, bname, IsPrivate(building) ? 1 : 0, p9, vehicleHome,
             ClassName(known));
    }

    // ---- billing ---------------------------------------------------------
    //
    // MODE 0 ONLY. A mode-1 call is the vehicle side of a movement a mode-0
    // call has already described; billing both would charge it twice.
    // v0.5: the probe runs even with billing off. It is a measurement, and
    // measuring a cost must not depend on whether money is being moved.
    if ((g_billing || g_tariffProbe || g_tripTariff) && committed && mode == 0 && building) {

        char rname[64];
        ResourceName(res, rname, sizeof(rname));

        double freightPaid   = 0.0;
        double probeTariff   = 0.0;
        double probeCost     = 0.0;
        double probeShare    = 0.0;
        int    probeIndex    = 0;
        bool   probeLine     = false;

        if (tons > 0.0f) {

            // ---- v0.7 THE TRIP TARIFF (this IS the money) -----------------
            // v0.6 computed this and paid the value share anyway. v0.7 pays
            // it. `trip_tariff = 0` puts the value share back untouched.
            if (g_tariffProbe || g_tripTariff) {
                probeTariff = TariffFor((double)tons, &probeCost, &probeShare,
                                        &probeIndex);

                EnterCriticalSection(&g_lock);
                if (probeTariff > 0.0) {
                    g_tariffCount++;
                    g_tariffSum       += probeTariff;
                    g_tariffCostSum   += probeCost * (1.0 + gTariffMargin);
                    g_tariffShareSum  += probeShare;
                    if (probeIndex == 1)      g_tariffFuelIndex++;
                    else if (probeIndex == 2) g_tariffCpiIndex++;
                    if (g_tariffLoggedToday < (unsigned)g_tariffLogPerDay) {
                        g_tariffLoggedToday++;
                        probeLine = true;
                    }
                } else {
                    g_tariffNoIndex++;
                }
                LeaveCriticalSection(&g_lock);
            }

            if (g_billing && g_payCarrier) {
                // H2 - freight on a delivery into `building`.
                // The fleet owner is param_9 on this family; if the site does
                // not carry one, fall back to the ring.
                void* paid = nullptr;
                if (owner && owner != building && IsPrivate(owner))      paid = owner;
                else if (!owner && carrier && carrier != building &&
                         IsPrivate(carrier))                             paid = carrier;

                if (paid) {
                    // v0.7: the trip price. The value share survives only for
                    // the comparison path (trip_tariff = 0).
                    const double freight = g_tripTariff
                        ? probeTariff
                        : FreightFor((double)tons, unitPrice);
                    if (freight <= 0.0) {
                        EnterCriticalSection(&g_lock); g_skipNoPrice++; LeaveCriticalSection(&g_lock);
                    } else if (Pay(paid, freight, TSM_CAP_WHOLESALE,
                                   "b2b_freight", "freight", res, tons)) {
                        freightPaid = freight;
                        TrackCarrier(paid);
                        NoteCarrierIncome(paid, freight);
                        BurnProbe(paid);              // v0.11: per-delivery tank sample
                    }
                } else {
                    EnterCriticalSection(&g_lock); g_skipNoPayee++; LeaveCriticalSection(&g_lock);
                }
            }

            // One line carries both numbers, so the calibration is a
            // subtraction the reader can do in their head.
            if (probeLine) {
                char bname[64];
                BuildingName(building, bname, sizeof(bname));
                Logf("[TARIFF MODEL] %s %.3ft @%.2f = %.2f -> %s: %.2f = fuel %.2f x%.2f "
                     "+ wear %.2f (index %s) | transfer_fragment=1 journey=0 "
                     "| payment_returned=%.2f billing=%d dry_run=%d",
                     rname, (double)tons, unitPrice, (double)tons * unitPrice, bname,
                     probeTariff, probeCost, 1.0 + (double)gTariffMargin, probeShare,
                     probeIndex == 1 ? "fuel" : (probeIndex == 2 ? "cpi" : "none"),
                     freightPaid, g_billing, g_dryRun);
            }
        }

        if (g_billing && tons < 0.0f && g_payFactory) {
            // H1 - the private building the goods leave is paid for them.
            if (IsPrivate(building)) {
                if (unitPrice > 0.0) {
                    const double value =
                        fabs((double)tons) * unitPrice * (double)g_factoryShare;
                    Pay(building, value, TSM_CAP_PRODUCTION,
                        "b2b_factory", "factory", res, tons);
                } else {
                    // A private building shipping goods this build cannot
                    // price. Count it and say so once per day-cap window:
                    // silence here would look like "nothing happened".
                    char bname[64];
                    BuildingName(building, bname, sizeof(bname));
                    EnterCriticalSection(&g_lock);
                    g_skipNoPrice++;
                    const bool say = (g_payLoggedToday < (unsigned)g_logCapPerDay);
                    if (say) g_payLoggedToday++;
                    LeaveCriticalSection(&g_lock);
                    if (say)
                        Logf("[PAY] skipped %s: no price for %s (unitPrice 0.00) - "
                             "resource_pricing absent or unpriced", bname, rname);
                }
            }
        }

        (void)carrierMatched;
    }

    if (emitAttrib) {
        char bname[64], rname[64];
        BuildingName(building, bname, sizeof(bname));
        ResourceName(res, rname, sizeof(rname));
        Logf("[B2B] #%u %-6s bld=%p \"%s\" priv=%d res=%s %.2ft @%.2f = %.2f "
             "(site 0x%06X, mode %u)",
             shipId, KindName(s.kind), building, bname, IsPrivate(building) ? 1 : 0,
             rname, (double)s.tons, s.unitPrice, s.value, siteRva, mode);
    }

    return r;
}

// ---------------------------------------------------------------------------
// call-site patching
// ---------------------------------------------------------------------------

static bool PatchSite(int index)
{
    if (index < 0 || index >= g_siteCount) return false;
    if (g_sitePatched[index]) return true;

    const unsigned rva = g_sites[index].rva;

    if (rva < 0x1000 || (size_t)rva + 5 > g_exeSize) {
        Logf("private_logistics  site 0x%06X is outside the image (%llu bytes) - skipped",
             rva, (unsigned long long)g_exeSize);
        return false;
    }

    BYTE* site = g_exeBase + rva;
    if (!ReadablePtr(site, 5)) {
        Logf("private_logistics  site 0x%06X not readable - skipped", rva);
        return false;
    }

    // Verify: a direct call, landing on the transfer.
    if (site[0] != 0xE8) {
        Logf("private_logistics  site 0x%06X is not a call (0x%02X) - skipped",
             rva, site[0]);
        return false;
    }
    const int32_t rel    = *(const int32_t*)(site + 1);
    BYTE*         target = site + 5 + rel;
    if (target != g_exeBase + TSM_RVA_TRANSFER) {
        Logf("private_logistics  site 0x%06X calls %p, not the transfer - skipped",
             rva, target);
        return false;
    }

    // The cave: `jmp qword ptr [rip+0]` plus the absolute detour address.
    BYTE* cave = AllocNear(site, 32);
    if (!cave) {
        Logf("private_logistics  no code cave near 0x%06X - skipped", rva);
        return false;
    }

    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&DetourTransfer;
    memcpy(cave + 6, &detour, 8);
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("private_logistics  cave out of rel32 range at 0x%06X - skipped", rva);
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("private_logistics  VirtualProtect failed at 0x%06X - nothing written", rva);
        return false;
    }

    // Re-verify under the protection.
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("private_logistics  site 0x%06X changed under us - nothing written", rva);
        return false;
    }

    site[0] = 0xE8;                              // same opcode: still a call
    *(int32_t*)(site + 1) = (int32_t)toCave;     // new destination
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    g_sitePatched[index] = true;
    Logf("private_logistics  hook 0x%06X -> transfer ok (cave %p)", rva, (void*)cave);
    return true;
}

static void PatchAllSites(void)
{
    if (!g_siteFilterActive)
        for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = true;

    int ok = 0, wanted = 0;
    for (int i = 0; i < g_siteCount; i++) {
        if (!g_siteWanted[i]) continue;
        wanted++;
        if (PatchSite(i)) ok++;
    }

    if (g_siteFilterActive)
        Logf("private_logistics  site_list active: %d of %d sites selected",
             wanted, g_siteCount);

    Logf("private_logistics  patched %d of %d transfer call sites", ok, wanted);

    if (ok == 0) {
        Logf("private_logistics  ERROR  no sites patched. Either this is not "
             "SOVIET64.exe v1.1.1.9, or the transfer is not at RVA 0x420350. "
             "Nothing was written; the game is unaffected.");
    } else if (ok < wanted) {
        Logf("private_logistics  WARN  %d site(s) refused - see the lines above. "
             "A refusal is safe: that call still reaches the engine normally.",
             wanted - ok);
    }
}

// ---------------------------------------------------------------------------
// the daily roll-up
// ---------------------------------------------------------------------------

static void OnDay(void*, int day, int month, int year)
{
    (void)year;

    // Snapshot under the lock, clear everything, print outside it.
    //
    // v0.3 BUGFIX: v0.2 snapshotted these counters without zeroing them, so
    // its "[B2B] day N" line was cumulative since load while the per-site
    // table was genuinely per-day. Both are per-day now.
    static SiteTally snap[MAX_SITES];
    unsigned calls, committed, noop, unknown, stag, nonPriv;
    unsigned siteSupp, attrSupp, paySupp, payCount, payCap, total;
    unsigned skNoPayee, skNoPrice, skSmall, skTreasury;
    unsigned tCount, tNoIndex, tFuelIdx, tCpiIdx;
    unsigned trChecked, trExact, trOff, fuelEvents;
    double   paidF, paidC, freight, fuel, tSum, tCostSum, tShareSum, trMaxGap;
    static double cIn[BAL_CARRIERS];
    static void*  cPt[BAL_CARRIERS];
    int      cCount;
    bool     newMonth;

    EnterCriticalSection(&g_lock);
    memcpy(snap, g_tally, sizeof(snap));
    memset(g_tally, 0, sizeof(g_tally));

    calls      = g_calls;
    committed  = g_callsCommitted;
    noop       = g_callsNoop;
    unknown    = g_unknownBuilding;
    stag       = g_staging;
    nonPriv    = g_nonPrivate;
    siteSupp   = g_siteSuppressed;
    attrSupp   = g_attrSuppressed;
    paySupp    = g_paySuppressed;
    payCount   = g_payCount;
    payCap     = g_payCapped;
    skNoPayee  = g_skipNoPayee;
    skNoPrice  = g_skipNoPrice;
    skSmall    = g_skipSmall;
    skTreasury = g_skipTreasury;
    tCount     = g_tariffCount;
    tNoIndex   = g_tariffNoIndex;
    tFuelIdx   = g_tariffFuelIndex;
    tCpiIdx    = g_tariffCpiIndex;
    tSum       = g_tariffSum;
    trChecked  = g_treasuryChecked;
    trExact    = g_treasuryExact;
    trOff      = g_treasuryOff;
    trMaxGap   = g_treasuryMaxGap;
    fuelEvents = g_fuelEvents;
    tCostSum   = g_tariffCostSum;
    tShareSum  = g_tariffShareSum;
    cCount     = g_balCarrierCount;
    for (int i = 0; i < BAL_CARRIERS; i++) {
        cPt[i] = g_balCarrier[i];
        cIn[i] = g_balCarrierIn[i];
        g_balCarrierIn[i] = 0.0;      // this day's income, spent by the close
    }
    total      = g_ledgerTotal;
    paidF      = g_paidFactory;
    paidC      = g_paidCarrier;
    freight    = g_wouldPayFreight;
    fuel       = g_wouldPayFuel;

    g_calls = g_callsCommitted = g_callsNoop = 0;
    g_unknownBuilding = g_staging = g_nonPrivate = 0;
    g_siteSuppressed = g_attrSuppressed = g_paySuppressed = 0;
    g_payCount = g_payCapped = 0;
    g_skipNoPayee = g_skipNoPrice = g_skipSmall = g_skipTreasury = 0;
    g_tariffCount = g_tariffNoIndex = g_tariffFuelIndex = g_tariffCpiIndex = 0;
    g_tariffCostSum = g_tariffShareSum = 0.0;
    g_tariffSum = 0.0;
    g_treasuryChecked = g_treasuryExact = g_treasuryOff = 0;
    g_treasuryMaxGap = 0.0;
    g_fuelEvents = 0;
    g_fuelRes = nullptr;          // re-resolve the index next day
    g_paidFactory = g_paidCarrier = 0.0;
    g_wouldPayFreight = g_wouldPayFuel = 0.0;
    g_paidToday = 0.0;

    g_siteLoggedToday = 0;
    g_attrLoggedToday = 0;
    g_payLoggedToday  = 0;
    g_tariffLoggedToday = 0;

    newMonth = (month != g_lastMonth);
    g_lastMonth = month;

    // v0.4: this callback fires at the START of `day`, so everything above is
    // the day that just ENDED. Report that day, not this one - the old label
    // was one ahead of its own numbers.
    // A loaded save can rewind the clock; do not report a day in the future.
    if (g_summaryDay > day) g_summaryDay = -1;
    const int closedDay = (g_summaryDay >= 0) ? g_summaryDay : (day - 1);
    g_summaryDay = day;
    LeaveCriticalSection(&g_lock);

    Logf("[B2B] day %d closed: %u calls, %u committed, %u no-op | unknown %u, "
         "staging %u, non-private %u | suppressed site %u attrib %u pay %u",
         closedDay, calls, committed, noop, unknown, stag, nonPriv, siteSupp,
         attrSupp, paySupp);

    // Per-site table. A site that never fired is simply absent - absence is
    // evidence rather than a logging artefact.
    for (int i = 0; i < g_siteCount; i++) {
        const SiteTally& t = snap[i];
        if (t.calls == 0) continue;
        Logf("[B2B SITE] day %d  0x%06X %-9s calls=%u commit=%u in=%.3f out=%.3f "
             "value=%.2f | arg9!=0 %u home!=0 %u",
             closedDay, g_sites[i].rva, ClassName(g_sites[i].known), t.calls,
             t.committed, t.tonsIn, t.tonsOut, t.value, t.sawBld, t.sawHome);
    }

    if (g_billing) {
        Logf("[PAY] day %d closed: %u payment(s), %u capped | skipped: no payee %u, "
             "no price %u, too small %u, treasury %u | factory %.2f, carrier %.2f | %s",
             closedDay, payCount, payCap, skNoPayee, skNoPrice, skSmall,
             skTreasury, paidF, paidC,
             g_dryRun ? "DRY RUN - nothing moved"
                      : "PAID by the state treasury");
    } else {
        Logf("[PAY] day %d closed: billing off", closedDay);
    }

    // v0.5: the model's own total, beside the index it managed to use.
    // `unindexed` is the honest failure: neither a fuel price nor a CPI
    // basket resolved, so the probe refused to invent a figure.
    if (g_tariffProbe || g_tripTariff) {
        Logf("[TARIFF MODEL] day %d closed: %u priced_transfer_fragment(s), model %.2f = fuel %.2f x%.2f + "
             "wear %.2f, mean %.2f | indexed by fuel %u, by cpi %u, unindexed %u "
             "| NOT journeys or payment totals; billing=%d dry_run=%d",
             closedDay, tCount,
             tSum, tCostSum, 1.0 + (double)gTariffMargin, tShareSum,
             tCount ? tSum / (double)tCount : 0.0,
             tFuelIdx, tCpiIdx, tNoIndex, g_billing, g_dryRun);
    }

    // v0.5: THE MONEY PATH, BOTH ENDS (H33, H37).
    //
    // v0.4 proved the payee's account to the kopeck; it never read the payer.
    // Here the treasury is sampled at every close, so consecutive closes give
    // the movement, and `unexplained` is what the [PAY] lines cannot account
    // for - zero when the books close, non-zero when something else touched
    // the treasury in the same day.
    //
    // The carriers' capital is printed beside it. Income minus everything the
    // company spent, fuel included: if the game charges a private DO for
    // diesel, the difference between this delta and the freight it was paid
    // IS that bill - the number Ship 2 has to price against.
    if (g_balanceLog) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(cur);
            have  = true;
        }

        const double paid = paidF + paidC;

        if (!have) {
            Logf("[BAL] day %d closed: treasury unavailable (no currency service)", closedDay);
        } else if (!g_treasuryHaveClose) {
            Logf("[BAL] day %d closed: treasury %.2f (baseline - the next close gives the "
                 "movement) | paid %.2f", closedDay, treas, paid);
        } else {
            const double moved = treas - g_treasuryAtClose;
            Logf("[BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid "
                 "%.2f | the rest of the movement is the rest of the state's day%s",
                 closedDay, g_treasuryAtClose, treas, moved, paid,
                 g_dryRun ? " | DRY RUN: nothing should have moved" : "");
        }
        if (have) { g_treasuryAtClose = treas; g_treasuryHaveClose = true; }

        // v0.5 (H33): the proof, per payment rather than per day.
        if (trChecked > 0 || trOff > 0) {
            Logf("[BAL] day %d treasury leg: %u payment(s) sampled either side, "
                 "%u moved exactly the amount, %u off by any margin (largest %.2f)",
                 closedDay, trChecked, trExact, trOff, trMaxGap);
        }

        // v0.6: the carrier's day in one line. `spent` is the arithmetic
        // difference - freight in, minus the change in capital - and in this
        // save it is the fuel bill, because fuel is the only thing a private
        // DO buys (it arrives as a cat=2 FOREIGN debit on the same account).
        for (int i = 0; i < cCount; i++) {
            void* b = cPt[i];
            if (!b || !g_enterprise || !g_enterprise->getCompanyCapital) continue;
            const double cap = g_enterprise->getCompanyCapital(b);
            char bname[64];
            BuildingName(b, bname, sizeof(bname));
            if (g_balCarrierPrev[i] < 0.0) {
                Logf("[BAL] day %d   %s capital %.2f (baseline) | freight in %.2f",
                     closedDay, bname, cap, cIn[i]);
            } else {
                const double delta = cap - g_balCarrierPrev[i];
                Logf("[BAL] day %d   %s capital %.2f (%+.2f) | freight in %.2f | spent %.2f",
                     closedDay, bname, cap, delta, cIn[i], cIn[i] - delta);
            }

            // v0.7: THE FUEL METER.
            //
            // monthWholesale is where enterprise_base books cat=2 FOREIGN
            // debits, and diesel is the only foreign purchase a distribution
            // office makes - so this is the carrier's real fuel bill for the
            // calendar month, read back from the game rather than assumed.
            //
            // Fuel is bought in bulk and stored, so a day's purchases are not
            // a day's burn. The run average is, because the tank delta washes
            // out of a long run. That is the number the [TARIFF] model is
            // scored against, and the number litres_per_trip should hold.
            if (g_enterprise->describeEnterprise) {
                TsmEnterpriseInfo info;
                memset(&info, 0, sizeof(info));
                info.structSize = sizeof(TsmEnterpriseInfo);
                if (g_enterprise->describeEnterprise(b, &info) == 1) {
                    const double bill = info.monthWholesale;
                    double spent = 0.0;
                    if (g_balCarrierFuel[i] < 0.0) {
                        // First read after a load: monthWholesale already holds
                        // whatever the save carried. Take it as the baseline and
                        // say so - counting it would bill one day for a month.
                        g_balCarrierFuel[i] = bill;
                        Logf("[FUEL] day %d   %s meter baseline: month wholesale %.2f "
                             "(not counted as this day's burn)", closedDay, bname, bill);
                    } else {
                        spent = (bill < g_balCarrierFuel[i])
                              ? bill                      // month boundary: accumulators cleared
                              : (bill - g_balCarrierFuel[i]);
                        g_balCarrierFuel[i] = bill;
                    }
                    g_fuelPaidRun += (spent > 0.0) ? spent : 0.0;
                    const double fprice = FuelUnitPrice();
                    double litres = 0.0;
                    if (fprice > 0.0 && gDieselDensity > 0.0)
                        litres = ((spent > 0.0) ? spent : 0.0) / (gDieselDensity * fprice);
                    g_fuelLitresRun += litres;
                    Logf("[FUEL] day %d   %s diesel %.2f (measured, cat=2) = %.2f L at %.2f/%s",
                         closedDay, bname, (spent > 0.0) ? spent : 0.0, litres, fprice,
                         g_currency ? TsmCurrencySymbol(g_currency) : "RUB");
                }
            }
            g_balCarrierPrev[i] = cap;
        }

        // v0.7: the run total, and the one number to copy into the ini.
        g_fuelTripsRun += tCount;
        if (g_fuelTripsRun > 0) {
            const double perTrip = g_fuelLitresRun / (double)g_fuelTripsRun;
            // v0.9: this counts PURCHASES, not burn. Fuel is bought in bulk
            // and sits in tanks, so a short run and a long run give different
            // answers from the same fleet - 3.27 L/trip over 1,100 trips,
            // 7.14 over 214. Do not treat it as a measurement of the burn.
            Logf("[FUEL PURCHASE MODEL] run: %.2f RUB = %.2f configured_litres_estimate over %llu transfer_fragment(s) = %.3f estimate_per_fragment "
                 "| configured %.2f L | fuel sits in tanks: this is purchases, not burn",
                 g_fuelPaidRun, g_fuelLitresRun, g_fuelTripsRun, perTrip, gLitresPerTrip);
        }

        // Legacy candidate differences: not native burn and not loaded journeys.
        if (g_fuelTripsRun > 0 && g_burnTonsRun > 0.0 && gDieselDensity > 0.0) {
            const double litres = g_burnTonsRun / gDieselDensity;
            const double lpt    = litres / (double)g_fuelTripsRun;
            // v0.11: L/DAY is printed beside L/trip because the physics is
            // per-tick. L/trip moves whenever the shops order differently -
            // the same fleet gave 3.27 and 7.14 L/trip in two runs - while
            // L per fleet-day is the same quantity in both. Read this one.
            const double lpd = g_tankDaysRun ? litres / (double)g_tankDaysRun : 0.0;
            const double lvd = g_tankVehDaysRun ? litres / (double)g_tankVehDaysRun : 0.0;
            const double lva = g_activeVehDaysRun ? litres / (double)g_activeVehDaysRun : 0.0;
            Logf("[TANK SAMPLE] run: decrease_raw=%.4f configured_density_litres_estimate=%.1f "
                 "| estimate_per_transfer_fragment=%.2f over %llu fragment(s) "
                 "| estimate_per_fleet_day=%.2f over %u close(s) | estimate_per_vehicle_day=%.2f over %u "
                 "| estimate_per_legacy_active_day=%.2f over %u legacy_active / %u legacy_idle "
                 "| sample_increases=%u | configured_model_litres=%.2f "
                 "units=unverified native_activity=unverified exact_burn=0 verified_refill=0",
                 g_burnTonsRun, litres, lpt, g_fuelTripsRun, lpd, g_tankDaysRun, lvd,
                 g_tankVehDaysRun, lva, g_activeVehDaysRun, g_idleVehDaysRun,
                 g_refillsRun, gLitresPerTrip);
            if (g_litresAuto)
                Logf("[TANK SAMPLE] auto-tune NOT applied: sampled changes/units are unverified; "
                     "configured litres_per_trip remains %.3f", gLitresPerTrip);
        }
    }

    // Legacy inter-transfer endpoint samples are not deduction events.
    if (g_probeStepsRun > 0 && gDieselDensity > 0.0) {
        const double litres = g_probeBurnRun / gDieselDensity;
        const double lpt    = litres / (double)g_probeStepsRun;
        Logf("[FUEL SAMPLE] run: net_raw=%.4f configured_density_litres_estimate=%.1f "
             "between %u transfer_fragment_pair(s) = %.2f estimate_per_pair "
             "| %u interval(s) skipped (sample increase or candidate-count change) "
             "| configured_model_litres=%.2f | %s",
             g_probeBurnRun, litres, g_probeStepsRun, lpt, g_probeSkipRun,
             gLitresPerTrip,
             "units=unverified exact_burn=0 journey_count=unknown calibration_input=0");
    }

    // v0.8: who does this company actually own, and what can they carry?
    CensusFleets(closedDay);

    // v0.6: re-read the index at every close. At plugin start the world has
    // not loaded, so v0.5's "fuel NOT FOUND" and "treasury opens at 0.00" were
    // both true for about a second and then false forever. Printing them here
    // is the honest place.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        Logf("[IDX] day %d closed: fuel '%s' = %.2f/%s (%s) | cpi basket %.2f/day | "
             "margin %.0f%%, wear %.2f L",
             closedDay, gFuelResource, fp,
             g_currency ? TsmCurrencySymbol(g_currency) : "RUB",
             fp > 0.0 ? "resolved" : "NOT RESOLVED",
             (double)basket, gTariffMargin * 100.0, gWearLitres);
    }

    // v0.5: fuel telemetry. The refuel site has been silent in every session
    // so far; a count of 0 here after fuel is switched on is itself a result.
    Logf("[FUEL] day %d refuel sites: %u call(s), %.2f RUB of fuel moved through "
         "the hooked sites", closedDay, fuelEvents, fuel);

    if (newMonth) {
        Logf("[B2B] month %d: %u shipments recorded, %.2f RUB would have moved "
             "(freight %.2f, fuel %.2f)",
             month, total, freight + fuel, freight, fuel);
    }

    AutoTargetsOnDay(day, closedDay);
}

// ---------------------------------------------------------------------------
// automatic private DO targets and infrastructure discovery (v0.16)
// ---------------------------------------------------------------------------

#define AUTO_RVA_VECTOR_PUSH_BACK 0x08E050
#define AUTO_RVA_TASKCFG_CTOR     0x1512A0
#define AUTO_DO_TARGETS_BEGIN     0x0D38
#define AUTO_DO_TARGETS_END       0x0D40
#define AUTO_DO_DIRTY             0x0D50
#define AUTO_DO_UI_TIMER          0x0FC0
// Conservative plugin-side bound: native add-flow supports five selections,
// but panel+0x310 per-target control-array capacity above five is unverified.
// This limits new auto targets without deleting/purging any manual entries.
#define AUTO_DO_SAFE_TARGET_COUNT 5
#define AUTO_DO_ROAD_TYPE         43
#define AUTO_DO_RAIL_TYPE         52
#define AUTO_STORAGE_BUILDING_TYPE 5
#define AUTO_CUSTOMS_TYPE         20
#define AUTO_CUSTOMS_MAX          512
#define AUTO_STATE_STORAGE_MAX    512
#define AUTO_STATE_PERMISSION_MAX 512
#define AUTO_STATE_PREF_FILE "private_logistics_state.bin"
struct AutoCustomSite { void* building; float x; float z; char name[64]; };
// permission bits: 1=load FROM site, 2=deliver INTO site; zero=deny both.
struct AutoStateStorageSite { void* building; uint64_t key; float x; float z; char name[64]; unsigned char permission; };
struct AutoStatePermission { uint64_t key; unsigned char mode; };
static AutoCustomSite g_autoCustomSites[AUTO_CUSTOMS_MAX];
static AutoStateStorageSite g_autoStateStorageSites[AUTO_STATE_STORAGE_MAX];
static unsigned g_autoStateStorageCount = 0;
static AutoStatePermission g_statePermissions[AUTO_STATE_PERMISSION_MAX];
static unsigned g_statePermissionCount = 0;
#define AUTO_TARGET_ENTRY_SIZE    0x18
#define AUTO_TASKCFG_SIZE         0x198
#define AUTO_STORAGE_STRIDE       0xE0
#define AUTO_MAX_TARGETS_READ     65536

static uint64_t AutoHashMix(uint64_t h, uint8_t byte)
{
    return (h ^ byte) * 1099511628211ull;
}

static bool AutoReadBuildingType(void* b, int* outType)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return false;
    void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td) || !ReadablePtr((BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
        return false;
    *outType = *(int*)((BYTE*)td + TSM_TD_TYPE_NUM);
    return true;
}

static bool AutoBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // The building's position is the translation row of its embedded
    // C3D_NODE, not bld+0x08/+0x10 (those yielded zeros in the v0.15 probe).
    const BYTE* node = (const BYTE*)b + TSM_BLD_NODE;
    if (ReadablePtr(node, 0x40)) {
        const float x = *(const float*)(node + 0x30);
        const float z = *(const float*)(node + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            if (outX) *outX = x;
            if (outZ) *outZ = z;
            return true;
        }
    }

    // Some unfinished sites can lack a usable node transform; use the
    // verified placement-bbox fallback used by enterprise_base.
    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                const float x = mn[1], z = mn[3];
                if (x == x && z == z && fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
                    if (outX) *outX = x;
                    if (outZ) *outZ = z;
                    return true;
                }
            }
        }
    }
    return false;
}

// Stable session/save identity: native object name + type + quantized position.
// No raw Building* is retained in the retry ledger.
static bool AutoBuildingKey(void* b, uint64_t* outKey, int* outType)
{
    int type = -1;
    float x = 0.0f, z = 0.0f;
    if (!AutoReadBuildingType(b, &type) || !AutoBuildingPosition(b, &x, &z))
        return false;

    uint64_t h = 14695981039346656037ull;
    const char* objectName = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { objectName = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { objectName = nullptr; }
    }
    if (objectName) {
        for (size_t i = 0; i < 128; ++i) {
            if (!ReadablePtr(objectName + i, 1)) return false;
            const uint8_t c = (uint8_t)objectName[i];
            h = AutoHashMix(h, c);
            if (c == 0) break;
        }
    }
    const int32_t qx = (int32_t)(x * 10.0f + (x >= 0.0f ? 0.5f : -0.5f));
    const int32_t qz = (int32_t)(z * 10.0f + (z >= 0.0f ? 0.5f : -0.5f));
    const uint32_t values[3] = { (uint32_t)type, (uint32_t)qx, (uint32_t)qz };
    const uint8_t* bytes = (const uint8_t*)values;
    for (size_t i = 0; i < sizeof(values); ++i) h = AutoHashMix(h, bytes[i]);
    // H1 FIX: mix raw float bits to break 0.1m quantized collisions (two clones within 10cm)
    // keeps qx/qz for backward compat but adds sub-decimeter entropy; one-time permission reset for old saves
    uint32_t fx = 0, fz = 0;
    memcpy(&fx, &x, sizeof(fx));
    memcpy(&fz, &z, sizeof(fz));
    const uint8_t* fxB = (const uint8_t*)&fx; for (size_t i = 0; i < sizeof(fx); ++i) h = AutoHashMix(h, fxB[i]);
    const uint8_t* fzB = (const uint8_t*)&fz; for (size_t i = 0; i < sizeof(fz); ++i) h = AutoHashMix(h, fzB[i]);
    *outKey = h ? h : 1;
    if (outType) *outType = type;
    return true;
}

static void AutoBuildingName(void* b, char* out, size_t capacity)
{
    if (!out || capacity == 0) return;
    out[0] = 0;
    const char* name = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { name = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { name = nullptr; }
    }
    if (name) {
        size_t i = 0;
        for (; i + 1 < capacity; ++i) {
            if (!ReadablePtr(name + i, 1)) break;
            out[i] = name[i];
            if (out[i] == 0) return;
        }
        out[i] = 0;
        if (i > 0) return;
    }
    const char fallback[] = "unknown";
    size_t j = 0;
    while (j + 1 < capacity && fallback[j]) { out[j] = fallback[j]; ++j; }
    out[j] = 0;
}

static unsigned char AutoStatePermissionGet(uint64_t key)
{
    if (!key) return 0;
    unsigned char mode = 0;
    EnterCriticalSection(&g_lock);
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { mode = g_statePermissions[i].mode; break; }
    }
    LeaveCriticalSection(&g_lock);
    return mode;
}

static bool AutoStatePermissionSet(uint64_t key, unsigned char mode)
{
    if (!key || mode > AUTO_ALLOW_BOTH) return false;
    EnterCriticalSection(&g_lock);
    unsigned found = g_statePermissionCount;
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { found = i; break; }
    }
    if (mode == 0) {
        if (found < g_statePermissionCount) {
            g_statePermissions[found] = g_statePermissions[g_statePermissionCount - 1];
            --g_statePermissionCount;
        }
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (found < g_statePermissionCount) {
        g_statePermissions[found].mode = mode;
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (g_statePermissionCount >= AUTO_STATE_PERMISSION_MAX) {
        LeaveCriticalSection(&g_lock);
        return false;
    }
    g_statePermissions[g_statePermissionCount].key = key;
    g_statePermissions[g_statePermissionCount].mode = mode;
    ++g_statePermissionCount;
    LeaveCriticalSection(&g_lock);
    return true;
}

static bool AutoHasNativeStorage(void* b);

static unsigned AutoBuildingCapabilities(void* building)
{
    int type = -1;
    if (!AutoReadBuildingType(building, &type) || !AutoHasNativeStorage(building)) return 0;
    char asset[128];
    AutoBuildingName(building, asset, sizeof(asset));
    return AutoSiteCapabilities(type, asset);
}

static void AutoDrawStatePermissions(uint64_t key, unsigned capabilities)
{
    const unsigned char oldMode = AutoStatePermissionGet(key);
    unsigned char newMode = (unsigned char)(oldMode & capabilities);
    if (capabilities & AUTO_ALLOW_LOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_LOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO loading (take goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_LOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (capabilities & AUTO_ALLOW_UNLOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_UNLOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO deliveries (receive goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_UNLOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (newMode != oldMode) {
        if (!AutoStatePermissionSet(key, newMode)) {
            Logf("[AUTO DO] state permission table full; change not retained");
            return;
        }
        Logf("[AUTO DO] state permissions key=%016llX load=%u unload=%u",
             (unsigned long long)key, (newMode & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (newMode & AUTO_ALLOW_UNLOAD) ? 1u : 0u);
    }
}

static void StateStorageWidgetProvider(void* building, void*)
{
    if (!g_autoTargetsEnabled || !g_uiPanels || !TsmUiPanelsHasWidgets(g_uiPanels) ||
        !building || !TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE) || !g_enterprise ||
        !g_enterprise->isBuildingPrivate || g_enterprise->isBuildingPrivate(building)) return;
    const unsigned capabilities = AutoBuildingCapabilities(building);
    uint64_t key = 0;
    if (!AutoBuildingKey(building, &key, nullptr)) return;
    static uint64_t lastPanelKey = 0;
    if (lastPanelKey != key) {
        lastPanelKey = key;
        int type = -1; AutoReadBuildingType(building, &type);
        char asset[128]; AutoBuildingName(building, asset, sizeof(asset));
        const unsigned effective = AutoStatePermissionGet(key) & capabilities;
        Logf("[AUTO DO] state panel key=%016llX type=%d asset=%s capabilities=%u permissions=%u native_load=%u native_unload=%u (native 0=checked,1=unchecked)%s",
             (unsigned long long)key, type, asset, capabilities, effective,
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_LOAD) != 0),
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_UNLOAD) != 0),
             capabilities ? "" : " (unsupported role: controls hidden, automatic access denied)");
    }
    if (!capabilities) return; // unknown roles do not get a misleading loading control
    AutoDrawStatePermissions(key, capabilities);
}

static bool AutoHasNativeStorage(void* b)
{
    if (!b || !TsmIsValidGamePtr(b) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_STORAGES_BEGIN, 16)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
    const uintptr_t end = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_END);
    if (!begin || end <= begin || ((end - begin) % AUTO_STORAGE_STRIDE) != 0) return false;
    const uintptr_t count = (end - begin) / AUTO_STORAGE_STRIDE;
    return count <= 4096 && ReadablePtr((const void*)begin, count * AUTO_STORAGE_STRIDE);
}

static bool AutoReadTargetVector(void* office, void*** outBegin, size_t* outCount)
{
    if (!office || !TsmIsValidGamePtr(office) ||
        !ReadablePtr((BYTE*)office + AUTO_DO_TARGETS_BEGIN, 24)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_BEGIN);
    const uintptr_t end   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END);
    const uintptr_t cap   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END + 8);
    if (!begin && !end && !cap) { *outBegin = nullptr; *outCount = 0; return true; }
    if (!begin || !end || !cap || end < begin || cap < end || ((end - begin) & 7) != 0)
        return false;
    const size_t count = (size_t)((end - begin) / sizeof(void*));
    if (count > AUTO_MAX_TARGETS_READ ||
        !ReadablePtr((const void*)begin, count * sizeof(void*))) return false;
    *outBegin = (void**)begin; *outCount = count;
    return true;
}

static unsigned AutoOfficeHash(uint64_t officeKey)
{
    officeKey ^= officeKey >> 33; officeKey *= 0xff51afd7ed558ccdull;
    officeKey ^= officeKey >> 33; officeKey *= 0xc4ceb9fe1a85ec53ull;
    officeKey ^= officeKey >> 33;
    return (unsigned)officeKey & (AUTO_OFFICE_HASH_SIZE - 1);
}

static bool AutoBuildOfficeMemoIndex(void)
{
    memset(g_autoOfficeMemoHeads, 0, sizeof(g_autoOfficeMemoHeads));
    for (unsigned i = 0; i < g_autoMemoCount; ++i) {
        AutoMemo& memo = g_autoMemo[i];
        unsigned slot = AutoOfficeHash(memo.officeKey);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
            AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
            if (head.officeKey == 0 || head.officeKey == memo.officeKey) {
                head.officeKey = memo.officeKey;
                memo.nextOffice = head.headPlusOne ? head.headPlusOne - 1 : 0xFFFFFFFFu;
                head.headPlusOne = i + 1;
                inserted = true;
                break;
            }
            slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoOfficeMemoHeadIndex(uint64_t officeKey)
{
    unsigned slot = AutoOfficeHash(officeKey);
    for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
        const AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
        if (head.officeKey == 0) return -1;
        if (head.officeKey == officeKey)
            return head.headPlusOne ? (int)(head.headPlusOne - 1) : -1;
        slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
    }
    return -1;
}

static unsigned AutoPairHash(uint64_t officeKey, uint64_t targetKey)
{
    uint64_t h = officeKey ^ (targetKey + 0x9E3779B97F4A7C15ull + (officeKey << 6) + (officeKey >> 2));
    h ^= h >> 30; h *= 0xBF58476D1CE4E5B9ull;
    h ^= h >> 27; h *= 0x94D049BB133111EBull;
    h ^= h >> 31;
    return (unsigned)h & (AUTO_MEMO_HASH_SIZE - 1);
}

static int AutoMemoFind(uint64_t officeKey, uint64_t targetKey)
{
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        const unsigned stored = g_autoMemoHash[slot];
        if (stored == 0) return -1;
        const unsigned index = stored - 1;
        if (g_autoMemo[index].officeKey == officeKey &&
            g_autoMemo[index].targetKey == targetKey) return (int)index;
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static int AutoPointerIndex(void* building)
{
    if (!building) return -1;
    uintptr_t h = (uintptr_t)building;
    h >>= 4; h ^= h >> 17; h ^= h >> 9;
    unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
    for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
        AutoPointerSlot& item = g_autoPointerHash[slot];
        if (!item.building) return -1;
        if (item.building == building) return (int)item.entityIndex;
        slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoBuildPointerIndex(unsigned entityCount)
{
    memset(g_autoPointerHash, 0, sizeof(g_autoPointerHash));
    for (unsigned i = 0; i < entityCount; ++i) {
        uintptr_t h = (uintptr_t)g_autoEntities[i].building;
        h >>= 4; h ^= h >> 17; h ^= h >> 9;
        unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
            AutoPointerSlot& item = g_autoPointerHash[slot];
            if (!item.building) {
                item.building = g_autoEntities[i].building;
                item.entityIndex = i;
                inserted = true;
                break;
            }
            if (item.building == g_autoEntities[i].building) { inserted = true; break; }
            slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoMemoInsert(uint64_t officeKey, uint64_t targetKey)
{
    if (g_autoMemoCount >= AUTO_MEMO_MAX) return -1;
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        if (g_autoMemoHash[slot] == 0) {
            const unsigned index = g_autoMemoCount++;
            g_autoMemo[index].officeKey = officeKey;
            g_autoMemo[index].targetKey = targetKey;
            g_autoMemo[index].nextOffice = 0xFFFFFFFFu;
            g_autoMemo[index].state = 1;
            g_autoMemoHash[slot] = index + 1;
            return (int)index;
        }
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoAppendNativeTarget(void* office, void* target, unsigned directions)
{
    if (!g_exeBase || g_exeSize <= AUTO_RVA_TASKCFG_CTOR + 1 ||
        g_exeSize <= AUTO_RVA_VECTOR_PUSH_BACK + 1) return false;
    void** oldBegin = nullptr; size_t oldCount = 0;
    if (!AutoReadTargetVector(office, &oldBegin, &oldCount)) return false;
    (void)oldBegin; (void)oldCount;

    typedef void* (__fastcall *TTaskCfgCtor)(void*);
    typedef void (__fastcall *TVectorPushBack)(void*, const void*);
    TTaskCfgCtor ctor = (TTaskCfgCtor)(g_exeBase + AUTO_RVA_TASKCFG_CTOR);
    TVectorPushBack pushBack = (TVectorPushBack)(g_exeBase + AUTO_RVA_VECTOR_PUSH_BACK);
    if (!ReadablePtr((const void*)ctor, 1) || !ReadablePtr((const void*)pushBack, 1)) return false;

    void* entry = ::operator new(AUTO_TARGET_ENTRY_SIZE, std::nothrow);
    void* cfgMem = ::operator new(AUTO_TASKCFG_SIZE, std::nothrow);
    if (!entry || !cfgMem) {
        if (entry) ::operator delete(entry);
        if (cfgMem) ::operator delete(cfgMem);
        return false;
    }
    memset(entry, 0, AUTO_TARGET_ENTRY_SIZE);
    memset(cfgMem, 0, AUTO_TASKCFG_SIZE);
    void* cfg = nullptr;
    try { cfg = ctor(cfgMem); } catch (...) { cfg = nullptr; }
    if (!cfg) { ::operator delete(entry); ::operator delete(cfgMem); return false; }

    // The native UI adds entries OFF, awaiting its activation checkbox. An
    // autonomous company must request activation itself, not only add the row.
    // Native dirty/upkeep/planner paths still validate route compatibility.
    AutoApplyEntryActivation(entry, directions);
    *(void**)((BYTE*)entry + 0x08) = target;
    *(void**)((BYTE*)entry + 0x10) = cfg;
    // Ghidra reports no resource-filter field in this 0x18-byte TaskEntry.
    // The planner's target/connection index is rebuilt in its working area
    // by reachability; do not invent or write a filter at entry+0x0C.
    AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    *((BYTE*)cfg + 0x28) = 1;                    // native active flag
    *((BYTE*)cfg + 0xF0) = 1;                    // native validated flag

    try {
        // FUN_14008E050(vector, &entry) is the native vector<TaskEntry*>::push_back.
        pushBack((BYTE*)office + AUTO_DO_TARGETS_BEGIN, &entry);
    } catch (...) {
        ::operator delete(entry); ::operator delete(cfgMem);
        return false;
    }
    return true;
}

static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void*)
{
    if (event == TSM_SAVE_LOADING) {
        InterlockedIncrement(&g_fuelDiagnosticEpoch);
        PanelObserverLoading();
    }
    // Persist permission keys only; never retain building pointers. Save/load
    // callbacks run on the file thread, so no game-memory access is performed.
    if (!g_persistence || !dir) return;
    if (event == TSM_SAVE_WRITING) {
        if (!g_persistence->writeAtomic) return;
        BYTE data[12 + AUTO_STATE_PERMISSION_MAX * 9];
        memcpy(data, "PLSTOR18", 8);
        uint32_t count = 0;
        EnterCriticalSection(&g_lock);
        count = g_statePermissionCount;
        memcpy(data + 8, &count, sizeof(count));
        size_t at = 12;
        for (unsigned i = 0; i < g_statePermissionCount; ++i) {
            memcpy(data + at, &g_statePermissions[i].key, sizeof(uint64_t));
            at += sizeof(uint64_t);
            data[at++] = g_statePermissions[i].mode;
        }
        LeaveCriticalSection(&g_lock);
        if (!g_persistence->writeAtomic(dir, AUTO_STATE_PREF_FILE, data, at))
            Logf("[AUTO DO] state permissions were not saved");
    } else if (event == TSM_SAVE_LOADING) {
        if (g_persistence->readFile) {
            size_t len = 0;
            void* raw = g_persistence->readFile(dir, AUTO_STATE_PREF_FILE, &len);
            AutoStatePermission loaded[AUTO_STATE_PERMISSION_MAX];
            unsigned loadedCount = 0;
            const bool legacy = raw && len >= 12 && memcmp(raw, "PLSTOR17", 8) == 0;
            if (raw && len >= 12 && (legacy || memcmp(raw, "PLSTOR18", 8) == 0)) {
                uint32_t count = 0;
                memcpy(&count, (BYTE*)raw + 8, sizeof(count));
                if (count <= AUTO_STATE_PERMISSION_MAX && len == 12 + (size_t)count * 9) {
                    size_t at = 12;
                    for (uint32_t i = 0; i < count; ++i) {
                        uint64_t key = 0;
                        memcpy(&key, (BYTE*)raw + at, sizeof(key));
                        at += sizeof(key);
                        const unsigned char mode = AutoMigratePermission(*((BYTE*)raw + at++), legacy);
                        if (key && mode != 0) {
                            loaded[loadedCount].key = key;
                            loaded[loadedCount].mode = mode;
                            ++loadedCount;
                        }
                    }
                } else {
                    Logf("[AUTO DO] state permission file has invalid length/count");
                }
            } else if (raw) {
                Logf("[AUTO DO] state permission file header invalid");
            }
            if (raw) free(raw);
            EnterCriticalSection(&g_lock);
            memset(g_statePermissions, 0, sizeof(g_statePermissions));
            if (loadedCount) memcpy(g_statePermissions, loaded,
                                    loadedCount * sizeof(loaded[0]));
            g_statePermissionCount = loadedCount;
            LeaveCriticalSection(&g_lock);
        }
        InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 1);
    }
}

static void AutoProbeStateInfrastructure(int closedDay, unsigned entityCount)
{
    g_autoStateStorageCount = 0;
    BYTE* game = g_exeBase ? TsmGameObject(g_exeBase) : nullptr;
    TsmBuildingSpan span;
    if (!game || !TsmGetBuildings(game, &span) ||
        !span.begin || !ReadablePtr(span.begin, span.count * sizeof(void*))) {
        Logf("[AUTO DO] day %d infra probe: world building vector unavailable", closedDay);
        return;
    }

    unsigned stateStorages = 0, privateStorages = 0, customsCount = 0;
    unsigned stateStoragesStored = 0, customsStored = 0;
    for (size_t i = 0; i < span.count; ++i) {
        void* b = span.begin[i];
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b)) continue;
        int type = -1;
        if (!AutoReadBuildingType(b, &type)) continue;
        // Discover storage-bearing sites for enforcement; classification, not
        // mere storage presence, determines allowed directions/visible widgets.
        if (AutoHasNativeStorage(b) && type != AUTO_CUSTOMS_TYPE &&
            type != AUTO_DO_ROAD_TYPE && type != AUTO_DO_RAIL_TYPE) {
            const bool isPrivate = g_enterprise && g_enterprise->isBuildingPrivate &&
                                   g_enterprise->isBuildingPrivate(b);
            if (isPrivate) ++privateStorages;
            else {
                ++stateStorages;
                if (stateStoragesStored < AUTO_STATE_STORAGE_MAX) {
                    AutoStateStorageSite& site = g_autoStateStorageSites[stateStoragesStored++];
                    site.building = b;
                    site.key = 0;
                    AutoBuildingKey(b, &site.key, nullptr);
                    site.permission = site.key ? (unsigned char)(AutoStatePermissionGet(site.key) & AutoBuildingCapabilities(b)) : 0;
                    site.x = 0.0f; site.z = 0.0f;
                    AutoBuildingPosition(b, &site.x, &site.z);
                    AutoBuildingName(b, site.name, sizeof(site.name));
                }
            }
            continue;
        }
        if (type != AUTO_CUSTOMS_TYPE) continue;
        ++customsCount;
        float x = 0.0f, z = 0.0f;
        if (!AutoBuildingPosition(b, &x, &z) || customsStored >= AUTO_CUSTOMS_MAX) continue;
        AutoCustomSite& site = g_autoCustomSites[customsStored++];
        site.building = b; site.x = x; site.z = z;
        AutoBuildingName(b, site.name, sizeof(site.name));
    }

    g_autoStateStorageCount = stateStoragesStored;
    Logf("[AUTO DO] day %d infra probe: world=%llu state_storage=%u private_storage=%u customs=%u customs_indexed=%u",
         closedDay, (unsigned long long)span.count, stateStorages, privateStorages,
         customsCount, customsStored);
    static int previousStateStorages = -1;
    static int previousCustoms = -1;
    if ((int)stateStorages != previousStateStorages || (int)customsCount != previousCustoms) {
        previousStateStorages = (int)stateStorages;
        previousCustoms = (int)customsCount;
        const unsigned show = (stateStoragesStored < 32) ? stateStoragesStored : 32;
        for (unsigned i = 0; i < show; ++i)
            Logf("[AUTO DO] state storage candidate name=%s key=%016llX pos=(%.1f,%.1f) permission=%s",
                 g_autoStateStorageSites[i].name,
                 (unsigned long long)g_autoStateStorageSites[i].key,
                 g_autoStateStorageSites[i].x, g_autoStateStorageSites[i].z,
                 g_autoStateStorageSites[i].permission == AUTO_ALLOW_BOTH ? "LOAD+UNLOAD" :
                 (g_autoStateStorageSites[i].permission == AUTO_ALLOW_LOAD ? "LOAD" :
                  (g_autoStateStorageSites[i].permission == AUTO_ALLOW_UNLOAD ? "UNLOAD" : "DENY")));
        if (stateStoragesStored > show)
            Logf("[AUTO DO] state storage list truncated: %u of %u shown",
                 show, stateStorages);
    }
    for (unsigned i = 0; i < entityCount; ++i) {
        AutoEntity& office = g_autoEntities[i];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        float ox = 0.0f, oz = 0.0f;
        if (!AutoBuildingPosition(office.building, &ox, &oz)) continue;
        int nearest = -1;
        double bestSq = 1.0e100;
        for (unsigned c = 0; c < customsStored; ++c) {
            const double dx = (double)g_autoCustomSites[c].x - ox;
            const double dz = (double)g_autoCustomSites[c].z - oz;
            const double d2 = dx * dx + dz * dz;
            if (d2 < bestSq) { bestSq = d2; nearest = (int)c; }
        }
        if (nearest >= 0)
            Logf("[AUTO DO] day %d nearest customs (geometric, not assigned) office=%016llX customs=%s dist=%.1fm",
                 closedDay, (unsigned long long)office.key,
                 g_autoCustomSites[nearest].name, sqrt(bestSq));
        else
            Logf("[AUTO DO] day %d nearest customs unavailable office=%016llX",
                 closedDay, (unsigned long long)office.key);
    }
}

static bool AutoTaskConfigValid(void* cfg)
{
    if (!cfg || !TsmIsValidGamePtr(cfg) || !ReadablePtr(cfg, AUTO_TASKCFG_SIZE)) return false;
    for (unsigned side = 0; side < 2; ++side) {
        const BYTE* p = (const BYTE*)cfg + (side ? 0xC8 : 0);
        const uintptr_t begin = *(const uintptr_t*)(p + 8);
        const uintptr_t end = *(const uintptr_t*)(p + 16);
        if ((!begin && end) || end < begin || (end - begin) % 16 != 0 ||
            (end - begin) / 16 > 4096) return false;
        if (end != begin && (!TsmIsValidGamePtr((void*)begin) ||
                            !ReadablePtr((void*)begin, end - begin))) return false;
    }
    return true;
}

static bool AutoSetTargetDirections(void* entry, unsigned directions)
{
    if (!entry || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) return false;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) {
        Logf("[AUTO DO] invalid task config; cannot enforce direction mask=%u", directions);
        return false;
    }
    const bool audit = g_autoLogLines < (unsigned)g_autoTargetLogLimit;
    AutoTaskAudit before = {};
    if (audit) before = AutoReadTaskAudit(entry, cfg);
    const bool directionsChanged = AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    const bool activationChanged = AutoApplyEntryActivation(entry, directions);
    const bool changed = directionsChanged || activationChanged;
    if (changed && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        Logf("[AUTO DO] task directions target=%p load=%u unload=%u entry_enabled=%u (native route validation still required)",
             *(void**)((BYTE*)entry + 0x08), (directions & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (directions & AUTO_ALLOW_UNLOAD) ? 1u : 0u, (unsigned)*(BYTE*)entry);
        ++g_autoLogLines;
    }
    if (changed && audit && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        const AutoTaskAudit after = AutoReadTaskAudit(entry, cfg);
        Logf("[AUTO DO AUDIT] target=%p changed_fields=0x%02X enabled=%u->%u load_all=%u->%u unload_all=%u->%u load_pct=%.9g->%.9g unload_pct=%.9g->%.9g load_bits=%08X->%08X unload_bits=%08X->%08X",
             *(void**)((BYTE*)entry + 8), AutoTaskAuditChanges(before, after),
             (unsigned)before.enabled, (unsigned)after.enabled,
             (unsigned)before.loadAll, (unsigned)after.loadAll,
             (unsigned)before.unloadAll, (unsigned)after.unloadAll,
             AutoAuditPercent(before.loadBits), AutoAuditPercent(after.loadBits),
             AutoAuditPercent(before.unloadBits), AutoAuditPercent(after.unloadBits),
             (unsigned)before.loadBits, (unsigned)after.loadBits,
             (unsigned)before.unloadBits, (unsigned)after.unloadBits);
        ++g_autoLogLines;
    }
    return changed;
}

static void AutoLogNativeTarget(void* entry, int type, unsigned requested, bool preserve)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) return; // enforcement path reports invalid configs
    const BYTE* p = (const BYTE*)cfg;
    const uintptr_t lb = *(const uintptr_t*)(p + 8), le = *(const uintptr_t*)(p + 16);
    const uintptr_t ub = *(const uintptr_t*)(p + 0xD0), ue = *(const uintptr_t*)(p + 0xD8);
    Logf("[AUTO DO] native target=%p type=%d policy=%s requested_mask=%u enabled=%u load_all=%u load_pct=%.3f load_selectors=%llu unload_all=%u unload_pct=%.3f unload_selectors=%llu",
         *(void**)((BYTE*)entry + 8), type, preserve ? "preserve-customs" : "managed",
         requested, (unsigned)*(BYTE*)entry, (unsigned)p[0], (double)*(const float*)(p + 4),
         (unsigned long long)((le - lb) / 16), (unsigned)p[0xC8], (double)*(const float*)(p + 0xCC),
         (unsigned long long)((ue - ub) / 16));
    ++g_autoLogLines;
}


// Called only from daily reconciliation; does not arm, clear or advance timers.
static void AutoLogOfficePlanState(void* office, uint64_t key, int day,
                                   const char* phase, bool notificationRequested)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    const BYTE* p = (const BYTE*)office;
    const bool dirtyReadable = ReadablePtr(p + AUTO_DO_DIRTY, 1);
    const bool timerReadable = ReadablePtr(p + AUTO_DO_UI_TIMER, sizeof(float));
    float timer = 0.0f;
    if (timerReadable) memcpy(&timer, p + AUTO_DO_UI_TIMER, sizeof(timer));
    Logf("[AUTO DO AUDIT] day=%d office=%p key=%016llX phase=%s notification_requested=%u dirty=%d timer_readable=%u countdown=%.9g",
         day, office, (unsigned long long)key, phase, notificationRequested ? 1u : 0u,
         dirtyReadable ? (int)p[AUTO_DO_DIRTY] : -1, timerReadable ? 1u : 0u, (double)timer);
    ++g_autoLogLines;
}

static void AutoTargetsOnDay(int day, int closedDay)
{
    if (!g_autoTargetsEnabled || !g_enterprise ||
        !g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex ||
        !g_enterprise->isBuildingPrivate) return;

    const bool afterLoad = (InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 0) != 0) ||
                           (g_autoLastDay >= 0 && day < g_autoLastDay);
    if (afterLoad)
        Logf("[AUTO DO] day %d save/load detected: native TaskEntry/TaskCfg were deserialized; reconciling generated targets after load", closedDay);
    g_autoLastDay = day;

    const size_t n = g_enterprise->getEnterpriseCount();
    if (n > AUTO_ENTITY_MAX) {
        Logf("[AUTO DO] day %d skipped: registered count %llu exceeds fixed discovery capacity %u",
             closedDay, (unsigned long long)n, (unsigned)AUTO_ENTITY_MAX);
        return;
    }

    unsigned entityCount = 0, officeCount = 0, candidateCount = 0;
    for (size_t i = 0; i < n; ++i) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b) ||
            !g_enterprise->isBuildingPrivate(b)) continue;
        uint64_t key = 0; int type = -1;
        if (!AutoBuildingKey(b, &key, &type)) continue;
        const bool isOffice = (type == AUTO_DO_ROAD_TYPE || type == AUTO_DO_RAIL_TYPE);
        const unsigned capabilities = isOffice ? 0 : AutoBuildingCapabilities(b);
        const bool hasStorage = isOffice || capabilities != 0;
        if (!hasStorage) continue;
        g_autoEntities[entityCount].building = b;
        g_autoEntities[entityCount].key = key;
        g_autoEntities[entityCount].type = type;
        g_autoEntities[entityCount].hasStorage = isOffice ? 0 : 1;
        g_autoEntities[entityCount].isStateStorage = 0;
        g_autoEntities[entityCount].statePermission = (unsigned char)capabilities;
        g_autoEntities[entityCount].targetPresent = 0;
        ++entityCount;
        if (isOffice) ++officeCount; else ++candidateCount;
    }

    AutoProbeStateInfrastructure(closedDay, entityCount);

    // Public access defaults deny. Keep denied sites indexed too: existing
    // native/manual entries must obey the same state permissions as new ones.
    for (unsigned si = 0; si < g_autoStateStorageCount && entityCount < AUTO_ENTITY_MAX; ++si) {
        AutoStateStorageSite& site = g_autoStateStorageSites[si];
        if (!site.building || !site.key) continue;
        AutoEntity& e = g_autoEntities[entityCount++];
        e.building = site.building; e.key = site.key; e.type = AUTO_STORAGE_BUILDING_TYPE;
        e.hasStorage = site.permission != 0;
        e.isStateStorage = 1; e.statePermission = site.permission; e.targetPresent = 0;
        if (site.permission != 0) ++candidateCount;
    }

    if (officeCount == 0) return;
    if (!AutoBuildPointerIndex(entityCount)) {
        Logf("[AUTO DO] day %d: pointer index capacity exhausted; scan aborted", closedDay);
        return;
    }
    if (!AutoBuildOfficeMemoIndex()) {
        Logf("[AUTO DO] day %d: office memo index capacity exhausted; scan aborted", closedDay);
        return;
    }

    unsigned planned = 0, added = 0, existing = 0, suppressed = 0, invalid = 0;
    g_autoLogLines = 0;
    for (unsigned oi = 0; oi < entityCount; ++oi) {
        AutoEntity& office = g_autoEntities[oi];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        void** entries = nullptr; size_t entryCount = 0;
        if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
            Logf("[AUTO DO] day %d invalid target vector; skipped office key=%016llX",
                 closedDay, (unsigned long long)office.key);
            ++invalid; continue;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-reconcile", false);
        const size_t entryCountAtStart = entryCount;
        for (unsigned ei = 0; ei < entityCount; ++ei) g_autoEntities[ei].targetPresent = 0;
        bool doConfigChanged = false;
        for (size_t ei = 0; ei < entryCount; ++ei) {
            void* entry = entries[ei];
            if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
            void* targetBuilding = *(void**)((BYTE*)entry + 0x08);
            if (!targetBuilding || !TsmIsValidGamePtr(targetBuilding) ||
                !ReadablePtr(targetBuilding, TSM_BLD_MIN_READABLE)) continue;
            int targetType = -1;
            if (AutoReadBuildingType(targetBuilding, &targetType) && AutoPreserveManualCustoms(targetType)) {
                // Border trade is not a state-enterprise permission. Preserve
                // the user's native customs task, including its resource lists
                // and activation. Customs auto-selection is not implemented.
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] preserving manual customs target=%p entry_enabled=%u",
                         targetBuilding, (unsigned)*(BYTE*)entry);
                    ++g_autoLogLines;
                }
                AutoLogNativeTarget(entry, targetType, 0, true);
                continue;
            }
            const int targetIndex = AutoPointerIndex(targetBuilding);
            if (targetIndex >= 0) g_autoEntities[targetIndex].targetPresent = 1;
            // Enforce all targets of this PRIVATE office, including manually
            // present public entries, unindexed/unknown roles and default OFF.
            const unsigned capabilities = AutoBuildingCapabilities(targetBuilding);
            const bool isPrivate = g_enterprise->isBuildingPrivate(targetBuilding) != 0;
            unsigned permissions = 0;
            if (!isPrivate) {
                uint64_t key = 0;
                permissions = AutoBuildingKey(targetBuilding, &key, nullptr) ?
                    AutoStatePermissionGet(key) : 0;
            }
            const unsigned directions = AutoEffectiveDirections(isPrivate, permissions, capabilities);
            if (!g_autoTargetsDryRun)
                doConfigChanged |= AutoSetTargetDirections(entry, directions);
            AutoLogNativeTarget(entry, targetType, directions, false);
        }

        // Native upkeep can disable an entry in place: keep deduplication for
        // entries still present. Missing generated entries are eligible for
        // recreation regardless of why they disappeared. A private company
        // owns its target list; manual removal is not a persistent veto.
        // State-building permissions remain a separate eligibility gate.
        for (int mi = AutoOfficeMemoHeadIndex(office.key); mi >= 0;
             mi = g_autoMemo[mi].nextOffice == 0xFFFFFFFFu ? -1 : (int)g_autoMemo[mi].nextOffice) {
            AutoMemo& memo = g_autoMemo[mi];
            if (memo.state != 1) continue;
            bool found = false;
            for (size_t ei = 0; ei < entryCount; ++ei) {
                void* entry = entries[ei];
                if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
                void* target = *(void**)((BYTE*)entry + 0x08);
                const int targetIndex = AutoPointerIndex(target);
                if (targetIndex >= 0 && g_autoEntities[targetIndex].key == memo.targetKey) {
                    found = true; break;
                }
            }
            if (!found) {
                memo.state = 3; // preserve hash chain; allow normal eligibility/re-add
                Logf("[AUTO DO] day %d target key=%016llX absent; eligible for automatic reconnection",
                     closedDay, (unsigned long long)memo.targetKey);
            }
        }

        unsigned doPlanned = 0, doAdded = 0, doExisting = 0, doSuppressed = 0;
        size_t projectedAdds = 0;
        // State storages were appended to g_autoEntities after private sites;
        // walk backward so an explicit state-source opt-in gets first access
        // to the five-entry conservative safe budget.
        for (unsigned ti = entityCount; ti > 0; --ti) {
            AutoEntity& target = g_autoEntities[ti - 1];
            if (!target.hasStorage || target.building == office.building) continue;
            if (target.targetPresent) {
                ++existing; ++doExisting; continue;
            }
            const int memoIndex = AutoMemoFind(office.key, target.key);
            if (entryCountAtStart + projectedAdds >= AUTO_DO_SAFE_TARGET_COUNT) {
                ++suppressed; ++doSuppressed;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d target=%016llX omitted: conservative target count %u reached (existing=%llu)",
                         closedDay, (unsigned long long)target.key,
                         (unsigned)AUTO_DO_SAFE_TARGET_COUNT,
                         (unsigned long long)entryCountAtStart);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++planned; ++doPlanned;
            if (g_autoTargetsDryRun) {
                ++projectedAdds;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d DRY RUN office=%016llX target=%016llX type=%d",
                         closedDay, (unsigned long long)office.key,
                         (unsigned long long)target.key, target.type);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (memoIndex < 0 && g_autoMemoCount >= AUTO_MEMO_MAX) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d memo capacity %u reached; target omitted",
                         closedDay, (unsigned)AUTO_MEMO_MAX);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (!AutoAppendNativeTarget(office.building, target.building, target.statePermission)) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d append failed office=%016llX target=%016llX",
                         closedDay, (unsigned long long)office.key, (unsigned long long)target.key);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++projectedAdds;
            if (memoIndex < 0) {
                if (AutoMemoInsert(office.key, target.key) < 0) {
                    ++invalid;
                    Logf("[AUTO DO] day %d memo hash insertion failed after native append", closedDay);
                }
            } else {
                g_autoMemo[memoIndex].state = 1;
            }
            target.targetPresent = 1;
            ++added; ++doAdded;
            entries = nullptr; entryCount = 0;
            if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
                ++invalid; break;
            }
        }

        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-notify", doAdded > 0 || doConfigChanged);
        if (doAdded > 0 || doConfigChanged) {
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_DIRTY, 1))
                *((BYTE*)office.building + AUTO_DO_DIRTY) = 1;
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_UI_TIMER, sizeof(float)))
                *(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "after-notify", doAdded > 0 || doConfigChanged);
        if (doPlanned || doExisting || doSuppressed || doAdded || doConfigChanged)
            Logf("[AUTO DO] day %d office=%016llX type=%d existing=%u planned=%u added=%u config_changed=%u suppressed=%u",
                 closedDay, (unsigned long long)office.key, office.type, doExisting,
                 doPlanned, doAdded, doConfigChanged ? 1u : 0u, doSuppressed);
    }

    Logf("[AUTO DO] day %d summary: private offices=%u candidates=%u planned=%u added=%u existing=%u suppressed=%u invalid=%u mode=%s",
         closedDay, officeCount, candidateCount, planned, added, existing, suppressed, invalid,
         g_autoTargetsDryRun ? "DRY RUN" : "WRITE");
}

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_logistics.ini";
    char v[256];

    g_enabled        = H->configInt(ini, "private_logistics", "enabled", 1);
    g_billing        = H->configInt(ini, "private_logistics", "billing", 0);
    g_panelObserverEnabled = H->configInt(ini, "private_logistics", "panel_fuel_observer", 1);
    g_scheduleObserverEnabled = H->configInt(ini, "private_logistics", "schedule_observer", 1);
    g_dryRun         = H->configInt(ini, "private_logistics", "dry_run", 1);
    g_payFactory     = H->configInt(ini, "private_logistics", "pay_factory", 1);
    g_payCarrier     = H->configInt(ini, "private_logistics", "pay_carrier", 1);
    g_logCapPerDay   = H->configInt(ini, "private_logistics", "log_cap_per_day", 400);
    g_ledgerCapacity = H->configInt(ini, "private_logistics", "ledger_capacity", 512);
    g_siteLog        = H->configInt(ini, "private_logistics", "site_log", 1);
    g_autoTargetsEnabled = H->configInt(ini, "private_logistics", "auto_targets_enabled", 1);
    g_autoTargetsDryRun  = H->configInt(ini, "private_logistics", "auto_targets_dry_run", 0);
    g_autoTargetLogLimit = H->configInt(ini, "private_logistics", "auto_targets_log_limit", 80);
    g_autoSourcePercent = AutoClampSourcePercent(H->configInt(ini, "private_logistics", "auto_source_percent", 10));
    g_autoDestinationPercent = AutoClampDestinationPercent(H->configInt(ini, "private_logistics", "auto_destination_percent", 30));
    Logf("[AUTO DO] company thresholds: load above %d%% / deliver below %d%% (managed tasks, reconciled daily)",
         g_autoSourcePercent, g_autoDestinationPercent);
    if (g_autoSourcePercent != 10)
        Logf("[AUTO DO] source threshold override=%d%% (this test ships 10%%); an explicit deployed INI setting overrides the fallback", g_autoSourcePercent);
    Logf("[AUTO DO] permission-test profile: billing=%d auto_targets=%d apply=%d (expected 0/1/1)",
         g_billing, g_autoTargetsEnabled, !g_autoTargetsDryRun);
    if (g_billing || !g_autoTargetsEnabled || g_autoTargetsDryRun)
        Logf("[AUTO DO] WARNING deployed INI overrides the permission-test defaults; rebuild/deploy the shipped INI for this test");
    if (H->configInt(ini, "private_logistics", "state_storage_always_on", 0))
        Logf("[AUTO DO] state_storage_always_on is obsolete and ignored; per-building permissions are required");
    if (g_autoTargetLogLimit < 0) g_autoTargetLogLimit = 0;
    if (g_autoTargetLogLimit > 1000) g_autoTargetLogLimit = 1000;

    if (g_logCapPerDay < 0)            g_logCapPerDay = 0;
    if (g_logCapPerDay > 5000)         g_logCapPerDay = 5000;
    if (g_ledgerCapacity < 1)          g_ledgerCapacity = 1;
    if (g_ledgerCapacity > LEDGER_MAX) g_ledgerCapacity = LEDGER_MAX;

    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), ""))
        g_freightShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "factory_share", v, sizeof(v), ""))
        g_factoryShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "min_payment", v, sizeof(v), ""))
        g_minPayment = (float)atof(v);
    if (H->configString(ini, "private_logistics", "daily_pay_cap", v, sizeof(v), ""))
        g_dailyPayCap = atof(v);

    // v0.5: the tariff model. Every number here is a physical quantity or a
    // ratio - litres, tonnes per litre, a percentage. Not one of them is a
    // price in currency units, which is what makes the model survive a
    // currency event.
    g_tariffProbe     = H->configInt(ini, "private_logistics", "tariff_probe", 1);
    g_balanceLog      = H->configInt(ini, "private_logistics", "balance_log", 1);
    g_tariffLogPerDay = H->configInt(ini, "private_logistics", "tariff_log_per_day", 60);

    if (H->configString(ini, "private_logistics", "litres_per_trip", v, sizeof(v), ""))
        gLitresPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "litres_per_tonne", v, sizeof(v), ""))
        gLitresPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "tariff_margin", v, sizeof(v), ""))
        gTariffMargin = atof(v);
    if (H->configString(ini, "private_logistics", "diesel_density", v, sizeof(v), ""))
        gDieselDensity = atof(v);
    g_tripTariff = H->configInt(ini, "private_logistics", "trip_tariff", 1);
    g_censusLog  = H->configInt(ini, "private_logistics", "census_log", 1);
    g_vehScan    = H->configInt(ini, "private_logistics", "veh_scan", 1);
    g_vehScanLogPerDay = H->configInt(ini, "private_logistics", "veh_scan_log_per_day", 40);
    // v0.11: raw cargo-vector dump, up to three vehicles per close.
    g_cargoDump  = H->configInt(ini, "private_logistics", "cargo_dump", 1);
    if (g_vehScanLogPerDay < 0)   g_vehScanLogPerDay = 0;
    if (g_vehScanLogPerDay > 512) g_vehScanLogPerDay = 512;
    g_litresAuto = H->configInt(ini, "private_logistics", "litres_auto", 0);
    if (H->configString(ini, "private_logistics", "wear_litres", v, sizeof(v), ""))
        gWearLitres = atof(v);

    // v0.7: freight_share is RETIRED. Honouring it silently would make every
    // delivery look mispriced against a tariff that no longer reads it, so a
    // key left in the ini is reported and zeroed rather than obeyed.
    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), "") && v[0]) {
        const float f = (float)atof(v);
        g_freightShareRetired = (f > 0.0f) ? f : 0.0f;
        g_freightShare = 0.0f;
    }
    if (H->configString(ini, "private_logistics", "baskets_per_trip", v, sizeof(v), ""))
        gBasketsPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "baskets_per_tonne", v, sizeof(v), ""))
        gBasketsPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "fuel_resource", v, sizeof(v), "") && v[0])
        strncpy_s(gFuelResource, sizeof(gFuelResource), v, _TRUNCATE);

    // v0.4: a share is a fraction of cargo value. 0 is legal (freight off);
    // more than half would make haulage cost more than the goods.
    if (g_freightShare < 0.0f)   g_freightShare = 0.0f;
    if (g_freightShare > 0.50f)  g_freightShare = 0.50f;
    if (g_tripTariff != 0)       g_tripTariff   = 1;
    if (gWearLitres  < 0.0)      gWearLitres    = 0.0;
    if (gWearLitres  > 500.0)    gWearLitres    = 500.0;
    if (g_litresAuto != 1)       g_litresAuto   = 0;
    if (g_factoryShare  < 0.0f) g_factoryShare  = 0.0f;
    if (g_factoryShare  > 5.0f) g_factoryShare  = 5.0f;
    if (g_minPayment    < 0.0f) g_minPayment    = 0.0f;
    if (g_dailyPayCap   < 0.0)  g_dailyPayCap   = 0.0;

    // v0.5 clamps. Density is the one hard range: a value outside it is a
    // typo, and a typo here would misprice every delivery by orders of
    // magnitude, so it falls back to diesel rather than being honoured.
    if (g_tariffLogPerDay < 0)    g_tariffLogPerDay = 0;
    if (g_tariffLogPerDay > 5000) g_tariffLogPerDay = 5000;
    if (gLitresPerTrip  < 0.0)   gLitresPerTrip  = 0.0;
    if (gLitresPerTonne < 0.0)   gLitresPerTonne = 0.0;
    if (gLitresPerTrip  > 500.0) gLitresPerTrip  = 500.0;
    if (gLitresPerTonne > 50.0)  gLitresPerTonne = 50.0;
    if (gTariffMargin   < 0.0)   gTariffMargin   = 0.0;
    if (gTariffMargin   > 5.0)   gTariffMargin   = 5.0;
    if (!(gDieselDensity > 0.0001) || gDieselDensity > 0.01)
        gDieselDensity = 0.00084;
    if (gBasketsPerTrip  < 0.0)  gBasketsPerTrip  = 0.0;
    if (gBasketsPerTonne < 0.0)  gBasketsPerTonne = 0.0;
    if (gBasketsPerTrip  > 1000.0) gBasketsPerTrip = 1000.0;
    if (gBasketsPerTonne > 100.0)  gBasketsPerTonne = 100.0;

    // site_list, e.g. "0x1CB7FF" or "1CB7FF,68F792". Absent or empty = all.
    if (H->configString(ini, "private_logistics", "site_list", v, sizeof(v), "") && v[0]) {
        bool hit[MAX_SITES] = { false };
        int  matched = 0;

        for (char* p = v; *p; ) {
            while (*p == ' ' || *p == '\t' || *p == ',') p++;
            if (!*p) break;

            char* end = p;
            while (*end && *end != ',' && *end != ' ' && *end != '\t') end++;

            const char save = *end;
            *end = 0;
            const unsigned want = (unsigned)strtoul(p, nullptr, 16);
            *end = save;

            for (int i = 0; i < g_siteCount; i++) {
                if (g_sites[i].rva == want) { hit[i] = true; matched++; break; }
            }
            p = end;
        }

        if (matched > 0) {
            for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = hit[i];
            g_siteFilterActive = true;
        } else {
            Logf("private_logistics  WARN  site_list matched no known site - patching all");
        }
    }
}

// ---------------------------------------------------------------------------
// v0.8: the vehicle census
// ---------------------------------------------------------------------------
//
// Read-only, once a day, on the day callback. Every pointer is validated
// before it is followed, every vector length is capped, and nothing here can
// fail a delivery: a bad read reports "unreadable" and moves on.
//
// The numbers this produces are the ones the final model needs and no formula
// can supply:
//   - how many vehicles a company actually owns
//   - what each one carries at its peak (its capacity, observed not assumed)
//   - which catalogue entry the game attaches to it (+0x510, unproven)

static int VehSlot(void* v)
{
    for (int i = 0; i < g_vehCount; i++)
        if (g_vehPtr[i] == v) return i;
    if (g_vehCount >= VEH_MAX) return -1;
    g_vehPtr[g_vehCount]     = v;
    g_vehPeakTons[g_vehCount] = 0.0;
    g_vehSamples[g_vehCount]  = 0;
    return g_vehCount++;
}

// veh+0x3B8 is a vector of cargo entries, stride 0x10: { Resource* , float tons }.
// Returns the total carried and the number of non-empty slots.
static double CargoTons(void* veh, int* slots)
{
    *slots = 0;
    const BYTE* p = (const BYTE*)veh;
    if (!ReadablePtr(p + 0x3B8, 16)) return 0.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end <= beg) return 0.0;

    // v0.12: the cap was 8, and over 8 it returned 0 SILENTLY - which is how
    // this census came to report "0 slot(s)" on trucks that demonstrably held
    // fuel. A storage vector with a pre-allocated slot per resource type is
    // perfectly plausible, and it would be longer than eight. Raise the cap and
    // stop hiding the case.
    size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return 0.0;

    double t = 0.0;
    for (size_t i = 0; i < n; i++) {
        const float tons = *(const float*)(beg + i * 16 + 8);
        if (tons == tons && tons > 0.0f && tons < 1.0e6f) {
            t += (double)tons;
            (*slots)++;
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// Candidate resource-slot and display-field readers, NOT authoritative fuel.
// The old building/upkeep report does not prove this road truck's burn or
// refill writer, canonical store, native clamp, or physical mass conversion.
// ---------------------------------------------------------------------------
static void* FuelResourcePtr(void)
{
    if (!g_pricing || !g_pricing->getByName) return nullptr;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    return g_fuelRes;
}

// Returns raw quantity from a matching resource slot. Units/classification are
// unverified for this road truck. *found = 1 even if the quantity is invalid.
// -1 when the vector cannot be read at all.
static double FuelSlotTons(void* v, int* found)
{
    *found = 0;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x3B8, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) return -1.0;
    if (n == 0) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    void* fuel = FuelResourcePtr();
    if (!fuel) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* e = beg + i * 16;
        if (*(const void* const*)e != fuel) continue;
        float t = *(const float*)(e + 8);
        // Preserve invalid/negative raw values: diagnostics must never invent
        // an empty tank by applying an unverified native clamp.
        *found = 1;
        return (double)t;
    }
    return 0.0;
}

// The panel backing quantity (+0x5F0) and metadata capacity (+0x8684).
// Display correlation alone does not establish native burn/refill semantics.
static double MirrorTons(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_FUEL_OFF, 4)) return -1.0;
    float f = *(const float*)(p + VEH_FUEL_OFF);
    if (!(f == f) || f < -1000.0f || f > 1000.0f) return -1.0;
    return (double)f;
}

static double TankCapacity(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x1708, 8)) return -1.0;
    const BYTE* meta = *(const BYTE* const*)(p + 0x1708);
    if (!meta || !TsmIsValidGamePtr(meta)) return -1.0;
    if (!ReadablePtr(meta + 0x8684, 4)) return -1.0;
    const float c = *(const float*)(meta + 0x8684);
    if (!(c == c) || c <= 0.0f || c > 1000.0f) return -1.0;
    return (double)c;
}

// v0.11: sum of one building's fleet tanks, in tonnes. -1 = not readable.
static double FleetFuelTons(void* b, int* outCount)
{
    *outCount = 0;
    if (!b || !ReadablePtr((const BYTE*)b + 0xC70, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
    const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 8;
    if (n == 0 || n > VEH_MAX) return -1.0;
    if (!ReadablePtr(beg, n * 8)) return -1.0;

    double t = 0.0;
    int c = 0;
    for (size_t i = 0; i < n; i++) {
        void* v = *(void* const*)(beg + i * 8);
        if (!v) continue;
        int found = 0;
        double lvl = FuelSlotTons(v, &found);
        const auto reading = private_logistics_fuel_diagnostics::Select(
            found != 0, lvl, MirrorTons(v));
        if (!reading.valid) continue;
        t += reading.value;
        c++;
    }
    *outCount = c;
    return (c > 0) ? t : -1.0;
}

// One sample per committed delivery. Cheap: one vector walk and a few floats.
static void BurnProbe(void* b)
{
    int n = 0;
    const double now = FleetFuelTons(b, &n);
    if (now < 0.0) return;

    if (g_probeBld != b || g_probeFuelTons < 0.0 || n != g_probeVehCount) {
        g_probeBld      = b;                     // first sample, or a new fleet
        g_probeFuelTons = now;
        g_probeVehCount = n;
        return;
    }

    const double d = g_probeFuelTons - now;      // positive = burned
    if (d < -0.02 || d > 0.5) {
        g_probeSkipRun++;                        // refill, or the fleet changed size
    } else {
        g_probeBurnRun  += d;
        g_probeStepsRun++;
    }
    g_probeFuelTons = now;
}

// v0.12: RAW storage-vector dump. Two vectors, every slot, nothing filtered.
//
// Why this exists: the vehicle appears to keep its fuel in a
// {Resource*, float tons} slot, the same shape the transfer function moves
// goods with. If so, one of the vectors below holds diesel and the slot will
// track the +0x5F0 reading day after day.
//
// Two things went wrong in v0.10 and are fixed here:
//   - the census capped the vector at eight slots and returned "0 slot(s)"
//     for anything longer, which is why a truck holding 0.29 t of fuel could
//     report an empty hold. That was OUR bug, not evidence about the game.
//   - only the 0x3B8 vector was read. The trailer vector at 0x3A0 was never
//     looked at, and fuel may live there.
//
// A slot whose tonnage falls every day and jumps at a refill is the tank.
static void DumpVector(void* v, int off, const char* label, int closedDay, int slotIdx)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + off, 16)) return;

    const BYTE* beg = *(const BYTE* const*)(p + off);
    const BYTE* end = *(const BYTE* const*)(p + off + 8);
    if (!beg || !end || end < beg) {
        Logf("[CARGO] day %d s=%d %s unreadable beg=%p end=%p",
             closedDay, slotIdx, label, beg, end);
        return;
    }

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) {
        Logf("[CARGO] day %d s=%d %s n=%llu (implausible)",
             closedDay, slotIdx, label, (unsigned long long)n);
        return;
    }
    if (n == 0) {
        Logf("[CARGO] day %d s=%d %s n=0", closedDay, slotIdx, label);
        return;
    }
    if (!ReadablePtr(beg, n * 16)) return;

    Logf("[CARGO] day %d s=%d %s n=%llu", closedDay, slotIdx, label,
         (unsigned long long)n);
    const size_t show = (n < 16) ? n : 16;
    for (size_t i = 0; i < show; i++) {
        const BYTE* e   = beg + i * 16;
        const void* res = *(const void* const*)e;
        const float t   = *(const float*)(e + 8);
        if (!(t > 0.0f) && !(t < 0.0f)) continue;      // skip empty slots
        Logf("[CARGO] day %d s=%d %s   %llu res=%p %.4f t", closedDay, slotIdx,
             label, (unsigned long long)i, res, (double)t);
    }
}

static void CargoDump(int slot, void* v, int closedDay)
{
    if (!g_cargoDump || slot < 0 || slot > 2) return;   // first three vehicles
    DumpVector(v, 0x3A0, "trailer", closedDay, slot);
    DumpVector(v, 0x3B8, "cargo",   closedDay, slot);
}

// ---------------------------------------------------------------------------
// v0.9: the field scanner
// ---------------------------------------------------------------------------
//
// Compare the vehicle's first 0x800 bytes against yesterday's copy and report
// the words that moved. The baseline is taken silently on first sight so that
// day one does not dump a whole struct.
//
// Known pointer/vector ranges are skipped: they are noise here and the values
// would only ever read as "some address".
//
//   +0x3A0..0x3C8  trailer vector, cargo vector
//   +0x4F0..0x520  current building, home building, the +0x510 integer
//   +0x610..0x620  line stops
static bool VehScanSkip(int off)
{
    return (off >= 0x3A0 && off < 0x3C8) ||
           (off >= 0x4F0 && off < 0x520) ||
           (off >= 0x610 && off < 0x620);
}

// ---------------------------------------------------------------------------
// v0.10: read the tank
// ---------------------------------------------------------------------------
//
// Legacy daily candidate sampling, NOT a native deduction/refill adapter.
// Reading, provenance and baseline changes are explicit. A negative candidate
// is invalid, not zero; changing source, invalid reads and load reset baselines.
static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
{
    if (slot < 0 || slot >= VEH_MAX) return;
    using namespace private_logistics_fuel_diagnostics;
    int found = 0;
    const double slotRaw = FuelSlotTons(v, &found);
    const double displayRaw = MirrorTons(v);
    const Reading reading = Select(found != 0, slotRaw, displayRaw);
    const Result result = g_vehFuelSamples[slot].Observe(reading,
        (unsigned long long)InterlockedCompareExchange(&g_fuelDiagnosticEpoch, 0, 0));
    g_vehFuelValid[slot] = reading.valid;
    if (reading.valid) g_vehFuel[slot] = reading.value;
    // Legacy accumulators are candidate sample changes ONLY. No tracker feed.
    if (burned) *burned += result.decrease;
    if (refills && result.change == Change::Increase) ++*refills;
    if (g_tankCrossLog > 0) {
        Logf("[TANK] day %d v=%p selected_source=%s source_valid=%d "
             "slot_found=%d slot_candidate_raw=%.9g display_5f0_raw=%.9g "
             "drift_valid=%d drift_raw=%+.9g change=%s decrease_raw=%.9g "
             "units=unverified exact_burn=0 verified_refill=0 billing_input=0",
             closedDay, v, SourceName(reading.source), reading.valid ? 1 : 0,
             found, slotRaw, displayRaw, reading.comparable ? 1 : 0,
             reading.drift, ChangeName(result.change), result.decrease);
        --g_tankCrossLog;
    }
}

static void ScanVehicle(int slot, void* v, int closedDay)
{
    if (!g_vehScan || slot < 0 || slot >= VEH_MAX) return;
    if (!ReadablePtr((const BYTE*)v, VEH_SCAN_WORDS * 4)) return;

    const unsigned* src = (const unsigned*)v;

    if (!g_vehSnapValid[slot]) {
        for (int i = 0; i < VEH_SCAN_WORDS; i++) g_vehSnap[slot][i] = src[i];
        g_vehSnapValid[slot] = true;
        return;                                   // baseline, not a delta
    }

    int logged = 0;
    int changed = 0;
    for (int i = 0; i < VEH_SCAN_WORDS; i++) {
        const int off = i * 4;
        if (g_vehSnap[slot][i] == src[i]) continue;
        changed++;
        if (VehScanSkip(off)) { g_vehSnap[slot][i] = src[i]; continue; }
        if (logged < g_vehScanLogPerDay) {
            const unsigned a = g_vehSnap[slot][i];
            const unsigned b = src[i];
            float fa, fb;
            memcpy(&fa, &a, 4);
            memcpy(&fb, &b, 4);
            Logf("[VEH CHG] day %d v=%p +0x%03X %08X -> %08X (f %.4f -> %.4f)",
                 closedDay, v, off, a, b, (double)fa, (double)fb);
            logged++;
        }
        g_vehSnap[slot][i] = src[i];
    }
    if (changed > logged)
        Logf("[VEH CHG] day %d v=%p %d more word(s) changed, not logged",
             closedDay, v, changed - logged);
}

static void CensusFleets(int closedDay)
{
    if (!g_censusLog || !g_enterprise) return;
    if (!g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex) return;

    size_t n = g_enterprise->getEnterpriseCount();
    if (n == 0) return;
    if (n > 32) n = 32;                          // a republic, not a spreadsheet

    // v0.11: one close counted once, not once per building. L per fleet-day is
    // the rate the physics has; it must not double-count a fleet seen from two
    // enterprises.
    const unsigned vehDaysBefore = g_tankVehDaysRun;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr((const BYTE*)b + 0xC70, 16)) continue;

        const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
        const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
        if (!beg || !end || end < beg) continue;

        size_t cnt = (size_t)(end - beg) / sizeof(void*);
        if (cnt > 64) cnt = 64;
        if (!ReadablePtr(beg, cnt * sizeof(void*))) continue;

        char bname[64];
        BuildingName(b, bname, sizeof(bname));

        if (cnt == 0) {
            Logf("[VEH] day %d %s fleet: 0 vehicle(s)", closedDay, bname);
            continue;
        }
        Logf("[VEH] day %d %s fleet: %u vehicle(s)", closedDay, bname, (unsigned)cnt);

        for (size_t k = 0; k < cnt; k++) {
            void* v = ((void* const*)beg)[k];
            if (!v || !TsmIsValidGamePtr(v)) continue;

            void* home = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x4F8, 8))
                home = *(void* const*)((const BYTE*)v + 0x4F8);

            void* cat = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x510, 8))
                cat = *(void* const*)((const BYTE*)v + 0x510);

            // v0.12: the Ghidra report says the vehicle KIND is an int at
            // +0x50C, and that 0x42 marks a fuel consumer. +0x510, which this
            // census has been printing as a pointer, is the next field along -
            // which is why it kept printing nonsense like FFFFFFFF. If every
            // truck reads 66 here, that claim is confirmed; if none does, the
            // report has the wrong offset and we say so.
            int kind = -1;
            if (ReadablePtr((const BYTE*)v + 0x50C, 4))
                kind = *(const int*)((const BYTE*)v + 0x50C);

            int slots = 0;
            const double tons = CargoTons(v, &slots);
            CargoDump(VehSlot(v), v, closedDay);

            const int s = VehSlot(v);
            const bool first = (s >= 0 && g_vehSamples[s] == 0);
            if (s >= 0) {
                g_vehSamples[s]++;
                if (tons > g_vehPeakTons[s]) g_vehPeakTons[s] = tons;
            }

            // Retained raw words and legacy guess; disproven as road-truck
            // activity evidence in the supplied game test. Never gate on them.
            int state = -1, mode = -1;
            if (ReadablePtr((const BYTE*)v + 0x448, 4))
                state = *(const int*)((const BYTE*)v + 0x448);
            if (ReadablePtr((const BYTE*)v + 0x440, 4))
                mode  = *(const int*)((const BYTE*)v + 0x440);
            const bool active = (state == 1 || state == 2);
            if (active) g_activeVehDaysRun++; else g_idleVehDaysRun++;

            // Metadata capacity candidate used by the panel, NOT proof of a
            // native refill clamp or physical tonnes.
            const double cap = TankCapacity(v);
            if (cap > 0.0 && s >= 0 && !g_vehCapValid[s]) {
                g_vehCapValid[s] = 1;
                g_vehCap[s] = cap;
                Logf("[VEH] day %d v=%p capacity8684_raw=%.9g display_scale1000_estimate=%.2f "
                     "physical_units=unverified refill_clamp=unverified",
                     closedDay, v, cap, cap * 1000.0);
            }

            Logf("[VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) legacy_kind50c=%d catalogue510_candidate=%p peak %.3ft "
                 "legacy_state448=%d legacy_mode440=%d %s native_activity=unverified",
                 closedDay, v, (home == b) ? "this" : "other",
                 tons, slots, kind, cat, (s >= 0) ? g_vehPeakTons[s] : 0.0,
                 state, mode, active ? "legacy_active_guess" : "legacy_idle_guess");

            // One dump per vehicle, ever. +0x510 is the lead for capacity,
            // speed, price and consumption; this tells us what it actually is
            // before anything is built on it.
            // v0.10: fuel on board, and what left the tank since yesterday.
            double burned = 0.0;
            unsigned refills = 0;
            ReadTank(s, v, closedDay, &burned, &refills);
            g_burnTonsRun += burned;
            g_refillsRun  += refills;
            if (g_vehFuelValid[s]) g_tankVehDaysRun++;
            if (burned > 0.0 || refills > 0)
                Logf("[TANK SAMPLE] day %d v=%p candidate_raw=%.4f | decrease_raw=%.4f "
                     "| sample_increases=%u units=unverified exact_burn=0 verified_refill=0",
                     closedDay, v, g_vehFuelValid[s] ? g_vehFuel[s] : 0.0,
                     burned, refills);

            ScanVehicle(s, v, closedDay);

            if (first && cat && TsmIsValidGamePtr(cat) && ReadablePtr(cat, 32)) {
                const unsigned* d = (const unsigned*)cat;
                Logf("[VEH] day %d   cat[+0x00] %08X %08X %08X %08X | %08X %08X %08X %08X",
                     closedDay, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
            }
        }
    }

    // v0.11: this close saw at least one tank -> it counts as a fleet-day.
    if (g_tankVehDaysRun > vehDaysBefore) g_tankDaysRun++;
}

// ---------------------------------------------------------------------------
// entry points
// ---------------------------------------------------------------------------

#include "panel_fuel_runtime.h"

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_logistics";
    info->version = PL_VERSION;

    ReadSettings();
    if (!g_enabled) {
        Logf("private_logistics  disabled in the ini");
        return 1;
    }

    g_realTransfer = (tTransfer)(g_exeBase + TSM_RVA_TRANSFER);

    Logf("private_logistics  v%s - billing %s",
         PL_VERSION,
         g_dryRun ? "ARMED IN DRY RUN (computes every payment, moves nothing)"
                  : "LIVE (moves money)");
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_exeBase || g_exeSize == 0) {
        Logf("private_logistics  no executable base - refusing to start");
        return 1;
    }

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK,
                                                   TSM_SIM_CLOCK_VERSION);
    if (!g_simClock)
        Logf("private_logistics  WARN  sim_clock is not installed - "
             "the daily summary will not be printed");

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE,
                                                           TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        Logf("private_logistics  WARN  enterprise_base is not installed - "
             "no building can be identified as private and nothing will be paid");

    if (g_autoTargetsEnabled) {
        g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                       TSM_UI_PANELS_VERSION);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V5);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V4);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V3);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V2);
        if (g_uiPanels && TsmUiPanelsHasWidgets(g_uiPanels) &&
            g_uiPanels->registerWidgetProvider)
            g_stateStorageWidgetHandle =
                g_uiPanels->registerWidgetProvider(StateStorageWidgetProvider, nullptr);
        if (!g_stateStorageWidgetHandle)
            Logf("private_logistics  [AUTO DO] WARN state-storage checkbox unavailable; "
                 "ui_panels v2 widgets are required");
    }

    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                   TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                       TSM_CURRENCY_VERSION_V3);
    if (!g_currency)
        Logf("private_logistics  WARN  currency is not installed - "
             "the treasury leg of every payment is unavailable");

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                         TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                             TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing)
        Logf("private_logistics  WARN  resource_pricing is not installed - "
             "factory payments fall back to the raw Res+0x58 field, which read "
             "0.00 for fabric and plants in two sessions");

    if (g_billing && (!g_enterprise || !g_currency)) {
        g_billing = 0;
        Logf("private_logistics  billing DISABLED: it needs both enterprise_base "
             "and currency. Instrumentation continues normally.");
    }

    if (g_autoTargetsEnabled &&
        (!g_simClock || !g_enterprise || !g_enterprise->getEnterpriseCount ||
         !g_enterprise->getEnterpriseByIndex || !g_enterprise->isBuildingPrivate)) {
        g_autoTargetsEnabled = 0;
        Logf("private_logistics  [AUTO DO] DISABLED: requires sim_clock and "
             "enterprise_base discovery/private checks");
    }

    if (g_autoTargetsEnabled) {
        g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE,
                                                              TSM_PERSISTENCE_VERSION);
        if (g_persistence &&
            g_persistence->structSize >= offsetof(TsmPersistenceApi, registerHandler) +
                                         sizeof(g_persistence->registerHandler) &&
            g_persistence->registerHandler)
            g_persistenceHandle = g_persistence->registerHandler("private_logistics",
                                                                  AutoPersistenceEvent, nullptr);
        if (!g_persistenceHandle)
            Logf("private_logistics  [AUTO DO] WARN persistence load signal unavailable; "
                 "reconciliation after load may wait for date rewind or restart");
    }

    PanelObserverStart();

    PatchAllSites();

    if (g_simClock) g_simClock->registerDay(OnDay, nullptr);

    Logf("private_logistics  live v%s: billing=%d dry_run=%d factory=%d carrier=%d, "
         "%d sites, cap %.0f/day, price = %s, factory share %.2f",
         PL_VERSION, g_billing, g_dryRun, g_payFactory, g_payCarrier,
         g_siteCount, (double)g_dailyPayCap,
         g_tripTariff ? "LEGACY FRAGMENT MODEL (fuel x margin + wear), NOT journey settlement"
                      : "value share (v0.6 path, for comparison)",
         (double)g_factoryShare);

    Logf("private_logistics  [AUTO DO] enabled=%d dry_run=%d cadence=once/day; "
         "private endpoints -> private road/rail DOs; existing entries preserved",
         g_autoTargetsEnabled, g_autoTargetsDryRun);

    if (g_freightShareRetired > 0.0f)
        Logf("private_logistics  WARN freight_share = %.3f is RETIRED in v0.7 and was "
             "ignored - the legacy model prices transfer fragments, NOT journeys. Remove the key.",
             (double)g_freightShareRetired);

    // v0.5: name both indices before a single delivery is priced. A probe that
    // silently cannot resolve a price is worse than no probe, so the log says
    // which index is live and what it reads.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);

        Logf("private_logistics  v%s legacy fragment model: %.2f configured L/fragment + %.2f L/t, wear %.2f L, "
             "margin %.0f%%, density %.5f t/L, index '%s', auto-tune %s",
             PL_VERSION, gLitresPerTrip, gLitresPerTonne, gWearLitres,
             gTariffMargin * 100.0, gDieselDensity, gFuelResource,
             g_litresAuto ? "requested_but_blocked_unverified_samples" : "off");

        // The world is not loaded yet at plugin start, so both of these read
        // zero here and are re-read at every day close. Say that rather than
        // printing a scary NOT FOUND that resolves itself a second later.
        Logf("private_logistics  [IDX] at plugin start: fuel '%s' = %.2f, cpi basket %.2f - "
             "the world is still loading; both are re-read at every day close",
             gFuelResource, fp, (double)basket);

        Logf("private_logistics  legacy tariff is a MODEL per transfer fragment, NOT per journey; "
             "billing=%d dry_run=%d carrier_enabled=%d tariff_mode=%s. "
             "Only [PAY] reports payments; native journey settlement OFF.",
             g_billing, g_dryRun, g_payCarrier, g_tripTariff ? "model" : "value_share");
    }

    if (g_balanceLog) {
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
            have  = true;
        }
        if (have) {
            g_treasuryAtClose   = treas;
            g_treasuryHaveClose = false;   // first close prints a baseline
            Logf("private_logistics  [BAL] treasury opens at %.2f", treas);
        }
    }

    if (g_billing && g_dryRun)
        Logf("private_logistics  DRY RUN: every [PAY] line says 'would'. "
             "Set dry_run = 0 in the ini and restart to move the money.");
    else if (g_billing)
        Logf("private_logistics  LIVE MONEY: the state treasury pays. Every [PAY] "
             "line reports the amount, the payee and the shortfall gate.");

    Logf("private_logistics  send back every [PAY] line and the daily [PAY] totals");
    return 0;
}

````

### Final source: `plugins/private_logistics/private_logistics.ini`

SHA-256: `50a414a602b24f507e6286df6a0e25ec3e27ed8b0a1a3a9b710ddb19c27a68ed`

````ini
; ---------------------------------------------------------------------------
; private_logistics.ini - v0.18.8 read-only schedule observations
;
; Billing OFF. Dispatch and independent state permissions are unchanged.
; Native loaded-journey settlement remains OFF; its adapter is not yet wired.
; Existing tariff knobs describe a LEGACY MODEL PER TRANSFER FRAGMENT, not a
; journey or measured fuel cost. [TARIFF MODEL] totals do not report payments.
; Only [PAY] reports legacy payment outcomes if billing is explicitly enabled.
; Model: work = litres_per_trip + litres_per_tonne x fragment tonnes;
; fuel = work x diesel_density x fuel_price; wear = wear_litres x density x price;
; tariff = fuel x (1 + tariff_margin) + wear. No parameter is verified physics.
; Purchases, sampled decreases and increases cannot prove burn/refill events.
; See docs/private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md.
; ---------------------------------------------------------------------------

[private_logistics]
enabled             = 1

; PERMISSIONS TEST DEFAULT: no legacy freight payments. Target management
; remains active below. 1 explicitly opts into the old live billing algorithm.
billing             = 0
; 1 = count every payment and move nothing.
dry_run             = 1

; NEW NEXT-TEST DEFAULT: inspect the actual opened detail-panel object and
; candidate-home road-DO fleet membership; read-only, NOT exact burn or billing.
; Requires matching executable bytes, sim_clock and persistence load signal.
; One snapshot/second, 240 accepted records/load epoch, 16 queued.
; Paused captures keep the latest queued snapshot of the same window/root.
; [PANEL GAP] explicitly reports omitted paused snapshots, queue overflow,
; lock contention and capture stop/fault. Accepted sample IDs are NOT coverage.
; Capture-time and drain-time queue-full counters are separate.
panel_fuel_observer = 1

; Read-only schedule observations on the SAME verified panel hook/queue above.
; Additional native fingerprints and unique road-DO fleet membership required.
; At most 16 raw rows; +5FD's special last row has no config-field reads.
; Changes are sampled stop/config/context observations, NOT whole-job boundaries,
; success/cancellation reasons, buyer identity or a billing input.
schedule_observer = 1

; H1: the private building the goods LEAVE is paid = value x factory_share.
; This leg only fires on a privately owned producer. If it reads 0.00 every
; day, no building in the chain is private - check for a [PRIV CLICK] line.
pay_factory         = 1
factory_share       = 0.90

; Legacy carrier model: one price per eligible transfer fragment, NOT journey.
pay_carrier         = 1

; 1 = use the legacy fragment tariff model above when billing is enabled.
;       0 = charge 8 % of cargo value instead - the v0.6 path, kept only so
;           the two can be compared inside one session.
trip_tariff         = 1

; ---------------------------------------------------------------------------
; the tariff, in physical units
; ---------------------------------------------------------------------------

; Legacy model estimate retained unchanged for compatibility. It was derived
; from sampled fleet endpoints, NOT verified clamped native deductions.
; Do not calibrate it from [TANK SAMPLE], purchases, or a screenshot difference.
litres_per_trip     = 14.75

; Legacy model default only. Native cargo-load dependence is unverified.
litres_per_tonne    = 0.0

; v0.7: the vehicle sinking fund, denominated in litres of diesel so it
; inflates WITH fuel instead of behind it. This is POLICY, not physics: it is
; what you decide a trip should put aside towards the next truck.
;
; Set it from a truck, not from taste:
;     wear_litres = truck_price / trips_per_life / (density x fuel_price)
; A 6,000 RUB truck over 4,000 trips is 1.50 RUB/trip, about 13 L at 133/RUB.
; 5.00 is deliberately conservative - raise it and the DO saves for vehicles.
wear_litres         = 5.00

; Auto-tune is blocked even if requested: candidate sample changes/units are
; unverified and cannot calibrate physical fuel cost. Default remains OFF.
litres_auto         = 0

diesel_density      = 0.00100   ; legacy model t/L; NOT proof of native mass units.

; The carrier's profit, as a fraction of fuel cost. Applies to fuel only;
; the wear reserve is a cost and is passed through at cost.
tariff_margin       = 0.35

; The index. 'fuel' is resolved by NAME through resource_pricing every day,
; so the tariff follows the game's own diesel price. If it cannot resolve,
; the same work is priced in daily survival baskets instead, and if neither
; resolves the delivery is reported UNINDEXED and priced at nothing -
; this build never invents a figure in money.
fuel_resource       = fuel
baskets_per_trip    = 0.80
baskets_per_tonne   = 0.02

; Per-delivery [TARIFF] lines; the day total is always printed.
tariff_log_per_day  = 60

; v0.8: the vehicle census. Once a day every private enterprise's fleet is
; walked (bld+0xC70/+0xC78, proven) and each vehicle is logged with what it
; carries now and at its peak. The first sighting of a vehicle also dumps the
; 8 dwords at the pointer the game gives for its catalogue entry (+0x510) -
; that is where capacity, speed, price and consumption live.
;
; Nothing is priced from the census yet. One number cannot describe a fleet,
; and this build refuses to pretend otherwise; the census is how the next one
; stops needing to.
census_log          = 1

; v0.9: the field scanner. Each vehicle's first 0x800 bytes are copied on first
; sight; every day after, only the words that CHANGED are logged:
;
;   [VEH CHG] day 12 v=0000... +0x6D4 000001C8 -> 0000019A (f 456.00 -> 410.00)
;
; A fuel tank is a word that falls while the truck drives and jumps when it
; refuels. 456 above is the kind of value to look for - litres, not roubles.
; Known pointer ranges are skipped, and at most veh_scan_log_per_day words are
; printed per vehicle per day.
veh_scan            = 1
veh_scan_log_per_day = 40

; v0.12: raw storage-vector dump for up to three vehicles per close. BOTH
; vectors - the trailer vector at veh+0x3A0 and the cargo vector at veh+0x3B8 -
; every slot, with the resource pointer and the tonnage. If diesel lives in a
; slot, one slot will fall every day and jump at a refill, exactly like
; [TANK]. Set to 0 once that is settled.
cargo_dump          = 1

; Candidate diagnostics only. Each [TANK] record explicitly states selected
; source (slot_candidate / display_5f0_fallback / unavailable), validity, raw
; candidates, independent drift validity and sampled change classification.
; Fallback is NEVER compared to itself. Invalid/negative values are not zero;
; source changes, invalid reads and load epochs break sample continuity.
; No native clamp, actual burn/refill or physical mass unit is claimed.
tank_cross_log      = 40

; ---------------------------------------------------------------------------
; instrument
; ---------------------------------------------------------------------------

; [BAL] treasury movement, the carriers' capital and the fuel meter, per day.
; The meter lives here, so balance_log = 0 also disables the measured fuel.
balance_log         = 1
; one [B2B SITE] line per transfer call
site_log            = 1
; cap on [B2B]/[PAY]/[TARIFF] detail lines per day
log_cap_per_day     = 400
; shipment ring buffer
ledger_capacity     = 512

; Absolute payment cap per day, in the active currency. A runaway loop should
; show up as a cap line, not as an emptied treasury.
daily_pay_cap       = 5000.0
; below this, a payment is skipped and counted, never rounded up.
min_payment         = 0.01

; Sites to hook. Empty = all 16. "0x68F792,68F7F9" for a subset.
site_list           =

; Autonomous private DO connections; state site access requires explicit
; per-building direction permissions. State retail: deliveries only. State
; factory/storage: independent loading and deliveries. Unknown roles deny.
; Private endpoints follow their role without player access checkboxes.
; Missing eligible targets are re-added at the daily scan (not instantly).
auto_targets_enabled = 1
; 1 = report plans only (no TaskCfg writes/appends); 0 = apply permissions.
auto_targets_dry_run = 0
; state_storage_always_on is obsolete: any old value is ignored and logged.
; Maximum target-detail lines per daily scan; summary lines are always printed.
auto_targets_log_limit = 80

; ---------------------------------------------------------------------------
; NOT IMPLEMENTED - declared so the intent is on record, not to work yet
; ---------------------------------------------------------------------------
; 0 = the DO pays for diesel through the game's own foreign purchase, which
; is what happens today and what the v0.6 reconciliation measured.
charge_fuel_at_pump      = 0
charge_vehicle_purchase  = 1
upkeep_per_vehicle_day   = 0.0     ; a private DO pays no upkeep: vehicles + fuel

; who pays when the receiving building cannot be identified
unknown_payer            = state

; Company policy for automatic tasks, applied to both new and existing entries
; at daily reconciliation. Native per-DO/per-task edits are overwritten for
; managed targets. Manually assigned customs tasks remain untouched.
; Source 10% matches the working manual setting reported in the latest test.
; This is an empirical test policy, NOT a proven native ceiling or dispatch fix.
; Load when source stock is ABOVE this percentage (policy range 0..90).
auto_source_percent = 10
; Deliver when destination stock is BELOW this percentage (policy range 10..100).
; This is a dispatch trigger, NOT a spending limit or exact final-stock cap.
auto_destination_percent = 30

````

### Final source: `tests/private_logistics/panel_fuel_runtime_test.cpp`

SHA-256: `942e623f7061fbbe6704891e6ee68426b3228140f5aea2641f296027e4025ebe`

````cpp
// Native host shim exercises the production queue/control code. NOT Windows ABI,
// SEH, patch execution, real threads, or native game lifecycle validation.
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include <cstdarg>

using BYTE = unsigned char;
using LONG = long; // host-shim log ABI; real Windows LONG is 32-bit
using ULONGLONG = unsigned long long;
struct CRITICAL_SECTION { bool busy=false; };
static bool TryEnterCriticalSection(CRITICAL_SECTION* p) { if(p->busy)return false;p->busy=true;return true; }
static void LeaveCriticalSection(CRITICAL_SECTION* p) { CHECK(p->busy);p->busy=false; }
static void InitializeCriticalSection(CRITICAL_SECTION* p) { p->busy=false; }
static LONG InterlockedCompareExchange(volatile LONG* p,LONG value,LONG expected) { LONG old=*p;if(old==expected)*p=value;return old; }
static LONG InterlockedExchange(volatile LONG* p,LONG value) { LONG old=*p;*p=value;return old; }
static LONG InterlockedIncrement(volatile LONG* p) { return ++*p; }
static ULONGLONG clockMs=1000;
static ULONGLONG GetTickCount64() { return clockMs; }
static std::vector<BYTE> image(0x90B080);
static BYTE* g_exeBase=image.data();
static std::size_t g_exeSize=image.size();
static bool throwOnRead=false;
static uintptr_t throwAtRead=0;
static bool ReadablePtr(const void* p,std::size_t n) {
    if(throwOnRead || (throwAtRead && (uintptr_t)p==throwAtRead)) throw 1;
    const auto a=(uintptr_t)p,b=(uintptr_t)g_exeBase;
    return a>=b && a-b<=image.size() && n<=image.size()-(a-b);
}
constexpr unsigned IMAGE_DOS_SIGNATURE=0x5A4D,IMAGE_NT_SIGNATURE=0x4550;
constexpr unsigned IMAGE_FILE_MACHINE_AMD64=0x8664,IMAGE_NT_OPTIONAL_HDR64_MAGIC=0x20B;
struct IMAGE_DOS_HEADER { unsigned e_magic; int e_lfanew; };
struct IMAGE_NT_HEADERS64 {
    unsigned Signature;
    struct { unsigned Machine, TimeDateStamp; } FileHeader;
    struct { unsigned Magic; } OptionalHeader;
};
static std::vector<std::string> logs;
#if defined(__GNUC__) || defined(__clang__)
static void Logf(const char*,...) __attribute__((format(printf,1,2)));
#endif
static void Logf(const char* format,...) {
    char line[2048];va_list args;va_start(args,format);std::vsnprintf(line,sizeof(line),format,args);va_end(args);logs.emplace_back(line);
}
static int g_panelObserverEnabled=1,g_scheduleObserverEnabled=0,g_billing=0,g_persistenceHandle=1;
static unsigned installed=0,originalCalls=0;
static void Original(void*,void*) { ++originalCalls; }
static bool InstallInlineHook(void*,void*,void** trampoline,const BYTE*,std::size_t n,const char*) {
    CHECK(n==20);*trampoline=(void*)Original;++installed;return true;
}
using Tick=void(*)(void*,float,float);
static Tick tick=nullptr;
static int RegisterTick(Tick fn,void*) { tick=fn;return 1; }
static bool paused=false;
static int IsPaused() { return paused ? 1 : 0; }
struct ClockApi { int(*registerTick)(Tick,void*); int(*isPaused)(); };
static ClockApi api{RegisterTick,IsPaused};
static ClockApi* g_simClock=&api;
#define __fastcall
#define __try try
#define __except(x) catch(...)
#include "panel_fuel_runtime.h"

#ifndef PANEL_RUNTIME_TEST_NO_MAIN
int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    for(const auto& f:g_scheduleFingerprints) std::memcpy(g_exeBase+f.rva,f.bytes,f.size);
    CHECK(ScheduleTargetMatches());
    g_exeBase[0x7E8438]^=1;CHECK(!ScheduleTargetMatches());g_exeBase[0x7E8438]^=1;
    PanelObserverStart();CHECK(installed==1 && tick && g_panelStarted);
    std::puts("PASS runtime_install_fingerprint");
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    const auto logged=logs.size();PanelRendererDetour(context,window);
    CHECK(originalCalls==1 && g_panelCount==1 && logs.size()==logged);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    tick(nullptr,1,1);CHECK(g_panelCount==0 && logs.size()==logged+2);
    CHECK(logs[logged].find("litres_candidate=549.54")!=std::string::npos);
    CHECK(logs[logged+1].find("exact_fleet_pointer")!=std::string::npos);
    std::puts("PASS runtime_post_render_throttle_and_deferred_log");
    paused=true;
    const auto pausedLogs=logs.size();
    for(unsigned i=0;i<72;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==1 && g_panelDropped==0 && g_panelSamples==2);
    CHECK(g_panelQueue[g_panelHead].coalesced==71 && logs.size()==pausedLogs);
    paused=false;clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2);tick(nullptr,1,1);
    CHECK(logs[pausedLogs].find("reason=paused_snapshot_coalescing")!=std::string::npos);
    CHECK(logs[pausedLogs].find("discarded_paused_snapshots=71")!=std::string::npos);
    CHECK(logs[pausedLogs+1].find("captured_paused=1")!=std::string::npos);
    tick(nullptr,1,1);CHECK(g_panelCount==0);
    std::puts("PASS runtime_long_pause_coalesces_and_reports_omissions");
    PanelObserverLoading();tick(nullptr,1,1);paused=true;
    clockMs+=1000;PanelCapture(context,window);
    put(fixture.window+0x240,ptr(0x90000)); // another root, no assumed native metadata
    clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2 && g_panelQueue[g_panelHead].snapshot.root==ptr(fixture.root));
    CHECK(g_panelQueue[(g_panelHead+1)%16].snapshot.root==ptr(0x90000));
    std::memcpy(g_exeBase+0xB0000,g_exeBase+fixture.window,0x500);
    clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==3);
    paused=false;clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==4);
    put(fixture.window+0x240,ptr(fixture.root));
    PanelObserverLoading();tick(nullptr,1,1);
    std::puts("PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record");
    for(unsigned i=0;i<17;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==16 && g_panelDropped==1);
    const auto fullLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[fullLogs].find("reason=queue_full")!=std::string::npos);
    CHECK(logs[fullLogs].find("skipped_snapshot_captures=1")!=std::string::npos);
    CHECK(logs[fullLogs+1].find("queue_full_total_at_capture=0 queue_full_total_at_drain=1")!=std::string::npos);
    const auto afterGap=logs.size();tick(nullptr,1,1);
    CHECK(logs[afterGap].find("[PANEL FUEL]")!=std::string::npos); // no duplicate gap
    g_panelQueueLock.busy=true;clockMs+=1000;PanelCapture(context,window);
    g_panelQueueLock.busy=false;const auto contentionLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[contentionLogs].find("reason=render_trylock_contention")!=std::string::npos);
    PanelObserverLoading();const auto oldLogs=logs.size();tick(nullptr,1,1);
    CHECK(g_panelCount==0 && g_panelSamples==0 && logs.size()==oldLogs);
    std::puts("PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard");
    clockMs+=1000;PanelCapture(context,nullptr);CHECK(g_panelLastMs==0);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    g_panelSamples=239;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    const auto capLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[capLogs].find("reason=capture_cap cap=240 capture=stopped")!=std::string::npos);
    tick(nullptr,1,1);const auto drainedLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs.size()==drainedLogs);
    std::puts("PASS runtime_inactive_does_not_starve_capture_and_cap");
    PanelObserverLoading();tick(nullptr,1,1);clockMs+=1000;
    throwOnRead=true;PanelCapture(context,window);throwOnRead=false;
    CHECK(g_panelFault && g_panelCount==0);
    const auto faultLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[faultLogs].find("reason=guarded_read_fault observer=disabled")!=std::string::npos);
    const auto disabledLogs=logs.size();clockMs+=1000;PanelCapture(context,window);tick(nullptr,1,1);
    CHECK(g_panelCount==0 && logs.size()==disabledLogs);
    std::puts("PASS runtime_guarded_fault_disables_and_reports_once");
    g_exeBase[0x90B07C]=1;CHECK(!PanelTargetMatches());g_exeBase[0x90B07C]=0;
    g_exeBase[0x8F3E98]=0;CHECK(!PanelTargetMatches());
    std::puts("PASS runtime_changed_data_refused");
    std::puts("8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}

#endif

````

### Final source: `tests/private_logistics/schedule_observer_test.cpp`

SHA-256: `0876f5b7f5ef48bcab288f24950f70fa6ebd86ec4b99135275fdc784100ce9e2`

````cpp
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include "schedule_observer.h"
namespace sch=private_logistics_schedule;
struct ScheduleFixture : Fixture {
    Address targets=0xB0000,configs=0xC0000;
    ScheduleFixture() {
        Add(targets,sch::MaxRows*8);Add(configs,sch::MaxRows*sch::ConfigStride);
        Set(root,0x680,targets);Set(root,0x688,targets+16);Set(root,0x638,configs);
        Set(root,0x698,std::int32_t(1));Set(root,0x610,Address(0x123456));
        Set(root,0x650,Address(0xFEDCBA));Set(root,0x4A0,Address(0xFA000));
        Set(targets,0,Address(0xDEAD0000));Set(targets,8,Address(0xBEEF0000));
        Set(configs,0,std::uint8_t(1));Set(configs,4,1.0f);
        Set(configs,sch::ConfigStride+0xC8,std::uint8_t(1));
        Set(configs,sch::ConfigStride+0xCC,0.5f);
    }
    sch::Snapshot Now() {sch::Snapshot s;sch::Capture(Mem(),root,s);return s;}
};
TEST(schedule_selected_fields_mapping_no_pointee_following) {
    ScheduleFixture f;auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==2);
    CHECK(s.index698==1 && s.rows[0].target==0xDEAD0000 && s.rows[1].target==0xBEEF0000);
    CHECK(s.rows[0].configRead && s.rows[0].flag00==1 && s.rows[0].value04==1.0f);
    CHECK(s.rows[1].flagC8==1 && s.rows[1].valueCC==0.5f && s.raw650==0xFEDCBA);
}
TEST(schedule_special_last_row_does_not_read_config) {
    ScheduleFixture f;f.Set(f.root,0x5FD,std::uint8_t(1));
    f.denied.push_back({f.configs+sch::ConfigStride,sch::ConfigStride});
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.rows[0].configRead);
    CHECK(!s.rows[1].configRead && s.rows[1].target==0xBEEF0000);
    f.Set(f.root,0x688,f.targets+8);f.Set(f.root,0x638,Address(0));
    s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==1 && !s.rows[0].configRead);
}
TEST(schedule_empty_list_is_valid_not_completion) {
    ScheduleFixture f;f.Set(f.root,0x680,Address(0));f.Set(f.root,0x688,Address(0));
    f.Set(f.root,0x638,Address(0));auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==0);
    f.Set(f.root,0x680,f.targets);f.Set(f.root,0x688,f.targets);CHECK(f.Now().status==sch::Status::Complete);
}
TEST(schedule_null_target_and_unknown_index_preserved) {
    ScheduleFixture f;f.Set(f.targets,0,Address(0));f.Set(f.root,0x698,std::int32_t(-7));
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && !s.rows[0].target && s.index698==-7);
    f.Set(f.root,0x698,std::int32_t(999));CHECK(f.Now().index698==999);
}
TEST(schedule_missing_header_and_config_fail_closed) {
    ScheduleFixture f;f.denied.push_back({f.root+0x698,4});CHECK(f.Now().status==sch::Status::Unreadable);
    f.denied.clear();f.Set(f.root,0x638,Address(0));CHECK(f.Now().status==sch::Status::MissingConfigs);
    f.Set(f.root,0x638,f.configs);f.denied.push_back({f.configs+4,4});CHECK(f.Now().status==sch::Status::Unreadable);
}
TEST(schedule_bad_bounds_and_oversize_do_not_walk) {
    ScheduleFixture f;f.Set(f.root,0x688,f.targets-8);CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x688,f.targets+7);CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x680,Address(0));CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x680,f.targets);f.Set(f.root,0x688,f.targets+17*8);f.reads=0;
    CHECK(f.Now().status==sch::Status::TooManyRows && f.reads==8);
}
TEST(schedule_max_rows_and_checked_arithmetic) {
    ScheduleFixture f;f.Set(f.root,0x688,f.targets+sch::MaxRows*8);
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==sch::MaxRows);
    f.Set(f.root,0x638,UINTPTR_MAX-2);CHECK(f.Now().status==sch::Status::Unreadable);
    sch::Capture(f.Mem(),UINTPTR_MAX-0x100,s);CHECK(s.status==sch::Status::Unreadable);
}
TEST(schedule_nonfinite_no_partial_table) {
    ScheduleFixture f;f.Set(f.configs,sch::ConfigStride+0xCC,std::numeric_limits<float>::quiet_NaN());
    CHECK(f.Now().status==sch::Status::NonFinite);
    f.Set(f.configs,sch::ConfigStride+0xCC,std::numeric_limits<float>::infinity());
    CHECK(f.Now().status==sch::Status::NonFinite);
}
struct Mutation {
    ScheduleFixture f;
    Address address=0;unsigned hits=0;
    static bool Read(Address a,void* out,std::size_t n,void* user) {
        auto& m=*static_cast<Mutation*>(user);
        if(a==m.address && ++m.hits==2) {
            if(a==m.f.root+0x698)m.f.Set(m.f.root,0x698,std::int32_t(2));
            else m.f.Set(m.f.configs,4,0.3f);
        }
        return Fixture::Reader(a,out,n,&m.f);
    }
};
TEST(schedule_header_and_row_mutation_rejected) {
    Mutation m;sch::Snapshot s;m.address=m.f.root+0x698;
    sch::Capture({Mutation::Read,&m},m.f.root,s);CHECK(s.status==sch::Status::ChangedDuringRead);
    m.address=m.f.configs+4;m.hits=0;
    sch::Capture({Mutation::Read,&m},m.f.root,s);CHECK(s.status==sch::Status::ChangedDuringRead);
}
TEST(schedule_history_first_unchanged_and_index_only) {
    ScheduleFixture f;auto s=f.Now();sch::History h;
    auto d=h.Observe(s,1,f.root,f.home);CHECK(d.emit && d.baseline==sch::Baseline::First && !d.fields);
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
    s.index698=0;d=h.Observe(s,1,f.root,f.home);CHECK(d.emit && d.index && !d.fields && !d.storage);
    CHECK(d.baseline==sch::Baseline::None); // row advance, NOT job completion
}
TEST(schedule_history_field_storage_and_context_changes_separate) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.rows[0].value04=0.2f;auto d=h.Observe(s,1,f.root,f.home);CHECK(d.fields && !d.index && !d.storage);
    s.configs+=0x10000;d=h.Observe(s,1,f.root,f.home);CHECK(!d.fields && d.storage && !d.index);
    s.raw650=1;d=h.Observe(s,1,f.root,f.home);CHECK(d.context && !d.fields && !d.index);
}
TEST(schedule_history_list_clear_replacement_are_observed_fields_only) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.rows[1].target=0xEEEE;CHECK(h.Observe(s,1,f.root,f.home).fields);
    s.count=0;CHECK(h.Observe(s,1,f.root,f.home).fields);
    // No success/cancel reason/job generation exists in this class.
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
}
TEST(schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.index698=0;auto d=h.Observe(s,2,f.root,f.home);CHECK(d.baseline==sch::Baseline::Epoch && !d.index);
    d=h.Observe(s,2,f.root+1,f.home);CHECK(d.baseline==sch::Baseline::Identity);
    d=h.Observe(s,2,f.root+1,f.home+1);CHECK(d.baseline==sch::Baseline::Identity);
    h.Break();s.count=0;d=h.Observe(s,2,f.root+1,f.home+1);CHECK(d.baseline==sch::Baseline::Gap && !d.fields);
    s.status=sch::Status::ChangedDuringRead;CHECK(h.Observe(s,2,f.root,f.home).baseline==sch::Baseline::Invalid);
    s=f.Now();d=h.Observe(s,2,f.root,f.home);CHECK(d.baseline==sch::Baseline::Gap && !d.fields);
}
TEST(schedule_identical_visible_schedule_is_not_new_job) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    // Even if an unseen cancellation/reassignment happened, equal selected
    // fields cannot reveal it. No synthesized generation counter or bill.
    for(unsigned i=0;i<1000;++i) CHECK(!h.Observe(s,1,f.root,f.home).emit);
}
int main(int argc,char** argv) {
    unsigned count=0;
#define RUN(name) do {if(argc==1 || std::string(argv[1])==#name){name();++count;std::printf("PASS %s\n",#name);}} while(0)
    RUN(schedule_selected_fields_mapping_no_pointee_following);
    RUN(schedule_special_last_row_does_not_read_config);
    RUN(schedule_empty_list_is_valid_not_completion);
    RUN(schedule_null_target_and_unknown_index_preserved);
    RUN(schedule_missing_header_and_config_fail_closed);
    RUN(schedule_bad_bounds_and_oversize_do_not_walk);
    RUN(schedule_max_rows_and_checked_arithmetic);
    RUN(schedule_nonfinite_no_partial_table);
    RUN(schedule_header_and_row_mutation_rejected);
    RUN(schedule_history_first_unchanged_and_index_only);
    RUN(schedule_history_field_storage_and_context_changes_separate);
    RUN(schedule_history_list_clear_replacement_are_observed_fields_only);
    RUN(schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas);
    RUN(schedule_identical_visible_schedule_is_not_new_job);
    CHECK(count);std::printf("%u schedule host scenarios PASS (not native lifecycle validation)\n",count);
}

````

### Final source: `tests/private_logistics/schedule_runtime_test.cpp`

SHA-256: `2ec7494f84211d0a2f77496a2d6ce4e82be9c33f8fe19a8cabfc7aba73adaad2`

````cpp
// Production runtime through the same portable shim. No DLL/SEH/thread proof.
#define PANEL_RUNTIME_TEST_NO_MAIN
#include "panel_fuel_runtime_test.cpp"

int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    const auto put32=[&](Address a,int v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    const auto putFloat=[&](Address a,float v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    for(const auto& f:g_scheduleFingerprints)std::memcpy(g_exeBase+f.rva,f.bytes,f.size);
    g_scheduleObserverEnabled=1;
    g_exeBase[0x7E8438]^=1;PanelObserverStart();CHECK(g_panelStarted && !g_scheduleReady && installed==1);
    g_exeBase[0x7E8438]^=1; // no second hook/start; emulate fresh process gate result for remainder
    g_scheduleReady=ScheduleTargetMatches();CHECK(g_scheduleReady);
    std::puts("PASS schedule_runtime_mismatch_disables_schedule_not_fuel");
    Address targets=0xD0000,configs=0xD1000;
    put(fixture.root+0x680,ptr(targets));put(fixture.root+0x688,ptr(targets+16));put(fixture.root+0x638,ptr(configs));
    put(targets,Address(0xDEAD0000));put(targets+8,Address(0xBEEF0000));
    putFloat(configs+4,1.0f);putFloat(configs+0x198+0xCC,0.5f);
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    auto capture=[&](){clockMs+=1000;PanelCapture(context,window);};
    auto drain=[&](){tick(nullptr,1,1);};
    auto contains=[&](const char* text){for(const auto& l:logs)if(l.find(text)!=std::string::npos)return true;return false;};
    logs.clear();capture();CHECK(logs.empty());CHECK(g_panelCount==1);
    CHECK(g_panelQueue[g_panelHead].schedule.status==private_logistics_schedule::Status::Complete);
    drain();CHECK(logs.size()==5 && contains("baseline=first_observation") && contains("target_raw=0xdead0000"));
    CHECK(contains("native_job_id=unverified buyer=unverified completion=unverified"));
    std::puts("PASS schedule_runtime_same_post_render_record_deferred_rows_and_truthful_labels");
    logs.clear();capture();drain();CHECK(logs.size()==2); // unchanged: fuel/object only
    put32(fixture.root+0x698,1);logs.clear();capture();drain();
    CHECK(logs.size()==5 && contains("observed_fields_changed=0 storage_changed=0 row_index_changed=1"));
    std::puts("PASS schedule_runtime_unchanged_suppressed_and_row_index_not_job_end");
    paused=true;capture();putFloat(configs+4,0.25f);capture();
    CHECK(g_panelCount==1 && g_panelQueue[g_panelHead].schedule.rows[0].value04==0.25f);
    logs.clear();paused=false;drain();CHECK(contains("baseline=diagnostic_gap") && contains("value04=0.25"));
    std::puts("PASS schedule_runtime_paused_coalescing_carries_latest_schedule_and_rebaselines");
    PanelObserverLoading();drain();logs.clear();capture();drain();CHECK(contains("baseline=load_epoch"));
    put(fixture.fleet,Address(0));logs.clear();capture();drain();
    CHECK(contains("status=not_unique_road_fleet_root") && !contains("[PANEL SCHEDULE ROW]"));
    put(fixture.fleet,ptr(fixture.root));logs.clear();capture();drain();CHECK(contains("baseline=diagnostic_gap"));
    put(fixture.fleet+8,ptr(fixture.root));put(fixture.home+0xC78,ptr(fixture.fleet+16));
    logs.clear();capture();drain();CHECK(contains("status=not_unique_road_fleet_root"));
    put(fixture.home+0xC78,ptr(fixture.fleet+8));
    std::puts("PASS schedule_runtime_load_absent_and_duplicate_fleet_fail_closed");
    // Explicitly cover delayed capture-drop counter advance after a drain-side report.
    logs.clear();capture();drain();g_scheduleHistory.Break();capture();drain();
    ++g_panelDropped;drain(); // report at drain BEFORE the next accepted capture
    logs.clear();capture();drain();CHECK(contains("baseline=diagnostic_gap"));
    std::puts("PASS schedule_runtime_capture_drop_counter_advance_rebaselines");
    // Huge raw list is not a partial/empty schedule. Fuel continues unchanged.
    put(fixture.root+0x688,ptr(targets+17*8));logs.clear();capture();drain();
    CHECK(contains("status=too_many_rows_no_partial_table") && !contains("[PANEL SCHEDULE ROW]") && contains("[PANEL FUEL]"));
    put(fixture.root+0x688,ptr(targets+16));put(fixture.root+0x638,Address(1));
    logs.clear();capture();drain();CHECK(contains("status=unreadable") && !g_panelFault);
    put(fixture.root+0x638,ptr(configs));
    std::puts("PASS schedule_runtime_invalid_schedule_never_blocks_fuel_or_prints_partial_rows");
    // Inject a C++ exception through the shim at a schedule-only address.
    // This exercises the production isolated catch/latch, NOT Windows SEH.
    throwAtRead=ptr(fixture.root+0x698);logs.clear();capture();throwAtRead=0;drain();
    CHECK(g_scheduleFault && contains("guarded_schedule_fault_disabled") && contains("[PANEL FUEL]") && !g_panelFault);
    logs.clear();capture();drain();CHECK(contains("guarded_schedule_fault_disabled") && !g_panelFault);
    std::puts("PASS schedule_runtime_fault_latch_isolated_from_fuel_not_a_windows_seh_test");
    std::puts("8 schedule runtime host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");
}

````

### Final source: `tests/private_logistics/test_schedule_observer.py`

SHA-256: `08f8665fc43384e5d86d2ed4e8274bec6068e4dcc1e5a4dd0229bb0914d7096b`

````python
"""Host/source tests for sampled schedule observations, NOT native job evidence."""
from pathlib import Path
import configparser
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'
HARNESS = Path(__file__).with_name('schedule_observer_test.cpp')

class ScheduleObserverTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.exe = Path(cls.directory.name) / 'schedule_test'
        result = subprocess.run([cls.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(HARNESS),
                                 '-o', str(cls.exe)], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def run_case(self, name):
        result = subprocess.run([str(self.exe), name], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + name, result.stdout)

    def test_production_runtime_enabled(self):
        exe = Path(self.directory.name) / 'runtime_test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN),
                                 str(HARNESS.with_name('schedule_runtime_test.cpp')),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('8 schedule runtime host-shim scenarios PASS', result.stdout)

    def test_native_fingerprints_match_existing_raw_bytes(self):
        raw = (ROOT / 'docs/private-logistics-runtime/RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md').read_bytes().decode('utf-16')
        instructions = {}
        for line in raw.splitlines():
            m = re.match(r'^(140[0-9a-f]+):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+', line)
            if m:
                instructions[int(m[1], 16)] = bytes.fromhex(m[2])
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        records = re.findall(r'\{(0x[0-9A-F]+), (\d+), \{([^}]+)\}\}', runtime)
        self.assertEqual(len(records), 13)
        for rva, size, values in records:
            expected = bytes(int(x, 16) for x in re.findall(r'0x[0-9A-F]{2}', values))
            address = int(rva, 16) + 0x140000000
            end = address + int(size)
            actual = b''
            while address < end:
                chunk = instructions[address]
                actual += chunk
                address += len(chunk)
            self.assertEqual(address, end)  # whole instructions
            self.assertEqual(len(expected), int(size))
            self.assertEqual(actual, expected)

    def test_defaults_and_read_only_separation(self):
        config = configparser.ConfigParser()
        config.read(PLUGIN / 'private_logistics.ini')
        cpp = (PLUGIN / 'private_logistics.cpp').read_text()
        runtime = (PLUGIN / 'panel_fuel_runtime.h').read_text()
        header = (PLUGIN / 'schedule_observer.h').read_text()
        self.assertEqual(config.getint('private_logistics', 'schedule_observer'), 1)
        self.assertIn('static int g_scheduleObserverEnabled = 1;', cpp)
        self.assertIn('H->configInt(ini, "private_logistics", "schedule_observer", 1)', cpp)
        for forbidden in ('modifyCompanyCapital', 'private_logistics_journey::Tracker',
                          'WriteProcessMemory', 'FuelSlotTons(', 'reinterpret_cast', 'new ',
                          'std::vector', 'std::string'):
            self.assertNotIn(forbidden, header)
        self.assertIn('if (s.status != private_logistics_schedule::Status::Complete) return;', runtime)
        self.assertIn('native_job_id=unverified buyer=unverified', runtime)
        self.assertIn('completion=unverified exact_burn=0 billing=off', runtime)
        self.assertIn('g_scheduleObserverEnabled && ScheduleTargetMatches()', runtime)
        self.assertEqual(runtime.count('InstallInlineHook('), 1)  # same existing hook

    def test_windows_min_max_macros(self):
        path = Path(self.directory.name) / 'macros.cpp'
        path.write_text('#include <cstddef>\n#include <cstdint>\n#include <cmath>\n'
                        '#define max(a,b) ((a)>(b)?(a):(b))\n#define min(a,b) ((a)<(b)?(a):(b))\n'
                        '#include "schedule_observer.h"\nint main(){return 0;}\n')
        result = subprocess.run([self.compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                                 '-I', str(PLUGIN), str(path), '-o', str(Path(self.directory.name) / 'macros')],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    def run(self, case=name):
        self.run_case(case)
    setattr(ScheduleObserverTest, 'test_' + name, run)

if __name__ == '__main__':
    unittest.main()

````

### Final source: `tools/audit_vehicle_schedule_export.py`

SHA-256: `121e8788d41f26abb77a87482ad4a2b3f7955f44bbdf2da8449b78a93a072c83`

````python
#!/usr/bin/env python3
"""Bounded local reaching-definition audit of the supplied renderer export.
Requires capstone==5.0.7. Run from repo root. No executable, hook, simulation
writer or native lifecycle is validated. Calls are Windows x64 ABI-preserving;
only direct RBP/RSP slot writes are tracked (not arbitrary escaped aliases).
"""
from pathlib import Path
import re,collections,hashlib
from capstone import *
from capstone.x86 import *
p=Path('docs/private-logistics-runtime/RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md');raw=p.read_bytes();text=raw.decode('utf-16')
rows={}
for l in text.splitlines():
 m=re.match(r'^(140[0-9a-f]+):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+(\S.*)$',l)
 if m:
  a=int(m[1],16)
  if 0x1407d5460<=a<=0x1407ebb7b:
   assert a not in rows
   rows[a]=(bytes.fromhex(m[2]),m[3])
md=Cs(CS_ARCH_X86,CS_MODE_64);md.detail=True
inst={}
for a,(b,t) in rows.items():
 ds=list(md.disasm(b,a));assert len(ds)==1 and ds[0].size==len(b),(hex(a),t)
 inst[a]=ds[0]
addresses=sorted(inst)
assert all(a+inst[a].size==b for a,b in zip(addresses,addresses[1:]))
print('SOURCE_SHA256',hashlib.sha256(raw).hexdigest(),'instructions',len(inst),'contiguous_decode=PASS')
edges={};indirect=[];outside=[]
for a,i in inst.items():
 nxt=a+i.size;succ=[]
 if i.group(CS_GRP_JUMP):
  if i.operands[0].type==X86_OP_IMM:succ.append(i.operands[0].imm)
  else:indirect.append(a)
  if i.mnemonic!='jmp' and nxt in inst:succ.append(nxt)
 elif not i.group(CS_GRP_RET) and nxt in inst:succ.append(nxt)
 edges[a]=[v for v in succ if v in inst]
 outside.extend((a,v) for v in succ if v not in inst)
print('INDIRECT_JUMPS',[hex(x) for x in indirect],'OUTSIDE_BRANCHES',outside)
# Must-provenance lattice, only for exported instructions: ROOT=1, OTHER/unknown=2.
# Stack slot RBP+40 was used to save R15; current RSP+150 aliases it (RBP-RSP=110).
# Prologue: LEA RBP,[RSP-2EB0]; allocation 2FC0; no body RSP adjustment.
# Windows x64 direct/indirect calls preserve R15/RBP; arbitrary callees and escaped
# stack aliases are not analyzed, so this is a local register-reaching-def audit.
families=[{X86_REG_R15,X86_REG_R15D,X86_REG_R15W,X86_REG_R15B},
          {X86_REG_R12,X86_REG_R12D,X86_REG_R12W,X86_REG_R12B}]
# State: (R15, slot40, R12, slot08); 1=selected root/window, 2=other/unknown.
def xfer(i,state):
 result=list(state)
 written=i.regs_access()[1]
 for regidx,(family,slot,restore) in enumerate(zip(families,[0x40,8],[X86_REG_R15,X86_REG_R12])):
  pos=regidx*2
  if any(r in family for r in written):
   result[pos]=2
   if i.mnemonic=='mov' and i.operands[0].type==X86_OP_REG and i.operands[0].reg==restore:
    o=i.operands[1]
    if i.address in (0x1407d5550,0x1407d54fd):result[pos]=1
    elif o.type==X86_OP_MEM and o.mem.base==X86_REG_RBP and o.mem.disp==slot:result[pos]=state[pos+1]
 for o in i.operands:
  if o.type!=X86_OP_MEM or not o.access&CS_AC_WRITE:continue
  disp=o.mem.disp-(0x110 if o.mem.base==X86_REG_RSP else 0)
  for pos,slot,reg in [(0,0x40,X86_REG_R15),(2,8,X86_REG_R12)]:
   if o.mem.base in (X86_REG_RBP,X86_REG_RSP) and disp<slot+8 and disp+o.size>slot:
    result[pos+1]=2
    if disp==slot and o.size==8 and i.mnemonic=='mov':
     if i.operands[-1].type==X86_OP_REG and i.operands[-1].reg==reg:result[pos+1]=state[pos]
     if i.address==0x1407d5500 and slot==8:result[pos+1]=1
 return tuple(result)
states={addresses[0]:(2,2,2,2)};todo=collections.deque([addresses[0]])
while todo:
 a=todo.popleft();out=xfer(inst[a],states[a])
 for v in edges[a]:
  old=states.get(v,(0,0,0,0));merged=tuple(x|y for x,y in zip(old,out))
  if merged!=old:states[v]=merged;todo.append(v)
points=[0x1407d9ff3,0x1407da049,0x1407e6904,0x1407e8143,0x1407e815d,0x1407e8173,0x1407e8438,0x1407e8ed2,0x1407e9273]
for a in points:
 print('LOCAL_R15_PROVENANCE',hex(a),states.get(a),'ROOT_ONLY' if states.get(a,(0,))[0]==1 else 'UNRESOLVED')
 assert states[a][0]==1
for a in [0x1407d9feb,0x1407da036]:
 print('LOCAL_R12_PROVENANCE',hex(a),states[a],'WINDOW_ONLY' if states[a][2]==1 else 'UNRESOLVED')
 assert states[a][2]==1
for i in inst.values():
 if any(o.type==X86_OP_MEM and o.mem.base==X86_REG_R15 and o.mem.disp in (0x610,0x638,0x650,0x680,0x688,0x698) for o in i.operands):
  if i.address>=0x1407e6904:
   print('FIELD',hex(i.address),i.bytes.hex(' '),i.mnemonic,i.op_str)
fields={0x610,0x638,0x650,0x680,0x688,0x698}
writes=[hex(i.address) for i in inst.values() if any(
 o.type==X86_OP_MEM and o.mem.base==X86_REG_R15 and
 o.mem.disp in fields and o.access&CS_AC_WRITE for o in i.operands)]
print('DIRECT_R15_FIELD_WRITES',writes,'(callee/alias writers not analyzed)')

````

### Final source: `plugins/private_logistics/panel_fuel_observer.h`

SHA-256: `3392f202151bddd4d4946da1fd9d0c46d85b2768f68fe08ed6d515a432e2da12`

````cpp
// Read-only diagnostic of the verified 7D5460 panel quantity. NOT a burn meter.
// No native calls, allocations, logging, persistence, tracker or money here.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace private_logistics_panel {
using Address = std::uintptr_t;
using Read = bool (*)(Address, void*, std::size_t, void*);
constexpr unsigned MaxObjects = 9; // root + at most eight links; never partial totals
constexpr unsigned MaxFleet = 256;
struct Memory {
    Read read;
    void* user;
    template<class T> bool Get(Address base, Address offset, T& out) const {
        if (!read || !base || offset > UINTPTR_MAX - base ||
            sizeof(T) > UINTPTR_MAX - (base + offset)) return false;
        return read(base + offset, &out, sizeof(T), user);
    }
};
enum class Match { Unavailable, NotRoadOffice, InvalidFleet, Absent, Found };
enum class Status { Unreadable, TooManyLinks, NonFinite, Overflow, Complete };
struct Object {
    Address pointer = 0, metadata = 0, homeCandidate = 0;
    float raw = 0, capacity = 0;
    bool quantityRead = false;
    Match match = Match::Unavailable;
    unsigned fleetIndex = 0, occurrences = 0;
};
struct Snapshot {
    Address context = 0, window = 0, root = 0;
    Status status = Status::Unreadable;
    unsigned count = 0, links = 0;
    Object objects[MaxObjects]{};
    int contextMode = -1, metadataKind = -1;
    std::uint8_t flag8681 = 0, flag8682 = 0;
    bool flagsRead = false, adjustmentRead = false, adjustmentNeeded = false;
    bool aggregateValid = false, ratioValid = false;
    float current = 0, unadjustedCapacity = 0, litresCandidate = 0, ratio = 0;
};
// This tests ONLY object+4F8 as an office candidate. An absent match is NOT a
// search of all DOs. Type 43 and literal fleet-slot pointer equality are needed.
inline void FindMembership(const Memory& m, Object& o) {
    if (!m.Get(o.pointer, 0x4F8, o.homeCandidate) || !o.homeCandidate) return;
    Address descriptor = 0, begin = 0, end = 0;
    int type = -1;
    if (!m.Get(o.homeCandidate, 0x318, descriptor) || !m.Get(descriptor, 0x360, type)) return;
    if (type != 43) { o.match = Match::NotRoadOffice; return; }
    if (!m.Get(o.homeCandidate, 0xC70, begin) || !m.Get(o.homeCandidate, 0xC78, end)) return;
    if (end < begin || (end - begin) % sizeof(Address) ||
        (end - begin) / sizeof(Address) > MaxFleet || (!begin && end)) {
        o.match = Match::InvalidFleet; return;
    }
    const unsigned count = static_cast<unsigned>((end - begin) / sizeof(Address));
    for (unsigned i = 0; i < count; ++i) {
        Address entry = 0;
        if (!m.Get(begin, i * sizeof(Address), entry)) { o.match = Match::Unavailable; return; }
        if (entry == o.pointer) {
            if (!o.occurrences) o.fleetIndex = i;
            ++o.occurrences;
        }
    }
    o.match = o.occurrences ? Match::Found : Match::Absent;
}
// false = not an eligible visible detail window. true may contain an INVALID
// snapshot: callers must check status/aggregateValid, not interpret defaults as 0.
inline bool Capture(const Memory& m, Address context, Address window, Snapshot& s) {
    s = Snapshot{};
    s.context = context; s.window = window;
    std::uint8_t hidden = 1, live = 0;
    int primary = -1, subkind = -1, origin = -1, submode = -1;
    if (!m.Get(window, 1, hidden) || !m.Get(window, 0x20, live) || hidden || !live ||
        !m.Get(window, 0x34, primary) || !m.Get(window, 0x248, subkind) ||
        !m.Get(window, 0x24C, origin) || !m.Get(window, 0x23C, submode) ||
        primary != 0 || subkind != 1 || origin != 2 || submode != 0) return false;
    if (!m.Get(window, 0x240, s.root) || !s.root) return true;
    Address begin = 0, end = 0;
    if (!m.Get(s.root, 0x3A0, begin) || !m.Get(s.root, 0x3A8, end) ||
        end < begin || (end - begin) % sizeof(Address) || (!begin && end)) return true;
    const Address links = (end - begin) / sizeof(Address);
    if (links >= MaxObjects) { s.status = Status::TooManyLinks; return true; }
    s.links = static_cast<unsigned>(links); s.count = s.links + 1;
    for (unsigned i = 0; i < s.count; ++i) {
        Object& o = s.objects[i];
        o.pointer = s.root;
        if (i && !m.Get(begin, (i - 1) * sizeof(Address), o.pointer)) return true;
        FindMembership(m, o); // independent of quantity-read validity
        if (!m.Get(o.pointer, 0x1708, o.metadata) ||
            !m.Get(o.metadata, 0x8684, o.capacity) || !m.Get(o.pointer, 0x5F0, o.raw)) return true;
        o.quantityRead = true;
        if (!std::isfinite(o.capacity) || !std::isfinite(o.raw)) {
            s.status = Status::NonFinite; return true;
        }
    }
    m.Get(context, 0x5B0, s.contextMode);
    m.Get(s.objects[0].metadata, 0x294, s.metadataKind);
    s.flagsRead = m.Get(s.objects[0].metadata, 0x8681, s.flag8681) &&
                  m.Get(s.objects[0].metadata, 0x8682, s.flag8682);
    Address adjustBegin = 0, adjustEnd = 0;
    s.adjustmentRead = m.Get(s.root, 0x19E0, adjustBegin) &&
                       m.Get(s.root, 0x19E8, adjustEnd) && adjustEnd >= adjustBegin &&
                       (adjustEnd - adjustBegin) % sizeof(Address) == 0 &&
                       (adjustBegin || !adjustEnd);
    s.adjustmentNeeded = s.adjustmentRead && adjustEnd != adjustBegin;
    if (s.objects[0].capacity > 0) {
        for (unsigned i = 0; i < s.count; ++i) {
            const Object& o = s.objects[i];
            if (o.capacity <= 0) continue;
            s.unadjustedCapacity += o.capacity;
            if (o.raw > 0) s.current += o.raw;
        }
    }
    // The actual PE constant is 00 00 7A 44 = 1000.0f. Verify it at install.
    s.litresCandidate = s.current * 1000.0f;
    if (!std::isfinite(s.current) || !std::isfinite(s.unadjustedCapacity) ||
        !std::isfinite(s.litresCandidate)) { s.status = Status::Overflow; return true; }
    s.aggregateValid = true; s.status = Status::Complete;
    // Do not call the untyped native capacity adjustment or guess its result.
    s.ratioValid = s.adjustmentRead && !s.adjustmentNeeded && s.unadjustedCapacity > 0;
    if (s.ratioValid) s.ratio = s.current / s.unadjustedCapacity;
    if (!std::isfinite(s.ratio)) { s.ratioValid = false; }
    // flag8681/8682 can prove an alternate presentation, but both zero cannot
    // prove literal-litres mode without the native +7C58 helper's result.
    return true;
}
} // namespace private_logistics_panel
#endif

````

### Final source: `tests/private_logistics/panel_fuel_observer_test.cpp`

SHA-256: `7730a4b3523580f3ae37990a4503286d8662bbbbab0795eedd7b4cffb1a74869`

````cpp
#include "panel_fuel_observer.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <vector>
#include <string>
#include <utility>
using namespace private_logistics_panel;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
struct Region { Address address; std::vector<unsigned char> data; };
struct Fixture {
    std::vector<Region> regions;
    Address window=0x10000, context=0x20000, root=0x30000, meta=0x40000;
    Address home=0x50000, descriptor=0x60000, fleet=0x70000, links=0x80000;
    std::size_t reads = 0;
    std::vector<std::pair<Address,std::size_t>> denied;
    void Add(Address a, std::size_t n) { regions.push_back({a,std::vector<unsigned char>(n)}); }
    template<class T> void Set(Address a, Address off, T v) {
        for(auto& r:regions) if(a+off>=r.address && a+off+sizeof(T)<=r.address+r.data.size()) {
            std::memcpy(r.data.data()+a+off-r.address,&v,sizeof(T));return;
        }
        CHECK(false);
    }
    static bool Reader(Address a, void* out, std::size_t n, void* user) {
        auto& f=*static_cast<Fixture*>(user); ++f.reads;
        for(auto d:f.denied) if(a<d.first+d.second && d.first<a+n) return false;
        for(auto& r:f.regions) if(a>=r.address && a+n>=a && a+n<=r.address+r.data.size()) {
            std::memcpy(out,r.data.data()+a-r.address,n); return true;
        }
        return false;
    }
    Memory Mem() { return {Reader,this}; }
    Fixture() {
        Add(window,0x500);Add(context,0x600);Add(root,0x2000);Add(meta,0x9000);
        Add(home,0x1000);Add(descriptor,0x400);Add(fleet,MaxFleet*8);Add(links,0x100);
        Set(window,0x20,std::uint8_t(1)); Set(window,0x248,1);Set(window,0x24C,2);
        Set(window,0x240,root);Set(context,0x5B0,2);
        Set(root,0x1708,meta);Set(root,0x5F0,0.54954f);Set(meta,0x8684,0.7f);
        Set(root,0x4F8,home);Set(home,0x318,descriptor);Set(descriptor,0x360,43);
        Set(home,0xC70,fleet);Set(home,0xC78,fleet+8);Set(fleet,0,root);
    }
    Snapshot CaptureNow() { Snapshot s; CHECK(Capture(Mem(),context,window,s));return s; }
    void Link(float raw, float capacity) {
        Address child=0x90000, childMeta=0xA0000;
        Add(child,0x2000);Add(childMeta,0x9000);
        Set(root,0x3A0,links);Set(root,0x3A8,links+8);Set(links,0,child);
        Set(child,0x1708,childMeta);Set(child,0x5F0,raw);Set(childMeta,0x8684,capacity);
        Set(child,0x4F8,home);Set(fleet,8,child);Set(home,0xC78,fleet+16);
    }
};
#define TEST(name) static void name()
TEST(literal_litres_and_identity) {
    Fixture f;auto s=f.CaptureNow();CHECK(s.status==Status::Complete && s.aggregateValid);
    CHECK(s.current==0.54954f && s.litresCandidate==0.54954f*1000.0f);
    char text[32];std::snprintf(text,sizeof(text),"%.2fl",double(s.litresCandidate));CHECK(std::string(text)=="549.54l");
    CHECK(s.count==1 && s.objects[0].match==Match::Found && s.objects[0].fleetIndex==0);
    CHECK(s.objects[0].occurrences==1 && s.ratioValid);
}
TEST(root_plus_child) {
    Fixture f; f.Link(0.1f,0.2f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.count==2 && s.current==0.54954f+0.1f);
    CHECK(s.objects[1].match==Match::Found && s.objects[1].fleetIndex==1);
}
TEST(nonpositive_capacity_excludes_current) {
    Fixture f;f.Link(500.0f,0.0f);auto s=f.CaptureNow();CHECK(s.current==0.54954f);
    f.Set(f.meta,0x8684,0.0f);s=f.CaptureNow();CHECK(s.current==0 && s.litresCandidate==0 && !s.ratioValid);
}
TEST(negative_raw_is_preserved_not_burn) {
    Fixture f;f.Set(f.root,0x5F0,-0.03f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.current==0 && s.objects[0].raw==-0.03f);
}
TEST(nonfinite_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::quiet_NaN());auto s=f.CaptureNow();
    CHECK(s.status==Status::NonFinite && !s.aggregateValid);
    f.Set(f.root,0x5F0,1.0f);f.Set(f.meta,0x8684,std::numeric_limits<float>::infinity());
    CHECK(f.CaptureNow().status==Status::NonFinite);
}
TEST(arithmetic_overflow_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::max());auto s=f.CaptureNow();
    CHECK(s.status==Status::Overflow && !s.aggregateValid);
}
TEST(optional_capacity_adjustment_is_not_guessed) {
    Fixture f;f.Set(f.root,0x19E0,f.links);f.Set(f.root,0x19E8,f.links+8);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.adjustmentNeeded && !s.ratioValid);
    f.denied.push_back({f.root+0x19E0,16});s=f.CaptureNow();CHECK(s.aggregateValid && !s.adjustmentRead && !s.ratioValid);
}
TEST(window_gates_and_4f8_not_required) {
    Fixture f;Snapshot s;CHECK(Capture(f.Mem(),f.context,f.window,s));
    f.Set(f.window,0x4F8,std::uint8_t(0));CHECK(Capture(f.Mem(),f.context,f.window,s));
    for(Address off:{Address(0x34),Address(0x23C)}) {f.Set(f.window,off,9);CHECK(!Capture(f.Mem(),f.context,f.window,s));f.Set(f.window,off,0);}
    f.Set(f.window,0x248,3);CHECK(!Capture(f.Mem(),f.context,f.window,s));f.Set(f.window,0x248,1);
    f.Set(f.window,1,std::uint8_t(1));CHECK(!Capture(f.Mem(),f.context,f.window,s));
    f.Set(f.window,1,std::uint8_t(0));f.Set(f.window,0x20,std::uint8_t(0));CHECK(!Capture(f.Mem(),f.context,f.window,s));
}
TEST(invalid_links_never_produce_partial_total) {
    Fixture f;f.Set(f.root,0x3A0,f.links);f.Set(f.root,0x3A8,f.links+MaxObjects*8);
    auto s=f.CaptureNow();CHECK(s.status==Status::TooManyLinks && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links+1);s=f.CaptureNow();CHECK(s.status==Status::Unreadable && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links-8);CHECK(!f.CaptureNow().aggregateValid);
}
TEST(unreadable_current_keeps_identity_but_not_total) {
    Fixture f;f.denied.push_back({f.root+0x5F0,4});auto s=f.CaptureNow();
    CHECK(!s.aggregateValid && s.status==Status::Unreadable && !s.objects[0].quantityRead);
    CHECK(s.objects[0].match==Match::Found);
}
TEST(home_pointer_alone_is_not_membership) {
    Fixture f;f.Set(f.fleet,0,Address(0xDEADBEEF));auto s=f.CaptureNow();
    CHECK(s.objects[0].homeCandidate==f.home && s.objects[0].match==Match::Absent);
    f.Set(f.descriptor,0x360,52);CHECK(f.CaptureNow().objects[0].match==Match::NotRoadOffice);
}
TEST(fleet_bounds_and_duplicate_membership) {
    Fixture f;f.Set(f.home,0xC78,f.fleet+(MaxFleet+1)*8);
    CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+1);CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+16);f.Set(f.fleet,8,f.root);auto s=f.CaptureNow();
    CHECK(s.objects[0].match==Match::Found && s.objects[0].occurrences==2);
    f.denied.push_back({f.fleet+8,8});CHECK(f.CaptureNow().objects[0].match==Match::Unavailable);
}
TEST(snapshot_reuse_clears_old_pointer_and_quantity) {
    Fixture f;Snapshot s=f.CaptureNow();CHECK(s.aggregateValid);
    f.Set(f.window,0x240,Address(0));CHECK(Capture(f.Mem(),f.context,f.window,s));
    CHECK(!s.aggregateValid && s.root==0 && s.count==0 && s.objects[0].pointer==0);
}
TEST(address_overflow_does_not_call_reader) {
    Fixture f;float value=0;CHECK(!f.Mem().Get(UINTPTR_MAX-1,8,value));CHECK(f.reads==0);
    CHECK(!f.Mem().Get(0,0,value));CHECK(f.reads==0);
}
TEST(flags_are_evidence_not_literal_mode_proof) {
    Fixture f;f.Set(f.meta,0x8681,std::uint8_t(1));auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.flagsRead && s.flag8681==1 && s.flag8682==0);
    f.denied.push_back({f.meta+0x8682,1});CHECK(!f.CaptureNow().flagsRead);
}
TEST(capture_is_read_only_and_bounded) {
    Fixture f;auto before=f.regions;auto s=f.CaptureNow();CHECK(s.aggregateValid && f.reads<100);
    CHECK(before.size()==f.regions.size());for(std::size_t i=0;i<before.size();++i) CHECK(before[i].data==f.regions[i].data);
}
struct Case {const char* name;void(*run)();};
static const Case cases[]={
#define CASE(name) {#name,name}
CASE(literal_litres_and_identity),CASE(root_plus_child),CASE(nonpositive_capacity_excludes_current),
CASE(negative_raw_is_preserved_not_burn),CASE(nonfinite_is_unavailable),CASE(arithmetic_overflow_is_unavailable),
CASE(optional_capacity_adjustment_is_not_guessed),CASE(window_gates_and_4f8_not_required),
CASE(invalid_links_never_produce_partial_total),CASE(unreadable_current_keeps_identity_but_not_total),
CASE(home_pointer_alone_is_not_membership),CASE(fleet_bounds_and_duplicate_membership),
CASE(snapshot_reuse_clears_old_pointer_and_quantity),CASE(address_overflow_does_not_call_reader),
CASE(flags_are_evidence_not_literal_mode_proof),CASE(capture_is_read_only_and_bounded)
};
int main(int argc,char**argv) {
    unsigned count=0;for(const auto& c:cases)if(argc==1 || std::strcmp(argv[1],c.name)==0) {c.run();std::printf("PASS %s\n",c.name);++count;}
    CHECK(count>0);std::printf("%u native scenarios PASS\n",count);return 0;
}

````

### Final source: `.cache/run_schedule_validation.py`

SHA-256: `d5621a13b07732cd52f2ea82360c1d6a24d1c5e045ead8b264b79c3046c46ec9`

````python
from pathlib import Path
import configparser, os, re, subprocess, sys

ROOT=Path.cwd()
def run(cmd,**kwargs):
    r=subprocess.run(cmd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,**kwargs)
    print(r.stdout,end='',flush=True)
    if r.returncode: raise SystemExit(r.returncode)
    return r.stdout

with open('.cache/schedule-regression-results.txt','w') as log:
    original=sys.stdout;sys.stdout=log
    total=0
    for d in ['private_logistics','private_logistics_probe','private_shops','private_logistics_v018_model','vehicle_gauge_strings']:
        cmd=[sys.executable,'-m','unittest','discover','-s','tests/'+d,'-p','test_*.py']
        print('\nCOMMAND',' '.join(cmd),flush=True)
        output=run(cmd)
        total+=int(re.search(r'Ran (\d+) tests',output)[1])
    print('\nAll',total,'Python regression tests PASS; no native game/DLL validation.',flush=True)
    sys.stdout=original

with open('.cache/schedule-host-results.txt','w') as log:
    original=sys.stdout;sys.stdout=log
    for sanitize in [False,True]:
        for case in ['fuel_diagnostics','panel_fuel_runtime','panel_fuel_observer','journey_tracker','schedule_observer','schedule_runtime']:
            exe='.cache/'+case+('-asan' if sanitize else '-optimized')
            flags=['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie'] if sanitize else ['-O2']
            cmd=['g++','-std=c++17','-Wall','-Wextra','-Werror','-pedantic',*flags,'-I','plugins/private_logistics','tests/private_logistics/'+case+'_test.cpp','-o',exe]
            print('\nCOMMAND',' '.join(cmd),flush=True);run(cmd)
            print('RUN',exe+(' ASAN_OPTIONS=detect_leaks=0' if sanitize else ''),flush=True)
            run([exe],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
    print('\nAll six optimized and ASan/UBSan host suites PASS. Not Windows ABI/SEH/thread/native lifecycle proof.',flush=True)
    sys.stdout=original

with open('.cache/schedule-settings-check.txt','w') as log:
    original=sys.stdout;sys.stdout=log
    baseline='a999dfaef66453409c648812ba3d40a9bd32317e'
    old=configparser.ConfigParser()
    old.read_string(subprocess.check_output(['git','show',baseline+':plugins/private_logistics/private_logistics.ini'],text=True))
    new=configparser.ConfigParser();new.read('plugins/private_logistics/private_logistics.ini')
    a=dict(old['private_logistics']);b=dict(new['private_logistics'])
    assert b.pop('schedule_observer')=='1' and a==b
    print('PASS all existing INI values identical to baseline '+baseline+'; only schedule_observer=1 added')
    assert new.getint('private_logistics','billing')==0 and new.getint('private_logistics','dry_run')==1
    assert new.getint('private_logistics','auto_source_percent')==10 and new.getint('private_logistics','auto_destination_percent')==30
    print('PASS billing=0 dry_run=1 source=10 destination=30')
    diff=subprocess.check_output(['git','diff',baseline,'--','plugins/private_logistics/private_logistics.cpp'],text=True)
    changes=[l for l in diff.splitlines() if l.startswith(('+','-')) and not l.startswith(('+++','---'))]
    assert len(changes)==6
    print('PASS plugin CPP diff is version/comment + schedule default/config only; no dispatch, permissions, tracker or billing edits')
    sys.stdout=original

run(['git','diff','--check'])
print('PASS git diff --check')
print('Completed Python/native/settings validation. Run the optional Capstone export audit separately.')

````

### Actual command results

Validation driver: `python .cache/run_schedule_validation.py`. Its successful console result was:

```text
PASS git diff --check
Completed Python/native/settings validation. Run the optional Capstone export audit separately.
```

#### `.cache/schedule-regression-results.txt`

SHA-256: `37bb9926c923b1f4e2dcc55803c8a031dce07a2cd5f35a7e8efc36a815b9e242`

````text

COMMAND /usr/bin/python -m unittest discover -s tests/private_logistics -p test_*.py
.........................................................................................
----------------------------------------------------------------------
Ran 89 tests in 5.873s

OK

COMMAND /usr/bin/python -m unittest discover -s tests/private_logistics_probe -p test_*.py
.............................................................................................
----------------------------------------------------------------------
Ran 93 tests in 0.084s

OK

COMMAND /usr/bin/python -m unittest discover -s tests/private_shops -p test_*.py
....
----------------------------------------------------------------------
Ran 4 tests in 0.148s

OK

COMMAND /usr/bin/python -m unittest discover -s tests/private_logistics_v018_model -p test_*.py
......................................
----------------------------------------------------------------------
Ran 38 tests in 0.003s

OK

COMMAND /usr/bin/python -m unittest discover -s tests/vehicle_gauge_strings -p test_*.py
..............
----------------------------------------------------------------------
Ran 14 tests in 0.035s

OK

All 238 Python regression tests PASS; no native game/DLL validation.

````

#### `.cache/schedule-host-results.txt`

SHA-256: `51f96ff26cfba2afeea5a3abb762c5a757a543ecd2d1b0035378633958745c74`

````text

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/fuel_diagnostics_test.cpp -o .cache/fuel_diagnostics-optimized
RUN .cache/fuel_diagnostics-optimized
PASS display_fallback_not_self_comparable
PASS independent_candidate_comparison
PASS invalid_negative_and_nonfinite_not_zero_or_hidden
PASS readable_zero_is_valid_invalid_is_not_zero
PASS changes_are_sample_decreases_and_increases_only
PASS source_changes_reset_baseline_in_both_directions
PASS invalid_read_breaks_sample_continuity
PASS load_epoch_breaks_sample_continuity
8 candidate-provenance scenarios PASS (not native fuel validation)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/panel_fuel_runtime_test.cpp -o .cache/panel_fuel_runtime-optimized
RUN .cache/panel_fuel_runtime-optimized
PASS runtime_install_fingerprint
PASS runtime_post_render_throttle_and_deferred_log
PASS runtime_long_pause_coalesces_and_reports_omissions
PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record
PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard
PASS runtime_inactive_does_not_starve_capture_and_cap
PASS runtime_guarded_fault_disables_and_reports_once
PASS runtime_changed_data_refused
8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/panel_fuel_observer_test.cpp -o .cache/panel_fuel_observer-optimized
RUN .cache/panel_fuel_observer-optimized
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/journey_tracker_test.cpp -o .cache/journey_tracker-optimized
RUN .cache/journey_tracker-optimized
PASS boundaries
PASS fuel_excluded
PASS two_journeys
PASS initially_loaded
PASS transfer_before_is_baseline
PASS snapshots_not_transfers
PASS partial_snapshots_and_residue
PASS refill_does_not_cancel_burn
PASS uncertain_evidence
PASS coverage_default_is_unverified
PASS duplicate_completion
PASS duplicate_burn_and_equal_values
PASS gap_empty_payload_not_baseline
PASS sequence_exhaustion
PASS conflicting_duplicate
PASS sequence_gap
PASS sequence_order
PASS explicit_gap
PASS identity_changes
PASS reload_reset
PASS mismatched_cargo
PASS invalid_cargo
PASS invalid_fuel
PASS overflow
PASS invalid_enums
PASS zero_deductions
PASS formatted_output
PASS format_incomplete
PASS interleaved_vehicles
PASS randomized_journeys

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/schedule_observer_test.cpp -o .cache/schedule_observer-optimized
RUN .cache/schedule_observer-optimized
PASS schedule_selected_fields_mapping_no_pointee_following
PASS schedule_special_last_row_does_not_read_config
PASS schedule_empty_list_is_valid_not_completion
PASS schedule_null_target_and_unknown_index_preserved
PASS schedule_missing_header_and_config_fail_closed
PASS schedule_bad_bounds_and_oversize_do_not_walk
PASS schedule_max_rows_and_checked_arithmetic
PASS schedule_nonfinite_no_partial_table
PASS schedule_header_and_row_mutation_rejected
PASS schedule_history_first_unchanged_and_index_only
PASS schedule_history_field_storage_and_context_changes_separate
PASS schedule_history_list_clear_replacement_are_observed_fields_only
PASS schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas
PASS schedule_identical_visible_schedule_is_not_new_job
14 schedule host scenarios PASS (not native lifecycle validation)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I plugins/private_logistics tests/private_logistics/schedule_runtime_test.cpp -o .cache/schedule_runtime-optimized
RUN .cache/schedule_runtime-optimized
PASS schedule_runtime_mismatch_disables_schedule_not_fuel
PASS schedule_runtime_same_post_render_record_deferred_rows_and_truthful_labels
PASS schedule_runtime_unchanged_suppressed_and_row_index_not_job_end
PASS schedule_runtime_paused_coalescing_carries_latest_schedule_and_rebaselines
PASS schedule_runtime_load_absent_and_duplicate_fleet_fail_closed
PASS schedule_runtime_capture_drop_counter_advance_rebaselines
PASS schedule_runtime_invalid_schedule_never_blocks_fuel_or_prints_partial_rows
PASS schedule_runtime_fault_latch_isolated_from_fuel_not_a_windows_seh_test
8 schedule runtime host-shim scenarios PASS (not Windows ABI/SEH or a DLL)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/fuel_diagnostics_test.cpp -o .cache/fuel_diagnostics-asan
RUN .cache/fuel_diagnostics-asan ASAN_OPTIONS=detect_leaks=0
PASS display_fallback_not_self_comparable
PASS independent_candidate_comparison
PASS invalid_negative_and_nonfinite_not_zero_or_hidden
PASS readable_zero_is_valid_invalid_is_not_zero
PASS changes_are_sample_decreases_and_increases_only
PASS source_changes_reset_baseline_in_both_directions
PASS invalid_read_breaks_sample_continuity
PASS load_epoch_breaks_sample_continuity
8 candidate-provenance scenarios PASS (not native fuel validation)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/panel_fuel_runtime_test.cpp -o .cache/panel_fuel_runtime-asan
RUN .cache/panel_fuel_runtime-asan ASAN_OPTIONS=detect_leaks=0
PASS runtime_install_fingerprint
PASS runtime_post_render_throttle_and_deferred_log
PASS runtime_long_pause_coalesces_and_reports_omissions
PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record
PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard
PASS runtime_inactive_does_not_starve_capture_and_cap
PASS runtime_guarded_fault_disables_and_reports_once
PASS runtime_changed_data_refused
8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/panel_fuel_observer_test.cpp -o .cache/panel_fuel_observer-asan
RUN .cache/panel_fuel_observer-asan ASAN_OPTIONS=detect_leaks=0
PASS literal_litres_and_identity
PASS root_plus_child
PASS nonpositive_capacity_excludes_current
PASS negative_raw_is_preserved_not_burn
PASS nonfinite_is_unavailable
PASS arithmetic_overflow_is_unavailable
PASS optional_capacity_adjustment_is_not_guessed
PASS window_gates_and_4f8_not_required
PASS invalid_links_never_produce_partial_total
PASS unreadable_current_keeps_identity_but_not_total
PASS home_pointer_alone_is_not_membership
PASS fleet_bounds_and_duplicate_membership
PASS snapshot_reuse_clears_old_pointer_and_quantity
PASS address_overflow_does_not_call_reader
PASS flags_are_evidence_not_literal_mode_proof
PASS capture_is_read_only_and_bounded
16 native scenarios PASS

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/journey_tracker_test.cpp -o .cache/journey_tracker-asan
RUN .cache/journey_tracker-asan ASAN_OPTIONS=detect_leaks=0
PASS boundaries
PASS fuel_excluded
PASS two_journeys
PASS initially_loaded
PASS transfer_before_is_baseline
PASS snapshots_not_transfers
PASS partial_snapshots_and_residue
PASS refill_does_not_cancel_burn
PASS uncertain_evidence
PASS coverage_default_is_unverified
PASS duplicate_completion
PASS duplicate_burn_and_equal_values
PASS gap_empty_payload_not_baseline
PASS sequence_exhaustion
PASS conflicting_duplicate
PASS sequence_gap
PASS sequence_order
PASS explicit_gap
PASS identity_changes
PASS reload_reset
PASS mismatched_cargo
PASS invalid_cargo
PASS invalid_fuel
PASS overflow
PASS invalid_enums
PASS zero_deductions
PASS formatted_output
PASS format_incomplete
PASS interleaved_vehicles
PASS randomized_journeys

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/schedule_observer_test.cpp -o .cache/schedule_observer-asan
RUN .cache/schedule_observer-asan ASAN_OPTIONS=detect_leaks=0
PASS schedule_selected_fields_mapping_no_pointee_following
PASS schedule_special_last_row_does_not_read_config
PASS schedule_empty_list_is_valid_not_completion
PASS schedule_null_target_and_unknown_index_preserved
PASS schedule_missing_header_and_config_fail_closed
PASS schedule_bad_bounds_and_oversize_do_not_walk
PASS schedule_max_rows_and_checked_arithmetic
PASS schedule_nonfinite_no_partial_table
PASS schedule_header_and_row_mutation_rejected
PASS schedule_history_first_unchanged_and_index_only
PASS schedule_history_field_storage_and_context_changes_separate
PASS schedule_history_list_clear_replacement_are_observed_fields_only
PASS schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas
PASS schedule_identical_visible_schedule_is_not_new_job
14 schedule host scenarios PASS (not native lifecycle validation)

COMMAND g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie -I plugins/private_logistics tests/private_logistics/schedule_runtime_test.cpp -o .cache/schedule_runtime-asan
RUN .cache/schedule_runtime-asan ASAN_OPTIONS=detect_leaks=0
PASS schedule_runtime_mismatch_disables_schedule_not_fuel
PASS schedule_runtime_same_post_render_record_deferred_rows_and_truthful_labels
PASS schedule_runtime_unchanged_suppressed_and_row_index_not_job_end
PASS schedule_runtime_paused_coalescing_carries_latest_schedule_and_rebaselines
PASS schedule_runtime_load_absent_and_duplicate_fleet_fail_closed
PASS schedule_runtime_capture_drop_counter_advance_rebaselines
PASS schedule_runtime_invalid_schedule_never_blocks_fuel_or_prints_partial_rows
PASS schedule_runtime_fault_latch_isolated_from_fuel_not_a_windows_seh_test
8 schedule runtime host-shim scenarios PASS (not Windows ABI/SEH or a DLL)

All six optimized and ASan/UBSan host suites PASS. Not Windows ABI/SEH/thread/native lifecycle proof.

````

#### `.cache/schedule-settings-check.txt`

SHA-256: `17db7bc690cb8aa6a28c4c941beeb57f1963aa7e3458ab23cdd4782d16d84b22`

````text
PASS all existing INI values identical to baseline a999dfaef66453409c648812ba3d40a9bd32317e; only schedule_observer=1 added
PASS billing=0 dry_run=1 source=10 destination=30
PASS plugin CPP diff is version/comment + schedule default/config only; no dispatch, permissions, tracker or billing edits

````

#### `.cache/schedule-native-audit.txt`

SHA-256: `43998abc7dc7e793c6b7f9b27e2ae27a93b297250a50b9c1e3c30e672b95522a`

Command: `PYTHONPATH=.cache/python-packages python tools/audit_vehicle_schedule_export.py > .cache/schedule-native-audit.txt`

Capstone 5.0.7 was installed only into ignored `.cache/python-packages`; no new runtime dependency.

````text
SOURCE_SHA256 2f3c638236c3c7d2e26cd97fe46d570748b497b1a69272c7358cac7877ce20e0 instructions 18611 contiguous_decode=PASS
INDIRECT_JUMPS [] OUTSIDE_BRANCHES []
LOCAL_R15_PROVENANCE 0x1407d9ff3 (1, 1, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407da049 (1, 1, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e6904 (1, 1, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e8143 (1, 2, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e815d (1, 2, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e8173 (1, 2, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e8438 (1, 2, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e8ed2 (1, 2, 1, 1) ROOT_ONLY
LOCAL_R15_PROVENANCE 0x1407e9273 (1, 2, 1, 1) ROOT_ONLY
LOCAL_R12_PROVENANCE 0x1407d9feb (1, 1, 1, 1) WINDOW_ONLY
LOCAL_R12_PROVENANCE 0x1407da036 (1, 1, 1, 1) WINDOW_ONLY
FIELD 0x1407e6904 49 8b 8f 88 06 00 00 mov rcx, qword ptr [r15 + 0x688]
FIELD 0x1407e690b 49 2b 8f 80 06 00 00 sub rcx, qword ptr [r15 + 0x680]
FIELD 0x1407e6981 49 39 bf 50 06 00 00 cmp qword ptr [r15 + 0x650], rdi
FIELD 0x1407e699a 49 8b 87 50 06 00 00 mov rax, qword ptr [r15 + 0x650]
FIELD 0x1407e6a60 4d 8b 87 50 06 00 00 mov r8, qword ptr [r15 + 0x650]
FIELD 0x1407e6bdb 49 83 bf 50 06 00 00 00 cmp qword ptr [r15 + 0x650], 0
FIELD 0x1407e6d3b 4d 8b 87 50 06 00 00 mov r8, qword ptr [r15 + 0x650]
FIELD 0x1407e7262 49 83 bf 50 06 00 00 00 cmp qword ptr [r15 + 0x650], 0
FIELD 0x1407e726c 49 8b 87 50 06 00 00 mov rax, qword ptr [r15 + 0x650]
FIELD 0x1407e7616 49 8b 87 88 06 00 00 mov rax, qword ptr [r15 + 0x688]
FIELD 0x1407e761d 49 2b 87 80 06 00 00 sub rax, qword ptr [r15 + 0x680]
FIELD 0x1407e7631 49 8b 97 88 06 00 00 mov rdx, qword ptr [r15 + 0x688]
FIELD 0x1407e7638 49 2b 97 80 06 00 00 sub rdx, qword ptr [r15 + 0x680]
FIELD 0x1407e7643 49 8b 87 80 06 00 00 mov rax, qword ptr [r15 + 0x680]
FIELD 0x1407e8143 49 8b 87 88 06 00 00 mov rax, qword ptr [r15 + 0x688]
FIELD 0x1407e814a 49 2b 87 80 06 00 00 sub rax, qword ptr [r15 + 0x680]
FIELD 0x1407e815d 49 8b 87 80 06 00 00 mov rax, qword ptr [r15 + 0x680]
FIELD 0x1407e8173 49 03 b7 38 06 00 00 add rsi, qword ptr [r15 + 0x638]
FIELD 0x1407e8186 49 8b 87 38 06 00 00 mov rax, qword ptr [r15 + 0x638]
FIELD 0x1407e819b 49 8b 87 88 06 00 00 mov rax, qword ptr [r15 + 0x688]
FIELD 0x1407e81a2 49 2b 87 80 06 00 00 sub rax, qword ptr [r15 + 0x680]
FIELD 0x1407e8438 41 39 87 98 06 00 00 cmp dword ptr [r15 + 0x698], eax
FIELD 0x1407e8649 41 39 87 98 06 00 00 cmp dword ptr [r15 + 0x698], eax
FIELD 0x1407e870c 41 39 87 98 06 00 00 cmp dword ptr [r15 + 0x698], eax
FIELD 0x1407e8a3d 41 39 bf 98 06 00 00 cmp dword ptr [r15 + 0x698], edi
FIELD 0x1407e8cb3 41 39 bf 98 06 00 00 cmp dword ptr [r15 + 0x698], edi
FIELD 0x1407e8cbc 49 83 bf 10 06 00 00 00 cmp qword ptr [r15 + 0x610], 0
FIELD 0x1407e8cc6 4d 3b b7 10 06 00 00 cmp r14, qword ptr [r15 + 0x610]
FIELD 0x1407e8ddf 41 39 bf 98 06 00 00 cmp dword ptr [r15 + 0x698], edi
FIELD 0x1407e8de8 49 83 bf 10 06 00 00 00 cmp qword ptr [r15 + 0x610], 0
FIELD 0x1407e8df2 4d 3b b7 10 06 00 00 cmp r14, qword ptr [r15 + 0x610]
FIELD 0x1407e9cde 49 8b 97 80 06 00 00 mov rdx, qword ptr [r15 + 0x680]
FIELD 0x1407e9d52 49 8b 87 88 06 00 00 mov rax, qword ptr [r15 + 0x688]
FIELD 0x1407e9d59 49 2b 87 80 06 00 00 sub rax, qword ptr [r15 + 0x680]
FIELD 0x1407e9d6c 49 8b 87 80 06 00 00 mov rax, qword ptr [r15 + 0x680]
FIELD 0x1407e9d9f 49 8b 87 80 06 00 00 mov rax, qword ptr [r15 + 0x680]
FIELD 0x1407e9dea 49 8b 87 80 06 00 00 mov rax, qword ptr [r15 + 0x680]
FIELD 0x1407eb1fc 49 8b 97 10 06 00 00 mov rdx, qword ptr [r15 + 0x610]
DIRECT_R15_FIELD_WRITES ['0x1407d9ff3', '0x1407da049'] (callee/alias writers not analyzed)

````


---

# 2026-10-30 policy amendment — assignment interval regardless of outcome

The user explicitly chose the simpler future accounting envelope: **task assigned to a DO vehicle → that task removed**, regardless of whether delivery succeeded. This supersedes earlier success-only terminal-outcome requirements in the proposed assignment investigation. Actual fuel consumed during cancelled/unsuccessful work is intended to be compensable too. No runtime accounting/payment change is made by this amendment; v0.18.8 is still observations only and billing remains OFF.

## Consequences

- Successful completion versus cancellation is no longer a prerequisite for closing the accounting interval. It is optional diagnostic detail.
- Internal pickup/unload stop transitions are not separate tasks or invoices. Replacements close the old interval and begin a new one only when the actual native boundary is verified; a changed row/config fingerprint is not sufficient proof.
- Travel while the task is still assigned belongs inside this chosen interval, including return travel if the assignment actually persists during it. Travel after removal is outside. The old provisional “all return separate/unbilled” proposal is superseded by this interval-based rule, not by guessing which UI row is return.
- A stable native job ID is not mandatory if reliable task presence/removal/replacement can be established another way. Initial already-active observations, reloads, pointer reuse, lost transitions and identical replacements must remain explicitly uncertain rather than inventing start points or duplicate bills.
- Task removal need not mean success, but must mean a real task removal—not closing the panel, switching vehicles, a failed read, list-storage relocation or ordinary row advance. The visible schedule may contain retained/default/return rows; its empty/nonempty state is not yet a verified assignment flag.
- Actual fuel burn still differs from endpoint tank decline when refuelling or other quantity changes intervene. For now a no-observed-refill endpoint difference can be a labelled estimate, not exact burn or proof that no refill occurred. Verified refill/deduction/units/clamp evidence is still needed for exact billing.
- Payer for cancelled/replaced work is not specified by this message. Do not silently charge the home DO, treasury or presumed destination. Establish the party mapping and obtain an explicit payer policy before enabling money transfers.
- The older goods-loading → fully-empty tracker is still disconnected and unchanged. The new policy is the future native assignment adapter's contract, not a silent rewrite of that tracker.

## Should the user continue testing?

**Do not repeat v0.18.7.** A rebuilt/deployed **v0.18.8** may be tested in parallel with the native investigation, but only as a schedule-correlation diagnostic—not billing and not completed interval records. Source in this session is not yet committed/pushed; pulling the target branch alone does not retrieve it. Follow publication/merge and Windows build steps in the preceding section before any run. Confirm the loaded plugin version and additional schedule fingerprint match; keep billing=0/dry_run=1 (both already defaults).

If proceeding with that changed diagnostic, use one truck and keep its panel open: observe idle-before-assignment, first assignment, pickup→unload stop change, removal, second assignment/removal and the empty return. Retain the log and note visible boundary times. If safe/convenient, cancel one extra task mid-work to correlate removal without success; no unsafe in-flight deletion or permission changes are requested. Avoid refuelling for an easier endpoint-estimate comparison where practical; if it happens, note it rather than hiding it. Keep a spare/adequately fuelled truck; no empty-tank experiment is requested. Stop once the capture cap is reported; later transitions will not be captured. No manual save is required, and this is not a save/reload validation request. v0.18.8 still cannot itself prove whole-job boundaries from a visible run.

The decisive missing native evidence remains FUN_1406BF1D0 and its relevant callers/callees, now investigated for presence/removal/replacement rather than successful-delivery semantics. The exact small-context prompt follows for the agent with the matching Ghidra program/executable. It requests one Markdown result; returned evidence will be reviewed and folded into this cumulative handover before implementation.

## Copy/paste agent prompt

````text
Investigate the smallest reliable native DO-truck assignment interval in Ghidra. Do not implement billing or alter the plugin.

USER POLICY (supersedes success-only closure): start an interval when a task is assigned to a vehicle working for a distribution office; end it when that task is removed, cancelled or replaced, regardless of delivery success. Internal loading/unloading stop changes remain inside the interval. Include travel while the task remains assigned; exclude travel after its removal. Successful completion versus cancellation is optional diagnostic detail, NOT a prerequisite for closing the interval. A stable native job ID is not mandatory if presence/absence and replacement boundaries can be proved reliably.

TARGET: SOVIET64.exe v1.1.1.9, image base 0x140000000, PE timestamp 0x6A3EB6AD, reported SHA256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8. Verify the program identity first; report mismatch/missing inputs rather than guessing.

Read only the latest relevant sections of docs/private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md, RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md, docs/ghidra-do-storage-findings.md, and plugins/private_logistics/schedule_observer.h. Existing source v0.18.8 observes selected schedule fields, not assignment intervals. The renderer export is UTF-16. Do not repeat a giant renderer decompilation or broad gauge/string search.

VERIFIED ANCHORS:
- Renderer FUN_1407D5460: selected vehicle R15=[window+240]. At 1407E9D43 RCX=R15; CALL FUN_1406BF1D0 at 1407E9D46 with RDX/R8/R9 zero. Other calls at 1407D9959 and 1407D9C51 pass the selected root with DL=1. The helper's semantics/body are missing here.
- Root +680/+688: raw target list; +638: configuration base, stride 0x198; +698: rendered row index. Root byte +5FD suppresses config use for the last raw row. +610 and +650 remain opaque context pointers, not verified job IDs.
- Renderer 1407D9FF3 writes +698 from window+264; 1407DA049 writes +610 from a selected target. Neither store proves natural completion.
- DO C70/C78 is fleet; D38/D40 are task/target configurations, NOT the fleet or a verified per-vehicle job identity. Candidate home is not the buyer.

TASKS, in order:
1. Export complete addressed bytes/disassembly of FUN_1406BF1D0, its signature/function bounds, incoming call references, and bounded caller argument setup. Obtain decompilation only if readily available; raw instructions are required.
2. Follow only relevant callers/callees to identify actual DO assignment installation, final removal and replacement on the SAME proven vehicle receiver. Distinguish schedule editing/reset helpers from simulation dispatch/termination. Preserve receiver dataflow; similarly named offsets on unrelated objects are not evidence.
3. Prove the smallest presence/absence signal. Test its meaning for idle-at-DO, assigned-but-not-loaded, internal stop advance, final removal, cancellation/replacement, and empty return. Are idle/default/return rows retained? Can a new task replace the old one without an empty list? Can identical consecutive jobs be distinguished? A nonempty schedule is NOT automatically an active DO task.
4. Recommend read-only snapshot fields if sufficient; otherwise identify an exact verified native boundary suitable for later guarded observation, including signature/receiver/event order. Do not install hooks. Identify intended destination/payer evidence if adjacent, but keep it separate from interval detection. Do not guess who pays cancelled work.

DELIVER ONE Markdown containing executable identity, conclusions/unknowns, exact offsets and addressed instruction evidence, complete raw bodies for the small relevant functions, caller/callee provenance, and any complete analysis scripts plus actual output. Label static deductions versus live-game validation. If blocked, give the smallest precise missing artifact. Do not request another unchanged game run, enable payments, infer exact fuel burn from endpoint subtraction, or modify working dispatch/permissions/defaults.

````
