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

## Active index

Reconciled 2026-11-18. `private_logistics` is now at `0.19.0-simplified-settlement`
and `ui_panels` well past v6.11, so every packet below predates the current
source. None of them is the current goal.

- [active/NEXT_PHASE.md](active/NEXT_PHASE.md) — **BLOCKED.** Staged v0.17 opt-in
  state-storage targets, cap 5, `dry_run=1`. Superseded in source by the shipped
  road-20/rail-5 policy; keep for its reasoning, not as an implementation plan.
- [active/TEST16_ROAD_TARGETS.md](active/TEST16_ROAD_TARGETS.md) — **IMPLEMENTED, native
  acceptance PENDING.** Road type 43 = 20 total targets, rail type 52 = 5. Host
  regressions pass; no live-game confirmation.
- [active/TEST15_INSERTION_CONTRACT.md](active/TEST15_INSERTION_CONTRACT.md) — read-only
  native question about `FUN_14001E140`. Not started here; needs the Windows
  Ghidra operator.
- [active/GHIDRA_REQUEST.md](active/GHIDRA_REQUEST.md) — read-only DO-panel capacity
  probe above five entries.

See [`archive/`](archive/README.md) for the six records retired on 2026-11-18.

The controlling workflow is [`../workflow/EXECUTION.md`](../workflow/EXECUTION.md)
and [`../workflow/REVIEW.md`](../workflow/REVIEW.md). Ignored `scratch/` may hold
working packets, but the tracked record and bounded evidence are what future
agents inherit.
