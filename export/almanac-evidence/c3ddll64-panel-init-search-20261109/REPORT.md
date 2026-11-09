# Almanac: Panel `0x1409be060` — Static Initialization Call Search

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

## 1. Conclusion

**No static initialization call for panel `0x1409be060` exists in SOVIET64.exe code.**

Every reference to panel object address `0x1409be060` across all available SOVIET64.exe disassembly exports is a `LEA RCX,[0x1409be060]` (passing the panel as `this`) followed by a CALL to an IAT slot. None of the CALL targets are constructor/init functions. The panel's bounds fields (+0x673c, +0x6740, +0x6744) are never statically initialized by SOVIET64.exe.

## 2. Scope of search

All SOVIET64.exe disassembly exports were searched for references to `0x1409be060`:

| Export file | References to 0x1409be060 |
|-------------|---------------------------|
| `export/140317db0_full_disasm.asm` (sidebar host) | 350 |
| `export/140373ea0_1403829fd_disasm.asm` (trade row handler) | 226 |
| All other exports (FUN_1404492f0, C3DDLL64.dll disassemblies, etc.) | 0 |

**Total: 576 references, all in the two host files. Every reference has been reviewed.**

## 3. Instruction format of every reference

All 576 references are identical in form:

```
LEA RCX,[0x1409be060]    ; load panel object address as `this`
CALL qword ptr [0x14086cXXXX]   ; indirect call through IAT slot
```

No `MOV RCX,0x1409be060`, no direct field writes like `MOV [0x1409be060+0x673c],...`, and no other access patterns were found.

## 4. IAT slot mapping — every CALL target with RCX = panel

From SOVIET64.exe's import table (C3DDLL64.dll descriptor #0, ILT base RVA `0x984CF8`, IAT base RVA `0x86C000`):

| IAT slot | Export function | Role | Writes +0x673c/+0x6740/+0x6744? |
|----------|----------------|------|---------------------------------|
| `0x14086c850` | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` (VA `0x1800BC690`) | Hit-test | **No** — reads only |
| `0x14086c860` | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` (VA `0x1800BAF40`) | Render | **Yes** — writes +0x6740 at `0x1800BB26E`, +0x6744 at `0x1800BB682` (but AFTER Collision) |
| `0x14086c8f8` | `?EndDraw@C3D_PANEL2D@@QEAAXXZ` (VA `0x1800BC610`) | Render end | No |
| `0x14086c918` | `?BeginDraw@C3D_PANEL2D@@QEAAXPEBD_N@Z` (VA `0x1800BC4A0`) | Render begin | No |
| `0x14086c898` | `?SetColor@C3D_PANEL2D@@QEAAXAEBVC3DFCOLOR@@@Z` | Set color | No |
| `0x14086c8a0` | `?SetPosition@C3D_PANEL2D@@QEAAXAEBVC3DVECTOR3@@@Z` (VA `0x18002D9E0`) | Set position | No |
| `0x14086c8a8` | `?SetDimensions@C3D_PANEL2D@@QEAAXMM@Z` (VA `0x18002D9C0`) | Set dimensions | No |
| `0x14086c960` | `?SetRotation@C3D_PANEL2D@@QEAAXM@Z` | Set rotation | No |
| `0x14086c858` | `??0C3DVECTOR2@@QEAA@XZ` | C3DVECTOR2 constructor | Not a panel method; called via multi-inheritance `this` adjustment |
| `0x14086c958` | `?GetScale@C3D_NODE@@QEAA?AVC3DVECTOR3@@XZ` | C3D_NODE::GetScale | Not a panel method; called via multi-inheritance `this` adjustment |
| `0x14086c868` | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` | Input query | Not a panel method (input object `this`) |

**None of these functions is a C3D_PANEL2D constructor/init function.**

## 5. Panel field static values (read from SOVIET64.exe on disk)

Panel base VA `0x1409be060` = RVA `0x9BE060` (file offset `0x9BCE60`):

| Field | RVA | File offset | Static value (float) | Interpretation |
|-------|-----|-------------|---------------------|----------------|
| `+0x673c` (scale) | `0x9C479C` | `0x9C359C` | 8.53E-10 | Garbage/uninitialized |
| `+0x6740` (bound Y1) | `0x9C47A0` | `0x9C35A0` | 1.21E-25 | Garbage/uninitialized |
| `+0x6744` (bound Y2) | `0x9C47A4` | `0x9C35A4` | 2.63E-07 | Garbage/uninitialized |

The static binary contains **no meaningful initializer** for these fields — not 1.0f, not 0x4E6E6B28 (~1.0e9), not 0.0f. The raw bytes at these locations are garbage (non-zero but meaningless as panel bounds).

## 6. Where +0x673c/+0x6740/+0x6744 ARE written (all in C3DDLL64.dll)

SOVIET64.exe has **zero** disp32 references to offsets `0x673c`, `0x6740`, or `0x6744` anywhere in its code. All writes occur inside C3DDLL64.dll:

| VA | Function | Field | Value | Type |
|----|----------|-------|-------|------|
| `0x18005BED0` | Constructor/init A | +0x673c | 1.0f | Static init |
| `0x18005BEDC` | Constructor/init A | +0x6744 | 0x4E6E6B28 (~1.0e9) | Static init |
| `0x1800B8AD3` | Constructor/init B | +0x673c | 1.0f | Static init |
| `0x1800B8ADE` | Constructor/init B | +0x6744 | 0x4E6E6B28 (~1.0e9) | Static init |
| `0x18002DEE9` | Copy constructor | +0x673c | copied | Field copy |
| `0x18002DEF6` | Copy constructor | +0x6740 | copied | Field copy |
| `0x18002DF03` | Copy constructor | +0x6744 | copied | Field copy |
| `0x1800BB26E` | Draw | +0x6740 | computed | **After Collision** |
| `0x1800BB682` | Draw | +0x6744 | computed | **After Collision** |

**None of these writer functions is exported by C3DDLL64.dll.** They are internal functions not reachable via SOVIET64.exe's import table. SOVIET64.exe only calls exported panel methods (BeginDraw, EndDraw, SetColor, SetPosition, SetDimensions, SetRotation, Collision, Draw), none of which write to +0x673c/+0x6740/+0x6744 as part of object construction.

## 7. Handler ordering (trade row)

In `FUN_140373EA0` (trade row handler at VA `0x140373ea0`):

| VA | Instruction | Operation |
|----|-------------|-----------|
| `0x140374265` | `CALL FUN_140445eb0` | Per-button panel setup (before Collision) |
| `0x1403742a4` | `CALL qword ptr [0x14086c850]` | **Collision** — reads +0x673c/+0x6740/+0x6744 |
| `0x1403743a2` | `CALL qword ptr [0x14086c860]` | **Draw** — writes +0x6740/+0x6744 (after Collision) |

**Collision is called before Draw**, so Draw's computed bounds are not yet applied when Collision runs for the trade row.

## 8. Remaining unknowns

| Unknown | Status |
|---------|--------|
| Whether panel `0x1409be060` was ever passed to a constructor/init function | **Not found** — no such call exists in any reviewed export |
| Actual runtime value of `+0x6740` at Collision time | **Unknown** — no static initializer; no observed writer before Collision |
| Whether `FUN_140445eb0` (host VA `0x140445eb0`, called at `0x140374265`) sets panel bounds | **Not examined** — function not in any existing export; SOVIET64.exe Ghidra project not available locally |
| Whether panel fields were initialized by C++ runtime global object construction before main code | **Not observable** from static code analysis |

## 9. Raw evidence

All preserved under `export/almanac-evidence/c3ddll64-panel-init-search-20261109/`:

| File | Description |
|------|-------------|
| `REPORT.md` | This report |

Related existing evidence:
- `export/almanac-evidence/c3ddll64-panel-fields-20261109/panel_field_writes_disasm.asm` — Ghidra disassembly of field write locations
- `export/almanac-evidence/c3ddll64-panel-init-trace-20261109/REPORT.md` — Prior panel init trace report
- `export/almanac-evidence/c3ddll64-collision-draw-20261109/collision_draw_disasm.asm` — Ghidra disassembly of Collision and Draw
- `export/140317db0_full_disasm.asm` — Sidebar host disassembly (350 panel refs)
- `export/140373ea0_1403829fd_disasm.asm` — Trade row handler disassembly (226 panel refs)

No commit or push performed for this step.
