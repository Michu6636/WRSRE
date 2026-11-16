# Finance Apply/Cancel controller

`src/almanac_finance_editor.h` is mod-owned policy, not a native UI, service ABI
or tax provider. It calls existing public getters/setters only when explicitly
asked. There are no hooks, game offsets, allocations, settlement calls or save
format changes. The controller is noncopyable and intended for one UI thread.

## Adapter usage

1. Resolve currency v4 and taxes v3, with normal service lifetime guarantees.
   Call `FinanceEditor::Load(currency, taxes)` once on opening an editing session.
   Only `Loaded` makes `Active()` true. Failure clears the local draft and
   disarms previous edits; display the error rather than old values.
2. Bind UI fields to a copy returned by `Draft()`. Call `SetValue(kind, value)`
   / `SetEnabled(kind, enabled)` on edits. SetValue accepts invalid numbers so
   the UI can retain input and display validation errors; it does not clamp.
   VAT/profit draft values are PERCENT [0,100], not API fractions. Property is
   an active-currency amount per enterprise, not a percentage or final bill.
3. Cancel (including panel close without Apply) calls `Cancel()` and discards
   the local draft with zero service calls. Cancel cannot undo an earlier Apply.
4. Apply calls `Apply(currency, taxes)` once at a verified safe UI phase. It
   validates all inputs before ANY setter call, checks the current policy,
   calls only changed fields' setters and reads back the observed policy.
5. Preserve/display the returned `FinanceApplyResult` before loading a new
   editing session. Any write attempt disarms the editor. Another Apply needs
   an explicit Load and fresh user edits; there are no blind automatic retries.
   A conflict also disarms. Validation, service and preflight-read errors keep
   the draft armed for correction/retry. NoChanges leaves it armed.

Read-only Finance display continues to support v1/v2 providers. Editing requires
the v3 configured-policy snapshot and a known active currency, so suspended
rates/units are never invented. A setter is required only if its kind of write
is needed. The property scaling rule is read-only: there is no setter for it.
No native widget/provider integration is added in this milestone.

## Validation and conflict rules

- All drafts must be finite and nonnegative; percent fields cannot exceed 100.
  Validation happens before conversion to the provider's float representation.
- New property amounts cannot exceed 1,000,000, matching the current provider's
  `SetTaxRateImpl` cap. An unchanged larger configured amount can remain in
  place while another field is edited; it is not silently clamped/rewritten.
- Changes are compared at provider float precision. Loading the draft and
  immediately applying it must produce NoChanges, not rounding-driven writes.
- Preflight compares BOTH the rate and enabled status of each tax being edited
  against the loaded baseline. A change to either requires reload. For property
  edits, a changed scaling rule also conflicts. A changed active currency
  conflicts for any edit. Untouched taxes are not overwritten or made conflicts.
- This is optimistic preflight, NOT compare-and-swap or a transaction. No API
  revision token exists, so intervening edits after preflight (or ABA changes)
  cannot be prevented. Treasury and currency/policy are separate snapshots.

## Writes, partial results and verification

Writes run VAT -> profit -> property; within each tax the value precedes its
new enabled state. If a value setter rejects, that tax's changed enabled state
is `SkippedDependency` rather than enabling it at an unintended old rate.
Unrelated tax edits continue. Even a requested disable is skipped in that case;
reload and submit that disable separately if needed.

Results include per-field Unchanged/Accepted/Rejected/SkippedDependency and
counts of accepted/rejected CALLS (skipped fields are not calls). Accepted means
the setter returned 1, not that a final read has independently confirmed it.
The controller performs no rollback: reversing a successful write could itself
fail or overwrite someone else's change. Intermediate policy is observable
between setters, including a new rate before a requested suspension.

- Applied: all attempted writes accepted and read back as requested.
- PartiallyApplied: some writes accepted and verified; some rejected.
- Rejected: no writes accepted. Related enabled writes may have been skipped.
- ReadbackFailed: writes were attempted but the resulting policy/currency could
  not be read. Accepted/rejected outcomes remain available; do not claim success.
- VerificationMismatch: a setter accepted but the observed field differs, the
  currency changed, or the property rule changed during property edits.
- Conflict: no writes attempted; display observed current policy and reload.
- InvalidDraft: no writes; invalidValues is a bitmask using the tax-kind indices.
- ServiceUnavailable/SnapshotUnavailable: no writes; display an explicit error.
- NotLoaded/Busy/NoChanges: no writes; do not report a change was committed.

`observedValid` means all tax-policy fields and active currency are available,
NOT that the treasury/native window is valid. Check individual snapshot fields
for other readouts. On Conflict it is the preflight policy; after a write attempt
it is the readback policy. An error enum is not permission to show default zero
storage as actual policy. A future UI must show partial/failure status prominently.

Service pointers are not retained. The caller must keep providers alive through
Load/Apply and keep the established currency stable for property units. Reentrant
Load/Apply/Cancel/staging callbacks are refused while an operation is underway;
this is not cross-thread synchronization or a shield against faulty providers.

## Host tests

```sh
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -Isrc \
  tests/almanac_button/almanac_finance_editor_test.cpp \
  -o build/almanac_finance_editor_test
build/almanac_finance_editor_test
```

For ASan/UBSan replace `-O2` with
`-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer`.

Tests run the actual editor against fake public-service callbacks: staging and
zero-call cancellation, percent/fraction conversion and rounding no-ops,
validation including NaN/infinity/tiny negatives, range endpoints, missing or
old services, missing individual setters, snapshot failure, policy/currency
conflicts, preservation of unrelated policy, ordered writes, dependency skips,
partial rejection, dishonest acceptance/readback mismatch, readback failure,
currency changes during writes, repeat-Apply prevention and reentrant callbacks.
They do not call the real game or change live tax policy.

Optimized and ASan/UBSan tests passed on 2026-11-14. No Windows DLL build or
native UI test was performed. Independent child review is unavailable here;
this work is not independently reviewed. Sidebar/native integration files
are intentionally untouched by this milestone.
