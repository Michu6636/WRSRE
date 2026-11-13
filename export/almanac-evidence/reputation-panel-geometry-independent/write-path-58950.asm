
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180058958 <.text+0x57958>:
   180058958:	83 3d 8d a9 06 00 00 	cmp    DWORD PTR [rip+0x6a98d],0x0        # 0x1800c32ec
   18005895f:	0f 84 8f 02 00 00    	je     0x180058bf4
   180058965:	48 8b 1d 14 c4 06 00 	mov    rbx,QWORD PTR [rip+0x6c414]        # 0x1800c4d80
   18005896c:	ba 0c 00 00 00       	mov    edx,0xc
   180058971:	48 8d 8b 60 fd 9d 00 	lea    rcx,[rbx+0x9dfd60]
   180058978:	48 8b 05 f1 c3 06 00 	mov    rax,QWORD PTR [rip+0x6c3f1]        # 0x1800c4d70
   18005897f:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180058982:	85 c0                	test   eax,eax
   180058984:	0f 84 6a 02 00 00    	je     0x180058bf4
   18005898a:	f3 44 0f 10 83 60 fd 	movss  xmm8,DWORD PTR [rbx+0x9dfd60]
   180058991:	9d 00 
   180058993:	f3 44 0f 11 84 24 e8 	movss  DWORD PTR [rsp+0xe8],xmm8
   18005899a:	00 00 00 
   18005899d:	f3 0f 10 0d 17 6e 05 	movss  xmm1,DWORD PTR [rip+0x56e17]        # 0x1800af7bc
   1800589a4:	00 
   1800589a5:	44 0f 2f c1          	comiss xmm8,xmm1
   1800589a9:	0f 86 45 02 00 00    	jbe    0x180058bf4
   1800589af:	f3 0f 10 05 4d 6d 05 	movss  xmm0,DWORD PTR [rip+0x56d4d]        # 0x1800af704
   1800589b6:	00 
   1800589b7:	41 0f 2f c0          	comiss xmm0,xmm8
   1800589bb:	0f 86 33 02 00 00    	jbe    0x180058bf4
   1800589c1:	f3 44 0f 10 8b 68 fd 	movss  xmm9,DWORD PTR [rbx+0x9dfd68]
   1800589c8:	9d 00 
   1800589ca:	f3 44 0f 11 4c 24 54 	movss  DWORD PTR [rsp+0x54],xmm9
   1800589d1:	44 0f 2f c9          	comiss xmm9,xmm1
   1800589d5:	0f 86 19 02 00 00    	jbe    0x180058bf4
   1800589db:	41 0f 2f c1          	comiss xmm0,xmm9
   1800589df:	0f 86 0f 02 00 00    	jbe    0x180058bf4
   1800589e5:	f3 0f 10 b3 64 fd 9d 	movss  xmm6,DWORD PTR [rbx+0x9dfd64]
   1800589ec:	00 
   1800589ed:	f3 0f 11 74 24 50    	movss  DWORD PTR [rsp+0x50],xmm6
   1800589f3:	0f 2f 35 be 6d 05 00 	comiss xmm6,DWORD PTR [rip+0x56dbe]        # 0x1800af7b8
   1800589fa:	0f 86 f4 01 00 00    	jbe    0x180058bf4
   180058a00:	0f 2f c6             	comiss xmm0,xmm6
   180058a03:	0f 86 eb 01 00 00    	jbe    0x180058bf4
   180058a09:	0f 57 ff             	xorps  xmm7,xmm7
   180058a0c:	0f 2f fe             	comiss xmm7,xmm6
   180058a0f:	76 08                	jbe    0x180058a19
   180058a11:	85 ff                	test   edi,edi
   180058a13:	0f 84 db 01 00 00    	je     0x180058bf4
   180058a19:	e8 22 88 00 00       	call   0x180061240
   180058a1e:	44 0f 28 d0          	movaps xmm10,xmm0
   180058a22:	f3 44 0f 11 94 24 e0 	movss  DWORD PTR [rsp+0xe0],xmm10
   180058a29:	00 00 00 
   180058a2c:	48 8b 1d 4d c3 06 00 	mov    rbx,QWORD PTR [rip+0x6c34d]        # 0x1800c4d80
   180058a33:	ba 04 00 00 00       	mov    edx,0x4
   180058a38:	48 8d 8b 74 52 99 00 	lea    rcx,[rbx+0x995274]
   180058a3f:	48 8b 05 2a c3 06 00 	mov    rax,QWORD PTR [rip+0x6c32a]        # 0x1800c4d70
   180058a46:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180058a49:	85 c0                	test   eax,eax
   180058a4b:	74 19                	je     0x180058a66
   180058a4d:	8b 8b 74 52 99 00    	mov    ecx,DWORD PTR [rbx+0x995274]
   180058a53:	8d 41 ff             	lea    eax,[rcx-0x1]
   180058a56:	3d fe 7f 00 00       	cmp    eax,0x7ffe
   180058a5b:	77 09                	ja     0x180058a66
   180058a5d:	66 0f 6e d1          	movd   xmm2,ecx
   180058a61:	0f 5b d2             	cvtdq2ps xmm2,xmm2
   180058a64:	eb 0c                	jmp    0x180058a72
   180058a66:	41 0f 28 d0          	movaps xmm2,xmm8
   180058a6a:	f3 0f 58 15 7a 6c 05 	addss  xmm2,DWORD PTR [rip+0x56c7a]        # 0x1800af6ec
   180058a71:	00 
   180058a72:	f3 0f 10 0d 76 a8 06 	movss  xmm1,DWORD PTR [rip+0x6a876]        # 0x1800c32f0
   180058a79:	00 
   180058a7a:	f3 0f 11 8c 24 d8 00 	movss  DWORD PTR [rsp+0xd8],xmm1
   180058a81:	00 00 
   180058a83:	0f 2f cf             	comiss xmm1,xmm7
   180058a86:	76 21                	jbe    0x180058aa9
   180058a88:	f3 0f 10 05 3c 6c 05 	movss  xmm0,DWORD PTR [rip+0x56c3c]        # 0x1800af6cc
   180058a8f:	00 
   180058a90:	f3 0f 5f c1          	maxss  xmm0,xmm1
   180058a94:	f3 0f 10 0d 60 6c 05 	movss  xmm1,DWORD PTR [rip+0x56c60]        # 0x1800af6fc
   180058a9b:	00 
   180058a9c:	f3 0f 5d c8          	minss  xmm1,xmm0
   180058aa0:	f3 0f 11 8c 24 d8 00 	movss  DWORD PTR [rsp+0xd8],xmm1
   180058aa7:	00 00 
   180058aa9:	f3 0f 10 05 4b a8 06 	movss  xmm0,DWORD PTR [rip+0x6a84b]        # 0x1800c32fc
   180058ab0:	00 
   180058ab1:	f3 41 0f 59 c2       	mulss  xmm0,xmm10
   180058ab6:	f3 0f 5c d0          	subss  xmm2,xmm0
   180058aba:	f3 0f 10 1d e2 b4 14 	movss  xmm3,DWORD PTR [rip+0x14b4e2]        # 0x1801a3fa4
   180058ac1:	00 
   180058ac2:	0f 2f fb             	comiss xmm7,xmm3
   180058ac5:	72 36                	jb     0x180058afd
   180058ac7:	8b 05 8b a6 06 00    	mov    eax,DWORD PTR [rip+0x6a68b]        # 0x1800c3158
   180058acd:	f7 d8                	neg    eax
   180058acf:	48 1b c9             	sbb    rcx,rcx
   180058ad2:	48 8d 05 27 75 fa ff 	lea    rax,[rip+0xfffffffffffa7527]        # 0x180000000
   180058ad9:	83 bc 88 38 0e 28 00 	cmp    DWORD PTR [rax+rcx*4+0x280e38],0x0
   180058ae0:	00 
   180058ae1:	74 17                	je     0x180058afa
   180058ae3:	41 0f 28 da          	movaps xmm3,xmm10
   180058ae7:	f3 0f 59 1d 99 6b 05 	mulss  xmm3,DWORD PTR [rip+0x56b99]        # 0x1800af688
   180058aee:	00 
   180058aef:	f3 0f 58 9c 88 28 0e 	addss  xmm3,DWORD PTR [rax+rcx*4+0x280e28]
   180058af6:	28 00 
   180058af8:	eb 03                	jmp    0x180058afd
   180058afa:	0f 28 da             	movaps xmm3,xmm2
   180058afd:	41 0f 28 e0          	movaps xmm4,xmm8
   180058b01:	f3 0f 58 25 c7 6b 05 	addss  xmm4,DWORD PTR [rip+0x56bc7]        # 0x1800af6d0
   180058b08:	00 
   180058b09:	f3 0f 5d d3          	minss  xmm2,xmm3
   180058b0d:	f3 0f 5f e2          	maxss  xmm4,xmm2
   180058b11:	f3 0f 11 64 24 58    	movss  DWORD PTR [rsp+0x58],xmm4
   180058b17:	48 8b 05 62 c2 06 00 	mov    rax,QWORD PTR [rip+0x6c262]        # 0x1800c4d80
   180058b1e:	0f 2f cf             	comiss xmm1,xmm7
   180058b21:	76 08                	jbe    0x180058b2b
   180058b23:	f3 0f 11 88 64 fd 9d 	movss  DWORD PTR [rax+0x9dfd64],xmm1
   180058b2a:	00 
   180058b2b:	f3 0f 11 a0 68 fd 9d 	movss  DWORD PTR [rax+0x9dfd68],xmm4
   180058b32:	00 
   180058b33:	b8 01 00 00 00       	mov    eax,0x1
   180058b38:	89 44 24 4c          	mov    DWORD PTR [rsp+0x4c],eax
   180058b3c:	eb 39                	jmp    0x180058b77
