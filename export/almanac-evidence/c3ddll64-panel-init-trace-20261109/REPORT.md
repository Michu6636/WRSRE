# Almanac: Panel `0x1409be060` — Source of +0x6740/+0x6744 before trade-row Collision

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

## 1. Summary

The shared panel object at `0x1409be060` (C3D_PANEL2D) is used as the `this` pointer (RCX) for multiple DLL function calls from SOVIET64.exe. **None of the observed callers write to +0x6740 or +0x6744.** The Collision hit-test reads these fields but does not write them. The Draw function writes both, but runs **after** Collision in the handler flow. Therefore, the values of `+0x6740` and `+0x6744` at trade-row Collision time cannot be determined from static analysis of the observed call path.

## 2. Panel object usage — all observed IAT calls with RCX = 0x1409be060

From SOVIET64.exe's import table (C3DDLL64.dll descriptor #0, ILT base RVA `0x984CF8`, IAT base RVA `0x86C000`):

| IAT slot | VA called | Exported function | Writes +0x6740? | Writes +0x6744? |
|----------|-----------|-------------------|-----------------|-----------------|
| `0x14086c918` | `0x1800BC4A0` | `?BeginDraw@C3D_PANEL2D@@QEAAXPEBD_N@Z` | No | No |
| `0x14086c898` | — | `?SetColor@C3D_PANEL2D@@QEAAXAEBVC3DFCOLOR@@@Z` | No | No |
| `0x14086c8a0` | `0x18002D9E0` | `?SetPosition@C3D_PANEL2D@@QEAAXAEBVC3DVECTOR3@@@Z` | No | No |
| `0x14086c8a8` | `0x18002D9C0` | `?SetDimensions@C3D_PANEL2D@@QEAAXMM@Z` | No | No |
| `0x14086c960` | — | `?SetRotation@C3D_PANEL2D@@QEAAXM@Z` | No | No |
| `0x14086c850` | `0x1800BC690` | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` | No (reads only) | No (reads only) |
| `0x14086c860` | `0x1800BAF40` | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` | Yes (`0x1800BB26E`) | Yes (`0x1800BB682`) |
| `0x14086c868` | — | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` | N/A (input object) | N/A |

Verified call sites in `export/140317db0_full_disasm.asm`:
- `0x1403181ca`: `LEA RCX,[0x1409be060]; CALL [0x14086c918]` (BeginDraw)
- `0x14031825f`: `LEA RCX,[0x1409be060]; CALL [0x14086c898]` (SetColor)
- `0x1403182f2`: `LEA RCX,[0x1409be060]; CALL [0x14086c8a0]` (SetPosition)
- `0x14031830d`: `LEA RCX,[0x1409be060]; CALL [0x14086c8a8]` (SetDimensions)
- `0x140318322`: `LEA RCX,[0x1409be060]; CALL [0x14086c960]` (SetRotation)

Approximately 70 total `LEA RCX,[0x1409be060]` references exist in the host disassembly; 5 were verified. The remainder follow the same pattern (panel manipulation setters).

## 3. Where +0x6740 and +0x6744 ARE written (all in C3DDLL64.dll)

SOVIET64.exe has **zero** disp32 references to `0x673c`, `0x6740`, or `0x6744` anywhere in its code. All writes occur in C3DDLL64.dll:

### 3a. Copy constructor (VA `0x18002DEE0`–`0x18002DF1D`)

Copies fields field-by-field from source panel `[RDX]` to dest panel `[R9]`:

| VA | Instruction | Field |
|----|-------------|-------|
| `0x18002DEE3` | `MOV EAX, dword ptr [RDX + 0x673c]` | read +0x673c |
| `0x18002DEE9` | `MOV dword ptr [R9 + 0x673c], EAX` | **write +0x673c** |
| `0x18002DEF0` | `MOV EAX, dword ptr [RDX + 0x6740]` | read +0x6740 |
| `0x18002DEF6` | `MOV dword ptr [R9 + 0x6740], EAX` | **write +0x6740** |
| `0x18002DEFD` | `MOV EAX, dword ptr [RDX + 0x6744]` | read +0x6744 |
| `0x18002DF03` | `MOV dword ptr [R9 + 0x6744], EAX` | **write +0x6744** |
| `0x18002DF0A` | `MOV EAX, dword ptr [RDX + 0x6748]` | read +0x6748 |
| `0x18002DF10` | `MOV dword ptr [R9 + 0x6748], EAX` | **write +0x6748** |
| `0x18002DF17` | `MOV EAX, dword ptr [RDX + 0x674c]` | read +0x674c |
| `0x18002DF1D` | `MOV dword ptr [R9 + 0x674c], EAX` | **write +0x674c** |

This copies values from another panel object — it does not initialize from constants.

### 3b. Constructor/init function A (VA `0x18005BED0`–`0x18005BFD`)

| VA | Instruction | Field | Value |
|----|-------------|-------|-------|
| `0x18005BED0` | `MOV qword ptr [R12 + 0x673c], 0x3f800000` | **+0x673c** | 1.0f (scale) |
| `0x18005BEDC` | `MOV qword ptr [R12 + 0x6744], 28 6B 6E 4E` | **+0x6744** | 0x4E6E6B28 (~1.0e9) |
| `0x18005BEE8` | `MOV qword ptr [R12 + 0x6728], 0` | +0x6728 | 0 |
| `0x18005BEF4` | `MOV byte ptr [R12 + 0x6760], 1` | +0x6760 | 1 (enabled flag) |
| `0x18005BF01` | `MOVSD qword ptr [R12 + 0x6730], XMM0` | +0x6730 | XMM0 (origin) |

**+0x6740 is NOT written by this constructor.**

0x4E6E6B28 as IEEE 754 float32 ≈ 1.0×10⁹ — effectively an "infinite" upper bound.

### 3c. Constructor/init function B (VA `0x1800B8AD0`–`0x1800B8B02`)

| VA | Instruction | Field | Value |
|----|-------------|-------|-------|
| `0x1800B8AD3` | `MOV qword ptr [RCX + 0x673c], 0x3f800000` | **+0x673c** | 1.0f |
| `0x1800B8ADE` | `MOV qword ptr [RCX + 0x6744], 28 6B 6E 4E` | **+0x6744** | 0x4E6E6B28 (~1.0e9) |
| `0x1800B8AE9` | `MOV qword ptr [RCX + 0x6728], RDX` | +0x6728 | pointer arg |
| `0x1800B8AF0` | `MOV byte ptr [RCX + 0x6760], 1` | +0x6760 | 1 |
| `0x1800B8AFF` | `RET` | | |

**+0x6740 is NOT written by this constructor either.**

### 3d. Draw-time updates (VA `0x1800BAF40`+)

| VA | Instruction | Field | Value | Timing |
|----|-------------|-------|-------|--------|
| `0x1800BB26E` | `MOVSS [RCX + 0x6740], XMM1` | **+0x6740** | computed XMM1 | **After Collision** |
| `0x1800BB682` | `MOVSS [RCX + 0x6744], XMM2` | **+0x6744** | computed XMM2 | **After Collision** |

Both occur in Draw's bounds-computation block, which executes **after** the Collision call within the same frame iteration.

## 4. Handler flow — Collision vs Draw ordering

In `FUN_140373EA0` (trade row handler):

| VA | Instruction | Operation |
|----|-------------|-----------|
| `0x140374265` | `CALL FUN_140445eb0` | Per-button panel setup (XMM1 = XMM6, advancing by pitch) |
| `0x1403742a4` | `CALL qword ptr [0x14086c850]` | **Collision** — reads `+0x673c`, `+0x6740`, `+0x6744` |
| `0x1403743a2` | `CALL qword ptr [0x14086c860]` | **Draw** — writes `+0x6740`, `+0x6744` |

**Collision is called before Draw.** Therefore, Draw's computed bounds are NOT yet applied when Collision runs for the trade row.

## 5. What can be established about +0x6740/+0x6744 at Collision time

### +0x6740 (bound Y1)

- **No static initialization** found in any C3DDLL64.dll constructor
- **Not written** by BeginDraw, SetColor, SetPosition, SetDimensions, SetRotation, or Collision
- **Only written** by: copy constructor (copies from another panel) or Draw (after Collision)
- **At Collision time:** value is whatever was left in memory — could be from a previous frame's Draw call, from a copy operation, or uninitialized garbage. **Cannot be determined statically.**

### +0x6744 (bound Y2)

- **Statically initialized** to ~1.0e9 (0x4E6E6B28) by both constructor/init functions
- **Not written** by BeginDraw, SetColor, SetPosition, SetDimensions, SetRotation, or Collision
- **Only written** by: constructors (~1.0e9) or Draw (after Collision, computed value)
- **At Collision time:** ~1.0e9 **IF** a constructor was called for panel `0x1409be060` at some point before this frame. But **no constructor call with RCX = 0x1409be060 was observed** in the host code (only setters, Collision, Draw). If no constructor ran, the value is unknown.

## 6. Remaining unknowns

| Unknown | Why unresolved |
|---------|---------------|
| Whether panel `0x1409be060` ever went through a constructor/init function | Only ~5 of ~70 `LEA RCX,[0x1409be060]` call sites verified; none were constructors |
| The actual runtime value of `+0x6740` at Collision time | No static init; no observed writer before Collision |
| Whether `FUN_140445eb0` (called at `0x140374265`, before Collision) sets panel bounds | Function in SOVIET64.exe at VA `0x140445eb0`; not in any existing export; Ghidra project for SOVIET64.exe not available locally |
| Whether Draw ran in a previous frame (which would set +0x6740/+0x6744 via computed values) | Frame ordering across frames not determinable from static analysis |

## 7. Raw evidence

All preserved under `export/almanac-evidence/c3ddll64-panel-init-trace-20261109/`:

| File | Description |
|------|-------------|
| `iat_names.txt` | ILT Hint/Name entries for IAT slots used with panel `0x1409be060` |
| `iat_slot_mapping.txt` | (attempted — output clearing issue) |
| `REPORT.md` | This report |

Related existing evidence:
- `export/almanac-evidence/c3ddll64-panel-fields-20261109/panel_field_writes_disasm.asm` — Ghidra disassembly of write locations
- `export/almanac-evidence/c3ddll64-collision-draw-20261109/collision_draw_disasm.asm` — Ghidra disassembly of Collision and Draw
- `export/140317db0_full_disasm.asm` — Host disassembly with panel object references

No commit or push performed for this step.
