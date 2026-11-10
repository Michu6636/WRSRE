
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180057230 <.text+0x56230>:
   180057230:	4c 8b dc             	mov    r11,rsp
   180057233:	53                   	push   rbx
   180057234:	48 81 ec 20 02 00 00 	sub    rsp,0x220
   18005723b:	48 8b 05 3e cf 06 00 	mov    rax,QWORD PTR [rip+0x6cf3e]        # 0x1800c4180
   180057242:	48 33 c4             	xor    rax,rsp
   180057245:	48 89 84 24 00 02 00 	mov    QWORD PTR [rsp+0x200],rax
   18005724c:	00 
   18005724d:	83 3d 2c 9b 22 00 00 	cmp    DWORD PTR [rip+0x229b2c],0x0        # 0x180280d80
   180057254:	0f 84 b7 06 00 00    	je     0x180057911
   18005725a:	83 3d 23 9b 22 00 00 	cmp    DWORD PTR [rip+0x229b23],0x0        # 0x180280d84
   180057261:	0f 84 aa 06 00 00    	je     0x180057911
   180057267:	48 8b 05 02 db 06 00 	mov    rax,QWORD PTR [rip+0x6db02]        # 0x1800c4d70
   18005726e:	ba 08 00 00 00       	mov    edx,0x8
   180057273:	49 89 7b 18          	mov    QWORD PTR [r11+0x18],rdi
   180057277:	48 8b 3d 02 db 06 00 	mov    rdi,QWORD PTR [rip+0x6db02]        # 0x1800c4d80
   18005727e:	49 89 6b 08          	mov    QWORD PTR [r11+0x8],rbp
   180057282:	49 89 73 10          	mov    QWORD PTR [r11+0x10],rsi
   180057286:	48 8d 8f 60 15 9e 00 	lea    rcx,[rdi+0x9e1560]
   18005728d:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057290:	33 db                	xor    ebx,ebx
   180057292:	85 c0                	test   eax,eax
   180057294:	0f 84 d7 01 00 00    	je     0x180057471
   18005729a:	48 8b 87 60 15 9e 00 	mov    rax,QWORD PTR [rdi+0x9e1560]
   1800572a1:	48 3b 05 d0 76 48 00 	cmp    rax,QWORD PTR [rip+0x4876d0]        # 0x1804de978
   1800572a8:	0f 84 c3 01 00 00    	je     0x180057471
   1800572ae:	48 89 05 c3 76 48 00 	mov    QWORD PTR [rip+0x4876c3],rax        # 0x1804de978
   1800572b5:	48 85 c0             	test   rax,rax
   1800572b8:	0f 84 d2 01 00 00    	je     0x180057490
