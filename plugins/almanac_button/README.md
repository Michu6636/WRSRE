> **Superseded prototype — disabled by default.** Use
> [`plugins/almanac_overlay_dev`](../almanac_overlay_dev/README.md) instead. This
> plugin is kept only as a disposable-test reference. As of 2026-11-18 the
> shipped INI **and** the missing-key fallback are `enabled = 0`; installing the
> DLL patches nothing until a tester opts in on a throwaway game copy.

# almanac_button (experimental prototype, disabled by default)

A clickable ninth tile beside the Economy & Trade row. It draws one extra
game-style backdrop tile after the eighth tile and checks for a click on it
(mouse over rect + click latch set, read-only). Clicking the ninth tile toggles
a mod-owned Almanac open/close state; clicking another native row button closes
the Almanac. The shipped INI and missing-key fallback set `enabled = 0`.

**Caution:** when installed, this experimental plugin attempts its guarded draw
hook automatically. It can end the game process on certain faults. It is not a
finished Almanac button. Disable the plugin with `enabled = 0` in the active INI
and restart.

**Texture:** The ninth tile attempts to load a distinct icon (`icon_inspectnumbers.png`)
via CreateManagedTexture + Load + TexBind. If the texture fails to load, the tile
shows repeated native artwork (fallback). Texture-icon ownership is partially
resolved (v2.12) but runtime verification is pending.

**Content suppression:** The plugin writes game+0x5E6 = 1 when Almanac is open.
This is NOT visual content suppression — native Draw still executes. It provides
a partial, mouse-dependent input effect (disables click handling + tint change).
See docs/engine/almanac-ui.md section 1.2.1.

**Fail-stop:** If the rect pair cannot be read, written, or restored, or the extra
Draw faults, the plugin logs a FAIL-STOP line and ends the game process. It does
not try to repair the rect pair or the import slot. The log line is evidence only.
Ending the process is what stops the game.

**Restoring after a normal test:** set enabled = 0 and restart the game.

**If the log says RECT DIRTY:** the saved rect pair could not be put back at
0x9BE2F0. The plugin ends the process and does not read the pair again. Treat
that session as suspect.

**Faults in the engine's own draw of the eighth tile are not caught by this
plugin.** They propagate as they would without it.

**Click detection requires GetMouseSolid** to be resolved from C3DDLL64.dll. If it
cannot be resolved, the plugin still arms but ninth-tile clicks are not detected.
This does not affect the eight native buttons.

The screenshot, loader log and result note are preserved at
[`export/almanac-evidence/almanac-button-visual-test-run1/result.md`](../../export/almanac-evidence/almanac-button-visual-test-run1/result.md).