# WRSRE agent workflow

Canonical operating contracts for autonomous WRSRE engineering goals. The root
[`AGENTS.md`](../../AGENTS.md) is the lean discovery entry point; this directory
holds the detailed rules that agents load only when needed.

## Load map

| When | Read |
|---|---|
| Starting or resuming a goal | [`EXECUTION.md`](EXECUTION.md) and the task record under [`../tasks/`](../tasks/README.md) |
| Sending or answering a review | [`REVIEW.md`](REVIEW.md) |
| Configuring or launching Kilo | [`KILO.md`](KILO.md) |
| Handing work between Antigravity, Copilot and Kilo | [`PROVIDERS.md`](PROVIDERS.md) |
| Ghidra or C++ work | Relevant sections of [`../../INSTRUCTIONS.md`](../../INSTRUCTIONS.md); [`../../plugins/AGENTS.md`](../../plugins/AGENTS.md) when touching plugins |
| Native evidence details or long-write recovery | Relevant sections of [`../../KILO_AGENT_WORKFLOW.md`](../../KILO_AGENT_WORKFLOW.md) |
| Documentation/wiki work | [`../AGENTS.md`](../AGENTS.md), [`../README.md`](../README.md), [`../engine/README.md`](../engine/README.md), [`../ghidra-index/README.md`](../ghidra-index/README.md) |

Do not preload every file in this table. A task record names its subsystem and
acceptance criteria; load the matching guidance and evidence, not the whole wiki.

## Five objectives

1. **Set and forget:** one accountable lead runs Investigate → Plan → Implement
   → Build/Test → Review → Repair until the acceptance gate passes or a genuine
   blocker is recorded. The repository adds no round cap or `steps` cap.
2. **Independent verification:** the Worker writes; `reviewer-focus1` and
   `reviewer-focus2` are strictly read-only and review all three stages. Every
   prior in-scope audit finding is reconciled in a Finding Ledger.
3. **Standing authorization with guardrails:** routine inspection, serial
   `.java` Ghidra queries, small verified edits, `build.bat` and tests are
   pre-authorized by an engineering goal. Destructive Git actions, Kilo
   configuration/model changes, deployment and live game tests are not.
4. **Tracked knowledge:** durable results live in `docs/tasks/`,
   `docs/engine/`, `docs/ghidra-index/` and bounded `docs/ghidra-exports/`.
   Ignored `scratch/` is working space, never final proof.
5. **Quota discipline:** keep always-loaded instructions short, reuse verified
   evidence, delegate bounded packets, and reserve stronger external models for
   architecture, Ghidra lead and final audit. Free-tier availability is not
   guaranteed; quota exhaustion is a blocker, not permission to switch models.

## Authority and evidence

Session instructions and the user's task scope control authorization. These
files describe the workflow; they do not override a narrower task. A reviewer
verdict, goal completion report, index label or exit code is not native runtime
proof. Keep STATIC REVIEW, BUILD/TEST EVIDENCE and RUNTIME VALIDATION separate.
