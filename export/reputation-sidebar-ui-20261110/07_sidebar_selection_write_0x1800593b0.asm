   1800593b0:	mov    QWORD PTR [rsp+0x10],rbx
   1800593b5:	push   rdi
   1800593b6:	sub    rsp,0x40
   1800593ba:	cmp    DWORD PTR [rip+0x69f2b],0x0        # 0x1800c32ec
   1800593c1:	je     0x1800594ab
   1800593c7:	cmp    DWORD PTR [rip+0x2279b6],0x0        # 0x180280d84
   1800593ce:	je     0x1800594ab
   1800593d4:	cmp    DWORD PTR [rip+0x2279a5],0x0        # 0x180280d80
   1800593db:	je     0x1800594ab
   1800593e1:	mov    QWORD PTR [rsp+0x50],0x0
   1800593ea:	lea    rcx,[rsp+0x50]
   1800593ef:	call   0x1800595b0
   1800593f4:	test   eax,eax
   1800593f6:	je     0x1800594ab
   1800593fc:	mov    r9,QWORD PTR [rsp+0x50]
   180059401:	test   r9,r9
   180059404:	je     0x1800594ab
   18005940a:	xor    edx,edx
   18005940c:	mov    rbx,QWORD PTR [rip+0x6b96d]        # 0x1800c4d80
   180059413:	mov    r10,QWORD PTR [rbx+0x9e1518]
   18005941a:	lea    r11,[rip+0x6a7ef]        # 0x1800c3c10    ;; "reputation"
   180059421:	nop    DWORD PTR [rax+0x0]
   180059425:	data16 data16 nop WORD PTR [rax+rax*1+0x0]
   180059430:	imul   rdi,rdx,0x68
   180059434:	lea    r8,[rdi+r10*1]
   180059438:	xor    eax,eax
   18005943a:	nop    WORD PTR [rax+rax*1+0x0]
   180059440:	movzx  ecx,BYTE PTR [r8+rax*1+0x1]
   180059446:	cmp    cl,BYTE PTR [r11+rax*1]
   18005944a:	jne    0x1800594a3
   18005944c:	test   cl,cl
   18005944e:	je     0x18005945b
   180059450:	inc    rax
   180059453:	cmp    rax,0x17
   180059457:	jb     0x180059440
   180059459:	jmp    0x1800594a3
   18005945b:	cmp    rdx,0xffffffffffffffff
   18005945f:	je     0x1800594ab
   180059461:	mov    edx,0x8
   180059466:	lea    rcx,[rbx+0x9e1560]
   18005946d:	mov    rax,QWORD PTR [rip+0x6b8fc]        # 0x1800c4d70
   180059474:	call   QWORD PTR [rax+0x60]
   180059477:	test   eax,eax
   180059479:	je     0x1800594ab
   18005947b:	mov    rax,QWORD PTR [rip+0x6b8fe]        # 0x1800c4d80
   180059482:	mov    rax,QWORD PTR [rax+0x9e1518]
   180059489:	add    rax,rdi
   18005948c:	mov    QWORD PTR [rbx+0x9e1560],rax
   180059493:	jmp    0x1800594ab
   180059495:	lea    rcx,[rip+0x458dc]        # 0x18009ed78    ;; "taking the selection faulted"
   18005949c:	call   0x18002da30
   1800594a1:	jmp    0x1800594ab
   1800594a3:	inc    rdx
   1800594a6:	cmp    rdx,r9
   1800594a9:	jb     0x180059430
   1800594ab:	mov    rbx,QWORD PTR [rsp+0x58]
   1800594b0:	add    rsp,0x40
   1800594b4:	pop    rdi
   1800594b5:	ret
