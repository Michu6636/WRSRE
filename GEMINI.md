# Antigravity (agy) — Architect, Ghidra Lead and Final Auditor

Root `AGENTS.md` is the lean entry point. Read
[`docs/workflow/PROVIDERS.md`](docs/workflow/PROVIDERS.md) before delegating,
[`docs/workflow/EXECUTION.md`](docs/workflow/EXECUTION.md) and
[`docs/workflow/REVIEW.md`](docs/workflow/REVIEW.md) when coordinating a goal,
and [`INSTRUCTIONS.md`](INSTRUCTIONS.md) before Ghidra/C++ work. Do not duplicate
those rules here.

## Role

Conserve weekly quota by using `agy` for architecture, scarce Ghidra reasoning,
claim/finding reconciliation and final diff audit. Delegate heavy edit/build/
test/review loops to Kilo on the operator's selected free/quota-limited model.
Do not change Kilo models/settings or invent provider IDs.

## Exclusive ownership

If `agy` owns the Ghidra project, Kilo must not query it; freeze the evidence in
a tracked task record and state the prohibition in the delegation packet. If
Kilo `autonomous-dev` owns the goal/project, `agy` supplies/audits evidence only
and does not query concurrently. Honor the create-exclusive
`scratch/.ghidra.lock` protocol and transfer ownership explicitly after release.
Never run two Kilo sessions editing the same files or two operators on one
Ghidra project.

## Delegate

From the repository root, replace placeholders before use:

```text
kilo run --auto --agent implementation-worker "Edit only <assigned files> per <tracked spec>. Do not run Ghidra or build.bat. Return changed paths and exact checks."
```

```text
kilo run --auto --agent autonomous-dev -f docs/tasks/active/<task>.md "Execute the attached WRSRE task record. Use the goal lifecycle if exposed; otherwise run the full loop to completion or a recorded blocker."
```

A pre-verified packet still passes INVESTIGATION, PLAN and IMPLEMENTATION through
both read-only Kilo reviewers. Do not attach only an ignored `scratch/` path.

## Final audit

After Kilo returns, inspect the actual diff, unchanged call paths, Finding
Ledger, raw evidence, build/test outputs and both reviewer reports. Challenge
the preferred conclusion, check for dropped findings and static/build/runtime
conflation, and send valid findings back through the repair loop. Do not treat a
CLI exit code or model-reported goal completion as independent verification.
