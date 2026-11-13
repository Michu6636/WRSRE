# AI agent entry point

Start here, then read [INSTRUCTIONS.md](INSTRUCTIONS.md) and the relevant parts
of [KILO_AGENT_WORKFLOW.md](KILO_AGENT_WORKFLOW.md). Read the user's task and
subsystem documentation before acting; these files do not grant additional
permissions or override session instructions.

## Before you act

1. Check the current branch, working tree, actual host/shell, available tools,
   task scope and acceptance criteria. Preserve existing user changes. Do not
   assume Windows paths, Ghidra, the game or an agent/connector is present.
2. Find the designated current report, index and **raw evidence**. A report,
   agent summary or index status is a lead, not independent verification.
   Check whether cited scratch files are actually accessible before relying on
   or requesting them. Follow the claim and counterexample gates in the workflow.
3. Do the permitted work yourself when practical. If delegation is available,
   use one accountable parent and focused children; do not invent capabilities
   or make the user relay routine messages. A single designated operator owns
   each Ghidra project, including GUI and headless access.
4. When the next pass adds no new evidence or repeats a contradiction, stop:
   inspect the source, change the question or report the precise blocker. Do
   not keep appending summaries, guessing offsets or seeking a convenient hook.
5. Report what was actually checked, what changed and what remains unknown.
   Research, runtime observation, implementation, build, deployment and Git
   operations have separate authorization; do not infer one from another.
