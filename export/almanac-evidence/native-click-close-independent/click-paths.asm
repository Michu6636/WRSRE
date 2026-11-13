=== export/140373ea0_1403829fd_disasm.asm: 0x140374278..0x140374366 ===
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
=== export/140373ea0_1403829fd_disasm.asm: 0x140373fcc..0x140374044 ===
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
=== export/ch_tab23_full.asm: 0x1403620c1..0x1403621a5 ===
1403620c1:  FF 15 89 A7 50 00        CALL qword ptr [0x14086c850]
1403620c7:  84 C0                    TEST AL,AL
1403620c9:  0F 84 D6 00 00 00        JZ 0x1403621a5
1403620cf:  48 8B 45 20              MOV RAX,qword ptr [RBP + 0x20]
1403620d3:  41 8B 54 07 40           MOV EDX,dword ptr [R15 + RAX*0x1 + 0x40]
1403620d8:  48 8D 0D B1 54 63 00     LEA RCX,[0x140997590]
1403620df:  FF 15 AB A7 50 00        CALL qword ptr [0x14086c890]
1403620e5:  49 8B D6                 MOV RDX,R14
1403620e8:  48 2B D0                 SUB RDX,RAX
1403620eb:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
1403620f0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
1403620f3:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
1403620fb:  48 8D 40 02              LEA RAX,[RAX + 0x2]
1403620ff:  66 85 C9                 TEST CX,CX
140362102:  75 EC                    JNZ 0x1403620f0
140362104:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14036210b:  41 0F 28 D9              MOVAPS XMM3,XMM9
14036210f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140362113:  41 0F 28 C9              MOVAPS XMM1,XMM9
140362117:  48 8D 8D 84 0C 00 00     LEA RCX,[RBP + 0xc84]
14036211e:  FF 15 FC 9E 50 00        CALL qword ptr [0x14086c020]
140362124:  48 8B D0                 MOV RDX,RAX
140362127:  48 8D 0D 32 BF 65 00     LEA RCX,[0x1409be060]
14036212e:  FF 15 64 A7 50 00        CALL qword ptr [0x14086c898]
140362134:  48 8D 0D 55 2A 6F 00     LEA RCX,[0x140a54b90]
14036213b:  FF 15 77 A7 50 00        CALL qword ptr [0x14086c8b8]
140362141:  84 C0                    TEST AL,AL
140362143:  74 60                    JZ 0x1403621a5
140362145:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
14036214a:  75 47                    JNZ 0x140362193
14036214c:  49 8D 9E 10 FD 00 00     LEA RBX,[R14 + 0xfd10]
140362153:  48 8D 15 5E 66 55 00     LEA RDX,[0x1408b87b8]
14036215a:  48 8B CB                 MOV RCX,RBX
14036215d:  FF 15 AD AB 50 00        CALL qword ptr [0x14086cd10]
140362163:  84 C0                    TEST AL,AL
140362165:  74 2C                    JZ 0x140362193
140362167:  48 8D 15 4A 66 55 00     LEA RDX,[0x1408b87b8]
14036216e:  48 8B CB                 MOV RCX,RBX
140362171:  FF 15 A1 AB 50 00        CALL qword ptr [0x14086cd18]
140362177:  48 8B D0                 MOV RDX,RAX
14036217a:  4C 89 64 24 20           MOV qword ptr [RSP + 0x20],R12
14036217f:  41 0F 28 D9              MOVAPS XMM3,XMM9
140362183:  41 B0 01                 MOV R8B,0x1
140362186:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
14036218d:  FF 15 15 B1 50 00        CALL qword ptr [0x14086d2a8]
140362193:  41 89 B6 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],ESI
14036219a:  41 C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [R14 + 0x10be8],0xffffffff
1403621a5:  41 39 B6 5C C6 00 00     CMP dword ptr [R14 + 0xc65c],ESI
=== export/ch_tab23_full.asm: 0x140362713..0x140362807 ===
140362713:  48 8D 95 E8 0A 00 00     LEA RDX,[RBP + 0xae8]
14036271a:  48 8D 0D 6F 24 6F 00     LEA RCX,[0x140a54b90]
140362721:  FF 15 41 A1 50 00        CALL qword ptr [0x14086c868]
140362727:  48 8B D0                 MOV RDX,RAX
14036272a:  41 0F 28 D8              MOVAPS XMM3,XMM8
14036272e:  41 0F 28 D0              MOVAPS XMM2,XMM8
140362732:  48 8D 0D 27 B9 65 00     LEA RCX,[0x1409be060]
140362739:  FF 15 11 A1 50 00        CALL qword ptr [0x14086c850]
14036273f:  84 C0                    TEST AL,AL
140362741:  0F 84 F0 00 00 00        JZ 0x140362837
140362747:  BA 5D 02 00 00           MOV EDX,0x25d
14036274c:  48 8D 0D 3D 4E 63 00     LEA RCX,[0x140997590]
140362753:  FF 15 37 A1 50 00        CALL qword ptr [0x14086c890]
140362759:  49 8B D6                 MOV RDX,R14
14036275c:  48 2B D0                 SUB RDX,RAX
14036275f:  90                       NOP
140362760:  0F B7 08                 MOVZX ECX,word ptr [RAX]
140362763:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
14036276b:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14036276f:  66 85 C9                 TEST CX,CX
140362772:  75 EC                    JNZ 0x140362760
140362774:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14036277b:  41 0F 28 D9              MOVAPS XMM3,XMM9
14036277f:  41 0F 28 D1              MOVAPS XMM2,XMM9
140362783:  41 0F 28 C9              MOVAPS XMM1,XMM9
140362787:  48 8D 8D 04 0D 00 00     LEA RCX,[RBP + 0xd04]
14036278e:  FF 15 8C 98 50 00        CALL qword ptr [0x14086c020]
140362794:  48 8B D0                 MOV RDX,RAX
140362797:  48 8D 0D C2 B8 65 00     LEA RCX,[0x1409be060]
14036279e:  FF 15 F4 A0 50 00        CALL qword ptr [0x14086c898]
1403627a4:  48 8D 0D E5 23 6F 00     LEA RCX,[0x140a54b90]
1403627ab:  FF 15 07 A1 50 00        CALL qword ptr [0x14086c8b8]
1403627b1:  84 C0                    TEST AL,AL
1403627b3:  74 52                    JZ 0x140362807
1403627b5:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
1403627ba:  75 47                    JNZ 0x140362803
1403627bc:  49 8D 9E 10 FD 00 00     LEA RBX,[R14 + 0xfd10]
1403627c3:  48 8D 15 6E 5F 55 00     LEA RDX,[0x1408b8738]
1403627ca:  48 8B CB                 MOV RCX,RBX
1403627cd:  FF 15 3D A5 50 00        CALL qword ptr [0x14086cd10]
1403627d3:  84 C0                    TEST AL,AL
1403627d5:  74 2C                    JZ 0x140362803
1403627d7:  48 8D 15 5A 5F 55 00     LEA RDX,[0x1408b8738]
1403627de:  48 8B CB                 MOV RCX,RBX
1403627e1:  FF 15 31 A5 50 00        CALL qword ptr [0x14086cd18]
1403627e7:  48 8B D0                 MOV RDX,RAX
1403627ea:  4C 89 64 24 20           MOV qword ptr [RSP + 0x20],R12
1403627ef:  41 0F 28 D9              MOVAPS XMM3,XMM9
1403627f3:  41 B0 01                 MOV R8B,0x1
1403627f6:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
1403627fd:  FF 15 A5 AA 50 00        CALL qword ptr [0x14086d2a8]
140362803:  C6 45 AC 01              MOV byte ptr [RBP + -0x54],0x1
140362807:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
=== export/ch_tab23_full.asm: 0x140361c9d..0x140361cc6 ===
140361c9d:  41 8B 86 E8 0B 01 00     MOV EAX,dword ptr [R14 + 0x10be8]
140361ca4:  48 3B C2                 CMP RAX,RDX
140361ca7:  73 12                    JNC 0x140361cbb
140361ca9:  4D 85 FF                 TEST R15,R15
140361cac:  75 0D                    JNZ 0x140361cbb
140361cae:  49 8B CE                 MOV RCX,R14
140361cb1:  E8 DA DD 14 00           CALL 0x1404afa90
140361cb6:  E9 76 21 01 00           JMP 0x140373e31
140361cbb:  41 C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [R14 + 0x10be8],0xffffffff
140361cc6:  45 0F 57 C0              XORPS XMM8,XMM8
