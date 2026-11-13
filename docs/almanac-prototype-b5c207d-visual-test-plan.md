# Visual-check plan: Almanac draw-only prototype, default-on follow-up

**Status:** The one visual-only test previously approved under this plan was performed by the user in a disposable game/profile with a copied test save and cloud sync off. The user reports no intentional save, time advance, policy change, or button click, and a normal close. The screenshot shows the extra tile at the end of the Economy & Trade row, but the user reports that it repeats the crime-button artwork. The log contains 259 `game.ERROR` lines, many for missing game/save/workshop content; the user clarifies that the `saved_last` open/write errors at close are expected when exiting without saving. This is a one-view placement pass, not a working Almanac button or a general safety test. **Any further game run, even after an icon/rendering fix, requires separate explicit approval.**

## What the original check could answer

The prototype makes an extra native `C3D_PANEL2D::Draw` call after the eighth Economy & Trade tile, changing the shared X/Y rectangle pair. The check could show whether an extra draw appeared in that row in one chosen setup. It could not establish a finished button, click handling, a distinct icon, all vanilla input behavior, or general hook/render safety.

## Preserved evidence

The user's original screenshot and loader log are preserved byte-for-byte in [`export/almanac-evidence/almanac-button-visual-test-run1/`](../export/almanac-evidence/almanac-button-visual-test-run1/); the [result note](../export/almanac-evidence/almanac-button-visual-test-run1/result.md) records their SHA-256 values and assessment.

Observed in the supplied files:

- The log identifies loader b0.3.6, game v1.1.1.9, and plugin v1.5. It reports the plugin armed, `ninth tile drawn (first row pass)`, and subsequent `row ok` entries. There is no `FAIL-STOP` or `RECT DIRTY` line.
- The screenshot shows an extra tile in the same visible row, with the eight native tiles still visible and no obvious second row or overlap in this capture. The user reports the extra image looks like the crime button.
- The log contains **259 `game.ERROR` lines**, including 158 `IO_Building_Read BuildingType is NULL - temp` entries, missing resources/workshop files, two read errors under `5643 - Test`, and `setfocus` errors. It says `Workshop subscribed items num 0`. These entries point to missing game/save/workshop content in the run environment; they are not attributed to `almanac_button` by the available evidence.
- The log records open/write errors under `media_soviet/saved_last` around shutdown. Per the user, these are expected on a no-save exit and are not a failure by themselves. The user confirms this run used a disposable copy/profile and copied save with cloud sync off, made no intentional game-state changes or button clicks, and closed normally.

**Outcome:** placement is a **single-view visual pass**. Distinct Almanac artwork is a **visual failure / unresolved design requirement**. The user-reported disposable setup and normal close satisfy the containment intent for the one approved run; the log's unrelated/missing-content errors are not proof that the plugin caused a fault. They also do not establish safety in general. The exact DLL SHA-256/source revision is not recorded. No click test is established. Do not infer more than this.

## Boundaries for any separately authorized follow-up

- Use a fresh disposable game copy/profile with a copied test save. Keep the normal installation and valuable saves untouched; disable cloud sync for the test copy. A VM is optional if the copy can simply be discarded and recreated.
- Do not save, advance time, or change tax, wage, subsidy, or other policies. Do not click the prototype's tile; it has no click handling.
- Rebuild the exact, reviewed source revision in scratch and record the DLL SHA-256. Use only the disposable copy. Do not bypass a refused build guard.
- Stay present while it runs. The experimental hook can terminate or hang the game on certain faults. If it crashes or stops responding, close only the test game process and discard the test copy. Do not force faults or retry after an error.
- A future test is not covered by the prior one-run approval. Get separate user approval before launching the game again.

## Preparation checklist for a separately approved visual-only retest

1. Review the missing game/save/workshop-content errors and establish a clean, reproducible disposable environment before any new game launch.
2. Record the exact reviewed source commit, test-game build, rebuilt DLL SHA-256, Windows display resolution, game UI scale, and loader/plugin versions.
3. Confirm the test copy/profile and copied save are disposable, cloud sync is off, and the normal game/save locations remain untouched.
4. Capture a baseline Economy & Trade row without the candidate build, at the chosen resolution and UI scale; confirm the eight vanilla tiles appear in one row.
5. Put the exact candidate build only in the disposable copy and verify its INI resolves to `enabled = 1`. Do not alter the normal installation or its INI.
6. Only if separately approved, observe the same view briefly without saving or changing policy. Do not click the extra tile. Capture the same view and loader/plugin log.

## Stop conditions for a future approved run

- `FAIL-STOP` or `RECT DIRTY`, a hang, crash, or unresponsive game;
- plugin does not arm, the added tile is missing/misplaced, or any native tile moves, disappears, overlaps, wraps, or is clipped;
- the added tile shows unintended/borrowed vanilla artwork rather than a reviewed distinct Almanac visual;
- any unexpected game, UI, rendering, loader, hook, or save-related error occurs. The `saved_last` open/write errors observed at shutdown in run 1 are expected per the user when exiting without saving; by themselves, those lines are not a stop condition.

After any stop condition, do not continue playing or reuse the test save. Preserve logs and screenshots if available, close only the isolated game process if needed, and discard/recreate the copy. Do not repeat the test until the failure has been reviewed and a new test is separately approved.

## Limits

Even a successful visual retest would establish only that the reviewed image appeared as expected in one test setup and that the game remained responsive during that observation. It would not prove clickability, that all eight vanilla buttons still respond, layout at other resolutions/scales, hook safety under re-entrancy or other mods, or behavior on other game builds. The product Almanac still needs a dedicated button with its own identity, working input, global page lifecycle, finance information, and the requested tax controls.
