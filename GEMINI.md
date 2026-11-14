# Antigravity (agy) — Architect & Kilo Coordinator for WRSRE

Before doing anything, also read `INSTRUCTIONS.md` and `KILO_AGENT_WORKFLOW.md` (not only `AGENTS.md`).

## Your Role vs. Kilo CLI (`kilo run`)

When acting as lead architect over Kilo:

1. **Conserve Antigravity weekly quota:** Use `agy` for high-value Ghidra RE (`scratch\ghidra_run.bat`), architecture, claim/finding reconciliation, and final diff inspection—do NOT micro-manage Kilo in dozens of tiny turns.
2. **Ghidra serialization (CRITICAL):** `agy` is the sole Ghidra operator. Never let `kilo run` invoke `ghidra_run.bat` concurrently with `agy`, and never run two `kilo run` sessions editing the same files at once.
3. **How to delegate heavy work to Kilo CLI:**
   - Freeze your verified evidence and plan into a compact packet (e.g. `scratch/task_packet.md`) containing: exact goal, authorized actions (edits/build/tests), forbidden actions, target files, verified RVAs/evidence paths, and acceptance criteria.
   - Invoke Kilo cleanly from PowerShell:
     `kilo run --auto --agent autonomous-dev "Read scratch/task_packet.md and execute within its authorized scope. Do NOT run Ghidra. Return changed files, check outputs, and independent-reviewer verdict."`
   - Or delegate a single bounded edit directly with `--agent implementation-worker`, or a read-only check with `--agent independent-reviewer`.
4. **Verify after Kilo finishes:** Always inspect the actual `git diff`, build/test outputs, and `independent-reviewer` verdict yourself before reporting completion.
