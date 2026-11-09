; Range: 0x140001a50 to 0x140001b80
140001a50:  48 83 EC 28              SUB RSP,0x28
140001a54:  4C 8B CA                 MOV R9,RDX
140001a57:  48 83 C8 FF              OR RAX,-0x1
140001a5b:  33 D2                    XOR EDX,EDX
140001a5d:  49 F7 F0                 DIV R8
140001a60:  4C 3B C8                 CMP R9,RAX
140001a63:  76 07                    JBE 0x140001a6c
140001a65:  FF 15 FD C3 86 00        CALL qword ptr [0x14086de68]
140001a6c:  4D 0F AF C8              IMUL R9,R8
140001a70:  49 81 F9 00 10 00 00     CMP R9,0x1000
140001a77:  72 3C                    JC 0x140001ab5
140001a79:  F6 C1 1F                 TEST CL,0x1f
140001a7c:  74 07                    JZ 0x140001a85
140001a7e:  FF 15 E4 C3 86 00        CALL qword ptr [0x14086de68]
140001a85:  48 8B 41 F8              MOV RAX,qword ptr [RCX + -0x8]
140001a89:  48 3B C1                 CMP RAX,RCX
140001a8c:  72 07                    JC 0x140001a95
140001a8e:  FF 15 D4 C3 86 00        CALL qword ptr [0x14086de68]
140001a95:  48 2B C8                 SUB RCX,RAX
140001a98:  48 83 F9 08              CMP RCX,0x8
140001a9c:  73 07                    JNC 0x140001aa5
140001a9e:  FF 15 C4 C3 86 00        CALL qword ptr [0x14086de68]
140001aa5:  48 83 F9 27              CMP RCX,0x27
140001aa9:  76 07                    JBE 0x140001ab2
140001aab:  FF 15 B7 C3 86 00        CALL qword ptr [0x14086de68]
140001ab2:  48 8B C8                 MOV RCX,RAX
140001ab5:  48 83 C4 28              ADD RSP,0x28
140001ab9:  E9 1A C5 84 00           JMP 0x14084dfd8
140001ac0:  40 53                    PUSH RBX
140001ac2:  48 83 EC 20              SUB RSP,0x20
140001ac6:  48 8B D9                 MOV RBX,RCX
140001ac9:  48 8B 09                 MOV RCX,qword ptr [RCX]
140001acc:  48 85 C9                 TEST RCX,RCX
140001acf:  74 0D                    JZ 0x140001ade
140001ad1:  E8 0A C5 84 00           CALL 0x14084dfe0
140001ad6:  33 C0                    XOR EAX,EAX
140001ad8:  48 89 03                 MOV qword ptr [RBX],RAX
140001adb:  89 43 08                 MOV dword ptr [RBX + 0x8],EAX
140001ade:  48 83 C4 20              ADD RSP,0x20
140001ae2:  5B                       POP RBX
140001ae3:  C3                       RET
140001af0:  4D 85 C0                 TEST R8,R8
140001af3:  74 42                    JZ 0x140001b37
140001af5:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
140001afa:  48 89 6C 24 10           MOV qword ptr [RSP + 0x10],RBP
140001aff:  48 89 74 24 18           MOV qword ptr [RSP + 0x18],RSI
140001b04:  57                       PUSH RDI
140001b05:  48 83 EC 20              SUB RSP,0x20
140001b09:  49 8B F1                 MOV RSI,R9
140001b0c:  49 8B D8                 MOV RBX,R8
140001b0f:  48 8B EA                 MOV RBP,RDX
140001b12:  48 8B F9                 MOV RDI,RCX
140001b15:  48 8B CF                 MOV RCX,RDI
140001b18:  FF D6                    CALL RSI
140001b1a:  48 03 FD                 ADD RDI,RBP
140001b1d:  48 83 EB 01              SUB RBX,0x1
140001b21:  75 F2                    JNZ 0x140001b15
140001b23:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
140001b28:  48 8B 6C 24 38           MOV RBP,qword ptr [RSP + 0x38]
140001b2d:  48 8B 74 24 40           MOV RSI,qword ptr [RSP + 0x40]
140001b32:  48 83 C4 20              ADD RSP,0x20
140001b36:  5F                       POP RDI
140001b37:  C3                       RET
140001b40:  48 89 6C 24 10           MOV qword ptr [RSP + 0x10],RBP
140001b45:  48 89 74 24 18           MOV qword ptr [RSP + 0x18],RSI
140001b4a:  57                       PUSH RDI
140001b4b:  41 56                    PUSH R14
140001b4d:  41 57                    PUSH R15
140001b4f:  48 83 EC 60              SUB RSP,0x60
140001b53:  48 8B 41 30              MOV RAX,qword ptr [RCX + 0x30]
140001b57:  33 ED                    XOR EBP,EBP
140001b59:  48 89 41 38              MOV qword ptr [RCX + 0x38],RAX
140001b5d:  49 BF 67 66 66 66 66 66 66 66 MOV R15,0x6666666666666667
140001b67:  48 8B 51 20              MOV RDX,qword ptr [RCX + 0x20]
140001b6b:  49 8B C7                 MOV RAX,R15
140001b6e:  48 2B 51 18              SUB RDX,qword ptr [RCX + 0x18]
140001b72:  48 8B F9                 MOV RDI,RCX
140001b75:  48 F7 EA                 IMUL RDX
140001b78:  8B F5                    MOV ESI,EBP
140001b7a:  48 C1 FA 04              SAR RDX,0x4
140001b7e:  48 8B C2                 MOV RAX,RDX

