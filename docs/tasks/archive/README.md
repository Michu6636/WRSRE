# Task archive

Completed or deliberately retired task records retain their original filename,
stable Finding Ledger IDs, final dispositions, actual check outputs, review
coverage and remaining limits. A record here is history, not current
authorization or proof of runtime behavior.

Move a record here only after its status and ledger are reconciled. If work is
blocked, keep it in [`../active/`](../README.md) with the exact blocker and resume
checkpoint instead of archiving it as done.

## Retired 2026-11-18

These six moved out of `active/` during the repository cleanup. Each declared
itself complete or superseded in its own text. No source was changed by the move.

| Record | Disposition at retirement |
|---|---|
| [current-task-state-storage.md](current-task-state-storage.md) | Opt-in load-only state-storage targets. Staged v0.17 candidate; the shipped `private_logistics` 0.19.0 supersedes it. Retained for its design invariants and native offsets. |
| [TEST15_ELEMENT_CONSTRUCTION.md](TEST15_ELEMENT_CONSTRUCTION.md) | Evidence task COMPLETE (2026-11-04). Count-one pointer transfer decoded and reviewed. |
| [TEST15_NATIVE_CLOSURE.md](TEST15_NATIVE_CLOSURE.md) | Evidence request COMPLETE (2026-11-04). Bounded native closure for the insertion path. |
| [TEST15_PACKET_REPAIR.md](TEST15_PACKET_REPAIR.md) | Packet repair COMPLETE (2026-11-04). Mechanical defects in the V1 packet catalogued and corrected in V2. |
| [2026-10-22-old-vs-new-analysis.md](2026-10-22-old-vs-new-analysis.md) | Historical diff analysis of the v0.3 vs v0.17 builds. Descriptive only. |
| [2026-10-22-regression-triage.md](2026-10-22-regression-triage.md) | Triage of the duplicate-DLL regression; the fix shipped. Historical. |
