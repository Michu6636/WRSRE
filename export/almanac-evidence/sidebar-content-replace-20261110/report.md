# Sidebar content replacement boundary — Almanac design recommendation

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out/` (sections 39a–49, `ui_panels.cpp`, `ui_panels_api.h`)
**Method:** Static only. No game execution, no debugger, no hooks, no writes. Design recommendation, not implementation.

## 1. The problem

The Almanac button (Approach B: separate additive button) avoids rearranging native icons, but the **vanilla page could still draw or receive clicks underneath** the Almanac panel. The existing `widgetButton` precedent (`ui_panels.cpp`) is building-window-specific and cannot be dropped into the global sidebar. The remaining blocker: **where can the Almanac replace native sidebar content without breaking drawing, input, or cleanup?**

## 2. Candidate content-replacement boundaries (traced from disassembly)

### Boundary 1 — Skip `FUN_140373EA0` entirely (master dispatch `0x14031EDEF`)

**Instruction** (verified, `140317db0_full_disasm.asm` lines ~3139–3141):
```
14031ede3: MOV RCX, R14              ; game
14031ede6: CMP byte ptr [R14 + 0x10398], R15B   ; R15B = 0
14031eded: JZ 0x14031ee10            ; game+0x10398 == 0 -> FUN_140361BB0
14031edef: CALL 0x140373ea0          ; game+0x10398 != 0 -> trade row drawer
```

**What this replaces:** The entire trade panel drawing: row buttons (with hit-test + `game+0xC65C` selection write at `0x140374356`) AND content sub-view (`FUN_14035F3E0` at `0x140374606`, called when `game+0xC65C <= 1`).

**Side effects skipped (state writes in `FUN_140373EA0`):**
- `game+0xC65C` ← selected sub-tab index (write at `0x140374356`)
- `game+0x10BE8` ← -1 (dirty counter reset, write at `0x14037435c`)
- `game+0x1039A` ← 0 (dirty flag clear when selection changed, write at `0x14037434f`)
- `game+0xAE58` ← computed panel height (write at `0x14031ee02`)
- `FUN_14035F3E0` not called → its state writes skipped: `game+0x10F20` (sub-page state), `game+0x5E8/0x5EC/0x5E6`, `game+0xADB8/0xADC8`, sub-page navigation via `FUN_1403613b0`

**Cleanup:** No explicit "restore native panel" instruction exists anywhere in the traced path. Skipping the whole handler leaves native state partially updated.

**Verdict:** Not recommended — too much native state is skipped, and vanilla sub-page interaction (`FUN_1403613b0`) is disabled.

### Boundary 2 — Replace only content sub-view `FUN_14035F3E0` (call at `0x140374606`)

**Instruction** (verified, `140373ea0_1403829fd_disasm.asm`):
```
1403745f1: CMP dword ptr [RSI + 0xc65c], 0x1   ; if selected sub-tab <= 1
1403745f8: JA 0x14037461b                      ; skip if > 1
140374606: CALL 0x14035f3e0                    ; content sub-view
14037460b: MOVSS XMM10,[RSP + 0x70]
140374612: TEST AL,AL                          ; AL = click indicator return
140374614: MOV byte ptr [RSP + 0x40],0x0
140374619: JNZ 0x140374620                     ; if AL != 0, skip flag set
14037461b: MOV byte ptr [RSP + 0x40],0x1       ; set flag when AL == 0
```

**What this replaces:** Only the content drawn below the row (sub-page panel with 6 clickable sub-items, labels, DrawFrom9Patch buttons). Row button drawing is preserved — native `game+0xC65C` selection management, hit-test, and click latch handling continue normally.

**Side effects preserved:** Row button drawing writes `game+0xC65C`, `game+0x10BE8`, `game+0x1039A` — all intact. Native input routing unchanged.

**Side effects skipped:** `FUN_14035F3E0`'s own writes (`game+0x10F20` sub-page state, `game+0x5E8/0x5EC/0x5E6`, `game+0xADB8/0xADC8`) and its click handling (`FUN_1402fd840`, `FUN_1403613b0` sub-page navigation, `gui_click` sound). These are trade-specific; Almanac would supply its own.

**Cleanup:** Almanac draws its own panel in the content area; no native panel pointer write needed. When Almanac closes, `FUN_14035F3E0` is simply not called (or returns without drawing), and native row buttons remain visible and clickable.

**Verdict:** Cleanest boundary — preserves native drawing/input, only replaces content below the row. But requires identifying which instructions in the body block (after the call) are pure draw vs state write, and the body extent (bottom edge) is unknown → overlap risk with native content.

### Boundary 3 — Section switch `FUN_14007C400` (writes `game+0xC650`)

**Instruction** (verified, `14007c400_140084754_disasm.asm` at `0x1400843f3`):
```
1400843f3: MOV qword ptr [R14 + 0xc650], RDI   ; game+0xC650 = panel pointer
```

**What this does:** Switches the selected sidebar panel (writes panel pointer). Also writes `game+0xC7A0/A8/B0/B4`, `game+0xC7B0/B4`. Called at `0x14031BB22` before content dispatch.

**Does NOT write:** `game+0xC65C` (row item index). Does NOT close any open Almanac content.

**Verdict:** Not recommended for Almanac close/return — manually writing `game+0xC650` targets an arbitrary panel object (unsafe, unproven), and it doesn't close Almanac content.

### Boundary 4 — Dispatcher ESC close `FUN_1406e9b80` (called at `0x14031D545`)

**Close conditions** (from `ui_panels.cpp` comments, grounded in `do_dispatcher.c`):
- `game+0xD428 == 0` (no placement/modal tool)
- `0xA54C91 != 0` (ESC press-edge, set by input decoder `FUN_140443b90` → param+0x69)
- visible window (`window+0x01 != 0`)
- Sets `window+0x00 = 1` (close flag) and zeroes `0xA54C91`

**Verdict:** Native ESC-close path — Almanac would be affected by it (if window is the sidebar) but cannot trigger it cleanly for its own panel. Not a reliable Almanac close mechanism.

## 3. Clip context (`game+0xADB0`) — not set in any traced function

All four panel setup helpers (`FUN_1404465A0`, `FUN_1404460A0`, `FUN_140445EB0`, `FUN_1404461F0`) are sorted-collection or scale helpers — **none set width, height, or clip rectangle** for `game+0xADB0`. Panel context dimensions are set during object construction (before `FUN_140317DB0`) or by a function not in cached exports. Clipping is performed by `C3D_PANEL2D::Draw`/`Collision` using internal dimensions that are not statically established.

**Implication:** An Almanac panel drawn over the sidebar has no established clip rect — overlap with native content and native input underneath cannot be ruled out statically.

## 4. WidgetButtonImplEx comparison (`plugins/ui_panels/ui_panels.cpp`)

| Aspect | `widgetButton` (ui_panels) | Global sidebar Almanac button |
|--------|---------------------------|-------------------------------|
| Scope | Individual building window (kind==0 builders) | Global sidebar (Economy & Trade row) |
| Placement | Appends at `window+0x250` bottom edge | Beside 8 native icons in row |
| Geometry | `window+0x240` is Building* union | `game+0xC650` panel pointer, row vector |
| Draw | C3D_PANEL2D::DrawFrom9Patch + 9-patch chrome | Same idiom (reusable) |
| Hit-test | GetMouseSolid + Collision over tile rect | Same idiom (reusable) |
| Click latch | `0xA54E91` consumed, `gui_click` sound | Same idiom (reusable) |
| Row budget | Consumes 2 rows (DPI*35 each) | N/A (separate button, no row) |
| ESC close | Dispatcher `FUN_1406e9b80` (window+0x00=1) | Same dispatcher, but sidebar not a "window" |
| Clip push/pop | Balanced in builder scope (finished panels) | No builder scope — clip untraced |
| Foreign path | `appendWidgetsToWindow` skips clip → ESC blocked | No foreign path equivalent |

**Key distinction:** `widgetButton` is **not a global sidebar API**. It is tied to building-window builders (`window+0x240` is a Building* union; `window+0x250` is the building panel's bottom edge). The global sidebar has no `window+0x250` bottom edge to advance, no building context, and no clip scope to balance. The button-drawing idiom (9-patch + Collision + latch) is reusable, but the **placement and lifecycle contract is not**.

## 5. Design recommendation (static, not implemented)

**Recommended boundary: Boundary 2 — Replace only the content sub-view (`FUN_14035F3E0` at `0x140374606`).**

Rationale:
- Keeps native row button drawing intact → native `game+0xC65C` selection, hit-test, and click latch all work unchanged
- Native input routing preserved → no input contention underneath
- Only replaces what's drawn below the row (content area) → Almanac panel occupies the content region
- No manual `game+0xC650` panel pointer write (unsafe)
- No `FUN_140373EA0` state writes skipped (dirty flag, counter intact)
- When Almanac closes, simply don't draw Almanac content → native trade page (if `game+0xC65C <= 1`, `FUN_14035F3E0` would have drawn; otherwise nothing) remains visible

**Implementation sketch (not to be implemented):**
1. Almanac button drawn separately (Approach B) at a free slot after native icons, using DPI math from trade drawer (`[0x140992088]` DPI, constants `[0x14090aa0c]`/`[0x14090aaa0]`)
2. Almanac button click sets mod-owned open/closed flag (DLL global, NOT `game+0xC65C`/`game+0xC650`)
3. When open and `game+0xC65C <= 1` (trade sub-tab 0 or 1), skip `FUN_14035F3E0` and draw Almanac content in its place
4. When closed, let `FUN_14035F3E0` run normally (native trade content)
5. Custom text ID ≥ 1,000,000 for Almanac label

**Why not Boundary 1:** Skipping `FUN_140373EA0` entirely breaks native row drawing, `game+0xC65C` selection, and `FUN_14035F3E0` sub-page navigation — too much native state is lost.

**Why not Boundary 3/4:** Manually writing `game+0xC650` is unsafe (arbitrary panel object); dispatcher ESC close is window-scoped and not triggerable for Almanac.

## 6. Unresolved risks

| Risk | Why it matters |
|------|----------------|
| Body extent unknown | The bottom edge of `FUN_140373EA0`'s body block (after `0x1403745b6`) is not traced → Almanac content may overlap native content below the row |
| Clip rect untraced | `game+0xADB0` dimensions not set in any examined function → no established clip for Almanac panel |
| Native input underneath | Almanac panel drawn over native content may receive clicks intended for native row buttons (hit-test rects not compared) |
| `FUN_14035F3E0` state writes skipped | `game+0x10F20` (sub-page state), `game+0x5E8/0x5EC/0x5E6`, `game+0xADB8/0xADC8` not updated when Almanac open → consistency unverified |
| Sub-page navigation (`FUN_1403613b0`) skipped | Vanity trade sub-page navigation disabled when Almanac open — acceptable if Almanac replaces trade view entirely |
| Handler selector (`game+0x10398`) | Boundary 2 only applies when `FUN_140373EA0` runs; when `game+0x10398 == 0`, `FUN_140361BB0` runs (different content path, not traced for content replacement) |

## 7. Context-specific tests needed (read-only snapshot)

1. Read `game+0xC65C` during trade render to confirm which sub-tab is selected (0/1 → `FUN_14035F3E0` called; >1 → not called)
2. Read DPI scalar `[0x140992088]` and constants `[0x14090aa0c]`/`[0x14090aaa0]` to verify Almanac button position math
3. Snapshot `[0x1409be2e8/ec]` (backdrop size) and `[0x1409be2f0/f8]` (position) during row draw to capture native button rects
4. Read `game+0xADB0` width/height fields (offsets unknown — need to discover constructor/init)
5. Verify whether `FUN_140361BB0` content path (when `game+0x10398 == 0`) has an equivalent content sub-view boundary

## 8. Supporting instruction references

| Purpose | VA | File |
|---------|-----|------|
| Trade drawer entry | `0x140373EA0` | `140373ea0_1403829fd_disasm.asm` |
| Content sub-view call | `0x140374606` | `140373ea0_1403829fd_disasm.asm` |
| Content sub-view guard | `0x1403745f1`–`0x1403745f8` | `140373ea0_1403829fd_disasm.asm` |
| Row selection write | `0x140374356` | `140373ea0_1403829fd_disasm.asm` |
| Section switch panel write | `0x1400843f3` | `14007c400_140084754_disasm.asm` |
| Master dispatch trade call | `0x14031EDEF` | `140317db0_full_disasm.asm` |
| Master dispatch section switch | `0x14031BB22` | `140317db0_full_disasm.asm` |
| Dispatcher ESC close | `0x1406e9b80` (called at `0x14031D545`) | `140317db0_full_disasm.asm` |
| Clip helpers (no width/clip set) | `0x4460A0`/`0x445EB0`/`0x4461F0`/`0x4465A0` | `1404460a0_decomp.c`/`140445eb0_decomp.c`/`1404461f0_decomp.c`/`1404465a0_full_disasm.asm` |
| `FUN_14035F3E0` content sub-view | `0x14035F3E0` | `14035f3e0_decomp.c` |
| Button backdrop/position globals | `[0x1409be2e8/ec]`, `[0x1409be2f0/f8]` | `140373ea0_1403829fd_disasm.asm` |

**Bottom line:** Boundary 2 (replace only `FUN_14035F3E0` content sub-view) is the recommended content-replacement boundary — it preserves native drawing/input/selection while swapping the sub-page content. The Almanac button itself should be Approach B (separate additive button). Critical unknowns: body extent (overlap), clip rect (clipping), and handler selector (which content path runs). All static — nothing executed, nothing modified.
