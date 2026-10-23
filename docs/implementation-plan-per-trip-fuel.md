# Implementation Plan — Per-Trip Fuel Cost Estimation (v0.18, implementation-ready)

**Target:** `SOVIET64.exe` v1.1.1.9, base `0x140000000`, ASLR on  
**Plugin:** `private_logistics` (v0.17 → v0.18)  
**Status:** Corrected — the Option A architecture is kept, the event handling and pricing are rewritten so the pseudocode can be coded as written  
**Architecture:** Option A — defer the carrier payment to the mode-1 pair  
**Measurement:** Continuous per-vehicle tank baseline at mode-1 events  
**Granularity:** Per-delivery, per-vehicle  
**Reviewed against:** `plugins/private_logistics/private_logistics.cpp` (3639 lines), `plugins/private_logistics/private_logistics.ini`, `docs/engine/vehicle-fuel-system.md`, `docs/AGENT_REPORT_private_industry.md`, `src/resource_pricing_api.h`

---

## 0. What changed in this revision

The previous revision adopted Option A, but its §4.2 pseudocode contained thirteen defects that would have shipped bugs, and its §10.0 decision rule was tautological — it could only ever return one answer, at every site, regardless of the truth. This revision fixes both. Each item is marked `C#` in the body so the diff is legible.

| # | Defect in the previous revision | Fix |
|---|---|---|
| **C1** | The `!st->valid` branch consumed the pending delivery (`pd->valid = false; // no interval yet, defer`) and never paid it. Every vehicle's first delivery after plugin start or after a load was silently dropped. | Pay it, with `TariffFor()` as `source=fallback:no-baseline`, then set the baseline. |
| **C2** | `st->lastTons` advanced only inside a matched branch. Any unmatched or invalid event left the baseline stale, so the next delivery's delta spanned two deliveries — double-counting burn that had already been paid as fallback. | The baseline advances on **every** readable mode-1 event, independent of matching or payment. |
| **C3** | `findMatch(..., fabsf(tons), ...)` had no direction filter, so a mode-1 **load** (`+tons`, site `0x6953C4`) could match a pending delivery. | Require `tons < 0` for the delivery leg. Use `tons > 0` as the load leg. Key on `mode` + sign, never on the `known` label (`private_logistics.cpp:594-596`). |
| **C4** | `if (freight > 0.0 && pd->owner) Pay(...)` dropped the payee guards: `IsPrivate`, `owner != building`, and the NULL-owner fallback to the carrier ring (`:1245-1257`, `:1375-1378`). | Explicit `ResolvePayee()` (§4.5), with the vehicle's home building first. |
| **C5** | `wearCharged` was reset only on refill and first sighting. Refuel is stochastic and can be absent for many trips (finding B5), so the reserve was charged once per *refill*, not once per trip; `tripFuel` was never reset at all. | Reset on the mode-1 **load**, which always marks a trip boundary. The return leg is optional and never load-bearing (§4.4). |
| **C6** | No bounds guards: `VehSlot()` returns `-1` at capacity (`:2966-2974`) → `g_vehFuelState[-1]`; `ReadTankNow()` can return `-1.0`; `TankCapacity()` returns `-1.0` (`:3080-3092`), and `delta > cap` is then always true, marking every interval invalid. | Guards on `slot`, `tank`, and `cap > 0.0` (§4.2). |
| **C7** | `g_pendingHead++` was unbounded while indexing used `% PENDING_RING`. | `g_pendingHead = (g_pendingHead + 1) % PENDING_RING`. |
| **C8** | `trip_tariff = 0` and `per_trip_fallback = 0` were declared in §6 but absent from the payment path. | Both branches present in §5 (§5.2). |
| **C9** | The `[TRIP FUEL]` format printed `delta` (tonnes) with `%.1f L`. | Convert through `gDieselDensity` (§8). |
| **C10** | Stale pending deliveries were flushed only "at each close", so an unmatched delivery could wait a whole day and be paid after the daily counters were read. | Sweep on every event with a 2 s TTL (§4.2, §4.6). |
| **C11** | The `[TARIFF]` line and the `g_tariff*` counters stayed at mode 0 while the money moved at mode 1, so the calibration log no longer matched the payment (review G11). | Emit the line at the paid event, carrying measured and constant figures (§8.3). |
| **C13** | `TariffFor()` unconditionally adds the wear reserve, so a delivery that falls back mid-trip pays the reserve twice. | Optional `wearLitresOverride` on `TariffFor()`, defaulted so existing call sites are untouched (§4.6). |
| **C12** | §14 cited `AGENT_REPORT_private_industry.md` for "`param_9` = fleet owner". That document says the opposite, and the previous §4.2 used `vehicleHome == p9` as the return predicate — a predicate that fires on **every** delivery if `p9` is in fact the owner. | `p9` becomes an explicit pre-step (§10.0). The return leg is gated behind `trip_return_via_p9` and is never required for correct pricing (§4.4). |
| **C14** | §10.0's decision rule tested `p9 == vhome` "on ordinary deliveries" — but the delivery site `0x68F7F9` loads `param_9` from `*(veh+0x4F8)`, so that test is **tautologically true** and can only ever return its first branch. | Compare `p9` against `p2` at mode 0 on `0x68F792` instead, with four outcomes; `p9` is documented as site-dependent, not a single meaning (§4.5, §10.0). |

**Numbering caution:** `B#`/`G#` below are the findings of `docs/review-per-trip-fuel-plan.md` §2–§3. They are **not** the `G1`–`G10` of `docs/AGENT_REPORT_private_industry.md`, which is a different investigation with colliding numbering. Where the AGENT_REPORT is cited it is named in full.

---

## 1. Summary of changes from the original plan

| Area | Original (flawed) | This revision |
|------|-------------------|---------|
| Event model | Read at mode 0, no vehicle | Option A: defer the payment to the mode-1 pair |
| Trip boundaries | SITE_LOAD → SITE_DELIVERY → SITE_REFUEL | Continuous baseline; trip starts at the mode-1 load, ends at the return leg |
| Fuel price | `litres * FuelUnitPrice()` (1000× too high) | `burnTons * FuelUnitPrice()` (price is per tonne) |
| Fallback | `TariffFor(...) * (1+margin) + wear` (double-applied) | `TariffFor(...)` only (it already returns a total) |
| SITE_REFUEL | Trip boundary (never observed) | Dropped entirely |
| Return detection | SITE_TRANSSHIP (fires at every delivery) | `isStaging` predicate, `vehicleHome == p9` |
| Vehicle state | New linear-scan table | Reuse `VehSlot()` + continuous baseline |
| Wear allocation | Per delivery (3× on a 3-drop route) | Once per trip, reset at load/return |
| Validation | Fleet-level totals vs priced deliveries | Per-vehicle vs `ReadTank()` daily sum |

---

## 2. Architecture — Option A: deferred carrier payment

The only event that carries the vehicle is mode 1 (`private_logistics.cpp:1146-1147`, `:355`, `:1179-1182`). The billing block currently runs at mode 0 (`:1332`), where `p2` is a building. Option A therefore defers the payment:

```
T0  mode-0 delivery call:  mode == 0, tons > 0, p2 = destination building, p9 = ?
    → stash PendingDelivery { building, owner, resource, tons, unitPrice, t }
    → do NOT compute or move money here

T1  mode-1 call, 30-70 ms later (code comment :690-692): p2 = vehicle, p9 = ?
    → vehicleHome = *(vehicle + 0x4F8)          ← the fleet owner, authoritative
    → if tons < 0: this is the vehicle side of the delivery
        match the pending record, read the tank, compute the delta,
        price it, Pay() the resolved payee
    → if tons > 0: vehicle side of a load → trip start, baseline advance
    → if tons < 0 and vehicleHome == p9: return leg → close the trip

TTL  no mode-1 pair within 2 s → pay from TariffFor() as fallback:no-vehicle
```

No new call-site patches. `Pay()` is always called outside `g_lock` (it takes the lock internally at `:1127-1132`), and the pending ring is only touched inside the detour.

**Known cost:** the payment moves 30-70 ms later. That is invisible at human scale but it changes the ordering of `[PAY]` lines relative to `[B2B]` lines, and it makes `daily_pay_cap` count the delivery on the mode-1 tick. Both are acceptable; note them in the release notes.

---

## 3. Data structures

Reuse `VehSlot(v)` (`:2966`) for the vehicle→slot mapping — it already caps at `VEH_MAX` and is shared with the census. Do not add a second linear scan.

```cpp
#define PENDING_RING 32          // one delivery per vehicle in flight, ×4 headroom

enum TripSkip {
    TRIP_SKIP_NONE = 0, TRIP_SKIP_NO_SLOT, TRIP_SKIP_UNREADABLE,
    TRIP_SKIP_INVALID, TRIP_SKIP_NO_MATCH, TRIP_SKIP_NO_FALLBACK,
    TRIP_SKIP_NO_PRICE, TRIP_SKIP_NO_PAYEE, TRIP_SKIP_COUNT
};

struct PendingDelivery {
    void*  building;     // destination (p2 at mode 0)  — log + payee exclusion
    void*  owner;        // p9 at mode 0 — DISPUTED, see §4.5
    void*  resource;
    double tons;         // positive
    double unitPrice;    // RUB per tonne at stash time
    double t;            // NowSeconds() at stash time
    bool   valid;
};

static PendingDelivery g_pending[PENDING_RING];
static int            g_pendingHead = 0;

struct VehFuelState {
    void*  vehicle;      // re-validated each event
    void*  home;         // *(veh+0x4F8) at the last event — may be re-homed
    double lastTons;     // validated tank level at the last event (tonnes)
    double lastT;        // NowSeconds() at the last event
    double tripFuel;     // burn accumulated for the current trip (tonnes)
    unsigned tripDrops;  // deliveries priced in the current trip
    bool   wearCharged;  // per-trip reserve already charged
    bool   valid;        // baseline established
};

static VehFuelState g_vehFuelState[VEH_MAX];
```

`g_vehFuelState` replaces nothing: `g_vehFuel[]` (`:423`) stays as the day-close baseline owned by `ReadTank()`. The two must never write each other (finding G3).

### 3.1 Helper contracts

Small, non-allocating, and all `static` — the detour runs on the game thread and the zero-allocation rule applies (`.clinerules` §C):

```cpp
static void   CountSkip(TripSkip why);                                   // per-day counter
static void   CountRefill();                                             // per-day counter
static void*  ResolvePayee(const PendingDelivery&, void* home, void* ring);
static void   PriceAndPay(PendingDelivery&, VehFuelState&, double burnTons,
                          void* home, double now);
static void   PayFallback(PendingDelivery&, VehFuelState* st,
                          const char* source, double now);
static void   LogTripFuel(const PendingDelivery&, const VehFuelState* st,
                          double burnTons, int index, const char* source,
                          double charged, double now);
static void   LogTripClose(void* vehicle, int slot, const VehFuelState&,
                           double lastDrop);
static void   NotePaidTariff(double freight, double fuelCost,
                             double wearCost, int index);                 // C11
static const char* SkipName(TripSkip why);                               // log text
```

`LogTripFuel` takes `const VehFuelState*` because a flushed pending delivery has no vehicle state. It must guard the null. All logging goes through `Logf`, which already handles the per-day caps.

---

## 4. Event handling

### 4.0 Where the code goes

In `DetourTransfer`, after the forward call and the `committed` classification (`:1194`), and **outside** `EnterCriticalSection(&g_lock)`:

- mode 0, `tons > 0`, `committed` → `StashDelivery()` (§4.1)
- mode 1, `committed`, `p2 && vehicleHome` → `OnVehicleEvent()` (§4.2)
- mode 0, `tons < 0` → the factory payment stays where it is, with the G5 gate (§4.6)

The existing billing block (`:1332`) keeps computing the probe figures only when `g_perTripFuel == 0`; when per-trip measurement is on, the pricing moves to the paid event.

### 4.1 Mode 0 — stash the delivery

```cpp
static void StashDelivery(void* building, void* owner, void* res,
                          float tons, double now)
{
    if (!g_billing || !g_payCarrier) return;      // nothing to pay later
    if (!(tons > 0.0f) || !building) return;      // deliveries only

    PendingDelivery& pd = g_pending[g_pendingHead];
    pd.building  = building;
    pd.owner     = owner;
    pd.resource  = res;
    pd.tons      = (double)tons;
    pd.unitPrice = UnitPrice(res);
    pd.t         = now;
    pd.valid     = true;
    g_pendingHead = (g_pendingHead + 1) % PENDING_RING;   // C7
}
```

Stash unconditionally on sign; decide *who gets paid* at mode 1, where the vehicle exists.

### 4.2 Mode 1 — measure first, then pay

```cpp
static void OnVehicleEvent(void* vehicle, void* home, void* p9,
                           void* res, float tons, double now)
{
    FlushStale(now);                                   // C10, TTL 2 s

    const int slot = VehSlot(vehicle);                 // C6
    if (slot < 0) { CountSkip(TRIP_SKIP_NO_SLOT); return; }

    VehFuelState& st = g_vehFuelState[slot];
    const double tank = ReadTankNow(vehicle);          // -1 = unreadable

    if (tank < 0.0) {                                  // C6
        CountSkip(TRIP_SKIP_UNREADABLE);
        st.valid = false;                              // force a fresh baseline
        return;
    }

    if (!st.valid) {                                   // first sighting
        st.valid   = true;
        st.lastTons = tank;  st.lastT   = now;
        st.home     = home;  st.vehicle = vehicle;
        st.tripFuel = 0.0;   st.wearCharged = false;  st.tripDrops = 0;
        return;                                        // no interval yet
    }

    const double cap  = TankCapacity(vehicle);
    const double rise = tank - st.lastTons;
    const double drop = st.lastTons - tank;

    if (rise > 0.02) {                                 // refill — skip the interval
        CountRefill();
        st.lastTons = tank; st.lastT = now; st.home = home;
        // A refill does NOT close a trip (B5): refuelling is stochastic and
        // can occur while the vehicle is working, so it is not a boundary.
        return;
    }

    if (drop < 0.0 || (cap > 0.0 && drop > cap)) {     // C6: cap <= 0 must not kill it
        CountSkip(TRIP_SKIP_INVALID);
        st.lastTons = tank; st.lastT = now;
        return;
    }

    // ---- a readable, monotone (or flat) interval ------------------------
    const bool isDelivery = (tons < 0.0f);                       // C3, site 0x68F7F9
    const bool isLoad     = (tons > 0.0f);                       // C3, site 0x6953C4

    // The return leg is OPTIONAL and ships DISABLED (default 0).
    // At 0x68F7F9 the engine loads param_9 from *(veh+0x4F8), so home == p9 is
    // true on EVERY delivery by construction - not because p9 is the owner.
    // Leaving this on would swallow every payment. See §4.4 and §10.0.
    const bool isReturn = g_tripReturnViaP9 && isDelivery && home && home == p9;

    if (isLoad) {                 // the load ALWAYS starts a trip (C5)
        st.tripFuel = 0.0; st.wearCharged = false; st.tripDrops = 0;
    }
    st.tripFuel += drop;          // deadhead, loaded leg, or return leg

    if (isDelivery && !isReturn) {
        PendingDelivery* pd = FindPending(res, fabsf(tons), now);
        if (pd) { PriceAndPay(*pd, st, drop, home, now); pd->valid = false; }
        else    { CountSkip(TRIP_SKIP_NO_MATCH); }   // C1: counted, not silent
    }

    if (isReturn && g_tripReturnViaP9) {   // display only; never load-bearing
        LogTripClose(vehicle, slot, st, drop);
        st.tripFuel = 0.0; st.wearCharged = false; st.tripDrops = 0;
    }

    st.lastTons = tank; st.lastT = now; st.home = home;   // C2: always
}
```

**Why `isLoad` resets before accumulating:** the load happens at the source *after* the deadhead, so the drop measured at the load event is the outbound leg of the **new** trip. Reset, then add.

**Why the trip boundary is the load and not the return:** `load -> next load` is an event pair the engine emits unconditionally, and it is the only boundary that does not depend on what `param_9` means. The return leg is a nicer display and an earlier `wearCharged` reset, but if §10.0 shows `p9` is the owner, `trip_return_via_p9` stays `0` and the plan still prices every delivery correctly. A design whose money depends on an unverified offset is the defect this whole review set out to remove.

**Why the baseline advances unconditionally:** C2. Any event that reads a valid tank becomes the next reference. Skipping it would make the following delta span two legs and re-charge burn already priced as fallback.

### 4.3 `ReadTankNow()` — pure reader

```cpp
// Same validation as ReadTank() (:3237) minus all daily state.
// Returns the level in tonnes, or -1.0 when unreadable.
static double ReadTankNow(void* v)
{
    int found = 0;
    double level = FuelSlotTons(v, &found);   // -1 unreadable, 0 no slot
    if (!found) {
        const double mirror = MirrorTons(v);
        if (level < 0.0 && mirror < 0.0) return -1.0;
        level = (mirror > 0.0) ? mirror : 0.0;
    }
    if (!(level >= 0.0)) return -1.0;         // NaN or negative
    return level;
}
```

`ReadTank()` keeps its current job: the daily baseline for `[TANK]`, `g_burnTonsRun`, refill counting and the `[TANK]` cross-check line. `ReadTankNow()` never touches `g_vehFuel[]`.

### 4.4 Trip boundaries

| Boundary | Signal | Action | Load-bearing? |
|---|---|---|---|
| Trip start | mode 1, `tons > 0` (vehicle side of a load) | `tripFuel = 0`, `wearCharged = false`, `tripDrops = 0` | **Yes** — this is the boundary wear relies on |
| Trip end (optional) | mode 1, `tons < 0` **and** `home == p9`, only when `trip_return_via_p9 = 1` | log the trip total, then reset | No — display and an earlier wear reset only |
| Refill | `tank - lastTons > 0.02` | skip the interval, advance the baseline, **do not** close the trip | Yes |
| TTL | no event for `per_trip_ttl_sec` | log `source=expired`, reset the accumulators | Yes |

`load -> next load` is the primary boundary because both events are emitted for every trip regardless of `param_9`. The return leg is worth having — it lets `tripFuel` be reported per trip rather than per load cycle — but it must not be the thing the money depends on.

> **Why `trip_return_via_p9` ships at `0`.** At `0x68F7F9`, `p9 == vhome` is guaranteed by the engine: `param_9` is loaded from `*(veh+0x4F8)` (`docs/AGENT_REPORT_private_industry.md:114`, store at `:121`). A return predicate built on that comparison would classify **every** delivery as a return leg. v0.17 escapes this only because `isStaging` sets a ledger label and never gates money — see §4.5.

Neither boundary uses the `known` site label. The site table's labels are for the log only (`:594-596`), and three of them (`0x68EECA`, `0x1CB7FF`, `0x1B2898`) are `+tons` at mode 0 even though they are labelled load/delivery.

### 4.5 Payee resolution — and the `p9` question

**This is the one open question in the plan and it must be settled before §4.2 ships.** The sources in this repo disagree with each other — and the AGENT_REPORT's summary line disagrees with its own per-site detail:

| Source | Claim |
|---|---|
| `private_logistics.cpp:1148-1149` | `p9` = **fleet owner** on the delivery family, NULL on the load family, "NOT an endpoint" (proven in two sessions) |
| `private_logistics.cpp:629` | `0x68F792` — "param_2 = building, **param_9 = fleet owner**" |
| `docs/AGENT_REPORT_private_industry.md:33` | summary line — `Transfer param_9 (stack+0x40)` = **dest Building***, NULL = order-scan mode, "likely (banked)" |
| `docs/AGENT_REPORT_private_industry.md:110-115` | **the per-site table**, from literal disassembly — NULL at `0x1B2898`/`0x1B484B`, `*(veh+0x4F8)` at `0x68F7F9`, `RSI` at `0x6953C4` (store provenance `:121`, callee slot `:139`) |
| `docs/AGENT_REPORT_private_industry.md:130` | "For `bld=NULL` sites, read the destination from the resolved order (`+0x50` records), **not from `param_9`**" |

The previous revision's §14 cited the AGENT_REPORT for "param_9 = fleet owner", which that document does **not** say. But the report's summary line is also incomplete, because **`p9` is site-dependent, not a single thing**:

> At `0x68F7F9` `p9` equals the vehicle's home depot **by construction** (`param_9` is loaded from `*(veh+0x4F8)`); at `0x68F792` it may be the destination; at `0x1B2898` and `0x1B484B` it is NULL, and the destination is resolved from the order. Neither the plugin comment (`:1148-1149`) nor the AGENT_REPORT summary (`:33`) describes all four sites.

Resolve the `0x68F792` case with the read-only procedure in §10.0, then:

```cpp
// The vehicle's own home building is the fleet owner the engine refuels from
// (FUN_14016E3B0 reads *(veh+0x4F8) at 0x14016F827). It is authoritative and
// available at mode 1 regardless of what param_9 turns out to be.
static void* ResolvePayee(const PendingDelivery& pd, void* home, void* ringCarrier)
{
    if (home && home != pd.building && IsPrivate(home))            return home;
    if (pd.owner && pd.owner != pd.building && IsPrivate(pd.owner)) return pd.owner;
    if (ringCarrier && ringCarrier != pd.building && IsPrivate(ringCarrier))
        return ringCarrier;
    return nullptr;   // counted as TRIP_SKIP_NO_PAYEE, never guessed
}
```

Why `home` first: at mode 1 it is read directly from the vehicle, it is the building the engine itself uses as the owner for refuelling, and it cannot be confused with the destination. `pd.owner` stays as a secondary candidate so that, if §10.0 proves `p9` is the fleet owner, the previous behaviour is preserved.

`ringCarrier` is passed as `nullptr` at mode 1: the carrier ring exists precisely because mode-0 calls do not name the vehicle, and at mode 1 `vehicleHome` answers the question directly. Keep the parameter so the existing ring can still serve the v0.17 path when `per_trip_fuel = 0`.

> **Warning — `isStaging` is not free.** `private_logistics.cpp:1197` defines `isStaging = (mode != 0 && vehicleHome && vehicleHome == p9)`. If `p9` is the fleet owner, that expression is true on **every** delivery, and any pricing that treats `isStaging` as "the vehicle is home" would classify all deliveries as returns. The existing v0.17 code survives this because `isStaging` only sets `s.kind` for the ledger (`:1210-1211`) and never gates the payment. The new code must preserve that property: use `isStaging` for labels and for the optional return leg, never as a precondition for paying.

### 4.6 Match, flush, and the factory gate

```cpp
// Matching is on resource + tonnage + age only. The vehicle is not in the
// pending record and mode 0 cannot supply it, so `home` cannot be a match key
// until §10.0 says whether p9 is the owner.
static PendingDelivery* FindPending(void* res, float tons, double now)
{
    for (int i = 1; i <= PENDING_RING; i++) {      // newest first
        const int k = (g_pendingHead - i + PENDING_RING) % PENDING_RING;
        PendingDelivery& pd = g_pending[k];
        if (!pd.valid || pd.resource != res) continue;
        if (now - pd.t > 2.0) continue;
        const float tol = (tons > 0.01f) ? tons * 0.005f : 0.0005f;
        if (fabsf((float)pd.tons - tons) > tol) continue;
        return &pd;
    }
    return nullptr;
}

static void FlushStale(double now)
{
    for (int i = 0; i < PENDING_RING; i++) {
        PendingDelivery& pd = g_pending[i];
        if (!pd.valid || now - pd.t <= 2.0) continue;
        PayFallback(pd, nullptr, "fallback:no-vehicle", now);   // C10: 2 s
        pd.valid = false;
    }
}

// No vehicle was ever matched, so there is no state to consult.
// `st` may be null; see the wear note below.
static void PayFallback(PendingDelivery& pd, VehFuelState* st,
                        const char* source, double now)
{
    if (!g_perTripFallback) { CountSkip(TRIP_SKIP_NO_FALLBACK); return; }

    double f = 0.0, w = 0.0;
    int    index = 0;
    const double wearLitres = (st && st->wearCharged) ? 0.0 : -1.0;  // see C13
    const double freight = TariffFor(pd.tons, &f, &w, &index, wearLitres);

    void* payee = ResolvePayee(pd, st ? st->home : nullptr, nullptr);
    if (freight <= 0.0) { CountSkip(TRIP_SKIP_NO_PRICE); return; }
    if (!payee)         { CountSkip(TRIP_SKIP_NO_PAYEE); return; }

    const bool paid = Pay(payee, g_tripTariff ? freight
                                              : FreightFor(pd.tons, pd.unitPrice),
                          TSM_CAP_WHOLESALE, "b2b_freight", "freight",
                          pd.resource, (float)pd.tons);
    if (paid) {
        TrackCarrier(payee);
        NoteCarrierIncome(payee, freight);
        if (st && wearLitres < 0.0) st->wearCharged = true;
    }
    LogTripFuel(pd, st, 0.0, index,   source, paid ? freight : 0.0, now);
}
```

**C13 — `TariffFor()` always adds wear, so a mid-trip fallback would charge the reserve twice.** Change its signature minimally:

```cpp
static double TariffFor(double tons, double* outFuel, double* outWear,
                        int* outIndex, double wearLitresOverride = -1.0)
{
    ...
    const double wearLitres = (wearLitresOverride >= 0.0)
                            ? wearLitresOverride : gWearLitres;
    ...
    wear = wearLitres * gDieselDensity * fp;          // both index paths
    ...
    // CPI path: ratio = (gLitresPerTrip > 0.0) ? (wearLitres / gLitresPerTrip) : 0.0
}
```

A default argument keeps every existing call site (`:1350`, and any other) compiling unchanged, and it means the measured path and the fallback path share one total computation — which is the property that made B4 possible in the first place.

**Residual asymmetry, stated rather than hidden:** an unmatched delivery has no trip state, so `PayFallback(pd, nullptr, ...)` charges a full reserve even if the trip that produced it already paid one. That over-charges by at most one `wear_litres` (0.665 RUB at shipped values) on an already-degraded measurement, and it is counted in `fallback:no-vehicle`. Do not paper over it by suppressing the reservoir — that would under-charge every genuinely unmatched trip instead.

Ambiguity: two vehicles from the same fleet delivering the same resource in the same tonnage within 2 s match FIFO. Count those (`TRIP_SKIP_AMBIGUOUS`) rather than pretend the attribution is certain.

**The factory gate (G5).** The mode-0 `tons < 0` block pays the building as a factory sale without checking `s.kind` (`:1414-1421`). A fuel draw from a private building would be paid as a sale:

```cpp
if (g_billing && tons < 0.0f && g_payFactory &&
    s.kind != SHIP_FUEL && known != SITE_REFUEL)
```

Add a counter for `SHIP_FUEL` reaching that branch; the gate should never fire, and if it does the label is wrong.

---

## 5. Pricing

### 5.1 The arithmetic

```cpp
// FuelUnitPrice() is RUB per TONNE (src/resource_pricing_api.h:104).
// TariffFor() (:959-993) converts through gDieselDensity itself:
//   fuel = work * gDieselDensity * fp        (litres -> tonnes -> RUB)
static double PriceMeasured(double burnTons, double fp,
                            bool wearAlreadyCharged, bool* outWearCharged)
{
    const double fuel = burnTons * fp;                       // B3: tonnes × RUB/t
    const double wear = wearAlreadyCharged ? 0.0
                      : gWearLitres * gDieselDensity * fp;    // B4: once per trip
    *outWearCharged = true;
    return fuel * (1.0 + gTariffMargin) + wear;
}
```

**B3** — `burnTons * fp`, not `litres * fp`. At the ini's own 133/RUB and 0.01475 t, that is 1.96 RUB of fuel, not 1961.75.  
**B4** — the fallback calls `TariffFor()` and uses the returned total. `TariffFor` already applies `(1 + margin)` and adds wear (`:991`); applying either again overcharges by ~55% at shipped values.

### 5.2 The pay path, with every config branch present

```cpp
static void PriceAndPay(PendingDelivery& pd, VehFuelState& st,
                        double burnTons, void* home, double now)
{
    const double fp = FuelUnitPrice();
    double freight = 0.0, fuelCost = 0.0, wearCost = 0.0;
    int    index   = 0;
    const char* source = nullptr;

    bool wearIncluded = false;

    if (g_perTripFuel && burnTons > 0.0 && fp > 0.0) {
        fuelCost = burnTons * fp;
        wearCost = st.wearCharged ? 0.0 : gWearLitres * gDieselDensity * fp;
        freight  = fuelCost * (1.0 + gTariffMargin) + wearCost;
        wearIncluded = !st.wearCharged;
        st.tripDrops++;
        index  = 1;
        source = "measured";
    }
    else if (g_perTripFallback) {                       // C8
        double f = 0.0, w = 0.0;
        const double wl = st.wearCharged ? 0.0 : -1.0;   // C13: never twice
        freight  = TariffFor(pd.tons, &f, &w, &index, wl);  // total, no extra terms
        fuelCost = f; wearCost = w;
        wearIncluded = (wl < 0.0);
        st.tripDrops++;                                 // keeps the log honest
        source   = (burnTons > 0.0) ? "fallback:no-index" : "fallback:no-burn";
    }
    else {                                              // C8: fallback disabled
        CountSkip(TRIP_SKIP_NO_FALLBACK);
        LogTripFuel(pd, &st, burnTons, index, "skipped:no-fallback", 0.0, now);
        return;
    }

    if (freight <= 0.0) { CountSkip(TRIP_SKIP_NO_PRICE);
                          LogTripFuel(pd, &st, burnTons, index, "no-price", 0.0, now);
                          return; }

    void* payee = ResolvePayee(pd, home, /*ring*/ nullptr);
    if (!payee) { CountSkip(TRIP_SKIP_NO_PAYEE);
                  LogTripFuel(pd, &st, burnTons, index, "no-payee", 0.0, now);
                  return; }

    // trip_tariff = 0 -> measure and log, but keep the v0.6 value share. (C8)
    const double charged = g_tripTariff
        ? freight
        : FreightFor(pd.tons, pd.unitPrice);

    const bool paid = Pay(payee, charged, TSM_CAP_WHOLESALE, "b2b_freight",
                          "freight", pd.resource, (float)pd.tons);

    // Retire the reserve only when the money actually moved. Pay() refuses on
    // min_payment, daily_pay_cap, a short treasury or missing services, and in
    // every one of those cases the carrier did not collect the wear term -
    // marking it charged would quietly lose it for the rest of the trip.
    if (paid && wearIncluded) st.wearCharged = true;
    if (paid) { TrackCarrier(payee); NoteCarrierIncome(payee, charged); }

    LogTripFuel(pd, &st, burnTons, index, source, paid ? charged : 0.0, now);
    NotePaidTariff(freight, fuelCost, wearCost, index);   // C11: counters move here
}
```

`Pay()` already handles `dry_run` (it logs `would ...` and returns true at `:1092-1133`), `min_payment` (`:1053`), `daily_pay_cap` (`:1066`) and the treasury affordability gate (`:1080-1089`). Do not duplicate any of them.

### 5.3 The CPI fallback

When `FuelUnitPrice()` is 0 but the measurement is valid, this path falls through to `TariffFor()`, which prices the **constant** `litres_per_trip` in survival baskets (`:975-985`, `index = 2`). That is deliberate: there is no litres→basket rate, so a measured burn cannot be expressed in baskets without a new policy parameter. The log says `fallback:no-index` so the reader knows the charge is not the measurement.

If measured burn in the CPI path is ever wanted, add `baskets_per_litre` as an explicit key. Do not invent the conversion silently (review G4).

---

## 6. Configuration keys

| Key | Default | Meaning |
|-----|---------|---------|
| `per_trip_fuel` | `1` | Enable per-trip burn measurement |
| `per_trip_fallback` | `1` | Fall back to `TariffFor()` when no measured burn |
| `per_trip_fallback_skip` | `0` | With `per_trip_fallback = 0`: 0 = skip silently, 1 = skip and log each one |
| `per_trip_log_per_day` | `60` | Per-delivery `[TRIP FUEL]` lines |
| `per_trip_ttl_sec` | `600` | Close/skip trip state idle for this long |
| `trip_return_via_p9` | `0` | Treat `vehicleHome == p9` at a delivery as the return leg. **Disabled:** at `0x68F7F9` the engine loads `param_9` from `*(veh+0x4F8)`, so the predicate is true on every delivery (§4.4) |

| State | Behaviour |
|---|---|
| `per_trip_fuel = 0` | Exact v0.17 path: billing block at mode 0, `TariffFor()` only |
| `per_trip_fallback = 0` | No measured burn → no payment, counted as `TRIP_SKIP_NO_FALLBACK` |
| `trip_tariff = 0` | Measurement runs and logs; payment is `FreightFor()` (value share) |
| `billing = 0`, `tariff_probe = 1` | Measurement runs and logs; nothing is paid |
| `dry_run = 1` | `Pay()` logs `would ...`; measurement and counters unchanged |
| `litres_auto = 1` | Per-trip EMA disabled (review G8); only the existing tank EMA runs |

`g_perTripLogToday` must be reset beside `g_tariffLoggedToday` at `:1648-1651`.

---

## 7. Wear and margin allocation

- `wearCharged` is cleared at the **mode-1 load** and at the **return leg** (C5) — not at a refill, which is not a trip boundary.
- The reserve is charged on the first measured delivery of a trip; later drops in the same trip pay fuel only.
- `tripDrops` is recorded so the log can show `charged once over N drop(s)`.
- On a multi-drop route at shipped values the reserve is 0.665 RUB per **trip** instead of per drop; the previous behaviour charged 1.995 RUB on a 3-drop trip, about 68% of the whole fuel cost of a short trip.
- If prorating is wanted later, add `wear_prorate = 1` and divide by `tripDrops` — but then the first drop cannot know N. Prorating requires deferring the reserve to the return leg, which is a different cash-flow decision.

---

## 8. Logging

### 8.1 Per-delivery line

```
[TRIP FUEL] %s | v=%p slot=%d home="%s" dest="%s" res=%s tons=%.3f leg=%s
             delta=%.4f t (%.2f L) trip=%.2f L source=%s
             fuel=%.2f wear=%.2f charged=%.2f index=%s
```

- `delta` is in tonnes; the litres figure is `delta / gDieselDensity` (C9).
- `leg` = `load | deliver | return`, derived from `mode` + sign + `isStaging`, never from `known`.
- `source` = `measured | fallback:no-burn | fallback:no-index | fallback:no-vehicle | fallback:no-baseline | fallback:invalid | skipped:no-fallback | no-price | no-payee | expired`
- `index` = `fuel | cpi | none`, so the CPI branch is visible at the same place as the money.

### 8.2 Daily summary

```
[TRIP FUEL] run: measured %u/%u deliveries = %.1f L (%.2f L/delivery)
             | fallback %u (no-burn %u, no-index %u, no-vehicle %u, no-baseline %u, invalid %u)
             | skipped %u | refills %u | wear charged %u time(s) | trip closes %u
```

### 8.3 Keeping `[TARIFF]` honest (C11)

`probeTariff`, `probeCost` and `probeShare` are currently computed at mode 0 (`:1350`) and printed at `:1402-1410`. When measurement is on, emit the `[TARIFF]` line at the **paid** event and print both figures:

```
[TARIFF] ... measured %.2f (%.4f t x %.2f/t) vs constant %.2f | charged %.2f
```

Move `g_tariffCount`, `g_tariffSum`, `g_tariffCostSum` and `g_tariffShareSum` to the same place, so `[FUEL] run ... L/trip` and `[TANK] run ... L/trip` (`:1815`, `:1833`) keep counting the same population — deliveries actually charged.

---

## 9. Lifetime, capacity, save/load

- `g_vehFuelState` is sized `VEH_MAX` and indexed through `VehSlot()`; a `-1` slot is skipped and counted (C6).
- `VehSlot()` never evicts. When it returns `-1` the fleet is larger than 64; log once per day and keep the measurement off for the new vehicle rather than reusing a slot.
- Clear `g_vehFuelState` and `g_pending` on the persistence load event (`AutoPersistenceEvent`, `:588`) and on save. A stale `void*` can alias a recycled vehicle; `TsmIsValidGamePtr` + `ReadablePtr` catch a dead pointer but not a reused one.
- Re-read `home` at every event; `:1155-1157` warns the vehicle can be re-homed later in the tick.
- Idle-expire trip state after `per_trip_ttl_sec`; log `source=expired` rather than silently resetting.

---

## 10. Validation

### 10.0 Pre-step: settle `p9` (read-only — no instrumentation required)

The existing `[B2B SITE]` line already prints everything this experiment needs
(`private_logistics.cpp:1318-1323`):

```
[B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u p2=%p "%s" priv=%d p9=%p vhome=%p class=%s
```

Compare `p9` against `p2` at **mode 0** on `0x68F792`, where `p2` is the destination building:

| Observation | Conclusion | Consequence |
|---|---|---|
| `p9 == p2` | `p9` is the destination | `pd.owner` is unusable; `ResolvePayee()` must lead with `home` |
| `p9 == vhome` | `p9` is the fleet owner | keep `pd.owner` as the primary candidate |
| `p9 == NULL` | the plugin comment is wrong | `pd.owner` is never set; `home`-first is mandatory |
| anything else | ambiguous | keep `home`-first; leave `trip_return_via_p9 = 0` |

Adding `BuildingName(p9)` to the same line is the only code change worth making, and it is optional — a building name makes the verdict readable without matching raw pointers by hand.

> **Do not** test `p9 == vhome` at `0x68F7F9`. There `param_9` is loaded from `*(veh+0x4F8)` (`docs/AGENT_REPORT_private_industry.md:114`, store at `:121`), so the comparison is **tautologically true** and cannot distinguish the hypotheses. That is why the earlier form of this section was wrong: it would have reported "`p9` is plausibly the fleet owner" on every run, at every site, regardless of the truth.

### 10.1 Procedure

1. One private DO, one vehicle, `litres_auto = 0`, `per_trip_log_per_day = 500`, `tank_cross_log` high, `cargo_dump = 1`.
2. Log per event: `t`, `site`, `mode`, sign, `building`, `res`, `tons`, `vehicle`, `home`, `tank`, `source`.
3. At each day close, print per vehicle: `ReadTank()`'s `burned`/`refills` **and** the sum of that vehicle's event deltas.

### 10.2 Acceptance

- [ ] Per **vehicle**, event-sum vs `ReadTank()` daily-sum within **2%** over 24 h. If a refill falls between the two boundaries, report the delta for the non-refill window instead of loosening the tolerance: the two sums drop different intervals by construction.
- [ ] Every `[TRIP FUEL]` line carries a `source`; the fallback rate is reported **by reason**.
- [ ] Multi-drop: exactly one wear charge per trip.
- [ ] Refill: the interval is skipped and counted, never charged as zero burn (this is what the previous revision's `burnTons < 0 → 0` would have done).
- [ ] Save/load: no stale pointer; the first post-load interval is skipped and logged.
- [ ] `fp = 0` forced (rename the resource in `resource_pricing.ini`): the CPI path still pays, and the line says `index=cpi`.
- [ ] `per_trip_fuel = 0` reproduces v0.17 payments exactly.
- [ ] `daily_pay_cap` is never reached in a normal day — if it is, the price unit is wrong again.

### 10.3 Mixed-fleet run

Seven days: a state-owned DO control, a private DO, two vehicle types. Compare each vehicle's L per active truck-day to that **vehicle's own** active days. `[TANK]`'s `lva` (`:1842`) is a fleet-wide average across every enterprise the census walks (`:3327`), so use it as a sanity band, not as the reference.

---

## 11. Out of scope (v0.18)

- Per-vehicle-class rates — feasible via `*(veh+0x1708)+0x85E0+4+slot_index*0x10` (`docs/engine/vehicle-fuel-system.md` §3.2) as a validation signal, but deferred.
- Vehicle purchase / depreciation — `AGENT_REPORT_private_industry.md` G3 was left open; `charge_vehicle_purchase` is still unimplemented (`private_logistics.ini:210-217`).
- Driver wages — needs `citizen_labor`, no link exists.
- Real-time position tracking — no movement hooks.
- Refuel-site billing — `0x16F831` was never observed; not a trip boundary.
- Full cost — this is **fuel + wear reserve + margin**, not total cost per trip (review G9).

---

## 12. Implementation priority

| Priority | Item | Where |
|---|---|---|
| P0 | Settle `p9` with the one-line log (§10.0) | §10.0 |
| P0 | B1 vehicle identity at the billing point | §4.1, §4.2 |
| P0 | B3 price unit | §5.1 |
| P0 | B4 double margin/wear in the fallback | §5.2 |
| P0 | G5 factory-on-fuel double-pay | §4.6 |
| P1 | C1 first delivery dropped | §4.2 |
| P1 | C2 baseline must always advance | §4.2 |
| P1 | C3 direction filter at mode 1 | §4.2 |
| P1 | C4 payee guards | §4.5 |
| P1 | B5 drop `SITE_REFUEL` | §4.2, §4.4 |
| P1 | B6 return detection — **optional**, gated, never load-bearing | §4.2, §4.4, §4.5 |
| P1 | G3 pure `ReadTankNow()` | §4.3 |
| P1 | G4 CPI fallback | §5.3 |
| P2 | C5 wear reset at load/return | §4.4, §7 |
| P2 | C6 bounds guards | §4.2 |
| P2 | C8 config precedence in the pay path | §5.2 |
| P2 | C10 TTL sweep | §4.6 |
| P2 | C11 `[TARIFF]` and counters at the paid event | §8.3 |
| P2 | C13 wear double-charge on a mid-trip fallback | §4.6 |
| P2 | G6 lifetime/save-load | §9 |
| P3 | C7, C9, C12 polish | §3, §8, §14 |

---

## 13. Files to modify

| File | Change |
|---|---|
| `plugins/private_logistics/private_logistics.cpp` | `PendingDelivery`, `VehFuelState`, `ReadTankNow()`, `StashDelivery()`, `OnVehicleEvent()`, `FindPending()`, `FlushStale()`, `ResolvePayee()`, `PriceAndPay()`, `[TRIP FUEL]` logging, `[TARIFF]` moved to the paid event, factory gate | 
| `plugins/private_logistics/private_logistics.ini` | `per_trip_fuel`, `per_trip_fallback`, `per_trip_fallback_skip`, `per_trip_log_per_day`, `per_trip_ttl_sec` |
| `plugins/private_logistics/private_logistics.cpp` (`PL_VERSION`) | `0.17` → `0.18` (`:550`) |

No changes to `ui_panels.cpp`, `enterprise_base.cpp`, or any other plugin.

**Build gate:** per `INSTRUCTIONS.md:122-130`, a change is not done until `cmd /c build.bat private_logistics` succeeds and `build\plugins\private_logistics.dll` has a fresh timestamp. The event-ordering changes in §4 cannot be smoke-tested without a Windows build and a running save.

---

## 14. Evidence base

- `plugins/private_logistics/private_logistics.cpp` — `:355` (`veh+0x4F8`, mode 1 only), `:959-993` (`TariffFor` returns a total), `:971-973` (density conversion), `:1053-1089` (`Pay` guards), `:1146-1152` (`p2`/`p9` roles), `:1179-1182`, `:1197` (`isStaging`), `:1245-1257` (carrier ring), `:1332` (billing gate), `:1375-1378` (payee rules), `:1414-1421` (factory block), `:1648-1651` (counter reset), `:1815`, `:1833-1849` (`[TANK]` denominators), `:2966-2974` (`VehSlot`), `:3039-3092` (`FuelSlotTons`, `MirrorTons`, `TankCapacity`), `:3237-3269` (`ReadTank`)
- `src/resource_pricing_api.h:104` — "The live price of **one ton** of the given resource"
- `docs/engine/vehicle-fuel-system.md` — tank is the `veh+0x3B8` slot matching `game+0xC3F0`; refuel draws from `veh+0x4F8` with `game+0xC330`; burn gate `veh+0x448 ∈ {1,2}` and `veh+0x440 == 9` ("no burn, no refuel"); refuel is stochastic (§3.4, §4.1)
- `docs/AGENT_REPORT_private_industry.md` — `:33` summary line `param_9` = destination, "likely (banked)" (conflicts with the plugin comment; see §4.5); **`:110-115` the per-site `param_9` table** (NULL at `0x1B2898`/`0x1B484B`, `*(veh+0x4F8)` at `0x68F7F9`, `RSI` at `0x6953C4`); `:121` the `MOV [RSP+0x40],R15` store that makes `param_9` readable at the call; `:139` its callee slot `[RSP+0xD8]`; `:130` destination from order records for `bld = NULL`; `:163-166` `bld+0xC70/+0xC78` fleet vector; G6 "billing must key on `mode==1`, never on `flag`"
