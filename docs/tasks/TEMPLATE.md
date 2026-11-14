# Task: <short title>

> Template only. Copy to `docs/tasks/active/YYYY-MM-DD-<slug>.md`, replace every
> placeholder, and keep the record tracked. Do not launch a goal with unfilled
> acceptance criteria or authorization fields.

## Contract

- **Status:** active | paused | blocked | complete
- **Task record:** `<tracked path>`
- **Baseline:** `<branch / commit / pre-existing worktree caveats>`
- **Goal:** `<one concrete objective>`
- **Deliverables:** `<files/reports/behavior expected>`
- **Acceptance criteria:** `<observable, checkable list>`
- **Non-goals:** `<explicit exclusions>`
- **Authorized:** inspect repo/game files; serial `.java` Ghidra; small verified
  edits; `build.bat`; tests; read existing logs/artifacts. Narrow this list if
  the user task is narrower.
- **Forbidden:** live `SOVIET64.exe` launch/deployment/game-memory writes unless
  explicitly added above; Kilo config/model changes; branch creation/switch;
  destructive Git; commit/push from the autonomous profile.
- **Ghidra owner:** `<one operator / none / not available>`
- **Task-owned files:** `<paths; preserve all other changes>`
- **Starting evidence:** `<docs/engine, ghidra-index, ghidra-exports, audits>`

## Prior findings imported

List every prior in-scope audit finding before starting. Use stable source IDs;
do not renumber away history. Missing source material is an explicit gap.

| Finding ID | Source path/line/address | Exact statement | Initial status |
|---|---|---|---|
| <ID> | <source> | <statement> | open |

## Gate status

| Gate | Frozen artifact/revision | Focus 1 | Focus 2 | Open IDs / blocker |
|---|---|---|---|---|
| INVESTIGATION | <path/revision> | pending | pending | <IDs> |
| PLAN | <path/revision> | pending | pending | <IDs> |
| IMPLEMENTATION | <diff hash/revision> | pending | pending | <IDs> |

## Finding Ledger

Keep one row per finding across all rounds. Dispositions: fixed; disproven with
deciding evidence; out-of-scope with explicit scope reason; unresolved; blocked.
Reviewers reconcile every row. In-scope defects are not closed for convenience.

| ID | Severity | Statement / source | Disposition | Deciding evidence / fix | Test / recheck | Limit |
|---|---|---|---|---|---|---|
| <ID> | <severity> | <statement + path/address> | open | <evidence> | <check> | <limit> |

## Native claim evidence

Use for consequential native claims; link bounded tracked excerpts rather than
pasting whole exports. Label RVA / preferred VA / runtime VA / file offset.

| Claim ID | Exact claim | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
|---|---|---|---|---|---|
| <ID> | <claim> | <tracked path/range> | <derivation> | supported/contested/unresolved | <limit> |

## Build/test evidence

Record exact command, working directory, exit status and accessible output.
Do not label a skipped Windows build or host-only test as native runtime proof.

| Check | Command / cwd | Exit | Output artifact | Coverage / limits |
|---|---|---|---|---|
| <static/host/build> | <exact command and cwd> | <code> | <tracked path or supplied attachment> | <limit> |

## Review rounds

Attach or link each independent report. Reviewers receive the same frozen
inputs but do not see each other's verdict first.

| Round / gate | Diff/artifact hash | Focus 1 report | Focus 2 report | Repairs / new evidence |
|---|---|---|---|---|
| <round> | <hash> | <path + verdict> | <path + verdict> | <IDs fixed> |

## Separate validation statuses

- **STATIC REVIEW:** APPROVED | REJECTED — <coverage and limits>
- **BUILD/TEST EVIDENCE:** PASS | PARTIAL | FAIL | NOT RUN | BLOCKED — <checks>
- **RUNTIME VALIDATION:** PASS | FAIL | NOT RUN — <exact live evidence/reason>

## Blocker / resume checkpoint

- **Current gate and next action:** <smallest useful next step>
- **Owned files / resource lock:** <writer and Ghidra owner; release state>
- **Unresolved finding IDs:** <IDs>
- **Missing artifact/capability/authorization:** <exact request, if genuinely needed>
- **Strategy change tried:** <what changed after a no-progress pass>

## Final delivery

- **Changed files:** <task-owned paths>
- **Acceptance results:** <criterion → evidence>
- **Ledger reconciliation:** <all IDs accounted for>
- **Checks actually run:** <commands/results>
- **Independent coverage:** <both reports and final external audit, if run>
- **Remaining unknowns:** <explicit limits; no silent drops>

See [`../workflow/EXECUTION.md`](../workflow/EXECUTION.md) and
[`../workflow/REVIEW.md`](../workflow/REVIEW.md) for the controlling contract.
