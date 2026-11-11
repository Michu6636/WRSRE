# Bounded static-investigation prompt — Almanac button artwork and native interaction

You are a Ghidra-capable static-analysis investigator. Work read-only in the WRSRE repository and examine the current `almanac_button` source, existing Ghidra exports/projects, and preserved visual evidence at `export/almanac-evidence/almanac-button-visual-test-run1/`.

## Question

The user-provided screenshot shows the extra Economy & Trade tile using what appears to be the crime-button artwork. The accompanying loader log reports `almanac_button` v1.5 armed and `ninth tile drawn`; it also contains 259 `game.ERROR` lines, including save-path open/write errors. These are recorded in the evidence note but are not attributed to this plugin. In `plugins/almanac_button/almanac_button.cpp`, the prototype changes the shared X/Y rectangle pair and calls the native `Draw` again with the intercepted `self` and original arguments.

Determine, from static evidence, why that draw repeats the vanilla artwork and what a defensible path to a distinct, working Almanac button would require. Do not assume the existing backdrop-only hook can become a real button just by changing coordinates or drawing another time.

## Required analysis

1. Trace the Economy & Trade row's native draw setup far enough to identify the intercepted object's ownership and the source of its texture/icon state. Cite exact EXE/engine addresses and control-flow evidence; distinguish verified facts, inferred facts, and unknowns.
2. Trace any relevant object/texture creation, assignment, draw, and hit-test paths. Determine whether a separate native panel/texture object can be created safely, what its ownership/lifetime would be, and whether the loader/plugin APIs expose a supported route. Do not assume a `reputation.dll` sidebar-entry technique applies to this different row; compare it only where relevant and explain the differences.
3. Evaluate candidates for a unique Almanac visual and real click behavior that preserve all eight existing Economy & Trade buttons, the same-row requirement, and existing native input semantics. Include vanilla coexistence, clipping/layout, interaction ordering, click-latch ownership, and content open/close/return behavior. Do not move the City Hall TAX button. ImGui is deferred.
4. Identify what can be proven from existing static evidence and what needs a later, separately approved test. If no defensible design is established, recommend stopping rather than proposing a speculative hook.
5. Review the license note in the City Manager UI reference only as context: do not copy its code, assets, screenshots, fonts, or artwork.

## Boundaries

- Static investigation only: no source edits, builds, test harness execution, game launch, hook installation, deployment, save edit, click test, or tax/policy changes.
- Preserve existing evidence unchanged. Do not extract `Mods for analysis/City manager/assets/game_assets.zip`.
- Do not delete or bypass a Ghidra project lock. If a project appears locked, identify its owner/process and stop rather than running concurrent analysis.
- Do not open a PR or make GitHub changes.

## Deliverable

Write a concise report to `export/almanac-evidence/almanac-button-own-icon-investigation/REPORT.md`. Include methods and source/executable identity limits, exact evidence paths/addresses, a candidate comparison, unresolved risks, and a recommended next gate. Do not modify any other file.
