# Almanac overlay PR #81 follow-up review

**Status:** Read-only review. No source edits, builds, dependency installers, deployment, game execution, runtime instrumentation, Ghidra changes or PR merge.  
**Base reviewed:** Almanac @ `3b030d4` (PR #80 merged).  
**PR #81 head (verified):** `d1fcdece8466afadbafcde4c2dfa6ee3822a6dcc` (expected `d1fcdec`).  
**Prior report (linked, not overwritten):** `docs/reviews/almanac-overlay-backend-review.md` (commit `8136ba1`).  
**Access method:** `git fetch origin pull/81/head:pr81_head` (no branch switch, no local file overwrite). PR head commit confirmed via `git rev-parse pr81_head`.

---

## 0. Access confirmation

- PR #81 head commit `d1fcdece8466afadbafcde4c2dfa6ee3822a6dcc` matches the expected `d1fcdec`.
- `docs/almanac-overlay-review-reconciliation.md` is readable from the fetched ref.
- `src/almanac_overlay_bridge.h`, both test files, `run_host_tests.py`, `HOST_TESTS.md`, `OVERLAY_BRIDGE.md` are readable from the fetched ref.
- No access failure. No substitution of older scope documents.

---

## 1. Findings ranked by severity (exact file/line references)

### F1 (High) — PR #81 deletes the previously published review report
- **File:** `docs/reviews/almanac-overlay-backend-review.md` (present on Almanac @ `8136ba1`, absent on PR #81 head).
- **Evidence:** `git diff --stat Almanac..pr81_head` shows `docs/reviews/almanac-overlay-backend-review.md | 213 ---------------------`; `git ls-tree -r --name-only pr81_head` returns no match.
- **Impact:** The PR is branched from a pre-review Almanac commit and would remove the published review on merge. This is a branch-coordination defect, not a code defect, but it must be resolved before merge (rebase PR onto current Almanac or re-add the file).
- **Disposition:** Request changes (branch hygiene).

### F2 (Medium) — Sticky invalidation is correct but only covers the `Result` phase
- **File:** `src/almanac_overlay_bridge.h`, lines 42-51 (`SetContext`).
- **Change:** Added `if (phase_ == Phase::Result && !Matches(result_.context)) result_.stale = true;`.
- **Assessment:** Correctly addresses the reproduced defect: a completed-but-undelivered acknowledgement now becomes stale on context change, and invalidation is sticky because `result_.stale` is only ever set true here and in `Complete`/`FinishWithoutExecution` (never reset to false). The actual outcome (`executed`, `finance`, `dispatch`) is preserved. Pending slot is not released (capacity remains busy until `Consume`), which matches the documented "missing completion deliberately leaves the bridge busy" contract.
- **Residual gap:** `SetContext` does not re-check `Matches` for a `Result` whose context still matches but whose `ready` later flips; however `Matches` includes `ready`, so a readiness loss does mark stale. Good. No gap found in the added logic itself.
- **Disposition:** Agree (with the caveat in F3).

### F3 (Medium) — `SetContext` is not externally serialized with service execution by construction
- **File:** `src/almanac_overlay_bridge.h`, lines 40-51; contract in `OVERLAY_BRIDGE.md` ("SetContext and real service execution MUST be externally serialized by the model owner").
- **Assessment:** The bridge mutex protects transport state but does not establish the service-safe phase or thread affinity. The reconciliation doc and design doc both state this. The fix does not change this; it only makes the stale flag accurate. This remains a design-only gate (F0/live Finance blocked until service-safe phase is proved).
- **Disposition:** Agree; no change requested to the bridge, but the gate must remain documented as blocking F0.

### F4 (Low) — Test runner reports "11 core/extraction suites" but SUITES tuple lists 10
- **File:** `tests/almanac_button/run_host_tests.py`, lines 14-24 (SUITES tuple has 10 entries) vs `HOST_TESTS.md` ("ten strict warnings-as-errors C++ suites" + tax extraction harness = 11 total).
- **Assessment:** The runner runs 10 SUITES plus `tax_policy_extracted` plus optional `imgui_headless`. The HOST_TESTS.md wording ("ten strict ... suites ... plus the actual tax-provider/publication/consumer extraction harness") is consistent with 10+1. The reconciliation doc's "all 11 core/extraction suites passed" is consistent. No defect; wording is slightly ambiguous but not incorrect. Minor documentation clarity only.
- **Disposition:** Needs evidence / cosmetic; not a blocker.

### F5 (Low) — Controller test `CompletedBeforeWorldChange` relies on `SetWorld(2, false)` invalidating
- **File:** `tests/almanac_button/almanac_overlay_controller_test.cpp`, lines 14-26.
- **Assessment:** Tests completion -> world change -> Poll returns stale+executed with finance preserved. Consistent with bridge fix. Good coverage of the controller-level protection that already existed (design doc §lifecycle). No defect found.
- **Disposition:** Agree.

### F6 (Info) — No native code, hooks, or deployment in PR #81
- **Evidence:** Diff stat shows only host header, tests, docs. No `plugins/` changes. Consistent with "host-only" scope.
- **Disposition:** Agree; no native-hook approval implied.

---

## 2. PR #81 recommendation

**Request changes** (branch hygiene + minor documentation clarity), not blocked on code correctness.

- The host-only stale-acknowledgement fix is correct and well-tested (12 bridge cases + controller case). Approve the code change itself.
- Request that the PR be rebased onto current Almanac (or the published review file re-added) so merge does not delete `docs/reviews/almanac-overlay-backend-review.md`.
- Request clarification of suite-count wording in `HOST_TESTS.md` (cosmetic).
- Do not treat this PR as native-backend approval. It changes no native behavior.

---

## 3. Disposition of each reconciliation correction

| # | Correction | Disposition | Reason |
|---|-----------|-------------|--------|
| 1 | `plugins/*` is included in normal `build.bat` discovery. | **Agree** | `build.bat` enumerates `plugins\*`. Experimental native code must live outside `plugins/` (e.g. `prototypes/almanac_overlay/native/`) with an explicit opt-in build target. This matches the scope doc and my prior file-scope proposal. |
| 2 | Pinned upstream ImGui DX11 backend saves/restores more state than Earthworks wrapper's explicit OM target handling. | **Agree (fact)** | The pinned backend evidence (imgui_impl_dx11.cpp lines ~202-319) confirms rasterizer, viewport, scissor, blend, depth-stencil, shader/IA state save/restore. This is a fact about the pinned backend. It does NOT prove complete integration safety: outer OM target handling, context ownership, callbacks, device state, resource lifetimes and failure paths still require audit. Distinguish fact from proof. |
| 3 | Blanket termination of the game on any overlay fault is not accepted policy. | **Agree** | Recoverable startup/render failures should bypass overlay work, chain the original function and release input claims. A native fault may leave recovery unsafe; that requires a separately reviewed policy, not a blanket promise of safe recovery or deliberate process termination. Do not copy the old launcher's fail-stop convention as default. |
| 4 | A repository-wide license cannot simply be assigned by an agent. | **Agree** | No agent may relicense all repository content by assumption. Inventory provenance and preserve required notices. Root-license decisions require rights-holder authorization. Original wrapper code plus MIT ImGui still require their own clear distribution treatment; do not copy GPL-adapted Earthworks code until obligations are resolved. |
| 5 | Live Finance and native-launcher acceptance gates must remain separate from a synthetic noninteractive graphics experiment. | **Agree** | Synthetic drawing makes zero provider calls and does not require the service-safe Finance phase. Live Finance (F0) and native launcher remain independently gated. |
| 6 | The existing drawing prototype has clickable/window controls and is not automatically compliant with a truly noninteractive scope. | **Agree** | `DrawOverlayPrototype` has a Close button and window interaction. A separate draw-only mode (no titlebar Close, button, movement/resize interaction or input backend) is required before claiming noninteractive scope. Control visibility only through later reviewed candidate configuration/readiness policy. |

Additional reconciliation items (from the doc):
- Font-manager warning: **Agree** — already dispositioned in PR #80 with source/native evidence. Do not add speculative DWORD dereference. Not an open graphics dependency.
- "No graphics/input hooks yet, but Go for visible DXGI overlay" is preparation only, not installing the mechanism required to render in-game: **Agree** — a subsequent bounded backend/diagnostic review is still required.

---

## 4. Explicit S0 Go/No-Go

**S0 (static graphics-route investigation): GO.**

S0 is a read-only static investigation with no game execution, memory patching, runtime logging hook or dummy swap chain. It is the correct next smallest evidence task. Approve the S0 boundary as defined in the reconciliation doc:

1. Confirm source executable SHA256 `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` and identify/hash the paired engine DLL.
2. Parse executable AND engine imports for `d3d11.dll`/`D3D11CreateDeviceAndSwapChain` and `dxgi.dll` creation exports; provide descriptor, slot RVA and hint/name evidence; report absence if not imported.
3. For the relevant imported creation call, export the bounded caller sequence showing argument/output storage and failure handling; identify where the swap-chain pointer is retained if traceable.
4. Inspect the actual loader's plugin Init/Start sequence against the relevant engine startup call; provide source references or bounded executable evidence. If artifacts cannot prove ordering, explicitly defer ONLY ordering to D0.
5. Inspect current `patchIat` behavior: whether it adopts/preserves an existing target, uses slot ownership guards, supports chaining/unpatch and how partial failures affect DLL residency.

**Smallest missing prerequisite:** None beyond the existing designated matching-binary/Ghidra operator and cached artifacts. S0 must be executed by the designated operator; this review does not execute it.

**Explicit non-conflation:** S0 approval is NOT approval of D0 (runtime diagnostics), V0 (visible drawing), I0 (interaction) or F0 (live Finance). Each requires its own separate review and authorization.

---

## 5. Updated gate table

| Stage | Status | Required evidence / decisions | Explicit exclusions |
|-------|--------|------------------------------|---------------------|
| S0: static graphics-route investigation | **GO (approved this pass)** | Matching executable/engine identity; D3D11 imports/callers; loader/plugin-start ordering; patchIat ownership/chaining/failure semantics | No game execution, memory patching, runtime logging hook or dummy swap chain |
| D0: observation-only runtime diagnostics | Not yet authorized | Separate user authorization on disposable copy; reviewed observer mechanism and file scope; default-off gate, bounded logging, callback lifetime/failure policy; concrete plan to capture thread/window/chain identity and actual slot owners | No drawing, WndProc replacement, input capture, hotkey, old launcher activation, writes to game-owned objects or Finance |
| V0: noninteractive synthetic drawing | Blocked on S0/D0 + review | S0/D0 results; independently reviewed graphics integration, pinned dependency/notices, render-state/resource audit, readiness/display policy and suspension/recovery behavior; explicit opt-in build outside plugin auto-discovery | No input registration/capture, clickable controls, service-backed data, native launcher or keyboard trigger |
| I0: interactive developer panel | Blocked | Input-path ownership and native-modal/readiness evidence, complete gesture marshalling/release/overflow policy, same-coordinate hit tests, reviewed development activation and close paths | No assumption that WndProc swallowing alone blocks game input; no native-latch offsets copied without proof |
| F0: live Finance | Blocked | Verified service lifetime/safe phase, explicit editing session and result policy, prior gates, independent integration review | No automatic Apply replay/reload or writes from render/input callbacks |
| Native launcher | Independently unresolved | Matching-target boundary and input route; no fabricated native entries, hardcoded eighth index or patch at `0x1403204EB` using the current non-relocating helper | Not unblocked by S0/D0/V0 |

**No implied approval of native hooks.** S0 is static-only. D0/V0/I0/F0 and native launcher remain separately gated.

---

## 6. Limits

- No builds, dependency installation, deployment, game execution, runtime instrumentation, Ghidra mutation or PR merge performed.
- Reported test results (HOST_TESTS.md) are worker evidence, not my own executed checks.
- No independent child-review tool is exposed; no independent approval claimed.
- Sidebar geometry investigation not repeated.
- Arena's implementation branch not modified.

---

*Follow-up report published as `docs/reviews/almanac-overlay-pr81-followup.md`. Links to prior report `docs/reviews/almanac-overlay-backend-review.md` (commit `8136ba1`).*
