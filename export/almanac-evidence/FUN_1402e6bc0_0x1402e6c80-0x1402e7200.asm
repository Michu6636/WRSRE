; Range: 0x1402E6C80 to 0x1402E7200  (containing fn: FUN_1402e6bc0@0x1402E6BC0)
1402e6c81:  48 03 F0                                  ADD RSI,RAX
1402e6c84:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e6c88:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e6c8c:  75 08                                     JNZ 0x1402e6c96
1402e6c8e:  48 8B CB                                  MOV RCX,RBX
1402e6c91:  E8 AA 56 16 00                            CALL 0x14044c340
1402e6c96:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6c9a:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e6c9f:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e6ca3:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e6ca6:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e6cab:  48 85 C9                                  TEST RCX,RCX
1402e6cae:  74 06                                     JZ 0x1402e6cb6
1402e6cb0:  E8 7B 28 17 00                            CALL 0x140459530
1402e6cb5:  90                                        NOP
1402e6cb6:  EB 2F                                     JMP 0x1402e6ce7
1402e6cb8:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e6cbc:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e6cc0:  75 08                                     JNZ 0x1402e6cca
1402e6cc2:  48 8B CB                                  MOV RCX,RBX
1402e6cc5:  E8 76 56 16 00                            CALL 0x14044c340
1402e6cca:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6cce:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e6cd3:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e6cd8:  48 85 C9                                  TEST RCX,RCX
1402e6cdb:  74 0A                                     JZ 0x1402e6ce7
1402e6cdd:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e6ce1:  E8 4A 28 17 00                            CALL 0x140459530
1402e6ce6:  90                                        NOP
1402e6ce7:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e6cec:  C7 45 80 A9 02 00 00                      MOV dword ptr [RBP + -0x80],0x2a9
1402e6cf3:  8B 05 CF 0D 5C 00                         MOV EAX,dword ptr [0x1408a7ac8]
1402e6cf9:  89 44 24 40                               MOV dword ptr [RSP + 0x40],EAX
1402e6cfd:  0F B7 05 C8 0D 5C 00                      MOVZX EAX,word ptr [0x1408a7acc]
1402e6d04:  66 89 44 24 44                            MOV word ptr [RSP + 0x44],AX
1402e6d09:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6d0d:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6d10:  49 8B C6                                  MOV RAX,R14
1402e6d13:  48 F7 E9                                  IMUL RCX
1402e6d16:  48 C1 FA 05                               SAR RDX,0x5
1402e6d1a:  48 8B C2                                  MOV RAX,RDX
1402e6d1d:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6d21:  48 03 D0                                  ADD RDX,RAX
1402e6d24:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6d28:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6d2b:  48 83 C0 E0                               ADD RAX,-0x20
1402e6d2f:  48 03 C8                                  ADD RCX,RAX
1402e6d32:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6d37:  E8 B4 25 16 00                            CALL 0x1404492f0
1402e6d3c:  C7 45 80 AB 02 00 00                      MOV dword ptr [RBP + -0x80],0x2ab
1402e6d43:  F2 0F 10 05 BD 0E 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c08]
1402e6d4b:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e6d51:  0F B6 05 B8 0E 5C 00                      MOVZX EAX,byte ptr [0x1408a7c10]
1402e6d58:  88 44 24 48                               MOV byte ptr [RSP + 0x48],AL
1402e6d5c:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6d60:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6d63:  49 8B C6                                  MOV RAX,R14
1402e6d66:  48 F7 E9                                  IMUL RCX
1402e6d69:  48 C1 FA 05                               SAR RDX,0x5
1402e6d6d:  48 8B C2                                  MOV RAX,RDX
1402e6d70:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6d74:  48 03 D0                                  ADD RDX,RAX
1402e6d77:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6d7b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6d7e:  48 83 C0 E0                               ADD RAX,-0x20
1402e6d82:  48 03 C8                                  ADD RCX,RAX
1402e6d85:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6d8a:  E8 61 25 16 00                            CALL 0x1404492f0
1402e6d8f:  C7 45 80 AA 02 00 00                      MOV dword ptr [RBP + -0x80],0x2aa
1402e6d96:  48 B8 67 72 61 70 68 69 63 00             MOV RAX,0x63696870617267
1402e6da0:  48 89 44 24 40                            MOV qword ptr [RSP + 0x40],RAX
1402e6da5:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6da9:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6dac:  49 8B C6                                  MOV RAX,R14
1402e6daf:  48 F7 E9                                  IMUL RCX
1402e6db2:  48 C1 FA 05                               SAR RDX,0x5
1402e6db6:  48 8B C2                                  MOV RAX,RDX
1402e6db9:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6dbd:  48 03 D0                                  ADD RDX,RAX
1402e6dc0:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6dc4:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6dc7:  48 83 C0 E0                               ADD RAX,-0x20
1402e6dcb:  48 03 C8                                  ADD RCX,RAX
1402e6dce:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6dd3:  E8 18 25 16 00                            CALL 0x1404492f0
1402e6dd8:  C7 45 80 AF 02 00 00                      MOV dword ptr [RBP + -0x80],0x2af
1402e6ddf:  F2 0F 10 05 09 0E 5C 00                   MOVSD XMM0,qword ptr [0x1408a7bf0]
1402e6de7:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e6ded:  8B 05 05 0E 5C 00                         MOV EAX,dword ptr [0x1408a7bf8]
1402e6df3:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e6df7:  0F B6 05 FE 0D 5C 00                      MOVZX EAX,byte ptr [0x1408a7bfc]
1402e6dfe:  88 44 24 4C                               MOV byte ptr [RSP + 0x4c],AL
1402e6e02:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6e06:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6e09:  49 8B C6                                  MOV RAX,R14
1402e6e0c:  48 F7 E9                                  IMUL RCX
1402e6e0f:  48 C1 FA 05                               SAR RDX,0x5
1402e6e13:  48 8B C2                                  MOV RAX,RDX
1402e6e16:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6e1a:  48 03 D0                                  ADD RDX,RAX
1402e6e1d:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6e21:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6e24:  48 83 C0 E0                               ADD RAX,-0x20
1402e6e28:  48 03 C8                                  ADD RCX,RAX
1402e6e2b:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6e30:  E8 BB 24 16 00                            CALL 0x1404492f0
1402e6e35:  C7 45 80 EB 3A 00 00                      MOV dword ptr [RBP + -0x80],0x3aeb
1402e6e3c:  0F 10 05 95 0D 5C 00                      MOVUPS XMM0,xmmword ptr [0x1408a7bd8]
1402e6e43:  0F 29 44 24 40                            MOVAPS xmmword ptr [RSP + 0x40],XMM0
1402e6e48:  8B 05 9A 0D 5C 00                         MOV EAX,dword ptr [0x1408a7be8]
1402e6e4e:  89 44 24 50                               MOV dword ptr [RSP + 0x50],EAX
1402e6e52:  0F B7 05 93 0D 5C 00                      MOVZX EAX,word ptr [0x1408a7bec]
1402e6e59:  66 89 44 24 54                            MOV word ptr [RSP + 0x54],AX
1402e6e5e:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6e62:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6e65:  49 8B C6                                  MOV RAX,R14
1402e6e68:  48 F7 E9                                  IMUL RCX
1402e6e6b:  48 C1 FA 05                               SAR RDX,0x5
1402e6e6f:  48 8B C2                                  MOV RAX,RDX
1402e6e72:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6e76:  48 03 D0                                  ADD RDX,RAX
1402e6e79:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6e7d:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6e80:  48 83 C0 E0                               ADD RAX,-0x20
1402e6e84:  48 03 C8                                  ADD RCX,RAX
1402e6e87:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6e8c:  E8 5F 24 16 00                            CALL 0x1404492f0
1402e6e91:  C7 45 80 61 EA 00 00                      MOV dword ptr [RBP + -0x80],0xea61
1402e6e98:  F2 0F 10 05 A8 0D 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c48]
1402e6ea0:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e6ea6:  8B 05 A4 0D 5C 00                         MOV EAX,dword ptr [0x1408a7c50]
1402e6eac:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e6eb0:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6eb4:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6eb7:  49 8B C6                                  MOV RAX,R14
1402e6eba:  48 F7 E9                                  IMUL RCX
1402e6ebd:  48 C1 FA 05                               SAR RDX,0x5
1402e6ec1:  48 8B C2                                  MOV RAX,RDX
1402e6ec4:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6ec8:  48 03 D0                                  ADD RDX,RAX
1402e6ecb:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6ecf:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6ed2:  48 83 C0 E0                               ADD RAX,-0x20
1402e6ed6:  48 03 C8                                  ADD RCX,RAX
1402e6ed9:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6ede:  E8 0D 24 16 00                            CALL 0x1404492f0
1402e6ee3:  C7 45 80 62 EA 00 00                      MOV dword ptr [RBP + -0x80],0xea62
1402e6eea:  F2 0F 10 05 46 0D 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c38]
1402e6ef2:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e6ef8:  8B 05 42 0D 5C 00                         MOV EAX,dword ptr [0x1408a7c40]
1402e6efe:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e6f02:  0F B6 05 3B 0D 5C 00                      MOVZX EAX,byte ptr [0x1408a7c44]
1402e6f09:  88 44 24 4C                               MOV byte ptr [RSP + 0x4c],AL
1402e6f0d:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6f11:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6f14:  49 8B C6                                  MOV RAX,R14
1402e6f17:  48 F7 E9                                  IMUL RCX
1402e6f1a:  48 C1 FA 05                               SAR RDX,0x5
1402e6f1e:  48 8B C2                                  MOV RAX,RDX
1402e6f21:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6f25:  48 03 D0                                  ADD RDX,RAX
1402e6f28:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6f2c:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6f2f:  48 83 C0 E0                               ADD RAX,-0x20
1402e6f33:  48 03 C8                                  ADD RCX,RAX
1402e6f36:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6f3b:  E8 B0 23 16 00                            CALL 0x1404492f0
1402e6f40:  C7 45 80 63 EA 00 00                      MOV dword ptr [RBP + -0x80],0xea63
1402e6f47:  F2 0F 10 05 D9 0C 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c28]
1402e6f4f:  F2 0F 11 44 24 40                         MOVSD qword ptr [RSP + 0x40],XMM0
1402e6f55:  8B 05 D5 0C 5C 00                         MOV EAX,dword ptr [0x1408a7c30]
1402e6f5b:  89 44 24 48                               MOV dword ptr [RSP + 0x48],EAX
1402e6f5f:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e6f63:  48 2B 0B                                  SUB RCX,qword ptr [RBX]
1402e6f66:  49 8B C6                                  MOV RAX,R14
1402e6f69:  48 F7 E9                                  IMUL RCX
1402e6f6c:  48 C1 FA 05                               SAR RDX,0x5
1402e6f70:  48 8B C2                                  MOV RAX,RDX
1402e6f73:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6f77:  48 03 D0                                  ADD RDX,RAX
1402e6f7a:  48 6B CA 68                               IMUL RCX,RDX,0x68
1402e6f7e:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6f81:  48 83 C0 E0                               ADD RAX,-0x20
1402e6f85:  48 03 C8                                  ADD RCX,RAX
1402e6f88:  48 8D 54 24 40                            LEA RDX,[RSP + 0x40]
1402e6f8d:  E8 5E 23 16 00                            CALL 0x1404492f0
1402e6f92:  F2 0F 10 05 7E 0C 5C 00                   MOVSD XMM0,qword ptr [0x1408a7c18]
1402e6f9a:  F2 0F 11 45 91                            MOVSD qword ptr [RBP + -0x6f],XMM0
1402e6f9f:  0F B7 05 7A 0C 5C 00                      MOVZX EAX,word ptr [0x1408a7c20]
1402e6fa6:  66 89 45 99                               MOV word ptr [RBP + -0x67],AX
1402e6faa:  0F B6 05 71 0C 5C 00                      MOVZX EAX,byte ptr [0x1408a7c22]
1402e6fb1:  88 45 9B                                  MOV byte ptr [RBP + -0x65],AL
1402e6fb4:  C7 45 D4 C8 AF 00 00                      MOV dword ptr [RBP + -0x2c],0xafc8
1402e6fbb:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e6fbf:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e6fc3:  73 5E                                     JNC 0x1402e7023
1402e6fc5:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e6fc8:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e6fcc:  48 3B C1                                  CMP RAX,RCX
1402e6fcf:  77 52                                     JA 0x1402e7023
1402e6fd1:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e6fd5:  48 2B C8                                  SUB RCX,RAX
1402e6fd8:  49 8B C6                                  MOV RAX,R14
1402e6fdb:  48 F7 E9                                  IMUL RCX
1402e6fde:  48 8B F2                                  MOV RSI,RDX
1402e6fe1:  48 C1 FE 05                               SAR RSI,0x5
1402e6fe5:  48 8B C6                                  MOV RAX,RSI
1402e6fe8:  48 C1 E8 3F                               SHR RAX,0x3f
1402e6fec:  48 03 F0                                  ADD RSI,RAX
1402e6fef:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e6ff3:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e6ff7:  75 08                                     JNZ 0x1402e7001
1402e6ff9:  48 8B CB                                  MOV RCX,RBX
1402e6ffc:  E8 3F 53 16 00                            CALL 0x14044c340
1402e7001:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7005:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e700a:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e700e:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e7011:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7016:  48 85 C9                                  TEST RCX,RCX
1402e7019:  74 06                                     JZ 0x1402e7021
1402e701b:  E8 10 25 17 00                            CALL 0x140459530
1402e7020:  90                                        NOP
1402e7021:  EB 2F                                     JMP 0x1402e7052
1402e7023:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7027:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e702b:  75 08                                     JNZ 0x1402e7035
1402e702d:  48 8B CB                                  MOV RCX,RBX
1402e7030:  E8 0B 53 16 00                            CALL 0x14044c340
1402e7035:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e7039:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e703e:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e7043:  48 85 C9                                  TEST RCX,RCX
1402e7046:  74 0A                                     JZ 0x1402e7052
1402e7048:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e704c:  E8 DF 24 17 00                            CALL 0x140459530
1402e7051:  90                                        NOP
1402e7052:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e7057:  48 B8 73 74 6F 72 61 67 65 00             MOV RAX,0x656761726f7473
1402e7061:  48 89 45 91                               MOV qword ptr [RBP + -0x6f],RAX
1402e7065:  C7 45 D4 14 05 00 00                      MOV dword ptr [RBP + -0x2c],0x514
1402e706c:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e7070:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e7074:  73 5E                                     JNC 0x1402e70d4
1402e7076:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e7079:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e707d:  48 3B C1                                  CMP RAX,RCX
1402e7080:  77 52                                     JA 0x1402e70d4
1402e7082:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7086:  48 2B C8                                  SUB RCX,RAX
1402e7089:  49 8B C6                                  MOV RAX,R14
1402e708c:  48 F7 E9                                  IMUL RCX
1402e708f:  48 8B F2                                  MOV RSI,RDX
1402e7092:  48 C1 FE 05                               SAR RSI,0x5
1402e7096:  48 8B C6                                  MOV RAX,RSI
1402e7099:  48 C1 E8 3F                               SHR RAX,0x3f
1402e709d:  48 03 F0                                  ADD RSI,RAX
1402e70a0:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e70a4:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e70a8:  75 08                                     JNZ 0x1402e70b2
1402e70aa:  48 8B CB                                  MOV RCX,RBX
1402e70ad:  E8 8E 52 16 00                            CALL 0x14044c340
1402e70b2:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e70b6:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e70bb:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e70bf:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e70c2:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e70c7:  48 85 C9                                  TEST RCX,RCX
1402e70ca:  74 06                                     JZ 0x1402e70d2
1402e70cc:  E8 5F 24 17 00                            CALL 0x140459530
1402e70d1:  90                                        NOP
1402e70d2:  EB 2F                                     JMP 0x1402e7103
1402e70d4:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e70d8:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e70dc:  75 08                                     JNZ 0x1402e70e6
1402e70de:  48 8B CB                                  MOV RCX,RBX
1402e70e1:  E8 5A 52 16 00                            CALL 0x14044c340
1402e70e6:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e70ea:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e70ef:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e70f4:  48 85 C9                                  TEST RCX,RCX
1402e70f7:  74 0A                                     JZ 0x1402e7103
1402e70f9:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e70fd:  E8 2E 24 17 00                            CALL 0x140459530
1402e7102:  90                                        NOP
1402e7103:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e7108:  F2 0F 10 05 70 0A 5C 00                   MOVSD XMM0,qword ptr [0x1408a7b80]
1402e7110:  F2 0F 11 45 91                            MOVSD qword ptr [RBP + -0x6f],XMM0
1402e7115:  0F B7 05 6C 0A 5C 00                      MOVZX EAX,word ptr [0x1408a7b88]
1402e711c:  66 89 45 99                               MOV word ptr [RBP + -0x67],AX
1402e7120:  0F B6 05 63 0A 5C 00                      MOVZX EAX,byte ptr [0x1408a7b8a]
1402e7127:  88 45 9B                                  MOV byte ptr [RBP + -0x65],AL
1402e712a:  C7 45 D4 B0 04 00 00                      MOV dword ptr [RBP + -0x2c],0x4b0
1402e7131:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
1402e7135:  48 3B 43 08                               CMP RAX,qword ptr [RBX + 0x8]
1402e7139:  73 5E                                     JNC 0x1402e7199
1402e713b:  48 8B 03                                  MOV RAX,qword ptr [RBX]
1402e713e:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e7142:  48 3B C1                                  CMP RAX,RCX
1402e7145:  77 52                                     JA 0x1402e7199
1402e7147:  48 8D 4D 90                               LEA RCX,[RBP + -0x70]
1402e714b:  48 2B C8                                  SUB RCX,RAX
1402e714e:  49 8B C6                                  MOV RAX,R14
1402e7151:  48 F7 E9                                  IMUL RCX
1402e7154:  48 8B F2                                  MOV RSI,RDX
1402e7157:  48 C1 FE 05                               SAR RSI,0x5
1402e715b:  48 8B C6                                  MOV RAX,RSI
1402e715e:  48 C1 E8 3F                               SHR RAX,0x3f
1402e7162:  48 03 F0                                  ADD RSI,RAX
1402e7165:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e7169:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e716d:  75 08                                     JNZ 0x1402e7177
1402e716f:  48 8B CB                                  MOV RCX,RBX
1402e7172:  E8 C9 51 16 00                            CALL 0x14044c340
1402e7177:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e717b:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e7180:  48 6B D6 68                               IMUL RDX,RSI,0x68
1402e7184:  48 03 13                                  ADD RDX,qword ptr [RBX]
1402e7187:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e718c:  48 85 C9                                  TEST RCX,RCX
1402e718f:  74 06                                     JZ 0x1402e7197
1402e7191:  E8 9A 23 17 00                            CALL 0x140459530
1402e7196:  90                                        NOP
1402e7197:  EB 2F                                     JMP 0x1402e71c8
1402e7199:  48 8B 43 10                               MOV RAX,qword ptr [RBX + 0x10]
1402e719d:  48 39 43 08                               CMP qword ptr [RBX + 0x8],RAX
1402e71a1:  75 08                                     JNZ 0x1402e71ab
1402e71a3:  48 8B CB                                  MOV RCX,RBX
1402e71a6:  E8 95 51 16 00                            CALL 0x14044c340
1402e71ab:  48 8B 4B 08                               MOV RCX,qword ptr [RBX + 0x8]
1402e71af:  48 89 4C 24 28                            MOV qword ptr [RSP + 0x28],RCX
1402e71b4:  48 89 4C 24 20                            MOV qword ptr [RSP + 0x20],RCX
1402e71b9:  48 85 C9                                  TEST RCX,RCX
1402e71bc:  74 0A                                     JZ 0x1402e71c8
1402e71be:  48 8D 55 90                               LEA RDX,[RBP + -0x70]
1402e71c2:  E8 69 23 17 00                            CALL 0x140459530
1402e71c7:  90                                        NOP
1402e71c8:  48 83 43 08 68                            ADD qword ptr [RBX + 0x8],0x68
1402e71cd:  F2 0F 10 05 9B 09 5C 00                   MOVSD XMM0,qword ptr [0x1408a7b70]
1402e71d5:  F2 0F 11 45 91                            MOVSD qword ptr [RBP + -0x6f],XMM0
1402e71da:  8B 05 98 09 5C 00                         MOV EAX,dword ptr [0x1408a7b78]
1402e71e0:  89 45 99                                  MOV dword ptr [RBP + -0x67],EAX
1402e71e3:  0F B7 05 92 09 5C 00                      MOVZX EAX,word ptr [0x1408a7b7c]
1402e71ea:  66 89 45 9D                               MOV word ptr [RBP + -0x63],AX
1402e71ee:  0F B6 05 89 09 5C 00                      MOVZX EAX,byte ptr [0x1408a7b7e]
1402e71f5:  88 45 9F                                  MOV byte ptr [RBP + -0x61],AL
1402e71f8:  C7 45 D4 E8 03 00 00                      MOV dword ptr [RBP + -0x2c],0x3e8
1402e71ff:  48 8D 45 90                               LEA RAX,[RBP + -0x70]
; 348 instruction(s)
