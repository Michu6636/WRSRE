
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180068799 <.text+0x67799>:
   180068799:	83 3d 3c 8e 13 00 00 	cmp    DWORD PTR [rip+0x138e3c],0x0        # 0x1801a15dc
   1800687a0:	7e 55                	jle    0x1800687f7
   1800687a2:	66 0f 2f 35 be aa 05 	comisd xmm6,QWORD PTR [rip+0x5aabe]        # 0x1800c3268
   1800687a9:	00 
   1800687aa:	73 4b                	jae    0x1800687f7
   1800687ac:	e8 df 26 fd ff       	call   0x18003ae90
   1800687b1:	48 63 0d 20 8e 13 00 	movsxd rcx,DWORD PTR [rip+0x138e20]        # 0x1801a15d8
   1800687b8:	83 f9 30             	cmp    ecx,0x30
   1800687bb:	7d 32                	jge    0x1800687ef
   1800687bd:	48 8d 15 14 39 03 00 	lea    rdx,[rip+0x33914]        # 0x18009c0d8
   1800687c4:	48 89 94 cf e0 15 1a 	mov    QWORD PTR [rdi+rcx*8+0x1a15e0],rdx
   1800687cb:	00 
   1800687cc:	0f 28 c8             	movaps xmm1,xmm0
   1800687cf:	f2 0f 5c 0d f9 8d 13 	subsd  xmm1,QWORD PTR [rip+0x138df9]        # 0x1801a15d0
   1800687d6:	00 
   1800687d7:	0f 57 d2             	xorps  xmm2,xmm2
   1800687da:	f2 0f 5a d1          	cvtsd2ss xmm2,xmm1
   1800687de:	f3 0f 11 94 8f 60 17 	movss  DWORD PTR [rdi+rcx*4+0x1a1760],xmm2
   1800687e5:	1a 00 
   1800687e7:	ff c1                	inc    ecx
   1800687e9:	89 0d e9 8d 13 00    	mov    DWORD PTR [rip+0x138de9],ecx        # 0x1801a15d8
   1800687ef:	f2 0f 11 05 d9 8d 13 	movsd  QWORD PTR [rip+0x138dd9],xmm0        # 0x1801a15d0
   1800687f6:	00 
   1800687f7:	e8 74 e3 fe ff       	call   0x180056b70
   1800687fc:	e8 2f ea fe ff       	call   0x180057230
   180068801:	eb 44                	jmp    0x180068847
