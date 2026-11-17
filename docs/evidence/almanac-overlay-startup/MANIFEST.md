# Almanac Overlay Startup Evidence Manifest

## Target Binaries and Identity Verification

Verified with `certutil -hashfile <path> SHA256`:

- **SOVIET64.exe**:
  - Path: `C:\Workers Resources Soviet Republic\SOVIET64.exe`
  - SHA256: `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8`
  - Target build: v1.1.1.9, PE timestamp `0x6A3EB6AD`
  - Preferred Image Base: `0x140000000`
  - AddressOfEntryPoint: RVA `0x84eb58` (VA `0x14084eb58`)

- **C3DDLL64.dll**:
  - Path: `C:\Workers Resources Soviet Republic\C3DDLL64.dll`
  - SHA256: `65fcb6845c3cb7c25a22121f25904a855ea88657e31a3b50a15adac26e231516`
  - Preferred Image Base: `0x180000000`

## Scope
Read-only static investigation of startup ordering, loader mechanism, engine initialize caller route, and game HWND origin. No builds, game execution, deployment, hooks, or Ghidra database mutation.

## Evidence Files in Packet

| File | Type / Tool | Description |
|---|---|---|
| `MANIFEST.md` | Documentation | Binary identities, environment, tool commands, file inventory |
| `loader_injection_evidence.txt` | Source excerpt | Source code proof of tesmioloader remote thread injection and DllMain lock |
| `engine_init_call_chain.asm` | Static disassembly (Capstone) | Traced route from SOVIET64 CRT startup / WinMain to C3DDLL64 Initialize and D3D11 creation |
| `game_hwnd_provenance.asm` | Static disassembly (Capstone) | CreateWindowExW site, storage to global `0x1409941f8`, passing to engine and swap chain descriptor |
| `STARTUP_ORDERING_ANALYSIS.md` | Technical Analysis | Structured findings, partial ordering table, observation vs interpretation, contract satisfaction verdict |

## Commands and Inspection Tools Used
- `certutil -hashfile <path> SHA256` for SHA256 authentication of binaries
- Read-only Python 3 script using `capstone` (x86-64 disassembly) and standard `struct` for PE header parsing, import parsing, export parsing, and `.pdata` function boundary resolution
- `git status`, `git show`, and file reading tools across `src/tesmioloader.cpp`, `src/tesmiolauncher.cpp`, `prototypes/almanac_overlay/native/graphics_adapter.{h,cpp}`
