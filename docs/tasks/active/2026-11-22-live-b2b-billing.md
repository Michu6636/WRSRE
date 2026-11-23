# Task: live B2B billing (roadmap step 3)

## Contract

- **Status:** active — implementation and host tests complete; Windows CI, the
  owner's in-game check and the `state_ledger` reconciliation are outstanding.
- **Task record:** `docs/tasks/active/2026-11-22-live-b2b-billing.md`
- **Baseline:** branch `arena/5a89acc8-wrsre`, from `main` at `823df769`
  (PR #101 merged). Working tree contains only this task's changes.
- **Goal:** private goods are paid for by the buyer on delivery at the native
  price, and the shipped defaults are live (`billing = 1`, `dry_run = 0`)
  without any path settling the same delivery twice (D16, D19, D4).
- **Deliverables:**
  - `private_shops` 3.8: no 0.70 `wholesale_ratio` delivery bill, no foreign
    restocking, no direct `TSM_SLOT_CONTENT` writes. `service_fees` remain.
    The dead slot-walk, slot-identity snapshot, storage probe and
    `utility_filter.h` helper are removed with their only consumers.
  - `private_logistics` 0.19.1: `planSettlement` takes `buyerIsCustoms` and
    marks `customsExport` with no goods leg, so a customs-house buyer never
    creates `b2b_state_buy`. The caller identifies type 20 through
    `TsmBuildingType` and logs the export outcome.
  - `private_logistics.ini` and `g_billing` / `g_dryRun` defaults are
    `1` / `0`.
  - Documentation: design doc §12, `DO_OFFICES.md`, roadmap §1/§3, economy
    direction §4.1/§4.2/§4.4/§11, `SUPERSEDED.md`, task index.
  - Host tests: `tests/private_shops/test_no_delivery_billing.py`, updated
    `tests/private_logistics/test_b2b_settlement.py` (C++ scenario 12),
    `test_permission_test_defaults.py` and version assertions.
- **Acceptance criteria:**
  - One delivery = one bill: `private_shops` contains no wholesale or foreign
    charge path and its INI no longer defines those keys.
  - A private seller's delivery to a customs house creates no transfer and no
    state delta; a non-customs state buyer still does.
  - `configparser` reads `billing + dry_run = 1 + 0` from the shipped INI, and
    the missing-key fallbacks agree.
  - All host tests pass from the repository root.
  - **Owner in game:** factory → shop delivery produces one `[B2B GOODS]`
    payment and no `private_shops` delivery charge.
  - **Owner in game:** one live month closes in `state_ledger` with zero
    unexplained delta (I5).
- **Non-goals:** company export revenue and import cost (D5) until Ghidra G1;
  `usd_divisor` / `*_usd` re-basing (step 7); service-fee pricing changes;
  wage, shop-price or construction-office work (steps 4–6).
- **Authorized:** inspect repo files; small verified edits; host tests;
  documentation updates. No Windows build or game launch available here.
- **Forbidden:** launching `SOVIET64.exe`, deploying DLLs, writing game
  memory, changing Kilo settings/models, creating/switching branches,
  destructive Git.
- **Ghidra owner:** none needed; D5 attribution is explicitly deferred to G1.
- **Task-owned files:**
  - `plugins/private_shops/private_shops.cpp`, `private_shops.ini`
  - `plugins/private_logistics/b2b_settlement.h`,
    `private_logistics.cpp`, `private_logistics.ini`,
    `panel_fuel_runtime.h`
  - `src/enterprise_base_api.h`,
    `plugins/enterprise_base/enterprise_base.cpp` (deprecation comments only)
  - `tests/private_shops/*`, `tests/private_logistics/test_b2b_settlement.py`,
    `tests/private_logistics/b2b_settlement_test.cpp`,
    `tests/private_logistics/test_permission_test_defaults.py`,
    `tests/private_logistics/test_panel_fuel_observer.py`,
    `tests/private_logistics/test_schedule_observer.py`
  - `docs/WRSRE DESIGN DOC.md`,
    `docs/private-logistics-runtime/DO_OFFICES.md`,
    `docs/ROADMAP.md`, `docs/visions/economy-direction-2026-11-22.md`,
    `docs/SUPERSEDED.md`, `docs/tasks/README.md`, this record
- **Starting evidence:** PR #100/#101, the economy direction §4, and the
  step-2 reconciliation of `private_shops` wholesale/restock source.

## Prior findings imported

| Finding ID | Source path/line/address | Exact statement | Initial status |
|---|---|---|---|
| S1 | `private_shops.cpp` `RunWholesaleBilling`, `WholesaleSlot` | Every stock increase in a non-export storage was billed at 0.70, independently of the verified B2B transfer. | open |
| S2 | `private_shops.cpp` `RestockSlot` | Foreign restock wrote `TSM_SLOT_CONTENT` directly, creating stock and teleporting goods. | open |
| S3 | `b2b_settlement.h` `planSettlement` | A private seller delivering to a customs house produced a `b2b_state_buy` state leg. | open |
| S4 | `private_logistics.ini` 17–19; `g_billing`/`g_dryRun` | The shipped profile was observation-only (`billing = 0`, `dry_run = 1`). | open |
| S5 | design doc §12, `DO_OFFICES.md` §1/§9/§10 | The current docs still described payments as OFF after the flip. | open |
| S6 | `tests/private_shops/test_wholesale_debt.py`, `test_restock_defaults.py` | Tests required the removed billing/restock code to exist. | open |
| S7 | `plugins/private_shops/utility_filter.h` | The utility filter had no consumer after foreign restock was removed. | open |

## Gate status

| Gate | Frozen artifact/revision | Focus 1 | Focus 2 | Open IDs / blocker |
|---|---|---|---|---|
| INVESTIGATION | `main` @ `823df769` | complete | complete | S1–S7 |
| PLAN | this record | complete | complete | none |
| IMPLEMENTATION | branch diff | pending owner review | pending owner review | Windows CI + in-game check |

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| S1 | high | 0.70 delivery bill duplicated B2B settlement. | fixed | `RunWholesaleBilling`, `WholesaleSlot`, `g_wholesaleRatio` and the INI key deleted. | `tests/private_shops/test_no_delivery_billing.py` | Host/source only |
| S2 | high | Foreign restock created stock by writing slot content. | fixed | `RestockSlot`, `RunForeignRestock`, the foreign settings and their INI keys deleted. | same | Host/source only |
| S3 | high | Customs buyer produced `b2b_state_buy`. | fixed | `buyerIsCustoms` → `customsExport`, early return before any leg; type 20 detected at the caller. | C++ scenario 12 in `b2b_settlement_test.cpp` | Native type-20 identity is already the verified world probe; no new Ghidra claim |
| S4 | high | Defaults did not move money. | fixed | INI and initialisers are `billing = 1`, `dry_run = 0`; profile log states the shipped profile. | `test_live_billing_is_the_shipped_default`, `test_permission_test_defaults` | Runtime not executed here |
| S5 | medium | Docs contradicted the shipped defaults. | fixed | Design doc §12 Rev 6, `DO_OFFICES.md` §1/§5/§6/§9/§10, roadmap and economy direction updated. | `test_documentation.py` sweep | — |
| S6 | medium | Removed behaviour was still asserted by tests. | fixed | Old tests replaced by a removal contract; B2B test extended. | full host sweep | — |
| S7 | low | Dead utility filter remained. | fixed | Header and its test deleted with the restock path. | full host sweep | — |

## Native claim evidence

No new native claim is made. Customs type 20 and the loading-seller identity
already come from the verified world/enterprise probes recorded in the
existing private-logistics evidence. The only new branch is a policy decision
(`buyerIsCustoms`), not a memory offset.

## Build/test evidence

| Check | Command / cwd | Exit | Output artifact | Coverage / limits |
|---|---|---|---|---|
| Private-shops contract | `python3 -m unittest tests.private_shops.test_no_delivery_billing -v` (repo root) | 0 | this record | Source/INI only |
| B2B runtime | `python3 -m unittest tests.private_logistics.test_b2b_settlement -v` (repo root) | 0 | this record | Stubbed callbacks, not Windows |
| Permission defaults | `python3 -m unittest tests.private_logistics.test_permission_test_defaults -v` (repo root) | 0 | this record | INI/source only |
| Full host sweep | `python3` driver over every `tests/**/test_*.py` with `PYTHONPATH=.` (repo root) | 0 | 39 files, 0 failures | Host tests only |
| Almanac host runner | `python3 tests/almanac_overlay_dev/run_host_tests.py` (repo root) | 0 | `build/almanac_overlay_dev/normal/summary.json` | Host tests only |

## Review rounds

| Round / gate | Diff/artifact hash | Focus 1 report | Focus 2 report | Repairs / new evidence |
|---|---|---|---|---|
| Implementation | branch diff | not requested yet | not requested yet | S1–S7 fixed |

## Separate validation statuses

- **STATIC REVIEW:** pending owner/reviewer — no independent review requested.
- **BUILD/TEST EVIDENCE:** PARTIAL — host tests pass; Windows CI has not run
  for this branch yet.
- **RUNTIME VALIDATION:** NOT RUN — no live game or deployment in this
  environment.

## Blocker / resume checkpoint

- **Current gate and next action:** push the branch, wait for Windows CI, then
  report to the owner for the in-game factory → shop and `state_ledger` check.
- **Owned files / resource lock:** this branch; no Ghidra owner needed.
- **Unresolved finding IDs:** none.
- **Missing artifact/capability/authorization:** Windows build/game run.
- **Strategy change tried:** none.

## Final delivery

- **Changed files:** see the branch diff; the owned paths above.
- **Acceptance results:** one-delivery/one-bill and customs exclusion are
  proven by host tests; the two live checks are pending.
- **Ledger reconciliation:** S1–S7 all fixed, no open finding.
- **Checks actually run:** the three targeted suites and the full host sweep;
  the Windows CI result is pending.
- **Independent coverage:** none yet; owner review requested through the PR.
- **Remaining unknowns:** native in-game settlement and `state_ledger`
  divergence; exact fuel burn and assignment coverage remain as documented.
