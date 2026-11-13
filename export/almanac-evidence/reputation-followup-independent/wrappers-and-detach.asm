GNU objdump independent static decode; DLL SHA-256 fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a  Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180068a20 <.text+0x67a20>:
   180068a20:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   180068a25:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
   180068a2a:	57                   	push   rdi
   180068a2b:	48 83 ec 40          	sub    rsp,0x40
   180068a2f:	48 8b fa             	mov    rdi,rdx
   180068a32:	48 8b f1             	mov    rsi,rcx
   180068a35:	ff 15 95 f3 11 00    	call   QWORD PTR [rip+0x11f395]        # 0x180187dd0
   180068a3b:	48 8b d8             	mov    rbx,rax
   180068a3e:	48 89 44 24 60       	mov    QWORD PTR [rsp+0x60],rax
   180068a43:	48 85 c0             	test   rax,rax
   180068a46:	74 1c                	je     0x180068a64
   180068a48:	48 8b d7             	mov    rdx,rdi
   180068a4b:	48 8b ce             	mov    rcx,rsi
   180068a4e:	e8 8d 79 fc ff       	call   0x1800303e0
   180068a53:	eb 0f                	jmp    0x180068a64
   180068a55:	c7 05 31 a6 05 00 00 	mov    DWORD PTR [rip+0x5a631],0x0        # 0x1800c3090
   180068a5c:	00 00 00
   180068a5f:	48 8b 5c 24 60       	mov    rbx,QWORD PTR [rsp+0x60]
   180068a64:	48 8b c3             	mov    rax,rbx
   180068a67:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   180068a6c:	48 8b 74 24 58       	mov    rsi,QWORD PTR [rsp+0x58]
   180068a71:	48 83 c4 40          	add    rsp,0x40
   180068a75:	5f                   	pop    rdi
   180068a76:	c3                   	ret
   180068a77:	cc                   	int3
   180068a78:	cc                   	int3
   180068a79:	cc                   	int3
   180068a7a:	cc                   	int3
   180068a7b:	cc                   	int3
   180068a7c:	cc                   	int3
   180068a7d:	cc                   	int3
   180068a7e:	cc                   	int3
   180068a7f:	cc                   	int3
   180068a80:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   180068a85:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
   180068a8a:	57                   	push   rdi
   180068a8b:	48 83 ec 40          	sub    rsp,0x40
   180068a8f:	48 8b fa             	mov    rdi,rdx
   180068a92:	48 8b f1             	mov    rsi,rcx
   180068a95:	ff 15 2d f3 11 00    	call   QWORD PTR [rip+0x11f32d]        # 0x180187dc8
   180068a9b:	48 8b d8             	mov    rbx,rax
   180068a9e:	48 89 44 24 60       	mov    QWORD PTR [rsp+0x60],rax
   180068aa3:	48 85 c0             	test   rax,rax
   180068aa6:	74 1c                	je     0x180068ac4
   180068aa8:	48 8b d7             	mov    rdx,rdi
   180068aab:	48 8b ce             	mov    rcx,rsi
   180068aae:	e8 2d 79 fc ff       	call   0x1800303e0
   180068ab3:	eb 0f                	jmp    0x180068ac4
   180068ab5:	c7 05 d1 a5 05 00 00 	mov    DWORD PTR [rip+0x5a5d1],0x0        # 0x1800c3090
   180068abc:	00 00 00
   180068abf:	48 8b 5c 24 60       	mov    rbx,QWORD PTR [rsp+0x60]
   180068ac4:	48 8b c3             	mov    rax,rbx
   180068ac7:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   180068acc:	48 8b 74 24 58       	mov    rsi,QWORD PTR [rsp+0x58]
   180068ad1:	48 83 c4 40          	add    rsp,0x40
   180068ad5:	5f                   	pop    rdi
   180068ad6:	c3                   	ret
   180068ad7:	cc                   	int3
   180068ad8:	cc                   	int3
   180068ad9:	cc                   	int3
   180068ada:	cc                   	int3
   180068adb:	cc                   	int3
   180068adc:	cc                   	int3
   180068add:	cc                   	int3
   180068ade:	cc                   	int3
   180068adf:	cc                   	int3
   180068ae0:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   180068ae5:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
   180068aea:	57                   	push   rdi
   180068aeb:	48 83 ec 40          	sub    rsp,0x40
   180068aef:	49 8b f8             	mov    rdi,r8
   180068af2:	48 8b f2             	mov    rsi,rdx
   180068af5:	ff 15 e5 f2 11 00    	call   QWORD PTR [rip+0x11f2e5]        # 0x180187de0
   180068afb:	8b d8                	mov    ebx,eax
   180068afd:	89 44 24 68          	mov    DWORD PTR [rsp+0x68],eax
   180068b01:	85 c0                	test   eax,eax
   180068b03:	75 1b                	jne    0x180068b20
   180068b05:	48 8b d7             	mov    rdx,rdi
   180068b08:	48 8b ce             	mov    rcx,rsi
   180068b0b:	e8 d0 78 fc ff       	call   0x1800303e0
   180068b10:	eb 0e                	jmp    0x180068b20
   180068b12:	c7 05 74 a5 05 00 00 	mov    DWORD PTR [rip+0x5a574],0x0        # 0x1800c3090
   180068b19:	00 00 00
   180068b1c:	8b 5c 24 68          	mov    ebx,DWORD PTR [rsp+0x68]
   180068b20:	8b c3                	mov    eax,ebx
   180068b22:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   180068b27:	48 8b 74 24 58       	mov    rsi,QWORD PTR [rsp+0x58]
   180068b2c:	48 83 c4 40          	add    rsp,0x40
   180068b30:	5f                   	pop    rdi
   180068b31:	c3                   	ret
   180068b32:	cc                   	int3
   180068b33:	cc                   	int3
   180068b34:	cc                   	int3
   180068b35:	cc                   	int3
   180068b36:	cc                   	int3
   180068b37:	cc                   	int3
   180068b38:	cc                   	int3
   180068b39:	cc                   	int3
   180068b3a:	cc                   	int3
   180068b3b:	cc                   	int3
   180068b3c:	cc                   	int3
   180068b3d:	cc                   	int3
   180068b3e:	cc                   	int3
   180068b3f:	cc                   	int3
   180068b40:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   180068b45:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
   180068b4a:	57                   	push   rdi
   180068b4b:	48 83 ec 40          	sub    rsp,0x40
   180068b4f:	49 8b f8             	mov    rdi,r8
   180068b52:	48 8b f2             	mov    rsi,rdx
   180068b55:	ff 15 7d f2 11 00    	call   QWORD PTR [rip+0x11f27d]        # 0x180187dd8
   180068b5b:	8b d8                	mov    ebx,eax
   180068b5d:	89 44 24 68          	mov    DWORD PTR [rsp+0x68],eax
   180068b61:	85 c0                	test   eax,eax
   180068b63:	75 1b                	jne    0x180068b80
   180068b65:	48 8b d7             	mov    rdx,rdi
   180068b68:	48 8b ce             	mov    rcx,rsi
   180068b6b:	e8 70 78 fc ff       	call   0x1800303e0
   180068b70:	eb 0e                	jmp    0x180068b80
   180068b72:	c7 05 14 a5 05 00 00 	mov    DWORD PTR [rip+0x5a514],0x0        # 0x1800c3090
   180068b79:	00 00 00
   180068b7c:	8b 5c 24 68          	mov    ebx,DWORD PTR [rsp+0x68]
   180068b80:	8b c3                	mov    eax,ebx
   180068b82:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   180068b87:	48 8b 74 24 58       	mov    rsi,QWORD PTR [rsp+0x58]
   180068b8c:	48 83 c4 40          	add    rsp,0x40
   180068b90:	5f                   	pop    rdi
   180068b91:	c3                   	ret
   180068b92:	cc                   	int3
   180068b93:	cc                   	int3
   180068b94:	cc                   	int3
   180068b95:	cc                   	int3
   180068b96:	cc                   	int3
   180068b97:	cc                   	int3
   180068b98:	cc                   	int3
   180068b99:	cc                   	int3
   180068b9a:	cc                   	int3
   180068b9b:	cc                   	int3
   180068b9c:	cc                   	int3
   180068b9d:	cc                   	int3
   180068b9e:	cc                   	int3
   180068b9f:	cc                   	int3
   180068ba0:	48 83 ec 28          	sub    rsp,0x28
   180068ba4:	85 d2                	test   edx,edx
   180068ba6:	75 29                	jne    0x180068bd1
   180068ba8:	33 d2                	xor    edx,edx
   180068baa:	8b c2                	mov    eax,edx
   180068bac:	89 15 ce b2 05 00    	mov    DWORD PTR [rip+0x5b2ce],edx        # 0x1800c3e80
   180068bb2:	87 05 a4 03 3e 00    	xchg   DWORD PTR [rip+0x3e03a4],eax        # 0x180448f5c
   180068bb8:	48 8b 0d 71 03 3e 00 	mov    rcx,QWORD PTR [rip+0x3e0371]        # 0x180448f30
   180068bbf:	48 89 15 6a 03 3e 00 	mov    QWORD PTR [rip+0x3e036a],rdx        # 0x180448f30
   180068bc6:	48 85 c9             	test   rcx,rcx
   180068bc9:	74 06                	je     0x180068bd1
   180068bcb:	ff 15 17 47 02 00    	call   QWORD PTR [rip+0x24717]        # 0x18008d2e8
   180068bd1:	b8 01 00 00 00       	mov    eax,0x1
   180068bd6:	48 83 c4 28          	add    rsp,0x28
   180068bda:	c3                   	ret
   180068bdb:	cc                   	int3
