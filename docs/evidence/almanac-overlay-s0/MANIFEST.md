# S0 Evidence Manifest — Almanac Overlay Static Graphics-Route Investigation

## Binary Identity
- Executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
  SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
- Engine DLL: C:\Workers Resources Soviet Republic\C3DDLL64.dll
  SHA256: 65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516

## Files

| File | Source | Description |
|------|--------|-------------|
| S0_FINDINGS.md | — | Concise findings report (5 questions answered) |
| c3d_import_d3d11.txt | ImportAt.java 0x18016B750 | IAT slot → D3D11CreateDeviceAndSwapChain (C3DDLL64.dll, hint 5) |
| c3d_init_caller.asm | DisasmRange.java 0x1800145C0-0x180014920 | Initialize function (creation caller) |
| c3d_init_xrefs.txt | XrefsTo.java 0x1800145C0 | Callers of Initialize function |

## Commands Used
- Get-FileHash (binary identity)
- Python PE import table parse (read-only)
- scratch\ghidra_run_c3d.bat (serialized Ghidra access, read-only)
- Select-String/Get-Content (source inspection)

## Scope
S0 only: static graphics-route investigation. No implementation, build, deployment, game execution, runtime instrumentation, or Ghidra mutation.
