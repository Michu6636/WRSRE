
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

00000001800280e0 <.text+0x270e0>:
   1800280e0:	48 83 ec 48          	sub    rsp,0x48
   1800280e4:	e8 a7 00 00 00       	call   0x180028190
   1800280e9:	85 c0                	test   eax,eax
   1800280eb:	0f 88 8d 00 00 00    	js     0x18002817e
   1800280f1:	48 63 c8             	movsxd rcx,eax
   1800280f4:	48 8d 14 89          	lea    rdx,[rcx+rcx*4]
   1800280f8:	48 8d 0d 21 c6 18 00 	lea    rcx,[rip+0x18c621]        # 0x1801b4720
   1800280ff:	f3 0f 10 54 d1 20    	movss  xmm2,DWORD PTR [rcx+rdx*8+0x20]
   180028105:	f3 0f 10 4c d1 24    	movss  xmm1,DWORD PTR [rcx+rdx*8+0x24]
   18002810b:	0f 57 c0             	xorps  xmm0,xmm0
   18002810e:	0f 2f c2             	comiss xmm0,xmm2
   180028111:	72 05                	jb     0x180028118
   180028113:	0f 2f c1             	comiss xmm0,xmm1
   180028116:	73 66                	jae    0x18002817e
   180028118:	89 44 24 50          	mov    DWORD PTR [rsp+0x50],eax
   18002811c:	48 8b 05 5d cc 09 00 	mov    rax,QWORD PTR [rip+0x9cc5d]        # 0x1800c4d80
   180028123:	0f 2f d0             	comiss xmm2,xmm0
   180028126:	76 08                	jbe    0x180028130
   180028128:	f3 0f 11 90 64 fd 9d 	movss  DWORD PTR [rax+0x9dfd64],xmm2
   18002812f:	00 
   180028130:	0f 2f c8             	comiss xmm1,xmm0
   180028133:	76 1c                	jbe    0x180028151
   180028135:	f3 0f 10 80 60 fd 9d 	movss  xmm0,DWORD PTR [rax+0x9dfd60]
   18002813c:	00 
   18002813d:	f3 0f 58 05 8b 75 08 	addss  xmm0,DWORD PTR [rip+0x8758b]        # 0x1800af6d0
   180028144:	00 
   180028145:	f3 0f 5f c8          	maxss  xmm1,xmm0
   180028149:	f3 0f 11 88 68 fd 9d 	movss  DWORD PTR [rax+0x9dfd68],xmm1
   180028150:	00 
   180028151:	eb 2b                	jmp    0x18002817e
   180028153:	48 63 54 24 50       	movsxd rdx,DWORD PTR [rsp+0x50]
   180028158:	48 8d 0c 92          	lea    rcx,[rdx+rdx*4]
   18002815c:	48 8d 05 bd c5 18 00 	lea    rax,[rip+0x18c5bd]        # 0x1801b4720
   180028163:	c7 44 c8 24 00 00 00 	mov    DWORD PTR [rax+rcx*8+0x24],0x0
   18002816a:	00 
   18002816b:	48 8d 0c 92          	lea    rcx,[rdx+rdx*4]
   18002816f:	48 8d 05 aa c5 18 00 	lea    rax,[rip+0x18c5aa]        # 0x1801b4720
   180028176:	c7 44 c8 20 00 00 00 	mov    DWORD PTR [rax+rcx*8+0x20],0x0
   18002817d:	00 
   18002817e:	48 83 c4 48          	add    rsp,0x48
   180028182:	c3                   	ret
   180028183:	cc                   	int3
   180028184:	cc                   	int3
   180028185:	cc                   	int3
   180028186:	cc                   	int3
   180028187:	cc                   	int3
   180028188:	cc                   	int3
   180028189:	cc                   	int3
   18002818a:	cc                   	int3
   18002818b:	cc                   	int3
   18002818c:	cc                   	int3
   18002818d:	cc                   	int3
   18002818e:	cc                   	int3
   18002818f:	cc                   	int3
