   1800693a5:	call   0x180053ef0
   1800693aa:	call   0x180038730
   1800693af:	cmp    DWORD PTR [rip+0x59f37],ebp        # 0x1800c32ec
   1800693b5:	je     0x180069490
   1800693bb:	cmp    QWORD PTR [rip+0x21ccf6],rbp        # 0x1802860b8
   1800693c2:	je     0x180069490
   1800693c8:	mov    rcx,QWORD PTR [rip+0x5b9a9]        # 0x1800c4d78
   1800693cf:	lea    rax,[rip+0x4557a]        # 0x1800ae950    ;; "C3D_PANEL2D::SetDimensions"
   1800693d6:	mov    QWORD PTR [rsp+0x28],rax
   1800693db:	lea    r9,[rip+0xffffffffffffebbe]        # 0x180067fa0
   1800693e2:	lea    rax,[rip+0x21cccf]        # 0x1802860b8
   1800693e9:	mov    QWORD PTR [rsp+0x20],rax
   1800693ee:	lea    r8,[rip+0x32f13]        # 0x18009c308    ;; "?SetDimensions@C3D_PANEL2D@@QEAAXMM@Z"
   1800693f5:	mov    rax,QWORD PTR [rip+0x5b974]        # 0x1800c4d70
   1800693fc:	lea    rdx,[rip+0x25555]        # 0x18008e958    ;; "C3DDLL64.dll"
   180069403:	call   QWORD PTR [rax+0x48]
   180069406:	mov    rcx,QWORD PTR [rip+0x5b96b]        # 0x1800c4d78
   18006940d:	lea    r9,[rip+0xffffffffffffec7c]        # 0x180068090
   180069414:	mov    ebx,eax
   180069416:	lea    r8,[rip+0x32f23]        # 0x18009c340    ;; "?SetPosition@C3D_PANEL2D@@QEAAXAEBVC3DVECTOR3@@@Z"
   18006941d:	lea    rax,[rip+0x4554c]        # 0x1800ae970    ;; "C3D_PANEL2D::SetPosition"
   180069424:	mov    QWORD PTR [rsp+0x28],rax
   180069429:	lea    rdx,[rip+0x25528]        # 0x18008e958    ;; "C3DDLL64.dll"
   180069430:	lea    rax,[rip+0x21cc89]        # 0x1802860c0
   180069437:	mov    QWORD PTR [rsp+0x20],rax
   18006943c:	mov    rax,QWORD PTR [rip+0x5b92d]        # 0x1800c4d70
   180069443:	call   QWORD PTR [rax+0x48]
   180069446:	test   ebx,ebx
   180069448:	je     0x180069467
   18006944a:	test   eax,eax
   18006944c:	je     0x180069467
   18006944e:	mov    edx,0x31e478
   180069453:	lea    rcx,[rip+0x45536]        # 0x1800ae990    ;; "  side  panel-rectangle hooks armed at exe+0x%X and 0x%X - the panel holds the width in_panel_w asks for, and our conten"
   18006945a:	mov    r8d,0x31e509
   180069460:	call   0x180053ef0
   180069465:	jmp    0x180069490
   180069467:	test   eax,eax
   180069469:	lea    rcx,[rip+0x2a6e0]        # 0x180093b50    ;; "hooked"
   180069470:	lea    rdx,[rip+0x455c1]        # 0x1800aea38    ;; "MISSING"
   180069477:	mov    r8,rdx
   18006947a:	cmovne r8,rcx
   18006947e:	test   ebx,ebx
   180069480:	cmovne rdx,rcx
   180069484:	lea    rcx,[rip+0x455b5]        # 0x1800aea40    ;; "  side  SetDimensions %s, SetPosition %s in the import table - in_panel cannot find the panel and stays off"
   18006948b:	call   0x180053ef0
   180069490:	mov    rcx,QWORD PTR [rip+0x5b8e1]        # 0x1800c4d78
   180069497:	lea    rax,[rip+0x45612]        # 0x1800aeab0    ;; "C3D_PANEL2D::SetReallyRender"
   18006949e:	mov    QWORD PTR [rsp+0x28],rax
   1800694a3:	lea    r9,[rip+0xffffffffffffec96]        # 0x180068140
   1800694aa:	lea    rax,[rip+0x21cc57]        # 0x180286108
   1800694b1:	mov    QWORD PTR [rsp+0x20],rax
   1800694b6:	lea    r8,[rip+0x45613]        # 0x1800aead0    ;; "?SetReallyRender@C3D_PANEL2D@@QEAAX_N@Z"
   1800694bd:	mov    rax,QWORD PTR [rip+0x5b8ac]        # 0x1800c4d70
   1800694c4:	lea    rdx,[rip+0x2548d]        # 0x18008e958    ;; "C3DDLL64.dll"
   1800694cb:	call   QWORD PTR [rax+0x48]
   1800694ce:	test   eax,eax
   1800694d0:	je     0x1800694e0
   1800694d2:	lea    rcx,[rip+0x4561f]        # 0x1800aeaf8    ;; "  panel  interface-pass hook installed"
   1800694d9:	call   0x180053ef0
   1800694de:	jmp    0x1800694fb
   1800694e0:	lea    rcx,[rip+0x45639]        # 0x1800aeb20    ;; "  panel  SetReallyRender is NOT in the import table - the interface pass is unreachable, using the terrain pass"
   1800694e7:	call   0x180053ef0
   1800694ec:	cmp    DWORD PTR [rip+0x217619],0x1        # 0x180280b0c
   1800694f3:	jne    0x1800694fb
   1800694f5:	mov    DWORD PTR [rip+0x217611],ebp        # 0x180280b0c
   1800694fb:	mov    rcx,QWORD PTR [rip+0x5b876]        # 0x1800c4d78
   180069502:	lea    rax,[rip+0x45687]        # 0x1800aeb90    ;; "C3D_PANEL2D::EndDraw"
   180069509:	mov    QWORD PTR [rsp+0x28],rax
   18006950e:	lea    r9,[rip+0xffffffffffffe6db]        # 0x180067bf0
   180069515:	lea    rax,[rip+0x21cbf4]        # 0x180286110
   18006951c:	mov    QWORD PTR [rsp+0x20],rax
   180069521:	lea    r8,[rip+0x32cf8]        # 0x18009c220    ;; "?EndDraw@C3D_PANEL2D@@QEAAXXZ"
   180069528:	mov    rax,QWORD PTR [rip+0x5b841]        # 0x1800c4d70
   18006952f:	lea    rdx,[rip+0x25422]        # 0x18008e958    ;; "C3DDLL64.dll"
   180069536:	call   QWORD PTR [rax+0x48]
   180069539:	test   eax,eax
   18006953b:	jne    0x180069558
   18006953d:	lea    rcx,[rip+0x4566c]        # 0x1800aebb0    ;; "  panel  EndDraw is not in the import table - hook 2 unavailable"
   180069544:	call   0x180053ef0
   180069549:	cmp    DWORD PTR [rip+0x2175bc],0x2        # 0x180280b0c
   180069550:	jne    0x180069558
   180069552:	mov    DWORD PTR [rip+0x2175b4],ebp        # 0x180280b0c
   180069558:	mov    rcx,QWORD PTR [rip+0x5b819]        # 0x1800c4d78
   18006955f:	lea    rax,[rip+0x45692]        # 0x1800aebf8    ;; "C3D_PANEL2D::Draw"
   180069566:	mov    QWORD PTR [rsp+0x28],rax
   18006956b:	lea    r9,[rip+0xffffffffffffe82e]        # 0x180067da0
   180069572:	lea    rax,[rip+0x21cb9f]        # 0x180286118
   180069579:	mov    QWORD PTR [rsp+0x20],rax
   18006957e:	lea    r8,[rip+0x32cc3]        # 0x18009c248    ;; "?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z"
   180069585:	mov    rax,QWORD PTR [rip+0x5b7e4]        # 0x1800c4d70
   18006958c:	lea    rdx,[rip+0x253c5]        # 0x18008e958    ;; "C3DDLL64.dll"
   180069593:	call   QWORD PTR [rax+0x48]
   180069596:	test   eax,eax
   180069598:	jne    0x1800695b5
   18006959a:	lea    rcx,[rip+0x4566f]        # 0x1800aec10    ;; "  panel  Draw is not in the import table - hook 3 unavailable"
   1800695a1:	call   0x180053ef0
   1800695a6:	cmp    DWORD PTR [rip+0x21755f],0x3        # 0x180280b0c
   1800695ad:	jne    0x1800695b5
   1800695af:	mov    DWORD PTR [rip+0x217557],ebp        # 0x180280b0c
   1800695b5:	mov    rcx,QWORD PTR [rip+0x5b7bc]        # 0x1800c4d78
   1800695bc:	lea    rax,[rip+0x4568d]        # 0x1800aec50    ;; "C3D_FONTMANAGER::PrintAllTexts"
   1800695c3:	mov    QWORD PTR [rsp+0x28],rax
   1800695c8:	lea    r9,[rip+0xffffffffffffe941]        # 0x180067f10
   1800695cf:	lea    rax,[rip+0x21cb12]        # 0x1802860e8
   1800695d6:	mov    QWORD PTR [rsp+0x20],rax
   1800695db:	lea    r8,[rip+0x32f1e]        # 0x18009c500    ;; "?PrintAllTexts@C3D_FONTMANAGER@@QEAAXPEAD@Z"
   1800695e2:	mov    rax,QWORD PTR [rip+0x5b787]        # 0x1800c4d70
   1800695e9:	lea    rdx,[rip+0x25368]        # 0x18008e958    ;; "C3DDLL64.dll"
   1800695f0:	call   QWORD PTR [rax+0x48]
   1800695f3:	test   eax,eax
   1800695f5:	jne    0x180069603
   1800695f7:	lea    rcx,[rip+0x45672]        # 0x1800aec70    ;; "  fill   PrintAllTexts is not in the import table - [ui] fill_hook = 3 cannot work; try 2, then 1."
   1800695fe:	call   0x180053ef0
