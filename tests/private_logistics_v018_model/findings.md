# Findings — fuel-accounting / settlement design (v0.18)

Scope: `plugins/private_logistics/private_logistics.cpp` (v0.17, 3641 lines),
`docs/implementation-plan-per-trip-fuel.md` (the v0.18 plan),
`docs/review-per-trip-fuel-plan.md` (its review), `private_logistics.ini`,
`src/resource_pricing_api.h`.

The claimed plan `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md`
**does not exist** in this worktree. The v0.18 plan actually lives at
`docs/implementation-plan-per-trip-fuel.md`; this findings doc treats the
review (`docs/review-per-trip-fuel-plan.md`) as the corrective authority where
it disagrees with the plan.

These findings describe **design defects and required production integration
changes**. They do not modify production source and prove nothing about the real
game; the reference model in `model.py` validates only its own internal
behaviour.

---

## 1. Discrepancies: production behaviour (v0.17) vs proposed v0.18 semantics

| Area | Production (v0.17) | Proposed v0.18 semantics | Status |
|------|-------------------|--------------------------|--------|
| Billing site | Mode 0 only (`:1332`); `p2` is a **building**, no vehicle | Option A: defer carrier payment to the mode-1 pair, which carries the vehicle | Not implemented |
| Carrier resolution | `param_9` (p9) at mode 0, else carrier-ring match (`:1245-1257`, `:1375-1378`) | `vehicleHome = *(veh+0x4F8)` first (authoritative fleet owner), p9 as secondary candidate | Not implemented |
| Trip boundary | None defined; `SITE_REFUEL` (`0x16F831`) listed as "never observed" | Trip starts at mode-1 **load**; return leg optional; `SITE_REFUEL` dropped as a boundary | Not implemented |
| Measurement | `ReadTank()` day-close baseline only; `BurnProbe()` fleet-level per-delivery sample | Continuous per-vehicle baseline at every readable mode-1 event; separate `ReadTankNow()` pure reader | Not implemented |
| Pricing formula | `TariffFor()`: `work*density*fp*(1+margin) + wear*density*fp` (total) | Same structure, but `burnTons*fp` for measured quantity; wear once per **trip** not per delivery | Partially (TariffFor ok; wear allocation wrong) |
| Pricing units | `TariffFor()` already RUB/tonne via density | Confirmed RUB/tonne; plan draft's `litres*price` was 1000x too high (B3) | Production ok; plan draft wrong |
| Fallback arithmetic | `TariffFor()` only (already a total) | `TariffFor()` only — margin/wear **not** re-applied (B4) | Production ok |
| Factory gate (`tons<0`) | Pays building as factory sale **without** excluding `SHIP_FUEL`/`SITE_REFUEL` (G5 latent double-pay) | Gate on `s.kind != SHIP_FUEL && known != SITE_REFUEL`; add counter/log if fuel reaches it | Not implemented |
| Wear allocation | Charged per delivery via `TariffFor()` total | Once per **trip**, reset at mode-1 load/return (C5) | Not implemented |
| Baseline advancement | `ReadTank()` advances baseline only on readable reads | Baseline advances on **every** readable mode-1 event, independent of match/payment (C2) | Partial |
| Unreadable handling | Refill counted, negative mirror possible | Unreadable closes window, cannot anchor usable new one; baseline recovery never repairs missing coverage | Not implemented |
| Association | p9/ring; no verified/heuristic/ambiguous quality | Verified → live; heuristic → simulated only; ambiguous/unmatched → no payment | Not implemented |
| Dry-run vs live | `Pay()` computes + logs "would"; moves nothing in dry-run | Actual and simulated **never** overlap; heuristic never pays live | Partial |
| Save/load | Persistence only for auto-target permissions (`AutoPersistenceEvent`) | Invalidate tank baselines + carrier identities **before any consumer** | Not implemented |
| Rehoming | None (no vehicle-state lifecycle) | Rehome invalidates baseline/window; never charge old-owner consumption to new owner | Not implemented |
| Dedup / TTL | Ledger ring (`g_ledger`, 512); carrier ring (32, 2s) | Pending-delivery ring with TTL; dedup-record eviction; sweep every event | Not implemented |
| Validation | Fleet-level `[TANK] run` vs `[TRIP FUEL] run` (mismatched populations, G11) | Per-vehicle vs `ReadTank()` daily sum; fleet totals only secondary | Not implemented |

---

## 2. Blocking design defects (adopted from `docs/review-per-trip-fuel-plan.md`)

**B1 — The mode-0 event has no vehicle.** `p2` is the building at mode 0 and the vehicle at mode 1 (`private_logistics.cpp:1146-1147`). Any design that reads a vehicle/tank at mode 0 is unsound. Carrier payment must be deferred to the mode-1 pair (Option A).

**B3 — Fuel price is per tonne.** `FuelUnitPrice()` returns RUB/tonne (`src/resource_pricing_api.h:104`). Trip-tariff fuel cost must be `burn_tons * fuel_price`, wear = `wear_litres * diesel_density * fuel_price`. The plan draft's `litres * price` is 1000x too high (~2649 RUB vs ~1.96 RUB at 133/RUB).

**B4 — Fallback applies margin/wear twice.** `TariffFor()` already returns a total `fuel*(1+margin)+wear`. The plan draft's fallback recomputed `TariffFor() * (1+margin) + wear`. Fix: use `TariffFor()` only; optional `wearLitresOverride` to avoid double-charging wear mid-trip (C13).

**B5 — `SITE_REFUEL` is not a trip boundary.** Never observed, stochastic, active-only (`veh+0x448 in {1,2}` and `veh+0x440==9`), and the mode-0 refuel call carries no vehicle. Drop it entirely; detect refills from the tank **increase** at the next mode-1 event (existing `ReadTank()` already does this: `level > prev + 0.02` → count refill, do not subtract).

**B6 — Return detection uses the wrong site.** `SITE_TRANSSHIP` (`0x68F7F9`) is the vehicle side of **every** delivery, not a return-to-depot. Using it closes the trip at each drop, undercounting deadhead/return and overcharging wear on multi-drop routes. Gate return on `isStaging` (`vehicleHome == p9`) behind `trip_return_via_p9`, never required for pricing.

**G1 — Carrier ring / vehicle identity.** The ring joins mode-0 to mode-1 by resource+tons+time; `p9`'s meaning is site-dependent (C14), not a single "fleet owner" truth. `p9` must be an explicit pre-step with four documented outcomes at `0x68F792`.

**G3 — Refill state.** `g_vehFuel[]` (day-close baseline, `ReadTank()`) and the new per-vehicle `VehFuelState` must **never** write each other. Separate `ReadTankNow()` pure reader.

**G4 — CPI fallback.** If neither fuel price nor CPI resolves, fail-closed (unpriceable, refuse); never invent a currency figure. Every `[TRIP FUEL]` line carries a `source=` field reported by reason.

**G5 — Factory payment on fuel (latent double-pay).** At mode 0 `tons<0`, the factory block pays `building` without checking `s.kind`. A `SHIP_FUEL` transfer from a private building would be booked as a sale. Fix: `if (... && s.kind != SHIP_FUEL && known != SITE_REFUEL)`, plus a counter/log if fuel reaches it.

**G9 — Not "full cost".** The model is fuel + a 5 L policy reserve + margin. Wages, vehicle purchase/depreciation, maintenance, overhead are out of scope. Rename to "per-trip fuel cost" or add a full-cost section with data sources.

**G11 — Validation population mismatch.** `[TANK] run` denominator is `g_tariffCount` (deliveries with a positive tariff), not all deliveries; `[TRIP FUEL]` would be only paid deliveries. The two cannot match. Validate per vehicle against `ReadTank()` per-vehicle burned; fleet totals secondary.

---

## 3. Required production integration changes

1. **Implement Option A:** in `DetourTransfer`, mode-0 `tons>0` → `StashDelivery()` (no money); mode-1 committed → `OnVehicleEvent()`: match pending record, read tank via pure `ReadTankNow()`, compute delta, `PriceAndPay()`, `Pay()` resolved payee. Keep existing mode-0 probe block only when `per_trip_fuel == 0`.
2. **Add `VehFuelState[VEH_MAX]`**, indexed via existing `VehSlot()` (no second linear scan). Fields: vehicle, home, lastTons, lastT, tripFuel, tripDrops, wearCharged, valid. Guard slot `-1` (C6), unreadable `-1.0`, and `cap <= 0.0`.
3. **Add pure `ReadTankNow(v)`**: validated tank level, leaves daily `g_vehFuel[]`/baseline alone. Call per event; `ReadTank()` keeps its day-close job. Never let them write each other (G3).
4. **`ResolvePayee()`:** vehicle's home building first (`veh+0x4F8`), then `p9`, then carrier-ring fallback (`:1375-1378`). Apply `IsPrivate`, `owner != building`, NULL-owner fallback.
5. **Gate factory payment** (G5): exclude `SHIP_FUEL`/`SITE_REFUEL`; add counter/log.
6. **Wear once per trip:** reset `tripFuel`, `wearCharged`, `tripDrops` at mode-1 load/return (C5).
7. **Add `PendingDelivery` ring** (`PENDING_RING 32`) with 2 s TTL; sweep every event (C10); bound `g_pendingHead % PENDING_RING` (C7).
8. **`Pay()` call sites** stay outside `g_lock` (they take it internally). Pending ring touched only inside detour.
9. **`[TRIP FUEL]` logging** at the paid event with `source=` field (`measured | fallback:no-baseline | fallback:refill | fallback:no-vehicle | fallback:invalid | fallback:no-index`) and daily per-source counts (G4, review §7).
10. **Save/load handler:** invalidate tank baselines and carrier identities before any consumer runs; retain persistent contamination reasons.
11. **Rehoming:** on carrier-identity change, invalidate baseline + open window; never charge old-owner consumption to new owner.
12. **Per-vehicle validation:** sum `[TRIP FUEL]` deltas per vehicle vs `ReadTank()` per-vehicle burned over the same window (refills excluded); fleet totals only as a secondary check (G11).
13. **Document ordering change:** payment moves 30-70 ms later (mode-1 tick), changing `[PAY]` vs `[B2B]` line ordering and `daily_pay_cap` counting tick.

---

## 4. Current `Pay()` dry-run counters (production, `private_logistics.cpp`)

These are the existing refusal/tally counters that any production integration must preserve or extend:

| Counter | Meaning | Where incremented |
|---------|---------|-------------------|
| `g_skipSmall` | amount `< min_payment` | `Pay()` `:1053` |
| `g_skipNoPayee` | missing enterprise/currency service or no payee | `Pay()` `:1058`, detour `:1396` |
| `g_skipNoPrice` | `unitPrice <= 0` (factory leg) or `freight <= 0` (carrier leg) | `Pay()` `:1058`; detour `:1387`, `:1429` |
| `g_skipTreasury` | treasury cannot afford the amount | `Pay()` `:1087` |
| `g_payCapped` | `daily_pay_cap` exceeded | `Pay()` `:1068` |
| `g_tariffNoIndex` | delivery could not be priced (no fuel price, no CPI) | detour `:1366` |
| `g_payCount` / `g_paidToday` | payments computed (dry-run: "would"; live: moved) | `Pay()` `:1127-1129` |
| `g_paidFactory` / `g_paidCarrier` | split by `TSM_CAP_PRODUCTION` vs `TSM_CAP_WHOLESALE` | `Pay()` `:1130-1131` |
| `g_wouldPayFreight` / `g_wouldPayFuel` | freight/fuel value tallied at committed sites | detour `:1283-1285` |
| `g_treasuryChecked` / `g_treasuryExact` / `g_treasuryOff` / `g_treasuryMaxGap` | treasury leg sampled either side of the move (H33) | `Pay()` `:1113-1119` |

Note: in `dry_run = 1`, `Pay()` logs `[PAY] would ...` and still increments `g_payCount`/`g_paidToday`/category totals but moves no money — so dry-run counters **overstate** live payments until `dry_run = 0`. The treasury leg (`getTreasuryBalance` before/after) is **not** sampled in dry-run (`g_treasuryChecked` stays 0).

---

## 5. Provisional assumptions (not approved requirements)

- Verified association, actual quantity and carrier identity are **inputs** from a future association adapter; the model does not perform resource/quantity/time matching.
- `trip_tariff = 0` → value pricing `cargo_tons * unit_price * freight_share`, independent of fuel/CPI/fallback.
- Fail-closed: no live fuel price and no CPI basket → unpriceable → refuse; no invented currency figure.
- Heuristic association → simulated (dry-run) only, **never** live; ambiguous/unmatched → no payment in either mode.
- Deterministic injected clock; no real wall clock in the model.
- Save/load invalidates baselines + identities; persistent contamination reasons retained.
