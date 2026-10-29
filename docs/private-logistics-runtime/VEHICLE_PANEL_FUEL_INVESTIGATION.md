# Road-vehicle panel fuel: focused investigation

## Status

Investigation in progress; no tracker implementation or validated truck-tank source. The full raw exports in remote commit c7bb15e were reviewed directly on 2026-10-28; see the latest review below for supported conclusions and the new callee to inspect. Earlier appendix claims of a confirmed panel fuel chain/conversion are not accepted as established findings. Billing remains OFF; working dispatch policy stays unchanged.

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
