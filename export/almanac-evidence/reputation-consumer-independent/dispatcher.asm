Static GNU objdump; preferred VA; .pdata function range 0x180060e20..0x18006123a

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180060e20 <.text+0x5fe20>:
   180060e20:	40 53                	rex push rbx
   180060e22:	48 81 ec 10 01 00 00 	sub    rsp,0x110
   180060e29:	33 db                	xor    ebx,ebx
   180060e2b:	89 1d 27 00 22 00    	mov    DWORD PTR [rip+0x220027],ebx        # 0x180280e58
   180060e31:	89 1d 29 51 22 00    	mov    DWORD PTR [rip+0x225129],ebx        # 0x180285f60
   180060e37:	89 1d 37 52 22 00    	mov    DWORD PTR [rip+0x225237],ebx        # 0x180286074
   180060e3d:	8b 0d 1d 81 3e 00    	mov    ecx,DWORD PTR [rip+0x3e811d]        # 0x180448f60
   180060e43:	8b 05 87 31 14 00    	mov    eax,DWORD PTR [rip+0x143187]        # 0x1801a3fd0
   180060e49:	3b c8                	cmp    ecx,eax
   180060e4b:	75 04                	jne    0x180060e51
   180060e4d:	85 c9                	test   ecx,ecx
   180060e4f:	75 55                	jne    0x180060ea6
   180060e51:	89 05 09 81 3e 00    	mov    DWORD PTR [rip+0x3e8109],eax        # 0x180448f60
   180060e57:	89 1d fb 80 3e 00    	mov    DWORD PTR [rip+0x3e80fb],ebx        # 0x180448f58
   180060e5d:	39 1d 1d 30 06 00    	cmp    DWORD PTR [rip+0x6301d],ebx        # 0x1800c3e80
   180060e63:	74 39                	je     0x180060e9e
   180060e65:	8b cb                	mov    ecx,ebx
   180060e67:	f0 0f c1 0d c9 80 3e 	lock xadd DWORD PTR [rip+0x3e80c9],ecx        # 0x180448f38
   180060e6e:	00
   180060e6f:	b8 89 88 88 88       	mov    eax,0x88888889
   180060e74:	f7 e9                	imul   ecx
   180060e76:	03 d1                	add    edx,ecx
   180060e78:	c1 fa 06             	sar    edx,0x6
   180060e7b:	8b c2                	mov    eax,edx
   180060e7d:	c1 e8 1f             	shr    eax,0x1f
   180060e80:	03 d0                	add    edx,eax
   180060e82:	74 1c                	je     0x180060ea0
   180060e84:	6b c2 88             	imul   eax,edx,0xffffff88
   180060e87:	f0 01 05 aa 80 3e 00 	lock add DWORD PTR [rip+0x3e80aa],eax        # 0x180448f38
   180060e8e:	8b c2                	mov    eax,edx
   180060e90:	f7 d8                	neg    eax
   180060e92:	0f 48 c2             	cmovs  eax,edx
   180060e95:	f0 01 05 a4 80 3e 00 	lock add DWORD PTR [rip+0x3e80a4],eax        # 0x180448f40
   180060e9c:	eb 02                	jmp    0x180060ea0
   180060e9e:	8b d3                	mov    edx,ebx
   180060ea0:	89 15 ae 80 3e 00    	mov    DWORD PTR [rip+0x3e80ae],edx        # 0x180448f54
   180060ea6:	39 1d dc 51 22 00    	cmp    DWORD PTR [rip+0x2251dc],ebx        # 0x180286088
   180060eac:	74 1f                	je     0x180060ecd
   180060eae:	89 1d a4 80 3e 00    	mov    DWORD PTR [rip+0x3e80a4],ebx        # 0x180448f58
   180060eb4:	89 1d 9a 80 3e 00    	mov    DWORD PTR [rip+0x3e809a],ebx        # 0x180448f54
   180060eba:	87 1d 9c 80 3e 00    	xchg   DWORD PTR [rip+0x3e809c],ebx        # 0x180448f5c
   180060ec0:	48 81 c4 10 01 00 00 	add    rsp,0x110
   180060ec7:	5b                   	pop    rbx
   180060ec8:	e9 a3 9a fc ff       	jmp    0x18002a970
   180060ecd:	e8 7e 31 00 00       	call   0x180064050
   180060ed2:	eb 30                	jmp    0x180060f04
   180060ed4:	48 8d 0d a5 ad 03 00 	lea    rcx,[rip+0x3ada5]        # 0x18009bc80
   180060edb:	48 8b 05 8e 3e 06 00 	mov    rax,QWORD PTR [rip+0x63e8e]        # 0x1800c4d70
   180060ee2:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180060ee5:	c7 05 a9 fe 21 00 00 	mov    DWORD PTR [rip+0x21fea9],0x0        # 0x180280d98
   180060eec:	00 00 00
   180060eef:	48 8b 0d ea 22 06 00 	mov    rcx,QWORD PTR [rip+0x622ea]        # 0x1800c31e0
   180060ef6:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180060efa:	74 06                	je     0x180060f02
   180060efc:	ff 15 66 c1 02 00    	call   QWORD PTR [rip+0x2c166]        # 0x18008d068
   180060f02:	33 db                	xor    ebx,ebx
   180060f04:	b9 01 00 00 00       	mov    ecx,0x1
   180060f09:	e8 52 e1 fc ff       	call   0x18002f060
   180060f0e:	90                   	nop
   180060f0f:	e8 5c 06 fc ff       	call   0x180021570
   180060f14:	eb 30                	jmp    0x180060f46
   180060f16:	48 8d 0d 9b ad 03 00 	lea    rcx,[rip+0x3ad9b]        # 0x18009bcb8
   180060f1d:	48 8b 05 4c 3e 06 00 	mov    rax,QWORD PTR [rip+0x63e4c]        # 0x1800c4d70
   180060f24:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180060f27:	c7 05 27 22 06 00 00 	mov    DWORD PTR [rip+0x62227],0x0        # 0x1800c3158
   180060f2e:	00 00 00
   180060f31:	48 8b 0d a8 22 06 00 	mov    rcx,QWORD PTR [rip+0x622a8]        # 0x1800c31e0
   180060f38:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180060f3c:	74 06                	je     0x180060f44
   180060f3e:	ff 15 24 c1 02 00    	call   QWORD PTR [rip+0x2c124]        # 0x18008d068
   180060f44:	33 db                	xor    ebx,ebx
   180060f46:	b9 02 00 00 00       	mov    ecx,0x2
   180060f4b:	e8 10 e1 fc ff       	call   0x18002f060
   180060f50:	90                   	nop
   180060f51:	e8 2a b2 ff ff       	call   0x18005c180
   180060f56:	eb 3a                	jmp    0x180060f92
   180060f58:	48 8d 0d 99 ad 03 00 	lea    rcx,[rip+0x3ad99]        # 0x18009bcf8
   180060f5f:	48 8b 05 0a 3e 06 00 	mov    rax,QWORD PTR [rip+0x63e0a]        # 0x1800c4d70
   180060f66:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180060f69:	c7 05 e1 fd 21 00 01 	mov    DWORD PTR [rip+0x21fde1],0x1        # 0x180280d54
   180060f70:	00 00 00
   180060f73:	c7 05 1f fe 21 00 00 	mov    DWORD PTR [rip+0x21fe1f],0x0        # 0x180280d9c
   180060f7a:	00 00 00
   180060f7d:	48 8b 0d 5c 22 06 00 	mov    rcx,QWORD PTR [rip+0x6225c]        # 0x1800c31e0
   180060f84:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180060f88:	74 06                	je     0x180060f90
   180060f8a:	ff 15 d8 c0 02 00    	call   QWORD PTR [rip+0x2c0d8]        # 0x18008d068
   180060f90:	33 db                	xor    ebx,ebx
   180060f92:	83 3d 63 fb 21 00 00 	cmp    DWORD PTR [rip+0x21fb63],0x0        # 0x180280afc
   180060f99:	74 76                	je     0x180061011
   180060f9b:	83 3d 5e fb 21 00 00 	cmp    DWORD PTR [rip+0x21fb5e],0x0        # 0x180280b00
   180060fa2:	75 6d                	jne    0x180061011
   180060fa4:	83 3d 4d fb 21 00 01 	cmp    DWORD PTR [rip+0x21fb4d],0x1        # 0x180280af8
   180060fab:	7c 64                	jl     0x180061011
   180060fad:	89 1d 5d fb 21 00    	mov    DWORD PTR [rip+0x21fb5d],ebx        # 0x180280b10
   180060fb3:	8b 15 53 fb 21 00    	mov    edx,DWORD PTR [rip+0x21fb53]        # 0x180280b0c
   180060fb9:	85 d2                	test   edx,edx
   180060fbb:	74 4f                	je     0x18006100c
   180060fbd:	83 3d 50 fb 21 00 00 	cmp    DWORD PTR [rip+0x21fb50],0x0        # 0x180280b14
   180060fc4:	75 3f                	jne    0x180061005
   180060fc6:	8b 05 4c fb 21 00    	mov    eax,DWORD PTR [rip+0x21fb4c]        # 0x180280b18
   180060fcc:	ff c0                	inc    eax
   180060fce:	89 05 44 fb 21 00    	mov    DWORD PTR [rip+0x21fb44],eax        # 0x180280b18
   180060fd4:	83 f8 78             	cmp    eax,0x78
   180060fd7:	75 22                	jne    0x180060ffb
   180060fd9:	44 8b c3             	mov    r8d,ebx
   180060fdc:	83 fa 01             	cmp    edx,0x1
   180060fdf:	41 0f 94 c0          	sete   r8b
   180060fe3:	41 ff c0             	inc    r8d
   180060fe6:	48 8d 0d 53 bb 03 00 	lea    rcx,[rip+0x3bb53]        # 0x18009cb40
   180060fed:	e8 fe 2e ff ff       	call   0x180053ef0
   180060ff2:	8b 05 20 fb 21 00    	mov    eax,DWORD PTR [rip+0x21fb20]        # 0x180280b18
   180060ff8:	83 f8 78             	cmp    eax,0x78
   180060ffb:	7c 08                	jl     0x180061005
   180060ffd:	89 1d 09 fb 21 00    	mov    DWORD PTR [rip+0x21fb09],ebx        # 0x180280b0c
   180061003:	eb 07                	jmp    0x18006100c
   180061005:	e8 26 8c fd ff       	call   0x180039c30
   18006100a:	eb 05                	jmp    0x180061011
   18006100c:	e8 1f 7f fd ff       	call   0x180038f30
   180061011:	eb 30                	jmp    0x180061043
   180061013:	48 8d 0d 1e ad 03 00 	lea    rcx,[rip+0x3ad1e]        # 0x18009bd38
   18006101a:	48 8b 05 4f 3d 06 00 	mov    rax,QWORD PTR [rip+0x63d4f]        # 0x1800c4d70
   180061021:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180061024:	c7 05 d2 fa 21 00 01 	mov    DWORD PTR [rip+0x21fad2],0x1        # 0x180280b00
   18006102b:	00 00 00
   18006102e:	48 8b 0d ab 21 06 00 	mov    rcx,QWORD PTR [rip+0x621ab]        # 0x1800c31e0
   180061035:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180061039:	74 06                	je     0x180061041
   18006103b:	ff 15 27 c0 02 00    	call   QWORD PTR [rip+0x2c027]        # 0x18008d068
   180061041:	33 db                	xor    ebx,ebx
   180061043:	e8 38 de fb ff       	call   0x18001ee80
   180061048:	eb 30                	jmp    0x18006107a
   18006104a:	48 8d 0d 27 ad 03 00 	lea    rcx,[rip+0x3ad27]        # 0x18009bd78
   180061051:	48 8b 05 18 3d 06 00 	mov    rax,QWORD PTR [rip+0x63d18]        # 0x1800c4d70
   180061058:	ff 50 38             	call   QWORD PTR [rax+0x38]
   18006105b:	c7 05 eb 3b 06 00 00 	mov    DWORD PTR [rip+0x63beb],0x0        # 0x1800c4c50
   180061062:	00 00 00
   180061065:	48 8b 0d 74 21 06 00 	mov    rcx,QWORD PTR [rip+0x62174]        # 0x1800c31e0
   18006106c:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180061070:	74 06                	je     0x180061078
   180061072:	ff 15 f0 bf 02 00    	call   QWORD PTR [rip+0x2bff0]        # 0x18008d068
   180061078:	33 db                	xor    ebx,ebx
   18006107a:	e8 01 bc fb ff       	call   0x18001cc80
   18006107f:	eb 3a                	jmp    0x1800610bb
   180061081:	48 8d 0d 38 ad 03 00 	lea    rcx,[rip+0x3ad38]        # 0x18009bdc0
   180061088:	48 8b 05 e1 3c 06 00 	mov    rax,QWORD PTR [rip+0x63ce1]        # 0x1800c4d70
   18006108f:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180061092:	c7 05 18 3e 12 00 00 	mov    DWORD PTR [rip+0x123e18],0x0        # 0x180184eb4
   180061099:	00 00 00
   18006109c:	c7 05 0a 79 06 00 01 	mov    DWORD PTR [rip+0x6790a],0x1        # 0x1800c89b0
   1800610a3:	00 00 00
   1800610a6:	48 8b 0d 33 21 06 00 	mov    rcx,QWORD PTR [rip+0x62133]        # 0x1800c31e0
   1800610ad:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1800610b1:	74 06                	je     0x1800610b9
   1800610b3:	ff 15 af bf 02 00    	call   QWORD PTR [rip+0x2bfaf]        # 0x18008d068
   1800610b9:	33 db                	xor    ebx,ebx
   1800610bb:	e8 e0 cf fb ff       	call   0x18001e0a0
   1800610c0:	eb 30                	jmp    0x1800610f2
   1800610c2:	48 8d 0d 3f ad 03 00 	lea    rcx,[rip+0x3ad3f]        # 0x18009be08
   1800610c9:	48 8b 05 a0 3c 06 00 	mov    rax,QWORD PTR [rip+0x63ca0]        # 0x1800c4d70
   1800610d0:	ff 50 38             	call   QWORD PTR [rax+0x38]
   1800610d3:	c7 05 a3 4f 22 00 00 	mov    DWORD PTR [rip+0x224fa3],0x0        # 0x180286080
   1800610da:	00 00 00
   1800610dd:	48 8b 0d fc 20 06 00 	mov    rcx,QWORD PTR [rip+0x620fc]        # 0x1800c31e0
   1800610e4:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1800610e8:	74 06                	je     0x1800610f0
   1800610ea:	ff 15 78 bf 02 00    	call   QWORD PTR [rip+0x2bf78]        # 0x18008d068
   1800610f0:	33 db                	xor    ebx,ebx
   1800610f2:	e8 59 ed fb ff       	call   0x18001fe50
   1800610f7:	eb 30                	jmp    0x180061129
   1800610f9:	48 8d 0d 50 ad 03 00 	lea    rcx,[rip+0x3ad50]        # 0x18009be50
   180061100:	48 8b 05 69 3c 06 00 	mov    rax,QWORD PTR [rip+0x63c69]        # 0x1800c4d70
   180061107:	ff 50 38             	call   QWORD PTR [rax+0x38]
   18006110a:	c7 05 00 c9 16 00 00 	mov    DWORD PTR [rip+0x16c900],0x0        # 0x1801cda14
   180061111:	00 00 00
   180061114:	48 8b 0d c5 20 06 00 	mov    rcx,QWORD PTR [rip+0x620c5]        # 0x1800c31e0
   18006111b:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   18006111f:	74 06                	je     0x180061127
   180061121:	ff 15 41 bf 02 00    	call   QWORD PTR [rip+0x2bf41]        # 0x18008d068
   180061127:	33 db                	xor    ebx,ebx
   180061129:	e8 82 e0 fb ff       	call   0x18001f1b0
   18006112e:	eb 30                	jmp    0x180061160
   180061130:	48 8d 0d 69 ad 03 00 	lea    rcx,[rip+0x3ad69]        # 0x18009bea0
   180061137:	48 8b 05 32 3c 06 00 	mov    rax,QWORD PTR [rip+0x63c32]        # 0x1800c4d70
   18006113e:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180061141:	c7 05 cd c8 16 00 00 	mov    DWORD PTR [rip+0x16c8cd],0x0        # 0x1801cda18
   180061148:	00 00 00
   18006114b:	48 8b 0d 8e 20 06 00 	mov    rcx,QWORD PTR [rip+0x6208e]        # 0x1800c31e0
   180061152:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180061156:	74 06                	je     0x18006115e
   180061158:	ff 15 0a bf 02 00    	call   QWORD PTR [rip+0x2bf0a]        # 0x18008d068
   18006115e:	33 db                	xor    ebx,ebx
   180061160:	e8 1b c8 fb ff       	call   0x18001d980
   180061165:	eb 30                	jmp    0x180061197
   180061167:	48 8d 0d 7a ad 03 00 	lea    rcx,[rip+0x3ad7a]        # 0x18009bee8
   18006116e:	48 8b 05 fb 3b 06 00 	mov    rax,QWORD PTR [rip+0x63bfb]        # 0x1800c4d70
   180061175:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180061178:	c7 05 9a c8 16 00 00 	mov    DWORD PTR [rip+0x16c89a],0x0        # 0x1801cda1c
   18006117f:	00 00 00
   180061182:	48 8b 0d 57 20 06 00 	mov    rcx,QWORD PTR [rip+0x62057]        # 0x1800c31e0
   180061189:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   18006118d:	74 06                	je     0x180061195
   18006118f:	ff 15 d3 be 02 00    	call   QWORD PTR [rip+0x2bed3]        # 0x18008d068
   180061195:	33 db                	xor    ebx,ebx
   180061197:	e8 e4 2a 00 00       	call   0x180063c80
   18006119c:	eb 4a                	jmp    0x1800611e8
   18006119e:	48 8d 0d 8b ad 03 00 	lea    rcx,[rip+0x3ad8b]        # 0x18009bf30
   1800611a5:	48 8b 05 c4 3b 06 00 	mov    rax,QWORD PTR [rip+0x63bc4]        # 0x1800c4d70
   1800611ac:	ff 50 38             	call   QWORD PTR [rax+0x38]
   1800611af:	c7 05 c7 2c 06 00 00 	mov    DWORD PTR [rip+0x62cc7],0x0        # 0x1800c3e80
   1800611b6:	00 00 00
   1800611b9:	c7 05 91 7d 3e 00 00 	mov    DWORD PTR [rip+0x3e7d91],0x0        # 0x180448f54
   1800611c0:	00 00 00
   1800611c3:	33 c0                	xor    eax,eax
   1800611c5:	87 05 91 7d 3e 00    	xchg   DWORD PTR [rip+0x3e7d91],eax        # 0x180448f5c
   1800611cb:	33 c9                	xor    ecx,ecx
   1800611cd:	87 0d 65 7d 3e 00    	xchg   DWORD PTR [rip+0x3e7d65],ecx        # 0x180448f38
   1800611d3:	48 8b 0d 06 20 06 00 	mov    rcx,QWORD PTR [rip+0x62006]        # 0x1800c31e0
   1800611da:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1800611de:	74 06                	je     0x1800611e6
   1800611e0:	ff 15 82 be 02 00    	call   QWORD PTR [rip+0x2be82]        # 0x18008d068
   1800611e6:	33 db                	xor    ebx,ebx
   1800611e8:	e8 03 d0 fa ff       	call   0x18000e1f0
   1800611ed:	eb 30                	jmp    0x18006121f
   1800611ef:	48 8d 0d 82 ad 03 00 	lea    rcx,[rip+0x3ad82]        # 0x18009bf78
   1800611f6:	48 8b 05 73 3b 06 00 	mov    rax,QWORD PTR [rip+0x63b73]        # 0x1800c4d70
   1800611fd:	ff 50 38             	call   QWORD PTR [rax+0x38]
   180061200:	c7 05 8e fb 21 00 00 	mov    DWORD PTR [rip+0x21fb8e],0x0        # 0x180280d98
   180061207:	00 00 00
   18006120a:	48 8b 0d cf 1f 06 00 	mov    rcx,QWORD PTR [rip+0x61fcf]        # 0x1800c31e0
   180061211:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   180061215:	74 06                	je     0x18006121d
   180061217:	ff 15 4b be 02 00    	call   QWORD PTR [rip+0x2be4b]        # 0x18008d068
   18006121d:	33 db                	xor    ebx,ebx
   18006121f:	33 c9                	xor    ecx,ecx
   180061221:	e8 3a de fc ff       	call   0x18002f060
   180061226:	89 1d 64 fb 21 00    	mov    DWORD PTR [rip+0x21fb64],ebx        # 0x180280d90
   18006122c:	e8 3f 97 fc ff       	call   0x18002a970
   180061231:	48 81 c4 10 01 00 00 	add    rsp,0x110
   180061238:	5b                   	pop    rbx
   180061239:	c3                   	ret
