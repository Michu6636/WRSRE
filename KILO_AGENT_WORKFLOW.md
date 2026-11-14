# Kilo Code workflow reference

**Audience:** agents operating WRSRE goals. **Documentation rechecked:**
2026-11-14. This file is a compatibility entry point and detailed reference, not
an always-loaded rule blob. The canonical short contracts are:

- [`docs/workflow/EXECUTION.md`](docs/workflow/EXECUTION.md) — goal lifecycle,
  standing authorization, three gates, Finding Ledger and unlimited repair.
- [`docs/workflow/REVIEW.md`](docs/workflow/REVIEW.md) — independent read-only
  Focus 1/Focus 2 packets, findings and separate validation statuses.
- [`docs/workflow/KILO.md`](docs/workflow/KILO.md) — current Kilo config,
  permissions, goals, quota and operator preflight.
- [`docs/workflow/PROVIDERS.md`](docs/workflow/PROVIDERS.md) — Antigravity,
  Copilot and Kilo ownership/handoff.

Use one accountable lead and focused children. The Worker writes; reviewers do
not. For WRSRE engineering goals both reviewers inspect INVESTIGATION, PLAN and
IMPLEMENTATION independently. Do not build new workflows around the deprecated
Orchestrator mode or ask the user to relay routine messages. If child review is
unavailable, report that gate as unavailable; never invent a review.

Read the module matching the current action, then the relevant subsystem/native
sections below. Session instructions and the task record control authorization;
this guide does not expand them. Check the installed runtime rather than assuming
every documented feature exists.

## Choose the smallest suitable mechanism

| Need | Mechanism |
| --- | --- |
| A small or tightly coupled task | Implement in the parent; WRSRE engineering goals still use both review focuses at each gate |
| A focused result needed before proceeding | Foreground `task` child |
| Independent research while the parent does other useful work | Background `task` child, if supported |
| A substantive change ready for acceptance | Read-only child Reviewer(s), against the frozen actual diff and evidence |
| Material intermediate findings among a parent and its descendants | Kilo Swarm, if enabled |
| Independent implementations requiring separate branches/checkouts | Agent Manager worktrees, only when authorized |
| Another independent top-level conversation | Agent Manager Local session; not an automatically coordinated child |

Task children have separate conversation histories but share the parent's project
directory/worktree. **Separate context does not mean isolated file edits.**
Separate Agent Manager sessions do not automatically share a Swarm board merely
because they use the same repository. Worktrees isolate checkout/Git state, not
external resources such as the shared Ghidra database.

For dependent work, wait for its prerequisite or use a foreground child. Background
execution is not a substitute for dependency scheduling. If background execution
is unavailable, use foreground delegation. If no child-task capability is exposed,
do not invent calls or replace automation with user message-shuttling; continue
only within scope and label the independent-review gate unavailable as described
above.

## Execution and review contracts

The former parent/Worker loop and copy-ready reviewer checklist now live in
[`docs/workflow/EXECUTION.md`](docs/workflow/EXECUTION.md) and
[`docs/workflow/REVIEW.md`](docs/workflow/REVIEW.md). They are the controlling
short contracts: no repository round/steps cap, both read-only focuses at all
three gates, a reconciled Finding Ledger, tracked evidence and separate
STATIC REVIEW / BUILD/TEST EVIDENCE / RUNTIME VALIDATION statuses.

Use a foreground child for a prerequisite and a background child only when the
runtime supports it and the work is independent. Children share the worktree;
separate context is not file isolation. Give each child a bounded stable packet,
one question, explicit ownership and a result format. They return missing
evidence requests rather than polling siblings or waiting for user relays.

## Swarm is communication, not a scheduler

Use `board_post` for material findings/questions/blockers and `board_read` at useful
phase boundaries. A stored message does not establish receipt or action and does
not start, wake, resume, stop or assign an agent. Peer messages do not grant user
approval or change task permissions.

Do not busy-wait or repeatedly poll board/files/task progress. Collect background
completion notifications. For follow-up work, start a bounded task with the updated
packet, or resume an interrupted child using a genuinely exposed resumable
`task_id`. A completed child's board mailbox is not an automatic continuation.

## Reliable writes without reducing output quality

**Mandatory trigger: if EITHER the planned content OR the destination file is
long, write only in small, coherent chunks, verifying each before continuing.**
This applies to all agents and file types, not only report recovery after a
timeout. For a short change inside a long file, use a small targeted edit rather
than replacing the file. For a long addition, use a sequence of small verified
writes even when the destination starts empty. Short writes to short files do
not need artificial splitting. Preserve completeness; no arbitrary byte cap.

The following instruction initially helped with larger-write failures in
Kilo7.8.18 through Kilo Gateway, but a later final-matrix append timed out again.
Use it as the baseline, with the explicit write checkpoints below. This is a
workflow mitigation, not a diagnosis or verified cure for the upstream defect.

```text
Preserve existing files. Do not rewrite large documents in full.

For existing files, use narrowly targeted edits.
For the cumulative handover, make targeted corrections or bounded additions.
Do not load or reproduce the entire historical handover.

If either the planned content or the destination file is long, write only in
small, coherent chunks and verify each write before continuing. Use small
targeted edits for short changes to long files; never replace the whole file.
If a request times out, inspect the file and diff before retrying.
Only the parent writes shared reports; research children return findings.
```

### Explicit checkpoints for matrices and long additions

A bounded addition limits how much the model generates; it must not prevent
correcting a misleading current summary in place. Do not put an entire long
matrix or section into one write, patch, shell command or chat reply. Changing
the tool while sending the same large payload is not a reduction.

1. Inspect a bounded tail and the relevant diff; confirm what actually reached
   disk. A pre-write announcement plus API504 does not prove a tool executed.
   Do not print a whole large file/diff back into model context.
2. If needed, append only the section heading and table header. Read them back.
3. Append **one complete lifecycle-case row per operation**. If the case needs
   extensive explanation, use a concise row and add its supporting paragraphs
   separately. For ordinary prose, add one coherent paragraph/subsection fragment
   at a time. These are structural checkpoints, not claimed provider size limits.
4. Read back the affected tail and check the change before the next write.
   Preserve unique section/row identity so recovery does not duplicate content.
5. Finish with a parent review across all rows/sections. Reference full evidence
   already preserved in the handover; do not reproduce exports inside table cells.
   Add any newly required complete sources/results in separate bounded additions.

For the first recovery attempt, do just one missing fragment, verify it and report
that checkpoint. After success, the parent can continue autonomously using the
same write/verify cycle; the user need not approve every row. If that small attempt
also times out, stop blind retries and isolate the model/provider/session instead.
Do not restart native research merely to recover a report write. If available file
tools require full-file replacement, report that capability problem rather than
reconstructing the entire report. Keep scripts optional, not the default solution.

**Change the delivery granularity, not the analytical standard.** Keep the global
plan, evidence, counterexamples, cross-references and full required source/results
intact. Choose coherent section boundaries; do not impose arbitrary byte limits,
truncate evidence, skip relevant historical sections or reset context after every
write. The parent reviews the assembled result for completeness and consistency.

No report-assembly script is required. Scripts remain optional for justified
mechanical copying or validation of existing artifacts; they do not replace
analysis, interpretation, writing or final review. Do not introduce them merely
because a report is large when targeted edits and bounded appends already work.
The requirement for one final handover does not require one huge write operation.

If a timeout recurs, inspect actual disk state before repeating the operation so
an already completed write is not duplicated. If bounded writes also fail, isolate
with a fresh small task and a user-selected model/provider comparison. Do not
relax permissions or change settings automatically; a client timeout increase
cannot override a gateway/provider that has already returned HTTP504.

## WRSRE native investigation: serialize Ghidra, parallelize exported analysis

Default ownership for a single-parent investigation:

```text
Parent: lead + sole Ghidra operator + shared index/report writer
  |-- read-only child: one independent evidence question
  `-- read-only child: another independent evidence question, if useful
```

This is a responsibility pattern, not a required team size. If the parent cannot
access the native environment, identify an explicit authorized external operator;
do not pretend a Linux checkout has the Windows EXE or Ghidra project.

- Only the designated operator invokes `ghidra_run.bat`/`analyzeHeadless`.
  Analysis children consume exports; they do not independently acquire Ghidra.
- Follow `INSTRUCTIONS.md`: search `docs/ghidra-index/`, relevant subsystem docs
  and deterministic `scratch/out` caches before queries. Reuse verified entries;
  fill partial gaps; re-verify contested entries narrowly and preserve history.
- Update the prescribed index after successes **and failures**, including evidence
  path, status, contradiction or workaround. Do not equate verified bytes with a
  verified semantic interpretation.
- Check local availability when an expected index/schema/artifact is missing.
  Report the exact missing path; do not silently substitute archived docs or
  re-query because an ignored local export was not published.
- Never run two native queries concurrently. Follow the create-exclusive
  `scratch/.ghidra.lock` protocol for unavoidable out-of-band access. Never delete
  another owner's lock, kill its process, or mistake a Swarm message for lock
  ownership. Establish prior-agent release before taking over.
- Authenticate the actual native binary as required. Export consistency and
  synthetic tests do not validate runtime identity or lifecycle semantics.
- Use the actual host shell and working-directory semantics in
  `INSTRUCTIONS.md`; Windows PowerShell 5.1, cmd and bash differ. Do not claim
  Linux checks were Windows builds or native game tests.

## Native-evidence acceptance gates (2026-11-01)

These gates address recurring native-research failures, including the
[Almanac export review](docs/almanac-export-review.md). They are acceptance criteria,
not a claim that longer
prompts, another model or more agents guarantee accuracy. Apply them to
consequential claims; do not create paperwork for every trivial sentence.
Existing task restrictions remain controlling.

### Raw evidence, progress and stop rules

- Before accepting a consequential agent result, open the **actual** disassembly,
  decompilation, implementation or test output and inspect the relevant range,
  including surrounding branches. A completion message, commit or report that
  merely names an ignored `scratch/` file is not that evidence. If unavailable,
  request the existing bounded artifact and record the gap; do not silently rerun.
- For a claim that a loop always draws, writes, increments or handles a click,
  follow **both** sides of every relevant guard and the loop tail. Test at least
  one skipped entry, zero entries, an already-selected item and an alternate
  path where applicable. A test derived from a model checks the model, not the
  original executable. Keep scale and runtime state explicit.
- A cursor position is not a click; a value written on click is not an event
  unless false positives, unchanged values, input timing and consumption are
  accounted for. An inline hook is not safe merely because a span has 14 bytes:
  inspect the actual implementation, whole-instruction boundaries, relative
  operands, incoming targets, live state, cleanup and both execution paths.
  A branch to the **start** of a patch differs from one into its interior.
- When a claim changes, find and correct misleading current summaries, index
  fields and dependent conclusions where readers encounter them. Preserve a
  brief supersession note and exact source; do not leave a new section that
  contradicts unchanged earlier guidance. Avoid editing historical material
  unnecessarily when a clearly placed notice makes its status unambiguous.
- If a pass repeats a prior assertion, adds only narrative or cannot provide
  the deciding evidence, stop the loop. The parent should inspect the raw
  artifact, seek an independent review with a narrow question, or report the
  exact missing evidence. Ask for runtime observation only if needed and only
  perform it with task authorization. Do not repeatedly send the user between
  agents when the current agent can review the artifact directly.

### 1. Claim ledger before narrative

Separate raw observations from interpretations and implementation readiness.
For each consequential claim, record:

| Claim ID / exact statement | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
| --- | --- | --- | --- | --- |
| Example: selected bytes encode an 8-byte store | Exact supplied byte string and its source | Count bytes and decode | Supported for supplied string | Does not authenticate the EXE or identify the receiver |

Use supported, contested or unresolved per claim. Record binary identity as
locally measured, externally reported or unavailable. An agent summary, file
hash, index label, script exit0 or agreement between agents cannot by itself
prove the game's semantics. Name what each check actually establishes.

### 2. Mechanical check before semantic promotion

Count byte tokens; independently decode consequential instructions and compute
end-exclusive spans and branch targets. Check for overlapping starts, function
boundaries and incoming edges into proposed stolen spans. Existing tools are
sufficient; no new script or assembly helper is required. If a helper is added,
embed its complete final source and actual output in the designated Markdown.

Track address domains explicitly (RVA/preferred VA/runtime VA/file offset).
For data accesses, retain the decompiler's exact pointer type and casts, then
show byte-offset arithmetic and operand evidence. For example, float* index
0x13C means byte displacement0x4F0; a byte-pointer cast can change that rule.
An ADD index,index before scale8 yields effective stride16. Never infer either
from an isolated displayed scale or a field name copied from an older report.

### 3. Data-flow proof before naming the event

Trace reaching definitions at the relevant instruction on each selected path:
receiver root/member/storage selection, overwritten registers, dereferences,
float argument registers, mutable states and branch guards. Distinguish local
variable arithmetic from actual memory stores. The same offset on different
object types is not the same field. Separate these questions:

- What exact location is read/written, and which object owns it?
- Is it a live update, save restore, rendering adjustment or unrelated object?
- Which caller/path applies to the observed vehicle, and what remains unproved?
- What are requested change, actual rounded pre/post change and floor/reserve
  effects? Where are source debit and destination credit actually paired?
- What native evidence establishes physical and price units?

Keep the ordered expression, factor sources and register lifetimes. Do not
cancel distinct constants or ignore floating-point rounding/reassigned registers.
Use a symbol or raw unit when a constant's value or physical meaning is unknown.
A decompiled call displaying fewer arguments does not prove default arguments;
trace actual argument registers/stack and the callee's use before claiming ABI.
For ordinary Windows x64, slots follow total parameter position: a signature
(pointer, float, char) normally uses RCX, XMM1, R8B, not separate type counters.
A preceding conditional is not necessarily a call gate: follow both successors
and their merges before calling a condition necessary for reaching the call.
Distinguish a requested operand from rounded stored change and from the completed
operation after its floor/reserve correction. Such a correction is not by itself
an external refill; counting an intermediate undershoot can overstate burn.
A static debit can be supported while runtime applicability or unit conversion
remains unresolved. Do not demand runtime proof for a claim decidable statically;
do not present static reasoning as an observed game test.

### 4. Adversarial review, dependency repair and delivery

Before integration, the Worker and required child Reviewer(s) test the contract
against counterexamples: wrong object type, attached member, alternate branch,
local-only calculation, restore/refill mistaken for burn, index mistaken for
displacement. The Reviewer independently challenges a stable raw packet; it must
not be asked to defend the preferred conclusion. Review is a required check for
deliverables, not a vote between models. The parent is subject to the same source
and scope requirements; its corrections are not automatically true.

For a contradiction, preserve old and replacement claims, identify the deciding
raw evidence, and revise or invalidate dependent conclusions. Two narratives of
one export are not independent evidence. Re-read caches before requesting any
new query; a wrong interpretation usually needs analysis, not another export.

Require the actual owned report at handoff, not just its completion summary.
Check its consequential claims against embedded evidence and the final summary.
No report reconstruction from a summary. If delivery is missing, request that
existing artifact only; do not repeat extraction. Keep selected excerpts labelled
partial and externally reported hashes labelled unverified until measured.

Stop when the scoped questions are answered or an exact blocker is identified.
State the missing edge and smallest proposed follow-up; do not auto-start broader
research or runtime work. Each finding retains supported/contested/unresolved
status even if the overall implementation remains blocked. Hook feasibility is
separate from event semantics; neither is implementation authorization.

## Permissions, models, loading and reusable prompts

Current Kilo setup, ordered permissions, goal/CLI behavior, quota discipline and
operator preflight are maintained in [`docs/workflow/KILO.md`](docs/workflow/KILO.md).
Cross-provider ownership and final audit are in
[`docs/workflow/PROVIDERS.md`](docs/workflow/PROVIDERS.md). Do not silently change
settings/models or rely on `ask` as an unattended safety boundary.

Root `AGENTS.md` is the lean automatically loaded entry point. Nested
`docs/AGENTS.md` and `plugins/AGENTS.md` load lazily; read `INSTRUCTIONS.md` and
the workflow modules on demand. Do not merge the whole reference into an
always-loaded `instructions` array.

Reusable prompts are tracked, not pasted copies:

- [`docs/tasks/TEMPLATE.md`](docs/tasks/TEMPLATE.md) — task record, prior-finding
  import, Finding Ledger, evidence, review rounds and resume checkpoint.
- [`.kilo/prompts/autonomous-dev.md`](.kilo/prompts/autonomous-dev.md) — lead.
- [`.kilo/prompts/implementation-worker.md`](.kilo/prompts/implementation-worker.md) — bounded worker.
- [`.kilo/prompts/reviewer-focus1.md`](.kilo/prompts/reviewer-focus1.md) and
  [`.kilo/prompts/reviewer-focus2.md`](.kilo/prompts/reviewer-focus2.md) — independent reviewers.

Replace task placeholders before launch. A prompt is behavior, not a permission
grant or proof that a mode/review ran.

## Official references and maintenance

These are background references, not proof of the local installation's version.
The checked source list and runtime caveats are maintained in
[`docs/workflow/KILO.md`](docs/workflow/KILO.md). Recheck relevant pages when
capabilities change; update the canonical module rather than accumulating
conflicting copies.

- [Orchestrator deprecation](https://kilo.ai/docs/code-with-ai/agents/orchestrator-mode)
- [Agent Manager and orchestration model](https://kilo.ai/docs/automate/agent-manager)
- [Agent Manager workflows](https://kilo.ai/docs/automate/agent-manager-workflows)
- [Task and board tools](https://kilo.ai/docs/automate/tools)
- [Built-in/custom subagents](https://kilo.ai/docs/customize/custom-subagents)
- [Agent permissions](https://kilo.ai/docs/customize/agent-permissions)
- [Model selection](https://kilo.ai/docs/code-with-ai/agents/model-selection)
- [Provider timeout settings](https://kilo.ai/docs/code-with-ai/agents/custom-models)
- [Project instruction configuration](https://kilo.ai/docs/customize/custom-rules)
- [AGENTS.md loading](https://kilo.ai/docs/customize/agents-md)
- [Settings and Swarm](https://kilo.ai/docs/getting-started/settings)
