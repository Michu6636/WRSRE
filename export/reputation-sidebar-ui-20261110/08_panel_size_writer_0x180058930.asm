   180058930:	mov    rax,rsp
   180058933:	mov    QWORD PTR [rax+0x8],rbx
   180058937:	push   rdi
   180058938:	sub    rsp,0xc0
   18005893f:	movaps XMMWORD PTR [rax-0x18],xmm6
   180058943:	movaps XMMWORD PTR [rax-0x28],xmm7
   180058947:	movaps XMMWORD PTR [rax-0x38],xmm8
   18005894c:	movaps XMMWORD PTR [rax-0x48],xmm9
   180058951:	movaps XMMWORD PTR [rax-0x58],xmm10
   180058956:	mov    edi,ecx
   180058958:	cmp    DWORD PTR [rip+0x6a98d],0x0        # 0x1800c32ec
   18005895f:	je     0x180058bf4
   180058965:	mov    rbx,QWORD PTR [rip+0x6c414]        # 0x1800c4d80
   18005896c:	mov    edx,0xc
   180058971:	lea    rcx,[rbx+0x9dfd60]
   180058978:	mov    rax,QWORD PTR [rip+0x6c3f1]        # 0x1800c4d70
   18005897f:	call   QWORD PTR [rax+0x60]
   180058982:	test   eax,eax
   180058984:	je     0x180058bf4
   18005898a:	movss  xmm8,DWORD PTR [rbx+0x9dfd60]
   180058993:	movss  DWORD PTR [rsp+0xe8],xmm8
   18005899d:	movss  xmm1,DWORD PTR [rip+0x56e17]        # 0x1800af7bc
   1800589a5:	comiss xmm8,xmm1
   1800589a9:	jbe    0x180058bf4
   1800589af:	movss  xmm0,DWORD PTR [rip+0x56d4d]        # 0x1800af704
   1800589b7:	comiss xmm0,xmm8
   1800589bb:	jbe    0x180058bf4
   1800589c1:	movss  xmm9,DWORD PTR [rbx+0x9dfd68]
   1800589ca:	movss  DWORD PTR [rsp+0x54],xmm9
   1800589d1:	comiss xmm9,xmm1
   1800589d5:	jbe    0x180058bf4
   1800589db:	comiss xmm0,xmm9
   1800589df:	jbe    0x180058bf4
   1800589e5:	movss  xmm6,DWORD PTR [rbx+0x9dfd64]
   1800589ed:	movss  DWORD PTR [rsp+0x50],xmm6
   1800589f3:	comiss xmm6,DWORD PTR [rip+0x56dbe]        # 0x1800af7b8
   1800589fa:	jbe    0x180058bf4
   180058a00:	comiss xmm0,xmm6
   180058a03:	jbe    0x180058bf4
   180058a09:	xorps  xmm7,xmm7
   180058a0c:	comiss xmm7,xmm6
   180058a0f:	jbe    0x180058a19
   180058a11:	test   edi,edi
   180058a13:	je     0x180058bf4
   180058a19:	call   0x180061240
   180058a1e:	movaps xmm10,xmm0
   180058a22:	movss  DWORD PTR [rsp+0xe0],xmm10
   180058a2c:	mov    rbx,QWORD PTR [rip+0x6c34d]        # 0x1800c4d80
   180058a33:	mov    edx,0x4
   180058a38:	lea    rcx,[rbx+0x995274]
   180058a3f:	mov    rax,QWORD PTR [rip+0x6c32a]        # 0x1800c4d70
   180058a46:	call   QWORD PTR [rax+0x60]
   180058a49:	test   eax,eax
   180058a4b:	je     0x180058a66
   180058a4d:	mov    ecx,DWORD PTR [rbx+0x995274]
   180058a53:	lea    eax,[rcx-0x1]
   180058a56:	cmp    eax,0x7ffe
   180058a5b:	ja     0x180058a66
   180058a5d:	movd   xmm2,ecx
   180058a61:	cvtdq2ps xmm2,xmm2
   180058a64:	jmp    0x180058a72
   180058a66:	movaps xmm2,xmm8
   180058a6a:	addss  xmm2,DWORD PTR [rip+0x56c7a]        # 0x1800af6ec
   180058a72:	movss  xmm1,DWORD PTR [rip+0x6a876]        # 0x1800c32f0
   180058a7a:	movss  DWORD PTR [rsp+0xd8],xmm1
   180058a83:	comiss xmm1,xmm7
   180058a86:	jbe    0x180058aa9
   180058a88:	movss  xmm0,DWORD PTR [rip+0x56c3c]        # 0x1800af6cc
   180058a90:	maxss  xmm0,xmm1
   180058a94:	movss  xmm1,DWORD PTR [rip+0x56c60]        # 0x1800af6fc
   180058a9c:	minss  xmm1,xmm0
   180058aa0:	movss  DWORD PTR [rsp+0xd8],xmm1
   180058aa9:	movss  xmm0,DWORD PTR [rip+0x6a84b]        # 0x1800c32fc
   180058ab1:	mulss  xmm0,xmm10
   180058ab6:	subss  xmm2,xmm0
   180058aba:	movss  xmm3,DWORD PTR [rip+0x14b4e2]        # 0x1801a3fa4
   180058ac2:	comiss xmm7,xmm3
   180058ac5:	jb     0x180058afd
   180058ac7:	mov    eax,DWORD PTR [rip+0x6a68b]        # 0x1800c3158
   180058acd:	neg    eax
   180058acf:	sbb    rcx,rcx
   180058ad2:	lea    rax,[rip+0xfffffffffffa7527]        # 0x180000000
   180058ad9:	cmp    DWORD PTR [rax+rcx*4+0x280e38],0x0
   180058ae1:	je     0x180058afa
   180058ae3:	movaps xmm3,xmm10
   180058ae7:	mulss  xmm3,DWORD PTR [rip+0x56b99]        # 0x1800af688
   180058aef:	addss  xmm3,DWORD PTR [rax+rcx*4+0x280e28]
   180058af8:	jmp    0x180058afd
   180058afa:	movaps xmm3,xmm2
   180058afd:	movaps xmm4,xmm8
   180058b01:	addss  xmm4,DWORD PTR [rip+0x56bc7]        # 0x1800af6d0
   180058b09:	minss  xmm2,xmm3
   180058b0d:	maxss  xmm4,xmm2
   180058b11:	movss  DWORD PTR [rsp+0x58],xmm4
   180058b17:	mov    rax,QWORD PTR [rip+0x6c262]        # 0x1800c4d80
   180058b1e:	comiss xmm1,xmm7
   180058b21:	jbe    0x180058b2b
   180058b23:	movss  DWORD PTR [rax+0x9dfd64],xmm1
   180058b2b:	movss  DWORD PTR [rax+0x9dfd68],xmm4
   180058b33:	mov    eax,0x1
   180058b38:	mov    DWORD PTR [rsp+0x4c],eax
   180058b3c:	jmp    0x180058b77
   180058b3e:	xor    eax,eax
   180058b40:	mov    DWORD PTR [rsp+0x4c],eax
   180058b44:	movss  xmm1,DWORD PTR [rsp+0xd8]
   180058b4d:	xorps  xmm7,xmm7
   180058b50:	movss  xmm10,DWORD PTR [rsp+0xe0]
   180058b5a:	movss  xmm8,DWORD PTR [rsp+0xe8]
   180058b64:	movss  xmm6,DWORD PTR [rsp+0x50]
   180058b6a:	movss  xmm9,DWORD PTR [rsp+0x54]
   180058b71:	movss  xmm4,DWORD PTR [rsp+0x58]
   180058b77:	test   eax,eax
   180058b79:	jne    0x180058b89
   180058b7b:	lea    rcx,[rip+0x46276]        # 0x18009edf8    ;; "sizing the panel faulted"
   180058b82:	call   0x18002da30
   180058b87:	jmp    0x180058bf4
   180058b89:	cmp    DWORD PTR [rip+0x14b434],0x0        # 0x1801a3fc4
   180058b90:	jne    0x180058bf4
   180058b92:	mov    DWORD PTR [rip+0x14b428],0x1        # 0x1801a3fc4
   180058b9c:	comiss xmm1,xmm7
   180058b9f:	jbe    0x180058ba6
   180058ba1:	movaps xmm2,xmm1
   180058ba4:	jmp    0x180058ba9
   180058ba6:	movaps xmm2,xmm6
   180058ba9:	ja     0x180058bae
   180058bab:	movaps xmm1,xmm6
   180058bae:	cvtps2pd xmm0,xmm8
   180058bb2:	cvtps2pd xmm4,xmm4
   180058bb5:	cvtps2pd xmm5,xmm9
   180058bb9:	mulss  xmm2,xmm10
   180058bbe:	cvtps2pd xmm3,xmm2
   180058bc1:	cvtps2pd xmm2,xmm1
   180058bc4:	cvtps2pd xmm1,xmm6
   180058bc7:	movsd  QWORD PTR [rsp+0x30],xmm0
   180058bcd:	movsd  QWORD PTR [rsp+0x28],xmm4
   180058bd3:	movsd  QWORD PTR [rsp+0x20],xmm5
   180058bd9:	movq   r9,xmm3
   180058bde:	movq   r8,xmm2
   180058be3:	movq   rdx,xmm1
   180058be8:	lea    rcx,[rip+0x46231]        # 0x18009ee20    ;; "  side  panel sized: right edge %.0f -> %.0f (%.0f px), bottom %.0f -> %.0f px, top %.0f px"
   180058bef:	call   0x180053ef0
   180058bf4:	lea    r11,[rsp+0xc0]
   180058bfc:	mov    rbx,QWORD PTR [r11+0x10]
   180058c00:	movaps xmm6,XMMWORD PTR [r11-0x10]
   180058c05:	movaps xmm7,XMMWORD PTR [r11-0x20]
   180058c0a:	movaps xmm8,XMMWORD PTR [r11-0x30]
   180058c0f:	movaps xmm9,XMMWORD PTR [r11-0x40]
   180058c14:	movaps xmm10,XMMWORD PTR [r11-0x50]
   180058c19:	mov    rsp,r11
   180058c1c:	pop    rdi
   180058c1d:	ret
