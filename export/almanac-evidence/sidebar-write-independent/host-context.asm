=== 0x14031ac6e to 0x14031ace0 from export/140317db0_full_disasm.asm ===
14031ac6e:  49 83 BE 50 C6 00 00 00  CMP qword ptr [R14 + 0xc650],0x0
14031ac76:  74 1A                    JZ 0x14031ac92
14031ac78:  49 8B 8E 50 C6 00 00     MOV RCX,qword ptr [R14 + 0xc650]
14031ac7f:  48 FF C1                 INC RCX
14031ac82:  48 8D 15 47 6F 59 00     LEA RDX,[0x1408b1bd0]
14031ac89:  E8 92 48 53 00           CALL 0x14084f520
14031ac8e:  85 C0                    TEST EAX,EAX
14031ac90:  74 55                    JZ 0x14031ace7
14031ac92:  49 8D 8E 08 C6 00 00     LEA RCX,[R14 + 0xc608]
14031ac99:  E8 22 65 D9 FF           CALL 0x1400b11c0
14031ac9e:  48 8B D8                 MOV RBX,RAX
14031aca1:  85 C0                    TEST EAX,EAX
14031aca3:  74 3B                    JZ 0x14031ace0
14031aca5:  49 8B B6 08 C6 00 00     MOV RSI,qword ptr [R14 + 0xc608]
14031acac:  0F 1F 40 00              NOP dword ptr [RAX]
14031acb0:  FF CB                    DEC EBX
14031acb2:  8B CB                    MOV ECX,EBX
14031acb4:  48 6B F9 68              IMUL RDI,RCX,0x68
14031acb8:  48 8D 4E 01              LEA RCX,[RSI + 0x1]
14031acbc:  48 03 CF                 ADD RCX,RDI
14031acbf:  48 8D 15 FA 6E 59 00     LEA RDX,[0x1408b1bc0]
14031acc6:  E8 55 48 53 00           CALL 0x14084f520
14031accb:  85 C0                    TEST EAX,EAX
14031accd:  74 06                    JZ 0x14031acd5
14031accf:  85 DB                    TEST EBX,EBX
14031acd1:  75 DD                    JNZ 0x14031acb0
14031acd3:  EB 0B                    JMP 0x14031ace0
14031acd5:  48 8D 0C 37              LEA RCX,[RDI + RSI*0x1]
14031acd9:  49 89 8E 50 C6 00 00     MOV qword ptr [R14 + 0xc650],RCX
14031ace0:  4D 8D BE B0 AD 00 00     LEA R15,[R14 + 0xadb0]
=== 0x14031f87f to 0x14031f9bc from export/140317db0_full_disasm.asm ===
14031f87f:  48 8D 95 20 04 00 00     LEA RDX,[RBP + 0x420]
14031f886:  48 8D 0D 03 53 73 00     LEA RCX,[0x140a54b90]
14031f88d:  FF 15 D5 CF 54 00        CALL qword ptr [0x14086c868]
14031f893:  48 8B D0                 MOV RDX,RAX
14031f896:  0F 28 DE                 MOVAPS XMM3,XMM6
14031f899:  0F 28 D6                 MOVAPS XMM2,XMM6
14031f89c:  48 8D 0D BD E7 69 00     LEA RCX,[0x1409be060]
14031f8a3:  FF 15 A7 CF 54 00        CALL qword ptr [0x14086c850]
14031f8a9:  84 C0                    TEST AL,AL
14031f8ab:  0F 84 15 07 00 00        JZ 0x14031ffc6
14031f8b1:  41 83 BE 74 AD 00 00 FF  CMP dword ptr [R14 + 0xad74],-0x1
14031f8b9:  0F 85 07 07 00 00        JNZ 0x14031ffc6
14031f8bf:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
14031f8c3:  48 8B 0C F0              MOV RCX,qword ptr [RAX + RSI*0x8]
14031f8c7:  8B 51 44                 MOV EDX,dword ptr [RCX + 0x44]
14031f8ca:  48 8D 0D BF 7C 67 00     LEA RCX,[0x140997590]
14031f8d1:  FF 15 B9 CF 54 00        CALL qword ptr [0x14086c890]
14031f8d7:  49 8B D6                 MOV RDX,R14
14031f8da:  48 2B D0                 SUB RDX,RAX
14031f8dd:  0F 1F 00                 NOP dword ptr [RAX]
14031f8e0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
14031f8e3:  66 89 8C 02 A0 D5 00 00  MOV word ptr [RDX + RAX*0x1 + 0xd5a0],CX
14031f8eb:  48 8D 40 02              LEA RAX,[RAX + 0x2]
14031f8ef:  66 85 C9                 TEST CX,CX
14031f8f2:  75 EC                    JNZ 0x14031f8e0
14031f8f4:  F3 44 0F 11 7C 24 20     MOVSS dword ptr [RSP + 0x20],XMM15
14031f8fb:  41 0F 28 DF              MOVAPS XMM3,XMM15
14031f8ff:  41 0F 28 D7              MOVAPS XMM2,XMM15
14031f903:  41 0F 28 CF              MOVAPS XMM1,XMM15
14031f907:  48 8D 8D 00 0E 00 00     LEA RCX,[RBP + 0xe00]
14031f90e:  FF 15 0C C7 54 00        CALL qword ptr [0x14086c020]
14031f914:  48 8B D0                 MOV RDX,RAX
14031f917:  48 8D 0D 42 E7 69 00     LEA RCX,[0x1409be060]
14031f91e:  FF 15 74 CF 54 00        CALL qword ptr [0x14086c898]
14031f924:  48 8D 0D 65 52 73 00     LEA RCX,[0x140a54b90]
14031f92b:  FF 15 87 CF 54 00        CALL qword ptr [0x14086c8b8]
14031f931:  84 C0                    TEST AL,AL
14031f933:  0F 84 8D 06 00 00        JZ 0x14031ffc6
14031f939:  41 80 BE C3 0F 00 00 00  CMP byte ptr [R14 + 0xfc3],0x0
14031f941:  0F 85 7F 06 00 00        JNZ 0x14031ffc6
14031f947:  49 83 BE 50 C6 00 00 00  CMP qword ptr [R14 + 0xc650],0x0
14031f94f:  74 4B                    JZ 0x14031f99c
14031f951:  49 8B 96 50 C6 00 00     MOV RDX,qword ptr [R14 + 0xc650]
14031f958:  33 C0                    XOR EAX,EAX
14031f95a:  8B C8                    MOV ECX,EAX
14031f95c:  0F 1F 40 00              NOP dword ptr [RAX]
14031f960:  0F B6 44 0A 01           MOVZX EAX,byte ptr [RDX + RCX*0x1 + 0x1]
14031f965:  48 FF C1                 INC RCX
14031f968:  41 3A 44 0C FF           CMP AL,byte ptr [R12 + RCX*0x1 + -0x1]
14031f96d:  75 0E                    JNZ 0x14031f97d
14031f96f:  48 83 F9 07              CMP RCX,0x7
14031f973:  75 EB                    JNZ 0x14031f960
14031f975:  49 8B CE                 MOV RCX,R14
14031f978:  E8 D3 74 01 00           CALL 0x140336e50
14031f97d:  49 8B 8E 50 C6 00 00     MOV RCX,qword ptr [R14 + 0xc650]
14031f984:  48 85 C9                 TEST RCX,RCX
14031f987:  74 13                    JZ 0x14031f99c
14031f989:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
14031f98d:  48 3B 0C F0              CMP RCX,qword ptr [RAX + RSI*0x8]
14031f991:  75 09                    JNZ 0x14031f99c
14031f993:  C6 45 80 01              MOV byte ptr [RBP + -0x80],0x1
14031f997:  E9 D4 05 00 00           JMP 0x14031ff70
14031f99c:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
14031f9a0:  48 8B 0C F0              MOV RCX,qword ptr [RAX + RSI*0x8]
14031f9a4:  49 89 8E 50 C6 00 00     MOV qword ptr [R14 + 0xc650],RCX
14031f9ab:  48 FF C1                 INC RCX
14031f9ae:  48 8D 15 FB 28 59 00     LEA RDX,[0x1408b22b0]
14031f9b5:  E8 66 FB 52 00           CALL 0x14084f520
14031f9ba:  85 C0                    TEST EAX,EAX
14031f9bc:  0F 85 78 01 00 00        JNZ 0x14031fb3a
=== 0x14031fb3a to 0x14031fbc7 from export/140317db0_full_disasm.asm ===
14031fb3a:  49 8B 8E 50 C6 00 00     MOV RCX,qword ptr [R14 + 0xc650]
14031fb41:  48 FF C1                 INC RCX
14031fb44:  48 8D 15 A5 26 59 00     LEA RDX,[0x1408b21f0]
14031fb4b:  E8 D0 F9 52 00           CALL 0x14084f520
14031fb50:  85 C0                    TEST EAX,EAX
14031fb52:  48 8B 05 97 46 67 00     MOV RAX,qword ptr [0x1409941f0]
14031fb59:  75 0A                    JNZ 0x14031fb65
14031fb5b:  C6 40 1A 00              MOV byte ptr [RAX + 0x1a],0x0
14031fb5f:  C6 40 20 01              MOV byte ptr [RAX + 0x20],0x1
14031fb63:  EB 08                    JMP 0x14031fb6d
14031fb65:  C6 40 1A 01              MOV byte ptr [RAX + 0x1a],0x1
14031fb69:  C6 40 20 00              MOV byte ptr [RAX + 0x20],0x0
14031fb6d:  45 33 C9                 XOR R9D,R9D
14031fb70:  4D 89 8E 28 D4 00 00     MOV qword ptr [R14 + 0xd428],R9
14031fb77:  48 83 CF FF              OR RDI,-0x1
14031fb7b:  41 89 BE 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],EDI
14031fb82:  45 89 8E D8 CC 00 00     MOV dword ptr [R14 + 0xccd8],R9D
14031fb89:  41 C6 86 DC CC 00 00 01  MOV byte ptr [R14 + 0xccdc],0x1
14031fb91:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
14031fb95:  48 8B 1C F0              MOV RBX,qword ptr [RAX + RSI*0x8]
14031fb99:  48 FF C3                 INC RBX
14031fb9c:  41 8B C9                 MOV ECX,R9D
14031fb9f:  48 8D 15 3E 26 59 00     LEA RDX,[0x1408b21e4]
14031fba6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
14031fbb0:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031fbb4:  48 FF C1                 INC RCX
14031fbb7:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031fbbb:  0F 85 FD 00 00 00        JNZ 0x14031fcbe
14031fbc1:  48 83 F9 06              CMP RCX,0x6
14031fbc5:  75 E9                    JNZ 0x14031fbb0
14031fbc7:  45 8B C1                 MOV R8D,R9D
=== 0x14031fc55 to 0x14031fcb9 from export/140317db0_full_disasm.asm ===
14031fc55:  41 FF C0                 INC R8D
14031fc58:  49 8B 86 C8 CA 00 00     MOV RAX,qword ptr [R14 + 0xcac8]
14031fc5f:  49 2B 86 C0 CA 00 00     SUB RAX,qword ptr [R14 + 0xcac0]
14031fc66:  48 C1 F8 03              SAR RAX,0x3
14031fc6a:  41 8B D0                 MOV EDX,R8D
14031fc6d:  48 3B D0                 CMP RDX,RAX
14031fc70:  0F 82 7A FF FF FF        JC 0x14031fbf0
14031fc76:  45 89 8E 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],R9D
14031fc7d:  45 89 8E D0 C7 00 00     MOV dword ptr [R14 + 0xc7d0],R9D
14031fc84:  41 89 BE D4 C7 00 00     MOV dword ptr [R14 + 0xc7d4],EDI
14031fc8b:  45 88 8E D8 C7 00 00     MOV byte ptr [R14 + 0xc7d8],R9B
14031fc92:  45 89 8E 20 0F 01 00     MOV dword ptr [R14 + 0x10f20],R9D
14031fc99:  45 33 C0                 XOR R8D,R8D
14031fc9c:  33 D2                    XOR EDX,EDX
14031fc9e:  49 8B CE                 MOV RCX,R14
14031fca1:  E8 9A DB FD FF           CALL 0x1402fd840
14031fca6:  41 C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [R14 + 0x10be8],0xffffffff
14031fcb1:  41 C6 86 9A 03 01 00 00  MOV byte ptr [R14 + 0x1039a],0x0
14031fcb9:  E9 B2 02 00 00           JMP 0x14031ff70
=== 0x14031fdb4 to 0x14031fe5d from export/140317db0_full_disasm.asm ===
14031fdb4:  45 33 C0                 XOR R8D,R8D
14031fdb7:  41 8B C8                 MOV ECX,R8D
14031fdba:  48 8D 15 97 24 59 00     LEA RDX,[0x1408b2258]
14031fdc1:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031fdc5:  48 FF C1                 INC RCX
14031fdc8:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031fdcc:  75 35                    JNZ 0x14031fe03
14031fdce:  48 83 F9 05              CMP RCX,0x5
14031fdd2:  75 ED                    JNZ 0x14031fdc1
14031fdd4:  41 89 BE C8 CC 00 00     MOV dword ptr [R14 + 0xccc8],EDI
14031fddb:  45 89 86 D0 CC 00 00     MOV dword ptr [R14 + 0xccd0],R8D
14031fde2:  45 89 86 CC CC 00 00     MOV dword ptr [R14 + 0xcccc],R8D
14031fde9:  45 89 86 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],R8D
14031fdf0:  45 88 86 EC CC 00 00     MOV byte ptr [R14 + 0xccec],R8B
14031fdf7:  45 88 86 ED CC 00 00     MOV byte ptr [R14 + 0xcced],R8B
14031fdfe:  E9 6D 01 00 00           JMP 0x14031ff70
14031fe03:  49 8B C8                 MOV RCX,R8
14031fe06:  48 8D 15 43 24 59 00     LEA RDX,[0x1408b2250]
14031fe0d:  0F 1F 00                 NOP dword ptr [RAX]
14031fe10:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031fe14:  48 FF C1                 INC RCX
14031fe17:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031fe1b:  75 45                    JNZ 0x14031fe62
14031fe1d:  48 83 F9 06              CMP RCX,0x6
14031fe21:  75 ED                    JNZ 0x14031fe10
14031fe23:  41 89 BE C8 CC 00 00     MOV dword ptr [R14 + 0xccc8],EDI
14031fe2a:  45 89 86 D0 CC 00 00     MOV dword ptr [R14 + 0xccd0],R8D
14031fe31:  45 89 86 CC CC 00 00     MOV dword ptr [R14 + 0xcccc],R8D
14031fe38:  45 89 86 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],R8D
14031fe3f:  41 89 BE E8 CC 00 00     MOV dword ptr [R14 + 0xcce8],EDI
14031fe46:  41 89 BE E0 CC 00 00     MOV dword ptr [R14 + 0xcce0],EDI
14031fe4d:  41 C6 86 E4 CC 00 00 01  MOV byte ptr [R14 + 0xcce4],0x1
14031fe55:  49 8B CE                 MOV RCX,R14
14031fe58:  E8 83 FF 0B 00           CALL 0x1403dfde0
14031fe5d:  E9 0E 01 00 00           JMP 0x14031ff70
=== 0x14031fec7 to 0x14031ff70 from export/140317db0_full_disasm.asm ===
14031fec7:  E9 A4 00 00 00           JMP 0x14031ff70
14031fecc:  48 8D 15 65 23 59 00     LEA RDX,[0x1408b2238]
14031fed3:  48 8B CB                 MOV RCX,RBX
14031fed6:  E8 45 F6 52 00           CALL 0x14084f520
14031fedb:  85 C0                    TEST EAX,EAX
14031fedd:  0F 84 7F 00 00 00        JZ 0x14031ff62
14031fee3:  48 8D 15 AE 24 59 00     LEA RDX,[0x1408b2398]
14031feea:  48 8B CB                 MOV RCX,RBX
14031feed:  E8 2E F6 52 00           CALL 0x14084f520
14031fef2:  85 C0                    TEST EAX,EAX
14031fef4:  74 6C                    JZ 0x14031ff62
14031fef6:  45 33 C0                 XOR R8D,R8D
14031fef9:  41 8B C8                 MOV ECX,R8D
14031fefc:  48 8D 15 8D 24 59 00     LEA RDX,[0x1408b2390]
14031ff03:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031ff07:  48 FF C1                 INC RCX
14031ff0a:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031ff0e:  75 08                    JNZ 0x14031ff18
14031ff10:  48 83 F9 08              CMP RCX,0x8
14031ff14:  75 ED                    JNZ 0x14031ff03
14031ff16:  EB 4A                    JMP 0x14031ff62
14031ff18:  49 8B C8                 MOV RCX,R8
14031ff1b:  48 8D 15 D2 92 55 00     LEA RDX,[0x1408791f4]
14031ff22:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031ff26:  48 FF C1                 INC RCX
14031ff29:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031ff2d:  75 08                    JNZ 0x14031ff37
14031ff2f:  48 83 F9 07              CMP RCX,0x7
14031ff33:  75 ED                    JNZ 0x14031ff22
14031ff35:  EB 39                    JMP 0x14031ff70
14031ff37:  48 8D 15 42 24 59 00     LEA RDX,[0x1408b2380]
14031ff3e:  48 8B CB                 MOV RCX,RBX
14031ff41:  E8 DA F5 52 00           CALL 0x14084f520
14031ff46:  85 C0                    TEST EAX,EAX
14031ff48:  75 26                    JNZ 0x14031ff70
14031ff4a:  48 8D 15 97 24 59 00     LEA RDX,[0x1408b23e8]
14031ff51:  49 8B CE                 MOV RCX,R14
14031ff54:  E8 47 AB D1 FF           CALL 0x14003aaa0
14031ff59:  49 89 86 28 D4 00 00     MOV qword ptr [R14 + 0xd428],RAX
14031ff60:  EB 0E                    JMP 0x14031ff70
14031ff62:  41 89 BE 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],EDI
14031ff69:  41 89 BE A8 C6 00 00     MOV dword ptr [R14 + 0xc6a8],EDI
14031ff70:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
=== 0x1403282ad to 0x140328303 from export/140317db0_full_disasm.asm ===
1403282ad:  49 83 BE 50 C6 00 00 00  CMP qword ptr [R14 + 0xc650],0x0
1403282b5:  74 29                    JZ 0x1403282e0
1403282b7:  49 8B 8E 50 C6 00 00     MOV RCX,qword ptr [R14 + 0xc650]
1403282be:  48 FF C1                 INC RCX
1403282c1:  48 8D 15 70 AF 58 00     LEA RDX,[0x1408b3238]
1403282c8:  E8 53 72 52 00           CALL 0x14084f520
1403282cd:  85 C0                    TEST EAX,EAX
1403282cf:  75 0F                    JNZ 0x1403282e0
1403282d1:  48 8B 05 18 BF 66 00     MOV RAX,qword ptr [0x1409941f0]
1403282d8:  C6 40 1A 01              MOV byte ptr [RAX + 0x1a],0x1
1403282dc:  C6 40 20 00              MOV byte ptr [RAX + 0x20],0x0
1403282e0:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
1403282e7:  80 79 14 00              CMP byte ptr [RCX + 0x14],0x0
1403282eb:  74 05                    JZ 0x1403282f2
1403282ed:  E8 CE 3E D1 FF           CALL 0x14003c1c0
1403282f2:  4D 89 BE 28 D4 00 00     MOV qword ptr [R14 + 0xd428],R15
1403282f9:  4D 89 BE 50 C6 00 00     MOV qword ptr [R14 + 0xc650],R15
140328300:  45 32 E4                 XOR R12B,R12B
140328303:  49 8B CE                 MOV RCX,R14
