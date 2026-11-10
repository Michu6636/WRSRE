   180057d50:	mov    QWORD PTR [rsp+0x8],rbx
   180057d55:	push   rdi
   180057d56:	sub    rsp,0x20
   180057d5a:	cmp    DWORD PTR [rip+0x229023],0x0        # 0x180280d84
   180057d61:	je     0x180057e26
   180057d67:	mov    rbx,QWORD PTR [rip+0x6d012]        # 0x1800c4d80
   180057d6e:	mov    edi,0x1
   180057d73:	add    rbx,0x9e1560
   180057d7a:	cmp    DWORD PTR [rip+0x486af7],0x0        # 0x1804de878
   180057d81:	jne    0x180057da3
   180057d83:	mov    rax,QWORD PTR [rip+0x6cfe6]        # 0x1800c4d70
   180057d8a:	mov    edx,0x8
   180057d8f:	mov    rcx,rbx
   180057d92:	call   QWORD PTR [rax+0x60]
   180057d95:	test   eax,eax
   180057d97:	je     0x180057e26
   180057d9d:	mov    DWORD PTR [rip+0x486ad5],edi        # 0x1804de878
   180057da3:	test   rbx,rbx
   180057da6:	je     0x180057e26
   180057da8:	mov    rbx,QWORD PTR [rbx]
   180057dab:	test   rbx,rbx
   180057dae:	je     0x180057e26
   180057db0:	cmp    rbx,QWORD PTR [rip+0x486ac9]        # 0x1804de880
   180057db7:	jne    0x180057dc7
   180057db9:	mov    eax,DWORD PTR [rip+0x6b419]        # 0x1800c31d8
   180057dbf:	cmp    DWORD PTR [rip+0x486ac3],eax        # 0x1804de888
   180057dc5:	je     0x180057df1
   180057dc7:	mov    rax,QWORD PTR [rip+0x6cfa2]        # 0x1800c4d70
   180057dce:	lea    rcx,[rbx+0x1]
   180057dd2:	mov    edx,0x17
   180057dd7:	call   QWORD PTR [rax+0x60]
   180057dda:	test   eax,eax
   180057ddc:	je     0x180057e26
   180057dde:	mov    eax,DWORD PTR [rip+0x6b3f4]        # 0x1800c31d8
   180057de4:	mov    DWORD PTR [rip+0x486a9e],eax        # 0x1804de888
   180057dea:	mov    QWORD PTR [rip+0x486a8f],rbx        # 0x1804de880
   180057df1:	xor    eax,eax
   180057df3:	lea    rdx,[rip+0x6be16]        # 0x1800c3c10    ;; "reputation"
   180057dfa:	nop    WORD PTR [rax+rax*1+0x0]
   180057e00:	movzx  ecx,BYTE PTR [rax+rbx*1+0x1]
   180057e05:	cmp    cl,BYTE PTR [rdx+rax*1]
   180057e08:	jne    0x180057e17
   180057e0a:	test   cl,cl
   180057e0c:	je     0x180057e19
   180057e0e:	inc    rax
   180057e11:	cmp    rax,0x17
   180057e15:	jb     0x180057e00
   180057e17:	xor    edi,edi
   180057e19:	mov    eax,edi
   180057e1b:	mov    rbx,QWORD PTR [rsp+0x30]
   180057e20:	add    rsp,0x20
   180057e24:	pop    rdi
   180057e25:	ret
   180057e26:	mov    rbx,QWORD PTR [rsp+0x30]
   180057e2b:	xor    eax,eax
   180057e2d:	add    rsp,0x20
   180057e31:	pop    rdi
   180057e32:	ret
