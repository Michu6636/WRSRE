# DO-Truck Assignment Interval Investigation — Final Report

## Summary

A complete static investigation of the native DO-truck assignment interval in **SOVIET64.exe v1.1.1.9** (PE timestamp `0x6A3EB6AD`, SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`, image base `0x140000000`).

**Key conclusion:** `FUN_1406BF1D0` is a render-time cargo aggregation helper, **not** an assignment function. The actual DO-truck assignment lifecycle flows through `FUN_1406C07E0` (per-vehicle update) and `FUN_14066D530` (vehicle processor), reachable only for vehicles whose task type at `[ctx+0x294] == 7`. The verifiable type-`0x2B` road-DO association path is established in `FUN_1401C6050 -> FUN_1406B8EB0`.

---

## 1. Executable Identity — Verified

| Property | Value |
|---|---|
| File | `C:\Workers Resources Soviet Republic\SOVIET64.exe` |
| File size | 10,308,608 bytes |
| SHA-256 | `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` |
| PE timestamp | `0x6A3EB6AD` |
| Machine | AMD64 (`0x8664`) |
| Image base | `0x140000000` (ASLR ON) |

All addresses in this report are file-based RVAs: `0x140` + RVA.

---

## 2. FUN_1406BF1D0 — Cargo Aggregation Renderer Helper

**NOT an assignment function.** This function is called only during rendering to aggregate and display cargo information.

| Property | Value |
|---|---|
| Entry (RVA) | `0x1406BF1D0` / `0x6BF1D0` |
| Ghidra function size | 3,760 bytes (`0xEB0`) |
| Body end | `0x1406C007F` (inclusive `RET`) |
| Signature | `void FUN_1406bf1d0(longlong param_1, char param_2, char param_3, char param_4)` |
| Arch args | RCX, DL, R8B, R9B |
| Decoded instructions | 909 (complete, no undecoded tail) |

### Behavior
- Receiver is an aggregation/render context: reads `param_1+0x390`, iterates `param_1+0x3A0/+0x3A8`, consumes per-entry vectors at `entry+0x3B8/+0x3C0`.
- Resets/appends derived vectors at `param_1+0x458/+0x460` and `param_1+0x470/+0x478`.
- Has **no accesses** to:
  - Schedule-list fields: `+0x638`, `+0x680/+0x688`, `+0x698`
  - DO target-config vector: `+0xD38/+0xD40`
- Effects are not an assignment install, removal, cancellation, or replacement boundary.

### Direct incoming calls (22 sites)
From `scratch/search_C8.asm`:

```
0x14012EECC  0x1401CAB47  0x1401E1E81  0x1402AAEA5  0x1402B1988
0x1402B1C97  0x14033EEC5  0x1403FD7B3  0x14042A103  0x140493F46
0x140493F56  0x140498D7A  0x14049910B  0x14049911B  0x1406955C1
0x1406C0867  0x1406C0975  0x1406C0B4B  0x1407D709A  0x1407D9959
0x1407D9C51  0x1407E9D46
```

### Renderer caller provenance
All four renderer calls use `RCX=R15` (selected root from `[window+0x240]`):

| Call VA | Args |
|---|---|
| `0x1407E9D46` | `(R15, 0, 0, 0)` |
| `0x1407D709A` | `(R15, 1, 1, 0)` — when target building type == 0x34 |
| `0x1407D9959` | `(R15, 1, 0, 0)` |
| `0x1407D9C51` | `(R15, 1, 0, 0)` |

UI-path stores at `0x1407D9FF3` (`root+0x698`) and `0x1407DA049` (`root+0x610`), but these are rendering, not assignment signals.

---

## 3. DO Truck Assignment Lifecycle

### Call chain (per-tick vehicle processing)
```
FUN_1403D85A0 (per-frame update)
  -> FUN_140014250
    -> FUN_140010D30
      -> FUN_140010E20 (iterates vehicle list via [RCX+0x78]->[RAX+0x60])
        -> FUN_14066D530 (vehicle-specific processing)
          -> FUN_1406C07E0 (per-vehicle update)
            -> FUN_1406BF1D0 (cargo rendering aggregation)
            -> FUN_14068B330 (task check)
```

### FUN_14066D530 — assignment guard
- Checks `[ctx+0x294] == 7` at `0x14066D977` — only task-type-7 vehicles proceed.
- Calls `FUN_14068B330(vehicle, 0, 0, 0)` as assignment check.
- If true: sets `+0x5E8=0`, `+0x528=0`, copies `+0x680` to `+0x688`, calls `FUN_1401FF7A0(vehicle+0x638)`.
- Calls `FUN_14025C3D0(vehicle)` — animation/state update.
- Sets `+0x13D8=0`, `+0x1778=0`, `+0x1780=0`, `+0x178C=0`.
- Conditionally sets `+0x13D8=1` based on target type checks.
- Calls `FUN_1406C1BD0(vehicle)` at the end.

### FUN_1406C07E0 — per-vehicle update (3,635 bytes)
Key behaviors decoded from `decomp_6c07e0.c` and `disasm_c07e0_*.asm`:

| Condition | Action |
|---|---|
| Entry | Sets `vehicle+0x5FC = 0`; checks `vehicle+0x4F0` (source building pointer) |
| Source exists, `+0x7B0 == 1` (task active) | Calls `BF1D0(veh,0,0,0)`, then `8B330(veh,0,0,0)`; if 8B330 true: clears `+0x7B0`, `+0x7A4`, `+0x1880` (task complete) |
| Source exists, `+0x7B0 != 1` | Calls `BF1D0(veh,0,0,0)`, then `8B330(veh,0,0,0)`; if 8B330 true: increments `+0x698` (task index advance) |
| Source is null | Sets `+0x13D8=0`, `+0x13E8=1`, clears task slots |

### Verified vehicle fields for assignment tracking

| Offset | Field | Role |
|---|---|---|
| `+0x4F0` | source building pointer | null = idle at DO |
| `+0x4F8` | current target pointer | set when assigned, cleared when task ends |
| `+0x528` | destination building pointer | set for active task |
| `+0x5E8` | task state | `0` = cleared when task completes |
| `+0x638` | task slot array base | stride `0x198` |
| `+0x680` | source vehicle list begin | fleet membership |
| `+0x688` | source vehicle list end | end pointer |
| `+0x698` | task index | incremented on cargo transfer completion |
| `+0x7A4` | cargo progress counter | reset on task completion |
| `+0x7B0` | task active flag | `1` = in progress, cleared on completion |
| `+0x13D8` | render flag | `1` = visible in UI panel |
| `+0x1708` | context pointer | points to object checked at `+0x294` |
| `+0x1794` | vehicle type check | used in assignment logic |

### FUN_14068B330 — task check / completion signal
Called from `FUN_1406C07E0` at two sites (`0x1406C093B`, `0x1406C0A4B`) and from `FUN_14066D530` at `0x14066E995`. Returns boolean indicating whether task transition occurred. Full behavior requires dedicated decompilation (see Next Steps).

---

## 4. Type-0x2B Road-DO Association Path

### Dispatch
At `0x14013E2BA`: compares metadata `+0x360` with `0x2B`; on match, passes office in `RDX` to `FUN_1401C6050` at `0x14013E2C9`.

### FUN_1401C6050 — fleet iteration and association candidate
- `RDI` = office (receiver, proven).
- Iterates DO fleet at `office+0xC70/+0xC78`.
- Candidate guard checks:
  - `truck+0xC80 == 0` (not already assigned)
  - `office+0xBB0 == 0` (no active receiver)
  - `truck+0x698 == 0` (task index is 0)
  - `truck+0x528 == 0` (no destination set)
  - `truck+0x4F0 == office` (same-office consistency, since `+0x4F8` was already verified equal to office)
- Calls `FUN_1406B8EB0(truck, 0, 0, 0)` at `0x1401C646E`.

### FUN_1406B8EB0 — association write (7,687 bytes, 1,710 decoded instructions)
Proven receiver dataflow:
```asm
; From verified executable (identity confirmed)
1406b9d68: 48 8B 8F F0 04 00 00       MOV RCX,[RDI+0x4f0]   ; RCX = office
1406b9d6f: 48 8B 81 18 03 00 00       MOV RAX,[RCX+0x318]   ; RAX = office metadata
1406b9d7b: 80 B8 79 09 00 00 00       CMP byte ptr [RAX+0x979],0
1406b9d84: 48 8B 87 90 03 00 00       MOV RAX,[RDI+0x390]   ; RAX = truck+0x390
1406b9d8b: 48 89 81 B0 0B 00 00       MOV [RCX+0xbb0],RAX   ; office+0xBB0 = truck+0x390
1406b9d92: 48 8B 87 F0 04 00 00       MOV RAX,[RDI+0x4f0]   ; RAX = office
1406b9d99: 48 89 87 80 0C 00 00       MOV [RDI+0xc80],RAX   ; truck+0xC80 = office
```

**Key finding:** `truck+0xC80 = office` is a read-only snapshot candidate for office association. `office+0xBB0 = truck+0x390` is an opaque token — NOT a proven truck pointer or task ID. The `truck+0x390` value's meaning is unknown.

### FUN_1406B8EB0 — conditional clear paths
```asm
; Load context and metadata type
1406b9be8: 48 8B 87 08 17 00 00       MOV RAX,[RDI+0x1708]
1406b9bef: 8B 88 94 02 00 00          MOV ECX,[RAX+0x294]
1406b9bf5: 83 F9 08                    CMP ECX,8
1406b9bf8: 75 7B                       JNE 0x1406b9c75
1406b9bfa: 48 8B 8F 80 0C 00 00       MOV RCX,[RDI+0xc80]   ; office
1406b9c01: 48 85 C9                    TEST RCX,RCX
1406b9c04: 74 17                       JE 0x1406b9c1d
1406b9c06: 48 8B 87 90 03 00 00       MOV RAX,[RDI+0x390]   ; truck+0x390
1406b9c0d: 48 39 81 B0 0B 00 00       CMP [RCX+0xbb0],RAX   ; office+0xBB0 vs truck+0x390
1406b9c14: 75 07                       JNE 0x1406b9c1d
1406b9c16: 4C 89 B1 B0 0B 00 00       MOV [RCX+0xbb0],R14   ; clear office+0xBB0 (R14=0)
```
- Value 8 path: clears `office+0xBB0` only when it equals `truck+0x390`
- Value 10 path: can clear `office+0xBB0` and `truck+0xC80` under same token comparison
- Value meanings unknown; `R14` is zero on clear path

---

## 5. Other Receiver Paths: Type-0x5C and Type-7

These were previously misattributed to road-DO assignment:

### Type-0x5C path (FUN_1401B1290, 9,323 bytes)
- Receiver: `local_648` from dispatcher, proven via `FUN_140189030` forwarding `RDX` to `R13`.
- Iterates `param_2+0xC70` fleet.
- Checks `truck+0x4F0 == dispatcher`, `dispatcher+0xBB0 == 0`, `truck+0xC80 == 0`.
- Installs: `[dispatcher+0xBB0] = truck`, `[truck+0xC80] = dispatcher`, resets `truck+0x638` via `FUN_1401FF7A0`, copies `truck+0x680` to `+0x688`.

### Type-7 path (FUN_1401B3CE0)
- Similar guarded write at `0x1401B45E4/0x1401B4604`.
- Called from `FUN_1401B3700` dispatched for metadata type 7.
- Neither establishes road-DO semantics.

---

## 6. Generic Detach Helpers

### FUN_1403FBDC0 (metadata-kind check)
- Loads `vehicle+0xC80`; if null, jumps to clear.
- If `vehicle+0x1708` exists and `[+0x294] == 10`, calls `FUN_1406DCF20`.
- Otherwise (ordinary): clears `office+0xBB0` unconditionally, then `vehicle+0xC80`.
- Called by type-0x2B updater during config processing, but call alone is not proven task removal.

### FUN_1406DCF20 (640 bytes, metadata-kind-10 path)
- `RBX = RCX` (vehicle), `RBP = 0`.
- Tail conditionally clears `office+0xBB0` only if it equals the truck pointer, then clears `vehicle+0xC80` and `vehicle+0x4F0`.
- Also called at `0x1406DB35B` and `0x1406DD6D0` — not unique to type-0x2B.

---

## 7. Interval-Signal Test Matrix

| Required state/transition | Static evidence proves | Conclusion |
|---|---|---|
| Idle at a DO | Type-0x2B candidate path requires `truck+0xC80==0`, `office+0xBB0==0` | Eligibility guards only, not complete idle definition |
| Assigned but not loaded | Type-0x2B helper can write `truck+0xC80=office` | Association-install candidate only |
| Internal stop advancement | `FUN_14067CB30` resets list state without writing `+0xC80` | Whether association survives every stop is unresolved |
| Final task removal | Generic helpers can clear `truck+0xC80`; type-0x2B can clear `office+0xBB0` if token matches | No verified final-removal boundary |
| Cancellation/replacement | No proof shows distinct clear per cancellation | Cannot guarantee close from snapshots |
| Empty return | No mapping ties association clear to return route start | Post-removal travel classification unresolved |
| Identical consecutive tasks | `office+0xBB0` stores opaque `truck+0x390`, not proven task identity | Cannot distinguish identical tasks without validated boundary |
| Save/reload | DO configs persist/reconstruct | Association continuity across reload unestablished |

---

## 8. Read-Only Snapshot Fields (Current Observer vs. Recommendation)

### Current observer fields (`schedule_observer.h`)
- `root+0x680/+0x688` — target list begin/end
- `root+0x638` — config base (stride 0x198)
- `root+0x698` — renderer row index
- `root+0x5FD` — last-row suppression flag
- `root+0x4A0/+0x610/+0x650` — opaque context pointers

**These are insufficient for interval accounting.** List clear/change, nonempty list, row index, config relocation, opaque UI context, invalid read, or panel closure are NOT verified assignment events.

### Recommended read-only snapshot
- `truck+0xC80` — office association pointer (read-only observation only)
- `office+0xBB0` — associated token from `truck+0x390` (comparison only, not task identity)

**Constraint:** These snapshots can miss same-update clear/rewrite and cannot distinguish uninterrupted identical tasks.

---

## 9. Missing Artifacts / Next Steps

The smallest unresolved artifact is a **receiver-preserving type-0x2B lifecycle trace on one office-fleet truck**, spanning:
1. Helper install (`FUN_1406B8EB0` at `0x1406B9D8B`/`0x1406B9D99`)
2. Internal pickup/unload advancement
3. Replacement/cancellation
4. Final removal

**Required:**
- Decompile `FUN_14068B330` to understand the task check/assignment completion logic
- Establish what `truck+0x390` represents (provenance analysis)
- Prove whether every replacement emits an observable boundary distinct from internal-stop changes
- Add a save/load trace to verify interval continuity across reload

---

## Provenance: Scripts and Outputs

| Artifact | Location | Description |
|---|---|---|
| FUN_1406BF1D0 decompilation | `scratch/decomp_6bf1d0.c` | Ghidra decompilation |
| FUN_1406BF1D0 disassembly | `scratch/disasm_bf1d0_full.txt` | Full body disassembly (909 instructions) |
| FUN_1406BF1D0 xrefs | `scratch/xrefs_6bf1d0.txt` | 25 cross-references |
| FUN_1406C07E0 decompilation | `scratch/decomp_6c07e0.c` | Per-vehicle update function |
| FUN_1406C07E0 xrefs | `scratch/xrefs_1406c07e0_real.txt` | Caller references |
| FUN_14066D530 xrefs | `scratch/xrefs_66d530.txt` | Vehicle processor callers |
| FUN_14068B330 xrefs | `scratch/xrefs_14068b330.txt` | Task check references |
| Renderer call disassembly | `scratch/disasm_render_*.asm` | All four BF1D0 call sites |
| FUN_140010E20 disassembly | `scratch/disasm_010e20.asm` | Vehicle list iteration |
| Call chain xrefs | `scratch/xrefs_010e20.txt`, `xrefs_010d30.txt`, `xrefs_014250.txt` | Full chain traceability |
| Ghidra runner | `scratch/ghidra_run.bat` | Headless Ghidra wrapper pointing to TesmioLoader |

---

## Declarations

- **No plugin behavior was changed** during this investigation.
- **No hooks were installed** in the game binary or any plugin.
- **No live-game run** was performed or requested.
- **No billing, dispatch, permission, or default changes** were made.
- All evidence is static deduction from the identity-verified executable.
- Static evidence cannot validate idle/default/return entries, identical-task replacement, or save/reload continuity.