# Researching other mods' DLLs for Almanac integration

**Status:** Proposed read-only investigation. No third-party mod DLL has yet been supplied or analyzed for this task. No code, hook, game change, execution of a third-party DLL, commit, or redistribution is authorized by this plan alone.

## Why this may help

A mod that adds a global sidebar button, replaces native panel content, or integrates a new page may show how another developer approached the exact hard part of the Almanac. Its DLL can provide useful **evidence of an approach**, even without source code. The game engine's `C3DDLL64.dll`, which was examined to understand native controls, is not a substitute for examining a relevant mod's own DLL.

Promising questions include:

- Which game or loader interfaces does the mod use? Does it register a UI service, hook an existing function, intercept a virtual method, or replace content another way?
- How does it detect the active page, position a button, handle selection and input, and return to native content?
- Does it suppress or replace vanilla rendering? If so, where are control flow, cleanup, and output values handled?
- How does it protect against game-version changes, relocate instructions, handle other mods, and avoid corrupting state?

A related mod may not solve all of these. A DLL may be optimized, stripped, packed, rely on another component or a different game version, or use an approach that is unsafe or incompatible here. Finding a hook or address is a **lead**, not evidence that it can be copied into WRSRE safely.

## What to provide

For each relevant mod, provide the DLL file itself through the workspace upload or another accessible path. If available, also provide:

- Mod name, author, version, and where it came from; any accompanying README, license, configuration, and dependency/loader information.
- Game version the mod supports and a short description of what it does, particularly a button or content-replacement behavior relevant to the Almanac.
- Any additional DLLs it depends on, when known. Screenshots or videos are optional and not required to begin static analysis.

A Windows path alone is not readable from the Linux workspace unless the file has been shared. The user does not need to operate Ghidra or Visual Studio for the initial handoff. The game does **not** need to be running for static inspection.

## Proposed bounded workflow

1. **Identify and preserve provenance.** Record filename, source, file size, SHA-256, binary architecture, version and available license. Keep the original unchanged; distinguish each mod and dependency.
2. **Triage without execution.** Determine whether the DLL is native or managed, inspect PE headers, exports, imports, strings, and dependencies with tools actually available. For managed assemblies, inspect metadata/decompiled output if suitable tools are available; for native code, use disassembly and, when useful, Ghidra. Do not run or load the DLL into the game merely to inspect it.
3. **Follow one relevant path.** For a mod likely to affect the sidebar, trace initialization, any loader/API registration, and the specific hook or call path connected to its button or content. Capture actual instruction/function boundaries and show how addresses map to its supported build; do not infer behavior from strings alone.
4. **Compare with WRSRE.** Identify what is a transferable design idea, what depends on that mod's dependencies/version, and what would require independent verification for our loader and UI. Check normal and alternate paths, cleanup, output values, input/selection, and return behavior as applicable.
5. **Report observations, not assumptions.** Preserve short raw excerpts and a concise report in a non-ignored `export/almanac-evidence/` subfolder: file hashes, tool/version, exact provenance, observations, confidence, limits, and concrete next steps. Keep source disassembly separate from commentary; no ZIPs.

Serialize access to any shared Ghidra project. Stop and report missing tools, unavailable code, or ambiguous behavior rather than guessing. Runtime testing, debugging, hook installation, or implementation require separate approval.

## Handling third-party material

- Treat a DLL as third-party material: respect applicable license and redistribution restrictions. Inspection does not automatically grant permission to copy code, assets, or redistribute the binary.
- Keep the original binary out of Git by default; this repository ignores `*.dll`. Do not force-add or publish it without permission and an appropriate basis. Preserve only appropriate bounded evidence and attribution, avoiding sensitive or unnecessary content.
- Prefer independently implementing a compatible idea using WRSRE's own interfaces and documented observations. Check any proposed source or asset reuse against the applicable license and the project owner's decision.

## What a successful result looks like

A short report should explain **what the mod demonstrably does**, what might transfer to our Almanac, and which of button placement, content suppression, cleanup, return behavior, or compatibility it actually answers. It should also state what remains unproved and cite the raw evidence. If the DLL offers no useful lead, say so and move on rather than repeating a broad search.
