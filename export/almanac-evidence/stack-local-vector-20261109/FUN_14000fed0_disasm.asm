; Range: 0x14000fed0 to 0x140010000
14000fed0:  40 53                    PUSH RBX
14000fed2:  48 83 EC 20              SUB RSP,0x20
14000fed6:  48 8B D9                 MOV RBX,RCX
14000fed9:  48 8B 09                 MOV RCX,qword ptr [RCX]
14000fedc:  48 85 C9                 TEST RCX,RCX
14000fedf:  74 23                    JZ 0x14000ff04
14000fee1:  48 8B 53 10              MOV RDX,qword ptr [RBX + 0x10]
14000fee5:  41 B8 10 00 00 00        MOV R8D,0x10
14000feeb:  48 2B D1                 SUB RDX,RCX
14000feee:  48 C1 FA 04              SAR RDX,0x4
14000fef2:  E8 59 1B FF FF           CALL 0x140001a50
14000fef7:  33 C0                    XOR EAX,EAX
14000fef9:  48 89 03                 MOV qword ptr [RBX],RAX
14000fefc:  48 89 43 08              MOV qword ptr [RBX + 0x8],RAX
14000ff00:  48 89 43 10              MOV qword ptr [RBX + 0x10],RAX
14000ff04:  48 83 C4 20              ADD RSP,0x20
14000ff08:  5B                       POP RBX
14000ff09:  C3                       RET
14000ff10:  48 83 EC 28              SUB RSP,0x28
14000ff14:  E8 95 F5 83 00           CALL 0x14084f4ae
14000ff19:  48 83 C4 28              ADD RSP,0x28
14000ff1d:  C3                       RET
14000ff20:  48 83 EC 28              SUB RSP,0x28
14000ff24:  E8 C1 F5 83 00           CALL 0x14084f4ea
14000ff29:  48 83 C4 28              ADD RSP,0x28
14000ff2d:  C3                       RET
14000ff30:  48 83 EC 28              SUB RSP,0x28
14000ff34:  E8 E1 F5 83 00           CALL 0x14084f51a
14000ff39:  48 83 C4 28              ADD RSP,0x28
14000ff3d:  C3                       RET
14000ff40:  48 81 EC B8 00 00 00     SUB RSP,0xb8
14000ff47:  44 0F 29 54 24 60        MOVAPS xmmword ptr [RSP + 0x60],XMM10
14000ff4d:  33 C0                    XOR EAX,EAX
14000ff4f:  44 0F 29 5C 24 50        MOVAPS xmmword ptr [RSP + 0x50],XMM11
14000ff55:  44 0F 29 64 24 40        MOVAPS xmmword ptr [RSP + 0x40],XMM12
14000ff5b:  44 0F 29 6C 24 30        MOVAPS xmmword ptr [RSP + 0x30],XMM13
14000ff61:  41 83 F9 01              CMP R9D,0x1
14000ff65:  75 1E                    JNZ 0x14000ff85
14000ff67:  F3 44 0F 10 12           MOVSS XMM10,dword ptr [RDX]
14000ff6c:  F3 44 0F 10 5A 04        MOVSS XMM11,dword ptr [RDX + 0x4]
14000ff72:  F3 44 0F 10 21           MOVSS XMM12,dword ptr [RCX]
14000ff77:  F3 44 0F 10 69 04        MOVSS XMM13,dword ptr [RCX + 0x4]
14000ff7d:  89 41 08                 MOV dword ptr [RCX + 0x8],EAX
14000ff80:  89 42 08                 MOV dword ptr [RDX + 0x8],EAX
14000ff83:  EB 40                    JMP 0x14000ffc5
14000ff85:  41 83 F9 02              CMP R9D,0x2
14000ff89:  75 1E                    JNZ 0x14000ffa9
14000ff8b:  F3 44 0F 10 52 08        MOVSS XMM10,dword ptr [RDX + 0x8]
14000ff91:  F3 44 0F 10 5A 04        MOVSS XMM11,dword ptr [RDX + 0x4]
14000ff97:  F3 44 0F 10 61 08        MOVSS XMM12,dword ptr [RCX + 0x8]
14000ff9d:  F3 44 0F 10 69 04        MOVSS XMM13,dword ptr [RCX + 0x4]
14000ffa3:  89 01                    MOV dword ptr [RCX],EAX
14000ffa5:  89 02                    MOV dword ptr [RDX],EAX
14000ffa7:  EB 1C                    JMP 0x14000ffc5
14000ffa9:  F3 44 0F 10 12           MOVSS XMM10,dword ptr [RDX]
14000ffae:  F3 44 0F 10 5A 08        MOVSS XMM11,dword ptr [RDX + 0x8]
14000ffb4:  F3 44 0F 10 21           MOVSS XMM12,dword ptr [RCX]
14000ffb9:  F3 44 0F 10 69 08        MOVSS XMM13,dword ptr [RCX + 0x8]
14000ffbf:  89 41 04                 MOV dword ptr [RCX + 0x4],EAX
14000ffc2:  89 42 04                 MOV dword ptr [RDX + 0x4],EAX
14000ffc5:  45 84 C0                 TEST R8B,R8B
14000ffc8:  74 39                    JZ 0x140010003
14000ffca:  41 0F 28 C2              MOVAPS XMM0,XMM10
14000ffce:  41 0F 28 CB              MOVAPS XMM1,XMM11
14000ffd2:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14000ffd7:  F3 41 0F 59 CD           MULSS XMM1,XMM13
14000ffdc:  F3 0F 58 C8              ADDSS XMM1,XMM0
14000ffe0:  F3 0F 10 05 70 9F 8F 00  MOVSS XMM0,dword ptr [0x140909f58]
14000ffe8:  0F 2F C8                 COMISS XMM1,XMM0
14000ffeb:  77 0C                    JA 0x14000fff9
14000ffed:  F3 0F 10 05 0B B3 8F 00  MOVSS XMM0,dword ptr [0x14090b300]
14000fff5:  F3 0F 5F C1              MAXSS XMM0,XMM1
14000fff9:  E8 B0 F4 83 00           CALL 0x14084f4ae
14000fffe:  E9 E5 00 00 00           JMP 0x1400100e8

