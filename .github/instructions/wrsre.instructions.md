---
applyTo: "**"
---
# GitHub Copilot Instructions for WRSRE

AGENTS.md is already loaded. Read INSTRUCTIONS.md before Ghidra/C++ work and KILO_AGENT_WORKFLOW.md before delegating. Do not duplicate them here.

1. Respect strict task authorization: research/audit is read-only; edits, `build.bat`, DLL deployment, `SOVIET64.exe` launches, and Git operations each require explicit authorization.
2. Follow WRSRE Ghidra serialization (`.java` only, single operator, check `docs/ghidra-index/` and `docs/ghidra-exports/` first) and C++ rules (plugin isolation, `ReadablePtr`, 14-byte whole-instruction spans + `expect`, zero-alloc hot paths, flow UI + DPI `0x992088`, text IDs >= 1,000,000, UTF-8 no-BOM `.ini`, small-chunk writes).
3. For any substantive deliverable, invoke a read-only reviewer against the frozen diff and raw evidence before declaring it reviewed. Use two focuses for material changes.
