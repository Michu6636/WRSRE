; Range: 0x180057230 to 0x180057980  (containing fn: FUN_180057230@0x180057230)
180057230:  4C 8B DC                                  MOV R11,RSP
180057233:  53                                        PUSH RBX
180057234:  48 81 EC 20 02 00 00                      SUB RSP,0x220
18005723b:  48 8B 05 3E CF 06 00                      MOV RAX,qword ptr [0x1800c4180]
180057242:  48 33 C4                                  XOR RAX,RSP
180057245:  48 89 84 24 00 02 00 00                   MOV qword ptr [RSP + 0x200],RAX
18005724d:  83 3D 2C 9B 22 00 00                      CMP dword ptr [0x180280d80],0x0
180057254:  0F 84 B7 06 00 00                         JZ 0x180057911
18005725a:  83 3D 23 9B 22 00 00                      CMP dword ptr [0x180280d84],0x0
180057261:  0F 84 AA 06 00 00                         JZ 0x180057911
180057267:  48 8B 05 02 DB 06 00                      MOV RAX,qword ptr [0x1800c4d70]
18005726e:  BA 08 00 00 00                            MOV EDX,0x8
180057273:  49 89 7B 18                               MOV qword ptr [R11 + 0x18],RDI
180057277:  48 8B 3D 02 DB 06 00                      MOV RDI,qword ptr [0x1800c4d80]
18005727e:  49 89 6B 08                               MOV qword ptr [R11 + 0x8],RBP
180057282:  49 89 73 10                               MOV qword ptr [R11 + 0x10],RSI
180057286:  48 8D 8F 60 15 9E 00                      LEA RCX,[RDI + 0x9e1560]
18005728d:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180057290:  33 DB                                     XOR EBX,EBX
180057292:  85 C0                                     TEST EAX,EAX
180057294:  0F 84 D7 01 00 00                         JZ 0x180057471
18005729a:  48 8B 87 60 15 9E 00                      MOV RAX,qword ptr [RDI + 0x9e1560]
1800572a1:  48 3B 05 D0 76 48 00                      CMP RAX,qword ptr [0x1804de978]
1800572a8:  0F 84 C3 01 00 00                         JZ 0x180057471
1800572ae:  48 89 05 C3 76 48 00                      MOV qword ptr [0x1804de978],RAX
1800572b5:  48 85 C0                                  TEST RAX,RAX
1800572b8:  0F 84 D2 01 00 00                         JZ 0x180057490
1800572be:  48 8B 35 BB DA 06 00                      MOV RSI,qword ptr [0x1800c4d80]
1800572c5:  48 81 C6 60 15 9E 00                      ADD RSI,0x9e1560
1800572cc:  39 1D A6 75 48 00                         CMP dword ptr [0x1804de878],EBX
1800572d2:  75 20                                     JNZ 0x1800572f4
1800572d4:  48 8B 05 95 DA 06 00                      MOV RAX,qword ptr [0x1800c4d70]
1800572db:  BA 08 00 00 00                            MOV EDX,0x8
1800572e0:  48 8B CE                                  MOV RCX,RSI
1800572e3:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
1800572e6:  85 C0                                     TEST EAX,EAX
1800572e8:  74 69                                     JZ 0x180057353
1800572ea:  C7 05 84 75 48 00 01 00 00 00             MOV dword ptr [0x1804de878],0x1
1800572f4:  48 85 F6                                  TEST RSI,RSI
1800572f7:  74 5A                                     JZ 0x180057353
1800572f9:  48 8B 3E                                  MOV RDI,qword ptr [RSI]
1800572fc:  48 85 FF                                  TEST RDI,RDI
1800572ff:  74 52                                     JZ 0x180057353
180057301:  48 3B 3D 78 75 48 00                      CMP RDI,qword ptr [0x1804de880]
180057308:  75 0E                                     JNZ 0x180057318
18005730a:  8B 05 C8 BE 06 00                         MOV EAX,dword ptr [0x1800c31d8]
180057310:  39 05 72 75 48 00                         CMP dword ptr [0x1804de888],EAX
180057316:  74 33                                     JZ 0x18005734b
180057318:  48 8B 05 51 DA 06 00                      MOV RAX,qword ptr [0x1800c4d70]
18005731f:  48 8D 4F 01                               LEA RCX,[RDI + 0x1]
180057323:  BA 17 00 00 00                            MOV EDX,0x17
180057328:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
18005732b:  85 C0                                     TEST EAX,EAX
18005732d:  75 09                                     JNZ 0x180057338
18005732f:  48 8D 2D 8A 78 04 00                      LEA RBP,[0x18009ebc0]
180057336:  EB 22                                     JMP 0x18005735a
180057338:  8B 05 9A BE 06 00                         MOV EAX,dword ptr [0x1800c31d8]
18005733e:  89 05 44 75 48 00                         MOV dword ptr [0x1804de888],EAX
180057344:  48 89 3D 35 75 48 00                      MOV qword ptr [0x1804de880],RDI
18005734b:  48 8B 2E                                  MOV RBP,qword ptr [RSI]
18005734e:  48 FF C5                                  INC RBP
180057351:  EB 07                                     JMP 0x18005735a
180057353:  48 8D 2D 5A 78 04 00                      LEA RBP,[0x18009ebb4]
18005735a:  8B 05 08 C9 06 00                         MOV EAX,dword ptr [0x1800c3c68]
180057360:  85 C0                                     TEST EAX,EAX
180057362:  0F 84 FF 00 00 00                         JZ 0x180057467
180057368:  4C 89 A4 24 48 02 00 00                   MOV qword ptr [RSP + 0x248],R12
180057370:  4C 8D 0D 01 78 04 00                      LEA R9,[0x18009eb78]
180057377:  44 8B 25 EE C8 06 00                      MOV R12D,dword ptr [0x1800c3c6c]
18005737e:  4C 8D 44 24 40                            LEA R8,[RSP + 0x40]
180057383:  4C 89 BC 24 10 02 00 00                   MOV qword ptr [RSP + 0x210],R15
18005738b:  48 8D 4C 24 50                            LEA RCX,[RSP + 0x50]
180057390:  44 8B F8                                  MOV R15D,EAX
180057393:  89 5C 24 40                               MOV dword ptr [RSP + 0x40],EBX
180057397:  4C 03 3D E2 D9 06 00                      ADD R15,qword ptr [0x1800c4d80]
18005739e:  41 8B F4                                  MOV ESI,R12D
1800573a1:  BA A4 01 00 00                            MOV EDX,0x1a4
1800573a6:  48 89 6C 24 20                            MOV qword ptr [RSP + 0x20],RBP
1800573ab:  F7 DE                                     NEG ESI
1800573ad:  E8 1E A9 FA FF                            CALL 0x180001cd0
1800573b2:  41 3B F4                                  CMP ESI,R12D
1800573b5:  0F 8F 8B 00 00 00                         JG 0x180057446
1800573bb:  4C 89 B4 24 18 02 00 00                   MOV qword ptr [RSP + 0x218],R14
1800573c3:  8D 3C B5 00 00 00 00                      LEA EDI,[RSI*0x4]
1800573ca:  66 0F 1F 44 00 00                         NOP word ptr [RAX + RAX*0x1]
1800573d0:  48 8B 05 99 D9 06 00                      MOV RAX,qword ptr [0x1800c4d70]
1800573d7:  49 8D 8F 10 4F 9D 00                      LEA RCX,[R15 + 0x9d4f10]
1800573de:  4C 63 F7                                  MOVSXD R14,EDI
1800573e1:  BA 04 00 00 00                            MOV EDX,0x4
1800573e6:  49 03 CE                                  ADD RCX,R14
1800573e9:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
1800573ec:  4C 8D 44 24 40                            LEA R8,[RSP + 0x40]
1800573f1:  BA A4 01 00 00                            MOV EDX,0x1a4
1800573f6:  48 8D 4C 24 50                            LEA RCX,[RSP + 0x50]
1800573fb:  85 C0                                     TEST EAX,EAX
1800573fd:  75 12                                     JNZ 0x180057411
1800573ff:  4C 8D 0D 92 77 04 00                      LEA R9,[0x18009eb98]
180057406:  89 7C 24 20                               MOV dword ptr [RSP + 0x20],EDI
18005740a:  E8 C1 A8 FA FF                            CALL 0x180001cd0
18005740f:  EB 23                                     JMP 0x180057434
180057411:  F3 43 0F 10 84 37 10 4F 9D 00             MOVSS XMM0,dword ptr [R15 + R14*0x1 + 0x9d4f10]
18005741b:  4C 8D 0D 86 77 04 00                      LEA R9,[0x18009eba8]
180057422:  0F 5A C0                                  CVTPS2PD XMM0,XMM0
180057425:  F2 0F 11 44 24 28                         MOVSD qword ptr [RSP + 0x28],XMM0
18005742b:  89 7C 24 20                               MOV dword ptr [RSP + 0x20],EDI
18005742f:  E8 9C A8 FA FF                            CALL 0x180001cd0
180057434:  FF C6                                     INC ESI
180057436:  83 C7 04                                  ADD EDI,0x4
180057439:  41 3B F4                                  CMP ESI,R12D
18005743c:  7E 92                                     JLE 0x1800573d0
18005743e:  4C 8B B4 24 18 02 00 00                   MOV R14,qword ptr [RSP + 0x218]
180057446:  48 8D 54 24 50                            LEA RDX,[RSP + 0x50]
18005744b:  48 8D 0D CA 6A 03 00                      LEA RCX,[0x18008df1c]
180057452:  E8 99 CA FF FF                            CALL 0x180053ef0
180057457:  4C 8B BC 24 10 02 00 00                   MOV R15,qword ptr [RSP + 0x210]
18005745f:  4C 8B A4 24 48 02 00 00                   MOV R12,qword ptr [RSP + 0x248]
180057467:  48 8B CD                                  MOV RCX,RBP
18005746a:  E8 C1 04 00 00                            CALL 0x180057930
18005746f:  EB 1F                                     JMP 0x180057490
180057471:  48 8B 05 F8 D8 06 00                      MOV RAX,qword ptr [0x1800c4d70]
180057478:  48 8D 8F 60 15 9E 00                      LEA RCX,[RDI + 0x9e1560]
18005747f:  BA 08 00 00 00                            MOV EDX,0x8
180057484:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180057487:  85 C0                                     TEST EAX,EAX
180057489:  74 05                                     JZ 0x180057490
18005748b:  E8 E0 EE FF FF                            CALL 0x180056370
180057490:  E8 2B 1E 00 00                            CALL 0x1800592c0
180057495:  39 1D 09 A7 3B 00                         CMP dword ptr [0x180411ba4],EBX
18005749b:  0F 84 10 01 00 00                         JZ 0x1800575b1
1800574a1:  39 1D 01 A7 3B 00                         CMP dword ptr [0x180411ba8],EBX
1800574a7:  0F 8E 04 01 00 00                         JLE 0x1800575b1
1800574ad:  39 1D B5 C7 06 00                         CMP dword ptr [0x1800c3c68],EBX
1800574b3:  0F 84 F8 00 00 00                         JZ 0x1800575b1
1800574b9:  E8 92 08 00 00                            CALL 0x180057d50
1800574be:  85 C0                                     TEST EAX,EAX
1800574c0:  0F 84 EB 00 00 00                         JZ 0x1800575b1
1800574c6:  8B 05 A8 74 48 00                         MOV EAX,dword ptr [0x1804de974]
1800574cc:  FF C0                                     INC EAX
1800574ce:  89 05 A0 74 48 00                         MOV dword ptr [0x1804de974],EAX
1800574d4:  99                                        CDQ
1800574d5:  F7 3D 95 C7 06 00                         IDIV dword ptr [0x1800c3c70]
1800574db:  85 D2                                     TEST EDX,EDX
1800574dd:  0F 85 CE 00 00 00                         JNZ 0x1800575b1
1800574e3:  8B 3D 7F C7 06 00                         MOV EDI,dword ptr [0x1800c3c68]
1800574e9:  BA 18 00 00 00                            MOV EDX,0x18
1800574ee:  48 8B 35 8B D8 06 00                      MOV RSI,qword ptr [0x1800c4d80]
1800574f5:  48 8B 05 74 D8 06 00                      MOV RAX,qword ptr [0x1800c4d70]
1800574fc:  48 8D 2C 37                               LEA RBP,[RDI + RSI*0x1]
180057500:  48 8D 8D 08 4F 9D 00                      LEA RCX,[RBP + 0x9d4f08]
180057507:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
18005750a:  85 C0                                     TEST EAX,EAX
18005750c:  75 0B                                     JNZ 0x180057519
18005750e:  89 1D 94 A6 3B 00                         MOV dword ptr [0x180411ba8],EBX
180057514:  E9 98 00 00 00                            JMP 0x1800575b1
180057519:  8B 15 89 A6 3B 00                         MOV EDX,dword ptr [0x180411ba8]
18005751f:  48 8D 0D 9A 74 04 00                      LEA RCX,[0x18009e9c0]
180057526:  F3 0F 10 84 37 18 4F 9D 00                MOVSS XMM0,dword ptr [RDI + RSI*0x1 + 0x9d4f18]
18005752f:  FF CA                                     DEC EDX
180057531:  F3 0F 10 8C 37 14 4F 9D 00                MOVSS XMM1,dword ptr [RDI + RSI*0x1 + 0x9d4f14]
18005753a:  F3 0F 10 9C 37 0C 4F 9D 00                MOVSS XMM3,dword ptr [RDI + RSI*0x1 + 0x9d4f0c]
180057543:  F3 0F 10 95 08 4F 9D 00                   MOVSS XMM2,dword ptr [RBP + 0x9d4f08]
18005754b:  F3 0F 10 A4 37 10 4F 9D 00                MOVSS XMM4,dword ptr [RDI + RSI*0x1 + 0x9d4f10]
180057554:  0F 5A C0                                  CVTPS2PD XMM0,XMM0
180057557:  89 15 4B A6 3B 00                         MOV dword ptr [0x180411ba8],EDX
18005755d:  0F 5A C9                                  CVTPS2PD XMM1,XMM1
180057560:  F2 0F 11 44 24 30                         MOVSD qword ptr [RSP + 0x30],XMM0
180057566:  0F 5A DB                                  CVTPS2PD XMM3,XMM3
180057569:  0F 5A D2                                  CVTPS2PD XMM2,XMM2
18005756c:  0F 5A E4                                  CVTPS2PD XMM4,XMM4
18005756f:  F2 0F 11 4C 24 28                         MOVSD qword ptr [RSP + 0x28],XMM1
180057575:  66 49 0F 7E D9                            MOVQ R9,XMM3
18005757a:  66 49 0F 7E D0                            MOVQ R8,XMM2
18005757f:  F2 0F 11 64 24 20                         MOVSD qword ptr [RSP + 0x20],XMM4
180057585:  E8 66 C9 FF FF                            CALL 0x180053ef0
18005758a:  39 1D 18 A6 3B 00                         CMP dword ptr [0x180411ba8],EBX
180057590:  75 1F                                     JNZ 0x1800575b1
180057592:  48 8D 0D 67 74 04 00                      LEA RCX,[0x18009ea00]
180057599:  E8 52 C9 FF FF                            CALL 0x180053ef0
18005759e:  48 8B 0D 3B BC 06 00                      MOV RCX,qword ptr [0x1800c31e0]
1800575a5:  48 83 F9 FF                               CMP RCX,-0x1
1800575a9:  74 06                                     JZ 0x1800575b1
1800575ab:  FF 15 B7 5A 03 00                         CALL qword ptr [0x18008d068]
1800575b1:  39 1D A1 BB 06 00                         CMP dword ptr [0x1800c3158],EBX
1800575b7:  48 8B B4 24 38 02 00 00                   MOV RSI,qword ptr [RSP + 0x238]
1800575bf:  48 8B AC 24 30 02 00 00                   MOV RBP,qword ptr [RSP + 0x230]
1800575c7:  75 30                                     JNZ 0x1800575f9
1800575c9:  39 1D B1 EA 22 00                         CMP dword ptr [0x180286080],EBX
1800575cf:  75 28                                     JNZ 0x1800575f9
1800575d1:  39 1D 41 64 17 00                         CMP dword ptr [0x1801cda18],EBX
1800575d7:  75 20                                     JNZ 0x1800575f9
1800575d9:  39 1D 35 64 17 00                         CMP dword ptr [0x1801cda14],EBX
1800575df:  75 18                                     JNZ 0x1800575f9
1800575e1:  39 1D CD D8 12 00                         CMP dword ptr [0x180184eb4],EBX
1800575e7:  75 10                                     JNZ 0x1800575f9
1800575e9:  39 1D 2D 64 17 00                         CMP dword ptr [0x1801cda1c],EBX
1800575ef:  75 08                                     JNZ 0x1800575f9
1800575f1:  39 1D 59 D6 06 00                         CMP dword ptr [0x1800c4c50],EBX
1800575f7:  74 25                                     JZ 0x18005761e
1800575f9:  44 8B C3                                  MOV R8D,EBX
1800575fc:  0F 1F 40 00                               NOP dword ptr [RAX]
180057600:  41 8B C8                                  MOV ECX,R8D
180057603:  E8 18 7B FD FF                            CALL 0x18002f120
180057608:  85 C0                                     TEST EAX,EAX
18005760a:  75 0B                                     JNZ 0x180057617
18005760c:  41 FF C0                                  INC R8D
18005760f:  41 83 F8 08                               CMP R8D,0x8
180057613:  7C EB                                     JL 0x180057600
180057615:  EB 07                                     JMP 0x18005761e
180057617:  44 89 05 AE A5 3B 00                      MOV dword ptr [0x180411bcc],R8D
18005761e:  E8 1D 08 00 00                            CALL 0x180057e40
180057623:  E8 E8 0C 00 00                            CALL 0x180058310
180057628:  3B 05 52 73 48 00                         CMP EAX,dword ptr [0x1804de980]
18005762e:  74 67                                     JZ 0x180057697
180057630:  89 05 4A 73 48 00                         MOV dword ptr [0x1804de980],EAX
180057636:  85 C0                                     TEST EAX,EAX
180057638:  74 5D                                     JZ 0x180057697
18005763a:  39 1D 18 BB 06 00                         CMP dword ptr [0x1800c3158],EBX
180057640:  75 3B                                     JNZ 0x18005767d
180057642:  39 1D 38 EA 22 00                         CMP dword ptr [0x180286080],EBX
180057648:  75 33                                     JNZ 0x18005767d
18005764a:  39 1D C8 63 17 00                         CMP dword ptr [0x1801cda18],EBX
180057650:  75 2B                                     JNZ 0x18005767d
180057652:  39 1D BC 63 17 00                         CMP dword ptr [0x1801cda14],EBX
180057658:  75 23                                     JNZ 0x18005767d
18005765a:  39 1D 54 D8 12 00                         CMP dword ptr [0x180184eb4],EBX
180057660:  75 1B                                     JNZ 0x18005767d
180057662:  39 1D B4 63 17 00                         CMP dword ptr [0x1801cda1c],EBX
180057668:  75 13                                     JNZ 0x18005767d
18005766a:  39 1D E0 D5 06 00                         CMP dword ptr [0x1800c4c50],EBX
180057670:  75 0B                                     JNZ 0x18005767d
180057672:  8B 0D 54 A5 3B 00                         MOV ECX,dword ptr [0x180411bcc]
180057678:  E8 63 7B FD FF                            CALL 0x18002f1e0
18005767d:  4C 8D 05 A4 C5 06 00                      LEA R8,[0x1800c3c28]
180057684:  48 8D 15 B5 C5 06 00                      LEA RDX,[0x1800c3c40]
18005768b:  48 8D 0D 0E 7B 04 00                      LEA RCX,[0x18009f1a0]
180057692:  E8 59 C8 FF FF                            CALL 0x180053ef0
180057697:  E8 B4 06 00 00                            CALL 0x180057d50
18005769c:  39 1D 4A BC 06 00                         CMP dword ptr [0x1800c32ec],EBX
1800576a2:  8B F8                                     MOV EDI,EAX
1800576a4:  74 2A                                     JZ 0x1800576d0
1800576a6:  85 C0                                     TEST EAX,EAX
1800576a8:  75 26                                     JNZ 0x1800576d0
1800576aa:  39 1D 18 A5 3B 00                         CMP dword ptr [0x180411bc8],EBX
1800576b0:  75 48                                     JNZ 0x1800576fa
1800576b2:  E8 C9 A5 FA FF                            CALL 0x180001c80
1800576b7:  85 C0                                     TEST EAX,EAX
1800576b9:  74 3F                                     JZ 0x1800576fa
1800576bb:  E8 50 0C 00 00                            CALL 0x180058310
1800576c0:  85 C0                                     TEST EAX,EAX
1800576c2:  75 36                                     JNZ 0x1800576fa
1800576c4:  E8 E7 1C 00 00                            CALL 0x1800593b0
1800576c9:  E8 82 06 00 00                            CALL 0x180057d50
1800576ce:  8B F8                                     MOV EDI,EAX
1800576d0:  85 FF                                     TEST EDI,EDI
1800576d2:  74 26                                     JZ 0x1800576fa
1800576d4:  39 1D EE A4 3B 00                         CMP dword ptr [0x180411bc8],EBX
1800576da:  74 1E                                     JZ 0x1800576fa
1800576dc:  39 1D 92 C5 06 00                         CMP dword ptr [0x1800c3c74],EBX
1800576e2:  75 16                                     JNZ 0x1800576fa
1800576e4:  E8 97 A5 FA FF                            CALL 0x180001c80
1800576e9:  85 C0                                     TEST EAX,EAX
1800576eb:  75 0D                                     JNZ 0x1800576fa
1800576ed:  E8 CE FA FF FF                            CALL 0x1800571c0
1800576f2:  89 1D B0 C8 14 00                         MOV dword ptr [0x1801a3fa8],EBX
1800576f8:  8B FB                                     MOV EDI,EBX
1800576fa:  3B 3D C8 A4 3B 00                         CMP EDI,dword ptr [0x180411bc8]
180057700:  74 76                                     JZ 0x180057778
180057702:  89 3D C0 A4 3B 00                         MOV dword ptr [0x180411bc8],EDI
180057708:  85 FF                                     TEST EDI,EDI
18005770a:  0F 85 89 00 00 00                         JNZ 0x180057799
180057710:  39 1D 5E C5 06 00                         CMP dword ptr [0x1800c3c74],EBX
180057716:  75 60                                     JNZ 0x180057778
180057718:  E8 63 A5 FA FF                            CALL 0x180001c80
18005771d:  85 C0                                     TEST EAX,EAX
18005771f:  74 57                                     JZ 0x180057778
180057721:  E8 EA 0B 00 00                            CALL 0x180058310
180057726:  85 C0                                     TEST EAX,EAX
180057728:  75 4E                                     JNZ 0x180057778
18005772a:  39 1D 50 E9 22 00                         CMP dword ptr [0x180286080],EBX
180057730:  74 0C                                     JZ 0x18005773e
180057732:  89 1D 48 E9 22 00                         MOV dword ptr [0x180286080],EBX
180057738:  89 1D 46 E9 22 00                         MOV dword ptr [0x180286084],EBX
18005773e:  48 8D 0D A3 7A 04 00                      LEA RCX,[0x18009f1e8]
180057745:  89 1D 05 D5 06 00                         MOV dword ptr [0x1800c4c50],EBX
18005774b:  89 1D CB 62 17 00                         MOV dword ptr [0x1801cda1c],EBX
180057751:  89 1D 5D D7 12 00                         MOV dword ptr [0x180184eb4],EBX
180057757:  89 1D B7 62 17 00                         MOV dword ptr [0x1801cda14],EBX
18005775d:  89 1D B5 62 17 00                         MOV dword ptr [0x1801cda18],EBX
180057763:  89 1D EF B9 06 00                         MOV dword ptr [0x1800c3158],EBX
180057769:  C7 05 FD B9 06 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
180057773:  E8 78 C7 FF FF                            CALL 0x180053ef0
180057778:  48 8B BC 24 40 02 00 00                   MOV RDI,qword ptr [RSP + 0x240]
180057780:  48 8B 8C 24 00 02 00 00                   MOV RCX,qword ptr [RSP + 0x200]
180057788:  48 33 CC                                  XOR RCX,RSP
18005778b:  E8 00 27 01 00                            CALL 0x180069e90
180057790:  48 81 C4 20 02 00 00                      ADD RSP,0x220
180057797:  5B                                        POP RBX
180057798:  C3                                        RET
180057799:  E8 E2 A4 FA FF                            CALL 0x180001c80
18005779e:  39 1D D0 C4 06 00                         CMP dword ptr [0x1800c3c74],EBX
1800577a4:  75 76                                     JNZ 0x18005781c
1800577a6:  85 C0                                     TEST EAX,EAX
1800577a8:  75 0B                                     JNZ 0x1800577b5
1800577aa:  8B 0D 1C A4 3B 00                         MOV ECX,dword ptr [0x180411bcc]
1800577b0:  E8 2B 7A FD FF                            CALL 0x18002f1e0
1800577b5:  8B 0D 15 C8 14 00                         MOV ECX,dword ptr [0x1801a3fd0]
1800577bb:  8B C1                                     MOV EAX,ECX
1800577bd:  2B 05 C1 71 48 00                         SUB EAX,dword ptr [0x1804de984]
1800577c3:  89 0D BB 71 48 00                         MOV dword ptr [0x1804de984],ECX
1800577c9:  83 F8 1E                                  CMP EAX,0x1e
1800577cc:  0F 83 B6 00 00 00                         JNC 0x180057888
1800577d2:  8B 15 B0 71 48 00                         MOV EDX,dword ptr [0x1804de988]
1800577d8:  FF C2                                     INC EDX
1800577da:  89 15 A8 71 48 00                         MOV dword ptr [0x1804de988],EDX
1800577e0:  83 FA 08                                  CMP EDX,0x8
1800577e3:  0F 82 A5 00 00 00                         JC 0x18005788e
1800577e9:  39 1D 85 C4 06 00                         CMP dword ptr [0x1800c3c74],EBX
1800577ef:  0F 84 99 00 00 00                         JZ 0x18005788e
1800577f5:  48 8D 0D 24 7A 04 00                      LEA RCX,[0x18009f220]
1800577fc:  89 1D 72 C4 06 00                         MOV dword ptr [0x1800c3c74],EBX
180057802:  E8 E9 C6 FF FF                            CALL 0x180053ef0
180057807:  48 8B 0D D2 B9 06 00                      MOV RCX,qword ptr [0x1800c31e0]
18005780e:  48 83 F9 FF                               CMP RCX,-0x1
180057812:  74 7A                                     JZ 0x18005788e
180057814:  FF 15 4E 58 03 00                         CALL qword ptr [0x18008d068]
18005781a:  EB 72                                     JMP 0x18005788e
18005781c:  85 C0                                     TEST EAX,EAX
18005781e:  74 8A                                     JZ 0x1800577aa
180057820:  44 8B C3                                  MOV R8D,EBX
180057823:  41 8B C8                                  MOV ECX,R8D
180057826:  E8 F5 78 FD FF                            CALL 0x18002f120
18005782b:  85 C0                                     TEST EAX,EAX
18005782d:  75 0B                                     JNZ 0x18005783a
18005782f:  41 FF C0                                  INC R8D
180057832:  41 83 F8 08                               CMP R8D,0x8
180057836:  7C EB                                     JL 0x180057823
180057838:  EB 07                                     JMP 0x180057841
18005783a:  44 89 05 8B A3 3B 00                      MOV dword ptr [0x180411bcc],R8D
180057841:  39 1D 39 E8 22 00                         CMP dword ptr [0x180286080],EBX
180057847:  74 0C                                     JZ 0x180057855
180057849:  89 1D 31 E8 22 00                         MOV dword ptr [0x180286080],EBX
18005784f:  89 1D 2F E8 22 00                         MOV dword ptr [0x180286084],EBX
180057855:  89 1D F5 D3 06 00                         MOV dword ptr [0x1800c4c50],EBX
18005785b:  89 1D BB 61 17 00                         MOV dword ptr [0x1801cda1c],EBX
180057861:  89 1D 4D D6 12 00                         MOV dword ptr [0x180184eb4],EBX
180057867:  89 1D A7 61 17 00                         MOV dword ptr [0x1801cda14],EBX
18005786d:  89 1D A5 61 17 00                         MOV dword ptr [0x1801cda18],EBX
180057873:  89 1D DF B8 06 00                         MOV dword ptr [0x1800c3158],EBX
180057879:  C7 05 ED B8 06 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
180057883:  E9 2D FF FF FF                            JMP 0x1800577b5
180057888:  89 1D FA 70 48 00                         MOV dword ptr [0x1804de988],EBX
18005788e:  E8 7D F8 FF FF                            CALL 0x180057110
180057893:  39 1D DB C3 06 00                         CMP dword ptr [0x1800c3c74],EBX
180057899:  74 06                                     JZ 0x1800578a1
18005789b:  89 1D 27 A3 3B 00                         MOV dword ptr [0x180411bc8],EBX
1800578a1:  39 1D B1 B8 06 00                         CMP dword ptr [0x1800c3158],EBX
1800578a7:  75 30                                     JNZ 0x1800578d9
1800578a9:  39 1D D1 E7 22 00                         CMP dword ptr [0x180286080],EBX
1800578af:  75 28                                     JNZ 0x1800578d9
1800578b1:  39 1D 61 61 17 00                         CMP dword ptr [0x1801cda18],EBX
1800578b7:  75 20                                     JNZ 0x1800578d9
1800578b9:  39 1D 55 61 17 00                         CMP dword ptr [0x1801cda14],EBX
1800578bf:  75 18                                     JNZ 0x1800578d9
1800578c1:  39 1D ED D5 12 00                         CMP dword ptr [0x180184eb4],EBX
1800578c7:  75 10                                     JNZ 0x1800578d9
1800578c9:  39 1D 4D 61 17 00                         CMP dword ptr [0x1801cda1c],EBX
1800578cf:  75 08                                     JNZ 0x1800578d9
1800578d1:  39 1D 79 D3 06 00                         CMP dword ptr [0x1800c4c50],EBX
1800578d7:  74 05                                     JZ 0x1800578de
1800578d9:  BB 01 00 00 00                            MOV EBX,0x1
1800578de:  44 8B 0D E7 A2 3B 00                      MOV R9D,dword ptr [0x180411bcc]
1800578e5:  48 8D 05 D8 79 04 00                      LEA RAX,[0x18009f2c4]
1800578ec:  85 DB                                     TEST EBX,EBX
1800578ee:  4C 8D 05 D7 79 04 00                      LEA R8,[0x18009f2cc]
1800578f5:  48 8D 15 14 C3 06 00                      LEA RDX,[0x1800c3c10]
1800578fc:  4C 0F 45 C0                               CMOVNZ R8,RAX
180057900:  48 8D 0D D1 79 04 00                      LEA RCX,[0x18009f2d8]
180057907:  E8 E4 C5 FF FF                            CALL 0x180053ef0
18005790c:  E9 67 FE FF FF                            JMP 0x180057778
180057911:  33 DB                                     XOR EBX,EBX
180057913:  89 1D AF A2 3B 00                         MOV dword ptr [0x180411bc8],EBX
180057919:  89 1D 89 C6 14 00                         MOV dword ptr [0x1801a3fa8],EBX
18005791f:  E9 5C FE FF FF                            JMP 0x180057780
180057930:  48 8B C4                                  MOV RAX,RSP
180057933:  48 89 48 08                               MOV qword ptr [RAX + 0x8],RCX
180057937:  41 55                                     PUSH R13
180057939:  48 83 EC 60                               SUB RSP,0x60
18005793d:  83 3D 10 02 23 00 00                      CMP dword ptr [0x180287b54],0x0
180057944:  4C 8B E9                                  MOV R13,RCX
180057947:  0F 84 A0 03 00 00                         JZ 0x180057ced
18005794d:  4C 89 70 D0                               MOV qword ptr [RAX + -0x30],R14
180057951:  41 BE 00 00 08 00                         MOV R14D,0x80000
180057957:  48 89 58 18                               MOV qword ptr [RAX + 0x18],RBX
18005795b:  48 89 68 F0                               MOV qword ptr [RAX + -0x10],RBP
18005795f:  48 89 70 E8                               MOV qword ptr [RAX + -0x18],RSI
180057963:  48 89 78 E0                               MOV qword ptr [RAX + -0x20],RDI
180057967:  4C 89 60 D8                               MOV qword ptr [RAX + -0x28],R12
18005796b:  4C 89 78 C8                               MOV qword ptr [RAX + -0x38],R15
18005796f:  8B 05 EF C2 06 00                         MOV EAX,dword ptr [0x1800c3c64]
180057975:  44 89 74 24 78                            MOV dword ptr [RSP + 0x78],R14D
18005797a:  41 3B C6                                  CMP EAX,R14D
18005797d:  76 18                                     JBE 0x180057997
18005797f:  44 8B 25 DA C2 06 00                      MOV R12D,dword ptr [0x1800c3c60]
; 403 instruction(s)
