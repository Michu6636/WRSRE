; Excerpt: 140373ea0_1403829fd_disasm.asm lines 125-290 (skip checks, c7d4 test, click sequence, advance)

  125: 1403740f8:  74 1A                    JZ 0x140374114
  126: 1403740fa:  C6 86 70 09 01 00 00     MOV byte ptr [RSI + 0x10970],0x0
  127: 140374101:  4C 89 B6 78 09 01 00     MOV qword ptr [RSI + 0x10978],R14
  128: 140374108:  48 8D 8E 80 09 01 00     LEA RCX,[RSI + 0x10980]
  129: 14037410f:  E8 AC DB E8 FF           CALL 0x140201cc0
  130: 140374114:  F3 0F 10 3D 6C DF 61 00  MOVSS XMM7,dword ptr [0x140992088]
  131: 14037411c:  0F 28 F7                 MOVAPS XMM6,XMM7
  132: 14037411f:  F3 0F 59 35 E5 68 59 00  MULSS XMM6,dword ptr [0x14090aa0c]
  133: 140374127:  0F 28 C7                 MOVAPS XMM0,XMM7
  134: 14037412a:  F3 41 0F 59 C3           MULSS XMM0,XMM11
  135: 14037412f:  F3 0F 58 F0              ADDSS XMM6,XMM0
  136: 140374133:  83 BE D4 C7 00 00 FF     CMP dword ptr [RSI + 0xc7d4],-0x1
  137: 14037413a:  0F 85 B2 02 00 00        JNZ 0x1403743f2
  138: 140374140:  F3 44 0F 10 25 57 69 59 00 MOVSS XMM12,dword ptr [0x14090aaa0]
  139: 140374149:  F3 41 0F 59 FC           MULSS XMM7,XMM12
  140: 14037414e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
  141: 140374155:  41 B8 06 00 00 00        MOV R8D,0x6
  142: 14037415b:  41 0F 28 CA              MOVAPS XMM1,XMM10
  143: 14037415f:  E8 3C 1F 0D 00           CALL 0x1404460a0
  144: 140374164:  41 8B FE                 MOV EDI,R14D
  145: 140374167:  4C 8B 86 50 C6 00 00     MOV R8,qword ptr [RSI + 0xc650]
  146: 14037416e:  49 83 C0 48              ADD R8,0x48
  147: 140374172:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
  148: 140374176:  49 2B 08                 SUB RCX,qword ptr [R8]
  149: 140374179:  49 8B C5                 MOV RAX,R13
  150: 14037417c:  48 F7 E9                 IMUL RCX
  151: 14037417f:  48 C1 FA 05              SAR RDX,0x5
  152: 140374183:  48 8B C2                 MOV RAX,RDX
  153: 140374186:  48 C1 E8 3F              SHR RAX,0x3f
  154: 14037418a:  48 03 D0                 ADD RDX,RAX
  155: 14037418d:  45 0F 57 C0              XORPS XMM8,XMM8
  156: 140374191:  F3 44 0F 10 0D BE 5D 59 00 MOVSS XMM9,dword ptr [0x140909f58]
  157: 14037419a:  85 D2                    TEST EDX,EDX
  158: 14037419c:  0F 8E 14 04 00 00        JLE 0x1403745b6
  159: 1403741a2:  83 FF 02                 CMP EDI,0x2
  160: 1403741a5:  7D 04                    JGE 0x1403741ab
  161: 1403741a7:  8B DF                    MOV EBX,EDI
  162: 1403741a9:  EB 24                    JMP 0x1403741cf
  163: 1403741ab:  75 1F                    JNZ 0x1403741cc
  164: 1403741ad:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
  165: 1403741b1:  49 2B 08                 SUB RCX,qword ptr [R8]
  166: 1403741b4:  49 8B C5                 MOV RAX,R13
  167: 1403741b7:  48 F7 E9                 IMUL RCX
  168: 1403741ba:  48 C1 FA 05              SAR RDX,0x5
  169: 1403741be:  48 8B DA                 MOV RBX,RDX
  170: 1403741c1:  48 C1 EB 3F              SHR RBX,0x3f
  171: 1403741c5:  48 03 DA                 ADD RBX,RDX
  172: 1403741c8:  FF CB                    DEC EBX
  173: 1403741ca:  EB 03                    JMP 0x1403741cf
  174: 1403741cc:  8D 5F FF                 LEA EBX,[RDI + -0x1]
  175: 1403741cf:  83 BE 78 05 00 00 00     CMP dword ptr [RSI + 0x578],0x0
  176: 1403741d6:  74 0C                    JZ 0x1403741e4
  177: 1403741d8:  8D 43 FD                 LEA EAX,[RBX + -0x3]
  178: 1403741db:  83 F8 01                 CMP EAX,0x1
  179: 1403741de:  0F 86 D2 01 00 00        JBE 0x1403743b6
  180: 1403741e4:  83 FB 06                 CMP EBX,0x6
  181: 1403741e7:  75 0D                    JNZ 0x1403741f6
  182: 1403741e9:  83 BE E0 05 00 00 00     CMP dword ptr [RSI + 0x5e0],0x0
  183: 1403741f0:  0F 84 C0 01 00 00        JZ 0x1403743b6
  184: 1403741f6:  48 63 C3                 MOVSXD RAX,EBX
  185: 1403741f9:  4C 8D 2C 80              LEA R13,[RAX + RAX*0x4]
  186: 1403741fd:  49 C1 E5 04              SHL R13,0x4
  187: 140374201:  4D 03 28                 ADD R13,qword ptr [R8]
  188: 140374204:  49 8B 4D 48              MOV RCX,qword ptr [R13 + 0x48]
  189: 140374208:  48 8B 01                 MOV RAX,qword ptr [RCX]
  190: 14037420b:  4C 8B 05 F6 6A 67 00     MOV R8,qword ptr [0x1409ead08]
  191: 140374212:  33 D2                    XOR EDX,EDX
  192: 140374214:  FF 50 70                 CALL qword ptr [RAX + 0x70]
  193: 140374217:  F3 0F 10 05 69 DE 61 00  MOVSS XMM0,dword ptr [0x140992088]
  194: 14037421f:  F3 41 0F 59 C4           MULSS XMM0,XMM12
  195: 140374224:  F3 0F 11 05 BC A0 64 00  MOVSS dword ptr [0x1409be2e8],XMM0
  196: 14037422c:  F3 0F 11 05 B8 A0 64 00  MOVSS dword ptr [0x1409be2ec],XMM0
  197: 140374234:  C7 44 24 58 00 00 00 00  MOV dword ptr [RSP + 0x58],0x0
  198: 14037423c:  0F 28 C6                 MOVAPS XMM0,XMM6
  199: 14037423f:  41 0F 14 C2              UNPCKLPS XMM0,XMM10
  200: 140374243:  F2 0F 11 05 A5 A0 64 00  MOVSD qword ptr [0x1409be2f0],XMM0
  201: 14037424b:  8B 44 24 58              MOV EAX,dword ptr [RSP + 0x58]
  202: 14037424f:  89 05 A3 A0 64 00        MOV dword ptr [0x1409be2f8],EAX
  203: 140374255:  41 B8 06 00 00 00        MOV R8D,0x6
  204: 14037425b:  0F 28 CE                 MOVAPS XMM1,XMM6
  205: 14037425e:  48 8D 8E B0 AD 00 00     LEA RCX,[RSI + 0xadb0]
  206: 140374265:  E8 46 1C 0D 00           CALL 0x140445eb0
  207: 14037426a:  0F 28 05 8F 7E 59 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
  208: 140374271:  0F 11 05 94 A0 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
  209: 140374278:  F2 0F 10 45 80           MOVSD XMM0,qword ptr [RBP + -0x80]
  210: 14037427d:  F2 0F 11 85 10 02 00 00  MOVSD qword ptr [RBP + 0x210],XMM0
  211: 140374285:  8B 45 88                 MOV EAX,dword ptr [RBP + -0x78]
  212: 140374288:  89 85 18 02 00 00        MOV dword ptr [RBP + 0x218],EAX
  213: 14037428e:  41 0F 28 D8              MOVAPS XMM3,XMM8
  214: 140374292:  41 0F 28 D0              MOVAPS XMM2,XMM8
  215: 140374296:  48 8D 95 10 02 00 00     LEA RDX,[RBP + 0x210]
  216: 14037429d:  48 8D 0D BC 9D 64 00     LEA RCX,[0x1409be060]
  217: 1403742a4:  FF 15 A6 85 4F 00        CALL qword ptr [0x14086c850]
  218: 1403742aa:  84 C0                    TEST AL,AL
  219: 1403742ac:  0F 84 B4 00 00 00        JZ 0x140374366
  220: 1403742b2:  41 8B 55 40              MOV EDX,dword ptr [R13 + 0x40]
  221: 1403742b6:  48 8D 0D D3 32 62 00     LEA RCX,[0x140997590]
  222: 1403742bd:  FF 15 CD 85 4F 00        CALL qword ptr [0x14086c890]
  223: 1403742c3:  48 8B D6                 MOV RDX,RSI
  224: 1403742c6:  48 2B D0                 SUB RDX,RAX
  225: 1403742c9:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
  226: 1403742d0:  0F B7 08                 MOVZX ECX,word ptr [RAX]
  227: 1403742d3:  66 89 8C 10 A0 D5 00 00  MOV word ptr [RAX + RDX*0x1 + 0xd5a0],CX
  228: 1403742db:  48 8D 40 02              LEA RAX,[RAX + 0x2]
  229: 1403742df:  66 85 C9                 TEST CX,CX
  230: 1403742e2:  75 EC                    JNZ 0x1403742d0
  231: 1403742e4:  0F 28 05 D5 81 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
  232: 1403742eb:  0F 11 05 1A A0 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
  233: 1403742f2:  38 0D 99 0B 6E 00        CMP byte ptr [0x140a54e91],CL
  234: 1403742f8:  74 6C                    JZ 0x140374366
  235: 1403742fa:  38 4E 14                 CMP byte ptr [RSI + 0x14],CL
  236: 1403742fd:  75 48                    JNZ 0x140374347
  237: 1403742ff:  48 8D 15 A2 8D 54 00     LEA RDX,[0x1408bd0a8]
  238: 140374306:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
  239: 14037430d:  FF 15 FD 89 4F 00        CALL qword ptr [0x14086cd10]
  240: 140374313:  84 C0                    TEST AL,AL
  241: 140374315:  74 30                    JZ 0x140374347
  242: 140374317:  48 8D 15 8A 8D 54 00     LEA RDX,[0x1408bd0a8]
  243: 14037431e:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
  244: 140374325:  FF 15 ED 89 4F 00        CALL qword ptr [0x14086cd18]
  245: 14037432b:  48 8B D0                 MOV RDX,RAX
  246: 14037432e:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
  247: 140374333:  41 0F 28 D9              MOVAPS XMM3,XMM9
  248: 140374337:  41 B0 01                 MOV R8B,0x1
  249: 14037433a:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
  250: 140374341:  FF 15 61 8F 4F 00        CALL qword ptr [0x14086d2a8]
  251: 140374347:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX
  252: 14037434d:  74 07                    JZ 0x140374356
  253: 14037434f:  C6 86 9A 03 01 00 00     MOV byte ptr [RSI + 0x1039a],0x0
  254: 140374356:  89 9E 5C C6 00 00        MOV dword ptr [RSI + 0xc65c],EBX
  255: 14037435c:  C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [RSI + 0x10be8],0xffffffff
  256: 140374366:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX
  257: 14037436c:  75 0E                    JNZ 0x14037437c
  258: 14037436e:  0F 28 05 4B 81 59 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
  259: 140374375:  0F 11 05 90 9F 64 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
  260: 14037437c:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
  261: 140374381:  F3 44 0F 11 44 24 28     MOVSS dword ptr [RSP + 0x28],XMM8
  262: 140374388:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
  263: 14037438f:  41 0F 28 D9              MOVAPS XMM3,XMM9
  264: 140374393:  41 0F 28 D0              MOVAPS XMM2,XMM8
  265: 140374397:  41 0F 28 C8              MOVAPS XMM1,XMM8
  266: 14037439b:  48 8D 0D BE 9C 64 00     LEA RCX,[0x1409be060]
  267: 1403743a2:  FF 15 B8 84 4F 00        CALL qword ptr [0x14086c860]
  268: 1403743a8:  F3 0F 58 F7              ADDSS XMM6,XMM7
  269: 1403743ac:  49 BD 67 66 66 66 66 66 66 66 MOV R13,0x6666666666666667
  270: 1403743b6:  FF C7                    INC EDI
  271: 1403743b8:  4C 8B 86 50 C6 00 00     MOV R8,qword ptr [RSI + 0xc650]
  272: 1403743bf:  49 83 C0 48              ADD R8,0x48
  273: 1403743c3:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
  274: 1403743c7:  49 2B 08                 SUB RCX,qword ptr [R8]
  275: 1403743ca:  49 8B C5                 MOV RAX,R13
  276: 1403743cd:  48 F7 E9                 IMUL RCX
  277: 1403743d0:  48 C1 FA 05              SAR RDX,0x5
  278: 1403743d4:  48 8B C2                 MOV RAX,RDX
  279: 1403743d7:  48 C1 E8 3F              SHR RAX,0x3f
  280: 1403743db:  48 03 D0                 ADD RDX,RAX
  281: 1403743de:  3B FA                    CMP EDI,EDX
  282: 1403743e0:  0F 8C BC FD FF FF        JL 0x1403741a2
  283: 1403743e6:  4C 8D A6 F0 C7 00 00     LEA R12,[RSI + 0xc7f0]
  284: 1403743ed:  E9 C4 01 00 00           JMP 0x1403745b6
  285: 1403743f2:  48 8B 8E 48 B4 00 00     MOV RCX,qword ptr [RSI + 0xb448]
  286: 1403743f9:  48 8B 01                 MOV RAX,qword ptr [RCX]
  287: 1403743fc:  4C 8B 05 05 69 67 00     MOV R8,qword ptr [0x1409ead08]
  288: 140374403:  33 D2                    XOR EDX,EDX
  289: 140374405:  FF 50 70                 CALL qword ptr [RAX + 0x70]
  290: 140374408:  F3 0F 10 05 78 DC 61 00  MOVSS XMM0,dword ptr [0x140992088]
