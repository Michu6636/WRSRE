# Task: private DOs must not serve buildings under construction

## Contract

- **Status:** active (host tests and CI Windows build pass; no in-game run)
- **Task record:** `docs/tasks/active/2026-11-22-private-do-under-construction.md`
- **Baseline:** branch `arena/672c9ef4-wrsre` (PR #100), on top of economy
  step 1 (`50c293c`).
- **Goal:** stop `private_logistics` automatic target assistance from
  directing private DO trucks at a building that is still under
  construction, and stop charging fuel for such trips.
- **Deliverables:** code fix in `private_logistics`, host tests, the vision
  doc [vision-private-construction.md](../../visions/vision-private-construction.md)
  (the owner asked for it in the same request).
- **Acceptance criteria:**
  1. An unfinished private storage or consumer is never appended as a target.
  2. An unfinished, opted-in state storage is never appended as a source.
  3. An existing entry (manual or generated) whose target is unfinished
     gets directions 0. The entry is kept, and its directions return on the
     first daily pass after the building is finished.
  4. An unfinished DO is not treated as an office.
  5. A DO trip whose buyer is unfinished is never charged
     (`reason=target_under_construction`).
  6. Fail closed: an unreadable build-progress float counts as unfinished.
- **Non-goals:** private construction offices (vision only), changes to
  `private_construction`, PL_VERSION bump (tests pin it).
- **Authorized:** the owner chose "implement the fix now" and "add to PR #100"
  on 2026-11-22.
- **Task-owned files:** `plugins/private_logistics/private_logistics.cpp`,
  `site_permissions.h`, `dry_assignment_runtime.h`, and the tests listed
  below.
- **Starting evidence:** the owner's `tesmioloader.log` from the PR #100
  build (save "Test 02").

## Prior findings imported

| Finding ID | Source | Exact statement | Initial status |
|---|---|---|---|
| UC-1 | owner log, 17:15:28 | `[AUTO DO] native target=...FF0968A0 type=23 policy=managed requested_mask=2 ... unload_all=1` for `MIRRORZ_pub`, which was `[CLAIMED]` by `private_construction` at 17:14:41 and still `[BUILDING]` (66 % at 17:20:11) | open |
| UC-2 | owner log, 17:17:36–17:19:52 | 302 logged `[B2B SITE] rva=0x68F792 ... "MIRRORZ_pub" priv=1 ... commit=0`, 0 with `commit=1`. The same RVA committed 566 times for other buildings in the log | open |
| UC-3 | owner log, 17:17:37 | `[DO TRIP] dry_run assignment=3 buyer="MIRRORZ_pub" ... charge=6.09 paid=6.09 eligible=1`: dry run only, but a live run would pay for a trip that delivered nothing | open |

## Finding Ledger

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| UC-1 | high | discovery in `AutoTargetsOnDay` checked valid, not demolished, private and capabilities, but never `TsmIsFinished` | fixed | new `AutoBuildingFinished` (progress `+0x604 >= 1.0`, unreadable = unfinished). An unfinished site is indexed with `hasStorage=0`, and an unfinished DO is skipped | `target_policy_runtime_test.cpp` `UnderConstruction`; `test_site_permissions.py` integration guards | host stubs only |
| UC-1b | high | the reconcile pass re-applied directions to existing entries whose target was unfinished | fixed | `AutoDirectionsForBuildState(..., targetFinished)` gives mask 0, so the entry is deactivated but kept | `UnderConstruction` (deactivated, kept, restored, no "absent" memo churn); `site_permissions.h` truth table | host stubs only |
| UC-2 | info | `commit=0` means the native stock add returned non-1 (`private_logistics.cpp` about line 603) | explained | a construction site has no working storage | none | native reason not traced in Ghidra |
| UC-3 | high | trip settlement charged a buyer that never received goods | fixed | `AssignmentSettlementEvent` gate gives `reason=target_under_construction` | `dry_assignment_runtime_test.cpp` (no capital moves, reason logged) | a site that finishes during the trip is charged as normal |
| UC-4 | medium | 6 test files had no `unittest.main()`, so CI's `python3 "$f" -v` ran 0 tests from them | fixed | added the entry point; all 6 pass when executed | direct runs | was pre-existing |

## Build/test evidence

| Check | Command / cwd | Exit | Coverage / limits |
|---|---|---|---|
| host suite | `for f in $(find tests -name 'test_*.py'); do PYTHONPATH=. python3 $f; done` (repo root) | 0 failures | host only |
| syntax | `g++ -std=c++17 -fsyntax-only -fno-exceptions -fpermissive -w -I/tmp/shim -include new private_logistics.cpp` | 29 shim errors before and after; none in new code | not MSVC |
| Windows | CI run 45067152128 on PR #100 (`03ef0bc`): hygiene, host tests, Windows (MSVC + SEH) | pass | build only, no in-game run |

## Separate validation statuses

- **STATIC REVIEW:** self-review only.
- **BUILD/TEST EVIDENCE:** PASS: host suite and CI Windows build (run 45067152128).
- **RUNTIME VALIDATION:** NOT RUN. Expected in a new log: `[AUTO DO] day N
  under construction: K storage/consumer site(s) excluded`, no new
  `[B2B SITE] ... commit=0` lines at a `[BUILDING]` site, and an existing
  entry logging `under construction: directions 0 (entry kept; restored once
  finished)`.

## Blocker / resume checkpoint

- **Next action:** owner in-game check on a save with a private site under
  construction ([`ROADMAP.md`](../../ROADMAP.md) step 0); then merge with
  PR #100.
- **Unresolved finding IDs:** none in scope.
