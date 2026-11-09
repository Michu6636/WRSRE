
scratch/reputation-static/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180058310 <.text+0x57310>:
   180058310:	40 53                	rex push rbx
   180058312:	48 83 ec 20          	sub    rsp,0x20
   180058316:	83 3d 3f b9 06 00 00 	cmp    DWORD PTR [rip+0x6b93f],0x0        # 0x1800c3c5c
   18005831d:	7c 79                	jl     0x180058398
   18005831f:	83 3d 22 f8 22 00 00 	cmp    DWORD PTR [rip+0x22f822],0x0        # 0x180287b48
   180058326:	74 70                	je     0x180058398
   180058328:	48 8b 1d 51 ca 06 00 	mov    rbx,QWORD PTR [rip+0x6ca51]        # 0x1800c4d80
   18005832f:	ba 08 00 00 00       	mov    edx,0x8
   180058334:	48 8b 05 35 ca 06 00 	mov    rax,QWORD PTR [rip+0x6ca35]        # 0x1800c4d70
   18005833b:	48 8d 8b 60 15 9e 00 	lea    rcx,[rbx+0x9e1560]
   180058342:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180058345:	85 c0                	test   eax,eax
   180058347:	74 4f                	je     0x180058398
   180058349:	48 8b 8b 60 15 9e 00 	mov    rcx,QWORD PTR [rbx+0x9e1560]
   180058350:	48 85 c9             	test   rcx,rcx
   180058353:	74 43                	je     0x180058398
   180058355:	48 8b 05 14 ca 06 00 	mov    rax,QWORD PTR [rip+0x6ca14]        # 0x1800c4d70
   18005835c:	48 ff c1             	inc    rcx
   18005835f:	ba 17 00 00 00       	mov    edx,0x17
   180058364:	ff 50 60             	call   QWORD PTR [rax+0x60]
   180058367:	85 c0                	test   eax,eax
   180058369:	74 2d                	je     0x180058398
   18005836b:	48 8b 93 60 15 9e 00 	mov    rdx,QWORD PTR [rbx+0x9e1560]
   180058372:	4c 8d 05 af b8 06 00 	lea    r8,[rip+0x6b8af]        # 0x1800c3c28
   180058379:	33 c0                	xor    eax,eax
   18005837b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
   180058380:	0f b6 4c 02 01       	movzx  ecx,BYTE PTR [rdx+rax*1+0x1]
   180058385:	41 3a 0c 00          	cmp    cl,BYTE PTR [r8+rax*1]
   180058389:	75 0d                	jne    0x180058398
   18005838b:	84 c9                	test   cl,cl
   18005838d:	74 11                	je     0x1800583a0
   18005838f:	48 ff c0             	inc    rax
   180058392:	48 83 f8 17          	cmp    rax,0x17
   180058396:	72 e8                	jb     0x180058380
   180058398:	33 c0                	xor    eax,eax
   18005839a:	48 83 c4 20          	add    rsp,0x20
   18005839e:	5b                   	pop    rbx
   18005839f:	c3                   	ret
   1800583a0:	48 8b 1d d9 c9 06 00 	mov    rbx,QWORD PTR [rip+0x6c9d9]        # 0x1800c4d80
   1800583a7:	ba 04 00 00 00       	mov    edx,0x4
   1800583ac:	48 8b 05 bd c9 06 00 	mov    rax,QWORD PTR [rip+0x6c9bd]        # 0x1800c4d70
   1800583b3:	48 8d 8b 6c 15 9e 00 	lea    rcx,[rbx+0x9e156c]
   1800583ba:	ff 50 60             	call   QWORD PTR [rax+0x60]
   1800583bd:	85 c0                	test   eax,eax
   1800583bf:	74 d7                	je     0x180058398
   1800583c1:	8b 0d 95 b8 06 00    	mov    ecx,DWORD PTR [rip+0x6b895]        # 0x1800c3c5c
   1800583c7:	33 c0                	xor    eax,eax
   1800583c9:	39 8b 6c 15 9e 00    	cmp    DWORD PTR [rbx+0x9e156c],ecx
   1800583cf:	0f 94 c0             	sete   al
   1800583d2:	48 83 c4 20          	add    rsp,0x20
   1800583d6:	5b                   	pop    rbx
   1800583d7:	c3                   	ret
