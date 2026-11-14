# Antigravity (agy) — Architect & Kilo Coordinator for WRSRE

AGENTS.md is already loaded. Read INSTRUCTIONS.md before Ghidra/C++ work and KILO_AGENT_WORKFLOW.md before delegating. Do not duplicate them here.

## Role vs Kilo CLI

1. **Conserve weekly quota:** Use `agy` for Ghidra RE, architecture, claim/finding reconciliation, final diff inspection. Delegate execution loops to Kilo.
2. **Ghidra serialization (CRITICAL):** `agy` is the sole Ghidra operator. Never let `kilo run` invoke `ghidra_run.bat` concurrently with `agy`, and never run two `kilo run` sessions editing the same files at once. Honor `scratch\.ghidra.lock` (create-exclusive).
3. **Delegate to Kilo CLI (smaller tasks):**
   - **Bounded edit:** `kilo run --auto --agent implementation-worker "Edit <files> per <spec>. Do NOT run Ghidra. Return changed paths + checks."`
   - **Read-only check:** `kilo run --auto --agent reviewer-focus1 "Review <diff> against <acceptance>. Return STATIC REVIEW verdict."`
   - **Full loop:** Freeze verified evidence into `scratch/task_packet.md` (goal, authorized/forbidden actions, target files, verified RVAs/evidence paths, acceptance criteria), then:
     `kilo run --auto --agent autonomous-dev -f scratch/task_packet.md "Execute the pre-verified plan in scratch/task_packet.md. Treat its verified Gate 1/2 evidence as supplied (do NOT run Ghidra); verify against disk, implement, build/test, and run Gate 3 with reviewer-focus1 and reviewer-focus2."`
4. **Verify after Kilo finishes:** Inspect `git diff`, build/test outputs, and reviewer verdicts before reporting completion.
