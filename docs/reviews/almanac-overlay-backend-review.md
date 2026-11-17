# Almanac overlay backend — independent read-only review (reconciled)

**Status:** Read-only review. No source edits, builds, dependency installers, deployment, game execution or Ghidra project changes were performed in this pass.  
**Branch:** `Almanac` @ `3b030d4e0e7a693ec4217607d3bb2ad402ca367e` (PR #80 merged).  
**Scope sources:** `docs/almanac-overlay-backend-review.md`, `docs/almanac-overlay-design.md`, `prototypes/almanac_overlay/README.md`, `docs/almanac-review-handoff.md`, `tests/almanac_button/{OVERLAY_BRIDGE,OVERLAY_CONTROLLER,FINANCE_EXECUTOR,FINANCE_SESSION,PANEL_LIFECYCLE}.md`, `docs/engine/almanac-ui.md`, Earthworks committed source (Git, not scratch cache).  
**Prior review reconciliation:** An earlier conditional review could not read the scope document (it was uncommitted at that time). That review's seven evidence requests, conditions and checklist are reconciled below against the now-published scope. The earlier incomplete handoff is not treated as final approval.

---

## 1. Revised Go / No-Go for the bounded synthetic-data backend

**Revised Go/No-Go: CONDITIONAL GO for a bounded, read-only, synthetic-data overlay backend; NO-GO for any interactive/clickable panel, live Finance writes, native launcher arming, or deployment until the evidence gates in §3 and §5 are closed.**

- **Go (bounded, noninteractive, synthetic):** A minimal visible overlay that renders the existing `prototypes/almanac_overlay` synthetic Finance table (fixed invented RUB treasury, policy fields labelled unavailable) inside an owned ImGui frame, with a conspicuous synthetic-data banner, no input capture, no service calls, no game-state writes, and no native sidebar modification. This is the next low-risk candidate after the host-only bridge/controller.
- **No-Go (interactive):** Any clickable Close, text editing, Apply/Cancel, or input capture. Noninteractive drawing does not authorize a clickable panel with leaky controls.
- **No-Go (deployment/player acceptance):** Until disposable-copy tests (§7) pass and native-modal/input arbitration is evidenced.
- **No-Go (native launcher):** The native sidebar launcher remains independently unresolved; no patch at `0x1403204EB` using the current non-relocating helper is approved.

---

## 2. Established facts vs. design proposals vs. blockers

### Established facts (with evidence references)

| # | Fact | Evidence |
|---|------|----------|
| F1 | `ui_overlay` is a `C3D_TERRAIN::Render` IAT hook drawing text via `C3D_FONTMANAGER::PrintLeftUnicodeNoArg`; a text HUD, not a DXGI overlay. | `plugins/ui_overlay/ui_overlay.cpp` (lines 267-275, 78-116) |
| F2 | Earthworks overlay uses a DXGI `Present` hook via `D3D11CreateDeviceAndSwapChain` IAT interception, ImGui context, per-frame RTV, OM target save/restore, WndProc subclass. Reference only. | `Mods for analysis/wrsr-Earthworks-main/src/ui/overlay.cpp` (lines 1860-2031) |
| F3 | Almanac launcher is request-only: emits open request, never draws panel or mutates Finance state. | `plugins/almanac_button/almanac_open_state.h`; `almanac_button.cpp` (lines 580-640) |
| F4 | `game+0x5E6` is NOT visual content suppression; `C3D_PANEL2D::Draw` still executes; only disables click handling + tint when mouse inside a rect. | `docs/engine/almanac-ui.md` §1.2.1 |
| F5 | Tesmio host API v4, `/MT` static CRT, plugin isolation (no cross-boundary frees). | `src/tesmio_api.h`; `plugins/AGENTS.md` |
| F6 | Earthworks dependency pinning: TesmioLoader 0.3.6 (`7d6bb894…`), RepublicMP `630775724…`, ImGui MIT, GPLv3. | `dependencies.lock.json`; `THIRD_PARTY_NOTICES.md` |
| F7 | Native button geometry/hitbox logic is host-tested. | `tests/almanac_button/almanac_button_geometry_test.cpp` |
| F8 | Host-only overlay bridge/controller exist and pass strict C++17 + ASan/UBSan; synthetic data only, no hooks. | `tests/almanac_button/OVERLAY_BRIDGE.md`, `OVERLAY_CONTROLLER.md` |
| F9 | Prototype drawing code is original (no Earthworks/RepublicMP copy); ImGui v1.91.9b @ `f5befd2d…` used for headless checks; MIT notice must be preserved. | `prototypes/almanac_overlay/README.md` |
| F10 | No `LICENSE` file exists in the WRSRE repo root. | repo root listing |
| F11 | Hook point `0x1403745B6` first two instructions are 9+8 bytes, RIP-relative; unsafe for the 17-byte non-relocating loader helper. | `docs/engine/almanac-ui.md` §4 |
| F12 | `CreateManagedTexture` middlepoint must be `Base()+0x9EACD0`; plugin computation is wrong. | `docs/engine/almanac-ui.md` §2.1, §3 |
| F13 | `GetMouseSolid` ABI is `void(*)(void*, C3DVec3*)` (sret); plugin typedef `float*(*)(void*)` is wrong. | `docs/engine/almanac-ui.md` §2.4 |

### Design proposals (not established)

- Reusing Earthworks' DXGI bootstrap as the Almanac overlay backend (reference only; not approved).
- Synthetic-data-only rendering (no game state reads) — proposed, not yet wired to graphics.
- Dev-only activation mechanism (INI flag / env var / compile-time) — proposed, not implemented.
- Click-through, keyboard, and native-modal arbitration for an Almanac panel — design-only.
- A new `almanac_overlay` plugin consuming `TSM_SERVICE_ALMANAC` — proposed.
- Host-only bridge/controller as the integration seam — implemented and host-tested, but not connected to graphics or Finance.

### Blockers

1. **Swap-chain identity / hook coexistence:** Earthworks hooks the real chain after Steam init; whether this game build's chain is the same, and whether Steam overlay chaining survives, is unverified. No Present call-stack evidence captured.
2. **Render-state restoration completeness:** Earthworks saves/restores OM render targets + depth only. Blend, rasterizer, viewport, scissor, depth-stencil are not saved/restored. Resize/fullscreen transitions untested.
3. **Thread ownership / input marshalling:** Present runs on the render thread; WndProc subclass runs on the UI thread. Whether they are the same thread, and whether the recursive `guiMutex` is sufficient, is unverified. No thread-ID logging exists.
4. **Native-modal arbitration:** No mechanism to detect game modals (pause, dialogs) to suppress overlay input. `game+0x5E6` is insufficient (mouse-dependent, not visibility).
5. **Dev-trigger gate:** No existing dev-gate mechanism in Almanac plugins; design undecided.
6. **Dependency pinning / licensing:** WRSRE has no ImGui/DXGI dependency pinning and no LICENSE file. Earthworks is GPL-3.0-only (adapted from RepublicMP). Mixing requires license compatibility review.
7. **Host-side lifecycle contracts:** `almanac_api.h` defines request/ack lifecycle, but no backend has ever called `acknowledgeOpened`/`acknowledgeFailed`/`confirmClosed`. Mock host can test service consumption but no overlay backend exists to exercise it.
8. **Service-safe Finance phase:** The permitted model/service phase must be established from existing service contracts before connecting real Finance writes. "Game thread" is not a substitute for proof of service thread affinity. Until then, only synthetic host data is allowed.
9. **Font-manager warning:** An automated font-manager warning was addressed with source/native evidence in PR #80. Do not apply the suggested DWORD dereference without contrary matching-target evidence.

---

## 3. Seven smallest evidence requests

1. **Graphics creation timing / swap-chain identity:** Capture the game's `D3D11CreateDeviceAndSwapChain` IAT slot and a Present call stack (thread ID + module chain) on the target build; confirm Steam overlay chaining and whether creation already happened before plugin start. Default to unavailable rather than inventing a second interception route.
2. **Thread affinity:** Log thread IDs in Present and WndProc; confirm same-thread or document bounded marshalling with overflow behavior and gesture-release ownership.
3. **Render-state completeness:** Enumerate full render state saved/restored (OM targets, depth, blend, rasterizer, viewport, scissor) and test resize + fullscreen + alt-tab + device removal + window recreation.
4. **Native-modal signal:** Identify a reliable native-modal signal (game state flag or window focus) to gate overlay input; establish that failed capture is not interpreted as an empty native UI.
5. **Dev-gate mechanism:** Decide and document the dev-gate mechanism (recommend compile-time `ALMANAC_DEV_OVERLAY` + INI `dev_overlay=1`, both required); confirm no auto-open during menus/load.
6. **Dependency/legal:** Add a `LICENSE` file and reconcile with GPL-3.0-only ImGui/RepublicMP adaptation before any code reuse; preserve ImGui MIT notice; handle Earthworks SDK hash discrepancy (API 4 numbering alone is not a complete compatibility check).
7. **Host-side lifecycle:** Add mock-host tests for a hypothetical backend consuming `TSM_SERVICE_ALMANAC` (acknowledge success/failure/close); verify world-epoch vs panel-generation separation and world transition invalidation.

---

## 4. Conditions attached to the synthetic-prototype Go

The synthetic-prototype Go is granted only under all of the following conditions:

1. **Read-only synthetic data only:** Render `SyntheticOverlayView` (fixed invented RUB treasury, policy fields unavailable). No live Finance provider calls, no game-owned object writes, no save operations.
2. **Conspicuous synthetic-data banner** must remain visible in any development integration.
3. **No input capture:** Noninteractive drawing only. No clickable Close, no text editing, no Apply/Cancel. The prototype's `DrawOverlayPrototype` returns a close action to the lifecycle owner; it does not destroy resources from inside drawing.
4. **No native sidebar modification:** Do not change the eight native sidebar entries or write `game+0xC650` for the mod launcher.
5. **No terrain/facility hooks:** Do not install Earthworks' terrain, facility, save, construction or vehicle-filter hooks or its newer-version native bindings.
6. **No graphics/input hooks yet:** The prototype installs NO Present, WndProc, DirectInput, native Draw or sidebar hook. Caller must own ImGui context, frame boundaries, rendering resources, coordinate/font scaling and input routing.
7. **Dev-gated by default:** Disabled by default, explicitly configured, no live policy writes. A shortcut is a candidate ONLY after text/native-modal/readiness gates and key ownership are verified; otherwise report it blocked.
8. **No hot unload promise:** Require resident lifetime until safe unpatch, callback quiescence and resource retirement are actually supported and proved.
9. **Partial installation failure must not invite unloading:** Once callbacks point into the DLL, failure must not invite unloading it.
10. **No Earthworks code copy until provenance handled:** Earthworks-adapted overlay/underlay code has GPL/RepublicMP provenance; do not copy until attribution/distribution obligations are handled.
11. **Host-only bridge/controller remain the integration seam:** All public operations on one externally serialized owner thread; bridge mutex is not a transaction with the game or tax providers.

---

## 5. Conditions that block implementation, deployment or player acceptance

### Block implementation
- Graphics creation timing / swap-chain identity unverified (§3.1).
- Thread affinity unverified (§3.2).
- Render-state restoration incomplete (§3.3).
- Native-modal signal unidentified (§3.4).
- Dev-gate mechanism undecided (§3.5).
- Service-safe Finance phase unproved (blocker 8).
- Font-manager DWORD dereference not applied without contrary matching-target evidence (blocker 9).

### Block deployment
- No Windows DLL/loader build has occurred.
- No GPU/input validation has occurred.
- No native launcher test has occurred.
- Host tests are not evidence that native renderer refactor or overlay integration is safe in-game.

### Block player acceptance
- No disposable-copy test has been run.
- No visual inspection of the panel has occurred.
- No click-through/shortcut-leakage verification.
- No DPI/resolution-change verification.
- No pause/menu/load/new-world transition verification.
- No backend-failure verification.
- No supported-overlay coexistence verification.

---

## 6. Exact proposed implementation file scope

New plugin (isolated, dev-gated), outside normal plugin build discovery until approved:

- `plugins/almanac_overlay/almanac_overlay.cpp` — DXGI bootstrap adapted from Earthworks pattern, synthetic data only, no game state writes, no hooks until gate approved.
- `plugins/almanac_overlay/almanac_overlay.ini` — `enabled`, `dev_overlay`, hotkey, geometry (UTF-8 no BOM).
- `plugins/almanac_overlay/almanac_overlay_geometry.h` — pure hit-test/layout helpers (host-testable).
- `src/almanac_overlay_api.h` — optional; or reuse `ui_overlay_api.h` + `almanac_api.h`.
- `tests/almanac_overlay/almanac_overlay_geometry_test.cpp` — host tests.
- `docs/` updates: engine note, claim ledger entry, dependency/license note.

Untouched: `plugins/almanac_button/`, `plugins/ui_overlay/`, `src/almanac_api.h` (consumer only), `prototypes/almanac_overlay/` (reference, not modified).

No changes to `src/tesmio_api.h` or host ABI. No new cross-DLL ABI in this pass.

---

## 7. Eleven-item disposable-copy test checklist

1. Copy game to disposable location; install TesmioLoader + plugin DLL + INI.
2. Set `dev_overlay = 1` (and compile flag if used); confirm overlay appears only when gate is on.
3. Launch game; verify no crash at startup and log shows "Present hook installed" (when hooks are eventually approved).
4. Toggle overlay hotkey; verify show/hide without game state change.
5. Resize window and toggle fullscreen; verify no crash and overlay re-anchors.
6. Alt-tab away and back; verify overlay recovers.
7. Verify game input (camera, selection, panels) works when overlay hidden.
8. Verify overlay does not capture input unless explicitly enabled (dev mode).
9. Open native modals (pause, dialogs); verify overlay input is suppressed.
10. Remove DLL; verify game runs vanilla with no log errors.
11. Report log excerpts and screenshots; do not treat absence of crash as acceptance of correctness.

Expected failure behavior: on any fault, plugin logs a FAIL-STOP line and ends the game process (per `almanac_button` README fail-stop convention); does not attempt repair. Treat that session as suspect.

---

## 8. Hook target identity / chain / ABI audit

- **Preferred route:** No new executable mid-function interception. Prefer IAT slot patching (as `ui_overlay` does for `C3D_TERRAIN::Render`) or vtable slot patching (as Earthworks does for Present slot 8), with full whole-instruction relocation audit if any inline hook is proposed.
- **Earthworks reference:** Patches vtable slot 8 of the real swap chain obtained in `CreateDevice` (after Steam init); re-entry guard via `thread_local int presentDepth`; OM targets saved/restored per frame; RTV recreated per frame then `CleanupTarget()`. This is a reference, not an approved chain.
- **Unsafe site:** `0x1403745B6` (9+8 byte RIP-relative) is unsafe for the 17-byte non-relocating loader helper. No hook approved or specified.
- **Coexistence:** Establish coexistence with other overlays and existing import-slot owners. One service owns each process-wide hook. Repeated registrations must not stack Present/WndProc hooks.
- **Partial installation failure:** Must not leave an active hidden input sink. Once callbacks point into the DLL, failure must not invite unloading it.

---

## 9. Input ownership

- **Current state:** No input ownership is implemented. The prototype installs no WndProc hook.
- **Design contract:** PanelState is single-thread-owned. Present and WndProc must not concurrently mutate it. Select and verify one owner thread; marshal other callbacks through a bounded synchronized event queue. Do not hold a model/service lock while calling renderer, native code or client callbacks.
- **Click-through:** Claim input only for visible, enabled overlay regions or a gesture already owned by the overlay. Drawing above native UI does not automatically grant input ownership.
- **Keyboard:** Text editing must suppress corresponding game shortcuts in all verified input paths, not just `WM_CHAR`. No blanket global keyboard suppression. No copying native latch offsets without matching-target evidence.
- **Escape:** First closes a child overlay popup, otherwise requests panel close. Consume only the Escape sequence we own; leave native Escape untouched while closed.
- **Focus loss:** Releases capture and pressed-key/button state; need not destroy or cancel the draft.
- **Native-modal arbitration:** Must be established before enabling the launcher. If modal/occlusion arbitration is unknown or stale, disable launcher activation/overlay interaction requiring that information.

---

## 10. Dependency pinning and licensing / provenance

- **ImGui:** v1.91.9b @ `f5befd2d29e66809cd1110a152e375a7f1981f06` (MIT, Omar Cornut and contributors). Preserve `LICENSE.txt` with any future vendored/distributed copy. Prototype-only selection; not a decision to change Earthworks or production SDK.
- **Earthworks:** GPL-3.0-only (adapted from RepublicMP, pinned at `630775724abd5e3346eb59f9c66e2873128d74a4`). Do not copy adapted code until attribution/distribution obligations are handled.
- **TesmioLoader:** 0.3.6, commit `7d6bb8943ca5f6af817d8e7e6cf5be2c7faa818a`, API 4. SDK hash discrepancy remains documented; API 4 numbering alone is not a complete compatibility check.
- **WRSRE repo:** No `LICENSE` file exists. Licensing/provenance must be resolved before any adapted code is distributed.
- **Font-manager warning:** Addressed with source/native evidence in PR #80. Do not apply the suggested DWORD dereference without contrary matching-target evidence.

---

## 11. Existing host-side lifecycle / session / result contracts

- **OpenRequestPolicy** (`plugins/almanac_button/almanac_open_state.h`): Closed/Pending/Open state machine; `Activate`/`AcknowledgeOpened`/`AcknowledgeFailed`/`ConfirmClosed`. Host-testable, single-threaded, non-atomic.
- **Almanac API** (`src/almanac_api.h`): `TSM_SERVICE_ALMANAC` v1; `requestOpen`/`isOpen`/`isPending`/`acknowledgeOpened`/`acknowledgeFailed`/`confirmClosed`. Launcher only emits; backend acknowledges.
- **Overlay bridge** (`src/almanac_overlay_bridge.h`, documented in `OVERLAY_BRIDGE.md`): one-slot transport; Busy on saturation; monotonic request IDs; world/panel/revision/readiness invalidation; Take/Complete/Consume; no pointers or STL ownership across DLL boundary.
- **Overlay controller** (`src/almanac_overlay_controller.h`, documented in `OVERLAY_CONTROLLER.md`): joins PanelState, FinanceView and bridge; synthetic backend protocol; world epoch vs panel generation separation; Apply deduplication; close-vs-pending-Apply ordering.
- **Panel lifecycle** (`PANEL_LIFECYCLE.md`): two distinct generations (world epoch vs panel generation/close ticket); every mutating command carries world epoch, panel generation, snapshot revision and unique request ID.
- **Finance binding** (`FINANCE_EDITOR.md`, `FINANCE_SNAPSHOT.md`, `FINANCE_SESSION.md`, `FINANCE_EXECUTOR.md`): single-thread-owned FinanceEditor; synthetic controller does not enforce FinanceEditor load/validation/reload eligibility; UI editing controls must remain disabled until adapter reports successful edit-session load.
- **World transition:** Stop accepting edits, release input claims, invalidate model data/queued old-world commands, discard draft edits without applying, request closure on UI owner. Do not recreate PanelState or falsely acknowledge backend destruction.

---

## 12. Limits and non-approvals

- No independent child-review tool is exposed in this environment; no independent approval is claimed.
- Host tests are not evidence that native renderer refactor or overlay integration is safe in-game.
- Build success is not runtime acceptance.
- The review result is still required; this packet is not an approval.
- No builds, dependency-fetch scripts, deployment or game operations were performed in this review pass.
- Sidebar geometry investigation was not repeated.
- The native sidebar launcher remains inconclusive; no patch at `0x1403204EB` is approved.

---

*Published as `docs/reviews/almanac-overlay-backend-review.md` per review request.*
