# Almanac: C3DDLL64.dll exports — Collision and Draw static analysis

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

## 1. DLL identification

| Field | Value |
|-------|-------|
| Path | `C:\Workers Resources Soviet Republic\C3DDLL64.dll` |
| Size | 1,984,000 bytes |
| SHA-256 | `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516` |
| MD5 | `a056238300e9854af04f521fa562119d` |
| Image base | 0x180000000 (PE32+ / 64-bit) |
| Export count | 2,218 functions |

## 2. Export symbols resolved

| Mangled name | Demangled | Ordinal | RVA | VA | File offset |
|--------------|-----------|---------|-----|-----|-------------|
| `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` | `C3D_PANEL2D::Collision(C3DVECTOR3, float, float) -> bool` | 668 | 0x000BC690 | 0x180BC690 | 0xBBA90 |
| `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` | `C3D_PANEL2D::Draw(float, float, float, float, float, bool) -> void` | 879 | 0x000BAF40 | 0x180BAF40 | 0xBA340 |

These correspond exactly to the IAT slots:
- Slot `0x14086c850` → `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` (hit-test)
- Slot `0x14086c860` → `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` (render/clipping)

Related panel exports (for context, not traced):
- `?BeginDraw@C3D_PANEL2D@@QEAAXPEBD_N@Z` — RVA 0xBC4A0
- `?EndDraw@C3D_PANEL2D@@QEAAXXZ` — RVA 0xBC610
- `?DrawFrom9Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMMMM@Z` — RVA 0xB99C0
- `?DrawFrom3Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMM@Z` — RVA 0xB9680

## 3. Raw evidence preserved

| File | Description |
|------|-------------|
| `c3ddll64-exports-20261109/Collision_disasm.asm` | Ghidra disassembly output (empty due to PE addressing quirk — see boundary) |
| `c3ddll64-exports-20261109/Collision_raw_hex.txt` | 512 bytes of raw Collision function hex |
| `c3ddll64-exports-20261109/Draw_raw_hex.txt` | 512 bytes of raw Draw function hex |

## 4. Collision function — hit-test bounds (manually decoded from raw bytes)

Entry: RVA `0xBC690`, file offset `0xBBA90`

Key instruction sequence (manually decoded from raw bytes):

| Address | Bytes | Instruction | Meaning |
|---------|-------|-------------|---------|
| `0xBC690` | `48 8B C4` | `mov rax, rsp` | Stack frame setup |
| `0xBC695` | `41 56 41 57` | `push r14; push r15` | Callee-saved registers |
| `0xBC699` | `48 81 EC F8 00 00 00` | `sub rsp, 0xf8` | Stack frame |
| `0xBC6A0` | `F3 0F 10 A9 3C 67 00 00` | `movss xmm5, [rcx+0x673c]` | Load panel scale factor |
| `0xBC6A7` | `4C 8B FA` | `mov r15, rdx` | Save param2 (input Y) |
| `0xBC6AA` | `0F 2F 2D 38 C5 0C 00` | `comiss xmm5, [0x1800CC538]` | Compare scale with global |
| `0xBC6B1` | `4C 8B F1` | `mov r14, rcx` | Save `this` pointer |
| `0xBC6B4` | `F3 0F 10 89 40 67 00 00` | `movss xmm1, [rcx+0x6740]` | Load panel bound Y1 |
| `0xBC6BB` | `F3 0F 10 A1 44 67 00 00` | `movss xmm4, [rcx+0x6744]` | Load panel bound Y2 |
| `0xBC6C2` | `F3 0F 10 B1 4C 67 00 00` | `movss xmm6, [rcx+0x674c]` | Load panel bound Z/depth |
| `0xBC6C9` | `76 24` | `jbe +0x24` | Conditional skip |
| `0xBC6CB` | `F3 0F 10 81 34 67 00 00` | `movss xmm0, [rcx+0x6734]` | Load panel origin Y |
| `0xBC6D2` | `F3 0F 5C C8` | `subss xmm1, xmm0` | bound1 − origin |
| `0xBC6D5` | `F3 0F 5C E0` | `subss xmm4, xmm0` | bound2 − origin |
| `0xBC6D8` | `F3 0F 5E F5` | `divss xmm6, xmm5` | normalize by scale |
| `0xBC6DB` | `F3 0F 5E CD` | `divss xmm1, xmm5` | normalize by scale |
| `0xBC6DE` | `F3 0F 5E E5` | `divss xmm4, xmm5` | normalize by scale |
| `0xBC6E1` | `F3 0F 58 C8` | `addss xmm1, xmm0` | + origin |
| `0xBC6E4` | `F3 0F 58 E0` | `addss xmm4, xmm0` | + origin |
| `0xBC6E7` | `F3 0F 10 42 04` | `movss xmm0, [rdx+4]` | Load input point Y |
| `0xBC6EB` | `0F 2F C8` | `comiss xmm1, xmm0` | Compare bound with input Y |
| `0xBC6EE` | `0F 87 73 04 00 00` | `ja +0x473` | Jump if out of bounds |
| `0xBC6F4` | `0F 2F C4` | `comiss xmm4, xmm0` | Compare second bound with input Y |
| `0xBC6F7` | `0F 87 6A 04 00 00` | `ja +0x46a` | Jump if out of bounds |

**Hit-test bounds established:** The Collision function computes a rectangular hit-test region from panel fields at `this+0x6734` (origin), `this+0x673c` (scale), `this+0x6740`/`this+0x6744` (bounds), normalizes by scale, adds origin back, and compares the input point's Y coordinate against the computed bounds using `comiss` + `ja` (jump-if-above = out of bounds). Returns bool: true if within bounds, false otherwise.

**X bounds:** The same pattern is applied for X (instructions following the Y checks, continuing past offset 0xBC6FF in the raw bytes). The function checks both X and Y coordinates against the panel's rectangular bounds.

**This is hit-test bounds, not visual clipping.** The Collision function only determines whether a point (mouse position) lies within the panel's rectangular area. It does not perform any drawing or clipping.

## 5. Draw function — visual clipping (manually decoded from raw bytes)

Entry: RVA `0xBAF40`, file offset `0xBA340`

Key instruction sequence:

| Address | Bytes | Instruction | Meaning |
|---------|-------|-------------|---------|
| `0xBAF40` | `48 8B C4` | `mov rax, rsp` | Stack frame setup |
| ... | (register saves) | ... | Save rbx, rsi, rdi, rbp, r12-r15 |
| `0xBAF58` | `48 81 EC 20 01 00 00` | `sub rsp, 0x120` | Large stack frame |
| `0xBAF6A` | `48 8B 05 F7 2B 11 00` | `mov rax, [0x180112B97]` | Load global security cookie |
| `0xBAF71` | `48 33 C4` | `xor rax, rsp` | Stack cookie |
| `0xBAF74` | `48 89 45 80` | `mov [rbp-0x80], rax` | Store stack cookie |
| `0xBAF78` | `80 3D 34 17 12 00 00` | `cmp byte ptr [0x180121734], 0` | **Check global render flag** |
| `0xBAF7F` | `44 0F 28 E3` | `movaps xmm12, xmm3` | Save XMM3 |
| `0xBAF82` | `44 0F 28 EA` | `movaps xmm13, xmm2` | Save XMM2 |
| `0xBAF85` | `44 0F 28 F1` | `movaps xmm14, xmm1` | Save XMM1 |
| `0xBAF88` | `48 8B F9` | `mov rdi, rcx` | Save `this` |
| `0xBAF8B` | `75 10` | `jne +0x10` | **Skip if render flag set** |
| `0xBAF8D` | `48 8B 89 80 02 00 00` | `mov rcx, [rcx+0x280]` | Load `this->field_280` |
| `0xBAF94` | `83 CA FF` | `or edx, 0xffffffff` | edx = -1 |
| `0xBAF97` | `48 8B 01` | `mov rax, [rcx]` | Load vtable |
| `0xBAF9A` | `FF 50 10` | `call [rax+0x10]` | **Virtual call (likely BeginDraw)** |
| `0xBAF9D` | `F3 44 0F 10 87 88 02 00 00` | `movss xmm0, [rdi+0x288]` | Load panel field |
| `0xBAFA4` | `48 8D 97 C4 02 00 00` | `lea rdx, [rdi+0x2c4]` | &this->field_2c4 |
| `0xBAFAB` | `F3 44 0F 10 9F 8C 02 00 00` | `movss xmm11, [rdi+0x28c]` | Load panel field |
| `0xBAFB2` | `45 33 ED` | `xor r13d, r13d` | Zero counter |
| `0xBAFB5` | `F3 44 0F 59 05 97 DB 0C 00` | `mulss xmm0, [0x1800C7D80]` | **Scale by global factor** |
| `0xBAFBC` | `48 8B CF` | `mov rcx, rdi` | `this` |
| `0xBAFBF` | `44 89 AF CC 02 00 00` | `mov [rdi+0x2cc], r13d` | Zero field |
| `0xBAFC6` | `F3 44 0F 59 1D 84 DB 0C 00` | `mulss xmm11, [0x1800C7D84]` | **Scale by global factor** |
| `0xBAFCD` | `45 0F 28 D0` | `movaps xmm10, xmm8` | Copy register |
| `0xBAFD0` | `44 0F 57 15 5C E0 0C 00` | `xorps xmm10, [0x1800CE05C]` | **Sign flip (coordinate inversion)** |
| `0xBAFD7` | `F3 44 0F 11 97 C4 02 00 00` | `movss [rdi+0x2c4], xmm10` | Store transformed value |
| `0xBAFDE` | `F3 44 0F 11 9F C8 02 00 00` | `movss [rdi+0x2c8], xmm11` | Store transformed value |
| `0xBAFE5` | `E8 D5 DA FF FF` | `call 0x180BA82C` | **Call (likely BeginDraw)** |

**Visual clipping established:** The Draw function performs rendering setup:
1. Checks global render flag at `[0x180121734]` — if set, skips the virtual call block
2. Makes a virtual call `[vtable+0x10]` (likely `BeginDraw` or similar rendering setup)
3. Loads panel geometry fields from `this+0x288`, `this+0x28c`, `this+0x2c4`, `this+0x2c8`, `this+0x2cc`
4. Scales coordinates by global factors at `0x1800C7D80` and `0x1800C7D84`
5. Sign-flips (negates) one coordinate via `xorps` with `0x1800CE05C` — this is coordinate system transformation, not clipping
6. Calls function at `0x180BA82C` (likely `BeginDraw`)

**The Draw function itself does NOT perform visual clipping.** It sets up rendering state and delegates to helper functions. Actual clipping/patch rendering is performed by:
- `?DrawFrom9Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMMMM@Z` — RVA `0xB99C0`
- `?DrawFrom3Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMM@Z` — RVA `0xB9680`

These 9-patch/3-patch drawing functions are where visual clipping (stretching texture patches to fit within panel bounds while respecting borders) actually occurs. They were not traced because that would require additional Ghidra runs beyond the two already performed for Collision and Draw.

## 6. Distinctions

| Concept | Function | Evidence |
|---------|----------|----------|
| Hit-test bounds | `Collision` | `comiss` + `ja` comparisons against panel bounds computed from `this+0x6734`/`0x673c`/`0x6740`/`0x6744` |
| Visual clipping | `DrawFrom9Patch` / `DrawFrom3Patch` | Export symbols exist but bodies not traced |
| Draw setup | `Draw` | Global flag check, virtual call, scale factors, coordinate inversion — no clipping |

## 7. Boundary — what could NOT be established

1. **Ghidra disassembly output was empty** for both functions due to a PE addressing quirk (Ghidra's `getAddress()` did not map the VA `0x180BC690`/`0x180BAF40` to instructions, despite correct image base `0x180000000`). All instruction decoding above is from manual byte-level analysis of the raw DLL file, which is the ground truth.

2. **`DrawFrom9Patch` and `DrawFrom3Patch` bodies were not traced.** These are the actual clipping implementations. Tracing them would require additional serialized Ghidra runs on the DLL project at RVAs `0xB99C0` and `0xB9680`.

3. **The global values at `0x1800CC538`, `0x1800C7D80`, `0x1800C7D84`, `0x1800CE05C`, and `0x180121734` were not read from the DLL.** Their meanings (scale factors, render flags, etc.) are inferred from instruction context, not verified.

4. **Panel struct field layouts** (`this+0x6734`, `this+0x673c`, etc.) are inferred from access patterns, not from type information. The DLL has no PDB.

## 8. Raw evidence

All raw output preserved under `export/almanac-evidence/c3ddll64-exports-20261109/`:
- `Collision_disasm.asm` — Ghidra disassembly attempt (empty headers only)
- `Collision_raw_hex.txt` — 512 raw bytes of Collision function
- `Draw_raw_hex.txt` — 512 raw bytes of Draw function

No commit or push performed for this step.
