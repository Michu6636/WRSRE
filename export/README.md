# `export/` — cached reverse-engineering evidence

Tracked, bounded raw artifacts produced by Ghidra/PE queries. These exist so a
future agent can **re-read the evidence instead of re-querying** the binary.

This directory previously had no index. Start here.

## Rules

- Evidence here is raw material, not a conclusion. Verified bytes do not verify
  semantics. Label claims `supported` / `inferred` / `contested` / `unresolved`
  and cite the file **and** line range or address range.
- Addresses are Ghidra **preferred-image VAs** (`0x140000000 + RVA`) unless a
  file states otherwise. Never hardcode absolute pointers.
- `scratch/` is git-ignored and is never final proof. Promote only the bounded
  excerpt needed for a claim into this tree.
- Do not overwrite an existing artifact without authorization.

## Layout

| Path | Contents |
|---|---|
| [`almanac-evidence/`](almanac-evidence/) | Almanac research bundles: C3DDLL64 draw/collision/panel traces, reputation and sidebar UI studies, Frontline triage, click-close traces. See its [`MANIFEST.txt`](almanac-evidence/MANIFEST.txt) for per-file SHA-256 and address ranges, and [`REVIEW.md`](almanac-evidence/REVIEW.md) for the review verdict. |
| [`almanac-row-item-analysis-20261110/`](almanac-row-item-analysis-20261110/) | Row-item vs button comparison for the Economy & Trade sidebar row. |
| [`reputation-sidebar-ui-20261110/`](reputation-sidebar-ui-20261110/) | Reputation mod sidebar UI mechanism captures. |
| `140317db0_full_disasm.asm` | Full disassembly of `FUN_140317db0`. |
| `140373ea0_1403829fd_disasm.asm` | Disassembly, `0x140373EA0`–`0x1403829FD`. |
| `ch_tab23_full.asm` | Full disassembly for City Hall tab 23. |

## Related evidence trees

Evidence is also kept in three other tracked locations. Check all of them before
requesting a new query:

- [`docs/ghidra-exports/`](../docs/ghidra-exports/README.md) — curated, bounded
  decompilations/disassemblies promoted out of `export/`.
- [`docs/ghidra-index/`](../docs/ghidra-index/README.md) — function-by-RVA
  knowledge base. Read this **first**; it should cite which export backs a claim.
- [`docs/evidence/`](../docs/evidence/) and
  [`docs/private-logistics-runtime/evidence/`](../docs/private-logistics-runtime/evidence/)
  — runtime logs, screenshots and test packets.

Historical agent transcripts are duplicated under sibling `*-agent-<sha>/` and
`*-independent/` directories inside `almanac-evidence/`. Those duplicates are
intentional: they preserve each reviewer's independent output unchanged.
