# Almanac: PE import table analysis — slots 0x14086c850, 0x14086c860, 0x14086c868

Static analysis only. No hooks, debugger, game changes, commit, or push during investigation.

Source: Original executable `C:\Workers Resources Soviet Republic\SOVIET64.exe` (SHA-256: 296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8), image base 0x140000000, PE32+ (64-bit). PE headers parsed directly from the file on disk; no Ghidra decompilation of target functions performed.

Raw evidence: `export/almanac-evidence/pe-import-table-20261109/` — one raw hex dump per target RVA plus this report.

## 1. PE import descriptor covering the slots

| Field | Value |
|-------|-------|
| Import Directory RVA | 0x00984B3C (file offset 0x983B3C) |
| Import Directory size | 0x1B8 (~21 descriptors + null terminator) |
| Descriptor index | #0 |
| DLL name RVA | 0x0098FBD2 |
| DLL name | `C3DDLL64.dll` |
| OriginalFirstThunk (ILT) RVA | 0x00984CF8 |
| FirstThunk (IAT) RVA | 0x0086C000 |

**All three slots fall within C3DDLL64.dll's IAT**, which begins at RVA 0x86C000. The next descriptor's IAT (steam_api64.dll) begins at RVA 0x86E0D8, so C3DDLL64.dll's IAT spans 0x86C000–0x86E0D8.

| Slot | RVA | IAT entry index | IAT file offset |
|------|-----|-----------------|-----------------|
| 0x14086c850 | 0x86c850 | 266 | 0x86b850 |
| 0x14086c860 | 0x86c860 | 268 | 0x86b860 |
| 0x14086c868 | 0x86c868 | 269 | 0x86b868 |

Index = (slot_RVA − 0x86c000) / 8 (8-byte entries in PE32+).

## 2. Raw bytes at RVAs 0x98a302, 0x98a36c, 0x98a390 (Hint/Name entries)

These are the values stored in the three IAT slots. Each is an RVA into a Hint/Name table entry in .rdata. Format: 2-byte hint (little-endian u16) followed by null-terminated ASCII name.

### RVA 0x98a302 (file offset 0x989302)

| Field | Value |
|-------|-------|
| Hint | 0x029B |
| Name | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` |
| Name length | 47 bytes |
| Total entry size | 50 bytes (2 hint + 47 name + 1 null) |
| Raw bytes | `9B 02 3F 43 6F 6C 6C 69 73 69 6F 6E 40 43 33 44 5F 50 41 4E 45 4C 32 44 40 40 51 45 41 41 5F 4E 56 43 33 44 56 45 43 54 4F 52 33 40 40 4D 4D 40 5A 00` |

### RVA 0x98a36c (file offset 0x98936c)

| Field | Value |
|-------|-------|
| Hint | 0x036E |
| Name | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` |
| Name length | 33 bytes |
| Total entry size | 36 bytes (2 hint + 33 name + 1 null) |
| Raw bytes | `6E 03 3F 44 72 61 77 40 43 33 44 5F 50 41 4E 45 4C 32 44 40 40 51 45 41 41 58 4D 4D 4D 4D 4D 5F 4E 40 5A 00` |

### RVA 0x98a390 (file offset 0x989390)

| Field | Value |
|-------|-------|
| Hint | 0x04FC |
| Name | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` |
| Name length | 47 bytes |
| Total entry size | 50 bytes (2 hint + 47 name + 1 null) |
| Raw bytes | `FC 04 3F 47 65 74 4D 6F 75 73 65 53 6F 6C 69 64 40 43 33 44 5F 49 4E 50 55 54 40 40 51 45 41 41 3F 41 56 43 33 44 56 45 43 54 4F 52 33 40 40 58 5A 00` |

## 3. Correspondence between each slot and its import name

| Slot RVA | IAT index | ILT entry RVA | ILT value (Hint/Name RVA) | Import name from C3DDLL64.dll |
|----------|-----------|---------------|---------------------------|-------------------------------|
| 0x86c850 | 266 | 0x985548 | 0x98a302 | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` |
| 0x86c860 | 268 | 0x985558 | 0x98a36c | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` |
| 0x86c868 | 269 | 0x985560 | 0x98a390 | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` |

For all three slots, **IAT entry value == ILT entry value** (both contain the Hint/Name RVA). This means the IAT entries on disk are **unbound** — they contain the same hint/name pointer as the ILT, not a resolved function address.

## 4. Static image vs actual runtime binding

**What the static image establishes:**
- All three slots are genuine PE import table entries (IAT entries) in C3DDLL64.dll's import table.
- Each slot has a well-defined Hint/Name entry naming a specific C3DDLL64.dll export.
- The mapping is positional: IAT entry N corresponds to ILT entry N for the same descriptor.
- The DLL is `C3DDLL64.dll`, not `USER32.dll` or any system DLL.

**What the static image does NOT establish (runtime binding):**
- The IAT entries are unbound on disk (IAT value == ILT value == Hint/Name RVA). At runtime, the Windows loader would resolve each entry by loading `C3DDLL64.dll`, looking up the exported function by name/hint, and writing the actual function address into the IAT slot.
- The actual runtime target address of each slot depends on which version of `C3DDLL64.dll` is loaded and where it is mapped. This cannot be determined from the PE file alone.
- Ghidra's external manager reported `<none>` at all three slots — Ghidra's import analysis did not record an external location for them. This may be because they are unbound or because Ghidra's import resolution did not fully process this table. The Ghidra finding (no external location) is consistent with the static PE evidence (unbound IAT entries), but it is not a substitute for the actual runtime binding.

**Demangled names (for reference, not from the binary):**
- `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` → `C3D_PANEL2D::Collision(C3DVECTOR3, float, float)` (returns bool)
- `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` → `C3D_PANEL2D::Draw(float, float, float, float, float, bool)` (returns void)
- `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` → `C3D_INPUT::GetMouseSolid()` (returns C3DVECTOR3)

These demanglings are inferred from the mangled names; the actual function signatures and behavior require the C3DDLL64.dll body, which is not available from the static PE file.

## Raw evidence

Raw hex dumps preserved under `export/almanac-evidence/pe-import-table-20261109/`:
- `slot_14086c850_rva_0x98a302.raw` — Hint/Name entry for ?Collision@C3D_PANEL2D...
- `slot_14086c860_rva_0x98a36c.raw` — Hint/Name entry for ?Draw@C3D_PANEL2D...
- `slot_14086c868_rva_0x98a390.raw` — Hint/Name entry for ?GetMouseSolid@C3D_INPUT...

No commit or push performed for this step.
