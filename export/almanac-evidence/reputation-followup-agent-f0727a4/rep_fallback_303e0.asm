; Range: 0x1800303E0 to 0x180030710  (containing fn: FUN_1800303e0@0x1800303E0)
1800303e0:  40 57                                     PUSH RDI
1800303e2:  41 56                                     PUSH R14
1800303e4:  48 81 EC 58 01 00 00                      SUB RSP,0x158
1800303eb:  48 8B 05 8E 3D 09 00                      MOV RAX,qword ptr [0x1800c4180]
1800303f2:  48 33 C4                                  XOR RAX,RSP
1800303f5:  48 89 84 24 40 01 00 00                   MOV qword ptr [RSP + 0x140],RAX
1800303fd:  83 3D 8C 2C 09 00 00                      CMP dword ptr [0x1800c3090],0x0
180030404:  4C 8B F2                                  MOV R14,RDX
180030407:  48 8B F9                                  MOV RDI,RCX
18003040a:  0F 84 CC 02 00 00                         JZ 0x1800306dc
180030410:  48 85 C9                                  TEST RCX,RCX
180030413:  0F 84 C3 02 00 00                         JZ 0x1800306dc
180030419:  48 85 D2                                  TEST RDX,RDX
18003041c:  0F 84 BA 02 00 00                         JZ 0x1800306dc
180030422:  83 3D AB 6E 0A 00 00                      CMP dword ptr [0x1800d72d4],0x0
180030429:  74 0D                                     JZ 0x180030438
18003042b:  48 8D 0D 76 49 09 00                      LEA RCX,[0x1800c4da8]
180030432:  FF 15 80 CC 05 00                         CALL qword ptr [0x18008d0b8]
180030438:  80 3F 00                                  CMP byte ptr [RDI],0x0
18003043b:  48 8B CF                                  MOV RCX,RDI
18003043e:  48 89 9C 24 80 01 00 00                   MOV qword ptr [RSP + 0x180],RBX
180030446:  48 89 B4 24 50 01 00 00                   MOV qword ptr [RSP + 0x150],RSI
18003044e:  74 47                                     JZ 0x180030497
180030450:  0F B6 01                                  MOVZX EAX,byte ptr [RCX]
180030453:  3C 5C                                     CMP AL,0x5c
180030455:  74 04                                     JZ 0x18003045b
180030457:  3C 2F                                     CMP AL,0x2f
180030459:  75 34                                     JNZ 0x18003048f
18003045b:  0F B6 41 01                               MOVZX EAX,byte ptr [RCX + 0x1]
18003045f:  04 AD                                     ADD AL,0xad
180030461:  A8 DF                                     TEST AL,0xdf
180030463:  75 2A                                     JNZ 0x18003048f
180030465:  0F B6 41 02                               MOVZX EAX,byte ptr [RCX + 0x2]
180030469:  04 BF                                     ADD AL,0xbf
18003046b:  A8 DF                                     TEST AL,0xdf
18003046d:  75 20                                     JNZ 0x18003048f
18003046f:  0F B6 41 03                               MOVZX EAX,byte ptr [RCX + 0x3]
180030473:  04 AA                                     ADD AL,0xaa
180030475:  A8 DF                                     TEST AL,0xdf
180030477:  75 16                                     JNZ 0x18003048f
180030479:  0F B6 41 04                               MOVZX EAX,byte ptr [RCX + 0x4]
18003047d:  04 BB                                     ADD AL,0xbb
18003047f:  A8 DF                                     TEST AL,0xdf
180030481:  75 0C                                     JNZ 0x18003048f
180030483:  0F B6 41 05                               MOVZX EAX,byte ptr [RCX + 0x5]
180030487:  3C 5C                                     CMP AL,0x5c
180030489:  74 61                                     JZ 0x1800304ec
18003048b:  3C 2F                                     CMP AL,0x2f
18003048d:  74 5D                                     JZ 0x1800304ec
18003048f:  48 FF C1                                  INC RCX
180030492:  80 39 00                                  CMP byte ptr [RCX],0x0
180030495:  75 B9                                     JNZ 0x180030450
180030497:  33 F6                                     XOR ESI,ESI
180030499:  83 3D EC 2B 09 00 01                      CMP dword ptr [0x1800c308c],0x1
1800304a0:  48 8B 9C 24 80 01 00 00                   MOV RBX,qword ptr [RSP + 0x180]
1800304a8:  7E 2B                                     JLE 0x1800304d5
1800304aa:  48 85 F6                                  TEST RSI,RSI
1800304ad:  75 14                                     JNZ 0x1800304c3
1800304af:  48 8D 15 02 02 06 00                      LEA RDX,[0x1800906b8]
1800304b6:  48 8B CF                                  MOV RCX,RDI
1800304b9:  E8 12 A6 03 00                            CALL 0x18006aad0
1800304be:  48 85 C0                                  TEST RAX,RAX
1800304c1:  74 17                                     JZ 0x1800304da
1800304c3:  4C 8B C7                                  MOV R8,RDI
1800304c6:  48 8D 0D F3 01 06 00                      LEA RCX,[0x1800906c0]
1800304cd:  49 8B D6                                  MOV RDX,R14
1800304d0:  E8 1B 3A 02 00                            CALL 0x180053ef0
1800304d5:  48 85 F6                                  TEST RSI,RSI
1800304d8:  75 78                                     JNZ 0x180030552
1800304da:  83 3D F3 6D 0A 00 00                      CMP dword ptr [0x1800d72d4],0x0
1800304e1:  0F 85 E0 01 00 00                         JNZ 0x1800306c7
1800304e7:  E9 E8 01 00 00                            JMP 0x1800306d4
1800304ec:  48 8D 59 06                               LEA RBX,[RCX + 0x6]
1800304f0:  48 85 DB                                  TEST RBX,RBX
1800304f3:  74 A2                                     JZ 0x180030497
1800304f5:  80 3B 00                                  CMP byte ptr [RBX],0x0
1800304f8:  74 9D                                     JZ 0x180030497
1800304fa:  66 0F 1F 44 00 00                         NOP word ptr [RAX + RAX*0x1]
180030500:  0F B6 03                                  MOVZX EAX,byte ptr [RBX]
180030503:  0F B6 C8                                  MOVZX ECX,AL
180030506:  3C 5C                                     CMP AL,0x5c
180030508:  74 0F                                     JZ 0x180030519
18003050a:  3C 2F                                     CMP AL,0x2f
18003050c:  74 0B                                     JZ 0x180030519
18003050e:  0F B6 4B 01                               MOVZX ECX,byte ptr [RBX + 0x1]
180030512:  48 FF C3                                  INC RBX
180030515:  84 C9                                     TEST CL,CL
180030517:  75 E7                                     JNZ 0x180030500
180030519:  84 C9                                     TEST CL,CL
18003051b:  0F 84 76 FF FF FF                         JZ 0x180030497
180030521:  48 2B DF                                  SUB RBX,RDI
180030524:  48 8D 43 FF                               LEA RAX,[RBX + -0x1]
180030528:  48 3D 02 01 00 00                         CMP RAX,0x102
18003052e:  0F 87 63 FF FF FF                         JA 0x180030497
180030534:  48 8D 35 A5 23 46 00                      LEA RSI,[0x1804928e0]
18003053b:  4C 8B C3                                  MOV R8,RBX
18003053e:  48 8B CE                                  MOV RCX,RSI
180030541:  48 8B D7                                  MOV RDX,RDI
180030544:  E8 97 6C 05 00                            CALL 0x1800871e0
180030549:  C6 04 33 00                               MOV byte ptr [RBX + RSI*0x1],0x0
18003054d:  E9 47 FF FF FF                            JMP 0x180030499
180030552:  41 0F B6 06                               MOVZX EAX,byte ptr [R14]
180030556:  3C 77                                     CMP AL,0x77
180030558:  74 66                                     JZ 0x1800305c0
18003055a:  3C 61                                     CMP AL,0x61
18003055c:  74 62                                     JZ 0x1800305c0
18003055e:  3C 72                                     CMP AL,0x72
180030560:  0F 85 58 01 00 00                         JNZ 0x1800306be
180030566:  80 3F 00                                  CMP byte ptr [RDI],0x0
180030569:  48 8B C7                                  MOV RAX,RDI
18003056c:  74 1B                                     JZ 0x180030589
18003056e:  66 90                                     NOP
180030570:  0F B6 08                                  MOVZX ECX,byte ptr [RAX]
180030573:  80 F9 5C                                  CMP CL,0x5c
180030576:  74 05                                     JZ 0x18003057d
180030578:  80 F9 2F                                  CMP CL,0x2f
18003057b:  75 04                                     JNZ 0x180030581
18003057d:  48 8D 78 01                               LEA RDI,[RAX + 0x1]
180030581:  48 FF C0                                  INC RAX
180030584:  80 38 00                                  CMP byte ptr [RAX],0x0
180030587:  75 E7                                     JNZ 0x180030570
180030589:  48 8D 15 28 02 06 00                      LEA RDX,[0x1800907b8]
180030590:  48 8B CF                                  MOV RCX,RDI
180030593:  E8 58 D5 03 00                            CALL 0x18006daf0
180030598:  85 C0                                     TEST EAX,EAX
18003059a:  0F 85 1E 01 00 00                         JNZ 0x1800306be
1800305a0:  49 C7 C1 FF FF FF FF                      MOV R9,-0x1
1800305a7:  48 8D 0D E2 7A 15 00                      LEA RCX,[0x180188090]
1800305ae:  4C 8B C6                                  MOV R8,RSI
1800305b1:  BA 04 01 00 00                            MOV EDX,0x104
1800305b6:  E8 35 D4 03 00                            CALL 0x18006d9f0
1800305bb:  E9 FE 00 00 00                            JMP 0x1800306be
1800305c0:  48 8B D6                                  MOV RDX,RSI
1800305c3:  48 8D 0D B6 79 15 00                      LEA RCX,[0x180187f80]
1800305ca:  E8 21 D5 03 00                            CALL 0x18006daf0
1800305cf:  85 C0                                     TEST EAX,EAX
1800305d1:  74 4F                                     JZ 0x180030622
1800305d3:  80 3D B6 7A 15 00 00                      CMP byte ptr [0x180188090],0x0
1800305da:  75 46                                     JNZ 0x180030622
1800305dc:  4C 8D 0D 0D FA 05 00                      LEA R9,[0x18008fff0]
1800305e3:  48 89 74 24 20                            MOV qword ptr [RSP + 0x20],RSI
1800305e8:  BA 04 01 00 00                            MOV EDX,0x104
1800305ed:  48 8D 4C 24 30                            LEA RCX,[RSP + 0x30]
1800305f2:  49 C7 C0 FF FF FF FF                      MOV R8,-0x1
1800305f9:  E8 52 94 03 00                            CALL 0x180069a50
1800305fe:  48 8D 4C 24 30                            LEA RCX,[RSP + 0x30]
180030603:  FF 15 67 CA 05 00                         CALL qword ptr [0x18008d070]
180030609:  83 F8 FF                                  CMP EAX,-0x1
18003060c:  74 14                                     JZ 0x180030622
18003060e:  4C 8B C6                                  MOV R8,RSI
180030611:  48 8D 15 B8 00 06 00                      LEA RDX,[0x1800906d0]
180030618:  B9 05 00 00 00                            MOV ECX,0x5
18003061d:  E8 0E B4 FE FF                            CALL 0x18001ba30
180030622:  49 C7 C1 FF FF FF FF                      MOV R9,-0x1
180030629:  48 8D 0D 50 79 15 00                      LEA RCX,[0x180187f80]
180030630:  4C 8B C6                                  MOV R8,RSI
180030633:  BA 04 01 00 00                            MOV EDX,0x104
180030638:  E8 B3 D3 03 00                            CALL 0x18006d9f0
18003063d:  48 8B CE                                  MOV RCX,RSI
180030640:  E8 EB 4A 02 00                            CALL 0x180055130
180030645:  80 3F 00                                  CMP byte ptr [RDI],0x0
180030648:  48 8B C7                                  MOV RAX,RDI
18003064b:  74 1C                                     JZ 0x180030669
18003064d:  0F 1F 00                                  NOP dword ptr [RAX]
180030650:  0F B6 08                                  MOVZX ECX,byte ptr [RAX]
180030653:  80 F9 5C                                  CMP CL,0x5c
180030656:  74 05                                     JZ 0x18003065d
180030658:  80 F9 2F                                  CMP CL,0x2f
18003065b:  75 04                                     JNZ 0x180030661
18003065d:  48 8D 78 01                               LEA RDI,[RAX + 0x1]
180030661:  48 FF C0                                  INC RAX
180030664:  80 38 00                                  CMP byte ptr [RAX],0x0
180030667:  75 E7                                     JNZ 0x180030650
180030669:  48 8D 15 20 01 06 00                      LEA RDX,[0x180090790]
180030670:  48 8B CF                                  MOV RCX,RDI
180030673:  E8 78 D4 03 00                            CALL 0x18006daf0
180030678:  85 C0                                     TEST EAX,EAX
18003067a:  75 14                                     JNZ 0x180030690
18003067c:  E8 0F 41 03 00                            CALL 0x180064790
180030681:  48 8B C8                                  MOV RCX,RAX
180030684:  BA 01 00 00 00                            MOV EDX,0x1
180030689:  E8 42 78 FD FF                            CALL 0x180007ed0
18003068e:  EB 2E                                     JMP 0x1800306be
180030690:  48 8D 15 11 01 06 00                      LEA RDX,[0x1800907a8]
180030697:  48 8B CF                                  MOV RCX,RDI
18003069a:  E8 51 D4 03 00                            CALL 0x18006daf0
18003069f:  85 C0                                     TEST EAX,EAX
1800306a1:  75 1B                                     JNZ 0x1800306be
1800306a3:  39 05 E7 38 17 00                         CMP dword ptr [0x1801a3f90],EAX
1800306a9:  74 13                                     JZ 0x1800306be
1800306ab:  89 05 DF 38 17 00                         MOV dword ptr [0x1801a3f90],EAX
1800306b1:  E8 DA 40 03 00                            CALL 0x180064790
1800306b6:  48 8B C8                                  MOV RCX,RAX
1800306b9:  E8 12 79 FD FF                            CALL 0x180007fd0
1800306be:  83 3D 0F 6C 0A 00 00                      CMP dword ptr [0x1800d72d4],0x0
1800306c5:  74 0D                                     JZ 0x1800306d4
1800306c7:  48 8D 0D DA 46 09 00                      LEA RCX,[0x1800c4da8]
1800306ce:  FF 15 EC C9 05 00                         CALL qword ptr [0x18008d0c0]
1800306d4:  48 8B B4 24 50 01 00 00                   MOV RSI,qword ptr [RSP + 0x150]
1800306dc:  48 8B 8C 24 40 01 00 00                   MOV RCX,qword ptr [RSP + 0x140]
1800306e4:  48 33 CC                                  XOR RCX,RSP
1800306e7:  E8 A4 97 03 00                            CALL 0x180069e90
1800306ec:  48 81 C4 58 01 00 00                      ADD RSP,0x158
1800306f3:  41 5E                                     POP R14
1800306f5:  5F                                        POP RDI
1800306f6:  C3                                        RET
180030700:  48 89 5C 24 18                            MOV qword ptr [RSP + 0x18],RBX
180030705:  48 89 74 24 20                            MOV qword ptr [RSP + 0x20],RSI
18003070a:  48 89 54 24 10                            MOV qword ptr [RSP + 0x10],RDX
18003070f:  48 89 4C 24 08                            MOV qword ptr [RSP + 0x8],RCX
; 210 instruction(s)
