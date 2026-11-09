; Range: 0x140202270 to 0x140202380
140202270:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
140202275:  57                       PUSH RDI
140202276:  48 83 EC 20              SUB RSP,0x20
14020227a:  4C 8B 41 08              MOV R8,qword ptr [RCX + 0x8]
14020227e:  48 8B F9                 MOV RDI,RCX
140202281:  4C 8B 11                 MOV R10,qword ptr [RCX]
140202284:  48 8B DA                 MOV RBX,RDX
140202287:  49 8B C8                 MOV RCX,R8
14020228a:  49 2B CA                 SUB RCX,R10
14020228d:  48 C1 F9 04              SAR RCX,0x4
140202291:  48 3B D1                 CMP RDX,RCX
140202294:  73 19                    JNC 0x1402022af
140202296:  48 2B D9                 SUB RBX,RCX
140202299:  48 C1 E3 04              SHL RBX,0x4
14020229d:  49 03 D8                 ADD RBX,R8
1402022a0:  48 89 5F 08              MOV qword ptr [RDI + 0x8],RBX
1402022a4:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
1402022a9:  48 83 C4 20              ADD RSP,0x20
1402022ad:  5F                       POP RDI
1402022ae:  C3                       RET
1402022af:  48 3B CB                 CMP RCX,RBX
1402022b2:  0F 83 96 00 00 00        JNC 0x14020234e
1402022b8:  4C 8B 4F 10              MOV R9,qword ptr [RDI + 0x10]
1402022bc:  48 2B D1                 SUB RDX,RCX
1402022bf:  49 8B C1                 MOV RAX,R9
1402022c2:  49 2B C0                 SUB RAX,R8
1402022c5:  48 C1 F8 04              SAR RAX,0x4
1402022c9:  48 3B C2                 CMP RAX,RDX
1402022cc:  73 51                    JNC 0x14020231f
1402022ce:  49 B8 FF FF FF FF FF FF FF 0F MOV R8,0xfffffffffffffff
1402022d8:  49 8B C0                 MOV RAX,R8
1402022db:  48 2B C1                 SUB RAX,RCX
1402022de:  48 3B C2                 CMP RAX,RDX
1402022e1:  73 0E                    JNC 0x1402022f1
1402022e3:  48 8D 0D 06 E2 66 00     LEA RCX,[0x1408704f0]
1402022ea:  FF 15 38 B7 66 00        CALL qword ptr [0x14086da28]
1402022f0:  CC                       INT3
1402022f1:  48 03 D1                 ADD RDX,RCX
1402022f4:  4D 2B CA                 SUB R9,R10
1402022f7:  49 C1 F9 04              SAR R9,0x4
1402022fb:  33 C9                    XOR ECX,ECX
1402022fd:  49 8B C1                 MOV RAX,R9
140202300:  48 D1 E8                 SHR RAX,0x1
140202303:  4C 2B C0                 SUB R8,RAX
140202306:  49 03 C1                 ADD RAX,R9
140202309:  4D 3B C1                 CMP R8,R9
14020230c:  48 0F 43 C8              CMOVNC RCX,RAX
140202310:  48 3B CA                 CMP RCX,RDX
140202313:  48 0F 43 D1              CMOVNC RDX,RCX
140202317:  48 8B CF                 MOV RCX,RDI
14020231a:  E8 31 CC E2 FF           CALL 0x14002ef50
14020231f:  48 8B 4F 08              MOV RCX,qword ptr [RDI + 0x8]
140202323:  48 8B D3                 MOV RDX,RBX
140202326:  48 8B C1                 MOV RAX,RCX
140202329:  48 2B 07                 SUB RAX,qword ptr [RDI]
14020232c:  48 C1 F8 04              SAR RAX,0x4
140202330:  48 2B D0                 SUB RDX,RAX
140202333:  E8 88 5E 00 00           CALL 0x1402081c0
140202338:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
14020233c:  48 2B 07                 SUB RAX,qword ptr [RDI]
14020233f:  48 C1 F8 04              SAR RAX,0x4
140202343:  48 2B D8                 SUB RBX,RAX
140202346:  48 C1 E3 04              SHL RBX,0x4
14020234a:  48 01 5F 08              ADD qword ptr [RDI + 0x8],RBX
14020234e:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
140202353:  48 83 C4 20              ADD RSP,0x20
140202357:  5F                       POP RDI
140202358:  C3                       RET
140202360:  48 89 5C 24 10           MOV qword ptr [RSP + 0x10],RBX
140202365:  57                       PUSH RDI
140202366:  48 83 EC 20              SUB RSP,0x20
14020236a:  48 8B FA                 MOV RDI,RDX
14020236d:  48 8B D9                 MOV RBX,RCX
140202370:  48 3B CA                 CMP RCX,RDX
140202373:  74 54                    JZ 0x1402023c9
140202375:  48 8B 09                 MOV RCX,qword ptr [RCX]
140202378:  48 89 74 24 30           MOV qword ptr [RSP + 0x30],RSI
14020237d:  33 F6                    XOR ESI,ESI
14020237f:  48 85 C9                 TEST RCX,RCX

