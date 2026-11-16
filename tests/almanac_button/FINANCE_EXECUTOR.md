# Host-only Finance executor and rejection acknowledgements

`src/almanac_finance_executor.h` wires the synthetic controller to FinanceSession. ExecuteFinanceStep drains at most one request, runs the session adapter and publishes either a Finance completion or an explicit dispatch rejection. There are no hooks, threads, provider lookup, real scheduling, automatic retries or graphics operations. All calls, context updates and borrowed provider use must occur on one externally serialized model owner at a separately verified safe phase. Current tests use fake providers only.

## Result contract

- OverlayApplyResult now includes SessionDispatch; OverlayOutcome includes Rejected.
- Completed means FinanceEditor ran. `executed=true` is NOT a claim that any write succeeded. Inspect the full FinanceApplyResult.
- Rejected means the session handler refused execution, with `executed=false` and a reason such as Stale, NotLoaded, Busy or DuplicateOrOld. Default Finance status in this envelope is not a real Finance result.
- Controller Feedback returns the current visible acknowledgement, including dispatch failures. Result continues to return only actually executed Finance outcomes. UI must branch on outcome/dispatch, not display a rejected envelope as successful application.
- Poll drains either acknowledgement, clears pending status and releases the slot. Stale transport acknowledgements remain diagnostic-only after close/world changes. A session Stale rejection can be displayed when transport context is still current (for example an old edit-session token); it is not itself evidence that writes occurred.
- Reject cannot complete the wrong request, overwrite a completed acknowledgement or use Ran as a refusal. Busy rejection is terminal for that request; only an explicit new user request may retry.
- ExecutorStep::AcknowledgementFailed is an invariant failure, not permission to replay writes. Retain ownership and diagnose; do not reset bridge/session IDs or silently retry.
- Provider exceptions/faults and device/native failures are not caught or made transactional by this wrapper. Production integration requires the existing approved provider contracts and error policy. This does not establish safe game-thread/service affinity.

## Tests performed

Strict C++17 build/run passed for Finance executor, Finance session, overlay controller, bridge, Finance editor and lifecycle suites. New executor tests passed under AddressSanitizer/UndefinedBehaviorSanitizer. The fake-provider integration checks:

- Wrong-session rejection invokes zero provider calls and releases pending capacity after Poll.
- Accepted execution preserves Applied and Ran distinctly from transport rejection.
- Attempting another Apply after a write yields visible NotLoaded, without auto-reloading or writing.
- Changed live-world identity rejects an otherwise queued request without writes.
- Wrong/duplicate rejection and invalid Ran refusal cannot overwrite state.
- Close-before-rejection prevents feedback appearing in the closed panel.

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -pthread -Isrc \
  tests/almanac_button/almanac_finance_executor_test.cpp \
  -o build/almanac_finance_executor_test
build/almanac_finance_executor_test
# Sanitized: add -fsanitize=address,undefined -fno-omit-frame-pointer
```

## Status and next gate

Host rejection-result wiring is implemented and tested, not independently reviewed. No child Reviewer capability is available here. No Windows plugin build, live Finance calls, deployment, game execution or visible panel was performed. The sidebar hook remains unapproved.

The next risky boundary is the actual graphics/input backend. Before implementation, freeze and independently review its minimal synthetic-data scope, pinned dependencies/license provenance, Present/WndProc hook coexistence, context/state restoration, partial initialization cleanup, input ownership and safe developer-trigger gates. Host test success is not evidence that these native integration requirements are satisfied.
