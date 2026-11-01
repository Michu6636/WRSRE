# AI agent entry point

Before working in this repository or writing prompts for another team agent:

1. Read `INSTRUCTIONS.md` for project, native-tool and environment rules.
2. Read `KILO_AGENT_WORKFLOW.md` for Kilo coordination and prompt-writing guidance.
3. Read the relevant subsystem documentation and the user's current task limits.

Default to one parent session coordinating focused task subagents, not manually
managed peer sessions. Parallelize independent analysis; serialize shared native
resources such as Ghidra. The parent owns dependencies and final integration.
Native contracts must pass the claim-level evidence gates in
`KILO_AGENT_WORKFLOW.md`; verified bytes/index labels do not verify receiver
semantics, units or runtime applicability. This applies to parent reviews too.

These files do not grant approval to change configuration, branches, runtime
behavior, billing, hooks or deployment. Preserve explicit user and session limits.
