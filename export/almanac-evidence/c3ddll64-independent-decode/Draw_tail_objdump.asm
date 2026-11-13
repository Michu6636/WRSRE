
/tmp/wrsre_draw_tail.bin:     file format binary


Disassembly of section .data:

00000001800bb400 <.data>:
   1800bb400:	0f 10 87 48 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x248]
   1800bb407:	8d 04 85 01 00 00 00 	lea    eax,[rax*4+0x1]
   1800bb40e:	48 63 c8             	movsxd rcx,eax
   1800bb411:	48 81 c1 71 02 00 00 	add    rcx,0x271
   1800bb418:	48 03 c9             	add    rcx,rcx
   1800bb41b:	0f 11 04 cf          	movups XMMWORD PTR [rdi+rcx*8],xmm0
   1800bb41f:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb425:	8d 04 85 01 00 00 00 	lea    eax,[rax*4+0x1]
   1800bb42c:	48 63 c8             	movsxd rcx,eax
   1800bb42f:	48 81 c1 71 04 00 00 	add    rcx,0x471
   1800bb436:	48 03 c9             	add    rcx,rcx
   1800bb439:	0f 11 0c cf          	movups XMMWORD PTR [rdi+rcx*8],xmm1
   1800bb43d:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb443:	0f 10 87 18 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x218]
   1800bb44a:	8d 04 85 02 00 00 00 	lea    eax,[rax*4+0x2]
   1800bb451:	48 63 c8             	movsxd rcx,eax
   1800bb454:	48 83 c1 71          	add    rcx,0x71
   1800bb458:	48 03 c9             	add    rcx,rcx
   1800bb45b:	0f 11 04 cf          	movups XMMWORD PTR [rdi+rcx*8],xmm0
   1800bb45f:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb465:	0f 10 87 58 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x258]
   1800bb46c:	8d 04 85 02 00 00 00 	lea    eax,[rax*4+0x2]
   1800bb473:	48 63 c8             	movsxd rcx,eax
   1800bb476:	48 81 c1 71 02 00 00 	add    rcx,0x271
   1800bb47d:	48 03 c9             	add    rcx,rcx
   1800bb480:	0f 11 04 cf          	movups XMMWORD PTR [rdi+rcx*8],xmm0
   1800bb484:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb48a:	8d 04 85 02 00 00 00 	lea    eax,[rax*4+0x2]
   1800bb491:	48 63 c8             	movsxd rcx,eax
   1800bb494:	48 81 c1 71 04 00 00 	add    rcx,0x471
   1800bb49b:	48 03 c9             	add    rcx,rcx
   1800bb49e:	0f 11 0c cf          	movups XMMWORD PTR [rdi+rcx*8],xmm1
   1800bb4a2:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb4a8:	0f 10 87 28 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x228]
   1800bb4af:	8d 04 85 03 00 00 00 	lea    eax,[rax*4+0x3]
   1800bb4b6:	48 63 c8             	movsxd rcx,eax
   1800bb4b9:	48 83 c1 71          	add    rcx,0x71
   1800bb4bd:	48 03 c9             	add    rcx,rcx
   1800bb4c0:	0f 11 04 cf          	movups XMMWORD PTR [rdi+rcx*8],xmm0
   1800bb4c4:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb4ca:	0f 10 87 68 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x268]
   1800bb4d1:	8d 04 85 03 00 00 00 	lea    eax,[rax*4+0x3]
   1800bb4d8:	48 63 c8             	movsxd rcx,eax
   1800bb4db:	48 81 c1 71 02 00 00 	add    rcx,0x271
   1800bb4e2:	48 03 c9             	add    rcx,rcx
   1800bb4e5:	0f 11 04 cf          	movups XMMWORD PTR [rdi+rcx*8],xmm0
   1800bb4e9:	8b 87 08 03 00 00    	mov    eax,DWORD PTR [rdi+0x308]
   1800bb4ef:	8d 04 85 03 00 00 00 	lea    eax,[rax*4+0x3]
   1800bb4f6:	48 63 c8             	movsxd rcx,eax
   1800bb4f9:	48 81 c1 71 04 00 00 	add    rcx,0x471
   1800bb500:	48 03 c9             	add    rcx,rcx
   1800bb503:	0f 11 0c cf          	movups XMMWORD PTR [rdi+rcx*8],xmm1
   1800bb507:	ff 87 08 03 00 00    	inc    DWORD PTR [rdi+0x308]
   1800bb50d:	81 bf 08 03 00 00 80 	cmp    DWORD PTR [rdi+0x308],0x80
   1800bb514:	00 00 00
   1800bb517:	0f 85 a3 01 00 00    	jne    0x1800bb6c0
   1800bb51d:	44 38 af 60 67 00 00 	cmp    BYTE PTR [rdi+0x6760],r13b
   1800bb524:	74 08                	je     0x1800bb52e
   1800bb526:	48 8b cf             	mov    rcx,rdi
   1800bb529:	e8 f2 01 00 00       	call   0x1800bb720
   1800bb52e:	44 89 af 08 03 00 00 	mov    DWORD PTR [rdi+0x308],r13d
   1800bb535:	e9 86 01 00 00       	jmp    0x1800bb6c0
   1800bb53a:	0f 10 87 ac 02 00 00 	movups xmm0,XMMWORD PTR [rdi+0x2ac]
   1800bb541:	f3 0f 10 97 b0 02 00 	movss  xmm2,DWORD PTR [rdi+0x2b0]
   1800bb548:	00
   1800bb549:	f3 0f 10 9f b4 02 00 	movss  xmm3,DWORD PTR [rdi+0x2b4]
   1800bb550:	00
   1800bb551:	f3 0f 10 a7 b8 02 00 	movss  xmm4,DWORD PTR [rdi+0x2b8]
   1800bb558:	00
   1800bb559:	f3 0f 11 44 24 30    	movss  DWORD PTR [rsp+0x30],xmm0
   1800bb55f:	f3 0f 11 44 24 40    	movss  DWORD PTR [rsp+0x40],xmm0
   1800bb565:	f3 0f 11 54 24 34    	movss  DWORD PTR [rsp+0x34],xmm2
   1800bb56b:	f3 0f 11 5c 24 38    	movss  DWORD PTR [rsp+0x38],xmm3
   1800bb571:	f3 0f 11 64 24 3c    	movss  DWORD PTR [rsp+0x3c],xmm4
   1800bb577:	0f 10 44 24 30       	movups xmm0,XMMWORD PTR [rsp+0x30]
   1800bb57c:	f3 0f 11 54 24 44    	movss  DWORD PTR [rsp+0x44],xmm2
   1800bb582:	0f 11 44 24 50       	movups XMMWORD PTR [rsp+0x50],xmm0
   1800bb587:	0f 11 44 24 60       	movups XMMWORD PTR [rsp+0x60],xmm0
   1800bb58c:	0f 11 44 24 70       	movups XMMWORD PTR [rsp+0x70],xmm0
   1800bb591:	f3 0f 11 5c 24 48    	movss  DWORD PTR [rsp+0x48],xmm3
   1800bb597:	f3 0f 11 64 24 4c    	movss  DWORD PTR [rsp+0x4c],xmm4
   1800bb59d:	44 38 ad 80 00 00 00 	cmp    BYTE PTR [rbp+0x80],r13b
   1800bb5a4:	74 5d                	je     0x1800bb603
   1800bb5a6:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb5a9:	48 8d 15 78 5c 0c 00 	lea    rdx,[rip+0xc5c78]        # 0x180181228
   1800bb5b0:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb5b4:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb5b7:	ff 50 50             	call   QWORD PTR [rax+0x50]
   1800bb5ba:	48 8b 0f             	mov    rcx,QWORD PTR [rdi]
   1800bb5bd:	4c 8d 44 24 40       	lea    r8,[rsp+0x40]
   1800bb5c2:	41 b9 04 00 00 00    	mov    r9d,0x4
   1800bb5c8:	48 8b d0             	mov    rdx,rax
   1800bb5cb:	48 8b 49 38          	mov    rcx,QWORD PTR [rcx+0x38]
   1800bb5cf:	4c 8b 11             	mov    r10,QWORD PTR [rcx]
   1800bb5d2:	41 ff 52 70          	call   QWORD PTR [r10+0x70]
   1800bb5d6:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb5d9:	48 8d 15 38 5c 0c 00 	lea    rdx,[rip+0xc5c38]        # 0x180181218
   1800bb5e0:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb5e4:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb5e7:	ff 50 50             	call   QWORD PTR [rax+0x50]
   1800bb5ea:	48 8b 0f             	mov    rcx,QWORD PTR [rdi]
   1800bb5ed:	48 8b d0             	mov    rdx,rax
   1800bb5f0:	f3 0f 10 97 bc 02 00 	movss  xmm2,DWORD PTR [rdi+0x2bc]
   1800bb5f7:	00
   1800bb5f8:	48 8b 49 38          	mov    rcx,QWORD PTR [rcx+0x38]
   1800bb5fc:	4c 8b 09             	mov    r9,QWORD PTR [rcx]
   1800bb5ff:	41 ff 51 58          	call   QWORD PTR [r9+0x58]
   1800bb603:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb606:	48 8d 15 73 5c 0c 00 	lea    rdx,[rip+0xc5c73]        # 0x180181280
   1800bb60d:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb611:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb614:	ff 50 50             	call   QWORD PTR [rax+0x50]
   1800bb617:	48 8b 0f             	mov    rcx,QWORD PTR [rdi]
   1800bb61a:	4c 8d 87 f8 01 00 00 	lea    r8,[rdi+0x1f8]
   1800bb621:	41 b9 04 00 00 00    	mov    r9d,0x4
   1800bb627:	48 8b d0             	mov    rdx,rax
   1800bb62a:	48 8b 49 38          	mov    rcx,QWORD PTR [rcx+0x38]
   1800bb62e:	4c 8b 11             	mov    r10,QWORD PTR [rcx]
   1800bb631:	41 ff 52 70          	call   QWORD PTR [r10+0x70]
   1800bb635:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb638:	48 8d 15 31 5c 0c 00 	lea    rdx,[rip+0xc5c31]        # 0x180181270
   1800bb63f:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb643:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb646:	ff 50 50             	call   QWORD PTR [rax+0x50]
   1800bb649:	48 8b 0f             	mov    rcx,QWORD PTR [rdi]
   1800bb64c:	4c 8d 87 38 02 00 00 	lea    r8,[rdi+0x238]
   1800bb653:	41 b9 04 00 00 00    	mov    r9d,0x4
   1800bb659:	48 8b d0             	mov    rdx,rax
   1800bb65c:	48 8b 49 38          	mov    rcx,QWORD PTR [rcx+0x38]
   1800bb660:	4c 8b 11             	mov    r10,QWORD PTR [rcx]
   1800bb663:	41 ff 52 70          	call   QWORD PTR [r10+0x70]
   1800bb667:	44 38 2d 75 10 12 00 	cmp    BYTE PTR [rip+0x121075],r13b        # 0x1801dc6e3
   1800bb66e:	75 14                	jne    0x1800bb684
   1800bb670:	48 8b 4f 08          	mov    rcx,QWORD PTR [rdi+0x8]
   1800bb674:	41 83 c9 ff          	or     r9d,0xffffffff
   1800bb678:	33 d2                	xor    edx,edx
   1800bb67a:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb67d:	45 8d 41 05          	lea    r8d,[r9+0x5]
   1800bb681:	ff 50 20             	call   QWORD PTR [rax+0x20]
   1800bb684:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb687:	83 ca ff             	or     edx,0xffffffff
   1800bb68a:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb68e:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb691:	ff 50 28             	call   QWORD PTR [rax+0x28]
   1800bb694:	44 38 af 60 67 00 00 	cmp    BYTE PTR [rdi+0x6760],r13b
   1800bb69b:	74 23                	je     0x1800bb6c0
   1800bb69d:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb6a0:	41 b9 02 00 00 00    	mov    r9d,0x2
   1800bb6a6:	45 33 c0             	xor    r8d,r8d
   1800bb6a9:	c7 44 24 20 ff ff ff 	mov    DWORD PTR [rsp+0x20],0xffffffff
   1800bb6b0:	ff
   1800bb6b1:	41 8b d1             	mov    edx,r9d
   1800bb6b4:	48 8b 08             	mov    rcx,QWORD PTR [rax]
   1800bb6b7:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb6ba:	ff 90 a8 00 00 00    	call   QWORD PTR [rax+0xa8]
   1800bb6c0:	48 8b 4d 80          	mov    rcx,QWORD PTR [rbp-0x80]
   1800bb6c4:	48 33 cc             	xor    rcx,rsp
   1800bb6c7:	e8 14 b9 0a 00       	call   0x180166fe0
   1800bb6cc:	4c 8d 9c 24 20 01 00 	lea    r11,[rsp+0x120]
   1800bb6d3:	00
   1800bb6d4:	49 8b 5b 38          	mov    rbx,QWORD PTR [r11+0x38]
   1800bb6d8:	49 8b 73 40          	mov    rsi,QWORD PTR [r11+0x40]
   1800bb6dc:	49 8b 7b 48          	mov    rdi,QWORD PTR [r11+0x48]
   1800bb6e0:	41 0f 28 73 f0       	movaps xmm6,XMMWORD PTR [r11-0x10]
   1800bb6e5:	41 0f 28 7b e0       	movaps xmm7,XMMWORD PTR [r11-0x20]
   1800bb6ea:	45 0f 28 43 d0       	movaps xmm8,XMMWORD PTR [r11-0x30]
   1800bb6ef:	45 0f 28 4b c0       	movaps xmm9,XMMWORD PTR [r11-0x40]
   1800bb6f4:	45 0f 28 53 b0       	movaps xmm10,XMMWORD PTR [r11-0x50]
   1800bb6f9:	45 0f 28 5b a0       	movaps xmm11,XMMWORD PTR [r11-0x60]
   1800bb6fe:	45 0f 28 63 90       	movaps xmm12,XMMWORD PTR [r11-0x70]
   1800bb703:	45 0f 28 6b 80       	movaps xmm13,XMMWORD PTR [r11-0x80]
   1800bb708:	45 0f 28 b3 70 ff ff 	movaps xmm14,XMMWORD PTR [r11-0x90]
   1800bb70f:	ff
   1800bb710:	49 8b e3             	mov    rsp,r11
   1800bb713:	41 5f                	pop    r15
   1800bb715:	41 5e                	pop    r14
   1800bb717:	41 5d                	pop    r13
   1800bb719:	41 5c                	pop    r12
   1800bb71b:	5d                   	pop    rbp
   1800bb71c:	c3                   	ret
   1800bb71d:	cc                   	int3
   1800bb71e:	cc                   	int3
   1800bb71f:	cc                   	int3
   1800bb720:	40 57                	rex push rdi
   1800bb722:	48 83 ec 30          	sub    rsp,0x30
   1800bb726:	80 b9 0d 03 00 00 00 	cmp    BYTE PTR [rcx+0x30d],0x0
   1800bb72d:	48 8b f9             	mov    rdi,rcx
   1800bb730:	0f 84 a4 01 00 00    	je     0x1800bb8da
   1800bb736:	48 8b 49 08          	mov    rcx,QWORD PTR [rcx+0x8]
   1800bb73a:	41 83 c9 ff          	or     r9d,0xffffffff
   1800bb73e:	33 d2                	xor    edx,edx
   1800bb740:	48 89 6c 24 48       	mov    QWORD PTR [rsp+0x48],rbp
   1800bb745:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb748:	45 8d 41 05          	lea    r8d,[r9+0x5]
   1800bb74c:	ff 50 20             	call   QWORD PTR [rax+0x20]
   1800bb74f:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb752:	48 8d 15 07 5b 0c 00 	lea    rdx,[rip+0xc5b07]        # 0x180181260
   1800bb759:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb75d:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb760:	ff 50 50             	call   QWORD PTR [rax+0x50]
   1800bb763:	48 8b 0f             	mov    rcx,QWORD PTR [rdi]
   1800bb766:	0f 57 d2             	xorps  xmm2,xmm2
   1800bb769:	48 8b d0             	mov    rdx,rax
   1800bb76c:	48 8b 49 38          	mov    rcx,QWORD PTR [rcx+0x38]
   1800bb770:	4c 8b 01             	mov    r8,QWORD PTR [rcx]
   1800bb773:	41 ff 50 58          	call   QWORD PTR [r8+0x58]
   1800bb777:	48 8b 8f 10 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6710]
   1800bb77e:	48 8d 97 10 07 00 00 	lea    rdx,[rdi+0x710]
   1800bb785:	44 8b 87 08 03 00 00 	mov    r8d,DWORD PTR [rdi+0x308]
   1800bb78c:	41 c1 e0 06          	shl    r8d,0x6
   1800bb790:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb793:	ff 50 28             	call   QWORD PTR [rax+0x28]
   1800bb796:	48 8b 8f 18 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6718]
   1800bb79d:	48 8d 97 10 27 00 00 	lea    rdx,[rdi+0x2710]
   1800bb7a4:	44 8b 87 08 03 00 00 	mov    r8d,DWORD PTR [rdi+0x308]
   1800bb7ab:	41 c1 e0 06          	shl    r8d,0x6
   1800bb7af:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb7b2:	ff 50 28             	call   QWORD PTR [rax+0x28]
   1800bb7b5:	48 8b 8f 20 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6720]
   1800bb7bc:	48 8d 97 10 47 00 00 	lea    rdx,[rdi+0x4710]
   1800bb7c3:	44 8b 87 08 03 00 00 	mov    r8d,DWORD PTR [rdi+0x308]
   1800bb7ca:	41 c1 e0 06          	shl    r8d,0x6
   1800bb7ce:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb7d1:	ff 50 28             	call   QWORD PTR [rax+0x28]
   1800bb7d4:	33 ed                	xor    ebp,ebp
   1800bb7d6:	39 af 08 03 00 00    	cmp    DWORD PTR [rdi+0x308],ebp
   1800bb7dc:	0f 8e f3 00 00 00    	jle    0x1800bb8d5
   1800bb7e2:	48 89 5c 24 40       	mov    QWORD PTR [rsp+0x40],rbx
   1800bb7e7:	48 89 74 24 50       	mov    QWORD PTR [rsp+0x50],rsi
   1800bb7ec:	48 8d b7 10 03 00 00 	lea    rsi,[rdi+0x310]
   1800bb7f3:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
   1800bb7f7:	66 0f 1f 84 00 00 00 	nop    WORD PTR [rax+rax*1+0x0]
   1800bb7fe:	00 00
   1800bb800:	48 8b 0e             	mov    rcx,QWORD PTR [rsi]
   1800bb803:	33 d2                	xor    edx,edx
   1800bb805:	4c 8b 07             	mov    r8,QWORD PTR [rdi]
   1800bb808:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb80b:	4d 8b 40 38          	mov    r8,QWORD PTR [r8+0x38]
   1800bb80f:	ff 50 70             	call   QWORD PTR [rax+0x70]
   1800bb812:	85 ed                	test   ebp,ebp
   1800bb814:	74 0b                	je     0x1800bb821
   1800bb816:	7e 19                	jle    0x1800bb831
   1800bb818:	48 8b 46 f8          	mov    rax,QWORD PTR [rsi-0x8]
   1800bb81c:	48 39 06             	cmp    QWORD PTR [rsi],rax
   1800bb81f:	74 10                	je     0x1800bb831
   1800bb821:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb824:	83 ca ff             	or     edx,0xffffffff
   1800bb827:	48 8b 48 38          	mov    rcx,QWORD PTR [rax+0x38]
   1800bb82b:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb82e:	ff 50 28             	call   QWORD PTR [rax+0x28]
   1800bb831:	48 8b 8f 10 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6710]
   1800bb838:	45 33 c9             	xor    r9d,r9d
   1800bb83b:	8b dd                	mov    ebx,ebp
   1800bb83d:	c7 44 24 20 ff ff ff 	mov    DWORD PTR [rsp+0x20],0xffffffff
   1800bb844:	ff
   1800bb845:	c1 e3 06             	shl    ebx,0x6
   1800bb848:	8b d3                	mov    edx,ebx
   1800bb84a:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb84d:	45 8d 41 40          	lea    r8d,[r9+0x40]
   1800bb851:	ff 50 30             	call   QWORD PTR [rax+0x30]
   1800bb854:	48 8b 8f 18 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6718]
   1800bb85b:	41 b9 01 00 00 00    	mov    r9d,0x1
   1800bb861:	8b d3                	mov    edx,ebx
   1800bb863:	c7 44 24 20 ff ff ff 	mov    DWORD PTR [rsp+0x20],0xffffffff
   1800bb86a:	ff
   1800bb86b:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb86e:	45 8d 41 3f          	lea    r8d,[r9+0x3f]
   1800bb872:	ff 50 30             	call   QWORD PTR [rax+0x30]
   1800bb875:	48 8b 8f 20 67 00 00 	mov    rcx,QWORD PTR [rdi+0x6720]
   1800bb87c:	41 b9 02 00 00 00    	mov    r9d,0x2
   1800bb882:	8b d3                	mov    edx,ebx
   1800bb884:	c7 44 24 20 ff ff ff 	mov    DWORD PTR [rsp+0x20],0xffffffff
   1800bb88b:	ff
   1800bb88c:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb88f:	45 8d 41 3e          	lea    r8d,[r9+0x3e]
   1800bb893:	ff 50 30             	call   QWORD PTR [rax+0x30]
   1800bb896:	48 8b 07             	mov    rax,QWORD PTR [rdi]
   1800bb899:	41 b9 02 00 00 00    	mov    r9d,0x2
   1800bb89f:	45 33 c0             	xor    r8d,r8d
   1800bb8a2:	c7 44 24 20 ff ff ff 	mov    DWORD PTR [rsp+0x20],0xffffffff
   1800bb8a9:	ff
   1800bb8aa:	41 8b d1             	mov    edx,r9d
   1800bb8ad:	48 8b 08             	mov    rcx,QWORD PTR [rax]
   1800bb8b0:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bb8b3:	ff 90 a8 00 00 00    	call   QWORD PTR [rax+0xa8]
   1800bb8b9:	ff c5                	inc    ebp
   1800bb8bb:	48 83 c6 08          	add    rsi,0x8
   1800bb8bf:	3b af 08 03 00 00    	cmp    ebp,DWORD PTR [rdi+0x308]
   1800bb8c5:	0f 8c 35 ff ff ff    	jl     0x1800bb800
   1800bb8cb:	48 8b 74 24 50       	mov    rsi,QWORD PTR [rsp+0x50]
   1800bb8d0:	48 8b 5c 24 40       	mov    rbx,QWORD PTR [rsp+0x40]
   1800bb8d5:	48 8b 6c 24 48       	mov    rbp,QWORD PTR [rsp+0x48]
   1800bb8da:	48 83 c4 30          	add    rsp,0x30
   1800bb8de:	5f                   	pop    rdi
   1800bb8df:	c3                   	ret
   1800bb8e0:	48 8b c4             	mov    rax,rsp
   1800bb8e3:	48 89 58 10          	mov    QWORD PTR [rax+0x10],rbx
   1800bb8e7:	48 89 68 18          	mov    QWORD PTR [rax+0x18],rbp
   1800bb8eb:	48 89 70 20          	mov    QWORD PTR [rax+0x20],rsi
   1800bb8ef:	57                   	push   rdi
   1800bb8f0:	41 56                	push   r14
   1800bb8f2:	41 57                	push   r15
   1800bb8f4:	48 81 ec 20 01 00 00 	sub    rsp,0x120
   1800bb8fb:	0f 29 70 d8          	movaps XMMWORD PTR [rax-0x28],xmm6
   1800bb8ff:	0f                   	.byte 0xf
