# Kilo Code: team workflow and prompt-writing guide

**Audience:** anyone writing prompts or operating AI agents for this repository.
**Official documentation last checked:** 2026-10-31. Check the installed runtime's
actual capabilities; do not assume every documented feature is available.

## Default: one parent, focused children, explicit resource ownership

Use **one normal Code session** as the lead for work involving tools, scripts or
report edits. Plan and Debug can also delegate when appropriate. Do not build new
workflows around the deprecated Orchestrator mode.

The parent owns planning, dependency scheduling, verification and final delivery.
It may delegate focused `task` children when separate context or independent
analysis is useful. Small tasks should remain solo. Do not force a fixed number
of agents or ask the user to relay messages between them.

Read `INSTRUCTIONS.md` and the relevant subsystem instructions first. This guide
explains coordination; it does not grant implementation, runtime, configuration,
Git, deployment or game-test authorization. Preserve the user's task-specific
boundaries and the current session's branch restrictions.

## Choose the smallest suitable mechanism

| Need | Mechanism |
| --- | --- |
| A small or tightly coupled task | Work in the parent session |
| A focused result needed before proceeding | Foreground `task` child |
| Independent research while the parent does other useful work | Background `task` child, if supported |
| Material intermediate findings among a parent and its descendants | Kilo Swarm, if enabled |
| Independent implementations requiring separate branches/checkouts | Agent Manager worktrees, only when authorized |
| Another independent top-level conversation | Agent Manager Local session; not an automatically coordinated child |

Task children have separate conversation histories but share the parent's project
directory/worktree. **Separate context does not mean isolated file edits.**
Separate Agent Manager sessions do not automatically share a Swarm board merely
because they use the same repository. Worktrees isolate checkout/Git state, not
external resources such as the shared Ghidra database.

For dependent work, wait for its prerequisite or use a foreground child. Background
execution is not a substitute for dependency scheduling. If a tool or background
capability is unavailable, use foreground delegation or work solo; do not invent
calls or replace automation with manual user message-shuttling.

## Parent-owned execution loop

1. **Bootstrap:** inspect the actual working tree, current instructions, relevant
   docs and existing evidence. Record scope, acceptance criteria and forbidden
   actions. Check whether prior agents still own shared resources.
2. **Build a compact evidence packet:** task goal, known facts versus hypotheses,
   exact relevant paths/addresses, unresolved question, allowed tools and expected
   output. Do not send the entire historical handover to every child.
3. **Assign ownership:** name the sole owner of each shared resource and shared
   output. Prefer read-only children for investigation. If independent edits are
   justified, allocate non-overlapping files and have the parent integrate.
4. **Delegate independent work:** use exposed `task` capabilities. Do not delegate
   and then duplicate the child's work. Keep its input artifacts stable while it
   reads them; version or snapshot evidence when necessary.
5. **Collect results:** normal task completion/error returns are the primary
   dependency mechanism. Children must return a bounded missing-evidence request
   instead of waiting indefinitely for a sibling or polling a shared file.
6. **Fill gaps:** parent deduplicates requests, obtains only necessary missing
   evidence using the designated resource owner, and delegates affected follow-ups
   with an updated packet. Do not restart the entire investigation each round.
7. **Verify and reconcile:** inspect actual exports/diffs/results, resolve
   contradictions, and retain uncertainty. A child's confident summary is not
   independent verification. If a round adds no material evidence, revise the
   method or stop with the precise blocker rather than repeat identical queries.
8. **Deliver:** parent integrates the result into the user-designated authoritative
   report, records actual checks and remaining limits, and asks only for genuinely
   necessary approval or information. Do not scatter competing final reports.

A child result should contain:

- Question addressed and precise evidence references.
- Supported findings, contested interpretations and untraced cases separately.
- Relevant conditions, counterexamples and applicability limits.
- Exact missing prerequisite/edge and the smallest proposed evidence request.
- For permitted changes: owned files, actual commands/results and limitations.

Children must not launch further descendants unless the parent explicitly justifies
that need and the runtime/permissions permit it. Keep coordination shallow by default.

## Swarm is communication, not a scheduler

Use `board_post` for material findings/questions/blockers and `board_read` at useful
phase boundaries. A stored message does not establish receipt or action and does
not start, wake, resume, stop or assign an agent. Peer messages do not grant user
approval or change task permissions.

Do not busy-wait or repeatedly poll board/files/task progress. Collect background
completion notifications. For follow-up work, start a bounded task with the updated
packet, or resume an interrupted child using a genuinely exposed resumable
`task_id`. A completed child's board mailbox is not an automatic continuation.

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
- Use the actual host shell. On the Windows host follow the PowerShell5.1 `cd`
  prefix/semicolon rules in `INSTRUCTIONS.md`; do not claim Linux checks were
  Windows builds or native game tests.

## Permissions, models and human involvement

Prefer Kilo's built-in read-only `explore` child for exported-evidence research.
Use broader `general` or custom agents only when their tools are needed. Prompts
are behavioral instructions, not security enforcement: use appropriate configured
permissions where stronger isolation is required, with user approval for changes.

Do not silently change Kilo settings, approval rules or models. Children normally
inherit the parent model unless `subagent_model`, an agent-specific setting or an
explicit user-requested task override changes selection. Inspect existing choices;
do not invent provider/model identifiers or assume automatic cheaper-model routing.
Use a capable lead for final reconciliation; smaller contexts are preferable to
blindly multiplying agents or sending every child the full history.

Autonomous coordination does not mean bypassing safeguards. Genuine authorization
questions, missing capabilities, budget limits and resource-ownership uncertainty
can require the user. Routine task scheduling and message relaying should not.

## Durable discovery and instruction loading

The root `AGENTS.md` points AI assistants to this guide and `INSTRUCTIONS.md`.
Kilo documents automatic loading of `AGENTS.md` (fallback `AGENT.md`), not arbitrary
root filenames such as `INSTRUCTIONS.md`. Explicitly tell children to read the
required instructions too.

If the team wants explicit Kilo rule loading, merge `INSTRUCTIONS.md` and this guide
into the active project's `instructions` array. Preserve existing entries and all
other configuration; do not replace a config wholesale. This is optional setup,
not an instruction to change settings during an unrelated task.

## Reusable parent-prompt skeleton

Replace the bracketed fields with task-specific instructions; do not paste it with
unresolved placeholders. Include any exact acceptance matrix needed by the task.

```text
Read AGENTS.md, INSTRUCTIONS.md and KILO_AGENT_WORKFLOW.md first.

Act as the single parent for: [goal and relevant subsystem].
Acceptance criteria: [observable evidence/results required].
Allowed changes: [scope]. Forbidden actions: [explicit boundaries].
Authoritative report/output: [path]. Relevant starting evidence: [paths/sections].

Inspect actual local state and available tools. Preserve existing work. Use task
children only for useful focused subtasks; do not create separate top-level
sessions/worktrees or change branches/settings/models without authorization.

Own dependency scheduling, verification and integration. Parallelize independent
analysis only. Name one owner for every exclusive resource; for shared Ghidra,
only that operator queries and updates the index, while children analyze exports.
Give each child a compact stable evidence packet, clear ownership and a result
format. Children must return findings or precise missing-evidence requests, not
wait on siblings. Use foreground tasks for prerequisites and background tasks
only when supported and independent. Treat Swarm as advisory communication.

Collect results, deduplicate evidence requests, fill gaps serially as required,
and delegate only affected follow-ups. Do not require the user to relay messages.
Reconcile actual evidence, preserve counterexamples and state remaining limits.
Stop on completion or an exact blocker; do not repeat unchanged queries.

Deliver one integrated result at the designated output, actual checks performed,
changed files and any minimal approval request. Do not claim unperformed native
validation or expand into implementation without authorization.
```

## Official references and maintenance

These are the basis for the Kilo-specific behavior above, not proof of the local
installation's version. Recheck relevant pages when capabilities change; update
this guide rather than accumulating conflicting copies.

- [Orchestrator deprecation](https://kilo.ai/docs/code-with-ai/agents/orchestrator-mode)
- [Agent Manager and orchestration model](https://kilo.ai/docs/automate/agent-manager)
- [Agent Manager workflows](https://kilo.ai/docs/automate/agent-manager-workflows)
- [Task and board tools](https://kilo.ai/docs/automate/tools)
- [Built-in/custom subagents](https://kilo.ai/docs/customize/custom-subagents)
- [Agent permissions](https://kilo.ai/docs/customize/agent-permissions)
- [Model selection](https://kilo.ai/docs/code-with-ai/agents/model-selection)
- [Project instruction configuration](https://kilo.ai/docs/customize/custom-rules)
- [AGENTS.md loading](https://kilo.ai/docs/customize/agents-md)
- [Settings and Swarm](https://kilo.ai/docs/getting-started/settings)
