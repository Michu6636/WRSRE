# Visual-check plan: Almanac draw-only prototype `b5c207d`

**Status: plan only. This is not approval to install, enable, or run the prototype.** No game test has been performed. The user must explicitly approve any execution after reviewing this plan.

## What this check can answer

The prototype attempts to draw one extra **backdrop tile** after the eight Economy & Trade tiles. A single visual check can tell us whether that backdrop appears in the expected row position while the eight existing tiles remain visible.

It cannot validate a finished Almanac button: this prototype has **no icon, label, click handling, or Almanac page**. It also cannot establish that the hook or extra draw is safe in all game states, screen scales, or mod combinations.

## Non-negotiable boundaries

- Use only an isolated, disposable Windows VM or equivalent copy with a restorable snapshot. Disable cloud-save synchronization for this copy.
- Use a copied test save only. Do not open or modify valuable saves. Do not change taxes, wages, subsidies, game time, or other policies; do not save. The copy is disposable even if the game autosaves unexpectedly.
- Install the plugin DLL and set `enabled = 1` **only in the isolated copy**. Leave the normal game, loader, configuration and saves untouched. Do not change `addresses.ini` or enable `allow_unverified_exe` to bypass a refusal.
- Confirm the game build passes the plugin's own checks. If the plugin refuses to arm, stop; do not bypass the guard.
- The code can call `TerminateProcess` after certain faults, and a failed restore may leave the rectangle pair unknown. The loader log is not a dependable sole safety control: the plugin logs before attempting termination. Before any test, use a separately verified, time-bounded watchdog that targets only this test game's PID, including when no failure log line appears. Test the watchdog with a harmless dummy process first. If this watchdog or the disposable recovery setup is unavailable, do not run the game test.
- Do not test a forced fault, do not click the proposed ninth tile, and do not use the ordinary game installation.

## Before launch

1. Record the exact prototype commit (`b5c207dd865e7c9cc124d528323182dc17687e5a`), test-game build, DLL SHA-256, Windows display resolution, game UI scale and loader/plugin versions.
2. Preserve a clean VM snapshot and place one copied test save in the isolated save location. Confirm cloud sync is off and recovery works without using the original game/save folders.
3. Capture a **baseline** screenshot of the Economy & Trade row with the prototype absent or disabled, at the chosen resolution and UI scale. Record that all eight vanilla tiles appear in one row.
4. Prepare the isolated copy with the compiled prototype and change only its `almanac_button.ini` to `enabled = 1`. Confirm the normal install and saves remain untouched.
5. Start the PID-specific watchdog, then launch the isolated game copy.

## One visual pass

1. Open the same test city and Economy & Trade view as the baseline. Do not advance time, change policies, or save.
2. Observe the row for a short, fixed interval (for example, ten seconds). Do not click the proposed ninth backdrop.
3. Capture the same view at the same resolution and UI scale. Save the screenshot and relevant loader/plugin log output outside the game/save folders.
4. If the game stays responsive and there are no warning/failure indicators, exit normally without saving, then revert the VM snapshot anyway. Do not run a second attempt on the same test state.

## Stop immediately if

- the process reports `FAIL-STOP` or `RECT DIRTY`, hangs, crashes, or becomes unresponsive;
- the watchdog has to terminate the process;
- the plugin does not arm, the ninth backdrop is missing or misplaced, or any of the eight existing tiles move, disappear, overlap, wrap, or appear clipped;
- any unexpected UI, rendering, loader, hook, or game error occurs.

After any stop condition, do not continue playing or reuse the test save. Preserve the logs and screenshot if available, terminate only the isolated game process if needed, and revert the VM snapshot. Do not repeat the test until the failure has been reviewed.

## Result criteria and limits

A **visual pass** means only that, in this one test setup, an extra backdrop was visible in the same row, without an obvious overlap or loss of the eight existing tiles, and the game remained responsive through the observation. A **visual fail** is any stop condition above. Record inconclusive results as inconclusive rather than changing the pass criteria.

Even a visual pass does not show that the ninth item is clickable, that all eight vanilla items still respond to input, that the layout works at other resolutions/scales, or that the hook is safe under re-entrancy, other mods, later frames or other game builds. The prototype remains experimental and must not be merged or treated as the finished Almanac UI on the strength of this check alone.
