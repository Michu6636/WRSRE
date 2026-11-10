   180058e77:	movaps XMMWORD PTR [rsp+0x80],xmm13
   180058e80:	call   0x180061240
   180058e85:	mov    ecx,DWORD PTR [rip+0x227ee1]        # 0x180280d6c
   180058e8b:	movaps xmm13,xmm0
   180058e8f:	test   ecx,ecx
   180058e91:	je     0x1800591cd
   180058e97:	mov    eax,DWORD PTR [rip+0x14b133]        # 0x1801a3fd0
   180058e9d:	sub    eax,ecx
   180058e9f:	cmp    eax,0x3
   180058ea2:	ja     0x1800591cd
   180058ea8:	movss  xmm1,DWORD PTR [rip+0x56758]        # 0x1800af608
   180058eb0:	comiss xmm1,DWORD PTR [rip+0x227ea5]        # 0x180280d5c
   180058eb7:	ja     0x1800591cd
   180058ebd:	cmp    DWORD PTR [rip+0x15b845],edi        # 0x1801b4708
   180058ec3:	mov    DWORD PTR [rip+0x3b8cef],edi        # 0x180411bb8
   180058ec9:	jne    0x180058efd
   180058ecb:	call   0x180001c80
   180058ed0:	test   eax,eax
   180058ed2:	je     0x180058f03
   180058ed4:	mov    eax,DWORD PTR [rip+0x3b8ce6]        # 0x180411bc0
   180058eda:	inc    eax
   180058edc:	mov    DWORD PTR [rip+0x3b8cde],eax        # 0x180411bc0
   180058ee2:	cmp    eax,0x384
   180058ee7:	jne    0x180058f03
   180058ee9:	lea    rcx,[rip+0x46190]        # 0x18009f080    ;; "the panel body hook has never once fired"
   180058ef0:	call   0x18002da30
   180058ef5:	cmp    DWORD PTR [rip+0x15b80d],edi        # 0x1801b4708
   180058efb:	je     0x180058f03
   180058efd:	mov    DWORD PTR [rip+0x3b8cbd],edi        # 0x180411bc0
   180058f03:	movss  xmm1,DWORD PTR [rip+0x227e59]        # 0x180280d64
   180058f0b:	movss  xmm2,DWORD PTR [rip+0x227e49]        # 0x180280d5c
   180058f13:	movaps xmm0,xmm1
   180058f16:	mulss  xmm2,DWORD PTR [rip+0x5638a]        # 0x1800af2a8
   180058f1e:	movss  DWORD PTR [rip+0x14b092],xmm6        # 0x1801a3fb8
   180058f26:	subss  xmm0,xmm2
   180058f2a:	addss  xmm2,xmm1
   180058f2e:	movss  DWORD PTR [rip+0x227e3a],xmm0        # 0x180280d70
   180058f36:	movss  DWORD PTR [rip+0x14b07e],xmm2        # 0x1801a3fbc
   180058f3e:	call   0x180061240
   180058f43:	mulss  xmm0,DWORD PTR [rip+0x6a789]        # 0x1800c36d4
   180058f4b:	mov    ecx,0x995274
   180058f50:	movss  xmm1,DWORD PTR [rip+0x6a39c]        # 0x1800c32f4
   180058f58:	mulss  xmm1,xmm13
   180058f5d:	addss  xmm1,DWORD PTR [rip+0x227e0b]        # 0x180280d70
   180058f65:	divss  xmm1,xmm0
   180058f69:	movss  xmm0,DWORD PTR [rip+0x6a387]        # 0x1800c32f8
   180058f71:	mulss  xmm0,xmm13
   180058f76:	movss  DWORD PTR [rip+0x14b02e],xmm1        # 0x1801a3fac
   180058f7e:	addss  xmm0,xmm6
   180058f82:	movss  DWORD PTR [rip+0x14b026],xmm0        # 0x1801a3fb0
   180058f8a:	call   0x180056650
   180058f8f:	test   eax,eax
   180058f91:	je     0x180058f9c
   180058f93:	movd   xmm1,eax
   180058f97:	cvtdq2ps xmm1,xmm1
   180058f9a:	jmp    0x180058fa7
   180058f9c:	movaps xmm1,xmm6
   180058f9f:	addss  xmm1,DWORD PTR [rip+0x56745]        # 0x1800af6ec
   180058fa7:	movss  xmm0,DWORD PTR [rip+0x6a34d]        # 0x1800c32fc
   180058faf:	mulss  xmm0,xmm13
   180058fb4:	subss  xmm1,xmm0
   180058fb8:	movss  xmm0,DWORD PTR [rip+0x14aff0]        # 0x1801a3fb0
   180058fc0:	addss  xmm0,DWORD PTR [rip+0x56700]        # 0x1800af6c8
   180058fc8:	movss  DWORD PTR [rip+0x14afe4],xmm1        # 0x1801a3fb4
   180058fd0:	comiss xmm0,xmm1
   180058fd3:	jbe    0x180058fdd
   180058fd5:	movss  DWORD PTR [rip+0x14afd7],xmm0        # 0x1801a3fb4
   180058fdd:	cmp    DWORD PTR [rip+0x14afdd],edi        # 0x1801a3fc0
   180058fe3:	mov    DWORD PTR [rip+0x14afbb],0x1        # 0x1801a3fa8
   180058fed:	je     0x180059020
   180058fef:	movss  xmm1,DWORD PTR [rip+0x6b155]        # 0x1800c414c
   180058ff7:	movss  xmm2,DWORD PTR [rip+0x56481]        # 0x1800af480
   180058fff:	movaps xmm0,xmm1
   180059002:	movss  xmm3,DWORD PTR [rip+0x227d66]        # 0x180280d70
   18005900a:	subss  xmm0,xmm2
   18005900e:	comiss xmm0,xmm3
   180059011:	ja     0x180059020
   180059013:	addss  xmm1,xmm2
   180059017:	comiss xmm3,xmm1
   18005901a:	jbe    0x18005919a
   180059020:	mov    ecx,0x99528c
   180059025:	call   0x180056650
   18005902a:	mov    ecx,0x995274
   18005902f:	mov    ebx,eax
   180059031:	call   0x180056650
   180059036:	mov    ecx,DWORD PTR [rip+0x14af84]        # 0x1801a3fc0
   18005903c:	movss  xmm3,DWORD PTR [rip+0x227d2c]        # 0x180280d70
   180059044:	inc    ecx
   180059046:	movss  DWORD PTR [rip+0x6b0fe],xmm3        # 0x1800c414c
   18005904e:	mov    DWORD PTR [rip+0x14af6c],ecx        # 0x1801a3fc0
   180059054:	cmp    ecx,0x8
   180059057:	jg     0x18005919a
