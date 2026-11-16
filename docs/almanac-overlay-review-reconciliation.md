# Overlay review reconciliation and bounded evidence plan

Status: worker response to the independent review published as `docs/reviews/almanac-overlay-backend-review.md` on Almanac (reported commit 8136ba1). This response is NOT reviewer approval. No graphics/input interception, deployment or gameplay operation is authorized by this document.

## Agreed scope

The first in-game candidate, if later approved, is read-only synthetic drawing. No native sidebar alteration, service consumption that arms the old launcher, Finance writes, terrain/facility/save hook, native latch write, clickable Close or keyboard capture. A separate draw-only mode is required: today's DrawOverlayPrototype has a Close button and window interaction and cannot simply be called unchanged while claiming a noninteractive prototype.

Rendering diagnostics, drawing, interactive controls and live Finance are separate gates. Unknown input/modal handling blocks activation and interaction; it must not be bypassed by a hotkey described merely as development-only. The current absence of a safe native launcher does not by itself block a separately reviewed noninteractive graphics experiment.

## Corrections / requested reviewer disposition

| Review claim or condition | Reconciliation |
| --- | --- |
| New code under plugins/almanac_overlay is outside normal discovery | Incorrect: build.bat around line 102 enumerates plugins\\*. Keep experimental native code outside plugins/, for example prototypes/almanac_overlay/native/, with an explicit opt-in build target reviewed before addition. No such build target is added now. |
| Only OM render targets/depth are restored | Incomplete as an integration conclusion. The chosen upstream ImGui v1.91.9b DX11 backend saves/restores rasterizer, viewport, scissor, blend, depth-stencil, shader and IA state. See evidence below. Outer OM target handling and every extra operation still require audit, including early exits and failures. This does not prove Earthworks' different pinned backend or future code correct. |
| End the game process on any fault | Not accepted as a default overlay policy. Do not copy the old launcher's fail-stop convention. For recoverable startup/render failures, bypass overlay work, chain the original function and release input claims. A native fault may leave recovery unsafe; handling that requires a separately reviewed policy, not a blanket promise of safe recovery or deliberate process termination. |
| Add a root LICENSE | No agent may relicense all repository content by assumption. Inventory provenance and preserve required notices. Root-license decisions require rights-holder authorization. Original wrapper code plus MIT ImGui still require their own clear distribution treatment; do not copy GPL-adapted Earthworks code until obligations are resolved. |
| Finance safe phase blocks implementation | It blocks live Finance integration, not a synthetic drawing candidate that makes zero provider calls. |
| Native launcher test blocks deployment | It blocks launcher acceptance. Do not arm/deploy that launcher for a separate synthetic candidate. Separate graphics-experiment authorization remains necessary. |
| Font-manager warning is an implementation blocker | Already dispositioned in PR #80 with base-source/native evidence. Do not add a speculative DWORD dereference. Not an open graphics dependency. |
| No graphics/input hooks yet, but Go for visible DXGI overlay | This is approval for preparation only, not installing the mechanism required to render in-game. A subsequent bounded backend/diagnostic review is still required. |

## Pinned backend evidence

Read-only inspection of upstream Dear ImGui commit `f5befd2d29e66809cd1110a152e375a7f1981f06` (v1.91.9b), `backends/imgui_impl_dx11.cpp`:

- Around lines 202–225: BACKUP_DX11_STATE includes rasterizer, viewports, scissors, blend factor/sample mask, depth-stencil/stencil ref, shader resources/samplers/shaders/class instances and IA/constant-buffer state.
- Around lines 228–249: corresponding Get calls capture modified state.
- Around lines 302–319: corresponding Set calls restore modified state and release acquired references.

This addresses the missing backend analysis, not the entire host render pipeline. Audit the wrapper's render-target selection/restoration, context ownership, callbacks, device state, resource lifetimes and failure paths before drawing. The existing headless test neither executes the DX11 backend nor proves runtime restoration.

## Gate separation

| Stage | Required evidence / decisions | Explicit exclusions |
| --- | --- | --- |
| S0: static graphics-route investigation | Matching executable/engine identity; D3D11 imports/callers; loader/plugin start ordering; exact available patch API semantics and coexistence constraints | No game execution, memory patching, runtime logging hook or dummy swap chain |
| D0: observation-only runtime diagnostics | Separate user authorization on disposable copy; reviewed observer mechanism and file scope; default-off gate, bounded logging, callback lifetime/failure policy; concrete plan to capture thread/window/chain identity and actual slot owners | No drawing, WndProc replacement, input capture, hotkey, old launcher activation, writes to game-owned objects or Finance |
| V0: noninteractive synthetic drawing | S0/D0 results where needed; independently reviewed graphics integration, pinned dependency/notices, render-state/resource audit, readiness/display policy and suspension/recovery behavior; explicit opt-in build outside plugin auto-discovery | No input registration/capture, clickable controls, service-backed data, native launcher or keyboard trigger |
| I0: interactive developer panel | Input-path ownership and native-modal/readiness evidence, complete gesture marshalling/release/overflow policy, same-coordinate hit tests, reviewed development activation and close paths | No assumption that WndProc swallowing alone blocks game input; no native-latch offsets copied without proof |
| F0: live Finance | Verified service lifetime/safe phase, explicit editing session and result policy, prior gates, independent integration review | No automatic Apply replay/reload or writes from render/input callbacks |

A diagnostic observer can itself involve interception. “Observation-only” is not permission to patch: the mechanism needs explicit review first. If a debugger/observer cannot obtain an item without changing execution state, disclose that fact. Do not demand runtime evidence while silently authorizing an unreviewed hook to obtain it.

## Next smallest evidence task — S0 only

Use the existing designated matching-binary/Ghidra operator. Inspect existing cached artifacts first; do not re-open or modify a shared project unnecessarily. Return actual bounded exports and commands, not a summary listing inaccessible paths.

1. Confirm source executable SHA256 `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` and identify/hash the paired engine DLL. A version label alone is insufficient.
2. Parse executable AND engine imports for d3d11.dll/D3D11CreateDeviceAndSwapChain and dxgi.dll creation exports. Provide descriptor, slot RVA and hint/name evidence for relevant imports; report absence if not imported. Do not assume Earthworks' engine import exists here.
3. For the relevant imported creation call, export the bounded caller sequence showing argument/output storage and failure handling. Identify where the swap-chain pointer is retained if traceable. Do not infer that a static pointer location remains valid across device/window recreation.
4. Inspect the actual loader's plugin Init/Start sequence against the relevant engine startup call. Provide source references or bounded executable evidence. If artifacts cannot prove ordering, explicitly defer ONLY ordering to D0. Do not claim a Present call stack/thread identity from static analysis.
5. Inspect current patchIat behavior: whether it adopts/preserves an existing target, uses slot ownership guards, supports chaining/unpatch and how partial failures affect DLL residency. Treat static semantics and observed runtime owners as different facts.

Deliver one short table: established fact, exact evidence, runtime-only question, and smallest D0 observation needed. Stop after the creation/ordering/ownership route; no sidebar, modal, Finance or general engine scan. No build, deploy, game execution, dependency installer, Ghidra mutation or interception implementation.

## D0 plan to be reviewed later — not an implementation authorization

Based on S0, select one observation mechanism and exact file scope. Capture only finite diagnostic records: plugin-start thread, creation-entry/exit thread, HRESULT, HWND/process/window identity, swap-chain/device identity and relevant slot/module owners. Identify Present-chain evidence that cannot be established without interception and request a separate narrow decision rather than installing one implicitly. Do not dump arbitrary process memory or personal filesystem data.

Default-off gate proposal: explicit development compile target plus `dev_overlay=1` for an eventual runtime candidate. This is proposed, not a claim that a safe activation gate already exists. For D0 there is no hotkey and no panel to open. Never automatically enable an old plugin while preparing this observer. No early disposable swap chain to discover vtables; no secondary hook route if creation was missed.

For V0, design a truly noninteractive drawing function (no titlebar Close, button, movement/resize interaction or input backend). Control visibility only through the later reviewed candidate configuration/readiness policy. If readiness cannot be established, remain unavailable and report it. User-run observations of window/chain behavior are separate from accepting an interactive Almanac.

## Review requests and branch coordination

Ask the independent reviewer to disposition each correction above and approve/reject the S0 task boundary separately from D0/V0. No revised reviewer conclusion is implied by this worker note. Beam owns the review report on Almanac; Arena owns this response and the small host-test fix on its current branch. Neither task needs simultaneous Ghidra access or overlapping file edits.

The host fix marks completed-but-undelivered bridge acknowledgements stale on context invalidation. It changes no native behavior. See tests/almanac_button/HOST_TESTS.md for the reproduced regression, matrix and reproducible runner. A follow-up PR publishes that work and this response for review; do not merge it as purported native-backend approval.
