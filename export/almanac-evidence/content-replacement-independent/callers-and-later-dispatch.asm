=== export/140373ea0_1403829fd_disasm.asm: 1403745bf..14037466b ===
1403745bf:  F3 0F 10 0D C1 DA 61 00  MOVSS XMM1,dword ptr [0x140992088]
1403745c7:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1403745cc:  F3 44 0F 58 D1           ADDSS XMM10,XMM1
1403745d1:  0F 28 05 E8 7E 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1403745d8:  F3 44 0F 11 54 24 70     MOVSS dword ptr [RSP + 0x70],XMM10
1403745df:  0F 11 05 26 9D 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1403745e6:  F3 44 0F 11 7D B4        MOVSS dword ptr [RBP + -0x4c],XMM15
1403745ec:  C6 44 24 7D 00           MOV byte ptr [RSP + 0x7d],0x0
1403745f1:  83 BE 5C C6 00 00 01     CMP dword ptr [RSI + 0xc65c],0x1
1403745f8:  77 21                    JA 0x14037461b
1403745fa:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
1403745ff:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140374603:  48 8B CE                 MOV RCX,RSI
140374606:  E8 D5 AD FE FF           CALL 0x14035f3e0
14037460b:  F3 44 0F 10 54 24 70     MOVSS XMM10,dword ptr [RSP + 0x70]
140374612:  84 C0                    TEST AL,AL
140374614:  C6 44 24 40 00           MOV byte ptr [RSP + 0x40],0x0
140374619:  75 05                    JNZ 0x140374620
14037461b:  C6 44 24 40 01           MOV byte ptr [RSP + 0x40],0x1
140374620:  44 8B AE DC C7 00 00     MOV R13D,dword ptr [RSI + 0xc7dc]
140374627:  32 C0                    XOR AL,AL
140374629:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14037462c:  45 0F 28 E8              MOVAPS XMM13,XMM8
140374630:  45 0F 28 F0              MOVAPS XMM14,XMM8
140374634:  F3 44 0F 11 45 C0        MOVSS dword ptr [RBP + -0x40],XMM8
14037463a:  F3 44 0F 11 45 F0        MOVSS dword ptr [RBP + -0x10],XMM8
140374640:  F3 44 0F 11 45 C8        MOVSS dword ptr [RBP + -0x38],XMM8
140374646:  F3 44 0F 11 45 F8        MOVSS dword ptr [RBP + -0x8],XMM8
14037464c:  0F 57 C0                 XORPS XMM0,XMM0
14037464f:  F3 0F 7F 45 90           MOVDQU xmmword ptr [RBP + -0x70],XMM0
140374654:  4C 89 75 A0              MOV qword ptr [RBP + -0x60],R14
140374658:  0F 57 C9                 XORPS XMM1,XMM1
14037465b:  F3 0F 7F 4D 78           MOVDQU xmmword ptr [RBP + 0x78],XMM1
140374660:  4C 89 B5 88 00 00 00     MOV qword ptr [RBP + 0x88],R14
140374667:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037466b:  E8 40 64 CD FF           CALL 0x14004aab0
=== export/140373ea0_1403829fd_disasm.asm: 140374a69..140374bc4 ===
140374a69:  48 8D 96 A8 08 01 00     LEA RDX,[RSI + 0x108a8]
140374a70:  49 8B CC                 MOV RCX,R12
140374a73:  E8 D8 0B D7 FF           CALL 0x1400e5650
140374a78:  8B 8E 5C C6 00 00        MOV ECX,dword ptr [RSI + 0xc65c]
140374a7e:  8D 41 FD                 LEA EAX,[RCX + -0x3]
140374a81:  83 F8 01                 CMP EAX,0x1
140374a84:  77 50                    JA 0x140374ad6
140374a86:  F3 0F 10 15 FA D5 61 00  MOVSS XMM2,dword ptr [0x140992088]
140374a8e:  0F 28 CA                 MOVAPS XMM1,XMM2
140374a91:  F3 0F 59 0D CF 5E 59 00  MULSS XMM1,dword ptr [0x14090a968]
140374a99:  0F 28 C2                 MOVAPS XMM0,XMM2
140374a9c:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140374aa1:  F3 0F 58 C8              ADDSS XMM1,XMM0
140374aa5:  F3 0F 11 4D B4           MOVSS dword ptr [RBP + -0x4c],XMM1
140374aaa:  F3 0F 59 15 F6 5F 59 00  MULSS XMM2,dword ptr [0x14090aaa8]
140374ab2:  F3 0F 58 96 50 AE 00 00  ADDSS XMM2,dword ptr [RSI + 0xae50]
140374aba:  F3 0F 11 54 24 70        MOVSS dword ptr [RSP + 0x70],XMM2
140374ac0:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140374ac5:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140374ac9:  48 8B CE                 MOV RCX,RSI
140374acc:  E8 5F E7 13 00           CALL 0x1404b3230
140374ad1:  E9 67 01 00 00           JMP 0x140374c3d
140374ad6:  83 F9 05                 CMP ECX,0x5
140374ad9:  0F 85 81 00 00 00        JNZ 0x140374b60
140374adf:  F3 0F 10 15 A1 D5 61 00  MOVSS XMM2,dword ptr [0x140992088]
140374ae7:  0F 28 CA                 MOVAPS XMM1,XMM2
140374aea:  F3 0F 59 0D 76 5E 59 00  MULSS XMM1,dword ptr [0x14090a968]
140374af2:  0F 28 C2                 MOVAPS XMM0,XMM2
140374af5:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140374afa:  F3 0F 58 C8              ADDSS XMM1,XMM0
140374afe:  F3 0F 11 4D B4           MOVSS dword ptr [RBP + -0x4c],XMM1
140374b03:  F3 0F 59 15 1D 5F 59 00  MULSS XMM2,dword ptr [0x14090aa28]
140374b0b:  F3 0F 58 96 50 AE 00 00  ADDSS XMM2,dword ptr [RSI + 0xae50]
140374b13:  F3 0F 11 54 24 70        MOVSS dword ptr [RSP + 0x70],XMM2
140374b19:  0F B6 44 24 7D           MOVZX EAX,byte ptr [RSP + 0x7d]
140374b1e:  88 44 24 20              MOV byte ptr [RSP + 0x20],AL
140374b22:  4C 8D 8D 90 00 00 00     LEA R9,[RBP + 0x90]
140374b29:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140374b2e:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140374b32:  48 8B CE                 MOV RCX,RSI
140374b35:  E8 E6 11 14 00           CALL 0x1404b5d20
140374b3a:  80 BE E6 05 00 00 00     CMP byte ptr [RSI + 0x5e6],0x0
140374b41:  0F 84 E3 00 00 00        JZ 0x140374c2a
140374b47:  48 8D 0D 12 95 64 00     LEA RCX,[0x1409be060]
140374b4e:  FF 15 A4 7D 4F 00        CALL qword ptr [0x14086c8f8]
140374b54:  48 8D 15 59 85 54 00     LEA RDX,[0x1408bd0b4]
140374b5b:  E9 81 00 00 00           JMP 0x140374be1
140374b60:  83 F9 06                 CMP ECX,0x6
140374b63:  0F 85 F4 00 00 00        JNZ 0x140374c5d
140374b69:  F3 0F 10 15 17 D5 61 00  MOVSS XMM2,dword ptr [0x140992088]
140374b71:  0F 28 CA                 MOVAPS XMM1,XMM2
140374b74:  F3 0F 59 0D EC 5D 59 00  MULSS XMM1,dword ptr [0x14090a968]
140374b7c:  0F 28 C2                 MOVAPS XMM0,XMM2
140374b7f:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140374b84:  F3 0F 58 C8              ADDSS XMM1,XMM0
140374b88:  F3 0F 11 4D B4           MOVSS dword ptr [RBP + -0x4c],XMM1
140374b8d:  F3 0F 59 15 93 5E 59 00  MULSS XMM2,dword ptr [0x14090aa28]
140374b95:  F3 0F 58 96 50 AE 00 00  ADDSS XMM2,dword ptr [RSI + 0xae50]
140374b9d:  F3 0F 11 54 24 70        MOVSS dword ptr [RSP + 0x70],XMM2
140374ba3:  0F B6 44 24 7D           MOVZX EAX,byte ptr [RSP + 0x7d]
140374ba8:  88 44 24 20              MOV byte ptr [RSP + 0x20],AL
140374bac:  4C 8D 8D 90 00 00 00     LEA R9,[RBP + 0x90]
140374bb3:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140374bb8:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140374bbc:  48 8B CE                 MOV RCX,RSI
140374bbf:  E8 7C 23 14 00           CALL 0x1404b6f40
140374bc4:  80 BE E6 05 00 00 00     CMP byte ptr [RSI + 0x5e6],0x0
=== export/ch_tab23_full.asm: 140362893..1403628dc ===
140362893:  F3 44 0F 11 55 B4        MOVSS dword ptr [RBP + -0x4c],XMM10
140362899:  B3 01                    MOV BL,0x1
14036289b:  89 5D 80                 MOV dword ptr [RBP + -0x80],EBX
14036289e:  4D 85 FF                 TEST R15,R15
1403628a1:  75 33                    JNZ 0x1403628d6
1403628a3:  41 83 BE 5C C6 00 00 01  CMP dword ptr [R14 + 0xc65c],0x1
1403628ab:  77 29                    JA 0x1403628d6
1403628ad:  4C 8D 45 E0              LEA R8,[RBP + -0x20]
1403628b1:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
1403628b5:  49 8B CE                 MOV RCX,R14
1403628b8:  E8 23 CB FF FF           CALL 0x14035f3e0
1403628bd:  0F B6 DB                 MOVZX EBX,BL
1403628c0:  84 C0                    TEST AL,AL
1403628c2:  41 0F 45 DC              CMOVNZ EBX,R12D
1403628c6:  89 5D 80                 MOV dword ptr [RBP + -0x80],EBX
1403628c9:  F3 44 0F 10 7D E0        MOVSS XMM15,dword ptr [RBP + -0x20]
1403628cf:  F3 44 0F 11 7C 24 70     MOVSS dword ptr [RSP + 0x70],XMM15
1403628d6:  F3 44 0F 11 45 C8        MOVSS dword ptr [RBP + -0x38],XMM8
1403628dc:  F3 44 0F 11 45 B0        MOVSS dword ptr [RBP + -0x50],XMM8
=== export/140317db0_full_disasm.asm: 14031edd3..14031ee18 ===
14031edd3:  44 89 7D 48              MOV dword ptr [RBP + 0x48],R15D
14031edd7:  44 89 7D F0              MOV dword ptr [RBP + -0x10],R15D
14031eddb:  4C 8D 45 F0              LEA R8,[RBP + -0x10]
14031eddf:  48 8D 55 48              LEA RDX,[RBP + 0x48]
14031ede3:  49 8B CE                 MOV RCX,R14
14031ede6:  45 38 BE 98 03 01 00     CMP byte ptr [R14 + 0x10398],R15B
14031eded:  74 21                    JZ 0x14031ee10
14031edef:  E8 AC 50 05 00           CALL 0x140373ea0
14031edf4:  F3 0F 10 45 F0           MOVSS XMM0,dword ptr [RBP + -0x10]
14031edf9:  F3 41 0F 5C 86 50 AE 00 00 SUBSS XMM0,dword ptr [R14 + 0xae50]
14031ee02:  F3 41 0F 11 86 58 AE 00 00 MOVSS dword ptr [R14 + 0xae58],XMM0
14031ee0b:  E9 08 02 00 00           JMP 0x14031f018
14031ee10:  45 33 C9                 XOR R9D,R9D
14031ee13:  E8 98 2D 04 00           CALL 0x140361bb0
14031ee18:  E9 FB 01 00 00           JMP 0x14031f018
