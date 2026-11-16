# Earthworks-derived prototype styling

`panel_style.cpp` extracts the style block from NavySeal5/wrsr-Earthworks, `src/ui/overlay.cpp` (local snapshot: `Mods for analysis/wrsr-Earthworks-main`). Source repository: https://github.com/NavySeal5/wrsr-Earthworks. The exact snapshot is retained in WRSRE commit `670eb5315f9d3240bb3467e47af9f1bef8f12a40`; no upstream Earthworks commit identity is asserted. SHA-256 of the source overlay.cpp: `fb81d45691aace1600260953520baa1dde32ea137824efda17ce0c3b79793782`.

That file bears `SPDX-License-Identifier: GPL-3.0-only` and credits its DXGI bootstrap, re-entry protection and Win32 input approach to RepublicMP, E3418 and contributors, commit `630775724abd5e3346eb59f9c66e2873128d74a4`. The extraction retains that provenance; no bootstrap/input code is included in this extraction. Changes: wrap the unmodified style block in `almanac::ApplyEarthworksPanelStyle`; omit font loading, native textures, cursor art, hooks and runtime dependencies. Added header is likewise marked GPL-3.0-only.

The GPLv3 text is copied verbatim into `GPL-3.0.txt` from Earthworks' `LICENSES/GPL-3.0.txt`. Preserve these notices and corresponding source with redistribution, and resolve GPL obligations for any combined distributed binary. This is NOT a license assignment for the rest of WRSRE. No deployable plugin or release package is produced here.

Dear ImGui remains a separate MIT dependency. Its source and LICENSE.txt are checked at the pinned version by the host runner, not vendored by this extraction. Include its MIT notice with any future distributed copy. No game-owned images or fonts were copied.
