; Range: 0x140459530 to 0x1404595B0  (containing fn: FUN_140459530@0x140459530)
140459530:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
140459535:  57                                        PUSH RDI
140459536:  48 83 EC 20                               SUB RSP,0x20
14045953a:  0F B6 02                                  MOVZX EAX,byte ptr [RDX]
14045953d:  48 8B DA                                  MOV RBX,RDX
140459540:  88 01                                     MOV byte ptr [RCX],AL
140459542:  48 8B F9                                  MOV RDI,RCX
140459545:  0F 10 42 01                               MOVUPS XMM0,xmmword ptr [RDX + 0x1]
140459549:  0F 11 41 01                               MOVUPS xmmword ptr [RCX + 0x1],XMM0
14045954d:  0F 10 4A 11                               MOVUPS XMM1,xmmword ptr [RDX + 0x11]
140459551:  0F 11 49 11                               MOVUPS xmmword ptr [RCX + 0x11],XMM1
140459555:  0F 10 42 21                               MOVUPS XMM0,xmmword ptr [RDX + 0x21]
140459559:  0F 11 41 21                               MOVUPS xmmword ptr [RCX + 0x21],XMM0
14045955d:  0F 10 4A 31                               MOVUPS XMM1,xmmword ptr [RDX + 0x31]
140459561:  0F 11 49 31                               MOVUPS xmmword ptr [RCX + 0x31],XMM1
140459565:  8B 42 44                                  MOV EAX,dword ptr [RDX + 0x44]
140459568:  48 83 C2 48                               ADD RDX,0x48
14045956c:  89 41 44                                  MOV dword ptr [RCX + 0x44],EAX
14045956f:  48 83 C1 48                               ADD RCX,0x48
140459573:  E8 98 0D 00 00                            CALL 0x14045a310
140459578:  48 8B 43 60                               MOV RAX,qword ptr [RBX + 0x60]
14045957c:  48 8B 5C 24 30                            MOV RBX,qword ptr [RSP + 0x30]
140459581:  48 89 47 60                               MOV qword ptr [RDI + 0x60],RAX
140459585:  48 8B C7                                  MOV RAX,RDI
140459588:  48 83 C4 20                               ADD RSP,0x20
14045958c:  5F                                        POP RDI
14045958d:  C3                                        RET
140459590:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
140459595:  57                                        PUSH RDI
140459596:  48 83 EC 20                               SUB RSP,0x20
14045959a:  48 8B DA                                  MOV RBX,RDX
14045959d:  48 8B F9                                  MOV RDI,RCX
1404595a0:  FF 15 2A 35 41 00                         CALL qword ptr [0x14086cad0]
1404595a6:  48 8B 83 C0 00 00 00                      MOV RAX,qword ptr [RBX + 0xc0]
1404595ad:  48 89 87 C0 00 00 00                      MOV qword ptr [RDI + 0xc0],RAX
; 35 instruction(s)
