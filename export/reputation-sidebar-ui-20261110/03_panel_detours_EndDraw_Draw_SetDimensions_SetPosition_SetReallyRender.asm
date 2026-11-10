   180067bf0:	sub    rsp,0x28
   180067bf4:	call   QWORD PTR [rip+0x21e516]        # 0x180286110
   180067bfa:	inc    DWORD PTR [rip+0x218f14]        # 0x180280b14
   180067c00:	mov    eax,DWORD PTR [rip+0x13c37e]        # 0x1801a3f84
   180067c06:	cmp    eax,0x1
   180067c09:	jne    0x180067c10
   180067c0b:	call   0x18002e410
   180067c10:	mov    eax,DWORD PTR [rip+0x13b492]        # 0x1801a30a8
   180067c16:	cmp    DWORD PTR [rip+0x476c54],eax        # 0x1804de870
   180067c1c:	je     0x180067c33
   180067c1e:	mov    DWORD PTR [rip+0x476c4c],eax        # 0x1804de870
   180067c24:	mov    eax,DWORD PTR [rip+0x166e3e]        # 0x1801cea68
   180067c2a:	test   eax,eax
   180067c2c:	je     0x180067c33
   180067c2e:	call   0x180006580
   180067c33:	cmp    DWORD PTR [rip+0x218ed2],0x2        # 0x180280b0c
   180067c3a:	jne    0x180067c45
   180067c3c:	add    rsp,0x28
   180067c40:	jmp    0x180039400
   180067c45:	add    rsp,0x28
   180067c49:	ret
   180067c4a:	int3
   180067c4b:	int3
   180067c4c:	int3
   180067c4d:	int3
   180067c4e:	int3
   180067c4f:	int3
   180067c50:	mov    QWORD PTR [rsp+0x8],rbx
   180067c55:	push   rdi
   180067c56:	sub    rsp,0x40
   180067c5a:	movsxd rbx,edx
   180067c5d:	mov    rdi,rcx
   180067c60:	mov    r8,QWORD PTR [rip+0x5d119]        # 0x1800c4d80
   180067c67:	add    r8,0x31f03c
   180067c6e:	mov    rax,QWORD PTR [rsp+0x48]
   180067c73:	cmp    rax,r8
   180067c76:	jne    0x180067c96
   180067c78:	cmp    DWORD PTR [rip+0x13c329],0x0        # 0x1801a3fa8
   180067c7f:	je     0x180067c88
   180067c81:	call   0x180038ea0
   180067c86:	jmp    0x180067c96
   180067c88:	cmp    DWORD PTR [rip+0x14cbd1],0x0        # 0x1801b4860
   180067c8f:	je     0x180067c96
   180067c91:	call   0x180028050
   180067c96:	lea    eax,[rbx-0x7f000000]
   180067c9c:	cmp    eax,0xb
   180067c9f:	ja     0x180067cc4
   180067ca1:	lea    rax,[rbx-0x7f000000]
   180067ca8:	lea    rcx,[rax+rax*4]
   180067cac:	shl    rcx,0x4
   180067cb0:	lea    rax,[rip+0x14c5f9]        # 0x1801b42b0
   180067cb7:	add    rax,rcx
   180067cba:	cmp    WORD PTR [rax],0x0
   180067cbe:	jne    0x180067d87
   180067cc4:	cmp    WORD PTR [rip+0x14c3c4],0x0        # 0x1801b4090
   180067ccc:	je     0x180067d09
   180067cce:	cmp    DWORD PTR [rip+0x13c2d3],0x0        # 0x1801a3fa8
   180067cd5:	je     0x180067d09
   180067cd7:	cmp    ebx,DWORD PTR [rip+0x5b62b]        # 0x1800c3308
   180067cdd:	jne    0x180067d09
   180067cdf:	mov    rcx,QWORD PTR [rip+0x5d09a]        # 0x1800c4d80
   180067ce6:	add    rcx,0x31f03c
   180067ced:	mov    rax,QWORD PTR [rsp+0x48]
   180067cf2:	cmp    rax,rcx
   180067cf5:	jne    0x180067d09
   180067cf7:	lea    rax,[rip+0x14c392]        # 0x1801b4090
   180067cfe:	mov    rbx,QWORD PTR [rsp+0x50]
   180067d03:	add    rsp,0x40
   180067d07:	pop    rdi
   180067d08:	ret
   180067d09:	mov    edx,ebx
   180067d0b:	mov    rcx,rdi
   180067d0e:	call   QWORD PTR [rip+0x71784]        # 0x1800d9498
   180067d14:	mov    rdi,rax
   180067d17:	mov    QWORD PTR [rsp+0x60],rax
   180067d1c:	test   rax,rax
   180067d1f:	jne    0x180067d2c
   180067d21:	mov    rbx,QWORD PTR [rsp+0x50]
   180067d26:	add    rsp,0x40
   180067d2a:	pop    rdi
   180067d2b:	ret
   180067d2c:	cmp    DWORD PTR [rip+0x5b429],0x0        # 0x1800c315c
   180067d33:	je     0x180067d84
   180067d35:	lea    eax,[rbx-0x984e]
   180067d3b:	cmp    eax,0x6e
   180067d3e:	jbe    0x180067d45
   180067d40:	cmp    ebx,0xffffffff
   180067d43:	jg     0x180067d84
   180067d45:	inc    QWORD PTR [rip+0x14c324]        # 0x1801b4070
   180067d4c:	mov    ecx,ebx
   180067d4e:	call   0x18000c350
   180067d53:	test   eax,eax
   180067d55:	js     0x180067d84
   180067d57:	inc    QWORD PTR [rip+0x14c322]        # 0x1801b4080
   180067d5e:	mov    edx,ebx
   180067d60:	mov    rcx,rdi
   180067d63:	call   0x18005cb10
   180067d68:	mov    QWORD PTR [rsp+0x38],rax
   180067d6d:	jmp    0x180067d79
   180067d6f:	mov    rax,QWORD PTR [rsp+0x60]
   180067d74:	mov    QWORD PTR [rsp+0x38],rax
   180067d79:	mov    rbx,QWORD PTR [rsp+0x50]
   180067d7e:	add    rsp,0x40
   180067d82:	pop    rdi
   180067d83:	ret
   180067d84:	mov    rax,rdi
   180067d87:	mov    rbx,QWORD PTR [rsp+0x50]
   180067d8c:	add    rsp,0x40
   180067d90:	pop    rdi
   180067d91:	ret
   180067d92:	int3
   180067d93:	int3
   180067d94:	int3
   180067d95:	int3
   180067d96:	int3
   180067d97:	int3
   180067d98:	int3
   180067d99:	int3
   180067d9a:	int3
   180067d9b:	int3
   180067d9c:	int3
   180067d9d:	int3
   180067d9e:	int3
   180067d9f:	int3
   180067da0:	rex push rbx
   180067da2:	sub    rsp,0x70
   180067da6:	cmp    DWORD PTR [rip+0x5b93b],0x2        # 0x1800c36e8
   180067dad:	mov    rbx,rcx
   180067db0:	movaps XMMWORD PTR [rsp+0x60],xmm6
   180067db5:	movaps xmm6,xmm3
   180067db8:	movaps XMMWORD PTR [rsp+0x50],xmm7
   180067dbd:	movaps xmm7,xmm2
   180067dc0:	movaps XMMWORD PTR [rsp+0x40],xmm8
   180067dc6:	movaps xmm8,xmm1
   180067dca:	jne    0x180067de6
   180067dcc:	cmp    DWORD PTR [rip+0x21e2a1],0x0        # 0x180286074
   180067dd3:	jne    0x180067de6
   180067dd5:	xor    ecx,ecx
   180067dd7:	mov    DWORD PTR [rip+0x21e293],0x1        # 0x180286074
   180067de1:	call   0x18002a6d0
   180067de6:	movss  xmm1,DWORD PTR [rsp+0xa0]
   180067def:	movaps xmm3,xmm6
   180067df2:	movzx  eax,BYTE PTR [rsp+0xb0]
   180067dfa:	movaps xmm2,xmm7
   180067dfd:	movss  xmm0,DWORD PTR [rsp+0xa8]
   180067e06:	mov    rcx,rbx
   180067e09:	mov    BYTE PTR [rsp+0x30],al
   180067e0d:	movss  DWORD PTR [rsp+0x28],xmm0
   180067e13:	movss  DWORD PTR [rsp+0x20],xmm1
   180067e19:	movaps xmm1,xmm8
   180067e1d:	call   QWORD PTR [rip+0x21e2f5]        # 0x180286118
   180067e23:	inc    DWORD PTR [rip+0x218ceb]        # 0x180280b14
   180067e29:	cmp    DWORD PTR [rip+0x218cdc],0x3        # 0x180280b0c
   180067e30:	jne    0x180067e4b
   180067e32:	mov    DWORD PTR [rip+0x218ce4],0x1        # 0x180280b20
   180067e3c:	call   0x180039400
   180067e41:	mov    DWORD PTR [rip+0x218cd5],0x0        # 0x180280b20
   180067e4b:	movaps xmm6,XMMWORD PTR [rsp+0x60]
   180067e50:	movaps xmm7,XMMWORD PTR [rsp+0x50]
   180067e55:	movaps xmm8,XMMWORD PTR [rsp+0x40]
   180067e5b:	add    rsp,0x70
   180067e5f:	pop    rbx
   180067e60:	ret
   180067e61:	int3
   180067e62:	int3
   180067e63:	int3
   180067e64:	int3
   180067e65:	int3
   180067e66:	int3
   180067e67:	int3
   180067e68:	int3
   180067e69:	int3
   180067e6a:	int3
   180067e6b:	int3
   180067e6c:	int3
   180067e6d:	int3
   180067e6e:	int3
   180067e6f:	int3
   180067e70:	mov    QWORD PTR [rsp+0x8],rbx
   180067e75:	push   rdi
   180067e76:	sub    rsp,0x20
   180067e7a:	mov    eax,DWORD PTR [rip+0x13bea0]        # 0x1801a3d20
   180067e80:	mov    rbx,rdx
   180067e83:	mov    rdi,rcx
   180067e86:	test   eax,eax
   180067e88:	je     0x180067e9b
   180067e8a:	test   rdx,rdx
   180067e8d:	je     0x180067e9b
   180067e8f:	mov    rcx,rdx
   180067e92:	call   0x180008160
   180067e97:	test   eax,eax
   180067e99:	jne    0x180067ea7
   180067e9b:	mov    rdx,rbx
   180067e9e:	mov    rcx,rdi
   180067ea1:	call   QWORD PTR [rip+0x16a6e1]        # 0x1801d2588
   180067ea7:	mov    rbx,QWORD PTR [rsp+0x30]
   180067eac:	add    rsp,0x20
   180067eb0:	pop    rdi
   180067eb1:	ret
   180067eb2:	int3
   180067eb3:	int3
   180067eb4:	int3
   180067eb5:	int3
   180067eb6:	int3
   180067eb7:	int3
   180067eb8:	int3
   180067eb9:	int3
   180067eba:	int3
   180067ebb:	int3
   180067ebc:	int3
   180067ebd:	int3
   180067ebe:	int3
   180067ebf:	int3
   180067ec0:	mov    QWORD PTR [rsp+0x8],rbx
   180067ec5:	push   rdi
   180067ec6:	sub    rsp,0x20
   180067eca:	mov    eax,DWORD PTR [rip+0x13be50]        # 0x1801a3d20
   180067ed0:	mov    rbx,rdx
   180067ed3:	mov    rdi,rcx
   180067ed6:	test   eax,eax
   180067ed8:	je     0x180067eeb
   180067eda:	test   rdx,rdx
   180067edd:	je     0x180067eeb
   180067edf:	mov    rcx,rdx
   180067ee2:	call   0x180008160
   180067ee7:	test   eax,eax
   180067ee9:	jne    0x180067ef7
   180067eeb:	mov    rdx,rbx
   180067eee:	mov    rcx,rdi
   180067ef1:	call   QWORD PTR [rip+0x16a689]        # 0x1801d2580
   180067ef7:	mov    rbx,QWORD PTR [rsp+0x30]
   180067efc:	add    rsp,0x20
   180067f00:	pop    rdi
   180067f01:	ret
   180067f02:	int3
   180067f03:	int3
   180067f04:	int3
   180067f05:	int3
   180067f06:	int3
   180067f07:	int3
   180067f08:	int3
   180067f09:	int3
   180067f0a:	int3
   180067f0b:	int3
   180067f0c:	int3
   180067f0d:	int3
   180067f0e:	int3
   180067f0f:	int3
   180067f10:	mov    QWORD PTR [rsp+0x8],rbx
   180067f15:	push   rdi
   180067f16:	sub    rsp,0x20
   180067f1a:	cmp    DWORD PTR [rip+0x5b7c7],0x3        # 0x1800c36e8
   180067f21:	mov    rbx,rdx
   180067f24:	mov    rdi,rcx
   180067f27:	jne    0x180067f84
   180067f29:	cmp    DWORD PTR [rip+0x21e144],0x0        # 0x180286074
   180067f30:	jne    0x180067f84
   180067f32:	mov    eax,DWORD PTR [rip+0x1396a4]        # 0x1801a15dc
   180067f38:	mov    r8d,eax
   180067f3b:	inc    eax
   180067f3d:	mov    DWORD PTR [rip+0x21e12d],0x1        # 0x180286074
   180067f47:	mov    DWORD PTR [rip+0x13968f],eax        # 0x1801a15dc
   180067f4d:	test   r8d,r8d
   180067f50:	jg     0x180067f71
   180067f52:	mov    DWORD PTR [rip+0x13967c],0x0        # 0x1801a15d8
   180067f5c:	call   0x18003ae90
   180067f61:	movsd  QWORD PTR [rip+0x139667],xmm0        # 0x1801a15d0
   180067f69:	movsd  QWORD PTR [rip+0x139657],xmm0        # 0x1801a15c8
   180067f71:	xor    ecx,ecx
   180067f73:	call   0x18002a6d0
   180067f78:	lea    rcx,[rip+0x34d69]        # 0x18009cce8    ;; "the panel's shapes (text flush)"
   180067f7f:	call   0x18003ac40
   180067f84:	mov    rdx,rbx
   180067f87:	mov    rcx,rdi
   180067f8a:	mov    rbx,QWORD PTR [rsp+0x30]
   180067f8f:	add    rsp,0x20
   180067f93:	pop    rdi
   180067f94:	rex.W jmp QWORD PTR [rip+0x21e14d]        # 0x1802860e8
   180067f9b:	int3
   180067f9c:	int3
   180067f9d:	int3
   180067f9e:	int3
   180067f9f:	int3
   180067fa0:	rex push rbx
   180067fa2:	sub    rsp,0x40
   180067fa6:	movaps XMMWORD PTR [rsp+0x30],xmm6
   180067fab:	movaps xmm6,xmm2
   180067fae:	movaps XMMWORD PTR [rsp+0x20],xmm7
   180067fb3:	movaps xmm7,xmm1
   180067fb6:	call   QWORD PTR [rip+0x21e0fc]        # 0x1802860b8
   180067fbc:	cmp    DWORD PTR [rip+0x14c89d],0x0        # 0x1801b4860
   180067fc3:	mov    rbx,QWORD PTR [rsp+0x48]
   180067fc8:	mov    rcx,QWORD PTR [rip+0x5cdb1]        # 0x1800c4d80
   180067fcf:	je     0x18006800a
   180067fd1:	lea    rdx,[rcx+0x31e478]
   180067fd8:	cmp    rbx,rdx
   180067fdb:	je     0x180067fe9
   180067fdd:	lea    rax,[rcx+0x31e4b2]
   180067fe4:	cmp    rbx,rax
   180067fe7:	jne    0x18006800a
   180067fe9:	movss  DWORD PTR [rip+0x14c877],xmm7        # 0x1801b4868
   180067ff1:	movss  DWORD PTR [rip+0x14c873],xmm6        # 0x1801b486c
   180067ff9:	cmp    rbx,rdx
   180067ffc:	jne    0x18006800a
   180067ffe:	call   0x1800280e0
   180068003:	mov    rcx,QWORD PTR [rip+0x5cd76]        # 0x1800c4d80
   18006800a:	cmp    DWORD PTR [rip+0x218d63],0x0        # 0x180280d74
   180068011:	je     0x18006807c
   180068013:	cmp    DWORD PTR [rip+0x218d5e],0x0        # 0x180280d78
   18006801a:	je     0x18006807c
   18006801c:	lea    rax,[rcx+0x31e478]
   180068023:	cmp    rbx,rax
   180068026:	jne    0x180068060
   180068028:	cmp    DWORD PTR [rip+0x5b2bd],0x0        # 0x1800c32ec
   18006802f:	movss  DWORD PTR [rip+0x218d25],xmm7        # 0x180280d5c
   180068037:	movss  DWORD PTR [rip+0x218d21],xmm6        # 0x180280d60
   18006803f:	je     0x18006807c
   180068041:	call   0x180057d50
   180068046:	test   eax,eax
   180068048:	je     0x18006807c
   18006804a:	xor    ecx,ecx
   18006804c:	movaps xmm6,XMMWORD PTR [rsp+0x30]
   180068051:	movaps xmm7,XMMWORD PTR [rsp+0x20]
   180068056:	add    rsp,0x40
   18006805a:	pop    rbx
   18006805b:	jmp    0x180058930
   180068060:	lea    rax,[rcx+0x31e4b2]
   180068067:	cmp    rbx,rax
   18006806a:	jne    0x18006807c
   18006806c:	movss  DWORD PTR [rip+0x218ce8],xmm7        # 0x180280d5c
   180068074:	movss  DWORD PTR [rip+0x218ce4],xmm6        # 0x180280d60
   18006807c:	movaps xmm6,XMMWORD PTR [rsp+0x30]
   180068081:	movaps xmm7,XMMWORD PTR [rsp+0x20]
   180068086:	add    rsp,0x40
   18006808a:	pop    rbx
   18006808b:	ret
   18006808c:	int3
   18006808d:	int3
   18006808e:	int3
   18006808f:	int3
   180068090:	rex push rbx
   180068092:	sub    rsp,0x20
   180068096:	mov    rbx,rdx
   180068099:	call   QWORD PTR [rip+0x21e021]        # 0x1802860c0
   18006809f:	cmp    DWORD PTR [rip+0x14c7ba],0x0        # 0x1801b4860
   1800680a6:	mov    r8,QWORD PTR [rip+0x5ccd3]        # 0x1800c4d80
   1800680ad:	mov    edx,DWORD PTR [rip+0x13bf1d]        # 0x1801a3fd0
   1800680b3:	je     0x1800680ea
   1800680b5:	test   rbx,rbx
   1800680b8:	je     0x1800680ea
   1800680ba:	mov    rax,QWORD PTR [rsp+0x28]
   1800680bf:	lea    rcx,[r8+0x31e509]
   1800680c6:	cmp    rax,rcx
   1800680c9:	jne    0x1800680ea
   1800680cb:	movss  xmm0,DWORD PTR [rbx]
   1800680cf:	movss  xmm1,DWORD PTR [rbx+0x4]
   1800680d4:	movss  DWORD PTR [rip+0x14c794],xmm0        # 0x1801b4870
   1800680dc:	movss  DWORD PTR [rip+0x14c790],xmm1        # 0x1801b4874
   1800680e4:	mov    DWORD PTR [rip+0x14c78e],edx        # 0x1801b4878
   1800680ea:	cmp    DWORD PTR [rip+0x218c83],0x0        # 0x180280d74
   1800680f1:	je     0x180068131
   1800680f3:	cmp    DWORD PTR [rip+0x218c7e],0x0        # 0x180280d78
   1800680fa:	je     0x180068131
   1800680fc:	test   rbx,rbx
   1800680ff:	je     0x180068131
   180068101:	mov    rax,QWORD PTR [rsp+0x28]
   180068106:	lea    rcx,[r8+0x31e509]
   18006810d:	cmp    rax,rcx
   180068110:	jne    0x180068131
   180068112:	movss  xmm0,DWORD PTR [rbx]
   180068116:	movss  xmm1,DWORD PTR [rbx+0x4]
   18006811b:	movss  DWORD PTR [rip+0x218c41],xmm0        # 0x180280d64
   180068123:	movss  DWORD PTR [rip+0x218c3d],xmm1        # 0x180280d68
   18006812b:	mov    DWORD PTR [rip+0x218c3b],edx        # 0x180280d6c
   180068131:	add    rsp,0x20
   180068135:	pop    rbx
   180068136:	ret
   180068137:	int3
   180068138:	int3
   180068139:	int3
   18006813a:	int3
   18006813b:	int3
   18006813c:	int3
   18006813d:	int3
   18006813e:	int3
   18006813f:	int3
   180068140:	rex push rbx
   180068142:	sub    rsp,0x20
   180068146:	movzx  ebx,dl
   180068149:	call   QWORD PTR [rip+0x21dfb9]        # 0x180286108
   18006814f:	inc    DWORD PTR [rip+0x2189bf]        # 0x180280b14
   180068155:	cmp    DWORD PTR [rip+0x2189b0],0x1        # 0x180280b0c
   18006815c:	jne    0x18006816c
   18006815e:	test   bl,bl
   180068160:	je     0x18006816c
   180068162:	add    rsp,0x20
   180068166:	pop    rbx
   180068167:	jmp    0x180039400
   18006816c:	add    rsp,0x20
   180068170:	pop    rbx
   180068171:	ret
