# Almanac: FUN_14035f3e0 analysis (content sub-view before suppression boundary)

Static analysis only. No debugger, hooks, implementation, or game changes.
Sources:
- `scratch/out/14035f3e0_decomp.c` (decompilation, preserved at
  `export/almanac-evidence/fun-14035f3e0-20261108/14035f3e0_decomp.c`)
- `docs/ghidra-index/35F3E0.md` (index entry, preserved at
  `export/almanac-evidence/fun-14035f3e0-20261108/35F3E0.md`)
- `export/140373ea0_1403829fd_disasm.asm` (caller context at 0x140374606)
- `export/140317db0_full_disasm.asm` (host output consumption at 0x14031edf4–0x14031ee02)

Call site: handler `FUN_140373EA0` calls `FUN_14035f3e0` at `0x140374606` when
`game+0xC65C <= 1` (selection is 0 or 1), with:
- param_2 = `RBP-0x4c` (output-X pointer, saved at handler entry `0x140374050`)
- param_3 = `RSP+0x70` (output-Y pointer)

No commit or push performed (per task constraint).

## 1. Does it draw or handle input when selection is 0 or 1?

**Yes — it both draws and handles input.**

The function is conditionally invoked from the handler at `0x140374606` only when
`game+0xC65C <= 1` (handler lines `0x1403745f1` CMP, `0x1403745f8` JA). Once
entered, the decompiled body shows:

### Drawing (unconditional, always executes when called)
| Decomp line | Action |
|---|---|
| 74 | `C3D_PANEL2D::Draw(&DAT_1409be060, ...)` — backdrop panel |
| 87–109 | Loop over 6 sub-items: `GetString` + `PrintLeftUnicode` label drawing |
| 147 | `C3D_PANEL2D::DrawFrom9Patch(&DAT_1409be060, ...)` — 9-patch button per sub-item |
| 249, 286, 293, 329, 366, 410, 460, 503, 536, 572, 616, 652, 693, 729 | Multiple additional `C3D_PANEL2D::Draw` / text draws after the loop |

There is **no early return that skips drawing**. The only `return` is at
decomp line 733 (`return cVar4;`), after all drawing has completed.

### Input handling (gated on cVar4)
| Decomp line | Action |
|---|---|
| 128–135 | `C3D_INPUT::GetMouseSolid` reads cursor X/Y |
| 226 | `C3D_PANEL2D::Collision(&DAT_1409be060, mouse)` — hit test |
| 230–242 | On hit: latch `DAT_140a54e91`, play `gui_click` sound, call `FUN_1403613b0(param_1,0xffffffff,1,local_148)`, set `game+0x10F20 = 4` |

Input handling runs **only when `cVar4 == '\0'`** (decomp line 224 `if (cVar4 == '\0')`, else jumps to `LAB_14035fcfc` at line 244, skipping the click block).

`cVar4` is set at decomp lines 177–181:
```
cVar4 = '\0';
if ((game+0x5e6 != '\0') && (FUN_140335bc0() != '\0')) {
    cVar4 = '\x01';
}
```
So cVar4 == 1 (input handling skipped) when BOTH `game+0x5E6 != 0` AND `FUN_140335bc0()` returns nonzero. Input handling runs when either is false/zero.

## 2. Does it change either output local passed by the caller?

**param_3 (output-Y pointer, RSP+0x70): YES — it is modified.**
**param_2 (output-X pointer, RBP-0x4c): NO — it is only read, never written.**

Evidence from decompilation:
- Line 53: `fVar24 = *param_2;` — reads output-X into local fVar24. **No write to *param_2 anywhere in the body.**
- Line 54: `fVar27 = *param_3;` — reads output-Y into local fVar27.
- Line 63: `local_110 = param_3;` — aliases the output-Y pointer.
- Line 732: `*local_110 = DAT_140992088 * DAT_14090aa28 + fVar27;` — **writes `*param_3 = scale * const_aa28 + original_Y`**.
- Line 733: `return cVar4;`

The caller at `0x14037460b` reloads `XMM10 = [RSP+0x70]` immediately after the call, confirming it expects the Y output may have changed.

The function also writes to game fields (`game+0x5E8` at lines 192/232, `game+0xC818` at line 233, `game+0x5EC` at line 483, `game+0x5F0` at line 493, `game+0x5E6` at line 492, `game+0xADA8` at line 448, `game+0xC808` at line 451) but these are game state, not the output locals.

## 3. Which conditions allow it to return without drawing?

**None — from inside FUN_14035f3e0, there is no condition that causes it to return without drawing.**

The function always executes at least the backdrop draw (decomp line 74) and subsequent drawing. The single `return` at line 733 occurs only after all drawing is complete.

However, from the **caller's** perspective, the function is not called at all when `game+0xC65C > 1` (handler `0x1403745f8` JA to `0x14037461b`, which sets `[RSP+0x40]=1` and skips the call). So at the call site, "no drawing by this function" occurs when `game+0xC65C > 1` — but that is caller gating, not an early return inside FUN_14035f3e0.

Within the function, input handling (but not drawing) is skipped when `cVar4 != '\0'`, i.e., when `game+0x5E6 != 0` AND `FUN_140335bc0()` returns nonzero (decomp lines 178–181, 224).

## 4. Fact vs assumption summary

**Facts (from decompilation):**
- Signature: `char FUN_14035f3e0(game, float *param_2, float *param_3)` (decomp line 9)
- Called from handler at `0x140374606` when `game+0xC65C <= 1` (handler `0x1403745f1–0x1403745f8`)
- Always draws backdrop + 6 sub-items + panels (decomp lines 74, 87–109, 147, etc.)
- `*param_3` (output-Y) written at decomp line 732: `*param_3 = scale * DAT_14090aa28 + original_Y`
- `*param_2` (output-X) only read (line 53), never written
- Input handling gated on `cVar4 == '\0'` (decomp line 224)
- `cVar4 == 1` when `game+0x5E6 != 0` AND `FUN_140335bc0() != 0` (decomp lines 178–181)
- On click: `game+0x10F20 = 4`, `DAT_140a54e91` latched, `gui_click` sound (decomp lines 230–242)
- Returns `cVar4` (decomp line 733)

**Assumptions (not proven):**
- The value `DAT_14090aa28` added to output-Y is the pitch constant (value unverified in this export).
- `FUN_140335bc0()` return value semantics are not decoded here.

## 5. Relevance to content-suppression boundary

Calling FUN_14035f3e0 at `0x140374606` happens **after** the proposed suppression boundary (`0x1403749f2`, where all cleanup locals are initialized). It modifies `*param_3` (output-Y), so if content is suppressed but this call is retained, output-Y would still be advanced by `scale * DAT_14090aa28`. If this call is also suppressed, output-Y would retain its pre-call value (`[RSP+0x70]` as set earlier), which may differ from what the host expects for `game+0xae58`.

This is a planning observation, not a patch proposal. No hook is authorized.
