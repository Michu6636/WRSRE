   180058c20:	rex push rdi
   180058c22:	sub    rsp,0x100
   180058c29:	mov    eax,DWORD PTR [rip+0x14b3a1]        # 0x1801a3fd0
   180058c2f:	xor    edi,edi
   180058c31:	mov    ecx,DWORD PTR [rip+0x6a6b5]        # 0x1800c32ec
   180058c37:	inc    eax
   180058c39:	mov    DWORD PTR [rip+0x14b369],edi        # 0x1801a3fa8
   180058c3f:	mov    DWORD PTR [rip+0x228133],edi        # 0x180280d78
   180058c45:	mov    DWORD PTR [rip+0x14b385],eax        # 0x1801a3fd0
   180058c4b:	test   ecx,ecx
   180058c4d:	je     0x180058ccd
   180058c4f:	cmp    DWORD PTR [rip+0x6a503],edi        # 0x1800c3158
   180058c55:	jne    0x180058c87
   180058c57:	cmp    DWORD PTR [rip+0x22d423],edi        # 0x180286080
   180058c5d:	jne    0x180058c87
   180058c5f:	cmp    DWORD PTR [rip+0x174db3],edi        # 0x1801cda18
   180058c65:	jne    0x180058c87
   180058c67:	cmp    DWORD PTR [rip+0x174da7],edi        # 0x1801cda14
   180058c6d:	jne    0x180058c87
   180058c6f:	cmp    DWORD PTR [rip+0x12c23f],edi        # 0x180184eb4
   180058c75:	jne    0x180058c87
   180058c77:	cmp    DWORD PTR [rip+0x174d9f],edi        # 0x1801cda1c
   180058c7d:	jne    0x180058c87
   180058c7f:	cmp    DWORD PTR [rip+0x6bfcb],edi        # 0x1800c4c50
   180058c85:	je     0x180058ccd
   180058c87:	cmp    DWORD PTR [rip+0x15ba7b],edi        # 0x1801b4708
   180058c8d:	je     0x180058c9a
   180058c8f:	sub    eax,DWORD PTR [rip+0x15ba77]        # 0x1801b470c
   180058c95:	cmp    eax,0x2
   180058c98:	jbe    0x180058ccd
   180058c9a:	mov    eax,DWORD PTR [rip+0x3b8f24]        # 0x180411bc4
   180058ca0:	inc    eax
   180058ca2:	mov    DWORD PTR [rip+0x3b8f1c],eax        # 0x180411bc4
   180058ca8:	cmp    eax,0xf0
   180058cad:	jne    0x180058cf3
   180058caf:	lea    rcx,[rip+0x461ca]        # 0x18009ee80    ;; "  side  a window has been open for four seconds and the panel has not drawn it - the windows come back to the terrain pa"
   180058cb6:	mov    DWORD PTR [rip+0x2280c8],0x1        # 0x180280d88
   180058cc0:	call   0x180053ef0
   180058cc5:	mov    ecx,DWORD PTR [rip+0x6a621]        # 0x1800c32ec
   180058ccb:	jmp    0x180058cf3
   180058ccd:	cmp    DWORD PTR [rip+0x2280b5],edi        # 0x180280d88
   180058cd3:	je     0x180058ce7
   180058cd5:	lea    rcx,[rip+0x46234]        # 0x18009ef10    ;; "  side  the panel is drawing again - the windows go back inside it"
   180058cdc:	call   0x180053ef0
   180058ce1:	mov    ecx,DWORD PTR [rip+0x6a605]        # 0x1800c32ec
   180058ce7:	mov    DWORD PTR [rip+0x3b8ed7],edi        # 0x180411bc4
   180058ced:	mov    DWORD PTR [rip+0x228095],edi        # 0x180280d88
   180058cf3:	test   ecx,ecx
   180058cf5:	je     0x180059209
   180058cfb:	cmp    DWORD PTR [rip+0x22807f],edi        # 0x180280d80
   180058d01:	je     0x180059209
   180058d07:	cmp    DWORD PTR [rip+0x228077],edi        # 0x180280d84
   180058d0d:	je     0x180059209
