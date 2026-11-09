# Almanac: C3DDLL64.dll — Collision and Draw static analysis (complete)

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

## 1. DLL identification (verified)

| Field | Value |
|-------|-------|
| Path | `C:\Workers Resources Soviet Republic\C3DDLL64.dll` |
| Size | 1,984,000 bytes |
| SHA-256 | `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516` |
| MD5 | `a056238300e9854af04f521fa562119d` |
| Image base | `0x180000000` (PE32+ / 64-bit) |
| Export count | 2,218 functions |

**VA = ImageBase + RVA.** Export RVAs resolve to:
- `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z`: RVA `0xBC690` → VA `0x1800BC690`
- `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z`: RVA `0xBAF40` → VA `0x1800BAF40`

These correspond exactly to IAT slots:
- Slot `0x14086c850` → `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` (hit-test)
- Slot `0x14086c860` → `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` (render setup)

## 2. Raw evidence preserved

| File | Description |
|------|-------------|
| `c3ddll64-collision-draw-20261109/collision_draw_disasm.asm` | Ghidra headless disassembly of Collision range `0x1800BC690:0x1800BD000` and Draw range `0x1800BAF40:0x1800BB200` |

## 3. Collision function — hit-test bounds (observed from disassembly)

Entry: VA `0x1800BC690`

**Bound computation** (lines 2–26):
- `MOVSS XMM5,[RCX+0x673c]` — load panel scale
- `COMISS XMM5,[0x180188be8]; JBE 0x1800bc70a` — if scale <= global, skip normalization
- If scale > global: load bounds `[RCX+0x6740]`/`[RCX+0x6744]`, origin `[RCX+0x6734]`, depth `[RCX+0x674c]`; divide by scale; add origin → normalized bounds in XMM1/XMM4

**Hit-test against input point** (lines 27–31):
- `MOVSS XMM0,[RDX+0x4]` — load input Y coordinate
- `COMISS XMM1,XMM0; JA 0x1800bcb8b` — if bound1 > input Y → **return false** (AL=0 at 0x1800bcb8b)
- `COMISS XMM0,XMM4; JA 0x1800bcb8b` — if input Y > bound2 → **return false** (AL=0 at 0x1800bcb8b)

**Branch target at 0x1800bcb8b** (line 234): `XOR AL,AL` → returns `false` (0). This is the out-of-bounds exit.

**In-bounds path** (lines 32–233):
- Computes accumulated distance/magnitude value in XMM6 via multiple `sqrtf`-like calls (`0x1801687f4`) and `rsqrtf`-like calls (`0x1801687b8`)
- `COMISS XMM6,[0x180188c60]; SETA AL` (line 224–232) — AL = (XMM6 > global_threshold)
- `0x1800bcb8d:` — return epilogue with AL as result

**Conclusion on hit-test bounds:** The Collision function computes rectangular hit-test bounds from panel fields `this+0x6734` (origin), `this+0x673c` (scale), `this+0x6740`/`this+0x6744` (bounds), normalizes by scale, and rejects points outside Y bounds (and presumably X bounds via similar logic before the branch target). Points within bounds proceed to a distance/threshold check against global `[0x180188c60]`. Returns `bool`.

## 4. Draw function — visual clipping (observed from disassembly)

Entry: VA `0x1800BAF40`

**Render setup** (lines 437–546):
- Stack cookie from `[0x1801cdb98]` XOR RSP
- `CMP byte ptr [0x1801dc6e3],0x0; JNZ 0x1800bafd0` — if render flag != 0, skip virtual call
- If flag == 0: `MOV RCX,[RCX+0x280]; OR EDX,0xffffffff; MOV RAX,[RCX]; CALL [RAX+0x10]` — virtual call (likely BeginDraw or similar rendering setup)

**Coordinate transformation** (lines 470–481):
- Loads `[RDI+0x288]` → XMM8, `[RDI+0x28c]` → XMM11
- Multiplies both by global `[0x180188b8c]` (scale factor)
- `XORPS XMM10,xmmword ptr [0x180189070]` — sign flip (coordinate system transformation)
- Stores to `[RDI+0x2c4]` and `[RDI+0x2c8]`
- Zeroes `[RDI+0x2cc]`

**Helper calls** (lines 482–531): Calls `0x1800b8b00` four times with different panel field offsets (`0x2d0`, `0x2dc`, `0x2e8`), storing panel geometry to `this+0x200` through `this+0x2f0`

**Conditional exit** (lines 541–546):
- `MOV ECX,[RDI+0x6728]; TEST ECX,ECX; JZ 0x1800bb272` — if field `this+0x6728` == 0, jump to end
- `MOV EAX,[RDI+0x672c]; TEST EAX,EAX; JZ 0x1800bb272` — if field `this+0x672c` == 0, jump to end

**Conclusion on visual clipping:** The Draw function performs rendering setup (global flag check, virtual call, coordinate scaling, sign-flip, helper calls). It does **NOT** perform per-pixel clipping. Clipping is delegated to `DrawFrom9Patch` (RVA `0xB99C0`) and `DrawFrom3Patch` (RVA `0xB9680`), whose bodies were not traced.

## 5. Distinctions

| Concept | Function | Observed behavior |
|---------|----------|-------------------|
| Hit-test bounds | `Collision` | COMISS + JA comparisons against normalized panel bounds; out-of-bounds → `XOR AL,AL` at `0x1800bcb8b` |
| Visual clipping | `DrawFrom9Patch` / `DrawFrom3Patch` | Export symbols exist; bodies NOT traced |
| Draw setup | `Draw` | Global flag check, virtual call, scale by `[0x180188b8c]`, sign-flip via `[0x180189070]`, helper calls — no clipping |

## 6. Global values referenced (addresses only, values not read)

| Address | Used in | Inferred role |
|---------|---------|---------------|
| `[0x180188be8]` | Collision | Scale comparison threshold |
| `[0x180188b8c]` | Collision + Draw | Global scale factor |
| `[0x180189070]` | Collision + Draw | Sign-flip mask (coordinate inversion) |
| `[0x180188c60]` | Collision | Hit-test distance threshold |
| `[0x1801cdb98]` | Draw | Stack cookie |
| `[0x1801dc6e3]` | Draw | Render enable flag |

## 7. Unexamined calls (do not assume behavior)

| Address | Called in | Call count | Status |
|---------|-----------|------------|--------|
| `0x1800b8c80` | Collision | 4 | Unexamined helper |
| `0x1801687f4` | Collision | 4 | Likely sqrtf, unexamined |
| `0x1801687b8` | Collision | 4 | Likely rsqrtf, unexamined |
| `0x1800b8f20` | Draw | 4 | Unexamined helper |
| `0x1800b8b00` | Draw | 4 | Unexamined helper |
| `DrawFrom9Patch` (RVA 0xB99C0) | — | 0 | Not traced |
| `DrawFrom3Patch` (RVA 0xB9680) | — | 0 | Not traced |

## 8. Raw evidence

Raw disassembly preserved under `export/almanac-evidence/c3ddll64-collision-draw-20261109/collision_draw_disasm.asm` (548 lines, both functions).

No commit or push performed for this step.
