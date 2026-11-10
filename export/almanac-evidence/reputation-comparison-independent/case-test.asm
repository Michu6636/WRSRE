
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

000000018002f120 <.text+0x2e120>:
   18002f120:	83 f9 07             	cmp    ecx,0x7
   18002f123:	0f 87 90 00 00 00    	ja     0x18002f1b9
   18002f129:	48 63 c1             	movsxd rax,ecx
   18002f12c:	48 8d 15 cd 0e fd ff 	lea    rdx,[rip+0xfffffffffffd0ecd]        # 0x180000000
   18002f133:	8b 8c 82 bc f1 02 00 	mov    ecx,DWORD PTR [rdx+rax*4+0x2f1bc]
   18002f13a:	48 03 ca             	add    rcx,rdx
   18002f13d:	ff e1                	jmp    rcx
   18002f13f:	33 c0                	xor    eax,eax
   18002f141:	39 05 11 40 09 00    	cmp    DWORD PTR [rip+0x94011],eax        # 0x1800c3158
   18002f147:	0f 95 c0             	setne  al
   18002f14a:	c3                   	ret
   18002f14b:	33 c0                	xor    eax,eax
   18002f14d:	39 05 2d 6f 25 00    	cmp    DWORD PTR [rip+0x256f2d],eax        # 0x180286080
   18002f153:	0f 95 c0             	setne  al
   18002f156:	c3                   	ret
   18002f157:	83 3d 4a 42 09 00 00 	cmp    DWORD PTR [rip+0x9424a],0x0        # 0x1800c33a8
   18002f15e:	74 59                	je     0x18002f1b9
   18002f160:	83 3d b1 e8 19 00 00 	cmp    DWORD PTR [rip+0x19e8b1],0x0        # 0x1801cda18
   18002f167:	74 50                	je     0x18002f1b9
   18002f169:	b8 01 00 00 00       	mov    eax,0x1
   18002f16e:	c3                   	ret
   18002f16f:	83 3d 2e 42 09 00 00 	cmp    DWORD PTR [rip+0x9422e],0x0        # 0x1800c33a4
   18002f176:	74 41                	je     0x18002f1b9
   18002f178:	83 3d 95 e8 19 00 00 	cmp    DWORD PTR [rip+0x19e895],0x0        # 0x1801cda14
   18002f17f:	74 38                	je     0x18002f1b9
   18002f181:	b8 01 00 00 00       	mov    eax,0x1
   18002f186:	c3                   	ret
   18002f187:	33 c0                	xor    eax,eax
   18002f189:	83 3d 24 5d 15 00 01 	cmp    DWORD PTR [rip+0x155d24],0x1        # 0x180184eb4
   18002f190:	0f 94 c0             	sete   al
   18002f193:	c3                   	ret
   18002f194:	33 c0                	xor    eax,eax
   18002f196:	83 3d 17 5d 15 00 02 	cmp    DWORD PTR [rip+0x155d17],0x2        # 0x180184eb4
   18002f19d:	0f 94 c0             	sete   al
   18002f1a0:	c3                   	ret
   18002f1a1:	33 c0                	xor    eax,eax
   18002f1a3:	39 05 73 e8 19 00    	cmp    DWORD PTR [rip+0x19e873],eax        # 0x1801cda1c
   18002f1a9:	0f 95 c0             	setne  al
   18002f1ac:	c3                   	ret
   18002f1ad:	33 c0                	xor    eax,eax
   18002f1af:	39 05 9b 5a 09 00    	cmp    DWORD PTR [rip+0x95a9b],eax        # 0x1800c4c50
   18002f1b5:	0f 95 c0             	setne  al
   18002f1b8:	c3                   	ret
   18002f1b9:	33 c0                	xor    eax,eax
   18002f1bb:	c3                   	ret
