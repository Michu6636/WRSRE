# Instructions for Workers & Resources Mod Development & Reverse Engineering

## Agent coordination and prompt writing

Before planning delegated work or writing prompts for other agents, read
[`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md). Use one accountable Worker
parent; for every substantive deliverable, consult at least one independent,
read-only child Reviewer before calling it fully reviewed. Use distinct review
focuses for material changes and follow the bounded repair loop in that guide.
Do not make the user relay routine dependencies. Shared Ghidra queries remain
serialized. The root `AGENTS.md` is the discovery entry point; workflow guidance
does not expand the user's task authorization.

## Mandatory small-chunk writes

If EITHER the content an AI agent plans to write OR the destination file is
long, the agent MUST use small, coherent chunks and verify each write before
continuing. This applies to code, scripts, documentation and reports, including
appends and edits to an already-long file. A short edit to a long file must stay
a small targeted edit; never replace the whole file to make that change.

Preserve existing content. Split long additions at logical boundaries (for
example, a heading, paragraph, table row or bounded verbatim artifact slice).
Read back the affected region after each operation. Do not hide a long write in
one large patch, shell command, generated script or tool payload. After a timeout,
inspect disk state before retrying to avoid duplicate or lost content.

Chunking changes delivery size, not completeness: retain all evidence, source,
results and reasoning, then check the assembled result. No arbitrary byte cap or
assembly script is required. Short writes to short files can use a normal single
operation. Follow the detailed checkpoints in KILO_AGENT_WORKFLOW.md.

## Project Overview

This repository contains plugins and mods for Workers & Resources: Soviet Republic.
Check the actual checkout, branch, host, shell and available files; historical
Windows paths are examples, not proof of local access. Preserve existing work.

- Historical target: SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD. Verify the
  actual binary/project identity for the task; an export or reported hash alone
  does not authenticate the local executable.
- Runtime base is dynamic (ASLR). Use RVAs for runtime addressing; never
  hardcode absolute pointers. Label preferred-image VAs and file offsets clearly.

---

## 1. Ghidra Headless Tools

First check whether the pre-analyzed project, executable, wrapper and JDK exist
on the actual host. They may be on a separate Windows machine and inaccessible
from this checkout. Do not claim a query was run unless it actually was. On a
Windows workspace where the existing wrapper is present and configured, it sets
the JDK and points at the project and scripts:

scratch\ghidra_run.bat <Script.java> <out_file> [args...]

What it expands to:
analyzeHeadless <TesmioLoader>\ghidra\proj soviet -process SOVIET64.exe
-noanalysis -scriptPath "<TesmioLoader>\tools\ghidra;<workspace>\scratch"
-postScript <Script.java> [args...]
NOTE: ONE -scriptPath flag only - a second -scriptPath flag REPLACES the
first instead of appending. Multiple dirs go in one flag, ';' separated.

MCP is optional and does not replace these scripts. For bethington GhidraMCP
v7, the Python bridge is separate from the GUI plugin or Java headless backend;
starting the bridge or changing its transport does not itself load Ghidra. Other
MCP implementations may combine these components. Check the selected tool's
version and architecture before making assumptions. Do not install, reconfigure
or expose a server merely because a query timed out; identify the operation and
backend first. Restrict approved services to the intended local interface and
preserve unrelated configuration.

### Available Ghidra Scripts:

Tracked Java query scripts are in `tools/ghidra/`; see its README for the
actual inventory and argument contracts. Historical wrappers may reference
`Xrefs.java` or `ListFuncs.java` from a separate workspace; verify each file
before invoking it. Cached exports may be in `docs/ghidra-exports/`, a
user-designated tracked directory, or ignored `scratch/` on the operator's host.

- Decompile: `scratch\ghidra_run.bat DecompAt.java <out_file.c> <preferred_VA> [preferred_VA ...]`
- Cross-references (if locally available): `scratch\ghidra_run.bat Xrefs.java <out_xrefs.txt> <preferred_VA>`
- Disassemble one range: `scratch\ghidra_run.bat DisasmMany.java <out_disasm.asm> <start_VA:end_VA>`
- Enumerate functions (if locally available): `scratch\ghidra_run.bat ListFuncs.java <out.txt> <start_VA> <end_VA>`

### Operational rules (each cost a failed run the first time):

- Use the `.java` scripts. The `.py` copies beside them need PyGhidra and fail
  with "Ghidra was not started with PyGhidra". Read a `.py` for its argument
  contract, run the `.java`.
- ONE RUN AT A TIME. `analyzeHeadless` takes an exclusive lock on the project;
  two concurrent invocations die with
  "LockException: Unable to lock project!". Never run Ghidra queries in
  parallel with each other. (See "Serialization across agents" below - this
  rule applies to _all_ agents and Agent Manager sessions, not just your own.)
- These scripts accept preferred-image virtual addresses (VAs), NOT raw file
  offsets or RVAs. The project image base is 0x140000000: compute VA = image
  base + RVA (e.g. RVA 0x997590 -> VA 0x140997590). Do not concatenate prefixes
  as a general conversion rule. Runtime VA = actual loaded base + RVA; mapping
  an RVA to a raw PE file offset requires the containing section's headers.
- Verify the JDK and Ghidra versions and the wrapper's paths on the actual
  host. One historical JDK location was `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot`;
  it may not exist here. Missing Java can cause the wrapper to hang. Do not
  alter a user's wrapper or environment without task authorization.
- The decompiler has a ~120 s budget per function. `FUN_14028c770` (a world
  writer) and `FUN_14030d1a0` (the per-frame update) time out; use
  DisasmMany on a range for those.

### Scripts are read-only (know this before you plan a workflow)

The intended `DecompAt`, `Xrefs`, `DisasmMany`, and `ListFuncs` query scripts
do not intentionally rename or retype program data. Treat the Ghidra project as
a **read-only oracle**: record interpretations in `docs/ghidra-index/`, not in
the database. This is a workflow restriction, not an OS-enforced read-only
boundary: verify the script and options, and account for project locks, metadata
or cache writes. Back up both `.gpr` and its matching `.rep` and use a separate
copy for untested tools; only copy after confirming the project is closed and
unlocked. Do not assume an MCP tool allowlist prevents filesystem writes.

### Knowledge Base (READ THIS FIRST, WRITE HERE AFTER)

Ghidra is a **serialized, scarce resource**. Every query consumes a slot that
no other agent can use concurrently. The index is what lets you amortize that
cost across sessions instead of re-investigating the same function on every
pass. Treat it as mandatory infrastructure, not an optional nicety.

**Before ANY `ghidra_run.bat` call:**

1. Grep `docs\ghidra-index\` for the RVA or the function name.
2. Search the relevant in-repository subsystem docs and the user-designated
   report. Historical `docs\17-ui-and-integration.md` and `Tesmio Mods\docs\01-16`
   may be on another host; verify availability instead of assuming they exist.
3. If the index entry is `Status: verified` -> reuse its evidenced claims and
   cached exports. **DO NOT re-run Ghidra for the same available evidence.**
   This label does not verify every inferred purpose, receiver, unit or runtime
   application; follow the claim-level evidence gates in KILO_AGENT_WORKFLOW.md.
4. If the entry is `Status: partial` -> inspect caches, then query only the gap.
5. If the entry is `Status: contested` -> first test the conflicting claims
   against existing raw evidence. Query only a genuinely missing edge/range;
   a disagreement in interpretation does not itself justify another export.
   Preserve both views and record the evidence-based resolution.

**After ANY `ghidra_run.bat` call:**

- Append or update the entry in `docs\ghidra-index\` using the schema in
  `docs\ghidra-index\README.md`. At minimum record: RVA, name (recovered or
  `FUN_<rva>`), signature, 1-2 sentence purpose, callers, data refs, artifact
  path, notes, status.
- If the run contradicted an existing entry, set `Status: contested` and add a
  `Note:` explaining the contradiction. Do not silently overwrite - the next
  agent needs to know there was a disagreement.
- If a run failed (timeout, LockException, bad address form), record the
  failure and the workaround in the entry's `Notes:` so the next agent does
  not repeat it.

### Cache rule (do not burn a serialized slot for a cached result)

`DecompAt` writes its output to the `out_file` you pass. Use a deterministic
name so results are cacheable:

- Decompile: `scratch\out\<rva>_decomp.c`
- Xrefs: `scratch\out\<rva>_xrefs.txt`
- Disasm: `scratch\out\<start>_<end>_disasm.asm`
- ListFuncs: `scratch\out\<start>_<end>_funcs.txt`

Before re-running, check whether the output actually exists on the current
host and read it. Match its provenance to the executable/project version and
requested range; timestamps alone do not establish identity or freshness. If
identity is unknown, mark it unverified. Re-query only when existing evidence
cannot answer the question or the project/version changed, following the
single-operator rule; interpretation disputes alone do not require a rerun.
`scratch/` is ignored by Git: an agent's completion summary or commit does not
publish it. For review, preserve appropriately sized, permitted raw excerpts
and their provenance in a tracked evidence location, or request the existing
artifact from its owner. Do not publish the executable, project or large exports
without authorization.

### Serialization across agents (Agent Manager / parallel sessions)

The "ONE RUN AT A TIME" rule above is not just about your own calls. Kilo Code
Agent Manager sessions run in parallel; if two sessions each invoke
`ghidra_run.bat`, the second dies with `LockException`. Therefore:

- **Route all Ghidra calls through a single designated "Ghidra agent"** (the
  orchestrator). Parallel analysis agents must consume the orchestrator's
  exported artifacts (`scratch\out\*`, `docs\ghidra-index\`) and must NOT
  invoke the wrapper themselves.
- If an out-of-band call is genuinely unavoidable, acquire
  `scratch\.ghidra.lock` (create-exclusive) before running, and release it
  after. If the lock exists, wait or fail loudly - do not delete another
  agent's lock.
- Never run two `analyzeHeadless` invocations concurrently, even from separate
  agents. Also do not operate on the same project concurrently through a Ghidra
  GUI, MCP backend or another headless process. Treat locks as exclusive; never
  kill processes or remove another owner's lock. Separate copies require
  explicit ownership and do not relax the serialized-query rule.

Findings from the UI/integration investigation (panel builders, the vehicle
list, GetString retagging, save-format): workspace `docs\17-ui-and-integration.md`.

---

## 2. Architecture & C++ Coding Standards (CRITICAL)

To prevent bloat and crashes as the mod grows, you MUST adhere to these boundaries:

### A. Plugin Isolation & Services

- **NO CROSS-PLUGIN LINKING:** Never `#include` a `.cpp` or internal `.h` from another plugin.
- **USE THE API:** Plugins MUST communicate exclusively via the shared headers in `src/` (e.g., `currency_api.h`, `ui_panels_api.h`).
- **FALLBACKS:** When consuming a service (e.g., `H->consume(TSM_SERVICE_CURRENCY)`), always handle `NULL`. If a plugin is missing, your code must degrade gracefully (e.g., hide the UI row, do not crash).
- **NO GLOBAL STATE BLEEDING:** Each DLL has its own global state. Do not try to share raw pointers across DLL boundaries unless they are opaque handles wrapped in an API function.

### B. Memory Safety & Hooking

- **MEMORY ACCESS:** Use `ReadablePtr(ptr, size)` before approved reads, but
  readability/exception handling does not prove object type, lifetime or offset.
  Unverified fields stay unavailable outside explicitly approved diagnostics;
  a `try/catch` or heuristic is not a substitute for native layout evidence.
- **HOOK SAFETY:** The project's 14-byte inline patch requires a sufficient
  contiguous span of WHOLE instructions, not a 14-byte first instruction.
  Verify exact bytes, boundaries, incoming edges, relocation of RIP-relative and
  relative-control-flow instructions, ABI and receiver/event semantics. A short
  instruction/prologue does NOT automatically justify patching a caller's
  `call rel32`; that is a separate design requiring proof and authorization.
  Inspect the actual helper: it accepts a whole-instruction stolen span of at
  least 14 bytes and pads the remainder, but copies instructions without fixing
  RIP-relative or relative-control-flow operands. A mid-function detour also
  needs live-register/flags, stack, cleanup and output analysis. Always provide
  a verified `expect` array for an approved patch.
- **UNVERIFIED HOOKS:** Stop at the exact missing evidence. Do not implement or
  compile a speculative hook merely because it would default to disabled.
  Implementation, logging-only runtime validation and deployment each require
  the task's authorization; static evidence alone grants none of them.

### C. Performance (Zero-Allocation Rule)

- **RENDER/TICK LOOPS:** Never use `std::string`, `std::vector`, `new`, or `malloc` inside a per-frame hook, render hook, or the `CheckoutAffordabilityGate`.
- **PRE-ALLOCATE:** Use static arrays or pre-allocated buffers (e.g., `char buf[128]`) for formatting text.
- **STRING FORMATTING:** Use `_snprintf_s` or `_snwprintf_s` instead of `std::stringstream` or `std::format` in hot paths.

### D. UI/UX Specific Rules

- **FLOW-BASED UI ONLY:** Do not use absolute X/Y screen coordinates for UI elements.
- **NATIVE ROWS:** The existing building-window widget contract appends from
  `window+0x250`; use it only for supported, verified window types. Do not
  assume a global sidebar or unrelated window has the same structure.
- **DPI SCALING:** Always multiply pixel values by the game's DPI scalar (found at `0x992088`).

---

## 3. Critical Modding Rules

1. INI Encoding: ALL .ini configuration files must be saved in UTF-8 WITHOUT BOM.
2. Threading: Never call Direct3D 11 texture mapping (TextureAccessOpen) outside of render hooks.
3. Custom Text IDs: Any private text IDs minted for UI must start at 1,000,000 or higher.

For new game-system research, start with `KILO_AGENT_WORKFLOW.md`'s native
acceptance gates and the relevant in-repository docs. A historical
`docs/03-reverse-engineering.md` may not be present; verify before citing it.

---

## 4. Shell & Tooling Rules

### Identify the actual shell

The repository can be opened in Windows or a remote Linux workspace. Check the
host, current directory and tool semantics before choosing commands; do not
assume this checkout can access a separate Windows machine. Use the tool's
working-directory option where available and do not assume shell state persists.

In **Windows PowerShell 5.1**, `&&` is not supported; use PowerShell commands
such as `Get-ChildItem`, `Select-Object` and `Select-String`, and choose error
handling that preserves failure status. Quote paths safely. PowerShell 7, cmd
and bash have different syntax. A previously used checkout path was
`C:\Users\EricM\Downloads\WRSRE`; verify it rather than hardcoding it.

### Content search: prefer Select-String

- Pass ONLY a plain `queries` string array to `search_codebase`; when in doubt, re-verify with Select-String.
- Recipes:
  `Select-String -Path 'plugins\private*shops\private_shops.cpp' -Pattern 'IsRetailType'`
  `Get-ChildItem plugins -Recurse -Include *.cpp,_.h | Select-String -Pattern 'monthlyDividend'`
  `... | Select-Object -First 40 | ForEach-Object { $_.LineNumber.ToString() + ': ' + $_.Line.Trim() }`

### Output discipline (terminal mangles tables)

- Use `-Name`, `-ExpandProperty FullName`, or build strings.
- Numbered line-range read:
  `$lines = Get-Content 'path\file.cpp' -Encoding UTF8`
  `for ($i=100; $i -lt 150; $i++) { '{0,4}: {1}' -f ($i+1), $lines[$i] }`
- Keep greps targeted (`-First N`). If "Command completion could not be observed" occurs, re-run in a simpler form.

### Workspace map

- Plugin sources: `plugins\<name>\<name>.cpp`; shared headers: `src\`.
- `docs/` contains multiple subsystem reports, an index and cached exports;
  use `docs/README.md` and inspect the current checkout. Historical `Tesmio Mods`
  documentation and game files may be on a different host.
- A historical game path was `C:\Workers Resources Soviet Republic`; confirm
  the actual installation and version before relying on its files.
- Build: `cmd /c build.bat <plugin_name>` or `cmd /c build.bat` from workspace root. Verify DLL `LastWriteTime` under `build\plugins`.
- INI BOM check: `[System.IO.File]::ReadAllBytes($p)[0..2]` must NOT be `239 187 191`.

### Docs read/write map

- **Read:** the existing task-designated report, `docs/README.md`, the relevant
  subsystem documents, `docs/ghidra-index/` and available raw evidence. Treat
  external or historical paths as optional until checked.
- **Write:** update the designated current report and affected function index
  entries. Correct misleading consequential claims where readers encounter them;
  preserve a concise supersession note and invalidate dependent conclusions.
  Do not append a new summary solely to restate unchanged evidence. Do not edit
  a separate external knowledge base unless the task authorizes it.

### Verifying bug reports

- Report line numbers are ALWAYS stale - locate by the verbatim snippet or symbol name.
- Corroborate consequential field/layout semantics with independent native
  evidence (e.g. writer plus caller/parser), not two summaries of one export.
  Cite each source and the inference it supports; mark missing corroboration
  unresolved rather than inventing support. A decoded operand can prove a byte
  displacement without proving object type, ownership, physical units or purpose.
- Trace the FULL impact chain before verdicts.
- Fix conventions: new `// vN.N updates:` block -> `info->version = "N.N"` -> `Logf(...)` -> edit -> `build.bat <plugin>` -> verify.
