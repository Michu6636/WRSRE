# Almanac UI shell and provider API

## Purpose

This document describes the reusable, mod-owned Almanac UI API and internal-page
shell implemented in worktree `05-almanac-ui-shell`. It consumes the integrated
`AlmanacHostSurface` and validation helpers from worktree 01. It provides no
production hook, no native tab indices, and no economic state.

Owned files:

- `src/almanac_ui_api.h`
- `plugins/almanac/almanac_shell.h`
- `plugins/almanac/almanac_shell.cpp`
- `plugins/almanac/almanac_pages.h`
- `plugins/almanac/almanac_pages.cpp`
- `tests/almanac/almanac_shell_test.cpp`
- `tests/almanac/test_almanac_shell.py`
- `docs/almanac/05-ui-shell.md`

## Page IDs

Stable mod-owned IDs (not native tab indices):

- `ALMANAC_PAGE_OVERVIEW` (default)
- `ALMANAC_PAGE_FISCAL_POLICY`
- `ALMANAC_PAGE_MONEY_FLOW`
- `ALMANAC_PAGE_PRIVATE_ECONOMY`
- `ALMANAC_PAGE_PRICES_CITIZENS`
- `ALMANAC_PAGE_LOGISTICS_PRODUCTION`
- `ALMANAC_PAGE_CAUSATION`

Navigation is internal to the shell. `almanac_shell_navigate` validates the ID;
`almanac_shell_go_back` returns to the previous page or Overview at the root.

## Shell/provider contract

- `AlmanacShell*` is an opaque handle.
- Providers register with `almanac_shell_register_provider(shell, page, populate, userData)`.
- Returns a handle >= 1 on success and 0 when full or invalid.
- `almanac_shell_unregister_provider` removes a provider and invalidates its
  callback lifetime immediately.
- Providers are never invoked after unregistration.
- `almanac_shell_provider_count` reports active providers.

## Snapshot and threading rules

- Provider population is separate from rendering.
- `almanac_shell_populate(shell, host)` is the only path that invokes providers.
- Providers fill bounded `AlmanacPageSnapshot` structures (`ALMANAC_MAX_ROWS`).
- Rendering (`almanac_shell_render`) performs no provider calls, I/O, service
  discovery, hooking, or unbounded allocation.
- Providers must not retain locks or pointers after their callback returns.
- Snapshots carry `hostGeneration`, `frameNumber`, `populated`, `available`,
  `stale`, and `coherent`.
- Incoherent or oversized snapshots are discarded.

## Value classes and units

- `ALMANAC_VALUE_ACTUAL`, `ALMANAC_VALUE_DRY_RUN`, and
  `ALMANAC_VALUE_ESTIMATED` are structural values.
- They are never silently aggregated together.
- `almanac_aggregate_money` creates separate buckets for differing value class,
  currency, or unit.
- Totals are per class and per currency/unit.

## Host consumption

- Render only when `host_surface_can_render` is true.
- Geometry alone is insufficient; visibility, selection validation, geometry,
  DPI, flow layout, frame identity, and lifecycle capabilities are required.
- Clipping is applied only when `host_surface_can_clip` is true; enabled but
  invalid or unsupported clipping prevents rendering.
- Scrolling is used only when `host_surface_can_scroll` is true.
- Pointer input is accepted only when `host_surface_can_receive_pointer` is
  true: renderable surface, pointer capability, valid non-empty in-bounds event
  from the current frame.
- Host disappearance clears snapshots and stops rendering.
- Lifecycle invalidation clears cached page state.
- No raw game pointer or native panel index is accepted.

## Empty and unavailable states

- Pages without providers render an explicit empty/unavailable state.
- No fake economic values are inserted.
- Missing sources render "unavailable" and disable dependent rows.
- Stale data carries a stale marker.

## Graceful degradation

- Missing capabilities produce safe no-ops or read-only behaviour.
- Missing providers leave the page empty but renderable.
- Clipped or out-of-bounds input is rejected.
- DPI changes rescale row height, padding, and clip rectangle together.

## Deferred / integration still required

- Native hook installation and plugin entry-point registration.
- Final economic data-provider wiring for each page.
- Production host enablement.
- Integration, Windows-build, and native-runtime acceptance rows remain
  deferred; mock tests do not mark them passing.
- City Hall remains an unchanged fiscal-control fallback.
