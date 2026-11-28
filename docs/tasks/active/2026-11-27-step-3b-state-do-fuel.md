# Task: roadmap step 3b — state-paid private DO trip fuel

## Contract

- **Status:** blocked — verified road receiver evidence is unavailable
- **Task record:** `docs/tasks/active/2026-11-27-step-3b-state-do-fuel.md`
- **Baseline:** `main` at `be946b9`, clean before this task; working branch
  `step-3b-state-do-fuel`.
- **Goal:** settle eligible private DO trip fuel to the actual goods receiver,
  correctly use the treasury for a state receiver, and make billing diagnostics
  accurately describe behavior.
- **Deliverables:** receiver-based payer policy and vehicle-linked road
  receiver evidence; accurate trip/day-close diagnostics; host tests; version
  and documentation updates; a pull request, not merged until the owner orders
  it. No implementation or PR is being claimed while receiver provenance is
  blocked.
- **Acceptance criteria:**
  - Host-tested payer policy: private receiver pays; state receiver pays from
    treasury with `b2b_state_do_fuel`; customs receiver is deferred; unknown
    direction is unpaid; a state seller with a private receiver is charged to
    that private receiver, never the treasury.
  - The receiver is determined from evidence proving the actual receiver for
    the road DO trip, not assumed from `originalTarget`, an intended target, or
    a sea/rail event. Missing/ambiguous delivery evidence results in
    `reason=direction_unknown`.
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
- **Ghidra owner:** none; the configured project directory is unavailable on
  this host. A missing completed-delivery-log `Building*` edge needs evidence
  from the project owner or an accessible, version-matched project/export.
- **Task-owned files:** `plugins/private_logistics/*` files directly related to
  trip settlement/diagnostics; focused tests under
  `tests/private_logistics/`; `docs/ROADMAP.md`,
  `docs/visions/economy-direction-2026-11-22.md`,
  `docs/private-logistics-runtime/DO_OFFICES.md`, and this task record.
- **Starting evidence:** D40/D41 in economy direction §1; roadmap step 3b;
  existing B2B matcher and current assignment observer. Neither currently
  proves the actual road receiver for a particular DO trip.

## Prior findings imported

| Finding ID | Source path/line/address | Exact statement | Initial status |
|---|---|---|---|
| 3B-1 | First step-3 in-game run, recorded in roadmap §3 | 22 private DO trips to state buildings were unpaid (`private_parties_required`, 41.33 RUB). | open |
| 3B-2 | `dry_assignment_runtime.h`, `AssignmentSettlementEvent` | `originalTarget` is used as the buyer even when that building was the loading site; the actual receiver must come from native task directions or matched vehicle load/unload events. | blocked — verified road receiver evidence unavailable |
| 3B-3 | Fleet, panel, dry-cost and journey diagnostics | Hardcoded `billing=off` is misleading while live billing is active; these observers never move money. | open |
| 3B-4 | `dry_assignment_runtime.h`, `DryCostContext` / persistence | Dry-cost collection requires `!g_billing`, but start/persistence diagnostics still run while billing is live. | open |
| 3B-5 | Roadmap step 3b / D40 | Day-close output does not distinguish private-paid fuel, treasury-paid fuel, or unknown direction. | open |
| 3B-6 | D39/D41 and roadmap step 3b | Customs receiver fuel is deferred; exports and foreign trade are out of scope. | open |
| 3B-7 | `dry_assignment_runtime.h`, `TransferFunds` | `TransferFunds` returns success in dry-run without moving money, but the trip path reports that amount as `paid`. | open |
| 3B-8 | `b2b_settlement.h`, `TransferFunds` and currency callbacks | Treasury transfers are not read back after mutation and can report success if the move callback is absent or ineffective. | open |

## Gate status

| Gate | Frozen artifact/revision | Focus 1 | Focus 2 | Open IDs / blocker |
|---|---|---|---|---|
| INVESTIGATION | Source and tracked evidence at `be946b9` plus revised evidence audit | rejected: 3B-2 has no verified road receiver path | rejected: 3B-2 and baseline test evidence are not sufficient for acceptance | 3B-2 blocks state-receiver billing |
| PLAN | This task record, revision 2 | approved before the road-evidence gap was identified | approved before the road-evidence gap was identified | Superseded; its 0x68F792/0x68F7F9 assumption is not valid for road DO receiver billing |
| IMPLEMENTATION | Not started | not run | not run | Do not implement payer settlement until 3B-2 is resolved |

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| 3B-1 | high | State-receiver DO fuel refused by `private_parties_required`. | open | — | — | Actual fuel units and coverage remain provisional. |
| 3B-2 | blocker | Neither `originalTarget`, the existing 0x68F792/0x68F7F9 sea/rail pair, nor an active-task target plus direction flags proves which road building actually received goods. | blocked | Road site 0x1B2898 has no destination argument; completed-delivery log Building* field is unresolved; task target is intent, not completion. Do not select treasury/private payer without verified receiver evidence. | Obtain a version-matched native export confirming the road delivery-log Building* field and its vehicle/order association, or receive explicitly authorized runtime evidence. | No Ghidra project path is available; live game test is not authorized. |
| 3B-3 | medium | Observer/diagnostic hardcodes say `billing=off`. | open | — | — | Source/host checks do not prove live game output. |
| 3B-4 | medium | Dry-cost lifecycle emits records/logs despite the billing gate. | open | — | — | Persistence and game lifecycle remain host-tested only. |
| 3B-5 | medium | Day-close summaries omit payer-class totals and unknown direction. | open | — | — | No runtime ledger reconciliation is claimed. |
| 3B-6 | low | Customs must remain deferred under D41. | open | — | — | Foreign-trade branch remains separate. |
| 3B-7 | high | Dry-run transfer success is logged as money paid. | open | — | — | Dry-run must be a planned amount, not `paid`. |
| 3B-8 | high | Treasury mutation is not verified by a balance read-back. | open | — | — | Runtime currency API behavior remains unverified. |

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

## Native claim evidence and current blocker

| Claim ID | Exact claim | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
|---|---|---|---|---|---|
| 3B-N1 | The existing `0x68F792`/`0x68F7F9` pair is identified as the sea/rail `FUN_14068B330` family, not the road DO unload. | `plugins/private_logistics/private_logistics.cpp:226-232`; `docs/private-logistics-runtime/evidence/INVESTIGATION_REPORT.md:10-23`; `docs/ASSOCIATION_REVIEW_COMPLETE.md:97-108` | The site table names 0x68F792 as family A building-side and 0x68F7F9 as its vehicle-side event; the raw-evidence report separately describes road transfer sites. | supported | No game observation was performed. |
| 3B-N2 | Road site 0x1B2898 does not pass a destination Building* to the transfer; the completed-delivery-log Building* field is unresolved. | `docs/engine/private-industry-re.md` H1/H7, especially lines 168-201 and 345-360; `docs/ASSOCIATION_REVIEW_COMPLETE.md:215,245`; `docs/ghidra-index/420350.md` | The call has NULL param_9; known log fields include completion and Resource*, while the Building* offset is an explicit open Ghidra question. | blocked | `0x1404209A0:0x1404209E0` query/export is missing; the cached local decomp does not resolve the field. |
| 3B-N3 | `vehicle+0x528` plus an unload-only route row is not proof of completed receipt. | `docs/private-logistics-runtime/DO_TRUCK_ASSIGNMENT_INTERVAL_FINAL.md:105-116`; `docs/private-logistics-runtime/DO_ASSIGNMENT_INTERVAL_INVESTIGATION.md:98-141`; `plugins/private_logistics/schedule_observer.h:1-2,30-52,75-80,136-142` | The target can be bound before cargo; the schedule observer explicitly is not a native job identity/completion API. | insufficient | Does not establish that goods reached the intended target or distinguish every partial/cancelled/replaced assignment. |
| 3B-N4 | The configured Ghidra project path is unavailable on this host. | `scratch/ghidra_run.bat`; host path check on `C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\ghidra\proj` | The configured JDK and analyzeHeadless paths exist, but the configured project directory does not. | blocked | Do not guess another project path or run against an unrelated project. |

`direction_unknown` remains the only safe payer decision when the actual
receiver cannot be determined. Do not treat a synthetic test of a proposed
direction rule as proof of native receiver semantics.

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
| Implementation | not started | not run | not run | Blocked; no implementation-review gate |

## Separate validation statuses

- **STATIC REVIEW:** REJECTED for this task — both reviewers found 3B-2 lacks
  verified road receiver provenance.
- **BUILD/TEST EVIDENCE:** BLOCKED — fresh Python baseline has one failure and
  compiler skips; C++ host tests and Windows CI were not run.
- **RUNTIME VALIDATION:** NOT RUN — no in-game run is authorized or claimed.

## Blocker / resume checkpoint

- **Current gate and next action:** blocked at investigation by 3B-2. Resume
  only after obtaining version-matched static evidence for the road completed
  delivery Building* field and its vehicle/order association, or after the
  owner explicitly authorizes another evidence method.
- **Owned files / resource lock:** one writer on `step-3b-state-do-fuel`; no
  Ghidra lock; configured project directory absent.
- **Unresolved finding IDs:** 3B-2 blocks payer settlement; 3B-1 and 3B-3–3B-8
  remain unimplemented.
- **Missing artifact/capability/authorization:** version-matched native
  evidence for road receipt; no live game authorization. GitHub write/PR
  creation is also not exposed by the available GitHub tools.
- **Strategy change tried:** evaluated active task target `+0x528` plus
  unload-only route flags as an alternative. Both reviewers found it proves
  intent at most, not completed receipt; rejected as a billing basis.

## Final delivery

- **Changed files:** task record only; no source, tests, version or roadmap
  changes.
- **Acceptance results:** not met; state receiver payment and required PR are
  blocked by 3B-2.
- **Ledger reconciliation:** 3B-2 is an explicit blocker; all other findings
  remain open pending implementation.
- **Checks actually run:** all 39 Python files independently invoked; 38
  passed their process exit, 1 failed, 31 tests skipped; no C++ build or Windows
  CI.
- **Independent coverage:** both focus reviewers independently rejected the
  revised investigation on road receiver evidence. Their revision-2 plan
  approvals predate and are superseded by this finding.
- **Remaining unknowns:** road completed-delivery receiver field and association,
  C++ host coverage, Windows CI, and all runtime/game behavior.
