Static GNU objdump; string consumer, hash as documented

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180022c50 <.text+0x21c50>:
   180022c50:	48 8b c4             	mov    rax,rsp
   180022c53:	53                   	push   rbx
   180022c54:	48 81 ec 30 02 00 00 	sub    rsp,0x230
   180022c5b:	0f 29 78 d8          	movaps XMMWORD PTR [rax-0x28],xmm7
   180022c5f:	44 0f 29 40 c8       	movaps XMMWORD PTR [rax-0x38],xmm8
   180022c64:	48 8b 05 15 15 0a 00 	mov    rax,QWORD PTR [rip+0xa1515]        # 0x1800c4180
   180022c6b:	48 33 c4             	xor    rax,rsp
   180022c6e:	48 89 84 24 f0 01 00 	mov    QWORD PTR [rsp+0x1f0],rax
   180022c75:	00
   180022c76:	33 d2                	xor    edx,edx
   180022c78:	41 8b d8             	mov    ebx,r8d
   180022c7b:	44 0f 28 c1          	movaps xmm8,xmm1
   180022c7f:	0f 28 f8             	movaps xmm7,xmm0
   180022c82:	41 38 11             	cmp    BYTE PTR [r9],dl
   180022c85:	74 21                	je     0x180022ca8
   180022c87:	81 fa db 00 00 00    	cmp    edx,0xdb
   180022c8d:	7d 19                	jge    0x180022ca8
   180022c8f:	48 63 ca             	movsxd rcx,edx
   180022c92:	ff c2                	inc    edx
   180022c94:	41 0f b6 04 09       	movzx  eax,BYTE PTR [r9+rcx*1]
   180022c99:	66 89 44 4c 30       	mov    WORD PTR [rsp+rcx*2+0x30],ax
   180022c9e:	48 63 c2             	movsxd rax,edx
   180022ca1:	41 80 3c 01 00       	cmp    BYTE PTR [r9+rax*1],0x0
   180022ca6:	75 df                	jne    0x180022c87
   180022ca8:	48 63 c2             	movsxd rax,edx
   180022cab:	48 8d 0c 00          	lea    rcx,[rax+rax*1]
   180022caf:	48 81 f9 b8 01 00 00 	cmp    rcx,0x1b8
   180022cb6:	0f 83 28 03 00 00    	jae    0x180022fe4
   180022cbc:	33 c0                	xor    eax,eax
   180022cbe:	0f 29 b4 24 20 02 00 	movaps XMMWORD PTR [rsp+0x220],xmm6
   180022cc5:	00
   180022cc6:	66 89 44 0c 30       	mov    WORD PTR [rsp+rcx*1+0x30],ax
   180022ccb:	8b 05 23 0a 0a 00    	mov    eax,DWORD PTR [rip+0xa0a23]        # 0x1800c36f4
   180022cd1:	83 f8 02             	cmp    eax,0x2
   180022cd4:	0f 8c 28 01 00 00    	jl     0x180022e02
   180022cda:	4c 8b 0d d7 66 0b 00 	mov    r9,QWORD PTR [rip+0xb66d7]        # 0x1800d93b8
   180022ce1:	4c 8b 05 98 20 0a 00 	mov    r8,QWORD PTR [rip+0xa2098]        # 0x1800c4d80
   180022ce8:	4d 85 c9             	test   r9,r9
   180022ceb:	0f 84 7b 01 00 00    	je     0x180022e6c
   180022cf1:	8b 05 91 04 0a 00    	mov    eax,DWORD PTR [rip+0xa0491]        # 0x1800c3188
   180022cf7:	f3 0f 10 35 c9 c6 08 	movss  xmm6,DWORD PTR [rip+0x8c6c9]        # 0x1800af3c8
   180022cfe:	00
   180022cff:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022d03:	48 85 d2             	test   rdx,rdx
   180022d06:	74 37                	je     0x180022d3f
   180022d08:	8b 0d 76 04 0a 00    	mov    ecx,DWORD PTR [rip+0xa0476]        # 0x1800c3184
   180022d0e:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
   180022d13:	0f 28 d7             	movaps xmm2,xmm7
   180022d16:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180022d1b:	f3 0f 5c d6          	subss  xmm2,xmm6
   180022d1f:	c7 44 24 20 00 00 00 	mov    DWORD PTR [rsp+0x20],0xff000000
   180022d26:	ff
   180022d27:	49 03 c8             	add    rcx,r8
   180022d2a:	41 0f 28 d8          	movaps xmm3,xmm8
   180022d2e:	41 ff d1             	call   r9
   180022d31:	4c 8b 05 48 20 0a 00 	mov    r8,QWORD PTR [rip+0xa2048]        # 0x1800c4d80
   180022d38:	4c 8b 0d 79 66 0b 00 	mov    r9,QWORD PTR [rip+0xb6679]        # 0x1800d93b8
   180022d3f:	4d 85 c9             	test   r9,r9
   180022d42:	0f 84 24 01 00 00    	je     0x180022e6c
   180022d48:	8b 05 3a 04 0a 00    	mov    eax,DWORD PTR [rip+0xa043a]        # 0x1800c3188
   180022d4e:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022d52:	48 85 d2             	test   rdx,rdx
   180022d55:	74 37                	je     0x180022d8e
   180022d57:	8b 0d 27 04 0a 00    	mov    ecx,DWORD PTR [rip+0xa0427]        # 0x1800c3184
   180022d5d:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
   180022d62:	0f 28 d7             	movaps xmm2,xmm7
   180022d65:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180022d6a:	f3 0f 58 d6          	addss  xmm2,xmm6
   180022d6e:	c7 44 24 20 00 00 00 	mov    DWORD PTR [rsp+0x20],0xff000000
   180022d75:	ff
   180022d76:	49 03 c8             	add    rcx,r8
   180022d79:	41 0f 28 d8          	movaps xmm3,xmm8
   180022d7d:	41 ff d1             	call   r9
   180022d80:	4c 8b 05 f9 1f 0a 00 	mov    r8,QWORD PTR [rip+0xa1ff9]        # 0x1800c4d80
   180022d87:	4c 8b 0d 2a 66 0b 00 	mov    r9,QWORD PTR [rip+0xb662a]        # 0x1800d93b8
   180022d8e:	4d 85 c9             	test   r9,r9
   180022d91:	0f 84 d5 00 00 00    	je     0x180022e6c
   180022d97:	8b 05 eb 03 0a 00    	mov    eax,DWORD PTR [rip+0xa03eb]        # 0x1800c3188
   180022d9d:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022da1:	48 85 d2             	test   rdx,rdx
   180022da4:	74 37                	je     0x180022ddd
   180022da6:	8b 0d d8 03 0a 00    	mov    ecx,DWORD PTR [rip+0xa03d8]        # 0x1800c3184
   180022dac:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
   180022db1:	41 0f 28 d8          	movaps xmm3,xmm8
   180022db5:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180022dba:	f3 0f 5c de          	subss  xmm3,xmm6
   180022dbe:	c7 44 24 20 00 00 00 	mov    DWORD PTR [rsp+0x20],0xff000000
   180022dc5:	ff
   180022dc6:	49 03 c8             	add    rcx,r8
   180022dc9:	0f 28 d7             	movaps xmm2,xmm7
   180022dcc:	41 ff d1             	call   r9
   180022dcf:	4c 8b 05 aa 1f 0a 00 	mov    r8,QWORD PTR [rip+0xa1faa]        # 0x1800c4d80
   180022dd6:	4c 8b 0d db 65 0b 00 	mov    r9,QWORD PTR [rip+0xb65db]        # 0x1800d93b8
   180022ddd:	4d 85 c9             	test   r9,r9
   180022de0:	0f 84 86 00 00 00    	je     0x180022e6c
   180022de6:	8b 05 9c 03 0a 00    	mov    eax,DWORD PTR [rip+0xa039c]        # 0x1800c3188
   180022dec:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022df0:	48 85 d2             	test   rdx,rdx
   180022df3:	74 77                	je     0x180022e6c
   180022df5:	41 0f 28 d8          	movaps xmm3,xmm8
   180022df9:	0f 28 d7             	movaps xmm2,xmm7
   180022dfc:	f3 0f 58 de          	addss  xmm3,xmm6
   180022e00:	eb 3e                	jmp    0x180022e40
   180022e02:	83 f8 01             	cmp    eax,0x1
   180022e05:	75 57                	jne    0x180022e5e
   180022e07:	4c 8b 0d aa 65 0b 00 	mov    r9,QWORD PTR [rip+0xb65aa]        # 0x1800d93b8
   180022e0e:	4c 8b 05 6b 1f 0a 00 	mov    r8,QWORD PTR [rip+0xa1f6b]        # 0x1800c4d80
   180022e15:	4d 85 c9             	test   r9,r9
   180022e18:	74 52                	je     0x180022e6c
   180022e1a:	8b 05 68 03 0a 00    	mov    eax,DWORD PTR [rip+0xa0368]        # 0x1800c3188
   180022e20:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022e24:	48 85 d2             	test   rdx,rdx
   180022e27:	74 43                	je     0x180022e6c
   180022e29:	f3 0f 10 05 97 c5 08 	movss  xmm0,DWORD PTR [rip+0x8c597]        # 0x1800af3c8
   180022e30:	00
   180022e31:	41 0f 28 d8          	movaps xmm3,xmm8
   180022e35:	0f 28 d7             	movaps xmm2,xmm7
   180022e38:	f3 0f 58 d8          	addss  xmm3,xmm0
   180022e3c:	f3 0f 58 d0          	addss  xmm2,xmm0
   180022e40:	8b 0d 3e 03 0a 00    	mov    ecx,DWORD PTR [rip+0xa033e]        # 0x1800c3184
   180022e46:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
   180022e4b:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180022e50:	49 03 c8             	add    rcx,r8
   180022e53:	c7 44 24 20 00 00 00 	mov    DWORD PTR [rsp+0x20],0xff000000
   180022e5a:	ff
   180022e5b:	41 ff d1             	call   r9
   180022e5e:	4c 8b 05 1b 1f 0a 00 	mov    r8,QWORD PTR [rip+0xa1f1b]        # 0x1800c4d80
   180022e65:	4c 8b 0d 4c 65 0b 00 	mov    r9,QWORD PTR [rip+0xb654c]        # 0x1800d93b8
   180022e6c:	48 63 15 29 df 25 00 	movsxd rdx,DWORD PTR [rip+0x25df29]        # 0x180280d9c
   180022e73:	0f 28 b4 24 20 02 00 	movaps xmm6,XMMWORD PTR [rsp+0x220]
   180022e7a:	00
   180022e7b:	8d 42 ff             	lea    eax,[rdx-0x1]
   180022e7e:	83 f8 01             	cmp    eax,0x1
   180022e81:	0f 87 fe 00 00 00    	ja     0x180022f85
   180022e87:	33 c0                	xor    eax,eax
   180022e89:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
   180022e90:	66 83 7c 44 30 00    	cmp    WORD PTR [rsp+rax*2+0x30],0x0
   180022e96:	74 3d                	je     0x180022ed5
   180022e98:	66 83 7c 44 32 00    	cmp    WORD PTR [rsp+rax*2+0x32],0x0
   180022e9e:	74 33                	je     0x180022ed3
   180022ea0:	66 83 7c 44 34 00    	cmp    WORD PTR [rsp+rax*2+0x34],0x0
   180022ea6:	74 26                	je     0x180022ece
   180022ea8:	66 83 7c 44 36 00    	cmp    WORD PTR [rsp+rax*2+0x36],0x0
   180022eae:	74 19                	je     0x180022ec9
   180022eb0:	66 83 7c 44 38 00    	cmp    WORD PTR [rsp+rax*2+0x38],0x0
   180022eb6:	74 0c                	je     0x180022ec4
   180022eb8:	83 c0 05             	add    eax,0x5
   180022ebb:	3d 90 01 00 00       	cmp    eax,0x190
   180022ec0:	7c ce                	jl     0x180022e90
   180022ec2:	eb 11                	jmp    0x180022ed5
   180022ec4:	83 c0 04             	add    eax,0x4
   180022ec7:	eb 0c                	jmp    0x180022ed5
   180022ec9:	83 c0 03             	add    eax,0x3
   180022ecc:	eb 07                	jmp    0x180022ed5
   180022ece:	83 c0 02             	add    eax,0x2
   180022ed1:	eb 02                	jmp    0x180022ed5
   180022ed3:	ff c0                	inc    eax
   180022ed5:	f3 0f 10 15 ff 07 0a 	movss  xmm2,DWORD PTR [rip+0xa07ff]        # 0x1800c36dc
   180022edc:	00
   180022edd:	48 8d 0d 1c d1 fd ff 	lea    rcx,[rip+0xfffffffffffdd11c]        # 0x180000000
   180022ee4:	83 bc 91 a0 0d 28 00 	cmp    DWORD PTR [rcx+rdx*4+0x280da0],0x0
   180022eeb:	00
   180022eec:	f3 41 0f 58 d0       	addss  xmm2,xmm8
   180022ef1:	66 0f 6e c8          	movd   xmm1,eax
   180022ef5:	0f 5b c9             	cvtdq2ps xmm1,xmm1
   180022ef8:	f3 0f 59 0d d8 07 0a 	mulss  xmm1,DWORD PTR [rip+0xa07d8]        # 0x1800c36d8
   180022eff:	00
   180022f00:	f3 0f 58 cf          	addss  xmm1,xmm7
   180022f04:	75 29                	jne    0x180022f2f
   180022f06:	f3 0f 11 bc 91 b0 0d 	movss  DWORD PTR [rcx+rdx*4+0x280db0],xmm7
   180022f0d:	28 00
   180022f0f:	f3 44 0f 11 84 91 c0 	movss  DWORD PTR [rcx+rdx*4+0x280dc0],xmm8
   180022f16:	0d 28 00
   180022f19:	f3 0f 11 8c 91 d0 0d 	movss  DWORD PTR [rcx+rdx*4+0x280dd0],xmm1
   180022f20:	28 00
   180022f22:	c7 84 91 a0 0d 28 00 	mov    DWORD PTR [rcx+rdx*4+0x280da0],0x1
   180022f29:	01 00 00 00
   180022f2d:	eb 4d                	jmp    0x180022f7c
   180022f2f:	f3 0f 10 84 91 b0 0d 	movss  xmm0,DWORD PTR [rcx+rdx*4+0x280db0]
   180022f36:	28 00
   180022f38:	0f 2f c7             	comiss xmm0,xmm7
   180022f3b:	76 09                	jbe    0x180022f46
   180022f3d:	f3 0f 11 bc 91 b0 0d 	movss  DWORD PTR [rcx+rdx*4+0x280db0],xmm7
   180022f44:	28 00
   180022f46:	f3 0f 10 84 91 c0 0d 	movss  xmm0,DWORD PTR [rcx+rdx*4+0x280dc0]
   180022f4d:	28 00
   180022f4f:	41 0f 2f c0          	comiss xmm0,xmm8
   180022f53:	76 0a                	jbe    0x180022f5f
   180022f55:	f3 44 0f 11 84 91 c0 	movss  DWORD PTR [rcx+rdx*4+0x280dc0],xmm8
   180022f5c:	0d 28 00
   180022f5f:	0f 2f 8c 91 d0 0d 28 	comiss xmm1,DWORD PTR [rcx+rdx*4+0x280dd0]
   180022f66:	00
   180022f67:	76 09                	jbe    0x180022f72
   180022f69:	f3 0f 11 8c 91 d0 0d 	movss  DWORD PTR [rcx+rdx*4+0x280dd0],xmm1
   180022f70:	28 00
   180022f72:	0f 2f 94 91 e0 0d 28 	comiss xmm2,DWORD PTR [rcx+rdx*4+0x280de0]
   180022f79:	00
   180022f7a:	76 09                	jbe    0x180022f85
   180022f7c:	f3 0f 11 94 91 e0 0d 	movss  DWORD PTR [rcx+rdx*4+0x280de0],xmm2
   180022f83:	28 00
   180022f85:	4d 85 c9             	test   r9,r9
   180022f88:	74 30                	je     0x180022fba
   180022f8a:	8b 05 f8 01 0a 00    	mov    eax,DWORD PTR [rip+0xa01f8]        # 0x1800c3188
   180022f90:	4a 8b 14 00          	mov    rdx,QWORD PTR [rax+r8*1]
   180022f94:	48 85 d2             	test   rdx,rdx
   180022f97:	74 21                	je     0x180022fba
   180022f99:	8b 0d e5 01 0a 00    	mov    ecx,DWORD PTR [rip+0xa01e5]        # 0x1800c3184
   180022f9f:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
   180022fa4:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180022fa9:	49 03 c8             	add    rcx,r8
   180022fac:	41 0f 28 d8          	movaps xmm3,xmm8
   180022fb0:	89 5c 24 20          	mov    DWORD PTR [rsp+0x20],ebx
   180022fb4:	0f 28 d7             	movaps xmm2,xmm7
   180022fb7:	41 ff d1             	call   r9
   180022fba:	48 8b 8c 24 f0 01 00 	mov    rcx,QWORD PTR [rsp+0x1f0]
   180022fc1:	00
   180022fc2:	48 33 cc             	xor    rcx,rsp
   180022fc5:	e8 c6 6e 04 00       	call   0x180069e90
   180022fca:	0f 28 bc 24 10 02 00 	movaps xmm7,XMMWORD PTR [rsp+0x210]
   180022fd1:	00
   180022fd2:	44 0f 28 84 24 00 02 	movaps xmm8,XMMWORD PTR [rsp+0x200]
   180022fd9:	00 00
   180022fdb:	48 81 c4 30 02 00 00 	add    rsp,0x230
   180022fe2:	5b                   	pop    rbx
   180022fe3:	c3                   	ret
   180022fe4:	e8 a7 6b 04 00       	call   0x180069b90
