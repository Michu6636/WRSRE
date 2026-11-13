   180038f30:	sub    rsp,0x28
   180038f34:	call   0x180039c30
   180038f39:	cmp    DWORD PTR [rip+0x247bc0],0x0        # 0x180280b00
   180038f40:	jne    0x180038f72
   180038f42:	mov    eax,DWORD PTR [rip+0x247bb0]        # 0x180280af8
   180038f48:	cmp    eax,0x1
   180038f4b:	je     0x180038f72
   180038f4d:	cmp    eax,0x2
   180038f50:	jne    0x180038f5b
   180038f52:	add    rsp,0x28
   180038f56:	jmp    0x180039d90
   180038f5b:	cmp    eax,0x3
   180038f5e:	jne    0x180038f69
   180038f60:	add    rsp,0x28
   180038f64:	jmp    0x18003a260
   180038f69:	add    rsp,0x28
   180038f6d:	jmp    0x180039960
   180038f72:	add    rsp,0x28
   180038f76:	ret
