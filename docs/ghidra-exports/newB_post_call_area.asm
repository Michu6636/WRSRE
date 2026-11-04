; Range: 0x14066EB00 to 0x14066EC20
14066eb02:  4C 8B 83 28 05 00 00     MOV R8,qword ptr [RBX + 0x528]
14066eb09:  4D 85 C0                 TEST R8,R8
14066eb0c:  0F 84 53 01 00 00        JZ 0x14066ec65
14066eb12:  48 8B 8B 40 07 00 00     MOV RCX,qword ptr [RBX + 0x740]
14066eb19:  48 BE AB AA AA AA AA AA AA 2A MOV RSI,0x2aaaaaaaaaaaaaab
14066eb23:  48 2B 8B 38 07 00 00     SUB RCX,qword ptr [RBX + 0x738]
14066eb2a:  48 8B C6                 MOV RAX,RSI
14066eb2d:  48 F7 E9                 IMUL RCX
14066eb30:  48 D1 FA                 SAR RDX,0x1
14066eb33:  48 8B C2                 MOV RAX,RDX
14066eb36:  48 C1 E8 3F              SHR RAX,0x3f
14066eb3a:  48 03 D0                 ADD RDX,RAX
14066eb3d:  39 93 68 07 00 00        CMP dword ptr [RBX + 0x768],EDX
14066eb43:  0F 8C 1C 01 00 00        JL 0x14066ec65
14066eb49:  4D 8B 90 D0 05 00 00     MOV R10,qword ptr [R8 + 0x5d0]
14066eb50:  4D 85 D2                 TEST R10,R10
14066eb53:  0F 84 0C 01 00 00        JZ 0x14066ec65
14066eb59:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066eb60:  83 B8 00 86 00 00 02     CMP dword ptr [RAX + 0x8600],0x2
14066eb67:  0F 85 F8 00 00 00        JNZ 0x14066ec65
14066eb6d:  49 39 98 B0 0B 00 00     CMP qword ptr [R8 + 0xbb0],RBX
14066eb74:  0F 85 EB 00 00 00        JNZ 0x14066ec65
14066eb7a:  49 8B 88 18 06 00 00     MOV RCX,qword ptr [R8 + 0x618]
14066eb81:  48 BF DD 0B A2 D3 9C 6D 5E 1E MOV RDI,0x1e5e6d9cd3a20bdd
14066eb8b:  49 2B 88 10 06 00 00     SUB RCX,qword ptr [R8 + 0x610]
14066eb92:  48 8B C7                 MOV RAX,RDI
14066eb95:  48 F7 E9                 IMUL RCX
14066eb98:  48 C1 FA 0A              SAR RDX,0xa
14066eb9c:  48 8B C2                 MOV RAX,RDX
14066eb9f:  48 C1 E8 3F              SHR RAX,0x3f
14066eba3:  48 03 D0                 ADD RDX,RAX
14066eba6:  0F 84 B9 00 00 00        JZ 0x14066ec65
14066ebac:  F3 41 0F 10 88 04 06 00 00 MOVSS XMM1,dword ptr [R8 + 0x604]
14066ebb5:  4D 8B CF                 MOV R9,R15
14066ebb8:  4D 8B 98 10 06 00 00     MOV R11,qword ptr [R8 + 0x610]
14066ebbf:  90                       NOP
14066ebc0:  49 69 C1 B8 21 00 00     IMUL RAX,R9,0x21b8
14066ebc7:  F3 42 0F 10 04 18        MOVSS XMM0,dword ptr [RAX + R11*0x1]
14066ebcd:  0F 2F C1                 COMISS XMM0,XMM1
14066ebd0:  77 2B                    JA 0x14066ebfd
14066ebd2:  49 8B 88 18 06 00 00     MOV RCX,qword ptr [R8 + 0x618]
14066ebd9:  41 FF C7                 INC R15D
14066ebdc:  49 2B CB                 SUB RCX,R11
14066ebdf:  45 8B CF                 MOV R9D,R15D
14066ebe2:  48 8B C7                 MOV RAX,RDI
14066ebe5:  48 F7 E9                 IMUL RCX
14066ebe8:  48 C1 FA 0A              SAR RDX,0xa
14066ebec:  48 8B C2                 MOV RAX,RDX
14066ebef:  48 C1 E8 3F              SHR RAX,0x3f
14066ebf3:  48 03 D0                 ADD RDX,RAX
14066ebf6:  4C 3B CA                 CMP R9,RDX
14066ebf9:  72 C5                    JC 0x14066ebc0
14066ebfb:  EB 68                    JMP 0x14066ec65
14066ebfd:  45 85 FF                 TEST R15D,R15D
14066ec00:  78 63                    JS 0x14066ec65
14066ec02:  49 63 CF                 MOVSXD RCX,R15D
14066ec05:  48 69 F9 B8 21 00 00     IMUL RDI,RCX,0x21b8
14066ec0c:  41 83 7C 3B 04 06        CMP dword ptr [R11 + RDI*0x1 + 0x4],0x6
14066ec12:  75 34                    JNZ 0x14066ec48
14066ec14:  41 83 BA 2C 01 00 00 04  CMP dword ptr [R10 + 0x12c],0x4
14066ec1c:  74 2A                    JZ 0x14066ec48
14066ec1e:  49 8B 4A 10              MOV RCX,qword ptr [R10 + 0x10]

