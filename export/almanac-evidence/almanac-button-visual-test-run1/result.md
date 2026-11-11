# Almanac button visual check — run 1

**Source:** user-provided screenshot and loader log, preserved byte-for-byte alongside this note. The uploaded log was named `tesmioloader.log`; its copy is named `tesmioloader-log.txt` so it is not excluded by the repository's `*.log` ignore rule. SHA-256 values:

- `economy-trade-row.png`: `d58b88af4ab75d018eaecf7f967de80d6f7ec156569a1078fd532173121d6089`
- `tesmioloader-log.txt` (uploaded as `tesmioloader.log`): `4dc838ff11a0fb7836d37b82c1ddcb0a79b22157ff88b46174b6ec36ae254b9d`

## Observed result

- The screenshot shows nine tiles in the Economy & Trade row, with the added tile rendered at the end and no obvious overlap or second row in this view.
- The user reports that the added tile appears to use the crime-button artwork.
- The loader log identifies `tesmioloader` b0.3.6, game v1.1.1.9 and plugin `almanac_button` 1.5. It records the plugin as armed (`17:58:47.687`), `ninth tile drawn (first row pass)` (`18:00:08.655`), and repeated `row ok` entries. It has no `FAIL-STOP` or `RECT DIRTY` entry.
- **The loader log also contains 259 `game.ERROR` lines.** They include 158 `IO_Building_Read BuildingType is NULL - temp` entries, missing save/workshop resources and files, read errors for two files under the `5643 - Test` save, errors opening/writing many files under `media_soviet/saved_last`, and three `setfocus` errors. The log also says `Workshop subscribed items num 0`. These may be environment/content/path problems and are not shown to be caused by `almanac_button`, but the captured run is not an error-free visual pass.
- The log records save-related open/write errors under `media_soviet/saved_last` at about `18:00:24`. It is not possible to tell from this log alone whether the game attempted a save/cleanup operation or whether game state was changed. The log references a save named `5643 - Test`, but does not establish that the whole game profile was disposable, that cloud sync was off, or that the normal saves were untouched.

## Assessment

**Placement:** a single-view visual pass—the extra draw appears at the end of the visible row. **Artwork:** failure for a distinct Almanac identity—the user reports crime-button artwork in the extra slot. **Overall clean/safety result:** inconclusive and does not meet the plan's no-unexpected-game-errors criterion because of the many `game.ERROR` records and save-path writes. Their cause has not been attributed to this plugin. The one-run approval is used; do not repeat or extend the game test without separate approval and review of these errors.

The prototype has no click handling. No clickability, response of the eight existing buttons, behavior at other resolutions/scales, or full hook safety is established. The screenshot/log do not record the DLL SHA-256 or exact source commit, so the tested binary cannot be independently identified.

The source confirms that the extra native `Draw` receives the same intercepted `self` panel object and arguments, with only the shared X/Y rectangle pair changed, and that the prototype provides no distinct Almanac icon/content. This is relevant to the duplicate appearance, but does **not** prove that `C3D_PANEL2D::Draw` owns or binds the crime artwork. The exact texture/icon path behind the observed image is unresolved and needs a Ghidra-capable static trace before proposing a code fix.
