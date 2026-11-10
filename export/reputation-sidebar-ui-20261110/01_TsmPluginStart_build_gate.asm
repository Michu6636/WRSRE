   180068f30:	sub    rsp,0x48
   180068f34:	cmp    DWORD PTR [rip+0x6401a5],0x0        # 0x1806a90e0
   180068f3b:	je     0x180068f55
   180068f3d:	mov    rax,QWORD PTR [rip+0x5be2c]        # 0x1800c4d70
   180068f44:	lea    rcx,[rip+0x45895]        # 0x1800ae7e0    ;; "reput    TsmPluginStart called twice - ignored"
   180068f4b:	call   QWORD PTR [rax+0x38]
   180068f4e:	xor    eax,eax
   180068f50:	add    rsp,0x48
   180068f54:	ret
   180068f55:	mov    QWORD PTR [rsp+0x58],rbx
   180068f5a:	lea    rcx,[rip+0x458af]        # 0x1800ae810    ;; "reputation.log"
   180068f61:	mov    QWORD PTR [rsp+0x60],rbp
   180068f66:	mov    QWORD PTR [rsp+0x68],rsi
   180068f6b:	mov    QWORD PTR [rsp+0x40],rdi
   180068f70:	mov    DWORD PTR [rip+0x640166],0x1        # 0x1806a90e0
   180068f7a:	call   0x180060c10
   180068f7f:	cmp    DWORD PTR [rip+0x5a102],0x0        # 0x1800c3088
   180068f86:	lea    r8,[rip+0x458c3]        # 0x1800ae850    ;; "LIVE - this build will change game state"
   180068f8d:	mov    QWORD PTR [rip+0x5a24c],rax        # 0x1800c31e0
   180068f94:	lea    rdx,[rip+0x38fd1]        # 0x1800a1f6c    ;; "2.1"
   180068f9b:	lea    rax,[rip+0x4587e]        # 0x1800ae820    ;; "DRY RUN - nothing is written to the game"
   180068fa2:	cmovne r8,rax
   180068fa6:	lea    rcx,[rip+0x458d3]        # 0x1800ae880    ;; "reputation %s   %s"
   180068fad:	call   0x180053ef0
   180068fb2:	xor    r8d,r8d
   180068fb5:	lea    rdx,[rip+0x44b3c]        # 0x1800adaf8    ;; "build_stamp"
   180068fbc:	lea    rcx,[rip+0x43171]        # 0x1800ac134    ;; "sites"
   180068fc3:	call   0x18000cc60
   180068fc8:	mov    rdi,rax
   180068fcb:	test   eax,eax
   180068fcd:	je     0x180069069
   180068fd3:	mov    rbx,QWORD PTR [rip+0x5bda6]        # 0x1800c4d80
   180068fda:	mov    edx,0x40
   180068fdf:	mov    rax,QWORD PTR [rip+0x5bd8a]        # 0x1800c4d70
   180068fe6:	mov    rcx,rbx
   180068fe9:	call   QWORD PTR [rax+0x60]
   180068fec:	test   eax,eax
   180068fee:	je     0x180069069
   180068ff0:	mov    eax,0x5a4d
   180068ff5:	cmp    WORD PTR [rbx],ax
   180068ff8:	jne    0x180069069
   180068ffa:	movsxd rbx,DWORD PTR [rbx+0x3c]
   180068ffe:	mov    edx,0x108
   180069003:	add    rbx,QWORD PTR [rip+0x5bd76]        # 0x1800c4d80
   18006900a:	mov    rax,QWORD PTR [rip+0x5bd5f]        # 0x1800c4d70
   180069011:	mov    rcx,rbx
   180069014:	call   QWORD PTR [rax+0x60]
   180069017:	test   eax,eax
   180069019:	je     0x180069069
   18006901b:	cmp    DWORD PTR [rbx],0x4550
   180069021:	jne    0x180069069
   180069023:	mov    ebx,DWORD PTR [rbx+0x8]
   180069026:	mov    edx,ebx
   180069028:	cmp    ebx,edi
   18006902a:	jne    0x180069044
   18006902c:	lea    rcx,[rip+0x44ad5]        # 0x1800adb08    ;; "build 0x%08X - the one every offset was read off"
   180069033:	mov    DWORD PTR [rip+0x217ae7],0x1        # 0x180280b24
   18006903d:	call   0x180053ef0
