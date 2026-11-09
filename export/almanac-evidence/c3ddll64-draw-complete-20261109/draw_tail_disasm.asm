; Range: 0x1800BB1D0 to 0x1800BB400
1800bb1d1:  F3 44 0F 11 B7 58 02 00 00 MOVSS dword ptr [RDI + 0x258],XMM14
1800bb1da:  F3 44 0F 11 AF 5C 02 00 00 MOVSS dword ptr [RDI + 0x25c],XMM13
1800bb1e3:  8B 8F 28 67 00 00        MOV ECX,dword ptr [RDI + 0x6728]
1800bb1e9:  85 C9                    TEST ECX,ECX
1800bb1eb:  0F 84 81 00 00 00        JZ 0x1800bb272
1800bb1f1:  8B 87 2C 67 00 00        MOV EAX,dword ptr [RDI + 0x672c]
1800bb1f7:  85 C0                    TEST EAX,EAX
1800bb1f9:  74 77                    JZ 0x1800bb272
1800bb1fb:  F3 0F 10 8F A8 02 00 00  MOVSS XMM1,dword ptr [RDI + 0x2a8]
1800bb203:  0F 57 C0                 XORPS XMM0,XMM0
1800bb206:  0F 2E C8                 UCOMISS XMM1,XMM0
1800bb209:  7A 67                    JP 0x1800bb272
1800bb20b:  75 65                    JNZ 0x1800bb272
1800bb20d:  F3 0F 10 1D 67 DA 0C 00  MOVSS XMM3,dword ptr [0x180188c7c]
1800bb215:  66 0F 6E C1              MOVD XMM0,ECX
1800bb219:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1800bb21c:  F3 0F 58 C3              ADDSS XMM0,XMM3
1800bb220:  0F 2F D0                 COMISS XMM2,XMM0
1800bb223:  0F 87 97 04 00 00        JA 0x1800bb6c0
1800bb229:  F3 0F 10 97 4C 67 00 00  MOVSS XMM2,dword ptr [RDI + 0x674c]
1800bb231:  66 0F 6E C0              MOVD XMM0,EAX
1800bb235:  0F 28 CA                 MOVAPS XMM1,XMM2
1800bb238:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1800bb23b:  F3 0F 58 CC              ADDSS XMM1,XMM4
1800bb23f:  F3 0F 58 C3              ADDSS XMM0,XMM3
1800bb243:  0F 2F C8                 COMISS XMM1,XMM0
1800bb246:  0F 87 74 04 00 00        JA 0x1800bb6c0
1800bb24c:  F3 0F 10 05 7C DB 0C 00  MOVSS XMM0,dword ptr [0x180188dd0]
1800bb254:  0F 2F 87 D0 02 00 00     COMISS XMM0,dword ptr [RDI + 0x2d0]
1800bb25b:  0F 87 5F 04 00 00        JA 0x1800bb6c0
1800bb261:  F3 0F 58 97 D4 02 00 00  ADDSS XMM2,dword ptr [RDI + 0x2d4]
1800bb269:  0F 2F C2                 COMISS XMM0,XMM2
1800bb26c:  0F 87 4E 04 00 00        JA 0x1800bb6c0
1800bb272:  F3 0F 10 3D 6E D9 0C 00  MOVSS XMM7,dword ptr [0x180188be8]
1800bb27a:  BB 04 00 00 00           MOV EBX,0x4
1800bb27f:  C7 87 04 02 00 00 00 00 80 3F MOV dword ptr [RDI + 0x204],0x3f800000
1800bb289:  C7 87 14 02 00 00 00 00 80 3F MOV dword ptr [RDI + 0x214],0x3f800000
1800bb293:  C7 87 24 02 00 00 00 00 80 3F MOV dword ptr [RDI + 0x224],0x3f800000
1800bb29d:  C7 87 34 02 00 00 00 00 80 3F MOV dword ptr [RDI + 0x234],0x3f800000
1800bb2a7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
1800bb2b0:  48 8B 07                 MOV RAX,qword ptr [RDI]
1800bb2b3:  83 CA FF                 OR EDX,0xffffffff
1800bb2b6:  F3 0F 10 76 FC           MOVSS XMM6,dword ptr [RSI + -0x4]
1800bb2bb:  48 8B 08                 MOV RCX,qword ptr [RAX]
1800bb2be:  48 8B 01                 MOV RAX,qword ptr [RCX]
1800bb2c1:  FF 50 48                 CALL qword ptr [RAX + 0x48]
1800bb2c4:  8B C0                    MOV EAX,EAX
1800bb2c6:  0F 57 C0                 XORPS XMM0,XMM0
1800bb2c9:  83 CA FF                 OR EDX,0xffffffff
1800bb2cc:  F3 48 0F 2A C0           CVTSI2SS XMM0,RAX
1800bb2d1:  F3 0F 5E F0              DIVSS XMM6,XMM0
1800bb2d5:  F3 0F 58 F6              ADDSS XMM6,XMM6
1800bb2d9:  F3 0F 5C F7              SUBSS XMM6,XMM7
1800bb2dd:  F3 0F 11 76 FC           MOVSS dword ptr [RSI + -0x4],XMM6
1800bb2e2:  48 8B 07                 MOV RAX,qword ptr [RDI]
1800bb2e5:  F3 0F 10 36              MOVSS XMM6,dword ptr [RSI]
1800bb2e9:  48 8B 08                 MOV RCX,qword ptr [RAX]
1800bb2ec:  48 8B 01                 MOV RAX,qword ptr [RCX]
1800bb2ef:  FF 50 50                 CALL qword ptr [RAX + 0x50]
1800bb2f2:  0F 57 C0                 XORPS XMM0,XMM0
1800bb2f5:  8B C0                    MOV EAX,EAX
1800bb2f7:  F3 48 0F 2A C0           CVTSI2SS XMM0,RAX
1800bb2fc:  F3 0F 5E F0              DIVSS XMM6,XMM0
1800bb300:  0F 28 C7                 MOVAPS XMM0,XMM7
1800bb303:  F3 0F 58 F6              ADDSS XMM6,XMM6
1800bb307:  F3 0F 5C C6              SUBSS XMM0,XMM6
1800bb30b:  F3 0F 11 06              MOVSS dword ptr [RSI],XMM0
1800bb30f:  48 83 C6 10              ADD RSI,0x10
1800bb313:  48 83 EB 01              SUB RBX,0x1
1800bb317:  75 97                    JNZ 0x1800bb2b0
1800bb319:  44 38 AF 0D 03 00 00     CMP byte ptr [RDI + 0x30d],R13B
1800bb320:  0F 84 14 02 00 00        JZ 0x1800bb53a
1800bb326:  48 8B 07                 MOV RAX,qword ptr [RDI]
1800bb329:  48 63 8F 08 03 00 00     MOVSXD RCX,dword ptr [RDI + 0x308]
1800bb330:  48 8B 40 38              MOV RAX,qword ptr [RAX + 0x38]
1800bb334:  48 8B 80 90 00 00 00     MOV RAX,qword ptr [RAX + 0x90]
1800bb33b:  48 89 84 CF 10 03 00 00  MOV qword ptr [RDI + RCX*0x8 + 0x310],RAX
1800bb343:  0F 10 87 AC 02 00 00     MOVUPS XMM0,xmmword ptr [RDI + 0x2ac]
1800bb34a:  8B 87 08 03 00 00        MOV EAX,dword ptr [RDI + 0x308]
1800bb350:  F3 0F 10 97 B0 02 00 00  MOVSS XMM2,dword ptr [RDI + 0x2b0]
1800bb358:  F3 0F 10 9F B4 02 00 00  MOVSS XMM3,dword ptr [RDI + 0x2b4]
1800bb360:  F3 0F 10 A7 B8 02 00 00  MOVSS XMM4,dword ptr [RDI + 0x2b8]
1800bb368:  C1 E0 02                 SHL EAX,0x2
1800bb36b:  48 63 C8                 MOVSXD RCX,EAX
1800bb36e:  48 83 C1 71              ADD RCX,0x71
1800bb372:  F3 0F 11 44 24 30        MOVSS dword ptr [RSP + 0x30],XMM0
1800bb378:  0F 10 87 F8 01 00 00     MOVUPS XMM0,xmmword ptr [RDI + 0x1f8]
1800bb37f:  48 03 C9                 ADD RCX,RCX
1800bb382:  F3 0F 11 54 24 34        MOVSS dword ptr [RSP + 0x34],XMM2
1800bb388:  F3 0F 11 5C 24 38        MOVSS dword ptr [RSP + 0x38],XMM3
1800bb38e:  F3 0F 11 64 24 3C        MOVSS dword ptr [RSP + 0x3c],XMM4
1800bb394:  0F 11 04 CF              MOVUPS xmmword ptr [RDI + RCX*0x8],XMM0
1800bb398:  8B 87 08 03 00 00        MOV EAX,dword ptr [RDI + 0x308]
1800bb39e:  0F 10 87 38 02 00 00     MOVUPS XMM0,xmmword ptr [RDI + 0x238]
1800bb3a5:  C1 E0 02                 SHL EAX,0x2
1800bb3a8:  0F 10 4C 24 30           MOVUPS XMM1,xmmword ptr [RSP + 0x30]
1800bb3ad:  48 63 C8                 MOVSXD RCX,EAX
1800bb3b0:  48 81 C1 71 02 00 00     ADD RCX,0x271
1800bb3b7:  48 03 C9                 ADD RCX,RCX
1800bb3ba:  0F 11 04 CF              MOVUPS xmmword ptr [RDI + RCX*0x8],XMM0
1800bb3be:  8B 87 08 03 00 00        MOV EAX,dword ptr [RDI + 0x308]
1800bb3c4:  C1 E0 02                 SHL EAX,0x2
1800bb3c7:  48 63 C8                 MOVSXD RCX,EAX
1800bb3ca:  48 81 C1 71 04 00 00     ADD RCX,0x471
1800bb3d1:  48 03 C9                 ADD RCX,RCX
1800bb3d4:  0F 11 0C CF              MOVUPS xmmword ptr [RDI + RCX*0x8],XMM1
1800bb3d8:  8B 87 08 03 00 00        MOV EAX,dword ptr [RDI + 0x308]
1800bb3de:  0F 10 87 08 02 00 00     MOVUPS XMM0,xmmword ptr [RDI + 0x208]
1800bb3e5:  8D 04 85 01 00 00 00     LEA EAX,[0x1 + RAX*0x4]
1800bb3ec:  48 63 C8                 MOVSXD RCX,EAX
1800bb3ef:  48 83 C1 71              ADD RCX,0x71
1800bb3f3:  48 03 C9                 ADD RCX,RCX
1800bb3f6:  0F 11 04 CF              MOVUPS xmmword ptr [RDI + RCX*0x8],XMM0
1800bb3fa:  8B 87 08 03 00 00        MOV EAX,dword ptr [RDI + 0x308]
1800bb400:  0F 10 87 48 02 00 00     MOVUPS XMM0,xmmword ptr [RDI + 0x248]

