
/tmp/wrsre_Draw.bin:     file format binary


Disassembly of section .data:

00000001800baf40 <.data>:
   1800baf40:	48 8b c4             	mov    rax,rsp
   1800baf43:	48 89 58 10          	mov    QWORD PTR [rax+0x10],rbx
   1800baf47:	48 89 70 18          	mov    QWORD PTR [rax+0x18],rsi
   1800baf4b:	48 89 78 20          	mov    QWORD PTR [rax+0x20],rdi
   1800baf4f:	55                   	push   rbp
   1800baf50:	41 54                	push   r12
   1800baf52:	41 55                	push   r13
   1800baf54:	41 56                	push   r14
   1800baf56:	41 57                	push   r15
   1800baf58:	48 8d 68 b8          	lea    rbp,[rax-0x48]
   1800baf5c:	48 81 ec 20 01 00 00 	sub    rsp,0x120
   1800baf63:	0f 29 70 c8          	movaps XMMWORD PTR [rax-0x38],xmm6
   1800baf67:	0f 29 78 b8          	movaps XMMWORD PTR [rax-0x48],xmm7
   1800baf6b:	44 0f 29 40 a8       	movaps XMMWORD PTR [rax-0x58],xmm8
   1800baf70:	44 0f 29 48 98       	movaps XMMWORD PTR [rax-0x68],xmm9
   1800baf75:	44 0f 29 50 88       	movaps XMMWORD PTR [rax-0x78],xmm10
   1800baf7a:	44 0f 29 98 78 ff ff 	movaps XMMWORD PTR [rax-0x88],xmm11
   1800baf81:	ff
   1800baf82:	44 0f 29 a0 68 ff ff 	movaps XMMWORD PTR [rax-0x98],xmm12
   1800baf89:	ff
   1800baf8a:	44 0f 29 a8 58 ff ff 	movaps XMMWORD PTR [rax-0xa8],xmm13
   1800baf91:	ff
   1800baf92:	44 0f 29 b0 48 ff ff 	movaps XMMWORD PTR [rax-0xb8],xmm14
   1800baf99:	ff
   1800baf9a:	48 8b 05 f7 2b 11 00 	mov    rax,QWORD PTR [rip+0x112bf7]        # 0x1801cdb98
   1800bafa1:	48 33 c4             	xor    rax,rsp
   1800bafa4:	48 89 45 80          	mov    QWORD PTR [rbp-0x80],rax
   1800bafa8:	80 3d 34 17 12 00 00 	cmp    BYTE PTR [rip+0x121734],0x0        # 0x1801dc6e3
   1800bafaf:	44 0f 28 e3          	movaps xmm12,xmm3
   1800bafb3:	44 0f 28 ea          	movaps xmm13,xmm2
   1800bafb7:	44 0f 28 f1          	movaps xmm14,xmm1
   1800bafbb:	48 8b f9             	mov    rdi,rcx
   1800bafbe:	75 10                	jne    0x1800bafd0
   1800bafc0:	48 8b 89 80 02 00 00 	mov    rcx,QWORD PTR [rcx+0x280]
   1800bafc7:	83 ca ff             	or     edx,0xffffffff
   1800bafca:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1800bafcd:	ff 50 10             	call   QWORD PTR [rax+0x10]
   1800bafd0:	f3 44 0f 10 87 88 02 	movss  xmm8,DWORD PTR [rdi+0x288]
   1800bafd7:	00 00
   1800bafd9:	48 8d 97 c4 02 00 00 	lea    rdx,[rdi+0x2c4]
   1800bafe0:	f3 44 0f 10 9f 8c 02 	movss  xmm11,DWORD PTR [rdi+0x28c]
   1800bafe7:	00 00
   1800bafe9:	45 33 ed             	xor    r13d,r13d
   1800bafec:	f3 44 0f 59 05 97 db 	mulss  xmm8,DWORD PTR [rip+0xcdb97]        # 0x180188b8c
   1800baff3:	0c 00
   1800baff5:	48 8b cf             	mov    rcx,rdi
   1800baff8:	44 89 af cc 02 00 00 	mov    DWORD PTR [rdi+0x2cc],r13d
   1800bafff:	f3 44 0f 59 1d 84 db 	mulss  xmm11,DWORD PTR [rip+0xcdb84]        # 0x180188b8c
   1800bb006:	0c 00
   1800bb008:	45 0f 28 d0          	movaps xmm10,xmm8
   1800bb00c:	44 0f 57 15 5c e0 0c 	xorps  xmm10,XMMWORD PTR [rip+0xce05c]        # 0x180189070
   1800bb013:	00
   1800bb014:	f3 44 0f 11 97 c4 02 	movss  DWORD PTR [rdi+0x2c4],xmm10
   1800bb01b:	00 00
   1800bb01d:	f3 44 0f 11 9f c8 02 	movss  DWORD PTR [rdi+0x2c8],xmm11
   1800bb024:	00 00
   1800bb026:	e8 d5 da ff ff       	call   0x1800b8b00
   1800bb02b:	f3 0f 10 8f c4 02 00 	movss  xmm1,DWORD PTR [rdi+0x2c4]
   1800bb032:	00
   1800bb033:	48 8d b7 fc 01 00 00 	lea    rsi,[rdi+0x1fc]
   1800bb03a:	f3 0f 10 87 c8 02 00 	movss  xmm0,DWORD PTR [rdi+0x2c8]
   1800bb041:	00
   1800bb042:	48 8d 97 d0 02 00 00 	lea    rdx,[rdi+0x2d0]
   1800bb049:	f3 44 0f 10 4d 78    	movss  xmm9,DWORD PTR [rbp+0x78]
   1800bb04f:	48 8b cf             	mov    rcx,rdi
   1800bb052:	f3 0f 10 75 70       	movss  xmm6,DWORD PTR [rbp+0x70]
   1800bb057:	f3 0f 11 8f f8 01 00 	movss  DWORD PTR [rdi+0x1f8],xmm1
   1800bb05e:	00
   1800bb05f:	f3 0f 11 06          	movss  DWORD PTR [rsi],xmm0
   1800bb063:	f3 0f 11 8f 40 02 00 	movss  DWORD PTR [rdi+0x240],xmm1
   1800bb06a:	00
   1800bb06b:	f3 0f 11 87 44 02 00 	movss  DWORD PTR [rdi+0x244],xmm0
   1800bb072:	00
   1800bb073:	f3 44 0f 11 8f 00 02 	movss  DWORD PTR [rdi+0x200],xmm9
   1800bb07a:	00 00
   1800bb07c:	f3 44 0f 11 b7 38 02 	movss  DWORD PTR [rdi+0x238],xmm14
   1800bb083:	00 00
   1800bb085:	f3 0f 11 b7 3c 02 00 	movss  DWORD PTR [rdi+0x23c],xmm6
   1800bb08c:	00
   1800bb08d:	f3 44 0f 11 87 d0 02 	movss  DWORD PTR [rdi+0x2d0],xmm8
   1800bb094:	00 00
   1800bb096:	f3 44 0f 11 9f d4 02 	movss  DWORD PTR [rdi+0x2d4],xmm11
   1800bb09d:	00 00
   1800bb09f:	44 89 af d8 02 00 00 	mov    DWORD PTR [rdi+0x2d8],r13d
   1800bb0a6:	e8 55 da ff ff       	call   0x1800b8b00
   1800bb0ab:	f3 0f 10 8f d0 02 00 	movss  xmm1,DWORD PTR [rdi+0x2d0]
   1800bb0b2:	00
   1800bb0b3:	48 8d 97 dc 02 00 00 	lea    rdx,[rdi+0x2dc]
   1800bb0ba:	f3 0f 10 87 d4 02 00 	movss  xmm0,DWORD PTR [rdi+0x2d4]
   1800bb0c1:	00
   1800bb0c2:	48 8b cf             	mov    rcx,rdi
   1800bb0c5:	44 0f 57 1d a3 df 0c 	xorps  xmm11,XMMWORD PTR [rip+0xcdfa3]        # 0x180189070
   1800bb0cc:	00
   1800bb0cd:	f3 0f 11 8f 08 02 00 	movss  DWORD PTR [rdi+0x208],xmm1
   1800bb0d4:	00
   1800bb0d5:	f3 0f 11 87 0c 02 00 	movss  DWORD PTR [rdi+0x20c],xmm0
   1800bb0dc:	00
   1800bb0dd:	f3 0f 11 8f 50 02 00 	movss  DWORD PTR [rdi+0x250],xmm1
   1800bb0e4:	00
   1800bb0e5:	f3 0f 11 87 54 02 00 	movss  DWORD PTR [rdi+0x254],xmm0
   1800bb0ec:	00
   1800bb0ed:	f3 44 0f 11 8f 10 02 	movss  DWORD PTR [rdi+0x210],xmm9
   1800bb0f4:	00 00
   1800bb0f6:	f3 44 0f 11 a7 48 02 	movss  DWORD PTR [rdi+0x248],xmm12
   1800bb0fd:	00 00
   1800bb0ff:	f3 0f 11 b7 4c 02 00 	movss  DWORD PTR [rdi+0x24c],xmm6
   1800bb106:	00
   1800bb107:	f3 44 0f 11 87 dc 02 	movss  DWORD PTR [rdi+0x2dc],xmm8
   1800bb10e:	00 00
   1800bb110:	f3 44 0f 11 9f e0 02 	movss  DWORD PTR [rdi+0x2e0],xmm11
   1800bb117:	00 00
   1800bb119:	44 89 af e4 02 00 00 	mov    DWORD PTR [rdi+0x2e4],r13d
   1800bb120:	e8 db d9 ff ff       	call   0x1800b8b00
   1800bb125:	f3 0f 10 8f dc 02 00 	movss  xmm1,DWORD PTR [rdi+0x2dc]
   1800bb12c:	00
   1800bb12d:	48 8d 97 e8 02 00 00 	lea    rdx,[rdi+0x2e8]
   1800bb134:	f3 0f 10 87 e0 02 00 	movss  xmm0,DWORD PTR [rdi+0x2e0]
   1800bb13b:	00
   1800bb13c:	48 8b cf             	mov    rcx,rdi
   1800bb13f:	f3                   	repz
