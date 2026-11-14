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
