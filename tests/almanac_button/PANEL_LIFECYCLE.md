# Almanac panel lifecycle: integration contract

`src/almanac_panel_state.h` is the reusable mod-owned policy. The host test
executes that actual code plus a fake backend; it neither opens a game window
nor establishes native UI safety. No new hooks or service ABI are introduced.

## Recommended behavior

| Event | Result |
| --- | --- |
| Launcher activated while closed | Queue one open request |
| More activations while queued/opening/open/closing | Ignore; never spawn duplicates |
| Backend starts opening | Enter Opening; emit one generation ID |
| Backend confirms creation | Enter Open (or request close if cancelled meanwhile) |
| Backend reports creation failure | Return to Closed; next activation may retry |
| Close/Escape before creation starts | Cancel the pending request |
| Close/Escape during creation | Remember close; await creation result |
| Close/Escape while open | Queue one close request |
| Backend starts close | Enter Closing; emit one close-attempt ticket |
| Backend confirms close | Return to Closed |
| Backend reports close failure | Remain owned; queue a retry, never allow a second panel |
| Native backend independently confirms destruction | Return to Closed |

Close requests during Opening do not assume the backend can cancel safely.
Repeated launcher clicks are not a toggle. Reopening during Closing requires
another click after destruction is confirmed; there is no surprise reopen.

## Adapter responsibilities

- Keep one noncopyable `almanac::PanelState` for the mod session, on one UI
  thread. It does not synchronize threads or own native handles/resources.
  Do not recreate it while any old completion can still arrive.
- Call `RequestOpen()` from the accepted launcher activation. It queues only;
  only the backend can confirm that a panel actually exists.
- At a verified safe UI phase, call `BeginOpen()`. A nonzero return transfers
  exactly one action to the adapter; zero means do nothing. The state changes
  before the return, so synchronous backend completion is supported.
- Associate the returned generation with the native handle. When creation
  completes, call `CompleteOpen(generation, succeeded)`. Failure MUST mean
  partially created resources have been cleaned up and no live panel remains.
- Route close button/Escape/host teardown requests to `RequestClose()`. This
  does not implement or prove Escape routing or modal input capture itself.
- Drain `BeginClose()` similarly. Its nonzero value is a CLOSE-ATTEMPT TICKET,
  not the panel generation. Pass that ticket to `CompleteClose(ticket, ok)`.
  Retain the panel generation separately for native destruction callbacks.
- A failed close means the existing panel is still live. Retry only at a safe
  phase with adapter-owned backoff/error handling; do not loop in one frame.
  Every retry gets a new ticket, rejecting delayed results from older attempts.
- Use `NotifyDestroyed(panelGeneration)` only after native removal/destruction
  is confirmed, whether close was native or mod initiated. During Opening,
  first deliver the creation result; do not skip that ownership handoff.
- Completion methods return false for wrong-state, zero, duplicate or stale
  IDs. They do not clean up resources. The adapter remains responsible for
  orphan prevention, callback ordering and native handle lifetime.
- A missing completion deliberately blocks further opens. Do not add a timeout
  that resets ownership and risks a duplicate window. Diagnose the backend;
  teardown/unload must drain callbacks and confirm destruction before releasing
  the owner. No force-reset API is provided.

Generation and close-ticket counters never wrap/recycle; exhaustion refuses
new work rather than accepting stale IDs. This unreachable-in-practice bound
is a guard, not a recovery path. Tokens are mod bookkeeping, not game pointers.

## Running the host test

From the repository root on Linux:

```sh
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -Isrc \
  tests/almanac_button/almanac_panel_state_test.cpp \
  -o build/almanac_panel_state_test
build/almanac_panel_state_test
```

For sanitizers, replace `-O2` with
`-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer`.

From a configured MSVC developer command prompt (instructions, not a Windows
build result from this session):

```bat
if not exist build mkdir build
cl /nologo /std:c++17 /EHsc /MT /W4 /WX /Isrc tests\almanac_button\almanac_panel_state_test.cpp /Fobuild\almanac_panel_state_test.obj /Febuild\almanac_panel_state_test.exe
build\almanac_panel_state_test.exe
```

Coverage includes normal lifecycle, duplicate requests/completions, failed
creation, pre-start cancellation, close-during-create success/failure, close
retry with stale results, stale generations after reopen, independent native
destruction and a fake backend pumped for hundreds of repeated frames.

Linux optimized and ASan/UBSan runs passed on 2026-11-14; existing row geometry
and shared button-core host tests also passed. No Windows DLL build, native
window test or independent child review was available in this session.

## Parallel work boundary

Local sidebar agents should consume this controller rather than independently
implementing a second pending/open boolean policy. This work only adds the
controller, host test and this contract; it does not alter sidebar hooks,
button rendering, game APIs, tax services or UI content. Connecting it to the
launcher/backend remains integration work after their native contracts are
verified. Treasury/tax view models are separate follow-up work, not part of
this lifecycle implementation.
