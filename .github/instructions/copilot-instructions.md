# GitHub Copilot Instructions for WRSRE

Before acting on any task in this repository:

1. Read `AGENTS.md`, `INSTRUCTIONS.md`, and `KILO_AGENT_WORKFLOW.md` in full.
2. Respect strict task authorization: research/audit is read-only; edits, `build.bat`, DLL deployment, `SOVIET64.exe` launches, and Git operations each require explicit authorization.
3. Follow all WRSRE Ghidra serialization rules (`.java` scripts only, single operator, check `docs/ghidra-index/` and `docs/ghidra-exports/` first) and C++ rules (plugin isolation, `ReadablePtr`, 14-byte whole-instruction hook spans + `expect`, zero-alloc render/tick hooks, flow UI + DPI `0x992088`, text IDs >= 1,000,000, UTF-8 no-BOM `.ini`, small-chunk writes).
4. For any substantive deliverable, invoke a read-only reviewer subagent against the frozen diff and raw evidence before declaring it reviewed.
