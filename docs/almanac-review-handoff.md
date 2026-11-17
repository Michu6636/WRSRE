# Almanac branch synchronization / reviewer entry point

> **Historical synchronization packet.** Work subsequently merged through PRs #80, #81 and #82. The current entry point is [engineering status](almanac-engineering-status.md), including the now-present native adapter source, missing DLL integration, and review/acceptance limits. The inventory below describes the earlier handoff, not the latest tree.

This source handoff publishes Arena's previously uncommitted work so agents using `Almanac` can inspect the same files. It is NOT approval to deploy, arm the old sidebar hook, or call live Finance services from the prototype. The user authorized creating and merging this synchronization PR. Review gates below remain open after merge.

## Included work groups

1. Earlier native-button preparation: private rectangle rendering refactor in ui_panels 6.16, shared button policy and host tests. No new public screen API or standalone launcher. This changes existing native rendering code; Windows compilation and disposable-copy regression testing are still required before deployment.
2. Taxes 4.6/API v3 configured-policy snapshot, exact-version v1/v2 publication, enterprise_base/private_shops fallback consumption, Finance snapshot/view/editor policy and tests. The enterprise build banner now matches its 9.19 metadata.
3. Host-only panel lifecycle, Apply transport, controller, edit session and executor. Explicit stale/duplicate/rejection handling, no real scheduling or native integration.
4. Isolated ImGui drawing source in prototypes/almanac_overlay, outside plugin build discovery. Synthetic read-only data; no graphics/input hooks, development shortcut or live service calls.
5. Overlay direction/design/backend-review scope and Earthworks/Space Race research notes. No third-party dependency implementation is vendored by this PR.

## Read these first

- `docs/almanac-overlay-backend-review.md`: independent native-backend review scope.
- `docs/almanac-overlay-design.md`: ownership/input/lifecycle contract.
- `prototypes/almanac_overlay/README.md`: drawing source, pinned test dependency and limits.
- `tests/almanac_button/FINANCE_EXECUTOR.md`, `FINANCE_SESSION.md`, `OVERLAY_CONTROLLER.md`: host integration contracts.
- `docs/almanac-ui-direction-20261114.md`: reported launcher evidence and corrections.

Beam's previous conditional review could not read the scope file. Do not treat that incomplete handoff as final approval. Reconcile its actual seven evidence requests, conditions and test checklist against the now-published scope. No independent reviewer was available in this Arena environment, and no independent approval is claimed.

## Checks rerun for publication

Passed with strict C++17 warnings-as-errors: ui_button_core, almanac_finance, almanac_finance_editor, almanac_finance_session, almanac_finance_executor, almanac_overlay_bridge, almanac_overlay_controller, almanac_panel_state and existing almanac_button_geometry suites.

ASan/UBSan builds/runs passed for the first eight suites above. The production-body extraction tax-policy harness passed normally and with ASan/UBSan. Headless drawing test passed against upstream ImGui v1.91.9b at f5befd2d29e66809cd1110a152e375a7f1981f06, fetched into ignored cache. No OS/GPU window was created. `git diff --check` passed before staging.

No Windows DLL/loader build, deployment, game execution, GPU/input validation or native launcher test. Host tests are not evidence that the native renderer refactor or overlay integration is safe in-game.

## Excluded local artifacts

Generated review/extraction ZIPs, duplicate extraction exports and the old standalone agent-prompt file are preserved locally but not committed. Canonical cached disassembly already exists in export/140317db0_full_disasm.asm. Beam's published raw evidence is on the target Almanac branch under docs/evidence/almanac-launcher. Use Git source and the above entry points rather than relying on an ignored cache or a temporary archive.

## Next action after synchronization

Independent read-only review of the bounded synthetic graphics/input backend and host integration. Resolve blockers before installing native hooks. The native sidebar launcher is still inconclusive; no patch at 0x1403204EB is approved. A development activation mechanism is not implemented and must not bypass unverified readiness/modal/input gates.
