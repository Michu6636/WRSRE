; Range: 0x1402E7200 to 0x1402E8200  (containing fn: FUN_1402e6bc0@0x1402E6BC0)
1402e7203:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e7207:  73 5E                                     JNC 0x1402e7267
1402e7209:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e720c:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7210:  48 3B C1                                  CMP RAX,RCX
1402e7213:  77 52                                     JA 0x1402e7267
1402e7215:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7219:  48 2B C8                                  SUB RCX,RAX
1402e721c:  49 8B C6                                  MOV RAX,R14
1402e721f:  48 F7 E9                                  IMUL RCX
1402e7222:  48 8B F2                                  MOV RSI,RDX
1402e7225:  48 C1 FE 05                               SAR RSI,0x5
1402e7229:  48 8B C6                                  MOV RAX,RSI
1402e722c:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7230:  48 03 F0                                  ADD RSI,RAX
1402e7233:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7237:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e723b:  75 08                                     JNZ 0x1402e7245
1402e723d:  48 8B CB                                  MOV RCX,RBX
1402e7240:  E8 FB 50 16 00                            CALL 0x14044c340
1402e7245:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7249:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e724e:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e7252:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e7255:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e725a:  48 85 C9                                  TEST RCX,RCX
1402e725d:  74 06                                     JZ 0x1402e7265
1402e725f:  E8 CC 22 17 00                            CALL 0x140459530
1402e7264:  90                                        NOP
1402e7265:  EB 2F                                     JMP 0x1402e7296
1402e7267:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e726b:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e726f:  75 08                                     JNZ 0x1402e7279
1402e7271:  48 8B CB                                  MOV RCX,RBX
1402e7274:  E8 C7 50 16 00                            CALL 0x14044c340
1402e7279:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e727d:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e7282:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7287:  48 85 C9                                  TEST RCX,RCX
1402e728a:  74 0A                                     JZ 0x1402e7296
1402e728c:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e7290:  E8 9B 22 17 00                            CALL 0x140459530
1402e7295:  90                                        NOP
1402e7296:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e729b:  C7 45 80 2E 04 00 00                      MOV dword ptr [RBP + -0x80],0x42e
1402e72a2:  8B 05 C0 08 5C 00                         MOV EAX,dword ptr [0x1408a7b68]
1402e72a8:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e72ac:  0F B7 05 B9 08 5C 00                      MOVZX EAX,word ptr [0x1408a7b6c]
1402e72b3:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e72b8:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e72bc:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e72bf:  49 8B C6                                  MOV RAX,R14
1402e72c2:  48 F7 E9                                  IMUL RCX
1402e72c5:  48 C1 FA 05                               SAR RDX,0x5
1402e72c9:  48 8B C2                                  MOV RAX,RDX
1402e72cc:  48 C1 E8 3F                               SHR RAX,0x3f
1402e72d0:  48 03 D0                                  ADD RDX,RAX
1402e72d3:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e72d7:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e72da:  48 83 C0 E0                               ADD RAX,-0x20
1402e72de:  48 03 C8                                  ADD RCX,RAX
1402e72e1:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e72e6:  E8 05 20 16 00                            CALL 0x1404492f0
1402e72eb:  C7 45 80 2F 04 00 00                      MOV dword ptr [RBP + -0x80],0x42f
1402e72f2:  8B 05 D4 08 5C 00                         MOV EAX,dword ptr [0x1408a7bcc]
1402e72f8:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e72fc:  0F B7 05 CD 08 5C 00                      MOVZX EAX,word ptr [0x1408a7bd0]
1402e7303:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7308:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e730c:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e730f:  49 8B C6                                  MOV RAX,R14
1402e7312:  48 F7 E9                                  IMUL RCX
1402e7315:  48 C1 FA 05                               SAR RDX,0x5
1402e7319:  48 8B C2                                  MOV RAX,RDX
1402e731c:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7320:  48 03 D0                                  ADD RDX,RAX
1402e7323:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7327:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e732a:  48 83 C0 E0                               ADD RAX,-0x20
1402e732e:  48 03 C8                                  ADD RCX,RAX
1402e7331:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7336:  E8 B5 1F 16 00                            CALL 0x1404492f0
1402e733b:  C7 45 80 30 04 00 00                      MOV dword ptr [RBP + -0x80],0x430
1402e7342:  0F 10 05 6F 08 5C 00                      MOVUPS XMM0,xmmword ptr [0x1408a7bb8]
1402e7349:  0F 29 44 24 40                            MOVAPS xmmword ptr [RSP + 0x40],XMM0
1402e734e:  0F B7 05 73 08 5C 00                      MOVZX EAX,word ptr [0x1408a7bc8]
1402e7355:  66 89 44 24 50                            MOV word ptr [RSP + 0x50],AX
1402e735a:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e735e:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7361:  49 8B C6                                  MOV RAX,R14
1402e7364:  48 F7 E9                                  IMUL RCX
1402e7367:  48 C1 FA 05                               SAR RDX,0x5
1402e736b:  48 8B C2                                  MOV RAX,RDX
1402e736e:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7372:  48 03 D0                                  ADD RDX,RAX
1402e7375:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7379:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e737c:  48 83 C0 E0                               ADD RAX,-0x20
1402e7380:  48 03 C8                                  ADD RCX,RAX
1402e7383:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7388:  E8 63 1F 16 00                            CALL 0x1404492f0
1402e738d:  C7 45 80 31 04 00 00                      MOV dword ptr [RBP + -0x80],0x431
1402e7394:  F2 0F 10 05 0C 08 5C 00                   MOVSD XMM0,qword ptr [0x1408a7ba8]
1402e739c:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e73a2:  0F B7 05 07 08 5C 00                      MOVZX EAX,word ptr [0x1408a7bb0]
1402e73a9:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e73ae:  0F B6 05 FD 07 5C 00                      MOVZX EAX,byte ptr [0x1408a7bb2]
1402e73b5:  88 44 24 4A                               MOV byte ptr [RSP + 0x4a],AL
1402e73b9:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e73bd:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e73c0:  49 8B C6                                  MOV RAX,R14
1402e73c3:  48 F7 E9                                  IMUL RCX
1402e73c6:  48 C1 FA 05                               SAR RDX,0x5
1402e73ca:  48 8B C2                                  MOV RAX,RDX
1402e73cd:  48 C1 E8 3F                               SHR RAX,0x3f
1402e73d1:  48 03 D0                                  ADD RDX,RAX
1402e73d4:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e73d8:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e73db:  48 83 C0 E0                               ADD RAX,-0x20
1402e73df:  48 03 C8                                  ADD RCX,RAX
1402e73e2:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e73e7:  E8 04 1F 16 00                            CALL 0x1404492f0
1402e73ec:  C7 45 80 32 04 00 00                      MOV dword ptr [RBP + -0x80],0x432
1402e73f3:  F2 0F 10 05 9D 07 5C 00                   MOVSD XMM0,qword ptr [0x1408a7b98]
1402e73fb:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7401:  0F B6 05 98 07 5C 00                      MOVZX EAX,byte ptr [0x1408a7ba0]
1402e7408:  88 44 24 48                               MOV byte ptr [RSP + 0x48],AL
1402e740c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7410:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7413:  49 8B C6                                  MOV RAX,R14
1402e7416:  48 F7 E9                                  IMUL RCX
1402e7419:  48 C1 FA 05                               SAR RDX,0x5
1402e741d:  48 8B C2                                  MOV RAX,RDX
1402e7420:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7424:  48 03 D0                                  ADD RDX,RAX
1402e7427:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e742b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e742e:  48 83 C0 E0                               ADD RAX,-0x20
1402e7432:  48 03 C8                                  ADD RCX,RAX
1402e7435:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e743a:  E8 B1 1E 16 00                            CALL 0x1404492f0
1402e743f:  C7 45 80 33 04 00 00                      MOV dword ptr [RBP + -0x80],0x433
1402e7446:  8B 05 94 08 5C 00                         MOV EAX,dword ptr [0x1408a7ce0]
1402e744c:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7450:  0F B7 05 8D 08 5C 00                      MOVZX EAX,word ptr [0x1408a7ce4]
1402e7457:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e745c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7460:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7463:  49 8B C6                                  MOV RAX,R14
1402e7466:  48 F7 E9                                  IMUL RCX
1402e7469:  48 C1 FA 05                               SAR RDX,0x5
1402e746d:  48 8B C2                                  MOV RAX,RDX
1402e7470:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7474:  48 03 D0                                  ADD RDX,RAX
1402e7477:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e747b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e747e:  48 83 C0 E0                               ADD RAX,-0x20
1402e7482:  48 03 C8                                  ADD RCX,RAX
1402e7485:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e748a:  E8 61 1E 16 00                            CALL 0x1404492f0
1402e748f:  C7 45 80 34 04 00 00                      MOV dword ptr [RBP + -0x80],0x434
1402e7496:  F2 0F 10 05 32 08 5C 00                   MOVSD XMM0,qword ptr [0x1408a7cd0]
1402e749e:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e74a4:  8B 05 2E 08 5C 00                         MOV EAX,dword ptr [0x1408a7cd8]
1402e74aa:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e74ae:  0F B6 05 27 08 5C 00                      MOVZX EAX,byte ptr [0x1408a7cdc]
1402e74b5:  88 44 24 4C                               MOV byte ptr [RSP + 0x4c],AL
1402e74b9:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e74bd:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e74c0:  49 8B C6                                  MOV RAX,R14
1402e74c3:  48 F7 E9                                  IMUL RCX
1402e74c6:  48 C1 FA 05                               SAR RDX,0x5
1402e74ca:  48 8B C2                                  MOV RAX,RDX
1402e74cd:  48 C1 E8 3F                               SHR RAX,0x3f
1402e74d1:  48 03 D0                                  ADD RDX,RAX
1402e74d4:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e74d8:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e74db:  48 83 C0 E0                               ADD RAX,-0x20
1402e74df:  48 03 C8                                  ADD RCX,RAX
1402e74e2:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e74e7:  E8 04 1E 16 00                            CALL 0x1404492f0
1402e74ec:  C7 45 80 82 04 00 00                      MOV dword ptr [RBP + -0x80],0x482
1402e74f3:  F2 0F 10 05 C5 07 5C 00                   MOVSD XMM0,qword ptr [0x1408a7cc0]
1402e74fb:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7501:  8B 05 C1 07 5C 00                         MOV EAX,dword ptr [0x1408a7cc8]
1402e7507:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e750b:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e750f:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7512:  49 8B C6                                  MOV RAX,R14
1402e7515:  48 F7 E9                                  IMUL RCX
1402e7518:  48 C1 FA 05                               SAR RDX,0x5
1402e751c:  48 8B C2                                  MOV RAX,RDX
1402e751f:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7523:  48 03 D0                                  ADD RDX,RAX
1402e7526:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e752a:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e752d:  48 83 C0 E0                               ADD RAX,-0x20
1402e7531:  48 03 C8                                  ADD RCX,RAX
1402e7534:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7539:  E8 B2 1D 16 00                            CALL 0x1404492f0
1402e753e:  C7 45 80 F0 87 00 00                      MOV dword ptr [RBP + -0x80],0x87f0
1402e7545:  8B 05 69 07 5C 00                         MOV EAX,dword ptr [0x1408a7cb4]
1402e754b:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e754f:  0F B7 05 62 07 5C 00                      MOVZX EAX,word ptr [0x1408a7cb8]
1402e7556:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e755b:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e755f:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7562:  49 8B C6                                  MOV RAX,R14
1402e7565:  48 F7 E9                                  IMUL RCX
1402e7568:  48 C1 FA 05                               SAR RDX,0x5
1402e756c:  48 8B C2                                  MOV RAX,RDX
1402e756f:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7573:  48 03 D0                                  ADD RDX,RAX
1402e7576:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e757a:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e757d:  48 83 C0 E0                               ADD RAX,-0x20
1402e7581:  48 03 C8                                  ADD RCX,RAX
1402e7584:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7589:  E8 62 1D 16 00                            CALL 0x1404492f0
1402e758e:  C7 45 80 F9 0A 00 00                      MOV dword ptr [RBP + -0x80],0xaf9
1402e7595:  F2 0F 10 05 63 07 5C 00                   MOVSD XMM0,qword ptr [0x1408a7d00]
1402e759d:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e75a3:  0F B6 05 5E 07 5C 00                      MOVZX EAX,byte ptr [0x1408a7d08]
1402e75aa:  88 44 24 48                               MOV byte ptr [RSP + 0x48],AL
1402e75ae:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e75b2:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e75b5:  49 8B C6                                  MOV RAX,R14
1402e75b8:  48 F7 E9                                  IMUL RCX
1402e75bb:  48 C1 FA 05                               SAR RDX,0x5
1402e75bf:  48 8B C2                                  MOV RAX,RDX
1402e75c2:  48 C1 E8 3F                               SHR RAX,0x3f
1402e75c6:  48 03 D0                                  ADD RDX,RAX
1402e75c9:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e75cd:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e75d0:  48 83 C0 E0                               ADD RAX,-0x20
1402e75d4:  48 03 C8                                  ADD RCX,RAX
1402e75d7:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e75dc:  E8 0F 1D 16 00                            CALL 0x1404492f0
1402e75e1:  C7 45 80 90 E2 00 00                      MOV dword ptr [RBP + -0x80],0xe290
1402e75e8:  48 B8 68 65 61 74 69 6E 67 00             MOV RAX,0x676e6974616568
1402e75f2:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e75f7:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e75fb:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e75fe:  49 8B C6                                  MOV RAX,R14
1402e7601:  48 F7 E9                                  IMUL RCX
1402e7604:  48 C1 FA 05                               SAR RDX,0x5
1402e7608:  48 8B C2                                  MOV RAX,RDX
1402e760b:  48 C1 E8 3F                               SHR RAX,0x3f
1402e760f:  48 03 D0                                  ADD RDX,RAX
1402e7612:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7616:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7619:  48 83 C0 E0                               ADD RAX,-0x20
1402e761d:  48 03 C8                                  ADD RCX,RAX
1402e7620:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7625:  E8 C6 1C 16 00                            CALL 0x1404492f0
1402e762a:  C7 45 80 A8 06 00 00                      MOV dword ptr [RBP + -0x80],0x6a8
1402e7631:  F2 0F 10 05 AF 06 5C 00                   MOVSD XMM0,qword ptr [0x1408a7ce8]
1402e7639:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e763f:  0F B7 05 AA 06 5C 00                      MOVZX EAX,word ptr [0x1408a7cf0]
1402e7646:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e764b:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e764f:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7652:  49 8B C6                                  MOV RAX,R14
1402e7655:  48 F7 E9                                  IMUL RCX
1402e7658:  48 C1 FA 05                               SAR RDX,0x5
1402e765c:  48 8B C2                                  MOV RAX,RDX
1402e765f:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7663:  48 03 D0                                  ADD RDX,RAX
1402e7666:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e766a:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e766d:  48 83 C0 E0                               ADD RAX,-0x20
1402e7671:  48 03 C8                                  ADD RCX,RAX
1402e7674:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7679:  E8 72 1C 16 00                            CALL 0x1404492f0
1402e767e:  C7 45 80 E8 FD 00 00                      MOV dword ptr [RBP + -0x80],0xfde8
1402e7685:  8B 05 91 0C 59 00                         MOV EAX,dword ptr [0x14087831c]
1402e768b:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e768f:  0F B7 05 8A 0C 59 00                      MOVZX EAX,word ptr [0x140878320]
1402e7696:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e769b:  0F B6 05 80 0C 59 00                      MOVZX EAX,byte ptr [0x140878322]
1402e76a2:  88 44 24 46                               MOV byte ptr [RSP + 0x46],AL
1402e76a6:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e76aa:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e76ad:  49 8B C6                                  MOV RAX,R14
1402e76b0:  48 F7 E9                                  IMUL RCX
1402e76b3:  48 C1 FA 05                               SAR RDX,0x5
1402e76b7:  48 8B C2                                  MOV RAX,RDX
1402e76ba:  48 C1 E8 3F                               SHR RAX,0x3f
1402e76be:  48 03 D0                                  ADD RDX,RAX
1402e76c1:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e76c5:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e76c8:  48 83 C0 E0                               ADD RAX,-0x20
1402e76cc:  48 03 C8                                  ADD RCX,RAX
1402e76cf:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e76d4:  E8 17 1C 16 00                            CALL 0x1404492f0
1402e76d9:  F2 0F 10 05 A7 05 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c88]
1402e76e1:  F2 0F 11 45 91                            MOVSD qword ptr [RBP + -0x6f],XMM0
1402e76e6:  0F B6 05 A3 05 5C 00                      MOVZX EAX,byte ptr [0x1408a7c90]
1402e76ed:  88 45 99                                  MOV byte ptr [RBP + -0x67],AL
1402e76f0:  C7 45 D4 78 05 00 00                      MOV dword ptr [RBP + -0x2c],0x578
1402e76f7:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e76fb:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e76ff:  73 5E                                     JNC 0x1402e775f
1402e7701:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7704:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7708:  48 3B C1                                  CMP RAX,RCX
1402e770b:  77 52                                     JA 0x1402e775f
1402e770d:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7711:  48 2B C8                                  SUB RCX,RAX
1402e7714:  49 8B C6                                  MOV RAX,R14
1402e7717:  48 F7 E9                                  IMUL RCX
1402e771a:  48 8B F2                                  MOV RSI,RDX
1402e771d:  48 C1 FE 05                               SAR RSI,0x5
1402e7721:  48 8B C6                                  MOV RAX,RSI
1402e7724:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7728:  48 03 F0                                  ADD RSI,RAX
1402e772b:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e772f:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e7733:  75 08                                     JNZ 0x1402e773d
1402e7735:  48 8B CB                                  MOV RCX,RBX
1402e7738:  E8 03 4C 16 00                            CALL 0x14044c340
1402e773d:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7741:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e7746:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e774a:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e774d:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7752:  48 85 C9                                  TEST RCX,RCX
1402e7755:  74 06                                     JZ 0x1402e775d
1402e7757:  E8 D4 1D 17 00                            CALL 0x140459530
1402e775c:  90                                        NOP
1402e775d:  EB 2F                                     JMP 0x1402e778e
1402e775f:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7763:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e7767:  75 08                                     JNZ 0x1402e7771
1402e7769:  48 8B CB                                  MOV RCX,RBX
1402e776c:  E8 CF 4B 16 00                            CALL 0x14044c340
1402e7771:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7775:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e777a:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e777f:  48 85 C9                                  TEST RCX,RCX
1402e7782:  74 0A                                     JZ 0x1402e778e
1402e7784:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e7788:  E8 A3 1D 17 00                            CALL 0x140459530
1402e778d:  90                                        NOP
1402e778e:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e7793:  C7 45 80 2E 18 00 00                      MOV dword ptr [RBP + -0x80],0x182e
1402e779a:  F2 0F 10 05 D6 04 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c78]
1402e77a2:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e77a8:  8B 05 D2 04 5C 00                         MOV EAX,dword ptr [0x1408a7c80]
1402e77ae:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e77b2:  0F B6 05 CB 04 5C 00                      MOVZX EAX,byte ptr [0x1408a7c84]
1402e77b9:  88 44 24 4C                               MOV byte ptr [RSP + 0x4c],AL
1402e77bd:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e77c1:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e77c4:  49 8B C6                                  MOV RAX,R14
1402e77c7:  48 F7 E9                                  IMUL RCX
1402e77ca:  48 C1 FA 05                               SAR RDX,0x5
1402e77ce:  48 8B C2                                  MOV RAX,RDX
1402e77d1:  48 C1 E8 3F                               SHR RAX,0x3f
1402e77d5:  48 03 D0                                  ADD RDX,RAX
1402e77d8:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e77dc:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e77df:  48 83 C0 E0                               ADD RAX,-0x20
1402e77e3:  48 03 C8                                  ADD RCX,RAX
1402e77e6:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e77eb:  E8 00 1B 16 00                            CALL 0x1404492f0
1402e77f0:  C7 45 80 70 17 00 00                      MOV dword ptr [RBP + -0x80],0x1770
1402e77f7:  F2 0F 10 05 69 04 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c68]
1402e77ff:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7805:  0F B7 05 64 04 5C 00                      MOVZX EAX,word ptr [0x1408a7c70]
1402e780c:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e7811:  0F B6 05 5A 04 5C 00                      MOVZX EAX,byte ptr [0x1408a7c72]
1402e7818:  88 44 24 4A                               MOV byte ptr [RSP + 0x4a],AL
1402e781c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7820:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7823:  49 8B C6                                  MOV RAX,R14
1402e7826:  48 F7 E9                                  IMUL RCX
1402e7829:  48 C1 FA 05                               SAR RDX,0x5
1402e782d:  48 8B C2                                  MOV RAX,RDX
1402e7830:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7834:  48 03 D0                                  ADD RDX,RAX
1402e7837:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e783b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e783e:  48 83 C0 E0                               ADD RAX,-0x20
1402e7842:  48 03 C8                                  ADD RCX,RAX
1402e7845:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e784a:  E8 A1 1A 16 00                            CALL 0x1404492f0
1402e784f:  C7 45 80 73 17 00 00                      MOV dword ptr [RBP + -0x80],0x1773
1402e7856:  F2 0F 10 05 FA 03 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c58]
1402e785e:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7864:  0F B7 05 F5 03 5C 00                      MOVZX EAX,word ptr [0x1408a7c60]
1402e786b:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e7870:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7874:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7877:  49 8B C6                                  MOV RAX,R14
1402e787a:  48 F7 E9                                  IMUL RCX
1402e787d:  48 C1 FA 05                               SAR RDX,0x5
1402e7881:  48 8B C2                                  MOV RAX,RDX
1402e7884:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7888:  48 03 D0                                  ADD RDX,RAX
1402e788b:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e788f:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7892:  48 83 C0 E0                               ADD RAX,-0x20
1402e7896:  48 03 C8                                  ADD RCX,RAX
1402e7899:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e789e:  E8 4D 1A 16 00                            CALL 0x1404492f0
1402e78a3:  C7 45 80 75 17 00 00                      MOV dword ptr [RBP + -0x80],0x1775
1402e78aa:  F2 0F 10 05 F6 03 5C 00                   MOVSD XMM0,qword ptr [0x1408a7ca8]
1402e78b2:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e78b8:  0F B7 05 F1 03 5C 00                      MOVZX EAX,word ptr [0x1408a7cb0]
1402e78bf:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e78c4:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e78c8:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e78cb:  49 8B C6                                  MOV RAX,R14
1402e78ce:  48 F7 E9                                  IMUL RCX
1402e78d1:  48 C1 FA 05                               SAR RDX,0x5
1402e78d5:  48 8B C2                                  MOV RAX,RDX
1402e78d8:  48 C1 E8 3F                               SHR RAX,0x3f
1402e78dc:  48 03 D0                                  ADD RDX,RAX
1402e78df:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e78e3:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e78e6:  48 83 C0 E0                               ADD RAX,-0x20
1402e78ea:  48 03 C8                                  ADD RCX,RAX
1402e78ed:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e78f2:  E8 F9 19 16 00                            CALL 0x1404492f0
1402e78f7:  C7 45 80 B2 92 00 00                      MOV dword ptr [RBP + -0x80],0x92b2
1402e78fe:  49 BC 74 6F 75 72 69 73 6D 00             MOV R12,0x6d736972756f74
1402e7908:  4C 89 64 24 40                            MOV qword ptr [RSP + 0x40],R12
1402e790d:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7911:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7914:  49 8B C6                                  MOV RAX,R14
1402e7917:  48 F7 E9                                  IMUL RCX
1402e791a:  48 C1 FA 05                               SAR RDX,0x5
1402e791e:  48 8B C2                                  MOV RAX,RDX
1402e7921:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7925:  48 03 D0                                  ADD RDX,RAX
1402e7928:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e792c:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e792f:  48 83 C0 E0                               ADD RAX,-0x20
1402e7933:  48 03 C8                                  ADD RCX,RAX
1402e7936:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e793b:  E8 B0 19 16 00                            CALL 0x1404492f0
1402e7940:  C7 45 80 D4 17 00 00                      MOV dword ptr [RBP + -0x80],0x17d4
1402e7947:  8B 05 47 03 5C 00                         MOV EAX,dword ptr [0x1408a7c94]
1402e794d:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7951:  0F B6 05 40 03 5C 00                      MOVZX EAX,byte ptr [0x1408a7c98]
1402e7958:  88 44 24 44                               MOV byte ptr [RSP + 0x44],AL
1402e795c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7960:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7963:  49 8B C6                                  MOV RAX,R14
1402e7966:  48 F7 E9                                  IMUL RCX
1402e7969:  48 C1 FA 05                               SAR RDX,0x5
1402e796d:  48 8B C2                                  MOV RAX,RDX
1402e7970:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7974:  48 03 D0                                  ADD RDX,RAX
1402e7977:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e797b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e797e:  48 83 C0 E0                               ADD RAX,-0x20
1402e7982:  48 03 C8                                  ADD RCX,RAX
1402e7985:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e798a:  E8 61 19 16 00                            CALL 0x1404492f0
1402e798f:  C7 45 80 A2 17 00 00                      MOV dword ptr [RBP + -0x80],0x17a2
1402e7996:  C7 44 24 40 6F 69 6C 00                   MOV dword ptr [RSP + 0x40],0x6c696f
1402e799e:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e79a2:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e79a5:  49 8B C6                                  MOV RAX,R14
1402e79a8:  48 F7 E9                                  IMUL RCX
1402e79ab:  48 C1 FA 05                               SAR RDX,0x5
1402e79af:  48 8B C2                                  MOV RAX,RDX
1402e79b2:  48 C1 E8 3F                               SHR RAX,0x3f
1402e79b6:  48 03 D0                                  ADD RDX,RAX
1402e79b9:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e79bd:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e79c0:  48 83 C0 E0                               ADD RAX,-0x20
1402e79c4:  48 03 C8                                  ADD RCX,RAX
1402e79c7:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e79cc:  E8 1F 19 16 00                            CALL 0x1404492f0
1402e79d1:  C7 45 80 BC 66 00 00                      MOV dword ptr [RBP + -0x80],0x66bc
1402e79d8:  F2 0F 10 05 90 03 5C 00                   MOVSD XMM0,qword ptr [0x1408a7d70]
1402e79e0:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e79e6:  0F B7 05 8B 03 5C 00                      MOVZX EAX,word ptr [0x1408a7d78]
1402e79ed:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e79f2:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e79f6:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e79f9:  49 8B C6                                  MOV RAX,R14
1402e79fc:  48 F7 E9                                  IMUL RCX
1402e79ff:  48 C1 FA 05                               SAR RDX,0x5
1402e7a03:  48 8B C2                                  MOV RAX,RDX
1402e7a06:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7a0a:  48 03 D0                                  ADD RDX,RAX
1402e7a0d:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7a11:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7a14:  48 83 C0 E0                               ADD RAX,-0x20
1402e7a18:  48 03 C8                                  ADD RCX,RAX
1402e7a1b:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7a20:  E8 CB 18 16 00                            CALL 0x1404492f0
1402e7a25:  C7 45 80 CE 18 00 00                      MOV dword ptr [RBP + -0x80],0x18ce
1402e7a2c:  48 B8 76 61 72 69 6F 75 73 00             MOV RAX,0x73756f69726176
1402e7a36:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e7a3b:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7a3f:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7a42:  49 8B C6                                  MOV RAX,R14
1402e7a45:  48 F7 E9                                  IMUL RCX
1402e7a48:  48 C1 FA 05                               SAR RDX,0x5
1402e7a4c:  48 8B C2                                  MOV RAX,RDX
1402e7a4f:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7a53:  48 03 D0                                  ADD RDX,RAX
1402e7a56:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7a5a:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7a5d:  48 83 C0 E0                               ADD RAX,-0x20
1402e7a61:  48 03 C8                                  ADD RCX,RAX
1402e7a64:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7a69:  E8 82 18 16 00                            CALL 0x1404492f0
1402e7a6e:  C7 45 80 2F 18 00 00                      MOV dword ptr [RBP + -0x80],0x182f
1402e7a75:  8B 05 31 09 59 00                         MOV EAX,dword ptr [0x1408783ac]
1402e7a7b:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7a7f:  0F B7 05 2A 09 59 00                      MOVZX EAX,word ptr [0x1408783b0]
1402e7a86:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7a8b:  0F B6 05 20 09 59 00                      MOVZX EAX,byte ptr [0x1408783b2]
1402e7a92:  88 44 24 46                               MOV byte ptr [RSP + 0x46],AL
1402e7a96:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7a9a:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7a9d:  49 8B C6                                  MOV RAX,R14
1402e7aa0:  48 F7 E9                                  IMUL RCX
1402e7aa3:  48 C1 FA 05                               SAR RDX,0x5
1402e7aa7:  48 8B C2                                  MOV RAX,RDX
1402e7aaa:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7aae:  48 03 D0                                  ADD RDX,RAX
1402e7ab1:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7ab5:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7ab8:  48 83 C0 E0                               ADD RAX,-0x20
1402e7abc:  48 03 C8                                  ADD RCX,RAX
1402e7abf:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7ac4:  E8 27 18 16 00                            CALL 0x1404492f0
1402e7ac9:  C7 45 80 38 18 00 00                      MOV dword ptr [RBP + -0x80],0x1838
1402e7ad0:  F2 0F 10 05 80 02 5C 00                   MOVSD XMM0,qword ptr [0x1408a7d58]
1402e7ad8:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7ade:  0F B7 05 7B 02 5C 00                      MOVZX EAX,word ptr [0x1408a7d60]
1402e7ae5:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e7aea:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7aee:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7af1:  49 8B C6                                  MOV RAX,R14
1402e7af4:  48 F7 E9                                  IMUL RCX
1402e7af7:  48 C1 FA 05                               SAR RDX,0x5
1402e7afb:  48 8B C2                                  MOV RAX,RDX
1402e7afe:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7b02:  48 03 D0                                  ADD RDX,RAX
1402e7b05:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7b09:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7b0c:  48 83 C0 E0                               ADD RAX,-0x20
1402e7b10:  48 03 C8                                  ADD RCX,RAX
1402e7b13:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7b18:  E8 D3 17 16 00                            CALL 0x1404492f0
1402e7b1d:  C7 45 80 F7 07 00 00                      MOV dword ptr [RBP + -0x80],0x7f7
1402e7b24:  0F 10 05 6D 02 5C 00                      MOVUPS XMM0,xmmword ptr [0x1408a7d98]
1402e7b2b:  0F 29 44 24 40                            MOVAPS xmmword ptr [RSP + 0x40],XMM0
1402e7b30:  0F B7 05 71 02 5C 00                      MOVZX EAX,word ptr [0x1408a7da8]
1402e7b37:  66 89 44 24 50                            MOV word ptr [RSP + 0x50],AX
1402e7b3c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7b40:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7b43:  49 8B C6                                  MOV RAX,R14
1402e7b46:  48 F7 E9                                  IMUL RCX
1402e7b49:  48 C1 FA 05                               SAR RDX,0x5
1402e7b4d:  48 8B C2                                  MOV RAX,RDX
1402e7b50:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7b54:  48 03 D0                                  ADD RDX,RAX
1402e7b57:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7b5b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7b5e:  48 83 C0 E0                               ADD RAX,-0x20
1402e7b62:  48 03 C8                                  ADD RCX,RAX
1402e7b65:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7b6a:  E8 81 17 16 00                            CALL 0x1404492f0
1402e7b6f:  C7 45 80 9F 18 00 00                      MOV dword ptr [RBP + -0x80],0x189f
1402e7b76:  48 B8 65 6C 65 74 72 69 63 00             MOV RAX,0x63697274656c65
1402e7b80:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e7b85:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7b89:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7b8c:  49 8B C6                                  MOV RAX,R14
1402e7b8f:  48 F7 E9                                  IMUL RCX
1402e7b92:  48 C1 FA 05                               SAR RDX,0x5
1402e7b96:  48 8B C2                                  MOV RAX,RDX
1402e7b99:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7b9d:  48 03 D0                                  ADD RDX,RAX
1402e7ba0:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7ba4:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7ba7:  48 83 C0 E0                               ADD RAX,-0x20
1402e7bab:  48 03 C8                                  ADD RCX,RAX
1402e7bae:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7bb3:  E8 38 17 16 00                            CALL 0x1404492f0
1402e7bb8:  C7 45 80 A8 04 00 00                      MOV dword ptr [RBP + -0x80],0x4a8
1402e7bbf:  F2 0F 10 05 B9 01 5C 00                   MOVSD XMM0,qword ptr [0x1408a7d80]
1402e7bc7:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7bcd:  0F B6 05 B4 01 5C 00                      MOVZX EAX,byte ptr [0x1408a7d88]
1402e7bd4:  88 44 24 48                               MOV byte ptr [RSP + 0x48],AL
1402e7bd8:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7bdc:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7bdf:  49 8B C6                                  MOV RAX,R14
1402e7be2:  48 F7 E9                                  IMUL RCX
1402e7be5:  48 C1 FA 05                               SAR RDX,0x5
1402e7be9:  48 8B C2                                  MOV RAX,RDX
1402e7bec:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7bf0:  48 03 D0                                  ADD RDX,RAX
1402e7bf3:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7bf7:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7bfa:  48 83 C0 E0                               ADD RAX,-0x20
1402e7bfe:  48 03 C8                                  ADD RCX,RAX
1402e7c01:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7c06:  E8 E5 16 16 00                            CALL 0x1404492f0
1402e7c0b:  C7 45 80 A2 04 00 00                      MOV dword ptr [RBP + -0x80],0x4a2
1402e7c12:  8B 05 B4 07 59 00                         MOV EAX,dword ptr [0x1408783cc]
1402e7c18:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7c1c:  0F B7 05 AD 07 59 00                      MOVZX EAX,word ptr [0x1408783d0]
1402e7c23:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7c28:  0F B6 05 A3 07 59 00                      MOVZX EAX,byte ptr [0x1408783d2]
1402e7c2f:  88 44 24 46                               MOV byte ptr [RSP + 0x46],AL
1402e7c33:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7c37:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7c3a:  49 8B C6                                  MOV RAX,R14
1402e7c3d:  48 F7 E9                                  IMUL RCX
1402e7c40:  48 C1 FA 05                               SAR RDX,0x5
1402e7c44:  48 8B C2                                  MOV RAX,RDX
1402e7c47:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7c4b:  48 03 D0                                  ADD RDX,RAX
1402e7c4e:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7c52:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7c55:  48 83 C0 E0                               ADD RAX,-0x20
1402e7c59:  48 03 C8                                  ADD RCX,RAX
1402e7c5c:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7c61:  E8 8A 16 16 00                            CALL 0x1404492f0
1402e7c66:  C7 45 80 A4 01 00 00                      MOV dword ptr [RBP + -0x80],0x1a4
1402e7c6d:  8B 05 B5 00 5C 00                         MOV EAX,dword ptr [0x1408a7d28]
1402e7c73:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7c77:  0F B7 05 AE 00 5C 00                      MOVZX EAX,word ptr [0x1408a7d2c]
1402e7c7e:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7c83:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7c87:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7c8a:  49 8B C6                                  MOV RAX,R14
1402e7c8d:  48 F7 E9                                  IMUL RCX
1402e7c90:  48 C1 FA 05                               SAR RDX,0x5
1402e7c94:  48 8B C2                                  MOV RAX,RDX
1402e7c97:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7c9b:  48 03 D0                                  ADD RDX,RAX
1402e7c9e:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7ca2:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7ca5:  48 83 C0 E0                               ADD RAX,-0x20
1402e7ca9:  48 03 C8                                  ADD RCX,RAX
1402e7cac:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7cb1:  E8 3A 16 16 00                            CALL 0x1404492f0
1402e7cb6:  C7 45 80 11 19 00 00                      MOV dword ptr [RBP + -0x80],0x1911
1402e7cbd:  C7 44 24 40 63 69 76 00                   MOV dword ptr [RSP + 0x40],0x766963
1402e7cc5:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7cc9:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7ccc:  49 8B C6                                  MOV RAX,R14
1402e7ccf:  48 F7 E9                                  IMUL RCX
1402e7cd2:  48 C1 FA 05                               SAR RDX,0x5
1402e7cd6:  48 8B C2                                  MOV RAX,RDX
1402e7cd9:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7cdd:  48 03 D0                                  ADD RDX,RAX
1402e7ce0:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7ce4:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7ce7:  48 83 C0 E0                               ADD RAX,-0x20
1402e7ceb:  48 03 C8                                  ADD RCX,RAX
1402e7cee:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7cf3:  E8 F8 15 16 00                            CALL 0x1404492f0
1402e7cf8:  8B 05 22 00 5C 00                         MOV EAX,dword ptr [0x1408a7d20]
1402e7cfe:  89 45 91                                  MOV dword ptr [RBP + -0x6f],EAX
1402e7d01:  0F B7 05 1C 00 5C 00                      MOVZX EAX,word ptr [0x1408a7d24]
1402e7d08:  66 89 45 95                               MOV word ptr [RBP + -0x6b],AX
1402e7d0c:  C7 45 D4 DC 05 00 00                      MOV dword ptr [RBP + -0x2c],0x5dc
1402e7d13:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e7d17:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e7d1b:  73 5E                                     JNC 0x1402e7d7b
1402e7d1d:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7d20:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7d24:  48 3B C1                                  CMP RAX,RCX
1402e7d27:  77 52                                     JA 0x1402e7d7b
1402e7d29:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7d2d:  48 2B C8                                  SUB RCX,RAX
1402e7d30:  49 8B C6                                  MOV RAX,R14
1402e7d33:  48 F7 E9                                  IMUL RCX
1402e7d36:  48 8B F2                                  MOV RSI,RDX
1402e7d39:  48 C1 FE 05                               SAR RSI,0x5
1402e7d3d:  48 8B C6                                  MOV RAX,RSI
1402e7d40:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7d44:  48 03 F0                                  ADD RSI,RAX
1402e7d47:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7d4b:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e7d4f:  75 08                                     JNZ 0x1402e7d59
1402e7d51:  48 8B CB                                  MOV RCX,RBX
1402e7d54:  E8 E7 45 16 00                            CALL 0x14044c340
1402e7d59:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7d5d:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e7d62:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e7d66:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e7d69:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7d6e:  48 85 C9                                  TEST RCX,RCX
1402e7d71:  74 06                                     JZ 0x1402e7d79
1402e7d73:  E8 B8 17 17 00                            CALL 0x140459530
1402e7d78:  90                                        NOP
1402e7d79:  EB 2F                                     JMP 0x1402e7daa
1402e7d7b:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7d7f:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e7d83:  75 08                                     JNZ 0x1402e7d8d
1402e7d85:  48 8B CB                                  MOV RCX,RBX
1402e7d88:  E8 B3 45 16 00                            CALL 0x14044c340
1402e7d8d:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7d91:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e7d96:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7d9b:  48 85 C9                                  TEST RCX,RCX
1402e7d9e:  74 0A                                     JZ 0x1402e7daa
1402e7da0:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e7da4:  E8 87 17 17 00                            CALL 0x140459530
1402e7da9:  90                                        NOP
1402e7daa:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e7daf:  C7 45 80 DD 05 00 00                      MOV dword ptr [RBP + -0x80],0x5dd
1402e7db6:  F2 0F 10 05 52 FF 5B 00                   MOVSD XMM0,qword ptr [0x1408a7d10]
1402e7dbe:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7dc4:  8B 05 4E FF 5B 00                         MOV EAX,dword ptr [0x1408a7d18]
1402e7dca:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e7dce:  0F B6 05 47 FF 5B 00                      MOVZX EAX,byte ptr [0x1408a7d1c]
1402e7dd5:  88 44 24 4C                               MOV byte ptr [RSP + 0x4c],AL
1402e7dd9:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7ddd:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7de0:  49 8B C6                                  MOV RAX,R14
1402e7de3:  48 F7 E9                                  IMUL RCX
1402e7de6:  48 C1 FA 05                               SAR RDX,0x5
1402e7dea:  48 8B C2                                  MOV RAX,RDX
1402e7ded:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7df1:  48 03 D0                                  ADD RDX,RAX
1402e7df4:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7df8:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7dfb:  48 83 C0 E0                               ADD RAX,-0x20
1402e7dff:  48 03 C8                                  ADD RCX,RAX
1402e7e02:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7e07:  E8 E4 14 16 00                            CALL 0x1404492f0
1402e7e0c:  C7 45 80 E7 05 00 00                      MOV dword ptr [RBP + -0x80],0x5e7
1402e7e13:  0F 10 05 26 FF 5B 00                      MOVUPS XMM0,xmmword ptr [0x1408a7d40]
1402e7e1a:  0F 29 44 24 40                            MOVAPS xmmword ptr [RSP + 0x40],XMM0
1402e7e1f:  8B 05 2B FF 5B 00                         MOV EAX,dword ptr [0x1408a7d50]
1402e7e25:  89 44 24 50                               MOV dword ptr [RSP + 0x50],EAX
1402e7e29:  0F B7 05 24 FF 5B 00                      MOVZX EAX,word ptr [0x1408a7d54]
1402e7e30:  66 89 44 24 54                            MOV word ptr [RSP + 0x54],AX
1402e7e35:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7e39:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7e3c:  49 8B C6                                  MOV RAX,R14
1402e7e3f:  48 F7 E9                                  IMUL RCX
1402e7e42:  48 C1 FA 05                               SAR RDX,0x5
1402e7e46:  48 8B C2                                  MOV RAX,RDX
1402e7e49:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7e4d:  48 03 D0                                  ADD RDX,RAX
1402e7e50:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7e54:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7e57:  48 83 C0 E0                               ADD RAX,-0x20
1402e7e5b:  48 03 C8                                  ADD RCX,RAX
1402e7e5e:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7e63:  E8 88 14 16 00                            CALL 0x1404492f0
1402e7e68:  C7 45 80 80 06 00 00                      MOV dword ptr [RBP + -0x80],0x680
1402e7e6f:  8B 05 C7 04 59 00                         MOV EAX,dword ptr [0x14087833c]
1402e7e75:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7e79:  0F B7 05 C0 04 59 00                      MOVZX EAX,word ptr [0x140878340]
1402e7e80:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7e85:  0F B6 05 B6 04 59 00                      MOVZX EAX,byte ptr [0x140878342]
1402e7e8c:  88 44 24 46                               MOV byte ptr [RSP + 0x46],AL
1402e7e90:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7e94:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7e97:  49 8B C6                                  MOV RAX,R14
1402e7e9a:  48 F7 E9                                  IMUL RCX
1402e7e9d:  48 C1 FA 05                               SAR RDX,0x5
1402e7ea1:  48 8B C2                                  MOV RAX,RDX
1402e7ea4:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7ea8:  48 03 D0                                  ADD RDX,RAX
1402e7eab:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7eaf:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7eb2:  48 83 C0 E0                               ADD RAX,-0x20
1402e7eb6:  48 03 C8                                  ADD RCX,RAX
1402e7eb9:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7ebe:  E8 2D 14 16 00                            CALL 0x1404492f0
1402e7ec3:  C7 45 80 93 06 00 00                      MOV dword ptr [RBP + -0x80],0x693
1402e7eca:  48 B8 6C 6F 61 6E 72 75 62 00             MOV RAX,0x6275726e616f6c
1402e7ed4:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e7ed9:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7edd:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7ee0:  49 8B C6                                  MOV RAX,R14
1402e7ee3:  48 F7 E9                                  IMUL RCX
1402e7ee6:  48 C1 FA 05                               SAR RDX,0x5
1402e7eea:  48 8B C2                                  MOV RAX,RDX
1402e7eed:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7ef1:  48 03 D0                                  ADD RDX,RAX
1402e7ef4:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7ef8:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7efb:  48 83 C0 E0                               ADD RAX,-0x20
1402e7eff:  48 03 C8                                  ADD RCX,RAX
1402e7f02:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7f07:  E8 E4 13 16 00                            CALL 0x1404492f0
1402e7f0c:  C7 45 80 94 06 00 00                      MOV dword ptr [RBP + -0x80],0x694
1402e7f13:  48 B8 6C 6F 61 6E 75 73 64 00             MOV RAX,0x6473756e616f6c
1402e7f1d:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e7f22:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7f26:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7f29:  49 8B C6                                  MOV RAX,R14
1402e7f2c:  48 F7 E9                                  IMUL RCX
1402e7f2f:  48 C1 FA 05                               SAR RDX,0x5
1402e7f33:  48 8B C2                                  MOV RAX,RDX
1402e7f36:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7f3a:  48 03 D0                                  ADD RDX,RAX
1402e7f3d:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7f41:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7f44:  48 83 C0 E0                               ADD RAX,-0x20
1402e7f48:  48 03 C8                                  ADD RCX,RAX
1402e7f4b:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7f50:  E8 9B 13 16 00                            CALL 0x1404492f0
1402e7f55:  C7 45 80 B2 92 00 00                      MOV dword ptr [RBP + -0x80],0x92b2
1402e7f5c:  4C 89 64 24 40                            MOV qword ptr [RSP + 0x40],R12
1402e7f61:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7f65:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7f68:  49 8B C6                                  MOV RAX,R14
1402e7f6b:  48 F7 E9                                  IMUL RCX
1402e7f6e:  48 C1 FA 05                               SAR RDX,0x5
1402e7f72:  48 8B C2                                  MOV RAX,RDX
1402e7f75:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7f79:  48 03 D0                                  ADD RDX,RAX
1402e7f7c:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7f80:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7f83:  48 83 C0 E0                               ADD RAX,-0x20
1402e7f87:  48 03 C8                                  ADD RCX,RAX
1402e7f8a:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7f8f:  E8 5C 13 16 00                            CALL 0x1404492f0
1402e7f94:  C7 45 80 FA 05 01 00                      MOV dword ptr [RBP + -0x80],0x105fa
1402e7f9b:  8B 05 73 FE 5B 00                         MOV EAX,dword ptr [0x1408a7e14]
1402e7fa1:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e7fa5:  0F B7 05 6C FE 5B 00                      MOVZX EAX,word ptr [0x1408a7e18]
1402e7fac:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e7fb1:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7fb5:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e7fb8:  49 8B C6                                  MOV RAX,R14
1402e7fbb:  48 F7 E9                                  IMUL RCX
1402e7fbe:  48 C1 FA 05                               SAR RDX,0x5
1402e7fc2:  48 8B C2                                  MOV RAX,RDX
1402e7fc5:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7fc9:  48 03 D0                                  ADD RDX,RAX
1402e7fcc:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e7fd0:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7fd3:  48 83 C0 E0                               ADD RAX,-0x20
1402e7fd7:  48 03 C8                                  ADD RCX,RAX
1402e7fda:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e7fdf:  E8 0C 13 16 00                            CALL 0x1404492f0
1402e7fe4:  C7 45 80 FA 05 00 00                      MOV dword ptr [RBP + -0x80],0x5fa
1402e7feb:  F2 0F 10 05 15 FE 5B 00                   MOVSD XMM0,qword ptr [0x1408a7e08]
1402e7ff3:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e7ff9:  0F B7 05 10 FE 5B 00                      MOVZX EAX,word ptr [0x1408a7e10]
1402e8000:  66 89 44 24 48                            MOV word ptr [RSP + 0x48],AX
1402e8005:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e8009:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e800c:  49 8B C6                                  MOV RAX,R14
1402e800f:  48 F7 E9                                  IMUL RCX
1402e8012:  48 C1 FA 05                               SAR RDX,0x5
1402e8016:  48 8B C2                                  MOV RAX,RDX
1402e8019:  48 C1 E8 3F                               SHR RAX,0x3f
1402e801d:  48 03 D0                                  ADD RDX,RAX
1402e8020:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e8024:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e8027:  48 83 C0 E0                               ADD RAX,-0x20
1402e802b:  48 03 C8                                  ADD RCX,RAX
1402e802e:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e8033:  E8 B8 12 16 00                            CALL 0x1404492f0
1402e8038:  8B 05 BE FD 5B 00                         MOV EAX,dword ptr [0x1408a7dfc]
1402e803e:  89 45 91                                  MOV dword ptr [RBP + -0x6f],EAX
1402e8041:  0F B6 05 B8 FD 5B 00                      MOVZX EAX,byte ptr [0x1408a7e00]
1402e8048:  88 45 95                                  MOV byte ptr [RBP + -0x6b],AL
1402e804b:  C7 45 D4 40 1F 00 00                      MOV dword ptr [RBP + -0x2c],0x1f40
1402e8052:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e8056:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e805a:  73 5E                                     JNC 0x1402e80ba
1402e805c:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e805f:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e8063:  48 3B C1                                  CMP RAX,RCX
1402e8066:  77 52                                     JA 0x1402e80ba
1402e8068:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e806c:  48 2B C8                                  SUB RCX,RAX
1402e806f:  49 8B C6                                  MOV RAX,R14
1402e8072:  48 F7 E9                                  IMUL RCX
1402e8075:  48 8B F2                                  MOV RSI,RDX
1402e8078:  48 C1 FE 05                               SAR RSI,0x5
1402e807c:  48 8B C6                                  MOV RAX,RSI
1402e807f:  48 C1 E8 3F                               SHR RAX,0x3f
1402e8083:  48 03 F0                                  ADD RSI,RAX
1402e8086:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e808a:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e808e:  75 08                                     JNZ 0x1402e8098
1402e8090:  48 8B CB                                  MOV RCX,RBX
1402e8093:  E8 A8 42 16 00                            CALL 0x14044c340
1402e8098:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e809c:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e80a1:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e80a5:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e80a8:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e80ad:  48 85 C9                                  TEST RCX,RCX
1402e80b0:  74 06                                     JZ 0x1402e80b8
1402e80b2:  E8 79 14 17 00                            CALL 0x140459530
1402e80b7:  90                                        NOP
1402e80b8:  EB 2F                                     JMP 0x1402e80e9
1402e80ba:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e80be:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e80c2:  75 08                                     JNZ 0x1402e80cc
1402e80c4:  48 8B CB                                  MOV RCX,RBX
1402e80c7:  E8 74 42 16 00                            CALL 0x14044c340
1402e80cc:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e80d0:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e80d5:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e80da:  48 85 C9                                  TEST RCX,RCX
1402e80dd:  74 0A                                     JZ 0x1402e80e9
1402e80df:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e80e3:  E8 48 14 17 00                            CALL 0x140459530
1402e80e8:  90                                        NOP
1402e80e9:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e80ee:  C7 45 80 41 1F 00 00                      MOV dword ptr [RBP + -0x80],0x1f41
1402e80f5:  F2 0F 10 05 43 FD 5B 00                   MOVSD XMM0,qword ptr [0x1408a7e40]
1402e80fd:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e8103:  0F B6 05 3E FD 5B 00                      MOVZX EAX,byte ptr [0x1408a7e48]
1402e810a:  88 44 24 48                               MOV byte ptr [RSP + 0x48],AL
1402e810e:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e8112:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e8115:  49 8B C6                                  MOV RAX,R14
1402e8118:  48 F7 E9                                  IMUL RCX
1402e811b:  48 C1 FA 05                               SAR RDX,0x5
1402e811f:  48 8B C2                                  MOV RAX,RDX
1402e8122:  48 C1 E8 3F                               SHR RAX,0x3f
1402e8126:  48 03 D0                                  ADD RDX,RAX
1402e8129:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e812d:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e8130:  48 83 C0 E0                               ADD RAX,-0x20
1402e8134:  48 03 C8                                  ADD RCX,RAX
1402e8137:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e813c:  E8 AF 11 16 00                            CALL 0x1404492f0
1402e8141:  C7 45 80 42 1F 00 00                      MOV dword ptr [RBP + -0x80],0x1f42
1402e8148:  8B 05 06 03 59 00                         MOV EAX,dword ptr [0x140878454]
1402e814e:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e8152:  0F B7 05 FF 02 59 00                      MOVZX EAX,word ptr [0x140878458]
1402e8159:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e815e:  0F B6 05 F5 02 59 00                      MOVZX EAX,byte ptr [0x14087845a]
1402e8165:  88 44 24 46                               MOV byte ptr [RSP + 0x46],AL
1402e8169:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e816d:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e8170:  49 8B C6                                  MOV RAX,R14
1402e8173:  48 F7 E9                                  IMUL RCX
1402e8176:  48 C1 FA 05                               SAR RDX,0x5
1402e817a:  48 8B C2                                  MOV RAX,RDX
1402e817d:  48 C1 E8 3F                               SHR RAX,0x3f
1402e8181:  48 03 D0                                  ADD RDX,RAX
1402e8184:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e8188:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e818b:  48 83 C0 E0                               ADD RAX,-0x20
1402e818f:  48 03 C8                                  ADD RCX,RAX
1402e8192:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e8197:  E8 54 11 16 00                            CALL 0x1404492f0
1402e819c:  C7 45 80 45 1F 00 00                      MOV dword ptr [RBP + -0x80],0x1f45
1402e81a3:  8B 05 8B FC 5B 00                         MOV EAX,dword ptr [0x1408a7e34]
1402e81a9:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e81ad:  0F B7 05 84 FC 5B 00                      MOVZX EAX,word ptr [0x1408a7e38]
1402e81b4:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e81b9:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e81bd:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e81c0:  49 8B C6                                  MOV RAX,R14
1402e81c3:  48 F7 E9                                  IMUL RCX
1402e81c6:  48 C1 FA 05                               SAR RDX,0x5
1402e81ca:  48 8B C2                                  MOV RAX,RDX
1402e81cd:  48 C1 E8 3F                               SHR RAX,0x3f
1402e81d1:  48 03 D0                                  ADD RDX,RAX
1402e81d4:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e81d8:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e81db:  48 83 C0 E0                               ADD RAX,-0x20
1402e81df:  48 03 C8                                  ADD RCX,RAX
1402e81e2:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e81e7:  E8 04 11 16 00                            CALL 0x1404492f0
1402e81ec:  C7 45 80 46 1F 00 00                      MOV dword ptr [RBP + -0x80],0x1f46
1402e81f3:  F2 0F 10 05 2D FC 5B 00                   MOVSD XMM0,qword ptr [0x1408a7e28]
1402e81fb:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
; 977 instruction(s)
