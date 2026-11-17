# Synthetic overlay controller

`src/almanac_overlay_controller.h` joins PanelState, FinanceView and OverlayApplyBridge without a graphics backend or provider calls. It is a host-only integration seam, NOT a deployable overlay, native launcher, Finance executor or cross-DLL ABI. All public operations must run on one externally serialized owner thread. The bridge's internal mutex does not make the controller thread-safe.

## Synthetic backend protocol

1. SetWorld supplies a nonzero, monotonically increasing world epoch and readiness. A world change/readiness loss invalidates display data and queued work before requesting close. Epoch rollback is rejected. Never recycle a previous world identity.
2. Activate queues one open. BeginOpen/CompleteOpen acknowledge the fake backend's actual creation; repeated activation cannot duplicate a panel. Closure during opening still waits for the open result and then closure. Missing acknowledgements retain ownership rather than timeout/reset.
3. Publish requires the current world, current panel generation and a strictly increasing nonzero revision for that session. View produces fixed-capacity Finance rows; unavailable fields remain labelled unavailable. It never reads services.
4. Apply copies a draft into the one-slot bridge. Take is the synthetic model endpoint; Complete reports its outcome; Poll retires the acknowledgement. Apply remains Busy until that acknowledgement is drained. No automatic retry occurs.
5. Result exposes only an executed, nonstale result associated with the current panel/world/revision at acknowledgement time. Partial-write outcomes are preserved. Feedback also exposes explicit session-dispatch rejections with executed=false; those are not Finance results. Poll retires both paths and returns stale acknowledgements for diagnostics, but they never populate a new panel's visible result. See `FINANCE_EXECUTOR.md`.
6. Close cancels queued work and hides display/result data. Executing work cannot be cancelled; its eventual result is retained for diagnostics, marked stale and not displayed after reopen. CompleteClose must mean the backend really retired its panel callbacks/resources.

A published snapshot is display data, NOT proof that Finance editing is permitted. This synthetic controller does not enforce FinanceEditor's load/validation/reload eligibility, draft cancellation, provider lifetimes or service affinity. Do not connect Take directly to setters. A future model adapter must own FinanceEditor, validate its session/baseline and explicit reload requirements, run all service work at a verified safe phase and return its full outcome. UI editing controls must remain disabled until that adapter reports successful edit-session load. The tests intentionally submit synthetic drafts even when displayed data is unavailable to exercise transport independently; that is not intended player behavior.

No focus/input/DPI/graphics handling is implemented. Activation, snapshot publication and backend acknowledgements are synthetic method calls, not live game signals. No ImGui code is copied.

## Verification performed

Strict C++17 host tests passed for controller, bridge, lifecycle and Finance editor. Controller AddressSanitizer/UndefinedBehaviorSanitizer tests also passed. No Windows build or game execution. Independent child review remains unavailable and is not claimed.

Coverage: open and duplicate activation; copied Finance view with unavailable values; invalid snapshot identities/revisions; queued/executing Apply; partial-write outcome; close/reopen before acknowledgement; stale old-panel publication/completion; snapshot revision invalidation; world epoch rollback; world transition during Opening; readiness loss. A test initially exposed world-change invalidation being classified as ordinary cancellation because close ran first. SetWorld now invalidates bridge context before requesting close; the regression test passes.

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -pthread -Isrc \
  tests/almanac_button/almanac_overlay_controller_test.cpp \
  -o build/almanac_overlay_controller_test
build/almanac_overlay_controller_test
# Sanitized: add -fsanitize=address,undefined -fno-omit-frame-pointer
```

## Next gate

Review controller ownership/event ordering and design the explicit Finance edit-session adapter before live provider integration. Separately review a minimal synthetic-data graphics/input backend, including hook coexistence, state restoration, safe failure and licensing. Do not arm the unverified native sidebar hook. A real visible panel still requires those backend steps and identified disposable-copy tests.
