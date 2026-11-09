# PE import table raw evidence manifest

Source executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
Executable SHA-256: 296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8
Image base: 0x140000000 (PE32+ / 64-bit)
All file offsets are raw byte offsets within SOVIET64.exe; all RVAs are image-relative virtual addresses.

## 1. Import descriptor — C3DDLL64.dll (descriptor #0)

| File | RVA | File offset | Size | Contents |
|------|-----|-------------|------|----------|
| `import_descriptor_c3ddll64_rva_0x984b3c.raw` | 0x00984B3C | 0x983B3C | 20 bytes | One IMAGE_IMPORT_DESCRIPTOR |

Descriptor fields (little-endian):
- OriginalFirstThunk (ILT RVA): 0x00984CF8
- TimeDateStamp: 0x00000000
- ForwarderChain: 0x00000000
- Name RVA: 0x0098FBD2
- FirstThunk (IAT RVA): 0x0086C000

## 2. DLL name — C3DDLL64.dll

| File | RVA | File offset | Size | Contents |
|------|-----|-------------|------|----------|
| `dll_name_c3ddll64_rva_0x98fbd2.raw` | 0x0098FBD2 | 0x98EBD2 | 13 bytes | `C3DDLL64.dll\0` |

## 3. ILT entries (OriginalFirstThunk table)

| File | RVA | File offset | Size | Value (u64 LE) | Import name |
|------|-----|-------------|------|----------------|-------------|
| `ilt_entry_rva_0x00985548.raw` | 0x00985548 | 0x984548 | 8 bytes | 0x000000000098A302 | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` |
| `ilt_entry_rva_0x00985558.raw` | 0x00985558 | 0x984558 | 8 bytes | 0x000000000098A36C | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` |
| `ilt_entry_rva_0x00985560.raw` | 0x00985560 | 0x984560 | 8 bytes | 0x000000000098A390 | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` |

## 4. IAT entries (FirstThunk table)

| File | RVA | File offset | Size | Value (u64 LE) | Import name |
|------|-----|-------------|------|----------------|-------------|
| `iat_entry_rva_0x0086c850.raw` | 0x0086C850 | 0x86B850 | 8 bytes | 0x000000000098A302 | `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` |
| `iat_entry_rva_0x0086c860.raw` | 0x0086C860 | 0x86B860 | 8 bytes | 0x000000000098A36C | `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` |
| `iat_entry_rva_0x0086c868.raw` | 0x0086C868 | 0x86B868 | 8 bytes | 0x000000000098A390 | `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` |

## Correspondence

For all three slots, the IAT entry value equals the ILT entry value (both are the Hint/Name RVA). The IAT entries are unbound on disk — at runtime the Windows loader would resolve each by loading C3DDLL64.dll and writing the actual exported function address into the IAT slot.

The three slots are consecutive (indices 266, 268, 269 within C3DDLL64.dll's IAT at RVA 0x0086C000).

## Scope note

No Ghidra decompilation of target functions was performed. No runtime inspection, hooks, or game changes were used. All evidence is from static PE header parsing of SOVIET64.exe on disk.
