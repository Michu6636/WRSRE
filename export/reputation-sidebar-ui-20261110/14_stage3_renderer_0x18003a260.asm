   18003a260:	mov    QWORD PTR [rsp+0x8],rbx
   18003a265:	push   rdi
   18003a266:	sub    rsp,0x100
   18003a26d:	movaps XMMWORD PTR [rsp+0xf0],xmm6
   18003a275:	mov    rax,QWORD PTR [rip+0x89f04]        # 0x1800c4180
   18003a27c:	xor    rax,rsp
   18003a27f:	mov    QWORD PTR [rsp+0xe0],rax
   18003a287:	movss  xmm4,DWORD PTR [rip+0x892e9]        # 0x1800c3578
   18003a28f:	cmp    DWORD PTR [rip+0x24d816],0x0        # 0x180287aac
   18003a296:	je     0x18003a2e6
   18003a298:	movss  xmm1,DWORD PTR [rip+0x24d804]        # 0x180287aa4
   18003a2a0:	comiss xmm1,xmm4
   18003a2a3:	jb     0x18003a2e6
   18003a2a5:	movaps xmm0,xmm4
   18003a2a8:	addss  xmm0,DWORD PTR [rip+0x892d0]        # 0x1800c3580
   18003a2b0:	comiss xmm0,xmm1
   18003a2b3:	jb     0x18003a2e6
   18003a2b5:	movss  xmm1,DWORD PTR [rip+0x24d7eb]        # 0x180287aa8
   18003a2bd:	movss  xmm0,DWORD PTR [rip+0x892b7]        # 0x1800c357c
   18003a2c5:	comiss xmm1,xmm0
   18003a2c8:	jb     0x18003a2e6
   18003a2ca:	addss  xmm0,DWORD PTR [rip+0x892b2]        # 0x1800c3584
   18003a2d2:	comiss xmm0,xmm1
   18003a2d5:	jb     0x18003a2e6
   18003a2d7:	mov    edi,0x1
   18003a2dc:	movss  xmm0,DWORD PTR [rip+0x74f38]        # 0x1800af21c
   18003a2e4:	jmp    0x18003a2eb
   18003a2e6:	xorps  xmm0,xmm0
   18003a2e9:	xor    edi,edi
   18003a2eb:	addss  xmm0,DWORD PTR [rip+0x75051]        # 0x1800af344
   18003a2f3:	movss  xmm1,DWORD PTR [rip+0x75035]        # 0x1800af330
   18003a2fb:	movss  DWORD PTR [rsp+0x30],xmm1
   18003a301:	movss  DWORD PTR [rsp+0x28],xmm0
   18003a307:	lea    rax,[rip+0x89302]        # 0x1800c3610    ;; "editor/window_small.png"
   18003a30e:	mov    QWORD PTR [rsp+0x20],rax
   18003a313:	movss  xmm3,DWORD PTR [rip+0x89269]        # 0x1800c3584
   18003a31b:	movss  xmm2,DWORD PTR [rip+0x8925d]        # 0x1800c3580
   18003a323:	movss  xmm1,DWORD PTR [rip+0x89251]        # 0x1800c357c
   18003a32b:	movaps xmm0,xmm4
   18003a32e:	call   0x180039490
   18003a333:	nop
   18003a334:	movss  xmm1,DWORD PTR [rip+0x89248]        # 0x1800c3584
   18003a33c:	mulss  xmm1,DWORD PTR [rip+0x74ed4]        # 0x1800af218
   18003a344:	addss  xmm1,DWORD PTR [rip+0x89230]        # 0x1800c357c
   18003a34c:	movss  xmm0,DWORD PTR [rip+0x89224]        # 0x1800c3578
   18003a354:	addss  xmm0,DWORD PTR [rip+0x7531c]        # 0x1800af678
   18003a35c:	lea    r9,[rip+0x626a5]        # 0x18009ca08    ;; "REPUTATION"
   18003a363:	mov    r8d,0xffffffff
   18003a369:	call   0x180022c50
   18003a36e:	movss  xmm6,DWORD PTR [rip+0x8920e]        # 0x1800c3584
   18003a376:	mulss  xmm6,DWORD PTR [rip+0x74eea]        # 0x1800af268
   18003a37e:	addss  xmm6,DWORD PTR [rip+0x891f6]        # 0x1800c357c
   18003a386:	movsd  xmm0,QWORD PTR [rip+0x14da62]        # 0x180187df0
   18003a38e:	movsd  QWORD PTR [rsp+0x28],xmm0
   18003a394:	movsd  xmm1,QWORD PTR [rip+0x14da4c]        # 0x180187de8
   18003a39c:	movsd  QWORD PTR [rsp+0x20],xmm1
   18003a3a2:	lea    r9,[rip+0x6266f]        # 0x18009ca18    ;; "soviet %6.2f      west %6.2f"
   18003a3a9:	mov    edx,0x80
   18003a3ae:	mov    r8,0xffffffffffffffff
   18003a3b5:	lea    rcx,[rsp+0x60]
   18003a3ba:	call   0x180069a50
   18003a3bf:	movss  xmm0,DWORD PTR [rip+0x891b1]        # 0x1800c3578
   18003a3c7:	addss  xmm0,DWORD PTR [rip+0x752a9]        # 0x1800af678
   18003a3cf:	lea    r9,[rsp+0x60]
   18003a3d4:	mov    ebx,0xff2a2018
   18003a3d9:	mov    r8d,ebx
   18003a3dc:	movaps xmm1,xmm6
   18003a3df:	call   0x180022c50
   18003a3e4:	lea    rcx,[rip+0x6264d]        # 0x18009ca38    ;; "OVER"
   18003a3eb:	lea    rax,[rip+0x6264e]        # 0x18009ca40    ;; "outside"
   18003a3f2:	test   edi,edi
   18003a3f4:	cmovne rax,rcx
   18003a3f8:	mov    QWORD PTR [rsp+0x20],rax
   18003a3fd:	lea    r9,[rip+0x62644]        # 0x18009ca48    ;; "cursor is %s the panel"
   18003a404:	mov    edx,0x80
   18003a409:	mov    r8,0xffffffffffffffff
   18003a410:	lea    rcx,[rsp+0x60]
   18003a415:	call   0x180069a50
   18003a41a:	mov    eax,0xff206020
   18003a41f:	test   edi,edi
   18003a421:	cmovne ebx,eax
   18003a424:	addss  xmm6,DWORD PTR [rip+0x75238]        # 0x1800af664
   18003a42c:	movss  xmm0,DWORD PTR [rip+0x89144]        # 0x1800c3578
   18003a434:	addss  xmm0,DWORD PTR [rip+0x7523c]        # 0x1800af678
   18003a43c:	lea    r9,[rsp+0x60]
   18003a441:	mov    r8d,ebx
   18003a444:	movaps xmm1,xmm6
   18003a447:	call   0x180022c50
   18003a44c:	cmp    DWORD PTR [rip+0x2466b5],0x0        # 0x180280b08
   18003a453:	je     0x18003a497
   18003a455:	mov    DWORD PTR [rip+0x2466a9],0x0        # 0x180280b08
   18003a45f:	lea    rcx,[rip+0x625fa]        # 0x18009ca60    ;; "  panel  CLICK taken on the panel - the game did not see it"
   18003a466:	call   0x180053ef0
   18003a46b:	jmp    0x18003a497
   18003a46d:	lea    rcx,[rip+0x6256c]        # 0x18009c9e0    ;; "  panel  stage 3 faulted - panel off"
   18003a474:	call   0x180053ef0
   18003a479:	mov    DWORD PTR [rip+0x24667d],0x1        # 0x180280b00
   18003a483:	mov    rcx,QWORD PTR [rip+0x88d56]        # 0x1800c31e0
   18003a48a:	cmp    rcx,0xffffffffffffffff
   18003a48e:	je     0x18003a497
   18003a490:	call   QWORD PTR [rip+0x52bd2]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   18003a496:	nop
   18003a497:	mov    rcx,QWORD PTR [rsp+0xe0]
   18003a49f:	xor    rcx,rsp
   18003a4a2:	call   0x180069e90
   18003a4a7:	mov    rbx,QWORD PTR [rsp+0x110]
   18003a4af:	movaps xmm6,XMMWORD PTR [rsp+0xf0]
   18003a4b7:	add    rsp,0x100
   18003a4be:	pop    rdi
   18003a4bf:	ret
