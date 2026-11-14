# Provider roles and handoff

Use stronger available models for scarce reasoning, not for every edit/test
cycle. Antigravity (`agy`, via `GEMINI.md`) or GitHub Copilot (via `.github/`)
acts as Architect, Ghidra Lead and Final Auditor. Kilo runs the heavy
Investigate → Plan → Implement → Build/Test → Review → Repair loop on the
operator's selected free/quota-limited model. This is a manual coordination
pattern, not automatic cross-provider model routing.

## Ownership modes

**Kilo-led goal:** `autonomous-dev` is the sole Ghidra operator and task-record
writer. The external architect may supply a packet and later audit read-only;
it must not query the same project while Kilo owns it.

**Architect-led pre-verified packet:** `agy` or Copilot owns Ghidra, freezes the
investigation/plan evidence, then delegates implementation. The packet must
state that Kilo may not query Ghidra. Kilo still verifies the supplied evidence
against disk and runs all three review gates; “pre-verified” never skips review.

In both modes there is one writer per shared file and one Ghidra operator per
project, including GUI/headless/MCP access. Use the create-exclusive
`scratch/.ghidra.lock` protocol from `INSTRUCTIONS.md`; never delete another
owner's lock. Transfer ownership explicitly in the tracked task record after
the prior operator releases it. Separate worktrees do not isolate a shared
Ghidra database.

## Delegation commands

Run from the repository root on the host where Kilo is installed. Replace the
task path; do not leave placeholders in an actual launch.

```text
kilo run --auto --agent implementation-worker "Edit only <assigned files> per <tracked spec>. Do not run Ghidra or build.bat. Return changed paths and exact checks."
```

```text
kilo run --auto --agent autonomous-dev -f docs/tasks/active/<task>.md "Execute the attached WRSRE task record. Use the goal lifecycle if exposed; otherwise run the full loop to completion or a recorded blocker."
```

For a read-only smoke check, have the Kilo lead delegate a bounded review packet
to `reviewer-focus1` or `reviewer-focus2`; they are subagents, not primary CLI
agents. Reviewers cannot run shell commands, so the parent must supply the
actual diff and check outputs as accessible files or attachments. Do not ask a
reviewer to generate evidence it cannot read.

## Final audit

After Kilo returns, the stronger external auditor inspects the actual final
diff, unchanged call paths, Finding Ledger, raw evidence, build/test outputs
and both reviewer reports. It challenges the preferred conclusion and checks
for dropped findings, scope creep and static/build/runtime conflation. Repair
valid audit findings through the same Worker/Reviewer loop and re-audit the new
frozen state. The auditor does not edit files or grant restricted permissions.

If a provider, child-review capability, quota or required artifact is missing,
record the exact blocker. Do not fabricate a review, switch models silently or
make the user relay routine messages between agents.
