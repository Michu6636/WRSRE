# Task: remove the market multiplier and state procurement (economy code step 1)

## Contract

- **Status:** active. Host-side work is done. The Windows build is pending
  CI, and nothing has been run in-game.
- **Task record:** `docs/tasks/active/2026-11-22-remove-market-multiplier-and-procurement.md`
- **Baseline:** branch `arena/672c9ef4-wrsre` from `main` `01ff95a` (PR #99
  merged), clean worktree.
- **Goal:** code step 1 of
  [economy direction §11](../../visions/economy-direction-2026-11-22.md):
  - money uses the native price only (D2/D3);
  - the treasury no longer buys private output on dispatch (D4).
- **Deliverables:**
  - `resource_pricing` v5 without the market book or local scarcity;
  - every consumer on the plain native price;
  - `private_shops` v3.6 without procurement;
  - updated INIs, tests and docs.
- **Acceptance criteria:**
  1. No code path records market supply/demand or applies a market or
     local-scarcity multiplier.
  2. No `moveTreasury(..., "procurement")` / `TSM_CAP_PRODUCTION` credit
     for dispatched private output.
  3. The pricing ABI is consistent: the v5 struct and the v1 prefix are
     published, and every in-tree consumer requests v5 → v1.
  4. The full host test suite passes. CI `build-windows` is green.
- **Non-goals (later steps):**
  - fallback prices, `usd_divisor` and `UNPRICED`;
  - the 2.15 CPI fallback;
  - `wholesale_ratio` 0.70 delivery billing;
  - foreign restock;
  - B2B defaults and customs attribution.
- **Authorized:** repository edits, host tests, commit/push/PR on the
  session branch (owner approved 2026-11-22).
- **Forbidden:** game launch or deployment; Ghidra.
- **Ghidra owner:** none.
- **Task-owned files:**
  - `src/resource_pricing_api.h`, `src/ui_panels_api.h` (comment);
  - `plugins/resource_pricing/*`, `plugins/private_shops/*`,
    `plugins/citizen_retail/*`;
  - `plugins/enterprise_base/enterprise_base.cpp`;
  - `plugins/private_logistics/{private_logistics.cpp,panel_fuel_runtime.h,dry_assignment_runtime.h,b2b_settlement.h,private_logistics.ini}`;
  - `plugins/taxes/*`, `plugins/republic_bank/*`;
  - `plugins/private_construction/private_construction.cpp`;
  - the tests listed below, `docs/SUPERSEDED.md`, `docs/engine/README.md`
    and economy doc §11.

## What changed

| Area | Change |
|---|---|
| `resource_pricing_api.h` | v5 layout: the v1 fields + `getCpiDailyBasket`. Removed `recordSupply`, `recordDemand`, `getMarketMultiplier`, `getMarketBalance`, `getLocalScarcityMultiplier`, `getLocalPriceQuote`, their helpers and the `V2`/`V3`/`V4` version macros. Removing fields reorders the layout, so this is a new version, not an append. |
| `resource_pricing` 5.0 | Removed the market book (map, rebalance, `sim_clock`/persistence subscriptions, `[market_balance]`) and local scarcity (`scarcity_curve.h`, `[local_scarcity]`). The CPI basket uses native prices with no multiplier. Publishes v1 (prefix through `getPrice`) and v5 only. |
| `private_shops` 3.6 | Removed state procurement: the treasury payout, `procurement_price_mult`, `procurement_flag_fallback`, `DispatchFallbackAllowed`, the export/import direction reads and `[PROC]` logs. Market supply/demand recording is removed. The B2B-active visitor is snapshot-only. **Growth in an export-flagged storage (own production) is no longer billed** as a state delivery. |
| `citizen_retail` 3.0 | Checkout and the destitute bread-day use the native price. Removed the market demand tap, its `g_demandDay` dedup and `ShopStocksResource`. |
| `enterprise_base` 9.20 | The destitute bread-day uses the native price. Removed the building-window "Market" Shortage/Glut row. |
| `private_logistics` | `UnitPrice` and the fuel panel quote use the native price. Reason/log strings renamed (`global_native_price`, `price_policy=global_native_price_at_observed_close`). `PL_VERSION` unchanged (tests pin it). |
| `taxes`, `republic_bank`, `private_construction` | Consume v5 → v1. |

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| S1-1 | high | Removing procurement exposed this: `WholesaleSlot` billed **any** stock growth, including a factory's own output filling its export storage, at `wholesale_ratio`. Before, the 0.90 dispatch payment offset it. With `private_logistics` in dry-run by default, factories would only pay. | fixed (owner chose option "don't bill export-storage growth") | `private_shops.cpp` `WholesaleSlot`: `isExport` (+0x85) growth is not billed | `test_wholesale_debt.py` export case | An **unflagged** factory output storage is still billed until B2B billing replaces the 0.70 bill |
| S1-2 | info | `tests/resource_pricing/scarcity_curve_test.cpp` was not wired into CI | out of scope, deleted with the code it tested | — | — | — |

## Build/test evidence

| Check | Command / cwd | Exit | Coverage / limits |
|---|---|---|---|
| Full host suite | `for f in $(find tests -name 'test_*.py'); do PYTHONPATH=. python3 $f; done` (repo root) | 0 (all files pass) | Host only |
| Almanac + tools | `run_host_tests.py`, `run_binary_sha256_tests.py`, `run_tax_policy_tests.py`, `almanac_overlay_dev/run_host_tests.py`, `audit_economy_csv.py selftest`, `peinfo.py selftest` | 0 | Host only |
| Header compile | g++ C++17 and gcc C11, `-Wall -Wextra -Werror`, layout `static_assert`s | 0 | Header only |
| Plugin syntax | g++ `-fsyntax-only` with a throwaway Windows shim, base vs new | No new errors in any edited plugin | Not MSVC; SEH is faked |
| Windows build | CI `build-windows` | pending | Real MSVC verification |

## Separate validation statuses

- **STATIC REVIEW:** self-review only.
- **BUILD/TEST EVIDENCE:** PARTIAL. Host tests pass; the MSVC build is pending CI.
- **RUNTIME VALIDATION:** NOT RUN. No in-game session.

## Resume checkpoint

- **Next action:** confirm CI `build-windows` on the PR. Then run code step 2
  (fallback prices and made-up sample data).
- **Save compatibility:** unchanged. Neither the market book nor procurement
  persisted anything. Old INI keys are ignored.
