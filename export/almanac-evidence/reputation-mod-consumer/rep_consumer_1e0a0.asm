; Range: 0x18001E0A0 to 0x18001E4C0  (containing fn: FUN_18001e0a0@0x18001E0A0)
18001e0a0:  4C 8B DC                                  MOV R11,RSP
18001e0a3:  55                                        PUSH RBP
18001e0a4:  49 8D AB 88 FB FF FF                      LEA RBP,[R11 + -0x478]
18001e0ab:  48 81 EC 70 05 00 00                      SUB RSP,0x570
18001e0b2:  48 8B 05 C7 60 0A 00                      MOV RAX,qword ptr [0x1800c4180]
18001e0b9:  48 33 C4                                  XOR RAX,RSP
18001e0bc:  48 89 85 A8 03 00 00                      MOV qword ptr [RBP + 0x3a8],RAX
18001e0c3:  83 3D B6 7F 26 00 00                      CMP dword ptr [0x180286080],0x0
18001e0ca:  0F 84 40 0D 00 00                         JZ 0x18001ee10
18001e0d0:  49 89 5B 08                               MOV qword ptr [R11 + 0x8],RBX
18001e0d4:  49 89 73 10                               MOV qword ptr [R11 + 0x10],RSI
18001e0d8:  49 89 7B 18                               MOV qword ptr [R11 + 0x18],RDI
18001e0dc:  4D 89 6B E8                               MOV qword ptr [R11 + -0x18],R13
18001e0e0:  4D 89 73 E0                               MOV qword ptr [R11 + -0x20],R14
18001e0e4:  41 0F 29 73 C8                            MOVAPS xmmword ptr [R11 + -0x38],XMM6
18001e0e9:  41 0F 29 7B B8                            MOVAPS xmmword ptr [R11 + -0x48],XMM7
18001e0ee:  45 0F 29 43 A8                            MOVAPS xmmword ptr [R11 + -0x58],XMM8
18001e0f3:  45 0F 29 4B 98                            MOVAPS xmmword ptr [R11 + -0x68],XMM9
18001e0f8:  45 0F 29 53 88                            MOVAPS xmmword ptr [R11 + -0x78],XMM10
18001e0fd:  45 0F 29 9B 78 FF FF FF                   MOVAPS xmmword ptr [R11 + -0x88],XMM11
18001e105:  45 0F 29 A3 68 FF FF FF                   MOVAPS xmmword ptr [R11 + -0x98],XMM12
18001e10d:  45 0F 29 AB 58 FF FF FF                   MOVAPS xmmword ptr [R11 + -0xa8],XMM13
18001e115:  45 0F 29 B3 48 FF FF FF                   MOVAPS xmmword ptr [R11 + -0xb8],XMM14
18001e11d:  45 0F 29 BB 38 FF FF FF                   MOVAPS xmmword ptr [R11 + -0xc8],XMM15
18001e125:  E8 16 31 04 00                            CALL 0x180061240
18001e12a:  F3 0F 59 05 A2 55 0A 00                   MULSS XMM0,dword ptr [0x1800c36d4]
18001e132:  8B 05 70 5E 18 00                         MOV EAX,dword ptr [0x1801a3fa8]
18001e138:  F3 0F 11 44 24 50                         MOVSS dword ptr [RSP + 0x50],XMM0
18001e13e:  85 C0                                     TEST EAX,EAX
18001e140:  74 0B                                     JZ 0x18001e14d
18001e142:  F3 44 0F 10 0D 61 5E 18 00                MOVSS XMM9,dword ptr [0x1801a3fac]
18001e14b:  EB 09                                     JMP 0x18001e156
18001e14d:  F3 44 0F 10 0D 26 50 0A 00                MOVSS XMM9,dword ptr [0x1800c317c]
18001e156:  F3 44 0F 59 C8                            MULSS XMM9,XMM0
18001e15b:  85 C0                                     TEST EAX,EAX
18001e15d:  74 0A                                     JZ 0x18001e169
18001e15f:  F3 0F 10 15 49 5E 18 00                   MOVSS XMM2,dword ptr [0x1801a3fb0]
18001e167:  EB 0C                                     JMP 0x18001e175
18001e169:  F3 0F 10 15 0F 50 0A 00                   MOVSS XMM2,dword ptr [0x1800c3180]
18001e171:  F3 0F 59 D0                               MULSS XMM2,XMM0
18001e175:  E8 36 E9 FF FF                            CALL 0x18001cab0
18001e17a:  F3 44 0F 10 05 59 55 0A 00                MOVSS XMM8,dword ptr [0x1800c36dc]
18001e183:  4C 8D 0D 76 1E FE FF                      LEA R9,[0x180000000]
18001e18a:  F3 44 0F 59 05 BD 14 09 00                MULSS XMM8,dword ptr [0x1800af650]
18001e193:  33 DB                                     XOR EBX,EBX
18001e195:  44 8B 05 08 D7 14 00                      MOV R8D,dword ptr [0x18016b8a4]
18001e19c:  33 D2                                     XOR EDX,EDX
18001e19e:  44 0F 28 E0                               MOVAPS XMM12,XMM0
18001e1a2:  F3 44 0F 58 E2                            ADDSS XMM12,XMM2
18001e1a7:  F3 44 0F 59 05 64 10 09 00                MULSS XMM8,dword ptr [0x1800af214]
18001e1b0:  F3 44 0F 5F 05 07 14 09 00                MAXSS XMM8,dword ptr [0x1800af5c0]
18001e1b9:  45 85 C0                                  TEST R8D,R8D
18001e1bc:  7E 2F                                     JLE 0x18001e1ed
18001e1be:  66 90                                     NOP
18001e1c0:  81 FB A0 00 00 00                         CMP EBX,0xa0
18001e1c6:  7D 25                                     JGE 0x18001e1ed
18001e1c8:  48 63 C2                                  MOVSXD RAX,EDX
18001e1cb:  48 8D 0C 40                               LEA RCX,[RAX + RAX*0x2]
18001e1cf:  48 03 C9                                  ADD RCX,RCX
18001e1d2:  41 83 BC C9 B0 0C 18 00 02                CMP dword ptr [R9 + RCX*0x8 + 0x180cb0],0x2
18001e1db:  75 09                                     JNZ 0x18001e1e6
18001e1dd:  48 63 C3                                  MOVSXD RAX,EBX
18001e1e0:  FF C3                                     INC EBX
18001e1e2:  89 54 84 60                               MOV dword ptr [RSP + RAX*0x4 + 0x60],EDX
18001e1e6:  FF C2                                     INC EDX
18001e1e8:  41 3B D0                                  CMP EDX,R8D
18001e1eb:  7C D3                                     JL 0x18001e1c0
18001e1ed:  33 D2                                     XOR EDX,EDX
18001e1ef:  8B F3                                     MOV ESI,EBX
18001e1f1:  45 85 C0                                  TEST R8D,R8D
18001e1f4:  7E 2D                                     JLE 0x18001e223
18001e1f6:  81 FB A0 00 00 00                         CMP EBX,0xa0
18001e1fc:  7D 25                                     JGE 0x18001e223
18001e1fe:  48 63 C2                                  MOVSXD RAX,EDX
18001e201:  48 8D 0C 40                               LEA RCX,[RAX + RAX*0x2]
18001e205:  48 03 C9                                  ADD RCX,RCX
18001e208:  41 83 BC C9 B0 0C 18 00 01                CMP dword ptr [R9 + RCX*0x8 + 0x180cb0],0x1
18001e211:  75 09                                     JNZ 0x18001e21c
18001e213:  48 63 C3                                  MOVSXD RAX,EBX
18001e216:  FF C3                                     INC EBX
18001e218:  89 54 84 60                               MOV dword ptr [RSP + RAX*0x4 + 0x60],EDX
18001e21c:  FF C2                                     INC EDX
18001e21e:  41 3B D0                                  CMP EDX,R8D
18001e221:  7C D3                                     JL 0x18001e1f6
18001e223:  8D 4B 09                                  LEA ECX,[RBX + 0x9]
18001e226:  B8 67 66 66 66                            MOV EAX,0x66666667
18001e22b:  F7 E9                                     IMUL ECX
18001e22d:  8B FA                                     MOV EDI,EDX
18001e22f:  C1 FF 02                                  SAR EDI,0x2
18001e232:  8B C7                                     MOV EAX,EDI
18001e234:  C1 E8 1F                                  SHR EAX,0x1f
18001e237:  03 F8                                     ADD EDI,EAX
18001e239:  B8 01 00 00 00                            MOV EAX,0x1
18001e23e:  3B F8                                     CMP EDI,EAX
18001e240:  0F 4C F8                                  CMOVL EDI,EAX
18001e243:  8B 05 3B 7E 26 00                         MOV EAX,dword ptr [0x180286084]
18001e249:  3B C7                                     CMP EAX,EDI
18001e24b:  7C 08                                     JL 0x18001e255
18001e24d:  33 C0                                     XOR EAX,EAX
18001e24f:  89 05 2F 7E 26 00                         MOV dword ptr [0x180286084],EAX
18001e255:  8D 04 80                                  LEA EAX,[RAX + RAX*0x4]
18001e258:  44 8B F3                                  MOV R14D,EBX
18001e25b:  44 8D 2C 00                               LEA R13D,[RAX + RAX*0x1]
18001e25f:  41 8D 45 0A                               LEA EAX,[R13 + 0xa]
18001e263:  3B C3                                     CMP EAX,EBX
18001e265:  44 0F 4E F0                               CMOVLE R14D,EAX
18001e269:  44 89 74 24 54                            MOV dword ptr [RSP + 0x54],R14D
18001e26e:  E8 CD 2F 04 00                            CALL 0x180061240
18001e273:  0F 28 E8                                  MOVAPS XMM5,XMM0
18001e276:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e27a:  F3 0F 59 2D 52 54 0A 00                   MULSS XMM5,dword ptr [0x1800c36d4]
18001e282:  0F 28 CD                                  MOVAPS XMM1,XMM5
18001e285:  E8 66 2A 04 00                            CALL 0x180060cf0
18001e28a:  F3 44 0F 10 2D 55 14 09 00                MOVSS XMM13,dword ptr [0x1800af6e8]
18001e293:  45 0F 28 D9                               MOVAPS XMM11,XMM9
18001e297:  F3 44 0F 58 1D F0 13 09 00                ADDSS XMM11,dword ptr [0x1800af690]
18001e2a0:  44 0F 2F D8                               COMISS XMM11,XMM0
18001e2a4:  76 0D                                     JBE 0x18001e2b3
18001e2a6:  0F 28 C5                                  MOVAPS XMM0,XMM5
18001e2a9:  F3 41 0F 59 C5                            MULSS XMM0,XMM13
18001e2ae:  F3 41 0F 58 C1                            ADDSS XMM0,XMM9
18001e2b3:  F3 0F 10 15 1D 10 09 00                   MOVSS XMM2,dword ptr [0x1800af2d8]
18001e2bb:  F3 41 0F 5C C1                            SUBSS XMM0,XMM9
18001e2c0:  F3 44 0F 10 3D 27 10 09 00                MOVSS XMM15,dword ptr [0x1800af2f0]
18001e2c9:  45 0F 28 D0                               MOVAPS XMM10,XMM8
18001e2cd:  F3 44 0F 59 15 A6 0F 09 00                MULSS XMM10,dword ptr [0x1800af27c]
18001e2d6:  F3 44 0F 10 35 D9 11 09 00                MOVSS XMM14,dword ptr [0x1800af4b8]
18001e2df:  F3 44 0F 11 7C 24 38                      MOVSS dword ptr [RSP + 0x38],XMM15
18001e2e6:  F3 0F 11 54 24 30                         MOVSS dword ptr [RSP + 0x30],XMM2
18001e2ec:  41 0F 28 CA                               MOVAPS XMM1,XMM10
18001e2f0:  F3 0F 10 15 DC 0F 09 00                   MOVSS XMM2,dword ptr [0x1800af2d4]
18001e2f8:  F3 41 0F 58 CC                            ADDSS XMM1,XMM12
18001e2fd:  F3 0F 11 54 24 28                         MOVSS dword ptr [RSP + 0x28],XMM2
18001e303:  F3 0F 10 15 C5 0F 09 00                   MOVSS XMM2,dword ptr [0x1800af2d0]
18001e30b:  F3 41 0F 59 EE                            MULSS XMM5,XMM14
18001e310:  F3 0F 11 54 24 20                         MOVSS dword ptr [RSP + 0x20],XMM2
18001e316:  0F 28 D0                                  MOVAPS XMM2,XMM0
18001e319:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e31d:  0F 28 DD                                  MOVAPS XMM3,XMM5
18001e320:  E8 AB 8A FE FF                            CALL 0x180006dd0
18001e325:  8B 05 59 7D 26 00                         MOV EAX,dword ptr [0x180286084]
18001e32b:  4C 8D 0D BE 33 08 00                      LEA R9,[0x1800a16f0]
18001e332:  41 0F 28 F8                               MOVAPS XMM7,XMM8
18001e336:  89 7C 24 38                               MOV dword ptr [RSP + 0x38],EDI
18001e33a:  F3 0F 59 3D 7E 0F 09 00                   MULSS XMM7,dword ptr [0x1800af2c0]
18001e342:  8B CB                                     MOV ECX,EBX
18001e344:  2B CE                                     SUB ECX,ESI
18001e346:  FF C0                                     INC EAX
18001e348:  89 44 24 30                               MOV dword ptr [RSP + 0x30],EAX
18001e34c:  BA DC 00 00 00                            MOV EDX,0xdc
18001e351:  89 4C 24 28                               MOV dword ptr [RSP + 0x28],ECX
18001e355:  49 C7 C0 FF FF FF FF                      MOV R8,-0x1
18001e35c:  0F 28 F7                                  MOVAPS XMM6,XMM7
18001e35f:  89 74 24 20                               MOV dword ptr [RSP + 0x20],ESI
18001e363:  48 8D 8D 00 02 00 00                      LEA RCX,[RBP + 0x200]
18001e36a:  F3 41 0F 58 F4                            ADDSS XMM6,XMM12
18001e36f:  E8 DC B6 04 00                            CALL 0x180069a50
18001e374:  4C 8D 8D 00 02 00 00                      LEA R9,[RBP + 0x200]
18001e37b:  41 B8 FF FF FF FF                         MOV R8D,0xffffffff
18001e381:  0F 28 CE                                  MOVAPS XMM1,XMM6
18001e384:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e388:  E8 C3 48 00 00                            CALL 0x180022c50
18001e38d:  45 0F 28 E0                               MOVAPS XMM12,XMM8
18001e391:  F3 44 0F 58 E6                            ADDSS XMM12,XMM6
18001e396:  E8 A5 2E 04 00                            CALL 0x180061240
18001e39b:  0F 28 C8                                  MOVAPS XMM1,XMM0
18001e39e:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e3a2:  F3 0F 59 0D 2A 53 0A 00                   MULSS XMM1,dword ptr [0x1800c36d4]
18001e3aa:  E8 41 29 04 00                            CALL 0x180060cf0
18001e3af:  44 0F 2F D8                               COMISS XMM11,XMM0
18001e3b3:  76 0D                                     JBE 0x18001e3c2
18001e3b5:  0F 28 C1                                  MOVAPS XMM0,XMM1
18001e3b8:  F3 41 0F 59 C5                            MULSS XMM0,XMM13
18001e3bd:  F3 41 0F 58 C1                            ADDSS XMM0,XMM9
18001e3c2:  F3 0F 10 15 0E 0F 09 00                   MOVSS XMM2,dword ptr [0x1800af2d8]
18001e3ca:  F3 41 0F 5C C1                            SUBSS XMM0,XMM9
18001e3cf:  F3 44 0F 11 7C 24 38                      MOVSS dword ptr [RSP + 0x38],XMM15
18001e3d6:  F3 45 0F 58 D4                            ADDSS XMM10,XMM12
18001e3db:  F3 0F 11 54 24 30                         MOVSS dword ptr [RSP + 0x30],XMM2
18001e3e1:  F3 0F 10 15 EB 0E 09 00                   MOVSS XMM2,dword ptr [0x1800af2d4]
18001e3e9:  F3 0F 11 54 24 28                         MOVSS dword ptr [RSP + 0x28],XMM2
18001e3ef:  F3 0F 10 15 D9 0E 09 00                   MOVSS XMM2,dword ptr [0x1800af2d0]
18001e3f7:  F3 41 0F 59 CE                            MULSS XMM1,XMM14
18001e3fc:  4C 89 A4 24 68 05 00 00                   MOV qword ptr [RSP + 0x568],R12
18001e404:  F3 0F 11 54 24 20                         MOVSS dword ptr [RSP + 0x20],XMM2
18001e40a:  0F 28 D0                                  MOVAPS XMM2,XMM0
18001e40d:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e411:  4C 89 BC 24 50 05 00 00                   MOV qword ptr [RSP + 0x550],R15
18001e419:  0F 28 D9                                  MOVAPS XMM3,XMM1
18001e41c:  41 0F 28 CA                               MOVAPS XMM1,XMM10
18001e420:  E8 AB 89 FE FF                            CALL 0x180006dd0
18001e425:  F3 44 0F 10 6C 24 50                      MOVSS XMM13,dword ptr [RSP + 0x50]
18001e42c:  F3 41 0F 58 FC                            ADDSS XMM7,XMM12
18001e431:  45 0F 28 F5                               MOVAPS XMM14,XMM13
18001e435:  41 BF DC DC DC FF                         MOV R15D,0xffdcdcdc
18001e43b:  F3 44 0F 59 35 54 11 09 00                MULSS XMM14,dword ptr [0x1800af598]
18001e444:  F3 41 0F 58 FE                            ADDSS XMM7,XMM14
18001e449:  85 DB                                     TEST EBX,EBX
18001e44b:  0F 85 B7 00 00 00                         JNZ 0x18001e508
18001e451:  4C 8D 0D D8 32 08 00                      LEA R9,[0x1800a1730]
18001e458:  45 8B C7                                  MOV R8D,R15D
18001e45b:  0F 28 CF                                  MOVAPS XMM1,XMM7
18001e45e:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e462:  E8 E9 47 00 00                            CALL 0x180022c50
18001e467:  41 0F 28 F0                               MOVAPS XMM6,XMM8
18001e46b:  4C 8D 0D E6 32 08 00                      LEA R9,[0x1800a1758]
18001e472:  F3 0F 58 F7                               ADDSS XMM6,XMM7
18001e476:  45 8B C7                                  MOV R8D,R15D
18001e479:  41 0F 28 C1                               MOVAPS XMM0,XMM9
18001e47d:  0F 28 CE                                  MOVAPS XMM1,XMM6
18001e480:  E8 CB 47 00 00                            CALL 0x180022c50
18001e485:  39 1D 45 4C 0A 00                         CMP dword ptr [0x1800c30d0],EBX
18001e48b:  0F 84 EF 08 00 00                         JZ 0x18001ed80
18001e491:  39 1D F9 69 0A 00                         CMP dword ptr [0x1800c4e90],EBX
18001e497:  0F 85 E3 08 00 00                         JNZ 0x18001ed80
18001e49d:  39 1D F1 69 0A 00                         CMP dword ptr [0x1800c4e94],EBX
18001e4a3:  0F 85 D7 08 00 00                         JNZ 0x18001ed80
18001e4a9:  41 0F 28 F8                               MOVAPS XMM7,XMM8
18001e4ad:  4C 8D 0D DC 32 08 00                      LEA R9,[0x1800a1790]
18001e4b4:  F3 0F 58 FE                               ADDSS XMM7,XMM6
18001e4b8:  41 BC 60 D0 FF FF                         MOV R12D,0xffffd060
18001e4be:  45 8B C4                                  MOV R8D,R12D
; 222 instruction(s)
