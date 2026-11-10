GNU objdump independent static decode; DLL SHA-256 fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a  Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

00000001800303e0 <.text+0x2f3e0>:
   1800303e0:	40 57                	rex push rdi
   1800303e2:	41 56                	push   r14
   1800303e4:	48 81 ec 58 01 00 00 	sub    rsp,0x158
   1800303eb:	48 8b 05 8e 3d 09 00 	mov    rax,QWORD PTR [rip+0x93d8e]        # 0x1800c4180
   1800303f2:	48 33 c4             	xor    rax,rsp
   1800303f5:	48 89 84 24 40 01 00 	mov    QWORD PTR [rsp+0x140],rax
   1800303fc:	00
   1800303fd:	83 3d 8c 2c 09 00 00 	cmp    DWORD PTR [rip+0x92c8c],0x0        # 0x1800c3090
   180030404:	4c 8b f2             	mov    r14,rdx
   180030407:	48 8b f9             	mov    rdi,rcx
   18003040a:	0f 84 cc 02 00 00    	je     0x1800306dc
   180030410:	48 85 c9             	test   rcx,rcx
   180030413:	0f 84 c3 02 00 00    	je     0x1800306dc
   180030419:	48 85 d2             	test   rdx,rdx
   18003041c:	0f 84 ba 02 00 00    	je     0x1800306dc
   180030422:	83 3d ab 6e 0a 00 00 	cmp    DWORD PTR [rip+0xa6eab],0x0        # 0x1800d72d4
   180030429:	74 0d                	je     0x180030438
   18003042b:	48 8d 0d 76 49 09 00 	lea    rcx,[rip+0x94976]        # 0x1800c4da8
   180030432:	ff 15 80 cc 05 00    	call   QWORD PTR [rip+0x5cc80]        # 0x18008d0b8
   180030438:	80 3f 00             	cmp    BYTE PTR [rdi],0x0
   18003043b:	48 8b cf             	mov    rcx,rdi
   18003043e:	48 89 9c 24 80 01 00 	mov    QWORD PTR [rsp+0x180],rbx
   180030445:	00
   180030446:	48 89 b4 24 50 01 00 	mov    QWORD PTR [rsp+0x150],rsi
   18003044d:	00
   18003044e:	74 47                	je     0x180030497
   180030450:	0f b6 01             	movzx  eax,BYTE PTR [rcx]
   180030453:	3c 5c                	cmp    al,0x5c
   180030455:	74 04                	je     0x18003045b
   180030457:	3c 2f                	cmp    al,0x2f
   180030459:	75 34                	jne    0x18003048f
   18003045b:	0f b6 41 01          	movzx  eax,BYTE PTR [rcx+0x1]
   18003045f:	04 ad                	add    al,0xad
   180030461:	a8 df                	test   al,0xdf
   180030463:	75 2a                	jne    0x18003048f
   180030465:	0f b6 41 02          	movzx  eax,BYTE PTR [rcx+0x2]
   180030469:	04 bf                	add    al,0xbf
   18003046b:	a8 df                	test   al,0xdf
   18003046d:	75 20                	jne    0x18003048f
   18003046f:	0f b6 41 03          	movzx  eax,BYTE PTR [rcx+0x3]
   180030473:	04 aa                	add    al,0xaa
   180030475:	a8 df                	test   al,0xdf
   180030477:	75 16                	jne    0x18003048f
   180030479:	0f b6 41 04          	movzx  eax,BYTE PTR [rcx+0x4]
   18003047d:	04 bb                	add    al,0xbb
   18003047f:	a8 df                	test   al,0xdf
   180030481:	75 0c                	jne    0x18003048f
   180030483:	0f b6 41 05          	movzx  eax,BYTE PTR [rcx+0x5]
   180030487:	3c 5c                	cmp    al,0x5c
   180030489:	74 61                	je     0x1800304ec
   18003048b:	3c 2f                	cmp    al,0x2f
   18003048d:	74 5d                	je     0x1800304ec
   18003048f:	48 ff c1             	inc    rcx
   180030492:	80 39 00             	cmp    BYTE PTR [rcx],0x0
   180030495:	75 b9                	jne    0x180030450
   180030497:	33 f6                	xor    esi,esi
   180030499:	83 3d ec 2b 09 00 01 	cmp    DWORD PTR [rip+0x92bec],0x1        # 0x1800c308c
   1800304a0:	48 8b 9c 24 80 01 00 	mov    rbx,QWORD PTR [rsp+0x180]
   1800304a7:	00
   1800304a8:	7e 2b                	jle    0x1800304d5
   1800304aa:	48 85 f6             	test   rsi,rsi
   1800304ad:	75 14                	jne    0x1800304c3
   1800304af:	48 8d 15 02 02 06 00 	lea    rdx,[rip+0x60202]        # 0x1800906b8
   1800304b6:	48 8b cf             	mov    rcx,rdi
   1800304b9:	e8 12 a6 03 00       	call   0x18006aad0
   1800304be:	48 85 c0             	test   rax,rax
   1800304c1:	74 17                	je     0x1800304da
   1800304c3:	4c 8b c7             	mov    r8,rdi
   1800304c6:	48 8d 0d f3 01 06 00 	lea    rcx,[rip+0x601f3]        # 0x1800906c0
   1800304cd:	49 8b d6             	mov    rdx,r14
   1800304d0:	e8 1b 3a 02 00       	call   0x180053ef0
   1800304d5:	48 85 f6             	test   rsi,rsi
   1800304d8:	75 78                	jne    0x180030552
   1800304da:	83 3d f3 6d 0a 00 00 	cmp    DWORD PTR [rip+0xa6df3],0x0        # 0x1800d72d4
   1800304e1:	0f 85 e0 01 00 00    	jne    0x1800306c7
   1800304e7:	e9 e8 01 00 00       	jmp    0x1800306d4
   1800304ec:	48 8d 59 06          	lea    rbx,[rcx+0x6]
   1800304f0:	48 85 db             	test   rbx,rbx
   1800304f3:	74 a2                	je     0x180030497
   1800304f5:	80 3b 00             	cmp    BYTE PTR [rbx],0x0
   1800304f8:	74 9d                	je     0x180030497
   1800304fa:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
   180030500:	0f b6 03             	movzx  eax,BYTE PTR [rbx]
   180030503:	0f b6 c8             	movzx  ecx,al
   180030506:	3c 5c                	cmp    al,0x5c
   180030508:	74 0f                	je     0x180030519
   18003050a:	3c 2f                	cmp    al,0x2f
   18003050c:	74 0b                	je     0x180030519
   18003050e:	0f b6 4b 01          	movzx  ecx,BYTE PTR [rbx+0x1]
   180030512:	48 ff c3             	inc    rbx
   180030515:	84 c9                	test   cl,cl
   180030517:	75 e7                	jne    0x180030500
   180030519:	84 c9                	test   cl,cl
   18003051b:	0f 84 76 ff ff ff    	je     0x180030497
   180030521:	48 2b df             	sub    rbx,rdi
   180030524:	48 8d 43 ff          	lea    rax,[rbx-0x1]
   180030528:	48 3d 02 01 00 00    	cmp    rax,0x102
   18003052e:	0f 87 63 ff ff ff    	ja     0x180030497
   180030534:	48 8d 35 a5 23 46 00 	lea    rsi,[rip+0x4623a5]        # 0x1804928e0
   18003053b:	4c 8b c3             	mov    r8,rbx
   18003053e:	48 8b ce             	mov    rcx,rsi
   180030541:	48 8b d7             	mov    rdx,rdi
   180030544:	e8 97 6c 05 00       	call   0x1800871e0
   180030549:	c6 04 33 00          	mov    BYTE PTR [rbx+rsi*1],0x0
   18003054d:	e9 47 ff ff ff       	jmp    0x180030499
   180030552:	41 0f b6 06          	movzx  eax,BYTE PTR [r14]
   180030556:	3c 77                	cmp    al,0x77
   180030558:	74 66                	je     0x1800305c0
   18003055a:	3c 61                	cmp    al,0x61
   18003055c:	74 62                	je     0x1800305c0
   18003055e:	3c 72                	cmp    al,0x72
   180030560:	0f 85 58 01 00 00    	jne    0x1800306be
   180030566:	80 3f 00             	cmp    BYTE PTR [rdi],0x0
   180030569:	48 8b c7             	mov    rax,rdi
   18003056c:	74 1b                	je     0x180030589
   18003056e:	66 90                	xchg   ax,ax
   180030570:	0f b6 08             	movzx  ecx,BYTE PTR [rax]
   180030573:	80 f9 5c             	cmp    cl,0x5c
   180030576:	74 05                	je     0x18003057d
   180030578:	80 f9 2f             	cmp    cl,0x2f
   18003057b:	75 04                	jne    0x180030581
   18003057d:	48 8d 78 01          	lea    rdi,[rax+0x1]
   180030581:	48 ff c0             	inc    rax
   180030584:	80 38 00             	cmp    BYTE PTR [rax],0x0
   180030587:	75 e7                	jne    0x180030570
   180030589:	48 8d 15 28 02 06 00 	lea    rdx,[rip+0x60228]        # 0x1800907b8
   180030590:	48 8b cf             	mov    rcx,rdi
   180030593:	e8 58 d5 03 00       	call   0x18006daf0
   180030598:	85 c0                	test   eax,eax
   18003059a:	0f 85 1e 01 00 00    	jne    0x1800306be
   1800305a0:	49 c7 c1 ff ff ff ff 	mov    r9,0xffffffffffffffff
   1800305a7:	48 8d 0d e2 7a 15 00 	lea    rcx,[rip+0x157ae2]        # 0x180188090
   1800305ae:	4c 8b c6             	mov    r8,rsi
   1800305b1:	ba 04 01 00 00       	mov    edx,0x104
   1800305b6:	e8 35 d4 03 00       	call   0x18006d9f0
   1800305bb:	e9 fe 00 00 00       	jmp    0x1800306be
   1800305c0:	48 8b d6             	mov    rdx,rsi
   1800305c3:	48 8d 0d b6 79 15 00 	lea    rcx,[rip+0x1579b6]        # 0x180187f80
   1800305ca:	e8 21 d5 03 00       	call   0x18006daf0
   1800305cf:	85 c0                	test   eax,eax
   1800305d1:	74 4f                	je     0x180030622
   1800305d3:	80 3d b6 7a 15 00 00 	cmp    BYTE PTR [rip+0x157ab6],0x0        # 0x180188090
   1800305da:	75 46                	jne    0x180030622
   1800305dc:	4c 8d 0d 0d fa 05 00 	lea    r9,[rip+0x5fa0d]        # 0x18008fff0
   1800305e3:	48 89 74 24 20       	mov    QWORD PTR [rsp+0x20],rsi
   1800305e8:	ba 04 01 00 00       	mov    edx,0x104
   1800305ed:	48 8d 4c 24 30       	lea    rcx,[rsp+0x30]
   1800305f2:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   1800305f9:	e8 52 94 03 00       	call   0x180069a50
   1800305fe:	48 8d 4c 24 30       	lea    rcx,[rsp+0x30]
   180030603:	ff 15 67 ca 05 00    	call   QWORD PTR [rip+0x5ca67]        # 0x18008d070
   180030609:	83 f8 ff             	cmp    eax,0xffffffff
   18003060c:	74 14                	je     0x180030622
   18003060e:	4c 8b c6             	mov    r8,rsi
   180030611:	48 8d 15 b8 00 06 00 	lea    rdx,[rip+0x600b8]        # 0x1800906d0
   180030618:	b9 05 00 00 00       	mov    ecx,0x5
   18003061d:	e8 0e b4 fe ff       	call   0x18001ba30
   180030622:	49 c7 c1 ff ff ff ff 	mov    r9,0xffffffffffffffff
   180030629:	48 8d 0d 50 79 15 00 	lea    rcx,[rip+0x157950]        # 0x180187f80
   180030630:	4c 8b c6             	mov    r8,rsi
   180030633:	ba 04 01 00 00       	mov    edx,0x104
   180030638:	e8 b3 d3 03 00       	call   0x18006d9f0
   18003063d:	48 8b ce             	mov    rcx,rsi
   180030640:	e8 eb 4a 02 00       	call   0x180055130
   180030645:	80 3f 00             	cmp    BYTE PTR [rdi],0x0
   180030648:	48 8b c7             	mov    rax,rdi
   18003064b:	74 1c                	je     0x180030669
   18003064d:	0f 1f 00             	nop    DWORD PTR [rax]
   180030650:	0f b6 08             	movzx  ecx,BYTE PTR [rax]
   180030653:	80 f9 5c             	cmp    cl,0x5c
   180030656:	74 05                	je     0x18003065d
   180030658:	80 f9 2f             	cmp    cl,0x2f
   18003065b:	75 04                	jne    0x180030661
   18003065d:	48 8d 78 01          	lea    rdi,[rax+0x1]
   180030661:	48 ff c0             	inc    rax
   180030664:	80 38 00             	cmp    BYTE PTR [rax],0x0
   180030667:	75 e7                	jne    0x180030650
   180030669:	48 8d 15 20 01 06 00 	lea    rdx,[rip+0x60120]        # 0x180090790
   180030670:	48 8b cf             	mov    rcx,rdi
   180030673:	e8 78 d4 03 00       	call   0x18006daf0
   180030678:	85 c0                	test   eax,eax
   18003067a:	75 14                	jne    0x180030690
   18003067c:	e8 0f 41 03 00       	call   0x180064790
   180030681:	48 8b c8             	mov    rcx,rax
   180030684:	ba 01 00 00 00       	mov    edx,0x1
   180030689:	e8 42 78 fd ff       	call   0x180007ed0
   18003068e:	eb 2e                	jmp    0x1800306be
   180030690:	48 8d 15 11 01 06 00 	lea    rdx,[rip+0x60111]        # 0x1800907a8
   180030697:	48 8b cf             	mov    rcx,rdi
   18003069a:	e8 51 d4 03 00       	call   0x18006daf0
   18003069f:	85 c0                	test   eax,eax
   1800306a1:	75 1b                	jne    0x1800306be
   1800306a3:	39 05 e7 38 17 00    	cmp    DWORD PTR [rip+0x1738e7],eax        # 0x1801a3f90
   1800306a9:	74 13                	je     0x1800306be
   1800306ab:	89 05 df 38 17 00    	mov    DWORD PTR [rip+0x1738df],eax        # 0x1801a3f90
   1800306b1:	e8 da 40 03 00       	call   0x180064790
   1800306b6:	48 8b c8             	mov    rcx,rax
   1800306b9:	e8 12 79 fd ff       	call   0x180007fd0
   1800306be:	83 3d 0f 6c 0a 00 00 	cmp    DWORD PTR [rip+0xa6c0f],0x0        # 0x1800d72d4
   1800306c5:	74 0d                	je     0x1800306d4
   1800306c7:	48 8d 0d da 46 09 00 	lea    rcx,[rip+0x946da]        # 0x1800c4da8
   1800306ce:	ff 15 ec c9 05 00    	call   QWORD PTR [rip+0x5c9ec]        # 0x18008d0c0
   1800306d4:	48 8b b4 24 50 01 00 	mov    rsi,QWORD PTR [rsp+0x150]
   1800306db:	00
   1800306dc:	48 8b 8c 24 40 01 00 	mov    rcx,QWORD PTR [rsp+0x140]
   1800306e3:	00
   1800306e4:	48 33 cc             	xor    rcx,rsp
   1800306e7:	e8 a4 97 03 00       	call   0x180069e90
   1800306ec:	48 81 c4 58 01 00 00 	add    rsp,0x158
   1800306f3:	41 5e                	pop    r14
   1800306f5:	5f                   	pop    rdi
   1800306f6:	c3                   	ret
