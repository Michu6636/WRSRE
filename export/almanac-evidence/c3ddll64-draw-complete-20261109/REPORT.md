# Almanac: C3DDLL64.dll — Complete Draw static analysis + export-table evidence

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

VA = ImageBase + RVA. `.text` section: VA `0x1000`, Raw `0x400` (file offset = VA − 0xC00).

## 2. Export-table evidence — Collision and Draw

Raw PE export directory parsed directly from SOVIET64.exe on disk.

| Mangled name | Demangled | Ordinal | RVA | VA | File offset |
|--------------|-----------|---------|-----|-----|-------------|
| `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` | `C3D_PANEL2D::Collision(C3DVECTOR3, float, float) -> bool` | 668 | 0x000BC690 | 0x1800BC690 | 0xBBA90 |
| `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` | `C3D_PANEL2D::Draw(float, float, float, float, float, bool) -> void` | 879 | 0x000BAF40 | 0x1800BAF40 | 0xBA340 |

Related panel exports (context only, not traced):
- `?BeginDraw@C3D_PANEL2D@@QEAAXPEBD_N@Z` — VA `0x1800BC4A0`, ordinal 443
- `?EndDraw@C3D_PANEL2D@@QEAAXXZ` — VA `0x1800BC610`, ordinal 960

## 3. Ghidra function boundaries

Ghidra's Function Start Search (headless analysis run) identified:

| Function | Start VA | End VA | Boundary evidence |
|----------|----------|--------|-------------------|
| `Collision` | `0x1800BC690` | `0x1800BCBAB` | RET at `0x1800BCBAB`; next function starts `0x1800BCBB0` |
| `Draw` | `0x1800BAF40` | `0x1800BB720` | RET at `0x1800BB720`; next function starts `0x1800BB72C` (PUSH RDI) |

Draw spans `0x1800BAF40`–`0x1800BB720` (960 bytes / 0x3E0).

## 4. Draw function — complete exits

All four reachable exits, traced from raw disassembly:

### Exit 1: `0x1800BB272` — early-out join point

Reached via conditional jumps from:
- `0x1800BB1EB`: `JZ 0x1800BB272` — if `[RDI+0x6728] == 0`
- `0x1800BB1F9`: `JZ 0x1800BB272` — if `[RDI+0x672C] == 0`
- `0x1800BB209`: `JP 0x1800BB272` — if `[RDI+0x2A8]` is NaN
- `0x1800BB20B`: `JNZ 0x1800BB272` — if `[RDI+0x2A8] != 0.0`

From `0x1800BB272`, execution proceeds through stack-cookie setup (`MOVSS XMM7,[0x180188BE8]`, `MOV EBX,4`, stores 1.0 to panel fields `0x204`/`0x214`/`0x224`/`0x234`), then enters a 4-iteration loop at `0x1800BB2B0` calling vtable methods `[RAX+0x48]` and `[RAX+0x50]`, then either:
- Falls through to panel setup at `0x1800BB326` if `[RDI+0x30D] != 0`
- Or jumps to `0x1800BB53A` → `JMP 0x1800BB6C0` if `[RDI+0x30D] == 0`

### Exit 2: `0x1800BB6C0` — stack cookie check (bounds-check and loop-exit path)

Reached via:
- `0x1800BB223`: `JA 0x1800BB6C0` — Y upper bound check failed
- `0x1800BB246`: `JA 0x1800BB6C0` — second Y bound check failed
- `0x1800BB25B`: `JA 0x1800BB6C0` — depth bound check failed
- `0x1800BB26C`: `JA 0x1800BB6C0` — third bound check failed
- `0x1800BB526`: `JNZ 0x1800BB6CF` — loop counter `[RDI+0x308]` reached `0x80` (0x1800BB6CF is 3 bytes into the cookie check at `0x1800BB6C0`)

At `0x1800BB6CD`: `MOV RCX,[RBP+0x80]; XOR RCX,RSP; CALL 0x1801687E8` (`__security_check_cookie`), then epilogue.

### Exit 3: `0x1800BB720` — final `RET`

Draw's last instruction: `C3` at `0x1800BB720`. This is the function's normal return after the stack-cookie check and epilogue (`POP R15/R14/R13/R12/RBP` at `0x1800BB717`–`0x1800BB71F`).

### Exit 4: `0x1800BB72C` — next function boundary (not an exit of Draw)

`0x1800BB72C` begins with `40 57` (`PUSH RDI`), confirming it is a separate function. This confirms Draw ends at `0x1800BB720`.

## 5. Draw function — observed behavior (not clipping)

| Address range | Instruction | Observed effect |
|---------------|-------------|-----------------|
| `0x1800BAF40`–`0x1800BAFBF` | Stack frame setup, stack cookie from `[0x1801CDB98]`, `CMP byte [0x1801DC6E3],0` | Render-enable flag check |
| `0x1800BAFC0`–`0x1800BAFCD` | `MOV RCX,[RCX+0x280]; OR EDX,0xFFFFFFFF; MOV RAX,[RCX]; CALL [RAX+0x10]` | Virtual call (vtable method 2) — likely BeginDraw setup |
| `0x1800BAFD0`–`0x1800BB025` | Scale `[RDI+0x288]`/`[RDI+0x28C]` by `[0x180188B8C]`, `XORPS` with `[0x180189070]`, store to `0x2C4`/`0x2C8`, zero `0x2CC`, `CALL 0x1800B8B00` | Coordinate transform, render setup helper |
| `0x1800BB026`–`0x1800BB197` | Four more `CALL 0x1800B8B00` with different field offsets; store panel geometry to `0x200`–`0x2F0` | Render state setup |
| `0x1800BB1E3`–`0x1800BB26C` | Bounds checks: `[RDI+0x6728]`, `[RDI+0x672C]`, `[RDI+0x2A8]`, `[RDI+0x674C]`, `[RDI+0x2D0]` | Early-out gating |
| `0x1800BB272`–`0x1800BB317` | 4-iteration loop: vtable calls `[RAX+0x48]`/`[RAX+0x50]`, FP division and adjustment | Per-vertex/panel processing |
| `0x1800BB319`–`0x1800BB6C0` | Panel setup (MOVUPS stores), more vtable calls `[RAX+0x20]`/`[RAX+0x28]`/`[RAX+0xA8]`, stack cookie, RET | Render state commit and exit |

**Draw does NOT perform visual clipping.** It performs render-state setup (virtual calls, coordinate scaling, sign-flip, helper calls, bounds gating). Clipping is delegated to `DrawFrom9Patch` (RVA `0xB99C0`, VA `0x1800B99C0`) and `DrawFrom3Patch` (RVA `0xB9680`, VA `0x1800B9680`), whose bodies were not traced.

## 6. Unexamined calls in Draw (do not infer behavior)

| Address | Call count | Status |
|---------|-----------|--------|
| `0x1800B8B00` | 4 | Unexamined helper |
| `[RAX+0x10]` (vtable) | 1 | Unexamined virtual method |
| `[RAX+0x48]` (vtable) | 4 | Unexamined virtual method |
| `[RAX+0x50]` (vtable) | 4 | Unexamined virtual method |
| `[RAX+0x20]` (vtable) | 1 | Unexamined virtual method |
| `[RAX+0x28]` (vtable) | 1 | Unexamined virtual method |
| `[RAX+0xA8]` (vtable) | 1 | Unexamined virtual method |
| `DrawFrom9Patch` (VA `0x1800B99C0`) | 0 | Not traced |
| `DrawFrom3Patch` (VA `0x1800B9680`) | 0 | Not traced |

## 7. Global values referenced by Draw (addresses only)

| Address | Inferred role | Value read? |
|---------|--------------|-------------|
| `[0x1801CDB98]` | Stack cookie | No |
| `[0x1801DC6E3]` | Render-enable flag | No |
| `[0x180188BE8]` | Scale comparison | No |
| `[0x180188B8C]` | Global scale factor | No |
| `[0x180189070]` | Sign-flip mask | No |

## 8. Raw evidence

All preserved under `export/almanac-evidence/c3ddll64-draw-complete-20261109/`:

| File | Description |
|------|-------------|
| `draw_tail_disasm.asm` | Ghidra disassembly of Draw range `0x1800BB1D0:0x1800BB400` |
| `draw_tail2_disasm.asm` | **Not produced** — see Notes. The empty placeholder was removed on 2026-11-18. |
| `draw_tail3_raw.asm` | Raw hex bytes of Draw from `0x1800BB400` to `0x1800BB900` (manual decode) |
| `collision_draw_disasm.asm` | Ghidra disassembly of Collision `0x1800BC690:0x1800BD000` and Draw `0x1800BAF40:0x1800BB200` |
| `export_directory_raw.bin` | Raw 40-byte IMAGE_EXPORT_DIRECTORY |
| `export_name_?_Collision@C3D_PANEL2D___QEAA_NVC3DVECTOR3___MM@Z_rva_0x001B4EF6.raw` | Raw Hint/Name entry for Collision |
| `export_name_?_Draw@C3D_PANEL2D___QEAAXMMMMM_N@Z_rva_0x001B7EBC.raw` | Raw Hint/Name entry for Draw |

## 9. Notes

- `draw_tail2_disasm.asm` was not produced: re-opening the Ghidra project without `-import` fails with `currentProgram is null`; re-importing triggers a "conflicting program file" error because the project already contains C3DDLL64.dll. The missing range (`0x1800BB400`–`0x1800BB700`) was recovered by reading raw bytes directly from the DLL file (`draw_tail3_raw.asm`) and manually decoding instructions.
- No Ghidra decompilation of Draw or Collision was performed — only disassembly.
- No runtime inspection, hooks, or game changes were used.

No commit or push performed for this step.
