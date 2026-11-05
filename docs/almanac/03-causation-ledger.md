# Almanac Causation Ledger (0.1, synthetic)

UI-neutral, event-backed causation ledger. Synthetic events only in this worktree;
live observer/hook registration belongs to integration.

## Contracts

Event node (`EventNode`):
- `eventId`, `correlationId`, `parentEventId`, `causalEdgeId`
- `simTime` (double), `sequence` (deterministic, monotonic)
- `sourcePlugin`, `sourceKind`, `subjectKey` (stable entity key, no raw pointers)
- `amount`, `unit`, `currency`
- `valueClass`: Actual / DryRun / Estimated
- `evidence`: Unknown / Supplied / Verified / Unresolved / Truncated
- `truncated` marker

Edge (`CausalEdge`):
- `edgeId`, `fromEventId`, `toEventId`, `kind`, `evidence`, `truncated`

Edge kinds: Causes, Accrues, Settles, MovesTreasury, Estimates, Pairs, DependsOn.
A shared category, timestamp, or treasury movement alone does not prove causation;
edges must be explicitly supplied. Unknown provenance remains Unknown.

## Storage

Fixed-capacity arenas: nodes `kMaxNodes=256`, edges `kMaxEdges=1024`.
- Duplicate IDs rejected.
- Node parent cycles rejected at insertion.
- Edge cycles rejected at insertion.
- Missing parent node: accepted, `parentMissing=true` in snapshots.
- Missing edge endpoint: rejected.
- Non-finite `simTime`/`amount` rejected.
- Capacity exhaustion sets `truncated`/`overflow` and rejects insertion (no silent partial state).
- `removeSubtree(root)` evicts the complete affected subtree and dependent edges.
- `evictOldest(n)` evicts oldest roots as complete subtrees.

## Snapshots

`buildSnapshot` produces deterministic, fixed-capacity output sorted by `sequence`
then `eventId`. Traversal budget via `maxDepth`, `nodeBudget`, `edgeBudget`.
No allocation in hot path. Overflow sets `Snapshot.overflow`/`truncated`.

## Aggregation

`aggregate` partitions by `(valueClass, currency, unit)`. Dry-run and estimated
amounts never contribute to actual totals. `aggregateForRoot` restricts to a subtree.

## Reset

- `resetAll()` clears state.
- `observeTime(t)` resets on backward time.
- `resetOnIdReuse(newMaxId)` resets on reused IDs.
- Save/load and world changes should call `resetAll()`.

## Synthetic examples

1. policy change -> tax accrual -> settlement -> treasury movement (each edge supplied).
2. Matched B2B delivery with dry-run settlement output kept separate from actual money.
3. Estimated cost feeds estimate-only aggregate.
4. Incomplete provenance remains visibly unresolved.

## Integration needs (not implemented here)

- Live currency/state_ledger/tax observers feeding synthetic events.
- Loader registration and native hooks.
- UI rendering of snapshots.

## Synthetic vs runtime

All tests are synthetic C++17. No runtime/game validation performed.
