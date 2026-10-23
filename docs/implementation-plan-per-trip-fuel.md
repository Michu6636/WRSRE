# Implementation Plan — Per-Trip Fuel Cost Estimation (v0.18 Revised)

**Target:** `SOVIET64.exe` v1.1.1.9, base `0x140000000`, ASLR on  
**Plugin:** `private_logistics` (v0.17 → v0.18)  
**Status:** Revised plan — addresses all blocking findings in `docs/review-per-trip-fuel-plan.md`  
**Architecture:** Option A — defer carrier payment to mode-1 pair (recommended by review)  
**Measurement:** Continuous per-vehicle tank baseline at mode-1 events  
**Granularity:** Per-delivery, per-vehicle

---

## 1. Summary of Changes from Original Plan

| Area | Original (flawed) | Revised |
|------|-------------------|---------|
| Event model | Read at mode 0, no vehicle | Option A: defer payment to mode-1 pair |
| Trip boundaries | SITE_LOAD → SITE_DELIVERY → SITE_REFUEL | Continuous baseline + refill detection |
| Fuel price | `litres * FuelUnitPrice()` (1000× too high) | `burnTons * FuelUnitPrice()` (correct per-tonne) |
| Fallback | `TariffFor(...) * (1+margin) + wear` (double-applied) | `TariffFor(...)` only (total, no extra terms) |
| SITE_REFUEL | Trip boundary (never observed) | Dropped entirely |
| Return detection | SITE_TRANSSHIP (fires at every delivery) | `isStaging` predicate |
| Vehicle state | New linear-scan table | Reuse `VehSlot()` + continuous baseline |
| Wear allocation | Per delivery (3× on 3-drop) | Once per trip |
| Validation | Fleet-level totals vs priced deliveries | Per-vehicle vs `ReadTank()` daily sum |

---

## 2. Architecture — Option A: Deferred Carrier Payment

The review's recommended architecture. The only event that carries the vehicle is mode 1. The billing block runs at mode 0, so we **defer** the carrier payment:

```
Timeline at each delivery:
  T0   mode-0 delivery call: tons > 0, building, p9 = fleet owner
       → Stash PendingDelivery { building, owner, res, tons, unitPrice, t, destination }
  T1   mode-1 call follows 30-70 ms later: p2 = vehicle, vehicleHome = home
       → Match to pending delivery by home + res + |tons| within 2 s
       → Read tank (ReadTankNow), compute delta since last reading
       → Compute tariff from measured burn × price × (1+margin) + wear
       → Call Pay() for the stashed delivery
  Timeout → Pay stashed delivery using TariffFor() fallback, count as fallback
```

This gives exact per-vehicle, per-delivery attribution with only existing hooks — no new call-site patches.

---

## 3. Data Structures

Reuse existing `VehSlot(v)` for vehicle→slot mapping (already caps at `VEH_MAX`, used by census). Add:

```cpp
#define PENDING_RING 32

struct PendingDelivery {
    void*  building;     // destination building (p2 at mode 0)
    void*  owner;        // fleet owner (p9 at mode 0)
    void*  resource;     // resource pointer
    double tons;         // delivery tons
    double unitPrice;    // live price per tonne
    double t;            // NowSeconds() at stash time
    bool   valid;        // cleared after matched or timeout
};

static PendingDelivery g_pending[PENDING_RING];
static int   g_pendingHead = 0;

struct VehFuelState {
    void*  vehicle;      // validated each event
    void*  home;         // veh+0x4F8 at last event
    double lastTons;     // validated tank level at last event (tonnes)
    double lastT;        // NowSeconds()
    double tripFuel;     // accumulated burn for current trip (tonnes)
    bool   wearCharged;  // per-trip wear reserve charged once
    bool   valid;        // first sighting sets baseline, no interval yet
};

static VehFuelState g_vehFuelState[VEH_MAX];
```

---

## 4. Event Handling

### 4.1 Mode 0 — Stash Delivery (Billing Deferred)

At every mode-0 committed call with `tons > 0.0f`:

```cpp
PendingDelivery* pd = &g_pending[g_pendingHead % PENDING_RING];
pd->building   = building;    // destination (p2 at mode 0)
pd->owner      = owner;       // p9 = fleet owner
pd->resource   = res;
pd->tons       = (double)fabsf(tons);
pd->unitPrice  = UnitPrice(res);
pd->t          = NowSeconds();
pd->valid      = true;
g_pendingHead++;
```

Do **not** compute tariff here. The factory leg (`tons < 0`) can stay at mode 0 with the G5 fix (exclude `SHIP_FUEL`).

### 4.2 Mode 1 — Match, Measure, Pay

At every mode-1 committed call:

```cpp
void* vehicle = p2;  // vehicle IS available at mode 1
void* home = vehicleHome;

// Find matching pending delivery
PendingDelivery* pd = findMatch(home, res, fabsf(tons), NowSeconds());

if (pd && pd->valid) {
    // Read tank now
    double nowTons = ReadTankNow(vehicle);  // pure reader, no day-close state
    int slot = VehSlot(vehicle);
    VehFuelState* st = &g_vehFuelState[slot];
    
    if (!st->valid) {
        st->valid = true;
        st->lastTons = nowTons;
        st->lastT = NowSeconds();
        st->home = home;
        st->vehicle = vehicle;
        pd->valid = false;  // no interval yet, defer
    } else {
        double delta = st->lastTons - nowTons;
        if (nowTons > st->lastTons + 0.02) {
            // refill — skip interval, count refill, reset baseline
            countRefill(slot);
            st->lastTons = nowTons;
            st->lastT = NowSeconds();
            st->wearCharged = false;  // new trip
        } else if (delta < 0.0 || delta > tankCapacity(vehicle)) {
            countInvalid(slot);
        } else {
            // valid burn interval
            st->tripFuel += delta;
            
            double fp = FuelUnitPrice();
            double freight;
            int index = 0;
            if (fp > 0.0 && delta > 0.0) {
                const double fuel = delta * fp;              // tonnes × RUB/tonne
                const double wear = st->wearCharged ? 0.0
                    : gWearLitres * gDieselDensity * fp;     // once per trip
                freight = fuel * (1.0 + gTariffMargin) + wear;
                st->wearCharged = true;
                index = 1;
            } else {
                double fuel = 0.0, wear = 0.0;
                freight = TariffFor(pd->tons, &fuel, &wear, &index);
            }
            
            if (freight > 0.0 && pd->owner) {
                Pay(pd->owner, freight, TSM_CAP_WHOLESALE,
                    "b2b_freight", "freight", pd->resource, (float)pd->tons);
            }
            
            st->lastTons = nowTons;
            st->lastT = NowSeconds();
        }
    }
    
    pd->valid = false;  // consumed
} else {
    // no vehicle match — pay pending delivery with fallback
    flushStalePending();
}
```

Stale pending deliveries (no mode-1 pair within ~2 s) are flushed at each close using `TariffFor()` fallback.

### 4.3 `ReadTankNow()` — Pure Reader

Split `ReadTank()` into a pure reader that doesn't mutate daily state:

```cpp
static double ReadTankNow(void* v) {
    int found = 0;
    double level = FuelSlotTons(v, &found);
    if (!found) {
        const double mirror = MirrorTons(v);
        if (level < 0.0 && mirror < 0.0) return -1.0;
        level = (mirror > 0.0) ? mirror : 0.0;
    }
    return level;
}
```

The existing `ReadTank()` (with `slot`, `closedDay`, daily accumulation) continues for census/day-close validation.

---

## 5. Pricing Arithmetic (Fixed)

```cpp
const double fp = FuelUnitPrice();  // RUB per tonne
double freight;
int    index = 0;

if (measuredValid && fp > 0.0) {
    const double burnTons = measuredTons;     // tonnes, from state machine
    const double fuel     = burnTons * fp;    // tonnes × RUB/tonne = RUB
    const double wear     = wearCharged ? 0.0
        : gWearLitres * gDieselDensity * fp;  // once per trip
    freight = fuel * (1.0 + gTariffMargin) + wear;
    index   = 1;  // fuel index
} else {
    double fuel = 0.0, wear = 0.0;
    freight = TariffFor(tons, &fuel, &wear, &index);  // total; no extra margin/wear
}
```

**Fixes B3, B4:**
- `burnTons * fp` (not `litres * fp`) — tonnes × RUB/tonne, correct magnitude
- Fallback uses `TariffFor()` total only — margin and wear applied once

---

## 6. Configuration Keys

| Key | Default | Meaning |
|-----|---------|---------|
| `per_trip_fuel` | `1` | Enable per-trip burn measurement |
| `per_trip_fallback` | `1` | Fall back to `TariffFor()` constant when no measured burn |
| `per_trip_fallback_skip` | `0` | If 1, skip payment when no measured burn and no fallback |
| `per_trip_log_per_day` | `60` | Per-delivery [TRIP FUEL] lines |
| `per_trip_ttl_sec` | `600` | Close/skip intervals older than 10 min |

Precedence:
- `per_trip_fuel = 0` → exact v0.17 path
- `per_trip_fallback = 0` → skip payment when no measured burn
- `trip_tariff = 0` → measurement still runs, payment stays `FreightFor()`
- `dry_run = 1` → measure and log, pay nothing
- `litres_auto = 1` → disable per-trip EMA (or feed per-vehicle rate); don't tune same global from two sources

---

## 7. Wear/Margin Allocation

Charge wear **once per trip** using `wearCharged`:
- Set `wearCharged = false` on refill or first sighting (new trip)
- Charge wear on the first valid delivery of each trip
- Multi-drop: subsequent deliveries in same trip skip the wear term
- If the user wants wear prorated instead, add `wear_prorate = 1` and divide by trip delivery count

---

## 8. Logging

```cpp
[TRIP FUEL] v=%p slot=%d home=%s res=%s tons=%.3f leg=%s delta=%.4f t (%.1f L) source=%s trip=%.1f L charged=%.2f
```

Fields:
- `source=measured|fallback:no-baseline|fallback:refill|fallback:no-vehicle|fallback:invalid|fallback:no-index`
- `leg=load|deliver|return|unknown`
- `trip` = accumulated burn for current trip
- Daily summary: `measured N/M deliveries, X L, Y L/trip; fallback by reason; refills R; wear charged W times`

Reset per-day counters where existing counters reset (`g_tariffLoggedToday` block).

---

## 9. Lifetime, Capacity, Save/Load

- Size `g_vehFuelState` and `g_pending` to `VEH_MAX` / `PENDING_RING`
- Evict oldest inactive slot when full
- Add TTL: close/skip intervals older than `per_trip_ttl_sec` or at day close
- Clear all state on save/load load event (hook `AutoPersistenceEvent` or plugin load path)
- Validate `vehicle` with `TsmIsValidGamePtr` + `ReadablePtr` at every event; re-read `home` each time (code warns it can be re-homed)

---

## 10. Validation Plan (Per-Vehicle Against `ReadTank()`)

The review's recommended validation:

1. **One private DO, one vehicle**, `litres_auto = 0`, `per_trip_log_per_day = 500`, `tank_cross_log` high, `cargo_dump = 1`
2. Log per event: `t`, `site`, `mode`, `sign`, `building`, `res`, `tons`, `vehicle`, `home`, `tank`, `source`
3. **At each day close**, print per-vehicle `ReadTank()` `burned`/`refills` and the sum of event deltas for that vehicle
4. **Acceptance (per vehicle, not fleet-level):**
   - per-vehicle event-sum vs `ReadTank()` daily-sum within **2%** over 24 h
   - every `[TRIP FUEL]` line has a `source`; fallback rate reported by reason
   - multi-drop: one wear charge per trip
   - refuel: interval skipped, not charged as 0
   - save/load: no stale pointer; first post-load interval skipped
   - `fp = 0` forced: CPI path still pays, log says `index=cpi`
5. **7-day mixed fleet:** state-owned DO control, private DO, two vehicle types. Compare per-vehicle L/active-truck-day to `[TANK]`'s `lva` — the physical rate, not L/trip.

This validates per-vehicle attribution, not just fleet-level totals.

---

## 11. Out of Scope (v0.18 Revised)

- Per-vehicle-class rates (G10: feasible using `*(veh+0x1708)+0x85E0+4+slot_index*0x10` slot rate, but deferred)
- Vehicle purchase/depreciation (G3 not done; `charge_vehicle_purchase` unimplemented)
- Driver wages (cross-plugin with `citizen_labor`, no link yet)
- Real-time position tracking (no movement hooks available)
- Refuel-site billing (SITE_REFUEL never observed; not a trip boundary)
- "Full cost" model — this is **fuel + wear reserve + margin**, not full cost (G9)

---

## 12. Implementation Priority

| Priority | Issue | Addressed By |
|----------|-------|--------------|
| P0 | B1 vehicle identity at billing point | Option A deferred payment |
| P0 | B3 price unit (1000× error) | `burnTons * fp` |
| P0 | B4 double margin/wear in fallback | `TariffFor()` total only |
| P0 | G5 factory-on-fuel latent double-pay | Gate `s.kind != SHIP_FUEL && known != SITE_REFUEL` |
| P1 | B5 SITE_REFUEL dependency | Continuous baseline + refill detection |
| P1 | B6 return detection | `isStaging` predicate |
| P1 | G1 CarrierNote has no vehicle | `PendingDelivery` ring at mode 1 |
| P1 | G3 refill state contradicts `ReadTank()` | Pure `ReadTankNow()` reader |
| P1 | G4 CPI fallback regression | Fall through to `TariffFor()` when `fp <= 0` |
| P2 | G2 state machine circularity | Continuous baseline, not per-trip resets |
| P2 | G6 lifetime/save-load | Clear state on load, TTL eviction |
| P2 | G7 wear per delivery vs per trip | `wearCharged` once per trip |
| P2 | G8 config precedence | Defined above |
| P2 | G11 probe/log integration | Measurement outside payment branch; `[TARIFF]` log preserved |
| P3 | G9 full cost | Renamed to fuel-cost component |
| P3 | G10 per-class rates | Feasible but deferred |

---

## 13. Files to Modify

| File | Changes |
|------|---------|
| `plugins/private_logistics/private_logistics.cpp` | Add `PendingDelivery`, `VehFuelState`, `ReadTankNow()`, Option A event handling, corrected pricing, wearCharged, `[TRIP FUEL]` logging |
| `plugins/private_logistics/private_logistics.ini` | Add `per_trip_fuel`, `per_trip_fallback`, `per_trip_fallback_skip`, `per_trip_log_per_day`, `per_trip_ttl_sec` |

No changes to:
- `ui_panels.cpp` — no UI changes needed
- `enterprise_base.cpp` — no cross-plugin linking
- Other plugins — isolated to `private_logistics`

---

## 14. Evidence Base

All offsets and behaviors verified against:
- `plugins/private_logistics/private_logistics.cpp` (lines cited throughout)
- `src/resource_pricing_api.h:104` — price is per tonne
- `docs/engine/vehicle-fuel-system.md` — tank at `veh+0x3B8` slot matching `game+0xC3F0`, refuel uses `game+0xC330`, burn gate `veh+0x448 ∈ {1,2}`, `veh+0x440 == 9`, refuel stochastic
- `docs/AGENT_REPORT_private_industry.md` G1–G10 — vehicle lifecycle, transfer sites, `param_9` = fleet owner, `isStaging` home signal
