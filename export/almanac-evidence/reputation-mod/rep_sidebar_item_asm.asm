; Range: 0x180068A20 to 0x180068BD9  (containing fn: FUN_180068a20@0x180068A20)
180068a20:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
180068a25:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
180068a2a:  57                                        PUSH RDI
180068a2b:  48 83 EC 40                               SUB RSP,0x40
180068a2f:  48 8B FA                                  MOV RDI,RDX
180068a32:  48 8B F1                                  MOV RSI,RCX
180068a35:  FF 15 95 F3 11 00                         CALL qword ptr [0x180187dd0]
180068a3b:  48 8B D8                                  MOV RBX,RAX
180068a3e:  48 89 44 24 60                            MOV qword ptr [RSP + 0x60],RAX
180068a43:  48 85 C0                                  TEST RAX,RAX
180068a46:  74 1C                                     JZ 0x180068a64
180068a48:  48 8B D7                                  MOV RDX,RDI
180068a4b:  48 8B CE                                  MOV RCX,RSI
180068a4e:  E8 8D 79 FC FF                            CALL 0x1800303e0
180068a53:  EB 0F                                     JMP 0x180068a64
180068a64:  48 8B C3                                  MOV RAX,RBX
180068a67:  48 8B 5C 24 50                            MOV RBX,qword ptr [RSP + 0x50]
180068a6c:  48 8B 74 24 58                            MOV RSI,qword ptr [RSP + 0x58]
180068a71:  48 83 C4 40                               ADD RSP,0x40
180068a75:  5F                                        POP RDI
180068a76:  C3                                        RET
180068a80:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
180068a85:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
180068a8a:  57                                        PUSH RDI
180068a8b:  48 83 EC 40                               SUB RSP,0x40
180068a8f:  48 8B FA                                  MOV RDI,RDX
180068a92:  48 8B F1                                  MOV RSI,RCX
180068a95:  FF 15 2D F3 11 00                         CALL qword ptr [0x180187dc8]
180068a9b:  48 8B D8                                  MOV RBX,RAX
180068a9e:  48 89 44 24 60                            MOV qword ptr [RSP + 0x60],RAX
180068aa3:  48 85 C0                                  TEST RAX,RAX
180068aa6:  74 1C                                     JZ 0x180068ac4
180068aa8:  48 8B D7                                  MOV RDX,RDI
180068aab:  48 8B CE                                  MOV RCX,RSI
180068aae:  E8 2D 79 FC FF                            CALL 0x1800303e0
180068ab3:  EB 0F                                     JMP 0x180068ac4
180068ac4:  48 8B C3                                  MOV RAX,RBX
180068ac7:  48 8B 5C 24 50                            MOV RBX,qword ptr [RSP + 0x50]
180068acc:  48 8B 74 24 58                            MOV RSI,qword ptr [RSP + 0x58]
180068ad1:  48 83 C4 40                               ADD RSP,0x40
180068ad5:  5F                                        POP RDI
180068ad6:  C3                                        RET
180068ae0:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
180068ae5:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
180068aea:  57                                        PUSH RDI
180068aeb:  48 83 EC 40                               SUB RSP,0x40
180068aef:  49 8B F8                                  MOV RDI,R8
180068af2:  48 8B F2                                  MOV RSI,RDX
180068af5:  FF 15 E5 F2 11 00                         CALL qword ptr [0x180187de0]
180068afb:  8B D8                                     MOV EBX,EAX
180068afd:  89 44 24 68                               MOV dword ptr [RSP + 0x68],EAX
180068b01:  85 C0                                     TEST EAX,EAX
180068b03:  75 1B                                     JNZ 0x180068b20
180068b05:  48 8B D7                                  MOV RDX,RDI
180068b08:  48 8B CE                                  MOV RCX,RSI
180068b0b:  E8 D0 78 FC FF                            CALL 0x1800303e0
180068b10:  EB 0E                                     JMP 0x180068b20
180068b20:  8B C3                                     MOV EAX,EBX
180068b22:  48 8B 5C 24 50                            MOV RBX,qword ptr [RSP + 0x50]
180068b27:  48 8B 74 24 58                            MOV RSI,qword ptr [RSP + 0x58]
180068b2c:  48 83 C4 40                               ADD RSP,0x40
180068b30:  5F                                        POP RDI
180068b31:  C3                                        RET
180068b40:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
180068b45:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
180068b4a:  57                                        PUSH RDI
180068b4b:  48 83 EC 40                               SUB RSP,0x40
180068b4f:  49 8B F8                                  MOV RDI,R8
180068b52:  48 8B F2                                  MOV RSI,RDX
180068b55:  FF 15 7D F2 11 00                         CALL qword ptr [0x180187dd8]
180068b5b:  8B D8                                     MOV EBX,EAX
180068b5d:  89 44 24 68                               MOV dword ptr [RSP + 0x68],EAX
180068b61:  85 C0                                     TEST EAX,EAX
180068b63:  75 1B                                     JNZ 0x180068b80
180068b65:  48 8B D7                                  MOV RDX,RDI
180068b68:  48 8B CE                                  MOV RCX,RSI
180068b6b:  E8 70 78 FC FF                            CALL 0x1800303e0
180068b70:  EB 0E                                     JMP 0x180068b80
180068b80:  8B C3                                     MOV EAX,EBX
180068b82:  48 8B 5C 24 50                            MOV RBX,qword ptr [RSP + 0x50]
180068b87:  48 8B 74 24 58                            MOV RSI,qword ptr [RSP + 0x58]
180068b8c:  48 83 C4 40                               ADD RSP,0x40
180068b90:  5F                                        POP RDI
180068b91:  C3                                        RET
180068ba0:  48 83 EC 28                               SUB RSP,0x28
180068ba4:  85 D2                                     TEST EDX,EDX
180068ba6:  75 29                                     JNZ 0x180068bd1
180068ba8:  33 D2                                     XOR EDX,EDX
180068baa:  8B C2                                     MOV EAX,EDX
180068bac:  89 15 CE B2 05 00                         MOV dword ptr [0x1800c3e80],EDX
180068bb2:  87 05 A4 03 3E 00                         XCHG dword ptr [0x180448f5c],EAX
180068bb8:  48 8B 0D 71 03 3E 00                      MOV RCX,qword ptr [0x180448f30]
180068bbf:  48 89 15 6A 03 3E 00                      MOV qword ptr [0x180448f30],RDX
180068bc6:  48 85 C9                                  TEST RCX,RCX
180068bc9:  74 06                                     JZ 0x180068bd1
180068bcb:  FF 15 17 47 02 00                         CALL qword ptr [0x18008d2e8]
180068bd1:  B8 01 00 00 00                            MOV EAX,0x1
180068bd6:  48 83 C4 28                               ADD RSP,0x28
; 98 instruction(s)
