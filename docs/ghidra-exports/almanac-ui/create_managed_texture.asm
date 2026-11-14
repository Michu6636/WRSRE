; Range: 0x1800B0190 to 0x1800B01D0  (containing fn: CreateManagedTexture@0x1800B0190)
1800b0190:  48 89 74 24 18                            MOV qword ptr [RSP + 0x18],RSI
1800b0195:  57                                        PUSH RDI
1800b0196:  48 83 EC 20                               SUB RSP,0x20
1800b019a:  48 8B F9                                  MOV RDI,RCX
1800b019d:  48 8B F2                                  MOV RSI,RDX
1800b01a0:  48 8B 49 08                               MOV RCX,qword ptr [RCX + 0x8]
1800b01a4:  E8 87 15 02 00                            CALL 0x1800d1730
1800b01a9:  48 85 C0                                  TEST RAX,RAX
1800b01ac:  0F 85 A1 00 00 00                         JNZ 0x1800b0253
1800b01b2:  48 8B 07                                  MOV RAX,qword ptr [RDI]
1800b01b5:  83 78 38 02                               CMP dword ptr [RAX + 0x38],0x2
1800b01b9:  0F 85 92 00 00 00                         JNZ 0x1800b0251
1800b01bf:  B9 78 01 00 00                            MOV ECX,0x178
1800b01c4:  48 89 5C 24 38                            MOV qword ptr [RSP + 0x38],RBX
1800b01c9:  E8 6A 6E 0B 00                            CALL 0x180167038
1800b01ce:  48 8B D8                                  MOV RBX,RAX
; 16 instruction(s)
