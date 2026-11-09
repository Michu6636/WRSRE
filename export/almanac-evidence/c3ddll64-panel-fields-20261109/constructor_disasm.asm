; Range: 0x18002D2C0 to 0x18002D360
18002d2c3:  48 8D 87 C8 01 00 00     LEA RAX,[RDI + 0x1c8]
18002d2ca:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
18002d2cd:  48 8D 8B C8 01 00 00     LEA RCX,[RBX + 0x1c8]
18002d2d4:  0F 11 01                 MOVUPS xmmword ptr [RCX],XMM0
18002d2d7:  0F 10 48 10              MOVUPS XMM1,xmmword ptr [RAX + 0x10]
18002d2db:  0F 11 49 10              MOVUPS xmmword ptr [RCX + 0x10],XMM1
18002d2df:  0F 10 40 20              MOVUPS XMM0,xmmword ptr [RAX + 0x20]
18002d2e3:  0F 11 41 20              MOVUPS xmmword ptr [RCX + 0x20],XMM0
18002d2e7:  0F 10 48 30              MOVUPS XMM1,xmmword ptr [RAX + 0x30]
18002d2eb:  0F 11 49 30              MOVUPS xmmword ptr [RCX + 0x30],XMM1
18002d2ef:  0F 10 40 40              MOVUPS XMM0,xmmword ptr [RAX + 0x40]
18002d2f3:  0F 11 41 40              MOVUPS xmmword ptr [RCX + 0x40],XMM0
18002d2f7:  0F 10 48 50              MOVUPS XMM1,xmmword ptr [RAX + 0x50]
18002d2fb:  0F 11 49 50              MOVUPS xmmword ptr [RCX + 0x50],XMM1
18002d2ff:  0F 10 40 60              MOVUPS XMM0,xmmword ptr [RAX + 0x60]
18002d303:  0F 11 41 60              MOVUPS xmmword ptr [RCX + 0x60],XMM0
18002d307:  0F 10 48 70              MOVUPS XMM1,xmmword ptr [RAX + 0x70]
18002d30b:  0F 11 49 70              MOVUPS xmmword ptr [RCX + 0x70],XMM1
18002d30f:  0F 10 80 80 00 00 00     MOVUPS XMM0,xmmword ptr [RAX + 0x80]
18002d316:  0F 11 81 80 00 00 00     MOVUPS xmmword ptr [RCX + 0x80],XMM0
18002d31d:  0F 10 88 90 00 00 00     MOVUPS XMM1,xmmword ptr [RAX + 0x90]
18002d324:  0F 11 89 90 00 00 00     MOVUPS xmmword ptr [RCX + 0x90],XMM1
18002d32b:  48 8B 80 A0 00 00 00     MOV RAX,qword ptr [RAX + 0xa0]
18002d332:  48 89 81 A0 00 00 00     MOV qword ptr [RCX + 0xa0],RAX
18002d339:  48 8D 87 70 02 00 00     LEA RAX,[RDI + 0x270]
18002d340:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
18002d343:  48 8D 8B 70 02 00 00     LEA RCX,[RBX + 0x270]
18002d34a:  0F 11 01                 MOVUPS xmmword ptr [RCX],XMM0
18002d34d:  0F 10 48 10              MOVUPS XMM1,xmmword ptr [RAX + 0x10]
18002d351:  0F 11 49 10              MOVUPS xmmword ptr [RCX + 0x10],XMM1
18002d355:  0F 10 40 20              MOVUPS XMM0,xmmword ptr [RAX + 0x20]
18002d359:  0F 11 41 20              MOVUPS xmmword ptr [RCX + 0x20],XMM0
18002d35d:  0F 10 48 30              MOVUPS XMM1,xmmword ptr [RAX + 0x30]

