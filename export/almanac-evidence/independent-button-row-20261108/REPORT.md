# Almanac: independent same-row button fit investigation (0x140373EA0 normal row)

Investigation and planning only. No implementation, hook, debugger, game run, or modification. Preferred-image VAs (image base 0x140000000). Source exports: `export/140373ea0_1403829fd_disasm.asm` (handler) and `export/140317db0_full_disasm.asm` (host). Latest merged review context: PR #61, merge commit 359d77a. Branch not switched; working tree left untouched.

## Scope

Focus is the normal row path of `FUN_140373EA0` when `game+0xc7d4 == -1` (0x140374133 test) and the selected handler is 373EA0 (game+0x10398 != 0, per the host dispatch at 0x14031edef / 0x14031ee13). The alternate handler `FUN_140361BB0`, content suppression, and close/return behavior are out of scope for this step.

## 1. Row drawing and collision trace (0x140373EA0 normal row)

### 1.1 Entry gate and reset

- 0x140373ea0: function entry; RSI = game pointer (RCX), RBX = RDX (output X), RDI = R8 (output Y).
- 0x140373f2f: XMM10 = dword [0x140992088] (DPI scale).
- 0x140373f38: XMM15 = XMM10.
- 0x140373f3c: MULSS XMM15, [0x14090a968].
- 0x140373f45: XMM0 = XMM10.
- 0x140373f49: XMM11 = [0x14090ab14].
- 0x140373f52: MULSS XMM0, XMM11.
- 0x140373f57: ADDSS XMM15, XMM0. (stored to [RSP+0x74] at 0x140373f5c)
- 0x140373f63: MULSS XMM10, [0x14090a8cc].
- 0x140373f6c: ADDSS XMM10, [RSI+0xae50]. (Y base = scale*const_a8cc + game+0xae50)
- 0x14037403a: MOV [RSI+0x10be8], -1 (reset before row, independent of click).
- 0x140374044: EAX = [RSI+0xc7d4]; stored [RBP-0x30].
- 0x140374133: CMP [RSI+0xc7d4], -1.
- 0x14037413a: JNZ 0x1403743f2 (alternate path; normal row only if -1).

### 1.2 Normal-row setup

- 0x140374114: XMM7 = [0x140992088] (scale).
- 0x14037411c: XMM6 = XMM7 (scale).
- 0x14037411f: MULSS XMM6, [0x14090aa0c]. (X origin component A)
- 0x140374127: XMM0 = scale.
- 0x14037412a: MULSS XMM0, XMM11 ([0x14090ab14]).
- 0x14037412f: ADDSS XMM6, XMM0. (X origin = scale*(const_aa0c + const_ab14); review's "130" is the sum of these two unverified constants)
- 0x140374140: XMM12 = [0x14090aaa0].
- 0x140374149: MULSS XMM7, XMM12. (pitch = scale * const_aaa0; review's "64" is this unverified constant)
- 0x140374155: R8D = 6.
- 0x14037415f: CALL 0x1404460a0 (panel/layout helper on game+0xadb0; no width/clip set here).
- 0x140374167: R8 = [RSI+0xc650].
- 0x14037416e: ADD R8, 0x48 (sub-entry vector begin).
- 0x140374172..0x14037418a: IMUL by 0x6666666666666667 / SAR 5 -> sub-entry count in EDX.
- 0x14037419a: TEST EDX, EDX.
- 0x14037419c: JLE 0x1403745b6 (loop exit if count <= 0).

### 1.3 Mapped index (EBX) and skip conditions

- 0x1403741a2: CMP EDI, 2. (EDI = linear loop counter i)
- 0x1403741a5: JGE 0x1403741ab.
- 0x1403741a7: MOV EBX, EDI. (i<2: mapped = i)
- 0x1403741a9: JMP 0x1403741cf.
- 0x1403741ab: JNZ 0x1403741cc.
- 0x1403741ad..0x1403741c8: if i==2, count-1 mapped index (EBX = count-1).
- 0x1403741cc: LEA EBX, [EDI-1]. (i>2: mapped = i-1)
- 0x1403741cf: CMP [RSI+0x578], 0.
- 0x1403741d6: JZ 0x1403741e4.
- 0x1403741d8: LEA EAX, [RBX-3].
- 0x1403741db: CMP EAX, 1.
- 0x1403741de: JBE 0x1403743b6. (skip mapped indices 3 and 4 when game+0x578 != 0)
- 0x1403741e4: CMP EBX, 6.
- 0x1403741e7: JNZ 0x1403741f6.
- 0x1403741e9: CMP [RSI+0x5e0], 0.
- 0x1403741f0: JZ 0x1403743b6. (skip mapped index 6 when game+0x5e0 == 0)
- 0x1403741f6: MOVSXD RAX, EBX.
- 0x1403741f9: LEA R13, [RAX+RAX*4].
- 0x1403741fd: SHL R13, 4. (R13 = RBX * 0x50)
- 0x140374201: ADD R13, [R8]. (R13 = sub-entry base + mapped*0x50)

Skipped entries jump to 0x1403743b6 (INC EDI, loop test), bypassing the X advance at 0x1403743a8. Therefore skipped entries consume no horizontal slot.

### 1.4 Per-tile draw and X advance

- 0x140374204: RCX = [R13+0x48] (sub-entry texture object).
- 0x140374208: RAX = [RCX] (vtable).
- 0x14037420b: R8 = [0x1409ead08].
- 0x140374212: XOR EDX, EDX.
- 0x140374214: CALL [RAX+0x70]. (draw texture for this tile)
- 0x140374217: XMM0 = [0x140992088] (scale).
- 0x14037421f: MULSS XMM0, XMM12 (const_aaa0? no — XMM12 currently holds [0x14090aaa0] from 0x140374140, already multiplied into XMM7 as pitch; here XMM12 is re-used? At 0x14037421f XMM12 still = [0x14090aaa0]. So width = scale * const_aaa0. Note: backdrop width equals pitch constant.)
- 0x140374224: MOVSS [0x1409be2e8], XMM0. (tile width)
- 0x14037422c: MOVSS [0x1409be2ec], XMM0. (tile height, same as width)
- 0x140374234: MOV [RSP+0x58], 0.
- 0x14037423c: XMM0 = XMM6 (current X).
- 0x14037423f: UNPCKLPS XMM0, XMM10. (pack X, Y)
- 0x140374243: MOVSD [0x1409be2f0], XMM0. (tile position X,Y)
- 0x14037424b: EAX = [RSP+0x58] (0).
- 0x14037424f: MOV [0x1409be2f8], EAX.
- 0x140374255: R8D = 6.
- 0x14037425b: XMM1 = XMM6.
- 0x14037425e: RCX = RSI+0xadb0.
- 0x140374265: CALL 0x140445eb0 (sorted-collection helper; no width/clip).
- 0x14037426a: XMM0 = [0x14090c100].
- 0x140374271: MOVUPS [0x1409be30c], XMM0.
- 0x140374278: XMM0 = [RBP-0x80] (from copied sub-vector; see 0x140374075).
- 0x14037427d: MOVSD [RBP+0x210], XMM0.
- 0x140374285: EAX = [RBP-0x78].
- 0x140374288: MOV [RBP+0x218], EAX.
- 0x14037428e: XMM3 = XMM8 (0).
- 0x140374292: XMM2 = XMM8 (0).
- 0x140374296: RDX = RBP+0x210.
- 0x14037429d: RCX = 0x1409be060 (C3D_PANEL2D widget).
- 0x1403742a4: CALL [0x14086c850]. (collision/hit test; indirect, DLL body unavailable)
- 0x1403742aa: TEST AL, AL.
- 0x1403742ac: JZ 0x140374366 (no hit: skip click handling).

### 1.5 Click handling on hit (0x1403742b2..0x140374366)

- 0x1403742b2: EDX = [R13+0x40].
- 0x1403742b6: RCX = 0x140997590.
- 0x1403742bd: CALL [0x14086c890].
- 0x1403742c3: RDX = RSI.
- 0x1403742c6: SUB RDX, RAX.
- 0x1403742d0..0x1403742e2: UTF-16 string copy loop (exits when CX == 0).
- 0x1403742e4: XMM0 = [0x14090c4c0].
- 0x1403742eb: MOVUPS [0x1409be30c], XMM0.
- 0x1403742f2: CMP byte [0x140a54e91], CL. (CL == 0 here, so compares to 0)
- 0x1403742f8: JZ 0x140374366.
- 0x1403742fa: CMP byte [RSI+0x14], CL.
- 0x1403742fd: JNZ 0x140374347.
- 0x1403742ff..0x140374345: sound playback (conditional).
- 0x140374347: CMP [RSI+0xc65c], EBX.
- 0x14037434d: JZ 0x140374356.
- 0x14037434f: MOV byte [RSI+0x1039a], 0. (conditional on index change)
- 0x140374356: MOV [RSI+0xc65c], EBX. (selected index written even if unchanged)
- 0x14037435c: MOV [RSI+0x10be8], -1. (written on hit)
- 0x140374366: CMP [RSI+0xc65c], EBX.
- 0x14037436c: JNZ 0x14037437c.
- 0x14037436e: XMM0 = [0x14090c4c0].
- 0x140374375: MOVUPS [0x1409be30c], XMM0.
- 0x14037437c: MOV byte [RSP+0x30], 1.
- 0x140374381: MOVSS [RSP+0x28], XMM8.
- 0x140374388: MOVSS [RSP+0x20], XMM9.
- 0x14037438f: XMM3 = XMM9.
- 0x140374393: XMM2 = XMM8.
- 0x140374397: XMM1 = XMM8.
- 0x14037439b: RCX = 0x1409be060.
- 0x1403743a2: CALL [0x14086c860]. (draw tile, indirect, DLL body unavailable)
- 0x1403743a8: ADDSS XMM6, XMM7. (X advance by pitch)
- 0x1403743ac: R13 = 0x6666666666666667 (reload).
- 0x1403743b6: INC EDI.
- 0x1403743b8..0x1403743de: recompute count, CMP EDI, EDX.
- 0x1403743e0: JL 0x1403741a2. (loop back)
- 0x1403743e6: LEA R12, [RSI+0xc7f0].
- 0x1403743ed: JMP 0x1403745b6. (exit to post-loop)

### 1.6 Post-loop (0x1403745b6)

- 0x1403745b6: XMM12 = [0x14090a9cc] (=35 per review, unverified here).
- 0x1403745bf: XMM1 = scale.
- 0x1403745c7: MULSS XMM1, XMM12 (35).
- 0x1403745cc: ADDSS XMM10, XMM1. (Y += 35*scale; XMM10 no longer the row Y after this)
- 0x1403745d1: XMM0 = [0x14090c4c0].
- 0x1403745d8: MOVSS [RSP+0x70], XMM10.
- 0x1403745df: MOVUPS [0x1409be30c], XMM0.
- 0x1403745e6: MOVSS [RBP-0x4c], XMM15.
- 0x1403745ec: MOV byte [RSP+0x7d], 0.
- 0x1403745f1: CMP [RSI+0xc65c], 1.
- 0x1403745f8: JA 0x14037461b.
- 0x1403745fa..0x140374619: conditional call 0x14035f3e0, then MOVSS XMM10 = [RSP+0x70]; TEST AL,AL; MOV byte [RSP+0x40],0; JNZ.
- 0x14037461b: MOV byte [RSP+0x40], 1.
- 0x140374620: R13D = [RSI+0xc7dc].
- ... further post-loop panel drawing follows (not traced here; uses XMM10 as Y).

At loop exit (0x1403743e6), XMM6 has been incremented once per drawn tile, so XMM6 = X_origin + (drawn_count)*pitch. XMM10 still equals the row Y (Y base) at 0x1403743e6; it is only modified at 0x1403745cc. XMM7 holds pitch. XMM9 holds [0x140909f58] (value unverified). XMM8 = 0.
## 2. Bounds, clipping, and click area for a ninth position

### 2.1 What the cached exports establish

The tile widget is `_DAT_0x1409be060` (a C3D_PANEL2D). Its backdrop size fields are written in the row loop at 0x140374224 / 0x14037422c (width and height both = scale * [0x14090aaa0], i.e. pitch-sized square, 64x64 under the unverified constants). Its position field is written at 0x140374243 ([0x1409be2f0] = {X, Y}). Hit-testing uses CALL [0x14086c850] at 0x1403742a4 with RCX = 0x1409be060 and RDX = RBP+0x210 (a position/size struct copied from RBP-0x80). Drawing uses CALL [0x14086c860] at 0x1403743a2 with RCX = 0x1409be060.

The host sets the same widget's dimensions and position in a block gated on `game+0xc650 != 0`:
- 0x14031e412: CMP [R14+0xc650], 0; 0x14031e41a: JZ 0x14031f140 (skip if null).
- 0x14031e420: XMM6 = [0x14090af4c] (= 512 per the reported constant list).
- 0x14031e44d: XMM1 = scale; 0x14031e458: XMM2 = scale * XMM10 ([0x14090a928]); 0x14031e45d: XMM9 = [0x14090b080] (= 1024); 0x14031e466: MULSS XMM1, XMM9.
- 0x14031e46b: RCX = 0x1409be060; 0x14031e472: CALL [0x14086c8a8]. (SetDimensions, this, width = scale*1024, height = scale*[0x14090a928], argument order unverified)
- 0x14031e478: XMM0 = [R14+0xae54]; 0x14031e481: XMM11 = [0x14090b098] (= 1200); 0x14031e48a: COMISS XMM0, XMM9 (1024).
- 0x14031e48e: JBE 0x14031e4ba.
- 0x14031e490..0x14031e4ac: if ae54 > 1024, a second CALL [0x14086c8a8] with width = scale*1200 (XMM1 = scale*1200), height = scale*XMM10 (XMM10 = [0x14090a928]? reported list does not include this address; value unverified).
- 0x14031e4ba: XMM0 = scale; 0x14031e4c2: XMM2 = scale * XMM8; 0x14031e4ca: ADDSS XMM2, [R14+0xae50]; 0x14031e4d3: XMM1 = [R14+0xae54]; 0x14031e4dc: SUBSS XMM1, XMM7; 0x14031e4e0: MULSS XMM1, XMM0.
- 0x14031e4ec: RCX = RBP+0x354; 0x14031e4f3: CALL 0x14086c008 (vector constructor).
- 0x14031e4f9: RDX = RAX; 0x14031e4fc: RCX = 0x1409be060; 0x14031e503: CALL [0x14086c8a0]. (SetPosition, this, (game+0xae54 - 512)*scale or (game+0xae54 - 600)*scale, game+0xae50 + height*scale? argument order per review is (X, Y, 0) but the exact slot/coordinate convention is unverified)
- 0x14031e509..0x14031e565: further SetPosition/Draw calls for the same widget (0x1409be060), including CALL [0x14086c860] at 0x14031e55f (draw).

So the panel width is 1024*scale or 1200*scale (whichever branch), height = scale*[0x14090a928] (value unverified; review reports 256*scale but that constant is at 0x14090adc4, not 0x14090a928). Position X = (game+0xae54 - 512 or 600)*scale, Y = game+0xae50 + (height*scale) (the ADDSS at 0x14031e4ca uses [0x14090af9c]? no — XMM7 at 0x14031e4ba holds XMM7 from earlier: at 0x14031e428 XMM7 = XMM6 = [0x14090af4c] = 512; and the branch at 0x14031e4b2 sets XMM7 = [0x14090af9c] = 600 when ae54 > 1024). So X = (game+0xae54 - 512 or 600)*scale. Y = game+0xae50 + scale*[0x14090a928? or 0x14090af9c?] (unverified).

### 2.2 What is NOT established

- The collision function [0x14086c850] and draw function [0x14086c860] are indirect call slots. Their implementations live in a DLL (C3D_PANEL2D). No DLL body, import descriptor, or vtable layout is available in the cached exports. Clipping, if performed, happens inside those implementations, not in the executable code examined.
- The executable performs no explicit bounds check before drawing a tile. Whether a tile whose X exceeds the panel width is clipped, wrapped, or drawn off-panel is determined by the DLL.
- The relationship between the tile X coordinate (scale*(const_aa0c+const_ab14) + i*pitch) and the panel X ( (game+0xae54 - 512/600)*scale ) is not established. The two coordinate systems may share an origin or may not; the constants are unverified. The review's hypothetical ninth tile at [513.6, 564.8] and the "fits" conclusion are conditional arithmetic, not a verified screen rectangle.
- The RBP-0x80 struct passed to the collision call (0x1403742a4) is built from a copied sub-vector element at 0x140374075 (CALL 0x140202270, RCX = RBP+0x50). Its fields (size, offset, flags) are not decoded here; the collision rectangle's width/height and whether it matches the 64x64 backdrop are not established from the cached exports. Absence of an explicit size in the handler does not prove the collision rect equals 64x64.
- Whether game+0xae54 takes the 300/385/685 value when the normal row is actually reached is a runtime question; the static export alone does not establish it.

### 2.3 Conclusion on bounds

Existing evidence does NOT establish that a ninth tile fits. It establishes the panel's nominal dimensions (1024/1200 wide, 256 tall, scaled) and the tile's nominal 64x64 size and pitch, but not the coordinate alignment, clipping behavior, or runtime values. The DLL body is the missing piece.

## 3. Candidate control-flow point for an independently drawn same-row button

### 3.1 Location

0x1403745b6 (the start of the post-loop block, immediately after the row loop exits at 0x1403743ed). At this address:
- XMM6 = X_origin + drawn_count * pitch (the X position for the next slot).
- XMM10 = row Y (Y base), not yet modified (the ADDSS at 0x1403745cc has not executed).
- XMM7 = pitch.
- XMM9 = [0x140909f58] (value unverified).
- XMM8 = 0.
- RSI = game pointer.
- RDI = drawn_count (loop counter, now >= count).
- The native vector and game+0xc65c have already been updated by any native click handling during the loop.

This point is after all native tiles are drawn and before the post-loop 35x35 panel draw, so an independent button drawn here appears at the end of the row and before the trailing panel.

### 3.2 Instructions to preserve

The first 17 bytes at 0x1403745b6 are:
- 0x1403745b6: MOVSS XMM12, [0x14090a9cc] (9 bytes, RIP-relative).
- 0x1403745bf: MOVSS XMM1, [0x140992088] (8 bytes, RIP-relative).

These must be relocated with RIP-relative fixup if detoured. The existing 17-byte inline hook helper in src/tesmioloader.cpp copies without relocating, so it cannot be used blindly here. All registers, flags, and the post-loop draw at 0x1403743a2...0x1403745b6 must remain reachable and uncorrupted.

### 3.3 State, drawing order, and input handling required

- Draw: set [0x1409be2e8]/[0x1409be2ec] (width/height) and [0x1409be2f0] (X,Y = XMM6, XMM10) for the button's own rectangle, then CALL [0x14086c860] (draw) with RCX = 0x1409be060, exactly as the native tiles do at 0x14037439b/0x1403743a2. Drawing order: native tiles (already drawn), then independent button, then post-loop panel.
- Hit test: build the RBP+0x210 struct (as at 0x140374278..0x140374288) and CALL [0x14086c850] (collision) with RCX = 0x1409be060. Only on AL != 0 is there a hover/hit candidate.
- Click signal: cursor position alone proves only hover. A real click requires an actual input signal plus the correct hit test, active-handler behavior, window/input gates, same-frame ordering (review section 3). The native path writes game+0x10be8 and game+0xc65c on hit; the independent button must not overwrite those (or must use separate state) to avoid corrupting native selection.
- Post-loop Y: the button must use XMM10 before 0x1403745cc (ADDSS XMM10, 35*scale), because after that instruction XMM10 is the trailing-panel Y, not the row Y.

### 3.4 Candidate status

This is an investigation candidate, not an approved implementation. Hook safety (incoming edges, interior targets, register/flag preservation, cleanup) at 0x1403745b6 is not established beyond the existing review's 17-byte analysis. No hook is specified or authorized here.
## 4. Result table

### Proven (static, from the cited exports)

| Claim | Evidence |
|---|---|
| Normal row entry gate: row runs only when game+0xc7d4 == -1 | 0x140374133 CMP; 0x14037413a JNZ 0x1403743f2 |
| Per-tile backdrop width = height = scale * [0x14090aaa0] (pitch-sized square) | 0x140374217..0x14037422c |
| Tile position stored at [0x1409be2f0] = {XMM6, XMM10} | 0x14037423c..0x140374243 |
| X origin = scale*([0x14090aa0c]+[0x14090ab14]) | 0x140374114..0x14037412f |
| Pitch = scale * [0x14090aaa0] (XMM7) | 0x140374114, 0x140374140, 0x140374149 |
| Y base = scale*[0x14090a8cc] + game+0xae50 (XMM10) | 0x140373f63, 0x140373f6c |
| Skipped indices 3,4 when game+0x578 != 0; index 6 when game+0x5e0 == 0; skips bypass X advance | 0x1403741cf..0x1403741f0, 0x1403743a8, 0x1403743b6 |
| Mapped index reordering [0,1,count-1,2,3,...] | 0x1403741a2..0x1403741cc |
| X advance occurs at 0x1403743a8 (ADDSS XMM6, XMM7), before INC EDI | 0x1403743a8, 0x1403743b6 |
| Collision call at 0x1403742a4 (RCX=0x1409be060, RDX=RBP+0x210); draw at 0x1403743a2 (RCX=0x1409be060) | 0x14037429d, 0x1403742a4, 0x14037439b, 0x1403743a2 |
| Native click writes game+0xc65c and game+0x10be8 on hit; game+0x1039a conditional on index change | 0x140374347, 0x14037434f, 0x140374356, 0x14037435c |
| game+0x10be8 reset to -1 at 0x14037403a before the row, independent of click | 0x14037403a |
| game+0x10be8 also written at 0x14037435c on hit | 0x14037435c |
| Panel widget 0x1409be060 dimensions set in host only when game+0xc650 != 0: width 1024*scale or 1200*scale, height scale*[0x14090a928] | 0x14031e412, 0x14031e420, 0x14031e44d..0x14031e4ac, 0x14031e46b, 0x14031e472 |
| Panel position X = (game+0xae54 - 512 or 600)*scale | 0x14031e478, 0x14031e4ba, 0x14031e4d3, 0x14031e4dc, 0x14031e4e0, 0x14031e4f9, 0x14031e503 |
| 17-byte span at 0x1403745b6 = two RIP-relative MOVSS (9 + 8 bytes); existing hook helper does not relocate | review section 4; bytes at 0x1403745b6..0x1403745c6 |

### Conditional (depends on unverified runtime/constants)

| Claim | Condition |
|---|---|
| Native row draws 8 tiles with no skips (game+0x578=0, game+0x5e0=1) | Requires game+0x578=0 and game+0x5e0=1 at execution; observed in a read-only snapshot but not proven atomic with handler execution |
| Ninth tile fits in the row | Requires coordinate alignment between tile X (scale*(const_aa0c+const_ab14)+8*pitch) and panel X/width; constants and runtime game+0xae54 unverified; clipping in DLL unverified |
| Hypothetical ninth tile rectangle [513.6, 564.8] | Arithmetic using 130/64/0.8; conditional only |
| Hook at 0x1403745b6 is a safe interception point | Requires incoming-edge, interior-target, register/flag, and cleanup analysis not present in cached exports |
| Independent button can be clicked | Requires a real click signal plus hit test, active-handler behavior, window/input gates, same-frame ordering; mouse position alone is insufficient |

### Unknown

| Question | Why unknown |
|---|---|
| Whether the 9th tile is clipped or visible | Clipping lives in the DLL (C3D_PANEL2D::Draw/Collision, slots 0x14086c850/0x14086c860); no DLL body or import descriptor available |
| Collision rectangle size/offset (whether it equals 64x64) | Built from RBP-0x80 via CALL 0x140202270; fields not decoded; no explicit size in handler |
| Coordinate alignment between tile X and panel X | Constants [0x14090aa0c], [0x14090ab14], [0x14090aaa0], [0x14090a8cc] not in cached exports; runtime game+0xae54 not established for the reached row |
| Exact value of [0x14090a928] (panel height multiplier) | Not in the reported constant list; only [0x14090adc4]=256 is reported |
| Whether 0x1403745b6 has additional incoming edges or interior targets | Not assessed beyond the review's two noted exits (0x14037419c JLE and 0x1403743ed JMP) |
| Runtime reachability of the normal row and the post-loop block | Static export only; no game run performed |

## 5. Smallest next read-only observation

1. Serialized Ghidra Xrefs on VA 0x1409be060 (preferred) to locate every writer of the widget's dimension/position fields, especially the constructor or init that sets width/height/clip before 0x140317db0. This is the smallest static step that can resolve the missing bounds owner. (Do not run concurrently with any other Ghidra invocation; use scratch/ghidra_run.bat with Xrefs.java, output to scratch/out/1409be060_xrefs.txt.)
2. If Xrefs is inconclusive, a read-only memory snapshot of the 0x1409be060 structure at runtime to identify which fields hold the nominal dimensions (1024/1200/256 scaled), using the host's known writes as anchors. No writes, no debugger required if a read-only memory tool is available; otherwise this requires separate approval.
3. Only after bounds are established: a read-only observation of the row's actual X origin/pitch constants via the same memory snapshot (scale at B+0x992088 and the constants at 0x14090aa0c/0x14090ab14/0x14090aaa0/0x14090a8cc) to compute the true ninth-tile X and compare against the panel width.

No implementation, hook, debugger, or game modification is authorized or performed by this report.

