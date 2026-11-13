; Range: 0x180058310 to 0x180058510  (containing fn: FUN_180058310@0x180058310)
180058310:  40 53                                     PUSH RBX
180058312:  48 83 EC 20                               SUB RSP,0x20
180058316:  83 3D 3F B9 06 00 00                      CMP dword ptr [0x1800c3c5c],0x0
18005831d:  7C 79                                     JL 0x180058398
18005831f:  83 3D 22 F8 22 00 00                      CMP dword ptr [0x180287b48],0x0
180058326:  74 70                                     JZ 0x180058398
180058328:  48 8B 1D 51 CA 06 00                      MOV RBX,qword ptr [0x1800c4d80]
18005832f:  BA 08 00 00 00                            MOV EDX,0x8
180058334:  48 8B 05 35 CA 06 00                      MOV RAX,qword ptr [0x1800c4d70]
18005833b:  48 8D 8B 60 15 9E 00                      LEA RCX,[RBX + 0x9e1560]
180058342:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180058345:  85 C0                                     TEST EAX,EAX
180058347:  74 4F                                     JZ 0x180058398
180058349:  48 8B 8B 60 15 9E 00                      MOV RCX,qword ptr [RBX + 0x9e1560]
180058350:  48 85 C9                                  TEST RCX,RCX
180058353:  74 43                                     JZ 0x180058398
180058355:  48 8B 05 14 CA 06 00                      MOV RAX,qword ptr [0x1800c4d70]
18005835c:  48 FF C1                                  INC RCX
18005835f:  BA 17 00 00 00                            MOV EDX,0x17
180058364:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180058367:  85 C0                                     TEST EAX,EAX
180058369:  74 2D                                     JZ 0x180058398
18005836b:  48 8B 93 60 15 9E 00                      MOV RDX,qword ptr [RBX + 0x9e1560]
180058372:  4C 8D 05 AF B8 06 00                      LEA R8,[0x1800c3c28]
180058379:  33 C0                                     XOR EAX,EAX
18005837b:  0F 1F 44 00 00                            NOP dword ptr [RAX + RAX*0x1]
180058380:  0F B6 4C 02 01                            MOVZX ECX,byte ptr [RDX + RAX*0x1 + 0x1]
180058385:  41 3A 0C 00                               CMP CL,byte ptr [R8 + RAX*0x1]
180058389:  75 0D                                     JNZ 0x180058398
18005838b:  84 C9                                     TEST CL,CL
18005838d:  74 11                                     JZ 0x1800583a0
18005838f:  48 FF C0                                  INC RAX
180058392:  48 83 F8 17                               CMP RAX,0x17
180058396:  72 E8                                     JC 0x180058380
180058398:  33 C0                                     XOR EAX,EAX
18005839a:  48 83 C4 20                               ADD RSP,0x20
18005839e:  5B                                        POP RBX
18005839f:  C3                                        RET
1800583a0:  48 8B 1D D9 C9 06 00                      MOV RBX,qword ptr [0x1800c4d80]
1800583a7:  BA 04 00 00 00                            MOV EDX,0x4
1800583ac:  48 8B 05 BD C9 06 00                      MOV RAX,qword ptr [0x1800c4d70]
1800583b3:  48 8D 8B 6C 15 9E 00                      LEA RCX,[RBX + 0x9e156c]
1800583ba:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
1800583bd:  85 C0                                     TEST EAX,EAX
1800583bf:  74 D7                                     JZ 0x180058398
1800583c1:  8B 0D 95 B8 06 00                         MOV ECX,dword ptr [0x1800c3c5c]
1800583c7:  33 C0                                     XOR EAX,EAX
1800583c9:  39 8B 6C 15 9E 00                         CMP dword ptr [RBX + 0x9e156c],ECX
1800583cf:  0F 94 C0                                  SETZ AL
1800583d2:  48 83 C4 20                               ADD RSP,0x20
1800583d6:  5B                                        POP RBX
1800583d7:  C3                                        RET
1800583e0:  48 81 EC 68 02 00 00                      SUB RSP,0x268
1800583e7:  48 8B 05 92 BD 06 00                      MOV RAX,qword ptr [0x1800c4180]
1800583ee:  48 33 C4                                  XOR RAX,RSP
1800583f1:  48 89 84 24 30 02 00 00                   MOV qword ptr [RSP + 0x230],RAX
1800583f9:  83 3D 9C F5 07 00 00                      CMP dword ptr [0x1800d799c],0x0
180058400:  0F 84 0F 05 00 00                         JZ 0x180058915
180058406:  83 3D 37 F7 22 00 00                      CMP dword ptr [0x180287b44],0x0
18005840d:  0F 85 02 05 00 00                         JNZ 0x180058915
180058413:  48 8B 05 56 C9 06 00                      MOV RAX,qword ptr [0x1800c4d70]
18005841a:  BA 18 00 00 00                            MOV EDX,0x18
18005841f:  48 89 9C 24 70 02 00 00                   MOV qword ptr [RSP + 0x270],RBX
180058427:  48 8B 1D 52 C9 06 00                      MOV RBX,qword ptr [0x1800c4d80]
18005842e:  48 8D 8B 18 15 9E 00                      LEA RCX,[RBX + 0x9e1518]
180058435:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180058438:  85 C0                                     TEST EAX,EAX
18005843a:  0F 84 CD 04 00 00                         JZ 0x18005890d
180058440:  48 8B 83 18 15 9E 00                      MOV RAX,qword ptr [RBX + 0x9e1518]
180058447:  48 85 C0                                  TEST RAX,RAX
18005844a:  0F 84 BD 04 00 00                         JZ 0x18005890d
180058450:  48 8B 8B 20 15 9E 00                      MOV RCX,qword ptr [RBX + 0x9e1520]
180058457:  48 3B C8                                  CMP RCX,RAX
18005845a:  0F 82 AD 04 00 00                         JC 0x18005890d
180058460:  48 2B C8                                  SUB RCX,RAX
180058463:  4C 89 AC 24 50 02 00 00                   MOV qword ptr [RSP + 0x250],R13
18005846b:  48 B8 C5 4E EC C4 4E EC C4 4E             MOV RAX,0x4ec4ec4ec4ec4ec5
180058475:  48 F7 E1                                  MUL RCX
180058478:  4C 8B EA                                  MOV R13,RDX
18005847b:  49 C1 ED 05                               SHR R13,0x5
18005847f:  49 8D 45 FF                               LEA RAX,[R13 + -0x1]
180058483:  48 83 F8 7F                               CMP RAX,0x7f
180058487:  0F 87 78 04 00 00                         JA 0x180058905
18005848d:  48 89 BC 24 60 02 00 00                   MOV qword ptr [RSP + 0x260],RDI
180058495:  4C 8D 0D C4 5A 04 00                      LEA R9,[0x18009df60]
18005849c:  4C 89 A4 24 58 02 00 00                   MOV qword ptr [RSP + 0x258],R12
1800584a4:  4C 8D 44 24 50                            LEA R8,[RSP + 0x50]
1800584a9:  4C 89 BC 24 40 02 00 00                   MOV qword ptr [RSP + 0x240],R15
1800584b1:  48 8D 8C 24 80 00 00 00                   LEA RCX,[RSP + 0x80]
1800584b9:  49 C7 C7 FF FF FF FF                      MOV R15,-0x1
1800584c0:  48 89 B4 24 80 02 00 00                   MOV qword ptr [RSP + 0x280],RSI
1800584c8:  45 33 E4                                  XOR R12D,R12D
1800584cb:  4C 89 7C 24 58                            MOV qword ptr [RSP + 0x58],R15
1800584d0:  BA A4 01 00 00                            MOV EDX,0x1a4
1800584d5:  4C 89 B4 24 48 02 00 00                   MOV qword ptr [RSP + 0x248],R14
1800584dd:  C7 05 5D F6 22 00 01 00 00 00             MOV dword ptr [0x180287b44],0x1
1800584e7:  44 89 64 24 50                            MOV dword ptr [RSP + 0x50],R12D
1800584ec:  E8 DF 97 FA FF                            CALL 0x180001cd0
1800584f1:  33 FF                                     XOR EDI,EDI
1800584f3:  4D 85 ED                                  TEST R13,R13
1800584f6:  0F 84 A5 00 00 00                         JZ 0x1800585a1
1800584fc:  0F 1F 40 00                               NOP dword ptr [RAX]
180058500:  48 8B 05 79 C8 06 00                      MOV RAX,qword ptr [0x1800c4d80]
180058507:  BA 18 00 00 00                            MOV EDX,0x18
18005850c:  48 6B F7 68                               IMUL RSI,RDI,0x68
180058510:  4C 8B B0 18 15 9E 00                      MOV R14,qword ptr [RAX + 0x9e1518]
; 106 instruction(s)
