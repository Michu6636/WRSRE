   180056b70:	mov    QWORD PTR [rsp+0x8],rbx
   180056b75:	mov    QWORD PTR [rsp+0x10],rsi
   180056b7a:	push   rdi
   180056b7b:	sub    rsp,0xf0
   180056b82:	movaps XMMWORD PTR [rsp+0xe0],xmm6
   180056b8a:	mov    rax,QWORD PTR [rip+0x6d5ef]        # 0x1800c4180
   180056b91:	xor    rax,rsp
   180056b94:	mov    QWORD PTR [rsp+0xd0],rax
   180056b9c:	cmp    DWORD PTR [rip+0x230f95],0x0        # 0x180287b38
   180056ba3:	jne    0x1800570da
   180056ba9:	cmp    DWORD PTR [rip+0x22a1d0],0x0        # 0x180280d80
   180056bb0:	je     0x1800570da
   180056bb6:	cmp    DWORD PTR [rip+0x229f67],0x0        # 0x180280b24
   180056bbd:	jne    0x180056bda
   180056bbf:	mov    DWORD PTR [rip+0x230f6f],0x1        # 0x180287b38
   180056bc9:	lea    rcx,[rip+0x47930]        # 0x18009e500    ;; "  side  [ui] sidebar is on, but this is not the build these addresses were read off. Declined."
   180056bd0:	call   0x180053ef0
   180056bd5:	jmp    0x1800570da
   180056bda:	call   0x180064790
   180056bdf:	test   rax,rax
   180056be2:	jne    0x180056bf1
   180056be4:	xor    edi,edi
   180056be6:	mov    DWORD PTR [rip+0x230f50],edi        # 0x180287b3c
   180056bec:	jmp    0x1800570da
   180056bf1:	mov    eax,DWORD PTR [rip+0x230f45]        # 0x180287b3c
   180056bf7:	inc    eax
   180056bf9:	mov    DWORD PTR [rip+0x230f3d],eax        # 0x180287b3c
   180056bff:	cmp    eax,0xf0
   180056c04:	jl     0x1800570da
   180056c0a:	xor    edi,edi
   180056c0c:	mov    DWORD PTR [rip+0x230f2a],edi        # 0x180287b3c
   180056c12:	call   0x180051680
   180056c17:	mov    QWORD PTR [rsp+0x30],rdi
   180056c1c:	lea    rcx,[rsp+0x30]
   180056c21:	call   0x1800595b0
   180056c26:	test   eax,eax
   180056c28:	jne    0x180056c39
   180056c2a:	mov    DWORD PTR [rip+0x230f04],0x1        # 0x180287b38
   180056c34:	jmp    0x1800570da
   180056c39:	mov    r9,QWORD PTR [rsp+0x30]
   180056c3e:	mov    rsi,QWORD PTR [rip+0x6e13b]        # 0x1800c4d80
   180056c45:	lea    rbx,[rip+0x6cfc4]        # 0x1800c3c10    ;; "reputation"
   180056c4c:	test   r9,r9
   180056c4f:	je     0x180056d1f
   180056c55:	mov    r8,rdi
   180056c58:	mov    r10,QWORD PTR [rsi+0x9e1518]
   180056c5f:	nop
   180056c60:	imul   rdx,r8,0x68
   180056c64:	add    rdx,r10
   180056c67:	mov    rax,rdi
   180056c6a:	nop    WORD PTR [rax+rax*1+0x0]
   180056c70:	movzx  ecx,BYTE PTR [rdx+rax*1+0x1]
   180056c75:	cmp    cl,BYTE PTR [rbx+rax*1]
   180056c78:	jne    0x180056d13
   180056c7e:	test   cl,cl
   180056c80:	je     0x180056c90
   180056c82:	inc    rax
   180056c85:	cmp    rax,0x17
   180056c89:	jb     0x180056c70
   180056c8b:	jmp    0x180056d13
   180056c90:	cmp    r8,0xffffffffffffffff
   180056c94:	je     0x180056d1f
   180056c9a:	mov    DWORD PTR [rip+0x22a0e0],0x1        # 0x180280d84
   180056ca4:	mov    ebx,DWORD PTR [rdx+0x44]
   180056ca7:	movsxd r8,DWORD PTR [rip+0x6c65a]        # 0x1800c3308
   180056cae:	cmp    ebx,r8d
   180056cb1:	je     0x180056d09
   180056cb3:	mov    DWORD PTR [rdx+0x44],r8d
   180056cb7:	lea    eax,[r8-0x7f000000]
   180056cbe:	cmp    eax,0xb
   180056cc1:	ja     0x180056ce9
   180056cc3:	lea    rax,[r8-0x7f000000]
   180056cca:	lea    rax,[rax+rax*4]
   180056cce:	lea    rcx,[rip+0x15d3fb]        # 0x1801b40d0
   180056cd5:	lea    rcx,[rcx+rax*8]
   180056cd9:	lea    rax,[rip+0x38710]        # 0x18008f3f0    ;; "(blank)"
   180056ce0:	cmp    BYTE PTR [rcx],0x0
   180056ce3:	cmovne rax,rcx
   180056ce7:	jmp    0x180056cf8
   180056ce9:	mov    ecx,r8d
   180056cec:	call   0x180019ef0
   180056cf1:	mov    r8d,DWORD PTR [rip+0x6c610]        # 0x1800c3308
   180056cf8:	mov    r9,rax
   180056cfb:	mov    edx,ebx
   180056cfd:	lea    rcx,[rip+0x4778c]        # 0x18009e490    ;; "  side  label id 0x%X -> 0x%X ("%s") - the tooltip follows our word to where the engine keeps it now"
   180056d04:	call   0x180053ef0
   180056d09:	jmp    0x1800570da
   180056d0e:	jmp    0x1800570da
   180056d13:	inc    r8
   180056d16:	cmp    r8,r9
   180056d19:	jb     0x180056c60
   180056d1f:	mov    DWORD PTR [rip+0x22a05f],edi        # 0x180280d84
   180056d25:	mov    eax,DWORD PTR [rip+0x230e15]        # 0x180287b40
   180056d2b:	inc    eax
   180056d2d:	mov    DWORD PTR [rip+0x230e0d],eax        # 0x180287b40
   180056d33:	cmp    eax,0x3
   180056d36:	jle    0x180056d56
   180056d38:	mov    DWORD PTR [rip+0x230df6],0x1        # 0x180287b38
   180056d42:	mov    rdx,rbx
   180056d45:	lea    rcx,[rip+0x47834]        # 0x18009e580    ;; "  side  three attempts and "%s" is still not in the sidebar - giving up rather than retrying forever"
   180056d4c:	call   0x180053ef0
   180056d51:	jmp    0x1800570da
   180056d56:	mov    rax,QWORD PTR [rsi+0x9e1528]
   180056d5d:	mov    rdx,r9
   180056d60:	cmp    QWORD PTR [rsi+0x9e1520],rax
   180056d67:	jne    0x180056d8a
   180056d69:	mov    DWORD PTR [rip+0x230dc5],0x1        # 0x180287b38
   180056d73:	mov    r8d,0x9e1560
   180056d79:	lea    rcx,[rip+0x47870]        # 0x18009e5f0    ;; "  side  the vector is full (%llu entries) - appending would reallocate it and the selection pointer at exe+0x%X would da"
   180056d80:	call   0x180053ef0
   180056d85:	jmp    0x1800570da
   180056d8a:	test   r9,r9
   180056d8d:	je     0x1800570bf
   180056d93:	lea    r8,[rsi+0x9d4f10]
   180056d9a:	mov    rcx,QWORD PTR [rsi+0x9e1518]
   180056da1:	imul   rax,rdx,0x68
   180056da5:	cmp    QWORD PTR [rcx+rax*1-0x38],r8
   180056daa:	jne    0x1800570b5
   180056db0:	cmp    QWORD PTR [rcx+rax*1-0x20],rdi
   180056db5:	jne    0x1800570b5
   180056dbb:	cmp    QWORD PTR [rcx+rax*1-0x18],rdi
   180056dc0:	jne    0x1800570b5
   180056dc6:	cmp    QWORD PTR [rcx+rax*1-0x10],rdi
   180056dcb:	jne    0x1800570b5
   180056dd1:	cmp    BYTE PTR [rcx+rax*1-0x68],0x1
   180056dd6:	jne    0x1800570b5
   180056ddc:	test   rdx,rdx
   180056ddf:	je     0x1800570bf
   180056de5:	lea    r8,[rdx-0x1]
   180056de9:	imul   rax,r8,0x68
   180056ded:	movups xmm0,XMMWORD PTR [rcx+rax*1]
   180056df1:	movaps XMMWORD PTR [rsp+0x60],xmm0
   180056df6:	movups xmm1,XMMWORD PTR [rcx+rax*1+0x10]
   180056dfb:	movaps XMMWORD PTR [rsp+0x70],xmm1
   180056e00:	movups xmm0,XMMWORD PTR [rcx+rax*1+0x20]
   180056e05:	movaps XMMWORD PTR [rsp+0x80],xmm0
   180056e0d:	movups xmm1,XMMWORD PTR [rcx+rax*1+0x30]
   180056e12:	movaps XMMWORD PTR [rsp+0x90],xmm1
   180056e1a:	movups xmm0,XMMWORD PTR [rcx+rax*1+0x40]
   180056e1f:	movaps XMMWORD PTR [rsp+0xa0],xmm0
   180056e27:	movups xmm1,XMMWORD PTR [rcx+rax*1+0x50]
   180056e2c:	movaps XMMWORD PTR [rsp+0xb0],xmm1
   180056e34:	movsd  xmm0,QWORD PTR [rcx+rax*1+0x60]
   180056e3a:	movsd  QWORD PTR [rsp+0xc0],xmm0
   180056e43:	xorps  xmm1,xmm1
   180056e46:	xor    eax,eax
   180056e48:	movups XMMWORD PTR [rsp+0x61],xmm1
   180056e4d:	mov    QWORD PTR [rsp+0x70],rax
   180056e52:	mov    rcx,rdi
   180056e55:	cmp    BYTE PTR [rip+0x6cdb5],al        # 0x1800c3c10    ;; "reputation"
   180056e5b:	je     0x180056e77
   180056e5d:	nop    DWORD PTR [rax]
   180056e60:	cmp    rcx,0x16
   180056e64:	jae    0x180056e77
   180056e66:	movzx  eax,BYTE PTR [rbx+rcx*1]
   180056e6a:	mov    BYTE PTR [rsp+rcx*1+0x61],al
   180056e6e:	inc    rcx
   180056e71:	cmp    BYTE PTR [rbx+rcx*1],dil
   180056e75:	jne    0x180056e60
   180056e77:	mov    eax,DWORD PTR [rip+0x6c48b]        # 0x1800c3308
   180056e7d:	mov    DWORD PTR [rsp+0xa4],eax
   180056e84:	mov    QWORD PTR [rsp+0xb8],rdi
   180056e8c:	mov    QWORD PTR [rsp+0xb0],rdi
   180056e94:	mov    QWORD PTR [rsp+0xa8],rdi
   180056e9c:	mov    DWORD PTR [rsp+0x28],eax
   180056ea0:	mov    QWORD PTR [rsp+0x20],rbx
   180056ea5:	mov    rdx,rbx
   180056ea8:	lea    rcx,[rip+0x47861]        # 0x18009e710    ;; "  side  appending "%s" - cloned from entry %llu, %llu entries now, icon editor/maintab_%s.png, string id 0x%X"
   180056eaf:	call   0x180053ef0
   180056eb4:	mov    rcx,QWORD PTR [rip+0x6c325]        # 0x1800c31e0
   180056ebb:	cmp    rcx,0xffffffffffffffff
   180056ebf:	je     0x180056ec8
   180056ec1:	call   QWORD PTR [rip+0x361a1]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   180056ec7:	nop
   180056ec8:	mov    rax,QWORD PTR [rip+0x6deb1]        # 0x1800c4d80
   180056ecf:	add    rax,0x459530
   180056ed5:	lea    rdx,[rsp+0x60]
   180056eda:	mov    rcx,QWORD PTR [rsi+0x9e1520]
   180056ee1:	call   rax
   180056ee3:	add    QWORD PTR [rsi+0x9e1520],0x68
   180056eeb:	cmp    QWORD PTR [rip+0x22f1fd],0x0        # 0x1802860f0
   180056ef3:	je     0x180056fb6
   180056ef9:	cmp    QWORD PTR [rip+0x22f1ff],0x0        # 0x180286100
   180056f01:	je     0x180056fb6
   180056f07:	mov    QWORD PTR [rsp+0x20],rbx
   180056f0c:	lea    r9,[rip+0x478c5]        # 0x18009e7d8    ;; "editor/maintab_%s.png"
   180056f13:	mov    edx,0x60
   180056f18:	mov    r8,0xffffffffffffffff
   180056f1f:	lea    rcx,[rip+0x4879ea]        # 0x1804de910
   180056f26:	call   0x180069a50
   180056f2b:	call   0x18003ae90
   180056f30:	movaps xmm6,xmm0
   180056f33:	lea    rcx,[rip+0x4879d6]        # 0x1804de910
   180056f3a:	call   0x1800594c0
   180056f3f:	mov    rsi,rax
   180056f42:	mov    QWORD PTR [rsp+0x30],rdi
   180056f47:	test   rax,rax
   180056f4a:	je     0x180056fa1
   180056f4c:	lea    rcx,[rsp+0x30]
   180056f51:	call   0x1800595b0
   180056f56:	test   eax,eax
   180056f58:	je     0x180056fa1
   180056f5a:	mov    rcx,QWORD PTR [rsp+0x30]
   180056f5f:	test   rcx,rcx
   180056f62:	je     0x180056fa1
   180056f64:	imul   r8,rcx,0x68
   180056f68:	mov    rax,QWORD PTR [rip+0x6de11]        # 0x1800c4d80
   180056f6f:	mov    rcx,QWORD PTR [rax+0x9e1518]
   180056f76:	mov    QWORD PTR [rcx+r8*1-0x8],rsi
   180056f7b:	call   0x18003ae90
   180056f80:	subsd  xmm0,xmm6
   180056f84:	movaps xmm2,xmm0
   180056f87:	movq   r8,xmm0
   180056f8c:	lea    rdx,[rip+0x48797d]        # 0x1804de910
   180056f93:	lea    rcx,[rip+0x47856]        # 0x18009e7f0    ;; "  side  icon %s loaded on its own in %.1f ms - not through the game's whole interface pass, which took two seconds (step"
   180056f9a:	call   0x180053ef0
   180056f9f:	jmp    0x180056fc2
   180056fa1:	lea    rdx,[rip+0x487968]        # 0x1804de910
   180056fa8:	lea    rcx,[rip+0x478c1]        # 0x18009e870    ;; "  side  icon %s did not load - the entry keeps the picture it was cloned with"
   180056faf:	call   0x180053ef0
   180056fb4:	jmp    0x180056fc2
   180056fb6:	lea    rcx,[rip+0x4517b]        # 0x18009c138    ;; "sidebar"
   180056fbd:	call   0x180057d00
   180056fc2:	mov    QWORD PTR [rsp+0x30],rdi
   180056fc7:	lea    rcx,[rsp+0x30]
   180056fcc:	call   0x1800595b0
   180056fd1:	test   eax,eax
   180056fd3:	je     0x18005704c
   180056fd5:	mov    rax,QWORD PTR [rsp+0x30]
   180056fda:	test   rax,rax
   180056fdd:	je     0x18005704c
   180056fdf:	mov    rcx,QWORD PTR [rip+0x6dd9a]        # 0x1800c4d80
   180056fe6:	mov    r8,QWORD PTR [rcx+0x9e1518]
   180056fed:	imul   rax,rax,0x68
   180056ff1:	cmp    QWORD PTR [r8+rax*1-0x8],0x0
   180056ff7:	jne    0x18005704c
   180056ff9:	mov    rdx,QWORD PTR [rcx+0x9e1520]
   180057000:	mov    rax,rdx
   180057003:	sub    rax,r8
   180057006:	cmp    rax,0x68
   18005700a:	jl     0x180057017
   18005700c:	lea    rax,[rdx-0x68]
   180057010:	mov    QWORD PTR [rcx+0x9e1520],rax
   180057017:	mov    DWORD PTR [rip+0x230b17],0x1        # 0x180287b38
   180057021:	mov    rdx,rbx
   180057024:	lea    rcx,[rip+0x47895]        # 0x18009e8c0    ;; "  side  "%s" has no texture - taken back out before the game could draw it."
   18005702b:	call   0x180053ef0
   180057030:	mov    rcx,QWORD PTR [rip+0x6c1a9]        # 0x1800c31e0
   180057037:	cmp    rcx,0xffffffffffffffff
   18005703b:	je     0x1800570da
   180057041:	call   QWORD PTR [rip+0x36021]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   180057047:	jmp    0x1800570da
   18005704c:	mov    DWORD PTR [rip+0x229d2e],0x1        # 0x180280d84
   180057056:	mov    DWORD PTR [rip+0x230ae4],edi        # 0x180287b40
   18005705c:	mov    rdx,rbx
   18005705f:	lea    rcx,[rip+0x478aa]        # 0x18009e910    ;; "  side  "%s" is in the sidebar"
   180057066:	call   0x180053ef0
   18005706b:	mov    rcx,QWORD PTR [rip+0x6c16e]        # 0x1800c31e0
   180057072:	cmp    rcx,0xffffffffffffffff
   180057076:	je     0x18005707e
   180057078:	call   QWORD PTR [rip+0x35fea]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   18005707e:	call   0x1800583e0
   180057083:	jmp    0x1800570da
   180057085:	mov    DWORD PTR [rip+0x230aa9],0x1        # 0x180287b38
   18005708f:	mov    edx,0x459530
   180057094:	lea    rcx,[rip+0x476fd]        # 0x18009e798    ;; "  side  the copy-construct at 0x%X faulted - entry NOT added"
   18005709b:	call   0x180053ef0
   1800570a0:	mov    rcx,QWORD PTR [rip+0x6c139]        # 0x1800c31e0
   1800570a7:	cmp    rcx,0xffffffffffffffff
   1800570ab:	je     0x1800570b3
   1800570ad:	call   QWORD PTR [rip+0x35fb5]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   1800570b3:	jmp    0x1800570da
   1800570b5:	sub    rdx,0x1
   1800570b9:	jne    0x180056da1
   1800570bf:	mov    DWORD PTR [rip+0x230a6f],0x1        # 0x180287b38
   1800570c9:	mov    edx,0x30
   1800570ce:	lea    rcx,[rip+0x475ab]        # 0x18009e680    ;; "  side  no entry has both a null sub-list and a g_ui back-pointer at +0x%02X - the layout is not what section 8c describ"
   1800570d5:	call   0x180053ef0
   1800570da:	mov    rcx,QWORD PTR [rsp+0xd0]
   1800570e2:	xor    rcx,rsp
   1800570e5:	call   0x180069e90
   1800570ea:	lea    r11,[rsp+0xf0]
   1800570f2:	mov    rbx,QWORD PTR [r11+0x10]
   1800570f6:	mov    rsi,QWORD PTR [r11+0x18]
   1800570fa:	movaps xmm6,XMMWORD PTR [r11-0x10]
   1800570ff:	mov    rsp,r11
   180057102:	pop    rdi
   180057103:	ret
