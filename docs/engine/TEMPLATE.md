# <Subsystem> engine contract

> Template only. Replace placeholders and remove this notice before publishing.
> Do not create a new native claim without accessible evidence and a recorded
> derivation. Link the task record and Finding Ledger that produced the page.

## Identity and scope

- **Task record:** `<docs/tasks/...>`
- **Binary/project identity:** `<locally measured / externally reported / unavailable; version, PE timestamp, image base>`
- **Scope / non-goals:** `<what this page covers and does not cover>`
- **Ghidra operator / date:** `<one owner; query date or cached-evidence reuse>`
- **Status:** draft | partial | verified-for-listed-claims | contested | superseded

## Interfaces and state

Describe only evidenced objects, fields, vectors, call paths, guards, lifecycle
and failure behavior. Label every address as RVA, preferred VA, runtime VA or
file offset. Separate static layout from observed runtime application.

## Claim ledger

| Claim ID | Exact statement | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
|---|---|---|---|---|---|
| <ID> | <claim> | <tracked docs/ghidra-exports path/range> | <decode/trace> | supported/inferred/contested/unresolved | <limit> |

## Callers, counterexamples and unchanged paths

List relevant callers/callees, alternate guards, zero/already-selected/repeated
cases, early returns, cleanup and order-dependent writes. For negative claims,
record scan scope, pattern, register/alias coverage and transitive helpers.

## Validation separation

- **Static evidence:** `<what the bytes/code establish>`
- **Build/test evidence:** `<commands/results or NOT RUN>`
- **Runtime validation:** `<exact live observation or NOT RUN; authorization>`

## Supersession and open questions

Record old claim, replacement, deciding source and dependent conclusions that
must change. List the smallest missing evidence/query rather than guessing.
Update [`../ghidra-index/`](../ghidra-index/README.md) after query successes and
failures, and link this page from [`README.md`](README.md).
