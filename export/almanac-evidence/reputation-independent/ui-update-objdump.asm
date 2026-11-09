
scratch/reputation-static/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180057230 <.text+0x56230>:
   180057230:	4c 8b dc             	mov    r11,rsp
   180057233:	53                   	push   rbx
   180057234:	48 81 ec 20 02 00 00 	sub    rsp,0x220
   18005723b:	48 8b 05 3e cf 06 00 	mov    rax,QWORD PTR [rip+0x6cf3e]        # 0x1800c4180
   180057242:	48 33 c4             	xor    rax,rsp
   180057245:	48 89 84 24 00 02 00 	mov    QWORD PTR [rsp+0x200],rax
   18005724c:	00
   18005724d:	83 3d 2c 9b 22 00 00 	cmp    DWORD PTR [rip+0x229b2c],0x0        # 0x180280d80
   180057254:	0f 84 b7 06 00 00    	je     0x180057911
   18005725a:	83 3d 23 9b 22 00 00 	cmp    DWORD PTR [rip+0x229b23],0x0        # 0x180280d84
   180057261:	0f 84 aa 06 00 00    	je     0x180057911
   180057267:	48 8b 05 02 db 06 00 	mov    rax,QWORD PTR [rip+0x6db02]        # 0x1800c4d70
   18005726e:	ba 08 00 00 00       	mov    edx,0x8
   180057273:	49 89 7b 18          	mov    QWORD PTR [r11+0x18],rdi
   180057277:	48 8b 3d 02 db 06 00 	mov    rdi,QWORD PTR [rip+0x6db02]        # 0x1800c4d80
   18005727e:	49 89 6b 08          	mov    QWORD PTR [r11+0x8],rbp
   180057282:	49 89 73 10          	mov    QWORD PTR [r11+0x10],rsi
   180057286:	48 8d 8f 60 15 9e 00 	lea    rcx,[rdi+0x9e1560]
   18005728d:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057290:	33 db                	xor    ebx,ebx
   180057292:	85 c0                	test   eax,eax
   180057294:	0f 84 d7 01 00 00    	je     0x180057471
   18005729a:	48 8b 87 60 15 9e 00 	mov    rax,QWORD PTR [rdi+0x9e1560]
   1800572a1:	48 3b 05 d0 76 48 00 	cmp    rax,QWORD PTR [rip+0x4876d0]        # 0x1804de978
   1800572a8:	0f 84 c3 01 00 00    	je     0x180057471
   1800572ae:	48 89 05 c3 76 48 00 	mov    QWORD PTR [rip+0x4876c3],rax        # 0x1804de978
   1800572b5:	48 85 c0             	test   rax,rax
   1800572b8:	0f 84 d2 01 00 00    	je     0x180057490
   1800572be:	48 8b 35 bb da 06 00 	mov    rsi,QWORD PTR [rip+0x6dabb]        # 0x1800c4d80
   1800572c5:	48 81 c6 60 15 9e 00 	add    rsi,0x9e1560
   1800572cc:	39 1d a6 75 48 00    	cmp    DWORD PTR [rip+0x4875a6],ebx        # 0x1804de878
   1800572d2:	75 20                	jne    0x1800572f4
   1800572d4:	48 8b 05 95 da 06 00 	mov    rax,QWORD PTR [rip+0x6da95]        # 0x1800c4d70
   1800572db:	ba 08 00 00 00       	mov    edx,0x8
   1800572e0:	48 8b ce             	mov    rcx,rsi
   1800572e3:	ff 50 60             	call   QWORD PTR [rax+0x60]
   1800572e6:	85 c0                	test   eax,eax
   1800572e8:	74 69                	je     0x180057353
   1800572ea:	c7 05 84 75 48 00 01 	mov    DWORD PTR [rip+0x487584],0x1        # 0x1804de878
   1800572f1:	00 00 00
   1800572f4:	48 85 f6             	test   rsi,rsi
   1800572f7:	74 5a                	je     0x180057353
   1800572f9:	48 8b 3e             	mov    rdi,QWORD PTR [rsi]
   1800572fc:	48 85 ff             	test   rdi,rdi
   1800572ff:	74 52                	je     0x180057353
   180057301:	48 3b 3d 78 75 48 00 	cmp    rdi,QWORD PTR [rip+0x487578]        # 0x1804de880
   180057308:	75 0e                	jne    0x180057318
   18005730a:	8b 05 c8 be 06 00    	mov    eax,DWORD PTR [rip+0x6bec8]        # 0x1800c31d8
   180057310:	39 05 72 75 48 00    	cmp    DWORD PTR [rip+0x487572],eax        # 0x1804de888
   180057316:	74 33                	je     0x18005734b
   180057318:	48 8b 05 51 da 06 00 	mov    rax,QWORD PTR [rip+0x6da51]        # 0x1800c4d70
   18005731f:	48 8d 4f 01          	lea    rcx,[rdi+0x1]
   180057323:	ba 17 00 00 00       	mov    edx,0x17
   180057328:	ff 50 60             	call   QWORD PTR [rax+0x60]
   18005732b:	85 c0                	test   eax,eax
   18005732d:	75 09                	jne    0x180057338
   18005732f:	48 8d 2d 8a 78 04 00 	lea    rbp,[rip+0x4788a]        # 0x18009ebc0
   180057336:	eb 22                	jmp    0x18005735a
   180057338:	8b 05 9a be 06 00    	mov    eax,DWORD PTR [rip+0x6be9a]        # 0x1800c31d8
   18005733e:	89 05 44 75 48 00    	mov    DWORD PTR [rip+0x487544],eax        # 0x1804de888
   180057344:	48 89 3d 35 75 48 00 	mov    QWORD PTR [rip+0x487535],rdi        # 0x1804de880
   18005734b:	48 8b 2e             	mov    rbp,QWORD PTR [rsi]
   18005734e:	48 ff c5             	inc    rbp
   180057351:	eb 07                	jmp    0x18005735a
   180057353:	48 8d 2d 5a 78 04 00 	lea    rbp,[rip+0x4785a]        # 0x18009ebb4
   18005735a:	8b 05 08 c9 06 00    	mov    eax,DWORD PTR [rip+0x6c908]        # 0x1800c3c68
   180057360:	85 c0                	test   eax,eax
   180057362:	0f 84 ff 00 00 00    	je     0x180057467
   180057368:	4c 89 a4 24 48 02 00 	mov    QWORD PTR [rsp+0x248],r12
   18005736f:	00
   180057370:	4c 8d 0d 01 78 04 00 	lea    r9,[rip+0x47801]        # 0x18009eb78
   180057377:	44 8b 25 ee c8 06 00 	mov    r12d,DWORD PTR [rip+0x6c8ee]        # 0x1800c3c6c
   18005737e:	4c 8d 44 24 40       	lea    r8,[rsp+0x40]
   180057383:	4c 89 bc 24 10 02 00 	mov    QWORD PTR [rsp+0x210],r15
   18005738a:	00
   18005738b:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   180057390:	44 8b f8             	mov    r15d,eax
   180057393:	89 5c 24 40          	mov    DWORD PTR [rsp+0x40],ebx
   180057397:	4c 03 3d e2 d9 06 00 	add    r15,QWORD PTR [rip+0x6d9e2]        # 0x1800c4d80
   18005739e:	41 8b f4             	mov    esi,r12d
   1800573a1:	ba a4 01 00 00       	mov    edx,0x1a4
   1800573a6:	48 89 6c 24 20       	mov    QWORD PTR [rsp+0x20],rbp
   1800573ab:	f7 de                	neg    esi
   1800573ad:	e8 1e a9 fa ff       	call   0x180001cd0
   1800573b2:	41 3b f4             	cmp    esi,r12d
   1800573b5:	0f 8f 8b 00 00 00    	jg     0x180057446
   1800573bb:	4c 89 b4 24 18 02 00 	mov    QWORD PTR [rsp+0x218],r14
   1800573c2:	00
   1800573c3:	8d 3c b5 00 00 00 00 	lea    edi,[rsi*4+0x0]
   1800573ca:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
   1800573d0:	48 8b 05 99 d9 06 00 	mov    rax,QWORD PTR [rip+0x6d999]        # 0x1800c4d70
   1800573d7:	49 8d 8f 10 4f 9d 00 	lea    rcx,[r15+0x9d4f10]
   1800573de:	4c 63 f7             	movsxd r14,edi
   1800573e1:	ba 04 00 00 00       	mov    edx,0x4
   1800573e6:	49 03 ce             	add    rcx,r14
   1800573e9:	ff 50 60             	call   QWORD PTR [rax+0x60]
   1800573ec:	4c 8d 44 24 40       	lea    r8,[rsp+0x40]
   1800573f1:	ba a4 01 00 00       	mov    edx,0x1a4
   1800573f6:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   1800573fb:	85 c0                	test   eax,eax
   1800573fd:	75 12                	jne    0x180057411
   1800573ff:	4c 8d 0d 92 77 04 00 	lea    r9,[rip+0x47792]        # 0x18009eb98
   180057406:	89 7c 24 20          	mov    DWORD PTR [rsp+0x20],edi
   18005740a:	e8 c1 a8 fa ff       	call   0x180001cd0
   18005740f:	eb 23                	jmp    0x180057434
   180057411:	f3 43 0f 10 84 37 10 	movss  xmm0,DWORD PTR [r15+r14*1+0x9d4f10]
   180057418:	4f 9d 00
   18005741b:	4c 8d 0d 86 77 04 00 	lea    r9,[rip+0x47786]        # 0x18009eba8
   180057422:	0f 5a c0             	cvtps2pd xmm0,xmm0
   180057425:	f2 0f 11 44 24 28    	movsd  QWORD PTR [rsp+0x28],xmm0
   18005742b:	89 7c 24 20          	mov    DWORD PTR [rsp+0x20],edi
   18005742f:	e8 9c a8 fa ff       	call   0x180001cd0
   180057434:	ff c6                	inc    esi
   180057436:	83 c7 04             	add    edi,0x4
   180057439:	41 3b f4             	cmp    esi,r12d
   18005743c:	7e 92                	jle    0x1800573d0
   18005743e:	4c 8b b4 24 18 02 00 	mov    r14,QWORD PTR [rsp+0x218]
   180057445:	00
   180057446:	48 8d 54 24 50       	lea    rdx,[rsp+0x50]
   18005744b:	48 8d 0d ca 6a 03 00 	lea    rcx,[rip+0x36aca]        # 0x18008df1c
   180057452:	e8 99 ca ff ff       	call   0x180053ef0
   180057457:	4c 8b bc 24 10 02 00 	mov    r15,QWORD PTR [rsp+0x210]
   18005745e:	00
   18005745f:	4c 8b a4 24 48 02 00 	mov    r12,QWORD PTR [rsp+0x248]
   180057466:	00
   180057467:	48 8b cd             	mov    rcx,rbp
   18005746a:	e8 c1 04 00 00       	call   0x180057930
   18005746f:	eb 1f                	jmp    0x180057490
   180057471:	48 8b 05 f8 d8 06 00 	mov    rax,QWORD PTR [rip+0x6d8f8]        # 0x1800c4d70
   180057478:	48 8d 8f 60 15 9e 00 	lea    rcx,[rdi+0x9e1560]
   18005747f:	ba 08 00 00 00       	mov    edx,0x8
   180057484:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057487:	85 c0                	test   eax,eax
   180057489:	74 05                	je     0x180057490
   18005748b:	e8 e0 ee ff ff       	call   0x180056370
   180057490:	e8 2b 1e 00 00       	call   0x1800592c0
   180057495:	39 1d 09 a7 3b 00    	cmp    DWORD PTR [rip+0x3ba709],ebx        # 0x180411ba4
   18005749b:	0f 84 10 01 00 00    	je     0x1800575b1
   1800574a1:	39 1d 01 a7 3b 00    	cmp    DWORD PTR [rip+0x3ba701],ebx        # 0x180411ba8
   1800574a7:	0f 8e 04 01 00 00    	jle    0x1800575b1
   1800574ad:	39 1d b5 c7 06 00    	cmp    DWORD PTR [rip+0x6c7b5],ebx        # 0x1800c3c68
   1800574b3:	0f 84 f8 00 00 00    	je     0x1800575b1
   1800574b9:	e8 92 08 00 00       	call   0x180057d50
   1800574be:	85 c0                	test   eax,eax
   1800574c0:	0f 84 eb 00 00 00    	je     0x1800575b1
   1800574c6:	8b 05 a8 74 48 00    	mov    eax,DWORD PTR [rip+0x4874a8]        # 0x1804de974
   1800574cc:	ff c0                	inc    eax
   1800574ce:	89 05 a0 74 48 00    	mov    DWORD PTR [rip+0x4874a0],eax        # 0x1804de974
   1800574d4:	99                   	cdq
   1800574d5:	f7 3d 95 c7 06 00    	idiv   DWORD PTR [rip+0x6c795]        # 0x1800c3c70
   1800574db:	85 d2                	test   edx,edx
   1800574dd:	0f 85 ce 00 00 00    	jne    0x1800575b1
   1800574e3:	8b 3d 7f c7 06 00    	mov    edi,DWORD PTR [rip+0x6c77f]        # 0x1800c3c68
   1800574e9:	ba 18 00 00 00       	mov    edx,0x18
   1800574ee:	48 8b 35 8b d8 06 00 	mov    rsi,QWORD PTR [rip+0x6d88b]        # 0x1800c4d80
   1800574f5:	48 8b 05 74 d8 06 00 	mov    rax,QWORD PTR [rip+0x6d874]        # 0x1800c4d70
   1800574fc:	48 8d 2c 37          	lea    rbp,[rdi+rsi*1]
   180057500:	48 8d 8d 08 4f 9d 00 	lea    rcx,[rbp+0x9d4f08]
   180057507:	ff 50 60             	call   QWORD PTR [rax+0x60]
   18005750a:	85 c0                	test   eax,eax
   18005750c:	75 0b                	jne    0x180057519
   18005750e:	89 1d 94 a6 3b 00    	mov    DWORD PTR [rip+0x3ba694],ebx        # 0x180411ba8
   180057514:	e9 98 00 00 00       	jmp    0x1800575b1
   180057519:	8b 15 89 a6 3b 00    	mov    edx,DWORD PTR [rip+0x3ba689]        # 0x180411ba8
   18005751f:	48 8d 0d 9a 74 04 00 	lea    rcx,[rip+0x4749a]        # 0x18009e9c0
   180057526:	f3 0f 10 84 37 18 4f 	movss  xmm0,DWORD PTR [rdi+rsi*1+0x9d4f18]
   18005752d:	9d 00
   18005752f:	ff ca                	dec    edx
   180057531:	f3 0f 10 8c 37 14 4f 	movss  xmm1,DWORD PTR [rdi+rsi*1+0x9d4f14]
   180057538:	9d 00
   18005753a:	f3 0f 10 9c 37 0c 4f 	movss  xmm3,DWORD PTR [rdi+rsi*1+0x9d4f0c]
   180057541:	9d 00
   180057543:	f3 0f 10 95 08 4f 9d 	movss  xmm2,DWORD PTR [rbp+0x9d4f08]
   18005754a:	00
   18005754b:	f3 0f 10 a4 37 10 4f 	movss  xmm4,DWORD PTR [rdi+rsi*1+0x9d4f10]
   180057552:	9d 00
   180057554:	0f 5a c0             	cvtps2pd xmm0,xmm0
   180057557:	89 15 4b a6 3b 00    	mov    DWORD PTR [rip+0x3ba64b],edx        # 0x180411ba8
   18005755d:	0f 5a c9             	cvtps2pd xmm1,xmm1
   180057560:	f2 0f 11 44 24 30    	movsd  QWORD PTR [rsp+0x30],xmm0
   180057566:	0f 5a db             	cvtps2pd xmm3,xmm3
   180057569:	0f 5a d2             	cvtps2pd xmm2,xmm2
   18005756c:	0f 5a e4             	cvtps2pd xmm4,xmm4
   18005756f:	f2 0f 11 4c 24 28    	movsd  QWORD PTR [rsp+0x28],xmm1
   180057575:	66 49 0f 7e d9       	movq   r9,xmm3
   18005757a:	66 49 0f 7e d0       	movq   r8,xmm2
   18005757f:	f2 0f 11 64 24 20    	movsd  QWORD PTR [rsp+0x20],xmm4
   180057585:	e8 66 c9 ff ff       	call   0x180053ef0
   18005758a:	39 1d 18 a6 3b 00    	cmp    DWORD PTR [rip+0x3ba618],ebx        # 0x180411ba8
   180057590:	75 1f                	jne    0x1800575b1
   180057592:	48 8d 0d 67 74 04 00 	lea    rcx,[rip+0x47467]        # 0x18009ea00
   180057599:	e8 52 c9 ff ff       	call   0x180053ef0
   18005759e:	48 8b 0d 3b bc 06 00 	mov    rcx,QWORD PTR [rip+0x6bc3b]        # 0x1800c31e0
   1800575a5:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1800575a9:	74 06                	je     0x1800575b1
   1800575ab:	ff 15 b7 5a 03 00    	call   QWORD PTR [rip+0x35ab7]        # 0x18008d068
   1800575b1:	39 1d a1 bb 06 00    	cmp    DWORD PTR [rip+0x6bba1],ebx        # 0x1800c3158
   1800575b7:	48 8b b4 24 38 02 00 	mov    rsi,QWORD PTR [rsp+0x238]
   1800575be:	00
   1800575bf:	48 8b ac 24 30 02 00 	mov    rbp,QWORD PTR [rsp+0x230]
   1800575c6:	00
   1800575c7:	75 30                	jne    0x1800575f9
   1800575c9:	39 1d b1 ea 22 00    	cmp    DWORD PTR [rip+0x22eab1],ebx        # 0x180286080
   1800575cf:	75 28                	jne    0x1800575f9
   1800575d1:	39 1d 41 64 17 00    	cmp    DWORD PTR [rip+0x176441],ebx        # 0x1801cda18
   1800575d7:	75 20                	jne    0x1800575f9
   1800575d9:	39 1d 35 64 17 00    	cmp    DWORD PTR [rip+0x176435],ebx        # 0x1801cda14
   1800575df:	75 18                	jne    0x1800575f9
   1800575e1:	39 1d cd d8 12 00    	cmp    DWORD PTR [rip+0x12d8cd],ebx        # 0x180184eb4
   1800575e7:	75 10                	jne    0x1800575f9
   1800575e9:	39 1d 2d 64 17 00    	cmp    DWORD PTR [rip+0x17642d],ebx        # 0x1801cda1c
   1800575ef:	75 08                	jne    0x1800575f9
   1800575f1:	39 1d 59 d6 06 00    	cmp    DWORD PTR [rip+0x6d659],ebx        # 0x1800c4c50
   1800575f7:	74 25                	je     0x18005761e
   1800575f9:	44 8b c3             	mov    r8d,ebx
   1800575fc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
   180057600:	41 8b c8             	mov    ecx,r8d
   180057603:	e8 18 7b fd ff       	call   0x18002f120
   180057608:	85 c0                	test   eax,eax
   18005760a:	75 0b                	jne    0x180057617
   18005760c:	41 ff c0             	inc    r8d
   18005760f:	41 83 f8 08          	cmp    r8d,0x8
   180057613:	7c eb                	jl     0x180057600
   180057615:	eb 07                	jmp    0x18005761e
   180057617:	44 89 05 ae a5 3b 00 	mov    DWORD PTR [rip+0x3ba5ae],r8d        # 0x180411bcc
   18005761e:	e8 1d 08 00 00       	call   0x180057e40
   180057623:	e8 e8 0c 00 00       	call   0x180058310
   180057628:	3b 05 52 73 48 00    	cmp    eax,DWORD PTR [rip+0x487352]        # 0x1804de980
   18005762e:	74 67                	je     0x180057697
   180057630:	89 05 4a 73 48 00    	mov    DWORD PTR [rip+0x48734a],eax        # 0x1804de980
   180057636:	85 c0                	test   eax,eax
   180057638:	74 5d                	je     0x180057697
   18005763a:	39 1d 18 bb 06 00    	cmp    DWORD PTR [rip+0x6bb18],ebx        # 0x1800c3158
   180057640:	75 3b                	jne    0x18005767d
   180057642:	39 1d 38 ea 22 00    	cmp    DWORD PTR [rip+0x22ea38],ebx        # 0x180286080
   180057648:	75 33                	jne    0x18005767d
   18005764a:	39 1d c8 63 17 00    	cmp    DWORD PTR [rip+0x1763c8],ebx        # 0x1801cda18
   180057650:	75 2b                	jne    0x18005767d
   180057652:	39 1d bc 63 17 00    	cmp    DWORD PTR [rip+0x1763bc],ebx        # 0x1801cda14
   180057658:	75 23                	jne    0x18005767d
   18005765a:	39 1d 54 d8 12 00    	cmp    DWORD PTR [rip+0x12d854],ebx        # 0x180184eb4
   180057660:	75 1b                	jne    0x18005767d
   180057662:	39 1d b4 63 17 00    	cmp    DWORD PTR [rip+0x1763b4],ebx        # 0x1801cda1c
   180057668:	75 13                	jne    0x18005767d
   18005766a:	39 1d e0 d5 06 00    	cmp    DWORD PTR [rip+0x6d5e0],ebx        # 0x1800c4c50
   180057670:	75 0b                	jne    0x18005767d
   180057672:	8b 0d 54 a5 3b 00    	mov    ecx,DWORD PTR [rip+0x3ba554]        # 0x180411bcc
   180057678:	e8 63 7b fd ff       	call   0x18002f1e0
   18005767d:	4c 8d 05 a4 c5 06 00 	lea    r8,[rip+0x6c5a4]        # 0x1800c3c28
   180057684:	48 8d 15 b5 c5 06 00 	lea    rdx,[rip+0x6c5b5]        # 0x1800c3c40
   18005768b:	48 8d 0d 0e 7b 04 00 	lea    rcx,[rip+0x47b0e]        # 0x18009f1a0
   180057692:	e8 59 c8 ff ff       	call   0x180053ef0
   180057697:	e8 b4 06 00 00       	call   0x180057d50
   18005769c:	39 1d 4a bc 06 00    	cmp    DWORD PTR [rip+0x6bc4a],ebx        # 0x1800c32ec
   1800576a2:	8b f8                	mov    edi,eax
   1800576a4:	74 2a                	je     0x1800576d0
   1800576a6:	85 c0                	test   eax,eax
   1800576a8:	75 26                	jne    0x1800576d0
   1800576aa:	39 1d 18 a5 3b 00    	cmp    DWORD PTR [rip+0x3ba518],ebx        # 0x180411bc8
   1800576b0:	75 48                	jne    0x1800576fa
   1800576b2:	e8 c9 a5 fa ff       	call   0x180001c80
   1800576b7:	85 c0                	test   eax,eax
   1800576b9:	74 3f                	je     0x1800576fa
   1800576bb:	e8 50 0c 00 00       	call   0x180058310
   1800576c0:	85 c0                	test   eax,eax
   1800576c2:	75 36                	jne    0x1800576fa
   1800576c4:	e8 e7 1c 00 00       	call   0x1800593b0
   1800576c9:	e8 82 06 00 00       	call   0x180057d50
   1800576ce:	8b f8                	mov    edi,eax
   1800576d0:	85 ff                	test   edi,edi
   1800576d2:	74 26                	je     0x1800576fa
   1800576d4:	39 1d ee a4 3b 00    	cmp    DWORD PTR [rip+0x3ba4ee],ebx        # 0x180411bc8
   1800576da:	74 1e                	je     0x1800576fa
   1800576dc:	39 1d 92 c5 06 00    	cmp    DWORD PTR [rip+0x6c592],ebx        # 0x1800c3c74
   1800576e2:	75 16                	jne    0x1800576fa
   1800576e4:	e8 97 a5 fa ff       	call   0x180001c80
   1800576e9:	85 c0                	test   eax,eax
   1800576eb:	75 0d                	jne    0x1800576fa
   1800576ed:	e8 ce fa ff ff       	call   0x1800571c0
   1800576f2:	89 1d b0 c8 14 00    	mov    DWORD PTR [rip+0x14c8b0],ebx        # 0x1801a3fa8
   1800576f8:	8b fb                	mov    edi,ebx
   1800576fa:	3b 3d c8 a4 3b 00    	cmp    edi,DWORD PTR [rip+0x3ba4c8]        # 0x180411bc8
   180057700:	74 76                	je     0x180057778
   180057702:	89 3d c0 a4 3b 00    	mov    DWORD PTR [rip+0x3ba4c0],edi        # 0x180411bc8
   180057708:	85 ff                	test   edi,edi
   18005770a:	0f 85 89 00 00 00    	jne    0x180057799
   180057710:	39 1d 5e c5 06 00    	cmp    DWORD PTR [rip+0x6c55e],ebx        # 0x1800c3c74
   180057716:	75 60                	jne    0x180057778
   180057718:	e8 63 a5 fa ff       	call   0x180001c80
   18005771d:	85 c0                	test   eax,eax
   18005771f:	74 57                	je     0x180057778
   180057721:	e8 ea 0b 00 00       	call   0x180058310
   180057726:	85 c0                	test   eax,eax
   180057728:	75 4e                	jne    0x180057778
   18005772a:	39 1d 50 e9 22 00    	cmp    DWORD PTR [rip+0x22e950],ebx        # 0x180286080
   180057730:	74 0c                	je     0x18005773e
   180057732:	89 1d 48 e9 22 00    	mov    DWORD PTR [rip+0x22e948],ebx        # 0x180286080
   180057738:	89 1d 46 e9 22 00    	mov    DWORD PTR [rip+0x22e946],ebx        # 0x180286084
   18005773e:	48 8d 0d a3 7a 04 00 	lea    rcx,[rip+0x47aa3]        # 0x18009f1e8
   180057745:	89 1d 05 d5 06 00    	mov    DWORD PTR [rip+0x6d505],ebx        # 0x1800c4c50
   18005774b:	89 1d cb 62 17 00    	mov    DWORD PTR [rip+0x1762cb],ebx        # 0x1801cda1c
   180057751:	89 1d 5d d7 12 00    	mov    DWORD PTR [rip+0x12d75d],ebx        # 0x180184eb4
   180057757:	89 1d b7 62 17 00    	mov    DWORD PTR [rip+0x1762b7],ebx        # 0x1801cda14
   18005775d:	89 1d b5 62 17 00    	mov    DWORD PTR [rip+0x1762b5],ebx        # 0x1801cda18
   180057763:	89 1d ef b9 06 00    	mov    DWORD PTR [rip+0x6b9ef],ebx        # 0x1800c3158
   180057769:	c7 05 fd b9 06 00 01 	mov    DWORD PTR [rip+0x6b9fd],0x1        # 0x1800c3170
   180057770:	00 00 00
   180057773:	e8 78 c7 ff ff       	call   0x180053ef0
   180057778:	48 8b bc 24 40 02 00 	mov    rdi,QWORD PTR [rsp+0x240]
   18005777f:	00
   180057780:	48 8b 8c 24 00 02 00 	mov    rcx,QWORD PTR [rsp+0x200]
   180057787:	00
   180057788:	48 33 cc             	xor    rcx,rsp
   18005778b:	e8 00 27 01 00       	call   0x180069e90
   180057790:	48 81 c4 20 02 00 00 	add    rsp,0x220
   180057797:	5b                   	pop    rbx
   180057798:	c3                   	ret
   180057799:	e8 e2 a4 fa ff       	call   0x180001c80
   18005779e:	39 1d d0 c4 06 00    	cmp    DWORD PTR [rip+0x6c4d0],ebx        # 0x1800c3c74
   1800577a4:	75 76                	jne    0x18005781c
   1800577a6:	85 c0                	test   eax,eax
   1800577a8:	75 0b                	jne    0x1800577b5
   1800577aa:	8b 0d 1c a4 3b 00    	mov    ecx,DWORD PTR [rip+0x3ba41c]        # 0x180411bcc
   1800577b0:	e8 2b 7a fd ff       	call   0x18002f1e0
   1800577b5:	8b 0d 15 c8 14 00    	mov    ecx,DWORD PTR [rip+0x14c815]        # 0x1801a3fd0
   1800577bb:	8b c1                	mov    eax,ecx
   1800577bd:	2b 05 c1 71 48 00    	sub    eax,DWORD PTR [rip+0x4871c1]        # 0x1804de984
   1800577c3:	89 0d bb 71 48 00    	mov    DWORD PTR [rip+0x4871bb],ecx        # 0x1804de984
   1800577c9:	83 f8 1e             	cmp    eax,0x1e
   1800577cc:	0f 83 b6 00 00 00    	jae    0x180057888
   1800577d2:	8b 15 b0 71 48 00    	mov    edx,DWORD PTR [rip+0x4871b0]        # 0x1804de988
   1800577d8:	ff c2                	inc    edx
   1800577da:	89 15 a8 71 48 00    	mov    DWORD PTR [rip+0x4871a8],edx        # 0x1804de988
   1800577e0:	83 fa 08             	cmp    edx,0x8
   1800577e3:	0f 82 a5 00 00 00    	jb     0x18005788e
   1800577e9:	39 1d 85 c4 06 00    	cmp    DWORD PTR [rip+0x6c485],ebx        # 0x1800c3c74
   1800577ef:	0f 84 99 00 00 00    	je     0x18005788e
   1800577f5:	48 8d 0d 24 7a 04 00 	lea    rcx,[rip+0x47a24]        # 0x18009f220
   1800577fc:	89 1d 72 c4 06 00    	mov    DWORD PTR [rip+0x6c472],ebx        # 0x1800c3c74
   180057802:	e8 e9 c6 ff ff       	call   0x180053ef0
   180057807:	48 8b 0d d2 b9 06 00 	mov    rcx,QWORD PTR [rip+0x6b9d2]        # 0x1800c31e0
   18005780e:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180057812:	74 7a                	je     0x18005788e
   180057814:	ff 15 4e 58 03 00    	call   QWORD PTR [rip+0x3584e]        # 0x18008d068
   18005781a:	eb 72                	jmp    0x18005788e
   18005781c:	85 c0                	test   eax,eax
   18005781e:	74 8a                	je     0x1800577aa
   180057820:	44 8b c3             	mov    r8d,ebx
   180057823:	41 8b c8             	mov    ecx,r8d
   180057826:	e8 f5 78 fd ff       	call   0x18002f120
   18005782b:	85 c0                	test   eax,eax
   18005782d:	75 0b                	jne    0x18005783a
   18005782f:	41 ff c0             	inc    r8d
   180057832:	41 83 f8 08          	cmp    r8d,0x8
   180057836:	7c eb                	jl     0x180057823
   180057838:	eb 07                	jmp    0x180057841
   18005783a:	44 89 05 8b a3 3b 00 	mov    DWORD PTR [rip+0x3ba38b],r8d        # 0x180411bcc
   180057841:	39 1d 39 e8 22 00    	cmp    DWORD PTR [rip+0x22e839],ebx        # 0x180286080
   180057847:	74 0c                	je     0x180057855
   180057849:	89 1d 31 e8 22 00    	mov    DWORD PTR [rip+0x22e831],ebx        # 0x180286080
   18005784f:	89 1d 2f e8 22 00    	mov    DWORD PTR [rip+0x22e82f],ebx        # 0x180286084
   180057855:	89 1d f5 d3 06 00    	mov    DWORD PTR [rip+0x6d3f5],ebx        # 0x1800c4c50
   18005785b:	89 1d bb 61 17 00    	mov    DWORD PTR [rip+0x1761bb],ebx        # 0x1801cda1c
   180057861:	89 1d 4d d6 12 00    	mov    DWORD PTR [rip+0x12d64d],ebx        # 0x180184eb4
   180057867:	89 1d a7 61 17 00    	mov    DWORD PTR [rip+0x1761a7],ebx        # 0x1801cda14
   18005786d:	89 1d a5 61 17 00    	mov    DWORD PTR [rip+0x1761a5],ebx        # 0x1801cda18
   180057873:	89 1d df b8 06 00    	mov    DWORD PTR [rip+0x6b8df],ebx        # 0x1800c3158
   180057879:	c7 05 ed b8 06 00 01 	mov    DWORD PTR [rip+0x6b8ed],0x1        # 0x1800c3170
   180057880:	00 00 00
   180057883:	e9 2d ff ff ff       	jmp    0x1800577b5
   180057888:	89 1d fa 70 48 00    	mov    DWORD PTR [rip+0x4870fa],ebx        # 0x1804de988
   18005788e:	e8 7d f8 ff ff       	call   0x180057110
   180057893:	39 1d db c3 06 00    	cmp    DWORD PTR [rip+0x6c3db],ebx        # 0x1800c3c74
   180057899:	74 06                	je     0x1800578a1
   18005789b:	89 1d 27 a3 3b 00    	mov    DWORD PTR [rip+0x3ba327],ebx        # 0x180411bc8
   1800578a1:	39 1d b1 b8 06 00    	cmp    DWORD PTR [rip+0x6b8b1],ebx        # 0x1800c3158
   1800578a7:	75 30                	jne    0x1800578d9
   1800578a9:	39 1d d1 e7 22 00    	cmp    DWORD PTR [rip+0x22e7d1],ebx        # 0x180286080
   1800578af:	75 28                	jne    0x1800578d9
   1800578b1:	39 1d 61 61 17 00    	cmp    DWORD PTR [rip+0x176161],ebx        # 0x1801cda18
   1800578b7:	75 20                	jne    0x1800578d9
   1800578b9:	39 1d 55 61 17 00    	cmp    DWORD PTR [rip+0x176155],ebx        # 0x1801cda14
   1800578bf:	75 18                	jne    0x1800578d9
   1800578c1:	39 1d ed d5 12 00    	cmp    DWORD PTR [rip+0x12d5ed],ebx        # 0x180184eb4
   1800578c7:	75 10                	jne    0x1800578d9
   1800578c9:	39 1d 4d 61 17 00    	cmp    DWORD PTR [rip+0x17614d],ebx        # 0x1801cda1c
   1800578cf:	75 08                	jne    0x1800578d9
   1800578d1:	39 1d 79 d3 06 00    	cmp    DWORD PTR [rip+0x6d379],ebx        # 0x1800c4c50
   1800578d7:	74 05                	je     0x1800578de
   1800578d9:	bb 01 00 00 00       	mov    ebx,0x1
   1800578de:	44 8b 0d e7 a2 3b 00 	mov    r9d,DWORD PTR [rip+0x3ba2e7]        # 0x180411bcc
   1800578e5:	48 8d 05 d8 79 04 00 	lea    rax,[rip+0x479d8]        # 0x18009f2c4
   1800578ec:	85 db                	test   ebx,ebx
   1800578ee:	4c 8d 05 d7 79 04 00 	lea    r8,[rip+0x479d7]        # 0x18009f2cc
   1800578f5:	48 8d 15 14 c3 06 00 	lea    rdx,[rip+0x6c314]        # 0x1800c3c10
   1800578fc:	4c 0f 45 c0          	cmovne r8,rax
   180057900:	48 8d 0d d1 79 04 00 	lea    rcx,[rip+0x479d1]        # 0x18009f2d8
   180057907:	e8 e4 c5 ff ff       	call   0x180053ef0
   18005790c:	e9 67 fe ff ff       	jmp    0x180057778
   180057911:	33 db                	xor    ebx,ebx
   180057913:	89 1d af a2 3b 00    	mov    DWORD PTR [rip+0x3ba2af],ebx        # 0x180411bc8
   180057919:	89 1d 89 c6 14 00    	mov    DWORD PTR [rip+0x14c689],ebx        # 0x1801a3fa8
   18005791f:	e9 5c fe ff ff       	jmp    0x180057780
   180057924:	cc                   	int3
   180057925:	cc                   	int3
   180057926:	cc                   	int3
   180057927:	cc                   	int3
   180057928:	cc                   	int3
   180057929:	cc                   	int3
   18005792a:	cc                   	int3
   18005792b:	cc                   	int3
   18005792c:	cc                   	int3
   18005792d:	cc                   	int3
   18005792e:	cc                   	int3
   18005792f:	cc                   	int3
