# Private logistics — runtime observation work package

> Current implementation: see `RUNTIME_PROTOTYPE.md` for the Arena-fixed
> v0.2.3 snapshot prototype, local checks, Windows cross-build and controlled
> test instructions. Agent assignments below are historical planning.

## Current goal

Observe ONE road distribution-office truck over TWO consecutive simple journeys:

**confirmed cargo load → actual per-vehicle fuel deductions → complete cargo unload**.

Compute fuel-only cost = summed actual tonnes deducted during the loaded trip × settlement-time in-game fuel price per tonne. Exclude empty travel before pickup and after delivery. No new trip payments. No claim of in-game validation until an actual game run and captured logs exist.

## Status and provenance

- Arena branch: `arena/83c93542-wrsre`; production remains v0.17, unchanged.
- Old Agent 1 report fetched at `80a4404d8d3fa5161bcaaf8de8cfd0a7fc7a72b7`, branch `reverse-engineering-and-verified-transport-associa`. Clean of conflict markers; not accepted as a verified road load/burn/unload implementation. Still contains task-stride/prototype inconsistencies.
- Old Agent 2 model fetched at `94d304dfbd182aef6464ff35315de8d16ae1a694`, branch `executable-accounting-model-and-deterministic-test`. Independently ran 27 tests, all pass. Additional probes: CLOSED prevents a legitimate second trip; UNSUPPORTED can be reactivated by a repeated load. Not production-ready.
- Review exports exist only in ignored `.cache/agent-review/` in this checkout. Remote reports/model have NOT been merged here. New agents must obtain the exact artifacts without blindly merging old branches.
- Old fuel-branch commit `75c1126` contains a conflict-corrupted report: do not use it as canonical.
- Prior delivery-to-delivery architecture is superseded. Preserve historical files; do not delete/rewrite all old docs or expand the Python model again.

## Retire old agents safely

Tell both old agents: stop new work; return current branch, full HEAD, `git status --short`, last test commands/results and any uncommitted artifact locations. Keep commits/worktrees until replacement agents verify access. Do not delete/reset worktrees or apply all their changes to a shared branch. Do not merge the prior conflicting report. Existing pushed artifacts are references, not accepted integration.

## Scheduling / ownership

Suggested allocation (not a claim of benchmarked model capability):

| Agent | Suggested model | Start | Exclusive writes |
|---|---|---|---|
| A: road-path evidence | Nemotron 3 ultra | Now | `docs/private-logistics-runtime/evidence/` |
| B: diagnostic scaffold | Nemotron 3.5 super vl | Now, no hooks | `plugins/private_logistics_probe/`, `docs/private-logistics-runtime/probe/` |
| C: manual test and log checker | Poolside Laguna S 2.1, bounded context | Now | `tests/private_logistics_probe/`, `docs/private-logistics-runtime/validation/` |
| D: gated integration/review | Nemotron 3 ultra | After A/B/C | B's probe paths only after B stops; review docs under `docs/private-logistics-runtime/integration/` |

Give every agent its own Kilo worktree and the corresponding prompt. In Arena, never switch/create a branch: use the fixed session branch. In Kilo, remain on the assigned existing worktree branch. No two agents edit the same files concurrently. B hands ownership to D; C owns the final checker/schema changes. Agents do not merge/close PRs or apply other worktrees themselves.

## Gates

1. **Static evidence:** A supplies exact-byte verification, vehicle provenance and a safe proposed observation mechanism. Unsupported paths stay unsupported.
2. **Offline readiness:** B builds a disabled-by-default diagnostic scaffold; C tests the log checker with explicitly synthetic fixtures. Host tests are not game evidence.
3. **Integration:** D reviews A's evidence and adds only supported observation paths. Hook code changes require ABI/register/stack/return correctness and exact-byte guards. Do not invent a loader hook API.
4. **Deployment approval:** user approves the build, diagnostic configuration, coexistence/exclusion with existing private_logistics, and rollback steps BEFORE DLL copying or game execution. This package authorizes investigation and an opt-in test build, not automatic installation.
5. **Runtime:** actual controlled game run, raw logs, independent cargo/tank comparisons and clear pass/fail/inconclusive results. No observation that changes the game's transfer/fuel behaviour. No new reimbursements; disable existing payment paths for the controlled run with documented config verification.
6. **Later:** only after observation is verified should C++ accounting/settlement implementation be proposed.

## Minimal diagnostic contract (v0, proposal)

B proposes a versioned structured log schema; C implements against that same frozen schema. Record raw and interpreted events separately. Minimum: schema version, run/session generation, process-wide event/call sequence, thread/depth where relevant, simulation tick if verified (else null plus monotonic wall timestamp), static site RVA, raw args/return only where supported, vehicle/DO/resource identity with confidence, before/after cargo or fuel values, actual deducted tonnes, sequence gaps/drops/errors, boundary reason, settlement-time price/currency and trip summary. Raw addresses are session-scoped identities, not persistent IDs. Full buffers/gaps invalidate affected trip results; never block the game thread on disk IO.

No guessed typed dereferences, no universal mode/sign classifier, no silent fallback when evidence fails. Duplicate observations and genuine extra loads must not be conflated solely by equal trip/carrier/resource. Prefer explicit raw observation identity and a verified complete-load boundary. Partial cargo transfer calls are not automatically complete pickup/delivery.

## First in-game scenario

Controlled copy of a save, one road DO, one fuel-consuming truck, one resource, one source, one destination, no extra pickup/destination. Two consecutive full loaded journeys. First choose no refuel during a loaded leg: compare summed deductions against authoritative tank decrease, allowing logged numeric tolerance and tick-order effects. Use actual cargo state changes as an independent boundary check. Then separately test refuel, partial unload, session reset and invalid evidence. Do not manipulate inventory merely to make expected results pass.

## Artifact hygiene and final reports

No executable, DLL, Ghidra project DB, save, caches or huge logs in Git. Small scripts/excerpts and synthetic fixtures only. Keep large runtime artifacts outside Git with manifest/hash and location. No credential capture in logs. Each agent returns one consolidated Markdown with complete relevant evidence/source references, exact commands, actual output, limitations, branch/full commit SHA, and successful push confirmation. Run tests/build if available, `git diff --check`, scan conflict markers and inspect staged files. Do not claim a build/runtime result that was not executed.
