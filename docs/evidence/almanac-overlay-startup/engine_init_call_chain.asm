; ==============================================================================
; ENGINE INITIALIZE CALL CHAIN: FROM EXECUTABLE ENTRY TO D3D11 CREATION
; Target: SOVIET64.exe (SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8)
; Target: C3DDLL64.dll (SHA256: 65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516)
; ==============================================================================

; --- STEP 1: SOVIET64.exe PE EntryPoint to WinMain ---
; PE AddressOfEntryPoint: RVA 0x84eb58 (VA 0x14084eb58)
; In MSVC CRT startup:
14084eb58: sub      rsp, 0x28
14084eb5c: call     0x14084f1c8
14084eb61: add      rsp, 0x28
14084eb65: jmp      0x14084e9ec     ; jump to __scrt_common_main_seh

; Inside __scrt_common_main_seh around 0x14084eaf7:
14084eaf7: lea      rcx, [rip - 0x84eafe] ; hInstance (0x140000000)
14084eafe: call     0x140103cb0           ; invoke WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd)

; --- STEP 2: WinMain (RVA 0x103cb0) down to Device Creation and Engine Initialize ---
; Function start: 0x140103cb0
; ... window class registration and CreateWindowExW at 0x140104070 ...
; ... ShowWindow / UpdateWindow at 0x14010409c - 0x140104117 ...

; At 0x140104176 - 0x140104180:
; SOVIET64 calls C3D_MIDDLEPOINT::CreateDevice(?CreateDevice@C3D_MIDDLEPOINT@@QEAAPEAVC3DAPI_DEVICE@@W4_C3DAPI_DEVICE_TYPE@@@Z)
140104176: lea      edx, [rsi + 2]        ; edx = 2 (device type: D3D11)
140104179: lea      rcx, [rip + 0x8e6b50] ; C3D_MIDDLEPOINT instance
140104180: call     qword ptr [rip + 0x768cfa] ; IAT slot 0x14086ce80 (CreateDevice)
140104186: mov      qword ptr [rip + 0x8910a3], rax ; store C3DAPI_D3D11_DEVICE* in global

; At 0x1401041c1 - 0x14010425b:
; Setup parameters in C3DAPI_D3D11_DEVICE and call virtual Initialize (slot 0)
1401041c1: mov      rcx, qword ptr [rip + 0x891068] ; C3DAPI_D3D11_DEVICE*
1401041c8: mov      eax, dword ptr [rip + 0x8910be] ; width
1401041d5: mov      dword ptr [rcx + 0x10], eax
1401041d8: mov      eax, dword ptr [rip + 0x891096] ; height
1401041de: mov      dword ptr [rcx + 0x14], eax
1401041e1: mov      eax, dword ptr [rip + 0x891079] ; adapter index / refresh
1401041e7: mov      dword ptr [rcx + 0x28], eax
1401041ea: mov      byte ptr [rcx + 0x20], sil
...
140104236: mov      rax, qword ptr [rip + 0x88ffbb] ; HWND from global 0x1409941f8
14010423d: mov      qword ptr [rcx + 0x30], rax     ; store HWND into C3DAPI_D3D11_DEVICE+0x30
...
140104258: mov      rax, qword ptr [rcx]           ; load vtable pointer (0x180187aa8)
14010425b: call     qword ptr [rax]                ; invoke slot 0: C3DAPI_D3D11_DEVICE::Initialize (RVA 0x145c0)

; --- STEP 3: Inside C3DDLL64.dll C3DAPI_D3D11_DEVICE::Initialize (RVA 0x145c0) ---
; Function RVA: 0x145c0 (VA 0x1800145c0)
; At entry: RCX = this (C3DAPI_D3D11_DEVICE*)
; Parameter setup into stack DXGI_SWAP_CHAIN_DESC at [RBP - 0x50]:
1800147c0: mov      eax, dword ptr [rdi + 0x10]     ; width
1800147c3: mov      dword ptr [rbp - 0x50], eax     ; BufferDesc.Width
1800147c6: mov      eax, dword ptr [rdi + 0x14]     ; height
1800147c9: mov      dword ptr [rbp - 0x4c], eax     ; BufferDesc.Height
1800147fe: mov      dword ptr [rbp - 0x28], 1       ; BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT (0x20) / Count
180014805: mov      dword ptr [rbp - 0x40], 0x1c    ; Format = DXGI_FORMAT_R8G8B8A8_UNORM (28 = 0x1c)
180014825: mov      rax, qword ptr [rdi + 0x30]     ; load HWND from C3DAPI_D3D11_DEVICE+0x30
18001483b: mov      qword ptr [rbp - 0x20], rax     ; store HWND to [RBP - 0x20] (DXGI_SWAP_CHAIN_DESC.OutputWindow)

; Stack argument marshalling for D3D11CreateDeviceAndSwapChain:
18001485e: lea      rcx, [rip + 0x1c2edb]          ; 0x1801d7730: ppSwapChain output
180014865: lea      rax, [rip + 0x1c2e54]          ; 0x1801d76c0: ppDevice output
180014878: mov      qword ptr [rsp + 0x58], r12    ; r12 = 0x1801d76c8 (ppImmediateContext)
180014880: mov      qword ptr [rsp + 0x50], r13    ; r13 = 0x1801cdbe4 (pFeatureLevel)
180014885: mov      qword ptr [rsp + 0x48], rax    ; 0x1801d76c0 (ppDevice)
18001488a: lea      rax, [rbp - 0x50]              ; pSwapChainDesc ([RBP - 0x50])
18001488e: mov      qword ptr [rsp + 0x40], rcx    ; 0x1801d7730 (ppSwapChain)
180014893: xor      ecx, ecx                       ; pAdapter = NULL
180014895: mov      qword ptr [rsp + 0x38], rax    ; pSwapChainDesc
18001489a: lea      rax, [rbp + 0x190]             ; pFeatureLevels array
1800148a1: mov      dword ptr [rsp + 0x30], 7      ; SDKVersion = D3D11_SDK_VERSION (7)
1800148a9: mov      dword ptr [rsp + 0x28], 3      ; FeatureLevels count = 3
1800148b1: mov      qword ptr [rsp + 0x20], rax    ; pFeatureLevels
180014873: mov      edx, dword ptr [rsi]           ; DriverType (D3D_DRIVER_TYPE_HARDWARE = 1)
180014875: xor      r9d, r9d                       ; Flags = 0
18001487d: xor      r8d, r8d                       ; Software = NULL

; Indirect call through C3DDLL64 IAT slot (RVA 0x16b750):
1800148bc: call     qword ptr [rip + 0x156e8e]     ; d3d11.dll!D3D11CreateDeviceAndSwapChain
