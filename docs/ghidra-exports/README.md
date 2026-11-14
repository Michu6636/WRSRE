# Raw Ghidra Exports (`docs/ghidra-exports/`)

Cached raw decompilations, disassemblies, and cross-reference dumps relocated from the repository root during workspace cleanup. File contents are preserved byte-for-byte.

Always consult [`docs/ghidra-index/`](../ghidra-index/README.md) for the reviewed status and interpretation limits of each function before relying on raw decompiler output.

## Promoting evidence from scratch

Publish only the bounded, permitted excerpts needed for consequential claims;
do not commit the executable, Ghidra project or whole unbounded exports. Record
the binary/project identity, script and arguments, address domain/range, query
or cache-reuse date, and whether hashes were locally measured or externally
reported. Keep raw bytes/instructions distinct from interpretation, link the
claim/task record, and update the function index after successes and failures.
An ignored `scratch/` filename, timestamp or agent summary is not accessible
proof. If an artifact cannot be published, record the exact gap and request the
existing bounded excerpt from its owner rather than rerunning blindly.

| File | Function / Range | Related Index / Notes |
|---|---|---|
| `decomp_1406EE2A0.c` | `FUN_1406EE2A0` | Generic window creator (`0x5D8` alloc + 100-slot state init); see [`../ghidra-index/6EE2A0.md`](../ghidra-index/6EE2A0.md) |
| `xrefs_1406EE2A0.txt` | `FUN_1406EE2A0` callers | 231 UI callers including road/rail DO builders (`0x741170`, `0x745350`) and dispatcher (`0x6E9B80`) |
| `newB_fun_14066d530.asm` | `0x14066D500..0x14066EB00` | `FUN_14066D530` disassembly (DO vehicle assignment interval investigation) |
| `newB_post_call_area.asm` | `0x14066EB00..0x14066EC20` | Post-call flow after `CALL 0x140699420` |
| `newB_fun_140699420.asm` | `0x140699420..0x14069A000` | `FUN_140699420` disassembly (part 1) |
| `newB_fun_140699420_part2.asm` | `0x14069A000..0x14069E200` | `FUN_140699420` disassembly (part 2) |
| `newB_fun_1406bc5a0.asm` | `0x1406BC5A0..0x1406BCA00` | `FUN_1406BC5A0` path-check disassembly |
| `takeover1_1e5350_xrefs.txt` | `0x1401E5350` callers | Cross-references for `FUN_1401E5350`; see [`../ghidra-index/1E5350.md`](../ghidra-index/1E5350.md) |
| `takeover1_builder_decomp.c` | Builder decompilation | Cached panel/builder decompilation snapshot |
