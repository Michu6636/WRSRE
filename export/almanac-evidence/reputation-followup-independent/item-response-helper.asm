GNU objdump independent static decode; SHA-256 fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a  Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

000000018002f1e0 <.text+0x2e1e0>:
   18002f1e0:	45 33 c9             	xor    r9d,r9d
   18002f1e3:	83 f9 07             	cmp    ecx,0x7
   18002f1e6:	0f 87 3b 01 00 00    	ja     0x18002f327
   18002f1ec:	4c 8d 05 0d 0e fd ff 	lea    r8,[rip+0xfffffffffffd0e0d]        # 0x180000000
   18002f1f3:	48 63 c1             	movsxd rax,ecx
   18002f1f6:	41 8b 94 80 f8 f3 02 	mov    edx,DWORD PTR [r8+rax*4+0x2f3f8]
   18002f1fd:	00
   18002f1fe:	49 03 d0             	add    rdx,r8
   18002f201:	ff e2                	jmp    rdx
   18002f203:	44 39 0d 4e 3f 09 00 	cmp    DWORD PTR [rip+0x93f4e],r9d        # 0x1800c3158
   18002f20a:	ba 02 00 00 00       	mov    edx,0x2
   18002f20f:	c7 05 57 3f 09 00 01 	mov    DWORD PTR [rip+0x93f57],0x1        # 0x1800c3170
   18002f216:	00 00 00
   18002f219:	41 0f 45 d1          	cmovne edx,r9d
   18002f21d:	89 15 35 3f 09 00    	mov    DWORD PTR [rip+0x93f35],edx        # 0x1800c3158
   18002f223:	e9 ff 00 00 00       	jmp    0x18002f327
   18002f228:	41 8b c1             	mov    eax,r9d
   18002f22b:	44 89 0d 52 6e 25 00 	mov    DWORD PTR [rip+0x256e52],r9d        # 0x180286084
   18002f232:	39 05 48 6e 25 00    	cmp    DWORD PTR [rip+0x256e48],eax        # 0x180286080
   18002f238:	0f 94 c0             	sete   al
   18002f23b:	89 05 3f 6e 25 00    	mov    DWORD PTR [rip+0x256e3f],eax        # 0x180286080
   18002f241:	e9 e1 00 00 00       	jmp    0x18002f327
   18002f246:	44 39 0d 5b 41 09 00 	cmp    DWORD PTR [rip+0x9415b],r9d        # 0x1800c33a8
   18002f24d:	0f 84 d4 00 00 00    	je     0x18002f327
   18002f253:	41 8b c1             	mov    eax,r9d
   18002f256:	c7 05 10 3f 09 00 01 	mov    DWORD PTR [rip+0x93f10],0x1        # 0x1800c3170
   18002f25d:	00 00 00
   18002f260:	39 05 b2 e7 19 00    	cmp    DWORD PTR [rip+0x19e7b2],eax        # 0x1801cda18
   18002f266:	0f 94 c0             	sete   al
   18002f269:	89 05 a9 e7 19 00    	mov    DWORD PTR [rip+0x19e7a9],eax        # 0x1801cda18
   18002f26f:	e9 b3 00 00 00       	jmp    0x18002f327
   18002f274:	44 39 0d 29 41 09 00 	cmp    DWORD PTR [rip+0x94129],r9d        # 0x1800c33a4
   18002f27b:	0f 84 a6 00 00 00    	je     0x18002f327
   18002f281:	41 8b c1             	mov    eax,r9d
   18002f284:	c7 05 e2 3e 09 00 01 	mov    DWORD PTR [rip+0x93ee2],0x1        # 0x1800c3170
   18002f28b:	00 00 00
   18002f28e:	39 05 80 e7 19 00    	cmp    DWORD PTR [rip+0x19e780],eax        # 0x1801cda14
   18002f294:	0f 94 c0             	sete   al
   18002f297:	89 05 77 e7 19 00    	mov    DWORD PTR [rip+0x19e777],eax        # 0x1801cda14
   18002f29d:	e9 85 00 00 00       	jmp    0x18002f327
   18002f2a2:	44 39 0d 07 97 09 00 	cmp    DWORD PTR [rip+0x99707],r9d        # 0x1800c89b0
   18002f2a9:	75 7c                	jne    0x18002f327
   18002f2ab:	83 3d 02 5c 15 00 01 	cmp    DWORD PTR [rip+0x155c02],0x1        # 0x180184eb4
   18002f2b2:	41 8b c1             	mov    eax,r9d
   18002f2b5:	44 89 0d f4 5b 15 00 	mov    DWORD PTR [rip+0x155bf4],r9d        # 0x180184eb0
   18002f2bc:	0f 95 c0             	setne  al
   18002f2bf:	89 05 ef 5b 15 00    	mov    DWORD PTR [rip+0x155bef],eax        # 0x180184eb4
   18002f2c5:	eb 60                	jmp    0x18002f327
   18002f2c7:	44 39 0d e2 96 09 00 	cmp    DWORD PTR [rip+0x996e2],r9d        # 0x1800c89b0
   18002f2ce:	75 57                	jne    0x18002f327
   18002f2d0:	ba 02 00 00 00       	mov    edx,0x2
   18002f2d5:	44 89 0d d4 5b 15 00 	mov    DWORD PTR [rip+0x155bd4],r9d        # 0x180184eb0
   18002f2dc:	39 15 d2 5b 15 00    	cmp    DWORD PTR [rip+0x155bd2],edx        # 0x180184eb4
   18002f2e2:	41 0f 44 d1          	cmove  edx,r9d
   18002f2e6:	89 15 c8 5b 15 00    	mov    DWORD PTR [rip+0x155bc8],edx        # 0x180184eb4
   18002f2ec:	eb 39                	jmp    0x18002f327
   18002f2ee:	44 39 0d b7 40 09 00 	cmp    DWORD PTR [rip+0x940b7],r9d        # 0x1800c33ac
   18002f2f5:	74 30                	je     0x18002f327
   18002f2f7:	41 8b c1             	mov    eax,r9d
   18002f2fa:	c7 05 6c 3e 09 00 01 	mov    DWORD PTR [rip+0x93e6c],0x1        # 0x1800c3170
   18002f301:	00 00 00
   18002f304:	39 05 12 e7 19 00    	cmp    DWORD PTR [rip+0x19e712],eax        # 0x1801cda1c
   18002f30a:	0f 94 c0             	sete   al
   18002f30d:	89 05 09 e7 19 00    	mov    DWORD PTR [rip+0x19e709],eax        # 0x1801cda1c
   18002f313:	eb 12                	jmp    0x18002f327
   18002f315:	41 8b c1             	mov    eax,r9d
   18002f318:	39 05 32 59 09 00    	cmp    DWORD PTR [rip+0x95932],eax        # 0x1800c4c50
   18002f31e:	0f 94 c0             	sete   al
   18002f321:	89 05 29 59 09 00    	mov    DWORD PTR [rip+0x95929],eax        # 0x1800c4c50
   18002f327:	44 39 0d 52 44 09 00 	cmp    DWORD PTR [rip+0x94452],r9d        # 0x1800c3780
   18002f32e:	74 64                	je     0x18002f394
   18002f330:	83 f9 01             	cmp    ecx,0x1
   18002f333:	7c 1c                	jl     0x18002f351
   18002f335:	44 39 0d 1c 3e 09 00 	cmp    DWORD PTR [rip+0x93e1c],r9d        # 0x1800c3158
   18002f33c:	74 5b                	je     0x18002f399
   18002f33e:	44 89 0d 13 3e 09 00 	mov    DWORD PTR [rip+0x93e13],r9d        # 0x1800c3158
   18002f345:	c7 05 21 3e 09 00 01 	mov    DWORD PTR [rip+0x93e21],0x1        # 0x1800c3170
   18002f34c:	00 00 00
   18002f34f:	eb 48                	jmp    0x18002f399
   18002f351:	85 c9                	test   ecx,ecx
   18002f353:	0f 85 9b 00 00 00    	jne    0x18002f3f4
   18002f359:	44 39 0d 20 6d 25 00 	cmp    DWORD PTR [rip+0x256d20],r9d        # 0x180286080
   18002f360:	74 0e                	je     0x18002f370
   18002f362:	44 89 0d 17 6d 25 00 	mov    DWORD PTR [rip+0x256d17],r9d        # 0x180286080
   18002f369:	44 89 0d 14 6d 25 00 	mov    DWORD PTR [rip+0x256d14],r9d        # 0x180286084
   18002f370:	44 89 0d a5 e6 19 00 	mov    DWORD PTR [rip+0x19e6a5],r9d        # 0x1801cda1c
   18002f377:	44 89 0d 36 5b 15 00 	mov    DWORD PTR [rip+0x155b36],r9d        # 0x180184eb4
   18002f37e:	44 89 0d 8f e6 19 00 	mov    DWORD PTR [rip+0x19e68f],r9d        # 0x1801cda14
   18002f385:	44 89 0d 8c e6 19 00 	mov    DWORD PTR [rip+0x19e68c],r9d        # 0x1801cda18
   18002f38c:	44 89 0d bd 58 09 00 	mov    DWORD PTR [rip+0x958bd],r9d        # 0x1800c4c50
   18002f393:	c3                   	ret
   18002f394:	83 f9 01             	cmp    ecx,0x1
   18002f397:	7c 5b                	jl     0x18002f3f4
   18002f399:	83 f9 01             	cmp    ecx,0x1
   18002f39c:	74 1c                	je     0x18002f3ba
   18002f39e:	44 39 0d db 6c 25 00 	cmp    DWORD PTR [rip+0x256cdb],r9d        # 0x180286080
   18002f3a5:	74 0e                	je     0x18002f3b5
   18002f3a7:	44 89 0d d2 6c 25 00 	mov    DWORD PTR [rip+0x256cd2],r9d        # 0x180286080
   18002f3ae:	44 89 0d cf 6c 25 00 	mov    DWORD PTR [rip+0x256ccf],r9d        # 0x180286084
   18002f3b5:	83 f9 02             	cmp    ecx,0x2
   18002f3b8:	74 0c                	je     0x18002f3c6
   18002f3ba:	44 89 0d 57 e6 19 00 	mov    DWORD PTR [rip+0x19e657],r9d        # 0x1801cda18
   18002f3c1:	83 f9 03             	cmp    ecx,0x3
   18002f3c4:	74 0f                	je     0x18002f3d5
   18002f3c6:	8d 41 fc             	lea    eax,[rcx-0x4]
   18002f3c9:	44 89 0d 44 e6 19 00 	mov    DWORD PTR [rip+0x19e644],r9d        # 0x1801cda14
   18002f3d0:	83 f8 01             	cmp    eax,0x1
   18002f3d3:	76 0c                	jbe    0x18002f3e1
   18002f3d5:	44 89 0d d8 5a 15 00 	mov    DWORD PTR [rip+0x155ad8],r9d        # 0x180184eb4
   18002f3dc:	83 f9 06             	cmp    ecx,0x6
   18002f3df:	74 0c                	je     0x18002f3ed
   18002f3e1:	44 89 0d 34 e6 19 00 	mov    DWORD PTR [rip+0x19e634],r9d        # 0x1801cda1c
   18002f3e8:	83 f9 07             	cmp    ecx,0x7
   18002f3eb:	74 07                	je     0x18002f3f4
   18002f3ed:	44 89 0d 5c 58 09 00 	mov    DWORD PTR [rip+0x9585c],r9d        # 0x1800c4c50
   18002f3f4:	c3                   	ret
