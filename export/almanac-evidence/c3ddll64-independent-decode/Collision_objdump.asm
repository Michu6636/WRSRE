
/tmp/wrsre_Collision.bin:     file format binary


Disassembly of section .data:

00000001800bc690 <.data>:
   1800bc690:	48 8b c4             	mov    rax,rsp
   1800bc693:	41 56                	push   r14
   1800bc695:	41 57                	push   r15
   1800bc697:	48 81 ec f8 00 00 00 	sub    rsp,0xf8
   1800bc69e:	f3 0f 10 a9 3c 67 00 	movss  xmm5,DWORD PTR [rcx+0x673c]
   1800bc6a5:	00
   1800bc6a6:	4c 8b fa             	mov    r15,rdx
   1800bc6a9:	0f 2f 2d 38 c5 0c 00 	comiss xmm5,DWORD PTR [rip+0xcc538]        # 0x180188be8
   1800bc6b0:	4c 8b f1             	mov    r14,rcx
   1800bc6b3:	f3 0f 10 89 40 67 00 	movss  xmm1,DWORD PTR [rcx+0x6740]
   1800bc6ba:	00
   1800bc6bb:	f3 0f 10 a1 44 67 00 	movss  xmm4,DWORD PTR [rcx+0x6744]
   1800bc6c2:	00
   1800bc6c3:	0f 29 70 c8          	movaps XMMWORD PTR [rax-0x38],xmm6
   1800bc6c7:	f3 0f 10 b1 4c 67 00 	movss  xmm6,DWORD PTR [rcx+0x674c]
   1800bc6ce:	00
   1800bc6cf:	44 0f 29 50 88       	movaps XMMWORD PTR [rax-0x78],xmm10
   1800bc6d4:	44 0f 28 d3          	movaps xmm10,xmm3
   1800bc6d8:	44 0f 29 98 78 ff ff 	movaps XMMWORD PTR [rax-0x88],xmm11
   1800bc6df:	ff
   1800bc6e0:	44 0f 28 da          	movaps xmm11,xmm2
   1800bc6e4:	76 24                	jbe    0x1800bc70a
   1800bc6e6:	f3 0f 10 81 34 67 00 	movss  xmm0,DWORD PTR [rcx+0x6734]
   1800bc6ed:	00
   1800bc6ee:	f3 0f 5c c8          	subss  xmm1,xmm0
   1800bc6f2:	f3 0f 5c e0          	subss  xmm4,xmm0
   1800bc6f6:	f3 0f 5e f5          	divss  xmm6,xmm5
   1800bc6fa:	f3 0f 5e cd          	divss  xmm1,xmm5
   1800bc6fe:	f3 0f 5e e5          	divss  xmm4,xmm5
   1800bc702:	f3 0f 58 c8          	addss  xmm1,xmm0
   1800bc706:	f3 0f 58 e0          	addss  xmm4,xmm0
   1800bc70a:	f3 0f 10 42 04       	movss  xmm0,DWORD PTR [rdx+0x4]
   1800bc70f:	0f 2f c8             	comiss xmm1,xmm0
   1800bc712:	0f 87 73 04 00 00    	ja     0x1800bcb8b
   1800bc718:	0f 2f c4             	comiss xmm0,xmm4
   1800bc71b:	0f 87 6a 04 00 00    	ja     0x1800bcb8b
   1800bc721:	48 89 9c 24 20 01 00 	mov    QWORD PTR [rsp+0x120],rbx
   1800bc728:	00
   1800bc729:	f3 0f 5c c6          	subss  xmm0,xmm6
   1800bc72d:	48 89 ac 24 28 01 00 	mov    QWORD PTR [rsp+0x128],rbp
   1800bc734:	00
   1800bc735:	48 89 b4 24 f0 00 00 	mov    QWORD PTR [rsp+0xf0],rsi
   1800bc73c:	00
   1800bc73d:	48 89 bc 24 e8 00 00 	mov    QWORD PTR [rsp+0xe8],rdi
   1800bc744:	00
   1800bc745:	4c 89 a4 24 e0 00 00 	mov    QWORD PTR [rsp+0xe0],r12
   1800bc74c:	00
   1800bc74d:	45 33 e4             	xor    r12d,r12d
   1800bc750:	0f 29 bc 24 c0 00 00 	movaps XMMWORD PTR [rsp+0xc0],xmm7
   1800bc757:	00
   1800bc758:	f3 0f 11 42 04       	movss  DWORD PTR [rdx+0x4],xmm0
   1800bc75d:	48 8d 91 c4 02 00 00 	lea    rdx,[rcx+0x2c4]
   1800bc764:	f3 0f 10 b9 8c 02 00 	movss  xmm7,DWORD PTR [rcx+0x28c]
   1800bc76b:	00
   1800bc76c:	f3 0f 59 3d 18 c4 0c 	mulss  xmm7,DWORD PTR [rip+0xcc418]        # 0x180188b8c
   1800bc773:	00
   1800bc774:	44 0f 29 84 24 b0 00 	movaps XMMWORD PTR [rsp+0xb0],xmm8
   1800bc77b:	00 00
   1800bc77d:	f3 44 0f 10 81 88 02 	movss  xmm8,DWORD PTR [rcx+0x288]
   1800bc784:	00 00
   1800bc786:	f3 44 0f 59 05 fd c3 	mulss  xmm8,DWORD PTR [rip+0xcc3fd]        # 0x180188b8c
   1800bc78d:	0c 00
   1800bc78f:	0f 28 f7             	movaps xmm6,xmm7
   1800bc792:	44 89 a1 cc 02 00 00 	mov    DWORD PTR [rcx+0x2cc],r12d
   1800bc799:	44 0f 29 8c 24 a0 00 	movaps XMMWORD PTR [rsp+0xa0],xmm9
   1800bc7a0:	00 00
   1800bc7a2:	f3 41 0f 5c f2       	subss  xmm6,xmm10
   1800bc7a7:	44 0f 29 64 24 70    	movaps XMMWORD PTR [rsp+0x70],xmm12
   1800bc7ad:	45 0f 28 cb          	movaps xmm9,xmm11
   1800bc7b1:	44 0f 29 6c 24 60    	movaps XMMWORD PTR [rsp+0x60],xmm13
   1800bc7b7:	f3 45 0f 5c c8       	subss  xmm9,xmm8
   1800bc7bc:	44 0f 29 74 24 50    	movaps XMMWORD PTR [rsp+0x50],xmm14
   1800bc7c2:	44 0f 29 7c 24 40    	movaps XMMWORD PTR [rsp+0x40],xmm15
   1800bc7c8:	f3 0f 11 b1 c8 02 00 	movss  DWORD PTR [rcx+0x2c8],xmm6
   1800bc7cf:	00
   1800bc7d0:	f3 44 0f 11 89 c4 02 	movss  DWORD PTR [rcx+0x2c4],xmm9
   1800bc7d7:	00 00
   1800bc7d9:	e8 a2 c4 ff ff       	call   0x1800b8c80
   1800bc7de:	f3 45 0f 5c c3       	subss  xmm8,xmm11
   1800bc7e3:	f3 41 0f 11 b6 d4 02 	movss  DWORD PTR [r14+0x2d4],xmm6
   1800bc7ea:	00 00
   1800bc7ec:	49 8d 96 d0 02 00 00 	lea    rdx,[r14+0x2d0]
   1800bc7f3:	45 89 a6 d8 02 00 00 	mov    DWORD PTR [r14+0x2d8],r12d
   1800bc7fa:	49 8b ce             	mov    rcx,r14
   1800bc7fd:	f3 45 0f 11 86 d0 02 	movss  DWORD PTR [r14+0x2d0],xmm8
   1800bc804:	00 00
   1800bc806:	e8 75 c4 ff ff       	call   0x1800b8c80
   1800bc80b:	f3 44 0f 5c d7       	subss  xmm10,xmm7
   1800bc810:	f3 45 0f 11 86 dc 02 	movss  DWORD PTR [r14+0x2dc],xmm8
   1800bc817:	00 00
   1800bc819:	49 8d 96 dc 02 00 00 	lea    rdx,[r14+0x2dc]
   1800bc820:	45 89 a6 e4 02 00 00 	mov    DWORD PTR [r14+0x2e4],r12d
   1800bc827:	49 8b ce             	mov    rcx,r14
   1800bc82a:	f3 45 0f 11 96 e0 02 	movss  DWORD PTR [r14+0x2e0],xmm10
   1800bc831:	00 00
   1800bc833:	e8 48 c4 ff ff       	call   0x1800b8c80
   1800bc838:	49 8d 96 e8 02 00 00 	lea    rdx,[r14+0x2e8]
   1800bc83f:	f3 45 0f 11 8e e8 02 	movss  DWORD PTR [r14+0x2e8],xmm9
   1800bc846:	00 00
   1800bc848:	49 8b ce             	mov    rcx,r14
   1800bc84b:	f3 45 0f 11 96 ec 02 	movss  DWORD PTR [r14+0x2ec],xmm10
   1800bc852:	00 00
   1800bc854:	45 89 a6 f0 02 00 00 	mov    DWORD PTR [r14+0x2f0],r12d
   1800bc85b:	e8 20 c4 ff ff       	call   0x1800b8c80
   1800bc860:	f3 41 0f 10 17       	movss  xmm2,DWORD PTR [r15]
   1800bc865:	f3 41 0f 10 4f 04    	movss  xmm1,DWORD PTR [r15+0x4]
   1800bc86b:	f3 41 0f 10 86 c4 02 	movss  xmm0,DWORD PTR [r14+0x2c4]
   1800bc872:	00 00
   1800bc874:	f3 45 0f 10 ae d0 02 	movss  xmm13,DWORD PTR [r14+0x2d0]
   1800bc87b:	00 00
   1800bc87d:	f3 0f 5c c2          	subss  xmm0,xmm2
   1800bc881:	f3 41 0f 10 b6 dc 02 	movss  xmm6,DWORD PTR [r14+0x2dc]
   1800bc888:	00 00
   1800bc88a:	f3 44 0f 5c ea       	subss  xmm13,xmm2
   1800bc88f:	f3                   	repz
