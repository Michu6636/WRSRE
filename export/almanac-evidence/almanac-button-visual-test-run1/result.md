# Almanac button visual check — run 1

**Source:** user-provided screenshot and loader log, preserved byte-for-byte alongside this note. The uploaded log was named `tesmioloader.log`; its copy is named `tesmioloader-log.txt` so it is not excluded by the repository's `*.log` ignore rule. SHA-256 values:

- `economy-trade-row.png`: `d58b88af4ab75d018eaecf7f967de80d6f7ec156569a1078fd532173121d6089`
- `tesmioloader-log.txt` (uploaded as `tesmioloader.log`): `4dc838ff11a0fb7836d37b82c1ddcb0a79b22157ff88b46174b6ec36ae254b9d`

## Observed result

- The screenshot shows nine tiles in the Economy & Trade row, with the added tile rendered at the end and no obvious overlap or second row in this view.
- The user reports that the added tile appears to use the crime-button artwork.
- The loader log identifies `tesmioloader` b0.3.6, game v1.1.1.9 and plugin `almanac_button` 1.5. It records the plugin as armed (`17:58:47.687`), `ninth tile drawn (first row pass)` (`18:00:08.655`), and repeated `row ok` entries. It has no `FAIL-STOP` or `RECT DIRTY` entry.
- The log also contains 259 `game.ERROR` lines, including 158 `IO_Building_Read BuildingType is NULL - temp` entries, missing resources/workshop files, two read errors under the `5643 - Test` save, and `setfocus` errors. It reports `Workshop subscribed items num 0`. These entries indicate missing game/save/mod content in the run environment; they are not attributed to `almanac_button` by the evidence.
- The log records open/write errors under `media_soviet/saved_last` around shutdown. The user confirms these are normal when exiting without saving. The user also confirms the run used a disposable game/profile and copied test save with cloud sync off; they did not intentionally save, advance time, change policies, or click a button, and the game closed normally.

## Assessment

**Placement:** a single-view visual pass—the extra draw appears at the end of the visible row. **Artwork:** failure for a distinct Almanac identity—the user reports crime-button artwork in the extra slot. The supplied log shows no plugin fail-stop/dirty-rectangle marker, and the user reports a normal close from a disposable copy. The other `game.ERROR` records appear to concern missing game/save/workshop content, but there is no baseline to classify each one and they are not shown to be caused by this plugin. This one visual run does not establish general hook safety.

The prototype has no click handling. No clickability, response of the eight existing buttons, behavior at other resolutions/scales, or full hook safety is established. The screenshot/log do not record the DLL SHA-256 or exact source commit, so the tested binary cannot be independently identified. The one-run approval has been used; any additional game run requires separate approval.

The source confirms that the extra native `Draw` receives the same intercepted `self` panel object and arguments, with only the shared X/Y rectangle pair changed, and that the prototype provides no distinct Almanac icon/content. This is relevant to the duplicate appearance, but does **not** prove that `C3D_PANEL2D::Draw` owns or binds the crime artwork. The exact texture/icon path behind the observed image is unresolved and needs a Ghidra-capable static trace before proposing a code fix.
