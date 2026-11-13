# Almanac: IAT slot resolution — 0x14086c868, 0x14086c850, 0x14086c860

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

Source: `ImportsAt.java` headless Ghidra queries, serialized (one at a time).
Output: `export/almanac-evidence/iat-slots-20261109/slot_14086c868.txt`, `slot_14086c850.txt`, `slot_14086c860.txt`.

## Key finding: none of the three slots is a DLL import

Ghidra's external manager reports **no external location at any of the three slots**. All three contain internal RVAs pointing into the image itself (image base 0x140000000), not imported DLL function addresses. The program does contain many mouse-related external symbols (GetMouseSolid, GetMouseRelative, etc.) but none are attached to these specific slots.

## Per-slot results

### Slot 0x14086c868

| Field | Value |
|-------|-------|
| Slot contents (LE u64 at 0x14086c868) | RVA 0x0098A390 → VA 0x14098A390 |
| External location at slot | <none> |
| External symbols containing "mouse" | GetMouseSolid, GetMouseRelative, GetMouseLeftPress*, etc. (unattached) |
| Referenced by | `[READ]` from 20+ sites (0x14009DDE9, 0x14009DE39, etc.) |

Used as indirect call target at `0x140373f80` (`CALL qword ptr [0x14086c868]`) with `RCX=[0x140a54b90]` (C3D_INPUT object).

### Slot 0x14086c850

| Field | Value |
|-------|-------|
| Slot contents (LE u64 at 0x14086c850) | RVA 0x0098A302 → VA 0x14098A302 |
| External location at slot | <none> |
| External symbols containing "mouse" | Same list (unattached) |
| Referenced by | `[READ]` from 20+ sites (0x1400A9813, 0x1400CA439, etc.) |

Used as indirect call target at `0x1403742a4` (`CALL qword ptr [0x14086c850]`) — panel collision context.

### Slot 0x14086c860

| Field | Value |
|-------|-------|
| Slot contents (LE u64 at 0x14086c860) | RVA 0x0098A36C → VA 0x14098A36C |
| External location at slot | <none> |
| External symbols containing "mouse" | Same list (unattached) |
| Referenced by | `[READ]` from 20+ sites (0x1400A961B, 0x1400A9CB7, etc.) |

Used as indirect call target at `0x1403743a2` (`CALL qword ptr [0x14086c860]`) — panel draw context.

## Distinctions

| Concept | Status |
|---------|--------|
| Recorded import name | None — Ghidra external manager has no record for any of the three slots |
| Actual runtime target | Internal VA from RVA stored in the slot (0x14098A390 / 0x14098A302 / 0x14098A36C) — these are function pointers within SOVIET64.exe itself, not DLL imports |
| Behavior visible without DLL body | Not applicable — no DLL body involved; the targets are internal functions whose bodies would need separate Ghidra decompilation |

## What this corrects

These slots are **not** IAT slots. They are entries in an internal function pointer table at addresses around 0x14086c848–0x14086c888. The assumption that they resolve to DLL imports (e.g., `C3D_INPUT::GetMouseSolid`, `C3D_PANEL2D::Collision`, `C3D_PANEL2D::Draw`) is **not supported** by Ghidra's import analysis. They may be internal thunks/wrappers that call DLL functions, or they may be entirely internal implementations. The actual behavior can only be determined by decompiling the target functions at 0x14098A390, 0x14098A302, and 0x14098A36C respectively.

## Raw evidence

All three raw output files preserved under `export/almanac-evidence/iat-slots-20261109/`.
