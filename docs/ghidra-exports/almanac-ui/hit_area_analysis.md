# Almanac v2.9 ninth-tile hit-area analysis (read-only)

Scope: explain why the v2.9 ninth-tile hit area is below the visible tile.
Sources: cached disassembly `scratch/out/140373ea0_1403829fd_disasm.asm` (FUN_140373EA0),
`export/almanac-evidence/c3ddll64-collision-draw-20261109/collision_draw_disasm.asm` (C3DDLL64 Collision),
plugin `plugins/almanac_button/almanac_button.cpp` + `almanac_button_geometry.h`.
No new Ghidra queries were run; no source/build/game changes.

## Raw address-level artifacts (tracked)

- `docs/ghidra-exports/almanac-ui/373EA0_panel_setup_and_row_loop.asm`
  - FUN_140373EA0 setup (0x140373F1E) through row loop tail (0x1403743ED).
- `docs/ghidra-exports/almanac-ui/c3ddll64_collision_1800BC690.asm`
  - C3DDLL64 `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` at image base 0x180000000 + RVA 0xBC690.
- `docs/ghidra-exports/almanac-ui/373EA0_excerpt_1.asm` (pre-existing)
  - Row loop excerpt 0x1403740F8..0x140374408.

## Native row call order (FUN_140373EA0, normal path, c7d4 == -1)

Setup (once per handler invocation):
- 0x140373F2C: RSI = game (RCX); RDI = R8 (output ptr 2); RBX = RDX (output ptr 1).
- 0x140373F2F..0x140373F57: XMM10 = scale (0x140992088); XMM15 = scale*0x14090a968 + scale*0x14090ab14 (X origin component, "45+85" uses 0x14090aa0c/0x14090ab14 in the loop, not here); XMM10 = scale*0x14090a8cc + game+0xAE50 (row Y).
- 0x140373F75..0x140373F80: GetMouseSolid: CALL [0x14086C868], RCX = 0x140A54B90 (input object), RDX = RBP-0x80 (output C3DVECTOR3). Result stored at RBP-0x80..RBP-0x74. Called ONCE per handler invocation.
- 0x140373FA0..0x140373FD5: early-exit branch if game+0x10BE8 (unsigned) < count; otherwise set game+0x10BE8 = -1 and continue.

Row loop (0x140374114..0x1403743ED), EDI = visited index, EBX = mapped visual index:
- 0x140374114: XMM7 = scale (0x140992088).
- 0x14037411F: XMM6 = scale * 0x14090AA0C (45).
- 0x14037412A: XMM0 = scale * XMM11 (XMM11 = 0x14090AB14 = 85).
- 0x14037412F: XMM6 += XMM0  => origin X = scale*(45+85).
- 0x140374133: CMP [RSI+0xC7D4], -1; JNZ 0x1403743F2 (alternate path).
- 0x140374140: XMM12 = 0x14090AAA0 (64); XMM7 *= XMM12 => pitch = scale*64.
- 0x14037414E: CALL 0x1404460A0 (RCX = game+0xADB0, R8D = 6, XMM1 = XMM10 row Y). Helper (unexamined body).
- 0x140374167..0x14037418A: count = ([game+0xC650]+0x50 - [game+0xC650]+0x48) / 0x50.
- 0x1403741A2..0x1403741CC: EBX mapping: EDI<2 -> EBX=EDI; EDI==2 -> EBX=count-1; else EBX=EDI-1.
- 0x1403741CF..0x1403741F0: skip checks (game+0x578 nonzero skips EBX in {3,4}; game+0x5E0 zero skips EBX==6).
- 0x1403741F6: R13 = EBX*0x50 + subvec_begin (sub-entry pointer).
- 0x140374204: RCX = [R13+0x48] (per-tile object, passed to vtable[0x70]).
- 0x14037420B: R8 = [0x1409EAD08]; EDX = 0.
- 0x140374214: CALL [RCX+0x70] (vtable slot 0x70; TexBind per plugin comment, unverified).
- 0x140374217..0x14037422C: [0x1409BE2E8] = scale*64 (W); [0x1409BE2EC] = scale*64 (H). (rect store W/H)
- 0x140374234..0x14037424F: XMM0 = {XMM6 (origin+k*pitch), XMM10 (rowY)} via UNPCKLPS; [0x1409BE2F0] = XMM0 (8-byte store: X at +0, Y at +4). [0x1409BE2F8] = 0.
- 0x140374255..0x140374265: CALL 0x140445EB0 (RCX = game+0xADB0, XMM1 = XMM6 origin, R8D = 6). Helper (unexamined body).
- 0x14037426A..0x140374271: [0x1409BE30C] = xmmword [0x14090C100].
- 0x140374278..0x140374288: re-copy original mouse vector: XMM0 = [RBP-0x80] (8 bytes) -> [RBP+0x210]; EAX = [RBP-0x78] -> [RBP+0x218]. (RBP+0x210..0x21B = mouse C3DVECTOR3, refreshed per tile.)
- 0x14037428E..0x14037429D: XMM3 = 0; XMM2 = 0; RDX = RBP+0x210; RCX = 0x1409BE060 (shared panel object).
- 0x1403742A4: CALL [0x14086C850] = Collision(panel=0x1409BE060, mouseVec=RBP+0x210, 0, 0). Returns bool in AL.
- 0x1403742AA: TEST AL,AL; JZ 0x140374366 (skip click handling if no hit).
- 0x1403742B2..0x140374366: on hit: sound, selection writes (game+0xC65C = EBX, game+0x10BE8 = -1, dirty flag), etc.
- 0x140374366..0x14037437B: selected-overlay branch.
- 0x14037437C..0x1403743A2: [RSP+0x30]=1; [RSP+0x28]=0; [RSP+0x20]=XMM9; XMM3=XMM9; XMM2=0; XMM1=0; RCX=0x1409BE060; CALL [0x14086C860] = Draw(panel, 0, 0, XMM9, XMM9, 0, true).  <-- plugin hooks this slot.
- 0x1403743A8: XMM6 += XMM7 (advance origin by pitch for next tile).
- 0x1403743B6: INC EDI; loop if EDI < count.

Panel object: 0x1409BE060 (shared across all tiles; same `this` for every Draw and Collision).

Rect store globals written per tile:
- 0x1409BE2E8 (W), 0x1409BE2EC (H): scale*64 each.
- 0x1409BE2F0 (X), 0x1409BE2F4 (Y): 8-byte store {origin+k*pitch, rowY}.
- 0x1409BE2F8: 0.
- 0x1409BE30C: xmmword from 0x14090C100.

## Collision (C3DDLL64, image base 0x180000000, RVA 0xBC690)

Signature (from PE import table): `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z`
(this, C3DVECTOR3, float, float) -> bool.

Fields read from `this`:
- this+0x6734 (XMM0, base)
- this+0x673C (XMM5, scale factor)
- this+0x6740 (XMM1, lower Y bound)
- this+0x6744 (XMM4, upper Y bound)
- this+0x674C (XMM6, Y offset subtracted from mouse.y)

Logic (prefix, supported):
- 0x1800BC69E: XMM5 = this+0x673C.
- 0x1800BC6A9: COMISS XMM5, [0x180188BE8] (threshold). JBE 0x1800BC70A (skip normalization if <= threshold).
- 0x1800BC6B3: XMM1 = this+0x6740.
- 0x1800BC6BB: XMM4 = this+0x6744.
- 0x1800BC6C7: XMM6 = this+0x674C.
- 0x1800BC6E4..0x1800BC706 (if XMM5 > threshold): XMM1 -= this+0x6734; XMM4 -= this+0x6734; XMM6 /= XMM5; XMM1 /= XMM5; XMM4 /= XMM5; XMM1 += this+0x6734; XMM4 += this+0x6734. (normalization by scale factor, with base offset)
- 0x1800BC70A: XMM0 = mouse.y ([RDX+0x4]).
- 0x1800BC70F: COMISS XMM1, XMM0; JA 0x1800BCB8B (return false if mouse.y < lower bound).
- 0x1800BC718: COMISS XMM0, XMM4; JA 0x1800BCB8B (return false if mouse.y > upper bound).
- 0x1800BC758: [RDX+0x4] = mouse.y - XMM6 (mutates the caller's mouse vector Y by subtracting this+0x674C).
- Then continues with X-bounds / quad test (complex, uses panel vertices 0x2C4..0x2F0, not fully decoded here).

Supported: Collision mutates the caller's mouse vector Y (subtract this+0x674C) and uses panel fields, not the rect store globals.

## GetMouseSolid output coordinate space

- Called with input object at RVA 0xA54B90 (same as plugin's TSM_RVA_DEFAULT_INPUT).
- Output is a C3DVECTOR3 written to RBP-0x80 (x,y,z), 12 bytes (8 + 4).
- The native row re-copies this vector per tile into RBP+0x210 before Collision.
- Collision receives this vector and compares its Y against panel fields this+0x6740/0x6744 (after optional normalization by this+0x673C with base this+0x6734), then subtracts this+0x674C from Y.
- The rect store X/Y (0x9BE2F0) are NOT inputs to Collision. Collision uses the panel object's fields, which are set by the host (0x140318D79 writes this+0x673C = 1; DLL init sets this+0x673C = 1.0f, this+0x6740 = 0, this+0x6744 = ~1e9 as 64-bit stores) and are in the same space as the mouse vector (input space).

## Plugin x/y/w/h calculation (from almanac_button.cpp / geometry.h)

- scale = game+0x992088 (TSM_RVA_DEFAULT_DPI), originA = 45 (0x90AA0C), originB = 85 (0x90AB14), pitch = 64 (0x90AAA0).
- origin = scale*(45+85). ninth X = origin + 8*pitch = scale*642.
- Y = rowY = scale*15 + game+0xAE50 (captured from rect store 0x9BE2F4, which equals XMM10).
- w = h = scale*pitch = scale*64.
- CheckNinthClick: GetMouseSolid(input, &mouse); test mouse.x in [x, x+w), mouse.y in [y, y+h] (X half-open, Y inclusive, v2.8). No transform, no panel-bounds normalization, no subtraction of this+0x674C.

## Supported facts

1. The native hit test (Collision) operates on the shared panel object 0x1409BE060 and its fields (this+0x6734/0x673C/0x6740/0x6744/0x674C). It does not directly read the rect-store position pair at 0x9BE2F0/0x9BE2F4 (panel offsets +0x290/+0x294). However, the rect-store size fields at 0x9BE2E8/0x9BE2EC ARE panel fields at offsets +0x288/+0x28C, and Collision reads those (XMM5/XMM6 paths use panel geometry that includes the size). The position pair's only established consumer is Draw positioning; whether it has any indirect effect on collision state is not established.
2. Collision mutates the caller's mouse vector Y by subtracting this+0x674C (0x1800BC758), but only on the in-bounds path after the Y-range comparison (this+0x6740 <= mouse.y <= this+0x6744). The plugin's CheckNinthClick does not apply this adjustment.
3. The plugin's Y (rowY = scale*15 + game+0xAE50) is the rect-store/Draw Y. The native collision Y bounds are this+0x6740..this+0x6744 (panel fields). Whether these bounds coincide with rowY at runtime is not established by static exports.
4. GetMouseSolid is called once per handler invocation (0x140373F80), before the row loop. The saved vector (RBP-0x80) is copied into each tile's Collision argument (0x140374278); it is not re-sampled per tile. The plugin makes a separate GetMouseSolid call for its ninth-tile AABB check, so the two paths do not share one mouse sample.
5. The extra Draw at the ninth-tile position writes the rect-store X/Y pair (one 8-byte store) before the native Draw and restores the saved pair after. It does not directly write the panel fields this+0x673C etc. However, the Draw path writes derived panel-geometry fields that Collision also reads (Collision recomputes several of them itself). Whether an extra Draw leaves any state that affects a later Collision is unresolved; neither "it does" nor "it does not" is established.
6. The plugin's Detour only acts on the row's own eighth-tile Draw (return address 0x3743A8, self == 0x9BE060). The ninth Draw is a separate call to g_drawOrig with the same arguments.

## Inference (not directly proven)

- The plugin's simple AABB differs from the native panel-collision algorithm (which uses panel fields, a Y-range test, a Y adjustment, and further geometry calculations). A hit-test mismatch between the two is therefore plausible, but it is not demonstrated that their coordinate spaces differ or that such a difference caused the reported offset.
- The native hit test's Y adjustment (subtract this+0x674C) would shift the effective hit region relative to the drawn tile only if this+0x674C is nonzero and only if the plugin's Y (rowY) is in the same numeric space as the mouse vector. Neither condition is established.
- The panel's collision Y bounds (this+0x6740..this+0x6744) may or may not span the tile row; their exact values and relation to rowY are not established by static exports (host writes only this+0x673C; DLL init sets this+0x6740=0, this+0x6744=~1e9).
- Whether C3D_PANEL2D::Draw reads the rect store globals (0x9BE2E8..) to position the panel, or whether the panel position comes from elsewhere, is not verified. The plugin assumes the rect store drives Draw position, which is supported by the handler writing it before Draw, but Draw's consumption of it is unverified.

## Unresolved

1. Runtime value of this+0x674C (0x1409C47AC) during the trade row. Static exports do not set it (DLL init covers +0x673C/+0x6740/+0x6744 only; host writes +0x673C). Without it, the native Y adjustment magnitude is unknown.
2. Runtime values of this+0x6740 and this+0x6744 (0x1409C47A0, 0x1409C47A4) during the trade row, and their relation to rowY / the drawn tile Y.
3. Whether GetMouseSolid output is in the same space as the rect store X/Y, or whether an additional DPI/camera transform applies. The input object (0x140A54B90) state is not decoded here.
4. Whether Draw reads the rect store globals or uses the panel's own position fields. The Draw body (C3DDLL64 RVA 0xBAF40) was only partially decoded (prefix); its consumption of 0x9BE2F0 is not established.
5. The helper bodies 0x1404460A0 and 0x140445EB0 (called per tile with game+0xADB0) are unexamined; their effect on panel/rect state is unknown.
6. No runtime measurement of mouse coordinates or panel bounds was performed (debugger unavailable per prior report). The misalignment cause is inferred from static code, not observed.

## Conclusion

The plugin's ninth-tile hit test (CheckNinthClick) is a simple AABB over GetMouseSolid output using rect-store draw coordinates (X = origin+8*pitch, Y = rowY). The native hit test (Collision) is a panel-field-based algorithm with a Y-range test, a Y adjustment (subtract this+0x674C on the in-bounds path), and further geometry calculations, and it reads the rect-store size fields (panel offsets +0x288/+0x28C) as part of that geometry. The two algorithms differ, so a hit-test mismatch is plausible. Whether the reported offset is caused by a coordinate-space difference, by the missing Y adjustment, by the extra Draw leaving state that affects a later Collision, or by another factor is not established: the runtime values of this+0x674C and the panel Y bounds, the mouse coordinates, and Draw's consumption of the rect store are all unresolved. The coordinate-space mismatch remains a hypothesis, not a demonstrated root cause.
