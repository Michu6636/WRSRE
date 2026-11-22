# WRSRE agent entry point

Lean router for all agents. Session instructions and the user's task scope
control authorization; this file does not override a narrower task. Check the
actual host, shell, branch, working tree, tools and acceptance criteria before
acting, and preserve existing user changes.

## Load only what the task needs

- **What to work on next:** [`docs/ROADMAP.md`](docs/ROADMAP.md) (ordered
  steps, current state, how to run a step).

- **Goal execution/delegation:** [`docs/workflow/EXECUTION.md`](docs/workflow/EXECUTION.md)
  and [`docs/workflow/REVIEW.md`](docs/workflow/REVIEW.md). Use the tracked task
  record and Finding Ledger, not a chat-only handover.
- **Kilo setup/launch:** [`docs/workflow/KILO.md`](docs/workflow/KILO.md).
  **Antigravity/Copilot handoff:** [`docs/workflow/PROVIDERS.md`](docs/workflow/PROVIDERS.md).
- **Ghidra/C++:** relevant sections of [`INSTRUCTIONS.md`](INSTRUCTIONS.md);
  [`plugins/AGENTS.md`](plugins/AGENTS.md) loads lazily when touching plugins.
- **Docs/wiki:** [`docs/AGENTS.md`](docs/AGENTS.md), [`docs/README.md`](docs/README.md),
  the subsystem page, function index and bounded raw exports.
- **Native-evidence details/long-write recovery:** relevant sections of
  [`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md). Do not preload the whole
  historical guide or duplicate its rules into every prompt.

## Standing engineering-goal contract

When the user starts a WRSRE engineering goal adopting this workflow, the lead
may inspect repo/game files, run serial `.java` Ghidra queries, edit in small
verified chunks, run `build.bat` and tests, and read existing logs/artifacts
without routine confirmation. Research/audit-only tasks remain read-only.

Not included: launching `SOVIET64.exe`, deploying DLLs, writing game memory,
changing Kilo settings/models, creating/switching branches, destructive Git,
commits or pushes. A required live game test is a genuine blocker unless the
task explicitly authorizes it. Autonomous Kilo profiles deny commit/push;
publication is a separate explicit operator action.

## Non-negotiable loop

Run **Investigate → Plan → Implement → Build/Test → Review → Repair** until the
acceptance gate passes or a genuine blocker is recorded. The repository adds no
round limit or `steps` cap; client/provider limits still apply. The Worker writes;
`reviewer-focus1` and `reviewer-focus2` are strictly read-only and independently
review all three stages. Reconcile every prior in-scope finding in the ledger.
Do not stop at a plan, first patch or “final review,” and do not ask whether to
continue routine repairs.

Reuse tracked evidence before querying. One operator owns each Ghidra project,
including GUI/headless/MCP access. Never cite ignored `scratch/` as final proof.
Keep STATIC REVIEW, BUILD/TEST EVIDENCE and RUNTIME VALIDATION separate; a goal
completion report, reviewer vote, host test or exit code is not live game proof.
If an independent reviewer or required artifact is unavailable, report that
exact gate/blocker instead of claiming it passed.
