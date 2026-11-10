
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180057467 <.text+0x56467>:
   180057467:	48 8b cd             	mov    rcx,rbp
   18005746a:	e8 c1 04 00 00       	call   0x180057930
   18005746f:	eb 1f                	jmp    0x180057490
   180057471:	48 8b 05 f8 d8 06 00 	mov    rax,QWORD PTR [rip+0x6d8f8]        # 0x1800c4d70
   180057478:	48 8d 8f 60 15 9e 00 	lea    rcx,[rdi+0x9e1560]
   18005747f:	ba 08 00 00 00       	mov    edx,0x8
   180057484:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057487:	85 c0                	test   eax,eax
   180057489:	74 05                	je     0x180057490
   18005748b:	e8 e0 ee ff ff       	call   0x180056370
   180057490:	e8 2b 1e 00 00       	call   0x1800592c0
   180057495:	39 1d 09 a7 3b 00    	cmp    DWORD PTR [rip+0x3ba709],ebx        # 0x180411ba4
   18005749b:	0f 84 10 01 00 00    	je     0x1800575b1
   1800574a1:	39 1d 01 a7 3b 00    	cmp    DWORD PTR [rip+0x3ba701],ebx        # 0x180411ba8
   1800574a7:	0f 8e 04 01 00 00    	jle    0x1800575b1
   1800574ad:	39 1d b5 c7 06 00    	cmp    DWORD PTR [rip+0x6c7b5],ebx        # 0x1800c3c68
   1800574b3:	0f 84 f8 00 00 00    	je     0x1800575b1
   1800574b9:	e8 92 08 00 00       	call   0x180057d50
   1800574be:	85 c0                	test   eax,eax
   1800574c0:	0f 84 eb 00 00 00    	je     0x1800575b1
