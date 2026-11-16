# Global Almanac: current technical findings

**Status:** Planning and investigation; this is a concise guide to evidence, not an implementation specification. The detailed [independent export review](almanac-export-review.md) records exact addresses, caveats, and corrections. The older [investigation](almanac-investigation.md) and [native-evidence notes](almanac-native-evidence.md) are historical and contain superseded conclusions.

## What we know

| Area | Evidence-supported finding | What it means |
|---|---|---|
| Current row | The current screenshot has eight Economy & Trade tiles. A read-only snapshot also found eight entries in the selected trade subvector. | Design for the existing eight and a separate Almanac button; the snapshot was non-atomic and does not prove eight draws in a particular frame. |
| Button interaction | Existing code in [`ui_panels`](../plugins/ui_panels/ui_panels.cpp) contains native button, mouse, and collision machinery, including the City Hall TAX precedent. | We do **not** need to prove that buttons in general can work. Reuse the established interaction where applicable, while checking its integration in the global sidebar. |
| Native dispatch | Static host code selects between two trade handlers based on a game-state flag. Matching bytes and snapshots do not prove which handler executed in a frame. | The integration must consider the active path and avoid assuming building-window behavior applies unchanged. |
| Native subentries | A proposed append changes the observed normal ordering in one handler; another handler's normal loop omits the final entry under the analyzed conditions. | Blindly appending to the native collection is not a demonstrated safe way to obtain the desired button. A dynamically placed mod-owned button is a candidate, not an established implementation. |
| Content | On some selections an existing content helper is called before a proposed late suppression point; it performs drawing and other behavior. Native paths use different output and cleanup routes. | Suppressing only a later block can leave vanilla content or alter required state. A safe replacement/return flow remains the primary integration question. |
| Cleanup and hooks | A normal handler path initializes a local object and later runs several cleanup calls. Jumping directly to the epilogue skips them; the supplied helper excerpts strongly indicate an allocation/release pair when the relevant vector is nonempty. The loader's inline hook copies instructions without relocating RIP-relative instructions. | Do not assume an epilogue jump or a copied-instruction trampoline is safe. Allocation count, helper side effects and complete hook safety are not proven. |
| Mouse and drawing | Preserved PE import records link the relevant game imports to `C3D_INPUT::GetMouseSolid`, `C3D_PANEL2D::Collision`, and `C3D_PANEL2D::Draw`. DLL excerpts show a conditional Collision rejection path and Draw geometry checks. | We know the native interaction mechanisms, but their runtime state, inherited clipping and the global row's behavior are not fully established. The prior claim that particular unexamined patch functions perform clipping is unsupported. |

### Why test the new button's response?

It is a **context-specific integration check**, not a research project to rediscover how buttons work. Reusing an existing working button is the preferred starting point. Testing the actual new button at its dynamically computed row position verifies that the global sidebar's input routing, clipping, selection state, and native controls coexist with it. It should be an acceptance test of the proposed integration, not a reason to delay all other work or demand proof of a new generic button system.

## Placement and replacement questions

1. **Placement:** How should the mod obtain the live row origin, pitch, visible count and scale, and fit its button beside the visible native tiles without overlap or a new line? The snapshot-based ninth rectangle is only conditional arithmetic, not a fit or clipping guarantee.
2. **Integration:** Which sidebar path is reached, and at what point can the established button interaction be drawn and handled without colliding with native selection or consuming the wrong input?
3. **Content:** How can the Almanac replace/suppress the vanilla body at the right point while preserving outputs, input effects, cleanup and state? The conditional native content helper can draw before a later proposed boundary.
4. **Navigation:** What happens on switching tabs, closing the sidebar, and returning to vanilla content? Static leads exist; full behavior remains unverified.
5. **Hook safety:** Does the actual integration site have safe instruction boundaries, relocated instructions where needed, and no unsafe incoming control-flow edges? No current hook location is approved.

An independent button in the same row and a separate global page are the preferred direction; an Economy & Trade page is the accepted fallback for the page location. Neither changes the same-row button requirement.

## Current direction — overlay handoff

The user selected in-game overlays for Almanac/macro panels; native controls
remain for supported native windows. The older native-popup research below is
historical, not the current panel implementation plan. See
[overlay design](almanac-overlay-design.md) and
[backend review scope](almanac-overlay-backend-review.md).
The native sidebar launcher remains unverified and must not be armed.
Host-tested policy and the isolated ImGui prototype do not establish Windows
plugin compilation, input isolation or in-game acceptance. In particular,
preparatory ui_panels renderer changes still require Windows/runtime review.

## Button implementation progress — 2026-11-14

Preparatory implementation, **not a working modal launcher**:

- `ui_panels` 6.16 separates `DrawPanelButtonAtRect` from widget layout/budget.
  Game, optional verified window (diagnostics only), fonts, clip permission,
  cursor-fallback permission, live state and press owner are explicit inputs.
  The helper is private to that DLL; a screen adapter/service is still needed
  before `almanac_button` can use it. No public API version changed.
- `src/ui_button_core.h` supplies validated, pixel-aligned bounds and the press
  policy used by the renderer. Collision globals, textured Draw, 9-patch rect
  and label derive from those bounds. Pressed/active feedback no longer shifts
  the quad away from the tested rectangle. Existing widget callers retain
  their OS-cursor fallback; the future launcher must disable it.
- `tests/almanac_button/ui_button_core_test.cpp` covers DPI geometry, invalid
  bounds, physical edges, delayed engine latches, release/re-arm and hover/live
  gating. This tests shared policy, not the native DLL or collision geometry.
- The trade-row hook, `DrawNinthTile`, its AABB test and its `game+0x5E6`
  writes are **not migrated yet**. Native row ordering is untouched. Do not
  deploy this as the completed launcher or infer click-through protection.

The modal prerequisite is narrower than finding an allocator. In cached raw
`ghidra-exports/decomp_1406EE2A0.c`, lines 127–139 can mark existing windows;
155–165 allocate/initialize a window and retain `param_3` at byte `+0x240`;
680–725 show existing-window changes, table insertion and game input-state
writes. The insertion ABI has subsequent corrections/closure in
`ghidra-index/6EE2A0.md`, `1E140.md` and `1E8C0.md`; do not treat the
three-argument decompiler call as the verified ABI. None of that establishes
an available mod-owned window kind or a modal lifecycle.

Next native evidence needed: one supported non-building window route from
creation arguments through dispatcher drawing, frontmost input ownership,
close/Escape, removal/destruction and resize/DPI updates. Establish whether a
custom draw/provider route exists without a fabricated building descriptor or
unknown kind. Do not call the generic creator or reuse building-window offsets
on the trade screen to bypass this gap. This Linux checkout has an engine DLL
but no locally available game executable/Ghidra project or Windows build tool;
no fresh native analysis, DLL build or runtime observation was performed.
Independent child review is unavailable in this session; this change is not
independently reviewed.

## Finance and tax service findings

Read-only presentation preparation (2026-11-14):
`src/almanac_finance_snapshot.h` and `src/almanac_finance_view.h` now build a
per-field availability model and fixed-capacity Finance rows from existing
public services. Integration contract, API limitations and host-test commands:
[`FINANCE_SNAPSHOT.md`](../tests/almanac_button/FINANCE_SNAPSHOT.md).
Taxes API v3 now adds a coherent configured-policy getter, consumed by this
model; v1/v2 remain published and older providers retain unavailable-field
fallbacks. This does not add native editing controls or native UI.

`src/almanac_finance_editor.h` now supplies the separate host-tested Apply/Cancel
controller: local percent/amount drafts, validation, optimistic conflict checks,
changed-field setters, dependency skips, explicit partial outcomes and policy
readback. It is not an atomic policy transaction or connected native widget.
Integration and limits: [`FINANCE_EDITOR.md`](../tests/almanac_button/FINANCE_EDITOR.md).

- [`currency_api.h`](../src/currency_api.h) exposes treasury and active-currency access. Consumers must handle unavailable services and version/size compatibility.
- [`state_ledger`](../plugins/state_ledger/state_ledger.cpp) holds its flow aggregation and closed-period HUD values privately; it does not currently publish a general snapshot service. Its flow total and the observed treasury change are separate values. A versioned snapshot with period, currency, validity, tracked flows, observed change, and unexplained difference is a **design candidate**, not an implemented API.
- [`taxes_api.h`](../src/taxes_api.h) v3 appends `getPolicySnapshot`: configured VAT/profit fractions, live property base, enabled flags and the property scaling rule copied under one provider lock. Legacy rate getters still report **effective zero when suspended**. Taxes 4.6 preserves exact-version v1/v2 publication; rebuilt enterprise_base/private_shops also fall back to those older providers. Provider/API/consumer tests extract the actual new production bodies; Windows DLL build and independent review remain pending.
- The property-tax control sets a base amount per enterprise, not a percentage. `SettlePropertyAll` scales that base by attractiveness when `property_land_value` is enabled (the shipped INI default), and uses it flat when disabled. The Almanac should present the active rule and call the authoritative setters rather than inventing parallel behavior.

## Evidence and limits

- [Independent review and corrections](almanac-export-review.md): detailed static claims, candidate paths and counterexamples.
- [Raw evidence directory](../export/almanac-evidence/): preserved Windows-agent exports and reports; originals should remain unchanged. Reports are leads and are not automatically proof.
- [DLL research approach](almanac-dll-research.md): how to inspect other mods without mistaking observed binary behavior for permission to copy it.
- The available local workspace does not include the running Windows game or its Ghidra project. A previously attempted debugger observation was blocked; matching call bytes are not execution evidence. A non-pausing, read-only panel snapshot has been approved for when the user can run the game, but even a successful snapshot would not prove which handler executed or that the new button can be clicked.
- Some preserved upstream evidence and the latest review changes may be uncommitted in the current workspace. Check the actual working tree before Git operations; do not overwrite original reports or treat workspace state as a published commit.
