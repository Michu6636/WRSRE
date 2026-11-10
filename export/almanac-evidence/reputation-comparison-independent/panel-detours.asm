
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180067fa0 <.text+0x66fa0>:
   180067fa0:	40 53                	rex push rbx
   180067fa2:	48 83 ec 40          	sub    rsp,0x40
   180067fa6:	0f 29 74 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm6
   180067fab:	0f 28 f2             	movaps xmm6,xmm2
   180067fae:	0f 29 7c 24 20       	movaps XMMWORD PTR [rsp+0x20],xmm7
   180067fb3:	0f 28 f9             	movaps xmm7,xmm1
   180067fb6:	ff 15 fc e0 21 00    	call   QWORD PTR [rip+0x21e0fc]        # 0x1802860b8
   180067fbc:	83 3d 9d c8 14 00 00 	cmp    DWORD PTR [rip+0x14c89d],0x0        # 0x1801b4860
   180067fc3:	48 8b 5c 24 48       	mov    rbx,QWORD PTR [rsp+0x48]
   180067fc8:	48 8b 0d b1 cd 05 00 	mov    rcx,QWORD PTR [rip+0x5cdb1]        # 0x1800c4d80
   180067fcf:	74 39                	je     0x18006800a
   180067fd1:	48 8d 91 78 e4 31 00 	lea    rdx,[rcx+0x31e478]
   180067fd8:	48 3b da             	cmp    rbx,rdx
   180067fdb:	74 0c                	je     0x180067fe9
   180067fdd:	48 8d 81 b2 e4 31 00 	lea    rax,[rcx+0x31e4b2]
   180067fe4:	48 3b d8             	cmp    rbx,rax
   180067fe7:	75 21                	jne    0x18006800a
   180067fe9:	f3 0f 11 3d 77 c8 14 	movss  DWORD PTR [rip+0x14c877],xmm7        # 0x1801b4868
   180067ff0:	00 
   180067ff1:	f3 0f 11 35 73 c8 14 	movss  DWORD PTR [rip+0x14c873],xmm6        # 0x1801b486c
   180067ff8:	00 
   180067ff9:	48 3b da             	cmp    rbx,rdx
   180067ffc:	75 0c                	jne    0x18006800a
   180067ffe:	e8 dd 00 fc ff       	call   0x1800280e0
   180068003:	48 8b 0d 76 cd 05 00 	mov    rcx,QWORD PTR [rip+0x5cd76]        # 0x1800c4d80
   18006800a:	83 3d 63 8d 21 00 00 	cmp    DWORD PTR [rip+0x218d63],0x0        # 0x180280d74
   180068011:	74 69                	je     0x18006807c
   180068013:	83 3d 5e 8d 21 00 00 	cmp    DWORD PTR [rip+0x218d5e],0x0        # 0x180280d78
   18006801a:	74 60                	je     0x18006807c
   18006801c:	48 8d 81 78 e4 31 00 	lea    rax,[rcx+0x31e478]
   180068023:	48 3b d8             	cmp    rbx,rax
   180068026:	75 38                	jne    0x180068060
   180068028:	83 3d bd b2 05 00 00 	cmp    DWORD PTR [rip+0x5b2bd],0x0        # 0x1800c32ec
   18006802f:	f3 0f 11 3d 25 8d 21 	movss  DWORD PTR [rip+0x218d25],xmm7        # 0x180280d5c
   180068036:	00 
   180068037:	f3 0f 11 35 21 8d 21 	movss  DWORD PTR [rip+0x218d21],xmm6        # 0x180280d60
   18006803e:	00 
   18006803f:	74 3b                	je     0x18006807c
   180068041:	e8 0a fd fe ff       	call   0x180057d50
   180068046:	85 c0                	test   eax,eax
   180068048:	74 32                	je     0x18006807c
   18006804a:	33 c9                	xor    ecx,ecx
   18006804c:	0f 28 74 24 30       	movaps xmm6,XMMWORD PTR [rsp+0x30]
   180068051:	0f 28 7c 24 20       	movaps xmm7,XMMWORD PTR [rsp+0x20]
   180068056:	48 83 c4 40          	add    rsp,0x40
   18006805a:	5b                   	pop    rbx
   18006805b:	e9 d0 08 ff ff       	jmp    0x180058930
   180068060:	48 8d 81 b2 e4 31 00 	lea    rax,[rcx+0x31e4b2]
   180068067:	48 3b d8             	cmp    rbx,rax
   18006806a:	75 10                	jne    0x18006807c
   18006806c:	f3 0f 11 3d e8 8c 21 	movss  DWORD PTR [rip+0x218ce8],xmm7        # 0x180280d5c
   180068073:	00 
   180068074:	f3 0f 11 35 e4 8c 21 	movss  DWORD PTR [rip+0x218ce4],xmm6        # 0x180280d60
   18006807b:	00 
   18006807c:	0f 28 74 24 30       	movaps xmm6,XMMWORD PTR [rsp+0x30]
   180068081:	0f 28 7c 24 20       	movaps xmm7,XMMWORD PTR [rsp+0x20]
   180068086:	48 83 c4 40          	add    rsp,0x40
   18006808a:	5b                   	pop    rbx
   18006808b:	c3                   	ret
   18006808c:	cc                   	int3
   18006808d:	cc                   	int3
   18006808e:	cc                   	int3
   18006808f:	cc                   	int3
   180068090:	40 53                	rex push rbx
   180068092:	48 83 ec 20          	sub    rsp,0x20
   180068096:	48 8b da             	mov    rbx,rdx
   180068099:	ff 15 21 e0 21 00    	call   QWORD PTR [rip+0x21e021]        # 0x1802860c0
   18006809f:	83 3d ba c7 14 00 00 	cmp    DWORD PTR [rip+0x14c7ba],0x0        # 0x1801b4860
   1800680a6:	4c 8b 05 d3 cc 05 00 	mov    r8,QWORD PTR [rip+0x5ccd3]        # 0x1800c4d80
   1800680ad:	8b 15 1d bf 13 00    	mov    edx,DWORD PTR [rip+0x13bf1d]        # 0x1801a3fd0
   1800680b3:	74 35                	je     0x1800680ea
   1800680b5:	48 85 db             	test   rbx,rbx
   1800680b8:	74 30                	je     0x1800680ea
   1800680ba:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
   1800680bf:	49 8d 88 09 e5 31 00 	lea    rcx,[r8+0x31e509]
   1800680c6:	48 3b c1             	cmp    rax,rcx
   1800680c9:	75 1f                	jne    0x1800680ea
   1800680cb:	f3 0f 10 03          	movss  xmm0,DWORD PTR [rbx]
   1800680cf:	f3 0f 10 4b 04       	movss  xmm1,DWORD PTR [rbx+0x4]
   1800680d4:	f3 0f 11 05 94 c7 14 	movss  DWORD PTR [rip+0x14c794],xmm0        # 0x1801b4870
   1800680db:	00 
   1800680dc:	f3 0f 11 0d 90 c7 14 	movss  DWORD PTR [rip+0x14c790],xmm1        # 0x1801b4874
   1800680e3:	00 
   1800680e4:	89 15 8e c7 14 00    	mov    DWORD PTR [rip+0x14c78e],edx        # 0x1801b4878
   1800680ea:	83 3d 83 8c 21 00 00 	cmp    DWORD PTR [rip+0x218c83],0x0        # 0x180280d74
   1800680f1:	74 3e                	je     0x180068131
   1800680f3:	83 3d 7e 8c 21 00 00 	cmp    DWORD PTR [rip+0x218c7e],0x0        # 0x180280d78
   1800680fa:	74 35                	je     0x180068131
   1800680fc:	48 85 db             	test   rbx,rbx
   1800680ff:	74 30                	je     0x180068131
   180068101:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
   180068106:	49 8d 88 09 e5 31 00 	lea    rcx,[r8+0x31e509]
   18006810d:	48 3b c1             	cmp    rax,rcx
   180068110:	75 1f                	jne    0x180068131
   180068112:	f3 0f 10 03          	movss  xmm0,DWORD PTR [rbx]
   180068116:	f3 0f 10 4b 04       	movss  xmm1,DWORD PTR [rbx+0x4]
   18006811b:	f3 0f 11 05 41 8c 21 	movss  DWORD PTR [rip+0x218c41],xmm0        # 0x180280d64
   180068122:	00 
   180068123:	f3 0f 11 0d 3d 8c 21 	movss  DWORD PTR [rip+0x218c3d],xmm1        # 0x180280d68
   18006812a:	00 
   18006812b:	89 15 3b 8c 21 00    	mov    DWORD PTR [rip+0x218c3b],edx        # 0x180280d6c
   180068131:	48 83 c4 20          	add    rsp,0x20
   180068135:	5b                   	pop    rbx
   180068136:	c3                   	ret
   180068137:	cc                   	int3
   180068138:	cc                   	int3
   180068139:	cc                   	int3
   18006813a:	cc                   	int3
   18006813b:	cc                   	int3
   18006813c:	cc                   	int3
   18006813d:	cc                   	int3
   18006813e:	cc                   	int3
   18006813f:	cc                   	int3
