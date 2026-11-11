# Visual-check plan: Almanac draw-only prototype, default-on follow-up

**Status: one visual-only test is approved under this plan but has not yet been run.** Test the rebuilt default-on follow-up, not the earlier `b5c207d` binary. Approval covers only the single disposable visual pass described here; any wider testing, fault injection, deployment to the normal game, or scope change requires separate approval.

## What this check can answer

The prototype attempts to draw one extra **backdrop tile** after the eight Economy & Trade tiles. A single visual check can tell us whether that backdrop appears in the expected row position while the eight existing tiles remain visible.

It cannot validate a finished Almanac button: this prototype has **no icon, label, click handling, or Almanac page**. It also cannot establish that the hook or extra draw is safe in all game states, screen scales, or mod combinations.

## Test boundaries

- Use a disposable game copy/profile and a copied test save. Keep the normal
  game folder and valuable saves untouched; disable cloud sync for the test copy.
  A VM is optional if the copy can simply be discarded and recreated.
- Do not save, advance time, or change tax, wage, subsidy, or other policies. The
  prototype has no click handling; do not try to click its backdrop.
- Rebuild the exact default-on follow-up for this test. Use only the test copy;
  do not alter the normal installation or bypass a refused build guard.
- The experimental hook can end or hang the game on a fault. Stay present while
  it runs. If it crashes or stops responding, close only the test game process
  and discard the test copy. Do not force faults or retry after an error.

## Before launch

1. Record the exact prototype commit (`31a9da3e4bbbcd7ef64044ca6a8a2d1142655046`), test-game build, rebuilt DLL SHA-256, Windows display resolution, game UI scale and loader/plugin versions. This is the default-on follow-up, not `b5c207d`.
2. Place one copied test save in the disposable profile. Confirm cloud sync is off and the copy can be discarded without touching the original game/save folders.
3. Capture a **baseline** screenshot of the Economy & Trade row with the prototype absent or disabled, at the chosen resolution and UI scale. Record that all eight vanilla tiles appear in one row.
4. Prepare the isolated copy with a fresh build of the default-on follow-up. Confirm its INI resolves to `enabled = 1`; do not change the normal install or its INI. Confirm the normal saves remain untouched.
5. Launch the disposable game copy and remain present for the short visual pass.

## One visual pass

1. Open the same test city and Economy & Trade view as the baseline. Do not advance time, change policies, or save.
2. Observe the row for a short, fixed interval (for example, ten seconds). Do not click the proposed ninth backdrop.
3. Capture the same view at the same resolution and UI scale. Save the screenshot and relevant loader/plugin log output outside the game/save folders.
4. If the game stays responsive and there are no warning/failure indicators, exit normally without saving, then discard or recreate the disposable copy. Do not run a second attempt on the same test state.

## Stop immediately if

- the process reports `FAIL-STOP` or `RECT DIRTY`, hangs, crashes, or becomes unresponsive;
- the plugin does not arm, the ninth backdrop is missing or misplaced, or any of the eight existing tiles move, disappear, overlap, wrap, or appear clipped;
- any unexpected UI, rendering, loader, hook, or game error occurs.

After any stop condition, do not continue playing or reuse the test save. Preserve the logs and screenshot if available, close only the isolated game process if needed, and discard/recreate the test copy. Do not repeat the test until the failure has been reviewed.

## Result criteria and limits

A **visual pass** means only that, in this one test setup, an extra backdrop was visible in the same row, without an obvious overlap or loss of the eight existing tiles, and the game remained responsive through the observation. A **visual fail** is any stop condition above. Record inconclusive results as inconclusive rather than changing the pass criteria.

Even a visual pass does not show that the ninth item is clickable, that all eight vanilla items still respond to input, that the layout works at other resolutions/scales, or that the hook is safe under re-entrancy, other mods, later frames or other game builds. The source prototype is already merged; this check does not establish that it is a finished or production-ready Almanac UI.
