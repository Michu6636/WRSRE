# Implementation Plan — Per-Trip Fuel Cost Estimation

**Target:** `SOVIET64.exe` v1.1.1.9, base `0x140000000`, ASLR on  
**Plugin:** `private_logistics` (v0.17 → v0.18)  
**Status:** Plan only — no implementation yet  
**Model:** REPLACE the current `litres_per_trip` constant tariff with measured per-trip fuel burn  
**Trip detection:** Transfer-sequence heuristic (no new hooks)  
**Granularity:** Per-delivery

---

## 1. Goals

- Calculate the actual fuel burned for each trip (DO → load → deliver → return to DO)
- Replace `litres_per_trip` constant with per-delivery measured burn in the tariff formula
- Calibrate start/stop detection based on real in-game tests (deferred)
- Retain the fail-closed design: if burn cannot be measured, fall back to calibrated constant or skip payment

---

## 2. Current State (v0.17)

| Component | Mechanism | Limitation |
|-----------|-----------|------------|
| Tariff formula | `work = litres_per_trip + litres_per_tonne * tons` | `litres_per_trip = 14.75` is a fleet-average constant from one fleet |
| Fuel measurement | Tank readings at every close (`veh+0x3B8` fuel slot) | Only aggregated per day, not per trip |
| Tariff probe | `BurnProbe()` at every committed delivery | Fleet-level delta, not per-vehicle |
| Trip detection | None — all deliveries treated identically | No trip boundaries |

## 3. Verified Ghidra Findings

### Vehicle Lifecycle

| Offset | Type | Meaning | Values |
|--------|------|---------|--------|
| `veh+0x448` | int | Vehicle state | `0` = idle/parked, `1`/`2` = active |
| `veh+0x440` | int | Vehicle mode | `9` = working (gated) |
| `veh+0x4F8` | `Building*` | Home DO (depot/garage) | Fleet owner |
| `veh+0x50C` | int | Vehicle kind/fuel consumer flag | `0x42` = fuel consumer |
| `veh+0x5F0` | float | Fuel mirror (unclamped tonnes) | Cross-check only |
| `veh+0x3B8` | `vec` | Cargo vector, stride `0x10` | `{Resource*, float tons}` per slot |
| `veh+0x1708` | pointer | Vehicle metadata | `+0x8684` = tank capacity |

**Burn gate:** `FUN_14016E3B0` skips vehicles unless `veh+0x448 ∈ {1,2}` and `veh+0x440 == 9` and `veh+0x50C == 0x42`.

### Transfer Call Sites (Round-Trip Sequence)

| Site RVA | Class | Mode | `bld` | Context |
|----------|-------|------|-------|---------|
| `0x16F831` | SITE_REFUEL | 0 | `*(veh+0x4F8)` | DO refuel (fuel from home DO to vehicle) |
| `0x694F40` | SITE_LOAD | 0 | Source building | Load at source building |
| `0x68EECA` | SITE_LOAD | 0 | Source building | Load at source building |
| `0x1CB7FF` | SITE_LOAD | 0 | Source building | Load at source building |
| `0x68F792` | SITE_DELIVERY | 0 | Destination building | Delivery at destination |
| `0x68F7F9` | SITE_TRANSSHIP | 1 | `*(veh+0x4F8)` | Return-to-depot unload |
| `0x6953C4` | SITE_TRANSSHIP | 1 | Register-loaded | Ship/line delivery |
| `0x1B2898` | SITE_UNKNOWN | 1 | NULL | Road delivery unload |
| `0x1B484B` | SITE_UNKNOWN | 1 | NULL | Delivery path (alt) |
| `0x1B5EC0`–`0x1B7981` | SITE_UNKNOWN | varies | varies | Unobserved |
| `0x6945B3` | SITE_TRANSSHIP | 0 | -tons | Never observed |

### Transfer Function Signature
`transfer(game, bld, mode, res, tons, &out)` — `FUN_140420350` (RVA `0x420350`)
- `mode=0`: resource flows from building to vehicle (negative tons = draw)
- `mode=1`: resource flows from vehicle to building (delivery/unload)
- `param_9` = destination building; `NULL` = resolve from order

### Round-Trip Sequence
```
DO refuel → leave DO (state 0→1/2) → load → deliver → return DO → unload → refuel → repeat
```

### Existing Infrastructure Reusable
- `g_carrier[CARRIER_RING]` — links mode-1 vehicle calls to mode-0 load calls
- `g_vehFuel[VEH_MAX]` / `g_vehFuelValid[VEH_MAX]` — per-slot last tank reading
- `ReadTank()` / `FuelSlotTons()` / `MirrorTons()` — read vehicle fuel level
- `FleetFuelTons()` — fleet-level tank total (used by `BurnProbe`)
- `BurnProbe()` — fleet-level burn delta between deliveries

---

## 4. Design: Per-Trip Fuel Cost Estimation

### 4.1 Data Structures

Add to `private_logistics.cpp`:

```cpp
#define TRIP_VEH_MAX 64

// Per-vehicle trip state
struct TripState {
    void*   vehicle;          // Vehicle* (transient, never persisted)
    void*   homeDO;           // Home DO building (veh+0x4F8)
    bool    active;           // True once a trip has started
    double  fuelAtStart;      // Fuel level (tonnes) at trip start
    double  fuelAtLoad;       // Fuel level at load confirmation
    double  fuelAtDeliver;    // Fuel level at delivery/unload
    double  fuelAtReturn;     // Fuel level at return to DO
    unsigned lastLoadSeq;     // Shipment seq at last load (dedup)
    unsigned lastDeliverSeq;  // Shipment seq at last delivery (dedup)
    double  lastLoadT;        // NowSeconds() at last load
    double  lastDeliverT;     // NowSeconds() at last delivery
};

static TripState g_trips[TRIP_VEH_MAX];
static int       g_tripCount = 0;       // vehicles with active trip records
static int       g_tripSlot[TRIP_VEH_MAX]; // vehicle -> trip index mapping (linear scan)
```

### 4.2 Trip Boundary Detection (Transfer-Sequence Heuristic)

The user wants:
- **Start:** vehicle leaves DO office (inferred from load event at source)
- **Confirm start:** vehicle loads resources (SITE_LOAD at source building)
- **End:** vehicle unloads resources and returns to DO (SITE_DELIVERY at dest + return inference)

Since there is no explicit "vehicle leaves DO" event, we infer trip boundaries from the transfer call sequence:

```
Trip start (inferred):  SITE_LOAD at source building → vehicleHome == DO
Trip confirm:           Same SITE_LOAD event (tonnage > 0, resource is cargo)
Trip end:               SITE_DELIVERY at destination → vehicleHome == DO
Trip complete:          Return to DO (inferred from next SITE_LOAD at same DO, or SITE_REFUEL at home DO)
```

**Detection rules** (in `DetourTransfer`, mode 0 and mode 1):

| Event | Site | Mode | Tons | `vehicleHome == DO` | Action |
|-------|------|------|------|---------------------|--------|
| Load | SITE_LOAD | 0 | `-tons` | Yes | Start trip: record `fuelAtStart = ReadTank(vehicle)` |
| Load confirm | SITE_LOAD | 0 | `-tons` | Yes | Same event — fuel at load = `fuelAtStart` |
| Delivery | SITE_DELIVERY | 0 | `+tons` | Yes (if dest is DO) or No | Record `fuelAtDeliver = ReadTank(vehicle)` |
| Return to DO | SITE_TRANSSHIP | 1 | `-tons` | Yes | Record `fuelAtReturn = ReadTank(vehicle)` |
| Refuel at DO | SITE_REFUEL | 0 | `-tons` | Yes | Trip end marker: compute burn |

### 4.3 Burn Calculation

For each vehicle that completes a trip (load → deliver → return):

```cpp
double burnTons = fuelAtStart - fuelAtReturn;  // positive = burned
if (burnTons < 0.0) burnTons = 0.0;  // refill between measurements
if (burnTons > tankCapacity) burnTons = tankCapacity;  // sanity clamp

double litres = burnTons / gDieselDensity;  // tonnes → litres
double fuelCost = litres * FuelUnitPrice();  // at live fuel price
double wearCost = gWearLitres * gDieselDensity * FuelUnitPrice();
double tariff = fuelCost * (1.0 + gTariffMargin) + wearCost;
```

### 4.4 Per-Delivery Granularity

Each delivery gets its own burn measurement:

1. At `SITE_LOAD` (mode 0, tons < 0) — record `fuelAtStart` for the vehicle
2. At `SITE_DELIVERY` (mode 0, tons > 0) — record `fuelAtDeliver`
3. At next `SITE_LOAD` for same vehicle — compute burn since last delivery, reset state

**Edge cases:**
- **Multi-drop route** (load → deliver → deliver → deliver): Each delivery computes burn since previous delivery for that vehicle. The first delivery after leaving DO uses `fuelAtStart` from the DO load.
- **No return observed**: Burn computed from last known fuel to current delivery fuel. Trip remains "open" until next load event.
- **Refill between deliveries**: Burn probe detects tank increase → skip interval, count refill separately.
- **Vehicle state change (idle→active)**: Not directly hookable — inferred from load/delivery call sequence.

### 4.5 Integration with Billing

Replace the current tariff calculation in `DetourTransfer` (line 1383-1385):

```cpp
// Current (v0.17):
const double freight = g_tripTariff
    ? probeTariff   // TariffFor() uses gLitresPerTrip constant
    : FreightFor(...);  // value share

// New (v0.18):
double perTripLitres = 0.0;
bool   haveActual = GetPerTripBurn(vehicle, tons, &perTripLitres);
double fuelCost   = haveActual
    ? perTripLitres * FuelUnitPrice()
    : TariffFor(tons, ...);  // fallback to calibrated constant
double freight    = fuelCost * (1.0 + gTariffMargin) + gWearLitres * gDieselDensity * FuelUnitPrice();
```

If `haveActual` is false (no trip boundary detected), fall back to `gLitresPerTrip` constant. Log which source was used for each payment.

### 4.6 Configuration Keys

| Key | Default | Meaning |
|-----|---------|---------|
| `per_trip_fuel` | `1` | Enable per-trip burn measurement |
| `per_trip_fallback` | `1` | Fall back to `litres_per_trip` when actual unavailable |
| `per_trip_log_per_day` | `60` | Per-delivery [TRIP FUEL] lines |

---

## 5. Implementation Steps

### Step 1 — Add Trip State Data Structures
- Add `TripState` struct and `g_trips[]` array
- Add helper `FindTripSlot(void* vehicle)` — linear scan (max 64 vehicles)
- Add `GetPerTripBurn(void* vehicle, double tons, double* outLitres)` — compute burn from stored trip state

### Step 2 — Hook into DetourTransfer
In `DetourTransfer`:
- At **mode 0, tons < 0** (SITE_LOAD): record `fuelAtStart` for `vehicleHome`
- At **mode 0, tons > 0** (SITE_DELIVERY): record `fuelAtDeliver`, compute burn since last load for same vehicle
- At **mode 1, tons < 0** (SITE_TRANSSHIP return): record `fuelAtReturn`, close trip
- At **SITE_REFUEL** at home DO: reset trip state

### Step 3 — Replace Tariff Formula
In the billing path (line ~1383):
- Use per-trip burn litres × live price × (1 + margin) + wear
- Fall back to `gLitresPerTrip` constant when no actual available
- Log `[TRIP FUEL]` line with actual litres vs configured litres

### Step 4 — Add Logging
- `[TRIP FUEL] v=%p burn=%.4f t = %.2f L | start=%.4f t load=%.4f t deliver=%.4f t return=%.4f t | charged %.2f`
- Daily summary: `[TRIP FUEL] run: %.1f L over %llu delivery(ies) = %.3f L/trip | actual %.2f vs configured %.2f`

### Step 5 — Auto-Tune (Optional)
If `litres_auto = 1`:
- EMA update: `gLitresPerTrip = 0.70 * old + 0.30 * measuredPerTrip`
- Only when per-trip actual available for N consecutive deliveries

---

## 6. Calibration and Validation (Deferred to In-Game Testing)

### 6.1 What Needs Real In-Game Verification

| Question | Method | Status |
|----------|--------|--------|
| Does SITE_LOAD always fire when vehicle leaves DO? | Log every SITE_LOAD with vehicleHome vs building | Pending |
| Does SITE_DELIVERY always fire at destination? | Log every SITE_DELIVERY vs destination building | Pending |
| Is vehicleHome stable across trip? | Check veh+0x4F8 doesn't change mid-trip | Pending |
| How accurate is per-delivery burn vs refuel-anchored? | Compare per-delivery sum to daily tank delta | Pending |
| Multi-drop route behavior | Test vehicle making multiple deliveries per load | Pending |

### 6.2 Test Procedure
1. Build plugin with `per_trip_fuel = 1`, `per_trip_log_per_day = 200`
2. Run disposable save with one private DO, one or two vehicles
3. Log every `[TRIP FUEL]` line for one full day
4. Compare `[TRIP FUEL] run` total to `[TANK] run` burn total
5. Investigate discrepancies (refills, fleet changes, missed boundaries)

### 6.3 Acceptance Criteria
- [ ] Per-trip burn totals match fleet-level tank deltas within 10% over 24h
- [ ] No trip boundary missed for >5% of deliveries
- [ ] Refill events correctly excluded from burn
- [ ] Tariff payments match measured fuel cost × (1 + margin)
- [ ] Fallback to `litres_per_trip` works when trip boundary undetected

---

## 7. Files to Modify

| File | Changes |
|------|---------|
| `plugins/private_logistics/private_logistics.cpp` | Add `TripState` struct, trip detection in `DetourTransfer`, per-trip burn calculation, replace tariff formula, add `[TRIP FUEL]` logging |
| `plugins/private_logistics/private_logistics.ini` | Add `per_trip_fuel`, `per_trip_fallback`, `per_trip_log_per_day` keys |

No changes to:
- `ui_panels.cpp` — no UI changes needed
- `enterprise_base.cpp` — no cross-plugin linking
- Other plugins — isolated to `private_logistics`

---

## 8. Risk Assessment

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| Trip boundary missed (no SITE_LOAD/DELIVERY for some routes) | Medium | Medium | Fallback to `litres_per_trip` constant |
| Refill between deliveries counted as negative burn | Low | Low | Clamp to 0, count refill separately |
| Multi-drop route splits one physical trip into multiple | Medium | Low | Per-delivery granularity is acceptable per user decision |
| VehicleHome pointer stale/invalid | Low | High | `ReadablePtr` guard, skip trip if unreadable |
| No explicit "leave DO" event — departure inferred | High | Medium | Start at first SITE_LOAD after refuel at DO |

---

## 9. Out of Scope (v0.18)

- Per-vehicle-class fuel rates (requires G3/G9: vehicle purchase price, wear fields)
- Refuel-site billing (SITE_REFUEL already hooked but not billed — separate feature)
- Vehicle state machine polling (no per-frame hooks allowed)
- Real-time position tracking (no movement hooks available)

---

## 10. Calibration Parameters for In-Game Testing

These values are starting points and **must be calibrated against real in-game tests**:

| Parameter | Starting Value | Source |
|-----------|---------------|--------|
| `litres_per_trip` | `14.75` | v0.10 tank measurement (one fleet, one day) |
| `litres_per_tonne` | `0.0` | FALSIFIED — Ghidra FUN_14016E3B0 shows load does not burn |
| `diesel_density` | `0.00100` | t/L — game uses 1 kg/L |
| `tariff_margin` | `0.35` | 35% over fuel cost |
| `wear_litres` | `5.0` | Maintenance reserve (poliy, not physics) |

**Calibration target:** After in-game testing, `per-trip actual L/trip` should converge to a stable value per vehicle class. `litres_per_trip` should be replaced by per-vehicle measured values when `per_trip_fallback = 0`.