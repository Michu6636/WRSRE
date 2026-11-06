# Transition table & association-adapter requirements (v0.18)

Companion to `findings.md`. Proposes how production `private_logistics` moves
from v0.17 (mode-0 billing, value share / trip tariff probe) to the v0.18
per-trip measurement and settlement semantics. Provisional only — no production
source is changed by this document.

---

## 1. Proposed transition table

| v0.17 behaviour | v0.18 target behaviour | Transition action |
|-----------------|------------------------|-------------------|
| `Pay()` at mode 0, `p2` = building, carrier from `p9`/ring | Carrier `Pay()` deferred to mode-1 pair (vehicle present) | Add `StashDelivery()` at mode 0; `OnVehicleEvent()` + `PriceAndPay()` at mode 1 (Option A) |
| `TariffFor()` total applied per delivery | Same total, but `burn_tons * fuel_price` for measured quantity; wear once per trip | `ReadTankNow()` per event; reset wear/tripFuel at mode-1 load/return |
| `SITE_REFUEL` (`0x16F831`) treated as a possible trip boundary | `SITE_REFUEL` dropped entirely (never observed, stochastic, no vehicle) | Remove refuel-as-boundary; detect refills from tank increase at next mode-1 event |
| `ReadTank()` day-close baseline shared with per-event measurement | `ReadTank()` keeps day-close baseline; new pure `ReadTankNow()` for per-event use; never share state | Split reader; guard `VehSlot() == -1`, `cap <= 0`, unreadable `-1.0` |
| `p9` treated as the fleet owner (tautological at `0x68F7F9`) | `vehicleHome = *(veh+0x4F8)` first; `p9` secondary candidate with four documented outcomes | Add `ResolvePayee()`; document `p9` as site-dependent at `0x68F792` (C14) |
| Factory gate pays `tons<0` building unconditionally | Exclude `SHIP_FUEL` / `SITE_REFUEL`; counter/log if fuel reaches it | Gate factory payment (G5) |
| `g_payCount`/`g_paidToday` incremented in dry-run ("would") | Actual and simulated **never** overlap; dry-run tallies separate | Separate `simulated_total` / `actual_total` counters |
| No per-vehicle dedup/TTL on pending deliveries | Pending-delivery ring with 2 s TTL, sweep every event, bounded index | Add `PendingDelivery[PENDING_RING]`, TTL sweep, `g_pendingHead % PENDING_RING` |
| Save/load persists only auto-target permissions | Invalidate tank baselines + carrier identities before any consumer | Extend persistence handler / add save-load invalidation |
| No rehoming handling | Rehome invalidates baseline + open window; no old-owner charge to new owner | On carrier-identity change, reset vehicle state |
| Validation: fleet-level `[TANK]` vs `[TRIP FUEL]` (mismatched) | Per-vehicle vs `ReadTank()` per-vehicle burned; fleet totals secondary | Add per-vehicle `[TRIP FUEL]` sum vs daily burned comparison |

---

## 2. Association-adapter input requirements

The model treats verified association, actual quantity and carrier identity as
**inputs from a future association adapter**. The adapter must supply, per
delivery event, the following. None of these may be invented by the plugin;
each must carry a quality flag so the model can gate settlement.

| Input | Type | Requirement | Used for |
|-------|------|-------------|----------|
| `association_quality` | enum {verified, heuristic, ambiguous, unmatched} | Mandatory. Set by the adapter after resource/quantity/time matching. | Settlement gating: verified → live; heuristic → simulated only; ambiguous/unmatched → no payment |
| `carrier_id` | opaque handle | Mandatory when quality != unmatched. Stable across save/load (identity, not a raw game pointer). | Payee resolution; rehoming detection |
| `actual_quantity_tons` | float >= 0 | The measured fuel burned for the interval (from tank delta). | Trip-tariff pricing (`burn_tons * fuel_price`) |
| `cargo_tons` | float > 0 | Tons delivered on this delivery leg. | Value pricing (`trip_tariff=0`) |
| `unit_price_rub_per_tonne` | float > 0 | Live price at settlement time. | Value pricing |
| `delivery_timestamp` | monotonic seconds | For TTL matching (mode-0 → mode-1 within 2 s) and dedup. | Pending-ring match, expiry fallback |
| `event_id` | opaque | Unique per delivery for dedup-record identity; ring-evictable. | Duplicate-event suppression |

### Adapter quality rules the model enforces

1. **Verified** → `Pay()` may move money in live mode (subject to treasury/minimum/cap).
2. **Heuristic** → `Pay()` may only log "would pay" (simulated). In live mode it is **refused** (reason `association`); never actual.
3. **Ambiguous / unmatched** → `Pay()` refused in **both** modes; no simulated amount either. The physical accounting window still closes (measurement continues), but no payment is requested.
4. Missing vehicle storage does **not** discard a known `carrier_id`; it only blocks tank measurement.

### What the adapter must NOT do

- Do not treat resource/quantity/time matching as verified identity. A match raises quality to "heuristic" at best; "verified" requires an independent authority.
- Do not invent game offsets or claim the model validates the real game.
- Do not persist raw `Building*`/`Vehicle*` pointers across save/load; use hashed stable identity keys.

---

## 3. Settlement decision table (adapter input → model outcome)

| Quality | Mode | Fuel price | CPI | Outcome | `pricing_source` |
|---------|------|------------|-----|---------|------------------|
| verified | live | ok | any | actual pay | actual_quantity |
| verified | dry-run | ok | any | simulated | actual_quantity |
| verified | live/dry-run | missing | ok | pay (simulated if dry-run) | cpi |
| verified | live/dry-run | missing | missing | refused: unpriceable | none |
| heuristic | dry-run | ok | any | simulated | actual_quantity / cpi / value |
| heuristic | live | any | any | refused: association | — |
| ambiguous/unmatched | live/dry-run | any | any | refused: association | — |
| trip_tariff=0 | any | any | any | value pricing (cargo*price*share) | value |
