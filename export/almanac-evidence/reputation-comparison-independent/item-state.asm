
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
