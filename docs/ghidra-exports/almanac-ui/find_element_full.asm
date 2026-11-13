; Range: 0x1800D1730 to 0x1800D17C0  (containing fn: FindElement@0x1800D1730)
1800d1730:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
1800d1735:  4C 8B 19                                  MOV R11,qword ptr [RCX]
1800d1738:  48 B8 79 78 78 78 78 78 78 78             MOV RAX,0x7878787878787879
1800d1742:  48 8B 49 08                               MOV RCX,qword ptr [RCX + 0x8]
1800d1746:  48 8B DA                                  MOV RBX,RDX
1800d1749:  49 2B CB                                  SUB RCX,R11
1800d174c:  45 33 C9                                  XOR R9D,R9D
1800d174f:  48 F7 E9                                  IMUL RCX
1800d1752:  4C 8B D2                                  MOV R10,RDX
1800d1755:  49 C1 FA 06                               SAR R10,0x6
1800d1759:  49 8B C2                                  MOV RAX,R10
1800d175c:  48 C1 E8 3F                               SHR RAX,0x3f
1800d1760:  4C 03 D0                                  ADD R10,RAX
1800d1763:  74 3D                                     JZ 0x1800d17a2
1800d1765:  41 8B C1                                  MOV EAX,R9D
1800d1768:  0F 1F 84 00 00 00 00 00                   NOP dword ptr [RAX + RAX*0x1]
1800d1770:  48 69 C0 88 00 00 00                      IMUL RAX,RAX,0x88
1800d1777:  4C 8B C3                                  MOV R8,RBX
1800d177a:  49 03 C3                                  ADD RAX,R11
1800d177d:  4C 2B C0                                  SUB R8,RAX
1800d1780:  0F B6 10                                  MOVZX EDX,byte ptr [RAX]
1800d1783:  42 0F B6 0C 00                            MOVZX ECX,byte ptr [RAX + R8*0x1]
1800d1788:  2B D1                                     SUB EDX,ECX
1800d178a:  75 07                                     JNZ 0x1800d1793
1800d178c:  48 FF C0                                  INC RAX
1800d178f:  85 C9                                     TEST ECX,ECX
1800d1791:  75 ED                                     JNZ 0x1800d1780
1800d1793:  85 D2                                     TEST EDX,EDX
1800d1795:  74 13                                     JZ 0x1800d17aa
1800d1797:  41 FF C1                                  INC R9D
1800d179a:  41 8B C1                                  MOV EAX,R9D
1800d179d:  49 3B C2                                  CMP RAX,R10
1800d17a0:  72 CE                                     JC 0x1800d1770
1800d17a2:  33 C0                                     XOR EAX,EAX
1800d17a4:  48 8B 5C 24 08                            MOV RBX,qword ptr [RSP + 0x8]
1800d17a9:  C3                                        RET
1800d17aa:  48 8B 5C 24 08                            MOV RBX,qword ptr [RSP + 0x8]
1800d17af:  41 8B C9                                  MOV ECX,R9D
1800d17b2:  48 69 C1 88 00 00 00                      IMUL RAX,RCX,0x88
1800d17b9:  4A 8B 84 18 80 00 00 00                   MOV RAX,qword ptr [RAX + R11*0x1 + 0x80]
; 40 instruction(s)
