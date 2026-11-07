# Headless Ghidra query scripts

These Java scripts query a pre-analyzed Ghidra project with `analyzeHeadless`;
they do not require the graphical Ghidra window or an MCP server. See
[`INSTRUCTIONS.md`](../../INSTRUCTIONS.md) for the single-operator, cache,
index and authorization rules. A project, executable, JDK or Windows wrapper
may be on a different host: check actual availability before claiming access.

| Script | Purpose and arguments |
|---|---|
| `DecompAt.java` | `out.c VA [VA ...]`: decompile one or more functions. |
| `DisasmMany.java` | `out.asm start:end` or `out.asm start end`: disassemble **one** range of instruction start addresses, inclusive, with raw bytes. With only `start`, end defaults to start. |
| `FindDisp.java` | `out.txt displacement [block]`: find possible displacement uses; inspect each instruction, receiver and path. |
| `dump_all_strings.java` | Dump narrow ASCII/UTF-8 strings to the hardcoded `C:\WRSRE\scratch\all_strings_decoded.txt`. |
| `dump_wide_strings.java` | Dump UTF-16LE strings to the hardcoded `scratch/wide_strings_decoded.txt`. |

All five scripts overwrite their output file. Inspect the destination and script
first; do not overwrite an existing artifact without authorization.

Historical wrappers may also reference `Xrefs.java` or `ListFuncs.java`, but
those files are not in this tracked directory. Check the actual operator's
script paths; do not assume they exist or silently substitute another script.

## Addresses and evidence

For the historical project, the preferred image base is `0x140000000`.
`DecompAt` and `DisasmMany` take **preferred-image virtual addresses**, not raw
file offsets or bare RVAs: compute `VA = image base + RVA`, e.g. RVA
`0x4461F0` becomes `0x1404461F0`. Runtime addresses use the actual ASLR base.
`FindDisp` instead takes a numeric field displacement, not a function VA.
Confirm the actual project base and binary version before querying.

Before running, check the index and available cached raw exports; after a query,
record the function/range, provenance, result and limitations in the index.
`scratch/` is ignored by Git and may exist only on the operator's host: a
summary or commit does not publish it. Supply reviewers the actual authorized
small excerpt or artifact, not just its filename. Timestamps and hashes alone do
not prove the program's semantics or authenticate a different binary.

One operator must serialize headless queries and coordinate with any GUI or MCP
process holding the project. The standard query scripts are intended not to
modify program analysis; this is not an OS-level read-only guarantee. Back up
and work on a separate project copy for untested tooling. With bethington
GhidraMCP v7 the bridge is separate from its GUI or headless backend; other MCP
implementations can differ. Do not change transports or install tools just to
run these existing scripts.
