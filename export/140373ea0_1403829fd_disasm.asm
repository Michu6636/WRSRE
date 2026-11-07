; Range: 0x140373EA0 to 0x1403829FD
140373ea0:  48 8B C4                 MOV RAX,RSP
140373ea3:  55                       PUSH RBP
140373ea4:  56                       PUSH RSI
140373ea5:  57                       PUSH RDI
140373ea6:  41 54                    PUSH R12
140373ea8:  41 55                    PUSH R13
140373eaa:  41 56                    PUSH R14
140373eac:  41 57                    PUSH R15
140373eae:  48 8D A8 E8 F1 FF FF     LEA RBP,[RAX + -0xe18]
140373eb5:  48 81 EC E0 0E 00 00     SUB RSP,0xee0
140373ebc:  48 C7 85 C0 02 00 00 FE FF FF FF MOV qword ptr [RBP + 0x2c0],-0x2
140373ec7:  48 89 58 20              MOV qword ptr [RAX + 0x20],RBX
140373ecb:  0F 29 70 B8              MOVAPS xmmword ptr [RAX + -0x48],XMM6
140373ecf:  0F 29 78 A8              MOVAPS xmmword ptr [RAX + -0x58],XMM7
140373ed3:  44 0F 29 40 98           MOVAPS xmmword ptr [RAX + -0x68],XMM8
140373ed8:  44 0F 29 48 88           MOVAPS xmmword ptr [RAX + -0x78],XMM9
140373edd:  44 0F 29 90 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM10
140373ee5:  44 0F 29 98 68 FF FF FF  MOVAPS xmmword ptr [RAX + -0x98],XMM11
140373eed:  44 0F 29 A0 58 FF FF FF  MOVAPS xmmword ptr [RAX + -0xa8],XMM12
140373ef5:  44 0F 29 A8 48 FF FF FF  MOVAPS xmmword ptr [RAX + -0xb8],XMM13
140373efd:  44 0F 29 B0 38 FF FF FF  MOVAPS xmmword ptr [RAX + -0xc8],XMM14
140373f05:  44 0F 29 B8 28 FF FF FF  MOVAPS xmmword ptr [RAX + -0xd8],XMM15
140373f0d:  48 8B 05 34 E1 61 00     MOV RAX,qword ptr [0x140992048]
140373f14:  48 33 C4                 XOR RAX,RSP
140373f17:  48 89 85 30 0D 00 00     MOV qword ptr [RBP + 0xd30],RAX
140373f1e:  49 8B F8                 MOV RDI,R8
140373f21:  4C 89 45 28              MOV qword ptr [RBP + 0x28],R8
140373f25:  48 8B DA                 MOV RBX,RDX
140373f28:  48 89 55 30              MOV qword ptr [RBP + 0x30],RDX
140373f2c:  48 8B F1                 MOV RSI,RCX
140373f2f:  F3 44 0F 10 15 50 E1 61 00 MOVSS XMM10,dword ptr [0x140992088]
140373f38:  45 0F 28 FA              MOVAPS XMM15,XMM10
140373f3c:  F3 44 0F 59 3D 23 6A 59 00 MULSS XMM15,dword ptr [0x14090a968]
140373f45:  41 0F 28 C2              MOVAPS XMM0,XMM10
140373f49:  F3 44 0F 10 1D C2 6B 59 00 MOVSS XMM11,dword ptr [0x14090ab14]
140373f52:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140373f57:  F3 44 0F 58 F8           ADDSS XMM15,XMM0
140373f5c:  F3 44 0F 11 7C 24 74     MOVSS dword ptr [RSP + 0x74],XMM15
140373f63:  F3 44 0F 59 15 60 69 59 00 MULSS XMM10,dword ptr [0x14090a8cc]
140373f6c:  F3 44 0F 58 91 50 AE 00 00 ADDSS XMM10,dword ptr [RCX + 0xae50]
140373f75:  48 8D 55 80              LEA RDX,[RBP + -0x80]
140373f79:  48 8D 0D 10 0C 6E 00     LEA RCX,[0x140a54b90]
140373f80:  FF 15 E2 88 4F 00        CALL qword ptr [0x14086c868]
140373f86:  48 8B 05 73 02 62 00     MOV RAX,qword ptr [0x140994200]
140373f8d:  48 89 45 68              MOV qword ptr [RBP + 0x68],RAX
140373f91:  4C 8B 3D 88 12 62 00     MOV R15,qword ptr [0x140995220]
140373f98:  32 C0                    XOR AL,AL
140373f9a:  88 45 B9                 MOV byte ptr [RBP + -0x47],AL
140373f9d:  88 45 A8                 MOV byte ptr [RBP + -0x58],AL
140373fa0:  48 8B 8E D8 0B 01 00     MOV RCX,qword ptr [RSI + 0x10bd8]
140373fa7:  48 2B 8E D0 0B 01 00     SUB RCX,qword ptr [RSI + 0x10bd0]
140373fae:  49 BD 67 66 66 66 66 66 66 66 MOV R13,0x6666666666666667
140373fb8:  49 8B C5                 MOV RAX,R13
140373fbb:  48 F7 E9                 IMUL RCX
140373fbe:  48 C1 FA 04              SAR RDX,0x4
140373fc2:  48 8B C2                 MOV RAX,RDX
140373fc5:  48 C1 E8 3F              SHR RAX,0x3f
140373fc9:  48 03 D0                 ADD RDX,RAX
140373fcc:  8B 86 E8 0B 01 00        MOV EAX,dword ptr [RSI + 0x10be8]
140373fd2:  48 3B C2                 CMP RAX,RDX
140373fd5:  73 63                    JNC 0x14037403a
140373fd7:  F3 0F 10 15 A9 E0 61 00  MOVSS XMM2,dword ptr [0x140992088]
140373fdf:  0F 28 CA                 MOVAPS XMM1,XMM2
140373fe2:  F3 0F 59 0D 3E 6A 59 00  MULSS XMM1,dword ptr [0x14090aa28]
140373fea:  0F 28 C2                 MOVAPS XMM0,XMM2
140373fed:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140373ff2:  F3 0F 58 C8              ADDSS XMM1,XMM0
140373ff6:  F3 0F 11 4D B4           MOVSS dword ptr [RBP + -0x4c],XMM1
140373ffb:  F3 0F 59 15 9D 69 59 00  MULSS XMM2,dword ptr [0x14090a9a0]
140374003:  F3 0F 58 96 50 AE 00 00  ADDSS XMM2,dword ptr [RSI + 0xae50]
14037400b:  F3 0F 11 54 24 70        MOVSS dword ptr [RSP + 0x70],XMM2
140374011:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140374016:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
14037401a:  48 8B CE                 MOV RCX,RSI
14037401d:  E8 9E 39 14 00           CALL 0x1404b79c0
140374022:  F3 0F 10 45 B4           MOVSS XMM0,dword ptr [RBP + -0x4c]
140374027:  F3 0F 11 03              MOVSS dword ptr [RBX],XMM0
14037402b:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140374031:  F3 0F 11 0F              MOVSS dword ptr [RDI],XMM1
140374035:  E9 BD E6 00 00           JMP 0x1403826f7
14037403a:  C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [RSI + 0x10be8],0xffffffff
140374044:  8B 86 D4 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d4]
14037404a:  89 45 D0                 MOV dword ptr [RBP + -0x30],EAX
14037404d:  0F 57 C0                 XORPS XMM0,XMM0
140374050:  F3 0F 7F 45 50           MOVDQU xmmword ptr [RBP + 0x50],XMM0
140374055:  45 33 F6                 XOR R14D,R14D
140374058:  4C 89 75 60              MOV qword ptr [RBP + 0x60],R14
14037405c:  4C 8D A6 F0 C7 00 00     LEA R12,[RSI + 0xc7f0]
140374063:  49 8B 54 24 08           MOV RDX,qword ptr [R12 + 0x8]
140374068:  49 2B 14 24              SUB RDX,qword ptr [R12]
14037406c:  48 C1 FA 04              SAR RDX,0x4
140374070:  48 8D 4D 50              LEA RCX,[RBP + 0x50]
140374074:  E8 F7 E1 E8 FF           CALL 0x140202270
140374079:  45 8B C6                 MOV R8D,R14D
14037407c:  48 8B 45 58              MOV RAX,qword ptr [RBP + 0x58]
140374080:  48 8B 55 50              MOV RDX,qword ptr [RBP + 0x50]
140374084:  48 2B C2                 SUB RAX,RDX
140374087:  48 C1 F8 04              SAR RAX,0x4
14037408b:  48 85 C0                 TEST RAX,RAX
14037408e:  74 40                    JZ 0x1403740d0
140374090:  41 8B CE                 MOV ECX,R14D
140374093:  0F 1F 40 00              NOP dword ptr [RAX]
140374097:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
1403740a0:  48 03 C9                 ADD RCX,RCX
1403740a3:  49 8B 04 24              MOV RAX,qword ptr [R12]
1403740a7:  0F 10 04 C8              MOVUPS XMM0,xmmword ptr [RAX + RCX*0x8]
1403740ab:  0F 11 04 CA              MOVUPS xmmword ptr [RDX + RCX*0x8],XMM0
1403740af:  41 FF C0                 INC R8D
1403740b2:  48 8B 45 58              MOV RAX,qword ptr [RBP + 0x58]
1403740b6:  48 8B 55 50              MOV RDX,qword ptr [RBP + 0x50]
1403740ba:  48 2B C2                 SUB RAX,RDX
1403740bd:  48 C1 F8 04              SAR RAX,0x4
1403740c1:  41 8B C8                 MOV ECX,R8D
1403740c4:  48 3B C8                 CMP RCX,RAX
1403740c7:  72 D7                    JC 0x1403740a0
1403740c9:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
1403740d0:  80 BE 70 09 01 00 00     CMP byte ptr [RSI + 0x10970],0x0
1403740d7:  74 3B                    JZ 0x140374114
1403740d9:  48 83 BE 78 09 01 00 00  CMP qword ptr [RSI + 0x10978],0x0
1403740e1:  74 0E                    JZ 0x1403740f1
1403740e3:  48 8B 86 78 09 01 00     MOV RAX,qword ptr [RSI + 0x10978]
1403740ea:  48 89 86 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],RAX
1403740f1:  80 BE 70 09 01 00 00     CMP byte ptr [RSI + 0x10970],0x0
1403740f8:  74 1A                    JZ 0x140374114
1403740fa:  C6 86 70 09 01 00 00     MOV byte ptr [RSI + 0x10970],0x0
140374101:  4C 89 B6 78 09 01 00     MOV qword ptr [RSI + 0x10978],R14
140374108:  48 8D 8E 80 09 01 00     LEA RCX,[RSI + 0x10980]
14037410f:  E8 AC DB E8 FF           CALL 0x140201cc0
140374114:  F3 0F 10 3D 6C DF 61 00  MOVSS XMM7,dword ptr [0x140992088]
14037411c:  0F 28 F7                 MOVAPS XMM6,XMM7
14037411f:  F3 0F 59 35 E5 68 59 00  MULSS XMM6,dword ptr [0x14090aa0c]
140374127:  0F 28 C7                 MOVAPS XMM0,XMM7
14037412a:  F3 41 0F 59 C3           MULSS XMM0,XMM11
14037412f:  F3 0F 58 F0              ADDSS XMM6,XMM0
140374133:  83 BE D4 C7 00 00 FF     CMP dword ptr [RSI + 0xc7d4],-0x1
14037413a:  0F 85 B2 02 00 00        JNZ 0x1403743f2
140374140:  F3 44 0F 10 25 57 69 59 00 MOVSS XMM12,dword ptr [0x14090aaa0]
140374149:  F3 41 0F 59 FC           MULSS XMM7,XMM12
14037414e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140374155:  41 B8 06 00 00 00        MOV R8D,0x6
14037415b:  41 0F 28 CA              MOVAPS XMM1,XMM10
14037415f:  E8 3C 1F 0D 00           CALL 0x1404460a0
140374164:  41 8B FE                 MOV EDI,R14D
140374167:  4C 8B 86 50 C6 00 00     MOV R8,qword ptr [RSI + 0xc650]
14037416e:  49 83 C0 48              ADD R8,0x48
140374172:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
140374176:  49 2B 08                 SUB RCX,qword ptr [R8]
140374179:  49 8B C5                 MOV RAX,R13
14037417c:  48 F7 E9                 IMUL RCX
14037417f:  48 C1 FA 05              SAR RDX,0x5
140374183:  48 8B C2                 MOV RAX,RDX
140374186:  48 C1 E8 3F              SHR RAX,0x3f
14037418a:  48 03 D0                 ADD RDX,RAX
14037418d:  45 0F 57 C0              XORPS XMM8,XMM8
140374191:  F3 44 0F 10 0D BE 5D 59 00 MOVSS XMM9,dword ptr [0x140909f58]
14037419a:  85 D2                    TEST EDX,EDX
14037419c:  0F 8E 14 04 00 00        JLE 0x1403745b6
1403741a2:  83 FF 02                 CMP EDI,0x2
1403741a5:  7D 04                    JGE 0x1403741ab
1403741a7:  8B DF                    MOV EBX,EDI
1403741a9:  EB 24                    JMP 0x1403741cf
1403741ab:  75 1F                    JNZ 0x1403741cc
1403741ad:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
1403741b1:  49 2B 08                 SUB RCX,qword ptr [R8]
1403741b4:  49 8B C5                 MOV RAX,R13
1403741b7:  48 F7 E9                 IMUL RCX
1403741ba:  48 C1 FA 05              SAR RDX,0x5
1403741be:  48 8B DA                 MOV RBX,RDX
1403741c1:  48 C1 EB 3F              SHR RBX,0x3f
1403741c5:  48 03 DA                 ADD RBX,RDX
1403741c8:  FF CB                    DEC EBX
1403741ca:  EB 03                    JMP 0x1403741cf
1403741cc:  8D 5F FF                 LEA EBX,[RDI + -0x1]
1403741cf:  83 BE 78 05 00 00 00     CMP dword ptr [RSI + 0x578],0x0
1403741d6:  74 0C                    JZ 0x1403741e4
1403741d8:  8D 43 FD                 LEA EAX,[RBX + -0x3]
1403741db:  83 F8 01                 CMP EAX,0x1
1403741de:  0F 86 D2 01 00 00        JBE 0x1403743b6
1403741e4:  83 FB 06                 CMP EBX,0x6
1403741e7:  75 0D                    JNZ 0x1403741f6
1403741e9:  83 BE E0 05 00 00 00     CMP dword ptr [RSI + 0x5e0],0x0
1403741f0:  0F 84 C0 01 00 00        JZ 0x1403743b6
1403741f6:  48 63 C3                 MOVSXD RAX,EBX
1403741f9:  4C 8D 2C 80              LEA R13,[RAX + RAX*0x4]
1403741fd:  49 C1 E5 04              SHL R13,0x4
140374201:  4D 03 28                 ADD R13,qword ptr [R8]
140374204:  49 8B 4D 48              MOV RCX,qword ptr [R13 + 0x48]
140374208:  48 8B 01                 MOV RAX,qword ptr [RCX]
14037420b:  4C 8B 05 F6 6A 67 00     MOV R8,qword ptr [0x1409ead08]
140374212:  33 D2                    XOR EDX,EDX
140374214:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140374217:  F3 0F 10 05 69 DE 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037421f:  F3 41 0F 59 C4           MULSS XMM0,XMM12
140374224:  F3 0F 11 05 BC A0 64 00  MOVSS dword ptr [0x1409be2e8],XMM0
14037422c:  F3 0F 11 05 B8 A0 64 00  MOVSS dword ptr [0x1409be2ec],XMM0
140374234:  C7 44 24 58 00 00 00 00  MOV dword ptr [RSP + 0x58],0x0
14037423c:  0F 28 C6                 MOVAPS XMM0,XMM6
14037423f:  41 0F 14 C2              UNPCKLPS XMM0,XMM10
140374243:  F2 0F 11 05 A5 A0 64 00  MOVSD qword ptr [0x1409be2f0],XMM0
14037424b:  8B 44 24 58              MOV EAX,dword ptr [RSP + 0x58]
14037424f:  89 05 A3 A0 64 00        MOV dword ptr [0x1409be2f8],EAX
140374255:  41 B8 06 00 00 00        MOV R8D,0x6
14037425b:  0F 28 CE                 MOVAPS XMM1,XMM6
14037425e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140374265:  E8 46 1C 0D 00           CALL 0x140445eb0
14037426a:  0F 28 05 8F 7E 59 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
140374271:  0F 11 05 94 A0 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140374278:  F2 0F 10 45 80           MOVSD XMM0,qword ptr [RBP + -0x80]
14037427d:  F2 0F 11 85 10 02 00 00  MOVSD qword ptr [RBP + 0x210],XMM0
140374285:  8B 45 88                 MOV EAX,dword ptr [RBP + -0x78]
140374288:  89 85 18 02 00 00        MOV dword ptr [RBP + 0x218],EAX
14037428e:  41 0F 28 D8              MOVAPS XMM3,XMM8
140374292:  41 0F 28 D0              MOVAPS XMM2,XMM8
140374296:  48 8D 95 10 02 00 00     LEA RDX,[RBP + 0x210]
14037429d:  48 8D 0D BC 9D 64 00     LEA RCX,[0x1409be060]
1403742a4:  FF 15 A6 85 4F 00        CALL qword ptr [0x14086c850]
1403742aa:  84 C0                    TEST AL,AL
1403742ac:  0F 84 B4 00 00 00        JZ 0x140374366
1403742b2:  41 8B 55 40              MOV EDX,dword ptr [R13 + 0x40]
1403742b6:  48 8D 0D D3 32 62 00     LEA RCX,[0x140997590]
1403742bd:  FF 15 CD 85 4F 00        CALL qword ptr [0x14086c890]
1403742c3:  48 8B D6                 MOV RDX,RSI
1403742c6:  48 2B D0                 SUB RDX,RAX
1403742c9:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
1403742d0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
1403742d3:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
1403742db:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403742df:  66 85 C9                 TEST CX,CX
1403742e2:  75 EC                    JNZ 0x1403742d0
1403742e4:  0F 28 05 D5 81 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1403742eb:  0F 11 05 1A A0 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1403742f2:  38 0D 99 0B 6E 00        CMP byte ptr [0x140a54e91],CL
1403742f8:  74 6C                    JZ 0x140374366
1403742fa:  38 4E 14                 CMP byte ptr [RSI + 0x14],CL
1403742fd:  75 48                    JNZ 0x140374347
1403742ff:  48 8D 15 A2 8D 54 00     LEA RDX,[0x1408bd0a8]
140374306:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037430d:  FF 15 FD 89 4F 00        CALL qword ptr [0x14086cd10]
140374313:  84 C0                    TEST AL,AL
140374315:  74 30                    JZ 0x140374347
140374317:  48 8D 15 8A 8D 54 00     LEA RDX,[0x1408bd0a8]
14037431e:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140374325:  FF 15 ED 89 4F 00        CALL qword ptr [0x14086cd18]
14037432b:  48 8B D0                 MOV RDX,RAX
14037432e:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
140374333:  41 0F 28 D9              MOVAPS XMM3,XMM9
140374337:  41 B0 01                 MOV R8B,0x1
14037433a:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
140374341:  FF 15 61 8F 4F 00        CALL qword ptr [0x14086d2a8]
140374347:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX
14037434d:  74 07                    JZ 0x140374356
14037434f:  C6 86 9A 03 01 00 00     MOV byte ptr [RSI + 0x1039a],0x0
140374356:  89 9E 5C C6 00 00        MOV dword ptr [RSI + 0xc65c],EBX
14037435c:  C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [RSI + 0x10be8],0xffffffff
140374366:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX
14037436c:  75 0E                    JNZ 0x14037437c
14037436e:  0F 28 05 4B 81 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140374375:  0F 11 05 90 9F 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14037437c:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140374381:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140374388:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037438f:  41 0F 28 D9              MOVAPS XMM3,XMM9
140374393:  41 0F 28 D0              MOVAPS XMM2,XMM8
140374397:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037439b:  48 8D 0D BE 9C 64 00     LEA RCX,[0x1409be060]
1403743a2:  FF 15 B8 84 4F 00        CALL qword ptr [0x14086c860]
1403743a8:  F3 0F 58 F7              ADDSS XMM6,XMM7
1403743ac:  49 BD 67 66 66 66 66 66 66 66 MOV R13,0x6666666666666667
1403743b6:  FF C7                    INC EDI
1403743b8:  4C 8B 86 50 C6 00 00     MOV R8,qword ptr [RSI + 0xc650]
1403743bf:  49 83 C0 48              ADD R8,0x48
1403743c3:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
1403743c7:  49 2B 08                 SUB RCX,qword ptr [R8]
1403743ca:  49 8B C5                 MOV RAX,R13
1403743cd:  48 F7 E9                 IMUL RCX
1403743d0:  48 C1 FA 05              SAR RDX,0x5
1403743d4:  48 8B C2                 MOV RAX,RDX
1403743d7:  48 C1 E8 3F              SHR RAX,0x3f
1403743db:  48 03 D0                 ADD RDX,RAX
1403743de:  3B FA                    CMP EDI,EDX
1403743e0:  0F 8C BC FD FF FF        JL 0x1403741a2
1403743e6:  4C 8D A6 F0 C7 00 00     LEA R12,[RSI + 0xc7f0]
1403743ed:  E9 C4 01 00 00           JMP 0x1403745b6
1403743f2:  48 8B 8E 48 B4 00 00     MOV RCX,qword ptr [RSI + 0xb448]
1403743f9:  48 8B 01                 MOV RAX,qword ptr [RCX]
1403743fc:  4C 8B 05 05 69 67 00     MOV R8,qword ptr [0x1409ead08]
140374403:  33 D2                    XOR EDX,EDX
140374405:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140374408:  F3 0F 10 05 78 DC 61 00  MOVSS XMM0,dword ptr [0x140992088]
140374410:  F3 0F 59 05 88 66 59 00  MULSS XMM0,dword ptr [0x14090aaa0]
140374418:  F3 0F 11 05 C8 9E 64 00  MOVSS dword ptr [0x1409be2e8],XMM0
140374420:  F3 0F 11 05 C4 9E 64 00  MOVSS dword ptr [0x1409be2ec],XMM0
140374428:  45 0F 57 C0              XORPS XMM8,XMM8
14037442c:  C7 44 24 58 00 00 00 00  MOV dword ptr [RSP + 0x58],0x0
140374434:  0F 28 C6                 MOVAPS XMM0,XMM6
140374437:  41 0F 14 C2              UNPCKLPS XMM0,XMM10
14037443b:  F2 0F 11 05 AD 9E 64 00  MOVSD qword ptr [0x1409be2f0],XMM0
140374443:  8B 44 24 58              MOV EAX,dword ptr [RSP + 0x58]
140374447:  89 05 AB 9E 64 00        MOV dword ptr [0x1409be2f8],EAX
14037444d:  41 B8 06 00 00 00        MOV R8D,0x6
140374453:  41 0F 28 CA              MOVAPS XMM1,XMM10
140374457:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037445e:  E8 3D 1C 0D 00           CALL 0x1404460a0
140374463:  41 B8 06 00 00 00        MOV R8D,0x6
140374469:  0F 28 CE                 MOVAPS XMM1,XMM6
14037446c:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140374473:  E8 38 1A 0D 00           CALL 0x140445eb0
140374478:  0F 28 05 81 7C 59 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
14037447f:  0F 11 05 86 9E 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140374486:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
14037448b:  F3 0F 11 85 90 02 00 00  MOVSS dword ptr [RBP + 0x290],XMM0
140374493:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140374498:  F3 0F 11 8D 94 02 00 00  MOVSS dword ptr [RBP + 0x294],XMM1
1403744a0:  F3 0F 10 45 88           MOVSS XMM0,dword ptr [RBP + -0x78]
1403744a5:  F3 0F 11 85 98 02 00 00  MOVSS dword ptr [RBP + 0x298],XMM0
1403744ad:  41 0F 28 D8              MOVAPS XMM3,XMM8
1403744b1:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403744b5:  48 8D 95 90 02 00 00     LEA RDX,[RBP + 0x290]
1403744bc:  48 8D 0D 9D 9B 64 00     LEA RCX,[0x1409be060]
1403744c3:  FF 15 87 83 4F 00        CALL qword ptr [0x14086c850]
1403744c9:  F3 44 0F 10 0D 86 5A 59 00 MOVSS XMM9,dword ptr [0x140909f58]
1403744d2:  84 C0                    TEST AL,AL
1403744d4:  0F 84 B0 00 00 00        JZ 0x14037458a
1403744da:  BA 5D 02 00 00           MOV EDX,0x25d
1403744df:  48 8D 0D AA 30 62 00     LEA RCX,[0x140997590]
1403744e6:  FF 15 A4 83 4F 00        CALL qword ptr [0x14086c890]
1403744ec:  48 8B D6                 MOV RDX,RSI
1403744ef:  48 2B D0                 SUB RDX,RAX
1403744f2:  0F 1F 40 00              NOP dword ptr [RAX]
1403744f6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140374500:  0F B7 08                 MOVZX ECX,word ptr [RAX]
140374503:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
14037450b:  48 83 C0 02              ADD RAX,0x2
14037450f:  66 85 C9                 TEST CX,CX
140374512:  75 EC                    JNZ 0x140374500
140374514:  0F 28 05 A5 7F 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
14037451b:  0F 11 05 EA 9D 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140374522:  38 0D 69 09 6E 00        CMP byte ptr [0x140a54e91],CL
140374528:  74 52                    JZ 0x14037457c
14037452a:  38 4E 14                 CMP byte ptr [RSI + 0x14],CL
14037452d:  75 48                    JNZ 0x140374577
14037452f:  48 8D 15 62 8B 54 00     LEA RDX,[0x1408bd098]
140374536:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037453d:  FF 15 CD 87 4F 00        CALL qword ptr [0x14086cd10]
140374543:  84 C0                    TEST AL,AL
140374545:  74 30                    JZ 0x140374577
140374547:  48 8D 15 4A 8B 54 00     LEA RDX,[0x1408bd098]
14037454e:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140374555:  FF 15 BD 87 4F 00        CALL qword ptr [0x14086cd18]
14037455b:  48 8B D0                 MOV RDX,RAX
14037455e:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
140374563:  41 0F 28 D9              MOVAPS XMM3,XMM9
140374567:  41 B0 01                 MOV R8B,0x1
14037456a:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
140374571:  FF 15 31 8D 4F 00        CALL qword ptr [0x14086d2a8]
140374577:  B0 01                    MOV AL,0x1
140374579:  88 45 B9                 MOV byte ptr [RBP + -0x47],AL
14037457c:  0F 28 05 3D 7F 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140374583:  0F 11 05 82 9D 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14037458a:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037458f:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140374596:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037459d:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403745a1:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403745a5:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403745a9:  48 8D 0D B0 9A 64 00     LEA RCX,[0x1409be060]
1403745b0:  FF 15 AA 82 4F 00        CALL qword ptr [0x14086c860]
1403745b6:  F3 44 0F 10 25 0D 64 59 00 MOVSS XMM12,dword ptr [0x14090a9cc]
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
140374670:  0F 10 45 78              MOVUPS XMM0,xmmword ptr [RBP + 0x78]
140374674:  0F 11 45 90              MOVUPS xmmword ptr [RBP + -0x70],XMM0
140374678:  F2 0F 10 8D 88 00 00 00  MOVSD XMM1,qword ptr [RBP + 0x88]
140374680:  F2 0F 11 4D A0           MOVSD qword ptr [RBP + -0x60],XMM1
140374685:  0F 57 C0                 XORPS XMM0,XMM0
140374688:  F3 0F 7F 45 78           MOVDQU xmmword ptr [RBP + 0x78],XMM0
14037468d:  4C 89 B5 88 00 00 00     MOV qword ptr [RBP + 0x88],R14
140374694:  48 8D 4D 78              LEA RCX,[RBP + 0x78]
140374698:  E8 13 64 CD FF           CALL 0x14004aab0
14037469d:  90                       NOP
14037469e:  32 C0                    XOR AL,AL
1403746a0:  88 44 24 7C              MOV byte ptr [RSP + 0x7c],AL
1403746a4:  66 0F 6F 05 94 80 59 00  MOVDQA XMM0,xmmword ptr [0x14090c740]
1403746ac:  F3 0F 7F 45 08           MOVDQU xmmword ptr [RBP + 0x8],XMM0
1403746b1:  48 C7 45 18 FF FF FF FF  MOV qword ptr [RBP + 0x18],-0x1
1403746b9:  4C 89 75 20              MOV qword ptr [RBP + 0x20],R14
1403746bd:  66 0F 7F 85 20 05 00 00  MOVDQA xmmword ptr [RBP + 0x520],XMM0
1403746c5:  48 C7 85 30 05 00 00 FF FF FF FF MOV qword ptr [RBP + 0x530],-0x1
1403746d0:  4C 89 B5 38 05 00 00     MOV qword ptr [RBP + 0x538],R14
1403746d7:  48 8D 85 40 05 00 00     LEA RAX,[RBP + 0x540]
1403746de:  48 89 44 24 50           MOV qword ptr [RSP + 0x50],RAX
1403746e3:  4C 89 B5 68 06 00 00     MOV qword ptr [RBP + 0x668],R14
1403746ea:  44 89 B5 70 06 00 00     MOV dword ptr [RBP + 0x670],R14D
1403746f1:  4C 89 B5 78 06 00 00     MOV qword ptr [RBP + 0x678],R14
1403746f8:  44 89 B5 80 06 00 00     MOV dword ptr [RBP + 0x680],R14D
1403746ff:  4C 89 B5 88 06 00 00     MOV qword ptr [RBP + 0x688],R14
140374706:  44 89 B5 90 06 00 00     MOV dword ptr [RBP + 0x690],R14D
14037470d:  4C 89 B5 98 06 00 00     MOV qword ptr [RBP + 0x698],R14
140374714:  44 89 B5 A0 06 00 00     MOV dword ptr [RBP + 0x6a0],R14D
14037471b:  48 8D 8D 40 05 00 00     LEA RCX,[RBP + 0x540]
140374722:  E8 29 E9 25 00           CALL 0x1405d3050
140374727:  90                       NOP
140374728:  48 8D 8D B0 06 00 00     LEA RCX,[RBP + 0x6b0]
14037472f:  E8 8C 67 0D 00           CALL 0x14044aec0
140374734:  48 8D 8D C8 06 00 00     LEA RCX,[RBP + 0x6c8]
14037473b:  E8 10 40 CC FF           CALL 0x140038750
140374740:  0F 57 C0                 XORPS XMM0,XMM0
140374743:  66 0F 7F 85 E0 06 00 00  MOVDQA xmmword ptr [RBP + 0x6e0],XMM0
14037474b:  4C 89 B5 F0 06 00 00     MOV qword ptr [RBP + 0x6f0],R14
140374752:  66 C7 85 10 05 00 00 00 00 MOV word ptr [RBP + 0x510],0x0
14037475b:  4C 89 B5 18 05 00 00     MOV qword ptr [RBP + 0x518],R14
140374762:  4C 89 B5 A8 06 00 00     MOV qword ptr [RBP + 0x6a8],R14
140374769:  8B 86 D0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d0]
14037476f:  89 45 10                 MOV dword ptr [RBP + 0x10],EAX
140374772:  44 8B 8E DC C7 00 00     MOV R9D,dword ptr [RSI + 0xc7dc]
140374779:  44 89 4D 1C              MOV dword ptr [RBP + 0x1c],R9D
14037477d:  44 8B 96 D4 C7 00 00     MOV R10D,dword ptr [RSI + 0xc7d4]
140374784:  44 89 55 18              MOV dword ptr [RBP + 0x18],R10D
140374788:  4C 8B 9E E8 C7 00 00     MOV R11,qword ptr [RSI + 0xc7e8]
14037478f:  4C 89 5D 20              MOV qword ptr [RBP + 0x20],R11
140374793:  8B 9E 5C C6 00 00        MOV EBX,dword ptr [RSI + 0xc65c]
140374799:  89 5D 14                 MOV dword ptr [RBP + 0x14],EBX
14037479c:  44 8B 86 18 0F 01 00     MOV R8D,dword ptr [RSI + 0x10f18]
1403747a3:  44 89 45 08              MOV dword ptr [RBP + 0x8],R8D
1403747a7:  48 63 96 1C 0F 01 00     MOVSXD RDX,dword ptr [RSI + 0x10f1c]
1403747ae:  89 55 0C                 MOV dword ptr [RBP + 0xc],EDX
1403747b1:  48 8B 8E 30 17 01 00     MOV RCX,qword ptr [RSI + 0x11730]
1403747b8:  48 2B 8E 28 17 01 00     SUB RCX,qword ptr [RSI + 0x11728]
1403747bf:  48 C1 F9 0A              SAR RCX,0xa
1403747c3:  48 3B D1                 CMP RDX,RCX
1403747c6:  0F 82 AC 00 00 00        JC 0x140374878
1403747cc:  44 3B C2                 CMP R8D,EDX
1403747cf:  0F 8D A3 00 00 00        JGE 0x140374878
1403747d5:  83 FA 01                 CMP EDX,0x1
1403747d8:  0F 8E 9A 00 00 00        JLE 0x140374878
1403747de:  C6 44 24 7C 01           MOV byte ptr [RSP + 0x7c],0x1
1403747e3:  8B 86 1C 0F 01 00        MOV EAX,dword ptr [RSI + 0x10f1c]
1403747e9:  FF C8                    DEC EAX
1403747eb:  89 45 08                 MOV dword ptr [RBP + 0x8],EAX
1403747ee:  8B 86 1C 0F 01 00        MOV EAX,dword ptr [RSI + 0x10f1c]
1403747f4:  89 45 0C                 MOV dword ptr [RBP + 0xc],EAX
1403747f7:  4C 89 74 24 60           MOV qword ptr [RSP + 0x60],R14
1403747fc:  44 89 74 24 68           MOV dword ptr [RSP + 0x68],R14D
140374801:  4C 8D 05 34 52 50 00     LEA R8,[0x140879a3c]
140374808:  83 CA FF                 OR EDX,0xffffffff
14037480b:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140374810:  E8 6B 03 CC FF           CALL 0x140034b80
140374815:  90                       NOP
140374816:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037481b:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
140374822:  E8 B9 F7 25 00           CALL 0x1405d3fe0
140374827:  90                       NOP
140374828:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037482d:  48 85 C9                 TEST RCX,RCX
140374830:  74 05                    JZ 0x140374837
140374832:  E8 A9 97 4D 00           CALL 0x14084dfe0
140374837:  48 8D 55 08              LEA RDX,[RBP + 0x8]
14037483b:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
140374842:  E8 79 27 26 00           CALL 0x1405d6fc0
140374847:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
14037484e:  E8 ED E9 26 00           CALL 0x1405e3240
140374853:  44 8B 86 18 0F 01 00     MOV R8D,dword ptr [RSI + 0x10f18]
14037485a:  44 89 45 08              MOV dword ptr [RBP + 0x8],R8D
14037485e:  8B 96 1C 0F 01 00        MOV EDX,dword ptr [RSI + 0x10f1c]
140374864:  FF CA                    DEC EDX
140374866:  89 55 0C                 MOV dword ptr [RBP + 0xc],EDX
140374869:  4C 8B 5D 20              MOV R11,qword ptr [RBP + 0x20]
14037486d:  44 8B 4D 1C              MOV R9D,dword ptr [RBP + 0x1c]
140374871:  44 8B 55 18              MOV R10D,dword ptr [RBP + 0x18]
140374875:  8B 5D 14                 MOV EBX,dword ptr [RBP + 0x14]
140374878:  48 8D 86 D8 06 01 00     LEA RAX,[RSI + 0x106d8]
14037487f:  48 8B 0D BA 1D 67 00     MOV RCX,qword ptr [0x1409e6640]
140374886:  48 8B 3D AB 1D 67 00     MOV RDI,qword ptr [0x1409e6638]
14037488d:  80 38 00                 CMP byte ptr [RAX],0x0
140374890:  0F 85 1E 01 00 00        JNZ 0x1403749b4
140374896:  80 78 01 00              CMP byte ptr [RAX + 0x1],0x0
14037489a:  75 56                    JNZ 0x1403748f2
14037489c:  48 8B C1                 MOV RAX,RCX
14037489f:  48 2B C7                 SUB RAX,RDI
1403748a2:  48 C1 F8 0A              SAR RAX,0xa
1403748a6:  3B D0                    CMP EDX,EAX
1403748a8:  0F 9C C0                 SETL AL
1403748ab:  84 C0                    TEST AL,AL
1403748ad:  48 8D 86 D8 06 01 00     LEA RAX,[RSI + 0x106d8]
1403748b4:  74 3C                    JZ 0x1403748f2
1403748b6:  44 39 40 10              CMP dword ptr [RAX + 0x10],R8D
1403748ba:  75 36                    JNZ 0x1403748f2
1403748bc:  39 50 14                 CMP dword ptr [RAX + 0x14],EDX
1403748bf:  75 31                    JNZ 0x1403748f2
1403748c1:  39 58 1C                 CMP dword ptr [RAX + 0x1c],EBX
1403748c4:  75 2C                    JNZ 0x1403748f2
1403748c6:  44 39 48 24              CMP dword ptr [RAX + 0x24],R9D
1403748ca:  75 26                    JNZ 0x1403748f2
1403748cc:  44 39 50 20              CMP dword ptr [RAX + 0x20],R10D
1403748d0:  75 06                    JNZ 0x1403748d8
1403748d2:  4C 39 58 28              CMP qword ptr [RAX + 0x28],R11
1403748d6:  74 1A                    JZ 0x1403748f2
1403748d8:  4D 8B C3                 MOV R8,R11
1403748db:  41 8B D2                 MOV EDX,R10D
1403748de:  48 8D BE D8 06 01 00     LEA RDI,[RSI + 0x106d8]
1403748e5:  48 8B CF                 MOV RCX,RDI
1403748e8:  E8 73 98 26 00           CALL 0x1405de160
1403748ed:  E9 D5 00 00 00           JMP 0x1403749c7
1403748f2:  80 38 00                 CMP byte ptr [RAX],0x0
1403748f5:  0F 85 B9 00 00 00        JNZ 0x1403749b4
1403748fb:  80 78 01 00              CMP byte ptr [RAX + 0x1],0x0
1403748ff:  75 5C                    JNZ 0x14037495d
140374901:  48 8B C1                 MOV RAX,RCX
140374904:  48 2B C7                 SUB RAX,RDI
140374907:  48 C1 F8 0A              SAR RAX,0xa
14037490b:  3B D0                    CMP EDX,EAX
14037490d:  0F 9C C0                 SETL AL
140374910:  84 C0                    TEST AL,AL
140374912:  48 8D 86 D8 06 01 00     LEA RAX,[RSI + 0x106d8]
140374919:  74 42                    JZ 0x14037495d
14037491b:  44 39 40 10              CMP dword ptr [RAX + 0x10],R8D
14037491f:  75 3C                    JNZ 0x14037495d
140374921:  39 50 14                 CMP dword ptr [RAX + 0x14],EDX
140374924:  75 37                    JNZ 0x14037495d
140374926:  39 58 1C                 CMP dword ptr [RAX + 0x1c],EBX
140374929:  75 32                    JNZ 0x14037495d
14037492b:  44 39 50 20              CMP dword ptr [RAX + 0x20],R10D
14037492f:  75 2C                    JNZ 0x14037495d
140374931:  4C 39 58 28              CMP qword ptr [RAX + 0x28],R11
140374935:  75 26                    JNZ 0x14037495d
140374937:  44 39 48 24              CMP dword ptr [RAX + 0x24],R9D
14037493b:  0F 95 C0                 SETNZ AL
14037493e:  84 C0                    TEST AL,AL
140374940:  74 14                    JZ 0x140374956
140374942:  41 8B D1                 MOV EDX,R9D
140374945:  48 8D BE D8 06 01 00     LEA RDI,[RSI + 0x106d8]
14037494c:  48 8B CF                 MOV RCX,RDI
14037494f:  E8 4C DD 26 00           CALL 0x1405e26a0
140374954:  EB 71                    JMP 0x1403749c7
140374956:  48 8D 86 D8 06 01 00     LEA RAX,[RSI + 0x106d8]
14037495d:  80 38 00                 CMP byte ptr [RAX],0x0
140374960:  75 52                    JNZ 0x1403749b4
140374962:  80 78 01 00              CMP byte ptr [RAX + 0x1],0x0
140374966:  75 4C                    JNZ 0x1403749b4
140374968:  48 2B CF                 SUB RCX,RDI
14037496b:  48 C1 F9 0A              SAR RCX,0xa
14037496f:  3B D1                    CMP EDX,ECX
140374971:  0F 9D C0                 SETGE AL
140374974:  48 8D BE D8 06 01 00     LEA RDI,[RSI + 0x106d8]
14037497b:  84 C0                    TEST AL,AL
14037497d:  75 3C                    JNZ 0x1403749bb
14037497f:  44 39 47 10              CMP dword ptr [RDI + 0x10],R8D
140374983:  75 21                    JNZ 0x1403749a6
140374985:  39 57 14                 CMP dword ptr [RDI + 0x14],EDX
140374988:  75 1C                    JNZ 0x1403749a6
14037498a:  39 5F 1C                 CMP dword ptr [RDI + 0x1c],EBX
14037498d:  75 17                    JNZ 0x1403749a6
14037498f:  44 39 4F 24              CMP dword ptr [RDI + 0x24],R9D
140374993:  75 11                    JNZ 0x1403749a6
140374995:  44 39 57 20              CMP dword ptr [RDI + 0x20],R10D
140374999:  75 0B                    JNZ 0x1403749a6
14037499b:  4C 39 5F 28              CMP qword ptr [RDI + 0x28],R11
14037499f:  B8 01 00 00 00           MOV EAX,0x1
1403749a4:  74 03                    JZ 0x1403749a9
1403749a6:  41 8B C6                 MOV EAX,R14D
1403749a9:  84 C0                    TEST AL,AL
1403749ab:  0F 94 C0                 SETZ AL
1403749ae:  84 C0                    TEST AL,AL
1403749b0:  74 1A                    JZ 0x1403749cc
1403749b2:  EB 07                    JMP 0x1403749bb
1403749b4:  48 8D BE D8 06 01 00     LEA RDI,[RSI + 0x106d8]
1403749bb:  48 8D 55 08              LEA RDX,[RBP + 0x8]
1403749bf:  48 8B CF                 MOV RCX,RDI
1403749c2:  E8 F9 25 26 00           CALL 0x1405d6fc0
1403749c7:  C6 44 24 7D 01           MOV byte ptr [RSP + 0x7d],0x1
1403749cc:  0F B6 5C 24 7C           MOVZX EBX,byte ptr [RSP + 0x7c]
1403749d1:  84 DB                    TEST BL,BL
1403749d3:  74 0F                    JZ 0x1403749e4
1403749d5:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
1403749dc:  48 8B CF                 MOV RCX,RDI
1403749df:  E8 2C E9 26 00           CALL 0x1405e3310
1403749e4:  48 8D 96 08 07 01 00     LEA RDX,[RSI + 0x10708]
1403749eb:  48 8D 8D 90 00 00 00     LEA RCX,[RBP + 0x90]
1403749f2:  E8 29 6D EE FF           CALL 0x14025b720
1403749f7:  90                       NOP
1403749f8:  8B 85 90 00 00 00        MOV EAX,dword ptr [RBP + 0x90]
1403749fe:  85 C0                    TEST EAX,EAX
140374a00:  7E 67                    JLE 0x140374a69
140374a02:  66 0F 6E C0              MOVD XMM0,EAX
140374a06:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140374a09:  F3 44 0F 10 AD 04 01 00 00 MOVSS XMM13,dword ptr [RBP + 0x104]
140374a12:  F3 44 0F 5E E8           DIVSS XMM13,XMM0
140374a17:  F3 44 0F 10 B5 08 01 00 00 MOVSS XMM14,dword ptr [RBP + 0x108]
140374a20:  F3 44 0F 5E F0           DIVSS XMM14,XMM0
140374a25:  F3 0F 10 B5 0C 01 00 00  MOVSS XMM6,dword ptr [RBP + 0x10c]
140374a2d:  F3 0F 5E F0              DIVSS XMM6,XMM0
140374a31:  F3 0F 11 75 C0           MOVSS dword ptr [RBP + -0x40],XMM6
140374a36:  F3 0F 10 B5 10 01 00 00  MOVSS XMM6,dword ptr [RBP + 0x110]
140374a3e:  F3 0F 5E F0              DIVSS XMM6,XMM0
140374a42:  F3 0F 11 75 F0           MOVSS dword ptr [RBP + -0x10],XMM6
140374a47:  F3 0F 10 B5 00 01 00 00  MOVSS XMM6,dword ptr [RBP + 0x100]
140374a4f:  F3 0F 5E F0              DIVSS XMM6,XMM0
140374a53:  F3 0F 11 75 C8           MOVSS dword ptr [RBP + -0x38],XMM6
140374a58:  F3 0F 10 B5 F4 00 00 00  MOVSS XMM6,dword ptr [RBP + 0xf4]
140374a60:  F3 0F 5E F0              DIVSS XMM6,XMM0
140374a64:  F3 0F 11 75 F8           MOVSS dword ptr [RBP + -0x8],XMM6
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
140374bcb:  74 5D                    JZ 0x140374c2a
140374bcd:  48 8D 0D 8C 94 64 00     LEA RCX,[0x1409be060]
140374bd4:  FF 15 1E 7D 4F 00        CALL qword ptr [0x14086c8f8]
140374bda:  48 8D 15 A7 83 54 00     LEA RDX,[0x1408bcf88]
140374be1:  48 8D 0D C8 23 62 00     LEA RCX,[0x140996fb0]
140374be8:  FF 15 82 7C 4F 00        CALL qword ptr [0x14086c870]
140374bee:  45 33 C0                 XOR R8D,R8D
140374bf1:  33 D2                    XOR EDX,EDX
140374bf3:  48 8D 0D 66 94 64 00     LEA RCX,[0x1409be060]
140374bfa:  FF 15 18 7D 4F 00        CALL qword ptr [0x14086c918]
140374c00:  48 8D 86 18 0F 01 00     LEA RAX,[RSI + 0x10f18]
140374c07:  48 39 86 F0 05 00 00     CMP qword ptr [RSI + 0x5f0],RAX
140374c0e:  41 0F 94 C1              SETZ R9B
140374c12:  F3 0F 10 96 EC 05 00 00  MOVSS XMM2,dword ptr [RSI + 0x5ec]
140374c1a:  F3 0F 10 8E E8 05 00 00  MOVSS XMM1,dword ptr [RSI + 0x5e8]
140374c22:  48 8B CE                 MOV RCX,RSI
140374c25:  E8 96 10 FC FF           CALL 0x140335cc0
140374c2a:  84 DB                    TEST BL,BL
140374c2c:  74 0F                    JZ 0x140374c3d
140374c2e:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
140374c35:  48 8B CF                 MOV RCX,RDI
140374c38:  E8 E3 E8 26 00           CALL 0x1405e3520
140374c3d:  F3 0F 10 45 B4           MOVSS XMM0,dword ptr [RBP + -0x4c]
140374c42:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
140374c46:  F3 0F 11 00              MOVSS dword ptr [RAX],XMM0
140374c4a:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140374c50:  48 8B 45 28              MOV RAX,qword ptr [RBP + 0x28]
140374c54:  F3 0F 11 08              MOVSS dword ptr [RAX],XMM1
140374c58:  E9 6D DA 00 00           JMP 0x1403826ca
140374c5d:  F3 44 0F 11 7D B4        MOVSS dword ptr [RBP + -0x4c],XMM15
140374c63:  F3 0F 10 35 1D D4 61 00  MOVSS XMM6,dword ptr [0x140992088]
140374c6b:  0F 28 C6                 MOVAPS XMM0,XMM6
140374c6e:  F3 0F 59 05 B2 5C 59 00  MULSS XMM0,dword ptr [0x14090a928]
140374c76:  F3 0F 11 44 24 78        MOVSS dword ptr [RSP + 0x78],XMM0
140374c7c:  0F 28 D6                 MOVAPS XMM2,XMM6
140374c7f:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140374c84:  0F 28 C2                 MOVAPS XMM0,XMM2
140374c87:  F3 44 0F 10 5C 24 78     MOVSS XMM11,dword ptr [RSP + 0x78]
140374c8e:  F3 41 0F 58 C3           ADDSS XMM0,XMM11
140374c93:  F3 44 0F 2C C8           CVTTSS2SI R9D,XMM0
140374c98:  44 89 4C 24 60           MOV dword ptr [RSP + 0x60],R9D
140374c9d:  0F 28 CE                 MOVAPS XMM1,XMM6
140374ca0:  F3 0F 59 0D 3C 63 59 00  MULSS XMM1,dword ptr [0x14090afe4]
140374ca8:  F3 0F 58 CA              ADDSS XMM1,XMM2
140374cac:  F3 0F 2C C1              CVTTSS2SI EAX,XMM1
140374cb0:  89 44 24 68              MOV dword ptr [RSP + 0x68],EAX
140374cb4:  8B 96 D4 C7 00 00        MOV EDX,dword ptr [RSI + 0xc7d4]
140374cba:  83 FA FF                 CMP EDX,-0x1
140374cbd:  0F 85 3E AA 00 00        JNZ 0x14037f701
140374cc3:  85 C9                    TEST ECX,ECX
140374cc5:  0F 94 C0                 SETZ AL
140374cc8:  83 F9 01                 CMP ECX,0x1
140374ccb:  41 0F 94 C0              SETZ R8B
140374ccf:  84 C0                    TEST AL,AL
140374cd1:  0F 84 96 5A 00 00        JZ 0x14037a76d
140374cd7:  4C 89 B6 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],R14
140374cde:  0F 28 FE                 MOVAPS XMM7,XMM6
140374ce1:  F3 0F 59 3D 8B 61 59 00  MULSS XMM7,dword ptr [0x14090ae74]
140374ce9:  F3 41 0F 58 FF           ADDSS XMM7,XMM15
140374cee:  F3 0F 11 7D B0           MOVSS dword ptr [RBP + -0x50],XMM7
140374cf3:  44 0F 28 E6              MOVAPS XMM12,XMM6
140374cf7:  F3 44 0F 59 25 18 62 59 00 MULSS XMM12,dword ptr [0x14090af18]
140374d00:  F3 45 0F 58 E7           ADDSS XMM12,XMM15
140374d05:  F3 44 0F 11 64 24 74     MOVSS dword ptr [RSP + 0x74],XMM12
140374d0c:  0F 28 C6                 MOVAPS XMM0,XMM6
140374d0f:  F3 0F 59 05 91 62 59 00  MULSS XMM0,dword ptr [0x14090afa8]
140374d17:  F3 41 0F 58 C7           ADDSS XMM0,XMM15
140374d1c:  F3 0F 11 44 24 70        MOVSS dword ptr [RSP + 0x70],XMM0
140374d22:  0F 28 CE                 MOVAPS XMM1,XMM6
140374d25:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140374d2a:  41 0F 28 C2              MOVAPS XMM0,XMM10
140374d2e:  F3 0F 5C C1              SUBSS XMM0,XMM1
140374d32:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
140374d36:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
140374d3a:  41 0F 28 C3              MOVAPS XMM0,XMM11
140374d3e:  F3 0F 59 05 D2 58 59 00  MULSS XMM0,dword ptr [0x14090a618]
140374d46:  F3 0F 11 45 BC           MOVSS dword ptr [RBP + -0x44],XMM0
140374d4b:  F3 41 0F 58 C2           ADDSS XMM0,XMM10
140374d50:  F3 0F 58 C1              ADDSS XMM0,XMM1
140374d54:  F3 0F 2C C8              CVTTSS2SI ECX,XMM0
140374d58:  89 4C 24 6C              MOV dword ptr [RSP + 0x6c],ECX
140374d5c:  F3 44 0F 10 1D 77 50 59 00 MOVSS XMM11,dword ptr [0x140909ddc]
140374d65:  44 8B 6C 24 68           MOV R13D,dword ptr [RSP + 0x68]
140374d6a:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140374d6f:  0F 84 9B 02 00 00        JZ 0x140375010
140374d75:  66 41 0F 6E C1           MOVD XMM0,R9D
140374d7a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140374d7d:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140374d82:  0F 2F C8                 COMISS XMM1,XMM0
140374d85:  0F 86 85 02 00 00        JBE 0x140375010
140374d8b:  66 41 0F 6E C5           MOVD XMM0,R13D
140374d90:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140374d93:  0F 2F C1                 COMISS XMM0,XMM1
140374d96:  0F 86 74 02 00 00        JBE 0x140375010
140374d9c:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140374da1:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140374da6:  66 0F 6E C0              MOVD XMM0,EAX
140374daa:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140374dad:  0F 2F C8                 COMISS XMM1,XMM0
140374db0:  0F 86 5A 02 00 00        JBE 0x140375010
140374db6:  66 0F 6E C1              MOVD XMM0,ECX
140374dba:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140374dbd:  0F 2F C1                 COMISS XMM0,XMM1
140374dc0:  0F 86 4A 02 00 00        JBE 0x140375010
140374dc6:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140374dcd:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140374dd0:  48 8D 0D F9 5E 67 00     LEA RCX,[0x1409eacd0]
140374dd7:  FF 15 13 79 4F 00        CALL qword ptr [0x14086c6f0]
140374ddd:  4C 8B C0                 MOV R8,RAX
140374de0:  33 D2                    XOR EDX,EDX
140374de2:  48 8B CB                 MOV RCX,RBX
140374de5:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140374de8:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140374def:  41 0F 28 D9              MOVAPS XMM3,XMM9
140374df3:  41 0F 28 D1              MOVAPS XMM2,XMM9
140374df7:  41 0F 28 C9              MOVAPS XMM1,XMM9
140374dfb:  48 8D 8D 80 0A 00 00     LEA RCX,[RBP + 0xa80]
140374e02:  FF 15 18 72 4F 00        CALL qword ptr [0x14086c020]
140374e08:  48 8B D0                 MOV RDX,RAX
140374e0b:  48 8D 0D 4E 92 64 00     LEA RCX,[0x1409be060]
140374e12:  FF 15 80 7A 4F 00        CALL qword ptr [0x14086c898]
140374e18:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140374e1d:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140374e23:  F3 0F 10 15 5D D2 61 00  MOVSS XMM2,dword ptr [0x140992088]
140374e2b:  F3 0F 59 15 F5 5A 59 00  MULSS XMM2,dword ptr [0x14090a928]
140374e33:  F3 0F 10 05 3D 50 59 00  MOVSS XMM0,dword ptr [0x140909e78]
140374e3b:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
140374e41:  F3 0F 10 0D BB 4E 59 00  MOVSS XMM1,dword ptr [0x140909d04]
140374e49:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
140374e4f:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140374e55:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
140374e5b:  0F 28 DA                 MOVAPS XMM3,XMM2
140374e5e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140374e63:  48 8D 0D F6 91 64 00     LEA RCX,[0x1409be060]
140374e6a:  FF 15 E8 79 4F 00        CALL qword ptr [0x14086c858]
140374e70:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140374e75:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140374e7a:  4C 8D 05 F7 80 54 00     LEA R8,[0x1408bcf78]
140374e81:  83 CA FF                 OR EDX,0xffffffff
140374e84:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140374e89:  E8 92 5E EE FF           CALL 0x14025ad20
140374e8e:  90                       NOP
140374e8f:  45 8B D6                 MOV R10D,R14D
140374e92:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140374e96:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140374e9a:  4C 2B DF                 SUB R11,RDI
140374e9d:  49 C1 FB 04              SAR R11,0x4
140374ea1:  45 85 DB                 TEST R11D,R11D
140374ea4:  74 6D                    JZ 0x140374f13
140374ea6:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140374eab:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140374eb0:  41 8B C2                 MOV EAX,R10D
140374eb3:  48 03 C0                 ADD RAX,RAX
140374eb6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140374eba:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140374ebe:  85 D2                    TEST EDX,EDX
140374ec0:  79 17                    JNS 0x140374ed9
140374ec2:  48 85 C9                 TEST RCX,RCX
140374ec5:  74 44                    JZ 0x140374f0b
140374ec7:  48 83 CA FF              OR RDX,-0x1
140374ecb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140374ed0:  48 FF C2                 INC RDX
140374ed3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140374ed7:  75 F7                    JNZ 0x140374ed0
140374ed9:  48 85 C9                 TEST RCX,RCX
140374edc:  74 2D                    JZ 0x140374f0b
140374ede:  41 3B D4                 CMP EDX,R12D
140374ee1:  75 28                    JNZ 0x140374f0b
140374ee3:  4C 63 C2                 MOVSXD R8,EDX
140374ee6:  85 D2                    TEST EDX,EDX
140374ee8:  7E 3C                    JLE 0x140374f26
140374eea:  48 8B D1                 MOV RDX,RCX
140374eed:  48 F7 DA                 NEG RDX
140374ef0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140374ef4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140374ef7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140374efb:  75 0E                    JNZ 0x140374f0b
140374efd:  48 FF C1                 INC RCX
140374f00:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140374f04:  49 3B C0                 CMP RAX,R8
140374f07:  7D 1D                    JGE 0x140374f26
140374f09:  EB E9                    JMP 0x140374ef4
140374f0b:  41 FF C2                 INC R10D
140374f0e:  45 3B D3                 CMP R10D,R11D
140374f11:  72 9D                    JC 0x140374eb0
140374f13:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140374f18:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140374f1c:  E8 CF A7 0D 00           CALL 0x14044f6f0
140374f21:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140374f26:  48 85 DB                 TEST RBX,RBX
140374f29:  74 08                    JZ 0x140374f33
140374f2b:  48 8B CB                 MOV RCX,RBX
140374f2e:  E8 AD 90 4D 00           CALL 0x14084dfe0
140374f33:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140374f38:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140374f3d:  4C 8D 05 3C 7F 54 00     LEA R8,[0x1408bce80]
140374f44:  83 CA FF                 OR EDX,0xffffffff
140374f47:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140374f4c:  E8 CF 5D EE FF           CALL 0x14025ad20
140374f51:  90                       NOP
140374f52:  45 8B D6                 MOV R10D,R14D
140374f55:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140374f59:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140374f5d:  4C 2B DF                 SUB R11,RDI
140374f60:  49 C1 FB 04              SAR R11,0x4
140374f64:  45 85 DB                 TEST R11D,R11D
140374f67:  74 7A                    JZ 0x140374fe3
140374f69:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140374f6e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140374f73:  0F 1F 40 00              NOP dword ptr [RAX]
140374f77:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140374f80:  41 8B C2                 MOV EAX,R10D
140374f83:  48 03 C0                 ADD RAX,RAX
140374f86:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140374f8a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140374f8e:  85 D2                    TEST EDX,EDX
140374f90:  79 17                    JNS 0x140374fa9
140374f92:  48 85 C9                 TEST RCX,RCX
140374f95:  74 44                    JZ 0x140374fdb
140374f97:  48 83 CA FF              OR RDX,-0x1
140374f9b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140374fa0:  48 FF C2                 INC RDX
140374fa3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140374fa7:  75 F7                    JNZ 0x140374fa0
140374fa9:  48 85 C9                 TEST RCX,RCX
140374fac:  74 2D                    JZ 0x140374fdb
140374fae:  41 3B D4                 CMP EDX,R12D
140374fb1:  75 28                    JNZ 0x140374fdb
140374fb3:  4C 63 C2                 MOVSXD R8,EDX
140374fb6:  85 D2                    TEST EDX,EDX
140374fb8:  7E 3C                    JLE 0x140374ff6
140374fba:  48 8B D1                 MOV RDX,RCX
140374fbd:  48 F7 DA                 NEG RDX
140374fc0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140374fc4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140374fc7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140374fcb:  75 0E                    JNZ 0x140374fdb
140374fcd:  48 FF C1                 INC RCX
140374fd0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140374fd4:  49 3B C0                 CMP RAX,R8
140374fd7:  7D 1D                    JGE 0x140374ff6
140374fd9:  EB E9                    JMP 0x140374fc4
140374fdb:  41 FF C2                 INC R10D
140374fde:  45 3B D3                 CMP R10D,R11D
140374fe1:  72 9D                    JC 0x140374f80
140374fe3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140374fe8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140374fec:  E8 FF A6 0D 00           CALL 0x14044f6f0
140374ff1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140374ff6:  48 85 DB                 TEST RBX,RBX
140374ff9:  74 08                    JZ 0x140375003
140374ffb:  48 8B CB                 MOV RCX,RBX
140374ffe:  E8 DD 8F 4D 00           CALL 0x14084dfe0
140375003:  41 B4 01                 MOV R12B,0x1
140375006:  F3 0F 10 35 7A D0 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037500e:  EB 04                    JMP 0x140375014
140375010:  44 8B 65 AC              MOV R12D,dword ptr [RBP + -0x54]
140375014:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140375019:  BA DE 05 00 00           MOV EDX,0x5de
14037501e:  48 8D 0D 6B 25 62 00     LEA RCX,[0x140997590]
140375025:  FF 15 65 78 4F 00        CALL qword ptr [0x14086c890]
14037502b:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037502f:  F3 0F 5C DE              SUBSS XMM3,XMM6
140375033:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140375038:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037503d:  48 8D 05 2C 7E 54 00     LEA RAX,[0x1408bce70]
140375044:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140375049:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140375051:  66 41 0F 7E D9           MOVD R9D,XMM3
140375056:  0F 28 D6                 MOVAPS XMM2,XMM6
140375059:  66 41 0F 7E D0           MOVD R8D,XMM2
14037505e:  49 8B D7                 MOV RDX,R15
140375061:  48 8D 0D 48 1F 62 00     LEA RCX,[0x140996fb0]
140375068:  FF 15 12 78 4F 00        CALL qword ptr [0x14086c880]
14037506e:  BA 77 06 00 00           MOV EDX,0x677
140375073:  48 8D 0D 16 25 62 00     LEA RCX,[0x140997590]
14037507a:  FF 15 10 78 4F 00        CALL qword ptr [0x14086c890]
140375080:  F3 0F 10 05 00 D0 61 00  MOVSS XMM0,dword ptr [0x140992088]
140375088:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037508d:  41 0F 28 DA              MOVAPS XMM3,XMM10
140375091:  F3 0F 5C D8              SUBSS XMM3,XMM0
140375095:  66 0F 6E 05 EF 01 62 00  MOVD XMM0,dword ptr [0x14099528c]
14037509d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403750a0:  F3 41 0F 59 C3           MULSS XMM0,XMM11
1403750a5:  0F 28 D7                 MOVAPS XMM2,XMM7
1403750a8:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403750ac:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403750b1:  48 8D 05 A8 7D 54 00     LEA RAX,[0x1408bce60]
1403750b8:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403750bd:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403750c5:  66 41 0F 7E D9           MOVD R9D,XMM3
1403750ca:  66 41 0F 7E D0           MOVD R8D,XMM2
1403750cf:  49 8B D7                 MOV RDX,R15
1403750d2:  48 8D 0D D7 1E 62 00     LEA RCX,[0x140996fb0]
1403750d9:  FF 15 A9 77 4F 00        CALL qword ptr [0x14086c888]
1403750df:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
1403750e6:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403750e9:  48 8D 0D E0 5B 67 00     LEA RCX,[0x1409eacd0]
1403750f0:  FF 15 FA 75 4F 00        CALL qword ptr [0x14086c6f0]
1403750f6:  4C 8B C0                 MOV R8,RAX
1403750f9:  33 D2                    XOR EDX,EDX
1403750fb:  48 8B CB                 MOV RCX,RBX
1403750fe:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140375101:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375108:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037510c:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375110:  41 0F 28 C9              MOVAPS XMM1,XMM9
140375114:  48 8D 8D 90 0A 00 00     LEA RCX,[RBP + 0xa90]
14037511b:  FF 15 FF 6E 4F 00        CALL qword ptr [0x14086c020]
140375121:  48 8B D0                 MOV RDX,RAX
140375124:  48 8D 0D 35 8F 64 00     LEA RCX,[0x1409be060]
14037512b:  FF 15 67 77 4F 00        CALL qword ptr [0x14086c898]
140375131:  F3 0F 10 0D 4F CF 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375139:  F3 0F 59 0D CB 58 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140375141:  0F 28 D1                 MOVAPS XMM2,XMM1
140375144:  48 8D 0D 15 8F 64 00     LEA RCX,[0x1409be060]
14037514b:  FF 15 57 77 4F 00        CALL qword ptr [0x14086c8a8]
140375151:  F3 0F 10 15 2F CF 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375159:  F3 0F 59 15 C7 56 59 00  MULSS XMM2,dword ptr [0x14090a828]
140375161:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140375166:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037516a:  41 0F 28 CC              MOVAPS XMM1,XMM12
14037516e:  48 8D 8D 58 03 00 00     LEA RCX,[RBP + 0x358]
140375175:  FF 15 8D 6E 4F 00        CALL qword ptr [0x14086c008]
14037517b:  48 8B D0                 MOV RDX,RAX
14037517e:  48 8D 0D DB 8E 64 00     LEA RCX,[0x1409be060]
140375185:  FF 15 15 77 4F 00        CALL qword ptr [0x14086c8a0]
14037518b:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140375190:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140375197:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037519e:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403751a2:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403751a6:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403751aa:  48 8D 0D AF 8E 64 00     LEA RCX,[0x1409be060]
1403751b1:  FF 15 A9 76 4F 00        CALL qword ptr [0x14086c860]
1403751b7:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
1403751be:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403751c1:  48 8D 0D 08 5B 67 00     LEA RCX,[0x1409eacd0]
1403751c8:  FF 15 22 75 4F 00        CALL qword ptr [0x14086c6f0]
1403751ce:  4C 8B C0                 MOV R8,RAX
1403751d1:  33 D2                    XOR EDX,EDX
1403751d3:  48 8B CB                 MOV RCX,RBX
1403751d6:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403751d9:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403751e0:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403751e4:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403751e8:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403751ec:  48 8D 8D A0 0A 00 00     LEA RCX,[RBP + 0xaa0]
1403751f3:  FF 15 27 6E 4F 00        CALL qword ptr [0x14086c020]
1403751f9:  48 8B D0                 MOV RDX,RAX
1403751fc:  48 8D 0D 5D 8E 64 00     LEA RCX,[0x1409be060]
140375203:  FF 15 8F 76 4F 00        CALL qword ptr [0x14086c898]
140375209:  F3 0F 10 0D 77 CE 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375211:  F3 0F 59 0D F3 57 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140375219:  0F 28 D1                 MOVAPS XMM2,XMM1
14037521c:  48 8D 0D 3D 8E 64 00     LEA RCX,[0x1409be060]
140375223:  FF 15 7F 76 4F 00        CALL qword ptr [0x14086c8a8]
140375229:  F3 0F 10 15 57 CE 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375231:  F3 0F 59 15 EF 55 59 00  MULSS XMM2,dword ptr [0x14090a828]
140375239:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037523e:  41 0F 28 D8              MOVAPS XMM3,XMM8
140375242:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140375248:  48 8D 8D 64 03 00 00     LEA RCX,[RBP + 0x364]
14037524f:  FF 15 B3 6D 4F 00        CALL qword ptr [0x14086c008]
140375255:  48 8B D0                 MOV RDX,RAX
140375258:  48 8D 0D 01 8E 64 00     LEA RCX,[0x1409be060]
14037525f:  FF 15 3B 76 4F 00        CALL qword ptr [0x14086c8a0]
140375265:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037526a:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140375271:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375278:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037527c:  41 0F 28 D0              MOVAPS XMM2,XMM8
140375280:  41 0F 28 C8              MOVAPS XMM1,XMM8
140375284:  48 8D 0D D5 8D 64 00     LEA RCX,[0x1409be060]
14037528b:  FF 15 CF 75 4F 00        CALL qword ptr [0x14086c860]
140375291:  F3 0F 10 74 24 78        MOVSS XMM6,dword ptr [RSP + 0x78]
140375297:  44 0F 28 E6              MOVAPS XMM12,XMM6
14037529b:  F3 44 0F 59 25 08 4C 59 00 MULSS XMM12,dword ptr [0x140909eac]
1403752a4:  F3 44 0F 11 65 AC        MOVSS dword ptr [RBP + -0x54],XMM12
1403752aa:  F3 45 0F 58 D4           ADDSS XMM10,XMM12
1403752af:  8B 05 BB FF 61 00        MOV EAX,dword ptr [0x140995270]
1403752b5:  83 F8 02                 CMP EAX,0x2
1403752b8:  75 09                    JNZ 0x1403752c3
1403752ba:  48 8D 05 FF 7A 54 00     LEA RAX,[0x1408bcdc0]
1403752c1:  EB 13                    JMP 0x1403752d6
1403752c3:  83 F8 01                 CMP EAX,0x1
1403752c6:  48 8D 05 C3 7C 54 00     LEA RAX,[0x1408bcf90]
1403752cd:  74 07                    JZ 0x1403752d6
1403752cf:  48 8D 05 DA 7B 54 00     LEA RAX,[0x1408bceb0]
1403752d6:  F3 0F 10 15 AA CD 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403752de:  F3 41 0F 59 D0           MULSS XMM2,XMM8
1403752e3:  0F 28 DA                 MOVAPS XMM3,XMM2
1403752e6:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403752eb:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
1403752f0:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
1403752f5:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403752fd:  66 41 0F 7E D9           MOVD R9D,XMM3
140375302:  66 41 0F 7E D0           MOVD R8D,XMM2
140375307:  49 8B D7                 MOV RDX,R15
14037530a:  48 8D 0D 9F 1C 62 00     LEA RCX,[0x140996fb0]
140375311:  FF 15 69 75 4F 00        CALL qword ptr [0x14086c880]
140375317:  F3 45 0F 58 D4           ADDSS XMM10,XMM12
14037531c:  F3 0F 10 0D 64 CD 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375324:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140375329:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037532d:  F3 0F 5C C1              SUBSS XMM0,XMM1
140375331:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140375335:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140375339:  0F 28 FE                 MOVAPS XMM7,XMM6
14037533c:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
140375341:  F3 0F 58 CF              ADDSS XMM1,XMM7
140375345:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140375349:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037534d:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140375354:  75 45                    JNZ 0x14037539b
140375356:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037535b:  48 C1 E9 20              SHR RCX,0x20
14037535f:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140375364:  48 C1 E8 20              SHR RAX,0x20
140375368:  03 C8                    ADD ECX,EAX
14037536a:  66 0F 6E D1              MOVD XMM2,ECX
14037536e:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140375371:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140375376:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
14037537a:  41 03 C5                 ADD EAX,R13D
14037537d:  66 0F 6E C8              MOVD XMM1,EAX
140375381:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140375384:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140375389:  41 B9 06 00 00 00        MOV R9D,0x6
14037538f:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140375396:  E8 55 0E 0D 00           CALL 0x1404461f0
14037539b:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
1403753a0:  0F 84 48 01 00 00        JZ 0x1403754ee
1403753a6:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
1403753ac:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403753af:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
1403753b4:  0F 2F C8                 COMISS XMM1,XMM0
1403753b7:  0F 86 31 01 00 00        JBE 0x1403754ee
1403753bd:  66 41 0F 6E C5           MOVD XMM0,R13D
1403753c2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403753c5:  0F 2F C1                 COMISS XMM0,XMM1
1403753c8:  0F 86 20 01 00 00        JBE 0x1403754ee
1403753ce:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
1403753d3:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
1403753d8:  66 0F 6E C3              MOVD XMM0,EBX
1403753dc:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403753df:  0F 2F C8                 COMISS XMM1,XMM0
1403753e2:  0F 86 06 01 00 00        JBE 0x1403754ee
1403753e8:  66 0F 6E C7              MOVD XMM0,EDI
1403753ec:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403753ef:  0F 2F C1                 COMISS XMM0,XMM1
1403753f2:  0F 86 F6 00 00 00        JBE 0x1403754ee
1403753f8:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
1403753ff:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140375402:  48 8D 0D C7 58 67 00     LEA RCX,[0x1409eacd0]
140375409:  FF 15 E1 72 4F 00        CALL qword ptr [0x14086c6f0]
14037540f:  4C 8B C0                 MOV R8,RAX
140375412:  33 D2                    XOR EDX,EDX
140375414:  48 8B CB                 MOV RCX,RBX
140375417:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037541a:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140375421:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375425:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375429:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037542d:  48 8D 8D B0 0A 00 00     LEA RCX,[RBP + 0xab0]
140375434:  FF 15 E6 6B 4F 00        CALL qword ptr [0x14086c020]
14037543a:  48 8B D0                 MOV RDX,RAX
14037543d:  48 8D 0D 1C 8C 64 00     LEA RCX,[0x1409be060]
140375444:  FF 15 4E 74 4F 00        CALL qword ptr [0x14086c898]
14037544a:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037544e:  48 8D 0D 0B 8C 64 00     LEA RCX,[0x1409be060]
140375455:  FF 15 FD 74 4F 00        CALL qword ptr [0x14086c958]
14037545b:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140375460:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140375466:  F3 0F 10 15 1A CC 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037546e:  F3 0F 59 15 B2 53 59 00  MULSS XMM2,dword ptr [0x14090a828]
140375476:  F3 0F 10 05 FA 49 59 00  MOVSS XMM0,dword ptr [0x140909e78]
14037547e:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
140375484:  F3 0F 10 0D 78 48 59 00  MOVSS XMM1,dword ptr [0x140909d04]
14037548c:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
140375492:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140375498:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
14037549e:  0F 28 DA                 MOVAPS XMM3,XMM2
1403754a1:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403754a6:  48 8D 0D B3 8B 64 00     LEA RCX,[0x1409be060]
1403754ad:  FF 15 A5 73 4F 00        CALL qword ptr [0x14086c858]
1403754b3:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403754b7:  48 8D 0D A2 8B 64 00     LEA RCX,[0x1409be060]
1403754be:  FF 15 94 74 4F 00        CALL qword ptr [0x14086c958]
1403754c4:  48 8D 0D C5 F6 6D 00     LEA RCX,[0x140a54b90]
1403754cb:  FF 15 E7 73 4F 00        CALL qword ptr [0x14086c8b8]
1403754d1:  84 C0                    TEST AL,AL
1403754d3:  74 19                    JZ 0x1403754ee
1403754d5:  41 B0 01                 MOV R8B,0x1
1403754d8:  48 8D 15 C1 79 54 00     LEA RDX,[0x1408bcea0]
1403754df:  48 8B CE                 MOV RCX,RSI
1403754e2:  E8 E9 A5 08 00           CALL 0x1403ffad0
1403754e7:  44 89 B6 D4 C7 00 00     MOV dword ptr [RSI + 0xc7d4],R14D
1403754ee:  F3 0F 10 35 92 CB 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403754f6:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403754fb:  BA DF 05 00 00           MOV EDX,0x5df
140375500:  48 8D 0D 89 20 62 00     LEA RCX,[0x140997590]
140375507:  FF 15 83 73 4F 00        CALL qword ptr [0x14086c890]
14037550d:  41 0F 28 DA              MOVAPS XMM3,XMM10
140375511:  F3 0F 5C DE              SUBSS XMM3,XMM6
140375515:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037551a:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037551f:  48 8D 05 6A 79 54 00     LEA RAX,[0x1408bce90]
140375526:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037552b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140375533:  66 41 0F 7E D9           MOVD R9D,XMM3
140375538:  0F 28 D6                 MOVAPS XMM2,XMM6
14037553b:  66 41 0F 7E D0           MOVD R8D,XMM2
140375540:  49 8B D7                 MOV RDX,R15
140375543:  48 8D 0D 66 1A 62 00     LEA RCX,[0x140996fb0]
14037554a:  FF 15 30 73 4F 00        CALL qword ptr [0x14086c880]
140375550:  F3 0F 10 95 94 00 00 00  MOVSS XMM2,dword ptr [RBP + 0x94]
140375558:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037555b:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140375560:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140375565:  45 33 C9                 XOR R9D,R9D
140375568:  48 8D 95 A0 02 00 00     LEA RDX,[RBP + 0x2a0]
14037556f:  48 8B CE                 MOV RCX,RSI
140375572:  E8 E9 16 FC FF           CALL 0x140336c60
140375577:  90                       NOP
140375578:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037557b:  F3 0F 10 15 05 CB 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375583:  0F 28 C2                 MOVAPS XMM0,XMM2
140375586:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037558b:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037558f:  F3 0F 5C D8              SUBSS XMM3,XMM0
140375593:  F3 0F 10 35 CD 53 59 00  MOVSS XMM6,dword ptr [0x14090a968]
14037559b:  F3 0F 59 D6              MULSS XMM2,XMM6
14037559f:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
1403755a4:  66 0F 6E 05 E0 FC 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403755ac:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403755af:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403755b3:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
1403755b8:  48 8D 05 D1 7D 54 00     LEA RAX,[0x1408bd390]
1403755bf:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403755c4:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403755cc:  66 41 0F 7E D9           MOVD R9D,XMM3
1403755d1:  66 41 0F 7E D0           MOVD R8D,XMM2
1403755d6:  49 8B D7                 MOV RDX,R15
1403755d9:  48 8D 0D D0 19 62 00     LEA RCX,[0x140996fb0]
1403755e0:  FF 15 92 72 4F 00        CALL qword ptr [0x14086c878]
1403755e6:  90                       NOP
1403755e7:  48 8B 8D A0 02 00 00     MOV RCX,qword ptr [RBP + 0x2a0]
1403755ee:  48 85 C9                 TEST RCX,RCX
1403755f1:  74 13                    JZ 0x140375606
1403755f3:  E8 E8 89 4D 00           CALL 0x14084dfe0
1403755f8:  4C 89 B5 A0 02 00 00     MOV qword ptr [RBP + 0x2a0],R14
1403755ff:  44 89 B5 A8 02 00 00     MOV dword ptr [RBP + 0x2a8],R14D
140375606:  F3 0F 10 8D 98 00 00 00  MOVSS XMM1,dword ptr [RBP + 0x98]
14037560e:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140375611:  41 B1 01                 MOV R9B,0x1
140375614:  45 33 C0                 XOR R8D,R8D
140375617:  48 8B CE                 MOV RCX,RSI
14037561a:  E8 01 0E FC FF           CALL 0x140336420
14037561f:  F3 0F 10 15 61 CA 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375627:  0F 28 C2                 MOVAPS XMM0,XMM2
14037562a:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037562f:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
140375634:  F3 0F 59 D6              MULSS XMM2,XMM6
140375638:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037563e:  66 0F 6E 05 46 FC 61 00  MOVD XMM0,dword ptr [0x14099528c]
140375646:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375649:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037564d:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140375652:  48 8D 05 2F 7D 54 00     LEA RAX,[0x1408bd388]
140375659:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037565e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140375666:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037566a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037566f:  66 41 0F 7E D0           MOVD R8D,XMM2
140375674:  49 8B D7                 MOV RDX,R15
140375677:  48 8D 0D 32 19 62 00     LEA RCX,[0x140996fb0]
14037567e:  FF 15 F4 71 4F 00        CALL qword ptr [0x14086c878]
140375684:  F3 0F 10 0D FC C9 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037568c:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140375691:  0F 28 C7                 MOVAPS XMM0,XMM7
140375694:  F3 0F 5C C1              SUBSS XMM0,XMM1
140375698:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037569c:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
1403756a0:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
1403756a7:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
1403756ac:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
1403756b1:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
1403756b5:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
1403756b9:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
1403756c0:  75 45                    JNZ 0x140375707
1403756c2:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
1403756c7:  48 C1 E9 20              SHR RCX,0x20
1403756cb:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
1403756d0:  48 C1 E8 20              SHR RAX,0x20
1403756d4:  03 C8                    ADD ECX,EAX
1403756d6:  66 0F 6E D1              MOVD XMM2,ECX
1403756da:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
1403756dd:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1403756e2:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
1403756e6:  41 03 C5                 ADD EAX,R13D
1403756e9:  66 0F 6E C8              MOVD XMM1,EAX
1403756ed:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1403756f0:  F3 41 0F 59 CB           MULSS XMM1,XMM11
1403756f5:  41 B9 06 00 00 00        MOV R9D,0x6
1403756fb:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140375702:  E8 E9 0A 0D 00           CALL 0x1404461f0
140375707:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037570c:  0F 84 4C 01 00 00        JZ 0x14037585e
140375712:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140375718:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037571b:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140375720:  0F 2F C8                 COMISS XMM1,XMM0
140375723:  0F 86 35 01 00 00        JBE 0x14037585e
140375729:  66 41 0F 6E C5           MOVD XMM0,R13D
14037572e:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375731:  0F 2F C1                 COMISS XMM0,XMM1
140375734:  0F 86 24 01 00 00        JBE 0x14037585e
14037573a:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037573f:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140375744:  66 0F 6E C3              MOVD XMM0,EBX
140375748:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037574b:  0F 2F C8                 COMISS XMM1,XMM0
14037574e:  0F 86 0A 01 00 00        JBE 0x14037585e
140375754:  66 0F 6E C7              MOVD XMM0,EDI
140375758:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037575b:  0F 2F C1                 COMISS XMM0,XMM1
14037575e:  0F 86 FA 00 00 00        JBE 0x14037585e
140375764:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037576b:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037576e:  48 8D 0D 5B 55 67 00     LEA RCX,[0x1409eacd0]
140375775:  FF 15 75 6F 4F 00        CALL qword ptr [0x14086c6f0]
14037577b:  4C 8B C0                 MOV R8,RAX
14037577e:  33 D2                    XOR EDX,EDX
140375780:  48 8B CB                 MOV RCX,RBX
140375783:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140375786:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037578d:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375791:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375795:  41 0F 28 C9              MOVAPS XMM1,XMM9
140375799:  48 8D 8D C0 0A 00 00     LEA RCX,[RBP + 0xac0]
1403757a0:  FF 15 7A 68 4F 00        CALL qword ptr [0x14086c020]
1403757a6:  48 8B D0                 MOV RDX,RAX
1403757a9:  48 8D 0D B0 88 64 00     LEA RCX,[0x1409be060]
1403757b0:  FF 15 E2 70 4F 00        CALL qword ptr [0x14086c898]
1403757b6:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403757ba:  48 8D 0D 9F 88 64 00     LEA RCX,[0x1409be060]
1403757c1:  FF 15 91 71 4F 00        CALL qword ptr [0x14086c958]
1403757c7:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
1403757cc:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
1403757d2:  F3 0F 10 15 AE C8 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403757da:  F3 0F 59 15 46 50 59 00  MULSS XMM2,dword ptr [0x14090a828]
1403757e2:  F3 0F 10 05 8E 46 59 00  MOVSS XMM0,dword ptr [0x140909e78]
1403757ea:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1403757f0:  F3 0F 10 0D 0C 45 59 00  MOVSS XMM1,dword ptr [0x140909d04]
1403757f8:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
1403757fe:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140375804:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
14037580a:  0F 28 DA                 MOVAPS XMM3,XMM2
14037580d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140375812:  48 8D 0D 47 88 64 00     LEA RCX,[0x1409be060]
140375819:  FF 15 39 70 4F 00        CALL qword ptr [0x14086c858]
14037581f:  41 0F 28 C8              MOVAPS XMM1,XMM8
140375823:  48 8D 0D 36 88 64 00     LEA RCX,[0x1409be060]
14037582a:  FF 15 28 71 4F 00        CALL qword ptr [0x14086c958]
140375830:  48 8D 0D 59 F3 6D 00     LEA RCX,[0x140a54b90]
140375837:  FF 15 7B 70 4F 00        CALL qword ptr [0x14086c8b8]
14037583d:  84 C0                    TEST AL,AL
14037583f:  74 1D                    JZ 0x14037585e
140375841:  41 B0 01                 MOV R8B,0x1
140375844:  48 8D 15 2D 7B 54 00     LEA RDX,[0x1408bd378]
14037584b:  48 8B CE                 MOV RCX,RSI
14037584e:  E8 7D A2 08 00           CALL 0x1403ffad0
140375853:  B8 01 00 00 00           MOV EAX,0x1
140375858:  89 86 D4 C7 00 00        MOV dword ptr [RSI + 0xc7d4],EAX
14037585e:  F3 0F 10 35 22 C8 61 00  MOVSS XMM6,dword ptr [0x140992088]
140375866:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037586b:  BA E0 05 00 00           MOV EDX,0x5e0
140375870:  48 8D 0D 19 1D 62 00     LEA RCX,[0x140997590]
140375877:  FF 15 13 70 4F 00        CALL qword ptr [0x14086c890]
14037587d:  0F 28 DF                 MOVAPS XMM3,XMM7
140375880:  F3 0F 5C DE              SUBSS XMM3,XMM6
140375884:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140375889:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037588e:  48 8D 05 D3 7A 54 00     LEA RAX,[0x1408bd368]
140375895:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037589a:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403758a2:  66 41 0F 7E D9           MOVD R9D,XMM3
1403758a7:  0F 28 D6                 MOVAPS XMM2,XMM6
1403758aa:  66 41 0F 7E D0           MOVD R8D,XMM2
1403758af:  49 8B D7                 MOV RDX,R15
1403758b2:  48 8D 0D F7 16 62 00     LEA RCX,[0x140996fb0]
1403758b9:  FF 15 C1 6F 4F 00        CALL qword ptr [0x14086c880]
1403758bf:  F3 0F 10 95 9C 00 00 00  MOVSS XMM2,dword ptr [RBP + 0x9c]
1403758c7:  0F 5A D2                 CVTPS2PD XMM2,XMM2
1403758ca:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
1403758cf:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
1403758d4:  45 33 C9                 XOR R9D,R9D
1403758d7:  48 8D 95 B0 02 00 00     LEA RDX,[RBP + 0x2b0]
1403758de:  48 8B CE                 MOV RCX,RSI
1403758e1:  E8 7A 13 FC FF           CALL 0x140336c60
1403758e6:  90                       NOP
1403758e7:  48 8B 08                 MOV RCX,qword ptr [RAX]
1403758ea:  F3 0F 10 15 96 C7 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403758f2:  0F 28 C2                 MOVAPS XMM0,XMM2
1403758f5:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403758fa:  0F 28 DF                 MOVAPS XMM3,XMM7
1403758fd:  F3 0F 5C D8              SUBSS XMM3,XMM0
140375901:  F3 0F 10 35 5F 50 59 00  MOVSS XMM6,dword ptr [0x14090a968]
140375909:  F3 0F 59 D6              MULSS XMM2,XMM6
14037590d:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
140375912:  66 0F 6E 05 72 F9 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037591a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037591d:  F3 0F 5C D0              SUBSS XMM2,XMM0
140375921:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140375926:  48 8D 05 93 7A 54 00     LEA RAX,[0x1408bd3c0]
14037592d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140375932:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037593a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037593f:  66 41 0F 7E D0           MOVD R8D,XMM2
140375944:  49 8B D7                 MOV RDX,R15
140375947:  48 8D 0D 62 16 62 00     LEA RCX,[0x140996fb0]
14037594e:  FF 15 24 6F 4F 00        CALL qword ptr [0x14086c878]
140375954:  90                       NOP
140375955:  48 8B 8D B0 02 00 00     MOV RCX,qword ptr [RBP + 0x2b0]
14037595c:  48 85 C9                 TEST RCX,RCX
14037595f:  74 13                    JZ 0x140375974
140375961:  E8 7A 86 4D 00           CALL 0x14084dfe0
140375966:  4C 89 B5 B0 02 00 00     MOV qword ptr [RBP + 0x2b0],R14
14037596d:  44 89 B5 B8 02 00 00     MOV dword ptr [RBP + 0x2b8],R14D
140375974:  F3 0F 10 8D A0 00 00 00  MOVSS XMM1,dword ptr [RBP + 0xa0]
14037597c:  0F 5A C9                 CVTPS2PD XMM1,XMM1
14037597f:  41 B1 01                 MOV R9B,0x1
140375982:  45 33 C0                 XOR R8D,R8D
140375985:  48 8B CE                 MOV RCX,RSI
140375988:  E8 93 0A FC FF           CALL 0x140336420
14037598d:  F3 0F 10 15 F3 C6 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375995:  0F 28 C2                 MOVAPS XMM0,XMM2
140375998:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037599d:  F3 0F 5C F8              SUBSS XMM7,XMM0
1403759a1:  F3 0F 59 D6              MULSS XMM2,XMM6
1403759a5:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
1403759ab:  66 0F 6E 05 D9 F8 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403759b3:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403759b6:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403759ba:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403759bf:  48 8D 05 F2 79 54 00     LEA RAX,[0x1408bd3b8]
1403759c6:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403759cb:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403759d3:  0F 28 DF                 MOVAPS XMM3,XMM7
1403759d6:  66 41 0F 7E D9           MOVD R9D,XMM3
1403759db:  66 41 0F 7E D0           MOVD R8D,XMM2
1403759e0:  49 8B D7                 MOV RDX,R15
1403759e3:  48 8D 0D C6 15 62 00     LEA RCX,[0x140996fb0]
1403759ea:  FF 15 88 6E 4F 00        CALL qword ptr [0x14086c878]
1403759f0:  F3 0F 10 35 90 C6 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403759f8:  0F 28 FE                 MOVAPS XMM7,XMM6
1403759fb:  F3 0F 59 3D 25 4E 59 00  MULSS XMM7,dword ptr [0x14090a828]
140375a03:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
140375a08:  0F 28 D6                 MOVAPS XMM2,XMM6
140375a0b:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140375a10:  0F 28 C7                 MOVAPS XMM0,XMM7
140375a13:  F3 0F 5C C2              SUBSS XMM0,XMM2
140375a17:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
140375a1b:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
140375a1f:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
140375a26:  41 0F 28 CA              MOVAPS XMM1,XMM10
140375a2a:  F3 0F 59 0D FE 4C 59 00  MULSS XMM1,dword ptr [0x14090a730]
140375a32:  F3 0F 58 CF              ADDSS XMM1,XMM7
140375a36:  F3 0F 58 CA              ADDSS XMM1,XMM2
140375a3a:  F3 0F 2C C9              CVTTSS2SI ECX,XMM1
140375a3e:  89 4C 24 6C              MOV dword ptr [RSP + 0x6c],ECX
140375a42:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140375a47:  0F 84 A2 02 00 00        JZ 0x140375cef
140375a4d:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140375a53:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375a56:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140375a5b:  0F 2F C8                 COMISS XMM1,XMM0
140375a5e:  0F 86 8B 02 00 00        JBE 0x140375cef
140375a64:  66 41 0F 6E C5           MOVD XMM0,R13D
140375a69:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375a6c:  0F 2F C1                 COMISS XMM0,XMM1
140375a6f:  0F 86 7A 02 00 00        JBE 0x140375cef
140375a75:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140375a7a:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140375a7f:  66 0F 6E C0              MOVD XMM0,EAX
140375a83:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375a86:  0F 2F C8                 COMISS XMM1,XMM0
140375a89:  0F 86 60 02 00 00        JBE 0x140375cef
140375a8f:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140375a94:  66 0F 6E C1              MOVD XMM0,ECX
140375a98:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375a9b:  0F 2F C1                 COMISS XMM0,XMM1
140375a9e:  0F 86 4B 02 00 00        JBE 0x140375cef
140375aa4:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140375aab:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140375aae:  48 8D 0D 1B 52 67 00     LEA RCX,[0x1409eacd0]
140375ab5:  FF 15 35 6C 4F 00        CALL qword ptr [0x14086c6f0]
140375abb:  4C 8B C0                 MOV R8,RAX
140375abe:  33 D2                    XOR EDX,EDX
140375ac0:  48 8B CB                 MOV RCX,RBX
140375ac3:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140375ac6:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140375acd:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375ad1:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375ad5:  41 0F 28 C9              MOVAPS XMM1,XMM9
140375ad9:  48 8D 8D D0 0A 00 00     LEA RCX,[RBP + 0xad0]
140375ae0:  FF 15 3A 65 4F 00        CALL qword ptr [0x14086c020]
140375ae6:  48 8B D0                 MOV RDX,RAX
140375ae9:  48 8D 0D 70 85 64 00     LEA RCX,[0x1409be060]
140375af0:  FF 15 A2 6D 4F 00        CALL qword ptr [0x14086c898]
140375af6:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140375afb:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140375b01:  F3 0F 10 15 7F C5 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375b09:  F3 0F 59 15 17 4E 59 00  MULSS XMM2,dword ptr [0x14090a928]
140375b11:  F3 0F 10 05 5F 43 59 00  MOVSS XMM0,dword ptr [0x140909e78]
140375b19:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
140375b1f:  F3 0F 10 0D DD 41 59 00  MOVSS XMM1,dword ptr [0x140909d04]
140375b27:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
140375b2d:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140375b33:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
140375b39:  0F 28 DA                 MOVAPS XMM3,XMM2
140375b3c:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140375b41:  48 8D 0D 18 85 64 00     LEA RCX,[0x1409be060]
140375b48:  FF 15 0A 6D 4F 00        CALL qword ptr [0x14086c858]
140375b4e:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140375b53:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140375b58:  4C 8D 05 49 78 54 00     LEA R8,[0x1408bd3a8]
140375b5f:  83 CA FF                 OR EDX,0xffffffff
140375b62:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140375b67:  E8 B4 51 EE FF           CALL 0x14025ad20
140375b6c:  90                       NOP
140375b6d:  45 8B D6                 MOV R10D,R14D
140375b70:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140375b74:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140375b78:  4C 2B DF                 SUB R11,RDI
140375b7b:  49 C1 FB 04              SAR R11,0x4
140375b7f:  45 85 DB                 TEST R11D,R11D
140375b82:  74 6F                    JZ 0x140375bf3
140375b84:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140375b89:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140375b8e:  66 90                    NOP
140375b90:  41 8B C2                 MOV EAX,R10D
140375b93:  48 03 C0                 ADD RAX,RAX
140375b96:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140375b9a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140375b9e:  85 D2                    TEST EDX,EDX
140375ba0:  79 17                    JNS 0x140375bb9
140375ba2:  48 85 C9                 TEST RCX,RCX
140375ba5:  74 44                    JZ 0x140375beb
140375ba7:  48 83 CA FF              OR RDX,-0x1
140375bab:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140375bb0:  48 FF C2                 INC RDX
140375bb3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140375bb7:  75 F7                    JNZ 0x140375bb0
140375bb9:  48 85 C9                 TEST RCX,RCX
140375bbc:  74 2D                    JZ 0x140375beb
140375bbe:  41 3B D4                 CMP EDX,R12D
140375bc1:  75 28                    JNZ 0x140375beb
140375bc3:  4C 63 C2                 MOVSXD R8,EDX
140375bc6:  85 D2                    TEST EDX,EDX
140375bc8:  7E 3C                    JLE 0x140375c06
140375bca:  48 8B D1                 MOV RDX,RCX
140375bcd:  48 F7 DA                 NEG RDX
140375bd0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140375bd4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140375bd7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140375bdb:  75 0E                    JNZ 0x140375beb
140375bdd:  48 FF C1                 INC RCX
140375be0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140375be4:  49 3B C0                 CMP RAX,R8
140375be7:  7D 1D                    JGE 0x140375c06
140375be9:  EB E9                    JMP 0x140375bd4
140375beb:  41 FF C2                 INC R10D
140375bee:  45 3B D3                 CMP R10D,R11D
140375bf1:  72 9D                    JC 0x140375b90
140375bf3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140375bf8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140375bfc:  E8 EF 9A 0D 00           CALL 0x14044f6f0
140375c01:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140375c06:  48 85 DB                 TEST RBX,RBX
140375c09:  74 08                    JZ 0x140375c13
140375c0b:  48 8B CB                 MOV RCX,RBX
140375c0e:  E8 CD 83 4D 00           CALL 0x14084dfe0
140375c13:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140375c18:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140375c1d:  4C 8D 05 74 77 54 00     LEA R8,[0x1408bd398]
140375c24:  83 CA FF                 OR EDX,0xffffffff
140375c27:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140375c2c:  E8 EF 50 EE FF           CALL 0x14025ad20
140375c31:  90                       NOP
140375c32:  45 8B D6                 MOV R10D,R14D
140375c35:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
140375c39:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140375c3d:  48 2B DF                 SUB RBX,RDI
140375c40:  48 C1 FB 04              SAR RBX,0x4
140375c44:  85 DB                    TEST EBX,EBX
140375c46:  74 7C                    JZ 0x140375cc4
140375c48:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140375c4d:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
140375c52:  0F 1F 40 00              NOP dword ptr [RAX]
140375c56:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140375c60:  41 8B C2                 MOV EAX,R10D
140375c63:  48 03 C0                 ADD RAX,RAX
140375c66:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
140375c6a:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
140375c6e:  85 C9                    TEST ECX,ECX
140375c70:  79 17                    JNS 0x140375c89
140375c72:  48 85 D2                 TEST RDX,RDX
140375c75:  74 45                    JZ 0x140375cbc
140375c77:  48 83 C9 FF              OR RCX,-0x1
140375c7b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140375c80:  48 FF C1                 INC RCX
140375c83:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
140375c87:  75 F7                    JNZ 0x140375c80
140375c89:  48 85 D2                 TEST RDX,RDX
140375c8c:  74 2E                    JZ 0x140375cbc
140375c8e:  41 3B CC                 CMP ECX,R12D
140375c91:  75 29                    JNZ 0x140375cbc
140375c93:  4C 63 C9                 MOVSXD R9,ECX
140375c96:  85 C9                    TEST ECX,ECX
140375c98:  7E 3D                    JLE 0x140375cd7
140375c9a:  49 8B CB                 MOV RCX,R11
140375c9d:  49 2B D3                 SUB RDX,R11
140375ca0:  4D 8B C3                 MOV R8,R11
140375ca3:  49 F7 D8                 NEG R8
140375ca6:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
140375caa:  38 01                    CMP byte ptr [RCX],AL
140375cac:  75 0E                    JNZ 0x140375cbc
140375cae:  48 FF C1                 INC RCX
140375cb1:  49 8D 04 08              LEA RAX,[R8 + RCX*0x1]
140375cb5:  49 3B C1                 CMP RAX,R9
140375cb8:  7D 1D                    JGE 0x140375cd7
140375cba:  EB EA                    JMP 0x140375ca6
140375cbc:  41 FF C2                 INC R10D
140375cbf:  44 3B D3                 CMP R10D,EBX
140375cc2:  72 9C                    JC 0x140375c60
140375cc4:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140375cc9:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140375ccd:  E8 1E 9A 0D 00           CALL 0x14044f6f0
140375cd2:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
140375cd7:  4D 85 DB                 TEST R11,R11
140375cda:  74 08                    JZ 0x140375ce4
140375cdc:  49 8B CB                 MOV RCX,R11
140375cdf:  E8 FC 82 4D 00           CALL 0x14084dfe0
140375ce4:  41 B4 01                 MOV R12B,0x1
140375ce7:  F3 0F 10 35 99 C3 61 00  MOVSS XMM6,dword ptr [0x140992088]
140375cef:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140375cf4:  BA E1 05 00 00           MOV EDX,0x5e1
140375cf9:  48 8D 0D 90 18 62 00     LEA RCX,[0x140997590]
140375d00:  FF 15 8A 6B 4F 00        CALL qword ptr [0x14086c890]
140375d06:  0F 28 DF                 MOVAPS XMM3,XMM7
140375d09:  F3 0F 5C DE              SUBSS XMM3,XMM6
140375d0d:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140375d12:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140375d17:  48 8D 05 52 75 54 00     LEA RAX,[0x1408bd270]
140375d1e:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140375d23:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140375d2b:  66 41 0F 7E D9           MOVD R9D,XMM3
140375d30:  0F 28 D6                 MOVAPS XMM2,XMM6
140375d33:  66 41 0F 7E D0           MOVD R8D,XMM2
140375d38:  49 8B D7                 MOV RDX,R15
140375d3b:  48 8D 0D 6E 12 62 00     LEA RCX,[0x140996fb0]
140375d42:  FF 15 38 6B 4F 00        CALL qword ptr [0x14086c880]
140375d48:  BA 77 06 00 00           MOV EDX,0x677
140375d4d:  48 8D 0D 3C 18 62 00     LEA RCX,[0x140997590]
140375d54:  FF 15 36 6B 4F 00        CALL qword ptr [0x14086c890]
140375d5a:  F3 0F 10 05 26 C3 61 00  MOVSS XMM0,dword ptr [0x140992088]
140375d62:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140375d67:  0F 28 DF                 MOVAPS XMM3,XMM7
140375d6a:  F3 0F 5C D8              SUBSS XMM3,XMM0
140375d6e:  66 0F 6E 05 16 F5 61 00  MOVD XMM0,dword ptr [0x14099528c]
140375d76:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140375d79:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140375d7e:  F3 0F 10 55 B0           MOVSS XMM2,dword ptr [RBP + -0x50]
140375d83:  F3 0F 5C D0              SUBSS XMM2,XMM0
140375d87:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140375d8c:  48 8D 05 CD 74 54 00     LEA RAX,[0x1408bd260]
140375d93:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140375d98:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140375da0:  66 41 0F 7E D9           MOVD R9D,XMM3
140375da5:  66 41 0F 7E D0           MOVD R8D,XMM2
140375daa:  49 8B D7                 MOV RDX,R15
140375dad:  48 8D 0D FC 11 62 00     LEA RCX,[0x140996fb0]
140375db4:  FF 15 CE 6A 4F 00        CALL qword ptr [0x14086c888]
140375dba:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
140375dc1:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140375dc4:  48 8D 0D 05 4F 67 00     LEA RCX,[0x1409eacd0]
140375dcb:  FF 15 1F 69 4F 00        CALL qword ptr [0x14086c6f0]
140375dd1:  4C 8B C0                 MOV R8,RAX
140375dd4:  33 D2                    XOR EDX,EDX
140375dd6:  48 8B CB                 MOV RCX,RBX
140375dd9:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140375ddc:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375de3:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375de7:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375deb:  41 0F 28 C9              MOVAPS XMM1,XMM9
140375def:  48 8D 8D E0 0A 00 00     LEA RCX,[RBP + 0xae0]
140375df6:  FF 15 24 62 4F 00        CALL qword ptr [0x14086c020]
140375dfc:  48 8B D0                 MOV RDX,RAX
140375dff:  48 8D 0D 5A 82 64 00     LEA RCX,[0x1409be060]
140375e06:  FF 15 8C 6A 4F 00        CALL qword ptr [0x14086c898]
140375e0c:  F3 0F 10 0D 74 C2 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375e14:  F3 0F 59 0D F0 4B 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140375e1c:  0F 28 D1                 MOVAPS XMM2,XMM1
140375e1f:  48 8D 0D 3A 82 64 00     LEA RCX,[0x1409be060]
140375e26:  FF 15 7C 6A 4F 00        CALL qword ptr [0x14086c8a8]
140375e2c:  F3 0F 10 15 54 C2 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375e34:  F3 0F 10 35 EC 49 59 00  MOVSS XMM6,dword ptr [0x14090a828]
140375e3c:  F3 0F 59 D6              MULSS XMM2,XMM6
140375e40:  F3 0F 58 D7              ADDSS XMM2,XMM7
140375e44:  41 0F 28 D8              MOVAPS XMM3,XMM8
140375e48:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
140375e4e:  48 8D 8D 70 03 00 00     LEA RCX,[RBP + 0x370]
140375e55:  FF 15 AD 61 4F 00        CALL qword ptr [0x14086c008]
140375e5b:  48 8B D0                 MOV RDX,RAX
140375e5e:  48 8D 0D FB 81 64 00     LEA RCX,[0x1409be060]
140375e65:  FF 15 35 6A 4F 00        CALL qword ptr [0x14086c8a0]
140375e6b:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140375e70:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140375e77:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375e7e:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375e82:  41 0F 28 D0              MOVAPS XMM2,XMM8
140375e86:  41 0F 28 C8              MOVAPS XMM1,XMM8
140375e8a:  48 8D 0D CF 81 64 00     LEA RCX,[0x1409be060]
140375e91:  FF 15 C9 69 4F 00        CALL qword ptr [0x14086c860]
140375e97:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
140375e9e:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140375ea1:  48 8D 0D 28 4E 67 00     LEA RCX,[0x1409eacd0]
140375ea8:  FF 15 42 68 4F 00        CALL qword ptr [0x14086c6f0]
140375eae:  4C 8B C0                 MOV R8,RAX
140375eb1:  33 D2                    XOR EDX,EDX
140375eb3:  48 8B CB                 MOV RCX,RBX
140375eb6:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140375eb9:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375ec0:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375ec4:  41 0F 28 D1              MOVAPS XMM2,XMM9
140375ec8:  41 0F 28 C9              MOVAPS XMM1,XMM9
140375ecc:  48 8D 8D 00 07 00 00     LEA RCX,[RBP + 0x700]
140375ed3:  FF 15 47 61 4F 00        CALL qword ptr [0x14086c020]
140375ed9:  48 8B D0                 MOV RDX,RAX
140375edc:  48 8D 0D 7D 81 64 00     LEA RCX,[0x1409be060]
140375ee3:  FF 15 AF 69 4F 00        CALL qword ptr [0x14086c898]
140375ee9:  F3 0F 10 0D 97 C1 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375ef1:  F3 0F 59 0D 13 4B 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140375ef9:  0F 28 D1                 MOVAPS XMM2,XMM1
140375efc:  48 8D 0D 5D 81 64 00     LEA RCX,[0x1409be060]
140375f03:  FF 15 9F 69 4F 00        CALL qword ptr [0x14086c8a8]
140375f09:  F3 0F 10 15 77 C1 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375f11:  F3 0F 59 D6              MULSS XMM2,XMM6
140375f15:  F3 0F 58 D7              ADDSS XMM2,XMM7
140375f19:  41 0F 28 D8              MOVAPS XMM3,XMM8
140375f1d:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140375f23:  48 8D 8D 7C 03 00 00     LEA RCX,[RBP + 0x37c]
140375f2a:  FF 15 D8 60 4F 00        CALL qword ptr [0x14086c008]
140375f30:  48 8B D0                 MOV RDX,RAX
140375f33:  48 8D 0D 26 81 64 00     LEA RCX,[0x1409be060]
140375f3a:  FF 15 60 69 4F 00        CALL qword ptr [0x14086c8a0]
140375f40:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140375f45:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140375f4c:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140375f53:  41 0F 28 D9              MOVAPS XMM3,XMM9
140375f57:  41 0F 28 D0              MOVAPS XMM2,XMM8
140375f5b:  41 0F 28 C8              MOVAPS XMM1,XMM8
140375f5f:  48 8D 0D FA 80 64 00     LEA RCX,[0x1409be060]
140375f66:  FF 15 F4 68 4F 00        CALL qword ptr [0x14086c860]
140375f6c:  F3 41 0F 58 FC           ADDSS XMM7,XMM12
140375f71:  8B 05 F9 F2 61 00        MOV EAX,dword ptr [0x140995270]
140375f77:  83 F8 02                 CMP EAX,0x2
140375f7a:  75 09                    JNZ 0x140375f85
140375f7c:  48 8D 05 3D 72 54 00     LEA RAX,[0x1408bd1c0]
140375f83:  EB 13                    JMP 0x140375f98
140375f85:  83 F8 01                 CMP EAX,0x1
140375f88:  48 8D 05 31 71 54 00     LEA RAX,[0x1408bd0c0]
140375f8f:  74 07                    JZ 0x140375f98
140375f91:  48 8D 05 08 73 54 00     LEA RAX,[0x1408bd2a0]
140375f98:  F3 0F 10 15 E8 C0 61 00  MOVSS XMM2,dword ptr [0x140992088]
140375fa0:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140375fa5:  0F 28 DA                 MOVAPS XMM3,XMM2
140375fa8:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140375fad:  F3 0F 58 DF              ADDSS XMM3,XMM7
140375fb1:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
140375fb6:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140375fbe:  66 41 0F 7E D9           MOVD R9D,XMM3
140375fc3:  66 41 0F 7E D0           MOVD R8D,XMM2
140375fc8:  49 8B D7                 MOV RDX,R15
140375fcb:  48 8D 0D DE 0F 62 00     LEA RCX,[0x140996fb0]
140375fd2:  FF 15 A8 68 4F 00        CALL qword ptr [0x14086c880]
140375fd8:  F3 41 0F 58 FC           ADDSS XMM7,XMM12
140375fdd:  F3 0F 10 0D A3 C0 61 00  MOVSS XMM1,dword ptr [0x140992088]
140375fe5:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140375fea:  0F 28 C7                 MOVAPS XMM0,XMM7
140375fed:  F3 0F 5C C1              SUBSS XMM0,XMM1
140375ff1:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140375ff5:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140375ff9:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
140375ffe:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140376003:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140376007:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037600b:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140376012:  75 45                    JNZ 0x140376059
140376014:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140376019:  48 C1 E9 20              SHR RCX,0x20
14037601d:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140376022:  48 C1 E8 20              SHR RAX,0x20
140376026:  03 C8                    ADD ECX,EAX
140376028:  66 0F 6E D1              MOVD XMM2,ECX
14037602c:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037602f:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140376034:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
140376038:  41 03 C5                 ADD EAX,R13D
14037603b:  66 0F 6E C8              MOVD XMM1,EAX
14037603f:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140376042:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140376047:  41 B9 06 00 00 00        MOV R9D,0x6
14037604d:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140376054:  E8 97 01 0D 00           CALL 0x1404461f0
140376059:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037605e:  0F 84 47 01 00 00        JZ 0x1403761ab
140376064:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
14037606a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037606d:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140376072:  0F 2F C8                 COMISS XMM1,XMM0
140376075:  0F 86 30 01 00 00        JBE 0x1403761ab
14037607b:  66 41 0F 6E C5           MOVD XMM0,R13D
140376080:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376083:  0F 2F C1                 COMISS XMM0,XMM1
140376086:  0F 86 1F 01 00 00        JBE 0x1403761ab
14037608c:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140376091:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140376096:  66 0F 6E C3              MOVD XMM0,EBX
14037609a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037609d:  0F 2F C8                 COMISS XMM1,XMM0
1403760a0:  0F 86 05 01 00 00        JBE 0x1403761ab
1403760a6:  66 0F 6E C7              MOVD XMM0,EDI
1403760aa:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403760ad:  0F 2F C1                 COMISS XMM0,XMM1
1403760b0:  0F 86 F5 00 00 00        JBE 0x1403761ab
1403760b6:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
1403760bd:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403760c0:  48 8D 0D 09 4C 67 00     LEA RCX,[0x1409eacd0]
1403760c7:  FF 15 23 66 4F 00        CALL qword ptr [0x14086c6f0]
1403760cd:  4C 8B C0                 MOV R8,RAX
1403760d0:  33 D2                    XOR EDX,EDX
1403760d2:  48 8B CB                 MOV RCX,RBX
1403760d5:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403760d8:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
1403760df:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403760e3:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403760e7:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403760eb:  48 8D 8D 10 07 00 00     LEA RCX,[RBP + 0x710]
1403760f2:  FF 15 28 5F 4F 00        CALL qword ptr [0x14086c020]
1403760f8:  48 8B D0                 MOV RDX,RAX
1403760fb:  48 8D 0D 5E 7F 64 00     LEA RCX,[0x1409be060]
140376102:  FF 15 90 67 4F 00        CALL qword ptr [0x14086c898]
140376108:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037610c:  48 8D 0D 4D 7F 64 00     LEA RCX,[0x1409be060]
140376113:  FF 15 3F 68 4F 00        CALL qword ptr [0x14086c958]
140376119:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037611e:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140376124:  F3 0F 10 15 5C BF 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037612c:  F3 0F 59 D6              MULSS XMM2,XMM6
140376130:  F3 0F 10 05 40 3D 59 00  MOVSS XMM0,dword ptr [0x140909e78]
140376138:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
14037613e:  F3 0F 10 0D BE 3B 59 00  MOVSS XMM1,dword ptr [0x140909d04]
140376146:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
14037614c:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140376152:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
140376158:  0F 28 DA                 MOVAPS XMM3,XMM2
14037615b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140376160:  48 8D 0D F9 7E 64 00     LEA RCX,[0x1409be060]
140376167:  FF 15 EB 66 4F 00        CALL qword ptr [0x14086c858]
14037616d:  41 0F 28 C8              MOVAPS XMM1,XMM8
140376171:  48 8D 0D E8 7E 64 00     LEA RCX,[0x1409be060]
140376178:  FF 15 DA 67 4F 00        CALL qword ptr [0x14086c958]
14037617e:  48 8D 0D 0B EA 6D 00     LEA RCX,[0x140a54b90]
140376185:  FF 15 2D 67 4F 00        CALL qword ptr [0x14086c8b8]
14037618b:  84 C0                    TEST AL,AL
14037618d:  74 1C                    JZ 0x1403761ab
14037618f:  41 B0 01                 MOV R8B,0x1
140376192:  48 8D 15 F7 70 54 00     LEA RDX,[0x1408bd290]
140376199:  48 8B CE                 MOV RCX,RSI
14037619c:  E8 2F 99 08 00           CALL 0x1403ffad0
1403761a1:  C7 86 D4 C7 00 00 02 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x2
1403761ab:  F3 0F 10 35 D5 BE 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403761b3:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403761b8:  BA DF 05 00 00           MOV EDX,0x5df
1403761bd:  48 8D 0D CC 13 62 00     LEA RCX,[0x140997590]
1403761c4:  FF 15 C6 66 4F 00        CALL qword ptr [0x14086c890]
1403761ca:  0F 28 DF                 MOVAPS XMM3,XMM7
1403761cd:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403761d1:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
1403761d6:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403761db:  48 8D 05 9E 70 54 00     LEA RAX,[0x1408bd280]
1403761e2:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403761e7:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403761ef:  66 41 0F 7E D9           MOVD R9D,XMM3
1403761f4:  0F 28 D6                 MOVAPS XMM2,XMM6
1403761f7:  66 41 0F 7E D0           MOVD R8D,XMM2
1403761fc:  49 8B D7                 MOV RDX,R15
1403761ff:  48 8D 0D AA 0D 62 00     LEA RCX,[0x140996fb0]
140376206:  FF 15 74 66 4F 00        CALL qword ptr [0x14086c880]
14037620c:  F3 0F 10 95 B4 00 00 00  MOVSS XMM2,dword ptr [RBP + 0xb4]
140376214:  0F 5A D2                 CVTPS2PD XMM2,XMM2
140376217:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037621c:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140376221:  45 33 C9                 XOR R9D,R9D
140376224:  48 8D 95 00 02 00 00     LEA RDX,[RBP + 0x200]
14037622b:  48 8B CE                 MOV RCX,RSI
14037622e:  E8 2D 0A FC FF           CALL 0x140336c60
140376233:  90                       NOP
140376234:  48 8B 08                 MOV RCX,qword ptr [RAX]
140376237:  F3 0F 10 15 49 BE 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037623f:  0F 28 C2                 MOVAPS XMM0,XMM2
140376242:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140376247:  0F 28 DF                 MOVAPS XMM3,XMM7
14037624a:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037624e:  F3 0F 10 35 12 47 59 00  MOVSS XMM6,dword ptr [0x14090a968]
140376256:  F3 0F 59 D6              MULSS XMM2,XMM6
14037625a:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
14037625f:  66 0F 6E 05 25 F0 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376267:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037626a:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037626e:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140376273:  48 8D 05 3E 6F 54 00     LEA RAX,[0x1408bd1b8]
14037627a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037627f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376287:  66 41 0F 7E D9           MOVD R9D,XMM3
14037628c:  66 41 0F 7E D0           MOVD R8D,XMM2
140376291:  49 8B D7                 MOV RDX,R15
140376294:  48 8D 0D 15 0D 62 00     LEA RCX,[0x140996fb0]
14037629b:  FF 15 D7 65 4F 00        CALL qword ptr [0x14086c878]
1403762a1:  90                       NOP
1403762a2:  48 8B 8D 00 02 00 00     MOV RCX,qword ptr [RBP + 0x200]
1403762a9:  48 85 C9                 TEST RCX,RCX
1403762ac:  74 13                    JZ 0x1403762c1
1403762ae:  E8 2D 7D 4D 00           CALL 0x14084dfe0
1403762b3:  4C 89 B5 00 02 00 00     MOV qword ptr [RBP + 0x200],R14
1403762ba:  44 89 B5 08 02 00 00     MOV dword ptr [RBP + 0x208],R14D
1403762c1:  F3 0F 10 85 B8 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xb8]
1403762c9:  F3 44 0F 10 25 4E 64 59 00 MOVSS XMM12,dword ptr [0x14090c720]
1403762d2:  41 0F 57 C4              XORPS XMM0,XMM12
1403762d6:  0F 57 C9                 XORPS XMM1,XMM1
1403762d9:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
1403762dd:  41 B1 01                 MOV R9B,0x1
1403762e0:  45 33 C0                 XOR R8D,R8D
1403762e3:  48 8B CE                 MOV RCX,RSI
1403762e6:  E8 35 01 FC FF           CALL 0x140336420
1403762eb:  F3 0F 10 15 95 BD 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403762f3:  0F 28 C2                 MOVAPS XMM0,XMM2
1403762f6:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403762fb:  F3 0F 5C F8              SUBSS XMM7,XMM0
1403762ff:  F3 0F 59 D6              MULSS XMM2,XMM6
140376303:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
140376309:  66 0F 6E 05 7B EF 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376311:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376314:  F3 0F 5C D0              SUBSS XMM2,XMM0
140376318:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037631d:  48 8D 05 5C 71 54 00     LEA RAX,[0x1408bd480]
140376324:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376329:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376331:  0F 28 DF                 MOVAPS XMM3,XMM7
140376334:  66 41 0F 7E D9           MOVD R9D,XMM3
140376339:  66 41 0F 7E D0           MOVD R8D,XMM2
14037633e:  49 8B D7                 MOV RDX,R15
140376341:  48 8D 0D 68 0C 62 00     LEA RCX,[0x140996fb0]
140376348:  FF 15 2A 65 4F 00        CALL qword ptr [0x14086c878]
14037634e:  F3 0F 10 0D 32 BD 61 00  MOVSS XMM1,dword ptr [0x140992088]
140376356:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037635b:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037635f:  F3 0F 5C C1              SUBSS XMM0,XMM1
140376363:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140376367:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037636b:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
140376371:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
140376376:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037637a:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037637e:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140376382:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140376389:  75 45                    JNZ 0x1403763d0
14037638b:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140376390:  48 C1 E9 20              SHR RCX,0x20
140376394:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140376399:  48 C1 E8 20              SHR RAX,0x20
14037639d:  03 C8                    ADD ECX,EAX
14037639f:  66 0F 6E D1              MOVD XMM2,ECX
1403763a3:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
1403763a6:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1403763ab:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
1403763af:  41 03 C5                 ADD EAX,R13D
1403763b2:  66 0F 6E C8              MOVD XMM1,EAX
1403763b6:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1403763b9:  F3 41 0F 59 CB           MULSS XMM1,XMM11
1403763be:  41 B9 06 00 00 00        MOV R9D,0x6
1403763c4:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
1403763cb:  E8 20 FE 0C 00           CALL 0x1404461f0
1403763d0:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
1403763d5:  0F 84 4B 01 00 00        JZ 0x140376526
1403763db:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
1403763e1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403763e4:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
1403763e9:  0F 2F C8                 COMISS XMM1,XMM0
1403763ec:  0F 86 34 01 00 00        JBE 0x140376526
1403763f2:  66 41 0F 6E C5           MOVD XMM0,R13D
1403763f7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403763fa:  0F 2F C1                 COMISS XMM0,XMM1
1403763fd:  0F 86 23 01 00 00        JBE 0x140376526
140376403:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140376408:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037640d:  66 0F 6E C3              MOVD XMM0,EBX
140376411:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376414:  0F 2F C8                 COMISS XMM1,XMM0
140376417:  0F 86 09 01 00 00        JBE 0x140376526
14037641d:  66 0F 6E C7              MOVD XMM0,EDI
140376421:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376424:  0F 2F C1                 COMISS XMM0,XMM1
140376427:  0F 86 F9 00 00 00        JBE 0x140376526
14037642d:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140376434:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140376437:  48 8D 0D 92 48 67 00     LEA RCX,[0x1409eacd0]
14037643e:  FF 15 AC 62 4F 00        CALL qword ptr [0x14086c6f0]
140376444:  4C 8B C0                 MOV R8,RAX
140376447:  33 D2                    XOR EDX,EDX
140376449:  48 8B CB                 MOV RCX,RBX
14037644c:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037644f:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140376456:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037645a:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037645e:  41 0F 28 C9              MOVAPS XMM1,XMM9
140376462:  48 8D 8D 20 07 00 00     LEA RCX,[RBP + 0x720]
140376469:  FF 15 B1 5B 4F 00        CALL qword ptr [0x14086c020]
14037646f:  48 8B D0                 MOV RDX,RAX
140376472:  48 8D 0D E7 7B 64 00     LEA RCX,[0x1409be060]
140376479:  FF 15 19 64 4F 00        CALL qword ptr [0x14086c898]
14037647f:  41 0F 28 C9              MOVAPS XMM1,XMM9
140376483:  48 8D 0D D6 7B 64 00     LEA RCX,[0x1409be060]
14037648a:  FF 15 C8 64 4F 00        CALL qword ptr [0x14086c958]
140376490:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140376495:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037649b:  F3 0F 10 15 E5 BB 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403764a3:  F3 0F 59 15 7D 43 59 00  MULSS XMM2,dword ptr [0x14090a828]
1403764ab:  F3 0F 10 05 C5 39 59 00  MOVSS XMM0,dword ptr [0x140909e78]
1403764b3:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
1403764b9:  F3 0F 10 0D 43 38 59 00  MOVSS XMM1,dword ptr [0x140909d04]
1403764c1:  F3 0F 11 4C 24 30        MOVSS dword ptr [RSP + 0x30],XMM1
1403764c7:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
1403764cd:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
1403764d3:  0F 28 DA                 MOVAPS XMM3,XMM2
1403764d6:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403764db:  48 8D 0D 7E 7B 64 00     LEA RCX,[0x1409be060]
1403764e2:  FF 15 70 63 4F 00        CALL qword ptr [0x14086c858]
1403764e8:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403764ec:  48 8D 0D 6D 7B 64 00     LEA RCX,[0x1409be060]
1403764f3:  FF 15 5F 64 4F 00        CALL qword ptr [0x14086c958]
1403764f9:  48 8D 0D 90 E6 6D 00     LEA RCX,[0x140a54b90]
140376500:  FF 15 B2 63 4F 00        CALL qword ptr [0x14086c8b8]
140376506:  84 C0                    TEST AL,AL
140376508:  74 1C                    JZ 0x140376526
14037650a:  41 B0 01                 MOV R8B,0x1
14037650d:  48 8D 15 5C 6F 54 00     LEA RDX,[0x1408bd470]
140376514:  48 8B CE                 MOV RCX,RSI
140376517:  E8 B4 95 08 00           CALL 0x1403ffad0
14037651c:  C7 86 D4 C7 00 00 03 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x3
140376526:  F3 0F 10 35 5A BB 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037652e:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140376533:  BA E0 05 00 00           MOV EDX,0x5e0
140376538:  48 8D 0D 51 10 62 00     LEA RCX,[0x140997590]
14037653f:  FF 15 4B 63 4F 00        CALL qword ptr [0x14086c890]
140376545:  41 0F 28 DA              MOVAPS XMM3,XMM10
140376549:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037654d:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140376552:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140376557:  48 8D 05 02 6F 54 00     LEA RAX,[0x1408bd460]
14037655e:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376563:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037656b:  66 41 0F 7E D9           MOVD R9D,XMM3
140376570:  0F 28 D6                 MOVAPS XMM2,XMM6
140376573:  66 41 0F 7E D0           MOVD R8D,XMM2
140376578:  49 8B D7                 MOV RDX,R15
14037657b:  48 8D 0D 2E 0A 62 00     LEA RCX,[0x140996fb0]
140376582:  FF 15 F8 62 4F 00        CALL qword ptr [0x14086c880]
140376588:  F3 0F 10 95 C8 00 00 00  MOVSS XMM2,dword ptr [RBP + 0xc8]
140376590:  0F 5A D2                 CVTPS2PD XMM2,XMM2
140376593:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140376598:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037659d:  45 33 C9                 XOR R9D,R9D
1403765a0:  48 8D 95 20 02 00 00     LEA RDX,[RBP + 0x220]
1403765a7:  48 8B CE                 MOV RCX,RSI
1403765aa:  E8 B1 06 FC FF           CALL 0x140336c60
1403765af:  90                       NOP
1403765b0:  48 8B 08                 MOV RCX,qword ptr [RAX]
1403765b3:  F3 0F 10 15 CD BA 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403765bb:  0F 28 C2                 MOVAPS XMM0,XMM2
1403765be:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403765c3:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403765c7:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403765cb:  F3 0F 10 35 95 43 59 00  MOVSS XMM6,dword ptr [0x14090a968]
1403765d3:  F3 0F 59 D6              MULSS XMM2,XMM6
1403765d7:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
1403765dc:  66 0F 6E 05 A8 EC 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403765e4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403765e7:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403765eb:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
1403765f0:  48 8D 05 61 6E 54 00     LEA RAX,[0x1408bd458]
1403765f7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403765fc:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376604:  66 41 0F 7E D9           MOVD R9D,XMM3
140376609:  66 41 0F 7E D0           MOVD R8D,XMM2
14037660e:  49 8B D7                 MOV RDX,R15
140376611:  48 8D 0D 98 09 62 00     LEA RCX,[0x140996fb0]
140376618:  FF 15 5A 62 4F 00        CALL qword ptr [0x14086c878]
14037661e:  90                       NOP
14037661f:  48 8B 8D 20 02 00 00     MOV RCX,qword ptr [RBP + 0x220]
140376626:  48 85 C9                 TEST RCX,RCX
140376629:  74 13                    JZ 0x14037663e
14037662b:  E8 B0 79 4D 00           CALL 0x14084dfe0
140376630:  4C 89 B5 20 02 00 00     MOV qword ptr [RBP + 0x220],R14
140376637:  44 89 B5 28 02 00 00     MOV dword ptr [RBP + 0x228],R14D
14037663e:  F3 0F 10 85 CC 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xcc]
140376646:  41 0F 57 C4              XORPS XMM0,XMM12
14037664a:  0F 57 C9                 XORPS XMM1,XMM1
14037664d:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140376651:  41 B1 01                 MOV R9B,0x1
140376654:  45 33 C0                 XOR R8D,R8D
140376657:  48 8B CE                 MOV RCX,RSI
14037665a:  E8 C1 FD FB FF           CALL 0x140336420
14037665f:  F3 0F 10 15 21 BA 61 00  MOVSS XMM2,dword ptr [0x140992088]
140376667:  0F 28 C2                 MOVAPS XMM0,XMM2
14037666a:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037666f:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
140376674:  F3 0F 59 D6              MULSS XMM2,XMM6
140376678:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037667e:  66 0F 6E 05 06 EC 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376686:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376689:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037668d:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140376692:  48 8D 05 0F 6E 54 00     LEA RAX,[0x1408bd4a8]
140376699:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037669e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403766a6:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403766aa:  66 41 0F 7E D9           MOVD R9D,XMM3
1403766af:  66 41 0F 7E D0           MOVD R8D,XMM2
1403766b4:  49 8B D7                 MOV RDX,R15
1403766b7:  48 8D 0D F2 08 62 00     LEA RCX,[0x140996fb0]
1403766be:  FF 15 B4 61 4F 00        CALL qword ptr [0x14086c878]
1403766c4:  F3 0F 10 35 BC B9 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403766cc:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403766d1:  BA E2 05 00 00           MOV EDX,0x5e2
1403766d6:  48 8D 0D B3 0E 62 00     LEA RCX,[0x140997590]
1403766dd:  FF 15 AD 61 4F 00        CALL qword ptr [0x14086c890]
1403766e3:  0F 28 DF                 MOVAPS XMM3,XMM7
1403766e6:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403766ea:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
1403766ef:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403766f4:  48 8D 05 9D 6D 54 00     LEA RAX,[0x1408bd498]
1403766fb:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376700:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376708:  66 41 0F 7E D9           MOVD R9D,XMM3
14037670d:  0F 28 D6                 MOVAPS XMM2,XMM6
140376710:  66 41 0F 7E D0           MOVD R8D,XMM2
140376715:  49 8B D7                 MOV RDX,R15
140376718:  48 8D 0D 91 08 62 00     LEA RCX,[0x140996fb0]
14037671f:  FF 15 5B 61 4F 00        CALL qword ptr [0x14086c880]
140376725:  F3 0F 10 85 BC 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xbc]
14037672d:  41 0F 57 C4              XORPS XMM0,XMM12
140376731:  0F 57 C9                 XORPS XMM1,XMM1
140376734:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140376738:  45 33 C9                 XOR R9D,R9D
14037673b:  45 33 C0                 XOR R8D,R8D
14037673e:  48 8B CE                 MOV RCX,RSI
140376741:  E8 DA FC FB FF           CALL 0x140336420
140376746:  F3 0F 10 15 3A B9 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037674e:  0F 28 C2                 MOVAPS XMM0,XMM2
140376751:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140376756:  0F 28 DF                 MOVAPS XMM3,XMM7
140376759:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037675d:  F3 0F 59 15 03 42 59 00  MULSS XMM2,dword ptr [0x14090a968]
140376765:  F3 44 0F 10 54 24 74     MOVSS XMM10,dword ptr [RSP + 0x74]
14037676c:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140376771:  66 0F 6E 05 13 EB 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376779:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037677c:  F3 0F 5C D0              SUBSS XMM2,XMM0
140376780:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140376785:  48 8D 05 04 6D 54 00     LEA RAX,[0x1408bd490]
14037678c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376791:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376799:  66 41 0F 7E D9           MOVD R9D,XMM3
14037679e:  66 41 0F 7E D0           MOVD R8D,XMM2
1403767a3:  49 8B D7                 MOV RDX,R15
1403767a6:  48 8D 0D 03 08 62 00     LEA RCX,[0x140996fb0]
1403767ad:  FF 15 C5 60 4F 00        CALL qword ptr [0x14086c878]
1403767b3:  F3 0F 10 85 D0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xd0]
1403767bb:  41 0F 57 C4              XORPS XMM0,XMM12
1403767bf:  0F 57 C9                 XORPS XMM1,XMM1
1403767c2:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
1403767c6:  45 33 C9                 XOR R9D,R9D
1403767c9:  45 33 C0                 XOR R8D,R8D
1403767cc:  48 8B CE                 MOV RCX,RSI
1403767cf:  E8 4C FC FB FF           CALL 0x140336420
1403767d4:  F3 0F 10 15 AC B8 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403767dc:  0F 28 C2                 MOVAPS XMM0,XMM2
1403767df:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403767e4:  0F 28 DF                 MOVAPS XMM3,XMM7
1403767e7:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403767eb:  F3 0F 59 15 75 41 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403767f3:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
1403767f9:  66 0F 6E 05 8B EA 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376801:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376804:  F3 0F 5C D0              SUBSS XMM2,XMM0
140376808:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037680d:  48 8D 05 74 6C 54 00     LEA RAX,[0x1408bd488]
140376814:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376819:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376821:  66 41 0F 7E D9           MOVD R9D,XMM3
140376826:  66 41 0F 7E D0           MOVD R8D,XMM2
14037682b:  49 8B D7                 MOV RDX,R15
14037682e:  48 8D 0D 7B 07 62 00     LEA RCX,[0x140996fb0]
140376835:  FF 15 3D 60 4F 00        CALL qword ptr [0x14086c878]
14037683b:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
140376841:  48 8D 15 B0 6B 54 00     LEA RDX,[0x1408bd3f8]
140376848:  48 8D 0D 71 77 64 00     LEA RCX,[0x1409bdfc0]
14037684f:  FF 15 6B 58 4F 00        CALL qword ptr [0x14086c0c0]
140376855:  F3 0F 10 35 2B B8 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037685d:  48 8D 0D 2C 0D 62 00     LEA RCX,[0x140997590]
140376864:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140376869:  85 C0                    TEST EAX,EAX
14037686b:  0F 84 DA 00 00 00        JZ 0x14037694b
140376871:  BA DF 3A 00 00           MOV EDX,0x3adf
140376876:  FF 15 14 60 4F 00        CALL qword ptr [0x14086c890]
14037687c:  0F 28 DF                 MOVAPS XMM3,XMM7
14037687f:  F3 0F 5C DE              SUBSS XMM3,XMM6
140376883:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140376888:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037688d:  48 8D 05 54 6B 54 00     LEA RAX,[0x1408bd3e8]
140376894:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376899:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403768a1:  66 41 0F 7E D9           MOVD R9D,XMM3
1403768a6:  0F 28 D6                 MOVAPS XMM2,XMM6
1403768a9:  66 41 0F 7E D0           MOVD R8D,XMM2
1403768ae:  49 8B D7                 MOV RDX,R15
1403768b1:  48 8D 0D F8 06 62 00     LEA RCX,[0x140996fb0]
1403768b8:  FF 15 C2 5F 4F 00        CALL qword ptr [0x14086c880]
1403768be:  F3 44 0F 59 2D FD 37 59 00 MULSS XMM13,dword ptr [0x14090a0c4]
1403768c7:  0F 57 C9                 XORPS XMM1,XMM1
1403768ca:  F3 41 0F 5A CD           CVTSS2SD XMM1,XMM13
1403768cf:  F3 0F 10 15 B1 B7 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403768d7:  0F 28 C2                 MOVAPS XMM0,XMM2
1403768da:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403768df:  0F 28 DF                 MOVAPS XMM3,XMM7
1403768e2:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403768e6:  F3 0F 59 15 7A 40 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403768ee:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
1403768f3:  66 0F 6E 05 91 E9 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403768fb:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403768fe:  F3 0F 5C D0              SUBSS XMM2,XMM0
140376902:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
140376908:  48 8D 05 C9 6A 54 00     LEA RAX,[0x1408bd3d8]
14037690f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376914:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037691c:  66 41 0F 7E D9           MOVD R9D,XMM3
140376921:  66 41 0F 7E D0           MOVD R8D,XMM2
140376926:  49 8B D7                 MOV RDX,R15
140376929:  48 8D 0D 80 06 62 00     LEA RCX,[0x140996fb0]
140376930:  FF 15 42 5F 4F 00        CALL qword ptr [0x14086c878]
140376936:  F3 44 0F 59 35 85 37 59 00 MULSS XMM14,dword ptr [0x14090a0c4]
14037693f:  48 8D 05 82 6A 54 00     LEA RAX,[0x1408bd3c8]
140376946:  E9 C3 00 00 00           JMP 0x140376a0e
14037694b:  BA E3 05 00 00           MOV EDX,0x5e3
140376950:  FF 15 3A 5F 4F 00        CALL qword ptr [0x14086c890]
140376956:  0F 28 DF                 MOVAPS XMM3,XMM7
140376959:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037695d:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140376962:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140376967:  48 8D 05 DA 6A 54 00     LEA RAX,[0x1408bd448]
14037696e:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376973:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037697b:  66 41 0F 7E D9           MOVD R9D,XMM3
140376980:  0F 28 D6                 MOVAPS XMM2,XMM6
140376983:  66 41 0F 7E D0           MOVD R8D,XMM2
140376988:  49 8B D7                 MOV RDX,R15
14037698b:  48 8D 0D 1E 06 62 00     LEA RCX,[0x140996fb0]
140376992:  FF 15 E8 5E 4F 00        CALL qword ptr [0x14086c880]
140376998:  0F 57 C9                 XORPS XMM1,XMM1
14037699b:  F3 41 0F 5A CD           CVTSS2SD XMM1,XMM13
1403769a0:  F3 0F 10 15 E0 B6 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403769a8:  0F 28 C2                 MOVAPS XMM0,XMM2
1403769ab:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403769b0:  0F 28 DF                 MOVAPS XMM3,XMM7
1403769b3:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403769b7:  F3 0F 59 15 A9 3F 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403769bf:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
1403769c4:  66 0F 6E 05 C0 E8 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403769cc:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403769cf:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403769d3:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
1403769d9:  48 8D 05 58 6A 54 00     LEA RAX,[0x1408bd438]
1403769e0:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403769e5:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403769ed:  66 41 0F 7E D9           MOVD R9D,XMM3
1403769f2:  66 41 0F 7E D0           MOVD R8D,XMM2
1403769f7:  49 8B D7                 MOV RDX,R15
1403769fa:  48 8D 0D AF 05 62 00     LEA RCX,[0x140996fb0]
140376a01:  FF 15 71 5E 4F 00        CALL qword ptr [0x14086c878]
140376a07:  48 8D 05 1A 6A 54 00     LEA RAX,[0x1408bd428]
140376a0e:  F3 0F 10 15 72 B6 61 00  MOVSS XMM2,dword ptr [0x140992088]
140376a16:  0F 28 C2                 MOVAPS XMM0,XMM2
140376a19:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140376a1e:  0F 28 DF                 MOVAPS XMM3,XMM7
140376a21:  0F 57 C9                 XORPS XMM1,XMM1
140376a24:  F3 0F 5C D8              SUBSS XMM3,XMM0
140376a28:  F3 41 0F 5A CE           CVTSS2SD XMM1,XMM14
140376a2d:  66 0F 6E 05 57 E8 61 00  MOVD XMM0,dword ptr [0x14099528c]
140376a35:  F3 44 0F 10 6C 24 70     MOVSS XMM13,dword ptr [RSP + 0x70]
140376a3c:  F3 0F 59 15 24 3F 59 00  MULSS XMM2,dword ptr [0x14090a968]
140376a44:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
140376a4a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376a4d:  F3 41 0F 58 D5           ADDSS XMM2,XMM13
140376a52:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376a57:  F3 0F 5C D0              SUBSS XMM2,XMM0
140376a5b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140376a63:  66 41 0F 7E D9           MOVD R9D,XMM3
140376a68:  66 41 0F 7E D0           MOVD R8D,XMM2
140376a6d:  49 8B D7                 MOV RDX,R15
140376a70:  48 8D 0D 39 05 62 00     LEA RCX,[0x140996fb0]
140376a77:  FF 15 FB 5D 4F 00        CALL qword ptr [0x14086c878]
140376a7d:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
140376a83:  F3 0F 10 35 FD B5 61 00  MOVSS XMM6,dword ptr [0x140992088]
140376a8b:  0F 28 C6                 MOVAPS XMM0,XMM6
140376a8e:  F3 0F 59 05 92 3D 59 00  MULSS XMM0,dword ptr [0x14090a828]
140376a96:  F3 0F 58 F8              ADDSS XMM7,XMM0
140376a9a:  0F 28 D6                 MOVAPS XMM2,XMM6
140376a9d:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140376aa2:  0F 28 C7                 MOVAPS XMM0,XMM7
140376aa5:  F3 0F 5C C2              SUBSS XMM0,XMM2
140376aa9:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140376aad:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140376ab1:  F3 0F 10 4D BC           MOVSS XMM1,dword ptr [RBP + -0x44]
140376ab6:  F3 0F 58 CF              ADDSS XMM1,XMM7
140376aba:  F3 0F 58 CA              ADDSS XMM1,XMM2
140376abe:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140376ac2:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140376ac6:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140376acd:  75 4D                    JNZ 0x140376b1c
140376acf:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140376ad4:  48 C1 E9 20              SHR RCX,0x20
140376ad8:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140376add:  48 C1 E8 20              SHR RAX,0x20
140376ae1:  03 C8                    ADD ECX,EAX
140376ae3:  66 0F 6E D1              MOVD XMM2,ECX
140376ae7:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140376aea:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140376aef:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
140376af3:  41 03 C5                 ADD EAX,R13D
140376af6:  66 0F 6E C8              MOVD XMM1,EAX
140376afa:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140376afd:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140376b02:  41 B9 06 00 00 00        MOV R9D,0x6
140376b08:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140376b0f:  E8 DC F6 0C 00           CALL 0x1404461f0
140376b14:  F3 0F 10 35 6C B5 61 00  MOVSS XMM6,dword ptr [0x140992088]
140376b1c:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140376b21:  0F 84 A9 02 00 00        JZ 0x140376dd0
140376b27:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140376b2d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376b30:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140376b35:  0F 2F C8                 COMISS XMM1,XMM0
140376b38:  0F 86 92 02 00 00        JBE 0x140376dd0
140376b3e:  66 41 0F 6E C5           MOVD XMM0,R13D
140376b43:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376b46:  0F 2F C1                 COMISS XMM0,XMM1
140376b49:  0F 86 81 02 00 00        JBE 0x140376dd0
140376b4f:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140376b54:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140376b59:  66 0F 6E C3              MOVD XMM0,EBX
140376b5d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376b60:  0F 2F C8                 COMISS XMM1,XMM0
140376b63:  0F 86 67 02 00 00        JBE 0x140376dd0
140376b69:  66 0F 6E C7              MOVD XMM0,EDI
140376b6d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140376b70:  0F 2F C1                 COMISS XMM0,XMM1
140376b73:  0F 86 57 02 00 00        JBE 0x140376dd0
140376b79:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140376b80:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140376b83:  48 8D 0D 46 41 67 00     LEA RCX,[0x1409eacd0]
140376b8a:  FF 15 60 5B 4F 00        CALL qword ptr [0x14086c6f0]
140376b90:  4C 8B C0                 MOV R8,RAX
140376b93:  33 D2                    XOR EDX,EDX
140376b95:  48 8B CB                 MOV RCX,RBX
140376b98:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140376b9b:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140376ba2:  41 0F 28 D9              MOVAPS XMM3,XMM9
140376ba6:  41 0F 28 D1              MOVAPS XMM2,XMM9
140376baa:  41 0F 28 C9              MOVAPS XMM1,XMM9
140376bae:  48 8D 8D 30 07 00 00     LEA RCX,[RBP + 0x730]
140376bb5:  FF 15 65 54 4F 00        CALL qword ptr [0x14086c020]
140376bbb:  48 8B D0                 MOV RDX,RAX
140376bbe:  48 8D 0D 9B 74 64 00     LEA RCX,[0x1409be060]
140376bc5:  FF 15 CD 5C 4F 00        CALL qword ptr [0x14086c898]
140376bcb:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140376bd0:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140376bd6:  F3 0F 10 15 AA B4 61 00  MOVSS XMM2,dword ptr [0x140992088]
140376bde:  F3 0F 59 15 42 3D 59 00  MULSS XMM2,dword ptr [0x14090a928]
140376be6:  F3 44 0F 10 35 89 32 59 00 MOVSS XMM14,dword ptr [0x140909e78]
140376bef:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
140376bf6:  F3 0F 10 05 06 31 59 00  MOVSS XMM0,dword ptr [0x140909d04]
140376bfe:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
140376c04:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
140376c0b:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
140376c11:  0F 28 DA                 MOVAPS XMM3,XMM2
140376c14:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140376c19:  48 8D 0D 40 74 64 00     LEA RCX,[0x1409be060]
140376c20:  FF 15 32 5C 4F 00        CALL qword ptr [0x14086c858]
140376c26:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140376c2b:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140376c30:  4C 8D 05 D9 67 54 00     LEA R8,[0x1408bd410]
140376c37:  83 CA FF                 OR EDX,0xffffffff
140376c3a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140376c3f:  E8 DC 40 EE FF           CALL 0x14025ad20
140376c44:  90                       NOP
140376c45:  45 8B D6                 MOV R10D,R14D
140376c48:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140376c4c:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140376c50:  4C 2B DF                 SUB R11,RDI
140376c53:  49 C1 FB 04              SAR R11,0x4
140376c57:  45 85 DB                 TEST R11D,R11D
140376c5a:  74 77                    JZ 0x140376cd3
140376c5c:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140376c61:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140376c66:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140376c70:  41 8B C2                 MOV EAX,R10D
140376c73:  48 03 C0                 ADD RAX,RAX
140376c76:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140376c7a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140376c7e:  85 D2                    TEST EDX,EDX
140376c80:  79 17                    JNS 0x140376c99
140376c82:  48 85 C9                 TEST RCX,RCX
140376c85:  74 44                    JZ 0x140376ccb
140376c87:  48 83 CA FF              OR RDX,-0x1
140376c8b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140376c90:  48 FF C2                 INC RDX
140376c93:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140376c97:  75 F7                    JNZ 0x140376c90
140376c99:  48 85 C9                 TEST RCX,RCX
140376c9c:  74 2D                    JZ 0x140376ccb
140376c9e:  41 3B D4                 CMP EDX,R12D
140376ca1:  75 28                    JNZ 0x140376ccb
140376ca3:  4C 63 C2                 MOVSXD R8,EDX
140376ca6:  85 D2                    TEST EDX,EDX
140376ca8:  7E 3C                    JLE 0x140376ce6
140376caa:  48 8B D1                 MOV RDX,RCX
140376cad:  48 F7 DA                 NEG RDX
140376cb0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140376cb4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140376cb7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140376cbb:  75 0E                    JNZ 0x140376ccb
140376cbd:  48 FF C1                 INC RCX
140376cc0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140376cc4:  49 3B C0                 CMP RAX,R8
140376cc7:  7D 1D                    JGE 0x140376ce6
140376cc9:  EB E9                    JMP 0x140376cb4
140376ccb:  41 FF C2                 INC R10D
140376cce:  45 3B D3                 CMP R10D,R11D
140376cd1:  72 9D                    JC 0x140376c70
140376cd3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140376cd8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140376cdc:  E8 0F 8A 0D 00           CALL 0x14044f6f0
140376ce1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140376ce6:  48 85 DB                 TEST RBX,RBX
140376ce9:  74 08                    JZ 0x140376cf3
140376ceb:  48 8B CB                 MOV RCX,RBX
140376cee:  E8 ED 72 4D 00           CALL 0x14084dfe0
140376cf3:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140376cf8:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140376cfd:  4C 8D 05 04 68 54 00     LEA R8,[0x1408bd508]
140376d04:  83 CA FF                 OR EDX,0xffffffff
140376d07:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140376d0c:  E8 0F 40 EE FF           CALL 0x14025ad20
140376d11:  90                       NOP
140376d12:  45 8B D6                 MOV R10D,R14D
140376d15:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140376d19:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140376d1d:  4C 2B DF                 SUB R11,RDI
140376d20:  49 C1 FB 04              SAR R11,0x4
140376d24:  45 85 DB                 TEST R11D,R11D
140376d27:  74 7A                    JZ 0x140376da3
140376d29:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140376d2e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140376d33:  0F 1F 40 00              NOP dword ptr [RAX]
140376d37:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140376d40:  41 8B C2                 MOV EAX,R10D
140376d43:  48 03 C0                 ADD RAX,RAX
140376d46:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140376d4a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140376d4e:  85 D2                    TEST EDX,EDX
140376d50:  79 17                    JNS 0x140376d69
140376d52:  48 85 C9                 TEST RCX,RCX
140376d55:  74 44                    JZ 0x140376d9b
140376d57:  48 83 CA FF              OR RDX,-0x1
140376d5b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140376d60:  48 FF C2                 INC RDX
140376d63:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140376d67:  75 F7                    JNZ 0x140376d60
140376d69:  48 85 C9                 TEST RCX,RCX
140376d6c:  74 2D                    JZ 0x140376d9b
140376d6e:  41 3B D4                 CMP EDX,R12D
140376d71:  75 28                    JNZ 0x140376d9b
140376d73:  4C 63 C2                 MOVSXD R8,EDX
140376d76:  85 D2                    TEST EDX,EDX
140376d78:  7E 3C                    JLE 0x140376db6
140376d7a:  48 8B D1                 MOV RDX,RCX
140376d7d:  48 F7 DA                 NEG RDX
140376d80:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140376d84:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140376d87:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140376d8b:  75 0E                    JNZ 0x140376d9b
140376d8d:  48 FF C1                 INC RCX
140376d90:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140376d94:  49 3B C0                 CMP RAX,R8
140376d97:  7D 1D                    JGE 0x140376db6
140376d99:  EB E9                    JMP 0x140376d84
140376d9b:  41 FF C2                 INC R10D
140376d9e:  45 3B D3                 CMP R10D,R11D
140376da1:  72 9D                    JC 0x140376d40
140376da3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140376da8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140376dac:  E8 3F 89 0D 00           CALL 0x14044f6f0
140376db1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140376db6:  48 85 DB                 TEST RBX,RBX
140376db9:  74 08                    JZ 0x140376dc3
140376dbb:  48 8B CB                 MOV RCX,RBX
140376dbe:  E8 1D 72 4D 00           CALL 0x14084dfe0
140376dc3:  41 B4 01                 MOV R12B,0x1
140376dc6:  F3 0F 10 35 BA B2 61 00  MOVSS XMM6,dword ptr [0x140992088]
140376dce:  EB 09                    JMP 0x140376dd9
140376dd0:  F3 44 0F 10 35 9F 30 59 00 MOVSS XMM14,dword ptr [0x140909e78]
140376dd9:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140376dde:  BA 91 06 00 00           MOV EDX,0x691
140376de3:  48 8D 0D A6 07 62 00     LEA RCX,[0x140997590]
140376dea:  FF 15 A0 5A 4F 00        CALL qword ptr [0x14086c890]
140376df0:  0F 28 DF                 MOVAPS XMM3,XMM7
140376df3:  F3 0F 5C DE              SUBSS XMM3,XMM6
140376df7:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140376dfc:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140376e01:  48 8D 05 28 6B 54 00     LEA RAX,[0x1408bd930]
140376e08:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140376e0d:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140376e15:  66 41 0F 7E D9           MOVD R9D,XMM3
140376e1a:  0F 28 D6                 MOVAPS XMM2,XMM6
140376e1d:  66 41 0F 7E D0           MOVD R8D,XMM2
140376e22:  49 8B D7                 MOV RDX,R15
140376e25:  48 8D 0D 84 01 62 00     LEA RCX,[0x140996fb0]
140376e2c:  FF 15 4E 5A 4F 00        CALL qword ptr [0x14086c880]
140376e32:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
140376e39:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140376e3c:  48 8D 0D 8D 3E 67 00     LEA RCX,[0x1409eacd0]
140376e43:  FF 15 A7 58 4F 00        CALL qword ptr [0x14086c6f0]
140376e49:  4C 8B C0                 MOV R8,RAX
140376e4c:  33 D2                    XOR EDX,EDX
140376e4e:  48 8B CB                 MOV RCX,RBX
140376e51:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140376e54:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140376e5b:  41 0F 28 D9              MOVAPS XMM3,XMM9
140376e5f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140376e63:  41 0F 28 C9              MOVAPS XMM1,XMM9
140376e67:  48 8D 8D 40 07 00 00     LEA RCX,[RBP + 0x740]
140376e6e:  FF 15 AC 51 4F 00        CALL qword ptr [0x14086c020]
140376e74:  48 8B D0                 MOV RDX,RAX
140376e77:  48 8D 0D E2 71 64 00     LEA RCX,[0x1409be060]
140376e7e:  FF 15 14 5A 4F 00        CALL qword ptr [0x14086c898]
140376e84:  F3 0F 10 0D FC B1 61 00  MOVSS XMM1,dword ptr [0x140992088]
140376e8c:  F3 0F 59 0D 78 3B 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140376e94:  0F 28 D1                 MOVAPS XMM2,XMM1
140376e97:  48 8D 0D C2 71 64 00     LEA RCX,[0x1409be060]
140376e9e:  FF 15 04 5A 4F 00        CALL qword ptr [0x14086c8a8]
140376ea4:  F3 0F 10 15 DC B1 61 00  MOVSS XMM2,dword ptr [0x140992088]
140376eac:  F3 0F 59 15 74 39 59 00  MULSS XMM2,dword ptr [0x14090a828]
140376eb4:  F3 0F 58 D7              ADDSS XMM2,XMM7
140376eb8:  41 0F 28 D8              MOVAPS XMM3,XMM8
140376ebc:  41 0F 28 CA              MOVAPS XMM1,XMM10
140376ec0:  48 8D 8D 88 03 00 00     LEA RCX,[RBP + 0x388]
140376ec7:  FF 15 3B 51 4F 00        CALL qword ptr [0x14086c008]
140376ecd:  48 8B D0                 MOV RDX,RAX
140376ed0:  48 8D 0D 89 71 64 00     LEA RCX,[0x1409be060]
140376ed7:  FF 15 C3 59 4F 00        CALL qword ptr [0x14086c8a0]
140376edd:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140376ee2:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140376ee9:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140376ef0:  41 0F 28 D9              MOVAPS XMM3,XMM9
140376ef4:  41 0F 28 D0              MOVAPS XMM2,XMM8
140376ef8:  41 0F 28 C8              MOVAPS XMM1,XMM8
140376efc:  48 8D 0D 5D 71 64 00     LEA RCX,[0x1409be060]
140376f03:  FF 15 57 59 4F 00        CALL qword ptr [0x14086c860]
140376f09:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
140376f10:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140376f13:  48 8D 0D B6 3D 67 00     LEA RCX,[0x1409eacd0]
140376f1a:  FF 15 D0 57 4F 00        CALL qword ptr [0x14086c6f0]
140376f20:  4C 8B C0                 MOV R8,RAX
140376f23:  33 D2                    XOR EDX,EDX
140376f25:  48 8B CB                 MOV RCX,RBX
140376f28:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140376f2b:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140376f32:  41 0F 28 D9              MOVAPS XMM3,XMM9
140376f36:  41 0F 28 D1              MOVAPS XMM2,XMM9
140376f3a:  41 0F 28 C9              MOVAPS XMM1,XMM9
140376f3e:  48 8D 8D 50 07 00 00     LEA RCX,[RBP + 0x750]
140376f45:  FF 15 D5 50 4F 00        CALL qword ptr [0x14086c020]
140376f4b:  48 8B D0                 MOV RDX,RAX
140376f4e:  48 8D 0D 0B 71 64 00     LEA RCX,[0x1409be060]
140376f55:  FF 15 3D 59 4F 00        CALL qword ptr [0x14086c898]
140376f5b:  F3 0F 10 0D 25 B1 61 00  MOVSS XMM1,dword ptr [0x140992088]
140376f63:  F3 0F 59 0D A1 3A 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140376f6b:  0F 28 D1                 MOVAPS XMM2,XMM1
140376f6e:  48 8D 0D EB 70 64 00     LEA RCX,[0x1409be060]
140376f75:  FF 15 2D 59 4F 00        CALL qword ptr [0x14086c8a8]
140376f7b:  F3 0F 10 15 05 B1 61 00  MOVSS XMM2,dword ptr [0x140992088]
140376f83:  F3 0F 59 15 9D 38 59 00  MULSS XMM2,dword ptr [0x14090a828]
140376f8b:  F3 0F 58 D7              ADDSS XMM2,XMM7
140376f8f:  41 0F 28 D8              MOVAPS XMM3,XMM8
140376f93:  41 0F 28 CD              MOVAPS XMM1,XMM13
140376f97:  48 8D 8D 94 03 00 00     LEA RCX,[RBP + 0x394]
140376f9e:  FF 15 64 50 4F 00        CALL qword ptr [0x14086c008]
140376fa4:  48 8B D0                 MOV RDX,RAX
140376fa7:  48 8D 0D B2 70 64 00     LEA RCX,[0x1409be060]
140376fae:  FF 15 EC 58 4F 00        CALL qword ptr [0x14086c8a0]
140376fb4:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140376fb9:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140376fc0:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140376fc7:  41 0F 28 D9              MOVAPS XMM3,XMM9
140376fcb:  41 0F 28 D0              MOVAPS XMM2,XMM8
140376fcf:  41 0F 28 C8              MOVAPS XMM1,XMM8
140376fd3:  48 8D 0D 86 70 64 00     LEA RCX,[0x1409be060]
140376fda:  FF 15 80 58 4F 00        CALL qword ptr [0x14086c860]
140376fe0:  F3 44 0F 10 6D AC        MOVSS XMM13,dword ptr [RBP + -0x54]
140376fe6:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140376feb:  8B 05 7F E2 61 00        MOV EAX,dword ptr [0x140995270]
140376ff1:  83 F8 02                 CMP EAX,0x2
140376ff4:  75 09                    JNZ 0x140376fff
140376ff6:  48 8D 05 93 68 54 00     LEA RAX,[0x1408bd890]
140376ffd:  EB 13                    JMP 0x140377012
140376fff:  83 F8 01                 CMP EAX,0x1
140377002:  48 8D 05 87 67 54 00     LEA RAX,[0x1408bd790]
140377009:  74 07                    JZ 0x140377012
14037700b:  48 8D 05 4E 69 54 00     LEA RAX,[0x1408bd960]
140377012:  F3 0F 10 15 6E B0 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037701a:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037701f:  0F 28 DA                 MOVAPS XMM3,XMM2
140377022:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377027:  F3 0F 58 DF              ADDSS XMM3,XMM7
14037702b:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
140377030:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140377038:  66 41 0F 7E D9           MOVD R9D,XMM3
14037703d:  66 41 0F 7E D0           MOVD R8D,XMM2
140377042:  49 8B D7                 MOV RDX,R15
140377045:  48 8D 0D 64 FF 61 00     LEA RCX,[0x140996fb0]
14037704c:  FF 15 2E 58 4F 00        CALL qword ptr [0x14086c880]
140377052:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140377057:  F3 0F 10 35 29 B0 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037705f:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140377064:  BA DF 05 00 00           MOV EDX,0x5df
140377069:  48 8D 0D 20 05 62 00     LEA RCX,[0x140997590]
140377070:  FF 15 1A 58 4F 00        CALL qword ptr [0x14086c890]
140377076:  0F 28 DF                 MOVAPS XMM3,XMM7
140377079:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037707d:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140377082:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377087:  48 8D 05 C2 68 54 00     LEA RAX,[0x1408bd950]
14037708e:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377093:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037709b:  66 41 0F 7E D9           MOVD R9D,XMM3
1403770a0:  0F 28 D6                 MOVAPS XMM2,XMM6
1403770a3:  66 41 0F 7E D0           MOVD R8D,XMM2
1403770a8:  49 8B D7                 MOV RDX,R15
1403770ab:  48 8D 0D FE FE 61 00     LEA RCX,[0x140996fb0]
1403770b2:  FF 15 C8 57 4F 00        CALL qword ptr [0x14086c880]
1403770b8:  F3 0F 10 8D E8 00 00 00  MOVSS XMM1,dword ptr [RBP + 0xe8]
1403770c0:  0F 5A C9                 CVTPS2PD XMM1,XMM1
1403770c3:  45 33 C9                 XOR R9D,R9D
1403770c6:  45 33 C0                 XOR R8D,R8D
1403770c9:  48 8B CE                 MOV RCX,RSI
1403770cc:  E8 4F F3 FB FF           CALL 0x140336420
1403770d1:  F3 0F 10 15 AF AF 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403770d9:  0F 28 C2                 MOVAPS XMM0,XMM2
1403770dc:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403770e1:  0F 28 DF                 MOVAPS XMM3,XMM7
1403770e4:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403770e8:  F3 0F 59 15 78 38 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403770f0:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
1403770f5:  66 0F 6E 05 8F E1 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403770fd:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377100:  F3 0F 5C D0              SUBSS XMM2,XMM0
140377104:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377109:  48 8D 05 D8 64 54 00     LEA RAX,[0x1408bd5e8]
140377110:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377115:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037711d:  66 41 0F 7E D9           MOVD R9D,XMM3
140377122:  66 41 0F 7E D0           MOVD R8D,XMM2
140377127:  49 8B D7                 MOV RDX,R15
14037712a:  48 8D 0D 7F FE 61 00     LEA RCX,[0x140996fb0]
140377131:  FF 15 41 57 4F 00        CALL qword ptr [0x14086c878]
140377137:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037713e:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
140377143:  F3 0F 10 35 3D AF 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037714b:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140377150:  BA E0 05 00 00           MOV EDX,0x5e0
140377155:  48 8D 0D 34 04 62 00     LEA RCX,[0x140997590]
14037715c:  FF 15 2E 57 4F 00        CALL qword ptr [0x14086c890]
140377162:  0F 28 DF                 MOVAPS XMM3,XMM7
140377165:  F3 0F 5C DE              SUBSS XMM3,XMM6
140377169:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037716e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377173:  48 8D 05 C6 67 54 00     LEA RAX,[0x1408bd940]
14037717a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037717f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140377187:  66 41 0F 7E D9           MOVD R9D,XMM3
14037718c:  0F 28 D6                 MOVAPS XMM2,XMM6
14037718f:  66 41 0F 7E D0           MOVD R8D,XMM2
140377194:  49 8B D7                 MOV RDX,R15
140377197:  48 8D 0D 12 FE 61 00     LEA RCX,[0x140996fb0]
14037719e:  FF 15 DC 56 4F 00        CALL qword ptr [0x14086c880]
1403771a4:  F3 0F 10 8D E4 00 00 00  MOVSS XMM1,dword ptr [RBP + 0xe4]
1403771ac:  0F 5A C9                 CVTPS2PD XMM1,XMM1
1403771af:  45 33 C9                 XOR R9D,R9D
1403771b2:  45 33 C0                 XOR R8D,R8D
1403771b5:  48 8B CE                 MOV RCX,RSI
1403771b8:  E8 63 F2 FB FF           CALL 0x140336420
1403771bd:  F3 0F 10 15 C3 AE 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403771c5:  0F 28 C2                 MOVAPS XMM0,XMM2
1403771c8:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403771cd:  0F 28 DF                 MOVAPS XMM3,XMM7
1403771d0:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403771d4:  F3 0F 59 15 8C 37 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403771dc:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
1403771e2:  66 0F 6E 05 A2 E0 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403771ea:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403771ed:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403771f1:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403771f6:  48 8D 05 F3 62 54 00     LEA RAX,[0x1408bd4f0]
1403771fd:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377202:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037720a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037720f:  66 41 0F 7E D0           MOVD R8D,XMM2
140377214:  49 8B D7                 MOV RDX,R15
140377217:  48 8D 0D 92 FD 61 00     LEA RCX,[0x140996fb0]
14037721e:  FF 15 54 56 4F 00        CALL qword ptr [0x14086c878]
140377224:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
140377229:  F3 0F 10 35 57 AE 61 00  MOVSS XMM6,dword ptr [0x140992088]
140377231:  0F 28 C6                 MOVAPS XMM0,XMM6
140377234:  F3 0F 59 05 EC 35 59 00  MULSS XMM0,dword ptr [0x14090a828]
14037723c:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140377241:  0F 28 D6                 MOVAPS XMM2,XMM6
140377244:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140377249:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037724d:  F3 0F 5C C2              SUBSS XMM0,XMM2
140377251:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140377255:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140377259:  F3 0F 10 4D BC           MOVSS XMM1,dword ptr [RBP + -0x44]
14037725e:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140377263:  F3 0F 58 CA              ADDSS XMM1,XMM2
140377267:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037726b:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037726f:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140377276:  75 4D                    JNZ 0x1403772c5
140377278:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037727d:  48 C1 E9 20              SHR RCX,0x20
140377281:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140377286:  48 C1 E8 20              SHR RAX,0x20
14037728a:  03 C8                    ADD ECX,EAX
14037728c:  66 0F 6E D1              MOVD XMM2,ECX
140377290:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140377293:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140377298:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
14037729c:  41 03 C5                 ADD EAX,R13D
14037729f:  66 0F 6E C8              MOVD XMM1,EAX
1403772a3:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1403772a6:  F3 41 0F 59 CB           MULSS XMM1,XMM11
1403772ab:  41 B9 06 00 00 00        MOV R9D,0x6
1403772b1:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
1403772b8:  E8 33 EF 0C 00           CALL 0x1404461f0
1403772bd:  F3 0F 10 35 C3 AD 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403772c5:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
1403772ca:  0F 84 9F 02 00 00        JZ 0x14037756f
1403772d0:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
1403772d6:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403772d9:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
1403772de:  0F 2F C8                 COMISS XMM1,XMM0
1403772e1:  0F 86 88 02 00 00        JBE 0x14037756f
1403772e7:  66 41 0F 6E C5           MOVD XMM0,R13D
1403772ec:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403772ef:  0F 2F C1                 COMISS XMM0,XMM1
1403772f2:  0F 86 77 02 00 00        JBE 0x14037756f
1403772f8:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
1403772fd:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140377302:  66 0F 6E C3              MOVD XMM0,EBX
140377306:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377309:  0F 2F C8                 COMISS XMM1,XMM0
14037730c:  0F 86 5D 02 00 00        JBE 0x14037756f
140377312:  66 0F 6E C7              MOVD XMM0,EDI
140377316:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377319:  0F 2F C1                 COMISS XMM0,XMM1
14037731c:  0F 86 4D 02 00 00        JBE 0x14037756f
140377322:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140377329:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037732c:  48 8D 0D 9D 39 67 00     LEA RCX,[0x1409eacd0]
140377333:  FF 15 B7 53 4F 00        CALL qword ptr [0x14086c6f0]
140377339:  4C 8B C0                 MOV R8,RAX
14037733c:  33 D2                    XOR EDX,EDX
14037733e:  48 8B CB                 MOV RCX,RBX
140377341:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140377344:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037734b:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037734f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140377353:  41 0F 28 C9              MOVAPS XMM1,XMM9
140377357:  48 8D 8D 60 07 00 00     LEA RCX,[RBP + 0x760]
14037735e:  FF 15 BC 4C 4F 00        CALL qword ptr [0x14086c020]
140377364:  48 8B D0                 MOV RDX,RAX
140377367:  48 8D 0D F2 6C 64 00     LEA RCX,[0x1409be060]
14037736e:  FF 15 24 55 4F 00        CALL qword ptr [0x14086c898]
140377374:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140377379:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037737f:  F3 0F 10 15 01 AD 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377387:  F3 0F 59 15 99 35 59 00  MULSS XMM2,dword ptr [0x14090a928]
14037738f:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
140377396:  F3 0F 10 05 66 29 59 00  MOVSS XMM0,dword ptr [0x140909d04]
14037739e:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
1403773a4:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
1403773ab:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
1403773b1:  0F 28 DA                 MOVAPS XMM3,XMM2
1403773b4:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403773b9:  48 8D 0D A0 6C 64 00     LEA RCX,[0x1409be060]
1403773c0:  FF 15 92 54 4F 00        CALL qword ptr [0x14086c858]
1403773c6:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
1403773cb:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
1403773d0:  4C 8D 05 01 61 54 00     LEA R8,[0x1408bd4d8]
1403773d7:  83 CA FF                 OR EDX,0xffffffff
1403773da:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
1403773df:  E8 3C 39 EE FF           CALL 0x14025ad20
1403773e4:  90                       NOP
1403773e5:  45 8B D6                 MOV R10D,R14D
1403773e8:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
1403773ec:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
1403773f0:  4C 2B DF                 SUB R11,RDI
1403773f3:  49 C1 FB 04              SAR R11,0x4
1403773f7:  45 85 DB                 TEST R11D,R11D
1403773fa:  74 77                    JZ 0x140377473
1403773fc:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140377401:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377406:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140377410:  41 8B C2                 MOV EAX,R10D
140377413:  48 03 C0                 ADD RAX,RAX
140377416:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037741a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037741e:  85 D2                    TEST EDX,EDX
140377420:  79 17                    JNS 0x140377439
140377422:  48 85 C9                 TEST RCX,RCX
140377425:  74 44                    JZ 0x14037746b
140377427:  48 83 CA FF              OR RDX,-0x1
14037742b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140377430:  48 FF C2                 INC RDX
140377433:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140377437:  75 F7                    JNZ 0x140377430
140377439:  48 85 C9                 TEST RCX,RCX
14037743c:  74 2D                    JZ 0x14037746b
14037743e:  41 3B D4                 CMP EDX,R12D
140377441:  75 28                    JNZ 0x14037746b
140377443:  4C 63 C2                 MOVSXD R8,EDX
140377446:  85 D2                    TEST EDX,EDX
140377448:  7E 3C                    JLE 0x140377486
14037744a:  48 8B D1                 MOV RDX,RCX
14037744d:  48 F7 DA                 NEG RDX
140377450:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140377454:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140377457:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037745b:  75 0E                    JNZ 0x14037746b
14037745d:  48 FF C1                 INC RCX
140377460:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140377464:  49 3B C0                 CMP RAX,R8
140377467:  7D 1D                    JGE 0x140377486
140377469:  EB E9                    JMP 0x140377454
14037746b:  41 FF C2                 INC R10D
14037746e:  45 3B D3                 CMP R10D,R11D
140377471:  72 9D                    JC 0x140377410
140377473:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140377478:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037747c:  E8 6F 82 0D 00           CALL 0x14044f6f0
140377481:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377486:  48 85 DB                 TEST RBX,RBX
140377489:  74 08                    JZ 0x140377493
14037748b:  48 8B CB                 MOV RCX,RBX
14037748e:  E8 4D 6B 4D 00           CALL 0x14084dfe0
140377493:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140377498:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037749d:  4C 8D 05 1C 60 54 00     LEA R8,[0x1408bd4c0]
1403774a4:  83 CA FF                 OR EDX,0xffffffff
1403774a7:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
1403774ac:  E8 6F 38 EE FF           CALL 0x14025ad20
1403774b1:  90                       NOP
1403774b2:  45 8B D6                 MOV R10D,R14D
1403774b5:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
1403774b9:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
1403774bd:  48 2B DF                 SUB RBX,RDI
1403774c0:  48 C1 FB 04              SAR RBX,0x4
1403774c4:  85 DB                    TEST EBX,EBX
1403774c6:  74 7C                    JZ 0x140377544
1403774c8:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
1403774cd:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
1403774d2:  0F 1F 40 00              NOP dword ptr [RAX]
1403774d6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
1403774e0:  41 8B C2                 MOV EAX,R10D
1403774e3:  48 03 C0                 ADD RAX,RAX
1403774e6:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
1403774ea:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
1403774ee:  85 C9                    TEST ECX,ECX
1403774f0:  79 17                    JNS 0x140377509
1403774f2:  48 85 D2                 TEST RDX,RDX
1403774f5:  74 45                    JZ 0x14037753c
1403774f7:  48 83 C9 FF              OR RCX,-0x1
1403774fb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140377500:  48 FF C1                 INC RCX
140377503:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
140377507:  75 F7                    JNZ 0x140377500
140377509:  48 85 D2                 TEST RDX,RDX
14037750c:  74 2E                    JZ 0x14037753c
14037750e:  41 3B CC                 CMP ECX,R12D
140377511:  75 29                    JNZ 0x14037753c
140377513:  4C 63 C9                 MOVSXD R9,ECX
140377516:  85 C9                    TEST ECX,ECX
140377518:  7E 3D                    JLE 0x140377557
14037751a:  49 8B CB                 MOV RCX,R11
14037751d:  49 2B D3                 SUB RDX,R11
140377520:  4D 8B C3                 MOV R8,R11
140377523:  49 F7 D8                 NEG R8
140377526:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
14037752a:  38 01                    CMP byte ptr [RCX],AL
14037752c:  75 0E                    JNZ 0x14037753c
14037752e:  48 FF C1                 INC RCX
140377531:  49 8D 04 08              LEA RAX,[R8 + RCX*0x1]
140377535:  49 3B C1                 CMP RAX,R9
140377538:  7D 1D                    JGE 0x140377557
14037753a:  EB EA                    JMP 0x140377526
14037753c:  41 FF C2                 INC R10D
14037753f:  44 3B D3                 CMP R10D,EBX
140377542:  72 9C                    JC 0x1403774e0
140377544:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140377549:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037754d:  E8 9E 81 0D 00           CALL 0x14044f6f0
140377552:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
140377557:  4D 85 DB                 TEST R11,R11
14037755a:  74 08                    JZ 0x140377564
14037755c:  49 8B CB                 MOV RCX,R11
14037755f:  E8 7C 6A 4D 00           CALL 0x14084dfe0
140377564:  41 B4 01                 MOV R12B,0x1
140377567:  F3 0F 10 35 19 AB 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037756f:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140377574:  BA 7F 06 00 00           MOV EDX,0x67f
140377579:  48 8D 0D 10 00 62 00     LEA RCX,[0x140997590]
140377580:  FF 15 0A 53 4F 00        CALL qword ptr [0x14086c890]
140377586:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037758a:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037758e:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140377593:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377598:  48 8D 05 11 5F 54 00     LEA RAX,[0x1408bd4b0]
14037759f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403775a4:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
1403775ac:  66 41 0F 7E D9           MOVD R9D,XMM3
1403775b1:  0F 28 D6                 MOVAPS XMM2,XMM6
1403775b4:  66 41 0F 7E D0           MOVD R8D,XMM2
1403775b9:  49 8B D7                 MOV RDX,R15
1403775bc:  48 8D 0D ED F9 61 00     LEA RCX,[0x140996fb0]
1403775c3:  FF 15 B7 52 4F 00        CALL qword ptr [0x14086c880]
1403775c9:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
1403775d0:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403775d3:  48 8D 0D F6 36 67 00     LEA RCX,[0x1409eacd0]
1403775da:  FF 15 10 51 4F 00        CALL qword ptr [0x14086c6f0]
1403775e0:  4C 8B C0                 MOV R8,RAX
1403775e3:  33 D2                    XOR EDX,EDX
1403775e5:  48 8B CB                 MOV RCX,RBX
1403775e8:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403775eb:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403775f2:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403775f6:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403775fa:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403775fe:  48 8D 8D 70 07 00 00     LEA RCX,[RBP + 0x770]
140377605:  FF 15 15 4A 4F 00        CALL qword ptr [0x14086c020]
14037760b:  48 8B D0                 MOV RDX,RAX
14037760e:  48 8D 0D 4B 6A 64 00     LEA RCX,[0x1409be060]
140377615:  FF 15 7D 52 4F 00        CALL qword ptr [0x14086c898]
14037761b:  F3 0F 10 0D 65 AA 61 00  MOVSS XMM1,dword ptr [0x140992088]
140377623:  F3 0F 59 0D E1 33 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
14037762b:  0F 28 D1                 MOVAPS XMM2,XMM1
14037762e:  48 8D 0D 2B 6A 64 00     LEA RCX,[0x1409be060]
140377635:  FF 15 6D 52 4F 00        CALL qword ptr [0x14086c8a8]
14037763b:  F3 0F 10 15 45 AA 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377643:  F3 0F 59 15 DD 31 59 00  MULSS XMM2,dword ptr [0x14090a828]
14037764b:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140377650:  41 0F 28 D8              MOVAPS XMM3,XMM8
140377654:  F3 0F 10 7C 24 74        MOVSS XMM7,dword ptr [RSP + 0x74]
14037765a:  0F 28 CF                 MOVAPS XMM1,XMM7
14037765d:  48 8D 8D A0 03 00 00     LEA RCX,[RBP + 0x3a0]
140377664:  FF 15 9E 49 4F 00        CALL qword ptr [0x14086c008]
14037766a:  48 8B D0                 MOV RDX,RAX
14037766d:  48 8D 0D EC 69 64 00     LEA RCX,[0x1409be060]
140377674:  FF 15 26 52 4F 00        CALL qword ptr [0x14086c8a0]
14037767a:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037767f:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140377686:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037768d:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377691:  41 0F 28 D0              MOVAPS XMM2,XMM8
140377695:  41 0F 28 C8              MOVAPS XMM1,XMM8
140377699:  48 8D 0D C0 69 64 00     LEA RCX,[0x1409be060]
1403776a0:  FF 15 BA 51 4F 00        CALL qword ptr [0x14086c860]
1403776a6:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
1403776ad:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403776b0:  48 8D 0D 19 36 67 00     LEA RCX,[0x1409eacd0]
1403776b7:  FF 15 33 50 4F 00        CALL qword ptr [0x14086c6f0]
1403776bd:  4C 8B C0                 MOV R8,RAX
1403776c0:  33 D2                    XOR EDX,EDX
1403776c2:  48 8B CB                 MOV RCX,RBX
1403776c5:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403776c8:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403776cf:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403776d3:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403776d7:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403776db:  48 8D 8D 80 07 00 00     LEA RCX,[RBP + 0x780]
1403776e2:  FF 15 38 49 4F 00        CALL qword ptr [0x14086c020]
1403776e8:  48 8B D0                 MOV RDX,RAX
1403776eb:  48 8D 0D 6E 69 64 00     LEA RCX,[0x1409be060]
1403776f2:  FF 15 A0 51 4F 00        CALL qword ptr [0x14086c898]
1403776f8:  F3 0F 10 0D 88 A9 61 00  MOVSS XMM1,dword ptr [0x140992088]
140377700:  F3 0F 59 0D 04 33 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140377708:  0F 28 D1                 MOVAPS XMM2,XMM1
14037770b:  48 8D 0D 4E 69 64 00     LEA RCX,[0x1409be060]
140377712:  FF 15 90 51 4F 00        CALL qword ptr [0x14086c8a8]
140377718:  F3 0F 10 15 68 A9 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377720:  F3 0F 59 15 00 31 59 00  MULSS XMM2,dword ptr [0x14090a828]
140377728:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037772d:  41 0F 28 D8              MOVAPS XMM3,XMM8
140377731:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140377737:  48 8D 8D AC 03 00 00     LEA RCX,[RBP + 0x3ac]
14037773e:  FF 15 C4 48 4F 00        CALL qword ptr [0x14086c008]
140377744:  48 8B D0                 MOV RDX,RAX
140377747:  48 8D 0D 12 69 64 00     LEA RCX,[0x1409be060]
14037774e:  FF 15 4C 51 4F 00        CALL qword ptr [0x14086c8a0]
140377754:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140377759:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140377760:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140377767:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037776b:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037776f:  41 0F 28 C8              MOVAPS XMM1,XMM8
140377773:  48 8D 0D E6 68 64 00     LEA RCX,[0x1409be060]
14037777a:  FF 15 E0 50 4F 00        CALL qword ptr [0x14086c860]
140377780:  F3 45 0F 58 D5           ADDSS XMM10,XMM13
140377785:  8B 05 E5 DA 61 00        MOV EAX,dword ptr [0x140995270]
14037778b:  83 F8 02                 CMP EAX,0x2
14037778e:  75 09                    JNZ 0x140377799
140377790:  48 8D 05 59 5F 54 00     LEA RAX,[0x1408bd6f0]
140377797:  EB 13                    JMP 0x1403777ac
140377799:  83 F8 01                 CMP EAX,0x1
14037779c:  48 8D 05 4D 5E 54 00     LEA RAX,[0x1408bd5f0]
1403777a3:  74 07                    JZ 0x1403777ac
1403777a5:  48 8D 05 74 5D 54 00     LEA RAX,[0x1408bd520]
1403777ac:  F3 0F 10 15 D4 A8 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403777b4:  F3 41 0F 59 D0           MULSS XMM2,XMM8
1403777b9:  0F 28 DA                 MOVAPS XMM3,XMM2
1403777bc:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403777c1:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
1403777c6:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
1403777cb:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403777d3:  66 41 0F 7E D9           MOVD R9D,XMM3
1403777d8:  66 41 0F 7E D0           MOVD R8D,XMM2
1403777dd:  49 8B D7                 MOV RDX,R15
1403777e0:  48 8D 0D C9 F7 61 00     LEA RCX,[0x140996fb0]
1403777e7:  FF 15 93 50 4F 00        CALL qword ptr [0x14086c880]
1403777ed:  F3 45 0F 58 D5           ADDSS XMM10,XMM13
1403777f2:  F3 0F 10 35 8E A8 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403777fa:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403777ff:  BA DF 05 00 00           MOV EDX,0x5df
140377804:  48 8D 0D 85 FD 61 00     LEA RCX,[0x140997590]
14037780b:  FF 15 7F 50 4F 00        CALL qword ptr [0x14086c890]
140377811:  41 0F 28 DA              MOVAPS XMM3,XMM10
140377815:  F3 0F 5C DE              SUBSS XMM3,XMM6
140377819:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037781e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377823:  48 8D 05 CE 5C 54 00     LEA RAX,[0x1408bd4f8]
14037782a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037782f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140377837:  66 41 0F 7E D9           MOVD R9D,XMM3
14037783c:  0F 28 D6                 MOVAPS XMM2,XMM6
14037783f:  66 41 0F 7E D0           MOVD R8D,XMM2
140377844:  49 8B D7                 MOV RDX,R15
140377847:  48 8D 0D 62 F7 61 00     LEA RCX,[0x140996fb0]
14037784e:  FF 15 2C 50 4F 00        CALL qword ptr [0x14086c880]
140377854:  F3 0F 10 85 E0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xe0]
14037785c:  41 0F 57 C4              XORPS XMM0,XMM12
140377860:  0F 57 C9                 XORPS XMM1,XMM1
140377863:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140377867:  45 33 C9                 XOR R9D,R9D
14037786a:  45 33 C0                 XOR R8D,R8D
14037786d:  48 8B CE                 MOV RCX,RSI
140377870:  E8 AB EB FB FF           CALL 0x140336420
140377875:  F3 0F 10 15 0B A8 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037787d:  0F 28 C2                 MOVAPS XMM0,XMM2
140377880:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140377885:  41 0F 28 DA              MOVAPS XMM3,XMM10
140377889:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037788d:  F3 0F 59 15 D3 30 59 00  MULSS XMM2,dword ptr [0x14090a968]
140377895:  F3 0F 58 D7              ADDSS XMM2,XMM7
140377899:  66 0F 6E 05 EB D9 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403778a1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403778a4:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403778a8:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403778ad:  48 8D 05 8C 63 54 00     LEA RAX,[0x1408bdc40]
1403778b4:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403778b9:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403778c1:  66 41 0F 7E D9           MOVD R9D,XMM3
1403778c6:  66 41 0F 7E D0           MOVD R8D,XMM2
1403778cb:  49 8B D7                 MOV RDX,R15
1403778ce:  48 8D 0D DB F6 61 00     LEA RCX,[0x140996fb0]
1403778d5:  FF 15 9D 4F 4F 00        CALL qword ptr [0x14086c878]
1403778db:  F3 44 0F 58 54 24 78     ADDSS XMM10,dword ptr [RSP + 0x78]
1403778e2:  F3 0F 10 35 9E A7 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403778ea:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403778ef:  BA E0 05 00 00           MOV EDX,0x5e0
1403778f4:  48 8D 0D 95 FC 61 00     LEA RCX,[0x140997590]
1403778fb:  FF 15 8F 4F 4F 00        CALL qword ptr [0x14086c890]
140377901:  41 0F 28 DA              MOVAPS XMM3,XMM10
140377905:  F3 0F 5C DE              SUBSS XMM3,XMM6
140377909:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037790e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377913:  48 8D 05 16 63 54 00     LEA RAX,[0x1408bdc30]
14037791a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037791f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140377927:  66 41 0F 7E D9           MOVD R9D,XMM3
14037792c:  0F 28 D6                 MOVAPS XMM2,XMM6
14037792f:  66 41 0F 7E D0           MOVD R8D,XMM2
140377934:  49 8B D7                 MOV RDX,R15
140377937:  48 8D 0D 72 F6 61 00     LEA RCX,[0x140996fb0]
14037793e:  FF 15 3C 4F 4F 00        CALL qword ptr [0x14086c880]
140377944:  F3 0F 10 85 DC 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xdc]
14037794c:  41 0F 57 C4              XORPS XMM0,XMM12
140377950:  0F 57 C9                 XORPS XMM1,XMM1
140377953:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140377957:  45 33 C9                 XOR R9D,R9D
14037795a:  45 33 C0                 XOR R8D,R8D
14037795d:  48 8B CE                 MOV RCX,RSI
140377960:  E8 BB EA FB FF           CALL 0x140336420
140377965:  F3 0F 10 15 1B A7 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037796d:  0F 28 C2                 MOVAPS XMM0,XMM2
140377970:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140377975:  41 0F 28 DA              MOVAPS XMM3,XMM10
140377979:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037797d:  F3 0F 59 15 E3 2F 59 00  MULSS XMM2,dword ptr [0x14090a968]
140377985:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037798b:  66 0F 6E 05 F9 D8 61 00  MOVD XMM0,dword ptr [0x14099528c]
140377993:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377996:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037799a:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037799f:  48 8D 05 82 62 54 00     LEA RAX,[0x1408bdc28]
1403779a6:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403779ab:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403779b3:  66 41 0F 7E D9           MOVD R9D,XMM3
1403779b8:  66 41 0F 7E D0           MOVD R8D,XMM2
1403779bd:  49 8B D7                 MOV RDX,R15
1403779c0:  48 8D 0D E9 F5 61 00     LEA RCX,[0x140996fb0]
1403779c7:  FF 15 AB 4E 4F 00        CALL qword ptr [0x14086c878]
1403779cd:  F3 0F 10 54 24 78        MOVSS XMM2,dword ptr [RSP + 0x78]
1403779d3:  0F 28 FA                 MOVAPS XMM7,XMM2
1403779d6:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
1403779db:  F3 0F 10 35 A5 A6 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403779e3:  0F 28 C6                 MOVAPS XMM0,XMM6
1403779e6:  F3 0F 59 05 3A 2E 59 00  MULSS XMM0,dword ptr [0x14090a828]
1403779ee:  F3 0F 58 F8              ADDSS XMM7,XMM0
1403779f2:  0F 28 CE                 MOVAPS XMM1,XMM6
1403779f5:  F3 41 0F 59 C8           MULSS XMM1,XMM8
1403779fa:  0F 28 C7                 MOVAPS XMM0,XMM7
1403779fd:  F3 0F 5C C1              SUBSS XMM0,XMM1
140377a01:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140377a05:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140377a09:  44 0F 28 D2              MOVAPS XMM10,XMM2
140377a0d:  F3 44 0F 10 25 92 2C 59 00 MOVSS XMM12,dword ptr [0x14090a6a8]
140377a16:  F3 45 0F 59 D4           MULSS XMM10,XMM12
140377a1b:  41 0F 28 C2              MOVAPS XMM0,XMM10
140377a1f:  F3 0F 58 C7              ADDSS XMM0,XMM7
140377a23:  F3 0F 58 C1              ADDSS XMM0,XMM1
140377a27:  F3 0F 2C F8              CVTTSS2SI EDI,XMM0
140377a2b:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140377a2f:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140377a36:  75 4D                    JNZ 0x140377a85
140377a38:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140377a3d:  48 C1 E9 20              SHR RCX,0x20
140377a41:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140377a46:  48 C1 E8 20              SHR RAX,0x20
140377a4a:  03 C8                    ADD ECX,EAX
140377a4c:  66 0F 6E D1              MOVD XMM2,ECX
140377a50:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140377a53:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140377a58:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
140377a5c:  41 03 C5                 ADD EAX,R13D
140377a5f:  66 0F 6E C8              MOVD XMM1,EAX
140377a63:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140377a66:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140377a6b:  41 B9 06 00 00 00        MOV R9D,0x6
140377a71:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140377a78:  E8 73 E7 0C 00           CALL 0x1404461f0
140377a7d:  F3 0F 10 35 03 A6 61 00  MOVSS XMM6,dword ptr [0x140992088]
140377a85:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140377a8a:  0F 84 9E 02 00 00        JZ 0x140377d2e
140377a90:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140377a96:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377a99:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140377a9e:  0F 2F C8                 COMISS XMM1,XMM0
140377aa1:  0F 86 87 02 00 00        JBE 0x140377d2e
140377aa7:  66 41 0F 6E C5           MOVD XMM0,R13D
140377aac:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377aaf:  0F 2F C1                 COMISS XMM0,XMM1
140377ab2:  0F 86 76 02 00 00        JBE 0x140377d2e
140377ab8:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140377abd:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140377ac2:  66 0F 6E C3              MOVD XMM0,EBX
140377ac6:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377ac9:  0F 2F C8                 COMISS XMM1,XMM0
140377acc:  0F 86 5C 02 00 00        JBE 0x140377d2e
140377ad2:  66 0F 6E C7              MOVD XMM0,EDI
140377ad6:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377ad9:  0F 2F C1                 COMISS XMM0,XMM1
140377adc:  0F 86 4C 02 00 00        JBE 0x140377d2e
140377ae2:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140377ae9:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140377aec:  48 8D 0D DD 31 67 00     LEA RCX,[0x1409eacd0]
140377af3:  FF 15 F7 4B 4F 00        CALL qword ptr [0x14086c6f0]
140377af9:  4C 8B C0                 MOV R8,RAX
140377afc:  33 D2                    XOR EDX,EDX
140377afe:  48 8B CB                 MOV RCX,RBX
140377b01:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140377b04:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140377b0b:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377b0f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140377b13:  41 0F 28 C9              MOVAPS XMM1,XMM9
140377b17:  48 8D 8D 90 07 00 00     LEA RCX,[RBP + 0x790]
140377b1e:  FF 15 FC 44 4F 00        CALL qword ptr [0x14086c020]
140377b24:  48 8B D0                 MOV RDX,RAX
140377b27:  48 8D 0D 32 65 64 00     LEA RCX,[0x1409be060]
140377b2e:  FF 15 64 4D 4F 00        CALL qword ptr [0x14086c898]
140377b34:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140377b39:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140377b3f:  F3 0F 10 15 41 A5 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377b47:  F3 0F 59 15 D9 2D 59 00  MULSS XMM2,dword ptr [0x14090a928]
140377b4f:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
140377b56:  F3 0F 10 05 A6 21 59 00  MOVSS XMM0,dword ptr [0x140909d04]
140377b5e:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
140377b64:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
140377b6b:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
140377b71:  0F 28 DA                 MOVAPS XMM3,XMM2
140377b74:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140377b79:  48 8D 0D E0 64 64 00     LEA RCX,[0x1409be060]
140377b80:  FF 15 D2 4C 4F 00        CALL qword ptr [0x14086c858]
140377b86:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140377b8b:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140377b90:  4C 8D 05 81 60 54 00     LEA R8,[0x1408bdc18]
140377b97:  83 CA FF                 OR EDX,0xffffffff
140377b9a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140377b9f:  E8 7C 31 EE FF           CALL 0x14025ad20
140377ba4:  90                       NOP
140377ba5:  45 8B D6                 MOV R10D,R14D
140377ba8:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140377bac:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140377bb0:  4C 2B DF                 SUB R11,RDI
140377bb3:  49 C1 FB 04              SAR R11,0x4
140377bb7:  45 85 DB                 TEST R11D,R11D
140377bba:  74 77                    JZ 0x140377c33
140377bbc:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140377bc1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377bc6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140377bd0:  41 8B C2                 MOV EAX,R10D
140377bd3:  48 03 C0                 ADD RAX,RAX
140377bd6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140377bda:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140377bde:  85 D2                    TEST EDX,EDX
140377be0:  79 17                    JNS 0x140377bf9
140377be2:  48 85 C9                 TEST RCX,RCX
140377be5:  74 44                    JZ 0x140377c2b
140377be7:  48 83 CA FF              OR RDX,-0x1
140377beb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140377bf0:  48 FF C2                 INC RDX
140377bf3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140377bf7:  75 F7                    JNZ 0x140377bf0
140377bf9:  48 85 C9                 TEST RCX,RCX
140377bfc:  74 2D                    JZ 0x140377c2b
140377bfe:  41 3B D4                 CMP EDX,R12D
140377c01:  75 28                    JNZ 0x140377c2b
140377c03:  4C 63 C2                 MOVSXD R8,EDX
140377c06:  85 D2                    TEST EDX,EDX
140377c08:  7E 3C                    JLE 0x140377c46
140377c0a:  48 8B D1                 MOV RDX,RCX
140377c0d:  48 F7 DA                 NEG RDX
140377c10:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140377c14:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140377c17:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140377c1b:  75 0E                    JNZ 0x140377c2b
140377c1d:  48 FF C1                 INC RCX
140377c20:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140377c24:  49 3B C0                 CMP RAX,R8
140377c27:  7D 1D                    JGE 0x140377c46
140377c29:  EB E9                    JMP 0x140377c14
140377c2b:  41 FF C2                 INC R10D
140377c2e:  45 3B D3                 CMP R10D,R11D
140377c31:  72 9D                    JC 0x140377bd0
140377c33:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140377c38:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140377c3c:  E8 AF 7A 0D 00           CALL 0x14044f6f0
140377c41:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377c46:  48 85 DB                 TEST RBX,RBX
140377c49:  74 08                    JZ 0x140377c53
140377c4b:  48 8B CB                 MOV RCX,RBX
140377c4e:  E8 8D 63 4D 00           CALL 0x14084dfe0
140377c53:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140377c58:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140377c5d:  4C 8D 05 AC 60 54 00     LEA R8,[0x1408bdd10]
140377c64:  83 CA FF                 OR EDX,0xffffffff
140377c67:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140377c6c:  E8 AF 30 EE FF           CALL 0x14025ad20
140377c71:  90                       NOP
140377c72:  45 8B D6                 MOV R10D,R14D
140377c75:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140377c79:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140377c7d:  4C 2B DF                 SUB R11,RDI
140377c80:  49 C1 FB 04              SAR R11,0x4
140377c84:  45 85 DB                 TEST R11D,R11D
140377c87:  74 7A                    JZ 0x140377d03
140377c89:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140377c8e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377c93:  0F 1F 40 00              NOP dword ptr [RAX]
140377c97:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140377ca0:  41 8B C2                 MOV EAX,R10D
140377ca3:  48 03 C0                 ADD RAX,RAX
140377ca6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
140377caa:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
140377cae:  85 D2                    TEST EDX,EDX
140377cb0:  79 17                    JNS 0x140377cc9
140377cb2:  48 85 C9                 TEST RCX,RCX
140377cb5:  74 44                    JZ 0x140377cfb
140377cb7:  48 83 CA FF              OR RDX,-0x1
140377cbb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140377cc0:  48 FF C2                 INC RDX
140377cc3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140377cc7:  75 F7                    JNZ 0x140377cc0
140377cc9:  48 85 C9                 TEST RCX,RCX
140377ccc:  74 2D                    JZ 0x140377cfb
140377cce:  41 3B D4                 CMP EDX,R12D
140377cd1:  75 28                    JNZ 0x140377cfb
140377cd3:  4C 63 C2                 MOVSXD R8,EDX
140377cd6:  85 D2                    TEST EDX,EDX
140377cd8:  7E 3C                    JLE 0x140377d16
140377cda:  48 8B D1                 MOV RDX,RCX
140377cdd:  48 F7 DA                 NEG RDX
140377ce0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140377ce4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140377ce7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
140377ceb:  75 0E                    JNZ 0x140377cfb
140377ced:  48 FF C1                 INC RCX
140377cf0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140377cf4:  49 3B C0                 CMP RAX,R8
140377cf7:  7D 1D                    JGE 0x140377d16
140377cf9:  EB E9                    JMP 0x140377ce4
140377cfb:  41 FF C2                 INC R10D
140377cfe:  45 3B D3                 CMP R10D,R11D
140377d01:  72 9D                    JC 0x140377ca0
140377d03:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140377d08:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140377d0c:  E8 DF 79 0D 00           CALL 0x14044f6f0
140377d11:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140377d16:  48 85 DB                 TEST RBX,RBX
140377d19:  74 08                    JZ 0x140377d23
140377d1b:  48 8B CB                 MOV RCX,RBX
140377d1e:  E8 BD 62 4D 00           CALL 0x14084dfe0
140377d23:  41 B4 01                 MOV R12B,0x1
140377d26:  F3 0F 10 35 5A A3 61 00  MOVSS XMM6,dword ptr [0x140992088]
140377d2e:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140377d33:  BA E4 05 00 00           MOV EDX,0x5e4
140377d38:  48 8D 0D 51 F8 61 00     LEA RCX,[0x140997590]
140377d3f:  FF 15 4B 4B 4F 00        CALL qword ptr [0x14086c890]
140377d45:  0F 28 DF                 MOVAPS XMM3,XMM7
140377d48:  F3 0F 5C DE              SUBSS XMM3,XMM6
140377d4c:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140377d51:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377d56:  48 8D 05 A3 5F 54 00     LEA RAX,[0x1408bdd00]
140377d5d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377d62:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140377d6a:  66 41 0F 7E D9           MOVD R9D,XMM3
140377d6f:  0F 28 D6                 MOVAPS XMM2,XMM6
140377d72:  66 41 0F 7E D0           MOVD R8D,XMM2
140377d77:  49 8B D7                 MOV RDX,R15
140377d7a:  48 8D 0D 2F F2 61 00     LEA RCX,[0x140996fb0]
140377d81:  FF 15 F9 4A 4F 00        CALL qword ptr [0x14086c880]
140377d87:  BA E5 05 00 00           MOV EDX,0x5e5
140377d8c:  48 8D 0D FD F7 61 00     LEA RCX,[0x140997590]
140377d93:  FF 15 F7 4A 4F 00        CALL qword ptr [0x14086c890]
140377d99:  F3 0F 10 05 E7 A2 61 00  MOVSS XMM0,dword ptr [0x140992088]
140377da1:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140377da6:  0F 28 DF                 MOVAPS XMM3,XMM7
140377da9:  F3 0F 5C D8              SUBSS XMM3,XMM0
140377dad:  66 0F 6E 05 D7 D4 61 00  MOVD XMM0,dword ptr [0x14099528c]
140377db5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140377db8:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140377dbd:  F3 44 0F 10 75 B0        MOVSS XMM14,dword ptr [RBP + -0x50]
140377dc3:  41 0F 28 D6              MOVAPS XMM2,XMM14
140377dc7:  F3 0F 5C D0              SUBSS XMM2,XMM0
140377dcb:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140377dd0:  48 8D 05 19 5F 54 00     LEA RAX,[0x1408bdcf0]
140377dd7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377ddc:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140377de4:  66 41 0F 7E D9           MOVD R9D,XMM3
140377de9:  66 41 0F 7E D0           MOVD R8D,XMM2
140377dee:  49 8B D7                 MOV RDX,R15
140377df1:  48 8D 0D B8 F1 61 00     LEA RCX,[0x140996fb0]
140377df8:  FF 15 8A 4A 4F 00        CALL qword ptr [0x14086c888]
140377dfe:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
140377e05:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140377e08:  48 8D 0D C1 2E 67 00     LEA RCX,[0x1409eacd0]
140377e0f:  FF 15 DB 48 4F 00        CALL qword ptr [0x14086c6f0]
140377e15:  4C 8B C0                 MOV R8,RAX
140377e18:  33 D2                    XOR EDX,EDX
140377e1a:  48 8B CB                 MOV RCX,RBX
140377e1d:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140377e20:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140377e27:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377e2b:  41 0F 28 D1              MOVAPS XMM2,XMM9
140377e2f:  41 0F 28 C9              MOVAPS XMM1,XMM9
140377e33:  48 8D 8D A0 07 00 00     LEA RCX,[RBP + 0x7a0]
140377e3a:  FF 15 E0 41 4F 00        CALL qword ptr [0x14086c020]
140377e40:  48 8B D0                 MOV RDX,RAX
140377e43:  48 8D 0D 16 62 64 00     LEA RCX,[0x1409be060]
140377e4a:  FF 15 48 4A 4F 00        CALL qword ptr [0x14086c898]
140377e50:  F3 0F 10 0D 30 A2 61 00  MOVSS XMM1,dword ptr [0x140992088]
140377e58:  F3 0F 59 0D AC 2B 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140377e60:  0F 28 D1                 MOVAPS XMM2,XMM1
140377e63:  48 8D 0D F6 61 64 00     LEA RCX,[0x1409be060]
140377e6a:  FF 15 38 4A 4F 00        CALL qword ptr [0x14086c8a8]
140377e70:  F3 0F 10 15 10 A2 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377e78:  F3 0F 59 15 A8 29 59 00  MULSS XMM2,dword ptr [0x14090a828]
140377e80:  F3 0F 58 D7              ADDSS XMM2,XMM7
140377e84:  41 0F 28 D8              MOVAPS XMM3,XMM8
140377e88:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
140377e8e:  48 8D 8D B8 03 00 00     LEA RCX,[RBP + 0x3b8]
140377e95:  FF 15 6D 41 4F 00        CALL qword ptr [0x14086c008]
140377e9b:  48 8B D0                 MOV RDX,RAX
140377e9e:  48 8D 0D BB 61 64 00     LEA RCX,[0x1409be060]
140377ea5:  FF 15 F5 49 4F 00        CALL qword ptr [0x14086c8a0]
140377eab:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140377eb0:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140377eb7:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140377ebe:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377ec2:  41 0F 28 D0              MOVAPS XMM2,XMM8
140377ec6:  41 0F 28 C8              MOVAPS XMM1,XMM8
140377eca:  48 8D 0D 8F 61 64 00     LEA RCX,[0x1409be060]
140377ed1:  FF 15 89 49 4F 00        CALL qword ptr [0x14086c860]
140377ed7:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
140377ede:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140377ee1:  48 8D 0D E8 2D 67 00     LEA RCX,[0x1409eacd0]
140377ee8:  FF 15 02 48 4F 00        CALL qword ptr [0x14086c6f0]
140377eee:  4C 8B C0                 MOV R8,RAX
140377ef1:  33 D2                    XOR EDX,EDX
140377ef3:  48 8B CB                 MOV RCX,RBX
140377ef6:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140377ef9:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140377f00:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377f04:  41 0F 28 D1              MOVAPS XMM2,XMM9
140377f08:  41 0F 28 C9              MOVAPS XMM1,XMM9
140377f0c:  48 8D 8D B0 07 00 00     LEA RCX,[RBP + 0x7b0]
140377f13:  FF 15 07 41 4F 00        CALL qword ptr [0x14086c020]
140377f19:  48 8B D0                 MOV RDX,RAX
140377f1c:  48 8D 0D 3D 61 64 00     LEA RCX,[0x1409be060]
140377f23:  FF 15 6F 49 4F 00        CALL qword ptr [0x14086c898]
140377f29:  F3 0F 10 0D 57 A1 61 00  MOVSS XMM1,dword ptr [0x140992088]
140377f31:  F3 0F 59 0D D3 2A 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140377f39:  0F 28 D1                 MOVAPS XMM2,XMM1
140377f3c:  48 8D 0D 1D 61 64 00     LEA RCX,[0x1409be060]
140377f43:  FF 15 5F 49 4F 00        CALL qword ptr [0x14086c8a8]
140377f49:  F3 0F 10 15 37 A1 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377f51:  F3 0F 59 15 CF 28 59 00  MULSS XMM2,dword ptr [0x14090a828]
140377f59:  F3 0F 58 D7              ADDSS XMM2,XMM7
140377f5d:  41 0F 28 D8              MOVAPS XMM3,XMM8
140377f61:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140377f67:  48 8D 8D C4 03 00 00     LEA RCX,[RBP + 0x3c4]
140377f6e:  FF 15 94 40 4F 00        CALL qword ptr [0x14086c008]
140377f74:  48 8B D0                 MOV RDX,RAX
140377f77:  48 8D 0D E2 60 64 00     LEA RCX,[0x1409be060]
140377f7e:  FF 15 1C 49 4F 00        CALL qword ptr [0x14086c8a0]
140377f84:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140377f89:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140377f90:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140377f97:  41 0F 28 D9              MOVAPS XMM3,XMM9
140377f9b:  41 0F 28 D0              MOVAPS XMM2,XMM8
140377f9f:  41 0F 28 C8              MOVAPS XMM1,XMM8
140377fa3:  48 8D 0D B6 60 64 00     LEA RCX,[0x1409be060]
140377faa:  FF 15 B0 48 4F 00        CALL qword ptr [0x14086c860]
140377fb0:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140377fb5:  8B 05 B5 D2 61 00        MOV EAX,dword ptr [0x140995270]
140377fbb:  83 F8 02                 CMP EAX,0x2
140377fbe:  75 09                    JNZ 0x140377fc9
140377fc0:  48 8D 05 89 5C 54 00     LEA RAX,[0x1408bdc50]
140377fc7:  EB 13                    JMP 0x140377fdc
140377fc9:  83 F8 01                 CMP EAX,0x1
140377fcc:  48 8D 05 3D 5B 54 00     LEA RAX,[0x1408bdb10]
140377fd3:  74 07                    JZ 0x140377fdc
140377fd5:  48 8D 05 64 5A 54 00     LEA RAX,[0x1408bda40]
140377fdc:  F3 0F 10 15 A4 A0 61 00  MOVSS XMM2,dword ptr [0x140992088]
140377fe4:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140377fe9:  0F 28 DA                 MOVAPS XMM3,XMM2
140377fec:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140377ff1:  F3 0F 58 DF              ADDSS XMM3,XMM7
140377ff5:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
140377ffa:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378002:  66 41 0F 7E D9           MOVD R9D,XMM3
140378007:  66 41 0F 7E D0           MOVD R8D,XMM2
14037800c:  49 8B D7                 MOV RDX,R15
14037800f:  48 8D 0D 9A EF 61 00     LEA RCX,[0x140996fb0]
140378016:  FF 15 64 48 4F 00        CALL qword ptr [0x14086c880]
14037801c:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140378021:  F3 0F 10 35 5F A0 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378029:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037802e:  BA DF 05 00 00           MOV EDX,0x5df
140378033:  48 8D 0D 56 F5 61 00     LEA RCX,[0x140997590]
14037803a:  FF 15 50 48 4F 00        CALL qword ptr [0x14086c890]
140378040:  0F 28 DF                 MOVAPS XMM3,XMM7
140378043:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378047:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037804c:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378051:  48 8D 05 D0 59 54 00     LEA RAX,[0x1408bda28]
140378058:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037805d:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378065:  66 41 0F 7E D9           MOVD R9D,XMM3
14037806a:  0F 28 D6                 MOVAPS XMM2,XMM6
14037806d:  66 41 0F 7E D0           MOVD R8D,XMM2
140378072:  49 8B D7                 MOV RDX,R15
140378075:  48 8D 0D 34 EF 61 00     LEA RCX,[0x140996fb0]
14037807c:  FF 15 FE 47 4F 00        CALL qword ptr [0x14086c880]
140378082:  F3 0F 10 95 C0 00 00 00  MOVSS XMM2,dword ptr [RBP + 0xc0]
14037808a:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037808d:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140378092:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140378097:  45 33 C9                 XOR R9D,R9D
14037809a:  48 8D 95 30 02 00 00     LEA RDX,[RBP + 0x230]
1403780a1:  48 8B CE                 MOV RCX,RSI
1403780a4:  E8 B7 EB FB FF           CALL 0x140336c60
1403780a9:  90                       NOP
1403780aa:  48 8B 08                 MOV RCX,qword ptr [RAX]
1403780ad:  F3 0F 10 15 D3 9F 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403780b5:  0F 28 C2                 MOVAPS XMM0,XMM2
1403780b8:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403780bd:  0F 28 DF                 MOVAPS XMM3,XMM7
1403780c0:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403780c4:  F3 0F 10 35 9C 28 59 00  MOVSS XMM6,dword ptr [0x14090a968]
1403780cc:  F3 0F 59 D6              MULSS XMM2,XMM6
1403780d0:  F3 41 0F 58 D6           ADDSS XMM2,XMM14
1403780d5:  66 0F 6E 05 AF D1 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403780dd:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403780e0:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403780e4:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
1403780e9:  48 8D 05 F8 55 54 00     LEA RAX,[0x1408bd6e8]
1403780f0:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403780f5:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403780fd:  66 41 0F 7E D9           MOVD R9D,XMM3
140378102:  66 41 0F 7E D0           MOVD R8D,XMM2
140378107:  49 8B D7                 MOV RDX,R15
14037810a:  48 8D 0D 9F EE 61 00     LEA RCX,[0x140996fb0]
140378111:  FF 15 61 47 4F 00        CALL qword ptr [0x14086c878]
140378117:  90                       NOP
140378118:  48 8B 8D 30 02 00 00     MOV RCX,qword ptr [RBP + 0x230]
14037811f:  48 85 C9                 TEST RCX,RCX
140378122:  74 13                    JZ 0x140378137
140378124:  E8 B7 5E 4D 00           CALL 0x14084dfe0
140378129:  4C 89 B5 30 02 00 00     MOV qword ptr [RBP + 0x230],R14
140378130:  44 89 B5 38 02 00 00     MOV dword ptr [RBP + 0x238],R14D
140378137:  F3 0F 10 85 C4 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xc4]
14037813f:  F3 44 0F 10 35 D8 45 59 00 MOVSS XMM14,dword ptr [0x14090c720]
140378148:  41 0F 57 C6              XORPS XMM0,XMM14
14037814c:  0F 57 C9                 XORPS XMM1,XMM1
14037814f:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140378153:  45 33 C9                 XOR R9D,R9D
140378156:  45 33 C0                 XOR R8D,R8D
140378159:  48 8B CE                 MOV RCX,RSI
14037815c:  E8 BF E2 FB FF           CALL 0x140336420
140378161:  F3 0F 10 15 1F 9F 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378169:  0F 28 C2                 MOVAPS XMM0,XMM2
14037816c:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378171:  0F 28 DF                 MOVAPS XMM3,XMM7
140378174:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378178:  F3 0F 59 D6              MULSS XMM2,XMM6
14037817c:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
140378182:  66 0F 6E 05 02 D1 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037818a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037818d:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378191:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378196:  48 8D 05 6B 59 54 00     LEA RAX,[0x1408bdb08]
14037819d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403781a2:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403781aa:  66 41 0F 7E D9           MOVD R9D,XMM3
1403781af:  66 41 0F 7E D0           MOVD R8D,XMM2
1403781b4:  49 8B D7                 MOV RDX,R15
1403781b7:  48 8D 0D F2 ED 61 00     LEA RCX,[0x140996fb0]
1403781be:  FF 15 B4 46 4F 00        CALL qword ptr [0x14086c878]
1403781c4:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
1403781ca:  F3 0F 10 35 B6 9E 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403781d2:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403781d7:  BA E0 05 00 00           MOV EDX,0x5e0
1403781dc:  48 8D 0D AD F3 61 00     LEA RCX,[0x140997590]
1403781e3:  FF 15 A7 46 4F 00        CALL qword ptr [0x14086c890]
1403781e9:  0F 28 DF                 MOVAPS XMM3,XMM7
1403781ec:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403781f0:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
1403781f5:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403781fa:  48 8D 05 07 5A 54 00     LEA RAX,[0x1408bdc08]
140378201:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378206:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037820e:  66 41 0F 7E D9           MOVD R9D,XMM3
140378213:  0F 28 D6                 MOVAPS XMM2,XMM6
140378216:  66 41 0F 7E D0           MOVD R8D,XMM2
14037821b:  49 8B D7                 MOV RDX,R15
14037821e:  48 8D 0D 8B ED 61 00     LEA RCX,[0x140996fb0]
140378225:  FF 15 55 46 4F 00        CALL qword ptr [0x14086c880]
14037822b:  F3 0F 10 95 D4 00 00 00  MOVSS XMM2,dword ptr [RBP + 0xd4]
140378233:  0F 5A D2                 CVTPS2PD XMM2,XMM2
140378236:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037823b:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140378240:  45 33 C9                 XOR R9D,R9D
140378243:  48 8D 95 40 02 00 00     LEA RDX,[RBP + 0x240]
14037824a:  48 8B CE                 MOV RCX,RSI
14037824d:  E8 0E EA FB FF           CALL 0x140336c60
140378252:  90                       NOP
140378253:  48 8B 08                 MOV RCX,qword ptr [RAX]
140378256:  F3 0F 10 15 2A 9E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037825e:  0F 28 C2                 MOVAPS XMM0,XMM2
140378261:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378266:  0F 28 DF                 MOVAPS XMM3,XMM7
140378269:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037826d:  F3 0F 10 35 F3 26 59 00  MOVSS XMM6,dword ptr [0x14090a968]
140378275:  F3 0F 59 D6              MULSS XMM2,XMM6
140378279:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
14037827e:  66 0F 6E 05 06 D0 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378286:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378289:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037828d:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140378292:  48 8D 05 9F 57 54 00     LEA RAX,[0x1408bda38]
140378299:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037829e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403782a6:  66 41 0F 7E D9           MOVD R9D,XMM3
1403782ab:  66 41 0F 7E D0           MOVD R8D,XMM2
1403782b0:  49 8B D7                 MOV RDX,R15
1403782b3:  48 8D 0D F6 EC 61 00     LEA RCX,[0x140996fb0]
1403782ba:  FF 15 B8 45 4F 00        CALL qword ptr [0x14086c878]
1403782c0:  90                       NOP
1403782c1:  48 8B 8D 40 02 00 00     MOV RCX,qword ptr [RBP + 0x240]
1403782c8:  48 85 C9                 TEST RCX,RCX
1403782cb:  74 13                    JZ 0x1403782e0
1403782cd:  E8 0E 5D 4D 00           CALL 0x14084dfe0
1403782d2:  4C 89 B5 40 02 00 00     MOV qword ptr [RBP + 0x240],R14
1403782d9:  44 89 B5 48 02 00 00     MOV dword ptr [RBP + 0x248],R14D
1403782e0:  F3 0F 10 85 D8 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xd8]
1403782e8:  41 0F 57 C6              XORPS XMM0,XMM14
1403782ec:  0F 57 C9                 XORPS XMM1,XMM1
1403782ef:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
1403782f3:  45 33 C9                 XOR R9D,R9D
1403782f6:  45 33 C0                 XOR R8D,R8D
1403782f9:  48 8B CE                 MOV RCX,RSI
1403782fc:  E8 1F E1 FB FF           CALL 0x140336420
140378301:  F3 0F 10 15 7F 9D 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378309:  0F 28 C2                 MOVAPS XMM0,XMM2
14037830c:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378311:  0F 28 DF                 MOVAPS XMM3,XMM7
140378314:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378318:  F3 0F 59 D6              MULSS XMM2,XMM6
14037831c:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140378322:  66 0F 6E 05 62 CF 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037832a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037832d:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378331:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378336:  48 8D 05 4B 55 54 00     LEA RAX,[0x1408bd888]
14037833d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378342:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037834a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037834f:  66 41 0F 7E D0           MOVD R8D,XMM2
140378354:  49 8B D7                 MOV RDX,R15
140378357:  48 8D 0D 52 EC 61 00     LEA RCX,[0x140996fb0]
14037835e:  FF 15 14 45 4F 00        CALL qword ptr [0x14086c878]
140378364:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
14037836a:  F3 0F 10 35 16 9D 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378372:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140378377:  BA E6 05 00 00           MOV EDX,0x5e6
14037837c:  48 8D 0D 0D F2 61 00     LEA RCX,[0x140997590]
140378383:  FF 15 07 45 4F 00        CALL qword ptr [0x14086c890]
140378389:  0F 28 DF                 MOVAPS XMM3,XMM7
14037838c:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378390:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140378395:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037839a:  48 8D 05 AF 5B 54 00     LEA RAX,[0x1408bdf50]
1403783a1:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403783a6:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403783ae:  66 41 0F 7E D9           MOVD R9D,XMM3
1403783b3:  0F 28 D6                 MOVAPS XMM2,XMM6
1403783b6:  66 41 0F 7E D0           MOVD R8D,XMM2
1403783bb:  49 8B D7                 MOV RDX,R15
1403783be:  48 8D 0D EB EB 61 00     LEA RCX,[0x140996fb0]
1403783c5:  FF 15 B5 44 4F 00        CALL qword ptr [0x14086c880]
1403783cb:  F3 0F 10 4D C0           MOVSS XMM1,dword ptr [RBP + -0x40]
1403783d0:  0F 5A C9                 CVTPS2PD XMM1,XMM1
1403783d3:  F3 0F 10 15 AD 9C 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403783db:  0F 28 C2                 MOVAPS XMM0,XMM2
1403783de:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403783e3:  0F 28 DF                 MOVAPS XMM3,XMM7
1403783e6:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403783ea:  F3 0F 59 15 76 25 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403783f2:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
1403783f8:  66 0F 6E 05 8C CE 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378400:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378403:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378407:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
14037840d:  48 8D 05 2C 5B 54 00     LEA RAX,[0x1408bdf40]
140378414:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378419:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378421:  66 41 0F 7E D9           MOVD R9D,XMM3
140378426:  66 41 0F 7E D0           MOVD R8D,XMM2
14037842b:  49 8B D7                 MOV RDX,R15
14037842e:  48 8D 0D 7B EB 61 00     LEA RCX,[0x140996fb0]
140378435:  FF 15 3D 44 4F 00        CALL qword ptr [0x14086c878]
14037843b:  F3 0F 10 4D F0           MOVSS XMM1,dword ptr [RBP + -0x10]
140378440:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140378443:  F3 0F 10 15 3D 9C 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037844b:  0F 28 C2                 MOVAPS XMM0,XMM2
14037844e:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378453:  0F 28 DF                 MOVAPS XMM3,XMM7
140378456:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037845a:  F3 0F 59 15 06 25 59 00  MULSS XMM2,dword ptr [0x14090a968]
140378462:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140378468:  66 0F 6E 05 1C CE 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378470:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378473:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378477:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
14037847d:  48 8D 05 AC 5A 54 00     LEA RAX,[0x1408bdf30]
140378484:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378489:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378491:  66 41 0F 7E D9           MOVD R9D,XMM3
140378496:  66 41 0F 7E D0           MOVD R8D,XMM2
14037849b:  49 8B D7                 MOV RDX,R15
14037849e:  48 8D 0D 0B EB 61 00     LEA RCX,[0x140996fb0]
1403784a5:  FF 15 CD 43 4F 00        CALL qword ptr [0x14086c878]
1403784ab:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
1403784b1:  F3 0F 10 35 CF 9B 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403784b9:  0F 28 C6                 MOVAPS XMM0,XMM6
1403784bc:  F3 0F 59 05 64 23 59 00  MULSS XMM0,dword ptr [0x14090a828]
1403784c4:  F3 0F 58 F8              ADDSS XMM7,XMM0
1403784c8:  0F 28 CE                 MOVAPS XMM1,XMM6
1403784cb:  F3 41 0F 59 C8           MULSS XMM1,XMM8
1403784d0:  0F 28 C7                 MOVAPS XMM0,XMM7
1403784d3:  F3 0F 5C C1              SUBSS XMM0,XMM1
1403784d7:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
1403784db:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
1403784df:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
1403784e4:  F3 44 0F 58 D1           ADDSS XMM10,XMM1
1403784e9:  F3 41 0F 2C FA           CVTTSS2SI EDI,XMM10
1403784ee:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
1403784f2:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
1403784f9:  75 4D                    JNZ 0x140378548
1403784fb:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140378500:  48 C1 E9 20              SHR RCX,0x20
140378504:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140378509:  48 C1 E8 20              SHR RAX,0x20
14037850d:  03 C8                    ADD ECX,EAX
14037850f:  66 0F 6E D1              MOVD XMM2,ECX
140378513:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140378516:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037851b:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
14037851f:  41 03 C5                 ADD EAX,R13D
140378522:  66 0F 6E C8              MOVD XMM1,EAX
140378526:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140378529:  F3 41 0F 59 CB           MULSS XMM1,XMM11
14037852e:  41 B9 06 00 00 00        MOV R9D,0x6
140378534:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037853b:  E8 B0 DC 0C 00           CALL 0x1404461f0
140378540:  F3 0F 10 35 40 9B 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378548:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037854d:  0F 84 AD 02 00 00        JZ 0x140378800
140378553:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140378559:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037855c:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140378561:  0F 2F C8                 COMISS XMM1,XMM0
140378564:  0F 86 96 02 00 00        JBE 0x140378800
14037856a:  66 41 0F 6E C5           MOVD XMM0,R13D
14037856f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378572:  0F 2F C1                 COMISS XMM0,XMM1
140378575:  0F 86 85 02 00 00        JBE 0x140378800
14037857b:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140378580:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140378585:  66 0F 6E C3              MOVD XMM0,EBX
140378589:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037858c:  0F 2F C8                 COMISS XMM1,XMM0
14037858f:  0F 86 6B 02 00 00        JBE 0x140378800
140378595:  66 0F 6E C7              MOVD XMM0,EDI
140378599:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037859c:  0F 2F C1                 COMISS XMM0,XMM1
14037859f:  0F 86 5B 02 00 00        JBE 0x140378800
1403785a5:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
1403785ac:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403785af:  48 8D 0D 1A 27 67 00     LEA RCX,[0x1409eacd0]
1403785b6:  FF 15 34 41 4F 00        CALL qword ptr [0x14086c6f0]
1403785bc:  4C 8B C0                 MOV R8,RAX
1403785bf:  33 D2                    XOR EDX,EDX
1403785c1:  48 8B CB                 MOV RCX,RBX
1403785c4:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403785c7:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
1403785ce:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403785d2:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403785d6:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403785da:  48 8D 8D C0 07 00 00     LEA RCX,[RBP + 0x7c0]
1403785e1:  FF 15 39 3A 4F 00        CALL qword ptr [0x14086c020]
1403785e7:  48 8B D0                 MOV RDX,RAX
1403785ea:  48 8D 0D 6F 5A 64 00     LEA RCX,[0x1409be060]
1403785f1:  FF 15 A1 42 4F 00        CALL qword ptr [0x14086c898]
1403785f7:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
1403785fc:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140378602:  F3 0F 10 15 7E 9A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037860a:  F3 0F 59 15 16 23 59 00  MULSS XMM2,dword ptr [0x14090a928]
140378612:  F3 0F 10 05 5E 18 59 00  MOVSS XMM0,dword ptr [0x140909e78]
14037861a:  F3 0F 11 44 24 38        MOVSS dword ptr [RSP + 0x38],XMM0
140378620:  F3 44 0F 10 15 DB 16 59 00 MOVSS XMM10,dword ptr [0x140909d04]
140378629:  F3 44 0F 11 54 24 30     MOVSS dword ptr [RSP + 0x30],XMM10
140378630:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140378636:  F3 44 0F 11 54 24 20     MOVSS dword ptr [RSP + 0x20],XMM10
14037863d:  0F 28 DA                 MOVAPS XMM3,XMM2
140378640:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140378645:  48 8D 0D 14 5A 64 00     LEA RCX,[0x1409be060]
14037864c:  FF 15 06 42 4F 00        CALL qword ptr [0x14086c858]
140378652:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140378657:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037865c:  4C 8D 05 B5 58 54 00     LEA R8,[0x1408bdf18]
140378663:  83 CA FF                 OR EDX,0xffffffff
140378666:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037866b:  E8 B0 26 EE FF           CALL 0x14025ad20
140378670:  90                       NOP
140378671:  45 8B D6                 MOV R10D,R14D
140378674:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140378678:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037867c:  4C 2B DF                 SUB R11,RDI
14037867f:  49 C1 FB 04              SAR R11,0x4
140378683:  45 85 DB                 TEST R11D,R11D
140378686:  74 7B                    JZ 0x140378703
140378688:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037868d:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140378692:  0F 1F 40 00              NOP dword ptr [RAX]
140378696:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
1403786a0:  41 8B C2                 MOV EAX,R10D
1403786a3:  48 03 C0                 ADD RAX,RAX
1403786a6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
1403786aa:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
1403786ae:  85 D2                    TEST EDX,EDX
1403786b0:  79 17                    JNS 0x1403786c9
1403786b2:  48 85 C9                 TEST RCX,RCX
1403786b5:  74 44                    JZ 0x1403786fb
1403786b7:  48 83 CA FF              OR RDX,-0x1
1403786bb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
1403786c0:  48 FF C2                 INC RDX
1403786c3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
1403786c7:  75 F7                    JNZ 0x1403786c0
1403786c9:  48 85 C9                 TEST RCX,RCX
1403786cc:  74 2D                    JZ 0x1403786fb
1403786ce:  41 3B D4                 CMP EDX,R12D
1403786d1:  75 28                    JNZ 0x1403786fb
1403786d3:  4C 63 C2                 MOVSXD R8,EDX
1403786d6:  85 D2                    TEST EDX,EDX
1403786d8:  7E 3C                    JLE 0x140378716
1403786da:  48 8B D1                 MOV RDX,RCX
1403786dd:  48 F7 DA                 NEG RDX
1403786e0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
1403786e4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
1403786e7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
1403786eb:  75 0E                    JNZ 0x1403786fb
1403786ed:  48 FF C1                 INC RCX
1403786f0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
1403786f4:  49 3B C0                 CMP RAX,R8
1403786f7:  7D 1D                    JGE 0x140378716
1403786f9:  EB E9                    JMP 0x1403786e4
1403786fb:  41 FF C2                 INC R10D
1403786fe:  45 3B D3                 CMP R10D,R11D
140378701:  72 9D                    JC 0x1403786a0
140378703:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140378708:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037870c:  E8 DF 6F 0D 00           CALL 0x14044f6f0
140378711:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140378716:  48 85 DB                 TEST RBX,RBX
140378719:  74 08                    JZ 0x140378723
14037871b:  48 8B CB                 MOV RCX,RBX
14037871e:  E8 BD 58 4D 00           CALL 0x14084dfe0
140378723:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140378728:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037872d:  4C 8D 05 EC 58 54 00     LEA R8,[0x1408be020]
140378734:  83 CA FF                 OR EDX,0xffffffff
140378737:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037873c:  E8 DF 25 EE FF           CALL 0x14025ad20
140378741:  90                       NOP
140378742:  45 8B D6                 MOV R10D,R14D
140378745:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140378749:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037874d:  4C 2B DF                 SUB R11,RDI
140378750:  49 C1 FB 04              SAR R11,0x4
140378754:  45 85 DB                 TEST R11D,R11D
140378757:  74 7A                    JZ 0x1403787d3
140378759:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037875e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140378763:  0F 1F 40 00              NOP dword ptr [RAX]
140378767:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140378770:  41 8B C2                 MOV EAX,R10D
140378773:  48 03 C0                 ADD RAX,RAX
140378776:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037877a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037877e:  85 D2                    TEST EDX,EDX
140378780:  79 17                    JNS 0x140378799
140378782:  48 85 C9                 TEST RCX,RCX
140378785:  74 44                    JZ 0x1403787cb
140378787:  48 83 CA FF              OR RDX,-0x1
14037878b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140378790:  48 FF C2                 INC RDX
140378793:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140378797:  75 F7                    JNZ 0x140378790
140378799:  48 85 C9                 TEST RCX,RCX
14037879c:  74 2D                    JZ 0x1403787cb
14037879e:  41 3B D4                 CMP EDX,R12D
1403787a1:  75 28                    JNZ 0x1403787cb
1403787a3:  4C 63 C2                 MOVSXD R8,EDX
1403787a6:  85 D2                    TEST EDX,EDX
1403787a8:  7E 3C                    JLE 0x1403787e6
1403787aa:  48 8B D1                 MOV RDX,RCX
1403787ad:  48 F7 DA                 NEG RDX
1403787b0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
1403787b4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
1403787b7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
1403787bb:  75 0E                    JNZ 0x1403787cb
1403787bd:  48 FF C1                 INC RCX
1403787c0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
1403787c4:  49 3B C0                 CMP RAX,R8
1403787c7:  7D 1D                    JGE 0x1403787e6
1403787c9:  EB E9                    JMP 0x1403787b4
1403787cb:  41 FF C2                 INC R10D
1403787ce:  45 3B D3                 CMP R10D,R11D
1403787d1:  72 9D                    JC 0x140378770
1403787d3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403787d8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
1403787dc:  E8 0F 6F 0D 00           CALL 0x14044f6f0
1403787e1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
1403787e6:  48 85 DB                 TEST RBX,RBX
1403787e9:  74 08                    JZ 0x1403787f3
1403787eb:  48 8B CB                 MOV RCX,RBX
1403787ee:  E8 ED 57 4D 00           CALL 0x14084dfe0
1403787f3:  41 B4 01                 MOV R12B,0x1
1403787f6:  F3 0F 10 35 8A 98 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403787fe:  EB 09                    JMP 0x140378809
140378800:  F3 44 0F 10 15 FB 14 59 00 MOVSS XMM10,dword ptr [0x140909d04]
140378809:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037880e:  BA 7C 06 00 00           MOV EDX,0x67c
140378813:  48 8D 0D 76 ED 61 00     LEA RCX,[0x140997590]
14037881a:  FF 15 70 40 4F 00        CALL qword ptr [0x14086c890]
140378820:  0F 28 DF                 MOVAPS XMM3,XMM7
140378823:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378827:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037882c:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378831:  48 8D 05 D8 57 54 00     LEA RAX,[0x1408be010]
140378838:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037883d:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140378845:  66 41 0F 7E D9           MOVD R9D,XMM3
14037884a:  0F 28 D6                 MOVAPS XMM2,XMM6
14037884d:  66 41 0F 7E D0           MOVD R8D,XMM2
140378852:  49 8B D7                 MOV RDX,R15
140378855:  48 8D 0D 54 E7 61 00     LEA RCX,[0x140996fb0]
14037885c:  FF 15 1E 40 4F 00        CALL qword ptr [0x14086c880]
140378862:  BA 77 06 00 00           MOV EDX,0x677
140378867:  48 8D 0D 22 ED 61 00     LEA RCX,[0x140997590]
14037886e:  FF 15 1C 40 4F 00        CALL qword ptr [0x14086c890]
140378874:  F3 0F 10 05 0C 98 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037887c:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378881:  0F 28 DF                 MOVAPS XMM3,XMM7
140378884:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378888:  66 0F 6E 05 FC C9 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378890:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378893:  F3 41 0F 59 C3           MULSS XMM0,XMM11
140378898:  F3 0F 10 55 B0           MOVSS XMM2,dword ptr [RBP + -0x50]
14037889d:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403788a1:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403788a6:  48 8D 05 53 57 54 00     LEA RAX,[0x1408be000]
1403788ad:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403788b2:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403788ba:  66 41 0F 7E D9           MOVD R9D,XMM3
1403788bf:  66 41 0F 7E D0           MOVD R8D,XMM2
1403788c4:  49 8B D7                 MOV RDX,R15
1403788c7:  48 8D 0D E2 E6 61 00     LEA RCX,[0x140996fb0]
1403788ce:  FF 15 B4 3F 4F 00        CALL qword ptr [0x14086c888]
1403788d4:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
1403788db:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403788de:  48 8D 0D EB 23 67 00     LEA RCX,[0x1409eacd0]
1403788e5:  FF 15 05 3E 4F 00        CALL qword ptr [0x14086c6f0]
1403788eb:  4C 8B C0                 MOV R8,RAX
1403788ee:  33 D2                    XOR EDX,EDX
1403788f0:  48 8B CB                 MOV RCX,RBX
1403788f3:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403788f6:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403788fd:  41 0F 28 D9              MOVAPS XMM3,XMM9
140378901:  41 0F 28 D1              MOVAPS XMM2,XMM9
140378905:  41 0F 28 C9              MOVAPS XMM1,XMM9
140378909:  48 8D 8D D0 07 00 00     LEA RCX,[RBP + 0x7d0]
140378910:  FF 15 0A 37 4F 00        CALL qword ptr [0x14086c020]
140378916:  48 8B D0                 MOV RDX,RAX
140378919:  48 8D 0D 40 57 64 00     LEA RCX,[0x1409be060]
140378920:  FF 15 72 3F 4F 00        CALL qword ptr [0x14086c898]
140378926:  F3 0F 10 0D 5A 97 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037892e:  F3 0F 59 0D D6 20 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140378936:  0F 28 D1                 MOVAPS XMM2,XMM1
140378939:  48 8D 0D 20 57 64 00     LEA RCX,[0x1409be060]
140378940:  FF 15 62 3F 4F 00        CALL qword ptr [0x14086c8a8]
140378946:  F3 0F 10 15 3A 97 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037894e:  F3 0F 59 15 D2 1E 59 00  MULSS XMM2,dword ptr [0x14090a828]
140378956:  F3 0F 58 D7              ADDSS XMM2,XMM7
14037895a:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037895e:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
140378964:  48 8D 8D D0 03 00 00     LEA RCX,[RBP + 0x3d0]
14037896b:  FF 15 97 36 4F 00        CALL qword ptr [0x14086c008]
140378971:  48 8B D0                 MOV RDX,RAX
140378974:  48 8D 0D E5 56 64 00     LEA RCX,[0x1409be060]
14037897b:  FF 15 1F 3F 4F 00        CALL qword ptr [0x14086c8a0]
140378981:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140378986:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037898d:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140378994:  41 0F 28 D9              MOVAPS XMM3,XMM9
140378998:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037899c:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403789a0:  48 8D 0D B9 56 64 00     LEA RCX,[0x1409be060]
1403789a7:  FF 15 B3 3E 4F 00        CALL qword ptr [0x14086c860]
1403789ad:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
1403789b4:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403789b7:  48 8D 0D 12 23 67 00     LEA RCX,[0x1409eacd0]
1403789be:  FF 15 2C 3D 4F 00        CALL qword ptr [0x14086c6f0]
1403789c4:  4C 8B C0                 MOV R8,RAX
1403789c7:  33 D2                    XOR EDX,EDX
1403789c9:  48 8B CB                 MOV RCX,RBX
1403789cc:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403789cf:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403789d6:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403789da:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403789de:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403789e2:  48 8D 8D E0 07 00 00     LEA RCX,[RBP + 0x7e0]
1403789e9:  FF 15 31 36 4F 00        CALL qword ptr [0x14086c020]
1403789ef:  48 8B D0                 MOV RDX,RAX
1403789f2:  48 8D 0D 67 56 64 00     LEA RCX,[0x1409be060]
1403789f9:  FF 15 99 3E 4F 00        CALL qword ptr [0x14086c898]
1403789ff:  F3 0F 10 0D 81 96 61 00  MOVSS XMM1,dword ptr [0x140992088]
140378a07:  F3 0F 59 0D FD 1F 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140378a0f:  0F 28 D1                 MOVAPS XMM2,XMM1
140378a12:  48 8D 0D 47 56 64 00     LEA RCX,[0x1409be060]
140378a19:  FF 15 89 3E 4F 00        CALL qword ptr [0x14086c8a8]
140378a1f:  F3 0F 10 15 61 96 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378a27:  F3 0F 59 15 F9 1D 59 00  MULSS XMM2,dword ptr [0x14090a828]
140378a2f:  F3 0F 58 D7              ADDSS XMM2,XMM7
140378a33:  41 0F 28 D8              MOVAPS XMM3,XMM8
140378a37:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
140378a3d:  48 8D 8D DC 03 00 00     LEA RCX,[RBP + 0x3dc]
140378a44:  FF 15 BE 35 4F 00        CALL qword ptr [0x14086c008]
140378a4a:  48 8B D0                 MOV RDX,RAX
140378a4d:  48 8D 0D 0C 56 64 00     LEA RCX,[0x1409be060]
140378a54:  FF 15 46 3E 4F 00        CALL qword ptr [0x14086c8a0]
140378a5a:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140378a5f:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140378a66:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140378a6d:  41 0F 28 D9              MOVAPS XMM3,XMM9
140378a71:  41 0F 28 D0              MOVAPS XMM2,XMM8
140378a75:  41 0F 28 C8              MOVAPS XMM1,XMM8
140378a79:  48 8D 0D E0 55 64 00     LEA RCX,[0x1409be060]
140378a80:  FF 15 DA 3D 4F 00        CALL qword ptr [0x14086c860]
140378a86:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140378a8b:  8B 05 DF C7 61 00        MOV EAX,dword ptr [0x140995270]
140378a91:  83 F8 02                 CMP EAX,0x2
140378a94:  75 09                    JNZ 0x140378a9f
140378a96:  48 8D 05 C3 54 54 00     LEA RAX,[0x1408bdf60]
140378a9d:  EB 13                    JMP 0x140378ab2
140378a9f:  83 F8 01                 CMP EAX,0x1
140378aa2:  48 8D 05 57 53 54 00     LEA RAX,[0x1408bde00]
140378aa9:  74 07                    JZ 0x140378ab2
140378aab:  48 8D 05 7E 52 54 00     LEA RAX,[0x1408bdd30]
140378ab2:  F3 0F 10 15 CE 95 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378aba:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140378abf:  0F 28 DA                 MOVAPS XMM3,XMM2
140378ac2:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378ac7:  F3 0F 58 DF              ADDSS XMM3,XMM7
140378acb:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
140378ad0:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378ad8:  66 41 0F 7E D9           MOVD R9D,XMM3
140378add:  66 41 0F 7E D0           MOVD R8D,XMM2
140378ae2:  49 8B D7                 MOV RDX,R15
140378ae5:  48 8D 0D C4 E4 61 00     LEA RCX,[0x140996fb0]
140378aec:  FF 15 8E 3D 4F 00        CALL qword ptr [0x14086c880]
140378af2:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140378af7:  F3 0F 10 35 89 95 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378aff:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140378b04:  BA DF 05 00 00           MOV EDX,0x5df
140378b09:  48 8D 0D 80 EA 61 00     LEA RCX,[0x140997590]
140378b10:  FF 15 7A 3D 4F 00        CALL qword ptr [0x14086c890]
140378b16:  0F 28 DF                 MOVAPS XMM3,XMM7
140378b19:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378b1d:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140378b22:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378b27:  48 8D 05 F2 51 54 00     LEA RAX,[0x1408bdd20]
140378b2e:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378b33:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378b3b:  66 41 0F 7E D9           MOVD R9D,XMM3
140378b40:  0F 28 D6                 MOVAPS XMM2,XMM6
140378b43:  66 41 0F 7E D0           MOVD R8D,XMM2
140378b48:  49 8B D7                 MOV RDX,R15
140378b4b:  48 8D 0D 5E E4 61 00     LEA RCX,[0x140996fb0]
140378b52:  FF 15 28 3D 4F 00        CALL qword ptr [0x14086c880]
140378b58:  66 0F 6E 95 F8 00 00 00  MOVD XMM2,dword ptr [RBP + 0xf8]
140378b60:  F3 0F E6 D2              CVTDQ2PD XMM2,XMM2
140378b64:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140378b69:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140378b6e:  45 33 C9                 XOR R9D,R9D
140378b71:  48 8D 95 50 02 00 00     LEA RDX,[RBP + 0x250]
140378b78:  48 8B CE                 MOV RCX,RSI
140378b7b:  E8 E0 E0 FB FF           CALL 0x140336c60
140378b80:  90                       NOP
140378b81:  48 8B 08                 MOV RCX,qword ptr [RAX]
140378b84:  F3 0F 10 15 FC 94 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378b8c:  0F 28 C2                 MOVAPS XMM0,XMM2
140378b8f:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378b94:  0F 28 DF                 MOVAPS XMM3,XMM7
140378b97:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378b9b:  F3 0F 10 35 C5 1D 59 00  MOVSS XMM6,dword ptr [0x14090a968]
140378ba3:  F3 0F 59 D6              MULSS XMM2,XMM6
140378ba7:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
140378bac:  66 0F 6E 05 D8 C6 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378bb4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378bb7:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378bbb:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140378bc0:  48 8D 05 81 50 54 00     LEA RAX,[0x1408bdc48]
140378bc7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378bcc:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378bd4:  66 41 0F 7E D9           MOVD R9D,XMM3
140378bd9:  66 41 0F 7E D0           MOVD R8D,XMM2
140378bde:  49 8B D7                 MOV RDX,R15
140378be1:  48 8D 0D C8 E3 61 00     LEA RCX,[0x140996fb0]
140378be8:  FF 15 8A 3C 4F 00        CALL qword ptr [0x14086c878]
140378bee:  90                       NOP
140378bef:  48 8B 8D 50 02 00 00     MOV RCX,qword ptr [RBP + 0x250]
140378bf6:  48 85 C9                 TEST RCX,RCX
140378bf9:  74 13                    JZ 0x140378c0e
140378bfb:  E8 E0 53 4D 00           CALL 0x14084dfe0
140378c00:  4C 89 B5 50 02 00 00     MOV qword ptr [RBP + 0x250],R14
140378c07:  44 89 B5 58 02 00 00     MOV dword ptr [RBP + 0x258],R14D
140378c0e:  F3 0F 10 85 FC 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xfc]
140378c16:  41 0F 57 C6              XORPS XMM0,XMM14
140378c1a:  0F 57 C9                 XORPS XMM1,XMM1
140378c1d:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140378c21:  45 33 C9                 XOR R9D,R9D
140378c24:  45 33 C0                 XOR R8D,R8D
140378c27:  48 8B CE                 MOV RCX,RSI
140378c2a:  E8 F1 D7 FB FF           CALL 0x140336420
140378c2f:  F3 0F 10 15 51 94 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378c37:  0F 28 C2                 MOVAPS XMM0,XMM2
140378c3a:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378c3f:  0F 28 DF                 MOVAPS XMM3,XMM7
140378c42:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378c46:  F3 0F 59 D6              MULSS XMM2,XMM6
140378c4a:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
140378c50:  66 0F 6E 05 34 C6 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378c58:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378c5b:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378c5f:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378c64:  48 8D 05 A5 52 54 00     LEA RAX,[0x1408bdf10]
140378c6b:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378c70:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378c78:  66 41 0F 7E D9           MOVD R9D,XMM3
140378c7d:  66 41 0F 7E D0           MOVD R8D,XMM2
140378c82:  49 8B D7                 MOV RDX,R15
140378c85:  48 8D 0D 24 E3 61 00     LEA RCX,[0x140996fb0]
140378c8c:  FF 15 E6 3B 4F 00        CALL qword ptr [0x14086c878]
140378c92:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
140378c98:  F3 0F 10 35 E8 93 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378ca0:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140378ca5:  BA 7D 06 00 00           MOV EDX,0x67d
140378caa:  48 8D 0D DF E8 61 00     LEA RCX,[0x140997590]
140378cb1:  FF 15 D9 3B 4F 00        CALL qword ptr [0x14086c890]
140378cb7:  0F 28 DF                 MOVAPS XMM3,XMM7
140378cba:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378cbe:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140378cc3:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378cc8:  48 8D 05 31 52 54 00     LEA RAX,[0x1408bdf00]
140378ccf:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378cd4:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378cdc:  66 41 0F 7E D9           MOVD R9D,XMM3
140378ce1:  0F 28 D6                 MOVAPS XMM2,XMM6
140378ce4:  66 41 0F 7E D0           MOVD R8D,XMM2
140378ce9:  49 8B D7                 MOV RDX,R15
140378cec:  48 8D 0D BD E2 61 00     LEA RCX,[0x140996fb0]
140378cf3:  FF 15 87 3B 4F 00        CALL qword ptr [0x14086c880]
140378cf9:  66 0F 6E 95 EC 00 00 00  MOVD XMM2,dword ptr [RBP + 0xec]
140378d01:  F3 0F E6 D2              CVTDQ2PD XMM2,XMM2
140378d05:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140378d0a:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140378d0f:  45 33 C9                 XOR R9D,R9D
140378d12:  48 8D 95 60 02 00 00     LEA RDX,[RBP + 0x260]
140378d19:  48 8B CE                 MOV RCX,RSI
140378d1c:  E8 3F DF FB FF           CALL 0x140336c60
140378d21:  90                       NOP
140378d22:  48 8B 08                 MOV RCX,qword ptr [RAX]
140378d25:  F3 0F 10 15 5B 93 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378d2d:  0F 28 C2                 MOVAPS XMM0,XMM2
140378d30:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378d35:  0F 28 DF                 MOVAPS XMM3,XMM7
140378d38:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378d3c:  F3 0F 59 15 24 1C 59 00  MULSS XMM2,dword ptr [0x14090a968]
140378d44:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
140378d49:  66 0F 6E 05 3B C5 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378d51:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378d54:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378d58:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140378d5d:  48 8D 05 94 51 54 00     LEA RAX,[0x1408bdef8]
140378d64:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378d69:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378d71:  66 41 0F 7E D9           MOVD R9D,XMM3
140378d76:  66 41 0F 7E D0           MOVD R8D,XMM2
140378d7b:  49 8B D7                 MOV RDX,R15
140378d7e:  48 8D 0D 2B E2 61 00     LEA RCX,[0x140996fb0]
140378d85:  FF 15 ED 3A 4F 00        CALL qword ptr [0x14086c878]
140378d8b:  90                       NOP
140378d8c:  48 8B 8D 60 02 00 00     MOV RCX,qword ptr [RBP + 0x260]
140378d93:  48 85 C9                 TEST RCX,RCX
140378d96:  74 13                    JZ 0x140378dab
140378d98:  E8 43 52 4D 00           CALL 0x14084dfe0
140378d9d:  4C 89 B5 60 02 00 00     MOV qword ptr [RBP + 0x260],R14
140378da4:  44 89 B5 68 02 00 00     MOV dword ptr [RBP + 0x268],R14D
140378dab:  F3 0F 10 85 F0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xf0]
140378db3:  41 0F 57 C6              XORPS XMM0,XMM14
140378db7:  0F 57 C9                 XORPS XMM1,XMM1
140378dba:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140378dbe:  45 33 C9                 XOR R9D,R9D
140378dc1:  45 33 C0                 XOR R8D,R8D
140378dc4:  48 8B CE                 MOV RCX,RSI
140378dc7:  E8 54 D6 FB FF           CALL 0x140336420
140378dcc:  F3 0F 10 15 B4 92 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378dd4:  0F 28 C2                 MOVAPS XMM0,XMM2
140378dd7:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378ddc:  0F 28 DF                 MOVAPS XMM3,XMM7
140378ddf:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378de3:  F3 44 0F 10 35 7C 1B 59 00 MOVSS XMM14,dword ptr [0x14090a968]
140378dec:  F3 41 0F 59 D6           MULSS XMM2,XMM14
140378df1:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140378df7:  66 0F 6E 05 8D C4 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378dff:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378e02:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378e06:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378e0b:  48 8D 05 E6 4F 54 00     LEA RAX,[0x1408bddf8]
140378e12:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378e17:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378e1f:  66 41 0F 7E D9           MOVD R9D,XMM3
140378e24:  66 41 0F 7E D0           MOVD R8D,XMM2
140378e29:  49 8B D7                 MOV RDX,R15
140378e2c:  48 8D 0D 7D E1 61 00     LEA RCX,[0x140996fb0]
140378e33:  FF 15 3F 3A 4F 00        CALL qword ptr [0x14086c878]
140378e39:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
140378e3f:  F3 0F 10 35 41 92 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378e47:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140378e4c:  BA 7E 06 00 00           MOV EDX,0x67e
140378e51:  48 8D 0D 38 E7 61 00     LEA RCX,[0x140997590]
140378e58:  FF 15 32 3A 4F 00        CALL qword ptr [0x14086c890]
140378e5e:  0F 28 DF                 MOVAPS XMM3,XMM7
140378e61:  F3 0F 5C DE              SUBSS XMM3,XMM6
140378e65:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140378e6a:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140378e6f:  48 8D 05 C2 53 54 00     LEA RAX,[0x1408be238]
140378e76:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378e7b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378e83:  66 41 0F 7E D9           MOVD R9D,XMM3
140378e88:  0F 28 D6                 MOVAPS XMM2,XMM6
140378e8b:  66 41 0F 7E D0           MOVD R8D,XMM2
140378e90:  49 8B D7                 MOV RDX,R15
140378e93:  48 8D 0D 16 E1 61 00     LEA RCX,[0x140996fb0]
140378e9a:  FF 15 E0 39 4F 00        CALL qword ptr [0x14086c880]
140378ea0:  F3 0F 10 4D C8           MOVSS XMM1,dword ptr [RBP + -0x38]
140378ea5:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140378ea8:  F3 0F 10 15 D8 91 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378eb0:  0F 28 C2                 MOVAPS XMM0,XMM2
140378eb3:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378eb8:  0F 28 DF                 MOVAPS XMM3,XMM7
140378ebb:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378ebf:  F3 41 0F 59 D6           MULSS XMM2,XMM14
140378ec4:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
140378eca:  66 0F 6E 05 BA C3 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378ed2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378ed5:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378ed9:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
140378edf:  48 8D 05 42 53 54 00     LEA RAX,[0x1408be228]
140378ee6:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378eeb:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378ef3:  66 41 0F 7E D9           MOVD R9D,XMM3
140378ef8:  66 41 0F 7E D0           MOVD R8D,XMM2
140378efd:  49 8B D7                 MOV RDX,R15
140378f00:  48 8D 0D A9 E0 61 00     LEA RCX,[0x140996fb0]
140378f07:  FF 15 6B 39 4F 00        CALL qword ptr [0x14086c878]
140378f0d:  F3 0F 10 4D F8           MOVSS XMM1,dword ptr [RBP + -0x8]
140378f12:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140378f15:  F3 0F 10 15 6B 91 61 00  MOVSS XMM2,dword ptr [0x140992088]
140378f1d:  0F 28 C2                 MOVAPS XMM0,XMM2
140378f20:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140378f25:  0F 28 DF                 MOVAPS XMM3,XMM7
140378f28:  F3 0F 5C D8              SUBSS XMM3,XMM0
140378f2c:  F3 41 0F 59 D6           MULSS XMM2,XMM14
140378f31:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140378f37:  66 0F 6E 05 4D C3 61 00  MOVD XMM0,dword ptr [0x14099528c]
140378f3f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140378f42:  F3 0F 5C D0              SUBSS XMM2,XMM0
140378f46:  F2 0F 11 4C 24 30        MOVSD qword ptr [RSP + 0x30],XMM1
140378f4c:  48 8D 05 C5 52 54 00     LEA RAX,[0x1408be218]
140378f53:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140378f58:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140378f60:  66 41 0F 7E D9           MOVD R9D,XMM3
140378f65:  66 41 0F 7E D0           MOVD R8D,XMM2
140378f6a:  49 8B D7                 MOV RDX,R15
140378f6d:  48 8D 0D 3C E0 61 00     LEA RCX,[0x140996fb0]
140378f74:  FF 15 FE 38 4F 00        CALL qword ptr [0x14086c878]
140378f7a:  F3 0F 58 7C 24 78        ADDSS XMM7,dword ptr [RSP + 0x78]
140378f80:  F3 0F 10 35 00 91 61 00  MOVSS XMM6,dword ptr [0x140992088]
140378f88:  0F 28 C6                 MOVAPS XMM0,XMM6
140378f8b:  F3 0F 59 05 95 18 59 00  MULSS XMM0,dword ptr [0x14090a828]
140378f93:  F3 0F 58 F8              ADDSS XMM7,XMM0
140378f97:  0F 28 CE                 MOVAPS XMM1,XMM6
140378f9a:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140378f9f:  0F 28 C7                 MOVAPS XMM0,XMM7
140378fa2:  F3 0F 5C C1              SUBSS XMM0,XMM1
140378fa6:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140378faa:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140378fae:  F3 0F 10 45 BC           MOVSS XMM0,dword ptr [RBP + -0x44]
140378fb3:  F3 0F 58 C7              ADDSS XMM0,XMM7
140378fb7:  F3 0F 58 C1              ADDSS XMM0,XMM1
140378fbb:  F3 0F 2C F8              CVTTSS2SI EDI,XMM0
140378fbf:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140378fc3:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140378fca:  75 4D                    JNZ 0x140379019
140378fcc:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140378fd1:  48 C1 E9 20              SHR RCX,0x20
140378fd5:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140378fda:  48 C1 E8 20              SHR RAX,0x20
140378fde:  03 C8                    ADD ECX,EAX
140378fe0:  66 0F 6E D1              MOVD XMM2,ECX
140378fe4:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140378fe7:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140378fec:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
140378ff0:  41 03 C5                 ADD EAX,R13D
140378ff3:  66 0F 6E C8              MOVD XMM1,EAX
140378ff7:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140378ffa:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140378fff:  41 B9 06 00 00 00        MOV R9D,0x6
140379005:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037900c:  E8 DF D1 0C 00           CALL 0x1404461f0
140379011:  F3 0F 10 35 6F 90 61 00  MOVSS XMM6,dword ptr [0x140992088]
140379019:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037901e:  0F 84 9C 02 00 00        JZ 0x1403792c0
140379024:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
14037902a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037902d:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140379032:  0F 2F C8                 COMISS XMM1,XMM0
140379035:  0F 86 85 02 00 00        JBE 0x1403792c0
14037903b:  66 41 0F 6E C5           MOVD XMM0,R13D
140379040:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379043:  0F 2F C1                 COMISS XMM0,XMM1
140379046:  0F 86 74 02 00 00        JBE 0x1403792c0
14037904c:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140379051:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140379056:  66 0F 6E C3              MOVD XMM0,EBX
14037905a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037905d:  0F 2F C8                 COMISS XMM1,XMM0
140379060:  0F 86 5A 02 00 00        JBE 0x1403792c0
140379066:  66 0F 6E C7              MOVD XMM0,EDI
14037906a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037906d:  0F 2F C1                 COMISS XMM0,XMM1
140379070:  0F 86 4A 02 00 00        JBE 0x1403792c0
140379076:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037907d:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140379080:  48 8D 0D 49 1C 67 00     LEA RCX,[0x1409eacd0]
140379087:  FF 15 63 36 4F 00        CALL qword ptr [0x14086c6f0]
14037908d:  4C 8B C0                 MOV R8,RAX
140379090:  33 D2                    XOR EDX,EDX
140379092:  48 8B CB                 MOV RCX,RBX
140379095:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379098:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037909f:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403790a3:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403790a7:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403790ab:  48 8D 8D F0 07 00 00     LEA RCX,[RBP + 0x7f0]
1403790b2:  FF 15 68 2F 4F 00        CALL qword ptr [0x14086c020]
1403790b8:  48 8B D0                 MOV RDX,RAX
1403790bb:  48 8D 0D 9E 4F 64 00     LEA RCX,[0x1409be060]
1403790c2:  FF 15 D0 37 4F 00        CALL qword ptr [0x14086c898]
1403790c8:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
1403790cd:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
1403790d3:  F3 0F 10 15 AD 8F 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403790db:  F3 0F 59 15 45 18 59 00  MULSS XMM2,dword ptr [0x14090a928]
1403790e3:  F3 44 0F 10 35 8C 0D 59 00 MOVSS XMM14,dword ptr [0x140909e78]
1403790ec:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
1403790f3:  F3 44 0F 11 54 24 30     MOVSS dword ptr [RSP + 0x30],XMM10
1403790fa:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
140379101:  F3 44 0F 11 54 24 20     MOVSS dword ptr [RSP + 0x20],XMM10
140379108:  0F 28 DA                 MOVAPS XMM3,XMM2
14037910b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140379110:  48 8D 0D 49 4F 64 00     LEA RCX,[0x1409be060]
140379117:  FF 15 3B 37 4F 00        CALL qword ptr [0x14086c858]
14037911d:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
140379122:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
140379127:  4C 8D 05 C2 50 54 00     LEA R8,[0x1408be1f0]
14037912e:  83 CA FF                 OR EDX,0xffffffff
140379131:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
140379136:  E8 E5 1B EE FF           CALL 0x14025ad20
14037913b:  90                       NOP
14037913c:  45 8B D6                 MOV R10D,R14D
14037913f:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140379143:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
140379147:  4C 2B DF                 SUB R11,RDI
14037914a:  49 C1 FB 04              SAR R11,0x4
14037914e:  45 85 DB                 TEST R11D,R11D
140379151:  74 70                    JZ 0x1403791c3
140379153:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
140379158:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037915d:  0F 1F 00                 NOP dword ptr [RAX]
140379160:  41 8B C2                 MOV EAX,R10D
140379163:  48 03 C0                 ADD RAX,RAX
140379166:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037916a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037916e:  85 D2                    TEST EDX,EDX
140379170:  79 17                    JNS 0x140379189
140379172:  48 85 C9                 TEST RCX,RCX
140379175:  74 44                    JZ 0x1403791bb
140379177:  48 83 CA FF              OR RDX,-0x1
14037917b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140379180:  48 FF C2                 INC RDX
140379183:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140379187:  75 F7                    JNZ 0x140379180
140379189:  48 85 C9                 TEST RCX,RCX
14037918c:  74 2D                    JZ 0x1403791bb
14037918e:  41 3B D4                 CMP EDX,R12D
140379191:  75 28                    JNZ 0x1403791bb
140379193:  4C 63 C2                 MOVSXD R8,EDX
140379196:  85 D2                    TEST EDX,EDX
140379198:  7E 3C                    JLE 0x1403791d6
14037919a:  48 8B D1                 MOV RDX,RCX
14037919d:  48 F7 DA                 NEG RDX
1403791a0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
1403791a4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
1403791a7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
1403791ab:  75 0E                    JNZ 0x1403791bb
1403791ad:  48 FF C1                 INC RCX
1403791b0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
1403791b4:  49 3B C0                 CMP RAX,R8
1403791b7:  7D 1D                    JGE 0x1403791d6
1403791b9:  EB E9                    JMP 0x1403791a4
1403791bb:  41 FF C2                 INC R10D
1403791be:  45 3B D3                 CMP R10D,R11D
1403791c1:  72 9D                    JC 0x140379160
1403791c3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403791c8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
1403791cc:  E8 1F 65 0D 00           CALL 0x14044f6f0
1403791d1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
1403791d6:  48 85 DB                 TEST RBX,RBX
1403791d9:  74 08                    JZ 0x1403791e3
1403791db:  48 8B CB                 MOV RCX,RBX
1403791de:  E8 FD 4D 4D 00           CALL 0x14084dfe0
1403791e3:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
1403791e8:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
1403791ed:  4C 8D 05 54 50 54 00     LEA R8,[0x1408be248]
1403791f4:  83 CA FF                 OR EDX,0xffffffff
1403791f7:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
1403791fc:  E8 1F 1B EE FF           CALL 0x14025ad20
140379201:  90                       NOP
140379202:  45 8B D6                 MOV R10D,R14D
140379205:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
140379209:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037920d:  4C 2B DF                 SUB R11,RDI
140379210:  49 C1 FB 04              SAR R11,0x4
140379214:  45 85 DB                 TEST R11D,R11D
140379217:  74 7A                    JZ 0x140379293
140379219:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037921e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
140379223:  0F 1F 40 00              NOP dword ptr [RAX]
140379227:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140379230:  41 8B C2                 MOV EAX,R10D
140379233:  48 03 C0                 ADD RAX,RAX
140379236:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037923a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037923e:  85 D2                    TEST EDX,EDX
140379240:  79 17                    JNS 0x140379259
140379242:  48 85 C9                 TEST RCX,RCX
140379245:  74 44                    JZ 0x14037928b
140379247:  48 83 CA FF              OR RDX,-0x1
14037924b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140379250:  48 FF C2                 INC RDX
140379253:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
140379257:  75 F7                    JNZ 0x140379250
140379259:  48 85 C9                 TEST RCX,RCX
14037925c:  74 2D                    JZ 0x14037928b
14037925e:  41 3B D4                 CMP EDX,R12D
140379261:  75 28                    JNZ 0x14037928b
140379263:  4C 63 C2                 MOVSXD R8,EDX
140379266:  85 D2                    TEST EDX,EDX
140379268:  7E 3C                    JLE 0x1403792a6
14037926a:  48 8B D1                 MOV RDX,RCX
14037926d:  48 F7 DA                 NEG RDX
140379270:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
140379274:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
140379277:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037927b:  75 0E                    JNZ 0x14037928b
14037927d:  48 FF C1                 INC RCX
140379280:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
140379284:  49 3B C0                 CMP RAX,R8
140379287:  7D 1D                    JGE 0x1403792a6
140379289:  EB E9                    JMP 0x140379274
14037928b:  41 FF C2                 INC R10D
14037928e:  45 3B D3                 CMP R10D,R11D
140379291:  72 9D                    JC 0x140379230
140379293:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140379298:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037929c:  E8 4F 64 0D 00           CALL 0x14044f6f0
1403792a1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
1403792a6:  48 85 DB                 TEST RBX,RBX
1403792a9:  74 08                    JZ 0x1403792b3
1403792ab:  48 8B CB                 MOV RCX,RBX
1403792ae:  E8 2D 4D 4D 00           CALL 0x14084dfe0
1403792b3:  41 B4 01                 MOV R12B,0x1
1403792b6:  F3 0F 10 35 CA 8D 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403792be:  EB 09                    JMP 0x1403792c9
1403792c0:  F3 44 0F 10 35 AF 0B 59 00 MOVSS XMM14,dword ptr [0x140909e78]
1403792c9:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403792ce:  BA B2 92 00 00           MOV EDX,0x92b2
1403792d3:  48 8D 0D B6 E2 61 00     LEA RCX,[0x140997590]
1403792da:  FF 15 B0 35 4F 00        CALL qword ptr [0x14086c890]
1403792e0:  0F 28 DF                 MOVAPS XMM3,XMM7
1403792e3:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403792e7:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
1403792ec:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403792f1:  48 8D 05 18 51 54 00     LEA RAX,[0x1408be410]
1403792f8:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403792fd:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140379305:  66 41 0F 7E D9           MOVD R9D,XMM3
14037930a:  0F 28 D6                 MOVAPS XMM2,XMM6
14037930d:  66 41 0F 7E D0           MOVD R8D,XMM2
140379312:  49 8B D7                 MOV RDX,R15
140379315:  48 8D 0D 94 DC 61 00     LEA RCX,[0x140996fb0]
14037931c:  FF 15 5E 35 4F 00        CALL qword ptr [0x14086c880]
140379322:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
140379329:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037932c:  48 8D 0D 9D 19 67 00     LEA RCX,[0x1409eacd0]
140379333:  FF 15 B7 33 4F 00        CALL qword ptr [0x14086c6f0]
140379339:  4C 8B C0                 MOV R8,RAX
14037933c:  33 D2                    XOR EDX,EDX
14037933e:  48 8B CB                 MOV RCX,RBX
140379341:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379344:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037934b:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037934f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379353:  41 0F 28 C9              MOVAPS XMM1,XMM9
140379357:  48 8D 8D 00 08 00 00     LEA RCX,[RBP + 0x800]
14037935e:  FF 15 BC 2C 4F 00        CALL qword ptr [0x14086c020]
140379364:  48 8B D0                 MOV RDX,RAX
140379367:  48 8D 0D F2 4C 64 00     LEA RCX,[0x1409be060]
14037936e:  FF 15 24 35 4F 00        CALL qword ptr [0x14086c898]
140379374:  F3 0F 10 0D 0C 8D 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037937c:  F3 0F 59 0D 88 16 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140379384:  0F 28 D1                 MOVAPS XMM2,XMM1
140379387:  48 8D 0D D2 4C 64 00     LEA RCX,[0x1409be060]
14037938e:  FF 15 14 35 4F 00        CALL qword ptr [0x14086c8a8]
140379394:  F3 0F 10 15 EC 8C 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037939c:  F3 0F 10 35 84 14 59 00  MOVSS XMM6,dword ptr [0x14090a828]
1403793a4:  F3 0F 59 D6              MULSS XMM2,XMM6
1403793a8:  F3 0F 58 D7              ADDSS XMM2,XMM7
1403793ac:  41 0F 28 D8              MOVAPS XMM3,XMM8
1403793b0:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
1403793b6:  48 8D 8D E8 03 00 00     LEA RCX,[RBP + 0x3e8]
1403793bd:  FF 15 45 2C 4F 00        CALL qword ptr [0x14086c008]
1403793c3:  48 8B D0                 MOV RDX,RAX
1403793c6:  48 8D 0D 93 4C 64 00     LEA RCX,[0x1409be060]
1403793cd:  FF 15 CD 34 4F 00        CALL qword ptr [0x14086c8a0]
1403793d3:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
1403793d8:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
1403793df:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403793e6:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403793ea:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403793ee:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403793f2:  48 8D 0D 67 4C 64 00     LEA RCX,[0x1409be060]
1403793f9:  FF 15 61 34 4F 00        CALL qword ptr [0x14086c860]
1403793ff:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
140379406:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140379409:  48 8D 0D C0 18 67 00     LEA RCX,[0x1409eacd0]
140379410:  FF 15 DA 32 4F 00        CALL qword ptr [0x14086c6f0]
140379416:  4C 8B C0                 MOV R8,RAX
140379419:  33 D2                    XOR EDX,EDX
14037941b:  48 8B CB                 MOV RCX,RBX
14037941e:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379421:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140379428:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037942c:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379430:  41 0F 28 C9              MOVAPS XMM1,XMM9
140379434:  48 8D 8D 10 08 00 00     LEA RCX,[RBP + 0x810]
14037943b:  FF 15 DF 2B 4F 00        CALL qword ptr [0x14086c020]
140379441:  48 8B D0                 MOV RDX,RAX
140379444:  48 8D 0D 15 4C 64 00     LEA RCX,[0x1409be060]
14037944b:  FF 15 47 34 4F 00        CALL qword ptr [0x14086c898]
140379451:  F3 0F 10 0D 2F 8C 61 00  MOVSS XMM1,dword ptr [0x140992088]
140379459:  F3 0F 59 0D AB 15 59 00  MULSS XMM1,dword ptr [0x14090aa0c]
140379461:  0F 28 D1                 MOVAPS XMM2,XMM1
140379464:  48 8D 0D F5 4B 64 00     LEA RCX,[0x1409be060]
14037946b:  FF 15 37 34 4F 00        CALL qword ptr [0x14086c8a8]
140379471:  F3 0F 10 15 0F 8C 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379479:  F3 0F 59 D6              MULSS XMM2,XMM6
14037947d:  F3 0F 58 D7              ADDSS XMM2,XMM7
140379481:  41 0F 28 D8              MOVAPS XMM3,XMM8
140379485:  F3 0F 10 4C 24 70        MOVSS XMM1,dword ptr [RSP + 0x70]
14037948b:  48 8D 8D F4 03 00 00     LEA RCX,[RBP + 0x3f4]
140379492:  FF 15 70 2B 4F 00        CALL qword ptr [0x14086c008]
140379498:  48 8B D0                 MOV RDX,RAX
14037949b:  48 8D 0D BE 4B 64 00     LEA RCX,[0x1409be060]
1403794a2:  FF 15 F8 33 4F 00        CALL qword ptr [0x14086c8a0]
1403794a8:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
1403794ad:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
1403794b4:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403794bb:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403794bf:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403794c3:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403794c7:  48 8D 0D 92 4B 64 00     LEA RCX,[0x1409be060]
1403794ce:  FF 15 8C 33 4F 00        CALL qword ptr [0x14086c860]
1403794d4:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
1403794d9:  8B 05 91 BD 61 00        MOV EAX,dword ptr [0x140995270]
1403794df:  83 F8 02                 CMP EAX,0x2
1403794e2:  75 09                    JNZ 0x1403794ed
1403794e4:  48 8D 05 85 4E 54 00     LEA RAX,[0x1408be370]
1403794eb:  EB 13                    JMP 0x140379500
1403794ed:  83 F8 01                 CMP EAX,0x1
1403794f0:  48 8D 05 79 4D 54 00     LEA RAX,[0x1408be270]
1403794f7:  74 07                    JZ 0x140379500
1403794f9:  48 8D 05 60 4B 54 00     LEA RAX,[0x1408be060]
140379500:  F3 0F 10 15 80 8B 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379508:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037950d:  0F 28 DA                 MOVAPS XMM3,XMM2
140379510:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379515:  F3 0F 58 DF              ADDSS XMM3,XMM7
140379519:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
14037951e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140379526:  66 41 0F 7E D9           MOVD R9D,XMM3
14037952b:  66 41 0F 7E D0           MOVD R8D,XMM2
140379530:  49 8B D7                 MOV RDX,R15
140379533:  48 8D 0D 76 DA 61 00     LEA RCX,[0x140996fb0]
14037953a:  FF 15 40 33 4F 00        CALL qword ptr [0x14086c880]
140379540:  F3 41 0F 58 FD           ADDSS XMM7,XMM13
140379545:  F3 0F 10 0D 3B 8B 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037954d:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140379552:  0F 28 C7                 MOVAPS XMM0,XMM7
140379555:  F3 0F 5C C1              SUBSS XMM0,XMM1
140379559:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037955d:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140379561:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
140379568:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037956d:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140379572:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140379576:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037957a:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140379581:  75 45                    JNZ 0x1403795c8
140379583:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140379588:  48 C1 E9 20              SHR RCX,0x20
14037958c:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140379591:  48 C1 E8 20              SHR RAX,0x20
140379595:  03 C8                    ADD ECX,EAX
140379597:  66 0F 6E D1              MOVD XMM2,ECX
14037959b:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037959e:  F3 41 0F 59 D3           MULSS XMM2,XMM11
1403795a3:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
1403795a7:  41 03 C5                 ADD EAX,R13D
1403795aa:  66 0F 6E C8              MOVD XMM1,EAX
1403795ae:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1403795b1:  F3 41 0F 59 CB           MULSS XMM1,XMM11
1403795b6:  41 B9 06 00 00 00        MOV R9D,0x6
1403795bc:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
1403795c3:  E8 28 CC 0C 00           CALL 0x1404461f0
1403795c8:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
1403795cd:  0F 84 41 01 00 00        JZ 0x140379714
1403795d3:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
1403795d9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403795dc:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
1403795e1:  0F 2F C8                 COMISS XMM1,XMM0
1403795e4:  0F 86 2A 01 00 00        JBE 0x140379714
1403795ea:  66 41 0F 6E C5           MOVD XMM0,R13D
1403795ef:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403795f2:  0F 2F C1                 COMISS XMM0,XMM1
1403795f5:  0F 86 19 01 00 00        JBE 0x140379714
1403795fb:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140379600:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140379605:  66 0F 6E C3              MOVD XMM0,EBX
140379609:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037960c:  0F 2F C8                 COMISS XMM1,XMM0
14037960f:  0F 86 FF 00 00 00        JBE 0x140379714
140379615:  66 0F 6E C7              MOVD XMM0,EDI
140379619:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037961c:  0F 2F C1                 COMISS XMM0,XMM1
14037961f:  0F 86 EF 00 00 00        JBE 0x140379714
140379625:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037962c:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037962f:  48 8D 0D 9A 16 67 00     LEA RCX,[0x1409eacd0]
140379636:  FF 15 B4 30 4F 00        CALL qword ptr [0x14086c6f0]
14037963c:  4C 8B C0                 MOV R8,RAX
14037963f:  33 D2                    XOR EDX,EDX
140379641:  48 8B CB                 MOV RCX,RBX
140379644:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379647:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037964e:  41 0F 28 D9              MOVAPS XMM3,XMM9
140379652:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379656:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037965a:  48 8D 8D 20 08 00 00     LEA RCX,[RBP + 0x820]
140379661:  FF 15 B9 29 4F 00        CALL qword ptr [0x14086c020]
140379667:  48 8B D0                 MOV RDX,RAX
14037966a:  48 8D 0D EF 49 64 00     LEA RCX,[0x1409be060]
140379671:  FF 15 21 32 4F 00        CALL qword ptr [0x14086c898]
140379677:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037967b:  48 8D 0D DE 49 64 00     LEA RCX,[0x1409be060]
140379682:  FF 15 D0 32 4F 00        CALL qword ptr [0x14086c958]
140379688:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037968d:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140379693:  F3 0F 10 15 ED 89 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037969b:  F3 0F 59 D6              MULSS XMM2,XMM6
14037969f:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
1403796a6:  F3 0F 10 05 56 06 59 00  MOVSS XMM0,dword ptr [0x140909d04]
1403796ae:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
1403796b4:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
1403796bb:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
1403796c1:  0F 28 DA                 MOVAPS XMM3,XMM2
1403796c4:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1403796c9:  48 8D 0D 90 49 64 00     LEA RCX,[0x1409be060]
1403796d0:  FF 15 82 31 4F 00        CALL qword ptr [0x14086c858]
1403796d6:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403796da:  48 8D 0D 7F 49 64 00     LEA RCX,[0x1409be060]
1403796e1:  FF 15 71 32 4F 00        CALL qword ptr [0x14086c958]
1403796e7:  48 8D 0D A2 B4 6D 00     LEA RCX,[0x140a54b90]
1403796ee:  FF 15 C4 31 4F 00        CALL qword ptr [0x14086c8b8]
1403796f4:  84 C0                    TEST AL,AL
1403796f6:  74 1C                    JZ 0x140379714
1403796f8:  41 B0 01                 MOV R8B,0x1
1403796fb:  48 8D 15 4E 49 54 00     LEA RDX,[0x1408be050]
140379702:  48 8B CE                 MOV RCX,RSI
140379705:  E8 C6 63 08 00           CALL 0x1403ffad0
14037970a:  C7 86 D4 C7 00 00 09 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x9
140379714:  F3 0F 10 35 6C 89 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037971c:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140379721:  BA B3 92 00 00           MOV EDX,0x92b3
140379726:  48 8D 0D 63 DE 61 00     LEA RCX,[0x140997590]
14037972d:  FF 15 5D 31 4F 00        CALL qword ptr [0x14086c890]
140379733:  0F 28 DF                 MOVAPS XMM3,XMM7
140379736:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037973a:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037973f:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379744:  48 8D 05 F5 48 54 00     LEA RAX,[0x1408be040]
14037974b:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379750:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140379758:  66 41 0F 7E D9           MOVD R9D,XMM3
14037975d:  0F 28 D6                 MOVAPS XMM2,XMM6
140379760:  66 41 0F 7E D0           MOVD R8D,XMM2
140379765:  49 8B D7                 MOV RDX,R15
140379768:  48 8D 0D 41 D8 61 00     LEA RCX,[0x140996fb0]
14037976f:  FF 15 0B 31 4F 00        CALL qword ptr [0x14086c880]
140379775:  F3 0F 10 8D 5C 01 00 00  MOVSS XMM1,dword ptr [RBP + 0x15c]
14037977d:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140379780:  45 33 C9                 XOR R9D,R9D
140379783:  45 33 C0                 XOR R8D,R8D
140379786:  48 8B CE                 MOV RCX,RSI
140379789:  E8 92 CC FB FF           CALL 0x140336420
14037978e:  F3 0F 10 15 F2 88 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379796:  0F 28 C2                 MOVAPS XMM0,XMM2
140379799:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037979e:  F3 0F 5C F8              SUBSS XMM7,XMM0
1403797a2:  F3 0F 59 15 BE 11 59 00  MULSS XMM2,dword ptr [0x14090a968]
1403797aa:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
1403797b0:  66 0F 6E 05 D4 BA 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403797b8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403797bb:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403797bf:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403797c4:  48 8D 05 6D 48 54 00     LEA RAX,[0x1408be038]
1403797cb:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403797d0:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403797d8:  0F 28 DF                 MOVAPS XMM3,XMM7
1403797db:  66 41 0F 7E D9           MOVD R9D,XMM3
1403797e0:  66 41 0F 7E D0           MOVD R8D,XMM2
1403797e5:  49 8B D7                 MOV RDX,R15
1403797e8:  48 8D 0D C1 D7 61 00     LEA RCX,[0x140996fb0]
1403797ef:  FF 15 83 30 4F 00        CALL qword ptr [0x14086c878]
1403797f5:  F3 0F 10 0D 8B 88 61 00  MOVSS XMM1,dword ptr [0x140992088]
1403797fd:  F3 41 0F 59 C8           MULSS XMM1,XMM8
140379802:  41 0F 28 C2              MOVAPS XMM0,XMM10
140379806:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037980a:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037980e:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140379812:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
140379818:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037981d:  F3 0F 58 CF              ADDSS XMM1,XMM7
140379821:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140379825:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140379829:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140379830:  75 45                    JNZ 0x140379877
140379832:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140379837:  48 C1 E9 20              SHR RCX,0x20
14037983b:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140379840:  48 C1 E8 20              SHR RAX,0x20
140379844:  03 C8                    ADD ECX,EAX
140379846:  66 0F 6E D1              MOVD XMM2,ECX
14037984a:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037984d:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140379852:  8B 44 24 60              MOV EAX,dword ptr [RSP + 0x60]
140379856:  41 03 C5                 ADD EAX,R13D
140379859:  66 0F 6E C8              MOVD XMM1,EAX
14037985d:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140379860:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140379865:  41 B9 06 00 00 00        MOV R9D,0x6
14037986b:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140379872:  E8 79 C9 0C 00           CALL 0x1404461f0
140379877:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037987c:  0F 84 45 01 00 00        JZ 0x1403799c7
140379882:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140379888:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037988b:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140379890:  0F 2F C8                 COMISS XMM1,XMM0
140379893:  0F 86 2E 01 00 00        JBE 0x1403799c7
140379899:  66 41 0F 6E C5           MOVD XMM0,R13D
14037989e:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403798a1:  0F 2F C1                 COMISS XMM0,XMM1
1403798a4:  0F 86 1D 01 00 00        JBE 0x1403799c7
1403798aa:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
1403798af:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
1403798b4:  66 0F 6E C3              MOVD XMM0,EBX
1403798b8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403798bb:  0F 2F C8                 COMISS XMM1,XMM0
1403798be:  0F 86 03 01 00 00        JBE 0x1403799c7
1403798c4:  66 0F 6E C7              MOVD XMM0,EDI
1403798c8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403798cb:  0F 2F C1                 COMISS XMM0,XMM1
1403798ce:  0F 86 F3 00 00 00        JBE 0x1403799c7
1403798d4:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
1403798db:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403798de:  48 8D 0D EB 13 67 00     LEA RCX,[0x1409eacd0]
1403798e5:  FF 15 05 2E 4F 00        CALL qword ptr [0x14086c6f0]
1403798eb:  4C 8B C0                 MOV R8,RAX
1403798ee:  33 D2                    XOR EDX,EDX
1403798f0:  48 8B CB                 MOV RCX,RBX
1403798f3:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403798f6:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
1403798fd:  41 0F 28 D9              MOVAPS XMM3,XMM9
140379901:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379905:  41 0F 28 C9              MOVAPS XMM1,XMM9
140379909:  48 8D 8D 30 08 00 00     LEA RCX,[RBP + 0x830]
140379910:  FF 15 0A 27 4F 00        CALL qword ptr [0x14086c020]
140379916:  48 8B D0                 MOV RDX,RAX
140379919:  48 8D 0D 40 47 64 00     LEA RCX,[0x1409be060]
140379920:  FF 15 72 2F 4F 00        CALL qword ptr [0x14086c898]
140379926:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037992a:  48 8D 0D 2F 47 64 00     LEA RCX,[0x1409be060]
140379931:  FF 15 21 30 4F 00        CALL qword ptr [0x14086c958]
140379937:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037993c:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140379942:  F3 0F 10 15 3E 87 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037994a:  F3 0F 59 15 D6 0E 59 00  MULSS XMM2,dword ptr [0x14090a828]
140379952:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
140379959:  F3 0F 10 05 A3 03 59 00  MOVSS XMM0,dword ptr [0x140909d04]
140379961:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
140379967:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
14037996e:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
140379974:  0F 28 DA                 MOVAPS XMM3,XMM2
140379977:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037997c:  48 8D 0D DD 46 64 00     LEA RCX,[0x1409be060]
140379983:  FF 15 CF 2E 4F 00        CALL qword ptr [0x14086c858]
140379989:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037998d:  48 8D 0D CC 46 64 00     LEA RCX,[0x1409be060]
140379994:  FF 15 BE 2F 4F 00        CALL qword ptr [0x14086c958]
14037999a:  48 8D 0D EF B1 6D 00     LEA RCX,[0x140a54b90]
1403799a1:  FF 15 11 2F 4F 00        CALL qword ptr [0x14086c8b8]
1403799a7:  84 C0                    TEST AL,AL
1403799a9:  74 1C                    JZ 0x1403799c7
1403799ab:  41 B0 01                 MOV R8B,0x1
1403799ae:  48 8D 15 2B 48 54 00     LEA RDX,[0x1408be1e0]
1403799b5:  48 8B CE                 MOV RCX,RSI
1403799b8:  E8 13 61 08 00           CALL 0x1403ffad0
1403799bd:  C7 86 D4 C7 00 00 0A 00 00 00 MOV dword ptr [RSI + 0xc7d4],0xa
1403799c7:  F3 0F 10 35 B9 86 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403799cf:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403799d4:  BA B4 92 00 00           MOV EDX,0x92b4
1403799d9:  48 8D 0D B0 DB 61 00     LEA RCX,[0x140997590]
1403799e0:  FF 15 AA 2E 4F 00        CALL qword ptr [0x14086c890]
1403799e6:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403799ea:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403799ee:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
1403799f3:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403799f8:  48 8D 05 D1 47 54 00     LEA RAX,[0x1408be1d0]
1403799ff:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379a04:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140379a0c:  66 41 0F 7E D9           MOVD R9D,XMM3
140379a11:  0F 28 D6                 MOVAPS XMM2,XMM6
140379a14:  66 41 0F 7E D0           MOVD R8D,XMM2
140379a19:  49 8B D7                 MOV RDX,R15
140379a1c:  48 8D 0D 8D D5 61 00     LEA RCX,[0x140996fb0]
140379a23:  FF 15 57 2E 4F 00        CALL qword ptr [0x14086c880]
140379a29:  F3 0F 10 8D 60 01 00 00  MOVSS XMM1,dword ptr [RBP + 0x160]
140379a31:  0F 5A C9                 CVTPS2PD XMM1,XMM1
140379a34:  45 33 C9                 XOR R9D,R9D
140379a37:  45 33 C0                 XOR R8D,R8D
140379a3a:  48 8B CE                 MOV RCX,RSI
140379a3d:  E8 DE C9 FB FF           CALL 0x140336420
140379a42:  F3 0F 10 15 3E 86 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379a4a:  0F 28 C2                 MOVAPS XMM0,XMM2
140379a4d:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140379a52:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
140379a57:  F3 0F 59 15 09 0F 59 00  MULSS XMM2,dword ptr [0x14090a968]
140379a5f:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140379a65:  66 0F 6E 05 1F B8 61 00  MOVD XMM0,dword ptr [0x14099528c]
140379a6d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379a70:  F3 0F 5C D0              SUBSS XMM2,XMM0
140379a74:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379a79:  48 8D 05 A8 46 54 00     LEA RAX,[0x1408be128]
140379a80:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379a85:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140379a8d:  41 0F 28 DA              MOVAPS XMM3,XMM10
140379a91:  66 41 0F 7E D9           MOVD R9D,XMM3
140379a96:  66 41 0F 7E D0           MOVD R8D,XMM2
140379a9b:  49 8B D7                 MOV RDX,R15
140379a9e:  48 8D 0D 0B D5 61 00     LEA RCX,[0x140996fb0]
140379aa5:  FF 15 CD 2D 4F 00        CALL qword ptr [0x14086c878]
140379aab:  F3 0F 10 15 D5 85 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379ab3:  0F 28 F2                 MOVAPS XMM6,XMM2
140379ab6:  F3 0F 59 35 6A 0D 59 00  MULSS XMM6,dword ptr [0x14090a828]
140379abe:  F3 0F 58 F7              ADDSS XMM6,XMM7
140379ac2:  8B 05 A8 B7 61 00        MOV EAX,dword ptr [0x140995270]
140379ac8:  83 F8 02                 CMP EAX,0x2
140379acb:  75 09                    JNZ 0x140379ad6
140379acd:  48 8D 05 5C 46 54 00     LEA RAX,[0x1408be130]
140379ad4:  EB 13                    JMP 0x140379ae9
140379ad6:  83 F8 01                 CMP EAX,0x1
140379ad9:  48 8D 05 90 4A 54 00     LEA RAX,[0x1408be570]
140379ae0:  74 07                    JZ 0x140379ae9
140379ae2:  48 8D 05 B7 49 54 00     LEA RAX,[0x1408be4a0]
140379ae9:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140379aee:  0F 28 DA                 MOVAPS XMM3,XMM2
140379af1:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379af6:  F3 0F 58 DE              ADDSS XMM3,XMM6
140379afa:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
140379aff:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140379b07:  66 41 0F 7E D9           MOVD R9D,XMM3
140379b0c:  66 41 0F 7E D0           MOVD R8D,XMM2
140379b11:  49 8B D7                 MOV RDX,R15
140379b14:  48 8D 0D 95 D4 61 00     LEA RCX,[0x140996fb0]
140379b1b:  FF 15 5F 2D 4F 00        CALL qword ptr [0x14086c880]
140379b21:  45 32 ED                 XOR R13B,R13B
140379b24:  F3 41 0F 58 F5           ADDSS XMM6,XMM13
140379b29:  F3 44 0F 10 15 56 85 61 00 MOVSS XMM10,dword ptr [0x140992088]
140379b32:  41 0F 28 C2              MOVAPS XMM0,XMM10
140379b36:  F3 41 0F 59 C4           MULSS XMM0,XMM12
140379b3b:  F3 0F 58 C6              ADDSS XMM0,XMM6
140379b3f:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140379b43:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140379b47:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
140379b4d:  F3 0F 58 FE              ADDSS XMM7,XMM6
140379b51:  41 0F 28 C2              MOVAPS XMM0,XMM10
140379b55:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140379b5a:  F3 0F 58 C7              ADDSS XMM0,XMM7
140379b5e:  F3 0F 2C F8              CVTTSS2SI EDI,XMM0
140379b62:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140379b66:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140379b6d:  75 4F                    JNZ 0x140379bbe
140379b6f:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140379b74:  48 C1 E9 20              SHR RCX,0x20
140379b78:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140379b7d:  48 C1 E8 20              SHR RAX,0x20
140379b81:  03 C8                    ADD ECX,EAX
140379b83:  66 0F 6E D1              MOVD XMM2,ECX
140379b87:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140379b8a:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140379b8f:  8B 44 24 68              MOV EAX,dword ptr [RSP + 0x68]
140379b93:  03 44 24 60              ADD EAX,dword ptr [RSP + 0x60]
140379b97:  66 0F 6E C8              MOVD XMM1,EAX
140379b9b:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140379b9e:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140379ba3:  41 B9 06 00 00 00        MOV R9D,0x6
140379ba9:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140379bb0:  E8 3B C6 0C 00           CALL 0x1404461f0
140379bb5:  F3 44 0F 10 15 CA 84 61 00 MOVSS XMM10,dword ptr [0x140992088]
140379bbe:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140379bc3:  0F 84 03 01 00 00        JZ 0x140379ccc
140379bc9:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140379bcf:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379bd2:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140379bd7:  0F 2F C8                 COMISS XMM1,XMM0
140379bda:  0F 86 EC 00 00 00        JBE 0x140379ccc
140379be0:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
140379be6:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379be9:  0F 2F C1                 COMISS XMM0,XMM1
140379bec:  0F 86 DA 00 00 00        JBE 0x140379ccc
140379bf2:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140379bf7:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140379bfc:  66 0F 6E C3              MOVD XMM0,EBX
140379c00:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379c03:  0F 2F C8                 COMISS XMM1,XMM0
140379c06:  0F 86 C0 00 00 00        JBE 0x140379ccc
140379c0c:  66 0F 6E C7              MOVD XMM0,EDI
140379c10:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379c13:  0F 2F C1                 COMISS XMM0,XMM1
140379c16:  0F 86 B0 00 00 00        JBE 0x140379ccc
140379c1c:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140379c23:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140379c26:  48 8D 0D A3 10 67 00     LEA RCX,[0x1409eacd0]
140379c2d:  FF 15 BD 2A 4F 00        CALL qword ptr [0x14086c6f0]
140379c33:  4C 8B C0                 MOV R8,RAX
140379c36:  33 D2                    XOR EDX,EDX
140379c38:  48 8B CB                 MOV RCX,RBX
140379c3b:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379c3e:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140379c45:  41 0F 28 D9              MOVAPS XMM3,XMM9
140379c49:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379c4d:  41 0F 28 C9              MOVAPS XMM1,XMM9
140379c51:  48 8D 8D 40 08 00 00     LEA RCX,[RBP + 0x840]
140379c58:  FF 15 C2 23 4F 00        CALL qword ptr [0x14086c020]
140379c5e:  48 8B D0                 MOV RDX,RAX
140379c61:  48 8D 0D F8 43 64 00     LEA RCX,[0x1409be060]
140379c68:  FF 15 2A 2C 4F 00        CALL qword ptr [0x14086c898]
140379c6e:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140379c73:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140379c79:  F3 0F 10 15 07 84 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379c81:  F3 0F 59 15 9F 0C 59 00  MULSS XMM2,dword ptr [0x14090a928]
140379c89:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
140379c90:  F3 0F 10 05 6C 00 59 00  MOVSS XMM0,dword ptr [0x140909d04]
140379c98:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
140379c9e:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
140379ca5:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
140379cab:  0F 28 DA                 MOVAPS XMM3,XMM2
140379cae:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
140379cb3:  48 8D 0D A6 43 64 00     LEA RCX,[0x1409be060]
140379cba:  FF 15 98 2B 4F 00        CALL qword ptr [0x14086c858]
140379cc0:  41 B5 01                 MOV R13B,0x1
140379cc3:  F3 44 0F 10 15 BC 83 61 00 MOVSS XMM10,dword ptr [0x140992088]
140379ccc:  F3 45 0F 59 D0           MULSS XMM10,XMM8
140379cd1:  BA 1A DF 00 00           MOV EDX,0xdf1a
140379cd6:  48 8D 0D B3 D8 61 00     LEA RCX,[0x140997590]
140379cdd:  FF 15 AD 2B 4F 00        CALL qword ptr [0x14086c890]
140379ce3:  0F 28 DE                 MOVAPS XMM3,XMM6
140379ce6:  F3 41 0F 5C DA           SUBSS XMM3,XMM10
140379ceb:  F3 45 0F 58 D7           ADDSS XMM10,XMM15
140379cf0:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379cf5:  48 8D 05 94 47 54 00     LEA RAX,[0x1408be490]
140379cfc:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140379d01:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140379d09:  66 41 0F 7E D9           MOVD R9D,XMM3
140379d0e:  41 0F 28 D2              MOVAPS XMM2,XMM10
140379d12:  66 41 0F 7E D0           MOVD R8D,XMM2
140379d17:  49 8B D7                 MOV RDX,R15
140379d1a:  48 8D 0D 8F D2 61 00     LEA RCX,[0x140996fb0]
140379d21:  FF 15 59 2B 4F 00        CALL qword ptr [0x14086c880]
140379d27:  F3 0F 10 85 A4 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xa4]
140379d2f:  0F 5A C8                 CVTPS2PD XMM1,XMM0
140379d32:  45 33 C9                 XOR R9D,R9D
140379d35:  45 33 C0                 XOR R8D,R8D
140379d38:  48 8B CE                 MOV RCX,RSI
140379d3b:  44 0F 2F C0              COMISS XMM8,XMM0
140379d3f:  72 2E                    JC 0x140379d6f
140379d41:  E8 DA C6 FB FF           CALL 0x140336420
140379d46:  48 8D 15 3B 47 54 00     LEA RDX,[0x1408be488]
140379d4d:  48 8D 0D 34 49 54 00     LEA RCX,[0x1408be688]
140379d54:  45 84 ED                 TEST R13B,R13B
140379d57:  48 0F 45 CA              CMOVNZ RCX,RDX
140379d5b:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379d60:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
140379d65:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140379d6d:  EB 2C                    JMP 0x140379d9b
140379d6f:  E8 AC C6 FB FF           CALL 0x140336420
140379d74:  48 8D 15 FD 48 54 00     LEA RDX,[0x1408be678]
140379d7b:  48 8D 0D E6 48 54 00     LEA RCX,[0x1408be668]
140379d82:  45 84 ED                 TEST R13B,R13B
140379d85:  48 0F 45 CA              CMOVNZ RCX,RDX
140379d89:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379d8e:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
140379d93:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
140379d9b:  F3 0F 10 15 E5 82 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379da3:  0F 28 C2                 MOVAPS XMM0,XMM2
140379da6:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140379dab:  0F 28 DE                 MOVAPS XMM3,XMM6
140379dae:  F3 0F 5C D8              SUBSS XMM3,XMM0
140379db2:  66 0F 6E 05 D2 B4 61 00  MOVD XMM0,dword ptr [0x14099528c]
140379dba:  F3 44 0F 10 54 24 74     MOVSS XMM10,dword ptr [RSP + 0x74]
140379dc1:  F3 0F 59 15 9F 0B 59 00  MULSS XMM2,dword ptr [0x14090a968]
140379dc9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379dcc:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140379dd1:  F3 0F 5C D0              SUBSS XMM2,XMM0
140379dd5:  66 41 0F 7E D9           MOVD R9D,XMM3
140379dda:  66 41 0F 7E D0           MOVD R8D,XMM2
140379ddf:  49 8B D7                 MOV RDX,R15
140379de2:  48 8D 0D C7 D1 61 00     LEA RCX,[0x140996fb0]
140379de9:  FF 15 89 2A 4F 00        CALL qword ptr [0x14086c878]
140379def:  F3 0F 10 85 A8 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xa8]
140379df7:  0F 5A C8                 CVTPS2PD XMM1,XMM0
140379dfa:  45 33 C9                 XOR R9D,R9D
140379dfd:  45 33 C0                 XOR R8D,R8D
140379e00:  48 8B CE                 MOV RCX,RSI
140379e03:  44 0F 2F C0              COMISS XMM8,XMM0
140379e07:  72 2E                    JC 0x140379e37
140379e09:  E8 12 C6 FB FF           CALL 0x140336420
140379e0e:  48 8D 15 53 47 54 00     LEA RDX,[0x1408be568]
140379e15:  48 8D 0D 34 46 54 00     LEA RCX,[0x1408be450]
140379e1c:  45 84 ED                 TEST R13B,R13B
140379e1f:  48 0F 45 CA              CMOVNZ RCX,RDX
140379e23:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379e28:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
140379e2d:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140379e35:  EB 2C                    JMP 0x140379e63
140379e37:  E8 E4 C5 FB FF           CALL 0x140336420
140379e3c:  48 8D 15 FD 45 54 00     LEA RDX,[0x1408be440]
140379e43:  48 8D 0D E6 45 54 00     LEA RCX,[0x1408be430]
140379e4a:  45 84 ED                 TEST R13B,R13B
140379e4d:  48 0F 45 CA              CMOVNZ RCX,RDX
140379e51:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140379e56:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
140379e5b:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
140379e63:  F3 0F 10 15 1D 82 61 00  MOVSS XMM2,dword ptr [0x140992088]
140379e6b:  0F 28 C2                 MOVAPS XMM0,XMM2
140379e6e:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140379e73:  F3 0F 5C F0              SUBSS XMM6,XMM0
140379e77:  66 0F 6E 05 0D B4 61 00  MOVD XMM0,dword ptr [0x14099528c]
140379e7f:  F3 0F 59 15 E1 0A 59 00  MULSS XMM2,dword ptr [0x14090a968]
140379e87:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379e8a:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140379e90:  0F 28 DE                 MOVAPS XMM3,XMM6
140379e93:  F3 0F 5C D0              SUBSS XMM2,XMM0
140379e97:  66 41 0F 7E D9           MOVD R9D,XMM3
140379e9c:  66 41 0F 7E D0           MOVD R8D,XMM2
140379ea1:  49 8B D7                 MOV RDX,R15
140379ea4:  48 8D 0D 05 D1 61 00     LEA RCX,[0x140996fb0]
140379eab:  FF 15 C7 29 4F 00        CALL qword ptr [0x14086c878]
140379eb1:  F3 0F 10 35 CF 81 61 00  MOVSS XMM6,dword ptr [0x140992088]
140379eb9:  0F 28 C6                 MOVAPS XMM0,XMM6
140379ebc:  F3 41 0F 59 C4           MULSS XMM0,XMM12
140379ec1:  F3 0F 58 C7              ADDSS XMM0,XMM7
140379ec5:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140379ec9:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140379ecd:  F3 0F 10 4C 24 78        MOVSS XMM1,dword ptr [RSP + 0x78]
140379ed3:  F3 0F 58 CF              ADDSS XMM1,XMM7
140379ed7:  0F 28 C6                 MOVAPS XMM0,XMM6
140379eda:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140379edf:  F3 0F 58 C8              ADDSS XMM1,XMM0
140379ee3:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140379ee7:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140379eeb:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
140379ef2:  75 4E                    JNZ 0x140379f42
140379ef4:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
140379ef9:  48 C1 E9 20              SHR RCX,0x20
140379efd:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
140379f02:  48 C1 E8 20              SHR RAX,0x20
140379f06:  03 C8                    ADD ECX,EAX
140379f08:  66 0F 6E D1              MOVD XMM2,ECX
140379f0c:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
140379f0f:  F3 41 0F 59 D3           MULSS XMM2,XMM11
140379f14:  8B 44 24 68              MOV EAX,dword ptr [RSP + 0x68]
140379f18:  03 44 24 60              ADD EAX,dword ptr [RSP + 0x60]
140379f1c:  66 0F 6E C8              MOVD XMM1,EAX
140379f20:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140379f23:  F3 41 0F 59 CB           MULSS XMM1,XMM11
140379f28:  41 B9 06 00 00 00        MOV R9D,0x6
140379f2e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140379f35:  E8 B6 C2 0C 00           CALL 0x1404461f0
140379f3a:  F3 0F 10 35 46 81 61 00  MOVSS XMM6,dword ptr [0x140992088]
140379f42:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140379f47:  0F 84 04 01 00 00        JZ 0x14037a051
140379f4d:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140379f53:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379f56:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140379f5b:  0F 2F C8                 COMISS XMM1,XMM0
140379f5e:  0F 86 ED 00 00 00        JBE 0x14037a051
140379f64:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
140379f6a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379f6d:  0F 2F C1                 COMISS XMM0,XMM1
140379f70:  0F 86 DB 00 00 00        JBE 0x14037a051
140379f76:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140379f7b:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140379f80:  66 0F 6E C3              MOVD XMM0,EBX
140379f84:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379f87:  0F 2F C8                 COMISS XMM1,XMM0
140379f8a:  0F 86 C1 00 00 00        JBE 0x14037a051
140379f90:  66 0F 6E C7              MOVD XMM0,EDI
140379f94:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140379f97:  0F 2F C1                 COMISS XMM0,XMM1
140379f9a:  0F 86 B1 00 00 00        JBE 0x14037a051
140379fa0:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140379fa7:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140379faa:  48 8D 0D 1F 0D 67 00     LEA RCX,[0x1409eacd0]
140379fb1:  FF 15 39 27 4F 00        CALL qword ptr [0x14086c6f0]
140379fb7:  4C 8B C0                 MOV R8,RAX
140379fba:  33 D2                    XOR EDX,EDX
140379fbc:  48 8B CB                 MOV RCX,RBX
140379fbf:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140379fc2:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140379fc9:  41 0F 28 D9              MOVAPS XMM3,XMM9
140379fcd:  41 0F 28 D1              MOVAPS XMM2,XMM9
140379fd1:  41 0F 28 C9              MOVAPS XMM1,XMM9
140379fd5:  48 8D 8D 50 08 00 00     LEA RCX,[RBP + 0x850]
140379fdc:  FF 15 3E 20 4F 00        CALL qword ptr [0x14086c020]
140379fe2:  48 8B D0                 MOV RDX,RAX
140379fe5:  48 8D 0D 74 40 64 00     LEA RCX,[0x1409be060]
140379fec:  FF 15 A6 28 4F 00        CALL qword ptr [0x14086c898]
140379ff2:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
140379ff7:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
140379ffd:  F3 0F 10 15 83 80 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a005:  F3 0F 59 15 1B 09 59 00  MULSS XMM2,dword ptr [0x14090a928]
14037a00d:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
14037a014:  F3 44 0F 10 25 E7 FC 58 00 MOVSS XMM12,dword ptr [0x140909d04]
14037a01d:  F3 44 0F 11 64 24 30     MOVSS dword ptr [RSP + 0x30],XMM12
14037a024:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
14037a02b:  F3 44 0F 11 64 24 20     MOVSS dword ptr [RSP + 0x20],XMM12
14037a032:  0F 28 DA                 MOVAPS XMM3,XMM2
14037a035:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037a03a:  48 8D 0D 1F 40 64 00     LEA RCX,[0x1409be060]
14037a041:  FF 15 11 28 4F 00        CALL qword ptr [0x14086c858]
14037a047:  F3 0F 10 35 39 80 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037a04f:  EB 09                    JMP 0x14037a05a
14037a051:  F3 44 0F 10 25 AA FC 58 00 MOVSS XMM12,dword ptr [0x140909d04]
14037a05a:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037a05f:  BA 1B DF 00 00           MOV EDX,0xdf1b
14037a064:  48 8D 0D 25 D5 61 00     LEA RCX,[0x140997590]
14037a06b:  FF 15 1F 28 4F 00        CALL qword ptr [0x14086c890]
14037a071:  0F 28 DF                 MOVAPS XMM3,XMM7
14037a074:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037a078:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037a07d:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a082:  48 8D 05 97 43 54 00     LEA RAX,[0x1408be420]
14037a089:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a08e:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a096:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a09b:  0F 28 D6                 MOVAPS XMM2,XMM6
14037a09e:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a0a3:  49 8B D7                 MOV RDX,R15
14037a0a6:  48 8D 0D 03 CF 61 00     LEA RCX,[0x140996fb0]
14037a0ad:  FF 15 CD 27 4F 00        CALL qword ptr [0x14086c880]
14037a0b3:  F3 0F 10 85 AC 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xac]
14037a0bb:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037a0be:  45 33 C9                 XOR R9D,R9D
14037a0c1:  45 33 C0                 XOR R8D,R8D
14037a0c4:  48 8B CE                 MOV RCX,RSI
14037a0c7:  44 0F 2F C0              COMISS XMM8,XMM0
14037a0cb:  72 20                    JC 0x14037a0ed
14037a0cd:  E8 4E C3 FB FF           CALL 0x140336420
14037a0d2:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a0d7:  48 8D 05 A2 43 54 00     LEA RAX,[0x1408be480]
14037a0de:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a0e3:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a0eb:  EB 1E                    JMP 0x14037a10b
14037a0ed:  E8 2E C3 FB FF           CALL 0x140336420
14037a0f2:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a0f7:  48 8D 05 72 43 54 00     LEA RAX,[0x1408be470]
14037a0fe:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a103:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
14037a10b:  F3 0F 10 15 75 7F 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a113:  0F 28 C2                 MOVAPS XMM0,XMM2
14037a116:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037a11b:  0F 28 DF                 MOVAPS XMM3,XMM7
14037a11e:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037a122:  F3 0F 10 35 3E 08 59 00  MOVSS XMM6,dword ptr [0x14090a968]
14037a12a:  66 0F 6E 05 5A B1 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037a132:  F3 0F 59 D6              MULSS XMM2,XMM6
14037a136:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a139:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037a13e:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037a142:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a147:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a14c:  49 8B D7                 MOV RDX,R15
14037a14f:  48 8D 0D 5A CE 61 00     LEA RCX,[0x140996fb0]
14037a156:  FF 15 1C 27 4F 00        CALL qword ptr [0x14086c878]
14037a15c:  F3 0F 10 85 B0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xb0]
14037a164:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037a167:  45 33 C9                 XOR R9D,R9D
14037a16a:  45 33 C0                 XOR R8D,R8D
14037a16d:  48 8B CE                 MOV RCX,RSI
14037a170:  44 0F 2F C0              COMISS XMM8,XMM0
14037a174:  72 20                    JC 0x14037a196
14037a176:  E8 A5 C2 FB FF           CALL 0x140336420
14037a17b:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a180:  48 8D 05 E1 41 54 00     LEA RAX,[0x1408be368]
14037a187:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a18c:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a194:  EB 1E                    JMP 0x14037a1b4
14037a196:  E8 85 C2 FB FF           CALL 0x140336420
14037a19b:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a1a0:  48 8D 05 B9 42 54 00     LEA RAX,[0x1408be460]
14037a1a7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a1ac:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
14037a1b4:  F3 0F 10 15 CC 7E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a1bc:  0F 28 C2                 MOVAPS XMM0,XMM2
14037a1bf:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037a1c4:  0F 28 DF                 MOVAPS XMM3,XMM7
14037a1c7:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037a1cb:  66 0F 6E 05 B9 B0 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037a1d3:  F3 0F 59 D6              MULSS XMM2,XMM6
14037a1d7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a1da:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037a1e0:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037a1e4:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a1e9:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a1ee:  49 8B D7                 MOV RDX,R15
14037a1f1:  48 8D 0D B8 CD 61 00     LEA RCX,[0x140996fb0]
14037a1f8:  FF 15 7A 26 4F 00        CALL qword ptr [0x14086c878]
14037a1fe:  F3 44 0F 58 EF           ADDSS XMM13,XMM7
14037a203:  F3 0F 10 54 24 78        MOVSS XMM2,dword ptr [RSP + 0x78]
14037a209:  F3 44 0F 58 EA           ADDSS XMM13,XMM2
14037a20e:  F3 0F 10 0D 72 7E 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037a216:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037a21b:  41 0F 28 C5              MOVAPS XMM0,XMM13
14037a21f:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037a223:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037a227:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037a22b:  44 0F 28 D2              MOVAPS XMM10,XMM2
14037a22f:  F3 45 0F 58 D5           ADDSS XMM10,XMM13
14037a234:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037a239:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037a23d:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037a241:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
14037a248:  75 46                    JNZ 0x14037a290
14037a24a:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037a24f:  48 C1 E9 20              SHR RCX,0x20
14037a253:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
14037a258:  48 C1 E8 20              SHR RAX,0x20
14037a25c:  03 C8                    ADD ECX,EAX
14037a25e:  66 0F 6E D1              MOVD XMM2,ECX
14037a262:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037a265:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037a26a:  8B 44 24 68              MOV EAX,dword ptr [RSP + 0x68]
14037a26e:  03 44 24 60              ADD EAX,dword ptr [RSP + 0x60]
14037a272:  66 0F 6E C8              MOVD XMM1,EAX
14037a276:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
14037a279:  F3 41 0F 59 CB           MULSS XMM1,XMM11
14037a27e:  41 B9 06 00 00 00        MOV R9D,0x6
14037a284:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037a28b:  E8 60 BF 0C 00           CALL 0x1404461f0
14037a290:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037a295:  0F 84 7A 02 00 00        JZ 0x14037a515
14037a29b:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
14037a2a1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a2a4:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037a2a9:  0F 2F C8                 COMISS XMM1,XMM0
14037a2ac:  0F 86 63 02 00 00        JBE 0x14037a515
14037a2b2:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
14037a2b8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a2bb:  0F 2F C1                 COMISS XMM0,XMM1
14037a2be:  0F 86 51 02 00 00        JBE 0x14037a515
14037a2c4:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037a2c9:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037a2ce:  66 0F 6E C3              MOVD XMM0,EBX
14037a2d2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a2d5:  0F 2F C8                 COMISS XMM1,XMM0
14037a2d8:  0F 86 37 02 00 00        JBE 0x14037a515
14037a2de:  66 0F 6E C7              MOVD XMM0,EDI
14037a2e2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a2e5:  0F 2F C1                 COMISS XMM0,XMM1
14037a2e8:  0F 86 27 02 00 00        JBE 0x14037a515
14037a2ee:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037a2f5:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037a2f8:  48 8D 0D D1 09 67 00     LEA RCX,[0x1409eacd0]
14037a2ff:  FF 15 EB 23 4F 00        CALL qword ptr [0x14086c6f0]
14037a305:  4C 8B C0                 MOV R8,RAX
14037a308:  33 D2                    XOR EDX,EDX
14037a30a:  48 8B CB                 MOV RCX,RBX
14037a30d:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037a310:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037a317:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037a31b:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037a31f:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037a323:  48 8D 8D 60 08 00 00     LEA RCX,[RBP + 0x860]
14037a32a:  FF 15 F0 1C 4F 00        CALL qword ptr [0x14086c020]
14037a330:  48 8B D0                 MOV RDX,RAX
14037a333:  48 8D 0D 26 3D 64 00     LEA RCX,[0x1409be060]
14037a33a:  FF 15 58 25 4F 00        CALL qword ptr [0x14086c898]
14037a340:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037a345:  66 0F 7F 44 24 60        MOVDQA xmmword ptr [RSP + 0x60],XMM0
14037a34b:  F3 0F 10 15 35 7D 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a353:  F3 0F 59 15 CD 05 59 00  MULSS XMM2,dword ptr [0x14090a928]
14037a35b:  F3 44 0F 11 74 24 38     MOVSS dword ptr [RSP + 0x38],XMM14
14037a362:  F3 44 0F 11 64 24 30     MOVSS dword ptr [RSP + 0x30],XMM12
14037a369:  F3 44 0F 11 74 24 28     MOVSS dword ptr [RSP + 0x28],XMM14
14037a370:  F3 44 0F 11 64 24 20     MOVSS dword ptr [RSP + 0x20],XMM12
14037a377:  0F 28 DA                 MOVAPS XMM3,XMM2
14037a37a:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037a37f:  48 8D 0D DA 3C 64 00     LEA RCX,[0x1409be060]
14037a386:  FF 15 CC 24 4F 00        CALL qword ptr [0x14086c858]
14037a38c:  4C 89 74 24 60           MOV qword ptr [RSP + 0x60],R14
14037a391:  44 89 74 24 68           MOV dword ptr [RSP + 0x68],R14D
14037a396:  4C 8D 05 3B 44 54 00     LEA R8,[0x1408be7d8]
14037a39d:  83 CA FF                 OR EDX,0xffffffff
14037a3a0:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037a3a5:  E8 76 09 EE FF           CALL 0x14025ad20
14037a3aa:  90                       NOP
14037a3ab:  45 8B D6                 MOV R10D,R14D
14037a3ae:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037a3b2:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037a3b6:  4C 2B DF                 SUB R11,RDI
14037a3b9:  49 C1 FB 04              SAR R11,0x4
14037a3bd:  45 85 DB                 TEST R11D,R11D
14037a3c0:  74 71                    JZ 0x14037a433
14037a3c2:  44 8B 64 24 68           MOV R12D,dword ptr [RSP + 0x68]
14037a3c7:  48 8B 5C 24 60           MOV RBX,qword ptr [RSP + 0x60]
14037a3cc:  0F 1F 40 00              NOP dword ptr [RAX]
14037a3d0:  41 8B C2                 MOV EAX,R10D
14037a3d3:  48 03 C0                 ADD RAX,RAX
14037a3d6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037a3da:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037a3de:  85 D2                    TEST EDX,EDX
14037a3e0:  79 17                    JNS 0x14037a3f9
14037a3e2:  48 85 C9                 TEST RCX,RCX
14037a3e5:  74 44                    JZ 0x14037a42b
14037a3e7:  48 83 CA FF              OR RDX,-0x1
14037a3eb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037a3f0:  48 FF C2                 INC RDX
14037a3f3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037a3f7:  75 F7                    JNZ 0x14037a3f0
14037a3f9:  48 85 C9                 TEST RCX,RCX
14037a3fc:  74 2D                    JZ 0x14037a42b
14037a3fe:  41 3B D4                 CMP EDX,R12D
14037a401:  75 28                    JNZ 0x14037a42b
14037a403:  4C 63 C2                 MOVSXD R8,EDX
14037a406:  85 D2                    TEST EDX,EDX
14037a408:  7E 3C                    JLE 0x14037a446
14037a40a:  48 8B D1                 MOV RDX,RCX
14037a40d:  48 F7 DA                 NEG RDX
14037a410:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037a414:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037a417:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037a41b:  75 0E                    JNZ 0x14037a42b
14037a41d:  48 FF C1                 INC RCX
14037a420:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037a424:  49 3B C0                 CMP RAX,R8
14037a427:  7D 1D                    JGE 0x14037a446
14037a429:  EB E9                    JMP 0x14037a414
14037a42b:  41 FF C2                 INC R10D
14037a42e:  45 3B D3                 CMP R10D,R11D
14037a431:  72 9D                    JC 0x14037a3d0
14037a433:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037a438:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037a43c:  E8 AF 52 0D 00           CALL 0x14044f6f0
14037a441:  48 8B 5C 24 60           MOV RBX,qword ptr [RSP + 0x60]
14037a446:  48 85 DB                 TEST RBX,RBX
14037a449:  74 08                    JZ 0x14037a453
14037a44b:  48 8B CB                 MOV RCX,RBX
14037a44e:  E8 8D 3B 4D 00           CALL 0x14084dfe0
14037a453:  4C 89 74 24 60           MOV qword ptr [RSP + 0x60],R14
14037a458:  44 89 74 24 68           MOV dword ptr [RSP + 0x68],R14D
14037a45d:  4C 8D 05 64 43 54 00     LEA R8,[0x1408be7c8]
14037a464:  83 CA FF                 OR EDX,0xffffffff
14037a467:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037a46c:  E8 AF 08 EE FF           CALL 0x14025ad20
14037a471:  90                       NOP
14037a472:  4C 8B 55 98              MOV R10,qword ptr [RBP + -0x68]
14037a476:  48 8B 5D 90              MOV RBX,qword ptr [RBP + -0x70]
14037a47a:  4C 2B D3                 SUB R10,RBX
14037a47d:  49 C1 FA 04              SAR R10,0x4
14037a481:  45 85 D2                 TEST R10D,R10D
14037a484:  74 6C                    JZ 0x14037a4f2
14037a486:  8B 7C 24 68              MOV EDI,dword ptr [RSP + 0x68]
14037a48a:  4C 8B 5C 24 60           MOV R11,qword ptr [RSP + 0x60]
14037a48f:  90                       NOP
14037a490:  41 8B C6                 MOV EAX,R14D
14037a493:  48 03 C0                 ADD RAX,RAX
14037a496:  8B 54 C3 08              MOV EDX,dword ptr [RBX + RAX*0x8 + 0x8]
14037a49a:  48 8B 0C C3              MOV RCX,qword ptr [RBX + RAX*0x8]
14037a49e:  85 D2                    TEST EDX,EDX
14037a4a0:  79 17                    JNS 0x14037a4b9
14037a4a2:  48 85 C9                 TEST RCX,RCX
14037a4a5:  74 43                    JZ 0x14037a4ea
14037a4a7:  48 83 CA FF              OR RDX,-0x1
14037a4ab:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037a4b0:  48 FF C2                 INC RDX
14037a4b3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037a4b7:  75 F7                    JNZ 0x14037a4b0
14037a4b9:  48 85 C9                 TEST RCX,RCX
14037a4bc:  74 2C                    JZ 0x14037a4ea
14037a4be:  3B D7                    CMP EDX,EDI
14037a4c0:  75 28                    JNZ 0x14037a4ea
14037a4c2:  4C 63 C2                 MOVSXD R8,EDX
14037a4c5:  85 D2                    TEST EDX,EDX
14037a4c7:  7E 3C                    JLE 0x14037a505
14037a4c9:  48 8B D1                 MOV RDX,RCX
14037a4cc:  48 F7 DA                 NEG RDX
14037a4cf:  4D 8D 0C 13              LEA R9,[R11 + RDX*0x1]
14037a4d3:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037a4d6:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037a4da:  75 0E                    JNZ 0x14037a4ea
14037a4dc:  48 FF C1                 INC RCX
14037a4df:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037a4e3:  49 3B C0                 CMP RAX,R8
14037a4e6:  7D 1D                    JGE 0x14037a505
14037a4e8:  EB E9                    JMP 0x14037a4d3
14037a4ea:  41 FF C6                 INC R14D
14037a4ed:  45 3B F2                 CMP R14D,R10D
14037a4f0:  72 9E                    JC 0x14037a490
14037a4f2:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037a4f7:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037a4fb:  E8 F0 51 0D 00           CALL 0x14044f6f0
14037a500:  4C 8B 5C 24 60           MOV R11,qword ptr [RSP + 0x60]
14037a505:  4D 85 DB                 TEST R11,R11
14037a508:  74 08                    JZ 0x14037a512
14037a50a:  49 8B CB                 MOV RCX,R11
14037a50d:  E8 CE 3A 4D 00           CALL 0x14084dfe0
14037a512:  41 B4 01                 MOV R12B,0x1
14037a515:  F3 0F 10 35 6B 7B 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037a51d:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037a522:  BA 7B 06 00 00           MOV EDX,0x67b
14037a527:  48 8D 0D 62 D0 61 00     LEA RCX,[0x140997590]
14037a52e:  FF 15 5C 23 4F 00        CALL qword ptr [0x14086c890]
14037a534:  41 0F 28 DD              MOVAPS XMM3,XMM13
14037a538:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037a53c:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037a541:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a546:  48 8D 05 6B 42 54 00     LEA RAX,[0x1408be7b8]
14037a54d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a552:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a55a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a55f:  0F 28 D6                 MOVAPS XMM2,XMM6
14037a562:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a567:  49 8B D7                 MOV RDX,R15
14037a56a:  48 8D 0D 3F CA 61 00     LEA RCX,[0x140996fb0]
14037a571:  FF 15 09 23 4F 00        CALL qword ptr [0x14086c880]
14037a577:  F3 0F 10 85 98 00 00 00  MOVSS XMM0,dword ptr [RBP + 0x98]
14037a57f:  F3 0F 5C 85 B8 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xb8]
14037a587:  F3 0F 5C 85 BC 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xbc]
14037a58f:  F3 0F 5C 85 C4 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xc4]
14037a597:  F3 0F 58 85 E8 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xe8]
14037a59f:  F3 0F 5C 85 E0 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xe0]
14037a5a7:  F3 0F 5C 85 FC 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xfc]
14037a5af:  F3 0F 58 85 5C 01 00 00  ADDSS XMM0,dword ptr [RBP + 0x15c]
14037a5b7:  F3 0F 58 85 AC 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xac]
14037a5bf:  45 84 ED                 TEST R13B,R13B
14037a5c2:  74 08                    JZ 0x14037a5cc
14037a5c4:  F3 0F 58 85 A4 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xa4]
14037a5cc:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037a5cf:  45 33 C9                 XOR R9D,R9D
14037a5d2:  45 33 C0                 XOR R8D,R8D
14037a5d5:  48 8B CE                 MOV RCX,RSI
14037a5d8:  44 0F 2F C0              COMISS XMM8,XMM0
14037a5dc:  72 20                    JC 0x14037a5fe
14037a5de:  E8 3D BE FB FF           CALL 0x140336420
14037a5e3:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a5e8:  48 8D 05 C1 41 54 00     LEA RAX,[0x1408be7b0]
14037a5ef:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a5f4:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a5fc:  EB 1E                    JMP 0x14037a61c
14037a5fe:  E8 1D BE FB FF           CALL 0x140336420
14037a603:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a608:  48 8D 05 01 42 54 00     LEA RAX,[0x1408be810]
14037a60f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a614:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
14037a61c:  F3 0F 10 15 64 7A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a624:  0F 28 C2                 MOVAPS XMM0,XMM2
14037a627:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037a62c:  41 0F 28 DD              MOVAPS XMM3,XMM13
14037a630:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037a634:  F3 0F 10 35 2C 03 59 00  MOVSS XMM6,dword ptr [0x14090a968]
14037a63c:  66 0F 6E 05 48 AC 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037a644:  F3 0F 59 D6              MULSS XMM2,XMM6
14037a648:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a64b:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037a651:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037a655:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a65a:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a65f:  49 8B D7                 MOV RDX,R15
14037a662:  48 8D 0D 47 C9 61 00     LEA RCX,[0x140996fb0]
14037a669:  FF 15 09 22 4F 00        CALL qword ptr [0x14086c878]
14037a66f:  F3 0F 10 85 A0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xa0]
14037a677:  F3 0F 5C 85 CC 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xcc]
14037a67f:  F3 0F 5C 85 D0 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xd0]
14037a687:  F3 0F 5C 85 D8 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xd8]
14037a68f:  F3 0F 58 85 E4 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xe4]
14037a697:  F3 0F 5C 85 DC 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xdc]
14037a69f:  F3 0F 5C 85 F0 00 00 00  SUBSS XMM0,dword ptr [RBP + 0xf0]
14037a6a7:  F3 0F 58 85 60 01 00 00  ADDSS XMM0,dword ptr [RBP + 0x160]
14037a6af:  F3 0F 58 85 B0 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xb0]
14037a6b7:  45 84 ED                 TEST R13B,R13B
14037a6ba:  74 08                    JZ 0x14037a6c4
14037a6bc:  F3 0F 58 85 A8 00 00 00  ADDSS XMM0,dword ptr [RBP + 0xa8]
14037a6c4:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037a6c7:  45 33 C9                 XOR R9D,R9D
14037a6ca:  45 33 C0                 XOR R8D,R8D
14037a6cd:  48 8B CE                 MOV RCX,RSI
14037a6d0:  44 0F 2F C0              COMISS XMM8,XMM0
14037a6d4:  72 20                    JC 0x14037a6f6
14037a6d6:  E8 45 BD FB FF           CALL 0x140336420
14037a6db:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a6e0:  48 8D 05 21 41 54 00     LEA RAX,[0x1408be808]
14037a6e7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a6ec:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a6f4:  EB 1E                    JMP 0x14037a714
14037a6f6:  E8 25 BD FB FF           CALL 0x140336420
14037a6fb:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a700:  48 8D 05 F1 40 54 00     LEA RAX,[0x1408be7f8]
14037a707:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a70c:  C7 44 24 20 00 AA 00 FF  MOV dword ptr [RSP + 0x20],0xff00aa00
14037a714:  F3 0F 10 15 6C 79 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037a71c:  0F 28 C2                 MOVAPS XMM0,XMM2
14037a71f:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037a724:  F3 44 0F 5C E8           SUBSS XMM13,XMM0
14037a729:  66 0F 6E 05 5B AB 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037a731:  F3 0F 59 D6              MULSS XMM2,XMM6
14037a735:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037a738:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037a73e:  41 0F 28 DD              MOVAPS XMM3,XMM13
14037a742:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037a746:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a74b:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a750:  49 8B D7                 MOV RDX,R15
14037a753:  48 8D 0D 56 C8 61 00     LEA RCX,[0x140996fb0]
14037a75a:  FF 15 18 21 4F 00        CALL qword ptr [0x14086c878]
14037a760:  F3 0F 10 35 20 79 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037a768:  E9 48 7D 00 00           JMP 0x1403824b5
14037a76d:  45 84 C0                 TEST R8B,R8B
14037a770:  0F 84 8B 4F 00 00        JZ 0x14037f701
14037a776:  4C 89 B6 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],R14
14037a77d:  0F 28 C6                 MOVAPS XMM0,XMM6
14037a780:  F3 0F 59 05 30 07 59 00  MULSS XMM0,dword ptr [0x14090aeb8]
14037a788:  F3 41 0F 58 C7           ADDSS XMM0,XMM15
14037a78d:  F3 0F 11 44 24 70        MOVSS dword ptr [RSP + 0x70],XMM0
14037a793:  0F 28 C6                 MOVAPS XMM0,XMM6
14037a796:  F3 0F 59 05 E6 07 59 00  MULSS XMM0,dword ptr [0x14090af84]
14037a79e:  F3 41 0F 58 C7           ADDSS XMM0,XMM15
14037a7a3:  F3 0F 11 44 24 74        MOVSS dword ptr [RSP + 0x74],XMM0
14037a7a9:  0F 28 FE                 MOVAPS XMM7,XMM6
14037a7ac:  F3 0F 59 3D A8 01 59 00  MULSS XMM7,dword ptr [0x14090a95c]
14037a7b4:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037a7b9:  BA E7 05 00 00           MOV EDX,0x5e7
14037a7be:  48 8D 0D CB CD 61 00     LEA RCX,[0x140997590]
14037a7c5:  FF 15 C5 20 4F 00        CALL qword ptr [0x14086c890]
14037a7cb:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037a7cf:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037a7d3:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037a7d8:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037a7dd:  48 8D 05 04 40 54 00     LEA RAX,[0x1408be7e8]
14037a7e4:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037a7e9:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037a7f1:  66 41 0F 7E D9           MOVD R9D,XMM3
14037a7f6:  0F 28 D6                 MOVAPS XMM2,XMM6
14037a7f9:  66 41 0F 7E D0           MOVD R8D,XMM2
14037a7fe:  48 8B 15 3B AA 61 00     MOV RDX,qword ptr [0x140995240]
14037a805:  48 8D 0D A4 C7 61 00     LEA RCX,[0x140996fb0]
14037a80c:  FF 15 6E 20 4F 00        CALL qword ptr [0x14086c880]
14037a812:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037a819:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037a81c:  48 8D 0D AD 04 67 00     LEA RCX,[0x1409eacd0]
14037a823:  FF 15 C7 1E 4F 00        CALL qword ptr [0x14086c6f0]
14037a829:  4C 8B C0                 MOV R8,RAX
14037a82c:  33 D2                    XOR EDX,EDX
14037a82e:  48 8B CB                 MOV RCX,RBX
14037a831:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037a834:  F3 0F 10 0D 4C 78 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037a83c:  F3 44 0F 10 35 63 02 59 00 MOVSS XMM14,dword ptr [0x14090aaa8]
14037a845:  F3 41 0F 59 CE           MULSS XMM1,XMM14
14037a84a:  0F 28 D1                 MOVAPS XMM2,XMM1
14037a84d:  48 8D 0D 0C 38 64 00     LEA RCX,[0x1409be060]
14037a854:  FF 15 4E 20 4F 00        CALL qword ptr [0x14086c8a8]
14037a85a:  F3 0F 10 0D 26 78 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037a862:  0F 28 D1                 MOVAPS XMM2,XMM1
14037a865:  F3 0F 59 15 FB 00 59 00  MULSS XMM2,dword ptr [0x14090a968]
14037a86d:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037a872:  F3 0F 59 0D E6 06 59 00  MULSS XMM1,dword ptr [0x14090af60]
14037a87a:  F3 41 0F 58 CF           ADDSS XMM1,XMM15
14037a87f:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037a883:  48 8D 8D 00 04 00 00     LEA RCX,[RBP + 0x400]
14037a88a:  FF 15 78 17 4F 00        CALL qword ptr [0x14086c008]
14037a890:  48 8B D0                 MOV RDX,RAX
14037a893:  48 8D 0D C6 37 64 00     LEA RCX,[0x1409be060]
14037a89a:  FF 15 00 20 4F 00        CALL qword ptr [0x14086c8a0]
14037a8a0:  F3 0F 10 0D E0 77 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037a8a8:  0F 28 D1                 MOVAPS XMM2,XMM1
14037a8ab:  F3 0F 59 15 B5 00 59 00  MULSS XMM2,dword ptr [0x14090a968]
14037a8b3:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037a8b8:  F3 0F 59 0D A0 06 59 00  MULSS XMM1,dword ptr [0x14090af60]
14037a8c0:  F3 41 0F 58 CF           ADDSS XMM1,XMM15
14037a8c5:  41 B9 06 00 00 00        MOV R9D,0x6
14037a8cb:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037a8d2:  E8 19 B9 0C 00           CALL 0x1404461f0
14037a8d7:  F3 44 0F 10 1D 48 F4 58 00 MOVSS XMM11,dword ptr [0x140909d28]
14037a8e0:  F3 44 0F 10 2D 27 F5 58 00 MOVSS XMM13,dword ptr [0x140909e10]
14037a8e9:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037a8f0:  75 10                    JNZ 0x14037a902
14037a8f2:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037a8f9:  48 8D 8D 70 08 00 00     LEA RCX,[RBP + 0x870]
14037a900:  EB 4B                    JMP 0x14037a94d
14037a902:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037a906:  48 8D 8D 10 04 00 00     LEA RCX,[RBP + 0x410]
14037a90d:  FF 15 FD 16 4F 00        CALL qword ptr [0x14086c010]
14037a913:  48 8B D0                 MOV RDX,RAX
14037a916:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037a91a:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037a91e:  48 8D 0D 3B 37 64 00     LEA RCX,[0x1409be060]
14037a925:  FF 15 25 1F 4F 00        CALL qword ptr [0x14086c850]
14037a92b:  84 C0                    TEST AL,AL
14037a92d:  74 10                    JZ 0x14037a93f
14037a92f:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037a936:  48 8D 8D 80 08 00 00     LEA RCX,[RBP + 0x880]
14037a93d:  EB 0E                    JMP 0x14037a94d
14037a93f:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037a946:  48 8D 8D 90 08 00 00     LEA RCX,[RBP + 0x890]
14037a94d:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037a951:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037a955:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037a959:  FF 15 C1 16 4F 00        CALL qword ptr [0x14086c020]
14037a95f:  48 8B D0                 MOV RDX,RAX
14037a962:  48 8D 0D F7 36 64 00     LEA RCX,[0x1409be060]
14037a969:  FF 15 29 1F 4F 00        CALL qword ptr [0x14086c898]
14037a96f:  44 0F B6 6C 24 40        MOVZX R13D,byte ptr [RSP + 0x40]
14037a975:  45 84 ED                 TEST R13B,R13B
14037a978:  0F 84 C7 00 00 00        JZ 0x14037aa45
14037a97e:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037a982:  48 8D 8D 20 04 00 00     LEA RCX,[RBP + 0x420]
14037a989:  FF 15 81 16 4F 00        CALL qword ptr [0x14086c010]
14037a98f:  48 8B D0                 MOV RDX,RAX
14037a992:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037a996:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037a99a:  48 8D 0D BF 36 64 00     LEA RCX,[0x1409be060]
14037a9a1:  FF 15 A9 1E 4F 00        CALL qword ptr [0x14086c850]
14037a9a7:  84 C0                    TEST AL,AL
14037a9a9:  0F 84 96 00 00 00        JZ 0x14037aa45
14037a9af:  48 8D 0D DA A1 6D 00     LEA RCX,[0x140a54b90]
14037a9b6:  FF 15 FC 1E 4F 00        CALL qword ptr [0x14086c8b8]
14037a9bc:  84 C0                    TEST AL,AL
14037a9be:  74 59                    JZ 0x14037aa19
14037a9c0:  80 7E 14 00              CMP byte ptr [RSI + 0x14],0x0
14037a9c4:  75 48                    JNZ 0x14037aa0e
14037a9c6:  48 8D 15 FB 3C 54 00     LEA RDX,[0x1408be6c8]
14037a9cd:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037a9d4:  FF 15 36 23 4F 00        CALL qword ptr [0x14086cd10]
14037a9da:  84 C0                    TEST AL,AL
14037a9dc:  74 30                    JZ 0x14037aa0e
14037a9de:  48 8D 15 E3 3C 54 00     LEA RDX,[0x1408be6c8]
14037a9e5:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037a9ec:  FF 15 26 23 4F 00        CALL qword ptr [0x14086cd18]
14037a9f2:  48 8B D0                 MOV RDX,RAX
14037a9f5:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
14037a9fa:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037a9fe:  41 B0 01                 MOV R8B,0x1
14037aa01:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
14037aa08:  FF 15 9A 28 4F 00        CALL qword ptr [0x14086d2a8]
14037aa0e:  44 89 B6 D0 C7 00 00     MOV dword ptr [RSI + 0xc7d0],R14D
14037aa15:  C6 45 A8 01              MOV byte ptr [RBP + -0x58],0x1
14037aa19:  BA 83 06 00 00           MOV EDX,0x683
14037aa1e:  48 8D 0D 6B CB 61 00     LEA RCX,[0x140997590]
14037aa25:  FF 15 65 1E 4F 00        CALL qword ptr [0x14086c890]
14037aa2b:  48 8B D6                 MOV RDX,RSI
14037aa2e:  48 2B D0                 SUB RDX,RAX
14037aa31:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14037aa34:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14037aa3c:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14037aa40:  66 85 C9                 TEST CX,CX
14037aa43:  75 EC                    JNZ 0x14037aa31
14037aa45:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037aa4a:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037aa51:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037aa58:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037aa5c:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037aa60:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037aa64:  48 8D 0D F5 35 64 00     LEA RCX,[0x1409be060]
14037aa6b:  FF 15 EF 1D 4F 00        CALL qword ptr [0x14086c860]
14037aa71:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037aa78:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037aa7b:  48 8D 0D 4E 02 67 00     LEA RCX,[0x1409eacd0]
14037aa82:  FF 15 68 1C 4F 00        CALL qword ptr [0x14086c6f0]
14037aa88:  4C 8B C0                 MOV R8,RAX
14037aa8b:  33 D2                    XOR EDX,EDX
14037aa8d:  48 8B CB                 MOV RCX,RBX
14037aa90:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037aa93:  F3 0F 10 0D ED 75 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037aa9b:  F3 41 0F 59 CE           MULSS XMM1,XMM14
14037aaa0:  0F 28 D1                 MOVAPS XMM2,XMM1
14037aaa3:  48 8D 0D B6 35 64 00     LEA RCX,[0x1409be060]
14037aaaa:  FF 15 F8 1D 4F 00        CALL qword ptr [0x14086c8a8]
14037aab0:  F3 0F 10 0D D0 75 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037aab8:  0F 28 D1                 MOVAPS XMM2,XMM1
14037aabb:  F3 0F 59 15 A5 FE 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037aac3:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037aac8:  F3 0F 59 0D D0 04 59 00  MULSS XMM1,dword ptr [0x14090afa0]
14037aad0:  F3 41 0F 58 CF           ADDSS XMM1,XMM15
14037aad5:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037aad9:  48 8D 8D 30 04 00 00     LEA RCX,[RBP + 0x430]
14037aae0:  FF 15 22 15 4F 00        CALL qword ptr [0x14086c008]
14037aae6:  48 8B D0                 MOV RDX,RAX
14037aae9:  48 8D 0D 70 35 64 00     LEA RCX,[0x1409be060]
14037aaf0:  FF 15 AA 1D 4F 00        CALL qword ptr [0x14086c8a0]
14037aaf6:  F3 0F 10 0D 8A 75 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037aafe:  0F 28 D1                 MOVAPS XMM2,XMM1
14037ab01:  F3 0F 59 15 5F FE 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037ab09:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037ab0e:  F3 0F 59 0D 8A 04 59 00  MULSS XMM1,dword ptr [0x14090afa0]
14037ab16:  F3 41 0F 58 CF           ADDSS XMM1,XMM15
14037ab1b:  41 B9 06 00 00 00        MOV R9D,0x6
14037ab21:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037ab28:  E8 C3 B6 0C 00           CALL 0x1404461f0
14037ab2d:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037ab34:  75 10                    JNZ 0x14037ab46
14037ab36:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037ab3d:  48 8D 8D A0 08 00 00     LEA RCX,[RBP + 0x8a0]
14037ab44:  EB 50                    JMP 0x14037ab96
14037ab46:  45 84 ED                 TEST R13B,R13B
14037ab49:  74 3D                    JZ 0x14037ab88
14037ab4b:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037ab4f:  48 8D 8D 40 04 00 00     LEA RCX,[RBP + 0x440]
14037ab56:  FF 15 B4 14 4F 00        CALL qword ptr [0x14086c010]
14037ab5c:  48 8B D0                 MOV RDX,RAX
14037ab5f:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037ab63:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037ab67:  48 8D 0D F2 34 64 00     LEA RCX,[0x1409be060]
14037ab6e:  FF 15 DC 1C 4F 00        CALL qword ptr [0x14086c850]
14037ab74:  84 C0                    TEST AL,AL
14037ab76:  74 10                    JZ 0x14037ab88
14037ab78:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037ab7f:  48 8D 8D B0 08 00 00     LEA RCX,[RBP + 0x8b0]
14037ab86:  EB 0E                    JMP 0x14037ab96
14037ab88:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14037ab8f:  48 8D 8D C0 08 00 00     LEA RCX,[RBP + 0x8c0]
14037ab96:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037ab9a:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037ab9e:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037aba2:  FF 15 78 14 4F 00        CALL qword ptr [0x14086c020]
14037aba8:  48 8B D0                 MOV RDX,RAX
14037abab:  48 8D 0D AE 34 64 00     LEA RCX,[0x1409be060]
14037abb2:  FF 15 E0 1C 4F 00        CALL qword ptr [0x14086c898]
14037abb8:  45 84 ED                 TEST R13B,R13B
14037abbb:  0F 84 D3 00 00 00        JZ 0x14037ac94
14037abc1:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037abc5:  48 8D 8D 50 04 00 00     LEA RCX,[RBP + 0x450]
14037abcc:  FF 15 3E 14 4F 00        CALL qword ptr [0x14086c010]
14037abd2:  48 8B D0                 MOV RDX,RAX
14037abd5:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037abd9:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037abdd:  48 8D 0D 7C 34 64 00     LEA RCX,[0x1409be060]
14037abe4:  FF 15 66 1C 4F 00        CALL qword ptr [0x14086c850]
14037abea:  84 C0                    TEST AL,AL
14037abec:  0F 84 A2 00 00 00        JZ 0x14037ac94
14037abf2:  48 8D 0D 97 9F 6D 00     LEA RCX,[0x140a54b90]
14037abf9:  FF 15 B9 1C 4F 00        CALL qword ptr [0x14086c8b8]
14037abff:  84 C0                    TEST AL,AL
14037ac01:  74 5C                    JZ 0x14037ac5f
14037ac03:  80 7E 14 00              CMP byte ptr [RSI + 0x14],0x0
14037ac07:  75 48                    JNZ 0x14037ac51
14037ac09:  48 8D 15 A8 3A 54 00     LEA RDX,[0x1408be6b8]
14037ac10:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037ac17:  FF 15 F3 20 4F 00        CALL qword ptr [0x14086cd10]
14037ac1d:  84 C0                    TEST AL,AL
14037ac1f:  74 30                    JZ 0x14037ac51
14037ac21:  48 8D 15 90 3A 54 00     LEA RDX,[0x1408be6b8]
14037ac28:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14037ac2f:  FF 15 E3 20 4F 00        CALL qword ptr [0x14086cd18]
14037ac35:  48 8B D0                 MOV RDX,RAX
14037ac38:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
14037ac3d:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037ac41:  41 B0 01                 MOV R8B,0x1
14037ac44:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
14037ac4b:  FF 15 57 26 4F 00        CALL qword ptr [0x14086d2a8]
14037ac51:  B8 01 00 00 00           MOV EAX,0x1
14037ac56:  89 86 D0 C7 00 00        MOV dword ptr [RSI + 0xc7d0],EAX
14037ac5c:  88 45 A8                 MOV byte ptr [RBP + -0x58],AL
14037ac5f:  BA 84 06 00 00           MOV EDX,0x684
14037ac64:  48 8D 0D 25 C9 61 00     LEA RCX,[0x140997590]
14037ac6b:  FF 15 1F 1C 4F 00        CALL qword ptr [0x14086c890]
14037ac71:  48 8B D6                 MOV RDX,RSI
14037ac74:  48 2B D0                 SUB RDX,RAX
14037ac77:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037ac80:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14037ac83:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14037ac8b:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14037ac8f:  66 85 C9                 TEST CX,CX
14037ac92:  75 EC                    JNZ 0x14037ac80
14037ac94:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037ac99:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037aca0:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037aca7:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037acab:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037acaf:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037acb3:  48 8D 0D A6 33 64 00     LEA RCX,[0x1409be060]
14037acba:  FF 15 A0 1B 4F 00        CALL qword ptr [0x14086c860]
14037acc0:  F3 0F 59 3D 98 F3 58 00  MULSS XMM7,dword ptr [0x14090a060]
14037acc8:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037accd:  F3 0F 10 35 B3 73 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037acd5:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037acda:  BA 82 06 00 00           MOV EDX,0x682
14037acdf:  48 8D 0D AA C8 61 00     LEA RCX,[0x140997590]
14037ace6:  FF 15 A4 1B 4F 00        CALL qword ptr [0x14086c890]
14037acec:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037acf0:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037acf4:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037acf9:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037acfe:  48 8D 05 A3 39 54 00     LEA RAX,[0x1408be6a8]
14037ad05:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037ad0a:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037ad12:  66 41 0F 7E D9           MOVD R9D,XMM3
14037ad17:  0F 28 D6                 MOVAPS XMM2,XMM6
14037ad1a:  66 41 0F 7E D0           MOVD R8D,XMM2
14037ad1f:  48 8B 15 E2 94 61 00     MOV RDX,qword ptr [0x140994208]
14037ad26:  48 8D 0D 83 C2 61 00     LEA RCX,[0x140996fb0]
14037ad2d:  FF 15 4D 1B 4F 00        CALL qword ptr [0x14086c880]
14037ad33:  F3 0F 10 35 4D 73 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037ad3b:  0F 28 C6                 MOVAPS XMM0,XMM6
14037ad3e:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14037ad43:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
14037ad48:  0F 28 D6                 MOVAPS XMM2,XMM6
14037ad4b:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037ad50:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037ad54:  F3 0F 5C C2              SUBSS XMM0,XMM2
14037ad58:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
14037ad5c:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
14037ad60:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
14037ad66:  0F 28 CF                 MOVAPS XMM1,XMM7
14037ad69:  F3 0F 59 0D D3 F6 58 00  MULSS XMM1,dword ptr [0x14090a444]
14037ad71:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037ad76:  F3 0F 58 CA              ADDSS XMM1,XMM2
14037ad7a:  F3 0F 2C C9              CVTTSS2SI ECX,XMM1
14037ad7e:  89 4C 24 6C              MOV dword ptr [RSP + 0x6c],ECX
14037ad82:  F3 44 0F 10 35 51 F0 58 00 MOVSS XMM14,dword ptr [0x140909ddc]
14037ad8b:  F3 44 0F 10 25 E4 F0 58 00 MOVSS XMM12,dword ptr [0x140909e78]
14037ad94:  F3 44 0F 10 2D 67 EF 58 00 MOVSS XMM13,dword ptr [0x140909d04]
14037ad9d:  44 8B 64 24 60           MOV R12D,dword ptr [RSP + 0x60]
14037ada2:  45 84 ED                 TEST R13B,R13B
14037ada5:  44 8B 6C 24 68           MOV R13D,dword ptr [RSP + 0x68]
14037adaa:  0F 84 B0 02 00 00        JZ 0x14037b060
14037adb0:  66 41 0F 6E C4           MOVD XMM0,R12D
14037adb5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037adb8:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037adbd:  0F 2F C8                 COMISS XMM1,XMM0
14037adc0:  0F 86 9A 02 00 00        JBE 0x14037b060
14037adc6:  66 41 0F 6E C5           MOVD XMM0,R13D
14037adcb:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037adce:  0F 2F C1                 COMISS XMM0,XMM1
14037add1:  0F 86 89 02 00 00        JBE 0x14037b060
14037add7:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037addc:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037ade1:  66 0F 6E C0              MOVD XMM0,EAX
14037ade5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ade8:  0F 2F C8                 COMISS XMM1,XMM0
14037adeb:  0F 86 6F 02 00 00        JBE 0x14037b060
14037adf1:  66 0F 6E C1              MOVD XMM0,ECX
14037adf5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037adf8:  0F 2F C1                 COMISS XMM0,XMM1
14037adfb:  0F 86 5F 02 00 00        JBE 0x14037b060
14037ae01:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037ae08:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037ae0b:  48 8D 0D BE FE 66 00     LEA RCX,[0x1409eacd0]
14037ae12:  FF 15 D8 18 4F 00        CALL qword ptr [0x14086c6f0]
14037ae18:  4C 8B C0                 MOV R8,RAX
14037ae1b:  33 D2                    XOR EDX,EDX
14037ae1d:  48 8B CB                 MOV RCX,RBX
14037ae20:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037ae23:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037ae2a:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037ae2e:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037ae32:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037ae36:  48 8D 8D D0 08 00 00     LEA RCX,[RBP + 0x8d0]
14037ae3d:  FF 15 DD 11 4F 00        CALL qword ptr [0x14086c020]
14037ae43:  48 8B D0                 MOV RDX,RAX
14037ae46:  48 8D 0D 13 32 64 00     LEA RCX,[0x1409be060]
14037ae4d:  FF 15 45 1A 4F 00        CALL qword ptr [0x14086c898]
14037ae53:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037ae58:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037ae5e:  F3 0F 10 15 22 72 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037ae66:  F3 0F 59 15 BA FA 58 00  MULSS XMM2,dword ptr [0x14090a928]
14037ae6e:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037ae75:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037ae7c:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037ae83:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037ae8a:  0F 28 DA                 MOVAPS XMM3,XMM2
14037ae8d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ae92:  48 8D 0D C7 31 64 00     LEA RCX,[0x1409be060]
14037ae99:  FF 15 B9 19 4F 00        CALL qword ptr [0x14086c858]
14037ae9f:  B0 01                    MOV AL,0x1
14037aea1:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14037aea4:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037aeab:  0F 85 D2 00 00 00        JNZ 0x14037af83
14037aeb1:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037aeb6:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037aebb:  4C 8D 05 D6 37 54 00     LEA R8,[0x1408be698]
14037aec2:  83 CA FF                 OR EDX,0xffffffff
14037aec5:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037aeca:  E8 51 FE ED FF           CALL 0x14025ad20
14037aecf:  90                       NOP
14037aed0:  45 8B D6                 MOV R10D,R14D
14037aed3:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037aed7:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037aedb:  4C 2B DF                 SUB R11,RDI
14037aede:  49 C1 FB 04              SAR R11,0x4
14037aee2:  45 85 DB                 TEST R11D,R11D
14037aee5:  74 7C                    JZ 0x14037af63
14037aee7:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037aeec:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037aef1:  0F 1F 40 00              NOP dword ptr [RAX]
14037aef5:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037af00:  41 8B C2                 MOV EAX,R10D
14037af03:  48 03 C0                 ADD RAX,RAX
14037af06:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037af0a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037af0e:  85 D2                    TEST EDX,EDX
14037af10:  79 17                    JNS 0x14037af29
14037af12:  48 85 C9                 TEST RCX,RCX
14037af15:  74 44                    JZ 0x14037af5b
14037af17:  48 83 CA FF              OR RDX,-0x1
14037af1b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037af20:  48 FF C2                 INC RDX
14037af23:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037af27:  75 F7                    JNZ 0x14037af20
14037af29:  48 85 C9                 TEST RCX,RCX
14037af2c:  74 2D                    JZ 0x14037af5b
14037af2e:  41 3B D4                 CMP EDX,R12D
14037af31:  75 28                    JNZ 0x14037af5b
14037af33:  4C 63 C2                 MOVSXD R8,EDX
14037af36:  85 D2                    TEST EDX,EDX
14037af38:  7E 3C                    JLE 0x14037af76
14037af3a:  48 8B D1                 MOV RDX,RCX
14037af3d:  48 F7 DA                 NEG RDX
14037af40:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037af44:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037af47:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037af4b:  75 0E                    JNZ 0x14037af5b
14037af4d:  48 FF C1                 INC RCX
14037af50:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037af54:  49 3B C0                 CMP RAX,R8
14037af57:  7D 1D                    JGE 0x14037af76
14037af59:  EB E9                    JMP 0x14037af44
14037af5b:  41 FF C2                 INC R10D
14037af5e:  45 3B D3                 CMP R10D,R11D
14037af61:  72 9D                    JC 0x14037af00
14037af63:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037af68:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037af6c:  E8 7F 47 0D 00           CALL 0x14044f6f0
14037af71:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037af76:  48 85 DB                 TEST RBX,RBX
14037af79:  74 08                    JZ 0x14037af83
14037af7b:  48 8B CB                 MOV RCX,RBX
14037af7e:  E8 5D 30 4D 00           CALL 0x14084dfe0
14037af83:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037af8a:  0F 85 C3 00 00 00        JNZ 0x14037b053
14037af90:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037af95:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037af9a:  4C 8D 05 FF 37 54 00     LEA R8,[0x1408be7a0]
14037afa1:  83 CA FF                 OR EDX,0xffffffff
14037afa4:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037afa9:  E8 72 FD ED FF           CALL 0x14025ad20
14037afae:  90                       NOP
14037afaf:  45 8B D6                 MOV R10D,R14D
14037afb2:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037afb6:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037afba:  4C 2B DF                 SUB R11,RDI
14037afbd:  49 C1 FB 04              SAR R11,0x4
14037afc1:  45 85 DB                 TEST R11D,R11D
14037afc4:  74 6D                    JZ 0x14037b033
14037afc6:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037afcb:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037afd0:  41 8B C2                 MOV EAX,R10D
14037afd3:  48 03 C0                 ADD RAX,RAX
14037afd6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037afda:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037afde:  85 D2                    TEST EDX,EDX
14037afe0:  79 17                    JNS 0x14037aff9
14037afe2:  48 85 C9                 TEST RCX,RCX
14037afe5:  74 44                    JZ 0x14037b02b
14037afe7:  48 83 CA FF              OR RDX,-0x1
14037afeb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037aff0:  48 FF C2                 INC RDX
14037aff3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037aff7:  75 F7                    JNZ 0x14037aff0
14037aff9:  48 85 C9                 TEST RCX,RCX
14037affc:  74 2D                    JZ 0x14037b02b
14037affe:  41 3B D4                 CMP EDX,R12D
14037b001:  75 28                    JNZ 0x14037b02b
14037b003:  4C 63 C2                 MOVSXD R8,EDX
14037b006:  85 D2                    TEST EDX,EDX
14037b008:  7E 3C                    JLE 0x14037b046
14037b00a:  48 8B D1                 MOV RDX,RCX
14037b00d:  48 F7 DA                 NEG RDX
14037b010:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037b014:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037b017:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037b01b:  75 0E                    JNZ 0x14037b02b
14037b01d:  48 FF C1                 INC RCX
14037b020:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037b024:  49 3B C0                 CMP RAX,R8
14037b027:  7D 1D                    JGE 0x14037b046
14037b029:  EB E9                    JMP 0x14037b014
14037b02b:  41 FF C2                 INC R10D
14037b02e:  45 3B D3                 CMP R10D,R11D
14037b031:  72 9D                    JC 0x14037afd0
14037b033:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b038:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037b03c:  E8 AF 46 0D 00           CALL 0x14044f6f0
14037b041:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b046:  48 85 DB                 TEST RBX,RBX
14037b049:  74 08                    JZ 0x14037b053
14037b04b:  48 8B CB                 MOV RCX,RBX
14037b04e:  E8 8D 2F 4D 00           CALL 0x14084dfe0
14037b053:  44 8B 64 24 60           MOV R12D,dword ptr [RSP + 0x60]
14037b058:  F3 0F 10 35 28 70 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037b060:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037b065:  BA E8 05 00 00           MOV EDX,0x5e8
14037b06a:  48 8D 0D 1F C5 61 00     LEA RCX,[0x140997590]
14037b071:  FF 15 19 18 4F 00        CALL qword ptr [0x14086c890]
14037b077:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037b07b:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037b07f:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037b084:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037b089:  48 8D 05 00 37 54 00     LEA RAX,[0x1408be790]
14037b090:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b095:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037b09d:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b0a2:  0F 28 D6                 MOVAPS XMM2,XMM6
14037b0a5:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b0aa:  49 8B D7                 MOV RDX,R15
14037b0ad:  48 8D 0D FC BE 61 00     LEA RCX,[0x140996fb0]
14037b0b4:  FF 15 C6 17 4F 00        CALL qword ptr [0x14086c880]
14037b0ba:  BA 77 06 00 00           MOV EDX,0x677
14037b0bf:  48 8D 0D CA C4 61 00     LEA RCX,[0x140997590]
14037b0c6:  FF 15 C4 17 4F 00        CALL qword ptr [0x14086c890]
14037b0cc:  F3 0F 10 05 B4 6F 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037b0d4:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037b0d9:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037b0dd:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037b0e1:  66 0F 6E 05 A3 A1 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037b0e9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b0ec:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037b0f1:  F3 0F 10 54 24 70        MOVSS XMM2,dword ptr [RSP + 0x70]
14037b0f7:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037b0fb:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037b100:  48 8D 05 79 36 54 00     LEA RAX,[0x1408be780]
14037b107:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b10c:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037b114:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b119:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b11e:  49 8B D7                 MOV RDX,R15
14037b121:  48 8D 0D 88 BE 61 00     LEA RCX,[0x140996fb0]
14037b128:  FF 15 5A 17 4F 00        CALL qword ptr [0x14086c888]
14037b12e:  48 8D 0D 9B FB 66 00     LEA RCX,[0x1409eacd0]
14037b135:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037b13c:  75 09                    JNZ 0x14037b147
14037b13e:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037b145:  EB 07                    JMP 0x14037b14e
14037b147:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037b14e:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037b151:  FF 15 99 15 4F 00        CALL qword ptr [0x14086c6f0]
14037b157:  4C 8B C0                 MOV R8,RAX
14037b15a:  33 D2                    XOR EDX,EDX
14037b15c:  48 8B CB                 MOV RCX,RBX
14037b15f:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037b162:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037b169:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037b16d:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037b171:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037b175:  48 8D 8D E0 08 00 00     LEA RCX,[RBP + 0x8e0]
14037b17c:  FF 15 9E 0E 4F 00        CALL qword ptr [0x14086c020]
14037b182:  48 8B D0                 MOV RDX,RAX
14037b185:  48 8D 0D D4 2E 64 00     LEA RCX,[0x1409be060]
14037b18c:  FF 15 06 17 4F 00        CALL qword ptr [0x14086c898]
14037b192:  F3 0F 10 0D EE 6E 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037b19a:  F3 0F 59 0D 6A F8 58 00  MULSS XMM1,dword ptr [0x14090aa0c]
14037b1a2:  0F 28 D1                 MOVAPS XMM2,XMM1
14037b1a5:  48 8D 0D B4 2E 64 00     LEA RCX,[0x1409be060]
14037b1ac:  FF 15 F6 16 4F 00        CALL qword ptr [0x14086c8a8]
14037b1b2:  F3 0F 10 15 CE 6E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b1ba:  F3 44 0F 10 1D 65 F6 58 00 MOVSS XMM11,dword ptr [0x14090a828]
14037b1c3:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037b1c8:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037b1cd:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037b1d1:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
14037b1d7:  48 8D 8D 60 04 00 00     LEA RCX,[RBP + 0x460]
14037b1de:  FF 15 24 0E 4F 00        CALL qword ptr [0x14086c008]
14037b1e4:  48 8B D0                 MOV RDX,RAX
14037b1e7:  48 8D 0D 72 2E 64 00     LEA RCX,[0x1409be060]
14037b1ee:  FF 15 AC 16 4F 00        CALL qword ptr [0x14086c8a0]
14037b1f4:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037b1f9:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037b200:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037b207:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037b20b:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037b20f:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037b213:  48 8D 0D 46 2E 64 00     LEA RCX,[0x1409be060]
14037b21a:  FF 15 40 16 4F 00        CALL qword ptr [0x14086c860]
14037b220:  0F 28 F7                 MOVAPS XMM6,XMM7
14037b223:  F3 0F 59 35 81 EC 58 00  MULSS XMM6,dword ptr [0x140909eac]
14037b22b:  F3 0F 11 75 BC           MOVSS dword ptr [RBP + -0x44],XMM6
14037b230:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
14037b235:  8B 05 35 A0 61 00        MOV EAX,dword ptr [0x140995270]
14037b23b:  83 F8 02                 CMP EAX,0x2
14037b23e:  75 09                    JNZ 0x14037b249
14037b240:  48 8D 05 99 34 54 00     LEA RAX,[0x1408be6e0]
14037b247:  EB 13                    JMP 0x14037b25c
14037b249:  83 F8 01                 CMP EAX,0x1
14037b24c:  48 8D 05 CD 37 54 00     LEA RAX,[0x1408bea20]
14037b253:  74 07                    JZ 0x14037b25c
14037b255:  48 8D 05 D4 36 54 00     LEA RAX,[0x1408be930]
14037b25c:  F3 0F 10 15 24 6E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b264:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037b269:  0F 28 DA                 MOVAPS XMM3,XMM2
14037b26c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b271:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
14037b276:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
14037b27b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037b283:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b288:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b28d:  49 8B D7                 MOV RDX,R15
14037b290:  48 8D 0D 19 BD 61 00     LEA RCX,[0x140996fb0]
14037b297:  FF 15 E3 15 4F 00        CALL qword ptr [0x14086c880]
14037b29d:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
14037b2a2:  F3 0F 10 0D DE 6D 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037b2aa:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037b2af:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037b2b3:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037b2b7:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037b2bb:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037b2bf:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037b2c4:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037b2c8:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037b2cc:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037b2d0:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
14037b2d7:  75 42                    JNZ 0x14037b31b
14037b2d9:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037b2de:  48 C1 E9 20              SHR RCX,0x20
14037b2e2:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
14037b2e7:  48 C1 E8 20              SHR RAX,0x20
14037b2eb:  03 C8                    ADD ECX,EAX
14037b2ed:  66 0F 6E D1              MOVD XMM2,ECX
14037b2f1:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037b2f4:  F3 41 0F 59 D6           MULSS XMM2,XMM14
14037b2f9:  43 8D 04 2C              LEA EAX,[R12 + R13*0x1]
14037b2fd:  66 0F 6E C8              MOVD XMM1,EAX
14037b301:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
14037b304:  F3 41 0F 59 CE           MULSS XMM1,XMM14
14037b309:  41 B9 06 00 00 00        MOV R9D,0x6
14037b30f:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037b316:  E8 D5 AE 0C 00           CALL 0x1404461f0
14037b31b:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037b320:  0F 84 3B 01 00 00        JZ 0x14037b461
14037b326:  66 41 0F 6E C4           MOVD XMM0,R12D
14037b32b:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b32e:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037b333:  0F 2F C8                 COMISS XMM1,XMM0
14037b336:  0F 86 25 01 00 00        JBE 0x14037b461
14037b33c:  66 41 0F 6E C5           MOVD XMM0,R13D
14037b341:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b344:  0F 2F C1                 COMISS XMM0,XMM1
14037b347:  0F 86 14 01 00 00        JBE 0x14037b461
14037b34d:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037b352:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037b357:  66 0F 6E C3              MOVD XMM0,EBX
14037b35b:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b35e:  0F 2F C8                 COMISS XMM1,XMM0
14037b361:  0F 86 FA 00 00 00        JBE 0x14037b461
14037b367:  66 0F 6E C7              MOVD XMM0,EDI
14037b36b:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b36e:  0F 2F C1                 COMISS XMM0,XMM1
14037b371:  0F 86 EA 00 00 00        JBE 0x14037b461
14037b377:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037b37e:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037b381:  48 8D 0D 48 F9 66 00     LEA RCX,[0x1409eacd0]
14037b388:  FF 15 62 13 4F 00        CALL qword ptr [0x14086c6f0]
14037b38e:  4C 8B C0                 MOV R8,RAX
14037b391:  33 D2                    XOR EDX,EDX
14037b393:  48 8B CB                 MOV RCX,RBX
14037b396:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037b399:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037b3a0:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037b3a4:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037b3a8:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037b3ac:  48 8D 8D F0 08 00 00     LEA RCX,[RBP + 0x8f0]
14037b3b3:  FF 15 67 0C 4F 00        CALL qword ptr [0x14086c020]
14037b3b9:  48 8B D0                 MOV RDX,RAX
14037b3bc:  48 8D 0D 9D 2C 64 00     LEA RCX,[0x1409be060]
14037b3c3:  FF 15 CF 14 4F 00        CALL qword ptr [0x14086c898]
14037b3c9:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037b3cd:  48 8D 0D 8C 2C 64 00     LEA RCX,[0x1409be060]
14037b3d4:  FF 15 7E 15 4F 00        CALL qword ptr [0x14086c958]
14037b3da:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037b3df:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037b3e5:  F3 0F 10 15 9B 6C 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b3ed:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037b3f2:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037b3f9:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037b400:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037b407:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037b40e:  0F 28 DA                 MOVAPS XMM3,XMM2
14037b411:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b416:  48 8D 0D 43 2C 64 00     LEA RCX,[0x1409be060]
14037b41d:  FF 15 35 14 4F 00        CALL qword ptr [0x14086c858]
14037b423:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037b427:  48 8D 0D 32 2C 64 00     LEA RCX,[0x1409be060]
14037b42e:  FF 15 24 15 4F 00        CALL qword ptr [0x14086c958]
14037b434:  48 8D 0D 55 97 6D 00     LEA RCX,[0x140a54b90]
14037b43b:  FF 15 77 14 4F 00        CALL qword ptr [0x14086c8b8]
14037b441:  84 C0                    TEST AL,AL
14037b443:  74 1C                    JZ 0x14037b461
14037b445:  C7 86 D4 C7 00 00 04 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x4
14037b44f:  41 B0 01                 MOV R8B,0x1
14037b452:  48 8D 15 BF 34 54 00     LEA RDX,[0x1408be918]
14037b459:  48 8B CE                 MOV RCX,RSI
14037b45c:  E8 6F 46 08 00           CALL 0x1403ffad0
14037b461:  F3 0F 10 35 1F 6C 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037b469:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037b46e:  BA E9 05 00 00           MOV EDX,0x5e9
14037b473:  48 8D 0D 16 C1 61 00     LEA RCX,[0x140997590]
14037b47a:  FF 15 10 14 4F 00        CALL qword ptr [0x14086c890]
14037b480:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037b484:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037b488:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037b48d:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037b492:  48 8D 05 6F 34 54 00     LEA RAX,[0x1408be908]
14037b499:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b49e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037b4a6:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b4ab:  0F 28 D6                 MOVAPS XMM2,XMM6
14037b4ae:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b4b3:  49 8B D7                 MOV RDX,R15
14037b4b6:  48 8D 0D F3 BA 61 00     LEA RCX,[0x140996fb0]
14037b4bd:  FF 15 BD 13 4F 00        CALL qword ptr [0x14086c880]
14037b4c3:  F3 0F 10 95 14 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x114]
14037b4cb:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037b4ce:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037b4d3:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037b4d8:  45 33 C9                 XOR R9D,R9D
14037b4db:  48 8D 95 70 02 00 00     LEA RDX,[RBP + 0x270]
14037b4e2:  48 8B CE                 MOV RCX,RSI
14037b4e5:  E8 76 B7 FB FF           CALL 0x140336c60
14037b4ea:  90                       NOP
14037b4eb:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037b4ee:  F3 0F 10 15 92 6B 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b4f6:  0F 28 C2                 MOVAPS XMM0,XMM2
14037b4f9:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037b4fe:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037b502:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037b506:  F3 44 0F 10 1D 59 F4 58 00 MOVSS XMM11,dword ptr [0x14090a968]
14037b50f:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037b514:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037b51a:  66 0F 6E 05 6A 9D 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037b522:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b525:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037b529:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037b52e:  48 8D 05 0B 36 54 00     LEA RAX,[0x1408beb40]
14037b535:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b53a:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037b542:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b547:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b54c:  49 8B D7                 MOV RDX,R15
14037b54f:  48 8D 0D 5A BA 61 00     LEA RCX,[0x140996fb0]
14037b556:  FF 15 1C 13 4F 00        CALL qword ptr [0x14086c878]
14037b55c:  90                       NOP
14037b55d:  48 8B 8D 70 02 00 00     MOV RCX,qword ptr [RBP + 0x270]
14037b564:  48 85 C9                 TEST RCX,RCX
14037b567:  74 13                    JZ 0x14037b57c
14037b569:  E8 72 2A 4D 00           CALL 0x14084dfe0
14037b56e:  4C 89 B5 70 02 00 00     MOV qword ptr [RBP + 0x270],R14
14037b575:  44 89 B5 78 02 00 00     MOV dword ptr [RBP + 0x278],R14D
14037b57c:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037b583:  75 0A                    JNZ 0x14037b58f
14037b585:  F3 0F 10 85 1C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x11c]
14037b58d:  EB 08                    JMP 0x14037b597
14037b58f:  F3 0F 10 85 18 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x118]
14037b597:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037b59a:  41 B1 01                 MOV R9B,0x1
14037b59d:  45 33 C0                 XOR R8D,R8D
14037b5a0:  48 8B CE                 MOV RCX,RSI
14037b5a3:  E8 78 AE FB FF           CALL 0x140336420
14037b5a8:  F3 0F 10 15 D8 6A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b5b0:  0F 28 C2                 MOVAPS XMM0,XMM2
14037b5b3:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037b5b8:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
14037b5bd:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037b5c2:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037b5c8:  66 0F 6E 05 BC 9C 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037b5d0:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b5d3:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037b5d7:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037b5dc:  48 8D 05 55 35 54 00     LEA RAX,[0x1408beb38]
14037b5e3:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037b5e8:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037b5f0:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037b5f4:  66 41 0F 7E D9           MOVD R9D,XMM3
14037b5f9:  66 41 0F 7E D0           MOVD R8D,XMM2
14037b5fe:  49 8B D7                 MOV RDX,R15
14037b601:  48 8D 0D A8 B9 61 00     LEA RCX,[0x140996fb0]
14037b608:  FF 15 6A 12 4F 00        CALL qword ptr [0x14086c878]
14037b60e:  F3 0F 10 15 72 6A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b616:  44 0F 28 D2              MOVAPS XMM10,XMM2
14037b61a:  F3 44 0F 59 15 05 F2 58 00 MULSS XMM10,dword ptr [0x14090a828]
14037b623:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037b628:  0F 28 CA                 MOVAPS XMM1,XMM2
14037b62b:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037b630:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037b634:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037b638:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
14037b63c:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
14037b640:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
14037b646:  0F 28 C7                 MOVAPS XMM0,XMM7
14037b649:  F3 0F 59 05 DF F0 58 00  MULSS XMM0,dword ptr [0x14090a730]
14037b651:  F3 0F 11 45 F8           MOVSS dword ptr [RBP + -0x8],XMM0
14037b656:  F3 41 0F 58 C2           ADDSS XMM0,XMM10
14037b65b:  F3 0F 58 C1              ADDSS XMM0,XMM1
14037b65f:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037b663:  89 5C 24 6C              MOV dword ptr [RSP + 0x6c],EBX
14037b667:  66 0F 6E F0              MOVD XMM6,EAX
14037b66b:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14037b66e:  43 8D 04 2C              LEA EAX,[R12 + R13*0x1]
14037b672:  66 0F 6E C0              MOVD XMM0,EAX
14037b676:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b679:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037b67e:  F3 0F 11 45 B0           MOVSS dword ptr [RBP + -0x50],XMM0
14037b683:  F3 0F 59 15 41 F2 58 00  MULSS XMM2,dword ptr [0x14090a8cc]
14037b68b:  F3 0F 58 D6              ADDSS XMM2,XMM6
14037b68f:  41 B9 06 00 00 00        MOV R9D,0x6
14037b695:  0F 28 C8                 MOVAPS XMM1,XMM0
14037b698:  48 8D 86 B0 AD 00 00     LEA RAX,[RSI + 0xadb0]
14037b69f:  48 8B C8                 MOV RCX,RAX
14037b6a2:  E8 49 AB 0C 00           CALL 0x1404461f0
14037b6a7:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037b6ac:  0F 84 72 07 00 00        JZ 0x14037be24
14037b6b2:  66 41 0F 6E C4           MOVD XMM0,R12D
14037b6b7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b6ba:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037b6bf:  0F 2F C8                 COMISS XMM1,XMM0
14037b6c2:  0F 86 5C 07 00 00        JBE 0x14037be24
14037b6c8:  66 41 0F 6E C5           MOVD XMM0,R13D
14037b6cd:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b6d0:  0F 2F C1                 COMISS XMM0,XMM1
14037b6d3:  0F 86 4B 07 00 00        JBE 0x14037be24
14037b6d9:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037b6de:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037b6e3:  0F 2F CE                 COMISS XMM1,XMM6
14037b6e6:  0F 86 38 07 00 00        JBE 0x14037be24
14037b6ec:  66 0F 6E C3              MOVD XMM0,EBX
14037b6f0:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037b6f3:  0F 2F C1                 COMISS XMM0,XMM1
14037b6f6:  0F 86 28 07 00 00        JBE 0x14037be24
14037b6fc:  B0 01                    MOV AL,0x1
14037b6fe:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14037b701:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037b708:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037b70b:  48 8D 0D BE F5 66 00     LEA RCX,[0x1409eacd0]
14037b712:  FF 15 D8 0F 4F 00        CALL qword ptr [0x14086c6f0]
14037b718:  4C 8B C0                 MOV R8,RAX
14037b71b:  33 D2                    XOR EDX,EDX
14037b71d:  48 8B CB                 MOV RCX,RBX
14037b720:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037b723:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037b72a:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037b72e:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037b732:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037b736:  48 8D 8D 00 09 00 00     LEA RCX,[RBP + 0x900]
14037b73d:  FF 15 DD 08 4F 00        CALL qword ptr [0x14086c020]
14037b743:  48 8B D0                 MOV RDX,RAX
14037b746:  48 8D 0D 13 29 64 00     LEA RCX,[0x1409be060]
14037b74d:  FF 15 45 11 4F 00        CALL qword ptr [0x14086c898]
14037b753:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037b758:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037b75e:  F3 0F 10 15 22 69 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037b766:  F3 0F 59 15 BA F1 58 00  MULSS XMM2,dword ptr [0x14090a928]
14037b76e:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037b775:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037b77c:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037b783:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037b78a:  0F 28 DA                 MOVAPS XMM3,XMM2
14037b78d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b792:  48 8D 0D C7 28 64 00     LEA RCX,[0x1409be060]
14037b799:  FF 15 B9 10 4F 00        CALL qword ptr [0x14086c858]
14037b79f:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037b7a6:  0F 85 37 03 00 00        JNZ 0x14037bae3
14037b7ac:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037b7b1:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037b7b6:  4C 8D 05 5B 33 54 00     LEA R8,[0x1408beb18]
14037b7bd:  83 CA FF                 OR EDX,0xffffffff
14037b7c0:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037b7c5:  E8 56 F5 ED FF           CALL 0x14025ad20
14037b7ca:  90                       NOP
14037b7cb:  45 8B D6                 MOV R10D,R14D
14037b7ce:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037b7d2:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037b7d6:  4C 2B DF                 SUB R11,RDI
14037b7d9:  49 C1 FB 04              SAR R11,0x4
14037b7dd:  45 85 DB                 TEST R11D,R11D
14037b7e0:  74 71                    JZ 0x14037b853
14037b7e2:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037b7e7:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b7ec:  0F 1F 40 00              NOP dword ptr [RAX]
14037b7f0:  41 8B C2                 MOV EAX,R10D
14037b7f3:  48 03 C0                 ADD RAX,RAX
14037b7f6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037b7fa:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037b7fe:  85 D2                    TEST EDX,EDX
14037b800:  79 17                    JNS 0x14037b819
14037b802:  48 85 C9                 TEST RCX,RCX
14037b805:  74 44                    JZ 0x14037b84b
14037b807:  48 83 CA FF              OR RDX,-0x1
14037b80b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037b810:  48 FF C2                 INC RDX
14037b813:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037b817:  75 F7                    JNZ 0x14037b810
14037b819:  48 85 C9                 TEST RCX,RCX
14037b81c:  74 2D                    JZ 0x14037b84b
14037b81e:  41 3B D4                 CMP EDX,R12D
14037b821:  75 28                    JNZ 0x14037b84b
14037b823:  4C 63 C2                 MOVSXD R8,EDX
14037b826:  85 D2                    TEST EDX,EDX
14037b828:  7E 3C                    JLE 0x14037b866
14037b82a:  48 8B D1                 MOV RDX,RCX
14037b82d:  48 F7 DA                 NEG RDX
14037b830:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037b834:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037b837:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037b83b:  75 0E                    JNZ 0x14037b84b
14037b83d:  48 FF C1                 INC RCX
14037b840:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037b844:  49 3B C0                 CMP RAX,R8
14037b847:  7D 1D                    JGE 0x14037b866
14037b849:  EB E9                    JMP 0x14037b834
14037b84b:  41 FF C2                 INC R10D
14037b84e:  45 3B D3                 CMP R10D,R11D
14037b851:  72 9D                    JC 0x14037b7f0
14037b853:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b858:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037b85c:  E8 8F 3E 0D 00           CALL 0x14044f6f0
14037b861:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b866:  48 85 DB                 TEST RBX,RBX
14037b869:  74 08                    JZ 0x14037b873
14037b86b:  48 8B CB                 MOV RCX,RBX
14037b86e:  E8 6D 27 4D 00           CALL 0x14084dfe0
14037b873:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037b878:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037b87d:  4C 8D 05 74 31 54 00     LEA R8,[0x1408be9f8]
14037b884:  83 CA FF                 OR EDX,0xffffffff
14037b887:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037b88c:  E8 8F F4 ED FF           CALL 0x14025ad20
14037b891:  90                       NOP
14037b892:  45 8B D6                 MOV R10D,R14D
14037b895:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037b899:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037b89d:  4C 2B DF                 SUB R11,RDI
14037b8a0:  49 C1 FB 04              SAR R11,0x4
14037b8a4:  45 85 DB                 TEST R11D,R11D
14037b8a7:  74 7A                    JZ 0x14037b923
14037b8a9:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037b8ae:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b8b3:  0F 1F 40 00              NOP dword ptr [RAX]
14037b8b7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037b8c0:  41 8B C2                 MOV EAX,R10D
14037b8c3:  48 03 C0                 ADD RAX,RAX
14037b8c6:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037b8ca:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037b8ce:  85 D2                    TEST EDX,EDX
14037b8d0:  79 17                    JNS 0x14037b8e9
14037b8d2:  48 85 C9                 TEST RCX,RCX
14037b8d5:  74 44                    JZ 0x14037b91b
14037b8d7:  48 83 CA FF              OR RDX,-0x1
14037b8db:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037b8e0:  48 FF C2                 INC RDX
14037b8e3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037b8e7:  75 F7                    JNZ 0x14037b8e0
14037b8e9:  48 85 C9                 TEST RCX,RCX
14037b8ec:  74 2D                    JZ 0x14037b91b
14037b8ee:  41 3B D4                 CMP EDX,R12D
14037b8f1:  75 28                    JNZ 0x14037b91b
14037b8f3:  4C 63 C2                 MOVSXD R8,EDX
14037b8f6:  85 D2                    TEST EDX,EDX
14037b8f8:  7E 3C                    JLE 0x14037b936
14037b8fa:  48 8B D1                 MOV RDX,RCX
14037b8fd:  48 F7 DA                 NEG RDX
14037b900:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037b904:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037b907:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037b90b:  75 0E                    JNZ 0x14037b91b
14037b90d:  48 FF C1                 INC RCX
14037b910:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037b914:  49 3B C0                 CMP RAX,R8
14037b917:  7D 1D                    JGE 0x14037b936
14037b919:  EB E9                    JMP 0x14037b904
14037b91b:  41 FF C2                 INC R10D
14037b91e:  45 3B D3                 CMP R10D,R11D
14037b921:  72 9D                    JC 0x14037b8c0
14037b923:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b928:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037b92c:  E8 BF 3D 0D 00           CALL 0x14044f6f0
14037b931:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b936:  48 85 DB                 TEST RBX,RBX
14037b939:  74 08                    JZ 0x14037b943
14037b93b:  48 8B CB                 MOV RCX,RBX
14037b93e:  E8 9D 26 4D 00           CALL 0x14084dfe0
14037b943:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037b948:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037b94d:  4C 8D 05 34 2F 54 00     LEA R8,[0x1408be888]
14037b954:  83 CA FF                 OR EDX,0xffffffff
14037b957:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037b95c:  E8 BF F3 ED FF           CALL 0x14025ad20
14037b961:  90                       NOP
14037b962:  45 8B D6                 MOV R10D,R14D
14037b965:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037b969:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037b96d:  4C 2B DF                 SUB R11,RDI
14037b970:  49 C1 FB 04              SAR R11,0x4
14037b974:  45 85 DB                 TEST R11D,R11D
14037b977:  74 7A                    JZ 0x14037b9f3
14037b979:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037b97e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037b983:  0F 1F 40 00              NOP dword ptr [RAX]
14037b987:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037b990:  41 8B C2                 MOV EAX,R10D
14037b993:  48 03 C0                 ADD RAX,RAX
14037b996:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037b99a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037b99e:  85 D2                    TEST EDX,EDX
14037b9a0:  79 17                    JNS 0x14037b9b9
14037b9a2:  48 85 C9                 TEST RCX,RCX
14037b9a5:  74 44                    JZ 0x14037b9eb
14037b9a7:  48 83 CA FF              OR RDX,-0x1
14037b9ab:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037b9b0:  48 FF C2                 INC RDX
14037b9b3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037b9b7:  75 F7                    JNZ 0x14037b9b0
14037b9b9:  48 85 C9                 TEST RCX,RCX
14037b9bc:  74 2D                    JZ 0x14037b9eb
14037b9be:  41 3B D4                 CMP EDX,R12D
14037b9c1:  75 28                    JNZ 0x14037b9eb
14037b9c3:  4C 63 C2                 MOVSXD R8,EDX
14037b9c6:  85 D2                    TEST EDX,EDX
14037b9c8:  7E 3C                    JLE 0x14037ba06
14037b9ca:  48 8B D1                 MOV RDX,RCX
14037b9cd:  48 F7 DA                 NEG RDX
14037b9d0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037b9d4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037b9d7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037b9db:  75 0E                    JNZ 0x14037b9eb
14037b9dd:  48 FF C1                 INC RCX
14037b9e0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037b9e4:  49 3B C0                 CMP RAX,R8
14037b9e7:  7D 1D                    JGE 0x14037ba06
14037b9e9:  EB E9                    JMP 0x14037b9d4
14037b9eb:  41 FF C2                 INC R10D
14037b9ee:  45 3B D3                 CMP R10D,R11D
14037b9f1:  72 9D                    JC 0x14037b990
14037b9f3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037b9f8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037b9fc:  E8 EF 3C 0D 00           CALL 0x14044f6f0
14037ba01:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037ba06:  48 85 DB                 TEST RBX,RBX
14037ba09:  74 08                    JZ 0x14037ba13
14037ba0b:  48 8B CB                 MOV RCX,RBX
14037ba0e:  E8 CD 25 4D 00           CALL 0x14084dfe0
14037ba13:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037ba18:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037ba1d:  4C 8D 05 44 2E 54 00     LEA R8,[0x1408be868]
14037ba24:  83 CA FF                 OR EDX,0xffffffff
14037ba27:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ba2c:  E8 EF F2 ED FF           CALL 0x14025ad20
14037ba31:  90                       NOP
14037ba32:  45 8B D6                 MOV R10D,R14D
14037ba35:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037ba39:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037ba3d:  4C 2B DF                 SUB R11,RDI
14037ba40:  49 C1 FB 04              SAR R11,0x4
14037ba44:  45 85 DB                 TEST R11D,R11D
14037ba47:  74 7A                    JZ 0x14037bac3
14037ba49:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037ba4e:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037ba53:  0F 1F 40 00              NOP dword ptr [RAX]
14037ba57:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037ba60:  41 8B C2                 MOV EAX,R10D
14037ba63:  48 03 C0                 ADD RAX,RAX
14037ba66:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037ba6a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037ba6e:  85 D2                    TEST EDX,EDX
14037ba70:  79 17                    JNS 0x14037ba89
14037ba72:  48 85 C9                 TEST RCX,RCX
14037ba75:  74 44                    JZ 0x14037babb
14037ba77:  48 83 CA FF              OR RDX,-0x1
14037ba7b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037ba80:  48 FF C2                 INC RDX
14037ba83:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037ba87:  75 F7                    JNZ 0x14037ba80
14037ba89:  48 85 C9                 TEST RCX,RCX
14037ba8c:  74 2D                    JZ 0x14037babb
14037ba8e:  41 3B D4                 CMP EDX,R12D
14037ba91:  75 28                    JNZ 0x14037babb
14037ba93:  4C 63 C2                 MOVSXD R8,EDX
14037ba96:  85 D2                    TEST EDX,EDX
14037ba98:  7E 3C                    JLE 0x14037bad6
14037ba9a:  48 8B D1                 MOV RDX,RCX
14037ba9d:  48 F7 DA                 NEG RDX
14037baa0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037baa4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037baa7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037baab:  75 0E                    JNZ 0x14037babb
14037baad:  48 FF C1                 INC RCX
14037bab0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037bab4:  49 3B C0                 CMP RAX,R8
14037bab7:  7D 1D                    JGE 0x14037bad6
14037bab9:  EB E9                    JMP 0x14037baa4
14037babb:  41 FF C2                 INC R10D
14037babe:  45 3B D3                 CMP R10D,R11D
14037bac1:  72 9D                    JC 0x14037ba60
14037bac3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037bac8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037bacc:  E8 1F 3C 0D 00           CALL 0x14044f6f0
14037bad1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037bad6:  48 85 DB                 TEST RBX,RBX
14037bad9:  74 08                    JZ 0x14037bae3
14037badb:  48 8B CB                 MOV RCX,RBX
14037bade:  E8 FD 24 4D 00           CALL 0x14084dfe0
14037bae3:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037baea:  0F 85 34 03 00 00        JNZ 0x14037be24
14037baf0:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037baf5:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037bafa:  4C 8D 05 47 2D 54 00     LEA R8,[0x1408be848]
14037bb01:  83 CA FF                 OR EDX,0xffffffff
14037bb04:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037bb09:  E8 12 F2 ED FF           CALL 0x14025ad20
14037bb0e:  90                       NOP
14037bb0f:  45 8B D6                 MOV R10D,R14D
14037bb12:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
14037bb16:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037bb1a:  48 2B DF                 SUB RBX,RDI
14037bb1d:  48 C1 FB 04              SAR RBX,0x4
14037bb21:  85 DB                    TEST EBX,EBX
14037bb23:  74 6F                    JZ 0x14037bb94
14037bb25:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037bb2a:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bb2f:  90                       NOP
14037bb30:  41 8B C2                 MOV EAX,R10D
14037bb33:  48 03 C0                 ADD RAX,RAX
14037bb36:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
14037bb3a:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
14037bb3e:  85 C9                    TEST ECX,ECX
14037bb40:  79 17                    JNS 0x14037bb59
14037bb42:  48 85 D2                 TEST RDX,RDX
14037bb45:  74 45                    JZ 0x14037bb8c
14037bb47:  48 83 C9 FF              OR RCX,-0x1
14037bb4b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037bb50:  48 FF C1                 INC RCX
14037bb53:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
14037bb57:  75 F7                    JNZ 0x14037bb50
14037bb59:  48 85 D2                 TEST RDX,RDX
14037bb5c:  74 2E                    JZ 0x14037bb8c
14037bb5e:  41 3B CC                 CMP ECX,R12D
14037bb61:  75 29                    JNZ 0x14037bb8c
14037bb63:  4C 63 C9                 MOVSXD R9,ECX
14037bb66:  85 C9                    TEST ECX,ECX
14037bb68:  7E 3D                    JLE 0x14037bba7
14037bb6a:  49 8B CB                 MOV RCX,R11
14037bb6d:  49 2B D3                 SUB RDX,R11
14037bb70:  4D 8B C3                 MOV R8,R11
14037bb73:  49 F7 D8                 NEG R8
14037bb76:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
14037bb7a:  38 01                    CMP byte ptr [RCX],AL
14037bb7c:  75 0E                    JNZ 0x14037bb8c
14037bb7e:  48 FF C1                 INC RCX
14037bb81:  49 8D 04 08              LEA RAX,[R8 + RCX*0x1]
14037bb85:  49 3B C1                 CMP RAX,R9
14037bb88:  7D 1D                    JGE 0x14037bba7
14037bb8a:  EB EA                    JMP 0x14037bb76
14037bb8c:  41 FF C2                 INC R10D
14037bb8f:  44 3B D3                 CMP R10D,EBX
14037bb92:  72 9C                    JC 0x14037bb30
14037bb94:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037bb99:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037bb9d:  E8 4E 3B 0D 00           CALL 0x14044f6f0
14037bba2:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bba7:  4D 85 DB                 TEST R11,R11
14037bbaa:  74 08                    JZ 0x14037bbb4
14037bbac:  49 8B CB                 MOV RCX,R11
14037bbaf:  E8 2C 24 4D 00           CALL 0x14084dfe0
14037bbb4:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037bbb9:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037bbbe:  4C 8D 05 5B 2C 54 00     LEA R8,[0x1408be820]
14037bbc5:  83 CA FF                 OR EDX,0xffffffff
14037bbc8:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037bbcd:  E8 4E F1 ED FF           CALL 0x14025ad20
14037bbd2:  90                       NOP
14037bbd3:  45 8B D6                 MOV R10D,R14D
14037bbd6:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
14037bbda:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037bbde:  48 2B DF                 SUB RBX,RDI
14037bbe1:  48 C1 FB 04              SAR RBX,0x4
14037bbe5:  85 DB                    TEST EBX,EBX
14037bbe7:  74 7B                    JZ 0x14037bc64
14037bbe9:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037bbee:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bbf3:  0F 1F 40 00              NOP dword ptr [RAX]
14037bbf7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037bc00:  41 8B C2                 MOV EAX,R10D
14037bc03:  48 03 C0                 ADD RAX,RAX
14037bc06:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
14037bc0a:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
14037bc0e:  85 C9                    TEST ECX,ECX
14037bc10:  79 17                    JNS 0x14037bc29
14037bc12:  48 85 D2                 TEST RDX,RDX
14037bc15:  74 45                    JZ 0x14037bc5c
14037bc17:  48 83 C9 FF              OR RCX,-0x1
14037bc1b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037bc20:  48 FF C1                 INC RCX
14037bc23:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
14037bc27:  75 F7                    JNZ 0x14037bc20
14037bc29:  48 85 D2                 TEST RDX,RDX
14037bc2c:  74 2E                    JZ 0x14037bc5c
14037bc2e:  41 3B CC                 CMP ECX,R12D
14037bc31:  75 29                    JNZ 0x14037bc5c
14037bc33:  4C 63 C9                 MOVSXD R9,ECX
14037bc36:  85 C9                    TEST ECX,ECX
14037bc38:  7E 3D                    JLE 0x14037bc77
14037bc3a:  49 8B CB                 MOV RCX,R11
14037bc3d:  49 2B D3                 SUB RDX,R11
14037bc40:  4D 8B C3                 MOV R8,R11
14037bc43:  49 F7 D8                 NEG R8
14037bc46:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
14037bc4a:  38 01                    CMP byte ptr [RCX],AL
14037bc4c:  75 0E                    JNZ 0x14037bc5c
14037bc4e:  48 FF C1                 INC RCX
14037bc51:  49 8D 04 08              LEA RAX,[R8 + RCX*0x1]
14037bc55:  49 3B C1                 CMP RAX,R9
14037bc58:  7D 1D                    JGE 0x14037bc77
14037bc5a:  EB EA                    JMP 0x14037bc46
14037bc5c:  41 FF C2                 INC R10D
14037bc5f:  44 3B D3                 CMP R10D,EBX
14037bc62:  72 9C                    JC 0x14037bc00
14037bc64:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037bc69:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037bc6d:  E8 7E 3A 0D 00           CALL 0x14044f6f0
14037bc72:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bc77:  4D 85 DB                 TEST R11,R11
14037bc7a:  74 08                    JZ 0x14037bc84
14037bc7c:  49 8B CB                 MOV RCX,R11
14037bc7f:  E8 5C 23 4D 00           CALL 0x14084dfe0
14037bc84:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037bc89:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037bc8e:  4C 8D 05 53 2C 54 00     LEA R8,[0x1408be8e8]
14037bc95:  83 CA FF                 OR EDX,0xffffffff
14037bc98:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037bc9d:  E8 7E F0 ED FF           CALL 0x14025ad20
14037bca2:  90                       NOP
14037bca3:  45 8B D6                 MOV R10D,R14D
14037bca6:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
14037bcaa:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037bcae:  48 2B DF                 SUB RBX,RDI
14037bcb1:  48 C1 FB 04              SAR RBX,0x4
14037bcb5:  85 DB                    TEST EBX,EBX
14037bcb7:  74 7B                    JZ 0x14037bd34
14037bcb9:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037bcbe:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bcc3:  0F 1F 40 00              NOP dword ptr [RAX]
14037bcc7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037bcd0:  41 8B C2                 MOV EAX,R10D
14037bcd3:  48 03 C0                 ADD RAX,RAX
14037bcd6:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
14037bcda:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
14037bcde:  85 C9                    TEST ECX,ECX
14037bce0:  79 17                    JNS 0x14037bcf9
14037bce2:  48 85 D2                 TEST RDX,RDX
14037bce5:  74 45                    JZ 0x14037bd2c
14037bce7:  48 83 C9 FF              OR RCX,-0x1
14037bceb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037bcf0:  48 FF C1                 INC RCX
14037bcf3:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
14037bcf7:  75 F7                    JNZ 0x14037bcf0
14037bcf9:  48 85 D2                 TEST RDX,RDX
14037bcfc:  74 2E                    JZ 0x14037bd2c
14037bcfe:  41 3B CC                 CMP ECX,R12D
14037bd01:  75 29                    JNZ 0x14037bd2c
14037bd03:  4C 63 C9                 MOVSXD R9,ECX
14037bd06:  85 C9                    TEST ECX,ECX
14037bd08:  7E 3D                    JLE 0x14037bd47
14037bd0a:  49 8B CB                 MOV RCX,R11
14037bd0d:  49 2B D3                 SUB RDX,R11
14037bd10:  4D 8B C3                 MOV R8,R11
14037bd13:  49 F7 D8                 NEG R8
14037bd16:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
14037bd1a:  38 01                    CMP byte ptr [RCX],AL
14037bd1c:  75 0E                    JNZ 0x14037bd2c
14037bd1e:  48 FF C1                 INC RCX
14037bd21:  49 8D 04 08              LEA RAX,[R8 + RCX*0x1]
14037bd25:  49 3B C1                 CMP RAX,R9
14037bd28:  7D 1D                    JGE 0x14037bd47
14037bd2a:  EB EA                    JMP 0x14037bd16
14037bd2c:  41 FF C2                 INC R10D
14037bd2f:  44 3B D3                 CMP R10D,EBX
14037bd32:  72 9C                    JC 0x14037bcd0
14037bd34:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037bd39:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037bd3d:  E8 AE 39 0D 00           CALL 0x14044f6f0
14037bd42:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bd47:  4D 85 DB                 TEST R11,R11
14037bd4a:  74 08                    JZ 0x14037bd54
14037bd4c:  49 8B CB                 MOV RCX,R11
14037bd4f:  E8 8C 22 4D 00           CALL 0x14084dfe0
14037bd54:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037bd59:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037bd5e:  4C 8D 05 63 2B 54 00     LEA R8,[0x1408be8c8]
14037bd65:  83 CA FF                 OR EDX,0xffffffff
14037bd68:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037bd6d:  E8 AE EF ED FF           CALL 0x14025ad20
14037bd72:  90                       NOP
14037bd73:  45 8B D6                 MOV R10D,R14D
14037bd76:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
14037bd7a:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037bd7e:  48 2B DF                 SUB RBX,RDI
14037bd81:  48 C1 FB 04              SAR RBX,0x4
14037bd85:  85 DB                    TEST EBX,EBX
14037bd87:  74 7B                    JZ 0x14037be04
14037bd89:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037bd8e:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037bd93:  0F 1F 40 00              NOP dword ptr [RAX]
14037bd97:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037bda0:  41 8B C2                 MOV EAX,R10D
14037bda3:  48 03 C0                 ADD RAX,RAX
14037bda6:  8B 4C C7 08              MOV ECX,dword ptr [RDI + RAX*0x8 + 0x8]
14037bdaa:  48 8B 14 C7              MOV RDX,qword ptr [RDI + RAX*0x8]
14037bdae:  85 C9                    TEST ECX,ECX
14037bdb0:  79 17                    JNS 0x14037bdc9
14037bdb2:  48 85 D2                 TEST RDX,RDX
14037bdb5:  74 45                    JZ 0x14037bdfc
14037bdb7:  48 83 C9 FF              OR RCX,-0x1
14037bdbb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037bdc0:  48 FF C1                 INC RCX
14037bdc3:  80 3C 0A 00              CMP byte ptr [RDX + RCX*0x1],0x0
14037bdc7:  75 F7                    JNZ 0x14037bdc0
14037bdc9:  48 85 D2                 TEST RDX,RDX
14037bdcc:  74 2E                    JZ 0x14037bdfc
14037bdce:  41 3B CC                 CMP ECX,R12D
14037bdd1:  75 29                    JNZ 0x14037bdfc
14037bdd3:  4C 63 C9                 MOVSXD R9,ECX
14037bdd6:  85 C9                    TEST ECX,ECX
14037bdd8:  7E 3D                    JLE 0x14037be17
14037bdda:  49 8B CB                 MOV RCX,R11
14037bddd:  49 2B D3                 SUB RDX,R11
14037bde0:  4D 8B C3                 MOV R8,R11
14037bde3:  49 F7 D8                 NEG R8
14037bde6:  0F B6 04 0A              MOVZX EAX,byte ptr [RDX + RCX*0x1]
14037bdea:  38 01                    CMP byte ptr [RCX],AL
14037bdec:  75 0E                    JNZ 0x14037bdfc
14037bdee:  48 FF C1                 INC RCX
14037bdf1:  4A 8D 04 01              LEA RAX,[RCX + R8*0x1]
14037bdf5:  49 3B C1                 CMP RAX,R9
14037bdf8:  7D 1D                    JGE 0x14037be17
14037bdfa:  EB EA                    JMP 0x14037bde6
14037bdfc:  41 FF C2                 INC R10D
14037bdff:  44 3B D3                 CMP R10D,EBX
14037be02:  72 9C                    JC 0x14037bda0
14037be04:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037be09:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037be0d:  E8 DE 38 0D 00           CALL 0x14044f6f0
14037be12:  4C 8B 5C 24 50           MOV R11,qword ptr [RSP + 0x50]
14037be17:  4D 85 DB                 TEST R11,R11
14037be1a:  74 08                    JZ 0x14037be24
14037be1c:  49 8B CB                 MOV RCX,R11
14037be1f:  E8 BC 21 4D 00           CALL 0x14084dfe0
14037be24:  F3 0F 10 35 5C 62 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037be2c:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037be31:  BA EA 05 00 00           MOV EDX,0x5ea
14037be36:  48 8D 0D 53 B7 61 00     LEA RCX,[0x140997590]
14037be3d:  FF 15 4D 0A 4F 00        CALL qword ptr [0x14086c890]
14037be43:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037be47:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037be4b:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037be50:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037be55:  48 8D 05 5C 2A 54 00     LEA RAX,[0x1408be8b8]
14037be5c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037be61:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037be69:  66 41 0F 7E D9           MOVD R9D,XMM3
14037be6e:  0F 28 D6                 MOVAPS XMM2,XMM6
14037be71:  66 41 0F 7E D0           MOVD R8D,XMM2
14037be76:  49 8B D7                 MOV RDX,R15
14037be79:  48 8D 0D 30 B1 61 00     LEA RCX,[0x140996fb0]
14037be80:  FF 15 FA 09 4F 00        CALL qword ptr [0x14086c880]
14037be86:  BA 77 06 00 00           MOV EDX,0x677
14037be8b:  48 8D 0D FE B6 61 00     LEA RCX,[0x140997590]
14037be92:  FF 15 F8 09 4F 00        CALL qword ptr [0x14086c890]
14037be98:  F3 0F 10 05 E8 61 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037bea0:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037bea5:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037bea9:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037bead:  66 0F 6E 05 D7 93 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037beb5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037beb8:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037bebd:  F3 0F 10 54 24 70        MOVSS XMM2,dword ptr [RSP + 0x70]
14037bec3:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037bec7:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037becc:  48 8D 05 D5 29 54 00     LEA RAX,[0x1408be8a8]
14037bed3:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037bed8:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037bee0:  66 41 0F 7E D9           MOVD R9D,XMM3
14037bee5:  66 41 0F 7E D0           MOVD R8D,XMM2
14037beea:  49 8B D7                 MOV RDX,R15
14037beed:  48 8D 0D BC B0 61 00     LEA RCX,[0x140996fb0]
14037bef4:  FF 15 8E 09 4F 00        CALL qword ptr [0x14086c888]
14037befa:  48 8D 0D CF ED 66 00     LEA RCX,[0x1409eacd0]
14037bf01:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037bf08:  75 09                    JNZ 0x14037bf13
14037bf0a:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037bf11:  EB 07                    JMP 0x14037bf1a
14037bf13:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037bf1a:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037bf1d:  FF 15 CD 07 4F 00        CALL qword ptr [0x14086c6f0]
14037bf23:  4C 8B C0                 MOV R8,RAX
14037bf26:  33 D2                    XOR EDX,EDX
14037bf28:  48 8B CB                 MOV RCX,RBX
14037bf2b:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037bf2e:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037bf35:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037bf39:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037bf3d:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037bf41:  48 8D 8D 10 09 00 00     LEA RCX,[RBP + 0x910]
14037bf48:  FF 15 D2 00 4F 00        CALL qword ptr [0x14086c020]
14037bf4e:  48 8B D0                 MOV RDX,RAX
14037bf51:  48 8D 0D 08 21 64 00     LEA RCX,[0x1409be060]
14037bf58:  FF 15 3A 09 4F 00        CALL qword ptr [0x14086c898]
14037bf5e:  F3 0F 10 0D 22 61 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037bf66:  F3 0F 59 0D 9E EA 58 00  MULSS XMM1,dword ptr [0x14090aa0c]
14037bf6e:  0F 28 D1                 MOVAPS XMM2,XMM1
14037bf71:  48 8D 0D E8 20 64 00     LEA RCX,[0x1409be060]
14037bf78:  FF 15 2A 09 4F 00        CALL qword ptr [0x14086c8a8]
14037bf7e:  F3 0F 10 15 02 61 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037bf86:  F3 0F 59 15 9A E8 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037bf8e:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037bf93:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037bf97:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
14037bf9d:  48 8D 8D 6C 04 00 00     LEA RCX,[RBP + 0x46c]
14037bfa4:  FF 15 5E 00 4F 00        CALL qword ptr [0x14086c008]
14037bfaa:  48 8B D0                 MOV RDX,RAX
14037bfad:  48 8D 0D AC 20 64 00     LEA RCX,[0x1409be060]
14037bfb4:  FF 15 E6 08 4F 00        CALL qword ptr [0x14086c8a0]
14037bfba:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037bfbf:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037bfc6:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037bfcd:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037bfd1:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037bfd5:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037bfd9:  48 8D 0D 80 20 64 00     LEA RCX,[0x1409be060]
14037bfe0:  FF 15 7A 08 4F 00        CALL qword ptr [0x14086c860]
14037bfe6:  F3 0F 10 75 BC           MOVSS XMM6,dword ptr [RBP + -0x44]
14037bfeb:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
14037bff0:  8B 05 7A 92 61 00        MOV EAX,dword ptr [0x140995270]
14037bff6:  83 F8 02                 CMP EAX,0x2
14037bff9:  75 09                    JNZ 0x14037c004
14037bffb:  48 8D 05 FE 2D 54 00     LEA RAX,[0x1408bee00]
14037c002:  EB 13                    JMP 0x14037c017
14037c004:  83 F8 01                 CMP EAX,0x1
14037c007:  48 8D 05 F2 2C 54 00     LEA RAX,[0x1408bed00]
14037c00e:  74 07                    JZ 0x14037c017
14037c010:  48 8D 05 F9 2B 54 00     LEA RAX,[0x1408bec10]
14037c017:  F3 0F 10 15 69 60 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037c01f:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037c024:  0F 28 DA                 MOVAPS XMM3,XMM2
14037c027:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037c02c:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
14037c031:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
14037c036:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037c03e:  66 41 0F 7E D9           MOVD R9D,XMM3
14037c043:  66 41 0F 7E D0           MOVD R8D,XMM2
14037c048:  49 8B D7                 MOV RDX,R15
14037c04b:  48 8D 0D 5E AF 61 00     LEA RCX,[0x140996fb0]
14037c052:  FF 15 28 08 4F 00        CALL qword ptr [0x14086c880]
14037c058:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
14037c05d:  F3 0F 10 0D 23 60 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037c065:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037c06a:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037c06e:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037c072:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037c076:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037c07a:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037c07f:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037c083:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037c087:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037c08b:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
14037c092:  75 37                    JNZ 0x14037c0cb
14037c094:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037c099:  48 C1 E9 20              SHR RCX,0x20
14037c09d:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
14037c0a2:  48 C1 E8 20              SHR RAX,0x20
14037c0a6:  03 C8                    ADD ECX,EAX
14037c0a8:  66 0F 6E D1              MOVD XMM2,ECX
14037c0ac:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037c0af:  F3 41 0F 59 D6           MULSS XMM2,XMM14
14037c0b4:  41 B9 06 00 00 00        MOV R9D,0x6
14037c0ba:  F3 0F 10 4D B0           MOVSS XMM1,dword ptr [RBP + -0x50]
14037c0bf:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037c0c6:  E8 25 A1 0C 00           CALL 0x1404461f0
14037c0cb:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037c0d0:  0F 84 5E 04 00 00        JZ 0x14037c534
14037c0d6:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
14037c0dc:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c0df:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037c0e4:  0F 2F C8                 COMISS XMM1,XMM0
14037c0e7:  0F 86 47 04 00 00        JBE 0x14037c534
14037c0ed:  66 41 0F 6E C5           MOVD XMM0,R13D
14037c0f2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c0f5:  0F 2F C1                 COMISS XMM0,XMM1
14037c0f8:  0F 86 36 04 00 00        JBE 0x14037c534
14037c0fe:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037c103:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037c108:  66 0F 6E C3              MOVD XMM0,EBX
14037c10c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c10f:  0F 2F C8                 COMISS XMM1,XMM0
14037c112:  0F 86 1C 04 00 00        JBE 0x14037c534
14037c118:  66 0F 6E C7              MOVD XMM0,EDI
14037c11c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c11f:  0F 2F C1                 COMISS XMM0,XMM1
14037c122:  0F 86 0C 04 00 00        JBE 0x14037c534
14037c128:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037c12f:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037c132:  48 8D 0D 97 EB 66 00     LEA RCX,[0x1409eacd0]
14037c139:  FF 15 B1 05 4F 00        CALL qword ptr [0x14086c6f0]
14037c13f:  4C 8B C0                 MOV R8,RAX
14037c142:  33 D2                    XOR EDX,EDX
14037c144:  48 8B CB                 MOV RCX,RBX
14037c147:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037c14a:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037c151:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037c155:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037c159:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037c15d:  48 8D 8D 20 09 00 00     LEA RCX,[RBP + 0x920]
14037c164:  FF 15 B6 FE 4E 00        CALL qword ptr [0x14086c020]
14037c16a:  48 8B D0                 MOV RDX,RAX
14037c16d:  48 8D 0D EC 1E 64 00     LEA RCX,[0x1409be060]
14037c174:  FF 15 1E 07 4F 00        CALL qword ptr [0x14086c898]
14037c17a:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037c17e:  48 8D 0D DB 1E 64 00     LEA RCX,[0x1409be060]
14037c185:  FF 15 CD 07 4F 00        CALL qword ptr [0x14086c958]
14037c18b:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037c190:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037c196:  F3 0F 10 15 EA 5E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037c19e:  F3 0F 59 15 82 E6 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037c1a6:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037c1ad:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037c1b4:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037c1bb:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037c1c2:  0F 28 DA                 MOVAPS XMM3,XMM2
14037c1c5:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c1ca:  48 8D 0D 8F 1E 64 00     LEA RCX,[0x1409be060]
14037c1d1:  FF 15 81 06 4F 00        CALL qword ptr [0x14086c858]
14037c1d7:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037c1db:  48 8D 0D 7E 1E 64 00     LEA RCX,[0x1409be060]
14037c1e2:  FF 15 70 07 4F 00        CALL qword ptr [0x14086c958]
14037c1e8:  48 8D 0D A1 89 6D 00     LEA RCX,[0x140a54b90]
14037c1ef:  FF 15 C3 06 4F 00        CALL qword ptr [0x14086c8b8]
14037c1f5:  84 C0                    TEST AL,AL
14037c1f7:  74 1C                    JZ 0x14037c215
14037c1f9:  41 B0 01                 MOV R8B,0x1
14037c1fc:  48 8D 15 FD 29 54 00     LEA RDX,[0x1408bec00]
14037c203:  48 8B CE                 MOV RCX,RSI
14037c206:  E8 C5 38 08 00           CALL 0x1403ffad0
14037c20b:  C7 86 D4 C7 00 00 05 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x5
14037c215:  B0 01                    MOV AL,0x1
14037c217:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14037c21a:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037c221:  0F 85 7D 01 00 00        JNZ 0x14037c3a4
14037c227:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c22c:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c231:  4C 8D 05 A8 2C 54 00     LEA R8,[0x1408beee0]
14037c238:  83 CA FF                 OR EDX,0xffffffff
14037c23b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c240:  E8 DB EA ED FF           CALL 0x14025ad20
14037c245:  90                       NOP
14037c246:  45 8B D6                 MOV R10D,R14D
14037c249:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037c24d:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037c251:  4C 2B DF                 SUB R11,RDI
14037c254:  49 C1 FB 04              SAR R11,0x4
14037c258:  45 85 DB                 TEST R11D,R11D
14037c25b:  74 76                    JZ 0x14037c2d3
14037c25d:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037c262:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c267:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037c270:  41 8B C2                 MOV EAX,R10D
14037c273:  48 03 C0                 ADD RAX,RAX
14037c276:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037c27a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037c27e:  85 D2                    TEST EDX,EDX
14037c280:  79 17                    JNS 0x14037c299
14037c282:  48 85 C9                 TEST RCX,RCX
14037c285:  74 44                    JZ 0x14037c2cb
14037c287:  48 83 CA FF              OR RDX,-0x1
14037c28b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037c290:  48 FF C2                 INC RDX
14037c293:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037c297:  75 F7                    JNZ 0x14037c290
14037c299:  48 85 C9                 TEST RCX,RCX
14037c29c:  74 2D                    JZ 0x14037c2cb
14037c29e:  41 3B D4                 CMP EDX,R12D
14037c2a1:  75 28                    JNZ 0x14037c2cb
14037c2a3:  4C 63 C2                 MOVSXD R8,EDX
14037c2a6:  85 D2                    TEST EDX,EDX
14037c2a8:  7E 3C                    JLE 0x14037c2e6
14037c2aa:  48 8B D1                 MOV RDX,RCX
14037c2ad:  48 F7 DA                 NEG RDX
14037c2b0:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037c2b4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037c2b7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037c2bb:  75 0E                    JNZ 0x14037c2cb
14037c2bd:  48 FF C1                 INC RCX
14037c2c0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037c2c4:  49 3B C0                 CMP RAX,R8
14037c2c7:  7D 1D                    JGE 0x14037c2e6
14037c2c9:  EB E9                    JMP 0x14037c2b4
14037c2cb:  41 FF C2                 INC R10D
14037c2ce:  45 3B D3                 CMP R10D,R11D
14037c2d1:  72 9D                    JC 0x14037c270
14037c2d3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c2d8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c2dc:  E8 0F 34 0D 00           CALL 0x14044f6f0
14037c2e1:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c2e6:  48 85 DB                 TEST RBX,RBX
14037c2e9:  74 08                    JZ 0x14037c2f3
14037c2eb:  48 8B CB                 MOV RCX,RBX
14037c2ee:  E8 ED 1C 4D 00           CALL 0x14084dfe0
14037c2f3:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c2f8:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c2fd:  4C 8D 05 D4 29 54 00     LEA R8,[0x1408becd8]
14037c304:  83 CA FF                 OR EDX,0xffffffff
14037c307:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c30c:  E8 0F EA ED FF           CALL 0x14025ad20
14037c311:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c316:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c31a:  E8 C1 E9 0C 00           CALL 0x14044ace0
14037c31f:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c324:  48 85 C9                 TEST RCX,RCX
14037c327:  74 05                    JZ 0x14037c32e
14037c329:  E8 B2 1C 4D 00           CALL 0x14084dfe0
14037c32e:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c333:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c338:  4C 8D 05 81 2B 54 00     LEA R8,[0x1408beec0]
14037c33f:  83 CA FF                 OR EDX,0xffffffff
14037c342:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c347:  E8 D4 E9 ED FF           CALL 0x14025ad20
14037c34c:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c351:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c355:  E8 86 E9 0C 00           CALL 0x14044ace0
14037c35a:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c35f:  48 85 C9                 TEST RCX,RCX
14037c362:  74 05                    JZ 0x14037c369
14037c364:  E8 77 1C 4D 00           CALL 0x14084dfe0
14037c369:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c36e:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c373:  4C 8D 05 26 2B 54 00     LEA R8,[0x1408beea0]
14037c37a:  83 CA FF                 OR EDX,0xffffffff
14037c37d:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c382:  E8 99 E9 ED FF           CALL 0x14025ad20
14037c387:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c38c:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c390:  E8 4B E9 0C 00           CALL 0x14044ace0
14037c395:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c39a:  48 85 C9                 TEST RCX,RCX
14037c39d:  74 05                    JZ 0x14037c3a4
14037c39f:  E8 3C 1C 4D 00           CALL 0x14084dfe0
14037c3a4:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037c3ab:  0F 85 83 01 00 00        JNZ 0x14037c534
14037c3b1:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c3b6:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c3bb:  4C 8D 05 EE 27 54 00     LEA R8,[0x1408bebb0]
14037c3c2:  83 CA FF                 OR EDX,0xffffffff
14037c3c5:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c3ca:  E8 51 E9 ED FF           CALL 0x14025ad20
14037c3cf:  90                       NOP
14037c3d0:  45 8B D6                 MOV R10D,R14D
14037c3d3:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037c3d7:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037c3db:  4C 2B DF                 SUB R11,RDI
14037c3de:  49 C1 FB 04              SAR R11,0x4
14037c3e2:  45 85 DB                 TEST R11D,R11D
14037c3e5:  74 7C                    JZ 0x14037c463
14037c3e7:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037c3ec:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c3f1:  0F 1F 40 00              NOP dword ptr [RAX]
14037c3f5:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037c400:  41 8B C2                 MOV EAX,R10D
14037c403:  48 03 C0                 ADD RAX,RAX
14037c406:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037c40a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037c40e:  85 D2                    TEST EDX,EDX
14037c410:  79 17                    JNS 0x14037c429
14037c412:  48 85 C9                 TEST RCX,RCX
14037c415:  74 44                    JZ 0x14037c45b
14037c417:  48 83 CA FF              OR RDX,-0x1
14037c41b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037c420:  48 FF C2                 INC RDX
14037c423:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037c427:  75 F7                    JNZ 0x14037c420
14037c429:  48 85 C9                 TEST RCX,RCX
14037c42c:  74 2D                    JZ 0x14037c45b
14037c42e:  41 3B D4                 CMP EDX,R12D
14037c431:  75 28                    JNZ 0x14037c45b
14037c433:  4C 63 C2                 MOVSXD R8,EDX
14037c436:  85 D2                    TEST EDX,EDX
14037c438:  7E 3C                    JLE 0x14037c476
14037c43a:  48 8B D1                 MOV RDX,RCX
14037c43d:  48 F7 DA                 NEG RDX
14037c440:  4C 8D 0C 13              LEA R9,[RBX + RDX*0x1]
14037c444:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037c447:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037c44b:  75 0E                    JNZ 0x14037c45b
14037c44d:  48 FF C1                 INC RCX
14037c450:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037c454:  49 3B C0                 CMP RAX,R8
14037c457:  7D 1D                    JGE 0x14037c476
14037c459:  EB E9                    JMP 0x14037c444
14037c45b:  41 FF C2                 INC R10D
14037c45e:  45 3B D3                 CMP R10D,R11D
14037c461:  72 9D                    JC 0x14037c400
14037c463:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c468:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c46c:  E8 7F 32 0D 00           CALL 0x14044f6f0
14037c471:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c476:  48 85 DB                 TEST RBX,RBX
14037c479:  74 08                    JZ 0x14037c483
14037c47b:  48 8B CB                 MOV RCX,RBX
14037c47e:  E8 5D 1B 4D 00           CALL 0x14084dfe0
14037c483:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c488:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c48d:  4C 8D 05 F4 26 54 00     LEA R8,[0x1408beb88]
14037c494:  83 CA FF                 OR EDX,0xffffffff
14037c497:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c49c:  E8 7F E8 ED FF           CALL 0x14025ad20
14037c4a1:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c4a6:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c4aa:  E8 31 E8 0C 00           CALL 0x14044ace0
14037c4af:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c4b4:  48 85 C9                 TEST RCX,RCX
14037c4b7:  74 05                    JZ 0x14037c4be
14037c4b9:  E8 22 1B 4D 00           CALL 0x14084dfe0
14037c4be:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c4c3:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c4c8:  4C 8D 05 99 26 54 00     LEA R8,[0x1408beb68]
14037c4cf:  83 CA FF                 OR EDX,0xffffffff
14037c4d2:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c4d7:  E8 44 E8 ED FF           CALL 0x14025ad20
14037c4dc:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c4e1:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c4e5:  E8 F6 E7 0C 00           CALL 0x14044ace0
14037c4ea:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c4ef:  48 85 C9                 TEST RCX,RCX
14037c4f2:  74 05                    JZ 0x14037c4f9
14037c4f4:  E8 E7 1A 4D 00           CALL 0x14084dfe0
14037c4f9:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c4fe:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c503:  4C 8D 05 3E 26 54 00     LEA R8,[0x1408beb48]
14037c50a:  83 CA FF                 OR EDX,0xffffffff
14037c50d:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c512:  E8 09 E8 ED FF           CALL 0x14025ad20
14037c517:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c51c:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c520:  E8 BB E7 0C 00           CALL 0x14044ace0
14037c525:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c52a:  48 85 C9                 TEST RCX,RCX
14037c52d:  74 05                    JZ 0x14037c534
14037c52f:  E8 AC 1A 4D 00           CALL 0x14084dfe0
14037c534:  F3 0F 10 35 4C 5B 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037c53c:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037c541:  BA EB 05 00 00           MOV EDX,0x5eb
14037c546:  48 8D 0D 43 B0 61 00     LEA RCX,[0x140997590]
14037c54d:  FF 15 3D 03 4F 00        CALL qword ptr [0x14086c890]
14037c553:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037c557:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037c55b:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037c560:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037c565:  48 8D 05 84 26 54 00     LEA RAX,[0x1408bebf0]
14037c56c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037c571:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037c579:  66 41 0F 7E D9           MOVD R9D,XMM3
14037c57e:  0F 28 D6                 MOVAPS XMM2,XMM6
14037c581:  66 41 0F 7E D0           MOVD R8D,XMM2
14037c586:  49 8B D7                 MOV RDX,R15
14037c589:  48 8D 0D 20 AA 61 00     LEA RCX,[0x140996fb0]
14037c590:  FF 15 EA 02 4F 00        CALL qword ptr [0x14086c880]
14037c596:  F3 0F 10 95 20 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x120]
14037c59e:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037c5a1:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037c5a6:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037c5ab:  45 33 C9                 XOR R9D,R9D
14037c5ae:  48 8D 95 80 02 00 00     LEA RDX,[RBP + 0x280]
14037c5b5:  48 8B CE                 MOV RCX,RSI
14037c5b8:  E8 A3 A6 FB FF           CALL 0x140336c60
14037c5bd:  90                       NOP
14037c5be:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037c5c1:  F3 0F 10 15 BF 5A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037c5c9:  0F 28 C2                 MOVAPS XMM0,XMM2
14037c5cc:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037c5d1:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037c5d5:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037c5d9:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037c5de:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037c5e4:  66 0F 6E 05 A0 8C 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037c5ec:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c5ef:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037c5f3:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037c5f8:  48 8D 05 E9 25 54 00     LEA RAX,[0x1408bebe8]
14037c5ff:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037c604:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037c60c:  66 41 0F 7E D9           MOVD R9D,XMM3
14037c611:  66 41 0F 7E D0           MOVD R8D,XMM2
14037c616:  49 8B D7                 MOV RDX,R15
14037c619:  48 8D 0D 90 A9 61 00     LEA RCX,[0x140996fb0]
14037c620:  FF 15 52 02 4F 00        CALL qword ptr [0x14086c878]
14037c626:  90                       NOP
14037c627:  48 8B 8D 80 02 00 00     MOV RCX,qword ptr [RBP + 0x280]
14037c62e:  48 85 C9                 TEST RCX,RCX
14037c631:  74 13                    JZ 0x14037c646
14037c633:  E8 A8 19 4D 00           CALL 0x14084dfe0
14037c638:  4C 89 B5 80 02 00 00     MOV qword ptr [RBP + 0x280],R14
14037c63f:  44 89 B5 88 02 00 00     MOV dword ptr [RBP + 0x288],R14D
14037c646:  F3 44 0F 10 1D D1 00 59 00 MOVSS XMM11,dword ptr [0x14090c720]
14037c64f:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037c656:  75 0A                    JNZ 0x14037c662
14037c658:  F3 0F 10 85 28 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x128]
14037c660:  EB 08                    JMP 0x14037c66a
14037c662:  F3 0F 10 85 2C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x12c]
14037c66a:  41 0F 57 C3              XORPS XMM0,XMM11
14037c66e:  0F 57 C9                 XORPS XMM1,XMM1
14037c671:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14037c675:  41 B1 01                 MOV R9B,0x1
14037c678:  45 33 C0                 XOR R8D,R8D
14037c67b:  48 8B CE                 MOV RCX,RSI
14037c67e:  E8 9D 9D FB FF           CALL 0x140336420
14037c683:  F3 0F 10 15 FD 59 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037c68b:  0F 28 C2                 MOVAPS XMM0,XMM2
14037c68e:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037c693:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
14037c698:  F3 0F 59 15 C8 E2 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037c6a0:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037c6a6:  66 0F 6E 05 DE 8B 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037c6ae:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c6b1:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037c6b5:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037c6ba:  48 8D 05 1F 25 54 00     LEA RAX,[0x1408bebe0]
14037c6c1:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037c6c6:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037c6ce:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037c6d2:  66 41 0F 7E D9           MOVD R9D,XMM3
14037c6d7:  66 41 0F 7E D0           MOVD R8D,XMM2
14037c6dc:  49 8B D7                 MOV RDX,R15
14037c6df:  48 8D 0D CA A8 61 00     LEA RCX,[0x140996fb0]
14037c6e6:  FF 15 8C 01 4F 00        CALL qword ptr [0x14086c878]
14037c6ec:  F3 0F 10 0D 94 59 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037c6f4:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037c6f9:  0F 28 C7                 MOVAPS XMM0,XMM7
14037c6fc:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037c700:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037c704:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037c708:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037c70f:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037c714:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037c719:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037c71d:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037c721:  83 BE F0 AD 00 00 06     CMP dword ptr [RSI + 0xadf0],0x6
14037c728:  75 37                    JNZ 0x14037c761
14037c72a:  48 8B 4C 24 60           MOV RCX,qword ptr [RSP + 0x60]
14037c72f:  48 C1 E9 20              SHR RCX,0x20
14037c733:  48 8B 44 24 68           MOV RAX,qword ptr [RSP + 0x68]
14037c738:  48 C1 E8 20              SHR RAX,0x20
14037c73c:  03 C8                    ADD ECX,EAX
14037c73e:  66 0F 6E D1              MOVD XMM2,ECX
14037c742:  0F 5B D2                 CVTDQ2PS XMM2,XMM2
14037c745:  F3 41 0F 59 D6           MULSS XMM2,XMM14
14037c74a:  41 B9 06 00 00 00        MOV R9D,0x6
14037c750:  F3 0F 10 4D B0           MOVSS XMM1,dword ptr [RBP + -0x50]
14037c755:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037c75c:  E8 8F 9A 0C 00           CALL 0x1404461f0
14037c761:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037c766:  0F 84 46 04 00 00        JZ 0x14037cbb2
14037c76c:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
14037c772:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c775:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037c77a:  0F 2F C8                 COMISS XMM1,XMM0
14037c77d:  0F 86 2F 04 00 00        JBE 0x14037cbb2
14037c783:  66 41 0F 6E C5           MOVD XMM0,R13D
14037c788:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c78b:  0F 2F C1                 COMISS XMM0,XMM1
14037c78e:  0F 86 1E 04 00 00        JBE 0x14037cbb2
14037c794:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037c799:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037c79e:  66 0F 6E C3              MOVD XMM0,EBX
14037c7a2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c7a5:  0F 2F C8                 COMISS XMM1,XMM0
14037c7a8:  0F 86 04 04 00 00        JBE 0x14037cbb2
14037c7ae:  66 0F 6E C7              MOVD XMM0,EDI
14037c7b2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037c7b5:  0F 2F C1                 COMISS XMM0,XMM1
14037c7b8:  0F 86 F4 03 00 00        JBE 0x14037cbb2
14037c7be:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037c7c5:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037c7c8:  48 8D 0D 01 E5 66 00     LEA RCX,[0x1409eacd0]
14037c7cf:  FF 15 1B FF 4E 00        CALL qword ptr [0x14086c6f0]
14037c7d5:  4C 8B C0                 MOV R8,RAX
14037c7d8:  33 D2                    XOR EDX,EDX
14037c7da:  48 8B CB                 MOV RCX,RBX
14037c7dd:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037c7e0:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037c7e7:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037c7eb:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037c7ef:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037c7f3:  48 8D 8D 30 09 00 00     LEA RCX,[RBP + 0x930]
14037c7fa:  FF 15 20 F8 4E 00        CALL qword ptr [0x14086c020]
14037c800:  48 8B D0                 MOV RDX,RAX
14037c803:  48 8D 0D 56 18 64 00     LEA RCX,[0x1409be060]
14037c80a:  FF 15 88 00 4F 00        CALL qword ptr [0x14086c898]
14037c810:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037c814:  48 8D 0D 45 18 64 00     LEA RCX,[0x1409be060]
14037c81b:  FF 15 37 01 4F 00        CALL qword ptr [0x14086c958]
14037c821:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037c826:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037c82c:  F3 0F 10 15 54 58 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037c834:  F3 0F 59 15 EC DF 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037c83c:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037c843:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037c84a:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037c851:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037c858:  0F 28 DA                 MOVAPS XMM3,XMM2
14037c85b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c860:  48 8D 0D F9 17 64 00     LEA RCX,[0x1409be060]
14037c867:  FF 15 EB FF 4E 00        CALL qword ptr [0x14086c858]
14037c86d:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037c871:  48 8D 0D E8 17 64 00     LEA RCX,[0x1409be060]
14037c878:  FF 15 DA 00 4F 00        CALL qword ptr [0x14086c958]
14037c87e:  48 8D 0D 0B 83 6D 00     LEA RCX,[0x140a54b90]
14037c885:  FF 15 2D 00 4F 00        CALL qword ptr [0x14086c8b8]
14037c88b:  84 C0                    TEST AL,AL
14037c88d:  74 1C                    JZ 0x14037c8ab
14037c88f:  41 B0 01                 MOV R8B,0x1
14037c892:  48 8D 15 37 23 54 00     LEA RDX,[0x1408bebd0]
14037c899:  48 8B CE                 MOV RCX,RSI
14037c89c:  E8 2F 32 08 00           CALL 0x1403ffad0
14037c8a1:  C7 86 D4 C7 00 00 07 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x7
14037c8ab:  B0 01                    MOV AL,0x1
14037c8ad:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14037c8b0:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037c8b7:  0F 85 77 01 00 00        JNZ 0x14037ca34
14037c8bd:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c8c2:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c8c7:  4C 8D 05 42 27 54 00     LEA R8,[0x1408bf010]
14037c8ce:  83 CA FF                 OR EDX,0xffffffff
14037c8d1:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c8d6:  E8 45 E4 ED FF           CALL 0x14025ad20
14037c8db:  90                       NOP
14037c8dc:  45 8B D6                 MOV R10D,R14D
14037c8df:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037c8e3:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037c8e7:  4C 2B DF                 SUB R11,RDI
14037c8ea:  49 C1 FB 04              SAR R11,0x4
14037c8ee:  45 85 DB                 TEST R11D,R11D
14037c8f1:  74 70                    JZ 0x14037c963
14037c8f3:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037c8f8:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c8fd:  0F 1F 00                 NOP dword ptr [RAX]
14037c900:  41 8B C2                 MOV EAX,R10D
14037c903:  48 03 C0                 ADD RAX,RAX
14037c906:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037c90a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037c90e:  85 D2                    TEST EDX,EDX
14037c910:  79 17                    JNS 0x14037c929
14037c912:  48 85 C9                 TEST RCX,RCX
14037c915:  74 44                    JZ 0x14037c95b
14037c917:  48 83 CA FF              OR RDX,-0x1
14037c91b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037c920:  48 FF C2                 INC RDX
14037c923:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037c927:  75 F7                    JNZ 0x14037c920
14037c929:  48 85 C9                 TEST RCX,RCX
14037c92c:  74 2D                    JZ 0x14037c95b
14037c92e:  41 3B D4                 CMP EDX,R12D
14037c931:  75 28                    JNZ 0x14037c95b
14037c933:  4C 63 C2                 MOVSXD R8,EDX
14037c936:  85 D2                    TEST EDX,EDX
14037c938:  7E 3C                    JLE 0x14037c976
14037c93a:  48 8B D1                 MOV RDX,RCX
14037c93d:  48 F7 DA                 NEG RDX
14037c940:  4C 8D 0C 1A              LEA R9,[RDX + RBX*0x1]
14037c944:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037c947:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037c94b:  75 0E                    JNZ 0x14037c95b
14037c94d:  48 FF C1                 INC RCX
14037c950:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037c954:  49 3B C0                 CMP RAX,R8
14037c957:  7D 1D                    JGE 0x14037c976
14037c959:  EB E9                    JMP 0x14037c944
14037c95b:  41 FF C2                 INC R10D
14037c95e:  45 3B D3                 CMP R10D,R11D
14037c961:  72 9D                    JC 0x14037c900
14037c963:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c968:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c96c:  E8 7F 2D 0D 00           CALL 0x14044f6f0
14037c971:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037c976:  48 85 DB                 TEST RBX,RBX
14037c979:  74 08                    JZ 0x14037c983
14037c97b:  48 8B CB                 MOV RCX,RBX
14037c97e:  E8 5D 16 4D 00           CALL 0x14084dfe0
14037c983:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c988:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c98d:  4C 8D 05 5C 26 54 00     LEA R8,[0x1408beff0]
14037c994:  83 CA FF                 OR EDX,0xffffffff
14037c997:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c99c:  E8 7F E3 ED FF           CALL 0x14025ad20
14037c9a1:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c9a6:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c9aa:  E8 31 E3 0C 00           CALL 0x14044ace0
14037c9af:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c9b4:  48 85 C9                 TEST RCX,RCX
14037c9b7:  74 05                    JZ 0x14037c9be
14037c9b9:  E8 22 16 4D 00           CALL 0x14084dfe0
14037c9be:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c9c3:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037c9c8:  4C 8D 05 01 26 54 00     LEA R8,[0x1408befd0]
14037c9cf:  83 CA FF                 OR EDX,0xffffffff
14037c9d2:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037c9d7:  E8 44 E3 ED FF           CALL 0x14025ad20
14037c9dc:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037c9e1:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037c9e5:  E8 F6 E2 0C 00           CALL 0x14044ace0
14037c9ea:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037c9ef:  48 85 C9                 TEST RCX,RCX
14037c9f2:  74 05                    JZ 0x14037c9f9
14037c9f4:  E8 E7 15 4D 00           CALL 0x14084dfe0
14037c9f9:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037c9fe:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037ca03:  4C 8D 05 A6 25 54 00     LEA R8,[0x1408befb0]
14037ca0a:  83 CA FF                 OR EDX,0xffffffff
14037ca0d:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ca12:  E8 09 E3 ED FF           CALL 0x14025ad20
14037ca17:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ca1c:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037ca20:  E8 BB E2 0C 00           CALL 0x14044ace0
14037ca25:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037ca2a:  48 85 C9                 TEST RCX,RCX
14037ca2d:  74 05                    JZ 0x14037ca34
14037ca2f:  E8 AC 15 4D 00           CALL 0x14084dfe0
14037ca34:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037ca3b:  0F 85 71 01 00 00        JNZ 0x14037cbb2
14037ca41:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037ca46:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037ca4b:  4C 8D 05 46 26 54 00     LEA R8,[0x1408bf098]
14037ca52:  83 CA FF                 OR EDX,0xffffffff
14037ca55:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ca5a:  E8 C1 E2 ED FF           CALL 0x14025ad20
14037ca5f:  90                       NOP
14037ca60:  45 8B D6                 MOV R10D,R14D
14037ca63:  4C 8B 5D 98              MOV R11,qword ptr [RBP + -0x68]
14037ca67:  48 8B 7D 90              MOV RDI,qword ptr [RBP + -0x70]
14037ca6b:  4C 2B DF                 SUB R11,RDI
14037ca6e:  49 C1 FB 04              SAR R11,0x4
14037ca72:  45 85 DB                 TEST R11D,R11D
14037ca75:  74 7C                    JZ 0x14037caf3
14037ca77:  44 8B 64 24 58           MOV R12D,dword ptr [RSP + 0x58]
14037ca7c:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037ca81:  0F 1F 40 00              NOP dword ptr [RAX]
14037ca85:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037ca90:  41 8B C2                 MOV EAX,R10D
14037ca93:  48 03 C0                 ADD RAX,RAX
14037ca96:  8B 54 C7 08              MOV EDX,dword ptr [RDI + RAX*0x8 + 0x8]
14037ca9a:  48 8B 0C C7              MOV RCX,qword ptr [RDI + RAX*0x8]
14037ca9e:  85 D2                    TEST EDX,EDX
14037caa0:  79 17                    JNS 0x14037cab9
14037caa2:  48 85 C9                 TEST RCX,RCX
14037caa5:  74 44                    JZ 0x14037caeb
14037caa7:  48 83 CA FF              OR RDX,-0x1
14037caab:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
14037cab0:  48 FF C2                 INC RDX
14037cab3:  80 3C 11 00              CMP byte ptr [RCX + RDX*0x1],0x0
14037cab7:  75 F7                    JNZ 0x14037cab0
14037cab9:  48 85 C9                 TEST RCX,RCX
14037cabc:  74 2D                    JZ 0x14037caeb
14037cabe:  41 3B D4                 CMP EDX,R12D
14037cac1:  75 28                    JNZ 0x14037caeb
14037cac3:  4C 63 C2                 MOVSXD R8,EDX
14037cac6:  85 D2                    TEST EDX,EDX
14037cac8:  7E 3C                    JLE 0x14037cb06
14037caca:  48 8B D1                 MOV RDX,RCX
14037cacd:  48 F7 DA                 NEG RDX
14037cad0:  4C 8D 0C 1A              LEA R9,[RDX + RBX*0x1]
14037cad4:  0F B6 01                 MOVZX EAX,byte ptr [RCX]
14037cad7:  41 38 04 09              CMP byte ptr [R9 + RCX*0x1],AL
14037cadb:  75 0E                    JNZ 0x14037caeb
14037cadd:  48 FF C1                 INC RCX
14037cae0:  48 8D 04 0A              LEA RAX,[RDX + RCX*0x1]
14037cae4:  49 3B C0                 CMP RAX,R8
14037cae7:  7D 1D                    JGE 0x14037cb06
14037cae9:  EB E9                    JMP 0x14037cad4
14037caeb:  41 FF C2                 INC R10D
14037caee:  45 3B D3                 CMP R10D,R11D
14037caf1:  72 9D                    JC 0x14037ca90
14037caf3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037caf8:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cafc:  E8 EF 2B 0D 00           CALL 0x14044f6f0
14037cb01:  48 8B 5C 24 50           MOV RBX,qword ptr [RSP + 0x50]
14037cb06:  48 85 DB                 TEST RBX,RBX
14037cb09:  74 08                    JZ 0x14037cb13
14037cb0b:  48 8B CB                 MOV RCX,RBX
14037cb0e:  E8 CD 14 4D 00           CALL 0x14084dfe0
14037cb13:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037cb18:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037cb1d:  4C 8D 05 54 25 54 00     LEA R8,[0x1408bf078]
14037cb24:  83 CA FF                 OR EDX,0xffffffff
14037cb27:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cb2c:  E8 EF E1 ED FF           CALL 0x14025ad20
14037cb31:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cb36:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cb3a:  E8 A1 E1 0C 00           CALL 0x14044ace0
14037cb3f:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
14037cb44:  48 85 C9                 TEST RCX,RCX
14037cb47:  74 05                    JZ 0x14037cb4e
14037cb49:  E8 92 14 4D 00           CALL 0x14084dfe0
14037cb4e:  4C 89 74 24 50           MOV qword ptr [RSP + 0x50],R14
14037cb53:  44 89 74 24 58           MOV dword ptr [RSP + 0x58],R14D
14037cb58:  41 83 C8 FF              OR R8D,0xffffffff
14037cb5c:  48 8D 15 F5 24 54 00     LEA RDX,[0x1408bf058]
14037cb63:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cb68:  E8 73 E2 ED FF           CALL 0x14025ade0
14037cb6d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cb72:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cb76:  E8 65 E1 0C 00           CALL 0x14044ace0
14037cb7b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cb80:  E8 3B 4F C8 FF           CALL 0x140001ac0
14037cb85:  41 83 C8 FF              OR R8D,0xffffffff
14037cb89:  48 8D 15 A8 24 54 00     LEA RDX,[0x1408bf038]
14037cb90:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cb95:  E8 16 E2 ED FF           CALL 0x14025adb0
14037cb9a:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cb9f:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cba3:  E8 38 E1 0C 00           CALL 0x14044ace0
14037cba8:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cbad:  E8 0E 4F C8 FF           CALL 0x140001ac0
14037cbb2:  F3 0F 10 35 CE 54 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037cbba:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037cbbf:  BA EC 05 00 00           MOV EDX,0x5ec
14037cbc4:  48 8D 0D C5 A9 61 00     LEA RCX,[0x140997590]
14037cbcb:  FF 15 BF FC 4E 00        CALL qword ptr [0x14086c890]
14037cbd1:  0F 28 DF                 MOVAPS XMM3,XMM7
14037cbd4:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037cbd8:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037cbdd:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037cbe2:  48 8D 05 2F 23 54 00     LEA RAX,[0x1408bef18]
14037cbe9:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037cbee:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037cbf6:  66 41 0F 7E D9           MOVD R9D,XMM3
14037cbfb:  0F 28 D6                 MOVAPS XMM2,XMM6
14037cbfe:  66 41 0F 7E D0           MOVD R8D,XMM2
14037cc03:  49 8B D7                 MOV RDX,R15
14037cc06:  48 8D 0D A3 A3 61 00     LEA RCX,[0x140996fb0]
14037cc0d:  FF 15 6D FC 4E 00        CALL qword ptr [0x14086c880]
14037cc13:  F3 0F 10 95 30 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x130]
14037cc1b:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037cc1e:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037cc23:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037cc28:  45 33 C9                 XOR R9D,R9D
14037cc2b:  48 8D 95 48 03 00 00     LEA RDX,[RBP + 0x348]
14037cc32:  48 8B CE                 MOV RCX,RSI
14037cc35:  E8 26 A0 FB FF           CALL 0x140336c60
14037cc3a:  90                       NOP
14037cc3b:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037cc3e:  F3 0F 10 15 42 54 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037cc46:  0F 28 C2                 MOVAPS XMM0,XMM2
14037cc49:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037cc4e:  0F 28 DF                 MOVAPS XMM3,XMM7
14037cc51:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037cc55:  F3 0F 10 35 0B DD 58 00  MOVSS XMM6,dword ptr [0x14090a968]
14037cc5d:  F3 0F 59 D6              MULSS XMM2,XMM6
14037cc61:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037cc67:  66 0F 6E 05 1D 86 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037cc6f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cc72:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037cc76:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037cc7b:  48 8D 05 8E 22 54 00     LEA RAX,[0x1408bef10]
14037cc82:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037cc87:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037cc8f:  66 41 0F 7E D9           MOVD R9D,XMM3
14037cc94:  66 41 0F 7E D0           MOVD R8D,XMM2
14037cc99:  49 8B D7                 MOV RDX,R15
14037cc9c:  48 8D 0D 0D A3 61 00     LEA RCX,[0x140996fb0]
14037cca3:  FF 15 CF FB 4E 00        CALL qword ptr [0x14086c878]
14037cca9:  90                       NOP
14037ccaa:  48 8D 8D 48 03 00 00     LEA RCX,[RBP + 0x348]
14037ccb1:  E8 0A 4E C8 FF           CALL 0x140001ac0
14037ccb6:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037ccbd:  75 0A                    JNZ 0x14037ccc9
14037ccbf:  F3 0F 10 85 38 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x138]
14037ccc7:  EB 08                    JMP 0x14037ccd1
14037ccc9:  F3 0F 10 85 3C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x13c]
14037ccd1:  41 0F 57 C3              XORPS XMM0,XMM11
14037ccd5:  0F 57 C9                 XORPS XMM1,XMM1
14037ccd8:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14037ccdc:  41 B1 01                 MOV R9B,0x1
14037ccdf:  45 33 C0                 XOR R8D,R8D
14037cce2:  48 8B CE                 MOV RCX,RSI
14037cce5:  E8 36 97 FB FF           CALL 0x140336420
14037ccea:  F3 0F 10 15 96 53 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037ccf2:  0F 28 C2                 MOVAPS XMM0,XMM2
14037ccf5:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037ccfa:  F3 0F 5C F8              SUBSS XMM7,XMM0
14037ccfe:  F3 0F 59 D6              MULSS XMM2,XMM6
14037cd02:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037cd08:  66 0F 6E 05 7C 85 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037cd10:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cd13:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037cd17:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037cd1c:  48 8D 05 D5 20 54 00     LEA RAX,[0x1408bedf8]
14037cd23:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037cd28:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037cd30:  0F 28 DF                 MOVAPS XMM3,XMM7
14037cd33:  66 41 0F 7E D9           MOVD R9D,XMM3
14037cd38:  66 41 0F 7E D0           MOVD R8D,XMM2
14037cd3d:  49 8B D7                 MOV RDX,R15
14037cd40:  48 8D 0D 69 A2 61 00     LEA RCX,[0x140996fb0]
14037cd47:  FF 15 2B FB 4E 00        CALL qword ptr [0x14086c878]
14037cd4d:  F3 0F 10 0D 33 53 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037cd55:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037cd5a:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037cd5e:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037cd62:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037cd66:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037cd6a:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
14037cd70:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037cd75:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037cd79:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037cd7d:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037cd81:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037cd86:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037cd8c:  41 B8 06 00 00 00        MOV R8D,0x6
14037cd92:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cd97:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037cd9e:  E8 9D 97 0C 00           CALL 0x140446540
14037cda3:  44 8B 74 24 60           MOV R14D,dword ptr [RSP + 0x60]
14037cda8:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037cdad:  0F 84 C4 02 00 00        JZ 0x14037d077
14037cdb3:  66 41 0F 6E C6           MOVD XMM0,R14D
14037cdb8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cdbb:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037cdc0:  0F 2F C8                 COMISS XMM1,XMM0
14037cdc3:  0F 86 AE 02 00 00        JBE 0x14037d077
14037cdc9:  66 41 0F 6E C5           MOVD XMM0,R13D
14037cdce:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cdd1:  0F 2F C1                 COMISS XMM0,XMM1
14037cdd4:  0F 86 9D 02 00 00        JBE 0x14037d077
14037cdda:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037cddf:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037cde4:  66 0F 6E C3              MOVD XMM0,EBX
14037cde8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cdeb:  0F 2F C8                 COMISS XMM1,XMM0
14037cdee:  0F 86 83 02 00 00        JBE 0x14037d077
14037cdf4:  66 0F 6E C7              MOVD XMM0,EDI
14037cdf8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037cdfb:  0F 2F C1                 COMISS XMM0,XMM1
14037cdfe:  0F 86 73 02 00 00        JBE 0x14037d077
14037ce04:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037ce0b:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037ce0e:  48 8D 0D BB DE 66 00     LEA RCX,[0x1409eacd0]
14037ce15:  FF 15 D5 F8 4E 00        CALL qword ptr [0x14086c6f0]
14037ce1b:  4C 8B C0                 MOV R8,RAX
14037ce1e:  33 D2                    XOR EDX,EDX
14037ce20:  48 8B CB                 MOV RCX,RBX
14037ce23:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037ce26:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037ce2d:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037ce31:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037ce35:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037ce39:  48 8D 8D 40 09 00 00     LEA RCX,[RBP + 0x940]
14037ce40:  FF 15 DA F1 4E 00        CALL qword ptr [0x14086c020]
14037ce46:  48 8B D0                 MOV RDX,RAX
14037ce49:  48 8D 0D 10 12 64 00     LEA RCX,[0x1409be060]
14037ce50:  FF 15 42 FA 4E 00        CALL qword ptr [0x14086c898]
14037ce56:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037ce5a:  48 8D 0D FF 11 64 00     LEA RCX,[0x1409be060]
14037ce61:  FF 15 F1 FA 4E 00        CALL qword ptr [0x14086c958]
14037ce67:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037ce6d:  F3 0F 10 15 13 52 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037ce75:  F3 0F 59 15 AB D9 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037ce7d:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037ce84:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037ce8b:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037ce92:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037ce99:  0F 28 DA                 MOVAPS XMM3,XMM2
14037ce9c:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cea1:  48 8D 0D B8 11 64 00     LEA RCX,[0x1409be060]
14037cea8:  FF 15 AA F9 4E 00        CALL qword ptr [0x14086c858]
14037ceae:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037ceb2:  48 8D 0D A7 11 64 00     LEA RCX,[0x1409be060]
14037ceb9:  FF 15 99 FA 4E 00        CALL qword ptr [0x14086c958]
14037cebf:  48 8D 0D CA 7C 6D 00     LEA RCX,[0x140a54b90]
14037cec6:  FF 15 EC F9 4E 00        CALL qword ptr [0x14086c8b8]
14037cecc:  84 C0                    TEST AL,AL
14037cece:  74 1C                    JZ 0x14037ceec
14037ced0:  41 B0 01                 MOV R8B,0x1
14037ced3:  48 8D 15 26 20 54 00     LEA RDX,[0x1408bef00]
14037ceda:  48 8B CE                 MOV RCX,RSI
14037cedd:  E8 EE 2B 08 00           CALL 0x1403ffad0
14037cee2:  C7 86 D4 C7 00 00 06 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x6
14037ceec:  41 B4 01                 MOV R12B,0x1
14037ceef:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037cef6:  0F 85 B6 00 00 00        JNZ 0x14037cfb2
14037cefc:  41 83 C8 FF              OR R8D,0xffffffff
14037cf00:  48 8D 15 89 20 54 00     LEA RDX,[0x1408bef90]
14037cf07:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf0c:  E8 9F DE ED FF           CALL 0x14025adb0
14037cf11:  90                       NOP
14037cf12:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cf17:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cf1b:  E8 D0 DE 0C 00           CALL 0x14044adf0
14037cf20:  90                       NOP
14037cf21:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf26:  E8 95 4B C8 FF           CALL 0x140001ac0
14037cf2b:  41 83 C8 FF              OR R8D,0xffffffff
14037cf2f:  48 8D 15 32 20 54 00     LEA RDX,[0x1408bef68]
14037cf36:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf3b:  E8 70 DE ED FF           CALL 0x14025adb0
14037cf40:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cf45:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cf49:  E8 92 DD 0C 00           CALL 0x14044ace0
14037cf4e:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf53:  E8 68 4B C8 FF           CALL 0x140001ac0
14037cf58:  41 83 C8 FF              OR R8D,0xffffffff
14037cf5c:  48 8D 15 E5 1F 54 00     LEA RDX,[0x1408bef48]
14037cf63:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf68:  E8 43 DE ED FF           CALL 0x14025adb0
14037cf6d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cf72:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cf76:  E8 65 DD 0C 00           CALL 0x14044ace0
14037cf7b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf80:  E8 3B 4B C8 FF           CALL 0x140001ac0
14037cf85:  41 83 C8 FF              OR R8D,0xffffffff
14037cf89:  48 8D 15 98 1F 54 00     LEA RDX,[0x1408bef28]
14037cf90:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cf95:  E8 16 DE ED FF           CALL 0x14025adb0
14037cf9a:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cf9f:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cfa3:  E8 38 DD 0C 00           CALL 0x14044ace0
14037cfa8:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cfad:  E8 0E 4B C8 FF           CALL 0x140001ac0
14037cfb2:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037cfb9:  0F 85 BC 00 00 00        JNZ 0x14037d07b
14037cfbf:  41 83 C8 FF              OR R8D,0xffffffff
14037cfc3:  48 8D 15 6E 22 54 00     LEA RDX,[0x1408bf238]
14037cfca:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cfcf:  E8 DC DD ED FF           CALL 0x14025adb0
14037cfd4:  90                       NOP
14037cfd5:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037cfda:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037cfde:  E8 0D DE 0C 00           CALL 0x14044adf0
14037cfe3:  90                       NOP
14037cfe4:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cfe9:  E8 D2 4A C8 FF           CALL 0x140001ac0
14037cfee:  41 83 C8 FF              OR R8D,0xffffffff
14037cff2:  48 8D 15 17 22 54 00     LEA RDX,[0x1408bf210]
14037cff9:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037cffe:  E8 AD DD ED FF           CALL 0x14025adb0
14037d003:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d008:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d00c:  E8 CF DC 0C 00           CALL 0x14044ace0
14037d011:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d016:  E8 A5 4A C8 FF           CALL 0x140001ac0
14037d01b:  41 83 C8 FF              OR R8D,0xffffffff
14037d01f:  48 8D 15 CA 21 54 00     LEA RDX,[0x1408bf1f0]
14037d026:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d02b:  E8 80 DD ED FF           CALL 0x14025adb0
14037d030:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d035:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d039:  E8 A2 DC 0C 00           CALL 0x14044ace0
14037d03e:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d043:  E8 78 4A C8 FF           CALL 0x140001ac0
14037d048:  41 83 C8 FF              OR R8D,0xffffffff
14037d04c:  48 8D 15 7D 21 54 00     LEA RDX,[0x1408bf1d0]
14037d053:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d058:  E8 53 DD ED FF           CALL 0x14025adb0
14037d05d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d062:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d066:  E8 75 DC 0C 00           CALL 0x14044ace0
14037d06b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d070:  E8 4B 4A C8 FF           CALL 0x140001ac0
14037d075:  EB 04                    JMP 0x14037d07b
14037d077:  44 8B 65 AC              MOV R12D,dword ptr [RBP + -0x54]
14037d07b:  F3 0F 10 35 05 50 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037d083:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037d088:  BA ED 05 00 00           MOV EDX,0x5ed
14037d08d:  48 8D 0D FC A4 61 00     LEA RCX,[0x140997590]
14037d094:  FF 15 F6 F7 4E 00        CALL qword ptr [0x14086c890]
14037d09a:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037d09e:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037d0a2:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037d0a7:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d0ac:  48 8D 05 C5 21 54 00     LEA RAX,[0x1408bf278]
14037d0b3:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d0b8:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d0c0:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d0c5:  0F 28 D6                 MOVAPS XMM2,XMM6
14037d0c8:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d0cd:  49 8B D7                 MOV RDX,R15
14037d0d0:  48 8D 0D D9 9E 61 00     LEA RCX,[0x140996fb0]
14037d0d7:  FF 15 A3 F7 4E 00        CALL qword ptr [0x14086c880]
14037d0dd:  F3 0F 10 95 40 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x140]
14037d0e5:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037d0e8:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037d0ed:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037d0f2:  45 33 C9                 XOR R9D,R9D
14037d0f5:  48 8D 95 C8 02 00 00     LEA RDX,[RBP + 0x2c8]
14037d0fc:  48 8B CE                 MOV RCX,RSI
14037d0ff:  E8 5C 9B FB FF           CALL 0x140336c60
14037d104:  90                       NOP
14037d105:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037d108:  F3 0F 10 15 78 4F 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037d110:  0F 28 C2                 MOVAPS XMM0,XMM2
14037d113:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037d118:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037d11c:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037d120:  F3 0F 10 35 40 D8 58 00  MOVSS XMM6,dword ptr [0x14090a968]
14037d128:  F3 0F 59 D6              MULSS XMM2,XMM6
14037d12c:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037d132:  66 0F 6E 05 52 81 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037d13a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d13d:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037d141:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037d146:  48 8D 05 23 21 54 00     LEA RAX,[0x1408bf270]
14037d14d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d152:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d15a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d15f:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d164:  49 8B D7                 MOV RDX,R15
14037d167:  48 8D 0D 42 9E 61 00     LEA RCX,[0x140996fb0]
14037d16e:  FF 15 04 F7 4E 00        CALL qword ptr [0x14086c878]
14037d174:  90                       NOP
14037d175:  48 8D 8D C8 02 00 00     LEA RCX,[RBP + 0x2c8]
14037d17c:  E8 3F 49 C8 FF           CALL 0x140001ac0
14037d181:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037d188:  75 0A                    JNZ 0x14037d194
14037d18a:  F3 0F 10 85 48 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x148]
14037d192:  EB 08                    JMP 0x14037d19c
14037d194:  F3 0F 10 85 4C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x14c]
14037d19c:  41 0F 57 C3              XORPS XMM0,XMM11
14037d1a0:  0F 57 C9                 XORPS XMM1,XMM1
14037d1a3:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14037d1a7:  41 B1 01                 MOV R9B,0x1
14037d1aa:  45 33 C0                 XOR R8D,R8D
14037d1ad:  48 8B CE                 MOV RCX,RSI
14037d1b0:  E8 6B 92 FB FF           CALL 0x140336420
14037d1b5:  F3 0F 10 15 CB 4E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037d1bd:  0F 28 C2                 MOVAPS XMM0,XMM2
14037d1c0:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037d1c5:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
14037d1ca:  F3 0F 59 D6              MULSS XMM2,XMM6
14037d1ce:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037d1d4:  66 0F 6E 05 B0 80 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037d1dc:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d1df:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037d1e3:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d1e8:  48 8D 05 79 20 54 00     LEA RAX,[0x1408bf268]
14037d1ef:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d1f4:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d1fc:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037d200:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d205:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d20a:  49 8B D7                 MOV RDX,R15
14037d20d:  48 8D 0D 9C 9D 61 00     LEA RCX,[0x140996fb0]
14037d214:  FF 15 5E F6 4E 00        CALL qword ptr [0x14086c878]
14037d21a:  F3 0F 10 0D 66 4E 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037d222:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037d227:  0F 28 C7                 MOVAPS XMM0,XMM7
14037d22a:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037d22e:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037d232:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037d236:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037d23d:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037d242:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037d247:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037d24b:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037d24f:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037d254:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037d25a:  41 B8 06 00 00 00        MOV R8D,0x6
14037d260:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d265:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037d26c:  E8 CF 92 0C 00           CALL 0x140446540
14037d271:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037d276:  0F 84 C2 02 00 00        JZ 0x14037d53e
14037d27c:  66 41 0F 6E C6           MOVD XMM0,R14D
14037d281:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d284:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037d289:  0F 2F C8                 COMISS XMM1,XMM0
14037d28c:  0F 86 AC 02 00 00        JBE 0x14037d53e
14037d292:  66 41 0F 6E C5           MOVD XMM0,R13D
14037d297:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d29a:  0F 2F C1                 COMISS XMM0,XMM1
14037d29d:  0F 86 9B 02 00 00        JBE 0x14037d53e
14037d2a3:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037d2a8:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037d2ad:  66 0F 6E C3              MOVD XMM0,EBX
14037d2b1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d2b4:  0F 2F C8                 COMISS XMM1,XMM0
14037d2b7:  0F 86 81 02 00 00        JBE 0x14037d53e
14037d2bd:  66 0F 6E C7              MOVD XMM0,EDI
14037d2c1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d2c4:  0F 2F C1                 COMISS XMM0,XMM1
14037d2c7:  0F 86 71 02 00 00        JBE 0x14037d53e
14037d2cd:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037d2d4:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037d2d7:  48 8D 0D F2 D9 66 00     LEA RCX,[0x1409eacd0]
14037d2de:  FF 15 0C F4 4E 00        CALL qword ptr [0x14086c6f0]
14037d2e4:  4C 8B C0                 MOV R8,RAX
14037d2e7:  33 D2                    XOR EDX,EDX
14037d2e9:  48 8B CB                 MOV RCX,RBX
14037d2ec:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037d2ef:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037d2f6:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037d2fa:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037d2fe:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037d302:  48 8D 8D 50 09 00 00     LEA RCX,[RBP + 0x950]
14037d309:  FF 15 11 ED 4E 00        CALL qword ptr [0x14086c020]
14037d30f:  48 8B D0                 MOV RDX,RAX
14037d312:  48 8D 0D 47 0D 64 00     LEA RCX,[0x1409be060]
14037d319:  FF 15 79 F5 4E 00        CALL qword ptr [0x14086c898]
14037d31f:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037d323:  48 8D 0D 36 0D 64 00     LEA RCX,[0x1409be060]
14037d32a:  FF 15 28 F6 4E 00        CALL qword ptr [0x14086c958]
14037d330:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037d336:  F3 0F 10 15 4A 4D 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037d33e:  F3 0F 59 15 E2 D4 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037d346:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037d34d:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037d354:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037d35b:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037d362:  0F 28 DA                 MOVAPS XMM3,XMM2
14037d365:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d36a:  48 8D 0D EF 0C 64 00     LEA RCX,[0x1409be060]
14037d371:  FF 15 E1 F4 4E 00        CALL qword ptr [0x14086c858]
14037d377:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037d37b:  48 8D 0D DE 0C 64 00     LEA RCX,[0x1409be060]
14037d382:  FF 15 D0 F5 4E 00        CALL qword ptr [0x14086c958]
14037d388:  48 8D 0D 01 78 6D 00     LEA RCX,[0x140a54b90]
14037d38f:  FF 15 23 F5 4E 00        CALL qword ptr [0x14086c8b8]
14037d395:  84 C0                    TEST AL,AL
14037d397:  74 1C                    JZ 0x14037d3b5
14037d399:  C7 86 D4 C7 00 00 08 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x8
14037d3a3:  41 B0 01                 MOV R8B,0x1
14037d3a6:  48 8D 15 AB 1E 54 00     LEA RDX,[0x1408bf258]
14037d3ad:  48 8B CE                 MOV RCX,RSI
14037d3b0:  E8 1B 27 08 00           CALL 0x1403ffad0
14037d3b5:  41 B4 01                 MOV R12B,0x1
14037d3b8:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037d3bf:  0F 85 B6 00 00 00        JNZ 0x14037d47b
14037d3c5:  41 83 C8 FF              OR R8D,0xffffffff
14037d3c9:  48 8D 15 58 1D 54 00     LEA RDX,[0x1408bf128]
14037d3d0:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d3d5:  E8 D6 D9 ED FF           CALL 0x14025adb0
14037d3da:  90                       NOP
14037d3db:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d3e0:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d3e4:  E8 07 DA 0C 00           CALL 0x14044adf0
14037d3e9:  90                       NOP
14037d3ea:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d3ef:  E8 CC 46 C8 FF           CALL 0x140001ac0
14037d3f4:  41 83 C8 FF              OR R8D,0xffffffff
14037d3f8:  48 8D 15 09 1D 54 00     LEA RDX,[0x1408bf108]
14037d3ff:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d404:  E8 A7 D9 ED FF           CALL 0x14025adb0
14037d409:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d40e:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d412:  E8 C9 D8 0C 00           CALL 0x14044ace0
14037d417:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d41c:  E8 9F 46 C8 FF           CALL 0x140001ac0
14037d421:  41 83 C8 FF              OR R8D,0xffffffff
14037d425:  48 8D 15 B4 1C 54 00     LEA RDX,[0x1408bf0e0]
14037d42c:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d431:  E8 7A D9 ED FF           CALL 0x14025adb0
14037d436:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d43b:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d43f:  E8 9C D8 0C 00           CALL 0x14044ace0
14037d444:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d449:  E8 72 46 C8 FF           CALL 0x140001ac0
14037d44e:  41 83 C8 FF              OR R8D,0xffffffff
14037d452:  48 8D 15 67 1C 54 00     LEA RDX,[0x1408bf0c0]
14037d459:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d45e:  E8 4D D9 ED FF           CALL 0x14025adb0
14037d463:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d468:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d46c:  E8 6F D8 0C 00           CALL 0x14044ace0
14037d471:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d476:  E8 45 46 C8 FF           CALL 0x140001ac0
14037d47b:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037d482:  0F 85 B6 00 00 00        JNZ 0x14037d53e
14037d488:  41 83 C8 FF              OR R8D,0xffffffff
14037d48c:  48 8D 15 1D 1D 54 00     LEA RDX,[0x1408bf1b0]
14037d493:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d498:  E8 13 D9 ED FF           CALL 0x14025adb0
14037d49d:  90                       NOP
14037d49e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d4a3:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d4a7:  E8 44 D9 0C 00           CALL 0x14044adf0
14037d4ac:  90                       NOP
14037d4ad:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d4b2:  E8 09 46 C8 FF           CALL 0x140001ac0
14037d4b7:  41 83 C8 FF              OR R8D,0xffffffff
14037d4bb:  48 8D 15 CE 1C 54 00     LEA RDX,[0x1408bf190]
14037d4c2:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d4c7:  E8 E4 D8 ED FF           CALL 0x14025adb0
14037d4cc:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d4d1:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d4d5:  E8 06 D8 0C 00           CALL 0x14044ace0
14037d4da:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d4df:  E8 DC 45 C8 FF           CALL 0x140001ac0
14037d4e4:  41 83 C8 FF              OR R8D,0xffffffff
14037d4e8:  48 8D 15 79 1C 54 00     LEA RDX,[0x1408bf168]
14037d4ef:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d4f4:  E8 B7 D8 ED FF           CALL 0x14025adb0
14037d4f9:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d4fe:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d502:  E8 D9 D7 0C 00           CALL 0x14044ace0
14037d507:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d50c:  E8 AF 45 C8 FF           CALL 0x140001ac0
14037d511:  41 83 C8 FF              OR R8D,0xffffffff
14037d515:  48 8D 15 2C 1C 54 00     LEA RDX,[0x1408bf148]
14037d51c:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d521:  E8 8A D8 ED FF           CALL 0x14025adb0
14037d526:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d52b:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d52f:  E8 AC D7 0C 00           CALL 0x14044ace0
14037d534:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d539:  E8 82 45 C8 FF           CALL 0x140001ac0
14037d53e:  F3 0F 10 35 42 4B 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037d546:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037d54b:  BA 81 06 00 00           MOV EDX,0x681
14037d550:  48 8D 0D 39 A0 61 00     LEA RCX,[0x140997590]
14037d557:  FF 15 33 F3 4E 00        CALL qword ptr [0x14086c890]
14037d55d:  0F 28 DF                 MOVAPS XMM3,XMM7
14037d560:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037d564:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037d569:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d56e:  48 8D 05 0B 20 54 00     LEA RAX,[0x1408bf580]
14037d575:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d57a:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d582:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d587:  0F 28 D6                 MOVAPS XMM2,XMM6
14037d58a:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d58f:  49 8B D7                 MOV RDX,R15
14037d592:  48 8D 0D 17 9A 61 00     LEA RCX,[0x140996fb0]
14037d599:  FF 15 E1 F2 4E 00        CALL qword ptr [0x14086c880]
14037d59f:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037d5a6:  75 0A                    JNZ 0x14037d5b2
14037d5a8:  F3 0F 10 85 54 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x154]
14037d5b0:  EB 08                    JMP 0x14037d5ba
14037d5b2:  F3 0F 10 85 58 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x158]
14037d5ba:  41 0F 57 C3              XORPS XMM0,XMM11
14037d5be:  0F 57 C9                 XORPS XMM1,XMM1
14037d5c1:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14037d5c5:  41 B1 01                 MOV R9B,0x1
14037d5c8:  45 33 C0                 XOR R8D,R8D
14037d5cb:  48 8B CE                 MOV RCX,RSI
14037d5ce:  E8 4D 8E FB FF           CALL 0x140336420
14037d5d3:  F3 0F 10 15 AD 4A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037d5db:  0F 28 C2                 MOVAPS XMM0,XMM2
14037d5de:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037d5e3:  F3 0F 5C F8              SUBSS XMM7,XMM0
14037d5e7:  F3 44 0F 10 1D 78 D3 58 00 MOVSS XMM11,dword ptr [0x14090a968]
14037d5f0:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037d5f5:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037d5fb:  66 0F 6E 05 89 7C 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037d603:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d606:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037d60a:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d60f:  48 8D 05 62 1F 54 00     LEA RAX,[0x1408bf578]
14037d616:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d61b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d623:  0F 28 DF                 MOVAPS XMM3,XMM7
14037d626:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d62b:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d630:  49 8B D7                 MOV RDX,R15
14037d633:  48 8D 0D 76 99 61 00     LEA RCX,[0x140996fb0]
14037d63a:  FF 15 38 F2 4E 00        CALL qword ptr [0x14086c878]
14037d640:  F3 0F 10 25 40 4A 61 00  MOVSS XMM4,dword ptr [0x140992088]
14037d648:  0F 28 FC                 MOVAPS XMM7,XMM4
14037d64b:  F3 0F 59 3D D5 D1 58 00  MULSS XMM7,dword ptr [0x14090a828]
14037d653:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037d658:  83 BE B8 05 00 00 00     CMP dword ptr [RSI + 0x5b8],0x0
14037d65f:  0F 8E 5C 11 00 00        JLE 0x14037e7c1
14037d665:  0F 28 DC                 MOVAPS XMM3,XMM4
14037d668:  F3 41 0F 59 D8           MULSS XMM3,XMM8
14037d66d:  0F 28 C7                 MOVAPS XMM0,XMM7
14037d670:  F3 0F 5C C3              SUBSS XMM0,XMM3
14037d674:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
14037d678:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
14037d67c:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037d683:  41 0F 28 CA              MOVAPS XMM1,XMM10
14037d687:  F3 0F 59 0D 19 D0 58 00  MULSS XMM1,dword ptr [0x14090a6a8]
14037d68f:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037d693:  F3 0F 58 CB              ADDSS XMM1,XMM3
14037d697:  F3 0F 2C D9              CVTTSS2SI EBX,XMM1
14037d69b:  89 5C 24 6C              MOV dword ptr [RSP + 0x6c],EBX
14037d69f:  66 0F 6E F0              MOVD XMM6,EAX
14037d6a3:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14037d6a6:  F3 0F 59 25 1E D2 58 00  MULSS XMM4,dword ptr [0x14090a8cc]
14037d6ae:  F3 0F 58 E6              ADDSS XMM4,XMM6
14037d6b2:  41 B9 06 00 00 00        MOV R9D,0x6
14037d6b8:  0F 28 D4                 MOVAPS XMM2,XMM4
14037d6bb:  F3 0F 10 4D B0           MOVSS XMM1,dword ptr [RBP + -0x50]
14037d6c0:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037d6c7:  E8 24 8B 0C 00           CALL 0x1404461f0
14037d6cc:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037d6d1:  0F 84 1F 02 00 00        JZ 0x14037d8f6
14037d6d7:  66 41 0F 6E C6           MOVD XMM0,R14D
14037d6dc:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d6df:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037d6e4:  0F 2F C8                 COMISS XMM1,XMM0
14037d6e7:  0F 86 09 02 00 00        JBE 0x14037d8f6
14037d6ed:  66 41 0F 6E C5           MOVD XMM0,R13D
14037d6f2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d6f5:  0F 2F C1                 COMISS XMM0,XMM1
14037d6f8:  0F 86 F8 01 00 00        JBE 0x14037d8f6
14037d6fe:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037d703:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037d708:  0F 2F CE                 COMISS XMM1,XMM6
14037d70b:  0F 86 E5 01 00 00        JBE 0x14037d8f6
14037d711:  66 0F 6E C3              MOVD XMM0,EBX
14037d715:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d718:  0F 2F C1                 COMISS XMM0,XMM1
14037d71b:  0F 86 D5 01 00 00        JBE 0x14037d8f6
14037d721:  41 B4 01                 MOV R12B,0x1
14037d724:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037d72b:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037d72e:  48 8D 0D 9B D5 66 00     LEA RCX,[0x1409eacd0]
14037d735:  FF 15 B5 EF 4E 00        CALL qword ptr [0x14086c6f0]
14037d73b:  4C 8B C0                 MOV R8,RAX
14037d73e:  33 D2                    XOR EDX,EDX
14037d740:  48 8B CB                 MOV RCX,RBX
14037d743:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037d746:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037d74d:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037d751:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037d755:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037d759:  48 8D 8D 60 09 00 00     LEA RCX,[RBP + 0x960]
14037d760:  FF 15 BA E8 4E 00        CALL qword ptr [0x14086c020]
14037d766:  48 8B D0                 MOV RDX,RAX
14037d769:  48 8D 0D F0 08 64 00     LEA RCX,[0x1409be060]
14037d770:  FF 15 22 F1 4E 00        CALL qword ptr [0x14086c898]
14037d776:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037d77b:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037d781:  F3 0F 10 15 FF 48 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037d789:  F3 0F 59 15 97 D1 58 00  MULSS XMM2,dword ptr [0x14090a928]
14037d791:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037d798:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037d79f:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037d7a6:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037d7ad:  0F 28 DA                 MOVAPS XMM3,XMM2
14037d7b0:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d7b5:  48 8D 0D A4 08 64 00     LEA RCX,[0x1409be060]
14037d7bc:  FF 15 96 F0 4E 00        CALL qword ptr [0x14086c858]
14037d7c2:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037d7c9:  0F 85 8D 00 00 00        JNZ 0x14037d85c
14037d7cf:  41 83 C8 FF              OR R8D,0xffffffff
14037d7d3:  48 8D 15 86 1D 54 00     LEA RDX,[0x1408bf560]
14037d7da:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d7df:  E8 CC D5 ED FF           CALL 0x14025adb0
14037d7e4:  90                       NOP
14037d7e5:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d7ea:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d7ee:  E8 FD D5 0C 00           CALL 0x14044adf0
14037d7f3:  90                       NOP
14037d7f4:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d7f9:  E8 C2 42 C8 FF           CALL 0x140001ac0
14037d7fe:  41 83 C8 FF              OR R8D,0xffffffff
14037d802:  48 8D 15 3F 1D 54 00     LEA RDX,[0x1408bf548]
14037d809:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d80e:  E8 9D D5 ED FF           CALL 0x14025adb0
14037d813:  90                       NOP
14037d814:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d819:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d81d:  E8 CE D5 0C 00           CALL 0x14044adf0
14037d822:  90                       NOP
14037d823:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d828:  E8 93 42 C8 FF           CALL 0x140001ac0
14037d82d:  41 83 C8 FF              OR R8D,0xffffffff
14037d831:  48 8D 15 A8 1D 54 00     LEA RDX,[0x1408bf5e0]
14037d838:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d83d:  E8 6E D5 ED FF           CALL 0x14025adb0
14037d842:  90                       NOP
14037d843:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d848:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d84c:  E8 9F D5 0C 00           CALL 0x14044adf0
14037d851:  90                       NOP
14037d852:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d857:  E8 64 42 C8 FF           CALL 0x140001ac0
14037d85c:  83 BE D0 C7 00 00 01     CMP dword ptr [RSI + 0xc7d0],0x1
14037d863:  0F 85 8D 00 00 00        JNZ 0x14037d8f6
14037d869:  41 83 C8 FF              OR R8D,0xffffffff
14037d86d:  48 8D 15 54 1D 54 00     LEA RDX,[0x1408bf5c8]
14037d874:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d879:  E8 32 D5 ED FF           CALL 0x14025adb0
14037d87e:  90                       NOP
14037d87f:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d884:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d888:  E8 63 D5 0C 00           CALL 0x14044adf0
14037d88d:  90                       NOP
14037d88e:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d893:  E8 28 42 C8 FF           CALL 0x140001ac0
14037d898:  41 83 C8 FF              OR R8D,0xffffffff
14037d89c:  48 8D 15 0D 1D 54 00     LEA RDX,[0x1408bf5b0]
14037d8a3:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d8a8:  E8 03 D5 ED FF           CALL 0x14025adb0
14037d8ad:  90                       NOP
14037d8ae:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d8b3:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d8b7:  E8 34 D5 0C 00           CALL 0x14044adf0
14037d8bc:  90                       NOP
14037d8bd:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d8c2:  E8 F9 41 C8 FF           CALL 0x140001ac0
14037d8c7:  41 83 C8 FF              OR R8D,0xffffffff
14037d8cb:  48 8D 15 BE 1C 54 00     LEA RDX,[0x1408bf590]
14037d8d2:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d8d7:  E8 D4 D4 ED FF           CALL 0x14025adb0
14037d8dc:  90                       NOP
14037d8dd:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037d8e2:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037d8e6:  E8 05 D5 0C 00           CALL 0x14044adf0
14037d8eb:  90                       NOP
14037d8ec:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037d8f1:  E8 CA 41 C8 FF           CALL 0x140001ac0
14037d8f6:  F3 0F 10 35 8A 47 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037d8fe:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037d903:  BA E8 10 00 00           MOV EDX,0x10e8
14037d908:  48 8D 0D 81 9C 61 00     LEA RCX,[0x140997590]
14037d90f:  FF 15 7B EF 4E 00        CALL qword ptr [0x14086c890]
14037d915:  0F 28 DF                 MOVAPS XMM3,XMM7
14037d918:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037d91c:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037d921:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d926:  48 8D 05 33 1B 54 00     LEA RAX,[0x1408bf460]
14037d92d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d932:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037d93a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d93f:  0F 28 D6                 MOVAPS XMM2,XMM6
14037d942:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d947:  49 8B D7                 MOV RDX,R15
14037d94a:  48 8D 0D 5F 96 61 00     LEA RCX,[0x140996fb0]
14037d951:  FF 15 29 EF 4E 00        CALL qword ptr [0x14086c880]
14037d957:  BA 77 06 00 00           MOV EDX,0x677
14037d95c:  48 8D 0D 2D 9C 61 00     LEA RCX,[0x140997590]
14037d963:  FF 15 27 EF 4E 00        CALL qword ptr [0x14086c890]
14037d969:  F3 0F 10 05 17 47 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037d971:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037d976:  0F 28 DF                 MOVAPS XMM3,XMM7
14037d979:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037d97d:  66 0F 6E 05 07 79 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037d985:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037d988:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037d98d:  F3 0F 10 54 24 70        MOVSS XMM2,dword ptr [RSP + 0x70]
14037d993:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037d997:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037d99c:  48 8D 05 AD 1A 54 00     LEA RAX,[0x1408bf450]
14037d9a3:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037d9a8:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037d9b0:  66 41 0F 7E D9           MOVD R9D,XMM3
14037d9b5:  66 41 0F 7E D0           MOVD R8D,XMM2
14037d9ba:  49 8B D7                 MOV RDX,R15
14037d9bd:  48 8D 0D EC 95 61 00     LEA RCX,[0x140996fb0]
14037d9c4:  FF 15 BE EE 4E 00        CALL qword ptr [0x14086c888]
14037d9ca:  48 8D 0D FF D2 66 00     LEA RCX,[0x1409eacd0]
14037d9d1:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037d9d8:  75 09                    JNZ 0x14037d9e3
14037d9da:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037d9e1:  EB 07                    JMP 0x14037d9ea
14037d9e3:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037d9ea:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037d9ed:  FF 15 FD EC 4E 00        CALL qword ptr [0x14086c6f0]
14037d9f3:  4C 8B C0                 MOV R8,RAX
14037d9f6:  33 D2                    XOR EDX,EDX
14037d9f8:  48 8B CB                 MOV RCX,RBX
14037d9fb:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037d9fe:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037da05:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037da09:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037da0d:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037da11:  48 8D 8D 70 09 00 00     LEA RCX,[RBP + 0x970]
14037da18:  FF 15 02 E6 4E 00        CALL qword ptr [0x14086c020]
14037da1e:  48 8B D0                 MOV RDX,RAX
14037da21:  48 8D 0D 38 06 64 00     LEA RCX,[0x1409be060]
14037da28:  FF 15 6A EE 4E 00        CALL qword ptr [0x14086c898]
14037da2e:  F3 0F 10 0D 52 46 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037da36:  F3 0F 59 0D CE CF 58 00  MULSS XMM1,dword ptr [0x14090aa0c]
14037da3e:  0F 28 D1                 MOVAPS XMM2,XMM1
14037da41:  48 8D 0D 18 06 64 00     LEA RCX,[0x1409be060]
14037da48:  FF 15 5A EE 4E 00        CALL qword ptr [0x14086c8a8]
14037da4e:  F3 0F 10 15 32 46 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037da56:  F3 0F 59 15 CA CD 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037da5e:  F3 0F 58 D7              ADDSS XMM2,XMM7
14037da62:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037da66:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
14037da6c:  48 8D 8D 78 04 00 00     LEA RCX,[RBP + 0x478]
14037da73:  FF 15 8F E5 4E 00        CALL qword ptr [0x14086c008]
14037da79:  48 8B D0                 MOV RDX,RAX
14037da7c:  48 8D 0D DD 05 64 00     LEA RCX,[0x1409be060]
14037da83:  FF 15 17 EE 4E 00        CALL qword ptr [0x14086c8a0]
14037da89:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037da8e:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037da95:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037da9c:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037daa0:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037daa4:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037daa8:  48 8D 0D B1 05 64 00     LEA RCX,[0x1409be060]
14037daaf:  FF 15 AB ED 4E 00        CALL qword ptr [0x14086c860]
14037dab5:  F3 0F 10 75 BC           MOVSS XMM6,dword ptr [RBP + -0x44]
14037daba:  F3 0F 58 FE              ADDSS XMM7,XMM6
14037dabe:  8B 05 AC 77 61 00        MOV EAX,dword ptr [0x140995270]
14037dac4:  83 F8 02                 CMP EAX,0x2
14037dac7:  75 09                    JNZ 0x14037dad2
14037dac9:  48 8D 05 E0 18 54 00     LEA RAX,[0x1408bf3b0]
14037dad0:  EB 13                    JMP 0x14037dae5
14037dad2:  83 F8 01                 CMP EAX,0x1
14037dad5:  48 8D 05 C4 17 54 00     LEA RAX,[0x1408bf2a0]
14037dadc:  74 07                    JZ 0x14037dae5
14037dade:  48 8D 05 9B 19 54 00     LEA RAX,[0x1408bf480]
14037dae5:  F3 0F 10 15 9B 45 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037daed:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037daf2:  0F 28 DA                 MOVAPS XMM3,XMM2
14037daf5:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037dafa:  F3 0F 58 DF              ADDSS XMM3,XMM7
14037dafe:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
14037db03:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037db0b:  66 41 0F 7E D9           MOVD R9D,XMM3
14037db10:  66 41 0F 7E D0           MOVD R8D,XMM2
14037db15:  49 8B D7                 MOV RDX,R15
14037db18:  48 8D 0D 91 94 61 00     LEA RCX,[0x140996fb0]
14037db1f:  FF 15 5B ED 4E 00        CALL qword ptr [0x14086c880]
14037db25:  F3 0F 58 FE              ADDSS XMM7,XMM6
14037db29:  F3 0F 10 0D 57 45 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037db31:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037db36:  0F 28 C7                 MOVAPS XMM0,XMM7
14037db39:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037db3d:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037db41:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037db45:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037db4a:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037db4f:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037db53:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037db57:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037db5c:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037db62:  41 B8 06 00 00 00        MOV R8D,0x6
14037db68:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037db6d:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037db74:  E8 C7 89 0C 00           CALL 0x140446540
14037db79:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037db7e:  0F 84 3D 02 00 00        JZ 0x14037ddc1
14037db84:  66 41 0F 6E C6           MOVD XMM0,R14D
14037db89:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037db8c:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037db91:  0F 2F C8                 COMISS XMM1,XMM0
14037db94:  0F 86 27 02 00 00        JBE 0x14037ddc1
14037db9a:  66 41 0F 6E C5           MOVD XMM0,R13D
14037db9f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dba2:  0F 2F C1                 COMISS XMM0,XMM1
14037dba5:  0F 86 16 02 00 00        JBE 0x14037ddc1
14037dbab:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037dbb0:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037dbb5:  66 0F 6E C3              MOVD XMM0,EBX
14037dbb9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dbbc:  0F 2F C8                 COMISS XMM1,XMM0
14037dbbf:  0F 86 FC 01 00 00        JBE 0x14037ddc1
14037dbc5:  66 0F 6E C7              MOVD XMM0,EDI
14037dbc9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dbcc:  0F 2F C1                 COMISS XMM0,XMM1
14037dbcf:  0F 86 EC 01 00 00        JBE 0x14037ddc1
14037dbd5:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037dbdc:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037dbdf:  48 8D 0D EA D0 66 00     LEA RCX,[0x1409eacd0]
14037dbe6:  FF 15 04 EB 4E 00        CALL qword ptr [0x14086c6f0]
14037dbec:  4C 8B C0                 MOV R8,RAX
14037dbef:  33 D2                    XOR EDX,EDX
14037dbf1:  48 8B CB                 MOV RCX,RBX
14037dbf4:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037dbf7:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037dbfe:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037dc02:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037dc06:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037dc0a:  48 8D 8D 80 09 00 00     LEA RCX,[RBP + 0x980]
14037dc11:  FF 15 09 E4 4E 00        CALL qword ptr [0x14086c020]
14037dc17:  48 8B D0                 MOV RDX,RAX
14037dc1a:  48 8D 0D 3F 04 64 00     LEA RCX,[0x1409be060]
14037dc21:  FF 15 71 EC 4E 00        CALL qword ptr [0x14086c898]
14037dc27:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037dc2b:  48 8D 0D 2E 04 64 00     LEA RCX,[0x1409be060]
14037dc32:  FF 15 20 ED 4E 00        CALL qword ptr [0x14086c958]
14037dc38:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037dc3e:  F3 0F 10 15 42 44 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037dc46:  F3 0F 59 15 DA CB 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037dc4e:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037dc55:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037dc5c:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037dc63:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037dc6a:  0F 28 DA                 MOVAPS XMM3,XMM2
14037dc6d:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037dc72:  48 8D 0D E7 03 64 00     LEA RCX,[0x1409be060]
14037dc79:  FF 15 D9 EB 4E 00        CALL qword ptr [0x14086c858]
14037dc7f:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037dc83:  48 8D 0D D6 03 64 00     LEA RCX,[0x1409be060]
14037dc8a:  FF 15 C8 EC 4E 00        CALL qword ptr [0x14086c958]
14037dc90:  48 8D 0D F9 6E 6D 00     LEA RCX,[0x140a54b90]
14037dc97:  FF 15 1B EC 4E 00        CALL qword ptr [0x14086c8b8]
14037dc9d:  84 C0                    TEST AL,AL
14037dc9f:  74 1C                    JZ 0x14037dcbd
14037dca1:  41 B0 01                 MOV R8B,0x1
14037dca4:  48 8D 15 C5 17 54 00     LEA RDX,[0x1408bf470]
14037dcab:  48 8B CE                 MOV RCX,RSI
14037dcae:  E8 1D 1E 08 00           CALL 0x1403ffad0
14037dcb3:  C7 86 D4 C7 00 00 46 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x46
14037dcbd:  41 B4 01                 MOV R12B,0x1
14037dcc0:  8B 86 D0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d0]
14037dcc6:  85 C0                    TEST EAX,EAX
14037dcc8:  75 65                    JNZ 0x14037dd2f
14037dcca:  41 83 C8 FF              OR R8D,0xffffffff
14037dcce:  48 8D 15 C3 16 54 00     LEA RDX,[0x1408bf398]
14037dcd5:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dcda:  E8 D1 D0 ED FF           CALL 0x14025adb0
14037dcdf:  90                       NOP
14037dce0:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037dce5:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037dce9:  E8 02 D1 0C 00           CALL 0x14044adf0
14037dcee:  90                       NOP
14037dcef:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dcf4:  E8 C7 3D C8 FF           CALL 0x140001ac0
14037dcf9:  41 83 C8 FF              OR R8D,0xffffffff
14037dcfd:  48 8D 15 84 15 54 00     LEA RDX,[0x1408bf288]
14037dd04:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd09:  E8 A2 D0 ED FF           CALL 0x14025adb0
14037dd0e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037dd13:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037dd17:  E8 C4 CF 0C 00           CALL 0x14044ace0
14037dd1c:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd21:  E8 9A 3D C8 FF           CALL 0x140001ac0
14037dd26:  48 8D 15 DB 19 54 00     LEA RDX,[0x1408bf708]
14037dd2d:  EB 6C                    JMP 0x14037dd9b
14037dd2f:  83 F8 01                 CMP EAX,0x1
14037dd32:  0F 85 89 00 00 00        JNZ 0x14037ddc1
14037dd38:  41 83 C8 FF              OR R8D,0xffffffff
14037dd3c:  48 8D 15 AD 19 54 00     LEA RDX,[0x1408bf6f0]
14037dd43:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd48:  E8 63 D0 ED FF           CALL 0x14025adb0
14037dd4d:  90                       NOP
14037dd4e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037dd53:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037dd57:  E8 94 D0 0C 00           CALL 0x14044adf0
14037dd5c:  90                       NOP
14037dd5d:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd62:  E8 59 3D C8 FF           CALL 0x140001ac0
14037dd67:  41 83 C8 FF              OR R8D,0xffffffff
14037dd6b:  48 8D 15 66 19 54 00     LEA RDX,[0x1408bf6d8]
14037dd72:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd77:  E8 34 D0 ED FF           CALL 0x14025adb0
14037dd7c:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037dd81:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037dd85:  E8 56 CF 0C 00           CALL 0x14044ace0
14037dd8a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dd8f:  E8 2C 3D C8 FF           CALL 0x140001ac0
14037dd94:  48 8D 15 1D 19 54 00     LEA RDX,[0x1408bf6b8]
14037dd9b:  41 83 C8 FF              OR R8D,0xffffffff
14037dd9f:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037dda4:  E8 07 D0 ED FF           CALL 0x14025adb0
14037dda9:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ddae:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037ddb2:  E8 29 CF 0C 00           CALL 0x14044ace0
14037ddb7:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ddbc:  E8 FF 3C C8 FF           CALL 0x140001ac0
14037ddc1:  F3 0F 10 35 BF 42 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037ddc9:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037ddce:  BA EB 05 00 00           MOV EDX,0x5eb
14037ddd3:  48 8D 0D B6 97 61 00     LEA RCX,[0x140997590]
14037ddda:  FF 15 B0 EA 4E 00        CALL qword ptr [0x14086c890]
14037dde0:  0F 28 DF                 MOVAPS XMM3,XMM7
14037dde3:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037dde7:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037ddec:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037ddf1:  48 8D 05 50 19 54 00     LEA RAX,[0x1408bf748]
14037ddf8:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037ddfd:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037de05:  66 41 0F 7E D9           MOVD R9D,XMM3
14037de0a:  0F 28 D6                 MOVAPS XMM2,XMM6
14037de0d:  66 41 0F 7E D0           MOVD R8D,XMM2
14037de12:  49 8B D7                 MOV RDX,R15
14037de15:  48 8D 0D 94 91 61 00     LEA RCX,[0x140996fb0]
14037de1c:  FF 15 5E EA 4E 00        CALL qword ptr [0x14086c880]
14037de22:  F3 0F 10 95 64 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x164]
14037de2a:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037de2d:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037de32:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037de37:  45 33 C9                 XOR R9D,R9D
14037de3a:  48 8D 95 D8 02 00 00     LEA RDX,[RBP + 0x2d8]
14037de41:  48 8B CE                 MOV RCX,RSI
14037de44:  E8 17 8E FB FF           CALL 0x140336c60
14037de49:  90                       NOP
14037de4a:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037de4d:  F3 0F 10 15 33 42 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037de55:  0F 28 C2                 MOVAPS XMM0,XMM2
14037de58:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037de5d:  0F 28 DF                 MOVAPS XMM3,XMM7
14037de60:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037de64:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037de69:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037de6f:  66 0F 6E 05 15 74 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037de77:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037de7a:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037de7e:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037de83:  48 8D 05 B6 18 54 00     LEA RAX,[0x1408bf740]
14037de8a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037de8f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037de97:  66 41 0F 7E D9           MOVD R9D,XMM3
14037de9c:  66 41 0F 7E D0           MOVD R8D,XMM2
14037dea1:  49 8B D7                 MOV RDX,R15
14037dea4:  48 8D 0D 05 91 61 00     LEA RCX,[0x140996fb0]
14037deab:  FF 15 C7 E9 4E 00        CALL qword ptr [0x14086c878]
14037deb1:  90                       NOP
14037deb2:  48 8D 8D D8 02 00 00     LEA RCX,[RBP + 0x2d8]
14037deb9:  E8 02 3C C8 FF           CALL 0x140001ac0
14037debe:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037dec5:  75 0A                    JNZ 0x14037ded1
14037dec7:  F3 0F 10 85 70 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x170]
14037decf:  EB 08                    JMP 0x14037ded9
14037ded1:  F3 0F 10 85 7C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x17c]
14037ded9:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037dedc:  41 B1 01                 MOV R9B,0x1
14037dedf:  45 33 C0                 XOR R8D,R8D
14037dee2:  48 8B CE                 MOV RCX,RSI
14037dee5:  E8 36 85 FB FF           CALL 0x140336420
14037deea:  F3 0F 10 15 96 41 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037def2:  0F 28 C2                 MOVAPS XMM0,XMM2
14037def5:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037defa:  F3 0F 5C F8              SUBSS XMM7,XMM0
14037defe:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037df03:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037df09:  66 0F 6E 05 7B 73 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037df11:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037df14:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037df18:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037df1d:  48 8D 05 14 18 54 00     LEA RAX,[0x1408bf738]
14037df24:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037df29:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037df31:  0F 28 DF                 MOVAPS XMM3,XMM7
14037df34:  66 41 0F 7E D9           MOVD R9D,XMM3
14037df39:  66 41 0F 7E D0           MOVD R8D,XMM2
14037df3e:  49 8B D7                 MOV RDX,R15
14037df41:  48 8D 0D 68 90 61 00     LEA RCX,[0x140996fb0]
14037df48:  FF 15 2A E9 4E 00        CALL qword ptr [0x14086c878]
14037df4e:  F3 0F 10 0D 32 41 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037df56:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037df5b:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037df5f:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037df63:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037df67:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037df6b:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
14037df71:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037df76:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037df7a:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037df7e:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037df82:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037df87:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037df8d:  41 B8 06 00 00 00        MOV R8D,0x6
14037df93:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037df98:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037df9f:  E8 9C 85 0C 00           CALL 0x140446540
14037dfa4:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037dfa9:  0F 84 3D 02 00 00        JZ 0x14037e1ec
14037dfaf:  66 41 0F 6E C6           MOVD XMM0,R14D
14037dfb4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dfb7:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037dfbc:  0F 2F C8                 COMISS XMM1,XMM0
14037dfbf:  0F 86 27 02 00 00        JBE 0x14037e1ec
14037dfc5:  66 41 0F 6E C5           MOVD XMM0,R13D
14037dfca:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dfcd:  0F 2F C1                 COMISS XMM0,XMM1
14037dfd0:  0F 86 16 02 00 00        JBE 0x14037e1ec
14037dfd6:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037dfdb:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037dfe0:  66 0F 6E C3              MOVD XMM0,EBX
14037dfe4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dfe7:  0F 2F C8                 COMISS XMM1,XMM0
14037dfea:  0F 86 FC 01 00 00        JBE 0x14037e1ec
14037dff0:  66 0F 6E C7              MOVD XMM0,EDI
14037dff4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037dff7:  0F 2F C1                 COMISS XMM0,XMM1
14037dffa:  0F 86 EC 01 00 00        JBE 0x14037e1ec
14037e000:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037e007:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037e00a:  48 8D 0D BF CC 66 00     LEA RCX,[0x1409eacd0]
14037e011:  FF 15 D9 E6 4E 00        CALL qword ptr [0x14086c6f0]
14037e017:  4C 8B C0                 MOV R8,RAX
14037e01a:  33 D2                    XOR EDX,EDX
14037e01c:  48 8B CB                 MOV RCX,RBX
14037e01f:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037e022:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037e029:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037e02d:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037e031:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037e035:  48 8D 8D 90 09 00 00     LEA RCX,[RBP + 0x990]
14037e03c:  FF 15 DE DF 4E 00        CALL qword ptr [0x14086c020]
14037e042:  48 8B D0                 MOV RDX,RAX
14037e045:  48 8D 0D 14 00 64 00     LEA RCX,[0x1409be060]
14037e04c:  FF 15 46 E8 4E 00        CALL qword ptr [0x14086c898]
14037e052:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037e056:  48 8D 0D 03 00 64 00     LEA RCX,[0x1409be060]
14037e05d:  FF 15 F5 E8 4E 00        CALL qword ptr [0x14086c958]
14037e063:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037e069:  F3 0F 10 15 17 40 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e071:  F3 0F 59 15 AF C7 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037e079:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037e080:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037e087:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037e08e:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037e095:  0F 28 DA                 MOVAPS XMM3,XMM2
14037e098:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e09d:  48 8D 0D BC FF 63 00     LEA RCX,[0x1409be060]
14037e0a4:  FF 15 AE E7 4E 00        CALL qword ptr [0x14086c858]
14037e0aa:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037e0ae:  48 8D 0D AB FF 63 00     LEA RCX,[0x1409be060]
14037e0b5:  FF 15 9D E8 4E 00        CALL qword ptr [0x14086c958]
14037e0bb:  48 8D 0D CE 6A 6D 00     LEA RCX,[0x140a54b90]
14037e0c2:  FF 15 F0 E7 4E 00        CALL qword ptr [0x14086c8b8]
14037e0c8:  84 C0                    TEST AL,AL
14037e0ca:  74 1C                    JZ 0x14037e0e8
14037e0cc:  41 B0 01                 MOV R8B,0x1
14037e0cf:  48 8D 15 52 16 54 00     LEA RDX,[0x1408bf728]
14037e0d6:  48 8B CE                 MOV RCX,RSI
14037e0d9:  E8 F2 19 08 00           CALL 0x1403ffad0
14037e0de:  C7 86 D4 C7 00 00 47 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x47
14037e0e8:  41 B4 01                 MOV R12B,0x1
14037e0eb:  8B 86 D0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d0]
14037e0f1:  85 C0                    TEST EAX,EAX
14037e0f3:  75 65                    JNZ 0x14037e15a
14037e0f5:  41 83 C8 FF              OR R8D,0xffffffff
14037e0f9:  48 8D 15 50 15 54 00     LEA RDX,[0x1408bf650]
14037e100:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e105:  E8 A6 CC ED FF           CALL 0x14025adb0
14037e10a:  90                       NOP
14037e10b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e110:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e114:  E8 D7 CC 0C 00           CALL 0x14044adf0
14037e119:  90                       NOP
14037e11a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e11f:  E8 9C 39 C8 FF           CALL 0x140001ac0
14037e124:  41 83 C8 FF              OR R8D,0xffffffff
14037e128:  48 8D 15 09 15 54 00     LEA RDX,[0x1408bf638]
14037e12f:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e134:  E8 77 CC ED FF           CALL 0x14025adb0
14037e139:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e13e:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e142:  E8 99 CB 0C 00           CALL 0x14044ace0
14037e147:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e14c:  E8 6F 39 C8 FF           CALL 0x140001ac0
14037e151:  48 8D 15 C0 14 54 00     LEA RDX,[0x1408bf618]
14037e158:  EB 6C                    JMP 0x14037e1c6
14037e15a:  83 F8 01                 CMP EAX,0x1
14037e15d:  0F 85 89 00 00 00        JNZ 0x14037e1ec
14037e163:  41 83 C8 FF              OR R8D,0xffffffff
14037e167:  48 8D 15 92 14 54 00     LEA RDX,[0x1408bf600]
14037e16e:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e173:  E8 38 CC ED FF           CALL 0x14025adb0
14037e178:  90                       NOP
14037e179:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e17e:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e182:  E8 69 CC 0C 00           CALL 0x14044adf0
14037e187:  90                       NOP
14037e188:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e18d:  E8 2E 39 C8 FF           CALL 0x140001ac0
14037e192:  41 83 C8 FF              OR R8D,0xffffffff
14037e196:  48 8D 15 03 15 54 00     LEA RDX,[0x1408bf6a0]
14037e19d:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e1a2:  E8 09 CC ED FF           CALL 0x14025adb0
14037e1a7:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e1ac:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e1b0:  E8 2B CB 0C 00           CALL 0x14044ace0
14037e1b5:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e1ba:  E8 01 39 C8 FF           CALL 0x140001ac0
14037e1bf:  48 8D 15 BA 14 54 00     LEA RDX,[0x1408bf680]
14037e1c6:  41 83 C8 FF              OR R8D,0xffffffff
14037e1ca:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e1cf:  E8 DC CB ED FF           CALL 0x14025adb0
14037e1d4:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e1d9:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e1dd:  E8 FE CA 0C 00           CALL 0x14044ace0
14037e1e2:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e1e7:  E8 D4 38 C8 FF           CALL 0x140001ac0
14037e1ec:  F3 0F 10 35 94 3E 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037e1f4:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037e1f9:  BA E9 10 00 00           MOV EDX,0x10e9
14037e1fe:  48 8D 0D 8B 93 61 00     LEA RCX,[0x140997590]
14037e205:  FF 15 85 E6 4E 00        CALL qword ptr [0x14086c890]
14037e20b:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037e20f:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037e213:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037e218:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037e21d:  48 8D 05 4C 14 54 00     LEA RAX,[0x1408bf670]
14037e224:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e229:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e231:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e236:  0F 28 D6                 MOVAPS XMM2,XMM6
14037e239:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e23e:  49 8B D7                 MOV RDX,R15
14037e241:  48 8D 0D 68 8D 61 00     LEA RCX,[0x140996fb0]
14037e248:  FF 15 32 E6 4E 00        CALL qword ptr [0x14086c880]
14037e24e:  F3 0F 10 95 68 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x168]
14037e256:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037e259:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037e25e:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037e263:  45 33 C9                 XOR R9D,R9D
14037e266:  48 8D 95 E8 02 00 00     LEA RDX,[RBP + 0x2e8]
14037e26d:  48 8B CE                 MOV RCX,RSI
14037e270:  E8 EB 89 FB FF           CALL 0x140336c60
14037e275:  90                       NOP
14037e276:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037e279:  F3 0F 10 15 07 3E 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e281:  0F 28 C2                 MOVAPS XMM0,XMM2
14037e284:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037e289:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037e28d:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037e291:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037e296:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037e29c:  66 0F 6E 05 E8 6F 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037e2a4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e2a7:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037e2ab:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037e2b0:  48 8D 05 B1 13 54 00     LEA RAX,[0x1408bf668]
14037e2b7:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e2bc:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e2c4:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e2c9:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e2ce:  49 8B D7                 MOV RDX,R15
14037e2d1:  48 8D 0D D8 8C 61 00     LEA RCX,[0x140996fb0]
14037e2d8:  FF 15 9A E5 4E 00        CALL qword ptr [0x14086c878]
14037e2de:  90                       NOP
14037e2df:  48 8D 8D E8 02 00 00     LEA RCX,[RBP + 0x2e8]
14037e2e6:  E8 D5 37 C8 FF           CALL 0x140001ac0
14037e2eb:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037e2f2:  75 0A                    JNZ 0x14037e2fe
14037e2f4:  F3 0F 10 85 74 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x174]
14037e2fc:  EB 08                    JMP 0x14037e306
14037e2fe:  F3 0F 10 85 80 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x180]
14037e306:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037e309:  41 B1 01                 MOV R9B,0x1
14037e30c:  45 33 C0                 XOR R8D,R8D
14037e30f:  48 8B CE                 MOV RCX,RSI
14037e312:  E8 09 81 FB FF           CALL 0x140336420
14037e317:  F3 0F 10 15 69 3D 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e31f:  0F 28 C2                 MOVAPS XMM0,XMM2
14037e322:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037e327:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
14037e32c:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037e331:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037e337:  66 0F 6E 05 4D 6F 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037e33f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e342:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037e346:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037e34b:  48 8D 05 E6 14 54 00     LEA RAX,[0x1408bf838]
14037e352:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e357:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e35f:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037e363:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e368:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e36d:  49 8B D7                 MOV RDX,R15
14037e370:  48 8D 0D 39 8C 61 00     LEA RCX,[0x140996fb0]
14037e377:  FF 15 FB E4 4E 00        CALL qword ptr [0x14086c878]
14037e37d:  F3 0F 10 0D 03 3D 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037e385:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037e38a:  0F 28 C7                 MOVAPS XMM0,XMM7
14037e38d:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037e391:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037e395:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037e399:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037e3a0:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037e3a5:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037e3aa:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037e3ae:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037e3b2:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037e3b7:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037e3bd:  41 B8 06 00 00 00        MOV R8D,0x6
14037e3c3:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e3c8:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037e3cf:  E8 6C 81 0C 00           CALL 0x140446540
14037e3d4:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037e3d9:  0F 84 3D 02 00 00        JZ 0x14037e61c
14037e3df:  66 41 0F 6E C6           MOVD XMM0,R14D
14037e3e4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e3e7:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037e3ec:  0F 2F C8                 COMISS XMM1,XMM0
14037e3ef:  0F 86 27 02 00 00        JBE 0x14037e61c
14037e3f5:  66 41 0F 6E C5           MOVD XMM0,R13D
14037e3fa:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e3fd:  0F 2F C1                 COMISS XMM0,XMM1
14037e400:  0F 86 16 02 00 00        JBE 0x14037e61c
14037e406:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037e40b:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037e410:  66 0F 6E C3              MOVD XMM0,EBX
14037e414:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e417:  0F 2F C8                 COMISS XMM1,XMM0
14037e41a:  0F 86 FC 01 00 00        JBE 0x14037e61c
14037e420:  66 0F 6E C7              MOVD XMM0,EDI
14037e424:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e427:  0F 2F C1                 COMISS XMM0,XMM1
14037e42a:  0F 86 EC 01 00 00        JBE 0x14037e61c
14037e430:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037e437:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037e43a:  48 8D 0D 8F C8 66 00     LEA RCX,[0x1409eacd0]
14037e441:  FF 15 A9 E2 4E 00        CALL qword ptr [0x14086c6f0]
14037e447:  4C 8B C0                 MOV R8,RAX
14037e44a:  33 D2                    XOR EDX,EDX
14037e44c:  48 8B CB                 MOV RCX,RBX
14037e44f:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037e452:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037e459:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037e45d:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037e461:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037e465:  48 8D 8D A0 09 00 00     LEA RCX,[RBP + 0x9a0]
14037e46c:  FF 15 AE DB 4E 00        CALL qword ptr [0x14086c020]
14037e472:  48 8B D0                 MOV RDX,RAX
14037e475:  48 8D 0D E4 FB 63 00     LEA RCX,[0x1409be060]
14037e47c:  FF 15 16 E4 4E 00        CALL qword ptr [0x14086c898]
14037e482:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037e486:  48 8D 0D D3 FB 63 00     LEA RCX,[0x1409be060]
14037e48d:  FF 15 C5 E4 4E 00        CALL qword ptr [0x14086c958]
14037e493:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037e499:  F3 0F 10 15 E7 3B 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e4a1:  F3 0F 59 15 7F C3 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037e4a9:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037e4b0:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037e4b7:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037e4be:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037e4c5:  0F 28 DA                 MOVAPS XMM3,XMM2
14037e4c8:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e4cd:  48 8D 0D 8C FB 63 00     LEA RCX,[0x1409be060]
14037e4d4:  FF 15 7E E3 4E 00        CALL qword ptr [0x14086c858]
14037e4da:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037e4de:  48 8D 0D 7B FB 63 00     LEA RCX,[0x1409be060]
14037e4e5:  FF 15 6D E4 4E 00        CALL qword ptr [0x14086c958]
14037e4eb:  48 8D 0D 9E 66 6D 00     LEA RCX,[0x140a54b90]
14037e4f2:  FF 15 C0 E3 4E 00        CALL qword ptr [0x14086c8b8]
14037e4f8:  84 C0                    TEST AL,AL
14037e4fa:  74 1C                    JZ 0x14037e518
14037e4fc:  41 B0 01                 MOV R8B,0x1
14037e4ff:  48 8D 15 22 13 54 00     LEA RDX,[0x1408bf828]
14037e506:  48 8B CE                 MOV RCX,RSI
14037e509:  E8 C2 15 08 00           CALL 0x1403ffad0
14037e50e:  C7 86 D4 C7 00 00 48 00 00 00 MOV dword ptr [RSI + 0xc7d4],0x48
14037e518:  41 B4 01                 MOV R12B,0x1
14037e51b:  8B 86 D0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d0]
14037e521:  85 C0                    TEST EAX,EAX
14037e523:  75 65                    JNZ 0x14037e58a
14037e525:  41 83 C8 FF              OR R8D,0xffffffff
14037e529:  48 8D 15 D8 12 54 00     LEA RDX,[0x1408bf808]
14037e530:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e535:  E8 76 C8 ED FF           CALL 0x14025adb0
14037e53a:  90                       NOP
14037e53b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e540:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e544:  E8 A7 C8 0C 00           CALL 0x14044adf0
14037e549:  90                       NOP
14037e54a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e54f:  E8 6C 35 C8 FF           CALL 0x140001ac0
14037e554:  41 83 C8 FF              OR R8D,0xffffffff
14037e558:  48 8D 15 91 12 54 00     LEA RDX,[0x1408bf7f0]
14037e55f:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e564:  E8 47 C8 ED FF           CALL 0x14025adb0
14037e569:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e56e:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e572:  E8 69 C7 0C 00           CALL 0x14044ace0
14037e577:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e57c:  E8 3F 35 C8 FF           CALL 0x140001ac0
14037e581:  48 8D 15 08 13 54 00     LEA RDX,[0x1408bf890]
14037e588:  EB 6C                    JMP 0x14037e5f6
14037e58a:  83 F8 01                 CMP EAX,0x1
14037e58d:  0F 85 89 00 00 00        JNZ 0x14037e61c
14037e593:  41 83 C8 FF              OR R8D,0xffffffff
14037e597:  48 8D 15 D2 12 54 00     LEA RDX,[0x1408bf870]
14037e59e:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e5a3:  E8 08 C8 ED FF           CALL 0x14025adb0
14037e5a8:  90                       NOP
14037e5a9:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e5ae:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e5b2:  E8 39 C8 0C 00           CALL 0x14044adf0
14037e5b7:  90                       NOP
14037e5b8:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e5bd:  E8 FE 34 C8 FF           CALL 0x140001ac0
14037e5c2:  41 83 C8 FF              OR R8D,0xffffffff
14037e5c6:  48 8D 15 8B 12 54 00     LEA RDX,[0x1408bf858]
14037e5cd:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e5d2:  E8 D9 C7 ED FF           CALL 0x14025adb0
14037e5d7:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e5dc:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e5e0:  E8 FB C6 0C 00           CALL 0x14044ace0
14037e5e5:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e5ea:  E8 D1 34 C8 FF           CALL 0x140001ac0
14037e5ef:  48 8D 15 4A 12 54 00     LEA RDX,[0x1408bf840]
14037e5f6:  41 83 C8 FF              OR R8D,0xffffffff
14037e5fa:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e5ff:  E8 AC C7 ED FF           CALL 0x14025adb0
14037e604:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e609:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e60d:  E8 CE C6 0C 00           CALL 0x14044ace0
14037e612:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e617:  E8 A4 34 C8 FF           CALL 0x140001ac0
14037e61c:  F3 0F 10 35 64 3A 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037e624:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037e629:  BA F9 10 00 00           MOV EDX,0x10f9
14037e62e:  48 8D 0D 5B 8F 61 00     LEA RCX,[0x140997590]
14037e635:  FF 15 55 E2 4E 00        CALL qword ptr [0x14086c890]
14037e63b:  0F 28 DF                 MOVAPS XMM3,XMM7
14037e63e:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037e642:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037e647:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037e64c:  48 8D 05 35 11 54 00     LEA RAX,[0x1408bf788]
14037e653:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e658:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e660:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e665:  0F 28 D6                 MOVAPS XMM2,XMM6
14037e668:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e66d:  49 8B D7                 MOV RDX,R15
14037e670:  48 8D 0D 39 89 61 00     LEA RCX,[0x140996fb0]
14037e677:  FF 15 03 E2 4E 00        CALL qword ptr [0x14086c880]
14037e67d:  F3 0F 10 95 6C 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x16c]
14037e685:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037e688:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037e68d:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037e692:  45 33 C9                 XOR R9D,R9D
14037e695:  48 8D 95 F8 02 00 00     LEA RDX,[RBP + 0x2f8]
14037e69c:  48 8B CE                 MOV RCX,RSI
14037e69f:  E8 BC 85 FB FF           CALL 0x140336c60
14037e6a4:  90                       NOP
14037e6a5:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037e6a8:  F3 0F 10 15 D8 39 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e6b0:  0F 28 C2                 MOVAPS XMM0,XMM2
14037e6b3:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037e6b8:  0F 28 DF                 MOVAPS XMM3,XMM7
14037e6bb:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037e6bf:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037e6c4:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037e6ca:  66 0F 6E 05 BA 6B 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037e6d2:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e6d5:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037e6d9:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037e6de:  48 8D 05 9B 10 54 00     LEA RAX,[0x1408bf780]
14037e6e5:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e6ea:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e6f2:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e6f7:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e6fc:  49 8B D7                 MOV RDX,R15
14037e6ff:  48 8D 0D AA 88 61 00     LEA RCX,[0x140996fb0]
14037e706:  FF 15 6C E1 4E 00        CALL qword ptr [0x14086c878]
14037e70c:  90                       NOP
14037e70d:  48 8D 8D F8 02 00 00     LEA RCX,[RBP + 0x2f8]
14037e714:  E8 A7 33 C8 FF           CALL 0x140001ac0
14037e719:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037e720:  75 0A                    JNZ 0x14037e72c
14037e722:  F3 0F 10 85 78 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x178]
14037e72a:  EB 08                    JMP 0x14037e734
14037e72c:  F3 0F 10 85 84 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x184]
14037e734:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037e737:  41 B1 01                 MOV R9B,0x1
14037e73a:  45 33 C0                 XOR R8D,R8D
14037e73d:  48 8B CE                 MOV RCX,RSI
14037e740:  E8 DB 7C FB FF           CALL 0x140336420
14037e745:  F3 0F 10 15 3B 39 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e74d:  0F 28 C2                 MOVAPS XMM0,XMM2
14037e750:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037e755:  F3 0F 5C F8              SUBSS XMM7,XMM0
14037e759:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037e75e:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037e764:  66 0F 6E 05 20 6B 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037e76c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e76f:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037e773:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037e778:  48 8D 05 F9 0F 54 00     LEA RAX,[0x1408bf778]
14037e77f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e784:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037e78c:  0F 28 DF                 MOVAPS XMM3,XMM7
14037e78f:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e794:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e799:  49 8B D7                 MOV RDX,R15
14037e79c:  48 8D 0D 0D 88 61 00     LEA RCX,[0x140996fb0]
14037e7a3:  FF 15 CF E0 4E 00        CALL qword ptr [0x14086c878]
14037e7a9:  F3 0F 10 25 D7 38 61 00  MOVSS XMM4,dword ptr [0x140992088]
14037e7b1:  0F 28 FC                 MOVAPS XMM7,XMM4
14037e7b4:  F3 0F 59 3D 6C C0 58 00  MULSS XMM7,dword ptr [0x14090a828]
14037e7bc:  F3 41 0F 58 FA           ADDSS XMM7,XMM10
14037e7c1:  0F 28 D4                 MOVAPS XMM2,XMM4
14037e7c4:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037e7c9:  0F 28 C7                 MOVAPS XMM0,XMM7
14037e7cc:  F3 0F 5C C2              SUBSS XMM0,XMM2
14037e7d0:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
14037e7d4:  89 44 24 64              MOV dword ptr [RSP + 0x64],EAX
14037e7d8:  F3 0F 10 45 F8           MOVSS XMM0,dword ptr [RBP + -0x8]
14037e7dd:  F3 0F 58 C7              ADDSS XMM0,XMM7
14037e7e1:  F3 0F 58 C2              ADDSS XMM0,XMM2
14037e7e5:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037e7e9:  89 5C 24 6C              MOV dword ptr [RSP + 0x6c],EBX
14037e7ed:  66 0F 6E F0              MOVD XMM6,EAX
14037e7f1:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14037e7f4:  F3 0F 59 25 D0 C0 58 00  MULSS XMM4,dword ptr [0x14090a8cc]
14037e7fc:  F3 0F 58 E6              ADDSS XMM4,XMM6
14037e800:  41 B9 06 00 00 00        MOV R9D,0x6
14037e806:  0F 28 D4                 MOVAPS XMM2,XMM4
14037e809:  F3 0F 10 4D B0           MOVSS XMM1,dword ptr [RBP + -0x50]
14037e80e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037e815:  E8 D6 79 0C 00           CALL 0x1404461f0
14037e81a:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037e81f:  0F 84 78 01 00 00        JZ 0x14037e99d
14037e825:  66 41 0F 6E C6           MOVD XMM0,R14D
14037e82a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e82d:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037e832:  0F 2F C8                 COMISS XMM1,XMM0
14037e835:  0F 86 62 01 00 00        JBE 0x14037e99d
14037e83b:  66 41 0F 6E C5           MOVD XMM0,R13D
14037e840:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e843:  0F 2F C1                 COMISS XMM0,XMM1
14037e846:  0F 86 51 01 00 00        JBE 0x14037e99d
14037e84c:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037e851:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037e856:  0F 2F CE                 COMISS XMM1,XMM6
14037e859:  0F 86 3E 01 00 00        JBE 0x14037e99d
14037e85f:  66 0F 6E C3              MOVD XMM0,EBX
14037e863:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037e866:  0F 2F C1                 COMISS XMM0,XMM1
14037e869:  0F 86 2E 01 00 00        JBE 0x14037e99d
14037e86f:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037e876:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037e879:  48 8D 0D 50 C4 66 00     LEA RCX,[0x1409eacd0]
14037e880:  FF 15 6A DE 4E 00        CALL qword ptr [0x14086c6f0]
14037e886:  4C 8B C0                 MOV R8,RAX
14037e889:  33 D2                    XOR EDX,EDX
14037e88b:  48 8B CB                 MOV RCX,RBX
14037e88e:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037e891:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037e898:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037e89c:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037e8a0:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037e8a4:  48 8D 8D B0 09 00 00     LEA RCX,[RBP + 0x9b0]
14037e8ab:  FF 15 6F D7 4E 00        CALL qword ptr [0x14086c020]
14037e8b1:  48 8B D0                 MOV RDX,RAX
14037e8b4:  48 8D 0D A5 F7 63 00     LEA RCX,[0x1409be060]
14037e8bb:  FF 15 D7 DF 4E 00        CALL qword ptr [0x14086c898]
14037e8c1:  0F 28 44 24 60           MOVAPS XMM0,xmmword ptr [RSP + 0x60]
14037e8c6:  66 0F 7F 44 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM0
14037e8cc:  F3 0F 10 15 B4 37 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037e8d4:  F3 0F 59 15 4C C0 58 00  MULSS XMM2,dword ptr [0x14090a928]
14037e8dc:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037e8e3:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037e8ea:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037e8f1:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037e8f8:  0F 28 DA                 MOVAPS XMM3,XMM2
14037e8fb:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e900:  48 8D 0D 59 F7 63 00     LEA RCX,[0x1409be060]
14037e907:  FF 15 4B DF 4E 00        CALL qword ptr [0x14086c858]
14037e90d:  41 B4 01                 MOV R12B,0x1
14037e910:  41 83 C8 FF              OR R8D,0xffffffff
14037e914:  48 8D 15 3D 0E 54 00     LEA RDX,[0x1408bf758]
14037e91b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e920:  E8 8B C4 ED FF           CALL 0x14025adb0
14037e925:  90                       NOP
14037e926:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e92b:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e92f:  E8 BC C4 0C 00           CALL 0x14044adf0
14037e934:  90                       NOP
14037e935:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e93a:  E8 81 31 C8 FF           CALL 0x140001ac0
14037e93f:  41 83 C8 FF              OR R8D,0xffffffff
14037e943:  48 8D 15 8E 0E 54 00     LEA RDX,[0x1408bf7d8]
14037e94a:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e94f:  E8 5C C4 ED FF           CALL 0x14025adb0
14037e954:  90                       NOP
14037e955:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e95a:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e95e:  E8 8D C4 0C 00           CALL 0x14044adf0
14037e963:  90                       NOP
14037e964:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e969:  E8 52 31 C8 FF           CALL 0x140001ac0
14037e96e:  41 83 C8 FF              OR R8D,0xffffffff
14037e972:  48 8D 15 3F 0E 54 00     LEA RDX,[0x1408bf7b8]
14037e979:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e97e:  E8 2D C4 ED FF           CALL 0x14025adb0
14037e983:  90                       NOP
14037e984:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037e989:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037e98d:  E8 5E C4 0C 00           CALL 0x14044adf0
14037e992:  90                       NOP
14037e993:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037e998:  E8 23 31 C8 FF           CALL 0x140001ac0
14037e99d:  F3 0F 10 35 E3 36 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037e9a5:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037e9aa:  BA 72 06 00 00           MOV EDX,0x672
14037e9af:  48 8D 0D DA 8B 61 00     LEA RCX,[0x140997590]
14037e9b6:  FF 15 D4 DE 4E 00        CALL qword ptr [0x14086c890]
14037e9bc:  0F 28 DF                 MOVAPS XMM3,XMM7
14037e9bf:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037e9c3:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037e9c8:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037e9cd:  48 8D 05 D4 0D 54 00     LEA RAX,[0x1408bf7a8]
14037e9d4:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037e9d9:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037e9e1:  66 41 0F 7E D9           MOVD R9D,XMM3
14037e9e6:  0F 28 D6                 MOVAPS XMM2,XMM6
14037e9e9:  66 41 0F 7E D0           MOVD R8D,XMM2
14037e9ee:  49 8B D7                 MOV RDX,R15
14037e9f1:  48 8D 0D B8 85 61 00     LEA RCX,[0x140996fb0]
14037e9f8:  FF 15 82 DE 4E 00        CALL qword ptr [0x14086c880]
14037e9fe:  BA E5 05 00 00           MOV EDX,0x5e5
14037ea03:  48 8D 0D 86 8B 61 00     LEA RCX,[0x140997590]
14037ea0a:  FF 15 80 DE 4E 00        CALL qword ptr [0x14086c890]
14037ea10:  F3 0F 10 05 70 36 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037ea18:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037ea1d:  0F 28 DF                 MOVAPS XMM3,XMM7
14037ea20:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037ea24:  66 0F 6E 05 60 68 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037ea2c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ea2f:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037ea34:  F3 0F 10 54 24 70        MOVSS XMM2,dword ptr [RSP + 0x70]
14037ea3a:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037ea3e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037ea43:  48 8D 05 4E 0D 54 00     LEA RAX,[0x1408bf798]
14037ea4a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037ea4f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037ea57:  66 41 0F 7E D9           MOVD R9D,XMM3
14037ea5c:  66 41 0F 7E D0           MOVD R8D,XMM2
14037ea61:  49 8B D7                 MOV RDX,R15
14037ea64:  48 8D 0D 45 85 61 00     LEA RCX,[0x140996fb0]
14037ea6b:  FF 15 17 DE 4E 00        CALL qword ptr [0x14086c888]
14037ea71:  48 8D 0D 58 C2 66 00     LEA RCX,[0x1409eacd0]
14037ea78:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
14037ea7f:  75 09                    JNZ 0x14037ea8a
14037ea81:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037ea88:  EB 07                    JMP 0x14037ea91
14037ea8a:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037ea91:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037ea94:  FF 15 56 DC 4E 00        CALL qword ptr [0x14086c6f0]
14037ea9a:  4C 8B C0                 MOV R8,RAX
14037ea9d:  33 D2                    XOR EDX,EDX
14037ea9f:  48 8B CB                 MOV RCX,RBX
14037eaa2:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037eaa5:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037eaac:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037eab0:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037eab4:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037eab8:  48 8D 8D C0 09 00 00     LEA RCX,[RBP + 0x9c0]
14037eabf:  FF 15 5B D5 4E 00        CALL qword ptr [0x14086c020]
14037eac5:  48 8B D0                 MOV RDX,RAX
14037eac8:  48 8D 0D 91 F5 63 00     LEA RCX,[0x1409be060]
14037eacf:  FF 15 C3 DD 4E 00        CALL qword ptr [0x14086c898]
14037ead5:  F3 0F 10 0D AB 35 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037eadd:  F3 0F 59 0D 27 BF 58 00  MULSS XMM1,dword ptr [0x14090aa0c]
14037eae5:  0F 28 D1                 MOVAPS XMM2,XMM1
14037eae8:  48 8D 0D 71 F5 63 00     LEA RCX,[0x1409be060]
14037eaef:  FF 15 B3 DD 4E 00        CALL qword ptr [0x14086c8a8]
14037eaf5:  F3 0F 10 15 8B 35 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037eafd:  F3 0F 59 15 23 BD 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037eb05:  F3 0F 58 D7              ADDSS XMM2,XMM7
14037eb09:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037eb0d:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
14037eb13:  48 8D 8D 84 04 00 00     LEA RCX,[RBP + 0x484]
14037eb1a:  FF 15 E8 D4 4E 00        CALL qword ptr [0x14086c008]
14037eb20:  48 8B D0                 MOV RDX,RAX
14037eb23:  48 8D 0D 36 F5 63 00     LEA RCX,[0x1409be060]
14037eb2a:  FF 15 70 DD 4E 00        CALL qword ptr [0x14086c8a0]
14037eb30:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037eb35:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037eb3c:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037eb43:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037eb47:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037eb4b:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037eb4f:  48 8D 0D 0A F5 63 00     LEA RCX,[0x1409be060]
14037eb56:  FF 15 04 DD 4E 00        CALL qword ptr [0x14086c860]
14037eb5c:  F3 0F 10 75 BC           MOVSS XMM6,dword ptr [RBP + -0x44]
14037eb61:  F3 0F 58 FE              ADDSS XMM7,XMM6
14037eb65:  8B 05 05 67 61 00        MOV EAX,dword ptr [0x140995270]
14037eb6b:  83 F8 02                 CMP EAX,0x2
14037eb6e:  75 09                    JNZ 0x14037eb79
14037eb70:  48 8D 05 F9 0F 54 00     LEA RAX,[0x1408bfb70]
14037eb77:  EB 13                    JMP 0x14037eb8c
14037eb79:  83 F8 01                 CMP EAX,0x1
14037eb7c:  48 8D 05 DD 0E 54 00     LEA RAX,[0x1408bfa60]
14037eb83:  74 07                    JZ 0x14037eb8c
14037eb85:  48 8D 05 04 0E 54 00     LEA RAX,[0x1408bf990]
14037eb8c:  F3 0F 10 15 F4 34 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037eb94:  F3 41 0F 59 D0           MULSS XMM2,XMM8
14037eb99:  0F 28 DA                 MOVAPS XMM3,XMM2
14037eb9c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037eba1:  F3 0F 58 DF              ADDSS XMM3,XMM7
14037eba5:  F3 41 0F 58 D7           ADDSS XMM2,XMM15
14037ebaa:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037ebb2:  66 41 0F 7E D9           MOVD R9D,XMM3
14037ebb7:  66 41 0F 7E D0           MOVD R8D,XMM2
14037ebbc:  49 8B D7                 MOV RDX,R15
14037ebbf:  48 8D 0D EA 83 61 00     LEA RCX,[0x140996fb0]
14037ebc6:  FF 15 B4 DC 4E 00        CALL qword ptr [0x14086c880]
14037ebcc:  F3 0F 58 FE              ADDSS XMM7,XMM6
14037ebd0:  F3 0F 10 0D B0 34 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037ebd8:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037ebdd:  0F 28 C7                 MOVAPS XMM0,XMM7
14037ebe0:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037ebe4:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037ebe8:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037ebec:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037ebf3:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037ebf8:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
14037ebfd:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037ec01:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037ec05:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037ec0a:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037ec10:  41 B8 06 00 00 00        MOV R8D,0x6
14037ec16:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ec1b:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037ec22:  E8 19 79 0C 00           CALL 0x140446540
14037ec27:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037ec2c:  0F 84 98 01 00 00        JZ 0x14037edca
14037ec32:  66 41 0F 6E C6           MOVD XMM0,R14D
14037ec37:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ec3a:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037ec3f:  0F 2F C8                 COMISS XMM1,XMM0
14037ec42:  0F 86 82 01 00 00        JBE 0x14037edca
14037ec48:  66 41 0F 6E C5           MOVD XMM0,R13D
14037ec4d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ec50:  0F 2F C1                 COMISS XMM0,XMM1
14037ec53:  0F 86 71 01 00 00        JBE 0x14037edca
14037ec59:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037ec5e:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037ec63:  66 0F 6E C3              MOVD XMM0,EBX
14037ec67:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ec6a:  0F 2F C8                 COMISS XMM1,XMM0
14037ec6d:  0F 86 57 01 00 00        JBE 0x14037edca
14037ec73:  66 0F 6E C7              MOVD XMM0,EDI
14037ec77:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ec7a:  0F 2F C1                 COMISS XMM0,XMM1
14037ec7d:  0F 86 47 01 00 00        JBE 0x14037edca
14037ec83:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037ec8a:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037ec8d:  48 8D 0D 3C C0 66 00     LEA RCX,[0x1409eacd0]
14037ec94:  FF 15 56 DA 4E 00        CALL qword ptr [0x14086c6f0]
14037ec9a:  4C 8B C0                 MOV R8,RAX
14037ec9d:  33 D2                    XOR EDX,EDX
14037ec9f:  48 8B CB                 MOV RCX,RBX
14037eca2:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037eca5:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037ecac:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037ecb0:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037ecb4:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037ecb8:  48 8D 8D D0 09 00 00     LEA RCX,[RBP + 0x9d0]
14037ecbf:  FF 15 5B D3 4E 00        CALL qword ptr [0x14086c020]
14037ecc5:  48 8B D0                 MOV RDX,RAX
14037ecc8:  48 8D 0D 91 F3 63 00     LEA RCX,[0x1409be060]
14037eccf:  FF 15 C3 DB 4E 00        CALL qword ptr [0x14086c898]
14037ecd5:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037ecd9:  48 8D 0D 80 F3 63 00     LEA RCX,[0x1409be060]
14037ece0:  FF 15 72 DC 4E 00        CALL qword ptr [0x14086c958]
14037ece6:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037ecec:  F3 0F 10 15 94 33 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037ecf4:  F3 0F 59 15 2C BB 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037ecfc:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037ed03:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037ed0a:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037ed11:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037ed18:  0F 28 DA                 MOVAPS XMM3,XMM2
14037ed1b:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ed20:  48 8D 0D 39 F3 63 00     LEA RCX,[0x1409be060]
14037ed27:  FF 15 2B DB 4E 00        CALL qword ptr [0x14086c858]
14037ed2d:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037ed31:  48 8D 0D 28 F3 63 00     LEA RCX,[0x1409be060]
14037ed38:  FF 15 1A DC 4E 00        CALL qword ptr [0x14086c958]
14037ed3e:  41 B4 01                 MOV R12B,0x1
14037ed41:  41 83 C8 FF              OR R8D,0xffffffff
14037ed45:  48 8D 15 24 0C 54 00     LEA RDX,[0x1408bf970]
14037ed4c:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ed51:  E8 5A C0 ED FF           CALL 0x14025adb0
14037ed56:  90                       NOP
14037ed57:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ed5c:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037ed60:  E8 8B C0 0C 00           CALL 0x14044adf0
14037ed65:  90                       NOP
14037ed66:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ed6b:  E8 50 2D C8 FF           CALL 0x140001ac0
14037ed70:  41 83 C8 FF              OR R8D,0xffffffff
14037ed74:  48 8D 15 DD 0D 54 00     LEA RDX,[0x1408bfb58]
14037ed7b:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ed80:  E8 2B C0 ED FF           CALL 0x14025adb0
14037ed85:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ed8a:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037ed8e:  E8 4D BF 0C 00           CALL 0x14044ace0
14037ed93:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037ed98:  E8 23 2D C8 FF           CALL 0x140001ac0
14037ed9d:  41 83 C8 FF              OR R8D,0xffffffff
14037eda1:  48 8D 15 78 0E 54 00     LEA RDX,[0x1408bfc20]
14037eda8:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037edad:  E8 FE BF ED FF           CALL 0x14025adb0
14037edb2:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037edb7:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037edbb:  E8 20 BF 0C 00           CALL 0x14044ace0
14037edc0:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037edc5:  E8 F6 2C C8 FF           CALL 0x140001ac0
14037edca:  F3 0F 10 35 B6 32 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037edd2:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037edd7:  BA EB 05 00 00           MOV EDX,0x5eb
14037eddc:  48 8D 0D AD 87 61 00     LEA RCX,[0x140997590]
14037ede3:  FF 15 A7 DA 4E 00        CALL qword ptr [0x14086c890]
14037ede9:  0F 28 DF                 MOVAPS XMM3,XMM7
14037edec:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037edf0:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037edf5:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037edfa:  48 8D 05 0F 0E 54 00     LEA RAX,[0x1408bfc10]
14037ee01:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037ee06:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037ee0e:  66 41 0F 7E D9           MOVD R9D,XMM3
14037ee13:  0F 28 D6                 MOVAPS XMM2,XMM6
14037ee16:  66 41 0F 7E D0           MOVD R8D,XMM2
14037ee1b:  49 8B D7                 MOV RDX,R15
14037ee1e:  48 8D 0D 8B 81 61 00     LEA RCX,[0x140996fb0]
14037ee25:  FF 15 55 DA 4E 00        CALL qword ptr [0x14086c880]
14037ee2b:  F3 0F 10 95 24 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x124]
14037ee33:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037ee36:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037ee3b:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037ee40:  45 33 C9                 XOR R9D,R9D
14037ee43:  48 8D 95 08 03 00 00     LEA RDX,[RBP + 0x308]
14037ee4a:  48 8B CE                 MOV RCX,RSI
14037ee4d:  E8 0E 7E FB FF           CALL 0x140336c60
14037ee52:  90                       NOP
14037ee53:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037ee56:  F3 0F 10 15 2A 32 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037ee5e:  0F 28 C2                 MOVAPS XMM0,XMM2
14037ee61:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037ee66:  F3 0F 5C F8              SUBSS XMM7,XMM0
14037ee6a:  F3 41 0F 59 D3           MULSS XMM2,XMM11
14037ee6f:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037ee75:  66 0F 6E 05 0F 64 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037ee7d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ee80:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037ee84:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037ee89:  48 8D 05 C8 0B 54 00     LEA RAX,[0x1408bfa58]
14037ee90:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037ee95:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037ee9d:  0F 28 DF                 MOVAPS XMM3,XMM7
14037eea0:  66 41 0F 7E D9           MOVD R9D,XMM3
14037eea5:  66 41 0F 7E D0           MOVD R8D,XMM2
14037eeaa:  49 8B D7                 MOV RDX,R15
14037eead:  48 8D 0D FC 80 61 00     LEA RCX,[0x140996fb0]
14037eeb4:  FF 15 BE D9 4E 00        CALL qword ptr [0x14086c878]
14037eeba:  90                       NOP
14037eebb:  48 8D 8D 08 03 00 00     LEA RCX,[RBP + 0x308]
14037eec2:  E8 F9 2B C8 FF           CALL 0x140001ac0
14037eec7:  F3 0F 10 0D B9 31 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037eecf:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037eed4:  41 0F 28 C2              MOVAPS XMM0,XMM10
14037eed8:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037eedc:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037eee0:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037eee4:  F3 0F 10 7C 24 78        MOVSS XMM7,dword ptr [RSP + 0x78]
14037eeea:  44 0F 28 DF              MOVAPS XMM11,XMM7
14037eeee:  F3 45 0F 58 DA           ADDSS XMM11,XMM10
14037eef3:  F3 41 0F 58 CB           ADDSS XMM1,XMM11
14037eef8:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037eefc:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037ef00:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037ef05:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037ef0b:  41 B8 06 00 00 00        MOV R8D,0x6
14037ef11:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037ef16:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037ef1d:  E8 1E 76 0C 00           CALL 0x140446540
14037ef22:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037ef27:  0F 84 98 01 00 00        JZ 0x14037f0c5
14037ef2d:  66 41 0F 6E C6           MOVD XMM0,R14D
14037ef32:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ef35:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037ef3a:  0F 2F C8                 COMISS XMM1,XMM0
14037ef3d:  0F 86 82 01 00 00        JBE 0x14037f0c5
14037ef43:  66 41 0F 6E C5           MOVD XMM0,R13D
14037ef48:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ef4b:  0F 2F C1                 COMISS XMM0,XMM1
14037ef4e:  0F 86 71 01 00 00        JBE 0x14037f0c5
14037ef54:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037ef59:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037ef5e:  66 0F 6E C3              MOVD XMM0,EBX
14037ef62:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ef65:  0F 2F C8                 COMISS XMM1,XMM0
14037ef68:  0F 86 57 01 00 00        JBE 0x14037f0c5
14037ef6e:  66 0F 6E C7              MOVD XMM0,EDI
14037ef72:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037ef75:  0F 2F C1                 COMISS XMM0,XMM1
14037ef78:  0F 86 47 01 00 00        JBE 0x14037f0c5
14037ef7e:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037ef85:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037ef88:  48 8D 0D 41 BD 66 00     LEA RCX,[0x1409eacd0]
14037ef8f:  FF 15 5B D7 4E 00        CALL qword ptr [0x14086c6f0]
14037ef95:  4C 8B C0                 MOV R8,RAX
14037ef98:  33 D2                    XOR EDX,EDX
14037ef9a:  48 8B CB                 MOV RCX,RBX
14037ef9d:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037efa0:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037efa7:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037efab:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037efaf:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037efb3:  48 8D 8D E0 09 00 00     LEA RCX,[RBP + 0x9e0]
14037efba:  FF 15 60 D0 4E 00        CALL qword ptr [0x14086c020]
14037efc0:  48 8B D0                 MOV RDX,RAX
14037efc3:  48 8D 0D 96 F0 63 00     LEA RCX,[0x1409be060]
14037efca:  FF 15 C8 D8 4E 00        CALL qword ptr [0x14086c898]
14037efd0:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037efd4:  48 8D 0D 85 F0 63 00     LEA RCX,[0x1409be060]
14037efdb:  FF 15 77 D9 4E 00        CALL qword ptr [0x14086c958]
14037efe1:  66 0F 7F 74 24 50        MOVDQA xmmword ptr [RSP + 0x50],XMM6
14037efe7:  F3 0F 10 15 99 30 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037efef:  F3 0F 59 15 31 B8 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037eff7:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037effe:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037f005:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037f00c:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037f013:  0F 28 DA                 MOVAPS XMM3,XMM2
14037f016:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037f01b:  48 8D 0D 3E F0 63 00     LEA RCX,[0x1409be060]
14037f022:  FF 15 30 D8 4E 00        CALL qword ptr [0x14086c858]
14037f028:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037f02c:  48 8D 0D 2D F0 63 00     LEA RCX,[0x1409be060]
14037f033:  FF 15 1F D9 4E 00        CALL qword ptr [0x14086c958]
14037f039:  41 B4 01                 MOV R12B,0x1
14037f03c:  41 83 C8 FF              OR R8D,0xffffffff
14037f040:  48 8D 15 A9 08 54 00     LEA RDX,[0x1408bf8f0]
14037f047:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f04c:  E8 5F BD ED FF           CALL 0x14025adb0
14037f051:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037f056:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f05a:  E8 81 BC 0C 00           CALL 0x14044ace0
14037f05f:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f064:  E8 57 2A C8 FF           CALL 0x140001ac0
14037f069:  41 83 C8 FF              OR R8D,0xffffffff
14037f06d:  48 8D 15 64 08 54 00     LEA RDX,[0x1408bf8d8]
14037f074:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f079:  E8 32 BD ED FF           CALL 0x14025adb0
14037f07e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037f083:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f087:  E8 54 BC 0C 00           CALL 0x14044ace0
14037f08c:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f091:  E8 2A 2A C8 FF           CALL 0x140001ac0
14037f096:  41 83 C8 FF              OR R8D,0xffffffff
14037f09a:  48 8D 15 17 08 54 00     LEA RDX,[0x1408bf8b8]
14037f0a1:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f0a6:  E8 05 BD ED FF           CALL 0x14025adb0
14037f0ab:  90                       NOP
14037f0ac:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
14037f0b1:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f0b5:  E8 36 BD 0C 00           CALL 0x14044adf0
14037f0ba:  90                       NOP
14037f0bb:  48 8D 4C 24 50           LEA RCX,[RSP + 0x50]
14037f0c0:  E8 FB 29 C8 FF           CALL 0x140001ac0
14037f0c5:  F3 0F 10 35 BB 2F 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037f0cd:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037f0d2:  BA EC 05 00 00           MOV EDX,0x5ec
14037f0d7:  48 8D 0D B2 84 61 00     LEA RCX,[0x140997590]
14037f0de:  FF 15 AC D7 4E 00        CALL qword ptr [0x14086c890]
14037f0e4:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037f0e8:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037f0ec:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037f0f1:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f0f6:  48 8D 05 AB 07 54 00     LEA RAX,[0x1408bf8a8]
14037f0fd:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f102:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f10a:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f10f:  0F 28 D6                 MOVAPS XMM2,XMM6
14037f112:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f117:  49 8B D7                 MOV RDX,R15
14037f11a:  48 8D 0D 8F 7E 61 00     LEA RCX,[0x140996fb0]
14037f121:  FF 15 59 D7 4E 00        CALL qword ptr [0x14086c880]
14037f127:  F3 0F 10 95 34 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x134]
14037f12f:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037f132:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037f137:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037f13c:  45 33 C9                 XOR R9D,R9D
14037f13f:  48 8D 95 18 03 00 00     LEA RDX,[RBP + 0x318]
14037f146:  48 8B CE                 MOV RCX,RSI
14037f149:  E8 12 7B FB FF           CALL 0x140336c60
14037f14e:  90                       NOP
14037f14f:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037f152:  F3 0F 10 15 2E 2F 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037f15a:  0F 28 C2                 MOVAPS XMM0,XMM2
14037f15d:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037f162:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
14037f167:  F3 0F 59 15 F9 B7 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037f16f:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037f175:  66 0F 6E 05 0F 61 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037f17d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f180:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037f184:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037f189:  48 8D 05 D8 07 54 00     LEA RAX,[0x1408bf968]
14037f190:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f195:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f19d:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037f1a1:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f1a6:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f1ab:  49 8B D7                 MOV RDX,R15
14037f1ae:  48 8D 0D FB 7D 61 00     LEA RCX,[0x140996fb0]
14037f1b5:  FF 15 BD D6 4E 00        CALL qword ptr [0x14086c878]
14037f1bb:  90                       NOP
14037f1bc:  48 8D 8D 18 03 00 00     LEA RCX,[RBP + 0x318]
14037f1c3:  E8 F8 28 C8 FF           CALL 0x140001ac0
14037f1c8:  F3 0F 10 0D B8 2E 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037f1d0:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037f1d5:  41 0F 28 C3              MOVAPS XMM0,XMM11
14037f1d9:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037f1dd:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
14037f1e1:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
14037f1e5:  F3 41 0F 58 FB           ADDSS XMM7,XMM11
14037f1ea:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037f1ee:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
14037f1f2:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
14037f1f6:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
14037f1fb:  66 0F 7F 74 24 60        MOVDQA xmmword ptr [RSP + 0x60],XMM6
14037f201:  41 B8 06 00 00 00        MOV R8D,0x6
14037f207:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037f20c:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037f213:  E8 28 73 0C 00           CALL 0x140446540
14037f218:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14037f21d:  0F 84 A0 01 00 00        JZ 0x14037f3c3
14037f223:  66 41 0F 6E C6           MOVD XMM0,R14D
14037f228:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f22b:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14037f230:  0F 2F C8                 COMISS XMM1,XMM0
14037f233:  0F 86 8A 01 00 00        JBE 0x14037f3c3
14037f239:  66 41 0F 6E C5           MOVD XMM0,R13D
14037f23e:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f241:  0F 2F C1                 COMISS XMM0,XMM1
14037f244:  0F 86 79 01 00 00        JBE 0x14037f3c3
14037f24a:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037f24f:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14037f254:  66 0F 6E C3              MOVD XMM0,EBX
14037f258:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f25b:  0F 2F C8                 COMISS XMM1,XMM0
14037f25e:  0F 86 5F 01 00 00        JBE 0x14037f3c3
14037f264:  66 0F 6E C7              MOVD XMM0,EDI
14037f268:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f26b:  0F 2F C1                 COMISS XMM0,XMM1
14037f26e:  0F 86 4F 01 00 00        JBE 0x14037f3c3
14037f274:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14037f27b:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037f27e:  48 8D 0D 4B BA 66 00     LEA RCX,[0x1409eacd0]
14037f285:  FF 15 65 D4 4E 00        CALL qword ptr [0x14086c6f0]
14037f28b:  4C 8B C0                 MOV R8,RAX
14037f28e:  33 D2                    XOR EDX,EDX
14037f290:  48 8B CB                 MOV RCX,RBX
14037f293:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037f296:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
14037f29d:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037f2a1:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037f2a5:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037f2a9:  48 8D 8D F0 09 00 00     LEA RCX,[RBP + 0x9f0]
14037f2b0:  FF 15 6A CD 4E 00        CALL qword ptr [0x14086c020]
14037f2b6:  48 8B D0                 MOV RDX,RAX
14037f2b9:  48 8D 0D A0 ED 63 00     LEA RCX,[0x1409be060]
14037f2c0:  FF 15 D2 D5 4E 00        CALL qword ptr [0x14086c898]
14037f2c6:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037f2ca:  48 8D 0D 8F ED 63 00     LEA RCX,[0x1409be060]
14037f2d1:  FF 15 81 D6 4E 00        CALL qword ptr [0x14086c958]
14037f2d7:  66 0F 7F 74 24 60        MOVDQA xmmword ptr [RSP + 0x60],XMM6
14037f2dd:  F3 0F 10 15 A3 2D 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037f2e5:  F3 44 0F 10 35 3A B5 58 00 MOVSS XMM14,dword ptr [0x14090a828]
14037f2ee:  F3 41 0F 59 D6           MULSS XMM2,XMM14
14037f2f3:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14037f2fa:  F3 44 0F 11 6C 24 30     MOVSS dword ptr [RSP + 0x30],XMM13
14037f301:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14037f308:  F3 44 0F 11 6C 24 20     MOVSS dword ptr [RSP + 0x20],XMM13
14037f30f:  0F 28 DA                 MOVAPS XMM3,XMM2
14037f312:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037f317:  48 8D 0D 42 ED 63 00     LEA RCX,[0x1409be060]
14037f31e:  FF 15 34 D5 4E 00        CALL qword ptr [0x14086c858]
14037f324:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037f328:  48 8D 0D 31 ED 63 00     LEA RCX,[0x1409be060]
14037f32f:  FF 15 23 D6 4E 00        CALL qword ptr [0x14086c958]
14037f335:  41 B4 01                 MOV R12B,0x1
14037f338:  41 83 C8 FF              OR R8D,0xffffffff
14037f33c:  48 8D 15 05 06 54 00     LEA RDX,[0x1408bf948]
14037f343:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f348:  E8 63 BA ED FF           CALL 0x14025adb0
14037f34d:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037f352:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f356:  E8 85 B9 0C 00           CALL 0x14044ace0
14037f35b:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f360:  E8 5B 27 C8 FF           CALL 0x140001ac0
14037f365:  41 83 C8 FF              OR R8D,0xffffffff
14037f369:  48 8D 15 C0 05 54 00     LEA RDX,[0x1408bf930]
14037f370:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f375:  E8 36 BA ED FF           CALL 0x14025adb0
14037f37a:  90                       NOP
14037f37b:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037f380:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f384:  E8 67 BA 0C 00           CALL 0x14044adf0
14037f389:  90                       NOP
14037f38a:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f38f:  E8 2C 27 C8 FF           CALL 0x140001ac0
14037f394:  41 83 C8 FF              OR R8D,0xffffffff
14037f398:  48 8D 15 71 05 54 00     LEA RDX,[0x1408bf910]
14037f39f:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f3a4:  E8 07 BA ED FF           CALL 0x14025adb0
14037f3a9:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
14037f3ae:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14037f3b2:  E8 29 B9 0C 00           CALL 0x14044ace0
14037f3b7:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
14037f3bc:  E8 FF 26 C8 FF           CALL 0x140001ac0
14037f3c1:  EB 09                    JMP 0x14037f3cc
14037f3c3:  F3 44 0F 10 35 5C B4 58 00 MOVSS XMM14,dword ptr [0x14090a828]
14037f3cc:  F3 0F 10 0D AC AE 58 00  MOVSS XMM1,dword ptr [0x14090a280]
14037f3d4:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f3d7:  48 8D 0D 82 EC 63 00     LEA RCX,[0x1409be060]
14037f3de:  FF 15 C4 D4 4E 00        CALL qword ptr [0x14086c8a8]
14037f3e4:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037f3e8:  F3 0F 10 0D 14 C9 58 00  MOVSS XMM1,dword ptr [0x14090bd04]
14037f3f0:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f3f3:  48 8D 8D 90 04 00 00     LEA RCX,[RBP + 0x490]
14037f3fa:  FF 15 08 CC 4E 00        CALL qword ptr [0x14086c008]
14037f400:  48 8B D0                 MOV RDX,RAX
14037f403:  48 8D 0D 56 EC 63 00     LEA RCX,[0x1409be060]
14037f40a:  FF 15 90 D4 4E 00        CALL qword ptr [0x14086c8a0]
14037f410:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037f415:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037f41c:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037f423:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037f427:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037f42b:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037f42f:  48 8D 0D 2A EC 63 00     LEA RCX,[0x1409be060]
14037f436:  FF 15 24 D4 4E 00        CALL qword ptr [0x14086c860]
14037f43c:  F3 0F 10 35 44 2C 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037f444:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037f449:  BA ED 05 00 00           MOV EDX,0x5ed
14037f44e:  48 8D 0D 3B 81 61 00     LEA RCX,[0x140997590]
14037f455:  FF 15 35 D4 4E 00        CALL qword ptr [0x14086c890]
14037f45b:  41 0F 28 DB              MOVAPS XMM3,XMM11
14037f45f:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037f463:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037f468:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f46d:  48 8D 05 DC 09 54 00     LEA RAX,[0x1408bfe50]
14037f474:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f479:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f481:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f486:  0F 28 D6                 MOVAPS XMM2,XMM6
14037f489:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f48e:  49 8B D7                 MOV RDX,R15
14037f491:  48 8D 0D 18 7B 61 00     LEA RCX,[0x140996fb0]
14037f498:  FF 15 E2 D3 4E 00        CALL qword ptr [0x14086c880]
14037f49e:  F3 0F 10 95 44 01 00 00  MOVSS XMM2,dword ptr [RBP + 0x144]
14037f4a6:  0F 5A D2                 CVTPS2PD XMM2,XMM2
14037f4a9:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14037f4ae:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14037f4b3:  45 33 C9                 XOR R9D,R9D
14037f4b6:  48 8D 95 28 03 00 00     LEA RDX,[RBP + 0x328]
14037f4bd:  48 8B CE                 MOV RCX,RSI
14037f4c0:  E8 9B 77 FB FF           CALL 0x140336c60
14037f4c5:  90                       NOP
14037f4c6:  48 8B 08                 MOV RCX,qword ptr [RAX]
14037f4c9:  F3 0F 10 15 B7 2B 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037f4d1:  0F 28 C2                 MOVAPS XMM0,XMM2
14037f4d4:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037f4d9:  F3 44 0F 5C D8           SUBSS XMM11,XMM0
14037f4de:  F3 44 0F 10 0D 81 B4 58 00 MOVSS XMM9,dword ptr [0x14090a968]
14037f4e7:  F3 41 0F 59 D1           MULSS XMM2,XMM9
14037f4ec:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
14037f4f2:  66 0F 6E 05 92 5D 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037f4fa:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f4fd:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037f501:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14037f506:  48 8D 05 6B 08 54 00     LEA RAX,[0x1408bfd78]
14037f50d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f512:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f51a:  41 0F 28 DB              MOVAPS XMM3,XMM11
14037f51e:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f523:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f528:  49 8B D7                 MOV RDX,R15
14037f52b:  48 8D 0D 7E 7A 61 00     LEA RCX,[0x140996fb0]
14037f532:  FF 15 40 D3 4E 00        CALL qword ptr [0x14086c878]
14037f538:  90                       NOP
14037f539:  48 8D 8D 28 03 00 00     LEA RCX,[RBP + 0x328]
14037f540:  E8 7B 25 C8 FF           CALL 0x140001ac0
14037f545:  F3 0F 10 35 3B 2B 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037f54d:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037f552:  BA E6 05 00 00           MOV EDX,0x5e6
14037f557:  48 8D 0D 32 80 61 00     LEA RCX,[0x140997590]
14037f55e:  FF 15 2C D3 4E 00        CALL qword ptr [0x14086c890]
14037f564:  0F 28 DF                 MOVAPS XMM3,XMM7
14037f567:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037f56b:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
14037f570:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f575:  48 8D 05 C4 08 54 00     LEA RAX,[0x1408bfe40]
14037f57c:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f581:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f589:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f58e:  0F 28 D6                 MOVAPS XMM2,XMM6
14037f591:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f596:  49 8B D7                 MOV RDX,R15
14037f599:  48 8D 0D 10 7A 61 00     LEA RCX,[0x140996fb0]
14037f5a0:  FF 15 DA D2 4E 00        CALL qword ptr [0x14086c880]
14037f5a6:  F3 0F 10 8D 44 01 00 00  MOVSS XMM1,dword ptr [RBP + 0x144]
14037f5ae:  F3 0F 58 8D 34 01 00 00  ADDSS XMM1,dword ptr [RBP + 0x134]
14037f5b6:  F3 0F 58 8D 24 01 00 00  ADDSS XMM1,dword ptr [RBP + 0x124]
14037f5be:  F3 0F 10 05 C6 A4 58 00  MOVSS XMM0,dword ptr [0x140909a8c]
14037f5c6:  0F 2F C1                 COMISS XMM0,XMM1
14037f5c9:  76 64                    JBE 0x14037f62f
14037f5cb:  F3 0F 10 15 B5 2A 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037f5d3:  0F 28 C2                 MOVAPS XMM0,XMM2
14037f5d6:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037f5db:  0F 28 DF                 MOVAPS XMM3,XMM7
14037f5de:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037f5e2:  F3 41 0F 59 D1           MULSS XMM2,XMM9
14037f5e7:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037f5ed:  66 0F 6E 05 97 5C 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037f5f5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f5f8:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037f5fc:  48 8D 05 2D 08 54 00     LEA RAX,[0x1408bfe30]
14037f603:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f608:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f610:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f615:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f61a:  49 8B D7                 MOV RDX,R15
14037f61d:  48 8D 0D 8C 79 61 00     LEA RCX,[0x140996fb0]
14037f624:  FF 15 4E D2 4E 00        CALL qword ptr [0x14086c878]
14037f62a:  E9 AC 00 00 00           JMP 0x14037f6db
14037f62f:  45 33 C9                 XOR R9D,R9D
14037f632:  41 B0 01                 MOV R8B,0x1
14037f635:  48 8B CE                 MOV RCX,RSI
14037f638:  44 39 8E D0 C7 00 00     CMP dword ptr [RSI + 0xc7d0],R9D
14037f63f:  75 22                    JNZ 0x14037f663
14037f641:  F3 0F 10 85 48 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x148]
14037f649:  F3 0F 5E C1              DIVSS XMM0,XMM1
14037f64d:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037f650:  E8 CB 6D FB FF           CALL 0x140336420
14037f655:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f65a:  48 8D 05 2F 08 54 00     LEA RAX,[0x1408bfe90]
14037f661:  EB 20                    JMP 0x14037f683
14037f663:  F3 0F 10 85 4C 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x14c]
14037f66b:  F3 0F 5E C1              DIVSS XMM0,XMM1
14037f66f:  0F 5A C8                 CVTPS2PD XMM1,XMM0
14037f672:  E8 A9 6D FB FF           CALL 0x140336420
14037f677:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f67c:  48 8D 05 FD 07 54 00     LEA RAX,[0x1408bfe80]
14037f683:  F3 0F 10 15 FD 29 61 00  MOVSS XMM2,dword ptr [0x140992088]
14037f68b:  0F 28 C2                 MOVAPS XMM0,XMM2
14037f68e:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037f693:  0F 28 DF                 MOVAPS XMM3,XMM7
14037f696:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037f69a:  66 0F 6E 05 EA 5B 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037f6a2:  F3 41 0F 59 D1           MULSS XMM2,XMM9
14037f6a7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037f6aa:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14037f6b0:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f6b5:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037f6b9:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037f6c1:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f6c6:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f6cb:  49 8B D7                 MOV RDX,R15
14037f6ce:  48 8D 0D DB 78 61 00     LEA RCX,[0x140996fb0]
14037f6d5:  FF 15 9D D1 4E 00        CALL qword ptr [0x14086c878]
14037f6db:  F3 44 0F 10 54 24 78     MOVSS XMM10,dword ptr [RSP + 0x78]
14037f6e2:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037f6e7:  F3 0F 10 35 99 29 61 00  MOVSS XMM6,dword ptr [0x140992088]
14037f6ef:  0F 28 C6                 MOVAPS XMM0,XMM6
14037f6f2:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14037f6f7:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
14037f6fc:  E9 B4 2D 00 00           JMP 0x1403824b5
14037f701:  83 F9 02                 CMP ECX,0x2
14037f704:  0F 85 32 16 00 00        JNZ 0x140380d3c
14037f70a:  83 FA FF                 CMP EDX,-0x1
14037f70d:  0F 85 29 16 00 00        JNZ 0x140380d3c
14037f713:  48 8B 86 E8 C7 00 00     MOV RAX,qword ptr [RSI + 0xc7e8]
14037f71a:  48 89 44 24 50           MOV qword ptr [RSP + 0x50],RAX
14037f71f:  48 8D 4D 38              LEA RCX,[RBP + 0x38]
14037f723:  E8 48 67 CA FF           CALL 0x140025e70
14037f728:  90                       NOP
14037f729:  4C 89 B6 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],R14
14037f730:  44 0F 28 DE              MOVAPS XMM11,XMM6
14037f734:  F3 44 0F 59 1D 17 B7 58 00 MULSS XMM11,dword ptr [0x14090ae54]
14037f73d:  F3 45 0F 58 DF           ADDSS XMM11,XMM15
14037f742:  F3 44 0F 11 5D C4        MOVSS dword ptr [RBP + -0x3c],XMM11
14037f748:  44 0F 28 F6              MOVAPS XMM14,XMM6
14037f74c:  F3 44 0F 59 35 AB B6 58 00 MULSS XMM14,dword ptr [0x14090ae00]
14037f755:  F3 45 0F 58 F7           ADDSS XMM14,XMM15
14037f75a:  F3 44 0F 11 75 BC        MOVSS dword ptr [RBP + -0x44],XMM14
14037f760:  0F 28 C6                 MOVAPS XMM0,XMM6
14037f763:  F3 0F 59 05 3D B7 58 00  MULSS XMM0,dword ptr [0x14090aea8]
14037f76b:  F3 41 0F 58 C7           ADDSS XMM0,XMM15
14037f770:  F3 0F 11 45 B0           MOVSS dword ptr [RBP + -0x50],XMM0
14037f775:  44 0F 28 EE              MOVAPS XMM13,XMM6
14037f779:  F3 44 0F 59 2D D6 B7 58 00 MULSS XMM13,dword ptr [0x14090af58]
14037f782:  F3 45 0F 58 EF           ADDSS XMM13,XMM15
14037f787:  0F 28 C6                 MOVAPS XMM0,XMM6
14037f78a:  F3 0F 59 05 8E B7 58 00  MULSS XMM0,dword ptr [0x14090af20]
14037f792:  F3 41 0F 58 C7           ADDSS XMM0,XMM15
14037f797:  F3 0F 11 44 24 70        MOVSS dword ptr [RSP + 0x70],XMM0
14037f79d:  44 0F 28 FE              MOVAPS XMM15,XMM6
14037f7a1:  F3 44 0F 59 3D DE B7 58 00 MULSS XMM15,dword ptr [0x14090af88]
14037f7aa:  F3 44 0F 58 7C 24 74     ADDSS XMM15,dword ptr [RSP + 0x74]
14037f7b1:  F3 44 0F 11 7D F8        MOVSS dword ptr [RBP + -0x8],XMM15
14037f7b7:  0F 28 FE                 MOVAPS XMM7,XMM6
14037f7ba:  F3 0F 59 3D 9A B1 58 00  MULSS XMM7,dword ptr [0x14090a95c]
14037f7c2:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14037f7c7:  BA 80 06 00 00           MOV EDX,0x680
14037f7cc:  48 8D 0D BD 7D 61 00     LEA RCX,[0x140997590]
14037f7d3:  FF 15 B7 D0 4E 00        CALL qword ptr [0x14086c890]
14037f7d9:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037f7dd:  F3 0F 5C DE              SUBSS XMM3,XMM6
14037f7e1:  F3 0F 58 74 24 74        ADDSS XMM6,dword ptr [RSP + 0x74]
14037f7e7:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037f7ec:  48 8D 05 7D 06 54 00     LEA RAX,[0x1408bfe70]
14037f7f3:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037f7f8:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14037f800:  66 41 0F 7E D9           MOVD R9D,XMM3
14037f805:  0F 28 D6                 MOVAPS XMM2,XMM6
14037f808:  66 41 0F 7E D0           MOVD R8D,XMM2
14037f80d:  48 8B 15 2C 5A 61 00     MOV RDX,qword ptr [0x140995240]
14037f814:  48 8D 0D 95 77 61 00     LEA RCX,[0x140996fb0]
14037f81b:  FF 15 5F D0 4E 00        CALL qword ptr [0x14086c880]
14037f821:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037f826:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
14037f82d:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037f830:  48 8D 0D 99 B4 66 00     LEA RCX,[0x1409eacd0]
14037f837:  FF 15 B3 CE 4E 00        CALL qword ptr [0x14086c6f0]
14037f83d:  4C 8B C0                 MOV R8,RAX
14037f840:  33 D2                    XOR EDX,EDX
14037f842:  48 8B CB                 MOV RCX,RBX
14037f845:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037f848:  F3 0F 10 0D 38 28 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037f850:  F3 0F 59 0D 50 B2 58 00  MULSS XMM1,dword ptr [0x14090aaa8]
14037f858:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f85b:  48 8D 0D FE E7 63 00     LEA RCX,[0x1409be060]
14037f862:  FF 15 40 D0 4E 00        CALL qword ptr [0x14086c8a8]
14037f868:  F3 0F 10 0D 18 28 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037f870:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f873:  F3 0F 59 15 ED B0 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037f87b:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037f880:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037f885:  F3 41 0F 58 CB           ADDSS XMM1,XMM11
14037f88a:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037f88e:  48 8D 8D 9C 04 00 00     LEA RCX,[RBP + 0x49c]
14037f895:  FF 15 6D C7 4E 00        CALL qword ptr [0x14086c008]
14037f89b:  48 8B D0                 MOV RDX,RAX
14037f89e:  48 8D 0D BB E7 63 00     LEA RCX,[0x1409be060]
14037f8a5:  FF 15 F5 CF 4E 00        CALL qword ptr [0x14086c8a0]
14037f8ab:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037f8b2:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037f8b6:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037f8ba:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037f8be:  48 8D 8D 00 0A 00 00     LEA RCX,[RBP + 0xa00]
14037f8c5:  FF 15 55 C7 4E 00        CALL qword ptr [0x14086c020]
14037f8cb:  48 8B D0                 MOV RDX,RAX
14037f8ce:  48 8D 0D 8B E7 63 00     LEA RCX,[0x1409be060]
14037f8d5:  FF 15 BD CF 4E 00        CALL qword ptr [0x14086c898]
14037f8db:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037f8df:  48 8D 8D 00 05 00 00     LEA RCX,[RBP + 0x500]
14037f8e6:  FF 15 24 C7 4E 00        CALL qword ptr [0x14086c010]
14037f8ec:  48 8B D0                 MOV RDX,RAX
14037f8ef:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037f8f3:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037f8f7:  48 8D 0D 62 E7 63 00     LEA RCX,[0x1409be060]
14037f8fe:  FF 15 4C CF 4E 00        CALL qword ptr [0x14086c850]
14037f904:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037f909:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037f910:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037f917:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037f91b:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037f91f:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037f923:  48 8D 0D 36 E7 63 00     LEA RCX,[0x1409be060]
14037f92a:  FF 15 30 CF 4E 00        CALL qword ptr [0x14086c860]
14037f930:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
14037f937:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14037f93a:  48 8D 0D 8F B3 66 00     LEA RCX,[0x1409eacd0]
14037f941:  FF 15 A9 CD 4E 00        CALL qword ptr [0x14086c6f0]
14037f947:  4C 8B C0                 MOV R8,RAX
14037f94a:  33 D2                    XOR EDX,EDX
14037f94c:  48 8B CB                 MOV RCX,RBX
14037f94f:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14037f952:  F3 0F 10 0D 2E 27 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037f95a:  F3 0F 59 0D 46 B1 58 00  MULSS XMM1,dword ptr [0x14090aaa8]
14037f962:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f965:  48 8D 0D F4 E6 63 00     LEA RCX,[0x1409be060]
14037f96c:  FF 15 36 CF 4E 00        CALL qword ptr [0x14086c8a8]
14037f972:  F3 0F 10 0D 0E 27 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037f97a:  0F 28 D1                 MOVAPS XMM2,XMM1
14037f97d:  F3 0F 59 15 E3 AF 58 00  MULSS XMM2,dword ptr [0x14090a968]
14037f985:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
14037f98a:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14037f98f:  F3 41 0F 58 CD           ADDSS XMM1,XMM13
14037f994:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037f998:  48 8D 8D F0 04 00 00     LEA RCX,[RBP + 0x4f0]
14037f99f:  FF 15 63 C6 4E 00        CALL qword ptr [0x14086c008]
14037f9a5:  48 8B D0                 MOV RDX,RAX
14037f9a8:  48 8D 0D B1 E6 63 00     LEA RCX,[0x1409be060]
14037f9af:  FF 15 EB CE 4E 00        CALL qword ptr [0x14086c8a0]
14037f9b5:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037f9bc:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037f9c0:  41 0F 28 D1              MOVAPS XMM2,XMM9
14037f9c4:  41 0F 28 C9              MOVAPS XMM1,XMM9
14037f9c8:  48 8D 8D 10 0A 00 00     LEA RCX,[RBP + 0xa10]
14037f9cf:  FF 15 4B C6 4E 00        CALL qword ptr [0x14086c020]
14037f9d5:  48 8B D0                 MOV RDX,RAX
14037f9d8:  48 8D 0D 81 E6 63 00     LEA RCX,[0x1409be060]
14037f9df:  FF 15 B3 CE 4E 00        CALL qword ptr [0x14086c898]
14037f9e5:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14037f9e9:  48 8D 8D E0 04 00 00     LEA RCX,[RBP + 0x4e0]
14037f9f0:  FF 15 1A C6 4E 00        CALL qword ptr [0x14086c010]
14037f9f6:  48 8B D0                 MOV RDX,RAX
14037f9f9:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037f9fd:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037fa01:  48 8D 0D 58 E6 63 00     LEA RCX,[0x1409be060]
14037fa08:  FF 15 42 CE 4E 00        CALL qword ptr [0x14086c850]
14037fa0e:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14037fa13:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14037fa1a:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14037fa21:  41 0F 28 D9              MOVAPS XMM3,XMM9
14037fa25:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037fa29:  41 0F 28 C8              MOVAPS XMM1,XMM8
14037fa2d:  48 8D 0D 2C E6 63 00     LEA RCX,[0x1409be060]
14037fa34:  FF 15 26 CE 4E 00        CALL qword ptr [0x14086c860]
14037fa3a:  F3 0F 59 3D 1E A6 58 00  MULSS XMM7,dword ptr [0x14090a060]
14037fa42:  F3 44 0F 58 D7           ADDSS XMM10,XMM7
14037fa47:  BA 88 06 00 00           MOV EDX,0x688
14037fa4c:  48 8D 0D 3D 7B 61 00     LEA RCX,[0x140997590]
14037fa53:  FF 15 37 CE 4E 00        CALL qword ptr [0x14086c890]
14037fa59:  F3 0F 10 05 27 26 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037fa61:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037fa66:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037fa6a:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037fa6e:  66 0F 6E 0D 16 58 61 00  MOVD XMM1,dword ptr [0x14099528c]
14037fa76:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
14037fa79:  F3 44 0F 10 25 5A A3 58 00 MOVSS XMM12,dword ptr [0x140909ddc]
14037fa82:  F3 41 0F 59 CC           MULSS XMM1,XMM12
14037fa87:  41 0F 28 D6              MOVAPS XMM2,XMM14
14037fa8b:  F3 0F 5C D1              SUBSS XMM2,XMM1
14037fa8f:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037fa94:  48 8D 05 C5 03 54 00     LEA RAX,[0x1408bfe60]
14037fa9b:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037faa0:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037faa8:  66 41 0F 7E D9           MOVD R9D,XMM3
14037faad:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fab2:  48 8B 15 4F 47 61 00     MOV RDX,qword ptr [0x140994208]
14037fab9:  48 8D 0D F0 74 61 00     LEA RCX,[0x140996fb0]
14037fac0:  FF 15 C2 CD 4E 00        CALL qword ptr [0x14086c888]
14037fac6:  BA 89 06 00 00           MOV EDX,0x689
14037facb:  48 8D 0D BE 7A 61 00     LEA RCX,[0x140997590]
14037fad2:  FF 15 B8 CD 4E 00        CALL qword ptr [0x14086c890]
14037fad8:  F3 0F 10 05 A8 25 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037fae0:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037fae5:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037fae9:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037faed:  66 0F 6E 05 97 57 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037faf5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037faf8:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14037fafd:  F3 0F 10 55 B0           MOVSS XMM2,dword ptr [RBP + -0x50]
14037fb02:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037fb06:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037fb0b:  48 8D 05 56 01 54 00     LEA RAX,[0x1408bfc68]
14037fb12:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fb17:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037fb1f:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fb24:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fb29:  48 8B 15 D8 46 61 00     MOV RDX,qword ptr [0x140994208]
14037fb30:  48 8D 0D 79 74 61 00     LEA RCX,[0x140996fb0]
14037fb37:  FF 15 4B CD 4E 00        CALL qword ptr [0x14086c888]
14037fb3d:  BA 88 06 00 00           MOV EDX,0x688
14037fb42:  48 8D 0D 47 7A 61 00     LEA RCX,[0x140997590]
14037fb49:  FF 15 41 CD 4E 00        CALL qword ptr [0x14086c890]
14037fb4f:  F3 0F 10 05 31 25 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037fb57:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037fb5c:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037fb60:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037fb64:  66 0F 6E 05 20 57 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037fb6c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037fb6f:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14037fb74:  F3 0F 10 54 24 70        MOVSS XMM2,dword ptr [RSP + 0x70]
14037fb7a:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037fb7e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037fb83:  48 8D 05 CE 00 54 00     LEA RAX,[0x1408bfc58]
14037fb8a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fb8f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037fb97:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fb9c:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fba1:  48 8B 15 60 46 61 00     MOV RDX,qword ptr [0x140994208]
14037fba8:  48 8D 0D 01 74 61 00     LEA RCX,[0x140996fb0]
14037fbaf:  FF 15 D3 CC 4E 00        CALL qword ptr [0x14086c888]
14037fbb5:  BA 89 06 00 00           MOV EDX,0x689
14037fbba:  48 8D 0D CF 79 61 00     LEA RCX,[0x140997590]
14037fbc1:  FF 15 C9 CC 4E 00        CALL qword ptr [0x14086c890]
14037fbc7:  F3 0F 10 05 B9 24 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037fbcf:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037fbd4:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037fbd8:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037fbdc:  66 0F 6E 05 A8 56 61 00  MOVD XMM0,dword ptr [0x14099528c]
14037fbe4:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14037fbe7:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14037fbec:  41 0F 28 D7              MOVAPS XMM2,XMM15
14037fbf0:  F3 0F 5C D0              SUBSS XMM2,XMM0
14037fbf4:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037fbf9:  48 8D 05 48 00 54 00     LEA RAX,[0x1408bfc48]
14037fc00:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fc05:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037fc0d:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fc12:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fc17:  48 8B 15 EA 45 61 00     MOV RDX,qword ptr [0x140994208]
14037fc1e:  48 8D 0D 8B 73 61 00     LEA RCX,[0x140996fb0]
14037fc25:  FF 15 5D CC 4E 00        CALL qword ptr [0x14086c888]
14037fc2b:  48 8B 05 D6 45 61 00     MOV RAX,qword ptr [0x140994208]
14037fc32:  48 8B 18                 MOV RBX,qword ptr [RAX]
14037fc35:  BA 88 06 00 00           MOV EDX,0x688
14037fc3a:  48 8D 0D 4F 79 61 00     LEA RCX,[0x140997590]
14037fc41:  FF 15 49 CC 4E 00        CALL qword ptr [0x14086c890]
14037fc47:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fc4c:  48 8D 05 ED FF 53 00     LEA RAX,[0x1408bfc40]
14037fc53:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14037fc58:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037fc5c:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fc61:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037fc65:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fc6a:  48 8D 95 00 0B 00 00     LEA RDX,[RBP + 0xb00]
14037fc71:  48 8B 0D 90 45 61 00     MOV RCX,qword ptr [0x140994208]
14037fc78:  FF 53 28                 CALL qword ptr [RBX + 0x28]
14037fc7b:  0F 10 30                 MOVUPS XMM6,xmmword ptr [RAX]
14037fc7e:  48 8B 05 83 45 61 00     MOV RAX,qword ptr [0x140994208]
14037fc85:  48 8B 18                 MOV RBX,qword ptr [RAX]
14037fc88:  BA 89 06 00 00           MOV EDX,0x689
14037fc8d:  48 8D 0D FC 78 61 00     LEA RCX,[0x140997590]
14037fc94:  FF 15 F6 CB 4E 00        CALL qword ptr [0x14086c890]
14037fc9a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fc9f:  48 8D 05 D2 FF 53 00     LEA RAX,[0x1408bfc78]
14037fca6:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14037fcab:  41 0F 28 D8              MOVAPS XMM3,XMM8
14037fcaf:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fcb4:  41 0F 28 D0              MOVAPS XMM2,XMM8
14037fcb8:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fcbd:  48 8D 95 10 0B 00 00     LEA RDX,[RBP + 0xb10]
14037fcc4:  48 8B 0D 3D 45 61 00     MOV RCX,qword ptr [0x140994208]
14037fccb:  FF 53 28                 CALL qword ptr [RBX + 0x28]
14037fcce:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
14037fcd1:  0F 11 45 E0              MOVUPS xmmword ptr [RBP + -0x20],XMM0
14037fcd5:  BA F1 05 00 00           MOV EDX,0x5f1
14037fcda:  48 8D 0D AF 78 61 00     LEA RCX,[0x140997590]
14037fce1:  FF 15 A9 CB 4E 00        CALL qword ptr [0x14086c890]
14037fce7:  F3 0F 10 05 99 23 61 00  MOVSS XMM0,dword ptr [0x140992088]
14037fcef:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14037fcf4:  41 0F 28 DA              MOVAPS XMM3,XMM10
14037fcf8:  F3 0F 5C D8              SUBSS XMM3,XMM0
14037fcfc:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
14037fd01:  48 8D 05 18 01 54 00     LEA RAX,[0x1408bfe20]
14037fd08:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14037fd0d:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14037fd15:  66 41 0F 7E D9           MOVD R9D,XMM3
14037fd1a:  F3 0F 10 7C 24 74        MOVSS XMM7,dword ptr [RSP + 0x74]
14037fd20:  0F 28 D7                 MOVAPS XMM2,XMM7
14037fd23:  66 41 0F 7E D0           MOVD R8D,XMM2
14037fd28:  48 8B 15 D9 44 61 00     MOV RDX,qword ptr [0x140994208]
14037fd2f:  48 8D 0D 7A 72 61 00     LEA RCX,[0x140996fb0]
14037fd36:  FF 15 44 CB 4E 00        CALL qword ptr [0x14086c880]
14037fd3c:  F3 0F 10 0D 44 23 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037fd44:  0F 28 C1                 MOVAPS XMM0,XMM1
14037fd47:  F3 0F 59 05 91 AA 58 00  MULSS XMM0,dword ptr [0x14090a7e0]
14037fd4f:  45 0F 28 DA              MOVAPS XMM11,XMM10
14037fd53:  F3 44 0F 5C D8           SUBSS XMM11,XMM0
14037fd58:  44 0F 28 F9              MOVAPS XMM15,XMM1
14037fd5c:  F3 44 0F 59 3D 67 AB 58 00 MULSS XMM15,dword ptr [0x14090a8cc]
14037fd65:  F3 45 0F 58 FA           ADDSS XMM15,XMM10
14037fd6a:  0F 28 D1                 MOVAPS XMM2,XMM1
14037fd6d:  F3 0F 59 15 B3 AA 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037fd75:  F3 41 0F 58 D3           ADDSS XMM2,XMM11
14037fd7a:  F3 0F 59 0D A6 AB 58 00  MULSS XMM1,dword ptr [0x14090a928]
14037fd82:  F3 0F 58 CF              ADDSS XMM1,XMM7
14037fd86:  41 B9 06 00 00 00        MOV R9D,0x6
14037fd8c:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037fd93:  E8 58 64 0C 00           CALL 0x1404461f0
14037fd98:  F3 0F 10 3D E8 22 61 00  MOVSS XMM7,dword ptr [0x140992088]
14037fda0:  0F 28 CF                 MOVAPS XMM1,XMM7
14037fda3:  F3 0F 59 0D BD AB 58 00  MULSS XMM1,dword ptr [0x14090a968]
14037fdab:  F3 0F 11 4D C8           MOVSS dword ptr [RBP + -0x38],XMM1
14037fdb0:  66 0F 73 DE 08           PSRLDQ XMM6,0x8
14037fdb5:  66 0F 7E F0              MOVD EAX,XMM6
14037fdb9:  66 44 0F 6E F0           MOVD XMM14,EAX
14037fdbe:  45 0F 5B F6              CVTDQ2PS XMM14,XMM14
14037fdc2:  41 0F 28 C6              MOVAPS XMM0,XMM14
14037fdc6:  F3 41 0F 59 C4           MULSS XMM0,XMM12
14037fdcb:  F3 0F 11 45 F0           MOVSS dword ptr [RBP + -0x10],XMM0
14037fdd0:  F3 0F 10 75 BC           MOVSS XMM6,dword ptr [RBP + -0x44]
14037fdd5:  F3 0F 5C F0              SUBSS XMM6,XMM0
14037fdd9:  0F 28 C6                 MOVAPS XMM0,XMM6
14037fddc:  F3 0F 5C C1              SUBSS XMM0,XMM1
14037fde0:  F3 44 0F 11 7C 24 28     MOVSS dword ptr [RSP + 0x28],XMM15
14037fde7:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
14037fded:  41 0F 28 DB              MOVAPS XMM3,XMM11
14037fdf1:  0F 28 D1                 MOVAPS XMM2,XMM1
14037fdf4:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037fdf9:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
14037fdfe:  E8 9D B8 ED FF           CALL 0x14025b6a0
14037fe03:  84 C0                    TEST AL,AL
14037fe05:  74 6F                    JZ 0x14037fe76
14037fe07:  BA D0 07 00 00           MOV EDX,0x7d0
14037fe0c:  48 8D 0D 7D 77 61 00     LEA RCX,[0x140997590]
14037fe13:  FF 15 77 CA 4E 00        CALL qword ptr [0x14086c890]
14037fe19:  48 8B D6                 MOV RDX,RSI
14037fe1c:  48 2B D0                 SUB RDX,RAX
14037fe1f:  90                       NOP
14037fe20:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14037fe23:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
14037fe2b:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14037fe2f:  66 85 C9                 TEST CX,CX
14037fe32:  75 EC                    JNZ 0x14037fe20
14037fe34:  48 8D 0D 55 4D 6D 00     LEA RCX,[0x140a54b90]
14037fe3b:  FF 15 77 CA 4E 00        CALL qword ptr [0x14086c8b8]
14037fe41:  84 C0                    TEST AL,AL
14037fe43:  74 29                    JZ 0x14037fe6e
14037fe45:  83 BE 10 C8 00 00 00     CMP dword ptr [RSI + 0xc810],0x0
14037fe4c:  74 09                    JZ 0x14037fe57
14037fe4e:  C6 86 0A C8 00 00 01     MOV byte ptr [RSI + 0xc80a],0x1
14037fe55:  EB 10                    JMP 0x14037fe67
14037fe57:  80 BE 0A C8 00 00 00     CMP byte ptr [RSI + 0xc80a],0x0
14037fe5e:  0F 94 C0                 SETZ AL
14037fe61:  88 86 0A C8 00 00        MOV byte ptr [RSI + 0xc80a],AL
14037fe67:  44 89 B6 10 C8 00 00     MOV dword ptr [RSI + 0xc810],R14D
14037fe6e:  F3 0F 10 3D 12 22 61 00  MOVSS XMM7,dword ptr [0x140992088]
14037fe76:  0F 28 D7                 MOVAPS XMM2,XMM7
14037fe79:  F3 0F 59 15 A7 A9 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037fe81:  F3 41 0F 58 D3           ADDSS XMM2,XMM11
14037fe86:  F3 44 0F 10 25 3D AA 58 00 MOVSS XMM12,dword ptr [0x14090a8cc]
14037fe8f:  F3 41 0F 59 FC           MULSS XMM7,XMM12
14037fe94:  F3 0F 58 7D C8           ADDSS XMM7,dword ptr [RBP + -0x38]
14037fe99:  41 B9 06 00 00 00        MOV R9D,0x6
14037fe9f:  0F 28 CF                 MOVAPS XMM1,XMM7
14037fea2:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037fea9:  E8 42 63 0C 00           CALL 0x1404461f0
14037feae:  41 0F 28 C6              MOVAPS XMM0,XMM14
14037feb2:  F3 0F 58 C6              ADDSS XMM0,XMM6
14037feb6:  F3 44 0F 11 7C 24 28     MOVSS dword ptr [RSP + 0x28],XMM15
14037febd:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
14037fec3:  41 0F 28 DB              MOVAPS XMM3,XMM11
14037fec7:  0F 28 D6                 MOVAPS XMM2,XMM6
14037feca:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037fecf:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
14037fed4:  E8 C7 B7 ED FF           CALL 0x14025b6a0
14037fed9:  84 C0                    TEST AL,AL
14037fedb:  74 75                    JZ 0x14037ff52
14037fedd:  BA 87 06 00 00           MOV EDX,0x687
14037fee2:  48 8D 0D A7 76 61 00     LEA RCX,[0x140997590]
14037fee9:  FF 15 A1 C9 4E 00        CALL qword ptr [0x14086c890]
14037feef:  48 8B D6                 MOV RDX,RSI
14037fef2:  48 2B D0                 SUB RDX,RAX
14037fef5:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14037ff00:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14037ff03:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
14037ff0b:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14037ff0f:  66 85 C9                 TEST CX,CX
14037ff12:  75 EC                    JNZ 0x14037ff00
14037ff14:  48 8D 0D 75 4C 6D 00     LEA RCX,[0x140a54b90]
14037ff1b:  FF 15 97 C9 4E 00        CALL qword ptr [0x14086c8b8]
14037ff21:  84 C0                    TEST AL,AL
14037ff23:  74 2D                    JZ 0x14037ff52
14037ff25:  83 BE 10 C8 00 00 01     CMP dword ptr [RSI + 0xc810],0x1
14037ff2c:  74 09                    JZ 0x14037ff37
14037ff2e:  C6 86 0A C8 00 00 00     MOV byte ptr [RSI + 0xc80a],0x0
14037ff35:  EB 10                    JMP 0x14037ff47
14037ff37:  80 BE 0A C8 00 00 00     CMP byte ptr [RSI + 0xc80a],0x0
14037ff3e:  0F 94 C0                 SETZ AL
14037ff41:  88 86 0A C8 00 00        MOV byte ptr [RSI + 0xc80a],AL
14037ff47:  B8 01 00 00 00           MOV EAX,0x1
14037ff4c:  89 86 10 C8 00 00        MOV dword ptr [RSI + 0xc810],EAX
14037ff52:  F3 0F 10 0D 2E 21 61 00  MOVSS XMM1,dword ptr [0x140992088]
14037ff5a:  0F 28 D1                 MOVAPS XMM2,XMM1
14037ff5d:  F3 0F 59 15 C3 A8 58 00  MULSS XMM2,dword ptr [0x14090a828]
14037ff65:  F3 41 0F 58 D3           ADDSS XMM2,XMM11
14037ff6a:  F3 41 0F 59 CC           MULSS XMM1,XMM12
14037ff6f:  F3 0F 58 CE              ADDSS XMM1,XMM6
14037ff73:  41 B9 06 00 00 00        MOV R9D,0x6
14037ff79:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14037ff80:  E8 6B 62 0C 00           CALL 0x1404461f0
14037ff85:  66 0F 6E 7D E8           MOVD XMM7,dword ptr [RBP + -0x18]
14037ff8a:  0F 5B FF                 CVTDQ2PS XMM7,XMM7
14037ff8d:  44 0F 28 E7              MOVAPS XMM12,XMM7
14037ff91:  F3 44 0F 59 25 42 9E 58 00 MULSS XMM12,dword ptr [0x140909ddc]
14037ff9a:  F3 0F 10 75 B0           MOVSS XMM6,dword ptr [RBP + -0x50]
14037ff9f:  F3 41 0F 5C F4           SUBSS XMM6,XMM12
14037ffa4:  0F 28 C7                 MOVAPS XMM0,XMM7
14037ffa7:  F3 0F 58 C6              ADDSS XMM0,XMM6
14037ffab:  F3 44 0F 11 7C 24 28     MOVSS dword ptr [RSP + 0x28],XMM15
14037ffb2:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
14037ffb8:  41 0F 28 DB              MOVAPS XMM3,XMM11
14037ffbc:  0F 28 D6                 MOVAPS XMM2,XMM6
14037ffbf:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14037ffc4:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
14037ffc9:  E8 D2 B6 ED FF           CALL 0x14025b6a0
14037ffce:  84 C0                    TEST AL,AL
14037ffd0:  74 6F                    JZ 0x140380041
14037ffd2:  BA 87 06 00 00           MOV EDX,0x687
14037ffd7:  48 8D 0D B2 75 61 00     LEA RCX,[0x140997590]
14037ffde:  FF 15 AC C8 4E 00        CALL qword ptr [0x14086c890]
14037ffe4:  48 8B D6                 MOV RDX,RSI
14037ffe7:  48 2B D0                 SUB RDX,RAX
14037ffea:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
14037fff0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14037fff3:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14037fffb:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14037ffff:  66 85 C9                 TEST CX,CX
140380002:  75 EC                    JNZ 0x14037fff0
140380004:  48 8D 0D 85 4B 6D 00     LEA RCX,[0x140a54b90]
14038000b:  FF 15 A7 C8 4E 00        CALL qword ptr [0x14086c8b8]
140380011:  84 C0                    TEST AL,AL
140380013:  74 2C                    JZ 0x140380041
140380015:  83 BE 10 C8 00 00 02     CMP dword ptr [RSI + 0xc810],0x2
14038001c:  74 09                    JZ 0x140380027
14038001e:  C6 86 0A C8 00 00 00     MOV byte ptr [RSI + 0xc80a],0x0
140380025:  EB 10                    JMP 0x140380037
140380027:  80 BE 0A C8 00 00 00     CMP byte ptr [RSI + 0xc80a],0x0
14038002e:  0F 94 C0                 SETZ AL
140380031:  88 86 0A C8 00 00        MOV byte ptr [RSI + 0xc80a],AL
140380037:  C7 86 10 C8 00 00 02 00 00 00 MOV dword ptr [RSI + 0xc810],0x2
140380041:  F3 0F 10 0D 3F 20 61 00  MOVSS XMM1,dword ptr [0x140992088]
140380049:  0F 28 D1                 MOVAPS XMM2,XMM1
14038004c:  F3 0F 59 15 D4 A7 58 00  MULSS XMM2,dword ptr [0x14090a828]
140380054:  F3 41 0F 58 D3           ADDSS XMM2,XMM11
140380059:  F3 0F 59 0D 6B A8 58 00  MULSS XMM1,dword ptr [0x14090a8cc]
140380061:  F3 0F 58 CE              ADDSS XMM1,XMM6
140380065:  41 B9 06 00 00 00        MOV R9D,0x6
14038006b:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140380072:  E8 79 61 0C 00           CALL 0x1404461f0
140380077:  F3 0F 10 74 24 70        MOVSS XMM6,dword ptr [RSP + 0x70]
14038007d:  F3 0F 5C 75 F0           SUBSS XMM6,dword ptr [RBP + -0x10]
140380082:  F3 44 0F 58 F6           ADDSS XMM14,XMM6
140380087:  F3 44 0F 11 7C 24 28     MOVSS dword ptr [RSP + 0x28],XMM15
14038008e:  F3 44 0F 11 74 24 20     MOVSS dword ptr [RSP + 0x20],XMM14
140380095:  41 0F 28 DB              MOVAPS XMM3,XMM11
140380099:  0F 28 D6                 MOVAPS XMM2,XMM6
14038009c:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
1403800a1:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
1403800a6:  E8 F5 B5 ED FF           CALL 0x14025b6a0
1403800ab:  84 C0                    TEST AL,AL
1403800ad:  74 72                    JZ 0x140380121
1403800af:  BA 87 06 00 00           MOV EDX,0x687
1403800b4:  48 8D 0D D5 74 61 00     LEA RCX,[0x140997590]
1403800bb:  FF 15 CF C7 4E 00        CALL qword ptr [0x14086c890]
1403800c1:  48 8B D6                 MOV RDX,RSI
1403800c4:  48 2B D0                 SUB RDX,RAX
1403800c7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
1403800d0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
1403800d3:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
1403800db:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403800df:  66 85 C9                 TEST CX,CX
1403800e2:  75 EC                    JNZ 0x1403800d0
1403800e4:  48 8D 0D A5 4A 6D 00     LEA RCX,[0x140a54b90]
1403800eb:  FF 15 C7 C7 4E 00        CALL qword ptr [0x14086c8b8]
1403800f1:  84 C0                    TEST AL,AL
1403800f3:  74 2C                    JZ 0x140380121
1403800f5:  83 BE 10 C8 00 00 03     CMP dword ptr [RSI + 0xc810],0x3
1403800fc:  74 09                    JZ 0x140380107
1403800fe:  C6 86 0A C8 00 00 00     MOV byte ptr [RSI + 0xc80a],0x0
140380105:  EB 10                    JMP 0x140380117
140380107:  80 BE 0A C8 00 00 00     CMP byte ptr [RSI + 0xc80a],0x0
14038010e:  0F 94 C0                 SETZ AL
140380111:  88 86 0A C8 00 00        MOV byte ptr [RSI + 0xc80a],AL
140380117:  C7 86 10 C8 00 00 03 00 00 00 MOV dword ptr [RSI + 0xc810],0x3
140380121:  F3 0F 10 0D 5F 1F 61 00  MOVSS XMM1,dword ptr [0x140992088]
140380129:  0F 28 D1                 MOVAPS XMM2,XMM1
14038012c:  F3 0F 59 15 F4 A6 58 00  MULSS XMM2,dword ptr [0x14090a828]
140380134:  F3 41 0F 58 D3           ADDSS XMM2,XMM11
140380139:  F3 0F 59 0D 8B A7 58 00  MULSS XMM1,dword ptr [0x14090a8cc]
140380141:  F3 0F 58 CE              ADDSS XMM1,XMM6
140380145:  41 B9 06 00 00 00        MOV R9D,0x6
14038014b:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140380152:  E8 99 60 0C 00           CALL 0x1404461f0
140380157:  F3 0F 10 55 F8           MOVSS XMM2,dword ptr [RBP + -0x8]
14038015c:  F3 41 0F 5C D4           SUBSS XMM2,XMM12
140380161:  F3 0F 58 FA              ADDSS XMM7,XMM2
140380165:  F3 44 0F 11 7C 24 28     MOVSS dword ptr [RSP + 0x28],XMM15
14038016c:  F3 0F 11 7C 24 20        MOVSS dword ptr [RSP + 0x20],XMM7
140380172:  41 0F 28 DB              MOVAPS XMM3,XMM11
140380176:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14038017b:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140380180:  E8 1B B5 ED FF           CALL 0x14025b6a0
140380185:  84 C0                    TEST AL,AL
140380187:  74 69                    JZ 0x1403801f2
140380189:  BA 87 06 00 00           MOV EDX,0x687
14038018e:  48 8D 0D FB 73 61 00     LEA RCX,[0x140997590]
140380195:  FF 15 F5 C6 4E 00        CALL qword ptr [0x14086c890]
14038019b:  48 8B D6                 MOV RDX,RSI
14038019e:  48 2B D0                 SUB RDX,RAX
1403801a1:  0F B7 08                 MOVZX ECX,word ptr [RAX]
1403801a4:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
1403801ac:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403801b0:  66 85 C9                 TEST CX,CX
1403801b3:  75 EC                    JNZ 0x1403801a1
1403801b5:  48 8D 0D D4 49 6D 00     LEA RCX,[0x140a54b90]
1403801bc:  FF 15 F6 C6 4E 00        CALL qword ptr [0x14086c8b8]
1403801c2:  84 C0                    TEST AL,AL
1403801c4:  74 2C                    JZ 0x1403801f2
1403801c6:  83 BE 10 C8 00 00 04     CMP dword ptr [RSI + 0xc810],0x4
1403801cd:  74 09                    JZ 0x1403801d8
1403801cf:  C6 86 0A C8 00 00 00     MOV byte ptr [RSI + 0xc80a],0x0
1403801d6:  EB 10                    JMP 0x1403801e8
1403801d8:  80 BE 0A C8 00 00 00     CMP byte ptr [RSI + 0xc80a],0x0
1403801df:  0F 94 C0                 SETZ AL
1403801e2:  88 86 0A C8 00 00        MOV byte ptr [RSI + 0xc80a],AL
1403801e8:  C7 86 10 C8 00 00 04 00 00 00 MOV dword ptr [RSI + 0xc810],0x4
1403801f2:  F3 0F 10 74 24 78        MOVSS XMM6,dword ptr [RSP + 0x78]
1403801f8:  F3 0F 59 35 AC 9C 58 00  MULSS XMM6,dword ptr [0x140909eac]
140380200:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
140380205:  8B 05 65 50 61 00        MOV EAX,dword ptr [0x140995270]
14038020b:  83 F8 02                 CMP EAX,0x2
14038020e:  75 09                    JNZ 0x140380219
140380210:  48 8D 05 69 FB 53 00     LEA RAX,[0x1408bfd80]
140380217:  EB 13                    JMP 0x14038022c
140380219:  83 F8 01                 CMP EAX,0x1
14038021c:  48 8D 05 5D FA 53 00     LEA RAX,[0x1408bfc80]
140380223:  74 07                    JZ 0x14038022c
140380225:  48 8D 05 64 FD 53 00     LEA RAX,[0x1408bff90]
14038022c:  F3 0F 10 15 54 1E 61 00  MOVSS XMM2,dword ptr [0x140992088]
140380234:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140380239:  0F 28 DA                 MOVAPS XMM3,XMM2
14038023c:  F3 0F 10 7C 24 74        MOVSS XMM7,dword ptr [RSP + 0x74]
140380242:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140380247:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
14038024c:  F3 0F 58 D7              ADDSS XMM2,XMM7
140380250:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140380258:  66 41 0F 7E D9           MOVD R9D,XMM3
14038025d:  66 41 0F 7E D0           MOVD R8D,XMM2
140380262:  49 8B D7                 MOV RDX,R15
140380265:  48 8D 0D 44 6D 61 00     LEA RCX,[0x140996fb0]
14038026c:  FF 15 0E C6 4E 00        CALL qword ptr [0x14086c880]
140380272:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
140380277:  C6 45 B8 00              MOV byte ptr [RBP + -0x48],0x0
14038027b:  83 C8 FF                 OR EAX,0xffffffff
14038027e:  89 45 C0                 MOV dword ptr [RBP + -0x40],EAX
140380281:  66 44 0F 6E 74 24 60     MOVD XMM14,dword ptr [RSP + 0x60]
140380288:  45 0F 5B F6              CVTDQ2PS XMM14,XMM14
14038028c:  F3 0F 10 1D F4 1D 61 00  MOVSS XMM3,dword ptr [0x140992088]
140380294:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140380299:  41 0F 2F D6              COMISS XMM2,XMM14
14038029d:  76 56                    JBE 0x1403802f5
14038029f:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
1403802a5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403802a8:  0F 2F C2                 COMISS XMM0,XMM2
1403802ab:  76 48                    JBE 0x1403802f5
1403802ad:  0F 28 CB                 MOVAPS XMM1,XMM3
1403802b0:  F3 0F 59 0D 70 A7 58 00  MULSS XMM1,dword ptr [0x14090aa28]
1403802b8:  F3 0F 10 45 BC           MOVSS XMM0,dword ptr [RBP + -0x44]
1403802bd:  F3 0F 5C C1              SUBSS XMM0,XMM1
1403802c1:  F3 44 0F 10 3D 12 9B 58 00 MOVSS XMM15,dword ptr [0x140909ddc]
1403802ca:  0F 2F D0                 COMISS XMM2,XMM0
1403802cd:  76 1C                    JBE 0x1403802eb
1403802cf:  F3 44 0F 58 6D C4        ADDSS XMM13,dword ptr [RBP + -0x3c]
1403802d5:  F3 45 0F 59 EF           MULSS XMM13,XMM15
1403802da:  45 8B C6                 MOV R8D,R14D
1403802dd:  44 0F 2F EA              COMISS XMM13,XMM2
1403802e1:  41 0F 96 C0              SETBE R8B
1403802e5:  44 89 45 C0              MOV dword ptr [RBP + -0x40],R8D
1403802e9:  EB 13                    JMP 0x1403802fe
1403802eb:  B8 02 00 00 00           MOV EAX,0x2
1403802f0:  89 45 C0                 MOV dword ptr [RBP + -0x40],EAX
1403802f3:  EB 09                    JMP 0x1403802fe
1403802f5:  F3 44 0F 10 3D DE 9A 58 00 MOVSS XMM15,dword ptr [0x140909ddc]
1403802fe:  F3 0F 10 74 24 78        MOVSS XMM6,dword ptr [RSP + 0x78]
140380304:  44 0F 28 E6              MOVAPS XMM12,XMM6
140380308:  F3 44 0F 59 25 C3 9B 58 00 MULSS XMM12,dword ptr [0x140909ed4]
140380311:  66 0F 6E 05 5B 4F 61 00  MOVD XMM0,dword ptr [0x140995274]
140380319:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14038031c:  F3 41 0F 5C C2           SUBSS XMM0,XMM10
140380321:  F3 0F 59 1D 0F AC 58 00  MULSS XMM3,dword ptr [0x14090af38]
140380329:  F3 0F 5C C3              SUBSS XMM0,XMM3
14038032d:  F3 41 0F 5E C4           DIVSS XMM0,XMM12
140380332:  F3 44 0F 2C E8           CVTTSS2SI R13D,XMM0
140380337:  B8 14 00 00 00           MOV EAX,0x14
14038033c:  44 3B E8                 CMP R13D,EAX
14038033f:  44 0F 4C E8              CMOVL R13D,EAX
140380343:  44 89 6D C4              MOV dword ptr [RBP + -0x3c],R13D
140380347:  8B 86 E0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7e0]
14038034d:  45 8B E5                 MOV R12D,R13D
140380350:  44 0F AF E0              IMUL R12D,EAX
140380354:  44 8D 78 01              LEA R15D,[RAX + 0x1]
140380358:  45 0F AF FD              IMUL R15D,R13D
14038035c:  41 8B DE                 MOV EBX,R14D
14038035f:  48 8D 8E B0 C2 00 00     LEA RCX,[RSI + 0xc2b0]
140380366:  E8 65 50 D6 FF           CALL 0x1400e53d0
14038036b:  48 85 C0                 TEST RAX,RAX
14038036e:  0F 84 A2 00 00 00        JZ 0x140380416
140380374:  4D 8B C6                 MOV R8,R14
140380377:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140380380:  49 69 D0 40 03 00 00     IMUL RDX,R8,0x340
140380387:  48 03 96 B0 C2 00 00     ADD RDX,qword ptr [RSI + 0xc2b0]
14038038e:  48 89 55 C8              MOV qword ptr [RBP + -0x38],RDX
140380392:  8B 42 44                 MOV EAX,dword ptr [RDX + 0x44]
140380395:  83 F8 FF                 CMP EAX,-0x1
140380398:  74 62                    JZ 0x1403803fc
14038039a:  83 F8 FE                 CMP EAX,-0x2
14038039d:  75 09                    JNZ 0x1403803a8
14038039f:  83 BE B0 05 00 00 00     CMP dword ptr [RSI + 0x5b0],0x0
1403803a6:  74 54                    JZ 0x1403803fc
1403803a8:  83 C0 05                 ADD EAX,0x5
1403803ab:  83 F8 02                 CMP EAX,0x2
1403803ae:  76 4C                    JBE 0x1403803fc
1403803b0:  48 3B 96 90 C3 00 00     CMP RDX,qword ptr [RSI + 0xc390]
1403803b7:  74 09                    JZ 0x1403803c2
1403803b9:  48 3B 96 88 C3 00 00     CMP RDX,qword ptr [RSI + 0xc388]
1403803c0:  75 09                    JNZ 0x1403803cb
1403803c2:  83 BE B4 05 00 00 00     CMP dword ptr [RSI + 0x5b4],0x0
1403803c9:  74 31                    JZ 0x1403803fc
1403803cb:  48 8B CE                 MOV RCX,RSI
1403803ce:  E8 7D 83 0A 00           CALL 0x140428750
1403803d3:  84 C0                    TEST AL,AL
1403803d5:  75 18                    JNZ 0x1403803ef
1403803d7:  45 33 C9                 XOR R9D,R9D
1403803da:  45 33 C0                 XOR R8D,R8D
1403803dd:  E8 9E 83 0A 00           CALL 0x140428780
1403803e2:  84 C0                    TEST AL,AL
1403803e4:  74 09                    JZ 0x1403803ef
1403803e6:  83 BE B8 05 00 00 00     CMP dword ptr [RSI + 0x5b8],0x0
1403803ed:  74 0D                    JZ 0x1403803fc
1403803ef:  48 8D 55 C8              LEA RDX,[RBP + -0x38]
1403803f3:  48 8D 4D 38              LEA RCX,[RBP + 0x38]
1403803f7:  E8 94 0A D3 FF           CALL 0x1400b0e90
1403803fc:  FF C3                    INC EBX
1403803fe:  4C 63 C3                 MOVSXD R8,EBX
140380401:  48 8D 8E B0 C2 00 00     LEA RCX,[RSI + 0xc2b0]
140380408:  E8 C3 4F D6 FF           CALL 0x1400e53d0
14038040d:  4C 3B C0                 CMP R8,RAX
140380410:  0F 82 6A FF FF FF        JC 0x140380380
140380416:  48 8B 4D 40              MOV RCX,qword ptr [RBP + 0x40]
14038041a:  48 2B 4D 38              SUB RCX,qword ptr [RBP + 0x38]
14038041e:  48 C1 F9 03              SAR RCX,0x3
140380422:  49 63 C7                 MOVSXD RAX,R15D
140380425:  48 3B C1                 CMP RAX,RCX
140380428:  76 03                    JBE 0x14038042d
14038042a:  4C 8B F9                 MOV R15,RCX
14038042d:  44 0F B6 8E 0A C8 00 00  MOVZX R9D,byte ptr [RSI + 0xc80a]
140380435:  44 8B 86 10 C8 00 00     MOV R8D,dword ptr [RSI + 0xc810]
14038043c:  48 8D 55 38              LEA RDX,[RBP + 0x38]
140380440:  48 8B CE                 MOV RCX,RSI
140380443:  E8 08 C6 F2 FF           CALL 0x1402aca50
140380448:  4D 63 E4                 MOVSXD R12,R12D
14038044b:  49 63 C7                 MOVSXD RAX,R15D
14038044e:  48 89 45 C8              MOV qword ptr [RBP + -0x38],RAX
140380452:  4C 3B E0                 CMP R12,RAX
140380455:  0F 8D F0 05 00 00        JGE 0x140380a4b
14038045b:  41 2B C4                 SUB EAX,R12D
14038045e:  44 8B F0                 MOV R14D,EAX
140380461:  4C 89 75 F0              MOV qword ptr [RBP + -0x10],R14
140380465:  F3 0F 10 3D 0B 9A 58 00  MOVSS XMM7,dword ptr [0x140909e78]
14038046d:  F3 44 0F 10 1D 8E 98 58 00 MOVSS XMM11,dword ptr [0x140909d04]
140380476:  F3 44 0F 10 2D C5 9F 58 00 MOVSS XMM13,dword ptr [0x14090a444]
14038047f:  4C 8B 6D C8              MOV R13,qword ptr [RBP + -0x38]
140380483:  0F 1F 40 00              NOP dword ptr [RAX]
140380487:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140380490:  48 8B 45 38              MOV RAX,qword ptr [RBP + 0x38]
140380494:  4E 8B 3C E0              MOV R15,qword ptr [RAX + R12*0x8]
140380498:  F3 0F 10 15 E8 1B 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403804a0:  F3 41 0F 59 D0           MULSS XMM2,XMM8
1403804a5:  41 0F 28 C2              MOVAPS XMM0,XMM10
1403804a9:  F3 0F 5C C2              SUBSS XMM0,XMM2
1403804ad:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
1403804b1:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
1403804b5:  0F 28 CE                 MOVAPS XMM1,XMM6
1403804b8:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
1403804bd:  F3 0F 58 CA              ADDSS XMM1,XMM2
1403804c1:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
1403804c5:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
1403804c9:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
1403804ce:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
1403804d3:  41 B8 06 00 00 00        MOV R8D,0x6
1403804d9:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
1403804dd:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
1403804e4:  E8 57 60 0C 00           CALL 0x140446540
1403804e9:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
1403804ee:  0F 84 B7 01 00 00        JZ 0x1403806ab
1403804f4:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
1403804f9:  41 0F 2F CE              COMISS XMM1,XMM14
1403804fd:  0F 86 A8 01 00 00        JBE 0x1403806ab
140380503:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
140380509:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14038050c:  0F 2F C1                 COMISS XMM0,XMM1
14038050f:  0F 86 96 01 00 00        JBE 0x1403806ab
140380515:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14038051a:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14038051f:  F3 0F 10 15 61 1B 61 00  MOVSS XMM2,dword ptr [0x140992088]
140380527:  F3 41 0F 59 D5           MULSS XMM2,XMM13
14038052c:  66 0F 6E C3              MOVD XMM0,EBX
140380530:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140380533:  F3 0F 58 C2              ADDSS XMM0,XMM2
140380537:  0F 2F C8                 COMISS XMM1,XMM0
14038053a:  0F 86 6B 01 00 00        JBE 0x1403806ab
140380540:  66 0F 6E C7              MOVD XMM0,EDI
140380544:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140380547:  F3 0F 5C C2              SUBSS XMM0,XMM2
14038054b:  0F 2F C1                 COMISS XMM0,XMM1
14038054e:  0F 86 57 01 00 00        JBE 0x1403806ab
140380554:  C6 45 B8 01              MOV byte ptr [RBP + -0x48],0x1
140380558:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14038055f:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140380562:  48 8D 0D 67 A7 66 00     LEA RCX,[0x1409eacd0]
140380569:  FF 15 81 C1 4E 00        CALL qword ptr [0x14086c6f0]
14038056f:  4C 8B C0                 MOV R8,RAX
140380572:  33 D2                    XOR EDX,EDX
140380574:  48 8B CB                 MOV RCX,RBX
140380577:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14038057a:  F3 44 0F 11 7C 24 20     MOVSS dword ptr [RSP + 0x20],XMM15
140380581:  41 0F 28 D9              MOVAPS XMM3,XMM9
140380585:  41 0F 28 D1              MOVAPS XMM2,XMM9
140380589:  41 0F 28 C9              MOVAPS XMM1,XMM9
14038058d:  48 8D 8D 20 0A 00 00     LEA RCX,[RBP + 0xa20]
140380594:  FF 15 86 BA 4E 00        CALL qword ptr [0x14086c020]
14038059a:  48 8B D0                 MOV RDX,RAX
14038059d:  48 8D 0D BC DA 63 00     LEA RCX,[0x1409be060]
1403805a4:  FF 15 EE C2 4E 00        CALL qword ptr [0x14086c898]
1403805aa:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403805ae:  48 8D 0D AB DA 63 00     LEA RCX,[0x1409be060]
1403805b5:  FF 15 9D C3 4E 00        CALL qword ptr [0x14086c958]
1403805bb:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
1403805c0:  F3 0F 10 15 C0 1A 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403805c8:  F3 0F 59 15 58 A2 58 00  MULSS XMM2,dword ptr [0x14090a828]
1403805d0:  F3 0F 11 7C 24 38        MOVSS dword ptr [RSP + 0x38],XMM7
1403805d6:  F3 44 0F 11 5C 24 30     MOVSS dword ptr [RSP + 0x30],XMM11
1403805dd:  F3 0F 11 7C 24 28        MOVSS dword ptr [RSP + 0x28],XMM7
1403805e3:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
1403805ea:  0F 28 DA                 MOVAPS XMM3,XMM2
1403805ed:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
1403805f1:  48 8D 0D 68 DA 63 00     LEA RCX,[0x1409be060]
1403805f8:  FF 15 5A C2 4E 00        CALL qword ptr [0x14086c858]
1403805fe:  41 0F 28 C8              MOVAPS XMM1,XMM8
140380602:  48 8D 0D 57 DA 63 00     LEA RCX,[0x1409be060]
140380609:  FF 15 49 C3 4E 00        CALL qword ptr [0x14086c958]
14038060f:  4C 89 BE E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],R15
140380616:  80 BE 9A 03 01 00 00     CMP byte ptr [RSI + 0x1039a],0x0
14038061d:  0F 84 88 00 00 00        JZ 0x1403806ab
140380623:  48 8D 0D 66 45 6D 00     LEA RCX,[0x140a54b90]
14038062a:  FF 15 88 C2 4E 00        CALL qword ptr [0x14086c8b8]
140380630:  84 C0                    TEST AL,AL
140380632:  74 77                    JZ 0x1403806ab
140380634:  48 8B 86 70 08 01 00     MOV RAX,qword ptr [RSI + 0x10870]
14038063b:  48 3B 86 E8 C7 00 00     CMP RAX,qword ptr [RSI + 0xc7e8]
140380642:  75 06                    JNZ 0x14038064a
140380644:  C6 45 A8 01              MOV byte ptr [RBP + -0x58],0x1
140380648:  EB 61                    JMP 0x1403806ab
14038064a:  C6 44 24 7D 01           MOV byte ptr [RSP + 0x7d],0x1
14038064f:  0F B6 7C 24 7C           MOVZX EDI,byte ptr [RSP + 0x7c]
140380654:  48 8D 9E D8 06 01 00     LEA RBX,[RSI + 0x106d8]
14038065b:  40 84 FF                 TEST DIL,DIL
14038065e:  74 25                    JZ 0x140380685
140380660:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
140380667:  48 8B CB                 MOV RCX,RBX
14038066a:  E8 21 B4 ED FF           CALL 0x14025ba90
14038066f:  4C 8B 86 E8 C7 00 00     MOV R8,qword ptr [RSI + 0xc7e8]
140380676:  8B 55 18                 MOV EDX,dword ptr [RBP + 0x18]
140380679:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
140380680:  E8 DB DA 25 00           CALL 0x1405de160
140380685:  4C 8B 86 E8 C7 00 00     MOV R8,qword ptr [RSI + 0xc7e8]
14038068c:  8B 55 18                 MOV EDX,dword ptr [RBP + 0x18]
14038068f:  48 8B CB                 MOV RCX,RBX
140380692:  E8 C9 DA 25 00           CALL 0x1405de160
140380697:  40 84 FF                 TEST DIL,DIL
14038069a:  74 0F                    JZ 0x1403806ab
14038069c:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
1403806a3:  48 8B CB                 MOV RCX,RBX
1403806a6:  E8 C5 B3 ED FF           CALL 0x14025ba70
1403806ab:  41 8B 57 40              MOV EDX,dword ptr [R15 + 0x40]
1403806af:  48 8D 0D DA 6E 61 00     LEA RCX,[0x140997590]
1403806b6:  FF 15 D4 C1 4E 00        CALL qword ptr [0x14086c890]
1403806bc:  F3 0F 10 15 C4 19 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403806c4:  0F 28 C2                 MOVAPS XMM0,XMM2
1403806c7:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403806cc:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403806d0:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403806d4:  F3 0F 59 15 8C A2 58 00  MULSS XMM2,dword ptr [0x14090a968]
1403806dc:  F3 0F 10 74 24 74        MOVSS XMM6,dword ptr [RSP + 0x74]
1403806e2:  F3 0F 58 D6              ADDSS XMM2,XMM6
1403806e6:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403806eb:  48 8D 05 76 F8 53 00     LEA RAX,[0x1408bff68]
1403806f2:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403806f7:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403806ff:  66 41 0F 7E D9           MOVD R9D,XMM3
140380704:  66 41 0F 7E D0           MOVD R8D,XMM2
140380709:  48 8B 15 F8 3A 61 00     MOV RDX,qword ptr [0x140994208]
140380710:  48 8D 0D 99 68 61 00     LEA RCX,[0x140996fb0]
140380717:  FF 15 63 C1 4E 00        CALL qword ptr [0x14086c880]
14038071d:  49 8B 5F 48              MOV RBX,qword ptr [R15 + 0x48]
140380721:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140380724:  48 8D 0D A5 A5 66 00     LEA RCX,[0x1409eacd0]
14038072b:  FF 15 BF BF 4E 00        CALL qword ptr [0x14086c6f0]
140380731:  4C 8B C0                 MOV R8,RAX
140380734:  33 D2                    XOR EDX,EDX
140380736:  48 8B CB                 MOV RCX,RBX
140380739:  FF 57 70                 CALL qword ptr [RDI + 0x70]
14038073c:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140380743:  41 0F 28 D9              MOVAPS XMM3,XMM9
140380747:  41 0F 28 D1              MOVAPS XMM2,XMM9
14038074b:  41 0F 28 C9              MOVAPS XMM1,XMM9
14038074f:  48 8D 8D 30 0A 00 00     LEA RCX,[RBP + 0xa30]
140380756:  FF 15 C4 B8 4E 00        CALL qword ptr [0x14086c020]
14038075c:  48 8B D0                 MOV RDX,RAX
14038075f:  48 8D 0D FA D8 63 00     LEA RCX,[0x1409be060]
140380766:  FF 15 2C C1 4E 00        CALL qword ptr [0x14086c898]
14038076c:  F3 0F 10 0D 14 19 61 00  MOVSS XMM1,dword ptr [0x140992088]
140380774:  F3 0F 59 0D EC A1 58 00  MULSS XMM1,dword ptr [0x14090a968]
14038077c:  0F 28 D1                 MOVAPS XMM2,XMM1
14038077f:  48 8D 0D DA D8 63 00     LEA RCX,[0x1409be060]
140380786:  FF 15 1C C1 4E 00        CALL qword ptr [0x14086c8a8]
14038078c:  F3 0F 10 15 F4 18 61 00  MOVSS XMM2,dword ptr [0x140992088]
140380794:  F3 0F 59 15 8C A0 58 00  MULSS XMM2,dword ptr [0x14090a828]
14038079c:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
1403807a1:  41 0F 28 D8              MOVAPS XMM3,XMM8
1403807a5:  0F 28 CE                 MOVAPS XMM1,XMM6
1403807a8:  48 8D 8D D4 04 00 00     LEA RCX,[RBP + 0x4d4]
1403807af:  FF 15 53 B8 4E 00        CALL qword ptr [0x14086c008]
1403807b5:  48 8B D0                 MOV RDX,RAX
1403807b8:  48 8D 0D A1 D8 63 00     LEA RCX,[0x1409be060]
1403807bf:  FF 15 DB C0 4E 00        CALL qword ptr [0x14086c8a0]
1403807c5:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
1403807ca:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
1403807d1:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403807d8:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403807dc:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403807e0:  41 0F 28 C8              MOVAPS XMM1,XMM8
1403807e4:  48 8D 0D 75 D8 63 00     LEA RCX,[0x1409be060]
1403807eb:  FF 15 6F C0 4E 00        CALL qword ptr [0x14086c860]
1403807f1:  45 33 C9                 XOR R9D,R9D
1403807f4:  4C 8D 86 08 15 01 00     LEA R8,[RSI + 0x11508]
1403807fb:  49 8B D7                 MOV RDX,R15
1403807fe:  E8 DD D9 F7 FF           CALL 0x1402fe1e0
140380803:  0F 57 C9                 XORPS XMM1,XMM1
140380806:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14038080a:  41 B0 01                 MOV R8B,0x1
14038080d:  48 8B CE                 MOV RCX,RSI
140380810:  E8 0B 5C FB FF           CALL 0x140336420
140380815:  F3 0F 10 15 6B 18 61 00  MOVSS XMM2,dword ptr [0x140992088]
14038081d:  0F 28 C2                 MOVAPS XMM0,XMM2
140380820:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140380825:  41 0F 28 DA              MOVAPS XMM3,XMM10
140380829:  F3 0F 5C D8              SUBSS XMM3,XMM0
14038082d:  F3 0F 10 35 33 A1 58 00  MOVSS XMM6,dword ptr [0x14090a968]
140380835:  F3 0F 59 D6              MULSS XMM2,XMM6
140380839:  F3 0F 58 55 BC           ADDSS XMM2,dword ptr [RBP + -0x44]
14038083e:  66 0F 6E 05 46 4A 61 00  MOVD XMM0,dword ptr [0x14099528c]
140380846:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140380849:  F3 0F 5C D0              SUBSS XMM2,XMM0
14038084d:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140380852:  48 8D 05 07 F7 53 00     LEA RAX,[0x1408bff60]
140380859:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14038085e:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140380866:  66 41 0F 7E D9           MOVD R9D,XMM3
14038086b:  66 41 0F 7E D0           MOVD R8D,XMM2
140380870:  48 8B 15 91 39 61 00     MOV RDX,qword ptr [0x140994208]
140380877:  48 8D 0D 32 67 61 00     LEA RCX,[0x140996fb0]
14038087e:  FF 15 F4 BF 4E 00        CALL qword ptr [0x14086c878]
140380884:  45 33 C9                 XOR R9D,R9D
140380887:  4C 8D 86 D8 14 01 00     LEA R8,[RSI + 0x114d8]
14038088e:  49 8B D7                 MOV RDX,R15
140380891:  E8 4A D9 F7 FF           CALL 0x1402fe1e0
140380896:  0F 57 C9                 XORPS XMM1,XMM1
140380899:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
14038089d:  41 B0 01                 MOV R8B,0x1
1403808a0:  48 8B CE                 MOV RCX,RSI
1403808a3:  E8 78 5B FB FF           CALL 0x140336420
1403808a8:  F3 0F 10 15 D8 17 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403808b0:  0F 28 C2                 MOVAPS XMM0,XMM2
1403808b3:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403808b8:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403808bc:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403808c0:  F3 0F 59 D6              MULSS XMM2,XMM6
1403808c4:  F3 0F 58 55 B0           ADDSS XMM2,dword ptr [RBP + -0x50]
1403808c9:  66 0F 6E 05 BB 49 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403808d1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403808d4:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403808d8:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403808dd:  48 8D 05 74 F6 53 00     LEA RAX,[0x1408bff58]
1403808e4:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403808e9:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403808f1:  66 41 0F 7E D9           MOVD R9D,XMM3
1403808f6:  66 41 0F 7E D0           MOVD R8D,XMM2
1403808fb:  48 8B 15 06 39 61 00     MOV RDX,qword ptr [0x140994208]
140380902:  48 8D 0D A7 66 61 00     LEA RCX,[0x140996fb0]
140380909:  FF 15 69 BF 4E 00        CALL qword ptr [0x14086c878]
14038090f:  45 33 C9                 XOR R9D,R9D
140380912:  4C 8D 86 F0 14 01 00     LEA R8,[RSI + 0x114f0]
140380919:  49 8B D7                 MOV RDX,R15
14038091c:  E8 BF D8 F7 FF           CALL 0x1402fe1e0
140380921:  0F 57 C9                 XORPS XMM1,XMM1
140380924:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
140380928:  41 B0 01                 MOV R8B,0x1
14038092b:  48 8B CE                 MOV RCX,RSI
14038092e:  E8 ED 5A FB FF           CALL 0x140336420
140380933:  F3 0F 10 15 4D 17 61 00  MOVSS XMM2,dword ptr [0x140992088]
14038093b:  0F 28 C2                 MOVAPS XMM0,XMM2
14038093e:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140380943:  41 0F 28 DA              MOVAPS XMM3,XMM10
140380947:  F3 0F 5C D8              SUBSS XMM3,XMM0
14038094b:  F3 0F 59 D6              MULSS XMM2,XMM6
14038094f:  F3 0F 58 54 24 70        ADDSS XMM2,dword ptr [RSP + 0x70]
140380955:  66 0F 6E 05 2F 49 61 00  MOVD XMM0,dword ptr [0x14099528c]
14038095d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140380960:  F3 0F 5C D0              SUBSS XMM2,XMM0
140380964:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140380969:  48 8D 05 08 F7 53 00     LEA RAX,[0x1408c0078]
140380970:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140380975:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14038097d:  66 41 0F 7E D9           MOVD R9D,XMM3
140380982:  66 41 0F 7E D0           MOVD R8D,XMM2
140380987:  48 8B 15 7A 38 61 00     MOV RDX,qword ptr [0x140994208]
14038098e:  48 8D 0D 1B 66 61 00     LEA RCX,[0x140996fb0]
140380995:  FF 15 DD BE 4E 00        CALL qword ptr [0x14086c878]
14038099b:  45 33 C9                 XOR R9D,R9D
14038099e:  4C 8D 86 C0 14 01 00     LEA R8,[RSI + 0x114c0]
1403809a5:  49 8B D7                 MOV RDX,R15
1403809a8:  E8 33 D8 F7 FF           CALL 0x1402fe1e0
1403809ad:  0F 57 C9                 XORPS XMM1,XMM1
1403809b0:  F3 0F 5A C8              CVTSS2SD XMM1,XMM0
1403809b4:  41 B0 01                 MOV R8B,0x1
1403809b7:  48 8B CE                 MOV RCX,RSI
1403809ba:  E8 61 5A FB FF           CALL 0x140336420
1403809bf:  F3 0F 10 15 C1 16 61 00  MOVSS XMM2,dword ptr [0x140992088]
1403809c7:  0F 28 C2                 MOVAPS XMM0,XMM2
1403809ca:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403809cf:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403809d3:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403809d7:  F3 0F 59 D6              MULSS XMM2,XMM6
1403809db:  F3 0F 58 55 F8           ADDSS XMM2,dword ptr [RBP + -0x8]
1403809e0:  66 0F 6E 05 A4 48 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403809e8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403809eb:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403809ef:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403809f4:  48 8D 05 75 F6 53 00     LEA RAX,[0x1408c0070]
1403809fb:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140380a00:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140380a08:  66 41 0F 7E D9           MOVD R9D,XMM3
140380a0d:  66 41 0F 7E D0           MOVD R8D,XMM2
140380a12:  48 8B 15 EF 37 61 00     MOV RDX,qword ptr [0x140994208]
140380a19:  48 8D 0D 90 65 61 00     LEA RCX,[0x140996fb0]
140380a20:  FF 15 52 BE 4E 00        CALL qword ptr [0x14086c878]
140380a26:  F3 45 0F 58 D4           ADDSS XMM10,XMM12
140380a2b:  49 FF C4                 INC R12
140380a2e:  4D 3B E5                 CMP R12,R13
140380a31:  F3 0F 10 74 24 78        MOVSS XMM6,dword ptr [RSP + 0x78]
140380a37:  0F 8C 53 FA FF FF        JL 0x140380490
140380a3d:  F3 0F 10 7C 24 74        MOVSS XMM7,dword ptr [RSP + 0x74]
140380a43:  4C 8B 75 F0              MOV R14,qword ptr [RBP + -0x10]
140380a47:  44 8B 6D C4              MOV R13D,dword ptr [RBP + -0x3c]
140380a4b:  41 8B C5                 MOV EAX,R13D
140380a4e:  41 2B C6                 SUB EAX,R14D
140380a51:  85 C0                    TEST EAX,EAX
140380a53:  7E 11                    JLE 0x140380a66
140380a55:  66 0F 6E C0              MOVD XMM0,EAX
140380a59:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140380a5c:  F3 41 0F 59 C4           MULSS XMM0,XMM12
140380a61:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140380a66:  F3 0F 10 0D 1A 16 61 00  MOVSS XMM1,dword ptr [0x140992088]
140380a6e:  0F 28 C1                 MOVAPS XMM0,XMM1
140380a71:  F3 0F 59 05 AF 9D 58 00  MULSS XMM0,dword ptr [0x14090a828]
140380a79:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140380a7e:  48 8B 45 40              MOV RAX,qword ptr [RBP + 0x40]
140380a82:  48 2B 45 38              SUB RAX,qword ptr [RBP + 0x38]
140380a86:  48 C1 F8 03              SAR RAX,0x3
140380a8a:  49 63 CD                 MOVSXD RCX,R13D
140380a8d:  33 D2                    XOR EDX,EDX
140380a8f:  48 F7 F1                 DIV RCX
140380a92:  0F 28 D9                 MOVAPS XMM3,XMM1
140380a95:  F3 0F 59 1D 53 9F 58 00  MULSS XMM3,dword ptr [0x14090a9f0]
140380a9d:  F3 0F 59 8E 54 AE 00 00  MULSS XMM1,dword ptr [RSI + 0xae54]
140380aa5:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140380aaa:  48 8D 8E E0 C7 00 00     LEA RCX,[RSI + 0xc7e0]
140380ab1:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
140380ab6:  89 44 24 20              MOV dword ptr [RSP + 0x20],EAX
140380aba:  41 0F 28 D2              MOVAPS XMM2,XMM10
140380abe:  48 8B CE                 MOV RCX,RSI
140380ac1:  E8 1A B6 13 00           CALL 0x1404bc0e0
140380ac6:  0F 28 CE                 MOVAPS XMM1,XMM6
140380ac9:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140380ace:  F3 0F 10 35 B2 15 61 00  MOVSS XMM6,dword ptr [0x140992088]
140380ad6:  0F 28 C6                 MOVAPS XMM0,XMM6
140380ad9:  F3 44 0F 10 0D 46 9F 58 00 MOVSS XMM9,dword ptr [0x14090aa28]
140380ae2:  F3 41 0F 59 C1           MULSS XMM0,XMM9
140380ae7:  F3 0F 58 C8              ADDSS XMM1,XMM0
140380aeb:  F3 44 0F 58 D1           ADDSS XMM10,XMM1
140380af0:  F3 41 0F 59 F0           MULSS XMM6,XMM8
140380af5:  BA 90 06 00 00           MOV EDX,0x690
140380afa:  48 8D 0D 8F 6A 61 00     LEA RCX,[0x140997590]
140380b01:  FF 15 89 BD 4E 00        CALL qword ptr [0x14086c890]
140380b07:  0F 28 DE                 MOVAPS XMM3,XMM6
140380b0a:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
140380b0f:  F3 0F 58 F7              ADDSS XMM6,XMM7
140380b13:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140380b18:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
140380b20:  66 41 0F 7E D9           MOVD R9D,XMM3
140380b25:  0F 28 D6                 MOVAPS XMM2,XMM6
140380b28:  66 41 0F 7E D0           MOVD R8D,XMM2
140380b2d:  48 8B 55 68              MOV RDX,qword ptr [RBP + 0x68]
140380b31:  48 8D 0D 78 64 61 00     LEA RCX,[0x140996fb0]
140380b38:  FF 15 42 BD 4E 00        CALL qword ptr [0x14086c880]
140380b3e:  F3 0F 10 44 24 78        MOVSS XMM0,dword ptr [RSP + 0x78]
140380b44:  F3 0F 59 05 14 95 58 00  MULSS XMM0,dword ptr [0x14090a060]
140380b4c:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140380b51:  F3 44 0F 11 54 24 70     MOVSS dword ptr [RSP + 0x70],XMM10
140380b58:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140380b5d:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140380b61:  48 8B CE                 MOV RCX,RSI
140380b64:  E8 77 E8 FD FF           CALL 0x14035f3e0
140380b69:  84 C0                    TEST AL,AL
140380b6b:  0F 94 C0                 SETZ AL
140380b6e:  84 C0                    TEST AL,AL
140380b70:  75 0A                    JNZ 0x140380b7c
140380b72:  C6 44 24 7D 01           MOV byte ptr [RSP + 0x7d],0x1
140380b77:  E9 9F 01 00 00           JMP 0x140380d1b
140380b7c:  80 7D B8 00              CMP byte ptr [RBP + -0x48],0x0
140380b80:  0F 84 95 01 00 00        JZ 0x140380d1b
140380b86:  8B 45 C0                 MOV EAX,dword ptr [RBP + -0x40]
140380b89:  85 C0                    TEST EAX,EAX
140380b8b:  0F 88 8A 01 00 00        JS 0x140380d1b
140380b91:  48 83 7C 24 50 00        CMP qword ptr [RSP + 0x50],0x0
140380b97:  0F 84 7E 01 00 00        JZ 0x140380d1b
140380b9d:  41 B4 01                 MOV R12B,0x1
140380ba0:  41 83 C8 FF              OR R8D,0xffffffff
140380ba4:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380ba9:  85 C0                    TEST EAX,EAX
140380bab:  75 5A                    JNZ 0x140380c07
140380bad:  48 8D 15 A4 F4 53 00     LEA RDX,[0x1408c0058]
140380bb4:  E8 F7 A1 ED FF           CALL 0x14025adb0
140380bb9:  90                       NOP
140380bba:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380bbf:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380bc3:  E8 28 A2 0C 00           CALL 0x14044adf0
140380bc8:  90                       NOP
140380bc9:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380bce:  E8 ED 0E C8 FF           CALL 0x140001ac0
140380bd3:  41 83 C8 FF              OR R8D,0xffffffff
140380bd7:  48 8D 15 9A F3 53 00     LEA RDX,[0x1408bff78]
140380bde:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380be3:  E8 C8 A1 ED FF           CALL 0x14025adb0
140380be8:  90                       NOP
140380be9:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380bee:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380bf2:  E8 F9 A1 0C 00           CALL 0x14044adf0
140380bf7:  90                       NOP
140380bf8:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380bfd:  E8 BE 0E C8 FF           CALL 0x140001ac0
140380c02:  E9 18 01 00 00           JMP 0x140380d1f
140380c07:  83 F8 01                 CMP EAX,0x1
140380c0a:  75 5A                    JNZ 0x140380c66
140380c0c:  48 8D 15 D5 F2 53 00     LEA RDX,[0x1408bfee8]
140380c13:  E8 98 A1 ED FF           CALL 0x14025adb0
140380c18:  90                       NOP
140380c19:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380c1e:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380c22:  E8 C9 A1 0C 00           CALL 0x14044adf0
140380c27:  90                       NOP
140380c28:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380c2d:  E8 8E 0E C8 FF           CALL 0x140001ac0
140380c32:  41 83 C8 FF              OR R8D,0xffffffff
140380c36:  48 8D 15 93 F2 53 00     LEA RDX,[0x1408bfed0]
140380c3d:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380c42:  E8 69 A1 ED FF           CALL 0x14025adb0
140380c47:  90                       NOP
140380c48:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380c4d:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380c51:  E8 9A A1 0C 00           CALL 0x14044adf0
140380c56:  90                       NOP
140380c57:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380c5c:  E8 5F 0E C8 FF           CALL 0x140001ac0
140380c61:  E9 B9 00 00 00           JMP 0x140380d1f
140380c66:  48 8D 15 4B F2 53 00     LEA RDX,[0x1408bfeb8]
140380c6d:  E8 3E A1 ED FF           CALL 0x14025adb0
140380c72:  90                       NOP
140380c73:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380c78:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380c7c:  E8 6F A1 0C 00           CALL 0x14044adf0
140380c81:  90                       NOP
140380c82:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380c87:  E8 34 0E C8 FF           CALL 0x140001ac0
140380c8c:  41 83 C8 FF              OR R8D,0xffffffff
140380c90:  48 8D 15 09 F2 53 00     LEA RDX,[0x1408bfea0]
140380c97:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380c9c:  E8 0F A1 ED FF           CALL 0x14025adb0
140380ca1:  90                       NOP
140380ca2:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380ca7:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380cab:  E8 40 A1 0C 00           CALL 0x14044adf0
140380cb0:  90                       NOP
140380cb1:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380cb6:  E8 05 0E C8 FF           CALL 0x140001ac0
140380cbb:  41 83 C8 FF              OR R8D,0xffffffff
140380cbf:  48 8D 15 7A F2 53 00     LEA RDX,[0x1408bff40]
140380cc6:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380ccb:  E8 E0 A0 ED FF           CALL 0x14025adb0
140380cd0:  90                       NOP
140380cd1:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380cd6:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380cda:  E8 11 A1 0C 00           CALL 0x14044adf0
140380cdf:  90                       NOP
140380ce0:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380ce5:  E8 D6 0D C8 FF           CALL 0x140001ac0
140380cea:  41 83 C8 FF              OR R8D,0xffffffff
140380cee:  48 8D 15 33 F2 53 00     LEA RDX,[0x1408bff28]
140380cf5:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380cfa:  E8 B1 A0 ED FF           CALL 0x14025adb0
140380cff:  90                       NOP
140380d00:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380d05:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380d09:  E8 E2 A0 0C 00           CALL 0x14044adf0
140380d0e:  90                       NOP
140380d0f:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380d14:  E8 A7 0D C8 FF           CALL 0x140001ac0
140380d19:  EB 04                    JMP 0x140380d1f
140380d1b:  44 8B 65 AC              MOV R12D,dword ptr [RBP + -0x54]
140380d1f:  48 8D 4D 38              LEA RCX,[RBP + 0x38]
140380d23:  E8 C8 31 C8 FF           CALL 0x140003ef0
140380d28:  F3 44 0F 10 54 24 70     MOVSS XMM10,dword ptr [RSP + 0x70]
140380d2f:  F3 0F 10 35 51 13 61 00  MOVSS XMM6,dword ptr [0x140992088]
140380d37:  E9 82 17 00 00           JMP 0x1403824be
140380d3c:  83 F9 07                 CMP ECX,0x7
140380d3f:  0F 85 C7 01 00 00        JNZ 0x140380f0c
140380d45:  4C 8D 8D 90 00 00 00     LEA R9,[RBP + 0x90]
140380d4c:  4C 8D 44 24 70           LEA R8,[RSP + 0x70]
140380d51:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
140380d55:  48 8B CE                 MOV RCX,RSI
140380d58:  E8 03 98 13 00           CALL 0x1404ba560
140380d5d:  41 B4 01                 MOV R12B,0x1
140380d60:  41 83 C8 FF              OR R8D,0xffffffff
140380d64:  48 8D 15 AD F1 53 00     LEA RDX,[0x1408bff18]
140380d6b:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380d70:  E8 3B A0 ED FF           CALL 0x14025adb0
140380d75:  90                       NOP
140380d76:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380d7b:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380d7f:  E8 6C A0 0C 00           CALL 0x14044adf0
140380d84:  90                       NOP
140380d85:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380d8a:  E8 31 0D C8 FF           CALL 0x140001ac0
140380d8f:  41 83 C8 FF              OR R8D,0xffffffff
140380d93:  48 8D 15 66 F1 53 00     LEA RDX,[0x1408bff00]
140380d9a:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380d9f:  E8 0C A0 ED FF           CALL 0x14025adb0
140380da4:  90                       NOP
140380da5:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380daa:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380dae:  E8 3D A0 0C 00           CALL 0x14044adf0
140380db3:  90                       NOP
140380db4:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380db9:  E8 02 0D C8 FF           CALL 0x140001ac0
140380dbe:  41 83 C8 FF              OR R8D,0xffffffff
140380dc2:  48 8D 15 AF F3 53 00     LEA RDX,[0x1408c0178]
140380dc9:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380dce:  E8 DD 9F ED FF           CALL 0x14025adb0
140380dd3:  90                       NOP
140380dd4:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380dd9:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380ddd:  E8 0E A0 0C 00           CALL 0x14044adf0
140380de2:  90                       NOP
140380de3:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380de8:  E8 D3 0C C8 FF           CALL 0x140001ac0
140380ded:  41 83 C8 FF              OR R8D,0xffffffff
140380df1:  48 8D 15 60 F3 53 00     LEA RDX,[0x1408c0158]
140380df8:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380dfd:  E8 AE 9F ED FF           CALL 0x14025adb0
140380e02:  90                       NOP
140380e03:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380e08:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380e0c:  E8 DF 9F 0C 00           CALL 0x14044adf0
140380e11:  90                       NOP
140380e12:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380e17:  E8 A4 0C C8 FF           CALL 0x140001ac0
140380e1c:  48 8B 96 E8 C7 00 00     MOV RDX,qword ptr [RSI + 0xc7e8]
140380e23:  48 8B CE                 MOV RCX,RSI
140380e26:  E8 A5 C2 F2 FF           CALL 0x1402ad0d0
140380e2b:  84 C0                    TEST AL,AL
140380e2d:  74 2F                    JZ 0x140380e5e
140380e2f:  41 83 C8 FF              OR R8D,0xffffffff
140380e33:  48 8D 15 F6 F2 53 00     LEA RDX,[0x1408c0130]
140380e3a:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380e3f:  E8 6C 9F ED FF           CALL 0x14025adb0
140380e44:  90                       NOP
140380e45:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380e4a:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380e4e:  E8 9D 9F 0C 00           CALL 0x14044adf0
140380e53:  90                       NOP
140380e54:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380e59:  E8 62 0C C8 FF           CALL 0x140001ac0
140380e5e:  48 8B 86 D0 C2 00 00     MOV RAX,qword ptr [RSI + 0xc2d0]
140380e65:  48 39 86 E8 C7 00 00     CMP qword ptr [RSI + 0xc7e8],RAX
140380e6c:  74 13                    JZ 0x140380e81
140380e6e:  48 8B 96 E8 C7 00 00     MOV RDX,qword ptr [RSI + 0xc7e8]
140380e75:  48 8B CE                 MOV RCX,RSI
140380e78:  E8 B3 C2 F2 FF           CALL 0x1402ad130
140380e7d:  84 C0                    TEST AL,AL
140380e7f:  74 2F                    JZ 0x140380eb0
140380e81:  41 83 C8 FF              OR R8D,0xffffffff
140380e85:  48 8D 15 84 F2 53 00     LEA RDX,[0x1408c0110]
140380e8c:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380e91:  E8 1A 9F ED FF           CALL 0x14025adb0
140380e96:  90                       NOP
140380e97:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380e9c:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380ea0:  E8 4B 9F 0C 00           CALL 0x14044adf0
140380ea5:  90                       NOP
140380ea6:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380eab:  E8 10 0C C8 FF           CALL 0x140001ac0
140380eb0:  48 8B 86 E0 C2 00 00     MOV RAX,qword ptr [RSI + 0xc2e0]
140380eb7:  48 39 86 E8 C7 00 00     CMP qword ptr [RSI + 0xc7e8],RAX
140380ebe:  75 2F                    JNZ 0x140380eef
140380ec0:  41 83 C8 FF              OR R8D,0xffffffff
140380ec4:  48 8D 15 05 F3 53 00     LEA RDX,[0x1408c01d0]
140380ecb:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380ed0:  E8 DB 9E ED FF           CALL 0x14025adb0
140380ed5:  90                       NOP
140380ed6:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140380edb:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140380edf:  E8 0C 9F 0C 00           CALL 0x14044adf0
140380ee4:  90                       NOP
140380ee5:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140380eea:  E8 D1 0B C8 FF           CALL 0x140001ac0
140380eef:  F3 44 0F 10 54 24 70     MOVSS XMM10,dword ptr [RSP + 0x70]
140380ef6:  F3 0F 10 35 8A 11 61 00  MOVSS XMM6,dword ptr [0x140992088]
140380efe:  F3 44 0F 10 0D 21 9B 58 00 MOVSS XMM9,dword ptr [0x14090aa28]
140380f07:  E9 B7 15 00 00           JMP 0x1403824c3
140380f0c:  85 D2                    TEST EDX,EDX
140380f0e:  0F 88 8E 15 00 00        JS 0x1403824a2
140380f14:  48 8B 86 E8 C7 00 00     MOV RAX,qword ptr [RSI + 0xc7e8]
140380f1b:  48 89 44 24 50           MOV qword ptr [RSP + 0x50],RAX
140380f20:  4C 89 B6 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],R14
140380f27:  0F 28 FE                 MOVAPS XMM7,XMM6
140380f2a:  F3 0F 59 3D 86 9F 58 00  MULSS XMM7,dword ptr [0x14090aeb8]
140380f32:  F3 41 0F 58 FF           ADDSS XMM7,XMM15
140380f37:  44 0F 28 EE              MOVAPS XMM13,XMM6
140380f3b:  F3 44 0F 59 2D 40 A0 58 00 MULSS XMM13,dword ptr [0x14090af84]
140380f44:  F3 45 0F 58 EF           ADDSS XMM13,XMM15
140380f49:  83 BE D4 C7 00 00 00     CMP dword ptr [RSI + 0xc7d4],0x0
140380f50:  75 4F                    JNZ 0x140380fa1
140380f52:  BA DF 05 00 00           MOV EDX,0x5df
140380f57:  48 8D 0D 32 66 61 00     LEA RCX,[0x140997590]
140380f5e:  FF 15 2C B9 4E 00        CALL qword ptr [0x14086c890]
140380f64:  48 8B D8                 MOV RBX,RAX
140380f67:  BA DE 05 00 00           MOV EDX,0x5de
140380f6c:  48 8D 0D 1D 66 61 00     LEA RCX,[0x140997590]
140380f73:  FF 15 17 B9 4E 00        CALL qword ptr [0x14086c890]
140380f79:  4C 8B C8                 MOV R9,RAX
140380f7c:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140380f81:  4C 8D 05 30 F2 53 00     LEA R8,[0x1408c01b8]
140380f88:  BA 00 01 00 00           MOV EDX,0x100
140380f8d:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140380f94:  E8 F7 42 C8 FF           CALL 0x140005290
140380f99:  F3 0F 10 35 E7 10 61 00  MOVSS XMM6,dword ptr [0x140992088]
140380fa1:  41 8B FE                 MOV EDI,R14D
140380fa4:  83 BE D4 C7 00 00 01     CMP dword ptr [RSI + 0xc7d4],0x1
140380fab:  75 54                    JNZ 0x140381001
140380fad:  BA E0 05 00 00           MOV EDX,0x5e0
140380fb2:  48 8D 0D D7 65 61 00     LEA RCX,[0x140997590]
140380fb9:  FF 15 D1 B8 4E 00        CALL qword ptr [0x14086c890]
140380fbf:  48 8B D8                 MOV RBX,RAX
140380fc2:  BA DE 05 00 00           MOV EDX,0x5de
140380fc7:  48 8D 0D C2 65 61 00     LEA RCX,[0x140997590]
140380fce:  FF 15 BC B8 4E 00        CALL qword ptr [0x14086c890]
140380fd4:  4C 8B C8                 MOV R9,RAX
140380fd7:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140380fdc:  4C 8D 05 BD F1 53 00     LEA R8,[0x1408c01a0]
140380fe3:  BA 00 01 00 00           MOV EDX,0x100
140380fe8:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140380fef:  E8 9C 42 C8 FF           CALL 0x140005290
140380ff4:  BF 01 00 00 00           MOV EDI,0x1
140380ff9:  F3 0F 10 35 87 10 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381001:  83 BE D4 C7 00 00 02     CMP dword ptr [RSI + 0xc7d4],0x2
140381008:  75 52                    JNZ 0x14038105c
14038100a:  BA DF 05 00 00           MOV EDX,0x5df
14038100f:  48 8D 0D 7A 65 61 00     LEA RCX,[0x140997590]
140381016:  FF 15 74 B8 4E 00        CALL qword ptr [0x14086c890]
14038101c:  48 8B D8                 MOV RBX,RAX
14038101f:  BA E1 05 00 00           MOV EDX,0x5e1
140381024:  48 8D 0D 65 65 61 00     LEA RCX,[0x140997590]
14038102b:  FF 15 5F B8 4E 00        CALL qword ptr [0x14086c890]
140381031:  4C 8B C8                 MOV R9,RAX
140381034:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381039:  4C 8D 05 48 F1 53 00     LEA R8,[0x1408c0188]
140381040:  BA 00 01 00 00           MOV EDX,0x100
140381045:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
14038104c:  E8 3F 42 C8 FF           CALL 0x140005290
140381051:  41 8B FE                 MOV EDI,R14D
140381054:  F3 0F 10 35 2C 10 61 00  MOVSS XMM6,dword ptr [0x140992088]
14038105c:  83 BE D4 C7 00 00 03     CMP dword ptr [RSI + 0xc7d4],0x3
140381063:  75 54                    JNZ 0x1403810b9
140381065:  BA E0 05 00 00           MOV EDX,0x5e0
14038106a:  48 8D 0D 1F 65 61 00     LEA RCX,[0x140997590]
140381071:  FF 15 19 B8 4E 00        CALL qword ptr [0x14086c890]
140381077:  48 8B D8                 MOV RBX,RAX
14038107a:  BA E1 05 00 00           MOV EDX,0x5e1
14038107f:  48 8D 0D 0A 65 61 00     LEA RCX,[0x140997590]
140381086:  FF 15 04 B8 4E 00        CALL qword ptr [0x14086c890]
14038108c:  4C 8B C8                 MOV R9,RAX
14038108f:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381094:  4C 8D 05 1D F0 53 00     LEA R8,[0x1408c00b8]
14038109b:  BA 00 01 00 00           MOV EDX,0x100
1403810a0:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
1403810a7:  E8 E4 41 C8 FF           CALL 0x140005290
1403810ac:  BF 01 00 00 00           MOV EDI,0x1
1403810b1:  F3 0F 10 35 CF 0F 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403810b9:  83 BE D4 C7 00 00 04     CMP dword ptr [RSI + 0xc7d4],0x4
1403810c0:  75 54                    JNZ 0x140381116
1403810c2:  BA DF 05 00 00           MOV EDX,0x5df
1403810c7:  48 8D 0D C2 64 61 00     LEA RCX,[0x140997590]
1403810ce:  FF 15 BC B7 4E 00        CALL qword ptr [0x14086c890]
1403810d4:  48 8B D8                 MOV RBX,RAX
1403810d7:  BA E8 05 00 00           MOV EDX,0x5e8
1403810dc:  48 8D 0D AD 64 61 00     LEA RCX,[0x140997590]
1403810e3:  FF 15 A7 B7 4E 00        CALL qword ptr [0x14086c890]
1403810e9:  4C 8B C8                 MOV R9,RAX
1403810ec:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
1403810f1:  4C 8D 05 B8 EF 53 00     LEA R8,[0x1408c00b0]
1403810f8:  BA 00 01 00 00           MOV EDX,0x100
1403810fd:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140381104:  E8 87 41 C8 FF           CALL 0x140005290
140381109:  BF 02 00 00 00           MOV EDI,0x2
14038110e:  F3 0F 10 35 72 0F 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381116:  83 BE D4 C7 00 00 05     CMP dword ptr [RSI + 0xc7d4],0x5
14038111d:  75 54                    JNZ 0x140381173
14038111f:  BA EB 05 00 00           MOV EDX,0x5eb
140381124:  48 8D 0D 65 64 61 00     LEA RCX,[0x140997590]
14038112b:  FF 15 5F B7 4E 00        CALL qword ptr [0x14086c890]
140381131:  48 8B D8                 MOV RBX,RAX
140381134:  BA EA 05 00 00           MOV EDX,0x5ea
140381139:  48 8D 0D 50 64 61 00     LEA RCX,[0x140997590]
140381140:  FF 15 4A B7 4E 00        CALL qword ptr [0x14086c890]
140381146:  4C 8B C8                 MOV R9,RAX
140381149:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
14038114e:  4C 8D 05 43 EF 53 00     LEA R8,[0x1408c0098]
140381155:  BA 00 01 00 00           MOV EDX,0x100
14038115a:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140381161:  E8 2A 41 C8 FF           CALL 0x140005290
140381166:  BF 02 00 00 00           MOV EDI,0x2
14038116b:  F3 0F 10 35 15 0F 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381173:  83 BE D4 C7 00 00 06     CMP dword ptr [RSI + 0xc7d4],0x6
14038117a:  75 54                    JNZ 0x1403811d0
14038117c:  BA ED 05 00 00           MOV EDX,0x5ed
140381181:  48 8D 0D 08 64 61 00     LEA RCX,[0x140997590]
140381188:  FF 15 02 B7 4E 00        CALL qword ptr [0x14086c890]
14038118e:  48 8B D8                 MOV RBX,RAX
140381191:  BA EA 05 00 00           MOV EDX,0x5ea
140381196:  48 8D 0D F3 63 61 00     LEA RCX,[0x140997590]
14038119d:  FF 15 ED B6 4E 00        CALL qword ptr [0x14086c890]
1403811a3:  4C 8B C8                 MOV R9,RAX
1403811a6:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
1403811ab:  4C 8D 05 CE EE 53 00     LEA R8,[0x1408c0080]
1403811b2:  BA 00 01 00 00           MOV EDX,0x100
1403811b7:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
1403811be:  E8 CD 40 C8 FF           CALL 0x140005290
1403811c3:  BF 02 00 00 00           MOV EDI,0x2
1403811c8:  F3 0F 10 35 B8 0E 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403811d0:  83 BE D4 C7 00 00 07     CMP dword ptr [RSI + 0xc7d4],0x7
1403811d7:  75 54                    JNZ 0x14038122d
1403811d9:  BA EC 05 00 00           MOV EDX,0x5ec
1403811de:  48 8D 0D AB 63 61 00     LEA RCX,[0x140997590]
1403811e5:  FF 15 A5 B6 4E 00        CALL qword ptr [0x14086c890]
1403811eb:  48 8B D8                 MOV RBX,RAX
1403811ee:  BA EA 05 00 00           MOV EDX,0x5ea
1403811f3:  48 8D 0D 96 63 61 00     LEA RCX,[0x140997590]
1403811fa:  FF 15 90 B6 4E 00        CALL qword ptr [0x14086c890]
140381200:  4C 8B C8                 MOV R9,RAX
140381203:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381208:  4C 8D 05 E9 EE 53 00     LEA R8,[0x1408c00f8]
14038120f:  BA 00 01 00 00           MOV EDX,0x100
140381214:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
14038121b:  E8 70 40 C8 FF           CALL 0x140005290
140381220:  BF 02 00 00 00           MOV EDI,0x2
140381225:  F3 0F 10 35 5B 0E 61 00  MOVSS XMM6,dword ptr [0x140992088]
14038122d:  83 BE D4 C7 00 00 08     CMP dword ptr [RSI + 0xc7d4],0x8
140381234:  75 54                    JNZ 0x14038128a
140381236:  BA 81 06 00 00           MOV EDX,0x681
14038123b:  48 8D 0D 4E 63 61 00     LEA RCX,[0x140997590]
140381242:  FF 15 48 B6 4E 00        CALL qword ptr [0x14086c890]
140381248:  48 8B D8                 MOV RBX,RAX
14038124b:  BA EA 05 00 00           MOV EDX,0x5ea
140381250:  48 8D 0D 39 63 61 00     LEA RCX,[0x140997590]
140381257:  FF 15 33 B6 4E 00        CALL qword ptr [0x14086c890]
14038125d:  4C 8B C8                 MOV R9,RAX
140381260:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381265:  4C 8D 05 74 EE 53 00     LEA R8,[0x1408c00e0]
14038126c:  BA 00 01 00 00           MOV EDX,0x100
140381271:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140381278:  E8 13 40 C8 FF           CALL 0x140005290
14038127d:  BF 02 00 00 00           MOV EDI,0x2
140381282:  F3 0F 10 35 FE 0D 61 00  MOVSS XMM6,dword ptr [0x140992088]
14038128a:  83 BE D4 C7 00 00 09     CMP dword ptr [RSI + 0xc7d4],0x9
140381291:  75 3A                    JNZ 0x1403812cd
140381293:  BA B3 92 00 00           MOV EDX,0x92b3
140381298:  48 8D 0D F1 62 61 00     LEA RCX,[0x140997590]
14038129f:  FF 15 EB B5 4E 00        CALL qword ptr [0x14086c890]
1403812a5:  4C 8B C8                 MOV R9,RAX
1403812a8:  4C 8D 05 29 EE 53 00     LEA R8,[0x1408c00d8]
1403812af:  BA 00 01 00 00           MOV EDX,0x100
1403812b4:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
1403812bb:  E8 D0 3F C8 FF           CALL 0x140005290
1403812c0:  BF 02 00 00 00           MOV EDI,0x2
1403812c5:  F3 0F 10 35 BB 0D 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403812cd:  83 BE D4 C7 00 00 0A     CMP dword ptr [RSI + 0xc7d4],0xa
1403812d4:  75 3A                    JNZ 0x140381310
1403812d6:  BA B4 92 00 00           MOV EDX,0x92b4
1403812db:  48 8D 0D AE 62 61 00     LEA RCX,[0x140997590]
1403812e2:  FF 15 A8 B5 4E 00        CALL qword ptr [0x14086c890]
1403812e8:  4C 8B C8                 MOV R9,RAX
1403812eb:  4C 8D 05 DE ED 53 00     LEA R8,[0x1408c00d0]
1403812f2:  BA 00 01 00 00           MOV EDX,0x100
1403812f7:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
1403812fe:  E8 8D 3F C8 FF           CALL 0x140005290
140381303:  BF 01 00 00 00           MOV EDI,0x1
140381308:  F3 0F 10 35 78 0D 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381310:  83 BE D4 C7 00 00 46     CMP dword ptr [RSI + 0xc7d4],0x46
140381317:  75 54                    JNZ 0x14038136d
140381319:  BA EB 05 00 00           MOV EDX,0x5eb
14038131e:  48 8D 0D 6B 62 61 00     LEA RCX,[0x140997590]
140381325:  FF 15 65 B5 4E 00        CALL qword ptr [0x14086c890]
14038132b:  48 8B D8                 MOV RBX,RAX
14038132e:  BA E8 10 00 00           MOV EDX,0x10e8
140381333:  48 8D 0D 56 62 61 00     LEA RCX,[0x140997590]
14038133a:  FF 15 50 B5 4E 00        CALL qword ptr [0x14086c890]
140381340:  4C 8B C8                 MOV R9,RAX
140381343:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381348:  4C 8D 05 19 F1 53 00     LEA R8,[0x1408c0468]
14038134f:  BA 00 01 00 00           MOV EDX,0x100
140381354:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
14038135b:  E8 30 3F C8 FF           CALL 0x140005290
140381360:  BF 02 00 00 00           MOV EDI,0x2
140381365:  F3 0F 10 35 1B 0D 61 00  MOVSS XMM6,dword ptr [0x140992088]
14038136d:  83 BE D4 C7 00 00 47     CMP dword ptr [RSI + 0xc7d4],0x47
140381374:  75 54                    JNZ 0x1403813ca
140381376:  BA E9 10 00 00           MOV EDX,0x10e9
14038137b:  48 8D 0D 0E 62 61 00     LEA RCX,[0x140997590]
140381382:  FF 15 08 B5 4E 00        CALL qword ptr [0x14086c890]
140381388:  48 8B D8                 MOV RBX,RAX
14038138b:  BA E8 10 00 00           MOV EDX,0x10e8
140381390:  48 8D 0D F9 61 61 00     LEA RCX,[0x140997590]
140381397:  FF 15 F3 B4 4E 00        CALL qword ptr [0x14086c890]
14038139d:  4C 8B C8                 MOV R9,RAX
1403813a0:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
1403813a5:  4C 8D 05 A4 F0 53 00     LEA R8,[0x1408c0450]
1403813ac:  BA 00 01 00 00           MOV EDX,0x100
1403813b1:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
1403813b8:  E8 D3 3E C8 FF           CALL 0x140005290
1403813bd:  BF 02 00 00 00           MOV EDI,0x2
1403813c2:  F3 0F 10 35 BE 0C 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403813ca:  83 BE D4 C7 00 00 48     CMP dword ptr [RSI + 0xc7d4],0x48
1403813d1:  75 54                    JNZ 0x140381427
1403813d3:  BA F9 10 00 00           MOV EDX,0x10f9
1403813d8:  48 8D 0D B1 61 61 00     LEA RCX,[0x140997590]
1403813df:  FF 15 AB B4 4E 00        CALL qword ptr [0x14086c890]
1403813e5:  48 8B D8                 MOV RBX,RAX
1403813e8:  BA E8 10 00 00           MOV EDX,0x10e8
1403813ed:  48 8D 0D 9C 61 61 00     LEA RCX,[0x140997590]
1403813f4:  FF 15 96 B4 4E 00        CALL qword ptr [0x14086c890]
1403813fa:  4C 8B C8                 MOV R9,RAX
1403813fd:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140381402:  4C 8D 05 2F F0 53 00     LEA R8,[0x1408c0438]
140381409:  BA 00 01 00 00           MOV EDX,0x100
14038140e:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
140381415:  E8 76 3E C8 FF           CALL 0x140005290
14038141a:  BF 02 00 00 00           MOV EDI,0x2
14038141f:  F3 0F 10 35 61 0C 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381427:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14038142c:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381430:  F3 0F 5C DE              SUBSS XMM3,XMM6
140381434:  F3 41 0F 58 F7           ADDSS XMM6,XMM15
140381439:  48 8D 85 30 0B 00 00     LEA RAX,[RBP + 0xb30]
140381440:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381445:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
14038144d:  66 41 0F 7E D9           MOVD R9D,XMM3
140381452:  0F 28 D6                 MOVAPS XMM2,XMM6
140381455:  66 41 0F 7E D0           MOVD R8D,XMM2
14038145a:  48 8B 15 DF 3D 61 00     MOV RDX,qword ptr [0x140995240]
140381461:  48 8D 0D 48 5B 61 00     LEA RCX,[0x140996fb0]
140381468:  FF 15 12 B4 4E 00        CALL qword ptr [0x14086c880]
14038146e:  41 0F 28 C3              MOVAPS XMM0,XMM11
140381472:  F3 0F 59 05 E6 8B 58 00  MULSS XMM0,dword ptr [0x14090a060]
14038147a:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
14038147f:  8B 86 D4 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d4]
140381485:  83 E8 09                 SUB EAX,0x9
140381488:  F3 44 0F 10 3D 4B 89 58 00 MOVSS XMM15,dword ptr [0x140909ddc]
140381491:  83 F8 01                 CMP EAX,0x1
140381494:  0F 86 FA 02 00 00        JBE 0x140381794
14038149a:  F3 0F 10 35 E6 0B 61 00  MOVSS XMM6,dword ptr [0x140992088]
1403814a2:  F3 41 0F 59 F0           MULSS XMM6,XMM8
1403814a7:  BA 85 06 00 00           MOV EDX,0x685
1403814ac:  48 8D 0D DD 60 61 00     LEA RCX,[0x140997590]
1403814b3:  FF 15 D7 B3 4E 00        CALL qword ptr [0x14086c890]
1403814b9:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403814bd:  F3 0F 5C DE              SUBSS XMM3,XMM6
1403814c1:  F3 44 0F 10 64 24 74     MOVSS XMM12,dword ptr [RSP + 0x74]
1403814c8:  F3 41 0F 58 F4           ADDSS XMM6,XMM12
1403814cd:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403814d2:  48 8D 05 4F EF 53 00     LEA RAX,[0x1408c0428]
1403814d9:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403814de:  C7 44 24 20 00 00 AA FF  MOV dword ptr [RSP + 0x20],0xffaa0000
1403814e6:  66 41 0F 7E D9           MOVD R9D,XMM3
1403814eb:  0F 28 D6                 MOVAPS XMM2,XMM6
1403814ee:  66 41 0F 7E D0           MOVD R8D,XMM2
1403814f3:  49 8B D7                 MOV RDX,R15
1403814f6:  48 8D 0D B3 5A 61 00     LEA RCX,[0x140996fb0]
1403814fd:  FF 15 7D B3 4E 00        CALL qword ptr [0x14086c880]
140381503:  BA 77 06 00 00           MOV EDX,0x677
140381508:  48 8D 0D 81 60 61 00     LEA RCX,[0x140997590]
14038150f:  FF 15 7B B3 4E 00        CALL qword ptr [0x14086c890]
140381515:  F3 0F 10 05 6B 0B 61 00  MOVSS XMM0,dword ptr [0x140992088]
14038151d:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140381522:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381526:  F3 0F 5C D8              SUBSS XMM3,XMM0
14038152a:  66 0F 6E 05 5A 3D 61 00  MOVD XMM0,dword ptr [0x14099528c]
140381532:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381535:  F3 41 0F 59 C7           MULSS XMM0,XMM15
14038153a:  0F 28 D7                 MOVAPS XMM2,XMM7
14038153d:  F3 0F 5C D0              SUBSS XMM2,XMM0
140381541:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140381546:  48 8D 05 E3 EF 53 00     LEA RAX,[0x1408c0530]
14038154d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381552:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14038155a:  66 41 0F 7E D9           MOVD R9D,XMM3
14038155f:  66 41 0F 7E D0           MOVD R8D,XMM2
140381564:  49 8B D7                 MOV RDX,R15
140381567:  48 8D 0D 42 5A 61 00     LEA RCX,[0x140996fb0]
14038156e:  FF 15 14 B3 4E 00        CALL qword ptr [0x14086c888]
140381574:  49 8B 1F                 MOV RBX,qword ptr [R15]
140381577:  BA 77 06 00 00           MOV EDX,0x677
14038157c:  48 8D 0D 0D 60 61 00     LEA RCX,[0x140997590]
140381583:  FF 15 07 B3 4E 00        CALL qword ptr [0x14086c890]
140381589:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14038158e:  48 8D 05 93 EF 53 00     LEA RAX,[0x1408c0528]
140381595:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14038159a:  41 0F 28 D8              MOVAPS XMM3,XMM8
14038159e:  66 41 0F 7E D9           MOVD R9D,XMM3
1403815a3:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403815a7:  66 41 0F 7E D0           MOVD R8D,XMM2
1403815ac:  48 8D 95 20 0B 00 00     LEA RDX,[RBP + 0xb20]
1403815b3:  49 8B CF                 MOV RCX,R15
1403815b6:  FF 53 28                 CALL qword ptr [RBX + 0x28]
1403815b9:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
1403815bc:  0F 11 45 E0              MOVUPS xmmword ptr [RBP + -0x20],XMM0
1403815c0:  49 8B 1F                 MOV RBX,qword ptr [R15]
1403815c3:  BA 85 06 00 00           MOV EDX,0x685
1403815c8:  48 8D 0D C1 5F 61 00     LEA RCX,[0x140997590]
1403815cf:  FF 15 BB B2 4E 00        CALL qword ptr [0x14086c890]
1403815d5:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1403815da:  48 8D 05 3F EF 53 00     LEA RAX,[0x1408c0520]
1403815e1:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1403815e6:  41 0F 28 D8              MOVAPS XMM3,XMM8
1403815ea:  66 41 0F 7E D9           MOVD R9D,XMM3
1403815ef:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403815f3:  66 41 0F 7E D0           MOVD R8D,XMM2
1403815f8:  48 8D 95 F0 0A 00 00     LEA RDX,[RBP + 0xaf0]
1403815ff:  49 8B CF                 MOV RCX,R15
140381602:  FF 53 28                 CALL qword ptr [RBX + 0x28]
140381605:  0F 10 10                 MOVUPS XMM2,xmmword ptr [RAX]
140381608:  F3 0F 10 35 78 0A 61 00  MOVSS XMM6,dword ptr [0x140992088]
140381610:  0F 28 CE                 MOVAPS XMM1,XMM6
140381613:  F3 0F 59 0D C5 91 58 00  MULSS XMM1,dword ptr [0x14090a7e0]
14038161b:  45 0F 28 DA              MOVAPS XMM11,XMM10
14038161f:  F3 44 0F 5C D9           SUBSS XMM11,XMM1
140381624:  F3 0F 59 35 A0 92 58 00  MULSS XMM6,dword ptr [0x14090a8cc]
14038162c:  F3 41 0F 58 F2           ADDSS XMM6,XMM10
140381631:  66 0F 73 DA 08           PSRLDQ XMM2,0x8
140381636:  66 0F 7E D0              MOVD EAX,XMM2
14038163a:  66 0F 6E C0              MOVD XMM0,EAX
14038163e:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381641:  0F 28 C8                 MOVAPS XMM1,XMM0
140381644:  F3 41 0F 58 CC           ADDSS XMM1,XMM12
140381649:  F3 41 0F 59 C7           MULSS XMM0,XMM15
14038164e:  41 0F 28 D4              MOVAPS XMM2,XMM12
140381652:  F3 0F 5C D0              SUBSS XMM2,XMM0
140381656:  F3 0F 11 74 24 28        MOVSS dword ptr [RSP + 0x28],XMM6
14038165c:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
140381662:  41 0F 28 DB              MOVAPS XMM3,XMM11
140381666:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14038166b:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140381670:  E8 2B A0 ED FF           CALL 0x14025b6a0
140381675:  84 C0                    TEST AL,AL
140381677:  74 66                    JZ 0x1403816df
140381679:  BA D0 07 00 00           MOV EDX,0x7d0
14038167e:  48 8D 0D 0B 5F 61 00     LEA RCX,[0x140997590]
140381685:  FF 15 05 B2 4E 00        CALL qword ptr [0x14086c890]
14038168b:  48 8B D6                 MOV RDX,RSI
14038168e:  48 2B D0                 SUB RDX,RAX
140381691:  0F B7 08                 MOVZX ECX,word ptr [RAX]
140381694:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14038169c:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403816a0:  66 85 C9                 TEST CX,CX
1403816a3:  75 EC                    JNZ 0x140381691
1403816a5:  48 8D 0D E4 34 6D 00     LEA RCX,[0x140a54b90]
1403816ac:  FF 15 06 B2 4E 00        CALL qword ptr [0x14086c8b8]
1403816b2:  84 C0                    TEST AL,AL
1403816b4:  74 29                    JZ 0x1403816df
1403816b6:  83 BE 0C C8 00 00 00     CMP dword ptr [RSI + 0xc80c],0x0
1403816bd:  74 09                    JZ 0x1403816c8
1403816bf:  C6 86 09 C8 00 00 01     MOV byte ptr [RSI + 0xc809],0x1
1403816c6:  EB 10                    JMP 0x1403816d8
1403816c8:  80 BE 09 C8 00 00 00     CMP byte ptr [RSI + 0xc809],0x0
1403816cf:  0F 94 C0                 SETZ AL
1403816d2:  88 86 09 C8 00 00        MOV byte ptr [RSI + 0xc809],AL
1403816d8:  44 89 B6 0C C8 00 00     MOV dword ptr [RSI + 0xc80c],R14D
1403816df:  66 0F 6E 4D E8           MOVD XMM1,dword ptr [RBP + -0x18]
1403816e4:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1403816e7:  0F 28 C1                 MOVAPS XMM0,XMM1
1403816ea:  F3 41 0F 59 C7           MULSS XMM0,XMM15
1403816ef:  0F 28 D7                 MOVAPS XMM2,XMM7
1403816f2:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403816f6:  F3 0F 58 CF              ADDSS XMM1,XMM7
1403816fa:  F3 0F 11 74 24 28        MOVSS dword ptr [RSP + 0x28],XMM6
140381700:  F3 0F 11 4C 24 20        MOVSS dword ptr [RSP + 0x20],XMM1
140381706:  41 0F 28 DB              MOVAPS XMM3,XMM11
14038170a:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14038170f:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140381714:  E8 87 9F ED FF           CALL 0x14025b6a0
140381719:  84 C0                    TEST AL,AL
14038171b:  0F 84 7C 00 00 00        JZ 0x14038179d
140381721:  BA 86 06 00 00           MOV EDX,0x686
140381726:  48 8D 0D 63 5E 61 00     LEA RCX,[0x140997590]
14038172d:  FF 15 5D B1 4E 00        CALL qword ptr [0x14086c890]
140381733:  48 8B D6                 MOV RDX,RSI
140381736:  48 2B D0                 SUB RDX,RAX
140381739:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
140381740:  0F B7 08                 MOVZX ECX,word ptr [RAX]
140381743:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14038174b:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14038174f:  66 85 C9                 TEST CX,CX
140381752:  75 EC                    JNZ 0x140381740
140381754:  48 8D 0D 35 34 6D 00     LEA RCX,[0x140a54b90]
14038175b:  FF 15 57 B1 4E 00        CALL qword ptr [0x14086c8b8]
140381761:  84 C0                    TEST AL,AL
140381763:  74 38                    JZ 0x14038179d
140381765:  83 BE 0C C8 00 00 01     CMP dword ptr [RSI + 0xc80c],0x1
14038176c:  74 09                    JZ 0x140381777
14038176e:  C6 86 09 C8 00 00 00     MOV byte ptr [RSI + 0xc809],0x0
140381775:  EB 10                    JMP 0x140381787
140381777:  80 BE 09 C8 00 00 00     CMP byte ptr [RSI + 0xc809],0x0
14038177e:  0F 94 C0                 SETZ AL
140381781:  88 86 09 C8 00 00        MOV byte ptr [RSI + 0xc809],AL
140381787:  B8 01 00 00 00           MOV EAX,0x1
14038178c:  89 86 0C C8 00 00        MOV dword ptr [RSI + 0xc80c],EAX
140381792:  EB 09                    JMP 0x14038179d
140381794:  F3 44 0F 10 64 24 74     MOVSS XMM12,dword ptr [RSP + 0x74]
14038179b:  EB 07                    JMP 0x1403817a4
14038179d:  F3 44 0F 10 5C 24 78     MOVSS XMM11,dword ptr [RSP + 0x78]
1403817a4:  85 FF                    TEST EDI,EDI
1403817a6:  75 09                    JNZ 0x1403817b1
1403817a8:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
1403817af:  EB 23                    JMP 0x1403817d4
1403817b1:  83 FF 01                 CMP EDI,0x1
1403817b4:  74 17                    JZ 0x1403817cd
1403817b6:  83 FF 02                 CMP EDI,0x2
1403817b9:  75 34                    JNZ 0x1403817ef
1403817bb:  83 BE D0 C7 00 00 00     CMP dword ptr [RSI + 0xc7d0],0x0
1403817c2:  75 09                    JNZ 0x1403817cd
1403817c4:  48 8B 9E C0 AE 00 00     MOV RBX,qword ptr [RSI + 0xaec0]
1403817cb:  EB 07                    JMP 0x1403817d4
1403817cd:  48 8B 9E B8 AE 00 00     MOV RBX,qword ptr [RSI + 0xaeb8]
1403817d4:  48 8B 3B                 MOV RDI,qword ptr [RBX]
1403817d7:  48 8D 0D F2 94 66 00     LEA RCX,[0x1409eacd0]
1403817de:  FF 15 0C AF 4E 00        CALL qword ptr [0x14086c6f0]
1403817e4:  4C 8B C0                 MOV R8,RAX
1403817e7:  33 D2                    XOR EDX,EDX
1403817e9:  48 8B CB                 MOV RCX,RBX
1403817ec:  FF 57 70                 CALL qword ptr [RDI + 0x70]
1403817ef:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
1403817f6:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403817fa:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403817fe:  41 0F 28 C9              MOVAPS XMM1,XMM9
140381802:  48 8D 8D 40 0A 00 00     LEA RCX,[RBP + 0xa40]
140381809:  FF 15 11 A8 4E 00        CALL qword ptr [0x14086c020]
14038180f:  48 8B D0                 MOV RDX,RAX
140381812:  48 8D 0D 47 C8 63 00     LEA RCX,[0x1409be060]
140381819:  FF 15 79 B0 4E 00        CALL qword ptr [0x14086c898]
14038181f:  F3 0F 10 0D 61 08 61 00  MOVSS XMM1,dword ptr [0x140992088]
140381827:  F3 0F 59 0D DD 91 58 00  MULSS XMM1,dword ptr [0x14090aa0c]
14038182f:  0F 28 D1                 MOVAPS XMM2,XMM1
140381832:  48 8D 0D 27 C8 63 00     LEA RCX,[0x1409be060]
140381839:  FF 15 69 B0 4E 00        CALL qword ptr [0x14086c8a8]
14038183f:  F3 0F 10 15 41 08 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381847:  F3 0F 59 15 D9 8F 58 00  MULSS XMM2,dword ptr [0x14090a828]
14038184f:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140381854:  41 0F 28 D8              MOVAPS XMM3,XMM8
140381858:  41 0F 28 CD              MOVAPS XMM1,XMM13
14038185c:  48 8D 8D C8 04 00 00     LEA RCX,[RBP + 0x4c8]
140381863:  FF 15 9F A7 4E 00        CALL qword ptr [0x14086c008]
140381869:  48 8B D0                 MOV RDX,RAX
14038186c:  48 8D 0D ED C7 63 00     LEA RCX,[0x1409be060]
140381873:  FF 15 27 B0 4E 00        CALL qword ptr [0x14086c8a0]
140381879:  8B 86 D4 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d4]
14038187f:  83 E8 09                 SUB EAX,0x9
140381882:  83 F8 01                 CMP EAX,0x1
140381885:  0F 86 96 00 00 00        JBE 0x140381921
14038188b:  48 8D 55 80              LEA RDX,[RBP + -0x80]
14038188f:  48 8D 8D B8 04 00 00     LEA RCX,[RBP + 0x4b8]
140381896:  FF 15 74 A7 4E 00        CALL qword ptr [0x14086c010]
14038189c:  48 8B D0                 MOV RDX,RAX
14038189f:  41 0F 28 D8              MOVAPS XMM3,XMM8
1403818a3:  41 0F 28 D0              MOVAPS XMM2,XMM8
1403818a7:  48 8D 0D B2 C7 63 00     LEA RCX,[0x1409be060]
1403818ae:  FF 15 9C AF 4E 00        CALL qword ptr [0x14086c850]
1403818b4:  84 C0                    TEST AL,AL
1403818b6:  74 69                    JZ 0x140381921
1403818b8:  BA 87 06 00 00           MOV EDX,0x687
1403818bd:  48 8D 0D CC 5C 61 00     LEA RCX,[0x140997590]
1403818c4:  FF 15 C6 AF 4E 00        CALL qword ptr [0x14086c890]
1403818ca:  48 8B D6                 MOV RDX,RSI
1403818cd:  48 2B D0                 SUB RDX,RAX
1403818d0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
1403818d3:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
1403818db:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403818df:  66 85 C9                 TEST CX,CX
1403818e2:  75 EC                    JNZ 0x1403818d0
1403818e4:  48 8D 0D A5 32 6D 00     LEA RCX,[0x140a54b90]
1403818eb:  FF 15 C7 AF 4E 00        CALL qword ptr [0x14086c8b8]
1403818f1:  84 C0                    TEST AL,AL
1403818f3:  74 2C                    JZ 0x140381921
1403818f5:  83 BE 0C C8 00 00 02     CMP dword ptr [RSI + 0xc80c],0x2
1403818fc:  74 09                    JZ 0x140381907
1403818fe:  C6 86 09 C8 00 00 00     MOV byte ptr [RSI + 0xc809],0x0
140381905:  EB 10                    JMP 0x140381917
140381907:  80 BE 09 C8 00 00 00     CMP byte ptr [RSI + 0xc809],0x0
14038190e:  0F 94 C0                 SETZ AL
140381911:  88 86 09 C8 00 00        MOV byte ptr [RSI + 0xc809],AL
140381917:  C7 86 0C C8 00 00 02 00 00 00 MOV dword ptr [RSI + 0xc80c],0x2
140381921:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140381926:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
14038192d:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140381934:  41 0F 28 D9              MOVAPS XMM3,XMM9
140381938:  41 0F 28 D0              MOVAPS XMM2,XMM8
14038193c:  41 0F 28 C8              MOVAPS XMM1,XMM8
140381940:  48 8D 0D 19 C7 63 00     LEA RCX,[0x1409be060]
140381947:  FF 15 13 AF 4E 00        CALL qword ptr [0x14086c860]
14038194d:  41 0F 28 F3              MOVAPS XMM6,XMM11
140381951:  F3 0F 59 35 53 85 58 00  MULSS XMM6,dword ptr [0x140909eac]
140381959:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
14038195e:  8B 05 0C 39 61 00        MOV EAX,dword ptr [0x140995270]
140381964:  83 F8 02                 CMP EAX,0x2
140381967:  75 09                    JNZ 0x140381972
140381969:  48 8D 05 10 EB 53 00     LEA RAX,[0x1408c0480]
140381970:  EB 13                    JMP 0x140381985
140381972:  83 F8 01                 CMP EAX,0x1
140381975:  48 8D 05 A4 E9 53 00     LEA RAX,[0x1408c0320]
14038197c:  74 07                    JZ 0x140381985
14038197e:  48 8D 05 AB E8 53 00     LEA RAX,[0x1408c0230]
140381985:  F3 0F 10 15 FB 06 61 00  MOVSS XMM2,dword ptr [0x140992088]
14038198d:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140381992:  0F 28 DA                 MOVAPS XMM3,XMM2
140381995:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14038199a:  F3 41 0F 58 DA           ADDSS XMM3,XMM10
14038199f:  F3 41 0F 58 D4           ADDSS XMM2,XMM12
1403819a4:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1403819ac:  66 41 0F 7E D9           MOVD R9D,XMM3
1403819b1:  66 41 0F 7E D0           MOVD R8D,XMM2
1403819b6:  49 8B D7                 MOV RDX,R15
1403819b9:  48 8D 0D F0 55 61 00     LEA RCX,[0x140996fb0]
1403819c0:  FF 15 BA AE 4E 00        CALL qword ptr [0x14086c880]
1403819c6:  F3 44 0F 58 D6           ADDSS XMM10,XMM6
1403819cb:  8B 86 D4 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7d4]
1403819d1:  83 E8 09                 SUB EAX,0x9
1403819d4:  83 F8 01                 CMP EAX,0x1
1403819d7:  0F 86 94 06 00 00        JBE 0x140382071
1403819dd:  45 0F 28 F0              MOVAPS XMM14,XMM8
1403819e1:  8B 86 E0 C7 00 00        MOV EAX,dword ptr [RSI + 0xc7e0]
1403819e7:  8D 0C 80                 LEA ECX,[RAX + RAX*0x4]
1403819ea:  8D 3C 8D 00 00 00 00     LEA EDI,[RCX*0x4]
1403819f1:  89 7D C4                 MOV dword ptr [RBP + -0x3c],EDI
1403819f4:  8D 5F 14                 LEA EBX,[RDI + 0x14]
1403819f7:  48 89 5D F8              MOV qword ptr [RBP + -0x8],RBX
1403819fb:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
140381a00:  49 2B 0C 24              SUB RCX,qword ptr [R12]
140381a04:  48 C1 F9 04              SAR RCX,0x4
140381a08:  8B C3                    MOV EAX,EBX
140381a0a:  48 3B C1                 CMP RAX,RCX
140381a0d:  76 11                    JBE 0x140381a20
140381a0f:  49 8B 5C 24 08           MOV RBX,qword ptr [R12 + 0x8]
140381a14:  49 2B 1C 24              SUB RBX,qword ptr [R12]
140381a18:  48 C1 FB 04              SAR RBX,0x4
140381a1c:  48 89 5D F8              MOV qword ptr [RBP + -0x8],RBX
140381a20:  44 0F B6 8E 09 C8 00 00  MOVZX R9D,byte ptr [RSI + 0xc809]
140381a28:  44 8B 86 0C C8 00 00     MOV R8D,dword ptr [RSI + 0xc80c]
140381a2f:  49 8B D4                 MOV RDX,R12
140381a32:  E8 39 A5 F2 FF           CALL 0x1402abf70
140381a37:  49 8B 44 24 08           MOV RAX,qword ptr [R12 + 0x8]
140381a3c:  49 2B 04 24              SUB RAX,qword ptr [R12]
140381a40:  48 C1 F8 04              SAR RAX,0x4
140381a44:  F3 0F 10 0D 8C 85 58 00  MOVSS XMM1,dword ptr [0x140909fd8]
140381a4c:  48 85 C0                 TEST RAX,RAX
140381a4f:  0F 84 29 05 00 00        JZ 0x140381f7e
140381a55:  49 8B CE                 MOV RCX,R14
140381a58:  F3 44 0F 10 1D 17 84 58 00 MOVSS XMM11,dword ptr [0x140909e78]
140381a61:  F3 44 0F 10 25 9A 82 58 00 MOVSS XMM12,dword ptr [0x140909d04]
140381a6a:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
140381a70:  44 3B F7                 CMP R14D,EDI
140381a73:  0F 82 DE 04 00 00        JC 0x140381f57
140381a79:  44 3B F3                 CMP R14D,EBX
140381a7c:  0F 83 D5 04 00 00        JNC 0x140381f57
140381a82:  48 8B D1                 MOV RDX,RCX
140381a85:  49 8B CC                 MOV RCX,R12
140381a88:  E8 43 D2 CA FF           CALL 0x14002ecd0
140381a8d:  4C 8B E8                 MOV R13,RAX
140381a90:  F3 0F 10 15 F0 05 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381a98:  F3 41 0F 59 D0           MULSS XMM2,XMM8
140381a9d:  41 0F 28 C2              MOVAPS XMM0,XMM10
140381aa1:  F3 0F 5C C2              SUBSS XMM0,XMM2
140381aa5:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
140381aa9:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
140381aad:  F3 0F 10 4C 24 78        MOVSS XMM1,dword ptr [RSP + 0x78]
140381ab3:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140381ab8:  F3 0F 58 CA              ADDSS XMM1,XMM2
140381abc:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
140381ac0:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
140381ac4:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
140381ac9:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
140381ace:  41 B8 06 00 00 00        MOV R8D,0x6
140381ad4:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
140381ad8:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140381adf:  E8 5C 4A 0C 00           CALL 0x140446540
140381ae4:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
140381ae9:  0F 84 AC 01 00 00        JZ 0x140381c9b
140381aef:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140381af5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381af8:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140381afd:  0F 2F C8                 COMISS XMM1,XMM0
140381b00:  0F 86 95 01 00 00        JBE 0x140381c9b
140381b06:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
140381b0c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381b0f:  0F 2F C1                 COMISS XMM0,XMM1
140381b12:  0F 86 83 01 00 00        JBE 0x140381c9b
140381b18:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140381b1d:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140381b22:  66 0F 6E C3              MOVD XMM0,EBX
140381b26:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381b29:  0F 2F C8                 COMISS XMM1,XMM0
140381b2c:  0F 86 69 01 00 00        JBE 0x140381c9b
140381b32:  66 0F 6E C7              MOVD XMM0,EDI
140381b36:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381b39:  0F 2F C1                 COMISS XMM0,XMM1
140381b3c:  0F 86 59 01 00 00        JBE 0x140381c9b
140381b42:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
140381b49:  48 8B 3B                 MOV RDI,qword ptr [RBX]
140381b4c:  48 8D 0D 7D 91 66 00     LEA RCX,[0x1409eacd0]
140381b53:  FF 15 97 AB 4E 00        CALL qword ptr [0x14086c6f0]
140381b59:  4C 8B C0                 MOV R8,RAX
140381b5c:  33 D2                    XOR EDX,EDX
140381b5e:  48 8B CB                 MOV RCX,RBX
140381b61:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140381b64:  F3 44 0F 11 7C 24 20     MOVSS dword ptr [RSP + 0x20],XMM15
140381b6b:  41 0F 28 D9              MOVAPS XMM3,XMM9
140381b6f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140381b73:  41 0F 28 C9              MOVAPS XMM1,XMM9
140381b77:  48 8D 8D 50 0A 00 00     LEA RCX,[RBP + 0xa50]
140381b7e:  FF 15 9C A4 4E 00        CALL qword ptr [0x14086c020]
140381b84:  48 8B D0                 MOV RDX,RAX
140381b87:  48 8D 0D D2 C4 63 00     LEA RCX,[0x1409be060]
140381b8e:  FF 15 04 AD 4E 00        CALL qword ptr [0x14086c898]
140381b94:  41 0F 28 C9              MOVAPS XMM1,XMM9
140381b98:  48 8D 0D C1 C4 63 00     LEA RCX,[0x1409be060]
140381b9f:  FF 15 B3 AD 4E 00        CALL qword ptr [0x14086c958]
140381ba5:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
140381baa:  F3 0F 10 15 D6 04 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381bb2:  F3 0F 59 15 6E 8C 58 00  MULSS XMM2,dword ptr [0x14090a828]
140381bba:  F3 44 0F 11 5C 24 38     MOVSS dword ptr [RSP + 0x38],XMM11
140381bc1:  F3 44 0F 11 64 24 30     MOVSS dword ptr [RSP + 0x30],XMM12
140381bc8:  F3 44 0F 11 5C 24 28     MOVSS dword ptr [RSP + 0x28],XMM11
140381bcf:  F3 44 0F 11 64 24 20     MOVSS dword ptr [RSP + 0x20],XMM12
140381bd6:  0F 28 DA                 MOVAPS XMM3,XMM2
140381bd9:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
140381bdd:  48 8D 0D 7C C4 63 00     LEA RCX,[0x1409be060]
140381be4:  FF 15 6E AC 4E 00        CALL qword ptr [0x14086c858]
140381bea:  41 0F 28 C8              MOVAPS XMM1,XMM8
140381bee:  48 8D 0D 6B C4 63 00     LEA RCX,[0x1409be060]
140381bf5:  FF 15 5D AD 4E 00        CALL qword ptr [0x14086c958]
140381bfb:  49 8B 45 00              MOV RAX,qword ptr [R13]
140381bff:  48 89 86 E8 C7 00 00     MOV qword ptr [RSI + 0xc7e8],RAX
140381c06:  80 BE 9A 03 01 00 00     CMP byte ptr [RSI + 0x1039a],0x0
140381c0d:  0F 84 88 00 00 00        JZ 0x140381c9b
140381c13:  48 8D 0D 76 2F 6D 00     LEA RCX,[0x140a54b90]
140381c1a:  FF 15 98 AC 4E 00        CALL qword ptr [0x14086c8b8]
140381c20:  84 C0                    TEST AL,AL
140381c22:  74 77                    JZ 0x140381c9b
140381c24:  48 8B 86 70 08 01 00     MOV RAX,qword ptr [RSI + 0x10870]
140381c2b:  48 3B 86 E8 C7 00 00     CMP RAX,qword ptr [RSI + 0xc7e8]
140381c32:  75 06                    JNZ 0x140381c3a
140381c34:  C6 45 A8 01              MOV byte ptr [RBP + -0x58],0x1
140381c38:  EB 61                    JMP 0x140381c9b
140381c3a:  C6 44 24 7D 01           MOV byte ptr [RSP + 0x7d],0x1
140381c3f:  0F B6 7C 24 7C           MOVZX EDI,byte ptr [RSP + 0x7c]
140381c44:  48 8D 9E D8 06 01 00     LEA RBX,[RSI + 0x106d8]
140381c4b:  40 84 FF                 TEST DIL,DIL
140381c4e:  74 25                    JZ 0x140381c75
140381c50:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
140381c57:  48 8B CB                 MOV RCX,RBX
140381c5a:  E8 31 9E ED FF           CALL 0x14025ba90
140381c5f:  4C 8B 86 E8 C7 00 00     MOV R8,qword ptr [RSI + 0xc7e8]
140381c66:  8B 55 18                 MOV EDX,dword ptr [RBP + 0x18]
140381c69:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
140381c70:  E8 EB C4 25 00           CALL 0x1405de160
140381c75:  4C 8B 86 E8 C7 00 00     MOV R8,qword ptr [RSI + 0xc7e8]
140381c7c:  8B 55 18                 MOV EDX,dword ptr [RBP + 0x18]
140381c7f:  48 8B CB                 MOV RCX,RBX
140381c82:  E8 D9 C4 25 00           CALL 0x1405de160
140381c87:  40 84 FF                 TEST DIL,DIL
140381c8a:  74 0F                    JZ 0x140381c9b
140381c8c:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
140381c93:  48 8B CB                 MOV RCX,RBX
140381c96:  E8 D5 9D ED FF           CALL 0x14025ba70
140381c9b:  49 8B 45 00              MOV RAX,qword ptr [R13]
140381c9f:  48 8B 78 48              MOV RDI,qword ptr [RAX + 0x48]
140381ca3:  F3 41 0F 10 75 0C        MOVSS XMM6,dword ptr [R13 + 0xc]
140381ca9:  83 BE D4 C7 00 00 46     CMP dword ptr [RSI + 0xc7d4],0x46
140381cb0:  75 2F                    JNZ 0x140381ce1
140381cb2:  48 3B 86 08 C4 00 00     CMP RAX,qword ptr [RSI + 0xc408]
140381cb9:  75 12                    JNZ 0x140381ccd
140381cbb:  48 8B BE A8 BD 00 00     MOV RDI,qword ptr [RSI + 0xbda8]
140381cc2:  BA 04 11 00 00           MOV EDX,0x1104
140381cc7:  41 0F 28 F0              MOVAPS XMM6,XMM8
140381ccb:  EB 17                    JMP 0x140381ce4
140381ccd:  48 3B 86 10 C4 00 00     CMP RAX,qword ptr [RSI + 0xc410]
140381cd4:  75 0B                    JNZ 0x140381ce1
140381cd6:  BA 03 11 00 00           MOV EDX,0x1103
140381cdb:  41 0F 28 F0              MOVAPS XMM6,XMM8
140381cdf:  EB 03                    JMP 0x140381ce4
140381ce1:  8B 50 40                 MOV EDX,dword ptr [RAX + 0x40]
140381ce4:  48 8D 0D A5 58 61 00     LEA RCX,[0x140997590]
140381ceb:  FF 15 9F AB 4E 00        CALL qword ptr [0x14086c890]
140381cf1:  F3 0F 10 15 8F 03 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381cf9:  0F 28 C2                 MOVAPS XMM0,XMM2
140381cfc:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140381d01:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381d05:  F3 0F 5C D8              SUBSS XMM3,XMM0
140381d09:  F3 0F 59 15 57 8C 58 00  MULSS XMM2,dword ptr [0x14090a968]
140381d11:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
140381d17:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140381d1c:  48 8D 05 D5 E4 53 00     LEA RAX,[0x1408c01f8]
140381d23:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381d28:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140381d30:  66 41 0F 7E D9           MOVD R9D,XMM3
140381d35:  66 41 0F 7E D0           MOVD R8D,XMM2
140381d3a:  49 8B D7                 MOV RDX,R15
140381d3d:  48 8D 0D 6C 52 61 00     LEA RCX,[0x140996fb0]
140381d44:  FF 15 36 AB 4E 00        CALL qword ptr [0x14086c880]
140381d4a:  48 8B 1F                 MOV RBX,qword ptr [RDI]
140381d4d:  48 8D 0D 7C 8F 66 00     LEA RCX,[0x1409eacd0]
140381d54:  FF 15 96 A9 4E 00        CALL qword ptr [0x14086c6f0]
140381d5a:  4C 8B C0                 MOV R8,RAX
140381d5d:  33 D2                    XOR EDX,EDX
140381d5f:  48 8B CF                 MOV RCX,RDI
140381d62:  FF 53 70                 CALL qword ptr [RBX + 0x70]
140381d65:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140381d6c:  41 0F 28 D9              MOVAPS XMM3,XMM9
140381d70:  41 0F 28 D1              MOVAPS XMM2,XMM9
140381d74:  41 0F 28 C9              MOVAPS XMM1,XMM9
140381d78:  48 8D 8D 60 0A 00 00     LEA RCX,[RBP + 0xa60]
140381d7f:  FF 15 9B A2 4E 00        CALL qword ptr [0x14086c020]
140381d85:  48 8B D0                 MOV RDX,RAX
140381d88:  48 8D 0D D1 C2 63 00     LEA RCX,[0x1409be060]
140381d8f:  FF 15 03 AB 4E 00        CALL qword ptr [0x14086c898]
140381d95:  F3 0F 10 0D EB 02 61 00  MOVSS XMM1,dword ptr [0x140992088]
140381d9d:  F3 0F 59 0D C3 8B 58 00  MULSS XMM1,dword ptr [0x14090a968]
140381da5:  0F 28 D1                 MOVAPS XMM2,XMM1
140381da8:  48 8D 0D B1 C2 63 00     LEA RCX,[0x1409be060]
140381daf:  FF 15 F3 AA 4E 00        CALL qword ptr [0x14086c8a8]
140381db5:  F3 0F 10 15 CB 02 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381dbd:  F3 0F 59 15 63 8A 58 00  MULSS XMM2,dword ptr [0x14090a828]
140381dc5:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140381dca:  41 0F 28 D8              MOVAPS XMM3,XMM8
140381dce:  F3 0F 10 4C 24 74        MOVSS XMM1,dword ptr [RSP + 0x74]
140381dd4:  48 8D 8D A8 04 00 00     LEA RCX,[RBP + 0x4a8]
140381ddb:  FF 15 27 A2 4E 00        CALL qword ptr [0x14086c008]
140381de1:  48 8B D0                 MOV RDX,RAX
140381de4:  48 8D 0D 75 C2 63 00     LEA RCX,[0x1409be060]
140381deb:  FF 15 AF AA 4E 00        CALL qword ptr [0x14086c8a0]
140381df1:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140381df6:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
140381dfd:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140381e04:  41 0F 28 D9              MOVAPS XMM3,XMM9
140381e08:  41 0F 28 D0              MOVAPS XMM2,XMM8
140381e0c:  41 0F 28 C8              MOVAPS XMM1,XMM8
140381e10:  48 8D 0D 49 C2 63 00     LEA RCX,[0x1409be060]
140381e17:  FF 15 43 AA 4E 00        CALL qword ptr [0x14086c860]
140381e1d:  F3 41 0F 10 55 08        MOVSS XMM2,dword ptr [R13 + 0x8]
140381e23:  0F 5A D2                 CVTPS2PD XMM2,XMM2
140381e26:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140381e2b:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140381e30:  41 B9 02 00 00 00        MOV R9D,0x2
140381e36:  48 8D 95 38 03 00 00     LEA RDX,[RBP + 0x338]
140381e3d:  48 8B CE                 MOV RCX,RSI
140381e40:  E8 1B 4E FB FF           CALL 0x140336c60
140381e45:  90                       NOP
140381e46:  48 8B 08                 MOV RCX,qword ptr [RAX]
140381e49:  F3 0F 10 15 37 02 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381e51:  0F 28 C2                 MOVAPS XMM0,XMM2
140381e54:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140381e59:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381e5d:  F3 0F 5C D8              SUBSS XMM3,XMM0
140381e61:  F3 0F 59 15 FF 8A 58 00  MULSS XMM2,dword ptr [0x14090a968]
140381e69:  F3 0F 58 D7              ADDSS XMM2,XMM7
140381e6d:  66 0F 6E 05 17 34 61 00  MOVD XMM0,dword ptr [0x14099528c]
140381e75:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381e78:  F3 0F 5C D0              SUBSS XMM2,XMM0
140381e7c:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
140381e81:  48 8D 05 68 E3 53 00     LEA RAX,[0x1408c01f0]
140381e88:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381e8d:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140381e95:  66 41 0F 7E D9           MOVD R9D,XMM3
140381e9a:  66 41 0F 7E D0           MOVD R8D,XMM2
140381e9f:  49 8B D7                 MOV RDX,R15
140381ea2:  48 8D 0D 07 51 61 00     LEA RCX,[0x140996fb0]
140381ea9:  FF 15 C9 A9 4E 00        CALL qword ptr [0x14086c878]
140381eaf:  90                       NOP
140381eb0:  48 8D 8D 38 03 00 00     LEA RCX,[RBP + 0x338]
140381eb7:  E8 04 FC C7 FF           CALL 0x140001ac0
140381ebc:  0F 57 C9                 XORPS XMM1,XMM1
140381ebf:  F3 0F 5A CE              CVTSS2SD XMM1,XMM6
140381ec3:  45 33 C9                 XOR R9D,R9D
140381ec6:  45 33 C0                 XOR R8D,R8D
140381ec9:  48 8B CE                 MOV RCX,RSI
140381ecc:  E8 4F 45 FB FF           CALL 0x140336420
140381ed1:  F3 0F 10 15 AF 01 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381ed9:  0F 28 C2                 MOVAPS XMM0,XMM2
140381edc:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140381ee1:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381ee5:  F3 0F 5C D8              SUBSS XMM3,XMM0
140381ee9:  F3 0F 59 15 77 8A 58 00  MULSS XMM2,dword ptr [0x14090a968]
140381ef1:  F3 41 0F 58 D5           ADDSS XMM2,XMM13
140381ef6:  66 0F 6E 05 8E 33 61 00  MOVD XMM0,dword ptr [0x14099528c]
140381efe:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381f01:  F3 0F 5C D0              SUBSS XMM2,XMM0
140381f05:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140381f0a:  48 8D 05 0F E5 53 00     LEA RAX,[0x1408c0420]
140381f11:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381f16:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140381f1e:  66 41 0F 7E D9           MOVD R9D,XMM3
140381f23:  66 41 0F 7E D0           MOVD R8D,XMM2
140381f28:  49 8B D7                 MOV RDX,R15
140381f2b:  48 8D 0D 7E 50 61 00     LEA RCX,[0x140996fb0]
140381f32:  FF 15 40 A9 4E 00        CALL qword ptr [0x14086c878]
140381f38:  F3 0F 10 44 24 78        MOVSS XMM0,dword ptr [RSP + 0x78]
140381f3e:  F3 0F 10 0D 92 80 58 00  MOVSS XMM1,dword ptr [0x140909fd8]
140381f46:  F3 0F 59 C1              MULSS XMM0,XMM1
140381f4a:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140381f4f:  F3 44 0F 58 F6           ADDSS XMM14,XMM6
140381f54:  8B 7D C4                 MOV EDI,dword ptr [RBP + -0x3c]
140381f57:  41 FF C6                 INC R14D
140381f5a:  49 8B 44 24 08           MOV RAX,qword ptr [R12 + 0x8]
140381f5f:  49 2B 04 24              SUB RAX,qword ptr [R12]
140381f63:  48 C1 F8 04              SAR RAX,0x4
140381f67:  41 8B CE                 MOV ECX,R14D
140381f6a:  48 3B C8                 CMP RCX,RAX
140381f6d:  48 8B 5D F8              MOV RBX,qword ptr [RBP + -0x8]
140381f71:  0F 82 F9 FA FF FF        JC 0x140381a70
140381f77:  F3 44 0F 10 5C 24 78     MOVSS XMM11,dword ptr [RSP + 0x78]
140381f7e:  41 0F 28 C3              MOVAPS XMM0,XMM11
140381f82:  F3 0F 59 C1              MULSS XMM0,XMM1
140381f86:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140381f8b:  0F 57 C9                 XORPS XMM1,XMM1
140381f8e:  F3 41 0F 5A CE           CVTSS2SD XMM1,XMM14
140381f93:  45 33 C9                 XOR R9D,R9D
140381f96:  45 33 C0                 XOR R8D,R8D
140381f99:  48 8B CE                 MOV RCX,RSI
140381f9c:  E8 7F 44 FB FF           CALL 0x140336420
140381fa1:  F3 0F 10 15 DF 00 61 00  MOVSS XMM2,dword ptr [0x140992088]
140381fa9:  0F 28 C2                 MOVAPS XMM0,XMM2
140381fac:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140381fb1:  41 0F 28 DA              MOVAPS XMM3,XMM10
140381fb5:  F3 0F 5C D8              SUBSS XMM3,XMM0
140381fb9:  F3 0F 59 15 A7 89 58 00  MULSS XMM2,dword ptr [0x14090a968]
140381fc1:  F3 41 0F 58 D5           ADDSS XMM2,XMM13
140381fc6:  66 0F 6E 05 BE 32 61 00  MOVD XMM0,dword ptr [0x14099528c]
140381fce:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140381fd1:  F3 0F 5C D0              SUBSS XMM2,XMM0
140381fd5:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140381fda:  48 8D 05 37 E4 53 00     LEA RAX,[0x1408c0418]
140381fe1:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140381fe6:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140381fee:  66 41 0F 7E D9           MOVD R9D,XMM3
140381ff3:  66 41 0F 7E D0           MOVD R8D,XMM2
140381ff8:  49 8B D7                 MOV RDX,R15
140381ffb:  48 8D 0D AE 4F 61 00     LEA RCX,[0x140996fb0]
140382002:  FF 15 70 A8 4E 00        CALL qword ptr [0x14086c878]
140382008:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14038200d:  49 2B 0C 24              SUB RCX,qword ptr [R12]
140382011:  48 C1 F9 04              SAR RCX,0x4
140382015:  48 B8 CD CC CC CC CC CC CC CC MOV RAX,-0x3333333333333333
14038201f:  48 F7 E1                 MUL RCX
140382022:  48 C1 EA 04              SHR RDX,0x4
140382026:  F3 0F 10 0D 5A 00 61 00  MOVSS XMM1,dword ptr [0x140992088]
14038202e:  0F 28 D9                 MOVAPS XMM3,XMM1
140382031:  F3 0F 59 1D B7 89 58 00  MULSS XMM3,dword ptr [0x14090a9f0]
140382039:  F3 0F 59 8E 54 AE 00 00  MULSS XMM1,dword ptr [RSI + 0xae54]
140382041:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140382046:  48 8D 86 E0 C7 00 00     LEA RAX,[RSI + 0xc7e0]
14038204d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140382052:  89 54 24 20              MOV dword ptr [RSP + 0x20],EDX
140382056:  41 0F 28 D2              MOVAPS XMM2,XMM10
14038205a:  48 8B CE                 MOV RCX,RSI
14038205d:  E8 7E A0 13 00           CALL 0x1404bc0e0
140382062:  F3 45 0F 58 DB           ADDSS XMM11,XMM11
140382067:  F3 45 0F 58 D3           ADDSS XMM10,XMM11
14038206c:  E9 D5 03 00 00           JMP 0x140382446
140382071:  41 BC F5 92 00 00        MOV R12D,0x92f5
140382077:  F3 44 0F 10 1D F8 7D 58 00 MOVSS XMM11,dword ptr [0x140909e78]
140382080:  F3 44 0F 10 25 7B 7C 58 00 MOVSS XMM12,dword ptr [0x140909d04]
140382089:  F3 44 0F 10 35 46 7F 58 00 MOVSS XMM14,dword ptr [0x140909fd8]
140382092:  41 8B D6                 MOV EDX,R14D
140382095:  83 BE D4 C7 00 00 09     CMP dword ptr [RSI + 0xc7d4],0x9
14038209c:  48 8D 8D B8 01 00 00     LEA RCX,[RBP + 0x1b8]
1403820a3:  74 07                    JZ 0x1403820ac
1403820a5:  48 8D 8D C8 01 00 00     LEA RCX,[RBP + 0x1c8]
1403820ac:  E8 8F 8B 0C 00           CALL 0x14044ac40
1403820b1:  F3 0F 10 38              MOVSS XMM7,dword ptr [RAX]
1403820b5:  41 0F 2E F8              UCOMISS XMM7,XMM8
1403820b9:  7A 06                    JP 0x1403820c1
1403820bb:  0F 84 78 03 00 00        JZ 0x140382439
1403820c1:  F3 0F 10 15 BF FF 60 00  MOVSS XMM2,dword ptr [0x140992088]
1403820c9:  F3 41 0F 59 D0           MULSS XMM2,XMM8
1403820ce:  41 0F 28 C2              MOVAPS XMM0,XMM10
1403820d2:  F3 0F 5C C2              SUBSS XMM0,XMM2
1403820d6:  F3 0F 2C D8              CVTTSS2SI EBX,XMM0
1403820da:  89 5C 24 64              MOV dword ptr [RSP + 0x64],EBX
1403820de:  F3 0F 10 4C 24 78        MOVSS XMM1,dword ptr [RSP + 0x78]
1403820e4:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
1403820e9:  F3 0F 58 CA              ADDSS XMM1,XMM2
1403820ed:  F3 0F 2C F9              CVTTSS2SI EDI,XMM1
1403820f1:  89 7C 24 6C              MOV dword ptr [RSP + 0x6c],EDI
1403820f5:  0F 28 74 24 60           MOVAPS XMM6,xmmword ptr [RSP + 0x60]
1403820fa:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
1403820ff:  41 B8 06 00 00 00        MOV R8D,0x6
140382105:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
140382109:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
140382110:  E8 2B 44 0C 00           CALL 0x140446540
140382115:  80 7C 24 40 00           CMP byte ptr [RSP + 0x40],0x0
14038211a:  0F 84 FC 01 00 00        JZ 0x14038231c
140382120:  66 0F 6E 44 24 60        MOVD XMM0,dword ptr [RSP + 0x60]
140382126:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140382129:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14038212e:  0F 2F C8                 COMISS XMM1,XMM0
140382131:  0F 86 E5 01 00 00        JBE 0x14038231c
140382137:  66 0F 6E 44 24 68        MOVD XMM0,dword ptr [RSP + 0x68]
14038213d:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140382140:  0F 2F C1                 COMISS XMM0,XMM1
140382143:  0F 86 D3 01 00 00        JBE 0x14038231c
140382149:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
14038214e:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140382153:  66 0F 6E C3              MOVD XMM0,EBX
140382157:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14038215a:  0F 2F C8                 COMISS XMM1,XMM0
14038215d:  0F 86 B9 01 00 00        JBE 0x14038231c
140382163:  66 0F 6E C7              MOVD XMM0,EDI
140382167:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14038216a:  0F 2F C1                 COMISS XMM0,XMM1
14038216d:  0F 86 A9 01 00 00        JBE 0x14038231c
140382173:  48 8B 9E 28 B0 00 00     MOV RBX,qword ptr [RSI + 0xb028]
14038217a:  48 8B 3B                 MOV RDI,qword ptr [RBX]
14038217d:  48 8D 0D 4C 8B 66 00     LEA RCX,[0x1409eacd0]
140382184:  FF 15 66 A5 4E 00        CALL qword ptr [0x14086c6f0]
14038218a:  4C 8B C0                 MOV R8,RAX
14038218d:  33 D2                    XOR EDX,EDX
14038218f:  48 8B CB                 MOV RCX,RBX
140382192:  FF 57 70                 CALL qword ptr [RDI + 0x70]
140382195:  F3 44 0F 11 7C 24 20     MOVSS dword ptr [RSP + 0x20],XMM15
14038219c:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403821a0:  41 0F 28 D1              MOVAPS XMM2,XMM9
1403821a4:  41 0F 28 C9              MOVAPS XMM1,XMM9
1403821a8:  48 8D 8D 70 0A 00 00     LEA RCX,[RBP + 0xa70]
1403821af:  FF 15 6B 9E 4E 00        CALL qword ptr [0x14086c020]
1403821b5:  48 8B D0                 MOV RDX,RAX
1403821b8:  48 8D 0D A1 BE 63 00     LEA RCX,[0x1409be060]
1403821bf:  FF 15 D3 A6 4E 00        CALL qword ptr [0x14086c898]
1403821c5:  66 0F 7F 75 E0           MOVDQA xmmword ptr [RBP + -0x20],XMM6
1403821ca:  F3 0F 10 15 B6 FE 60 00  MOVSS XMM2,dword ptr [0x140992088]
1403821d2:  F3 0F 59 15 4E 86 58 00  MULSS XMM2,dword ptr [0x14090a828]
1403821da:  F3 44 0F 11 5C 24 38     MOVSS dword ptr [RSP + 0x38],XMM11
1403821e1:  F3 44 0F 11 64 24 30     MOVSS dword ptr [RSP + 0x30],XMM12
1403821e8:  F3 44 0F 11 5C 24 28     MOVSS dword ptr [RSP + 0x28],XMM11
1403821ef:  F3 44 0F 11 64 24 20     MOVSS dword ptr [RSP + 0x20],XMM12
1403821f6:  0F 28 DA                 MOVAPS XMM3,XMM2
1403821f9:  48 8D 55 E0              LEA RDX,[RBP + -0x20]
1403821fd:  48 8D 0D 5C BE 63 00     LEA RCX,[0x1409be060]
140382204:  FF 15 4E A6 4E 00        CALL qword ptr [0x14086c858]
14038220a:  8B 45 AC                 MOV EAX,dword ptr [RBP + -0x54]
14038220d:  0F B6 C0                 MOVZX EAX,AL
140382210:  45 3B EE                 CMP R13D,R14D
140382213:  B9 01 00 00 00           MOV ECX,0x1
140382218:  0F 44 C1                 CMOVZ EAX,ECX
14038221b:  89 45 AC                 MOV dword ptr [RBP + -0x54],EAX
14038221e:  41 83 C8 FF              OR R8D,0xffffffff
140382222:  83 BE D4 C7 00 00 09     CMP dword ptr [RSI + 0xc7d4],0x9
140382229:  75 25                    JNZ 0x140382250
14038222b:  48 8D 15 C6 E0 53 00     LEA RDX,[0x1408c02f8]
140382232:  48 8D 4D 68              LEA RCX,[RBP + 0x68]
140382236:  E8 75 8B ED FF           CALL 0x14025adb0
14038223b:  90                       NOP
14038223c:  48 8D 55 68              LEA RDX,[RBP + 0x68]
140382240:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140382244:  E8 A7 8B 0C 00           CALL 0x14044adf0
140382249:  90                       NOP
14038224a:  48 8D 4D 68              LEA RCX,[RBP + 0x68]
14038224e:  EB 23                    JMP 0x140382273
140382250:  48 8D 15 B1 DF 53 00     LEA RDX,[0x1408c0208]
140382257:  48 8D 4D F8              LEA RCX,[RBP + -0x8]
14038225b:  E8 50 8B ED FF           CALL 0x14025adb0
140382260:  90                       NOP
140382261:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
140382265:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140382269:  E8 82 8B 0C 00           CALL 0x14044adf0
14038226e:  90                       NOP
14038226f:  48 8D 4D F8              LEA RCX,[RBP + -0x8]
140382273:  E8 48 F8 C7 FF           CALL 0x140001ac0
140382278:  80 BE 9A 03 01 00 00     CMP byte ptr [RSI + 0x1039a],0x0
14038227f:  0F 84 90 00 00 00        JZ 0x140382315
140382285:  48 8D 0D 04 29 6D 00     LEA RCX,[0x140a54b90]
14038228c:  FF 15 26 A6 4E 00        CALL qword ptr [0x14086c8b8]
140382292:  84 C0                    TEST AL,AL
140382294:  0F 84 82 00 00 00        JZ 0x14038231c
14038229a:  44 3B B6 DC C7 00 00     CMP R14D,dword ptr [RSI + 0xc7dc]
1403822a1:  75 10                    JNZ 0x1403822b3
1403822a3:  C6 45 A8 01              MOV byte ptr [RBP + -0x58],0x1
1403822a7:  C7 86 DC C7 00 00 FF FF FF FF MOV dword ptr [RSI + 0xc7dc],0xffffffff
1403822b1:  EB 69                    JMP 0x14038231c
1403822b3:  C6 44 24 7D 01           MOV byte ptr [RSP + 0x7d],0x1
1403822b8:  44 89 B6 DC C7 00 00     MOV dword ptr [RSI + 0xc7dc],R14D
1403822bf:  0F B6 7C 24 7C           MOVZX EDI,byte ptr [RSP + 0x7c]
1403822c4:  48 8D 9E D8 06 01 00     LEA RBX,[RSI + 0x106d8]
1403822cb:  40 84 FF                 TEST DIL,DIL
1403822ce:  74 21                    JZ 0x1403822f1
1403822d0:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
1403822d7:  48 8B CB                 MOV RCX,RBX
1403822da:  E8 B1 97 ED FF           CALL 0x14025ba90
1403822df:  8B 96 DC C7 00 00        MOV EDX,dword ptr [RSI + 0xc7dc]
1403822e5:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
1403822ec:  E8 AF 03 26 00           CALL 0x1405e26a0
1403822f1:  8B 96 DC C7 00 00        MOV EDX,dword ptr [RSI + 0xc7dc]
1403822f7:  48 8B CB                 MOV RCX,RBX
1403822fa:  E8 A1 03 26 00           CALL 0x1405e26a0
1403822ff:  40 84 FF                 TEST DIL,DIL
140382302:  74 18                    JZ 0x14038231c
140382304:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
14038230b:  48 8B CB                 MOV RCX,RBX
14038230e:  E8 5D 97 ED FF           CALL 0x14025ba70
140382313:  EB 07                    JMP 0x14038231c
140382315:  44 89 B6 DC C7 00 00     MOV dword ptr [RSI + 0xc7dc],R14D
14038231c:  41 8D 96 B5 92 00 00     LEA EDX,[R14 + 0x92b5]
140382323:  81 FA C8 92 00 00        CMP EDX,0x92c8
140382329:  75 07                    JNZ 0x140382332
14038232b:  BA F4 92 00 00           MOV EDX,0x92f4
140382330:  EB 19                    JMP 0x14038234b
140382332:  81 FA C9 92 00 00        CMP EDX,0x92c9
140382338:  75 07                    JNZ 0x140382341
14038233a:  BA F6 92 00 00           MOV EDX,0x92f6
14038233f:  EB 0A                    JMP 0x14038234b
140382341:  81 FA CA 92 00 00        CMP EDX,0x92ca
140382347:  41 0F 44 D4              CMOVZ EDX,R12D
14038234b:  48 8D 0D 3E 52 61 00     LEA RCX,[0x140997590]
140382352:  FF 15 38 A5 4E 00        CALL qword ptr [0x14086c890]
140382358:  F3 0F 10 15 28 FD 60 00  MOVSS XMM2,dword ptr [0x140992088]
140382360:  0F 28 C2                 MOVAPS XMM0,XMM2
140382363:  F3 41 0F 59 C0           MULSS XMM0,XMM8
140382368:  41 0F 28 DA              MOVAPS XMM3,XMM10
14038236c:  F3 0F 5C D8              SUBSS XMM3,XMM0
140382370:  F3 0F 59 15 F0 85 58 00  MULSS XMM2,dword ptr [0x14090a968]
140382378:  F3 0F 58 54 24 74        ADDSS XMM2,dword ptr [RSP + 0x74]
14038237e:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140382383:  48 8D 05 46 E2 53 00     LEA RAX,[0x1408c05d0]
14038238a:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14038238f:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140382397:  66 41 0F 7E D9           MOVD R9D,XMM3
14038239c:  66 41 0F 7E D0           MOVD R8D,XMM2
1403823a1:  49 8B D7                 MOV RDX,R15
1403823a4:  48 8D 0D 05 4C 61 00     LEA RCX,[0x140996fb0]
1403823ab:  FF 15 CF A4 4E 00        CALL qword ptr [0x14086c880]
1403823b1:  0F 5A CF                 CVTPS2PD XMM1,XMM7
1403823b4:  45 33 C9                 XOR R9D,R9D
1403823b7:  45 33 C0                 XOR R8D,R8D
1403823ba:  48 8B CE                 MOV RCX,RSI
1403823bd:  E8 5E 40 FB FF           CALL 0x140336420
1403823c2:  F3 0F 10 15 BE FC 60 00  MOVSS XMM2,dword ptr [0x140992088]
1403823ca:  0F 28 C2                 MOVAPS XMM0,XMM2
1403823cd:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1403823d2:  41 0F 28 DA              MOVAPS XMM3,XMM10
1403823d6:  F3 0F 5C D8              SUBSS XMM3,XMM0
1403823da:  F3 0F 59 15 86 85 58 00  MULSS XMM2,dword ptr [0x14090a968]
1403823e2:  F3 41 0F 58 D5           ADDSS XMM2,XMM13
1403823e7:  66 0F 6E 05 9D 2E 61 00  MOVD XMM0,dword ptr [0x14099528c]
1403823ef:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1403823f2:  F3 0F 5C D0              SUBSS XMM2,XMM0
1403823f6:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1403823fb:  48 8D 05 22 C5 53 00     LEA RAX,[0x1408be924]
140382402:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140382407:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
14038240f:  66 41 0F 7E D9           MOVD R9D,XMM3
140382414:  66 41 0F 7E D0           MOVD R8D,XMM2
140382419:  49 8B D7                 MOV RDX,R15
14038241c:  48 8D 0D 8D 4B 61 00     LEA RCX,[0x140996fb0]
140382423:  FF 15 4F A4 4E 00        CALL qword ptr [0x14086c878]
140382429:  F3 0F 10 44 24 78        MOVSS XMM0,dword ptr [RSP + 0x78]
14038242f:  F3 41 0F 59 C6           MULSS XMM0,XMM14
140382434:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140382439:  41 FF C6                 INC R14D
14038243c:  41 83 FE 16              CMP R14D,0x16
140382440:  0F 8C 4C FC FF FF        JL 0x140382092
140382446:  F3 0F 10 35 3A FC 60 00  MOVSS XMM6,dword ptr [0x140992088]
14038244e:  0F 28 C6                 MOVAPS XMM0,XMM6
140382451:  F3 0F 59 05 CF 83 58 00  MULSS XMM0,dword ptr [0x14090a828]
140382459:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
14038245e:  48 83 7C 24 50 00        CMP qword ptr [RSP + 0x50],0x0
140382464:  74 4B                    JZ 0x1403824b1
140382466:  41 B4 01                 MOV R12B,0x1
140382469:  41 83 C8 FF              OR R8D,0xffffffff
14038246d:  48 8D 15 4C E1 53 00     LEA RDX,[0x1408c05c0]
140382474:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140382479:  E8 32 89 ED FF           CALL 0x14025adb0
14038247e:  90                       NOP
14038247f:  48 8D 54 24 60           LEA RDX,[RSP + 0x60]
140382484:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
140382488:  E8 63 89 0C 00           CALL 0x14044adf0
14038248d:  90                       NOP
14038248e:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140382493:  E8 28 F6 C7 FF           CALL 0x140001ac0
140382498:  F3 0F 10 35 E8 FB 60 00  MOVSS XMM6,dword ptr [0x140992088]
1403824a0:  EB 13                    JMP 0x1403824b5
1403824a2:  F3 44 0F 10 0D 7D 85 58 00 MOVSS XMM9,dword ptr [0x14090aa28]
1403824ab:  44 8B 65 AC              MOV R12D,dword ptr [RBP + -0x54]
1403824af:  EB 12                    JMP 0x1403824c3
1403824b1:  44 8B 65 AC              MOV R12D,dword ptr [RBP + -0x54]
1403824b5:  F3 44 0F 10 0D 6A 85 58 00 MOVSS XMM9,dword ptr [0x14090aa28]
1403824be:  0F B6 5C 24 7C           MOVZX EBX,byte ptr [RSP + 0x7c]
1403824c3:  0F B6 45 B9              MOVZX EAX,byte ptr [RBP + -0x47]
1403824c7:  3C 01                    CMP AL,0x1
1403824c9:  75 0C                    JNZ 0x1403824d7
1403824cb:  C7 86 D4 C7 00 00 FF FF FF FF MOV dword ptr [RSI + 0xc7d4],0xffffffff
1403824d5:  EB 14                    JMP 0x1403824eb
1403824d7:  84 C0                    TEST AL,AL
1403824d9:  75 10                    JNZ 0x1403824eb
1403824db:  38 45 A8                 CMP byte ptr [RBP + -0x58],AL
1403824de:  75 0B                    JNZ 0x1403824eb
1403824e0:  8B 45 D0                 MOV EAX,dword ptr [RBP + -0x30]
1403824e3:  3B 86 D4 C7 00 00        CMP EAX,dword ptr [RSI + 0xc7d4]
1403824e9:  74 0A                    JZ 0x1403824f5
1403824eb:  C6 86 9A 03 01 00 00     MOV byte ptr [RSI + 0x1039a],0x0
1403824f2:  45 32 E4                 XOR R12B,R12B
1403824f5:  44 0F 28 C6              MOVAPS XMM8,XMM6
1403824f9:  F3 44 0F 59 05 26 83 58 00 MULSS XMM8,dword ptr [0x14090a828]
140382502:  0F 28 C6                 MOVAPS XMM0,XMM6
140382505:  F3 0F 59 05 3F 87 58 00  MULSS XMM0,dword ptr [0x14090ac4c]
14038250d:  0F 5A F8                 CVTPS2PD XMM7,XMM0
140382510:  0F 28 CE                 MOVAPS XMM1,XMM6
140382513:  F3 0F 59 0D B9 8A 58 00  MULSS XMM1,dword ptr [0x14090afd4]
14038251b:  0F 5A C1                 CVTPS2PD XMM0,XMM1
14038251e:  F2 0F 11 44 24 60        MOVSD qword ptr [RSP + 0x60],XMM0
140382524:  F2 0F 11 7C 24 68        MOVSD qword ptr [RSP + 0x68],XMM7
14038252a:  66 0F 6E 0D 42 2D 61 00  MOVD XMM1,dword ptr [0x140995274]
140382532:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140382535:  F3 41 0F 5C CA           SUBSS XMM1,XMM10
14038253a:  0F 28 C6                 MOVAPS XMM0,XMM6
14038253d:  F3 0F 59 05 73 85 58 00  MULSS XMM0,dword ptr [0x14090aab8]
140382545:  F3 0F 5C C8              SUBSS XMM1,XMM0
140382549:  F3 0F 59 35 CB 88 58 00  MULSS XMM6,dword ptr [0x14090ae1c]
140382551:  0F 2F CE                 COMISS XMM1,XMM6
140382554:  76 03                    JBE 0x140382559
140382556:  0F 28 CE                 MOVAPS XMM1,XMM6
140382559:  0F 5A C1                 CVTPS2PD XMM0,XMM1
14038255c:  66 0F 2F C7              COMISD XMM0,XMM7
140382560:  76 09                    JBE 0x14038256b
140382562:  0F 5A F9                 CVTPS2PD XMM7,XMM1
140382565:  F2 0F 11 7C 24 68        MOVSD qword ptr [RSP + 0x68],XMM7
14038256b:  F3 45 0F 58 D0           ADDSS XMM10,XMM8
140382570:  F3 44 0F 11 55 D0        MOVSS dword ptr [RBP + -0x30],XMM10
140382576:  0F B6 44 24 7D           MOVZX EAX,byte ptr [RSP + 0x7d]
14038257b:  84 C0                    TEST AL,AL
14038257d:  75 10                    JNZ 0x14038258f
14038257f:  38 86 9A 03 01 00        CMP byte ptr [RSI + 0x1039a],AL
140382585:  74 04                    JZ 0x14038258b
140382587:  84 DB                    TEST BL,BL
140382589:  75 04                    JNZ 0x14038258f
14038258b:  33 C0                    XOR EAX,EAX
14038258d:  EB 02                    JMP 0x140382591
14038258f:  B0 01                    MOV AL,0x1
140382591:  48 8D 4C 24 60           LEA RCX,[RSP + 0x60]
140382596:  48 89 4C 24 30           MOV qword ptr [RSP + 0x30],RCX
14038259b:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
14038259f:  48 89 4C 24 28           MOV qword ptr [RSP + 0x28],RCX
1403825a4:  88 44 24 20              MOV byte ptr [RSP + 0x20],AL
1403825a8:  45 0F B6 CC              MOVZX R9D,R12B
1403825ac:  4C 8D 45 D0              LEA R8,[RBP + -0x30]
1403825b0:  48 8D 55 B4              LEA RDX,[RBP + -0x4c]
1403825b4:  48 8B CE                 MOV RCX,RSI
1403825b7:  E8 14 97 49 00           CALL 0x14081bcd0
1403825bc:  41 0F 5A C0              CVTPS2PD XMM0,XMM8
1403825c0:  F2 0F 58 F8              ADDSD XMM7,XMM0
1403825c4:  F3 0F 10 1D BC FA 60 00  MOVSS XMM3,dword ptr [0x140992088]
1403825cc:  0F 28 CB                 MOVAPS XMM1,XMM3
1403825cf:  F3 41 0F 59 C9           MULSS XMM1,XMM9
1403825d4:  0F 5A D1                 CVTPS2PD XMM2,XMM1
1403825d7:  F2 0F 58 D7              ADDSD XMM2,XMM7
1403825db:  0F 57 C0                 XORPS XMM0,XMM0
1403825de:  F3 41 0F 5A C2           CVTSS2SD XMM0,XMM10
1403825e3:  F2 0F 58 D0              ADDSD XMM2,XMM0
1403825e7:  66 0F 5A F2              CVTPD2PS XMM6,XMM2
1403825eb:  F3 0F 10 4D B4           MOVSS XMM1,dword ptr [RBP + -0x4c]
1403825f0:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
1403825f4:  F3 0F 11 08              MOVSS dword ptr [RAX],XMM1
1403825f8:  48 8B 45 28              MOV RAX,qword ptr [RBP + 0x28]
1403825fc:  F3 0F 11 30              MOVSS dword ptr [RAX],XMM6
140382600:  F3 0F 59 1D 70 84 58 00  MULSS XMM3,dword ptr [0x14090aa78]
140382608:  0F 28 D6                 MOVAPS XMM2,XMM6
14038260b:  F3 0F 5C D3              SUBSS XMM2,XMM3
14038260f:  41 B9 06 00 00 00        MOV R9D,0x6
140382615:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
14038261c:  E8 CF 3B 0C 00           CALL 0x1404461f0
140382621:  F3 0F 10 05 5F FA 60 00  MOVSS XMM0,dword ptr [0x140992088]
140382629:  0F 28 C8                 MOVAPS XMM1,XMM0
14038262c:  F3 0F 59 0D F4 82 58 00  MULSS XMM1,dword ptr [0x14090a928]
140382634:  F3 0F 58 CE              ADDSS XMM1,XMM6
140382638:  F3 0F 59 05 EC 84 58 00  MULSS XMM0,dword ptr [0x14090ab2c]
140382640:  F3 0F 5C C8              SUBSS XMM1,XMM0
140382644:  F3 0F 11 8E 58 AE 00 00  MOVSS dword ptr [RSI + 0xae58],XMM1
14038264c:  80 BE E6 05 00 00 00     CMP byte ptr [RSI + 0x5e6],0x0
140382653:  74 5D                    JZ 0x1403826b2
140382655:  48 8D 0D 04 BA 63 00     LEA RCX,[0x1409be060]
14038265c:  FF 15 96 A2 4E 00        CALL qword ptr [0x14086c8f8]
140382662:  48 8D 15 6B C0 53 00     LEA RDX,[0x1408be6d4]
140382669:  48 8D 0D 40 49 61 00     LEA RCX,[0x140996fb0]
140382670:  FF 15 FA A1 4E 00        CALL qword ptr [0x14086c870]
140382676:  45 33 C0                 XOR R8D,R8D
140382679:  33 D2                    XOR EDX,EDX
14038267b:  48 8D 0D DE B9 63 00     LEA RCX,[0x1409be060]
140382682:  FF 15 90 A2 4E 00        CALL qword ptr [0x14086c918]
140382688:  48 8D 86 18 0F 01 00     LEA RAX,[RSI + 0x10f18]
14038268f:  48 39 86 F0 05 00 00     CMP qword ptr [RSI + 0x5f0],RAX
140382696:  41 0F 94 C1              SETZ R9B
14038269a:  F3 0F 10 96 EC 05 00 00  MOVSS XMM2,dword ptr [RSI + 0x5ec]
1403826a2:  F3 0F 10 8E E8 05 00 00  MOVSS XMM1,dword ptr [RSI + 0x5e8]
1403826aa:  48 8B CE                 MOV RCX,RSI
1403826ad:  E8 0E 36 FB FF           CALL 0x140335cc0
1403826b2:  84 DB                    TEST BL,BL
1403826b4:  74 14                    JZ 0x1403826ca
1403826b6:  48 8D 95 10 05 00 00     LEA RDX,[RBP + 0x510]
1403826bd:  48 8D 8E D8 06 01 00     LEA RCX,[RSI + 0x106d8]
1403826c4:  E8 C7 93 ED FF           CALL 0x14025ba90
1403826c9:  90                       NOP
1403826ca:  48 8D 8D 90 00 00 00     LEA RCX,[RBP + 0x90]
1403826d1:  E8 CA D0 D7 FF           CALL 0x1400ff7a0
1403826d6:  90                       NOP
1403826d7:  48 8D 8D 10 05 00 00     LEA RCX,[RBP + 0x510]
1403826de:  E8 5D D1 D7 FF           CALL 0x1400ff840
1403826e3:  90                       NOP
1403826e4:  48 8D 4D 90              LEA RCX,[RBP + -0x70]
1403826e8:  E8 F3 4A D8 FF           CALL 0x1401071e0
1403826ed:  90                       NOP
1403826ee:  48 8D 4D 50              LEA RCX,[RBP + 0x50]
1403826f2:  E8 D9 D7 C8 FF           CALL 0x14000fed0
1403826f7:  48 8B 8D 30 0D 00 00     MOV RCX,qword ptr [RBP + 0xd30]
1403826fe:  48 33 CC                 XOR RCX,RSP
140382701:  E8 9A B7 4C 00           CALL 0x14084dea0
140382706:  4C 8D 9C 24 E0 0E 00 00  LEA R11,[RSP + 0xee0]
14038270e:  49 8B 5B 58              MOV RBX,qword ptr [R11 + 0x58]
140382712:  41 0F 28 73 F0           MOVAPS XMM6,xmmword ptr [R11 + -0x10]
140382717:  41 0F 28 7B E0           MOVAPS XMM7,xmmword ptr [R11 + -0x20]
14038271c:  45 0F 28 43 D0           MOVAPS XMM8,xmmword ptr [R11 + -0x30]
140382721:  45 0F 28 4B C0           MOVAPS XMM9,xmmword ptr [R11 + -0x40]
140382726:  45 0F 28 53 B0           MOVAPS XMM10,xmmword ptr [R11 + -0x50]
14038272b:  45 0F 28 5B A0           MOVAPS XMM11,xmmword ptr [R11 + -0x60]
140382730:  45 0F 28 63 90           MOVAPS XMM12,xmmword ptr [R11 + -0x70]
140382735:  45 0F 28 6B 80           MOVAPS XMM13,xmmword ptr [R11 + -0x80]
14038273a:  45 0F 28 B3 70 FF FF FF  MOVAPS XMM14,xmmword ptr [R11 + -0x90]
140382742:  45 0F 28 BB 60 FF FF FF  MOVAPS XMM15,xmmword ptr [R11 + -0xa0]
14038274a:  49 8B E3                 MOV RSP,R11
14038274d:  41 5F                    POP R15
14038274f:  41 5E                    POP R14
140382751:  41 5D                    POP R13
140382753:  41 5C                    POP R12
140382755:  5F                       POP RDI
140382756:  5E                       POP RSI
140382757:  5D                       POP RBP
140382758:  C3                       RET
140382760:  48 8B C4                 MOV RAX,RSP
140382763:  48 89 58 10              MOV qword ptr [RAX + 0x10],RBX
140382767:  55                       PUSH RBP
140382768:  56                       PUSH RSI
140382769:  57                       PUSH RDI
14038276a:  41 54                    PUSH R12
14038276c:  41 55                    PUSH R13
14038276e:  41 56                    PUSH R14
140382770:  41 57                    PUSH R15
140382772:  48 8D 68 C1              LEA RBP,[RAX + -0x3f]
140382776:  48 81 EC D0 00 00 00     SUB RSP,0xd0
14038277d:  0F 29 70 B8              MOVAPS xmmword ptr [RAX + -0x48],XMM6
140382781:  4D 8B F8                 MOV R15,R8
140382784:  0F 29 78 A8              MOVAPS xmmword ptr [RAX + -0x58],XMM7
140382788:  4C 8B F2                 MOV R14,RDX
14038278b:  44 0F 29 50 98           MOVAPS xmmword ptr [RAX + -0x68],XMM10
140382790:  48 8B F9                 MOV RDI,RCX
140382793:  44 0F 29 58 88           MOVAPS xmmword ptr [RAX + -0x78],XMM11
140382798:  44 0F 29 A0 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM12
1403827a0:  48 83 C8 FF              OR RAX,-0x1
1403827a4:  44 0F 29 6C 24 70        MOVAPS xmmword ptr [RSP + 0x70],XMM13
1403827aa:  44 0F 28 EB              MOVAPS XMM13,XMM3
1403827ae:  44 0F 29 74 24 60        MOVAPS xmmword ptr [RSP + 0x60],XMM14
1403827b4:  C6 45 5F 00              MOV byte ptr [RBP + 0x5f],0x0
1403827b8:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
1403827c0:  48 FF C0                 INC RAX
1403827c3:  80 BC 10 B4 00 00 00 00  CMP byte ptr [RAX + RDX*0x1 + 0xb4],0x0
1403827cb:  75 F3                    JNZ 0x1403827c0
1403827cd:  45 33 E4                 XOR R12D,R12D
1403827d0:  48 85 C0                 TEST RAX,RAX
1403827d3:  74 3E                    JZ 0x140382813
1403827d5:  4C 39 62 58              CMP qword ptr [RDX + 0x58],R12
1403827d9:  75 38                    JNZ 0x140382813
1403827db:  48 81 C2 B4 00 00 00     ADD RDX,0xb4
1403827e2:  48 8D 0D E7 84 66 00     LEA RCX,[0x1409eacd0]
1403827e9:  FF 15 D1 9C 4E 00        CALL qword ptr [0x14086c4c0]
1403827ef:  44 89 64 24 28           MOV dword ptr [RSP + 0x28],R12D
1403827f4:  49 8D 96 B4 00 00 00     LEA RDX,[R14 + 0xb4]
1403827fb:  48 8B C8                 MOV RCX,RAX
1403827fe:  49 89 46 58              MOV qword ptr [R14 + 0x58],RAX
140382802:  45 33 C9                 XOR R9D,R9D
140382805:  44 89 64 24 20           MOV dword ptr [RSP + 0x20],R12D
14038280a:  48 8B 00                 MOV RAX,qword ptr [RAX]
14038280d:  45 33 C0                 XOR R8D,R8D
140382810:  FF 50 10                 CALL qword ptr [RAX + 0x10]
140382813:  4C 8B 05 EE 84 66 00     MOV R8,qword ptr [0x1409ead08]
14038281a:  33 D2                    XOR EDX,EDX
14038281c:  45 38 A6 B4 02 00 00     CMP byte ptr [R14 + 0x2b4],R12B
140382823:  74 09                    JZ 0x14038282e
140382825:  48 8B 8F 28 B0 00 00     MOV RCX,qword ptr [RDI + 0xb028]
14038282c:  EB 04                    JMP 0x140382832
14038282e:  49 8B 4E 58              MOV RCX,qword ptr [R14 + 0x58]
140382832:  48 8B 01                 MOV RAX,qword ptr [RCX]
140382835:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140382838:  F3 44 0F 10 65 67        MOVSS XMM12,dword ptr [RBP + 0x67]
14038283e:  41 0F 28 C5              MOVAPS XMM0,XMM13
140382842:  41 0F 14 C4              UNPCKLPS XMM0,XMM12
140382846:  41 8B C4                 MOV EAX,R12D
140382849:  F2 0F 11 05 9F BA 63 00  MOVSD qword ptr [0x1409be2f0],XMM0
140382851:  45 0F 57 DB              XORPS XMM11,XMM11
140382855:  0F 28 05 A4 98 58 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
14038285c:  0F 11 05 A9 BA 63 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140382863:  89 05 8F BA 63 00        MOV dword ptr [0x1409be2f8],EAX
140382869:  83 BF F0 AD 00 00 04     CMP dword ptr [RDI + 0xadf0],0x4
140382870:  44 89 65 8F              MOV dword ptr [RBP + -0x71],R12D
140382874:  74 1B                    JZ 0x140382891
140382876:  44 8B 8F A8 AD 00 00     MOV R9D,dword ptr [RDI + 0xada8]
14038287d:  48 8D 8F B0 AD 00 00     LEA RCX,[RDI + 0xadb0]
140382884:  41 0F 28 D4              MOVAPS XMM2,XMM12
140382888:  41 0F 28 CD              MOVAPS XMM1,XMM13
14038288c:  E8 5F 39 0C 00           CALL 0x1404461f0
140382891:  F3 0F 10 05 EF F7 60 00  MOVSS XMM0,dword ptr [0x140992088]
140382899:  48 8D 55 87              LEA RDX,[RBP + -0x79]
14038289d:  F3 0F 10 35 57 82 58 00  MOVSS XMM6,dword ptr [0x14090aafc]
1403828a5:  48 8D 0D E4 22 6D 00     LEA RCX,[0x140a54b90]
1403828ac:  F3 44 0F 10 75 6F        MOVSS XMM14,dword ptr [RBP + 0x6f]
1403828b2:  32 DB                    XOR BL,BL
1403828b4:  F3 0F 59 C6              MULSS XMM0,XMM6
1403828b8:  F3 41 0F 59 C6           MULSS XMM0,XMM14
1403828bd:  F3 0F 11 05 23 BA 63 00  MOVSS dword ptr [0x1409be2e8],XMM0
1403828c5:  F3 0F 11 05 1F BA 63 00  MOVSS dword ptr [0x1409be2ec],XMM0
1403828cd:  FF 15 95 9F 4E 00        CALL qword ptr [0x14086c868]
1403828d3:  41 0F 28 DB              MOVAPS XMM3,XMM11
1403828d7:  48 8D 0D 82 B7 63 00     LEA RCX,[0x1409be060]
1403828de:  48 8B D0                 MOV RDX,RAX
1403828e1:  41 0F 28 D3              MOVAPS XMM2,XMM11
1403828e5:  FF 15 65 9F 4E 00        CALL qword ptr [0x14086c850]
1403828eb:  49 8B 56 48              MOV RDX,qword ptr [R14 + 0x48]
1403828ef:  BE 01 00 00 00           MOV ESI,0x1
1403828f4:  84 C0                    TEST AL,AL
1403828f6:  0F B6 CB                 MOVZX ECX,BL
1403828f9:  44 89 65 47              MOV dword ptr [RBP + 0x47],R12D
1403828fd:  0F 45 CE                 CMOVNZ ECX,ESI
140382900:  89 5D 67                 MOV dword ptr [RBP + 0x67],EBX
140382903:  89 4D 6F                 MOV dword ptr [RBP + 0x6f],ECX
140382906:  48 85 D2                 TEST RDX,RDX
140382909:  74 2B                    JZ 0x140382936
14038290b:  44 38 A7 53 44 01 00     CMP byte ptr [RDI + 0x14453],R12B
140382912:  74 61                    JZ 0x140382975
140382914:  48 8B CF                 MOV RCX,RDI
140382917:  E8 A4 4D F7 FF           CALL 0x1402f76c0
14038291c:  48 8B 87 98 17 01 00     MOV RAX,qword ptr [RDI + 0x11798]
140382923:  48 2B 87 90 17 01 00     SUB RAX,qword ptr [RDI + 0x11790]
14038292a:  48 C1 F8 03              SAR RAX,0x3
14038292e:  48 85 C0                 TEST RAX,RAX
140382931:  0F 95 C3                 SETNZ BL
140382934:  EB 2F                    JMP 0x140382965
140382936:  44 38 A7 53 44 01 00     CMP byte ptr [RDI + 0x14453],R12B
14038293d:  74 36                    JZ 0x140382975
14038293f:  49 8B D6                 MOV RDX,R14
140382942:  48 8B CF                 MOV RCX,RDI
140382945:  E8 16 50 F7 FF           CALL 0x1402f7960
14038294a:  48 8B 87 98 17 01 00     MOV RAX,qword ptr [RDI + 0x11798]
140382951:  48 2B 87 90 17 01 00     SUB RAX,qword ptr [RDI + 0x11790]
140382958:  48 C1 F8 03              SAR RAX,0x3
14038295c:  48 85 C0                 TEST RAX,RAX
14038295f:  0F B6 DB                 MOVZX EBX,BL
140382962:  0F 45 DE                 CMOVNZ EBX,ESI
140382965:  89 5D 67                 MOV dword ptr [RBP + 0x67],EBX
140382968:  44 38 A7 53 44 01 00     CMP byte ptr [RDI + 0x14453],R12B
14038296f:  0F 85 9A 00 00 00        JNZ 0x140382a0f
140382975:  40 38 B7 A1 05 00 00     CMP byte ptr [RDI + 0x5a1],SIL
14038297c:  0F 85 8D 00 00 00        JNZ 0x140382a0f
140382982:  49 8B 56 48              MOV RDX,qword ptr [R14 + 0x48]
140382986:  48 8B CF                 MOV RCX,RDI
140382989:  48 85 D2                 TEST RDX,RDX
14038298c:  74 07                    JZ 0x140382995
14038298e:  E8 2D 4D F7 FF           CALL 0x1402f76c0
140382993:  EB 08                    JMP 0x14038299d
140382995:  49 8B D6                 MOV RDX,R14
140382998:  E8 C3 4F F7 FF           CALL 0x1402f7960
14038299d:  48 8B 87 98 17 01 00     MOV RAX,qword ptr [RDI + 0x11798]
1403829a4:  41 8B D4                 MOV EDX,R12D
1403829a7:  48 2B 87 90 17 01 00     SUB RAX,qword ptr [RDI + 0x11790]
1403829ae:  45 8B C4                 MOV R8D,R12D
1403829b1:  48 C1 F8 03              SAR RAX,0x3
1403829b5:  48 85 C0                 TEST RAX,RAX
1403829b8:  74 40                    JZ 0x1403829fa
1403829ba:  4C 8B 97 90 17 01 00     MOV R10,qword ptr [RDI + 0x11790]
1403829c1:  49 8B C4                 MOV RAX,R12
1403829c4:  4C 8B 8F 98 17 01 00     MOV R9,qword ptr [RDI + 0x11798]
1403829cb:  4D 2B CA                 SUB R9,R10
1403829ce:  49 C1 F9 03              SAR R9,0x3
1403829d2:  0F 1F 40 00              NOP dword ptr [RAX]
1403829d6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
1403829e0:  49 8B 04 C2              MOV RAX,qword ptr [R10 + RAX*0x8]
1403829e4:  8B 88 C8 00 00 00        MOV ECX,dword ptr [RAX + 0xc8]
1403829ea:  3B CA                    CMP ECX,EDX
1403829ec:  0F 4F D1                 CMOVG EDX,ECX
1403829ef:  41 FF C0                 INC R8D
1403829f2:  41 8B C0                 MOV EAX,R8D
1403829f5:  49 3B C1                 CMP RAX,R9
1403829f8:  72 E6                    JC 0x1403829e0
1403829fa:  3B 97 94 05 00 00        CMP EDX,dword ptr [RDI + 0x594]

