# `Mods for analysis` — vendored third-party mod sources

**This directory is not part of WRSRE.** It holds upstream mod repositories and
released binaries that were copied in so the Almanac research could review them
statically. Nothing here is built by `build.bat`, linked into `wrsre.dll`, or
covered by WRSRE's own licence.

## Why it is tracked

Several tracked documents cite files here by path and hash, for example:

- [`docs/almanac-earthworks-port-map.md`](../docs/almanac-earthworks-port-map.md) — compares Earthworks overlay source against WRSRE's panel
- [`docs/earthworks-source-review-20261114.md`](../docs/earthworks-source-review-20261114.md) — read-only source review
- [`docs/reputation-sidebar-ui-mechanism.md`](../docs/reputation-sidebar-ui-mechanism.md) — mechanism review of `reputation.dll`
- [`docs/reviews/almanac-overlay-backend-review.md`](../docs/reviews/almanac-overlay-backend-review.md) — cites Earthworks `overlay.cpp` line ranges
- [`prototypes/almanac_overlay/earthworks/NOTICE.md`](../prototypes/almanac_overlay/earthworks/NOTICE.md) — records the exact source snapshot and SHA-256

Deleting this tree would invalidate those citations, so it is retained.

## Handling rules

- **Read-only.** Do not build, run, install or load anything from this directory.
  Some entries are prebuilt DLLs (`Frontline/dinput8.dll`,
  `Diplomacy/tesmioloader/the build/plugins/reputation.dll`). They are data, not
  code to execute. WRSRE has not run any of them.
- **Never stage into a game.** These are other authors' mods with their own
  installers. Do not copy their DLLs into `build\plugins\`.
- **Vendored upstream files keep their own licences, agent files and CI.**
  `wrsr-Earthworks-main/AGENTS.md`, `wrsr_spacerace-main/CLAUDE.md` and similar
  files are upstream artefacts. They do **not** apply to WRSRE work, and the
  root [`AGENTS.md`](../AGENTS.md) does not extend to them.
- **Kept for fidelity.** Some files carry CRLF line endings and their own
  formatting. Do not normalise them: the recorded hashes depend on the bytes.

## Contents

| Path | Upstream | What WRSRE used it for |
|---|---|---|
| `wrsr-Earthworks-main/` | NavySeal5/wrsr-Earthworks | D3D11 `Present` hook + ImGui overlay reference; panel styling extraction (GPL-3.0, see `LICENSES/` and `THIRD_PARTY_NOTICES.md`) |
| `wrsr_spacerace-main/` | WRSR Space Race mod | Building/vehicle model layout and package structure reference |
| `Diplomacy/` | WRSR Reputation (Workshop 3797587125) | Sidebar row-item and reputation UI mechanism study |
| `Frontline/` | Frontline mod | Initial triage of third-party DLL injection (`dinput8.dll`) |

## If this tree ever moves

Update the five citing documents listed above and
`prototypes/almanac_overlay/earthworks/NOTICE.md` in the same change. A move
without those edits creates broken evidence links.
