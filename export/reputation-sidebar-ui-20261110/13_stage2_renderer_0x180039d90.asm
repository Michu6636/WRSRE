   180039d90:	mov    rax,rsp
   180039d93:	push   rbx
   180039d94:	sub    rsp,0x100
   180039d9b:	movaps XMMWORD PTR [rax-0x18],xmm6
   180039d9f:	movaps XMMWORD PTR [rax-0x28],xmm7
   180039da3:	movaps XMMWORD PTR [rax-0x38],xmm8
   180039da8:	movaps XMMWORD PTR [rax-0x48],xmm9
   180039dad:	movaps XMMWORD PTR [rax-0x58],xmm10
   180039db2:	movaps XMMWORD PTR [rax-0x68],xmm11
   180039db7:	movaps XMMWORD PTR [rax-0x78],xmm12
   180039dbc:	movaps XMMWORD PTR [rax-0x88],xmm13
   180039dc4:	movaps XMMWORD PTR [rsp+0x70],xmm14
   180039dca:	movaps XMMWORD PTR [rsp+0x60],xmm15
   180039dd0:	movss  xmm14,DWORD PTR [rip+0x8979f]        # 0x1800c3578
   180039dd9:	movss  xmm3,DWORD PTR [rip+0x897a3]        # 0x1800c3584
   180039de1:	movaps xmm8,xmm3
   180039de5:	movss  xmm1,DWORD PTR [rip+0x8978f]        # 0x1800c357c
   180039ded:	addss  xmm8,xmm1
   180039df2:	addss  xmm8,DWORD PTR [rip+0x7588d]        # 0x1800af688
   180039dfb:	movss  xmm11,DWORD PTR [rip+0x754fc]        # 0x1800af300
   180039e04:	movss  DWORD PTR [rsp+0x38],xmm11
   180039e0b:	movss  xmm6,DWORD PTR [rip+0x754f9]        # 0x1800af30c
   180039e13:	movss  DWORD PTR [rsp+0x30],xmm6
   180039e19:	movss  DWORD PTR [rsp+0x28],xmm11
   180039e20:	movss  DWORD PTR [rsp+0x20],xmm11
   180039e27:	movss  xmm2,DWORD PTR [rip+0x89751]        # 0x1800c3580
   180039e2f:	movaps xmm0,xmm14
   180039e33:	call   0x180039330
   180039e38:	movss  xmm3,DWORD PTR [rip+0x89744]        # 0x1800c3584
   180039e40:	movss  xmm10,DWORD PTR [rip+0x757ff]        # 0x1800af648
   180039e49:	subss  xmm3,xmm10
   180039e4e:	movss  xmm2,DWORD PTR [rip+0x8972a]        # 0x1800c3580
   180039e56:	subss  xmm2,xmm10
   180039e5b:	movss  xmm1,DWORD PTR [rip+0x89719]        # 0x1800c357c
   180039e63:	movss  xmm9,DWORD PTR [rip+0x7579c]        # 0x1800af608
   180039e6c:	addss  xmm1,xmm9
   180039e71:	movss  xmm5,DWORD PTR [rip+0x896ff]        # 0x1800c3578
   180039e79:	addss  xmm5,xmm9
   180039e7e:	movss  DWORD PTR [rsp+0x38],xmm6
   180039e84:	movss  xmm0,DWORD PTR [rip+0x753ac]        # 0x1800af238
   180039e8c:	movss  DWORD PTR [rsp+0x30],xmm0
   180039e92:	movss  xmm4,DWORD PTR [rip+0x75396]        # 0x1800af230
   180039e9a:	movss  DWORD PTR [rsp+0x28],xmm4
   180039ea0:	movss  xmm0,DWORD PTR [rip+0x75384]        # 0x1800af22c
   180039ea8:	movss  DWORD PTR [rsp+0x20],xmm0
   180039eae:	movaps xmm0,xmm5
   180039eb1:	call   0x180039330
   180039eb6:	xor    ebx,ebx
   180039eb8:	movss  xmm6,DWORD PTR [rip+0x757d4]        # 0x1800af694
   180039ec0:	movss  xmm7,DWORD PTR [rip+0x7547c]        # 0x1800af344
   180039ec8:	movss  xmm12,DWORD PTR [rip+0x7538f]        # 0x1800af260
   180039ed1:	movss  xmm13,DWORD PTR [rip+0x755a6]        # 0x1800af480
   180039eda:	mov    DWORD PTR [rsp+0x48],ebx
   180039ede:	imul   ecx,ebx,0x32
   180039ee1:	cvttss2si eax,DWORD PTR [rip+0x89697]        # 0x1800c3580
   180039ee9:	cmp    ecx,eax
   180039eeb:	jg     0x180039f4d
   180039eed:	mov    eax,ebx
   180039eef:	and    eax,0x80000001
   180039ef4:	jge    0x180039efd
   180039ef6:	dec    eax
   180039ef8:	or     eax,0xfffffffe
   180039efb:	inc    eax
   180039efd:	test   eax,eax
   180039eff:	je     0x180039f07
   180039f01:	movaps xmm3,xmm9
   180039f05:	jmp    0x180039f0b
   180039f07:	movaps xmm3,xmm10
   180039f0b:	movd   xmm0,ebx
   180039f0f:	cvtdq2ps xmm0,xmm0
   180039f12:	mulss  xmm0,xmm6
   180039f16:	addss  xmm0,DWORD PTR [rip+0x8965a]        # 0x1800c3578
   180039f1e:	movss  DWORD PTR [rsp+0x38],xmm7
   180039f24:	movss  DWORD PTR [rsp+0x30],xmm12
   180039f2b:	movss  DWORD PTR [rsp+0x28],xmm11
   180039f32:	movss  DWORD PTR [rsp+0x20],xmm7
   180039f38:	movaps xmm2,xmm13
   180039f3c:	movss  xmm1,DWORD PTR [rip+0x89638]        # 0x1800c357c
   180039f44:	call   0x180039330
   180039f49:	inc    ebx
   180039f4b:	jmp    0x180039eda
   180039f4d:	movss  xmm6,DWORD PTR [rip+0x753cb]        # 0x1800af320    ;; "33s?"
   180039f55:	movss  DWORD PTR [rsp+0x38],xmm6
   180039f5b:	movss  DWORD PTR [rsp+0x30],xmm12
   180039f62:	movss  DWORD PTR [rsp+0x28],xmm12
   180039f69:	movss  DWORD PTR [rsp+0x20],xmm7
   180039f6f:	movss  xmm9,DWORD PTR [rip+0x75724]        # 0x1800af69c
   180039f78:	movaps xmm3,xmm9
   180039f7c:	movaps xmm2,xmm9
   180039f80:	movaps xmm1,xmm8
   180039f84:	movaps xmm0,xmm14
   180039f88:	call   0x180039330
   180039f8d:	movaps xmm11,xmm14
   180039f91:	addss  xmm11,DWORD PTR [rip+0x75716]        # 0x1800af6b0
   180039f9a:	movss  DWORD PTR [rsp+0x38],xmm6
   180039fa0:	movss  DWORD PTR [rsp+0x30],xmm12
   180039fa7:	movss  DWORD PTR [rsp+0x28],xmm7
   180039fad:	movss  DWORD PTR [rsp+0x20],xmm12
   180039fb4:	movaps xmm3,xmm9
   180039fb8:	movaps xmm2,xmm9
   180039fbc:	movaps xmm1,xmm8
   180039fc0:	movaps xmm0,xmm11
   180039fc4:	call   0x180039330
   180039fc9:	movaps xmm15,xmm14
   180039fcd:	addss  xmm15,DWORD PTR [rip+0x756f2]        # 0x1800af6c8
   180039fd6:	movss  DWORD PTR [rsp+0x38],xmm6
   180039fdc:	movss  DWORD PTR [rsp+0x30],xmm7
   180039fe2:	movss  xmm0,DWORD PTR [rip+0x752aa]        # 0x1800af294
   180039fea:	movss  DWORD PTR [rsp+0x28],xmm0
   180039ff0:	movss  DWORD PTR [rsp+0x20],xmm12
   180039ff7:	movaps xmm3,xmm9
   180039ffb:	movaps xmm2,xmm9
   180039fff:	movaps xmm1,xmm8
   18003a003:	movaps xmm0,xmm15
   18003a007:	call   0x180039330
   18003a00c:	movaps xmm12,xmm14
   18003a010:	addss  xmm12,DWORD PTR [rip+0x756b7]        # 0x1800af6d0
   18003a019:	movss  DWORD PTR [rsp+0x30],xmm7
   18003a01f:	movss  DWORD PTR [rsp+0x28],xmm7
   18003a025:	lea    rax,[rip+0x895e4]        # 0x1800c3610    ;; "editor/window_small.png"
   18003a02c:	mov    QWORD PTR [rsp+0x20],rax
   18003a031:	movaps xmm3,xmm9
   18003a035:	movss  xmm2,DWORD PTR [rip+0x75687]        # 0x1800af6c4
   18003a03d:	movaps xmm1,xmm8
   18003a041:	movaps xmm0,xmm12
   18003a045:	call   0x180039490
   18003a04a:	nop
   18003a04b:	movss  xmm1,DWORD PTR [rip+0x89529]        # 0x1800c357c
   18003a053:	subss  xmm1,xmm10
   18003a058:	lea    r9,[rip+0x628e1]        # 0x18009c940
   18003a05f:	mov    r8d,0xff90f090
   18003a065:	movss  xmm0,DWORD PTR [rip+0x8950b]        # 0x1800c3578
   18003a06d:	call   0x180022c50
   18003a072:	movss  xmm1,DWORD PTR [rip+0x89502]        # 0x1800c357c
   18003a07a:	subss  xmm1,xmm10
   18003a07f:	movss  xmm0,DWORD PTR [rip+0x894f9]        # 0x1800c3580
   18003a087:	addss  xmm0,DWORD PTR [rip+0x894e9]        # 0x1800c3578
   18003a08f:	movss  xmm6,DWORD PTR [rip+0x755d1]        # 0x1800af668
   18003a097:	subss  xmm0,xmm6
   18003a09b:	lea    r9,[rip+0x628a2]        # 0x18009c944
   18003a0a2:	mov    r8d,0xff90f090
   18003a0a8:	call   0x180022c50
   18003a0ad:	movss  xmm1,DWORD PTR [rip+0x894cf]        # 0x1800c3584
   18003a0b5:	addss  xmm1,DWORD PTR [rip+0x894bf]        # 0x1800c357c
   18003a0bd:	addss  xmm1,xmm13
   18003a0c2:	lea    r9,[rip+0x6287f]        # 0x18009c948
   18003a0c9:	mov    r8d,0xff90f090
   18003a0cf:	movss  xmm0,DWORD PTR [rip+0x894a1]        # 0x1800c3578
   18003a0d7:	call   0x180022c50
   18003a0dc:	movss  xmm1,DWORD PTR [rip+0x894a0]        # 0x1800c3584
   18003a0e4:	addss  xmm1,DWORD PTR [rip+0x89490]        # 0x1800c357c
   18003a0ec:	addss  xmm1,xmm13
   18003a0f1:	movss  xmm0,DWORD PTR [rip+0x89487]        # 0x1800c3580
   18003a0f9:	addss  xmm0,DWORD PTR [rip+0x89477]        # 0x1800c3578
   18003a101:	subss  xmm0,xmm6
   18003a105:	lea    r9,[rip+0x62840]        # 0x18009c94c
   18003a10c:	mov    r8d,0xff90f090
   18003a112:	call   0x180022c50
   18003a117:	addss  xmm8,xmm9
   18003a11c:	addss  xmm8,DWORD PTR [rip+0x75473]        # 0x1800af598
   18003a125:	lea    r9,[rip+0x62824]        # 0x18009c950    ;; "red"
   18003a12c:	mov    r8d,0xffffffff
   18003a132:	movaps xmm1,xmm8
   18003a136:	movaps xmm0,xmm14
   18003a13a:	call   0x180022c50
   18003a13f:	lea    r9,[rip+0x6280e]        # 0x18009c954    ;; "green"
   18003a146:	mov    r8d,0xffffffff
   18003a14c:	movaps xmm1,xmm8
   18003a150:	movaps xmm0,xmm11
   18003a154:	call   0x180022c50
   18003a159:	lea    r9,[rip+0x627fc]        # 0x18009c95c    ;; "blue"
   18003a160:	mov    r8d,0xffffffff
   18003a166:	movaps xmm1,xmm8
   18003a16a:	movaps xmm0,xmm15
   18003a16e:	call   0x180022c50
   18003a173:	lea    r9,[rip+0x627ee]        # 0x18009c968    ;; "window art"
   18003a17a:	mov    r8d,0xffffffff
   18003a180:	movaps xmm1,xmm8
   18003a184:	movaps xmm0,xmm12
   18003a188:	call   0x180022c50
   18003a18d:	mov    eax,DWORD PTR [rip+0x246971]        # 0x180280b04
   18003a193:	inc    eax
   18003a195:	mov    DWORD PTR [rip+0x246969],eax        # 0x180280b04
   18003a19b:	cmp    eax,0x1
   18003a19e:	jne    0x18003a219
   18003a1a0:	movss  xmm0,DWORD PTR [rip+0x893dc]        # 0x1800c3584
   18003a1a8:	cvtps2pd xmm0,xmm0
   18003a1ab:	movss  xmm3,DWORD PTR [rip+0x893cd]        # 0x1800c3580
   18003a1b3:	cvtps2pd xmm3,xmm3
   18003a1b6:	movss  xmm2,DWORD PTR [rip+0x893be]        # 0x1800c357c
   18003a1be:	cvtps2pd xmm2,xmm2
   18003a1c1:	movss  xmm1,DWORD PTR [rip+0x893af]        # 0x1800c3578
   18003a1c9:	cvtps2pd xmm1,xmm1
   18003a1cc:	movsd  QWORD PTR [rsp+0x20],xmm0
   18003a1d2:	movq   r9,xmm3
   18003a1d7:	movq   r8,xmm2
   18003a1dc:	movq   rdx,xmm1
   18003a1e1:	lea    rcx,[rip+0x62798]        # 0x18009c980    ;; "  panel  calibration card at %.0f,%.0f  %.0fx%.0f  ticks every 50px"
   18003a1e8:	call   0x180053ef0
   18003a1ed:	jmp    0x18003a219
   18003a1ef:	lea    rcx,[rip+0x62722]        # 0x18009c918    ;; "  panel  stage 2 faulted - panel off"
   18003a1f6:	call   0x180053ef0
   18003a1fb:	mov    DWORD PTR [rip+0x2468fb],0x1        # 0x180280b00
   18003a205:	mov    rcx,QWORD PTR [rip+0x88fd4]        # 0x1800c31e0
   18003a20c:	cmp    rcx,0xffffffffffffffff
   18003a210:	je     0x18003a219
   18003a212:	call   QWORD PTR [rip+0x52e50]        # 0x18008d068    ;; [KERNEL32.dll!FlushFileBuffers]
   18003a218:	nop
   18003a219:	lea    r11,[rsp+0x100]
   18003a221:	movaps xmm6,XMMWORD PTR [r11-0x10]
   18003a226:	movaps xmm7,XMMWORD PTR [r11-0x20]
   18003a22b:	movaps xmm8,XMMWORD PTR [r11-0x30]
   18003a230:	movaps xmm9,XMMWORD PTR [r11-0x40]
   18003a235:	movaps xmm10,XMMWORD PTR [r11-0x50]
   18003a23a:	movaps xmm11,XMMWORD PTR [r11-0x60]
   18003a23f:	movaps xmm12,XMMWORD PTR [r11-0x70]
   18003a244:	movaps xmm13,XMMWORD PTR [r11-0x80]
   18003a249:	movaps xmm14,XMMWORD PTR [rsp+0x70]
   18003a24f:	movaps xmm15,XMMWORD PTR [rsp+0x60]
   18003a255:	mov    rsp,r11
   18003a258:	pop    rbx
   18003a259:	ret
