; Range: 0x14066D500 to 0x14066EB00
14066d503:  0F 28 BC 24 20 01 00 00  MOVAPS XMM7,xmmword ptr [RSP + 0x120]
14066d50b:  0F 28 B4 24 30 01 00 00  MOVAPS XMM6,xmmword ptr [RSP + 0x130]
14066d513:  48 8B B4 24 68 01 00 00  MOV RSI,qword ptr [RSP + 0x168]
14066d51b:  44 0F 28 B4 24 B0 00 00 00 MOVAPS XMM14,xmmword ptr [RSP + 0xb0]
14066d524:  48 81 C4 40 01 00 00     ADD RSP,0x140
14066d52b:  41 5F                    POP R15
14066d52d:  5F                       POP RDI
14066d52e:  5D                       POP RBP
14066d52f:  C3                       RET
14066d530:  48 8B C4                 MOV RAX,RSP
14066d533:  48 89 48 08              MOV qword ptr [RAX + 0x8],RCX
14066d537:  55                       PUSH RBP
14066d538:  53                       PUSH RBX
14066d539:  41 54                    PUSH R12
14066d53b:  41 56                    PUSH R14
14066d53d:  41 57                    PUSH R15
14066d53f:  48 8D 68 A1              LEA RBP,[RAX + -0x5f]
14066d543:  48 81 EC 00 01 00 00     SUB RSP,0x100
14066d54a:  0F 29 78 A8              MOVAPS xmmword ptr [RAX + -0x58],XMM7
14066d54e:  48 8B D9                 MOV RBX,RCX
14066d551:  44 0F 29 40 98           MOVAPS xmmword ptr [RAX + -0x68],XMM8
14066d556:  48 8D 0D 83 79 36 00     LEA RCX,[0x1409d4ee0]
14066d55d:  F3 44 0F 10 05 96 C5 29 00 MOVSS XMM8,dword ptr [0x140909afc]
14066d566:  45 33 C9                 XOR R9D,R9D
14066d569:  44 0F 29 48 88           MOVAPS xmmword ptr [RAX + -0x78],XMM9
14066d56e:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066d572:  45 33 C0                 XOR R8D,R8D
14066d575:  44 0F 29 90 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM10
14066d57d:  FF 15 A5 EA 1F 00        CALL qword ptr [0x14086c028]
14066d583:  F3 0F 58 03              ADDSS XMM0,dword ptr [RBX]
14066d587:  45 33 FF                 XOR R15D,R15D
14066d58a:  0F 2F 05 87 D7 29 00     COMISS XMM0,dword ptr [0x14090ad18]
14066d591:  F3 0F 11 03              MOVSS dword ptr [RBX],XMM0
14066d595:  76 03                    JBE 0x14066d59a
14066d597:  44 89 3B                 MOV dword ptr [RBX],R15D
14066d59a:  4C 8D A3 60 0D 00 00     LEA R12,[RBX + 0xd60]
14066d5a1:  49 8B CC                 MOV RCX,R12
14066d5a4:  48 8D 55 87              LEA RDX,[RBP + -0x79]
14066d5a8:  FF 15 0A F2 1F 00        CALL qword ptr [0x14086c7b8]
14066d5ae:  0F B6 83 D8 13 00 00     MOVZX EAX,byte ptr [RBX + 0x13d8]
14066d5b5:  88 83 D9 13 00 00        MOV byte ptr [RBX + 0x13d9],AL
14066d5bb:  4C 89 BB C8 0C 00 00     MOV qword ptr [RBX + 0xcc8],R15
14066d5c2:  44 88 BB 90 17 00 00     MOV byte ptr [RBX + 0x1790],R15B
14066d5c9:  44 38 BB 5C 19 00 00     CMP byte ptr [RBX + 0x195c],R15B
14066d5d0:  74 0F                    JZ 0x14066d5e1
14066d5d2:  48 8B CB                 MOV RCX,RBX
14066d5d5:  44 88 BB 5C 19 00 00     MOV byte ptr [RBX + 0x195c],R15B
14066d5dc:  E8 EF ED BE FF           CALL 0x14025c3d0
14066d5e1:  44 38 BB 5D 19 00 00     CMP byte ptr [RBX + 0x195d],R15B
14066d5e8:  74 0F                    JZ 0x14066d5f9
14066d5ea:  48 8B CB                 MOV RCX,RBX
14066d5ed:  44 88 BB 5D 19 00 00     MOV byte ptr [RBX + 0x195d],R15B
14066d5f4:  E8 07 3A 04 00           CALL 0x1406b1000
14066d5f9:  F3 0F 10 83 24 0D 00 00  MOVSS XMM0,dword ptr [RBX + 0xd24]
14066d601:  0F 57 FF                 XORPS XMM7,XMM7
14066d604:  0F 2E C7                 UCOMISS XMM0,XMM7
14066d607:  7A 09                    JP 0x14066d612
14066d609:  75 07                    JNZ 0x14066d612
14066d60b:  44 89 BB 60 11 00 00     MOV dword ptr [RBX + 0x1160],R15D
14066d612:  4C 8B 83 08 17 00 00     MOV R8,qword ptr [RBX + 0x1708]
14066d619:  49 BE AB AA AA AA AA AA AA 2A MOV R14,0x2aaaaaaaaaaaaaab
14066d623:  F3 44 0F 10 15 2C C9 29 00 MOVSS XMM10,dword ptr [0x140909f58]
14066d62c:  41 83 B8 00 86 00 00 02  CMP dword ptr [R8 + 0x8600],0x2
14066d634:  0F 85 E9 01 00 00        JNZ 0x14066d823
14066d63a:  48 8B 83 28 05 00 00     MOV RAX,qword ptr [RBX + 0x528]
14066d641:  44 89 BB 14 16 00 00     MOV dword ptr [RBX + 0x1614],R15D
14066d648:  48 85 C0                 TEST RAX,RAX
14066d64b:  0F 84 29 01 00 00        JZ 0x14066d77a
14066d651:  48 8B 88 D0 05 00 00     MOV RCX,qword ptr [RAX + 0x5d0]
14066d658:  48 85 C9                 TEST RCX,RCX
14066d65b:  74 1F                    JZ 0x14066d67c
14066d65d:  48 39 8B 98 07 00 00     CMP qword ptr [RBX + 0x798],RCX
14066d664:  75 16                    JNZ 0x14066d67c
14066d666:  F3 0F 10 80 DC 0D 00 00  MOVSS XMM0,dword ptr [RAX + 0xddc]
14066d66e:  0F 2F C7                 COMISS XMM0,XMM7
14066d671:  77 6D                    JA 0x14066d6e0
14066d673:  0F 2F B8 C0 0F 00 00     COMISS XMM7,dword ptr [RAX + 0xfc0]
14066d67a:  77 64                    JA 0x14066d6e0
14066d67c:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066d683:  49 8B C6                 MOV RAX,R14
14066d686:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066d68d:  48 F7 E9                 IMUL RCX
14066d690:  48 D1 FA                 SAR RDX,0x1
14066d693:  48 8B C2                 MOV RAX,RDX
14066d696:  48 C1 E8 3F              SHR RAX,0x3f
14066d69a:  48 03 D0                 ADD RDX,RAX
14066d69d:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066d6a3:  0F 8C D1 00 00 00        JL 0x14066d77a
14066d6a9:  F3 0F 10 83 7C 17 00 00  MOVSS XMM0,dword ptr [RBX + 0x177c]
14066d6b1:  0F 2E C7                 UCOMISS XMM0,XMM7
14066d6b4:  0F 8A C0 00 00 00        JP 0x14066d77a
14066d6ba:  0F 85 BA 00 00 00        JNZ 0x14066d77a
14066d6c0:  F3 0F 10 83 64 05 00 00  MOVSS XMM0,dword ptr [RBX + 0x564]
14066d6c8:  0F 2E C7                 UCOMISS XMM0,XMM7
14066d6cb:  7A 06                    JP 0x14066d6d3
14066d6cd:  0F 84 A7 00 00 00        JZ 0x14066d77a
14066d6d3:  44 39 BB 94 05 00 00     CMP dword ptr [RBX + 0x594],R15D
14066d6da:  0F 85 9A 00 00 00        JNZ 0x14066d77a
14066d6e0:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
14066d6e7:  49 8B D7                 MOV RDX,R15
14066d6ea:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
14066d6f1:  0F 28 C7                 MOVAPS XMM0,XMM7
14066d6f4:  48 C1 F8 04              SAR RAX,0x4
14066d6f8:  0F 28 D7                 MOVAPS XMM2,XMM7
14066d6fb:  41 8B CF                 MOV ECX,R15D
14066d6fe:  48 85 C0                 TEST RAX,RAX
14066d701:  74 77                    JZ 0x14066d77a
14066d703:  4C 8B 8B B8 03 00 00     MOV R9,qword ptr [RBX + 0x3b8]
14066d70a:  49 8B C7                 MOV RAX,R15
14066d70d:  4C 8B 93 C0 03 00 00     MOV R10,qword ptr [RBX + 0x3c0]
14066d714:  4D 2B D1                 SUB R10,R9
14066d717:  49 C1 FA 04              SAR R10,0x4
14066d71b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14066d720:  48 03 C0                 ADD RAX,RAX
14066d723:  F3 41 0F 10 4C C1 08     MOVSS XMM1,dword ptr [R9 + RAX*0x8 + 0x8]
14066d72a:  0F 2F CF                 COMISS XMM1,XMM7
14066d72d:  76 24                    JBE 0x14066d753
14066d72f:  49 8B 14 C1              MOV RDX,qword ptr [R9 + RAX*0x8]
14066d733:  0F 28 D1                 MOVAPS XMM2,XMM1
14066d736:  49 63 80 00 86 00 00     MOVSXD RAX,dword ptr [R8 + 0x8600]
14066d73d:  48 C1 E0 05              SHL RAX,0x5
14066d741:  F3 0F 10 84 10 CC 00 00 00 MOVSS XMM0,dword ptr [RAX + RDX*0x1 + 0xcc]
14066d74a:  F3 41 0F 59 80 04 86 00 00 MULSS XMM0,dword ptr [R8 + 0x8604]
14066d753:  FF C1                    INC ECX
14066d755:  8B C1                    MOV EAX,ECX
14066d757:  49 3B C2                 CMP RAX,R10
14066d75a:  72 C4                    JC 0x14066d720
14066d75c:  0F 2F D7                 COMISS XMM2,XMM7
14066d75f:  76 19                    JBE 0x14066d77a
14066d761:  48 85 D2                 TEST RDX,RDX
14066d764:  74 14                    JZ 0x14066d77a
14066d766:  F3 0F 5E D0              DIVSS XMM2,XMM0
14066d76a:  41 0F 28 C2              MOVAPS XMM0,XMM10
14066d76e:  F3 0F 5C C2              SUBSS XMM0,XMM2
14066d772:  F3 0F 11 83 14 16 00 00  MOVSS dword ptr [RBX + 0x1614],XMM0
14066d77a:  F3 0F 10 83 10 16 00 00  MOVSS XMM0,dword ptr [RBX + 0x1610]
14066d782:  F3 0F 10 8B 14 16 00 00  MOVSS XMM1,dword ptr [RBX + 0x1614]
14066d78a:  0F 2E C1                 UCOMISS XMM0,XMM1
14066d78d:  7A 06                    JP 0x14066d795
14066d78f:  0F 84 8E 00 00 00        JZ 0x14066d823
14066d795:  0F 2F C8                 COMISS XMM1,XMM0
14066d798:  76 3C                    JBE 0x14066d7d6
14066d79a:  45 33 C9                 XOR R9D,R9D
14066d79d:  48 8D 0D 3C 77 36 00     LEA RCX,[0x1409d4ee0]
14066d7a4:  45 33 C0                 XOR R8D,R8D
14066d7a7:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066d7ab:  FF 15 77 E8 1F 00        CALL qword ptr [0x14086c028]
14066d7b1:  F3 0F 58 83 10 16 00 00  ADDSS XMM0,dword ptr [RBX + 0x1610]
14066d7b9:  F3 0F 10 8B 14 16 00 00  MOVSS XMM1,dword ptr [RBX + 0x1614]
14066d7c1:  0F 2F C1                 COMISS XMM0,XMM1
14066d7c4:  F3 0F 11 83 10 16 00 00  MOVSS dword ptr [RBX + 0x1610],XMM0
14066d7cc:  76 08                    JBE 0x14066d7d6
14066d7ce:  F3 0F 11 8B 10 16 00 00  MOVSS dword ptr [RBX + 0x1610],XMM1
14066d7d6:  F3 0F 10 83 10 16 00 00  MOVSS XMM0,dword ptr [RBX + 0x1610]
14066d7de:  0F 2F C1                 COMISS XMM0,XMM1
14066d7e1:  76 40                    JBE 0x14066d823
14066d7e3:  45 33 C9                 XOR R9D,R9D
14066d7e6:  48 8D 0D F3 76 36 00     LEA RCX,[0x1409d4ee0]
14066d7ed:  45 33 C0                 XOR R8D,R8D
14066d7f0:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066d7f4:  FF 15 2E E8 1F 00        CALL qword ptr [0x14086c028]
14066d7fa:  F3 0F 10 8B 10 16 00 00  MOVSS XMM1,dword ptr [RBX + 0x1610]
14066d802:  F3 0F 5C C8              SUBSS XMM1,XMM0
14066d806:  F3 0F 10 83 14 16 00 00  MOVSS XMM0,dword ptr [RBX + 0x1614]
14066d80e:  0F 2F C1                 COMISS XMM0,XMM1
14066d811:  F3 0F 11 8B 10 16 00 00  MOVSS dword ptr [RBX + 0x1610],XMM1
14066d819:  76 08                    JBE 0x14066d823
14066d81b:  F3 0F 11 83 10 16 00 00  MOVSS dword ptr [RBX + 0x1610],XMM0
14066d823:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066d82a:  83 B8 00 86 00 00 08     CMP dword ptr [RAX + 0x8600],0x8
14066d831:  0F 85 86 00 00 00        JNZ 0x14066d8bd
14066d837:  44 39 BB 2C 16 00 00     CMP dword ptr [RBX + 0x162c],R15D
14066d83e:  7C 76                    JL 0x14066d8b6
14066d840:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
14066d847:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
14066d84e:  48 A9 F0 FF FF FF        TEST RAX,-0x10
14066d854:  74 60                    JZ 0x14066d8b6
14066d856:  48 8B 83 B8 03 00 00     MOV RAX,qword ptr [RBX + 0x3b8]
14066d85d:  F3 0F 10 40 08           MOVSS XMM0,dword ptr [RAX + 0x8]
14066d862:  0F 2F 05 5B C3 29 00     COMISS XMM0,dword ptr [0x140909bc4]
14066d869:  76 4B                    JBE 0x14066d8b6
14066d86b:  45 33 C9                 XOR R9D,R9D
14066d86e:  48 8D 0D 6B 76 36 00     LEA RCX,[0x1409d4ee0]
14066d875:  45 33 C0                 XOR R8D,R8D
14066d878:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066d87c:  FF 15 A6 E7 1F 00        CALL qword ptr [0x14086c028]
14066d882:  F3 0F 59 05 6E C7 29 00  MULSS XMM0,dword ptr [0x140909ff8]
14066d88a:  F3 0F 10 0D E6 CE 29 00  MOVSS XMM1,dword ptr [0x14090a778]
14066d892:  F3 0F 58 83 18 16 00 00  ADDSS XMM0,dword ptr [RBX + 0x1618]
14066d89a:  0F 2F C1                 COMISS XMM0,XMM1
14066d89d:  F3 0F 11 83 18 16 00 00  MOVSS dword ptr [RBX + 0x1618],XMM0
14066d8a5:  76 16                    JBE 0x14066d8bd
14066d8a7:  E8 3E 1C 1E 00           CALL 0x14084f4ea
14066d8ac:  F3 0F 11 83 18 16 00 00  MOVSS dword ptr [RBX + 0x1618],XMM0
14066d8b4:  EB 07                    JMP 0x14066d8bd
14066d8b6:  44 89 BB 18 16 00 00     MOV dword ptr [RBX + 0x1618],R15D
14066d8bd:  F3 0F 10 83 A0 17 00 00  MOVSS XMM0,dword ptr [RBX + 0x17a0]
14066d8c5:  F3 44 0F 10 0D AA C3 29 00 MOVSS XMM9,dword ptr [0x140909c78]
14066d8ce:  41 0F 2F C1              COMISS XMM0,XMM9
14066d8d2:  0F 82 8C 00 00 00        JC 0x14066d964
14066d8d8:  44 0F 2F D0              COMISS XMM10,XMM0
14066d8dc:  76 52                    JBE 0x14066d930
14066d8de:  48 8B 83 F0 04 00 00     MOV RAX,qword ptr [RBX + 0x4f0]
14066d8e5:  48 85 C0                 TEST RAX,RAX
14066d8e8:  74 12                    JZ 0x14066d8fc
14066d8ea:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14066d8f1:  74 09                    JZ 0x14066d8fc
14066d8f3:  44 88 3D 2A 9F 37 00     MOV byte ptr [0x1409e7824],R15B
14066d8fa:  EB 68                    JMP 0x14066d964
14066d8fc:  48 8B D3                 MOV RDX,RBX
14066d8ff:  E8 DC DF D8 FF           CALL 0x1403fb8e0
14066d904:  84 C0                    TEST AL,AL
14066d906:  75 1F                    JNZ 0x14066d927
14066d908:  C6 44 24 28 01           MOV byte ptr [RSP + 0x28],0x1
14066d90d:  48 8D 0D FC 75 36 00     LEA RCX,[0x1409d4f10]
14066d914:  45 33 C9                 XOR R9D,R9D
14066d917:  44 88 7C 24 20           MOV byte ptr [RSP + 0x20],R15B
14066d91c:  45 33 C0                 XOR R8D,R8D
14066d91f:  48 8B D3                 MOV RDX,RBX
14066d922:  E8 99 E4 D8 FF           CALL 0x1403fbdc0
14066d927:  44 89 BB A0 17 00 00     MOV dword ptr [RBX + 0x17a0],R15D
14066d92e:  EB 34                    JMP 0x14066d964
14066d930:  44 38 3D ED 9E 37 00     CMP byte ptr [0x1409e7824],R15B
14066d937:  74 2B                    JZ 0x14066d964
14066d939:  45 33 C9                 XOR R9D,R9D
14066d93c:  48 8D 0D 9D 75 36 00     LEA RCX,[0x1409d4ee0]
14066d943:  45 33 C0                 XOR R8D,R8D
14066d946:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066d94a:  FF 15 D8 E6 1F 00        CALL qword ptr [0x14086c028]
14066d950:  F3 0F 10 8B A0 17 00 00  MOVSS XMM1,dword ptr [RBX + 0x17a0]
14066d958:  F3 0F 5C C8              SUBSS XMM1,XMM0
14066d95c:  F3 0F 11 8B A0 17 00 00  MOVSS dword ptr [RBX + 0x17a0],XMM1
14066d964:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066d96b:  83 B8 94 02 00 00 07     CMP dword ptr [RAX + 0x294],0x7
14066d972:  75 0D                    JNZ 0x14066d981
14066d974:  48 8B CB                 MOV RCX,RBX
14066d977:  E8 64 2E 05 00           CALL 0x1406c07e0
14066d97c:  E9 25 14 00 00           JMP 0x14066eda6
14066d981:  48 8B 83 18 06 00 00     MOV RAX,qword ptr [RBX + 0x618]
14066d988:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066d98c:  48 89 83 20 06 00 00     MOV qword ptr [RBX + 0x620],RAX
14066d993:  49 8B CC                 MOV RCX,R12
14066d996:  8B 83 E0 13 00 00        MOV EAX,dword ptr [RBX + 0x13e0]
14066d99c:  89 83 E4 13 00 00        MOV dword ptr [RBX + 0x13e4],EAX
14066d9a2:  44 88 BB FC 05 00 00     MOV byte ptr [RBX + 0x5fc],R15B
14066d9a9:  66 44 89 BB DD 05 00 00  MOV word ptr [RBX + 0x5dd],R15W
14066d9b1:  F3 0F 10 05 57 C3 37 00  MOVSS XMM0,dword ptr [0x1409e9d10]
14066d9b9:  F3 0F 10 0D 53 C3 37 00  MOVSS XMM1,dword ptr [0x1409e9d14]
14066d9c1:  F3 0F 11 44 24 40        MOVSS dword ptr [RSP + 0x40],XMM0
14066d9c7:  F3 0F 10 05 49 C3 37 00  MOVSS XMM0,dword ptr [0x1409e9d18]
14066d9cf:  48 89 BC 24 F8 00 00 00  MOV qword ptr [RSP + 0xf8],RDI
14066d9d7:  4C 89 AC 24 F0 00 00 00  MOV qword ptr [RSP + 0xf0],R13
14066d9df:  F3 0F 11 44 24 48        MOVSS dword ptr [RSP + 0x48],XMM0
14066d9e5:  F3 0F 11 4C 24 44        MOVSS dword ptr [RSP + 0x44],XMM1
14066d9eb:  0F 29 B4 24 E0 00 00 00  MOVAPS xmmword ptr [RSP + 0xe0],XMM6
14066d9f3:  FF 15 BF ED 1F 00        CALL qword ptr [0x14086c7b8]
14066d9f9:  48 8B C8                 MOV RCX,RAX
14066d9fc:  48 8D 54 24 40           LEA RDX,[RSP + 0x40]
14066da01:  FF 15 E9 ED 1F 00        CALL qword ptr [0x14086c7f0]
14066da07:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066da0e:  F3 0F 11 83 E0 13 00 00  MOVSS dword ptr [RBX + 0x13e0],XMM0
14066da16:  48 8B 88 B8 02 00 00     MOV RCX,qword ptr [RAX + 0x2b8]
14066da1d:  48 85 C9                 TEST RCX,RCX
14066da20:  0F 84 B7 00 00 00        JZ 0x14066dadd
14066da26:  44 38 BB D8 13 00 00     CMP byte ptr [RBX + 0x13d8],R15B
14066da2d:  0F 84 AA 00 00 00        JZ 0x14066dadd
14066da33:  48 8B 81 B0 01 00 00     MOV RAX,qword ptr [RCX + 0x1b0]
14066da3a:  F3 0F 10 83 D4 13 00 00  MOVSS XMM0,dword ptr [RBX + 0x13d4]
14066da42:  0F 2F F8                 COMISS XMM7,XMM0
14066da45:  66 0F 6E B0 9C 00 00 00  MOVD XMM6,dword ptr [RAX + 0x9c]
14066da4d:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14066da50:  77 05                    JA 0x14066da57
14066da52:  0F 2F C6                 COMISS XMM0,XMM6
14066da55:  76 14                    JBE 0x14066da6b
14066da57:  0F 28 CE                 MOVAPS XMM1,XMM6
14066da5a:  0F 28 C7                 MOVAPS XMM0,XMM7
14066da5d:  FF 15 15 F0 1F 00        CALL qword ptr [0x14086ca78]
14066da63:  F3 0F 11 83 D4 13 00 00  MOVSS dword ptr [RBX + 0x13d4],XMM0
14066da6b:  45 33 C9                 XOR R9D,R9D
14066da6e:  48 8D 0D 6B 74 36 00     LEA RCX,[0x1409d4ee0]
14066da75:  45 33 C0                 XOR R8D,R8D
14066da78:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066da7c:  4C 39 BB 98 03 00 00     CMP qword ptr [RBX + 0x398],R15
14066da83:  74 17                    JZ 0x14066da9c
14066da85:  FF 15 9D E5 1F 00        CALL qword ptr [0x14086c028]
14066da8b:  48 8B 83 98 03 00 00     MOV RAX,qword ptr [RBX + 0x398]
14066da92:  F3 0F 59 80 7C 17 00 00  MULSS XMM0,dword ptr [RAX + 0x177c]
14066da9a:  EB 0E                    JMP 0x14066daaa
14066da9c:  FF 15 86 E5 1F 00        CALL qword ptr [0x14086c028]
14066daa2:  F3 0F 59 83 7C 17 00 00  MULSS XMM0,dword ptr [RBX + 0x177c]
14066daaa:  F3 0F 5E 05 BE D0 29 00  DIVSS XMM0,dword ptr [0x14090ab70]
14066dab2:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066dab9:  F3 0F 59 80 60 7A 00 00  MULSS XMM0,dword ptr [RAX + 0x7a60]
14066dac1:  F3 0F 58 83 D4 13 00 00  ADDSS XMM0,dword ptr [RBX + 0x13d4]
14066dac9:  0F 2F C6                 COMISS XMM0,XMM6
14066dacc:  F3 0F 11 83 D4 13 00 00  MOVSS dword ptr [RBX + 0x13d4],XMM0
14066dad4:  76 07                    JBE 0x14066dadd
14066dad6:  44 89 BB D4 13 00 00     MOV dword ptr [RBX + 0x13d4],R15D
14066dadd:  83 B8 94 02 00 00 09     CMP dword ptr [RAX + 0x294],0x9
14066dae4:  BF 01 00 00 00           MOV EDI,0x1
14066dae9:  0F 85 FC 01 00 00        JNZ 0x14066dceb
14066daef:  83 B8 00 86 00 00 0D     CMP dword ptr [RAX + 0x8600],0xd
14066daf6:  0F 85 EF 01 00 00        JNZ 0x14066dceb
14066dafc:  44 39 3D E1 79 36 00     CMP dword ptr [0x1409d54e4],R15D
14066db03:  0F 84 E2 01 00 00        JZ 0x14066dceb
14066db09:  45 33 C9                 XOR R9D,R9D
14066db0c:  48 8D 0D CD 73 36 00     LEA RCX,[0x1409d4ee0]
14066db13:  45 33 C0                 XOR R8D,R8D
14066db16:  41 0F 28 C8              MOVAPS XMM1,XMM8
14066db1a:  FF 15 08 E5 1F 00        CALL qword ptr [0x14086c028]
14066db20:  44 38 3D 35 B8 37 00     CMP byte ptr [0x1409e935c],R15B
14066db27:  8B C7                    MOV EAX,EDI
14066db29:  B9 64 00 00 00           MOV ECX,0x64
14066db2e:  0F 45 C1                 CMOVNZ EAX,ECX
14066db31:  66 0F 6E C8              MOVD XMM1,EAX
14066db35:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
14066db38:  F3 0F 59 C1              MULSS XMM0,XMM1
14066db3c:  F3 0F 58 83 54 04 00 00  ADDSS XMM0,dword ptr [RBX + 0x454]
14066db44:  0F 2F 05 DD CC 29 00     COMISS XMM0,dword ptr [0x14090a828]
14066db4b:  F3 0F 11 83 54 04 00 00  MOVSS dword ptr [RBX + 0x454],XMM0
14066db53:  0F 86 92 01 00 00        JBE 0x14066dceb
14066db59:  44 89 BB 54 04 00 00     MOV dword ptr [RBX + 0x454],R15D
14066db60:  32 D2                    XOR DL,DL
14066db62:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
14066db69:  45 8B C7                 MOV R8D,R15D
14066db6c:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
14066db73:  48 C1 F8 04              SAR RAX,0x4
14066db77:  48 85 C0                 TEST RAX,RAX
14066db7a:  0F 84 6B 01 00 00        JZ 0x14066dceb
14066db80:  4C 8B 8B B8 03 00 00     MOV R9,qword ptr [RBX + 0x3b8]
14066db87:  49 8B CF                 MOV RCX,R15
14066db8a:  4C 8B 93 C0 03 00 00     MOV R10,qword ptr [RBX + 0x3c0]
14066db91:  4D 2B D1                 SUB R10,R9
14066db94:  49 C1 FA 04              SAR R10,0x4
14066db98:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
14066dba0:  48 03 C9                 ADD RCX,RCX
14066dba3:  49 8B 04 C9              MOV RAX,qword ptr [R9 + RCX*0x8]
14066dba7:  81 78 40 1A 02 00 00     CMP dword ptr [RAX + 0x40],0x21a
14066dbae:  75 10                    JNZ 0x14066dbc0
14066dbb0:  F3 41 0F 10 44 C9 08     MOVSS XMM0,dword ptr [R9 + RCX*0x8 + 0x8]
14066dbb7:  0F 2F C7                 COMISS XMM0,XMM7
14066dbba:  0F B6 D2                 MOVZX EDX,DL
14066dbbd:  0F 47 D7                 CMOVA EDX,EDI
14066dbc0:  41 FF C0                 INC R8D
14066dbc3:  41 8B C8                 MOV ECX,R8D
14066dbc6:  49 3B CA                 CMP RCX,R10
14066dbc9:  72 D5                    JC 0x14066dba0
14066dbcb:  84 D2                    TEST DL,DL
14066dbcd:  0F 84 18 01 00 00        JZ 0x14066dceb
14066dbd3:  48 8B 8B F0 04 00 00     MOV RCX,qword ptr [RBX + 0x4f0]
14066dbda:  48 85 C9                 TEST RCX,RCX
14066dbdd:  74 0C                    JZ 0x14066dbeb
14066dbdf:  48 81 C1 20 03 00 00     ADD RCX,0x320
14066dbe6:  E9 9F 00 00 00           JMP 0x14066dc8a
14066dbeb:  48 8B 8B A0 04 00 00     MOV RCX,qword ptr [RBX + 0x4a0]
14066dbf2:  48 85 C9                 TEST RCX,RCX
14066dbf5:  0F 84 F0 00 00 00        JZ 0x14066dceb
14066dbfb:  48 8B 81 98 03 00 00     MOV RAX,qword ptr [RCX + 0x398]
14066dc02:  48 85 C0                 TEST RAX,RAX
14066dc05:  74 5A                    JZ 0x14066dc61
14066dc07:  48 8B C8                 MOV RCX,RAX
14066dc0a:  4C 89 7C 24 28           MOV qword ptr [RSP + 0x28],R15
14066dc0f:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066dc13:  0F 28 D7                 MOVAPS XMM2,XMM7
14066dc16:  4C 89 7C 24 20           MOV qword ptr [RSP + 0x20],R15
14066dc1b:  45 33 C9                 XOR R9D,R9D
14066dc1e:  E8 7D 76 03 00           CALL 0x1406a52a0
14066dc23:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14066dc27:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14066dc2a:  F2 0F 11 44 24 40        MOVSD qword ptr [RSP + 0x40],XMM0
14066dc30:  F3 0F 10 44 24 40        MOVSS XMM0,dword ptr [RSP + 0x40]
14066dc36:  0F 2E C7                 UCOMISS XMM0,XMM7
14066dc39:  89 44 24 48              MOV dword ptr [RSP + 0x48],EAX
14066dc3d:  7A 66                    JP 0x14066dca5
14066dc3f:  75 64                    JNZ 0x14066dca5
14066dc41:  F3 0F 10 44 24 44        MOVSS XMM0,dword ptr [RSP + 0x44]
14066dc47:  0F 2E C7                 UCOMISS XMM0,XMM7
14066dc4a:  7A 59                    JP 0x14066dca5
14066dc4c:  75 57                    JNZ 0x14066dca5
14066dc4e:  F3 0F 10 44 24 48        MOVSS XMM0,dword ptr [RSP + 0x48]
14066dc54:  0F 2E C7                 UCOMISS XMM0,XMM7
14066dc57:  7A 4C                    JP 0x14066dca5
14066dc59:  0F 84 8C 00 00 00        JZ 0x14066dceb
14066dc5f:  EB 44                    JMP 0x14066dca5
14066dc61:  48 85 C9                 TEST RCX,RCX
14066dc64:  0F 84 81 00 00 00        JZ 0x14066dceb
14066dc6a:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
14066dc71:  8B 90 94 02 00 00        MOV EDX,dword ptr [RAX + 0x294]
14066dc77:  83 EA 03                 SUB EDX,0x3
14066dc7a:  3B D7                    CMP EDX,EDI
14066dc7c:  76 8C                    JBE 0x14066dc0a
14066dc7e:  48 85 C9                 TEST RCX,RCX
14066dc81:  74 68                    JZ 0x14066dceb
14066dc83:  48 81 C1 60 0D 00 00     ADD RCX,0xd60
14066dc8a:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066dc8e:  FF 15 24 EB 1F 00        CALL qword ptr [0x14086c7b8]
14066dc94:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14066dc98:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14066dc9b:  89 44 24 48              MOV dword ptr [RSP + 0x48],EAX
14066dc9f:  F2 0F 11 44 24 40        MOVSD qword ptr [RSP + 0x40],XMM0
14066dca5:  F3 0F 10 44 24 40        MOVSS XMM0,dword ptr [RSP + 0x40]
14066dcab:  48 8D 55 B7              LEA RDX,[RBP + -0x49]
14066dcaf:  F3 0F 10 4C 24 44        MOVSS XMM1,dword ptr [RSP + 0x44]
14066dcb5:  48 8D 0D 54 72 36 00     LEA RCX,[0x1409d4f10]
14066dcbc:  F3 0F 10 1D 90 C1 29 00  MOVSS XMM3,dword ptr [0x140909e54]
14066dcc4:  F3 0F 10 15 BC BD 29 00  MOVSS XMM2,dword ptr [0x140909a88]
14066dccc:  F3 0F 11 45 B7           MOVSS dword ptr [RBP + -0x49],XMM0
14066dcd1:  F3 0F 10 44 24 48        MOVSS XMM0,dword ptr [RSP + 0x48]
14066dcd7:  F3 0F 11 45 BF           MOVSS dword ptr [RBP + -0x41],XMM0
14066dcdc:  F3 0F 11 4D BB           MOVSS dword ptr [RBP + -0x45],XMM1
14066dce1:  40 88 7C 24 20           MOV byte ptr [RSP + 0x20],DIL
14066dce6:  E8 B5 7A E6 FF           CALL 0x1404d57a0
14066dceb:  83 3D CE 77 36 00 02     CMP dword ptr [0x1409d54c0],0x2
14066dcf2:  49 BD 25 49 92 24 49 92 24 49 MOV R13,0x4924924924924925
14066dcfc:  48 89 B4 24 40 01 00 00  MOV qword ptr [RSP + 0x140],RSI
14066dd04:  0F 85 8F 04 00 00        JNZ 0x14066e199
14066dd0a:  4C 8B 93 08 17 00 00     MOV R10,qword ptr [RBX + 0x1708]
14066dd11:  41 83 BA 94 02 00 00 04  CMP dword ptr [R10 + 0x294],0x4
14066dd19:  0F 85 7A 04 00 00        JNZ 0x14066e199
14066dd1f:  48 8B 83 A8 03 00 00     MOV RAX,qword ptr [RBX + 0x3a8]
14066dd26:  41 8B D7                 MOV EDX,R15D
14066dd29:  48 2B 83 A0 03 00 00     SUB RAX,qword ptr [RBX + 0x3a0]
14066dd30:  F3 41 0F 10 82 84 86 00 00 MOVSS XMM0,dword ptr [R10 + 0x8684]
14066dd39:  F3 0F 10 93 F0 05 00 00  MOVSS XMM2,dword ptr [RBX + 0x5f0]
14066dd41:  48 C1 F8 03              SAR RAX,0x3
14066dd45:  48 85 C0                 TEST RAX,RAX
14066dd48:  74 4E                    JZ 0x14066dd98
14066dd4a:  4C 8B 8B A0 03 00 00     MOV R9,qword ptr [RBX + 0x3a0]
14066dd51:  49 8B C7                 MOV RAX,R15
14066dd54:  4C 8B 83 A8 03 00 00     MOV R8,qword ptr [RBX + 0x3a8]
14066dd5b:  4D 2B C1                 SUB R8,R9
14066dd5e:  49 C1 F8 03              SAR R8,0x3
14066dd62:  49 8B 0C C1              MOV RCX,qword ptr [R9 + RAX*0x8]
14066dd66:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
14066dd6d:  F3 0F 10 88 84 86 00 00  MOVSS XMM1,dword ptr [RAX + 0x8684]
14066dd75:  0F 2F CF                 COMISS XMM1,XMM7
14066dd78:  76 15                    JBE 0x14066dd8f
14066dd7a:  44 39 B9 94 17 00 00     CMP dword ptr [RCX + 0x1794],R15D
14066dd81:  75 0C                    JNZ 0x14066dd8f
14066dd83:  F3 0F 58 91 F0 05 00 00  ADDSS XMM2,dword ptr [RCX + 0x5f0]
14066dd8b:  F3 0F 58 C1              ADDSS XMM0,XMM1
14066dd8f:  FF C2                    INC EDX
14066dd91:  8B C2                    MOV EAX,EDX
14066dd93:  49 3B C0                 CMP RAX,R8
14066dd96:  72 CA                    JC 0x14066dd62
14066dd98:  4D 85 D2                 TEST R10,R10
14066dd9b:  0F 84 AC 00 00 00        JZ 0x14066de4d
14066dda1:  49 8B 8A 80 7C 00 00     MOV RCX,qword ptr [R10 + 0x7c80]
14066dda8:  49 B8 79 78 78 78 78 78 78 78 MOV R8,0x7878787878787879
14066ddb2:  49 2B 8A 78 7C 00 00     SUB RCX,qword ptr [R10 + 0x7c78]
14066ddb9:  49 8B C0                 MOV RAX,R8
14066ddbc:  48 F7 E9                 IMUL RCX
14066ddbf:  48 C1 FA 07              SAR RDX,0x7
14066ddc3:  48 8B C2                 MOV RAX,RDX
14066ddc6:  48 C1 E8 3F              SHR RAX,0x3f
14066ddca:  48 03 D0                 ADD RDX,RAX
14066ddcd:  0F 84 7A 00 00 00        JZ 0x14066de4d
14066ddd3:  48 8B 83 E8 19 00 00     MOV RAX,qword ptr [RBX + 0x19e8]
14066ddda:  48 2B 83 E0 19 00 00     SUB RAX,qword ptr [RBX + 0x19e0]
14066dde1:  48 C1 F8 03              SAR RAX,0x3
14066dde5:  48 85 C0                 TEST RAX,RAX
14066dde8:  74 63                    JZ 0x14066de4d
14066ddea:  48 8B 83 E8 19 00 00     MOV RAX,qword ptr [RBX + 0x19e8]
14066ddf1:  0F 57 C9                 XORPS XMM1,XMM1
14066ddf4:  48 2B 83 E0 19 00 00     SUB RAX,qword ptr [RBX + 0x19e0]
14066ddfb:  F3 0F 10 25 29 D4 29 00  MOVSS XMM4,dword ptr [0x14090b22c]
14066de03:  48 C1 F8 03              SAR RAX,0x3
14066de07:  F3 48 0F 2A C8           CVTSI2SS XMM1,RAX
14066de0c:  48 85 C0                 TEST RAX,RAX
14066de0f:  79 04                    JNS 0x14066de15
14066de11:  F3 0F 58 CC              ADDSS XMM1,XMM4
14066de15:  49 8B 8A 80 7C 00 00     MOV RCX,qword ptr [R10 + 0x7c80]
14066de1c:  49 8B C0                 MOV RAX,R8
14066de1f:  49 2B 8A 78 7C 00 00     SUB RCX,qword ptr [R10 + 0x7c78]
14066de26:  0F 57 DB                 XORPS XMM3,XMM3
14066de29:  48 F7 E9                 IMUL RCX
14066de2c:  48 C1 FA 07              SAR RDX,0x7
14066de30:  48 8B C2                 MOV RAX,RDX
14066de33:  48 C1 E8 3F              SHR RAX,0x3f
14066de37:  48 03 D0                 ADD RDX,RAX
14066de3a:  F3 48 0F 2A DA           CVTSI2SS XMM3,RDX
14066de3f:  79 04                    JNS 0x14066de45
14066de41:  F3 0F 58 DC              ADDSS XMM3,XMM4
14066de45:  F3 0F 5E CB              DIVSS XMM1,XMM3
14066de49:  F3 0F 59 C1              MULSS XMM0,XMM1
14066de4d:  4C 39 BB E8 05 00 00     CMP qword ptr [RBX + 0x5e8],R15
14066de54:  0F 85 3F 03 00 00        JNZ 0x14066e199
14066de5a:  F3 0F 59 05 26 BF 29 00  MULSS XMM0,dword ptr [0x140909d88]
14066de62:  0F 2F C2                 COMISS XMM0,XMM2
14066de65:  0F 86 2E 03 00 00        JBE 0x14066e199
14066de6b:  48 8B 83 98 07 00 00     MOV RAX,qword ptr [RBX + 0x798]
14066de72:  48 85 C0                 TEST RAX,RAX
14066de75:  0F 84 1E 03 00 00        JZ 0x14066e199
14066de7b:  44 38 B8 29 01 00 00     CMP byte ptr [RAX + 0x129],R15B
14066de82:  0F 85 11 03 00 00        JNZ 0x14066e199
14066de88:  48 63 8B 00 07 00 00     MOVSXD RCX,dword ptr [RBX + 0x700]
14066de8f:  85 C9                    TEST ECX,ECX
14066de91:  0F 8E 02 03 00 00        JLE 0x14066e199
14066de97:  48 8B 83 A8 06 00 00     MOV RAX,qword ptr [RBX + 0x6a8]
14066de9e:  48 2B 83 A0 06 00 00     SUB RAX,qword ptr [RBX + 0x6a0]
14066dea5:  48 C1 F8 03              SAR RAX,0x3
14066dea9:  3B C8                    CMP ECX,EAX
14066deab:  0F 8D E8 02 00 00        JGE 0x14066e199
14066deb1:  48 8B 83 A0 06 00 00     MOV RAX,qword ptr [RBX + 0x6a0]
14066deb8:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14066debc:  48 85 D2                 TEST RDX,RDX
14066debf:  0F 84 D4 02 00 00        JZ 0x14066e199
14066dec5:  48 8B 42 78              MOV RAX,qword ptr [RDX + 0x78]
14066dec9:  48 2B 42 70              SUB RAX,qword ptr [RDX + 0x70]
14066decd:  48 C1 F8 03              SAR RAX,0x3
14066ded1:  48 85 C0                 TEST RAX,RAX
14066ded4:  0F 84 BF 02 00 00        JZ 0x14066e199
14066deda:  48 8B 42 70              MOV RAX,qword ptr [RDX + 0x70]
14066dede:  4C 8B 20                 MOV R12,qword ptr [RAX]
14066dee1:  4C 89 65 6F              MOV qword ptr [RBP + 0x6f],R12
14066dee5:  49 8B 84 24 18 03 00 00  MOV RAX,qword ptr [R12 + 0x318]
14066deed:  48 85 C0                 TEST RAX,RAX
14066def0:  0F 84 9C 02 00 00        JZ 0x14066e192
14066def6:  83 B8 60 03 00 00 22     CMP dword ptr [RAX + 0x360],0x22
14066defd:  0F 85 8F 02 00 00        JNZ 0x14066e192
14066df03:  49 8B D4                 MOV RDX,R12
14066df06:  48 8D 0D 03 70 36 00     LEA RCX,[0x1409d4f10]
14066df0d:  E8 CE E0 B4 FF           CALL 0x1401bbfe0
14066df12:  84 C0                    TEST AL,AL
14066df14:  0F 85 78 02 00 00        JNZ 0x14066e192
14066df1a:  4D 85 D2                 TEST R10,R10
14066df1d:  0F 84 EE 00 00 00        JZ 0x14066e011
14066df23:  45 38 BA 81 86 00 00     CMP byte ptr [R10 + 0x8681],R15B
14066df2a:  0F 85 E1 00 00 00        JNZ 0x14066e011
14066df30:  49 8B 8C 24 78 09 00 00  MOV RCX,qword ptr [R12 + 0x978]
14066df38:  49 8B C5                 MOV RAX,R13
14066df3b:  49 2B 8C 24 70 09 00 00  SUB RCX,qword ptr [R12 + 0x970]
14066df43:  41 8B F7                 MOV ESI,R15D
14066df46:  48 F7 E9                 IMUL RCX
14066df49:  48 C1 FA 06              SAR RDX,0x6
14066df4d:  48 8B C2                 MOV RAX,RDX
14066df50:  48 C1 E8 3F              SHR RAX,0x3f
14066df54:  48 03 D0                 ADD RDX,RAX
14066df57:  0F 84 35 02 00 00        JZ 0x14066e192
14066df5d:  4D 8B 94 24 70 09 00 00  MOV R10,qword ptr [R12 + 0x970]
14066df65:  4D 8B CF                 MOV R9,R15
14066df68:  4C 8B 1D 81 32 37 00     MOV R11,qword ptr [0x1409e11f0]
14066df6f:  49 8D 7A 08              LEA RDI,[R10 + 0x8]
14066df73:  0F 1F 40 00              NOP dword ptr [RAX]
14066df77:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14066df80:  48 8B 07                 MOV RAX,qword ptr [RDI]
14066df83:  45 8B C7                 MOV R8D,R15D
14066df86:  4B 2B 04 11              SUB RAX,qword ptr [R9 + R10*0x1]
14066df8a:  48 C1 F8 04              SAR RAX,0x4
14066df8e:  48 85 C0                 TEST RAX,RAX
14066df91:  74 2F                    JZ 0x14066dfc2
14066df93:  4B 8B 14 0A              MOV RDX,qword ptr [R10 + R9*0x1]
14066df97:  4C 3B 1A                 CMP R11,qword ptr [RDX]
14066df9a:  75 0A                    JNZ 0x14066dfa6
14066df9c:  F3 0F 10 42 08           MOVSS XMM0,dword ptr [RDX + 0x8]
14066dfa1:  0F 2F C7                 COMISS XMM0,XMM7
14066dfa4:  77 58                    JA 0x14066dffe
14066dfa6:  4B 8B 4C 0A 08           MOV RCX,qword ptr [R10 + R9*0x1 + 0x8]
14066dfab:  41 FF C0                 INC R8D
14066dfae:  4B 2B 0C 0A              SUB RCX,qword ptr [R10 + R9*0x1]
14066dfb2:  48 83 C2 10              ADD RDX,0x10
14066dfb6:  48 C1 F9 04              SAR RCX,0x4
14066dfba:  49 63 C0                 MOVSXD RAX,R8D
14066dfbd:  48 3B C1                 CMP RAX,RCX
14066dfc0:  72 D5                    JC 0x14066df97
14066dfc2:  49 8B 8C 24 78 09 00 00  MOV RCX,qword ptr [R12 + 0x978]
14066dfca:  FF C6                    INC ESI
14066dfcc:  49 2B CA                 SUB RCX,R10
14066dfcf:  49 8B C5                 MOV RAX,R13
14066dfd2:  48 F7 E9                 IMUL RCX
14066dfd5:  49 81 C1 E0 00 00 00     ADD R9,0xe0
14066dfdc:  48 81 C7 E0 00 00 00     ADD RDI,0xe0
14066dfe3:  48 C1 FA 06              SAR RDX,0x6
14066dfe7:  48 8B C2                 MOV RAX,RDX
14066dfea:  48 C1 E8 3F              SHR RAX,0x3f
14066dfee:  48 03 D0                 ADD RDX,RAX
14066dff1:  48 63 C6                 MOVSXD RAX,ESI
14066dff4:  48 3B C2                 CMP RAX,RDX
14066dff7:  72 87                    JC 0x14066df80
14066dff9:  E9 94 01 00 00           JMP 0x14066e192
14066dffe:  C6 83 3A 0D 00 00 01     MOV byte ptr [RBX + 0xd3a],0x1
14066e005:  4C 89 A3 E8 05 00 00     MOV qword ptr [RBX + 0x5e8],R12
14066e00c:  E9 81 01 00 00           JMP 0x14066e192
14066e011:  49 8B 8C 24 78 09 00 00  MOV RCX,qword ptr [R12 + 0x978]
14066e019:  32 C0                    XOR AL,AL
14066e01b:  0F B6 F0                 MOVZX ESI,AL
14066e01e:  45 32 F6                 XOR R14B,R14B
14066e021:  44 39 3D 9C 74 36 00     CMP dword ptr [0x1409d54c4],R15D
14066e028:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
14066e032:  45 8B EF                 MOV R13D,R15D
14066e035:  0F 44 F7                 CMOVZ ESI,EDI
14066e038:  49 2B 8C 24 70 09 00 00  SUB RCX,qword ptr [R12 + 0x970]
14066e040:  48 F7 E9                 IMUL RCX
14066e043:  48 C1 FA 06              SAR RDX,0x6
14066e047:  48 8B C2                 MOV RAX,RDX
14066e04a:  48 C1 E8 3F              SHR RAX,0x3f
14066e04e:  48 03 D0                 ADD RDX,RAX
14066e051:  0F 84 27 01 00 00        JZ 0x14066e17e
14066e057:  4D 8B 8C 24 70 09 00 00  MOV R9,qword ptr [R12 + 0x970]
14066e05f:  4D 8B C7                 MOV R8,R15
14066e062:  4C 8B 1D 2F 32 37 00     MOV R11,qword ptr [0x1409e1298]
14066e069:  48 8B 3D 88 31 37 00     MOV RDI,qword ptr [0x1409e11f8]
14066e070:  48 8B 5D 6F              MOV RBX,qword ptr [RBP + 0x6f]
14066e074:  4D 8D 61 08              LEA R12,[R9 + 0x8]
14066e078:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
14066e080:  49 8B 04 24              MOV RAX,qword ptr [R12]
14066e084:  45 8B D7                 MOV R10D,R15D
14066e087:  4B 2B 04 08              SUB RAX,qword ptr [R8 + R9*0x1]
14066e08b:  48 C1 F8 04              SAR RAX,0x4
14066e08f:  48 85 C0                 TEST RAX,RAX
14066e092:  74 5D                    JZ 0x14066e0f1
14066e094:  4B 8B 14 08              MOV RDX,qword ptr [R8 + R9*0x1]
14066e098:  48 83 C2 08              ADD RDX,0x8
14066e09c:  0F 1F 40 00              NOP dword ptr [RAX]
14066e0a0:  48 8B 42 F8              MOV RAX,qword ptr [RDX + -0x8]
14066e0a4:  48 3B F8                 CMP RDI,RAX
14066e0a7:  75 09                    JNZ 0x14066e0b2
14066e0a9:  F3 0F 10 02              MOVSS XMM0,dword ptr [RDX]
14066e0ad:  0F 2F C7                 COMISS XMM0,XMM7
14066e0b0:  77 37                    JA 0x14066e0e9
14066e0b2:  4C 3B D8                 CMP R11,RAX
14066e0b5:  75 14                    JNZ 0x14066e0cb
14066e0b7:  43 83 BC 08 88 00 00 00 FF CMP dword ptr [R8 + R9*0x1 + 0x88],-0x1
14066e0c0:  75 09                    JNZ 0x14066e0cb
14066e0c2:  F3 0F 10 02              MOVSS XMM0,dword ptr [RDX]
14066e0c6:  0F 2F C7                 COMISS XMM0,XMM7
14066e0c9:  77 23                    JA 0x14066e0ee
14066e0cb:  4B 8B 4C 08 08           MOV RCX,qword ptr [R8 + R9*0x1 + 0x8]
14066e0d0:  41 FF C2                 INC R10D
14066e0d3:  4B 2B 0C 08              SUB RCX,qword ptr [R8 + R9*0x1]
14066e0d7:  48 83 C2 10              ADD RDX,0x10
14066e0db:  48 C1 F9 04              SAR RCX,0x4
14066e0df:  49 63 C2                 MOVSXD RAX,R10D
14066e0e2:  48 3B C1                 CMP RAX,RCX
14066e0e5:  72 B9                    JC 0x14066e0a0
14066e0e7:  EB 08                    JMP 0x14066e0f1
14066e0e9:  41 B6 01                 MOV R14B,0x1
14066e0ec:  EB 08                    JMP 0x14066e0f6
14066e0ee:  40 B6 01                 MOV SIL,0x1
14066e0f1:  45 84 F6                 TEST R14B,R14B
14066e0f4:  74 05                    JZ 0x14066e0fb
14066e0f6:  40 84 F6                 TEST SIL,SIL
14066e0f9:  75 46                    JNZ 0x14066e141
14066e0fb:  48 8B 8B 78 09 00 00     MOV RCX,qword ptr [RBX + 0x978]
14066e102:  41 FF C5                 INC R13D
14066e105:  48 2B 8B 70 09 00 00     SUB RCX,qword ptr [RBX + 0x970]
14066e10c:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
14066e116:  48 F7 E9                 IMUL RCX
14066e119:  49 81 C0 E0 00 00 00     ADD R8,0xe0
14066e120:  49 81 C4 E0 00 00 00     ADD R12,0xe0
14066e127:  48 C1 FA 06              SAR RDX,0x6
14066e12b:  48 8B C2                 MOV RAX,RDX
14066e12e:  48 C1 E8 3F              SHR RAX,0x3f
14066e132:  48 03 D0                 ADD RDX,RAX
14066e135:  49 63 C5                 MOVSXD RAX,R13D
14066e138:  48 3B C2                 CMP RAX,RDX
14066e13b:  0F 82 3F FF FF FF        JC 0x14066e080
14066e141:  48 8B 5D 67              MOV RBX,qword ptr [RBP + 0x67]
14066e145:  45 84 F6                 TEST R14B,R14B
14066e148:  74 34                    JZ 0x14066e17e
14066e14a:  4C 8D A3 60 0D 00 00     LEA R12,[RBX + 0xd60]
14066e151:  49 BD 25 49 92 24 49 92 24 49 MOV R13,0x4924924924924925
14066e15b:  49 BE AB AA AA AA AA AA AA 2A MOV R14,0x2aaaaaaaaaaaaaab
14066e165:  40 84 F6                 TEST SIL,SIL
14066e168:  74 2F                    JZ 0x14066e199
14066e16a:  48 8B 45 6F              MOV RAX,qword ptr [RBP + 0x6f]
14066e16e:  48 89 83 E8 05 00 00     MOV qword ptr [RBX + 0x5e8],RAX
14066e175:  C6 83 3A 0D 00 00 01     MOV byte ptr [RBX + 0xd3a],0x1
14066e17c:  EB 1B                    JMP 0x14066e199
14066e17e:  49 BE AB AA AA AA AA AA AA 2A MOV R14,0x2aaaaaaaaaaaaaab
14066e188:  49 BD 25 49 92 24 49 92 24 49 MOV R13,0x4924924924924925
14066e192:  4C 8D A3 60 0D 00 00     LEA R12,[RBX + 0xd60]
14066e199:  48 8B CB                 MOV RCX,RBX
14066e19c:  E8 CF 63 04 00           CALL 0x1406b4570
14066e1a1:  48 8B CB                 MOV RCX,RBX
14066e1a4:  E8 37 EC 00 00           CALL 0x14067cde0
14066e1a9:  48 8B CB                 MOV RCX,RBX
14066e1ac:  E8 4F 4A 05 00           CALL 0x1406c2c00
14066e1b1:  8B 83 94 17 00 00        MOV EAX,dword ptr [RBX + 0x1794]
14066e1b7:  83 CF FF                 OR EDI,0xffffffff
14066e1ba:  C6 83 D8 13 00 00 01     MOV byte ptr [RBX + 0x13d8],0x1
14066e1c1:  44 88 BB E8 13 00 00     MOV byte ptr [RBX + 0x13e8],R15B
14066e1c8:  89 BB 14 13 00 00        MOV dword ptr [RBX + 0x1314],EDI
14066e1ce:  83 F8 01                 CMP EAX,0x1
14066e1d1:  7E 05                    JLE 0x14066e1d8
14066e1d3:  8D 78 FE                 LEA EDI,[RAX + -0x2]
14066e1d6:  EB 0C                    JMP 0x14066e1e4
14066e1d8:  83 F8 F6                 CMP EAX,-0xa
14066e1db:  7F 07                    JG 0x14066e1e4
14066e1dd:  BF F6 FF FF FF           MOV EDI,0xfffffff6
14066e1e2:  2B F8                    SUB EDI,EAX
14066e1e4:  4C 8B 9B F0 04 00 00     MOV R11,qword ptr [RBX + 0x4f0]
14066e1eb:  4D 85 DB                 TEST R11,R11
14066e1ee:  0F 84 43 05 00 00        JZ 0x14066e737
14066e1f4:  44 38 BB 14 05 00 00     CMP byte ptr [RBX + 0x514],R15B
14066e1fb:  74 43                    JZ 0x14066e240
14066e1fd:  F3 0F 10 0D 13 C4 29 00  MOVSS XMM1,dword ptr [0x14090a618]
14066e205:  41 B0 01                 MOV R8B,0x1
14066e208:  48 8B CB                 MOV RCX,RBX
14066e20b:  E8 40 59 04 00           CALL 0x1406b3b50
14066e210:  0F 28 DF                 MOVAPS XMM3,XMM7
14066e213:  48 8B CB                 MOV RCX,RBX
14066e216:  E8 95 04 01 00           CALL 0x14067e6b0
14066e21b:  48 8B CB                 MOV RCX,RBX
14066e21e:  44 89 BB 7C 17 00 00     MOV dword ptr [RBX + 0x177c],R15D
14066e225:  44 89 BB 8C 17 00 00     MOV dword ptr [RBX + 0x178c],R15D
14066e22c:  E8 7F A5 FF FF           CALL 0x1406687b0
14066e231:  33 D2                    XOR EDX,EDX
14066e233:  48 8B CB                 MOV RCX,RBX
14066e236:  E8 95 39 05 00           CALL 0x1406c1bd0
14066e23b:  E9 46 0B 00 00           JMP 0x14066ed86
14066e240:  49 8B B3 18 03 00 00     MOV RSI,qword ptr [R11 + 0x318]
14066e247:  48 85 F6                 TEST RSI,RSI
14066e24a:  0F 85 80 01 00 00        JNZ 0x14066e3d0
14066e250:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066e257:  8B 88 94 02 00 00        MOV ECX,dword ptr [RAX + 0x294]
14066e25d:  83 E9 03                 SUB ECX,0x3
14066e260:  83 F9 02                 CMP ECX,0x2
14066e263:  0F 87 97 00 00 00        JA 0x14066e300
14066e269:  49 8B 83 D0 05 00 00     MOV RAX,qword ptr [R11 + 0x5d0]
14066e270:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e274:  0F 28 C7                 MOVAPS XMM0,XMM7
14066e277:  44 89 7C 24 48           MOV dword ptr [RSP + 0x48],R15D
14066e27c:  0F 14 C7                 UNPCKLPS XMM0,XMM7
14066e27f:  0F B6 88 78 01 00 00     MOVZX ECX,byte ptr [RAX + 0x178]
14066e286:  41 8B C7                 MOV EAX,R15D
14066e289:  88 8B D8 13 00 00        MOV byte ptr [RBX + 0x13d8],CL
14066e28f:  49 8B CC                 MOV RCX,R12
14066e292:  F2 0F 11 45 B7           MOVSD qword ptr [RBP + -0x49],XMM0
14066e297:  89 45 BF                 MOV dword ptr [RBP + -0x41],EAX
14066e29a:  44 89 7D C3              MOV dword ptr [RBP + -0x3d],R15D
14066e29e:  FF 15 14 E5 1F 00        CALL qword ptr [0x14086c7b8]
14066e2a4:  48 8D 55 B7              LEA RDX,[RBP + -0x49]
14066e2a8:  48 8D 0D E1 B9 37 00     LEA RCX,[0x1409e9c90]
14066e2af:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14066e2b3:  F2 0F 11 45 B7           MOVSD qword ptr [RBP + -0x49],XMM0
14066e2b8:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14066e2bb:  F3 0F 10 05 2D C7 29 00  MOVSS XMM0,dword ptr [0x14090a9f0]
14066e2c3:  F3 0F 11 45 C3           MOVSS dword ptr [RBP + -0x3d],XMM0
14066e2c8:  89 45 BF                 MOV dword ptr [RBP + -0x41],EAX
14066e2cb:  FF 15 37 ED 1F 00        CALL qword ptr [0x14086d008]
14066e2d1:  0F 2F 05 AC 3D 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e2d8:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e2e0:  76 07                    JBE 0x14066e2e9
14066e2e2:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e2e9:  48 8B CB                 MOV RCX,RBX
14066e2ec:  E8 2F 92 03 00           CALL 0x1406a7520
14066e2f1:  33 D2                    XOR EDX,EDX
14066e2f3:  48 8B CB                 MOV RCX,RBX
14066e2f6:  E8 D5 38 05 00           CALL 0x1406c1bd0
14066e2fb:  E9 86 0A 00 00           JMP 0x14066ed86
14066e300:  48 85 F6                 TEST RSI,RSI
14066e303:  0F 85 C7 00 00 00        JNZ 0x14066e3d0
14066e309:  49 8B 83 D0 05 00 00     MOV RAX,qword ptr [R11 + 0x5d0]
14066e310:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e314:  0F 28 C7                 MOVAPS XMM0,XMM7
14066e317:  44 89 7C 24 48           MOV dword ptr [RSP + 0x48],R15D
14066e31c:  0F 14 C7                 UNPCKLPS XMM0,XMM7
14066e31f:  0F B6 88 78 01 00 00     MOVZX ECX,byte ptr [RAX + 0x178]
14066e326:  41 8B C7                 MOV EAX,R15D
14066e329:  88 8B D8 13 00 00        MOV byte ptr [RBX + 0x13d8],CL
14066e32f:  49 8B CC                 MOV RCX,R12
14066e332:  F2 0F 11 45 B7           MOVSD qword ptr [RBP + -0x49],XMM0
14066e337:  89 45 BF                 MOV dword ptr [RBP + -0x41],EAX
14066e33a:  44 89 7D C3              MOV dword ptr [RBP + -0x3d],R15D
14066e33e:  FF 15 74 E4 1F 00        CALL qword ptr [0x14086c7b8]
14066e344:  48 8D 55 B7              LEA RDX,[RBP + -0x49]
14066e348:  48 8D 0D 41 B9 37 00     LEA RCX,[0x1409e9c90]
14066e34f:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14066e353:  F2 0F 11 45 B7           MOVSD qword ptr [RBP + -0x49],XMM0
14066e358:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14066e35b:  F3 0F 10 05 8D C6 29 00  MOVSS XMM0,dword ptr [0x14090a9f0]
14066e363:  F3 0F 11 45 C3           MOVSS dword ptr [RBP + -0x3d],XMM0
14066e368:  89 45 BF                 MOV dword ptr [RBP + -0x41],EAX
14066e36b:  FF 15 97 EC 1F 00        CALL qword ptr [0x14086d008]
14066e371:  0F 2F 05 0C 3D 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e378:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e380:  76 07                    JBE 0x14066e389
14066e382:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e389:  48 8B 83 F0 04 00 00     MOV RAX,qword ptr [RBX + 0x4f0]
14066e390:  44 0F 2F 90 04 06 00 00  COMISS XMM10,dword ptr [RAX + 0x604]
14066e398:  0F 86 99 03 00 00        JBE 0x14066e737
14066e39e:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066e3a5:  49 8B C6                 MOV RAX,R14
14066e3a8:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066e3af:  48 F7 E9                 IMUL RCX
14066e3b2:  48 D1 FA                 SAR RDX,0x1
14066e3b5:  48 8B C2                 MOV RAX,RDX
14066e3b8:  48 C1 E8 3F              SHR RAX,0x3f
14066e3bc:  48 03 D0                 ADD RDX,RAX
14066e3bf:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066e3c5:  0F 8C 6C 03 00 00        JL 0x14066e737
14066e3cb:  E9 27 02 00 00           JMP 0x14066e5f7
14066e3d0:  F3 0F 10 83 A0 17 00 00  MOVSS XMM0,dword ptr [RBX + 0x17a0]
14066e3d8:  41 0F 2F C1              COMISS XMM0,XMM9
14066e3dc:  77 09                    JA 0x14066e3e7
14066e3de:  45 38 BB 98 11 00 00     CMP byte ptr [R11 + 0x1198],R15B
14066e3e5:  74 73                    JZ 0x14066e45a
14066e3e7:  48 8B CB                 MOV RCX,RBX
14066e3ea:  E8 71 D3 FF FF           CALL 0x14066b760
14066e3ef:  84 C0                    TEST AL,AL
14066e3f1:  74 67                    JZ 0x14066e45a
14066e3f3:  4C 39 9B F8 04 00 00     CMP qword ptr [RBX + 0x4f8],R11
14066e3fa:  74 5E                    JZ 0x14066e45a
14066e3fc:  41 0F B6 83 5A 11 00 00  MOVZX EAX,byte ptr [R11 + 0x115a]
14066e404:  49 8D 8B 20 03 00 00     LEA RCX,[R11 + 0x320]
14066e40b:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e40f:  88 83 D8 13 00 00        MOV byte ptr [RBX + 0x13d8],AL
14066e415:  FF 15 9D E3 1F 00        CALL qword ptr [0x14086c7b8]
14066e41b:  48 8B D0                 MOV RDX,RAX
14066e41e:  48 8D 0D 6B B8 37 00     LEA RCX,[0x1409e9c90]
14066e425:  FF 15 DD EB 1F 00        CALL qword ptr [0x14086d008]
14066e42b:  0F 2F 05 52 3C 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e432:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e43a:  76 07                    JBE 0x14066e443
14066e43c:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e443:  48 8B CB                 MOV RCX,RBX
14066e446:  E8 45 DF FF FF           CALL 0x14066c390
14066e44b:  33 D2                    XOR EDX,EDX
14066e44d:  48 8B CB                 MOV RCX,RBX
14066e450:  E8 7B 37 05 00           CALL 0x1406c1bd0
14066e455:  E9 2C 09 00 00           JMP 0x14066ed86
14066e45a:  44 8B B6 60 03 00 00     MOV R14D,dword ptr [RSI + 0x360]
14066e461:  41 83 FE 08              CMP R14D,0x8
14066e465:  0F 85 8E 00 00 00        JNZ 0x14066e4f9
14066e46b:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066e472:  48 B8 AB AA AA AA AA AA AA 2A MOV RAX,0x2aaaaaaaaaaaaaab
14066e47c:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066e483:  48 F7 E9                 IMUL RCX
14066e486:  48 D1 FA                 SAR RDX,0x1
14066e489:  48 8B C2                 MOV RAX,RDX
14066e48c:  48 C1 E8 3F              SHR RAX,0x3f
14066e490:  48 03 D0                 ADD RDX,RAX
14066e493:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066e499:  7C 5E                    JL 0x14066e4f9
14066e49b:  41 0F B6 83 5A 11 00 00  MOVZX EAX,byte ptr [R11 + 0x115a]
14066e4a3:  49 8D 8B 20 03 00 00     LEA RCX,[R11 + 0x320]
14066e4aa:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e4ae:  88 83 D8 13 00 00        MOV byte ptr [RBX + 0x13d8],AL
14066e4b4:  FF 15 FE E2 1F 00        CALL qword ptr [0x14086c7b8]
14066e4ba:  48 8B D0                 MOV RDX,RAX
14066e4bd:  48 8D 0D CC B7 37 00     LEA RCX,[0x1409e9c90]
14066e4c4:  FF 15 3E EB 1F 00        CALL qword ptr [0x14086d008]
14066e4ca:  0F 2F 05 B3 3B 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e4d1:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e4d9:  76 07                    JBE 0x14066e4e2
14066e4db:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e4e2:  48 8B CB                 MOV RCX,RBX
14066e4e5:  E8 F6 C8 FF FF           CALL 0x14066ade0
14066e4ea:  33 D2                    XOR EDX,EDX
14066e4ec:  48 8B CB                 MOV RCX,RBX
14066e4ef:  E8 DC 36 05 00           CALL 0x1406c1bd0
14066e4f4:  E9 8D 08 00 00           JMP 0x14066ed86
14066e4f9:  45 0F 2F 93 04 06 00 00  COMISS XMM10,dword ptr [R11 + 0x604]
14066e501:  77 6D                    JA 0x14066e570
14066e503:  44 39 3D C2 6F 36 00     CMP dword ptr [0x1409d54cc],R15D
14066e50a:  7E 24                    JLE 0x14066e530
14066e50c:  49 8B D3                 MOV RDX,R11
14066e50f:  48 8D 0D FA 69 36 00     LEA RCX,[0x1409d4f10]
14066e516:  E8 45 B8 B3 FF           CALL 0x1401a9d60
14066e51b:  84 C0                    TEST AL,AL
14066e51d:  74 11                    JZ 0x14066e530
14066e51f:  49 8B D3                 MOV RDX,R11
14066e522:  48 8D 0D E7 69 36 00     LEA RCX,[0x1409d4f10]
14066e529:  E8 F2 BB B3 FF           CALL 0x1401aa120
14066e52e:  EB 09                    JMP 0x14066e539
14066e530:  F3 41 0F 10 83 04 06 00 00 MOVSS XMM0,dword ptr [R11 + 0x604]
14066e539:  44 0F 2F D0              COMISS XMM10,XMM0
14066e53d:  0F 86 CB 00 00 00        JBE 0x14066e60e
14066e543:  48 8B 83 F8 04 00 00     MOV RAX,qword ptr [RBX + 0x4f8]
14066e54a:  48 85 C0                 TEST RAX,RAX
14066e54d:  0F 84 BB 00 00 00        JZ 0x14066e60e
14066e553:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14066e55a:  48 85 C9                 TEST RCX,RCX
14066e55d:  0F 84 AB 00 00 00        JZ 0x14066e60e
14066e563:  83 B9 60 03 00 00 0C     CMP dword ptr [RCX + 0x360],0xc
14066e56a:  0F 85 9E 00 00 00        JNZ 0x14066e60e
14066e570:  49 B8 AB AA AA AA AA AA AA 2A MOV R8,0x2aaaaaaaaaaaaaab
14066e57a:  4C 3B 9B F8 04 00 00     CMP R11,qword ptr [RBX + 0x4f8]
14066e581:  0F 84 91 00 00 00        JZ 0x14066e618
14066e587:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066e58e:  49 8B C0                 MOV RAX,R8
14066e591:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066e598:  48 F7 E9                 IMUL RCX
14066e59b:  48 D1 FA                 SAR RDX,0x1
14066e59e:  48 8B C2                 MOV RAX,RDX
14066e5a1:  48 C1 E8 3F              SHR RAX,0x3f
14066e5a5:  48 03 D0                 ADD RDX,RAX
14066e5a8:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066e5ae:  7C 68                    JL 0x14066e618
14066e5b0:  41 0F B6 83 5A 11 00 00  MOVZX EAX,byte ptr [R11 + 0x115a]
14066e5b8:  49 8D 8B 20 03 00 00     LEA RCX,[R11 + 0x320]
14066e5bf:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e5c3:  88 83 D8 13 00 00        MOV byte ptr [RBX + 0x13d8],AL
14066e5c9:  FF 15 E9 E1 1F 00        CALL qword ptr [0x14086c7b8]
14066e5cf:  48 8B D0                 MOV RDX,RAX
14066e5d2:  48 8D 0D B7 B6 37 00     LEA RCX,[0x1409e9c90]
14066e5d9:  FF 15 29 EA 1F 00        CALL qword ptr [0x14086d008]
14066e5df:  0F 2F 05 9E 3A 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e5e6:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e5ee:  76 07                    JBE 0x14066e5f7
14066e5f0:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e5f7:  48 8B CB                 MOV RCX,RBX
14066e5fa:  E8 A1 A6 FF FF           CALL 0x140668ca0
14066e5ff:  33 D2                    XOR EDX,EDX
14066e601:  48 8B CB                 MOV RCX,RBX
14066e604:  E8 C7 35 05 00           CALL 0x1406c1bd0
14066e609:  E9 78 07 00 00           JMP 0x14066ed86
14066e60e:  49 B8 AB AA AA AA AA AA AA 2A MOV R8,0x2aaaaaaaaaaaaaab
14066e618:  41 83 FE 07              CMP R14D,0x7
14066e61c:  0F 85 A4 00 00 00        JNZ 0x14066e6c6
14066e622:  8B 86 68 03 00 00        MOV EAX,dword ptr [RSI + 0x368]
14066e628:  83 E8 03                 SUB EAX,0x3
14066e62b:  A9 FB FF FF FF           TEST EAX,0xfffffffb
14066e630:  0F 85 90 00 00 00        JNZ 0x14066e6c6
14066e636:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066e63d:  49 8B C0                 MOV RAX,R8
14066e640:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066e647:  48 F7 E9                 IMUL RCX
14066e64a:  48 D1 FA                 SAR RDX,0x1
14066e64d:  48 8B C2                 MOV RAX,RDX
14066e650:  48 C1 E8 3F              SHR RAX,0x3f
14066e654:  48 03 D0                 ADD RDX,RAX
14066e657:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066e65d:  7C 67                    JL 0x14066e6c6
14066e65f:  4C 39 9B E8 05 00 00     CMP qword ptr [RBX + 0x5e8],R11
14066e666:  74 5E                    JZ 0x14066e6c6
14066e668:  41 0F B6 83 5A 11 00 00  MOVZX EAX,byte ptr [R11 + 0x115a]
14066e670:  49 8D 8B 20 03 00 00     LEA RCX,[R11 + 0x320]
14066e677:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e67b:  88 83 D8 13 00 00        MOV byte ptr [RBX + 0x13d8],AL
14066e681:  FF 15 31 E1 1F 00        CALL qword ptr [0x14086c7b8]
14066e687:  48 8B D0                 MOV RDX,RAX
14066e68a:  48 8D 0D FF B5 37 00     LEA RCX,[0x1409e9c90]
14066e691:  FF 15 71 E9 1F 00        CALL qword ptr [0x14086d008]
14066e697:  0F 2F 05 E6 39 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e69e:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e6a6:  76 07                    JBE 0x14066e6af
14066e6a8:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e6af:  48 8B CB                 MOV RCX,RBX
14066e6b2:  E8 99 9B FF FF           CALL 0x140668250
14066e6b7:  33 D2                    XOR EDX,EDX
14066e6b9:  48 8B CB                 MOV RCX,RBX
14066e6bc:  E8 0F 35 05 00           CALL 0x1406c1bd0
14066e6c1:  E9 C0 06 00 00           JMP 0x14066ed86
14066e6c6:  4C 8B 83 F8 04 00 00     MOV R8,qword ptr [RBX + 0x4f8]
14066e6cd:  4D 85 C0                 TEST R8,R8
14066e6d0:  74 65                    JZ 0x14066e737
14066e6d2:  49 8B 88 18 03 00 00     MOV RCX,qword ptr [R8 + 0x318]
14066e6d9:  48 85 C9                 TEST RCX,RCX
14066e6dc:  74 59                    JZ 0x14066e737
14066e6de:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
14066e6e5:  75 50                    JNZ 0x14066e737
14066e6e7:  85 FF                    TEST EDI,EDI
14066e6e9:  78 4C                    JS 0x14066e737
14066e6eb:  49 8B 88 78 09 00 00     MOV RCX,qword ptr [R8 + 0x978]
14066e6f2:  49 8B C5                 MOV RAX,R13
14066e6f5:  49 2B 88 70 09 00 00     SUB RCX,qword ptr [R8 + 0x970]
14066e6fc:  48 F7 E9                 IMUL RCX
14066e6ff:  4C 63 CF                 MOVSXD R9,EDI
14066e702:  48 C1 FA 06              SAR RDX,0x6
14066e706:  48 8B C2                 MOV RAX,RDX
14066e709:  48 C1 E8 3F              SHR RAX,0x3f
14066e70d:  48 03 D0                 ADD RDX,RAX
14066e710:  4C 3B CA                 CMP R9,RDX
14066e713:  73 22                    JNC 0x14066e737
14066e715:  49 8B 80 70 09 00 00     MOV RAX,qword ptr [R8 + 0x970]
14066e71c:  49 69 C9 E0 00 00 00     IMUL RCX,R9,0xe0
14066e723:  81 BC 01 88 00 00 00 C8 00 00 00 CMP dword ptr [RCX + RAX*0x1 + 0x88],0xc8
14066e72e:  75 07                    JNZ 0x14066e737
14066e730:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e737:  8B 83 94 17 00 00        MOV EAX,dword ptr [RBX + 0x1794]
14066e73d:  83 C0 02                 ADD EAX,0x2
14066e740:  83 F8 01                 CMP EAX,0x1
14066e743:  77 07                    JA 0x14066e74c
14066e745:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e74c:  0F B6 83 D8 13 00 00     MOVZX EAX,byte ptr [RBX + 0x13d8]
14066e753:  48 8D 55 97              LEA RDX,[RBP + -0x69]
14066e757:  0F 28 C7                 MOVAPS XMM0,XMM7
14066e75a:  88 83 DA 13 00 00        MOV byte ptr [RBX + 0x13da],AL
14066e760:  41 8B C7                 MOV EAX,R15D
14066e763:  0F 14 C7                 UNPCKLPS XMM0,XMM7
14066e766:  49 8B CC                 MOV RCX,R12
14066e769:  89 45 AF                 MOV dword ptr [RBP + -0x51],EAX
14066e76c:  44 89 7C 24 48           MOV dword ptr [RSP + 0x48],R15D
14066e771:  F2 0F 11 45 A7           MOVSD qword ptr [RBP + -0x59],XMM0
14066e776:  44 89 7D B3              MOV dword ptr [RBP + -0x4d],R15D
14066e77a:  FF 15 38 E0 1F 00        CALL qword ptr [0x14086c7b8]
14066e780:  48 8B 8B 08 17 00 00     MOV RCX,qword ptr [RBX + 0x1708]
14066e787:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14066e78b:  F2 0F 11 45 A7           MOVSD qword ptr [RBP + -0x59],XMM0
14066e790:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14066e793:  F3 0F 10 05 8D C1 29 00  MOVSS XMM0,dword ptr [0x14090a928]
14066e79b:  89 45 AF                 MOV dword ptr [RBP + -0x51],EAX
14066e79e:  F3 0F 11 45 B3           MOVSS dword ptr [RBP + -0x4d],XMM0
14066e7a3:  8B 81 94 02 00 00        MOV EAX,dword ptr [RCX + 0x294]
14066e7a9:  83 E8 06                 SUB EAX,0x6
14066e7ac:  A9 FD FF FF FF           TEST EAX,0xfffffffd
14066e7b1:  75 5B                    JNZ 0x14066e80e
14066e7b3:  F2 0F 10 81 10 03 00 00  MOVSD XMM0,qword ptr [RCX + 0x310]
14066e7bb:  F2 0F 11 45 B7           MOVSD qword ptr [RBP + -0x49],XMM0
14066e7c0:  8B 81 18 03 00 00        MOV EAX,dword ptr [RCX + 0x318]
14066e7c6:  89 45 BF                 MOV dword ptr [RBP + -0x41],EAX
14066e7c9:  F2 0F 10 81 1C 03 00 00  MOVSD XMM0,qword ptr [RCX + 0x31c]
14066e7d1:  F2 0F 11 45 C3           MOVSD qword ptr [RBP + -0x3d],XMM0
14066e7d6:  8B 81 24 03 00 00        MOV EAX,dword ptr [RCX + 0x324]
14066e7dc:  48 8D 4D B7              LEA RCX,[RBP + -0x49]
14066e7e0:  89 45 CB                 MOV dword ptr [RBP + -0x35],EAX
14066e7e3:  FF 15 67 E3 1F 00        CALL qword ptr [0x14086cb50]
14066e7e9:  48 8D 4D C3              LEA RCX,[RBP + -0x3d]
14066e7ed:  0F 28 F0                 MOVAPS XMM6,XMM0
14066e7f0:  FF 15 5A E3 1F 00        CALL qword ptr [0x14086cb50]
14066e7f6:  0F 2F F0                 COMISS XMM6,XMM0
14066e7f9:  48 8D 4D B7              LEA RCX,[RBP + -0x49]
14066e7fd:  77 04                    JA 0x14066e803
14066e7ff:  48 8D 4D C3              LEA RCX,[RBP + -0x3d]
14066e803:  FF 15 47 E3 1F 00        CALL qword ptr [0x14086cb50]
14066e809:  F3 0F 11 45 B3           MOVSS dword ptr [RBP + -0x4d],XMM0
14066e80e:  48 8B 05 13 67 36 00     MOV RAX,qword ptr [0x1409d4f28]
14066e815:  48 85 C0                 TEST RAX,RAX
14066e818:  74 28                    JZ 0x14066e842
14066e81a:  48 39 58 70              CMP qword ptr [RAX + 0x70],RBX
14066e81e:  75 22                    JNZ 0x14066e842
14066e820:  48 8D 55 A7              LEA RDX,[RBP + -0x59]
14066e824:  C6 83 D8 13 00 00 01     MOV byte ptr [RBX + 0x13d8],0x1
14066e82b:  48 8D 0D 5E B4 37 00     LEA RCX,[0x1409e9c90]
14066e832:  FF 15 D0 E7 1F 00        CALL qword ptr [0x14086d008]
14066e838:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e840:  EB 3E                    JMP 0x14066e880
14066e842:  48 8D 55 A7              LEA RDX,[RBP + -0x59]
14066e846:  48 8D 0D 43 B4 37 00     LEA RCX,[0x1409e9c90]
14066e84d:  FF 15 DD EB 1F 00        CALL qword ptr [0x14086d430]
14066e853:  84 C0                    TEST AL,AL
14066e855:  74 22                    JZ 0x14066e879
14066e857:  48 8D 55 A7              LEA RDX,[RBP + -0x59]
14066e85b:  48 8D 0D 2E B4 37 00     LEA RCX,[0x1409e9c90]
14066e862:  FF 15 A0 E7 1F 00        CALL qword ptr [0x14086d008]
14066e868:  0F 2F 05 15 38 32 00     COMISS XMM0,dword ptr [0x140992084]
14066e86f:  F3 0F 11 83 DC 13 00 00  MOVSS dword ptr [RBX + 0x13dc],XMM0
14066e877:  76 07                    JBE 0x14066e880
14066e879:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e880:  48 8B 8B A0 04 00 00     MOV RCX,qword ptr [RBX + 0x4a0]
14066e887:  48 85 C9                 TEST RCX,RCX
14066e88a:  0F 85 E3 01 00 00        JNZ 0x14066ea73
14066e890:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066e897:  8B 88 94 02 00 00        MOV ECX,dword ptr [RAX + 0x294]
14066e89d:  8D 41 FF                 LEA EAX,[RCX + -0x1]
14066e8a0:  83 F8 01                 CMP EAX,0x1
14066e8a3:  0F 86 BD 01 00 00        JBE 0x14066ea66
14066e8a9:  44 38 BB 7D 06 00 00     CMP byte ptr [RBX + 0x67d],R15B
14066e8b0:  0F 85 B0 01 00 00        JNZ 0x14066ea66
14066e8b6:  83 F9 06                 CMP ECX,0x6
14066e8b9:  75 0D                    JNZ 0x14066e8c8
14066e8bb:  48 8B CB                 MOV RCX,RBX
14066e8be:  E8 DD FB 06 00           CALL 0x1406de4a0
14066e8c3:  E9 3A 02 00 00           JMP 0x14066eb02
14066e8c8:  83 F9 08                 CMP ECX,0x8
14066e8cb:  75 0D                    JNZ 0x14066e8da
14066e8cd:  48 8B CB                 MOV RCX,RBX
14066e8d0:  E8 1B E5 05 00           CALL 0x1406ccdf0
14066e8d5:  E9 28 02 00 00           JMP 0x14066eb02
14066e8da:  83 F9 0A                 CMP ECX,0xa
14066e8dd:  75 0D                    JNZ 0x14066e8ec
14066e8df:  48 8B CB                 MOV RCX,RBX
14066e8e2:  E8 59 BA 06 00           CALL 0x1406da340
14066e8e7:  E9 16 02 00 00           JMP 0x14066eb02
14066e8ec:  8D 41 FD                 LEA EAX,[RCX + -0x3]
14066e8ef:  83 F8 02                 CMP EAX,0x2
14066e8f2:  0F 87 0A 02 00 00        JA 0x14066eb02
14066e8f8:  32 C0                    XOR AL,AL
14066e8fa:  4C 39 BB 98 03 00 00     CMP qword ptr [RBX + 0x398],R15
14066e901:  74 3C                    JZ 0x14066e93f
14066e903:  44 39 BB 94 17 00 00     CMP dword ptr [RBX + 0x1794],R15D
14066e90a:  74 33                    JZ 0x14066e93f
14066e90c:  48 8B 8B F0 04 00 00     MOV RCX,qword ptr [RBX + 0x4f0]
14066e913:  48 85 C9                 TEST RCX,RCX
14066e916:  74 27                    JZ 0x14066e93f
14066e918:  83 BB C0 04 00 00 FF     CMP dword ptr [RBX + 0x4c0],-0x1
14066e91f:  75 1E                    JNZ 0x14066e93f
14066e921:  48 8B 91 18 03 00 00     MOV RDX,qword ptr [RCX + 0x318]
14066e928:  48 85 D2                 TEST RDX,RDX
14066e92b:  74 12                    JZ 0x14066e93f
14066e92d:  83 BA 60 03 00 00 34     CMP dword ptr [RDX + 0x360],0x34
14066e934:  B9 01 00 00 00           MOV ECX,0x1
14066e939:  0F B6 C0                 MOVZX EAX,AL
14066e93c:  0F 44 C1                 CMOVZ EAX,ECX
14066e93f:  48 8B 8B F0 04 00 00     MOV RCX,qword ptr [RBX + 0x4f0]
14066e946:  48 85 C9                 TEST RCX,RCX
14066e949:  0F 84 0A 01 00 00        JZ 0x14066ea59
14066e94f:  44 39 BB 94 17 00 00     CMP dword ptr [RBX + 0x1794],R15D
14066e956:  74 08                    JZ 0x14066e960
14066e958:  84 C0                    TEST AL,AL
14066e95a:  0F 84 A2 01 00 00        JZ 0x14066eb02
14066e960:  48 85 C9                 TEST RCX,RCX
14066e963:  0F 84 F0 00 00 00        JZ 0x14066ea59
14066e969:  4C 39 BB E8 05 00 00     CMP qword ptr [RBX + 0x5e8],R15
14066e970:  74 5D                    JZ 0x14066e9cf
14066e972:  4C 39 BB 28 05 00 00     CMP qword ptr [RBX + 0x528],R15
14066e979:  74 54                    JZ 0x14066e9cf
14066e97b:  44 88 7C 24 38           MOV byte ptr [RSP + 0x38],R15B
14066e980:  45 33 C9                 XOR R9D,R9D
14066e983:  44 88 7C 24 30           MOV byte ptr [RSP + 0x30],R15B
14066e988:  45 33 C0                 XOR R8D,R8D
14066e98b:  33 D2                    XOR EDX,EDX
14066e98d:  44 88 7C 24 20           MOV byte ptr [RSP + 0x20],R15B
14066e992:  48 8B CB                 MOV RCX,RBX
14066e995:  E8 96 C9 01 00           CALL 0x14068b330
14066e99a:  84 C0                    TEST AL,AL
14066e99c:  74 31                    JZ 0x14066e9cf
14066e99e:  4C 89 BB E8 05 00 00     MOV qword ptr [RBX + 0x5e8],R15
14066e9a5:  4C 89 BB 28 05 00 00     MOV qword ptr [RBX + 0x528],R15
14066e9ac:  4C 39 BB F8 04 00 00     CMP qword ptr [RBX + 0x4f8],R15
14066e9b3:  74 1A                    JZ 0x14066e9cf
14066e9b5:  48 8B 83 80 06 00 00     MOV RAX,qword ptr [RBX + 0x680]
14066e9bc:  48 8D 8B 38 06 00 00     LEA RCX,[RBX + 0x638]
14066e9c3:  48 89 83 88 06 00 00     MOV qword ptr [RBX + 0x688],RAX
14066e9ca:  E8 D1 0D B9 FF           CALL 0x1401ff7a0
14066e9cf:  48 8B CB                 MOV RCX,RBX
14066e9d2:  E8 F9 D9 BE FF           CALL 0x14025c3d0
14066e9d7:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066e9de:  44 88 BB D8 13 00 00     MOV byte ptr [RBX + 0x13d8],R15B
14066e9e5:  4C 89 BB 78 17 00 00     MOV qword ptr [RBX + 0x1778],R15
14066e9ec:  4C 89 BB 80 17 00 00     MOV qword ptr [RBX + 0x1780],R15
14066e9f3:  44 89 BB 8C 17 00 00     MOV dword ptr [RBX + 0x178c],R15D
14066e9fa:  83 B8 94 02 00 00 03     CMP dword ptr [RAX + 0x294],0x3
14066ea01:  75 24                    JNZ 0x14066ea27
14066ea03:  48 8B 83 98 03 00 00     MOV RAX,qword ptr [RBX + 0x398]
14066ea0a:  48 85 C0                 TEST RAX,RAX
14066ea0d:  74 18                    JZ 0x14066ea27
14066ea0f:  44 38 B8 D8 13 00 00     CMP byte ptr [RAX + 0x13d8],R15B
14066ea16:  74 0F                    JZ 0x14066ea27
14066ea18:  48 8B CB                 MOV RCX,RBX
14066ea1b:  C6 83 D8 13 00 00 01     MOV byte ptr [RBX + 0x13d8],0x1
14066ea22:  E8 F9 8A 03 00           CALL 0x1406a7520
14066ea27:  48 8B 83 F0 04 00 00     MOV RAX,qword ptr [RBX + 0x4f0]
14066ea2e:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14066ea35:  48 85 C9                 TEST RCX,RCX
14066ea38:  74 10                    JZ 0x14066ea4a
14066ea3a:  83 B9 60 03 00 00 6D     CMP dword ptr [RCX + 0x360],0x6d
14066ea41:  75 07                    JNZ 0x14066ea4a
14066ea43:  C6 83 D8 13 00 00 01     MOV byte ptr [RBX + 0x13d8],0x1
14066ea4a:  33 D2                    XOR EDX,EDX
14066ea4c:  48 8B CB                 MOV RCX,RBX
14066ea4f:  E8 7C 31 05 00           CALL 0x1406c1bd0
14066ea54:  E9 2D 03 00 00           JMP 0x14066ed86
14066ea59:  48 8B CB                 MOV RCX,RBX
14066ea5c:  E8 BF 8A 03 00           CALL 0x1406a7520
14066ea61:  E9 9C 00 00 00           JMP 0x14066eb02
14066ea66:  48 8B CB                 MOV RCX,RBX
14066ea69:  E8 B2 A9 02 00           CALL 0x140699420
14066ea6e:  E9 8F 00 00 00           JMP 0x14066eb02
14066ea73:  48 81 C1 60 0D 00 00     ADD RCX,0xd60
14066ea7a:  49 8D 94 24 80 00 00 00  LEA RDX,[R12 + 0x80]
14066ea82:  0F 10 01                 MOVUPS XMM0,xmmword ptr [RCX]
14066ea85:  41 0F 11 04 24           MOVUPS xmmword ptr [R12],XMM0
14066ea8a:  0F 10 49 10              MOVUPS XMM1,xmmword ptr [RCX + 0x10]
14066ea8e:  41 0F 11 4C 24 10        MOVUPS xmmword ptr [R12 + 0x10],XMM1
14066ea94:  0F 10 41 20              MOVUPS XMM0,xmmword ptr [RCX + 0x20]
14066ea98:  41 0F 11 44 24 20        MOVUPS xmmword ptr [R12 + 0x20],XMM0
14066ea9e:  0F 10 49 30              MOVUPS XMM1,xmmword ptr [RCX + 0x30]
14066eaa2:  41 0F 11 4C 24 30        MOVUPS xmmword ptr [R12 + 0x30],XMM1
14066eaa8:  0F 10 41 40              MOVUPS XMM0,xmmword ptr [RCX + 0x40]
14066eaac:  41 0F 11 44 24 40        MOVUPS xmmword ptr [R12 + 0x40],XMM0
14066eab2:  0F 10 49 50              MOVUPS XMM1,xmmword ptr [RCX + 0x50]
14066eab6:  41 0F 11 4C 24 50        MOVUPS xmmword ptr [R12 + 0x50],XMM1
14066eabc:  0F 10 41 60              MOVUPS XMM0,xmmword ptr [RCX + 0x60]
14066eac0:  41 0F 11 44 24 60        MOVUPS xmmword ptr [R12 + 0x60],XMM0
14066eac6:  0F 10 41 70              MOVUPS XMM0,xmmword ptr [RCX + 0x70]
14066eaca:  0F 11 42 F0              MOVUPS xmmword ptr [RDX + -0x10],XMM0
14066eace:  0F 10 89 80 00 00 00     MOVUPS XMM1,xmmword ptr [RCX + 0x80]
14066ead5:  0F 11 0A                 MOVUPS xmmword ptr [RDX],XMM1
14066ead8:  0F 10 81 90 00 00 00     MOVUPS XMM0,xmmword ptr [RCX + 0x90]
14066eadf:  0F 11 42 10              MOVUPS xmmword ptr [RDX + 0x10],XMM0
14066eae3:  0F 10 89 A0 00 00 00     MOVUPS XMM1,xmmword ptr [RCX + 0xa0]
14066eaea:  0F 11 4A 20              MOVUPS xmmword ptr [RDX + 0x20],XMM1
14066eaee:  48 8B 81 B0 00 00 00     MOV RAX,qword ptr [RCX + 0xb0]
14066eaf5:  48 89 42 30              MOV qword ptr [RDX + 0x30],RAX
14066eaf9:  8B 81 B8 00 00 00        MOV EAX,dword ptr [RCX + 0xb8]
14066eaff:  89 42 38                 MOV dword ptr [RDX + 0x38],EAX

