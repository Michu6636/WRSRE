
scratch/reputation-static/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180057e40 <.text+0x56e40>:
   180057e40:	40 53                	rex push rbx
   180057e42:	56                   	push   rsi
   180057e43:	57                   	push   rdi
   180057e44:	41 54                	push   r12
   180057e46:	41 55                	push   r13
   180057e48:	41 56                	push   r14
   180057e4a:	41 57                	push   r15
   180057e4c:	48 81 ec b0 00 00 00 	sub    rsp,0xb0
   180057e53:	48 8b 05 26 c3 06 00 	mov    rax,QWORD PTR [rip+0x6c326]        # 0x1800c4180
   180057e5a:	48 33 c4             	xor    rax,rsp
   180057e5d:	48 89 84 24 a0 00 00 	mov    QWORD PTR [rsp+0xa0],rax
   180057e64:	00
   180057e65:	83 3d e0 fc 22 00 00 	cmp    DWORD PTR [rip+0x22fce0],0x0        # 0x180287b4c
   180057e6c:	0f 85 6e 04 00 00    	jne    0x1800582e0
   180057e72:	83 3d cf fc 22 00 00 	cmp    DWORD PTR [rip+0x22fccf],0x0        # 0x180287b48
   180057e79:	0f 84 61 04 00 00    	je     0x1800582e0
   180057e7f:	83 3d fa 8e 22 00 00 	cmp    DWORD PTR [rip+0x228efa],0x0        # 0x180280d80
   180057e86:	0f 84 54 04 00 00    	je     0x1800582e0
   180057e8c:	83 3d 91 8c 22 00 00 	cmp    DWORD PTR [rip+0x228c91],0x0        # 0x180280b24
   180057e93:	75 1b                	jne    0x180057eb0
   180057e95:	c7 05 ad fc 22 00 01 	mov    DWORD PTR [rip+0x22fcad],0x1        # 0x180287b4c
   180057e9c:	00 00 00
   180057e9f:	48 8d 0d 2a 63 04 00 	lea    rcx,[rip+0x4632a]        # 0x18009e1d0
   180057ea6:	e8 45 c0 ff ff       	call   0x180053ef0
   180057eab:	e9 30 04 00 00       	jmp    0x1800582e0
   180057eb0:	e8 db c8 00 00       	call   0x180064790
   180057eb5:	48 85 c0             	test   rax,rax
   180057eb8:	75 0d                	jne    0x180057ec7
   180057eba:	33 ff                	xor    edi,edi
   180057ebc:	89 3d 8e fc 22 00    	mov    DWORD PTR [rip+0x22fc8e],edi        # 0x180287b50
   180057ec2:	e9 19 04 00 00       	jmp    0x1800582e0
   180057ec7:	8b 05 83 fc 22 00    	mov    eax,DWORD PTR [rip+0x22fc83]        # 0x180287b50
   180057ecd:	ff c0                	inc    eax
   180057ecf:	89 05 7b fc 22 00    	mov    DWORD PTR [rip+0x22fc7b],eax        # 0x180287b50
   180057ed5:	3d f0 00 00 00       	cmp    eax,0xf0
   180057eda:	0f 8c 00 04 00 00    	jl     0x1800582e0
   180057ee0:	33 ff                	xor    edi,edi
   180057ee2:	89 3d 68 fc 22 00    	mov    DWORD PTR [rip+0x22fc68],edi        # 0x180287b50
   180057ee8:	48 89 7c 24 48       	mov    QWORD PTR [rsp+0x48],rdi
   180057eed:	48 8d 4c 24 48       	lea    rcx,[rsp+0x48]
   180057ef2:	e8 b9 16 00 00       	call   0x1800595b0
   180057ef7:	85 c0                	test   eax,eax
   180057ef9:	0f 84 d7 03 00 00    	je     0x1800582d6
   180057eff:	4c 8b 44 24 48       	mov    r8,QWORD PTR [rsp+0x48]
   180057f04:	4c 8d 35 1d bd 06 00 	lea    r14,[rip+0x6bd1d]        # 0x1800c3c28
   180057f0b:	4d 85 c0             	test   r8,r8
   180057f0e:	74 40                	je     0x180057f50
   180057f10:	8b d7                	mov    edx,edi
   180057f12:	48 8b 05 67 ce 06 00 	mov    rax,QWORD PTR [rip+0x6ce67]        # 0x1800c4d80
   180057f19:	4c 8b 88 18 15 9e 00 	mov    r9,QWORD PTR [rax+0x9e1518]
   180057f20:	4c 6b e2 68          	imul   r12,rdx,0x68
   180057f24:	4b 8d 1c 0c          	lea    rbx,[r12+r9*1]
   180057f28:	48 8b c7             	mov    rax,rdi
   180057f2b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
   180057f30:	0f b6 4c 03 01       	movzx  ecx,BYTE PTR [rbx+rax*1+0x1]
   180057f35:	41 3a 0c 06          	cmp    cl,BYTE PTR [r14+rax*1]
   180057f39:	75 0d                	jne    0x180057f48
   180057f3b:	84 c9                	test   cl,cl
   180057f3d:	74 2f                	je     0x180057f6e
   180057f3f:	48 ff c0             	inc    rax
   180057f42:	48 83 f8 17          	cmp    rax,0x17
   180057f46:	72 e8                	jb     0x180057f30
   180057f48:	48 ff c2             	inc    rdx
   180057f4b:	49 3b d0             	cmp    rdx,r8
   180057f4e:	72 d0                	jb     0x180057f20
   180057f50:	c7 05 f2 fb 22 00 01 	mov    DWORD PTR [rip+0x22fbf2],0x1        # 0x180287b4c
   180057f57:	00 00 00
   180057f5a:	49 8b d6             	mov    rdx,r14
   180057f5d:	48 8d 0d d4 62 04 00 	lea    rcx,[rip+0x462d4]        # 0x18009e238
   180057f64:	e8 87 bf ff ff       	call   0x180053ef0
   180057f69:	e9 72 03 00 00       	jmp    0x1800582e0
   180057f6e:	48 83 fa ff          	cmp    rdx,0xffffffffffffffff
   180057f72:	74 dc                	je     0x180057f50
   180057f74:	4c 8d 7b 48          	lea    r15,[rbx+0x48]
   180057f78:	ba 18 00 00 00       	mov    edx,0x18
   180057f7d:	49 8b cf             	mov    rcx,r15
   180057f80:	48 8b 05 e9 cd 06 00 	mov    rax,QWORD PTR [rip+0x6cde9]        # 0x1800c4d70
   180057f87:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057f8a:	85 c0                	test   eax,eax
   180057f8c:	0f 84 29 03 00 00    	je     0x1800582bb
   180057f92:	49 8b 0f             	mov    rcx,QWORD PTR [r15]
   180057f95:	48 85 c9             	test   rcx,rcx
   180057f98:	0f 84 1d 03 00 00    	je     0x1800582bb
   180057f9e:	4c 8b 43 50          	mov    r8,QWORD PTR [rbx+0x50]
   180057fa2:	4c 3b c1             	cmp    r8,rcx
   180057fa5:	0f 82 10 03 00 00    	jb     0x1800582bb
   180057fab:	4c 2b c1             	sub    r8,rcx
   180057fae:	0f 84 e6 02 00 00    	je     0x18005829a
   180057fb4:	49 bd cd cc cc cc cc 	movabs r13,0xcccccccccccccccd
   180057fbb:	cc cc cc
   180057fbe:	49 8b c5             	mov    rax,r13
   180057fc1:	49 f7 e0             	mul    r8
   180057fc4:	48 8b f2             	mov    rsi,rdx
   180057fc7:	48 c1 ee 06          	shr    rsi,0x6
   180057fcb:	48 8d 14 b6          	lea    rdx,[rsi+rsi*4]
   180057fcf:	48 c1 e2 04          	shl    rdx,0x4
   180057fd3:	4c 3b c2             	cmp    r8,rdx
   180057fd6:	0f 85 be 02 00 00    	jne    0x18005829a
   180057fdc:	49 81 f8 00 00 01 00 	cmp    r8,0x10000
   180057fe3:	0f 87 b1 02 00 00    	ja     0x18005829a
   180057fe9:	49 8b d0             	mov    rdx,r8
   180057fec:	48 8b 05 7d cd 06 00 	mov    rax,QWORD PTR [rip+0x6cd7d]        # 0x1800c4d70
   180057ff3:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180057ff6:	85 c0                	test   eax,eax
   180057ff8:	0f 84 d8 02 00 00    	je     0x1800582d6
   180057ffe:	48 8b d7             	mov    rdx,rdi
   180058001:	48 8d 1d 38 bc 06 00 	lea    rbx,[rip+0x6bc38]        # 0x1800c3c40
   180058008:	48 85 f6             	test   rsi,rsi
   18005800b:	74 36                	je     0x180058043
   18005800d:	0f 1f 00             	nop    DWORD PTR [rax]
   180058010:	48 8b c7             	mov    rax,rdi
   180058013:	4c 8d 04 92          	lea    r8,[rdx+rdx*4]
   180058017:	49 c1 e0 04          	shl    r8,0x4
   18005801b:	4d 03 07             	add    r8,QWORD PTR [r15]
   18005801e:	66 90                	xchg   ax,ax
   180058020:	42 0f b6 0c 00       	movzx  ecx,BYTE PTR [rax+r8*1]
   180058025:	3a 0c 03             	cmp    cl,BYTE PTR [rbx+rax*1]
   180058028:	75 11                	jne    0x18005803b
   18005802a:	84 c9                	test   cl,cl
   18005802c:	0f 84 5e 01 00 00    	je     0x180058190
   180058032:	48 ff c0             	inc    rax
   180058035:	48 83 f8 18          	cmp    rax,0x18
   180058039:	72 e5                	jb     0x180058020
   18005803b:	48 ff c2             	inc    rdx
   18005803e:	48 3b d6             	cmp    rdx,rsi
   180058041:	72 cd                	jb     0x180058010
   180058043:	0f 57 c0             	xorps  xmm0,xmm0
   180058046:	0f 11 44 24 50       	movups XMMWORD PTR [rsp+0x50],xmm0
   18005804b:	0f 11 44 24 60       	movups XMMWORD PTR [rsp+0x60],xmm0
   180058050:	0f 11 44 24 70       	movups XMMWORD PTR [rsp+0x70],xmm0
   180058055:	0f 11 84 24 80 00 00 	movups XMMWORD PTR [rsp+0x80],xmm0
   18005805c:	00
   18005805d:	0f 11 84 24 90 00 00 	movups XMMWORD PTR [rsp+0x90],xmm0
   180058064:	00
   180058065:	40 38 3d d4 bb 06 00 	cmp    BYTE PTR [rip+0x6bbd4],dil        # 0x1800c3c40
   18005806c:	74 19                	je     0x180058087
   18005806e:	66 90                	xchg   ax,ax
   180058070:	48 83 ff 17          	cmp    rdi,0x17
   180058074:	73 11                	jae    0x180058087
   180058076:	0f b6 04 3b          	movzx  eax,BYTE PTR [rbx+rdi*1]
   18005807a:	88 44 3c 50          	mov    BYTE PTR [rsp+rdi*1+0x50],al
   18005807e:	48 ff c7             	inc    rdi
   180058081:	80 3c 3b 00          	cmp    BYTE PTR [rbx+rdi*1],0x0
   180058085:	75 e9                	jne    0x180058070
   180058087:	8b 05 cb bb 06 00    	mov    eax,DWORD PTR [rip+0x6bbcb]        # 0x1800c3c58
   18005808d:	89 84 24 90 00 00 00 	mov    DWORD PTR [rsp+0x90],eax
   180058094:	48 89 5c 24 28       	mov    QWORD PTR [rsp+0x28],rbx
   180058099:	4c 89 74 24 20       	mov    QWORD PTR [rsp+0x20],r14
   18005809e:	4c 8b ce             	mov    r9,rsi
   1800580a1:	4d 8b c6             	mov    r8,r14
   1800580a4:	48 8b d3             	mov    rdx,rbx
   1800580a7:	48 8d 0d 72 62 04 00 	lea    rcx,[rip+0x46272]        # 0x18009e320
   1800580ae:	e8 3d be ff ff       	call   0x180053ef0
   1800580b3:	48 8b 0d 26 b1 06 00 	mov    rcx,QWORD PTR [rip+0x6b126]        # 0x1800c31e0
   1800580ba:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1800580be:	74 07                	je     0x1800580c7
   1800580c0:	ff 15 a2 4f 03 00    	call   QWORD PTR [rip+0x34fa2]        # 0x18008d068
   1800580c6:	90                   	nop
   1800580c7:	48 8b 05 b2 cc 06 00 	mov    rax,QWORD PTR [rip+0x6ccb2]        # 0x1800c4d80
   1800580ce:	48 05 f0 92 44 00    	add    rax,0x4492f0
   1800580d4:	48 8d 54 24 50       	lea    rdx,[rsp+0x50]
   1800580d9:	49 8b cf             	mov    rcx,r15
   1800580dc:	ff d0                	call   rax
   1800580de:	90                   	nop
   1800580df:	48 83 3d 09 e0 22 00 	cmp    QWORD PTR [rip+0x22e009],0x0        # 0x1802860f0
   1800580e6:	00
   1800580e7:	0f 84 ae 00 00 00    	je     0x18005819b
   1800580ed:	48 83 3d 0b e0 22 00 	cmp    QWORD PTR [rip+0x22e00b],0x0        # 0x180286100
   1800580f4:	00
   1800580f5:	0f 84 a0 00 00 00    	je     0x18005819b
   1800580fb:	48 89 5c 24 28       	mov    QWORD PTR [rsp+0x28],rbx
   180058100:	4c 89 74 24 20       	mov    QWORD PTR [rsp+0x20],r14
   180058105:	4c 8d 0d b4 62 04 00 	lea    r9,[rip+0x462b4]        # 0x18009e3c0
   18005810c:	ba 60 00 00 00       	mov    edx,0x60
   180058111:	49 c7 c0 ff ff ff ff 	mov    r8,0xffffffffffffffff
   180058118:	48 8d 0d 91 67 48 00 	lea    rcx,[rip+0x486791]        # 0x1804de8b0
   18005811f:	e8 2c 19 01 00       	call   0x180069a50
   180058124:	48 8d 0d 85 67 48 00 	lea    rcx,[rip+0x486785]        # 0x1804de8b0
   18005812b:	e8 90 13 00 00       	call   0x1800594c0
   180058130:	4c 8b f8             	mov    r15,rax
   180058133:	48 85 c0             	test   rax,rax
   180058136:	74 6f                	je     0x1800581a7
   180058138:	48 8b 0d 41 cc 06 00 	mov    rcx,QWORD PTR [rip+0x6cc41]        # 0x1800c4d80
   18005813f:	48 8b b1 18 15 9e 00 	mov    rsi,QWORD PTR [rcx+0x9e1518]
   180058146:	4a 8d 3c 26          	lea    rdi,[rsi+r12*1]
   18005814a:	ba 18 00 00 00       	mov    edx,0x18
   18005814f:	48 8d 4f 48          	lea    rcx,[rdi+0x48]
   180058153:	4c 8b 05 16 cc 06 00 	mov    r8,QWORD PTR [rip+0x6cc16]        # 0x1800c4d70
   18005815a:	41 ff 50 60          	call   QWORD PTR [r8+0x60]
   18005815e:	85 c0                	test   eax,eax
   180058160:	74 45                	je     0x1800581a7
   180058162:	48 8b 4f 48          	mov    rcx,QWORD PTR [rdi+0x48]
   180058166:	48 85 c9             	test   rcx,rcx
   180058169:	74 3c                	je     0x1800581a7
   18005816b:	4a 8b 54 26 50       	mov    rdx,QWORD PTR [rsi+r12*1+0x50]
   180058170:	48 3b d1             	cmp    rdx,rcx
   180058173:	76 32                	jbe    0x1800581a7
   180058175:	48 2b d1             	sub    rdx,rcx
   180058178:	49 8b c5             	mov    rax,r13
   18005817b:	48 f7 e2             	mul    rdx
   18005817e:	48 c1 ea 06          	shr    rdx,0x6
   180058182:	48 8d 04 92          	lea    rax,[rdx+rdx*4]
   180058186:	48 03 c0             	add    rax,rax
   180058189:	4c 89 7c c1 f8       	mov    QWORD PTR [rcx+rax*8-0x8],r15
   18005818e:	eb 17                	jmp    0x1800581a7
   180058190:	89 15 c6 ba 06 00    	mov    DWORD PTR [rip+0x6bac6],edx        # 0x1800c3c5c
   180058196:	e9 45 01 00 00       	jmp    0x1800582e0
   18005819b:	48 8d 0d 32 62 04 00 	lea    rcx,[rip+0x46232]        # 0x18009e3d4
   1800581a2:	e8 59 fb ff ff       	call   0x180057d00
   1800581a7:	48 8b 05 d2 cb 06 00 	mov    rax,QWORD PTR [rip+0x6cbd2]        # 0x1800c4d80
   1800581ae:	48 8b b8 18 15 9e 00 	mov    rdi,QWORD PTR [rax+0x9e1518]
   1800581b5:	4a 8d 34 27          	lea    rsi,[rdi+r12*1]
   1800581b9:	ba 18 00 00 00       	mov    edx,0x18
   1800581be:	48 8d 4e 48          	lea    rcx,[rsi+0x48]
   1800581c2:	48 8b 05 a7 cb 06 00 	mov    rax,QWORD PTR [rip+0x6cba7]        # 0x1800c4d70
   1800581c9:	ff 50 60             	call   QWORD PTR [rax+0x60]
   1800581cc:	85 c0                	test   eax,eax
   1800581ce:	0f 84 0c 01 00 00    	je     0x1800582e0
   1800581d4:	4c 8b 4e 48          	mov    r9,QWORD PTR [rsi+0x48]
   1800581d8:	4d 85 c9             	test   r9,r9
   1800581db:	0f 84 ff 00 00 00    	je     0x1800582e0
   1800581e1:	4e 8b 54 27 50       	mov    r10,QWORD PTR [rdi+r12*1+0x50]
   1800581e6:	4d 3b d1             	cmp    r10,r9
   1800581e9:	0f 86 f1 00 00 00    	jbe    0x1800582e0
   1800581ef:	49 8b ca             	mov    rcx,r10
   1800581f2:	49 2b c9             	sub    rcx,r9
   1800581f5:	49 8b c5             	mov    rax,r13
   1800581f8:	48 f7 e1             	mul    rcx
   1800581fb:	48 c1 ea 06          	shr    rdx,0x6
   1800581ff:	48 8d 04 92          	lea    rax,[rdx+rdx*4]
   180058203:	48 03 c0             	add    rax,rax
   180058206:	49 83 7c c1 f8 00    	cmp    QWORD PTR [r9+rax*8-0x8],0x0
   18005820c:	75 2a                	jne    0x180058238
   18005820e:	49 8d 42 b0          	lea    rax,[r10-0x50]
   180058212:	4a 89 44 27 50       	mov    QWORD PTR [rdi+r12*1+0x50],rax
   180058217:	c7 05 2b f9 22 00 01 	mov    DWORD PTR [rip+0x22f92b],0x1        # 0x180287b4c
   18005821e:	00 00 00
   180058221:	4c 8b cb             	mov    r9,rbx
   180058224:	4d 8b c6             	mov    r8,r14
   180058227:	48 8b d3             	mov    rdx,rbx
   18005822a:	48 8d 0d af 61 04 00 	lea    rcx,[rip+0x461af]        # 0x18009e3e0
   180058231:	e8 ba bc ff ff       	call   0x180053ef0
   180058236:	eb 1d                	jmp    0x180058255
   180058238:	44 8d 42 ff          	lea    r8d,[rdx-0x1]
   18005823c:	44 89 05 19 ba 06 00 	mov    DWORD PTR [rip+0x6ba19],r8d        # 0x1800c3c5c
   180058243:	4d 8b ce             	mov    r9,r14
   180058246:	48 8b d3             	mov    rdx,rbx
   180058249:	48 8d 0d 08 62 04 00 	lea    rcx,[rip+0x46208]        # 0x18009e458
   180058250:	e8 9b bc ff ff       	call   0x180053ef0
   180058255:	48 8b 0d 84 af 06 00 	mov    rcx,QWORD PTR [rip+0x6af84]        # 0x1800c31e0
   18005825c:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180058260:	74 7e                	je     0x1800582e0
   180058262:	ff 15 00 4e 03 00    	call   QWORD PTR [rip+0x34e00]        # 0x18008d068
   180058268:	eb 76                	jmp    0x1800582e0
   18005826a:	c7 05 d8 f8 22 00 01 	mov    DWORD PTR [rip+0x22f8d8],0x1        # 0x180287b4c
   180058271:	00 00 00
   180058274:	ba f0 92 44 00       	mov    edx,0x4492f0
   180058279:	48 8d 0d 08 61 04 00 	lea    rcx,[rip+0x46108]        # 0x18009e388
   180058280:	e8 6b bc ff ff       	call   0x180053ef0
   180058285:	48 8b 0d 54 af 06 00 	mov    rcx,QWORD PTR [rip+0x6af54]        # 0x1800c31e0
   18005828c:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180058290:	74 06                	je     0x180058298
   180058292:	ff 15 d0 4d 03 00    	call   QWORD PTR [rip+0x34dd0]        # 0x18008d068
   180058298:	eb 46                	jmp    0x1800582e0
   18005829a:	c7 05 a8 f8 22 00 01 	mov    DWORD PTR [rip+0x22f8a8],0x1        # 0x180287b4c
   1800582a1:	00 00 00
   1800582a4:	41 b9 50 00 00 00    	mov    r9d,0x50
   1800582aa:	49 8b d6             	mov    rdx,r14
   1800582ad:	48 8d 0d fc 5f 04 00 	lea    rcx,[rip+0x45ffc]        # 0x18009e2b0
   1800582b4:	e8 37 bc ff ff       	call   0x180053ef0
   1800582b9:	eb 25                	jmp    0x1800582e0
   1800582bb:	c7 05 87 f8 22 00 01 	mov    DWORD PTR [rip+0x22f887],0x1        # 0x180287b4c
   1800582c2:	00 00 00
   1800582c5:	49 8b d6             	mov    rdx,r14
   1800582c8:	48 8d 0d a9 5f 04 00 	lea    rcx,[rip+0x45fa9]        # 0x18009e278
   1800582cf:	e8 1c bc ff ff       	call   0x180053ef0
   1800582d4:	eb 0a                	jmp    0x1800582e0
   1800582d6:	c7 05 6c f8 22 00 01 	mov    DWORD PTR [rip+0x22f86c],0x1        # 0x180287b4c
   1800582dd:	00 00 00
   1800582e0:	48 8b 8c 24 a0 00 00 	mov    rcx,QWORD PTR [rsp+0xa0]
   1800582e7:	00
   1800582e8:	48 33 cc             	xor    rcx,rsp
   1800582eb:	e8 a0 1b 01 00       	call   0x180069e90
   1800582f0:	48 81 c4 b0 00 00 00 	add    rsp,0xb0
   1800582f7:	41 5f                	pop    r15
   1800582f9:	41 5e                	pop    r14
   1800582fb:	41 5d                	pop    r13
   1800582fd:	41 5c                	pop    r12
   1800582ff:	5f                   	pop    rdi
   180058300:	5e                   	pop    rsi
   180058301:	5b                   	pop    rbx
   180058302:	c3                   	ret
