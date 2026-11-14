; Range: 0x1800BC690 to 0x1800BD000
1800bc690:  48 8B C4                 MOV RAX,RSP
1800bc693:  41 56                    PUSH R14
1800bc695:  41 57                    PUSH R15
1800bc697:  48 81 EC F8 00 00 00     SUB RSP,0xf8
1800bc69e:  F3 0F 10 A9 3C 67 00 00  MOVSS XMM5,dword ptr [RCX + 0x673c]
1800bc6a6:  4C 8B FA                 MOV R15,RDX
1800bc6a9:  0F 2F 2D 38 C5 0C 00     COMISS XMM5,dword ptr [0x180188be8]
1800bc6b0:  4C 8B F1                 MOV R14,RCX
1800bc6b3:  F3 0F 10 89 40 67 00 00  MOVSS XMM1,dword ptr [RCX + 0x6740]
1800bc6bb:  F3 0F 10 A1 44 67 00 00  MOVSS XMM4,dword ptr [RCX + 0x6744]
1800bc6c3:  0F 29 70 C8              MOVAPS xmmword ptr [RAX + -0x38],XMM6
1800bc6c7:  F3 0F 10 B1 4C 67 00 00  MOVSS XMM6,dword ptr [RCX + 0x674c]
1800bc6cf:  44 0F 29 50 88           MOVAPS xmmword ptr [RAX + -0x78],XMM10
1800bc6d4:  44 0F 28 D3              MOVAPS XMM10,XMM3
1800bc6d8:  44 0F 29 98 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM11
1800bc6e0:  44 0F 28 DA              MOVAPS XMM11,XMM2
1800bc6e4:  76 24                    JBE 0x1800bc70a
1800bc6e6:  F3 0F 10 81 34 67 00 00  MOVSS XMM0,dword ptr [RCX + 0x6734]
1800bc6ee:  F3 0F 5C C8              SUBSS XMM1,XMM0
1800bc6f2:  F3 0F 5C E0              SUBSS XMM4,XMM0
1800bc6f6:  F3 0F 5E F5              DIVSS XMM6,XMM5
1800bc6fa:  F3 0F 5E CD              DIVSS XMM1,XMM5
1800bc6fe:  F3 0F 5E E5              DIVSS XMM4,XMM5
1800bc702:  F3 0F 58 C8              ADDSS XMM1,XMM0
1800bc706:  F3 0F 58 E0              ADDSS XMM4,XMM0
1800bc70a:  F3 0F 10 42 04           MOVSS XMM0,dword ptr [RDX + 0x4]
1800bc70f:  0F 2F C8                 COMISS XMM1,XMM0
1800bc712:  0F 87 73 04 00 00        JA 0x1800bcb8b
1800bc718:  0F 2F C4                 COMISS XMM0,XMM4
1800bc71b:  0F 87 6A 04 00 00        JA 0x1800bcb8b
1800bc721:  48 89 9C 24 20 01 00 00  MOV qword ptr [RSP + 0x120],RBX
1800bc729:  F3 0F 5C C6              SUBSS XMM0,XMM6
1800bc72d:  48 89 AC 24 28 01 00 00  MOV qword ptr [RSP + 0x128],RBP
1800bc735:  48 89 B4 24 F0 00 00 00  MOV qword ptr [RSP + 0xf0],RSI
1800bc73d:  48 89 BC 24 E8 00 00 00  MOV qword ptr [RSP + 0xe8],RDI
1800bc745:  4C 89 A4 24 E0 00 00 00  MOV qword ptr [RSP + 0xe0],R12
1800bc74d:  45 33 E4                 XOR R12D,R12D
1800bc750:  0F 29 BC 24 C0 00 00 00  MOVAPS xmmword ptr [RSP + 0xc0],XMM7
1800bc758:  F3 0F 11 42 04           MOVSS dword ptr [RDX + 0x4],XMM0
1800bc75d:  48 8D 91 C4 02 00 00     LEA RDX,[RCX + 0x2c4]
1800bc764:  F3 0F 10 B9 8C 02 00 00  MOVSS XMM7,dword ptr [RCX + 0x28c]
1800bc76c:  F3 0F 59 3D 18 C4 0C 00  MULSS XMM7,dword ptr [0x180188b8c]
1800bc774:  44 0F 29 84 24 B0 00 00 00 MOVAPS xmmword ptr [RSP + 0xb0],XMM8
1800bc77d:  F3 44 0F 10 81 88 02 00 00 MOVSS XMM8,dword ptr [RCX + 0x288]
1800bc786:  F3 44 0F 59 05 FD C3 0C 00 MULSS XMM8,dword ptr [0x180188b8c]
1800bc78f:  0F 28 F7                 MOVAPS XMM6,XMM7
1800bc792:  44 89 A1 CC 02 00 00     MOV dword ptr [RCX + 0x2cc],R12D
1800bc799:  44 0F 29 8C 24 A0 00 00 00 MOVAPS xmmword ptr [RSP + 0xa0],XMM9
1800bc7a2:  F3 41 0F 5C F2           SUBSS XMM6,XMM10
1800bc7a7:  44 0F 29 64 24 70        MOVAPS xmmword ptr [RSP + 0x70],XMM12
1800bc7ad:  45 0F 28 CB              MOVAPS XMM9,XMM11
1800bc7b1:  44 0F 29 6C 24 60        MOVAPS xmmword ptr [RSP + 0x60],XMM13
1800bc7b7:  F3 45 0F 5C C8           SUBSS XMM9,XMM8
1800bc7bc:  44 0F 29 74 24 50        MOVAPS xmmword ptr [RSP + 0x50],XMM14
1800bc7c2:  44 0F 29 7C 24 40        MOVAPS xmmword ptr [RSP + 0x40],XMM15
1800bc7c8:  F3 0F 11 B1 C8 02 00 00  MOVSS dword ptr [RCX + 0x2c8],XMM6
1800bc7d0:  F3 44 0F 11 89 C4 02 00 00 MOVSS dword ptr [RCX + 0x2c4],XMM9
1800bc7d9:  E8 A2 C4 FF FF           CALL 0x1800b8c80
1800bc7de:  F3 45 0F 5C C3           SUBSS XMM8,XMM11
1800bc7e3:  F3 41 0F 11 B6 D4 02 00 00 MOVSS dword ptr [R14 + 0x2d4],XMM6
1800bc7ec:  49 8D 96 D0 02 00 00     LEA RDX,[R14 + 0x2d0]
1800bc7f3:  45 89 A6 D8 02 00 00     MOV dword ptr [R14 + 0x2d8],R12D
1800bc7fa:  49 8B CE                 MOV RCX,R14
1800bc7fd:  F3 45 0F 11 86 D0 02 00 00 MOVSS dword ptr [R14 + 0x2d0],XMM8
1800bc806:  E8 75 C4 FF FF           CALL 0x1800b8c80
1800bc80b:  F3 44 0F 5C D7           SUBSS XMM10,XMM7
1800bc810:  F3 45 0F 11 86 DC 02 00 00 MOVSS dword ptr [R14 + 0x2dc],XMM8
1800bc819:  49 8D 96 DC 02 00 00     LEA RDX,[R14 + 0x2dc]
1800bc820:  45 89 A6 E4 02 00 00     MOV dword ptr [R14 + 0x2e4],R12D
1800bc827:  49 8B CE                 MOV RCX,R14
1800bc82a:  F3 45 0F 11 96 E0 02 00 00 MOVSS dword ptr [R14 + 0x2e0],XMM10
1800bc833:  E8 48 C4 FF FF           CALL 0x1800b8c80
1800bc838:  49 8D 96 E8 02 00 00     LEA RDX,[R14 + 0x2e8]
1800bc83f:  F3 45 0F 11 8E E8 02 00 00 MOVSS dword ptr [R14 + 0x2e8],XMM9
1800bc848:  49 8B CE                 MOV RCX,R14
1800bc84b:  F3 45 0F 11 96 EC 02 00 00 MOVSS dword ptr [R14 + 0x2ec],XMM10
1800bc854:  45 89 A6 F0 02 00 00     MOV dword ptr [R14 + 0x2f0],R12D
1800bc85b:  E8 20 C4 FF FF           CALL 0x1800b8c80
1800bc860:  F3 41 0F 10 17           MOVSS XMM2,dword ptr [R15]
1800bc865:  F3 41 0F 10 4F 04        MOVSS XMM1,dword ptr [R15 + 0x4]
1800bc86b:  F3 41 0F 10 86 C4 02 00 00 MOVSS XMM0,dword ptr [R14 + 0x2c4]
1800bc874:  F3 45 0F 10 AE D0 02 00 00 MOVSS XMM13,dword ptr [R14 + 0x2d0]
1800bc87d:  F3 0F 5C C2              SUBSS XMM0,XMM2
1800bc881:  F3 41 0F 10 B6 DC 02 00 00 MOVSS XMM6,dword ptr [R14 + 0x2dc]
1800bc88a:  F3 44 0F 5C EA           SUBSS XMM13,XMM2
1800bc88f:  F3 41 0F 10 9E E8 02 00 00 MOVSS XMM3,dword ptr [R14 + 0x2e8]
1800bc898:  F3 0F 5C F2              SUBSS XMM6,XMM2
1800bc89c:  F3 41 0F 10 BE C8 02 00 00 MOVSS XMM7,dword ptr [R14 + 0x2c8]
1800bc8a5:  F3 0F 5C DA              SUBSS XMM3,XMM2
1800bc8a9:  F3 45 0F 10 96 CC 02 00 00 MOVSS XMM10,dword ptr [R14 + 0x2cc]
1800bc8b2:  F3 0F 5C F9              SUBSS XMM7,XMM1
1800bc8b6:  F3 41 0F 10 96 EC 02 00 00 MOVSS XMM2,dword ptr [R14 + 0x2ec]
1800bc8bf:  F3 45 0F 10 9E D4 02 00 00 MOVSS XMM11,dword ptr [R14 + 0x2d4]
1800bc8c8:  F3 0F 5C D1              SUBSS XMM2,XMM1
1800bc8cc:  F3 45 0F 10 A6 D8 02 00 00 MOVSS XMM12,dword ptr [R14 + 0x2d8]
1800bc8d5:  F3 44 0F 5C D9           SUBSS XMM11,XMM1
1800bc8da:  F3 45 0F 10 B6 E0 02 00 00 MOVSS XMM14,dword ptr [R14 + 0x2e0]
1800bc8e3:  F3 45 0F 10 BE E4 02 00 00 MOVSS XMM15,dword ptr [R14 + 0x2e4]
1800bc8ec:  F3 44 0F 5C F1           SUBSS XMM14,XMM1
1800bc8f1:  F3 45 0F 10 8E F0 02 00 00 MOVSS XMM9,dword ptr [R14 + 0x2f0]
1800bc8fa:  F3 0F 11 84 24 10 01 00 00 MOVSS dword ptr [RSP + 0x110],XMM0
1800bc903:  F3 41 0F 10 47 08        MOVSS XMM0,dword ptr [R15 + 0x8]
1800bc909:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
1800bc90e:  F3 0F 11 7C 24 24        MOVSS dword ptr [RSP + 0x24],XMM7
1800bc914:  F3 0F 11 74 24 2C        MOVSS dword ptr [RSP + 0x2c],XMM6
1800bc91a:  F3 44 0F 5C E0           SUBSS XMM12,XMM0
1800bc91f:  F3 0F 11 5C 24 20        MOVSS dword ptr [RSP + 0x20],XMM3
1800bc925:  F3 44 0F 5C F8           SUBSS XMM15,XMM0
1800bc92a:  F3 0F 11 94 24 18 01 00 00 MOVSS dword ptr [RSP + 0x118],XMM2
1800bc933:  F3 44 0F 5C C8           SUBSS XMM9,XMM0
1800bc938:  F3 44 0F 11 54 24 28     MOVSS dword ptr [RSP + 0x28],XMM10
1800bc93f:  41 0F 28 D3              MOVAPS XMM2,XMM11
1800bc943:  F3 44 0F 11 4C 24 30     MOVSS dword ptr [RSP + 0x30],XMM9
1800bc94a:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1800bc94f:  41 0F 28 C5              MOVAPS XMM0,XMM13
1800bc953:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1800bc958:  41 0F 28 CC              MOVAPS XMM1,XMM12
1800bc95c:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1800bc961:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bc965:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bc969:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bc96c:  E8 83 BE 0A 00           CALL 0x1801687f4
1800bc971:  F3 0F 10 8C 24 10 01 00 00 MOVSS XMM1,dword ptr [RSP + 0x110]
1800bc97a:  44 0F 28 C0              MOVAPS XMM8,XMM0
1800bc97e:  0F 28 C1                 MOVAPS XMM0,XMM1
1800bc981:  0F 28 D7                 MOVAPS XMM2,XMM7
1800bc984:  F3 0F 59 C1              MULSS XMM0,XMM1
1800bc988:  41 0F 28 CA              MOVAPS XMM1,XMM10
1800bc98c:  F3 0F 59 D7              MULSS XMM2,XMM7
1800bc990:  F3 41 0F 59 CA           MULSS XMM1,XMM10
1800bc995:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bc999:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bc99d:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bc9a0:  E8 4F BE 0A 00           CALL 0x1801687f4
1800bc9a5:  44 0F 28 D0              MOVAPS XMM10,XMM0
1800bc9a9:  41 0F 28 D6              MOVAPS XMM2,XMM14
1800bc9ad:  F3 41 0F 59 D6           MULSS XMM2,XMM14
1800bc9b2:  0F 28 C6                 MOVAPS XMM0,XMM6
1800bc9b5:  F3 0F 59 C6              MULSS XMM0,XMM6
1800bc9b9:  41 0F 28 CF              MOVAPS XMM1,XMM15
1800bc9bd:  F3 41 0F 59 CF           MULSS XMM1,XMM15
1800bc9c2:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bc9c6:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bc9ca:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bc9cd:  E8 22 BE 0A 00           CALL 0x1801687f4
1800bc9d2:  F3 0F 10 4C 24 20        MOVSS XMM1,dword ptr [RSP + 0x20]
1800bc9d8:  0F 28 F8                 MOVAPS XMM7,XMM0
1800bc9db:  F3 0F 10 84 24 18 01 00 00 MOVSS XMM0,dword ptr [RSP + 0x118]
1800bc9e4:  0F 28 D0                 MOVAPS XMM2,XMM0
1800bc9e7:  F3 0F 59 D0              MULSS XMM2,XMM0
1800bc9eb:  0F 28 C1                 MOVAPS XMM0,XMM1
1800bc9ee:  F3 0F 59 C1              MULSS XMM0,XMM1
1800bc9f2:  41 0F 28 C9              MOVAPS XMM1,XMM9
1800bc9f6:  F3 41 0F 59 C9           MULSS XMM1,XMM9
1800bc9fb:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bc9ff:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bca03:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bca06:  E8 E9 BD 0A 00           CALL 0x1801687f4
1800bca0b:  44 0F 28 C8              MOVAPS XMM9,XMM0
1800bca0f:  41 0F 28 D3              MOVAPS XMM2,XMM11
1800bca13:  F3 0F 59 54 24 24        MULSS XMM2,dword ptr [RSP + 0x24]
1800bca19:  41 0F 28 C5              MOVAPS XMM0,XMM13
1800bca1d:  F3 0F 59 84 24 10 01 00 00 MULSS XMM0,dword ptr [RSP + 0x110]
1800bca26:  41 0F 28 CC              MOVAPS XMM1,XMM12
1800bca2a:  F3 0F 59 4C 24 28        MULSS XMM1,dword ptr [RSP + 0x28]
1800bca30:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bca34:  41 0F 28 C2              MOVAPS XMM0,XMM10
1800bca38:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1800bca3d:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bca41:  F3 0F 5E D0              DIVSS XMM2,XMM0
1800bca45:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bca48:  E8 6B BD 0A 00           CALL 0x1801687b8
1800bca4d:  0F 28 F0                 MOVAPS XMM6,XMM0
1800bca50:  41 0F 28 D6              MOVAPS XMM2,XMM14
1800bca54:  0F 57 C0                 XORPS XMM0,XMM0
1800bca57:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1800bca5c:  F3 44 0F 10 5C 24 2C     MOVSS XMM11,dword ptr [RSP + 0x2c]
1800bca63:  F3 0F 58 F0              ADDSS XMM6,XMM0
1800bca67:  41 0F 28 C3              MOVAPS XMM0,XMM11
1800bca6b:  41 0F 28 CF              MOVAPS XMM1,XMM15
1800bca6f:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1800bca74:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1800bca79:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bca7d:  0F 28 C7                 MOVAPS XMM0,XMM7
1800bca80:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1800bca85:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bca89:  F3 0F 5E D0              DIVSS XMM2,XMM0
1800bca8d:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bca90:  E8 23 BD 0A 00           CALL 0x1801687b8
1800bca95:  F3 44 0F 10 AC 24 18 01 00 00 MOVSS XMM13,dword ptr [RSP + 0x118]
1800bca9f:  F3 0F 58 F0              ADDSS XMM6,XMM0
1800bcaa3:  41 0F 28 D5              MOVAPS XMM2,XMM13
1800bcaa7:  F3 44 0F 10 64 24 20     MOVSS XMM12,dword ptr [RSP + 0x20]
1800bcaae:  F3 44 0F 10 44 24 30     MOVSS XMM8,dword ptr [RSP + 0x30]
1800bcab5:  41 0F 28 C4              MOVAPS XMM0,XMM12
1800bcab9:  F3 41 0F 59 C3           MULSS XMM0,XMM11
1800bcabe:  41 0F 28 C8              MOVAPS XMM1,XMM8
1800bcac2:  F3 41 0F 59 D6           MULSS XMM2,XMM14
1800bcac7:  F3 41 0F 59 CF           MULSS XMM1,XMM15
1800bcacc:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcad0:  41 0F 28 C1              MOVAPS XMM0,XMM9
1800bcad4:  F3 0F 59 C7              MULSS XMM0,XMM7
1800bcad8:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcadc:  F3 0F 5E D0              DIVSS XMM2,XMM0
1800bcae0:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bcae3:  E8 D0 BC 0A 00           CALL 0x1801687b8
1800bcae8:  F3 44 0F 59 6C 24 24     MULSS XMM13,dword ptr [RSP + 0x24]
1800bcaef:  F3 0F 58 F0              ADDSS XMM6,XMM0
1800bcaf3:  F3 44 0F 59 A4 24 10 01 00 00 MULSS XMM12,dword ptr [RSP + 0x110]
1800bcafd:  F3 44 0F 59 44 24 28     MULSS XMM8,dword ptr [RSP + 0x28]
1800bcb04:  F3 45 0F 58 EC           ADDSS XMM13,XMM12
1800bcb09:  F3 45 0F 59 CA           MULSS XMM9,XMM10
1800bcb0e:  F3 45 0F 58 E8           ADDSS XMM13,XMM8
1800bcb13:  F3 45 0F 5E E9           DIVSS XMM13,XMM9
1800bcb18:  41 0F 28 C5              MOVAPS XMM0,XMM13
1800bcb1c:  E8 97 BC 0A 00           CALL 0x1801687b8
1800bcb21:  44 0F 28 7C 24 40        MOVAPS XMM15,xmmword ptr [RSP + 0x40]
1800bcb27:  F3 0F 58 F0              ADDSS XMM6,XMM0
1800bcb2b:  44 0F 28 74 24 50        MOVAPS XMM14,xmmword ptr [RSP + 0x50]
1800bcb31:  44 0F 28 6C 24 60        MOVAPS XMM13,xmmword ptr [RSP + 0x60]
1800bcb37:  44 0F 28 64 24 70        MOVAPS XMM12,xmmword ptr [RSP + 0x70]
1800bcb3d:  44 0F 28 8C 24 A0 00 00 00 MOVAPS XMM9,xmmword ptr [RSP + 0xa0]
1800bcb46:  0F 2F 35 13 C1 0C 00     COMISS XMM6,dword ptr [0x180188c60]
1800bcb4d:  4C 8B A4 24 E0 00 00 00  MOV R12,qword ptr [RSP + 0xe0]
1800bcb55:  48 8B BC 24 E8 00 00 00  MOV RDI,qword ptr [RSP + 0xe8]
1800bcb5d:  48 8B B4 24 F0 00 00 00  MOV RSI,qword ptr [RSP + 0xf0]
1800bcb65:  48 8B AC 24 28 01 00 00  MOV RBP,qword ptr [RSP + 0x128]
1800bcb6d:  48 8B 9C 24 20 01 00 00  MOV RBX,qword ptr [RSP + 0x120]
1800bcb75:  44 0F 28 84 24 B0 00 00 00 MOVAPS XMM8,xmmword ptr [RSP + 0xb0]
1800bcb7e:  0F 28 BC 24 C0 00 00 00  MOVAPS XMM7,xmmword ptr [RSP + 0xc0]
1800bcb86:  0F 97 C0                 SETA AL
1800bcb89:  EB 02                    JMP 0x1800bcb8d
1800bcb8b:  32 C0                    XOR AL,AL
1800bcb8d:  4C 8D 9C 24 F8 00 00 00  LEA R11,[RSP + 0xf8]
1800bcb95:  41 0F 28 73 D8           MOVAPS XMM6,xmmword ptr [R11 + -0x28]
1800bcb9a:  45 0F 28 53 98           MOVAPS XMM10,xmmword ptr [R11 + -0x68]
1800bcb9f:  45 0F 28 5B 88           MOVAPS XMM11,xmmword ptr [R11 + -0x78]
1800bcba4:  49 8B E3                 MOV RSP,R11
1800bcba7:  41 5F                    POP R15
1800bcba9:  41 5E                    POP R14
1800bcbab:  C3                       RET
1800bcbb0:  41 54                    PUSH R12
1800bcbb2:  41 56                    PUSH R14
1800bcbb4:  41 57                    PUSH R15
1800bcbb6:  48 81 EC 10 01 00 00     SUB RSP,0x110
1800bcbbd:  F3 0F 10 A1 3C 67 00 00  MOVSS XMM4,dword ptr [RCX + 0x673c]
1800bcbc5:  4D 8B E0                 MOV R12,R8
1800bcbc8:  0F 2F 25 19 C0 0C 00     COMISS XMM4,dword ptr [0x180188be8]
1800bcbcf:  4C 8B FA                 MOV R15,RDX
1800bcbd2:  4C 8B F1                 MOV R14,RCX
1800bcbd5:  F3 0F 10 89 40 67 00 00  MOVSS XMM1,dword ptr [RCX + 0x6740]
1800bcbdd:  F3 0F 10 91 44 67 00 00  MOVSS XMM2,dword ptr [RCX + 0x6744]
1800bcbe5:  F3 0F 10 A9 4C 67 00 00  MOVSS XMM5,dword ptr [RCX + 0x674c]
1800bcbed:  44 0F 29 8C 24 B0 00 00 00 MOVAPS xmmword ptr [RSP + 0xb0],XMM9
1800bcbf6:  44 0F 28 CB              MOVAPS XMM9,XMM3
1800bcbfa:  76 24                    JBE 0x1800bcc20
1800bcbfc:  F3 0F 10 81 34 67 00 00  MOVSS XMM0,dword ptr [RCX + 0x6734]
1800bcc04:  F3 0F 5C C8              SUBSS XMM1,XMM0
1800bcc08:  F3 0F 5C D0              SUBSS XMM2,XMM0
1800bcc0c:  F3 0F 5E EC              DIVSS XMM5,XMM4
1800bcc10:  F3 0F 5E CC              DIVSS XMM1,XMM4
1800bcc14:  F3 0F 5E D4              DIVSS XMM2,XMM4
1800bcc18:  F3 0F 58 C8              ADDSS XMM1,XMM0
1800bcc1c:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcc20:  F3 0F 10 42 04           MOVSS XMM0,dword ptr [RDX + 0x4]
1800bcc25:  0F 2F C8                 COMISS XMM1,XMM0
1800bcc28:  0F 87 36 05 00 00        JA 0x1800bd164
1800bcc2e:  0F 2F C2                 COMISS XMM0,XMM2
1800bcc31:  0F 87 2D 05 00 00        JA 0x1800bd164
1800bcc37:  F3 44 0F 59 0D 4C BF 0C 00 MULSS XMM9,dword ptr [0x180188b8c]
1800bcc40:  F3 0F 5C C5              SUBSS XMM0,XMM5
1800bcc44:  48 89 9C 24 40 01 00 00  MOV qword ptr [RSP + 0x140],RBX
1800bcc4c:  48 89 AC 24 08 01 00 00  MOV qword ptr [RSP + 0x108],RBP
1800bcc54:  48 89 B4 24 00 01 00 00  MOV qword ptr [RSP + 0x100],RSI
1800bcc5c:  48 89 BC 24 F8 00 00 00  MOV qword ptr [RSP + 0xf8],RDI
1800bcc64:  4C 89 AC 24 F0 00 00 00  MOV qword ptr [RSP + 0xf0],R13
1800bcc6c:  45 33 ED                 XOR R13D,R13D
1800bcc6f:  0F 29 B4 24 E0 00 00 00  MOVAPS xmmword ptr [RSP + 0xe0],XMM6
1800bcc77:  0F 29 BC 24 D0 00 00 00  MOVAPS xmmword ptr [RSP + 0xd0],XMM7
1800bcc7f:  41 0F 28 F9              MOVAPS XMM7,XMM9
1800bcc83:  0F 57 3D E6 C3 0C 00     XORPS XMM7,xmmword ptr [0x180189070]
1800bcc8a:  44 0F 29 84 24 C0 00 00 00 MOVAPS xmmword ptr [RSP + 0xc0],XMM8
1800bcc93:  F3 44 0F 10 84 24 50 01 00 00 MOVSS XMM8,dword ptr [RSP + 0x150]
1800bcc9d:  F3 44 0F 59 05 E6 BE 0C 00 MULSS XMM8,dword ptr [0x180188b8c]
1800bcca6:  F3 0F 11 42 04           MOVSS dword ptr [RDX + 0x4],XMM0
1800bccab:  48 8D 91 C4 02 00 00     LEA RDX,[RCX + 0x2c4]
1800bccb2:  44 0F 29 94 24 A0 00 00 00 MOVAPS xmmword ptr [RSP + 0xa0],XMM10
1800bccbb:  F3 44 0F 11 81 C8 02 00 00 MOVSS dword ptr [RCX + 0x2c8],XMM8
1800bccc4:  F3 0F 11 B9 C4 02 00 00  MOVSS dword ptr [RCX + 0x2c4],XMM7
1800bcccc:  44 89 A9 CC 02 00 00     MOV dword ptr [RCX + 0x2cc],R13D
1800bccd3:  F3 41 0F 10 00           MOVSS XMM0,dword ptr [R8]
1800bccd8:  F3 41 0F 10 48 04        MOVSS XMM1,dword ptr [R8 + 0x4]
1800bccde:  44 0F 29 9C 24 90 00 00 00 MOVAPS xmmword ptr [RSP + 0x90],XMM11
1800bcce7:  44 0F 29 A4 24 80 00 00 00 MOVAPS xmmword ptr [RSP + 0x80],XMM12
1800bccf0:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1800bccf6:  F3 41 0F 10 40 08        MOVSS XMM0,dword ptr [R8 + 0x8]
1800bccfc:  4C 8D 44 24 38           LEA R8,[RSP + 0x38]
1800bcd01:  44 0F 29 6C 24 70        MOVAPS xmmword ptr [RSP + 0x70],XMM13
1800bcd07:  44 0F 29 74 24 60        MOVAPS xmmword ptr [RSP + 0x60],XMM14
1800bcd0d:  F3 0F 11 44 24 40        MOVSS dword ptr [RSP + 0x40],XMM0
1800bcd13:  F3 0F 11 4C 24 3C        MOVSS dword ptr [RSP + 0x3c],XMM1
1800bcd19:  44 0F 29 7C 24 50        MOVAPS xmmword ptr [RSP + 0x50],XMM15
1800bcd1f:  E8 FC C1 FF FF           CALL 0x1800b8f20
1800bcd24:  F3 45 0F 11 8E D0 02 00 00 MOVSS dword ptr [R14 + 0x2d0],XMM9
1800bcd2d:  4C 8D 44 24 38           LEA R8,[RSP + 0x38]
1800bcd32:  F3 45 0F 11 86 D4 02 00 00 MOVSS dword ptr [R14 + 0x2d4],XMM8
1800bcd3b:  49 8D 96 D0 02 00 00     LEA RDX,[R14 + 0x2d0]
1800bcd42:  45 89 AE D8 02 00 00     MOV dword ptr [R14 + 0x2d8],R13D
1800bcd49:  49 8B CE                 MOV RCX,R14
1800bcd4c:  F3 41 0F 10 04 24        MOVSS XMM0,dword ptr [R12]
1800bcd52:  F3 41 0F 10 4C 24 04     MOVSS XMM1,dword ptr [R12 + 0x4]
1800bcd59:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1800bcd5f:  F3 41 0F 10 44 24 08     MOVSS XMM0,dword ptr [R12 + 0x8]
1800bcd66:  F3 0F 11 44 24 40        MOVSS dword ptr [RSP + 0x40],XMM0
1800bcd6c:  F3 0F 11 4C 24 3C        MOVSS dword ptr [RSP + 0x3c],XMM1
1800bcd72:  E8 A9 C1 FF FF           CALL 0x1800b8f20
1800bcd77:  44 0F 57 05 F1 C2 0C 00  XORPS XMM8,xmmword ptr [0x180189070]
1800bcd7f:  4C 8D 44 24 38           LEA R8,[RSP + 0x38]
1800bcd84:  F3 45 0F 11 86 E0 02 00 00 MOVSS dword ptr [R14 + 0x2e0],XMM8
1800bcd8d:  49 8D 96 DC 02 00 00     LEA RDX,[R14 + 0x2dc]
1800bcd94:  F3 45 0F 11 8E DC 02 00 00 MOVSS dword ptr [R14 + 0x2dc],XMM9
1800bcd9d:  49 8B CE                 MOV RCX,R14
1800bcda0:  45 89 AE E4 02 00 00     MOV dword ptr [R14 + 0x2e4],R13D
1800bcda7:  F3 41 0F 10 04 24        MOVSS XMM0,dword ptr [R12]
1800bcdad:  F3 41 0F 10 4C 24 04     MOVSS XMM1,dword ptr [R12 + 0x4]
1800bcdb4:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1800bcdba:  F3 41 0F 10 44 24 08     MOVSS XMM0,dword ptr [R12 + 0x8]
1800bcdc1:  F3 0F 11 44 24 40        MOVSS dword ptr [RSP + 0x40],XMM0
1800bcdc7:  F3 0F 11 4C 24 3C        MOVSS dword ptr [RSP + 0x3c],XMM1
1800bcdcd:  E8 4E C1 FF FF           CALL 0x1800b8f20
1800bcdd2:  F3 41 0F 11 BE E8 02 00 00 MOVSS dword ptr [R14 + 0x2e8],XMM7
1800bcddb:  4C 8D 44 24 38           LEA R8,[RSP + 0x38]
1800bcde0:  F3 45 0F 11 86 EC 02 00 00 MOVSS dword ptr [R14 + 0x2ec],XMM8
1800bcde9:  49 8D 96 E8 02 00 00     LEA RDX,[R14 + 0x2e8]
1800bcdf0:  45 89 AE F0 02 00 00     MOV dword ptr [R14 + 0x2f0],R13D
1800bcdf7:  49 8B CE                 MOV RCX,R14
1800bcdfa:  F3 41 0F 10 04 24        MOVSS XMM0,dword ptr [R12]
1800bce00:  F3 41 0F 10 4C 24 04     MOVSS XMM1,dword ptr [R12 + 0x4]
1800bce07:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1800bce0d:  F3 41 0F 10 44 24 08     MOVSS XMM0,dword ptr [R12 + 0x8]
1800bce14:  F3 0F 11 44 24 40        MOVSS dword ptr [RSP + 0x40],XMM0
1800bce1a:  F3 0F 11 4C 24 3C        MOVSS dword ptr [RSP + 0x3c],XMM1
1800bce20:  E8 FB C0 FF FF           CALL 0x1800b8f20
1800bce25:  F3 41 0F 10 17           MOVSS XMM2,dword ptr [R15]
1800bce2a:  F3 41 0F 10 86 C4 02 00 00 MOVSS XMM0,dword ptr [R14 + 0x2c4]
1800bce33:  F3 41 0F 10 4F 04        MOVSS XMM1,dword ptr [R15 + 0x4]
1800bce39:  F3 0F 5C C2              SUBSS XMM0,XMM2
1800bce3d:  F3 41 0F 10 BE C8 02 00 00 MOVSS XMM7,dword ptr [R14 + 0x2c8]
1800bce46:  F3 0F 5C F9              SUBSS XMM7,XMM1
1800bce4a:  F3 0F 11 84 24 30 01 00 00 MOVSS dword ptr [RSP + 0x130],XMM0
1800bce53:  F3 41 0F 10 47 08        MOVSS XMM0,dword ptr [R15 + 0x8]
1800bce59:  F3 45 0F 10 AE D0 02 00 00 MOVSS XMM13,dword ptr [R14 + 0x2d0]
1800bce62:  F3 41 0F 10 9E E8 02 00 00 MOVSS XMM3,dword ptr [R14 + 0x2e8]
1800bce6b:  F3 44 0F 5C EA           SUBSS XMM13,XMM2
1800bce70:  F3 45 0F 10 9E D4 02 00 00 MOVSS XMM11,dword ptr [R14 + 0x2d4]
1800bce79:  F3 0F 5C DA              SUBSS XMM3,XMM2
1800bce7d:  F3 45 0F 10 A6 D8 02 00 00 MOVSS XMM12,dword ptr [R14 + 0x2d8]
1800bce86:  F3 44 0F 5C D9           SUBSS XMM11,XMM1
1800bce8b:  F3 41 0F 10 B6 DC 02 00 00 MOVSS XMM6,dword ptr [R14 + 0x2dc]
1800bce94:  F3 44 0F 5C E0           SUBSS XMM12,XMM0
1800bce99:  F3 45 0F 10 96 CC 02 00 00 MOVSS XMM10,dword ptr [R14 + 0x2cc]
1800bcea2:  F3 0F 5C F2              SUBSS XMM6,XMM2
1800bcea6:  F3 41 0F 10 96 EC 02 00 00 MOVSS XMM2,dword ptr [R14 + 0x2ec]
1800bceaf:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
1800bceb4:  F3 45 0F 10 BE E4 02 00 00 MOVSS XMM15,dword ptr [R14 + 0x2e4]
1800bcebd:  F3 0F 5C D1              SUBSS XMM2,XMM1
1800bcec1:  F3 45 0F 10 8E F0 02 00 00 MOVSS XMM9,dword ptr [R14 + 0x2f0]
1800bceca:  F3 44 0F 5C F8           SUBSS XMM15,XMM0
1800bcecf:  F3 45 0F 10 B6 E0 02 00 00 MOVSS XMM14,dword ptr [R14 + 0x2e0]
1800bced8:  F3 44 0F 5C C8           SUBSS XMM9,XMM0
1800bcedd:  F3 44 0F 5C F1           SUBSS XMM14,XMM1
1800bcee2:  F3 0F 11 7C 24 24        MOVSS dword ptr [RSP + 0x24],XMM7
1800bcee8:  41 0F 28 C5              MOVAPS XMM0,XMM13
1800bceec:  F3 44 0F 11 54 24 28     MOVSS dword ptr [RSP + 0x28],XMM10
1800bcef3:  F3 0F 11 94 24 38 01 00 00 MOVSS dword ptr [RSP + 0x138],XMM2
1800bcefc:  41 0F 28 CC              MOVAPS XMM1,XMM12
1800bcf00:  41 0F 28 D3              MOVAPS XMM2,XMM11
1800bcf04:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1800bcf09:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1800bcf0e:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1800bcf13:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcf17:  F3 0F 11 74 24 2C        MOVSS dword ptr [RSP + 0x2c],XMM6
1800bcf1d:  F3 0F 11 5C 24 20        MOVSS dword ptr [RSP + 0x20],XMM3
1800bcf23:  F3 44 0F 11 4C 24 30     MOVSS dword ptr [RSP + 0x30],XMM9
1800bcf2a:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcf2e:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bcf31:  E8 BE B8 0A 00           CALL 0x1801687f4
1800bcf36:  F3 0F 10 8C 24 30 01 00 00 MOVSS XMM1,dword ptr [RSP + 0x130]
1800bcf3f:  44 0F 28 C0              MOVAPS XMM8,XMM0
1800bcf43:  0F 28 C1                 MOVAPS XMM0,XMM1
1800bcf46:  0F 28 D7                 MOVAPS XMM2,XMM7
1800bcf49:  F3 0F 59 C1              MULSS XMM0,XMM1
1800bcf4d:  41 0F 28 CA              MOVAPS XMM1,XMM10
1800bcf51:  F3 0F 59 D7              MULSS XMM2,XMM7
1800bcf55:  F3 41 0F 59 CA           MULSS XMM1,XMM10
1800bcf5a:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcf5e:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcf62:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bcf65:  E8 8A B8 0A 00           CALL 0x1801687f4
1800bcf6a:  44 0F 28 D0              MOVAPS XMM10,XMM0
1800bcf6e:  41 0F 28 D6              MOVAPS XMM2,XMM14
1800bcf72:  F3 41 0F 59 D6           MULSS XMM2,XMM14
1800bcf77:  0F 28 C6                 MOVAPS XMM0,XMM6
1800bcf7a:  F3 0F 59 C6              MULSS XMM0,XMM6
1800bcf7e:  41 0F 28 CF              MOVAPS XMM1,XMM15
1800bcf82:  F3 41 0F 59 CF           MULSS XMM1,XMM15
1800bcf87:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcf8b:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcf8f:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bcf92:  E8 5D B8 0A 00           CALL 0x1801687f4
1800bcf97:  F3 0F 10 4C 24 20        MOVSS XMM1,dword ptr [RSP + 0x20]
1800bcf9d:  0F 28 F8                 MOVAPS XMM7,XMM0
1800bcfa0:  F3 0F 10 84 24 38 01 00 00 MOVSS XMM0,dword ptr [RSP + 0x138]
1800bcfa9:  0F 28 D0                 MOVAPS XMM2,XMM0
1800bcfac:  F3 0F 59 D0              MULSS XMM2,XMM0
1800bcfb0:  0F 28 C1                 MOVAPS XMM0,XMM1
1800bcfb3:  F3 0F 59 C1              MULSS XMM0,XMM1
1800bcfb7:  41 0F 28 C9              MOVAPS XMM1,XMM9
1800bcfbb:  F3 41 0F 59 C9           MULSS XMM1,XMM9
1800bcfc0:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcfc4:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcfc8:  0F 28 C2                 MOVAPS XMM0,XMM2
1800bcfcb:  E8 24 B8 0A 00           CALL 0x1801687f4
1800bcfd0:  44 0F 28 C8              MOVAPS XMM9,XMM0
1800bcfd4:  41 0F 28 D3              MOVAPS XMM2,XMM11
1800bcfd8:  F3 0F 59 54 24 24        MULSS XMM2,dword ptr [RSP + 0x24]
1800bcfde:  41 0F 28 C5              MOVAPS XMM0,XMM13
1800bcfe2:  F3 0F 59 84 24 30 01 00 00 MULSS XMM0,dword ptr [RSP + 0x130]
1800bcfeb:  41 0F 28 CC              MOVAPS XMM1,XMM12
1800bcfef:  F3 0F 59 4C 24 28        MULSS XMM1,dword ptr [RSP + 0x28]
1800bcff5:  F3 0F 58 D0              ADDSS XMM2,XMM0
1800bcff9:  F3 0F 58 D1              ADDSS XMM2,XMM1
1800bcffd:  41 0F 28 C2              MOVAPS XMM0,XMM10

; Range: 0x1800BAF40 to 0x1800BB200
1800baf40:  48 8B C4                 MOV RAX,RSP
1800baf43:  48 89 58 10              MOV qword ptr [RAX + 0x10],RBX
1800baf47:  48 89 70 18              MOV qword ptr [RAX + 0x18],RSI
1800baf4b:  48 89 78 20              MOV qword ptr [RAX + 0x20],RDI
1800baf4f:  55                       PUSH RBP
1800baf50:  41 54                    PUSH R12
1800baf52:  41 55                    PUSH R13
1800baf54:  41 56                    PUSH R14
1800baf56:  41 57                    PUSH R15
1800baf58:  48 8D 68 B8              LEA RBP,[RAX + -0x48]
1800baf5c:  48 81 EC 20 01 00 00     SUB RSP,0x120
1800baf63:  0F 29 70 C8              MOVAPS xmmword ptr [RAX + -0x38],XMM6
1800baf67:  0F 29 78 B8              MOVAPS xmmword ptr [RAX + -0x48],XMM7
1800baf6b:  44 0F 29 40 A8           MOVAPS xmmword ptr [RAX + -0x58],XMM8
1800baf70:  44 0F 29 48 98           MOVAPS xmmword ptr [RAX + -0x68],XMM9
1800baf75:  44 0F 29 50 88           MOVAPS xmmword ptr [RAX + -0x78],XMM10
1800baf7a:  44 0F 29 98 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM11
1800baf82:  44 0F 29 A0 68 FF FF FF  MOVAPS xmmword ptr [RAX + -0x98],XMM12
1800baf8a:  44 0F 29 A8 58 FF FF FF  MOVAPS xmmword ptr [RAX + -0xa8],XMM13
1800baf92:  44 0F 29 B0 48 FF FF FF  MOVAPS xmmword ptr [RAX + -0xb8],XMM14
1800baf9a:  48 8B 05 F7 2B 11 00     MOV RAX,qword ptr [0x1801cdb98]
1800bafa1:  48 33 C4                 XOR RAX,RSP
1800bafa4:  48 89 45 80              MOV qword ptr [RBP + -0x80],RAX
1800bafa8:  80 3D 34 17 12 00 00     CMP byte ptr [0x1801dc6e3],0x0
1800bafaf:  44 0F 28 E3              MOVAPS XMM12,XMM3
1800bafb3:  44 0F 28 EA              MOVAPS XMM13,XMM2
1800bafb7:  44 0F 28 F1              MOVAPS XMM14,XMM1
1800bafbb:  48 8B F9                 MOV RDI,RCX
1800bafbe:  75 10                    JNZ 0x1800bafd0
1800bafc0:  48 8B 89 80 02 00 00     MOV RCX,qword ptr [RCX + 0x280]
1800bafc7:  83 CA FF                 OR EDX,0xffffffff
1800bafca:  48 8B 01                 MOV RAX,qword ptr [RCX]
1800bafcd:  FF 50 10                 CALL qword ptr [RAX + 0x10]
1800bafd0:  F3 44 0F 10 87 88 02 00 00 MOVSS XMM8,dword ptr [RDI + 0x288]
1800bafd9:  48 8D 97 C4 02 00 00     LEA RDX,[RDI + 0x2c4]
1800bafe0:  F3 44 0F 10 9F 8C 02 00 00 MOVSS XMM11,dword ptr [RDI + 0x28c]
1800bafe9:  45 33 ED                 XOR R13D,R13D
1800bafec:  F3 44 0F 59 05 97 DB 0C 00 MULSS XMM8,dword ptr [0x180188b8c]
1800baff5:  48 8B CF                 MOV RCX,RDI
1800baff8:  44 89 AF CC 02 00 00     MOV dword ptr [RDI + 0x2cc],R13D
1800bafff:  F3 44 0F 59 1D 84 DB 0C 00 MULSS XMM11,dword ptr [0x180188b8c]
1800bb008:  45 0F 28 D0              MOVAPS XMM10,XMM8
1800bb00c:  44 0F 57 15 5C E0 0C 00  XORPS XMM10,xmmword ptr [0x180189070]
1800bb014:  F3 44 0F 11 97 C4 02 00 00 MOVSS dword ptr [RDI + 0x2c4],XMM10
1800bb01d:  F3 44 0F 11 9F C8 02 00 00 MOVSS dword ptr [RDI + 0x2c8],XMM11
1800bb026:  E8 D5 DA FF FF           CALL 0x1800b8b00
1800bb02b:  F3 0F 10 8F C4 02 00 00  MOVSS XMM1,dword ptr [RDI + 0x2c4]
1800bb033:  48 8D B7 FC 01 00 00     LEA RSI,[RDI + 0x1fc]
1800bb03a:  F3 0F 10 87 C8 02 00 00  MOVSS XMM0,dword ptr [RDI + 0x2c8]
1800bb042:  48 8D 97 D0 02 00 00     LEA RDX,[RDI + 0x2d0]
1800bb049:  F3 44 0F 10 4D 78        MOVSS XMM9,dword ptr [RBP + 0x78]
1800bb04f:  48 8B CF                 MOV RCX,RDI
1800bb052:  F3 0F 10 75 70           MOVSS XMM6,dword ptr [RBP + 0x70]
1800bb057:  F3 0F 11 8F F8 01 00 00  MOVSS dword ptr [RDI + 0x1f8],XMM1
1800bb05f:  F3 0F 11 06              MOVSS dword ptr [RSI],XMM0
1800bb063:  F3 0F 11 8F 40 02 00 00  MOVSS dword ptr [RDI + 0x240],XMM1
1800bb06b:  F3 0F 11 87 44 02 00 00  MOVSS dword ptr [RDI + 0x244],XMM0
1800bb073:  F3 44 0F 11 8F 00 02 00 00 MOVSS dword ptr [RDI + 0x200],XMM9
1800bb07c:  F3 44 0F 11 B7 38 02 00 00 MOVSS dword ptr [RDI + 0x238],XMM14
1800bb085:  F3 0F 11 B7 3C 02 00 00  MOVSS dword ptr [RDI + 0x23c],XMM6
1800bb08d:  F3 44 0F 11 87 D0 02 00 00 MOVSS dword ptr [RDI + 0x2d0],XMM8
1800bb096:  F3 44 0F 11 9F D4 02 00 00 MOVSS dword ptr [RDI + 0x2d4],XMM11
1800bb09f:  44 89 AF D8 02 00 00     MOV dword ptr [RDI + 0x2d8],R13D
1800bb0a6:  E8 55 DA FF FF           CALL 0x1800b8b00
1800bb0ab:  F3 0F 10 8F D0 02 00 00  MOVSS XMM1,dword ptr [RDI + 0x2d0]
1800bb0b3:  48 8D 97 DC 02 00 00     LEA RDX,[RDI + 0x2dc]
1800bb0ba:  F3 0F 10 87 D4 02 00 00  MOVSS XMM0,dword ptr [RDI + 0x2d4]
1800bb0c2:  48 8B CF                 MOV RCX,RDI
1800bb0c5:  44 0F 57 1D A3 DF 0C 00  XORPS XMM11,xmmword ptr [0x180189070]
1800bb0cd:  F3 0F 11 8F 08 02 00 00  MOVSS dword ptr [RDI + 0x208],XMM1
1800bb0d5:  F3 0F 11 87 0C 02 00 00  MOVSS dword ptr [RDI + 0x20c],XMM0
1800bb0dd:  F3 0F 11 8F 50 02 00 00  MOVSS dword ptr [RDI + 0x250],XMM1
1800bb0e5:  F3 0F 11 87 54 02 00 00  MOVSS dword ptr [RDI + 0x254],XMM0
1800bb0ed:  F3 44 0F 11 8F 10 02 00 00 MOVSS dword ptr [RDI + 0x210],XMM9
1800bb0f6:  F3 44 0F 11 A7 48 02 00 00 MOVSS dword ptr [RDI + 0x248],XMM12
1800bb0ff:  F3 0F 11 B7 4C 02 00 00  MOVSS dword ptr [RDI + 0x24c],XMM6
1800bb107:  F3 44 0F 11 87 DC 02 00 00 MOVSS dword ptr [RDI + 0x2dc],XMM8
1800bb110:  F3 44 0F 11 9F E0 02 00 00 MOVSS dword ptr [RDI + 0x2e0],XMM11
1800bb119:  44 89 AF E4 02 00 00     MOV dword ptr [RDI + 0x2e4],R13D
1800bb120:  E8 DB D9 FF FF           CALL 0x1800b8b00
1800bb125:  F3 0F 10 8F DC 02 00 00  MOVSS XMM1,dword ptr [RDI + 0x2dc]
1800bb12d:  48 8D 97 E8 02 00 00     LEA RDX,[RDI + 0x2e8]
1800bb134:  F3 0F 10 87 E0 02 00 00  MOVSS XMM0,dword ptr [RDI + 0x2e0]
1800bb13c:  48 8B CF                 MOV RCX,RDI
1800bb13f:  F3 0F 11 8F 28 02 00 00  MOVSS dword ptr [RDI + 0x228],XMM1
1800bb147:  F3 0F 11 87 2C 02 00 00  MOVSS dword ptr [RDI + 0x22c],XMM0
1800bb14f:  F3 0F 11 8F 70 02 00 00  MOVSS dword ptr [RDI + 0x270],XMM1
1800bb157:  F3 0F 11 87 74 02 00 00  MOVSS dword ptr [RDI + 0x274],XMM0
1800bb15f:  F3 44 0F 11 8F 30 02 00 00 MOVSS dword ptr [RDI + 0x230],XMM9
1800bb168:  F3 44 0F 11 A7 68 02 00 00 MOVSS dword ptr [RDI + 0x268],XMM12
1800bb171:  F3 44 0F 11 AF 6C 02 00 00 MOVSS dword ptr [RDI + 0x26c],XMM13
1800bb17a:  F3 44 0F 11 97 E8 02 00 00 MOVSS dword ptr [RDI + 0x2e8],XMM10
1800bb183:  F3 44 0F 11 9F EC 02 00 00 MOVSS dword ptr [RDI + 0x2ec],XMM11
1800bb18c:  44 89 AF F0 02 00 00     MOV dword ptr [RDI + 0x2f0],R13D
1800bb193:  E8 68 D9 FF FF           CALL 0x1800b8b00
1800bb198:  F3 0F 10 97 E8 02 00 00  MOVSS XMM2,dword ptr [RDI + 0x2e8]
1800bb1a0:  F3 0F 10 A7 EC 02 00 00  MOVSS XMM4,dword ptr [RDI + 0x2ec]
1800bb1a8:  F3 0F 11 97 18 02 00 00  MOVSS dword ptr [RDI + 0x218],XMM2
1800bb1b0:  F3 0F 11 A7 1C 02 00 00  MOVSS dword ptr [RDI + 0x21c],XMM4
1800bb1b8:  F3 0F 11 97 60 02 00 00  MOVSS dword ptr [RDI + 0x260],XMM2
1800bb1c0:  F3 0F 11 A7 64 02 00 00  MOVSS dword ptr [RDI + 0x264],XMM4
1800bb1c8:  F3 44 0F 11 8F 20 02 00 00 MOVSS dword ptr [RDI + 0x220],XMM9
1800bb1d1:  F3 44 0F 11 B7 58 02 00 00 MOVSS dword ptr [RDI + 0x258],XMM14
1800bb1da:  F3 44 0F 11 AF 5C 02 00 00 MOVSS dword ptr [RDI + 0x25c],XMM13
1800bb1e3:  8B 8F 28 67 00 00        MOV ECX,dword ptr [RDI + 0x6728]
1800bb1e9:  85 C9                    TEST ECX,ECX
1800bb1eb:  0F 84 81 00 00 00        JZ 0x1800bb272
1800bb1f1:  8B 87 2C 67 00 00        MOV EAX,dword ptr [RDI + 0x672c]
1800bb1f7:  85 C0                    TEST EAX,EAX
1800bb1f9:  74 77                    JZ 0x1800bb272
1800bb1fb:  F3 0F 10 8F A8 02 00 00  MOVSS XMM1,dword ptr [RDI + 0x2a8]

