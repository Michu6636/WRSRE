# Docs agent instructions

These rules load lazily when working under `docs/`. The root [`AGENTS.md`](../AGENTS.md)
remains the lean entry point; use [`workflow/README.md`](workflow/README.md) for
the goal/review load map.

## Permanent wiki, not scratch proof

- Start at [`README.md`](README.md), the relevant [`engine/`](engine/README.md)
  subsystem page, [`ghidra-index/`](ghidra-index/README.md) and bounded
  [`ghidra-exports/`](ghidra-exports/README.md). Reuse accessible evidence before
  requesting a query.
- One named operator owns each Ghidra project, including GUI/headless/MCP access.
  Analysis children consume exports; they do not query concurrently.
- After a query success **or failure**, update the function index with evidence
  path/range, provenance, status, contradiction and workaround. Promote only the
  permitted bounded raw excerpts needed for consequential claims into
  `docs/ghidra-exports/`; never rely on git-ignored `scratch/` as final proof.
- Label addresses explicitly: RVA / preferred VA / runtime VA / file offset.
  Never hardcode absolute pointers. Verified bytes do not verify semantics.
- Use claim-level `supported` / `inferred` / `contested` / `unresolved` labels.
  Legacy entry-wide `verified` is not blanket proof of signature, receiver,
  purpose, units or runtime application. Correct misleading current summaries
  and dependent conclusions where readers encounter them; preserve a concise
  supersession note and deciding source.
- Keep durable goal state, prior-finding import and Finding Ledger in a tracked
  record from [`tasks/TEMPLATE.md`](tasks/TEMPLATE.md). Only the lead writes the
  shared task report; reviewers return findings. Write long additions in small
  verified chunks and never replace a long file for a short edit.
- Record what was actually checked and what remains unknown. Static review,
  build/test evidence and live runtime validation are separate statuses.
