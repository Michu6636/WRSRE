
scratch/reputation-static/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180068f30 <.text+0x67f30>:
   180068f30:	48 83 ec 48          	sub    rsp,0x48
   180068f34:	83 3d a5 01 64 00 00 	cmp    DWORD PTR [rip+0x6401a5],0x0        # 0x1806a90e0
   180068f3b:	74 18                	je     0x180068f55
   180068f3d:	48 8b 05 2c be 05 00 	mov    rax,QWORD PTR [rip+0x5be2c]        # 0x1800c4d70
   180068f44:	48 8d 0d 95 58 04 00 	lea    rcx,[rip+0x45895]        # 0x1800ae7e0
   180068f4b:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180068f4e:	33 c0                	xor    eax,eax
   180068f50:	48 83 c4 48          	add    rsp,0x48
   180068f54:	c3                   	ret
   180068f55:	48 89 5c 24 58       	mov    QWORD PTR [rsp+0x58],rbx
   180068f5a:	48 8d 0d af 58 04 00 	lea    rcx,[rip+0x458af]        # 0x1800ae810
   180068f61:	48 89 6c 24 60       	mov    QWORD PTR [rsp+0x60],rbp
   180068f66:	48 89 74 24 68       	mov    QWORD PTR [rsp+0x68],rsi
   180068f6b:	48 89 7c 24 40       	mov    QWORD PTR [rsp+0x40],rdi
   180068f70:	c7 05 66 01 64 00 01 	mov    DWORD PTR [rip+0x640166],0x1        # 0x1806a90e0
   180068f77:	00 00 00
   180068f7a:	e8 91 7c ff ff       	call   0x180060c10
   180068f7f:	83 3d 02 a1 05 00 00 	cmp    DWORD PTR [rip+0x5a102],0x0        # 0x1800c3088
   180068f86:	4c 8d 05 c3 58 04 00 	lea    r8,[rip+0x458c3]        # 0x1800ae850
   180068f8d:	48 89 05 4c a2 05 00 	mov    QWORD PTR [rip+0x5a24c],rax        # 0x1800c31e0
   180068f94:	48 8d 15 d1 8f 03 00 	lea    rdx,[rip+0x38fd1]        # 0x1800a1f6c
   180068f9b:	48 8d 05 7e 58 04 00 	lea    rax,[rip+0x4587e]        # 0x1800ae820
   180068fa2:	4c 0f 45 c0          	cmovne r8,rax
   180068fa6:	48 8d 0d d3 58 04 00 	lea    rcx,[rip+0x458d3]        # 0x1800ae880
   180068fad:	e8 3e af fe ff       	call   0x180053ef0
   180068fb2:	45 33 c0             	xor    r8d,r8d
   180068fb5:	48 8d 15 3c 4b 04 00 	lea    rdx,[rip+0x44b3c]        # 0x1800adaf8
   180068fbc:	48 8d 0d 71 31 04 00 	lea    rcx,[rip+0x43171]        # 0x1800ac134
   180068fc3:	e8 98 3c fa ff       	call   0x18000cc60
   180068fc8:	48 8b f8             	mov    rdi,rax
   180068fcb:	85 c0                	test   eax,eax
   180068fcd:	0f 84 96 00 00 00    	je     0x180069069
   180068fd3:	48 8b 1d a6 bd 05 00 	mov    rbx,QWORD PTR [rip+0x5bda6]        # 0x1800c4d80
   180068fda:	ba 40 00 00 00       	mov    edx,0x40
   180068fdf:	48 8b 05 8a bd 05 00 	mov    rax,QWORD PTR [rip+0x5bd8a]        # 0x1800c4d70
   180068fe6:	48 8b cb             	mov    rcx,rbx
   180068fe9:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180068fec:	85 c0                	test   eax,eax
   180068fee:	74 79                	je     0x180069069
   180068ff0:	b8 4d 5a 00 00       	mov    eax,0x5a4d
   180068ff5:	66 39 03             	cmp    WORD PTR [rbx],ax
   180068ff8:	75 6f                	jne    0x180069069
   180068ffa:	48 63 5b 3c          	movsxd rbx,DWORD PTR [rbx+0x3c]
   180068ffe:	ba 08 01 00 00       	mov    edx,0x108
   180069003:	48 03 1d 76 bd 05 00 	add    rbx,QWORD PTR [rip+0x5bd76]        # 0x1800c4d80
   18006900a:	48 8b 05 5f bd 05 00 	mov    rax,QWORD PTR [rip+0x5bd5f]        # 0x1800c4d70
   180069011:	48 8b cb             	mov    rcx,rbx
   180069014:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180069017:	85 c0                	test   eax,eax
   180069019:	74 4e                	je     0x180069069
   18006901b:	81 3b 50 45 00 00    	cmp    DWORD PTR [rbx],0x4550
   180069021:	75 46                	jne    0x180069069
   180069023:	8b 5b 08             	mov    ebx,DWORD PTR [rbx+0x8]
   180069026:	8b d3                	mov    edx,ebx
   180069028:	3b df                	cmp    ebx,edi
   18006902a:	75 18                	jne    0x180069044
   18006902c:	48 8d 0d d5 4a 04 00 	lea    rcx,[rip+0x44ad5]        # 0x1800adb08
   180069033:	c7 05 e7 7a 21 00 01 	mov    DWORD PTR [rip+0x217ae7],0x1        # 0x180280b24
   18006903a:	00 00 00
   18006903d:	e8 ae ae fe ff       	call   0x180053ef0
   180069042:	eb 25                	jmp    0x180069069
   180069044:	48 8b 05 25 bd 05 00 	mov    rax,QWORD PTR [rip+0x5bd25]        # 0x1800c4d70
   18006904b:	48 8d 0d ee 4a 04 00 	lea    rcx,[rip+0x44aee]        # 0x1800adb40
   180069052:	44 8b c7             	mov    r8d,edi
   180069055:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180069058:	44 8b c7             	mov    r8d,edi
   18006905b:	48 8d 0d 26 4b 04 00 	lea    rcx,[rip+0x44b26]        # 0x1800adb88
   180069062:	8b d3                	mov    edx,ebx
   180069064:	e8 87 ae fe ff       	call   0x180053ef0
   180069069:	8b 3d 51 85 13 00    	mov    edi,DWORD PTR [rip+0x138551]        # 0x1801a15c0
   18006906f:	33 ed                	xor    ebp,ebp
   180069071:	8b 35 4d 85 13 00    	mov    esi,DWORD PTR [rip+0x13854d]        # 0x1801a15c4
   180069077:	4c 89 74 24 38       	mov    QWORD PTR [rsp+0x38],r14
   18006907c:	89 2d 3e 85 13 00    	mov    DWORD PTR [rip+0x13853e],ebp        # 0x1801a15c0
   180069082:	89 2d 3c 85 13 00    	mov    DWORD PTR [rip+0x13853c],ebp        # 0x1801a15c4
   180069088:	85 ff                	test   edi,edi
   18006908a:	75 04                	jne    0x180069090
   18006908c:	85 f6                	test   esi,esi
   18006908e:	74 5f                	je     0x1800690ef
   180069090:	8b d7                	mov    edx,edi
   180069092:	48 8d 0d c7 4d 02 00 	lea    rcx,[rip+0x24dc7]        # 0x18008de60
   180069099:	e8 52 ae fe ff       	call   0x180053ef0
   18006909e:	8b dd                	mov    ebx,ebp
   1800690a0:	85 ff                	test   edi,edi
   1800690a2:	7e 27                	jle    0x1800690cb
   1800690a4:	4c 8d 35 15 85 12 00 	lea    r14,[rip+0x128515]        # 0x1801915c0
   1800690ab:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
   1800690b0:	8b d3                	mov    edx,ebx
   1800690b2:	48 8d 0d 63 4e 02 00 	lea    rcx,[rip+0x24e63]        # 0x18008df1c
   1800690b9:	48 c1 e2 09          	shl    rdx,0x9
   1800690bd:	49 03 d6             	add    rdx,r14
   1800690c0:	e8 2b ae fe ff       	call   0x180053ef0
   1800690c5:	ff c3                	inc    ebx
   1800690c7:	3b df                	cmp    ebx,edi
   1800690c9:	7c e5                	jl     0x1800690b0
   1800690cb:	85 f6                	test   esi,esi
   1800690cd:	74 14                	je     0x1800690e3
   1800690cf:	41 b8 80 00 00 00    	mov    r8d,0x80
   1800690d5:	48 8d 0d 44 4e 02 00 	lea    rcx,[rip+0x24e44]        # 0x18008df20
   1800690dc:	8b d6                	mov    edx,esi
   1800690de:	e8 0d ae fe ff       	call   0x180053ef0
   1800690e3:	48 8d 0d 7e 4e 02 00 	lea    rcx,[rip+0x24e7e]        # 0x18008df68
   1800690ea:	e8 01 ae fe ff       	call   0x180053ef0
   1800690ef:	39 2d 83 58 16 00    	cmp    DWORD PTR [rip+0x165883],ebp        # 0x1801ce978
   1800690f5:	48 8d 35 04 6f f9 ff 	lea    rsi,[rip+0xfffffffffff96f04]        # 0x180000000
   1800690fc:	4c 89 7c 24 30       	mov    QWORD PTR [rsp+0x30],r15
   180069101:	48 8d 3d 27 4d 02 00 	lea    rdi,[rip+0x24d27]        # 0x18008de2f
   180069108:	0f 84 1d 01 00 00    	je     0x18006922b
   18006910e:	8b dd                	mov    ebx,ebp
   180069110:	4c 8d 35 c1 4b 04 00 	lea    r14,[rip+0x44bc1]        # 0x1800adcd8
   180069117:	39 1d 83 27 10 00    	cmp    DWORD PTR [rip+0x102783],ebx        # 0x18016b8a0
   18006911d:	0f 8e 8d 00 00 00    	jle    0x1800691b0
   180069123:	4c 8d 3d 96 4b 04 00 	lea    r15,[rip+0x44b96]        # 0x1800adcc0
   18006912a:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
   180069130:	48 63 c3             	movsxd rax,ebx
   180069133:	4c 8d 8e a0 94 0d 00 	lea    r9,[rsi+0xd94a0]
   18006913a:	ba 01 00 00 00       	mov    edx,0x1
   18006913f:	48 8b cf             	mov    rcx,rdi
   180069142:	4c 8d 04 40          	lea    r8,[rax+rax*2]
   180069146:	49 c1 e0 09          	shl    r8,0x9
   18006914a:	42 8b 84 06 1c 96 0d 	mov    eax,DWORD PTR [rsi+r8*1+0xd961c]
   180069151:	00
   180069152:	83 f8 02             	cmp    eax,0x2
   180069155:	f2 42 0f 10 94 06 28 	movsd  xmm2,QWORD PTR [rsi+r8*1+0xd9628]
   18006915c:	96 0d 00
   18006915f:	0f 46 d0             	cmovbe edx,eax
   180069162:	42 39 ac 06 24 96 0d 	cmp    DWORD PTR [rsi+r8*1+0xd9624],ebp
   180069169:	00
   18006916a:	48 63 d2             	movsxd rdx,edx
   18006916d:	48 8b c7             	mov    rax,rdi
   180069170:	49 0f 45 cf          	cmovne rcx,r15
   180069174:	42 39 ac 06 20 96 0d 	cmp    DWORD PTR [rsi+r8*1+0xd9620],ebp
   18006917b:	00
   18006917c:	48 89 4c 24 28       	mov    QWORD PTR [rsp+0x28],rcx
   180069181:	48 8d 0d 68 4b 04 00 	lea    rcx,[rip+0x44b68]        # 0x1800adcf0
   180069188:	49 0f 45 c6          	cmovne rax,r14
   18006918c:	4d 03 c8             	add    r9,r8
   18006918f:	48 8b 94 d6 d8 d4 08 	mov    rdx,QWORD PTR [rsi+rdx*8+0x8d4d8]
   180069196:	00
   180069197:	66 49 0f 7e d0       	movq   r8,xmm2
   18006919c:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800691a1:	e8 4a ad fe ff       	call   0x180053ef0
   1800691a6:	ff c3                	inc    ebx
   1800691a8:	3b 1d f2 26 10 00    	cmp    ebx,DWORD PTR [rip+0x1026f2]        # 0x18016b8a0
   1800691ae:	7c 80                	jl     0x180069130
   1800691b0:	8b dd                	mov    ebx,ebp
   1800691b2:	39 1d ec 26 10 00    	cmp    DWORD PTR [rip+0x1026ec],ebx        # 0x18016b8a4
   1800691b8:	7e 71                	jle    0x18006922b
   1800691ba:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
   1800691c0:	48 63 c3             	movsxd rax,ebx
   1800691c3:	4c 8d 8e b0 b8 16 00 	lea    r9,[rsi+0x16b8b0]
   1800691ca:	4c 69 c0 20 02 00 00 	imul   r8,rax,0x220
   1800691d1:	b9 01 00 00 00       	mov    ecx,0x1
   1800691d6:	42 8b 84 06 c0 ba 16 	mov    eax,DWORD PTR [rsi+r8*1+0x16bac0]
   1800691dd:	00
   1800691de:	83 f8 02             	cmp    eax,0x2
   1800691e1:	f2 42 0f 10 94 06 c8 	movsd  xmm2,QWORD PTR [rsi+r8*1+0x16bac8]
   1800691e8:	ba 16 00
   1800691eb:	0f 46 c8             	cmovbe ecx,eax
   1800691ee:	42 39 ac 06 c4 ba 16 	cmp    DWORD PTR [rsi+r8*1+0x16bac4],ebp
   1800691f5:	00
   1800691f6:	48 63 d1             	movsxd rdx,ecx
   1800691f9:	48 8b c7             	mov    rax,rdi
   1800691fc:	49 0f 45 c6          	cmovne rax,r14
   180069200:	48 8d 0d 11 4b 04 00 	lea    rcx,[rip+0x44b11]        # 0x1800add18
   180069207:	4d 03 c8             	add    r9,r8
   18006920a:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18006920f:	66 49 0f 7e d0       	movq   r8,xmm2
   180069214:	48 8b 94 d6 d8 d4 08 	mov    rdx,QWORD PTR [rsi+rdx*8+0x8d4d8]
   18006921b:	00
   18006921c:	e8 cf ac fe ff       	call   0x180053ef0
   180069221:	ff c3                	inc    ebx
   180069223:	3b 1d 7b 26 10 00    	cmp    ebx,DWORD PTR [rip+0x10267b]        # 0x18016b8a4
   180069229:	7c 95                	jl     0x1800691c0
   18006922b:	39 2d ff a0 05 00    	cmp    DWORD PTR [rip+0x5a0ff],ebp        # 0x1800c3330
   180069231:	75 11                	jne    0x180069244
   180069233:	48 8d 0d 96 58 02 00 	lea    rcx,[rip+0x25896]        # 0x18008ead0
   18006923a:	e8 b1 ac fe ff       	call   0x180053ef0
   18006923f:	e9 23 01 00 00       	jmp    0x180069367
   180069244:	48 39 2d 25 b4 14 00 	cmp    QWORD PTR [rip+0x14b425],rbp        # 0x1801b4670
   18006924b:	75 18                	jne    0x180069265
   18006924d:	48 8d 15 24 58 02 00 	lea    rdx,[rip+0x25824]        # 0x18008ea78
   180069254:	48 8d 0d 35 59 02 00 	lea    rcx,[rip+0x25935]        # 0x18008eb90
   18006925b:	e8 90 ac fe ff       	call   0x180053ef0
   180069260:	e9 02 01 00 00       	jmp    0x180069367
   180069265:	39 2d 85 b4 14 00    	cmp    DWORD PTR [rip+0x14b485],ebp        # 0x1801b46f0
   18006926b:	74 18                	je     0x180069285
   18006926d:	48 8d 15 04 58 02 00 	lea    rdx,[rip+0x25804]        # 0x18008ea78
   180069274:	48 8d 0d 75 59 02 00 	lea    rcx,[rip+0x25975]        # 0x18008ebf0
   18006927b:	e8 70 ac fe ff       	call   0x180053ef0
   180069280:	e9 e2 00 00 00       	jmp    0x180069367
   180069285:	8b 15 61 b4 14 00    	mov    edx,DWORD PTR [rip+0x14b461]        # 0x1801b46ec
   18006928b:	85 d2                	test   edx,edx
   18006928d:	75 3c                	jne    0x1800692cb
   18006928f:	48 39 2d 02 02 07 00 	cmp    QWORD PTR [rip+0x70202],rbp        # 0x1800d9498
   180069296:	48 8d 05 b3 59 02 00 	lea    rax,[rip+0x259b3]        # 0x18008ec50
   18006929d:	44 8b 05 50 b4 14 00 	mov    r8d,DWORD PTR [rip+0x14b450]        # 0x1801b46f4
   1800692a4:	48 8d 15 cd 57 02 00 	lea    rdx,[rip+0x257cd]        # 0x18008ea78
   1800692ab:	48 0f 45 c7          	cmovne rax,rdi
   1800692af:	48 8d 0d 0a 5a 02 00 	lea    rcx,[rip+0x25a0a]        # 0x18008ecc0
   1800692b6:	41 b9 ff 0f 00 00    	mov    r9d,0xfff
   1800692bc:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800692c1:	e8 2a ac fe ff       	call   0x180053ef0
   1800692c6:	e9 9c 00 00 00       	jmp    0x180069367
   1800692cb:	48 39 2d c6 01 07 00 	cmp    QWORD PTR [rip+0x701c6],rbp        # 0x1800d9498
   1800692d2:	75 12                	jne    0x1800692e6
   1800692d4:	48 8d 0d 25 5b 02 00 	lea    rcx,[rip+0x25b25]        # 0x18008ee00
   1800692db:	e8 10 ac fe ff       	call   0x180053ef0
   1800692e0:	8b 15 06 b4 14 00    	mov    edx,DWORD PTR [rip+0x14b406]        # 0x1801b46ec
   1800692e6:	8d 42 ff             	lea    eax,[rdx-0x1]
   1800692e9:	85 d2                	test   edx,edx
   1800692eb:	7f 02                	jg     0x1800692ef
   1800692ed:	8b c5                	mov    eax,ebp
   1800692ef:	44 8b 05 82 b3 14 00 	mov    r8d,DWORD PTR [rip+0x14b382]        # 0x1801b4678
   1800692f6:	48 8d 0d b3 5b 02 00 	lea    rcx,[rip+0x25bb3]        # 0x18008eeb0
   1800692fd:	4c 63 c8             	movsxd r9,eax
   180069300:	46 8b 8c 8e 78 46 1b 	mov    r9d,DWORD PTR [rsi+r9*4+0x1b4678]
   180069307:	00
   180069308:	e8 e3 ab fe ff       	call   0x180053ef0
   18006930d:	8b 05 65 b3 14 00    	mov    eax,DWORD PTR [rip+0x14b365]        # 0x1801b4678
   180069313:	85 c0                	test   eax,eax
   180069315:	75 23                	jne    0x18006933a
   180069317:	81 3d bb ac 13 00 7a 	cmp    DWORD PTR [rip+0x13acbb],0x187a        # 0x1801a3fdc
   18006931e:	18 00 00
   180069321:	75 17                	jne    0x18006933a
   180069323:	ba 7a 18 00 00       	mov    edx,0x187a
   180069328:	48 8d 0d 11 5c 02 00 	lea    rcx,[rip+0x25c11]        # 0x18008ef40
   18006932f:	e8 bc ab fe ff       	call   0x180053ef0
   180069334:	8b 05 3e b3 14 00    	mov    eax,DWORD PTR [rip+0x14b33e]        # 0x1801b4678
   18006933a:	44 8b 05 c7 9f 05 00 	mov    r8d,DWORD PTR [rip+0x59fc7]        # 0x1800c3308
   180069341:	48 8d 0d a8 5d 02 00 	lea    rcx,[rip+0x25da8]        # 0x18008f0f0
   180069348:	8b 15 8e ac 13 00    	mov    edx,DWORD PTR [rip+0x13ac8e]        # 0x1801a3fdc
   18006934e:	4c 8d 0d 2b 5e 02 00 	lea    r9,[rip+0x25e2b]        # 0x18008f180
   180069355:	85 c0                	test   eax,eax
   180069357:	4c 0f 45 c9          	cmovne r9,rcx
   18006935b:	48 8d 0d 4e 5e 02 00 	lea    rcx,[rip+0x25e4e]        # 0x18008f1b0
   180069362:	e8 89 ab fe ff       	call   0x180053ef0
   180069367:	48 8b 05 02 ba 05 00 	mov    rax,QWORD PTR [rip+0x5ba02]        # 0x1800c4d70
   18006936e:	4c 8d 05 2b 55 04 00 	lea    r8,[rip+0x4552b]        # 0x1800ae8a0
   180069375:	48 8b 0d fc b9 05 00 	mov    rcx,QWORD PTR [rip+0x5b9fc]        # 0x1800c4d78
   18006937c:	48 8d 15 d5 55 02 00 	lea    rdx,[rip+0x255d5]        # 0x18008e958
   180069383:	ff 50 40             	call   QWORD PTR [rax+0x40]
   180069386:	48 85 c0             	test   rax,rax
   180069389:	74 13                	je     0x18006939e
   18006938b:	48 8b 00             	mov    rax,QWORD PTR [rax]
   18006938e:	48 8d 0d 53 55 04 00 	lea    rcx,[rip+0x45553]        # 0x1800ae8e8
   180069395:	48 89 05 1c 00 07 00 	mov    QWORD PTR [rip+0x7001c],rax        # 0x1800d93b8
   18006939c:	eb 07                	jmp    0x1800693a5
   18006939e:	48 8d 0d 7b 55 04 00 	lea    rcx,[rip+0x4557b]        # 0x1800ae920
   1800693a5:	e8 46 ab fe ff       	call   0x180053ef0
   1800693aa:	e8 81 f3 fc ff       	call   0x180038730
   1800693af:	39 2d 37 9f 05 00    	cmp    DWORD PTR [rip+0x59f37],ebp        # 0x1800c32ec
   1800693b5:	0f 84 d5 00 00 00    	je     0x180069490
   1800693bb:	48 39 2d f6 cc 21 00 	cmp    QWORD PTR [rip+0x21ccf6],rbp        # 0x1802860b8
   1800693c2:	0f 84 c8 00 00 00    	je     0x180069490
   1800693c8:	48 8b 0d a9 b9 05 00 	mov    rcx,QWORD PTR [rip+0x5b9a9]        # 0x1800c4d78
   1800693cf:	48 8d 05 7a 55 04 00 	lea    rax,[rip+0x4557a]        # 0x1800ae950
   1800693d6:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   1800693db:	4c 8d 0d be eb ff ff 	lea    r9,[rip+0xffffffffffffebbe]        # 0x180067fa0
   1800693e2:	48 8d 05 cf cc 21 00 	lea    rax,[rip+0x21cccf]        # 0x1802860b8
   1800693e9:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800693ee:	4c 8d 05 13 2f 03 00 	lea    r8,[rip+0x32f13]        # 0x18009c308
   1800693f5:	48 8b 05 74 b9 05 00 	mov    rax,QWORD PTR [rip+0x5b974]        # 0x1800c4d70
   1800693fc:	48 8d 15 55 55 02 00 	lea    rdx,[rip+0x25555]        # 0x18008e958
   180069403:	ff 50 48             	call   QWORD PTR [rax+0x48]
   180069406:	48 8b 0d 6b b9 05 00 	mov    rcx,QWORD PTR [rip+0x5b96b]        # 0x1800c4d78
   18006940d:	4c 8d 0d 7c ec ff ff 	lea    r9,[rip+0xffffffffffffec7c]        # 0x180068090
   180069414:	8b d8                	mov    ebx,eax
   180069416:	4c 8d 05 23 2f 03 00 	lea    r8,[rip+0x32f23]        # 0x18009c340
   18006941d:	48 8d 05 4c 55 04 00 	lea    rax,[rip+0x4554c]        # 0x1800ae970
   180069424:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180069429:	48 8d 15 28 55 02 00 	lea    rdx,[rip+0x25528]        # 0x18008e958
   180069430:	48 8d 05 89 cc 21 00 	lea    rax,[rip+0x21cc89]        # 0x1802860c0
   180069437:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18006943c:	48 8b 05 2d b9 05 00 	mov    rax,QWORD PTR [rip+0x5b92d]        # 0x1800c4d70
   180069443:	ff 50 48             	call   QWORD PTR [rax+0x48]
   180069446:	85 db                	test   ebx,ebx
   180069448:	74 1d                	je     0x180069467
   18006944a:	85 c0                	test   eax,eax
   18006944c:	74 19                	je     0x180069467
   18006944e:	ba 78 e4 31 00       	mov    edx,0x31e478
   180069453:	48 8d 0d 36 55 04 00 	lea    rcx,[rip+0x45536]        # 0x1800ae990
   18006945a:	41 b8 09 e5 31 00    	mov    r8d,0x31e509
   180069460:	e8 8b aa fe ff       	call   0x180053ef0
   180069465:	eb 29                	jmp    0x180069490
   180069467:	85 c0                	test   eax,eax
   180069469:	48 8d 0d e0 a6 02 00 	lea    rcx,[rip+0x2a6e0]        # 0x180093b50
   180069470:	48 8d 15 c1 55 04 00 	lea    rdx,[rip+0x455c1]        # 0x1800aea38
   180069477:	4c 8b c2             	mov    r8,rdx
   18006947a:	4c 0f 45 c1          	cmovne r8,rcx
   18006947e:	85 db                	test   ebx,ebx
   180069480:	48 0f 45 d1          	cmovne rdx,rcx
   180069484:	48 8d 0d b5 55 04 00 	lea    rcx,[rip+0x455b5]        # 0x1800aea40
   18006948b:	e8 60 aa fe ff       	call   0x180053ef0
   180069490:	48 8b 0d e1 b8 05 00 	mov    rcx,QWORD PTR [rip+0x5b8e1]        # 0x1800c4d78
   180069497:	48 8d 05 12 56 04 00 	lea    rax,[rip+0x45612]        # 0x1800aeab0
   18006949e:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   1800694a3:	4c 8d 0d 96 ec ff ff 	lea    r9,[rip+0xffffffffffffec96]        # 0x180068140
   1800694aa:	48 8d 05 57 cc 21 00 	lea    rax,[rip+0x21cc57]        # 0x180286108
   1800694b1:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800694b6:	4c 8d 05 13 56 04 00 	lea    r8,[rip+0x45613]        # 0x1800aead0
   1800694bd:	48 8b 05 ac b8 05 00 	mov    rax,QWORD PTR [rip+0x5b8ac]        # 0x1800c4d70
   1800694c4:	48 8d 15 8d 54 02 00 	lea    rdx,[rip+0x2548d]        # 0x18008e958
   1800694cb:	ff 50 48             	call   QWORD PTR [rax+0x48]
   1800694ce:	85 c0                	test   eax,eax
   1800694d0:	74 0e                	je     0x1800694e0
   1800694d2:	48 8d 0d 1f 56 04 00 	lea    rcx,[rip+0x4561f]        # 0x1800aeaf8
   1800694d9:	e8 12 aa fe ff       	call   0x180053ef0
   1800694de:	eb 1b                	jmp    0x1800694fb
   1800694e0:	48 8d 0d 39 56 04 00 	lea    rcx,[rip+0x45639]        # 0x1800aeb20
   1800694e7:	e8 04 aa fe ff       	call   0x180053ef0
   1800694ec:	83 3d 19 76 21 00 01 	cmp    DWORD PTR [rip+0x217619],0x1        # 0x180280b0c
   1800694f3:	75 06                	jne    0x1800694fb
   1800694f5:	89 2d 11 76 21 00    	mov    DWORD PTR [rip+0x217611],ebp        # 0x180280b0c
   1800694fb:	48                   	rex.W
   1800694fc:	8b                   	.byte 0x8b
   1800694fd:	0d                   	.byte 0xd
   1800694fe:	76 b8                	jbe    0x1800694b8
