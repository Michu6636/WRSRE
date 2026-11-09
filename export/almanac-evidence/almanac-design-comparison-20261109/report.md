# Almanac button placement — static design comparison

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out/` + live snapshot evidence in `C:\WRSRE\export\almanac-evidence/`
**Method:** Static only. No game execution, no debugger, no hooks, no writes. This is a design comparison, not an implementation.

## 1. Corrected premises

| Earlier claim | Correction |
|---|---|
| Stride 96 bytes | Live snapshot: stride 0x50 (80 bytes); count = (end-begin)/0x50 = 8 |
| New entry appended at end | Non-linear reordering: new entry appears at **screen position 3** (after indices 0,1); logical item at old screen pos 3 (vector index 7) is displaced to last position |
| Capacity exactly 8 | Live snapshot showed capacity pointer beyond end; exact capacity unknown |
| `trade` string hash not authenticatable | Resolved from PE section .rdata at RVA 0x1408B21E4, file offset 0x8B11E4, raw bytes `74 72 61 64 65 00 00 00`, EXE SHA-256 `296644A9...CE6841B8` |

## 2. Native row mechanics (verified instructions)

**Trade drawer `FUN_140373EA0`:**
- Iterates sub-entry vector at `*(game+0xC650)+0x48`..`+0x50`, count = (end-begin)/0x50 (live-verified)
- Non-linear index mapping: screen pos 0→vector idx 0, 1→1, 2→count-1, 3→2, …, count-1→count-2
- Per-entry: texture draw via virtual call `[sub_entry+0x48]->[RAX+0x70]`
- Hit-test: `Collision(mouse, rect at [RBP+0x210])` via IAT `[0x14086c850]`
- Click: `game+0xC65C = EBX` (new item index), `game+0x10BE8 = -1`, dirty flag `game+0x1039A = 0` if changed
- Content sub-view: `FUN_14035F3E0` called only when `game+0xC65C <= 1`

**Section switch `FUN_14007C400`:**
- Writes `game+0xC650` (panel pointer) at `0x1400843f3`
- Writes `game+0xC7A0/A8/B0/B4`, `game+0xC7B0/B4`
- Does NOT write `game+0xC65C`

**Master dispatch `FUN_140317DB0`:**
- Calls `FUN_14007C400` at `0x14031BB22` (section switch)
- Routes "trade" to `FUN_140373EA0` at `0x14031EDEF` when `game+0x10398 != 0`

**Position globals (written per entry):**
- `[0x1409be2e8/ec]`: DPI-scaled button dimension
- `[0x1409be2f0/f8]`: position origin / clip origin
- `[0x1409be30c]`: selected overlay color

## 3. Approach A: Append native entry to Economy & Trade row

### 3.1 Selection state owner
- Native drawer owns selection: writes `game+0xC65C` on click (verified at `0x140374356`)
- Almanac would share `game+0xC65C` with native entries — no separate state slot identified
- If Almanac item selected, `game+0xC65C` holds Almanac's index; native drawer's selected-overlay logic (`0x140374366`) highlights it

### 3.2 Native content suppression / replacement
- **Not suppressed.** The 8 native icons still draw; Almanac icon is an additional entry.
- Content replacement: when `game+0xC65C` points to Almanac's index (>1), `FUN_14035F3E0` is **skipped** (called only when `<= 1`, verified at `0x1403745f1`). The drawer falls through to end-of-function drawing.
- Almanac must supply its own content drawing elsewhere — no native "replace page" mechanism identified for row entries >1.
- **Unsafe assumption:** Calling `FUN_14035F3E0` manually for Almanac content — not verified safe; its side effects (writes `game+0x10F20=4`, calls `FUN_1403613b0`, plays sound) are trade-specific.

### 3.3 Switching away / closing Almanac
- Selecting another sidebar section: `FUN_14007C400` writes new panel pointer to `game+0xC650` (verified at `0x1400843f3`)
- Row selection state (`game+0xC65C`) persists across section switches — not cleared by `FUN_14007C400`
- No explicit "close Almanac" instruction identified in any traced path

### 3.4 Placement / input coordination with eight native icons
- Non-linear reordering: with count=9, Almanac (if appended as last vector entry) appears at **screen position 3**, after native indices 0 and 1. Native index 7 (old screen pos 7) displaces to screen pos 8 (last).
- Input: native `Collision` hit-test handles Almanac click automatically (same draw loop)
- Vector capacity: live snapshot showed capacity beyond end, but exact max unknown — must verify at runtime

### 3.5 Risks
- Reordering breaks "alongside the eight visible icons" assumption
- Vector may be fixed at exactly 8 entries
- Selection state shared with native entries — no isolation
- Content suppression for Almanac page unproven

## 4. Approach B: Separate dynamically positioned button (additive)

### 4.1 Selection state owner
- **Almanac owns its own selection state** — separate from `game+0xC65C` (native row index)
- **Unsafe:** Manually writing `game+0xC65C` to a synthetic native index is **not recommended without further proof** — it would select a native entry the user didn't click, and native drawer would highlight it
- Almanac should track its own open/closed state in mod-owned memory (DLL global or stack), not in game object fields

### 4.2 Native content suppression / replacement
- **Not suppressed.** Eight native icons still draw and respond to clicks normally.
- Almanac button drawn independently, positioned alongside the row using same DPI math.
- Content: Almanac renders its own panel; native trade page continues to draw behind/around it unless explicitly clipped (clip mechanism not traced)

### 4.3 Switching away / closing Almanac
- Almanac button toggles its own open/closed state (mod-owned flag)
- Switching sidebar sections: `FUN_14007C400` changes `game+0xC650`; Almanac panel would need to close itself (no native "close Almanac" instruction found)
- **Unsafe:** Manually writing `game+0xC650` (panel pointer) to force native content — not verified safe; panel pointer write targets arbitrary memory read as panel object

### 4.4 Placement / input coordination with eight native icons
- Position: compute button rect using same DPI-scaled math as trade drawer:
  - Base Y from `[0x140992088]` (DPI) × `[0x14090aa0c]` + DPI × `[0x14090aaa0]`
  - Button size from DPI × `[0x14090aaa0]`
  - Vertical step per entry: `XMM6 += XMM7`
  - Place Almanac button at Y after last native icon, or at a dedicated X offset
- Hit-test: `Collision(mouse, rect)` via IAT `[0x14086c850]` — must ensure Almanac rect doesn't overlap native button rects
- Input coordination: Almanac click handled by mod's own `Collision` call; native clicks handled by `FUN_140373EA0`. Both read mouse from `C3D_INPUT` (input source not fully traced)

### 4.5 Risks
- Button rect may visually overlap native icons if position math slightly off
- Hit-test rect vs visual clip bounds are separate unknowns (native drawer may clip differently than it hit-tests)
- DPI constants `[0x14090aa0c]`, `[0x14090aaa0]` purposes unverified
- No native content suppression — Almanac panel overlays native content, may look broken

## 5. Recommended design (static, not implemented)

**Recommended: Approach B (separate additive button)** with the following constraints:

1. **Selection state:** Mod-owned DLL global (not `game+0xC65C`, not `game+0xC650`). Do not write synthetic native indices.
2. **Positioning:** Reuse trade drawer's DPI math (`[0x140992088]` DPI, constants `[0x14090aa0c]`/`[0x14090aaa0]`) to place Almanac button adjacent to the 8 native icons without touching the trade vector.
3. **Input:** Mod's own `Collision` call against Almanac rect; native drawer handles native clicks independently.
4. **Open/close:** Toggle mod-owned flag on Almanac button click. Close when another sidebar section is selected — but **do not** manually write `game+0xC650` (panel pointer) to force this; instead detect section change via native mechanism (not yet traced) or accept that Almanac stays open until re-clicked.
5. **Content:** Almanac renders its own panel content; does not call `FUN_14035F3E0` (trade-specific, unproven safe).
6. **Text ID:** Custom GetString ID ≥ 1,000,000 (per modding rules) for Almanac label.

## 6. Unresolved risks

| Risk | Status |
|------|--------|
| Button rect vs native rect overlap (visual) | Unknown — position math constants unverified at runtime |
| Hit-test rect vs clip bounds mismatch | Unknown — clipping not traced |
| DPI constant meanings | `[0x14090aa0c]`, `[0x14090aaa0]` unverified |
| Section-change detection | Not traced — how to know user switched away |
| Panel pointer write safety | `game+0xC650` write targets arbitrary panel object — unsafe |
| `FUN_14035F3E0` reuse | Trade-specific side effects; not verified safe for Almanac |
| Vector capacity (Approach A only) | Unknown max entries |

## 7. Context-specific tests needed (read-only snapshot)

1. Read DPI scalar `[0x140992088]` and constants `[0x14090aa0c]`, `[0x14090aaa0]` to verify position math
2. Snapshot `[0x1409be2e8/ec]` and `[0x1409be2f0/f8]` globals during row draw to capture native button rects
3. Verify trade vector capacity (begin/end/capacity pointers) to confirm 9th-entry feasibility for Approach A
4. Confirm click writes `game+0xC65C` on native icon (already observed: yes)
5. Confirm section switch changes `game+0xC650` (already observed: yes)
6. Test whether Almanac button rect drawn by mod overlaps native button rects

## 8. Supporting instruction references

| Purpose | VA | File |
|---------|-----|------|
| Trade drawer entry | `0x140373EA0` | `140373ea0_1403829fd_disasm.asm` |
| Button size computation | `0x140374149` | `140373ea0_1403829fd_disasm.asm` |
| Count computation (stride) | `0x14037417c` | `140373ea0_1403829fd_disasm.asm` |
| Index reordering | `0x1403741a2`–`0x1403741cf` | `140373ea0_1403829fd_disasm.asm` |
| Entry address + texture draw | `0x1403741f6`–`0x140374214` | `140373ea0_1403829fd_disasm.asm` |
| Hit-test Collision call | `0x1403742a4` | `140373ea0_1403829fd_disasm.asm` |
| Click state write | `0x140374356` | `140373ea0_1403829fd_disasm.asm` |
| Selected overlay | `0x140374366`–`0x140374375` | `140373ea0_1403829fd_disasm.asm` |
| Vertical advance | `0x1403743a8` | `140373ea0_1403829fd_disasm.asm` |
| Content sub-view call | `0x140374606` | `140373ea0_1403829fd_disasm.asm` |
| Section switch panel write | `0x1400843f3` | `14007c400_140084754_disasm.asm` |
| Master dispatch section switch | `0x14031BB22` | `140317db0_full_disasm.asm` |
| Trade drawer dispatch | `0x14031EDEF` | `140317db0_full_disasm.asm` |

**Bottom line:** Approach B (separate additive button) is recommended over Approach A (native entry append) because it avoids vector reordering and capacity risks, keeps selection state isolated, and doesn't require unsafe synthetic index writes. Both require runtime verification of position math and hit-test bounds that static analysis cannot provide.
