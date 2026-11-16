# Almanac overlay integration design

> **Design contract, not current implementation inventory.** See [engineering status](almanac-engineering-status.md) for existing source, missing integrations and acceptance gates.

Status: proposed technical contract, not implementation approval or runtime acceptance.
User decision: Almanac/macro panels use in-game overlays; supported additions to native windows retain native controls. The native launcher remains independently gated.

## Scope and boundaries

Create a small reusable overlay backend, with Almanac as its first client. The backend owns graphics integration, context/resource lifetime, layout scaling, input arbitration and panel hosting. Almanac owns content, draft state and commands. Existing `ui_panels` remains the native-window service; it must not be made callable outside its verified widget pass.

Do not copy Earthworks' terrain, facility, save, construction or vehicle filters. Do not install its newer-version native bindings. Do not change the eight native sidebar entries or write game+0xC650 for the mod launcher. No generic native popup creator is required for this overlay.

Rendering and input integration are high-risk native work. This document introduces no hook, cross-DLL ABI or dependency installation. The service boundary below is conceptual until reviewed. Earthworks GPL/RepublicMP provenance and ImGui MIT notices must be accounted for before copying/distributing adapted code.

## Ownership and threading

| Owner | Responsibilities | Prohibited |
| --- | --- | --- |
| Overlay backend | One owned ImGui context; Present integration; window-input routing; frame/resource lifetime; input ownership; availability reporting | Finance writes, native selection changes, terrain hooks |
| Almanac UI adapter | One PanelState; render read-only view; draft editing; submit bounded commands; display acknowledgements | Raw game pointers, direct native calls, service mutation from Present/WndProc |
| Verified model/service phase | Acquire coherent Finance snapshots; validate commands; call FinanceEditor Apply/Cancel; publish results | ImGui calls, unverified thread-affinity assumptions |
| Launcher adapter | Emit one activation into UI ownership after its own safety checks | Opening from arbitrary native stack context; editing native vector/selection |

The permitted model/service phase must be established from existing service contracts before connecting real Finance writes. “Game thread” is not a substitute for proof of service thread affinity. Until then, only synthetic host data is allowed.

PanelState is single-thread-owned. Present and WndProc must not concurrently mutate it. Select and verify one owner thread; marshal other callbacks through a bounded synchronized event queue. Do not hold a model/service lock while calling renderer, native code or client callbacks. Do not pass STL objects or allocator ownership across DLL boundaries. If a shared service is introduced, use versioned POD/callback contracts with struct-size guards and explicit lifetime ownership; design that ABI in a later bounded pass.

## Lifecycle: two distinct generations

- **World epoch:** invalidates snapshots, queued commands and edit drafts across world unload/load or loss of readiness.
- **Panel generation/close ticket:** use existing PanelState tokens for one open lifetime and individual close attempts. These are not world epochs and must not be conflated.

Every mutating command carries world epoch, panel generation, snapshot revision and a unique request ID. Validate all relevant identities before work. Publish a result for accepted, rejected, stale, busy or unavailable requests. Never silently drop an Apply request.

On world transition, first stop accepting edits and release input claims, invalidate model data/queued old-world commands and discard draft edits without applying. Request closure on the UI owner. Do not recreate PanelState or falsely acknowledge backend destruction. Late completions must still be drained and rejected/cleaned up by the adapter as necessary.

Focus loss releases capture and pressed-key/button state; it need not destroy or cancel the draft. Close/Escape discards uncommitted draft changes. Launcher activation while opening/open/closing is ignored under the existing PanelState contract, not treated as a toggle.

## Open/close adapter

1. Accepted launcher event is marshalled to the UI owner; RequestOpen queues exactly one request.
2. BeginOpen runs only when graphics, world and input arbitration are available. If initialization fails, release partial ownership and acknowledge failure only after no visible/interactive panel remains.
3. CompleteOpen acknowledges the panel shell. Finance data may still be unavailable; show an unavailable/loading state with Apply disabled.
4. Close button or owned Escape calls RequestClose. Finish the current owned click/key sequence so closing cannot deliver its release to the underlying game as a new action.
5. BeginClose removes panel interaction and rendering. CompleteClose succeeds only when backend-owned panel work/callback references are retired. Closing one client does not destroy the shared graphics context.
6. If graphics fails, release input claims immediately, invalidate backend availability and close the client through the same ownership path. A fail-closed panel means no stale UI claims and no edits—not blanket blocking of native input.

## Finance binding

Render `BuildFinanceView` from a published snapshot, including its separate-sampling/treasury limitations. Missing data is unavailable, never invented zero. Editing requires currency v4, taxes v3 and a successful FinanceEditor load. VAT/profit fields are percentages; property is an active-currency amount per enterprise, not a percent or final bill. Property scaling rule stays read-only.

The existing FinanceEditor is single-thread-owned and currently documented for a UI thread. Do not move it casually between callbacks. The future bridge must give it one serialized owner at the verified service-safe phase and update its integration documentation accordingly; the renderer owns only a copied draft. Until that phase is proved, this bridge is design-only. Load/SetValue/SetEnabled/Apply/Cancel all execute on the editor owner; rendering never accesses that live object concurrently.

An Apply command carries the complete copied draft and baseline identity; only one may be pending per editing session. UI disables resubmission until an acknowledgement arrives. Queue saturation returns an explicit Busy outcome without discarding the visible draft. Do not automatically replay Apply after a lost acknowledgement. Track the completed request ID so a duplicate cannot execute a second time. A close or world transition must not be presented as rolling back an already executed Apply. Distinguish queued cancellation from an operation already executing.

Preserve FinanceEditor's validation, conflict detection, partial-write status and readback semantics. The setters are not a transaction: show PartiallyApplied, ReadbackFailed and VerificationMismatch honestly. After a write attempt/conflict, a new session requires an explicit reload; do not silently reload and erase the result. Snapshot revisions are bridge bookkeeping, not a replacement for the editor's actual provider-policy conflict checks.

## Layout and input contract

- Panel contents use flow layout with scrolling and a viewport-relative initial anchor/size. Native launcher placement remains tied to verified native layout, not a fixed `(x,y)` copied from Earthworks.
- Establish one logical-to-framebuffer scale policy. Verify the relationship among game UI scale, Windows DPI, client coordinates and framebuffer coordinates. Convert draw and hit-test geometry consistently; do not multiply both game scale and OS scale blindly.
- Do not reskin every control before basic input/lifecycle behavior is verified. Art/fonts need their own provenance and distribution review.
- Claim input only for visible, enabled overlay regions or a gesture already owned by the overlay. Drawing above native UI does not automatically grant input ownership.
- A popup/modal owns its defined overlay interaction scope; do not turn an unrelated game's modal into an overlay-owned surface. Native-modal arbitration must be established before enabling the launcher.
- Text editing must suppress corresponding game shortcuts in all verified input paths, not just WM_CHAR. No blanket global keyboard suppression and no copying native latch offsets without matching-target evidence.
- Own complete mouse/key sequences. Focus loss, close, world transition and backend failure release claims and avoid stuck input; a consumed down event must not leak an actionable up event.
- Escape first closes a child overlay popup, otherwise requests panel close. Consume only the Escape sequence we own; leave native Escape untouched while closed.
- Native launcher activation must not also select a native tab or reach the world. It emits an event, not immediate drawing or Finance work from a mid-function detour.
- If modal/occlusion arbitration is unknown or stale, disable launcher activation/overlay interaction requiring that information. Do not interpret failed capture as an empty native UI.

## Graphics integration gates

Earthworks provides an implementation reference, not an approved patch. Before installing anything, verify the matching engine's creation/import path, load timing, hook-chain ownership and coexistence with Steam/other overlays. Define behavior if loaded after device creation. Never assume the first visible swap chain is permanently the game chain.

Use explicit states: Unavailable, Initializing, Ready, Suspended, Failed. Initialization must clean up partial resources before retry. Resize, minimization, device removal and window recreation suspend interaction until resources/coordinate transforms are valid. Save/restore all pipeline and ImGui state the backend changes, including on failure. Audit the actual pinned backend implementation rather than assuming one render-target restore is sufficient.

One service owns each process-wide hook. Repeated registrations must not stack Present/WndProc hooks. Do not promise hot unload: require resident lifetime until safe unpatch, callback quiescence and resource retirement are actually supported and proved. Installation failure must not leave an active hidden input sink.

## Bounded milestones and acceptance

1. **Design/evidence (current):** contract and corrected launcher evidence request. No native code. Review thread ownership, input failure paths and license/dependency provenance independently.
2. **Host-only bridge:** fixed-capacity events/results and synthetic renderer/model adapters. Prove world/panel token separation, full-queue feedback, Apply deduplication, close-versus-pending-Apply ordering, stale acknowledgements and cancellation. No Windows/ImGui dependency or hook. This is the next low-risk implementation candidate after design review.
3. **Minimal overlay backend:** only after graphics/input design and target evidence are approved. Start with a read-only synthetic panel, not live Finance writes. Test failure injection before enabling input.
4. **Native launcher adapter:** independently reviewed matching-target boundary and input route. No fabricated native entries, hardcoded eighth index or patch at 0x1403204EB using the current non-relocating helper.
5. **Finance connection:** verified service-safe owner phase, real snapshots first, explicit editing second. Preserve all unavailable/conflict/readback states.
6. **User-run disposable-copy checks:** identified build/hash; native eight buttons unchanged; no click-through/shortcut leakage; Escape/focus loss; UI/DPI/resolution changes; pause/menu/load/new-world transitions; backend failures; supported overlay coexistence. Build success is not runtime acceptance.

### Open evidence gates

- Launcher layout boundary, dynamic count/eligibility, clipping/space and complete live-state analysis.
- Reliable world readiness/epoch events and native-modal/input ownership for this target.
- Service-safe phase and queue ownership for Finance bridge.
- Graphics creation timing, coexistence and recovery/cleanup protocol.
- Dependency hash reconciliation and licensing/distribution arrangement before adapted code.

No independent child-review tool is exposed in this environment. This document is a proposed design and has not passed independent review. No builds, dependency-fetch scripts, deployment or game operations were performed in this design pass.
