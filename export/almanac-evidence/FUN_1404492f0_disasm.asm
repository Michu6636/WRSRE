; Range: 0x1404492F0 to 0x1404493E6  (containing fn: FUN_1404492f0@0x1404492F0)
1404492f0:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
1404492f5:  57                                        PUSH RDI
1404492f6:  48 83 EC 20                               SUB RSP,0x20
1404492fa:  48 8B D9                                  MOV RBX,RCX
1404492fd:  48 8B FA                                  MOV RDI,RDX
140449300:  48 8B 49 08                               MOV RCX,qword ptr [RCX + 0x8]
140449304:  48 3B D1                                  CMP RDX,RCX
140449307:  0F 83 8D 00 00 00                         JNC 0x14044939a
14044930d:  48 8B 03                                  MOV RAX,qword ptr [RBX]
140449310:  48 3B C2                                  CMP RAX,RDX
140449313:  0F 87 81 00 00 00                         JA 0x14044939a
140449319:  48 2B F8                                  SUB RDI,RAX
14044931c:  48 B8 67 66 66 66 66 66 66 66             MOV RAX,0x6666666666666667
140449326:  48 F7 EF                                  IMUL RDI
140449329:  48 8B FA                                  MOV RDI,RDX
14044932c:  48 C1 FF 05                               SAR RDI,0x5
140449330:  48 8B C7                                  MOV RAX,RDI
140449333:  48 C1 E8 3F                               SHR RAX,0x3f
140449337:  48 03 F8                                  ADD RDI,RAX
14044933a:  48 3B 4B 10                               CMP RCX,qword ptr [RBX + 0x10]
14044933e:  75 08                                     JNZ 0x140449348
140449340:  48 8B CB                                  MOV RCX,RBX
140449343:  E8 C8 30 00 00                            CALL 0x14044c410
140449348:  48 8B 53 08                               MOV RDX,qword ptr [RBX + 0x8]
14044934c:  48 85 D2                                  TEST RDX,RDX
14044934f:  0F 84 82 00 00 00                         JZ 0x1404493d7
140449355:  48 8B 03                                  MOV RAX,qword ptr [RBX]
140449358:  48 8D 0C BF                               LEA RCX,[RDI + RDI*0x4]
14044935c:  48 03 C9                                  ADD RCX,RCX
14044935f:  0F 10 04 C8                               MOVUPS XMM0,xmmword ptr [RAX + RCX*0x8]
140449363:  0F 11 02                                  MOVUPS xmmword ptr [RDX],XMM0
140449366:  0F 10 4C C8 10                            MOVUPS XMM1,xmmword ptr [RAX + RCX*0x8 + 0x10]
14044936b:  0F 11 4A 10                               MOVUPS xmmword ptr [RDX + 0x10],XMM1
14044936f:  0F 10 44 C8 20                            MOVUPS XMM0,xmmword ptr [RAX + RCX*0x8 + 0x20]
140449374:  0F 11 42 20                               MOVUPS xmmword ptr [RDX + 0x20],XMM0
140449378:  0F 10 4C C8 30                            MOVUPS XMM1,xmmword ptr [RAX + RCX*0x8 + 0x30]
14044937d:  0F 11 4A 30                               MOVUPS xmmword ptr [RDX + 0x30],XMM1
140449381:  0F 10 44 C8 40                            MOVUPS XMM0,xmmword ptr [RAX + RCX*0x8 + 0x40]
140449386:  0F 11 42 40                               MOVUPS xmmword ptr [RDX + 0x40],XMM0
14044938a:  48 83 43 08 50                            ADD qword ptr [RBX + 0x8],0x50
14044938f:  48 8B 5C 24 30                            MOV RBX,qword ptr [RSP + 0x30]
140449394:  48 83 C4 20                               ADD RSP,0x20
140449398:  5F                                        POP RDI
140449399:  C3                                        RET
14044939a:  48 3B 4B 10                               CMP RCX,qword ptr [RBX + 0x10]
14044939e:  75 08                                     JNZ 0x1404493a8
1404493a0:  48 8B CB                                  MOV RCX,RBX
1404493a3:  E8 68 30 00 00                            CALL 0x14044c410
1404493a8:  48 8B 43 08                               MOV RAX,qword ptr [RBX + 0x8]
1404493ac:  48 85 C0                                  TEST RAX,RAX
1404493af:  74 26                                     JZ 0x1404493d7
1404493b1:  0F 10 07                                  MOVUPS XMM0,xmmword ptr [RDI]
1404493b4:  0F 11 00                                  MOVUPS xmmword ptr [RAX],XMM0
1404493b7:  0F 10 4F 10                               MOVUPS XMM1,xmmword ptr [RDI + 0x10]
1404493bb:  0F 11 48 10                               MOVUPS xmmword ptr [RAX + 0x10],XMM1
1404493bf:  0F 10 47 20                               MOVUPS XMM0,xmmword ptr [RDI + 0x20]
1404493c3:  0F 11 40 20                               MOVUPS xmmword ptr [RAX + 0x20],XMM0
1404493c7:  0F 10 4F 30                               MOVUPS XMM1,xmmword ptr [RDI + 0x30]
1404493cb:  0F 11 48 30                               MOVUPS xmmword ptr [RAX + 0x30],XMM1
1404493cf:  0F 10 47 40                               MOVUPS XMM0,xmmword ptr [RDI + 0x40]
1404493d3:  0F 11 40 40                               MOVUPS xmmword ptr [RAX + 0x40],XMM0
1404493d7:  48 83 43 08 50                            ADD qword ptr [RBX + 0x8],0x50
1404493dc:  48 8B 5C 24 30                            MOV RBX,qword ptr [RSP + 0x30]
1404493e1:  48 83 C4 20                               ADD RSP,0x20
1404493e5:  5F                                        POP RDI
1404493e6:  C3                                        RET
; 66 instruction(s)
