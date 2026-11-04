; Range: 0x1406BC5A0 to 0x1406BCA00
1406bc5a0:  48 8B C4                 MOV RAX,RSP
1406bc5a3:  55                       PUSH RBP
1406bc5a4:  56                       PUSH RSI
1406bc5a5:  57                       PUSH RDI
1406bc5a6:  41 54                    PUSH R12
1406bc5a8:  41 55                    PUSH R13
1406bc5aa:  41 56                    PUSH R14
1406bc5ac:  41 57                    PUSH R15
1406bc5ae:  48 81 EC 80 00 00 00     SUB RSP,0x80
1406bc5b5:  48 C7 40 88 FE FF FF FF  MOV qword ptr [RAX + -0x78],-0x2
1406bc5bd:  48 89 58 20              MOV qword ptr [RAX + 0x20],RBX
1406bc5c1:  4C 8B F1                 MOV R14,RCX
1406bc5c4:  40 32 FF                 XOR DIL,DIL
1406bc5c7:  0F 57 C0                 XORPS XMM0,XMM0
1406bc5ca:  F3 0F 7F 40 90           MOVDQU xmmword ptr [RAX + -0x70],XMM0
1406bc5cf:  33 F6                    XOR ESI,ESI
1406bc5d1:  48 89 70 A0              MOV qword ptr [RAX + -0x60],RSI
1406bc5d5:  F3 0F 7F 40 A8           MOVDQU xmmword ptr [RAX + -0x58],XMM0
1406bc5da:  48 89 70 B8              MOV qword ptr [RAX + -0x48],RSI
1406bc5de:  48 63 89 00 07 00 00     MOVSXD RCX,dword ptr [RCX + 0x700]
1406bc5e5:  85 C9                    TEST ECX,ECX
1406bc5e7:  0F 88 AD 02 00 00        JS 0x1406bc89a
1406bc5ed:  49 8D 9E A0 06 00 00     LEA RBX,[R14 + 0x6a0]
1406bc5f4:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
1406bc5f8:  48 2B 03                 SUB RAX,qword ptr [RBX]
1406bc5fb:  48 C1 F8 03              SAR RAX,0x3
1406bc5ff:  48 3B C8                 CMP RCX,RAX
1406bc602:  0F 83 92 02 00 00        JNC 0x1406bc89a
1406bc608:  4C 8B 03                 MOV R8,qword ptr [RBX]
1406bc60b:  49 8B 14 C8              MOV RDX,qword ptr [R8 + RCX*0x8]
1406bc60f:  44 8B D6                 MOV R10D,ESI
1406bc612:  85 C9                    TEST ECX,ECX
1406bc614:  7E 0A                    JLE 0x1406bc620
1406bc616:  8D 41 FF                 LEA EAX,[RCX + -0x1]
1406bc619:  48 63 C8                 MOVSXD RCX,EAX
1406bc61c:  4D 8B 14 C8              MOV R10,qword ptr [R8 + RCX*0x8]
1406bc620:  48 89 74 24 30           MOV qword ptr [RSP + 0x30],RSI
1406bc625:  49 8B 86 F8 04 00 00     MOV RAX,qword ptr [R14 + 0x4f8]
1406bc62c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1406bc631:  48 8D 44 24 60           LEA RAX,[RSP + 0x60]
1406bc636:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1406bc63b:  4C 8D 4C 24 48           LEA R9,[RSP + 0x48]
1406bc640:  4D 8B C2                 MOV R8,R10
1406bc643:  48 8D 0D C6 88 31 00     LEA RCX,[0x1409d4f10]
1406bc64a:  E8 31 3D A6 FF           CALL 0x140120380
1406bc64f:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
1406bc654:  4C 8B 64 24 48           MOV R12,qword ptr [RSP + 0x48]
1406bc659:  4D 2B DC                 SUB R11,R12
1406bc65c:  49 C1 FB 03              SAR R11,0x3
1406bc660:  4C 89 9C 24 D0 00 00 00  MOV qword ptr [RSP + 0xd0],R11
1406bc668:  4D 85 DB                 TEST R11,R11
1406bc66b:  0F 84 22 02 00 00        JZ 0x1406bc893
1406bc671:  49 8B CE                 MOV RCX,R14
1406bc674:  E8 77 F7 FF FF           CALL 0x1406bbdf0
1406bc679:  44 8B E6                 MOV R12D,ESI
1406bc67c:  4C 8B 6C 24 60           MOV R13,qword ptr [RSP + 0x60]
1406bc681:  4D 85 DB                 TEST R11,R11
1406bc684:  0F 84 85 01 00 00        JZ 0x1406bc80f
1406bc68a:  49 8D BE B8 06 00 00     LEA RDI,[R14 + 0x6b8]
1406bc691:  4D 8B FD                 MOV R15,R13
1406bc694:  48 8B 44 24 48           MOV RAX,qword ptr [RSP + 0x48]
1406bc699:  49 2B C5                 SUB RAX,R13
1406bc69c:  4C 8B E8                 MOV R13,RAX
1406bc69f:  90                       NOP
1406bc6a0:  4B 8B 34 2F              MOV RSI,qword ptr [R15 + R13*0x1]
1406bc6a4:  48 89 B4 24 C0 00 00 00  MOV qword ptr [RSP + 0xc0],RSI
1406bc6ac:  49 8B 2F                 MOV RBP,qword ptr [R15]
1406bc6af:  48 89 AC 24 C8 00 00 00  MOV qword ptr [RSP + 0xc8],RBP
1406bc6b7:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
1406bc6bb:  48 8D 8C 24 C0 00 00 00  LEA RCX,[RSP + 0xc0]
1406bc6c3:  48 3B C8                 CMP RCX,RAX
1406bc6c6:  73 47                    JNC 0x1406bc70f
1406bc6c8:  48 8B 0B                 MOV RCX,qword ptr [RBX]
1406bc6cb:  48 8D 94 24 C0 00 00 00  LEA RDX,[RSP + 0xc0]
1406bc6d3:  48 3B CA                 CMP RCX,RDX
1406bc6d6:  77 37                    JA 0x1406bc70f
1406bc6d8:  48 8D B4 24 C0 00 00 00  LEA RSI,[RSP + 0xc0]
1406bc6e0:  48 2B F1                 SUB RSI,RCX
1406bc6e3:  48 C1 FE 03              SAR RSI,0x3
1406bc6e7:  48 3B 43 10              CMP RAX,qword ptr [RBX + 0x10]
1406bc6eb:  75 0D                    JNZ 0x1406bc6fa
1406bc6ed:  BA 01 00 00 00           MOV EDX,0x1
1406bc6f2:  48 8B CB                 MOV RCX,RBX
1406bc6f5:  E8 06 50 9F FF           CALL 0x1400b1700
1406bc6fa:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
1406bc6fe:  48 8B 03                 MOV RAX,qword ptr [RBX]
1406bc701:  48 85 C9                 TEST RCX,RCX
1406bc704:  74 28                    JZ 0x1406bc72e
1406bc706:  48 8B 04 F0              MOV RAX,qword ptr [RAX + RSI*0x8]
1406bc70a:  48 89 01                 MOV qword ptr [RCX],RAX
1406bc70d:  EB 1F                    JMP 0x1406bc72e
1406bc70f:  48 3B 43 10              CMP RAX,qword ptr [RBX + 0x10]
1406bc713:  75 0D                    JNZ 0x1406bc722
1406bc715:  BA 01 00 00 00           MOV EDX,0x1
1406bc71a:  48 8B CB                 MOV RCX,RBX
1406bc71d:  E8 DE 4F 9F FF           CALL 0x1400b1700
1406bc722:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
1406bc726:  48 85 C0                 TEST RAX,RAX
1406bc729:  74 03                    JZ 0x1406bc72e
1406bc72b:  48 89 30                 MOV qword ptr [RAX],RSI
1406bc72e:  48 83 43 08 08           ADD qword ptr [RBX + 0x8],0x8
1406bc733:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
1406bc737:  48 8D 8C 24 C8 00 00 00  LEA RCX,[RSP + 0xc8]
1406bc73f:  48 3B C8                 CMP RCX,RAX
1406bc742:  73 47                    JNC 0x1406bc78b
1406bc744:  48 8B 0F                 MOV RCX,qword ptr [RDI]
1406bc747:  48 8D 94 24 C8 00 00 00  LEA RDX,[RSP + 0xc8]
1406bc74f:  48 3B CA                 CMP RCX,RDX
1406bc752:  77 37                    JA 0x1406bc78b
1406bc754:  48 8D B4 24 C8 00 00 00  LEA RSI,[RSP + 0xc8]
1406bc75c:  48 2B F1                 SUB RSI,RCX
1406bc75f:  48 C1 FE 03              SAR RSI,0x3
1406bc763:  48 3B 47 10              CMP RAX,qword ptr [RDI + 0x10]
1406bc767:  75 0D                    JNZ 0x1406bc776
1406bc769:  BA 01 00 00 00           MOV EDX,0x1
1406bc76e:  48 8B CF                 MOV RCX,RDI
1406bc771:  E8 8A 4F 9F FF           CALL 0x1400b1700
1406bc776:  48 8B 4F 08              MOV RCX,qword ptr [RDI + 0x8]
1406bc77a:  48 8B 07                 MOV RAX,qword ptr [RDI]
1406bc77d:  48 85 C9                 TEST RCX,RCX
1406bc780:  74 28                    JZ 0x1406bc7aa
1406bc782:  48 8B 04 F0              MOV RAX,qword ptr [RAX + RSI*0x8]
1406bc786:  48 89 01                 MOV qword ptr [RCX],RAX
1406bc789:  EB 1F                    JMP 0x1406bc7aa
1406bc78b:  48 3B 47 10              CMP RAX,qword ptr [RDI + 0x10]
1406bc78f:  75 0D                    JNZ 0x1406bc79e
1406bc791:  BA 01 00 00 00           MOV EDX,0x1
1406bc796:  48 8B CF                 MOV RCX,RDI
1406bc799:  E8 62 4F 9F FF           CALL 0x1400b1700
1406bc79e:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
1406bc7a2:  48 85 C0                 TEST RAX,RAX
1406bc7a5:  74 03                    JZ 0x1406bc7aa
1406bc7a7:  48 89 28                 MOV qword ptr [RAX],RBP
1406bc7aa:  48 83 47 08 08           ADD qword ptr [RDI + 0x8],0x8
1406bc7af:  48 85 ED                 TEST RBP,RBP
1406bc7b2:  74 3C                    JZ 0x1406bc7f0
1406bc7b4:  48 8B 95 00 01 00 00     MOV RDX,qword ptr [RBP + 0x100]
1406bc7bb:  48 85 D2                 TEST RDX,RDX
1406bc7be:  74 1A                    JZ 0x1406bc7da
1406bc7c0:  48 C7 85 00 01 00 00 00 00 00 00 MOV qword ptr [RBP + 0x100],0x0
1406bc7cb:  48 81 C2 48 0D 00 00     ADD RDX,0xd48
1406bc7d2:  48 8B CD                 MOV RCX,RBP
1406bc7d5:  E8 F6 D0 A3 FF           CALL 0x1400f98d0
1406bc7da:  4C 89 B5 00 01 00 00     MOV qword ptr [RBP + 0x100],R14
1406bc7e1:  49 8D 96 48 0D 00 00     LEA RDX,[R14 + 0xd48]
1406bc7e8:  48 8B CD                 MOV RCX,RBP
1406bc7eb:  E8 10 D2 A3 FF           CALL 0x1400f9a00
1406bc7f0:  41 FF C4                 INC R12D
1406bc7f3:  49 83 C7 08              ADD R15,0x8
1406bc7f7:  49 63 C4                 MOVSXD RAX,R12D
1406bc7fa:  48 3B 84 24 D0 00 00 00  CMP RAX,qword ptr [RSP + 0xd0]
1406bc802:  0F 82 98 FE FF FF        JC 0x1406bc6a0
1406bc808:  4C 8B 6C 24 60           MOV R13,qword ptr [RSP + 0x60]
1406bc80d:  33 F6                    XOR ESI,ESI
1406bc80f:  49 63 8E 98 06 00 00     MOVSXD RCX,dword ptr [R14 + 0x698]
1406bc816:  85 C9                    TEST ECX,ECX
1406bc818:  78 51                    JS 0x1406bc86b
1406bc81a:  49 8B 86 88 06 00 00     MOV RAX,qword ptr [R14 + 0x688]
1406bc821:  49 2B 86 80 06 00 00     SUB RAX,qword ptr [R14 + 0x680]
1406bc828:  48 C1 F8 03              SAR RAX,0x3
1406bc82c:  48 8B D1                 MOV RDX,RCX
1406bc82f:  48 3B C8                 CMP RCX,RAX
1406bc832:  73 37                    JNC 0x1406bc86b
1406bc834:  49 8B 86 80 06 00 00     MOV RAX,qword ptr [R14 + 0x680]
1406bc83b:  48 83 3C C8 00           CMP qword ptr [RAX + RCX*0x8],0x0
1406bc840:  75 29                    JNZ 0x1406bc86b
1406bc842:  4C 8B 43 08              MOV R8,qword ptr [RBX + 0x8]
1406bc846:  4C 2B 03                 SUB R8,qword ptr [RBX]
1406bc849:  49 C1 F8 03              SAR R8,0x3
1406bc84d:  48 8B 03                 MOV RAX,qword ptr [RBX]
1406bc850:  48 69 D2 98 01 00 00     IMUL RDX,RDX,0x198
1406bc857:  49 8B 8E 38 06 00 00     MOV RCX,qword ptr [R14 + 0x638]
1406bc85e:  4A 8B 44 C0 F8           MOV RAX,qword ptr [RAX + R8*0x8 + -0x8]
1406bc863:  48 89 84 0A 90 01 00 00  MOV qword ptr [RDX + RCX*0x1 + 0x190],RAX
1406bc86b:  40 B7 01                 MOV DIL,0x1
1406bc86e:  49 8B 86 80 0C 00 00     MOV RAX,qword ptr [R14 + 0xc80]
1406bc875:  48 85 C0                 TEST RAX,RAX
1406bc878:  74 25                    JZ 0x1406bc89f
1406bc87a:  4C 39 B0 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R14
1406bc881:  75 07                    JNZ 0x1406bc88a
1406bc883:  48 89 B0 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],RSI
1406bc88a:  49 89 B6 80 0C 00 00     MOV qword ptr [R14 + 0xc80],RSI
1406bc891:  EB 0C                    JMP 0x1406bc89f
1406bc893:  4C 8B 6C 24 60           MOV R13,qword ptr [RSP + 0x60]
1406bc898:  EB 0A                    JMP 0x1406bc8a4
1406bc89a:  4C 8B 6C 24 60           MOV R13,qword ptr [RSP + 0x60]
1406bc89f:  4C 8B 64 24 48           MOV R12,qword ptr [RSP + 0x48]
1406bc8a4:  4D 85 ED                 TEST R13,R13
1406bc8a7:  74 1B                    JZ 0x1406bc8c4
1406bc8a9:  48 8B 54 24 70           MOV RDX,qword ptr [RSP + 0x70]
1406bc8ae:  49 2B D5                 SUB RDX,R13
1406bc8b1:  48 C1 FA 03              SAR RDX,0x3
1406bc8b5:  41 B8 08 00 00 00        MOV R8D,0x8
1406bc8bb:  49 8B CD                 MOV RCX,R13
1406bc8be:  E8 8D 51 94 FF           CALL 0x140001a50
1406bc8c3:  90                       NOP
1406bc8c4:  4D 85 E4                 TEST R12,R12
1406bc8c7:  74 1A                    JZ 0x1406bc8e3
1406bc8c9:  48 8B 54 24 58           MOV RDX,qword ptr [RSP + 0x58]
1406bc8ce:  49 2B D4                 SUB RDX,R12
1406bc8d1:  48 C1 FA 03              SAR RDX,0x3
1406bc8d5:  41 B8 08 00 00 00        MOV R8D,0x8
1406bc8db:  49 8B CC                 MOV RCX,R12
1406bc8de:  E8 6D 51 94 FF           CALL 0x140001a50
1406bc8e3:  40 0F B6 C7              MOVZX EAX,DIL
1406bc8e7:  48 8B 9C 24 D8 00 00 00  MOV RBX,qword ptr [RSP + 0xd8]
1406bc8ef:  48 81 C4 80 00 00 00     ADD RSP,0x80
1406bc8f6:  41 5F                    POP R15
1406bc8f8:  41 5E                    POP R14
1406bc8fa:  41 5D                    POP R13
1406bc8fc:  41 5C                    POP R12
1406bc8fe:  5F                       POP RDI
1406bc8ff:  5E                       POP RSI
1406bc900:  5D                       POP RBP
1406bc901:  C3                       RET
1406bc910:  33 C0                    XOR EAX,EAX
1406bc912:  4C 8B D1                 MOV R10,RCX
1406bc915:  85 D2                    TEST EDX,EDX
1406bc917:  44 8B C8                 MOV R9D,EAX
1406bc91a:  44 0F 49 CA              CMOVNS R9D,EDX
1406bc91e:  4D 63 D9                 MOVSXD R11,R9D
1406bc921:  4E 8D 04 DD 08 00 00 00  LEA R8,[0x8 + R11*0x8]
1406bc929:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
1406bc930:  49 8B 8A A8 06 00 00     MOV RCX,qword ptr [R10 + 0x6a8]
1406bc937:  49 2B 8A A0 06 00 00     SUB RCX,qword ptr [R10 + 0x6a0]
1406bc93e:  48 C1 F9 03              SAR RCX,0x3
1406bc942:  44 3B C9                 CMP R9D,ECX
1406bc945:  7D 45                    JGE 0x1406bc98c
1406bc947:  49 8D 0C 03              LEA RCX,[R11 + RAX*0x1]
1406bc94b:  48 85 C9                 TEST RCX,RCX
1406bc94e:  7E 2C                    JLE 0x1406bc97c
1406bc950:  49 8B 92 A8 06 00 00     MOV RDX,qword ptr [R10 + 0x6a8]
1406bc957:  41 8D 49 01              LEA ECX,[R9 + 0x1]
1406bc95b:  49 2B 92 A0 06 00 00     SUB RDX,qword ptr [R10 + 0x6a0]
1406bc962:  48 C1 FA 03              SAR RDX,0x3
1406bc966:  3B CA                    CMP ECX,EDX
1406bc968:  7D 12                    JGE 0x1406bc97c
1406bc96a:  49 8B 92 A0 06 00 00     MOV RDX,qword ptr [R10 + 0x6a0]
1406bc971:  4A 8B 0C 02              MOV RCX,qword ptr [RDX + R8*0x1]
1406bc975:  4A 39 4C 02 F0           CMP qword ptr [RDX + R8*0x1 + -0x10],RCX
1406bc97a:  74 13                    JZ 0x1406bc98f
1406bc97c:  41 FF C1                 INC R9D
1406bc97f:  48 FF C0                 INC RAX
1406bc982:  49 83 C0 08              ADD R8,0x8
1406bc986:  48 83 F8 03              CMP RAX,0x3
1406bc98a:  7C A4                    JL 0x1406bc930
1406bc98c:  32 C0                    XOR AL,AL
1406bc98e:  C3                       RET
1406bc98f:  B0 01                    MOV AL,0x1
1406bc991:  C3                       RET
1406bc9a0:  4C 8B DC                 MOV R11,RSP
1406bc9a3:  55                       PUSH RBP
1406bc9a4:  53                       PUSH RBX
1406bc9a5:  49 8D AB A8 FB FF FF     LEA RBP,[R11 + -0x458]
1406bc9ac:  48 81 EC 48 05 00 00     SUB RSP,0x548
1406bc9b3:  48 8B 05 8E 56 2D 00     MOV RAX,qword ptr [0x140992048]
1406bc9ba:  48 33 C4                 XOR RAX,RSP
1406bc9bd:  48 89 85 10 04 00 00     MOV qword ptr [RBP + 0x410],RAX
1406bc9c4:  49 89 73 10              MOV qword ptr [R11 + 0x10],RSI
1406bc9c8:  41 0F B6 D8              MOVZX EBX,R8B
1406bc9cc:  49 89 7B 18              MOV qword ptr [R11 + 0x18],RDI
1406bc9d0:  48 8B F9                 MOV RDI,RCX
1406bc9d3:  4D 89 63 20              MOV qword ptr [R11 + 0x20],R12
1406bc9d7:  4D 89 6B E8              MOV qword ptr [R11 + -0x18],R13
1406bc9db:  45 33 ED                 XOR R13D,R13D
1406bc9de:  4D 89 7B D8              MOV qword ptr [R11 + -0x28],R15
1406bc9e2:  41 0F 29 73 C8           MOVAPS xmmword ptr [R11 + -0x38],XMM6
1406bc9e7:  88 5C 24 20              MOV byte ptr [RSP + 0x20],BL
1406bc9eb:  88 54 24 21              MOV byte ptr [RSP + 0x21],DL
1406bc9ef:  48 89 4C 24 50           MOV qword ptr [RSP + 0x50],RCX
1406bc9f4:  4C 39 A9 30 05 00 00     CMP qword ptr [RCX + 0x530],R13
1406bc9fb:  74 07                    JZ 0x1406bca04
1406bc9fd:  4C 89 A9 30 05 00 00     MOV qword ptr [RCX + 0x530],R13

