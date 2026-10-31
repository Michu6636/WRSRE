# Instructions for Workers & Resources Mod Development & Reverse Engineering

## Agent coordination and prompt writing

Before planning delegated work or writing prompts for other agents, read
[`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md). Use one parent session to
coordinate task subagents by default; do not make the user relay dependencies
between independent sessions. Shared Ghidra queries remain serialized. The root
`AGENTS.md` is the discovery entry point; workflow guidance does not expand the
user's task authorization.

## Project Overview

You are developing plugins and mods in this workspace (C:\Users\EricM\Downloads\WRSRE).

- Target Binary: SOVIET64.exe (v1.1.1.9, PE timestamp 0x6A3EB6AD).
- Base Address: Dynamic (ASLR is ON). Work strictly with RVAs. Never hardcode absolute pointers.

## 1. Ghidra Headless Tools

You have full access to the pre-analyzed Ghidra project located in TesmioLoader.
There is NO `$ghidraRunner` on PATH. Use the working wrapper instead - it sets
the JDK on PATH and points at the project, the tool scripts and this
workspace's `scratch\` scripts:

scratch\ghidra_run.bat <Script.java> <out_file> [args...]

What it expands to:
analyzeHeadless <TesmioLoader>\ghidra\proj soviet -process SOVIET64.exe
-noanalysis -scriptPath "<TesmioLoader>\tools\ghidra;<workspace>\scratch"
-postScript <Script.java> [args...]
NOTE: ONE -scriptPath flag only - a second -scriptPath flag REPLACES the
first instead of appending. Multiple dirs go in one flag, ';' separated.

### Operational rules (each cost a failed run the first time):

- Use the `.java` scripts. The `.py` copies beside them need PyGhidra and fail
  with "Ghidra was not started with PyGhidra". Read a `.py` for its argument
  contract, run the `.java`.
- ONE RUN AT A TIME. `analyzeHeadless` takes an exclusive lock on the project;
  two concurrent invocations die with
  "LockException: Unable to lock project!". Never run Ghidra queries in
  parallel with each other.
- Addresses for these scripts are FILE-BASED: the project loads SOVIET64.exe
  at base 0x140000000, so pass `0x140` + RVA. RVA 0x997590 -> 0x140997590.
  An address without the 0x140 prefix reads as "No function found containing".
- Java lives at C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot.
  Without a JDK on PATH, analyzeHeadless prints "The 'java' command could not
  be found" and then hangs on a keypress.
- The decompiler has a ~120 s budget per function. `FUN_14028c770` (a world
  writer) and `FUN_14030d1a0` (the per-frame update) time out; use
  DisasmMany on a range for those.

### Available Ghidra Scripts:

- Decompile Function: `scratch\ghidra_run.bat DecompAt.java <out_file.c> <0x140RVA1> [0x140RVA2...]`
- Find Cross-References: `scratch\ghidra_run.bat Xrefs.java <out_xrefs.txt> <0x140RVA>`
- Disassemble Instruction Bytes: `scratch\ghidra_run.bat DisasmMany.java <out_disasm.asm> <0x140start:0x140end>`
- Enumerate Functions in a range: `scratch\ghidra_run.bat ListFuncs.java <out.txt> <0x140start> <0x140end>`

Findings from the UI/integration investigation (panel builders, the vehicle
list, GetString retagging, save-format): workspace docs\17-ui-and-integration.md.

## 2. Architecture & C++ Coding Standards (CRITICAL)

To prevent bloat and crashes as the mod grows, you MUST adhere to these boundaries:

### A. Plugin Isolation & Services

- **NO CROSS-PLUGIN LINKING:** Never `#include` a `.cpp` or internal `.h` from another plugin.
- **USE THE API:** Plugins MUST communicate exclusively via the shared headers in `src/` (e.g., `currency_api.h`, `ui_panels_api.h`).
- **FALLBACKS:** When consuming a service (e.g., `H->consume(TSM_SERVICE_CURRENCY)`), always handle `NULL`. If a plugin is missing, your code must degrade gracefully (e.g., hide the UI row, do not crash).
- **NO GLOBAL STATE BLEEDING:** Each DLL has its own global state. Do not try to share raw pointers across DLL boundaries unless they are opaque handles wrapped in an API function.

### B. Memory Safety & Hooking

- **FAIL-SAFE MEMORY ACCESS:** When reading game memory, use `ReadablePtr(ptr, size)` before dereferencing. If an offset is unverified, wrap it in a `try/catch` or heuristic check.
- **HOOK SAFETY:** Inline hooks require at least 14 stolen bytes. If a prologue is <14 bytes, patch the caller's `call rel32` instead. Always provide an `expect` array to verify prologue bytes before patching.
- **UNVERIFIED HOOKS:** If you cannot verify a function's layout via Ghidra, compile the hook but disable it by default (`stolen = 0`, `expect = NULL`) and document how to verify it.

### C. Performance (Zero-Allocation Rule)

- **RENDER/TICK LOOPS:** Never use `std::string`, `std::vector`, `new`, or `malloc` inside a per-frame hook, render hook, or the `CheckoutAffordabilityGate`.
- **PRE-ALLOCATE:** Use static arrays or pre-allocated buffers (e.g., `char buf[128]`) for formatting text.
- **STRING FORMATTING:** Use `_snprintf_s` or `_snwprintf_s` instead of `std::stringstream` or `std::format` in hot paths.

### D. UI/UX Specific Rules

- **FLOW-BASED UI ONLY:** Do not use absolute X/Y screen coordinates for UI elements.
- **NATIVE ROWS:** Append UI rows using the native builder's bottom edge (`window+0x250`). Write back to `+0x250` to advance the cursor so native elements don't overlap your custom widgets.
- **DPI SCALING:** Always multiply pixel values by the game's DPI scalar (found at `0x992088`).

## 3. Critical Modding Rules

1. INI Encoding: ALL .ini configuration files must be saved in UTF-8 WITHOUT BOM.
2. Threading: Never call Direct3D 11 texture mapping (TextureAccessOpen) outside of render hooks.
3. Custom Text IDs: Any private text IDs minted for UI must start at 1,000,000 or higher.

Whenever researching new game systems or writing code caves/hooks, refer to docs/03-reverse-engineering.md for methodology and safety rules.

## 4. Shell & Tooling Rules

### The shell is Windows PowerShell 5.1

- `&&` is a PARSE ERROR. Sequence with `;`. `find`, `head`, `ls`, `grep` do not exist - use `Get-ChildItem`, `Select-Object`, `Select-String`.
- Prefix EVERY command with `cd 'C:\Users\EricM\Downloads\WRSRE';` - do not assume cwd persists between calls.
- Command strings must be clean. A stray `"` or `)` inside an array element fails with parser errors at the CALL SITE. Use single quotes for all paths and strings; avoid embedding double quotes entirely.
- Batch independent commands as separate array elements in one call - they run sequentially and the results come back together.

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
- Root `docs\` holds ONLY 17/18/19. The RE knowledge base 01-16 is under `Tesmio Mods\docs\`.
- THE GAME IS INSTALLED at `C:\Workers Resources Soviet Republic`. `media_soviet\buildings_types\*.ini` are the real recipes.
- Build: `cmd /c build.bat <plugin_name>` or `cmd /c build.bat` from workspace root. Verify DLL `LastWriteTime` under `build\plugins`.
- INI BOM check: `[System.IO.File]::ReadAllBytes($p)[0..2]` must NOT be `239 187 191`.

### Verifying bug reports

- Report line numbers are ALWAYS stale - locate by the verbatim snippet or symbol name.
- Confirm every field/layout claim from >=2 independent sources.
- Trace the FULL impact chain before verdicts.
- Fix conventions: new `// vN.N updates:` block -> `info->version = "N.N"` -> `Logf(...)` -> edit -> `build.bat <plugin>` -> verify.
