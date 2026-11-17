# Almanac Overlay Startup Ordering & Contract Investigation

## 1. Executive Summary

This report statically analyzes the startup sequence of Workers & Resources: Soviet Republic (`SOVIET64.exe` v1.1.1.9) and its 3D rendering engine (`C3DDLL64.dll`) to answer two concrete architectural questions for the Earthworks-style ImGui Almanac overlay:
1. **Can the loader install its graphics-creation observer before the game's D3D11 device and swap chain are created?**
2. **Can a verified game HWND be available at that point under the current installer contract?**

### Key Findings
1. **Graphics Observer Timing:** **YES.** The loader (`tesmioloader.dll`) and all plugins (`Init` and `Start`) execute during suspended process creation on a remote thread created by `tesmiolauncher.exe` before the game's main thread is released. The game's D3D11 device and swap chain are created much later on the game's main thread during `WinMain`. Therefore, an IAT hook on `C3DDLL64.dll`'s `d3d11.dll!D3D11CreateDeviceAndSwapChain` slot installed during plugin `Start` is in place well **before** D3D11 creation occurs.
2. **Pre-existing HWND Availability:** **NO.** Because plugin `Start` runs during suspended process initialization before the game main thread executes a single instruction, the game window has not been created yet (`CreateWindowExW` occurs in `WinMain` at RVA `0x104070`). Consequently, passing a valid, non-null, verified `HWND` to `InstallGraphicsAdapter(..., verifiedGameWindow, ...)` during plugin `Start` **cannot be satisfied**.
3. **Verified HWND Origin:** The HWND used in the swap-chain descriptor is created via `USER32.dll!CreateWindowExW` in `SOVIET64.exe` at RVA `0x104070` and stored in global variable `0x1409941f8`. It is later copied into `C3DAPI_D3D11_DEVICE+0x30` at RVA `0x10423d`, and then into `DXGI_SWAP_CHAIN_DESC.OutputWindow` at `C3DDLL64.dll` RVA `0x1483b`.

---

## 2. Loader Call Chain & Loading Mechanism

### Source Chain
In `src/tesmioloader.cpp`:
- `DllMain` (`DLL_PROCESS_ATTACH`) calls `Init()` under Windows loader lock.
- `Init()` configures the VFS, logging, and addresses, then calls `LoadPlugins()`.
- `LoadPlugins()` enumerates `plugins/*.dll`, calls `LoadLibraryA` on each, checks version/APIs, and invokes plugin `Init`.
- After all plugin `Init` routines finish, `LoadPlugins()` invokes `StartPlugins()`.
- `StartPlugins()` calls each plugin's `Start()` export.

### Binary Loading Mechanism
- `SOVIET64.exe` static import directory (RVA `0x984b3c`) does **not** import `tesmioloader.dll`.
- There is no DLL proxying (e.g. wrapper `d3d11.dll` or `dxgi.dll`).
- In `src/tesmiolauncher.cpp` (lines 1130-1175):
  - `CreateProcessW(..., CREATE_SUSPENDED, ...)` launches `SOVIET64.exe` with its main thread suspended.
  - `VirtualAllocEx` and `WriteProcessMemory` allocate and write the full path of `tesmioloader.dll` into the game process.
  - `CreateRemoteThread(..., LoadLibraryW, ...)` executes `LoadLibraryW("tesmioloader.dll")` on a dedicated remote thread.
  - `WaitForSingleObject` waits up to 30 seconds for the remote thread to finish.
  - `ResumeThread(pi.hThread)` releases the game's main thread only **after** `tesmioloader.dll`'s `DllMain`, `Init`, `LoadPlugins`, and `StartPlugins` have fully returned.

---

## 3. Engine Initialize Caller Route in Executable Startup

The call route from the game's entry point to D3D11 creation was traced through disassembly:

```
SOVIET64.exe PE AddressOfEntryPoint (RVA 0x84eb58 / VA 0x14084eb58)
  │
  ▼
MSVC CRT Startup (__scrt_common_main_seh at VA 0x14084e9ec)
  │
  ▼ [CALL at VA 0x14084eafe]
SOVIET64.exe WinMain (RVA 0x103cb0 / VA 0x140103cb0)
  │
  ├─► Class Registration: RegisterClassExW (VA 0x140104014)
  │
  ├─► Game Window Creation: CreateWindowExW (VA 0x140104070)
  │     └─► HWND returned in RAX, stored to global VA 0x1409941f8
  │
  ├─► ShowWindow (VA 0x14010409c) & UpdateWindow (VA 0x140104117)
  │
  ├─► Create Device Object: C3D_MIDDLEPOINT::CreateDevice(type = 2) (VA 0x140104180)
  │     └─► Returns C3DAPI_D3D11_DEVICE* (vtable at 0x180187aa8)
  │
  ├─► Configure C3DAPI_D3D11_DEVICE:
  │     ├─► [rcx + 0x10] = Width (VA 0x1401041d5)
  │     ├─► [rcx + 0x14] = Height (VA 0x1401041de)
  │     └─► [rcx + 0x30] = HWND from global 0x1409941f8 (VA 0x140104236 - 0x14010423d)
  │
  ▼ [CALL qword ptr [rax] at VA 0x14010425b]
C3DDLL64.dll C3DAPI_D3D11_DEVICE::Initialize (vtable slot 0, RVA 0x145c0 / VA 0x1800145c0)
  │
  ├─► Build DXGI_SWAP_CHAIN_DESC at [RBP - 0x50]:
  │     ├─► BufferDesc.Width = [RDI + 0x10] (VA 0x1800147c3)
  │     ├─► BufferDesc.Height = [RDI + 0x14] (VA 0x1800147c9)
  │     ├─► BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM (28) (VA 0x180014805)
  │     └─► OutputWindow ([RBP - 0x20]) = [RDI + 0x30] (HWND) (VA 0x180014825 - 0x18001483b)
  │
  ▼ [CALL qword ptr [0x18016b750] at VA 0x1800148bc]
d3d11.dll!D3D11CreateDeviceAndSwapChain (indirect call via C3DDLL64 IAT slot RVA 0x16b750)
  ├─► Out ppSwapChain: 0x1801d7730
  ├─► Out ppDevice: 0x1801d76c0
  └─► Out ppImmediateContext: 0x1801d76c8
```

---

## 4. HWND Origin & Provenance

1. **Creation Site:** `SOVIET64.exe` at RVA `0x104070` (VA `0x140104070`) via `USER32.dll!CreateWindowExW`.
   - `lpClassName`: `"Workers & Resources: Soviet Republic"` (wide string at VA `0x1408884c0`)
   - `lpWindowName`: `"Workers & Resources: Soviet Republic"` (wide string at VA `0x140888470`)
   - `dwStyle`: `0x86000000` (`WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS`)
   - `dwExStyle`: `0x40000` (`WS_EX_APPWINDOW`)
2. **Storage Site:** The returned `HWND` in `RAX` is immediately checked and written to global variable `0x1409941f8` (RVA `0x9941f8`) at instruction `mov qword ptr [rip + 0x89017b], rax` (VA `0x140104076`).
3. **Route into Engine Initialize:**
   - At VA `0x140104236`, `SOVIET64.exe` reads `0x1409941f8` into `RAX`.
   - At VA `0x14010423d`, it writes `RAX` into `[RCX + 0x30]` where `RCX` is `C3DAPI_D3D11_DEVICE*`.
   - At VA `0x14010425b`, it calls `[RAX]` (vtable slot 0), entering `C3DDLL64.dll!C3DAPI_D3D11_DEVICE::Initialize`.
   - Inside `Initialize`, at VA `0x180014825`, `mov rax, qword ptr [rdi + 0x30]` loads the HWND and at VA `0x18001483b`, `mov qword ptr [rbp - 0x20], rax` places it into the `DXGI_SWAP_CHAIN_DESC` struct at stack offset `-0x20` (which is offset `+0x30` from `[RBP - 0x50]`, the standard offset of `OutputWindow` in 64-bit `DXGI_SWAP_CHAIN_DESC`).

---

## 5. Partial Ordering Table

Every edge below is evaluated based strictly on verified binary/source evidence:

| Event A | Event B | Ordering Relation | Evidence / Proof | Status |
|---|---|---|---|---|
| `loader DllMain (remote thread)` | `main thread ResumeThread` | A precedes B | `tesmiolauncher.cpp:1162,1170` (`WaitForSingleObject(th)` before `ResumeThread`) | **PROVEN** |
| `loader DllMain` | `plugin Init` | A precedes B | `tesmioloader.cpp:2116,2105,1447` (`Init()` calls `LoadPlugins()` calls `init()`) | **PROVEN** |
| `plugin Init` | `plugin Start` | A precedes B | `tesmioloader.cpp:1528,1473` (`LoadPlugins()` calls `StartPlugins()` after all `Init`) | **PROVEN** |
| `plugin Start` | `main thread ResumeThread` | A precedes B | `tesmioloader.cpp` runs to completion on remote thread before `WaitForSingleObject` unblocks | **PROVEN** |
| `main thread ResumeThread` | `SOVIET64 entry (0x84eb58)` | A precedes B | Process was created `CREATE_SUSPENDED`; OS begins user code execution only on resume | **PROVEN** |
| `SOVIET64 entry` | `WinMain (0x103cb0)` | A precedes B | CRT `__scrt_common_main_seh` calls `0x140103cb0` | **PROVEN** |
| `WinMain entry` | `CreateWindowExW (0x104070)` | A precedes B | Instructions execute sequentially on the main thread | **PROVEN** |
| `CreateWindowExW (0x104070)` | `C3D_MIDDLEPOINT::CreateDevice` | A precedes B | Main thread sequence: HWND created at `0x104070`, device created at `0x104180` | **PROVEN** |
| `C3D_MIDDLEPOINT::CreateDevice` | `C3DAPI_D3D11_DEVICE::Initialize` | A precedes B | Main thread sequence: device created at `0x104180`, `Initialize` called at `0x10425b` | **PROVEN** |
| `C3DAPI_D3D11_DEVICE::Initialize` | `D3D11CreateDeviceAndSwapChain (0x148bc)` | A precedes B | Direct call within `Initialize` | **PROVEN** |
| `D3D11 creation return` | `First Present (slot 8)` | A precedes B | Swap chain must exist and be returned before rendering/presenting through it | **PROVEN** |
| `plugin Start` | `Game HWND Creation (0x104070)` | **A precedes B** | A runs during suspended init; B runs well after main thread resume in `WinMain` | **PROVEN** |
| `plugin Start` | `D3D11 creation (0x148bc)` | **A precedes B** | A runs during suspended init; B runs well after window creation in `WinMain` | **PROVEN** |
| `Game HWND Creation (0x104070)` | `D3D11 creation (0x148bc)` | **A precedes B** | HWND is created at `0x104070` and passed into `Initialize` at `0x10423d`/`0x1483b` | **PROVEN** |
| Non-launcher startup (e.g. Steam direct) | `loader attach` | **UNKNOWN** | Without `tesmiolauncher`, `tesmioloader` is not loaded (no proxy/static import exists) | **PROVEN UNKNOWN** |
| Thread safety of global `0x1409941f8` read | Plugin thread | **UNKNOWN** | Main thread writes `0x1409941f8`; no cross-thread lock or memory fence is emitted | **PROVEN UNKNOWN** |

---

## 6. Observation, Evidence, Interpretation, and Unknowns

| Observation | Evidence | Interpretation | Unknown / Limitation |
|---|---|---|---|
| Loader runs entirely before the game starts. | `tesmiolauncher.cpp:1130-1175` | IAT hooks placed in `C3DDLL64.dll` during plugin `Start` will intercept all engine calls made during normal game execution. | Does not apply if the game is launched without `tesmiolauncher.exe`. |
| Game HWND does not exist when plugin `Start` executes. | `SOVIET64.exe` disasm: `CreateWindowExW` is at RVA `0x104070` in `WinMain`. Plugin `Start` runs before `WinMain`. | An installer requiring a non-null, verified HWND during plugin `Start` cannot succeed. | N/A (statically proven). |
| Game HWND is available and stored before `D3D11CreateDeviceAndSwapChain` is called. | `SOVIET64.exe` RVA `0x104076` writes HWND to `0x1409941f8`. D3D11 creation is called at C3D RVA `0x148bc`. | When `ObserveCreation` is intercepted, `desc->OutputWindow` will contain the valid, already-created game HWND. | Does not guarantee window is active/focused yet at device creation time. |
| Output parameters of `D3D11CreateDeviceAndSwapChain` in `C3DDLL64.dll`. | Call at `0x1800148bc`: stack args at `+0x40` (`0x1801d7730`), `+0x48` (`0x1801d76c0`), `+0x58` (`0x1801d76c8`). | `0x1801d7730` is `ppSwapChain`, `0x1801d76c0` is `ppDevice`, `0x1801d76c8` is `ppImmediateContext`. | Dereferencing these globals directly at runtime is unsafe; intercepting via `ObserveCreation` parameters is required. |

---

## 7. Assessment of Installer Contract Satisfaction

### Current Contract in `prototypes/almanac_overlay/native/graphics_adapter.h`:
```cpp
bool InstallGraphicsAdapter(const TsmHost* host, HWND verifiedGameWindow, bool developmentOptIn);
```
Inside `graphics_adapter.cpp`:
```cpp
bool InstallGraphicsAdapter(const TsmHost* api, HWND verifiedGameWindow, bool developmentOptIn) {
    ...
    if (installed || !healthy.load() || !IsWindow(verifiedGameWindow) || !developmentOptIn || !api || ...)
        return false;
    ...
    verifiedWindow = verifiedGameWindow;
    installed = ReplaceSlot(slot, expected, reinterpret_cast<void*>(ObserveCreation), ...);
    return installed;
}
```
And inside `ObserveCreation`:
```cpp
if (!permission.load() || desc->OutputWindow != verifiedWindow || process != GetCurrentProcessId())
    return result;
```

### Verdict:
**The current installer contract CANNOT be satisfied during plugin `Start`.**

- **Why:** `InstallGraphicsAdapter` explicitly requires `!IsWindow(verifiedGameWindow)` to be false (i.e. `verifiedGameWindow` must be an existing, valid Win32 window handle). But during plugin `Start`, the game process is still suspended in early initialization; `WinMain` has not executed, and `CreateWindowExW` has not yet been called. Passing `NULL` or an uncreated handle causes `InstallGraphicsAdapter` to immediately return `false`.
- **Conversely, if `InstallGraphicsAdapter` is deferred until after the window is created:** `CreateWindowExW` (RVA `0x104070`) and `D3D11CreateDeviceAndSwapChain` (via `Initialize` at RVA `0x10425b`) execute back-to-back in the same function (`WinMain`) on the main thread, separated by only ~500 instructions (window setup and device configuration). No plugin callbacks or safe-phase hooks currently exist between `CreateWindowExW` and `Initialize`. If installation is deferred until after game startup, `D3D11CreateDeviceAndSwapChain` has **already occurred**, and `graphics_adapter.cpp` specifically states:
  > `"Startup ordering must still be observed: late installation intentionally stays unavailable."`

### Smallest Missing Observation / Architectural Decision:
The pre-existing HWND verification cannot occur at IAT installation time. At IAT installation time (during plugin `Start`), the IAT slot in `C3DDLL64.dll` can be hooked safely, but window verification must be evaluated **inside `ObserveCreation`** when `desc->OutputWindow` is presented by the engine, or the game HWND must be validated by matching its process ID and class name (`"SovietClass"` / `"Workers & Resources: Soviet Republic"`) rather than requiring a pre-supplied handle at DLL attachment time.
