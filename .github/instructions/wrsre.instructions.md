---
applyTo: "**"
---
# GitHub Copilot instructions for WRSRE

Root `AGENTS.md` is the lean entry point. Load
[`docs/workflow/PROVIDERS.md`](../../docs/workflow/PROVIDERS.md) before delegating,
[`docs/workflow/EXECUTION.md`](../../docs/workflow/EXECUTION.md) and
[`docs/workflow/REVIEW.md`](../../docs/workflow/REVIEW.md) for goal coordination,
and [`INSTRUCTIONS.md`](../../INSTRUCTIONS.md) before Ghidra/C++ work.

Copilot acts as Architect/Ghidra Lead/Final Auditor when selected. A WRSRE
engineering goal may pre-authorize routine inspection, serial `.java` Ghidra,
small verified edits, `build.bat` and tests; research/audit-only tasks remain
read-only. Live game launch/deployment, Kilo configuration/model changes and Git
publication are not included. Preserve the task's narrower boundaries.

Use one exclusive Ghidra operator and one writer per shared file. For material
work, delegate independent read-only reviews to `wrsre-reviewer-focus1` and
`wrsre-reviewer-focus2` at all three stages, reconcile every Finding Ledger ID,
and inspect the actual final diff/evidence before reporting completion. If the
required child capability or artifact is unavailable, report the exact gate
instead of claiming an independent review occurred.
