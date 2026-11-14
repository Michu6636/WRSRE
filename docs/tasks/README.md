# Task records

Tracked task records are the durable goal/compaction/handoff surface. Use
[`TEMPLATE.md`](TEMPLATE.md) for new work; it includes the contract, prior-finding
import, gate status, Finding Ledger, native claim evidence, build/test outputs,
review rounds, separate validation statuses and resume checkpoint.

## Lifecycle

- New/current work: `active/YYYY-MM-DD-<slug>.md`. Replace every placeholder and
  name one Ghidra operator and the task-owned files before launch.
- Blocked work stays in `active/` with an exact blocker, smallest next action,
  lock/release state and unresolved IDs. Do not hide a blocker by renaming it complete.
- Completed or deliberately retired work moves to [`archive/`](archive/README.md)
  with final ledger dispositions and evidence links. Preserve stable finding IDs.
- Existing files below are historical packets/specs. Their presence in `active/`
  does not prove they are the current goal, still authorized, or build/runtime
  verified. Read each status and reconcile it before reuse; do not silently
  reclassify or rerun old native requests.

## Current and historical index

- [active/current-task-state-storage.md](active/current-task-state-storage.md) — staged v0.17/v6.11 opt-in source-only, cap 5, dry_run=1
- [active/NEXT_PHASE.md](active/NEXT_PHASE.md) — staged status + 6 validations
- [active/GHIDRA_REQUEST.md](active/GHIDRA_REQUEST.md) — read-only probe panel+0x310 capacity

The controlling workflow is [`../workflow/EXECUTION.md`](../workflow/EXECUTION.md)
and [`../workflow/REVIEW.md`](../workflow/REVIEW.md). Ignored `scratch/` may hold
working packets, but the tracked record and bounded evidence are what future
agents inherit.
