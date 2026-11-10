Static GNU objdump; DLL sha256 fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

000000018001e0a0 <.text+0x1d0a0>:
   18001e0a0:	4c 8b dc             	mov    r11,rsp
   18001e0a3:	55                   	push   rbp
   18001e0a4:	49 8d ab 88 fb ff ff 	lea    rbp,[r11-0x478]
   18001e0ab:	48 81 ec 70 05 00 00 	sub    rsp,0x570
   18001e0b2:	48 8b 05 c7 60 0a 00 	mov    rax,QWORD PTR [rip+0xa60c7]        # 0x1800c4180
   18001e0b9:	48 33 c4             	xor    rax,rsp
   18001e0bc:	48 89 85 a8 03 00 00 	mov    QWORD PTR [rbp+0x3a8],rax
   18001e0c3:	83 3d b6 7f 26 00 00 	cmp    DWORD PTR [rip+0x267fb6],0x0        # 0x180286080
   18001e0ca:	0f 84 40 0d 00 00    	je     0x18001ee10
   18001e0d0:	49 89 5b 08          	mov    QWORD PTR [r11+0x8],rbx
   18001e0d4:	49 89 73 10          	mov    QWORD PTR [r11+0x10],rsi
   18001e0d8:	49 89 7b 18          	mov    QWORD PTR [r11+0x18],rdi
   18001e0dc:	4d 89 6b e8          	mov    QWORD PTR [r11-0x18],r13
   18001e0e0:	4d 89 73 e0          	mov    QWORD PTR [r11-0x20],r14
   18001e0e4:	41 0f 29 73 c8       	movaps XMMWORD PTR [r11-0x38],xmm6
   18001e0e9:	41 0f 29 7b b8       	movaps XMMWORD PTR [r11-0x48],xmm7
   18001e0ee:	45 0f 29 43 a8       	movaps XMMWORD PTR [r11-0x58],xmm8
   18001e0f3:	45 0f 29 4b 98       	movaps XMMWORD PTR [r11-0x68],xmm9
   18001e0f8:	45 0f 29 53 88       	movaps XMMWORD PTR [r11-0x78],xmm10
   18001e0fd:	45 0f 29 9b 78 ff ff 	movaps XMMWORD PTR [r11-0x88],xmm11
   18001e104:	ff
   18001e105:	45 0f 29 a3 68 ff ff 	movaps XMMWORD PTR [r11-0x98],xmm12
   18001e10c:	ff
   18001e10d:	45 0f 29 ab 58 ff ff 	movaps XMMWORD PTR [r11-0xa8],xmm13
   18001e114:	ff
   18001e115:	45 0f 29 b3 48 ff ff 	movaps XMMWORD PTR [r11-0xb8],xmm14
   18001e11c:	ff
   18001e11d:	45 0f 29 bb 38 ff ff 	movaps XMMWORD PTR [r11-0xc8],xmm15
   18001e124:	ff
   18001e125:	e8 16 31 04 00       	call   0x180061240
   18001e12a:	f3 0f 59 05 a2 55 0a 	mulss  xmm0,DWORD PTR [rip+0xa55a2]        # 0x1800c36d4
   18001e131:	00
   18001e132:	8b 05 70 5e 18 00    	mov    eax,DWORD PTR [rip+0x185e70]        # 0x1801a3fa8
   18001e138:	f3 0f 11 44 24 50    	movss  DWORD PTR [rsp+0x50],xmm0
   18001e13e:	85 c0                	test   eax,eax
   18001e140:	74 0b                	je     0x18001e14d
   18001e142:	f3 44 0f 10 0d 61 5e 	movss  xmm9,DWORD PTR [rip+0x185e61]        # 0x1801a3fac
   18001e149:	18 00
   18001e14b:	eb 09                	jmp    0x18001e156
   18001e14d:	f3 44 0f 10 0d 26 50 	movss  xmm9,DWORD PTR [rip+0xa5026]        # 0x1800c317c
   18001e154:	0a 00
   18001e156:	f3 44 0f 59 c8       	mulss  xmm9,xmm0
   18001e15b:	85 c0                	test   eax,eax
   18001e15d:	74 0a                	je     0x18001e169
   18001e15f:	f3 0f 10 15 49 5e 18 	movss  xmm2,DWORD PTR [rip+0x185e49]        # 0x1801a3fb0
   18001e166:	00
   18001e167:	eb 0c                	jmp    0x18001e175
   18001e169:	f3 0f 10 15 0f 50 0a 	movss  xmm2,DWORD PTR [rip+0xa500f]        # 0x1800c3180
   18001e170:	00
   18001e171:	f3 0f 59 d0          	mulss  xmm2,xmm0
   18001e175:	e8 36 e9 ff ff       	call   0x18001cab0
   18001e17a:	f3 44 0f 10 05 59 55 	movss  xmm8,DWORD PTR [rip+0xa5559]        # 0x1800c36dc
   18001e181:	0a 00
   18001e183:	4c 8d 0d 76 1e fe ff 	lea    r9,[rip+0xfffffffffffe1e76]        # 0x180000000
   18001e18a:	f3 44 0f 59 05 bd 14 	mulss  xmm8,DWORD PTR [rip+0x914bd]        # 0x1800af650
   18001e191:	09 00
   18001e193:	33 db                	xor    ebx,ebx
   18001e195:	44 8b 05 08 d7 14 00 	mov    r8d,DWORD PTR [rip+0x14d708]        # 0x18016b8a4
   18001e19c:	33 d2                	xor    edx,edx
   18001e19e:	44 0f 28 e0          	movaps xmm12,xmm0
   18001e1a2:	f3 44 0f 58 e2       	addss  xmm12,xmm2
   18001e1a7:	f3 44 0f 59 05 64 10 	mulss  xmm8,DWORD PTR [rip+0x91064]        # 0x1800af214
   18001e1ae:	09 00
   18001e1b0:	f3 44 0f 5f 05 07 14 	maxss  xmm8,DWORD PTR [rip+0x91407]        # 0x1800af5c0
   18001e1b7:	09 00
   18001e1b9:	45 85 c0             	test   r8d,r8d
   18001e1bc:	7e 2f                	jle    0x18001e1ed
   18001e1be:	66 90                	xchg   ax,ax
   18001e1c0:	81 fb a0 00 00 00    	cmp    ebx,0xa0
   18001e1c6:	7d 25                	jge    0x18001e1ed
   18001e1c8:	48 63 c2             	movsxd rax,edx
   18001e1cb:	48 8d 0c 40          	lea    rcx,[rax+rax*2]
   18001e1cf:	48 03 c9             	add    rcx,rcx
   18001e1d2:	41 83 bc c9 b0 0c 18 	cmp    DWORD PTR [r9+rcx*8+0x180cb0],0x2
   18001e1d9:	00 02
   18001e1db:	75 09                	jne    0x18001e1e6
   18001e1dd:	48 63 c3             	movsxd rax,ebx
   18001e1e0:	ff c3                	inc    ebx
   18001e1e2:	89 54 84 60          	mov    DWORD PTR [rsp+rax*4+0x60],edx
   18001e1e6:	ff c2                	inc    edx
   18001e1e8:	41 3b d0             	cmp    edx,r8d
   18001e1eb:	7c d3                	jl     0x18001e1c0
   18001e1ed:	33 d2                	xor    edx,edx
   18001e1ef:	8b f3                	mov    esi,ebx
   18001e1f1:	45 85 c0             	test   r8d,r8d
   18001e1f4:	7e 2d                	jle    0x18001e223
   18001e1f6:	81 fb a0 00 00 00    	cmp    ebx,0xa0
   18001e1fc:	7d 25                	jge    0x18001e223
   18001e1fe:	48 63 c2             	movsxd rax,edx
   18001e201:	48 8d 0c 40          	lea    rcx,[rax+rax*2]
   18001e205:	48 03 c9             	add    rcx,rcx
   18001e208:	41 83 bc c9 b0 0c 18 	cmp    DWORD PTR [r9+rcx*8+0x180cb0],0x1
   18001e20f:	00 01
   18001e211:	75 09                	jne    0x18001e21c
   18001e213:	48 63 c3             	movsxd rax,ebx
   18001e216:	ff c3                	inc    ebx
   18001e218:	89 54 84 60          	mov    DWORD PTR [rsp+rax*4+0x60],edx
   18001e21c:	ff c2                	inc    edx
   18001e21e:	41 3b d0             	cmp    edx,r8d
   18001e221:	7c d3                	jl     0x18001e1f6
   18001e223:	8d 4b 09             	lea    ecx,[rbx+0x9]
   18001e226:	b8 67 66 66 66       	mov    eax,0x66666667
   18001e22b:	f7 e9                	imul   ecx
   18001e22d:	8b fa                	mov    edi,edx
   18001e22f:	c1 ff 02             	sar    edi,0x2
   18001e232:	8b c7                	mov    eax,edi
   18001e234:	c1 e8 1f             	shr    eax,0x1f
   18001e237:	03 f8                	add    edi,eax
   18001e239:	b8 01 00 00 00       	mov    eax,0x1
   18001e23e:	3b f8                	cmp    edi,eax
   18001e240:	0f 4c f8             	cmovl  edi,eax
   18001e243:	8b 05 3b 7e 26 00    	mov    eax,DWORD PTR [rip+0x267e3b]        # 0x180286084
   18001e249:	3b c7                	cmp    eax,edi
   18001e24b:	7c 08                	jl     0x18001e255
   18001e24d:	33 c0                	xor    eax,eax
   18001e24f:	89 05 2f 7e 26 00    	mov    DWORD PTR [rip+0x267e2f],eax        # 0x180286084
   18001e255:	8d 04 80             	lea    eax,[rax+rax*4]
   18001e258:	44 8b f3             	mov    r14d,ebx
   18001e25b:	44 8d 2c 00          	lea    r13d,[rax+rax*1]
   18001e25f:	41 8d 45 0a          	lea    eax,[r13+0xa]
   18001e263:	3b c3                	cmp    eax,ebx
   18001e265:	44 0f 4e f0          	cmovle r14d,eax
   18001e269:	44 89 74 24 54       	mov    DWORD PTR [rsp+0x54],r14d
   18001e26e:	e8 cd 2f 04 00       	call   0x180061240
   18001e273:	0f 28 e8             	movaps xmm5,xmm0
   18001e276:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e27a:	f3 0f 59 2d 52 54 0a 	mulss  xmm5,DWORD PTR [rip+0xa5452]        # 0x1800c36d4
   18001e281:	00
   18001e282:	0f 28 cd             	movaps xmm1,xmm5
   18001e285:	e8 66 2a 04 00       	call   0x180060cf0
   18001e28a:	f3 44 0f 10 2d 55 14 	movss  xmm13,DWORD PTR [rip+0x91455]        # 0x1800af6e8
   18001e291:	09 00
   18001e293:	45 0f 28 d9          	movaps xmm11,xmm9
   18001e297:	f3 44 0f 58 1d f0 13 	addss  xmm11,DWORD PTR [rip+0x913f0]        # 0x1800af690
   18001e29e:	09 00
   18001e2a0:	44 0f 2f d8          	comiss xmm11,xmm0
   18001e2a4:	76 0d                	jbe    0x18001e2b3
   18001e2a6:	0f 28 c5             	movaps xmm0,xmm5
   18001e2a9:	f3 41 0f 59 c5       	mulss  xmm0,xmm13
   18001e2ae:	f3 41 0f 58 c1       	addss  xmm0,xmm9
   18001e2b3:	f3 0f 10 15 1d 10 09 	movss  xmm2,DWORD PTR [rip+0x9101d]        # 0x1800af2d8
   18001e2ba:	00
   18001e2bb:	f3 41 0f 5c c1       	subss  xmm0,xmm9
   18001e2c0:	f3 44 0f 10 3d 27 10 	movss  xmm15,DWORD PTR [rip+0x91027]        # 0x1800af2f0
   18001e2c7:	09 00
   18001e2c9:	45 0f 28 d0          	movaps xmm10,xmm8
   18001e2cd:	f3 44 0f 59 15 a6 0f 	mulss  xmm10,DWORD PTR [rip+0x90fa6]        # 0x1800af27c
   18001e2d4:	09 00
   18001e2d6:	f3 44 0f 10 35 d9 11 	movss  xmm14,DWORD PTR [rip+0x911d9]        # 0x1800af4b8
   18001e2dd:	09 00
   18001e2df:	f3 44 0f 11 7c 24 38 	movss  DWORD PTR [rsp+0x38],xmm15
   18001e2e6:	f3 0f 11 54 24 30    	movss  DWORD PTR [rsp+0x30],xmm2
   18001e2ec:	41 0f 28 ca          	movaps xmm1,xmm10
   18001e2f0:	f3 0f 10 15 dc 0f 09 	movss  xmm2,DWORD PTR [rip+0x90fdc]        # 0x1800af2d4
   18001e2f7:	00
   18001e2f8:	f3 41 0f 58 cc       	addss  xmm1,xmm12
   18001e2fd:	f3 0f 11 54 24 28    	movss  DWORD PTR [rsp+0x28],xmm2
   18001e303:	f3 0f 10 15 c5 0f 09 	movss  xmm2,DWORD PTR [rip+0x90fc5]        # 0x1800af2d0
   18001e30a:	00
   18001e30b:	f3 41 0f 59 ee       	mulss  xmm5,xmm14
   18001e310:	f3 0f 11 54 24 20    	movss  DWORD PTR [rsp+0x20],xmm2
   18001e316:	0f 28 d0             	movaps xmm2,xmm0
   18001e319:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e31d:	0f 28 dd             	movaps xmm3,xmm5
   18001e320:	e8 ab 8a fe ff       	call   0x180006dd0
   18001e325:	8b 05 59 7d 26 00    	mov    eax,DWORD PTR [rip+0x267d59]        # 0x180286084
   18001e32b:	4c 8d 0d be 33 08 00 	lea    r9,[rip+0x833be]        # 0x1800a16f0
   18001e332:	41 0f 28 f8          	movaps xmm7,xmm8
   18001e336:	89 7c 24 38          	mov    DWORD PTR [rsp+0x38],edi
   18001e33a:	f3 0f 59 3d 7e 0f 09 	mulss  xmm7,DWORD PTR [rip+0x90f7e]        # 0x1800af2c0
   18001e341:	00
   18001e342:	8b cb                	mov    ecx,ebx
   18001e344:	2b ce                	sub    ecx,esi
   18001e346:	ff c0                	inc    eax
   18001e348:	89 44 24 30          	mov    DWORD PTR [rsp+0x30],eax
   18001e34c:	ba dc 00 00 00       	mov    edx,0xdc
   18001e351:	89 4c 24 28          	mov    DWORD PTR [rsp+0x28],ecx
   18001e355:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001e35c:	0f 28 f7             	movaps xmm6,xmm7
   18001e35f:	89 74 24 20          	mov    DWORD PTR [rsp+0x20],esi
   18001e363:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001e36a:	f3 41 0f 58 f4       	addss  xmm6,xmm12
   18001e36f:	e8 dc b6 04 00       	call   0x180069a50
   18001e374:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001e37b:	41 b8 ff ff ff ff    	mov    r8d,0xffffffff
   18001e381:	0f 28 ce             	movaps xmm1,xmm6
   18001e384:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e388:	e8 c3 48 00 00       	call   0x180022c50
   18001e38d:	45 0f 28 e0          	movaps xmm12,xmm8
   18001e391:	f3 44 0f 58 e6       	addss  xmm12,xmm6
   18001e396:	e8 a5 2e 04 00       	call   0x180061240
   18001e39b:	0f 28 c8             	movaps xmm1,xmm0
   18001e39e:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e3a2:	f3 0f 59 0d 2a 53 0a 	mulss  xmm1,DWORD PTR [rip+0xa532a]        # 0x1800c36d4
   18001e3a9:	00
   18001e3aa:	e8 41 29 04 00       	call   0x180060cf0
   18001e3af:	44 0f 2f d8          	comiss xmm11,xmm0
   18001e3b3:	76 0d                	jbe    0x18001e3c2
   18001e3b5:	0f 28 c1             	movaps xmm0,xmm1
   18001e3b8:	f3 41 0f 59 c5       	mulss  xmm0,xmm13
   18001e3bd:	f3 41 0f 58 c1       	addss  xmm0,xmm9
   18001e3c2:	f3 0f 10 15 0e 0f 09 	movss  xmm2,DWORD PTR [rip+0x90f0e]        # 0x1800af2d8
   18001e3c9:	00
   18001e3ca:	f3 41 0f 5c c1       	subss  xmm0,xmm9
   18001e3cf:	f3 44 0f 11 7c 24 38 	movss  DWORD PTR [rsp+0x38],xmm15
   18001e3d6:	f3 45 0f 58 d4       	addss  xmm10,xmm12
   18001e3db:	f3 0f 11 54 24 30    	movss  DWORD PTR [rsp+0x30],xmm2
   18001e3e1:	f3 0f 10 15 eb 0e 09 	movss  xmm2,DWORD PTR [rip+0x90eeb]        # 0x1800af2d4
   18001e3e8:	00
   18001e3e9:	f3 0f 11 54 24 28    	movss  DWORD PTR [rsp+0x28],xmm2
   18001e3ef:	f3 0f 10 15 d9 0e 09 	movss  xmm2,DWORD PTR [rip+0x90ed9]        # 0x1800af2d0
   18001e3f6:	00
   18001e3f7:	f3 41 0f 59 ce       	mulss  xmm1,xmm14
   18001e3fc:	4c 89 a4 24 68 05 00 	mov    QWORD PTR [rsp+0x568],r12
   18001e403:	00
   18001e404:	f3 0f 11 54 24 20    	movss  DWORD PTR [rsp+0x20],xmm2
   18001e40a:	0f 28 d0             	movaps xmm2,xmm0
   18001e40d:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e411:	4c 89 bc 24 50 05 00 	mov    QWORD PTR [rsp+0x550],r15
   18001e418:	00
   18001e419:	0f 28 d9             	movaps xmm3,xmm1
   18001e41c:	41 0f 28 ca          	movaps xmm1,xmm10
   18001e420:	e8 ab 89 fe ff       	call   0x180006dd0
   18001e425:	f3 44 0f 10 6c 24 50 	movss  xmm13,DWORD PTR [rsp+0x50]
   18001e42c:	f3 41 0f 58 fc       	addss  xmm7,xmm12
   18001e431:	45 0f 28 f5          	movaps xmm14,xmm13
   18001e435:	41 bf dc dc dc ff    	mov    r15d,0xffdcdcdc
   18001e43b:	f3 44 0f 59 35 54 11 	mulss  xmm14,DWORD PTR [rip+0x91154]        # 0x1800af598
   18001e442:	09 00
   18001e444:	f3 41 0f 58 fe       	addss  xmm7,xmm14
   18001e449:	85 db                	test   ebx,ebx
   18001e44b:	0f 85 b7 00 00 00    	jne    0x18001e508
   18001e451:	4c 8d 0d d8 32 08 00 	lea    r9,[rip+0x832d8]        # 0x1800a1730
   18001e458:	45 8b c7             	mov    r8d,r15d
   18001e45b:	0f 28 cf             	movaps xmm1,xmm7
   18001e45e:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e462:	e8 e9 47 00 00       	call   0x180022c50
   18001e467:	41 0f 28 f0          	movaps xmm6,xmm8
   18001e46b:	4c 8d 0d e6 32 08 00 	lea    r9,[rip+0x832e6]        # 0x1800a1758
   18001e472:	f3 0f 58 f7          	addss  xmm6,xmm7
   18001e476:	45 8b c7             	mov    r8d,r15d
   18001e479:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e47d:	0f 28 ce             	movaps xmm1,xmm6
   18001e480:	e8 cb 47 00 00       	call   0x180022c50
   18001e485:	39 1d 45 4c 0a 00    	cmp    DWORD PTR [rip+0xa4c45],ebx        # 0x1800c30d0
   18001e48b:	0f 84 ef 08 00 00    	je     0x18001ed80
   18001e491:	39 1d f9 69 0a 00    	cmp    DWORD PTR [rip+0xa69f9],ebx        # 0x1800c4e90
   18001e497:	0f 85 e3 08 00 00    	jne    0x18001ed80
   18001e49d:	39 1d f1 69 0a 00    	cmp    DWORD PTR [rip+0xa69f1],ebx        # 0x1800c4e94
   18001e4a3:	0f 85 d7 08 00 00    	jne    0x18001ed80
   18001e4a9:	41 0f 28 f8          	movaps xmm7,xmm8
   18001e4ad:	4c 8d 0d dc 32 08 00 	lea    r9,[rip+0x832dc]        # 0x1800a1790
   18001e4b4:	f3 0f 58 fe          	addss  xmm7,xmm6
   18001e4b8:	41 bc 60 d0 ff ff    	mov    r12d,0xffffd060
   18001e4be:	45 8b c4             	mov    r8d,r12d
   18001e4c1:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e4c5:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001e4ca:	0f 28 cf             	movaps xmm1,xmm7
   18001e4cd:	e8 7e 47 00 00       	call   0x180022c50
   18001e4d2:	41 0f 28 f0          	movaps xmm6,xmm8
   18001e4d6:	4c 8d 0d 03 33 08 00 	lea    r9,[rip+0x83303]        # 0x1800a17e0
   18001e4dd:	f3 0f 58 f7          	addss  xmm6,xmm7
   18001e4e1:	45 8b c4             	mov    r8d,r12d
   18001e4e4:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e4e8:	0f 28 ce             	movaps xmm1,xmm6
   18001e4eb:	e8 60 47 00 00       	call   0x180022c50
   18001e4f0:	f3 44 0f 58 c6       	addss  xmm8,xmm6
   18001e4f5:	45 8b c4             	mov    r8d,r12d
   18001e4f8:	4c 8d 0d 29 33 08 00 	lea    r9,[rip+0x83329]        # 0x1800a1828
   18001e4ff:	41 0f 28 c8          	movaps xmm1,xmm8
   18001e503:	e9 6f 08 00 00       	jmp    0x18001ed77
   18001e508:	45 3b ee             	cmp    r13d,r14d
   18001e50b:	0f 8d 53 08 00 00    	jge    0x18001ed64
   18001e511:	f3 44 0f 59 2d 06 10 	mulss  xmm13,DWORD PTR [rip+0x91006]        # 0x1800af520
   18001e518:	09 00
   18001e51a:	4c 8d 15 8f d3 14 00 	lea    r10,[rip+0x14d38f]        # 0x18016b8b0
   18001e521:	48 8d 15 e0 fd 06 00 	lea    rdx,[rip+0x6fde0]        # 0x18008e308
   18001e528:	41 bc 60 d0 ff ff    	mov    r12d,0xffffd060
   18001e52e:	f2 44 0f 10 25 31 10 	movsd  xmm12,QWORD PTR [rip+0x91031]        # 0x1800af568
   18001e535:	09 00
   18001e537:	4c 8d 1d f1 f8 06 00 	lea    r11,[rip+0x6f8f1]        # 0x18008de2f
   18001e53e:	f2 44 0f 10 3d 59 10 	movsd  xmm15,QWORD PTR [rip+0x91059]        # 0x1800af5a0
   18001e545:	09 00
   18001e547:	4c 8d 35 b2 1a fe ff 	lea    r14,[rip+0xfffffffffffe1ab2]        # 0x180000000
   18001e54e:	45 0f 57 db          	xorps  xmm11,xmm11
   18001e552:	49 63 c5             	movsxd rax,r13d
   18001e555:	48 8d 35 e8 fd 06 00 	lea    rsi,[rip+0x6fde8]        # 0x18008e344
   18001e55c:	4c 8d 05 75 d3 14 00 	lea    r8,[rip+0x14d375]        # 0x18016b8d8
   18001e563:	44 0f 28 d7          	movaps xmm10,xmm7
   18001e567:	48 63 4c 84 60       	movsxd rcx,DWORD PTR [rsp+rax*4+0x60]
   18001e56c:	48 69 d9 20 02 00 00 	imul   rbx,rcx,0x220
   18001e573:	48 8d 3c 49          	lea    rdi,[rcx+rcx*2]
   18001e577:	48 c1 e7 04          	shl    rdi,0x4
   18001e57b:	41 83 bc 1a e8 00 00 	cmp    DWORD PTR [r10+rbx*1+0xe8],0x1
   18001e582:	00 01
   18001e584:	45 8b b4 3e b0 0c 18 	mov    r14d,DWORD PTR [r14+rdi*1+0x180cb0]
   18001e58b:	00
   18001e58c:	48 0f 44 f2          	cmove  rsi,rdx
   18001e590:	41 80 7c 1a 28 00    	cmp    BYTE PTR [r10+rbx*1+0x28],0x0
   18001e596:	4d 0f 44 c2          	cmove  r8,r10
   18001e59a:	4c 03 c3             	add    r8,rbx
   18001e59d:	83 3d 3c 4e 0a 00 00 	cmp    DWORD PTR [rip+0xa4e3c],0x0        # 0x1800c33e0
   18001e5a4:	74 30                	je     0x18001e5d6
   18001e5a6:	83 3d 37 4e 0a 00 00 	cmp    DWORD PTR [rip+0xa4e37],0x0        # 0x1800c33e4
   18001e5ad:	74 27                	je     0x18001e5d6
   18001e5af:	41 8b 84 1a 10 02 00 	mov    eax,DWORD PTR [r10+rbx*1+0x210]
   18001e5b6:	00
   18001e5b7:	4c 8d 0d 42 1a fe ff 	lea    r9,[rip+0xfffffffffffe1a42]        # 0x180000000
   18001e5be:	83 f8 02             	cmp    eax,0x2
   18001e5c1:	b9 01 00 00 00       	mov    ecx,0x1
   18001e5c6:	0f 46 c8             	cmovbe ecx,eax
   18001e5c9:	48 63 c1             	movsxd rax,ecx
   18001e5cc:	49 8b 94 c1 08 d5 08 	mov    rdx,QWORD PTR [r9+rax*8+0x8d508]
   18001e5d3:	00
   18001e5d4:	eb 0a                	jmp    0x18001e5e0
   18001e5d6:	49 8b d3             	mov    rdx,r11
   18001e5d9:	4c 8d 0d 20 1a fe ff 	lea    r9,[rip+0xfffffffffffe1a20]        # 0x180000000
   18001e5e0:	49 63 84 1a ec 00 00 	movsxd rax,DWORD PTR [r10+rbx*1+0xec]
   18001e5e7:	00
   18001e5e8:	48 8d 0d 69 32 08 00 	lea    rcx,[rip+0x83269]        # 0x1800a1858
   18001e5ef:	41 83 fe 02          	cmp    r14d,0x2
   18001e5f3:	49 0f 44 cb          	cmove  rcx,r11
   18001e5f7:	49 8b 84 c1 80 d5 08 	mov    rax,QWORD PTR [r9+rax*8+0x8d580]
   18001e5fe:	00
   18001e5ff:	4c 8d 0d 6a 32 08 00 	lea    r9,[rip+0x8326a]        # 0x1800a1870
   18001e606:	48 89 4c 24 40       	mov    QWORD PTR [rsp+0x40],rcx
   18001e60b:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001e612:	48 89 44 24 38       	mov    QWORD PTR [rsp+0x38],rax
   18001e617:	48 89 74 24 30       	mov    QWORD PTR [rsp+0x30],rsi
   18001e61c:	4c 89 44 24 28       	mov    QWORD PTR [rsp+0x28],r8
   18001e621:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001e628:	48 89 54 24 20       	mov    QWORD PTR [rsp+0x20],rdx
   18001e62d:	ba dc 00 00 00       	mov    edx,0xdc
   18001e632:	e8 19 b4 04 00       	call   0x180069a50
   18001e637:	45 8b c7             	mov    r8d,r15d
   18001e63a:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001e641:	b8 90 f0 90 ff       	mov    eax,0xff90f090
   18001e646:	41 83 fe 02          	cmp    r14d,0x2
   18001e64a:	0f 28 cf             	movaps xmm1,xmm7
   18001e64d:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e651:	44 0f 44 c0          	cmove  r8d,eax
   18001e655:	e8 f6 45 00 00       	call   0x180022c50
   18001e65a:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001e65f:	41 83 fe 02          	cmp    r14d,0x2
   18001e663:	0f 85 48 06 00 00    	jne    0x18001ecb1
   18001e669:	48 8d 35 40 d2 14 00 	lea    rsi,[rip+0x14d240]        # 0x18016b8b0
   18001e670:	f2 0f 10 8c 1e 00 01 	movsd  xmm1,QWORD PTR [rsi+rbx*1+0x100]
   18001e677:	00 00
   18001e679:	4c 8d 35 80 19 fe ff 	lea    r14,[rip+0xfffffffffffe1980]        # 0x180000000
   18001e680:	66 41 0f 2f cb       	comisd xmm1,xmm11
   18001e685:	76 15                	jbe    0x18001e69c
   18001e687:	f2 41 0f 10 84 3e b8 	movsd  xmm0,QWORD PTR [r14+rdi*1+0x180cb8]
   18001e68e:	0c 18 00
   18001e691:	f2 0f 5e c1          	divsd  xmm0,xmm1
   18001e695:	f2 41 0f 59 c4       	mulsd  xmm0,xmm12
   18001e69a:	eb 04                	jmp    0x18001e6a0
   18001e69c:	41 0f 28 c3          	movaps xmm0,xmm11
   18001e6a0:	f2 41 0f 10 94 3e b8 	movsd  xmm2,QWORD PTR [r14+rdi*1+0x180cb8]
   18001e6a7:	0c 18 00
   18001e6aa:	48 8d 8d 08 03 00 00 	lea    rcx,[rbp+0x308]
   18001e6b1:	41 0f 28 f7          	movaps xmm6,xmm15
   18001e6b5:	ba 28 00 00 00       	mov    edx,0x28
   18001e6ba:	f2 0f 5d f0          	minsd  xmm6,xmm0
   18001e6be:	e8 4d 13 01 00       	call   0x18002fa10
   18001e6c3:	f2 0f 10 94 1e 00 01 	movsd  xmm2,QWORD PTR [rsi+rbx*1+0x100]
   18001e6ca:	00 00
   18001e6cc:	48 8d 8d e0 02 00 00 	lea    rcx,[rbp+0x2e0]
   18001e6d3:	ba 28 00 00 00       	mov    edx,0x28
   18001e6d8:	e8 33 13 01 00       	call   0x18002fa10
   18001e6dd:	33 c9                	xor    ecx,ecx
   18001e6df:	ba 23 00 00 00       	mov    edx,0x23
   18001e6e4:	0f 28 c6             	movaps xmm0,xmm6
   18001e6e7:	f2 0f 5e 05 b9 0d 09 	divsd  xmm0,QWORD PTR [rip+0x90db9]        # 0x1800af4a8
   18001e6ee:	00
   18001e6ef:	f2 0f 2c c0          	cvttsd2si eax,xmm0
   18001e6f3:	85 c0                	test   eax,eax
   18001e6f5:	0f 49 c8             	cmovns ecx,eax
   18001e6f8:	b8 14 00 00 00       	mov    eax,0x14
   18001e6fd:	83 f9 14             	cmp    ecx,0x14
   18001e700:	0f 4f c8             	cmovg  ecx,eax
   18001e703:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e708:	85 c9                	test   ecx,ecx
   18001e70a:	0f 4f c2             	cmovg  eax,edx
   18001e70d:	83 f9 01             	cmp    ecx,0x1
   18001e710:	88 85 e0 01 00 00    	mov    BYTE PTR [rbp+0x1e0],al
   18001e716:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e71b:	0f 4f c2             	cmovg  eax,edx
   18001e71e:	83 f9 02             	cmp    ecx,0x2
   18001e721:	88 85 e1 01 00 00    	mov    BYTE PTR [rbp+0x1e1],al
   18001e727:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e72c:	0f 4f c2             	cmovg  eax,edx
   18001e72f:	83 f9 03             	cmp    ecx,0x3
   18001e732:	88 85 e2 01 00 00    	mov    BYTE PTR [rbp+0x1e2],al
   18001e738:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e73d:	0f 4f c2             	cmovg  eax,edx
   18001e740:	83 f9 04             	cmp    ecx,0x4
   18001e743:	88 85 e3 01 00 00    	mov    BYTE PTR [rbp+0x1e3],al
   18001e749:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e74e:	0f 4f c2             	cmovg  eax,edx
   18001e751:	83 f9 05             	cmp    ecx,0x5
   18001e754:	88 85 e4 01 00 00    	mov    BYTE PTR [rbp+0x1e4],al
   18001e75a:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e75f:	0f 4f c2             	cmovg  eax,edx
   18001e762:	83 f9 06             	cmp    ecx,0x6
   18001e765:	88 85 e5 01 00 00    	mov    BYTE PTR [rbp+0x1e5],al
   18001e76b:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e770:	0f 4f c2             	cmovg  eax,edx
   18001e773:	83 f9 07             	cmp    ecx,0x7
   18001e776:	88 85 e6 01 00 00    	mov    BYTE PTR [rbp+0x1e6],al
   18001e77c:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e781:	0f 4f c2             	cmovg  eax,edx
   18001e784:	83 f9 08             	cmp    ecx,0x8
   18001e787:	88 85 e7 01 00 00    	mov    BYTE PTR [rbp+0x1e7],al
   18001e78d:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e792:	0f 4f c2             	cmovg  eax,edx
   18001e795:	83 f9 09             	cmp    ecx,0x9
   18001e798:	88 85 e8 01 00 00    	mov    BYTE PTR [rbp+0x1e8],al
   18001e79e:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7a3:	0f 4f c2             	cmovg  eax,edx
   18001e7a6:	83 f9 0a             	cmp    ecx,0xa
   18001e7a9:	88 85 e9 01 00 00    	mov    BYTE PTR [rbp+0x1e9],al
   18001e7af:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7b4:	0f 4f c2             	cmovg  eax,edx
   18001e7b7:	83 f9 0b             	cmp    ecx,0xb
   18001e7ba:	88 85 ea 01 00 00    	mov    BYTE PTR [rbp+0x1ea],al
   18001e7c0:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7c5:	0f 4f c2             	cmovg  eax,edx
   18001e7c8:	83 f9 0c             	cmp    ecx,0xc
   18001e7cb:	88 85 eb 01 00 00    	mov    BYTE PTR [rbp+0x1eb],al
   18001e7d1:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7d6:	0f 4f c2             	cmovg  eax,edx
   18001e7d9:	83 f9 0d             	cmp    ecx,0xd
   18001e7dc:	88 85 ec 01 00 00    	mov    BYTE PTR [rbp+0x1ec],al
   18001e7e2:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7e7:	0f 4f c2             	cmovg  eax,edx
   18001e7ea:	83 f9 0e             	cmp    ecx,0xe
   18001e7ed:	88 85 ed 01 00 00    	mov    BYTE PTR [rbp+0x1ed],al
   18001e7f3:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e7f8:	0f 4f c2             	cmovg  eax,edx
   18001e7fb:	88 85 ee 01 00 00    	mov    BYTE PTR [rbp+0x1ee],al
   18001e801:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e806:	83 f9 0f             	cmp    ecx,0xf
   18001e809:	c6 85 f4 01 00 00 00 	mov    BYTE PTR [rbp+0x1f4],0x0
   18001e810:	4c 8d 0d 71 30 08 00 	lea    r9,[rip+0x83071]        # 0x1800a1888
   18001e817:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001e81e:	0f 4f c2             	cmovg  eax,edx
   18001e821:	83 f9 10             	cmp    ecx,0x10
   18001e824:	88 85 ef 01 00 00    	mov    BYTE PTR [rbp+0x1ef],al
   18001e82a:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e82f:	0f 4f c2             	cmovg  eax,edx
   18001e832:	83 f9 11             	cmp    ecx,0x11
   18001e835:	88 85 f0 01 00 00    	mov    BYTE PTR [rbp+0x1f0],al
   18001e83b:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e840:	0f 4f c2             	cmovg  eax,edx
   18001e843:	83 f9 12             	cmp    ecx,0x12
   18001e846:	88 85 f1 01 00 00    	mov    BYTE PTR [rbp+0x1f1],al
   18001e84c:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e851:	0f 4f c2             	cmovg  eax,edx
   18001e854:	83 f9 13             	cmp    ecx,0x13
   18001e857:	88 85 f2 01 00 00    	mov    BYTE PTR [rbp+0x1f2],al
   18001e85d:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001e864:	b8 2e 00 00 00       	mov    eax,0x2e
   18001e869:	0f 4f c2             	cmovg  eax,edx
   18001e86c:	ba dc 00 00 00       	mov    edx,0xdc
   18001e871:	88 85 f3 01 00 00    	mov    BYTE PTR [rbp+0x1f3],al
   18001e877:	48 8d 85 e0 02 00 00 	lea    rax,[rbp+0x2e0]
   18001e87e:	48 89 44 24 38       	mov    QWORD PTR [rsp+0x38],rax
   18001e883:	48 8d 85 08 03 00 00 	lea    rax,[rbp+0x308]
   18001e88a:	48 89 44 24 30       	mov    QWORD PTR [rsp+0x30],rax
   18001e88f:	48 8d 85 e0 01 00 00 	lea    rax,[rbp+0x1e0]
   18001e896:	f2 0f 11 74 24 28    	movsd  QWORD PTR [rsp+0x28],xmm6
   18001e89c:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18001e8a1:	e8 aa b1 04 00       	call   0x180069a50
   18001e8a6:	66 41 0f 2f f4       	comisd xmm6,xmm12
   18001e8ab:	41 b8 ff c8 70 ff    	mov    r8d,0xff70c8ff
   18001e8b1:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001e8b8:	b8 90 f0 90 ff       	mov    eax,0xff90f090
   18001e8bd:	0f 28 cf             	movaps xmm1,xmm7
   18001e8c0:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e8c4:	44 0f 43 c0          	cmovae r8d,eax
   18001e8c8:	e8 83 43 00 00       	call   0x180022c50
   18001e8cd:	48 63 84 1e ec 00 00 	movsxd rax,DWORD PTR [rsi+rbx*1+0xec]
   18001e8d4:	00
   18001e8d5:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001e8da:	0f 28 f7             	movaps xmm6,xmm7
   18001e8dd:	83 f8 13             	cmp    eax,0x13
   18001e8e0:	0f 87 9a 00 00 00    	ja     0x18001e980
   18001e8e6:	41 8b 8c 86 28 ee 01 	mov    ecx,DWORD PTR [r14+rax*4+0x1ee28]
   18001e8ed:	00
   18001e8ee:	49 03 ce             	add    rcx,r14
   18001e8f1:	ff e1                	jmp    rcx
   18001e8f3:	48 8d 05 ee 2a 08 00 	lea    rax,[rip+0x82aee]        # 0x1800a13e8
   18001e8fa:	e9 88 00 00 00       	jmp    0x18001e987
   18001e8ff:	48 8d 05 12 2b 08 00 	lea    rax,[rip+0x82b12]        # 0x1800a1418
   18001e906:	e9 7c 00 00 00       	jmp    0x18001e987
   18001e90b:	48 8d 05 36 2b 08 00 	lea    rax,[rip+0x82b36]        # 0x1800a1448
   18001e912:	eb 73                	jmp    0x18001e987
   18001e914:	48 8d 05 6d 2b 08 00 	lea    rax,[rip+0x82b6d]        # 0x1800a1488
   18001e91b:	eb 6a                	jmp    0x18001e987
   18001e91d:	48 8d 05 9c 2b 08 00 	lea    rax,[rip+0x82b9c]        # 0x1800a14c0
   18001e924:	eb 61                	jmp    0x18001e987
   18001e926:	48 8d 05 bb 2b 08 00 	lea    rax,[rip+0x82bbb]        # 0x1800a14e8
   18001e92d:	eb 58                	jmp    0x18001e987
   18001e92f:	48 8d 05 ea 2b 08 00 	lea    rax,[rip+0x82bea]        # 0x1800a1520
   18001e936:	eb 4f                	jmp    0x18001e987
   18001e938:	48 8d 05 21 2c 08 00 	lea    rax,[rip+0x82c21]        # 0x1800a1560
   18001e93f:	eb 46                	jmp    0x18001e987
   18001e941:	48 8d 05 68 2c 08 00 	lea    rax,[rip+0x82c68]        # 0x1800a15b0
   18001e948:	eb 3d                	jmp    0x18001e987
   18001e94a:	48 8d 05 a7 2c 08 00 	lea    rax,[rip+0x82ca7]        # 0x1800a15f8
   18001e951:	eb 34                	jmp    0x18001e987
   18001e953:	48 8d 05 be 2c 08 00 	lea    rax,[rip+0x82cbe]        # 0x1800a1618
   18001e95a:	eb 2b                	jmp    0x18001e987
   18001e95c:	48 8d 05 dd 2c 08 00 	lea    rax,[rip+0x82cdd]        # 0x1800a1640
   18001e963:	eb 22                	jmp    0x18001e987
   18001e965:	48 8d 05 ec 2c 08 00 	lea    rax,[rip+0x82cec]        # 0x1800a1658
   18001e96c:	eb 19                	jmp    0x18001e987
   18001e96e:	48 8d 05 03 2d 08 00 	lea    rax,[rip+0x82d03]        # 0x1800a1678
   18001e975:	eb 10                	jmp    0x18001e987
   18001e977:	48 8d 05 1a 2d 08 00 	lea    rax,[rip+0x82d1a]        # 0x1800a1698
   18001e97e:	eb 07                	jmp    0x18001e987
   18001e980:	48 8d 05 41 2d 08 00 	lea    rax,[rip+0x82d41]        # 0x1800a16c8
   18001e987:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   18001e98c:	4c 8d 0d 15 2f 08 00 	lea    r9,[rip+0x82f15]        # 0x1800a18a8
   18001e993:	41 8b 84 3e b4 0c 18 	mov    eax,DWORD PTR [r14+rdi*1+0x180cb4]
   18001e99a:	00
   18001e99b:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001e9a2:	ba dc 00 00 00       	mov    edx,0xdc
   18001e9a7:	89 44 24 20          	mov    DWORD PTR [rsp+0x20],eax
   18001e9ab:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001e9b2:	e8 99 b0 04 00       	call   0x180069a50
   18001e9b7:	41 83 bc 3e b4 0c 18 	cmp    DWORD PTR [r14+rdi*1+0x180cb4],0x1e
   18001e9be:	00 1e
   18001e9c0:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001e9c7:	45 8b c7             	mov    r8d,r15d
   18001e9ca:	0f 28 cf             	movaps xmm1,xmm7
   18001e9cd:	45 0f 4c c4          	cmovl  r8d,r12d
   18001e9d1:	41 0f 28 c1          	movaps xmm0,xmm9
   18001e9d5:	e8 76 42 00 00       	call   0x180022c50
   18001e9da:	f7 84 1e ec 00 00 00 	test   DWORD PTR [rsi+rbx*1+0xec],0xffffffee
   18001e9e1:	ee ff ff ff
   18001e9e5:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001e9ea:	0f 85 4c 03 00 00    	jne    0x18001ed3c
   18001e9f0:	f2 41 0f 10 84 3e b8 	movsd  xmm0,QWORD PTR [r14+rdi*1+0x180cb8]
   18001e9f7:	0c 18 00
   18001e9fa:	66 41 0f 2f c3       	comisd xmm0,xmm11
   18001e9ff:	0f 86 69 01 00 00    	jbe    0x18001eb6e
   18001ea05:	41 8b 84 3e b4 0c 18 	mov    eax,DWORD PTR [r14+rdi*1+0x180cb4]
   18001ea0c:	00
   18001ea0d:	8b b4 1e 18 01 00 00 	mov    esi,DWORD PTR [rsi+rbx*1+0x118]
   18001ea14:	2b f0                	sub    esi,eax
   18001ea16:	85 c0                	test   eax,eax
   18001ea18:	7e 25                	jle    0x18001ea3f
   18001ea1a:	48 8d 15 8f ce 14 00 	lea    rdx,[rip+0x14ce8f]        # 0x18016b8b0
   18001ea21:	f2 44 0f 10 94 1a 00 	movsd  xmm10,QWORD PTR [rdx+rbx*1+0x100]
   18001ea28:	01 00 00
   18001ea2b:	f2 44 0f 5c d0       	subsd  xmm10,xmm0
   18001ea30:	66 0f 6e c0          	movd   xmm0,eax
   18001ea34:	f3 0f e6 c0          	cvtdq2pd xmm0,xmm0
   18001ea38:	f2 44 0f 5e d0       	divsd  xmm10,xmm0
   18001ea3d:	eb 04                	jmp    0x18001ea43
   18001ea3f:	45 0f 28 d3          	movaps xmm10,xmm11
   18001ea43:	f2 41 0f 10 94 3e d0 	movsd  xmm2,QWORD PTR [r14+rdi*1+0x180cd0]
   18001ea4a:	0c 18 00
   18001ea4d:	48 8d 8d 58 03 00 00 	lea    rcx,[rbp+0x358]
   18001ea54:	ba 28 00 00 00       	mov    edx,0x28
   18001ea59:	e8 b2 0f 01 00       	call   0x18002fa10
   18001ea5e:	f2 41 0f 10 94 3e d8 	movsd  xmm2,QWORD PTR [r14+rdi*1+0x180cd8]
   18001ea65:	0c 18 00
   18001ea68:	48 8d 8d 30 03 00 00 	lea    rcx,[rbp+0x330]
   18001ea6f:	ba 28 00 00 00       	mov    edx,0x28
   18001ea74:	e8 97 0f 01 00       	call   0x18002fa10
   18001ea79:	41 0f 28 d2          	movaps xmm2,xmm10
   18001ea7d:	48 8d 8d 80 03 00 00 	lea    rcx,[rbp+0x380]
   18001ea84:	ba 28 00 00 00       	mov    edx,0x28
   18001ea89:	e8 82 0f 01 00       	call   0x18002fa10
   18001ea8e:	33 c0                	xor    eax,eax
   18001ea90:	4c 8d 0d 41 2e 08 00 	lea    r9,[rip+0x82e41]        # 0x1800a18d8
   18001ea97:	85 f6                	test   esi,esi
   18001ea99:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001eaa0:	ba dc 00 00 00       	mov    edx,0xdc
   18001eaa5:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001eaac:	0f 4f c6             	cmovg  eax,esi
   18001eaaf:	89 44 24 30          	mov    DWORD PTR [rsp+0x30],eax
   18001eab3:	48 8d 85 30 03 00 00 	lea    rax,[rbp+0x330]
   18001eaba:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   18001eabf:	48 8d 85 58 03 00 00 	lea    rax,[rbp+0x358]
   18001eac6:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18001eacb:	e8 80 af 04 00       	call   0x180069a50
   18001ead0:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001ead7:	45 8b c7             	mov    r8d,r15d
   18001eada:	0f 28 cf             	movaps xmm1,xmm7
   18001eadd:	41 0f 28 c1          	movaps xmm0,xmm9
   18001eae1:	e8 6a 41 00 00       	call   0x180022c50
   18001eae6:	f2 41 0f 10 84 3e d8 	movsd  xmm0,QWORD PTR [r14+rdi*1+0x180cd8]
   18001eaed:	0c 18 00
   18001eaf0:	48 8d 0d 19 2e 08 00 	lea    rcx,[rip+0x82e19]        # 0x1800a1910
   18001eaf7:	66 41 0f 2f c2       	comisd xmm0,xmm10
   18001eafc:	48 8d 05 15 2e 08 00 	lea    rax,[rip+0x82e15]        # 0x1800a1918
   18001eb03:	ba dc 00 00 00       	mov    edx,0xdc
   18001eb08:	4c 8d 0d 11 2e 08 00 	lea    r9,[rip+0x82e11]        # 0x1800a1920
   18001eb0f:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001eb16:	48 0f 43 c1          	cmovae rax,rcx
   18001eb1a:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001eb21:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   18001eb26:	48 8d 85 80 03 00 00 	lea    rax,[rbp+0x380]
   18001eb2d:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18001eb32:	e8 19 af 04 00       	call   0x180069a50
   18001eb37:	f2 41 0f 10 84 3e d8 	movsd  xmm0,QWORD PTR [r14+rdi*1+0x180cd8]
   18001eb3e:	0c 18 00
   18001eb41:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001eb48:	66 41 0f 2f c2       	comisd xmm0,xmm10
   18001eb4d:	45 8b c4             	mov    r8d,r12d
   18001eb50:	41 0f 28 c8          	movaps xmm1,xmm8
   18001eb54:	41 0f 28 c1          	movaps xmm0,xmm9
   18001eb58:	f3 0f 58 cf          	addss  xmm1,xmm7
   18001eb5c:	45 0f 43 c7          	cmovae r8d,r15d
   18001eb60:	e8 eb 40 00 00       	call   0x180022c50
   18001eb65:	48 8d 35 44 cd 14 00 	lea    rsi,[rip+0x14cd44]        # 0x18016b8b0
   18001eb6c:	eb 04                	jmp    0x18001eb72
   18001eb6e:	41 0f 28 f2          	movaps xmm6,xmm10
   18001eb72:	66 45 0f 2f 9c 3e b8 	comisd xmm11,QWORD PTR [r14+rdi*1+0x180cb8]
   18001eb79:	0c 18 00
   18001eb7c:	f3 41 0f 58 f0       	addss  xmm6,xmm8
   18001eb81:	f3 41 0f 58 f0       	addss  xmm6,xmm8
   18001eb86:	f3 41 0f 58 f0       	addss  xmm6,xmm8
   18001eb8b:	0f 82 18 01 00 00    	jb     0x18001eca9
   18001eb91:	8b 84 1e 18 01 00 00 	mov    eax,DWORD PTR [rsi+rbx*1+0x118]
   18001eb98:	41 2b 84 3e b4 0c 18 	sub    eax,DWORD PTR [r14+rdi*1+0x180cb4]
   18001eb9f:	00
   18001eba0:	83 f8 03             	cmp    eax,0x3
   18001eba3:	0f 8e 00 01 00 00    	jle    0x18001eca9
   18001eba9:	83 3d e8 95 16 00 00 	cmp    DWORD PTR [rip+0x1695e8],0x0        # 0x180188198
   18001ebb0:	75 3f                	jne    0x18001ebf1
   18001ebb2:	4c 8d 0d a7 2d 08 00 	lea    r9,[rip+0x82da7]        # 0x1800a1960
   18001ebb9:	41 b8 80 80 ff ff    	mov    r8d,0xffff8080
   18001ebbf:	0f 28 ce             	movaps xmm1,xmm6
   18001ebc2:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ebc6:	e8 85 40 00 00       	call   0x180022c50
   18001ebcb:	41 0f 28 f8          	movaps xmm7,xmm8
   18001ebcf:	4c 8d 0d da 2d 08 00 	lea    r9,[rip+0x82dda]        # 0x1800a19b0
   18001ebd6:	f3 0f 58 fe          	addss  xmm7,xmm6
   18001ebda:	41 b8 80 80 ff ff    	mov    r8d,0xffff8080
   18001ebe0:	0f 28 cf             	movaps xmm1,xmm7
   18001ebe3:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ebe7:	e8 64 40 00 00       	call   0x180022c50
   18001ebec:	e9 46 01 00 00       	jmp    0x18001ed37
   18001ebf1:	8b 84 1e ec 00 00 00 	mov    eax,DWORD PTR [rsi+rbx*1+0xec]
   18001ebf8:	83 e8 10             	sub    eax,0x10
   18001ebfb:	83 f8 01             	cmp    eax,0x1
   18001ebfe:	77 5e                	ja     0x18001ec5e
   18001ec00:	83 bc 1e a4 01 00 00 	cmp    DWORD PTR [rsi+rbx*1+0x1a4],0xfffffffe
   18001ec07:	fe
   18001ec08:	75 54                	jne    0x18001ec5e
   18001ec0a:	48 8d 86 88 01 00 00 	lea    rax,[rsi+0x188]
   18001ec11:	ba dc 00 00 00       	mov    edx,0xdc
   18001ec16:	48 03 c3             	add    rax,rbx
   18001ec19:	4c 8d 0d e0 2d 08 00 	lea    r9,[rip+0x82de0]        # 0x1800a1a00
   18001ec20:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001ec27:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18001ec2c:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001ec33:	e8 18 ae 04 00       	call   0x180069a50
   18001ec38:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001ec3f:	41 b8 80 80 ff ff    	mov    r8d,0xffff8080
   18001ec45:	0f 28 ce             	movaps xmm1,xmm6
   18001ec48:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ec4c:	e8 ff 3f 00 00       	call   0x180022c50
   18001ec51:	41 0f 28 f8          	movaps xmm7,xmm8
   18001ec55:	f3 0f 58 fe          	addss  xmm7,xmm6
   18001ec59:	e9 de 00 00 00       	jmp    0x18001ed3c
   18001ec5e:	4c 8d 0d eb 2d 08 00 	lea    r9,[rip+0x82deb]        # 0x1800a1a50
   18001ec65:	41 b8 80 80 ff ff    	mov    r8d,0xffff8080
   18001ec6b:	0f 28 ce             	movaps xmm1,xmm6
   18001ec6e:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ec72:	e8 d9 3f 00 00       	call   0x180022c50
   18001ec77:	41 0f 28 f8          	movaps xmm7,xmm8
   18001ec7b:	4c 8d 0d 1e 2e 08 00 	lea    r9,[rip+0x82e1e]        # 0x1800a1aa0
   18001ec82:	f3 0f 58 fe          	addss  xmm7,xmm6
   18001ec86:	41 b8 80 80 ff ff    	mov    r8d,0xffff8080
   18001ec8c:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ec90:	0f 28 cf             	movaps xmm1,xmm7
   18001ec93:	e8 b8 3f 00 00       	call   0x180022c50
   18001ec98:	4c 8d 0d 51 2e 08 00 	lea    r9,[rip+0x82e51]        # 0x1800a1af0
   18001ec9f:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001eca4:	e9 31 ff ff ff       	jmp    0x18001ebda
   18001eca9:	0f 28 fe             	movaps xmm7,xmm6
   18001ecac:	e9 8b 00 00 00       	jmp    0x18001ed3c
   18001ecb1:	48 8d 15 f8 cb 14 00 	lea    rdx,[rip+0x14cbf8]        # 0x18016b8b0
   18001ecb8:	41 b8 f0 00 00 00    	mov    r8d,0xf0
   18001ecbe:	f2 0f 10 84 1a b8 01 	movsd  xmm0,QWORD PTR [rdx+rbx*1+0x1b8]
   18001ecc5:	00 00
   18001ecc7:	48 8d 0c 13          	lea    rcx,[rbx+rdx*1]
   18001eccb:	f2 0f 58 84 1a b0 01 	addsd  xmm0,QWORD PTR [rdx+rbx*1+0x1b0]
   18001ecd2:	00 00
   18001ecd4:	83 bc 1a e8 00 00 00 	cmp    DWORD PTR [rdx+rbx*1+0xe8],0x1
   18001ecdb:	01
   18001ecdc:	4c 8d 0d 55 2e 08 00 	lea    r9,[rip+0x82e55]        # 0x1800a1b38
   18001ece3:	b8 f8 00 00 00       	mov    eax,0xf8
   18001ece8:	ba dc 00 00 00       	mov    edx,0xdc
   18001eced:	41 0f 45 c0          	cmovne eax,r8d
   18001ecf1:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   18001ecf8:	f2 0f 11 44 24 30    	movsd  QWORD PTR [rsp+0x30],xmm0
   18001ecfe:	48 89 74 24 28       	mov    QWORD PTR [rsp+0x28],rsi
   18001ed03:	f2 0f 10 04 08       	movsd  xmm0,QWORD PTR [rax+rcx*1]
   18001ed08:	48 8d 8d 00 02 00 00 	lea    rcx,[rbp+0x200]
   18001ed0f:	f2 0f 11 44 24 20    	movsd  QWORD PTR [rsp+0x20],xmm0
   18001ed15:	e8 36 ad 04 00       	call   0x180069a50
   18001ed1a:	4c 8d 8d 00 02 00 00 	lea    r9,[rbp+0x200]
   18001ed21:	45 8b c7             	mov    r8d,r15d
   18001ed24:	0f 28 cf             	movaps xmm1,xmm7
   18001ed27:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ed2b:	e8 20 3f 00 00       	call   0x180022c50
   18001ed30:	4c 8d 35 c9 12 fe ff 	lea    r14,[rip+0xfffffffffffe12c9]        # 0x180000000
   18001ed37:	f3 41 0f 58 f8       	addss  xmm7,xmm8
   18001ed3c:	41 ff c5             	inc    r13d
   18001ed3f:	f3 41 0f 58 fd       	addss  xmm7,xmm13
   18001ed44:	4c 8d 15 65 cb 14 00 	lea    r10,[rip+0x14cb65]        # 0x18016b8b0
   18001ed4b:	4c 8d 1d dd f0 06 00 	lea    r11,[rip+0x6f0dd]        # 0x18008de2f
   18001ed52:	48 8d 15 af f5 06 00 	lea    rdx,[rip+0x6f5af]        # 0x18008e308
   18001ed59:	44 3b 6c 24 54       	cmp    r13d,DWORD PTR [rsp+0x54]
   18001ed5e:	0f 8c ee f7 ff ff    	jl     0x18001e552
   18001ed64:	f3 44 0f 58 f7       	addss  xmm14,xmm7
   18001ed69:	45 8b c7             	mov    r8d,r15d
   18001ed6c:	4c 8d 0d ed 2d 08 00 	lea    r9,[rip+0x82ded]        # 0x1800a1b60
   18001ed73:	41 0f 28 ce          	movaps xmm1,xmm14
   18001ed77:	41 0f 28 c1          	movaps xmm0,xmm9
   18001ed7b:	e8 d0 3e 00 00       	call   0x180022c50
   18001ed80:	4c 8b bc 24 50 05 00 	mov    r15,QWORD PTR [rsp+0x550]
   18001ed87:	00
   18001ed88:	4c 8b a4 24 68 05 00 	mov    r12,QWORD PTR [rsp+0x568]
   18001ed8f:	00
   18001ed90:	44 0f 28 a4 24 e0 04 	movaps xmm12,XMMWORD PTR [rsp+0x4e0]
   18001ed97:	00 00
   18001ed99:	44 0f 28 9c 24 f0 04 	movaps xmm11,XMMWORD PTR [rsp+0x4f0]
   18001eda0:	00 00
   18001eda2:	44 0f 28 94 24 00 05 	movaps xmm10,XMMWORD PTR [rsp+0x500]
   18001eda9:	00 00
   18001edab:	44 0f 28 8c 24 10 05 	movaps xmm9,XMMWORD PTR [rsp+0x510]
   18001edb2:	00 00
   18001edb4:	44 0f 28 84 24 20 05 	movaps xmm8,XMMWORD PTR [rsp+0x520]
   18001edbb:	00 00
   18001edbd:	0f 28 bc 24 30 05 00 	movaps xmm7,XMMWORD PTR [rsp+0x530]
   18001edc4:	00
   18001edc5:	0f 28 b4 24 40 05 00 	movaps xmm6,XMMWORD PTR [rsp+0x540]
   18001edcc:	00
   18001edcd:	44 0f 28 ac 24 d0 04 	movaps xmm13,XMMWORD PTR [rsp+0x4d0]
   18001edd4:	00 00
   18001edd6:	4c 8b b4 24 58 05 00 	mov    r14,QWORD PTR [rsp+0x558]
   18001eddd:	00
   18001edde:	4c 8b ac 24 60 05 00 	mov    r13,QWORD PTR [rsp+0x560]
   18001ede5:	00
   18001ede6:	44 0f 28 b4 24 c0 04 	movaps xmm14,XMMWORD PTR [rsp+0x4c0]
   18001eded:	00 00
   18001edef:	48 8b bc 24 90 05 00 	mov    rdi,QWORD PTR [rsp+0x590]
   18001edf6:	00
   18001edf7:	48 8b b4 24 88 05 00 	mov    rsi,QWORD PTR [rsp+0x588]
   18001edfe:	00
   18001edff:	48 8b 9c 24 80 05 00 	mov    rbx,QWORD PTR [rsp+0x580]
   18001ee06:	00
   18001ee07:	44 0f 28 bc 24 b0 04 	movaps xmm15,XMMWORD PTR [rsp+0x4b0]
   18001ee0e:	00 00
   18001ee10:	48 8b 8d a8 03 00 00 	mov    rcx,QWORD PTR [rbp+0x3a8]
   18001ee17:	48 33 cc             	xor    rcx,rsp
   18001ee1a:	e8 71 b0 04 00       	call   0x180069e90
   18001ee1f:	48 81 c4 70 05 00 00 	add    rsp,0x570
   18001ee26:	5d                   	pop    rbp
   18001ee27:	c3                   	ret
