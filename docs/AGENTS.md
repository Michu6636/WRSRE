# Docs agent instructions

- Ghidra index entries live in `docs/ghidra-index/<RVA>.md` (see `docs/ghidra-index/README.md` schema). Raw excerpts live in `docs/ghidra-exports/`.
- Label addresses explicitly: RVA / preferred VA / runtime VA / file offset. Never hardcode absolute pointers.
- Use `verified` only for claims supported by the identity-confirmed PE. Use `inferred` / `contested` / `unresolved` otherwise. Verified bytes do not verify semantics.
- `scratch/` is git-ignored and may exist only on the operator's machine. Promote verified excerpts to `docs/ghidra-exports/`; never cite an inaccessible `scratch/` path as proof.
- Update the index after successes *and* failures (evidence path, status, contradiction, workaround).
