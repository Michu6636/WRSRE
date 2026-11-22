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

Reconciled 2026-11-22. `private_logistics` is now at `0.19.0-simplified-settlement`
and `ui_panels` well past v6.11, so every older packet below predates the current
source. None of them is the current goal except the 2026-11-22 records.
The ordered plan of future tasks is in [`../ROADMAP.md`](../ROADMAP.md).

- [active/2026-11-22-remove-fallbacks-and-made-up-data.md](active/2026-11-22-remove-fallbacks-and-made-up-data.md) —
  **CURRENT.** Roadmap step 2 / D32–D34: no fallback prices, no default CPI
  basket, `UNPRICED`/`UNAVAILABLE` made visible, Almanac sample rows removed.
  Host tests and the Windows CI build pass; independent review and the owner's in-game check are pending.
- [active/2026-11-22-remove-market-multiplier-and-procurement.md](active/2026-11-22-remove-market-multiplier-and-procurement.md) —
  **CURRENT.** Economy code step 1: native prices only, no state procurement.
  Host tests and CI pass; merged in PR #100. In-game acceptance check pending
  (roadmap step 0).
- [active/2026-11-22-private-do-under-construction.md](active/2026-11-22-private-do-under-construction.md) —
  **CURRENT.** Bug fix on the same PR: private DOs no longer target or charge
  for buildings under construction. Host tests and CI pass; merged in PR #100.
  In-game check pending (roadmap step 0).
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
