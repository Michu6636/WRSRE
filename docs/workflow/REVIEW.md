# Independent review contract

The Worker implements and runs checks. Reviewers are separate, strictly
read-only child sessions: no edits, shell, builds, tests, game launches, Ghidra
queries, delegation or goal control. They inspect supplied files/artifacts
directly; a Worker summary is a lead, not proof. A missing required artifact is
a blocker, not an approval.

For WRSRE engineering goals, send **each** of INVESTIGATION, PLAN and
IMPLEMENTATION to both focuses independently. Do not show one reviewer the
other's verdict first. Freeze the packet while it is reviewed. Tiny non-goal
chores may use one focused review only when the task explicitly allows it;
material, native, ABI, lifecycle or multi-domain work always uses both.

## Packet contents

Every stage packet contains:

- task goal, acceptance criteria, scope/non-goals and authorization boundaries;
- stage name and baseline revision/worktree caveats;
- exact changed paths and actual diff (or the exact investigation/plan artifact);
- relevant unchanged callers, state paths and counterexamples;
- Finding Ledger with every prior in-scope ID and current disposition;
- raw evidence paths plus address/line ranges, provenance and confidence;
- exact build/test/static-check commands, working directory, exit status and
  accessible outputs, or an explicit NOT RUN/unavailable reason.

Keep packets bounded. Point to tracked evidence instead of pasting entire
exports or historical handovers. Do not cite ignored `scratch/` as final proof.

## Focus 1 — correctness, regressions, ABI

Check requirements and full execution paths, including unchanged callers,
helpers, indirect dispatch, alternate guards, early returns, cleanup and
order-dependent writes. For every game call verify target/slot, ABI/registers,
arguments/return, side effects and receiver/vtable confidence. `ReadablePtr`
does not prove ABI or semantic safety; SEH does not handle normal failure
returns. Test state-machine counterexamples: non-default first call,
already-selected item, repeated frame, held/cleared latch, disarm/fault and
alternate paths. Check WRSRE mechanical rules: 14-byte whole-instruction spans,
`expect` bytes, RIP-relative/branch relocation, plugin isolation, zero-allocation
hot paths, flow UI/DPI, text IDs and INI encoding.

## Focus 2 — tests, evidence, counterexamples, scope

Check that each acceptance criterion and changed behavior maps to a check that
actually exercises it. Geometry-only tests do not prove click, texture, ABI or
runtime behavior. Reopen raw evidence at cited addresses/lines; check provenance,
derivation, address domains, typed offsets and applicability limits. Challenge
negative claims with exact scan scope, pattern, register/alias coverage and
transitive helpers; zero matches alone are not proof. Look for scope creep,
missing files, comment/evidence contradictions and silently dropped ledger IDs.

## Finding format and verdicts

Return evidence-backed findings, not a vote:

```text
Finding ID / severity (blocker|high|medium|low):
Location (path:line, address or test case):
Evidence inspected:
Trigger / counterexample:
Impact / violated requirement:
Smallest fix or missing evidence:
Confidence and limits:
```

Separate defects from hypotheses and optional improvements. If there are no
findings, list the files/artifacts and checks actually covered and what was not
reviewed. End every report with these separate statuses:

```text
STATIC REVIEW: APPROVED | REJECTED
BUILD/TEST EVIDENCE: PASS | PARTIAL | FAIL | NOT RUN | BLOCKED — coverage/limits
RUNTIME VALIDATION: PASS | FAIL | NOT RUN — exact live evidence or reason
```

A reviewer may approve static correctness while build/runtime evidence is
partial or not run; it must not relabel those as runtime passes. The lead
reconciles every finding against source, repairs valid defects, reruns affected
checks and requests a new frozen review. There is no round cap and no early
approval for convenience.
