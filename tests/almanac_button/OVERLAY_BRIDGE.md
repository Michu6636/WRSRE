# Host-only overlay Apply bridge

`src/almanac_overlay_bridge.h` is a process-local transport policy, not an overlay renderer, native hook, shared service ABI or Finance executor. It copies `FinanceDraft` commands and `FinanceApplyResult` acknowledgements. It does not call FinanceEditor or any provider. Existing lifecycle/Finance components remain unchanged.

## Contract

- Capacity is one outstanding command INCLUDING its unconsumed result. Submit returns Busy rather than silently dropping a new request. The caller retains its visible draft on rejection.
- Supply nonzero world epoch and panel generation, plus bridge snapshot revision and readiness. PanelState's own generation is not a world epoch.
- Request IDs increase monotonically for the entire bridge lifetime. Accepted IDs cannot be reused, including after cancellation/result consumption. Busy/stale/unavailable submissions do not reserve an ID. Do not recreate this bridge while callbacks or commands from its old instance can arrive. No process-restart/persistent exactly-once guarantee is offered.
- A Take succeeds once. Only one model owner may drain and execute commands. Invoke the actual handler after Take returns, outside the bridge mutex. Provider-policy validation remains FinanceEditor's responsibility; matching bridge revision alone proves no provider consistency.
- World/panel/revision/readiness changes invalidate queued commands with a retained Stale acknowledgement. They also mark already-completed, unconsumed acknowledgements stale, preserving the actual outcome and executed flag. That invalidation is sticky even if readiness/context later returns. Epoch/generation producers must never recycle old identities within this bridge lifetime. Regression coverage and the consolidated test runner are documented in `HOST_TESTS.md`.
- SetContext and real service execution MUST be externally serialized by the model owner. A mutex around transport is not a transaction with the game or tax providers. This component does not establish that owner phase or service thread affinity.
- Cancel succeeds only while queued. Once executing, cancellation is too late. Complete retains the actual handler result and marks it stale if its context has changed; the UI must not apply it to a new panel/world. `executed=true` means the handler ran, not that setters succeeded. A completed Apply cannot be rolled back by closing.
- Consume drains the acknowledgement once and frees capacity. Missing completion deliberately leaves the bridge busy; do not timeout/reset and risk re-execution. The owner must report handler errors through Complete. Wrong/duplicate completion IDs are rejected.
- Result consumption does not authorize blindly reloading/retrying Finance writes. Preserve partial-write/readback/conflict outcomes and require the existing explicit reload policy.
- No pointers to game objects or STL allocation ownership cross a DLL boundary. This C++ class and its mutex are not a cross-DLL API.

## Checks performed

On Linux, strict C++17 build/run and AddressSanitizer + UndefinedBehaviorSanitizer build/run passed for `almanac_overlay_bridge_test.cpp`. Tests cover copied drafts, busy/unavailable/stale feedback, duplicate submission/draining/completion, cancellation, world/panel/revision/readiness invalidation, late results, partial-write result preservation, concurrent duplicate submission and ID exhaustion. ASan/UBSan are not thread-race detection; full integration/concurrency verification remains open.

Commands:

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -pthread -Isrc \
  tests/almanac_button/almanac_overlay_bridge_test.cpp \
  -o build/almanac_overlay_bridge_test
build/almanac_overlay_bridge_test
# Sanitized: add -fsanitize=address,undefined -fno-omit-frame-pointer
```

No Windows plugin build, ImGui dependency installation, game hook, deployment or game execution occurred. No independent child Reviewer is available in this environment; this implementation is host-tested, not independently reviewed. Native integration remains gated.

## Next integration work

Define the externally serialized model owner and event ordering with a fake renderer/controller, including close versus pending Apply ordering, lost completions and stale-result UI rejection. Then review the bounded adapter before connecting real Finance services. Snapshot publication and launcher activation are not implemented by this single-purpose Apply bridge. Graphics/input backend and native launcher remain separate tasks under `docs/almanac-overlay-design.md`.
