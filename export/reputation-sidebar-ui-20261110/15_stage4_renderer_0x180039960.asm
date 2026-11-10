   180039960:	mov    QWORD PTR [rsp+0x8],rbx
   180039965:	mov    QWORD PTR [rsp+0x10],rsi
   18003996a:	push   rdi
   18003996b:	sub    rsp,0xa0
   180039972:	movaps XMMWORD PTR [rsp+0x90],xmm6
   18003997a:	mov    rax,QWORD PTR [rip+0x8a7ff]        # 0x1800c4180
   180039981:	xor    rax,rsp
   180039984:	mov    QWORD PTR [rsp+0x88],rax
   18003998c:	movdqa xmm0,XMMWORD PTR [rip+0x75edc]        # 0x1800af870
   180039994:	movups XMMWORD PTR [rsp+0x68],xmm0
   180039999:	movss  xmm2,DWORD PTR [rip+0x89bd7]        # 0x1800c3578
   1800399a1:	movss  DWORD PTR [rsp+0x58],xmm2
   1800399a7:	movss  xmm1,DWORD PTR [rip+0x89bcd]        # 0x1800c357c
   1800399af:	movss  DWORD PTR [rsp+0x5c],xmm1
   1800399b5:	mov    DWORD PTR [rsp+0x60],0x0
   1800399bd:	cmp    DWORD PTR [rip+0x24e0e8],0x0        # 0x180287aac
   1800399c4:	je     0x180039a09
   1800399c6:	movss  xmm3,DWORD PTR [rip+0x24e0d6]        # 0x180287aa4
   1800399ce:	comiss xmm3,xmm2
   1800399d1:	jb     0x180039a09
   1800399d3:	movss  xmm0,DWORD PTR [rip+0x89ba5]        # 0x1800c3580
   1800399db:	addss  xmm0,xmm2
   1800399df:	comiss xmm0,xmm3
   1800399e2:	jb     0x180039a09
   1800399e4:	movss  xmm2,DWORD PTR [rip+0x24e0bc]        # 0x180287aa8
   1800399ec:	comiss xmm2,xmm1
   1800399ef:	jb     0x180039a09
   1800399f1:	movss  xmm0,DWORD PTR [rip+0x89b8b]        # 0x1800c3584
   1800399f9:	addss  xmm0,xmm1
   1800399fd:	comiss xmm0,xmm2
   180039a00:	jb     0x180039a09
   180039a02:	mov    ebx,0x1
   180039a07:	jmp    0x180039a0b
   180039a09:	xor    ebx,ebx
   180039a0b:	cmp    QWORD PTR [rip+0x24c695],0x0        # 0x1802860a8
   180039a13:	jne    0x180039a24
   180039a15:	mov    DWORD PTR [rip+0x2470d9],0x3        # 0x180280af8
   180039a1f:	jmp    0x180039bfb
   180039a24:	movdqa xmm0,XMMWORD PTR [rip+0x75e04]        # 0x1800af830
   180039a2c:	movdqu XMMWORD PTR [rsp+0x78],xmm0
   180039a32:	mov    rcx,QWORD PTR [rip+0x8b347]        # 0x1800c4d80
   180039a39:	add    rcx,0x9be060
   180039a40:	mov    r8b,0x1
   180039a43:	lea    rdx,[rip+0x62d9a]        # 0x18009c7e4    ;; "Basic"
   180039a4a:	call   QWORD PTR [rip+0x24c640]        # 0x180286090
   180039a50:	lea    rcx,[rip+0x89bb9]        # 0x1800c3610    ;; "editor/window_small.png"
   180039a57:	call   0x18003a570
   180039a5c:	mov    rsi,rax
   180039a5f:	test   rax,rax
   180039a62:	je     0x180039aa7
   180039a64:	cmp    QWORD PTR [rip+0x24c68c],0x0        # 0x1802860f8
   180039a6c:	je     0x180039aa7
   180039a6e:	mov    rdi,QWORD PTR [rip+0x8b30b]        # 0x1800c4d80
   180039a75:	mov    edx,0x8
   180039a7a:	lea    rcx,[rdi+0x9ead08]
   180039a81:	mov    r8,QWORD PTR [rip+0x8b2e8]        # 0x1800c4d70
   180039a88:	call   QWORD PTR [r8+0x60]
   180039a8c:	test   eax,eax
   180039a8e:	je     0x180039aa7
   180039a90:	mov    r8,QWORD PTR [rdi+0x9ead08]
   180039a97:	test   r8,r8
   180039a9a:	je     0x180039aa7
   180039a9c:	xor    edx,edx
   180039a9e:	mov    rcx,rsi
   180039aa1:	call   QWORD PTR [rip+0x24c651]        # 0x1802860f8
   180039aa7:	mov    rcx,QWORD PTR [rip+0x8b2d2]        # 0x1800c4d80
   180039aae:	add    rcx,0x9be060
   180039ab5:	lea    rdx,[rsp+0x68]
   180039aba:	call   QWORD PTR [rip+0x24c5f0]        # 0x1802860b0
   180039ac0:	mov    rcx,QWORD PTR [rip+0x8b2b9]        # 0x1800c4d80
   180039ac7:	add    rcx,0x9be060
   180039ace:	movss  xmm2,DWORD PTR [rip+0x89aae]        # 0x1800c3584
   180039ad6:	movss  xmm1,DWORD PTR [rip+0x89aa2]        # 0x1800c3580
   180039ade:	call   QWORD PTR [rip+0x24c5d4]        # 0x1802860b8
   180039ae4:	mov    rcx,QWORD PTR [rip+0x8b295]        # 0x1800c4d80
   180039aeb:	add    rcx,0x9be060
   180039af2:	lea    rdx,[rsp+0x58]
   180039af7:	call   QWORD PTR [rip+0x24c5c3]        # 0x1802860c0
   180039afd:	movss  xmm6,DWORD PTR [rip+0x7576f]        # 0x1800af274
   180039b05:	test   ebx,ebx
   180039b07:	je     0x180039b0e
   180039b09:	movaps xmm1,xmm6
   180039b0c:	jmp    0x180039b11
   180039b0e:	xorps  xmm1,xmm1
   180039b11:	mov    rcx,QWORD PTR [rip+0x8b268]        # 0x1800c4d80
   180039b18:	add    rcx,0x9be060
   180039b1f:	call   QWORD PTR [rip+0x24c5a3]        # 0x1802860c8
   180039b25:	mov    rcx,QWORD PTR [rip+0x8b254]        # 0x1800c4d80
   180039b2c:	add    rcx,0x9be060
   180039b33:	movss  xmm0,DWORD PTR [rip+0x75809]        # 0x1800af344
   180039b3b:	movss  DWORD PTR [rsp+0x38],xmm0
   180039b41:	movss  DWORD PTR [rsp+0x30],xmm0
   180039b47:	movss  xmm3,DWORD PTR [rip+0x75795]        # 0x1800af2e4
   180039b4f:	movss  DWORD PTR [rsp+0x28],xmm3
   180039b55:	movss  DWORD PTR [rsp+0x20],xmm6
   180039b5b:	movaps xmm2,xmm6
   180039b5e:	lea    rdx,[rsp+0x78]
   180039b63:	call   QWORD PTR [rip+0x24c53f]        # 0x1802860a8
   180039b69:	mov    rcx,QWORD PTR [rip+0x8b210]        # 0x1800c4d80
   180039b70:	add    rcx,0x9be060
   180039b77:	xorps  xmm1,xmm1
   180039b7a:	call   QWORD PTR [rip+0x24c548]        # 0x1802860c8
   180039b80:	mov    rcx,QWORD PTR [rip+0x8b1f9]        # 0x1800c4d80
   180039b87:	add    rcx,0x9be060
   180039b8e:	call   QWORD PTR [rip+0x24c504]        # 0x180286098
   180039b94:	nop
   180039b95:	movss  xmm1,DWORD PTR [rip+0x899e7]        # 0x1800c3584
   180039b9d:	mulss  xmm1,DWORD PTR [rip+0x75673]        # 0x1800af218
   180039ba5:	addss  xmm1,DWORD PTR [rip+0x899cf]        # 0x1800c357c
   180039bad:	movss  xmm0,DWORD PTR [rip+0x899c3]        # 0x1800c3578
   180039bb5:	addss  xmm0,DWORD PTR [rip+0x75abb]        # 0x1800af678
   180039bbd:	lea    r9,[rip+0x62e44]        # 0x18009ca08    ;; "REPUTATION"
   180039bc4:	mov    r8d,0xffffffff
   180039bca:	call   0x180022c50
   180039bcf:	jmp    0x180039bfb
   180039bd1:	lea    rcx,[rip+0x62ee0]        # 0x18009cab8    ;; "  panel  stage 4 faulted - back to stage 3"
   180039bd8:	call   0x180053ef0
   180039bdd:	mov    DWORD PTR [rip+0x246f11],0x3        # 0x180280af8
   180039be7:	mov    rcx,QWORD PTR [rip+0x895f2]        # 0x1800c31e0
   180039bee:	cmp    rcx,0xffffffffffffffff
   180039bf2:	je     0x180039bfb
   180039bf4:	call   QWORD PTR [rip+0x5346e]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   180039bfa:	nop
   180039bfb:	mov    rcx,QWORD PTR [rsp+0x88]
   180039c03:	xor    rcx,rsp
   180039c06:	call   0x180069e90
   180039c0b:	lea    r11,[rsp+0xa0]
   180039c13:	mov    rbx,QWORD PTR [r11+0x10]
   180039c17:	mov    rsi,QWORD PTR [r11+0x18]
   180039c1b:	movaps xmm6,XMMWORD PTR [r11-0x10]
   180039c20:	mov    rsp,r11
   180039c23:	pop    rdi
   180039c24:	ret
