
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

00000001800575b1 <.text+0x565b1>:
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
   1800578de:	44                   	rex.R
   1800578df:	8b                   	.byte 0x8b
