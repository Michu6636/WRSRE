# Almanac Launcher Investigation — Evidence Manifest

## Executable Identity
- Path: C:\Workers Resources Soviet Republic\SOVIET64.exe
- SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
- Ghidra project: scratch/ghidra_proj_analysis (analysis.gpr)

## Files

### Import Resolution (raw PE import table)
| File | Source | Description |
|------|--------|-------------|
| import_14086d2a8.txt | ImportAt.java 0x14086D2A8 | IAT slot → PlaySound2D (C3DDLL64.dll, hint 1587) |
| import_14086c008.txt | ImportAt.java 0x14086C008 | IAT slot → C3DVECTOR3 constructor (hint 60) |
| import_14086c8a0.txt | ImportAt.java 0x14086C8A0 | IAT slot → SetPosition (hint 1984) |
| import_14086c8a8.txt | ImportAt.java 0x14086C8A8 | IAT slot → SetDimensions (hint 1893) |
| import_14086c8b8.txt | ImportAt.java 0x14086C8B8 | IAT slot → GetMouseLeftPress (hint 1267) |
| import_14086d220.txt | ImportAt.java 0x14086D220 | IAT slot → DrawFromNew3PatchVertical (hint 886) |

### Layout Disassembly
| File | Source | Description |
|------|--------|-------------|
| layout_preheader.asm | DisasmRange.java 0x14031F682-0x14031F780 | Tab layout preheader (SetPosition, SetDimensions, XMM14/XMM13 setup) |
| region_2031c.asm | DisasmRange.java 0x14032031C-0x140320360 | Internal branch target region |

### Raw Bytes
| File | Source | Description |
|------|--------|-------------|
| bytes_204eb.txt | DumpBytes.java 0x1403204EB-0x1403204F2 | Raw bytes at post-loop exit (RIP-relative MOVSS) |
| bytes_90aab4.txt | DumpBytes.java 0x14090AAB4-0x14090AAB7 | Raw bytes for float 68.0 |
| bytes_90a6a8.txt | DumpBytes.java 0x14090A6A8-0x14090A6AB | Raw bytes for float 5.0 |
| bytes_90aa28.txt | DumpBytes.java 0x14090AA28-0x14090AA2B | Raw bytes for float 50.0 |

### Float Constants
| File | Source | Description |
|------|--------|-------------|
| floats_90aab4.txt | ReadFloats.java 0x14090AAB4 0x14090AAB8 | 68.0, 70.0 |
| floats_90a6a8.txt | ReadFloats.java 0x14090A6A8 0x14090A6AC | 5.0, 5.06 |
| floats_90aa28.txt | ReadFloats.java 0x14090AA28 0x14090AA2C | 50.0, 51.0 |
| float_90a9cc.txt | ReadFloats.java 0x14090A9CC 0x14090A9D0 | 35.0, 35.4 |
| float_90a960.txt | ReadFloats.java 0x14090A960 0x14090A964 | 24.0, 24.987 |

## Commands Used
All exports generated via `scratch\ghidra_run_analysis.bat <Script>.java <output> <args>` using Ghidra 12.1.4 headless against `scratch\ghidra_proj_analysis`.

## Corrected Findings

### Errors in Previous Report
1. **0x14031F64E is a direct incoming edge to 0x1403204EB** — confirmed. Empty vector path bypasses loop.
2. **Bytes at 0x1403204EB are `F3 0F 10 0D 95 1B 67 00`** — corrected from `95 91`. RIP-relative MOVSS XMM1,[0x140992088].
3. **XMM1=DPI is the output of that instruction**, not its entry state.
4. **Decrement at 0x14031F9CD is conditional** — only executes when [R14+0x1377C] > 0.
5. **"Post-loop: no restoration needed" is unsupported** — next section consumes live registers and shared panel state.
6. **game+0xFC3 is a condition in this path** — general modal semantics unverified.

### Whole-Function Interception
Requires relocating only the stolen span (18 bytes), not the entire function. The 18-byte prologue is all position-independent (PUSH + LEA RBP,[RSP-0x2B30]), so no relocation is needed for the prologue itself. However, any hook at 0x1403204EB requires relocating the RIP-relative MOVSS instruction.

### Caller Ignoring EAX
Does not prove void return. The caller may simply not use the return value. ABI is not established.

### Separate Hit Detection
Does not establish click ownership. GetMouseLeftPress consumption is unverified. The native click handling is inside the tab loop and updates [game+0xC650]. A separate launcher would need its own click detection, but whether this prevents world-action dispatch is not traced.

### Shared-State Restoration
Unresolved. The next section at 0x1403204EB consumes XMM1 (DPI), XMM8 (35.0), XMM9 (50.0), XMM11 (0.0), XMM14 (DPI*35.0), XMM13 (DPI*24.0), R14 (game), RBP (frame), RSI (0), R15D (count). Any helper call must preserve these.

### Alternate-Path Register State
Unresolved. The empty-vector path (0x14031F64E→0x1403204EB) does not set XMM9, XMM8, XMM10, or XMM6. These retain prior values. XMM14 and XMM13 ARE set (before the empty check). R15D=0 on this path.

## Remaining Evidence Gaps

1. **Layout bounds:** No bounds check visible. Whether a 9th button at Y=DPI*(count*68+74) overlaps existing UI is unverified.
2. **Click ownership:** GetMouseLeftPress consumption and world-action dispatch prevention not traced.
3. **Modal semantics:** [game+0xFC3] writer not identified. General modal behavior unverified.
4. **Rect store provenance:** 0x9BE2F0/0x9BE2F4 not written by FUN_140317db0. Whether C3D_PANEL2D::Draw writes internally is unverified.
5. **Native entry count assumption:** Whether native selection/dispatch assumes exactly 8 entries is unverified.
6. **Empty-vector path register state:** XMM9/XMM8/XMM10/XMM6 not set on this path. Their values are unknown.
7. **Hook helper compatibility:** Current 14-byte helper does not relocate RIP-relative instructions. A suitable relocator is needed for 0x1403204EB.

## Recommendation
No safe launcher boundary is established. The missing prerequisite is a verified layout bounds check and input ownership mechanism. Do not implement or arm any hook.
