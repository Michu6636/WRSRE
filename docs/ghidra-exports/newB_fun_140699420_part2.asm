; Range: 0x14069A000 to 0x14069E200
14069a006:  F3 0F 10 4C 88 04        MOVSS XMM1,dword ptr [RAX + RCX*0x4 + 0x4]
14069a00c:  F3 0F 11 8D 2C 01 00 00  MOVSS dword ptr [RBP + 0x12c],XMM1
14069a014:  F3 0F 10 44 88 08        MOVSS XMM0,dword ptr [RAX + RCX*0x4 + 0x8]
14069a01a:  F3 0F 11 85 30 01 00 00  MOVSS dword ptr [RBP + 0x130],XMM0
14069a022:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069a029:  48 81 C1 20 03 00 00     ADD RCX,0x320
14069a030:  48 8D 95 30 0D 00 00     LEA RDX,[RBP + 0xd30]
14069a037:  FF 15 83 27 1D 00        CALL qword ptr [0x14086c7c0]
14069a03d:  4C 8B C0                 MOV R8,RAX
14069a040:  48 8D 95 28 01 00 00     LEA RDX,[RBP + 0x128]
14069a047:  48 8D 8D 28 01 00 00     LEA RCX,[RBP + 0x128]
14069a04e:  FF 15 B4 25 1D 00        CALL qword ptr [0x14086c608]
14069a054:  48 8D 95 28 01 00 00     LEA RDX,[RBP + 0x128]
14069a05b:  49 8B CC                 MOV RCX,R12
14069a05e:  E8 8D 52 99 FF           CALL 0x14002f2f0
14069a063:  FF C7                    INC EDI
14069a065:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a06c:  4C 8B 80 18 03 00 00     MOV R8,qword ptr [RAX + 0x318]
14069a073:  49 8B 88 D0 02 00 00     MOV RCX,qword ptr [R8 + 0x2d0]
14069a07a:  49 2B 88 C8 02 00 00     SUB RCX,qword ptr [R8 + 0x2c8]
14069a081:  48 8B C6                 MOV RAX,RSI
14069a084:  48 F7 E9                 IMUL RCX
14069a087:  48 D1 FA                 SAR RDX,0x1
14069a08a:  48 8B C2                 MOV RAX,RDX
14069a08d:  48 C1 E8 3F              SHR RAX,0x3f
14069a091:  48 03 D0                 ADD RDX,RAX
14069a094:  44 8B CF                 MOV R9D,EDI
14069a097:  4C 3B CA                 CMP R9,RDX
14069a09a:  0F 82 30 FF FF FF        JC 0x140699fd0
14069a0a0:  48 8B B5 A8 1C 00 00     MOV RSI,qword ptr [RBP + 0x1ca8]
14069a0a7:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a0ae:  48 C1 E6 05              SHL RSI,0x5
14069a0b2:  48 8B 90 78 0B 00 00     MOV RDX,qword ptr [RAX + 0xb78]
14069a0b9:  48 83 C2 0C              ADD RDX,0xc
14069a0bd:  48 03 D6                 ADD RDX,RSI
14069a0c0:  49 8B CC                 MOV RCX,R12
14069a0c3:  E8 28 52 99 FF           CALL 0x14002f2f0
14069a0c8:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a0cf:  48 8B 90 78 0B 00 00     MOV RDX,qword ptr [RAX + 0xb78]
14069a0d6:  48 03 D6                 ADD RDX,RSI
14069a0d9:  49 8B CC                 MOV RCX,R12
14069a0dc:  E8 0F 52 99 FF           CALL 0x14002f2f0
14069a0e1:  45 39 AF 10 05 00 00     CMP dword ptr [R15 + 0x510],R13D
14069a0e8:  7C 37                    JL 0x14069a121
14069a0ea:  45 38 AF FD 05 00 00     CMP byte ptr [R15 + 0x5fd],R13B
14069a0f1:  75 12                    JNZ 0x14069a105
14069a0f3:  45 33 C9                 XOR R9D,R9D
14069a0f6:  4D 8B C7                 MOV R8,R15
14069a0f9:  49 8B D6                 MOV RDX,R14
14069a0fc:  E8 3F 15 D2 FF           CALL 0x1403bb640
14069a101:  84 C0                    TEST AL,AL
14069a103:  74 1C                    JZ 0x14069a121
14069a105:  49 8B CF                 MOV RCX,R15
14069a108:  E8 13 DA 01 00           CALL 0x1406b7b20
14069a10d:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069a111:  41 B0 01                 MOV R8B,0x1
14069a114:  49 8B CF                 MOV RCX,R15
14069a117:  E8 F4 9D 00 00           CALL 0x1406a3f10
14069a11c:  E9 EB 93 00 00           JMP 0x1406a350c
14069a121:  41 BE 01 00 00 00        MOV R14D,0x1
14069a127:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069a12e:  F3 44 0F 10 3D F1 06 27 00 MOVSS XMM15,dword ptr [0x14090a828]
14069a137:  4D 85 C0                 TEST R8,R8
14069a13a:  0F 84 97 18 00 00        JZ 0x14069b9d7
14069a140:  84 DB                    TEST BL,BL
14069a142:  0F 85 8F 18 00 00        JNZ 0x14069b9d7
14069a148:  49 8B 9F F8 04 00 00     MOV RBX,qword ptr [R15 + 0x4f8]
14069a14f:  48 85 DB                 TEST RBX,RBX
14069a152:  0F 84 8E 04 00 00        JZ 0x14069a5e6
14069a158:  49 3B D8                 CMP RBX,R8
14069a15b:  75 17                    JNZ 0x14069a174
14069a15d:  48 8B C3                 MOV RAX,RBX
14069a160:  48 8B 8B 18 03 00 00     MOV RCX,qword ptr [RBX + 0x318]
14069a167:  83 B9 60 03 00 00 26     CMP dword ptr [RCX + 0x360],0x26
14069a16e:  0F 85 21 04 00 00        JNZ 0x14069a595
14069a174:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069a17b:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069a182:  48 C1 F8 03              SAR RAX,0x3
14069a186:  48 83 F8 01              CMP RAX,0x1
14069a18a:  0F 85 05 04 00 00        JNZ 0x14069a595
14069a190:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069a197:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069a19e:  83 B9 60 03 00 00 26     CMP dword ptr [RCX + 0x360],0x26
14069a1a5:  75 0D                    JNZ 0x14069a1b4
14069a1a7:  4D 39 AF D0 1A 00 00     CMP qword ptr [R15 + 0x1ad0],R13
14069a1ae:  0F 85 E1 03 00 00        JNZ 0x14069a595
14069a1b4:  41 83 BF 6C 05 00 00 FF  CMP dword ptr [R15 + 0x56c],-0x1
14069a1bc:  75 0E                    JNZ 0x14069a1cc
14069a1be:  41 83 BF 10 05 00 00 FF  CMP dword ptr [R15 + 0x510],-0x1
14069a1c6:  0F 84 1A 04 00 00        JZ 0x14069a5e6
14069a1cc:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069a1d1:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069a1d5:  48 B8 AB AA AA AA AA AA AA 2A MOV RAX,0x2aaaaaaaaaaaaaab
14069a1df:  48 F7 E9                 IMUL RCX
14069a1e2:  48 D1 FA                 SAR RDX,0x1
14069a1e5:  48 8B C2                 MOV RAX,RDX
14069a1e8:  48 C1 E8 3F              SHR RAX,0x3f
14069a1ec:  48 03 D0                 ADD RDX,RAX
14069a1ef:  41 39 97 68 07 00 00     CMP dword ptr [R15 + 0x768],EDX
14069a1f6:  0F 8C EA 03 00 00        JL 0x14069a5e6
14069a1fc:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069a203:  4C 39 28                 CMP qword ptr [RAX],R13
14069a206:  0F 84 DA 03 00 00        JZ 0x14069a5e6
14069a20c:  4C 39 00                 CMP qword ptr [RAX],R8
14069a20f:  74 13                    JZ 0x14069a224
14069a211:  48 8B C8                 MOV RCX,RAX
14069a214:  49 8B 87 38 05 00 00     MOV RAX,qword ptr [R15 + 0x538]
14069a21b:  48 39 01                 CMP qword ptr [RCX],RAX
14069a21e:  0F 85 C2 03 00 00        JNZ 0x14069a5e6
14069a224:  49 8B 87 90 04 00 00     MOV RAX,qword ptr [R15 + 0x490]
14069a22b:  49 2B 87 88 04 00 00     SUB RAX,qword ptr [R15 + 0x488]
14069a232:  48 C1 F8 03              SAR RAX,0x3
14069a236:  48 85 C0                 TEST RAX,RAX
14069a239:  0F 85 16 02 00 00        JNZ 0x14069a455
14069a23f:  4D 39 AF 08 17 00 00     CMP qword ptr [R15 + 0x1708],R13
14069a246:  74 2B                    JZ 0x14069a273
14069a248:  49 8B 8F 08 17 00 00     MOV RCX,qword ptr [R15 + 0x1708]
14069a24f:  48 8B 81 40 7C 00 00     MOV RAX,qword ptr [RCX + 0x7c40]
14069a256:  48 2B 81 38 7C 00 00     SUB RAX,qword ptr [RCX + 0x7c38]
14069a25d:  48 C1 F8 07              SAR RAX,0x7
14069a261:  48 85 C0                 TEST RAX,RAX
14069a264:  74 0D                    JZ 0x14069a273
14069a266:  83 B9 94 02 00 00 01     CMP dword ptr [RCX + 0x294],0x1
14069a26d:  0F 84 E2 01 00 00        JZ 0x14069a455
14069a273:  45 89 AF 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R13D
14069a27a:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069a281:  48 85 C0                 TEST RAX,RAX
14069a284:  74 23                    JZ 0x14069a2a9
14069a286:  49 3B 87 28 05 00 00     CMP RAX,qword ptr [R15 + 0x528]
14069a28d:  74 1A                    JZ 0x14069a2a9
14069a28f:  48 8B C8                 MOV RCX,RAX
14069a292:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069a299:  48 39 81 B0 0B 00 00     CMP qword ptr [RCX + 0xbb0],RAX
14069a2a0:  75 07                    JNZ 0x14069a2a9
14069a2a2:  4C 89 A9 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],R13
14069a2a9:  45 39 AF 6C 05 00 00     CMP dword ptr [R15 + 0x56c],R13D
14069a2b0:  7C 2C                    JL 0x14069a2de
14069a2b2:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069a2b9:  4C 39 A9 D0 05 00 00     CMP qword ptr [RCX + 0x5d0],R13
14069a2c0:  74 1C                    JZ 0x14069a2de
14069a2c2:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069a2c9:  48 89 81 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],RAX
14069a2d0:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a2d7:  49 89 87 80 0C 00 00     MOV qword ptr [R15 + 0xc80],RAX
14069a2de:  4D 89 AF 98 07 00 00     MOV qword ptr [R15 + 0x798],R13
14069a2e5:  49 8B 87 C0 07 00 00     MOV RAX,qword ptr [R15 + 0x7c0]
14069a2ec:  49 2B 87 B8 07 00 00     SUB RAX,qword ptr [R15 + 0x7b8]
14069a2f3:  48 C1 F8 03              SAR RAX,0x3
14069a2f7:  48 85 C0                 TEST RAX,RAX
14069a2fa:  74 08                    JZ 0x14069a304
14069a2fc:  49 8B CF                 MOV RCX,R15
14069a2ff:  E8 6C 76 FE FF           CALL 0x140681970
14069a304:  49 63 87 6C 05 00 00     MOVSXD RAX,dword ptr [R15 + 0x56c]
14069a30b:  85 C0                    TEST EAX,EAX
14069a30d:  78 28                    JS 0x14069a337
14069a30f:  48 8B C8                 MOV RCX,RAX
14069a312:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a319:  44 88 AC 01 58 0A 00 00  MOV byte ptr [RCX + RAX*0x1 + 0xa58],R13B
14069a321:  49 63 8F 6C 05 00 00     MOVSXD RCX,dword ptr [R15 + 0x56c]
14069a328:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a32f:  4C 89 AC C8 78 0A 00 00  MOV qword ptr [RAX + RCX*0x8 + 0xa78],R13
14069a337:  49 8B CF                 MOV RCX,R15
14069a33a:  E8 E1 8B 02 00           CALL 0x1406c2f20
14069a33f:  84 C0                    TEST AL,AL
14069a341:  74 15                    JZ 0x14069a358
14069a343:  41 83 BF 0C 05 00 00 15  CMP dword ptr [R15 + 0x50c],0x15
14069a34b:  75 0B                    JNZ 0x14069a358
14069a34d:  41 C7 87 0C 05 00 00 FF FF FF FF MOV dword ptr [R15 + 0x50c],0xffffffff
14069a358:  41 83 BF 0C 05 00 00 F6  CMP dword ptr [R15 + 0x50c],-0xa
14069a360:  7E 6F                    JLE 0x14069a3d1
14069a362:  4D 39 AF 38 05 00 00     CMP qword ptr [R15 + 0x538],R13
14069a369:  74 33                    JZ 0x14069a39e
14069a36b:  49 8B 8F 38 05 00 00     MOV RCX,qword ptr [R15 + 0x538]
14069a372:  49 89 8F F0 04 00 00     MOV qword ptr [R15 + 0x4f0],RCX
14069a379:  49 3B 8F F8 04 00 00     CMP RCX,qword ptr [R15 + 0x4f8]
14069a380:  74 13                    JZ 0x14069a395
14069a382:  49 8D 97 90 03 00 00     LEA RDX,[R15 + 0x390]
14069a389:  48 81 C1 70 0C 00 00     ADD RCX,0xc70
14069a390:  E8 FB 6A A1 FF           CALL 0x1400b0e90
14069a395:  4D 89 AF 38 05 00 00     MOV qword ptr [R15 + 0x538],R13
14069a39c:  EB 2A                    JMP 0x14069a3c8
14069a39e:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069a3a5:  49 89 8F F0 04 00 00     MOV qword ptr [R15 + 0x4f0],RCX
14069a3ac:  49 3B 8F F8 04 00 00     CMP RCX,qword ptr [R15 + 0x4f8]
14069a3b3:  74 13                    JZ 0x14069a3c8
14069a3b5:  49 8D 97 90 03 00 00     LEA RDX,[R15 + 0x390]
14069a3bc:  48 81 C1 70 0C 00 00     ADD RCX,0xc70
14069a3c3:  E8 C8 6A A1 FF           CALL 0x1400b0e90
14069a3c8:  4D 89 AF 28 05 00 00     MOV qword ptr [R15 + 0x528],R13
14069a3cf:  EB 07                    JMP 0x14069a3d8
14069a3d1:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069a3d8:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069a3df:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069a3e6:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069a3ed:  49 8B 97 90 03 00 00     MOV RDX,qword ptr [R15 + 0x390]
14069a3f4:  48 8D 0D 15 AB 33 00     LEA RCX,[0x1409d4f10]
14069a3fb:  E8 90 7E 19 00           CALL 0x140832290
14069a400:  45 39 AF 10 05 00 00     CMP dword ptr [R15 + 0x510],R13D
14069a407:  7C 1F                    JL 0x14069a428
14069a409:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069a410:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069a417:  44 38 A9 79 09 00 00     CMP byte ptr [RCX + 0x979],R13B
14069a41e:  75 08                    JNZ 0x14069a428
14069a420:  49 8B CF                 MOV RCX,R15
14069a423:  E8 58 33 01 00           CALL 0x1406ad780
14069a428:  41 B1 01                 MOV R9B,0x1
14069a42b:  4D 8B 87 F0 04 00 00     MOV R8,qword ptr [R15 + 0x4f0]
14069a432:  41 8B 97 10 05 00 00     MOV EDX,dword ptr [R15 + 0x510]
14069a439:  49 8B CF                 MOV RCX,R15
14069a43c:  E8 9F CC FC FF           CALL 0x1406670e0
14069a441:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069a445:  41 B0 01                 MOV R8B,0x1
14069a448:  49 8B CF                 MOV RCX,R15
14069a44b:  E8 C0 9A 00 00           CALL 0x1406a3f10
14069a450:  E9 B7 90 00 00           JMP 0x1406a350c
14069a455:  4D 39 AF 38 05 00 00     CMP qword ptr [R15 + 0x538],R13
14069a45c:  74 09                    JZ 0x14069a467
14069a45e:  49 8B 9F 38 05 00 00     MOV RBX,qword ptr [R15 + 0x538]
14069a465:  EB 07                    JMP 0x14069a46e
14069a467:  49 8B 9F 28 05 00 00     MOV RBX,qword ptr [R15 + 0x528]
14069a46e:  45 33 C0                 XOR R8D,R8D
14069a471:  48 8B D3                 MOV RDX,RBX
14069a474:  49 8B CF                 MOV RCX,R15
14069a477:  E8 94 FC FE FF           CALL 0x14068a110
14069a47c:  4D 8B C7                 MOV R8,R15
14069a47f:  48 8B D3                 MOV RDX,RBX
14069a482:  48 8D 0D 87 AA 33 00     LEA RCX,[0x1409d4f10]
14069a489:  E8 D2 B9 AB FF           CALL 0x140155e60
14069a48e:  48 8B D8                 MOV RBX,RAX
14069a491:  48 85 C0                 TEST RAX,RAX
14069a494:  74 1D                    JZ 0x14069a4b3
14069a496:  45 33 C0                 XOR R8D,R8D
14069a499:  48 8B D0                 MOV RDX,RAX
14069a49c:  49 8B CF                 MOV RCX,R15
14069a49f:  E8 7C FF FE FF           CALL 0x14068a420
14069a4a4:  84 C0                    TEST AL,AL
14069a4a6:  74 0B                    JZ 0x14069a4b3
14069a4a8:  41 C7 87 0C 05 00 00 08 00 00 00 MOV dword ptr [R15 + 0x50c],0x8
14069a4b3:  49 8B CF                 MOV RCX,R15
14069a4b6:  E8 85 27 FE FF           CALL 0x14067cc40
14069a4bb:  48 85 DB                 TEST RBX,RBX
14069a4be:  0F 84 9E 00 00 00        JZ 0x14069a562
14069a4c4:  48 8B 83 88 06 00 00     MOV RAX,qword ptr [RBX + 0x688]
14069a4cb:  48 2B 83 80 06 00 00     SUB RAX,qword ptr [RBX + 0x680]
14069a4d2:  48 C1 F8 03              SAR RAX,0x3
14069a4d6:  48 85 C0                 TEST RAX,RAX
14069a4d9:  0F 84 83 00 00 00        JZ 0x14069a562
14069a4df:  4D 39 AF F8 04 00 00     CMP qword ptr [R15 + 0x4f8],R13
14069a4e6:  74 7A                    JZ 0x14069a562
14069a4e8:  49 8B 97 F8 04 00 00     MOV RDX,qword ptr [R15 + 0x4f8]
14069a4ef:  48 81 C2 90 0F 00 00     ADD RDX,0xf90
14069a4f6:  48 8B 83 80 06 00 00     MOV RAX,qword ptr [RBX + 0x680]
14069a4fd:  4C 8B 10                 MOV R10,qword ptr [RAX]
14069a500:  74 60                    JZ 0x14069a562
14069a502:  48 8B 4A 08              MOV RCX,qword ptr [RDX + 0x8]
14069a506:  48 2B 0A                 SUB RCX,qword ptr [RDX]
14069a509:  48 C1 F9 03              SAR RCX,0x3
14069a50d:  48 85 C9                 TEST RCX,RCX
14069a510:  74 50                    JZ 0x14069a562
14069a512:  4C 8B 02                 MOV R8,qword ptr [RDX]
14069a515:  49 8B C5                 MOV RAX,R13
14069a518:  4D 3B 14 C0              CMP R10,qword ptr [R8 + RAX*0x8]
14069a51c:  74 0D                    JZ 0x14069a52b
14069a51e:  41 FF C5                 INC R13D
14069a521:  41 8B C5                 MOV EAX,R13D
14069a524:  48 3B C1                 CMP RAX,RCX
14069a527:  72 EF                    JC 0x14069a518
14069a529:  EB 37                    JMP 0x14069a562
14069a52b:  45 33 C9                 XOR R9D,R9D
14069a52e:  41 83 C8 FF              OR R8D,0xffffffff
14069a532:  49 8B D2                 MOV RDX,R10
14069a535:  49 8B CF                 MOV RCX,R15
14069a538:  E8 93 30 FE FF           CALL 0x14067d5d0
14069a53d:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069a544:  C6 00 01                 MOV byte ptr [RAX],0x1
14069a547:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069a54e:  C6 80 C8 00 00 00 00     MOV byte ptr [RAX + 0xc8],0x0
14069a555:  48 8B CB                 MOV RCX,RBX
14069a558:  E8 E3 26 FE FF           CALL 0x14067cc40
14069a55d:  E9 AA 8F 00 00           JMP 0x1406a350c
14069a562:  45 33 C9                 XOR R9D,R9D
14069a565:  41 83 C8 FF              OR R8D,0xffffffff
14069a569:  49 8B 97 F8 04 00 00     MOV RDX,qword ptr [R15 + 0x4f8]
14069a570:  49 8B CF                 MOV RCX,R15
14069a573:  E8 58 30 FE FF           CALL 0x14067d5d0
14069a578:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069a57f:  C6 00 00                 MOV byte ptr [RAX],0x0
14069a582:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069a589:  C6 80 C8 00 00 00 00     MOV byte ptr [RAX + 0xc8],0x0
14069a590:  E9 77 8F 00 00           JMP 0x1406a350c
14069a595:  48 85 DB                 TEST RBX,RBX
14069a598:  74 4C                    JZ 0x14069a5e6
14069a59a:  49 3B D8                 CMP RBX,R8
14069a59d:  75 17                    JNZ 0x14069a5b6
14069a59f:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069a5a6:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069a5ad:  83 B9 60 03 00 00 26     CMP dword ptr [RCX + 0x360],0x26
14069a5b4:  75 30                    JNZ 0x14069a5e6
14069a5b6:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069a5bd:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069a5c4:  48 C1 F8 03              SAR RAX,0x3
14069a5c8:  48 83 F8 01              CMP RAX,0x1
14069a5cc:  75 18                    JNZ 0x14069a5e6
14069a5ce:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a5d5:  48 8B 88 B0 0B 00 00     MOV RCX,qword ptr [RAX + 0xbb0]
14069a5dc:  48 85 C9                 TEST RCX,RCX
14069a5df:  74 05                    JZ 0x14069a5e6
14069a5e1:  49 3B CF                 CMP RCX,R15
14069a5e4:  75 72                    JNZ 0x14069a658
14069a5e6:  41 8B BF 6C 05 00 00     MOV EDI,dword ptr [R15 + 0x56c]
14069a5ed:  83 FF FF                 CMP EDI,-0x1
14069a5f0:  0F 85 69 03 00 00        JNZ 0x14069a95f
14069a5f6:  41 39 BF 10 05 00 00     CMP dword ptr [R15 + 0x510],EDI
14069a5fd:  75 74                    JNZ 0x14069a673
14069a5ff:  49 8B 97 28 05 00 00     MOV RDX,qword ptr [R15 + 0x528]
14069a606:  E8 85 83 01 00           CALL 0x1406b2990
14069a60b:  84 C0                    TEST AL,AL
14069a60d:  74 64                    JZ 0x14069a673
14069a60f:  4C 39 AA B0 0B 00 00     CMP qword ptr [RDX + 0xbb0],R13
14069a616:  74 1C                    JZ 0x14069a634
14069a618:  48 85 DB                 TEST RBX,RBX
14069a61b:  74 56                    JZ 0x14069a673
14069a61d:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069a624:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069a62b:  83 B9 60 03 00 00 2D     CMP dword ptr [RCX + 0x360],0x2d
14069a632:  75 3F                    JNZ 0x14069a673
14069a634:  49 8B CF                 MOV RCX,R15
14069a637:  E8 24 84 01 00           CALL 0x1406b2a60
14069a63c:  41 83 BF 6C 05 00 00 FF  CMP dword ptr [R15 + 0x56c],-0x1
14069a644:  0F 85 C2 8E 00 00        JNZ 0x1406a350c
14069a64a:  41 83 BF 10 05 00 00 FF  CMP dword ptr [R15 + 0x510],-0x1
14069a652:  0F 85 B4 8E 00 00        JNZ 0x1406a350c
14069a658:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069a65c:  41 B0 01                 MOV R8B,0x1
14069a65f:  49 8B CF                 MOV RCX,R15
14069a662:  E8 A9 98 00 00           CALL 0x1406a3f10
14069a667:  45 89 AF 24 0D 00 00     MOV dword ptr [R15 + 0xd24],R13D
14069a66e:  E9 99 8E 00 00           JMP 0x1406a350c
14069a673:  83 FF FF                 CMP EDI,-0x1
14069a676:  0F 85 E3 02 00 00        JNZ 0x14069a95f
14069a67c:  41 39 BF 10 05 00 00     CMP dword ptr [R15 + 0x510],EDI
14069a683:  0F 85 D6 02 00 00        JNZ 0x14069a95f
14069a689:  49 8B BF 28 05 00 00     MOV RDI,qword ptr [R15 + 0x528]
14069a690:  4C 39 AF D0 05 00 00     CMP qword ptr [RDI + 0x5d0],R13
14069a697:  0F 84 A7 02 00 00        JZ 0x14069a944
14069a69d:  48 8B 9F B0 0B 00 00     MOV RBX,qword ptr [RDI + 0xbb0]
14069a6a4:  49 3B 9F 90 03 00 00     CMP RBX,qword ptr [R15 + 0x390]
14069a6ab:  0F 84 93 02 00 00        JZ 0x14069a944
14069a6b1:  48 85 DB                 TEST RBX,RBX
14069a6b4:  0F 84 8A 02 00 00        JZ 0x14069a944
14069a6ba:  49 8B 97 08 17 00 00     MOV RDX,qword ptr [R15 + 0x1708]
14069a6c1:  48 8B 82 E8 85 00 00     MOV RAX,qword ptr [RDX + 0x85e8]
14069a6c8:  48 2B 82 E0 85 00 00     SUB RAX,qword ptr [RDX + 0x85e0]
14069a6cf:  48 C1 F8 04              SAR RAX,0x4
14069a6d3:  48 85 C0                 TEST RAX,RAX
14069a6d6:  74 0F                    JZ 0x14069a6e7
14069a6d8:  48 8B 82 E0 85 00 00     MOV RAX,qword ptr [RDX + 0x85e0]
14069a6df:  83 38 18                 CMP dword ptr [RAX],0x18
14069a6e2:  75 03                    JNZ 0x14069a6e7
14069a6e4:  41 B5 01                 MOV R13B,0x1
14069a6e7:  49 8B 87 90 04 00 00     MOV RAX,qword ptr [R15 + 0x490]
14069a6ee:  49 2B 87 88 04 00 00     SUB RAX,qword ptr [R15 + 0x488]
14069a6f5:  48 C1 F8 03              SAR RAX,0x3
14069a6f9:  48 85 C0                 TEST RAX,RAX
14069a6fc:  74 39                    JZ 0x14069a737
14069a6fe:  49 8B 87 88 04 00 00     MOV RAX,qword ptr [R15 + 0x488]
14069a705:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069a708:  4C 8B 81 08 17 00 00     MOV R8,qword ptr [RCX + 0x1708]
14069a70f:  49 8B 80 E8 85 00 00     MOV RAX,qword ptr [R8 + 0x85e8]
14069a716:  49 2B 80 E0 85 00 00     SUB RAX,qword ptr [R8 + 0x85e0]
14069a71d:  48 C1 F8 04              SAR RAX,0x4
14069a721:  48 85 C0                 TEST RAX,RAX
14069a724:  74 11                    JZ 0x14069a737
14069a726:  49 8B 80 E0 85 00 00     MOV RAX,qword ptr [R8 + 0x85e0]
14069a72d:  83 38 18                 CMP dword ptr [RAX],0x18
14069a730:  75 05                    JNZ 0x14069a737
14069a732:  41 B4 01                 MOV R12B,0x1
14069a735:  EB 03                    JMP 0x14069a73a
14069a737:  45 32 E4                 XOR R12B,R12B
14069a73a:  40 32 F6                 XOR SIL,SIL
14069a73d:  49 8B 87 90 04 00 00     MOV RAX,qword ptr [R15 + 0x490]
14069a744:  49 2B 87 88 04 00 00     SUB RAX,qword ptr [R15 + 0x488]
14069a74b:  48 A9 F8 FF FF FF        TEST RAX,-0x8
14069a751:  75 19                    JNZ 0x14069a76c
14069a753:  83 BA 00 86 00 00 01     CMP dword ptr [RDX + 0x8600],0x1
14069a75a:  75 10                    JNZ 0x14069a76c
14069a75c:  40 0F B6 F6              MOVZX ESI,SIL
14069a760:  41 83 BF 0C 05 00 00 32  CMP dword ptr [R15 + 0x50c],0x32
14069a768:  41 0F 4D F6              CMOVGE ESI,R14D
14069a76c:  44 0F 2F 8F 04 06 00 00  COMISS XMM9,dword ptr [RDI + 0x604]
14069a774:  0F 86 C7 01 00 00        JBE 0x14069a941
14069a77a:  44 8B B2 00 86 00 00     MOV R14D,dword ptr [RDX + 0x8600]
14069a781:  41 83 FE 07              CMP R14D,0x7
14069a785:  74 19                    JZ 0x14069a7a0
14069a787:  41 83 FE 02              CMP R14D,0x2
14069a78b:  74 13                    JZ 0x14069a7a0
14069a78d:  45 84 ED                 TEST R13B,R13B
14069a790:  75 0E                    JNZ 0x14069a7a0
14069a792:  45 84 E4                 TEST R12B,R12B
14069a795:  75 09                    JNZ 0x14069a7a0
14069a797:  40 84 F6                 TEST SIL,SIL
14069a79a:  0F 84 A1 01 00 00        JZ 0x14069a941
14069a7a0:  48 8B D7                 MOV RDX,RDI
14069a7a3:  48 8D 0D 66 A7 33 00     LEA RCX,[0x1409d4f10]
14069a7aa:  E8 31 36 AB FF           CALL 0x14014dde0
14069a7af:  4C 63 C0                 MOVSXD R8,EAX
14069a7b2:  85 C0                    TEST EAX,EAX
14069a7b4:  0F 88 87 01 00 00        JS 0x14069a941
14069a7ba:  48 8B 8F 18 06 00 00     MOV RCX,qword ptr [RDI + 0x618]
14069a7c1:  48 2B 8F 10 06 00 00     SUB RCX,qword ptr [RDI + 0x610]
14069a7c8:  48 B8 DD 0B A2 D3 9C 6D 5E 1E MOV RAX,0x1e5e6d9cd3a20bdd
14069a7d2:  48 F7 E9                 IMUL RCX
14069a7d5:  48 C1 FA 0A              SAR RDX,0xa
14069a7d9:  48 8B C2                 MOV RAX,RDX
14069a7dc:  48 C1 E8 3F              SHR RAX,0x3f
14069a7e0:  48 03 D0                 ADD RDX,RAX
14069a7e3:  44 3B C2                 CMP R8D,EDX
14069a7e6:  0F 8D 55 01 00 00        JGE 0x14069a941
14069a7ec:  49 69 C8 B8 21 00 00     IMUL RCX,R8,0x21b8
14069a7f3:  48 8B 87 10 06 00 00     MOV RAX,qword ptr [RDI + 0x610]
14069a7fa:  83 7C 01 04 08           CMP dword ptr [RCX + RAX*0x1 + 0x4],0x8
14069a7ff:  0F 85 3C 01 00 00        JNZ 0x14069a941
14069a805:  41 83 FE 07              CMP R14D,0x7
14069a809:  0F 84 15 01 00 00        JZ 0x14069a924
14069a80f:  40 84 F6                 TEST SIL,SIL
14069a812:  0F 85 0C 01 00 00        JNZ 0x14069a924
14069a818:  41 83 FE 02              CMP R14D,0x2
14069a81c:  75 54                    JNZ 0x14069a872
14069a81e:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069a825:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069a82c:  45 33 ED                 XOR R13D,R13D
14069a82f:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069a836:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a83d:  49 89 87 F0 04 00 00     MOV qword ptr [R15 + 0x4f0],RAX
14069a844:  4C 89 BD A0 1C 00 00     MOV qword ptr [RBP + 0x1ca0],R15
14069a84b:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069a852:  48 81 C1 70 0C 00 00     ADD RCX,0xc70
14069a859:  48 8D 95 A0 1C 00 00     LEA RDX,[RBP + 0x1ca0]
14069a860:  E8 2B 66 A1 FF           CALL 0x1400b0e90
14069a865:  49 8B CF                 MOV RCX,R15
14069a868:  E8 F3 81 01 00           CALL 0x1406b2a60
14069a86d:  E9 D2 00 00 00           JMP 0x14069a944
14069a872:  45 84 ED                 TEST R13B,R13B
14069a875:  75 09                    JNZ 0x14069a880
14069a877:  45 84 E4                 TEST R12B,R12B
14069a87a:  0F 84 C1 00 00 00        JZ 0x14069a941
14069a880:  48 89 9D A0 1C 00 00     MOV qword ptr [RBP + 0x1ca0],RBX
14069a887:  48 83 BB 08 17 00 00 00  CMP qword ptr [RBX + 0x1708],0x0
14069a88f:  74 1F                    JZ 0x14069a8b0
14069a891:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14069a898:  8B 88 00 86 00 00        MOV ECX,dword ptr [RAX + 0x8600]
14069a89e:  83 F9 01                 CMP ECX,0x1
14069a8a1:  0F 84 9A 00 00 00        JZ 0x14069a941
14069a8a7:  83 F9 07                 CMP ECX,0x7
14069a8aa:  0F 84 91 00 00 00        JZ 0x14069a941
14069a8b0:  45 33 ED                 XOR R13D,R13D
14069a8b3:  4C 89 AB 28 05 00 00     MOV qword ptr [RBX + 0x528],R13
14069a8ba:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069a8c1:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069a8c8:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069a8cf:  44 89 AB 34 0D 00 00     MOV dword ptr [RBX + 0xd34],R13D
14069a8d6:  4C 89 AB 98 07 00 00     MOV qword ptr [RBX + 0x798],R13
14069a8dd:  48 8B 83 C0 07 00 00     MOV RAX,qword ptr [RBX + 0x7c0]
14069a8e4:  48 2B 83 B8 07 00 00     SUB RAX,qword ptr [RBX + 0x7b8]
14069a8eb:  48 C1 F8 03              SAR RAX,0x3
14069a8ef:  48 85 C0                 TEST RAX,RAX
14069a8f2:  74 08                    JZ 0x14069a8fc
14069a8f4:  48 8B CB                 MOV RCX,RBX
14069a8f7:  E8 74 70 FE FF           CALL 0x140681970
14069a8fc:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a903:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069a90a:  4C 89 AB 80 0C 00 00     MOV qword ptr [RBX + 0xc80],R13
14069a911:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a918:  48 89 83 F0 04 00 00     MOV qword ptr [RBX + 0x4f0],RAX
14069a91f:  E9 27 FF FF FF           JMP 0x14069a84b
14069a924:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069a92b:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069a932:  45 33 ED                 XOR R13D,R13D
14069a935:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069a93c:  E9 24 FF FF FF           JMP 0x14069a865
14069a941:  45 33 ED                 XOR R13D,R13D
14069a944:  45 89 AF 24 0D 00 00     MOV dword ptr [R15 + 0xd24],R13D
14069a94b:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069a94f:  41 B0 01                 MOV R8B,0x1
14069a952:  49 8B CF                 MOV RCX,R15
14069a955:  E8 B6 95 00 00           CALL 0x1406a3f10
14069a95a:  E9 AD 8B 00 00           JMP 0x1406a350c
14069a95f:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069a964:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069a968:  48 BB AB AA AA AA AA AA AA 2A MOV RBX,0x2aaaaaaaaaaaaaab
14069a972:  48 8B C3                 MOV RAX,RBX
14069a975:  48 F7 E9                 IMUL RCX
14069a978:  48 D1 FA                 SAR RDX,0x1
14069a97b:  48 8B C2                 MOV RAX,RDX
14069a97e:  48 C1 E8 3F              SHR RAX,0x3f
14069a982:  48 03 D0                 ADD RDX,RAX
14069a985:  41 39 97 68 07 00 00     CMP dword ptr [R15 + 0x768],EDX
14069a98c:  0F 8C 45 10 00 00        JL 0x14069b9d7
14069a992:  4D 39 AF 80 0C 00 00     CMP qword ptr [R15 + 0xc80],R13
14069a999:  0F 84 E6 00 00 00        JZ 0x14069aa85
14069a99f:  83 FF F6                 CMP EDI,-0xa
14069a9a2:  7F 14                    JG 0x14069a9b8
14069a9a4:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069a9ab:  4C 39 A8 D0 05 00 00     CMP qword ptr [RAX + 0x5d0],R13
14069a9b2:  0F 85 82 00 00 00        JNZ 0x14069aa3a
14069a9b8:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069a9bf:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069a9c6:  75 07                    JNZ 0x14069a9cf
14069a9c8:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069a9cf:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069a9d6:  4D 39 AF 28 05 00 00     CMP qword ptr [R15 + 0x528],R13
14069a9dd:  74 5B                    JZ 0x14069aa3a
14069a9df:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069a9e6:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069a9ed:  48 85 C9                 TEST RCX,RCX
14069a9f0:  74 48                    JZ 0x14069aa3a
14069a9f2:  44 38 A9 79 09 00 00     CMP byte ptr [RCX + 0x979],R13B
14069a9f9:  75 3F                    JNZ 0x14069aa3a
14069a9fb:  45 39 AF 10 05 00 00     CMP dword ptr [R15 + 0x510],R13D
14069aa02:  7D 09                    JGE 0x14069aa0d
14069aa04:  45 39 AF 6C 05 00 00     CMP dword ptr [R15 + 0x56c],R13D
14069aa0b:  7C 2D                    JL 0x14069aa3a
14069aa0d:  45 89 AF 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R13D
14069aa14:  4D 89 AF 98 07 00 00     MOV qword ptr [R15 + 0x798],R13
14069aa1b:  49 8B 87 C0 07 00 00     MOV RAX,qword ptr [R15 + 0x7c0]
14069aa22:  49 2B 87 B8 07 00 00     SUB RAX,qword ptr [R15 + 0x7b8]
14069aa29:  48 C1 F8 03              SAR RAX,0x3
14069aa2d:  48 85 C0                 TEST RAX,RAX
14069aa30:  74 08                    JZ 0x14069aa3a
14069aa32:  49 8B CF                 MOV RCX,R15
14069aa35:  E8 36 6F FE FF           CALL 0x140681970
14069aa3a:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069aa41:  48 85 C0                 TEST RAX,RAX
14069aa44:  74 3F                    JZ 0x14069aa85
14069aa46:  49 3B 87 F8 04 00 00     CMP RAX,qword ptr [R15 + 0x4f8]
14069aa4d:  75 36                    JNZ 0x14069aa85
14069aa4f:  45 39 AF 10 05 00 00     CMP dword ptr [R15 + 0x510],R13D
14069aa56:  7C 2D                    JL 0x14069aa85
14069aa58:  45 89 AF 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R13D
14069aa5f:  4D 89 AF 98 07 00 00     MOV qword ptr [R15 + 0x798],R13
14069aa66:  49 8B 87 C0 07 00 00     MOV RAX,qword ptr [R15 + 0x7c0]
14069aa6d:  49 2B 87 B8 07 00 00     SUB RAX,qword ptr [R15 + 0x7b8]
14069aa74:  48 C1 F8 03              SAR RAX,0x3
14069aa78:  48 85 C0                 TEST RAX,RAX
14069aa7b:  74 08                    JZ 0x14069aa85
14069aa7d:  49 8B CF                 MOV RCX,R15
14069aa80:  E8 EB 6E FE FF           CALL 0x140681970
14069aa85:  4D 39 AF 28 05 00 00     CMP qword ptr [R15 + 0x528],R13
14069aa8c:  75 09                    JNZ 0x14069aa97
14069aa8e:  4D 39 AF 38 05 00 00     CMP qword ptr [R15 + 0x538],R13
14069aa95:  74 07                    JZ 0x14069aa9e
14069aa97:  45 89 AF 24 0D 00 00     MOV dword ptr [R15 + 0xd24],R13D
14069aa9e:  40 32 F6                 XOR SIL,SIL
14069aaa1:  45 8B 8F 6C 05 00 00     MOV R9D,dword ptr [R15 + 0x56c]
14069aaa8:  41 83 F9 F6              CMP R9D,-0xa
14069aaac:  7F 51                    JG 0x14069aaff
14069aaae:  4D 39 AF 28 05 00 00     CMP qword ptr [R15 + 0x528],R13
14069aab5:  74 48                    JZ 0x14069aaff
14069aab7:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069aabe:  49 8B 88 18 06 00 00     MOV RCX,qword ptr [R8 + 0x618]
14069aac5:  49 2B 88 10 06 00 00     SUB RCX,qword ptr [R8 + 0x610]
14069aacc:  48 B8 DD 0B A2 D3 9C 6D 5E 1E MOV RAX,0x1e5e6d9cd3a20bdd
14069aad6:  48 F7 E9                 IMUL RCX
14069aad9:  48 C1 FA 0A              SAR RDX,0xa
14069aadd:  48 8B C2                 MOV RAX,RDX
14069aae0:  48 C1 E8 3F              SHR RAX,0x3f
14069aae4:  48 03 D0                 ADD RDX,RAX
14069aae7:  74 16                    JZ 0x14069aaff
14069aae9:  49 8B 80 10 06 00 00     MOV RAX,qword ptr [R8 + 0x610]
14069aaf0:  83 78 04 07              CMP dword ptr [RAX + 0x4],0x7
14069aaf4:  74 06                    JZ 0x14069aafc
14069aaf6:  83 78 04 11              CMP dword ptr [RAX + 0x4],0x11
14069aafa:  75 03                    JNZ 0x14069aaff
14069aafc:  40 B6 01                 MOV SIL,0x1
14069aaff:  40 32 FF                 XOR DIL,DIL
14069ab02:  41 83 F9 F6              CMP R9D,-0xa
14069ab06:  7F 69                    JG 0x14069ab71
14069ab08:  49 8B 9F 28 05 00 00     MOV RBX,qword ptr [R15 + 0x528]
14069ab0f:  4C 39 AB D0 05 00 00     CMP qword ptr [RBX + 0x5d0],R13
14069ab16:  74 4F                    JZ 0x14069ab67
14069ab18:  44 0F 2F 8B 04 06 00 00  COMISS XMM9,dword ptr [RBX + 0x604]
14069ab20:  76 45                    JBE 0x14069ab67
14069ab22:  48 8B 83 B0 0B 00 00     MOV RAX,qword ptr [RBX + 0xbb0]
14069ab29:  48 85 C0                 TEST RAX,RAX
14069ab2c:  74 39                    JZ 0x14069ab67
14069ab2e:  49 3B 87 90 03 00 00     CMP RAX,qword ptr [R15 + 0x390]
14069ab35:  74 30                    JZ 0x14069ab67
14069ab37:  48 8B D3                 MOV RDX,RBX
14069ab3a:  48 8D 0D CF A3 33 00     LEA RCX,[0x1409d4f10]
14069ab41:  E8 9A 32 AB FF           CALL 0x14014dde0
14069ab46:  85 C0                    TEST EAX,EAX
14069ab48:  78 1D                    JS 0x14069ab67
14069ab4a:  48 8B 93 10 06 00 00     MOV RDX,qword ptr [RBX + 0x610]
14069ab51:  48 98                    CDQE
14069ab53:  48 69 C8 B8 21 00 00     IMUL RCX,RAX,0x21b8
14069ab5a:  40 0F B6 FF              MOVZX EDI,DIL
14069ab5e:  83 7C 11 04 08           CMP dword ptr [RCX + RDX*0x1 + 0x4],0x8
14069ab63:  41 0F 44 FE              CMOVZ EDI,R14D
14069ab67:  48 BB AB AA AA AA AA AA AA 2A MOV RBX,0x2aaaaaaaaaaaaaab
14069ab71:  44 88 6C 24 38           MOV byte ptr [RSP + 0x38],R13B
14069ab76:  44 88 6C 24 30           MOV byte ptr [RSP + 0x30],R13B
14069ab7b:  44 88 6C 24 20           MOV byte ptr [RSP + 0x20],R13B
14069ab80:  45 33 C9                 XOR R9D,R9D
14069ab83:  45 33 C0                 XOR R8D,R8D
14069ab86:  33 D2                    XOR EDX,EDX
14069ab88:  49 8B CF                 MOV RCX,R15
14069ab8b:  E8 A0 07 FF FF           CALL 0x14068b330
14069ab90:  84 C0                    TEST AL,AL
14069ab92:  75 0D                    JNZ 0x14069aba1
14069ab94:  4D 39 AF 10 06 00 00     CMP qword ptr [R15 + 0x610],R13
14069ab9b:  0F 84 6C F5 FF FF        JZ 0x14069a10d
14069aba1:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069aba8:  48 8B 88 B0 0B 00 00     MOV RCX,qword ptr [RAX + 0xbb0]
14069abaf:  48 85 C9                 TEST RCX,RCX
14069abb2:  74 45                    JZ 0x14069abf9
14069abb4:  49 3B 8F 90 03 00 00     CMP RCX,qword ptr [R15 + 0x390]
14069abbb:  74 3C                    JZ 0x14069abf9
14069abbd:  40 84 FF                 TEST DIL,DIL
14069abc0:  75 37                    JNZ 0x14069abf9
14069abc2:  F3 0F 10 80 04 06 00 00  MOVSS XMM0,dword ptr [RAX + 0x604]
14069abca:  41 0F 2F C1              COMISS XMM0,XMM9
14069abce:  73 05                    JNC 0x14069abd5
14069abd0:  40 84 F6                 TEST SIL,SIL
14069abd3:  74 0A                    JZ 0x14069abdf
14069abd5:  41 83 BF 6C 05 00 00 F6  CMP dword ptr [R15 + 0x56c],-0xa
14069abdd:  7E 1A                    JLE 0x14069abf9
14069abdf:  49 3B 87 F8 04 00 00     CMP RAX,qword ptr [R15 + 0x4f8]
14069abe6:  0F 85 21 F5 FF FF        JNZ 0x14069a10d
14069abec:  45 39 AF 10 05 00 00     CMP dword ptr [R15 + 0x510],R13D
14069abf3:  0F 8C 14 F5 FF FF        JL 0x14069a10d
14069abf9:  F3 0F 10 35 6B FC 26 00  MOVSS XMM6,dword ptr [0x14090a86c]
14069ac01:  48 85 C0                 TEST RAX,RAX
14069ac04:  0F 84 E3 04 00 00        JZ 0x14069b0ed
14069ac0a:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069ac11:  48 85 C9                 TEST RCX,RCX
14069ac14:  0F 84 D3 04 00 00        JZ 0x14069b0ed
14069ac1a:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
14069ac21:  0F 85 C6 04 00 00        JNZ 0x14069b0ed
14069ac27:  45 0F 2F 97 0C 0D 00 00  COMISS XMM10,dword ptr [R15 + 0xd0c]
14069ac2f:  0F 82 B8 04 00 00        JC 0x14069b0ed
14069ac35:  41 83 BF 6C 05 00 00 00  CMP dword ptr [R15 + 0x56c],0x0
14069ac3d:  0F 8C AA 04 00 00        JL 0x14069b0ed
14069ac43:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069ac4b:  0F 85 9C 04 00 00        JNZ 0x14069b0ed
14069ac51:  80 B9 79 09 00 00 00     CMP byte ptr [RCX + 0x979],0x0
14069ac58:  0F 84 8F 04 00 00        JZ 0x14069b0ed
14069ac5e:  0F 57 C0                 XORPS XMM0,XMM0
14069ac61:  F3 0F 7F 45 10           MOVDQU xmmword ptr [RBP + 0x10],XMM0
14069ac66:  45 33 C9                 XOR R9D,R9D
14069ac69:  4C 89 4D 20              MOV qword ptr [RBP + 0x20],R9
14069ac6d:  45 8B E1                 MOV R12D,R9D
14069ac70:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069ac77:  49 81 C0 10 0A 00 00     ADD R8,0xa10
14069ac7e:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
14069ac82:  49 2B 08                 SUB RCX,qword ptr [R8]
14069ac85:  48 8B C3                 MOV RAX,RBX
14069ac88:  48 F7 E9                 IMUL RCX
14069ac8b:  48 C1 FA 04              SAR RDX,0x4
14069ac8f:  48 8B C2                 MOV RAX,RDX
14069ac92:  48 C1 E8 3F              SHR RAX,0x3f
14069ac96:  48 03 D0                 ADD RDX,RAX
14069ac99:  48 8B 5D 18              MOV RBX,qword ptr [RBP + 0x18]
14069ac9d:  0F 84 4D 01 00 00        JZ 0x14069adf0
14069aca3:  41 8B C1                 MOV EAX,R9D
14069aca6:  45 8D 69 01              LEA R13D,[R9 + 0x1]
14069acaa:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
14069acb0:  48 8D 34 40              LEA RSI,[RAX + RAX*0x2]
14069acb4:  48 C1 E6 05              SHL RSI,0x5
14069acb8:  49 8B 00                 MOV RAX,qword ptr [R8]
14069acbb:  48 83 7C 30 28 00        CMP qword ptr [RAX + RSI*0x1 + 0x28],0x0
14069acc1:  0F 84 E7 00 00 00        JZ 0x14069adae
14069acc7:  48 8B 74 06 28           MOV RSI,qword ptr [RSI + RAX*0x1 + 0x28]
14069accc:  83 BE 2C 01 00 00 00     CMP dword ptr [RSI + 0x12c],0x0
14069acd3:  0F 85 D5 00 00 00        JNZ 0x14069adae
14069acd9:  45 8B F1                 MOV R14D,R9D
14069acdc:  48 8B 46 40              MOV RAX,qword ptr [RSI + 0x40]
14069ace0:  48 2B 46 38              SUB RAX,qword ptr [RSI + 0x38]
14069ace4:  48 C1 F8 03              SAR RAX,0x3
14069ace8:  48 85 C0                 TEST RAX,RAX
14069aceb:  0F 84 BD 00 00 00        JZ 0x14069adae
14069acf1:  49 8B F9                 MOV RDI,R9
14069acf4:  0F 1F 40 00              NOP dword ptr [RAX]
14069acf8:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
14069ad00:  48 8B 4E 38              MOV RCX,qword ptr [RSI + 0x38]
14069ad04:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069ad0b:  48 39 04 F9              CMP qword ptr [RCX + RDI*0x8],RAX
14069ad0f:  74 7F                    JZ 0x14069ad90
14069ad11:  48 8B 14 F9              MOV RDX,qword ptr [RCX + RDI*0x8]
14069ad15:  49 8B CF                 MOV RCX,R15
14069ad18:  E8 F3 E4 FF FF           CALL 0x140699210
14069ad1d:  84 C0                    TEST AL,AL
14069ad1f:  75 6F                    JNZ 0x14069ad90
14069ad21:  48 8B 46 38              MOV RAX,qword ptr [RSI + 0x38]
14069ad25:  48 8D 3C F8              LEA RDI,[RAX + RDI*0x8]
14069ad29:  48 3B FB                 CMP RDI,RBX
14069ad2c:  73 35                    JNC 0x14069ad63
14069ad2e:  48 8B 4D 10              MOV RCX,qword ptr [RBP + 0x10]
14069ad32:  48 3B CF                 CMP RCX,RDI
14069ad35:  77 2C                    JA 0x14069ad63
14069ad37:  48 2B F9                 SUB RDI,RCX
14069ad3a:  48 C1 FF 03              SAR RDI,0x3
14069ad3e:  48 3B 5D 20              CMP RBX,qword ptr [RBP + 0x20]
14069ad42:  75 10                    JNZ 0x14069ad54
14069ad44:  49 8B D5                 MOV RDX,R13
14069ad47:  48 8D 4D 10              LEA RCX,[RBP + 0x10]
14069ad4b:  E8 B0 69 A1 FF           CALL 0x1400b1700
14069ad50:  48 8B 5D 18              MOV RBX,qword ptr [RBP + 0x18]
14069ad54:  48 8B 45 10              MOV RAX,qword ptr [RBP + 0x10]
14069ad58:  48 85 DB                 TEST RBX,RBX
14069ad5b:  74 2B                    JZ 0x14069ad88
14069ad5d:  48 8B 04 F8              MOV RAX,qword ptr [RAX + RDI*0x8]
14069ad61:  EB 1E                    JMP 0x14069ad81
14069ad63:  48 3B 5D 20              CMP RBX,qword ptr [RBP + 0x20]
14069ad67:  75 10                    JNZ 0x14069ad79
14069ad69:  49 8B D5                 MOV RDX,R13
14069ad6c:  48 8D 4D 10              LEA RCX,[RBP + 0x10]
14069ad70:  E8 8B 69 A1 FF           CALL 0x1400b1700
14069ad75:  48 8B 5D 18              MOV RBX,qword ptr [RBP + 0x18]
14069ad79:  48 85 DB                 TEST RBX,RBX
14069ad7c:  74 0A                    JZ 0x14069ad88
14069ad7e:  48 8B 07                 MOV RAX,qword ptr [RDI]
14069ad81:  48 89 03                 MOV qword ptr [RBX],RAX
14069ad84:  48 8B 5D 18              MOV RBX,qword ptr [RBP + 0x18]
14069ad88:  48 83 C3 08              ADD RBX,0x8
14069ad8c:  48 89 5D 18              MOV qword ptr [RBP + 0x18],RBX
14069ad90:  41 FF C6                 INC R14D
14069ad93:  48 8B 46 40              MOV RAX,qword ptr [RSI + 0x40]
14069ad97:  48 2B 46 38              SUB RAX,qword ptr [RSI + 0x38]
14069ad9b:  48 C1 F8 03              SAR RAX,0x3
14069ad9f:  41 8B FE                 MOV EDI,R14D
14069ada2:  48 3B F8                 CMP RDI,RAX
14069ada5:  0F 82 55 FF FF FF        JC 0x14069ad00
14069adab:  45 33 C9                 XOR R9D,R9D
14069adae:  41 FF C4                 INC R12D
14069adb1:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069adb8:  49 81 C0 10 0A 00 00     ADD R8,0xa10
14069adbf:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
14069adc3:  49 2B 08                 SUB RCX,qword ptr [R8]
14069adc6:  48 B8 AB AA AA AA AA AA AA 2A MOV RAX,0x2aaaaaaaaaaaaaab
14069add0:  48 F7 E9                 IMUL RCX
14069add3:  48 C1 FA 04              SAR RDX,0x4
14069add7:  48 8B C2                 MOV RAX,RDX
14069adda:  48 C1 E8 3F              SHR RAX,0x3f
14069adde:  48 03 D0                 ADD RDX,RAX
14069ade1:  41 8B C4                 MOV EAX,R12D
14069ade4:  48 3B C2                 CMP RAX,RDX
14069ade7:  0F 82 C3 FE FF FF        JC 0x14069acb0
14069aded:  45 32 ED                 XOR R13B,R13B
14069adf0:  45 8B F1                 MOV R14D,R9D
14069adf3:  48 2B 5D 10              SUB RBX,qword ptr [RBP + 0x10]
14069adf7:  48 C1 FB 03              SAR RBX,0x3
14069adfb:  48 85 DB                 TEST RBX,RBX
14069adfe:  0F 84 F7 01 00 00        JZ 0x14069affb
14069ae04:  49 8B C9                 MOV RCX,R9
14069ae07:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069ae10:  48 8B 45 10              MOV RAX,qword ptr [RBP + 0x10]
14069ae14:  48 8B 34 C8              MOV RSI,qword ptr [RAX + RCX*0x8]
14069ae18:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069ae1c:  41 0F 28 D1              MOVAPS XMM2,XMM9
14069ae20:  41 0F 28 CA              MOVAPS XMM1,XMM10
14069ae24:  48 8D 8D 58 01 00 00     LEA RCX,[RBP + 0x158]
14069ae2b:  FF 15 D7 11 1D 00        CALL qword ptr [0x14086c008]
14069ae31:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069ae38:  F3 0F 10 98 7C 7A 00 00  MOVSS XMM3,dword ptr [RAX + 0x7a7c]
14069ae40:  F3 0F 58 DE              ADDSS XMM3,XMM6
14069ae44:  41 0F 28 D1              MOVAPS XMM2,XMM9
14069ae48:  41 0F 28 CA              MOVAPS XMM1,XMM10
14069ae4c:  48 8D 8D 48 01 00 00     LEA RCX,[RBP + 0x148]
14069ae53:  FF 15 AF 11 1D 00        CALL qword ptr [0x14086c008]
14069ae59:  48 8D 95 70 0D 00 00     LEA RDX,[RBP + 0xd70]
14069ae60:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069ae67:  FF 15 53 19 1D 00        CALL qword ptr [0x14086c7c0]
14069ae6d:  4C 8B C0                 MOV R8,RAX
14069ae70:  48 8D 95 58 01 00 00     LEA RDX,[RBP + 0x158]
14069ae77:  48 8D 8D 58 01 00 00     LEA RCX,[RBP + 0x158]
14069ae7e:  FF 15 84 17 1D 00        CALL qword ptr [0x14086c608]
14069ae84:  48 8D 95 B0 0D 00 00     LEA RDX,[RBP + 0xdb0]
14069ae8b:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069ae92:  FF 15 28 19 1D 00        CALL qword ptr [0x14086c7c0]
14069ae98:  4C 8B C0                 MOV R8,RAX
14069ae9b:  48 8D 95 48 01 00 00     LEA RDX,[RBP + 0x148]
14069aea2:  48 8D 8D 48 01 00 00     LEA RCX,[RBP + 0x148]
14069aea9:  FF 15 59 17 1D 00        CALL qword ptr [0x14086c608]
14069aeaf:  48 8D 95 48 01 00 00     LEA RDX,[RBP + 0x148]
14069aeb6:  48 8D 8D 20 0B 00 00     LEA RCX,[RBP + 0xb20]
14069aebd:  FF 15 4D 11 1D 00        CALL qword ptr [0x14086c010]
14069aec3:  48 8B F8                 MOV RDI,RAX
14069aec6:  48 8D 95 58 01 00 00     LEA RDX,[RBP + 0x158]
14069aecd:  48 8D 8D 30 0B 00 00     LEA RCX,[RBP + 0xb30]
14069aed4:  FF 15 36 11 1D 00        CALL qword ptr [0x14086c010]
14069aeda:  48 8B D8                 MOV RBX,RAX
14069aedd:  48 8D 8E 60 0D 00 00     LEA RCX,[RSI + 0xd60]
14069aee4:  48 8D 95 F0 0D 00 00     LEA RDX,[RBP + 0xdf0]
14069aeeb:  FF 15 07 1B 1D 00        CALL qword ptr [0x14086c9f8]
14069aef1:  48 8B D0                 MOV RDX,RAX
14069aef4:  48 8B 8E 08 17 00 00     MOV RCX,qword ptr [RSI + 0x1708]
14069aefb:  F3 44 0F 11 4C 24 28     MOVSS dword ptr [RSP + 0x28],XMM9
14069af02:  C7 44 24 20 FF FF FF FF  MOV dword ptr [RSP + 0x20],0xffffffff
14069af0a:  4C 8B CF                 MOV R9,RDI
14069af0d:  4C 8B C3                 MOV R8,RBX
14069af10:  48 8B 89 98 02 00 00     MOV RCX,qword ptr [RCX + 0x298]
14069af17:  FF 15 23 21 1D 00        CALL qword ptr [0x14086d040]
14069af1d:  84 C0                    TEST AL,AL
14069af1f:  0F 85 9F 00 00 00        JNZ 0x14069afc4
14069af25:  48 8B 86 08 17 00 00     MOV RAX,qword ptr [RSI + 0x1708]
14069af2c:  48 83 B8 A0 02 00 00 00  CMP qword ptr [RAX + 0x2a0],0x0
14069af34:  74 72                    JZ 0x14069afa8
14069af36:  48 8D 95 48 01 00 00     LEA RDX,[RBP + 0x148]
14069af3d:  48 8D 8D 20 04 00 00     LEA RCX,[RBP + 0x420]
14069af44:  FF 15 C6 10 1D 00        CALL qword ptr [0x14086c010]
14069af4a:  48 8B F8                 MOV RDI,RAX
14069af4d:  48 8D 95 58 01 00 00     LEA RDX,[RBP + 0x158]
14069af54:  48 8D 8D 30 04 00 00     LEA RCX,[RBP + 0x430]
14069af5b:  FF 15 AF 10 1D 00        CALL qword ptr [0x14086c010]
14069af61:  48 8B D8                 MOV RBX,RAX
14069af64:  48 8D 8E D8 0E 00 00     LEA RCX,[RSI + 0xed8]
14069af6b:  48 8D 95 30 0E 00 00     LEA RDX,[RBP + 0xe30]
14069af72:  FF 15 80 1A 1D 00        CALL qword ptr [0x14086c9f8]
14069af78:  48 8B D0                 MOV RDX,RAX
14069af7b:  48 8B 8E 08 17 00 00     MOV RCX,qword ptr [RSI + 0x1708]
14069af82:  F3 44 0F 11 4C 24 28     MOVSS dword ptr [RSP + 0x28],XMM9
14069af89:  C7 44 24 20 FF FF FF FF  MOV dword ptr [RSP + 0x20],0xffffffff
14069af91:  4C 8B CF                 MOV R9,RDI
14069af94:  4C 8B C3                 MOV R8,RBX
14069af97:  48 8B 89 A0 02 00 00     MOV RCX,qword ptr [RCX + 0x2a0]
14069af9e:  FF 15 9C 20 1D 00        CALL qword ptr [0x14086d040]
14069afa4:  84 C0                    TEST AL,AL
14069afa6:  75 1C                    JNZ 0x14069afc4
14069afa8:  41 FF C6                 INC R14D
14069afab:  48 8B 45 18              MOV RAX,qword ptr [RBP + 0x18]
14069afaf:  48 2B 45 10              SUB RAX,qword ptr [RBP + 0x10]
14069afb3:  48 C1 F8 03              SAR RAX,0x3
14069afb7:  41 8B CE                 MOV ECX,R14D
14069afba:  48 3B C8                 CMP RCX,RAX
14069afbd:  73 3C                    JNC 0x14069affb
14069afbf:  E9 4C FE FF FF           JMP 0x14069ae10
14069afc4:  41 8B 87 6C 05 00 00     MOV EAX,dword ptr [R15 + 0x56c]
14069afcb:  FF C0                    INC EAX
14069afcd:  66 0F 6E C0              MOVD XMM0,EAX
14069afd1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069afd4:  0F 28 C8                 MOVAPS XMM1,XMM0
14069afd7:  F3 44 0F 10 05 64 F4 26 00 MOVSS XMM8,dword ptr [0x14090a444]
14069afe0:  F3 41 0F 59 CF           MULSS XMM1,XMM15
14069afe5:  F3 41 0F 59 C0           MULSS XMM0,XMM8
14069afea:  FF 15 88 1A 1D 00        CALL qword ptr [0x14086ca78]
14069aff0:  F3 41 0F 11 87 0C 0D 00 00 MOVSS dword ptr [R15 + 0xd0c],XMM0
14069aff9:  EB 09                    JMP 0x14069b004
14069affb:  F3 44 0F 10 05 40 F4 26 00 MOVSS XMM8,dword ptr [0x14090a444]
14069b004:  45 0F 2F 97 0C 0D 00 00  COMISS XMM10,dword ptr [R15 + 0xd0c]
14069b00c:  0F 86 AB 00 00 00        JBE 0x14069b0bd
14069b012:  41 83 BF 6C 05 00 00 00  CMP dword ptr [R15 + 0x56c],0x0
14069b01a:  0F 8E 9D 00 00 00        JLE 0x14069b0bd
14069b020:  BB 07 00 00 00           MOV EBX,0x7
14069b025:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b02c:  48 83 B8 78 0A 00 00 00  CMP qword ptr [RAX + 0xa78],0x0
14069b034:  74 24                    JZ 0x14069b05a
14069b036:  48 8B 88 78 0A 00 00     MOV RCX,qword ptr [RAX + 0xa78]
14069b03d:  F3 0F 10 81 0C 0D 00 00  MOVSS XMM0,dword ptr [RCX + 0xd0c]
14069b045:  41 0F 2F C2              COMISS XMM0,XMM10
14069b049:  76 0F                    JBE 0x14069b05a
14069b04b:  BA 05 00 00 00           MOV EDX,0x5
14069b050:  83 B9 94 05 00 00 02     CMP dword ptr [RCX + 0x594],0x2
14069b057:  0F 44 DA                 CMOVZ EBX,EDX
14069b05a:  48 83 B8 80 0A 00 00 00  CMP qword ptr [RAX + 0xa80],0x0
14069b062:  74 21                    JZ 0x14069b085
14069b064:  48 8B 88 80 0A 00 00     MOV RCX,qword ptr [RAX + 0xa80]
14069b06b:  F3 0F 10 81 0C 0D 00 00  MOVSS XMM0,dword ptr [RCX + 0xd0c]
14069b073:  41 0F 2F C2              COMISS XMM0,XMM10
14069b077:  76 0C                    JBE 0x14069b085
14069b079:  83 B9 94 05 00 00 02     CMP dword ptr [RCX + 0x594],0x2
14069b080:  75 03                    JNZ 0x14069b085
14069b082:  83 EB 02                 SUB EBX,0x2
14069b085:  FF 15 15 30 1D 00        CALL qword ptr [0x14086e0a0]
14069b08b:  99                       CDQ
14069b08c:  F7 FB                    IDIV EBX
14069b08e:  41 8B 87 6C 05 00 00     MOV EAX,dword ptr [R15 + 0x56c]
14069b095:  3B D0                    CMP EDX,EAX
14069b097:  7D 24                    JGE 0x14069b0bd
14069b099:  03 C0                    ADD EAX,EAX
14069b09b:  66 0F 6E C0              MOVD XMM0,EAX
14069b09f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069b0a2:  0F 28 C8                 MOVAPS XMM1,XMM0
14069b0a5:  F3 41 0F 59 C8           MULSS XMM1,XMM8
14069b0aa:  F3 0F 58 C0              ADDSS XMM0,XMM0
14069b0ae:  FF 15 C4 19 1D 00        CALL qword ptr [0x14086ca78]
14069b0b4:  F3 41 0F 11 87 0C 0D 00 00 MOVSS dword ptr [R15 + 0xd0c],XMM0
14069b0bd:  48 8B 4D 10              MOV RCX,qword ptr [RBP + 0x10]
14069b0c1:  48 85 C9                 TEST RCX,RCX
14069b0c4:  74 2C                    JZ 0x14069b0f2
14069b0c6:  48 8B 55 20              MOV RDX,qword ptr [RBP + 0x20]
14069b0ca:  48 2B D1                 SUB RDX,RCX
14069b0cd:  48 C1 FA 03              SAR RDX,0x3
14069b0d1:  41 B8 08 00 00 00        MOV R8D,0x8
14069b0d7:  E8 74 69 96 FF           CALL 0x140001a50
14069b0dc:  0F 57 C0                 XORPS XMM0,XMM0
14069b0df:  F3 0F 7F 45 10           MOVDQU xmmword ptr [RBP + 0x10],XMM0
14069b0e4:  45 33 F6                 XOR R14D,R14D
14069b0e7:  4C 89 75 20              MOV qword ptr [RBP + 0x20],R14
14069b0eb:  EB 08                    JMP 0x14069b0f5
14069b0ed:  45 33 F6                 XOR R14D,R14D
14069b0f0:  EB 0A                    JMP 0x14069b0fc
14069b0f2:  45 33 F6                 XOR R14D,R14D
14069b0f5:  4D 8D A7 38 07 00 00     LEA R12,[R15 + 0x738]
14069b0fc:  F3 41 0F 10 87 0C 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd0c]
14069b105:  41 0F 2F C2              COMISS XMM0,XMM10
14069b109:  76 09                    JBE 0x14069b114
14069b10b:  F3 0F 5C C7              SUBSS XMM0,XMM7
14069b10f:  E9 B7 08 00 00           JMP 0x14069b9cb
14069b114:  45 89 B7 0C 0D 00 00     MOV dword ptr [R15 + 0xd0c],R14D
14069b11b:  41 8B B7 98 06 00 00     MOV ESI,dword ptr [R15 + 0x698]
14069b122:  89 B5 A8 1C 00 00        MOV dword ptr [RBP + 0x1ca8],ESI
14069b128:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b12f:  49 39 87 F8 04 00 00     CMP qword ptr [R15 + 0x4f8],RAX
14069b136:  0F 84 D5 03 00 00        JZ 0x14069b511
14069b13c:  41 83 BF 10 05 00 00 00  CMP dword ptr [R15 + 0x510],0x0
14069b144:  7C 1E                    JL 0x14069b164
14069b146:  48 85 C0                 TEST RAX,RAX
14069b149:  74 19                    JZ 0x14069b164
14069b14b:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b152:  48 85 C9                 TEST RCX,RCX
14069b155:  74 0D                    JZ 0x14069b164
14069b157:  83 B9 60 03 00 00 60     CMP dword ptr [RCX + 0x360],0x60
14069b15e:  0F 84 AD 03 00 00        JZ 0x14069b511
14069b164:  4D 8B 87 10 06 00 00     MOV R8,qword ptr [R15 + 0x610]
14069b16b:  4D 85 C0                 TEST R8,R8
14069b16e:  74 51                    JZ 0x14069b1c1
14069b170:  41 8B CE                 MOV ECX,R14D
14069b173:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069b17a:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069b181:  48 C1 F8 03              SAR RAX,0x3
14069b185:  48 85 C0                 TEST RAX,RAX
14069b188:  74 37                    JZ 0x14069b1c1
14069b18a:  49 8B D6                 MOV RDX,R14
14069b18d:  0F 1F 00                 NOP dword ptr [RAX]
14069b190:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069b197:  4C 39 04 D0              CMP qword ptr [RAX + RDX*0x8],R8
14069b19b:  74 1D                    JZ 0x14069b1ba
14069b19d:  FF C1                    INC ECX
14069b19f:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069b1a6:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069b1ad:  48 C1 F8 03              SAR RAX,0x3
14069b1b1:  8B D1                    MOV EDX,ECX
14069b1b3:  48 3B D0                 CMP RDX,RAX
14069b1b6:  72 D8                    JC 0x14069b190
14069b1b8:  EB 07                    JMP 0x14069b1c1
14069b1ba:  41 89 8F 98 06 00 00     MOV dword ptr [R15 + 0x698],ECX
14069b1c1:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069b1c8:  49 39 8F E8 05 00 00     CMP qword ptr [R15 + 0x5e8],RCX
14069b1cf:  74 24                    JZ 0x14069b1f5
14069b1d1:  49 8B 87 10 06 00 00     MOV RAX,qword ptr [R15 + 0x610]
14069b1d8:  48 85 C0                 TEST RAX,RAX
14069b1db:  74 05                    JZ 0x14069b1e2
14069b1dd:  48 3B C1                 CMP RAX,RCX
14069b1e0:  75 13                    JNZ 0x14069b1f5
14069b1e2:  41 80 BF 7D 06 00 00 00  CMP byte ptr [R15 + 0x67d],0x0
14069b1ea:  0F 94 C2                 SETZ DL
14069b1ed:  49 8B CF                 MOV RCX,R15
14069b1f0:  E8 0B 29 FE FF           CALL 0x14067db00
14069b1f5:  4D 89 B7 10 06 00 00     MOV qword ptr [R15 + 0x610],R14
14069b1fc:  49 83 BF E8 05 00 00 00  CMP qword ptr [R15 + 0x5e8],0x0
14069b204:  0F 84 90 00 00 00        JZ 0x14069b29a
14069b20a:  4D 89 B7 E8 05 00 00     MOV qword ptr [R15 + 0x5e8],R14
14069b211:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069b218:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069b21f:  48 C1 F8 03              SAR RAX,0x3
14069b223:  48 85 C0                 TEST RAX,RAX
14069b226:  74 72                    JZ 0x14069b29a
14069b228:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069b22f:  48 83 38 00              CMP qword ptr [RAX],0x0
14069b233:  74 65                    JZ 0x14069b29a
14069b235:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069b238:  48 83 B9 18 03 00 00 00  CMP qword ptr [RCX + 0x318],0x0
14069b240:  74 58                    JZ 0x14069b29a
14069b242:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069b245:  48 8B 81 18 03 00 00     MOV RAX,qword ptr [RCX + 0x318]
14069b24c:  83 B8 60 03 00 00 0E     CMP dword ptr [RAX + 0x360],0xe
14069b253:  75 45                    JNZ 0x14069b29a
14069b255:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069b25c:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069b25f:  F3 0F 10 81 04 06 00 00  MOVSS XMM0,dword ptr [RCX + 0x604]
14069b267:  41 0F 2F C1              COMISS XMM0,XMM9
14069b26b:  72 2D                    JC 0x14069b29a
14069b26d:  48 8B D0                 MOV RDX,RAX
14069b270:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
14069b275:  45 33 C9                 XOR R9D,R9D
14069b278:  4D 8B 87 08 17 00 00     MOV R8,qword ptr [R15 + 0x1708]
14069b27f:  48 8B 10                 MOV RDX,qword ptr [RAX]
14069b282:  48 8D 0D 87 9C 33 00     LEA RCX,[0x1409d4f10]
14069b289:  E8 72 76 D4 FF           CALL 0x1403e2900
14069b28e:  84 C0                    TEST AL,AL
14069b290:  74 08                    JZ 0x14069b29a
14069b292:  41 C6 87 FD 05 00 00 01  MOV byte ptr [R15 + 0x5fd],0x1
14069b29a:  41 80 BF FD 05 00 00 00  CMP byte ptr [R15 + 0x5fd],0x0
14069b2a2:  0F 85 91 00 00 00        JNZ 0x14069b339
14069b2a8:  49 63 8F 98 06 00 00     MOVSXD RCX,dword ptr [R15 + 0x698]
14069b2af:  85 C9                    TEST ECX,ECX
14069b2b1:  0F 88 82 00 00 00        JS 0x14069b339
14069b2b7:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069b2be:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069b2c5:  48 C1 F8 03              SAR RAX,0x3
14069b2c9:  3B C8                    CMP ECX,EAX
14069b2cb:  7D 6C                    JGE 0x14069b339
14069b2cd:  48 8B D1                 MOV RDX,RCX
14069b2d0:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069b2d7:  48 83 3C C8 00           CMP qword ptr [RAX + RCX*0x8],0x0
14069b2dc:  74 5B                    JZ 0x14069b339
14069b2de:  48 8B C8                 MOV RCX,RAX
14069b2e1:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b2e8:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
14069b2ec:  75 4B                    JNZ 0x14069b339
14069b2ee:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069b2f5:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069b2fc:  45 33 ED                 XOR R13D,R13D
14069b2ff:  4D 89 AF 58 05 00 00     MOV qword ptr [R15 + 0x558],R13
14069b306:  4D 89 AF 94 05 00 00     MOV qword ptr [R15 + 0x594],R13
14069b30d:  41 C7 87 68 05 00 00 00 00 80 3F MOV dword ptr [R15 + 0x568],0x3f800000
14069b318:  45 89 AF 9C 17 00 00     MOV dword ptr [R15 + 0x179c],R13D
14069b31f:  4D 89 AF 60 05 00 00     MOV qword ptr [R15 + 0x560],R13
14069b326:  45 89 AF 10 13 00 00     MOV dword ptr [R15 + 0x1310],R13D
14069b32d:  45 88 AF 9C 05 00 00     MOV byte ptr [R15 + 0x59c],R13B
14069b334:  E9 9E 06 00 00           JMP 0x14069b9d7
14069b339:  41 C7 87 F8 05 00 00 00 00 20 41 MOV dword ptr [R15 + 0x5f8],0x41200000
14069b344:  83 CB FF                 OR EBX,0xffffffff
14069b347:  41 8B 87 6C 05 00 00     MOV EAX,dword ptr [R15 + 0x56c]
14069b34e:  83 F8 F6                 CMP EAX,-0xa
14069b351:  7F 07                    JG 0x14069b35a
14069b353:  BB F6 FF FF FF           MOV EBX,0xfffffff6
14069b358:  2B D8                    SUB EBX,EAX
14069b35a:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14069b35f:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14069b364:  44 8B C3                 MOV R8D,EBX
14069b367:  33 D2                    XOR EDX,EDX
14069b369:  49 8B CF                 MOV RCX,R15
14069b36c:  E8 AF 56 FE FF           CALL 0x140680a20
14069b371:  41 80 BF 39 0D 00 00 01  CMP byte ptr [R15 + 0xd39],0x1
14069b379:  75 7A                    JNZ 0x14069b3f5
14069b37b:  85 DB                    TEST EBX,EBX
14069b37d:  78 76                    JS 0x14069b3f5
14069b37f:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b386:  48 8B 90 D0 05 00 00     MOV RDX,qword ptr [RAX + 0x5d0]
14069b38d:  48 85 D2                 TEST RDX,RDX
14069b390:  74 63                    JZ 0x14069b3f5
14069b392:  48 BF AB AA AA AA AA AA AA 2A MOV RDI,0x2aaaaaaaaaaaaaab
14069b39c:  F7 82 2C 01 00 00 FB FF FF FF TEST dword ptr [RDX + 0x12c],0xfffffffb
14069b3a6:  75 57                    JNZ 0x14069b3ff
14069b3a8:  48 8B 4A 10              MOV RCX,qword ptr [RDX + 0x10]
14069b3ac:  48 2B 4A 08              SUB RCX,qword ptr [RDX + 0x8]
14069b3b0:  48 8B C7                 MOV RAX,RDI
14069b3b3:  48 F7 E9                 IMUL RCX
14069b3b6:  48 C1 FA 02              SAR RDX,0x2
14069b3ba:  48 8B C2                 MOV RAX,RDX
14069b3bd:  48 C1 E8 3F              SHR RAX,0x3f
14069b3c1:  48 03 D0                 ADD RDX,RAX
14069b3c4:  48 83 FA 05              CMP RDX,0x5
14069b3c8:  73 35                    JNC 0x14069b3ff
14069b3ca:  85 DB                    TEST EBX,EBX
14069b3cc:  75 07                    JNZ 0x14069b3d5
14069b3ce:  BB 01 00 00 00           MOV EBX,0x1
14069b3d3:  EB 07                    JMP 0x14069b3dc
14069b3d5:  83 FB 01                 CMP EBX,0x1
14069b3d8:  41 0F 44 DE              CMOVZ EBX,R14D
14069b3dc:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14069b3e1:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14069b3e6:  44 8B C3                 MOV R8D,EBX
14069b3e9:  33 D2                    XOR EDX,EDX
14069b3eb:  49 8B CF                 MOV RCX,R15
14069b3ee:  E8 2D 56 FE FF           CALL 0x140680a20
14069b3f3:  EB 0A                    JMP 0x14069b3ff
14069b3f5:  48 BF AB AA AA AA AA AA AA 2A MOV RDI,0x2aaaaaaaaaaaaaab
14069b3ff:  49 8B CF                 MOV RCX,R15
14069b402:  E8 69 91 01 00           CALL 0x1406b4570
14069b407:  45 8B 87 6C 05 00 00     MOV R8D,dword ptr [R15 + 0x56c]
14069b40e:  45 85 C0                 TEST R8D,R8D
14069b411:  78 33                    JS 0x14069b446
14069b413:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069b41b:  75 29                    JNZ 0x14069b446
14069b41d:  49 8B 97 28 05 00 00     MOV RDX,qword ptr [R15 + 0x528]
14069b424:  48 8B 82 18 03 00 00     MOV RAX,qword ptr [RDX + 0x318]
14069b42b:  48 85 C0                 TEST RAX,RAX
14069b42e:  74 16                    JZ 0x14069b446
14069b430:  80 B8 79 09 00 00 00     CMP byte ptr [RAX + 0x979],0x0
14069b437:  74 0D                    JZ 0x14069b446
14069b439:  49 8B CF                 MOV RCX,R15
14069b43c:  E8 1F 79 FE FF           CALL 0x140682d60
14069b441:  E9 8A 03 00 00           JMP 0x14069b7d0
14069b446:  32 C9                    XOR CL,CL
14069b448:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b44f:  48 8B 90 18 03 00 00     MOV RDX,qword ptr [RAX + 0x318]
14069b456:  48 85 D2                 TEST RDX,RDX
14069b459:  74 12                    JZ 0x14069b46d
14069b45b:  0F B6 C9                 MOVZX ECX,CL
14069b45e:  80 BA 78 09 00 00 00     CMP byte ptr [RDX + 0x978],0x0
14069b465:  B8 01 00 00 00           MOV EAX,0x1
14069b46a:  0F 45 C8                 CMOVNZ ECX,EAX
14069b46d:  45 85 C0                 TEST R8D,R8D
14069b470:  78 4D                    JS 0x14069b4bf
14069b472:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069b47a:  75 43                    JNZ 0x14069b4bf
14069b47c:  84 C9                    TEST CL,CL
14069b47e:  75 3F                    JNZ 0x14069b4bf
14069b480:  4D 8B 04 24              MOV R8,qword ptr [R12]
14069b484:  4C 89 85 F8 03 00 00     MOV qword ptr [RBP + 0x3f8],R8
14069b48b:  49 63 8F 6C 05 00 00     MOVSXD RCX,dword ptr [R15 + 0x56c]
14069b492:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b499:  48 C1 E1 05              SHL RCX,0x5
14069b49d:  48 8B 80 40 0A 00 00     MOV RAX,qword ptr [RAX + 0xa40]
14069b4a4:  48 83 C0 0C              ADD RAX,0xc
14069b4a8:  48 03 C8                 ADD RCX,RAX
14069b4ab:  48 89 4C 24 20           MOV qword ptr [RSP + 0x20],RCX
14069b4b0:  48 8D 95 A8 1C 00 00     LEA RDX,[RBP + 0x1ca8]
14069b4b7:  49 8B CC                 MOV RCX,R12
14069b4ba:  E8 91 71 BB FF           CALL 0x140252650
14069b4bf:  41 83 BF 6C 05 00 00 00  CMP dword ptr [R15 + 0x56c],0x0
14069b4c7:  0F 8D 03 03 00 00        JGE 0x14069b7d0
14069b4cd:  49 8B 8F 80 05 00 00     MOV RCX,qword ptr [R15 + 0x580]
14069b4d4:  49 2B 8F 78 05 00 00     SUB RCX,qword ptr [R15 + 0x578]
14069b4db:  48 8B C7                 MOV RAX,RDI
14069b4de:  48 F7 E9                 IMUL RCX
14069b4e1:  48 D1 FA                 SAR RDX,0x1
14069b4e4:  48 8B C2                 MOV RAX,RDX
14069b4e7:  48 C1 E8 3F              SHR RAX,0x3f
14069b4eb:  48 03 D0                 ADD RDX,RAX
14069b4ee:  49 63 87 90 05 00 00     MOVSXD RAX,dword ptr [R15 + 0x590]
14069b4f5:  49 8B CF                 MOV RCX,R15
14069b4f8:  48 3B C2                 CMP RAX,RDX
14069b4fb:  73 0A                    JNC 0x14069b507
14069b4fd:  E8 0E 77 FE FF           CALL 0x140682c10
14069b502:  E9 C9 02 00 00           JMP 0x14069b7d0
14069b507:  E8 24 1F 01 00           CALL 0x1406ad430
14069b50c:  E9 BF 02 00 00           JMP 0x14069b7d0
14069b511:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b518:  49 89 87 F0 04 00 00     MOV qword ptr [R15 + 0x4f0],RAX
14069b51f:  49 83 BF E8 05 00 00 00  CMP qword ptr [R15 + 0x5e8],0x0
14069b527:  74 0A                    JZ 0x14069b533
14069b529:  4D 89 B7 E8 05 00 00     MOV qword ptr [R15 + 0x5e8],R14
14069b530:  41 B5 01                 MOV R13B,0x1
14069b533:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069b53b:  74 1E                    JZ 0x14069b55b
14069b53d:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069b544:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069b54b:  75 07                    JNZ 0x14069b554
14069b54d:  4C 89 B0 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R14
14069b554:  4D 89 B7 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R14
14069b55b:  41 83 BF 10 05 00 00 FF  CMP dword ptr [R15 + 0x510],-0x1
14069b563:  0F 85 67 02 00 00        JNZ 0x14069b7d0
14069b569:  41 8B CE                 MOV ECX,R14D
14069b56c:  49 8B 97 F0 04 00 00     MOV RDX,qword ptr [R15 + 0x4f0]
14069b573:  0F 1F 40 00              NOP dword ptr [RAX]
14069b577:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069b580:  44 8B C1                 MOV R8D,ECX
14069b583:  41 80 BC 10 90 0B 00 00 00 CMP byte ptr [R8 + RDX*0x1 + 0xb90],0x0
14069b58c:  75 17                    JNZ 0x14069b5a5
14069b58e:  48 8B 82 80 0B 00 00     MOV RAX,qword ptr [RDX + 0xb80]
14069b595:  48 2B 82 78 0B 00 00     SUB RAX,qword ptr [RDX + 0xb78]
14069b59c:  48 C1 F8 05              SAR RAX,0x5
14069b5a0:  49 3B C0                 CMP RAX,R8
14069b5a3:  77 0D                    JA 0x14069b5b2
14069b5a5:  FF C1                    INC ECX
14069b5a7:  83 F9 20                 CMP ECX,0x20
14069b5aa:  0F 83 20 02 00 00        JNC 0x14069b7d0
14069b5b0:  EB CE                    JMP 0x14069b580
14069b5b2:  8B F9                    MOV EDI,ECX
14069b5b4:  C6 84 17 90 0B 00 00 01  MOV byte ptr [RDI + RDX*0x1 + 0xb90],0x1
14069b5bc:  41 89 8F 10 05 00 00     MOV dword ptr [R15 + 0x510],ECX
14069b5c3:  45 89 B7 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R14D
14069b5ca:  4D 89 B7 98 07 00 00     MOV qword ptr [R15 + 0x798],R14
14069b5d1:  49 8B 87 C0 07 00 00     MOV RAX,qword ptr [R15 + 0x7c0]
14069b5d8:  49 2B 87 B8 07 00 00     SUB RAX,qword ptr [R15 + 0x7b8]
14069b5df:  48 C1 F8 03              SAR RAX,0x3
14069b5e3:  48 85 C0                 TEST RAX,RAX
14069b5e6:  74 08                    JZ 0x14069b5f0
14069b5e8:  49 8B CF                 MOV RCX,R15
14069b5eb:  E8 80 63 FE FF           CALL 0x140681970
14069b5f0:  49 8B 87 A0 06 00 00     MOV RAX,qword ptr [R15 + 0x6a0]
14069b5f7:  49 89 87 A8 06 00 00     MOV qword ptr [R15 + 0x6a8],RAX
14069b5fe:  49 8B 87 B8 06 00 00     MOV RAX,qword ptr [R15 + 0x6b8]
14069b605:  49 89 87 C0 06 00 00     MOV qword ptr [R15 + 0x6c0],RAX
14069b60c:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069b610:  49 89 44 24 08           MOV qword ptr [R12 + 0x8],RAX
14069b615:  49 8B 87 50 07 00 00     MOV RAX,qword ptr [R15 + 0x750]
14069b61c:  49 89 87 58 07 00 00     MOV qword ptr [R15 + 0x758],RAX
14069b623:  45 89 B7 68 07 00 00     MOV dword ptr [R15 + 0x768],R14D
14069b62a:  41 C6 87 6C 07 00 00 00  MOV byte ptr [R15 + 0x76c],0x0
14069b632:  45 89 B7 70 07 00 00     MOV dword ptr [R15 + 0x770],R14D
14069b639:  49 63 87 6C 05 00 00     MOVSXD RAX,dword ptr [R15 + 0x56c]
14069b640:  85 C0                    TEST EAX,EAX
14069b642:  78 24                    JS 0x14069b668
14069b644:  48 8B C8                 MOV RCX,RAX
14069b647:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b64e:  48 C1 E1 05              SHL RCX,0x5
14069b652:  48 8B 90 40 0A 00 00     MOV RDX,qword ptr [RAX + 0xa40]
14069b659:  48 83 C1 0C              ADD RCX,0xc
14069b65d:  48 03 D1                 ADD RDX,RCX
14069b660:  49 8B CC                 MOV RCX,R12
14069b663:  E8 88 3C 99 FF           CALL 0x14002f2f0
14069b668:  41 C6 87 39 0D 00 00 00  MOV byte ptr [R15 + 0xd39],0x0
14069b670:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b677:  4C 8B 80 18 03 00 00     MOV R8,qword ptr [RAX + 0x318]
14069b67e:  4D 85 C0                 TEST R8,R8
14069b681:  0F 84 07 01 00 00        JZ 0x14069b78e
14069b687:  41 8B DE                 MOV EBX,R14D
14069b68a:  49 8B 88 D0 02 00 00     MOV RCX,qword ptr [R8 + 0x2d0]
14069b691:  49 2B 88 C8 02 00 00     SUB RCX,qword ptr [R8 + 0x2c8]
14069b698:  48 B8 AB AA AA AA AA AA AA 2A MOV RAX,0x2aaaaaaaaaaaaaab
14069b6a2:  48 F7 E9                 IMUL RCX
14069b6a5:  48 D1 FA                 SAR RDX,0x1
14069b6a8:  48 8B C2                 MOV RAX,RDX
14069b6ab:  48 C1 E8 3F              SHR RAX,0x3f
14069b6af:  48 03 D0                 ADD RDX,RAX
14069b6b2:  0F 84 D6 00 00 00        JZ 0x14069b78e
14069b6b8:  4D 8B CE                 MOV R9,R14
14069b6bb:  48 BE AB AA AA AA AA AA AA 2A MOV RSI,0x2aaaaaaaaaaaaaab
14069b6c5:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069b6d0:  49 8B 80 E0 02 00 00     MOV RAX,qword ptr [R8 + 0x2e0]
14069b6d7:  42 83 3C 88 FF           CMP dword ptr [RAX + R9*0x4],-0x1
14069b6dc:  74 10                    JZ 0x14069b6ee
14069b6de:  48 8B C8                 MOV RCX,RAX
14069b6e1:  41 8B 87 10 05 00 00     MOV EAX,dword ptr [R15 + 0x510]
14069b6e8:  42 39 04 89              CMP dword ptr [RCX + R9*0x4],EAX
14069b6ec:  75 5D                    JNZ 0x14069b74b
14069b6ee:  4B 8D 0C 49              LEA RCX,[R9 + R9*0x2]
14069b6f2:  49 8B 80 C8 02 00 00     MOV RAX,qword ptr [R8 + 0x2c8]
14069b6f9:  48 8D 14 88              LEA RDX,[RAX + RCX*0x4]
14069b6fd:  48 8D 8D 00 02 00 00     LEA RCX,[RBP + 0x200]
14069b704:  FF 15 06 09 1D 00        CALL qword ptr [0x14086c010]
14069b70a:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069b711:  48 81 C1 20 03 00 00     ADD RCX,0x320
14069b718:  48 8D 95 70 0E 00 00     LEA RDX,[RBP + 0xe70]
14069b71f:  FF 15 9B 10 1D 00        CALL qword ptr [0x14086c7c0]
14069b725:  4C 8B C0                 MOV R8,RAX
14069b728:  48 8D 95 00 02 00 00     LEA RDX,[RBP + 0x200]
14069b72f:  48 8D 8D 00 02 00 00     LEA RCX,[RBP + 0x200]
14069b736:  FF 15 CC 0E 1D 00        CALL qword ptr [0x14086c608]
14069b73c:  48 8D 95 00 02 00 00     LEA RDX,[RBP + 0x200]
14069b743:  49 8B CC                 MOV RCX,R12
14069b746:  E8 A5 3B 99 FF           CALL 0x14002f2f0
14069b74b:  FF C3                    INC EBX
14069b74d:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b754:  4C 8B 80 18 03 00 00     MOV R8,qword ptr [RAX + 0x318]
14069b75b:  49 8B 88 D0 02 00 00     MOV RCX,qword ptr [R8 + 0x2d0]
14069b762:  49 2B 88 C8 02 00 00     SUB RCX,qword ptr [R8 + 0x2c8]
14069b769:  48 8B C6                 MOV RAX,RSI
14069b76c:  48 F7 E9                 IMUL RCX
14069b76f:  48 D1 FA                 SAR RDX,0x1
14069b772:  48 8B C2                 MOV RAX,RDX
14069b775:  48 C1 E8 3F              SHR RAX,0x3f
14069b779:  48 03 D0                 ADD RDX,RAX
14069b77c:  44 8B CB                 MOV R9D,EBX
14069b77f:  4C 3B CA                 CMP R9,RDX
14069b782:  0F 82 48 FF FF FF        JC 0x14069b6d0
14069b788:  8B B5 A8 1C 00 00        MOV ESI,dword ptr [RBP + 0x1ca8]
14069b78e:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b795:  48 C1 E7 05              SHL RDI,0x5
14069b799:  48 8B 90 78 0B 00 00     MOV RDX,qword ptr [RAX + 0xb78]
14069b7a0:  48 83 C2 0C              ADD RDX,0xc
14069b7a4:  48 03 D7                 ADD RDX,RDI
14069b7a7:  49 8B CC                 MOV RCX,R12
14069b7aa:  E8 41 3B 99 FF           CALL 0x14002f2f0
14069b7af:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b7b6:  48 8B 90 78 0B 00 00     MOV RDX,qword ptr [RAX + 0xb78]
14069b7bd:  48 03 D7                 ADD RDX,RDI
14069b7c0:  49 8B CC                 MOV RCX,R12
14069b7c3:  E8 28 3B 99 FF           CALL 0x14002f2f0
14069b7c8:  49 8B CF                 MOV RCX,R15
14069b7cb:  E8 B0 D4 01 00           CALL 0x1406b8c80
14069b7d0:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069b7d8:  0F 85 D5 01 00 00        JNZ 0x14069b9b3
14069b7de:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069b7e5:  48 85 C0                 TEST RAX,RAX
14069b7e8:  74 23                    JZ 0x14069b80d
14069b7ea:  49 3B 87 28 05 00 00     CMP RAX,qword ptr [R15 + 0x528]
14069b7f1:  74 1A                    JZ 0x14069b80d
14069b7f3:  48 8B C8                 MOV RCX,RAX
14069b7f6:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069b7fd:  48 39 81 B0 0B 00 00     CMP qword ptr [RCX + 0xbb0],RAX
14069b804:  75 07                    JNZ 0x14069b80d
14069b806:  4C 89 B1 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],R14
14069b80d:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069b814:  48 8B 81 18 03 00 00     MOV RAX,qword ptr [RCX + 0x318]
14069b81b:  48 85 C0                 TEST RAX,RAX
14069b81e:  74 09                    JZ 0x14069b829
14069b820:  80 B8 79 09 00 00 00     CMP byte ptr [RAX + 0x979],0x0
14069b827:  75 40                    JNZ 0x14069b869
14069b829:  41 8B 87 6C 05 00 00     MOV EAX,dword ptr [R15 + 0x56c]
14069b830:  85 C0                    TEST EAX,EAX
14069b832:  79 19                    JNS 0x14069b84d
14069b834:  83 F8 F6                 CMP EAX,-0xa
14069b837:  7F 30                    JG 0x14069b869
14069b839:  48 83 B9 D0 05 00 00 00  CMP qword ptr [RCX + 0x5d0],0x0
14069b841:  74 26                    JZ 0x14069b869
14069b843:  48 83 B9 B0 0B 00 00 00  CMP qword ptr [RCX + 0xbb0],0x0
14069b84b:  75 1C                    JNZ 0x14069b869
14069b84d:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069b854:  48 89 81 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],RAX
14069b85b:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b862:  49 89 87 80 0C 00 00     MOV qword ptr [R15 + 0xc80],RAX
14069b869:  49 63 87 6C 05 00 00     MOVSXD RAX,dword ptr [R15 + 0x56c]
14069b870:  85 C0                    TEST EAX,EAX
14069b872:  78 28                    JS 0x14069b89c
14069b874:  48 8B C8                 MOV RCX,RAX
14069b877:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b87e:  C6 84 01 58 0A 00 00 00  MOV byte ptr [RCX + RAX*0x1 + 0xa58],0x0
14069b886:  49 63 8F 6C 05 00 00     MOVSXD RCX,dword ptr [R15 + 0x56c]
14069b88d:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b894:  4C 89 B4 C8 78 0A 00 00  MOV qword ptr [RAX + RCX*0x8 + 0xa78],R14
14069b89c:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069b8a3:  49 39 87 28 05 00 00     CMP qword ptr [R15 + 0x528],RAX
14069b8aa:  75 74                    JNZ 0x14069b920
14069b8ac:  49 3B 87 F0 04 00 00     CMP RAX,qword ptr [R15 + 0x4f0]
14069b8b3:  75 6B                    JNZ 0x14069b920
14069b8b5:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b8bc:  48 85 C9                 TEST RCX,RCX
14069b8bf:  74 1D                    JZ 0x14069b8de
14069b8c1:  8B 81 60 03 00 00        MOV EAX,dword ptr [RCX + 0x360]
14069b8c7:  83 E8 2B                 SUB EAX,0x2b
14069b8ca:  A9 FD FF FF FF           TEST EAX,0xfffffffd
14069b8cf:  75 0D                    JNZ 0x14069b8de
14069b8d1:  41 B0 01                 MOV R8B,0x1
14069b8d4:  33 D2                    XOR EDX,EDX
14069b8d6:  49 8B CF                 MOV RCX,R15
14069b8d9:  E8 52 12 FE FF           CALL 0x14067cb30
14069b8de:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b8e5:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b8ec:  48 85 C9                 TEST RCX,RCX
14069b8ef:  74 16                    JZ 0x14069b907
14069b8f1:  80 B9 79 09 00 00 00     CMP byte ptr [RCX + 0x979],0x0
14069b8f8:  75 0D                    JNZ 0x14069b907
14069b8fa:  45 84 ED                 TEST R13B,R13B
14069b8fd:  75 08                    JNZ 0x14069b907
14069b8ff:  49 8B CF                 MOV RCX,R15
14069b902:  E8 79 1E 01 00           CALL 0x1406ad780
14069b907:  41 B1 01                 MOV R9B,0x1
14069b90a:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069b911:  41 8B 97 10 05 00 00     MOV EDX,dword ptr [R15 + 0x510]
14069b918:  49 8B CF                 MOV RCX,R15
14069b91b:  E8 C0 B7 FC FF           CALL 0x1406670e0
14069b920:  45 33 ED                 XOR R13D,R13D
14069b923:  4D 89 AF 28 05 00 00     MOV qword ptr [R15 + 0x528],R13
14069b92a:  4D 89 AF 38 05 00 00     MOV qword ptr [R15 + 0x538],R13
14069b931:  41 C7 87 6C 05 00 00 FF FF FF FF MOV dword ptr [R15 + 0x56c],0xffffffff
14069b93c:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069b943:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069b94a:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069b951:  45 89 AF 24 0D 00 00     MOV dword ptr [R15 + 0xd24],R13D
14069b958:  44 38 2D 06 DA 34 00     CMP byte ptr [0x1409e9365],R13B
14069b95f:  74 76                    JZ 0x14069b9d7
14069b961:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069b968:  48 85 C0                 TEST RAX,RAX
14069b96b:  74 6A                    JZ 0x14069b9d7
14069b96d:  49 39 87 F0 04 00 00     CMP qword ptr [R15 + 0x4f0],RAX
14069b974:  75 61                    JNZ 0x14069b9d7
14069b976:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b97d:  48 85 C9                 TEST RCX,RCX
14069b980:  74 55                    JZ 0x14069b9d7
14069b982:  8B 81 60 03 00 00        MOV EAX,dword ptr [RCX + 0x360]
14069b988:  83 F8 0C                 CMP EAX,0xc
14069b98b:  74 05                    JZ 0x14069b992
14069b98d:  83 F8 6A                 CMP EAX,0x6a
14069b990:  75 45                    JNZ 0x14069b9d7
14069b992:  49 8B 87 90 04 00 00     MOV RAX,qword ptr [R15 + 0x490]
14069b999:  49 2B 87 88 04 00 00     SUB RAX,qword ptr [R15 + 0x488]
14069b9a0:  48 C1 F8 03              SAR RAX,0x3
14069b9a4:  48 85 C0                 TEST RAX,RAX
14069b9a7:  74 2E                    JZ 0x14069b9d7
14069b9a9:  49 8B CF                 MOV RCX,R15
14069b9ac:  E8 DF E6 FE FF           CALL 0x14068a090
14069b9b1:  EB 24                    JMP 0x14069b9d7
14069b9b3:  41 89 B7 98 06 00 00     MOV dword ptr [R15 + 0x698],ESI
14069b9ba:  0F 28 CE                 MOVAPS XMM1,XMM6
14069b9bd:  F3 0F 10 05 1B EE 26 00  MOVSS XMM0,dword ptr [0x14090a7e0]
14069b9c5:  FF 15 AD 10 1D 00        CALL qword ptr [0x14086ca78]
14069b9cb:  45 33 ED                 XOR R13D,R13D
14069b9ce:  F3 41 0F 11 87 0C 0D 00 00 MOVSS dword ptr [R15 + 0xd0c],XMM0
14069b9d7:  F3 44 0F 11 4D 98        MOVSS dword ptr [RBP + -0x68],XMM9
14069b9dd:  BB 01 00 00 00           MOV EBX,0x1
14069b9e2:  89 5D 9C                 MOV dword ptr [RBP + -0x64],EBX
14069b9e5:  45 33 C9                 XOR R9D,R9D
14069b9e8:  45 33 C0                 XOR R8D,R8D
14069b9eb:  F3 41 0F 10 8F 24 0D 00 00 MOVSS XMM1,dword ptr [R15 + 0xd24]
14069b9f4:  48 8D 0D E5 94 33 00     LEA RCX,[0x1409d4ee0]
14069b9fb:  FF 15 BF 1B 1D 00        CALL qword ptr [0x14086d5c0]
14069ba01:  0F 2F 05 80 E3 26 00     COMISS XMM0,dword ptr [0x140909d88]
14069ba08:  76 25                    JBE 0x14069ba2f
14069ba0a:  F3 0F 5E 05 AE F8 26 00  DIVSS XMM0,dword ptr [0x14090b2c0]
14069ba12:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
14069ba16:  2B D8                    SUB EBX,EAX
14069ba18:  89 5D 9C                 MOV dword ptr [RBP + -0x64],EBX
14069ba1b:  66 0F 6E C3              MOVD XMM0,EBX
14069ba1f:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069ba22:  41 0F 28 F1              MOVAPS XMM6,XMM9
14069ba26:  F3 0F 5E F0              DIVSS XMM6,XMM0
14069ba2a:  F3 0F 11 75 98           MOVSS dword ptr [RBP + -0x68],XMM6
14069ba2f:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069ba36:  80 B8 80 86 00 00 00     CMP byte ptr [RAX + 0x8680],0x0
14069ba3d:  0F 84 99 00 00 00        JZ 0x14069badc
14069ba43:  41 0F 28 F2              MOVAPS XMM6,XMM10
14069ba47:  F3 41 0F 5F B7 30 0D 00 00 MAXSS XMM6,dword ptr [R15 + 0xd30]
14069ba50:  41 0F 28 C2              MOVAPS XMM0,XMM10
14069ba54:  F3 41 0F 10 8F 24 0D 00 00 MOVSS XMM1,dword ptr [R15 + 0xd24]
14069ba5d:  41 0F 2F C9              COMISS XMM1,XMM9
14069ba61:  76 3F                    JBE 0x14069baa2
14069ba63:  F3 41 0F 10 97 28 0D 00 00 MOVSS XMM2,dword ptr [R15 + 0xd28]
14069ba6c:  41 0F 2F D1              COMISS XMM2,XMM9
14069ba70:  76 30                    JBE 0x14069baa2
14069ba72:  F3 0F 5E CA              DIVSS XMM1,XMM2
14069ba76:  41 0F 28 C1              MOVAPS XMM0,XMM9
14069ba7a:  F3 0F 5D C1              MINSS XMM0,XMM1
14069ba7e:  41 0F 28 CA              MOVAPS XMM1,XMM10
14069ba82:  F3 0F 5F C8              MAXSS XMM1,XMM0
14069ba86:  F3 0F 59 C9              MULSS XMM1,XMM1
14069ba8a:  41 0F 28 C1              MOVAPS XMM0,XMM9
14069ba8e:  F3 0F 5C C1              SUBSS XMM0,XMM1
14069ba92:  F3 0F 59 05 3A E4 26 00  MULSS XMM0,dword ptr [0x140909ed4]
14069ba9a:  F3 0F 58 05 FE E1 26 00  ADDSS XMM0,dword ptr [0x140909ca0]
14069baa2:  F3 44 0F 10 05 7D E2 26 00 MOVSS XMM8,dword ptr [0x140909d28]
14069baab:  F3 41 0F 59 F0           MULSS XMM6,XMM8
14069bab0:  F3 0F 58 F0              ADDSS XMM6,XMM0
14069bab4:  45 33 C0                 XOR R8D,R8D
14069bab7:  0F 28 CE                 MOVAPS XMM1,XMM6
14069baba:  49 8B CF                 MOV RCX,R15
14069babd:  E8 8E 80 01 00           CALL 0x1406b3b50
14069bac2:  F3 0F 11 45 E8           MOVSS dword ptr [RBP + -0x18],XMM0
14069bac7:  F3 0F 59 F0              MULSS XMM6,XMM0
14069bacb:  45 0F 28 E9              MOVAPS XMM13,XMM9
14069bacf:  F3 44 0F 5D EE           MINSS XMM13,XMM6
14069bad4:  F3 44 0F 11 6D 58        MOVSS dword ptr [RBP + 0x58],XMM13
14069bada:  EB 09                    JMP 0x14069bae5
14069badc:  F3 44 0F 10 05 43 E2 26 00 MOVSS XMM8,dword ptr [0x140909d28]
14069bae5:  F3 41 0F 10 8F 88 0C 00 00 MOVSS XMM1,dword ptr [R15 + 0xc88]
14069baee:  41 0F 2F CA              COMISS XMM1,XMM10
14069baf2:  76 0D                    JBE 0x14069bb01
14069baf4:  F3 0F 5C CF              SUBSS XMM1,XMM7
14069baf8:  F3 41 0F 11 8F 88 0C 00 00 MOVSS dword ptr [R15 + 0xc88],XMM1
14069bb01:  4D 8B 97 F8 04 00 00     MOV R10,qword ptr [R15 + 0x4f8]
14069bb08:  F3 44 0F 10 2D C7 E1 26 00 MOVSS XMM13,dword ptr [0x140909cd8]
14069bb11:  F3 0F 10 35 3B E3 26 00  MOVSS XMM6,dword ptr [0x140909e54]
14069bb19:  4D 85 D2                 TEST R10,R10
14069bb1c:  0F 84 76 05 00 00        JZ 0x14069c098
14069bb22:  49 8B C2                 MOV RAX,R10
14069bb25:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069bb2c:  48 85 C9                 TEST RCX,RCX
14069bb2f:  0F 84 63 05 00 00        JZ 0x14069c098
14069bb35:  83 B9 60 03 00 00 31     CMP dword ptr [RCX + 0x360],0x31
14069bb3c:  0F 85 56 05 00 00        JNZ 0x14069c098
14069bb42:  49 83 BF 98 07 00 00 00  CMP qword ptr [R15 + 0x798],0x0
14069bb4a:  75 0E                    JNZ 0x14069bb5a
14069bb4c:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069bb54:  0F 84 3E 05 00 00        JZ 0x14069c098
14069bb5a:  4D 63 87 00 07 00 00     MOVSXD R8,dword ptr [R15 + 0x700]
14069bb61:  45 85 C0                 TEST R8D,R8D
14069bb64:  0F 88 2E 05 00 00        JS 0x14069c098
14069bb6a:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069bb71:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069bb78:  48 C1 F8 03              SAR RAX,0x3
14069bb7c:  4C 3B C0                 CMP R8,RAX
14069bb7f:  0F 83 13 05 00 00        JNC 0x14069c098
14069bb85:  49 8B CF                 MOV RCX,R15
14069bb88:  E8 13 1F FE FF           CALL 0x14067daa0
14069bb8d:  84 C0                    TEST AL,AL
14069bb8f:  0F 84 03 05 00 00        JZ 0x14069c098
14069bb95:  C6 85 B0 1C 00 00 01     MOV byte ptr [RBP + 0x1cb0],0x1
14069bb9c:  49 8B 87 C0 06 00 00     MOV RAX,qword ptr [R15 + 0x6c0]
14069bba3:  49 2B 87 B8 06 00 00     SUB RAX,qword ptr [R15 + 0x6b8]
14069bbaa:  48 C1 F8 03              SAR RAX,0x3
14069bbae:  44 3B C0                 CMP R8D,EAX
14069bbb1:  7D 76                    JGE 0x14069bc29
14069bbb3:  49 8B 87 B8 06 00 00     MOV RAX,qword ptr [R15 + 0x6b8]
14069bbba:  4A 83 3C C0 00           CMP qword ptr [RAX + R8*0x8],0x0
14069bbbf:  75 68                    JNZ 0x14069bc29
14069bbc1:  40 32 F6                 XOR SIL,SIL
14069bbc4:  40 88 B5 B0 1C 00 00     MOV byte ptr [RBP + 0x1cb0],SIL
14069bbcb:  41 C6 87 3A 0D 00 00 01  MOV byte ptr [R15 + 0xd3a],0x1
14069bbd3:  49 8B CF                 MOV RCX,R15
14069bbd6:  E8 15 02 02 00           CALL 0x1406bbdf0
14069bbdb:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bbe2:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bbe9:  48 C1 F8 03              SAR RAX,0x3
14069bbed:  48 85 C0                 TEST RAX,RAX
14069bbf0:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bbf7:  74 19                    JZ 0x14069bc12
14069bbf9:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bc00:  48 C1 F8 03              SAR RAX,0x3
14069bc04:  FF C8                    DEC EAX
14069bc06:  41 89 87 98 06 00 00     MOV dword ptr [R15 + 0x698],EAX
14069bc0d:  E9 95 04 00 00           JMP 0x14069c0a7
14069bc12:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bc19:  48 C1 F8 03              SAR RAX,0x3
14069bc1d:  41 89 87 98 06 00 00     MOV dword ptr [R15 + 0x698],EAX
14069bc24:  E9 7E 04 00 00           JMP 0x14069c0a7
14069bc29:  83 3D 04 A1 33 00 03     CMP dword ptr [0x1409d5d34],0x3
14069bc30:  0F 84 C8 00 00 00        JZ 0x14069bcfe
14069bc36:  81 3D 60 98 33 00 45 01 00 00 CMP dword ptr [0x1409d54a0],0x145
14069bc40:  0F 8D B8 00 00 00        JGE 0x14069bcfe
14069bc46:  40 32 F6                 XOR SIL,SIL
14069bc49:  40 88 B5 B0 1C 00 00     MOV byte ptr [RBP + 0x1cb0],SIL
14069bc50:  41 83 F8 01              CMP R8D,0x1
14069bc54:  0F 8E 4D 04 00 00        JLE 0x14069c0a7
14069bc5a:  49 63 8F 98 06 00 00     MOVSXD RCX,dword ptr [R15 + 0x698]
14069bc61:  85 C9                    TEST ECX,ECX
14069bc63:  0F 88 3E 04 00 00        JS 0x14069c0a7
14069bc69:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bc70:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bc77:  48 C1 F8 03              SAR RAX,0x3
14069bc7b:  48 3B C8                 CMP RCX,RAX
14069bc7e:  0F 83 23 04 00 00        JNC 0x14069c0a7
14069bc84:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069bc8b:  4C 39 14 C8              CMP qword ptr [RAX + RCX*0x8],R10
14069bc8f:  0F 84 12 04 00 00        JZ 0x14069c0a7
14069bc95:  41 C6 87 3A 0D 00 00 01  MOV byte ptr [R15 + 0xd3a],0x1
14069bc9d:  49 8B CF                 MOV RCX,R15
14069bca0:  E8 4B 01 02 00           CALL 0x1406bbdf0
14069bca5:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bcac:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bcb3:  48 C1 F8 03              SAR RAX,0x3
14069bcb7:  48 85 C0                 TEST RAX,RAX
14069bcba:  74 20                    JZ 0x14069bcdc
14069bcbc:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bcc3:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bcca:  48 C1 F8 03              SAR RAX,0x3
14069bcce:  FF C8                    DEC EAX
14069bcd0:  41 89 87 98 06 00 00     MOV dword ptr [R15 + 0x698],EAX
14069bcd7:  E9 CB 03 00 00           JMP 0x14069c0a7
14069bcdc:  45 33 C9                 XOR R9D,R9D
14069bcdf:  41 83 C8 FF              OR R8D,0xffffffff
14069bce3:  49 8B 97 F8 04 00 00     MOV RDX,qword ptr [R15 + 0x4f8]
14069bcea:  49 8B CF                 MOV RCX,R15
14069bced:  E8 DE 18 FE FF           CALL 0x14067d5d0
14069bcf2:  45 89 AF 98 06 00 00     MOV dword ptr [R15 + 0x698],R13D
14069bcf9:  E9 A9 03 00 00           JMP 0x14069c0a7
14069bcfe:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069bd05:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069bd0c:  48 C1 F8 03              SAR RAX,0x3
14069bd10:  4C 3B C0                 CMP R8,RAX
14069bd13:  0F 83 8B 03 00 00        JNC 0x14069c0a4
14069bd19:  48 8D 0D F0 91 33 00     LEA RCX,[0x1409d4f10]
14069bd20:  E8 0B 41 F3 FF           CALL 0x1405cfe30
14069bd25:  0F 2F 05 38 E0 26 00     COMISS XMM0,dword ptr [0x140909d64]
14069bd2c:  0F 86 46 03 00 00        JBE 0x14069c078
14069bd32:  49 83 BF A8 17 00 00 00  CMP qword ptr [R15 + 0x17a8],0x0
14069bd3a:  75 50                    JNZ 0x14069bd8c
14069bd3c:  F3 41 0F 10 87 24 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd24]
14069bd45:  41 0F 2F C2              COMISS XMM0,XMM10
14069bd49:  76 33                    JBE 0x14069bd7e
14069bd4b:  49 8B 97 08 17 00 00     MOV RDX,qword ptr [R15 + 0x1708]
14069bd52:  48 81 C2 94 87 00 00     ADD RDX,0x8794
14069bd59:  74 23                    JZ 0x14069bd7e
14069bd5b:  48 8D 05 28 74 1D 00     LEA RAX,[0x14087318a]
14069bd62:  48 3B D0                 CMP RDX,RAX
14069bd65:  74 17                    JZ 0x14069bd7e
14069bd67:  45 33 C0                 XOR R8D,R8D
14069bd6a:  48 8B 0D 9F 8E 34 00     MOV RCX,qword ptr [0x1409e4c10]
14069bd71:  FF 15 01 12 1D 00        CALL qword ptr [0x14086cf78]
14069bd77:  49 89 87 A8 17 00 00     MOV qword ptr [R15 + 0x17a8],RAX
14069bd7e:  49 83 BF A8 17 00 00 00  CMP qword ptr [R15 + 0x17a8],0x0
14069bd86:  0F 84 06 01 00 00        JZ 0x14069be92
14069bd8c:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069bd93:  FF 15 87 12 1D 00        CALL qword ptr [0x14086d020]
14069bd99:  84 C0                    TEST AL,AL
14069bd9b:  75 0F                    JNZ 0x14069bdac
14069bd9d:  B2 01                    MOV DL,0x1
14069bd9f:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069bda6:  FF 15 84 12 1D 00        CALL qword ptr [0x14086d030]
14069bdac:  F3 41 0F 10 8F 24 0D 00 00 MOVSS XMM1,dword ptr [R15 + 0xd24]
14069bdb5:  F3 0F 5E 0D 9F EB 26 00  DIVSS XMM1,dword ptr [0x14090a95c]
14069bdbd:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069bdc4:  FF 15 4E 12 1D 00        CALL qword ptr [0x14086d018]
14069bdca:  48 8D 95 40 04 00 00     LEA RDX,[RBP + 0x440]
14069bdd1:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069bdd8:  FF 15 A2 0D 1D 00        CALL qword ptr [0x14086cb80]
14069bdde:  48 8B C8                 MOV RCX,RAX
14069bde1:  F3 41 0F 10 97 24 0D 00 00 MOVSS XMM2,dword ptr [R15 + 0xd24]
14069bdea:  48 8D 95 50 04 00 00     LEA RDX,[RBP + 0x450]
14069bdf1:  FF 15 59 09 1D 00        CALL qword ptr [0x14086c750]
14069bdf7:  48 8B F0                 MOV RSI,RAX
14069bdfa:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069bdfe:  41 0F 28 D1              MOVAPS XMM2,XMM9
14069be02:  41 0F 28 CA              MOVAPS XMM1,XMM10
14069be06:  48 8D 8D 60 04 00 00     LEA RCX,[RBP + 0x460]
14069be0d:  FF 15 F5 01 1D 00        CALL qword ptr [0x14086c008]
14069be13:  48 8B F8                 MOV RDI,RAX
14069be16:  48 8D 95 70 04 00 00     LEA RDX,[RBP + 0x470]
14069be1d:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069be24:  FF 15 56 0D 1D 00        CALL qword ptr [0x14086cb80]
14069be2a:  48 8B D8                 MOV RBX,RAX
14069be2d:  48 8D 95 80 04 00 00     LEA RDX,[RBP + 0x480]
14069be34:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069be3b:  FF 15 77 09 1D 00        CALL qword ptr [0x14086c7b8]
14069be41:  48 8B D0                 MOV RDX,RAX
14069be44:  F3 44 0F 11 4C 24 28     MOVSS dword ptr [RSP + 0x28],XMM9
14069be4b:  48 89 74 24 20           MOV qword ptr [RSP + 0x20],RSI
14069be50:  4C 8B CF                 MOV R9,RDI
14069be53:  4C 8B C3                 MOV R8,RBX
14069be56:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069be5d:  FF 15 AD 11 1D 00        CALL qword ptr [0x14086d010]
14069be63:  49 83 BF A8 17 00 00 00  CMP qword ptr [R15 + 0x17a8],0x0
14069be6b:  74 25                    JZ 0x14069be92
14069be6d:  F3 41 0F 10 87 24 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd24]
14069be76:  41 0F 2E C2              UCOMISS XMM0,XMM10
14069be7a:  7A 16                    JP 0x14069be92
14069be7c:  75 14                    JNZ 0x14069be92
14069be7e:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069be85:  FF 15 9D 11 1D 00        CALL qword ptr [0x14086d028]
14069be8b:  4D 89 AF A8 17 00 00     MOV qword ptr [R15 + 0x17a8],R13
14069be92:  41 0F 2F FA              COMISS XMM7,XMM10
14069be96:  0F 86 08 02 00 00        JBE 0x14069c0a4
14069be9c:  41 80 BF D8 13 00 00 00  CMP byte ptr [R15 + 0x13d8],0x0
14069bea4:  0F 84 FA 01 00 00        JZ 0x14069c0a4
14069beaa:  F3 0F 10 05 6A EF 26 00  MOVSS XMM0,dword ptr [0x14090ae1c]
14069beb2:  41 0F 2F 87 DC 13 00 00  COMISS XMM0,dword ptr [R15 + 0x13dc]
14069beba:  0F 86 E4 01 00 00        JBE 0x14069c0a4
14069bec0:  F3 41 0F 10 87 24 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd24]
14069bec9:  41 0F 2F C1              COMISS XMM0,XMM9
14069becd:  0F 86 D1 01 00 00        JBE 0x14069c0a4
14069bed3:  49 8B BF 98 07 00 00     MOV RDI,qword ptr [R15 + 0x798]
14069beda:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069bee1:  F3 0F 10 98 7C 7A 00 00  MOVSS XMM3,dword ptr [RAX + 0x7a7c]
14069bee9:  41 0F 28 D5              MOVAPS XMM2,XMM13
14069beed:  41 0F 28 CB              MOVAPS XMM1,XMM11
14069bef1:  48 8D 8D 10 02 00 00     LEA RCX,[RBP + 0x210]
14069bef8:  FF 15 0A 01 1D 00        CALL qword ptr [0x14086c008]
14069befe:  48 8D 95 B0 0E 00 00     LEA RDX,[RBP + 0xeb0]
14069bf05:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069bf0c:  FF 15 AE 08 1D 00        CALL qword ptr [0x14086c7c0]
14069bf12:  4C 8B C0                 MOV R8,RAX
14069bf15:  48 8D 95 10 02 00 00     LEA RDX,[RBP + 0x210]
14069bf1c:  48 8D 8D 10 02 00 00     LEA RCX,[RBP + 0x210]
14069bf23:  FF 15 DF 06 1D 00        CALL qword ptr [0x14086c608]
14069bf29:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069bf2d:  0F 28 D6                 MOVAPS XMM2,XMM6
14069bf30:  0F 28 CE                 MOVAPS XMM1,XMM6
14069bf33:  48 8D 8D 08 01 00 00     LEA RCX,[RBP + 0x108]
14069bf3a:  FF 15 C8 00 1D 00        CALL qword ptr [0x14086c008]
14069bf40:  48 8D 95 08 01 00 00     LEA RDX,[RBP + 0x108]
14069bf47:  48 8D 8D 08 01 00 00     LEA RCX,[RBP + 0x108]
14069bf4e:  FF 15 B4 01 1D 00        CALL qword ptr [0x14086c108]
14069bf54:  48 8D 95 F0 0E 00 00     LEA RDX,[RBP + 0xef0]
14069bf5b:  49 8D 8F 60 0D 00 00     LEA RCX,[R15 + 0xd60]
14069bf62:  FF 15 58 08 1D 00        CALL qword ptr [0x14086c7c0]
14069bf68:  4C 8B C0                 MOV R8,RAX
14069bf6b:  48 8D 95 08 01 00 00     LEA RDX,[RBP + 0x108]
14069bf72:  48 8D 8D 08 01 00 00     LEA RCX,[RBP + 0x108]
14069bf79:  FF 15 31 04 1D 00        CALL qword ptr [0x14086c3b0]
14069bf7f:  48 85 FF                 TEST RDI,RDI
14069bf82:  0F 84 1C 01 00 00        JZ 0x14069c0a4
14069bf88:  45 8B CD                 MOV R9D,R13D
14069bf8b:  45 8B C5                 MOV R8D,R13D
14069bf8e:  48 8B 87 F0 00 00 00     MOV RAX,qword ptr [RDI + 0xf0]
14069bf95:  48 3B 87 E8 00 00 00     CMP RAX,qword ptr [RDI + 0xe8]
14069bf9c:  74 40                    JZ 0x14069bfde
14069bf9e:  49 8B D5                 MOV RDX,R13
14069bfa1:  0F 1F 40 00              NOP dword ptr [RAX]
14069bfa5:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069bfb0:  48 8B 87 E8 00 00 00     MOV RAX,qword ptr [RDI + 0xe8]
14069bfb7:  0F B6 0C 02              MOVZX ECX,byte ptr [RDX + RAX*0x1]
14069bfbb:  41 3B C9                 CMP ECX,R9D
14069bfbe:  7E 05                    JLE 0x14069bfc5
14069bfc0:  44 0F B6 0C 10           MOVZX R9D,byte ptr [RAX + RDX*0x1]
14069bfc5:  41 FF C0                 INC R8D
14069bfc8:  48 8B 87 F0 00 00 00     MOV RAX,qword ptr [RDI + 0xf0]
14069bfcf:  48 2B 87 E8 00 00 00     SUB RAX,qword ptr [RDI + 0xe8]
14069bfd6:  41 8B D0                 MOV EDX,R8D
14069bfd9:  48 3B D0                 CMP RDX,RAX
14069bfdc:  72 D2                    JC 0x14069bfb0
14069bfde:  48 8D 0D FB 8E 33 00     LEA RCX,[0x1409d4ee0]
14069bfe5:  FF 15 25 0F 1D 00        CALL qword ptr [0x14086cf10]
14069bfeb:  84 C0                    TEST AL,AL
14069bfed:  0F 85 B1 00 00 00        JNZ 0x14069c0a4
14069bff3:  48 8D 0D E6 8E 33 00     LEA RCX,[0x1409d4ee0]
14069bffa:  FF 15 08 0F 1D 00        CALL qword ptr [0x14086cf08]
14069c000:  84 C0                    TEST AL,AL
14069c002:  0F 85 9C 00 00 00        JNZ 0x14069c0a4
14069c008:  48 8D 95 08 01 00 00     LEA RDX,[RBP + 0x108]
14069c00f:  48 8D 8D 90 04 00 00     LEA RCX,[RBP + 0x490]
14069c016:  FF 15 F4 FF 1C 00        CALL qword ptr [0x14086c010]
14069c01c:  48 8B F8                 MOV RDI,RAX
14069c01f:  48 8D 95 10 02 00 00     LEA RDX,[RBP + 0x210]
14069c026:  48 8D 8D A0 04 00 00     LEA RCX,[RBP + 0x4a0]
14069c02d:  FF 15 DD FF 1C 00        CALL qword ptr [0x14086c010]
14069c033:  48 8B D8                 MOV RBX,RAX
14069c036:  48 8D 15 EB 1D 25 00     LEA RDX,[0x1408ede28]
14069c03d:  48 8B 0D AC 81 2F 00     MOV RCX,qword ptr [0x1409941f0]
14069c044:  48 8B 89 60 0F 00 00     MOV RCX,qword ptr [RCX + 0xf60]
14069c04b:  FF 15 7F 02 1D 00        CALL qword ptr [0x14086c2d0]
14069c051:  48 8B C8                 MOV RCX,RAX
14069c054:  C6 44 24 30 00           MOV byte ptr [RSP + 0x30],0x0
14069c059:  F3 44 0F 11 4C 24 28     MOVSS dword ptr [RSP + 0x28],XMM9
14069c060:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14069c067:  41 B1 01                 MOV R9B,0x1
14069c06a:  4C 8B C7                 MOV R8,RDI
14069c06d:  48 8B D3                 MOV RDX,RBX
14069c070:  FF 15 02 03 1D 00        CALL qword ptr [0x14086c378]
14069c076:  EB 2C                    JMP 0x14069c0a4
14069c078:  49 83 BF A8 17 00 00 00  CMP qword ptr [R15 + 0x17a8],0x0
14069c080:  74 22                    JZ 0x14069c0a4
14069c082:  49 8B 8F A8 17 00 00     MOV RCX,qword ptr [R15 + 0x17a8]
14069c089:  FF 15 99 0F 1D 00        CALL qword ptr [0x14086d028]
14069c08f:  4D 89 AF A8 17 00 00     MOV qword ptr [R15 + 0x17a8],R13
14069c096:  EB 0C                    JMP 0x14069c0a4
14069c098:  40 32 F6                 XOR SIL,SIL
14069c09b:  40 88 B5 B0 1C 00 00     MOV byte ptr [RBP + 0x1cb0],SIL
14069c0a2:  EB 03                    JMP 0x14069c0a7
14069c0a4:  40 B6 01                 MOV SIL,0x1
14069c0a7:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069c0af:  0F 85 83 00 00 00        JNZ 0x14069c138
14069c0b5:  49 83 BF 98 07 00 00 00  CMP qword ptr [R15 + 0x798],0x0
14069c0bd:  74 79                    JZ 0x14069c138
14069c0bf:  F3 41 0F 10 87 34 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd34]
14069c0c8:  41 0F 2F C2              COMISS XMM0,XMM10
14069c0cc:  72 29                    JC 0x14069c0f7
14069c0ce:  45 33 C9                 XOR R9D,R9D
14069c0d1:  45 33 C0                 XOR R8D,R8D
14069c0d4:  41 0F 28 CC              MOVAPS XMM1,XMM12
14069c0d8:  48 8D 0D 01 8E 33 00     LEA RCX,[0x1409d4ee0]
14069c0df:  FF 15 43 FF 1C 00        CALL qword ptr [0x14086c028]
14069c0e5:  F3 41 0F 58 87 34 0D 00 00 ADDSS XMM0,dword ptr [R15 + 0xd34]
14069c0ee:  F3 41 0F 11 87 34 0D 00 00 MOVSS dword ptr [R15 + 0xd34],XMM0
14069c0f7:  49 63 8F 00 07 00 00     MOVSXD RCX,dword ptr [R15 + 0x700]
14069c0fe:  85 C9                    TEST ECX,ECX
14069c100:  78 36                    JS 0x14069c138
14069c102:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069c109:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069c110:  48 C1 F8 03              SAR RAX,0x3
14069c114:  48 3B C8                 CMP RCX,RAX
14069c117:  73 1F                    JNC 0x14069c138
14069c119:  40 84 F6                 TEST SIL,SIL
14069c11c:  75 1A                    JNZ 0x14069c138
14069c11e:  4D 8B 87 A0 06 00 00     MOV R8,qword ptr [R15 + 0x6a0]
14069c125:  4D 8B 04 C8              MOV R8,qword ptr [R8 + RCX*0x8]
14069c129:  49 8B 97 98 07 00 00     MOV RDX,qword ptr [R15 + 0x798]
14069c130:  49 8B CF                 MOV RCX,R15
14069c133:  E8 58 FA 01 00           CALL 0x1406bbb90
14069c138:  F3 0F 10 0D C0 F1 26 00  MOVSS XMM1,dword ptr [0x14090b300]
14069c140:  49 8B CF                 MOV RCX,R15
14069c143:  E8 48 00 FE FF           CALL 0x14067c190
14069c148:  41 80 BF 7D 06 00 00 00  CMP byte ptr [R15 + 0x67d],0x0
14069c150:  74 66                    JZ 0x14069c1b8
14069c152:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069c15a:  74 5C                    JZ 0x14069c1b8
14069c15c:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069c164:  75 52                    JNZ 0x14069c1b8
14069c166:  F3 41 0F 10 87 0C 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd0c]
14069c16f:  F3 0F 5C C7              SUBSS XMM0,XMM7
14069c173:  F3 41 0F 11 87 0C 0D 00 00 MOVSS dword ptr [R15 + 0xd0c],XMM0
14069c17c:  44 0F 2F D0              COMISS XMM10,XMM0
14069c180:  72 36                    JC 0x14069c1b8
14069c182:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14069c187:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14069c18c:  41 83 C8 FF              OR R8D,0xffffffff
14069c190:  B2 01                    MOV DL,0x1
14069c192:  49 8B CF                 MOV RCX,R15
14069c195:  E8 86 48 FE FF           CALL 0x140680a20
14069c19a:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069c1a2:  74 0D                    JZ 0x14069c1b1
14069c1a4:  41 C7 87 0C 0D 00 00 00 00 20 41 MOV dword ptr [R15 + 0xd0c],0x41200000
14069c1af:  EB 07                    JMP 0x14069c1b8
14069c1b1:  45 89 AF 0C 0D 00 00     MOV dword ptr [R15 + 0xd0c],R13D
14069c1b8:  44 89 6C 24 44           MOV dword ptr [RSP + 0x44],R13D
14069c1bd:  F3 44 0F 10 25 5A 05 27 00 MOVSS XMM12,dword ptr [0x14090c720]
14069c1c6:  F3 44 0F 10 1D D9 E4 26 00 MOVSS XMM11,dword ptr [0x14090a6a8]
14069c1cf:  F3 44 0F 10 35 D4 DC 26 00 MOVSS XMM14,dword ptr [0x140909eac]
14069c1d8:  83 7D 9C 00              CMP dword ptr [RBP + -0x64],0x0
14069c1dc:  0F 8E 06 69 00 00        JLE 0x1406a2ae8
14069c1e2:  4D 8D B7 60 0D 00 00     LEA R14,[R15 + 0xd60]
14069c1e9:  F3 44 0F 10 25 36 E7 26 00 MOVSS XMM12,dword ptr [0x14090a928]
14069c1f2:  F3 44 0F 10 35 85 E0 26 00 MOVSS XMM14,dword ptr [0x14090a280]
14069c1fb:  F3 44 0F 10 05 AC E5 26 00 MOVSS XMM8,dword ptr [0x14090a7b0]
14069c204:  48 8D 1D A5 94 3B 00     LEA RBX,[0x140a556b0]
14069c20b:  48 BF A1 A0 A0 A0 A0 A0 A0 A0 MOV RDI,-0x5f5f5f5f5f5f5f5f
14069c215:  48 8D 95 B0 04 00 00     LEA RDX,[RBP + 0x4b0]
14069c21c:  49 8B CE                 MOV RCX,R14
14069c21f:  FF 15 8B 05 1D 00        CALL qword ptr [0x14086c7b0]
14069c225:  F3 0F 10 78 04           MOVSS XMM7,dword ptr [RAX + 0x4]
14069c22a:  F3 0F 11 7C 24 4C        MOVSS dword ptr [RSP + 0x4c],XMM7
14069c230:  48 8D 4D A8              LEA RCX,[RBP + -0x58]
14069c234:  FF 15 DE FD 1C 00        CALL qword ptr [0x14086c018]
14069c23a:  F3 44 0F 10 6C 24 78     MOVSS XMM13,dword ptr [RSP + 0x78]
14069c241:  49 8B CF                 MOV RCX,R15
14069c244:  E8 07 CA FF FF           CALL 0x140698c50
14069c249:  0F 28 D0                 MOVAPS XMM2,XMM0
14069c24c:  F3 0F 11 54 24 7C        MOVSS dword ptr [RSP + 0x7c],XMM2
14069c252:  48 BE AB AA AA AA AA AA AA 2A MOV RSI,0x2aaaaaaaaaaaaaab
14069c25c:  49 83 BF 08 17 00 00 00  CMP qword ptr [R15 + 0x1708],0x0
14069c264:  74 66                    JZ 0x14069c2cc
14069c266:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069c26d:  48 8B 88 60 7C 00 00     MOV RCX,qword ptr [RAX + 0x7c60]
14069c274:  48 2B 88 58 7C 00 00     SUB RCX,qword ptr [RAX + 0x7c58]
14069c27b:  48 8B C6                 MOV RAX,RSI
14069c27e:  48 F7 E9                 IMUL RCX
14069c281:  48 D1 FA                 SAR RDX,0x1
14069c284:  48 8B C2                 MOV RAX,RDX
14069c287:  48 C1 E8 3F              SHR RAX,0x3f
14069c28b:  48 03 D0                 ADD RDX,RAX
14069c28e:  74 3C                    JZ 0x14069c2cc
14069c290:  F3 41 0F 10 87 D0 19 00 00 MOVSS XMM0,dword ptr [R15 + 0x19d0]
14069c299:  41 0F 28 CD              MOVAPS XMM1,XMM13
14069c29d:  44 0F 28 E9              MOVAPS XMM13,XMM1
14069c2a1:  F3 45 0F 5C E8           SUBSS XMM13,XMM8
14069c2a6:  F3 44 0F 59 E8           MULSS XMM13,XMM0
14069c2ab:  F3 45 0F 58 E8           ADDSS XMM13,XMM8
14069c2b0:  41 83 BF D4 19 00 00 00  CMP dword ptr [R15 + 0x19d4],0x0
14069c2b8:  74 12                    JZ 0x14069c2cc
14069c2ba:  44 0F 28 E9              MOVAPS XMM13,XMM1
14069c2be:  F3 44 0F 59 2D F9 DD 26 00 MULSS XMM13,dword ptr [0x14090a0c0]
14069c2c7:  F3 44 0F 59 E8           MULSS XMM13,XMM0
14069c2cc:  F3 41 0F 10 87 30 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd30]
14069c2d5:  0F 57 05 44 04 27 00     XORPS XMM0,xmmword ptr [0x14090c720]
14069c2dc:  41 0F 2F C2              COMISS XMM0,XMM10
14069c2e0:  76 21                    JBE 0x14069c303
14069c2e2:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069c2e9:  41 0F 28 C9              MOVAPS XMM1,XMM9
14069c2ed:  F3 0F 5C 88 30 86 00 00  SUBSS XMM1,dword ptr [RAX + 0x8630]
14069c2f5:  F3 0F 59 C8              MULSS XMM1,XMM0
14069c2f9:  41 0F 28 C1              MOVAPS XMM0,XMM9
14069c2fd:  F3 0F 5C C1              SUBSS XMM0,XMM1
14069c301:  EB 13                    JMP 0x14069c316
14069c303:  44 0F 2F D0              COMISS XMM10,XMM0
14069c307:  76 17                    JBE 0x14069c320
14069c309:  F3 41 0F 5C C1           SUBSS XMM0,XMM9
14069c30e:  F3 0F 59 05 FA EF 26 00  MULSS XMM0,dword ptr [0x14090b310]
14069c316:  F3 0F 59 D0              MULSS XMM2,XMM0
14069c31a:  F3 0F 11 54 24 7C        MOVSS dword ptr [RSP + 0x7c],XMM2
14069c320:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069c327:  80 B8 80 86 00 00 00     CMP byte ptr [RAX + 0x8680],0x0
14069c32e:  74 0B                    JZ 0x14069c33b
14069c330:  F3 0F 59 55 E8           MULSS XMM2,dword ptr [RBP + -0x18]
14069c335:  F3 0F 11 54 24 7C        MOVSS dword ptr [RSP + 0x7c],XMM2
14069c33b:  F3 44 0F 11 AD 00 01 00 00 MOVSS dword ptr [RBP + 0x100],XMM13
14069c344:  45 0F 28 C5              MOVAPS XMM8,XMM13
14069c348:  F3 41 0F 10 87 18 07 00 00 MOVSS XMM0,dword ptr [R15 + 0x718]
14069c351:  41 0F 2F C2              COMISS XMM0,XMM10
14069c355:  72 11                    JC 0x14069c368
14069c357:  F3 0F 10 0D 41 E6 26 00  MOVSS XMM1,dword ptr [0x14090a9a0]
14069c35f:  0F 2F C8                 COMISS XMM1,XMM0
14069c362:  76 04                    JBE 0x14069c368
14069c364:  44 0F 28 C0              MOVAPS XMM8,XMM0
14069c368:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069c36d:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069c371:  48 8B C6                 MOV RAX,RSI
14069c374:  48 F7 E9                 IMUL RCX
14069c377:  48 D1 FA                 SAR RDX,0x1
14069c37a:  48 8B C2                 MOV RAX,RDX
14069c37d:  48 C1 E8 3F              SHR RAX,0x3f
14069c381:  48 03 D0                 ADD RDX,RAX
14069c384:  0F 85 93 00 00 00        JNZ 0x14069c41d
14069c38a:  45 0F 28 C2              MOVAPS XMM8,XMM10
14069c38e:  49 39 97 80 0C 00 00     CMP qword ptr [R15 + 0xc80],RDX
14069c395:  74 38                    JZ 0x14069c3cf
14069c397:  41 83 BF 6C 05 00 00 F6  CMP dword ptr [R15 + 0x56c],-0xa
14069c39f:  7F 10                    JG 0x14069c3b1
14069c3a1:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069c3a8:  48 39 90 D0 05 00 00     CMP qword ptr [RAX + 0x5d0],RDX
14069c3af:  75 1E                    JNZ 0x14069c3cf
14069c3b1:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069c3b8:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069c3bf:  75 07                    JNZ 0x14069c3c8
14069c3c1:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069c3c8:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069c3cf:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069c3d7:  74 0E                    JZ 0x14069c3e7
14069c3d9:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069c3e1:  0F 85 44 1E 00 00        JNZ 0x14069e22b
14069c3e7:  45 89 AF 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R13D
14069c3ee:  4D 89 AF 98 07 00 00     MOV qword ptr [R15 + 0x798],R13
14069c3f5:  49 8B 87 C0 07 00 00     MOV RAX,qword ptr [R15 + 0x7c0]
14069c3fc:  49 2B 87 B8 07 00 00     SUB RAX,qword ptr [R15 + 0x7b8]
14069c403:  48 C1 F8 03              SAR RAX,0x3
14069c407:  48 85 C0                 TEST RAX,RAX
14069c40a:  0F 84 1B 1E 00 00        JZ 0x14069e22b
14069c410:  49 8B CF                 MOV RCX,R15
14069c413:  E8 58 55 FE FF           CALL 0x140681970
14069c418:  E9 0E 1E 00 00           JMP 0x14069e22b
14069c41d:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069c422:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069c426:  48 8B C6                 MOV RAX,RSI
14069c429:  48 F7 E9                 IMUL RCX
14069c42c:  48 D1 FA                 SAR RDX,0x1
14069c42f:  48 8B C2                 MOV RAX,RDX
14069c432:  48 C1 E8 3F              SHR RAX,0x3f
14069c436:  48 03 D0                 ADD RDX,RAX
14069c439:  4D 63 87 68 07 00 00     MOVSXD R8,dword ptr [R15 + 0x768]
14069c440:  44 3B C2                 CMP R8D,EDX
14069c443:  0F 8C D4 01 00 00        JL 0x14069c61d
14069c449:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069c450:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069c457:  48 C1 F8 03              SAR RAX,0x3
14069c45b:  48 85 C0                 TEST RAX,RAX
14069c45e:  74 4E                    JZ 0x14069c4ae
14069c460:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069c467:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069c46e:  48 C1 F8 03              SAR RAX,0x3
14069c472:  41 39 87 00 07 00 00     CMP dword ptr [R15 + 0x700],EAX
14069c479:  7D 33                    JGE 0x14069c4ae
14069c47b:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069c480:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069c484:  48 8B C6                 MOV RAX,RSI
14069c487:  48 F7 E9                 IMUL RCX
14069c48a:  48 D1 FA                 SAR RDX,0x1
14069c48d:  48 8B C2                 MOV RAX,RDX
14069c490:  48 C1 E8 3F              SHR RAX,0x3f
14069c494:  48 03 D0                 ADD RDX,RAX
14069c497:  74 15                    JZ 0x14069c4ae
14069c499:  49 83 BF F0 04 00 00 00  CMP qword ptr [R15 + 0x4f0],0x0
14069c4a1:  75 0B                    JNZ 0x14069c4ae
14069c4a3:  41 8D 40 FF              LEA EAX,[R8 + -0x1]
14069c4a7:  41 89 87 68 07 00 00     MOV dword ptr [R15 + 0x768],EAX
14069c4ae:  45 0F 28 C2              MOVAPS XMM8,XMM10
14069c4b2:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069c4ba:  0F 84 8E 00 00 00        JZ 0x14069c54e
14069c4c0:  41 83 BF 6C 05 00 00 F6  CMP dword ptr [R15 + 0x56c],-0xa
14069c4c8:  7F 11                    JG 0x14069c4db
14069c4ca:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069c4d1:  48 83 B8 D0 05 00 00 00  CMP qword ptr [RAX + 0x5d0],0x0
14069c4d9:  75 1E                    JNZ 0x14069c4f9
14069c4db:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069c4e2:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069c4e9:  75 07                    JNZ 0x14069c4f2
14069c4eb:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069c4f2:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069c4f9:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069c500:  48 85 C0                 TEST RAX,RAX
14069c503:  74 49                    JZ 0x14069c54e
14069c505:  49 3B 87 F8 04 00 00     CMP RAX,qword ptr [R15 + 0x4f8]
14069c50c:  75 40                    JNZ 0x14069c54e
14069c50e:  49 8B 9F 90 04 00 00     MOV RBX,qword ptr [R15 + 0x490]
14069c515:  49 2B 9F 88 04 00 00     SUB RBX,qword ptr [R15 + 0x488]
14069c51c:  48 C1 FB 03              SAR RBX,0x3
14069c520:  83 EB 01                 SUB EBX,0x1
14069c523:  78 22                    JS 0x14069c547
14069c525:  66 66 66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069c530:  45 33 C0                 XOR R8D,R8D
14069c533:  49 8B 97 F0 04 00 00     MOV RDX,qword ptr [R15 + 0x4f0]
14069c53a:  49 8B CF                 MOV RCX,R15
14069c53d:  E8 CE DB FE FF           CALL 0x14068a110
14069c542:  83 EB 01                 SUB EBX,0x1
14069c545:  79 E9                    JNS 0x14069c530
14069c547:  48 8D 1D 62 91 3B 00     LEA RBX,[0x140a556b0]
14069c54e:  4D 63 87 98 06 00 00     MOVSXD R8,dword ptr [R15 + 0x698]
14069c555:  45 85 C0                 TEST R8D,R8D
14069c558:  0F 88 9E 00 00 00        JS 0x14069c5fc
14069c55e:  49 8B 8F 40 06 00 00     MOV RCX,qword ptr [R15 + 0x640]
14069c565:  49 2B 8F 38 06 00 00     SUB RCX,qword ptr [R15 + 0x638]
14069c56c:  48 8B C7                 MOV RAX,RDI
14069c56f:  48 F7 E9                 IMUL RCX
14069c572:  48 03 D1                 ADD RDX,RCX
14069c575:  48 C1 FA 08              SAR RDX,0x8
14069c579:  48 8B C2                 MOV RAX,RDX
14069c57c:  48 C1 E8 3F              SHR RAX,0x3f
14069c580:  48 03 D0                 ADD RDX,RAX
14069c583:  49 8B C8                 MOV RCX,R8
14069c586:  4C 3B C2                 CMP R8,RDX
14069c589:  73 71                    JNC 0x14069c5fc
14069c58b:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069c592:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069c599:  48 C1 F8 03              SAR RAX,0x3
14069c59d:  48 83 F8 02              CMP RAX,0x2
14069c5a1:  73 59                    JNC 0x14069c5fc
14069c5a3:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069c5aa:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069c5b1:  48 C1 F8 03              SAR RAX,0x3
14069c5b5:  48 85 C0                 TEST RAX,RAX
14069c5b8:  74 42                    JZ 0x14069c5fc
14069c5ba:  48 69 C9 98 01 00 00     IMUL RCX,RCX,0x198
14069c5c1:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069c5c8:  48 39 9C 01 90 01 00 00  CMP qword ptr [RCX + RAX*0x1 + 0x190],RBX
14069c5d0:  75 2A                    JNZ 0x14069c5fc
14069c5d2:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069c5da:  74 20                    JZ 0x14069c5fc
14069c5dc:  41 C6 87 3A 0D 00 00 00  MOV byte ptr [R15 + 0xd3a],0x0
14069c5e4:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14069c5e9:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14069c5ee:  41 83 C8 FF              OR R8D,0xffffffff
14069c5f2:  B2 01                    MOV DL,0x1
14069c5f4:  49 8B CF                 MOV RCX,R15
14069c5f7:  E8 24 44 FE FF           CALL 0x140680a20
14069c5fc:  41 83 BF 6C 05 00 00 FF  CMP dword ptr [R15 + 0x56c],-0x1
14069c604:  0F 85 C5 FD FF FF        JNZ 0x14069c3cf
14069c60a:  41 83 BF 10 05 00 00 FF  CMP dword ptr [R15 + 0x510],-0x1
14069c612:  0F 84 13 1C 00 00        JZ 0x14069e22b
14069c618:  E9 B2 FD FF FF           JMP 0x14069c3cf
14069c61d:  49 83 BF 98 07 00 00 00  CMP qword ptr [R15 + 0x798],0x0
14069c625:  0F 84 44 02 00 00        JZ 0x14069c86f
14069c62b:  49 83 BF F0 04 00 00 00  CMP qword ptr [R15 + 0x4f0],0x0
14069c633:  0F 85 36 02 00 00        JNZ 0x14069c86f
14069c639:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069c641:  0F 85 28 02 00 00        JNZ 0x14069c86f
14069c647:  49 8B 9F 98 07 00 00     MOV RBX,qword ptr [R15 + 0x798]
14069c64e:  48 8B 8B C0 00 00 00     MOV RCX,qword ptr [RBX + 0xc0]
14069c655:  48 2B 8B B8 00 00 00     SUB RCX,qword ptr [RBX + 0xb8]
14069c65c:  49 BE E1 83 0F 3E F8 E0 83 0F MOV R14,0xf83e0f83e0f83e1
14069c666:  49 8B C6                 MOV RAX,R14
14069c669:  48 F7 E9                 IMUL RCX
14069c66c:  48 C1 FA 04              SAR RDX,0x4
14069c670:  48 8B C2                 MOV RAX,RDX
14069c673:  48 C1 E8 3F              SHR RAX,0x3f
14069c677:  48 03 D0                 ADD RDX,RAX
14069c67a:  0F 84 EF 01 00 00        JZ 0x14069c86f
14069c680:  45 85 C0                 TEST R8D,R8D
14069c683:  0F 88 E6 01 00 00        JS 0x14069c86f
14069c689:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069c68e:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069c692:  48 8B C6                 MOV RAX,RSI
14069c695:  48 F7 E9                 IMUL RCX
14069c698:  48 D1 FA                 SAR RDX,0x1
14069c69b:  48 8B C2                 MOV RAX,RDX
14069c69e:  48 C1 E8 3F              SHR RAX,0x3f
14069c6a2:  48 03 D0                 ADD RDX,RAX
14069c6a5:  4C 3B C2                 CMP R8,RDX
14069c6a8:  0F 83 C1 01 00 00        JNC 0x14069c86f
14069c6ae:  49 63 8F 00 07 00 00     MOVSXD RCX,dword ptr [R15 + 0x700]
14069c6b5:  85 C9                    TEST ECX,ECX
14069c6b7:  0F 88 B2 01 00 00        JS 0x14069c86f
14069c6bd:  49 8B 87 A8 06 00 00     MOV RAX,qword ptr [R15 + 0x6a8]
14069c6c4:  49 2B 87 A0 06 00 00     SUB RAX,qword ptr [R15 + 0x6a0]
14069c6cb:  48 C1 F8 03              SAR RAX,0x3
14069c6cf:  48 8B D1                 MOV RDX,RCX
14069c6d2:  48 3B C8                 CMP RCX,RAX
14069c6d5:  0F 83 94 01 00 00        JNC 0x14069c86f
14069c6db:  49 8B 8F A0 06 00 00     MOV RCX,qword ptr [R15 + 0x6a0]
14069c6e2:  48 8B 43 20              MOV RAX,qword ptr [RBX + 0x20]
14069c6e6:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
14069c6ea:  40 0F 94 C6              SETZ SIL
14069c6ee:  4B 8D 0C 40              LEA RCX,[R8 + R8*0x2]
14069c6f2:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069c6f6:  48 8D 14 88              LEA RDX,[RAX + RCX*0x4]
14069c6fa:  48 8D 8D C0 04 00 00     LEA RCX,[RBP + 0x4c0]
14069c701:  FF 15 09 F9 1C 00        CALL qword ptr [0x14086c010]
14069c707:  41 8B FD                 MOV EDI,R13D
14069c70a:  48 8B 8B C0 00 00 00     MOV RCX,qword ptr [RBX + 0xc0]
14069c711:  48 2B 8B B8 00 00 00     SUB RCX,qword ptr [RBX + 0xb8]
14069c718:  49 8B C6                 MOV RAX,R14
14069c71b:  48 F7 E9                 IMUL RCX
14069c71e:  48 C1 FA 04              SAR RDX,0x4
14069c722:  48 8B C2                 MOV RAX,RDX
14069c725:  48 C1 E8 3F              SHR RAX,0x3f
14069c729:  48 03 D0                 ADD RDX,RAX
14069c72c:  0F 84 33 01 00 00        JZ 0x14069c865
14069c732:  49 8B C5                 MOV RAX,R13
14069c735:  F3 44 0F 10 35 62 E2 26 00 MOVSS XMM14,dword ptr [0x14090a9a0]
14069c73e:  F3 44 0F 10 25 D1 DE 26 00 MOVSS XMM12,dword ptr [0x14090a618]
14069c747:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
14069c750:  48 69 C8 08 01 00 00     IMUL RCX,RAX,0x108
14069c757:  48 8B 83 B8 00 00 00     MOV RAX,qword ptr [RBX + 0xb8]
14069c75e:  40 38 34 01              CMP byte ptr [RCX + RAX*0x1],SIL
14069c762:  0F 85 BC 00 00 00        JNZ 0x14069c824
14069c768:  80 BC 01 F8 00 00 00 01  CMP byte ptr [RCX + RAX*0x1 + 0xf8],0x1
14069c770:  0F 85 AE 00 00 00        JNZ 0x14069c824
14069c776:  48 83 C0 08              ADD RAX,0x8
14069c77a:  48 03 C8                 ADD RCX,RAX
14069c77d:  48 8D 95 18 01 00 00     LEA RDX,[RBP + 0x118]
14069c784:  FF 15 2E 00 1D 00        CALL qword ptr [0x14086c7b8]
14069c78a:  49 8D 8F 80 10 00 00     LEA RCX,[R15 + 0x1080]
14069c791:  48 8D 95 30 0F 00 00     LEA RDX,[RBP + 0xf30]
14069c798:  FF 15 5A 02 1D 00        CALL qword ptr [0x14086c9f8]
14069c79e:  4C 8B C0                 MOV R8,RAX
14069c7a1:  48 8D 95 18 01 00 00     LEA RDX,[RBP + 0x118]
14069c7a8:  48 8D 8D 18 01 00 00     LEA RCX,[RBP + 0x118]
14069c7af:  FF 15 53 FE 1C 00        CALL qword ptr [0x14086c608]
14069c7b5:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069c7bc:  F3 0F 10 80 7C 7A 00 00  MOVSS XMM0,dword ptr [RAX + 0x7a7c]
14069c7c4:  F3 0F 59 C6              MULSS XMM0,XMM6
14069c7c8:  F3 0F 10 8D 20 01 00 00  MOVSS XMM1,dword ptr [RBP + 0x120]
14069c7d0:  F3 0F 5C C8              SUBSS XMM1,XMM0
14069c7d4:  F3 0F 11 8D 20 01 00 00  MOVSS dword ptr [RBP + 0x120],XMM1
14069c7dc:  41 0F 2F C9              COMISS XMM1,XMM9
14069c7e0:  76 42                    JBE 0x14069c824
14069c7e2:  F3 0F 10 85 18 01 00 00  MOVSS XMM0,dword ptr [RBP + 0x118]
14069c7ea:  0F 2F 05 2B ED 26 00     COMISS XMM0,dword ptr [0x14090b51c]
14069c7f1:  76 31                    JBE 0x14069c824
14069c7f3:  44 0F 2F F0              COMISS XMM14,XMM0
14069c7f7:  76 2B                    JBE 0x14069c824
14069c7f9:  F3 0F 5C 0D 5F D8 26 00  SUBSS XMM1,dword ptr [0x14090a060]
14069c801:  41 0F 28 C2              MOVAPS XMM0,XMM10
14069c805:  F3 0F 5F C1              MAXSS XMM0,XMM1
14069c809:  45 0F 28 C4              MOVAPS XMM8,XMM12
14069c80d:  F3 44 0F 5D C0           MINSS XMM8,XMM0
14069c812:  F3 44 0F 59 05 E9 D4 26 00 MULSS XMM8,dword ptr [0x140909d04]
14069c81b:  F3 44 0F 59 05 94 E2 26 00 MULSS XMM8,dword ptr [0x14090aab8]
14069c824:  FF C7                    INC EDI
14069c826:  48 8B 8B C0 00 00 00     MOV RCX,qword ptr [RBX + 0xc0]
14069c82d:  48 2B 8B B8 00 00 00     SUB RCX,qword ptr [RBX + 0xb8]
14069c834:  49 8B C6                 MOV RAX,R14
14069c837:  48 F7 E9                 IMUL RCX
14069c83a:  48 C1 FA 04              SAR RDX,0x4
14069c83e:  48 8B C2                 MOV RAX,RDX
14069c841:  48 C1 E8 3F              SHR RAX,0x3f
14069c845:  48 03 D0                 ADD RDX,RAX
14069c848:  8B C7                    MOV EAX,EDI
14069c84a:  48 3B C2                 CMP RAX,RDX
14069c84d:  0F 82 FD FE FF FF        JC 0x14069c750
14069c853:  F3 44 0F 10 25 CC E0 26 00 MOVSS XMM12,dword ptr [0x14090a928]
14069c85c:  F3 44 0F 10 35 1B DA 26 00 MOVSS XMM14,dword ptr [0x14090a280]
14069c865:  48 BE AB AA AA AA AA AA AA 2A MOV RSI,0x2aaaaaaaaaaaaaab
14069c86f:  48 8D 8D 68 01 00 00     LEA RCX,[RBP + 0x168]
14069c876:  FF 15 9C F7 1C 00        CALL qword ptr [0x14086c018]
14069c87c:  49 63 87 68 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x768]
14069c883:  48 8D 0C 40              LEA RCX,[RAX + RAX*0x2]
14069c887:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069c88b:  48 8D 1C 88              LEA RBX,[RAX + RCX*0x4]
14069c88f:  49 8D BF 80 10 00 00     LEA RDI,[R15 + 0x1080]
14069c896:  48 8D 95 70 0F 00 00     LEA RDX,[RBP + 0xf70]
14069c89d:  48 8B CF                 MOV RCX,RDI
14069c8a0:  FF 15 52 01 1D 00        CALL qword ptr [0x14086c9f8]
14069c8a6:  4C 8B C0                 MOV R8,RAX
14069c8a9:  48 8B D3                 MOV RDX,RBX
14069c8ac:  48 8D 8D 68 01 00 00     LEA RCX,[RBP + 0x168]
14069c8b3:  FF 15 4F FD 1C 00        CALL qword ptr [0x14086c608]
14069c8b9:  F3 0F 10 BD 70 01 00 00  MOVSS XMM7,dword ptr [RBP + 0x170]
14069c8c1:  41 0F 28 DA              MOVAPS XMM3,XMM10
14069c8c5:  41 0F 28 D2              MOVAPS XMM2,XMM10
14069c8c9:  41 0F 28 CA              MOVAPS XMM1,XMM10
14069c8cd:  48 8D 8D D0 04 00 00     LEA RCX,[RBP + 0x4d0]
14069c8d4:  FF 15 2E F7 1C 00        CALL qword ptr [0x14086c008]
14069c8da:  48 8B D0                 MOV RDX,RAX
14069c8dd:  48 8D 8D 68 01 00 00     LEA RCX,[RBP + 0x168]
14069c8e4:  FF 15 F6 FF 1C 00        CALL qword ptr [0x14086c8e0]
14069c8ea:  F2 0F 10 35 0E FE 26 00  MOVSD XMM6,qword ptr [0x14090c700]
14069c8f2:  44 0F 2F D7              COMISS XMM10,XMM7
14069c8f6:  76 34                    JBE 0x14069c92c
14069c8f8:  F3 0F 10 8D 68 01 00 00  MOVSS XMM1,dword ptr [RBP + 0x168]
14069c900:  0F 5A C9                 CVTPS2PD XMM1,XMM1
14069c903:  0F 54 CE                 ANDPS XMM1,XMM6
14069c906:  66 0F 5A D1              CVTPD2PS XMM2,XMM1
14069c90a:  F3 0F 10 0D 06 DD 26 00  MOVSS XMM1,dword ptr [0x14090a618]
14069c912:  0F 2F CA                 COMISS XMM1,XMM2
14069c915:  76 09                    JBE 0x14069c920
14069c917:  0F 2F 05 4E E4 26 00     COMISS XMM0,dword ptr [0x14090ad6c]
14069c91e:  EB 07                    JMP 0x14069c927
14069c920:  0F 2F 05 A5 DF 26 00     COMISS XMM0,dword ptr [0x14090a8cc]
14069c927:  76 03                    JBE 0x14069c92c
14069c929:  0F 28 F8                 MOVAPS XMM7,XMM0
14069c92c:  49 63 97 68 07 00 00     MOVSXD RDX,dword ptr [R15 + 0x768]
14069c933:  85 D2                    TEST EDX,EDX
14069c935:  0F 84 96 00 00 00        JZ 0x14069c9d1
14069c93b:  0F 2F 3D D2 D2 26 00     COMISS XMM7,dword ptr [0x140909c14]
14069c942:  0F 87 89 00 00 00        JA 0x14069c9d1
14069c948:  8D 42 FF                 LEA EAX,[RDX + -0x1]
14069c94b:  48 63 C8                 MOVSXD RCX,EAX
14069c94e:  48 8D 0C 49              LEA RCX,[RCX + RCX*0x2]
14069c952:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069c956:  4C 8D 04 88              LEA R8,[RAX + RCX*0x4]
14069c95a:  48 8D 0C 52              LEA RCX,[RDX + RDX*0x2]
14069c95e:  48 8D 0C 88              LEA RCX,[RAX + RCX*0x4]
14069c962:  48 8D 95 DC 04 00 00     LEA RDX,[RBP + 0x4dc]
14069c969:  FF 15 E9 FD 1C 00        CALL qword ptr [0x14086c758]
14069c96f:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14069c973:  F2 0F 11 45 A8           MOVSD qword ptr [RBP + -0x58],XMM0
14069c978:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14069c97b:  89 45 B0                 MOV dword ptr [RBP + -0x50],EAX
14069c97e:  C7 45 AC 00 00 00 00     MOV dword ptr [RBP + -0x54],0x0
14069c985:  48 8D 8D D0 02 00 00     LEA RCX,[RBP + 0x2d0]
14069c98c:  FF 15 86 F6 1C 00        CALL qword ptr [0x14086c018]
14069c992:  48 8D 55 A8              LEA RDX,[RBP + -0x58]
14069c996:  48 8D 8D D0 02 00 00     LEA RCX,[RBP + 0x2d0]
14069c99d:  FF 15 5D 06 1D 00        CALL qword ptr [0x14086d000]
14069c9a3:  F3 0F 10 45 B0           MOVSS XMM0,dword ptr [RBP + -0x50]
14069c9a8:  F3 0F 58 45 A8           ADDSS XMM0,dword ptr [RBP + -0x58]
14069c9ad:  0F 5A C0                 CVTPS2PD XMM0,XMM0
14069c9b0:  0F 54 C6                 ANDPS XMM0,XMM6
14069c9b3:  66 0F 5A C0              CVTPD2PS XMM0,XMM0
14069c9b7:  0F 2F 05 3E D1 26 00     COMISS XMM0,dword ptr [0x140909afc]
14069c9be:  0F 86 9B 00 00 00        JBE 0x14069ca5f
14069c9c4:  F3 0F 10 85 D4 02 00 00  MOVSS XMM0,dword ptr [RBP + 0x2d4]
14069c9cc:  E9 88 00 00 00           JMP 0x14069ca59
14069c9d1:  48 8D 14 52              LEA RDX,[RDX + RDX*0x2]
14069c9d5:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069c9d9:  48 8D 1C 90              LEA RBX,[RAX + RDX*0x4]
14069c9dd:  48 8D 95 E8 04 00 00     LEA RDX,[RBP + 0x4e8]
14069c9e4:  48 8B CF                 MOV RCX,RDI
14069c9e7:  FF 15 CB FD 1C 00        CALL qword ptr [0x14086c7b8]
14069c9ed:  4C 8B C0                 MOV R8,RAX
14069c9f0:  48 8D 95 F4 04 00 00     LEA RDX,[RBP + 0x4f4]
14069c9f7:  48 8B CB                 MOV RCX,RBX
14069c9fa:  FF 15 58 FD 1C 00        CALL qword ptr [0x14086c758]
14069ca00:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
14069ca04:  F2 0F 11 45 A8           MOVSD qword ptr [RBP + -0x58],XMM0
14069ca09:  8B 40 08                 MOV EAX,dword ptr [RAX + 0x8]
14069ca0c:  89 45 B0                 MOV dword ptr [RBP + -0x50],EAX
14069ca0f:  C7 45 AC 00 00 00 00     MOV dword ptr [RBP + -0x54],0x0
14069ca16:  48 8D 8D 50 03 00 00     LEA RCX,[RBP + 0x350]
14069ca1d:  FF 15 F5 F5 1C 00        CALL qword ptr [0x14086c018]
14069ca23:  48 8D 55 A8              LEA RDX,[RBP + -0x58]
14069ca27:  48 8D 8D 50 03 00 00     LEA RCX,[RBP + 0x350]
14069ca2e:  FF 15 CC 05 1D 00        CALL qword ptr [0x14086d000]
14069ca34:  F3 0F 10 45 B0           MOVSS XMM0,dword ptr [RBP + -0x50]
14069ca39:  F3 0F 58 45 A8           ADDSS XMM0,dword ptr [RBP + -0x58]
14069ca3e:  0F 5A C0                 CVTPS2PD XMM0,XMM0
14069ca41:  0F 54 C6                 ANDPS XMM0,XMM6
14069ca44:  66 0F 5A C0              CVTPD2PS XMM0,XMM0
14069ca48:  0F 2F 05 AD D0 26 00     COMISS XMM0,dword ptr [0x140909afc]
14069ca4f:  76 0E                    JBE 0x14069ca5f
14069ca51:  F3 0F 10 85 54 03 00 00  MOVSS XMM0,dword ptr [RBP + 0x354]
14069ca59:  F3 0F 11 44 24 4C        MOVSS dword ptr [RSP + 0x4c],XMM0
14069ca5f:  48 8D 55 A8              LEA RDX,[RBP + -0x58]
14069ca63:  48 8D 4D A8              LEA RCX,[RBP + -0x58]
14069ca67:  FF 15 9B F6 1C 00        CALL qword ptr [0x14086c108]
14069ca6d:  F3 0F 10 0D 67 D3 26 00  MOVSS XMM1,dword ptr [0x140909ddc]
14069ca75:  0F 28 F1                 MOVAPS XMM6,XMM1
14069ca78:  41 8B 8F 68 07 00 00     MOV ECX,dword ptr [R15 + 0x768]
14069ca7f:  83 F9 03                 CMP ECX,0x3
14069ca82:  7C 38                    JL 0x14069cabc
14069ca84:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069ca8c:  74 2E                    JZ 0x14069cabc
14069ca8e:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069ca95:  48 83 B8 D0 05 00 00 00  CMP qword ptr [RAX + 0x5d0],0x0
14069ca9d:  74 0F                    JZ 0x14069caae
14069ca9f:  83 F9 05                 CMP ECX,0x5
14069caa2:  7D 0A                    JGE 0x14069caae
14069caa4:  41 83 BF 00 07 00 00 02  CMP dword ptr [R15 + 0x700],0x2
14069caac:  7C 0E                    JL 0x14069cabc
14069caae:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069cab5:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069cabc:  F3 0F 10 05 04 D7 26 00  MOVSS XMM0,dword ptr [0x14090a1c8]
14069cac4:  0F 2F C7                 COMISS XMM0,XMM7
14069cac7:  0F 86 32 01 00 00        JBE 0x14069cbff
14069cacd:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069cad2:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069cad6:  48 8B C6                 MOV RAX,RSI
14069cad9:  48 F7 E9                 IMUL RCX
14069cadc:  48 D1 FA                 SAR RDX,0x1
14069cadf:  48 8B C2                 MOV RAX,RDX
14069cae2:  48 C1 E8 3F              SHR RAX,0x3f
14069cae6:  48 03 C2                 ADD RAX,RDX
14069cae9:  4D 63 87 68 07 00 00     MOVSXD R8,dword ptr [R15 + 0x768]
14069caf0:  FF C8                    DEC EAX
14069caf2:  44 3B C0                 CMP R8D,EAX
14069caf5:  0F 8D 04 01 00 00        JGE 0x14069cbff
14069cafb:  41 80 BF 6C 07 00 00 00  CMP byte ptr [R15 + 0x76c],0x0
14069cb03:  0F 85 F6 00 00 00        JNZ 0x14069cbff
14069cb09:  4B 8D 0C 40              LEA RCX,[R8 + R8*0x2]
14069cb0d:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069cb11:  48 8D 14 88              LEA RDX,[RAX + RCX*0x4]
14069cb15:  41 8D 40 01              LEA EAX,[R8 + 0x1]
14069cb19:  48 63 C8                 MOVSXD RCX,EAX
14069cb1c:  48 8D 0C 49              LEA RCX,[RCX + RCX*0x2]
14069cb20:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069cb24:  48 8D 0C 88              LEA RCX,[RAX + RCX*0x4]
14069cb28:  FF 15 6A FC 1C 00        CALL qword ptr [0x14086c798]
14069cb2e:  49 63 87 68 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x768]
14069cb35:  48 8B C8                 MOV RCX,RAX
14069cb38:  FF C0                    INC EAX
14069cb3a:  48 63 D0                 MOVSXD RDX,EAX
14069cb3d:  48 8D 0C 49              LEA RCX,[RCX + RCX*0x2]
14069cb41:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069cb45:  4C 8D 04 88              LEA R8,[RAX + RCX*0x4]
14069cb49:  48 8D 0C 52              LEA RCX,[RDX + RDX*0x2]
14069cb4d:  48 8D 0C 88              LEA RCX,[RAX + RCX*0x4]
14069cb51:  48 8D 95 D0 00 00 00     LEA RDX,[RBP + 0xd0]
14069cb58:  FF 15 FA FB 1C 00        CALL qword ptr [0x14086c758]
14069cb5e:  C7 85 D4 00 00 00 00 00 00 00 MOV dword ptr [RBP + 0xd4],0x0
14069cb68:  48 8D 95 D0 00 00 00     LEA RDX,[RBP + 0xd0]
14069cb6f:  48 8D 8D D0 00 00 00     LEA RCX,[RBP + 0xd0]
14069cb76:  FF 15 8C F5 1C 00        CALL qword ptr [0x14086c108]
14069cb7c:  49 63 87 68 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x768]
14069cb83:  48 8D 0C 40              LEA RCX,[RAX + RAX*0x2]
14069cb87:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069cb8b:  F3 0F 10 85 D0 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xd0]
14069cb93:  F3 0F 59 05 2D D6 26 00  MULSS XMM0,dword ptr [0x14090a1c8]
14069cb9b:  F3 0F 58 04 88           ADDSS XMM0,dword ptr [RAX + RCX*0x4]
14069cba0:  F3 0F 11 04 88           MOVSS dword ptr [RAX + RCX*0x4],XMM0
14069cba5:  49 63 87 68 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x768]
14069cbac:  48 8D 0C 40              LEA RCX,[RAX + RAX*0x2]
14069cbb0:  49 8B 04 24              MOV RAX,qword ptr [R12]
14069cbb4:  F3 0F 10 85 D8 00 00 00  MOVSS XMM0,dword ptr [RBP + 0xd8]
14069cbbc:  F3 0F 59 05 04 D6 26 00  MULSS XMM0,dword ptr [0x14090a1c8]
14069cbc4:  F3 0F 58 44 88 08        ADDSS XMM0,dword ptr [RAX + RCX*0x4 + 0x8]
14069cbca:  F3 0F 11 44 88 08        MOVSS dword ptr [RAX + RCX*0x4 + 0x8],XMM0
14069cbd0:  41 C6 87 6C 07 00 00 01  MOV byte ptr [R15 + 0x76c],0x1
14069cbd8:  48 8D 55 A8              LEA RDX,[RBP + -0x58]
14069cbdc:  48 8D 8D D0 00 00 00     LEA RCX,[RBP + 0xd0]
14069cbe3:  FF 15 47 09 1D 00        CALL qword ptr [0x14086d530]
14069cbe9:  E8 C0 28 1B 00           CALL 0x14084f4ae
14069cbee:  F3 41 0F 11 87 70 07 00 00 MOVSS dword ptr [R15 + 0x770],XMM0
14069cbf7:  F3 0F 10 0D DD D1 26 00  MOVSS XMM1,dword ptr [0x140909ddc]
14069cbff:  49 8D 9F A0 06 00 00     LEA RBX,[R15 + 0x6a0]
14069cc06:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069cc0a:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069cc0d:  48 83 F8 08              CMP RAX,0x8
14069cc11:  0F 83 1E 04 00 00        JNC 0x14069d035
14069cc17:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069cc1c:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069cc20:  48 8B C6                 MOV RAX,RSI
14069cc23:  48 F7 E9                 IMUL RCX
14069cc26:  48 D1 FA                 SAR RDX,0x1
14069cc29:  48 8B C2                 MOV RAX,RDX
14069cc2c:  48 C1 E8 3F              SHR RAX,0x3f
14069cc30:  48 03 C2                 ADD RAX,RDX
14069cc33:  41 8B 8F 68 07 00 00     MOV ECX,dword ptr [R15 + 0x768]
14069cc3a:  FF C8                    DEC EAX
14069cc3c:  3B C8                    CMP ECX,EAX
14069cc3e:  7C 5C                    JL 0x14069cc9c
14069cc40:  F3 41 0F 10 B7 4C 11 00 00 MOVSS XMM6,dword ptr [R15 + 0x114c]
14069cc49:  F3 0F 59 35 27 D2 26 00  MULSS XMM6,dword ptr [0x140909e78]
14069cc51:  F3 0F 58 F1              ADDSS XMM6,XMM1
14069cc55:  0F 28 C6                 MOVAPS XMM0,XMM6
14069cc58:  F3 0F 58 05 B8 D9 26 00  ADDSS XMM0,dword ptr [0x14090a618]
14069cc60:  0F 2F C7                 COMISS XMM0,XMM7
14069cc63:  76 37                    JBE 0x14069cc9c
14069cc65:  0F 28 CF                 MOVAPS XMM1,XMM7
14069cc68:  F3 0F 5C CE              SUBSS XMM1,XMM6
14069cc6c:  F3 0F 59 0D 90 D0 26 00  MULSS XMM1,dword ptr [0x140909d04]
14069cc74:  41 0F 28 C2              MOVAPS XMM0,XMM10
14069cc78:  F3 0F 5F C1              MAXSS XMM0,XMM1
14069cc7c:  45 0F 28 C1              MOVAPS XMM8,XMM9
14069cc80:  F3 44 0F 5D C0           MINSS XMM8,XMM0
14069cc85:  F3 44 0F 59 05 9A D2 26 00 MULSS XMM8,dword ptr [0x140909f28]
14069cc8e:  F3 44 0F 58 05 7D CF 26 00 ADDSS XMM8,dword ptr [0x140909c14]
14069cc97:  F3 45 0F 59 C5           MULSS XMM8,XMM13
14069cc9c:  0F 2F F7                 COMISS XMM6,XMM7
14069cc9f:  0F 86 80 15 00 00        JBE 0x14069e225
14069cca5:  8D 41 01                 LEA EAX,[RCX + 0x1]
14069cca8:  41 89 87 68 07 00 00     MOV dword ptr [R15 + 0x768],EAX
14069ccaf:  41 C6 87 6C 07 00 00 00  MOV byte ptr [R15 + 0x76c],0x0
14069ccb7:  45 89 AF 70 07 00 00     MOV dword ptr [R15 + 0x770],R13D
14069ccbe:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069ccc3:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069ccc7:  48 8B C6                 MOV RAX,RSI
14069ccca:  48 F7 E9                 IMUL RCX
14069cccd:  48 D1 FA                 SAR RDX,0x1
14069ccd0:  48 8B C2                 MOV RAX,RDX
14069ccd3:  48 C1 E8 3F              SHR RAX,0x3f
14069ccd7:  48 03 D0                 ADD RDX,RAX
14069ccda:  41 39 97 68 07 00 00     CMP dword ptr [R15 + 0x768],EDX
14069cce1:  0F 8C 3E 15 00 00        JL 0x14069e225
14069cce7:  80 BD B0 1C 00 00 00     CMP byte ptr [RBP + 0x1cb0],0x0
14069ccee:  74 45                    JZ 0x14069cd35
14069ccf0:  49 63 87 00 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x700]
14069ccf7:  4C 8B 03                 MOV R8,qword ptr [RBX]
14069ccfa:  4D 8B 04 C0              MOV R8,qword ptr [R8 + RAX*0x8]
14069ccfe:  49 8B 97 98 07 00 00     MOV RDX,qword ptr [R15 + 0x798]
14069cd05:  49 8B CF                 MOV RCX,R15
14069cd08:  E8 03 F5 01 00           CALL 0x1406bc210
14069cd0d:  49 8B 8F 98 07 00 00     MOV RCX,qword ptr [R15 + 0x798]
14069cd14:  48 85 C9                 TEST RCX,RCX
14069cd17:  74 1C                    JZ 0x14069cd35
14069cd19:  4C 39 B9 00 01 00 00     CMP qword ptr [RCX + 0x100],R15
14069cd20:  75 07                    JNZ 0x14069cd29
14069cd22:  4C 89 A9 00 01 00 00     MOV qword ptr [RCX + 0x100],R13
14069cd29:  49 8D 97 48 0D 00 00     LEA RDX,[R15 + 0xd48]
14069cd30:  E8 9B CB A5 FF           CALL 0x1400f98d0
14069cd35:  32 C9                    XOR CL,CL
14069cd37:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069cd3f:  74 5E                    JZ 0x14069cd9f
14069cd41:  41 83 BF 6C 05 00 00 00  CMP dword ptr [R15 + 0x56c],0x0
14069cd49:  7D 04                    JGE 0x14069cd4f
14069cd4b:  B1 01                    MOV CL,0x1
14069cd4d:  EB 29                    JMP 0x14069cd78
14069cd4f:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069cd56:  48 8B 90 18 03 00 00     MOV RDX,qword ptr [RAX + 0x318]
14069cd5d:  48 85 D2                 TEST RDX,RDX
14069cd60:  74 16                    JZ 0x14069cd78
14069cd62:  0F B6 C9                 MOVZX ECX,CL
14069cd65:  80 BA 78 09 00 00 00     CMP byte ptr [RDX + 0x978],0x0
14069cd6c:  41 B8 01 00 00 00        MOV R8D,0x1
14069cd72:  41 0F 45 C8              CMOVNZ ECX,R8D
14069cd76:  EB 06                    JMP 0x14069cd7e
14069cd78:  41 B8 01 00 00 00        MOV R8D,0x1
14069cd7e:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069cd85:  48 8B 90 18 03 00 00     MOV RDX,qword ptr [RAX + 0x318]
14069cd8c:  48 85 D2                 TEST RDX,RDX
14069cd8f:  74 0E                    JZ 0x14069cd9f
14069cd91:  0F B6 C9                 MOVZX ECX,CL
14069cd94:  80 BA 79 09 00 00 00     CMP byte ptr [RDX + 0x979],0x0
14069cd9b:  41 0F 45 C8              CMOVNZ ECX,R8D
14069cd9f:  49 83 BF F0 04 00 00 00  CMP qword ptr [R15 + 0x4f0],0x0
14069cda7:  74 1C                    JZ 0x14069cdc5
14069cda9:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069cdb0:  48 8B 90 18 03 00 00     MOV RDX,qword ptr [RAX + 0x318]
14069cdb7:  48 85 D2                 TEST RDX,RDX
14069cdba:  74 09                    JZ 0x14069cdc5
14069cdbc:  80 BA 79 09 00 00 00     CMP byte ptr [RDX + 0x979],0x0
14069cdc3:  75 0C                    JNZ 0x14069cdd1
14069cdc5:  84 C9                    TEST CL,CL
14069cdc7:  75 08                    JNZ 0x14069cdd1
14069cdc9:  49 8B CF                 MOV RCX,R15
14069cdcc:  E8 AF 09 01 00           CALL 0x1406ad780
14069cdd1:  49 83 BF F0 04 00 00 00  CMP qword ptr [R15 + 0x4f0],0x0
14069cdd9:  0F 84 08 02 00 00        JZ 0x14069cfe7
14069cddf:  4D 8B 87 F0 04 00 00     MOV R8,qword ptr [R15 + 0x4f0]
14069cde6:  49 8B 80 18 03 00 00     MOV RAX,qword ptr [R8 + 0x318]
14069cded:  48 85 C0                 TEST RAX,RAX
14069cdf0:  0F 84 E9 01 00 00        JZ 0x14069cfdf
14069cdf6:  83 B8 60 03 00 00 0F     CMP dword ptr [RAX + 0x360],0xf
14069cdfd:  0F 85 DC 01 00 00        JNZ 0x14069cfdf
14069ce03:  41 80 BF 7D 06 00 00 00  CMP byte ptr [R15 + 0x67d],0x0
14069ce0b:  0F 84 CE 01 00 00        JZ 0x14069cfdf
14069ce11:  41 C6 87 7D 06 00 00 00  MOV byte ptr [R15 + 0x67d],0x0
14069ce19:  48 8B 05 F8 79 34 00     MOV RAX,qword ptr [0x1409e4818]
14069ce20:  49 89 87 10 17 00 00     MOV qword ptr [R15 + 0x1710],RAX
14069ce27:  41 C7 87 4C 11 00 00 00 00 40 3F MOV dword ptr [R15 + 0x114c],0x3f400000
14069ce32:  41 C7 87 50 11 00 00 00 00 40 3F MOV dword ptr [R15 + 0x1150],0x3f400000
14069ce3d:  49 8B CF                 MOV RCX,R15
14069ce40:  E8 CB 9F 00 00           CALL 0x1406a6e10
14069ce45:  49 83 BF 08 17 00 00 00  CMP qword ptr [R15 + 0x1708],0x0
14069ce4d:  0F 84 2E 01 00 00        JZ 0x14069cf81
14069ce53:  49 8B BF B0 04 00 00     MOV RDI,qword ptr [R15 + 0x4b0]
14069ce5a:  49 2B BF A8 04 00 00     SUB RDI,qword ptr [R15 + 0x4a8]
14069ce61:  48 C1 FF 03              SAR RDI,0x3
14069ce65:  FF CF                    DEC EDI
14069ce67:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069ce6e:  F3 0F 2C 88 04 86 00 00  CVTTSS2SI ECX,dword ptr [RAX + 0x8604]
14069ce76:  3B F9                    CMP EDI,ECX
14069ce78:  0F 8E 03 01 00 00        JLE 0x14069cf81
14069ce7e:  48 63 C7                 MOVSXD RAX,EDI
14069ce81:  4C 8D 34 C5 00 00 00 00  LEA R14,[RAX*0x8]
14069ce89:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
14069ce90:  40 32 F6                 XOR SIL,SIL
14069ce93:  41 8B DD                 MOV EBX,R13D
14069ce96:  49 8B 87 A8 03 00 00     MOV RAX,qword ptr [R15 + 0x3a8]
14069ce9d:  49 2B 87 A0 03 00 00     SUB RAX,qword ptr [R15 + 0x3a0]
14069cea4:  48 C1 F8 03              SAR RAX,0x3
14069cea8:  48 85 C0                 TEST RAX,RAX
14069ceab:  0F 84 D0 00 00 00        JZ 0x14069cf81
14069ceb1:  4D 8B C5                 MOV R8,R13
14069ceb4:  49 8B 87 A0 03 00 00     MOV RAX,qword ptr [R15 + 0x3a0]
14069cebb:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14069cebf:  48 83 B9 08 17 00 00 00  CMP qword ptr [RCX + 0x1708],0x0
14069cec7:  74 6E                    JZ 0x14069cf37
14069cec9:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14069cecd:  48 8B 91 B0 04 00 00     MOV RDX,qword ptr [RCX + 0x4b0]
14069ced4:  48 2B 91 A8 04 00 00     SUB RDX,qword ptr [RCX + 0x4a8]
14069cedb:  48 C1 FA 03              SAR RDX,0x3
14069cedf:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14069cee3:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
14069ceea:  F3 0F 2C 80 04 86 00 00  CVTTSS2SI EAX,dword ptr [RAX + 0x8604]
14069cef2:  48 98                    CDQE
14069cef4:  48 3B D0                 CMP RDX,RAX
14069cef7:  73 3E                    JNC 0x14069cf37
14069cef9:  40 B6 01                 MOV SIL,0x1
14069cefc:  49 8B 87 A8 04 00 00     MOV RAX,qword ptr [R15 + 0x4a8]
14069cf03:  49 8B 0C 06              MOV RCX,qword ptr [R14 + RAX*0x1]
14069cf07:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069cf0e:  48 89 41 60              MOV qword ptr [RCX + 0x60],RAX
14069cf12:  48 63 CF                 MOVSXD RCX,EDI
14069cf15:  49 8B 87 A8 04 00 00     MOV RAX,qword ptr [R15 + 0x4a8]
14069cf1c:  48 8D 14 C8              LEA RDX,[RAX + RCX*0x8]
14069cf20:  49 8B 87 A0 03 00 00     MOV RAX,qword ptr [R15 + 0x3a0]
14069cf27:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14069cf2b:  48 81 C1 A8 04 00 00     ADD RCX,0x4a8
14069cf32:  E8 59 3F A1 FF           CALL 0x1400b0e90
14069cf37:  FF C3                    INC EBX
14069cf39:  49 8B 87 A8 03 00 00     MOV RAX,qword ptr [R15 + 0x3a8]
14069cf40:  49 2B 87 A0 03 00 00     SUB RAX,qword ptr [R15 + 0x3a0]
14069cf47:  48 C1 F8 03              SAR RAX,0x3
14069cf4b:  44 8B C3                 MOV R8D,EBX
14069cf4e:  4C 3B C0                 CMP R8,RAX
14069cf51:  0F 82 5D FF FF FF        JC 0x14069ceb4
14069cf57:  40 84 F6                 TEST SIL,SIL
14069cf5a:  74 25                    JZ 0x14069cf81
14069cf5c:  49 83 87 B0 04 00 00 F8  ADD qword ptr [R15 + 0x4b0],-0x8
14069cf64:  FF CF                    DEC EDI
14069cf66:  49 83 EE 08              SUB R14,0x8
14069cf6a:  49 8B 87 08 17 00 00     MOV RAX,qword ptr [R15 + 0x1708]
14069cf71:  F3 0F 2C 88 04 86 00 00  CVTTSS2SI ECX,dword ptr [RAX + 0x8604]
14069cf79:  3B F9                    CMP EDI,ECX
14069cf7b:  0F 8F 0F FF FF FF        JG 0x14069ce90
14069cf81:  49 63 87 10 05 00 00     MOVSXD RAX,dword ptr [R15 + 0x510]
14069cf88:  F3 0F 10 7C 24 4C        MOVSS XMM7,dword ptr [RSP + 0x4c]
14069cf8e:  8B 5C 24 44              MOV EBX,dword ptr [RSP + 0x44]
14069cf92:  85 C0                    TEST EAX,EAX
14069cf94:  0F 88 95 12 00 00        JS 0x14069e22f
14069cf9a:  48 8B C8                 MOV RCX,RAX
14069cf9d:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069cfa4:  C6 84 01 90 0B 00 00 00  MOV byte ptr [RCX + RAX*0x1 + 0xb90],0x0
14069cfac:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069cfb3:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069cfba:  4C 89 B8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R15
14069cfc1:  49 8B 87 F0 04 00 00     MOV RAX,qword ptr [R15 + 0x4f0]
14069cfc8:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069cfcf:  41 C7 87 10 05 00 00 FF FF FF FF MOV dword ptr [R15 + 0x510],0xffffffff
14069cfda:  E9 50 12 00 00           JMP 0x14069e22f
14069cfdf:  41 B1 01                 MOV R9B,0x1
14069cfe2:  E9 2F 12 00 00           JMP 0x14069e216
14069cfe7:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069cfef:  0F 84 30 12 00 00        JZ 0x14069e225
14069cff5:  49 63 97 6C 05 00 00     MOVSXD RDX,dword ptr [R15 + 0x56c]
14069cffc:  85 D2                    TEST EDX,EDX
14069cffe:  78 26                    JS 0x14069d026
14069d000:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069d007:  49 8B 88 48 0A 00 00     MOV RCX,qword ptr [R8 + 0xa48]
14069d00e:  49 2B 88 40 0A 00 00     SUB RCX,qword ptr [R8 + 0xa40]
14069d015:  48 C1 F9 05              SAR RCX,0x5
14069d019:  48 3B D1                 CMP RDX,RCX
14069d01c:  73 08                    JNC 0x14069d026
14069d01e:  45 33 C9                 XOR R9D,R9D
14069d021:  E9 F7 11 00 00           JMP 0x14069e21d
14069d026:  41 B1 01                 MOV R9B,0x1
14069d029:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069d030:  E9 E1 11 00 00           JMP 0x14069e216
14069d035:  0F 2F CF                 COMISS XMM1,XMM7
14069d038:  0F 86 E7 11 00 00        JBE 0x14069e225
14069d03e:  41 FF 87 68 07 00 00     INC dword ptr [R15 + 0x768]
14069d045:  41 C6 87 6C 07 00 00 00  MOV byte ptr [R15 + 0x76c],0x0
14069d04d:  45 89 AF 70 07 00 00     MOV dword ptr [R15 + 0x770],R13D
14069d054:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
14069d059:  49 2B 0C 24              SUB RCX,qword ptr [R12]
14069d05d:  48 8B C6                 MOV RAX,RSI
14069d060:  48 F7 E9                 IMUL RCX
14069d063:  48 D1 FA                 SAR RDX,0x1
14069d066:  48 8B C2                 MOV RAX,RDX
14069d069:  48 C1 E8 3F              SHR RAX,0x3f
14069d06d:  48 03 D0                 ADD RDX,RAX
14069d070:  41 39 97 68 07 00 00     CMP dword ptr [R15 + 0x768],EDX
14069d077:  0F 8C A8 11 00 00        JL 0x14069e225
14069d07d:  0F B6 BD B0 1C 00 00     MOVZX EDI,byte ptr [RBP + 0x1cb0]
14069d084:  40 84 FF                 TEST DIL,DIL
14069d087:  74 6E                    JZ 0x14069d0f7
14069d089:  49 63 87 00 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x700]
14069d090:  4C 8B 03                 MOV R8,qword ptr [RBX]
14069d093:  4D 8B 04 C0              MOV R8,qword ptr [R8 + RAX*0x8]
14069d097:  49 8B 97 98 07 00 00     MOV RDX,qword ptr [R15 + 0x798]
14069d09e:  49 8B CF                 MOV RCX,R15
14069d0a1:  E8 6A F1 01 00           CALL 0x1406bc210
14069d0a6:  49 8B 8F 98 07 00 00     MOV RCX,qword ptr [R15 + 0x798]
14069d0ad:  48 85 C9                 TEST RCX,RCX
14069d0b0:  74 1C                    JZ 0x14069d0ce
14069d0b2:  4C 39 B9 00 01 00 00     CMP qword ptr [RCX + 0x100],R15
14069d0b9:  75 07                    JNZ 0x14069d0c2
14069d0bb:  4C 89 A9 00 01 00 00     MOV qword ptr [RCX + 0x100],R13
14069d0c2:  49 8D 97 48 0D 00 00     LEA RDX,[R15 + 0xd48]
14069d0c9:  E8 02 C8 A5 FF           CALL 0x1400f98d0
14069d0ce:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d0d2:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d0d5:  48 C1 F8 03              SAR RAX,0x3
14069d0d9:  FF C8                    DEC EAX
14069d0db:  41 39 87 00 07 00 00     CMP dword ptr [R15 + 0x700],EAX
14069d0e2:  7C 13                    JL 0x14069d0f7
14069d0e4:  49 8B CF                 MOV RCX,R15
14069d0e7:  E8 B4 F4 01 00           CALL 0x1406bc5a0
14069d0ec:  84 C0                    TEST AL,AL
14069d0ee:  75 07                    JNZ 0x14069d0f7
14069d0f0:  41 FF 87 98 06 00 00     INC dword ptr [R15 + 0x698]
14069d0f7:  49 63 97 00 07 00 00     MOVSXD RDX,dword ptr [R15 + 0x700]
14069d0fe:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d101:  48 8B 0C D0              MOV RCX,qword ptr [RAX + RDX*0x8]
14069d105:  48 83 B9 B8 00 00 00 00  CMP qword ptr [RCX + 0xb8],0x0
14069d10d:  0F 84 16 02 00 00        JZ 0x14069d329
14069d113:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069d117:  48 2B C8                 SUB RCX,RAX
14069d11a:  48 C1 F9 03              SAR RCX,0x3
14069d11e:  8D 42 01                 LEA EAX,[RDX + 0x1]
14069d121:  3B C1                    CMP EAX,ECX
14069d123:  0F 8D 00 02 00 00        JGE 0x14069d329
14069d129:  49 83 BF 00 05 00 00 00  CMP qword ptr [R15 + 0x500],0x0
14069d131:  0F 85 CA 01 00 00        JNZ 0x14069d301
14069d137:  44 89 6D B8              MOV dword ptr [RBP + -0x48],R13D
14069d13b:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069d142:  FF C0                    INC EAX
14069d144:  4C 63 C0                 MOVSXD R8,EAX
14069d147:  48 8B 13                 MOV RDX,qword ptr [RBX]
14069d14a:  49 63 8F 00 07 00 00     MOVSXD RCX,dword ptr [R15 + 0x700]
14069d151:  48 8B C2                 MOV RAX,RDX
14069d154:  4E 8B 0C C2              MOV R9,qword ptr [RDX + R8*0x8]
14069d158:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8]
14069d15c:  48 85 D2                 TEST RDX,RDX
14069d15f:  0F 84 C1 00 00 00        JZ 0x14069d226
14069d165:  48 8B 8A B8 00 00 00     MOV RCX,qword ptr [RDX + 0xb8]
14069d16c:  48 85 C9                 TEST RCX,RCX
14069d16f:  0F 84 B1 00 00 00        JZ 0x14069d226
14069d175:  83 79 14 05              CMP dword ptr [RCX + 0x14],0x5
14069d179:  0F 85 A7 00 00 00        JNZ 0x14069d226
14069d17f:  48 8B 42 10              MOV RAX,qword ptr [RDX + 0x10]
14069d183:  4C 3B 48 08              CMP R9,qword ptr [RAX + 0x8]
14069d187:  75 1B                    JNZ 0x14069d1a4
14069d189:  44 8B 81 B0 01 00 00     MOV R8D,dword ptr [RCX + 0x1b0]
14069d190:  44 89 45 B8              MOV dword ptr [RBP + -0x48],R8D
14069d194:  48 8B 82 B8 00 00 00     MOV RAX,qword ptr [RDX + 0xb8]
14069d19b:  0F BE 88 AB 01 00 00     MOVSX ECX,byte ptr [RAX + 0x1ab]
14069d1a2:  EB 1E                    JMP 0x14069d1c2
14069d1a4:  4C 3B 08                 CMP R9,qword ptr [RAX]
14069d1a7:  75 7D                    JNZ 0x14069d226
14069d1a9:  44 8B 81 B4 01 00 00     MOV R8D,dword ptr [RCX + 0x1b4]
14069d1b0:  44 89 45 B8              MOV dword ptr [RBP + -0x48],R8D
14069d1b4:  48 8B 82 B8 00 00 00     MOV RAX,qword ptr [RDX + 0xb8]
14069d1bb:  0F BE 88 AC 01 00 00     MOVSX ECX,byte ptr [RAX + 0x1ac]
14069d1c2:  83 F9 01                 CMP ECX,0x1
14069d1c5:  75 11                    JNZ 0x14069d1d8
14069d1c7:  45 89 AF 2C 0D 00 00     MOV dword ptr [R15 + 0xd2c],R13D
14069d1ce:  41 C6 87 18 0D 00 00 00  MOV byte ptr [R15 + 0xd18],0x0
14069d1d6:  EB 4E                    JMP 0x14069d226
14069d1d8:  83 F9 2A                 CMP ECX,0x2a
14069d1db:  74 13                    JZ 0x14069d1f0
14069d1dd:  83 F9 27                 CMP ECX,0x27
14069d1e0:  7E 0E                    JLE 0x14069d1f0
14069d1e2:  83 F9 2D                 CMP ECX,0x2d
14069d1e5:  7D 09                    JGE 0x14069d1f0
14069d1e7:  45 89 AF 2C 0D 00 00     MOV dword ptr [R15 + 0xd2c],R13D
14069d1ee:  EB 36                    JMP 0x14069d226
14069d1f0:  83 F9 03                 CMP ECX,0x3
14069d1f3:  75 13                    JNZ 0x14069d208
14069d1f5:  66 41 0F 6E C0           MOVD XMM0,R8D
14069d1fa:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069d1fd:  F3 41 0F 11 87 2C 0D 00 00 MOVSS dword ptr [R15 + 0xd2c],XMM0
14069d206:  EB 1E                    JMP 0x14069d226
14069d208:  83 F9 1D                 CMP ECX,0x1d
14069d20b:  74 11                    JZ 0x14069d21e
14069d20d:  83 F9 1E                 CMP ECX,0x1e
14069d210:  75 14                    JNZ 0x14069d226
14069d212:  49 8B CF                 MOV RCX,R15
14069d215:  E8 D6 02 02 00           CALL 0x1406bd4f0
14069d21a:  84 C0                    TEST AL,AL
14069d21c:  74 08                    JZ 0x14069d226
14069d21e:  41 C6 87 18 0D 00 00 01  MOV byte ptr [R15 + 0xd18],0x1
14069d226:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069d22d:  FF C0                    INC EAX
14069d22f:  4C 63 C0                 MOVSXD R8,EAX
14069d232:  48 8B 13                 MOV RDX,qword ptr [RBX]
14069d235:  49 63 8F 00 07 00 00     MOVSXD RCX,dword ptr [R15 + 0x700]
14069d23c:  48 8B C2                 MOV RAX,RDX
14069d23f:  4E 8B 0C C2              MOV R9,qword ptr [RDX + R8*0x8]
14069d243:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8]
14069d247:  48 85 D2                 TEST RDX,RDX
14069d24a:  0F 84 D9 00 00 00        JZ 0x14069d329
14069d250:  48 8B 8A B8 00 00 00     MOV RCX,qword ptr [RDX + 0xb8]
14069d257:  48 85 C9                 TEST RCX,RCX
14069d25a:  0F 84 C9 00 00 00        JZ 0x14069d329
14069d260:  83 79 14 05              CMP dword ptr [RCX + 0x14],0x5
14069d264:  0F 85 BF 00 00 00        JNZ 0x14069d329
14069d26a:  48 8B 42 10              MOV RAX,qword ptr [RDX + 0x10]
14069d26e:  4C 3B 48 08              CMP R9,qword ptr [RAX + 0x8]
14069d272:  75 1B                    JNZ 0x14069d28f
14069d274:  44 8B 81 B8 01 00 00     MOV R8D,dword ptr [RCX + 0x1b8]
14069d27b:  44 89 45 B8              MOV dword ptr [RBP + -0x48],R8D
14069d27f:  48 8B 82 B8 00 00 00     MOV RAX,qword ptr [RDX + 0xb8]
14069d286:  0F BE 88 C0 01 00 00     MOVSX ECX,byte ptr [RAX + 0x1c0]
14069d28d:  EB 22                    JMP 0x14069d2b1
14069d28f:  4C 3B 08                 CMP R9,qword ptr [RAX]
14069d292:  0F 85 91 00 00 00        JNZ 0x14069d329
14069d298:  44 8B 81 BC 01 00 00     MOV R8D,dword ptr [RCX + 0x1bc]
14069d29f:  44 89 45 B8              MOV dword ptr [RBP + -0x48],R8D
14069d2a3:  48 8B 82 B8 00 00 00     MOV RAX,qword ptr [RDX + 0xb8]
14069d2aa:  0F BE 88 C1 01 00 00     MOVSX ECX,byte ptr [RAX + 0x1c1]
14069d2b1:  83 F9 01                 CMP ECX,0x1
14069d2b4:  75 09                    JNZ 0x14069d2bf
14069d2b6:  45 89 AF 2C 0D 00 00     MOV dword ptr [R15 + 0xd2c],R13D
14069d2bd:  EB 62                    JMP 0x14069d321
14069d2bf:  83 F9 03                 CMP ECX,0x3
14069d2c2:  75 13                    JNZ 0x14069d2d7
14069d2c4:  66 41 0F 6E C0           MOVD XMM0,R8D
14069d2c9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069d2cc:  F3 41 0F 11 87 2C 0D 00 00 MOVSS dword ptr [R15 + 0xd2c],XMM0
14069d2d5:  EB 52                    JMP 0x14069d329
14069d2d7:  83 F9 1D                 CMP ECX,0x1d
14069d2da:  75 0A                    JNZ 0x14069d2e6
14069d2dc:  41 C6 87 18 0D 00 00 01  MOV byte ptr [R15 + 0xd18],0x1
14069d2e4:  EB 43                    JMP 0x14069d329
14069d2e6:  83 F9 1E                 CMP ECX,0x1e
14069d2e9:  75 3E                    JNZ 0x14069d329
14069d2eb:  49 8B CF                 MOV RCX,R15
14069d2ee:  E8 FD 01 02 00           CALL 0x1406bd4f0
14069d2f3:  84 C0                    TEST AL,AL
14069d2f5:  74 32                    JZ 0x14069d329
14069d2f7:  41 C6 87 18 0D 00 00 01  MOV byte ptr [R15 + 0xd18],0x1
14069d2ff:  EB 28                    JMP 0x14069d329
14069d301:  F3 41 0F 10 87 2C 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd2c]
14069d30a:  41 0F 2F C2              COMISS XMM0,XMM10
14069d30e:  76 07                    JBE 0x14069d317
14069d310:  45 89 AF 2C 0D 00 00     MOV dword ptr [R15 + 0xd2c],R13D
14069d317:  41 80 BF 18 0D 00 00 00  CMP byte ptr [R15 + 0xd18],0x0
14069d31f:  74 08                    JZ 0x14069d329
14069d321:  41 C6 87 18 0D 00 00 00  MOV byte ptr [R15 + 0xd18],0x0
14069d329:  41 FF 87 00 07 00 00     INC dword ptr [R15 + 0x700]
14069d330:  49 63 97 00 07 00 00     MOVSXD RDX,dword ptr [R15 + 0x700]
14069d337:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d33b:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d33e:  48 C1 F8 03              SAR RAX,0x3
14069d342:  3B D0                    CMP EDX,EAX
14069d344:  0F 8C 2E 0B 00 00        JL 0x14069de78
14069d34a:  45 32 DB                 XOR R11B,R11B
14069d34d:  4D 8B 87 E8 05 00 00     MOV R8,qword ptr [R15 + 0x5e8]
14069d354:  4D 85 C0                 TEST R8,R8
14069d357:  74 55                    JZ 0x14069d3ae
14069d359:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d35d:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d360:  48 C1 F8 03              SAR RAX,0x3
14069d364:  FF C8                    DEC EAX
14069d366:  48 63 C8                 MOVSXD RCX,EAX
14069d369:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d36c:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14069d370:  48 8B 42 78              MOV RAX,qword ptr [RDX + 0x78]
14069d374:  48 2B 42 70              SUB RAX,qword ptr [RDX + 0x70]
14069d378:  48 C1 F8 03              SAR RAX,0x3
14069d37c:  48 85 C0                 TEST RAX,RAX
14069d37f:  0F 84 AB 0A 00 00        JZ 0x14069de30
14069d385:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d389:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d38c:  48 C1 F8 03              SAR RAX,0x3
14069d390:  FF C8                    DEC EAX
14069d392:  48 63 C8                 MOVSXD RCX,EAX
14069d395:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d398:  48 8B 04 C8              MOV RAX,qword ptr [RAX + RCX*0x8]
14069d39c:  48 8B 40 70              MOV RAX,qword ptr [RAX + 0x70]
14069d3a0:  4C 39 00                 CMP qword ptr [RAX],R8
14069d3a3:  0F 85 87 0A 00 00        JNZ 0x14069de30
14069d3a9:  E9 29 08 00 00           JMP 0x14069dbd7
14069d3ae:  49 8B CF                 MOV RCX,R15
14069d3b1:  E8 4A 56 02 00           CALL 0x1406c2a00
14069d3b6:  84 C0                    TEST AL,AL
14069d3b8:  74 4F                    JZ 0x14069d409
14069d3ba:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d3be:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d3c1:  48 C1 F8 03              SAR RAX,0x3
14069d3c5:  FF C8                    DEC EAX
14069d3c7:  48 63 C8                 MOVSXD RCX,EAX
14069d3ca:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d3cd:  4C 8B 14 C8              MOV R10,qword ptr [RAX + RCX*0x8]
14069d3d1:  4D 85 D2                 TEST R10,R10
14069d3d4:  0F 84 56 0A 00 00        JZ 0x14069de30
14069d3da:  49 8B 42 78              MOV RAX,qword ptr [R10 + 0x78]
14069d3de:  49 2B 42 70              SUB RAX,qword ptr [R10 + 0x70]
14069d3e2:  48 C1 F8 03              SAR RAX,0x3
14069d3e6:  48 85 C0                 TEST RAX,RAX
14069d3e9:  0F 84 41 0A 00 00        JZ 0x14069de30
14069d3ef:  49 8B CF                 MOV RCX,R15
14069d3f2:  E8 E9 53 02 00           CALL 0x1406c27e0
14069d3f7:  49 8B 4A 70              MOV RCX,qword ptr [R10 + 0x70]
14069d3fb:  48 39 01                 CMP qword ptr [RCX],RAX
14069d3fe:  0F 84 D3 07 00 00        JZ 0x14069dbd7
14069d404:  E9 27 0A 00 00           JMP 0x14069de30
14069d409:  49 63 BF 98 06 00 00     MOVSXD RDI,dword ptr [R15 + 0x698]
14069d410:  85 FF                    TEST EDI,EDI
14069d412:  0F 88 18 0A 00 00        JS 0x14069de30
14069d418:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d41f:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d426:  48 C1 F8 03              SAR RAX,0x3
14069d42a:  48 85 C0                 TEST RAX,RAX
14069d42d:  0F 84 FD 09 00 00        JZ 0x14069de30
14069d433:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d43a:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d441:  48 C1 F8 03              SAR RAX,0x3
14069d445:  3B F8                    CMP EDI,EAX
14069d447:  0F 8D E3 09 00 00        JGE 0x14069de30
14069d44d:  4C 8B CF                 MOV R9,RDI
14069d450:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d457:  48 83 3C F8 00           CMP qword ptr [RAX + RDI*0x8],0x0
14069d45c:  0F 85 D2 00 00 00        JNZ 0x14069d534
14069d462:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d466:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d469:  48 C1 F8 03              SAR RAX,0x3
14069d46d:  FF C8                    DEC EAX
14069d46f:  4C 63 C0                 MOVSXD R8,EAX
14069d472:  48 8B 13                 MOV RDX,qword ptr [RBX]
14069d475:  49 69 C9 98 01 00 00     IMUL RCX,R9,0x198
14069d47c:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069d483:  48 8B 8C 01 90 01 00 00  MOV RCX,qword ptr [RCX + RAX*0x1 + 0x190]
14069d48b:  4A 39 0C C2              CMP qword ptr [RDX + R8*0x8],RCX
14069d48f:  0F 94 C2                 SETZ DL
14069d492:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d499:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d4a0:  48 C1 F8 03              SAR RAX,0x3
14069d4a4:  48 83 F8 02              CMP RAX,0x2
14069d4a8:  73 51                    JNC 0x14069d4fb
14069d4aa:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d4b1:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d4b8:  48 C1 F8 03              SAR RAX,0x3
14069d4bc:  48 85 C0                 TEST RAX,RAX
14069d4bf:  74 3A                    JZ 0x14069d4fb
14069d4c1:  49 69 C9 98 01 00 00     IMUL RCX,R9,0x198
14069d4c8:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069d4cf:  4C 8D 05 DA 81 3B 00     LEA R8,[0x140a556b0]
14069d4d6:  4C 39 84 01 90 01 00 00  CMP qword ptr [RCX + RAX*0x1 + 0x190],R8
14069d4de:  75 1B                    JNZ 0x14069d4fb
14069d4e0:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069d4e8:  0F 84 17 0D 00 00        JZ 0x14069e205
14069d4ee:  41 C6 87 3A 0D 00 00 00  MOV byte ptr [R15 + 0xd3a],0x0
14069d4f6:  E9 03 0C 00 00           JMP 0x14069e0fe
14069d4fb:  84 D2                    TEST DL,DL
14069d4fd:  74 29                    JZ 0x14069d528
14069d4ff:  8D 4F 01                 LEA ECX,[RDI + 0x1]
14069d502:  41 89 8F 98 06 00 00     MOV dword ptr [R15 + 0x698],ECX
14069d509:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d510:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d517:  48 C1 F8 03              SAR RAX,0x3
14069d51b:  3B C8                    CMP ECX,EAX
14069d51d:  41 0F 4D CD              CMOVGE ECX,R13D
14069d521:  41 89 8F 98 06 00 00     MOV dword ptr [R15 + 0x698],ECX
14069d528:  41 FF 8F 00 07 00 00     DEC dword ptr [R15 + 0x700]
14069d52f:  E9 CA 0B 00 00           JMP 0x14069e0fe
14069d534:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d53b:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d53f:  48 83 B9 18 03 00 00 00  CMP qword ptr [RCX + 0x318],0x0
14069d547:  0F 85 12 02 00 00        JNZ 0x14069d75f
14069d54d:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d551:  48 8B 91 D0 05 00 00     MOV RDX,qword ptr [RCX + 0x5d0]
14069d558:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d55c:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d55f:  48 C1 F8 03              SAR RAX,0x3
14069d563:  FF C8                    DEC EAX
14069d565:  48 63 C8                 MOVSXD RCX,EAX
14069d568:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d56b:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d56f:  48 39 4A 20              CMP qword ptr [RDX + 0x20],RCX
14069d573:  74 1D                    JZ 0x14069d592
14069d575:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d579:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d57c:  48 C1 F8 03              SAR RAX,0x3
14069d580:  FF C8                    DEC EAX
14069d582:  48 63 C8                 MOVSXD RCX,EAX
14069d585:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d588:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d58c:  48 39 4A 28              CMP qword ptr [RDX + 0x28],RCX
14069d590:  75 03                    JNZ 0x14069d595
14069d592:  41 B3 01                 MOV R11B,0x1
14069d595:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d599:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d59c:  48 C1 F8 03              SAR RAX,0x3
14069d5a0:  4C 8B 42 20              MOV R8,qword ptr [RDX + 0x20]
14069d5a4:  FF C8                    DEC EAX
14069d5a6:  48 63 C8                 MOVSXD RCX,EAX
14069d5a9:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d5ac:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d5b0:  49 39 88 D0 00 00 00     CMP qword ptr [R8 + 0xd0],RCX
14069d5b7:  0F 84 13 06 00 00        JZ 0x14069dbd0
14069d5bd:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d5c1:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d5c4:  48 C1 F8 03              SAR RAX,0x3
14069d5c8:  4C 8B 52 28              MOV R10,qword ptr [RDX + 0x28]
14069d5cc:  FF C8                    DEC EAX
14069d5ce:  48 63 C8                 MOVSXD RCX,EAX
14069d5d1:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d5d4:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d5d8:  49 39 8A D0 00 00 00     CMP qword ptr [R10 + 0xd0],RCX
14069d5df:  0F 84 EB 05 00 00        JZ 0x14069dbd0
14069d5e5:  45 84 DB                 TEST R11B,R11B
14069d5e8:  0F 85 E2 05 00 00        JNZ 0x14069dbd0
14069d5ee:  8B 8A 2C 01 00 00        MOV ECX,dword ptr [RDX + 0x12c]
14069d5f4:  8D 41 FC                 LEA EAX,[RCX + -0x4]
14069d5f7:  83 F8 05                 CMP EAX,0x5
14069d5fa:  76 0C                    JBE 0x14069d608
14069d5fc:  8D 41 F3                 LEA EAX,[RCX + -0xd]
14069d5ff:  83 F8 03                 CMP EAX,0x3
14069d602:  0F 87 28 08 00 00        JA 0x14069de30
14069d608:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d60c:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d60f:  48 C1 F8 03              SAR RAX,0x3
14069d613:  FF C8                    DEC EAX
14069d615:  48 63 C8                 MOVSXD RCX,EAX
14069d618:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d61b:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14069d61f:  48 8B 42 78              MOV RAX,qword ptr [RDX + 0x78]
14069d623:  48 2B 42 70              SUB RAX,qword ptr [RDX + 0x70]
14069d627:  48 C1 F8 03              SAR RAX,0x3
14069d62b:  48 85 C0                 TEST RAX,RAX
14069d62e:  0F 84 88 00 00 00        JZ 0x14069d6bc
14069d634:  49 8B 40 78              MOV RAX,qword ptr [R8 + 0x78]
14069d638:  49 2B 40 70              SUB RAX,qword ptr [R8 + 0x70]
14069d63c:  48 C1 F8 03              SAR RAX,0x3
14069d640:  48 85 C0                 TEST RAX,RAX
14069d643:  74 32                    JZ 0x14069d677
14069d645:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d649:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d64c:  48 C1 F8 03              SAR RAX,0x3
14069d650:  FF C8                    DEC EAX
14069d652:  48 63 C8                 MOVSXD RCX,EAX
14069d655:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d658:  48 8B 04 C8              MOV RAX,qword ptr [RAX + RCX*0x8]
14069d65c:  49 8B 48 70              MOV RCX,qword ptr [R8 + 0x70]
14069d660:  48 8B 40 70              MOV RAX,qword ptr [RAX + 0x70]
14069d664:  45 0F B6 DB              MOVZX R11D,R11B
14069d668:  48 8B 00                 MOV RAX,qword ptr [RAX]
14069d66b:  48 39 01                 CMP qword ptr [RCX],RAX
14069d66e:  B8 01 00 00 00           MOV EAX,0x1
14069d673:  44 0F 44 D8              CMOVZ R11D,EAX
14069d677:  49 8B 42 78              MOV RAX,qword ptr [R10 + 0x78]
14069d67b:  49 2B 42 70              SUB RAX,qword ptr [R10 + 0x70]
14069d67f:  48 C1 F8 03              SAR RAX,0x3
14069d683:  48 85 C0                 TEST RAX,RAX
14069d686:  74 2B                    JZ 0x14069d6b3
14069d688:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d68c:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d68f:  48 C1 F8 03              SAR RAX,0x3
14069d693:  FF C8                    DEC EAX
14069d695:  48 63 C8                 MOVSXD RCX,EAX
14069d698:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d69b:  48 8B 04 C8              MOV RAX,qword ptr [RAX + RCX*0x8]
14069d69f:  49 8B 52 70              MOV RDX,qword ptr [R10 + 0x70]
14069d6a3:  48 8B 40 70              MOV RAX,qword ptr [RAX + 0x70]
14069d6a7:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069d6aa:  48 39 0A                 CMP qword ptr [RDX],RCX
14069d6ad:  0F 84 1D 05 00 00        JZ 0x14069dbd0
14069d6b3:  45 84 DB                 TEST R11B,R11B
14069d6b6:  0F 85 14 05 00 00        JNZ 0x14069dbd0
14069d6bc:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d6c3:  4C 8B 04 F8              MOV R8,qword ptr [RAX + RDI*0x8]
14069d6c7:  45 8B CD                 MOV R9D,R13D
14069d6ca:  49 8B 88 18 0A 00 00     MOV RCX,qword ptr [R8 + 0xa18]
14069d6d1:  49 2B 88 10 0A 00 00     SUB RCX,qword ptr [R8 + 0xa10]
14069d6d8:  48 8B C6                 MOV RAX,RSI
14069d6db:  48 F7 E9                 IMUL RCX
14069d6de:  48 C1 FA 04              SAR RDX,0x4
14069d6e2:  48 8B C2                 MOV RAX,RDX
14069d6e5:  48 C1 E8 3F              SHR RAX,0x3f
14069d6e9:  48 03 D0                 ADD RDX,RAX
14069d6ec:  0F 84 3E 07 00 00        JZ 0x14069de30
14069d6f2:  4C 8B 13                 MOV R10,qword ptr [RBX]
14069d6f5:  49 8B CD                 MOV RCX,R13
14069d6f8:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
14069d700:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d704:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d707:  48 C1 F8 03              SAR RAX,0x3
14069d70b:  FF C8                    DEC EAX
14069d70d:  48 98                    CDQE
14069d70f:  48 8D 14 49              LEA RDX,[RCX + RCX*0x2]
14069d713:  48 C1 E2 05              SHL RDX,0x5
14069d717:  49 8B 88 10 0A 00 00     MOV RCX,qword ptr [R8 + 0xa10]
14069d71e:  49 8B 04 C2              MOV RAX,qword ptr [R10 + RAX*0x8]
14069d722:  48 39 44 0A 30           CMP qword ptr [RDX + RCX*0x1 + 0x30],RAX
14069d727:  0F 84 A3 04 00 00        JZ 0x14069dbd0
14069d72d:  41 FF C1                 INC R9D
14069d730:  49 8B 88 18 0A 00 00     MOV RCX,qword ptr [R8 + 0xa18]
14069d737:  49 2B 88 10 0A 00 00     SUB RCX,qword ptr [R8 + 0xa10]
14069d73e:  48 8B C6                 MOV RAX,RSI
14069d741:  48 F7 E9                 IMUL RCX
14069d744:  48 C1 FA 04              SAR RDX,0x4
14069d748:  48 8B C2                 MOV RAX,RDX
14069d74b:  48 C1 E8 3F              SHR RAX,0x3f
14069d74f:  48 03 D0                 ADD RDX,RAX
14069d752:  41 8B C9                 MOV ECX,R9D
14069d755:  48 3B CA                 CMP RCX,RDX
14069d758:  72 A6                    JC 0x14069d700
14069d75a:  E9 D1 06 00 00           JMP 0x14069de30
14069d75f:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d763:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d766:  48 C1 F8 03              SAR RAX,0x3
14069d76a:  FF C8                    DEC EAX
14069d76c:  48 63 C8                 MOVSXD RCX,EAX
14069d76f:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d772:  48 83 3C C8 00           CMP qword ptr [RAX + RCX*0x8],0x0
14069d777:  0F 84 09 01 00 00        JZ 0x14069d886
14069d77d:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d781:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d784:  48 C1 F8 03              SAR RAX,0x3
14069d788:  FF C8                    DEC EAX
14069d78a:  48 63 C8                 MOVSXD RCX,EAX
14069d78d:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d790:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14069d794:  48 8B 42 78              MOV RAX,qword ptr [RDX + 0x78]
14069d798:  48 2B 42 70              SUB RAX,qword ptr [RDX + 0x70]
14069d79c:  48 C1 F8 03              SAR RAX,0x3
14069d7a0:  48 85 C0                 TEST RAX,RAX
14069d7a3:  0F 84 87 06 00 00        JZ 0x14069de30
14069d7a9:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d7ad:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d7b0:  48 C1 F8 03              SAR RAX,0x3
14069d7b4:  FF C8                    DEC EAX
14069d7b6:  48 63 C8                 MOVSXD RCX,EAX
14069d7b9:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d7bc:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d7c0:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d7c7:  48 8B 49 70              MOV RCX,qword ptr [RCX + 0x70]
14069d7cb:  48 8B 04 F8              MOV RAX,qword ptr [RAX + RDI*0x8]
14069d7cf:  48 39 01                 CMP qword ptr [RCX],RAX
14069d7d2:  0F 85 58 06 00 00        JNZ 0x14069de30
14069d7d8:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d7df:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d7e3:  80 B9 98 11 00 00 00     CMP byte ptr [RCX + 0x1198],0x0
14069d7ea:  0F 84 E0 03 00 00        JZ 0x14069dbd0
14069d7f0:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d7f4:  80 B9 A8 0E 00 00 00     CMP byte ptr [RCX + 0xea8],0x0
14069d7fb:  0F 85 CF 03 00 00        JNZ 0x14069dbd0
14069d801:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d808:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d80f:  48 C1 F8 03              SAR RAX,0x3
14069d813:  48 83 F8 01              CMP RAX,0x1
14069d817:  0F 86 B3 03 00 00        JBE 0x14069dbd0
14069d81d:  41 80 BF FD 05 00 00 00  CMP byte ptr [R15 + 0x5fd],0x0
14069d825:  0F 85 A5 03 00 00        JNZ 0x14069dbd0
14069d82b:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d82f:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d832:  48 C1 F8 03              SAR RAX,0x3
14069d836:  48 83 F8 02              CMP RAX,0x2
14069d83a:  0F 86 90 03 00 00        JBE 0x14069dbd0
14069d840:  49 8B 8F F8 04 00 00     MOV RCX,qword ptr [R15 + 0x4f8]
14069d847:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d84e:  48 39 0C F8              CMP qword ptr [RAX + RDI*0x8],RCX
14069d852:  0F 84 78 03 00 00        JZ 0x14069dbd0
14069d858:  48 85 C9                 TEST RCX,RCX
14069d85b:  0F 84 C6 00 00 00        JZ 0x14069d927
14069d861:  48 8B C1                 MOV RAX,RCX
14069d864:  48 8B 89 18 03 00 00     MOV RCX,qword ptr [RCX + 0x318]
14069d86b:  48 85 C9                 TEST RCX,RCX
14069d86e:  0F 84 B3 00 00 00        JZ 0x14069d927
14069d874:  83 B9 60 03 00 00 19     CMP dword ptr [RCX + 0x360],0x19
14069d87b:  0F 85 A6 00 00 00        JNZ 0x14069d927
14069d881:  E9 4A 03 00 00           JMP 0x14069dbd0
14069d886:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d88d:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d891:  80 B9 98 11 00 00 00     CMP byte ptr [RCX + 0x1198],0x0
14069d898:  0F 84 32 03 00 00        JZ 0x14069dbd0
14069d89e:  48 8B 0C F8              MOV RCX,qword ptr [RAX + RDI*0x8]
14069d8a2:  80 B9 A8 0E 00 00 00     CMP byte ptr [RCX + 0xea8],0x0
14069d8a9:  0F 85 21 03 00 00        JNZ 0x14069dbd0
14069d8af:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d8b6:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d8bd:  48 C1 F8 03              SAR RAX,0x3
14069d8c1:  48 83 F8 01              CMP RAX,0x1
14069d8c5:  0F 86 05 03 00 00        JBE 0x14069dbd0
14069d8cb:  41 80 BF FD 05 00 00 00  CMP byte ptr [R15 + 0x5fd],0x0
14069d8d3:  0F 85 F7 02 00 00        JNZ 0x14069dbd0
14069d8d9:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d8dd:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d8e0:  48 C1 F8 03              SAR RAX,0x3
14069d8e4:  48 83 F8 02              CMP RAX,0x2
14069d8e8:  0F 86 E2 02 00 00        JBE 0x14069dbd0
14069d8ee:  49 8B 8F F8 04 00 00     MOV RCX,qword ptr [R15 + 0x4f8]
14069d8f5:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069d8fc:  48 39 0C F8              CMP qword ptr [RAX + RDI*0x8],RCX
14069d900:  0F 84 CA 02 00 00        JZ 0x14069dbd0
14069d906:  48 85 C9                 TEST RCX,RCX
14069d909:  74 1C                    JZ 0x14069d927
14069d90b:  48 8B C1                 MOV RAX,RCX
14069d90e:  48 8B 89 18 03 00 00     MOV RCX,qword ptr [RCX + 0x318]
14069d915:  48 85 C9                 TEST RCX,RCX
14069d918:  74 0D                    JZ 0x14069d927
14069d91a:  83 B9 60 03 00 00 19     CMP dword ptr [RCX + 0x360],0x19
14069d921:  0F 84 A9 02 00 00        JZ 0x14069dbd0
14069d927:  49 83 BF 08 17 00 00 00  CMP qword ptr [R15 + 0x1708],0x0
14069d92f:  74 5B                    JZ 0x14069d98c
14069d931:  41 B8 22 00 00 00        MOV R8D,0x22
14069d937:  49 8B 97 08 17 00 00     MOV RDX,qword ptr [R15 + 0x1708]
14069d93e:  E8 DD 49 D4 FF           CALL 0x1403e2320
14069d943:  84 C0                    TEST AL,AL
14069d945:  74 45                    JZ 0x14069d98c
14069d947:  49 8B 87 C0 06 00 00     MOV RAX,qword ptr [R15 + 0x6c0]
14069d94e:  49 2B 87 B8 06 00 00     SUB RAX,qword ptr [R15 + 0x6b8]
14069d955:  48 C1 F8 03              SAR RAX,0x3
14069d959:  85 C0                    TEST EAX,EAX
14069d95b:  74 2F                    JZ 0x14069d98c
14069d95d:  49 8B 87 C0 06 00 00     MOV RAX,qword ptr [R15 + 0x6c0]
14069d964:  49 2B 87 B8 06 00 00     SUB RAX,qword ptr [R15 + 0x6b8]
14069d96b:  48 C1 F8 03              SAR RAX,0x3
14069d96f:  FF C8                    DEC EAX
14069d971:  48 63 C8                 MOVSXD RCX,EAX
14069d974:  49 8B 87 B8 06 00 00     MOV RAX,qword ptr [R15 + 0x6b8]
14069d97b:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d97f:  83 B9 2C 01 00 00 05     CMP dword ptr [RCX + 0x12c],0x5
14069d986:  0F 84 44 02 00 00        JZ 0x14069dbd0
14069d98c:  8D 4F 01                 LEA ECX,[RDI + 0x1]
14069d98f:  41 89 8F 98 06 00 00     MOV dword ptr [R15 + 0x698],ECX
14069d996:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069d99d:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069d9a4:  48 C1 F8 03              SAR RAX,0x3
14069d9a8:  3B C8                    CMP ECX,EAX
14069d9aa:  41 0F 4D CD              CMOVGE ECX,R13D
14069d9ae:  41 89 8F 98 06 00 00     MOV dword ptr [R15 + 0x698],ECX
14069d9b5:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069d9b9:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069d9bc:  48 C1 F8 03              SAR RAX,0x3
14069d9c0:  FF C8                    DEC EAX
14069d9c2:  41 89 87 00 07 00 00     MOV dword ptr [R15 + 0x700],EAX
14069d9c9:  48 63 C8                 MOVSXD RCX,EAX
14069d9cc:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d9cf:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d9d3:  48 89 8D C8 00 00 00     MOV qword ptr [RBP + 0xc8],RCX
14069d9da:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069d9e1:  FF C8                    DEC EAX
14069d9e3:  48 63 C8                 MOVSXD RCX,EAX
14069d9e6:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069d9e9:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069d9ed:  48 89 8D B8 00 00 00     MOV qword ptr [RBP + 0xb8],RCX
14069d9f4:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069d9fb:  FF C8                    DEC EAX
14069d9fd:  48 63 C8                 MOVSXD RCX,EAX
14069da00:  49 8D BF B8 06 00 00     LEA RDI,[R15 + 0x6b8]
14069da07:  48 8B 07                 MOV RAX,qword ptr [RDI]
14069da0a:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069da0e:  48 89 8D C0 00 00 00     MOV qword ptr [RBP + 0xc0],RCX
14069da15:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069da18:  48 89 43 08              MOV qword ptr [RBX + 0x8],RAX
14069da1c:  48 8D 85 C8 00 00 00     LEA RAX,[RBP + 0xc8]
14069da23:  48 3B 43 08              CMP RAX,qword ptr [RBX + 0x8]
14069da27:  73 46                    JNC 0x14069da6f
14069da29:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069da2c:  48 8D 8D C8 00 00 00     LEA RCX,[RBP + 0xc8]
14069da33:  48 3B C1                 CMP RAX,RCX
14069da36:  77 37                    JA 0x14069da6f
14069da38:  48 8D B5 C8 00 00 00     LEA RSI,[RBP + 0xc8]
14069da3f:  48 2B F0                 SUB RSI,RAX
14069da42:  48 C1 FE 03              SAR RSI,0x3
14069da46:  48 8B 43 10              MOV RAX,qword ptr [RBX + 0x10]
14069da4a:  48 39 43 08              CMP qword ptr [RBX + 0x8],RAX
14069da4e:  75 0D                    JNZ 0x14069da5d
14069da50:  BA 01 00 00 00           MOV EDX,0x1
14069da55:  48 8B CB                 MOV RCX,RBX
14069da58:  E8 A3 3C A1 FF           CALL 0x1400b1700
14069da5d:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069da61:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069da64:  48 85 C9                 TEST RCX,RCX
14069da67:  74 30                    JZ 0x14069da99
14069da69:  48 8B 04 F0              MOV RAX,qword ptr [RAX + RSI*0x8]
14069da6d:  EB 27                    JMP 0x14069da96
14069da6f:  48 8B 43 10              MOV RAX,qword ptr [RBX + 0x10]
14069da73:  48 39 43 08              CMP qword ptr [RBX + 0x8],RAX
14069da77:  75 0D                    JNZ 0x14069da86
14069da79:  BA 01 00 00 00           MOV EDX,0x1
14069da7e:  48 8B CB                 MOV RCX,RBX
14069da81:  E8 7A 3C A1 FF           CALL 0x1400b1700
14069da86:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069da8a:  48 85 C9                 TEST RCX,RCX
14069da8d:  74 0A                    JZ 0x14069da99
14069da8f:  48 8B 85 C8 00 00 00     MOV RAX,qword ptr [RBP + 0xc8]
14069da96:  48 89 01                 MOV qword ptr [RCX],RAX
14069da99:  48 83 43 08 08           ADD qword ptr [RBX + 0x8],0x8
14069da9e:  48 8D 85 B8 00 00 00     LEA RAX,[RBP + 0xb8]
14069daa5:  48 3B 43 08              CMP RAX,qword ptr [RBX + 0x8]
14069daa9:  73 46                    JNC 0x14069daf1
14069daab:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069daae:  48 8D 8D B8 00 00 00     LEA RCX,[RBP + 0xb8]
14069dab5:  48 3B C1                 CMP RAX,RCX
14069dab8:  77 37                    JA 0x14069daf1
14069daba:  48 8D B5 B8 00 00 00     LEA RSI,[RBP + 0xb8]
14069dac1:  48 2B F0                 SUB RSI,RAX
14069dac4:  48 C1 FE 03              SAR RSI,0x3
14069dac8:  48 8B 43 10              MOV RAX,qword ptr [RBX + 0x10]
14069dacc:  48 39 43 08              CMP qword ptr [RBX + 0x8],RAX
14069dad0:  75 0D                    JNZ 0x14069dadf
14069dad2:  BA 01 00 00 00           MOV EDX,0x1
14069dad7:  48 8B CB                 MOV RCX,RBX
14069dada:  E8 21 3C A1 FF           CALL 0x1400b1700
14069dadf:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069dae3:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069dae6:  48 85 C9                 TEST RCX,RCX
14069dae9:  74 30                    JZ 0x14069db1b
14069daeb:  48 8B 04 F0              MOV RAX,qword ptr [RAX + RSI*0x8]
14069daef:  EB 27                    JMP 0x14069db18
14069daf1:  48 8B 43 10              MOV RAX,qword ptr [RBX + 0x10]
14069daf5:  48 39 43 08              CMP qword ptr [RBX + 0x8],RAX
14069daf9:  75 0D                    JNZ 0x14069db08
14069dafb:  BA 01 00 00 00           MOV EDX,0x1
14069db00:  48 8B CB                 MOV RCX,RBX
14069db03:  E8 F8 3B A1 FF           CALL 0x1400b1700
14069db08:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069db0c:  48 85 C9                 TEST RCX,RCX
14069db0f:  74 0A                    JZ 0x14069db1b
14069db11:  48 8B 85 B8 00 00 00     MOV RAX,qword ptr [RBP + 0xb8]
14069db18:  48 89 01                 MOV qword ptr [RCX],RAX
14069db1b:  48 83 43 08 08           ADD qword ptr [RBX + 0x8],0x8
14069db20:  BA 01 00 00 00           MOV EDX,0x1
14069db25:  41 89 97 00 07 00 00     MOV dword ptr [R15 + 0x700],EDX
14069db2c:  48 8B 07                 MOV RAX,qword ptr [RDI]
14069db2f:  48 89 47 08              MOV qword ptr [RDI + 0x8],RAX
14069db33:  48 8D 85 C0 00 00 00     LEA RAX,[RBP + 0xc0]
14069db3a:  48 3B 47 08              CMP RAX,qword ptr [RDI + 0x8]
14069db3e:  73 41                    JNC 0x14069db81
14069db40:  48 8B 07                 MOV RAX,qword ptr [RDI]
14069db43:  48 8D 8D C0 00 00 00     LEA RCX,[RBP + 0xc0]
14069db4a:  48 3B C1                 CMP RAX,RCX
14069db4d:  77 32                    JA 0x14069db81
14069db4f:  48 8D 9D C0 00 00 00     LEA RBX,[RBP + 0xc0]
14069db56:  48 2B D8                 SUB RBX,RAX
14069db59:  48 C1 FB 03              SAR RBX,0x3
14069db5d:  48 8B 47 10              MOV RAX,qword ptr [RDI + 0x10]
14069db61:  48 39 47 08              CMP qword ptr [RDI + 0x8],RAX
14069db65:  75 08                    JNZ 0x14069db6f
14069db67:  48 8B CF                 MOV RCX,RDI
14069db6a:  E8 91 3B A1 FF           CALL 0x1400b1700
14069db6f:  48 8B 4F 08              MOV RCX,qword ptr [RDI + 0x8]
14069db73:  48 8B 07                 MOV RAX,qword ptr [RDI]
14069db76:  48 85 C9                 TEST RCX,RCX
14069db79:  74 2B                    JZ 0x14069dba6
14069db7b:  48 8B 04 D8              MOV RAX,qword ptr [RAX + RBX*0x8]
14069db7f:  EB 22                    JMP 0x14069dba3
14069db81:  48 8B 47 10              MOV RAX,qword ptr [RDI + 0x10]
14069db85:  48 39 47 08              CMP qword ptr [RDI + 0x8],RAX
14069db89:  75 08                    JNZ 0x14069db93
14069db8b:  48 8B CF                 MOV RCX,RDI
14069db8e:  E8 6D 3B A1 FF           CALL 0x1400b1700
14069db93:  48 8B 4F 08              MOV RCX,qword ptr [RDI + 0x8]
14069db97:  48 85 C9                 TEST RCX,RCX
14069db9a:  74 0A                    JZ 0x14069dba6
14069db9c:  48 8B 85 C0 00 00 00     MOV RAX,qword ptr [RBP + 0xc0]
14069dba3:  48 89 01                 MOV qword ptr [RCX],RAX
14069dba6:  48 83 47 08 08           ADD qword ptr [RDI + 0x8],0x8
14069dbab:  45 89 AF 68 07 00 00     MOV dword ptr [R15 + 0x768],R13D
14069dbb2:  41 C6 87 6C 07 00 00 00  MOV byte ptr [R15 + 0x76c],0x0
14069dbba:  45 89 AF 70 07 00 00     MOV dword ptr [R15 + 0x770],R13D
14069dbc1:  33 D2                    XOR EDX,EDX
14069dbc3:  49 8B CF                 MOV RCX,R15
14069dbc6:  E8 D5 55 FE FF           CALL 0x1406831a0
14069dbcb:  E9 35 06 00 00           JMP 0x14069e205
14069dbd0:  0F B6 BD B0 1C 00 00     MOVZX EDI,byte ptr [RBP + 0x1cb0]
14069dbd7:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069dbdf:  74 1E                    JZ 0x14069dbff
14069dbe1:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069dbe8:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069dbef:  75 07                    JNZ 0x14069dbf8
14069dbf1:  4C 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R13
14069dbf8:  4D 89 AF 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R13
14069dbff:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069dc06:  83 F8 01                 CMP EAX,0x1
14069dc09:  7E 3C                    JLE 0x14069dc47
14069dc0b:  48 8B 53 08              MOV RDX,qword ptr [RBX + 0x8]
14069dc0f:  48 2B 13                 SUB RDX,qword ptr [RBX]
14069dc12:  48 C1 FA 03              SAR RDX,0x3
14069dc16:  FF C8                    DEC EAX
14069dc18:  48 63 C8                 MOVSXD RCX,EAX
14069dc1b:  48 3B CA                 CMP RCX,RDX
14069dc1e:  73 27                    JNC 0x14069dc47
14069dc20:  40 84 FF                 TEST DIL,DIL
14069dc23:  75 22                    JNZ 0x14069dc47
14069dc25:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069dc2c:  FF C8                    DEC EAX
14069dc2e:  48 63 C8                 MOVSXD RCX,EAX
14069dc31:  4C 8B 03                 MOV R8,qword ptr [RBX]
14069dc34:  4D 8B 04 C8              MOV R8,qword ptr [R8 + RCX*0x8]
14069dc38:  49 8B 97 98 07 00 00     MOV RDX,qword ptr [R15 + 0x798]
14069dc3f:  49 8B CF                 MOV RCX,R15
14069dc42:  E8 09 DD 01 00           CALL 0x1406bb950
14069dc47:  4D 89 AF 28 05 00 00     MOV qword ptr [R15 + 0x528],R13
14069dc4e:  49 83 BF E8 05 00 00 00  CMP qword ptr [R15 + 0x5e8],0x0
14069dc56:  74 09                    JZ 0x14069dc61
14069dc58:  49 8B 87 E8 05 00 00     MOV RAX,qword ptr [R15 + 0x5e8]
14069dc5f:  EB 14                    JMP 0x14069dc75
14069dc61:  49 8B CF                 MOV RCX,R15
14069dc64:  E8 97 4D 02 00           CALL 0x1406c2a00
14069dc69:  84 C0                    TEST AL,AL
14069dc6b:  74 0F                    JZ 0x14069dc7c
14069dc6d:  49 8B CF                 MOV RCX,R15
14069dc70:  E8 6B 4B 02 00           CALL 0x1406c27e0
14069dc75:  49 89 87 28 05 00 00     MOV qword ptr [R15 + 0x528],RAX
14069dc7c:  49 63 87 98 06 00 00     MOVSXD RAX,dword ptr [R15 + 0x698]
14069dc83:  85 C0                    TEST EAX,EAX
14069dc85:  78 17                    JS 0x14069dc9e
14069dc87:  49 8B 8F 88 06 00 00     MOV RCX,qword ptr [R15 + 0x688]
14069dc8e:  49 2B 8F 80 06 00 00     SUB RCX,qword ptr [R15 + 0x680]
14069dc95:  48 C1 F9 03              SAR RCX,0x3
14069dc99:  48 3B C1                 CMP RAX,RCX
14069dc9c:  72 3C                    JC 0x14069dcda
14069dc9e:  49 83 BF F8 04 00 00 00  CMP qword ptr [R15 + 0x4f8],0x0
14069dca6:  74 32                    JZ 0x14069dcda
14069dca8:  45 33 C9                 XOR R9D,R9D
14069dcab:  41 83 C8 FF              OR R8D,0xffffffff
14069dcaf:  49 8B 97 F8 04 00 00     MOV RDX,qword ptr [R15 + 0x4f8]
14069dcb6:  49 8B CF                 MOV RCX,R15
14069dcb9:  E8 12 F9 FD FF           CALL 0x14067d5d0
14069dcbe:  84 C0                    TEST AL,AL
14069dcc0:  74 18                    JZ 0x14069dcda
14069dcc2:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069dcc9:  C6 00 00                 MOV byte ptr [RAX],0x0
14069dccc:  49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069dcd3:  C6 80 C8 00 00 00 00     MOV byte ptr [RAX + 0xc8],0x0
14069dcda:  49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0x0
14069dce2:  75 19                    JNZ 0x14069dcfd
14069dce4:  49 63 8F 98 06 00 00     MOVSXD RCX,dword ptr [R15 + 0x698]
14069dceb:  49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069dcf2:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
14069dcf6:  49 89 8F 28 05 00 00     MOV qword ptr [R15 + 0x528],RCX
14069dcfd:  41 C7 87 6C 05 00 00 FF FF FF FF MOV dword ptr [R15 + 0x56c],0xffffffff
14069dd08:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069dd0f:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069dd16:  49 C7 87 58 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x558],0x0
14069dd21:  49 C7 87 94 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x594],0x0
14069dd2c:  41 C7 87 68 05 00 00 00 00 80 3F MOV dword ptr [R15 + 0x568],0x3f800000
14069dd37:  45 89 AF 9C 17 00 00     MOV dword ptr [R15 + 0x179c],R13D
14069dd3e:  49 C7 87 60 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x560],0x0
14069dd49:  45 89 AF 10 13 00 00     MOV dword ptr [R15 + 0x1310],R13D
14069dd50:  41 C6 87 9C 05 00 00 00  MOV byte ptr [R15 + 0x59c],0x0
14069dd58:  49 8B CF                 MOV RCX,R15
14069dd5b:  E8 E0 24 01 00           CALL 0x1406b0240
14069dd60:  49 8B D7                 MOV RDX,R15
14069dd63:  48 8D 0D A6 71 33 00     LEA RCX,[0x1409d4f10]
14069dd6a:  E8 31 09 AB FF           CALL 0x14014e6a0
14069dd6f:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069dd76:  49 8B 90 D0 05 00 00     MOV RDX,qword ptr [R8 + 0x5d0]
14069dd7d:  48 85 D2                 TEST RDX,RDX
14069dd80:  0F 84 7F 04 00 00        JZ 0x14069e205
14069dd86:  8B 8A 2C 01 00 00        MOV ECX,dword ptr [RDX + 0x12c]
14069dd8c:  8D 41 FB                 LEA EAX,[RCX + -0x5]
14069dd8f:  83 F8 04                 CMP EAX,0x4
14069dd92:  76 1E                    JBE 0x14069ddb2
14069dd94:  8D 41 F3                 LEA EAX,[RCX + -0xd]
14069dd97:  83 F8 03                 CMP EAX,0x3
14069dd9a:  76 16                    JBE 0x14069ddb2
14069dd9c:  83 F9 04                 CMP ECX,0x4
14069dd9f:  0F 85 60 04 00 00        JNZ 0x14069e205
14069dda5:  83 BA 20 01 00 00 0A     CMP dword ptr [RDX + 0x120],0xa
14069ddac:  0F 8C 53 04 00 00        JL 0x14069e205
14069ddb2:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069ddb6:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069ddb9:  48 C1 F8 03              SAR RAX,0x3
14069ddbd:  48 85 C0                 TEST RAX,RAX
14069ddc0:  0F 84 3F 04 00 00        JZ 0x14069e205
14069ddc6:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069ddca:  48 2B 0B                 SUB RCX,qword ptr [RBX]
14069ddcd:  48 C1 F9 03              SAR RCX,0x3
14069ddd1:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069ddd4:  48 8B 4C C8 F8           MOV RCX,qword ptr [RAX + RCX*0x8 + -0x8]
14069ddd9:  48 8B 41 78              MOV RAX,qword ptr [RCX + 0x78]
14069dddd:  48 2B 41 70              SUB RAX,qword ptr [RCX + 0x70]
14069dde1:  48 C1 F8 03              SAR RAX,0x3
14069dde5:  48 85 C0                 TEST RAX,RAX
14069dde8:  0F 84 17 04 00 00        JZ 0x14069e205
14069ddee:  4D 89 87 38 05 00 00     MOV qword ptr [R15 + 0x538],R8
14069ddf5:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069ddf9:  48 2B 0B                 SUB RCX,qword ptr [RBX]
14069ddfc:  48 C1 F9 03              SAR RCX,0x3
14069de00:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069de03:  48 8B 44 C8 F8           MOV RAX,qword ptr [RAX + RCX*0x8 + -0x8]
14069de08:  48 8B 40 70              MOV RAX,qword ptr [RAX + 0x70]
14069de0c:  48 8B 08                 MOV RCX,qword ptr [RAX]
14069de0f:  49 89 8F 28 05 00 00     MOV qword ptr [R15 + 0x528],RCX
14069de16:  49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069de1d:  49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069de24:  45 89 AF 58 05 00 00     MOV dword ptr [R15 + 0x558],R13D
14069de2b:  E9 D5 03 00 00           JMP 0x14069e205
14069de30:  49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069de37:  49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069de3e:  48 C1 F8 03              SAR RAX,0x3
14069de42:  41 39 87 98 06 00 00     CMP dword ptr [R15 + 0x698],EAX
14069de49:  7C 07                    JL 0x14069de52
14069de4b:  45 89 AF 98 06 00 00     MOV dword ptr [R15 + 0x698],R13D
14069de52:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069de5a:  74 08                    JZ 0x14069de64
14069de5c:  41 C6 87 3A 0D 00 00 00  MOV byte ptr [R15 + 0xd3a],0x0
14069de64:  41 C6 87 3C 0D 00 00 00  MOV byte ptr [R15 + 0xd3c],0x0
14069de6c:  41 FF 8F 00 07 00 00     DEC dword ptr [R15 + 0x700]
14069de73:  E9 86 02 00 00           JMP 0x14069e0fe
14069de78:  41 80 BF 7D 06 00 00 00  CMP byte ptr [R15 + 0x67d],0x0
14069de80:  74 4C                    JZ 0x14069dece
14069de82:  85 D2                    TEST EDX,EDX
14069de84:  78 4A                    JS 0x14069ded0
14069de86:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069de89:  48 83 3C D0 00           CMP qword ptr [RAX + RDX*0x8],0x0
14069de8e:  74 3E                    JZ 0x14069dece
14069de90:  8D 42 FF                 LEA EAX,[RDX + -0x1]
14069de93:  4C 63 C0                 MOVSXD R8,EAX
14069de96:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069de99:  4A 83 3C C0 00           CMP qword ptr [RAX + R8*0x8],0x0
14069de9e:  74 2E                    JZ 0x14069dece
14069dea0:  48 8B 0C D0              MOV RCX,qword ptr [RAX + RDX*0x8]
14069dea4:  83 B9 E0 00 00 00 01     CMP dword ptr [RCX + 0xe0],0x1
14069deab:  75 21                    JNZ 0x14069dece
14069dead:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14069deb1:  83 B9 E0 00 00 00 01     CMP dword ptr [RCX + 0xe0],0x1
14069deb8:  75 14                    JNZ 0x14069dece
14069deba:  49 8B 97 98 07 00 00     MOV RDX,qword ptr [R15 + 0x798]
14069dec1:  49 8B CF                 MOV RCX,R15
14069dec4:  E8 B7 F3 FD FF           CALL 0x14067d280
14069dec9:  E9 37 03 00 00           JMP 0x14069e205
14069dece:  85 D2                    TEST EDX,EDX
14069ded0:  7E 4D                    JLE 0x14069df1f
14069ded2:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
14069ded6:  48 2B 03                 SUB RAX,qword ptr [RBX]
14069ded9:  48 C1 F8 03              SAR RAX,0x3
14069dedd:  3B D0                    CMP EDX,EAX
14069dedf:  7D 3E                    JGE 0x14069df1f
14069dee1:  49 8D 8F E8 06 00 00     LEA RCX,[R15 + 0x6e8]
14069dee8:  4C 8B C2                 MOV R8,RDX
14069deeb:  E8 90 E4 02 00           CALL 0x1406cc380
14069def0:  4C 3B C0                 CMP R8,RAX
14069def3:  73 18                    JNC 0x14069df0d
14069def5:  E8 86 E4 02 00           CALL 0x1406cc380
14069defa:  48 83 F8 01              CMP RAX,0x1
14069defe:  72 0D                    JC 0x14069df0d
14069df00:  49 8B D0                 MOV RDX,R8
14069df03:  E8 68 E4 02 00           CALL 0x1406cc370
14069df08:  80 38 00                 CMP byte ptr [RAX],0x0
14069df0b:  75 12                    JNZ 0x14069df1f
14069df0d:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069df10:  4A 83 3C C0 00           CMP qword ptr [RAX + R8*0x8],0x0
14069df15:  75 08                    JNZ 0x14069df1f
14069df17:  41 C6 87 3A 0D 00 00 01  MOV byte ptr [R15 + 0xd3a],0x1
14069df1f:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069df27:  0F 85 A3 01 00 00        JNZ 0x14069e0d0
14069df2d:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069df34:  85 C0                    TEST EAX,EAX
14069df36:  0F 88 86 00 00 00        JS 0x14069dfc2
14069df3c:  48 8B 53 08              MOV RDX,qword ptr [RBX + 0x8]
14069df40:  48 2B 13                 SUB RDX,qword ptr [RBX]
14069df43:  48 C1 FA 03              SAR RDX,0x3
14069df47:  FF C0                    INC EAX
14069df49:  48 63 C8                 MOVSXD RCX,EAX
14069df4c:  48 3B CA                 CMP RCX,RDX
14069df4f:  73 71                    JNC 0x14069dfc2
14069df51:  49 83 BF F8 04 00 00 00  CMP qword ptr [R15 + 0x4f8],0x0
14069df59:  74 32                    JZ 0x14069df8d
14069df5b:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069df62:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069df69:  48 85 C9                 TEST RCX,RCX
14069df6c:  74 1F                    JZ 0x14069df8d
14069df6e:  83 B9 60 03 00 00 31     CMP dword ptr [RCX + 0x360],0x31
14069df75:  75 16                    JNZ 0x14069df8d
14069df77:  41 B8 23 00 00 00        MOV R8D,0x23
14069df7d:  49 8B 97 08 17 00 00     MOV RDX,qword ptr [R15 + 0x1708]
14069df84:  E8 97 43 D4 FF           CALL 0x1403e2320
14069df89:  84 C0                    TEST AL,AL
14069df8b:  75 35                    JNZ 0x14069dfc2
14069df8d:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069df94:  FF C0                    INC EAX
14069df96:  48 63 C8                 MOVSXD RCX,EAX
14069df99:  4C 8B 03                 MOV R8,qword ptr [RBX]
14069df9c:  49 63 87 00 07 00 00     MOVSXD RAX,dword ptr [R15 + 0x700]
14069dfa3:  49 8B D0                 MOV RDX,R8
14069dfa6:  4D 8B CF                 MOV R9,R15
14069dfa9:  4D 8B 04 C8              MOV R8,qword ptr [R8 + RCX*0x8]
14069dfad:  48 8B 14 C2              MOV RDX,qword ptr [RDX + RAX*0x8]
14069dfb1:  E8 EA E6 D7 FF           CALL 0x14041c6a0
14069dfb6:  84 C0                    TEST AL,AL
14069dfb8:  74 08                    JZ 0x14069dfc2
14069dfba:  41 C6 87 3A 0D 00 00 01  MOV byte ptr [R15 + 0xd3a],0x1
14069dfc2:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069dfca:  0F 85 00 01 00 00        JNZ 0x14069e0d0
14069dfd0:  80 3D 87 B3 34 00 00     CMP byte ptr [0x1409e935e],0x0
14069dfd7:  0F 84 F3 00 00 00        JZ 0x14069e0d0
14069dfdd:  45 0F 2F 97 88 0C 00 00  COMISS XMM10,dword ptr [R15 + 0xc88]
14069dfe5:  0F 82 E5 00 00 00        JC 0x14069e0d0
14069dfeb:  41 0F 28 C2              MOVAPS XMM0,XMM10
14069dfef:  45 8B 8F 00 07 00 00     MOV R9D,dword ptr [R15 + 0x700]
14069dff6:  41 FF C9                 DEC R9D
14069dff9:  49 8B 87 C0 06 00 00     MOV RAX,qword ptr [R15 + 0x6c0]
14069e000:  49 2B 87 B8 06 00 00     SUB RAX,qword ptr [R15 + 0x6b8]
14069e007:  48 C1 F8 03              SAR RAX,0x3
14069e00b:  4D 63 C1                 MOVSXD R8,R9D
14069e00e:  4C 3B C0                 CMP R8,RAX
14069e011:  0F 83 AE 00 00 00        JNC 0x14069e0c5
14069e017:  4E 8D 04 C5 00 00 00 00  LEA R8,[R8*0x8]
14069e01f:  F3 44 0F 10 1D F4 CD 26 00 MOVSS XMM11,dword ptr [0x14090ae1c]
14069e028:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
14069e030:  49 8B 87 B8 06 00 00     MOV RAX,qword ptr [R15 + 0x6b8]
14069e037:  49 8B 14 00              MOV RDX,qword ptr [R8 + RAX*0x1]
14069e03b:  48 85 D2                 TEST RDX,RDX
14069e03e:  74 57                    JZ 0x14069e097
14069e040:  83 BA 2C 01 00 00 00     CMP dword ptr [RDX + 0x12c],0x0
14069e047:  75 36                    JNZ 0x14069e07f
14069e049:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069e04c:  49 8B 0C 00              MOV RCX,qword ptr [R8 + RAX*0x1]
14069e050:  48 39 4A 20              CMP qword ptr [RDX + 0x20],RCX
14069e054:  75 0A                    JNZ 0x14069e060
14069e056:  44 0F 2F BA 08 01 00 00  COMISS XMM15,dword ptr [RDX + 0x108]
14069e05e:  EB 15                    JMP 0x14069e075
14069e060:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069e063:  49 8B 0C 00              MOV RCX,qword ptr [R8 + RAX*0x1]
14069e067:  48 39 4A 28              CMP qword ptr [RDX + 0x28],RCX
14069e06b:  75 12                    JNZ 0x14069e07f
14069e06d:  44 0F 2F BA 0C 01 00 00  COMISS XMM15,dword ptr [RDX + 0x10c]
14069e075:  72 08                    JC 0x14069e07f
14069e077:  41 C6 87 3A 0D 00 00 01  MOV byte ptr [R15 + 0xd3a],0x1
14069e07f:  F3 0F 58 82 1C 01 00 00  ADDSS XMM0,dword ptr [RDX + 0x11c]
14069e087:  41 0F 2F C3              COMISS XMM0,XMM11
14069e08b:  77 2F                    JA 0x14069e0bc
14069e08d:  41 80 BF 3A 0D 00 00 00  CMP byte ptr [R15 + 0xd3a],0x0
14069e095:  75 25                    JNZ 0x14069e0bc
14069e097:  41 FF C1                 INC R9D
14069e09a:  49 83 C0 08              ADD R8,0x8
14069e09e:  49 8B 8F C0 06 00 00     MOV RCX,qword ptr [R15 + 0x6c0]
14069e0a5:  49 2B 8F B8 06 00 00     SUB RCX,qword ptr [R15 + 0x6b8]
14069e0ac:  48 C1 F9 03              SAR RCX,0x3
14069e0b0:  49 63 C1                 MOVSXD RAX,R9D
14069e0b3:  48 3B C1                 CMP RAX,RCX
14069e0b6:  0F 82 74 FF FF FF        JC 0x14069e030
14069e0bc:  F3 44 0F 10 1D E3 C5 26 00 MOVSS XMM11,dword ptr [0x14090a6a8]
14069e0c5:  41 C7 87 88 0C 00 00 00 00 A0 41 MOV dword ptr [R15 + 0xc88],0x41a00000
14069e0d0:  41 80 BF 3A 0D 00 00 01  CMP byte ptr [R15 + 0xd3a],0x1
14069e0d8:  75 41                    JNZ 0x14069e11b
14069e0da:  41 C6 87 3A 0D 00 00 00  MOV byte ptr [R15 + 0xd3a],0x0
14069e0e2:  41 FF 8F 00 07 00 00     DEC dword ptr [R15 + 0x700]
14069e0e9:  41 8B 8F 00 07 00 00     MOV ECX,dword ptr [R15 + 0x700]
14069e0f0:  8B C1                    MOV EAX,ECX
14069e0f2:  85 C9                    TEST ECX,ECX
14069e0f4:  0F 4E C1                 CMOVLE EAX,ECX
14069e0f7:  41 89 87 00 07 00 00     MOV dword ptr [R15 + 0x700],EAX
14069e0fe:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
14069e103:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14069e108:  41 83 C8 FF              OR R8D,0xffffffff
14069e10c:  B2 01                    MOV DL,0x1
14069e10e:  49 8B CF                 MOV RCX,R15
14069e111:  E8 0A 29 FE FF           CALL 0x140680a20
14069e116:  E9 EA 00 00 00           JMP 0x14069e205
14069e11b:  45 89 AF 68 07 00 00     MOV dword ptr [R15 + 0x768],R13D
14069e122:  41 C6 87 6C 07 00 00 00  MOV byte ptr [R15 + 0x76c],0x0
14069e12a:  45 89 AF 70 07 00 00     MOV dword ptr [R15 + 0x770],R13D
14069e131:  33 D2                    XOR EDX,EDX
14069e133:  49 8B CF                 MOV RCX,R15
14069e136:  E8 65 50 FE FF           CALL 0x1406831a0
14069e13b:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069e142:  85 C0                    TEST EAX,EAX
14069e144:  7E 54                    JLE 0x14069e19a
14069e146:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14069e14a:  48 2B 0B                 SUB RCX,qword ptr [RBX]
14069e14d:  48 C1 F9 03              SAR RCX,0x3
14069e151:  FF C8                    DEC EAX
14069e153:  3B C1                    CMP EAX,ECX
14069e155:  7D 43                    JGE 0x14069e19a
14069e157:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069e15e:  FF C8                    DEC EAX
14069e160:  48 63 C8                 MOVSXD RCX,EAX
14069e163:  48 8B 03                 MOV RAX,qword ptr [RBX]
14069e166:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14069e16a:  48 8B 42 18              MOV RAX,qword ptr [RDX + 0x18]
14069e16e:  48 2B 42 10              SUB RAX,qword ptr [RDX + 0x10]
14069e172:  48 C1 F8 03              SAR RAX,0x3
14069e176:  48 83 F8 02              CMP RAX,0x2
14069e17a:  76 1E                    JBE 0x14069e19a
14069e17c:  41 C6 87 18 0D 00 00 00  MOV byte ptr [R15 + 0xd18],0x0
14069e184:  F3 41 0F 10 87 2C 0D 00 00 MOVSS XMM0,dword ptr [R15 + 0xd2c]
14069e18d:  41 0F 2F C2              COMISS XMM0,XMM10
14069e191:  76 07                    JBE 0x14069e19a
14069e193:  45 89 AF 2C 0D 00 00     MOV dword ptr [R15 + 0xd2c],R13D
14069e19a:  49 83 BF 00 05 00 00 00  CMP qword ptr [R15 + 0x500],0x0
14069e1a2:  75 61                    JNZ 0x14069e205
14069e1a4:  49 83 BF 98 07 00 00 00  CMP qword ptr [R15 + 0x798],0x0
14069e1ac:  74 57                    JZ 0x14069e205
14069e1ae:  45 0F 2F 97 2C 0D 00 00  COMISS XMM10,dword ptr [R15 + 0xd2c]
14069e1b6:  72 4D                    JC 0x14069e205
14069e1b8:  49 8B 8F 98 07 00 00     MOV RCX,qword ptr [R15 + 0x798]
14069e1bf:  48 83 B9 30 03 00 00 00  CMP qword ptr [RCX + 0x330],0x0
14069e1c7:  74 14                    JZ 0x14069e1dd
14069e1c9:  48 8B 81 30 03 00 00     MOV RAX,qword ptr [RCX + 0x330]
14069e1d0:  8B 50 04                 MOV EDX,dword ptr [RAX + 0x4]
14069e1d3:  85 D2                    TEST EDX,EDX
14069e1d5:  7E 06                    JLE 0x14069e1dd
14069e1d7:  66 0F 6E C2              MOVD XMM0,EDX
14069e1db:  EB 1C                    JMP 0x14069e1f9
14069e1dd:  48 83 B9 28 03 00 00 00  CMP qword ptr [RCX + 0x328],0x0
14069e1e5:  74 1E                    JZ 0x14069e205
14069e1e7:  48 8B 81 28 03 00 00     MOV RAX,qword ptr [RCX + 0x328]
14069e1ee:  8B 48 04                 MOV ECX,dword ptr [RAX + 0x4]
14069e1f1:  85 C9                    TEST ECX,ECX
14069e1f3:  7E 10                    JLE 0x14069e205
14069e1f5:  66 0F 6E C1              MOVD XMM0,ECX
14069e1f9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14069e1fc:  F3 41 0F 11 87 2C 0D 00 00 MOVSS dword ptr [R15 + 0xd2c],XMM0

