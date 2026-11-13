; Range: 0x180068F30 to 0x180069C40  (containing fn: TsmPluginStart@0x180068F30)
180068f30:  48 83 EC 48                               SUB RSP,0x48
180068f34:  83 3D A5 01 64 00 00                      CMP dword ptr [0x1806a90e0],0x0
180068f3b:  74 18                                     JZ 0x180068f55
180068f3d:  48 8B 05 2C BE 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180068f44:  48 8D 0D 95 58 04 00                      LEA RCX,[0x1800ae7e0]
180068f4b:  FF 50 38                                  CALL qword ptr [RAX + 0x38]
180068f4e:  33 C0                                     XOR EAX,EAX
180068f50:  48 83 C4 48                               ADD RSP,0x48
180068f54:  C3                                        RET
180068f55:  48 89 5C 24 58                            MOV qword ptr [RSP + 0x58],RBX
180068f5a:  48 8D 0D AF 58 04 00                      LEA RCX,[0x1800ae810]
180068f61:  48 89 6C 24 60                            MOV qword ptr [RSP + 0x60],RBP
180068f66:  48 89 74 24 68                            MOV qword ptr [RSP + 0x68],RSI
180068f6b:  48 89 7C 24 40                            MOV qword ptr [RSP + 0x40],RDI
180068f70:  C7 05 66 01 64 00 01 00 00 00             MOV dword ptr [0x1806a90e0],0x1
180068f7a:  E8 91 7C FF FF                            CALL 0x180060c10
180068f7f:  83 3D 02 A1 05 00 00                      CMP dword ptr [0x1800c3088],0x0
180068f86:  4C 8D 05 C3 58 04 00                      LEA R8,[0x1800ae850]
180068f8d:  48 89 05 4C A2 05 00                      MOV qword ptr [0x1800c31e0],RAX
180068f94:  48 8D 15 D1 8F 03 00                      LEA RDX,[0x1800a1f6c]
180068f9b:  48 8D 05 7E 58 04 00                      LEA RAX,[0x1800ae820]
180068fa2:  4C 0F 45 C0                               CMOVNZ R8,RAX
180068fa6:  48 8D 0D D3 58 04 00                      LEA RCX,[0x1800ae880]
180068fad:  E8 3E AF FE FF                            CALL 0x180053ef0
180068fb2:  45 33 C0                                  XOR R8D,R8D
180068fb5:  48 8D 15 3C 4B 04 00                      LEA RDX,[0x1800adaf8]
180068fbc:  48 8D 0D 71 31 04 00                      LEA RCX,[0x1800ac134]
180068fc3:  E8 98 3C FA FF                            CALL 0x18000cc60
180068fc8:  48 8B F8                                  MOV RDI,RAX
180068fcb:  85 C0                                     TEST EAX,EAX
180068fcd:  0F 84 96 00 00 00                         JZ 0x180069069
180068fd3:  48 8B 1D A6 BD 05 00                      MOV RBX,qword ptr [0x1800c4d80]
180068fda:  BA 40 00 00 00                            MOV EDX,0x40
180068fdf:  48 8B 05 8A BD 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180068fe6:  48 8B CB                                  MOV RCX,RBX
180068fe9:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180068fec:  85 C0                                     TEST EAX,EAX
180068fee:  74 79                                     JZ 0x180069069
180068ff0:  B8 4D 5A 00 00                            MOV EAX,0x5a4d
180068ff5:  66 39 03                                  CMP word ptr [RBX],AX
180068ff8:  75 6F                                     JNZ 0x180069069
180068ffa:  48 63 5B 3C                               MOVSXD RBX,dword ptr [RBX + 0x3c]
180068ffe:  BA 08 01 00 00                            MOV EDX,0x108
180069003:  48 03 1D 76 BD 05 00                      ADD RBX,qword ptr [0x1800c4d80]
18006900a:  48 8B 05 5F BD 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069011:  48 8B CB                                  MOV RCX,RBX
180069014:  FF 50 60                                  CALL qword ptr [RAX + 0x60]
180069017:  85 C0                                     TEST EAX,EAX
180069019:  74 4E                                     JZ 0x180069069
18006901b:  81 3B 50 45 00 00                         CMP dword ptr [RBX],0x4550
180069021:  75 46                                     JNZ 0x180069069
180069023:  8B 5B 08                                  MOV EBX,dword ptr [RBX + 0x8]
180069026:  8B D3                                     MOV EDX,EBX
180069028:  3B DF                                     CMP EBX,EDI
18006902a:  75 18                                     JNZ 0x180069044
18006902c:  48 8D 0D D5 4A 04 00                      LEA RCX,[0x1800adb08]
180069033:  C7 05 E7 7A 21 00 01 00 00 00             MOV dword ptr [0x180280b24],0x1
18006903d:  E8 AE AE FE FF                            CALL 0x180053ef0
180069042:  EB 25                                     JMP 0x180069069
180069044:  48 8B 05 25 BD 05 00                      MOV RAX,qword ptr [0x1800c4d70]
18006904b:  48 8D 0D EE 4A 04 00                      LEA RCX,[0x1800adb40]
180069052:  44 8B C7                                  MOV R8D,EDI
180069055:  FF 50 38                                  CALL qword ptr [RAX + 0x38]
180069058:  44 8B C7                                  MOV R8D,EDI
18006905b:  48 8D 0D 26 4B 04 00                      LEA RCX,[0x1800adb88]
180069062:  8B D3                                     MOV EDX,EBX
180069064:  E8 87 AE FE FF                            CALL 0x180053ef0
180069069:  8B 3D 51 85 13 00                         MOV EDI,dword ptr [0x1801a15c0]
18006906f:  33 ED                                     XOR EBP,EBP
180069071:  8B 35 4D 85 13 00                         MOV ESI,dword ptr [0x1801a15c4]
180069077:  4C 89 74 24 38                            MOV qword ptr [RSP + 0x38],R14
18006907c:  89 2D 3E 85 13 00                         MOV dword ptr [0x1801a15c0],EBP
180069082:  89 2D 3C 85 13 00                         MOV dword ptr [0x1801a15c4],EBP
180069088:  85 FF                                     TEST EDI,EDI
18006908a:  75 04                                     JNZ 0x180069090
18006908c:  85 F6                                     TEST ESI,ESI
18006908e:  74 5F                                     JZ 0x1800690ef
180069090:  8B D7                                     MOV EDX,EDI
180069092:  48 8D 0D C7 4D 02 00                      LEA RCX,[0x18008de60]
180069099:  E8 52 AE FE FF                            CALL 0x180053ef0
18006909e:  8B DD                                     MOV EBX,EBP
1800690a0:  85 FF                                     TEST EDI,EDI
1800690a2:  7E 27                                     JLE 0x1800690cb
1800690a4:  4C 8D 35 15 85 12 00                      LEA R14,[0x1801915c0]
1800690ab:  0F 1F 44 00 00                            NOP dword ptr [RAX + RAX*0x1]
1800690b0:  8B D3                                     MOV EDX,EBX
1800690b2:  48 8D 0D 63 4E 02 00                      LEA RCX,[0x18008df1c]
1800690b9:  48 C1 E2 09                               SHL RDX,0x9
1800690bd:  49 03 D6                                  ADD RDX,R14
1800690c0:  E8 2B AE FE FF                            CALL 0x180053ef0
1800690c5:  FF C3                                     INC EBX
1800690c7:  3B DF                                     CMP EBX,EDI
1800690c9:  7C E5                                     JL 0x1800690b0
1800690cb:  85 F6                                     TEST ESI,ESI
1800690cd:  74 14                                     JZ 0x1800690e3
1800690cf:  41 B8 80 00 00 00                         MOV R8D,0x80
1800690d5:  48 8D 0D 44 4E 02 00                      LEA RCX,[0x18008df20]
1800690dc:  8B D6                                     MOV EDX,ESI
1800690de:  E8 0D AE FE FF                            CALL 0x180053ef0
1800690e3:  48 8D 0D 7E 4E 02 00                      LEA RCX,[0x18008df68]
1800690ea:  E8 01 AE FE FF                            CALL 0x180053ef0
1800690ef:  39 2D 83 58 16 00                         CMP dword ptr [0x1801ce978],EBP
1800690f5:  48 8D 35 04 6F F9 FF                      LEA RSI,[0x180000000]
1800690fc:  4C 89 7C 24 30                            MOV qword ptr [RSP + 0x30],R15
180069101:  48 8D 3D 27 4D 02 00                      LEA RDI,[0x18008de2f]
180069108:  0F 84 1D 01 00 00                         JZ 0x18006922b
18006910e:  8B DD                                     MOV EBX,EBP
180069110:  4C 8D 35 C1 4B 04 00                      LEA R14,[0x1800adcd8]
180069117:  39 1D 83 27 10 00                         CMP dword ptr [0x18016b8a0],EBX
18006911d:  0F 8E 8D 00 00 00                         JLE 0x1800691b0
180069123:  4C 8D 3D 96 4B 04 00                      LEA R15,[0x1800adcc0]
18006912a:  66 0F 1F 44 00 00                         NOP word ptr [RAX + RAX*0x1]
180069130:  48 63 C3                                  MOVSXD RAX,EBX
180069133:  4C 8D 8E A0 94 0D 00                      LEA R9,[RSI + 0xd94a0]
18006913a:  BA 01 00 00 00                            MOV EDX,0x1
18006913f:  48 8B CF                                  MOV RCX,RDI
180069142:  4C 8D 04 40                               LEA R8,[RAX + RAX*0x2]
180069146:  49 C1 E0 09                               SHL R8,0x9
18006914a:  42 8B 84 06 1C 96 0D 00                   MOV EAX,dword ptr [RSI + R8*0x1 + 0xd961c]
180069152:  83 F8 02                                  CMP EAX,0x2
180069155:  F2 42 0F 10 94 06 28 96 0D 00             MOVSD XMM2,qword ptr [RSI + R8*0x1 + 0xd9628]
18006915f:  0F 46 D0                                  CMOVBE EDX,EAX
180069162:  42 39 AC 06 24 96 0D 00                   CMP dword ptr [RSI + R8*0x1 + 0xd9624],EBP
18006916a:  48 63 D2                                  MOVSXD RDX,EDX
18006916d:  48 8B C7                                  MOV RAX,RDI
180069170:  49 0F 45 CF                               CMOVNZ RCX,R15
180069174:  42 39 AC 06 20 96 0D 00                   CMP dword ptr [RSI + R8*0x1 + 0xd9620],EBP
18006917c:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
180069181:  48 8D 0D 68 4B 04 00                      LEA RCX,[0x1800adcf0]
180069188:  49 0F 45 C6                               CMOVNZ RAX,R14
18006918c:  4D 03 C8                                  ADD R9,R8
18006918f:  48 8B 94 D6 D8 D4 08 00                   MOV RDX,qword ptr [RSI + RDX*0x8 + 0x8d4d8]
180069197:  66 49 0F 7E D0                            MOVQ R8,XMM2
18006919c:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800691a1:  E8 4A AD FE FF                            CALL 0x180053ef0
1800691a6:  FF C3                                     INC EBX
1800691a8:  3B 1D F2 26 10 00                         CMP EBX,dword ptr [0x18016b8a0]
1800691ae:  7C 80                                     JL 0x180069130
1800691b0:  8B DD                                     MOV EBX,EBP
1800691b2:  39 1D EC 26 10 00                         CMP dword ptr [0x18016b8a4],EBX
1800691b8:  7E 71                                     JLE 0x18006922b
1800691ba:  66 0F 1F 44 00 00                         NOP word ptr [RAX + RAX*0x1]
1800691c0:  48 63 C3                                  MOVSXD RAX,EBX
1800691c3:  4C 8D 8E B0 B8 16 00                      LEA R9,[RSI + 0x16b8b0]
1800691ca:  4C 69 C0 20 02 00 00                      IMUL R8,RAX,0x220
1800691d1:  B9 01 00 00 00                            MOV ECX,0x1
1800691d6:  42 8B 84 06 C0 BA 16 00                   MOV EAX,dword ptr [RSI + R8*0x1 + 0x16bac0]
1800691de:  83 F8 02                                  CMP EAX,0x2
1800691e1:  F2 42 0F 10 94 06 C8 BA 16 00             MOVSD XMM2,qword ptr [RSI + R8*0x1 + 0x16bac8]
1800691eb:  0F 46 C8                                  CMOVBE ECX,EAX
1800691ee:  42 39 AC 06 C4 BA 16 00                   CMP dword ptr [RSI + R8*0x1 + 0x16bac4],EBP
1800691f6:  48 63 D1                                  MOVSXD RDX,ECX
1800691f9:  48 8B C7                                  MOV RAX,RDI
1800691fc:  49 0F 45 C6                               CMOVNZ RAX,R14
180069200:  48 8D 0D 11 4B 04 00                      LEA RCX,[0x1800add18]
180069207:  4D 03 C8                                  ADD R9,R8
18006920a:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
18006920f:  66 49 0F 7E D0                            MOVQ R8,XMM2
180069214:  48 8B 94 D6 D8 D4 08 00                   MOV RDX,qword ptr [RSI + RDX*0x8 + 0x8d4d8]
18006921c:  E8 CF AC FE FF                            CALL 0x180053ef0
180069221:  FF C3                                     INC EBX
180069223:  3B 1D 7B 26 10 00                         CMP EBX,dword ptr [0x18016b8a4]
180069229:  7C 95                                     JL 0x1800691c0
18006922b:  39 2D FF A0 05 00                         CMP dword ptr [0x1800c3330],EBP
180069231:  75 11                                     JNZ 0x180069244
180069233:  48 8D 0D 96 58 02 00                      LEA RCX,[0x18008ead0]
18006923a:  E8 B1 AC FE FF                            CALL 0x180053ef0
18006923f:  E9 23 01 00 00                            JMP 0x180069367
180069244:  48 39 2D 25 B4 14 00                      CMP qword ptr [0x1801b4670],RBP
18006924b:  75 18                                     JNZ 0x180069265
18006924d:  48 8D 15 24 58 02 00                      LEA RDX,[0x18008ea78]
180069254:  48 8D 0D 35 59 02 00                      LEA RCX,[0x18008eb90]
18006925b:  E8 90 AC FE FF                            CALL 0x180053ef0
180069260:  E9 02 01 00 00                            JMP 0x180069367
180069265:  39 2D 85 B4 14 00                         CMP dword ptr [0x1801b46f0],EBP
18006926b:  74 18                                     JZ 0x180069285
18006926d:  48 8D 15 04 58 02 00                      LEA RDX,[0x18008ea78]
180069274:  48 8D 0D 75 59 02 00                      LEA RCX,[0x18008ebf0]
18006927b:  E8 70 AC FE FF                            CALL 0x180053ef0
180069280:  E9 E2 00 00 00                            JMP 0x180069367
180069285:  8B 15 61 B4 14 00                         MOV EDX,dword ptr [0x1801b46ec]
18006928b:  85 D2                                     TEST EDX,EDX
18006928d:  75 3C                                     JNZ 0x1800692cb
18006928f:  48 39 2D 02 02 07 00                      CMP qword ptr [0x1800d9498],RBP
180069296:  48 8D 05 B3 59 02 00                      LEA RAX,[0x18008ec50]
18006929d:  44 8B 05 50 B4 14 00                      MOV R8D,dword ptr [0x1801b46f4]
1800692a4:  48 8D 15 CD 57 02 00                      LEA RDX,[0x18008ea78]
1800692ab:  48 0F 45 C7                               CMOVNZ RAX,RDI
1800692af:  48 8D 0D 0A 5A 02 00                      LEA RCX,[0x18008ecc0]
1800692b6:  41 B9 FF 0F 00 00                         MOV R9D,0xfff
1800692bc:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800692c1:  E8 2A AC FE FF                            CALL 0x180053ef0
1800692c6:  E9 9C 00 00 00                            JMP 0x180069367
1800692cb:  48 39 2D C6 01 07 00                      CMP qword ptr [0x1800d9498],RBP
1800692d2:  75 12                                     JNZ 0x1800692e6
1800692d4:  48 8D 0D 25 5B 02 00                      LEA RCX,[0x18008ee00]
1800692db:  E8 10 AC FE FF                            CALL 0x180053ef0
1800692e0:  8B 15 06 B4 14 00                         MOV EDX,dword ptr [0x1801b46ec]
1800692e6:  8D 42 FF                                  LEA EAX,[RDX + -0x1]
1800692e9:  85 D2                                     TEST EDX,EDX
1800692eb:  7F 02                                     JG 0x1800692ef
1800692ed:  8B C5                                     MOV EAX,EBP
1800692ef:  44 8B 05 82 B3 14 00                      MOV R8D,dword ptr [0x1801b4678]
1800692f6:  48 8D 0D B3 5B 02 00                      LEA RCX,[0x18008eeb0]
1800692fd:  4C 63 C8                                  MOVSXD R9,EAX
180069300:  46 8B 8C 8E 78 46 1B 00                   MOV R9D,dword ptr [RSI + R9*0x4 + 0x1b4678]
180069308:  E8 E3 AB FE FF                            CALL 0x180053ef0
18006930d:  8B 05 65 B3 14 00                         MOV EAX,dword ptr [0x1801b4678]
180069313:  85 C0                                     TEST EAX,EAX
180069315:  75 23                                     JNZ 0x18006933a
180069317:  81 3D BB AC 13 00 7A 18 00 00             CMP dword ptr [0x1801a3fdc],0x187a
180069321:  75 17                                     JNZ 0x18006933a
180069323:  BA 7A 18 00 00                            MOV EDX,0x187a
180069328:  48 8D 0D 11 5C 02 00                      LEA RCX,[0x18008ef40]
18006932f:  E8 BC AB FE FF                            CALL 0x180053ef0
180069334:  8B 05 3E B3 14 00                         MOV EAX,dword ptr [0x1801b4678]
18006933a:  44 8B 05 C7 9F 05 00                      MOV R8D,dword ptr [0x1800c3308]
180069341:  48 8D 0D A8 5D 02 00                      LEA RCX,[0x18008f0f0]
180069348:  8B 15 8E AC 13 00                         MOV EDX,dword ptr [0x1801a3fdc]
18006934e:  4C 8D 0D 2B 5E 02 00                      LEA R9,[0x18008f180]
180069355:  85 C0                                     TEST EAX,EAX
180069357:  4C 0F 45 C9                               CMOVNZ R9,RCX
18006935b:  48 8D 0D 4E 5E 02 00                      LEA RCX,[0x18008f1b0]
180069362:  E8 89 AB FE FF                            CALL 0x180053ef0
180069367:  48 8B 05 02 BA 05 00                      MOV RAX,qword ptr [0x1800c4d70]
18006936e:  4C 8D 05 2B 55 04 00                      LEA R8,[0x1800ae8a0]
180069375:  48 8B 0D FC B9 05 00                      MOV RCX,qword ptr [0x1800c4d78]
18006937c:  48 8D 15 D5 55 02 00                      LEA RDX,[0x18008e958]
180069383:  FF 50 40                                  CALL qword ptr [RAX + 0x40]
180069386:  48 85 C0                                  TEST RAX,RAX
180069389:  74 13                                     JZ 0x18006939e
18006938b:  48 8B 00                                  MOV RAX,qword ptr [RAX]
18006938e:  48 8D 0D 53 55 04 00                      LEA RCX,[0x1800ae8e8]
180069395:  48 89 05 1C 00 07 00                      MOV qword ptr [0x1800d93b8],RAX
18006939c:  EB 07                                     JMP 0x1800693a5
18006939e:  48 8D 0D 7B 55 04 00                      LEA RCX,[0x1800ae920]
1800693a5:  E8 46 AB FE FF                            CALL 0x180053ef0
1800693aa:  E8 81 F3 FC FF                            CALL 0x180038730
1800693af:  39 2D 37 9F 05 00                         CMP dword ptr [0x1800c32ec],EBP
1800693b5:  0F 84 D5 00 00 00                         JZ 0x180069490
1800693bb:  48 39 2D F6 CC 21 00                      CMP qword ptr [0x1802860b8],RBP
1800693c2:  0F 84 C8 00 00 00                         JZ 0x180069490
1800693c8:  48 8B 0D A9 B9 05 00                      MOV RCX,qword ptr [0x1800c4d78]
1800693cf:  48 8D 05 7A 55 04 00                      LEA RAX,[0x1800ae950]
1800693d6:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800693db:  4C 8D 0D BE EB FF FF                      LEA R9,[0x180067fa0]
1800693e2:  48 8D 05 CF CC 21 00                      LEA RAX,[0x1802860b8]
1800693e9:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800693ee:  4C 8D 05 13 2F 03 00                      LEA R8,[0x18009c308]
1800693f5:  48 8B 05 74 B9 05 00                      MOV RAX,qword ptr [0x1800c4d70]
1800693fc:  48 8D 15 55 55 02 00                      LEA RDX,[0x18008e958]
180069403:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069406:  48 8B 0D 6B B9 05 00                      MOV RCX,qword ptr [0x1800c4d78]
18006940d:  4C 8D 0D 7C EC FF FF                      LEA R9,[0x180068090]
180069414:  8B D8                                     MOV EBX,EAX
180069416:  4C 8D 05 23 2F 03 00                      LEA R8,[0x18009c340]
18006941d:  48 8D 05 4C 55 04 00                      LEA RAX,[0x1800ae970]
180069424:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
180069429:  48 8D 15 28 55 02 00                      LEA RDX,[0x18008e958]
180069430:  48 8D 05 89 CC 21 00                      LEA RAX,[0x1802860c0]
180069437:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
18006943c:  48 8B 05 2D B9 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069443:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069446:  85 DB                                     TEST EBX,EBX
180069448:  74 1D                                     JZ 0x180069467
18006944a:  85 C0                                     TEST EAX,EAX
18006944c:  74 19                                     JZ 0x180069467
18006944e:  BA 78 E4 31 00                            MOV EDX,0x31e478
180069453:  48 8D 0D 36 55 04 00                      LEA RCX,[0x1800ae990]
18006945a:  41 B8 09 E5 31 00                         MOV R8D,0x31e509
180069460:  E8 8B AA FE FF                            CALL 0x180053ef0
180069465:  EB 29                                     JMP 0x180069490
180069467:  85 C0                                     TEST EAX,EAX
180069469:  48 8D 0D E0 A6 02 00                      LEA RCX,[0x180093b50]
180069470:  48 8D 15 C1 55 04 00                      LEA RDX,[0x1800aea38]
180069477:  4C 8B C2                                  MOV R8,RDX
18006947a:  4C 0F 45 C1                               CMOVNZ R8,RCX
18006947e:  85 DB                                     TEST EBX,EBX
180069480:  48 0F 45 D1                               CMOVNZ RDX,RCX
180069484:  48 8D 0D B5 55 04 00                      LEA RCX,[0x1800aea40]
18006948b:  E8 60 AA FE FF                            CALL 0x180053ef0
180069490:  48 8B 0D E1 B8 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069497:  48 8D 05 12 56 04 00                      LEA RAX,[0x1800aeab0]
18006949e:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800694a3:  4C 8D 0D 96 EC FF FF                      LEA R9,[0x180068140]
1800694aa:  48 8D 05 57 CC 21 00                      LEA RAX,[0x180286108]
1800694b1:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800694b6:  4C 8D 05 13 56 04 00                      LEA R8,[0x1800aead0]
1800694bd:  48 8B 05 AC B8 05 00                      MOV RAX,qword ptr [0x1800c4d70]
1800694c4:  48 8D 15 8D 54 02 00                      LEA RDX,[0x18008e958]
1800694cb:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
1800694ce:  85 C0                                     TEST EAX,EAX
1800694d0:  74 0E                                     JZ 0x1800694e0
1800694d2:  48 8D 0D 1F 56 04 00                      LEA RCX,[0x1800aeaf8]
1800694d9:  E8 12 AA FE FF                            CALL 0x180053ef0
1800694de:  EB 1B                                     JMP 0x1800694fb
1800694e0:  48 8D 0D 39 56 04 00                      LEA RCX,[0x1800aeb20]
1800694e7:  E8 04 AA FE FF                            CALL 0x180053ef0
1800694ec:  83 3D 19 76 21 00 01                      CMP dword ptr [0x180280b0c],0x1
1800694f3:  75 06                                     JNZ 0x1800694fb
1800694f5:  89 2D 11 76 21 00                         MOV dword ptr [0x180280b0c],EBP
1800694fb:  48 8B 0D 76 B8 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069502:  48 8D 05 87 56 04 00                      LEA RAX,[0x1800aeb90]
180069509:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
18006950e:  4C 8D 0D DB E6 FF FF                      LEA R9,[0x180067bf0]
180069515:  48 8D 05 F4 CB 21 00                      LEA RAX,[0x180286110]
18006951c:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
180069521:  4C 8D 05 F8 2C 03 00                      LEA R8,[0x18009c220]
180069528:  48 8B 05 41 B8 05 00                      MOV RAX,qword ptr [0x1800c4d70]
18006952f:  48 8D 15 22 54 02 00                      LEA RDX,[0x18008e958]
180069536:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069539:  85 C0                                     TEST EAX,EAX
18006953b:  75 1B                                     JNZ 0x180069558
18006953d:  48 8D 0D 6C 56 04 00                      LEA RCX,[0x1800aebb0]
180069544:  E8 A7 A9 FE FF                            CALL 0x180053ef0
180069549:  83 3D BC 75 21 00 02                      CMP dword ptr [0x180280b0c],0x2
180069550:  75 06                                     JNZ 0x180069558
180069552:  89 2D B4 75 21 00                         MOV dword ptr [0x180280b0c],EBP
180069558:  48 8B 0D 19 B8 05 00                      MOV RCX,qword ptr [0x1800c4d78]
18006955f:  48 8D 05 92 56 04 00                      LEA RAX,[0x1800aebf8]
180069566:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
18006956b:  4C 8D 0D 2E E8 FF FF                      LEA R9,[0x180067da0]
180069572:  48 8D 05 9F CB 21 00                      LEA RAX,[0x180286118]
180069579:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
18006957e:  4C 8D 05 C3 2C 03 00                      LEA R8,[0x18009c248]
180069585:  48 8B 05 E4 B7 05 00                      MOV RAX,qword ptr [0x1800c4d70]
18006958c:  48 8D 15 C5 53 02 00                      LEA RDX,[0x18008e958]
180069593:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069596:  85 C0                                     TEST EAX,EAX
180069598:  75 1B                                     JNZ 0x1800695b5
18006959a:  48 8D 0D 6F 56 04 00                      LEA RCX,[0x1800aec10]
1800695a1:  E8 4A A9 FE FF                            CALL 0x180053ef0
1800695a6:  83 3D 5F 75 21 00 03                      CMP dword ptr [0x180280b0c],0x3
1800695ad:  75 06                                     JNZ 0x1800695b5
1800695af:  89 2D 57 75 21 00                         MOV dword ptr [0x180280b0c],EBP
1800695b5:  48 8B 0D BC B7 05 00                      MOV RCX,qword ptr [0x1800c4d78]
1800695bc:  48 8D 05 8D 56 04 00                      LEA RAX,[0x1800aec50]
1800695c3:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800695c8:  4C 8D 0D 41 E9 FF FF                      LEA R9,[0x180067f10]
1800695cf:  48 8D 05 12 CB 21 00                      LEA RAX,[0x1802860e8]
1800695d6:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800695db:  4C 8D 05 1E 2F 03 00                      LEA R8,[0x18009c500]
1800695e2:  48 8B 05 87 B7 05 00                      MOV RAX,qword ptr [0x1800c4d70]
1800695e9:  48 8D 15 68 53 02 00                      LEA RDX,[0x18008e958]
1800695f0:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
1800695f3:  85 C0                                     TEST EAX,EAX
1800695f5:  75 0C                                     JNZ 0x180069603
1800695f7:  48 8D 0D 72 56 04 00                      LEA RCX,[0x1800aec70]
1800695fe:  E8 ED A8 FE FF                            CALL 0x180053ef0
180069603:  48 8B 0D 76 B7 05 00                      MOV RCX,qword ptr [0x1800c4d80]
18006960a:  4C 8D 0D 6F EB FF FF                      LEA R9,[0x180068180]
180069611:  4C 8B 15 58 B7 05 00                      MOV R10,qword ptr [0x1800c4d70]
180069618:  4C 8D 05 D1 56 04 00                      LEA R8,[0x1800aecf0]
18006961f:  48 8D 15 32 53 02 00                      LEA RDX,[0x18008e958]
180069626:  48 8D 81 90 E3 08 00                      LEA RAX,[RCX + 0x8e390]
18006962d:  48 89 05 8C E4 21 00                      MOV qword ptr [0x180287ac0],RAX
180069634:  48 8D 81 80 E3 08 00                      LEA RAX,[RCX + 0x8e380]
18006963b:  48 89 05 86 E4 21 00                      MOV qword ptr [0x180287ac8],RAX
180069642:  48 8D 81 C0 E3 08 00                      LEA RAX,[RCX + 0x8e3c0]
180069649:  48 89 05 80 E4 21 00                      MOV qword ptr [0x180287ad0],RAX
180069650:  48 8D 81 30 94 07 00                      LEA RAX,[RCX + 0x79430]
180069657:  48 89 05 7A E4 21 00                      MOV qword ptr [0x180287ad8],RAX
18006965e:  48 8D 81 A0 1C 05 00                      LEA RAX,[RCX + 0x51ca0]
180069665:  48 89 05 74 E4 21 00                      MOV qword ptr [0x180287ae0],RAX
18006966c:  48 8D 81 90 DF 01 00                      LEA RAX,[RCX + 0x1df90]
180069673:  48 89 05 6E E4 21 00                      MOV qword ptr [0x180287ae8],RAX
18006967a:  48 8D 81 00 95 07 00                      LEA RAX,[RCX + 0x79500]
180069681:  48 89 05 68 E4 21 00                      MOV qword ptr [0x180287af0],RAX
180069688:  48 8D 81 20 95 07 00                      LEA RAX,[RCX + 0x79520]
18006968f:  48 89 05 62 E4 21 00                      MOV qword ptr [0x180287af8],RAX
180069696:  48 8D 81 00 E5 08 00                      LEA RAX,[RCX + 0x8e500]
18006969d:  48 89 05 5C E4 21 00                      MOV qword ptr [0x180287b00],RAX
1800696a4:  48 8D 81 70 E4 08 00                      LEA RAX,[RCX + 0x8e470]
1800696ab:  48 89 05 56 E4 21 00                      MOV qword ptr [0x180287b08],RAX
1800696b2:  48 8D 81 60 E4 08 00                      LEA RAX,[RCX + 0x8e460]
1800696b9:  48 89 05 50 E4 21 00                      MOV qword ptr [0x180287b10],RAX
1800696c0:  48 8D 81 80 1B 05 00                      LEA RAX,[RCX + 0x51b80]
1800696c7:  48 89 05 4A E4 21 00                      MOV qword ptr [0x180287b18],RAX
1800696ce:  48 8D 81 20 DE 01 00                      LEA RAX,[RCX + 0x1de20]
1800696d5:  48 8B 0D 9C B6 05 00                      MOV RCX,qword ptr [0x1800c4d78]
1800696dc:  48 89 05 3D E4 21 00                      MOV qword ptr [0x180287b20],RAX
1800696e3:  48 8D 05 EE 55 04 00                      LEA RAX,[0x1800aecd8]
1800696ea:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800696ef:  48 8D 05 CA E6 11 00                      LEA RAX,[0x180187dc0]
1800696f6:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800696fb:  41 FF 52 48                               CALL qword ptr [R10 + 0x48]
1800696ff:  85 C0                                     TEST EAX,EAX
180069701:  75 27                                     JNZ 0x18006972a
180069703:  48 8B 15 66 B6 05 00                      MOV RDX,qword ptr [0x1800c4d70]
18006970a:  48 8D 0D 17 56 04 00                      LEA RCX,[0x1800aed28]
180069711:  FF 52 38                                  CALL qword ptr [RDX + 0x38]
180069714:  48 8D 0D 4D 56 04 00                      LEA RCX,[0x1800aed68]
18006971b:  E8 D0 A7 FE FF                            CALL 0x180053ef0
180069720:  B8 01 00 00 00                            MOV EAX,0x1
180069725:  E9 E2 02 00 00                            JMP 0x180069a0c
18006972a:  48 8D 0D 7F 56 04 00                      LEA RCX,[0x1800aedb0]
180069731:  C7 05 49 A8 13 00 01 00 00 00             MOV dword ptr [0x1801a3f84],0x1
18006973b:  E8 B0 A7 FE FF                            CALL 0x180053ef0
180069740:  48 8B 05 29 B6 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069747:  4C 8D 05 1A 52 02 00                      LEA R8,[0x18008e968]
18006974e:  48 8B 0D 23 B6 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069755:  48 8D 15 FC 51 02 00                      LEA RDX,[0x18008e958]
18006975c:  FF 50 40                                  CALL qword ptr [RAX + 0x40]
18006975f:  48 85 C0                                  TEST RAX,RAX
180069762:  0F 84 AC 00 00 00                         JZ 0x180069814
180069768:  48 8B 00                                  MOV RAX,qword ptr [RAX]
18006976b:  48 8D 0D A6 56 04 00                      LEA RCX,[0x1800aee18]
180069772:  48 89 05 1F FD 06 00                      MOV qword ptr [0x1800d9498],RAX
180069779:  E8 72 A7 FE FF                            CALL 0x180053ef0
18006977e:  39 2D D8 99 05 00                         CMP dword ptr [0x1800c315c],EBP
180069784:  75 0C                                     JNZ 0x180069792
180069786:  39 2D 60 9B 05 00                         CMP dword ptr [0x1800c32ec],EBP
18006978c:  0F 84 A2 00 00 00                         JZ 0x180069834
180069792:  48 8B 0D DF B5 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069799:  48 8D 05 B0 56 04 00                      LEA RAX,[0x1800aee50]
1800697a0:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800697a5:  4C 8D 0D A4 E4 FF FF                      LEA R9,[0x180067c50]
1800697ac:  48 8D 05 E5 FC 06 00                      LEA RAX,[0x1800d9498]
1800697b3:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800697b8:  4C 8D 05 A9 51 02 00                      LEA R8,[0x18008e968]
1800697bf:  48 8B 05 AA B5 05 00                      MOV RAX,qword ptr [0x1800c4d70]
1800697c6:  48 8D 15 8B 51 02 00                      LEA RDX,[0x18008e958]
1800697cd:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
1800697d0:  85 C0                                     TEST EAX,EAX
1800697d2:  74 2F                                     JZ 0x180069803
1800697d4:  39 2D 82 99 05 00                         CMP dword ptr [0x1800c315c],EBP
1800697da:  74 0C                                     JZ 0x1800697e8
1800697dc:  48 8D 0D 85 56 04 00                      LEA RCX,[0x1800aee68]
1800697e3:  E8 08 A7 FE FF                            CALL 0x180053ef0
1800697e8:  39 2D FE 9A 05 00                         CMP dword ptr [0x1800c32ec],EBP
1800697ee:  74 44                                     JZ 0x180069834
1800697f0:  BA 3C F0 31 00                            MOV EDX,0x31f03c
1800697f5:  48 8D 0D B4 56 04 00                      LEA RCX,[0x1800aeeb0]
1800697fc:  E8 EF A6 FE FF                            CALL 0x180053ef0
180069801:  EB 31                                     JMP 0x180069834
180069803:  39 2D E3 9A 05 00                         CMP dword ptr [0x1800c32ec],EBP
180069809:  74 29                                     JZ 0x180069834
18006980b:  48 8D 0D FE 56 04 00                      LEA RCX,[0x1800aef10]
180069812:  EB 1B                                     JMP 0x18006982f
180069814:  48 8D 0D 15 57 04 00                      LEA RCX,[0x1800aef30]
18006981b:  E8 D0 A6 FE FF                            CALL 0x180053ef0
180069820:  39 2D C6 9A 05 00                         CMP dword ptr [0x1800c32ec],EBP
180069826:  74 0C                                     JZ 0x180069834
180069828:  48 8D 0D 41 57 04 00                      LEA RCX,[0x1800aef70]
18006982f:  E8 FC 41 FC FF                            CALL 0x18002da30
180069834:  39 2D 56 98 05 00                         CMP dword ptr [0x1800c3090],EBP
18006983a:  0F 84 33 01 00 00                         JZ 0x180069973
180069840:  48 8B 0D 31 B5 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069847:  4C 8D 35 4A 57 04 00                      LEA R14,[0x1800aef98]
18006984e:  48 8D 05 73 E5 11 00                      LEA RAX,[0x180187dc8]
180069855:  4C 89 74 24 28                            MOV qword ptr [RSP + 0x28],R14
18006985a:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
18006985f:  4C 8D 0D 1A F2 FF FF                      LEA R9,[0x180068a80]
180069866:  48 8B 05 03 B5 05 00                      MOV RAX,qword ptr [0x1800c4d70]
18006986d:  48 8D 15 2C 57 04 00                      LEA RDX,[0x1800aefa0]
180069874:  4D 8B C6                                  MOV R8,R14
180069877:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
18006987a:  48 8B 0D F7 B4 05 00                      MOV RCX,qword ptr [0x1800c4d78]
180069881:  4C 8D 3D 38 57 04 00                      LEA R15,[0x1800aefc0]
180069888:  85 C0                                     TEST EAX,EAX
18006988a:  4C 89 7C 24 28                            MOV qword ptr [RSP + 0x28],R15
18006988f:  8B F0                                     MOV ESI,EAX
180069891:  4C 8D 0D A8 F2 FF FF                      LEA R9,[0x180068b40]
180069898:  48 8D 05 39 E5 11 00                      LEA RAX,[0x180187dd8]
18006989f:  8B DD                                     MOV EBX,EBP
1800698a1:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800698a6:  48 8D 15 F3 56 04 00                      LEA RDX,[0x1800aefa0]
1800698ad:  48 8B 05 BC B4 05 00                      MOV RAX,qword ptr [0x1800c4d70]
1800698b4:  4D 8B C7                                  MOV R8,R15
1800698b7:  0F 95 C3                                  SETNZ BL
1800698ba:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
1800698bd:  85 C0                                     TEST EAX,EAX
1800698bf:  74 09                                     JZ 0x1800698ca
1800698c1:  85 F6                                     TEST ESI,ESI
1800698c3:  8B DD                                     MOV EBX,EBP
1800698c5:  0F 95 C3                                  SETNZ BL
1800698c8:  FF C3                                     INC EBX
1800698ca:  48 8B 0D BF B4 05 00                      MOV RCX,qword ptr [0x1800c4d90]
1800698d1:  48 85 C9                                  TEST RCX,RCX
1800698d4:  74 79                                     JZ 0x18006994f
1800698d6:  48 8D 05 EB 56 04 00                      LEA RAX,[0x1800aefc8]
1800698dd:  4D 8B C6                                  MOV R8,R14
1800698e0:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
1800698e5:  4C 8D 0D 34 F1 FF FF                      LEA R9,[0x180068a20]
1800698ec:  48 8D 05 DD E4 11 00                      LEA RAX,[0x180187dd0]
1800698f3:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
1800698f8:  48 8D 15 A1 56 04 00                      LEA RDX,[0x1800aefa0]
1800698ff:  48 8B 05 6A B4 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069906:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069909:  48 8B 0D 80 B4 05 00                      MOV RCX,qword ptr [0x1800c4d90]
180069910:  4C 8D 0D C9 F1 FF FF                      LEA R9,[0x180068ae0]
180069917:  83 F8 01                                  CMP EAX,0x1
18006991a:  48 8D 15 7F 56 04 00                      LEA RDX,[0x1800aefa0]
180069921:  48 8D 05 B0 56 04 00                      LEA RAX,[0x1800aefd8]
180069928:  4D 8B C7                                  MOV R8,R15
18006992b:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
180069930:  83 DB FF                                  SBB EBX,-0x1
180069933:  48 8D 05 A6 E4 11 00                      LEA RAX,[0x180187de0]
18006993a:  48 89 44 24 20                            MOV qword ptr [RSP + 0x20],RAX
18006993f:  48 8B 05 2A B4 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069946:  FF 50 48                                  CALL qword ptr [RAX + 0x48]
180069949:  83 F8 01                                  CMP EAX,0x1
18006994c:  83 DB FF                                  SBB EBX,-0x1
18006994f:  8B D3                                     MOV EDX,EBX
180069951:  48 8D 0D 90 56 04 00                      LEA RCX,[0x1800aefe8]
180069958:  E8 93 A5 FE FF                            CALL 0x180053ef0
18006995d:  85 DB                                     TEST EBX,EBX
18006995f:  75 12                                     JNZ 0x180069973
180069961:  48 8D 0D A8 56 04 00                      LEA RCX,[0x1800af010]
180069968:  89 2D 22 97 05 00                         MOV dword ptr [0x1800c3090],EBP
18006996e:  E8 7D A5 FE FF                            CALL 0x180053ef0
180069973:  39 2D 0F 97 05 00                         CMP dword ptr [0x1800c3088],EBP
180069979:  48 8D 05 C0 56 04 00                      LEA RAX,[0x1800af040]
180069980:  48 8D 15 E5 85 03 00                      LEA RDX,[0x1800a1f6c]
180069987:  48 0F 45 F8                               CMOVNZ RDI,RAX
18006998b:  48 8D 0D BE 56 04 00                      LEA RCX,[0x1800af050]
180069992:  48 8B 05 D7 B3 05 00                      MOV RAX,qword ptr [0x1800c4d70]
180069999:  4C 8B C7                                  MOV R8,RDI
18006999c:  FF 50 38                                  CALL qword ptr [RAX + 0x38]
18006999f:  48 8D 4C 24 50                            LEA RCX,[RSP + 0x50]
1800699a4:  FF 15 FE 36 02 00                         CALL qword ptr [0x18008d0a8]
1800699aa:  85 C0                                     TEST EAX,EAX
1800699ac:  74 1A                                     JZ 0x1800699c8
1800699ae:  48 8B 44 24 50                            MOV RAX,qword ptr [RSP + 0x50]
1800699b3:  48 85 C0                                  TEST RAX,RAX
1800699b6:  74 10                                     JZ 0x1800699c8
1800699b8:  0F 57 C0                                  XORPS XMM0,XMM0
1800699bb:  F2 48 0F 2A C0                            CVTSI2SD XMM0,RAX
1800699c0:  F2 0F 11 05 50 F5 3D 00                   MOVSD qword ptr [0x180448f18],XMM0
1800699c8:  39 2D F6 99 05 00                         CMP dword ptr [0x1800c33c4],EBP
1800699ce:  75 0E                                     JNZ 0x1800699de
1800699d0:  48 8D 0D 99 56 04 00                      LEA RCX,[0x1800af070]
1800699d7:  E8 14 A5 FE FF                            CALL 0x180053ef0
1800699dc:  EB 20                                     JMP 0x1800699fe
1800699de:  44 8B 0D EB 99 05 00                      MOV R9D,dword ptr [0x1800c33d0]
1800699e5:  48 8D 0D 04 57 04 00                      LEA RCX,[0x1800af0f0]
1800699ec:  44 8B 05 35 40 16 00                      MOV R8D,dword ptr [0x1801cda28]
1800699f3:  8B 15 D3 99 05 00                         MOV EDX,dword ptr [0x1800c33cc]
1800699f9:  E8 F2 A4 FE FF                            CALL 0x180053ef0
1800699fe:  48 8D 0D BB 57 04 00                      LEA RCX,[0x1800af1c0]
180069a05:  E8 E6 A4 FE FF                            CALL 0x180053ef0
180069a0a:  33 C0                                     XOR EAX,EAX
180069a0c:  4C 8B 7C 24 30                            MOV R15,qword ptr [RSP + 0x30]
180069a11:  4C 8B 74 24 38                            MOV R14,qword ptr [RSP + 0x38]
180069a16:  48 8B 74 24 68                            MOV RSI,qword ptr [RSP + 0x68]
180069a1b:  48 8B 6C 24 60                            MOV RBP,qword ptr [RSP + 0x60]
180069a20:  48 8B 5C 24 58                            MOV RBX,qword ptr [RSP + 0x58]
180069a25:  48 8B 7C 24 40                            MOV RDI,qword ptr [RSP + 0x40]
180069a2a:  48 83 C4 48                               ADD RSP,0x48
180069a2e:  C3                                        RET
180069a30:  48 8D 05 A1 E5 3F 00                      LEA RAX,[0x180467fd8]
180069a37:  C3                                        RET
180069a40:  48 8D 05 99 E5 3F 00                      LEA RAX,[0x180467fe0]
180069a47:  C3                                        RET
180069a50:  4C 89 4C 24 20                            MOV qword ptr [RSP + 0x20],R9
180069a55:  53                                        PUSH RBX
180069a56:  55                                        PUSH RBP
180069a57:  56                                        PUSH RSI
180069a58:  57                                        PUSH RDI
180069a59:  41 56                                     PUSH R14
180069a5b:  48 83 EC 40                               SUB RSP,0x40
180069a5f:  49 8B E9                                  MOV RBP,R9
180069a62:  4C 8D B4 24 90 00 00 00                   LEA R14,[RSP + 0x90]
180069a6a:  49 8B D8                                  MOV RBX,R8
180069a6d:  48 8B FA                                  MOV RDI,RDX
180069a70:  48 8B F1                                  MOV RSI,RCX
180069a73:  E8 B8 FF FF FF                            CALL 0x180069a30
180069a78:  4C 89 74 24 30                            MOV qword ptr [RSP + 0x30],R14
180069a7d:  4C 8B CB                                  MOV R9,RBX
180069a80:  48 C7 44 24 28 00 00 00 00                MOV qword ptr [RSP + 0x28],0x0
180069a89:  4C 8B C7                                  MOV R8,RDI
180069a8c:  48 8B D6                                  MOV RDX,RSI
180069a8f:  48 89 6C 24 20                            MOV qword ptr [RSP + 0x20],RBP
180069a94:  48 8B 08                                  MOV RCX,qword ptr [RAX]
180069a97:  E8 7C 75 00 00                            CALL 0x180071018
180069a9c:  85 C0                                     TEST EAX,EAX
180069a9e:  B9 FF FF FF FF                            MOV ECX,0xffffffff
180069aa3:  0F 48 C1                                  CMOVS EAX,ECX
180069aa6:  48 83 C4 40                               ADD RSP,0x40
180069aaa:  41 5E                                     POP R14
180069aac:  5F                                        POP RDI
180069aad:  5E                                        POP RSI
180069aae:  5D                                        POP RBP
180069aaf:  5B                                        POP RBX
180069ab0:  C3                                        RET
180069ac0:  40 53                                     PUSH RBX
180069ac2:  55                                        PUSH RBP
180069ac3:  56                                        PUSH RSI
180069ac4:  57                                        PUSH RDI
180069ac5:  48 83 EC 48                               SUB RSP,0x48
180069ac9:  49 8B D9                                  MOV RBX,R9
180069acc:  49 8B F8                                  MOV RDI,R8
180069acf:  48 8B F2                                  MOV RSI,RDX
180069ad2:  48 8B E9                                  MOV RBP,RCX
180069ad5:  E8 56 FF FF FF                            CALL 0x180069a30
180069ada:  4C 8B 94 24 90 00 00 00                   MOV R10,qword ptr [RSP + 0x90]
180069ae2:  4C 8B CF                                  MOV R9,RDI
180069ae5:  4C 89 54 24 30                            MOV qword ptr [RSP + 0x30],R10
180069aea:  4C 8B C6                                  MOV R8,RSI
180069aed:  48 C7 44 24 28 00 00 00 00                MOV qword ptr [RSP + 0x28],0x0
180069af6:  48 8B D5                                  MOV RDX,RBP
180069af9:  48 8B 08                                  MOV RCX,qword ptr [RAX]
180069afc:  48 89 5C 24 20                            MOV qword ptr [RSP + 0x20],RBX
180069b01:  E8 12 75 00 00                            CALL 0x180071018
180069b06:  85 C0                                     TEST EAX,EAX
180069b08:  B9 FF FF FF FF                            MOV ECX,0xffffffff
180069b0d:  0F 48 C1                                  CMOVS EAX,ECX
180069b10:  48 83 C4 48                               ADD RSP,0x48
180069b14:  5F                                        POP RDI
180069b15:  5E                                        POP RSI
180069b16:  5D                                        POP RBP
180069b17:  5B                                        POP RBX
180069b18:  C3                                        RET
180069b20:  48 89 54 24 10                            MOV qword ptr [RSP + 0x10],RDX
180069b25:  4C 89 44 24 18                            MOV qword ptr [RSP + 0x18],R8
180069b2a:  4C 89 4C 24 20                            MOV qword ptr [RSP + 0x20],R9
180069b2f:  53                                        PUSH RBX
180069b30:  56                                        PUSH RSI
180069b31:  57                                        PUSH RDI
180069b32:  48 83 EC 30                               SUB RSP,0x30
180069b36:  48 8B FA                                  MOV RDI,RDX
180069b39:  48 8D 74 24 60                            LEA RSI,[RSP + 0x60]
180069b3e:  48 8B D9                                  MOV RBX,RCX
180069b41:  E8 FA FE FF FF                            CALL 0x180069a40
180069b46:  48 89 74 24 28                            MOV qword ptr [RSP + 0x28],RSI
180069b4b:  4C 8B CF                                  MOV R9,RDI
180069b4e:  49 C7 C0 FF FF FF FF                      MOV R8,-0x1
180069b55:  48 C7 44 24 20 00 00 00 00                MOV qword ptr [RSP + 0x20],0x0
180069b5e:  48 8B D3                                  MOV RDX,RBX
180069b61:  48 8B 08                                  MOV RCX,qword ptr [RAX]
180069b64:  48 83 C9 01                               OR RCX,0x1
180069b68:  E8 F3 C7 00 00                            CALL 0x180076360
180069b6d:  48 83 C4 30                               ADD RSP,0x30
180069b71:  5F                                        POP RDI
180069b72:  5E                                        POP RSI
180069b73:  5B                                        POP RBX
180069b74:  C3                                        RET
180069b80:  B9 02 00 00 00                            MOV ECX,0x2
180069b85:  CD 29                                     INT 0x29
180069b87:  C3                                        RET
180069b90:  B9 08 00 00 00                            MOV ECX,0x8
180069b95:  CD 29                                     INT 0x29
180069b97:  C3                                        RET
180069ba0:  48 83 EC 28                               SUB RSP,0x28
180069ba4:  4D 8B 41 38                               MOV R8,qword ptr [R9 + 0x38]
180069ba8:  48 8B CA                                  MOV RCX,RDX
180069bab:  49 8B D1                                  MOV RDX,R9
180069bae:  E8 0D 00 00 00                            CALL 0x180069bc0
180069bb3:  B8 01 00 00 00                            MOV EAX,0x1
180069bb8:  48 83 C4 28                               ADD RSP,0x28
180069bbc:  C3                                        RET
180069bc0:  48 89 5C 24 10                            MOV qword ptr [RSP + 0x10],RBX
180069bc5:  48 89 6C 24 18                            MOV qword ptr [RSP + 0x18],RBP
180069bca:  56                                        PUSH RSI
180069bcb:  48 83 EC 20                               SUB RSP,0x20
180069bcf:  49 63 28                                  MOVSXD RBP,dword ptr [R8]
180069bd2:  48 8B F1                                  MOV RSI,RCX
180069bd5:  40 F6 C5 04                               TEST BPL,0x4
180069bd9:  74 15                                     JZ 0x180069bf0
180069bdb:  41 8B 40 08                               MOV EAX,dword ptr [R8 + 0x8]
180069bdf:  49 63 58 04                               MOVSXD RBX,dword ptr [R8 + 0x4]
180069be3:  F7 D8                                     NEG EAX
180069be5:  4C 63 C0                                  MOVSXD R8,EAX
180069be8:  48 03 D9                                  ADD RBX,RCX
180069beb:  49 23 D8                                  AND RBX,R8
180069bee:  EB 03                                     JMP 0x180069bf3
180069bf0:  48 8B DE                                  MOV RBX,RSI
180069bf3:  48 8B 42 10                               MOV RAX,qword ptr [RDX + 0x10]
180069bf7:  48 8B 52 08                               MOV RDX,qword ptr [RDX + 0x8]
180069bfb:  48 89 7C 24 30                            MOV qword ptr [RSP + 0x30],RDI
180069c00:  8B 48 08                                  MOV ECX,dword ptr [RAX + 0x8]
180069c03:  48 03 D1                                  ADD RDX,RCX
180069c06:  44 0F B6 02                               MOVZX R8D,byte ptr [RDX]
180069c0a:  0F B6 4A 03                               MOVZX ECX,byte ptr [RDX + 0x3]
180069c0e:  41 0F B6 C0                               MOVZX EAX,R8B
180069c12:  24 07                                     AND AL,0x7
180069c14:  3C 03                                     CMP AL,0x3
180069c16:  73 0D                                     JNC 0x180069c25
180069c18:  F6 C1 0F                                  TEST CL,0xf
180069c1b:  74 12                                     JZ 0x180069c2f
180069c1d:  44 8B C9                                  MOV R9D,ECX
180069c20:  E9 0C 01 00 00                            JMP 0x180069d31
180069c25:  8B F9                                     MOV EDI,ECX
180069c27:  44 8B D9                                  MOV R11D,ECX
180069c2a:  83 E7 1F                                  AND EDI,0x1f
180069c2d:  75 08                                     JNZ 0x180069c37
180069c2f:  45 33 C9                                  XOR R9D,R9D
180069c32:  E9 FE 00 00 00                            JMP 0x180069d35
180069c37:  48 83 C2 04                               ADD RDX,0x4
180069c3b:  8B C7                                     MOV EAX,EDI
180069c3d:  41 F6 C0 40                               TEST R8B,0x40
; 691 instruction(s)
