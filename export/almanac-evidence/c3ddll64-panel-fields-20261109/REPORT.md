# Almanac: C3DDLL64.dll — Panel field writes for Collision bounds (+0x673c, +0x6740, +0x6744)

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

## 1. Summary of findings

The Collision bounds fields `+0x673c` (scale), `+0x6740` (bound Y1), `+0x6744` (bound Y2) on the C3D_PANEL2D object are:

- **NEVER written by SOVIET64.exe** — zero disp32 references to `0x673c`, `0x6740`, or `0x6744` anywhere in SOVIET64.exe code
- **Written only in C3DDLL64.dll** at three locations: two constructor/init functions (static initialization), one copy function, and two Draw-time computed updates
- **Read by Collision** (VA `0x1800BC690`) and **Draw** (VA `0x1800BAF40`) for hit-test bounds computation

## 2. DLL identification

| Field | Value |
|-------|-------|
| Path | `C:\Workers Resources Soviet Republic\C3DDLL64.dll` |
| SHA-256 | `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516` |
| Image base | `0x180000000` (PE32+ / 64-bit) |

## 3. Write location 1 — Field copy function (VA 0x18002DEE0–0x18002DF1D)

Copies panel fields field-by-field from source object `[RDX]` to dest object `[R9]`. This is a copy constructor or assignment operator for C3D_PANEL2D.

| Address | Instruction | Operation |
|---------|-------------|-----------|
| `0x18002DEE3` | `MOV EAX, dword ptr [RDX + 0x673c]` | Read +0x673c from source |
| `0x18002DEE9` | `MOV dword ptr [R9 + 0x673c], EAX` | **Write +0x673c to dest** |
| `0x18002DEF0` | `MOV EAX, dword ptr [RDX + 0x6740]` | Read +0x6740 from source |
| `0x18002DEF6` | `MOV dword ptr [R9 + 0x6740], EAX` | **Write +0x6740 to dest** |
| `0x18002DEFD` | `MOV EAX, dword ptr [RDX + 0x6744]` | Read +0x6744 from source |
| `0x18002DF03` | `MOV dword ptr [R9 + 0x6744], EAX` | **Write +0x6744 to dest** |
| `0x18002DF0A` | `MOV EAX, dword ptr [RDX + 0x6748]` | Read +0x6748 from source |
| `0x18002DF10` | `MOV dword ptr [R9 + 0x6748], EAX` | **Write +0x6748 to dest** |
| `0x18002DF17` | `MOV EAX, dword ptr [RDX + 0x674c]` | Read +0x674c from source |
| `0x18002DF1D` | `MOV dword ptr [R9 + 0x674c], EAX` | **Write +0x674c to dest** |

This is a **static field copy** — values come from another panel object, not from constants.

## 4. Write location 2 — Constructor/init function A (VA 0x18005BED0–0x18005BFD)

Initializes panel object at `[R12]`. Static initialization of bounds fields.

| Address | Instruction | Operation |
|---------|-------------|-----------|
| `0x18005BED0` | `MOV qword ptr [R12 + 0x673c], 0x3f800000` | **Write +0x673c = 1.0f** (scale) |
| `0x18005BEDC` | `MOV qword ptr [R12 + 0x6744], 0x4e6e6b28` | **Write +0x6744 = 0x4E6E6B28** (~1.0×10^9 as float) |
| `0x18005BEE8` | `MOV qword ptr [R12 + 0x6728], 0x0` | Write +0x6728 = 0 (unrelated field) |
| `0x18005BEF4` | `MOV byte ptr [R12 + 0x6760], 0x1` | Write +0x6760 = 1 (flag) |
| `0x18005BF01` | `MOVSD qword ptr [R12 + 0x6730], XMM0` | Write +0x6730 = XMM0 (origin-related) |
| `0x18005BF0B` | `MOV dword ptr [RBP + 0xac], 0x0` | Unrelated (different object base) |

**0x4E6E6B28 as IEEE 754 float:** approximately 1.0×10^9 — effectively an "infinite" upper bound (any reasonable screen coordinate passes the hit-test).

## 5. Write location 3 — Constructor/init function B (VA 0x1800B8AD0–0x1800B8B02)

Initializes panel object at `[RCX]`. Same static initialization pattern as location 2, ending with RET at `0x1800B8AFF`.

| Address | Instruction | Operation |
|---------|-------------|-----------|
| `0x1800B8AD3` | `MOV qword ptr [RCX + 0x673c], 0x3f800000` | **Write +0x673c = 1.0f** (scale) |
| `0x1800B8ADE` | `MOV qword ptr [RCX + 0x6744], 0x4e6e6b28` | **Write +0x6744 = 0x4E6E6B28** (~1.0×10^9) |
| `0x1800B8AE9` | `MOV qword ptr [RCX + 0x6728], RDX` | Write +0x6728 = RDX (pointer argument) |
| `0x1800B8AF0` | `MOV byte ptr [RCX + 0x6760], 0x1` | Write +0x6760 = 1 (flag) |
| `0x1800B8AFF` | `RET` | Function return |

At `0x1800B8B00` a new function begins (FUN_1800B8B00), which reads `[RBX+0x673c]` at `0x1800B8B15` — this is called from Draw at `0x1800BB0A6`.

## 6. Write locations 4 & 5 — Draw-time computed updates

During Draw (VA `0x1800BAF40`), the bounds fields are overwritten with computed values:

| Address | Instruction | Operation | Context |
|---------|-------------|-----------|---------|
| `0x1800BB26E` | `MOVSS [RCX + 0x6740], XMM1` | **Write +0x6740 = computed XMM1** | Inside Draw, after bounds computation |
| `0x1800BB682` | `MOVSS [RCX + 0x6744], XMM2` | **Write +0x6744 = computed XMM2** | Inside Draw, after bounds computation |

These overwrite the statically-initialized values with per-frame computed bounds.

## 7. Read locations (for context)

| Address | Instruction | Operation | Function |
|---------|-------------|-----------|----------|
| `0x1800BC69E` | `MOVSS XMM5, [RCX + 0x673c]` | Read +0x673c (scale) | Collision |
| `0x1800BC6B3` | `MOVSS XMM1, [RCX + 0x6740]` | Read +0x6740 (bound Y1) | Collision |
| `0x1800BC6BB` | `MOVSS XMM4, [RCX + 0x6744]` | Read +0x6744 (bound Y2) | Collision |
| `0x1800BC6C7` | `MOVSS XMM6, [RCX + 0x674c]` | Read +0x674c (depth) | Collision |
| `0x1800BB63A` | `MOVSS XMM4, [RCX + 0x673c]` | Read +0x673c | Draw |
| `0x1800BBA74` | `MOVSS XMM1, [RCX + 0x673c]` | Read +0x673c | Draw/Collision vicinity |
| `0x1800BBAA2` | `MOVSS XMM5, [RCX + 0x673c]` | Read +0x673c | Draw/Collision vicinity |

## 8. Callers of constructor/init functions

**Not yet identified.** The two constructor/init functions at VA `0x18005BED0` and `0x1800B8AD0` have not had their callers traced. Finding callers would require Ghidra Xrefs on the C3DDLL64.dll project for these target addresses, which was not completed in this session.

## 9. Static vs dynamic values — trade row distinction

**Static assignments** (set once at object creation, persist until overwritten):
- `+0x673c` = 1.0f (scale) — set in both constructors, never changed by Draw
- `+0x6744` = 0x4E6E6B28 (~1.0e9) — set in both constructors, **overwritten by Draw** at `0x1800BB682`
- `+0x6760` = 1 (enabled flag) — set in both constructors

**Dynamic assignments** (recomputed each frame during Draw):
- `+0x6740` — written by Draw at `0x1800BB26E` with computed XMM1 value
- `+0x6744` — written by Draw at `0x1800BB682` with computed XMM2 value (overrides static init)

**During the trade row:** When Collision runs for hit-testing, it reads `+0x673c`/`+0x6740`/`+0x6744` from the panel object. The values at that moment reflect whatever Draw last wrote (if Draw ran before Collision in the frame) or the static constructor defaults (if Draw hasn't updated them yet). The exact ordering of Draw vs Collision within a frame is not determined by this static analysis.

## 10. Hit-test vs clipping distinction

The Collision function uses these fields for **hit-test bounds only** (point-in-rectangle test using COMISS/JA comparisons). It does **not** perform visual clipping. Visual clipping, if any, occurs in `DrawFrom9Patch` (VA `0x1800B99C0`) or `DrawFrom3Patch` (VA `0x1800B9680`), whose bodies were not traced.

## 11. Raw evidence

All preserved under `export/almanac-evidence/c3ddll64-panel-fields-20261109/`:

| File | Description |
|------|-------------|
| `panel_field_writes_disasm.asm` | Ghidra disassembly of VA ranges `0x18002DEE0:0x18002DF20` (copy), `0x18005BED0:0x18005BF10` (constructor A), `0x1800B8AD0:0x1800B8C00` (constructor B) |
| `constructor_disasm.asm` | Earlier Ghidra disassembly of `0x18002D2C0:0x18002D360` (bulk MOVUPS copy, does NOT touch +0x673c/0x6740/0x6744) |

No commit or push performed for this step.
