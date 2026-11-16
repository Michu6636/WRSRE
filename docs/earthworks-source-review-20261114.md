# Earthworks source review — 2026-11-14

## Scope and verdict

Read-only source review of Michu6636/WRSRE commit `d9447c5787e9011edb65ccf9338711e06f7af8ac`, directory `Mods for analysis/wrsr-Earthworks-main` (introduced by commit `9815178`). Source was inspected through an ignored `.cache/earthworks-review` archive without switching branches or changing existing work. This report is the only tracked-location addition from the review. No builds, tests, game execution, deployment or integration were performed. No independent child-review capability is exposed here; this is a single-agent static review, not independently reviewed acceptance.

**Verdict:** useful reference for a standalone overlay panel and input/lifecycle design, not a solved native ninth-sidebar-button implementation. Do not copy the entire runtime. Preserve the existing Finance/editor/PanelState components and native eight-button behavior.

References below are relative to the Earthworks directory at the frozen commit unless explicitly marked WRSRE. Line numbers refer to that commit.

## 1. What actually creates the button and panel

- `src/ui/overlay.cpp:2014–2030`: StartOverlay patches the engine's `D3D11CreateDeviceAndSwapChain` import. The creation callback patches swap-chain vtable slot 8 (Present), retaining the original.
- `overlay.cpp:1867–1944`: initializes its own ImGui context and Win32/DX11 backends, installs a window procedure, loads art, then attempts the native underlay/occlusion hooks.
- `overlay.cpp:1945–2001`: Present renders ImGui, restores its previous context, chains original Present and contains a re-entry route through Present1. The render pass saves/restores the render target/depth view and releases its back-buffer render-target reference each frame. This is an architectural reference, not a verified overlay-chain compatibility guarantee.
- `overlay.cpp:876–905`: launcher is `ImGui::Button("Earthworks", ...)` in a separate borderless ImGui window. Position is explicitly `(70, displayHeight-132)` and size `150x42`; native-UI rectangle overlap hides it. This is fixed-pixel placement, NOT native flow layout, NOT a native tab inserted into the sidebar, and NOT evidence of game-DPI scaling.
- `overlay.cpp:910–953`: main panel is `ImGui::Begin("##EarthworksPanel", ...)` with custom paper art and header controls. Closing clears input/pick state and queues CloseSiteEditor. Native-looking artwork does not make it a native game window.
- `overlay.cpp:371–389,480–505`: handles focus loss, Escape ownership, mouse-release swallowing, text-input capture and forwarding unowned messages to the original window procedure.

**Consequence:** Earthworks bypasses the unknown native popup creator; it does not identify or validate that creator. Using this approach for Almanac would be a deliberate overlay design choice and a departure from our flow-based/no-absolute-screen-coordinate rule, requiring an explicit design decision. It must not silently replace the requested native sidebar integration.

## 2. API and game-version compatibility

Earthworks records game 1.1.1.13 / Steam build 24922914 and TesmioLoader b0.3.7 in `COMPATIBILITY.md`. Acceptance records differ in freshness: `COMPATIBILITY.md` and older `docs/STAGE_REVIEWS.md` entries say current-candidate checks remain open, whereas `docs/RELEASE_READINESS.md:25–28` records user-reported successful terrain editing, facility costs, Tesmio use and saving/loading, with Stage 13 geometry/editing accepted. That report lacks itemized logs/results and leaves other checks open, including the RML fallback. Preserve this newer reported acceptance without treating it as comprehensive compatibility proof or our own runtime observation.

**API correction:** WRSRE `src/tesmio_api.h` already declares `TSM_API_VERSION 4u`. API 4 alone is not a porting blocker.

I fetched the exact SDK header at the Earthworks dependency lock's TesmioLoader commit `7d6bb8943ca5f6af817d8e7e6cf5be2c7faa818a` using GitHub's contents API. Its bytes are identical to WRSRE's current `src/tesmio_api.h`:

    SHA256 33c9fae4acb1041708c7b1b4675b0eb4740f0af737e7a1968c0acb0c325fff3c

However, `dependencies.lock.json` and `CMakeLists.txt:25–28` expect:

    SHA256 5daaf51ec1f6a5f279868bb039f01719931dfba0be69312a75863073ceac04a6

The pinned header as fetched would fail this CMake hash check. This is a source/dependency reproducibility discrepancy, not an observed build failure (no build was run). Investigate whether a different header was used locally before reusing their dependency setup. The loader SDK pin is tagged 0.3.6 while the recommended runtime is b0.3.7; those are distinct facts, not automatically an ABI failure.

## 3. Native dependencies and porting hazards

`src/ui/game_underlay.cpp:19–40` hardcodes the newer version's UI routine RVA `0x317D30`, panel object, import slots and panel fields. Our cached target function is at RVA `0x317DB0`. Similar names or unchanged globals do not make the newer routine address portable.

Its native hook does **not** create the ImGui panel: `HookedUiRoutine` draws terrain wireframes before the game UI and captures native panel rectangles during the original UI pass (`game_underlay.cpp:371–450`). Native Draw interception records rectangles, which the overlay uses to avoid claiming native UI clicks. This is useful evidence of a native Draw ABI/field usage, but remains another game version's implementation—not independent validation for our executable.

Important findings before reuse:

1. **Input isolation is larger than a WndProc hook.** `runtime.cpp:1849–2090` includes 13 C3D input-import detours, DirectInput keyboard interception and native input-latch writes at RVAs `0xA54E90`, `0xA54E91`, `0xA54C91`. Copying only the visible button/panel will omit behavior intended to prevent click-through and game shortcuts while typing. Conversely, copying all input suppression risks blocking unrelated game controls or other mods. Audit each ownership/release path against our target.
2. **Whole runtime startup is unsuitable for Almanac.** InstallRuntime requires terrain-height lookup, frame/load hooks, construction-assignment filtering and vehicle-slot filtering before StartOverlay runs. An Almanac panel does not need those terrain/facility modifications.
3. **Hook installation is not transactional.** InstallRuntime patches imports and then installs hooks sequentially. A later failure returns false without undoing earlier patches. `plugin.cpp:69–71` deliberately keeps the DLL resident after partial installation. There is no exported stop/unload implementation in that file. Preserve residency requirements; do not assume unload/rollback safety.
4. **Occlusion failure must be reconsidered.** InstallGameUnderlay patches Draw before installing the UI routine hook. If the latter fails, `boundHost` becomes null but Draw capture may remain marked installed. The original Draw still chains, but no successful UI pass supplies rectangles. Occlusion queries can then return false for an empty table. This is a statically identified failure path, not an observed bug. For Almanac, failure should disable the dependent launcher/input claim rather than imply that native UI is unoccupied.
5. **DPI/layout needs redesign.** The launcher and much styling use literal pixel dimensions; no game scale multiplication is present in the launcher path. Some wheel routing converts client/display coordinates, but that is not a complete DPI/layout policy. Do not copy the fixed lower-left placement into our native sidebar integration.
6. **Overlay recovery is not established.** Initialization retains device/context/window state, and there is no complete device-loss, window-recreation or DLL-unload protocol demonstrated by the inspected overlay. Partial Initialize failures can occur after resource/context creation. A reusable service needs explicit cleanup/retry ownership. Do not advertise resize/device-loss/hot-unload support from this review.
7. **License obligations survive permission to reuse.** Adapted overlay/underlay files declare GPL-3.0-only and name RepublicMP provenance; Dear ImGui is MIT. `THIRD_PARTY_NOTICES.md` and `LICENSES/` preserve those notices. The author's permission does not by itself remove upstream obligations. Review the distribution/license arrangement before incorporating adapted code, and retain attribution and corresponding-source obligations as applicable. This is a licensing flag, not legal advice.

## 4. Reusable architecture

The strongest reusable ideas are independent of ImGui:

- `runtime.h:13,51`: commands carry a world generation; snapshots carry generation/revision.
- `runtime.cpp:539–555`: ResetWorld increments generation, resets transient/durable session state, closes the panel and clears pending commands under the data mutex.
- `runtime.cpp:747–749`: processing rejects commands from a previous world or while not ready.
- `runtime.cpp:2008–2026`: renderer reads a mutex-protected snapshot into reusable per-thread storage; command queue is capped at 64 entries. Enqueue currently returns no overflow result; improve that if applying the pattern to user-visible edits.
- `overlay.cpp:108–140,864`: UI stamps commands with its rendered generation rather than passing live game objects into the renderer.
- `overlay.cpp:371–389,947–950`: explicit Escape, focus-loss and close-path input ownership.

These reinforce our existing `src/almanac_panel_state.h`, Finance snapshots and explicit Apply/Cancel editor. They do not justify replacing those host-tested policies or letting a render callback mutate services directly. Use a small view/command adapter if a renderer is later chosen. Review cross-thread atomics/mutex ownership separately before adopting their globals; a full concurrency audit was not performed.

## 5. Workflow lessons worth adopting

Earthworks `AGENTS.md`, `docs/DECISIONS.md`, `docs/STAGE_REVIEWS.md`, `docs/RELEASE_READINESS.md`, `docs/CODE_STRUCTURE.md`, `VERSION`, dependency lock and build scripts provide useful concrete practices:

- One identified candidate: version source, DLL hash, build result, exact player checklist and observed acceptance are distinct records. Attach those identifiers to every test request instead of saying only “works”.
- Keep scope decisions and stage acceptance explicit. Reuse the format, not another project's user-specific permissions or old exceptions. Its AGENTS text does not authorize us to build, stage or playtest.
- Core policy stays separate from runtime hooks and rendering. Borrow narrow cohesive interfaces, not the two 2,000-line integration files wholesale.
- Pin dependencies and verify hashes—but verify that the pins actually reproduce, as the SDK discrepancy above shows.
- One current release/acceptance ledger should supersede stale status statements where readers encounter them. Earthworks' compatibility/stage/release disagreement shows why several independent “current status” documents can drift.
- Separate build, package, deployment and public upload authorization. Do not execute a script solely because its name says “release”: the latest stage record and code-structure notes say the release command now stages into an existing Workshop item folder, whereas older records describe package-only outputs. Read the actual script and destinations before use.
- Give player-run checklists for input leaks, Escape/focus loss, world changes, saving/loading and loader coexistence. Report source checks and player reports separately from our own runtime observations.

Do not replace WRSRE's independent-review gate with an Earthworks stage summary. No child Reviewer was available for this pass.

## 6. Recommended next step

Continue native-sidebar work using the matching executable evidence, while using Earthworks as a reference—not a drop-in dependency. The standalone overlay is an alternative only if the user explicitly chooses it; it has additional rendering, input, licensing and recovery responsibilities.

Before implementation:

1. Preserve Beam's original raw executable import/constant records and finish the XMM13/XMM14 reaching-definition/path proof. Its pasted report is useful but not the original import artifact. Correct the former clipping label: the reported 0x14086D220 symbol is DrawFromNew3PatchVertical, not a clipping setter. XMM6 is zeroed before the loop, not DPI at the position constructor.
2. Specify one renderer contract: layout-relative launcher placement, identical native eight-button behavior, open/close policy, input ownership, unavailable-state behavior and world-generation handling. Do not quietly change the native-sidebar requirement to Earthworks' floating launcher.
3. If the user instead selects ImGui, design a minimal shared overlay/input service using the already matching API header. Exclude terrain/facility filters, require safe failure when native occlusion cannot be established, account for overlay coexistence, and settle licensing/attribution before copying code.
4. Subject the bounded design/diff to independent review, then obtain implementation/build/runtime authorization as appropriate. This source review provides no runtime acceptance.

### Checks actually performed

- Inspected a frozen Git archive of the uploaded source and its records.
- Traced launcher/panel -> Present -> Win32/backend -> native occlusion and runtime input dependencies.
- Compared local API version and byte-for-byte header contents against the exact upstream SDK pin through GitHub's contents API.
- Inspected generation-tagged command/snapshot/reset paths and selected failure paths.
- Read workflow, build/dependency, licensing and latest acceptance records; reconciled conflicting acceptance statements explicitly.
- No compilation, test executable, native patching or gameplay operation. Scope is UI portability/workflow triage, not a full terrain, persistence, security or concurrency audit.
