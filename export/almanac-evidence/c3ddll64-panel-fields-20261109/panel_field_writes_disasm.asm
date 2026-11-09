; Range: 0x18002DEE0 to 0x18002DF20
18002dee3:  8B 82 3C 67 00 00        MOV EAX,dword ptr [RDX + 0x673c]
18002dee9:  41 89 81 3C 67 00 00     MOV dword ptr [R9 + 0x673c],EAX
18002def0:  8B 82 40 67 00 00        MOV EAX,dword ptr [RDX + 0x6740]
18002def6:  41 89 81 40 67 00 00     MOV dword ptr [R9 + 0x6740],EAX
18002defd:  8B 82 44 67 00 00        MOV EAX,dword ptr [RDX + 0x6744]
18002df03:  41 89 81 44 67 00 00     MOV dword ptr [R9 + 0x6744],EAX
18002df0a:  8B 82 48 67 00 00        MOV EAX,dword ptr [RDX + 0x6748]
18002df10:  41 89 81 48 67 00 00     MOV dword ptr [R9 + 0x6748],EAX
18002df17:  8B 82 4C 67 00 00        MOV EAX,dword ptr [RDX + 0x674c]
18002df1d:  41 89 81 4C 67 00 00     MOV dword ptr [R9 + 0x674c],EAX

; Range: 0x18005BED0 to 0x18005BF10
18005bed0:  49 C7 84 24 3C 67 00 00 00 00 80 3F MOV qword ptr [R12 + 0x673c],0x3f800000
18005bedc:  49 C7 84 24 44 67 00 00 28 6B 6E 4E MOV qword ptr [R12 + 0x6744],0x4e6e6b28
18005bee8:  49 C7 84 24 28 67 00 00 00 00 00 00 MOV qword ptr [R12 + 0x6728],0x0
18005bef4:  41 C6 84 24 60 67 00 00 01 MOV byte ptr [R12 + 0x6760],0x1
18005befd:  41 0F 14 C7              UNPCKLPS XMM0,XMM15
18005bf01:  F2 41 0F 11 84 24 30 67 00 00 MOVSD qword ptr [R12 + 0x6730],XMM0
18005bf0b:  C7 85 AC 00 00 00 00 00 00 00 MOV dword ptr [RBP + 0xac],0x0

; Range: 0x1800B8AD0 to 0x1800B8C00
1800b8ad3:  48 C7 81 3C 67 00 00 00 00 80 3F MOV qword ptr [RCX + 0x673c],0x3f800000
1800b8ade:  48 C7 81 44 67 00 00 28 6B 6E 4E MOV qword ptr [RCX + 0x6744],0x4e6e6b28
1800b8ae9:  48 89 91 28 67 00 00     MOV qword ptr [RCX + 0x6728],RDX
1800b8af0:  C6 81 60 67 00 00 01     MOV byte ptr [RCX + 0x6760],0x1
1800b8af7:  89 54 24 08              MOV dword ptr [RSP + 0x8],EDX
1800b8afb:  48 83 C4 18              ADD RSP,0x18
1800b8aff:  C3                       RET
1800b8b00:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
1800b8b05:  57                       PUSH RDI
1800b8b06:  48 83 EC 30              SUB RSP,0x30
1800b8b0a:  48 8B FA                 MOV RDI,RDX
1800b8b0d:  48 8B D9                 MOV RBX,RCX
1800b8b10:  E8 6B 01 00 00           CALL 0x1800b8c80
1800b8b15:  F3 0F 10 A3 3C 67 00 00  MOVSS XMM4,dword ptr [RBX + 0x673c]
1800b8b1d:  0F 2F 25 C4 00 0D 00     COMISS XMM4,dword ptr [0x180188be8]
1800b8b24:  76 72                    JBE 0x1800b8b98
1800b8b26:  F3 0F 10 9B 30 67 00 00  MOVSS XMM3,dword ptr [RBX + 0x6730]
1800b8b2e:  F3 0F 10 05 3A 05 0D 00  MOVSS XMM0,dword ptr [0x180189070]
1800b8b36:  F3 0F 10 93 34 67 00 00  MOVSS XMM2,dword ptr [RBX + 0x6734]
1800b8b3e:  0F 57 D8                 XORPS XMM3,XMM0
1800b8b41:  F3 0F 58 1F              ADDSS XMM3,dword ptr [RDI]
1800b8b45:  F3 0F 10 8B 38 67 00 00  MOVSS XMM1,dword ptr [RBX + 0x6738]
1800b8b4d:  0F 57 D0                 XORPS XMM2,XMM0
1800b8b50:  F3 0F 58 57 04           ADDSS XMM2,dword ptr [RDI + 0x4]
1800b8b55:  0F 57 C8                 XORPS XMM1,XMM0
1800b8b58:  F3 0F 58 4F 08           ADDSS XMM1,dword ptr [RDI + 0x8]
1800b8b5d:  F3 0F 59 DC              MULSS XMM3,XMM4
1800b8b61:  F3 0F 59 D4              MULSS XMM2,XMM4
1800b8b65:  F3 0F 58 9B 30 67 00 00  ADDSS XMM3,dword ptr [RBX + 0x6730]
1800b8b6d:  F3 0F 59 CC              MULSS XMM1,XMM4
1800b8b71:  F3 0F 58 93 34 67 00 00  ADDSS XMM2,dword ptr [RBX + 0x6734]
1800b8b79:  F3 0F 58 8B 38 67 00 00  ADDSS XMM1,dword ptr [RBX + 0x6738]
1800b8b81:  0F 28 C3                 MOVAPS XMM0,XMM3
1800b8b84:  0F 14 C2                 UNPCKLPS XMM0,XMM2
1800b8b87:  F2 0F 11 07              MOVSD qword ptr [RDI],XMM0
1800b8b8b:  F3 0F 11 4C 24 28        MOVSS dword ptr [RSP + 0x28],XMM1
1800b8b91:  8B 44 24 28              MOV EAX,dword ptr [RSP + 0x28]
1800b8b95:  89 47 08                 MOV dword ptr [RDI + 0x8],EAX
1800b8b98:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
1800b8b9d:  48 83 C4 30              ADD RSP,0x30
1800b8ba1:  5F                       POP RDI
1800b8ba2:  C3                       RET
1800b8bb0:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
1800b8bb5:  57                       PUSH RDI
1800b8bb6:  48 83 EC 30              SUB RSP,0x30
1800b8bba:  F3 41 0F 10 00           MOVSS XMM0,dword ptr [R8]
1800b8bbf:  48 8B FA                 MOV RDI,RDX
1800b8bc2:  F3 41 0F 10 48 04        MOVSS XMM1,dword ptr [R8 + 0x4]
1800b8bc8:  48 8B D9                 MOV RBX,RCX
1800b8bcb:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
1800b8bd1:  F3 41 0F 10 40 08        MOVSS XMM0,dword ptr [R8 + 0x8]
1800b8bd7:  4C 8D 44 24 20           LEA R8,[RSP + 0x20]
1800b8bdc:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
1800b8be2:  F3 0F 11 4C 24 24        MOVSS dword ptr [RSP + 0x24],XMM1
1800b8be8:  E8 33 03 00 00           CALL 0x1800b8f20
1800b8bed:  F3 0F 10 A3 3C 67 00 00  MOVSS XMM4,dword ptr [RBX + 0x673c]
1800b8bf5:  0F 2F 25 EC FF 0C 00     COMISS XMM4,dword ptr [0x180188be8]
1800b8bfc:  76 72                    JBE 0x1800b8c70
1800b8bfe:  F3 0F 10 9B 30 67 00 00  MOVSS XMM3,dword ptr [RBX + 0x6730]

