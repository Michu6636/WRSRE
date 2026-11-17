# Almanac Overlay Startup Ordering & Provenance: Corrections and Addendum

## 1. Context and Baseline

- Baseline commit: `d67cf4d`
- Binary identities verified:
  - `SOVIET64.exe`: SHA256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`
  - `C3DDLL64.dll`: SHA256 `65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516`
- This addendum resolves four specific clarification and rigor points on startup ordering, error handling, HWND class identification, and post-window installation possibilities.

---

## 2. Launcher Timeout / Failure Path Analysis (WAIT_TIMEOUT and STILL_ACTIVE)

### Raw Source Excerpt (`src/tesmiolauncher.cpp:1156-1175`)
```cpp
HANDLE th = CreateRemoteThread(pi.hProcess, NULL, 0,
                               (LPTHREAD_START_ROUTINE)loadLib, remote, 0, NULL);
if (!th)
    Fail(L"CreateRemoteThread");
else
{
    WaitForSingleObject(th, 30000);

    DWORD loaded = 0;
    GetExitCodeThread(th, &loaded);
    CloseHandle(th);

    if (!loaded)
        Msg(L"LoadLibraryW returned NULL - the DLL did not load");
    else if (ResumeThread(pi.hThread) == (DWORD)-1)
        Fail(L"ResumeThread");
    else
    {
        Msg(L"tesmioloader.dll loaded, game resumed");
        ok = true;
    }
}
```

### Analysis of Asynchronous Failure Modes
1. **Ignored Return of `WaitForSingleObject`:**
   `WaitForSingleObject(th, 30000)` return value is discarded. If the remote thread hangs or exceeds 30 seconds, it returns `WAIT_TIMEOUT`.
2. **Behavior of `GetExitCodeThread` on Timeout:**
   If the remote thread is still running after 30 seconds, `GetExitCodeThread(th, &loaded)` populates `loaded` with `STILL_ACTIVE` (`0x00000103`, decimal `259`).
3. **Execution Branch:**
   Because `STILL_ACTIVE != 0`, `if (!loaded)` evaluates to `false`. The launcher proceeds directly to:
   ```cpp
   else if (ResumeThread(pi.hThread) == (DWORD)-1)
   ```
   and successfully resumes the game's main thread while the remote thread is **still executing** `LoadLibraryW("tesmioloader.dll")`.
4. **Correction to Ordering Claims:**
   The claim that plugin `Start` *unconditionally* completes before the main thread resumes is **INVALIDATED** in the failure/timeout case.
   - **Normal, non-timed-out injection path:** `plugin Start` completes on the remote thread before `WaitForSingleObject` returns `WAIT_OBJECT_0`, so `plugin Start` precedes `ResumeThread`.
   - **Timeout / Hang path (>= 30s):** `ResumeThread` executes concurrently while the remote thread is still in `LoadLibraryW` / `Init` / `StartPlugins`.
   - **Load failure path (LoadLibrary returns NULL):** `loaded == 0`, `ResumeThread` is never called, and the game process remains suspended until terminated.

---

## 3. Exact HWND Class and Window Title Provenance

### Bounded Instruction Setup (`SOVIET64.exe` RVA `0x103fd0 - 0x104080`)
```asm
; --- RegisterClassExW Setup ---
140103fe0: lea      rcx, [rbp - 0x60]              ; WNDCLASSEXW struct pointer
140103fe4: mov      qword ptr [rbp - 0x28], rsi    ; hbrBackground = NULL
140103fe8: mov      qword ptr [rbp - 0x18], rax    ; hIconSm
140103fec: mov      rax, qword ptr [rip + 0x891255] ; hInstance
140103ff3: mov      qword ptr [rbp - 0x48], rax    ; hInstance
140103ff7: lea      rax, [rip - 0xa6e]             ; lpfnWndProc (VA 0x140103590, RVA 0x103590)
140103ffe: mov      qword ptr [rbp - 0x58], rax    ; lpfnWndProc
140104002: lea      rax, [rip + 0x784417]          ; lpszClassName -> VA 0x140888420 (RVA 0x888420)
140104009: mov      qword ptr [rbp - 0x20], rax    ; lpszClassName in WNDCLASSEXW
14010400d: mov      dword ptr [rbp - 0x5c], 3      ; style = CS_VREDRAW | CS_HREDRAW (3)
140104014: call     qword ptr [rip + 0x769bbe]     ; USER32.dll!RegisterClassExW (IAT RVA 0x86dbd8)
14010401a: test     ax, ax
14010401d: je       0x140105382

; --- CreateWindowExW Parameter Marshalling ---
140104023: mov      rax, qword ptr [rbp - 0x48]    ; hInstance
140104027: lea      r8, [rip + 0x784442]           ; R8  = lpWindowName -> VA 0x140888470 (RVA 0x888470)
14010402e: mov      qword ptr [rsp + 0x58], rsi    ; arg 12: lpParam = NULL
140104033: lea      rdx, [rip + 0x784486]          ; RDX = lpClassName  -> VA 0x1408884c0 (RVA 0x8884c0)
14010403a: mov      qword ptr [rsp + 0x50], rax    ; arg 11: hInstance
14010403f: mov      r9d, 0x86000000                ; R9  = dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS
140104045: mov      eax, dword ptr [rip + 0x891229] ; nHeight
14010404b: mov      ecx, 0x40000                   ; RCX = dwExStyle = WS_EX_APPWINDOW
140104050: mov      qword ptr [rsp + 0x48], rsi    ; arg 10: hMenu = NULL
140104055: mov      qword ptr [rsp + 0x40], rsi    ; arg 9:  hWndParent = NULL
14010405a: mov      dword ptr [rsp + 0x38], eax    ; arg 8:  nHeight
14010405e: mov      eax, dword ptr [rip + 0x891228] ; nWidth
140104064: mov      dword ptr [rsp + 0x30], eax    ; arg 7:  nWidth
140104068: mov      dword ptr [rsp + 0x28], esi    ; arg 6:  y = 0
14010406c: mov      dword ptr [rsp + 0x20], esi    ; arg 5:  x = 0
140104070: call     qword ptr [rip + 0x769b52]     ; USER32.dll!CreateWindowExW (IAT RVA 0x86dbc8)
```

### Raw Referenced String Bytes and Decoded Values
All three referenced strings are located in the `.rdata` section (File Offset = RVA - `0x1000`):

1. **`RegisterClassExW` `lpszClassName` (`[rbp - 0x20]`):**
   - Address: VA `0x140888420`, RVA `0x888420`, File Offset `0x887420`
   - Raw UTF-16LE bytes:
     `57 00 6f 00 72 00 6b 00 65 00 72 00 73 00 20 00 26 00 20 00 52 00 65 00 73 00 6f 00 75 00 72 00 63 00 65 00 73 00 3a 00 20 00 53 00 6f 00 76 00 69 00 65 00 74 00 20 00 52 00 65 00 70 00 75 00 62 00 6c 00 69 00 63 00 00 00`
   - Decoded value: `L"Workers & Resources: Soviet Republic"`

2. **`CreateWindowExW` `lpWindowName` (`R8`):**
   - Address: VA `0x140888470`, RVA `0x888470`, File Offset `0x887470`
   - Raw UTF-16LE bytes: Identical bytes to above.
   - Decoded value: `L"Workers & Resources: Soviet Republic"`

3. **`CreateWindowExW` `lpClassName` (`RDX`):**
   - Address: VA `0x1408884c0`, RVA `0x8884c0`, File Offset `0x8874c0`
   - Raw UTF-16LE bytes: Identical bytes to above.
   - Decoded value: `L"Workers & Resources: Soviet Republic"`

### Resolution of Fallback Name `"SovietClass"`
- A byte search across the entire `SOVIET64.exe` binary for `b"SovietClass"` (ASCII) and `"SovietClass".encode("utf-16le")` returned **`False`**.
- `"SovietClass"` does **NOT** exist anywhere in `SOVIET64.exe`. It was an ungrounded hypothesis in earlier notes.
- **Verified Fact:** The window class name and the window title are identical 37-character UTF-16LE strings: `L"Workers & Resources: Soviet Republic"`. Both registration and window creation reference this exact string.

---

## 4. Qualification of Post-Window Installation Possibilities

### What the Observed Call Path Proves:
1. `CreateWindowExW` (RVA `0x104070`) and `C3DAPI_D3D11_DEVICE::Initialize` (RVA `0x10425b`) execute synchronously in the same function (`WinMain`) on thread 0.
2. Between these two calls, the executable executes standard window positioning and COM initialization (`ShowWindow`, `SetWindowPos`, `UpdateWindow`, `ShowCursor`, `CoInitializeEx`, `GetWindowRect`, `SetCursorPos`), followed by `C3D_MIDDLEPOINT::CreateDevice` and field population.
3. In the unhooked, baseline binary, no modding loader hook or callback is invoked along this ~500-instruction path.

### What Remains Unknown / Unproven:
1. **Deferred Hooking via Secondary Interception:**
   The instructions prove that `tesmioloader`'s current API does not expose a post-window creation hook. However, they do **NOT** prove that a post-window interception is architecturally impossible. For example:
   - A hook on an intermediate User32 API called between `CreateWindowExW` and `Initialize` (such as `SetWindowPos`, `UpdateWindow`, or `ShowCursor`) could observe the already-created `HWND` while `D3D11CreateDeviceAndSwapChain` is still pending.
   - A hook on `C3D_MIDDLEPOINT::CreateDevice` (RVA `0x104180` / engine RVA `0xb1470`) could intercept the device creation object immediately before `Initialize` is called.
2. **Current Adapter Contract Limit:**
   The current candidate contract (`InstallGraphicsAdapter(host, verifiedGameWindow, developmentOptIn)`) does not implement or use any intermediate User32 or engine hook. It attempts to install directly against `C3DDLL64.dll`'s IAT slot for `D3D11CreateDeviceAndSwapChain`.
   - Therefore, under the **existing, unmodified loader architecture and candidate contract**, any installation performed after window creation would arrive after graphics initialization unless a new intermediate interception point is explicitly introduced.

---

## 5. Corrected Conditional Ordering Table

| Condition | Ordering Relationship | Status |
|---|---|---|
| **Normal Launch (`th` unblocks within 30s, `loaded != 0`)** | `plugin Start` $\rightarrow$ `ResumeThread` $\rightarrow$ `WinMain` $\rightarrow$ `CreateWindowExW` $\rightarrow$ `D3D11 creation` | **PROVEN** |
| **Timeout Case (`th` blocks > 30s, `loaded == STILL_ACTIVE`)** | `ResumeThread` concurrent with `LoadLibraryW`/`plugin Start` | **PROVEN (Race Condition)** |
| **Direct Steam / Executable Launch (No launcher)** | Loader is never attached (`tesmioloader` not loaded) | **PROVEN** |
| **Pre-existing HWND at plugin Start** | HWND is NULL / invalid (`CreateWindowExW` has not executed) | **PROVEN** |
| **Window Class Name** | `L"Workers & Resources: Soviet Republic"` (not `"SovietClass"`) | **PROVEN** |
| **Window Title** | `L"Workers & Resources: Soviet Republic"` | **PROVEN** |
