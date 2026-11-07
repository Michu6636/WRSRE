; Range: 0x1402E6BC0 to 0x1402E6C80  (containing fn: FUN_1402e6bc0@0x1402E6BC0)
1402e6bc0:  48 8B C4                                  MOV RAX,RSP
1402e6bc3:  55                                        PUSH RBP
1402e6bc4:  57                                        PUSH RDI
1402e6bc5:  41 54                                     PUSH R12
1402e6bc7:  41 56                                     PUSH R14
1402e6bc9:  41 57                                     PUSH R15
1402e6bcb:  48 8D 68 C8                               LEA RBP,[RAX + -0x38]
1402e6bcf:  48 81 EC 10 01 00 00                      SUB RSP,0x110
1402e6bd6:  48 C7 44 24 30 FE FF FF FF                MOV qword ptr [RSP + 0x30],-0x2
1402e6bdf:  48 89 58 10                               MOV qword ptr [RAX + 0x10],RBX
1402e6be3:  48 89 70 18                               MOV qword ptr [RAX + 0x18],RSI
1402e6be7:  48 8B 05 5A B4 6A 00                      MOV RAX,qword ptr [0x140992048]
1402e6bee:  48 33 C4                                  XOR RAX,RSP
1402e6bf1:  48 89 45 00                               MOV qword ptr [RBP],RAX
1402e6bf5:  48 8B F9                                  MOV RDI,RCX
1402e6bf8:  48 8D 99 08 C6 00 00                      LEA RBX,[RCX + 0xc608]
1402e6bff:  48 8B 53 08                               MOV RDX,qword ptr [RBX + 0x8]
1402e6c03:  48 8B 0B                                  MOV RCX,qword ptr [RBX]
1402e6c06:  E8 45 13 E2 FF                            CALL 0x140107f50
1402e6c0b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6c0e:  48 89 43 08                               MOV qword ptr [RBX + 0x8],RAX
1402e6c12:  45 33 FF                                  XOR R15D,R15D
1402e6c15:  4C 89 7D D8                               MOV qword ptr [RBP + -0x28],R15
1402e6c19:  0F 57 C0                                  XORPS XMM0,XMM0
1402e6c1c:  66 0F 7F 45 E0                            MOVDQA xmmword ptr [RBP + -0x20],XMM0
1402e6c21:  8B 05 C5 16 59 00                         MOV EAX,dword ptr [0x1408782ec]
1402e6c27:  89 45 91                                  MOV dword ptr [RBP + -0x6f],EAX
1402e6c2a:  0F B7 05 BF 16 59 00                      MOVZX EAX,word ptr [0x1408782f0]
1402e6c31:  66 89 45 95                               MOV word ptr [RBP + -0x6b],AX
1402e6c35:  0F B6 05 B6 16 59 00                      MOVZX EAX,byte ptr [0x1408782f2]
1402e6c3c:  88 45 97                                  MOV byte ptr [RBP + -0x69],AL
1402e6c3f:  C7 45 D4 4C 04 00 00                      MOV dword ptr [RBP + -0x2c],0x44c
1402e6c46:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e6c4a:  49 BE C5 4E EC C4 4E EC C4 4E             MOV R14,0x4ec4ec4ec4ec4ec5
1402e6c54:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e6c58:  73 5E                                     JNC 0x1402e6cb8
1402e6c5a:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6c5d:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e6c61:  48 3B C1                                  CMP RAX,RCX
1402e6c64:  77 52                                     JA 0x1402e6cb8
1402e6c66:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e6c6a:  48 2B C8                                  SUB RCX,RAX
1402e6c6d:  49 8B C6                                  MOV RAX,R14
1402e6c70:  48 F7 E9                                  IMUL RCX
1402e6c73:  48 8B F2                                  MOV RSI,RDX
1402e6c76:  48 C1 FE 05                               SAR RSI,0x5
1402e6c7a:  48 8B C6                                  MOV RAX,RSI
1402e6c7d:  48 C1 E8 3F                               SHR RAX,0x3f
; 48 instruction(s)
