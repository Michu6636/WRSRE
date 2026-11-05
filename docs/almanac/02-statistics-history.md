# Almanac Statistics & History (02)

UI-neutral, read-only statistics and bounded-history core for the future Almanac.
This module derives presentation data; it is not an economic source of truth.

## Scope
- Owns only: `docs/almanac/02-statistics-history.md`, `plugins/almanac/statistics_history.h`,
  `plugins/almanac/statistics_history.cpp`, `tests/almanac/statistics_history_test.cpp`,
  `tests/almanac/test_statistics_history.py`.
- No changes to service APIs, plugins, UI shell, host contract, or game memory.

## Design
1. **Injected read-only source adapters.** The portable core never consumes services or
   reads game memory. Integration wires `Sources` (clock, currency, enterprise, pricing,
   flow, logistics) to public services later. Missing adapters/sources report unavailable.
2. **Metric model.** Every `Metric` carries: stable `MetricId` + `idString`, `schemaVersion`,
   value + validity (`hasValue`, `valueFinite`, `available`, `stale`), unit, currency code,
   stock vs flow (`MetricKind`), period start/end and as-of `SimDate`, source/provenance,
   value class (`ACTUAL`, `DRY_RUN`, `ESTIMATED`).
   - Actual, dry-run and estimated values are structurally distinct via `valueClass`.
   - They are never summed merely because numeric units match.
   - Different currencies or incompatible units are never summed. `MID_OVERVIEW_MONEY_SUPPLY`
     is computed only when all components are available, finite and in the same currency.
3. **Fixed-capacity snapshots.** `Snapshot` holds `Metric metrics[SH_MAX_METRICS]` (96) with
   `metricCount`. `getSnapshot()` returns a coherent copy by value; no live pointers or
   iterables escape. No allocation in the render-read path.
4. **Bounded history.** Ring buffer of `SH_HISTORY_CAPACITY` (120) `Snapshot` samples.
   Eviction overwrites the oldest sample; ordering is oldest->newest and deterministic.
   `readHistory()` copies into a caller-provided buffer.
5. **Reset behaviour.** `reset(ResetReason)` clears current metrics and history. `update()`
   detects backward simulation time and currency changes and resets derived history.
   Derived history may be reset; no duplicate economic state is persisted by this module.
   Reasons: `RR_SAVE_LOAD`, `RR_WORLD_CHANGE`, `RR_BACKWARD_TIME`, `RR_CURRENCY_CHANGE`,
   `RR_MANUAL`.
6. **Metric families.** Overview, Money Flow, Private Economy, Prices & Citizens,
   Logistics & Production. Unsupported metrics are reported unavailable with a documented
   missing source (e.g. `citizen.count`, `production.output`).

## Metric IDs
See `MetricId` enum in `statistics_history.h`. Stable string IDs via `metricIdString()`.

### Overview
- `overview.treasury` (stock, currency, ACTUAL) - state treasury.
- `overview.wallets_total` (stock, currency, ACTUAL) - total citizen wallets.
- `overview.money_supply` (stock, currency, ACTUAL) - treasury+wallets+enterprise+pool, same currency only.
- `overview.investment_pool` (stock, currency, ACTUAL).
- `overview.enterprise_capital_total` (stock, currency, ACTUAL).
- `overview.enterprise_count` (stock, count, ACTUAL).

### Money Flow (monthly, closed-period flows)
- `flow.wages`, `flow.pensions`, `flow.welfare`, `flow.state_store_sales`,
  `flow.wholesale_b2b`, `flow.vat`, `flow.profit_tax`, `flow.property_tax`,
  `flow.net_treasury`, `flow.unexplained` (flow, currency, ACTUAL, when flow source present).

### Private Economy
- `private.sales`, `private.wages`, `private.taxes`, `private.pool_inflow` (flow, currency, ACTUAL).
- `private.enterprise_count` (stock, count, ACTUAL).
- `private.evictions` (flow, count, ACTUAL).
- `private.rent_monthly` (stock, currency, ACTUAL).

### Prices & Citizens
- `price.food`, `price.meat`, `price.clothes`, `price.electronics` (stock, currency/ton, ACTUAL).
- `price.cpi_daily_basket` (stock, currency/day, ACTUAL).
- `price.market_mult_food` (stock, ratio, ACTUAL).
- `citizen.wallets_total` (stock, currency, ACTUAL).
- `citizen.count` (UNAVAILABLE - no citizen count source; never invented).

### Logistics & Production
- `logistics.fuel_consumed` (flow, fuel, DRY_RUN) - dry-run billing semantics.
- `logistics.trip_count` (flow, count, DRY_RUN).
- `logistics.tons_moved` (flow, tons, DRY_RUN).
- `logistics.dry_run` (stock, flag, ACTUAL) - reports configured dry-run flag (default 1).
- `production.output` (UNAVAILABLE - no production source).

## Period boundaries
- Stock metrics: periodStart == periodEnd == asOf date.
- Flow metrics: periodStart is day 1 of the as-of month/year; periodEnd is asOf.
- As-of date is supplied by the caller (integration) from sim_clock.

## Currency handling
- Active currency resolved from the injected currency source (0=RUB,1=USD).
- Currency change triggers reset of derived history and current metrics.
- Metric `currency` field and `unit` string reflect the active currency.
- No cross-currency summation.

## Snapshot consistency
- `getSnapshot()` takes the internal mutex and returns a value copy.
- `coherent == 1` on every returned snapshot.
- Sequence number increments on each `update()`.

## Unavailable values
- Missing source adapter or failed/non-finite source value -> `available=0`, `hasValue=0`.
- `citizen.count` and `production.output` are permanently unavailable in this core
  (no supporting service); integration must add a source to enable them. They are never
  estimated or presented as actual.

## Integration requirements (future)
- Wire `Sources` to public services: sim_clock, currency, enterprise_base, resource_pricing,
  flow observers, private_logistics.
- Call `update(asOf)` on tick/day boundaries; `recordHistory(asOf)` on period close.
- Call `reset(RR_SAVE_LOAD)` / `RR_WORLD_CHANGE` on save load and world changes.
- Render thread calls only `getSnapshot()` / `readHistory()`; no service calls or allocation.
- Do not persist duplicate economic state; history is derived and resettable.

## Tests
- `tests/almanac/test_statistics_history.py` compiles and runs `statistics_history_test.cpp`.
- Command: `python tests/almanac/test_statistics_history.py`
- Also runnable directly with g++:
  `g++ -std=c++17 -I plugins/almanac tests/almanac/statistics_history_test.cpp plugins/almanac/statistics_history.cpp -o sh.exe`
- Synthetic tests only; no runtime/game validation claimed.
