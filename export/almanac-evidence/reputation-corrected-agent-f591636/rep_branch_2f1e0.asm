; Range: 0x18002F1E0 to 0x18002F420  (containing fn: FUN_18002f1e0@0x18002F1E0)
18002f1e0:  45 33 C9                                  XOR R9D,R9D
18002f1e3:  83 F9 07                                  CMP ECX,0x7
18002f1e6:  0F 87 3B 01 00 00                         JA 0x18002f327
18002f1ec:  4C 8D 05 0D 0E FD FF                      LEA R8,[0x180000000]
18002f1f3:  48 63 C1                                  MOVSXD RAX,ECX
18002f1f6:  41 8B 94 80 F8 F3 02 00                   MOV EDX,dword ptr [R8 + RAX*0x4 + 0x2f3f8]
18002f1fe:  49 03 D0                                  ADD RDX,R8
18002f201:  FF E2                                     JMP RDX
18002f203:  44 39 0D 4E 3F 09 00                      CMP dword ptr [0x1800c3158],R9D
18002f20a:  BA 02 00 00 00                            MOV EDX,0x2
18002f20f:  C7 05 57 3F 09 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
18002f219:  41 0F 45 D1                               CMOVNZ EDX,R9D
18002f21d:  89 15 35 3F 09 00                         MOV dword ptr [0x1800c3158],EDX
18002f223:  E9 FF 00 00 00                            JMP 0x18002f327
18002f228:  41 8B C1                                  MOV EAX,R9D
18002f22b:  44 89 0D 52 6E 25 00                      MOV dword ptr [0x180286084],R9D
18002f232:  39 05 48 6E 25 00                         CMP dword ptr [0x180286080],EAX
18002f238:  0F 94 C0                                  SETZ AL
18002f23b:  89 05 3F 6E 25 00                         MOV dword ptr [0x180286080],EAX
18002f241:  E9 E1 00 00 00                            JMP 0x18002f327
18002f246:  44 39 0D 5B 41 09 00                      CMP dword ptr [0x1800c33a8],R9D
18002f24d:  0F 84 D4 00 00 00                         JZ 0x18002f327
18002f253:  41 8B C1                                  MOV EAX,R9D
18002f256:  C7 05 10 3F 09 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
18002f260:  39 05 B2 E7 19 00                         CMP dword ptr [0x1801cda18],EAX
18002f266:  0F 94 C0                                  SETZ AL
18002f269:  89 05 A9 E7 19 00                         MOV dword ptr [0x1801cda18],EAX
18002f26f:  E9 B3 00 00 00                            JMP 0x18002f327
18002f274:  44 39 0D 29 41 09 00                      CMP dword ptr [0x1800c33a4],R9D
18002f27b:  0F 84 A6 00 00 00                         JZ 0x18002f327
18002f281:  41 8B C1                                  MOV EAX,R9D
18002f284:  C7 05 E2 3E 09 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
18002f28e:  39 05 80 E7 19 00                         CMP dword ptr [0x1801cda14],EAX
18002f294:  0F 94 C0                                  SETZ AL
18002f297:  89 05 77 E7 19 00                         MOV dword ptr [0x1801cda14],EAX
18002f29d:  E9 85 00 00 00                            JMP 0x18002f327
18002f2a2:  44 39 0D 07 97 09 00                      CMP dword ptr [0x1800c89b0],R9D
18002f2a9:  75 7C                                     JNZ 0x18002f327
18002f2ab:  83 3D 02 5C 15 00 01                      CMP dword ptr [0x180184eb4],0x1
18002f2b2:  41 8B C1                                  MOV EAX,R9D
18002f2b5:  44 89 0D F4 5B 15 00                      MOV dword ptr [0x180184eb0],R9D
18002f2bc:  0F 95 C0                                  SETNZ AL
18002f2bf:  89 05 EF 5B 15 00                         MOV dword ptr [0x180184eb4],EAX
18002f2c5:  EB 60                                     JMP 0x18002f327
18002f2c7:  44 39 0D E2 96 09 00                      CMP dword ptr [0x1800c89b0],R9D
18002f2ce:  75 57                                     JNZ 0x18002f327
18002f2d0:  BA 02 00 00 00                            MOV EDX,0x2
18002f2d5:  44 89 0D D4 5B 15 00                      MOV dword ptr [0x180184eb0],R9D
18002f2dc:  39 15 D2 5B 15 00                         CMP dword ptr [0x180184eb4],EDX
18002f2e2:  41 0F 44 D1                               CMOVZ EDX,R9D
18002f2e6:  89 15 C8 5B 15 00                         MOV dword ptr [0x180184eb4],EDX
18002f2ec:  EB 39                                     JMP 0x18002f327
18002f2ee:  44 39 0D B7 40 09 00                      CMP dword ptr [0x1800c33ac],R9D
18002f2f5:  74 30                                     JZ 0x18002f327
18002f2f7:  41 8B C1                                  MOV EAX,R9D
18002f2fa:  C7 05 6C 3E 09 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
18002f304:  39 05 12 E7 19 00                         CMP dword ptr [0x1801cda1c],EAX
18002f30a:  0F 94 C0                                  SETZ AL
18002f30d:  89 05 09 E7 19 00                         MOV dword ptr [0x1801cda1c],EAX
18002f313:  EB 12                                     JMP 0x18002f327
18002f315:  41 8B C1                                  MOV EAX,R9D
18002f318:  39 05 32 59 09 00                         CMP dword ptr [0x1800c4c50],EAX
18002f31e:  0F 94 C0                                  SETZ AL
18002f321:  89 05 29 59 09 00                         MOV dword ptr [0x1800c4c50],EAX
18002f327:  44 39 0D 52 44 09 00                      CMP dword ptr [0x1800c3780],R9D
18002f32e:  74 64                                     JZ 0x18002f394
18002f330:  83 F9 01                                  CMP ECX,0x1
18002f333:  7C 1C                                     JL 0x18002f351
18002f335:  44 39 0D 1C 3E 09 00                      CMP dword ptr [0x1800c3158],R9D
18002f33c:  74 5B                                     JZ 0x18002f399
18002f33e:  44 89 0D 13 3E 09 00                      MOV dword ptr [0x1800c3158],R9D
18002f345:  C7 05 21 3E 09 00 01 00 00 00             MOV dword ptr [0x1800c3170],0x1
18002f34f:  EB 48                                     JMP 0x18002f399
18002f351:  85 C9                                     TEST ECX,ECX
18002f353:  0F 85 9B 00 00 00                         JNZ 0x18002f3f4
18002f359:  44 39 0D 20 6D 25 00                      CMP dword ptr [0x180286080],R9D
18002f360:  74 0E                                     JZ 0x18002f370
18002f362:  44 89 0D 17 6D 25 00                      MOV dword ptr [0x180286080],R9D
18002f369:  44 89 0D 14 6D 25 00                      MOV dword ptr [0x180286084],R9D
18002f370:  44 89 0D A5 E6 19 00                      MOV dword ptr [0x1801cda1c],R9D
18002f377:  44 89 0D 36 5B 15 00                      MOV dword ptr [0x180184eb4],R9D
18002f37e:  44 89 0D 8F E6 19 00                      MOV dword ptr [0x1801cda14],R9D
18002f385:  44 89 0D 8C E6 19 00                      MOV dword ptr [0x1801cda18],R9D
18002f38c:  44 89 0D BD 58 09 00                      MOV dword ptr [0x1800c4c50],R9D
18002f393:  C3                                        RET
18002f394:  83 F9 01                                  CMP ECX,0x1
18002f397:  7C 5B                                     JL 0x18002f3f4
18002f399:  83 F9 01                                  CMP ECX,0x1
18002f39c:  74 1C                                     JZ 0x18002f3ba
18002f39e:  44 39 0D DB 6C 25 00                      CMP dword ptr [0x180286080],R9D
18002f3a5:  74 0E                                     JZ 0x18002f3b5
18002f3a7:  44 89 0D D2 6C 25 00                      MOV dword ptr [0x180286080],R9D
18002f3ae:  44 89 0D CF 6C 25 00                      MOV dword ptr [0x180286084],R9D
18002f3b5:  83 F9 02                                  CMP ECX,0x2
18002f3b8:  74 0C                                     JZ 0x18002f3c6
18002f3ba:  44 89 0D 57 E6 19 00                      MOV dword ptr [0x1801cda18],R9D
18002f3c1:  83 F9 03                                  CMP ECX,0x3
18002f3c4:  74 0F                                     JZ 0x18002f3d5
18002f3c6:  8D 41 FC                                  LEA EAX,[RCX + -0x4]
18002f3c9:  44 89 0D 44 E6 19 00                      MOV dword ptr [0x1801cda14],R9D
18002f3d0:  83 F8 01                                  CMP EAX,0x1
18002f3d3:  76 0C                                     JBE 0x18002f3e1
18002f3d5:  44 89 0D D8 5A 15 00                      MOV dword ptr [0x180184eb4],R9D
18002f3dc:  83 F9 06                                  CMP ECX,0x6
18002f3df:  74 0C                                     JZ 0x18002f3ed
18002f3e1:  44 89 0D 34 E6 19 00                      MOV dword ptr [0x1801cda1c],R9D
18002f3e8:  83 F9 07                                  CMP ECX,0x7
18002f3eb:  74 07                                     JZ 0x18002f3f4
18002f3ed:  44 89 0D 5C 58 09 00                      MOV dword ptr [0x1800c4c50],R9D
18002f3f4:  C3                                        RET
18002f420:  48 8B C4                                  MOV RAX,RSP
; 111 instruction(s)
