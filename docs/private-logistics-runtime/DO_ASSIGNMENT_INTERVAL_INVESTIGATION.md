# Native DO-Truck Assignment Interval Investigation

## Executive result

**The smallest credible native interval signal is the reciprocal pair `DO+0xBB0 = truck` and `truck+0xC80 = DO`, observed at its attach and detach boundaries.** The dispatch-side attach and same-receiver detach paths are now proven, including the state-10 cleanup branch. These are native assignment-association transitions, not UI schedule changes.

This is not yet a complete accounting proof: the pair identifies the associated DO, not an individual job; the evidence does not establish that every internal pickup/unload advancement preserves it or that every replacement creates a separately observable detach; and reload continuity is unknown. Use guarded observation of these native boundaries as a candidate, not billing, until those semantics are confirmed.

The helper requested first, `FUN_1406BF1D0`, has now been fully decoded from the verified executable. Its renderer callers pass the selected panel root, and its body aggregates per-root/per-entry quantity data into `root+0x458` and `root+0x470` vectors. It does not expose a proven assignment lifecycle field or native task boundary. The start-side association and a same-receiver reciprocal detach are traced below; exact task semantics across internal stops and replacement remain unresolved.

No plugin, dispatch, permission, default, or billing behavior was changed. No hooks were installed and no live-game run was performed or requested.

## Executable identity — verified

The file at `C:\Workers Resources Soviet Republic\SOVIET64.exe` was read directly.

| Property | Verified value |
|---|---|
| File size | 10,308,608 bytes |
| SHA-256 | `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` |
| PE signature | `0x00004550` |
| PE timestamp | `0x6A3EB6AD` |
| Machine / optional-header magic | AMD64 (`0x8664`) / PE32+ (`0x020B`) |
| Image base | `0x140000000` |

All requested identity values match. The existing `RAW_PE_DATA_READ.md` documents the same path and hash.

## Evidence sources and method

Read the latest interval-policy and helper sections of [VEHICLE_PANEL_FUEL_INVESTIGATION.md](VEHICLE_PANEL_FUEL_INVESTIGATION.md), the relevant renderer call/store evidence in [RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md](RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md), [ghidra-do-storage-findings.md](../ghidra-do-storage-findings.md), and the current [schedule_observer.h](../../plugins/private_logistics/schedule_observer.h).

The workspace has existing Ghidra decompilation/function-listing exports in `scratch`, but no discoverable Ghidra command or project file. The Ghidra function metadata and signature below are from those exports. To avoid relying on the truncated existing `disasm_bf1d0_full.txt`, the full function bytes were independently decoded with Capstone 5.0.7 directly from the identity-verified executable. The decoder consumed the entire Ghidra-reported 3,760-byte function body and ended at its `RET`.

All lifecycle conclusions below are **static deductions or explicit unknowns**. There is no live-game validation of assignment presence, removal, replacement, cancellation, reload survival, or return travel in this investigation.

## `FUN_1406BF1D0`: identity, bounds, signature, and behavior

| Item | Evidence |
|---|---|
| Entry / RVA | `0x1406BF1D0` / `0x6BF1D0` |
| Ghidra function size | 3,760 bytes (`0xEB0`) |
| Body end | `0x1406C007F` inclusive; `RET` |
| Existing Ghidra signature | `void FUN_1406bf1d0(longlong param_1, char param_2, char param_3, char param_4)` |
| Windows x64 argument registers | `RCX`, `DL`, `R8B`, `R9B` respectively |
| Verified binary mapping | `.text`, file offset `0x6BE5D0` |
| Complete decoded body | 909 instructions / 3,760 bytes; no undecoded tail |

The earlier raw export `scratch/disasm_bf1d0_full.txt` stops at `0x1406BFC5F`, even though the body has in-range branches to `0x1406BFFB2`, `0x1406C000B`, and `0x1406C0040`. The complete listing below was regenerated from the matching executable, including those omitted blocks and the epilogue.

Static inspection of the full decompilation and decoded body shows:

- The receiver is used as an aggregation/render context: it reads `param_1+0x390`, iterates `param_1+0x3A0/+0x3A8`, and consumes per-entry vectors at `entry+0x3B8/+0x3C0`.
- It resets/appends derived vectors at `param_1+0x458/+0x460` and `param_1+0x470/+0x478`, including numeric accumulation and configuration-based scaling.
- The body has no accesses to the renderer schedule-list fields `+0x638`, `+0x680/+0x688`, or `+0x698`, and no accesses to the DO target-configuration vector `+0xD38/+0xD40`.
- Its effects are not an assignment install, final removal, cancellation, or replacement boundary. A call to it during rendering is not evidence that a task changed.
- Its callees are vector-growth/copy support, a lookup, and a final cleanup call; none of the observed call sites establish task installation or termination.

### Direct incoming calls present in the available Ghidra instruction export

`scratch/search_C8.asm` contains 22 direct `CALL 0x1406bf1d0` instructions. These are the addresses recorded by the export; no indirect-call target is inferred:

```text
0x14012EECC
0x1401CAB47
0x1401E1E81
0x1402AAEA5
0x1402B1988
0x1402B1C97
0x14033EEC5
0x1403FD7B3
0x14042A103
0x140493F46
0x140493F56
0x140498D7A
0x14049910B
0x14049911B
0x1406955C1
0x1406C0867
0x1406C0975
0x1406C0B4B
0x1407D709A
0x1407D9959
0x1407D9C51
0x1407E9D46
```

The export is not a dedicated Ghidra reference-table dump, so completeness of the program-wide reference set (especially indirect references) is not claimed. The four calls inside the verified vehicle-detail renderer have bounded argument setup below.

## Renderer caller and receiver provenance

The renderer evidence establishes selected vehicle/root `R15 = [window+0x240]`. At each listed call, `RCX` is explicitly loaded from that same `R15`; no unrelated-object offset match is used.

| Call VA | Immediately relevant setup | Callee arguments |
|---|---|---|
| `0x1407D709A` | `R9D=0`; `R8B=1`; `EDX=R8D`; `RCX=R15` | selected root, `param_2=1`, `param_3=1`, `param_4=0` |
| `0x1407D9959` | `R9D=0`; `R8D=0`; `DL=1`; `RCX=R15` | selected root, `1,0,0` |
| `0x1407D9C51` | `R9D=0`; `R8D=0`; `DL=1`; `RCX=R15` | selected root, `1,0,0` |
| `0x1407E9D46` | `R9D=0`; `R8D=0`; `EDX=0`; `RCX=R15` | selected root, `0,0,0` |

At `0x1407D9FF3`, the renderer writes `root+0x698` from `window+0x264`. At `0x1407DA049`, it writes `root+0x610` from a target selected by the rendered row. Those are UI-path stores, not natural completion/removal evidence. `+0x610` and `+0x650` remain opaque context pointers.

## Vehicle schedule and DO task storage are not the same proven object

The selected-root schedule fields currently read by the observer are:

- `root+0x680/+0x688`: raw target-list begin/end.
- `root+0x638`: configuration base; row stride `0x198`.
- `root+0x698`: renderer row index.
- `root+0x5FD`: special last-row configuration suppression flag.
- `root+0x610/+0x650`: opaque context pointers, not verified job identities.

The DO storage findings separately identify `DO+0xC70/+0xC78` as a fleet vector and `DO+0xD38/+0xD40` as target/task-configuration entries. Those entries persist/reconstruct on save load, but are not a verified per-vehicle active-job pointer or assignment history. A candidate/home DO is not the buyer. No receiver-preserving dataflow from the DO configuration vector to a particular truck's active assignment was established here.

## Simulation-side association install candidate

The renderer helper is not the only relevant path. The available dispatcher/decompiler exports establish a native, non-UI association write:

1. In `d_h11_dispatcher.c`, an object whose type metadata has `+0x360 == 0x5C` is passed as `local_648` to `FUN_140189030(param_1, local_648)`.
2. At `0x14018909E`, `FUN_140189030` copies `RDX` to `R13`; at `0x1401890A1` it calls `FUN_1401B1290` without replacing `RCX` or `RDX`. Thus `FUN_1401B1290` receives the same `local_648` object as `param_2`.
3. `FUN_1401B1290` has the exported signature `void FUN_1401b1290(longlong param_1, longlong param_2)` and a Ghidra-reported size of 9,323 bytes. Its loop obtains candidate trucks from `param_2+0xC70`; the decompilation names the current truck `lVar12` and the loop's fleet object is the same `param_2`. This is receiver provenance, not an offset match on an unrelated object.
4. The candidate branch checks `truck+0x4F0 == DO`, `DO+0xBB0 == 0`, and `truck+0xC80 == 0`. `R15` is initialized to zero at `0x1401B1318`.
5. After selecting route/cargo data, the branch installs the reciprocal pointers and resets the truck route state.

Relevant raw instructions from the verified-address export:

```asm
; FUN_140189030: same second argument forwarded to FUN_1401B1290
14018909e: 4C 8B EA                    MOV R13,RDX
1401890a1: E8 EA 81 02 00              CALL 0x1401b1290

; FUN_1401B1290: R15 was zeroed at 0x1401B1318
1401b1315: 4C 8B F1                    MOV R14,RCX
1401b1318: 45 33 FF                    XOR R15D,R15D

; candidate guard; RDI is the current element read from DO+0xC70
1401b225a: 48 8B 86 70 0C 00 00       MOV RAX,[RSI+0xc70]
1401b2261: 4A 8B 3C E8                 MOV RDI,[RAX+R13*0x8]
1401b2416: 4C 8B 87 F0 04 00 00       MOV R8,[RDI+0x4f0]
1401b241d: 4C 3B C6                    CMP R8,RSI
1401b2426: 4C 39 BE B0 0B 00 00       CMP [RSI+0xbb0],R15
1401b2433: 4C 39 BF 80 0C 00 00       CMP [RDI+0xc80],R15

; selected association and route-state reset
1401b258c: 48 89 BE B0 0B 00 00       MOV [RSI+0xbb0],RDI
1401b2593: 48 89 B7 80 0C 00 00       MOV [RDI+0xc80],RSI
1401b259a: 4C 89 BF F0 04 00 00       MOV [RDI+0x4f0],R15
1401b25cf: 48 8D 8F 38 06 00 00       LEA RCX,[RDI+0x638]
1401b25d6: E8 C5 D1 04 00             CALL 0x1401ff7a0
1401b25db: 48 8B 87 80 06 00 00       MOV RAX,[RDI+0x680]
1401b25e2: 48 89 87 88 06 00 00       MOV [RDI+0x688],RAX

; route point copied from the DO-side +0xE90 vector after association
1401b246f: 4C 8D A7 38 07 00 00       LEA R12,[RDI+0x738]
1401b2672: 48 6B DB 1C                 IMUL RBX,RBX,0x1c
1401b2676: 48 8B 86 90 0E 00 00       MOV RAX,[RSI+0xe90]
1401b267d: F3 0F 10 04 03              MOVSS XMM0,[RBX+RAX]
1401b26c1: 48 8B CF                    MOV RCX,RDI
1401b26c4: E8 CA 6B 4D 00              CALL 0x140689290
1401b26c6: 48 8B 96 90 0E 00 00       MOV RDX,[RSI+0xe90]
1401b26cd: 48 03 D3                    ADD RDX,RBX
1401b26d0: 49 8B CC                    MOV RCX,R12
1401b26d3: E8 18 CC E7 FF              CALL 0x14002f2f0
```

In the Windows x64 calling convention, `RSI` is `param_2` (the fleet-bearing dispatcher object), while `RDI` is the candidate fleet vehicle. The relevant decompilation also shows subsequent route-point writes to the same `lVar12`/`RDI` vehicle. The association writes occur before those route writes. This is an assignment-like simulation transition, not a UI-selected-row store.

`FUN_1401B3CE0` contains a similar guarded reciprocal write at `0x1401B45E4`/`0x1401B4604` (`[RSI+0xBB0]=RDI`, `[RDI+0xC80]=RSI`) and then calls `FUN_14067CB30` to reset schedule state. Its immediate caller is `FUN_1401B3700`; this corroborates another association path but is not used to infer that all object types or task modes share one lifecycle.

## Same-receiver detach and candidate close boundary

The relevant cleanup function has exported signature `void FUN_1403fbdc0(longlong param_1, longlong param_2, char param_3, char param_4, char param_5, undefined1 param_6)`. Its raw prologue copies `RDX` (`param_2`) into `RDI`, the receiver used below. At `0x1403FC085`, it loads that receiver's `+0xC80` association. If the vehicle has no state object at `+0x1708`, or its state code at `state+0x294` is not `10`, the reciprocal pair is cleared in this order:

```asm
1403fbdec:  48 8B FA                 MOV RDI,RDX
1403fc07f:  33 DB                    XOR EBX,EBX
1403fc085:  48 8B 87 80 0C 00 00     MOV RAX,[RDI+0xc80]
1403fc08c:  48 85 C0                 TEST RAX,RAX
1403fc08f:  74 2D                    JZ 0x1403fc0be
1403fc091:  48 8B 8F 08 17 00 00     MOV RCX,[RDI+0x1708]
1403fc098:  48 85 C9                 TEST RCX,RCX
1403fc09b:  74 13                    JZ 0x1403fc0b0
1403fc09d:  83 B9 94 02 00 00 0A     CMP dword ptr [RCX+0x294],0xa
1403fc0a4:  75 0A                    JNZ 0x1403fc0b0
1403fc0a6:  48 8B CF                 MOV RCX,RDI
1403fc0a9:  E8 72 0E 2E 00           CALL 0x1406dcf20
1403fc0ae:  EB 0E                    JMP 0x1403fc0be
1403fc0b0:  48 89 98 B0 0B 00 00     MOV [RAX+0xbb0],RBX
1403fc0b7:  48 89 9F 80 0C 00 00     MOV [RDI+0xc80],RBX
```

`RBX` is zero on this path. `RAX` is the DO loaded from the vehicle's `+0xC80`; `RDI` remains the same vehicle passed as `param_2`. Thus the ordinary branch clears `DO+0xBB0` before `truck+0xC80`.

The ordinary branch does not compare `[RAX+0xBB0]` with `RDI` before clearing it. A later observer must validate the reciprocal pair before attributing this write to the candidate assignment.

For state code `10`, the call at `0x1403FC0A9` passes the same vehicle in `RCX` to `FUN_1406DCF20`. Its Ghidra-reported body is 640 bytes (`0x280`), from `0x1406DCF20` through the `RET` at `0x1406DD19F`. The raw body confirms `RBX=RCX` (vehicle) and `RBP=0`; after broader detach work, the tail loads `vehicle+0xC80`, clears the DO back-pointer only if `DO+0xBB0 == vehicle`, then clears `vehicle+0xC80` and `vehicle+0x4F0`. The raw export also shows calls to this helper at `0x1406DB35B` and `0x1406DD6D0`; it is not unique to the FBDC0 state-10 path. The complete body is included below:

```asm
1406dcf20:  48 89 5C 24 18           MOV qword ptr [RSP + 0x18],RBX
1406dcf25:  48 89 6C 24 20           MOV qword ptr [RSP + 0x20],RBP
1406dcf2a:  57                       PUSH RDI
1406dcf2b:  48 83 EC 20              SUB RSP,0x20
1406dcf2f:  48 8B 91 80 0C 00 00     MOV RDX,qword ptr [RCX + 0xc80]
1406dcf36:  33 ED                    XOR EBP,EBP
1406dcf38:  48 8B D9                 MOV RBX,RCX
1406dcf3b:  8B FD                    MOV EDI,EBP
1406dcf3d:  48 85 D2                 TEST RDX,RDX
1406dcf40:  0F 84 4A 02 00 00        JZ 0x1406dd190
1406dcf46:  4C 8D 4C 24 30           LEA R9,[RSP + 0x30]
1406dcf4b:  45 33 C0                 XOR R8D,R8D
1406dcf4e:  E8 5D F9 CD FF           CALL 0x1403bc8b0
1406dcf53:  84 C0                    TEST AL,AL
1406dcf55:  74 3E                    JZ 0x1406dcf95
1406dcf57:  48 39 5C 24 30           CMP qword ptr [RSP + 0x30],RBX
1406dcf5c:  74 15                    JZ 0x1406dcf73
1406dcf5e:  FF C7                    INC EDI
1406dcf60:  4C 8D 4C 24 30           LEA R9,[RSP + 0x30]
1406dcf65:  44 8B C7                 MOV R8D,EDI
1406dcf68:  E8 43 F9 CD FF           CALL 0x1403bc8b0
1406dcf6d:  84 C0                    TEST AL,AL
1406dcf6f:  75 E6                    JNZ 0x1406dcf57
1406dcf71:  EB 22                    JMP 0x1406dcf95
1406dcf73:  85 FF                    TEST EDI,EDI
1406dcf75:  78 1E                    JS 0x1406dcf95
1406dcf77:  4C 8B C2                 MOV R8,RDX
1406dcf7a:  44 8B CF                 MOV R9D,EDI
1406dcf7d:  33 D2                    XOR EDX,EDX
1406dcf7f:  E8 5C FA CD FF           CALL 0x1403bc9e0
1406dcf84:  4C 8B 83 80 0C 00 00     MOV R8,qword ptr [RBX + 0xc80]
1406dcf8b:  44 8B CF                 MOV R9D,EDI
1406dcf8e:  33 D2                    XOR EDX,EDX
1406dcf90:  E8 3B FB CD FF           CALL 0x1403bcad0
1406dcf95:  4C 8B 83 F8 04 00 00     MOV R8,qword ptr [RBX + 0x4f8]
1406dcf9c:  4D 85 C0                 TEST R8,R8
1406dcf9f:  0F 84 B7 01 00 00        JZ 0x1406dd15c
1406dcfa5:  49 8B 80 18 03 00 00     MOV RAX,qword ptr [R8 + 0x318]
1406dcfac:  48 85 C0                 TEST RAX,RAX
1406dcfaf:  0F 84 A7 01 00 00        JZ 0x1406dd15c
1406dcfb5:  83 B8 60 03 00 00 2F     CMP dword ptr [RAX + 0x360],0x2f
1406dcfbc:  0F 85 9A 01 00 00        JNZ 0x1406dd15c
1406dcfc2:  4D 8B 88 78 0C 00 00     MOV R9,qword ptr [R8 + 0xc78]
1406dcfc9:  4D 8B 80 70 0C 00 00     MOV R8,qword ptr [R8 + 0xc70]
1406dcfd0:  48 89 74 24 38           MOV qword ptr [RSP + 0x38],RSI
1406dcfd5:  4D 3B C1                 CMP R8,R9
1406dcfd8:  74 55                    JZ 0x1406dd02f
1406dcfda:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
1406dcfe0:  49 39 18                 CMP qword ptr [R8],RBX
1406dcfe3:  74 09                    JZ 0x1406dcfee
1406dcfe5:  49 83 C0 08              ADD R8,0x8
1406dcfe9:  4D 3B C1                 CMP R8,R9
1406dcfec:  75 F2                    JNZ 0x1406dcfe0
1406dcfee:  4D 3B C1                 CMP R8,R9
1406dcff1:  74 3C                    JZ 0x1406dd02f
1406dcff3:  49 8D 40 08              LEA RAX,[R8 + 0x8]
1406dcff7:  4D 8B D1                 MOV R10,R9
1406dcffa:  4C 2B D0                 SUB R10,RAX
1406dcffd:  48 8B CD                 MOV RCX,RBP
1406dd000:  49 83 C2 07              ADD R10,0x7
1406dd004:  49 C1 EA 03              SHR R10,0x3
1406dd008:  49 3B C1                 CMP RAX,R9
1406dd00b:  4C 0F 47 D5              CMOVA R10,RBP
1406dd00f:  4D 85 D2                 TEST R10,R10
1406dd012:  74 1B                    JZ 0x1406dd02f
1406dd014:  48 8B 10                 MOV RDX,qword ptr [RAX]
1406dd017:  48 3B D3                 CMP RDX,RBX
1406dd01a:  74 07                    JZ 0x1406dd023
1406dd01c:  49 89 10                 MOV qword ptr [R8],RDX
1406dd01f:  49 83 C0 08              ADD R8,0x8
1406dd023:  48 83 C0 08              ADD RAX,0x8
1406dd027:  48 FF C1                 INC RCX
1406dd02a:  49 3B CA                 CMP RCX,R10
1406dd02d:  75 E5                    JNZ 0x1406dd014
1406dd02f:  48 8B 8B F8 04 00 00     MOV RCX,qword ptr [RBX + 0x4f8]
1406dd036:  48 8D 54 24 30           LEA RDX,[RSP + 0x30]
1406dd03b:  48 81 C1 70 0C 00 00     ADD RCX,0xc70
1406dd042:  E8 D9 CB A1 FF           CALL 0x1400f9c20
1406dd047:  48 8B 83 90 04 00 00     MOV RAX,qword ptr [RBX + 0x490]
1406dd04e:  8B F5                    MOV ESI,EBP
1406dd050:  48 2B 83 88 04 00 00     SUB RAX,qword ptr [RBX + 0x488]
1406dd057:  48 C1 F8 03              SAR RAX,0x3
1406dd05b:  48 85 C0                 TEST RAX,RAX
1406dd05e:  0F 84 EC 00 00 00        JZ 0x1406dd150
1406dd064:  48 8B CD                 MOV RCX,RBP
1406dd067:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
1406dd070:  4C 8B 9B 88 04 00 00     MOV R11,qword ptr [RBX + 0x488]
1406dd077:  48 8D 3C CD 00 00 00 00  LEA RDI,[RCX*0x8]
1406dd07f:  49 8B 04 3B              MOV RAX,qword ptr [R11 + RDI*0x1]
1406dd083:  4C 8B 80 F8 04 00 00     MOV R8,qword ptr [RAX + 0x4f8]
1406dd08a:  4D 85 C0                 TEST R8,R8
1406dd08d:  0F 84 8C 00 00 00        JZ 0x1406dd11f
1406dd093:  4D 8B 88 78 0C 00 00     MOV R9,qword ptr [R8 + 0xc78]
1406dd09a:  4D 8B 80 70 0C 00 00     MOV R8,qword ptr [R8 + 0xc70]
1406dd0a1:  4D 3B C1                 CMP R8,R9
1406dd0a4:  74 56                    JZ 0x1406dd0fc
1406dd0a6:  49 39 00                 CMP qword ptr [R8],RAX
1406dd0a9:  74 09                    JZ 0x1406dd0b4
1406dd0ab:  49 83 C0 08              ADD R8,0x8
1406dd0af:  4D 3B C1                 CMP R8,R9
1406dd0b2:  75 F2                    JNZ 0x1406dd0a6
1406dd0b4:  4D 3B C1                 CMP R8,R9
1406dd0b7:  74 43                    JZ 0x1406dd0fc
1406dd0b9:  49 8D 40 08              LEA RAX,[R8 + 0x8]
1406dd0bd:  4D 8B D1                 MOV R10,R9
1406dd0c0:  4C 2B D0                 SUB R10,RAX
1406dd0c3:  48 8B D5                 MOV RDX,RBP
1406dd0c6:  49 83 C2 07              ADD R10,0x7
1406dd0ca:  49 C1 EA 03              SHR R10,0x3
1406dd0ce:  49 3B C1                 CMP RAX,R9
1406dd0d1:  4C 0F 47 D5              CMOVA R10,RBP
1406dd0d5:  4D 85 D2                 TEST R10,R10
1406dd0d8:  74 22                    JZ 0x1406dd0fc
1406dd0da:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
1406dd0e0:  48 8B 08                 MOV RCX,qword ptr [RAX]
1406dd0e3:  49 3B 0C 3B              CMP RCX,qword ptr [R11 + RDI*0x1]
1406dd0e7:  74 07                    JZ 0x1406dd0f0
1406dd0e9:  49 89 08                 MOV qword ptr [R8],RCX
1406dd0ec:  49 83 C0 08              ADD R8,0x8
1406dd0f0:  48 83 C0 08              ADD RAX,0x8
1406dd0f4:  48 FF C2                 INC RDX
1406dd0f7:  49 3B D2                 CMP RDX,R10
1406dd0fa:  75 E4                    JNZ 0x1406dd0e0
1406dd0fc:  48 8B 83 88 04 00 00     MOV RAX,qword ptr [RBX + 0x488]
1406dd103:  48 8D 54 24 30           LEA RDX,[RSP + 0x30]
1406dd108:  48 8B 0C 07              MOV RCX,qword ptr [RDI + RAX*0x1]
1406dd10c:  48 8B 89 F8 04 00 00     MOV RCX,qword ptr [RCX + 0x4f8]
1406dd113:  48 81 C1 70 0C 00 00     ADD RCX,0xc70
1406dd11a:  E8 01 CB A1 FF           CALL 0x1400f9c20
1406dd11f:  48 8B 83 88 04 00 00     MOV RAX,qword ptr [RBX + 0x488]
1406dd126:  FF C6                    INC ESI
1406dd128:  48 8B 0C 07              MOV RCX,qword ptr [RDI + RAX*0x1]
1406dd12c:  48 89 A9 F8 04 00 00     MOV qword ptr [RCX + 0x4f8],RBP
1406dd133:  48 8B 83 90 04 00 00     MOV RAX,qword ptr [RBX + 0x490]
1406dd13a:  48 2B 83 88 04 00 00     SUB RAX,qword ptr [RBX + 0x488]
1406dd141:  48 C1 F8 03              SAR RAX,0x3
1406dd145:  8B CE                    MOV ECX,ESI
1406dd147:  48 3B C8                 CMP RCX,RAX
1406dd14a:  0F 82 20 FF FF FF        JC 0x1406dd070
1406dd150:  48 8B 74 24 38           MOV RSI,qword ptr [RSP + 0x38]
1406dd155:  48 89 AB F8 04 00 00     MOV qword ptr [RBX + 0x4f8],RBP
1406dd15c:  48 8B 83 80 0C 00 00     MOV RAX,qword ptr [RBX + 0xc80]
1406dd163:  C7 83 6C 05 00 00 FF FF FF FF MOV dword ptr [RBX + 0x56c],0xffffffff
1406dd16d:  48 85 C0                 TEST RAX,RAX
1406dd170:  74 17                    JZ 0x1406dd189
1406dd172:  48 39 98 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],RBX
1406dd179:  75 07                    JNZ 0x1406dd182
1406dd17b:  48 89 A8 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],RBP
1406dd182:  48 89 AB 80 0C 00 00     MOV qword ptr [RBX + 0xc80],RBP
1406dd189:  48 89 AB F0 04 00 00     MOV qword ptr [RBX + 0x4f0],RBP
1406dd190:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
1406dd195:  48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
1406dd19a:  48 83 C4 20              ADD RSP,0x20
1406dd19e:  5F                       POP RDI
1406dd19f:  C3                       RET
```

### Why the reciprocal pair is not yet a complete interval signal

- The pair records a DO/vehicle association, not a stable task identity. Two consecutive tasks for the same DO are distinguishable only if the pair actually detaches between them and that transition is observed; an uninterrupted same-DO pair cannot identify a task replacement.
- The install branches require an empty pair. This proves those branches do not directly overwrite an existing association, but it does not rule out a detach/reattach in another path or a same-update transition that a polling snapshot could miss.
- The reciprocal clears in `FUN_1403FBDC0` and `FUN_1406DCF20` are real same-receiver detach boundaries, but the cleanup is reused from update/configuration paths as well as dispatch-related code. For example, `FUN_1401C1770` processes DO target configurations and calls `FUN_1403FBDC0` on a fleet vehicle. The clear is not a success marker, nor does every callsite alone prove why the association ended. Under the requested policy it is a close only when that association represents the assigned task and the detach is not an internal stop transition.
- `FUN_14067CB30` resets schedule/list state without writing `truck+0xC80`; this helps distinguish schedule reset from the reciprocal association. Its complete 264-byte body shows the reset paths explicitly:

```asm
14067cb30:  48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
14067cb35:  57                       PUSH RDI
14067cb36:  48 83 EC 20              SUB RSP,0x20
14067cb3a:  41 0F B6 F8              MOVZX EDI,R8B
14067cb3e:  48 8B D9                 MOV RBX,RCX
14067cb41:  84 D2                    TEST DL,DL
14067cb43:  74 38                    JZ 0x14067cb7d
14067cb45:  48 83 B9 80 0C 00 00 00  CMP qword ptr [RCX + 0xc80],0x0
14067cb4d:  75 0A                    JNZ 0x14067cb59
14067cb4f:  48 83 B9 F0 04 00 00 00  CMP qword ptr [RCX + 0x4f0],0x0
14067cb57:  75 24                    JNZ 0x14067cb7d
14067cb59:  48 8B 81 80 06 00 00     MOV RAX,qword ptr [RCX + 0x680]
14067cb60:  48 89 81 88 06 00 00     MOV qword ptr [RCX + 0x688],RAX
14067cb67:  48 81 C1 38 06 00 00     ADD RCX,0x638
14067cb6e:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
14067cb73:  48 83 C4 20              ADD RSP,0x20
14067cb77:  5F                       POP RDI
14067cb78:  E9 23 2C B8 FF           JMP 0x1401ff7a0
14067cb7d:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
14067cb84:  48 85 C0                 TEST RAX,RAX
14067cb87:  74 10                    JZ 0x14067cb99
14067cb89:  83 B8 94 02 00 00 04     CMP dword ptr [RAX + 0x294],0x4
14067cb90:  75 07                    JNZ 0x14067cb99
14067cb92:  B2 01                    MOV DL,0x1
14067cb94:  E8 87 9B 02 00           CALL 0x1406a6720
14067cb99:  40 84 FF                 TEST DIL,DIL
14067cb9c:  74 29                    JZ 0x14067cbc7
14067cb9e:  48 8B 0D 6B AB 36 00     MOV RCX,qword ptr [0x1409e7710]
14067cba5:  48 8B D3                 MOV RDX,RBX
14067cba8:  E8 A3 AD E1 FF           CALL 0x140497950
14067cbad:  48 8D 8B 38 06 00 00     LEA RCX,[RBX + 0x638]
14067cbb4:  E8 E7 2B B8 FF           CALL 0x1401ff7a0
14067cbb9:  48 8B 83 80 06 00 00     MOV RAX,qword ptr [RBX + 0x680]
14067cbc0:  48 89 83 88 06 00 00     MOV qword ptr [RBX + 0x688],RAX
14067cbc7:  48 8B 83 A0 06 00 00     MOV RAX,qword ptr [RBX + 0x6a0]
14067cbce:  33 FF                    XOR EDI,EDI
14067cbd0:  48 89 83 A8 06 00 00     MOV qword ptr [RBX + 0x6a8],RAX
14067cbd7:  48 8B 83 B8 06 00 00     MOV RAX,qword ptr [RBX + 0x6b8]
14067cbde:  48 89 83 C0 06 00 00     MOV qword ptr [RBX + 0x6c0],RAX
14067cbe5:  48 8B 83 38 07 00 00     MOV RAX,qword ptr [RBX + 0x738]
14067cbec:  48 89 83 40 07 00 00     MOV qword ptr [RBX + 0x740],RAX
14067cbf3:  48 8B 83 50 07 00 00     MOV RAX,qword ptr [RBX + 0x750]
14067cbfa:  48 89 83 58 07 00 00     MOV qword ptr [RBX + 0x758],RAX
14067cc01:  48 89 BB 98 07 00 00     MOV qword ptr [RBX + 0x798],RDI
14067cc08:  48 8B 83 C0 07 00 00     MOV RAX,qword ptr [RBX + 0x7c0]
14067cc0f:  48 2B 83 B8 07 00 00     SUB RAX,qword ptr [RBX + 0x7b8]
14067cc16:  48 C1 F8 03              SAR RAX,0x3
14067cc1a:  48 85 C0                 TEST RAX,RAX
14067cc1d:  74 08                    JZ 0x14067cc27
14067cc1f:  48 8B CB                 MOV RCX,RBX
14067cc22:  E8 49 4D 00 00           CALL 0x140681970
14067cc27:  89 BB 34 0D 00 00        MOV dword ptr [RBX + 0xd34],EDI
14067cc2d:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
14067cc32:  48 83 C4 20              ADD RSP,0x20
14067cc36:  5F                       POP RDI
14067cc37:  C3                       RET
```

This proves only that this helper's reset does not clear the association; it does not prove that every pickup/unload advancement preserves the pair.
- `FUN_1406B8EB0` and other functions also write `+0xC80`, but their aliases and call ordering are not classified as DO-task transitions here. Do not treat every program-wide `+0xC80` store as an assignment boundary.

## Interval-signal test matrix

| Required state or transition | What static evidence proves | Interval conclusion |
|---|---|---|
| Idle at a DO | The install candidate requires `truck+0xC80 == 0` and `DO+0xBB0 == 0`; this is an eligibility guard, not a complete idle-state definition. | Candidate idle marker only; default/return rows are unresolved. |
| Assigned but not yet loaded | The reciprocal pair is installed before route-state reset/rebuild, but no loading-state mapping was established. | Native start-boundary candidate; loaded-state relation unknown. |
| Internal pickup/unload stop advancement | `FUN_14067CB30` can reset schedule/list state without touching `+0xC80`; no trace proves the pair stays stable through every internal stop. | Do not use row/index changes as a boundary; continuity remains unproven. |
| Final task removal | `FUN_1403FBDC0` clears the reciprocal pair for the ordinary state branch; state `10` delegates to `FUN_1406DCF20`, which also clears it on the same vehicle. | Verified association-detach boundary candidate; the lifecycle reason is not established at every caller. |
| Cancellation or replacement | The setter requires empty pointers, and same-receiver detach paths exist. No proof covers every replacement path or whether detach/reattach can occur within one update. | Observe native boundary events, not just snapshots; replacement semantics remain incomplete. |
| Empty return | The pair's clear is a potential cutoff, but no mapping ties it to a particular return route phase. | Travel after a verified task detach is outside; whether the observed clear precedes all return travel remains unresolved. |
| Identical consecutive assignments | `truck+0xC80` points to the DO, not an order/job ID. | Same-DO tasks can be separated only when a detach/reattach boundary is actually observed. |
| Save/reload | DO target configurations are serialized/reconstructed; persistence/reconstruction of the reciprocal pointers and assignment continuity is not established. | No post-load interval rule can be claimed. |

Therefore the requested interval rule is clear, but no native observation currently implements it: begin on actual assignment to a vehicle working for a DO, remain through internal stops and travel while assigned, close on actual task removal/cancellation/replacement, and exclude travel after removal. Success is not needed to close it.

## Read-only recommendation, destination, and payer

The current schedule observer's bounded fields are useful diagnostics but **insufficient for interval accounting**. A list clear/change, nonempty list, row index, config/storage relocation, opaque UI context, invalid read, or panel closure is not a verified assignment event. A snapshot of `truck+0xC80` plus reciprocal `DO+0xBB0` can show the current association, but can miss a same-update detach/reattach and cannot distinguish uninterrupted same-DO tasks.

For later guarded observation only, the verified start candidates are `FUN_1401B1290` at `0x1401B258C`/`0x1401B2593` and the similar `FUN_1401B3CE0` writes at `0x1401B45E4`/`0x1401B4604`. The verified close candidates on the same vehicle receiver are `FUN_1403FBDC0` at `0x1403FC0B0`/`0x1403FC0B7` for the non-10 state path, and the state-10 path `0x1403FC0A9` into `FUN_1406DCF20`, whose reciprocal clears are at `0x1406DD17B`/`0x1406DD182`. A future observer must validate the vehicle/DO reciprocal pair and task context at these boundaries; the generic cleanup call alone is not a success/cancellation classification. No hook is installed here. Do not hook `FUN_1406BF1D0`.

Adjacent route evidence reads point data from `DO+0xE90` and feeds it to `FUN_140689290`/the vehicle route-point vector; that establishes route construction only, not a buyer or payer. No destination-to-payer mapping is verified; cancelled work must not be charged to a guessed party.

## Smallest missing artifact

The helper's full body, start-side attach, and same-receiver detach instructions are no longer missing. The smallest remaining artifact is a **same-vehicle simulation trace/CFG spanning assignment, internal pickup/unload advancement, replacement, and final detach**, proving whether the reciprocal pair survives every internal stop and whether every replacement emits a separately observable detach. A save/load trace is additionally required before claiming that an interval can continue across reload.

Static evidence cannot by itself validate whether idle/default/return entries are retained, whether identical tasks replace without an empty state, or whether the association survives reload. These remain unvalidated; no unchanged game run is requested.

## Analysis script and actual output

The following complete local script verifies the file identity, maps RVA `0x6BF1D0` to `.text`, and prints the complete addressed bytes/disassembly. It requires `capstone==5.0.7` and performs no writes to the executable or game.

```python
from pathlib import Path
import hashlib
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_64

exe = Path(r"C:\Workers Resources Soviet Republic\SOVIET64.exe")
expected_sha256 = "296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8"
expected_timestamp = 0x6A3EB6AD
expected_image_base = 0x140000000
rva, size = 0x6BF1D0, 3760
data = exe.read_bytes()
sha256 = hashlib.sha256(data).hexdigest().upper()
pe = struct.unpack_from("<I", data, 0x3C)[0]
timestamp = struct.unpack_from("<I", data, pe + 8)[0]
section_count = struct.unpack_from("<H", data, pe + 6)[0]
optional_size = struct.unpack_from("<H", data, pe + 20)[0]
optional = pe + 24
image_base = struct.unpack_from("<Q", data, optional + 24)[0]
assert sha256 == expected_sha256
assert timestamp == expected_timestamp and image_base == expected_image_base
section_table = optional + optional_size
sections = []
for index in range(section_count):
    offset = section_table + index * 40
    name = data[offset:offset + 8].split(b"\0", 1)[0].decode("ascii")
    va, raw_size, raw_offset = struct.unpack_from("<III", data, offset + 12)
    sections.append((name, va, raw_size, raw_offset))
name, va, raw_size, raw_offset = next(
    item for item in sections if item[1] <= rva < item[1] + item[2]
)
file_offset = raw_offset + rva - va
body = data[file_offset:file_offset + size]
assert len(body) == size and name == ".text"
decoder = Cs(CS_ARCH_X86, CS_MODE_64)
instructions = list(decoder.disasm(body, image_base + rva))
assert sum(item.size for item in instructions) == size
assert instructions[-1].address == image_base + rva + size - 1
assert instructions[-1].mnemonic == "ret"
print("SHA256", sha256)
print("PE_TIMESTAMP", hex(timestamp), "IMAGE_BASE", hex(image_base))
print("FUNCTION", hex(image_base + rva), "SIZE", size,
      "FILE_OFFSET", hex(file_offset), "SECTION", name)
print("DECODED", len(instructions), "BYTES",
      sum(item.size for item in instructions), "COMPLETE")
for item in instructions:
    raw = item.bytes.hex(" ").upper()
    print(f"{item.address:010X}: {raw:<29} {item.mnemonic.upper():<8} {item.op_str}")
```

Actual summary output:

```text
SHA256 296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8
PE_TIMESTAMP 0x6a3eb6ad IMAGE_BASE 0x140000000
FUNCTION 0x1406bf1d0 Ghidra_size 3760 file_offset 0x6be5d0 section .text
DECODED 909 BYTES 3760 COMPLETE True LAST 0x1406c007f ret
```

The complete function body follows; addresses are VAs, and each line includes the bytes decoded at that address.

```asm
01406BF1D0: 48 8B C4                      MOV      rax, rsp
01406BF1D3: 44 88 48 20                   MOV      byte ptr [rax + 0x20], r9b
01406BF1D7: 44 88 40 18                   MOV      byte ptr [rax + 0x18], r8b
01406BF1DB: 88 50 10                      MOV      byte ptr [rax + 0x10], dl
01406BF1DE: 48 89 48 08                   MOV      qword ptr [rax + 8], rcx
01406BF1E2: 55                            PUSH     rbp
01406BF1E3: 53                            PUSH     rbx
01406BF1E4: 56                            PUSH     rsi
01406BF1E5: 57                            PUSH     rdi
01406BF1E6: 41 54                         PUSH     r12
01406BF1E8: 41 55                         PUSH     r13
01406BF1EA: 41 56                         PUSH     r14
01406BF1EC: 41 57                         PUSH     r15
01406BF1EE: 48 8D 68 A1                   LEA      rbp, [rax - 0x5f]
01406BF1F2: 48 81 EC D8 00 00 00          SUB      rsp, 0xd8
01406BF1F9: 48 C7 45 E7 FE FF FF FF       MOV      qword ptr [rbp - 0x19], 0xfffffffffffffffe
01406BF201: 0F 29 70 A8                   MOVAPS   xmmword ptr [rax - 0x58], xmm6
01406BF205: 0F 29 78 98                   MOVAPS   xmmword ptr [rax - 0x68], xmm7
01406BF209: 44 0F B6 EA                   MOVZX    r13d, dl
01406BF20D: 4C 8B D1                      MOV      r10, rcx
01406BF210: 41 B4 01                      MOV      r12b, 1
01406BF213: 44 88 64 24 20                MOV      byte ptr [rsp + 0x20], r12b
01406BF218: 45 33 C0                      XOR      r8d, r8d
01406BF21B: 4C 89 45 C7                   MOV      qword ptr [rbp - 0x39], r8
01406BF21F: 45 33 F6                      XOR      r14d, r14d
01406BF222: 4C 89 75 CF                   MOV      qword ptr [rbp - 0x31], r14
01406BF226: 45 33 DB                      XOR      r11d, r11d
01406BF229: 4C 89 5D A7                   MOV      qword ptr [rbp - 0x59], r11
01406BF22D: 4C 89 44 24 28                MOV      qword ptr [rsp + 0x28], r8
01406BF232: 4C 89 44 24 30                MOV      qword ptr [rsp + 0x30], r8
01406BF237: 0F 57 C0                      XORPS    xmm0, xmm0
01406BF23A: F3 0F 7F 44 24 38             MOVDQU   xmmword ptr [rsp + 0x38], xmm0
01406BF240: 33 D2                         XOR      edx, edx
01406BF242: 48 89 55 AF                   MOV      qword ptr [rbp - 0x51], rdx
01406BF246: 48 89 55 8F                   MOV      qword ptr [rbp - 0x71], rdx
01406BF24A: 48 8D 99 58 04 00 00          LEA      rbx, [rcx + 0x458]
01406BF251: 48 8B 03                      MOV      rax, qword ptr [rbx]
01406BF254: 48 89 43 08                   MOV      qword ptr [rbx + 8], rax
01406BF258: 48 8D B9 70 04 00 00          LEA      rdi, [rcx + 0x470]
01406BF25F: 48 8B 07                      MOV      rax, qword ptr [rdi]
01406BF262: 48 89 47 08                   MOV      qword ptr [rdi + 8], rax
01406BF266: 45 84 C9                      TEST     r9b, r9b
01406BF269: 74 16                         JE       0x1406bf281
01406BF26B: 4C 8B 05 2E 20 32 00          MOV      r8, qword ptr [rip + 0x32202e]
01406BF272: 4C 89 45 C7                   MOV      qword ptr [rbp - 0x39], r8
01406BF276: 4C 8B 35 1B 20 32 00          MOV      r14, qword ptr [rip + 0x32201b]
01406BF27D: 4C 89 75 CF                   MOV      qword ptr [rbp - 0x31], r14
01406BF281: C7 45 9F FF FF FF FF          MOV      dword ptr [rbp - 0x61], 0xffffffff
01406BF288: 48 8B 81 A8 03 00 00          MOV      rax, qword ptr [rcx + 0x3a8]
01406BF28F: 48 2B 81 A0 03 00 00          SUB      rax, qword ptr [rcx + 0x3a0]
01406BF296: 48 C1 F8 03                   SAR      rax, 3
01406BF29A: 83 F8 FF                      CMP      eax, -1
01406BF29D: 0F 8E 9D 0D 00 00             JLE      0x1406c0040
01406BF2A3: 48 83 C9 FF                   OR       rcx, 0xffffffffffffffff
01406BF2A7: 48 89 4D DF                   MOV      qword ptr [rbp - 0x21], rcx
01406BF2AB: 48 8B 35 BE 7C 32 00          MOV      rsi, qword ptr [rip + 0x327cbe]
01406BF2B2: 0F 1F 40 00                   NOP      dword ptr [rax]
01406BF2B6: 66 66 0F 1F 84 00 00 00 00 00 NOP      word ptr [rax + rax]
01406BF2C0: 4D 8B BA 90 03 00 00          MOV      r15, qword ptr [r10 + 0x390]
01406BF2C7: 4C 89 7D D7                   MOV      qword ptr [rbp - 0x29], r15
01406BF2CB: 48 85 C9                      TEST     rcx, rcx
01406BF2CE: 78 0F                         JS       0x1406bf2df
01406BF2D0: 49 8B 82 A0 03 00 00          MOV      rax, qword ptr [r10 + 0x3a0]
01406BF2D7: 4C 8B 3C C8                   MOV      r15, qword ptr [rax + rcx*8]
01406BF2DB: 4C 89 7D D7                   MOV      qword ptr [rbp - 0x29], r15
01406BF2DF: C7 45 9B FF FF FF FF          MOV      dword ptr [rbp - 0x65], 0xffffffff
01406BF2E6: 48 83 CA FF                   OR       rdx, 0xffffffffffffffff
01406BF2EA: 48 89 55 BF                   MOV      qword ptr [rbp - 0x41], rdx
01406BF2EE: 49 8B 87 90 04 00 00          MOV      rax, qword ptr [r15 + 0x490]
01406BF2F5: 49 2B 87 88 04 00 00          SUB      rax, qword ptr [r15 + 0x488]
01406BF2FC: 48 C1 F8 03                   SAR      rax, 3
01406BF300: 3B C2                         CMP      eax, edx
01406BF302: 0F 8E 03 0D 00 00             JLE      0x1406c000b
01406BF308: 0F 1F 84 00 00 00 00 00       NOP      dword ptr [rax + rax]
01406BF310: 4C 89 7D B7                   MOV      qword ptr [rbp - 0x49], r15
01406BF314: 48 85 D2                      TEST     rdx, rdx
01406BF317: 78 1C                         JS       0x1406bf335
01406BF319: 45 84 ED                      TEST     r13b, r13b
01406BF31C: 0F 84 A4 0C 00 00             JE       0x1406bffc6
01406BF322: 48 8B 45 D7                   MOV      rax, qword ptr [rbp - 0x29]
01406BF326: 48 8B 80 88 04 00 00          MOV      rax, qword ptr [rax + 0x488]
01406BF32D: 4C 8B 3C D0                   MOV      r15, qword ptr [rax + rdx*8]
01406BF331: 4C 89 7D B7                   MOV      qword ptr [rbp - 0x49], r15
01406BF335: 45 84 C9                      TEST     r9b, r9b
01406BF338: 74 26                         JE       0x1406bf360
01406BF33A: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BF341: 8B 88 00 86 00 00             MOV      ecx, dword ptr [rax + 0x8600]
01406BF347: 83 F9 10                      CMP      ecx, 0x10
01406BF34A: 75 09                         JNE      0x1406bf355
01406BF34C: 4D 8B D8                      MOV      r11, r8
01406BF34F: 4C 89 45 A7                   MOV      qword ptr [rbp - 0x59], r8
01406BF353: EB 0B                         JMP      0x1406bf360
01406BF355: 83 F9 0F                      CMP      ecx, 0xf
01406BF358: 4D 0F 44 DE                   CMOVE    r11, r14
01406BF35C: 4C 89 5D A7                   MOV      qword ptr [rbp - 0x59], r11
01406BF360: 83 C9 FF                      OR       ecx, 0xffffffff
01406BF363: 89 4D 97                      MOV      dword ptr [rbp - 0x69], ecx
01406BF366: 48 85 D2                      TEST     rdx, rdx
01406BF369: 79 16                         JNS      0x1406bf381
01406BF36B: 41 8B 87 94 17 00 00          MOV      eax, dword ptr [r15 + 0x1794]
01406BF372: 85 C0                         TEST     eax, eax
01406BF374: 74 1F                         JE       0x1406bf395
01406BF376: 83 F8 F6                      CMP      eax, -0xa
01406BF379: 7E 1A                         JLE      0x1406bf395
01406BF37B: 80 7D 77 00                   CMP      byte ptr [rbp + 0x77], 0
01406BF37F: 75 14                         JNE      0x1406bf395
01406BF381: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BF388: 83 B8 94 02 00 00 09          CMP      dword ptr [rax + 0x294], 9
01406BF38F: 0F 85 31 0C 00 00             JNE      0x1406bffc6
01406BF395: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BF39C: 48 85 C0                      TEST     rax, rax
01406BF39F: 0F 84 A5 04 00 00             JE       0x1406bf84a
01406BF3A5: 83 B8 00 86 00 00 11          CMP      dword ptr [rax + 0x8600], 0x11
01406BF3AC: 0F 85 98 04 00 00             JNE      0x1406bf84a
01406BF3B2: 4C 8B 6D 87                   MOV      r13, qword ptr [rbp - 0x79]
01406BF3B6: 45 84 E4                      TEST     r12b, r12b
01406BF3B9: 0F 84 D4 01 00 00             JE       0x1406bf593
01406BF3BF: C6 44 24 20 00                MOV      byte ptr [rsp + 0x20], 0
01406BF3C4: 48 C7 44 24 30 00 00 00 00    MOV      qword ptr [rsp + 0x30], 0
01406BF3CD: 45 33 F6                      XOR      r14d, r14d
01406BF3D0: 48 8B 05 A1 7B 32 00          MOV      rax, qword ptr [rip + 0x327ba1]
01406BF3D7: 48 2B C6                      SUB      rax, rsi
01406BF3DA: 48 C1 F8 03                   SAR      rax, 3
01406BF3DE: 85 C0                         TEST     eax, eax
01406BF3E0: 0F 84 AD 01 00 00             JE       0x1406bf593
01406BF3E6: 4C 8B 7C 24 38                MOV      r15, qword ptr [rsp + 0x38]
01406BF3EB: 0F 1F 44 00 00                NOP      dword ptr [rax + rax]
01406BF3F0: 4E 8B 24 F6                   MOV      r12, qword ptr [rsi + r14*8]
01406BF3F4: 4C 89 64 24 28                MOV      qword ptr [rsp + 0x28], r12
01406BF3F9: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF3FD: 48 8D 4C 24 28                LEA      rcx, [rsp + 0x28]
01406BF402: 0F 10 74 24 28                MOVUPS   xmm6, xmmword ptr [rsp + 0x28]
01406BF407: 48 3B C8                      CMP      rcx, rax
01406BF40A: 73 40                         JAE      0x1406bf44c
01406BF40C: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BF40F: 48 8D 54 24 28                LEA      rdx, [rsp + 0x28]
01406BF414: 48 3B CA                      CMP      rcx, rdx
01406BF417: 77 33                         JA       0x1406bf44c
01406BF419: 48 8D 74 24 28                LEA      rsi, [rsp + 0x28]
01406BF41E: 48 2B F1                      SUB      rsi, rcx
01406BF421: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BF425: 75 0D                         JNE      0x1406bf434
01406BF427: BA 01 00 00 00                MOV      edx, 1
01406BF42C: 48 8B CB                      MOV      rcx, rbx
01406BF42F: E8 EC 27 9F FF                CALL     0x1400b1c20
01406BF434: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF438: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BF43C: 48 03 33                      ADD      rsi, qword ptr [rbx]
01406BF43F: 48 85 C0                      TEST     rax, rax
01406BF442: 74 27                         JE       0x1406bf46b
01406BF444: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BF447: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BF44A: EB 1F                         JMP      0x1406bf46b
01406BF44C: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BF450: 75 0D                         JNE      0x1406bf45f
01406BF452: BA 01 00 00 00                MOV      edx, 1
01406BF457: 48 8B CB                      MOV      rcx, rbx
01406BF45A: E8 C1 27 9F FF                CALL     0x1400b1c20
01406BF45F: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF463: 48 85 C0                      TEST     rax, rax
01406BF466: 74 03                         JE       0x1406bf46b
01406BF468: 0F 11 30                      MOVUPS   xmmword ptr [rax], xmm6
01406BF46B: 48 83 43 08 10                ADD      qword ptr [rbx + 8], 0x10
01406BF470: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF474: 48 8D 4C 24 28                LEA      rcx, [rsp + 0x28]
01406BF479: 48 3B C8                      CMP      rcx, rax
01406BF47C: 73 40                         JAE      0x1406bf4be
01406BF47E: 48 8B 0F                      MOV      rcx, qword ptr [rdi]
01406BF481: 48 8D 54 24 28                LEA      rdx, [rsp + 0x28]
01406BF486: 48 3B CA                      CMP      rcx, rdx
01406BF489: 77 33                         JA       0x1406bf4be
01406BF48B: 48 8D 74 24 28                LEA      rsi, [rsp + 0x28]
01406BF490: 48 2B F1                      SUB      rsi, rcx
01406BF493: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF497: 75 0D                         JNE      0x1406bf4a6
01406BF499: BA 01 00 00 00                MOV      edx, 1
01406BF49E: 48 8B CF                      MOV      rcx, rdi
01406BF4A1: E8 7A 27 9F FF                CALL     0x1400b1c20
01406BF4A6: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF4AA: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BF4AE: 48 03 37                      ADD      rsi, qword ptr [rdi]
01406BF4B1: 48 85 C0                      TEST     rax, rax
01406BF4B4: 74 27                         JE       0x1406bf4dd
01406BF4B6: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BF4B9: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BF4BC: EB 1F                         JMP      0x1406bf4dd
01406BF4BE: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF4C2: 75 0D                         JNE      0x1406bf4d1
01406BF4C4: BA 01 00 00 00                MOV      edx, 1
01406BF4C9: 48 8B CF                      MOV      rcx, rdi
01406BF4CC: E8 4F 27 9F FF                CALL     0x1400b1c20
01406BF4D1: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF4D5: 48 85 C0                      TEST     rax, rax
01406BF4D8: 74 03                         JE       0x1406bf4dd
01406BF4DA: 0F 11 30                      MOVUPS   xmmword ptr [rax], xmm6
01406BF4DD: 48 83 47 08 10                ADD      qword ptr [rdi + 8], 0x10
01406BF4E2: 48 8D 44 24 28                LEA      rax, [rsp + 0x28]
01406BF4E7: 49 3B C5                      CMP      rax, r13
01406BF4EA: 73 4B                         JAE      0x1406bf537
01406BF4EC: 48 8D 44 24 28                LEA      rax, [rsp + 0x28]
01406BF4F1: 4C 3B F8                      CMP      r15, rax
01406BF4F4: 77 41                         JA       0x1406bf537
01406BF4F6: 48 8D 74 24 28                LEA      rsi, [rsp + 0x28]
01406BF4FB: 49 2B F7                      SUB      rsi, r15
01406BF4FE: 48 C1 FE 03                   SAR      rsi, 3
01406BF502: 4C 3B 6D AF                   CMP      r13, qword ptr [rbp - 0x51]
01406BF506: 75 20                         JNE      0x1406bf528
01406BF508: BA 01 00 00 00                MOV      edx, 1
01406BF50D: 48 8D 4C 24 38                LEA      rcx, [rsp + 0x38]
01406BF512: E8 E9 21 9F FF                CALL     0x1400b1700
01406BF517: 48 8B 45 8F                   MOV      rax, qword ptr [rbp - 0x71]
01406BF51B: 48 89 45 AF                   MOV      qword ptr [rbp - 0x51], rax
01406BF51F: 4C 8B 6D 87                   MOV      r13, qword ptr [rbp - 0x79]
01406BF523: 4C 8B 7C 24 38                MOV      r15, qword ptr [rsp + 0x38]
01406BF528: 4D 85 ED                      TEST     r13, r13
01406BF52B: 74 39                         JE       0x1406bf566
01406BF52D: 49 8B 04 F7                   MOV      rax, qword ptr [r15 + rsi*8]
01406BF531: 49 89 45 00                   MOV      qword ptr [r13], rax
01406BF535: EB 2F                         JMP      0x1406bf566
01406BF537: 4C 3B 6D AF                   CMP      r13, qword ptr [rbp - 0x51]
01406BF53B: 75 20                         JNE      0x1406bf55d
01406BF53D: BA 01 00 00 00                MOV      edx, 1
01406BF542: 48 8D 4C 24 38                LEA      rcx, [rsp + 0x38]
01406BF547: E8 B4 21 9F FF                CALL     0x1400b1700
01406BF54C: 48 8B 45 8F                   MOV      rax, qword ptr [rbp - 0x71]
01406BF550: 48 89 45 AF                   MOV      qword ptr [rbp - 0x51], rax
01406BF554: 4C 8B 6D 87                   MOV      r13, qword ptr [rbp - 0x79]
01406BF558: 4C 8B 7C 24 38                MOV      r15, qword ptr [rsp + 0x38]
01406BF55D: 4D 85 ED                      TEST     r13, r13
01406BF560: 74 04                         JE       0x1406bf566
01406BF562: 4D 89 65 00                   MOV      qword ptr [r13], r12
01406BF566: 49 83 C5 08                   ADD      r13, 8
01406BF56A: 4C 89 6D 87                   MOV      qword ptr [rbp - 0x79], r13
01406BF56E: 41 FF C6                      INC      r14d
01406BF571: 48 8B 05 00 7A 32 00          MOV      rax, qword ptr [rip + 0x327a00]
01406BF578: 48 8B 35 F1 79 32 00          MOV      rsi, qword ptr [rip + 0x3279f1]
01406BF57F: 48 2B C6                      SUB      rax, rsi
01406BF582: 48 C1 F8 03                   SAR      rax, 3
01406BF586: 44 3B F0                      CMP      r14d, eax
01406BF589: 0F 82 61 FE FF FF             JB       0x1406bf3f0
01406BF58F: 4C 8B 7D B7                   MOV      r15, qword ptr [rbp - 0x49]
01406BF593: 4D 8D B7 B8 03 00 00          LEA      r14, [r15 + 0x3b8]
01406BF59A: 0F 57 FF                      XORPS    xmm7, xmm7
01406BF59D: 33 C9                         XOR      ecx, ecx
01406BF59F: 49 8B 56 08                   MOV      rdx, qword ptr [r14 + 8]
01406BF5A3: 4D 8B 06                      MOV      r8, qword ptr [r14]
01406BF5A6: 48 8B C2                      MOV      rax, rdx
01406BF5A9: 49 2B C0                      SUB      rax, r8
01406BF5AC: 48 C1 F8 04                   SAR      rax, 4
01406BF5B0: 48 85 C0                      TEST     rax, rax
01406BF5B3: 74 1E                         JE       0x1406bf5d3
01406BF5B5: 33 C0                         XOR      eax, eax
01406BF5B7: 49 2B D0                      SUB      rdx, r8
01406BF5BA: 48 C1 FA 04                   SAR      rdx, 4
01406BF5BE: 66 90                         NOP
01406BF5C0: 48 03 C0                      ADD      rax, rax
01406BF5C3: F3 41 0F 58 7C C0 08          ADDSS    xmm7, dword ptr [r8 + rax*8 + 8]
01406BF5CA: FF C1                         INC      ecx
01406BF5CC: 8B C1                         MOV      eax, ecx
01406BF5CE: 48 3B C2                      CMP      rax, rdx
01406BF5D1: 72 ED                         JB       0x1406bf5c0
01406BF5D3: 0F 57 D2                      XORPS    xmm2, xmm2
01406BF5D6: 49 8B D6                      MOV      rdx, r14
01406BF5D9: 48 8D 0D 30 59 31 00          LEA      rcx, [rip + 0x315930]
01406BF5E0: E8 8B D7 AD FF                CALL     0x14019cd70
01406BF5E5: 4C 8B D0                      MOV      r10, rax
01406BF5E8: 48 85 C0                      TEST     rax, rax
01406BF5EB: 4C 0F 44 15 D5 1C 32 00       CMOVE    r10, qword ptr [rip + 0x321cd5]
01406BF5F3: 4C 89 55 B7                   MOV      qword ptr [rbp - 0x49], r10
01406BF5F7: 45 33 E4                      XOR      r12d, r12d
01406BF5FA: 48 8B 4C 24 38                MOV      rcx, qword ptr [rsp + 0x38]
01406BF5FF: 4C 2B E9                      SUB      r13, rcx
01406BF602: 49 C1 FD 03                   SAR      r13, 3
01406BF606: 45 85 ED                      TEST     r13d, r13d
01406BF609: 0F 84 A3 09 00 00             JE       0x1406bffb2
01406BF60F: 90                            NOP
01406BF610: 4E 8B 04 E1                   MOV      r8, qword ptr [rcx + r12*8]
01406BF614: 33 C9                         XOR      ecx, ecx
01406BF616: 49 8B 46 08                   MOV      rax, qword ptr [r14 + 8]
01406BF61A: 49 2B 06                      SUB      rax, qword ptr [r14]
01406BF61D: 48 C1 F8 04                   SAR      rax, 4
01406BF621: 85 C0                         TEST     eax, eax
01406BF623: 0F 84 04 02 00 00             JE       0x1406bf82d
01406BF629: 4D 8B 0E                      MOV      r9, qword ptr [r14]
01406BF62C: 0F 1F 40 00                   NOP      dword ptr [rax]
01406BF630: 8B D1                         MOV      edx, ecx
01406BF632: 48 03 D2                      ADD      rdx, rdx
01406BF635: 49 8B 04 D1                   MOV      rax, qword ptr [r9 + rdx*8]
01406BF639: 49 3B C0                      CMP      rax, r8
01406BF63C: 74 28                         JE       0x1406bf666
01406BF63E: 4C 3B 05 83 1C 32 00          CMP      r8, qword ptr [rip + 0x321c83]
01406BF645: 75 09                         JNE      0x1406bf650
01406BF647: 48 3B 05 CA 1C 32 00          CMP      rax, qword ptr [rip + 0x321cca]
01406BF64E: 74 16                         JE       0x1406bf666
01406BF650: FF C1                         INC      ecx
01406BF652: 49 8B 46 08                   MOV      rax, qword ptr [r14 + 8]
01406BF656: 49 2B C1                      SUB      rax, r9
01406BF659: 48 C1 F8 04                   SAR      rax, 4
01406BF65D: 3B C8                         CMP      ecx, eax
01406BF65F: 72 CF                         JB       0x1406bf630
01406BF661: E9 C7 01 00 00                JMP      0x1406bf82d
01406BF666: 4C 89 44 24 28                MOV      qword ptr [rsp + 0x28], r8
01406BF66B: 0F 28 D7                      MOVAPS   xmm2, xmm7
01406BF66E: F3 0F 11 7C 24 30             MOVSS    dword ptr [rsp + 0x30], xmm7
01406BF674: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BF67B: F3 0F 10 80 04 86 00 00       MOVSS    xmm0, dword ptr [rax + 0x8604]
01406BF683: 48 63 88 00 86 00 00          MOVSXD   rcx, dword ptr [rax + 0x8600]
01406BF68A: 48 C1 E1 05                   SHL      rcx, 5
01406BF68E: 49 8B 04 D1                   MOV      rax, qword ptr [r9 + rdx*8]
01406BF692: F3 0F 10 B4 01 CC 00 00 00    MOVSS    xmm6, dword ptr [rcx + rax + 0xcc]
01406BF69B: 0F 28 C8                      MOVAPS   xmm1, xmm0
01406BF69E: F3 41 0F 59 4C D1 0C          MULSS    xmm1, dword ptr [r9 + rdx*8 + 0xc]
01406BF6A5: F3 0F 59 CE                   MULSS    xmm1, xmm6
01406BF6A9: F3 0F 11 4C 24 34             MOVSS    dword ptr [rsp + 0x34], xmm1
01406BF6AF: F3 0F 59 F0                   MULSS    xmm6, xmm0
01406BF6B3: 4D 3B C2                      CMP      r8, r10
01406BF6B6: 74 09                         JE       0x1406bf6c1
01406BF6B8: 0F 57 D2                      XORPS    xmm2, xmm2
01406BF6BB: F3 0F 11 54 24 30             MOVSS    dword ptr [rsp + 0x30], xmm2
01406BF6C1: 33 D2                         XOR      edx, edx
01406BF6C3: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF6C7: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BF6CA: 48 C1 F8 04                   SAR      rax, 4
01406BF6CE: 48 85 C0                      TEST     rax, rax
01406BF6D1: 74 68                         JE       0x1406bf73b
01406BF6D3: 33 C9                         XOR      ecx, ecx
01406BF6D5: 4C 8B 0B                      MOV      r9, qword ptr [rbx]
01406BF6D8: 0F 1F 84 00 00 00 00 00       NOP      dword ptr [rax + rax]
01406BF6E0: 48 C1 E1 04                   SHL      rcx, 4
01406BF6E4: 49 03 C9                      ADD      rcx, r9
01406BF6E7: 4C 39 01                      CMP      qword ptr [rcx], r8
01406BF6EA: 74 16                         JE       0x1406bf702
01406BF6EC: FF C2                         INC      edx
01406BF6EE: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF6F2: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BF6F5: 48 C1 F8 04                   SAR      rax, 4
01406BF6F9: 8B CA                         MOV      ecx, edx
01406BF6FB: 48 3B C8                      CMP      rcx, rax
01406BF6FE: 72 E0                         JB       0x1406bf6e0
01406BF700: EB 39                         JMP      0x1406bf73b
01406BF702: 0F 28 C2                      MOVAPS   xmm0, xmm2
01406BF705: F3 0F 58 41 08                ADDSS    xmm0, dword ptr [rcx + 8]
01406BF70A: F3 0F 11 41 08                MOVSS    dword ptr [rcx + 8], xmm0
01406BF70F: F3 0F 58 49 0C                ADDSS    xmm1, dword ptr [rcx + 0xc]
01406BF714: F3 0F 11 49 0C                MOVSS    dword ptr [rcx + 0xc], xmm1
01406BF719: 8B C2                         MOV      eax, edx
01406BF71B: 48 C1 E0 04                   SHL      rax, 4
01406BF71F: 48 03 07                      ADD      rax, qword ptr [rdi]
01406BF722: F3 0F 58 50 08                ADDSS    xmm2, dword ptr [rax + 8]
01406BF727: F3 0F 11 50 08                MOVSS    dword ptr [rax + 8], xmm2
01406BF72C: F3 0F 58 70 0C                ADDSS    xmm6, dword ptr [rax + 0xc]
01406BF731: F3 0F 11 70 0C                MOVSS    dword ptr [rax + 0xc], xmm6
01406BF736: E9 F2 00 00 00                JMP      0x1406bf82d
01406BF73B: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF73F: 48 8D 4C 24 28                LEA      rcx, [rsp + 0x28]
01406BF744: 48 3B C8                      CMP      rcx, rax
01406BF747: 73 3D                         JAE      0x1406bf786
01406BF749: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BF74C: 48 8D 54 24 28                LEA      rdx, [rsp + 0x28]
01406BF751: 48 3B CA                      CMP      rcx, rdx
01406BF754: 77 30                         JA       0x1406bf786
01406BF756: 48 8D 74 24 28                LEA      rsi, [rsp + 0x28]
01406BF75B: 48 2B F1                      SUB      rsi, rcx
01406BF75E: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BF762: 75 0D                         JNE      0x1406bf771
01406BF764: BA 01 00 00 00                MOV      edx, 1
01406BF769: 48 8B CB                      MOV      rcx, rbx
01406BF76C: E8 AF 24 9F FF                CALL     0x1400b1c20
01406BF771: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF775: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BF779: 48 03 33                      ADD      rsi, qword ptr [rbx]
01406BF77C: 48 85 C0                      TEST     rax, rax
01406BF77F: 74 29                         JE       0x1406bf7aa
01406BF781: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BF784: EB 21                         JMP      0x1406bf7a7
01406BF786: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BF78A: 75 0D                         JNE      0x1406bf799
01406BF78C: BA 01 00 00 00                MOV      edx, 1
01406BF791: 48 8B CB                      MOV      rcx, rbx
01406BF794: E8 87 24 9F FF                CALL     0x1400b1c20
01406BF799: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF79D: 48 85 C0                      TEST     rax, rax
01406BF7A0: 74 08                         JE       0x1406bf7aa
01406BF7A2: 0F 10 44 24 28                MOVUPS   xmm0, xmmword ptr [rsp + 0x28]
01406BF7A7: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BF7AA: 48 83 43 08 10                ADD      qword ptr [rbx + 8], 0x10
01406BF7AF: F3 0F 11 74 24 34             MOVSS    dword ptr [rsp + 0x34], xmm6
01406BF7B5: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF7B9: 48 8D 4C 24 28                LEA      rcx, [rsp + 0x28]
01406BF7BE: 48 3B C8                      CMP      rcx, rax
01406BF7C1: 73 3D                         JAE      0x1406bf800
01406BF7C3: 48 8B 0F                      MOV      rcx, qword ptr [rdi]
01406BF7C6: 48 8D 54 24 28                LEA      rdx, [rsp + 0x28]
01406BF7CB: 48 3B CA                      CMP      rcx, rdx
01406BF7CE: 77 30                         JA       0x1406bf800
01406BF7D0: 48 8D 74 24 28                LEA      rsi, [rsp + 0x28]
01406BF7D5: 48 2B F1                      SUB      rsi, rcx
01406BF7D8: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF7DC: 75 0D                         JNE      0x1406bf7eb
01406BF7DE: BA 01 00 00 00                MOV      edx, 1
01406BF7E3: 48 8B CF                      MOV      rcx, rdi
01406BF7E6: E8 35 24 9F FF                CALL     0x1400b1c20
01406BF7EB: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF7EF: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BF7F3: 48 03 37                      ADD      rsi, qword ptr [rdi]
01406BF7F6: 48 85 C0                      TEST     rax, rax
01406BF7F9: 74 29                         JE       0x1406bf824
01406BF7FB: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BF7FE: EB 21                         JMP      0x1406bf821
01406BF800: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF804: 75 0D                         JNE      0x1406bf813
01406BF806: BA 01 00 00 00                MOV      edx, 1
01406BF80B: 48 8B CF                      MOV      rcx, rdi
01406BF80E: E8 0D 24 9F FF                CALL     0x1400b1c20
01406BF813: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF817: 48 85 C0                      TEST     rax, rax
01406BF81A: 74 08                         JE       0x1406bf824
01406BF81C: 0F 10 44 24 28                MOVUPS   xmm0, xmmword ptr [rsp + 0x28]
01406BF821: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BF824: 4C 8B 55 B7                   MOV      r10, qword ptr [rbp - 0x49]
01406BF828: 48 83 47 08 10                ADD      qword ptr [rdi + 8], 0x10
01406BF82D: 41 FF C4                      INC      r12d
01406BF830: 45 3B E5                      CMP      r12d, r13d
01406BF833: 48 8B 4C 24 38                MOV      rcx, qword ptr [rsp + 0x38]
01406BF838: 0F 82 D2 FD FF FF             JB       0x1406bf610
01406BF83E: 48 8B 35 2B 77 32 00          MOV      rsi, qword ptr [rip + 0x32772b]
01406BF845: E9 68 07 00 00                JMP      0x1406bffb2
01406BF84A: 45 33 ED                      XOR      r13d, r13d
01406BF84D: 49 8B 87 C0 03 00 00          MOV      rax, qword ptr [r15 + 0x3c0]
01406BF854: 49 2B 87 B8 03 00 00          SUB      rax, qword ptr [r15 + 0x3b8]
01406BF85B: 48 C1 F8 04                   SAR      rax, 4
01406BF85F: 48 85 C0                      TEST     rax, rax
01406BF862: 0F 84 59 07 00 00             JE       0x1406bffc1
01406BF868: 45 33 F6                      XOR      r14d, r14d
01406BF86B: 0F 1F 44 00 00                NOP      dword ptr [rax + rax]
01406BF870: 41 3B CD                      CMP      ecx, r13d
01406BF873: 0F 84 07 07 00 00             JE       0x1406bff80
01406BF879: 45 84 C9                      TEST     r9b, r9b
01406BF87C: 0F 84 7C 04 00 00             JE       0x1406bfcfe
01406BF882: 4D 8B E6                      MOV      r12, r14
01406BF885: 49 C1 E4 04                   SHL      r12, 4
01406BF889: 4D 8B 87 B8 03 00 00          MOV      r8, qword ptr [r15 + 0x3b8]
01406BF890: 4B 8D 34 04                   LEA      rsi, [r12 + r8]
01406BF894: 4C 8B 16                      MOV      r10, qword ptr [rsi]
01406BF897: 4C 3B 55 C7                   CMP      r10, qword ptr [rbp - 0x39]
01406BF89B: 74 0A                         JE       0x1406bf8a7
01406BF89D: 4C 3B 55 CF                   CMP      r10, qword ptr [rbp - 0x31]
01406BF8A1: 0F 85 57 04 00 00             JNE      0x1406bfcfe
01406BF8A7: 33 D2                         XOR      edx, edx
01406BF8A9: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF8AD: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BF8B0: 48 C1 F8 04                   SAR      rax, 4
01406BF8B4: 48 85 C0                      TEST     rax, rax
01406BF8B7: 0F 84 D7 00 00 00             JE       0x1406bf994
01406BF8BD: 33 C9                         XOR      ecx, ecx
01406BF8BF: 4C 8B 0B                      MOV      r9, qword ptr [rbx]
01406BF8C2: 48 03 C9                      ADD      rcx, rcx
01406BF8C5: 4D 39 1C C9                   CMP      qword ptr [r9 + rcx*8], r11
01406BF8C9: 74 19                         JE       0x1406bf8e4
01406BF8CB: FF C2                         INC      edx
01406BF8CD: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BF8D1: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BF8D4: 48 C1 F8 04                   SAR      rax, 4
01406BF8D8: 8B CA                         MOV      ecx, edx
01406BF8DA: 48 3B C8                      CMP      rcx, rax
01406BF8DD: 72 E3                         JB       0x1406bf8c2
01406BF8DF: E9 B0 00 00 00                JMP      0x1406bf994
01406BF8E4: 44 8B C2                      MOV      r8d, edx
01406BF8E7: 4D 03 C0                      ADD      r8, r8
01406BF8EA: 48 8B 17                      MOV      rdx, qword ptr [rdi]
01406BF8ED: F3 42 0F 10 44 C2 08          MOVSS    xmm0, dword ptr [rdx + r8*8 + 8]
01406BF8F4: F3 0F 58 46 08                ADDSS    xmm0, dword ptr [rsi + 8]
01406BF8F9: F3 42 0F 11 44 C2 08          MOVSS    dword ptr [rdx + r8*8 + 8], xmm0
01406BF900: 4C 8B 16                      MOV      r10, qword ptr [rsi]
01406BF903: 4D 3B D3                      CMP      r10, r11
01406BF906: 75 32                         JNE      0x1406bf93a
01406BF908: 49 8B 8F 08 17 00 00          MOV      rcx, qword ptr [r15 + 0x1708]
01406BF90F: 48 63 81 00 86 00 00          MOVSXD   rax, dword ptr [rcx + 0x8600]
01406BF916: 48 C1 E0 05                   SHL      rax, 5
01406BF91A: F3 42 0F 10 84 10 CC 00 00 00 MOVSS    xmm0, dword ptr [rax + r10 + 0xcc]
01406BF924: F3 0F 59 81 04 86 00 00       MULSS    xmm0, dword ptr [rcx + 0x8604]
01406BF92C: F3 42 0F 58 44 C2 0C          ADDSS    xmm0, dword ptr [rdx + r8*8 + 0xc]
01406BF933: F3 42 0F 11 44 C2 0C          MOVSS    dword ptr [rdx + r8*8 + 0xc], xmm0
01406BF93A: F3 43 0F 10 44 C1 08          MOVSS    xmm0, dword ptr [r9 + r8*8 + 8]
01406BF941: F3 0F 58 46 08                ADDSS    xmm0, dword ptr [rsi + 8]
01406BF946: F3 43 0F 11 44 C1 08          MOVSS    dword ptr [r9 + r8*8 + 8], xmm0
01406BF94D: 48 8B 0E                      MOV      rcx, qword ptr [rsi]
01406BF950: 49 3B CB                      CMP      rcx, r11
01406BF953: 0F 85 27 06 00 00             JNE      0x1406bff80
01406BF959: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BF960: F3 0F 10 80 04 86 00 00       MOVSS    xmm0, dword ptr [rax + 0x8604]
01406BF968: F3 0F 59 46 0C                MULSS    xmm0, dword ptr [rsi + 0xc]
01406BF96D: 48 63 80 00 86 00 00          MOVSXD   rax, dword ptr [rax + 0x8600]
01406BF974: 48 C1 E0 05                   SHL      rax, 5
01406BF978: F3 0F 59 84 08 CC 00 00 00    MULSS    xmm0, dword ptr [rax + rcx + 0xcc]
01406BF981: F3 43 0F 58 44 C1 0C          ADDSS    xmm0, dword ptr [r9 + r8*8 + 0xc]
01406BF988: F3 43 0F 11 44 C1 0C          MOVSS    dword ptr [r9 + r8*8 + 0xc], xmm0
01406BF98F: E9 EC 05 00 00                JMP      0x1406bff80
01406BF994: 4D 3B D3                      CMP      r10, r11
01406BF997: 0F 85 59 01 00 00             JNE      0x1406bfaf6
01406BF99D: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF9A1: 48 3B F0                      CMP      rsi, rax
01406BF9A4: 73 27                         JAE      0x1406bf9cd
01406BF9A6: 48 8B 0F                      MOV      rcx, qword ptr [rdi]
01406BF9A9: 48 3B CE                      CMP      rcx, rsi
01406BF9AC: 77 1F                         JA       0x1406bf9cd
01406BF9AE: 48 2B F1                      SUB      rsi, rcx
01406BF9B1: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF9B5: 75 0D                         JNE      0x1406bf9c4
01406BF9B7: BA 01 00 00 00                MOV      edx, 1
01406BF9BC: 48 8B CF                      MOV      rcx, rdi
01406BF9BF: E8 5C 22 9F FF                CALL     0x1400b1c20
01406BF9C4: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BF9C8: 48 03 37                      ADD      rsi, qword ptr [rdi]
01406BF9CB: EB 13                         JMP      0x1406bf9e0
01406BF9CD: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BF9D1: 75 0D                         JNE      0x1406bf9e0
01406BF9D3: BA 01 00 00 00                MOV      edx, 1
01406BF9D8: 48 8B CF                      MOV      rcx, rdi
01406BF9DB: E8 40 22 9F FF                CALL     0x1400b1c20
01406BF9E0: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BF9E4: 48 85 C0                      TEST     rax, rax
01406BF9E7: 74 06                         JE       0x1406bf9ef
01406BF9E9: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BF9EC: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BF9EF: 48 83 47 08 10                ADD      qword ptr [rdi + 8], 0x10
01406BF9F4: 4C 8B 55 67                   MOV      r10, qword ptr [rbp + 0x67]
01406BF9F8: 49 8B 82 78 04 00 00          MOV      rax, qword ptr [r10 + 0x478]
01406BF9FF: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFA06: 41 8B 54 0C 08                MOV      edx, dword ptr [r12 + rcx + 8]
01406BFA0B: 89 50 F8                      MOV      dword ptr [rax - 8], edx
01406BFA0E: 49 8B 82 78 04 00 00          MOV      rax, qword ptr [r10 + 0x478]
01406BFA15: 4D 8B 8F 08 17 00 00          MOV      r9, qword ptr [r15 + 0x1708]
01406BFA1C: 4D 63 81 00 86 00 00          MOVSXD   r8, dword ptr [r9 + 0x8600]
01406BFA23: 49 C1 E0 05                   SHL      r8, 5
01406BFA27: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFA2E: 49 8B 14 0C                   MOV      rdx, qword ptr [r12 + rcx]
01406BFA32: F3 41 0F 10 84 10 CC 00 00 00 MOVSS    xmm0, dword ptr [r8 + rdx + 0xcc]
01406BFA3C: F3 41 0F 59 81 04 86 00 00    MULSS    xmm0, dword ptr [r9 + 0x8604]
01406BFA45: F3 0F 11 40 FC                MOVSS    dword ptr [rax - 4], xmm0
01406BFA4A: 49 8B B7 B8 03 00 00          MOV      rsi, qword ptr [r15 + 0x3b8]
01406BFA51: 49 03 F4                      ADD      rsi, r12
01406BFA54: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFA58: 48 3B F0                      CMP      rsi, rax
01406BFA5B: 73 27                         JAE      0x1406bfa84
01406BFA5D: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFA60: 48 3B CE                      CMP      rcx, rsi
01406BFA63: 77 1F                         JA       0x1406bfa84
01406BFA65: 48 2B F1                      SUB      rsi, rcx
01406BFA68: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFA6C: 75 0D                         JNE      0x1406bfa7b
01406BFA6E: BA 01 00 00 00                MOV      edx, 1
01406BFA73: 48 8B CB                      MOV      rcx, rbx
01406BFA76: E8 A5 21 9F FF                CALL     0x1400b1c20
01406BFA7B: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BFA7F: 48 03 33                      ADD      rsi, qword ptr [rbx]
01406BFA82: EB 13                         JMP      0x1406bfa97
01406BFA84: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFA88: 75 0D                         JNE      0x1406bfa97
01406BFA8A: BA 01 00 00 00                MOV      edx, 1
01406BFA8F: 48 8B CB                      MOV      rcx, rbx
01406BFA92: E8 89 21 9F FF                CALL     0x1400b1c20
01406BFA97: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFA9B: 48 85 C0                      TEST     rax, rax
01406BFA9E: 74 06                         JE       0x1406bfaa6
01406BFAA0: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BFAA3: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BFAA6: 48 83 43 08 10                ADD      qword ptr [rbx + 8], 0x10
01406BFAAB: 48 8B 53 08                   MOV      rdx, qword ptr [rbx + 8]
01406BFAAF: 48 2B 13                      SUB      rdx, qword ptr [rbx]
01406BFAB2: 49 8B 87 B8 03 00 00          MOV      rax, qword ptr [r15 + 0x3b8]
01406BFAB9: 48 83 E2 F0                   AND      rdx, 0xfffffffffffffff0
01406BFABD: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFAC0: 41 8B 44 04 08                MOV      eax, dword ptr [r12 + rax + 8]
01406BFAC5: 89 44 0A F8                   MOV      dword ptr [rdx + rcx - 8], eax
01406BFAC9: 49 8B 97 B8 03 00 00          MOV      rdx, qword ptr [r15 + 0x3b8]
01406BFAD0: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BFAD7: F3 41 0F 10 44 14 0C          MOVSS    xmm0, dword ptr [r12 + rdx + 0xc]
01406BFADE: F3 0F 59 80 04 86 00 00       MULSS    xmm0, dword ptr [rax + 0x8604]
01406BFAE6: 48 63 88 00 86 00 00          MOVSXD   rcx, dword ptr [rax + 0x8600]
01406BFAED: 49 8B 04 14                   MOV      rax, qword ptr [r12 + rdx]
01406BFAF1: E9 68 04 00 00                JMP      0x1406bff5e
01406BFAF6: 33 D2                         XOR      edx, edx
01406BFAF8: 49 8B 87 C0 03 00 00          MOV      rax, qword ptr [r15 + 0x3c0]
01406BFAFF: 49 2B C0                      SUB      rax, r8
01406BFB02: 48 C1 F8 04                   SAR      rax, 4
01406BFB06: 48 85 C0                      TEST     rax, rax
01406BFB09: 0F 84 71 04 00 00             JE       0x1406bff80
01406BFB0F: 33 C9                         XOR      ecx, ecx
01406BFB11: 48 03 C9                      ADD      rcx, rcx
01406BFB14: 4D 39 1C C8                   CMP      qword ptr [r8 + rcx*8], r11
01406BFB18: 74 1C                         JE       0x1406bfb36
01406BFB1A: FF C2                         INC      edx
01406BFB1C: 49 8B 87 C0 03 00 00          MOV      rax, qword ptr [r15 + 0x3c0]
01406BFB23: 49 2B C0                      SUB      rax, r8
01406BFB26: 48 C1 F8 04                   SAR      rax, 4
01406BFB2A: 8B CA                         MOV      ecx, edx
01406BFB2C: 48 3B C8                      CMP      rcx, rax
01406BFB2F: 72 E0                         JB       0x1406bfb11
01406BFB31: E9 4A 04 00 00                JMP      0x1406bff80
01406BFB36: 89 55 97                      MOV      dword ptr [rbp - 0x69], edx
01406BFB39: 44 8B F2                      MOV      r14d, edx
01406BFB3C: 49 C1 E6 04                   SHL      r14, 4
01406BFB40: 4B 8D 34 30                   LEA      rsi, [r8 + r14]
01406BFB44: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BFB48: 48 3B F0                      CMP      rsi, rax
01406BFB4B: 73 27                         JAE      0x1406bfb74
01406BFB4D: 48 8B 0F                      MOV      rcx, qword ptr [rdi]
01406BFB50: 48 3B CE                      CMP      rcx, rsi
01406BFB53: 77 1F                         JA       0x1406bfb74
01406BFB55: 48 2B F1                      SUB      rsi, rcx
01406BFB58: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BFB5C: 75 0D                         JNE      0x1406bfb6b
01406BFB5E: BA 01 00 00 00                MOV      edx, 1
01406BFB63: 48 8B CF                      MOV      rcx, rdi
01406BFB66: E8 B5 20 9F FF                CALL     0x1400b1c20
01406BFB6B: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BFB6F: 48 03 37                      ADD      rsi, qword ptr [rdi]
01406BFB72: EB 13                         JMP      0x1406bfb87
01406BFB74: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BFB78: 75 0D                         JNE      0x1406bfb87
01406BFB7A: BA 01 00 00 00                MOV      edx, 1
01406BFB7F: 48 8B CF                      MOV      rcx, rdi
01406BFB82: E8 99 20 9F FF                CALL     0x1400b1c20
01406BFB87: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BFB8B: 48 85 C0                      TEST     rax, rax
01406BFB8E: 74 06                         JE       0x1406bfb96
01406BFB90: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BFB93: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BFB96: 48 83 47 08 10                ADD      qword ptr [rdi + 8], 0x10
01406BFB9B: 4C 8B 55 67                   MOV      r10, qword ptr [rbp + 0x67]
01406BFB9F: 49 8B 82 78 04 00 00          MOV      rax, qword ptr [r10 + 0x478]
01406BFBA6: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFBAD: 41 8B 54 0E 08                MOV      edx, dword ptr [r14 + rcx + 8]
01406BFBB2: 89 50 F8                      MOV      dword ptr [rax - 8], edx
01406BFBB5: 49 8B 82 78 04 00 00          MOV      rax, qword ptr [r10 + 0x478]
01406BFBBC: 4D 8B 8F 08 17 00 00          MOV      r9, qword ptr [r15 + 0x1708]
01406BFBC3: 4D 63 81 00 86 00 00          MOVSXD   r8, dword ptr [r9 + 0x8600]
01406BFBCA: 49 C1 E0 05                   SHL      r8, 5
01406BFBCE: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFBD5: 49 8B 14 0E                   MOV      rdx, qword ptr [r14 + rcx]
01406BFBD9: F3 41 0F 10 84 10 CC 00 00 00 MOVSS    xmm0, dword ptr [r8 + rdx + 0xcc]
01406BFBE3: F3 41 0F 59 81 04 86 00 00    MULSS    xmm0, dword ptr [r9 + 0x8604]
01406BFBEC: F3 0F 11 40 FC                MOVSS    dword ptr [rax - 4], xmm0
01406BFBF1: 49 8B B7 B8 03 00 00          MOV      rsi, qword ptr [r15 + 0x3b8]
01406BFBF8: 49 03 F6                      ADD      rsi, r14
01406BFBFB: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFBFF: 48 3B F0                      CMP      rsi, rax
01406BFC02: 73 2B                         JAE      0x1406bfc2f
01406BFC04: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFC07: 48 3B CE                      CMP      rcx, rsi
01406BFC0A: 77 23                         JA       0x1406bfc2f
01406BFC0C: 48 2B F1                      SUB      rsi, rcx
01406BFC0F: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFC13: 75 11                         JNE      0x1406bfc26
01406BFC15: BA 01 00 00 00                MOV      edx, 1
01406BFC1A: 48 8B CB                      MOV      rcx, rbx
01406BFC1D: E8 FE 1F 9F FF                CALL     0x1400b1c20
01406BFC22: 4C 8B 55 67                   MOV      r10, qword ptr [rbp + 0x67]
01406BFC26: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BFC2A: 48 03 33                      ADD      rsi, qword ptr [rbx]
01406BFC2D: EB 17                         JMP      0x1406bfc46
01406BFC2F: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFC33: 75 11                         JNE      0x1406bfc46
01406BFC35: BA 01 00 00 00                MOV      edx, 1
01406BFC3A: 48 8B CB                      MOV      rcx, rbx
01406BFC3D: E8 DE 1F 9F FF                CALL     0x1400b1c20
01406BFC42: 4C 8B 55 67                   MOV      r10, qword ptr [rbp + 0x67]
01406BFC46: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFC4A: 48 85 C0                      TEST     rax, rax
01406BFC4D: 74 06                         JE       0x1406bfc55
01406BFC4F: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BFC52: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BFC55: 48 83 43 08 10                ADD      qword ptr [rbx + 8], 0x10
01406BFC5A: 48 8B 53 08                   MOV      rdx, qword ptr [rbx + 8]
01406BFC5E: 48 2B 13                      SUB      rdx, qword ptr [rbx]
01406BFC61: 49 8B 87 B8 03 00 00          MOV      rax, qword ptr [r15 + 0x3b8]
01406BFC68: 48 83 E2 F0                   AND      rdx, 0xfffffffffffffff0
01406BFC6C: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFC6F: 41 8B 44 06 08                MOV      eax, dword ptr [r14 + rax + 8]
01406BFC74: 89 44 0A F8                   MOV      dword ptr [rdx + rcx - 8], eax
01406BFC78: 4C 8B 43 08                   MOV      r8, qword ptr [rbx + 8]
01406BFC7C: 4C 2B 03                      SUB      r8, qword ptr [rbx]
01406BFC7F: 49 8B 97 B8 03 00 00          MOV      rdx, qword ptr [r15 + 0x3b8]
01406BFC86: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BFC8D: F3 0F 10 80 04 86 00 00       MOVSS    xmm0, dword ptr [rax + 0x8604]
01406BFC95: F3 41 0F 59 44 16 0C          MULSS    xmm0, dword ptr [r14 + rdx + 0xc]
01406BFC9C: 48 63 88 00 86 00 00          MOVSXD   rcx, dword ptr [rax + 0x8600]
01406BFCA3: 48 C1 E1 05                   SHL      rcx, 5
01406BFCA7: 49 8B 04 16                   MOV      rax, qword ptr [r14 + rdx]
01406BFCAB: F3 0F 59 84 01 CC 00 00 00    MULSS    xmm0, dword ptr [rcx + rax + 0xcc]
01406BFCB4: 49 83 E0 F0                   AND      r8, 0xfffffffffffffff0
01406BFCB8: 48 8B 03                      MOV      rax, qword ptr [rbx]
01406BFCBB: F3 41 0F 11 44 00 FC          MOVSS    dword ptr [r8 + rax - 4], xmm0
01406BFCC2: 49 8B 82 78 04 00 00          MOV      rax, qword ptr [r10 + 0x478]
01406BFCC9: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFCD0: F3 0F 10 40 F8                MOVSS    xmm0, dword ptr [rax - 8]
01406BFCD5: F3 41 0F 58 44 0C 08          ADDSS    xmm0, dword ptr [r12 + rcx + 8]
01406BFCDC: F3 0F 11 40 F8                MOVSS    dword ptr [rax - 8], xmm0
01406BFCE1: 49 8B 82 60 04 00 00          MOV      rax, qword ptr [r10 + 0x460]
01406BFCE8: F3 41 0F 10 44 0C 08          MOVSS    xmm0, dword ptr [r12 + rcx + 8]
01406BFCEF: F3 0F 58 40 F8                ADDSS    xmm0, dword ptr [rax - 8]
01406BFCF4: F3 0F 11 40 F8                MOVSS    dword ptr [rax - 8], xmm0
01406BFCF9: E9 82 02 00 00                JMP      0x1406bff80
01406BFCFE: 33 D2                         XOR      edx, edx
01406BFD00: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFD04: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BFD07: 48 C1 F8 04                   SAR      rax, 4
01406BFD0B: 48 85 C0                      TEST     rax, rax
01406BFD0E: 0F 84 E8 00 00 00             JE       0x1406bfdfc
01406BFD14: 33 C9                         XOR      ecx, ecx
01406BFD16: 4C 8B 13                      MOV      r10, qword ptr [rbx]
01406BFD19: 4D 8B DE                      MOV      r11, r14
01406BFD1C: 4D 03 DB                      ADD      r11, r11
01406BFD1F: 49 8B B7 B8 03 00 00          MOV      rsi, qword ptr [r15 + 0x3b8]
01406BFD26: 4E 8B 04 DE                   MOV      r8, qword ptr [rsi + r11*8]
01406BFD2A: 66 0F 1F 44 00 00             NOP      word ptr [rax + rax]
01406BFD30: 48 03 C9                      ADD      rcx, rcx
01406BFD33: 4D 39 04 CA                   CMP      qword ptr [r10 + rcx*8], r8
01406BFD37: 74 19                         JE       0x1406bfd52
01406BFD39: FF C2                         INC      edx
01406BFD3B: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFD3F: 48 2B 03                      SUB      rax, qword ptr [rbx]
01406BFD42: 48 C1 F8 04                   SAR      rax, 4
01406BFD46: 8B CA                         MOV      ecx, edx
01406BFD48: 48 3B C8                      CMP      rcx, rax
01406BFD4B: 72 E3                         JB       0x1406bfd30
01406BFD4D: E9 AA 00 00 00                JMP      0x1406bfdfc
01406BFD52: 44 8B CA                      MOV      r9d, edx
01406BFD55: 49 C1 E1 04                   SHL      r9, 4
01406BFD59: 4C 8B 07                      MOV      r8, qword ptr [rdi]
01406BFD5C: F3 43 0F 10 44 08 08          MOVSS    xmm0, dword ptr [r8 + r9 + 8]
01406BFD63: F3 42 0F 58 44 DE 08          ADDSS    xmm0, dword ptr [rsi + r11*8 + 8]
01406BFD6A: F3 43 0F 11 44 08 08          MOVSS    dword ptr [r8 + r9 + 8], xmm0
01406BFD71: 49 8B 97 08 17 00 00          MOV      rdx, qword ptr [r15 + 0x1708]
01406BFD78: 48 63 8A 00 86 00 00          MOVSXD   rcx, dword ptr [rdx + 0x8600]
01406BFD7F: 48 C1 E1 05                   SHL      rcx, 5
01406BFD83: 4A 8B 04 DE                   MOV      rax, qword ptr [rsi + r11*8]
01406BFD87: F3 0F 10 84 01 CC 00 00 00    MOVSS    xmm0, dword ptr [rcx + rax + 0xcc]
01406BFD90: F3 0F 59 82 04 86 00 00       MULSS    xmm0, dword ptr [rdx + 0x8604]
01406BFD98: F3 43 0F 58 44 08 0C          ADDSS    xmm0, dword ptr [r8 + r9 + 0xc]
01406BFD9F: F3 43 0F 11 44 08 0C          MOVSS    dword ptr [r8 + r9 + 0xc], xmm0
01406BFDA6: F3 43 0F 10 44 0A 08          MOVSS    xmm0, dword ptr [r10 + r9 + 8]
01406BFDAD: F3 42 0F 58 44 DE 08          ADDSS    xmm0, dword ptr [rsi + r11*8 + 8]
01406BFDB4: F3 43 0F 11 44 0A 08          MOVSS    dword ptr [r10 + r9 + 8], xmm0
01406BFDBB: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BFDC2: F3 42 0F 10 44 DE 0C          MOVSS    xmm0, dword ptr [rsi + r11*8 + 0xc]
01406BFDC9: F3 0F 59 80 04 86 00 00       MULSS    xmm0, dword ptr [rax + 0x8604]
01406BFDD1: 48 63 88 00 86 00 00          MOVSXD   rcx, dword ptr [rax + 0x8600]
01406BFDD8: 48 C1 E1 05                   SHL      rcx, 5
01406BFDDC: 4A 8B 04 DE                   MOV      rax, qword ptr [rsi + r11*8]
01406BFDE0: F3 0F 59 84 01 CC 00 00 00    MULSS    xmm0, dword ptr [rcx + rax + 0xcc]
01406BFDE9: F3 43 0F 58 44 0A 0C          ADDSS    xmm0, dword ptr [r10 + r9 + 0xc]
01406BFDF0: F3 43 0F 11 44 0A 0C          MOVSS    dword ptr [r10 + r9 + 0xc], xmm0
01406BFDF7: E9 84 01 00 00                JMP      0x1406bff80
01406BFDFC: 49 C1 E6 04                   SHL      r14, 4
01406BFE00: 49 8B B7 B8 03 00 00          MOV      rsi, qword ptr [r15 + 0x3b8]
01406BFE07: 49 03 F6                      ADD      rsi, r14
01406BFE0A: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BFE0E: 48 3B F0                      CMP      rsi, rax
01406BFE11: 73 27                         JAE      0x1406bfe3a
01406BFE13: 48 8B 0F                      MOV      rcx, qword ptr [rdi]
01406BFE16: 48 3B CE                      CMP      rcx, rsi
01406BFE19: 77 1F                         JA       0x1406bfe3a
01406BFE1B: 48 2B F1                      SUB      rsi, rcx
01406BFE1E: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BFE22: 75 0D                         JNE      0x1406bfe31
01406BFE24: BA 01 00 00 00                MOV      edx, 1
01406BFE29: 48 8B CF                      MOV      rcx, rdi
01406BFE2C: E8 EF 1D 9F FF                CALL     0x1400b1c20
01406BFE31: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BFE35: 48 03 37                      ADD      rsi, qword ptr [rdi]
01406BFE38: EB 13                         JMP      0x1406bfe4d
01406BFE3A: 48 3B 47 10                   CMP      rax, qword ptr [rdi + 0x10]
01406BFE3E: 75 0D                         JNE      0x1406bfe4d
01406BFE40: BA 01 00 00 00                MOV      edx, 1
01406BFE45: 48 8B CF                      MOV      rcx, rdi
01406BFE48: E8 D3 1D 9F FF                CALL     0x1400b1c20
01406BFE4D: 48 8B 47 08                   MOV      rax, qword ptr [rdi + 8]
01406BFE51: 48 85 C0                      TEST     rax, rax
01406BFE54: 74 06                         JE       0x1406bfe5c
01406BFE56: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BFE59: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BFE5C: 48 83 47 08 10                ADD      qword ptr [rdi + 8], 0x10
01406BFE61: 4C 8B 45 67                   MOV      r8, qword ptr [rbp + 0x67]
01406BFE65: 49 8B 80 78 04 00 00          MOV      rax, qword ptr [r8 + 0x478]
01406BFE6C: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFE73: 41 8B 54 0E 08                MOV      edx, dword ptr [r14 + rcx + 8]
01406BFE78: 89 50 F8                      MOV      dword ptr [rax - 8], edx
01406BFE7B: 49 8B 80 78 04 00 00          MOV      rax, qword ptr [r8 + 0x478]
01406BFE82: 4D 8B 8F 08 17 00 00          MOV      r9, qword ptr [r15 + 0x1708]
01406BFE89: 4D 63 81 00 86 00 00          MOVSXD   r8, dword ptr [r9 + 0x8600]
01406BFE90: 49 C1 E0 05                   SHL      r8, 5
01406BFE94: 49 8B 8F B8 03 00 00          MOV      rcx, qword ptr [r15 + 0x3b8]
01406BFE9B: 49 8B 14 0E                   MOV      rdx, qword ptr [r14 + rcx]
01406BFE9F: F3 41 0F 10 84 10 CC 00 00 00 MOVSS    xmm0, dword ptr [r8 + rdx + 0xcc]
01406BFEA9: F3 41 0F 59 81 04 86 00 00    MULSS    xmm0, dword ptr [r9 + 0x8604]
01406BFEB2: F3 0F 11 40 FC                MOVSS    dword ptr [rax - 4], xmm0
01406BFEB7: 49 8B B7 B8 03 00 00          MOV      rsi, qword ptr [r15 + 0x3b8]
01406BFEBE: 49 03 F6                      ADD      rsi, r14
01406BFEC1: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFEC5: 48 3B F0                      CMP      rsi, rax
01406BFEC8: 73 27                         JAE      0x1406bfef1
01406BFECA: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFECD: 48 3B CE                      CMP      rcx, rsi
01406BFED0: 77 1F                         JA       0x1406bfef1
01406BFED2: 48 2B F1                      SUB      rsi, rcx
01406BFED5: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFED9: 75 0D                         JNE      0x1406bfee8
01406BFEDB: BA 01 00 00 00                MOV      edx, 1
01406BFEE0: 48 8B CB                      MOV      rcx, rbx
01406BFEE3: E8 38 1D 9F FF                CALL     0x1400b1c20
01406BFEE8: 48 83 E6 F0                   AND      rsi, 0xfffffffffffffff0
01406BFEEC: 48 03 33                      ADD      rsi, qword ptr [rbx]
01406BFEEF: EB 13                         JMP      0x1406bff04
01406BFEF1: 48 3B 43 10                   CMP      rax, qword ptr [rbx + 0x10]
01406BFEF5: 75 0D                         JNE      0x1406bff04
01406BFEF7: BA 01 00 00 00                MOV      edx, 1
01406BFEFC: 48 8B CB                      MOV      rcx, rbx
01406BFEFF: E8 1C 1D 9F FF                CALL     0x1400b1c20
01406BFF04: 48 8B 43 08                   MOV      rax, qword ptr [rbx + 8]
01406BFF08: 48 85 C0                      TEST     rax, rax
01406BFF0B: 74 06                         JE       0x1406bff13
01406BFF0D: 0F 10 06                      MOVUPS   xmm0, xmmword ptr [rsi]
01406BFF10: 0F 11 00                      MOVUPS   xmmword ptr [rax], xmm0
01406BFF13: 48 83 43 08 10                ADD      qword ptr [rbx + 8], 0x10
01406BFF18: 48 8B 53 08                   MOV      rdx, qword ptr [rbx + 8]
01406BFF1C: 48 2B 13                      SUB      rdx, qword ptr [rbx]
01406BFF1F: 49 8B 87 B8 03 00 00          MOV      rax, qword ptr [r15 + 0x3b8]
01406BFF26: 48 83 E2 F0                   AND      rdx, 0xfffffffffffffff0
01406BFF2A: 48 8B 0B                      MOV      rcx, qword ptr [rbx]
01406BFF2D: 41 8B 44 06 08                MOV      eax, dword ptr [r14 + rax + 8]
01406BFF32: 89 44 0A F8                   MOV      dword ptr [rdx + rcx - 8], eax
01406BFF36: 49 8B 97 B8 03 00 00          MOV      rdx, qword ptr [r15 + 0x3b8]
01406BFF3D: 49 8B 87 08 17 00 00          MOV      rax, qword ptr [r15 + 0x1708]
01406BFF44: F3 41 0F 10 44 16 0C          MOVSS    xmm0, dword ptr [r14 + rdx + 0xc]
01406BFF4B: F3 0F 59 80 04 86 00 00       MULSS    xmm0, dword ptr [rax + 0x8604]
01406BFF53: 48 63 88 00 86 00 00          MOVSXD   rcx, dword ptr [rax + 0x8600]
01406BFF5A: 49 8B 04 16                   MOV      rax, qword ptr [r14 + rdx]
01406BFF5E: 48 C1 E1 05                   SHL      rcx, 5
01406BFF62: 4C 8B 43 08                   MOV      r8, qword ptr [rbx + 8]
01406BFF66: F3 0F 59 84 01 CC 00 00 00    MULSS    xmm0, dword ptr [rcx + rax + 0xcc]
01406BFF6F: 4C 2B 03                      SUB      r8, qword ptr [rbx]
01406BFF72: 48 8B 03                      MOV      rax, qword ptr [rbx]
01406BFF75: 49 83 E0 F0                   AND      r8, 0xfffffffffffffff0
01406BFF79: F3 41 0F 11 44 00 FC          MOVSS    dword ptr [r8 + rax - 4], xmm0
01406BFF80: 41 FF C5                      INC      r13d
01406BFF83: 49 8B 87 C0 03 00 00          MOV      rax, qword ptr [r15 + 0x3c0]
01406BFF8A: 49 2B 87 B8 03 00 00          SUB      rax, qword ptr [r15 + 0x3b8]
01406BFF91: 48 C1 F8 04                   SAR      rax, 4
01406BFF95: 45 8B F5                      MOV      r14d, r13d
01406BFF98: 4C 3B F0                      CMP      r14, rax
01406BFF9B: 8B 4D 97                      MOV      ecx, dword ptr [rbp - 0x69]
01406BFF9E: 4C 8B 5D A7                   MOV      r11, qword ptr [rbp - 0x59]
01406BFFA2: 44 0F B6 4D 7F                MOVZX    r9d, byte ptr [rbp + 0x7f]
01406BFFA7: 0F 82 C3 F8 FF FF             JB       0x1406bf870
01406BFFAD: E9 8C F8 FF FF                JMP      0x1406bf83e
01406BFFB2: 44 0F B6 64 24 20             MOVZX    r12d, byte ptr [rsp + 0x20]
01406BFFB8: 44 0F B6 4D 7F                MOVZX    r9d, byte ptr [rbp + 0x7f]
01406BFFBD: 48 8B 55 BF                   MOV      rdx, qword ptr [rbp - 0x41]
01406BFFC1: 44 0F B6 6D 6F                MOVZX    r13d, byte ptr [rbp + 0x6f]
01406BFFC6: 44 8B 45 9B                   MOV      r8d, dword ptr [rbp - 0x65]
01406BFFCA: 41 FF C0                      INC      r8d
01406BFFCD: 44 89 45 9B                   MOV      dword ptr [rbp - 0x65], r8d
01406BFFD1: 48 FF C2                      INC      rdx
01406BFFD4: 48 89 55 BF                   MOV      qword ptr [rbp - 0x41], rdx
01406BFFD8: 4C 8B 7D D7                   MOV      r15, qword ptr [rbp - 0x29]
01406BFFDC: 49 8B 87 90 04 00 00          MOV      rax, qword ptr [r15 + 0x490]
01406BFFE3: 49 2B 87 88 04 00 00          SUB      rax, qword ptr [r15 + 0x488]
01406BFFEA: 48 C1 F8 03                   SAR      rax, 3
01406BFFEE: 44 3B C0                      CMP      r8d, eax
01406BFFF1: 4C 8B 5D A7                   MOV      r11, qword ptr [rbp - 0x59]
01406BFFF5: 4C 8B 45 C7                   MOV      r8, qword ptr [rbp - 0x39]
01406BFFF9: 4C 8B 75 CF                   MOV      r14, qword ptr [rbp - 0x31]
01406BFFFD: 0F 8C 0D F3 FF FF             JL       0x1406bf310
01406C0003: 4C 8B 55 67                   MOV      r10, qword ptr [rbp + 0x67]
01406C0007: 48 8B 4D DF                   MOV      rcx, qword ptr [rbp - 0x21]
01406C000B: 8B 55 9F                      MOV      edx, dword ptr [rbp - 0x61]
01406C000E: FF C2                         INC      edx
01406C0010: 89 55 9F                      MOV      dword ptr [rbp - 0x61], edx
01406C0013: 48 FF C1                      INC      rcx
01406C0016: 48 89 4D DF                   MOV      qword ptr [rbp - 0x21], rcx
01406C001A: 49 8B 82 A8 03 00 00          MOV      rax, qword ptr [r10 + 0x3a8]
01406C0021: 49 2B 82 A0 03 00 00          SUB      rax, qword ptr [r10 + 0x3a0]
01406C0028: 48 C1 F8 03                   SAR      rax, 3
01406C002C: 3B D0                         CMP      edx, eax
01406C002E: 4C 8B 45 C7                   MOV      r8, qword ptr [rbp - 0x39]
01406C0032: 4C 8B 75 CF                   MOV      r14, qword ptr [rbp - 0x31]
01406C0036: 0F 8C 84 F2 FF FF             JL       0x1406bf2c0
01406C003C: 48 8B 55 AF                   MOV      rdx, qword ptr [rbp - 0x51]
01406C0040: 48 8B 4C 24 38                MOV      rcx, qword ptr [rsp + 0x38]
01406C0045: 48 85 C9                      TEST     rcx, rcx
01406C0048: 74 12                         JE       0x1406c005c
01406C004A: 48 2B D1                      SUB      rdx, rcx
01406C004D: 48 C1 FA 03                   SAR      rdx, 3
01406C0051: 41 B8 08 00 00 00             MOV      r8d, 8
01406C0057: E8 F4 19 94 FF                CALL     0x140001a50
01406C005C: 0F 28 B4 24 C0 00 00 00       MOVAPS   xmm6, xmmword ptr [rsp + 0xc0]
01406C0064: 0F 28 BC 24 B0 00 00 00       MOVAPS   xmm7, xmmword ptr [rsp + 0xb0]
01406C006C: 48 81 C4 D8 00 00 00          ADD      rsp, 0xd8
01406C0073: 41 5F                         POP      r15
01406C0075: 41 5E                         POP      r14
01406C0077: 41 5D                         POP      r13
01406C0079: 41 5C                         POP      r12
01406C007B: 5F                            POP      rdi
01406C007C: 5E                            POP      rsi
01406C007D: 5B                            POP      rbx
01406C007E: 5D                            POP      rbp
01406C007F: C3                            RET
```
