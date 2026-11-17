# S0 Static Graphics-Route Investigation — Findings Report

## 1. Binary Identity

| File | Path | SHA256 |
|------|------|--------|
| Executable | `C:\Workers Resources Soviet Republic\SOVIET64.exe` | `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` |
| Engine DLL | `C:\Workers Resources Soviet Republic\C3DDLL64.dll` | `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516` |

## 2. Graphics Imports

### Executable (SOVIET64.exe)
| DLL | Import | Slot RVA | Hint |
|-----|--------|----------|------|
| dxgi.dll | CreateDXGIFactory1 | 0x86E0B0 | 3 |

**No D3D11 imports in the executable.** D3D11CreateDeviceAndSwapChain is NOT imported by SOVIET64.exe.

### Engine DLL (C3DDLL64.dll)
| DLL | Import | Slot RVA | Hint |
|-----|--------|----------|------|
| d3d11.dll | D3D11CreateDeviceAndSwapChain | 0x16B750 (VA 0x18016B750) | 5 |
| dxgi.dll | CreateDXGIFactory1 | 0x16B760 (VA 0x18016B760) | 3 |
| D3DCOMPILER_47.dll | D3DCompile | 0x16B020 | 1 |
| D3DCOMPILER_47.dll | D3DReflect | 0x16B028 | 22 |
| D3DCOMPILER_47.dll | D3DGetBlobPart | 0x16B030 | 13 |

**Conclusion:** Graphics creation route is in the engine DLL (C3DDLL64.dll), not the executable. The executable only imports CreateDXGIFactory1 from dxgi.dll.

## 3. Creation Caller

**Function:** `Initialize` at VA 0x1800145C0 in C3DDLL64.dll

**D3D11CreateDeviceAndSwapChain call site:** 0x1800148BC
- IAT slot: 0x18016B750 (QWORD = 0x1C91B4, thunk pointer)
- Arguments (from disassembly):
  - RCX = 0 (pAdapter)
  - RDX = [RSI] (DriverType, from feature level array)
  - R8D = 0 (Software)
  - R9D = 0 (Flags)
  - [RSP+0x20] = RAX (pFeatureLevels = 0x1801D76C0)
  - [RSP+0x28] = 3 (FeatureLevels count)
  - [RSP+0x30] = 7 (SDKVersion)
  - [RSP+0x38] = RAX (pSwapChain = [RBP-0x50])
  - [RSP+0x40] = RCX (pDevice = [RBP+0x180])
  - [RSP+0x48] = RAX (pImmediateContext = [RBP+0x190])
  - [RSP+0x50] = R13 (pFeatureLevel = [0x1801CDBE4])
  - [RSP+0x58] = R12 (pSwapChainDesc = [RBP-0x50]..[RBP-0x10])
- Return: EAX (HRESULT). JNS 0x1800148F5 on success.

**Swap-chain storage:** On success, `[0x1801D76C0]` holds the swap-chain pointer. The code then calls `[swapchain+0x128]` (a method on the swap chain) and checks for 0xB000/0xB100.

**Failure handling:** On failure (EAX < 0), EBX is incremented, RSI advances by 4, and the loop retries up to 3 times (CMP EBX,0x3; JC 0x180014873). After 3 failures, logs error and returns 1.

**Retained swap-chain storage:** `[0x1801D76C0]` (global variable in .data section). Whether this remains valid across device/window recreation is **not established** (static evidence only).

## 4. Startup Ordering

**Plugin loading sequence (from src/tesmioloader.cpp):**
1. `LoadLibraryExA(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)` — loads plugin DLL
2. `GetProcAddress(mod, TSM_EXPORT_APIVERSION)` — get version function
3. `GetProcAddress(mod, TSM_EXPORT_INIT)` — get init function
4. Version check (TSM_API_VERSION_MIN..TSM_API_VERSION)
5. `init(&g_host, &info)` — calls TsmPluginInit (plugin's init phase)
6. `GetProcAddress(mod, TSM_EXPORT_START)` — get start function
7. `StartPlugins()` — calls TsmPluginStart for all plugins (second phase)

**Graphics creation (Initialize at 0x1800145C0) is in C3DDLL64.dll**, which is the engine DLL loaded by the game executable. The plugin loader (tesmioloader) loads plugins from `plugins\*.dll`. The engine DLL is loaded by the game executable itself, not by the plugin loader.

**Ordering relative to plugin Init/Start:** The engine's Initialize function (which calls D3D11CreateDeviceAndSwapChain) is called through a function pointer table (data references at 0x1801AAA68, 0x1801DF8E8, 0x180196D38, 0x180196D70, 0x180187AA8). The exact ordering relative to plugin Init/Start is **not established by static evidence**. The engine DLL is loaded by the game executable before plugins are loaded, but the exact timing of Initialize relative to plugin loading is unknown.

**Deferred to D0:** Plugin-start thread, creation-entry/exit thread, and ordering relative to graphics creation require runtime diagnostics.

## 5. patchIat Semantics

**Implementation (src/tesmioloader.cpp, PatchIat function):**

```cpp
static bool PatchIat(HMODULE mod, const char* dll, const char* fn,
                     void* repl, void** origOut, const char* label)
{
    void** slot = FindIatSlot(mod, dll, fn);
    if (!slot) { Logf("hook FAILED  %-22s (no import slot)", label); return false; }

    DWORD prot = 0;
    if (!VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &prot))
    { Logf("hook FAILED  %-22s (VirtualProtect %lu)", label, GetLastError()); return false; }

    *origOut = *slot;       // Save current target (may be another mod's detour)
    *slot = repl;           // Write replacement
    VirtualProtect(slot, sizeof(void*), prot, &prot);  // Restore protection
    Logf("hook ok      %-22s orig=%p", label, *origOut);
    return true;
}
```

**Semantics:**
1. **Preservation of existing target:** `*origOut = *slot` saves whatever is currently in the slot. If another mod already patched it, `origOut` points to the other mod's detour, not the original function. This is implicit chaining.
2. **Slot ownership checks:** None. No check for whether the slot is already patched by another mod.
3. **Chaining/unpatch support:** Chaining is implicit (saves current target). No explicit unpatch support (no restore function).
4. **Partial failure:** If VirtualProtect fails, returns false without modifying the slot. If write succeeds but VirtualProtect restore fails, slot is modified but protection not restored (minor issue).
5. **DLL residency:** No effect. IAT slot is in the executable's/engine's import table, not in the DLL itself.

**Distinguish source semantics from runtime slot owners:**
- Source semantics: PatchIat always overwrites the slot, saving the previous value. It does not check ownership.
- Runtime slot owners: Unknown. Multiple mods may patch the same slot. The order of patching determines the chain. No ownership tracking exists.

## Summary Table

| Verified Fact | Evidence | Unresolved Runtime Question | Smallest D0 Observation Needed |
|---|---|---|---|
| Executable SHA256 matches | Get-FileHash | None | None |
| Engine DLL SHA256 = 65FCB684... | Get-FileHash | None | None |
| Executable imports CreateDXGIFactory1 from dxgi.dll (slot 0x86E0B0) | PE import table parse | None | None |
| Engine imports D3D11CreateDeviceAndSwapChain from d3d11.dll (slot 0x18016B750) | PE import table parse + Ghidra ImportAt | None | None |
| Engine imports CreateDXGIFactory1 from dxgi.dll (slot 0x18016B760) | PE import table parse | None | None |
| D3D11CreateDeviceAndSwapChain called in Initialize@0x1800145C0 | Ghidra xref + disassembly | Thread identity, timing | D0: capture thread ID at creation entry/exit |
| Swap chain stored at [0x1801D76C0] | Disassembly at 0x1800148F5 | Whether pointer remains valid across recreation | D0: capture swap-chain identity |
| Plugin Init/Start sequence | src/tesmioloader.cpp source | Ordering relative to graphics creation | D0: capture plugin-start thread and creation-entry thread |
| patchIat overwrites slot, saves previous value | src/tesmioloader.cpp source | Runtime slot owners, chain order | D0: capture slot owner before/after patch |
| patchIat has no ownership check | src/tesmioloader.cpp source | Whether multiple mods patch same slot | D0: capture module owners of slot |
| patchIat has no unpatch support | src/tesmioloader.cpp source | Whether unpatch is needed | D0: not applicable (no unpatch API) |

## Commands Used

```bash
# Binary identity
Get-FileHash "C:\Workers Resources Soviet Republic\SOVIET64.exe" -Algorithm SHA256
Get-FileHash "C:\Workers Resources Soviet Republic\C3DDLL64.dll" -Algorithm SHA256

# Import table parsing (Python, read-only)
python -c "..." # PE import table parse for both binaries

# Ghidra exports (read-only, serialized)
scratch\ghidra_run_c3d.bat ImportAt.java scratch\out\c3d_import_d3d11.txt 0x18016B750
scratch\ghidra_run_c3d.bat DisasmRange.java scratch\out\c3d_init_caller.asm 0x1800145C0 0x180014920
scratch\ghidra_run_c3d.bat XrefsTo.java scratch\out\c3d_init_xrefs.txt 0x1800145C0

# Source inspection
Select-String -Path src\tesmioloader.cpp -Pattern "patchIat|PatchIat"
Get-Content src\tesmioloader.cpp | Select-Object -Skip 190 -First 60
Get-Content src\tesmioloader.cpp | Select-Object -Skip 1380 -First 100
```

## Independent Review
Independent read-only reviewer was unavailable for this session. No review gate was claimed.
