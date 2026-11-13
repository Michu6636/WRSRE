GNU objdump independent static decode; DLL SHA-256 fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a  Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll

Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll:     file format pei-x86-64


Disassembly of section .text:

0000000180069834 <.text+0x68834>:
   180069834:	39 2d 56 98 05 00    	cmp    DWORD PTR [rip+0x59856],ebp        # 0x1800c3090
   18006983a:	0f 84 33 01 00 00    	je     0x180069973
   180069840:	48 8b 0d 31 b5 05 00 	mov    rcx,QWORD PTR [rip+0x5b531]        # 0x1800c4d78
   180069847:	4c 8d 35 4a 57 04 00 	lea    r14,[rip+0x4574a]        # 0x1800aef98
   18006984e:	48 8d 05 73 e5 11 00 	lea    rax,[rip+0x11e573]        # 0x180187dc8
   180069855:	4c 89 74 24 28       	mov    QWORD PTR [rsp+0x28],r14
   18006985a:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18006985f:	4c 8d 0d 1a f2 ff ff 	lea    r9,[rip+0xfffffffffffff21a]        # 0x180068a80
   180069866:	48 8b 05 03 b5 05 00 	mov    rax,QWORD PTR [rip+0x5b503]        # 0x1800c4d70
   18006986d:	48 8d 15 2c 57 04 00 	lea    rdx,[rip+0x4572c]        # 0x1800aefa0
   180069874:	4d 8b c6             	mov    r8,r14
   180069877:	ff 50 48             	call   QWORD PTR [rax+0x48]
   18006987a:	48 8b 0d f7 b4 05 00 	mov    rcx,QWORD PTR [rip+0x5b4f7]        # 0x1800c4d78
   180069881:	4c 8d 3d 38 57 04 00 	lea    r15,[rip+0x45738]        # 0x1800aefc0
   180069888:	85 c0                	test   eax,eax
   18006988a:	4c 89 7c 24 28       	mov    QWORD PTR [rsp+0x28],r15
   18006988f:	8b f0                	mov    esi,eax
   180069891:	4c 8d 0d a8 f2 ff ff 	lea    r9,[rip+0xfffffffffffff2a8]        # 0x180068b40
   180069898:	48 8d 05 39 e5 11 00 	lea    rax,[rip+0x11e539]        # 0x180187dd8
   18006989f:	8b dd                	mov    ebx,ebp
   1800698a1:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800698a6:	48 8d 15 f3 56 04 00 	lea    rdx,[rip+0x456f3]        # 0x1800aefa0
   1800698ad:	48 8b 05 bc b4 05 00 	mov    rax,QWORD PTR [rip+0x5b4bc]        # 0x1800c4d70
   1800698b4:	4d 8b c7             	mov    r8,r15
   1800698b7:	0f 95 c3             	setne  bl
   1800698ba:	ff 50 48             	call   QWORD PTR [rax+0x48]
   1800698bd:	85 c0                	test   eax,eax
   1800698bf:	74 09                	je     0x1800698ca
   1800698c1:	85 f6                	test   esi,esi
   1800698c3:	8b dd                	mov    ebx,ebp
   1800698c5:	0f 95 c3             	setne  bl
   1800698c8:	ff c3                	inc    ebx
   1800698ca:	48 8b 0d bf b4 05 00 	mov    rcx,QWORD PTR [rip+0x5b4bf]        # 0x1800c4d90
   1800698d1:	48 85 c9             	test   rcx,rcx
   1800698d4:	74 79                	je     0x18006994f
   1800698d6:	48 8d 05 eb 56 04 00 	lea    rax,[rip+0x456eb]        # 0x1800aefc8
   1800698dd:	4d 8b c6             	mov    r8,r14
   1800698e0:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   1800698e5:	4c 8d 0d 34 f1 ff ff 	lea    r9,[rip+0xfffffffffffff134]        # 0x180068a20
   1800698ec:	48 8d 05 dd e4 11 00 	lea    rax,[rip+0x11e4dd]        # 0x180187dd0
   1800698f3:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1800698f8:	48 8d 15 a1 56 04 00 	lea    rdx,[rip+0x456a1]        # 0x1800aefa0
   1800698ff:	48 8b 05 6a b4 05 00 	mov    rax,QWORD PTR [rip+0x5b46a]        # 0x1800c4d70
   180069906:	ff 50 48             	call   QWORD PTR [rax+0x48]
   180069909:	48 8b 0d 80 b4 05 00 	mov    rcx,QWORD PTR [rip+0x5b480]        # 0x1800c4d90
   180069910:	4c 8d 0d c9 f1 ff ff 	lea    r9,[rip+0xfffffffffffff1c9]        # 0x180068ae0
   180069917:	83 f8 01             	cmp    eax,0x1
   18006991a:	48 8d 15 7f 56 04 00 	lea    rdx,[rip+0x4567f]        # 0x1800aefa0
   180069921:	48 8d 05 b0 56 04 00 	lea    rax,[rip+0x456b0]        # 0x1800aefd8
   180069928:	4d 8b c7             	mov    r8,r15
   18006992b:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   180069930:	83 db ff             	sbb    ebx,0xffffffff
   180069933:	48 8d 05 a6 e4 11 00 	lea    rax,[rip+0x11e4a6]        # 0x180187de0
   18006993a:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   18006993f:	48 8b 05 2a b4 05 00 	mov    rax,QWORD PTR [rip+0x5b42a]        # 0x1800c4d70
   180069946:	ff 50 48             	call   QWORD PTR [rax+0x48]
   180069949:	83 f8 01             	cmp    eax,0x1
   18006994c:	83 db ff             	sbb    ebx,0xffffffff
