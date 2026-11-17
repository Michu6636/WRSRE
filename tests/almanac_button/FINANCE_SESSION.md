# Finance edit-session adapter (host-only)

`src/almanac_finance_session.h` wraps the existing FinanceEditor on one serialized model owner. It retains no provider pointers and adds no game offsets, hooks, renderer, DLL exports or automatic execution. Tests borrow fake currency/tax providers only. Real provider use remains gated on verified service lifetime and thread-affinity evidence.

## Integration contract

- Load requires a ready, nonzero world/panel identity. A successful load mints a new monotonically increasing edit-session token even if the snapshot revision is unchanged. Failed load clears the prior draft/session. Token exhaustion fails rather than wrapping.
- Copy Draft to the UI; submit it with the returned token. OverlayApplyCommand now carries `editSession`; OverlayController::Apply accepts it as an optional second argument. Zero remains permitted for older synthetic transport tests but this adapter rejects it. A display snapshot alone never enables editing.
- Execute requires command context, current live context and loaded edit context to match (world, panel, revision, readiness), plus the current edit-session token. Its nonzero request ID must exceed every previously processed request ID for this adapter lifetime. Rejected requests are also consumed, so they cannot become executable after a reload. Keep controller/session lifetimes paired and IDs monotonic; do not recreate either while old work can arrive.
- Stale/duplicate/unloaded requests do not call providers. Reentrancy during Load/Execute is rejected as Busy. This guard is not thread synchronization: every operation and actual provider execution must share one externally serialized owner.
- `ExecuteFinanceStep` in `src/almanac_finance_executor.h` now drains one request and routes Ran to Complete or a refusal to Reject. Ran means the policy ran, not that writes succeeded. SessionDispatch is retained separately in the acknowledgement. Controller Feedback exposes current-panel rejection reasons; Result exposes only executed Finance outcomes. Both paths require Poll to retire pending capacity. No automatic retry or reload is added. See `FINANCE_EXECUTOR.md`.
- Validation errors preserve editor eligibility for correction under a NEW request ID. A write attempt or conflict disarms the editor; another request returns NotLoaded until explicit Load. No automatic retries or reloads. Provider-policy conflict detection is still performed by FinanceEditor, not replaced by bridge revision comparison.
- Close/world/readiness loss must cancel queued bridge work AND call session.Cancel on the same owner, before accepting a new session. Cancellation is local, makes no provider calls and cannot undo completed writes. Live-context validation is an additional guard, not a source of authoritative world signals.
- Revision changes invalidate the bound edit context; explicitly load a new session if continuing. Do not use rapidly changing display sample revisions indiscriminately for edit-session revision identity; settle publication/edit-baseline policy in the production adapter.

## Verification

Strict C++17 host tests passed for session, controller, bridge, Finance editor and panel lifecycle. Session AddressSanitizer/UndefinedBehaviorSanitizer build/run passed. Coverage includes a controller -> bridge -> session -> fake providers -> acknowledgement success path, duplicate suppression, disarming after writes, explicit reload tokens, stale token rejection, validation correction, policy conflicts, partial writes, stale world with zero provider calls, cancellation and missing-service load failure. The existing FinanceEditor test suite supplies broader setter/readback/reentrancy tests; not every case is duplicated here.

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -pthread -Isrc \
  tests/almanac_button/almanac_finance_session_test.cpp \
  -o build/almanac_finance_session_test
build/almanac_finance_session_test
# Sanitized: add -fsanitize=address,undefined -fno-omit-frame-pointer
```

No live Finance writes, Windows plugin build, dependency installation, deployment or game execution. Independent child review was unavailable and is not claimed. This does not implement a visible panel or temporary shortcut.

## Next bounded gate

Review the host-side session/rejection acknowledgement path and edit-baseline revision policy before real services. For the visible prototype, review a minimal read-only synthetic overlay backend with a development-only activation path, input arbitration, dependency/license provenance, hook coexistence and recovery. The native sidebar interception remains unapproved.
