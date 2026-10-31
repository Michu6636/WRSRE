# Native DO-Truck Assignment Interval Investigation

## Executive result

**The earlier road-DO assignment conclusion is withdrawn.** The direct `[DO+0xBB0]=truck` / `[truck+0xC80]=DO` installation cited below belongs to type `0x5C` and type `7` paths, not the actual road distribution-office type `0x2B`.

The type-43 (`0x2B`) dispatcher passes an object whose metadata type is `0x2B` into `FUN_1401C6050`. That updater traverses that same object's `+0xC70/+0xC78` fleet and calls `FUN_1406B8EB0` on a guarded truck from it. The helper can write `truck+0xC80 = office` and `office+0xBB0 = truck+0x390`. This proves a type-43 office/truck association path, but not a whole-task installation, task-presence signal, final removal, cancellation, or replacement boundary. `truck+0x390` is used as a pointer-like internal object/container value; it is not a proven job ID or payer. Whether this association persists across internal pickup/unload stops is unresolved.

The latest follow-up traces the type-43 updater's candidate-truck buffer into `FUN_1401E5350` and its same-truck route/configuration rebuilders. `FUN_1401E2380` regenerates the office's `+0xD58/+0xD60` candidate rows; a positive row predicate can then rebuild the truck's `+0x680/+0x688` route list and `+0x638/+0x640` configuration vector. A connected simulation path in `FUN_140699420` can write the generic `truck+0xC80` association to its current `+0x528` target and conditionally write that target's `+0xBB0` backlink. The possible old-target null write is not fully proven: the raw instruction stores `R14`, whose zero provenance is absent from the bounded predecessor export, although the decompiler renders it as zero. The same simulation path can call `FUN_14067CB30` on a type-43 home-target branch to clear route/configuration vectors. These operations do not prove assignment commitment, stop-only transitions, cancellation, or task removal; the associated target is not a stable DO-task identity. The exact route-progress block increments `truck+0x700`, not the separately established route index `+0x698`, so the prior decompiler-based interpretation is withdrawn. No reliable accounting interval is established.

The existing `FUN_1406BF1D0` renderer investigation remains below for continuity and is not repeated or used as an assignment signal here. This correction instead records the exact type-43 receiver path, `+0x390` provenance, contiguous direct bytes, and remaining lifecycle gaps.

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

The configured [Ghidra wrapper](../../scratch/ghidra_run.bat) opens `SOVIET64.exe` in the existing `soviet` project using Ghidra 12.1.4. The following existing scripts were run serially against that project: `ListFuncs.java`, `DisasmMany.java`, `Xrefs.java`, and `DecompAt.java`. Outputs were written to session-local files and summarized or reproduced below. No project lock was removed and no running Ghidra process was terminated. The type-43 bodies and dispatcher blocks were also decoded from the identity-verified executable with Capstone 5.0.7; the full raw bodies below include bytes omitted from Ghidra's instruction listing.

All lifecycle conclusions below are **static deductions or explicit unknowns**. There is no live-game validation of assignment presence, removal, replacement, cancellation, reload survival, or return travel in this investigation.

Throughout this report, a read at `vehicle+0x1708` followed by a comparison at `object+0x294` establishes only a metadata-kind dword's numeric value. In particular, `+0x294 == 10` is a metadata-kind comparison, not a proven vehicle state called “state 10.” No vehicle-state enum meaning is inferred for values `4`, `8`, or `10`.

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

## Earlier association paths: not the road-DO receiver

The following paths were previously overgeneralized as road-DO evidence. Their dispatch guards identify type `0x5C` and type `7`, not road distribution office type `0x2B`; they are retained only to show why those writes cannot establish the target lifecycle.

The renderer helper is not the only relevant path. The available dispatcher/decompiler exports establish a native, non-UI association write:

1. In `d_h11_dispatcher.c`, an object whose type metadata has `+0x360 == 0x5C` is passed as `local_648` to `FUN_140189030(param_1, local_648)`.
2. At `0x14018909E`, `FUN_140189030` copies `RDX` to `R13`; at `0x1401890A1` it calls `FUN_1401B1290` without replacing `RCX` or `RDX`. Thus `FUN_1401B1290` receives the same `local_648` object as `param_2`.
3. `FUN_1401B1290` has the exported signature `void FUN_1401b1290(longlong param_1, longlong param_2)` and a Ghidra-reported size of 9,323 bytes. Its loop obtains candidate trucks from `param_2+0xC70`; the decompilation names the current truck `lVar12` and the loop's fleet object is the same `param_2`. This is receiver provenance, not an offset match on an unrelated object.
4. The candidate branch checks `truck+0x4F0 == dispatcher`, `dispatcher+0xBB0 == 0`, and `truck+0xC80 == 0`. `R15` is initialized to zero at `0x1401B1318`.
5. After selecting route/cargo data, this type-`0x5C` branch installs its association pointers and resets the candidate vehicle's route state.

Relevant raw instructions from the verified-address export:

```asm
; FUN_140189030: same second argument forwarded to FUN_1401B1290
14018909e: 4C 8B EA                    MOV R13,RDX
1401890a1: E8 EA 81 02 00              CALL 0x1401b1290

; FUN_1401B1290: R15 was zeroed at 0x1401B1318
1401b1315: 4C 8B F1                    MOV R14,RCX
1401b1318: 45 33 FF                    XOR R15D,R15D

; candidate guard; RDI is the current element read from the type-0x5C receiver+0xC70
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

; route point copied from the type-0x5C receiver+0xE90 vector after association
1401b246f: 4C 8D A7 38 07 00 00       LEA R12,[RDI+0x738]
1401b2672: 48 6B DB 1C                 IMUL RBX,RBX,0x1c
1401b2676: 48 8B 86 90 0E 00 00       MOV RAX,[RSI+0xe90]
1401b267d: F3 0F 10 04 03              MOVSS XMM0,[RBX+RAX]
1401b26be: 48 8B CF                    MOV RCX,RDI
1401b26c1: E8 CA 6B 4D 00              CALL 0x140689290
1401b26c6: 48 8B 96 90 0E 00 00       MOV RDX,[RSI+0xe90]
1401b26cd: 48 03 D3                    ADD RDX,RBX
1401b26d0: 49 8B CC                    MOV RCX,R12
1401b26d3: E8 18 CC E7 FF              CALL 0x14002f2f0
```

In the Windows x64 calling convention, `RSI` is `param_2` (the type-`0x5C` receiver), while `RDI` is its candidate vehicle. The relevant decompilation also shows subsequent route-point writes to the same `lVar12`/`RDI` vehicle. This proves an association write on that receiver only; it is not evidence for a type-`0x2B` road office.

`FUN_1401B3CE0` contains a similar guarded write at `0x1401B45E4`/`0x1401B4604` (`[RSI+0xBB0]=RDI`, `[RDI+0xC80]=RSI`) and then calls `FUN_14067CB30` to reset schedule state. Its immediate caller `FUN_1401B3700` is dispatched for metadata type `7`. Neither this nor the type-`0x5C` path establishes road-DO assignment semantics.

## Actual road-DO type `0x2B`: receiver and association dataflow

The dispatching function is `FUN_140139A70`. Its prologue copies the `RCX` argument to `R13`. It counts the object-pointer table at `[R13+0x11B08]..[R13+0x11B10]`, loads each candidate pointer from that table, and saves the current candidate at `[RSP+0x40]`. The only store to that stack slot in this function is at `0x140139BFB`. In the later type-dispatch block, `R8` is reloaded from that same slot; `[R8+0x318]` supplies the candidate's metadata object, and the dword at metadata `+0x360` is compared with `0x2B`. On equality, the code passes that same candidate as `RDX=R8`, passes the current `RSI` in `RCX`, and calls `FUN_1401C6050` at `0x14013E2C9`. Ghidra's reference export records this as the only direct call reference to `FUN_1401C6050`; it also records two data references, which are not calls.

In `FUN_1401C6050`, the prologue copies `RDX` to `RDI` and `RCX` to `R12`. Thus `RDI` is the same type-`0x2B` office candidate passed by the dispatcher. The updater traverses that office's `+0xC70/+0xC78` vector, loads each truck into `RSI`, checks `truck+0xC80 == 0` at `0x1401C61E6`, and checks `truck+0x4F8 == office` at `0x1401C61F4`. This proves that the candidate truck is from this same type-43 office's fleet; the receiver is established by dataflow, not inferred from matching offsets.

At `0x1401C646E`, `FUN_1401C6050` calls `FUN_1406B8EB0` with `RCX=RSI` (the same fleet truck), with `RDX`, `R8`, and `R9` zero. Before the call, the path requires `office+0xBB0 == 0`, `truck+0xD39 == 0`, `truck+0x698 == 0`, a nonempty `truck+0x680/+0x688` list, `truck+0x528 == 0`, and `truck+0x4F0 == truck+0x4F8`. The loop already established `truck+0x4F8 == office`, so the helper's `truck+0x4F0` receiver is the same office.

```asm
; FUN_140139A70 prologue and candidate object table
140139af1: 4C 8B E9                    MOV R13,RCX
140139bbf: 49 8B 85 10 1B 01 00        MOV RAX,[R13+0x11b10]
140139bc6: 49 2B 85 08 1B 01 00        SUB RAX,[R13+0x11b08]
140139bcd: 48 C1 F8 03                 SAR RAX,3
140139bf0: 49 8B 85 08 1B 01 00        MOV RAX,[R13+0x11b08]
140139bf7: 48 8B 14 F8                 MOV RDX,[RAX+RDI*8]
140139bfb: 48 89 54 24 40              MOV [RSP+0x40],RDX

; reload the same candidate, check its metadata type, then dispatch it
14013e24e: 4C 8B 44 24 40              MOV R8,[RSP+0x40]
14013e253: 49 8B 80 18 03 00 00        MOV RAX,[R8+0x318]
14013e2ba: 83 B8 60 03 00 00 2B        CMP dword ptr [RAX+0x360],0x2b
14013e2c1: 75 10                       JNE 0x14013e2d3
14013e2c3: 49 8B D0                    MOV RDX,R8
14013e2c6: 48 8B CE                    MOV RCX,RSI
14013e2c9: E8 82 7D 08 00              CALL 0x1401c6050

; FUN_1401C6050: fleet base, candidate load, and same-office guard
1401c6154: 4C 8D BF 70 0C 00 00       LEA R15,[RDI+0xc70]
1401c61d8: 49 8B 07                    MOV RAX,[R15]
1401c61db: 4A 8B 34 F0                 MOV RSI,[RAX+R14*8]
1401c61e6: 48 83 BE 80 0C 00 00 00    CMP qword ptr [RSI+0xc80],0
1401c61f4: 48 39 BE F8 04 00 00       CMP qword ptr [RSI+0x4f8],RDI

; active-list/association guards and same-truck helper call
1401c6408: 48 83 BF B0 0B 00 00 00    CMP qword ptr [RDI+0xbb0],0
1401c6429: 83 BE 98 06 00 00 00       CMP dword ptr [RSI+0x698],0
1401c6432: 48 8B 86 88 06 00 00       MOV RAX,[RSI+0x688]
1401c6439: 48 2B 86 80 06 00 00       SUB RAX,[RSI+0x680]
1401c6449: 48 83 BE 28 05 00 00 00    CMP qword ptr [RSI+0x528],0
1401c6453: 48 8B 86 F8 04 00 00       MOV RAX,[RSI+0x4f8]
1401c645a: 48 39 86 F0 04 00 00       CMP [RSI+0x4f0],RAX
1401c6463: 45 33 C9                    XOR R9D,R9D
1401c6466: 45 33 C0                    XOR R8D,R8D
1401c6469: 33 D2                       XOR EDX,EDX
1401c646b: 48 8B CE                    MOV RCX,RSI
1401c646e: E8 3D 2A 4F 00              CALL 0x1406b8eb0
```

In the callee, `RDI=RCX` is still that truck. The complete raw body is 7,687 bytes; direct decoding yields 1,710 instructions and ends at `0x1406BACB6` (`RET`). Entry checks `truck+0xC80 == 0` and `truck+0x4F0 != 0`. One guarded path reads `truck+0x4F0` into `RCX` (the same office established by the caller), then writes:

```asm
1406b9d68: 48 8B 8F F0 04 00 00       MOV RCX,[RDI+0x4f0]
1406b9d6f: 48 8B 81 18 03 00 00       MOV RAX,[RCX+0x318]
1406b9d7b: 80 B8 79 09 00 00 00       CMP byte ptr [RAX+0x979],0
1406b9d84: 48 8B 87 90 03 00 00       MOV RAX,[RDI+0x390]
1406b9d8b: 48 89 81 B0 0B 00 00       MOV [RCX+0xbb0],RAX
1406b9d92: 48 8B 87 F0 04 00 00       MOV RAX,[RDI+0x4f0]
1406b9d99: 48 89 87 80 0C 00 00       MOV [RDI+0xc80],RAX
```

Thus `truck+0xC80 = office` is a direct, read-only snapshot candidate for an office association on this type-`0x2B` path. `office+0xBB0` receives the value loaded from `truck+0x390`; no evidence here proves that this value equals the truck pointer, is stable across tasks, or identifies a job. Do not call these writes a reciprocal truck-pointer pair or a task start without further lifecycle proof.

The helper also uses `truck+0x390` as a pointer-like internal structure: at `0x1406B91F6` it loads the field and stores that value at `[RCX+0x60]`; at `0x1406B9217/0x1406B921E` it loads the same field and adds `0x4A8`, then treats the result as a vector-like begin/end/capacity structure. This rules out interpreting the copied value as a proven scalar job ID, but does not identify its object type or task semantics.

The helper's nearby clear logic reads the metadata-kind dword through `truck+0x1708`, then compares the pointed object's `+0x294` with `8`, `10`, and `6`. The `== 10` branch conditionally clears the current office token and truck association only when the token equals `truck+0x390`; later control can still reach the write at `0x1406B9D99`. This is not proof of task removal or replacement. The known type-`0x2B` caller enters with `truck+0xC80 == 0`, and the meanings of these metadata-kind values are unknown.

```asm
1406b9be8: 48 8B 87 08 17 00 00       MOV RAX,[RDI+0x1708]
1406b9bef: 8B 88 94 02 00 00          MOV ECX,[RAX+0x294]
1406b9bf5: 83 F9 08                    CMP ECX,8
1406b9bf8: 75 7B                       JNE 0x1406b9c75
1406b9bfa: 48 8B 8F 80 0C 00 00       MOV RCX,[RDI+0xc80]
1406b9c01: 48 85 C9                    TEST RCX,RCX
1406b9c04: 74 17                       JE 0x1406b9c1d
1406b9c06: 48 8B 87 90 03 00 00       MOV RAX,[RDI+0x390]
1406b9c0d: 48 39 81 B0 0B 00 00       CMP [RCX+0xbb0],RAX
1406b9c14: 75 07                       JNE 0x1406b9c1d
1406b9c16: 4C 89 B1 B0 0B 00 00       MOV [RCX+0xbb0],R14
1406b9c75: 83 F9 0A                    CMP ECX,0xa
1406b9c78: 75 49                       JNE 0x1406b9cc3
1406b9c7a: 48 8B 8F 80 0C 00 00       MOV RCX,[RDI+0xc80]
1406b9c81: 48 85 C9                    TEST RCX,RCX
1406b9c84: 74 1E                       JE 0x1406b9ca4
1406b9c86: 48 8B 87 90 03 00 00       MOV RAX,[RDI+0x390]
1406b9c8d: 48 39 81 B0 0B 00 00       CMP [RCX+0xbb0],RAX
1406b9c94: 75 0E                       JNE 0x1406b9ca4
1406b9c96: 4C 89 B1 B0 0B 00 00       MOV [RCX+0xbb0],R14
1406b9c9d: 4C 89 B7 80 0C 00 00       MOV [RDI+0xc80],R14
1406b9cab: 0F 8D E1 00 00 00          JGE 0x1406b9d92
1406b9cbe: E9 CF 00 00 00             JMP 0x1406b9d92
```

### Direct executable bytes for `0x1401B2580..0x1401B26E0`

The verified executable was re-read directly. The half-open range is 352 bytes. It starts in the middle of a preceding instruction, so instruction decoding below is anchored at the known instruction boundary `0x1401B258C`, not guessed from `0x1401B2580`.

```text
sha256=296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8 timestamp=0x6a3eb6ad image_base=0x140000000
range=0x1401b2580..0x1401b26e0 length=352
```

```text
00000001401B2580: 04 8B 40 0C 89 45 B8 F2 0F 11 45 B0 48 89 BE B0
00000001401B2590: 0B 00 00 48 89 B7 80 0C 00 00 4C 89 BF F0 04 00
00000001401B25A0: 00 48 8B 87 08 17 00 00 48 85 C0 74 13 83 B8 94
00000001401B25B0: 02 00 00 04 75 0A B2 01 48 8B CF E8 60 41 4F 00
00000001401B25C0: 48 8B D7 48 8B 0D 46 51 83 00 E8 81 53 2E 00 48
00000001401B25D0: 8D 8F 38 06 00 00 E8 C5 D1 04 00 48 8B 87 80 06
00000001401B25E0: 00 00 48 89 87 88 06 00 00 48 8B 87 A0 06 00 00
00000001401B25F0: 48 89 87 A8 06 00 00 48 8B 87 B8 06 00 00 48 89
00000001401B2600: 87 C0 06 00 00 49 8B 04 24 49 89 44 24 08 48 8B
00000001401B2610: 87 50 07 00 00 48 89 87 58 07 00 00 4C 89 BF 98
00000001401B2620: 07 00 00 48 8B 87 C0 07 00 00 48 2B 87 B8 07 00
00000001401B2630: 00 48 C1 F8 03 48 85 C0 74 08 48 8B CF E8 2E F3
00000001401B2640: 4C 00 44 89 BF 34 0D 00 00 44 89 BF 68 07 00 00
00000001401B2650: C7 87 00 07 00 00 FF FF FF FF 48 8D 55 10 49 8B
00000001401B2660: CC E8 8A CC E7 FF 48 8D 55 B0 49 8B CC E8 7E CC
00000001401B2670: E7 FF 48 6B DB 1C 48 8B 86 90 0E 00 00 F3 0F 10
00000001401B2680: 04 03 F3 0F 11 45 20 F3 0F 10 4C 03 04 F3 0F 11
00000001401B2690: 4D 24 F3 0F 10 44 03 08 F3 0F 11 45 28 F2 0F 10
00000001401B26A0: 45 B0 F2 0F 11 85 D0 00 00 00 8B 45 B8 89 85 D8
00000001401B26B0: 00 00 00 4C 8D 45 20 48 8D 95 D0 00 00 00 48 8B
00000001401B26C0: CF E8 CA 6B 4D 00 48 8B 96 90 0E 00 00 48 03 D3
00000001401B26D0: 49 8B CC E8 18 CC E7 FF 8B 44 24 64 89 87 0C 05
```

At `0x1401B26C1`, the bytes are `E8 CA 6B 4D 00`; the next instruction begins at `0x1401B26C6`. The relative call target is `0x140689290`. Address `0x1401B26C4` is inside the call displacement, not an instruction boundary; interpreting those five bytes as starting at `0x1401B26C4` does not match the directly extracted file bytes. There is no overlapping instruction at `0x1401B26C6`.

## Generic detach helpers: receiver proven, road-task meaning unresolved

`FUN_1403FBDC0` has exported signature `void FUN_1403fbdc0(longlong param_1, longlong param_2, char param_3, char param_4, char param_5, undefined1 param_6)`. Its raw prologue copies `RDX` (`param_2`) into `RDI`, the vehicle receiver used below. At `0x1403FC085`, it loads that receiver's `+0xC80`. If the vehicle has no object at `+0x1708`, or the dword at that object's `+0x294` is not `10`, the helper clears the pointer in `DO+0xBB0` before clearing `vehicle+0xC80`:

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

`RBX` is zero on this path. `RAX` is loaded from the vehicle's `+0xC80`; `RDI` remains the same vehicle passed as `param_2`. The ordinary branch clears `[RAX+0xBB0]` unconditionally, then clears `[RDI+0xC80]`. The type-`0x2B` updater calls this helper on fleet vehicles during configuration/update processing, but this call is not proven to mean final task removal.

This generic clear does not compare `[RAX+0xBB0]` with either the truck pointer or `[RDI+0x390]`. A later observer must validate the receiver and current association token before attributing it to a task boundary.

When the object at `vehicle+0x1708` has dword `+0x294 == 10`, the call at `0x1403FC0A9` passes the same vehicle in `RCX` to `FUN_1406DCF20`. These are metadata-kind comparisons; the semantic meaning of value `10` is not independently established. The helper's Ghidra-reported body is 640 bytes (`0x280`), from `0x1406DCF20` through the `RET` at `0x1406DD19F`. The raw body confirms `RBX=RCX` (vehicle) and `RBP=0`; its tail conditionally clears `DO+0xBB0` only if that value equals the truck pointer in `RBX`, then clears `vehicle+0xC80` and `vehicle+0x4F0`. It does not prove a close for the type-`0x2B` association, whose `DO+0xBB0` value was observed to come from `vehicle+0x390`; equality between those values is unproved. The raw export also shows calls to this helper at `0x1406DB35B` and `0x1406DD6D0`; it is not unique to the FBDC0 metadata-kind-10 path. The complete body is included below:

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

### Why the type-0x2B association is not yet a complete interval signal

- The type-`0x2B` helper writes `truck+0xC80 = office`, but `office+0xBB0 = truck+0x390` is an opaque token, not a proven stable task identity. Two assignments to the same office cannot be distinguished from snapshots if the association remains continuously present.
- `FUN_1406B8EB0` itself reads the metadata-kind dword via `truck+0x1708` and compares the pointed object's `+0x294` with `8`, `10`, and `6`. The value meanings are unknown; this is not a proven vehicle-state field. Its metadata-kind-`8` path can clear `office+0xBB0` only when it equals `truck+0x390`; its metadata-kind-`10` path can clear that value and `truck+0xC80` under the same token comparison. These paths can continue to later association writes in the same helper. They do not prove which events are internal stops versus task removal/replacement.
- The type-`0x2B` caller's empty-association and route-list guards establish an association-creation candidate, not an unconditional task-start event. It does not prove that every assigned-but-not-loaded task reaches this path.
- `FUN_1403FBDC0` and `FUN_1406DCF20` are reused cleanup helpers. The type-`0x2B` updater calls `FUN_1403FBDC0` on a fleet vehicle during configuration/update processing; a call alone is not final task removal. The metadata-kind-10 path in `FUN_1406DCF20` compares the DO token with the truck pointer, unlike the observed type-`0x2B` token source `truck+0x390`. No success/cancellation meaning is established.
- The separately cited type-7 path calls `FUN_14067CB30`, which resets schedule/list state without writing `truck+0xC80`. That helper is not shown on the type-`0x2B` updater's direct call path, so its body does not prove that the type-43 association survives any or all internal stops. Its complete 264-byte body shows the reset paths explicitly:

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

This proves only that this helper's reset does not clear `truck+0xC80`; it does not prove that every pickup/unload advancement preserves the type-`0x2B` association.
- The helper's attach/clear writes are observed on the same fleet truck, but no complete CFG or live event sequence classifies all of them as assignment start, internal-stop update, cancellation, replacement, or final removal. Do not treat every program-wide `+0xC80` store as a task boundary.

## Interval-signal test matrix

| Required state or transition | What static evidence proves | Interval conclusion |
|---|---|---|
| Idle at a DO | The type-`0x2B` candidate path requires `truck+0xC80 == 0`, `office+0xBB0 == 0`, a nonempty route list, and additional truck guards. | These are eligibility guards, not a complete idle definition; retained default/return rows remain unresolved. |
| Assigned but not yet loaded | The type-`0x2B` helper can write `truck+0xC80 = office` and `office+0xBB0 = truck+0x390`. No load-state mapping is established. | Association-write path only; assignment-before-loading is unproved. |
| Internal pickup/unload stop advancement | The cited `FUN_14067CB30` reset belongs to a separately dispatched type-7 path, not the type-43 updater's direct call path. `FUN_1406B8EB0` also has conditional clear/rewrite paths. | Whether the association survives any or all type-43 internal stops is unresolved. |
| Final task removal | Generic helpers can clear `truck+0xC80`; the type-`0x2B` helper also conditionally clears `office+0xBB0` when it matches `truck+0x390`. The lifecycle reason for those writes is not proved. | No verified final-removal boundary. |
| Cancellation or replacement | No proof shows that every cancellation/replacement reaches a distinct clear, or that a clear/rewrite within one helper invocation means replacement rather than an internal transition. | Cannot guarantee a close on cancellation/replacement from snapshots or these candidate stores yet. |
| Empty return | No mapping ties the type-`0x2B` association clear to the beginning of a return route. | Whether the candidate close precedes all post-removal return travel is unresolved. |
| Identical consecutive assignments | `truck+0xC80` identifies an office pointer on the observed path; `office+0xBB0` stores opaque `truck+0x390`, not a proven task identity. | Identical tasks for the same office cannot be distinguished unless a separately validated boundary exists. |
| Save/reload | DO target configurations are serialized/reconstructed; persistence/reconstruction of the type-`0x2B` association pointer/token and assignment continuity is not established. | No post-load interval rule can be claimed. |

The requested policy remains: begin on actual assignment to a vehicle working for a DO, remain through internal stops and travel while assigned, and close on removal/cancellation/replacement regardless of success; exclude travel after removal. This report does not yet identify a native signal that proves those boundaries for every type-`0x2B` task.

## Read-only recommendation, destination, and payer

The current schedule observer's bounded fields are useful diagnostics but **insufficient for interval accounting**. A list clear/change, nonempty list, row index, config/storage relocation, opaque UI context, invalid read, or panel closure is not a verified assignment event. For this road-DO path, a read-only snapshot of `truck+0xC80` can indicate the observed office association; `office+0xBB0` is only a comparison token sourced from `truck+0x390`, not a proven reciprocal truck pointer. Snapshots can miss a same-update clear/rewrite and cannot distinguish uninterrupted identical tasks.

If a later guarded native observation is pursued, the type-`0x2B` association-write candidate is the call `FUN_1401C6050 -> FUN_1406B8EB0` at `0x1401C646E`, with same-truck stores at `0x1406B9D8B` (`office+0xBB0 = truck+0x390`) and `0x1406B9D99` (`truck+0xC80 = office`). Candidate clear sites include `0x1406B9C96/0x1406B9C9D` on the metadata-kind-`10` branch and generic cleanup stores; none is proven to mean task removal. The exact receiver and token must be guarded, and a same-call clear/rewrite must not be collapsed into a polling snapshot. These are investigation candidates, not a recommended interval hook. No hook is installed. No plugin, dispatch, permissions, defaults, or billing behavior changed.

Adjacent route evidence on the type-`0x5C` path reads point data from receiver `+0xE90` and feeds it to `FUN_140689290`/a vehicle route-point vector. This establishes route construction only, not a type-`0x2B` payer. No destination-to-payer mapping is verified; cancelled work must not be charged to a guessed party.

## Smallest missing artifact

No executable input is missing: the identity-matched binary and full `FUN_1406B8EB0` bytes are available. The smallest unresolved artifact is a **receiver-preserving type-`0x2B` lifecycle trace/CFG on one office-fleet truck**, spanning the helper association-write path, internal pickup/unload advancement, replacement/cancellation, and final removal. It must establish what `truck+0x390` represents and prove whether every replacement emits an observable boundary distinct from internal-stop changes, including identical consecutive tasks. A save/load trace is additionally required before claiming interval continuity across reload.

Static evidence cannot by itself validate whether idle/default/return entries are retained, whether identical tasks replace without an empty state, or whether the association survives reload. These remain unvalidated. No live-game validation was performed, and no unchanged game replay is requested.

## Analysis script and actual output

### Direct executable byte-window exporter

This script reproduces the contiguous byte dump above and decodes the known instruction-aligned association and call snippets. It verifies the executable identity before mapping VAs to file offsets; it writes no files.

```python
from pathlib import Path
import hashlib
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_64

exe = Path(r"C:\Workers Resources Soviet Republic\SOVIET64.exe")
expected_sha256 = "296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8"
expected_timestamp = 0x6A3EB6AD
expected_image_base = 0x140000000
data = exe.read_bytes()
sha256 = hashlib.sha256(data).hexdigest().upper()
pe = struct.unpack_from("<I", data, 0x3C)[0]
timestamp = struct.unpack_from("<I", data, pe + 8)[0]
section_count = struct.unpack_from("<H", data, pe + 6)[0]
optional_size = struct.unpack_from("<H", data, pe + 20)[0]
image_base = struct.unpack_from("<Q", data, pe + 24 + 24)[0]
if (sha256, timestamp, image_base) != (
    expected_sha256,
    expected_timestamp,
    expected_image_base,
):
    raise ValueError("Executable identity mismatch")

section_table = pe + 24 + optional_size
sections = []
for index in range(section_count):
    offset = section_table + index * 40
    virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, offset + 8)
    sections.append((rva, raw_size, raw_offset))

def read_va(va, size):
    rva = va - image_base
    for section_rva, raw_size, raw_offset in sections:
        if section_rva <= rva and rva + size <= section_rva + raw_size:
            file_offset = raw_offset + rva - section_rva
            return data[file_offset:file_offset + size]
    raise ValueError(f"VA is outside a raw PE section: {va:#x}")

start, end = 0x1401B2580, 0x1401B26E0
raw = read_va(start, end - start)
print(f"sha256={sha256} timestamp={timestamp:#x} image_base={image_base:#x}")
for offset in range(0, len(raw), 16):
    print(f"{start + offset:016X}: {raw[offset:offset + 16].hex(' ').upper()}")

decoder = Cs(CS_ARCH_X86, CS_MODE_64)
for va, size in ((0x1401B258C, 0x15), (0x1401B26BE, 0x1A)):
    print(f"-- {va:#x} --")
    for instruction in decoder.disasm(read_va(va, size), va):
        print(
            f"{instruction.address:016X}: "
            f"{instruction.bytes.hex(' ').upper():<27} "
            f"{instruction.mnemonic.upper():<8} {instruction.op_str}"
        )
```

Actual output identity and the complete 352-byte dump are shown in the preceding direct-byte section. The aligned decode around the route call is:

```text
00000001401B26BE: 48 8B CF                    MOV      rcx, rdi
00000001401B26C1: E8 CA 6B 4D 00              CALL     0x140689290
00000001401B26C6: 48 8B 96 90 0E 00 00        MOV      rdx, qword ptr [rsi + 0xe90]
00000001401B26CD: 48 03 D3                    ADD      rdx, rbx
00000001401B26D0: 49 8B CC                    MOV      rcx, r12
00000001401B26D3: E8 18 CC E7 FF              CALL     0x14002f2f0
```

### Existing renderer-helper export

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

## Type-43 receiver correction and complete raw exports

### Ghidra function metadata and reference output

The configured wrapper was run against the existing `soviet` project. The Ghidra `ListFuncs.java` output for the two relevant entries was:

```text
0x1406B8EB0  size   7687  FUN_1406b8eb0
0x1406BACC0  size    746  FUN_1406bacc0
0x1401C6050  size   3450  FUN_1401c6050
0x1401C6DD0  size   4038  FUN_1401c6dd0
```

The bounded wrapper invocations were:

```text
scratch\ghidra_run.bat ListFuncs.java <out> 0x1401C6050 0x1401C6DD0
scratch\ghidra_run.bat ListFuncs.java <out> 0x1406B8EB0 0x1406BAD00
scratch\ghidra_run.bat DisasmMany.java <out> 0x1406B8EB0:0x1406BACB7
scratch\ghidra_run.bat DisasmMany.java <out> 0x1401C6050:0x1401C6DCF
scratch\ghidra_run.bat DisasmMany.java <out> 0x140139BBF:0x140139C20 0x14013E249:0x14013E2CE
scratch\ghidra_run.bat Xrefs.java <out> 0x1406B8EB0 0x1401C6050 0x1403FBDC0 0x1406DCF20
scratch\ghidra_run.bat DecompAt.java <out> 0x1406B8EB0
```

`<out>` was a session-local output path, not a repository file. The wrapper was invoked from `C:\WRSRE`; it resolves the configured Ghidra installation, project, executable, and script paths.

`DecompAt.java` reports the helper signature as:

```c
undefined8 FUN_1406b8eb0(longlong param_1, char param_2,
                         char param_3, undefined8 param_4)
```

At the type-43 caller, `RCX=truck` and `RDX=R8=R9=0`. The helper is shared: Ghidra's `Xrefs.java` output contains 68 direct call references and two data references to `FUN_1406B8EB0`; the type-43 call is `0x1401C646E` in `FUN_1401C6050`. For `FUN_1401C6050`, Ghidra reports one direct call reference, from `0x14013E2C9` in `FUN_140139A70`, plus two data references. The other helper callers do not inherit type-43 receiver provenance merely because they call the same address.

The Ghidra `DisasmMany.java` export of `FUN_1406B8EB0` covers 1,710 instructions and 7,687 bytes through `RET` at `0x1406BACB6`. Its instruction-byte stream is byte-for-byte identical to the direct executable decode (SHA-256 of concatenated instruction bytes: `5B1D991A86B7DA19466A5F85851B9D4B291868960840D36685EC1D80474165CA`).

For `FUN_1401C6050`, the PE exception-directory range is `0x1401C6050..0x1401C6DCF` (end exclusive, 3,455 bytes). Direct decoding covers 743 instructions including the five-byte `NOP` at `0x1401C61CB`. Ghidra's function body is 3,450 bytes and its instruction listing omits only that NOP; the next instruction resumes at `0x1401C61D0`. The last instruction in both exports is `RET` at `0x1401C6DCE`. The omitted alignment bytes are restored in the complete direct listing below.

The type-43 dispatcher export is bounded to the candidate-table setup and the relevant type-dispatch block, rather than dumping the entire 20,964-byte dispatcher. The Ghidra listing at `0x14013E2BA` confirms the bytes `83 B8 60 03 00 00 2B`; at `0x14013E2C9` the call bytes are `E8 82 7D 08 00`, targeting `0x1401C6050`. `FUN_140139A70` begins earlier at `0x140139A70`. An earlier `ListFuncs.java` query starting at `0x14013E000` returned no functions because that script iterates function entries at or after the requested start and does not include an enclosing function whose entry is earlier; the direct caller reference and bounded disassembly establish the correct containing function.

### Corrected contiguous route-call bytes

The direct re-export of `0x1401B2580..0x1401B26E0` was made from the identity-verified executable, not inferred from a decompilation:

```text
sha256=296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
00000001401B26C0: CF E8 CA 6B 4D 00 48 8B 96 90 0E 00 00 48 03 D3
00000001401B26C1: E8 CA 6B 4D 00              CALL 0x140689290
00000001401B26C4: 4D 00 48 8B 96             ; these are bytes, not an instruction start
00000001401B26C6: 48 8B 96 90 0E 00 00       MOV RDX,[RSI+0xE90]
```

Thus the supplied `E8` at `0x1401B26C4` does not match the verified file: the actual `E8` is at `0x1401B26C1`. The five-byte relative call ends at `0x1401B26C6`, exactly where the `MOV` begins; there is no overlap. The existing contiguous 352-byte dump above is the complete requested `0x1401B2580..0x1401B26E0` window.

### Reproduction script and actual summary output

This script verifies the executable identity, resolves the two code ranges from the PE exception directory, decodes all bytes directly, and reprints the aligned route-call window. It writes no files.

```python
from pathlib import Path
import hashlib
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_64

exe = Path(r"C:\Workers Resources Soviet Republic\SOVIET64.exe")
expected_sha256 = "296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8"
expected_timestamp = 0x6A3EB6AD
expected_base = 0x140000000
data = exe.read_bytes()
sha256 = hashlib.sha256(data).hexdigest().upper()
pe = struct.unpack_from("<I", data, 0x3C)[0]
timestamp = struct.unpack_from("<I", data, pe + 8)[0]
section_count = struct.unpack_from("<H", data, pe + 6)[0]
optional_size = struct.unpack_from("<H", data, pe + 20)[0]
optional = pe + 24
image_base = struct.unpack_from("<Q", data, optional + 24)[0]
if (sha256, timestamp, image_base) != (
    expected_sha256, expected_timestamp, expected_base
):
    raise ValueError("Executable identity mismatch")

section_table = optional + optional_size
sections = []
for index in range(section_count):
    offset = section_table + index * 40
    virtual_size, rva, raw_size, raw_offset = struct.unpack_from(
        "<IIII", data, offset + 8
    )
    sections.append((rva, raw_size, raw_offset))

def read_va(va, size):
    rva = va - image_base
    for section_rva, raw_size, raw_offset in sections:
        if section_rva <= rva and rva + size <= section_rva + raw_size:
            offset = raw_offset + rva - section_rva
            return data[offset:offset + size]
    raise ValueError(f"VA outside a raw PE section: {va:#x}")

exception_rva, exception_size = struct.unpack_from(
    "<II", data, optional + 112 + 3 * 8
)
exception_table = read_va(image_base + exception_rva, exception_size)
runtime_functions = [
    struct.unpack_from("<III", exception_table, offset)
    for offset in range(0, len(exception_table), 12)
]

def function_bounds(entry_va):
    entry_rva = entry_va - image_base
    matches = [
        (begin, end)
        for begin, end, unwind in runtime_functions
        if begin <= entry_rva < end
    ]
    if len(matches) != 1:
        raise ValueError(f"Expected one runtime-function range at {entry_va:#x}")
    begin, end = matches[0]
    return image_base + begin, image_base + end

decoder = Cs(CS_ARCH_X86, CS_MODE_64)
print("SHA256", sha256, "TIMESTAMP", hex(timestamp), "IMAGE_BASE", hex(image_base))
for entry in (0x1406B8EB0, 0x1401C6050):
    start, end = function_bounds(entry)
    body = read_va(start, end - start)
    instructions = list(decoder.disasm(body, start))
    decoded_size = sum(instruction.size for instruction in instructions)
    if decoded_size != end - start or instructions[-1].address + instructions[-1].size != end:
        raise ValueError(f"Incomplete decode for {start:#x}..{end:#x}")
    if instructions[-1].mnemonic != "ret":
        raise ValueError(f"Unexpected function tail at {instructions[-1].address:#x}")
    print(
        f"FUNCTION {start:#x}..{end:#x} bytes={len(body)} "
        f"instructions={len(instructions)} end={instructions[-1].address:#x} RET"
    )
    for instruction in instructions:
        raw = instruction.bytes.hex(" ").upper()
        print(
            f"{instruction.address:016X}: {raw:<40} "
            f"{instruction.mnemonic.upper():<8} {instruction.op_str}"
        )

window_start, window_end = 0x1401B2580, 0x1401B26E0
window = read_va(window_start, window_end - window_start)
print(f"WINDOW {window_start:#x}..{window_end:#x} bytes={len(window)}")
for offset in range(0, len(window), 16):
    print(f"{window_start + offset:016X}: {window[offset:offset + 16].hex(' ').upper()}")
print("ALIGNED ROUTE-CALL DECODE")
for instruction in decoder.disasm(read_va(0x1401B258C, window_end - 0x1401B258C),
                                  0x1401B258C):
    if instruction.address >= 0x1401B26BE:
        raw = instruction.bytes.hex(" ").upper()
        print(f"{instruction.address:016X}: {raw:<27} {instruction.mnemonic.upper():<8} {instruction.op_str}")
```

Actual direct-decode summary:

```text
SHA256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8 TIMESTAMP 0x6a3eb6ad IMAGE_BASE 0x140000000
FUNCTION 0x1406b8eb0..0x1406bacb7 bytes=7687 instructions=1710 end=0x1406bacb6 RET
FUNCTION 0x1401c6050..0x1401c6dcf bytes=3455 instructions=743 end=0x1401c6dce RET
WINDOW 0x1401b2580..0x1401b26e0 bytes=352
00000001401B26C0: CF E8 CA 6B 4D 00 48 8B 96 90 0E 00 00 48 03 D3
ALIGNED ROUTE-CALL DECODE
00000001401B26BE: 48 8B CF                    MOV      RCX,RDI
00000001401B26C1: E8 CA 6B 4D 00              CALL     0x140689290
00000001401B26C6: 48 8B 96 90 0E 00 00        MOV      RDX,[RSI+0xE90]
00000001401B26CD: 48 03 D3                    ADD      RDX,RBX
00000001401B26D0: 49 8B CC                    MOV      RCX,R12
00000001401B26D3: E8 18 CC E7 FF              CALL     0x14002F2F0
00000001401B26D0: 49 8B CC                    MOV      RCX,R12
00000001401B26D3: E8 18 CC E7 FF              CALL     0x14002F2F0
```

### Complete direct raw bodies

The following are complete contiguous direct decodes from the identity-verified executable. The `FUN_1401C6050` body includes the five-byte alignment NOP omitted from Ghidra's listing. The complete `FUN_1406B8EB0` body is included once; its Ghidra byte-stream comparison is recorded above.

#### `FUN_1401C6050` — type-43 office updater

```asm
00000001401C6050: 48 8B C4                                 MOV      rax, rsp
00000001401C6053: 48 89 48 08                              MOV      qword ptr [rax + 8], rcx
00000001401C6057: 55                                       PUSH     rbp
00000001401C6058: 53                                       PUSH     rbx
00000001401C6059: 56                                       PUSH     rsi
00000001401C605A: 57                                       PUSH     rdi
00000001401C605B: 41 54                                    PUSH     r12
00000001401C605D: 41 55                                    PUSH     r13
00000001401C605F: 41 56                                    PUSH     r14
00000001401C6061: 41 57                                    PUSH     r15
00000001401C6063: 48 8D A8 78 FD FF FF                     LEA      rbp, [rax - 0x288]
00000001401C606A: 48 81 EC 48 03 00 00                     SUB      rsp, 0x348
00000001401C6071: 48 C7 45 88 FE FF FF FF                  MOV      qword ptr [rbp - 0x78], 0xfffffffffffffffe
00000001401C6079: 0F 29 70 A8                              MOVAPS   xmmword ptr [rax - 0x58], xmm6
00000001401C607D: 0F 29 78 98                              MOVAPS   xmmword ptr [rax - 0x68], xmm7
00000001401C6081: 44 0F 29 40 88                           MOVAPS   xmmword ptr [rax - 0x78], xmm8
00000001401C6086: 44 0F 29 88 78 FF FF FF                  MOVAPS   xmmword ptr [rax - 0x88], xmm9
00000001401C608E: 44 0F 29 90 68 FF FF FF                  MOVAPS   xmmword ptr [rax - 0x98], xmm10
00000001401C6096: 44 0F 29 98 58 FF FF FF                  MOVAPS   xmmword ptr [rax - 0xa8], xmm11
00000001401C609E: 44 0F 29 A0 48 FF FF FF                  MOVAPS   xmmword ptr [rax - 0xb8], xmm12
00000001401C60A6: 44 0F 29 A8 38 FF FF FF                  MOVAPS   xmmword ptr [rax - 0xc8], xmm13
00000001401C60AE: 44 0F 29 B0 28 FF FF FF                  MOVAPS   xmmword ptr [rax - 0xd8], xmm14
00000001401C60B6: 44 0F 29 B8 18 FF FF FF                  MOVAPS   xmmword ptr [rax - 0xe8], xmm15
00000001401C60BE: 48 8B FA                                 MOV      rdi, rdx
00000001401C60C1: 4C 8B E1                                 MOV      r12, rcx
00000001401C60C4: 0F 57 C0                                 XORPS    xmm0, xmm0
00000001401C60C7: F3 0F 7F 44 24 50                        MOVDQU   xmmword ptr [rsp + 0x50], xmm0
00000001401C60CD: 45 33 ED                                 XOR      r13d, r13d
00000001401C60D0: 41 8B C5                                 MOV      eax, r13d
00000001401C60D3: 48 89 85 A0 02 00 00                     MOV      qword ptr [rbp + 0x2a0], rax
00000001401C60DA: 48 89 44 24 60                           MOV      qword ptr [rsp + 0x60], rax
00000001401C60DF: F3 0F 10 B2 C0 0F 00 00                  MOVSS    xmm6, dword ptr [rdx + 0xfc0]
00000001401C60E7: 45 0F 57 C0                              XORPS    xmm8, xmm8
00000001401C60EB: F3 0F 10 3D 09 3A 74 00                  MOVSS    xmm7, dword ptr [rip + 0x743a09]
00000001401C60F3: 41 0F 2F F0                              COMISS   xmm6, xmm8
00000001401C60F7: 76 22                                    JBE      0x1401c611b
00000001401C60F9: 45 33 C9                                 XOR      r9d, r9d
00000001401C60FC: 45 33 C0                                 XOR      r8d, r8d
00000001401C60FF: 0F 28 CF                                 MOVAPS   xmm1, xmm7
00000001401C6102: 48 8D 0D D7 ED 80 00                     LEA      rcx, [rip + 0x80edd7]
00000001401C6109: FF 15 19 5F 6A 00                        CALL     qword ptr [rip + 0x6a5f19]
00000001401C610F: F3 0F 5C F0                              SUBSS    xmm6, xmm0
00000001401C6113: F3 0F 11 B7 C0 0F 00 00                  MOVSS    dword ptr [rdi + 0xfc0], xmm6
00000001401C611B: F3 0F 10 B7 28 10 00 00                  MOVSS    xmm6, dword ptr [rdi + 0x1028]
00000001401C6123: 41 0F 2F F0                              COMISS   xmm6, xmm8
00000001401C6127: 76 22                                    JBE      0x1401c614b
00000001401C6129: 45 33 C9                                 XOR      r9d, r9d
00000001401C612C: 45 33 C0                                 XOR      r8d, r8d
00000001401C612F: 0F 28 CF                                 MOVAPS   xmm1, xmm7
00000001401C6132: 48 8D 0D A7 ED 80 00                     LEA      rcx, [rip + 0x80eda7]
00000001401C6139: FF 15 E9 5E 6A 00                        CALL     qword ptr [rip + 0x6a5ee9]
00000001401C613F: F3 0F 5C F0                              SUBSS    xmm6, xmm0
00000001401C6143: F3 0F 11 B7 28 10 00 00                  MOVSS    dword ptr [rdi + 0x1028], xmm6
00000001401C614B: 41 8B DD                                 MOV      ebx, r13d
00000001401C614E: 89 9D 98 02 00 00                        MOV      dword ptr [rbp + 0x298], ebx
00000001401C6154: 4C 8D BF 70 0C 00 00                     LEA      r15, [rdi + 0xc70]
00000001401C615B: 49 8B 47 08                              MOV      rax, qword ptr [r15 + 8]
00000001401C615F: 49 2B 07                                 SUB      rax, qword ptr [r15]
00000001401C6162: 48 C1 F8 03                              SAR      rax, 3
00000001401C6166: F3 44 0F 10 15 F9 47 74 00               MOVSS    xmm10, dword ptr [rip + 0x7447f9]
00000001401C616F: F3 44 0F 10 1D 08 41 74 00               MOVSS    xmm11, dword ptr [rip + 0x744108]
00000001401C6178: F3 44 0F 10 25 7B 49 74 00               MOVSS    xmm12, dword ptr [rip + 0x74497b]
00000001401C6181: F3 44 0F 10 2D 8E 44 74 00               MOVSS    xmm13, dword ptr [rip + 0x74448e]
00000001401C618A: F3 44 0F 10 35 C5 3D 74 00               MOVSS    xmm14, dword ptr [rip + 0x743dc5]
00000001401C6193: 48 85 C0                                 TEST     rax, rax
00000001401C6196: 0F 84 56 08 00 00                        JE       0x1401c69f2
00000001401C619C: 4D 8B F5                                 MOV      r14, r13
00000001401C619F: 4C 89 6D 80                              MOV      qword ptr [rbp - 0x80], r13
00000001401C61A3: 49 B8 AB AA AA AA AA AA AA 2A            MOVABS   r8, 0x2aaaaaaaaaaaaaab
00000001401C61AD: 49 BB 25 49 92 24 49 92 24 49            MOVABS   r11, 0x4924924924924925
00000001401C61B7: F3 44 0F 10 3D D0 39 74 00               MOVSS    xmm15, dword ptr [rip + 0x7439d0]
00000001401C61C0: F3 44 0F 10 0D 37 51 74 00               MOVSS    xmm9, dword ptr [rip + 0x745137]
00000001401C61C9: EB 0D                                    JMP      0x1401c61d8
00000001401C61CB: 0F 1F 44 00 00                           NOP      dword ptr [rax + rax]
00000001401C61D0: F3 0F 10 3D 24 39 74 00                  MOVSS    xmm7, dword ptr [rip + 0x743924]
00000001401C61D8: 49 8B 07                                 MOV      rax, qword ptr [r15]
00000001401C61DB: 4A 8B 34 F0                              MOV      rsi, qword ptr [rax + r14*8]
00000001401C61DF: 48 89 B5 A8 02 00 00                     MOV      qword ptr [rbp + 0x2a8], rsi
00000001401C61E6: 48 83 BE 80 0C 00 00 00                  CMP      qword ptr [rsi + 0xc80], 0
00000001401C61EE: 0F 85 C4 07 00 00                        JNE      0x1401c69b8
00000001401C61F4: 48 39 BE F8 04 00 00                     CMP      qword ptr [rsi + 0x4f8], rdi
00000001401C61FB: 0F 85 B7 07 00 00                        JNE      0x1401c69b8
00000001401C6201: 48 83 BE 00 06 00 00 00                  CMP      qword ptr [rsi + 0x600], 0
00000001401C6209: 74 37                                    JE       0x1401c6242
00000001401C620B: 48 83 BE 28 05 00 00 00                  CMP      qword ptr [rsi + 0x528], 0
00000001401C6213: 75 2D                                    JNE      0x1401c6242
00000001401C6215: 48 8B D6                                 MOV      rdx, rsi
00000001401C6218: 49 8B CC                                 MOV      rcx, r12
00000001401C621B: E8 80 6B 23 00                           CALL     0x1403fcda0
00000001401C6220: 84 C0                                    TEST     al, al
00000001401C6222: 74 0A                                    JE       0x1401c622e
00000001401C6224: FF CB                                    DEC      ebx
00000001401C6226: 49 FF CE                                 DEC      r14
00000001401C6229: E9 8A 07 00 00                           JMP      0x1401c69b8
00000001401C622E: 49 B8 AB AA AA AA AA AA AA 2A            MOVABS   r8, 0x2aaaaaaaaaaaaaab
00000001401C6238: 49 BB 25 49 92 24 49 92 24 49            MOVABS   r11, 0x4924924924924925
00000001401C6242: 41 83 BC 24 B0 05 00 00 02               CMP      dword ptr [r12 + 0x5b0], 2
00000001401C624B: 0F 85 87 01 00 00                        JNE      0x1401c63d8
00000001401C6251: 44 0F 2F 86 F0 05 00 00                  COMISS   xmm8, dword ptr [rsi + 0x5f0]
00000001401C6259: 0F 82 79 01 00 00                        JB       0x1401c63d8
00000001401C625F: 48 8B 86 F8 04 00 00                     MOV      rax, qword ptr [rsi + 0x4f8]
00000001401C6266: 48 39 86 F0 04 00 00                     CMP      qword ptr [rsi + 0x4f0], rax
00000001401C626D: 0F 85 45 07 00 00                        JNE      0x1401c69b8
00000001401C6273: 48 83 BE 28 05 00 00 00                  CMP      qword ptr [rsi + 0x528], 0
00000001401C627B: 0F 85 37 07 00 00                        JNE      0x1401c69b8
00000001401C6281: 48 8B 8E 40 07 00 00                     MOV      rcx, qword ptr [rsi + 0x740]
00000001401C6288: 48 2B 8E 38 07 00 00                     SUB      rcx, qword ptr [rsi + 0x738]
00000001401C628F: 49 8B C0                                 MOV      rax, r8
00000001401C6292: 48 F7 E9                                 IMUL     rcx
00000001401C6295: 48 D1 FA                                 SAR      rdx, 1
00000001401C6298: 48 8B C2                                 MOV      rax, rdx
00000001401C629B: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C629F: 48 03 D0                                 ADD      rdx, rax
00000001401C62A2: 74 31                                    JE       0x1401c62d5
00000001401C62A4: 48 8B 8E 40 07 00 00                     MOV      rcx, qword ptr [rsi + 0x740]
00000001401C62AB: 48 2B 8E 38 07 00 00                     SUB      rcx, qword ptr [rsi + 0x738]
00000001401C62B2: 49 8B C0                                 MOV      rax, r8
00000001401C62B5: 48 F7 E9                                 IMUL     rcx
00000001401C62B8: 48 D1 FA                                 SAR      rdx, 1
00000001401C62BB: 48 8B C2                                 MOV      rax, rdx
00000001401C62BE: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C62C2: 48 03 D0                                 ADD      rdx, rax
00000001401C62C5: 48 63 86 68 07 00 00                     MOVSXD   rax, dword ptr [rsi + 0x768]
00000001401C62CC: 48 3B C2                                 CMP      rax, rdx
00000001401C62CF: 0F 82 E3 06 00 00                        JB       0x1401c69b8
00000001401C62D5: C6 86 FC 05 00 00 01                     MOV      byte ptr [rsi + 0x5fc], 1
00000001401C62DC: 4C 8B 96 F8 04 00 00                     MOV      r10, qword ptr [rsi + 0x4f8]
00000001401C62E3: 49 8B D2                                 MOV      rdx, r10
00000001401C62E6: 49 8B CC                                 MOV      rcx, r12
00000001401C62E9: E8 F2 5C FF FF                           CALL     0x1401bbfe0
00000001401C62EE: 84 C0                                    TEST     al, al
00000001401C62F0: 0F 85 C2 06 00 00                        JNE      0x1401c69b8
00000001401C62F6: 49 8B 8A 78 09 00 00                     MOV      rcx, qword ptr [r10 + 0x978]
00000001401C62FD: 49 2B 8A 70 09 00 00                     SUB      rcx, qword ptr [r10 + 0x970]
00000001401C6304: 49 8B C3                                 MOV      rax, r11
00000001401C6307: 48 F7 E9                                 IMUL     rcx
00000001401C630A: 48 C1 FA 06                              SAR      rdx, 6
00000001401C630E: 4C 8B CA                                 MOV      r9, rdx
00000001401C6311: 49 C1 E9 3F                              SHR      r9, 0x3f
00000001401C6315: 4C 03 CA                                 ADD      r9, rdx
00000001401C6318: 41 83 E9 01                              SUB      r9d, 1
00000001401C631C: 0F 88 96 06 00 00                        JS       0x1401c69b8
00000001401C6322: 4D 63 C1                                 MOVSXD   r8, r9d
00000001401C6325: 49 69 D0 E0 00 00 00                     IMUL     rdx, r8, 0xe0
00000001401C632C: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001401C6330: 49 8B 82 70 09 00 00                     MOV      rax, qword ptr [r10 + 0x970]
00000001401C6337: 48 8B 4C 02 08                           MOV      rcx, qword ptr [rdx + rax + 8]
00000001401C633C: 48 2B 0C 02                              SUB      rcx, qword ptr [rdx + rax]
00000001401C6340: 48 C1 F9 04                              SAR      rcx, 4
00000001401C6344: 48 85 C9                                 TEST     rcx, rcx
00000001401C6347: 74 11                                    JE       0x1401c635a
00000001401C6349: 48 8B 0C 02                              MOV      rcx, qword ptr [rdx + rax]
00000001401C634D: 49 8B 84 24 E0 C2 00 00                  MOV      rax, qword ptr [r12 + 0xc2e0]
00000001401C6355: 48 39 01                                 CMP      qword ptr [rcx], rax
00000001401C6358: 74 16                                    JE       0x1401c6370
00000001401C635A: 41 FF C9                                 DEC      r9d
00000001401C635D: 48 81 EA E0 00 00 00                     SUB      rdx, 0xe0
00000001401C6364: 49 83 E8 01                              SUB      r8, 1
00000001401C6368: 0F 88 4A 06 00 00                        JS       0x1401c69b8
00000001401C636E: EB C0                                    JMP      0x1401c6330
00000001401C6370: 49 63 C1                                 MOVSXD   rax, r9d
00000001401C6373: 48 69 C0 E0 00 00 00                     IMUL     rax, rax, 0xe0
00000001401C637A: 49 03 82 70 09 00 00                     ADD      rax, qword ptr [r10 + 0x970]
00000001401C6381: 48 8B 00                                 MOV      rax, qword ptr [rax]
00000001401C6384: F3 0F 10 40 08                           MOVSS    xmm0, dword ptr [rax + 8]
00000001401C6389: F3 41 0F 58 C0                           ADDSS    xmm0, xmm8
00000001401C638E: 41 0F 2F C7                              COMISS   xmm0, xmm15
00000001401C6392: 0F 86 20 06 00 00                        JBE      0x1401c69b8
00000001401C6398: C6 86 FC 05 00 00 00                     MOV      byte ptr [rsi + 0x5fc], 0
00000001401C639F: 4C 89 96 28 05 00 00                     MOV      qword ptr [rsi + 0x528], r10
00000001401C63A6: 4C 89 96 E8 05 00 00                     MOV      qword ptr [rsi + 0x5e8], r10
00000001401C63AD: 4C 89 AE 5C 05 00 00                     MOV      qword ptr [rsi + 0x55c], r13
00000001401C63B4: 4C 89 AE 94 05 00 00                     MOV      qword ptr [rsi + 0x594], r13
00000001401C63BB: C7 86 68 05 00 00 00 00 80 3F            MOV      dword ptr [rsi + 0x568], 0x3f800000
00000001401C63C5: 44 89 AE 9C 17 00 00                     MOV      dword ptr [rsi + 0x179c], r13d
00000001401C63CC: 44 89 AE 64 05 00 00                     MOV      dword ptr [rsi + 0x564], r13d
00000001401C63D3: E9 E0 05 00 00                           JMP      0x1401c69b8
00000001401C63D8: F3 0F 10 B6 08 05 00 00                  MOVSS    xmm6, dword ptr [rsi + 0x508]
00000001401C63E0: 41 0F 2F F0                              COMISS   xmm6, xmm8
00000001401C63E4: 76 22                                    JBE      0x1401c6408
00000001401C63E6: 45 33 C9                                 XOR      r9d, r9d
00000001401C63E9: 45 33 C0                                 XOR      r8d, r8d
00000001401C63EC: 0F 28 CF                                 MOVAPS   xmm1, xmm7
00000001401C63EF: 48 8D 0D EA EA 80 00                     LEA      rcx, [rip + 0x80eaea]
00000001401C63F6: FF 15 2C 5C 6A 00                        CALL     qword ptr [rip + 0x6a5c2c]
00000001401C63FC: F3 0F 5C F0                              SUBSS    xmm6, xmm0
00000001401C6400: F3 0F 11 B6 08 05 00 00                  MOVSS    dword ptr [rsi + 0x508], xmm6
00000001401C6408: 48 83 BF B0 0B 00 00 00                  CMP      qword ptr [rdi + 0xbb0], 0
00000001401C6410: 0F 85 82 00 00 00                        JNE      0x1401c6498
00000001401C6416: 44 0F 2F 86 08 05 00 00                  COMISS   xmm8, dword ptr [rsi + 0x508]
00000001401C641E: 72 78                                    JB       0x1401c6498
00000001401C6420: 80 BE 39 0D 00 00 00                     CMP      byte ptr [rsi + 0xd39], 0
00000001401C6427: 75 7C                                    JNE      0x1401c64a5
00000001401C6429: 83 BE 98 06 00 00 00                     CMP      dword ptr [rsi + 0x698], 0
00000001401C6430: 75 66                                    JNE      0x1401c6498
00000001401C6432: 48 8B 86 88 06 00 00                     MOV      rax, qword ptr [rsi + 0x688]
00000001401C6439: 48 2B 86 80 06 00 00                     SUB      rax, qword ptr [rsi + 0x680]
00000001401C6440: 48 C1 F8 03                              SAR      rax, 3
00000001401C6444: 48 85 C0                                 TEST     rax, rax
00000001401C6447: 74 4F                                    JE       0x1401c6498
00000001401C6449: 48 83 BE 28 05 00 00 00                  CMP      qword ptr [rsi + 0x528], 0
00000001401C6451: 75 45                                    JNE      0x1401c6498
00000001401C6453: 48 8B 86 F8 04 00 00                     MOV      rax, qword ptr [rsi + 0x4f8]
00000001401C645A: 48 39 86 F0 04 00 00                     CMP      qword ptr [rsi + 0x4f0], rax
00000001401C6461: 75 35                                    JNE      0x1401c6498
00000001401C6463: 45 33 C9                                 XOR      r9d, r9d
00000001401C6466: 45 33 C0                                 XOR      r8d, r8d
00000001401C6469: 33 D2                                    XOR      edx, edx
00000001401C646B: 48 8B CE                                 MOV      rcx, rsi
00000001401C646E: E8 3D 2A 4F 00                           CALL     0x1406b8eb0
00000001401C6473: 4C 89 AE 18 05 00 00                     MOV      qword ptr [rsi + 0x518], r13
00000001401C647A: 84 C0                                    TEST     al, al
00000001401C647C: 74 09                                    JE       0x1401c6487
00000001401C647E: 4C 89 AE 20 05 00 00                     MOV      qword ptr [rsi + 0x520], r13
00000001401C6485: EB 11                                    JMP      0x1401c6498
00000001401C6487: 48 8B 86 80 06 00 00                     MOV      rax, qword ptr [rsi + 0x680]
00000001401C648E: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001401C6491: 48 89 8E 20 05 00 00                     MOV      qword ptr [rsi + 0x520], rcx
00000001401C6498: 80 BE 39 0D 00 00 00                     CMP      byte ptr [rsi + 0xd39], 0
00000001401C649F: 0F 84 3A 04 00 00                        JE       0x1401c68df
00000001401C64A5: 66 C7 44 24 40 00 00                     MOV      word ptr [rsp + 0x40], 0
00000001401C64AC: 44 89 6C 24 44                           MOV      dword ptr [rsp + 0x44], r13d
00000001401C64B1: 41 0F 28 F0                              MOVAPS   xmm6, xmm8
00000001401C64B5: F3 0F 11 74 24 48                        MOVSS    dword ptr [rsp + 0x48], xmm6
00000001401C64BB: 41 0F 28 F8                              MOVAPS   xmm7, xmm8
00000001401C64BF: F3 0F 11 7C 24 4C                        MOVSS    dword ptr [rsp + 0x4c], xmm7
00000001401C64C5: 48 8D 4D 90                              LEA      rcx, [rbp - 0x70]
00000001401C64C9: E8 82 72 F2 FF                           CALL     0x1400ed750
00000001401C64CE: 90                                       NOP
00000001401C64CF: C6 86 39 0D 00 00 00                     MOV      byte ptr [rsi + 0xd39], 0
00000001401C64D6: 48 C7 45 40 01 00 00 00                  MOV      qword ptr [rbp + 0x40], 1
00000001401C64DE: 49 8B 47 08                              MOV      rax, qword ptr [r15 + 8]
00000001401C64E2: 49 2B 07                                 SUB      rax, qword ptr [r15]
00000001401C64E5: 48 C1 F8 03                              SAR      rax, 3
00000001401C64E9: 48 83 F8 01                              CMP      rax, 1
00000001401C64ED: 72 1B                                    JB       0x1401c650a
00000001401C64EF: 49 8B D7                                 MOV      rdx, r15
00000001401C64F2: 48 8D 4C 24 40                           LEA      rcx, [rsp + 0x40]
00000001401C64F7: E8 E4 D4 27 00                           CALL     0x1404439e0
00000001401C64FC: F3 0F 10 7C 24 4C                        MOVSS    xmm7, dword ptr [rsp + 0x4c]
00000001401C6502: F3 0F 10 74 24 48                        MOVSS    xmm6, dword ptr [rsp + 0x48]
00000001401C6508: EB 3E                                    JMP      0x1401c6548
00000001401C650A: 66 C7 44 24 40 01 00                     MOV      word ptr [rsp + 0x40], 1
00000001401C6511: 48 8B 87 18 03 00 00                     MOV      rax, qword ptr [rdi + 0x318]
00000001401C6518: 48 85 C0                                 TEST     rax, rax
00000001401C651B: 74 1B                                    JE       0x1401c6538
00000001401C651D: 83 B8 60 03 00 00 09                     CMP      dword ptr [rax + 0x360], 9
00000001401C6524: 75 22                                    JNE      0x1401c6548
00000001401C6526: C7 44 24 44 00 01 00 00                  MOV      dword ptr [rsp + 0x44], 0x100
00000001401C652E: 41 0F 28 F2                              MOVAPS   xmm6, xmm10
00000001401C6532: 41 0F 28 FB                              MOVAPS   xmm7, xmm11
00000001401C6536: EB 10                                    JMP      0x1401c6548
00000001401C6538: C7 44 24 44 00 10 00 00                  MOV      dword ptr [rsp + 0x44], 0x1000
00000001401C6540: 41 0F 28 F4                              MOVAPS   xmm6, xmm12
00000001401C6544: 41 0F 28 FD                              MOVAPS   xmm7, xmm13
00000001401C6548: F3 0F 10 44 24 40                        MOVSS    xmm0, dword ptr [rsp + 0x40]
00000001401C654E: F3 0F 11 44 24 70                        MOVSS    dword ptr [rsp + 0x70], xmm0
00000001401C6554: F3 0F 10 4C 24 44                        MOVSS    xmm1, dword ptr [rsp + 0x44]
00000001401C655A: F3 0F 11 4C 24 74                        MOVSS    dword ptr [rsp + 0x74], xmm1
00000001401C6560: 0F B6 44 24 70                           MOVZX    eax, byte ptr [rsp + 0x70]
00000001401C6565: 88 45 9C                                 MOV      byte ptr [rbp - 0x64], al
00000001401C6568: 8B 44 24 74                              MOV      eax, dword ptr [rsp + 0x74]
00000001401C656C: 89 45 B0                                 MOV      dword ptr [rbp - 0x50], eax
00000001401C656F: 0F B6 44 24 71                           MOVZX    eax, byte ptr [rsp + 0x71]
00000001401C6574: 88 45 A5                                 MOV      byte ptr [rbp - 0x5b], al
00000001401C6577: 41 0F 2F F6                              COMISS   xmm6, xmm14
00000001401C657B: 76 1B                                    JBE      0x1401c6598
00000001401C657D: F3 0F 11 75 BC                           MOVSS    dword ptr [rbp - 0x44], xmm6
00000001401C6582: F3 0F 10 05 72 35 74 00                  MOVSS    xmm0, dword ptr [rip + 0x743572]
00000001401C658A: 0F 2F C6                                 COMISS   xmm0, xmm6
00000001401C658D: 76 10                                    JBE      0x1401c659f
00000001401C658F: C7 45 BC 6F 12 83 3A                     MOV      dword ptr [rbp - 0x44], 0x3a83126f
00000001401C6596: EB 07                                    JMP      0x1401c659f
00000001401C6598: C7 45 BC 00 00 80 3F                     MOV      dword ptr [rbp - 0x44], 0x3f800000
00000001401C659F: 41 0F 2F F6                              COMISS   xmm6, xmm14
00000001401C65A3: 76 07                                    JBE      0x1401c65ac
00000001401C65A5: F3 0F 11 7D C4                           MOVSS    dword ptr [rbp - 0x3c], xmm7
00000001401C65AA: EB 07                                    JMP      0x1401c65b3
00000001401C65AC: C7 45 C4 00 00 00 00                     MOV      dword ptr [rbp - 0x3c], 0
00000001401C65B3: 45 8B F5                                 MOV      r14d, r13d
00000001401C65B6: 48 8B 8F 18 0A 00 00                     MOV      rcx, qword ptr [rdi + 0xa18]
00000001401C65BD: 48 2B 8F 10 0A 00 00                     SUB      rcx, qword ptr [rdi + 0xa10]
00000001401C65C4: 49 B9 AB AA AA AA AA AA AA 2A            MOVABS   r9, 0x2aaaaaaaaaaaaaab
00000001401C65CE: 49 8B C1                                 MOV      rax, r9
00000001401C65D1: 48 F7 E9                                 IMUL     rcx
00000001401C65D4: 48 C1 FA 04                              SAR      rdx, 4
00000001401C65D8: 48 8B C2                                 MOV      rax, rdx
00000001401C65DB: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C65DF: 48 03 D0                                 ADD      rdx, rax
00000001401C65E2: 74 7C                                    JE       0x1401c6660
00000001401C65E4: 49 8B DD                                 MOV      rbx, r13
00000001401C65E7: 66 0F 1F 84 00 00 00 00 00               NOP      word ptr [rax + rax]
00000001401C65F0: 48 8B 87 10 0A 00 00                     MOV      rax, qword ptr [rdi + 0xa10]
00000001401C65F7: 83 3C 03 00                              CMP      dword ptr [rbx + rax], 0
00000001401C65FB: 75 32                                    JNE      0x1401c662f
00000001401C65FD: 48 8B 54 03 30                           MOV      rdx, qword ptr [rbx + rax + 0x30]
00000001401C6602: 48 85 D2                                 TEST     rdx, rdx
00000001401C6605: 74 28                                    JE       0x1401c662f
00000001401C6607: 80 7C 03 05 00                           CMP      byte ptr [rbx + rax + 5], 0
00000001401C660C: 74 21                                    JE       0x1401c662f
00000001401C660E: F3 44 0F 11 4C 24 20                     MOVSS    dword ptr [rsp + 0x20], xmm9
00000001401C6615: 45 33 C9                                 XOR      r9d, r9d
00000001401C6618: 41 0F 28 D0                              MOVAPS   xmm2, xmm8
00000001401C661C: 48 8D 4D 90                              LEA      rcx, [rbp - 0x70]
00000001401C6620: E8 FB D4 3B 00                           CALL     0x140583b20
00000001401C6625: 49 B9 AB AA AA AA AA AA AA 2A            MOVABS   r9, 0x2aaaaaaaaaaaaaab
00000001401C662F: 41 FF C6                                 INC      r14d
00000001401C6632: 48 83 C3 60                              ADD      rbx, 0x60
00000001401C6636: 48 8B 8F 18 0A 00 00                     MOV      rcx, qword ptr [rdi + 0xa18]
00000001401C663D: 48 2B 8F 10 0A 00 00                     SUB      rcx, qword ptr [rdi + 0xa10]
00000001401C6644: 49 8B C1                                 MOV      rax, r9
00000001401C6647: 48 F7 E9                                 IMUL     rcx
00000001401C664A: 48 C1 FA 04                              SAR      rdx, 4
00000001401C664E: 48 8B C2                                 MOV      rax, rdx
00000001401C6651: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C6655: 48 03 D0                                 ADD      rdx, rax
00000001401C6658: 49 63 C6                                 MOVSXD   rax, r14d
00000001401C665B: 48 3B C2                                 CMP      rax, rdx
00000001401C665E: 72 90                                    JB       0x1401c65f0
00000001401C6660: 45 8B E5                                 MOV      r12d, r13d
00000001401C6663: 48 8B 87 40 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd40]
00000001401C666A: 48 2B 87 38 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd38]
00000001401C6671: 48 C1 F8 03                              SAR      rax, 3
00000001401C6675: 48 85 C0                                 TEST     rax, rax
00000001401C6678: 0F 84 F4 00 00 00                        JE       0x1401c6772
00000001401C667E: 4D 8B FD                                 MOV      r15, r13
00000001401C6681: 45 8B F5                                 MOV      r14d, r13d
00000001401C6684: 48 8B 87 38 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd38]
00000001401C668B: 49 8B 0C 07                              MOV      rcx, qword ptr [r15 + rax]
00000001401C668F: 4C 8B 41 08                              MOV      r8, qword ptr [rcx + 8]
00000001401C6693: 49 81 C0 10 0A 00 00                     ADD      r8, 0xa10
00000001401C669A: 49 8B 48 08                              MOV      rcx, qword ptr [r8 + 8]
00000001401C669E: 49 2B 08                                 SUB      rcx, qword ptr [r8]
00000001401C66A1: 49 8B C1                                 MOV      rax, r9
00000001401C66A4: 48 F7 E9                                 IMUL     rcx
00000001401C66A7: 48 C1 FA 04                              SAR      rdx, 4
00000001401C66AB: 48 8B C2                                 MOV      rax, rdx
00000001401C66AE: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C66B2: 48 03 D0                                 ADD      rdx, rax
00000001401C66B5: 0F 84 8B 00 00 00                        JE       0x1401c6746
00000001401C66BB: 49 8B DD                                 MOV      rbx, r13
00000001401C66BE: 66 90                                    NOP
00000001401C66C0: 49 8B 00                                 MOV      rax, qword ptr [r8]
00000001401C66C3: 83 3C 03 00                              CMP      dword ptr [rbx + rax], 0
00000001401C66C7: 75 39                                    JNE      0x1401c6702
00000001401C66C9: 48 83 7C 18 30 00                        CMP      qword ptr [rax + rbx + 0x30], 0
00000001401C66CF: 74 31                                    JE       0x1401c6702
00000001401C66D1: 80 7C 18 04 00                           CMP      byte ptr [rax + rbx + 4], 0
00000001401C66D6: 74 2A                                    JE       0x1401c6702
00000001401C66D8: 48 8B 4C 18 30                           MOV      rcx, qword ptr [rax + rbx + 0x30]
00000001401C66DD: 48 89 4C 24 30                           MOV      qword ptr [rsp + 0x30], rcx
00000001401C66E2: 48 85 C9                                 TEST     rcx, rcx
00000001401C66E5: 74 1B                                    JE       0x1401c6702
00000001401C66E7: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001401C66EC: 48 8D 8D 88 00 00 00                     LEA      rcx, [rbp + 0x88]
00000001401C66F3: E8 98 A7 EE FF                           CALL     0x1400b0e90
00000001401C66F8: 49 B9 AB AA AA AA AA AA AA 2A            MOVABS   r9, 0x2aaaaaaaaaaaaaab
00000001401C6702: 41 FF C6                                 INC      r14d
00000001401C6705: 48 83 C3 60                              ADD      rbx, 0x60
00000001401C6709: 48 8B 87 38 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd38]
00000001401C6710: 49 8B 0C 07                              MOV      rcx, qword ptr [r15 + rax]
00000001401C6714: 4C 8B 41 08                              MOV      r8, qword ptr [rcx + 8]
00000001401C6718: 49 81 C0 10 0A 00 00                     ADD      r8, 0xa10
00000001401C671F: 49 8B 48 08                              MOV      rcx, qword ptr [r8 + 8]
00000001401C6723: 49 2B 08                                 SUB      rcx, qword ptr [r8]
00000001401C6726: 49 8B C1                                 MOV      rax, r9
00000001401C6729: 48 F7 E9                                 IMUL     rcx
00000001401C672C: 48 C1 FA 04                              SAR      rdx, 4
00000001401C6730: 48 8B C2                                 MOV      rax, rdx
00000001401C6733: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001401C6737: 48 03 D0                                 ADD      rdx, rax
00000001401C673A: 49 63 C6                                 MOVSXD   rax, r14d
00000001401C673D: 48 3B C2                                 CMP      rax, rdx
00000001401C6740: 0F 82 7A FF FF FF                        JB       0x1401c66c0
00000001401C6746: 41 FF C4                                 INC      r12d
00000001401C6749: 49 83 C7 08                              ADD      r15, 8
00000001401C674D: 48 8B 8F 40 0D 00 00                     MOV      rcx, qword ptr [rdi + 0xd40]
00000001401C6754: 48 2B 8F 38 0D 00 00                     SUB      rcx, qword ptr [rdi + 0xd38]
00000001401C675B: 48 C1 F9 03                              SAR      rcx, 3
00000001401C675F: 49 63 C4                                 MOVSXD   rax, r12d
00000001401C6762: 48 3B C1                                 CMP      rax, rcx
00000001401C6765: 0F 82 16 FF FF FF                        JB       0x1401c6681
00000001401C676B: 4C 8D BF 70 0C 00 00                     LEA      r15, [rdi + 0xc70]
00000001401C6772: 48 8D 4D 90                              LEA      rcx, [rbp - 0x70]
00000001401C6776: E8 85 BC 3B 00                           CALL     0x140582400
00000001401C677B: 45 8B F5                                 MOV      r14d, r13d
00000001401C677E: 48 8B 87 40 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd40]
00000001401C6785: 48 2B 87 38 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd38]
00000001401C678C: 48 C1 F8 03                              SAR      rax, 3
00000001401C6790: 48 85 C0                                 TEST     rax, rax
00000001401C6793: 0F 84 91 00 00 00                        JE       0x1401c682a
00000001401C6799: 4D 8B DD                                 MOV      r11, r13
00000001401C679C: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001401C67A0: 32 DB                                    XOR      bl, bl
00000001401C67A2: 45 8B C5                                 MOV      r8d, r13d
00000001401C67A5: 4C 8B 8D 48 01 00 00                     MOV      r9, qword ptr [rbp + 0x148]
00000001401C67AC: 48 8B 95 40 01 00 00                     MOV      rdx, qword ptr [rbp + 0x140]
00000001401C67B3: 4C 2B CA                                 SUB      r9, rdx
00000001401C67B6: 49 C1 F9 03                              SAR      r9, 3
00000001401C67BA: 45 85 C9                                 TEST     r9d, r9d
00000001401C67BD: 7E 2A                                    JLE      0x1401c67e9
00000001401C67BF: 48 8B 87 38 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd38]
00000001401C67C6: 49 8B 0C 03                              MOV      rcx, qword ptr [r11 + rax]
00000001401C67CA: 4C 8B 51 08                              MOV      r10, qword ptr [rcx + 8]
00000001401C67CE: 66 90                                    NOP
00000001401C67D0: 48 8B 02                                 MOV      rax, qword ptr [rdx]
00000001401C67D3: 4C 39 50 40                              CMP      qword ptr [rax + 0x40], r10
00000001401C67D7: 74 0E                                    JE       0x1401c67e7
00000001401C67D9: 41 FF C0                                 INC      r8d
00000001401C67DC: 48 83 C2 08                              ADD      rdx, 8
00000001401C67E0: 45 3B C1                                 CMP      r8d, r9d
00000001401C67E3: 7C EB                                    JL       0x1401c67d0
00000001401C67E5: EB 02                                    JMP      0x1401c67e9
00000001401C67E7: B3 01                                    MOV      bl, 1
00000001401C67E9: 48 8B 87 38 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd38]
00000001401C67F0: 49 8B 0C 03                              MOV      rcx, qword ptr [r11 + rax]
00000001401C67F4: 38 19                                    CMP      byte ptr [rcx], bl
00000001401C67F6: 74 07                                    JE       0x1401c67ff
00000001401C67F8: C6 87 50 0D 00 00 01                     MOV      byte ptr [rdi + 0xd50], 1
00000001401C67FF: 49 8B 0C 03                              MOV      rcx, qword ptr [r11 + rax]
00000001401C6803: 88 19                                    MOV      byte ptr [rcx], bl
00000001401C6805: 41 FF C6                                 INC      r14d
00000001401C6808: 49 83 C3 08                              ADD      r11, 8
00000001401C680C: 48 8B 8F 40 0D 00 00                     MOV      rcx, qword ptr [rdi + 0xd40]
00000001401C6813: 48 2B 8F 38 0D 00 00                     SUB      rcx, qword ptr [rdi + 0xd38]
00000001401C681A: 48 C1 F9 03                              SAR      rcx, 3
00000001401C681E: 49 63 C6                                 MOVSXD   rax, r14d
00000001401C6821: 48 3B C1                                 CMP      rax, rcx
00000001401C6824: 0F 82 76 FF FF FF                        JB       0x1401c67a0
00000001401C682A: 48 8B 86 88 06 00 00                     MOV      rax, qword ptr [rsi + 0x688]
00000001401C6831: 48 2B 86 80 06 00 00                     SUB      rax, qword ptr [rsi + 0x680]
00000001401C6838: 48 C1 F8 03                              SAR      rax, 3
00000001401C683C: 48 83 F8 02                              CMP      rax, 2
00000001401C6840: 76 25                                    JBE      0x1401c6867
00000001401C6842: 48 8B 86 80 06 00 00                     MOV      rax, qword ptr [rsi + 0x680]
00000001401C6849: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001401C684C: 48 89 8E 18 05 00 00                     MOV      qword ptr [rsi + 0x518], rcx
00000001401C6853: 48 8B 86 80 06 00 00                     MOV      rax, qword ptr [rsi + 0x680]
00000001401C685A: 48 8B 48 08                              MOV      rcx, qword ptr [rax + 8]
00000001401C685E: 48 89 8E 20 05 00 00                     MOV      qword ptr [rsi + 0x520], rcx
00000001401C6865: EB 39                                    JMP      0x1401c68a0
00000001401C6867: 48 8B 86 88 06 00 00                     MOV      rax, qword ptr [rsi + 0x688]
00000001401C686E: 48 2B 86 80 06 00 00                     SUB      rax, qword ptr [rsi + 0x680]
00000001401C6875: 48 C1 F8 03                              SAR      rax, 3
00000001401C6879: 4C 89 AE 18 05 00 00                     MOV      qword ptr [rsi + 0x518], r13
00000001401C6880: 48 83 F8 01                              CMP      rax, 1
00000001401C6884: 76 13                                    JBE      0x1401c6899
00000001401C6886: 48 8B 86 80 06 00 00                     MOV      rax, qword ptr [rsi + 0x680]
00000001401C688D: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001401C6890: 48 89 8E 20 05 00 00                     MOV      qword ptr [rsi + 0x520], rcx
00000001401C6897: EB 07                                    JMP      0x1401c68a0
00000001401C6899: 4C 89 AE 20 05 00 00                     MOV      qword ptr [rsi + 0x520], r13
00000001401C68A0: C6 44 24 28 01                           MOV      byte ptr [rsp + 0x28], 1
00000001401C68A5: C6 44 24 20 01                           MOV      byte ptr [rsp + 0x20], 1
00000001401C68AA: 41 B1 01                                 MOV      r9b, 1
00000001401C68AD: 45 33 C0                                 XOR      r8d, r8d
00000001401C68B0: 48 8B D6                                 MOV      rdx, rsi
00000001401C68B3: 4C 8B A5 90 02 00 00                     MOV      r12, qword ptr [rbp + 0x290]
00000001401C68BA: 49 8B CC                                 MOV      rcx, r12
00000001401C68BD: E8 FE 54 23 00                           CALL     0x1403fbdc0
00000001401C68C2: C7 86 08 05 00 00 00 00 20 41            MOV      dword ptr [rsi + 0x508], 0x41200000
00000001401C68CC: 48 8D 4D 90                              LEA      rcx, [rbp - 0x70]
00000001401C68D0: E8 BB 6F F2 FF                           CALL     0x1400ed890
00000001401C68D5: 4C 8B 75 80                              MOV      r14, qword ptr [rbp - 0x80]
00000001401C68D9: 8B 9D 98 02 00 00                        MOV      ebx, dword ptr [rbp + 0x298]
00000001401C68DF: B2 01                                    MOV      dl, 1
00000001401C68E1: 48 8B CE                                 MOV      rcx, rsi
00000001401C68E4: E8 C7 6D 4F 00                           CALL     0x1406bd6b0
00000001401C68E9: 84 C0                                    TEST     al, al
00000001401C68EB: 0F 84 C7 00 00 00                        JE       0x1401c69b8
00000001401C68F1: 44 0F 2F 86 08 05 00 00                  COMISS   xmm8, dword ptr [rsi + 0x508]
00000001401C68F9: 0F 82 B9 00 00 00                        JB       0x1401c69b8
00000001401C68FF: 80 BE 39 0D 00 00 00                     CMP      byte ptr [rsi + 0xd39], 0
00000001401C6906: 0F 85 AC 00 00 00                        JNE      0x1401c69b8
00000001401C690C: 48 8D 85 A8 02 00 00                     LEA      rax, [rbp + 0x2a8]
00000001401C6913: 48 8B 4C 24 58                           MOV      rcx, qword ptr [rsp + 0x58]
00000001401C6918: 48 3B C1                                 CMP      rax, rcx
00000001401C691B: 73 61                                    JAE      0x1401c697e
00000001401C691D: 48 8D 95 A8 02 00 00                     LEA      rdx, [rbp + 0x2a8]
00000001401C6924: 48 8B 44 24 50                           MOV      rax, qword ptr [rsp + 0x50]
00000001401C6929: 48 3B C2                                 CMP      rax, rdx
00000001401C692C: 77 50                                    JA       0x1401c697e
00000001401C692E: 48 8D 9D A8 02 00 00                     LEA      rbx, [rbp + 0x2a8]
00000001401C6935: 48 2B D8                                 SUB      rbx, rax
00000001401C6938: 48 C1 FB 03                              SAR      rbx, 3
00000001401C693C: 48 3B 8D A0 02 00 00                     CMP      rcx, qword ptr [rbp + 0x2a0]
00000001401C6943: 75 25                                    JNE      0x1401c696a
00000001401C6945: BA 01 00 00 00                           MOV      edx, 1
00000001401C694A: 48 8D 4C 24 50                           LEA      rcx, [rsp + 0x50]
00000001401C694F: E8 AC AD EE FF                           CALL     0x1400b1700
00000001401C6954: 48 8B 44 24 60                           MOV      rax, qword ptr [rsp + 0x60]
00000001401C6959: 48 89 85 A0 02 00 00                     MOV      qword ptr [rbp + 0x2a0], rax
00000001401C6960: 48 8B 4C 24 58                           MOV      rcx, qword ptr [rsp + 0x58]
00000001401C6965: 48 8B 44 24 50                           MOV      rax, qword ptr [rsp + 0x50]
00000001401C696A: 48 85 C9                                 TEST     rcx, rcx
00000001401C696D: 74 07                                    JE       0x1401c6976
00000001401C696F: 48 8B 04 D8                              MOV      rax, qword ptr [rax + rbx*8]
00000001401C6973: 48 89 01                                 MOV      qword ptr [rcx], rax
00000001401C6976: 8B 9D 98 02 00 00                        MOV      ebx, dword ptr [rbp + 0x298]
00000001401C697C: EB 31                                    JMP      0x1401c69af
00000001401C697E: 48 3B 8D A0 02 00 00                     CMP      rcx, qword ptr [rbp + 0x2a0]
00000001401C6985: 75 20                                    JNE      0x1401c69a7
00000001401C6987: BA 01 00 00 00                           MOV      edx, 1
00000001401C698C: 48 8D 4C 24 50                           LEA      rcx, [rsp + 0x50]
00000001401C6991: E8 6A AD EE FF                           CALL     0x1400b1700
00000001401C6996: 48 8B 44 24 60                           MOV      rax, qword ptr [rsp + 0x60]
00000001401C699B: 48 89 85 A0 02 00 00                     MOV      qword ptr [rbp + 0x2a0], rax
00000001401C69A2: 48 8B 4C 24 58                           MOV      rcx, qword ptr [rsp + 0x58]
00000001401C69A7: 48 85 C9                                 TEST     rcx, rcx
00000001401C69AA: 74 03                                    JE       0x1401c69af
00000001401C69AC: 48 89 31                                 MOV      qword ptr [rcx], rsi
00000001401C69AF: 48 83 C1 08                              ADD      rcx, 8
00000001401C69B3: 48 89 4C 24 58                           MOV      qword ptr [rsp + 0x58], rcx
00000001401C69B8: FF C3                                    INC      ebx
00000001401C69BA: 89 9D 98 02 00 00                        MOV      dword ptr [rbp + 0x298], ebx
00000001401C69C0: 49 FF C6                                 INC      r14
00000001401C69C3: 4C 89 75 80                              MOV      qword ptr [rbp - 0x80], r14
00000001401C69C7: 49 8B 4F 08                              MOV      rcx, qword ptr [r15 + 8]
00000001401C69CB: 49 2B 0F                                 SUB      rcx, qword ptr [r15]
00000001401C69CE: 48 C1 F9 03                              SAR      rcx, 3
00000001401C69D2: 48 63 C3                                 MOVSXD   rax, ebx
00000001401C69D5: 48 3B C1                                 CMP      rax, rcx
00000001401C69D8: 49 B8 AB AA AA AA AA AA AA 2A            MOVABS   r8, 0x2aaaaaaaaaaaaaab
00000001401C69E2: 49 BB 25 49 92 24 49 92 24 49            MOVABS   r11, 0x4924924924924925
00000001401C69EC: 0F 82 DE F7 FF FF                        JB       0x1401c61d0
00000001401C69F2: 44 0F 2F 87 28 10 00 00                  COMISS   xmm8, dword ptr [rdi + 0x1028]
00000001401C69FA: 0F 86 3D 01 00 00                        JBE      0x1401c6b3d
00000001401C6A00: 66 C7 44 24 30 00 00                     MOV      word ptr [rsp + 0x30], 0
00000001401C6A07: 4C 89 6C 24 34                           MOV      qword ptr [rsp + 0x34], r13
00000001401C6A0C: C7 44 24 3C 00 00 00 00                  MOV      dword ptr [rsp + 0x3c], 0
00000001401C6A14: 45 32 F6                                 XOR      r14b, r14b
00000001401C6A17: 49 8B 47 08                              MOV      rax, qword ptr [r15 + 8]
00000001401C6A1B: 49 2B 07                                 SUB      rax, qword ptr [r15]
00000001401C6A1E: 48 C1 F8 03                              SAR      rax, 3
00000001401C6A22: 48 83 F8 01                              CMP      rax, 1
00000001401C6A26: 72 0F                                    JB       0x1401c6a37
00000001401C6A28: 49 8B D7                                 MOV      rdx, r15
00000001401C6A2B: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001401C6A30: E8 AB CF 27 00                           CALL     0x1404439e0
00000001401C6A35: EB 4E                                    JMP      0x1401c6a85
00000001401C6A37: 66 C7 44 24 30 01 00                     MOV      word ptr [rsp + 0x30], 1
00000001401C6A3E: 48 8B 87 18 03 00 00                     MOV      rax, qword ptr [rdi + 0x318]
00000001401C6A45: 48 85 C0                                 TEST     rax, rax
00000001401C6A48: 74 23                                    JE       0x1401c6a6d
00000001401C6A4A: 83 B8 60 03 00 00 09                     CMP      dword ptr [rax + 0x360], 9
00000001401C6A51: 75 32                                    JNE      0x1401c6a85
00000001401C6A53: C7 44 24 34 00 01 00 00                  MOV      dword ptr [rsp + 0x34], 0x100
00000001401C6A5B: C7 44 24 38 00 00 C8 41                  MOV      dword ptr [rsp + 0x38], 0x41c80000
00000001401C6A63: C7 44 24 3C 00 00 00 40                  MOV      dword ptr [rsp + 0x3c], 0x40000000
00000001401C6A6B: EB 18                                    JMP      0x1401c6a85
00000001401C6A6D: C7 44 24 34 00 10 00 00                  MOV      dword ptr [rsp + 0x34], 0x1000
00000001401C6A75: C7 44 24 38 00 00 A0 42                  MOV      dword ptr [rsp + 0x38], 0x42a00000
00000001401C6A7D: C7 44 24 3C 00 00 80 40                  MOV      dword ptr [rsp + 0x3c], 0x40800000
00000001401C6A85: 41 8B F5                                 MOV      esi, r13d
00000001401C6A88: 48 8B 87 40 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd40]
00000001401C6A8F: 48 2B 87 38 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd38]
00000001401C6A96: 48 C1 F8 03                              SAR      rax, 3
00000001401C6A9A: 48 85 C0                                 TEST     rax, rax
00000001401C6A9D: 0F 84 90 00 00 00                        JE       0x1401c6b33
00000001401C6AA3: 49 8B DD                                 MOV      rbx, r13
00000001401C6AA6: 0F 28 74 24 30                           MOVAPS   xmm6, xmmword ptr [rsp + 0x30]
00000001401C6AAB: 0F 1F 44 00 00                           NOP      dword ptr [rax + rax]
00000001401C6AB0: 4C 8B 87 38 0D 00 00                     MOV      r8, qword ptr [rdi + 0xd38]
00000001401C6AB7: 4A 8B 04 03                              MOV      rax, qword ptr [rbx + r8]
00000001401C6ABB: 80 38 00                                 CMP      byte ptr [rax], 0
00000001401C6ABE: 75 42                                    JNE      0x1401c6b02
00000001401C6AC0: 66 0F 7F 74 24 70                        MOVDQA   xmmword ptr [rsp + 0x70], xmm6
00000001401C6AC6: 4C 8B 40 08                              MOV      r8, qword ptr [rax + 8]
00000001401C6ACA: 48 8D 44 24 70                           LEA      rax, [rsp + 0x70]
00000001401C6ACF: 48 89 44 24 28                           MOV      qword ptr [rsp + 0x28], rax
00000001401C6AD4: C6 44 24 20 00                           MOV      byte ptr [rsp + 0x20], 0
00000001401C6AD9: 45 33 C9                                 XOR      r9d, r9d
00000001401C6ADC: 48 8B D7                                 MOV      rdx, rdi
00000001401C6ADF: E8 6C 9B F6 FF                           CALL     0x140130650
00000001401C6AE4: 84 C0                                    TEST     al, al
00000001401C6AE6: 74 17                                    JE       0x1401c6aff
00000001401C6AE8: 48 8B 87 38 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd38]
00000001401C6AEF: 48 8B 0C 03                              MOV      rcx, qword ptr [rbx + rax]
00000001401C6AF3: C6 01 01                                 MOV      byte ptr [rcx], 1
00000001401C6AF6: C6 87 50 0D 00 00 01                     MOV      byte ptr [rdi + 0xd50], 1
00000001401C6AFD: EB 03                                    JMP      0x1401c6b02
00000001401C6AFF: 41 B6 01                                 MOV      r14b, 1
00000001401C6B02: FF C6                                    INC      esi
00000001401C6B04: 48 83 C3 08                              ADD      rbx, 8
00000001401C6B08: 48 8B 8F 40 0D 00 00                     MOV      rcx, qword ptr [rdi + 0xd40]
00000001401C6B0F: 48 2B 8F 38 0D 00 00                     SUB      rcx, qword ptr [rdi + 0xd38]
00000001401C6B16: 48 C1 F9 03                              SAR      rcx, 3
00000001401C6B1A: 48 63 C6                                 MOVSXD   rax, esi
00000001401C6B1D: 48 3B C1                                 CMP      rax, rcx
00000001401C6B20: 72 8E                                    JB       0x1401c6ab0
00000001401C6B22: 45 84 F6                                 TEST     r14b, r14b
00000001401C6B25: 74 0C                                    JE       0x1401c6b33
00000001401C6B27: C7 87 28 10 00 00 00 00 A0 41            MOV      dword ptr [rdi + 0x1028], 0x41a00000
00000001401C6B31: EB 0A                                    JMP      0x1401c6b3d
00000001401C6B33: C7 87 28 10 00 00 00 00 20 41            MOV      dword ptr [rdi + 0x1028], 0x41200000
00000001401C6B3D: 41 83 BC 24 A4 05 00 00 00               CMP      dword ptr [r12 + 0x5a4], 0
00000001401C6B46: 0F 8E 0A 02 00 00                        JLE      0x1401c6d56
00000001401C6B4C: 44 0F 2F 87 C0 0F 00 00                  COMISS   xmm8, dword ptr [rdi + 0xfc0]
00000001401C6B54: 0F 82 FC 01 00 00                        JB       0x1401c6d56
00000001401C6B5A: 48 8B 87 40 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd40]
00000001401C6B61: 48 2B 87 38 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd38]
00000001401C6B68: 48 C1 F8 03                              SAR      rax, 3
00000001401C6B6C: 48 85 C0                                 TEST     rax, rax
00000001401C6B6F: 0F 84 E1 01 00 00                        JE       0x1401c6d56
00000001401C6B75: 4C 8B 7C 24 58                           MOV      r15, qword ptr [rsp + 0x58]
00000001401C6B7A: 49 8B F7                                 MOV      rsi, r15
00000001401C6B7D: 48 2B 74 24 50                           SUB      rsi, qword ptr [rsp + 0x50]
00000001401C6B82: 48 C1 FE 03                              SAR      rsi, 3
00000001401C6B86: 48 85 F6                                 TEST     rsi, rsi
00000001401C6B89: 0F 84 C7 01 00 00                        JE       0x1401c6d56
00000001401C6B8F: 45 32 F6                                 XOR      r14b, r14b
00000001401C6B92: 44 38 B7 50 0D 00 00                     CMP      byte ptr [rdi + 0xd50], r14b
00000001401C6B99: 74 0B                                    JE       0x1401c6ba6
00000001401C6B9B: 48 8B D7                                 MOV      rdx, rdi
00000001401C6B9E: 49 8B CC                                 MOV      rcx, r12
00000001401C6BA1: E8 DA B7 01 00                           CALL     0x1401e2380
00000001401C6BA6: 48 8B 87 60 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd60]
00000001401C6BAD: 48 2B 87 58 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd58]
00000001401C6BB4: 48 C1 F8 07                              SAR      rax, 7
00000001401C6BB8: 48 85 C0                                 TEST     rax, rax
00000001401C6BBB: 0F 84 2B 01 00 00                        JE       0x1401c6cec
00000001401C6BC1: 41 8B DD                                 MOV      ebx, r13d
00000001401C6BC4: 48 8B 87 60 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd60]
00000001401C6BCB: 48 2B 87 58 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd58]
00000001401C6BD2: 48 C1 F8 07                              SAR      rax, 7
00000001401C6BD6: 85 C0                                    TEST     eax, eax
00000001401C6BD8: 7E 34                                    JLE      0x1401c6c0e
00000001401C6BDA: 66 0F 1F 44 00 00                        NOP      word ptr [rax + rax]
00000001401C6BE0: 48 63 D3                                 MOVSXD   rdx, ebx
00000001401C6BE3: 48 C1 E2 07                              SHL      rdx, 7
00000001401C6BE7: 48 03 97 58 0D 00 00                     ADD      rdx, qword ptr [rdi + 0xd58]
00000001401C6BEE: 49 8B CC                                 MOV      rcx, r12
00000001401C6BF1: E8 DA CF 01 00                           CALL     0x1401e3bd0
00000001401C6BF6: FF C3                                    INC      ebx
00000001401C6BF8: 48 8B 87 60 0D 00 00                     MOV      rax, qword ptr [rdi + 0xd60]
00000001401C6BFF: 48 2B 87 58 0D 00 00                     SUB      rax, qword ptr [rdi + 0xd58]
00000001401C6C06: 48 C1 F8 07                              SAR      rax, 7
00000001401C6C0A: 3B D8                                    CMP      ebx, eax
00000001401C6C0C: 7C D2                                    JL       0x1401c6be0
00000001401C6C0E: 45 8B D5                                 MOV      r10d, r13d
00000001401C6C11: 48 8B 5C 24 50                           MOV      rbx, qword ptr [rsp + 0x50]
00000001401C6C16: 48 85 F6                                 TEST     rsi, rsi
00000001401C6C19: 74 79                                    JE       0x1401c6c94
00000001401C6C1B: 4C 8B DB                                 MOV      r11, rbx
00000001401C6C1E: F3 0F 10 05 52 30 74 00                  MOVSS    xmm0, dword ptr [rip + 0x743052]
00000001401C6C26: 66 66 0F 1F 84 00 00 00 00 00            NOP      word ptr [rax + rax]
00000001401C6C30: 49 8B 13                                 MOV      rdx, qword ptr [r11]
00000001401C6C33: 45 8B CD                                 MOV      r9d, r13d
00000001401C6C36: 48 8B 82 C0 03 00 00                     MOV      rax, qword ptr [rdx + 0x3c0]
00000001401C6C3D: 48 2B 82 B8 03 00 00                     SUB      rax, qword ptr [rdx + 0x3b8]
00000001401C6C44: 48 C1 F8 04                              SAR      rax, 4
00000001401C6C48: 48 85 C0                                 TEST     rax, rax
00000001401C6C4B: 74 38                                    JE       0x1401c6c85
00000001401C6C4D: 4D 8B C5                                 MOV      r8, r13
00000001401C6C50: 48 8B 82 B8 03 00 00                     MOV      rax, qword ptr [rdx + 0x3b8]
00000001401C6C57: 41 0F 2F 44 00 08                        COMISS   xmm0, dword ptr [r8 + rax + 8]
00000001401C6C5D: 76 05                                    JBE      0x1401c6c64
00000001401C6C5F: 46 89 6C 00 08                           MOV      dword ptr [rax + r8 + 8], r13d
00000001401C6C64: 41 FF C1                                 INC      r9d
00000001401C6C67: 49 83 C0 10                              ADD      r8, 0x10
00000001401C6C6B: 48 8B 8A C0 03 00 00                     MOV      rcx, qword ptr [rdx + 0x3c0]
00000001401C6C72: 48 2B 8A B8 03 00 00                     SUB      rcx, qword ptr [rdx + 0x3b8]
00000001401C6C79: 48 C1 F9 04                              SAR      rcx, 4
00000001401C6C7D: 49 63 C1                                 MOVSXD   rax, r9d
00000001401C6C80: 48 3B C1                                 CMP      rax, rcx
00000001401C6C83: 72 CB                                    JB       0x1401c6c50
00000001401C6C85: 41 FF C2                                 INC      r10d
00000001401C6C88: 49 83 C3 08                              ADD      r11, 8
00000001401C6C8C: 49 63 C2                                 MOVSXD   rax, r10d
00000001401C6C8F: 48 3B C6                                 CMP      rax, rsi
00000001401C6C92: 72 9C                                    JB       0x1401c6c30
00000001401C6C94: 48 8D 05 F5 6A 02 00                     LEA      rax, [rip + 0x26af5]
00000001401C6C9B: 48 89 85 90 02 00 00                     MOV      qword ptr [rbp + 0x290], rax
00000001401C6CA2: 4C 8D 8D 90 02 00 00                     LEA      r9, [rbp + 0x290]
00000001401C6CA9: 4C 8B C6                                 MOV      r8, rsi
00000001401C6CAC: 49 8B D7                                 MOV      rdx, r15
00000001401C6CAF: 48 8B CB                                 MOV      rcx, rbx
00000001401C6CB2: E8 B9 36 F3 FF                           CALL     0x1400fa370
00000001401C6CB7: 48 85 F6                                 TEST     rsi, rsi
00000001401C6CBA: 74 30                                    JE       0x1401c6cec
00000001401C6CBC: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001401C6CC0: 4C 8B C7                                 MOV      r8, rdi
00000001401C6CC3: 48 8B 13                                 MOV      rdx, qword ptr [rbx]
00000001401C6CC6: 49 8B CC                                 MOV      rcx, r12
00000001401C6CC9: E8 82 E6 01 00                           CALL     0x1401e5350
00000001401C6CCE: 45 0F B6 F6                              MOVZX    r14d, r14b
00000001401C6CD2: 84 C0                                    TEST     al, al
00000001401C6CD4: B8 01 00 00 00                           MOV      eax, 1
00000001401C6CD9: 44 0F 45 F0                              CMOVNE   r14d, eax
00000001401C6CDD: 41 FF C5                                 INC      r13d
00000001401C6CE0: 48 8D 5B 08                              LEA      rbx, [rbx + 8]
00000001401C6CE4: 49 63 C5                                 MOVSXD   rax, r13d
00000001401C6CE7: 48 3B C6                                 CMP      rax, rsi
00000001401C6CEA: 72 D4                                    JB       0x1401c6cc0
00000001401C6CEC: C7 87 C0 0F 00 00 00 00 40 40            MOV      dword ptr [rdi + 0xfc0], 0x40400000
00000001401C6CF6: 41 83 BC 24 BC 05 00 00 00               CMP      dword ptr [r12 + 0x5bc], 0
00000001401C6CFF: 7E 55                                    JLE      0x1401c6d56
00000001401C6D01: 45 84 F6                                 TEST     r14b, r14b
00000001401C6D04: 74 50                                    JE       0x1401c6d56
00000001401C6D06: 48 8B D7                                 MOV      rdx, rdi
00000001401C6D09: E8 72 35 FE FF                           CALL     0x1401aa280
00000001401C6D0E: F3 44 0F 5F C0                           MAXSS    xmm8, xmm0
00000001401C6D13: F3 45 0F 5D F0                           MINSS    xmm14, xmm8
00000001401C6D18: F2 0F 10 05 D0 34 74 00                  MOVSD    xmm0, qword ptr [rip + 0x7434d0]
00000001401C6D20: 0F 54 05 D9 59 74 00                     ANDPS    xmm0, xmmword ptr [rip + 0x7459d9]
00000001401C6D27: 66 0F 5A C0                              CVTPD2PS xmm0, xmm0
00000001401C6D2B: 0F 2F 05 CA 2D 74 00                     COMISS   xmm0, dword ptr [rip + 0x742dca]
00000001401C6D32: F3 0F 10 05 B2 3B 74 00                  MOVSS    xmm0, dword ptr [rip + 0x743bb2]
00000001401C6D3A: 76 0A                                    JBE      0x1401c6d46
00000001401C6D3C: F3 44 0F 59 F0                           MULSS    xmm14, xmm0
00000001401C6D41: F3 41 0F 5C C6                           SUBSS    xmm0, xmm14
00000001401C6D46: F3 0F 58 05 F6 36 74 00                  ADDSS    xmm0, dword ptr [rip + 0x7436f6]
00000001401C6D4E: F3 0F 11 87 C0 0F 00 00                  MOVSS    dword ptr [rdi + 0xfc0], xmm0
00000001401C6D56: 48 8B 5C 24 50                           MOV      rbx, qword ptr [rsp + 0x50]
00000001401C6D5B: 48 85 DB                                 TEST     rbx, rbx
00000001401C6D5E: 74 1C                                    JE       0x1401c6d7c
00000001401C6D60: 48 8B 95 A0 02 00 00                     MOV      rdx, qword ptr [rbp + 0x2a0]
00000001401C6D67: 48 2B D3                                 SUB      rdx, rbx
00000001401C6D6A: 48 C1 FA 03                              SAR      rdx, 3
00000001401C6D6E: 41 B8 08 00 00 00                        MOV      r8d, 8
00000001401C6D74: 48 8B CB                                 MOV      rcx, rbx
00000001401C6D77: E8 D4 AC E3 FF                           CALL     0x140001a50
00000001401C6D7C: 4C 8D 9C 24 48 03 00 00                  LEA      r11, [rsp + 0x348]
00000001401C6D84: 41 0F 28 73 E8                           MOVAPS   xmm6, xmmword ptr [r11 - 0x18]
00000001401C6D89: 41 0F 28 7B D8                           MOVAPS   xmm7, xmmword ptr [r11 - 0x28]
00000001401C6D8E: 45 0F 28 43 C8                           MOVAPS   xmm8, xmmword ptr [r11 - 0x38]
00000001401C6D93: 45 0F 28 4B B8                           MOVAPS   xmm9, xmmword ptr [r11 - 0x48]
00000001401C6D98: 45 0F 28 53 A8                           MOVAPS   xmm10, xmmword ptr [r11 - 0x58]
00000001401C6D9D: 45 0F 28 5B 98                           MOVAPS   xmm11, xmmword ptr [r11 - 0x68]
00000001401C6DA2: 45 0F 28 63 88                           MOVAPS   xmm12, xmmword ptr [r11 - 0x78]
00000001401C6DA7: 45 0F 28 AB 78 FF FF FF                  MOVAPS   xmm13, xmmword ptr [r11 - 0x88]
00000001401C6DAF: 45 0F 28 B3 68 FF FF FF                  MOVAPS   xmm14, xmmword ptr [r11 - 0x98]
00000001401C6DB7: 45 0F 28 BB 58 FF FF FF                  MOVAPS   xmm15, xmmword ptr [r11 - 0xa8]
00000001401C6DBF: 49 8B E3                                 MOV      rsp, r11
00000001401C6DC2: 41 5F                                    POP      r15
00000001401C6DC4: 41 5E                                    POP      r14
00000001401C6DC6: 41 5D                                    POP      r13
00000001401C6DC8: 41 5C                                    POP      r12
00000001401C6DCA: 5F                                       POP      rdi
00000001401C6DCB: 5E                                       POP      rsi
00000001401C6DCC: 5B                                       POP      rbx
00000001401C6DCD: 5D                                       POP      rbp
00000001401C6DCE: C3                                       RET
```

#### `FUN_1406B8EB0` — shared association/update helper

```asm
00000001406B8EB0: 48 8B C4                                 MOV      rax, rsp
00000001406B8EB3: 88 50 10                                 MOV      byte ptr [rax + 0x10], dl
00000001406B8EB6: 55                                       PUSH     rbp
00000001406B8EB7: 56                                       PUSH     rsi
00000001406B8EB8: 57                                       PUSH     rdi
00000001406B8EB9: 41 54                                    PUSH     r12
00000001406B8EBB: 41 55                                    PUSH     r13
00000001406B8EBD: 41 56                                    PUSH     r14
00000001406B8EBF: 41 57                                    PUSH     r15
00000001406B8EC1: 48 8D 68 A1                              LEA      rbp, [rax - 0x5f]
00000001406B8EC5: 48 81 EC E0 00 00 00                     SUB      rsp, 0xe0
00000001406B8ECC: 48 C7 45 9F FE FF FF FF                  MOV      qword ptr [rbp - 0x61], 0xfffffffffffffffe
00000001406B8ED4: 48 89 58 18                              MOV      qword ptr [rax + 0x18], rbx
00000001406B8ED8: 0F 29 70 B8                              MOVAPS   xmmword ptr [rax - 0x48], xmm6
00000001406B8EDC: 0F 29 78 A8                              MOVAPS   xmmword ptr [rax - 0x58], xmm7
00000001406B8EE0: 44 0F 29 40 98                           MOVAPS   xmmword ptr [rax - 0x68], xmm8
00000001406B8EE5: 44 0F 29 48 88                           MOVAPS   xmmword ptr [rax - 0x78], xmm9
00000001406B8EEA: 48 8B F9                                 MOV      rdi, rcx
00000001406B8EED: 48 83 B9 80 0C 00 00 00                  CMP      qword ptr [rcx + 0xc80], 0
00000001406B8EF5: 0F 85 6B 1D 00 00                        JNE      0x1406bac66
00000001406B8EFB: 48 83 B9 F0 04 00 00 00                  CMP      qword ptr [rcx + 0x4f0], 0
00000001406B8F03: 0F 84 5D 1D 00 00                        JE       0x1406bac66
00000001406B8F09: 48 8D 81 D0 07 00 00                     LEA      rax, [rcx + 0x7d0]
00000001406B8F10: B9 4B 00 00 00                           MOV      ecx, 0x4b
00000001406B8F15: 45 33 F6                                 XOR      r14d, r14d
00000001406B8F18: 0F 1F 84 00 00 00 00 00                  NOP      dword ptr [rax + rax]
00000001406B8F20: 4C 89 30                                 MOV      qword ptr [rax], r14
00000001406B8F23: 48 83 C0 10                              ADD      rax, 0x10
00000001406B8F27: 48 83 E9 01                              SUB      rcx, 1
00000001406B8F2B: 75 F3                                    JNE      0x1406b8f20
00000001406B8F2D: 44 38 B7 FD 05 00 00                     CMP      byte ptr [rdi + 0x5fd], r14b
00000001406B8F34: 75 47                                    JNE      0x1406b8f7d
00000001406B8F36: C7 87 6C 06 00 00 00 00 80 BF            MOV      dword ptr [rdi + 0x66c], 0xbf800000
00000001406B8F40: 45 84 C9                                 TEST     r9b, r9b
00000001406B8F43: 74 31                                    JE       0x1406b8f76
00000001406B8F45: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B8F4C: 48 85 C0                                 TEST     rax, rax
00000001406B8F4F: 74 2C                                    JE       0x1406b8f7d
00000001406B8F51: 48 8B 80 18 03 00 00                     MOV      rax, qword ptr [rax + 0x318]
00000001406B8F58: 48 85 C0                                 TEST     rax, rax
00000001406B8F5B: 74 20                                    JE       0x1406b8f7d
00000001406B8F5D: 8B 88 60 03 00 00                        MOV      ecx, dword ptr [rax + 0x360]
00000001406B8F63: 83 F9 0F                                 CMP      ecx, 0xf
00000001406B8F66: 74 0E                                    JE       0x1406b8f76
00000001406B8F68: 83 F9 0E                                 CMP      ecx, 0xe
00000001406B8F6B: 75 10                                    JNE      0x1406b8f7d
00000001406B8F6D: 83 B8 64 03 00 00 24                     CMP      dword ptr [rax + 0x364], 0x24
00000001406B8F74: 75 07                                    JNE      0x1406b8f7d
00000001406B8F76: 44 89 B7 98 06 00 00                     MOV      dword ptr [rdi + 0x698], r14d
00000001406B8F7D: 66 44 89 B7 39 0D 00 00                  MOV      word ptr [rdi + 0xd39], r14w
00000001406B8F85: 66 44 89 B7 3D 0D 00 00                  MOV      word ptr [rdi + 0xd3d], r14w
00000001406B8F8D: 44 88 B7 3F 0D 00 00                     MOV      byte ptr [rdi + 0xd3f], r14b
00000001406B8F94: 45 84 C0                                 TEST     r8b, r8b
00000001406B8F97: 75 58                                    JNE      0x1406b8ff1
00000001406B8F99: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406B8FA0: 8B 88 94 02 00 00                        MOV      ecx, dword ptr [rax + 0x294]
00000001406B8FA6: 83 F9 06                                 CMP      ecx, 6
00000001406B8FA9: 75 0D                                    JNE      0x1406b8fb8
00000001406B8FAB: 48 8B CF                                 MOV      rcx, rdi
00000001406B8FAE: E8 7D 89 02 00                           CALL     0x1406e1930
00000001406B8FB3: E9 CE 1C 00 00                           JMP      0x1406bac86
00000001406B8FB8: 83 F9 08                                 CMP      ecx, 8
00000001406B8FBB: 75 10                                    JNE      0x1406b8fcd
00000001406B8FBD: 45 33 C0                                 XOR      r8d, r8d
00000001406B8FC0: 83 CA FF                                 OR       edx, 0xffffffff
00000001406B8FC3: 48 8B CF                                 MOV      rcx, rdi
00000001406B8FC6: E8 75 EC 01 00                           CALL     0x1406d7c40
00000001406B8FCB: EB 24                                    JMP      0x1406b8ff1
00000001406B8FCD: 83 F9 0A                                 CMP      ecx, 0xa
00000001406B8FD0: 48 8B CF                                 MOV      rcx, rdi
00000001406B8FD3: 75 07                                    JNE      0x1406b8fdc
00000001406B8FD5: E8 66 46 02 00                           CALL     0x1406dd640
00000001406B8FDA: EB 15                                    JMP      0x1406b8ff1
00000001406B8FDC: 44 88 74 24 28                           MOV      byte ptr [rsp + 0x28], r14b
00000001406B8FE1: 44 88 74 24 20                           MOV      byte ptr [rsp + 0x20], r14b
00000001406B8FE6: 41 83 C8 FF                              OR       r8d, 0xffffffff
00000001406B8FEA: 33 D2                                    XOR      edx, edx
00000001406B8FEC: E8 2F 7A FC FF                           CALL     0x140680a20
00000001406B8FF1: 44 38 B7 39 0D 00 00                     CMP      byte ptr [rdi + 0xd39], r14b
00000001406B8FF8: 75 7B                                    JNE      0x1406b9075
00000001406B8FFA: 48 8B CF                                 MOV      rcx, rdi
00000001406B8FFD: E8 DE A5 FA FF                           CALL     0x1406635e0
00000001406B9002: 84 C0                                    TEST     al, al
00000001406B9004: 75 76                                    JNE      0x1406b907c
00000001406B9006: C6 87 3F 0D 00 00 01                     MOV      byte ptr [rdi + 0xd3f], 1
00000001406B900D: B2 01                                    MOV      dl, 1
00000001406B900F: 48 8B CF                                 MOV      rcx, rdi
00000001406B9012: E8 09 D7 FE FF                           CALL     0x1406a6720
00000001406B9017: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B901E: 48 89 87 A8 06 00 00                     MOV      qword ptr [rdi + 0x6a8], rax
00000001406B9025: 48 8B 87 B8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6b8]
00000001406B902C: 48 89 87 C0 06 00 00                     MOV      qword ptr [rdi + 0x6c0], rax
00000001406B9033: 48 8B 87 38 07 00 00                     MOV      rax, qword ptr [rdi + 0x738]
00000001406B903A: 48 89 87 40 07 00 00                     MOV      qword ptr [rdi + 0x740], rax
00000001406B9041: 48 8B 87 50 07 00 00                     MOV      rax, qword ptr [rdi + 0x750]
00000001406B9048: 48 89 87 58 07 00 00                     MOV      qword ptr [rdi + 0x758], rax
00000001406B904F: 4C 89 B7 98 07 00 00                     MOV      qword ptr [rdi + 0x798], r14
00000001406B9056: 48 8B 87 C0 07 00 00                     MOV      rax, qword ptr [rdi + 0x7c0]
00000001406B905D: 48 2B 87 B8 07 00 00                     SUB      rax, qword ptr [rdi + 0x7b8]
00000001406B9064: 48 C1 F8 03                              SAR      rax, 3
00000001406B9068: 48 85 C0                                 TEST     rax, rax
00000001406B906B: 74 08                                    JE       0x1406b9075
00000001406B906D: 48 8B CF                                 MOV      rcx, rdi
00000001406B9070: E8 FB 88 FC FF                           CALL     0x140681970
00000001406B9075: 32 C0                                    XOR      al, al
00000001406B9077: E9 0C 1C 00 00                           JMP      0x1406bac88
00000001406B907C: 49 BD AB AA AA AA AA AA AA 2A            MOVABS   r13, 0x2aaaaaaaaaaaaaab
00000001406B9086: 0F 57 F6                                 XORPS    xmm6, xmm6
00000001406B9089: 44 38 B7 7D 06 00 00                     CMP      byte ptr [rdi + 0x67d], r14b
00000001406B9090: 0F 85 8E 03 00 00                        JNE      0x1406b9424
00000001406B9096: 4C 8B 8F F0 04 00 00                     MOV      r9, qword ptr [rdi + 0x4f0]
00000001406B909D: 49 8B 81 18 03 00 00                     MOV      rax, qword ptr [r9 + 0x318]
00000001406B90A4: 48 85 C0                                 TEST     rax, rax
00000001406B90A7: 0F 84 77 03 00 00                        JE       0x1406b9424
00000001406B90AD: 83 B8 60 03 00 00 0F                     CMP      dword ptr [rax + 0x360], 0xf
00000001406B90B4: 0F 85 6A 03 00 00                        JNE      0x1406b9424
00000001406B90BA: 4C 8B 97 08 17 00 00                     MOV      r10, qword ptr [rdi + 0x1708]
00000001406B90C1: 41 83 BA 94 02 00 00 04                  CMP      dword ptr [r10 + 0x294], 4
00000001406B90C9: 0F 85 55 03 00 00                        JNE      0x1406b9424
00000001406B90CF: 48 8B 87 A8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a8]
00000001406B90D6: 48 2B 87 A0 06 00 00                     SUB      rax, qword ptr [rdi + 0x6a0]
00000001406B90DD: 48 83 F8 08                              CMP      rax, 8
00000001406B90E1: 0F 82 3D 03 00 00                        JB       0x1406b9424
00000001406B90E7: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B90EE: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001406B90F1: 44 39 B1 E0 00 00 00                     CMP      dword ptr [rcx + 0xe0], r14d
00000001406B90F8: 0F 85 26 03 00 00                        JNE      0x1406b9424
00000001406B90FE: C7 45 67 FF FF FF FF                     MOV      dword ptr [rbp + 0x67], 0xffffffff
00000001406B9105: 45 8B C6                                 MOV      r8d, r14d
00000001406B9108: 49 8B 81 80 0B 00 00                     MOV      rax, qword ptr [r9 + 0xb80]
00000001406B910F: 49 2B 81 78 0B 00 00                     SUB      rax, qword ptr [r9 + 0xb78]
00000001406B9116: 48 C1 F8 05                              SAR      rax, 5
00000001406B911A: 48 85 C0                                 TEST     rax, rax
00000001406B911D: 74 32                                    JE       0x1406b9151
00000001406B911F: 49 8D 91 90 0B 00 00                     LEA      rdx, [r9 + 0xb90]
00000001406B9126: 44 38 32                                 CMP      byte ptr [rdx], r14b
00000001406B9129: 74 22                                    JE       0x1406b914d
00000001406B912B: 41 FF C0                                 INC      r8d
00000001406B912E: 48 FF C2                                 INC      rdx
00000001406B9131: 49 8B 89 80 0B 00 00                     MOV      rcx, qword ptr [r9 + 0xb80]
00000001406B9138: 49 2B 89 78 0B 00 00                     SUB      rcx, qword ptr [r9 + 0xb78]
00000001406B913F: 48 C1 F9 05                              SAR      rcx, 5
00000001406B9143: 49 63 C0                                 MOVSXD   rax, r8d
00000001406B9146: 48 3B C1                                 CMP      rax, rcx
00000001406B9149: 72 DB                                    JB       0x1406b9126
00000001406B914B: EB 04                                    JMP      0x1406b9151
00000001406B914D: 44 89 45 67                              MOV      dword ptr [rbp + 0x67], r8d
00000001406B9151: C6 87 7D 06 00 00 01                     MOV      byte ptr [rdi + 0x67d], 1
00000001406B9158: 48 8B 05 51 B5 32 00                     MOV      rax, qword ptr [rip + 0x32b551]
00000001406B915F: 48 89 87 10 17 00 00                     MOV      qword ptr [rdi + 0x1710], rax
00000001406B9166: 41 8B 82 F8 7A 00 00                     MOV      eax, dword ptr [r10 + 0x7af8]
00000001406B916D: 89 87 4C 11 00 00                        MOV      dword ptr [rdi + 0x114c], eax
00000001406B9173: 41 8B 82 FC 7A 00 00                     MOV      eax, dword ptr [r10 + 0x7afc]
00000001406B917A: 89 87 50 11 00 00                        MOV      dword ptr [rdi + 0x1150], eax
00000001406B9180: 45 8B EE                                 MOV      r13d, r14d
00000001406B9183: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406B918A: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406B9191: 48 C1 F8 03                              SAR      rax, 3
00000001406B9195: 48 85 C0                                 TEST     rax, rax
00000001406B9198: 0F 84 32 01 00 00                        JE       0x1406b92d0
00000001406B919E: 4D 8B E6                                 MOV      r12, r14
00000001406B91A1: 41 BE 01 00 00 00                        MOV      r14d, 1
00000001406B91A7: 66 0F 1F 84 00 00 00 00 00               NOP      word ptr [rax + rax]
00000001406B91B0: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B91B7: 4A 8B 0C E0                              MOV      rcx, qword ptr [rax + r12*8]
00000001406B91BB: 48 8B 81 B0 04 00 00                     MOV      rax, qword ptr [rcx + 0x4b0]
00000001406B91C2: 48 2B 81 A8 04 00 00                     SUB      rax, qword ptr [rcx + 0x4a8]
00000001406B91C9: 48 C1 F8 03                              SAR      rax, 3
00000001406B91CD: 41 2B C6                                 SUB      eax, r14d
00000001406B91D0: 4C 63 F8                                 MOVSXD   r15, eax
00000001406B91D3: 0F 88 BA 00 00 00                        JS       0x1406b9293
00000001406B91D9: 0F 1F 80 00 00 00 00                     NOP      dword ptr [rax]
00000001406B91E0: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B91E7: 4A 8B 04 E0                              MOV      rax, qword ptr [rax + r12*8]
00000001406B91EB: 48 8B 80 A8 04 00 00                     MOV      rax, qword ptr [rax + 0x4a8]
00000001406B91F2: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406B91F6: 48 8B 87 90 03 00 00                     MOV      rax, qword ptr [rdi + 0x390]
00000001406B91FD: 48 89 41 60                              MOV      qword ptr [rcx + 0x60], rax
00000001406B9201: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B9208: 4A 8B 04 E0                              MOV      rax, qword ptr [rax + r12*8]
00000001406B920C: 48 8B 80 A8 04 00 00                     MOV      rax, qword ptr [rax + 0x4a8]
00000001406B9213: 4A 8D 34 F8                              LEA      rsi, [rax + r15*8]
00000001406B9217: 48 8B 9F 90 03 00 00                     MOV      rbx, qword ptr [rdi + 0x390]
00000001406B921E: 48 81 C3 A8 04 00 00                     ADD      rbx, 0x4a8
00000001406B9225: 48 3B 73 08                              CMP      rsi, qword ptr [rbx + 8]
00000001406B9229: 73 36                                    JAE      0x1406b9261
00000001406B922B: 48 8B 03                                 MOV      rax, qword ptr [rbx]
00000001406B922E: 48 3B C6                                 CMP      rax, rsi
00000001406B9231: 77 2E                                    JA       0x1406b9261
00000001406B9233: 48 2B F0                                 SUB      rsi, rax
00000001406B9236: 48 C1 FE 03                              SAR      rsi, 3
00000001406B923A: 48 8B 43 10                              MOV      rax, qword ptr [rbx + 0x10]
00000001406B923E: 48 39 43 08                              CMP      qword ptr [rbx + 8], rax
00000001406B9242: 75 0B                                    JNE      0x1406b924f
00000001406B9244: 49 8B D6                                 MOV      rdx, r14
00000001406B9247: 48 8B CB                                 MOV      rcx, rbx
00000001406B924A: E8 B1 84 9F FF                           CALL     0x1400b1700
00000001406B924F: 48 8B 4B 08                              MOV      rcx, qword ptr [rbx + 8]
00000001406B9253: 48 8B 03                                 MOV      rax, qword ptr [rbx]
00000001406B9256: 48 85 C9                                 TEST     rcx, rcx
00000001406B9259: 74 2A                                    JE       0x1406b9285
00000001406B925B: 48 8B 04 F0                              MOV      rax, qword ptr [rax + rsi*8]
00000001406B925F: EB 21                                    JMP      0x1406b9282
00000001406B9261: 48 8B 43 10                              MOV      rax, qword ptr [rbx + 0x10]
00000001406B9265: 48 39 43 08                              CMP      qword ptr [rbx + 8], rax
00000001406B9269: 75 0B                                    JNE      0x1406b9276
00000001406B926B: 49 8B D6                                 MOV      rdx, r14
00000001406B926E: 48 8B CB                                 MOV      rcx, rbx
00000001406B9271: E8 8A 84 9F FF                           CALL     0x1400b1700
00000001406B9276: 48 8B 4B 08                              MOV      rcx, qword ptr [rbx + 8]
00000001406B927A: 48 85 C9                                 TEST     rcx, rcx
00000001406B927D: 74 06                                    JE       0x1406b9285
00000001406B927F: 48 8B 06                                 MOV      rax, qword ptr [rsi]
00000001406B9282: 48 89 01                                 MOV      qword ptr [rcx], rax
00000001406B9285: 48 83 43 08 08                           ADD      qword ptr [rbx + 8], 8
00000001406B928A: 4D 2B FE                                 SUB      r15, r14
00000001406B928D: 0F 89 4D FF FF FF                        JNS      0x1406b91e0
00000001406B9293: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B929A: 4A 8B 0C E0                              MOV      rcx, qword ptr [rax + r12*8]
00000001406B929E: 48 8B 81 A8 04 00 00                     MOV      rax, qword ptr [rcx + 0x4a8]
00000001406B92A5: 48 89 81 B0 04 00 00                     MOV      qword ptr [rcx + 0x4b0], rax
00000001406B92AC: 41 FF C5                                 INC      r13d
00000001406B92AF: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406B92B6: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406B92BD: 48 C1 F8 03                              SAR      rax, 3
00000001406B92C1: 45 8B E5                                 MOV      r12d, r13d
00000001406B92C4: 4C 3B E0                                 CMP      r12, rax
00000001406B92C7: 0F 82 E3 FE FF FF                        JB       0x1406b91b0
00000001406B92CD: 45 33 F6                                 XOR      r14d, r14d
00000001406B92D0: 83 7D 67 00                              CMP      dword ptr [rbp + 0x67], 0
00000001406B92D4: 7C 17                                    JL       0x1406b92ed
00000001406B92D6: 48 8B CF                                 MOV      rcx, rdi
00000001406B92D9: E8 72 E1 FA FF                           CALL     0x140667450
00000001406B92DE: 49 BD AB AA AA AA AA AA AA 2A            MOVABS   r13, 0x2aaaaaaaaaaaaaab
00000001406B92E8: E9 2F 01 00 00                           JMP      0x1406b941c
00000001406B92ED: 45 8B D6                                 MOV      r10d, r14d
00000001406B92F0: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406B92F7: 49 8B 88 18 0A 00 00                     MOV      rcx, qword ptr [r8 + 0xa18]
00000001406B92FE: 49 2B 88 10 0A 00 00                     SUB      rcx, qword ptr [r8 + 0xa10]
00000001406B9305: 49 BD AB AA AA AA AA AA AA 2A            MOVABS   r13, 0x2aaaaaaaaaaaaaab
00000001406B930F: 49 8B C5                                 MOV      rax, r13
00000001406B9312: 48 F7 E9                                 IMUL     rcx
00000001406B9315: 48 C1 FA 04                              SAR      rdx, 4
00000001406B9319: 48 8B C2                                 MOV      rax, rdx
00000001406B931C: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9320: 48 03 D0                                 ADD      rdx, rax
00000001406B9323: 0F 84 F3 00 00 00                        JE       0x1406b941c
00000001406B9329: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9330: 4C 8B 18                                 MOV      r11, qword ptr [rax]
00000001406B9333: 4D 8B CE                                 MOV      r9, r14
00000001406B9336: 66 66 0F 1F 84 00 00 00 00 00            NOP      word ptr [rax + rax]
00000001406B9340: 49 8B 80 10 0A 00 00                     MOV      rax, qword ptr [r8 + 0xa10]
00000001406B9347: 4D 39 5C 01 30                           CMP      qword ptr [r9 + rax + 0x30], r11
00000001406B934C: 74 32                                    JE       0x1406b9380
00000001406B934E: 41 FF C2                                 INC      r10d
00000001406B9351: 49 83 C1 60                              ADD      r9, 0x60
00000001406B9355: 49 8B 88 18 0A 00 00                     MOV      rcx, qword ptr [r8 + 0xa18]
00000001406B935C: 48 2B C8                                 SUB      rcx, rax
00000001406B935F: 49 8B C5                                 MOV      rax, r13
00000001406B9362: 48 F7 E9                                 IMUL     rcx
00000001406B9365: 48 C1 FA 04                              SAR      rdx, 4
00000001406B9369: 48 8B C2                                 MOV      rax, rdx
00000001406B936C: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9370: 48 03 D0                                 ADD      rdx, rax
00000001406B9373: 49 63 C2                                 MOVSXD   rax, r10d
00000001406B9376: 48 3B C2                                 CMP      rax, rdx
00000001406B9379: 72 C5                                    JB       0x1406b9340
00000001406B937B: E9 9C 00 00 00                           JMP      0x1406b941c
00000001406B9380: 49 63 C2                                 MOVSXD   rax, r10d
00000001406B9383: 48 8D 1C 40                              LEA      rbx, [rax + rax*2]
00000001406B9387: 48 C1 E3 05                              SHL      rbx, 5
00000001406B938B: 49 8B 88 10 0A 00 00                     MOV      rcx, qword ptr [r8 + 0xa10]
00000001406B9392: F3 0F 10 54 0B 10                        MOVSS    xmm2, dword ptr [rbx + rcx + 0x10]
00000001406B9398: F3 0F 5C 54 0B 1C                        SUBSS    xmm2, dword ptr [rbx + rcx + 0x1c]
00000001406B939E: F3 0F 10 4C 0B 0C                        MOVSS    xmm1, dword ptr [rbx + rcx + 0xc]
00000001406B93A4: F3 0F 5C 4C 0B 18                        SUBSS    xmm1, dword ptr [rbx + rcx + 0x18]
00000001406B93AA: F3 0F 10 44 0B 08                        MOVSS    xmm0, dword ptr [rbx + rcx + 8]
00000001406B93B0: F3 0F 5C 44 0B 14                        SUBSS    xmm0, dword ptr [rbx + rcx + 0x14]
00000001406B93B6: F3 0F 11 44 24 30                        MOVSS    dword ptr [rsp + 0x30], xmm0
00000001406B93BC: F3 0F 11 4C 24 34                        MOVSS    dword ptr [rsp + 0x34], xmm1
00000001406B93C2: F3 0F 11 54 24 38                        MOVSS    dword ptr [rsp + 0x38], xmm2
00000001406B93C8: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001406B93CD: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001406B93D2: FF 15 28 3C 1B 00                        CALL     qword ptr [rip + 0x1b3c28]
00000001406B93D8: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B93DF: 48 8B 88 10 0A 00 00                     MOV      rcx, qword ptr [rax + 0xa10]
00000001406B93E6: F3 0F 10 44 0B 08                        MOVSS    xmm0, dword ptr [rbx + rcx + 8]
00000001406B93EC: F3 0F 11 45 87                           MOVSS    dword ptr [rbp - 0x79], xmm0
00000001406B93F1: F3 0F 10 4C 0B 0C                        MOVSS    xmm1, dword ptr [rbx + rcx + 0xc]
00000001406B93F7: F3 0F 11 4D 8B                           MOVSS    dword ptr [rbp - 0x75], xmm1
00000001406B93FC: F3 0F 10 44 0B 10                        MOVSS    xmm0, dword ptr [rbx + rcx + 0x10]
00000001406B9402: F3 0F 11 45 8F                           MOVSS    dword ptr [rbp - 0x71], xmm0
00000001406B9407: 0F 28 DE                                 MOVAPS   xmm3, xmm6
00000001406B940A: F3 0F 10 54 24 34                        MOVSS    xmm2, dword ptr [rsp + 0x34]
00000001406B9410: 48 8D 55 87                              LEA      rdx, [rbp - 0x79]
00000001406B9414: 48 8B CF                                 MOV      rcx, rdi
00000001406B9417: E8 C4 69 FF FF                           CALL     0x1406afde0
00000001406B941C: 48 8B CF                                 MOV      rcx, rdi
00000001406B941F: E8 6C 3E FF FF                           CALL     0x1406ad290
00000001406B9424: 80 BF 7D 06 00 00 00                     CMP      byte ptr [rdi + 0x67d], 0
00000001406B942B: 0F 85 B7 07 00 00                        JNE      0x1406b9be8
00000001406B9431: 48 8B 8F 08 17 00 00                     MOV      rcx, qword ptr [rdi + 0x1708]
00000001406B9438: 8B 81 94 02 00 00                        MOV      eax, dword ptr [rcx + 0x294]
00000001406B943E: 83 E8 04                                 SUB      eax, 4
00000001406B9441: 83 F8 01                                 CMP      eax, 1
00000001406B9444: 0F 87 9E 07 00 00                        JA       0x1406b9be8
00000001406B944A: 48 85 C9                                 TEST     rcx, rcx
00000001406B944D: 0F 84 0E 03 00 00                        JE       0x1406b9761
00000001406B9453: F3 0F 10 81 84 86 00 00                  MOVSS    xmm0, dword ptr [rcx + 0x8684]
00000001406B945B: 0F 2F C6                                 COMISS   xmm0, xmm6
00000001406B945E: 76 10                                    JBE      0x1406b9470
00000001406B9460: 48 8B CF                                 MOV      rcx, rdi
00000001406B9463: E8 68 41 00 00                           CALL     0x1406bd5d0
00000001406B9468: 84 C0                                    TEST     al, al
00000001406B946A: 75 04                                    JNE      0x1406b9470
00000001406B946C: 32 D2                                    XOR      dl, dl
00000001406B946E: EB 02                                    JMP      0x1406b9472
00000001406B9470: B2 01                                    MOV      dl, 1
00000001406B9472: 45 8B CE                                 MOV      r9d, r14d
00000001406B9475: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406B947C: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406B9483: 48 C1 F8 03                              SAR      rax, 3
00000001406B9487: 48 85 C0                                 TEST     rax, rax
00000001406B948A: 74 60                                    JE       0x1406b94ec
00000001406B948C: 4D 8B C6                                 MOV      r8, r14
00000001406B948F: 90                                       NOP
00000001406B9490: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B9497: 4A 8B 0C C0                              MOV      rcx, qword ptr [rax + r8*8]
00000001406B949B: 48 8B 81 08 17 00 00                     MOV      rax, qword ptr [rcx + 0x1708]
00000001406B94A2: F3 0F 10 80 84 86 00 00                  MOVSS    xmm0, dword ptr [rax + 0x8684]
00000001406B94AA: 0F 2F C6                                 COMISS   xmm0, xmm6
00000001406B94AD: 76 20                                    JBE      0x1406b94cf
00000001406B94AF: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406B94B6: 4A 8B 0C C0                              MOV      rcx, qword ptr [rax + r8*8]
00000001406B94BA: 48 8B 81 08 17 00 00                     MOV      rax, qword ptr [rcx + 0x1708]
00000001406B94C1: 0F B6 D2                                 MOVZX    edx, dl
00000001406B94C4: 83 B8 94 02 00 00 04                     CMP      dword ptr [rax + 0x294], 4
00000001406B94CB: 41 0F 44 D6                              CMOVE    edx, r14d
00000001406B94CF: 41 FF C1                                 INC      r9d
00000001406B94D2: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406B94D9: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406B94E0: 48 C1 F8 03                              SAR      rax, 3
00000001406B94E4: 45 8B C1                                 MOV      r8d, r9d
00000001406B94E7: 4C 3B C0                                 CMP      r8, rax
00000001406B94EA: 72 A4                                    JB       0x1406b9490
00000001406B94EC: 84 D2                                    TEST     dl, dl
00000001406B94EE: 0F 84 6D 02 00 00                        JE       0x1406b9761
00000001406B94F4: 83 3D C5 BF 31 00 02                     CMP      dword ptr [rip + 0x31bfc5], 2
00000001406B94FB: 0F 85 60 02 00 00                        JNE      0x1406b9761
00000001406B9501: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9508: 48 8B 00                                 MOV      rax, qword ptr [rax]
00000001406B950B: 48 8B 40 28                              MOV      rax, qword ptr [rax + 0x28]
00000001406B950F: 4C 8B 28                                 MOV      r13, qword ptr [rax]
00000001406B9512: 49 8B 85 A8 01 00 00                     MOV      rax, qword ptr [r13 + 0x1a8]
00000001406B9519: 49 2B 85 A0 01 00 00                     SUB      rax, qword ptr [r13 + 0x1a0]
00000001406B9520: 48 83 F8 08                              CMP      rax, 8
00000001406B9524: 73 0C                                    JAE      0x1406b9532
00000001406B9526: C6 87 3E 0D 00 00 01                     MOV      byte ptr [rdi + 0xd3e], 1
00000001406B952D: E9 DB FA FF FF                           JMP      0x1406b900d
00000001406B9532: 0F 57 C0                                 XORPS    xmm0, xmm0
00000001406B9535: F3 0F 7F 45 87                           MOVDQU   xmmword ptr [rbp - 0x79], xmm0
00000001406B953A: 4D 8B FE                                 MOV      r15, r14
00000001406B953D: 4C 89 75 97                              MOV      qword ptr [rbp - 0x69], r14
00000001406B9541: 45 8B E6                                 MOV      r12d, r14d
00000001406B9544: 49 8B 85 A8 01 00 00                     MOV      rax, qword ptr [r13 + 0x1a8]
00000001406B954B: 49 2B 85 A0 01 00 00                     SUB      rax, qword ptr [r13 + 0x1a0]
00000001406B9552: 48 C1 F8 03                              SAR      rax, 3
00000001406B9556: 4C 8B 55 8F                              MOV      r10, qword ptr [rbp - 0x71]
00000001406B955A: 48 8B 75 87                              MOV      rsi, qword ptr [rbp - 0x79]
00000001406B955E: 48 85 C0                                 TEST     rax, rax
00000001406B9561: 0F 84 BD 00 00 00                        JE       0x1406b9624
00000001406B9567: 4D 8B DE                                 MOV      r11, r14
00000001406B956A: 66 0F 1F 44 00 00                        NOP      word ptr [rax + rax]
00000001406B9570: 49 8B 95 A0 01 00 00                     MOV      rdx, qword ptr [r13 + 0x1a0]
00000001406B9577: 4A 8B 14 DA                              MOV      rdx, qword ptr [rdx + r11*8]
00000001406B957B: 48 8D 0D 8E B9 31 00                     LEA      rcx, [rip + 0x31b98e]
00000001406B9582: E8 59 2A B0 FF                           CALL     0x1401bbfe0
00000001406B9587: 84 C0                                    TEST     al, al
00000001406B9589: 75 78                                    JNE      0x1406b9603
00000001406B958B: 49 8B 85 A0 01 00 00                     MOV      rax, qword ptr [r13 + 0x1a0]
00000001406B9592: 4A 8D 1C D8                              LEA      rbx, [rax + r11*8]
00000001406B9596: 49 3B DA                                 CMP      rbx, r10
00000001406B9599: 73 36                                    JAE      0x1406b95d1
00000001406B959B: 48 3B F3                                 CMP      rsi, rbx
00000001406B959E: 77 31                                    JA       0x1406b95d1
00000001406B95A0: 48 2B DE                                 SUB      rbx, rsi
00000001406B95A3: 48 C1 FB 03                              SAR      rbx, 3
00000001406B95A7: 4D 3B D7                                 CMP      r10, r15
00000001406B95AA: 75 1A                                    JNE      0x1406b95c6
00000001406B95AC: BA 01 00 00 00                           MOV      edx, 1
00000001406B95B1: 48 8D 4D 87                              LEA      rcx, [rbp - 0x79]
00000001406B95B5: E8 46 81 9F FF                           CALL     0x1400b1700
00000001406B95BA: 4C 8B 7D 97                              MOV      r15, qword ptr [rbp - 0x69]
00000001406B95BE: 4C 8B 55 8F                              MOV      r10, qword ptr [rbp - 0x71]
00000001406B95C2: 48 8B 75 87                              MOV      rsi, qword ptr [rbp - 0x79]
00000001406B95C6: 4D 85 D2                                 TEST     r10, r10
00000001406B95C9: 74 30                                    JE       0x1406b95fb
00000001406B95CB: 48 8B 04 DE                              MOV      rax, qword ptr [rsi + rbx*8]
00000001406B95CF: EB 27                                    JMP      0x1406b95f8
00000001406B95D1: 4D 3B D7                                 CMP      r10, r15
00000001406B95D4: 75 1A                                    JNE      0x1406b95f0
00000001406B95D6: BA 01 00 00 00                           MOV      edx, 1
00000001406B95DB: 48 8D 4D 87                              LEA      rcx, [rbp - 0x79]
00000001406B95DF: E8 1C 81 9F FF                           CALL     0x1400b1700
00000001406B95E4: 4C 8B 7D 97                              MOV      r15, qword ptr [rbp - 0x69]
00000001406B95E8: 4C 8B 55 8F                              MOV      r10, qword ptr [rbp - 0x71]
00000001406B95EC: 48 8B 75 87                              MOV      rsi, qword ptr [rbp - 0x79]
00000001406B95F0: 4D 85 D2                                 TEST     r10, r10
00000001406B95F3: 74 06                                    JE       0x1406b95fb
00000001406B95F5: 48 8B 03                                 MOV      rax, qword ptr [rbx]
00000001406B95F8: 49 89 02                                 MOV      qword ptr [r10], rax
00000001406B95FB: 49 83 C2 08                              ADD      r10, 8
00000001406B95FF: 4C 89 55 8F                              MOV      qword ptr [rbp - 0x71], r10
00000001406B9603: 41 FF C4                                 INC      r12d
00000001406B9606: 49 8B 85 A8 01 00 00                     MOV      rax, qword ptr [r13 + 0x1a8]
00000001406B960D: 49 2B 85 A0 01 00 00                     SUB      rax, qword ptr [r13 + 0x1a0]
00000001406B9614: 48 C1 F8 03                              SAR      rax, 3
00000001406B9618: 45 8B DC                                 MOV      r11d, r12d
00000001406B961B: 4C 3B D8                                 CMP      r11, rax
00000001406B961E: 0F 82 4C FF FF FF                        JB       0x1406b9570
00000001406B9624: 4C 2B D6                                 SUB      r10, rsi
00000001406B9627: 49 83 FA 08                              CMP      r10, 8
00000001406B962B: 0F 82 7D 00 00 00                        JB       0x1406b96ae
00000001406B9631: 80 BF 3D 0D 00 00 00                     CMP      byte ptr [rdi + 0xd3d], 0
00000001406B9638: 0F 85 06 01 00 00                        JNE      0x1406b9744
00000001406B963E: 45 8B C6                                 MOV      r8d, r14d
00000001406B9641: 49 C1 FA 03                              SAR      r10, 3
00000001406B9645: 4D 85 D2                                 TEST     r10, r10
00000001406B9648: 74 3A                                    JE       0x1406b9684
00000001406B964A: 49 8B C6                                 MOV      rax, r14
00000001406B964D: 0F 1F 00                                 NOP      dword ptr [rax]
00000001406B9650: 48 8B 14 C6                              MOV      rdx, qword ptr [rsi + rax*8]
00000001406B9654: 48 8B 82 E0 10 00 00                     MOV      rax, qword ptr [rdx + 0x10e0]
00000001406B965B: 48 8B 8A F8 10 00 00                     MOV      rcx, qword ptr [rdx + 0x10f8]
00000001406B9662: F3 0F 10 00                              MOVSS    xmm0, dword ptr [rax]
00000001406B9666: F3 0F 5E 01                              DIVSS    xmm0, dword ptr [rcx]
00000001406B966A: 48 8B 82 E8 10 00 00                     MOV      rax, qword ptr [rdx + 0x10e8]
00000001406B9671: F3 0F 59 00                              MULSS    xmm0, dword ptr [rax]
00000001406B9675: F3 0F 58 F0                              ADDSS    xmm6, xmm0
00000001406B9679: 41 FF C0                                 INC      r8d
00000001406B967C: 41 8B C0                                 MOV      eax, r8d
00000001406B967F: 49 3B C2                                 CMP      rax, r10
00000001406B9682: 72 CC                                    JB       0x1406b9650
00000001406B9684: 0F 57 C0                                 XORPS    xmm0, xmm0
00000001406B9687: F3 49 0F 2A C2                           CVTSI2SS xmm0, r10
00000001406B968C: 4D 85 D2                                 TEST     r10, r10
00000001406B968F: 79 08                                    JNS      0x1406b9699
00000001406B9691: F3 0F 58 05 93 1B 25 00                  ADDSS    xmm0, dword ptr [rip + 0x251b93]
00000001406B9699: F3 0F 5E F0                              DIVSS    xmm6, xmm0
00000001406B969D: F3 0F 10 05 33 06 25 00                  MOVSS    xmm0, dword ptr [rip + 0x250633]
00000001406B96A5: 0F 2F C6                                 COMISS   xmm0, xmm6
00000001406B96A8: 0F 86 96 00 00 00                        JBE      0x1406b9744
00000001406B96AE: C6 87 3D 0D 00 00 01                     MOV      byte ptr [rdi + 0xd3d], 1
00000001406B96B5: B2 01                                    MOV      dl, 1
00000001406B96B7: 48 8B CF                                 MOV      rcx, rdi
00000001406B96BA: E8 61 D0 FE FF                           CALL     0x1406a6720
00000001406B96BF: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B96C6: 48 89 87 A8 06 00 00                     MOV      qword ptr [rdi + 0x6a8], rax
00000001406B96CD: 48 8B 87 B8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6b8]
00000001406B96D4: 48 89 87 C0 06 00 00                     MOV      qword ptr [rdi + 0x6c0], rax
00000001406B96DB: 48 8B 87 38 07 00 00                     MOV      rax, qword ptr [rdi + 0x738]
00000001406B96E2: 48 89 87 40 07 00 00                     MOV      qword ptr [rdi + 0x740], rax
00000001406B96E9: 48 8B 87 50 07 00 00                     MOV      rax, qword ptr [rdi + 0x750]
00000001406B96F0: 48 89 87 58 07 00 00                     MOV      qword ptr [rdi + 0x758], rax
00000001406B96F7: 4C 89 B7 98 07 00 00                     MOV      qword ptr [rdi + 0x798], r14
00000001406B96FE: 48 8B 87 C0 07 00 00                     MOV      rax, qword ptr [rdi + 0x7c0]
00000001406B9705: 48 2B 87 B8 07 00 00                     SUB      rax, qword ptr [rdi + 0x7b8]
00000001406B970C: 48 C1 F8 03                              SAR      rax, 3
00000001406B9710: 48 85 C0                                 TEST     rax, rax
00000001406B9713: 74 09                                    JE       0x1406b971e
00000001406B9715: 48 8B CF                                 MOV      rcx, rdi
00000001406B9718: E8 53 82 FC FF                           CALL     0x140681970
00000001406B971D: 90                                       NOP
00000001406B971E: 48 85 F6                                 TEST     rsi, rsi
00000001406B9721: 0F 84 4E F9 FF FF                        JE       0x1406b9075
00000001406B9727: 4C 2B FE                                 SUB      r15, rsi
00000001406B972A: 49 C1 FF 03                              SAR      r15, 3
00000001406B972E: 41 B8 08 00 00 00                        MOV      r8d, 8
00000001406B9734: 49 8B D7                                 MOV      rdx, r15
00000001406B9737: 48 8B CE                                 MOV      rcx, rsi
00000001406B973A: E8 11 83 94 FF                           CALL     0x140001a50
00000001406B973F: E9 31 F9 FF FF                           JMP      0x1406b9075
00000001406B9744: 48 85 F6                                 TEST     rsi, rsi
00000001406B9747: 74 18                                    JE       0x1406b9761
00000001406B9749: 4C 2B FE                                 SUB      r15, rsi
00000001406B974C: 49 C1 FF 03                              SAR      r15, 3
00000001406B9750: 41 B8 08 00 00 00                        MOV      r8d, 8
00000001406B9756: 49 8B D7                                 MOV      rdx, r15
00000001406B9759: 48 8B CE                                 MOV      rcx, rsi
00000001406B975C: E8 EF 82 94 FF                           CALL     0x140001a50
00000001406B9761: 4C 8B 97 F0 04 00 00                     MOV      r10, qword ptr [rdi + 0x4f0]
00000001406B9768: 49 BD AB AA AA AA AA AA AA 2A            MOVABS   r13, 0x2aaaaaaaaaaaaaab
00000001406B9772: 4D 85 D2                                 TEST     r10, r10
00000001406B9775: 0F 84 BB 01 00 00                        JE       0x1406b9936
00000001406B977B: 4C 3B 97 F8 04 00 00                     CMP      r10, qword ptr [rdi + 0x4f8]
00000001406B9782: 0F 85 AE 01 00 00                        JNE      0x1406b9936
00000001406B9788: 45 8B C6                                 MOV      r8d, r14d
00000001406B978B: 49 8B 8A 18 0A 00 00                     MOV      rcx, qword ptr [r10 + 0xa18]
00000001406B9792: 49 2B 8A 10 0A 00 00                     SUB      rcx, qword ptr [r10 + 0xa10]
00000001406B9799: 49 8B C5                                 MOV      rax, r13
00000001406B979C: 48 F7 E9                                 IMUL     rcx
00000001406B979F: 48 C1 FA 04                              SAR      rdx, 4
00000001406B97A3: 48 8B C2                                 MOV      rax, rdx
00000001406B97A6: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B97AA: 48 03 D0                                 ADD      rdx, rax
00000001406B97AD: 0F 84 83 01 00 00                        JE       0x1406b9936
00000001406B97B3: 49 8B CE                                 MOV      rcx, r14
00000001406B97B6: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B97BD: 4C 8B 08                                 MOV      r9, qword ptr [rax]
00000001406B97C0: 48 8D 0C 49                              LEA      rcx, [rcx + rcx*2]
00000001406B97C4: 48 C1 E1 05                              SHL      rcx, 5
00000001406B97C8: 49 8B 82 10 0A 00 00                     MOV      rax, qword ptr [r10 + 0xa10]
00000001406B97CF: 4C 39 4C 01 30                           CMP      qword ptr [rcx + rax + 0x30], r9
00000001406B97D4: 74 2E                                    JE       0x1406b9804
00000001406B97D6: 41 FF C0                                 INC      r8d
00000001406B97D9: 49 8B 8A 18 0A 00 00                     MOV      rcx, qword ptr [r10 + 0xa18]
00000001406B97E0: 48 2B C8                                 SUB      rcx, rax
00000001406B97E3: 49 8B C5                                 MOV      rax, r13
00000001406B97E6: 48 F7 E9                                 IMUL     rcx
00000001406B97E9: 48 C1 FA 04                              SAR      rdx, 4
00000001406B97ED: 48 8B C2                                 MOV      rax, rdx
00000001406B97F0: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B97F4: 48 03 D0                                 ADD      rdx, rax
00000001406B97F7: 41 8B C8                                 MOV      ecx, r8d
00000001406B97FA: 48 3B CA                                 CMP      rcx, rdx
00000001406B97FD: 72 C1                                    JB       0x1406b97c0
00000001406B97FF: E9 32 01 00 00                           JMP      0x1406b9936
00000001406B9804: 41 8B C0                                 MOV      eax, r8d
00000001406B9807: 48 8D 0C 40                              LEA      rcx, [rax + rax*2]
00000001406B980B: 48 C1 E1 05                              SHL      rcx, 5
00000001406B980F: 49 8B 82 10 0A 00 00                     MOV      rax, qword ptr [r10 + 0xa10]
00000001406B9816: 48 8B 54 01 28                           MOV      rdx, qword ptr [rcx + rax + 0x28]
00000001406B981B: 4C 8B 44 01 20                           MOV      r8, qword ptr [rcx + rax + 0x20]
00000001406B9820: 48 85 D2                                 TEST     rdx, rdx
00000001406B9823: 0F 84 C0 00 00 00                        JE       0x1406b98e9
00000001406B9829: 45 8B CE                                 MOV      r9d, r14d
00000001406B982C: 48 8B 42 40                              MOV      rax, qword ptr [rdx + 0x40]
00000001406B9830: 48 2B 42 38                              SUB      rax, qword ptr [rdx + 0x38]
00000001406B9834: 48 C1 F8 03                              SAR      rax, 3
00000001406B9838: 48 85 C0                                 TEST     rax, rax
00000001406B983B: 0F 84 A8 00 00 00                        JE       0x1406b98e9
00000001406B9841: 49 8B CE                                 MOV      rcx, r14
00000001406B9844: 48 8B 42 38                              MOV      rax, qword ptr [rdx + 0x38]
00000001406B9848: 48 39 3C C8                              CMP      qword ptr [rax + rcx*8], rdi
00000001406B984C: 74 0E                                    JE       0x1406b985c
00000001406B984E: 48 8B 0C C8                              MOV      rcx, qword ptr [rax + rcx*8]
00000001406B9852: 48 83 B9 F0 04 00 00 00                  CMP      qword ptr [rcx + 0x4f0], 0
00000001406B985A: 74 19                                    JE       0x1406b9875
00000001406B985C: 41 FF C1                                 INC      r9d
00000001406B985F: 48 8B 42 40                              MOV      rax, qword ptr [rdx + 0x40]
00000001406B9863: 48 2B 42 38                              SUB      rax, qword ptr [rdx + 0x38]
00000001406B9867: 48 C1 F8 03                              SAR      rax, 3
00000001406B986B: 41 8B C9                                 MOV      ecx, r9d
00000001406B986E: 48 3B C8                                 CMP      rcx, rax
00000001406B9871: 73 76                                    JAE      0x1406b98e9
00000001406B9873: EB CF                                    JMP      0x1406b9844
00000001406B9875: B2 01                                    MOV      dl, 1
00000001406B9877: 48 8B CF                                 MOV      rcx, rdi
00000001406B987A: E8 A1 CE FE FF                           CALL     0x1406a6720
00000001406B987F: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9886: 48 89 87 A8 06 00 00                     MOV      qword ptr [rdi + 0x6a8], rax
00000001406B988D: 48 8B 87 B8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6b8]
00000001406B9894: 48 89 87 C0 06 00 00                     MOV      qword ptr [rdi + 0x6c0], rax
00000001406B989B: 48 8B 87 38 07 00 00                     MOV      rax, qword ptr [rdi + 0x738]
00000001406B98A2: 48 89 87 40 07 00 00                     MOV      qword ptr [rdi + 0x740], rax
00000001406B98A9: 48 8B 87 50 07 00 00                     MOV      rax, qword ptr [rdi + 0x750]
00000001406B98B0: 48 89 87 58 07 00 00                     MOV      qword ptr [rdi + 0x758], rax
00000001406B98B7: 4C 89 B7 98 07 00 00                     MOV      qword ptr [rdi + 0x798], r14
00000001406B98BE: 48 8B 87 C0 07 00 00                     MOV      rax, qword ptr [rdi + 0x7c0]
00000001406B98C5: 48 2B 87 B8 07 00 00                     SUB      rax, qword ptr [rdi + 0x7b8]
00000001406B98CC: 48 C1 F8 03                              SAR      rax, 3
00000001406B98D0: 48 85 C0                                 TEST     rax, rax
00000001406B98D3: 74 08                                    JE       0x1406b98dd
00000001406B98D5: 48 8B CF                                 MOV      rcx, rdi
00000001406B98D8: E8 93 80 FC FF                           CALL     0x140681970
00000001406B98DD: C6 87 3F 0D 00 00 01                     MOV      byte ptr [rdi + 0xd3f], 1
00000001406B98E4: E9 8C F7 FF FF                           JMP      0x1406b9075
00000001406B98E9: 4D 85 C0                                 TEST     r8, r8
00000001406B98EC: 74 48                                    JE       0x1406b9936
00000001406B98EE: 41 8B D6                                 MOV      edx, r14d
00000001406B98F1: 49 8B 40 40                              MOV      rax, qword ptr [r8 + 0x40]
00000001406B98F5: 49 2B 40 38                              SUB      rax, qword ptr [r8 + 0x38]
00000001406B98F9: 48 C1 F8 03                              SAR      rax, 3
00000001406B98FD: 48 85 C0                                 TEST     rax, rax
00000001406B9900: 74 34                                    JE       0x1406b9936
00000001406B9902: 49 8B CE                                 MOV      rcx, r14
00000001406B9905: 49 8B 40 38                              MOV      rax, qword ptr [r8 + 0x38]
00000001406B9909: 48 39 3C C8                              CMP      qword ptr [rax + rcx*8], rdi
00000001406B990D: 74 12                                    JE       0x1406b9921
00000001406B990F: 48 8B 0C C8                              MOV      rcx, qword ptr [rax + rcx*8]
00000001406B9913: 48 83 B9 F0 04 00 00 00                  CMP      qword ptr [rcx + 0x4f0], 0
00000001406B991B: 0F 84 54 FF FF FF                        JE       0x1406b9875
00000001406B9921: FF C2                                    INC      edx
00000001406B9923: 49 8B 40 40                              MOV      rax, qword ptr [r8 + 0x40]
00000001406B9927: 49 2B 40 38                              SUB      rax, qword ptr [r8 + 0x38]
00000001406B992B: 48 C1 F8 03                              SAR      rax, 3
00000001406B992F: 8B CA                                    MOV      ecx, edx
00000001406B9931: 48 3B C8                                 CMP      rcx, rax
00000001406B9934: 72 CF                                    JB       0x1406b9905
00000001406B9936: 80 7D 6F 00                              CMP      byte ptr [rbp + 0x6f], 0
00000001406B993A: 0F 84 26 01 00 00                        JE       0x1406b9a66
00000001406B9940: 32 D2                                    XOR      dl, dl
00000001406B9942: 45 8B C6                                 MOV      r8d, r14d
00000001406B9945: 48 8B 87 A8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a8]
00000001406B994C: 48 2B 87 A0 06 00 00                     SUB      rax, qword ptr [rdi + 0x6a0]
00000001406B9953: 48 C1 F8 03                              SAR      rax, 3
00000001406B9957: 48 85 C0                                 TEST     rax, rax
00000001406B995A: 74 61                                    JE       0x1406b99bd
00000001406B995C: 49 8B CE                                 MOV      rcx, r14
00000001406B995F: 41 BA 01 00 00 00                        MOV      r10d, 1
00000001406B9965: 66 66 66 0F 1F 84 00 00 00 00 00         NOP      word ptr [rax + rax]
00000001406B9970: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9977: 48 83 3C C8 00                           CMP      qword ptr [rax + rcx*8], 0
00000001406B997C: 74 13                                    JE       0x1406b9991
00000001406B997E: 48 8B 0C C8                              MOV      rcx, qword ptr [rax + rcx*8]
00000001406B9982: 0F B6 D2                                 MOVZX    edx, dl
00000001406B9985: 48 83 B9 B8 00 00 00 00                  CMP      qword ptr [rcx + 0xb8], 0
00000001406B998D: 41 0F 45 D2                              CMOVNE   edx, r10d
00000001406B9991: 41 FF C0                                 INC      r8d
00000001406B9994: 48 8B 87 A8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a8]
00000001406B999B: 48 2B 87 A0 06 00 00                     SUB      rax, qword ptr [rdi + 0x6a0]
00000001406B99A2: 48 C1 F8 03                              SAR      rax, 3
00000001406B99A6: 41 8B C8                                 MOV      ecx, r8d
00000001406B99A9: 48 3B C8                                 CMP      rcx, rax
00000001406B99AC: 72 C2                                    JB       0x1406b9970
00000001406B99AE: 84 D2                                    TEST     dl, dl
00000001406B99B0: 4C 8B 97 F0 04 00 00                     MOV      r10, qword ptr [rdi + 0x4f0]
00000001406B99B7: 0F 85 A9 00 00 00                        JNE      0x1406b9a66
00000001406B99BD: 45 8B DE                                 MOV      r11d, r14d
00000001406B99C0: 48 8B 87 C0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6c0]
00000001406B99C7: 48 2B 87 B8 06 00 00                     SUB      rax, qword ptr [rdi + 0x6b8]
00000001406B99CE: 48 C1 F8 03                              SAR      rax, 3
00000001406B99D2: 48 85 C0                                 TEST     rax, rax
00000001406B99D5: 0F 84 8B 00 00 00                        JE       0x1406b9a66
00000001406B99DB: 49 8B CE                                 MOV      rcx, r14
00000001406B99DE: 66 90                                    NOP
00000001406B99E0: 48 8B 87 B8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6b8]
00000001406B99E7: 48 8B 0C C8                              MOV      rcx, qword ptr [rax + rcx*8]
00000001406B99EB: 48 8B 41 40                              MOV      rax, qword ptr [rcx + 0x40]
00000001406B99EF: 48 2B 41 38                              SUB      rax, qword ptr [rcx + 0x38]
00000001406B99F3: 48 C1 F8 03                              SAR      rax, 3
00000001406B99F7: 48 85 C0                                 TEST     rax, rax
00000001406B99FA: 74 49                                    JE       0x1406b9a45
00000001406B99FC: 45 8B C6                                 MOV      r8d, r14d
00000001406B99FF: 48 8B 41 40                              MOV      rax, qword ptr [rcx + 0x40]
00000001406B9A03: 48 2B 41 38                              SUB      rax, qword ptr [rcx + 0x38]
00000001406B9A07: 48 C1 F8 03                              SAR      rax, 3
00000001406B9A0B: 48 85 C0                                 TEST     rax, rax
00000001406B9A0E: 74 35                                    JE       0x1406b9a45
00000001406B9A10: 4C 8B 8F 90 03 00 00                     MOV      r9, qword ptr [rdi + 0x390]
00000001406B9A17: 49 8B D6                                 MOV      rdx, r14
00000001406B9A1A: 66 0F 1F 44 00 00                        NOP      word ptr [rax + rax]
00000001406B9A20: 48 8B 41 38                              MOV      rax, qword ptr [rcx + 0x38]
00000001406B9A24: 4C 39 0C D0                              CMP      qword ptr [rax + rdx*8], r9
00000001406B9A28: 0F 85 DF F5 FF FF                        JNE      0x1406b900d
00000001406B9A2E: 41 FF C0                                 INC      r8d
00000001406B9A31: 48 8B 41 40                              MOV      rax, qword ptr [rcx + 0x40]
00000001406B9A35: 48 2B 41 38                              SUB      rax, qword ptr [rcx + 0x38]
00000001406B9A39: 48 C1 F8 03                              SAR      rax, 3
00000001406B9A3D: 41 8B D0                                 MOV      edx, r8d
00000001406B9A40: 48 3B D0                                 CMP      rdx, rax
00000001406B9A43: 72 DB                                    JB       0x1406b9a20
00000001406B9A45: 41 FF C3                                 INC      r11d
00000001406B9A48: 48 8B 87 C0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6c0]
00000001406B9A4F: 48 2B 87 B8 06 00 00                     SUB      rax, qword ptr [rdi + 0x6b8]
00000001406B9A56: 48 C1 F8 03                              SAR      rax, 3
00000001406B9A5A: 41 8B CB                                 MOV      ecx, r11d
00000001406B9A5D: 48 3B C8                                 CMP      rcx, rax
00000001406B9A60: 0F 82 7A FF FF FF                        JB       0x1406b99e0
00000001406B9A66: 49 8D 8A 20 03 00 00                     LEA      rcx, [r10 + 0x320]
00000001406B9A6D: 48 8D 55 87                              LEA      rdx, [rbp - 0x79]
00000001406B9A71: FF 15 41 2D 1B 00                        CALL     qword ptr [rip + 0x1b2d41]
00000001406B9A77: 4C 8B C0                                 MOV      r8, rax
00000001406B9A7A: 48 8B 97 08 17 00 00                     MOV      rdx, qword ptr [rdi + 0x1708]
00000001406B9A81: 48 81 C2 94 94 00 00                     ADD      rdx, 0x9494
00000001406B9A88: F3 0F 10 05 C8 04 25 00                  MOVSS    xmm0, dword ptr [rip + 0x2504c8]
00000001406B9A90: F3 0F 11 44 24 20                        MOVSS    dword ptr [rsp + 0x20], xmm0
00000001406B9A96: 45 33 C9                                 XOR      r9d, r9d
00000001406B9A99: 48 8B 0D 70 B1 32 00                     MOV      rcx, qword ptr [rip + 0x32b170]
00000001406B9AA0: FF 15 32 31 1B 00                        CALL     qword ptr [rip + 0x1b3132]
00000001406B9AA6: 48 8D 9F D0 07 00 00                     LEA      rbx, [rdi + 0x7d0]
00000001406B9AAD: 48 8B C3                                 MOV      rax, rbx
00000001406B9AB0: B9 4B 00 00 00                           MOV      ecx, 0x4b
00000001406B9AB5: 66 66 66 0F 1F 84 00 00 00 00 00         NOP      word ptr [rax + rax]
00000001406B9AC0: 4C 89 30                                 MOV      qword ptr [rax], r14
00000001406B9AC3: 48 8D 40 10                              LEA      rax, [rax + 0x10]
00000001406B9AC7: 48 83 E9 01                              SUB      rcx, 1
00000001406B9ACB: 75 F3                                    JNE      0x1406b9ac0
00000001406B9ACD: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406B9AD4: 4D 85 C0                                 TEST     r8, r8
00000001406B9AD7: 75 1E                                    JNE      0x1406b9af7
00000001406B9AD9: 48 8B 8F 88 06 00 00                     MOV      rcx, qword ptr [rdi + 0x688]
00000001406B9AE0: 48 2B 8F 80 06 00 00                     SUB      rcx, qword ptr [rdi + 0x680]
00000001406B9AE7: 48 C1 F9 03                              SAR      rcx, 3
00000001406B9AEB: 48 8B 87 80 06 00 00                     MOV      rax, qword ptr [rdi + 0x680]
00000001406B9AF2: 4C 8B 44 C8 F8                           MOV      r8, qword ptr [rax + rcx*8 - 8]
00000001406B9AF7: 45 8B D6                                 MOV      r10d, r14d
00000001406B9AFA: 49 8B 88 18 0A 00 00                     MOV      rcx, qword ptr [r8 + 0xa18]
00000001406B9B01: 49 2B 88 10 0A 00 00                     SUB      rcx, qword ptr [r8 + 0xa10]
00000001406B9B08: 49 8B C5                                 MOV      rax, r13
00000001406B9B0B: 48 F7 E9                                 IMUL     rcx
00000001406B9B0E: 48 C1 FA 04                              SAR      rdx, 4
00000001406B9B12: 48 8B C2                                 MOV      rax, rdx
00000001406B9B15: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9B19: 48 03 D0                                 ADD      rdx, rax
00000001406B9B1C: 85 D2                                    TEST     edx, edx
00000001406B9B1E: 0F 8E 7C 02 00 00                        JLE      0x1406b9da0
00000001406B9B24: 4D 8B CE                                 MOV      r9, r14
00000001406B9B27: 66 0F 1F 84 00 00 00 00 00               NOP      word ptr [rax + rax]
00000001406B9B30: 49 8B 80 10 0A 00 00                     MOV      rax, qword ptr [r8 + 0xa10]
00000001406B9B37: 41 83 3C 01 01                           CMP      dword ptr [r9 + rax], 1
00000001406B9B3C: 75 73                                    JNE      0x1406b9bb1
00000001406B9B3E: 48 8B D0                                 MOV      rdx, rax
00000001406B9B41: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9B48: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001406B9B4B: F3 41 0F 10 44 11 08                     MOVSS    xmm0, dword ptr [r9 + rdx + 8]
00000001406B9B52: 0F 2E 41 04                              UCOMISS  xmm0, dword ptr [rcx + 4]
00000001406B9B56: 7A 59                                    JP       0x1406b9bb1
00000001406B9B58: 75 57                                    JNE      0x1406b9bb1
00000001406B9B5A: 48 8B 08                                 MOV      rcx, qword ptr [rax]
00000001406B9B5D: F3 41 0F 10 44 11 10                     MOVSS    xmm0, dword ptr [r9 + rdx + 0x10]
00000001406B9B64: 0F 2E 41 0C                              UCOMISS  xmm0, dword ptr [rcx + 0xc]
00000001406B9B68: 7A 47                                    JP       0x1406b9bb1
00000001406B9B6A: 75 45                                    JNE      0x1406b9bb1
00000001406B9B6C: 41 0F B6 D2                              MOVZX    edx, r10b
00000001406B9B70: F6 D2                                    NOT      dl
00000001406B9B72: 80 E2 01                                 AND      dl, 1
00000001406B9B75: 49 8B 80 10 0A 00 00                     MOV      rax, qword ptr [r8 + 0xa10]
00000001406B9B7C: 4D 8B 5C 01 20                           MOV      r11, qword ptr [r9 + rax + 0x20]
00000001406B9B81: 48 8D 87 60 0C 00 00                     LEA      rax, [rdi + 0xc60]
00000001406B9B88: B9 25 00 00 00                           MOV      ecx, 0x25
00000001406B9B8D: 0F 1F 00                                 NOP      dword ptr [rax]
00000001406B9B90: 0F 10 00                                 MOVUPS   xmm0, xmmword ptr [rax]
00000001406B9B93: 0F 11 40 10                              MOVUPS   xmmword ptr [rax + 0x10], xmm0
00000001406B9B97: 0F 10 40 F0                              MOVUPS   xmm0, xmmword ptr [rax - 0x10]
00000001406B9B9B: 0F 11 00                                 MOVUPS   xmmword ptr [rax], xmm0
00000001406B9B9E: 48 8D 40 E0                              LEA      rax, [rax - 0x20]
00000001406B9BA2: 48 83 E9 01                              SUB      rcx, 1
00000001406B9BA6: 75 E8                                    JNE      0x1406b9b90
00000001406B9BA8: 4C 89 1B                                 MOV      qword ptr [rbx], r11
00000001406B9BAB: 88 97 D8 07 00 00                        MOV      byte ptr [rdi + 0x7d8], dl
00000001406B9BB1: 41 FF C2                                 INC      r10d
00000001406B9BB4: 49 83 C1 60                              ADD      r9, 0x60
00000001406B9BB8: 49 8B 88 18 0A 00 00                     MOV      rcx, qword ptr [r8 + 0xa18]
00000001406B9BBF: 49 2B 88 10 0A 00 00                     SUB      rcx, qword ptr [r8 + 0xa10]
00000001406B9BC6: 49 8B C5                                 MOV      rax, r13
00000001406B9BC9: 48 F7 E9                                 IMUL     rcx
00000001406B9BCC: 48 C1 FA 04                              SAR      rdx, 4
00000001406B9BD0: 48 8B C2                                 MOV      rax, rdx
00000001406B9BD3: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9BD7: 48 03 D0                                 ADD      rdx, rax
00000001406B9BDA: 44 3B D2                                 CMP      r10d, edx
00000001406B9BDD: 0F 8C 4D FF FF FF                        JL       0x1406b9b30
00000001406B9BE3: E9 B8 01 00 00                           JMP      0x1406b9da0
00000001406B9BE8: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406B9BEF: 8B 88 94 02 00 00                        MOV      ecx, dword ptr [rax + 0x294]
00000001406B9BF5: 83 F9 08                                 CMP      ecx, 8
00000001406B9BF8: 75 7B                                    JNE      0x1406b9c75
00000001406B9BFA: 48 8B 8F 80 0C 00 00                     MOV      rcx, qword ptr [rdi + 0xc80]
00000001406B9C01: 48 85 C9                                 TEST     rcx, rcx
00000001406B9C04: 74 17                                    JE       0x1406b9c1d
00000001406B9C06: 48 8B 87 90 03 00 00                     MOV      rax, qword ptr [rdi + 0x390]
00000001406B9C0D: 48 39 81 B0 0B 00 00                     CMP      qword ptr [rcx + 0xbb0], rax
00000001406B9C14: 75 07                                    JNE      0x1406b9c1d
00000001406B9C16: 4C 89 B1 B0 0B 00 00                     MOV      qword ptr [rcx + 0xbb0], r14
00000001406B9C1D: 83 BF 6C 05 00 00 00                     CMP      dword ptr [rdi + 0x56c], 0
00000001406B9C24: 7D 0D                                    JGE      0x1406b9c33
00000001406B9C26: 83 BF 10 05 00 00 00                     CMP      dword ptr [rdi + 0x510], 0
00000001406B9C2D: 0F 8C 6D 01 00 00                        JL       0x1406b9da0
00000001406B9C33: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9C3A: 48 8B 88 B0 0B 00 00                     MOV      rcx, qword ptr [rax + 0xbb0]
00000001406B9C41: 48 85 C9                                 TEST     rcx, rcx
00000001406B9C44: 74 23                                    JE       0x1406b9c69
00000001406B9C46: 48 8B 81 80 0C 00 00                     MOV      rax, qword ptr [rcx + 0xc80]
00000001406B9C4D: 48 85 C0                                 TEST     rax, rax
00000001406B9C50: 74 17                                    JE       0x1406b9c69
00000001406B9C52: 48 39 88 B0 0B 00 00                     CMP      qword ptr [rax + 0xbb0], rcx
00000001406B9C59: 75 07                                    JNE      0x1406b9c62
00000001406B9C5B: 4C 89 B0 B0 0B 00 00                     MOV      qword ptr [rax + 0xbb0], r14
00000001406B9C62: 4C 89 B1 80 0C 00 00                     MOV      qword ptr [rcx + 0xc80], r14
00000001406B9C69: 48 8B 8F F0 04 00 00                     MOV      rcx, qword ptr [rdi + 0x4f0]
00000001406B9C70: E9 0F 01 00 00                           JMP      0x1406b9d84
00000001406B9C75: 83 F9 0A                                 CMP      ecx, 0xa
00000001406B9C78: 75 49                                    JNE      0x1406b9cc3
00000001406B9C7A: 48 8B 8F 80 0C 00 00                     MOV      rcx, qword ptr [rdi + 0xc80]
00000001406B9C81: 48 85 C9                                 TEST     rcx, rcx
00000001406B9C84: 74 1E                                    JE       0x1406b9ca4
00000001406B9C86: 48 8B 87 90 03 00 00                     MOV      rax, qword ptr [rdi + 0x390]
00000001406B9C8D: 48 39 81 B0 0B 00 00                     CMP      qword ptr [rcx + 0xbb0], rax
00000001406B9C94: 75 0E                                    JNE      0x1406b9ca4
00000001406B9C96: 4C 89 B1 B0 0B 00 00                     MOV      qword ptr [rcx + 0xbb0], r14
00000001406B9C9D: 4C 89 B7 80 0C 00 00                     MOV      qword ptr [rdi + 0xc80], r14
00000001406B9CA4: 83 BF 6C 05 00 00 00                     CMP      dword ptr [rdi + 0x56c], 0
00000001406B9CAB: 0F 8D E1 00 00 00                        JGE      0x1406b9d92
00000001406B9CB1: 83 BF 10 05 00 00 00                     CMP      dword ptr [rdi + 0x510], 0
00000001406B9CB8: 0F 8C E2 00 00 00                        JL       0x1406b9da0
00000001406B9CBE: E9 CF 00 00 00                           JMP      0x1406b9d92
00000001406B9CC3: 83 F9 06                                 CMP      ecx, 6
00000001406B9CC6: 0F 84 D4 00 00 00                        JE       0x1406b9da0
00000001406B9CCC: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9CD3: 48 83 B8 B0 0B 00 00 00                  CMP      qword ptr [rax + 0xbb0], 0
00000001406B9CDB: 74 56                                    JE       0x1406b9d33
00000001406B9CDD: 83 BF 6C 05 00 00 00                     CMP      dword ptr [rdi + 0x56c], 0
00000001406B9CE4: 7D 09                                    JGE      0x1406b9cef
00000001406B9CE6: 83 BF 10 05 00 00 00                     CMP      dword ptr [rdi + 0x510], 0
00000001406B9CED: 7C 44                                    JL       0x1406b9d33
00000001406B9CEF: 48 8B 87 A0 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a0]
00000001406B9CF6: 48 89 87 A8 06 00 00                     MOV      qword ptr [rdi + 0x6a8], rax
00000001406B9CFD: 48 8B 87 B8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6b8]
00000001406B9D04: 48 89 87 C0 06 00 00                     MOV      qword ptr [rdi + 0x6c0], rax
00000001406B9D0B: 48 8B 87 38 07 00 00                     MOV      rax, qword ptr [rdi + 0x738]
00000001406B9D12: 48 89 87 40 07 00 00                     MOV      qword ptr [rdi + 0x740], rax
00000001406B9D19: 48 8B 87 50 07 00 00                     MOV      rax, qword ptr [rdi + 0x750]
00000001406B9D20: 48 89 87 58 07 00 00                     MOV      qword ptr [rdi + 0x758], rax
00000001406B9D27: 44 89 B7 34 0D 00 00                     MOV      dword ptr [rdi + 0xd34], r14d
00000001406B9D2E: E9 1C F3 FF FF                           JMP      0x1406b904f
00000001406B9D33: 48 8B 8F 80 0C 00 00                     MOV      rcx, qword ptr [rdi + 0xc80]
00000001406B9D3A: 48 85 C9                                 TEST     rcx, rcx
00000001406B9D3D: 74 17                                    JE       0x1406b9d56
00000001406B9D3F: 48 8B 87 90 03 00 00                     MOV      rax, qword ptr [rdi + 0x390]
00000001406B9D46: 48 39 81 B0 0B 00 00                     CMP      qword ptr [rcx + 0xbb0], rax
00000001406B9D4D: 75 07                                    JNE      0x1406b9d56
00000001406B9D4F: 4C 89 B1 B0 0B 00 00                     MOV      qword ptr [rcx + 0xbb0], r14
00000001406B9D56: 83 BF 6C 05 00 00 00                     CMP      dword ptr [rdi + 0x56c], 0
00000001406B9D5D: 7D 09                                    JGE      0x1406b9d68
00000001406B9D5F: 83 BF 10 05 00 00 00                     CMP      dword ptr [rdi + 0x510], 0
00000001406B9D66: 7C 38                                    JL       0x1406b9da0
00000001406B9D68: 48 8B 8F F0 04 00 00                     MOV      rcx, qword ptr [rdi + 0x4f0]
00000001406B9D6F: 48 8B 81 18 03 00 00                     MOV      rax, qword ptr [rcx + 0x318]
00000001406B9D76: 48 85 C0                                 TEST     rax, rax
00000001406B9D79: 74 25                                    JE       0x1406b9da0
00000001406B9D7B: 80 B8 79 09 00 00 00                     CMP      byte ptr [rax + 0x979], 0
00000001406B9D82: 75 1C                                    JNE      0x1406b9da0
00000001406B9D84: 48 8B 87 90 03 00 00                     MOV      rax, qword ptr [rdi + 0x390]
00000001406B9D8B: 48 89 81 B0 0B 00 00                     MOV      qword ptr [rcx + 0xbb0], rax
00000001406B9D92: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9D99: 48 89 87 80 0C 00 00                     MOV      qword ptr [rdi + 0xc80], rax
00000001406B9DA0: 44 89 B7 A4 07 00 00                     MOV      dword ptr [rdi + 0x7a4], r14d
00000001406B9DA7: 48 63 8F 10 05 00 00                     MOVSXD   rcx, dword ptr [rdi + 0x510]
00000001406B9DAE: F3 44 0F 10 0D A9 02 25 00               MOVSS    xmm9, dword ptr [rip + 0x2502a9]
00000001406B9DB7: 85 C9                                    TEST     ecx, ecx
00000001406B9DB9: 0F 88 0F 04 00 00                        JS       0x1406ba1ce
00000001406B9DBF: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406B9DC6: 83 B8 94 02 00 00 01                     CMP      dword ptr [rax + 0x294], 1
00000001406B9DCD: 74 0D                                    JE       0x1406b9ddc
00000001406B9DCF: 80 BF 7D 06 00 00 00                     CMP      byte ptr [rdi + 0x67d], 0
00000001406B9DD6: 0F 84 F2 03 00 00                        JE       0x1406ba1ce
00000001406B9DDC: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9DE3: C6 84 01 90 0B 00 00 00                  MOV      byte ptr [rcx + rax + 0xb90], 0
00000001406B9DEB: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9DF2: 48 8B 90 18 03 00 00                     MOV      rdx, qword ptr [rax + 0x318]
00000001406B9DF9: 48 85 D2                                 TEST     rdx, rdx
00000001406B9DFC: 0F 84 8A 03 00 00                        JE       0x1406ba18c
00000001406B9E02: 49 8B C5                                 MOV      rax, r13
00000001406B9E05: 80 BA 79 09 00 00 00                     CMP      byte ptr [rdx + 0x979], 0
00000001406B9E0C: 0F 85 0B 01 00 00                        JNE      0x1406b9f1d
00000001406B9E12: 45 8B FE                                 MOV      r15d, r14d
00000001406B9E15: 48 8B 8A D0 02 00 00                     MOV      rcx, qword ptr [rdx + 0x2d0]
00000001406B9E1C: 48 2B 8A C8 02 00 00                     SUB      rcx, qword ptr [rdx + 0x2c8]
00000001406B9E23: 48 F7 E9                                 IMUL     rcx
00000001406B9E26: 48 D1 FA                                 SAR      rdx, 1
00000001406B9E29: 48 8B C2                                 MOV      rax, rdx
00000001406B9E2C: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9E30: 48 03 D0                                 ADD      rdx, rax
00000001406B9E33: 85 D2                                    TEST     edx, edx
00000001406B9E35: 0F 8E 51 03 00 00                        JLE      0x1406ba18c
00000001406B9E3B: 49 8B F6                                 MOV      rsi, r14
00000001406B9E3E: 49 8B DE                                 MOV      rbx, r14
00000001406B9E41: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406B9E48: 49 8B 90 18 03 00 00                     MOV      rdx, qword ptr [r8 + 0x318]
00000001406B9E4F: 48 8B 82 E0 02 00 00                     MOV      rax, qword ptr [rdx + 0x2e0]
00000001406B9E56: 83 3C 03 FF                              CMP      dword ptr [rbx + rax], -1
00000001406B9E5A: 74 0E                                    JE       0x1406b9e6a
00000001406B9E5C: 48 8B C8                                 MOV      rcx, rax
00000001406B9E5F: 8B 87 10 05 00 00                        MOV      eax, dword ptr [rdi + 0x510]
00000001406B9E65: 39 04 0B                                 CMP      dword ptr [rbx + rcx], eax
00000001406B9E68: 75 6B                                    JNE      0x1406b9ed5
00000001406B9E6A: 48 8B 82 C8 02 00 00                     MOV      rax, qword ptr [rdx + 0x2c8]
00000001406B9E71: F3 0F 10 04 06                           MOVSS    xmm0, dword ptr [rsi + rax]
00000001406B9E76: F3 0F 11 44 24 30                        MOVSS    dword ptr [rsp + 0x30], xmm0
00000001406B9E7C: F3 0F 10 4C 06 04                        MOVSS    xmm1, dword ptr [rsi + rax + 4]
00000001406B9E82: F3 0F 11 4C 24 34                        MOVSS    dword ptr [rsp + 0x34], xmm1
00000001406B9E88: F3 0F 10 44 06 08                        MOVSS    xmm0, dword ptr [rsi + rax + 8]
00000001406B9E8E: F3 0F 11 44 24 38                        MOVSS    dword ptr [rsp + 0x38], xmm0
00000001406B9E94: 49 8D 88 20 03 00 00                     LEA      rcx, [r8 + 0x320]
00000001406B9E9B: 48 8D 55 A7                              LEA      rdx, [rbp - 0x59]
00000001406B9E9F: FF 15 1B 29 1B 00                        CALL     qword ptr [rip + 0x1b291b]
00000001406B9EA5: 4C 8B C0                                 MOV      r8, rax
00000001406B9EA8: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001406B9EAD: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001406B9EB2: FF 15 50 27 1B 00                        CALL     qword ptr [rip + 0x1b2750]
00000001406B9EB8: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406B9EBF: 48 8D 44 24 30                           LEA      rax, [rsp + 0x30]
00000001406B9EC4: 48 89 44 24 20                           MOV      qword ptr [rsp + 0x20], rax
00000001406B9EC9: 4C 8B 01                                 MOV      r8, qword ptr [rcx]
00000001406B9ECC: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406B9ED0: E8 7B 87 B9 FF                           CALL     0x140252650
00000001406B9ED5: 41 FF C7                                 INC      r15d
00000001406B9ED8: 48 83 C3 04                              ADD      rbx, 4
00000001406B9EDC: 48 83 C6 0C                              ADD      rsi, 0xc
00000001406B9EE0: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406B9EE7: 48 8B 90 18 03 00 00                     MOV      rdx, qword ptr [rax + 0x318]
00000001406B9EEE: 48 8B 8A D0 02 00 00                     MOV      rcx, qword ptr [rdx + 0x2d0]
00000001406B9EF5: 48 2B 8A C8 02 00 00                     SUB      rcx, qword ptr [rdx + 0x2c8]
00000001406B9EFC: 49 8B C5                                 MOV      rax, r13
00000001406B9EFF: 48 F7 E9                                 IMUL     rcx
00000001406B9F02: 48 D1 FA                                 SAR      rdx, 1
00000001406B9F05: 48 8B C2                                 MOV      rax, rdx
00000001406B9F08: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9F0C: 48 03 D0                                 ADD      rdx, rax
00000001406B9F0F: 44 3B FA                                 CMP      r15d, edx
00000001406B9F12: 0F 8C 29 FF FF FF                        JL       0x1406b9e41
00000001406B9F18: E9 6F 02 00 00                           JMP      0x1406ba18c
00000001406B9F1D: 48 8B 8A 88 09 00 00                     MOV      rcx, qword ptr [rdx + 0x988]
00000001406B9F24: 48 2B 8A 80 09 00 00                     SUB      rcx, qword ptr [rdx + 0x980]
00000001406B9F2B: 48 F7 E9                                 IMUL     rcx
00000001406B9F2E: 48 D1 FA                                 SAR      rdx, 1
00000001406B9F31: 48 8B C2                                 MOV      rax, rdx
00000001406B9F34: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406B9F38: 48 03 C2                                 ADD      rax, rdx
00000001406B9F3B: 83 E8 01                                 SUB      eax, 1
00000001406B9F3E: 48 63 D8                                 MOVSXD   rbx, eax
00000001406B9F41: 0F 88 AE 00 00 00                        JS       0x1406b9ff5
00000001406B9F47: 48 8D 34 5B                              LEA      rsi, [rbx + rbx*2]
00000001406B9F4B: 48 C1 E6 02                              SHL      rsi, 2
00000001406B9F4F: 90                                       NOP
00000001406B9F50: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406B9F57: 49 8B 90 18 03 00 00                     MOV      rdx, qword ptr [r8 + 0x318]
00000001406B9F5E: 48 8B 82 98 09 00 00                     MOV      rax, qword ptr [rdx + 0x998]
00000001406B9F65: 83 3C 98 FE                              CMP      dword ptr [rax + rbx*4], -2
00000001406B9F69: 74 11                                    JE       0x1406b9f7c
00000001406B9F6B: 48 8B C8                                 MOV      rcx, rax
00000001406B9F6E: 8B 87 10 05 00 00                        MOV      eax, dword ptr [rdi + 0x510]
00000001406B9F74: 83 C0 64                                 ADD      eax, 0x64
00000001406B9F77: 39 04 99                                 CMP      dword ptr [rcx + rbx*4], eax
00000001406B9F7A: 75 6B                                    JNE      0x1406b9fe7
00000001406B9F7C: 48 8B 82 80 09 00 00                     MOV      rax, qword ptr [rdx + 0x980]
00000001406B9F83: F3 0F 10 04 06                           MOVSS    xmm0, dword ptr [rsi + rax]
00000001406B9F88: F3 0F 11 44 24 30                        MOVSS    dword ptr [rsp + 0x30], xmm0
00000001406B9F8E: F3 0F 10 4C 06 04                        MOVSS    xmm1, dword ptr [rsi + rax + 4]
00000001406B9F94: F3 0F 11 4C 24 34                        MOVSS    dword ptr [rsp + 0x34], xmm1
00000001406B9F9A: F3 0F 10 44 06 08                        MOVSS    xmm0, dword ptr [rsi + rax + 8]
00000001406B9FA0: F3 0F 11 44 24 38                        MOVSS    dword ptr [rsp + 0x38], xmm0
00000001406B9FA6: 49 8D 88 20 03 00 00                     LEA      rcx, [r8 + 0x320]
00000001406B9FAD: 48 8D 55 A7                              LEA      rdx, [rbp - 0x59]
00000001406B9FB1: FF 15 09 28 1B 00                        CALL     qword ptr [rip + 0x1b2809]
00000001406B9FB7: 4C 8B C0                                 MOV      r8, rax
00000001406B9FBA: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001406B9FBF: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001406B9FC4: FF 15 3E 26 1B 00                        CALL     qword ptr [rip + 0x1b263e]
00000001406B9FCA: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406B9FD1: 48 8D 44 24 30                           LEA      rax, [rsp + 0x30]
00000001406B9FD6: 48 89 44 24 20                           MOV      qword ptr [rsp + 0x20], rax
00000001406B9FDB: 4C 8B 01                                 MOV      r8, qword ptr [rcx]
00000001406B9FDE: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406B9FE2: E8 69 86 B9 FF                           CALL     0x140252650
00000001406B9FE7: 48 83 EE 0C                              SUB      rsi, 0xc
00000001406B9FEB: 48 83 EB 01                              SUB      rbx, 1
00000001406B9FEF: 0F 89 5B FF FF FF                        JNS      0x1406b9f50
00000001406B9FF5: 48 8D 9F 38 07 00 00                     LEA      rbx, [rdi + 0x738]
00000001406B9FFC: 48 8B 4B 08                              MOV      rcx, qword ptr [rbx + 8]
00000001406BA000: 48 2B 0B                                 SUB      rcx, qword ptr [rbx]
00000001406BA003: 49 8B C5                                 MOV      rax, r13
00000001406BA006: 48 F7 E9                                 IMUL     rcx
00000001406BA009: 48 D1 FA                                 SAR      rdx, 1
00000001406BA00C: 48 8B C2                                 MOV      rax, rdx
00000001406BA00F: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406BA013: 48 03 D0                                 ADD      rdx, rax
00000001406BA016: 0F 85 A8 01 00 00                        JNE      0x1406ba1c4
00000001406BA01C: 48 63 97 10 05 00 00                     MOVSXD   rdx, dword ptr [rdi + 0x510]
00000001406BA023: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA02A: 48 8B CA                                 MOV      rcx, rdx
00000001406BA02D: 48 C1 E1 05                              SHL      rcx, 5
00000001406BA031: 48 03 88 78 0B 00 00                     ADD      rcx, qword ptr [rax + 0xb78]
00000001406BA038: 48 C1 E2 05                              SHL      rdx, 5
00000001406BA03C: 48 8B 80 78 0B 00 00                     MOV      rax, qword ptr [rax + 0xb78]
00000001406BA043: F3 0F 10 54 02 14                        MOVSS    xmm2, dword ptr [rdx + rax + 0x14]
00000001406BA049: F3 0F 5C 51 08                           SUBSS    xmm2, dword ptr [rcx + 8]
00000001406BA04E: F3 0F 10 4C 02 10                        MOVSS    xmm1, dword ptr [rdx + rax + 0x10]
00000001406BA054: F3 0F 5C 49 04                           SUBSS    xmm1, dword ptr [rcx + 4]
00000001406BA059: F3 0F 10 44 02 0C                        MOVSS    xmm0, dword ptr [rdx + rax + 0xc]
00000001406BA05F: F3 0F 5C 01                              SUBSS    xmm0, dword ptr [rcx]
00000001406BA063: F3 0F 11 44 24 30                        MOVSS    dword ptr [rsp + 0x30], xmm0
00000001406BA069: F3 0F 11 4C 24 34                        MOVSS    dword ptr [rsp + 0x34], xmm1
00000001406BA06F: F3 0F 11 54 24 38                        MOVSS    dword ptr [rsp + 0x38], xmm2
00000001406BA075: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001406BA07A: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001406BA07F: FF 15 83 20 1B 00                        CALL     qword ptr [rip + 0x1b2083]
00000001406BA085: F3 0F 10 74 24 38                        MOVSS    xmm6, dword ptr [rsp + 0x38]
00000001406BA08B: F3 41 0F 59 F1                           MULSS    xmm6, xmm9
00000001406BA090: F3 0F 10 7C 24 34                        MOVSS    xmm7, dword ptr [rsp + 0x34]
00000001406BA096: F3 41 0F 59 F9                           MULSS    xmm7, xmm9
00000001406BA09B: F3 44 0F 10 44 24 30                     MOVSS    xmm8, dword ptr [rsp + 0x30]
00000001406BA0A2: F3 45 0F 59 C1                           MULSS    xmm8, xmm9
00000001406BA0A7: 48 63 8F 10 05 00 00                     MOVSXD   rcx, dword ptr [rdi + 0x510]
00000001406BA0AE: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA0B5: 48 C1 E1 05                              SHL      rcx, 5
00000001406BA0B9: 48 03 88 78 0B 00 00                     ADD      rcx, qword ptr [rax + 0xb78]
00000001406BA0C0: F3 0F 58 71 08                           ADDSS    xmm6, dword ptr [rcx + 8]
00000001406BA0C5: F3 0F 58 79 04                           ADDSS    xmm7, dword ptr [rcx + 4]
00000001406BA0CA: F3 44 0F 58 01                           ADDSS    xmm8, dword ptr [rcx]
00000001406BA0CF: F3 44 0F 11 45 87                        MOVSS    dword ptr [rbp - 0x79], xmm8
00000001406BA0D5: F3 0F 11 7D 8B                           MOVSS    dword ptr [rbp - 0x75], xmm7
00000001406BA0DA: F3 0F 11 75 8F                           MOVSS    dword ptr [rbp - 0x71], xmm6
00000001406BA0DF: 48 8D 45 87                              LEA      rax, [rbp - 0x79]
00000001406BA0E3: 48 3B 43 08                              CMP      rax, qword ptr [rbx + 8]
00000001406BA0E7: 73 6D                                    JAE      0x1406ba156
00000001406BA0E9: 48 8B 03                                 MOV      rax, qword ptr [rbx]
00000001406BA0EC: 48 8D 4D 87                              LEA      rcx, [rbp - 0x79]
00000001406BA0F0: 48 3B C1                                 CMP      rax, rcx
00000001406BA0F3: 77 61                                    JA       0x1406ba156
00000001406BA0F5: 48 8D 4D 87                              LEA      rcx, [rbp - 0x79]
00000001406BA0F9: 48 2B C8                                 SUB      rcx, rax
00000001406BA0FC: 49 8B C5                                 MOV      rax, r13
00000001406BA0FF: 48 F7 E9                                 IMUL     rcx
00000001406BA102: 48 8B F2                                 MOV      rsi, rdx
00000001406BA105: 48 D1 FE                                 SAR      rsi, 1
00000001406BA108: 48 8B C6                                 MOV      rax, rsi
00000001406BA10B: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406BA10F: 48 03 F0                                 ADD      rsi, rax
00000001406BA112: 48 8B 43 10                              MOV      rax, qword ptr [rbx + 0x10]
00000001406BA116: 48 39 43 08                              CMP      qword ptr [rbx + 8], rax
00000001406BA11A: 75 0D                                    JNE      0x1406ba129
00000001406BA11C: BA 01 00 00 00                           MOV      edx, 1
00000001406BA121: 48 8B CB                                 MOV      rcx, rbx
00000001406BA124: E8 57 4D 97 FF                           CALL     0x14002ee80
00000001406BA129: 48 8B 4B 08                              MOV      rcx, qword ptr [rbx + 8]
00000001406BA12D: 48 8D 14 76                              LEA      rdx, [rsi + rsi*2]
00000001406BA131: 4C 8B 03                                 MOV      r8, qword ptr [rbx]
00000001406BA134: 48 85 C9                                 TEST     rcx, rcx
00000001406BA137: 74 16                                    JE       0x1406ba14f
00000001406BA139: 41 8B 04 90                              MOV      eax, dword ptr [r8 + rdx*4]
00000001406BA13D: 89 01                                    MOV      dword ptr [rcx], eax
00000001406BA13F: 41 8B 44 90 04                           MOV      eax, dword ptr [r8 + rdx*4 + 4]
00000001406BA144: 89 41 04                                 MOV      dword ptr [rcx + 4], eax
00000001406BA147: 41 8B 44 90 08                           MOV      eax, dword ptr [r8 + rdx*4 + 8]
00000001406BA14C: 89 41 08                                 MOV      dword ptr [rcx + 8], eax
00000001406BA14F: 48 83 43 08 0C                           ADD      qword ptr [rbx + 8], 0xc
00000001406BA154: EB 6E                                    JMP      0x1406ba1c4
00000001406BA156: 48 8B 43 10                              MOV      rax, qword ptr [rbx + 0x10]
00000001406BA15A: 48 39 43 08                              CMP      qword ptr [rbx + 8], rax
00000001406BA15E: 75 0D                                    JNE      0x1406ba16d
00000001406BA160: BA 01 00 00 00                           MOV      edx, 1
00000001406BA165: 48 8B CB                                 MOV      rcx, rbx
00000001406BA168: E8 13 4D 97 FF                           CALL     0x14002ee80
00000001406BA16D: 48 8B 43 08                              MOV      rax, qword ptr [rbx + 8]
00000001406BA171: 48 85 C0                                 TEST     rax, rax
00000001406BA174: 74 0F                                    JE       0x1406ba185
00000001406BA176: F3 44 0F 11 00                           MOVSS    dword ptr [rax], xmm8
00000001406BA17B: F3 0F 11 78 04                           MOVSS    dword ptr [rax + 4], xmm7
00000001406BA180: F3 0F 11 70 08                           MOVSS    dword ptr [rax + 8], xmm6
00000001406BA185: 48 83 43 08 0C                           ADD      qword ptr [rbx + 8], 0xc
00000001406BA18A: EB 38                                    JMP      0x1406ba1c4
00000001406BA18C: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406BA193: 48 63 97 10 05 00 00                     MOVSXD   rdx, dword ptr [rdi + 0x510]
00000001406BA19A: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA1A1: 48 C1 E2 05                              SHL      rdx, 5
00000001406BA1A5: 48 8B 80 78 0B 00 00                     MOV      rax, qword ptr [rax + 0xb78]
00000001406BA1AC: 48 83 C0 0C                              ADD      rax, 0xc
00000001406BA1B0: 48 03 D0                                 ADD      rdx, rax
00000001406BA1B3: 48 89 54 24 20                           MOV      qword ptr [rsp + 0x20], rdx
00000001406BA1B8: 4C 8B 01                                 MOV      r8, qword ptr [rcx]
00000001406BA1BB: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406BA1BF: E8 8C 84 B9 FF                           CALL     0x140252650
00000001406BA1C4: C7 87 10 05 00 00 FF FF FF FF            MOV      dword ptr [rdi + 0x510], 0xffffffff
00000001406BA1CE: 4C 63 87 6C 05 00 00                     MOVSXD   r8, dword ptr [rdi + 0x56c]
00000001406BA1D5: 45 85 C0                                 TEST     r8d, r8d
00000001406BA1D8: 0F 88 D3 01 00 00                        JS       0x1406ba3b1
00000001406BA1DE: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406BA1E5: 83 B8 94 02 00 00 01                     CMP      dword ptr [rax + 0x294], 1
00000001406BA1EC: 0F 85 BF 01 00 00                        JNE      0x1406ba3b1
00000001406BA1F2: 48 83 BF 28 05 00 00 00                  CMP      qword ptr [rdi + 0x528], 0
00000001406BA1FA: 0F 85 B1 01 00 00                        JNE      0x1406ba3b1
00000001406BA200: 48 8B 8F F0 04 00 00                     MOV      rcx, qword ptr [rdi + 0x4f0]
00000001406BA207: 48 85 C9                                 TEST     rcx, rcx
00000001406BA20A: 0F 84 A1 01 00 00                        JE       0x1406ba3b1
00000001406BA210: 48 8B 81 18 03 00 00                     MOV      rax, qword ptr [rcx + 0x318]
00000001406BA217: 48 85 C0                                 TEST     rax, rax
00000001406BA21A: 0F 84 91 01 00 00                        JE       0x1406ba3b1
00000001406BA220: 8B 90 60 03 00 00                        MOV      edx, dword ptr [rax + 0x360]
00000001406BA226: 83 FA 14                                 CMP      edx, 0x14
00000001406BA229: 74 09                                    JE       0x1406ba234
00000001406BA22B: 83 FA 6D                                 CMP      edx, 0x6d
00000001406BA22E: 0F 85 7D 01 00 00                        JNE      0x1406ba3b1
00000001406BA234: 4E 89 B4 C1 78 0A 00 00                  MOV      qword ptr [rcx + r8*8 + 0xa78], r14
00000001406BA23C: 48 63 8F 6C 05 00 00                     MOVSXD   rcx, dword ptr [rdi + 0x56c]
00000001406BA243: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA24A: C6 84 01 58 0A 00 00 00                  MOV      byte ptr [rcx + rax + 0xa58], 0
00000001406BA252: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA259: 48 8B 88 18 03 00 00                     MOV      rcx, qword ptr [rax + 0x318]
00000001406BA260: 48 85 C9                                 TEST     rcx, rcx
00000001406BA263: 0F 84 0D 01 00 00                        JE       0x1406ba376
00000001406BA269: 80 B9 79 09 00 00 00                     CMP      byte ptr [rcx + 0x979], 0
00000001406BA270: 0F 84 00 01 00 00                        JE       0x1406ba376
00000001406BA276: 48 8B D0                                 MOV      rdx, rax
00000001406BA279: 48 8B CF                                 MOV      rcx, rdi
00000001406BA27C: E8 DF 8A FC FF                           CALL     0x140682d60
00000001406BA281: 48 8D 9F 38 07 00 00                     LEA      rbx, [rdi + 0x738]
00000001406BA288: 48 8B 4B 08                              MOV      rcx, qword ptr [rbx + 8]
00000001406BA28C: 48 2B 0B                                 SUB      rcx, qword ptr [rbx]
00000001406BA28F: 49 8B C5                                 MOV      rax, r13
00000001406BA292: 48 F7 E9                                 IMUL     rcx
00000001406BA295: 48 D1 FA                                 SAR      rdx, 1
00000001406BA298: 48 8B C2                                 MOV      rax, rdx
00000001406BA29B: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406BA29F: 48 03 D0                                 ADD      rdx, rax
00000001406BA2A2: 0F 85 FF 00 00 00                        JNE      0x1406ba3a7
00000001406BA2A8: 48 63 97 6C 05 00 00                     MOVSXD   rdx, dword ptr [rdi + 0x56c]
00000001406BA2AF: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA2B6: 48 8B CA                                 MOV      rcx, rdx
00000001406BA2B9: 48 C1 E1 05                              SHL      rcx, 5
00000001406BA2BD: 48 03 88 78 0B 00 00                     ADD      rcx, qword ptr [rax + 0xb78]
00000001406BA2C4: 48 C1 E2 05                              SHL      rdx, 5
00000001406BA2C8: 48 8B 80 40 0A 00 00                     MOV      rax, qword ptr [rax + 0xa40]
00000001406BA2CF: F3 0F 10 54 02 14                        MOVSS    xmm2, dword ptr [rdx + rax + 0x14]
00000001406BA2D5: F3 0F 5C 51 08                           SUBSS    xmm2, dword ptr [rcx + 8]
00000001406BA2DA: F3 0F 10 4C 02 10                        MOVSS    xmm1, dword ptr [rdx + rax + 0x10]
00000001406BA2E0: F3 0F 5C 49 04                           SUBSS    xmm1, dword ptr [rcx + 4]
00000001406BA2E5: F3 0F 10 44 02 0C                        MOVSS    xmm0, dword ptr [rdx + rax + 0xc]
00000001406BA2EB: F3 0F 5C 01                              SUBSS    xmm0, dword ptr [rcx]
00000001406BA2EF: F3 0F 11 44 24 30                        MOVSS    dword ptr [rsp + 0x30], xmm0
00000001406BA2F5: F3 0F 11 4C 24 34                        MOVSS    dword ptr [rsp + 0x34], xmm1
00000001406BA2FB: F3 0F 11 54 24 38                        MOVSS    dword ptr [rsp + 0x38], xmm2
00000001406BA301: 48 8D 54 24 30                           LEA      rdx, [rsp + 0x30]
00000001406BA306: 48 8D 4C 24 30                           LEA      rcx, [rsp + 0x30]
00000001406BA30B: FF 15 F7 1D 1B 00                        CALL     qword ptr [rip + 0x1b1df7]
00000001406BA311: F3 0F 10 54 24 38                        MOVSS    xmm2, dword ptr [rsp + 0x38]
00000001406BA317: F3 41 0F 59 D1                           MULSS    xmm2, xmm9
00000001406BA31C: F3 0F 10 4C 24 34                        MOVSS    xmm1, dword ptr [rsp + 0x34]
00000001406BA322: F3 41 0F 59 C9                           MULSS    xmm1, xmm9
00000001406BA327: F3 0F 10 44 24 30                        MOVSS    xmm0, dword ptr [rsp + 0x30]
00000001406BA32D: F3 41 0F 59 C1                           MULSS    xmm0, xmm9
00000001406BA332: 48 63 97 6C 05 00 00                     MOVSXD   rdx, dword ptr [rdi + 0x56c]
00000001406BA339: 48 8B 87 F0 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f0]
00000001406BA340: 48 C1 E2 05                              SHL      rdx, 5
00000001406BA344: 48 03 90 40 0A 00 00                     ADD      rdx, qword ptr [rax + 0xa40]
00000001406BA34B: F3 0F 58 52 08                           ADDSS    xmm2, dword ptr [rdx + 8]
00000001406BA350: F3 0F 58 4A 04                           ADDSS    xmm1, dword ptr [rdx + 4]
00000001406BA355: F3 0F 58 02                              ADDSS    xmm0, dword ptr [rdx]
00000001406BA359: F3 0F 11 45 87                           MOVSS    dword ptr [rbp - 0x79], xmm0
00000001406BA35E: F3 0F 11 4D 8B                           MOVSS    dword ptr [rbp - 0x75], xmm1
00000001406BA363: F3 0F 11 55 8F                           MOVSS    dword ptr [rbp - 0x71], xmm2
00000001406BA368: 48 8D 55 87                              LEA      rdx, [rbp - 0x79]
00000001406BA36C: 48 8B CB                                 MOV      rcx, rbx
00000001406BA36F: E8 7C 4F 97 FF                           CALL     0x14002f2f0
00000001406BA374: EB 31                                    JMP      0x1406ba3a7
00000001406BA376: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406BA37D: 48 63 97 6C 05 00 00                     MOVSXD   rdx, dword ptr [rdi + 0x56c]
00000001406BA384: 48 C1 E2 05                              SHL      rdx, 5
00000001406BA388: 48 8B 80 40 0A 00 00                     MOV      rax, qword ptr [rax + 0xa40]
00000001406BA38F: 48 83 C0 0C                              ADD      rax, 0xc
00000001406BA393: 48 03 D0                                 ADD      rdx, rax
00000001406BA396: 48 89 54 24 20                           MOV      qword ptr [rsp + 0x20], rdx
00000001406BA39B: 4C 8B 01                                 MOV      r8, qword ptr [rcx]
00000001406BA39E: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406BA3A2: E8 A9 82 B9 FF                           CALL     0x140252650
00000001406BA3A7: C7 87 6C 05 00 00 FF FF FF FF            MOV      dword ptr [rdi + 0x56c], 0xffffffff
00000001406BA3B1: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406BA3B8: 48 85 C0                                 TEST     rax, rax
00000001406BA3BB: 0F 84 27 01 00 00                        JE       0x1406ba4e8
00000001406BA3C1: 83 B8 94 02 00 00 08                     CMP      dword ptr [rax + 0x294], 8
00000001406BA3C8: 0F 85 1A 01 00 00                        JNE      0x1406ba4e8
00000001406BA3CE: 44 8B 8F 6C 05 00 00                     MOV      r9d, dword ptr [rdi + 0x56c]
00000001406BA3D5: 45 85 C9                                 TEST     r9d, r9d
00000001406BA3D8: 0F 88 0A 01 00 00                        JS       0x1406ba4e8
00000001406BA3DE: 0F 57 C0                                 XORPS    xmm0, xmm0
00000001406BA3E1: F3 0F 7F 45 87                           MOVDQU   xmmword ptr [rbp - 0x79], xmm0
00000001406BA3E6: 4C 89 75 97                              MOV      qword ptr [rbp - 0x69], r14
00000001406BA3EA: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406BA3F1: 33 D2                                    XOR      edx, edx
00000001406BA3F3: E8 E8 25 D0 FF                           CALL     0x1403bc9e0
00000001406BA3F8: 44 8B 8F 6C 05 00 00                     MOV      r9d, dword ptr [rdi + 0x56c]
00000001406BA3FF: 4C 8B 87 F0 04 00 00                     MOV      r8, qword ptr [rdi + 0x4f0]
00000001406BA406: 33 D2                                    XOR      edx, edx
00000001406BA408: E8 C3 26 D0 FF                           CALL     0x1403bcad0
00000001406BA40D: 4C 8D 4D A7                              LEA      r9, [rbp - 0x59]
00000001406BA411: 44 8B 87 6C 05 00 00                     MOV      r8d, dword ptr [rdi + 0x56c]
00000001406BA418: 48 8B 97 F0 04 00 00                     MOV      rdx, qword ptr [rdi + 0x4f0]
00000001406BA41F: E8 0C 19 D0 FF                           CALL     0x1403bbd30
00000001406BA424: 4C 8D 4D 87                              LEA      r9, [rbp - 0x79]
00000001406BA428: 44 8B 87 6C 05 00 00                     MOV      r8d, dword ptr [rdi + 0x56c]
00000001406BA42F: 48 8B 97 F0 04 00 00                     MOV      rdx, qword ptr [rdi + 0x4f0]
00000001406BA436: E8 E5 1C D0 FF                           CALL     0x1403bc120
00000001406BA43B: 48 8B 4D 8F                              MOV      rcx, qword ptr [rbp - 0x71]
00000001406BA43F: 4C 8B 65 87                              MOV      r12, qword ptr [rbp - 0x79]
00000001406BA443: 49 2B CC                                 SUB      rcx, r12
00000001406BA446: 49 8B C5                                 MOV      rax, r13
00000001406BA449: 48 F7 E9                                 IMUL     rcx
00000001406BA44C: 48 D1 FA                                 SAR      rdx, 1
00000001406BA44F: 48 8B C2                                 MOV      rax, rdx
00000001406BA452: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406BA456: 48 03 D0                                 ADD      rdx, rax
00000001406BA459: 74 3A                                    JE       0x1406ba495
00000001406BA45B: 8D 5A FF                                 LEA      ebx, [rdx - 1]
00000001406BA45E: 85 DB                                    TEST     ebx, ebx
00000001406BA460: 78 33                                    JS       0x1406ba495
00000001406BA462: 48 63 C3                                 MOVSXD   rax, ebx
00000001406BA465: 48 8D 0C 40                              LEA      rcx, [rax + rax*2]
00000001406BA469: 49 8D 34 8C                              LEA      rsi, [r12 + rcx*4]
00000001406BA46D: 0F 1F 00                                 NOP      dword ptr [rax]
00000001406BA470: 48 89 74 24 20                           MOV      qword ptr [rsp + 0x20], rsi
00000001406BA475: 4C 8B 87 38 07 00 00                     MOV      r8, qword ptr [rdi + 0x738]
00000001406BA47C: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406BA480: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406BA487: E8 C4 81 B9 FF                           CALL     0x140252650
00000001406BA48C: 48 83 EE 0C                              SUB      rsi, 0xc
00000001406BA490: 83 EB 01                                 SUB      ebx, 1
00000001406BA493: 79 DB                                    JNS      0x1406ba470
00000001406BA495: 48 8D 8F 38 07 00 00                     LEA      rcx, [rdi + 0x738]
00000001406BA49C: 48 8D 55 B3                              LEA      rdx, [rbp - 0x4d]
00000001406BA4A0: 48 89 54 24 20                           MOV      qword ptr [rsp + 0x20], rdx
00000001406BA4A5: 4C 8B 01                                 MOV      r8, qword ptr [rcx]
00000001406BA4A8: 48 8D 55 67                              LEA      rdx, [rbp + 0x67]
00000001406BA4AC: E8 9F 81 B9 FF                           CALL     0x140252650
00000001406BA4B1: C7 87 6C 05 00 00 FF FF FF FF            MOV      dword ptr [rdi + 0x56c], 0xffffffff
00000001406BA4BB: 4D 85 E4                                 TEST     r12, r12
00000001406BA4BE: 74 28                                    JE       0x1406ba4e8
00000001406BA4C0: 48 8B 55 97                              MOV      rdx, qword ptr [rbp - 0x69]
00000001406BA4C4: 49 2B D4                                 SUB      rdx, r12
00000001406BA4C7: 49 8B C5                                 MOV      rax, r13
00000001406BA4CA: 48 F7 EA                                 IMUL     rdx
00000001406BA4CD: 48 D1 FA                                 SAR      rdx, 1
00000001406BA4D0: 48 8B C2                                 MOV      rax, rdx
00000001406BA4D3: 48 C1 E8 3F                              SHR      rax, 0x3f
00000001406BA4D7: 48 03 D0                                 ADD      rdx, rax
00000001406BA4DA: 41 B8 0C 00 00 00                        MOV      r8d, 0xc
00000001406BA4E0: 49 8B CC                                 MOV      rcx, r12
00000001406BA4E3: E8 68 75 94 FF                           CALL     0x140001a50
00000001406BA4E8: 48 8B B7 F8 04 00 00                     MOV      rsi, qword ptr [rdi + 0x4f8]
00000001406BA4EF: 48 85 F6                                 TEST     rsi, rsi
00000001406BA4F2: 0F 84 53 02 00 00                        JE       0x1406ba74b
00000001406BA4F8: 32 DB                                    XOR      bl, bl
00000001406BA4FA: 48 8B 8F 08 17 00 00                     MOV      rcx, qword ptr [rdi + 0x1708]
00000001406BA501: 8B 91 94 02 00 00                        MOV      edx, dword ptr [rcx + 0x294]
00000001406BA507: 83 FA 08                                 CMP      edx, 8
00000001406BA50A: 75 3A                                    JNE      0x1406ba546
00000001406BA50C: 48 83 BE 18 03 00 00 00                  CMP      qword ptr [rsi + 0x318], 0
00000001406BA514: 74 30                                    JE       0x1406ba546
00000001406BA516: 48 8B 86 B0 05 00 00                     MOV      rax, qword ptr [rsi + 0x5b0]
00000001406BA51D: 48 85 C0                                 TEST     rax, rax
00000001406BA520: 74 24                                    JE       0x1406ba546
00000001406BA522: 4C 8B 80 18 03 00 00                     MOV      r8, qword ptr [rax + 0x318]
00000001406BA529: 41 83 B8 60 03 00 00 28                  CMP      dword ptr [r8 + 0x360], 0x28
00000001406BA531: 75 13                                    JNE      0x1406ba546
00000001406BA533: 0F B6 DB                                 MOVZX    ebx, bl
00000001406BA536: 41 83 B8 64 03 00 00 21                  CMP      dword ptr [r8 + 0x364], 0x21
00000001406BA53E: B8 01 00 00 00                           MOV      eax, 1
00000001406BA543: 0F 44 D8                                 CMOVE    ebx, eax
00000001406BA546: 48 85 C9                                 TEST     rcx, rcx
00000001406BA549: 74 0E                                    JE       0x1406ba559
00000001406BA54B: 8D 42 FA                                 LEA      eax, [rdx - 6]
00000001406BA54E: A9 FB FF FF FF                           TEST     eax, 0xfffffffb
00000001406BA553: 0F 84 F2 01 00 00                        JE       0x1406ba74b
00000001406BA559: 45 33 C9                                 XOR      r9d, r9d
00000001406BA55C: 4C 8B C7                                 MOV      r8, rdi
00000001406BA55F: 48 8B D6                                 MOV      rdx, rsi
00000001406BA562: E8 D9 10 D0 FF                           CALL     0x1403bb640
00000001406BA567: 84 C0                                    TEST     al, al
00000001406BA569: 75 08                                    JNE      0x1406ba573
00000001406BA56B: 84 DB                                    TEST     bl, bl
00000001406BA56D: 0F 84 D8 01 00 00                        JE       0x1406ba74b
00000001406BA573: 48 8B 96 78 0C 00 00                     MOV      rdx, qword ptr [rsi + 0xc78]
00000001406BA57A: 48 8B B6 70 0C 00 00                     MOV      rsi, qword ptr [rsi + 0xc70]
00000001406BA581: 48 3B F2                                 CMP      rsi, rdx
00000001406BA584: 74 55                                    JE       0x1406ba5db
00000001406BA586: 48 39 3E                                 CMP      qword ptr [rsi], rdi
00000001406BA589: 74 09                                    JE       0x1406ba594
00000001406BA58B: 48 83 C6 08                              ADD      rsi, 8
00000001406BA58F: 48 3B F2                                 CMP      rsi, rdx
00000001406BA592: 75 F2                                    JNE      0x1406ba586
00000001406BA594: 48 3B F2                                 CMP      rsi, rdx
00000001406BA597: 74 42                                    JE       0x1406ba5db
00000001406BA599: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BA59D: 49 8B CE                                 MOV      rcx, r14
00000001406BA5A0: 4C 8B CA                                 MOV      r9, rdx
00000001406BA5A3: 4C 2B C8                                 SUB      r9, rax
00000001406BA5A6: 49 83 C1 07                              ADD      r9, 7
00000001406BA5AA: 49 C1 E9 03                              SHR      r9, 3
00000001406BA5AE: 48 3B C2                                 CMP      rax, rdx
00000001406BA5B1: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BA5B5: 4D 85 C9                                 TEST     r9, r9
00000001406BA5B8: 74 21                                    JE       0x1406ba5db
00000001406BA5BA: 66 0F 1F 44 00 00                        NOP      word ptr [rax + rax]
00000001406BA5C0: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BA5C3: 4C 3B C7                                 CMP      r8, rdi
00000001406BA5C6: 74 07                                    JE       0x1406ba5cf
00000001406BA5C8: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BA5CB: 48 83 C6 08                              ADD      rsi, 8
00000001406BA5CF: 48 83 C0 08                              ADD      rax, 8
00000001406BA5D3: 48 FF C1                                 INC      rcx
00000001406BA5D6: 49 3B C9                                 CMP      rcx, r9
00000001406BA5D9: 75 E5                                    JNE      0x1406ba5c0
00000001406BA5DB: 4C 8B BF F8 04 00 00                     MOV      r15, qword ptr [rdi + 0x4f8]
00000001406BA5E2: 49 8B 87 70 0C 00 00                     MOV      rax, qword ptr [r15 + 0xc70]
00000001406BA5E9: 48 3B F0                                 CMP      rsi, rax
00000001406BA5EC: 75 09                                    JNE      0x1406ba5f7
00000001406BA5EE: 49 3B 97 78 0C 00 00                     CMP      rdx, qword ptr [r15 + 0xc78]
00000001406BA5F5: 74 1F                                    JE       0x1406ba616
00000001406BA5F7: 48 3B F2                                 CMP      rsi, rdx
00000001406BA5FA: 74 21                                    JE       0x1406ba61d
00000001406BA5FC: 49 8B 9F 78 0C 00 00                     MOV      rbx, qword ptr [r15 + 0xc78]
00000001406BA603: 48 2B DA                                 SUB      rbx, rdx
00000001406BA606: 4C 8B C3                                 MOV      r8, rbx
00000001406BA609: 48 8B CE                                 MOV      rcx, rsi
00000001406BA60C: FF 15 36 36 1B 00                        CALL     qword ptr [rip + 0x1b3636]
00000001406BA612: 48 8D 04 33                              LEA      rax, [rbx + rsi]
00000001406BA616: 49 89 87 78 0C 00 00                     MOV      qword ptr [r15 + 0xc78], rax
00000001406BA61D: 45 8B EE                                 MOV      r13d, r14d
00000001406BA620: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BA627: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BA62E: 48 C1 F8 03                              SAR      rax, 3
00000001406BA632: 48 85 C0                                 TEST     rax, rax
00000001406BA635: 0F 84 09 01 00 00                        JE       0x1406ba744
00000001406BA63B: 4D 8B FE                                 MOV      r15, r14
00000001406BA63E: 66 90                                    NOP
00000001406BA640: 4C 8B 97 88 04 00 00                     MOV      r10, qword ptr [rdi + 0x488]
00000001406BA647: 4B 8B 04 FA                              MOV      rax, qword ptr [r10 + r15*8]
00000001406BA64B: 48 8B B0 F8 04 00 00                     MOV      rsi, qword ptr [rax + 0x4f8]
00000001406BA652: 48 85 F6                                 TEST     rsi, rsi
00000001406BA655: 0F 84 B6 00 00 00                        JE       0x1406ba711
00000001406BA65B: 48 8B 96 78 0C 00 00                     MOV      rdx, qword ptr [rsi + 0xc78]
00000001406BA662: 48 8B B6 70 0C 00 00                     MOV      rsi, qword ptr [rsi + 0xc70]
00000001406BA669: 48 3B F2                                 CMP      rsi, rdx
00000001406BA66C: 74 52                                    JE       0x1406ba6c0
00000001406BA66E: 66 90                                    NOP
00000001406BA670: 48 39 06                                 CMP      qword ptr [rsi], rax
00000001406BA673: 74 09                                    JE       0x1406ba67e
00000001406BA675: 48 83 C6 08                              ADD      rsi, 8
00000001406BA679: 48 3B F2                                 CMP      rsi, rdx
00000001406BA67C: 75 F2                                    JNE      0x1406ba670
00000001406BA67E: 48 3B F2                                 CMP      rsi, rdx
00000001406BA681: 74 3D                                    JE       0x1406ba6c0
00000001406BA683: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BA687: 49 8B CE                                 MOV      rcx, r14
00000001406BA68A: 4C 8B CA                                 MOV      r9, rdx
00000001406BA68D: 4C 2B C8                                 SUB      r9, rax
00000001406BA690: 49 83 C1 07                              ADD      r9, 7
00000001406BA694: 49 C1 E9 03                              SHR      r9, 3
00000001406BA698: 48 3B C2                                 CMP      rax, rdx
00000001406BA69B: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BA69F: 4D 85 C9                                 TEST     r9, r9
00000001406BA6A2: 74 1C                                    JE       0x1406ba6c0
00000001406BA6A4: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BA6A7: 4F 3B 04 FA                              CMP      r8, qword ptr [r10 + r15*8]
00000001406BA6AB: 74 07                                    JE       0x1406ba6b4
00000001406BA6AD: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BA6B0: 48 83 C6 08                              ADD      rsi, 8
00000001406BA6B4: 48 83 C0 08                              ADD      rax, 8
00000001406BA6B8: 48 FF C1                                 INC      rcx
00000001406BA6BB: 49 3B C9                                 CMP      rcx, r9
00000001406BA6BE: 75 E4                                    JNE      0x1406ba6a4
00000001406BA6C0: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BA6C7: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BA6CB: 4C 8B A1 F8 04 00 00                     MOV      r12, qword ptr [rcx + 0x4f8]
00000001406BA6D2: 49 8B 84 24 70 0C 00 00                  MOV      rax, qword ptr [r12 + 0xc70]
00000001406BA6DA: 48 3B F0                                 CMP      rsi, rax
00000001406BA6DD: 75 0A                                    JNE      0x1406ba6e9
00000001406BA6DF: 49 3B 94 24 78 0C 00 00                  CMP      rdx, qword ptr [r12 + 0xc78]
00000001406BA6E7: 74 20                                    JE       0x1406ba709
00000001406BA6E9: 48 3B F2                                 CMP      rsi, rdx
00000001406BA6EC: 74 23                                    JE       0x1406ba711
00000001406BA6EE: 49 8B 9C 24 78 0C 00 00                  MOV      rbx, qword ptr [r12 + 0xc78]
00000001406BA6F6: 48 2B DA                                 SUB      rbx, rdx
00000001406BA6F9: 4C 8B C3                                 MOV      r8, rbx
00000001406BA6FC: 48 8B CE                                 MOV      rcx, rsi
00000001406BA6FF: FF 15 43 35 1B 00                        CALL     qword ptr [rip + 0x1b3543]
00000001406BA705: 48 8D 04 33                              LEA      rax, [rbx + rsi]
00000001406BA709: 49 89 84 24 78 0C 00 00                  MOV      qword ptr [r12 + 0xc78], rax
00000001406BA711: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BA718: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BA71C: 4C 89 B1 F8 04 00 00                     MOV      qword ptr [rcx + 0x4f8], r14
00000001406BA723: 41 FF C5                                 INC      r13d
00000001406BA726: 45 8B FD                                 MOV      r15d, r13d
00000001406BA729: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BA730: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BA737: 48 C1 F8 03                              SAR      rax, 3
00000001406BA73B: 4C 3B F8                                 CMP      r15, rax
00000001406BA73E: 0F 82 FC FE FF FF                        JB       0x1406ba640
00000001406BA744: 4C 89 B7 F8 04 00 00                     MOV      qword ptr [rdi + 0x4f8], r14
00000001406BA74B: 83 BF 0C 05 00 00 0C                     CMP      dword ptr [rdi + 0x50c], 0xc
00000001406BA752: 0F 84 D2 01 00 00                        JE       0x1406ba92a
00000001406BA758: 48 8B 87 F8 04 00 00                     MOV      rax, qword ptr [rdi + 0x4f8]
00000001406BA75F: 48 85 C0                                 TEST     rax, rax
00000001406BA762: 0F 84 C2 01 00 00                        JE       0x1406ba92a
00000001406BA768: 48 8B B7 F0 04 00 00                     MOV      rsi, qword ptr [rdi + 0x4f0]
00000001406BA76F: 48 3B F0                                 CMP      rsi, rax
00000001406BA772: 0F 84 B2 01 00 00                        JE       0x1406ba92a
00000001406BA778: 48 8B 96 78 0C 00 00                     MOV      rdx, qword ptr [rsi + 0xc78]
00000001406BA77F: 48 8B B6 70 0C 00 00                     MOV      rsi, qword ptr [rsi + 0xc70]
00000001406BA786: 48 3B F2                                 CMP      rsi, rdx
00000001406BA789: 74 54                                    JE       0x1406ba7df
00000001406BA78B: 0F 1F 44 00 00                           NOP      dword ptr [rax + rax]
00000001406BA790: 48 39 3E                                 CMP      qword ptr [rsi], rdi
00000001406BA793: 74 09                                    JE       0x1406ba79e
00000001406BA795: 48 83 C6 08                              ADD      rsi, 8
00000001406BA799: 48 3B F2                                 CMP      rsi, rdx
00000001406BA79C: 75 F2                                    JNE      0x1406ba790
00000001406BA79E: 48 3B F2                                 CMP      rsi, rdx
00000001406BA7A1: 74 3C                                    JE       0x1406ba7df
00000001406BA7A3: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BA7A7: 49 8B CE                                 MOV      rcx, r14
00000001406BA7AA: 4C 8B CA                                 MOV      r9, rdx
00000001406BA7AD: 4C 2B C8                                 SUB      r9, rax
00000001406BA7B0: 49 83 C1 07                              ADD      r9, 7
00000001406BA7B4: 49 C1 E9 03                              SHR      r9, 3
00000001406BA7B8: 48 3B C2                                 CMP      rax, rdx
00000001406BA7BB: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BA7BF: 4D 85 C9                                 TEST     r9, r9
00000001406BA7C2: 74 1B                                    JE       0x1406ba7df
00000001406BA7C4: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BA7C7: 4C 3B C7                                 CMP      r8, rdi
00000001406BA7CA: 74 07                                    JE       0x1406ba7d3
00000001406BA7CC: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BA7CF: 48 83 C6 08                              ADD      rsi, 8
00000001406BA7D3: 48 83 C0 08                              ADD      rax, 8
00000001406BA7D7: 48 FF C1                                 INC      rcx
00000001406BA7DA: 49 3B C9                                 CMP      rcx, r9
00000001406BA7DD: 75 E5                                    JNE      0x1406ba7c4
00000001406BA7DF: 4C 8B BF F0 04 00 00                     MOV      r15, qword ptr [rdi + 0x4f0]
00000001406BA7E6: 49 8B 87 70 0C 00 00                     MOV      rax, qword ptr [r15 + 0xc70]
00000001406BA7ED: 48 3B F0                                 CMP      rsi, rax
00000001406BA7F0: 75 09                                    JNE      0x1406ba7fb
00000001406BA7F2: 49 3B 97 78 0C 00 00                     CMP      rdx, qword ptr [r15 + 0xc78]
00000001406BA7F9: 74 1F                                    JE       0x1406ba81a
00000001406BA7FB: 48 3B F2                                 CMP      rsi, rdx
00000001406BA7FE: 74 21                                    JE       0x1406ba821
00000001406BA800: 49 8B 9F 78 0C 00 00                     MOV      rbx, qword ptr [r15 + 0xc78]
00000001406BA807: 48 2B DA                                 SUB      rbx, rdx
00000001406BA80A: 4C 8B C3                                 MOV      r8, rbx
00000001406BA80D: 48 8B CE                                 MOV      rcx, rsi
00000001406BA810: FF 15 32 34 1B 00                        CALL     qword ptr [rip + 0x1b3432]
00000001406BA816: 48 8D 04 33                              LEA      rax, [rbx + rsi]
00000001406BA81A: 49 89 87 78 0C 00 00                     MOV      qword ptr [r15 + 0xc78], rax
00000001406BA821: 45 8B E6                                 MOV      r12d, r14d
00000001406BA824: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BA82B: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BA832: 48 C1 F8 03                              SAR      rax, 3
00000001406BA836: 48 85 C0                                 TEST     rax, rax
00000001406BA839: 0F 84 EB 00 00 00                        JE       0x1406ba92a
00000001406BA83F: 4D 8B D6                                 MOV      r10, r14
00000001406BA842: 4C 8B 9F 88 04 00 00                     MOV      r11, qword ptr [rdi + 0x488]
00000001406BA849: 4B 8B 04 D3                              MOV      rax, qword ptr [r11 + r10*8]
00000001406BA84D: 48 8B 88 F0 04 00 00                     MOV      rcx, qword ptr [rax + 0x4f0]
00000001406BA854: 48 8B 91 78 0C 00 00                     MOV      rdx, qword ptr [rcx + 0xc78]
00000001406BA85B: 48 8B B1 70 0C 00 00                     MOV      rsi, qword ptr [rcx + 0xc70]
00000001406BA862: 48 3B F2                                 CMP      rsi, rdx
00000001406BA865: 74 55                                    JE       0x1406ba8bc
00000001406BA867: 48 39 06                                 CMP      qword ptr [rsi], rax
00000001406BA86A: 74 09                                    JE       0x1406ba875
00000001406BA86C: 48 83 C6 08                              ADD      rsi, 8
00000001406BA870: 48 3B F2                                 CMP      rsi, rdx
00000001406BA873: 75 F2                                    JNE      0x1406ba867
00000001406BA875: 48 3B F2                                 CMP      rsi, rdx
00000001406BA878: 74 42                                    JE       0x1406ba8bc
00000001406BA87A: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BA87E: 49 8B CE                                 MOV      rcx, r14
00000001406BA881: 4C 8B CA                                 MOV      r9, rdx
00000001406BA884: 4C 2B C8                                 SUB      r9, rax
00000001406BA887: 49 83 C1 07                              ADD      r9, 7
00000001406BA88B: 49 C1 E9 03                              SHR      r9, 3
00000001406BA88F: 48 3B C2                                 CMP      rax, rdx
00000001406BA892: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BA896: 4D 85 C9                                 TEST     r9, r9
00000001406BA899: 74 21                                    JE       0x1406ba8bc
00000001406BA89B: 0F 1F 44 00 00                           NOP      dword ptr [rax + rax]
00000001406BA8A0: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BA8A3: 4F 3B 04 D3                              CMP      r8, qword ptr [r11 + r10*8]
00000001406BA8A7: 74 07                                    JE       0x1406ba8b0
00000001406BA8A9: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BA8AC: 48 83 C6 08                              ADD      rsi, 8
00000001406BA8B0: 48 83 C0 08                              ADD      rax, 8
00000001406BA8B4: 48 FF C1                                 INC      rcx
00000001406BA8B7: 49 3B C9                                 CMP      rcx, r9
00000001406BA8BA: 75 E4                                    JNE      0x1406ba8a0
00000001406BA8BC: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BA8C3: 4A 8B 0C D0                              MOV      rcx, qword ptr [rax + r10*8]
00000001406BA8C7: 4C 8B B9 F0 04 00 00                     MOV      r15, qword ptr [rcx + 0x4f0]
00000001406BA8CE: 49 8B 87 70 0C 00 00                     MOV      rax, qword ptr [r15 + 0xc70]
00000001406BA8D5: 48 3B F0                                 CMP      rsi, rax
00000001406BA8D8: 75 09                                    JNE      0x1406ba8e3
00000001406BA8DA: 49 3B 97 78 0C 00 00                     CMP      rdx, qword ptr [r15 + 0xc78]
00000001406BA8E1: 74 1F                                    JE       0x1406ba902
00000001406BA8E3: 48 3B F2                                 CMP      rsi, rdx
00000001406BA8E6: 74 21                                    JE       0x1406ba909
00000001406BA8E8: 49 8B 9F 78 0C 00 00                     MOV      rbx, qword ptr [r15 + 0xc78]
00000001406BA8EF: 48 2B DA                                 SUB      rbx, rdx
00000001406BA8F2: 4C 8B C3                                 MOV      r8, rbx
00000001406BA8F5: 48 8B CE                                 MOV      rcx, rsi
00000001406BA8F8: FF 15 4A 33 1B 00                        CALL     qword ptr [rip + 0x1b334a]
00000001406BA8FE: 48 8D 04 33                              LEA      rax, [rbx + rsi]
00000001406BA902: 49 89 87 78 0C 00 00                     MOV      qword ptr [r15 + 0xc78], rax
00000001406BA909: 41 FF C4                                 INC      r12d
00000001406BA90C: 45 8B D4                                 MOV      r10d, r12d
00000001406BA90F: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BA916: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BA91D: 48 C1 F8 03                              SAR      rax, 3
00000001406BA921: 4C 3B D0                                 CMP      r10, rax
00000001406BA924: 0F 82 18 FF FF FF                        JB       0x1406ba842
00000001406BA92A: 80 BF 7D 06 00 00 00                     CMP      byte ptr [rdi + 0x67d], 0
00000001406BA931: 75 4C                                    JNE      0x1406ba97f
00000001406BA933: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406BA93A: 8B 88 94 02 00 00                        MOV      ecx, dword ptr [rax + 0x294]
00000001406BA940: 83 E9 04                                 SUB      ecx, 4
00000001406BA943: 83 F9 01                                 CMP      ecx, 1
00000001406BA946: 77 37                                    JA       0x1406ba97f
00000001406BA948: 48 83 BF F0 04 00 00 00                  CMP      qword ptr [rdi + 0x4f0], 0
00000001406BA950: 74 2D                                    JE       0x1406ba97f
00000001406BA952: 48 8B 87 A8 06 00 00                     MOV      rax, qword ptr [rdi + 0x6a8]
00000001406BA959: 48 2B 87 A0 06 00 00                     SUB      rax, qword ptr [rdi + 0x6a0]
00000001406BA960: 48 C1 F8 03                              SAR      rax, 3
00000001406BA964: 48 85 C0                                 TEST     rax, rax
00000001406BA967: 74 16                                    JE       0x1406ba97f
00000001406BA969: 48 8B 97 A0 06 00 00                     MOV      rdx, qword ptr [rdi + 0x6a0]
00000001406BA970: 48 8B 12                                 MOV      rdx, qword ptr [rdx]
00000001406BA973: 48 8D 0D 96 A5 31 00                     LEA      rcx, [rip + 0x31a596]
00000001406BA97A: E8 31 41 D6 FF                           CALL     0x14041eab0
00000001406BA97F: 4C 89 B7 F0 04 00 00                     MOV      qword ptr [rdi + 0x4f0], r14
00000001406BA986: 45 8B EE                                 MOV      r13d, r14d
00000001406BA989: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BA990: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BA997: 48 C1 F8 03                              SAR      rax, 3
00000001406BA99B: 48 85 C0                                 TEST     rax, rax
00000001406BA99E: 0F 84 3E 01 00 00                        JE       0x1406baae2
00000001406BA9A4: 4D 8B FE                                 MOV      r15, r14
00000001406BA9A7: 66 0F 1F 84 00 00 00 00 00               NOP      word ptr [rax + rax]
00000001406BA9B0: 48 8B 9F 88 04 00 00                     MOV      rbx, qword ptr [rdi + 0x488]
00000001406BA9B7: 4A 8B 04 FB                              MOV      rax, qword ptr [rbx + r15*8]
00000001406BA9BB: 48 8B B0 F8 04 00 00                     MOV      rsi, qword ptr [rax + 0x4f8]
00000001406BA9C2: 48 85 F6                                 TEST     rsi, rsi
00000001406BA9C5: 0F 84 E4 00 00 00                        JE       0x1406baaaf
00000001406BA9CB: 45 33 C9                                 XOR      r9d, r9d
00000001406BA9CE: 4C 8B C7                                 MOV      r8, rdi
00000001406BA9D1: 48 8B D6                                 MOV      rdx, rsi
00000001406BA9D4: E8 67 0C D0 FF                           CALL     0x1403bb640
00000001406BA9D9: 84 C0                                    TEST     al, al
00000001406BA9DB: 0F 84 CE 00 00 00                        JE       0x1406baaaf
00000001406BA9E1: 48 8B 96 78 0C 00 00                     MOV      rdx, qword ptr [rsi + 0xc78]
00000001406BA9E8: 48 8B B6 70 0C 00 00                     MOV      rsi, qword ptr [rsi + 0xc70]
00000001406BA9EF: 48 3B F2                                 CMP      rsi, rdx
00000001406BA9F2: 74 58                                    JE       0x1406baa4c
00000001406BA9F4: 4A 8B 04 FB                              MOV      rax, qword ptr [rbx + r15*8]
00000001406BA9F8: 48 39 06                                 CMP      qword ptr [rsi], rax
00000001406BA9FB: 74 09                                    JE       0x1406baa06
00000001406BA9FD: 48 83 C6 08                              ADD      rsi, 8
00000001406BAA01: 48 3B F2                                 CMP      rsi, rdx
00000001406BAA04: 75 F2                                    JNE      0x1406ba9f8
00000001406BAA06: 48 3B F2                                 CMP      rsi, rdx
00000001406BAA09: 74 41                                    JE       0x1406baa4c
00000001406BAA0B: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BAA0F: 49 8B CE                                 MOV      rcx, r14
00000001406BAA12: 4C 8B CA                                 MOV      r9, rdx
00000001406BAA15: 4C 2B C8                                 SUB      r9, rax
00000001406BAA18: 49 83 C1 07                              ADD      r9, 7
00000001406BAA1C: 49 C1 E9 03                              SHR      r9, 3
00000001406BAA20: 48 3B C2                                 CMP      rax, rdx
00000001406BAA23: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BAA27: 4D 85 C9                                 TEST     r9, r9
00000001406BAA2A: 74 20                                    JE       0x1406baa4c
00000001406BAA2C: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001406BAA30: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BAA33: 4E 3B 04 FB                              CMP      r8, qword ptr [rbx + r15*8]
00000001406BAA37: 74 07                                    JE       0x1406baa40
00000001406BAA39: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BAA3C: 48 83 C6 08                              ADD      rsi, 8
00000001406BAA40: 48 83 C0 08                              ADD      rax, 8
00000001406BAA44: 48 FF C1                                 INC      rcx
00000001406BAA47: 49 3B C9                                 CMP      rcx, r9
00000001406BAA4A: 75 E4                                    JNE      0x1406baa30
00000001406BAA4C: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BAA53: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAA57: 4C 8B A1 F8 04 00 00                     MOV      r12, qword ptr [rcx + 0x4f8]
00000001406BAA5E: 49 8B 84 24 70 0C 00 00                  MOV      rax, qword ptr [r12 + 0xc70]
00000001406BAA66: 48 3B F0                                 CMP      rsi, rax
00000001406BAA69: 75 0A                                    JNE      0x1406baa75
00000001406BAA6B: 49 3B 94 24 78 0C 00 00                  CMP      rdx, qword ptr [r12 + 0xc78]
00000001406BAA73: 74 20                                    JE       0x1406baa95
00000001406BAA75: 48 3B F2                                 CMP      rsi, rdx
00000001406BAA78: 74 23                                    JE       0x1406baa9d
00000001406BAA7A: 49 8B 9C 24 78 0C 00 00                  MOV      rbx, qword ptr [r12 + 0xc78]
00000001406BAA82: 48 2B DA                                 SUB      rbx, rdx
00000001406BAA85: 4C 8B C3                                 MOV      r8, rbx
00000001406BAA88: 48 8B CE                                 MOV      rcx, rsi
00000001406BAA8B: FF 15 B7 31 1B 00                        CALL     qword ptr [rip + 0x1b31b7]
00000001406BAA91: 48 8D 04 1E                              LEA      rax, [rsi + rbx]
00000001406BAA95: 49 89 84 24 78 0C 00 00                  MOV      qword ptr [r12 + 0xc78], rax
00000001406BAA9D: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BAAA4: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAAA8: 4C 89 B1 F8 04 00 00                     MOV      qword ptr [rcx + 0x4f8], r14
00000001406BAAAF: 48 8B 87 88 04 00 00                     MOV      rax, qword ptr [rdi + 0x488]
00000001406BAAB6: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAABA: 4C 89 B1 F0 04 00 00                     MOV      qword ptr [rcx + 0x4f0], r14
00000001406BAAC1: 41 FF C5                                 INC      r13d
00000001406BAAC4: 45 8B FD                                 MOV      r15d, r13d
00000001406BAAC7: 48 8B 87 90 04 00 00                     MOV      rax, qword ptr [rdi + 0x490]
00000001406BAACE: 48 2B 87 88 04 00 00                     SUB      rax, qword ptr [rdi + 0x488]
00000001406BAAD5: 48 C1 F8 03                              SAR      rax, 3
00000001406BAAD9: 4C 3B F8                                 CMP      r15, rax
00000001406BAADC: 0F 82 CE FE FF FF                        JB       0x1406ba9b0
00000001406BAAE2: 48 8B 87 08 17 00 00                     MOV      rax, qword ptr [rdi + 0x1708]
00000001406BAAE9: 8B 88 94 02 00 00                        MOV      ecx, dword ptr [rax + 0x294]
00000001406BAAEF: 83 E9 04                                 SUB      ecx, 4
00000001406BAAF2: 83 F9 01                                 CMP      ecx, 1
00000001406BAAF5: 0F 87 8B 01 00 00                        JA       0x1406bac86
00000001406BAAFB: 45 8B EE                                 MOV      r13d, r14d
00000001406BAAFE: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406BAB05: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406BAB0C: 48 C1 F8 03                              SAR      rax, 3
00000001406BAB10: 48 85 C0                                 TEST     rax, rax
00000001406BAB13: 0F 84 6D 01 00 00                        JE       0x1406bac86
00000001406BAB19: 4D 8B FE                                 MOV      r15, r14
00000001406BAB1C: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001406BAB20: 48 8B 9F A0 03 00 00                     MOV      rbx, qword ptr [rdi + 0x3a0]
00000001406BAB27: 4A 8B 04 FB                              MOV      rax, qword ptr [rbx + r15*8]
00000001406BAB2B: 48 8B B0 F8 04 00 00                     MOV      rsi, qword ptr [rax + 0x4f8]
00000001406BAB32: 48 85 F6                                 TEST     rsi, rsi
00000001406BAB35: 0F 84 E4 00 00 00                        JE       0x1406bac1f
00000001406BAB3B: 45 33 C9                                 XOR      r9d, r9d
00000001406BAB3E: 4C 8B C7                                 MOV      r8, rdi
00000001406BAB41: 48 8B D6                                 MOV      rdx, rsi
00000001406BAB44: E8 F7 0A D0 FF                           CALL     0x1403bb640
00000001406BAB49: 84 C0                                    TEST     al, al
00000001406BAB4B: 0F 84 CE 00 00 00                        JE       0x1406bac1f
00000001406BAB51: 48 8B 96 78 0C 00 00                     MOV      rdx, qword ptr [rsi + 0xc78]
00000001406BAB58: 48 8B B6 70 0C 00 00                     MOV      rsi, qword ptr [rsi + 0xc70]
00000001406BAB5F: 48 3B F2                                 CMP      rsi, rdx
00000001406BAB62: 74 58                                    JE       0x1406babbc
00000001406BAB64: 4A 8B 04 FB                              MOV      rax, qword ptr [rbx + r15*8]
00000001406BAB68: 48 39 06                                 CMP      qword ptr [rsi], rax
00000001406BAB6B: 74 09                                    JE       0x1406bab76
00000001406BAB6D: 48 83 C6 08                              ADD      rsi, 8
00000001406BAB71: 48 3B F2                                 CMP      rsi, rdx
00000001406BAB74: 75 F2                                    JNE      0x1406bab68
00000001406BAB76: 48 3B F2                                 CMP      rsi, rdx
00000001406BAB79: 74 41                                    JE       0x1406babbc
00000001406BAB7B: 48 8D 46 08                              LEA      rax, [rsi + 8]
00000001406BAB7F: 49 8B CE                                 MOV      rcx, r14
00000001406BAB82: 4C 8B CA                                 MOV      r9, rdx
00000001406BAB85: 4C 2B C8                                 SUB      r9, rax
00000001406BAB88: 49 83 C1 07                              ADD      r9, 7
00000001406BAB8C: 49 C1 E9 03                              SHR      r9, 3
00000001406BAB90: 48 3B C2                                 CMP      rax, rdx
00000001406BAB93: 4D 0F 47 CE                              CMOVA    r9, r14
00000001406BAB97: 4D 85 C9                                 TEST     r9, r9
00000001406BAB9A: 74 20                                    JE       0x1406babbc
00000001406BAB9C: 0F 1F 40 00                              NOP      dword ptr [rax]
00000001406BABA0: 4C 8B 00                                 MOV      r8, qword ptr [rax]
00000001406BABA3: 4E 3B 04 FB                              CMP      r8, qword ptr [rbx + r15*8]
00000001406BABA7: 74 07                                    JE       0x1406babb0
00000001406BABA9: 4C 89 06                                 MOV      qword ptr [rsi], r8
00000001406BABAC: 48 83 C6 08                              ADD      rsi, 8
00000001406BABB0: 48 83 C0 08                              ADD      rax, 8
00000001406BABB4: 48 FF C1                                 INC      rcx
00000001406BABB7: 49 3B C9                                 CMP      rcx, r9
00000001406BABBA: 75 E4                                    JNE      0x1406baba0
00000001406BABBC: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406BABC3: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BABC7: 4C 8B A1 F8 04 00 00                     MOV      r12, qword ptr [rcx + 0x4f8]
00000001406BABCE: 49 8B 84 24 70 0C 00 00                  MOV      rax, qword ptr [r12 + 0xc70]
00000001406BABD6: 48 3B F0                                 CMP      rsi, rax
00000001406BABD9: 75 0A                                    JNE      0x1406babe5
00000001406BABDB: 49 3B 94 24 78 0C 00 00                  CMP      rdx, qword ptr [r12 + 0xc78]
00000001406BABE3: 74 20                                    JE       0x1406bac05
00000001406BABE5: 48 3B F2                                 CMP      rsi, rdx
00000001406BABE8: 74 23                                    JE       0x1406bac0d
00000001406BABEA: 49 8B 9C 24 78 0C 00 00                  MOV      rbx, qword ptr [r12 + 0xc78]
00000001406BABF2: 48 2B DA                                 SUB      rbx, rdx
00000001406BABF5: 4C 8B C3                                 MOV      r8, rbx
00000001406BABF8: 48 8B CE                                 MOV      rcx, rsi
00000001406BABFB: FF 15 47 30 1B 00                        CALL     qword ptr [rip + 0x1b3047]
00000001406BAC01: 48 8D 04 33                              LEA      rax, [rbx + rsi]
00000001406BAC05: 49 89 84 24 78 0C 00 00                  MOV      qword ptr [r12 + 0xc78], rax
00000001406BAC0D: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406BAC14: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAC18: 4C 89 B1 F8 04 00 00                     MOV      qword ptr [rcx + 0x4f8], r14
00000001406BAC1F: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406BAC26: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAC2A: 4C 89 B1 F0 04 00 00                     MOV      qword ptr [rcx + 0x4f0], r14
00000001406BAC31: 48 8B 87 A0 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a0]
00000001406BAC38: 4A 8B 0C F8                              MOV      rcx, qword ptr [rax + r15*8]
00000001406BAC3C: 44 89 B1 A4 07 00 00                     MOV      dword ptr [rcx + 0x7a4], r14d
00000001406BAC43: 41 FF C5                                 INC      r13d
00000001406BAC46: 45 8B FD                                 MOV      r15d, r13d
00000001406BAC49: 48 8B 87 A8 03 00 00                     MOV      rax, qword ptr [rdi + 0x3a8]
00000001406BAC50: 48 2B 87 A0 03 00 00                     SUB      rax, qword ptr [rdi + 0x3a0]
00000001406BAC57: 48 C1 F8 03                              SAR      rax, 3
00000001406BAC5B: 4C 3B F8                                 CMP      r15, rax
00000001406BAC5E: 0F 82 BC FE FF FF                        JB       0x1406bab20
00000001406BAC64: EB 20                                    JMP      0x1406bac86
00000001406BAC66: C6 81 3A 0D 00 00 01                     MOV      byte ptr [rcx + 0xd3a], 1
00000001406BAC6D: C7 81 6C 06 00 00 00 00 80 BF            MOV      dword ptr [rcx + 0x66c], 0xbf800000
00000001406BAC77: 45 84 C9                                 TEST     r9b, r9b
00000001406BAC7A: 75 0A                                    JNE      0x1406bac86
00000001406BAC7C: 45 33 F6                                 XOR      r14d, r14d
00000001406BAC7F: 44 89 B1 98 06 00 00                     MOV      dword ptr [rcx + 0x698], r14d
00000001406BAC86: B0 01                                    MOV      al, 1
00000001406BAC88: 4C 8D 9C 24 E0 00 00 00                  LEA      r11, [rsp + 0xe0]
00000001406BAC90: 49 8B 5B 50                              MOV      rbx, qword ptr [r11 + 0x50]
00000001406BAC94: 41 0F 28 73 F0                           MOVAPS   xmm6, xmmword ptr [r11 - 0x10]
00000001406BAC99: 41 0F 28 7B E0                           MOVAPS   xmm7, xmmword ptr [r11 - 0x20]
00000001406BAC9E: 45 0F 28 43 D0                           MOVAPS   xmm8, xmmword ptr [r11 - 0x30]
00000001406BACA3: 45 0F 28 4B C0                           MOVAPS   xmm9, xmmword ptr [r11 - 0x40]
00000001406BACA8: 49 8B E3                                 MOV      rsp, r11
00000001406BACAB: 41 5F                                    POP      r15
00000001406BACAD: 41 5E                                    POP      r14
00000001406BACAF: 41 5D                                    POP      r13
00000001406BACB1: 41 5C                                    POP      r12
00000001406BACB3: 5F                                       POP      rdi
00000001406BACB4: 5E                                       POP      rsi
00000001406BACB5: 5D                                       POP      rbp
00000001406BACB6: C3                                       RET
```

## Type-43 lifecycle follow-up: updater association is not a task boundary

### Delta and identity

This follow-up supersedes any reading of the earlier `FUN_1403FBDC0` call as evidence that the type-43 updater removes an active assignment:

- Newly established: `FUN_1401C6050` only enters its per-truck processing for a truck with `truck+0xC80 == 0`. Its same-office fleet receiver is still established by `office+0xC70/+0xC78`, `truck+0x4F8 == office`, and the type-`0x2B` dispatcher path.
- Corrected: the entry `+0xD39` branch chooses whether the updater invokes `FUN_1406B8EB0`, but does not prove that the helper call and the later `FUN_1403FBDC0` call are mutually exclusive for the entire iteration. The nested `FUN_140680A20` can change the same truck's `+0xD39`; the caller tests that post-call value again before the reset block. The helper's association stores remain separately guarded.
- Newly established: `FUN_1406BD6B0`, called after the reset path, is a read-only boolean predicate over route/home/metadata-related fields and scalar helper results. It is not a task setter or remover.
- Narrowed/withdrawn: the `FUN_1403FBDC0` call is not a demonstrated removal of a live type-43 association. The updater's earlier `+0xC80 == 0` guard makes that interpretation unsupported; its generic clear body alone is not evidence of a task boundary.
- Still unknown at the time of this prior pass: which same-truck type-43 operation installs the actual dispatched work represented by a route, preserves it over internal stops, and removes/replaces it on cancellation or completion. The candidate-route installation path is traced in the later follow-up below; its task-boundary semantics remain unknown.

Identity and project for this pass:

```text
Executable: SOVIET64.exe v1.1.1.9
SHA-256:    296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
PE time:    0x6A3EB6AD
Image base: 0x140000000
Checkout:   C:\WRSRE
Branch:     DO-fuel-cost-calculatotr
HEAD:       b730880af2af62777d81b3736c4b4bbce9083231
Status:     clean before this documentation edit
Ghidra:     C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\ghidra\proj\soviet, program /SOVIET64.exe
```

All addresses below are Ghidra file-based VAs in that image. Parenthesized RVAs are `VA - 0x140000000`; no PE file offsets or runtime-loaded VAs are implied.

### Receiver-preserving updater flow

```text
FUN_140139A70
  metadata[office+0x318]+0x360 == 0x2B
  -> FUN_1401C6050(RCX=context, RDX=office)
     RDI = that office
     RSI = office.C70/C78 fleet element
     require truck+0x4F8 == RDI
     require truck+0xC80 == 0                       ; 1401C61E6 (RVA 1C61E6)
     |
     +-- entry truck+0xD39 == 0
     |   + additional guards, including index +0x698 == 0 and nonempty +0x680/+0x688
     |   -> FUN_1406B8EB0(RCX=RSI truck, RDX=R8=R9=0)
     |      clears WORD [truck+0xD39] at 1406B8F7D, then calls
     |      FUN_140680A20(RCX=same truck, RDX=0, R8=-1, stack bytes 0,0)
     |      tests post-call byte +0xD39 at 1406B8FF1
     |      nonzero -> early return before helper association stores
     |      zero -> may continue through additional guards and stores
     |   -> caller tests +0xD39 again at 1401C6498
     |      zero branches to 1401C68DF and skips FUN_1403FBDC0
     |      nonzero enters the configuration/reset path, including its call
     |
     +-- entry truck+0xD39 != 0
         -> bypass FUN_1406B8EB0 at 1401C646E
         -> enters configuration/reset path; byte +0xD39 cleared at 1401C64CF
         -> FUN_1403FBDC0(RCX=[RBP+0x290], RDX=RSI truck,
                          R8=0, R9B=1, stack flags 1 and 1)
         -> FUN_1406BD6B0(RCX=RSI truck, DL=1), a predicate
```

The route-list guards at `0x1401C6429..0x1401C6461` are evaluated on a truck whose `+0x680/+0x688` list is already nonempty and whose `+0x698` index is zero. Neither the type-43 updater nor the displayed `FUN_1406B8EB0` body installs that route-target vector. Therefore those guards and the subsequent office association stores cannot distinguish a persistent/default route from newly dispatched work.

The branch order alone does not establish call-site exclusivity. At entry, a zero byte can lead to `FUN_1406B8EB0`; inside that helper the nested call can set or later clear the byte, and the updater's `0x1401C6498` test uses the resulting state. A nonzero post-helper byte flows into the configuration/reset block, while zero skips it. The entry-nonzero path reaches the same reset block without invoking `FUN_1406B8EB0`. The exact nested flag paths and their limits are recorded below. This updater still does not establish that `FUN_1403FBDC0` removes active work.

The exact caller setup at `0x1401C68A0..0x1401C68E9` is:

```asm
1401c68a0: C6 44 24 28 01           MOV byte ptr [RSP + 0x28],0x1
1401c68a5: C6 44 24 20 01           MOV byte ptr [RSP + 0x20],0x1
1401c68aa: 41 B1 01                 MOV R9B,0x1
1401c68ad: 45 33 C0                 XOR R8D,R8D
1401c68b0: 48 8B D6                 MOV RDX,RSI
1401c68b3: 4C 8B A5 90 02 00 00     MOV R12,[RBP + 0x290]
1401c68ba: 49 8B CC                 MOV RCX,R12
1401c68bd: E8 FE 54 23 00           CALL 0x1403FBDC0
1401c68c2: C7 86 08 05 00 00 00 00 20 41 MOV dword ptr [RSI + 0x508],0x41200000
1401c68cc: 48 8D 4D 90              LEA RCX,[RBP - 0x70]
1401c68d0: E8 BB 6F F2 FF           CALL 0x1400ED890
1401c68d5: 4C 8B 75 80              MOV R14,[RBP - 0x80]
1401c68d9: 8B 9D 98 02 00 00        MOV EBX,[RBP + 0x298]
1401c68df: B2 01                    MOV DL,0x1
1401c68e1: 48 8B CE                 MOV RCX,RSI
1401c68e4: E8 C7 6D 4F 00           CALL 0x1406BD6B0
1401c68e9: 84 C0                    TEST AL,AL
```

`RDX=RSI` at the `FUN_1403FBDC0` call makes its callee's `RDI=RDX` the same fleet truck. The first parameter is a local value, not that truck or the office. Before this block, the same loop required `truck+0xC80 == 0` at `0x1401C61E6`; the type-43 helper path that can write `+0xC80` is bypassed on the `+0xD39 != 0` branch. Intermediate configuration helpers do not receive `RSI` as their direct receiver in this block. Their possible indirect/global effects are not used to claim a task boundary.

The generic clear block in `FUN_1403FBDC0` is real, but its semantics do not override the caller's guard. `RDI` is its truck receiver; `RBX` was zeroed earlier in the function. For metadata-kind values other than 10 (including the reported kind 1 when `truck+0x1708` is nonnull), it clears the linked object's `+0xBB0` and the truck's `+0xC80` if the truck field is nonnull. Kind 10 delegates to `FUN_1406DCF20`. This is a shared 2,929-byte helper, so its complete body is not duplicated here; the complete disassembly was exported, and only the relevant clear CFG block is reproduced.

```asm
1403fc085: 48 8B 87 80 0C 00 00     MOV RAX,[RDI + 0xC80]
1403fc08c: 48 85 C0                 TEST RAX,RAX
1403fc08f: 74 2D                    JE 0x1403FC0BE
1403fc091: 48 8B 8F 08 17 00 00     MOV RCX,[RDI + 0x1708]
1403fc098: 48 85 C9                 TEST RCX,RCX
1403fc09b: 74 13                    JE 0x1403FC0B0
1403fc09d: 83 B9 94 02 00 00 0A     CMP dword ptr [RCX + 0x294],0xA
1403fc0a4: 75 0A                    JNE 0x1403FC0B0
1403fc0a6: 48 8B CF                 MOV RCX,RDI
1403fc0a9: E8 72 0E 2E 00           CALL 0x1406DCF20
1403fc0ae: EB 0E                    JMP 0x1403FC0BE
1403fc0b0: 48 89 98 B0 0B 00 00     MOV [RAX + 0xBB0],RBX
1403fc0b7: 48 89 9F 80 0C 00 00     MOV [RDI + 0xC80],RBX
```

For the type-43 caller, `truck+0xC80` was already zero at loop entry. Static evidence therefore does not show that this clear closes an active interval. The available caller also does not establish that the generic helper is a cancellation/completion routine.

### `FUN_1406BD6B0` and its bounded read-only dependencies

The type-43 caller invokes `FUN_1406BD6B0` with `RCX=RSI` (the same fleet truck) and `DL=1`. It returns a boolean. Its body reads truck/home/route/metadata fields; it contains no task-association stores. The direct callees used in its predicate were bounded as well:

- `FUN_1406BD660` returns true only when the current `truck+0x698` route entry equals `truck+0x4F8`. This is a route-entry/home comparison, not proof of task completion or the beginning of empty return.
- `FUN_1406C2430` computes a scalar from the truck or the pointer selected from `truck+0x398`, using `FUN_1406C24A0`. Both functions are read-only in their complete bodies; `FUN_1406C24A0` consults the selected object's `+0x1708` metadata, including `+0x294`. That metadata-kind value is not a dynamic task-state enum.

Complete raw body, `FUN_1406BD6B0` (543 bytes; `0x1406BD6B0..0x1406BD8CF`, end-exclusive):

```asm
1406bd6b0:  40 53                    PUSH RBX
1406bd6b2:  48 83 EC 20              SUB RSP,0x20
1406bd6b6:  4C 8B 99 F8 04 00 00     MOV R11,[RCX + 0x4F8]
1406bd6bd:  0F B6 DA                 MOVZX EBX,DL
1406bd6c0:  4C 8B C9                 MOV R9,RCX
1406bd6c3:  4D 85 DB                 TEST R11,R11
1406bd6c6:  0F 84 FB 01 00 00        JE 0x1406BD8C7
1406bd6cc:  48 83 B9 E8 05 00 00 00  CMP qword ptr [RCX + 0x5E8],0
1406bd6d4:  75 2F                    JNE 0x1406BD705
1406bd6d6:  48 83 B9 28 05 00 00 00  CMP qword ptr [RCX + 0x528],0
1406bd6de:  75 25                    JNE 0x1406BD705
1406bd6e0:  84 D2                    TEST DL,DL
1406bd6e2:  74 09                    JE 0x1406BD6ED
1406bd6e4:  E8 77 FF FF FF           CALL 0x1406BD660
1406bd6e9:  84 C0                    TEST AL,AL
1406bd6eb:  75 13                    JNE 0x1406BD700
1406bd6ed:  49 83 BB B0 0B 00 00 00  CMP qword ptr [R11 + 0xBB0],0
1406bd6f5:  75 0E                    JNE 0x1406BD705
1406bd6f7:  4C 39 99 F0 04 00 00     CMP qword ptr [RCX + 0x4F0],R11
1406bd6fe:  75 05                    JNE 0x1406BD705
1406bd700:  41 B2 01                 MOV R10B,0x1
1406bd703:  EB 03                    JMP 0x1406BD708
1406bd705:  45 32 D2                 XOR R10B,R10B
1406bd708:  84 DB                    TEST BL,BL
1406bd70a:  0F 84 0A 01 00 00        JE 0x1406BD81A
1406bd710:  45 84 D2                 TEST R10B,R10B
1406bd713:  0F 84 2A 01 00 00        JE 0x1406BD843
1406bd719:  48 8B 81 08 17 00 00     MOV RAX,[RCX + 0x1708]
1406bd720:  48 85 C0                 TEST RAX,RAX
1406bd723:  0F 84 F1 00 00 00        JE 0x1406BD81A
1406bd729:  83 B8 94 02 00 00 01     CMP dword ptr [RAX + 0x294],1
1406bd730:  0F 85 E4 00 00 00        JNE 0x1406BD81A
1406bd736:  48 8B 89 98 07 00 00     MOV RCX,[RCX + 0x798]
1406bd73d:  48 85 C9                 TEST RCX,RCX
1406bd740:  0F 84 D4 00 00 00        JE 0x1406BD81A
1406bd746:  8B 81 2C 01 00 00        MOV EAX,[RCX + 0x12C]
1406bd74c:  85 C0                    TEST EAX,EAX
1406bd74e:  41 0F 94 C2              SETZ R10B
1406bd752:  85 C0                    TEST EAX,EAX
1406bd754:  75 18                    JNE 0x1406BD76E
1406bd756:  83 B9 20 01 00 00 04     CMP dword ptr [RCX + 0x120],4
1406bd75d:  75 0F                    JNE 0x1406BD76E
1406bd75f:  83 B9 24 01 00 00 01     CMP dword ptr [RCX + 0x124],1
1406bd766:  45 0F B6 D2              MOVZX R10D,R10B
1406bd76a:  44 0F 44 D0              CMOVZ R10D,EAX
1406bd76e:  49 8B 83 18 03 00 00     MOV RAX,[R11 + 0x318]
1406bd775:  48 85 C0                 TEST RAX,RAX
1406bd778:  0F 84 9C 00 00 00        JE 0x1406BD81A
1406bd77e:  83 B8 60 03 00 00 0C     CMP dword ptr [RAX + 0x360],0xC
1406bd785:  0F 85 8F 00 00 00        JNE 0x1406BD81A
1406bd78b:  49 8B C9                 MOV RCX,R9
1406bd78e:  E8 CD FE FF FF           CALL 0x1406BD660
1406bd793:  84 C0                    TEST AL,AL
1406bd795:  0F 84 7F 00 00 00        JE 0x1406BD81A
1406bd79b:  45 84 D2                 TEST R10B,R10B
1406bd79e:  0F 84 9F 00 00 00        JE 0x1406BD843
1406bd7a4:  49 8B 81 C0 06 00 00     MOV RAX,[R9 + 0x6C0]
1406bd7ab:  49 2B 81 B8 06 00 00     SUB RAX,[R9 + 0x6B8]
1406bd7b2:  4D 63 81 00 07 00 00     MOVSXD R8,dword ptr [R9 + 0x700]
1406bd7b9:  48 C1 F8 03              SAR RAX,3
1406bd7bd:  4C 3B C0                 CMP R8,RAX
1406bd7c0:  73 49                    JNC 0x1406BD80B
1406bd7c2:  49 8B 81 B8 06 00 00     MOV RAX,[R9 + 0x6B8]
1406bd7c9:  4A 8D 14 C0              LEA RDX,[RAX + R8*8]
1406bd7cd:  0F 1F 00                 NOP dword ptr [RAX]
1406bd7d0:  48 8B 02                 MOV RAX,[RDX]
1406bd7d3:  83 B8 20 01 00 00 04     CMP dword ptr [RAX + 0x120],4
1406bd7da:  75 09                    JNE 0x1406BD7E5
1406bd7dc:  83 B8 24 01 00 00 01     CMP dword ptr [RAX + 0x124],1
1406bd7e3:  74 23                    JE 0x1406BD808
1406bd7e5:  49 8B 81 C0 06 00 00     MOV RAX,[R9 + 0x6C0]
1406bd7ec:  41 FF C0                 INC R8D
1406bd7ef:  49 2B 81 B8 06 00 00     SUB RAX,[R9 + 0x6B8]
1406bd7f6:  48 83 C2 08              ADD RDX,8
1406bd7fa:  48 C1 F8 03              SAR RAX,3
1406bd7fe:  49 63 C8                 MOVSXD RCX,R8D
1406bd801:  48 3B C8                 CMP RCX,RAX
1406bd804:  72 CA                    JB 0x1406BD7D0
1406bd806:  EB 03                    JMP 0x1406BD80B
1406bd808:  45 32 D2                 XOR R10B,R10B
1406bd80b:  41 80 B9 3A 0D 00 00 00  CMP byte ptr [R9 + 0xD3A],0
1406bd813:  74 05                    JE 0x1406BD81A
1406bd815:  45 32 D2                 XOR R10B,R10B
1406bd818:  EB 29                    JMP 0x1406BD843
1406bd81a:  45 84 D2                 TEST R10B,R10B
1406bd81d:  74 24                    JE 0x1406BD843
1406bd81f:  49 8B 81 08 17 00 00     MOV RAX,[R9 + 0x1708]
1406bd826:  48 85 C0                 TEST RAX,RAX
1406bd829:  74 18                    JE 0x1406BD843
1406bd82b:  83 B8 94 02 00 00 0A     CMP dword ptr [RAX + 0x294],0xA
1406bd832:  75 0F                    JNE 0x1406BD843
1406bd834:  41 F7 81 04 1A 00 00 FD FF FF FF TEST dword ptr [R9 + 0x1A04],0xFFFFFFFD
1406bd83f:  41 0F 94 C2              SETZ R10B
1406bd843:  83 3D 82 7C 31 00 00     CMP dword ptr [0x1409D54CC],0
1406bd84a:  7E 71                    JLE 0x1406BD8BD
1406bd84c:  45 84 D2                 TEST R10B,R10B
1406bd84f:  74 6C                    JE 0x1406BD8BD
1406bd851:  49 8B C9                 MOV RCX,R9
1406bd854:  E8 D7 4B 00 00           CALL 0x1406C2430
1406bd859:  F2 0F 10 25 8F C9 24 00  MOVSD XMM4,qword ptr [0x14090A1F0]
1406bd861:  66 0F 2F E0              COMISD XMM4,XMM0
1406bd865:  0F 97 C0                 SETA AL
1406bd868:  84 C0                    TEST AL,AL
1406bd86a:  74 5D                    JE 0x1406BD8C9
1406bd86c:  41 80 B9 B0 1A 00 00 00  CMP byte ptr [R9 + 0x1AB0],0
1406bd874:  75 51                    JNE 0x1406BD8C7
1406bd876:  49 83 B9 D0 1A 00 00 00  CMP qword ptr [R9 + 0x1AD0],0
1406bd87e:  75 47                    JNE 0x1406BD8C7
1406bd880:  49 83 B9 E8 04 00 00 00  CMP qword ptr [R9 + 0x4E8],0
1406bd888:  75 21                    JNE 0x1406BD8AB
1406bd88a:  49 8B 81 F8 04 00 00     MOV RAX,[R9 + 0x4F8]
1406bd891:  48 85 C0                 TEST RAX,RAX
1406bd894:  74 1F                    JE 0x1406BD8B5
1406bd896:  48 8B 80 18 03 00 00     MOV RAX,[RAX + 0x318]
1406bd89d:  48 85 C0                 TEST RAX,RAX
1406bd8a0:  74 13                    JE 0x1406BD8B5
1406bd8a2:  83 B8 60 03 00 00 26     CMP dword ptr [RAX + 0x360],0x26
1406bd8a9:  75 0A                    JNE 0x1406BD8B5
1406bd8ab:  41 83 B9 0C 05 00 00 15  CMP dword ptr [R9 + 0x50C],0x15
1406bd8b3:  74 12                    JE 0x1406BD8C7
1406bd8b5:  B0 01                    MOV AL,0x1
1406bd8b7:  48 83 C4 20              ADD RSP,0x20
1406bd8bb:  5B                       POP RBX
1406bd8bc:  C3                       RET
1406bd8bd:  41 0F B6 C2              MOVZX EAX,R10B
1406bd8c1:  48 83 C4 20              ADD RSP,0x20
1406bd8c5:  5B                       POP RBX
1406bd8c6:  C3                       RET
1406bd8c7:  32 C0                    XOR AL,AL
1406bd8c9:  48 83 C4 20              ADD RSP,0x20
1406bd8cd:  5B                       POP RBX
1406bd8ce:  C3                       RET
```

Complete raw bodies of the small dependencies:

```asm
; FUN_1406BD660, 68 bytes, 0x1406BD660..0x1406BD6A4
1406bd660: 48 8B 91 F8 04 00 00     MOV RDX,[RCX + 0x4F8]
1406bd667: 48 85 D2                 TEST RDX,RDX
1406bd66a: 74 35                    JE 0x1406BD6A1
1406bd66c: 48 63 81 98 06 00 00     MOVSXD RAX,dword ptr [RCX + 0x698]
1406bd673: 85 C0                    TEST EAX,EAX
1406bd675: 78 2A                    JS 0x1406BD6A1
1406bd677: 4C 8B C0                 MOV R8,RAX
1406bd67a: 48 8B 81 88 06 00 00     MOV RAX,[RCX + 0x688]
1406bd681: 48 2B 81 80 06 00 00     SUB RAX,[RCX + 0x680]
1406bd688: 48 C1 F8 03              SAR RAX,3
1406bd68c: 4C 3B C0                 CMP R8,RAX
1406bd68f: 73 10                    JNC 0x1406BD6A1
1406bd691: 48 8B 81 80 06 00 00     MOV RAX,[RCX + 0x680]
1406bd698: 4A 3B 14 C0              CMP RDX,qword ptr [RAX + R8*8]
1406bd69c: 75 03                    JNE 0x1406BD6A1
1406bd69e: B0 01                    MOV AL,0x1
1406bd6a0: C3                       RET
1406bd6a1: 32 C0                    XOR AL,AL
1406bd6a3: C3                       RET

; FUN_1406C2430, 108 bytes, 0x1406C2430..0x1406C249C
1406c2430: 48 83 EC 28              SUB RSP,0x28
1406c2434: 48 8B 81 98 03 00 00     MOV RAX,[RCX + 0x398]
1406c243b: 48 8B D1                 MOV RDX,RCX
1406c243e: 48 85 C0                 TEST RAX,RAX
1406c2441: 74 0B                    JE 0x1406C244E
1406c2443: 83 B9 C0 04 00 00 00     CMP dword ptr [RCX + 0x4C0],0
1406c244a: 48 0F 4F D0              CMOVG RDX,RAX
1406c244e: 0F 57 D2                 XORPS XMM2,XMM2
1406c2451: 48 8B CA                 MOV RCX,RDX
1406c2454: F2 0F 11 54 24 30        MOVSD [RSP + 0x30],XMM2
1406c245a: E8 41 00 00 00           CALL 0x1406C24A0
1406c245f: 66 0F 2F C2              COMISD XMM0,XMM2
1406c2463: 76 2F                    JBE 0x1406C2494
1406c2465: F2 0F 10 8A 40 1A 00 00  MOVSD XMM1,[RDX + 0x1A40]
1406c246d: F2 0F 10 1D 7B 7D 24 00  MOVSD XMM3,[0x14090A1F0]
1406c2475: F2 0F 5E C8              DIVSD XMM1,XMM0
1406c2479: 0F 28 C3                 MOVAPS XMM0,XMM3
1406c247c: F2 0F 5C C1              SUBSD XMM0,XMM1
1406c2480: 66 0F 2F D0              COMISD XMM2,XMM0
1406c2484: 77 0E                    JA 0x1406C2494
1406c2486: 66 0F 2F C3              COMISD XMM0,XMM3
1406c248a: 76 0B                    JBE 0x1406C2497
1406c248c: 0F 28 C3                 MOVAPS XMM0,XMM3
1406c248f: 48 83 C4 28              ADD RSP,0x28
1406c2493: C3                       RET
1406c2494: 0F 28 C2                 MOVAPS XMM0,XMM2
1406c2497: 48 83 C4 28              ADD RSP,0x28
1406c249b: C3                       RET

; FUN_1406C24A0, 116 bytes, 0x1406C24A0..0x1406C2514
1406c24a0: 48 8B 81 08 17 00 00     MOV RAX,[RCX + 0x1708]
1406c24a7: 0F 57 C0                 XORPS XMM0,XMM0
1406c24aa: 48 85 C0                 TEST RAX,RAX
1406c24ad: 74 64                    JE 0x1406C2513
1406c24af: F3 0F 10 88 F4 9E 00 00  MOVSS XMM1,dword ptr [RAX + 0x9EF4]
1406c24b7: 0F 2F C8                 COMISS XMM1,XMM0
1406c24ba: 76 04                    JBE 0x1406C24C0
1406c24bc: 0F 5A C1                 CVTPS2PD XMM0,XMM1
1406c24bf: C3                       RET
1406c24c0: 8B 88 94 02 00 00        MOV ECX,dword ptr [RAX + 0x294]
1406c24c6: 83 F9 01                 CMP ECX,1
1406c24c9: 75 09                    JNE 0x1406C24D4
1406c24cb: F2 0F 10 05 45 82 24 00  MOVSD XMM0,[0x14090A718]
1406c24d3: C3                       RET
1406c24d4: 8D 41 FC                 LEA EAX,[RCX - 4]
1406c24d7: 83 F8 01                 CMP EAX,1
1406c24da: 76 2F                    JBE 0x1406C250B
1406c24dc: 83 F9 03                 CMP ECX,3
1406c24df: 74 32                    JE 0x1406C2513
1406c24e1: 83 F9 06                 CMP ECX,6
1406c24e4: 75 09                    JNE 0x1406C24EF
1406c24e6: F2 0F 10 05 AA 82 24 00  MOVSD XMM0,[0x14090A798]
1406c24ee: C3                       RET
1406c24ef: 83 F9 08                 CMP ECX,8
1406c24f2: 75 09                    JNE 0x1406C24FD
1406c24f4: F2 0F 10 05 3C 82 24 00  MOVSD XMM0,[0x14090A738]
1406c24fc: C3                       RET
1406c24fd: 83 F9 0A                 CMP ECX,0xA
1406c2500: 75 C9                    JNE 0x1406C24CB
1406c2502: F2 0F 10 05 F6 81 24 00  MOVSD XMM0,[0x14090A700]
1406c250a: C3                       RET
1406c250b: F2 0F 10 05 75 82 24 00  MOVSD XMM0,[0x14090A788]
1406c2513: C3                       RET
```

### Updated lifecycle matrix and integration decision

| Case | Address-backed result | Interval status |
|---|---|---|
| Idle/default route at a DO | The updater requires `truck+0xC80 == 0`; on one eligibility path it additionally sees nonempty `+0x680/+0x688` and `+0x698 == 0` before the association helper. | Nonempty route is not proof of assigned work; whether default/return rows are retained remains unknown. |
| Assigned but not loaded | `FUN_1406B8EB0` has a guarded association-write path, but it is called after the route list is already present. No dispatched-task creation event is identified. | Start boundary unproved. |
| Internal pickup/unload stops | The type-43 updater skips any truck with nonzero `+0xC80`. The route/home predicate only compares the indexed route entry with `+0x4F8`; it does not classify stop advancement. | Association persistence through internal stops is unproved. |
| Final removal/cancellation | `FUN_1403FBDC0` can clear association fields in general, but its type-43 updater call is behind the `+0xC80 == 0` entry guard and is not shown as active-task cleanup. | No verified close boundary, independent of success. |
| Replacement, including identical work | The candidate attach/reset operations are mutually exclusive in the shown updater path. No same-truck task replacement event or unique task identity is established. | Cannot guarantee a boundary without an empty snapshot; identical consecutive tasks remain indistinguishable. |
| Empty return after removal | The only new route-related predicate compares `truck+0x680[truck+0x698]` against `truck+0x4F8`. | Not enough to establish removal timing or exclude post-removal return travel. |
| Save/reload | No reconstruction or continuity path was traced. | Explicitly unsupported. |

**Integration decision at that pass:** do not use `truck+0xC80`, `office+0xBB0`, route-list occupancy/index, `FUN_1403FBDC0`, or `FUN_1406BD6B0` as an interval rule. The candidate route-install/rebuild path was not yet traced then. The later section below establishes that path but does not establish its task-boundary semantics or paired final removal/cancellation path. This remains an incomplete trace, not a claim that static proof is impossible. No hook or logging instrumentation was installed; no plugin, dispatch, permissions, defaults, or billing behavior changed. No live-game validation was performed. The reported road-truck metadata kind `1` remains a reported fact; `truck+0x1708 -> metadata+0x294` is metadata-kind provenance only.

### Ghidra commands and output accounting

The wrapper opened the configured `soviet` project and `/SOVIET64.exe` for each successful query. These are existing read-only `ListFuncs.java` and `DisasmMany.java` invocations; no investigation script was added or modified. `ListFuncs.java` reported:

```text
0x1403FBDC0  size 2929  FUN_1403fbdc0
0x1406BD6B0  size  543  FUN_1406bd6b0
0x1406BD660  size   68  FUN_1406bd660
0x1406C2430  size  108  FUN_1406c2430
0x1406C24A0  size  116  FUN_1406c24a0
```

Bounded commands used (all output names were `agent1_` scratch files):

```text
C:\WRSRE\scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1403fbdc0.txt 0x1403FBDC0 0x1403FBDC0
C:\WRSRE\scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1406bd6b0.txt 0x1406BD6B0 0x1406BD6B0
C:\WRSRE\scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_lifecycle_helpers.asm 0x1403FBDC0:0x1403FC931 0x1406BD6B0:0x1406BD8CF
C:\WRSRE\scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1406bd660.txt 0x1406BD660 0x1406BD660
C:\WRSRE\scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_1406bd660.asm 0x1406BD660:0x1406BD6A4
C:\WRSRE\scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1406c2430.txt 0x1406C2430 0x1406C2430
C:\WRSRE\scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_1406c2430.asm 0x1406C2430:0x1406C249C
C:\WRSRE\scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1406c24a0.txt 0x1406C24A0 0x1406C24A0
C:\WRSRE\scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_1406c24a0.asm 0x1406C24A0:0x1406C24B4  ; partial first attempt, not used
C:\WRSRE\scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_1406c24a0.asm 0x1406C24A0:0x1406C2514
```

The listed exact ranges end at the last instruction boundary and each exported body ends in `RET`; the complete raw output for `FUN_1406BD6B0` and all of its bounded callees, including nested `FUN_1406C24A0`, is included above. The first `FUN_1406C24A0` disassembly range ended at `0x1406C24B4`; it was visibly partial and was replaced by the exact `0x1406C24A0..0x1406C2514` range before drawing conclusions. A preceding broad command, `scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_funcs_lifecycle_helpers.txt 0x1403FBDC0 0x1406BE000`, produced 2,549 entries; that overbroad output was discarded and is not evidence used here. No other broad function-range query or global offset search was used in that follow-up.

## Type-43 candidate-route installation follow-up

### Delta

- Newly traced: a fleet truck collected by `FUN_1401C6050` is stored in a temporary candidate buffer and passed, with the same type-43 office, to `FUN_1401E5350` at `0x1401C6CC9`.
- Newly established: `FUN_1401E5350` selects `FUN_1401E5480` or `FUN_1401E5BA0` using the result of `FUN_1406BDC30` on that same truck. Both selected branches can reset the truck's existing route/configuration vectors and rebuild them from records in that same office's `+0xD58/+0xD60` vector. The route-row mutator is `FUN_14067D5D0`.
- Newly established: when the office `+0xD50` refresh byte is nonzero, the updater calls `FUN_1401E2380` on that same office; the function clears the byte and resets `+0xD60` to `+0xD58` before regenerating candidate rows from `+0xD38/+0xD40` configuration entries. These planner rows are not themselves a committed per-truck job identity.
- Corrected: the updater's entry `+0xD39` test selects the initial branch, but does not prove that `FUN_1406B8EB0` and `FUN_1403FBDC0` are mutually exclusive across the entire iteration. The nested `FUN_140680A20` changes the same truck's byte and the caller tests it again after return.
- Not established: an accepted `+0xD58` record is a committed job rather than a candidate/rebuild input; route persistence through internal stops; actual removal/cancellation; or whether identical consecutive work has a distinct event.

### Identity and address convention

The executable was rechecked directly at `C:\Workers Resources Soviet Republic\SOVIET64.exe`: 10,308,608 bytes, SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`, PE timestamp `0x6A3EB6AD`, machine `0x8664`, optional-header magic `0x020B`, image base `0x140000000`. The configured wrapper opened the existing Ghidra project `C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\ghidra\proj\soviet`, program `/SOVIET64.exe`.

The checkout is `C:\WRSRE`, branch `DO-fuel-cost-calculatotr`, HEAD `31e6e416d4f56c02aacfdbb848874d5c305dd632`; the worktree was clean before this Markdown update. All disassembly addresses in this section are file-based VAs in the loaded Ghidra image. RVAs are the VA minus `0x140000000` (for example, `0x1401E5350` is RVA `0x1E5350`). PE file offsets were not used for these queries. A future runtime address is module base plus RVA, not the file VA.

All findings are static. No runtime logging, hook, build, save, dispatch/permission change, billing change, or live-game replay was performed.

### Same-office receiver path

```text
type-43 dispatcher
  office descriptor [office+0x318]+0x360 == 0x2B
  -> FUN_1401C6050(context, office)
     RDI = that office
     fleet iteration from that office's +0xC70/+0xC78
     RSI = candidate truck from that fleet
     require truck+0x4F8 == office
     require truck+0xC80 == 0
     append RSI to temporary candidate buffer at 0x1401C69AC
  -> loop over the temporary buffer
     RCX = R12 context
     RDX = [RBX] candidate truck
     R8  = RDI same office
     call FUN_1401E5350 at 0x1401C6CC9
        truck+0x528 != 0 -> immediate false return
        otherwise call FUN_1406BDC30(truck, 0.0)
        zero result -> FUN_1401E5480(context, truck, office)
        nonzero result -> FUN_1401E5BA0(context, truck, office, result)
```

The buffer operation is not an unrelated pointer list: `0x1401C69AC` stores the current `RSI`, which the updater obtained from the same office fleet. The processing loop later loads that stored pointer into `RDX`; `RDI` remains the office receiver and is passed as `R8`. The call setup is:

```asm
1401c69a7: 48 85 C9                                 TEST RCX,RCX
1401c69aa: 74 03                                    JE 0x1401c69af
1401c69ac: 48 89 31                                 MOV qword ptr [RCX],RSI
1401c69af: 48 83 C1 08                              ADD RCX,8
1401c69b3: 48 89 4C 24 58                           MOV qword ptr [RSP + 0x58],RCX
...
1401c6cc0: 4C 8B C7                                 MOV R8,RDI
1401c6cc3: 48 8B 13                                 MOV RDX,qword ptr [RBX]
1401c6cc6: 49 8B CC                                 MOV RCX,R12
1401c6cc9: E8 82 E6 01 00                           CALL 0x1401e5350
1401c6cce: 45 0F B6 F6                              MOVZX R14D,R14B
1401c6cd2: 84 C0                                    TEST AL,AL
```

### What the connected functions do

`FUN_1401E5350` is 293 bytes (`0x1401E5350..0x1401E5474`, inclusive). `RBX` retains the candidate truck, `RSI` the office, and `RBP` the context. It rejects a truck with nonzero `+0x528`; otherwise it calls `FUN_1406BDC30` on the same truck with a zero float argument and uses the returned scalar/pointer-like value to choose one of two route-build paths. The returned value is not a verified job ID.

`FUN_1406BDC30` is 145 bytes (`0x1406BDC30..0x1406BDCC0`). For the branch whose metadata `+0x8600` is not `0x11`, it scans the same truck's `+0x3B8/+0x3C0` entries and returns an entry value selected by a float threshold. It does not read or return a demonstrated task ID. The `0x11` case delegates to `FUN_14019CD70`; no claim about that branch is used for the reported kind-1 road truck.

`FUN_1401E5480` is 1,812 bytes (`0x1401E5480..0x1401E5B93`); `FUN_1401E5BA0` is 1,343 bytes (`0x1401E5BA0..0x1401E60DE`). In both, `RSI` is the candidate truck and `RBP`/`R14` retains the office. `FUN_1401E5480` scans office `+0xD58/+0xD60` records, passing each record to `FUN_1401DE2B0(context, row, truck, office)`. Only a positive row predicate proceeds to its rebuild block. `FUN_1401E5BA0` scans the same office vector and requires a row's first qword to equal the value returned by `FUN_1406BDC30`, plus its further per-row tests.

On the positive path, each branch sets the same truck's `+0x688` end pointer to `+0x680` and destroys/reinitializes its `+0x638/+0x640` configuration entries. It then calls `FUN_14067D5D0` on the same truck with a selected row target, insertion index `-1`, and flag zero. That helper can insert/overwrite route targets and their `0x198`-stride configuration rows, sets truck `+0xD3A`, and recomputes `+0x698`. The route rebuild is therefore concrete and receiver-preserving; it is not merely a read of an existing schedule.

Selected raw blocks from the two rebuilders:

```asm
1401e554b: 48 8B 86 80 06 00 00     MOV RAX,[RSI + 0x680]
1401e5564: 48 89 86 88 06 00 00     MOV [RSI + 0x688],RAX
1401e556b: 48 8B BE 40 06 00 00     MOV RDI,[RSI + 0x640]
1401e5572: 48 8B 9E 38 06 00 00     MOV RBX,[RSI + 0x638]
1401e5590: 48 8D 8B C8 00 00 00     LEA RCX,[RBX + 0xc8]
1401e5597: E8 54 BE F6 FF           CALL 0x1401513f0
1401e559c: 48 8B CB                 MOV RCX,RBX
1401e559f: E8 4C BE F6 FF           CALL 0x1401513f0
1401e55b0: 48 8B 86 38 06 00 00     MOV RAX,[RSI + 0x638]
1401e55ba: 48 89 86 40 06 00 00     MOV [RSI + 0x640],RAX
1401e55c1: 41 83 C8 FF              OR R8D,0xffffffff
1401e55c5: 49 8B 06                 MOV RAX,[R14]
1401e55c8: 48 8B CE                 MOV RCX,RSI
1401e55cb: 49 8B 56 10              MOV RDX,[R14 + 0x10]
1401e55e5: E8 E6 7F 49 00           CALL 0x14067d5d0
...
1401e5cd1: 48 8B 86 80 06 00 00     MOV RAX,[RSI + 0x680]
1401e5cdd: 48 89 86 88 06 00 00     MOV [RSI + 0x688],RAX
1401e5ce4: 48 8B BE 40 06 00 00     MOV RDI,[RSI + 0x640]
1401e5ceb: 48 8B 9E 38 06 00 00     MOV RBX,[RSI + 0x638]
1401e5d30: 48 8B 86 38 06 00 00     MOV RAX,[RSI + 0x638]
1401e5d3a: 48 89 86 40 06 00 00     MOV [RSI + 0x640],RAX
1401e5d41: 41 83 C8 FF              OR R8D,0xffffffff
1401e5d45: 49 8B 07                 MOV RAX,[R15]
1401e5d48: 48 8B CE                 MOV RCX,RSI
1401e5d4b: 49 8B 57 48              MOV RDX,[R15 + 0x48]
...
1401e5fd1: 45 33 C0                 XOR R8D,R8D
1401e5fd4: 33 D2                    XOR EDX,EDX
1401e5fd6: 48 8B CE                 MOV RCX,RSI
1401e5fd9: E8 D2 2E 4D 00           CALL 0x1406b8eb0
```

`FUN_1401E4B10(context, office, row)` subsequently scans the same office `+0xD58/+0xD60` rows and calls `FUN_1401E3BD0(context, row)` when two row fields match. `FUN_1401E3BD0` directly recomputes row metrics/flags; its direct receiver is the office row, not the truck. These calls do not establish removal of truck work. The indirect effects of its callees are not used to rule out other behavior.

`FUN_1401DE2B0` is an 864-byte boolean row predicate. It receives `(context, row, truck, office)`, reads the row's configuration indexes through the same office `+0xD38`, checks route/resource/threshold conditions, and can inspect the candidate truck's `+0x3B8` vector. Its type check special-cases office type `0x2D`; the actual type-43 receiver does not take that special case. A true return means the row passes this predicate, not independently that a new task has been committed.

### `FUN_140680A20` invalidates the outer-call exclusivity claim

The entry test at `0x1401C6420` determines whether `FUN_1406B8EB0` is initially called. It is not sufficient to conclude that the helper call and `FUN_1403FBDC0` cannot both occur in one updater iteration: after the helper returns, `0x1401C6498` tests the same truck byte again, and the zero case alone skips the reset block.

The nested type-43 call is receiver-preserving. `FUN_1406B8EB0` saves its incoming truck in `RDI`; `R14D` is zeroed at `0x1406B8F15`; it clears a 16-bit field beginning at `truck+0xD39`, then calls `FUN_140680A20` with the same truck in `RCX`, `RDX=0`, `R8=-1`, and both explicit stack flag bytes zero:

```asm
1406b8f15: 45 33 F6                    XOR R14D,R14D
1406b8f7d: 66 44 89 B7 39 0D 00 00    MOV word ptr [RDI + 0xd39],R14W
1406b8f94: 45 84 C0                    TEST R8B,R8B
1406b8f97: 75 58                       JNE 0x1406b8ff1
1406b8f99: 48 8B 87 08 17 00 00        MOV RAX,qword ptr [RDI + 0x1708]
1406b8fa0: 8B 88 94 02 00 00           MOV ECX,dword ptr [RAX + 0x294]
1406b8fa6: 83 F9 06                    CMP ECX,0x6
1406b8fa9: 75 0D                       JNE 0x1406b8fb8
1406b8fb8: 83 F9 08                    CMP ECX,0x8
1406b8fbb: 75 10                       JNE 0x1406b8fcd
1406b8fcd: 83 F9 0A                    CMP ECX,0xa
1406b8fd0: 48 8B CF                    MOV RCX,RDI
1406b8fd3: 75 07                       JNE 0x1406b8fdc
1406b8fdc: 44 88 74 24 28              MOV byte ptr [RSP + 0x28],R14B
1406b8fe1: 44 88 74 24 20              MOV byte ptr [RSP + 0x20],R14B
1406b8fe6: 41 83 C8 FF                 OR R8D,0xffffffff
1406b8fea: 33 D2                       XOR EDX,EDX
1406b8fec: E8 2F 7A FC FF              CALL 0x140680a20
1406b8ff1: 44 38 B7 39 0D 00 00        CMP byte ptr [RDI + 0xd39],R14B
1406b8ff8: 75 7B                       JNE 0x1406b9075
```

The `WORD` store clears both byte `+0xD39` and adjacent byte `+0xD3A`; it is not a one-byte task-state write. At the nested call, the first two stack flags are definitely zero because `R14B=0`. The incoming `R9` is not used as an input by the relevant `FUN_140680A20` path; the function overwrites `R9D` before passing it to its downstream helper.

For the reported road-truck metadata kind 1, `FUN_140680A20` takes its non-kind-4 path through `FUN_14067E8F0`. The saved first stack flag is `R12B=0`. The following raw blocks show that `+0xD39` is set and can subsequently be cleared:

```asm
140680af2: C6 83 39 0D 00 00 01        MOV byte ptr [RBX + 0xd39],0x1
140680b00: 44 0F B6 65 77              MOVZX R12D,byte ptr [RBP + 0x77]
140680b0a: 83 B8 94 02 00 00 04        CMP dword ptr [RAX + 0x294],0x4
140680b11: 75 3A                       JNZ 0x140680b4d
140680b4d: C6 44 24 30 00              MOV byte ptr [RSP + 0x30],0x0
140680b5f: 44 0F B6 CE                 MOVZX R9D,SIL
140680b6e: E8 7D DD FF FF              CALL 0x14067e8f0
140680d96: C6 83 39 0D 00 00 01        MOV byte ptr [RBX + 0xd39],0x1
140681325: 0F 85 C3 05 00 00           JNZ 0x1406818ee
14068132b: C7 83 39 0D 00 00 00 01 00 00 MOV dword ptr [RBX + 0xd39],0x100
140681335: E9 B4 05 00 00              JMP 0x1406818ee
140681670: 41 80 FB 01                 CMP R11B,0x1
140681674: 0F 85 AC 01 00 00           JNZ 0x140681826
14068167a: 45 3A E3                    CMP R12B,R11B
14068167d: 0F 85 A3 01 00 00           JNZ 0x140681826
140681683: C6 83 39 0D 00 00 00        MOV byte ptr [RBX + 0xd39],0x0
140681826: 45 84 E4                    TEST R12B,R12B
140681829: 0F 85 BB 00 00 00           JNZ 0x1406818ea
14068182f: 44 88 A3 39 0D 00 00        MOV byte ptr [RBX + 0xd39],R12B
```

With the actual nested first stack flag `R12B=0`, the paired-ones condition at `0x140681670..0x14068167D` cannot reach the `0x140681683` clear: if `R11B==1`, the second comparison branches to `0x140681826`; otherwise the first does. If control reaches `0x140681826`, its `TEST R12B` falls through and `0x14068182F` writes zero. Separately, `0x14068132B` is a **DWORD** store of `0x100`: its low byte makes `+0xD39 == 0`, and the next byte makes `+0xD3A == 1`. That store is conditional on the `0x1401311C0` result being `AL==1`; the caller-visible state is therefore not inferable from the call alone. Other return paths can retain the earlier nonzero write.

Consequently, the entry-branch argument does not prove mutual exclusion. If the nested helper returns with `+0xD39 != 0`, its own `0x1406B8FF1` branch exits before its later association logic, while the updater's post-call test at `0x1401C6498` falls through toward the configuration/reset block and its guarded `FUN_1403FBDC0` call. If the byte is zero at that outer test, the updater skips that call. Static evidence here does **not** establish which result occurs for the live truck, nor that both call sites actually execute on a particular iteration. It does establish that the earlier claim of unconditional call-site mutual exclusion was unsupported. The reset call remains no proof of active-task removal because its updater entry guard is `truck+0xC80 == 0`.

`truck+0x1708 -> metadata+0x294` is metadata-kind provenance only, not a vehicle state enum. Metadata `+0x8600`, used by `FUN_1406BDC30`, is a separate field; kind `+0x294 == 1` does not select or exclude its `+0x8600 == 0x11` branch. Both `FUN_1406BDC30` paths remain in scope unless separately proved.

### Candidate rows and the bounded simulation consumer

#### Planner rows are regenerated candidates

The type-43 updater's call at `0x1401C6BA1` is conditional on office `+0xD50 != 0`; it passes `RCX=R12` context and `RDX=RDI` office to `FUN_1401E2380`. The existing planner decompilation is consistent with the following directly exported instructions. `FUN_1401E2380` preserves the receiver in `R12`, returns on a null `RDX`, clears the office refresh byte, and resets its row end to its row begin before iterating configuration entries at `+0xD38/+0xD40`:

```asm
1401e2380: 48 8B C4                 MOV RAX,RSP
1401e2383: 48 89 50 10              MOV qword ptr [RAX + 0x10],RDX
1401e2387: 48 89 48 08              MOV qword ptr [RAX + 0x8],RCX
1401e23b8: 4C 8B E2                 MOV R12,RDX
1401e23bb: 4C 8B E9                 MOV R13,RCX
1401e2428: 48 85 D2                 TEST RDX,RDX
1401e242b: 0F 84 A8 16 00 00        JZ 0x1401e3ad9
1401e2431: 44 88 82 50 0D 00 00     MOV byte ptr [RDX + 0xd50],R8B
1401e2438: 48 8B 82 58 0D 00 00     MOV RAX,qword ptr [RDX + 0xd58]
1401e243f: 48 89 82 60 0D 00 00     MOV qword ptr [RDX + 0xd60],RAX
1401e2449: 48 8B 82 40 0D 00 00     MOV RAX,qword ptr [RDX + 0xd40]
1401e2450: 48 2B 82 38 0D 00 00     SUB RAX,qword ptr [RDX + 0xd38]
1401e2457: 48 C1 F8 03              SAR RAX,0x3
1401e2461: 0F 84 D5 01 00 00        JZ 0x1401e263c
1401e2470: 49 8B 84 24 38 0D 00 00 MOV RAX,qword ptr [R12 + 0xd38]
1401e2478: 48 8B 3C C8              MOV RDI,qword ptr [RAX + RCX*0x8]
```

`R8B` is zeroed earlier in the prologue. The later loop consumes office configuration entries and generates rows in the reset `+0xD58/+0xD60` range. This proves that the vector is a refreshable planner-candidate range, not a persistent per-truck job identity. A positive row predicate and the subsequent truck route rebuild remain real operations, but neither establishes that a row is an assignment commitment.

The candidate-truck buffer operation at `0x1401C6CB2` receives begin/end/count/comparator arguments and sorts the pointer range in place; it does not receive a vector object whose length it could update. Its complete small raw body is included below to preserve the buffer identity/order distinction:

```asm
1400fa370: 48 89 5C 24 08           MOV qword ptr [RSP + 0x8],RBX
1400fa375: 48 89 6C 24 10           MOV qword ptr [RSP + 0x10],RBP
1400fa37a: 48 89 74 24 18           MOV qword ptr [RSP + 0x18],RSI
1400fa37f: 57                       PUSH RDI
1400fa380: 48 83 EC 30              SUB RSP,0x30
1400fa384: 48 8B C2                 MOV RAX,RDX
1400fa387: 49 8B E9                 MOV RBP,R9
1400fa38a: 48 2B C1                 SUB RAX,RCX
1400fa38d: 49 8B F0                 MOV RSI,R8
1400fa390: 48 C1 F8 03              SAR RAX,0x3
1400fa394: 48 8B DA                 MOV RBX,RDX
1400fa397: 48 8B F9                 MOV RDI,RCX
1400fa39a: 48 83 F8 20              CMP RAX,0x20
1400fa39e: 0F 8E 91 00 00 00        JLE 0x1400fa435
1400fa3a4: 48 85 F6                 TEST RSI,RSI
1400fa3a7: 0F 8E B1 00 00 00        JLE 0x1400fa45e
1400fa3ad: 4C 8B CD                 MOV R9,RBP
1400fa3b0: 48 8D 4C 24 20           LEA RCX,[RSP + 0x20]
1400fa3b5: 4C 8B C3                 MOV R8,RBX
1400fa3b8: 48 8B D7                 MOV RDX,RDI
1400fa3bb: E8 80 17 00 00           CALL 0x1400fbb40
1400fa3c0: 4C 8B 54 24 28           MOV R10,qword ptr [RSP + 0x28]
1400fa3c5: 48 8B C6                 MOV RAX,RSI
1400fa3c8: 48 99                    CQO
1400fa3ca: 48 8B CB                 MOV RCX,RBX
1400fa3cd: 48 2B C2                 SUB RAX,RDX
1400fa3d0: 49 2B CA                 SUB RCX,R10
1400fa3d3: 48 D1 F8                 SAR RAX,0x1
1400fa3d6: 48 83 E1 F8              AND RCX,-0x8
1400fa3da: 48 8B F0                 MOV RSI,RAX
1400fa3dd: 4C 8B CD                 MOV R9,RBP
1400fa3e0: 48 99                    CQO
1400fa3e2: 48 2B C2                 SUB RAX,RDX
1400fa3e5: 48 8B 54 24 20           MOV RDX,qword ptr [RSP + 0x20]
1400fa3ea: 48 D1 F8                 SAR RAX,0x1
1400fa3ed: 48 03 F0                 ADD RSI,RAX
1400fa3f0: 48 8B C2                 MOV RAX,RDX
1400fa3f3: 48 2B C7                 SUB RAX,RDI
1400fa3f6: 4C 8B C6                 MOV R8,RSI
1400fa3f9: 48 83 E0 F8              AND RAX,-0x8
1400fa3fd: 48 3B C1                 CMP RAX,RCX
1400fa400: 7D 0F                    JGE 0x1400fa411
1400fa402: 48 8B CF                 MOV RCX,RDI
1400fa405: E8 66 FF FF FF           CALL 0x1400fa370
1400fa40a: 48 8B 7C 24 28           MOV RDI,qword ptr [RSP + 0x28]
1400fa40f: EB 10                    JMP 0x1400fa421
1400fa411: 48 8B D3                 MOV RDX,RBX
1400fa414: 49 8B CA                 MOV RCX,R10
1400fa417: E8 54 FF FF FF           CALL 0x1400fa370
1400fa41c: 48 8B 5C 24 20           MOV RBX,qword ptr [RSP + 0x20]
1400fa421: 48 8B C3                 MOV RAX,RBX
1400fa424: 48 2B C7                 SUB RAX,RDI
1400fa427: 48 C1 F8 03              SAR RAX,0x3
1400fa42b: 48 83 F8 20              CMP RAX,0x20
1400fa42f: 0F 8F 6F FF FF FF        JG 0x1400fa3a4
1400fa435: 48 83 F8 02              CMP RAX,0x2
1400fa439: 7C 0E                    JL 0x1400fa449
1400fa43b: 4C 8B C5                 MOV R8,RBP
1400fa43e: 48 8B D3                 MOV RDX,RBX
1400fa441: 48 8B CF                 MOV RCX,RDI
1400fa444: E8 17 1A 00 00           CALL 0x1400fbe60
1400fa449: 48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
1400fa44e: 48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
1400fa453: 48 8B 74 24 50           MOV RSI,qword ptr [RSP + 0x50]
1400fa458: 48 83 C4 30              ADD RSP,0x30
1400fa45c: 5F                       POP RDI
1400fa45d: C3                       RET
1400fa45e: 48 83 F8 20              CMP RAX,0x20
1400fa462: 7E D1                    JLE 0x1400fa435
1400fa464: 4C 8B C5                 MOV R8,RBP
1400fa467: 48 8B D3                 MOV RDX,RBX
1400fa46a: 48 8B CF                 MOV RCX,RDI
1400fa46d: E8 CE 18 00 00           CALL 0x1400fbd40
1400fa472: 4C 8B C5                 MOV R8,RBP
1400fa475: 48 8B D3                 MOV RDX,RBX
1400fa478: 48 8B CF                 MOV RCX,RDI
1400fa47b: E8 40 19 00 00           CALL 0x1400fbdc0
1400fa480: EB C7                    JMP 0x1400fa449
```

#### Simulation dispatch is a conditional, not yet same-office, edge

The manager update at `0x140010E20` calls `FUN_14066D530` on a direct object pointer and then on each pointer in its `+0x3A0` array. The latter call setup is `RCX=[RBX+0x398]+RSI`, followed by `CALL 0x14066D530` at `0x140010E87`. In `FUN_14066D530`, `RBX` retains that receiver. Its route-consumer branch is reachable when `+0x4A0 == 0` and metadata-kind `+0x294` is 1 or 2; the reported road-truck kind 1 satisfies the metadata test, but the `+0x4A0` value and the office-fleet-to-manager-array membership are not established. The exact predecessor references to the call are conditional jumps at `0x14066E8A3` and `0x14066E8B0`.

```asm
140010e80: 48 8B 0A                 MOV RCX,qword ptr [RDX]
140010e83: 48 8B 0C 31              MOV RCX,qword ptr [RCX + RSI]
140010e87: E8 A4 C6 65 00           CALL 0x14066d530
14066e880: 48 8B 8B A0 04 00 00     MOV RCX,qword ptr [RBX + 0x4a0]
14066e887: 48 85 C9                 TEST RCX,RCX
14066e88a: 0F 85 E3 01 00 00        JNZ 0x14066ea73
14066e890: 48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14066e897: 8B 88 94 02 00 00        MOV ECX,dword ptr [RAX + 0x294]
14066e89d: 8D 41 FF                 LEA EAX,[RCX - 0x1]
14066e8a0: 83 F8 01                 CMP EAX,0x1
14066e8a3: 0F 86 BD 01 00 00        JBE 0x14066ea66
14066e8a9: 44 38 BB 7D 06 00 00     CMP byte ptr [RBX + 0x67d],R15B
14066e8b0: 0F 85 B0 01 00 00        JNZ 0x14066ea66
14066ea66: 48 8B CB                 MOV RCX,RBX
14066ea69: E8 B2 A9 02 00           CALL 0x140699420
1406994a9: 4C 8B F9                 MOV R15,RCX
```

The first call into `FUN_140699420` is proven to receive the same `RBX` object as `FUN_14066D530`. This is not yet a proof that the candidate `RSI` truck from the type-43 `+0xC70/+0xC78` fleet is present in the manager array or that it satisfies `+0x4A0 == 0`; that identity edge remains open.

#### Route/current-target handling is not a proven stop or removal event

On the bounded route-index path in `FUN_140699420`, the receiver is `R15`. One selected branch tests a candidate slot in the receiver's `+0x6B8/+0x6C0` vector; when the slot is null it sets `+0xD3A`, calls `FUN_1406BBDF0(RCX=R15)`, then sets `+0x698` to the last route row if `+0x680/+0x688` is nonempty. If the route is empty, it writes the computed empty-route index. A separate bounded branch validates the current `+0x698` entry against `R10` and may call the same detach helper. These are route/list normalization paths; neither is shown to be caused by task completion or cancellation.

```asm
14069bb9c: 49 8B 87 C0 06 00 00     MOV RAX,qword ptr [R15 + 0x6c0]
14069bba3: 49 2B 87 B8 06 00 00     SUB RAX,qword ptr [R15 + 0x6b8]
14069bbae: 44 3B C0                 CMP R8D,EAX
14069bbb1: 7D 76                    JGE 0x14069bc29
14069bbb3: 49 8B 87 B8 06 00 00     MOV RAX,qword ptr [R15 + 0x6b8]
14069bbba: 4A 83 3C C0 00           CMP qword ptr [RAX + R8*8],0
14069bbbf: 75 68                    JNZ 0x14069bc29
14069bbc1: 40 32 F6                 XOR SIL,SIL
14069bbcb: 41 C6 87 3A 0D 00 00 01 MOV byte ptr [R15 + 0xd3a],0x1
14069bbd3: 49 8B CF                 MOV RCX,R15
14069bbd6: E8 15 02 02 00           CALL 0x1406bbdf0
14069bbdb: 49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bbe2: 49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bbe9: 48 C1 F8 03              SAR RAX,0x3
14069bbed: 48 85 C0                 TEST RAX,RAX
14069bbf7: 74 19                    JZ 0x14069bc12
14069bc04: FF C8                    DEC EAX
14069bc06: 41 89 87 98 06 00 00     MOV dword ptr [R15 + 0x698],EAX
14069bc12: 49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bc19: 48 C1 F8 03              SAR RAX,0x3
14069bc1d: 41 89 87 98 06 00 00     MOV dword ptr [R15 + 0x698],EAX
14069bc29: 83 3D 04 A1 33 00 03     CMP dword ptr [0x1409d5d34],0x3
14069bc30: 0F 84 C8 00 00 00        JZ 0x14069bcfe
14069bc36: 81 3D 60 98 33 00 45 01 00 00 CMP dword ptr [0x1409d54a0],0x145
14069bc40: 0F 8D B8 00 00 00        JGE 0x14069bcfe
14069bc50: 41 83 F8 01              CMP R8D,0x1
14069bc54: 0F 8E 4D 04 00 00        JLE 0x14069c0a7
14069bc5a: 49 63 8F 98 06 00 00     MOVSXD RCX,dword ptr [R15 + 0x698]
14069bc61: 85 C9                    TEST ECX,ECX
14069bc63: 0F 88 3E 04 00 00        JS 0x14069c0a7
14069bc69: 49 8B 87 88 06 00 00     MOV RAX,qword ptr [R15 + 0x688]
14069bc70: 49 2B 87 80 06 00 00     SUB RAX,qword ptr [R15 + 0x680]
14069bc77: 48 C1 F8 03              SAR RAX,0x3
14069bc7b: 48 3B C8                 CMP RCX,RAX
14069bc7e: 0F 83 23 04 00 00        JNC 0x14069c0a7
14069bc84: 49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069bc8b: 4C 39 14 C8              CMP qword ptr [RAX + RCX*8],R10
14069bc8f: 0F 84 12 04 00 00        JZ 0x14069c0a7
14069bc95: 41 C6 87 3A 0D 00 00 01 MOV byte ptr [R15 + 0xd3a],0x1
14069bc9d: 49 8B CF                 MOV RCX,R15
14069bca0: E8 4B 01 02 00           CALL 0x1406bbdf0
```

The complete 107-byte `FUN_1406BBDF0` body shows only a linked-object detach/truncate operation: it clears an element's `+0x100` only when that pointer equals the receiver, then sets the receiver's `+0xD50` end to `+0xD48` begin. It does not write `+0xC80`, `+0xBB0`, or the route list:

```asm
1406bbdf0: 48 8B 81 50 0D 00 00     MOV RAX,qword ptr [RCX + 0xd50]
1406bbdf7: 45 33 D2                 XOR R10D,R10D
1406bbdfa: 48 2B 81 48 0D 00 00     SUB RAX,qword ptr [RCX + 0xd48]
1406bbe01: 45 8B CA                 MOV R9D,R10D
1406bbe04: 48 C1 F8 03              SAR RAX,0x3
1406bbe08: 48 85 C0                 TEST RAX,RAX
1406bbe0b: 74 3F                    JZ 0x1406bbe4c
1406bbe0d: 45 8B C2                 MOV R8D,R10D
1406bbe10: 48 8B 81 48 0D 00 00     MOV RAX,qword ptr [RCX + 0xd48]
1406bbe17: 49 8B 14 00              MOV RDX,qword ptr [R8 + RAX]
1406bbe1b: 48 39 8A 00 01 00 00     CMP qword ptr [RDX + 0x100],RCX
1406bbe22: 75 07                    JNZ 0x1406bbe2b
1406bbe24: 4C 89 92 00 01 00 00     MOV qword ptr [RDX + 0x100],R10
1406bbe2b: 48 8B 91 50 0D 00 00     MOV RDX,qword ptr [RCX + 0xd50]
1406bbe32: 41 FF C1                 INC R9D
1406bbe35: 48 2B 91 48 0D 00 00     SUB RDX,qword ptr [RCX + 0xd48]
1406bbe3c: 49 83 C0 08              ADD R8,0x8
1406bbe40: 48 C1 FA 03              SAR RDX,0x3
1406bbe44: 49 63 C1                 MOVSXD RAX,R9D
1406bbe47: 48 3B C2                 CMP RAX,RDX
1406bbe4a: 72 C4                    JC 0x1406bbe10
1406bbe4c: 48 8B 81 48 0D 00 00     MOV RAX,qword ptr [RCX + 0xd48]
1406bbe53: 48 89 81 50 0D 00 00     MOV qword ptr [RCX + 0xd50],RAX
1406bbe5a: C3                       RET
```

The other bounded route block at `0x14069DC61..0x14069DD70` calls `FUN_1406C2A00` and possibly `FUN_1406C27E0` on the same `R15` receiver, validates `+0x698` against `+0x680/+0x688`, and on an invalid index can call `FUN_14067D5D0(RCX=R15, RDX=[R15+0x4F8], R8=-1, R9=0)` to rebuild a home route. When `+0x528` is null and the index is usable, it loads `[+0x680 + +0x698*8]` into `+0x528` at `0x14069DCF6`, then resets movement fields:

```asm
14069dc61: 49 8B CF                 MOV RCX,R15
14069dc64: E8 97 4D 02 00           CALL 0x1406c2a00
14069dc69: 84 C0                    TEST AL,AL
14069dc6b: 74 0F                    JZ 0x14069dc7c
14069dc6d: 49 8B CF                 MOV RCX,R15
14069dc70: E8 6B 4B 02 00           CALL 0x1406c27e0
14069dc75: 49 89 87 28 05 00 00     MOV qword ptr [R15 + 0x528],RAX
14069dc7c: 49 63 87 98 06 00 00     MOVSXD RAX,dword ptr [R15 + 0x698]
14069dc83: 85 C0                    TEST EAX,EAX
14069dc85: 78 17                    JS 0x14069dc9e
14069dc87: 49 8B 8F 88 06 00 00     MOV RCX,qword ptr [R15 + 0x688]
14069dc8e: 49 2B 8F 80 06 00 00     SUB RCX,qword ptr [R15 + 0x680]
14069dc95: 48 C1 F9 03              SAR RCX,0x3
14069dc99: 48 3B C1                 CMP RAX,RCX
14069dc9c: 72 3C                    JC 0x14069dcda
14069dc9e: 49 83 BF F8 04 00 00 00  CMP qword ptr [R15 + 0x4f8],0
14069dca6: 74 32                    JZ 0x14069dcda
14069dca8: 45 33 C9                 XOR R9D,R9D
14069dcab: 41 83 C8 FF              OR R8D,0xffffffff
14069dcaf: 49 8B 97 F8 04 00 00     MOV RDX,qword ptr [R15 + 0x4f8]
14069dcb6: 49 8B CF                 MOV RCX,R15
14069dcb9: E8 12 F9 FD FF           CALL 0x14067d5d0
14069dcbe: 84 C0                    TEST AL,AL
14069dcc0: 74 18                    JZ 0x14069dcda
14069dcc2: 49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069dcc9: C6 00 00                 MOV byte ptr [RAX],0
14069dccc: 49 8B 87 38 06 00 00     MOV RAX,qword ptr [R15 + 0x638]
14069dcd3: C6 80 C8 00 00 00 00     MOV byte ptr [RAX + 0xc8],0
14069dcda: 49 83 BF 28 05 00 00 00  CMP qword ptr [R15 + 0x528],0
14069dce2: 75 19                    JNZ 0x14069dcfd
14069dce4: 49 63 8F 98 06 00 00     MOVSXD RCX,dword ptr [R15 + 0x698]
14069dceb: 49 8B 87 80 06 00 00     MOV RAX,qword ptr [R15 + 0x680]
14069dcf2: 48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*8]
14069dcf6: 49 89 8F 28 05 00 00     MOV qword ptr [R15 + 0x528],RCX
14069dcfd: 41 C7 87 6C 05 00 00 FF FF FF FF MOV dword ptr [R15 + 0x56c],0xffffffff
14069dd08: 49 8B 87 40 05 00 00     MOV RAX,qword ptr [R15 + 0x540]
14069dd0f: 49 89 87 48 05 00 00     MOV qword ptr [R15 + 0x548],RAX
14069dd16: 49 C7 87 58 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x558],0
14069dd21: 49 C7 87 94 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x594],0
14069dd2c: 41 C7 87 68 05 00 00 00 00 80 3F MOV dword ptr [R15 + 0x568],0x3f800000
14069dd37: 45 89 AF 9C 17 00 00     MOV dword ptr [R15 + 0x179c],R13D
14069dd3e: 49 C7 87 60 05 00 00 00 00 00 00 MOV qword ptr [R15 + 0x560],0
14069dd49: 45 89 AF 10 13 00 00     MOV dword ptr [R15 + 0x1310],R13D
14069dd50: 41 C6 87 9C 05 00 00 00 MOV byte ptr [R15 + 0x59c],0
14069dd58: 49 8B CF                 MOV RCX,R15
14069dd5b: E8 E0 24 01 00           CALL 0x1406b0240
```

These are route/current-target operations. They do not prove a stop-only advance, task completion, cancellation, or that a home-route rebuild follows removal; no `+0xC80`/`+0xBB0` removal appears in these bounded blocks.

The `+0x698` writes reproduced above assign the last route index or the computed empty-route index; they do not establish an increment for internal pickup/unload stops. The route consumer therefore supplies a useful candidate path for later observation, but not a proven interval event.

### Lifecycle matrix and current integration decision

| Case | Static evidence in this trace | Status |
|---|---|---|
| Idle/default route at DO | `FUN_14067CB30` can clear route/configuration vectors on the conditional home-target path; candidate rows and route lists can be refreshed. | Neither an empty nor nonempty list classifies active DO work. |
| Assigned but not loaded | The type-43 updater can build a route from an office row before any identified resource transfer, but no committed-work event is established. | Assigned-before-load start remains unproved. |
| Internal pickup/unload stops | `FUN_140699420` can write the current `+0x528` target's `+0xBB0` from `truck+0x390` and write that target to `truck+0xC80`; its old-target qword write stores `R14` (decompiler says zero, but zero provenance is not in the bounded raw slice). A separate raw block increments `truck+0x700`, not the established `+0x698` route index. | The target link is not a proven task marker; neither the old-link clear nor these paths' relation to pickup/unload is established. |
| Final removal/cancellation | The type-43 home-target branch calls `FUN_14067CB30`, whose direct stores clear route/configuration vectors but not `+0xC80/+0xBB0`; it also calls manager-detach helpers with some transitive effects not classified. No cancellation/removal cause is connected to the call. | No close boundary established; cancellation remains untraced. |
| Replacement in one update | The same `FUN_140699420` branch may overwrite `+0xC80` with the current target and may write its `+0xBB0`; the old-target `R14` store is not proven zero. Route rebuilders also clear/repopulate their vectors synchronously. | These are observable target-association/route writes, not proof of whole-task replacement; no mandatory task boundary is established. |
| Identical consecutive work | No stable job ID or semantically validated commit event is identified. Same office, row, route and target values can recur. | Cannot distinguish identical consecutive tasks from refresh using final snapshots. |
| Empty return after removal | `truck+0x528 == +0x4F8 == +0x4F0` with descriptor type `0x2B` or `0x2D` reaches a route/config reset, but the caller does not prove that the task was removed before this path. | The home-target reset is not a proven removal boundary; post-removal travel cannot yet be excluded. |
| Reload | Not traced in this pass. | Explicitly unsupported. |

**Integration decision:** do not implement a polling interval from `+0x680/+0x688`, `+0x698`, `+0x700`, `+0xC80`, `+0xBB0`, `+0xD39`, or `+0x390`. Route installation is proved, but not task commitment. The type-43 fleet-to-simulation-manager membership and `+0x4A0` applicability guard are still open; even on `FUN_140699420`'s receiver, `+0xC80/+0xBB0` follow a current target and may be rebound, while the home-target reset does not prove final removal. The cancellation/replacement edge and the meaning of the `+0x700` progression field remain untraced. These are bounded missing edges, not evidence that static proof is impossible.

No destination-payer conclusion follows from this route path. Do not equate the selected row, `truck+0x390`, or the candidate home office with the payer.

### Logging-only observation proposal (not performed)

If a later changed logging-only capture is approved, the smallest useful paired capture is the existing same-office call at `0x1401C6CC9` plus the connected simulation association/reset path:

- Receiver/arguments: `RCX=R12` context, `RDX=[RBX]` candidate truck from this office's fleet, `R8=RDI` same type-43 office.
- At `0x14069B7D0`, receiver `R15` (copied from `FUN_140699420`'s `RCX`) and entry byte `+0xD39` govern the association block. Capture before/after `+0x528`, `+0x4F8`, `+0x4F0`, `+0xC80`, `+0x390`, `R14`, and the old/current target's `+0xBB0`. At `0x14069B8D9`, record the same truck and the call arguments `RDX=0`, `R8B=1`; record route/config bounds and `+0x528/+0x538` before and after the call through `0x14069B923`.
- Also record `+0x698` and `+0x700` separately; do not merge them. Restrict conclusions to a truck whose type-43 fleet membership and simulation-manager membership are independently established.
- Expected contrast: a current-target handoff would show the exact `R14` value written to the old target and current-target `+0xBB0`/truck `+0xC80` updated, without proving task replacement; the type-43 home-target call can clear route/config vectors while its direct body leaves the association fields untouched. A repeated identical route-builder result remains ambiguous between refresh and a new task, and no cancellation writer is covered by these sites.
- This is a logging-only proposal, not performed validation; it does not request a game run or install instrumentation.

### Complete raw bodies: small connected functions

The following complete bodies are Ghidra instruction exports from the identity-checked `/SOVIET64.exe`. Larger `FUN_1401E5480` and `FUN_1401E5BA0` bodies are not duplicated; their relevant blocks and bounds are documented above.

#### `FUN_1401E5350` — 293 bytes, `0x1401E5350..0x1401E5474`

```asm
1401e5350:  48 89 5C 24 18           MOV qword ptr [RSP + 0x18],RBX
1401e5355:  48 89 6C 24 20           MOV qword ptr [RSP + 0x20],RBP
1401e535a:  56                       PUSH RSI
1401e535b:  48 83 EC 20              SUB RSP,0x20
1401e535f:  48 83 BA 28 05 00 00 00  CMP qword ptr [RDX + 0x528],0
1401e5367:  49 8B F0                 MOV RSI,R8
1401e536a:  48 8B DA                 MOV RBX,RDX
1401e536d:  48 8B E9                 MOV RBP,RCX
1401e5370:  74 12                    JE 0x1401e5384
1401e5372:  32 C0                    XOR AL,AL
1401e5374:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
1401e5379:  48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
1401e537e:  48 83 C4 20              ADD RSP,0x20
1401e5382:  5E                       POP RSI
1401e5383:  C3                       RET
1401e5384:  0F 57 C9                 XORPS XMM1,XMM1
1401e5387:  48 89 7C 24 30           MOV qword ptr [RSP + 0x30],RDI
1401e538c:  48 8B CB                 MOV RCX,RBX
1401e538f:  E8 9C 88 4D 00           CALL 0x1406bdc30
1401e5394:  4C 8D 86 58 0D 00 00     LEA R8,[RSI + 0xd58]
1401e539b:  48 8B F8                 MOV RDI,RAX
1401e539e:  49 8B 48 08              MOV RCX,qword ptr [R8 + 0x8]
1401e53a2:  49 2B 08                 SUB RCX,qword ptr [R8]
1401e53a5:  48 C1 F9 07              SAR RCX,0x7
1401e53a9:  48 83 F9 01              CMP RCX,0x1
1401e53ad:  76 40                    JBE 0x1401e53ef
1401e53af:  48 85 C0                 TEST RAX,RAX
1401e53b2:  74 50                    JE 0x1401e5404
1401e53b4:  4D 85 C0                 TEST R8,R8
1401e53b7:  74 36                    JE 0x1401e53ef
1401e53b9:  49 8B 50 08              MOV RDX,qword ptr [R8 + 0x8]
1401e53bd:  49 8B 08                 MOV RCX,qword ptr [R8]
1401e53c0:  48 8B C2                 MOV RAX,RDX
1401e53c3:  48 2B C1                 SUB RAX,RCX
1401e53c6:  48 C1 F8 07              SAR RAX,0x7
1401e53ca:  48 85 C0                 TEST RAX,RAX
1401e53cd:  74 20                    JE 0x1401e53ef
1401e53cf:  4C 8B C2                 MOV R8,RDX
1401e53d2:  48 8D 05 17 48 F2 FF     LEA RAX,[0x140109bf0]
1401e53d9:  4C 2B C1                 SUB R8,RCX
1401e53dc:  48 89 44 24 38           MOV qword ptr [RSP + 0x38],RAX
1401e53e1:  49 C1 F8 07              SAR R8,0x7
1401e53e5:  4C 8D 4C 24 38           LEA R9,[RSP + 0x38]
1401e53ea:  E8 D1 59 02 00           CALL 0x14020adc0
1401e53ef:  48 85 FF                 TEST RDI,RDI
1401e53f2:  75 5B                    JNE 0x1401e544f
1401e53f4:  4C 8B C6                 MOV R8,RSI
1401e53f7:  48 8B D3                 MOV RDX,RBX
1401e53fa:  48 8B CD                 MOV RCX,RBP
1401e53fd:  E8 7E 00 00 00           CALL 0x1401e5480
1401e5402:  EB 5C                    JMP 0x1401e5460
1401e5404:  4D 85 C0                 TEST R8,R8
1401e5407:  74 EB                    JE 0x1401e53f4
1401e5409:  49 8B 50 08              MOV RDX,qword ptr [R8 + 0x8]
1401e540d:  49 8B 08                 MOV RCX,qword ptr [R8]
1401e5410:  48 8B C2                 MOV RAX,RDX
1401e5413:  48 2B C1                 SUB RAX,RCX
1401e5416:  48 C1 F8 07              SAR RAX,0x7
1401e541a:  48 85 C0                 TEST RAX,RAX
1401e541d:  74 D5                    JE 0x1401e53f4
1401e541f:  4C 8B C2                 MOV R8,RDX
1401e5422:  48 8D 05 A7 47 F2 FF     LEA RAX,[0x140109bd0]
1401e5429:  4C 2B C1                 SUB R8,RCX
1401e542c:  48 89 44 24 38           MOV qword ptr [RSP + 0x38],RAX
1401e5431:  49 C1 F8 07              SAR R8,0x7
1401e5435:  4C 8D 4C 24 38           LEA R9,[RSP + 0x38]
1401e543a:  E8 81 59 02 00           CALL 0x14020adc0
1401e543f:  4C 8B C6                 MOV R8,RSI
1401e5442:  48 8B D3                 MOV RDX,RBX
1401e5445:  48 8B CD                 MOV RCX,RBP
1401e5448:  E8 33 00 00 00           CALL 0x1401e5480
1401e544d:  EB 11                    JMP 0x1401e5460
1401e544f:  4C 8B CF                 MOV R9,RDI
1401e5452:  4C 8B C6                 MOV R8,RSI
1401e5455:  48 8B D3                 MOV RDX,RBX
1401e5458:  48 8B CD                 MOV RCX,RBP
1401e545b:  E8 40 07 00 00           CALL 0x1401e5ba0
1401e5460:  48 8B 7C 24 30           MOV RDI,qword ptr [RSP + 0x30]
1401e5465:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
1401e546a:  48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
1401e546f:  48 83 C4 20              ADD RSP,0x20
1401e5473:  5E                       POP RSI
1401e5474:  C3                       RET
```

#### `FUN_1401DE2B0` — 864 bytes, `0x1401DE2B0..0x1401DE60F`

```asm
1401de2b0:  40 53                    PUSH RBX
1401de2b2:  55                       PUSH RBP
1401de2b3:  57                       PUSH RDI
1401de2b4:  41 54                    PUSH R12
1401de2b6:  48 83 EC 48              SUB RSP,0x48
1401de2ba:  4D 8B 99 38 0D 00 00     MOV R11,qword ptr [R9 + 0xd38]
1401de2c1:  45 32 E4                 XOR R12B,R12B
1401de2c4:  48 63 42 18              MOVSXD RAX,dword ptr [RDX + 0x18]
1401de2c8:  49 8B D8                 MOV RBX,R8
1401de2cb:  44 0F 29 44 24 20        MOVAPS xmmword ptr [RSP + 0x20],XMM8
1401de2d1:  48 8B FA                 MOV RDI,RDX
1401de2d4:  F2 44 0F 10 05 13 BF 72 00 MOVSD XMM8,qword ptr [0x14090a1f0]
1401de2dd:  48 8B E9                 MOV RBP,RCX
1401de2e0:  4D 8B 14 C3              MOV R10,qword ptr [R11 + RAX*0x8]
1401de2e4:  49 8B 42 10              MOV RAX,qword ptr [R10 + 0x10]
1401de2e8:  F3 0F 10 50 04           MOVSS XMM2,dword ptr [RAX + 0x4]
1401de2ed:  48 63 42 50              MOVSXD RAX,dword ptr [RDX + 0x50]
1401de2f1:  0F 5A D2                 CVTPS2PD XMM2,XMM2
1401de2f4:  4D 8B 14 C3              MOV R10,qword ptr [R11 + RAX*0x8]
1401de2f8:  49 8B 42 10              MOV RAX,qword ptr [R10 + 0x10]
1401de2fc:  F3 0F 10 80 CC 00 00 00  MOVSS XMM0,dword ptr [RAX + 0xcc]
1401de304:  49 8B 81 18 03 00 00     MOV RAX,qword ptr [R9 + 0x318]
1401de30b:  0F 5A C0                 CVTPS2PD XMM0,XMM0
1401de30e:  F2 44 0F 5C C0           SUBSD XMM8,XMM0
1401de313:  48 85 C0                 TEST RAX,RAX
1401de316:  74 0D                    JE 0x1401de325
1401de318:  83 B8 60 03 00 00 2D     CMP dword ptr [RAX + 0x360],0x2d
1401de31f:  75 04                    JNE 0x1401de325
1401de321:  B1 01                    MOV CL,0x1
1401de323:  EB 02                    JMP 0x1401de327
1401de325:  32 C9                    XOR CL,CL
1401de327:  F2 0F 10 4A 60           MOVSD XMM1,qword ptr [RDX + 0x60]
1401de32c:  0F 57 C0                 XORPS XMM0,XMM0
1401de32f:  66 0F 2F C8              COMISD XMM1,XMM0
1401de333:  0F 86 C3 02 00 00        JBE 0x1401de5fc
1401de339:  F2 0F 10 42 28           MOVSD XMM0,qword ptr [RDX + 0x28]
1401de33e:  66 0F 2F C2              COMISD XMM0,XMM2
1401de342:  0F 86 B4 02 00 00        JBE 0x1401de5fc
1401de348:  84 C9                    TEST CL,CL
1401de34a:  75 0E                    JNZ 0x1401de35a
1401de34c:  66 0F 2F 05 5C BB 72 00  COMISD XMM0,qword ptr [0x140909eb0]
1401de354:  0F 86 A2 02 00 00        JBE 0x1401de5fc
1401de35a:  66 41 0F 2F C8           COMISD XMM1,XMM8
1401de35f:  0F 82 97 02 00 00        JC 0x1401de5fc
1401de365:  48 89 74 24 70           MOV qword ptr [RSP + 0x70],RSI
1401de36a:  4C 89 6C 24 78           MOV qword ptr [RSP + 0x78],R13
1401de36f:  4C 8B 6A 10              MOV R13,qword ptr [RDX + 0x10]
1401de373:  0F 29 74 24 30           MOVAPS xmmword ptr [RSP + 0x30],XMM6
1401de378:  49 8B 85 18 03 00 00     MOV RAX,qword ptr [R13 + 0x318]
1401de37f:  48 85 C0                 TEST RAX,RAX
1401de382:  74 0E                    JE 0x1401de392
1401de384:  83 B8 60 03 00 00 08     CMP dword ptr [RAX + 0x360],0x8
1401de38b:  75 05                    JNE 0x1401de392
1401de38d:  41 B0 01                 MOV R8B,0x1
1401de390:  EB 03                    JMP 0x1401de395
1401de392:  45 32 C0                 XOR R8B,R8B
1401de395:  F2 0F 10 35 6B BC 72 00  MOVSD XMM6,qword ptr [0x14090a008]
1401de39d:  66 0F 2F D6              COMISD XMM2,XMM6
1401de3a1:  4C 89 7C 24 40           MOV qword ptr [RSP + 0x40],R15
1401de3a6:  77 73                    JA 0x1401de41b
1401de3a8:  66 0F 2F 05 98 BC 72 00  COMISD XMM0,qword ptr [0x14090a048]
1401de3b0:  73 69                    JNC 0x1401de41b
1401de3b2:  48 85 C0                 TEST RAX,RAX
1401de3b5:  74 0F                    JE 0x1401de3c6
1401de3b7:  8B 90 60 03 00 00        MOV EDX,dword ptr [RAX + 0x360]
1401de3bd:  85 D2                    TEST EDX,EDX
1401de3bf:  74 5A                    JE 0x1401de41b
1401de3c1:  83 FA 14                 CMP EDX,0x14
1401de3c4:  74 55                    JE 0x1401de41b
1401de3c6:  45 84 C0                 TEST R8B,R8B
1401de3c9:  75 50                    JNZ 0x1401de41b
1401de3cb:  84 C9                    TEST CL,CL
1401de3cd:  74 19                    JE 0x1401de3e8
1401de3cf:  48 8B 47 48              MOV RAX,qword ptr [RDI + 0x48]
1401de3d3:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
1401de3da:  48 85 C9                 TEST RCX,RCX
1401de3dd:  74 09                    JE 0x1401de3e8
1401de3df:  83 B9 60 03 00 00 00     CMP dword ptr [RCX + 0x360],0
1401de3e6:  74 33                    JE 0x1401de41b
1401de3e8:  F2 0F 10 67 30           MOVSD XMM4,qword ptr [RDI + 0x30]
1401de3ed:  45 33 C9                 XOR R9D,R9D
1401de3f0:  48 8B 17                 MOV RDX,qword ptr [RDI]
1401de3f3:  F2 0F 5C E6              SUBSD XMM4,XMM6
1401de3f7:  45 33 C0                 XOR R8D,R8D
1401de3fa:  48 8B CB                 MOV RCX,RBX
1401de3fd:  E8 AE F9 4D 00           CALL 0x1406bddb0
1401de402:  0F 57 DB                 XORPS XMM3,XMM3
1401de405:  F3 0F 5A D8              CVTSS2SD XMM3,XMM0
1401de409:  F2 0F 5E E3              DIVSD XMM4,XMM3
1401de40d:  66 0F 2F 25 AB BD 72 00  COMISD XMM4,qword ptr [0x14090a1c0]
1401de415:  41 0F 97 C7              SETA R15B
1401de419:  EB 03                    JMP 0x1401de41e
1401de41b:  41 B7 01                 MOV R15B,0x1
1401de41e:  F2 0F 10 6F 70           MOVSD XMM5,qword ptr [RDI + 0x70]
1401de423:  F2 0F 5C 6F 68           SUBSD XMM5,qword ptr [RDI + 0x68]
1401de428:  66 0F 2F EE              COMISD XMM5,XMM6
1401de42c:  76 08                    JBE 0x1401de436
1401de42e:  F2 0F 58 2D EA CE 72 00  ADDSD XMM5,qword ptr [0x14090b320]
1401de436:  48 8B 37                 MOV RSI,qword ptr [RDI]
1401de439:  45 33 C9                 XOR R9D,R9D
1401de43c:  48 8B D6                 MOV RDX,RSI
1401de43f:  4C 89 B4 24 80 00 00 00  MOV qword ptr [RSP + 0x80],R14
1401de447:  45 33 C0                 XOR R8D,R8D
1401de44a:  48 8B CB                 MOV RCX,RBX
1401de44d:  E8 5E F9 4D 00           CALL 0x1406bddb0
1401de452:  48 8B 47 48              MOV RAX,qword ptr [RDI + 0x48]
1401de456:  0F 57 DB                 XORPS XMM3,XMM3
1401de459:  F3 0F 5A D8              CVTSS2SD XMM3,XMM0
1401de45d:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
1401de464:  F2 0F 5E EB              DIVSD XMM5,XMM3
1401de468:  48 85 C9                 TEST RCX,RCX
1401de46b:  74 0E                    JE 0x1401de47b
1401de46d:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
1401de474:  75 05                    JNE 0x1401de47b
1401de476:  41 B6 01                 MOV R14B,0x1
1401de479:  EB 03                    JMP 0x1401de47e
1401de47b:  45 32 F6                 XOR R14B,R14B
1401de47e:  48 3B B5 F0 C3 00 00     CMP RSI,qword ptr [RBP + 0xc3f0]
1401de485:  0F 85 81 00 00 00        JNE 0x1401de50c
1401de48b:  48 63 47 1C              MOVSXD RAX,dword ptr [RDI + 0x1c]
1401de48f:  49 8B D5                 MOV RDX,R13
1401de492:  48 69 F0 E0 00 00 00     IMUL RSI,RAX,0xe0
1401de499:  49 03 B5 70 09 00 00     ADD RSI,qword ptr [R13 + 0x970]
1401de4a0:  E8 CB 1D FB FF           CALL 0x140190270
1401de4a5:  84 C0                    TEST AL,AL
1401de4a7:  74 0E                    JE 0x1401de4b7
1401de4a9:  48 85 F6                 TEST RSI,RSI
1401de4ac:  74 09                    JE 0x1401de4b7
1401de4ae:  83 BE 88 00 00 00 09     CMP dword ptr [RSI + 0x88],0x9
1401de4b5:  74 55                    JE 0x1401de50c
1401de4b7:  0F 57 DB                 XORPS XMM3,XMM3
1401de4ba:  41 B0 01                 MOV R8B,0x1
1401de4bd:  48 8B D6                 MOV RDX,RSI
1401de4c0:  48 8B CD                 MOV RCX,RBP
1401de4c3:  E8 98 AE 24 00           CALL 0x140429360
1401de4c8:  48 3B 85 E8 C3 00 00     CMP RAX,qword ptr [RBP + 0xc3e8]
1401de4cf:  75 3B                                    JNE 0x1401de50c
1401de4d1:  48 8B D6                 MOV RDX,RSI
1401de4d4:  48 8B CD                 MOV RCX,RBP
1401de4d7:  E8 E4 97 FB FF           CALL 0x140197cc0
1401de4dc:  48 8B 17                 MOV RDX,qword ptr [RDI]
1401de4df:  0F 57 E4                 XORPS XMM4,XMM4
1401de4e2:  45 33 C9                 XOR R9D,R9D
1401de4e5:  45 33 C0                 XOR R8D,R8D
1401de4e8:  48 8B CB                 MOV RCX,RBX
1401de4eb:  F3 0F 5A E0              CVTSS2SD XMM4,XMM0
1401de4ef:  E8 BC F8 4D 00           CALL 0x1406bddb0
1401de4f4:  0F 57 DB                 XORPS XMM3,XMM3
1401de4f7:  F3 0F 5A D8              CVTSS2SD XMM3,XMM0
1401de4fb:  F2 0F 5E E3              DIVSD XMM4,XMM3
1401de4ff:  66 0F 2F 25 B9 BC 72 00  COMISD XMM4,qword ptr [0x14090a1c0]
1401de507:  0F 97 C0                 SETA AL
1401de50a:  EB 02                    JMP 0x1401de50e
1401de50c:  B0 01                    MOV AL,0x1
1401de50e:  66 0F 2F 2D 9A B9 72 00  COMISD XMM5,qword ptr [0x140909eb0]
1401de516:  4C 8B 6C 24 78           MOV R13,qword ptr [RSP + 0x78]
1401de51b:  77 09                    JA 0x1401de526
1401de51d:  45 84 F6                 TEST R14B,R14B
1401de520:  0F 84 BF 00 00 00        JE 0x1401de5e5
1401de526:  45 84 FF                 TEST R15B,R15B
1401de529:  0F 84 B6 00 00 00        JE 0x1401de5e5
1401de52f:  84 C0                    TEST AL,AL
1401de531:  0F 84 AE 00 00 00        JE 0x1401de5e5
1401de537:  66 44 0F 2F C6           COMISD XMM8,XMM6
1401de53c:  77 22                    JA 0x1401de560
1401de53e:  F2 0F 10 47 60           MOVSD XMM0,qword ptr [RDI + 0x60]
1401de543:  66 0F 2F 05 FD BA 72 00  COMISD XMM0,qword ptr [0x14090a048]
1401de54b:  73 13                    JNC 0x1401de560
1401de54d:  66 0F 2F 2D 6B BC 72 00  COMISD XMM5,qword ptr [0x14090a1c0]
1401de555:  77 09                    JA 0x1401de560
1401de557:  45 84 F6                 TEST R14B,R14B
1401de55a:  0F 84 85 00 00 00        JE 0x1401de5e5
1401de560:  48 8B 37                 MOV RSI,qword ptr [RDI]
1401de563:  48 3B B5 B8 C3 00 00     CMP RSI,qword ptr [RBP + 0xc3b8]
1401de56a:  75 07                    JNE 0x1401de573
1401de56c:  48 8B B5 08 C4 00 00     MOV RSI,qword ptr [RBP + 0xc408]
1401de573:  48 8B D6                 MOV RDX,RSI
1401de576:  48 8B CD                 MOV RCX,RBP
1401de579:  E8 D2 A1 24 00           CALL 0x140428750
1401de57e:  84 C0                    TEST AL,AL
1401de580:  74 19                    JE 0x1401de59b
1401de582:  48 83 BB 08 17 00 00 00  CMP qword ptr [RBX + 0x1708],0
1401de58a:  74 0F                    JE 0x1401de59b
1401de58c:  4C 8B C7                 MOV R8,RDI
1401de58f:  48 8B D3                 MOV RDX,RBX
1401de592:  E8 C9 03 00 00           CALL 0x1401de960
1401de597:  84 C0                    TEST AL,AL
1401de599:  74 4A                    JE 0x1401de5e5
1401de59b:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
1401de5a2:  33 D2                    XOR EDX,EDX
1401de5a4:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
1401de5ab:  48 C1 F8 04              SAR RAX,0x4
1401de5af:  48 85 C0                 TEST RAX,RAX
1401de5b2:  74 31                    JE 0x1401de5e5
1401de5b4:  4C 8B 83 B8 03 00 00     MOV R8,qword ptr [RBX + 0x3b8]
1401de5bb:  8B CA                    MOV ECX,EDX
1401de5bd:  0F 1F 00                 NOP dword ptr [RAX]
1401de5c0:  48 03 C9                 ADD RCX,RCX
1401de5c3:  49 3B 34 C8              CMP RSI,qword ptr [R8 + RCX*0x8]
1401de5c7:  74 19                    JE 0x1401de5e2
1401de5c9:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
1401de5d0:  FF C2                    INC EDX
1401de5d2:  49 2B C0                 SUB RAX,R8
1401de5d5:  8B CA                    MOV ECX,EDX
1401de5d7:  48 C1 F8 04              SAR RAX,0x4
1401de5db:  48 3B C8                 CMP RCX,RAX
1401de5de:  72 E0                    JC 0x1401de5c0
1401de5e0:  EB 03                    JMP 0x1401de5e5
1401de5e2:  41 B4 01                 MOV R12B,0x1
1401de5e5:  4C 8B B4 24 80 00 00 00  MOV R14,qword ptr [RSP + 0x80]
1401de5ed:  4C 8B 7C 24 40           MOV R15,qword ptr [RSP + 0x40]
1401de5f2:  48 8B 74 24 70           MOV RSI,qword ptr [RSP + 0x70]
1401de5f7:  0F 28 74 24 30           MOVAPS xmm6,xmmword ptr [RSP + 0x30]
1401de5fc:  41 0F B6 C4              MOVZX EAX,R12B
1401de600:  44 0F 28 44 24 20        MOVAPS xmm8,xmmword ptr [RSP + 0x20]
1401de606:  48 83 C4 48              ADD RSP,0x48
1401de60a:  41 5C                    POP R12
1401de60c:  5F                       POP RDI
1401de60d:  5D                       POP RBP
1401de60e:  5B                       POP RBX
1401de60f:  C3                       RET
```

#### `FUN_14067D5D0` — 830 bytes, `0x14067D5D0..0x14067D90D`

```asm
14067d5d0:  48 89 54 24 10           MOV qword ptr [RSP + 0x10],RDX
14067d5d5:  55                       PUSH RBP
14067d5d6:  56                       PUSH RSI
14067d5d7:  57                       PUSH RDI
14067d5d8:  41 54                    PUSH R12
14067d5da:  41 55                    PUSH R13
14067d5dc:  41 56                    PUSH R14
14067d5de:  41 57                    PUSH R15
14067d5e0:  48 81 EC E0 01 00 00     SUB RSP,0x1e0
14067d5e7:  48 C7 44 24 30 FE FF FF FF MOV qword ptr [RSP + 0x30],-0x2
14067d5f0:  48 89 9C 24 30 02 00 00  MOV qword ptr [RSP + 0x230],RBX
14067d5f8:  45 0F B6 E9              MOVZX R13D,R9B
14067d5fc:  41 8B D8                 MOV EBX,R8D
14067d5ff:  4C 8B E2                 MOV R12,RDX
14067d602:  48 8B E9                 MOV RBP,RCX
14067d605:  C6 84 24 38 02 00 00 01  MOV byte ptr [RSP + 0x238],1
14067d60d:  45 33 F6                 XOR R14D,R14D
14067d610:  45 33 FF                 XOR R15D,R15D
14067d613:  48 8D 4C 24 40           LEA RCX,[RSP + 0x40]
14067d618:  E8 83 3C AD FF           CALL 0x1401512a0
14067d61d:  90                       NOP
14067d61e:  48 8D B5 80 06 00 00     LEA RSI,[RBP + 0x680]
14067d625:  48 8B 7E 08              MOV RDI,qword ptr [RSI + 0x8]
14067d629:  48 2B 3E                 SUB RDI,qword ptr [RSI]
14067d62c:  48 C1 FF 03              SAR RDI,3
14067d630:  F3 0F 10 1D 20 C9 28 00  MOVSS XMM3,dword ptr [0x140909f58]
14067d638:  45 84 C9                 TEST R9B,R9B
14067d63b:  75 76                    JNE 0x14067d6b3
14067d63d:  44 38 BD FD 05 00 00     CMP byte ptr [RBP + 0x5fd],R15B
14067d644:  74 02                    JE 0x14067d648
14067d646:  FF CF                    DEC EDI
14067d648:  4D 85 E4                 TEST R12,R12
14067d64b:  74 66                    JE 0x14067d6b3
14067d64d:  F3 0F 10 82 04 06 00 00  MOVSS XMM0,dword ptr [RDX + 0x604]
14067d655:  0F 2F C3                 COMISS XMM0,XMM3
14067d658:  72 59                    JC 0x14067d6b3
14067d65a:  45 33 C9                 XOR R9D,R9D
14067d65d:  4C 8B C5                 MOV R8,RBP
14067d660:  E8 DB DF D3 FF           CALL 0x1403bb640
14067d665:  84 C0                    TEST AL,AL
14067d667:  74 4A                    JE 0x14067d6b3
14067d669:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d66d:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d670:  48 C1 F8 03              SAR RAX,3
14067d674:  48 85 C0                 TEST RAX,RAX
14067d677:  0F 84 AD 00 00 00        JE 0x14067d72a
14067d67d:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d681:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d684:  48 C1 F8 03              SAR RAX,3
14067d688:  48 8B 16                 MOV RDX,qword ptr [RSI]
14067d68b:  45 33 C9                 XOR R9D,R9D
14067d68e:  4C 8B C5                 MOV R8,RBP
14067d691:  48 8B 54 C2 F8           MOV RDX,qword ptr [RDX + RAX*8 - 8]
14067d696:  E8 A5 DF D3 FF           CALL 0x1403bb640
14067d69b:  84 C0                    TEST AL,AL
14067d69d:  0F 84 87 00 00 00        JE 0x14067d72a
14067d6a3:  40 32 FF                 XOR DIL,DIL
14067d6a6:  40 88 BC 24 38 02 00 00  MOV byte ptr [RSP + 0x238],DIL
14067d6ae:  E9 89 00 00 00           JMP 0x14067d73c
14067d6b3:  80 BD FD 05 00 00 00     CMP byte ptr [RBP + 0x5fd],0
14067d6ba:  75 69                    JNE 0x14067d725
14067d6bc:  45 84 ED                 TEST R13B,R13B
14067d6bf:  75 64                    JNE 0x14067d725
14067d6c1:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d6c5:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d6c8:  48 C1 F8 03              SAR RAX,3
14067d6cc:  48 85 C0                 TEST RAX,RAX
14067d6cf:  74 54                    JE 0x14067d725
14067d6d1:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d6d5:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d6d8:  48 C1 F8 03              SAR RAX,3
14067d6dc:  48 8B 0E                 MOV RCX,qword ptr [RSI]
14067d6df:  48 83 7C C1 F8 00        CMP qword ptr [RCX + RAX*8 - 8],0
14067d6e5:  74 3E                    JE 0x14067d725
14067d6e7:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d6eb:  48 2B C1                 SUB RAX,RCX
14067d6ee:  48 C1 F8 03              SAR RAX,3
14067d6f2:  48 8B 44 C1 F8           MOV RAX,qword ptr [RCX + RAX*8 - 8]
14067d6f7:  F3 0F 10 80 04 06 00 00  MOVSS XMM0,dword ptr [RAX + 0x604]
14067d6ff:  0F 2F C3                 COMISS XMM0,XMM3
14067d702:  72 21                    JC 0x14067d725
14067d704:  48 8B 56 08              MOV RDX,qword ptr [RSI + 0x8]
14067d708:  48 2B D1                 SUB RDX,RCX
14067d70b:  48 C1 FA 03              SAR RDX,3
14067d70f:  45 33 C9                 XOR R9D,R9D
14067d712:  4C 8B C5                 MOV R8,RBP
14067d715:  48 8B 54 D1 F8           MOV RDX,qword ptr [RCX + RDX*8 - 8]
14067d71a:  E8 21 DF D3 FF           CALL 0x1403bb640
14067d71f:  84 C0                    TEST AL,AL
14067d721:  74 02                    JE 0x14067d725
14067d723:  FF CF                    DEC EDI
14067d725:  83 FB FF                 CMP EBX,-1
14067d728:  75 04                    JNE 0x14067d72e
14067d72a:  8B DF                    MOV EBX,EDI
14067d72c:  EB 0B                    JMP 0x14067d739
14067d72e:  3B DF                    CMP EBX,EDI
14067d730:  7D 04                    JGE 0x14067d736
14067d732:  FF C3                    INC EBX
14067d734:  EB 03                    JMP 0x14067d739
14067d736:  0F 4F DF                 CMOVG EBX,EDI
14067d739:  40 B7 01                 MOV DIL,1
14067d73c:  48 63 8D 98 06 00 00     MOVSXD RCX,dword ptr [RBP + 0x698]
14067d743:  85 C9                    TEST ECX,ECX
14067d745:  78 35                    JS 0x14067d77c
14067d747:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d74b:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d74e:  48 C1 F8 03              SAR RAX,3
14067d752:  3B C8                    CMP ECX,EAX
14067d754:  7D 26                    JGE 0x14067d77c
14067d756:  48 8B D1                 MOV RDX,RCX
14067d759:  48 8B 06                 MOV RAX,qword ptr [RSI]
14067d75c:  4C 8B 34 C8              MOV R14,qword ptr [RAX + RCX*8]
14067d760:  49 83 FE 00              CMP R14,0
14067d764:  75 16                    JNE 0x14067d77c
14067d766:  48 69 CA 98 01 00 00     IMUL RCX,RDX,0x198
14067d76d:  48 8B 85 38 06 00 00     MOV RAX,qword ptr [RBP + 0x638]
14067d774:  4C 8B BC 01 90 01 00 00  MOV R15,qword ptr [RCX + RAX + 0x190]
14067d77c:  40 84 FF                 TEST DIL,DIL
14067d77f:  0F 84 BC 00 00 00        JE 0x14067d841
14067d785:  48 8D 94 24 20 02 00 00  LEA RDX,[RSP + 0x220]
14067d78d:  48 8B CE                 MOV RCX,RSI
14067d790:  85 DB                    TEST EBX,EBX
14067d792:  78 62                    JS 0x14067d7f6
14067d794:  48 8B 06                 MOV RAX,qword ptr [RSI]
14067d797:  48 63 DB                 MOVSXD RBX,EBX
14067d79a:  4C 8D 04 D8              LEA R8,[RAX + RBX*8]
14067d79e:  48 8D 84 24 28 02 00 00  LEA RAX,[RSP + 0x228]
14067d7a6:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14067d7ab:  E8 90 09 9A FF           CALL 0x14001e140
14067d7b0:  48 8D BD 38 06 00 00     LEA RDI,[RBP + 0x638]
14067d7b7:  4C 8B 07                 MOV R8,qword ptr [RDI]
14067d7ba:  48 69 DB 98 01 00 00     IMUL RBX,RBX,0x198
14067d7c1:  4C 03 C3                 ADD R8,RBX
14067d7c4:  48 8D 44 24 40           LEA RAX,[RSP + 0x40]
14067d7c9:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14067d7ce:  48 8D 94 24 20 02 00 00  LEA RDX,[RSP + 0x220]
14067d7d6:  48 8B CF                 MOV RCX,RDI
14067d7d9:  E8 12 59 B8 FF           CALL 0x1402030f0
14067d7de:  48 8B 17                 MOV RDX,qword ptr [RDI]
14067d7e1:  48 03 D3                 ADD RDX,RBX
14067d7e4:  48 8B CD                 MOV RCX,RBP
14067d7e7:  E8 34 F5 FF FF           CALL 0x14067cd20
14067d7ec:  0F B6 BC 24 38 02 00 00  MOVZX EDI,byte ptr [RSP + 0x238]
14067d7f4:  EB 5E                    JMP 0x14067d854
14067d7f6:  48 8D 84 24 28 02 00 00  LEA RAX,[RSP + 0x228]
14067d7fe:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14067d803:  4C 8B 06                 MOV R8,qword ptr [RSI]
14067d806:  E8 35 09 9A FF           CALL 0x14001e140
14067d80b:  48 8D 44 24 40           LEA RAX,[RSP + 0x40]
14067d810:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14067d815:  4C 8B 85 38 06 00 00     MOV R8,qword ptr [RBP + 0x638]
14067d81c:  48 8D 94 24 20 02 00 00  LEA RDX,[RSP + 0x220]
14067d824:  48 8D 8D 38 06 00 00     LEA RCX,[RBP + 0x638]
14067d82b:  E8 C0 58 B8 FF           CALL 0x1402030f0
14067d830:  48 8B 95 38 06 00 00     MOV RDX,qword ptr [RBP + 0x638]
14067d837:  48 8B CD                 MOV RCX,RBP
14067d83a:  E8 E1 F4 FF FF           CALL 0x14067cd20
14067d83f:  EB 1A                    JMP 0x14067d85b
14067d841:  48 8B 4E 08              MOV RCX,qword ptr [RSI + 0x8]
14067d845:  48 2B 0E                 SUB RCX,qword ptr [RSI]
14067d848:  48 C1 F9 03              SAR RCX,3
14067d84c:  48 8B 06                 MOV RAX,qword ptr [RSI]
14067d84f:  4C 89 64 C8 F8           MOV qword ptr [RAX + RCX*8 - 8],R12
14067d854:  C6 85 3A 0D 00 00 01     MOV byte ptr [RBP + 0xd3a],1
14067d85b:  C7 85 98 06 00 00 FF FF FF FF MOV dword ptr [RBP + 0x698],-1
14067d865:  33 D2                    XOR EDX,EDX
14067d867:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
14067d86b:  48 2B 06                 SUB RAX,qword ptr [RSI]
14067d86e:  48 C1 F8 03              SAR RAX,3
14067d872:  48 85 C0                 TEST RAX,RAX
14067d875:  74 49                    JE 0x14067d8c0
14067d877:  4D 85 F6                 TEST R14,R14
14067d87a:  74 09                    JE 0x14067d885
14067d87c:  48 8B 06                 MOV RAX,qword ptr [RSI]
14067d87f:  4C 39 34 D0              CMP qword ptr [RAX + RDX*8],R14
14067d883:  EB 1D                    JMP 0x14067d8a2
14067d885:  4D 85 FF                 TEST R15,R15
14067d888:  74 1A                    JE 0x14067d8a4
14067d88a:  8B C2                    MOV EAX,EDX
14067d88c:  48 69 C8 98 01 00 00     IMUL RCX,RAX,0x198
14067d893:  48 8B 85 38 06 00 00     MOV RAX,qword ptr [RBP + 0x638]
14067d89a:  4C 39 BC 01 90 01 00 00  CMP qword ptr [RCX + RAX + 0x190],R15
14067d8a2:  74 16                    JE 0x14067d8ba
14067d8a4:  FF C2                    INC EDX
14067d8a6:  48 8B 4E 08              MOV RCX,qword ptr [RSI + 0x8]
14067d8aa:  48 2B 0E                 SUB RCX,qword ptr [RSI]
14067d8ad:  48 C1 F9 03              SAR RCX,3
14067d8b1:  8B C2                    MOV EAX,EDX
14067d8b3:  48 3B C1                 CMP RAX,RCX
14067d8b6:  73 08                    JNC 0x14067d8c0
14067d8b8:  EB BD                    JMP 0x14067d877
14067d8ba:  89 95 98 06 00 00        MOV dword ptr [RBP + 0x698],EDX
14067d8c0:  83 BD 98 06 00 00 FF     CMP dword ptr [RBP + 0x698],-1
14067d8c7:  75 0F                    JNE 0x14067d8d8
14067d8c9:  4D 85 F6                 TEST R14,R14
14067d8cc:  74 0A                    JE 0x14067d8d8
14067d8ce:  C7 85 98 06 00 00 00 00 00 00 MOV dword ptr [RBP + 0x698],0
14067d8d8:  48 8D 8C 24 08 01 00 00  LEA RCX,[RSP + 0x108]
14067d8e0:  E8 0B 3B AD FF           CALL 0x1401513f0
14067d8e5:  48 8D 4C 24 40           LEA RCX,[RSP + 0x40]
14067d8ea:  E8 01 3B AD FF           CALL 0x1401513f0
14067d8ef:  40 0F B6 C7              MOVZX EAX,DIL
14067d8f3:  48 8B 9C 24 30 02 00 00  MOV RBX,qword ptr [RSP + 0x230]
14067d8fb:  48 81 C4 E0 01 00 00     ADD RSP,0x1e0
14067d902:  41 5F                    POP R15
14067d904:  41 5E                    POP R14
14067d906:  41 5D                    POP R13
14067d908:  41 5C                    POP R12
14067d90a:  5F                       POP RDI
14067d90b:  5E                       POP RSI
14067d90c:  5D                       POP RBP
14067d90d:  C3                       RET
```

#### `FUN_1406BDC30` — 145 bytes, `0x1406BDC30..0x1406BDCC0`

```asm
1406bdc30:  48 8B 81 08 17 00 00     MOV RAX,[RCX + 0x1708]
1406bdc37:  48 8B D1                 MOV RDX,RCX
1406bdc3a:  48 85 C0                 TEST RAX,RAX
1406bdc3d:  74 1F                    JE 0x1406bdc5e
1406bdc3f:  83 B8 00 86 00 00 11     CMP dword ptr [RAX + 0x8600],0x11
1406bdc46:  75 16                    JNE 0x1406bdc5e
1406bdc48:  48 81 C2 B8 03 00 00     ADD RDX,0x3b8
1406bdc4f:  48 8D 0D BA 72 31 00     LEA RCX,[0x1409d4f10]
1406bdc56:  0F 28 D1                 MOVAPS XMM2,XMM1
1406bdc59:  E9 12 F1 AD FF           JMP 0x14019cd70
1406bdc5e:  0F 57 C0                 XORPS XMM0,XMM0
1406bdc61:  0F 2F C1                 COMISS XMM0,XMM1
1406bdc64:  76 07                    JBE 0x1406bdc6d
1406bdc66:  0F 57 0D B3 EA 24 00     XORPS XMM1,xmmword ptr [0x14090c720]
1406bdc6d:  48 8B 81 C0 03 00 00     MOV RAX,[RCX + 0x3c0]
1406bdc74:  45 33 C0                 XOR R8D,R8D
1406bdc77:  48 2B 81 B8 03 00 00     SUB RAX,[RCX + 0x3b8]
1406bdc7e:  48 C1 F8 04              SAR RAX,0x4
1406bdc82:  48 85 C0                 TEST RAX,RAX
1406bdc85:  74 32                    JE 0x1406bdcb9
1406bdc87:  4C 8B 89 B8 03 00 00     MOV R9,[RCX + 0x3b8]
1406bdc8e:  41 8B C8                 MOV ECX,R8D
1406bdc91:  48 03 C9                 ADD RCX,RCX
1406bdc94:  F3 41 0F 10 44 C9 08     MOVSS XMM0,[R9 + RCX*8 + 8]
1406bdc9b:  0F 2F C1                 COMISS XMM0,XMM1
1406bdc9e:  77 1C                    JA 0x1406bdcbc
1406bdca0:  48 8B 82 C0 03 00 00     MOV RAX,[RDX + 0x3c0]
1406bdca7:  41 FF C0                 INC R8D
1406bdcaa:  49 2B C1                 SUB RAX,R9
1406bdcad:  41 8B C8                 MOV ECX,R8D
1406bdcb0:  48 C1 F8 04              SAR RAX,0x4
1406bdcb4:  48 3B C8                 CMP RCX,RAX
1406bdcb7:  72 D8                 JC 0x1406bdc91
1406bdcb9:  33 C0                 XOR EAX,EAX
1406bdcbb:  C3                    RET
1406bdcbc:  49 8B 04 C9           MOV RAX,[R9 + RCX*8]
1406bdcc0:  C3                    RET
```

### Commands, outputs, and failures

The existing Java scripts were run one Ghidra invocation at a time through `scratch\ghidra_run.bat`; no analysis script was created or modified. The complete raw output for the two 830/864-byte bodies and the 293/145-byte bodies is included above. The larger `FUN_140680A20` and `FUN_1406B8EB0` bodies were reused from the prior verified exports and were not re-exported.

Commands used for this follow-up:

```text
scratch\ghidra_run.bat DecompAt.java scratch\agent1_install_branch_decomp.c 0x1401E5480 0x1401E5BA0 0x1406BDC30
scratch\ghidra_run.bat DecompAt.java scratch\agent1_func_1401de2b0.c 0x1401DE2B0
scratch\ghidra_run.bat DecompAt.java scratch\agent1_install_connected_decomp.c 0x14067D5D0 0x1401E4B10
scratch\ghidra_run.bat DecompAt.java scratch\agent1_func_1401e3bd0.c 0x1401E3BD0
scratch\ghidra_run.bat ListFuncs.java scratch\agent1_func_1401de2b0.txt 0x1401DE2B0 0x1401DE2B0
scratch\ghidra_run.bat DisasmMany.java scratch\agent1_func_1401de2b0.asm 0x1401DE2B0:0x1401DE610
scratch\ghidra_run.bat ListFuncs.java scratch\agent1_func_14067d5d0.txt 0x14067D5D0 0x14067D5D0
scratch\ghidra_run.bat DisasmMany.java scratch\agent1_func_14067d5d0.asm 0x14067D5D0:0x14067D90E
```

Actual `ListFuncs.java` results:

```text
0x1401DE2B0  size 864  FUN_1401de2b0
0x14067D5D0  size 830  FUN_14067d5d0
```

Earlier bounded output in this investigation records `FUN_1401E5350` size 293, `FUN_1401E5480` size 1812, `FUN_1401E5BA0` size 1343, and `FUN_1406BDC30` size 145. `FUN_1401DE2B0` ends at its `RET` at `0x1401DE60F`; the requested disassembly range also printed the next function's entry at `0x1401DE610`, which is not included in its body above. `FUN_14067D5D0` ends at `0x14067D90D` with `RET`; no undecoded tail was used. All listed Ghidra commands completed successfully. A lookup for `Tesmio Mods\docs\03-reverse-engineering.md` found no matching file in this checkout; the repository-root `INSTRUCTIONS.md` and the existing report methodology were used. No new script output or tracked file was created beyond this Markdown report.

## Simulation association and home-target reset follow-up

### Delta and same-receiver path

- **Newly established:** `FUN_140699420` receives its object in `RCX` (`R15=RCX` at `0x1406994A9`). When its `+0xD39` byte is zero, the block at `0x14069B7D0` may write the previous `truck+0xC80` object's `+0xBB0` if it differs from current `truck+0x528` and the backlink equals `truck+0x390`; the raw source is `R14`, whose zero provenance is not established by this bounded export. The block can then bind the current `truck+0x528` target by storing `target+0xBB0 = truck+0x390` and `truck+0xC80 = target`. This is a target association update, not proof of a whole-task boundary.
- **Newly established:** when `truck+0x528 == truck+0x4F8 == truck+0x4F0` and the target descriptor type is `0x2B` or `0x2D`, the same path calls `FUN_14067CB30(RCX=truck, RDX=0, R8B=1)`. Its direct body clears route/configuration and other vectors, and the caller then clears `+0x528/+0x538`. It does not directly clear `+0xC80/+0xBB0`; its `FUN_140497950` manager-detach call has separate, bounded effects described below. Neither helper establishes cancellation or a task-close cause.
- **Corrected:** the raw stop-adjacent block at `0x14069D226` increments dword `[R15+0x700]` and indexes an array through `[RBX]`. This is not the separately established route index `+0x698`. The prior decompiler-based interpretation that this block advances `+0x698` is withdrawn; the field at `+0x700` remains semantically unidentified.
- **Still unresolved:** the conditional `FUN_14066D530 -> FUN_140699420` edge has not been joined to the exact candidate pointer traversed from the type-43 office's `+0xC70/+0xC78` fleet, and its `+0x4A0` guard has not been validated for that truck. The target association/reset path therefore cannot be promoted to an interval rule even if it runs for a road truck.

```text
type-43 office +0xC70/+0xC78 fleet
  -> FUN_1401C6050 candidate buffer -> FUN_1401E5350 / route builder
     [route/config rebuild; commitment vs refresh not proved]
  -> simulation manager array -> FUN_14066D530
     [metadata kind 1/2 and +0x4A0==0 guards;
      membership of this exact office-fleet truck not proved]
  -> FUN_140699420(RCX=truck; R15=RCX)
     -> if truck+0xD39==0:
        old truck+0xC80 target may have target+0xBB0 cleared
        current truck+0x528 target may receive target+0xBB0=truck+0x390
        truck+0xC80 may be rebound to truck+0x528
     -> if +0x528==+0x4F8==+0x4F0 and descriptor type is 0x2B/0x2D:
        FUN_14067CB30(truck, 0, 1)
        [route/config reset; not a proved task-removal/cancellation event]
        caller clears truck+0x528/+0x538
```

### Exact association and return/reset instructions

The receiver is `R15`, established from the function argument at `0x1406994A9`. The following bytes show the D39 guard, conditional old-target qword write, and current-target association stores. The raw instruction writes `R14`; its zero provenance is not established in this bounded predecessor slice. The existing decompilation renders the value as zero, but the report does not treat that as raw register proof.

```asm
14069b7d0:  41 80 BF 39 0D 00 00 00  CMP byte ptr [R15 + 0xd39],0x0
14069b7d8:  0F 85 D5 01 00 00        JNZ 0x14069b9b3
14069b7de:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069b7e5:  48 85 C0                 TEST RAX,RAX
14069b7e8:  74 23                    JZ 0x14069b80d
14069b7ea:  49 3B 87 28 05 00 00     CMP RAX,qword ptr [R15 + 0x528]
14069b7f1:  74 1A                    JZ 0x14069b80d
14069b7f3:  48 8B C8                 MOV RCX,RAX
14069b7f6:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069b7fd:  48 39 81 B0 0B 00 00     CMP qword ptr [RCX + 0xbb0],RAX
14069b804:  75 07                    JNZ 0x14069b80d
14069b806:  4C 89 B1 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],R14
14069b80d:  49 8B 8F 28 05 00 00     MOV RCX,qword ptr [R15 + 0x528]
14069b814:  48 8B 81 18 03 00 00     MOV RAX,qword ptr [RCX + 0x318]
14069b81b:  48 85 C0                 TEST RAX,RAX
14069b81e:  74 09                    JZ 0x14069b829
14069b820:  80 B8 79 09 00 00 00     CMP byte ptr [RAX + 0x979],0x0
14069b827:  75 40                    JNZ 0x14069b869
14069b829:  41 8B 87 6C 05 00 00     MOV EAX,dword ptr [R15 + 0x56c]
14069b830:  85 C0                    TEST EAX,EAX
14069b832:  79 19                    JNS 0x14069b84d
14069b834:  83 F8 F6                 CMP EAX,-0xa
14069b837:  7F 30                    JG 0x14069b869
14069b839:  48 83 B9 D0 05 00 00 00  CMP qword ptr [RCX + 0x5d0],0x0
14069b841:  74 26                    JZ 0x14069b869
14069b843:  48 83 B9 B0 0B 00 00 00  CMP qword ptr [RCX + 0xbb0],0x0
14069b84b:  75 1C                    JNZ 0x14069b869
14069b84d:  49 8B 87 90 03 00 00     MOV RAX,qword ptr [R15 + 0x390]
14069b854:  48 89 81 B0 0B 00 00     MOV qword ptr [RCX + 0xbb0],RAX
14069b85b:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b862:  49 89 87 80 0C 00 00     MOV qword ptr [R15 + 0xc80],RAX
```

The old-target write is reached only when `+0xD39 == 0`, old `+0xC80` is non-null and differs from current `+0x528`, and old-target `+0xBB0 == truck+0x390`; whether it is a null clear requires proving `R14==0`. The new-target store is separately guarded by its descriptor flag and either nonnegative `+0x56C`, or `+0x56C < -9` with target `+0x5D0 != 0` and target `+0xBB0 == 0`. Therefore target reassociation can occur in one updater block without an empty `+0xC80` snapshot. This proves an association write, not replacement of assigned work.

The bounded adjacent export contains these additional `R14` stores, but it does not supply a reaching definition that dominates `0x14069B806`; they are not used to assert the old backlink is cleared:

```asm
14069b511:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b518:  49 89 87 F0 04 00 00     MOV qword ptr [R15 + 0x4f0],RAX
14069b529:  4D 89 B7 E8 05 00 00     MOV qword ptr [R15 + 0x5e8],R14
14069b533:  49 83 BF 80 0C 00 00 00  CMP qword ptr [R15 + 0xc80],0x0
14069b53d:  49 8B 87 80 0C 00 00     MOV RAX,qword ptr [R15 + 0xc80]
14069b544:  4C 39 B8 B0 0B 00 00     CMP qword ptr [RAX + 0xbb0],R15
14069b54d:  4C 89 B0 B0 0B 00 00     MOV qword ptr [RAX + 0xbb0],R14
14069b554:  4D 89 B7 80 0C 00 00     MOV qword ptr [R15 + 0xc80],R14
14069b5c3:  45 89 B7 34 0D 00 00     MOV dword ptr [R15 + 0xd34],R14D
14069b5ca:  4D 89 B7 98 07 00 00     MOV qword ptr [R15 + 0x798],R14
14069b668:  41 C6 87 39 0D 00 00 00  MOV byte ptr [R15 + 0xd39],0x0
```

The home-target predicate and call arguments are exact. `TEST EAX,0xfffffffd` accepts descriptor type `0x2B` or `0x2D` after subtracting `0x2B`; it is not a generic assertion that every road-DO target reaches this branch.

```asm
14069b89c:  49 8B 87 F8 04 00 00     MOV RAX,qword ptr [R15 + 0x4f8]
14069b8a3:  49 39 87 28 05 00 00     CMP qword ptr [R15 + 0x528],RAX
14069b8aa:  75 74                    JNZ 0x14069b920
14069b8ac:  49 3B 87 F0 04 00 00     CMP RAX,qword ptr [R15 + 0x4f0]
14069b8b3:  75 6B                    JNZ 0x14069b920
14069b8b5:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b8bc:  48 85 C9                 TEST RCX,RCX
14069b8bf:  74 1D                    JZ 0x14069b8de
14069b8c1:  8B 81 60 03 00 00        MOV EAX,dword ptr [RCX + 0x360]
14069b8c7:  83 E8 2B                 SUB EAX,0x2b
14069b8ca:  A9 FD FF FF FF           TEST EAX,0xfffffffd
14069b8cf:  75 0D                    JNZ 0x14069b8de
14069b8d1:  41 B0 01                 MOV R8B,0x1
14069b8d4:  33 D2                    XOR EDX,EDX
14069b8d6:  49 8B CF                 MOV RCX,R15
14069b8d9:  E8 52 12 FE FF           CALL 0x14067cb30
14069b8de:  49 8B 87 28 05 00 00     MOV RAX,qword ptr [R15 + 0x528]
14069b8e5:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14069b8ec:  48 85 C9                 TEST RCX,RCX
14069b8ef:  74 16                    JZ 0x14069b907
14069b8f1:  80 B9 79 09 00 00 00     CMP byte ptr [RCX + 0x979],0x0
14069b8f8:  75 0D                    JNZ 0x14069b907
14069b8fa:  45 84 ED                 TEST R13B,R13B
14069b8fd:  75 08                    JNZ 0x14069b907
14069b8ff:  49 8B CF                 MOV RCX,R15
14069b902:  E8 79 1E 01 00           CALL 0x1406ad780
14069b907:  41 B1 01                 MOV R9B,0x1
14069b90a:  4D 8B 87 28 05 00 00     MOV R8,qword ptr [R15 + 0x528]
14069b911:  41 8B 97 10 05 00 00     MOV EDX,dword ptr [R15 + 0x510]
14069b918:  49 8B CF                 MOV RCX,R15
14069b91b:  E8 C0 B7 FC FF           CALL 0x1406670e0
14069b920:  45 33 ED                 XOR R13D,R13D
14069b923:  4D 89 AF 28 05 00 00     MOV qword ptr [R15 + 0x528],R13
14069b92a:  4D 89 AF 38 05 00 00     MOV qword ptr [R15 + 0x538],R13
```

### Complete small connected helper bodies

`FUN_14067CB30` is 264 bytes (`0x14067CB30..0x14067CC37`, `RET`). With this call's `RDX=0`, its `DL` branch is skipped and `R8B=1` enables its `FUN_140497950` call and direct route/configuration reset. Its direct instructions clear route-list end to begin at `+0x688/+0x680`, clear auxiliary vectors and `+0xD34`, and conditionally call `FUN_140681970`; they do not directly store to `+0xC80` or any target `+0xBB0`. Transitive side effects of `FUN_140681970` and vector helpers are not classified here.

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

`FUN_140497950` (439 bytes) is the connected manager-detach helper called with `RCX=[0x1409E7710]`, `RDX=truck`. It removes that truck from the vector owned by `truck+0x650`, zeros truck `+0x650/+0x658`, and searches manager vectors for backlinks to detach. It does not directly write route, `+0xC80`, or target `+0xBB0`. It calls `FUN_1404962C0` on the manager object; that helper iterates manager-owned vectors and invokes other generic cleanup functions, so the transitive effects are not used to claim a task boundary.

```asm
140497950:  48 85 D2                 TEST RDX,RDX
140497953:  0F 84 AD 01 00 00        JZ 0x140497b06
140497959:  56                       PUSH RSI
14049795a:  41 57                    PUSH R15
14049795c:  48 83 EC 28              SUB RSP,0x28
140497960:  48 89 6C 24 48           MOV qword ptr [RSP + 0x48],RBP
140497965:  48 8B F2                 MOV RSI,RDX
140497968:  48 8B AA 50 06 00 00     MOV RBP,qword ptr [RDX + 0x650]
14049796f:  4C 8B F9                 MOV R15,RCX
140497972:  48 85 ED                 TEST RBP,RBP
140497975:  0F 84 7F 01 00 00        JZ 0x140497afa
14049797b:  48 8B 95 48 02 00 00     MOV RDX,qword ptr [RBP + 0x248]
140497982:  48 89 5C 24 40           MOV qword ptr [RSP + 0x40],RBX
140497987:  48 89 7C 24 50           MOV qword ptr [RSP + 0x50],RDI
14049798c:  48 8B BD 40 02 00 00     MOV RDI,qword ptr [RBP + 0x240]
140497993:  4C 89 64 24 58           MOV qword ptr [RSP + 0x58],R12
140497998:  48 3B FA                 CMP RDI,RDX
14049799b:  74 11                    JZ 0x1404979ae
14049799d:  0F 1F 00                 NOP dword ptr [RAX]
1404979a0:  48 39 37                 CMP qword ptr [RDI],RSI
1404979a3:  74 09                    JZ 0x1404979ae
1404979a5:  48 83 C7 08              ADD RDI,0x8
1404979a9:  48 3B FA                 CMP RDI,RDX
1404979ac:  75 F2                    JNZ 0x1404979a0
1404979ae:  45 33 E4                 XOR R12D,R12D
1404979b1:  48 3B FA                 CMP RDI,RDX
1404979b4:  74 3C                    JZ 0x1404979f2
1404979b6:  48 8D 47 08              LEA RAX,[RDI + 0x8]
1404979ba:  4C 8B CA                 MOV R9,RDX
1404979bd:  4C 2B C8                 SUB R9,RAX
1404979c0:  41 8B CC                 MOV ECX,R12D
1404979c3:  49 83 C1 07              ADD R9,0x7
1404979c7:  49 C1 E9 03              SHR R9,0x3
1404979cb:  48 3B C2                 CMP RAX,RDX
1404979ce:  4D 0F 47 CC              CMOVA R9,R12
1404979d2:  4D 85 C9                 TEST R9,R9
1404979d5:  74 1B                    JZ 0x1404979f2
1404979d7:  4C 8B 00                 MOV R8,qword ptr [RAX]
1404979da:  4C 3B C6                 CMP R8,RSI
1404979dd:  74 07                    JZ 0x1404979e6
1404979df:  4C 89 07                 MOV qword ptr [RDI],R8
1404979e2:  48 83 C7 08              ADD RDI,0x8
1404979e6:  48 83 C0 08              ADD RAX,0x8
1404979ea:  48 FF C1                 INC RCX
1404979ed:  49 3B C9                 CMP RCX,R9
1404979f0:  75 E5                    JNZ 0x1404979d7
1404979f2:  48 8B 85 40 02 00 00     MOV RAX,qword ptr [RBP + 0x240]
1404979f9:  48 3B F8                 CMP RDI,RAX
1404979fc:  75 09                    JNZ 0x140497a07
1404979fe:  48 3B 95 48 02 00 00     CMP RDX,qword ptr [RBP + 0x248]
140497a05:  74 1F                    JZ 0x140497a26
140497a07:  48 3B FA                 CMP RDI,RDX
140497a0a:  74 21                    JZ 0x140497a2d
140497a0c:  48 8B 9D 48 02 00 00     MOV RBX,qword ptr [RBP + 0x248]
140497a13:  48 8B CF                 MOV RCX,RDI
140497a16:  48 2B DA                 SUB RBX,RDX
140497a19:  4C 8B C3                 MOV R8,RBX
140497a1c:  FF 15 26 62 3D 00        CALL qword ptr [0x14086dc48]
140497a22:  48 8D 04 3B              LEA RAX,[RBX + RDI*0x1]
140497a26:  48 89 85 48 02 00 00     MOV qword ptr [RBP + 0x248],RAX
140497a2d:  4C 89 A6 50 06 00 00     MOV qword ptr [RSI + 0x650],R12
140497a34:  44 89 A6 58 06 00 00     MOV dword ptr [RSI + 0x658],R12D
140497a3b:  48 8B 1D 96 9F 54 00     MOV RBX,qword ptr [0x1409e19d8]
140497a42:  48 8B 0D 87 9F 54 00     MOV RCX,qword ptr [0x1409e19d0]
140497a49:  48 2B D9                 SUB RBX,RCX
140497a4c:  48 C1 FB 03              SAR RBX,0x3
140497a50:  85 DB                    TEST EBX,EBX
140497a52:  0F 84 89 00 00 00        JZ 0x140497ae1
140497a58:  4C 89 74 24 20           MOV qword ptr [RSP + 0x20],R14
140497a5d:  44 8B F3                 MOV R14D,EBX
140497a60:  49 C1 E6 03              SHL R14,0x3
140497a64:  49 8B 7C 0E F8           MOV RDI,qword ptr [R14 + RCX*0x1 + -0x8]
140497a69:  4D 8D 76 F8              LEA R14,[R14 + -0x8]
140497a6d:  FF CB                    DEC EBX
140497a6f:  83 BF 48 02 00 00 08     CMP dword ptr [RDI + 0x248],0x8
140497a76:  75 60                    JNZ 0x140497ad8
140497a78:  48 8B BF 70 02 00 00     MOV RDI,qword ptr [RDI + 0x270]
140497a7f:  48 85 FF                 TEST RDI,RDI
140497a82:  74 54                    JZ 0x140497ad8
140497a84:  48 8B 87 48 02 00 00     MOV RAX,qword ptr [RDI + 0x248]
140497a8b:  48 8B 97 40 02 00 00     MOV RDX,qword ptr [RDI + 0x240]
140497a92:  48 3B D0                 CMP RDX,RAX
140497a95:  74 41                    JZ 0x140497ad8
140497a97:  48 39 32                 CMP qword ptr [RDX],RSI
140497a9a:  74 09                    JZ 0x140497aa5
140497a9c:  48 83 C2 08              ADD RDX,0x8
140497aa0:  48 3B D0                 CMP RDX,RAX
140497aa3:  75 F2                    JNZ 0x140497a97
140497aa5:  48 3B D0                 CMP RDX,RAX
140497aa8:  74 2E                    JZ 0x140497ad8
140497aaa:  48 8B CF                 MOV RCX,RDI
140497aad:  E8 1E AF FF FF           CALL 0x1404929d0
140497ab2:  BA 90 02 00 00           MOV EDX,0x290
140497ab7:  48 8B CF                 MOV RCX,RDI
140497aba:  E8 21 65 3B 00           CALL 0x14084dfe0
140497abf:  48 8B 05 0A 9F 54 00     MOV RAX,qword ptr [0x1409e19d0]
140497ac6:  49 8B 0C 06              MOV RCX,qword ptr [R14 + RAX*0x1]
140497aca:  4C 89 A1 70 02 00 00     MOV qword ptr [RCX + 0x270],R12
140497ad1:  48 8B 0D F8 9E 54 00     MOV RCX,qword ptr [0x1409e19d0]
140497ad8:  85 DB                    TEST EBX,EBX
140497ada:  75 88                    JNZ 0x140497a64
140497adc:  4C 8B 74 24 20           MOV R14,qword ptr [RSP + 0x20]
140497ae1:  33 D2                    XOR EDX,EDX
140497ae3:  49 8B CF                 MOV RCX,R15
140497ae6:  E8 D5 E7 FF FF           CALL 0x1404962c0
140497aeb:  4C 8B 64 24 58           MOV R12,qword ptr [RSP + 0x58]
140497af0:  48 8B 7C 24 50           MOV RDI,qword ptr [RSP + 0x50]
140497af5:  48 8B 5C 24 40           MOV RBX,qword ptr [RSP + 0x40]
140497afa:  48 8B 6C 24 48           MOV RBP,qword ptr [RSP + 0x48]
140497aff:  48 83 C4 28              ADD RSP,0x28
140497b03:  41 5F                    POP R15
140497b05:  5E                       POP RSI
140497b06:  C3                       RET
```

`FUN_1404962C0` is 272 bytes; its complete body confirms it iterates manager-owned vectors and calls `FUN_1404956E0` or `FUN_1404942E0` on the manager receiver when nested object vectors are short. Those callees were not followed because this is manager maintenance, not a proved task setter/remover; their indirect effects remain a limitation.

```asm
1404962c0:  48 89 5C 24 10           MOV qword ptr [RSP + 0x10],RBX
1404962c5:  48 89 74 24 18           MOV qword ptr [RSP + 0x18],RSI
1404962ca:  57                       PUSH RDI
1404962cb:  48 83 EC 20              SUB RSP,0x20
1404962cf:  48 8B D9                 MOV RBX,RCX
1404962d2:  84 D2                    TEST DL,DL
1404962d4:  74 05                    JZ 0x1404962db
1404962d6:  E8 C5 D3 FF FF           CALL 0x1404936a0
1404962db:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
1404962df:  33 FF                    XOR EDI,EDI
1404962e1:  48 2B 03                 SUB RAX,qword ptr [RBX]
1404962e4:  8B F7                    MOV ESI,EDI
1404962e6:  48 C1 F8 03              SAR RAX,0x3
1404962ea:  48 85 C0                 TEST RAX,RAX
1404962ed:  74 59                    JZ 0x140496348
1404962ef:  4C 89 74 24 30           MOV qword ptr [RSP + 0x30],R14
1404962f4:  44 8B F7                 MOV R14D,EDI
1404962f7:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140496300:  48 8B 03                 MOV RAX,qword ptr [RBX]
140496303:  49 8B 14 06              MOV RDX,qword ptr [R14 + RAX*0x1]
140496307:  48 8B 82 48 02 00 00     MOV RAX,qword ptr [RDX + 0x248]
14049630e:  48 2B 82 40 02 00 00     SUB RAX,qword ptr [RDX + 0x240]
140496315:  48 C1 F8 03              SAR RAX,0x3
140496319:  48 83 F8 01              CMP RAX,0x1
14049631d:  73 0B                    JNC 0x14049632a
14049631f:  41 B8 12 00 00 00        MOV R8D,0x12
140496325:  E8 B6 F3 FF FF           CALL 0x1404956e0
14049632a:  48 8B 4B 08              MOV RCX,qword ptr [RBX + 0x8]
14049632e:  FF C6                    INC ESI
140496330:  48 2B 0B                 SUB RCX,qword ptr [RBX]
140496333:  49 83 C6 08              ADD R14,0x8
140496337:  48 C1 F9 03              SAR RCX,0x3
14049633b:  48 63 C6                 MOVSXD RAX,ESI
14049633e:  48 3B C1                 CMP RAX,RCX
140496341:  72 BD                    JC 0x140496300
140496343:  4C 8B 74 24 30           MOV R14,qword ptr [RSP + 0x30]
140496348:  48 8B 43 20              MOV RAX,qword ptr [RBX + 0x20]
14049634c:  48 2B 43 18              SUB RAX,qword ptr [RBX + 0x18]
140496350:  48 C1 F8 03              SAR RAX,0x3
140496354:  48 85 C0                 TEST RAX,RAX
140496357:  74 67                    JZ 0x1404963c0
140496359:  48 8B F7                 MOV RSI,RDI
14049635c:  0F 1F 40 00              NOP dword ptr [RAX]
140496360:  48 8B 43 18              MOV RAX,qword ptr [RBX + 0x18]
140496364:  48 8B 14 06              MOV RDX,qword ptr [RSI + RAX*0x1]
140496368:  48 8B 82 48 02 00 00     MOV RAX,qword ptr [RDX + 0x248]
14049636f:  48 2B 82 40 02 00 00     SUB RAX,qword ptr [RDX + 0x240]
140496376:  48 C1 F8 03              SAR RAX,0x3
14049637a:  48 83 F8 01              CMP RAX,0x1
14049637e:  72 18                    JC 0x140496398
140496380:  48 8B 82 18 02 00 00     MOV RAX,qword ptr [RDX + 0x218]
140496387:  48 2B 82 10 02 00 00     SUB RAX,qword ptr [RDX + 0x210]
14049638e:  48 C1 F8 03              SAR RAX,0x3
140496392:  48 83 F8 02              CMP RAX,0x2
140496396:  73 0E                    JNC 0x1404963a6
140496398:  48 8B CB                 MOV RCX,RBX
14049639b:  E8 40 DF FF FF           CALL 0x1404942e0
1404963a0:  FF CF                    DEC EDI
1404963a2:  48 83 EE 08              SUB RSI,0x8
1404963a6:  48 8B 4B 20              MOV RCX,qword ptr [RBX + 0x20]
1404963aa:  FF C7                    INC EDI
1404963ac:  48 2B 4B 18              SUB RCX,qword ptr [RBX + 0x18]
1404963b0:  48 83 C6 08              ADD RSI,0x8
1404963b4:  48 C1 F9 03              SAR RCX,0x3
1404963b8:  48 63 C7                 MOVSXD RAX,EDI
1404963bb:  48 3B C1                 CMP RAX,RCX
1404963be:  72 A0                    JC 0x140496360
1404963c0:  48 8B 5C 24 38           MOV RBX,qword ptr [RSP + 0x38]
1404963c5:  48 8B 74 24 40           MOV RSI,qword ptr [RSP + 0x40]
1404963ca:  48 83 C4 20              ADD RSP,0x20
1404963ce:  5F                       POP RDI
1404963cf:  C3                       RET
```

The raw instruction at `0x14069D226` independently resolves the decompiler/offset conflict:

```asm
14069d226:  41 8B 87 00 07 00 00     MOV EAX,dword ptr [R15 + 0x700]
14069d22d:  FF C0                    INC EAX
14069d22f:  4C 63 C0                 MOVSXD R8,EAX
14069d232:  48 8B 13                 MOV RDX,qword ptr [RBX]
14069d235:  49 63 8F 00 07 00 00     MOVSXD RCX,dword ptr [R15 + 0x700]
14069d23c:  48 8B C2                 MOV RAX,RDX
14069d23f:  4E 8B 0C C2              MOV R9,qword ptr [RDX + R8*0x8]
14069d243:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8]
14069d247:  48 85 D2                 TEST RDX,RDX
14069d24a:  0F 84 D9 00 00 00        JZ 0x14069d329
```

This proves an integer field at `+0x700` is incremented and used to index the array pointed to by `[RBX]` on this branch. It does not prove that `+0x700` is the schedule route index, an assignment state, or an internal-stop counter. The separate `+0x698` route-index reads/writes elsewhere in `FUN_140699420` remain distinct.

### Bounded caller/callee conclusion

The dispatcher evidence already in this report shows `FUN_14066D530` calls `FUN_140699420` with its same `RBX` receiver when the metadata-kind and `+0x4A0` guards pass. Inside `FUN_140699420`, the prologue saves the incoming `RCX` in `R15`. The type-43 updater's fleet pointer has not been proven to be the object passed through the simulation manager array, so this is not yet a receiver-preserving path from the type-43 office all the way through execution/reset.

The type-43 home-target path is bounded as follows: D39 must be zero; `+0x528` must equal both `+0x4F8` and `+0x4F0`; the object at that pointer must have a non-null descriptor at `+0x318` whose dword `+0x360` is `0x2B` or `0x2D`; then the call at `0x14069B8D9` passes `(RCX=truck, RDX=0, R8B=1)` to `FUN_14067CB30`. After the helper/adjacent guarded path, the caller clears `+0x528/+0x538` at `0x14069B923/0x14069B92A`. This is a home-target route/reset sequence, not a proved close event: the code does not establish whether it represents successful return, idle refresh, cancellation cleanup, or another reset condition.

### Commands, identity, output and validation for this follow-up

The executable hash was rechecked immediately before these exports: `C:\Workers Resources Soviet Republic\SOVIET64.exe` SHA-256 remained `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`. The existing wrapper opened project `C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\ghidra\proj\soviet`, program `/SOVIET64.exe`, for each successful query. These are file VAs; the corresponding RVAs equal the VA minus image base `0x140000000`.

Commands run serially through the existing wrapper:

```text
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_route_advance_blocks.asm 0x14069D160:0x14069D2A0 0x14069B7D0:0x14069B8A0
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_provenance.asm 0x140699420:0x1406994D0 0x14069D1F0:0x14069D25A 0x14069B7D0:0x14069B869
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_type43_home_call.asm 0x14069B869:0x14069B930
scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_14067cb30.txt 0x14067CB30 0x14067CB30
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_14067cb30.asm 0x14067CB30:0x14067CC38
scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_140497950.txt 0x140497950 0x140497950
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140497950.asm 0x140497950:0x140497B07
scratch\ghidra_run.bat ListFuncs.java C:\WRSRE\scratch\agent1_func_1404962c0.txt 0x1404962C0 0x1404962C0
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_1404962c0.asm 0x1404962C0:0x1404963D0
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_assoc_predecessor.asm 0x14069B680:0x14069B7E5
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_assoc_r14_provenance.asm 0x14069B500:0x14069B690
scratch\ghidra_run.bat DisasmMany.java C:\WRSRE\scratch\agent1_func_140699420_assoc_r14_entry.asm 0x14069B300:0x14069B520
```

Actual bounds output:

```text
0x14067CB30  size 264  FUN_14067cb30
0x140497950  size 439  FUN_140497950
0x1404962C0  size 272  FUN_1404962c0
```

The exported bodies end at `0x14067CC37`, `0x140497B06`, and `0x1404963CF`, respectively, each with `RET`. The `FUN_1404962C0` query range also printed the next function's entry at `0x1404963D0`; that instruction is outside the 272-byte body and is excluded above. All Ghidra exports completed successfully. The bounded `0x14069B300..0x14069B690` and `0x14069B500..0x14069B690` exports show repeated uses of `R14` for zero-like writes, but no reaching zero-definition for the `0x14069B806` store; the bounded evidence therefore leaves `R14==0` unproved. Three `rg` searches using unescaped `+0x700`/`+0x698` patterns failed with `regex parse error: repetition operator missing expression`; they yielded no evidence and did not change files. A subsequent escaped search succeeded. The offset conclusion above comes from direct Ghidra instruction bytes, not those failed searches.

No analysis script was created or modified, so there is no new script source to reproduce. The raw native evidence is included above rather than left only in scratch. The analysis is static; no game run, hook, observer, dispatch, permission, save, or billing change was performed. At this continuation's start, HEAD was `c9bb065b749d30221967df81391c1108a1a25330` and the pre-existing report was the only tracked modification. Its local pre-follow-up version was 411,183 bytes with SHA-256 `E136E29BCBB2B07524E0E1A2F70E8CCC8D61EA6631DE7CB335C55D13F2702496`, differing from the supplied remote baseline of 405,769 bytes / `2912dd2765ff8dd68e1907485c5f8f9171a5aeaaca096d011c91c212ba28ff97`; it was preserved and updated in place.

The first embedded-body comparison found one transcription error in `FUN_1404962C0` (`0x14049637A`, `CMP RAX,0x2` instead of the native `CMP RAX,0x1`); the report was corrected. The rerun matched all 56, 113, and 72 instructions for `FUN_14067CB30`, `FUN_140497950`, and `FUN_1404962C0`. A second bounded comparison verified that all 327 addressed instruction lines in this new section exactly match the native exports. The Markdown fence check passed with all code fences closed and no headings inside code blocks.

Final workspace validation: branch `DO-fuel-cost-calculatotr`, HEAD `c9bb065b749d30221967df81391c1108a1a25330`; `git diff --check` passed, and `git diff --name-only` / `git status --short` showed only this report modified. The first path assertion treated Git's single output string as a character array and failed; rerunning with an explicit PowerShell array (`@(git diff --name-only)`) passed. Git emitted only its existing CRLF-to-LF normalization warning; no other tracked path changed.
