# Task: roadmap step 3b — state-paid private DO trip fuel

## Contract

- **Status:** merged in PR #105 (supersedes #104) on the owner's order;
  in-game check pending — see "Test fix and merge (2026-11-28)" at the end
- **Task record:** `docs/tasks/active/2026-11-27-step-3b-state-do-fuel.md`
- **Baseline:** `main` at `be946b96aafadcebe72581c4aa20887faa930fb4`, clean
  before this task; working branch `step-3b-state-do-fuel`.
- **Goal:** settle eligible private DO trip fuel to the actual goods receiver,
  correct a per-vehicle cargo-state eviction defect, make billing diagnostics
  truthful, and expose missing goods-cargo attribution.
- **Deliverables:** receiver-based payer policy; fix for unsafe cargo-slot
  selection; capped missing-cargo errors and daily counter; accurate vehicle
  leg and billing diagnostics; host tests; version/documentation updates; a
  pull request, not merged until the owner orders it.
- **Acceptance criteria:**
  - Host-tested payer policy: private receiver pays; state receiver pays from
    treasury with `b2b_state_do_fuel`; customs receiver is deferred; unknown
    direction is unpaid; a state seller with a private receiver is charged to
    that private receiver, never the treasury.
  - The receiver is determined from committed, adjacent `0x68F792` →
    `0x68F7F9` transfer pairs for the same truck and home DO while its observed
    assignment is open; never use `originalTarget` to choose a payer. Zero or
    multiple distinct receivers, failed pair verification, or overflow results
    in `reason=direction_unknown`.
  - Cargo tracking uses the documented native vehicle global index at `+0x04`;
    unavailable/out-of-range indices fail closed without hashing into a
    collision-prone vehicle slot. Pointer identity still guards slot reuse.
  - A matched building-side delivery with `cargoFound == false` increments
    `no_cargo` and emits the requested `[B2B GOODS] ERROR NO CARGO RECORD`
    subject to the existing daily payment-log cap.
  - A `0x68F7F9` vehicle-side leg prints as a vehicle leg and never claims
    `UNPRICED` or `bill refused`.
  - Billing status is `live`, `dry_run`, or `off` from `g_billing` and
    `g_dryRun`; non-settling diagnostics say `money_moved=0`.
  - Live billing emits one `[DRY COST] inactive: billing live` line and does
    not run or persist the dry-cost ledger.
  - Day-close output exposes `do_fuel_private`, `do_fuel_state`, and
    `direction_unknown`; `[DO TRIP]` names receiver, payer, and reason.
  - Every `tests/**/test_*.py` suite runs with `PYTHONPATH=.` and passes, C++
    host tests pass, and the Windows CI build passes.
  - Documentation and version are updated. No wording claims this step was
    tested in game. The owner's requested in-game checklist is in the PR body.
- **Non-goals:** customs/export/import billing and private foreign trade (D41,
  later step); exact fuel burn; live game validation; merging the PR.
- **Authorized:** inspect and edit repository files; host tests; Windows build
  via CI; create this task branch and publish a PR as explicitly requested.
- **Forbidden:** launching `SOVIET64.exe`, deploying DLLs, writing game memory,
  merging, destructive Git, or changing Kilo settings/models.
- **Ghidra owner:** none; the owner supplied a same-build runtime log for the
  receiver question, so no new Ghidra query is required for this step.
- **Task-owned files:** `plugins/private_logistics/*` files directly related to
  trip settlement/diagnostics; focused tests under
  `tests/private_logistics/`; `.github/workflows/ci.yml`; `docs/ROADMAP.md`,
  `docs/visions/economy-direction-2026-11-22.md`,
  `docs/private-logistics-runtime/DO_OFFICES.md`, this task record, and
  `docs/private-logistics-runtime/evidence/STEP3B_OWNER_RUN_2026-11-27.md`
  plus its tracked verifier.
- **Starting evidence:** D40/D41 in economy direction §1; roadmap step 3b;
  verified owner-run transfer-pair analysis; existing B2B matcher, cargo-slot
  index and assignment observer.

## Frozen implementation snapshot

- **Base:** `be946b96aafadcebe72581c4aa20887faa930fb4`.
- **Implementation commit:** `4dc25fdbdbb514483bbfb26955f63928bbaf1538`.
- **Commit-relative changed-file/line evidence:** [`STEP3B_DIFF_MANIFEST_BE946B96_4DC25FD.txt`](../../private-logistics-runtime/evidence/STEP3B_DIFF_MANIFEST_BE946B96_4DC25FD.txt) lists all 32 changed paths and every zero-context hunk range from `git diff --unified=0` for this base and commit; it records 3,793 insertions and 238 deletions.
- **Snapshot boundary:** the manifest freezes the complete implementation diff through `4dc25fd`. Later review-status or publication metadata is intentionally outside that snapshot; the exact full diff remains reproducible from the two recorded commits.
- **Code landmarks:** receiver/payer policy in [`step3b_policy.h`](../../../plugins/private_logistics/step3b_policy.h) and [`do_fuel_settlement.h`](../../../plugins/private_logistics/do_fuel_settlement.h); assignment evidence and transfer lifecycle in [`dry_assignment_runtime.h`](../../../plugins/private_logistics/dry_assignment_runtime.h); cargo indexing and matching in [`b2b_settlement.h`](../../../plugins/private_logistics/b2b_settlement.h); native detour, diagnostics and day-close integration in [`private_logistics.cpp`](../../../plugins/private_logistics/private_logistics.cpp).

## Prior findings imported

| Finding ID | Source path/line/address | Exact statement | Initial status |
|---|---|---|---|
| 3B-1 | First step-3 in-game run, recorded in roadmap §3 | 22 private DO trips to state buildings were unpaid (`private_parties_required`, 41.33 RUB). | open |
| 3B-2 | `dry_assignment_runtime.h`, `AssignmentSettlementEvent` | `originalTarget` is used as the buyer even when that building was the loading site; the actual receiver must come from matched vehicle delivery events. | open — owner-run pair evidence verified for 661 deliveries |
| 3B-3 | Fleet, panel, dry-cost and journey diagnostics | Hardcoded `billing=off` is misleading while live billing is active; these observers never move money. | open |
| 3B-4 | `dry_assignment_runtime.h`, `DryCostContext` / persistence | Dry-cost collection requires `!g_billing`, but start/persistence diagnostics still run while billing is live. | open |
| 3B-5 | Roadmap step 3b / D40 | Day-close output does not distinguish private-paid fuel, treasury-paid fuel, or unknown direction. | open |
| 3B-6 | D39/D41 and roadmap step 3b | Customs receiver fuel is deferred; exports and foreign trade are out of scope. | open |
| 3B-7 | `dry_assignment_runtime.h`, `TransferFunds` | `TransferFunds` returns success in dry-run without moving money, but the trip path reports that amount as `paid`. | open |
| 3B-8 | `b2b_settlement.h`, `TransferFunds` and currency callbacks | Treasury transfers are not read back after mutation and can report success if the move callback is absent or ineffective. | open |

## Gate status

| Gate | Frozen artifact/revision | Focus 1 | Focus 2 | Open IDs / blocker |
|---|---|---|---|---|
| INVESTIGATION | Owner-run evidence report, strict `do_receiver_check.py`, and this record | approved | approved | Supported for the observed run; historical sequence and build-identity limits retained |
| PLAN | Current plan revision 8 in this task record | approved | approved | Both independent reviewers approved revision 8 |
| IMPLEMENTATION | Commit `4dc25fdbdbb514483bbfb26955f63928bbaf1538` and changed-file/line manifest above | approved; no implementation findings | approved; no findings | Code and focused tests reviewed; Linux C++ coverage and PR CI pending |

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| 3B-1 | high | State-receiver DO fuel refused by `private_parties_required`. | implemented; CI pending | Receiver-based payer selection and verified treasury transfer use `b2b_state_do_fuel`; failed balance/read-back paths skip or roll back. | `ScenarioStep3bPayerPolicy`, treasury read-back/rollback and live trip scenarios. | Actual fuel units and coverage remain provisional; native transfer effectiveness needs CI/game evidence. |
| 3B-2 | high | `originalTarget` can be a loading site and is not the payer. | implemented; CI pending | Receiver is set only by committed consecutive mode-0/mode-1 evidence, same vehicle/home and matching destination; never reads `originalTarget` for payer choice. | Tests cover sequence gaps, signs/modes/sites/owner, ambiguity, zero/multiple receivers, identity changes, unfinished receiver, ring/token overflow, save-load/stale close and generation exhaustion. | One pre-change run does not prove all builds/routes; no post-change in-game validation is claimed. |
| 3B-3 | medium | Observer/diagnostic hardcodes say `billing=off`. | implemented; CI pending | Non-settling diagnostics use `money_moved=0`; startup and day-close use the billing-state helper. | Billing policy tests plus source/output assertions in private-logistics wrappers. | Host/source checks do not prove live game output. |
| 3B-4 | medium | Dry-cost lifecycle emits records/logs despite the billing gate. | implemented; CI pending | Live billing gates dry-cost sampling and emits the inactive notice once. | `step3b_live_billing_disables_dry_cost_startup` and wrapper checks. | Persistence and game lifecycle remain host-tested only. |
| 3B-5 | medium | Day-close summaries omit payer-class totals and unknown direction. | implemented; CI pending | Day-close output includes private/state DO fuel and direction-unknown totals; trips name receiver, payer and reason. | Source/output assertions plus dry-ledger trip scenarios. | No runtime ledger reconciliation is claimed. |
| 3B-6 | low | Customs must remain deferred under D41. | implemented; CI pending | Customs payer policy returns no payer and `customs_export_deferred`; customs/foreign-trade goods behavior remains deferred. | `ScenarioStep3bCustomsAndForeignTradeDeferred` and named Python marker assertion. | Foreign-trade branch remains separate. |
| 3B-7 | high | Dry-run transfer success is logged as money paid. | implemented; CI pending | Dry/off paths do not call transfer callbacks; `paid` and payer totals remain zero while `would_pay` may be positive. | `ScenarioStep3bDryRunNeverPaid`; both modes asserted. | Live transfer success requires verified payer/payee mutation. |
| 3B-8 | high | Treasury mutation is not verified by a balance read-back. | implemented; CI pending | DO-fuel transfer requires callbacks and finite reads; debit/credit read-back and observed-balance rollback are checked. | Treasury insufficiency, missing callback, no-op/partial debit/credit and identity/generation failures are asserted. | Host callbacks cannot prove native currency API effectiveness in game. |
| 3B-9 | high | Matched deliveries without cargo attribution are silently skipped; vehicle cargo state is stored under an unverified `vehicle+0x10`/hash index and is cleared on pointer mismatch. | implemented; CI pending | Use documented `vehicle+0x04`, fail closed on invalid index, preserve pointer guard, emit capped missing-cargo diagnostics and count `no_cargo`. | Tests cover distinct/reused/invalid indices, `resetAll`, absent destination, count/log cap and exact diagnostic fields. | Old log does not record slot index or owner-change resets, so not every runtime loss is independently attributed. |
| 3B-10 | medium | Delivery vehicle leg is mislabeled `load @UNPRICED ... bill refused`. | implemented; CI pending | The verified vehicle leg is logged as a vehicle leg without pricing/refusal language. | Python output assertion and receiver vehicle-leg predicate tests. | No runtime retest after change. |

## Superseded plan proposal (revision 2)

Both focus reviewers approved the revision-2 payer, logging, and test plan
before the road receiver evidence gap below was identified. It is not safe to
implement as written: its proposed `0x68F792`/`0x68F7F9` event pair is the
sea/rail family, not a verified road DO unload pair. The active-target plus
route-direction alternative was also independently assessed and found
insufficient to prove completed receipt. This plan is retained as history only;
it is not an implementation authorization or an approved current plan.

- **Receiver correlation:** when `matchMode1Delivery` returns a destination,
  pass that unload event's actual mode-1 vehicle and destination identity to
  the active assignment token for that same vehicle and home DO. The mode-0
  destination call itself has no vehicle (`private_logistics.cpp` records
  `p2` as the building and `p9` as the pending fleet owner). In addition to
  the existing resource/tonnage/time match, receiver evidence is verified only
  for the known `0x68F792` building-side / `0x68F7F9` vehicle-side pair when
  both callbacks are consecutive in the plugin's hooked-transfer sequence,
  the pending owner equals the mode-1 vehicle's current home, the vehicle cargo
  join succeeds, and exactly one pending destination matches. A non-adjacent
  or other-site match remains usable by existing goods settlement but is not
  DO receiver evidence. Keep existing goods-settlement selection unchanged,
  but expose a separate `receiverVerified` result for DO settlement. Increment
  the sequence under `g_lock` for every transfer detour, retain it and site RVA
  on pending deliveries, and reset it with B2B engine state on save load.
- **Assignment interval:** associate unload events only with the token matching
  the same vehicle, home DO, epoch and assignment ID while it is open. Do not
  wait after close: a close with no verified unload is `direction_unknown`;
  a late unload cannot repair or charge it. Never overwrite the fixed token
  table; token exhaustion is fail-closed. Multiple unloads to the same
  identity-stable receiver remain attributable; a changed/invalid receiver or
  any unverified event permanently makes that assignment's direction unknown.
  The existing 32-entry pending-delivery ring must expose overwritten pending
  entries; any overwritten record or ambiguous match is unverified. Close
  clears the token immediately, so no post-close callback can charge it. Do
  not wait for or reuse a late unload event.
- **Eligibility:** `Assess` no longer requires `originalTarget` or
  `originalTargetRows`; it still requires a valid DO home, complete assignment
  evidence, observed positive fuel decrease, and available price. Payer
  selection uses only the verified receiver from the active token.
- **Matcher ambiguity:** keep the existing goods matcher selection behavior,
  but expose whether more than one pending delivery matched its resource,
  tonnage and time criteria (`deliveryAmbiguous`). Such a result is unusable as
  DO receiver evidence. This avoids changing the current goods payer outcome
  while preventing the trip path from treating the matcher's newest-candidate
  choice as certain.
- **Payer decision:** extract a pure policy: private receiver → receiver; state
  non-customs receiver → treasury; customs → `customs_export_deferred`;
  unresolved receiver → `direction_unknown`; non-private DO → no tracked
  settlement. A state seller with a private receiver still selects that private
  receiver as payer.
- **Treasury execution:** preserve `TransferFunds`' existing treasury
  affordability / `treasury_skips` handling in a DO-fuel-specific transfer
  routine; do not broaden generic goods-settlement behavior. Require
  `getTreasury(userData, 0)`, `moveTreasury`, `getCapital`, `modifyCapital`,
  and `isLivePrivate` before any mutation or payee credit. Reject non-finite
  treasury and DO-capital reads, precheck the treasury balance, debit, and
  require a finite read-back with `(before - after)` within `0.05` of the
  requested amount before crediting. If debit read-back mismatches, attempt to
  restore the observed balance to `before` (not blindly add the full quote),
  then verify the restored balance within `0.05`; if DO credit fails after a
  verified debit, restore any observed partial DO credit back to its pre-credit
  balance and verify it, restore the treasury by the quoted amount and verify
  against `before`. Never count either path as paid. Unavailable or
  insufficient treasury increments the existing `treasury_skips`; log the
  precise `treasury_unavailable` or `treasury_insufficient` reason.
  Ineffective/partial mutation, failed DO credit, or either unverifiable
  rollback produces an explicit `[DO TRIP] ERROR` with observed deltas and
  rollback verification states, and never success. A missing required callback
  fails closed before mutation. Keep the existing private-receiver payer
  transfer behavior, but require verified live success before counting it as
  paid. Do not report a dry-run plan as paid. Add focused host cases for
  insufficient treasury, missing callbacks, ineffective/partial debit,
  DO-credit failure, partial DO-credit rollback, successful and ineffective
  treasury/DO rollback, and dry-run.
- **Money reporting:** keep `would_pay` as the eligible quote in dry-run/off
  modes, but set `paid` and payer-class day totals only after live transfer
  succeeds with `cb.dryRun == false`.
- **Diagnostics:** observers and non-settling cost/journey records use
  `money_moved=0`; centralize the three billing state names. Gate the dry-cost
  ledger and its persistence registration off when `g_billing` is enabled,
  while retaining a distinct assignment-settlement callback. Billing
  precedence is `off` if `g_billing == 0`, otherwise `dry_run` if
  `g_dryRun != 0`, otherwise `live`. Emit the live `[DRY COST] inactive` line
  once at startup and no dry-cost persistence callbacks while billing is on.
- **Exact output contract:** plugin-start and `[B2B DAY]`/`[B2B BAL]`/`[DO
  BAL]` lines use `billing=<live|dry_run|off>`. `[B2B DAY]` and each `[DO BAL]`
  include `do_fuel_private` and `do_fuel_state` paid totals plus
  `direction_unknown` count; customs has separate
  `customs_export_deferred` count and is never included in unknown. `[DO TRIP]`
  includes `receiver`, `payer`, `reason`, `would_pay`, `paid`, and billing
  state. Dry/off states show `paid=0` and `money_moved=0`; live paid totals
  rise only after verified transfer success.
- **Test contract and runnable mapping:**
  - Add named C++ host scenario functions `ScenarioStep3bPayerPolicy`,
    `ScenarioStep3bReceiverEvidenceFailsClosed`,
    `ScenarioStep3bTreasuryReadbackAndRollback`,
    `ScenarioStep3bDryRunNeverPaid`, and
    `ScenarioStep3bCustomsAndForeignTradeDeferred` to
    `tests/private_logistics/b2b_settlement_test.cpp`; invoke each from `main`
    and print a distinct `PASS <scenario-name>` line. The Python wrapper
    `tests/private_logistics/test_b2b_settlement.py::test_step3b_payer_policy_and_customs_deferred`
    covers private receiver, state receiver/treasury with exact
    `b2b_state_do_fuel` label, state seller → private receiver, customs
    deferral, unknown direction, and that state seller never selects treasury.
  - `ScenarioStep3bReceiverEvidenceFailsClosed` covers mode-0/mode-1 owner
    mismatch, duplicate candidate ambiguity, non-adjacent sequence, overwritten
    32-entry pending ring, changed receiver, late unload after close, crossed
    vehicle/home/epoch/assignment token, and exhaustion of all 64 assignment
    tokens. Each failure leaves payer null and reports `direction_unknown`;
    customs remains `customs_export_deferred`.
  - `ScenarioStep3bTreasuryReadbackAndRollback` covers
    `treasury_skips` on insufficient funds, required-callback absence before
    mutation, non-finite before/after reads, no-op/partial debit, successful
    debit and read-back, DO-credit failure/partial credit, successful DO and
    treasury restoration, and unverified rollback of either balance. Failed
    paths never increment `paid`.
  - `ScenarioStep3bDryRunNeverPaid` covers dry-run and off states:
    `would_pay` may be positive, `paid=0`, no callbacks mutate balances, and
    payer-class totals stay zero. Live state sets `paid` only on verified
    transfer success.
  - `tests/private_logistics/test_dry_assignment_ledger.py::test_step3b_trip_and_day_close_output`
    asserts exact `[DO TRIP]` receiver/payer/reason/`would_pay`/`paid` fields
    and `[B2B DAY]`/`[DO BAL]` payer totals, unknown/customs counters and
    billing state.
  - `tests/private_logistics/test_fuel_diagnostics.py::test_step3b_billing_states_and_money_moved`,
    `test_schedule_assignment_ledger.py::test_step3b_schedule_billing_state`,
    `test_panel_fuel_observer.py::test_step3b_panel_billing_state`, and
    `test_journey_tracker.py::test_step3b_journey_money_moved` assert all
    `live`/`dry_run`/`off` labels and `money_moved=0` in observer/diagnostic
    output. Dry-cost live-gate behavior is asserted by
    `test_dry_assignment_ledger.py::test_step3b_live_billing_disables_dry_cost`.
  - `ScenarioStep3bCustomsAndForeignTradeDeferred` and
    `tests/private_logistics/test_b2b_settlement.py::test_step3b_customs_and_foreign_trade_remain_deferred`
    assert customs is unpaid with `customs_export_deferred`, no customs
    treasury credit occurs, and the changed settlement files introduce no
    export/import/FX/private-foreign-trade payment path (D41).
  - Each C++ scenario asserts its exact expected payer, reason, label, callback
    deltas and final balances before printing `PASS ScenarioStep3b...`.
    `tests/private_logistics/test_b2b_settlement.py::test_production_runtime`
    must assert that all five distinct scenario markers occur; missing markers
    or nonzero executable exit fail the Python test.
  - Every `tests/**/test_*.py` file must expose a working direct unittest
    entrypoint. Replace `test_scheduled_phases.py`'s hardcoded `g++` lookup
    with the repository's portable compiler selection. C++-required wrappers
    must identify unavailable compilers as blocked, never as passed coverage;
    Linux CI must fail if either required step-3b host wrapper skips or runs
    zero tests. Keep unrelated platform-specific skips (such as real Windows
    SEH on Linux) unchanged.
- **Exact files:** `b2b_settlement.h`, `assignment_settlement.h`, new pure
  payer/billing helper headers, `private_logistics.cpp`,
  `dry_assignment_runtime.h`, `fleet_fuel_runtime.h`,
  `panel_fuel_runtime.h`, `journey_tracker.h`; relevant private-logistics
  C++/Python tests; roadmap, economy direction, `DO_OFFICES.md`, this record.
- **Scope guards:** no customs payments or foreign trade; no change to goods
  settlement policy except exposing ambiguity metadata; no live game launch.
- **Validation map:** the named test methods above compile and run production
  host helpers and assert named C++ scenarios, exact payer/log fields and
  callbacks. Run each discovered Python test file independently with
  `PYTHONPATH=.`, reject zero-test execution for the required step-3b host
  wrappers, run all C++ CI host suites, then PR Windows CI. No game test or
  runtime validation claim.

## Current plan revision 8

The old revision-2 proposal above is historical. Plan revision 5 was reviewed
by both focus reviewers; revisions 6 and 7 were also reviewed by both
reviewers and their findings below must be resolved before implementation.
The current implementation plan is:

1. **Receiver proof and assignment token:** increment a monotonic transfer
   sequence under `g_lock` for every hooked detour, before forwarding, including
   uncommitted and unrelated calls. In the always-registered
   `AutoPersistenceEvent(TSM_SAVE_LOADING)` path, before cargo/pending reset,
   discard pending receiver records and in-progress pairs, advance a
   non-wrapping save generation, reset the per-generation sequence, and
   permanently invalidate every open assignment token so evidence cannot cross
   the save boundary. This must run even when live billing disables
   `DryCostPersistence`. `DryCostPersistence` must no longer clear assignment
   tokens, receipts, or the receipt cursor; it may reset only dry-cost state.
   Never clear or persistently truncate assignment receipts on save loading.
   Include the generation in every sequence/pair identity. If either sequence
   or generation is exhausted, saturate rather than wrap, mark open tokens
   direction-unknown, and disable receiver proof until plugin restart. Preserve
   sequence, site RVA, commit, resource, building, owner, tonnage and mode on
   the pending mode-0
   delivery record. Receiver proof requires the immediately following detour
   to be a committed positive mode-0 `0x68F792` / committed negative mode-1
   `0x68F7F9` pair, same resource, magnitudes within the engine's 0.5% /
   0.0005-tonne tolerance, building-side p9 equal to the mode-1 vehicle home,
   and truck pointer equal to the open assignment root and same open home/token.
   Count every matching pending destination candidate in a read-only precheck;
   this precheck must not consume candidates, pop cargo, or alter the existing
   newest-candidate goods-settlement choice. Set `deliveryAmbiguous` and reject
   receiver proof unless exactly one candidate matches. Preserve existing
   goods matcher choice and payment behavior. An intervening call invalidates
   only an in-progress pair; failed/uncommitted/mismatched eligible legs,
   ambiguous candidates, pending-delivery ring overwrite, token exhaustion,
   identity change, or multiple distinct receiver identities permanently sets
   that assignment's direction unknown. Before overwriting an active pending
   receiver record, mark every potentially affected open token
   `receiverInvalid`; if the affected token cannot be identified safely, mark
   all open tokens unknown. This invalidation is persistent through close, not
   a transient ring flag. Store exact `(save_generation, epoch, id, root,
   vehicle, home pointer + stable key)` token identity and receiver pointer +
   stable key. At close, revalidate the private DO and receiver liveness,
   identity, and `AutoBuildingFinished` (or equivalent native completion).
   A changed/demolished receiver is `direction_unknown`; an identified but
   unfinished receiver is unpaid with `reason=receiver_unfinished`. Do not bind
   late events after close. If token allocation fails, the close event still
   emits a no-pay `[DO TRIP] receiver=unknown payer=none
   reason=direction_unknown` line. Zero verified receivers is
   `direction_unknown`. Remove `originalTarget` / `originalTargetRows` from
   token creation and assignment eligibility; they are not payer evidence.
   Keep receiver proof independent of `cargoFound`, so a verified delivery can
   identify its receiver even when goods settlement has no cargo attribution.
   State-owned DOs remain untracked.
2. **Payer and verified transfer:** pure tested policy uses only the verified
   receiver: private receiver → that company; non-customs state receiver →
   treasury with payer `nullptr` and exact label `b2b_state_do_fuel`; customs →
   `customs_export_deferred`; missing/ambiguous/changed receiver →
   `direction_unknown`, no payment. A state seller with a private receiver
   selects that private receiver, never the treasury. Preserve generic goods
   transfer behavior; all DO-fuel payments (private or treasury payer) use a
   DO-fuel-specific verified live-transfer routine, not generic
   `TransferFunds` success semantics. Before mutation, require the applicable
   payer/payee read and mutation callbacks, private-party liveness, and finite
   pre-balances. Immediately before each live mutation, revalidate the actual
   goods receiver and private DO payee against their captured stable keys,
   liveness, building type/private classification and completion state. For a
   private receiver, also verify its resolved company payer is still the same
   company; treasury (`nullptr`) has no building identity to validate. If a
   receiver or payee check fails before mutation, make no balance change. If a
   receiver or payee check fails after a verified payer debit, roll the payer
   back and verify it before returning failure. Verify payer debit and DO
   credit by finite read-back within 0.05. On mismatch, restore each observed
   balance to its pre-transfer value and verify rollback; a partial/no-op
   mutation, missing/non-finite read, or unverifiable rollback emits explicit
   observed deltas and never counts as paid. After DO credit/read-back, perform
   a final receiver, payer-company and DO-payee stable-key/liveness/completion
   check before counting payment. If any final check fails, restore both
   observed balances to their pre-transfer values and verify both rollbacks.
   Under `g_lock`, verify the save generation and reserved receipt are still
   current before committing the receipt and incrementing paid totals; a
   generation change fails closed and triggers verified rollback. For a state
   receiver, payer is the treasury (`nullptr`); missing or insufficient
   treasury increments existing `treasury_skips` and reports
   `treasury_unavailable` or `treasury_insufficient`. Private-receiver
   insufficiency retains the private insolvency counter. Dry/off paths do not
   mutate balances; `would_pay` may be positive, while `paid` and payer totals
   remain zero. Add `transfer_failed` and explicit mutation/rollback error
   reasons. Count a payment only after verified live payer debit and DO credit.
3. **Cargo slot key:** read the documented global vehicle index at
   `vehicle+0x04` into an explicit `{index, valid}` result. Invalid/unreadable/
   faulted or out-of-range indices never become slot zero and never fall back
   to modulo pointer hashing. `matchMode1Load` first identifies the pending
   mode-0 load using existing matching semantics; if the index is invalid, it
   consumes/drops that matched load and returns without indexing cargo, so a
   later vehicle cannot claim it. `matchMode1Delivery` may still match the
   pending destination for receiver/error reporting, but sets
   `cargoFound=false` without touching `vehCargo_` when the index is invalid.
   Keep the pointer identity guard so native-index reuse clears stale lots.
   Keep the existing save-load `resetAll()` for cargo, pending loads and
   pending deliveries; do not persist or carry cargo across a save load.
4. **Missing cargo:** every `mdMatched && !md.cargoFound` increments `no_cargo`,
   regardless of whether a payer is determined or `destBld` is present. Emit
   `[B2B GOODS] ERROR NO CARGO RECORD buyer=... truck=... res=... tons=...`
   under the existing per-day payment-log cap (use `buyer=unknown` safely if
   destination is absent). Invalid indices never read a cargo slot. Fix only
   the documented slot-key defect and add this visibility; do not guess another
   upstream fix from the old log.
5. **Vehicle leg, billing and visibility:** print the `0x68F7F9` vehicle leg
   as `vehicle`, without `UNPRICED` or `bill refused`. Central billing-state
   precedence is `off` when `g_billing==0`, otherwise `dry_run` when
   `g_dryRun!=0`, otherwise `live`. The plugin-start line and every daily-close
   line use that state. All non-settling observers/diagnostics use
   `money_moved=0`, never hardcoded `billing=off`. When billing is enabled,
   skip dry-cost event/persistence registration but keep assignment-settlement
   callbacks separate and active; emit one `[DRY COST] inactive: billing live`
   line for live startup (and an explicit dry-run inactive line for dry-run).
   `[B2B DAY]` and `[DO BAL]` show `do_fuel_private`, `do_fuel_state`, and
   `direction_unknown`; customs deferral has its own count. `[B2B BAL]` and
   other daily close lines also show the real billing state. `[DO TRIP]` names
   `receiver`, `payer`, `reason`, `would_pay`, `paid`, and billing state; dry/off
   output has `paid=0`.
   Keep assignment receipts non-circular and keyed by
   `(save_generation, epoch, id, root)`; retain prior-generation entries across
   save loading, and allow a genuinely new assignment with the same native
   epoch/id/root only in the new generation. Under `g_lock`, atomically check
   duplicate and capacity, then reserve a receipt as `in_progress` before any
   live transfer callback or balance mutation; do not hold the lock while
   calling native currency APIs. Concurrent closes see the reservation and
   fail with `duplicate_assignment`. After the transfer, atomically mark the
   reservation `committed` only with verified balances/final identity and the
   unchanged generation, or mark it terminal `failed` on any failure. Never
   release a failed reservation for retry, and never evict a reservation or
   receipt. At the existing 256-entry capacity, fail closed with
   `reason=receipt_capacity` before mutation. Keep the receipt table and cursor
   across save loading; in-flight transfers must recheck generation before
   every mutation and final commit, rolling back verified deltas if it changes.
6. **Docs, version and tests:** bump `PL_VERSION`; update the roadmap, economy
   direction §§4.1/11, `DO_OFFICES.md`, `docs/engine/private-industry-re.md`,
   and this record. Correct the stale sea/rail-only classification, preserving
   the owner-run's one-build limits and not claiming the new code was tested in
   game. Keep customs/export/import/private foreign trade deferred (D41).
   Add and invoke named C++ host scenarios:
   `ScenarioStep3bPayerPolicy` (private/state/customs/unknown and state seller
   → private receiver), `ScenarioStep3bReceiverEvidenceFailsClosed`
   (sequence gap, noncommit, sign/resource/tonnage/owner mismatch, ambiguity,
   multiple/zero receiver, persistent ring-overwrite invalidation, token
   exhaustion, late close, identity change, unfinished receiver, save-load
   boundary and sequence/generation exhaustion), `ScenarioStep3bTreasuryReadbackAndRollback` (insufficient,
   missing callbacks, non-finite reads, no-op/partial debit, payee failure,
   partial credit, verified and failed rollbacks, receiver/company/payee
   identity change before and after payer debit and after DO credit, generation
   change before final commit),
   `ScenarioStep3bDryRunNeverPaid`,
   `ScenarioStep3bCustomsAndForeignTradeDeferred`,
   `ScenarioStep3bReceiptCapacityFailsClosed` (atomic concurrent reservation,
   in-progress duplicate suppression, terminal failed reservations,
   receipt/cursor retention across save loading, old close without a current
   token, same native identity in a new generation, genuinely new post-load
   assignment, and 256-entry capacity with no eviction), and
   `ScenarioStep3bCargoIndexAndMissingRecord` (distinct indices, pointer reuse,
   invalid/out-of-range index without cargo access or cross-vehicle claim,
   save-load reset, no-cargo counter and capped output). Assert exact
   vehicle-leg and billing-state output. Update old
   expectations in `dry_assignment_runtime_test.cpp`,
   `endpoint_runtime_test.cpp`, `fleet_approximation_cases.h`,
   `journey_tracker_test.cpp`, and schedule/panel Python tests. Python wrappers
   must assert distinct C++ scenario markers and exact requested output; a
   required wrapper that skips or runs zero tests is not coverage. The primary
   C++ wrappers include `tests/private_logistics/test_b2b_settlement.py`,
   `B2bSettlementTest.test_production_runtime` (requires
   `ALL B2B SCENARIOS PASS`); `tests/private_logistics/test_dry_assignment_ledger.py`,
   `DryAssignmentLedgerTest.test_production_runtime` (requires the dry-ledger
   and fleet scenario markers); `test_fuel_diagnostics.py`,
   `FuelDiagnosticsTest.test_candidate_provenance_core` and
   `test_actual_readtank_and_slot_reader_with_host_memory`;
   `test_journey_tracker.py`,
   `JourneyTrackerTest.test_windows_min_max_macro_compatibility`;
   `test_fleet_fuel_observer.py`,
   `FleetFuelObserverTest.test_production_reader_and_runtime`;
   `test_panel_fuel_observer.py`,
   `PanelFuelObserverTest.test_production_runtime_with_host_shim` and
   `test_endpoint_runtime_with_real_pricing_contract`;
   `test_schedule_assignment_ledger.py`,
   `AssignmentLedgerTest.test_runtime_and_money_separation` and
   `test_windows_macro_compatibility`; and `test_schedule_observer.py`,
   `ScheduleObserverTest.test_production_runtime_enabled` and
   `test_windows_macro_compatibility`. Make
   `tests/sim_clock/test_scheduled_phases.py`,
   `SimClockPhaseTests.test_phase_ordering_and_structsize_fallback`, select
   g++ or clang++ and skip clearly only when neither exists.

   Run every discovered `tests/**/test_*.py` directly from the repository root
   using `.github/workflows/ci.yml`'s Linux test loop:
   `find tests -name 'test_*.py' | sort`, then `PYTHONPATH=. python3 "$f" -v`
   for each result. Update that workflow to require a `Ran [1-9][0-9]* test`
   unittest summary and no skip for these compiler-backed wrappers:
   `tests/private_logistics/test_b2b_settlement.py`,
   `test_dry_assignment_ledger.py`, `test_census_capacity.py`,
   `test_fuel_diagnostics.py`, `test_journey_tracker.py`,
   `test_schedule_assignment_ledger.py`, `test_fleet_fuel_observer.py`,
   `test_panel_fuel_observer.py`, `test_schedule_observer.py`,
   `test_target_policy.py`, `test_site_permissions.py`,
   `test_storage_checkbox.py`, `test_task_audit.py`, and
   `tests/sim_clock/test_scheduled_phases.py`. CI must fail if any required
   log lacks that summary or contains a skipped-test report. The B2B and dry
   assignment Python wrappers must fail if their named C++ scenario success
   markers are missing. Do not substitute `pytest` or a discovery command that
   can silently run zero tests.

   From the checkout root, preserve the Linux workflow's exact additional
   commands and environments. In the `Almanac host compile tests` step, with
   `PYTHONPATH=.` and `PYTHONDONTWRITEBYTECODE=1`, run
   `python3 tests/almanac_button/run_host_tests.py`,
   `python3 tests/almanac_button/run_binary_sha256_tests.py`,
   `python3 tests/almanac_button/run_tax_policy_tests.py`,
   and `python3 tests/almanac_overlay_dev/run_host_tests.py`. In the
   `Tool self-tests` step, with `PYTHONDONTWRITEBYTECODE=1`, run
   `python3 tools/audit_economy_csv.py selftest` and
   `python3 tools/peinfo.py selftest`; any nonzero command fails validation.
   The workflow's `test-logs` artifact retains Linux suite logs and summaries.
   Require the PR's Windows workflow jobs to pass from the checkout root,
   including `build.bat` and `build.bat clean wrsre`; the `windows-plugins`
   artifact retains build output. Add `.github/workflows/ci.yml` to the exact
   files. Keep the required in-game checklist in the PR body; do not claim
   post-change runtime validation.

**Exact files:** `.github/workflows/ci.yml`, `b2b_settlement.h`, `assignment_settlement.h`,
`dry_assignment_runtime.h`, `private_logistics.cpp`, the observer headers
(`fleet_fuel_runtime.h`, `panel_fuel_runtime.h`, `journey_tracker.h`), related
private-logistics tests, `tests/sim_clock/test_scheduled_phases.py`,
`docs/ROADMAP.md`, `docs/visions/economy-direction-2026-11-22.md`,
`docs/private-logistics-runtime/DO_OFFICES.md`,
`docs/engine/private-industry-re.md`,
`docs/private-logistics-runtime/evidence/STEP3B_OWNER_RUN_2026-11-27.md`, and
this task record.

**Validation:** investigation and plan revision 8 approved by both reviewers.
Implement, run each
`tests/**/test_*.py` directly with `PYTHONPATH=.` and verify it actually runs;
run C++ host tests in Linux CI and full Windows CI via the PR. The user
authorizes pushing and opening a PR, not merging it. No game launch/deployment
or post-change live validation is authorized.

## Native claim evidence

| Claim ID | Exact claim | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
|---|---|---|---|---|---|
| 3B-E1 | 661 committed `0x68F792` building-side deliveries have a next logged `0x68F7F9` vehicle leg with matching tonnage and p9 equal to vehicle home; 32/32 assignment windows have exactly one receiver equal to the logged trip target. | Tracked report and verifier `docs/private-logistics-runtime/evidence/STEP3B_OWNER_RUN_2026-11-27.md` and `do_receiver_check.py`; bounded raw rows at input lines 1488/1490; source-log SHA-256 in report. | Ran the tracked verifier against the owner-supplied log and independently counted committed site rows. | supported for this run | The old log has no transfer-sequence field; adjacency is among logged site rows and cannot rule out an unlogged intervening hook. The 1.82 MB source log stays local; owner identifies build as `be946b9`/PR #102 code; no binary hash. Does not prove every build or unobserved route. |
| 3B-E2 | Engine layout evidence identifies vehicle `+0x04` as global vehicle index; current `VehSlotIdx` reads `+0x10`, then falls back to modulo pointer hashing. | `docs/engine/private-industry-re.md:38`; `plugins/private_logistics/private_logistics.cpp:353-367` | Compare native offset table with direct source read. | supported | The old runtime log does not include raw `+0x04`/`+0x10` values. |
| 3B-E3 | Cargo is erased when a different vehicle pointer reaches the same cargo-state slot; this happens in both load and delivery paths. | `plugins/private_logistics/b2b_settlement.h:227-232,347-354` | Both branches replace `vehPtr` and clear all four lots. | supported | The log does not record slot index or pointer-owner changes, so not every affected delivery is individually attributable to this mechanism. |
| 3B-E4 | The only production `resetAll()` call is save loading. | `plugins/private_logistics/b2b_settlement.h:166-175`; `plugins/private_logistics/private_logistics.cpp:1612-1617` | Search all production references. | supported | The old run log contains no reset/slot diagnostic; whether a save-load reset occurred in that session is not established. |
| 3B-E5 | The old caller silently skips goods payment on `mdMatched && !cargoFound`. | `plugins/private_logistics/private_logistics.cpp:819-868`; `plugins/private_logistics/b2b_settlement.h:380-396` | Match returns true after delivery match, sets cargoFound false when tracked lots do not fully cover the delivery; caller has no missing-cargo branch. | supported | Existing log predates the requested diagnostic. |

## Build/test evidence

| Check | Command / cwd | Exit | Output artifact | Coverage / limits |
|---|---|---|---|---|
| Python suites (fresh baseline) | From `C:\WRSRE`, each discovered `tests/**/test_*.py` invoked independently with `PYTHONPATH=.` | 38/39 exit 0; `tests/sim_clock/test_scheduled_phases.py` exit 1 | Session run output summarized below; no raw output retained | 39 files invoked; one hardcoded `g++` `FileNotFoundError`; 31 test skips; five compiler-dependent private-logistics modules report `Ran=0` and one class-level skip. No C++ coverage on this host. |
| C++ host tests | Not run on this host | blocked | — | No `g++`, `clang++` or `cl` available; Linux CI host-test job not run. |
| Windows CI | No PR exists | not run | — | Build evidence unavailable; no build claim. |

Fresh test summary:
- `test_scheduled_phases.py` failed with `FileNotFoundError [WinError 2]`
  because it directly invokes `g++`.
- `test_fuel_diagnostics.py`, `test_journey_tracker.py`,
  `test_panel_fuel_observer.py`, `test_schedule_assignment_ledger.py`, and
  `test_schedule_observer.py` reported zero tests and a compiler-required
  class-level skip. Other compiler-dependent wrappers also reported explicit
  skips. The 39 invocations are not a pass and do not establish C++ host
  coverage.
- No raw log files were retained; this result is a summarized session
  observation, not an independently reopenable test artifact.

## Review rounds

| Round / gate | Diff/artifact hash | Focus 1 report | Focus 2 report | Repairs / new evidence |
|---|---|---|---|---|
| Investigation revision 1 | `be946b9` source/docs | rejected | rejected | Revisited after plan revision |
| Plan revision 2 | this record, before N1-N4 were identified | approved | approved | Approval superseded by receiver-provenance blocker |
| Investigation revision 2 | evidence audit and N1-N4 | rejected: 3B-2 still lacks road receipt proof | rejected: 3B-2 still lacks road receipt proof; test baseline not auditable as prior report | Corrected receiver claim; active target/direction alternative rejected as receipt evidence |
| Investigation revision 3 | tracked owner-run report, verifier, bounded excerpts, and this record | approved | approved | Both reviewers accepted receiver evidence for this observed run; full source log remains local and build identity is owner-reported |
| Investigation revision 4 | strict committed-mode/sign verifier and updated report | approved with medium hardening note | approved with medium hardening note | Both reviewers requested magnitude matching and clearer limits on the lack of an explicit sequence ID |
| Investigation revision 5 | tonnage-matching verifier and updated report | approved | approved | Supports the 661 logged pairs and 32 assignment windows for this run; both reviewers accept explicit sequence/build limits |
| Plan revision 4 | this task record | rejected: P4-1–P4-5 | rejected: receiver, treasury, cargo and output contracts underspecified | Revised rollback mechanics, receiver ambiguity/identity and explicit invalid-index behavior; added named test matrix and exact diagnostics contract |
| Plan revision 5 | this task record | approved with findings | rejected | P5-1–P5-6 required additional sequence/reset, transfer, receiver, receipt, and CI details; revision 6 addresses them |
| Plan revision 6 | this task record | rejected: P6-1–P6-3 | rejected: P6-1–P6-2 | Receipt lifecycle, exact CI enforcement and transfer-time identity revalidation needed further specification |
| Plan revision 7 | this task record | rejected: P7-1/P7-2 | approved with findings: P7-1/P7-2/P7-3 | Atomic receipt reservation, final post-credit validation, and generation-scoped receipt identity need explicit contracts |
| Plan revision 8 | this task record | approved | approved | Both reviewers approve implementation-ready plan; build/runtime remain separate |
| Implementation | not started | not run | not run | Implementation stage in progress; review gate follows frozen diff |

### Plan revision-4 finding reconciliation

| Finding | Disposition in revision 5 |
|---|---|
| P4-1 treasury read-back and rollback detail | Addressed: required callbacks, finite reads, observed-balance restoration, verified payee rollback and explicit failure reasons are specified. |
| P4-2 delivery ambiguity / pending-ring overwrite | Addressed: exact sequence, unique matching candidate, ambiguity metadata and permanent unknown direction on overwrite are required. |
| P4-3 invalid vehicle index behavior | Addressed: explicit `{index, valid}` result; no fallback or slot access when invalid. |
| P4-4 cargo/missing-cargo test coverage | Addressed: named scenario covers distinct/reused/invalid indices, save-load reset, no-cargo count and capped output. |
| P4-5 receiver and assignment identity | Addressed: token fields and receiver/home liveness/key revalidation at close are explicit. |
| Focus-2 receiver eligibility finding | Addressed: `originalTarget`/row-count are removed from token creation and eligibility; only a verified receiver enters payer policy. |
| Focus-2 exact output/test contract | Addressed: billing precedence, dry/off semantics, line fields and named C++/Python assertions are specified. |

### Plan revision-5 finding reconciliation

| Finding | Disposition in revision 6 |
|---|---|
| P5-1 sequence overflow, save-load reset and ring overwrite | Added per-save generation plus per-generation sequence reset; pending receiver/pair state is discarded and open tokens invalidated on save loading. Sequence/generation exhaustion saturates and disables proof. Active pending-record overwrite persistently invalidates affected open tokens, or all tokens if attribution is unsafe. |
| P5-2 ambiguity precheck side effects | Candidate counting is a read-only precheck; existing newest-candidate goods selection, candidate consumption and cargo-pop behavior remain unchanged. |
| P5-3 private transfer rollback | All DO-fuel live transfers, including private receiver payers, use a verified routine with required callbacks, finite pre/post reads, debit/credit read-back and observed-balance rollback. Paid accounting follows only verified success. |
| P5-4 receiver completion | Close-time receiver validation now requires identity, liveness and native completion; unfinished receivers fail closed with `receiver_unfinished`. |
| P5-5 receipt-ring overflow | Receipt history is non-circular and retained across save loads. At the existing 256-entry capacity, later settlement fails closed with `receipt_capacity`; no prior receipt is evicted. |
| P5-6 exact test and CI contract | Named Python wrapper methods and success markers, sorted direct Linux CI invocation, `test-logs` and `windows-plugins` artifacts, host-test commands and both Windows build commands are frozen in the plan. |

### Plan revision-6 finding reconciliation

| Finding | Disposition in revision 7 |
|---|---|
| P6-1 receipt/save-load lifecycle | `AutoPersistenceEvent(TSM_SAVE_LOADING)` owns save-generation advance and assignment-token invalidation in live and dry modes. Remove assignment-token and receipt clearing from `DryCostPersistence`; receipt history and cursor survive save loads. |
| P6-2 CI enforcement and exact commands | Add `.github/workflows/ci.yml` and require nonzero test counts/no skips for compiler-backed private-logistics wrappers, with wrapper assertions for scenario markers. The plan now records the exact Linux host/tool commands, checkout-root cwd, environment, Windows build commands and artifact names. |
| P6-3 transfer-time party identity | Before every live mutation, revalidate payer/payee stable keys, liveness, privacy and completion. If payee validation fails after payer debit, verify payer rollback and fail without credit. |

### Plan revision-7 finding reconciliation

| Finding | Disposition in revision 8 |
|---|---|
| P7-1 atomic duplicate/capacity reservation | Under `g_lock`, reserve a generation-scoped receipt before any live transfer callback/mutation; concurrent closes see `in_progress` and fail. Commit only after verified transfer, final identity and generation checks; retain terminal failed reservations; never evict. |
| P7-2 final post-credit identity check | After credit/read-back, revalidate receiver, private payer company and DO payee before paid accounting. Any change or generation mismatch triggers verified rollback of both observed balances and no paid count. |
| P7-3 receipt identity across save loads | Key receipts by `(save_generation, epoch, id, root)` while retaining prior generations. Test same native identity in a new generation as a distinct assignment, stale close without a current token, and a genuinely new post-load assignment. |
| Focus-1 stale status | Corrected: revision 8 is the approved plan gate; revision 6 findings are reconciled in revision 7 and revision 7 findings are reconciled here. |

## Pre-implementation validation snapshot (superseded by current status below)

- **STATIC REVIEW:** INVESTIGATION APPROVED for revision 5, limited to the
  observed run. PLAN REVISION 8 APPROVED by both independent reviewers.
  IMPLEMENTATION REVIEW NOT RUN; it follows the frozen implementation diff.
- **BUILD/TEST EVIDENCE:** BLOCKED — fresh Python baseline has one failure and
  compiler skips; C++ host tests and Windows CI were not run.
- **RUNTIME VALIDATION:** BASELINE OWNER RUN ONLY — the owner-authorized
  `be946b9` run supports receiver attribution for 661 pairs and 32 assignments.
  No post-change in-game validation is authorized or claimed.

## Pre-implementation blocker / resume checkpoint

- **Current gate and next action:** implementation. The Copilot lead is the
  sole writer after the required Kilo provider failed authentication; implement
  per approved revision 8, test, then send the frozen diff to both independent
  reviewers. The owner-run evidence supports the observed road receiver pair
  only; implementation must enforce sequence at the detour and fail closed
  outside verified assignment receiver evidence.
- **Owned files / resource lock:** one writer (Copilot lead) on
  `step-3b-state-do-fuel`; no Ghidra lock or query; configured project
  directory absent.
- **Unresolved finding IDs:** 3B-2 receiver evidence is supported for the
  observed run, but payer implementation remains open; 3B-1 and 3B-3–3B-10
  remain implementation-open. Plan-revision-7 findings are addressed in
  revision 8 and approved by both reviewers.
- **Missing artifact/capability/authorization:** Kilo CLI 7.8.27 was present;
  config check reported no warnings and agent list included the configured
  profiles, but the autonomous-dev launch exited 1 with
  `Your authentication token is invalid. Please sign in again.` No sign-in,
  credential, or settings changes were attempted; Kilo did not edit files.
  No binary hash for the owner run; the full local log is intentionally not
  copied into the repository. There is no live post-change game authorization.
- **Strategy change tried:** evaluated active task target `+0x528` plus
  unload-only route flags as an alternative. Both reviewers found it proves
  intent at most, not completed receipt; rejected as a billing basis.

## Pre-implementation delivery checkpoint (superseded below)

- **Changed files:** task record, owner-run evidence report, and tracked
  receiver verifier; production implementation is in progress.
- **Acceptance results:** not met; tests, implementation reviews, CI and
  required PR remain pending.
- **Ledger reconciliation:** 3B-2 is supported for this observed run, with
  scope limits; all implementation findings remain open.
- **Checks actually run:** all 39 Python files independently invoked; 38
  passed their process exit, 1 failed, 31 tests skipped; no C++ build or Windows
  CI.
- **Independent coverage:** both focus reviewers approved investigation
  revision 5 for this observed run, with explicit limits. Both reviewed plan
  revisions 5–8; revision 8 is approved. Implementation-stage reviews remain
  required. Kilo full-loop delegation is unavailable due the invalid CLI auth
  token; Copilot continues the work without changing Kilo credentials/settings.
- **Remaining unknowns:** exact cause for each of the 206 lost cargo matches,
  complete raw-log replay without the local source file, C++ host coverage,
  Windows CI, and post-change runtime/game behavior.

## Implementation update (current)

This section supersedes the pre-implementation checkpoint above.

- **Implementation:** receiver-based D40 payer, verified DO-fuel transfer,
  sequence/generation-scoped receiver evidence, non-evicting receipts,
  fail-closed cargo index handling, capped no-cargo diagnostics/counter,
  corrected vehicle-leg output, truthful billing-state logs, version `0.19.2-step3b`,
  and the requested roadmap/economy/DO-office documentation are present.
- **Review repair:** the first focus-1 review required the exact frozen diff
  before implementation review; that request is addressed by the complete
  commit-relative changed-file/line manifest above; focus 1 subsequently
  inspected the complete 32-path diff for `4dc25fd` and found no implementation
  defects. Focus 2 also reports no findings and approves static review. The
  first focus-2 review requested the receipt-capacity assertion
  and broader receiver/cargo edge coverage; the receipt wrapper assertion and
  named host scenarios for sequence/sign/owner mismatch, ring/token overflow,
  generation exhaustion, save-load/stale close, post-load assignment identity,
  unfinished/changed receivers, reset behavior, and no-cargo cap/fields have
  been added. Both independent implementation reviews are approved without
  code-level findings.
- **Python test evidence:** each of the 39 discovered `test_*.py` files was
  loaded separately with `python -m unittest discover`, repository-root cwd
  and `PYTHONPATH=.`. All 39 processes exited 0 (242 tests discovered, 32
  explicit skips; six compiler/platform-only suites report zero executed tests
  because their entire test class is skipped). The retained raw output is
  [`STEP3B_PYTHON_TESTS_2026-11-27.log`](../../private-logistics-runtime/evidence/STEP3B_PYTHON_TESTS_2026-11-27.log).
  Compiler/platform-dependent skips are explicitly present in that output;
  this is not Linux C++ host-test evidence.
- **MSVC build evidence:** `build.bat clean wrsre`, from `C:\WRSRE`, completed
  with exit 0 using the installed Visual Studio C++ tools. This is a local
  Windows monolithic build, not PR Windows CI evidence.
- **C++ host evidence:** the B2B host wrapper passed under MSVC. The dry-ledger
  host wrapper intentionally selects g++/clang++ and reports an explicit
  skip when neither is available; the current dry-ledger test translation unit
  compiles with MSVC, but an earlier manual execution crashed during its
  synthetic run, so compile-only is not counted as a host-test pass. Linux CI
  remains required for the C++ host-test gate.
- **STATIC REVIEW:** both focus reviewers approved the implementation diff
  without findings.
- **BUILD/TEST EVIDENCE:** Python process runs passed with skips noted; local
  monolithic MSVC build passed; B2B C++ host test passed under MSVC; dry-ledger
  C++ host suite and Linux CI are still pending.
- **RUNTIME VALIDATION:** owner-provided pre-change run only, as documented
  above. No post-change game validation has been run or is claimed.
- **Publication gate:** push was attempted and rejected by GitHub because the
  current Personal Access Token lacks `workflow` scope for the changed
  `.github/workflows/ci.yml`. The remote branch remains at
  `e7b5cfe3524cd30ee5f67ee63f07b436c61ae125`; local `HEAD` is
  `700effe098ecd0de0921e46d24ed049689eff8ef` and contains the reviewed
  implementation plus this evidence update. No PR was opened. The owner must
  push with credentials authorized to update workflow files, then open an
  unmerged PR.
- **PR body checklist (ready to paste):**
  - a private DO delivering to a state factory shows `paid>0` with
    `b2b_state_do_fuel` and the treasury goes down by that amount;
  - a pickup at a state building is charged to the receiving private company;
  - the start and day-close lines show `billing=live`;
  - no `billing=off` text appears while billing is live.

## Test fix and merge (2026-11-28)

This section supersedes the publication-gate notes above.

- **First real run of the dry-ledger host suite:** PR #104 CI (run
  46105541982) failed only in `tests/private_logistics/test_dry_assignment_ledger.py`.
  The earlier "39 processes exited 0" result was on Windows, where this suite
  skips, so it had never run before.
- **Cause: test harness only.** (1) `-Werror` unused
  `AssignmentObserveMode0FailureLocked` in the host translation unit;
  (2) the segfault: the test stubbed `TreasuryReadable()` true but left
  `g_currency` null; (3) three checks read `logs.back()` although the
  `[DO TRIP]` line precedes the `[FLEET ASSIGNMENT]` close line; (4) a stale
  expectation without `receiver_kind=`; (5) the Python wrapper expected a
  scenario name that does not exist (`ScenarioStep3bTripPayerAndBilling`; the
  real one is `assignment_close_charges_original_buyer_once_only_when_complete`).
- **Fix:** commit `a54dff1` (test files plus a `[[maybe_unused]]` attribute).
  Production behaviour is unchanged. With it: 18/18 dry-ledger and 29/29 fleet
  scenarios pass under `-fsanitize=address,undefined`; all `test_*.py` files
  exit 0 on Linux.
- **Open, non-blocking:** add the slot index and a cause to `ERROR NO CARGO
  RECORD`; `do_fuel_settlement.h` passes a literal `0` currency that the
  adapter ignores; PyYAML is not installed in CI, so `test_ci_workflow.py`
  skips.
- **RUNTIME VALIDATION:** none yet. The owner's in-game checklist is in the
  PR #105 body.

