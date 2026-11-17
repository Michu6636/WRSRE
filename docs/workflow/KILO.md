# Kilo setup and runtime notes

Checked against official Kilo documentation on **2026-11-14** and upstream
source at commit `85eb887a7560f452211bcd382eafe8cb2f8df497`. The npm registry
reported `@kilocode/cli` version `7.8.27` during this audit. Kilo was not
installed in the audit checkout, so these are documented contracts, not a local
runtime validation. Check `kilo --version`, `kilo run --help` and the effective
configuration on the operator machine before an unattended run.

## Tracked layout

- [`../../.kilo/kilo.jsonc`](../../.kilo/kilo.jsonc): agent modes, temperatures,
  compaction and ordered permissions. `autonomous-dev` is primary;
  `implementation-worker` is a hidden `all` profile for bounded CLI/child tasks;
  both reviewers are hidden subagents. It deliberately pins no model and sets no
  `steps` cap for the WRSRE profiles.
- [`../../.kilo/prompts/`](../../.kilo/prompts/): short prompt bodies referenced
  with `{file:./prompts/<name>.md}`. Paths are relative to the config file; keep
  them inside the project root. These files are tracked, unlike local Kilo state.
- Root `AGENTS.md` is loaded by Kilo. Nested `docs/AGENTS.md` and
  `plugins/AGENTS.md` are discovered lazily when files in those trees are read.
  `INSTRUCTIONS.md` and the workflow modules are read on demand, not added to an
  always-loaded `instructions` array. [1](https://kilo.ai/docs/customize/agents-md)

## Goals and CLI

`/goal <objective>` starts a session goal; `/goal pause`, `/goal resume` and
`/goal clear` control it. The agent can also use exposed `goal`/`goal_report`
tools. Only the root goal worker reports completion/blockers; children return
findings. Completion is model-reported, not independently verified. Goals can
pause on errors, rejected permissions, Stop, backend restart or no successful
action. Inspect and repair the cause before resuming. [2](https://kilo.ai/docs/code-with-ai/agents/goals)

For a full run, attach a tracked task record:

```text
kilo run --auto --agent autonomous-dev -f docs/tasks/active/<task>.md "Execute the attached WRSRE task record. Use the goal lifecycle if exposed; otherwise run the full loop to completion or a recorded blocker."
```

`kilo run --command goal` is a goal status/control command, not a way to pass a
new objective. Do not combine `--continue` with autonomous mode or a prompt
argument. A CLI exit code, including 0, is not proof that the acceptance gate
passed; inspect the task record, diff, checks and reviews. [3](https://kilo.ai/docs/code-with-ai/platforms/cli)

## Permissions are not a sandbox

Rules are ordered and the last matching rule wins. Put broad fallbacks first
and exceptions afterward. Custom agents inherit defaults for unspecified
permissions, so the reviewer profiles use a deny fallback and then allow only
read/search/list tools. Shell, edit, task, external-directory, web and goal
controls are denied for reviewers. [4](https://kilo.ai/docs/customize/agent-permissions)

Do not rely on `ask` as an unattended guard. Current CLI help/source describes
`--auto` as approving requests that are not explicitly denied, while some prose
pages describe auto-approval settings; verify the installed behavior. The
project therefore uses explicit `deny` for prohibited Git/config actions and
reviewer writes. `doom_loop` remains `ask` for the lead so a repeated-failure
cycle cannot silently continue. [3](https://kilo.ai/docs/code-with-ai/platforms/cli)
[5](https://kilo.ai/docs/getting-started/cost-controls-and-usage-safeguards)

The lead retains broad shell access for authorized builds/tests and external
game/Ghidra inspection. Git guards are conservative: unknown/ambiguous Git
commands and commands mentioning the Kilo CLI are denied, while common read-only
Git inspections are allowed. A false positive is a blocker to report, not a rule
for the agent to relax. Command patterns cannot make arbitrary shell execution
safe: a script, interpreter or alternate spelling can bypass a textual rule.
Never wrap a prohibited action to evade a deny. Use a trusted checkout and keep
the operating-system account, game installation and Ghidra project scoped to
the task. Built-in config protection is helpful but is not a substitute for
these explicit project denies and operator review.

Project permission edits are cached by a running backend. Reload the VS Code
window or start a fresh CLI session after an operator-approved config change;
do not have the autonomous agent relax its own rules. [6](https://kilo.ai/docs/getting-started/settings/auto-approving-actions)

## Models, quota and context

Use the operator's existing free/quota-limited Kilo model for the lead,
implementation worker and routine review loops. Use Antigravity or GitHub
Copilot as Architect/Ghidra Lead/Final Auditor when available; see
[`PROVIDERS.md`](PROVIDERS.md). Kilo does not automatically route work to those
external products. Do not invent provider/model IDs, add credentials, change
`model`/`subagent_model`, or silently fall back to a paid model when quota fails.

Keep stable guidance in the lean root and prompt files; load subsystem details
and raw evidence selectively. Reuse the index and exports before querying.
Compaction is enabled, but the tracked task record and Finding Ledger are the
recovery source of truth. Before compaction/handoff, persist the current gate,
owned files, lock owner, unresolved IDs, exact checks and next action. Free
models have rate limits and provider privacy/retention policies; quota exhaustion
is a recorded blocker, not a reason to lower verification standards. [5](https://kilo.ai/docs/getting-started/cost-controls-and-usage-safeguards)
[7](https://kilo.ai/docs/getting-started/rate-limits-and-costs)

## Operator preflight

1. Confirm the installed CLI/extension discovers `autonomous-dev`,
   `implementation-worker`, `reviewer-focus1` and `reviewer-focus2`.
2. Confirm the effective reviewer permissions deny writes/shell/delegation and
   the lead has no `steps` cap or unexpected model pin.
3. Confirm the task record is tracked, the goal/acceptance criteria are concrete,
   and the Ghidra owner is exclusive. Confirm wrapper/JDK/project/game paths on
   the actual host; do not assume historical Windows paths exist.
4. Have the lead delegate a small read-only reviewer smoke task and inspect its
   returned coverage. Reviewer profiles are subagents, not primary CLI agents.
   A discovered agent name is not proof that its permissions loaded correctly.
5. After the run, inspect the actual diff, ledger, check outputs and both review
   reports. Have the stronger external auditor challenge the final state before
   treating the goal as delivered.

## Sources

- [1](https://kilo.ai/docs/customize/agents-md) — AGENTS.md loading and nested rules.
- [2](https://kilo.ai/docs/code-with-ai/agents/goals) — session goals and automatic pauses.
- [3](https://kilo.ai/docs/code-with-ai/platforms/cli) — CLI commands and autonomous mode.
- [4](https://kilo.ai/docs/customize/agent-permissions) — ordered permissions and delegation.
- [5](https://kilo.ai/docs/getting-started/cost-controls-and-usage-safeguards) — doom loops, context and free-tier limits.
- [6](https://kilo.ai/docs/getting-started/settings/auto-approving-actions) — permission defaults and config reload.
- [7](https://kilo.ai/docs/getting-started/rate-limits-and-costs) — context and model-selection cost guidance.
- [8](https://kilo.ai/docs/customize/custom-subagents) — prompt file references and subagent inheritance.
- [9](https://github.com/Kilo-Org/kilocode/blob/85eb887a7560f452211bcd382eafe8cb2f8df497/packages/opencode/src/cli/cmd/run.ts) — inspected `--auto` permission handling.
- [10](https://github.com/Kilo-Org/kilocode/blob/85eb887a7560f452211bcd382eafe8cb2f8df497/packages/opencode/src/kilocode/session/goal/runner.ts) — inspected goal pause/progress behavior.
