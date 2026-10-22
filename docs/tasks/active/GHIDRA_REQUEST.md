# Ghidra follow-up — DO panel target-array capacity above five (READ ONLY)

## Hard boundary

Investigation/reporting only. Do not implement findings. Do not modify, replace, reformat, create, truncate, or delete any source, INI, build, or documentation file. Return findings in the response only. Save analysis output and any temporary query scripts under `scratch\\`, never the repository root.

Target: `SOVIET64.exe` v1.1.1.9, PE timestamp `0x6A3EB6AD`. Report RVAs; Ghidra address = `0x140` + RVA. Run one headless invocation at a time; use bounded disassembly if decompilation times out.

## Mandatory Ghidra execution in the local WRSRE workspace

This request is intended for an agent running inside the Windows WRSRE project workspace. **Use its pre-analyzed Ghidra project; do not answer from pasted findings alone.** There is no `ghidraRunner` on PATH: invoke `scratch\\ghidra_run.bat <Script.java> <out_file> [args...]`, which configures the JDK and project. First verify that the wrapper and scripts are present in that local workspace. Use Java scripts, not adjacent PyGhidra `.py` copies; pass file-based addresses as `0x140` + RVA; run exactly one headless invocation at a time. Keep outputs and temporary read-only query scripts under `scratch\\`.

Use existing `DecompAt.java`, `Xrefs.java`, `DisasmMany.java`, and `ListFuncs.java` as appropriate. If those scripts cannot enumerate a needed allocator/reference set, use a temporary read-only Ghidra Java query under `scratch\\` and invoke it through the wrapper. Do not stop merely because `analyzeHeadless` is not on PATH. If the wrapper/project is genuinely inaccessible, quote the exact failed command and error; do not claim a search was completed. **No implementation, config, build, or documentation edits are authorized by this request.**

## Latest findings — do not repeat unless new contradictory evidence appears

- Native save `FUN_1401EDC40` serializes the DO vector and TaskCfg. Load `FUN_1401F0BE0` resizes the target vector, allocates TaskEntry (`0x18`) and TaskCfg (`0x198`), deserializes fields, resolves the target by saved index, and marks the DO dirty. Native TaskEntry/TaskCfg save-load reconstruction is therefore confirmed; no plugin post-load repair is needed for entries using this format.
- `FUN_1407496C0` is a per-frame DO UI handler with world/panel/input/gui state, **not** a safe callable add API. Reproduce the allocation/vector operation rather than calling it from the day callback.
- New TaskEntry begins with `+0x00=0`; reachability upkeep sets it to 1 or 0. Explicit remove frees/removes; reachability disable retains the entry.
- Load-only TaskCfg is `+0x04=1.0f`, `+0xC8=0`, `+0xCC=0.0f`.
- Type-5 storage requires road connection type 3; rail-specific type-6 DOs cannot reach it.
- Vector/planner/upkeep/save/panel loops iterate all entries, but the panel per-target control array at `panel+0x310+idx*4` has no bounds check. Native add-flow stops after five selections per session. The backing panel storage capacity remains unknown, so entries above five are still unverified.
- Each DO plans independently; duplicate orders from shared sources are confirmed.

## Remaining task: establish the target-count bound

The staged plugin now conservatively limits **new automatic additions** to five total entries per DO while preserving any existing/manual entries. Determine whether that limit can safely be raised.

1. Trace how the panel object addressed through the world panel table (reported around `world+0xCAC0`) is allocated, initialized, resized, and destroyed.
2. Establish whether `panel+0x310` is a fixed array, dynamically sized vector, or part of a larger allocation, and calculate its actual capacity in DWORD target slots.
3. Trace all `panel+0x310+idx*4` reads/writes in both road and rail panel builders, including initialization and any adjacent fields that could be overwritten.
4. Reconcile the native add-flow break at five selections with the total target count: is five a per-selection batch limit, an actual panel storage capacity, or neither?
5. Provide the largest target count proven safe independently for road and rail panels. If no allocation/capacity proof is possible, keep >5 `UNVERIFIED`; do not infer a limit from visible rows or vector growth.

## Required response

Return findings in the response only. Label conclusions `CONFIRMED`, `CONTRADICTED`, or `UNVERIFIED`; cite RVAs and exact allocation/indexing evidence. List the wrapper commands and scripts actually run. End with the proven panel array capacity, safe target count for road/rail, and whether the staged conservative five-entry cap can be raised.
