; Range: 0x180060FE0 to 0x180061240  (containing fn: FUN_180060e20@0x180060E20)
180060fe3:  41 FF C0                                  INC R8D
180060fe6:  48 8D 0D 53 BB 03 00                      LEA RCX,[0x18009cb40]
180060fed:  E8 FE 2E FF FF                            CALL 0x180053ef0
180060ff2:  8B 05 20 FB 21 00                         MOV EAX,dword ptr [0x180280b18]
180060ff8:  83 F8 78                                  CMP EAX,0x78
180060ffb:  7C 08                                     JL 0x180061005
180060ffd:  89 1D 09 FB 21 00                         MOV dword ptr [0x180280b0c],EBX
180061003:  EB 07                                     JMP 0x18006100c
180061005:  E8 26 8C FD FF                            CALL 0x180039c30
18006100a:  EB 05                                     JMP 0x180061011
18006100c:  E8 1F 7F FD FF                            CALL 0x180038f30
180061011:  EB 30                                     JMP 0x180061043
180061043:  E8 38 DE FB FF                            CALL 0x18001ee80
180061048:  EB 30                                     JMP 0x18006107a
18006107a:  E8 01 BC FB FF                            CALL 0x18001cc80
18006107f:  EB 3A                                     JMP 0x1800610bb
1800610bb:  E8 E0 CF FB FF                            CALL 0x18001e0a0
1800610c0:  EB 30                                     JMP 0x1800610f2
1800610f2:  E8 59 ED FB FF                            CALL 0x18001fe50
1800610f7:  EB 30                                     JMP 0x180061129
180061129:  E8 82 E0 FB FF                            CALL 0x18001f1b0
18006112e:  EB 30                                     JMP 0x180061160
180061160:  E8 1B C8 FB FF                            CALL 0x18001d980
180061165:  EB 30                                     JMP 0x180061197
180061197:  E8 E4 2A 00 00                            CALL 0x180063c80
18006119c:  EB 4A                                     JMP 0x1800611e8
1800611e8:  E8 03 D0 FA FF                            CALL 0x18000e1f0
1800611ed:  EB 30                                     JMP 0x18006121f
18006121f:  33 C9                                     XOR ECX,ECX
180061221:  E8 3A DE FC FF                            CALL 0x18002f060
180061226:  89 1D 64 FB 21 00                         MOV dword ptr [0x180280d90],EBX
18006122c:  E8 3F 97 FC FF                            CALL 0x18002a970
180061231:  48 81 C4 10 01 00 00                      ADD RSP,0x110
180061238:  5B                                        POP RBX
180061239:  C3                                        RET
180061240:  48 83 EC 38                               SUB RSP,0x38
; 36 instruction(s)
