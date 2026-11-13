   180039400:	sub    rsp,0x48
   180039404:	cmp    DWORD PTR [rip+0x24e6b1],0x0        # 0x180287abc
   18003940b:	jne    0x180039480
   18003940d:	cmp    DWORD PTR [rip+0x2476fc],0x0        # 0x180280b10
   180039414:	jne    0x180039480
   180039416:	cmp    DWORD PTR [rip+0x2476df],0x0        # 0x180280afc
   18003941d:	je     0x180039480
   18003941f:	cmp    DWORD PTR [rip+0x2476da],0x0        # 0x180280b00
   180039426:	jne    0x180039480
   180039428:	cmp    DWORD PTR [rip+0x2476c9],0x1        # 0x180280af8
   18003942f:	jl     0x180039480
   180039431:	mov    DWORD PTR [rip+0x2476d5],0x1        # 0x180280b10
   18003943b:	mov    DWORD PTR [rip+0x24e677],0x1        # 0x180287abc
   180039445:	call   0x180038f30
   18003944a:	jmp    0x180039476
   18003944c:	lea    rcx,[rip+0x636b5]        # 0x18009cb08    ;; "  panel  faulted inside the interface pass - panel off"
   180039453:	call   0x180053ef0
   180039458:	mov    DWORD PTR [rip+0x24769e],0x1        # 0x180280b00
   180039462:	mov    rcx,QWORD PTR [rip+0x89d77]        # 0x1800c31e0
   180039469:	cmp    rcx,0xffffffffffffffff
   18003946d:	je     0x180039476
   18003946f:	call   QWORD PTR [rip+0x53bf3]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   180039475:	nop
   180039476:	mov    DWORD PTR [rip+0x24e63c],0x0        # 0x180287abc
   180039480:	add    rsp,0x48
   180039484:	ret
