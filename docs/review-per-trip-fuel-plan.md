# Review — `implementation-plan-per-trip-fuel.md`

**Reviewed:** `docs/implementation-plan-per-trip-fuel.md` (v0.17 → v0.18 plan)
**Review date:** 2026-10-23
**Evidence base:** `plugins/private_logistics/private_logistics.cpp` (3639 lines), `plugins/private_logistics/private_logistics.ini`, `docs/engine/vehicle-fuel-system.md`, `src/resource_pricing_api.h`.

---

## 0. Verdict

**Not ready to implement as written.** The measurement target is right and the offsets are correctly cited, but the plan's central event model is impossible with the current hook surface, the pricing arithmetic has two errors that would mispay by orders of magnitude, and the validation design would pass a wrong model.

| Area | Assessment |
|---|---|
| Offsets and fuel source (`veh+0x3B8` slot, `+0x5F0` mirror) | **Sound** — matches the engine report and the existing reader |
| "Load does not burn" / `litres_per_tonne = 0` | **Sound** — matches Ghidra and the v0.10 log |
| Live-price + margin + wear structure | **Sound in concept** — but the new arithmetic breaks it |
| Trip-boundary event model | **Broken** — assumes a vehicle at events that carry only a building |
| Pricing units | **Broken** — `litres × price_per_tonne` is 1000× too high |
| Fallback arithmetic | **Broken** — margin and wear applied twice |
| `SITE_REFUEL` as trip boundary | **Unsound** — unobserved, stochastic, active-only, no vehicle argument |
| Return detection | **Wrong site** — the cited site is the vehicle side of *every* delivery |
| Multi-drop / per-delivery | **Under-specified** — loses deadhead/return legs, overcharges wear |
| Validation | **Mismatched populations** — fleet-wide burn vs priced-delivery count |
| "Full cost" | **Not delivered** — fuel + a 5 L policy reserve, no wages/depreciation/maintenance |

The plan is a useful first draft: it asks the right questions and identifies the correct fuel source. It needs a rewrite of §4 (design), §4.5 (integration), and §6 (validation) before any code.

---

## 1. What the plan gets right

Keep these:

1. **The authoritative tank is the cargo slot, not `+0x5F0`.** The plan's §3 table and §4.3 use the slot, and treat `+0x5F0` as a cross-check. That matches `docs/engine/vehicle-fuel-system.md` §2 and the existing `FuelSlotTons()` / `MirrorTons()` (`private_logistics.cpp:3039-3080`).
2. **Load does not burn.** `litres_per_tonne = 0` is correct: `fVar24` in `FUN_14016E3B0` is building-level and cargo weight never enters the formula (doc §3.3; ini lines 89-93).
3. **The tariff must be denominated in physical units × live price.** The existing `TariffFor()` (`private_logistics.cpp:959-990`) does this correctly; the plan preserves the intent.
4. **Fail-closed, with a fallback and a log of which source was used.** The intent is right; the implementation in §4.5 does not achieve it (see B4, G4).
5. **Per-vehicle state is the right direction.** `VehSlot()` (`private_logistics.cpp:2966`) already maps a vehicle pointer to a census slot; the plan should reuse it instead of adding a second linear-scan table.
6. **Deferred calibration is honest.** Several of the plan's "calibration questions", however, are blocking design questions, not calibration (see §7).

---

## 2. Blocking findings

### B1. The mode-0 event has no vehicle. The plan's detection table assumes one.

Plan §4.2 and §5 Step 2 say:

> At **mode 0, tons < 0** (SITE_LOAD): record `fuelAtStart` for `vehicleHome`

> At **mode 0, tons > 0** (SITE_DELIVERY): record `fuelAtDeliver`

> At **mode 1, tons < 0** (SITE_TRANSSHIP return): record `fuelAtReturn`

But in the actual detour:

- `p2` is the **building** at mode 0 and the **vehicle** at mode 1 (`private_logistics.cpp:1146-1147`).
- `vehicleHome` is only read when `mode != 0` (`:1179-1182`). At mode 0 it is `nullptr`.
- `TSM_VEH_HOME` is annotated "Building* - the owning depot **(mode 1 only)**" (`:355`).
- `s.vehicle` is set to `p2` only for mode 1; at mode 0 it is `nullptr` (`:1202`).
- The billing block that the plan wants to modify runs at **mode 0 only** (`:1332`).

Therefore:

- `GetPerTripBurn(vehicle, ...)` cannot be called where the plan calls it. There is no `vehicle` in scope.
- `ReadTank(vehicle)` cannot be called at a mode-0 load or delivery. There is no vehicle.
- `vehicleHome` is a **building** pointer (the home building), not a vehicle. The plan uses it throughout as if it identified a vehicle. The condition "`vehicleHome == DO`" is true for every vehicle owned by that building; it does not identify a trip or a vehicle.

This is not an implementation detail. It invalidates §4.2, §4.4, §5 Step 2, and the §4.5 integration. The only events that carry the vehicle are mode 1. A design that measures per vehicle must either move the measurement to mode 1 or add a new hook — and the plan says it will add none.

### B2. `param_9` is not the destination.

Plan §3:

> `param_9` = destination building; `NULL` = resolve from order

The code's own comment says the opposite (`private_logistics.cpp:1148-1152`):

> `p9 = param_9. The FLEET OWNER on the delivery family, NULL on the load family. NOT an endpoint.`
> `Getting these two the wrong way round is the single easiest way to build a payment that pays nobody...`

The site table also says `0x0068F792` is "mode 0, +tons, param_2 = building, **param_9 = fleet owner**" (`:629`).

Consequences:

- Any rule that treats `p9` as the destination compares the wrong building.
- The destination on the delivery family is `building = p2` at mode 0, not `p9`.
- The correct home/return test already exists: `isStaging = (mode != 0 && vehicleHome && vehicleHome == p9)` (`:1197`). At mode 1, `p9` is the receiving building; when it equals the vehicle's home, the vehicle is at home. The plan never mentions `isStaging` and instead invents a `SITE_TRANSSHIP` rule.

This is the most dangerous factual error in the plan because it repeats the exact confusion the code comments warn about.

### B3. The fuel price is per tonne; the plan multiplies litres by it.

`src/resource_pricing_api.h:104` says:

> The live price of **one ton** of the given resource, in the requested currency.

`TariffFor()` therefore converts litres to tonnes before multiplying (`private_logistics.cpp:969-973`):

```cpp
const double fp   = FuelUnitPrice();      // RUB per tonne
fuel = work * gDieselDensity * fp;        // litres × t/L × RUB/t
```

with `gDieselDensity = 0.00100` t/L (`:543`).

Plan §4.3 and §4.5 do:

```cpp
double litres   = burnTons / gDieselDensity;  // 0.01475 t → 14.75 L
double fuelCost = litres * FuelUnitPrice();   // 14.75 × 133 = 1961.75 RUB
```

The correct value is `burnTons * FuelUnitPrice()` = `0.01475 × 133 = 1.96 RUB`, or `litres * gDieselDensity * FuelUnitPrice()`. The plan's own wear line uses the density:

```cpp
double wearCost = gWearLitres * gDieselDensity * FuelUnitPrice();
```

which makes the inconsistency obvious. **The fuel term is 1000× too large.** At the ini's own 133/RUB example, a 14.75 L trip that should cost ~3.31 RUB (with margin and wear) would be charged ~2649 RUB. The `daily_pay_cap = 5000` would trip after two deliveries, and the `[TARIFF]` log would be nonsense.

This is the single most damaging arithmetic error and it is repeated in both §4.3 and §4.5.

### B4. The fallback double-applies margin and wear.

Plan §4.5:

```cpp
double fuelCost = haveActual
    ? perTripLitres * FuelUnitPrice()
    : TariffFor(tons, ...);          // fallback to calibrated constant
double freight  = fuelCost * (1.0 + gTariffMargin) + gWearLitres * gDieselDensity * FuelUnitPrice();
```

`TariffFor()` returns a **total**: `fuel * (1 + margin) + wear` (`:985-986`), not a fuel cost. In the fallback branch:

```
freight = (fuel × 1.35 + wear) × 1.35 + wear
        = margin applied twice, wear applied twice (and wear gets margin)
```

At the shipped values (14.75 L, 133/RUB, 5 L wear, 35% margin):

- intended `TariffFor` total: `1.96175 × 1.35 + 0.665 = 3.313 RUB`
- plan's fallback: `3.313 × 1.35 + 0.665 = 5.138 RUB` → **~55% overcharge on every fallback**

Fix: either branch cleanly

```cpp
double freight;
if (haveActual && fp > 0.0) {
    const double fuel = burnTons * fp;                  // B3 fix
    const double wear = gWearLitres * gDieselDensity * fp;
    freight = fuel * (1.0 + gTariffMargin) + wear;
} else {
    freight = TariffFor(tons, &fuel, &wear, &index);    // total; no extra terms
}
```

or refactor `TariffFor()` to accept an override `work` in litres and keep one total path. Do not add margin/wear twice.

### B5. `SITE_REFUEL` is not a trip boundary.

The plan uses `SITE_REFUEL` (`0x16F831`) as the "trip end marker" (§4.2) and as the anchor for "start at first SITE_LOAD after refuel at DO" (§8 risk mitigation). The evidence does not support this:

1. **It has never been observed.** The plugin's own site table says `0x0016F831` is "refuel call - telemetry, never billed; **never observed**" (`:619`). The plan's §3 table lists the site without that caveat and §9 calls it "already hooked but not billed". A site that has never fired cannot anchor trip state.
2. **It is stochastic, not a boundary.** The refuel is a random trigger inside `FUN_14016E3B0` after burn (`docs/engine/vehicle-fuel-system.md` §4.1): a roll versus a threshold, then a transfer from `veh+0x4F8`. It is not a deterministic "trip ended" event.
3. **It only runs while the vehicle is active.** The burn gate says a vehicle is entirely skipped unless `veh+0x448 ∈ {1,2}` and `veh+0x440 == 9`: "no burn, **no refuel**" (doc §3.4). Refuel therefore happens while the vehicle is in the active state, not while parked at the DO. The doc does not require the vehicle to be at home for the refuel transfer; it draws from `veh+0x4F8` regardless. If the engine refuels an active vehicle away from home, the plan's "refuel = trip end" truncates a trip mid-route.
4. **The mode-0 refuel call carries no vehicle.** `param_2 = *(veh+0x4F8)` is the home *building* (doc §4.2). The detour has no vehicle at mode 0, so it cannot attribute the refuel to a vehicle or reset that vehicle's trip state.
5. **The resource pointers may differ.** The refuel transfer uses `res = *(game + 0xC330)` (doc §4.1), while the tank slot is matched against `*(game + 0xC3F0)` (doc §2.2). The plan does not establish that these are the same resource. If they are different keys (e.g., an order resource vs a cargo resource), the refuel event's `res` and the tank's resource are not interchangeable.

The plan's own risk table ranks "No explicit 'leave DO' event" as **High** and mitigates it with "Start at first SITE_LOAD after refuel at DO" — a mitigation that depends on B5 being true. It is not. The mitigation is circular.

**Recommendation:** drop `SITE_REFUEL` as a trip boundary entirely. Detect refills from the per-vehicle tank **increase** at the next mode-1 event; the existing `ReadTank()` already does exactly this (`level > prev + 0.02` → count refill, do not subtract: `:3264`). If an interval spans a refill, skip that interval and count it, or split it if a reliable refuel event is available (it isn't). Do not reset/close a trip on a refuel.

### B6. Return detection misclassifies every delivery; the trip legs are incomplete.

Plan §4.2:

> Return to DO | SITE_TRANSSHIP | 1 | `-tons` | Yes | Record `fuelAtReturn = ReadTank(vehicle)`

The site this refers to, `0x0068F7F9`, is described by the code as:

> "family A **delivery, vehicle side**: mode 1, `-tons`. NEVER BILLED" (`:630`)

It is the vehicle side of **every delivery**, at the destination, not a return-to-depot unload. Using it as a return marker records `fuelAtReturn` at every drop and "closes" the trip at each delivery.

The actual home test is `isStaging` (`:1197`): at mode 1, the receiving building `p9` equals `vehicleHome`. Use that. (Note: `isStaging` currently classifies as `SHIP_LOAD`, which is a label oddity, but the predicate is the correct home signal.)

Separately, the plan's interval boundaries lose legs:

- §4.2 starts at `SITE_LOAD` at the **source**, so the DO → source deadhead is never measured. Yet §1 says the trip is "DO → load → deliver → return to DO".
- §4.4 computes at the **next load**, so the return leg after the last delivery is attributed to the next trip or lost.
- Per-delivery granularity with a fixed wear/margin per payment charges the reserve once per drop, not once per trip (see G7).

The result is a systematic undercount of fuel (missing deadhead/return) and a systematic overcount of wear on multi-drop routes. The plan's risk table calls multi-drop impact "Low"; it is at least Medium because the fixed wear is per payment.

**Recommendation:** use a continuous per-vehicle tank baseline (see §6), not per-trip resets, and attribute each delta to the leg that just completed.

---

## 3. Serious design gaps

### G1. No vehicle resolution at the billing point.

The carrier ring stores `res`, `tons`, `home`, `t` — **no `vehicle`** (`CarrierNote`, `:684-689`). The plan calls `g_carrier` "existing infrastructure reusable" for per-vehicle state, but as written it can only resolve the **home building**. Extending it with `vehicle` is a code change the plan does not list. Even then, its match is by `home + res + |tons|` within 2 s (`:1249-1257`); a multi-drop delivery's tons may not equal the load's tons, so the lookup fails precisely where per-delivery granularity matters. The ring also stores `fabsf(tons)` and does not distinguish load from delivery, so it is not a trip tracker.

### G2. The state machine is circular and under-specified.

`GetPerTripBurn()` is supposed to price the current delivery, but §4.4 says the burn is computed at the **next** load. Inconsistent. The plan does not define:

- a second load of a different resource at the same source (multi-slot trucks): each transfer is a separate call — does the second load start a new trip or continue the same one?
- partial deliveries (delivery tons < load tons) and multiple deliveries per load;
- a load at the DO itself (DO with storage) vs a load at an external source;
- a vehicle re-homed mid-trip (`veh+0x4F8` can change; the code comments note it can be re-homed later in the tick);
- a vehicle despawned/sold with an open trip.

The `TripState` struct also has unused/redundant fields: `homeDO`, `active`, `lastLoadT`, `lastDeliverT` are not used in the described logic; `fuelAtLoad` duplicates `fuelAtStart`; `g_tripSlot` is described as "vehicle -> trip index mapping (linear scan)" — a mapping that is linearly scanned is just the array. This suggests the design has not been reduced to a state machine.

### G3. Refill handling contradicts `ReadTank`.

`ReadTank()` (`:3237`) already implements:

- slot-first, mirror fallback (`found`, `level < 0`);
- first-sighting baseline (`!g_vehFuelValid[slot]` → set and return);
- refill test `level > prev + 0.02` → count, do not subtract (`:3264`);
- burn only when `prev > level` (`:3266`);
- negative readings handled via `FuelSlotTons` clamp and mirror fallback.

The plan's §4.3 (which also references an undefined `tankCapacity`; the code has `TankCapacity(v)` at `:3080` and the census caches `g_vehCap[slot]` at `:436`):

```cpp
double burnTons = fuelAtStart - fuelAtReturn;
if (burnTons < 0.0) burnTons = 0.0;   // refill between measurements
```

throws all of that away and **undercharges** every interval that spans a refill: it charges 0 instead of the pre-refill burn. The prose says "skip interval, count refill separately", but the pseudo-code does not. The plan also calls `ReadTank(vehicle)`, which is not a function. The real signature is:

```cpp
ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
```

It needs a slot index and day-close counters, and it mutates the daily `g_vehFuel[slot]` baseline used by `CensusFleets()` (`:3422`). Calling it per event would interleave event-level and daily accounting. Use a separate `ReadTankNow(v)` that returns a validated level and leaves the daily state alone, or refactor `ReadTank()` into a pure reader plus a daily accumulator.

### G4. CPI fallback regression.

`TariffFor()` has two indices: fuel price (index 1) and CPI baskets (index 2, `:974-984`). If neither resolves, it returns 0 and the caller counts `g_tariffNoIndex` (`:1365`) and skips payment. The plan's measured path computes `perTripLitres * FuelUnitPrice()`; if `FuelUnitPrice()` is 0, fuel cost is 0 (and wear is 0), so `freight <= 0` and the delivery is **skipped**, even though the measured burn is valid. That is a regression from the CPI fallback.

The plan says "fall back to calibrated constant when actual unavailable", not "when the index is unavailable". Fix:

- if `fp <= 0`, fall through to `TariffFor()` (which prices the constant work in baskets);
- preserve `probeIndex` in the new path so the `[TARIFF]` log still says which index was used;
- if the user wants measured burn in the CPI path, add an explicit `baskets_per_litre` policy parameter — none exists today.

### G5. Factory payment on `SITE_REFUEL` (latent double-pay).

At mode 0, `tons < 0`, the code pays `building` as a factory sale (`:1414-1421`) **without checking `s.kind`**. `SITE_REFUEL` is classified `SHIP_FUEL` (`:1212-1215`), but the factory block does not exclude it. If the refuel site ever fires (the plan assumes it will), a private DO that owns the home building would be paid `|tons| × unitPrice × factory_share` for **drawing its own fuel** — an internal transfer booked as a sale.

Fix: gate the factory payment on `s.kind != SHIP_FUEL && known != SITE_REFUEL`, and add a counter/log if a `SHIP_FUEL` reaches that branch. Even if `SITE_REFUEL` never fires, any `tons < 0` fuel transfer from a private building has the same shape.

### G6. Lifetime, capacity, and save/load.

- `TRIP_VEH_MAX 64` with no eviction: open trips ("remains open until next load") can fill the array; new vehicles are dropped. The census is global across up to 32 enterprises × 64 vehicles (`:3327`), so 64 trip slots is not obviously enough.
- `void* vehicle` is "transient, never persisted", but the plan does not say to clear `g_trips` on save/load. A load can invalidate vehicle pointers; `ReadablePtr` prevents a crash but not pointer **reuse**, which can alias a different vehicle and misattribute burn. The plugin has a persistence callback (`AutoPersistenceEvent`, `:588`) to hook a reset.
- No stale/timeout policy despite `lastLoadT`/`lastDeliverT` fields. Add a TTL (e.g., skip/close intervals older than N minutes or after a day close) and an eviction rule (oldest inactive slot).

### G7. Wear/margin per delivery vs per trip.

The plan's §4.3/§4.5 adds `wear` and `margin` per payment, and §4.4 makes payments per delivery. On a 3-drop route the carrier receives 3× the fixed wear reserve. The ini defines `wear_litres` as a **per-trip** sinking fund (`private_logistics.ini:95-103`: "what you decide a trip should put aside"). At shipped values, a 3-drop route charges 1.995 RUB of wear instead of 0.665 RUB — the overcharge is ~68% of the entire fuel cost of a short trip.

Decide explicitly: charge wear once per trip (e.g., on the first delivery of the trip, or prorate 1/N across the trip's deliveries) and document it. Same for the margin if it is meant to cover per-trip overhead rather than per-delivery handling.

### G8. Config precedence and auto-tune conflict.

`per_trip_fuel`, `per_trip_fallback`, and `per_trip_log_per_day` are added without defining interaction with `billing`, `dry_run`, `trip_tariff`, `tariff_probe`, `balance_log`, `log_cap_per_day`, and `litres_auto`:

- If `trip_tariff = 0` (value-share path), does `per_trip_fuel = 1` still measure/log? The measurement is useful; the payment must stay the value share. Say so.
- If `per_trip_fuel = 0`, is the path exactly v0.17? If `per_trip_fallback = 0` and no actual, skip payment or use `TariffFor`? The plan says fallback is optional but not what "off" does.
- `litres_auto` already EMA-tunes `gLitresPerTrip` from the tank (`g_litresAuto`, `:1850-1858`, 0.70/0.30). Step 5 adds a second EMA from the per-trip measurement. Two tuners will fight, and the per-trip metric is the one the code's v0.11 comment calls "a ratio of two unrelated things" (`:1836-1839`). Either reuse `g_litresAuto` with a per-vehicle rate, or disable one path explicitly. (The ini comment says 85/15 while the code uses 70/30 — an existing doc/code mismatch, but not the plan's.)
- `per_trip_log_per_day` needs a reset in the day-close block where `g_tariffLoggedToday` etc. are reset (`:1648-1651`). The plan does not mention it.

### G9. "Full cost" is not full cost.

The plan's cost model is `fuel × (1 + margin) + wear_litres × density × price`, with `wear_litres = 5.0` described in the ini as **"POLICY, not physics"** (`:95-103`). It excludes:

- **driver wages** (the `citizen_labor` plugin exists; no link);
- **vehicle purchase/depreciation** (the ini's `charge_vehicle_purchase = 1` is under "NOT IMPLEMENTED", `:210-217`);
- **maintenance/repair** (the 5 L reserve is a token: 5 L × 0.001 × 133 = 0.665 RUB/trip; the ini's own example for a 6,000 RUB truck is ~13 L/trip, `:99-102`);
- **insurance, overhead, loading/unloading**, and the portion of deadhead the state machine does not capture;
- **the distinction between the DO's fuel purchase** (`charge_fuel_at_pump = 0`, `:215`) **and the carrier's fuel burn**.

Also, `veh+0x4F8` is the owning depot/garage, and the census prints `home=this/other` without requiring the home to be a distribution office. If the tariff is only for distribution offices, gate on the building type; otherwise call it the "home building", not the "home DO".

If "full cost" means total cost per trip, this plan is a fuel-cost component, not a full-cost model. Either rename it ("per-trip fuel cost") or add the missing terms with explicit data sources and cross-plugin work. The §9 "Out of Scope" list confirms the gap: it defers per-vehicle-class rates and vehicle purchase/wear fields.

### G10. Per-vehicle-class rates are feasible; the out-of-scope rationale is wrong.

§9 says per-vehicle-class rates "requires G3/G9: vehicle purchase price, wear fields". But the repo's own RE report gives the engine's per-vehicle consumption rate:

```
slot_rate = *(float *)(*(veh+0x1708) + 0x85E0 + 4 + slot_index * 0x10)
```

(`docs/engine/vehicle-fuel-system.md` §3.2), and the plugin already reads `*(veh+0x1708)+0x8684` for capacity (`TankCapacity`, `:3080`). The slot rate is per-vehicle metadata, not purchase price. It can be read to:

- validate measured burn;
- seed a per-class fallback;
- detect a vehicle that is not burning (wrong kind / no fuel slot).

Caveat: the rate is per tick and is scaled by building `fVar24`, so it is a validation signal, not a replacement for the measurement. But the plan's blanket deferral is more conservative than necessary.

### G11. Replacing only the `freight` line leaves the tariff probe and log on the old model.

Plan §5 Step 3 says "In the billing path (line ~1383): use per-trip burn...". Line 1383-1385 is the `freight` assignment. But `probeTariff` is computed earlier (`:1350`) by `TariffFor()` and is also used for:

- the `[TARIFF]` log (`:1402-1410`), which prints `probeTariff`, `probeCost`, and `probeShare`;
- the run counters `g_tariffSum`, `g_tariffCostSum`, `g_tariffShareSum` (`:1356-1360`).

If you replace only `freight`, the log and counters still show the constant-based number, so the calibration log no longer matches the money. If you replace `probeTariff` itself, you need to restructure `TariffFor()` to accept the measured work and preserve the `probeCost`/`probeShare`/`probeIndex` semantics. The plan does neither.

Also, `BurnProbe(paid)` is called only when `Pay()` succeeds (`:1393`). The v0.5 comment says "the probe runs even with billing off" (`:1330-1331`), but it does not: the whole `if (g_billing && g_payCarrier)` block is skipped when billing is off. The new measurement should run **outside** the payment branch, otherwise the plan's calibration has no data in instrument-only mode. This is an existing inconsistency, but the plan's design inherits it.

---

## 4. Factual and documentation errors

| Plan claim | Evidence | Impact |
|---|---|---|
| `param_9` = destination building | `private_logistics.cpp:1148-1149` (fleet owner, not an endpoint); `:629` (fleet owner) | Wrong building in every rule |
| `mode=1` = resource flows vehicle→building | `:633` — `0x6953C4` mode 1 **+tons** (vehicle-side load) | Sign-based rules misclassify |
| `0x6953C4` = "Ship/line delivery" | `:633` — "family A **load**, vehicle side: mode 1, +tons" | If used as a delivery marker, wrong |
| `0x68F7F9` = "Return-to-depot unload" | `:630` — "family A **delivery**, vehicle side: mode 1, -tons" | Fires at every destination |
| `0x68EECA` / `0x1CB7FF` = SITE_LOAD under the plan's `tons < 0` rule | `:627-628` — mode 0 **+tons** | Rule misses them if they fire |
| `0x1B2898` = "Road delivery unload" | `:620` — "CONTESTED ... never observed" | Unverified site treated as classified |
| `0x16F831` = "DO refuel" usable for trip state | `:619` — "never observed"; doc §3.4 active-only; doc §4.1 stochastic | Trip boundary depends on an unobserved event |
| `transfer(game, bld, mode, res, tons, &out)` | Actual detour has 9 params incl. `cap`, `flag`, `p9` (`:1140-1142`) | Loses the `p2`/`p9` distinction |
| `litres_per_trip = 14.75` = "v0.10, one fleet, one day" | ini `:75-85` — v0.9 run, 51 days, 139 deliveries; v0.10 same office gave **10.28** | Fallback provenance and stability are wrong |
| `SITE_REFUEL` "already hooked but not billed" (§9) vs "trip end marker" (§4.2) | same plan | Contradictory scope |
| "No Ghidra investigations were blocked" | open questions in §7 | Overconfident; several are blocking |

Also:

- §2 says "Fuel measurement: Tank readings at every close". In the code, `ReadTank()` is called once per vehicle per **day** in `CensusFleets()` (`:3422`), while `BurnProbe()` is the per-**delivery** **fleet-level** sample (`:1393`). The plan conflates the two.
- §2 says "`BurnProbe()` at every committed delivery". It is called only when a payment succeeds (`:1393`); if `paid` is null or `freight <= 0`, no probe.
- The plan's site-class labels are copied from `g_sites[]`, but the code explicitly says: *"The instrument does not act on the label — every site is logged the same way"* (`:594-596`). Using `known` as a control signal (SITE_LOAD/SITE_DELIVERY/SITE_TRANSSHIP) violates that design principle. Use `mode`, the sign of `tons`, `isStaging`, and `p9` instead; keep `known` for log readability.

---

## 5. Validation and calibration flaws

### V1. The acceptance test compares different populations.

§6.2 says: "Compare `[TRIP FUEL] run` total to `[TANK] run` burn total." But `[TANK] run` is:

- **numerator** `g_burnTonsRun`: burn of **every vehicle of every enterprise** walked by `CensusFleets()` (up to 32 enterprises, `:3327`; `ReadTank()` at `:3422`). `CensusFleets()` does not filter on `IsPrivate`, so this includes vehicles that are not doing the plugin's private deliveries;
- **denominator** `g_fuelTripsRun`: `tCount = g_tariffCount`, the number of deliveries with a **positive tariff** (`:1355`), not all deliveries.

`[TRIP FUEL] run` would be only the plugin's paid deliveries. The two totals cannot match within 10%, and a match would be a coincidence. The code's own v0.11 comment (`:1829-1832`) says the tank burn is "what actually left the vehicles' tanks while they worked" — but "they" is the whole census, not the paid fleet.

### V2. Fleet-level deltas cannot validate per-vehicle attribution.

A fleet-level delta is invariant to swapping which vehicle burned the fuel. A per-vehicle model can be wrong in attribution and still match the fleet total. Validate **per vehicle**: sum `[TRIP FUEL]` deltas for vehicle V and compare to `ReadTank()`'s per-vehicle `burned` for V over the same window, with refills excluded. Only then compare fleet totals as a secondary check.

### V3. "No boundary missed for >5% of deliveries" is undefined.

There is no ground truth for a "missed boundary". If the code falls back silently, the count is unknowable. Add a `source=` field to every `[TRIP FUEL]` line (`measured`, `fallback:no-baseline`, `fallback:refill`, `fallback:no-vehicle`, `fallback:invalid`, `fallback:no-index`) and a daily count per source. Then "missed" = deliveries where a mode-1 pair was present but no measurement was produced. Without that, the criterion cannot be evaluated.

### V4. The fallback constant isn't a constant.

The ini says 14.75 came from a 51-day, 139-delivery run and that the same office gave **10.28** in v0.10 (`:75-85`). The code's v0.11 comment says L/trip is "a ratio of two unrelated things" and that L per active truck-day is the physical rate (`:1836-1839`). Using 14.75 as a fallback is a policy choice, not a calibration. If you keep it:

- label it "shipped default, fleet-specific";
- add a per-vehicle fallback (e.g., the last measured value for that vehicle) or use `TariffFor()` with the configured constant;
- do not tell the reader it is a converged per-trip value.

### V5. The test procedure doesn't isolate the measurement.

§6.2 uses "one private DO, one or two vehicles", which is good, but:

- it does not say to disable `litres_auto` (otherwise the fallback changes during the test);
- it does not include a state-owned DO control, a multi-drop route, a refuel mid-trip, a partial delivery, or a vehicle re-homed/sold;
- it does not log the per-vehicle tank at each event, so a mismatch cannot be localized;
- it does not reconcile the event-level baseline with `ReadTank()`'s daily baseline; if both are active, the daily and per-event deltas must be reconciled by design, not by hope.

Add a table per vehicle: `t, event, building, tank_before, tank_after, delta, source, charged`. That is the calibration artifact.

---

## 6. Recommended corrected design

### 6.1 Choose the measurement point first.

Two viable architectures, both using only existing hooks (no new call-site patches):

**Option A — defer the carrier payment to the mode-1 pair (recommended).**
At a mode-0 delivery (`tons > 0`, `building`, `p9`), do not pay yet. Stash a `PendingDelivery { building, owner, res, tons, unitPrice, t }` in a small ring. At the matching mode-1 delivery (same `res`, `|tons|` within tolerance, same `home`, within ~2 s), you have `p2` = vehicle and `vehicleHome` = home building. Read the tank, compute the delta since that vehicle's last reading, and call `Pay()` for the stashed delivery. If no pair arrives within the window, pay the stashed delivery at timeout using `TariffFor()` (fallback) and count it. This gives exact per-vehicle, per-delivery attribution; the payment is delayed by the 30-70 ms the code already observes between the pair. The factory leg at mode 0 (`tons < 0`) can stay where it is; it is a different payment.

Risks: a missing mode-1 pair; `Pay()` now happens outside the mode-0 block, so `daily_pay_cap`, the payment counters, and log ordering need a check; the destination building must be carried in the stash for the `[TARIFF]` log.

**Option B — keep billing at mode 0, accept a one-trip lag.**
At every mode-1 event, update the vehicle's baseline and store the completed interval's burn in the trip state. At a mode-0 delivery, resolve the vehicle from an extended ring (add `vehicle` to `CarrierNote`) and pay using the **last completed interval** for that vehicle. The first delivery after plugin start or after a gap uses `TariffFor()`. Simpler, no deferred payment, but the charge lags one delivery, and a multi-drop delivery's tons may not match the ring's load note. Use `home + res` and the most recent event for that home, not exact tons, to improve it.

Do **not** keep the plan's current hybrid (read at mode 0, no vehicle): it cannot be implemented.

### 6.2 Use a continuous per-vehicle baseline, not per-trip resets.

Per vehicle:

```cpp
struct VehFuelState {
    void*  vehicle;        // key, validated each event
    void*  home;           // veh+0x4F8 at last event
    double lastTons;       // validated tank at last event
    double lastT;          // NowSeconds()
    int    lastKind;       // load / deliver / return
    double tripFuel;       // accumulated for the current trip
    bool   wearCharged;    // charge the per-trip reserve once
    bool   valid;
};
```

At each mode-1 event:

1. `now = ReadTankNow(vehicle)`; if invalid, mark the interval invalid and fall back.
2. If `!valid`: set baseline, return (first sighting).
3. If `now > lastTons + 0.02`: refill; count it, set baseline, do **not** charge the interval (or split if a reliable refuel event is available; it isn't).
4. Else `delta = lastTons - now`; if `delta < 0` or `delta > capacity`, invalid; fall back.
5. Attribute `delta` to the leg that just completed:
   - **load**: deadhead to the source → add to `tripFuel` for the new trip;
   - **delivery**: loaded leg → add to `tripFuel` and to the delivery's charge;
   - **return** (`isStaging`): return leg → add to `tripFuel` and the last delivery's charge (or the trip's final settlement).
6. Set `lastTons = now`, `lastT = now`, `lastKind`.

This captures DO→source, source→destination, destination→destination (multi-drop), and destination→DO. It handles refuels without `SITE_REFUEL` and gives a per-trip total for wear allocation.

### 6.3 Pricing.

```cpp
const double fp = FuelUnitPrice();          // RUB per tonne
double freight;
int    index = 0;
if (measuredValid && fp > 0.0) {
    const double burnTons = measuredTons;    // from the state machine
    const double fuel     = burnTons * fp;   // NOT litres * fp  (B3)
    const double wear     = gWearLitres * gDieselDensity * fp;
    freight = fuel * (1.0 + gTariffMargin) + wear;
    index   = 1;
} else {
    double fuel = 0.0, wear = 0.0;
    freight = TariffFor(tons, &fuel, &wear, &index);  // total; no extra margin/wear (B4)
}
```

- Preserve `[TARIFF]` logging and the `g_tariff*` counters for both paths; add `source=measured|fallback`.
- If `fp <= 0` and a measured burn exists, `TariffFor()` prices the **constant** work in baskets (index 2). That is a deliberate fallback: measured litres cannot be converted to baskets without a litres→basket rate. Say so in the log (`measured burn unavailable in CPI index; priced on constant`). Alternatively, add `baskets_per_litre` if the user wants the measured burn in the CPI path; that is a new policy parameter.
- Apply wear once per **trip**: use `wearCharged`; on multi-drop, either charge it on the first delivery or prorate. Do not charge it per resource transfer.
- Gate the factory payment: `if (g_billing && tons < 0 && g_payFactory && s.kind != SHIP_FUEL && known != SITE_REFUEL)`. Add a log/counter if a `SHIP_FUEL` reaches that branch.

### 6.4 State lookup and capacity.

- Reuse `VehSlot(v)` (`:2966`) for the vehicle→slot mapping; it already caps at `VEH_MAX` and is used by the census. Do not add a second linear-scan table.
- Size the trip/fuel state to `VEH_MAX`; evict the oldest inactive slot when full; add a TTL (e.g., close/skip intervals older than 10 minutes or on day close).
- Clear all state on the persistence load event (hook `AutoPersistenceEvent` or the plugin's load path).
- Validate `vehicle` with `TsmIsValidGamePtr` + `ReadablePtr` at every event; re-read `home` each time (the code warns it can be re-homed).

### 6.5 Logging and config.

- `[TRIP FUEL] v=%p slot=%d home=%s res=%s tons=%.3f leg=%s delta=%.4f t (%.1f L) source=%s trip=%.1f L charged=%.2f` — include `home`/`res`/`leg`/`source`, not just the pointer and burn.
- Daily summary: `measured N/M deliveries, X L, Y L/trip; fallback by reason; refills R; wear charged W times`.
- Reset the per-day counters where the existing counters reset (`:1648-1651`).
- Define precedence:
  - `per_trip_fuel = 0` → exact v0.17 path;
  - `per_trip_fallback = 0` → skip payment when no measured burn (and count a new reason);
  - `trip_tariff = 0` → measurement still runs if enabled, payment stays `FreightFor()`;
  - `dry_run = 1` → measure and log, pay nothing;
  - `litres_auto = 1` → either disable the per-trip EMA or feed it a per-vehicle rate; do not tune the same global from two sources.

### 6.6 Validation plan (replace §6.2/§6.3).

1. One private DO, one vehicle, `litres_auto = 0`, `per_trip_log_per_day = 500`, `tank_cross_log` high, `cargo_dump = 1`.
2. Log per event: `t`, `site`, `mode`, `sign`, `building`, `res`, `tons`, `vehicle`, `home`, `tank`, `source`.
3. At each day close, print per-vehicle `ReadTank()` `burned`/`refills` and the sum of event deltas for that vehicle. They should reconcile to the same total (within the 0.02 t refill threshold) because both are sums of the same tank deltas, split differently. If they don't, an event was missed or double-counted.
4. Acceptance:
   - per-vehicle event-sum vs `ReadTank()` daily-sum within 2% over 24 h (**not** fleet-level 10%);
   - every `[TRIP FUEL]` line has a `source`; the fallback rate is reported by reason;
   - multi-drop: one wear charge per trip;
   - refuel: interval skipped, not charged as 0;
   - save/load: no stale pointer; the first post-load interval is skipped;
   - `fp = 0` forced (rename the resource in `resource_pricing.ini`): the CPI path still pays, and the log says `index=cpi`.
5. Then run a 7-day mixed fleet: state-owned DO control, private DO, two vehicle types. Compare per-vehicle L/active-truck-day to `[TANK]`'s `lva` (`:1842`); that is the physical rate, not L/trip.

---

## 7. Conclusions

1. **The idea is right; the event model is not.** Measuring the tank at transfer events is the correct way to get per-vehicle burn. But the only event that carries the vehicle is mode 1, and the plan bills at mode 0. Fix that first; everything else follows.
2. **Three arithmetic errors must be fixed before any in-game test:** the per-tonne price unit (1000×), the double margin/wear in the fallback (~55% over), and the per-delivery wear on multi-drop (3× on a 3-drop route). These would produce obviously wrong payments and could hit `daily_pay_cap` immediately.
3. **`SITE_REFUEL` should be dropped as a trip boundary.** It is unobserved, stochastic, active-only, and carries no vehicle. Detect refills from the tank increase at the next mode-1 event; the existing `ReadTank()` already does this correctly.
4. **The plan is not a "full cost" plan.** It is fuel + a 5 L policy reserve + margin. Driver wages, vehicle purchase/depreciation, maintenance, and overhead are out of scope, and the ini's `charge_vehicle_purchase` / `upkeep_per_vehicle_day` / `charge_fuel_at_pump` are unimplemented. Either rename it or add a full-cost section with data sources.
5. **The validation design would pass a wrong model.** Fleet-level deltas include state-owned and unrelated vehicles, and the denominator is priced deliveries only. Validate per vehicle against `ReadTank()`, and report fallback reasons.
6. **"No Ghidra investigations were blocked" is not accurate.** At least four questions are open and blocking:
   - **Vehicle identity at the billing event** — architecture, not Ghidra, but unresolved. (Option A solves it.)
   - **When does refuel occur relative to trip boundaries, and can it occur mid-trip?** The doc's gate (`veh+0x448 ∈ {1,2}`, "no burn, no refuel") and the stochastic trigger suggest it can. The plan assumes trip-end. (Continuous baseline + refill detection solves it.)
   - **Are `*(game+0xC330)` (refuel resource) and `*(game+0xC3F0)` (tank slot resource) the same?** If not, the refuel event's `res` and the tank's resource are different keys. (Avoiding `SITE_REFUEL` solves it.)
   - **What are the semantics/signs of `0x68EECA`, `0x1CB7FF`, `0x68F7F9`, `0x6953C4`, `0x1B2898`?** The code's own table has "never observed"/"CONTESTED" for several; the plan presents them as classified. (Use `known` only for logs, not control flow.)
7. **Recommended path:** rewrite §4 around Option A, reuse `ReadTank()`'s validation logic in a pure `ReadTankNow()`, fix the pricing formula, remove the `SITE_REFUEL` dependency, add per-vehicle validation, and keep the fallback fail-closed. The rest of the plan (offsets, live price, margin/wear structure, deferred calibration) is a good foundation.

**Priority:**

- **P0:** B1 (vehicle identity), B3 (price unit), B4 (double margin/wear), G5 (factory-on-fuel).
- **P1:** B5 (refuel), B6 (return/legs), G1 (ring/vehicle), G3 (refill state), G4 (CPI).
- **P2:** G2 (state machine), G6 (lifetime/save-load), G7 (wear allocation), G8 (config), G11 (probe/log integration).
- **P3:** G9 (full cost), G10 (per-class rates), logging polish.

**One-sentence verdict:** *Rewrite the event model and the pricing arithmetic; keep the offsets and the measurement concept.*
