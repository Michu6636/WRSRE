# Almanac: native row hit-test rectangle trace — [RBP-0x80] source

Static analysis only. No hooks, debugger, game changes, commit, or push.
Source: `export/140373ea0_1403829fd_disasm.asm` (handler `FUN_140373EA0`).

## 1. Trace of [RBP-0x80] back to its source

[RBP-0x80] is written exactly once, before the row loop, by an indirect call:

| Address | Instruction | Meaning |
|---------|-------------|---------|
| `0x140373f75` | `LEA RDX,[RBP + -0x80]` | RDX = output pointer &[RBP-0x80] |
| `0x140373f79` | `LEA RCX,[0x140a54b90]` | RCX = `C3D_INPUT` object at `0x140a54b90` |
| `0x140373f80` | `CALL qword ptr [0x14086c868]` | indirect call, writes mouse coords to [RBP-0x80] |

`[0x140a54b90]` is confirmed as the `C3D_INPUT` object (same global used in `FUN_14035f3e0` via `C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90)`). The call at `0x140373f80` is therefore almost certainly `C3D_INPUT::GetMouseSolid` writing mouse X/Y to [RBP-0x80]/[RBP-0x78]. The function-pointer target at `[0x14086c868]` is **unresolved** from static analysis (indirect call through a global pointer slot).

[RBP-0x80] is **not modified** between `0x140373f80` and its reads inside the loop. The same mouse position is reused for every loop iteration.

## 2. Reads of [RBP-0x80] and [RBP-0x78] inside the hit-test

| Address | Instruction | Meaning |
|---------|-------------|---------|
| `0x140374278` | `MOVSD XMM0,qword ptr [RBP + -0x80]` | load 8 bytes (double) from [RBP-0x80] |
| `0x14037427d` | `MOVSD qword ptr [RBP + 0x210],XMM0` | rect field at [RBP+0x210] = [RBP-0x80] |
| `0x140374285` | `MOV EAX,dword ptr [RBP + -0x78]` | load 4 bytes from [RBP-0x78] |
| `0x140374288` | `MOV dword ptr [RBP + 0x218],EAX` | rect field at [RBP+0x218] = [RBP-0x78] |

[RBP-0x80] and [RBP-0x78] are 8 bytes apart (adjacent, non-overlapping):
- [RBP-0x80] covers bytes RBP-0x80..RBP-0x79 (read as double/MOVSD)
- [RBP-0x78] covers bytes RBP-0x78..RBP-0x75 (read as dword/MOV)

Based on surrounding constant computation:
- `XMM15 = scale * (45 + 85) = 0.8 * 130 = 104.0` (X origin, computed at `0x140373f3c`–`0x140374057`)
- `XMM10 = scale * 15 + game+0xae50 = 0.8 * 15 + 184.0 = 196.0` (Y base, computed at `0x140373f63`–`0x140373f6c`)

These are computed before the `0x140373f80` call but it is **not established** that [RBP-0x80] receives exactly these values — the indirect call's behavior is unresolved.

## 3. Hit-test rectangle construction at [RBP+0x210]

| Address | Instruction | Field |
|---------|-------------|-------|
| `0x14037427d` | `MOVSD [RBP+0x210],XMM0` | [RBP+0x210] = [RBP-0x80] (8 bytes) |
| `0x140374288` | `MOV [RBP+0x218],EAX` | [RBP+0x218] = [RBP-0x78] (4 bytes) |
| `0x14037428e` | `MOVAPS XMM3,XMM8` | XMM3 = XMM8 = 0 |
| `0x140374292` | `MOVAPS XMM2,XMM8` | XMM2 = XMM8 = 0 |
| `0x140374296` | `LEA RDX,[RBP + 0x210]` | RDX = rect pointer |
| `0x14037429d` | `LEA RCX,[0x1409be060]` | RCX = panel object |
| `0x1403742a4` | `CALL qword ptr [0x14086c850]` | `C3D_PANEL2D::Collision(panel, rect)` |

XMM8 is zeroed at `0x14037418d` (`XORPS XMM8,XMM8`), before the loop entry at `0x1403741a2`, and **no instruction modifies XMM8 between `0x14037418d` and `0x14037428e`**. So XMM8 = 0 at the hit-test.

The rectangle at [RBP+0x210] is built from:
- [RBP+0x210] = [RBP-0x80] (mouse X, read as double)
- [RBP+0x218] = [RBP-0x78] (mouse Y, read as dword)
- Remaining fields = 0 (from XMM8)

This is a **point-in-rect test**: the mouse position is tested against the panel at `[0x1409be060]`. The same mouse position is used for every loop iteration.

## 4. Does the hit-test rectangle limit a hypothetical ninth same-row button?

**No — not directly.** The hit-test rectangle encodes the **mouse position**, not button coordinates. It is identical for every loop iteration. A ninth button at a different X offset would not be excluded by this rectangle because the rectangle doesn't encode button positions at all.

What does limit a ninth button:
1. **Loop count** — the loop iterates over subvector entries at `[RSI+0xc650]` (count = `(end-begin)/stride`). Only `count` buttons are iterated. If count is 8, no ninth button is reached regardless of hit-test bounds. (Do not infer the live count from the eight trade buttons observed in the geometry snapshot — that is a separate vector/observation.)
2. **Per-button panel positioning** — `FUN_140445eb0` is called per-iteration at `0x140374265` with `XMM1 = XMM6` (advancing by pitch each iteration via `0x1403743a8: ADDSS XMM6,XMM7`). This likely repositions the panel per button. Whether it positions a ninth button is unexamined (function body not in exports).
3. **Panel bounds / clipping** — the panel's visual bounds are set up elsewhere; clipping happens in the draw path (`C3D_PANEL2D::Draw` at `[0x14086c860]`), not the hit-test path.

## 5. Hit-test bounds vs visual clipping

- **Hit-test bounds** (`0x1403742a4`): `C3D_PANEL2D::Collision(panel, rect)` — tests mouse point against panel. Rectangle built from mouse position ([RBP-0x80]/[RBP-0x78]) with zeroed remaining fields.
- **Visual clipping**: occurs in `C3D_PANEL2D::Draw` / `DrawFrom9Patch` through indirect call slot `[0x14086c860]` (and `[0x14086c858]`). DLL bodies unavailable; clipping behavior not proven from static analysis.

Neither hit-test bounds nor visual clipping is established from a screenshot or text match. Both are inferred solely from instruction semantics.

## 6. Unresolved items

| Item | Status |
|------|--------|
| Target of `[0x14086c868]` at `0x140373f80` | Unresolved (indirect call through global pointer) — almost certainly `C3D_INPUT::GetMouseSolid` but not proven |
| Target of `[0x14086c850]` at `0x1403742a4` | Unresolved (indirect call) — almost certainly `C3D_PANEL2D::Collision` but not proven |
| Exact values written to [RBP-0x80]/[RBP-0x78] | Not established — mouse position inferred from context, not verified |
| `FUN_140445eb0` body (called per-iteration at `0x140374265`) | Not in existing exports; not decompiled |
| Panel bounds / clipping behavior | DLL bodies unavailable; not proven |
| Loop count for the current session | Not observed; geometry snapshot's 8 entries are a separate vector |

## 7. Raw evidence

No new Ghidra queries were run. All analysis is from the existing export:
- `export/140373ea0_1403829fd_disasm.asm` — handler disassembly, lines 30–284 (key region `0x140373f75`–`0x1403742a4`)

No commit or push performed for this step.
