# Goal execution contract

One accountable lead (`autonomous-dev` in Kilo) owns the task record, shared
resources, integration and final report. Children do bounded work and return
evidence; they do not start competing goals or top-level sessions.

## Start or resume

1. Inspect the actual host, shell, branch, working tree, available tools and
   task scope. Preserve pre-existing changes and identify the task-owned diff.
2. Create or update a tracked task record from
   [`../tasks/TEMPLATE.md`](../tasks/TEMPLATE.md). Record the goal, acceptance
   criteria, authorized/forbidden actions, baseline, evidence paths, Ghidra
   owner and expected deliverables. Import every prior in-scope audit finding
   with a stable ID; do not silently discard old findings.
3. Reuse `docs/ghidra-index/`, `docs/ghidra-exports/`, `docs/engine/` and
   accessible local caches before querying. A cited `scratch/` path is not an
   accessible artifact. Promote only permitted, bounded raw excerpts needed for
   the claim, with provenance and limits.
4. If Kilo goal tools are exposed, start/resume the attached objective with the
   goal lifecycle in [`KILO.md`](KILO.md). Otherwise execute the same contract
   in the current run. Do not invent a tool or claim cross-turn continuation.

## Standing authorization

For a WRSRE engineering goal that explicitly adopts this contract, the lead may
inspect repository/game files, run serial `.java` Ghidra queries through the
available wrapper, edit in small verified chunks, run `build.bat`, run tests,
and read existing logs/artifacts without routine confirmation.

This does **not** authorize launching `SOVIET64.exe`, deploying DLLs, writing
game memory, changing Kilo settings/models, creating/switching branches,
destructive Git actions, commits or pushes. A required live game test is a
blocker unless the task record explicitly authorizes it. Research/audit-only
tasks remain read-only. The autonomous profiles deny commit/push by default;
publication is a separate explicit operator action.

## Three reviewed gates

Run the full loop: **Investigate → Plan → Implement → Build/Test → Review →
Repair**. There is no repository-imposed 2–4 round limit, final-review-only
stop, or `steps` cap. Client/provider limits still apply and must be reported.

### Gate 1 — INVESTIGATION

- Separate observed facts, inferences, contested claims and unknowns.
- For each consequential game call record target/slot, ABI/registers,
  arguments/return, side effects, receiver/vtable evidence and fault protection.
- Trace unchanged callers, alternate guards, state transitions and negative
  claims far enough to expose counterexamples. Record scan scope and limits.
- Update the Finding Ledger and claim evidence. Send the frozen investigation
  packet independently to both reviewers. Do not promote missing evidence.

### Gate 2 — PLAN

- Name exact files, state transitions, ABI assumptions, failure paths,
  cleanup, non-goals and rollback/preservation strategy.
- Map every acceptance criterion and changed behavior to a static check, host
  test, Windows build or explicitly pending runtime test.
- Reconcile Gate 1 findings. Send the plan and ledger independently to both
  reviewers. Do not implement until both approve the plan or the lead records
  a genuine blocker. Evidence-backed reviewer objections are repaired, not voted away.

### Gate 3 — IMPLEMENTATION and verification

- Edit only task-owned files in small coherent chunks; read back each affected
  region. Keep one writer per shared file/report and one Ghidra operator.
- Inspect the full task diff plus relevant unchanged call paths. Run the
  smallest affected checks after each logical change, then the required suite.
- Record exact commands, working directory, exit status and accessible output.
  Verify build artifacts/timestamps where applicable; a skipped Windows build
  is not a pass. Check `.ini` files are UTF-8 without BOM when touched.
- Freeze the affected files. Send the exact diff, changed/unchanged paths,
  ledger, raw evidence and check outputs independently to both reviewers.
- Triage every finding against source/evidence, repair valid defects, rerun
  affected checks and submit a new frozen packet. Repeat without a round cap.

## Finding Ledger and exit gate

Every imported or newly discovered finding keeps its ID and one disposition:
`fixed`, `disproven` with deciding evidence, `out-of-scope` with an explicit
scope reason, `unresolved`, or `blocked`. In-scope defects cannot be closed as
out-of-scope merely to obtain approval. Reviewers reconcile every ID at every
stage; the lead checks the final ledger against the actual diff.

Completion requires all of the following:

- acceptance criteria are checked against actual artifacts;
- both reviewers approve the final frozen state with no open in-scope defect;
- every ledger ID has an evidence-backed disposition;
- required static/host/build checks pass, or an unavailable required check is a
  recorded blocker rather than a silent omission;
- STATIC REVIEW, BUILD/TEST EVIDENCE and RUNTIME VALIDATION are reported
  separately. Host tests and static reasoning are not live runtime validation;
- the task record contains the final diff summary, checks, review coverage,
  remaining limits and resume point.

## Genuine blockers and progress

Pause/report only for a missing required artifact/capability, a required live
`SOVIET64.exe` test without authorization, quota/provider failure, resource
ownership conflict, permission denial, or a `doom_loop`/no-progress condition.
Before stopping for no progress, inspect the source, change the question or
method, and record the smallest next evidence/action. Do not repeat an unchanged
query, append status-only summaries, or ask “should I continue?” for routine work.

Kilo itself may pause a goal after errors or no successful action. Inspect the
cause, update the task record, repair what is safely repairable, and resume the
same goal when appropriate. This runtime pause is not a repository round cap.
Before compaction or handoff, persist the current gate, ledger, owned files,
resource lock, checks and next action in the tracked task record.
