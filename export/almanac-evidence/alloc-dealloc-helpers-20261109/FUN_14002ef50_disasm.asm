; Range: 0x14002ef50 to 0x14002f100
14002ef50:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
14002ef55:  48 89 6C 24 10           MOV qword ptr [RSP + 0x10],RBP
14002ef5a:  48 89 74 24 18           MOV qword ptr [RSP + 0x18],RSI
14002ef5f:  57                       PUSH RDI
14002ef60:  48 83 EC 30              SUB RSP,0x30
14002ef64:  48 8B F2                 MOV RSI,RDX
14002ef67:  48 8B F9                 MOV RDI,RCX
14002ef6a:  48 8B CE                 MOV RCX,RSI
14002ef6d:  BA 10 00 00 00           MOV EDX,0x10
14002ef72:  E8 69 2A FD FF           CALL 0x1400019e0
14002ef77:  48 8B 57 08              MOV RDX,qword ptr [RDI + 0x8]
14002ef7b:  4C 8B C0                 MOV R8,RAX
14002ef7e:  48 8B 0F                 MOV RCX,qword ptr [RDI]
14002ef81:  48 8B E8                 MOV RBP,RAX
14002ef84:  E8 37 04 00 00           CALL 0x14002f3c0
14002ef89:  48 8B 0F                 MOV RCX,qword ptr [RDI]
14002ef8c:  48 8B 5F 08              MOV RBX,qword ptr [RDI + 0x8]
14002ef90:  48 2B D9                 SUB RBX,RCX
14002ef93:  48 85 C9                 TEST RCX,RCX
14002ef96:  74 16                    JZ 0x14002efae
14002ef98:  48 8B 57 10              MOV RDX,qword ptr [RDI + 0x10]
14002ef9c:  41 B8 10 00 00 00        MOV R8D,0x10
14002efa2:  48 2B D1                 SUB RDX,RCX
14002efa5:  48 C1 FA 04              SAR RDX,0x4
14002efa9:  E8 A2 2A FD FF           CALL 0x140001a50
14002efae:  48 C1 E6 04              SHL RSI,0x4
14002efb2:  48 83 E3 F0              AND RBX,-0x10
14002efb6:  48 03 F5                 ADD RSI,RBP
14002efb9:  48 03 DD                 ADD RBX,RBP
14002efbc:  48 89 77 10              MOV qword ptr [RDI + 0x10],RSI
14002efc0:  48 8B 74 24 50           MOV RSI,qword ptr [RSP + 0x50]
14002efc5:  48 89 5F 08              MOV qword ptr [RDI + 0x8],RBX
14002efc9:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
14002efce:  48 89 2F                 MOV qword ptr [RDI],RBP
14002efd1:  48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
14002efd6:  48 83 C4 30              ADD RSP,0x30
14002efda:  5F                       POP RDI
14002efdb:  C3                       RET
14002efe0:  40 53                    PUSH RBX
14002efe2:  48 83 EC 20              SUB RSP,0x20
14002efe6:  4C 8B C9                 MOV R9,RCX
14002efe9:  48 BB AB AA AA AA AA AA AA 2A MOV RBX,0x2aaaaaaaaaaaaaab
14002eff3:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
14002eff7:  48 8B C3                 MOV RAX,RBX
14002effa:  48 8B D1                 MOV RDX,RCX
14002effd:  4D 8B 41 08              MOV R8,qword ptr [R9 + 0x8]
14002f001:  49 2B D0                 SUB RDX,R8
14002f004:  48 F7 EA                 IMUL RDX
14002f007:  48 C1 FA 02              SAR RDX,0x2
14002f00b:  48 8B C2                 MOV RAX,RDX
14002f00e:  48 C1 E8 3F              SHR RAX,0x3f
14002f012:  48 03 D0                 ADD RDX,RAX
14002f015:  48 83 FA 01              CMP RDX,0x1
14002f019:  0F 83 85 00 00 00        JNC 0x14002f0a4
14002f01f:  4D 8B 19                 MOV R11,qword ptr [R9]
14002f022:  48 8B C3                 MOV RAX,RBX
14002f025:  4D 2B C3                 SUB R8,R11
14002f028:  49 BA AA AA AA AA AA AA AA 0A MOV R10,0xaaaaaaaaaaaaaaa
14002f032:  49 F7 E8                 IMUL R8
14002f035:  48 C1 FA 02              SAR RDX,0x2
14002f039:  48 8B C2                 MOV RAX,RDX
14002f03c:  48 C1 E8 3F              SHR RAX,0x3f
14002f040:  48 03 D0                 ADD RDX,RAX
14002f043:  49 8B C2                 MOV RAX,R10
14002f046:  48 2B C2                 SUB RAX,RDX
14002f049:  48 83 F8 01              CMP RAX,0x1
14002f04d:  73 0E                    JNC 0x14002f05d
14002f04f:  48 8D 0D 9A 14 84 00     LEA RCX,[0x1408704f0]
14002f056:  FF 15 CC E9 83 00        CALL qword ptr [0x14086da28]
14002f05c:  CC                       INT3
14002f05d:  49 2B CB                 SUB RCX,R11
14002f060:  4C 8D 42 01              LEA R8,[RDX + 0x1]
14002f064:  48 8B C3                 MOV RAX,RBX
14002f067:  48 F7 E9                 IMUL RCX
14002f06a:  33 C9                    XOR ECX,ECX
14002f06c:  48 C1 FA 02              SAR RDX,0x2
14002f070:  48 8B C2                 MOV RAX,RDX
14002f073:  48 C1 E8 3F              SHR RAX,0x3f
14002f077:  48 03 D0                 ADD RDX,RAX
14002f07a:  48 8B C2                 MOV RAX,RDX
14002f07d:  48 D1 E8                 SHR RAX,0x1
14002f080:  4C 2B D0                 SUB R10,RAX
14002f083:  48 03 C2                 ADD RAX,RDX
14002f086:  4C 3B D2                 CMP R10,RDX
14002f089:  48 0F 43 C8              CMOVNC RCX,RAX
14002f08d:  49 3B C8                 CMP RCX,R8
14002f090:  4C 0F 43 C1              CMOVNC R8,RCX
14002f094:  49 8B C9                 MOV RCX,R9
14002f097:  49 8B D0                 MOV RDX,R8
14002f09a:  48 83 C4 20              ADD RSP,0x20
14002f09e:  5B                       POP RBX
14002f09f:  E9 FC 00 00 00           JMP 0x14002f1a0
14002f0a4:  48 83 C4 20              ADD RSP,0x20
14002f0a8:  5B                       POP RBX
14002f0a9:  C3                       RET
14002f0b0:  49 8B C0                 MOV RAX,R8
14002f0b3:  48 8B CA                 MOV RCX,RDX
14002f0b6:  48 8B D0                 MOV RDX,RAX
14002f0b9:  41 B8 0C 00 00 00        MOV R8D,0xc
14002f0bf:  E9 8C 29 FD FF           JMP 0x140001a50
14002f0d0:  48 89 54 24 10           MOV qword ptr [RSP + 0x10],RDX
14002f0d5:  56                       PUSH RSI
14002f0d6:  57                       PUSH RDI
14002f0d7:  41 56                    PUSH R14
14002f0d9:  48 83 EC 40              SUB RSP,0x40
14002f0dd:  48 C7 44 24 30 FE FF FF FF MOV qword ptr [RSP + 0x30],-0x2
14002f0e6:  48 89 5C 24 70           MOV qword ptr [RSP + 0x70],RBX
14002f0eb:  4C 8B F2                 MOV R14,RDX
14002f0ee:  48 8B D9                 MOV RBX,RCX
14002f0f1:  BA 0C 00 00 00           MOV EDX,0xc
14002f0f6:  49 8B CE                 MOV RCX,R14
14002f0f9:  E8 E2 28 FD FF           CALL 0x1400019e0
14002f0fe:  48 8B F8                 MOV RDI,RAX

