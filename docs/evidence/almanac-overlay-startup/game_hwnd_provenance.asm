; ==============================================================================
; GAME HWND PROVENANCE, STORAGE, AND ROUTE INTO ENGINE INITIALIZE
; Target: SOVIET64.exe (v1.1.1.9, SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8)
; Target: C3DDLL64.dll (SHA256: 65FCB6845C3CB7C25A22121F25904A855EA88657E31A3B50A15ADAC26E231516)
; ==============================================================================

; --- PART 1: HWND CREATION SITE ---
; Located in SOVIET64.exe within WinMain (RVA 0x103cb0, function chunk 0x103d16-0x104086)

; 1. Window Class Registration (RegisterClassExW)
140103fe0: lea      rcx, [rbp - 0x60]              ; WNDCLASSEXW structure
140103ff7: lea      rax, [rip - 0xa6e]             ; lpfnWndProc (RVA 0x103590, VA 0x140103590)
140103ffe: mov      qword ptr [rbp - 0x58], rax
140104002: lea      rax, [rip + 0x784417]          ; lpszClassName at 0x140888420: L"SovietClass"
140104009: mov      qword ptr [rbp - 0x20], rax
140104014: call     qword ptr [rip + 0x769bbe]     ; USER32.dll!RegisterClassExW (IAT RVA 0x86dbd8)

; 2. Window Creation (CreateWindowExW)
140104023: mov      rax, qword ptr [rbp - 0x48]    ; hInstance
140104027: lea      r8, [rip + 0x784442]           ; lpWindowName at 0x140888470: L"Workers & Resources: Soviet Republic"
14010402e: mov      qword ptr [rsp + 0x58], rsi    ; lpParam = NULL
140104033: lea      rdx, [rip + 0x784486]          ; lpClassName at 0x1408884c0: L"Workers & Resources: Soviet Republic"
14010403a: mov      qword ptr [rsp + 0x50], rax    ; hInstance
14010403f: mov      r9d, 0x86000000                ; dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS (or fullscreen/windowed flags)
140104045: mov      eax, dword ptr [rip + 0x891229] ; nHeight
14010404b: mov      ecx, 0x40000                   ; dwExStyle = WS_EX_APPWINDOW
140104050: mov      qword ptr [rsp + 0x48], rsi    ; hMenu = NULL
140104055: mov      qword ptr [rsp + 0x40], rsi    ; hWndParent = NULL
14010405a: mov      dword ptr [rsp + 0x38], eax    ; nHeight
14010405e: mov      eax, dword ptr [rip + 0x891228] ; nWidth
140104064: mov      dword ptr [rsp + 0x30], eax    ; nWidth
140104068: mov      dword ptr [rsp + 0x28], esi    ; y = 0
14010406c: mov      dword ptr [rsp + 0x20], esi    ; x = 0
140104070: call     qword ptr [rip + 0x769b52]     ; USER32.dll!CreateWindowExW (IAT RVA 0x86dbc8)

; --- PART 2: HWND STORAGE SITE ---
; The returned HWND (in RAX) is verified non-NULL and immediately stored in global variable:
140104076: mov      qword ptr [rip + 0x89017b], rax ; STORE TO GLOBAL: VA 0x1409941f8 (RVA 0x9941f8)
14010407d: test     rax, rax
140104080: je       0x140105382                    ; fail if NULL

; --- PART 3: IMMEDIATE WINDOW USAGE (BEFORE GRAPHICS INITIALIZATION) ---
; The window is immediately made visible and updated on the main thread:
14010408e: lea      edx, [rsi + 0xa]               ; nCmdShow = SW_SHOWDEFAULT (10)
140104091: mov      rcx, rax                       ; hWnd
14010409c: call     qword ptr [rip + 0x769b3e]     ; USER32.dll!ShowWindow (IAT RVA 0x86dbe0)
...
14010410a: call     qword ptr [rip + 0x769ab0]     ; USER32.dll!SetWindowPos (IAT RVA 0x86dbc0)
140104110: mov      rcx, qword ptr [rip + 0x8900e1] ; load hWnd from global 0x1409941f8
140104117: call     qword ptr [rip + 0x769a73]     ; USER32.dll!UpdateWindow (IAT RVA 0x86db90)

; --- PART 4: ROUTE INTO ENGINE INITIALIZE ---
; Later in the same function (chunk 0x104086-0x1051d0):
; At 0x140104236:
140104236: mov      rax, qword ptr [rip + 0x88ffbb] ; LOAD FROM GLOBAL: 0x14010423d + 0x88ffbb = 0x1409941f8 (HWND)
14010423d: mov      qword ptr [rcx + 0x30], rax     ; STORE INTO C3DAPI_D3D11_DEVICE struct at offset +0x30
140104258: mov      rax, qword ptr [rcx]           ; load vtable (0x180187aa8)
14010425b: call     qword ptr [rax]                ; invoke C3DAPI_D3D11_DEVICE::Initialize (slot 0, RVA 0x145c0)

; --- PART 5: ROUTE INTO DXGI_SWAP_CHAIN_DESC INSIDE ENGINE ---
; In C3DDLL64.dll C3DAPI_D3D11_DEVICE::Initialize (RVA 0x145c0):
180014825: mov      rax, qword ptr [rdi + 0x30]     ; LOAD HWND FROM C3DAPI_D3D11_DEVICE+0x30
18001483b: mov      qword ptr [rbp - 0x20], rax     ; STORE INTO [RBP - 0x20] (DXGI_SWAP_CHAIN_DESC.OutputWindow)
; DXGI_SWAP_CHAIN_DESC is located at [RBP - 0x50]. Offset +0x30 is OutputWindow (HWND).
; Passed directly to D3D11CreateDeviceAndSwapChain at 0x1800148bc as 8th argument (stack offset +0x38).
