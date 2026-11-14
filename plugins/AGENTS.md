# Plugins agent instructions

- Plugin isolation: nothing may be freed across the loader/plugin boundary. `/MT` static CRT is required (see `build.bat`).
- Hooks: 14-byte whole-instruction spans + `expect` bytes + RIP-relative/branch relocation. Inspect whole-instruction boundaries, incoming targets, live state, cleanup, both paths.
- Hot paths (render/tick): zero-alloc. Use `ReadablePtr` for protection; it does not prove ABI/semantic safety. SEH does not handle normal failure returns.
- UI: flow UI + DPI `0x992088`. Custom text IDs >= 1,000,000. `.ini` files UTF-8 no-BOM.
- Writes: small coherent chunks, verify each before continuing. Never replace a long file for a short edit.
- Build: `cmd /c build.bat <plugin>` from workspace root (PowerShell 5.1: no `&&`). Verify DLL `LastWriteTime` under `build\plugins`.
