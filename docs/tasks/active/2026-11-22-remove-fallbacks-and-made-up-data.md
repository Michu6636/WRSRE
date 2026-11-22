# Task: remove fallbacks and made-up data (roadmap step 2)

## Contract

- **Status:** active — implementation, host tests and the Windows CI build
  complete; independent review and the owner's in-game check are outstanding.
- **Task record:** `docs/tasks/active/2026-11-22-remove-fallbacks-and-made-up-data.md`
- **Baseline:** branch `arena/c676e273-wrsre`, from `main` at `71e2fb0`
  (PR #100 merged). Working tree contains only this task's changes.
- **Goal:** every displayed or charged number comes from the game, a player
  policy or a documented INI constant (invariant I6). A missing native price is
  `UNPRICED`; a missing CPI basket is `UNAVAILABLE`. D32, D33 and D34 are
  implemented.
- **Deliverables:**
  - `resource_pricing` v6: no `[fallback_prices]`, `[fallback_prices_usd]`,
    `default`, `default_usd` or `usd_divisor`; `getPrice` returns 0.0f for an
    unpriced resource and `getCpiDailyBasket` returns 0.0f when unavailable.
  - D32 in `citizen_retail`: an unpriced sale gives the good, moves no money,
    logs `ERROR UNPRICED`, and is counted for the Almanac (API v3);
    `min_cost` cannot manufacture a charge.
  - D33 in `private_construction`, `private_shops` and `private_logistics`:
    an unpriced bill refuses the whole transaction, loudly, never partially.
    The invented 8 t restock cap, the `0.5` reorder repair and the duplicate
    `13.30` USD wage fallback are gone as well.
  - D34 in `citizen_retail`, `republic_bank` and `taxes`: no default basket;
    demand scaling is skipped that day, CPI/runway show `UNAVAILABLE`, and the
    bank refuses new credit until CPI returns.
  - D35: `[cpi_basket]` carries separate RUB (`default_daily_rent`) and USD
    (`default_daily_rent_usd`) defaults, both 0.33; nothing divides either. A
    missing USD key makes the USD basket UNAVAILABLE.
  - `almanac_overlay_dev`: no synthetic rows; `NO DATA: <service>
    unavailable` in warning colour.
  - New host tests under `tests/resource_pricing`, `tests/citizen_retail`,
    `tests/private_construction`, `tests/enterprise_base`, `tests/taxes`,
    `tests/republic_bank` and `tests/almanac_overlay_dev`.
- **Acceptance criteria:**
  - `grep` finds no fallback price, sample Almanac row or default CPI basket
    (see the Finding Ledger, F1–F12).
  - A host test proves an unpriced sale is not charged (D32), an unpriced
    material refuses the whole estimate (D33), and no CPI answers 0.0 rather
    than 2.15 (D34).
  - The Almanac source has no sample values and renders `NO DATA` when a
    service is absent.
  - Every host test file ends with `unittest.main()` and passes from the
    repository root.
- **Non-goals:** live B2B billing defaults (roadmap step 3), the 0.70 delivery
  bill and foreign restock removal (step 3), `*_usd` keys outside the price
  path and flat-money re-basing (step 7), wage indexation (step 5), City Hall
  tile removal (step 6). `default_daily_rent`, `default_daily_rent_usd`,
  `reference_cpi_rub` and `min_cost` remain documented constants until step 7.
- **Authorized:** inspect repo files; small verified edits; host tests;
  documentation updates. No Windows build or game launch available here.
- **Forbidden:** launching `SOVIET64.exe`, deploying DLLs, writing game memory,
  changing Kilo settings/models, creating/switching branches, destructive Git.
- **Ghidra owner:** none needed (no native claim in this task).
- **Task-owned files:**
  - `src/resource_pricing_api.h`, `src/citizen_retail_api.h`,
    `src/price_guards.h`
  - `plugins/resource_pricing/*`
  - `plugins/citizen_retail/*`
  - `plugins/enterprise_base/enterprise_base.cpp`
  - `plugins/private_construction/private_construction.cpp`
  - `plugins/private_shops/private_shops.cpp`
  - `plugins/private_logistics/private_logistics.cpp`
  - `plugins/republic_bank/*` (including `bank_ledger.h`)
  - `plugins/taxes/taxes.cpp`
  - `plugins/almanac_overlay_dev/almanac_panel_view.{h,cpp}`,
    `graphics_hook.cpp`, `TESTING.md`
  - `tests/resource_pricing/`, `tests/citizen_retail/`,
    `tests/private_construction/`, `tests/enterprise_base/`, `tests/taxes/`,
    `tests/republic_bank/test_credit_without_cpi.py`,
    `tests/almanac_overlay_dev/test_no_synthetic_data.py`,
    `tests/private_shops/test_wholesale_debt.py`
  - `docs/tasks/active/2026-11-22-remove-fallbacks-and-made-up-data.md`,
    `docs/tasks/README.md`, `docs/ROADMAP.md`,
    `docs/visions/economy-direction-2026-11-22.md`, `docs/SUPERSEDED.md`
- **Starting evidence:**
  `docs/visions/economy-direction-2026-11-22.md` §3.2, §11 items 1 and 9;
  `docs/ROADMAP.md` step 2; D32–D34.

## Prior findings imported

These are the step-2 call sites listed in `docs/ROADMAP.md` and the economy
direction. Each row is the finding as written there, not a new claim.

| Finding ID | Source path/line/address | Exact statement | Initial status (pre-change) |
|---|---|---|---|
| F1 | `resource_pricing.ini` / `resource_pricing.cpp` | `[fallback_prices]`, `[fallback_prices_usd]`, `default`, `default_usd` and `usd_divisor` invent prices. | open |
| F2 | `resource_pricing.cpp` `GetCpiDailyBasketImpl` | A faulted/unpriced basket falls back to 2.15 RUB/day. | open |
| F3 | `citizen_retail.cpp` `CheckoutAffordabilityGate` | `min_cost` can turn a missing price into a charge. | open |
| F4 | `citizen_retail.cpp` `SolCpiDaily` | No CPI means the 2.15 default basket. | open |
| F5 | `citizen_retail.cpp` `DestituteBreadCost` | Missing food price silently disables the destitute class. | open |
| F6 | `enterprise_base.cpp` `CollectResidentialRent` | Missing food price silently disables the bread-day stage. | open |
| F7 | `private_construction.cpp` `PriceOne` / panel loop | A missing material price contributes 0 to a partly billed estimate. | open |
| F8 | `private_shops.cpp` restock/wholesale | Missing price silently skips the bill; foreign restock can write stock. | open |
| F9 | `private_logistics.cpp` `UnitPrice` / settlement | `UnitPrice` may skip a delivery silently and parts can settle selectively. | open |
| F10 | `republic_bank.cpp` `CurrentEconomicScale` | Missing CPI substitutes a neutral/divisor scale and still lends. | open |
| F11 | `taxes.cpp` City Hall macro rows | CPI/poverty show `n/a` and the log claims a 2.15 default. | open |
| F12 | `almanac_overlay_dev/almanac_panel_view.cpp` | `kEconomyRows`/`kLabourRows`/`kLogisticsRows` and `DrawTable` show synthetic values. | open |
| F13 | `resource_pricing.cpp` `GetCpiDailyBasketImpl` | USD mode used the RUB `default_daily_rent` without conversion (D35 review). | open |
| F14 | `resource_pricing.cpp` `TsmPluginInit` | Publishing the v1 prefix lets a pre-step-2 consumer read UNPRICED `0.0f` as free. | open |
| F15 | `private_construction.cpp` `NativeCostForBuilding` / `[WAITING]` | UNPRICED logs every pass and the waiting line tells the player to open a panel that cannot help. | open |
| F16 | `almanac_overlay_dev/graphics_hook.cpp` | The “Unpriced shop sales” row is drawn in normal colour. | open |
| F17 | `private_logistics.cpp` B2B goods refusal | Only the first refused delivery is ever described (session-wide bool). | open |

## Gate status

| Gate | Frozen artifact/revision | Focus 1 | Focus 2 | Open IDs / blocker |
|---|---|---|---|---|
| INVESTIGATION | this record + economy direction §3.2/§11 | self-review | not available | none |
| PLAN | this record, Deliverables | self-review | not available | none |
| IMPLEMENTATION | current working tree | self-review | not available | owner in-game check (roadmap step 0/2) |

**Independent review blocker:** this session has one agent and no
`reviewer-focus1`/`reviewer-focus2` runs. The AGENTS.md contract requires both
read-only reviewers before an implementation is called approved; that gate is
recorded as **not available**, not passed. Static review below is a self-review.

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| F1 | high | Fallback/default price table invents data | fixed | `resource_pricing.cpp`/`.ini` rewritten: table, default and divisor removed; `getPrice` returns 0.0f and logs `ERROR UNPRICED` once per resource/currency | `tests/resource_pricing/test_no_fallbacks.py` | source + header host test only |
| F2 | high | Default 2.15 CPI basket | fixed | `GetCpiDailyBasketImpl` returns 0.0f unless all three goods are priced; `TsmPricingCpiBasket` returns 0.0f without the tail | same test, CPI harness | no world-load path on host |
| F3 | high | `min_cost` charges an unpriced sale | fixed | `plugins/citizen_retail/checkout_policy.h` `QuoteSale` returns `priced=false` before the floor; gate returns 1 and counts | `tests/citizen_retail/test_unpriced_sale.py` | hook itself not run |
| F4 | high | 2.15 fallback in `SolCpiDaily` | fixed | no default; 0.0f logs `ERROR CPI UNAVAILABLE` once per day and `ApplySolScaling` returns | `tests/citizen_retail/test_unpriced_sale.py` | source contract |
| F5 | medium | Silent destitute class when food unpriced | fixed | 0.0f is already handled as "no class"; with CPI unavailable the whole scaling path is skipped and logged | source review | no live citizen |
| F6 | medium | Silent bread-day when food unpriced | fixed | explicit `TsmNativePriceUsable(pFood)` branch logs `ERROR UNPRICED` and skips the stage | `tests/enterprise_base/test_unpriced_bread.py` | source contract |
| F7 | high | Partial construction estimate | fixed | `PriceOne` returns false; panel loop returns 0; native phase walk fails with `unpriced construction material`; site waits, never claims | `tests/private_construction/test_unpriced_estimate.py` (executes `PriceOne`) | native walk not executable on host |
| F8 | high | Silent/piecemeal shop delivery bill | fixed | both paths test `TsmNativePriceUsable`; log once; wholesale snapshot advances with the refused delivery, restock writes nothing | `tests/private_shops/test_wholesale_debt.py` (executes the settlement tail) | restock path source-only |
| F9 | high | Selective B2B settlement | fixed | `UnitPrice` logs once per resource; delivery pre-scan refuses ALL parts if any is unpriced; daily line reports `unpriced=N` | `tests/private_construction/test_unpriced_estimate.py` source checks, existing B2B tests | observed lifecycle not live |
| F10 | high | Neutral scale/lending without CPI | fixed | `CurrentEconomicScale` returns 0.0f; `requestCreditDrawImpl` and `onDay` refuse; daily/save logs say UNAVAILABLE | `tests/republic_bank/test_credit_without_cpi.py` (executes the scale) | no native loan run |
| F11 | low | `n/a` and a 2.15 log claim | fixed | both rows say `UNAVAILABLE (no CPI)`; startup log says UNAVAILABLE | `tests/taxes/test_cpi_unavailable.py` | source contract |
| F12 | high | Synthetic Almanac rows | fixed | arrays and `DrawTable` removed; `DrawNoData` prints `NO DATA: <service> unavailable`; default notice changed | `tests/almanac_overlay_dev/test_no_synthetic_data.py` | ImGui drawing needs external checkout |
| F13 | medium | USD CPI rent reused the RUB constant | fixed | `cpi_rent.h` `ChooseCpiRent` selects `default_daily_rent_usd`; both ship as 0.33; a missing key answers 0.0f (UNAVAILABLE) | `tests/resource_pricing/test_no_fallbacks.py::test_usd_rent_uses_its_own_ini_key` | choice helper + INI, not a live basket |
| F14 | medium | v1 prefix can be misread as free | fixed | v1 is no longer published; a pre-step-2 consumer now fails to resolve and degrades like a missing service | same suite, source assertion | mixed-install behaviour not executed |
| F15 | low | construction log spam and misleading hint | fixed | native refusals go through `NoteUnpricedMaterial` (once per resource); `[WAITING]` says the estimate is refused and the panel cannot price an unpriced material | `tests/private_construction/test_unpriced_estimate.py` | source + extracted helper |
| F16 | low | unpriced-sale row not emphasised | fixed | `TabRow::warn`; count > 0 draws label and value in `kWarn` | `tests/almanac_overlay_dev/test_no_synthetic_data.py` | drawing test needs ImGui |
| F17 | low | B2B detail logged once per session | fixed | `FirstB2bUnpricedDetail` latches per resource; daily `unpriced=` still counts every refusal | source review | no live delivery run |

All twelve original rows and the five review follow-ups (F13–F17) are resolved;
the `Initial status` column records the state before this change.

## Native claim evidence

No new native claim. The offsets and fields used (`+0x58`/`+0x5C`, record names,
basket quantities) are unchanged from step 1 and the economy direction.

## Build/test evidence

| Check | Command / cwd | Exit | Output artifact | Coverage / limits |
|---|---|---|---|---|
| Host test suite | `for f in $(find tests -name 'test_*.py'); do PYTHONPATH=. python3 $f; done` / repo root | 0 | terminal | Linux host only; no MSVC/Windows build here |
| Price contract | `python3 tests/resource_pricing/test_no_fallbacks.py` | 0 | terminal | compiles API header with g++ |
| D32 gate | `python3 tests/citizen_retail/test_unpriced_sale.py` | 0 | terminal | compiles the production policy header |
| D33 estimate | `python3 tests/private_construction/test_unpriced_estimate.py` | 0 | terminal | executes extracted `PriceOne` |
| D34 bank | `python3 tests/republic_bank/test_credit_without_cpi.py` | 0 | terminal | executes extracted `CurrentEconomicScale` |
| D35 rent choice | `python3 tests/resource_pricing/test_no_fallbacks.py` | 0 | terminal | compiles and runs `ChooseCpiRent`; USD key missing answers UNAVAILABLE |
| Almanac D8 | `python3 tests/almanac_overlay_dev/test_no_synthetic_data.py` | 0 | terminal | source contract only |
| Enterprise bread-day | `python3 tests/enterprise_base/test_unpriced_bread.py` | 0 | terminal | source contract only |
| Taxes CPI readout | `python3 tests/taxes/test_cpi_unavailable.py` | 0 | terminal | source contract only |
| Bank ledger arithmetic | `g++ ... tests/republic_bank/test_ledger.cpp` | 0 | terminal | `BankEconomicScale` now answers 0, no fallback |
| Windows build | CI run 45078257208 (PR #101) | 0 | GitHub Actions `windows-plugins` artifact | MSVC + SEH job built every plugin and `wrsre.dll`; not run locally |
| In-game check | not run | — | — | requires the owner's Windows game |

## Review rounds

| Round / gate | Diff/artifact hash | Focus 1 report | Focus 2 report | Repairs / new evidence |
|---|---|---|---|---|
| self-review | working tree | not available | not available | F1–F12 fixed in this change |

## Separate validation statuses

- **STATIC REVIEW:** SELF-REVIEW ONLY — both independent read-only reviewers are
  unavailable in this session. Do not treat this as the AGENTS.md approval gate.
- **BUILD/TEST EVIDENCE:** PARTIAL — all Python/host tests pass; the Windows
  MSVC build is not run here.
- **RUNTIME VALIDATION:** NOT RUN — no `SOVIET64.exe` launch or game-memory
  inspection in this task.

## Blocker / resume checkpoint

- **Current gate and next action:** owner in-game check of the acceptance list
  in roadmap step 2, using the CI `windows-plugins` artifact or
  `build.bat clean wrsre`.
- **Owned files / resource lock:** one writer (this task); no Ghidra operator.
- **Unresolved finding IDs:** none in the ledger; F1–F12 fixed. Independent
  review and runtime validation remain open gates, not findings.
- **Missing artifact/capability/authorization:** Windows MSVC build, an ImGui
  checkout for the drawing test, a game install and the owner's save.
- **Strategy change tried:** extracted the portable parts of the checkout and
  bank decisions into headers so the host tests execute production logic rather
  than matching strings only.

## Final delivery

- **Changed files:** see Task-owned files. No branch/commit operation by the
  agent.
- **Acceptance results:**
  - no fallback price/default/divisor → `tests/resource_pricing`
  - unpriced sale is free and counted → `tests/citizen_retail`
  - unpriced estimate refused whole → `tests/private_construction`
  - no CPI, no credit → `tests/republic_bank`
  - separate RUB/USD rent defaults, no divisor → `tests/resource_pricing`
  - Almanac has no sample rows → `tests/almanac_overlay_dev`
  - enterprise bread-day and taxes readout → their source tests
- **Ledger reconciliation:** all twelve imported findings fixed; review and
  runtime gates recorded separately.
- **Checks actually run:** the full `find tests -name 'test_*.py'` loop (exit 0),
  the new tests individually, and CI run 45078257208 (host C++, docs hygiene and
  the Windows MSVC + SEH build all green).
- **Independent coverage:** none available in this session.
- **Remaining unknowns:** Windows compilation, in-game appearance of the
  `UNPRICED`/`NO DATA` messages, and whether any out-of-tree consumer still
  links resource_pricing v5 (it must now fail to resolve and degrade).
