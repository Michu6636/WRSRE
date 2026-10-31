# Ghidra Index Schema

Each entry in `docs/ghidra-index/` is a Markdown file named `<RVA>.md` (e.g., `6BF1D0.md` for `FUN_1406BF1D0`).

## Required Fields

| Field | Description |
|-------|-------------|
| **RVA** | Function RVA (hex, without `0x140` prefix) |
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

- All addresses are file-based RVAs (image base `0x140000000` + RVA).
- Use `verified` only when byte-verified against the identity-confirmed PE.
- Use `partial` when only some regions/guards are confirmed.
- Use `contested` when a new query contradicts an existing entry — preserve both views with a `Note:` explaining the contradiction.
- Use `failed` for timeouts, LockException, or bad address form — record the failure and workaround.
- After ANY `ghidra_run.bat` call, update or append the entry.
- Before ANY `ghidra_run.bat` call, grep this index first.