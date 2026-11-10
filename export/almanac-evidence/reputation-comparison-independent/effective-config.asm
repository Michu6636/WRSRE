
Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180048cc2 <.text+0x47cc2>:
   180048cc2:	8b 05 24 a6 07 00    	mov    eax,DWORD PTR [rip+0x7a624]        # 0x1800c32ec
   180048cc8:	8b 0d a6 af 07 00    	mov    ecx,DWORD PTR [rip+0x7afa6]        # 0x1800c3c74
   180048cce:	89 0d 00 b3 15 00    	mov    DWORD PTR [rip+0x15b300],ecx        # 0x1801a3fd4
   180048cd4:	85 c0                	test   eax,eax
   180048cd6:	74 6a                	je     0x180048d42
   180048cd8:	44 39 3d a1 80 23 00 	cmp    DWORD PTR [rip+0x2380a1],r15d        # 0x180280d80
   180048cdf:	75 21                	jne    0x180048d02
   180048ce1:	48 8b 05 88 c0 07 00 	mov    rax,QWORD PTR [rip+0x7c088]        # 0x1800c4d70
   180048ce8:	48 8d 0d 31 2e 06 00 	lea    rcx,[rip+0x62e31]        # 0x1800abb20
   180048cef:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180048cf2:	44 89 3d 7b 80 23 00 	mov    DWORD PTR [rip+0x23807b],r15d        # 0x180280d74
   180048cf9:	44 89 3d ec a5 07 00 	mov    DWORD PTR [rip+0x7a5ec],r15d        # 0x1800c32ec
   180048d00:	eb 40                	jmp    0x180048d42
   180048d02:	85 c9                	test   ecx,ecx
   180048d04:	74 19                	je     0x180048d1f
   180048d06:	48 8d 0d 63 2e 06 00 	lea    rcx,[rip+0x62e63]        # 0x1800abb70
   180048d0d:	44 89 3d 60 af 07 00 	mov    DWORD PTR [rip+0x7af60],r15d        # 0x1800c3c74
   180048d14:	e8 d7 b1 00 00       	call   0x180053ef0
   180048d19:	8b 05 cd a5 07 00    	mov    eax,DWORD PTR [rip+0x7a5cd]        # 0x1800c32ec
   180048d1f:	85 c0                	test   eax,eax
   180048d21:	74 1f                	je     0x180048d42
   180048d23:	44 39 3d 56 aa 07 00 	cmp    DWORD PTR [rip+0x7aa56],r15d        # 0x1800c3780
   180048d2a:	75 16                	jne    0x180048d42
   180048d2c:	48 8d 0d ad 2e 06 00 	lea    rcx,[rip+0x62ead]        # 0x1800abbe0
   180048d33:	c7 05 43 aa 07 00 01 	mov    DWORD PTR [rip+0x7aa43],0x1        # 0x1800c3780
   180048d3a:	00 00 00 
   180048d3d:	e8 ae b1 00 00       	call   0x180053ef0
