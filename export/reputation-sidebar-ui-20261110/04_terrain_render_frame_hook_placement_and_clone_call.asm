   180068180:	rex push rsi
   180068182:	push   rdi
   180068183:	push   r14
   180068185:	push   r15
   180068187:	sub    rsp,0x128
   18006818e:	movaps XMMWORD PTR [rsp+0x110],xmm6
   180068196:	mov    rax,QWORD PTR [rip+0x5bfe3]        # 0x1800c4180
   18006819d:	xor    rax,rsp
   1800681a0:	mov    QWORD PTR [rsp+0x100],rax
   1800681a8:	mov    eax,DWORD PTR [rsp+0x178]
   1800681af:	mov    DWORD PTR [rsp+0x28],eax
   1800681b3:	mov    eax,DWORD PTR [rsp+0x170]
   1800681ba:	mov    DWORD PTR [rsp+0x20],eax
   1800681be:	call   QWORD PTR [rip+0x11fbfc]        # 0x180187dc0
   1800681c4:	mov    eax,DWORD PTR [rip+0x13bdba]        # 0x1801a3f84
   1800681ca:	cmp    eax,0x1
   1800681cd:	jne    0x1800681d4
   1800681cf:	call   0x18002e410
   1800681d4:	cmp    DWORD PTR [rip+0x5ae45],0x0        # 0x1800c3020
   1800681db:	je     0x18006894b
   1800681e1:	xorps  xmm6,xmm6
   1800681e4:	movsd  xmm1,QWORD PTR [rip+0x5b07c]        # 0x1800c3268
   1800681ec:	comisd xmm6,xmm1
   1800681f0:	jae    0x1800682a9
   1800681f6:	movsd  xmm0,QWORD PTR [rip+0x139632]        # 0x1801a1830
   1800681fe:	comisd xmm0,xmm1
   180068202:	jb     0x1800682a9
   180068208:	mov    eax,DWORD PTR [rip+0x139612]        # 0x1801a1820
   18006820e:	cmp    eax,0x190
   180068213:	jge    0x1800682a9
   180068219:	inc    eax
   18006821b:	mov    DWORD PTR [rip+0x1395ff],eax        # 0x1801a1820
   180068221:	lea    rcx,[rsp+0xd0]
   180068229:	call   QWORD PTR [rip+0x24ed1]        # 0x18008d100    ;; [KERNEL32.dll!GetLocalTime]
   18006822f:	movzx  eax,WORD PTR [rsp+0xde]
   180068237:	movzx  ecx,WORD PTR [rsp+0xdc]
   18006823f:	movzx  edx,WORD PTR [rsp+0xda]
   180068247:	movzx  r8d,WORD PTR [rsp+0xd8]
   180068250:	mov    DWORD PTR [rsp+0x38],eax
   180068254:	mov    DWORD PTR [rsp+0x30],ecx
   180068258:	mov    DWORD PTR [rsp+0x28],edx
   18006825c:	mov    DWORD PTR [rsp+0x20],r8d
   180068261:	lea    r9,[rip+0x25d48]        # 0x18008dfb0    ;; "%02d:%02d:%02d.%03d"
   180068268:	mov    edx,0x10
   18006826d:	mov    r8,0xffffffffffffffff
   180068274:	lea    rcx,[rsp+0xf0]
   18006827c:	call   0x180069a50
   180068281:	movsd  xmm3,QWORD PTR [rip+0x1395a7]        # 0x1801a1830
   180068289:	movq   r9,xmm3
   18006828e:	mov    r8,QWORD PTR [rip+0x1395a3]        # 0x1801a1838
   180068295:	lea    rdx,[rsp+0xf0]
   18006829d:	lea    rcx,[rip+0x25dbc]        # 0x18008e060    ;; "  PERF  [%s] the country tags resolved %lld new label id(s) in %.0f ms during the game's 2D pass - a menu drawing worksh"
   1800682a4:	call   0x180053ef0
   1800682a9:	movsd  QWORD PTR [rip+0x13957f],xmm6        # 0x1801a1830
   1800682b1:	mov    QWORD PTR [rip+0x13957c],0x0        # 0x1801a1838
   1800682bc:	mov    eax,DWORD PTR [rip+0x13931a]        # 0x1801a15dc
   1800682c2:	mov    ecx,eax
   1800682c4:	inc    eax
   1800682c6:	mov    DWORD PTR [rip+0x139310],eax        # 0x1801a15dc
   1800682cc:	test   ecx,ecx
   1800682ce:	jg     0x1800682ef
   1800682d0:	mov    DWORD PTR [rip+0x1392fe],0x0        # 0x1801a15d8
   1800682da:	call   0x18003ae90
   1800682df:	movsd  QWORD PTR [rip+0x1392e9],xmm0        # 0x1801a15d0
   1800682e7:	movsd  QWORD PTR [rip+0x1392d9],xmm0        # 0x1801a15c8
   1800682ef:	mov    eax,DWORD PTR [rip+0x5c95b]        # 0x1800c4c50
   1800682f5:	mov    DWORD PTR [rsp+0xc0],eax
   1800682fc:	test   eax,eax
   1800682fe:	je     0x180068310
   180068300:	call   0x18001a060
   180068305:	movsd  QWORD PTR [rsp+0xc8],xmm0
   18006830e:	jmp    0x180068319
   180068310:	movsd  QWORD PTR [rsp+0xc8],xmm6
   180068319:	call   0x18003a4c0
   18006831e:	jmp    0x18006832d
   180068320:	mov    DWORD PTR [rip+0x2187d6],0x1        # 0x180280b00
   18006832a:	xorps  xmm6,xmm6
   18006832d:	cmp    DWORD PTR [rip+0x603f8],0x0        # 0x1800c872c
   180068334:	jne    0x18006844e
   18006833a:	call   QWORD PTR [rip+0x24fd8]        # 0x18008d318    ;; [USER32.dll!GetForegroundWindow]
   180068340:	test   rax,rax
   180068343:	jne    0x180068349
   180068345:	xor    ecx,ecx
   180068347:	jmp    0x18006836e
   180068349:	mov    DWORD PTR [rsp+0x5c],0x0
   180068351:	lea    rdx,[rsp+0x5c]
   180068356:	mov    rcx,rax
   180068359:	call   QWORD PTR [rip+0x24fa1]        # 0x18008d300    ;; [USER32.dll!GetWindowThreadProcessId]
   18006835f:	call   QWORD PTR [rip+0x24d73]        # 0x18008d0d8    ;; [KERNEL32.dll!GetCurrentProcessId]
   180068365:	xor    ecx,ecx
   180068367:	cmp    DWORD PTR [rsp+0x5c],eax
   18006836b:	sete   cl
   18006836e:	mov    DWORD PTR [rip+0x71058],ecx        # 0x1800d93cc
   180068374:	test   ecx,ecx
   180068376:	jne    0x180068382
   180068378:	call   0x18003afa0
   18006837d:	jmp    0x18006844e
   180068382:	cmp    DWORD PTR [rip+0x6ef4b],0x0        # 0x1800d72d4
   180068389:	je     0x180068398
   18006838b:	lea    rcx,[rip+0x5ca16]        # 0x1800c4da8
   180068392:	call   QWORD PTR [rip+0x24d20]        # 0x18008d0b8    ;; [KERNEL32.dll!EnterCriticalSection]
   180068398:	mov    eax,DWORD PTR [rip+0x5adba]        # 0x1800c3158
   18006839e:	mov    DWORD PTR [rsp+0xd0],eax
   1800683a5:	mov    eax,DWORD PTR [rip+0x21dcd5]        # 0x180286080
   1800683ab:	mov    DWORD PTR [rsp+0xd4],eax
   1800683b2:	mov    eax,DWORD PTR [rip+0x165660]        # 0x1801cda18
   1800683b8:	mov    DWORD PTR [rsp+0xd8],eax
   1800683bf:	mov    eax,DWORD PTR [rip+0x16564f]        # 0x1801cda14
   1800683c5:	mov    DWORD PTR [rsp+0xdc],eax
   1800683cc:	mov    eax,DWORD PTR [rip+0x11cae2]        # 0x180184eb4
   1800683d2:	mov    DWORD PTR [rsp+0xe0],eax
   1800683d9:	mov    eax,DWORD PTR [rip+0x16563d]        # 0x1801cda1c
   1800683df:	mov    DWORD PTR [rsp+0xe4],eax
   1800683e6:	mov    eax,DWORD PTR [rip+0x5c864]        # 0x1800c4c50
   1800683ec:	mov    DWORD PTR [rsp+0xe8],eax
   1800683f3:	call   0x18003afa0
   1800683f8:	jmp    0x18006842b
   1800683fa:	lea    rcx,[rip+0x33bcf]        # 0x18009bfd0    ;; "reput    hotkeys faulted - keys off"
   180068401:	mov    rax,QWORD PTR [rip+0x5c968]        # 0x1800c4d70
   180068408:	call   QWORD PTR [rax+0x38]
   18006840b:	mov    DWORD PTR [rip+0x60317],0x1        # 0x1800c872c
   180068415:	mov    rcx,QWORD PTR [rip+0x5adc4]        # 0x1800c31e0
   18006841c:	cmp    rcx,0xffffffffffffffff
   180068420:	je     0x180068428
   180068422:	call   QWORD PTR [rip+0x24c40]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   180068428:	xorps  xmm6,xmm6
   18006842b:	lea    rcx,[rsp+0xd0]
   180068433:	call   0x18001cb40
   180068438:	cmp    DWORD PTR [rip+0x6ee95],0x0        # 0x1800d72d4
   18006843f:	je     0x18006844e
   180068441:	lea    rcx,[rip+0x5c960]        # 0x1800c4da8
   180068448:	call   QWORD PTR [rip+0x24c72]        # 0x18008d0c0    ;; [KERNEL32.dll!LeaveCriticalSection]
   18006844e:	cmp    DWORD PTR [rip+0x139187],0x0        # 0x1801a15dc
   180068455:	jle    0x1800684b5
   180068457:	comisd xmm6,QWORD PTR [rip+0x5ae09]        # 0x1800c3268
   18006845f:	jae    0x1800684b5
   180068461:	call   0x18003ae90
   180068466:	movsxd rcx,DWORD PTR [rip+0x13916b]        # 0x1801a15d8
   18006846d:	lea    rdi,[rip+0xfffffffffff97b8c]        # 0x180000000
   180068474:	cmp    ecx,0x30
   180068477:	jge    0x1800684ab
   180068479:	lea    rdx,[rip+0x33b74]        # 0x18009bff4    ;; "keys"
   180068480:	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   180068488:	movaps xmm1,xmm0
   18006848b:	subsd  xmm1,QWORD PTR [rip+0x13913d]        # 0x1801a15d0
   180068493:	xorps  xmm2,xmm2
   180068496:	cvtsd2ss xmm2,xmm1
   18006849a:	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   1800684a3:	inc    ecx
   1800684a5:	mov    DWORD PTR [rip+0x13912d],ecx        # 0x1801a15d8
   1800684ab:	movsd  QWORD PTR [rip+0x13911d],xmm0        # 0x1801a15d0
   1800684b3:	jmp    0x1800684bc
   1800684b5:	lea    rdi,[rip+0xfffffffffff97b44]        # 0x180000000
   1800684bc:	call   0x18005cdb0
   1800684c1:	jmp    0x180068505
   1800684c3:	lea    rcx,[rip+0x33b46]        # 0x18009c010    ;; "reput    tick faulted - simulation off, windows still live"
   1800684ca:	mov    rax,QWORD PTR [rip+0x5c89f]        # 0x1800c4d70
   1800684d1:	call   QWORD PTR [rax+0x38]
   1800684d4:	mov    DWORD PTR [rip+0x218612],0x1        # 0x180280af0
   1800684de:	mov    DWORD PTR [rip+0x5ac88],0x1        # 0x1800c3170
   1800684e8:	mov    rcx,QWORD PTR [rip+0x5acf1]        # 0x1800c31e0
   1800684ef:	cmp    rcx,0xffffffffffffffff
   1800684f3:	je     0x1800684fb
   1800684f5:	call   QWORD PTR [rip+0x24b6d]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   1800684fb:	lea    rdi,[rip+0xfffffffffff97afe]        # 0x180000000
   180068502:	xorps  xmm6,xmm6
   180068505:	cmp    DWORD PTR [rip+0x1390d0],0x0        # 0x1801a15dc
   18006850c:	jle    0x180068563
   18006850e:	comisd xmm6,QWORD PTR [rip+0x5ad52]        # 0x1800c3268
   180068516:	jae    0x180068563
   180068518:	call   0x18003ae90
   18006851d:	movsxd rcx,DWORD PTR [rip+0x1390b4]        # 0x1801a15d8
   180068524:	cmp    ecx,0x30
   180068527:	jge    0x18006855b
   180068529:	lea    rdx,[rip+0x33b1c]        # 0x18009c04c    ;; "tick"
   180068530:	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   180068538:	movaps xmm1,xmm0
   18006853b:	subsd  xmm1,QWORD PTR [rip+0x13908d]        # 0x1801a15d0
   180068543:	xorps  xmm2,xmm2
   180068546:	cvtsd2ss xmm2,xmm1
   18006854a:	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   180068553:	inc    ecx
   180068555:	mov    DWORD PTR [rip+0x13907d],ecx        # 0x1801a15d8
   18006855b:	movsd  QWORD PTR [rip+0x13906d],xmm0        # 0x1801a15d0
   180068563:	mov    eax,DWORD PTR [rip+0x1664ff]        # 0x1801cea68
   180068569:	test   eax,eax
   18006856b:	jne    0x180068575
   18006856d:	mov    DWORD PTR [rip+0x166501],eax        # 0x1801cea74
   180068573:	jmp    0x1800685a6
   180068575:	mov    eax,DWORD PTR [rip+0x1664f9]        # 0x1801cea74
   18006857b:	inc    eax
   18006857d:	mov    DWORD PTR [rip+0x1664f1],eax        # 0x1801cea74
   180068583:	cmp    eax,0x78
   180068586:	jne    0x1800685a6
   180068588:	mov    edx,DWORD PTR [rip+0x1663fa]        # 0x1801ce988
   18006858e:	add    edx,DWORD PTR [rip+0x1663f0]        # 0x1801ce984
   180068594:	add    edx,DWORD PTR [rip+0x1663e6]        # 0x1801ce980
   18006859a:	lea    rcx,[rip+0x28faf]        # 0x180091550    ;; "  ARRIVE  %d waiting and the interface pass has not come in 120 frames. They are made at its first EndDraw, where the ga"
   1800685a1:	call   0x180053ef0
   1800685a6:	mov    esi,0x1
   1800685ab:	mov    ecx,esi
   1800685ad:	call   QWORD PTR [rip+0x24d25]        # 0x18008d2d8    ;; [USER32.dll!GetAsyncKeyState]
   1800685b3:	test   ax,ax
   1800685b6:	jns    0x1800685c9
   1800685b8:	mov    r14d,esi
   1800685bb:	cmp    DWORD PTR [rip+0x2187ca],0x0        # 0x180280d8c
   1800685c2:	jne    0x1800685cc
   1800685c4:	mov    r15d,esi
   1800685c7:	jmp    0x1800685cf
   1800685c9:	xor    r14d,r14d
   1800685cc:	xor    r15d,r15d
   1800685cf:	mov    DWORD PTR [rip+0x2187b6],r14d        # 0x180280d8c
   1800685d6:	lea    rcx,[rsp+0xd0]
   1800685de:	call   0x1800395a0
   1800685e3:	mov    DWORD PTR [rip+0x21f4c3],eax        # 0x180287aac
   1800685e9:	test   eax,eax
   1800685eb:	je     0x180068611
   1800685ed:	movss  xmm0,DWORD PTR [rsp+0xd0]
   1800685f6:	movss  DWORD PTR [rip+0x21f4a6],xmm0        # 0x180287aa4
   1800685fe:	movss  xmm1,DWORD PTR [rsp+0xd4]
   180068607:	movss  DWORD PTR [rip+0x21f499],xmm1        # 0x180287aa8
   18006860f:	jmp    0x180068621
   180068611:	movss  xmm0,DWORD PTR [rip+0x21f48b]        # 0x180287aa4
   180068619:	movss  xmm1,DWORD PTR [rip+0x21f487]        # 0x180287aa8
   180068621:	test   r14d,r14d
   180068624:	jne    0x18006862d
   180068626:	mov    DWORD PTR [rip+0x21874f],r14d        # 0x180280d7c
   18006862d:	test   r15d,r15d
   180068630:	je     0x18006865a
   180068632:	mov    DWORD PTR [rip+0x218758],esi        # 0x180280d90
   180068638:	mov    DWORD PTR [rip+0x218752],0x0        # 0x180280d94
   180068642:	mov    DWORD PTR [rip+0x21f470],eax        # 0x180287ab8
   180068648:	movss  DWORD PTR [rip+0x21f460],xmm0        # 0x180287ab0
   180068650:	movss  DWORD PTR [rip+0x21f45c],xmm1        # 0x180287ab4
   180068658:	jmp    0x180068685
   18006865a:	cmp    DWORD PTR [rip+0x21872f],0x0        # 0x180280d90
   180068661:	je     0x180068685
   180068663:	test   r14d,r14d
   180068666:	je     0x18006867b
   180068668:	mov    eax,DWORD PTR [rip+0x218726]        # 0x180280d94
   18006866e:	inc    eax
   180068670:	mov    DWORD PTR [rip+0x21871e],eax        # 0x180280d94
   180068676:	cmp    eax,0x3
   180068679:	jle    0x180068685
   18006867b:	mov    DWORD PTR [rip+0x21870b],0x0        # 0x180280d90
   180068685:	call   0x180058c20
   18006868a:	jmp    0x1800686a7
   18006868c:	lea    rcx,[rip+0x339dd]        # 0x18009c070    ;; "the placement faulted"
   180068693:	call   0x18002da30
   180068698:	lea    rdi,[rip+0xfffffffffff97961]        # 0x180000000
   18006869f:	mov    esi,0x1
   1800686a4:	xorps  xmm6,xmm6
   1800686a7:	cmp    DWORD PTR [rip+0x13b8fa],0x0        # 0x1801a3fa8
   1800686ae:	je     0x1800686ca
   1800686b0:	cmp    DWORD PTR [rip+0x14c051],0x0        # 0x1801b4708
   1800686b7:	jle    0x1800686ca
   1800686b9:	mov    eax,DWORD PTR [rip+0x13b911]        # 0x1801a3fd0
   1800686bf:	sub    eax,DWORD PTR [rip+0x14c047]        # 0x1801b470c
   1800686c5:	cmp    eax,0x2
   1800686c8:	jbe    0x1800686fc
   1800686ca:	cmp    DWORD PTR [rip+0x5ac1b],0x0        # 0x1800c32ec
   1800686d1:	je     0x1800686e5
   1800686d3:	cmp    DWORD PTR [rip+0x2186aa],0x0        # 0x180280d84
   1800686da:	je     0x1800686e5
   1800686dc:	cmp    DWORD PTR [rip+0x2186a5],0x0        # 0x180280d88
   1800686e3:	je     0x1800686e7
   1800686e5:	xor    esi,esi
   1800686e7:	mov    DWORD PTR [rip+0x21d99b],esi        # 0x180286088
   1800686ed:	call   0x180060e20
   1800686f2:	mov    DWORD PTR [rip+0x21d98c],0x0        # 0x180286088
   1800686fc:	cmp    DWORD PTR [rip+0x138ed9],0x0        # 0x1801a15dc
   180068703:	jle    0x18006875a
   180068705:	comisd xmm6,QWORD PTR [rip+0x5ab5b]        # 0x1800c3268
   18006870d:	jae    0x18006875a
   18006870f:	call   0x18003ae90
   180068714:	movsxd rcx,DWORD PTR [rip+0x138ebd]        # 0x1801a15d8
   18006871b:	cmp    ecx,0x30
   18006871e:	jge    0x180068752
   180068720:	lea    rdx,[rip+0x33961]        # 0x18009c088    ;; "panel place+draw"
   180068727:	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   18006872f:	movaps xmm1,xmm0
   180068732:	subsd  xmm1,QWORD PTR [rip+0x138e96]        # 0x1801a15d0
   18006873a:	xorps  xmm2,xmm2
   18006873d:	cvtsd2ss xmm2,xmm1
   180068741:	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   18006874a:	inc    ecx
   18006874c:	mov    DWORD PTR [rip+0x138e86],ecx        # 0x1801a15d8
   180068752:	movsd  QWORD PTR [rip+0x138e76],xmm0        # 0x1801a15d0
   18006875a:	call   0x18005abb0
   18006875f:	jmp    0x180068799
   180068761:	lea    rcx,[rip+0x33948]        # 0x18009c0b0    ;; "reput    tab append faulted - off"
   180068768:	mov    rax,QWORD PTR [rip+0x5c601]        # 0x1800c4d70
   18006876f:	call   QWORD PTR [rax+0x38]
   180068772:	mov    DWORD PTR [rip+0x2183b4],0x1        # 0x180280b30
   18006877c:	mov    rcx,QWORD PTR [rip+0x5aa5d]        # 0x1800c31e0
   180068783:	cmp    rcx,0xffffffffffffffff
   180068787:	je     0x18006878f
   180068789:	call   QWORD PTR [rip+0x248d9]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   18006878f:	lea    rdi,[rip+0xfffffffffff9786a]        # 0x180000000
   180068796:	xorps  xmm6,xmm6
   180068799:	cmp    DWORD PTR [rip+0x138e3c],0x0        # 0x1801a15dc
   1800687a0:	jle    0x1800687f7
   1800687a2:	comisd xmm6,QWORD PTR [rip+0x5aabe]        # 0x1800c3268
   1800687aa:	jae    0x1800687f7
   1800687ac:	call   0x18003ae90
   1800687b1:	movsxd rcx,DWORD PTR [rip+0x138e20]        # 0x1801a15d8
   1800687b8:	cmp    ecx,0x30
   1800687bb:	jge    0x1800687ef
   1800687bd:	lea    rdx,[rip+0x33914]        # 0x18009c0d8    ;; "bottom tab"
   1800687c4:	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   1800687cc:	movaps xmm1,xmm0
   1800687cf:	subsd  xmm1,QWORD PTR [rip+0x138df9]        # 0x1801a15d0
   1800687d7:	xorps  xmm2,xmm2
   1800687da:	cvtsd2ss xmm2,xmm1
   1800687de:	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   1800687e7:	inc    ecx
   1800687e9:	mov    DWORD PTR [rip+0x138de9],ecx        # 0x1801a15d8
   1800687ef:	movsd  QWORD PTR [rip+0x138dd9],xmm0        # 0x1801a15d0
   1800687f7:	call   0x180056b70
   1800687fc:	call   0x180057230
   180068801:	jmp    0x180068847
   180068803:	lea    rcx,[rip+0x338f6]        # 0x18009c100    ;; "reput    sidebar faulted - off"
   18006880a:	mov    rax,QWORD PTR [rip+0x5c55f]        # 0x1800c4d70
   180068811:	call   QWORD PTR [rax+0x38]
   180068814:	mov    DWORD PTR [rip+0x218562],0x0        # 0x180280d80
   18006881e:	lea    rcx,[rip+0x338fb]        # 0x18009c120    ;; "the sidebar faulted"
   180068825:	call   0x18002da30
   18006882a:	mov    rcx,QWORD PTR [rip+0x5a9af]        # 0x1800c31e0
   180068831:	cmp    rcx,0xffffffffffffffff
   180068835:	je     0x18006883d
   180068837:	call   QWORD PTR [rip+0x2482b]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   18006883d:	lea    rdi,[rip+0xfffffffffff977bc]        # 0x180000000
   180068844:	xorps  xmm6,xmm6
   180068847:	cmp    DWORD PTR [rip+0x138d8e],0x0        # 0x1801a15dc
   18006884e:	jle    0x1800688a5
   180068850:	comisd xmm6,QWORD PTR [rip+0x5aa10]        # 0x1800c3268
   180068858:	jae    0x1800688a5
   18006885a:	call   0x18003ae90
   18006885f:	movsxd rcx,DWORD PTR [rip+0x138d72]        # 0x1801a15d8
   180068866:	cmp    ecx,0x30
   180068869:	jge    0x18006889d
   18006886b:	lea    rdx,[rip+0x338c6]        # 0x18009c138    ;; "sidebar"
   180068872:	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   18006887a:	movaps xmm1,xmm0
   18006887d:	subsd  xmm1,QWORD PTR [rip+0x138d4b]        # 0x1801a15d0
   180068885:	xorps  xmm2,xmm2
   180068888:	cvtsd2ss xmm2,xmm1
   18006888c:	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   180068895:	inc    ecx
   180068897:	mov    DWORD PTR [rip+0x138d3b],ecx        # 0x1801a15d8
   18006889d:	movsd  QWORD PTR [rip+0x138d2b],xmm0        # 0x1801a15d0
   1800688a5:	lea    rcx,[rip+0x33894]        # 0x18009c140    ;; "the plugin's frame work (terrain pass)"
   1800688ac:	call   0x18003ac40
   1800688b1:	cmp    DWORD PTR [rsp+0xc0],0x0
   1800688b9:	je     0x18006894b
   1800688bf:	call   0x18001a060
   1800688c4:	subsd  xmm0,QWORD PTR [rsp+0xc8]
   1800688cd:	comisd xmm6,xmm0
   1800688d1:	ja     0x18006894b
   1800688d3:	comisd xmm0,QWORD PTR [rip+0x46ccd]        # 0x1800af5a8
   1800688db:	ja     0x18006894b
   1800688dd:	movsd  QWORD PTR [rip+0x3e0603],xmm0        # 0x180448ee8
   1800688e5:	comisd xmm0,QWORD PTR [rip+0x3e0603]        # 0x180448ef0
   1800688ed:	jbe    0x1800688f7
   1800688ef:	movsd  QWORD PTR [rip+0x3e05f9],xmm0        # 0x180448ef0
   1800688f7:	movsd  xmm1,QWORD PTR [rip+0x3e05f9]        # 0x180448ef8
   1800688ff:	addsd  xmm1,xmm0
   180068903:	movsd  QWORD PTR [rip+0x3e05ed],xmm1        # 0x180448ef8
   18006890b:	mov    eax,DWORD PTR [rip+0x3e05ef]        # 0x180448f00
   180068911:	inc    eax
   180068913:	mov    DWORD PTR [rip+0x3e05e7],eax        # 0x180448f00
   180068919:	cmp    eax,0x78
   18006891c:	jl     0x180068944
   18006891e:	movd   xmm0,eax
   180068922:	cvtdq2pd xmm0,xmm0
   180068926:	divsd  xmm1,xmm0
   18006892a:	movsd  QWORD PTR [rip+0x3e05d6],xmm1        # 0x180448f08
   180068932:	movsd  QWORD PTR [rip+0x3e05be],xmm6        # 0x180448ef8
   18006893a:	mov    DWORD PTR [rip+0x3e05bc],0x0        # 0x180448f00
   180068944:	inc    QWORD PTR [rip+0x3e05c5]        # 0x180448f10
   18006894b:	mov    rcx,QWORD PTR [rsp+0x100]
   180068953:	xor    rcx,rsp
   180068956:	call   0x180069e90
   18006895b:	movaps xmm6,XMMWORD PTR [rsp+0x110]
   180068963:	add    rsp,0x128
   18006896a:	pop    r15
   18006896c:	pop    r14
   18006896e:	pop    rdi
   18006896f:	pop    rsi
   180068970:	ret
