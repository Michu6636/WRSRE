# Ghidra Index Schema

Each entry in `docs/ghidra-index/` is a Markdown file named `<RVA>.md` (e.g., `6BF1D0.md` for `FUN_1406BF1D0`).

## Required Fields

| Field | Description |
|-------|-------------|
| **RVA** | Function relative virtual address (preferred VA minus image base) |
| **Name** | Recovered name or `FUN_<RVA>` |
| **Signature** | Function signature (registers/stack) |
| **Purpose** | 1-2 sentence summary of what the function does |
| **Callers** | List of caller RVAs (or function names) |
| **Data Refs** | Key data offsets accessed (e.g., `+0xC80`, `+0xBB0`) |
| **Artifact Path** | Path to cached output in `scratch/out/` or `scratch/` |
| **Notes** | Additional context, contradictions, limitations |
| **Status** | `verified` / `partial` / `contested` / `failed` |

## Entry Template

```markdown
# FUN_140<RVA> — <Name>

**RVA:** `0x<RVA>`
**Signature:** `<signature>`
**Purpose:** `<purpose>`
**Callers:** `<list>`
**Data Refs:** `<list>`
**Artifact:** `<path>`
**Notes:** `<notes>`
**Status:** `<status>`
```

## Conventions

- Label addresses explicitly: RVA, preferred-image VA, runtime VA or raw file
  offset. Preferred VA = `0x140000000 + RVA`; runtime VA uses the actual loaded
  base. Neither VA is a raw file offset. Use section headers for file mapping.
- Use `verified` only for explicitly identified claims supported by the
  identity-confirmed PE. Verified instruction bytes do not verify a recovered
  signature, receiver type, purpose, physical unit or observed runtime path.
  Legacy entry-wide labels must not be inherited as blanket semantic proof.
- For consequential or disputed contracts, add a compact claim table under
  Notes: claim ID/text, raw artifact plus exact address/line, derivation,
  supported/contested/unresolved status, applicability limit and missing edge.
  If exports alone were checked, say so; do not claim PE authentication.
- Correct summaries only after checking raw evidence. Preserve the old claim,
  identify the replacement and explain the resolution; review dependent claims
  rather than changing the disputed sentence in isolation.
- Use `partial` when only some regions/guards are confirmed.
- Use `contested` when native evidence or a review of cached evidence contradicts
  an existing claim. Preserve both views with a `Note:` explaining the conflict;
  no fresh native query is required when existing evidence decides it.
- Use `failed` for timeouts, LockException, or bad address form — record the failure and workaround.
- After ANY `ghidra_run.bat` call, update or append the entry.
- Before ANY `ghidra_run.bat` call, grep this index first.