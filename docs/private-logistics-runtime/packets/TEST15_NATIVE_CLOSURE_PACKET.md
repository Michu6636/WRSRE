# TEST15 Native Closure Packet

Purpose: Single-file package of the Test15 bounded native evidence closure (Windows / read-only)
for delivery to the reviewing agent.
Repository root: `C:\WRSRE`
Source binary: SOVIET64.exe v1.1.1.9, image base 0x140000000, ASLR ON.
Analysis mode: Read-only static analysis via Ghidra headless oracle. No runtime edits, no build,
no deployment, no configuration changes, no game changes.

All byte counts and SHA-256 digests below were computed on the live workspace files at package time.
GHIDRA SERIALIZATION: one DisasmMany invocation was performed for the creator insertion region
(`scratch/out/test15_creator.asm`); all other artifacts are pre-existing caches reused as-is.

---

## Artifact Inventory

| # | Artifact | Source path | Bytes | SHA-256 |
|---|----------|-------------|-------|---------|
| 1 | Test15 claim ledger | `scratch/test15_claim_ledger.md` | 15737 | `2EA410E1FEE7E81D226082000B75D014A8F64505B134D8D5A7F31D8C2A464F26` |
| 2 | Ghidra index entry (updated) | `docs/ghidra-index/6EE2A0.md` | 4836 | `C38FFFDD793D1CFFB2E6E49B9259D6783E0F661B57C435A6F3ED929F11E69ECE` |
| 3 | Creator disassembly (fresh) | `scratch/out/test15_creator.asm` | 72356 | `8E5C9CF1BCB91C96B446766C9FE81E1CE1B63401A7CA88783C5411512224EFB8` |
| 4 | Ctor disassembly (cache) | `scratch/disasm_25c500.txt` | 12453 | `B15A9B9F82E4252E242318E39AA390C885F344057B0A72D207DFD8322A9C87B7` |
| 5 | Dispatcher decompilation (cache) | `scratch/decomp_6e9b80.c` | 67072 | `43F384885C890B0F59D7D81D0C7D3821C47DE50BEEA551D38B086AB800B896F3` |
| 6 | Rail panel decompilation (cache) | `scratch/panel_745350_final.c` | 79895 | `6DB7AD084C686263AB04653769947BA6756DC95C3E7FED2BA0CFDD042B12DEBE` |
| 7 | FUN_14001e140 corroboration excerpt | `scratch/agent1_install_connected_decomp.c` (lines 86-101) | 5439 | `5A8DD879472BC1E430C9E5CD5AE5ABC56090D4DC71BD1628C5F107ADBE9B09FD` |

---

## Complete Test15 Claim Ledger

### Part A — Receiver / Consumer Edge

#### A1. Creator allocation + ctor — SUPPORTED

| Claim | Raw artifact + address/line | Derivation | Status | Limit / missing edge |
|---|---|---|---|---|
| Allocation requests 0x5D8 bytes | `decomp_1406EE2A0.c:155` `pvVar14 = operator_new(0x5d8)` | Direct decompiler read | Supported (cached decompilation) | Constructor return/receiver linkage needed instruction-level check |
| Ctor `FUN_14025C500` called, returns window* | `decomp_1406EE2A0.c:156` `puVar15 = (undefined2 *)FUN_14025c500(pvVar14)` | Direct | Supported | — |
| Ctor zeroes `[window+0x4c0, window+0x590)` and `[window+0x580, window+0x598)` etc. | `disasm_25c500.txt:8-45` `MOV [RCX+0x4c0..0x598], RSI` | Instruction-level decode | Supported (asm cache) | — |
| **Creator** writes 100 DWORD values of 2 as QWORDs `0x200000002` in `[window+0x310, window+0x4A0)` | `test15_creator.asm:932-952` `MOV RDX,0x200000002`; loop stores `[RAX]`, `[RAX+8]`, `[RAX+0x10]`, steps `RAX += 0x40`, stores five more QWORDs per iteration | Byte arithmetic: 50 QWORDS × 8 = 400 bytes = 100 DWORDs of value 2 | Supported (instruction-level) | Physical slots ≠ native UI policy ≠ total target count |
| Next distinct field at byte `window+0x4A0` | `test15_creator.asm:952` `MOV dword ptr [RBX + 0x4a0],0x41200000` | Direct asm | Supported | — |
| 0x200000002 = two little-endian DWORDs both equal 2 | QWORD pattern; each DWORD lane = 2 | Typed byte arithmetic | Supported | — |

#### A2. Table insertion — SUPPORTED at instruction level

Fresh artifact: `scratch/out/test15_creator.asm` (DisasmMany VA `0x1406EE2A0:0x1406EF7E4`).

| Claim | Raw artifact + address | Derivation | Status | Limit |
|---|---|---|---|---|
| Insertion call `FUN_14001e140(RCX=&table, RDX=&out_slot, R8=insert_pos, stack=window)` | `test15_creator.asm:1140-1144` `LEA RAX,[RSP+0x30]`; `MOV [RSP+0x20],RAX`; `LEA RDX,[RSP+0x120]`; `MOV RCX,R14`; `CALL 0x14001e140` | Register/stack decode at call site | Supported (instruction-level) | R9 register value at CALL boundary not observable (see Corrections #1) |
| `R14 = &table(context+0xcac0)` | `decomp_1406EE2A0.c:103` `plVar1 = (longlong *)(param_1 + 0xcac0)`; asm uses R14 as table base | Correlate decompiler var to asm register | Supported | Register allocation is decompiler recovery |
| `local_res8 = param_1` (context); window passed on stack at [RSP+0x30] | `decomp_1406EE2A0.c:62` `local_res8 = param_1`; asm `MOV [RSP+0x30],RBX` where RBX=window puVar15 | Contradicts decompiler's `&local_res8` display; asm shows window pointer on stack | Supported | Decompiler displayed 3 args `(plVar1,&local_res8,lVar34)`; actual 4th stack arg is the window |
| Insert pos: type 8 (road) → R8 = table[0] (pos 0); type 9 (rail) → R8 = table[0]+8 (pos 1) | `test15_creator.asm:1106-1138` `CMP [RBX+0x34],0x8` → R8=[R14]; `CMP [RBX+0x34],0x9` → R8=[R14]+8 | Branch on `window+0x34` | Supported | Road=type 8, rail=type 9 (per dispatcher mapping below) |
| `FUN_14001e140` writes inserted value directly into table slot | `agent1_install_connected_decomp.c:91-96` `FUN_14001e140(plVar1,local_res8,*plVar1)` and `*plVar1 + param_3*8` | Corroborating callers pass local_res8 by value; table slot written directly | Supported (cross-caller corroboration) | Creator's own call uses stack slot for output |
| Post-insert loop writes `table[idx]+0x18 = 0` from insert index onward | `test15_creator.asm:1151-1161` `MOV [RCX+0x18],EDI` (EDI=0) over `iVar12` from insert index to table end | Direct asm | Supported | Zeroes a per-window field at +0x18, not the target-state array |

#### A3. Dispatcher — SUPPORTED

Artifact: `scratch/decomp_6e9b80.c` (`FUN_1406E9B80`, VA 0x1406E9B80).

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Iterates window table at `context+0xcac0`/`+0xcac8` | `decomp_6e9b80.c:187,200,223` `*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3` | Table count = (end-begin)/8 | Supported | — |
| Type `0x2b` (43) → road builder `FUN_140741170(context, window)` | `decomp_6e9b80.c:825-828` `else if (uVar1 == 0x2b) { ... FUN_140741170(param_1,puVar2); }` | Direct | Supported | puVar2 = window entry from table |
| Type `0x34` (52) → rail builder `FUN_140745350(context, window)` | `decomp_6e9b80.c:829-832` `else if (uVar1 == 0x34) { ... FUN_140745350(param_1,puVar2); }` | Direct | Supported | — |
| Both builders called as `(context, window)` | Lines 827, 831 | Two-arg call | Supported | — |

#### A4. Road builder (type 0x2b) — SUPPORTED

Artifact: `docs/private-logistics-runtime/RAW_EXPORTS_741170_1E440.md` (FUN_140741170).

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Reads/writes `window+0x310+idx*4` DWORD state | Lines 1697-1702 `uVar4 = *(uint *)(uVar44 + 0x310 + uVar43 * 4)`; `*(uint *)(uVar44 + 0x310 + uVar43 * 4) = (uint)(cVar16 != '\0')` | Direct decompiler read | Supported (cached export) | uVar44 = window (param_2) |
| Targets vector at `window+0x240` | Line 175 `lVar38 = *(longlong *)(param_2 + 0x240)` | Direct | Supported | — |
| Bulk propagation over total target count | Lines 1705-1713 loop `uVar41 < (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3)` | Count = (end-begin)/8 | Supported | — |
| Native `/49` UI policy | Line 1017 `L"%ls: %d / 49"` | Literal string | Supported | Display policy, not a hard cap on slots |
| Panel+building bound at lines 162/175 | Lines 162 `local_eb0 = param_2`; 175 `lVar38 = *(longlong *)(param_2 + 0x240)` | Established evidence | Supported | — |

#### A5. Rail builder (type 0x34) — SUPPORTED

Artifact: `scratch/panel_745350_final.c` (`FUN_140745350`, VA 0x140745350).

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Reads/writes `window+0x310+idx*4` DWORD state | Lines 1619-1624 `uVar38 = *(uint *)(lVar20 + 0x310 + uVar17 * 4)`; `*(uint *)(lVar20 + 0x310 + uVar17 * 4) = (uint)(cVar13 != '\0')` | Direct decompiler read | Supported (cached export) | lVar20 = window entry from targets vector |
| Targets vector at `window+0x240` | Line 172 `lVar35 = *(longlong *)(param_2 + 0x240)` | Direct | Supported | — |
| Bulk propagation over total target count | Lines 1628-1636 loop `uVar37 < (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3)` | Count = (end-begin)/8 | Supported | — |
| Native `/49` UI policy | Line 904 `L"%ls: %d / 49"` | Literal string | Supported | Same as road |
| Additional `/20` policy (separate mode) | Line 1803 `L"%ls: %d / 20"` | Literal string | Supported | Different code path (line 1799 else-branch), not the /49 admission path |

#### A6. Distinctions — SUPPORTED

| Concept | Value | Evidence | Road | Rail |
|---|---|---|---|---|
| Initialized physical slots | 100 DWORDs | Creator lines 580-596 | Same window layout | Same window layout |
| Initialized byte range | `[+0x310, +0x4A0)` = 400 bytes | Creator `undefined2* + 0x188` = 0x310; `+ 0x250` = 0x4A0 | Same | Same |
| Value written | QWORD `0x200000002` | Creator line 584 | Same | Same |
| Native UI policy | 49 | Road line 1017; Rail line 904 | /49 | /49 |
| Additional native policy | 20 | Rail line 1803 (else branch) | n/a | /20 separate mode |
| Per-selection batch limit | Not identified in cached evidence | — | Unresolved | Unresolved |
| Total target count | Targets vector `(end-begin)>>3` | Road line 1712; Rail line 1635 | Same shape | Same shape |

#### A7. Part A Conclusion

**Road: CLOSED.** Creator allocates 0x5D8, ctor FUN_14025C500 initializes zero fields, creator writes 100 DWORD target-state slots at window+0x310..+0x4A0, table insertion via FUN_14001e140 verified at instruction level (asm 1406EF6A2), dispatcher type 0x2b routes to road builder FUN_140741170 which consumes window+0x310+idx*4 and bulk-propagates over total target count, native /49 policy.

**Rail: CLOSED.** Same creator/ctor/insertion table; dispatcher type 0x34 routes to rail builder FUN_140745350 which consumes the identical window+0x310+idx*4 state and bulk-propagates over total target count; native /49 policy (plus separate /20 mode).

**Road-only bounded fix retaining rail's conservative policy: Supported.** Road and rail share the same creator/ctor/insertion/table infrastructure and the same window+0x310+idx*4 consumer layout. A road-only change to the automatic-append ceiling does not require touching rail; rail retains its existing /49 (and /20) display policy. No cross-receiver coupling is evidenced.

### Part B — Commodity Admission: Ratio Denominators

#### B1. Predicate — SUPPORTED (cache)

Artifact: `scratch/decomp_1de2b0.c` / `scratch/agent1_func_1401de2b0.c` (`FUN_1401de2b0`, VA 0x1401DE2B0, 128 lines).

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Rejects `row+0x60 <= 0` | Line 43 `if ((double)param_2[0xc] <= 0.0) return 0;` | param_2[0xc] = row+0x60 | Supported | — |
| Rejects `row+0x28 <= source threshold` | Line 46-49 `dVar13 = (double)param_2[5]; if (dVar13 <= dVar12) return 0;` where dVar12 = float at `param_4+0xd38` vector[idx=param_2[3]] + 0x14, +4 | param_2[5] = row+0x28 | Supported | dVar12 derived from TaskCfg sub-vector indexed by row+0x18, not row-internal |
| Ordinary-office guard | Lines 50-52 `if ((!bVar3) && (dVar13 <= DAT_14090a048)) return 0;` | Additional guard | Supported | — |
| Rejects `row+0x60 < (DAT_14090a1f0 - unload)` | Lines 53-55 `if ((double)param_2[0xc] < dVar2) return 0;` where dVar2 = DAT_14090a1f0 - float at `param_4+0xd38` vector[idx=param_2[10]] + 0xdc | param_2[0xc] = row+0x60 | Supported | Matches established evidence (constant at 0x14090A1F0) |
| TaskCfg+0x04 and TaskCfg+0xCC referenced | Lines 27-35 via `param_4 + 0xd38` vector entries read at +0x14/+0x10+0xcc | param_4 is office context; +0xd38 is a sub-vector | Supported per established evidence | Exact TaskCfg field mapping not independently re-verified |

#### B2. Consumer — SUPPORTED (cache)

Artifact: `scratch/decomp_1e5480.c` (`FUN_1401e5480`, VA 0x1401E5480, 267 lines). Cumulative-report lines 74500 onward reuse confirmed present.

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Iterates office row vector at `param_3+0xd58`/`+0xd60` (0x80-byte rows) | Lines 38, 52 `plVar1 = (longlong *)(param_3 + 0xd58)`; `*(longlong *)(param_3 + 0xd60) - *plVar1 >> 7` | Count = size/128 | Supported | — |
| Calls `FUN_1401de2b0` per row | Line 55 `uVar14 = FUN_1401de2b0(param_1, uVar14 * 0x80 + *plVar1, param_2, param_3)` | Row stride 0x80 | Supported | — |
| On pass, computes ratio `(row+0x70 - row+0x68 - DAT_14090a008) / dVar22` | Lines 134-135 `*pfVar18 = (float)((((double)puVar20[0xe] - (double)puVar20[0xd]) - DAT_14090a008) / dVar22);` | puVar20[0xe]=row+0x70, puVar20[0xd]=row+0x68; dVar22 = FUN_1406bddb0(...) | Supported | This is the post-admission ratio, separate from the predicate's row+0x28/+0x60 gating |
| Denominator `dVar22 = FUN_1406bddb0(param_2, *puVar20, 0, 0)` | Line 125 `fVar21 = (float)FUN_1406bddb0(param_2,*puVar20,0,0)` | Denominator function | Supported | FUN_1406bddb0 semantics not traced |

#### B3. Rebuilder — PARTIAL

Artifact: `scratch/rebuild_1E2380.c` (`FUN_1401e2380`, VA 0x1401E2380, 1001 lines).

| Claim | Raw artifact + address/line | Derivation | Status | Limit |
|---|---|---|---|---|
| Rebuilds office row vector at `param_2+0xd58` | Line 144 `*(undefined8 *)(param_2 + 0xd60) = *(undefined8 *)(param_2 + 0xd58)`; line 938 `FUN_140200280(param_2 + 0xd58, &local_128)` | Vector push | Supported | — |
| Reads building storage records at `building+0x970`, stride 0xE0 | Lines 354-355 `plVar19 = (longlong *)(*(longlong *)(local_200 + 0x970) + uVar10 * 0xe0)` | plVar19 = storage record | Supported | plVar19 is storage record, NOT TaskCfg |
| plVar19[0x11] = commodity type (compared to 5, 7) | Lines 360-361 `(int)plVar19[0x11] - 5U` / `- 7U` | Direct | Supported | Commodity selection for admission |
| plVar19[0x12] = storage type (0x11) | Lines 355, 366 `(int)plVar19[0x12] == 0x11` | Direct | Supported | Storage-type filter |
| Stock read as `float` at `storage_entry+8` | Lines 371 `if (fVar6 <= *(float *)(lVar13 + 8 + uVar10 * 0x10))` | Storage entry + 8 | Supported | Stock amount source |
| Pushes entry via `FUN_140200280(param_2+0xd58, &local_128)` | Line 938 | Vector push | Supported | local_128 is a single longlong; 128-byte entry layout not decompiler-resolved |

#### B4. UNRESOLVED — Writers of row+0x28 / row+0x60 and ratio denominators

| Missing edge | Why unresolved | Smallest proposed follow-up |
|---|---|---|
| Exact writer of `row+0x28` (office row offset 0x28) | rebuild_1E2380 decompiler splits 8-byte fields into low/high stack parts; stack-to-offset mapping not determinable from decompiled output | Decompile `FUN_140200280` (VA 0x140200280) or instruction-level trace of FUN_1401e2380 stores to +0x28/+0x60 |
| Exact writer of `row+0x60` (office row offset 0x60) | Same as above | Same follow-up |
| Ratio denominators for admission | Predicate compares row+0x28 against dVar12 (derived from TaskCfg sub-vector indexed by row+0x18) and row+0x60 against DAT_14090a1f0 - unload; no field identified as a pure denominator | Same follow-up |

**DO_PLANNER_RAW_EVIDENCE.md warning heeded:** Lines 300-331 plVar19 is `building+0x970` storage record (stride 0xE0), NOT TaskCfg. That separate planner branch is NOT used to assert the DO threshold ratio.

**Test15 crops80/880≈9.09% heuristic:** Not adopted. It is only a conditional source10 explanation; Electronics stock was cropped out; Electro comp. is distinct. Defaults 10/30 unchanged; no 30→100 suggestion based on aggregate fullness.

### Part C — Overall Verdict

| Subsystem | Native capacity-safe append ready? | Basis |
|---|---|---|
| Road | YES | Receiver/consumer closed: ctor initializes zero fields; creator writes 100 slots at window+0x310..+0x4A0; insertion verified; dispatcher type 0x2b → road builder consumes identical state; native /49 policy |
| Rail | YES | Same creator/ctor/insertion; dispatcher type 0x34 → rail builder consumes identical state; native /49 policy (+ separate /20 mode) |
| Commodity admission (ratio denominators) | NO — blocked | Writers of row+0x28/+0x60 and denominators unresolved (see B4) |

**Minimal remaining blocker:** Part B only — identify exact writers of office row+0x28 and row+0x60 and the ratio denominators. Requires either decompilation of `FUN_140200280` (VA 0x140200280) or instruction-level trace of `FUN_1401e2380` stores to those offsets. Part A is fully closed for both road and rail; no blocker there.

**No runtime edits performed. No build, deployment, or configuration changes. New output kept in `scratch/`.**

---

## Corrections / Limitations

1. **Insertion argument R9 omitted.** The ledger's A2 table describes the `FUN_14001e140` call at
   `0x1406EF6A2` as `(RCX=&table, RDX=&out_slot, R8=insert_pos, stack=[RSP+0x30]=window)`. Under
   Windows x64 calling convention, `[RSP+0x20]` immediately before the `CALL` is the fifth
   argument slot (R9's home location). The disassembly shows:

   ```
   1406ef68d:  48 8D 44 24 30           LEA RAX,[RSP + 0x30]
   1406ef692:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
   1406ef697:  48 8D 94 24 20 01 00 00  LEA RDX,[RSP + 0x120]
   1406ef69f:  49 8B CE                 MOV RCX,R14
   1406ef6a2:  E8 99 EA 92 FF           CALL 0x14001e140
   ```

   Raw evidence confirms RCX=R14 (&table), RDX=[RSP+0x120] (&out_slot), R8=insert_pos, and stack slot
   [RSP+0x20]=&RSP[0x30] (the window pointer). R9 is **not** materialized as a register in the
   visible instruction stream; its home slot is the stack at [RSP+0x20]. Whether R9 was loaded into a
   register before the CALL is not shown in the exported range. Marked **unresolved**: R9 register
   value at the CALL boundary is not observable in this export.

2. **`0x200000002` is a QWORD pattern containing two DWORD values of 2.** The ledger A1 table
   states "100 DWORD values of `0x200000002`"; corrected: each QWORD store writes `0x200000002`,
   which is two little-endian DWORDs both equal to 2. The initialization loop at
   `scratch/out/test15_creator.asm:932-946` stores `RDX=0x200000002` as QWORDs at `[RAX]`,
   `[RAX+8]`, `[RAX+0x10]`, then steps `RAX += 0x40` and stores five more QWORDs per iteration
   (`RAX-0x28` through `RAX-0x8`), for `(6*8+2)*8 = 400` bytes = 50 QWORDS = 100 DWORDs of value 2.

3. **Creator initialization vs. `FUN_14025C500` initialization distinguished.** The creator
   (`FUN_1406EE2A0`) performs its own field stores (e.g. `window+0x240=RBP`, `window+0x248`,
   `window+0x24c`, `window+0x34`, `window+0x18=0`, etc.) in addition to calling the ctor. The ctor
   `FUN_14025C500` zeroes `[window+0x4c0..0x598]`, `[window+0x580..0x590]`, `[window+0x598]`, and
   several other fields, then returns `RAX=RBX=window*`. The 100-DWORD target-state array at
   `[window+0x310..0x4A0)` is written by the **creator** directly (asm `0x1406ef305-0x1406ef352`),
   not by the ctor. Prior narrative conflated the two; corrected here.

---

## Ghidra Index Entry — docs/ghidra-index/6EE2A0.md (updated)

Source path: `docs/ghidra-index/6EE2A0.md` | Bytes: 4836 | SHA-256: `C38FFFDD793D1CFFB2E6E49B9259D6783E0F661B57C435A6F3ED929F11E69ECE`

**Artifact:** root `decomp_1406EE2A0.c` and `xrefs_1406EE2A0.txt`; fresh `scratch/out/test15_creator.asm` (DisasmMany VA `0x1406EE2A0:0x1406EF7E4`, insertion call at `0x1406EF6A2`); DO consumer in `docs/private-logistics-runtime/RAW_EXPORTS_741170_1E440.md`; rail consumer in `scratch/panel_745350_final.c`; dispatcher in `scratch/decomp_6e9b80.c`.
**Status:** `partial` — fresh Ghidra DisasmMany execution for insertion region; claim table updated below. No local PE authentication (verified bytes do not verify receiver semantics/units/runtime applicability).

| This initialized allocation is the receiver of every road/rail target-state consumer | Dispatcher type 0x2b→`FUN_140741170` (road) and type 0x34→`FUN_140745350` (rail), both called `(context,window)`; both consume `window+0x310+idx*4` and bulk-propagate over total target count; insertion `FUN_14001e140` verified at asm `0x1406EF6A2` (`scratch/out/test15_creator.asm`) | Resolved — road and rail both closed at receiver/consumer edge; native `/49` policy on both (road export line 1017, rail `panel_745350_final.c` line 904); rail has separate `/20` mode at line 1803 |
| Creator constructor return/receiver linkage instruction-checked | `disasm_25c500.txt` (VA `0x14025C500`): zeroes `[window+0x4c0..0x598]`, returns RAX=RBX=window* | Supported |
| Creator initializes 100 DWORD values of `0x200000002` in `[window+0x310, window+0x4A0)` | Lines 580–596: `undefined2* + 0x188` = byte 0x310; six iterations of eight QWORD stores + two more = `(6*8+2)*8=400` bytes; next field `undefined2* + 0x250` = byte 0x4A0 | Supported arithmetic; physical slots ≠ native UI policy ≠ total target count |
| Table insertion actual args | Asm `0x1406EF6A2`: `FUN_14001e140(RCX=&table(context+0xcac0), RDX=&out_slot([RSP+0x120]), R8=insert_pos, stack=[RSP+0x30]=window)`; insert pos = table[0] for type 8 (road), table[0]+8 for type 9 (rail) | Supported (instruction-level); `FUN_14001e140` signature corroborated by `agent1_install_connected_decomp.c:91-96` |
| Post-insert zeroing of `table[idx]+0x18` | Asm `0x1406EF6C2-0x1406EF6E1`: `MOV [RCX+0x18],EDI` (EDI=0) from insert index to table end | Supported |
| 100 initialized slots → automatic-target ceiling NOT implied | Creator lines 580-596; bulk propagation over target count not slot count | Limit: physical slots (100) ≠ native UI policy (49) ≠ total target count; per-selection batch limit not identified |

---

## Raw Artifact: scratch/out/test15_creator.asm

Source path: `scratch/out/test15_creator.asm` | Bytes: 72356 | SHA-256: `8E5C9CF1BCB91C96B446766C9FE81E1CE1B63401A7CA88783C5411512224EFB8`
Lines: 1211 | DisasmMany VA range: `0x1406EE2A0:0x1406EF7E4` (creator insertion region)

```asm
; Range: 0x1406EE2A0 to 0x1406EF7E4
1406ee2a0:  48 8B C4                 MOV RAX,RSP
1406ee2a3:  44 89 48 20              MOV dword ptr [RAX + 0x20],R9D
1406ee2a7:  89 50 10                 MOV dword ptr [RAX + 0x10],EDX
1406ee2aa:  48 89 48 08              MOV qword ptr [RAX + 0x8],RCX
1406ee2ae:  55                       PUSH RBP
1406ee2af:  56                       PUSH RSI
1406ee2b0:  57                       PUSH RDI
1406ee2b1:  41 54                    PUSH R12
1406ee2b3:  41 55                    PUSH R13
1406ee2b5:  41 56                    PUSH R14
1406ee2b7:  41 57                    PUSH R15
1406ee2b9:  48 81 EC E0 00 00 00     SUB RSP,0xe0
1406ee2c0:  48 C7 44 24 40 FE FF FF FF MOV qword ptr [RSP + 0x40],-0x2
1406ee2c9:  48 89 58 18              MOV qword ptr [RAX + 0x18],RBX
1406ee2cd:  0F 29 70 B8              MOVAPS xmmword ptr [RAX + -0x48],XMM6
1406ee2d1:  0F 29 78 A8              MOVAPS xmmword ptr [RAX + -0x58],XMM7
1406ee2d5:  44 0F 29 40 98           MOVAPS xmmword ptr [RAX + -0x68],XMM8
1406ee2da:  44 0F 29 48 88           MOVAPS xmmword ptr [RAX + -0x78],XMM9
1406ee2df:  44 0F 29 90 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM10
1406ee2e7:  44 0F 29 98 68 FF FF FF  MOVAPS xmmword ptr [RAX + -0x98],XMM11
1406ee2ef:  44 0F 29 64 24 70        MOVAPS xmmword ptr [RSP + 0x70],XMM12
1406ee2f5:  41 8B C1                 MOV EAX,R9D
1406ee2f8:  49 8B E8                 MOV RBP,R8
1406ee2fb:  44 8B E2                 MOV R12D,EDX
1406ee2fe:  4C 8B E9                 MOV R13,RCX
1406ee301:  48 8B 99 50 C6 00 00     MOV RBX,qword ptr [RCX + 0xc650]
1406ee308:  33 FF                    XOR EDI,EDI
1406ee30a:  48 85 DB                 TEST RBX,RBX
1406ee30d:  74 5E                    JZ 0x1406ee36d
1406ee30f:  85 C0                    TEST EAX,EAX
1406ee311:  0F 85 06 01 00 00        JNZ 0x1406ee41d
1406ee317:  4D 85 C0                 TEST R8,R8
1406ee31a:  74 59                    JZ 0x1406ee375
1406ee31c:  49 8B 80 18 03 00 00     MOV RAX,qword ptr [R8 + 0x318]
1406ee323:  48 85 C0                 TEST RAX,RAX
1406ee326:  74 4D                    JZ 0x1406ee375
1406ee328:  83 B8 60 03 00 00 27     CMP dword ptr [RAX + 0x360],0x27
1406ee32f:  75 44                    JNZ 0x1406ee375
1406ee331:  48 8D 15 50 8D 20 00     LEA RDX,[0x1408f7088]
1406ee338:  48 8D 4B 01              LEA RCX,[RBX + 0x1]
1406ee33c:  E8 DF 11 16 00           CALL 0x14084f520
1406ee341:  85 C0                    TEST EAX,EAX
1406ee343:  74 1F                    JZ 0x1406ee364
1406ee345:  8B CF                    MOV ECX,EDI
1406ee347:  48 8D 15 46 8D 20 00     LEA RDX,[0x1408f7094]
1406ee34e:  66 90                    NOP
1406ee350:  0F B6 44 0B 01           MOVZX EAX,byte ptr [RBX + RCX*0x1 + 0x1]
1406ee355:  48 FF C1                 INC RCX
1406ee358:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
1406ee35c:  75 17                    JNZ 0x1406ee375
1406ee35e:  48 83 F9 06              CMP RCX,0x6
1406ee362:  75 EC                    JNZ 0x1406ee350
1406ee364:  49 89 BD 50 C6 00 00     MOV qword ptr [R13 + 0xc650],RDI
1406ee36b:  EB 08                    JMP 0x1406ee375
1406ee36d:  85 C0                    TEST EAX,EAX
1406ee36f:  0F 85 A8 00 00 00        JNZ 0x1406ee41d
1406ee375:  41 83 FC 04              CMP R12D,0x4
1406ee379:  0F 85 9E 00 00 00        JNZ 0x1406ee41d
1406ee37f:  48 85 ED                 TEST RBP,RBP
1406ee382:  0F 84 95 00 00 00        JZ 0x1406ee41d
1406ee388:  48 8B 85 18 03 00 00     MOV RAX,qword ptr [RBP + 0x318]
1406ee38f:  48 85 C0                 TEST RAX,RAX
1406ee392:  0F 84 85 00 00 00        JZ 0x1406ee41d
1406ee398:  83 B8 60 03 00 00 0F     CMP dword ptr [RAX + 0x360],0xf
1406ee39f:  75 7C                    JNZ 0x1406ee41d
1406ee3a1:  44 8B C7                 MOV R8D,EDI
1406ee3a4:  49 8B 85 C8 CA 00 00     MOV RAX,qword ptr [R13 + 0xcac8]
1406ee3ab:  49 2B 85 C0 CA 00 00     SUB RAX,qword ptr [R13 + 0xcac0]
1406ee3b2:  48 C1 F8 03              SAR RAX,0x3
1406ee3b6:  48 85 C0                 TEST RAX,RAX
1406ee3b9:  74 62                    JZ 0x1406ee41d
1406ee3bb:  48 8B CF                 MOV RCX,RDI
1406ee3be:  66 90                    NOP
1406ee3c0:  49 8B 85 C0 CA 00 00     MOV RAX,qword ptr [R13 + 0xcac0]
1406ee3c7:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
1406ee3cb:  39 B9 48 02 00 00        CMP dword ptr [RCX + 0x248],EDI
1406ee3d1:  75 2D                    JNZ 0x1406ee400
1406ee3d3:  83 B9 4C 02 00 00 04     CMP dword ptr [RCX + 0x24c],0x4
1406ee3da:  75 24                    JNZ 0x1406ee400
1406ee3dc:  48 8B 81 40 02 00 00     MOV RAX,qword ptr [RCX + 0x240]
1406ee3e3:  48 3B C5                 CMP RAX,RBP
1406ee3e6:  74 18                    JZ 0x1406ee400
1406ee3e8:  48 8B 90 18 03 00 00     MOV RDX,qword ptr [RAX + 0x318]
1406ee3ef:  48 85 D2                 TEST RDX,RDX
1406ee3f2:  74 0C                    JZ 0x1406ee400
1406ee3f4:  83 BA 60 03 00 00 0F     CMP dword ptr [RDX + 0x360],0xf
1406ee3fb:  75 03                    JNZ 0x1406ee400
1406ee3fd:  C6 01 01                 MOV byte ptr [RCX],0x1
1406ee400:  41 FF C0                 INC R8D
1406ee403:  49 8B 85 C8 CA 00 00     MOV RAX,qword ptr [R13 + 0xcac8]
1406ee40a:  49 2B 85 C0 CA 00 00     SUB RAX,qword ptr [R13 + 0xcac0]
1406ee411:  48 C1 F8 03              SAR RAX,0x3
1406ee415:  41 8B C8                 MOV ECX,R8D
1406ee418:  48 3B C8                 CMP RCX,RAX
1406ee41b:  72 A3                    JC 0x1406ee3c0
1406ee41d:  32 D2                    XOR DL,DL
1406ee41f:  4D 8D B5 C0 CA 00 00     LEA R14,[R13 + 0xcac0]
1406ee426:  4C 89 74 24 38           MOV qword ptr [RSP + 0x38],R14
1406ee42b:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ee42f:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ee432:  48 C1 F8 03              SAR RAX,0x3
1406ee436:  44 8B BC 24 50 01 00 00  MOV R15D,dword ptr [RSP + 0x150]
1406ee43e:  8B B4 24 40 01 00 00     MOV ESI,dword ptr [RSP + 0x140]
1406ee445:  48 85 C0                 TEST RAX,RAX
1406ee448:  0F 84 38 01 00 00        JZ 0x1406ee586
1406ee44e:  33 C9                    XOR ECX,ECX
1406ee450:  41 B8 E6 3E 00 00        MOV R8D,0x3ee6
1406ee456:  44 8B A4 24 38 01 00 00  MOV R12D,dword ptr [RSP + 0x138]
1406ee45e:  66 90                    NOP
1406ee460:  49 8B 06                 MOV RAX,qword ptr [R14]
1406ee463:  48 8B 1C C8              MOV RBX,qword ptr [RAX + RCX*0x8]
1406ee467:  48 39 AB 40 02 00 00     CMP qword ptr [RBX + 0x240],RBP
1406ee46e:  74 0E                    JZ 0x1406ee47e
1406ee470:  8B 8B 3C 02 00 00        MOV ECX,dword ptr [RBX + 0x23c]
1406ee476:  8D 41 F4                 LEA EAX,[RCX + -0xc]
1406ee479:  83 F8 01                 CMP EAX,0x1
1406ee47c:  77 61                    JA 0x1406ee4df
1406ee47e:  8B 8B 3C 02 00 00        MOV ECX,dword ptr [RBX + 0x23c]
1406ee484:  3B CE                    CMP ECX,ESI
1406ee486:  75 57                    JNZ 0x1406ee4df
1406ee488:  8B 84 24 48 01 00 00     MOV EAX,dword ptr [RSP + 0x148]
1406ee48f:  39 43 34                 CMP dword ptr [RBX + 0x34],EAX
1406ee492:  75 4B                    JNZ 0x1406ee4df
1406ee494:  44 39 7B 38              CMP dword ptr [RBX + 0x38],R15D
1406ee498:  75 45                    JNZ 0x1406ee4df
1406ee49a:  41 83 FF 28              CMP R15D,0x28
1406ee49e:  74 3F                    JZ 0x1406ee4df
1406ee4a0:  C6 43 01 00              MOV byte ptr [RBX + 0x1],0x0
1406ee4a4:  C7 43 14 6F 12 83 3A     MOV dword ptr [RBX + 0x14],0x3a83126f
1406ee4ab:  8B D7                    MOV EDX,EDI
1406ee4ad:  49 8B CD                 MOV RCX,R13
1406ee4b0:  E8 6B EC FF FF           CALL 0x1406ed120
1406ee4b5:  8B 8B 3C 02 00 00        MOV ECX,dword ptr [RBX + 0x23c]
1406ee4bb:  8D 41 F5                 LEA EAX,[RCX + -0xb]
1406ee4be:  83 F8 02                 CMP EAX,0x2
1406ee4c1:  77 14                    JA 0x1406ee4d7
1406ee4c3:  C7 83 64 02 00 00 FF FF FF FF MOV dword ptr [RBX + 0x264],0xffffffff
1406ee4cd:  C7 83 68 02 00 00 00 00 00 00 MOV dword ptr [RBX + 0x268],0x0
1406ee4d7:  41 B8 E6 3E 00 00        MOV R8D,0x3ee6
1406ee4dd:  EB 1A                    JMP 0x1406ee4f9
1406ee4df:  84 D2                    TEST DL,DL
1406ee4e1:  75 18                    JNZ 0x1406ee4fb
1406ee4e3:  41 83 FC 07              CMP R12D,0x7
1406ee4e7:  75 12                    JNZ 0x1406ee4fb
1406ee4e9:  44 39 A3 48 02 00 00     CMP dword ptr [RBX + 0x248],R12D
1406ee4f0:  75 09                    JNZ 0x1406ee4fb
1406ee4f2:  48 89 AB 40 02 00 00     MOV qword ptr [RBX + 0x240],RBP
1406ee4f9:  B2 01                    MOV DL,0x1
1406ee4fb:  83 FE 0D                 CMP ESI,0xd
1406ee4fe:  77 37                    JA 0x1406ee537
1406ee500:  41 0F A3 F0              BT R8D,ESI
1406ee504:  73 31                    JNC 0x1406ee537
1406ee506:  3B CE                    CMP ECX,ESI
1406ee508:  75 14                    JNZ 0x1406ee51e
1406ee50a:  8D 46 F5                 LEA EAX,[RSI + -0xb]
1406ee50d:  83 F8 02                 CMP EAX,0x2
1406ee510:  77 0C                    JA 0x1406ee51e
1406ee512:  C7 83 64 02 00 00 FE FF FF FF MOV dword ptr [RBX + 0x264],0xfffffffe
1406ee51c:  EB 19                    JMP 0x1406ee537
1406ee51e:  83 F9 0D                 CMP ECX,0xd
1406ee521:  77 14                    JA 0x1406ee537
1406ee523:  41 0F A3 C8              BT R8D,ECX
1406ee527:  73 0E                    JNC 0x1406ee537
1406ee529:  C6 03 01                 MOV byte ptr [RBX],0x1
1406ee52c:  41 C7 85 C8 0F 00 00 00 00 00 BF MOV dword ptr [R13 + 0xfc8],0xbf000000
1406ee537:  8D 46 FD                 LEA EAX,[RSI + -0x3]
1406ee53a:  83 F8 01                 CMP EAX,0x1
1406ee53d:  77 1C                    JA 0x1406ee55b
1406ee53f:  8B 83 3C 02 00 00        MOV EAX,dword ptr [RBX + 0x23c]
1406ee545:  83 E8 03                 SUB EAX,0x3
1406ee548:  83 F8 01                 CMP EAX,0x1
1406ee54b:  77 0E                    JA 0x1406ee55b
1406ee54d:  C6 03 01                 MOV byte ptr [RBX],0x1
1406ee550:  41 C7 85 C8 0F 00 00 00 00 00 BF MOV dword ptr [R13 + 0xfc8],0xbf000000
1406ee55b:  FF C7                    INC EDI
1406ee55d:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ee561:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ee564:  48 C1 F8 03              SAR RAX,0x3
1406ee568:  8B CF                    MOV ECX,EDI
1406ee56a:  48 3B C8                 CMP RCX,RAX
1406ee56d:  0F 82 ED FE FF FF        JC 0x1406ee460
1406ee573:  84 D2                    TEST DL,DL
1406ee575:  44 8B A4 24 28 01 00 00  MOV R12D,dword ptr [RSP + 0x128]
1406ee57d:  74 07                    JZ 0x1406ee586
1406ee57f:  33 C0                    XOR EAX,EAX
1406ee581:  E9 2F 12 00 00           JMP 0x1406ef7b5
1406ee586:  41 83 FC 01              CMP R12D,0x1
1406ee58a:  77 57                    JA 0x1406ee5e3
1406ee58c:  41 80 7D 14 00           CMP byte ptr [R13 + 0x14],0x0
1406ee591:  75 50                    JNZ 0x1406ee5e3
1406ee593:  48 8D 15 8E 8A 20 00     LEA RDX,[0x1408f7028]
1406ee59a:  49 8D 8D 10 FD 00 00     LEA RCX,[R13 + 0xfd10]
1406ee5a1:  FF 15 69 E7 17 00        CALL qword ptr [0x14086cd10]
1406ee5a7:  84 C0                    TEST AL,AL
1406ee5a9:  74 38                    JZ 0x1406ee5e3
1406ee5ab:  48 8D 15 76 8A 20 00     LEA RDX,[0x1408f7028]
1406ee5b2:  49 8D 8D 10 FD 00 00     LEA RCX,[R13 + 0xfd10]
1406ee5b9:  FF 15 59 E7 17 00        CALL qword ptr [0x14086cd18]
1406ee5bf:  48 8B D0                 MOV RDX,RAX
```

```asm
; test15_creator.asm lines 201-400 (partial excerpt)
1406ee5c2:  48 C7 44 24 20 00 00 00 00 MOV qword ptr [RSP + 0x20],0x0
1406ee5cb:  F3 0F 10 1D 85 B9 21 00  MOVSS XMM3,dword ptr [0x140909f58]
1406ee5d3:  41 B0 01                 MOV R8B,0x1
1406ee5d6:  49 8B 8D 08 FD 00 00     MOV RCX,qword ptr [R13 + 0xfd08]
1406ee5dd:  FF 15 C5 EC 17 00        CALL qword ptr [0x14086d2a8]
1406ee5e3:  B9 D8 05 00 00           MOV ECX,0x5d8
1406ee5e8:  E8 DF F8 15 00           CALL 0x14084decc
1406ee5ed:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1406ee5f2:  48 8B C8                 MOV RCX,RAX
1406ee5f5:  E8 06 DF B6 FF           CALL 0x14025c500
1406ee5fa:  48 8B D8                 MOV RBX,RAX
1406ee5fd:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1406ee602:  33 D2                    XOR EDX,EDX
1406ee604:  89 90 C0 05 00 00        MOV dword ptr [RAX + 0x5c0],EDX
1406ee60a:  88 90 C4 05 00 00        MOV byte ptr [RAX + 0x5c4],DL
1406ee610:  48 89 90 24 05 00 00     MOV qword ptr [RAX + 0x524],RDX
1406ee617:  48 89 A8 40 02 00 00     MOV qword ptr [RAX + 0x240],RBP
1406ee61e:  8B 84 24 38 01 00 00     MOV EAX,dword ptr [RSP + 0x138]
1406ee625:  89 83 48 02 00 00        MOV dword ptr [RBX + 0x248],EAX
1406ee62b:  44 89 A3 4C 02 00 00     MOV dword ptr [RBX + 0x24c],R12D
1406ee632:  89 93 58 02 00 00        MOV dword ptr [RBX + 0x258],EDX
1406ee638:  49 83 C8 FF              OR R8,-0x1
1406ee63c:  44 89 83 5C 02 00 00     MOV dword ptr [RBX + 0x25c],R8D
1406ee643:  44 8B A4 24 48 01 00 00  MOV R12D,dword ptr [RSP + 0x148]
1406ee64b:  44 89 63 34              MOV dword ptr [RBX + 0x34],R12D
1406ee64f:  44 89 7B 38              MOV dword ptr [RBX + 0x38],R15D
1406ee653:  66 89 53 3C              MOV word ptr [RBX + 0x3c],DX
1406ee657:  48 8B BC 24 58 01 00 00  MOV RDI,qword ptr [RSP + 0x158]
1406ee65f:  48 85 FF                 TEST RDI,RDI
1406ee662:  74 6D                    JZ 0x1406ee6d1
1406ee664:  41 83 FC 23              CMP R12D,0x23
1406ee668:  75 4E                    JNZ 0x1406ee6b8
1406ee66a:  49 8B C8                 MOV RCX,R8
1406ee66d:  0F 1F 00                 NOP dword ptr [RAX]
1406ee670:  48 FF C1                 INC RCX
1406ee673:  66 39 14 4F              CMP word ptr [RDI + RCX*0x2],DX
1406ee677:  75 F7                    JNZ 0x1406ee670
1406ee679:  48 FF C1                 INC RCX
1406ee67c:  B8 02 00 00 00           MOV EAX,0x2
1406ee681:  48 F7 E1                 MUL RCX
1406ee684:  49 0F 40 C0              CMOVO RAX,R8
1406ee688:  48 8B C8                 MOV RCX,RAX
1406ee68b:  E8 84 F9 15 00           CALL 0x14084e014
1406ee690:  48 89 83 D0 05 00 00     MOV qword ptr [RBX + 0x5d0],RAX
1406ee697:  48 2B C7                 SUB RAX,RDI
1406ee69a:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
1406ee6a0:  0F B7 0F                 MOVZX ECX,word ptr [RDI]
1406ee6a3:  66 89 0C 38              MOV word ptr [RAX + RDI*0x1],CX
1406ee6a7:  48 83 C7 02              ADD RDI,0x2
1406ee6ab:  66 85 C9                 TEST CX,CX
1406ee6ae:  75 F0                    JNZ 0x1406ee6a0
1406ee6b0:  33 D2                    XOR EDX,EDX
1406ee6b2:  49 83 C8 FF              OR R8,-0x1
1406ee6b6:  EB 19                    JMP 0x1406ee6d1
1406ee6b8:  48 8B CB                 MOV RCX,RBX
1406ee6bb:  48 2B CF                 SUB RCX,RDI
1406ee6be:  66 90                    NOP
1406ee6c0:  0F B7 07                 MOVZX EAX,word ptr [RDI]
1406ee6c3:  66 89 44 39 3C           MOV word ptr [RCX + RDI*0x1 + 0x3c],AX
1406ee6c8:  48 83 C7 02              ADD RDI,0x2
1406ee6cc:  66 85 C0                 TEST AX,AX
1406ee6cf:  75 EF                    JNZ 0x1406ee6c0
1406ee6d1:  66 0F 6E 05 B3 6B 2A 00  MOVD XMM0,dword ptr [0x14099528c]
1406ee6d9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ee6dc:  F3 44 0F 10 25 F7 B6 21 00 MOVSS XMM12,dword ptr [0x140909ddc]
1406ee6e5:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406ee6ea:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ee6ef:  66 0F 6E 05 7D 6B 2A 00  MOVD XMM0,dword ptr [0x140995274]
1406ee6f7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ee6fa:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406ee6ff:  F3 0F 11 43 08           MOVSS dword ptr [RBX + 0x8],XMM0
1406ee704:  89 53 0C                 MOV dword ptr [RBX + 0xc],EDX
1406ee707:  44 89 83 60 02 00 00     MOV dword ptr [RBX + 0x260],R8D
1406ee70e:  44 89 83 64 02 00 00     MOV dword ptr [RBX + 0x264],R8D
1406ee715:  48 89 93 70 02 00 00     MOV qword ptr [RBX + 0x270],RDX
1406ee71c:  89 93 68 02 00 00        MOV dword ptr [RBX + 0x268],EDX
1406ee722:  44 89 83 78 02 00 00     MOV dword ptr [RBX + 0x278],R8D
1406ee729:  48 89 93 80 02 00 00     MOV qword ptr [RBX + 0x280],RDX
1406ee730:  44 89 83 88 02 00 00     MOV dword ptr [RBX + 0x288],R8D
1406ee737:  48 89 93 90 02 00 00     MOV qword ptr [RBX + 0x290],RDX
1406ee73e:  44 89 83 98 02 00 00     MOV dword ptr [RBX + 0x298],R8D
1406ee745:  48 89 93 A0 02 00 00     MOV qword ptr [RBX + 0x2a0],RDX
1406ee74c:  48 89 93 A8 02 00 00     MOV qword ptr [RBX + 0x2a8],RDX
1406ee753:  48 89 93 B0 02 00 00     MOV qword ptr [RBX + 0x2b0],RDX
1406ee75a:  89 53 14                 MOV dword ptr [RBX + 0x14],EDX
1406ee75d:  66 C7 03 00 00           MOV word ptr [RBX],0x0
1406ee762:  48 8D 15 CF 88 20 00     LEA RDX,[0x1408f7038]
1406ee769:  48 8D 0D 20 64 36 00     LEA RCX,[0x140a54b90]
1406ee770:  FF 15 62 E0 17 00        CALL qword ptr [0x14086c7d8]
1406ee776:  0F B6 F8                 MOVZX EDI,AL
1406ee779:  48 8D 15 D0 88 20 00     LEA RDX,[0x1408f7050]
1406ee780:  48 8D 0D 39 F8 2C 00     LEA RCX,[0x1409bdfc0]
1406ee787:  FF 15 33 D9 17 00        CALL qword ptr [0x14086c0c0]
1406ee78d:  85 C0                    TEST EAX,EAX
1406ee78f:  74 25                    JZ 0x1406ee7b6
1406ee791:  83 BB 4C 02 00 00 01     CMP dword ptr [RBX + 0x24c],0x1
1406ee798:  74 12                    JZ 0x1406ee7ac
1406ee79a:  40 84 FF                 TEST DIL,DIL
1406ee79d:  75 0D                    JNZ 0x1406ee7ac
1406ee79f:  83 7B 34 1D              CMP dword ptr [RBX + 0x34],0x1d
1406ee7a3:  74 07                    JZ 0x1406ee7ac
1406ee7a5:  B8 01 00 00 00           MOV EAX,0x1
1406ee7aa:  EB 02                    JMP 0x1406ee7ae
1406ee7ac:  33 C0                    XOR EAX,EAX
1406ee7ae:  88 83 F0 04 00 00        MOV byte ptr [RBX + 0x4f0],AL
1406ee7b4:  EB 07                    JMP 0x1406ee7bd
1406ee7b6:  C6 83 F0 04 00 00 00     MOV byte ptr [RBX + 0x4f0],0x0
1406ee7bd:  C7 83 F4 04 00 00 02 00 00 00 MOV dword ptr [RBX + 0x4f4],0x2
1406ee7c7:  48 8D 15 5A 8B 20 00     LEA RDX,[0x1408f7328]
1406ee7ce:  48 8D 0D EB F7 2C 00     LEA RCX,[0x1409bdfc0]
1406ee7d5:  FF 15 E5 D8 17 00        CALL qword ptr [0x14086c0c0]
1406ee7db:  4C 8B BC 24 60 01 00 00  MOV R15,qword ptr [RSP + 0x160]
1406ee7e3:  45 33 C9                 XOR R9D,R9D
1406ee7e6:  85 C0                    TEST EAX,EAX
1406ee7e8:  74 09                    JZ 0x1406ee7f3
1406ee7ea:  4C 89 BB C8 05 00 00     MOV qword ptr [RBX + 0x5c8],R15
1406ee7f1:  EB 07                    JMP 0x1406ee7fa
1406ee7f3:  4C 89 8B C8 05 00 00     MOV qword ptr [RBX + 0x5c8],R9
1406ee7fa:  4D 85 FF                 TEST R15,R15
1406ee7fd:  74 1C                    JZ 0x1406ee81b
1406ee7ff:  49 8B 87 C0 02 00 00     MOV RAX,qword ptr [R15 + 0x2c0]
1406ee806:  48 89 83 C0 02 00 00     MOV qword ptr [RBX + 0x2c0],RAX
1406ee80d:  49 8B 87 C8 02 00 00     MOV RAX,qword ptr [R15 + 0x2c8]
1406ee814:  48 89 83 C8 02 00 00     MOV qword ptr [RBX + 0x2c8],RAX
1406ee81b:  48 C7 43 18 00 00 00 00  MOV qword ptr [RBX + 0x18],0x0
1406ee823:  44 89 4B 24              MOV dword ptr [RBX + 0x24],R9D
1406ee827:  48 C7 83 50 02 00 00 00 00 00 00 MOV qword ptr [RBX + 0x250],0x0
1406ee832:  89 B3 3C 02 00 00        MOV dword ptr [RBX + 0x23c],ESI
1406ee838:  44 89 8B A8 04 00 00     MOV dword ptr [RBX + 0x4a8],R9D
1406ee83f:  48 C7 43 18 00 00 00 00  MOV qword ptr [RBX + 0x18],0x0
1406ee843:  89 83 AC 04 00 00        MOV dword ptr [RBX + 0x4ac],EAX
1406ee849:  4C 89 8B B0 04 00 00     MOV qword ptr [RBX + 0x4b0],R9
1406ee850:  48 C7 83 B8 04 00 00 00 00 00 00 MOV qword ptr [RBX + 0x4b8],0x0
1406ee85b:  F3 0F 10 15 25 38 2A 00  MOVSS XMM2,dword ptr [0x140992088]
1406ee863:  F3 0F 10 0D D1 C7 21 00  MOVSS XMM1,dword ptr [0x14090b03c]
1406ee86b:  85 F6                    TEST ESI,ESI
1406ee86d:  74 17                    JZ 0x1406ee886
1406ee86f:  0F 28 C2                 MOVAPS XMM0,XMM2
1406ee872:  F3 0F 59 C1              MULSS XMM0,XMM1
1406ee876:  F3 0F 11 83 24 05 00 00  MOVSS dword ptr [RBX + 0x524],XMM0
1406ee87e:  F3 0F 11 83 28 05 00 00  MOVSS dword ptr [RBX + 0x528],XMM0
1406ee886:  8B 94 24 38 01 00 00     MOV EDX,dword ptr [RSP + 0x138]
1406ee88d:  85 D2                    TEST EDX,EDX
1406ee88f:  75 31                    JNZ 0x1406ee8c2
1406ee891:  48 85 ED                 TEST RBP,RBP
1406ee894:  74 2C                    JZ 0x1406ee8c2
1406ee896:  48 8B 85 18 03 00 00     MOV RAX,qword ptr [RBP + 0x318]
1406ee89d:  48 85 C0                 TEST RAX,RAX
1406ee8a0:  74 20                    JZ 0x1406ee8c2
1406ee8a2:  83 B8 60 03 00 00 69     CMP dword ptr [RAX + 0x360],0x69
1406ee8a9:  75 17                    JNZ 0x1406ee8c2
1406ee8ab:  0F 28 C2                 MOVAPS XMM0,XMM2
1406ee8ae:  F3 0F 59 C1              MULSS XMM0,XMM1
1406ee8b2:  F3 0F 11 83 24 05 00 00  MOVSS dword ptr [RBX + 0x524],XMM0
1406ee8ba:  F3 0F 11 83 28 05 00 00  MOVSS dword ptr [RBX + 0x528],XMM0
1406ee8c2:  C7 83 A4 04 00 00 0F 00 00 00 MOV dword ptr [RBX + 0x4a4],0xf
1406ee8cc:  4C 89 8B D0 02 00 00     MOV qword ptr [RBX + 0x2d0],R9
1406ee8d3:  4C 89 8B D8 02 00 00     MOV qword ptr [RBX + 0x2d8],R9
1406ee8da:  4C 89 8B E0 02 00 00     MOV qword ptr [RBX + 0x2e0],R9
1406ee8e1:  4C 89 8B E8 02 00 00     MOV qword ptr [RBX + 0x2e8],R9
1406ee8e8:  4C 89 8B F0 02 00 00     MOV qword ptr [RBX + 0x2f0],R9
1406ee8ef:  4C 89 8B 2C 05 00 00     MOV qword ptr [RBX + 0x52c],R9
1406ee8f6:  4C 89 8B 34 05 00 00     MOV qword ptr [RBX + 0x534],R9
1406ee8fd:  4C 89 8B 3C 05 00 00     MOV qword ptr [RBX + 0x53c],R9
1406ee904:  4C 89 8B 44 05 00 00     MOV qword ptr [RBX + 0x544],R9
1406ee90b:  4C 89 8B 4C 05 00 00     MOV qword ptr [RBX + 0x54c],R9
1406ee912:  4C 89 8B 54 05 00 00     MOV qword ptr [RBX + 0x554],R9
1406ee919:  4C 89 8B 5C 05 00 00     MOV qword ptr [RBX + 0x55c],R9
1406ee920:  4C 89 8B 64 05 00 00     MOV qword ptr [RBX + 0x564],R9
1406ee927:  4C 89 8B 6C 05 00 00     MOV qword ptr [RBX + 0x56c],R9
1406ee92e:  4C 89 8B 74 05 00 00     MOV qword ptr [RBX + 0x574],R9
1406ee935:  F3 44 0F 10 05 3A C1 21 00 MOVSS XMM8,dword ptr [0x14090aa78]
1406ee93e:  49 BA 25 49 92 24 49 92 24 49 MOV R10,0x4924924924924925
1406ee948:  0F 57 C9                 XORPS XMM1,XMM1
1406ee94b:  85 D2                    TEST EDX,EDX
1406ee94d:  0F 85 DB 01 00 00        JNZ 0x1406eeb2e
1406ee953:  48 85 ED                 TEST RBP,RBP
1406ee956:  0F 84 D2 01 00 00        JZ 0x1406eeb2e
1406ee95c:  8B 85 10 13 00 00        MOV EAX,dword ptr [RBP + 0x1310]
1406ee962:  89 83 54 05 00 00        MOV dword ptr [RBX + 0x554],EAX
1406ee968:  F3 0F 10 85 10 13 00 00  MOVSS XMM0,dword ptr [RBP + 0x1310]
1406ee970:  0F 2E C1                 UCOMISS XMM0,XMM1
1406ee973:  7A 06                    JP 0x1406ee97b
1406ee975:  0F 84 A9 00 00 00        JZ 0x1406eea24
1406ee97b:  48 8B 85 18 03 00 00     MOV RAX,qword ptr [RBP + 0x318]
1406ee982:  4C 63 80 3C 08 00 00     MOVSXD R8,dword ptr [RAX + 0x83c]
1406ee989:  45 85 C0                 TEST R8D,R8D
1406ee98c:  0F 88 92 00 00 00        JS 0x1406eea24
1406ee992:  48 8B 8D 78 09 00 00     MOV RCX,qword ptr [RBP + 0x978]
1406ee999:  48 2B 8D 70 09 00 00     SUB RCX,qword ptr [RBP + 0x970]
1406ee9a0:  49 8B C2                 MOV RAX,R10
1406ee9a3:  48 F7 E9                 IMUL RCX
1406ee9a6:  48 C1 FA 06              SAR RDX,0x6
1406ee9aa:  48 8B C2                 MOV RAX,RDX
1406ee9ad:  48 C1 E8 3F              SHR RAX,0x3f
1406ee9b1:  48 03 D0                 ADD RDX,RAX
1406ee9b4:  49 8B C0                 MOV RAX,R8
1406ee9b7:  4C 3B C2                 CMP R8,RDX
1406ee9ba:  73 68                    JNC 0x1406eea24
1406ee9bc:  48 69 C8 E0 00 00 00     IMUL RCX,RAX,0xe0
```

```asm
; test15_creator.asm lines 401-600 (partial excerpt)
1406ee9c3:  48 03 8D 70 09 00 00     ADD RCX,qword ptr [RBP + 0x970]
1406ee9ca:  8B 81 90 00 00 00        MOV EAX,dword ptr [RCX + 0x90]
1406ee9d0:  83 E8 0F                 SUB EAX,0xf
1406ee9d3:  83 F8 01                 CMP EAX,0x1
1406ee9d6:  77 41                    JA 0x1406eea19
1406ee9d8:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1406ee9dc:  48 2B 01                 SUB RAX,qword ptr [RCX]
1406ee9df:  48 C1 F8 04              SAR RAX,0x4
1406ee9e3:  48 83 F8 02              CMP RAX,0x2
1406ee9e7:  75 30                    JNZ 0x1406eea19
1406ee9e9:  48 8B 11                 MOV RDX,qword ptr [RCX]
1406ee9ec:  49 8B 85 88 C3 00 00     MOV RAX,qword ptr [R13 + 0xc388]
1406ee9f3:  48 39 02                 CMP qword ptr [RDX],RAX
1406ee9f6:  75 21                    JNZ 0x1406eea19
1406ee9f8:  49 8B 85 90 C3 00 00     MOV RAX,qword ptr [R13 + 0xc390]
1406ee9ff:  48 39 42 10              CMP qword ptr [RDX + 0x10],RAX
1406eea03:  75 14                    JNZ 0x1406eea19
1406eea05:  F3 0F 10 42 18           MOVSS XMM0,dword ptr [RDX + 0x18]
1406eea0a:  F3 0F 58 42 08           ADDSS XMM0,dword ptr [RDX + 0x8]
1406eea0f:  F3 0F 5E 81 8C 00 00 00  DIVSS XMM0,dword ptr [RCX + 0x8c]
1406eea17:  EB 03                    JMP 0x1406eea1c
1406eea19:  0F 28 C1                 MOVAPS XMM0,XMM1
1406eea1c:  F3 0F 11 83 78 05 00 00  MOVSS dword ptr [RBX + 0x578],XMM0
1406eea24:  48 8B 85 18 03 00 00     MOV RAX,qword ptr [RBP + 0x318]
1406eea2b:  48 85 C0                 TEST RAX,RAX
1406eea2e:  0F 84 C9 00 00 00        JZ 0x1406eeafd
1406eea34:  8B 80 60 03 00 00        MOV EAX,dword ptr [RAX + 0x360]
1406eea3a:  83 F8 62                 CMP EAX,0x62
1406eea3d:  74 0C                    JZ 0x1406eea4b
1406eea3f:  83 C0 A3                 ADD EAX,-0x5d
1406eea42:  83 F8 02                 CMP EAX,0x2
1406eea45:  0F 87 B2 00 00 00        JA 0x1406eeafd
1406eea4b:  48 8B 8D 78 09 00 00     MOV RCX,qword ptr [RBP + 0x978]
1406eea52:  48 2B 8D 70 09 00 00     SUB RCX,qword ptr [RBP + 0x970]
1406eea59:  49 8B C2                 MOV RAX,R10
1406eea5c:  48 F7 E9                 IMUL RCX
1406eea5f:  48 C1 FA 06              SAR RDX,0x6
1406eea63:  48 8B C2                 MOV RAX,RDX
1406eea66:  48 C1 E8 3F              SHR RAX,0x3f
1406eea6a:  48 03 D0                 ADD RDX,RAX
1406eea6d:  85 D2                    TEST EDX,EDX
1406eea6f:  0F 84 88 00 00 00        JZ 0x1406eeafd
1406eea75:  4C 8B 85 70 09 00 00     MOV R8,qword ptr [RBP + 0x970]
1406eea7c:  0F 1F 40 00              NOP dword ptr [RAX]
1406eea80:  FF CA                    DEC EDX
1406eea82:  8B C2                    MOV EAX,EDX
1406eea84:  48 69 C8 E0 00 00 00     IMUL RCX,RAX,0xe0
1406eea8b:  42 83 BC 01 90 00 00 00 10 CMP dword ptr [RCX + R8*0x1 + 0x90],0x10
1406eea94:  74 06                    JZ 0x1406eea9c
1406eea96:  85 D2                    TEST EDX,EDX
1406eea98:  75 E6                    JNZ 0x1406eea80
1406eea9a:  EB 61                    JMP 0x1406eeafd
1406eea9c:  8B C2                    MOV EAX,EDX
1406eea9e:  48 69 C8 E0 00 00 00     IMUL RCX,RAX,0xe0
1406eeaa5:  49 03 C8                 ADD RCX,R8
1406eeaa8:  8B 81 90 00 00 00        MOV EAX,dword ptr [RCX + 0x90]
1406eeaae:  83 E8 0F                 SUB EAX,0xf
1406eeab1:  83 F8 01                 CMP EAX,0x1
1406eeab4:  77 3F                    JA 0x1406eeaf5
1406eeab6:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1406eeaba:  48 2B 01                 SUB RAX,qword ptr [RCX]
1406eeabd:  48 C1 F8 04              SAR RAX,0x4
1406eeac1:  48 83 F8 02              CMP RAX,0x2
1406eeac5:  75 2E                    JNZ 0x1406eeaf5
1406eeac7:  48 8B 11                 MOV RDX,qword ptr [RCX]
1406eeaca:  49 8B 85 88 C3 00 00     MOV RAX,qword ptr [R13 + 0xc388]
1406eead1:  48 39 02                 CMP qword ptr [RDX],RAX
1406eead4:  75 1F                    JNZ 0x1406eeaf5
1406eead6:  49 8B 85 90 C3 00 00     MOV RAX,qword ptr [R13 + 0xc390]
1406eeadd:  48 39 42 10              CMP qword ptr [RDX + 0x10],RAX
1406eeae1:  75 12                    JNZ 0x1406eeaf5
1406eeae3:  F3 0F 10 4A 18           MOVSS XMM1,dword ptr [RDX + 0x18]
1406eeae8:  F3 0F 58 4A 08           ADDSS XMM1,dword ptr [RDX + 0x8]
1406eeaed:  F3 0F 5E 89 8C 00 00 00  DIVSS XMM1,dword ptr [RCX + 0x8c]
1406eeaf5:  F3 0F 11 8B 78 05 00 00  MOVSS dword ptr [RBX + 0x578],XMM1
1406eeafd:  48 8B 85 18 03 00 00     MOV RAX,qword ptr [RBP + 0x318]
1406eeb04:  8B 94 24 38 01 00 00     MOV EDX,dword ptr [RSP + 0x138]
1406eeb0b:  48 85 C0                 TEST RAX,RAX
1406eeb0e:  74 1E                    JZ 0x1406eeb2e
1406eeb10:  83 B8 60 03 00 00 5F     CMP dword ptr [RAX + 0x360],0x5f
1406eeb17:  75 15                    JNZ 0x1406eeb2e
1406eeb19:  F3 0F 10 85 18 13 00 00  MOVSS XMM0,dword ptr [RBP + 0x1318]
1406eeb21:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1406eeb26:  F3 0F 11 83 58 05 00 00  MOVSS dword ptr [RBX + 0x558],XMM0
1406eeb2e:  8B 4B 34                 MOV ECX,dword ptr [RBX + 0x34]
1406eeb31:  8D 41 F9                 LEA EAX,[RCX + -0x7]
1406eeb34:  41 B8 E6 06 00 00        MOV R8D,0x6e6
1406eeb3a:  A9 FA FF FF FF           TEST EAX,0xfffffffa
1406eeb3f:  75 05                    JNZ 0x1406eeb46
1406eeb41:  83 F9 0B                 CMP ECX,0xb
1406eeb44:  75 0F                    JNZ 0x1406eeb55
1406eeb46:  85 D2                    TEST EDX,EDX
1406eeb48:  75 42                    JNZ 0x1406eeb8c
1406eeb4a:  83 FE 0A                 CMP ESI,0xa
1406eeb4d:  77 3D                    JA 0x1406eeb8c
1406eeb4f:  41 0F A3 F0              BT R8D,ESI
1406eeb53:  73 37                    JNC 0x1406eeb8c
1406eeb55:  B9 70 02 00 00           MOV ECX,0x270
1406eeb5a:  E8 6D F3 15 00           CALL 0x14084decc
1406eeb5f:  48 89 44 24 48           MOV qword ptr [RSP + 0x48],RAX
1406eeb64:  41 B8 00 00 AA FF        MOV R8D,0xffaa0000
1406eeb6a:  48 8B 15 CF 66 2A 00     MOV RDX,qword ptr [0x140995240]
1406eeb71:  48 8B C8                 MOV RCX,RAX
1406eeb74:  E8 17 ED A0 FF           CALL 0x1400fd890
1406eeb79:  90                       NOP
1406eeb7a:  48 89 83 90 02 00 00     MOV qword ptr [RBX + 0x290],RAX
1406eeb81:  F3 0F 10 15 FF 34 2A 00  MOVSS XMM2,dword ptr [0x140992088]
1406eeb89:  45 33 C9                 XOR R9D,R9D
1406eeb8c:  F3 0F 10 1D AC CC 21 00  MOVSS XMM3,dword ptr [0x14090b840]
1406eeb94:  F3 44 0F 10 0D 37 B3 21 00 MOVSS XMM9,dword ptr [0x140909ed4]
1406eeb9d:  F3 0F 10 25 D7 CA 21 00  MOVSS XMM4,dword ptr [0x14090b67c]
1406eeba5:  83 BB 4C 02 00 00 05     CMP dword ptr [RBX + 0x24c],0x5
1406eebac:  75 1D                    JNZ 0x1406eebcb
1406eebae:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eebb1:  F3 0F 59 C3              MULSS XMM0,XMM3
1406eebb5:  F3 41 0F 59 C1           MULSS XMM0,XMM9
1406eebba:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eebbf:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eebc2:  F3 0F 59 C4              MULSS XMM0,XMM4
1406eebc6:  F3 0F 11 43 2C           MOVSS dword ptr [RBX + 0x2c],XMM0
1406eebcb:  83 BB 4C 02 00 00 01     CMP dword ptr [RBX + 0x24c],0x1
1406eebd2:  75 18                    JNZ 0x1406eebec
1406eebd4:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eebd7:  F3 0F 59 C3              MULSS XMM0,XMM3
1406eebdb:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eebe0:  0F 28 CA                 MOVAPS XMM1,XMM2
1406eebe3:  F3 0F 59 CC              MULSS XMM1,XMM4
1406eebe7:  F3 0F 11 4B 2C           MOVSS dword ptr [RBX + 0x2c],XMM1
1406eebec:  83 BB 4C 02 00 00 00     CMP dword ptr [RBX + 0x24c],0x0
1406eebf3:  75 18                    JNZ 0x1406eec0d
1406eebf5:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eebf8:  F3 0F 59 C3              MULSS XMM0,XMM3
1406eebfc:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eec01:  0F 28 CA                 MOVAPS XMM1,XMM2
1406eec04:  F3 0F 59 CC              MULSS XMM1,XMM4
1406eec08:  F3 0F 11 4B 2C           MOVSS dword ptr [RBX + 0x2c],XMM1
1406eec0d:  83 BB 4C 02 00 00 02     CMP dword ptr [RBX + 0x24c],0x2
1406eec14:  75 2E                    JNZ 0x1406eec44
1406eec16:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eec19:  F3 0F 59 05 BF CC 21 00  MULSS XMM0,dword ptr [0x14090b8e0]
1406eec21:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eec26:  0F 28 CA                 MOVAPS XMM1,XMM2
1406eec29:  F3 0F 59 0D 33 CC 21 00  MULSS XMM1,dword ptr [0x14090b864]
1406eec31:  F3 0F 11 4B 2C           MOVSS dword ptr [RBX + 0x2c],XMM1
1406eec36:  4D 89 8D E8 C8 00 00     MOV qword ptr [R13 + 0xc8e8],R9
1406eec3d:  4D 89 8D E0 C8 00 00     MOV qword ptr [R13 + 0xc8e0],R9
1406eec44:  83 BB 4C 02 00 00 03     CMP dword ptr [RBX + 0x24c],0x3
1406eec4b:  75 2E                    JNZ 0x1406eec7b
1406eec4d:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eec50:  F3 0F 59 05 44 CD 21 00  MULSS XMM0,dword ptr [0x14090b99c]
1406eec58:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eec5d:  0F 28 CA                 MOVAPS XMM1,XMM2
1406eec60:  F3 0F 59 0D 44 CC 21 00  MULSS XMM1,dword ptr [0x14090b8ac]
1406eec68:  F3 0F 11 4B 2C           MOVSS dword ptr [RBX + 0x2c],XMM1
1406eec6d:  4D 89 8D E8 C8 00 00     MOV qword ptr [R13 + 0xc8e8],R9
1406eec74:  4D 89 8D E0 C8 00 00     MOV qword ptr [R13 + 0xc8e0],R9
1406eec7b:  83 BB 4C 02 00 00 04     CMP dword ptr [RBX + 0x24c],0x4
1406eec82:  75 48                    JNZ 0x1406eeccc
1406eec84:  0F 28 C2                 MOVAPS XMM0,XMM2
1406eec87:  F3 0F 59 05 85 CE 21 00  MULSS XMM0,dword ptr [0x14090bb14]
1406eec8f:  F3 0F 11 43 28           MOVSS dword ptr [RBX + 0x28],XMM0
1406eec94:  0F 28 CA                 MOVAPS XMM1,XMM2
1406eec97:  F3 0F 59 0D CD CC 21 00  MULSS XMM1,dword ptr [0x14090b96c]
1406eec9f:  F3 0F 11 4B 2C           MOVSS dword ptr [RBX + 0x2c],XMM1
1406eeca4:  8B 43 34                 MOV EAX,dword ptr [RBX + 0x34]
1406eeca7:  83 E8 27                 SUB EAX,0x27
1406eecaa:  A9 FB FF FF FF           TEST EAX,0xfffffffb
1406eecaf:  75 0D                    JNZ 0x1406eecbe
1406eecb1:  F3 0F 59 15 BF CA 21 00  MULSS XMM2,dword ptr [0x14090b778]
1406eecb9:  F3 0F 11 53 2C           MOVSS dword ptr [RBX + 0x2c],XMM2
1406eecbe:  4D 89 8D E8 C8 00 00     MOV qword ptr [R13 + 0xc8e8],R9
1406eecc5:  4D 89 8D E0 C8 00 00     MOV qword ptr [R13 + 0xc8e0],R9
1406eeccc:  48 8B CB                 MOV RCX,RBX
1406eeccf:  E8 DC 8A 12 00           CALL 0x1408177b0
1406eecd4:  48 8D 15 65 86 20 00     LEA RDX,[0x1408f7340]
1406eecdb:  48 8D 0D DE F2 2C 00     LEA RCX,[0x1409bdfc0]
1406eece2:  FF 15 D8 D3 17 00        CALL qword ptr [0x14086c0c0]
1406eece8:  BF 01 00 00 00           MOV EDI,0x1
1406eeced:  85 C0                    TEST EAX,EAX
1406eecef:  48 C7 C0 FF FF FF FF     MOV RAX,-0x1
1406eecf6:  0F 45 F8                 CMOVNZ EDI,EAX
1406eecf9:  4D 85 FF                 TEST R15,R15
1406eecfc:  74 1D                    JZ 0x1406eed1b
1406eecfe:  66 0F 6E 05 86 65 2A 00  MOVD XMM0,dword ptr [0x14099528c]
1406eed06:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406eed09:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406eed0e:  BF 01 00 00 00           MOV EDI,0x1
1406eed13:  41 0F 2F 47 04           COMISS XMM0,dword ptr [R15 + 0x4]
1406eed18:  0F 47 F8                 CMOVA EDI,EAX
1406eed1b:  48 8D 15 36 86 20 00     LEA RDX,[0x1408f7358]
1406eed22:  48 8D 0D 97 F2 2C 00     LEA RCX,[0x1409bdfc0]
1406eed29:  FF 15 91 D3 17 00        CALL qword ptr [0x14086c0c0]
1406eed2f:  85 C0                    TEST EAX,EAX
1406eed31:  0F 84 1F 01 00 00        JZ 0x1406eee56
1406eed37:  4D 85 FF                 TEST R15,R15
1406eed3a:  74 54                    JZ 0x1406eed90
1406eed3c:  F3 0F 10 35 44 33 2A 00  MOVSS XMM6,dword ptr [0x140992088]
1406eed44:  0F 28 C6                 MOVAPS XMM0,XMM6
1406eed47:  F3 0F 59 05 05 C1 21 00  MULSS XMM0,dword ptr [0x14090ae54]
1406eed4f:  F3 0F 5C 43 2C           SUBSS XMM0,dword ptr [RBX + 0x2c]
```

```asm
; test15_creator.asm lines 601-800 (partial excerpt)
1406eed54:  F3 0F 11 43 08           MOVSS dword ptr [RBX + 0x8],XMM0
1406eed59:  F3 0F 10 4B 28           MOVSS XMM1,dword ptr [RBX + 0x28]
1406eed5e:  F3 41 0F 58 4F 28        ADDSS XMM1,dword ptr [R15 + 0x28]
1406eed64:  0F 28 C6                 MOVAPS XMM0,XMM6
1406eed67:  F3 41 0F 59 C0           MULSS XMM0,XMM8
1406eed6c:  F3 0F 5C C8              SUBSS XMM1,XMM0
1406eed70:  66 0F 6E C7              MOVD XMM0,EDI
1406eed74:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406eed77:  F3 0F 59 C8              MULSS XMM1,XMM0
1406eed7b:  F3 41 0F 58 4F 04        ADDSS XMM1,dword ptr [R15 + 0x4]
1406eed81:  F3 0F 11 4B 04           MOVSS dword ptr [RBX + 0x4],XMM1
1406eed86:  F3 0F 10 7B 08           MOVSS XMM7,dword ptr [RBX + 0x8]
1406eed8b:  E9 EE 00 00 00           JMP 0x1406eee7e
1406eed90:  F3 0F 10 3D F0 32 2A 00  MOVSS XMM7,dword ptr [0x140992088]
1406eed98:  F3 0F 59 3D B4 C0 21 00  MULSS XMM7,dword ptr [0x14090ae54]
1406eeda0:  F3 0F 5C 7B 2C           SUBSS XMM7,dword ptr [RBX + 0x2c]
1406eeda5:  F3 0F 11 7B 08           MOVSS dword ptr [RBX + 0x8],XMM7
1406eedaa:  48 8D 15 BF 85 20 00     LEA RDX,[0x1408f7370]
1406eedb1:  48 8D 0D 08 F2 2C 00     LEA RCX,[0x1409bdfc0]
1406eedb8:  FF 15 02 D3 17 00        CALL qword ptr [0x14086c0c0]
1406eedbe:  F3 0F 10 35 C2 32 2A 00  MOVSS XMM6,dword ptr [0x140992088]
1406eedc6:  0F 28 C6                 MOVAPS XMM0,XMM6
1406eedc9:  85 C0                    TEST EAX,EAX
1406eedcb:  74 49                    JZ 0x1406eee16
1406eedcd:  0F 28 CE                 MOVAPS XMM1,XMM6
1406eedd0:  F3 0F 59 05 18 BC 21 00  MULSS XMM0,dword ptr [0x14090a9f0]
1406eedd8:  49 83 BD 50 C6 00 00 00  CMP qword ptr [R13 + 0xc650],0x0
1406eede0:  74 1C                    JZ 0x1406eedfe
1406eede2:  F3 41 0F 59 8D 54 AE 00 00 MULSS XMM1,dword ptr [R13 + 0xae54]
1406eedeb:  F3 0F 5C 4B 28           SUBSS XMM1,dword ptr [RBX + 0x28]
1406eedf0:  F3 0F 58 C8              ADDSS XMM1,XMM0
1406eedf4:  F3 0F 11 4B 04           MOVSS dword ptr [RBX + 0x4],XMM1
1406eedf9:  E9 80 00 00 00           JMP 0x1406eee7e
1406eedfe:  F3 0F 59 0D 26 BD 21 00  MULSS XMM1,dword ptr [0x14090ab2c]
1406eee06:  F3 0F 5C 4B 28           SUBSS XMM1,dword ptr [RBX + 0x28]
1406eee0b:  F3 0F 58 C8              ADDSS XMM1,XMM0
1406eee0f:  F3 0F 11 4B 04           MOVSS dword ptr [RBX + 0x4],XMM1
1406eee14:  EB 68                    JMP 0x1406eee7e
1406eee16:  66 0F 6E 0D 6E 64 2A 00  MOVD XMM1,dword ptr [0x14099528c]
1406eee1e:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406eee21:  F3 0F 58 4B 28           ADDSS XMM1,dword ptr [RBX + 0x28]
1406eee26:  41 83 BD 78 31 01 00 FF  CMP dword ptr [R13 + 0x13178],-0x1
1406eee2e:  74 13                    JZ 0x1406eee43
1406eee30:  F3 0F 59 05 D4 C0 21 00  MULSS XMM0,dword ptr [0x14090af0c]
1406eee38:  F3 0F 5C C8              SUBSS XMM1,XMM0
1406eee3c:  F3 0F 11 4B 04           MOVSS dword ptr [RBX + 0x4],XMM1
1406eee41:  EB 3B                    JMP 0x1406eee7e
1406eee43:  F3 0F 59 05 A5 BB 21 00  MULSS XMM0,dword ptr [0x14090a9f0]
1406eee4b:  F3 0F 5C C8              SUBSS XMM1,XMM0
1406eee4f:  F3 0F 11 4B 04           MOVSS dword ptr [RBX + 0x4],XMM1
1406eee54:  EB 28                    JMP 0x1406eee7e
1406eee56:  F3 0F 10 35 2A 32 2A 00  MOVSS XMM6,dword ptr [0x140992088]
1406eee5e:  0F 28 C6                 MOVAPS XMM0,XMM6
1406eee61:  F3 0F 59 05 B3 BF 21 00  MULSS XMM0,dword ptr [0x14090ae1c]
1406eee69:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406eee6e:  0F 28 FE                 MOVAPS XMM7,XMM6
1406eee71:  F3 0F 59 3D 2F BE 21 00  MULSS XMM7,dword ptr [0x14090aca8]
1406eee79:  F3 0F 11 7B 08           MOVSS dword ptr [RBX + 0x8],XMM7
1406eee7e:  0F 28 C6                 MOVAPS XMM0,XMM6
1406eee81:  F3 0F 59 05 9F BA 21 00  MULSS XMM0,dword ptr [0x14090a928]
1406eee89:  F3 41 0F 59 B5 54 AE 00 00 MULSS XMM6,dword ptr [R13 + 0xae54]
1406eee92:  F3 0F 58 F0              ADDSS XMM6,XMM0
1406eee96:  49 83 BD 50 C6 00 00 00  CMP qword ptr [R13 + 0xc650],0x0
1406eee9e:  75 03                    JNZ 0x1406eeea3
1406eeea0:  0F 28 F0                 MOVAPS XMM6,XMM0
1406eeea3:  48 8D 15 1E 84 20 00     LEA RDX,[0x1408f72c8]
1406eeeaa:  48 8D 0D 0F F1 2C 00     LEA RCX,[0x1409bdfc0]
1406eeeb1:  FF 15 09 D2 17 00        CALL qword ptr [0x14086c0c0]
1406eeeb7:  85 C0                    TEST EAX,EAX
1406eeeb9:  75 0B                    JNZ 0x1406eeec6
1406eeebb:  0F 2F 73 04              COMISS XMM6,dword ptr [RBX + 0x4]
1406eeebf:  76 05                    JBE 0x1406eeec6
1406eeec1:  F3 0F 11 73 04           MOVSS dword ptr [RBX + 0x4],XMM6
1406eeec6:  F3 44 0F 10 5B 04        MOVSS XMM11,dword ptr [RBX + 0x4]
1406eeecc:  48 8D 15 0D 84 20 00     LEA RDX,[0x1408f72e0]
1406eeed3:  48 8D 0D E6 F0 2C 00     LEA RCX,[0x1409bdfc0]
1406eeeda:  FF 15 E0 D1 17 00        CALL qword ptr [0x14086c0c0]
1406eeee0:  33 ED                    XOR EBP,EBP
1406eeee2:  85 C0                    TEST EAX,EAX
1406eeee4:  0F 84 9D 00 00 00        JZ 0x1406eef87
1406eeeea:  8B D5                    MOV EDX,EBP
1406eeeec:  45 32 C9                 XOR R9B,R9B
1406eeeef:  83 BB 48 02 00 00 05     CMP dword ptr [RBX + 0x248],0x5
1406eeef6:  0F 84 8B 00 00 00        JZ 0x1406eef87
1406eeefc:  4D 85 FF                 TEST R15,R15
1406eeeff:  0F 85 82 00 00 00        JNZ 0x1406eef87
1406eef05:  4D 8B 46 08              MOV R8,qword ptr [R14 + 0x8]
1406eef09:  4D 2B 06                 SUB R8,qword ptr [R14]
1406eef0c:  49 C1 F8 03              SAR R8,0x3
1406eef10:  45 85 C0                 TEST R8D,R8D
1406eef13:  74 72                    JZ 0x1406eef87
1406eef15:  41 8B C8                 MOV ECX,R8D
1406eef18:  49 8B 06                 MOV RAX,qword ptr [R14]
1406eef1b:  4C 8D 14 C8              LEA R10,[RAX + RCX*0x8]
1406eef1f:  90                       NOP
1406eef20:  41 FF C8                 DEC R8D
1406eef23:  4D 8D 52 F8              LEA R10,[R10 + -0x8]
1406eef27:  49 8B 02                 MOV RAX,qword ptr [R10]
1406eef2a:  40 38 A8 F0 04 00 00     CMP byte ptr [RAX + 0x4f0],BPL
1406eef31:  74 35                    JZ 0x1406eef68
1406eef33:  48 85 C0                 TEST RAX,RAX
1406eef36:  74 30                    JZ 0x1406eef68
1406eef38:  48 85 D2                 TEST RDX,RDX
1406eef3b:  75 05                    JNZ 0x1406eef42
1406eef3d:  48 8B D0                 MOV RDX,RAX
1406eef40:  EB 0D                    JMP 0x1406eef4f
1406eef42:  F3 0F 10 42 08           MOVSS XMM0,dword ptr [RDX + 0x8]
1406eef47:  0F 2F 40 08              COMISS XMM0,dword ptr [RAX + 0x8]
1406eef4b:  48 0F 47 D0              CMOVA RDX,RAX
1406eef4f:  45 84 C9                 TEST R9B,R9B
1406eef52:  75 14                    JNZ 0x1406eef68
1406eef54:  F3 0F 10 40 08           MOVSS XMM0,dword ptr [RAX + 0x8]
1406eef59:  0F 2E C7                 UCOMISS XMM0,XMM7
1406eef5c:  7A 07                    JP 0x1406eef65
1406eef5e:  75 05                    JNZ 0x1406eef65
1406eef60:  41 B1 01                 MOV R9B,0x1
1406eef63:  EB 03                    JMP 0x1406eef68
1406eef65:  45 32 C9                 XOR R9B,R9B
1406eef68:  45 85 C0                 TEST R8D,R8D
1406eef6b:  75 B3                    JNZ 0x1406eef20
1406eef6d:  48 85 D2                 TEST RDX,RDX
1406eef70:  74 15                    JZ 0x1406eef87
1406eef72:  45 84 C9                 TEST R9B,R9B
1406eef75:  74 10                    JZ 0x1406eef87
1406eef77:  F2 0F 10 42 04           MOVSD XMM0,qword ptr [RDX + 0x4]
1406eef7c:  F2 0F 11 43 04           MOVSD qword ptr [RBX + 0x4],XMM0
1406eef81:  8B 42 0C                 MOV EAX,dword ptr [RDX + 0xc]
1406eef84:  89 43 0C                 MOV dword ptr [RBX + 0xc],EAX
1406eef87:  83 BB 4C 02 00 00 01     CMP dword ptr [RBX + 0x24c],0x1
1406eef8e:  0F 84 C6 01 00 00        JZ 0x1406ef15a
1406eef94:  8B 43 34                 MOV EAX,dword ptr [RBX + 0x34]
1406eef97:  83 E8 29                 SUB EAX,0x29
1406eef9a:  83 F8 01                 CMP EAX,0x1
1406eef9d:  0F 86 B7 01 00 00        JBE 0x1406ef15a
1406eefa3:  8B 83 48 02 00 00        MOV EAX,dword ptr [RBX + 0x248]
1406eefa9:  83 E8 05                 SUB EAX,0x5
1406eefac:  A9 FD FF FF FF           TEST EAX,0xfffffffd
1406eefb1:  0F 84 A3 01 00 00        JZ 0x1406ef15a
1406eefb7:  48 8D 15 3A 83 20 00     LEA RDX,[0x1408f72f8]
1406eefbe:  48 8D 0D FB EF 2C 00     LEA RCX,[0x1409bdfc0]
1406eefc5:  FF 15 F5 D0 17 00        CALL qword ptr [0x14086c0c0]
1406eefcb:  85 C0                    TEST EAX,EAX
1406eefcd:  0F 84 81 00 00 00        JZ 0x1406ef054
1406eefd3:  4D 85 FF                 TEST R15,R15
1406eefd6:  0F 84 7E 01 00 00        JZ 0x1406ef15a
1406eefdc:  49 8B 4E 08              MOV RCX,qword ptr [R14 + 0x8]
1406eefe0:  49 2B 0E                 SUB RCX,qword ptr [R14]
1406eefe3:  48 C1 F9 03              SAR RCX,0x3
1406eefe7:  85 C9                    TEST ECX,ECX
1406eefe9:  0F 84 6B 01 00 00        JZ 0x1406ef15a
1406eefef:  8B D1                    MOV EDX,ECX
1406eeff1:  48 C1 E2 03              SHL RDX,0x3
1406eeff5:  F3 0F 10 1D 8B 30 2A 00  MOVSS XMM3,dword ptr [0x140992088]
1406eeffd:  0F 1F 00                 NOP dword ptr [RAX]
1406ef000:  FF C9                    DEC ECX
1406ef002:  48 8D 52 F8              LEA RDX,[RDX + -0x8]
1406ef006:  49 8B 06                 MOV RAX,qword ptr [R14]
1406ef009:  4C 8B 04 02              MOV R8,qword ptr [RDX + RAX*0x1]
1406ef00d:  4D 39 B8 C8 05 00 00     CMP qword ptr [R8 + 0x5c8],R15
1406ef014:  75 35                    JNZ 0x1406ef04b
1406ef016:  41 83 B8 3C 02 00 00 00  CMP dword ptr [R8 + 0x23c],0x0
1406ef01e:  75 2B                    JNZ 0x1406ef04b
1406ef020:  0F 28 D3                 MOVAPS XMM2,XMM3
1406ef023:  F3 41 0F 59 D0           MULSS XMM2,XMM8
1406ef028:  66 0F 6E CF              MOVD XMM1,EDI
1406ef02c:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef02f:  F3 0F 59 CA              MULSS XMM1,XMM2
1406ef033:  F3 0F 10 43 04           MOVSS XMM0,dword ptr [RBX + 0x4]
1406ef038:  F3 0F 5C C1              SUBSS XMM0,XMM1
1406ef03c:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef041:  F3 0F 58 53 08           ADDSS XMM2,dword ptr [RBX + 0x8]
1406ef046:  F3 0F 11 53 08           MOVSS dword ptr [RBX + 0x8],XMM2
1406ef04b:  85 C9                    TEST ECX,ECX
1406ef04d:  75 B1                    JNZ 0x1406ef000
1406ef04f:  E9 06 01 00 00           JMP 0x1406ef15a
1406ef054:  44 8B 15 19 62 2A 00     MOV R10D,dword ptr [0x140995274]
1406ef05b:  44 8B 1D 2A 62 2A 00     MOV R11D,dword ptr [0x14099528c]
1406ef062:  F3 0F 10 3D 1E 30 2A 00  MOVSS XMM7,dword ptr [0x140992088]
1406ef06a:  F3 44 0F 10 15 59 B8 21 00 MOVSS XMM10,dword ptr [0x14090a8cc]
1406ef073:  F3 44 0F 10 05 AC B9 21 00 MOVSS XMM8,dword ptr [0x14090aa28]
1406ef07c:  0F 1F 40 00              NOP dword ptr [RAX]
1406ef080:  41 B1 01                 MOV R9B,0x1
1406ef083:  44 8B C5                 MOV R8D,EBP
1406ef086:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef08a:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef08d:  48 C1 F8 03              SAR RAX,0x3
1406ef091:  48 85 C0                 TEST RAX,RAX
1406ef094:  0F 84 C0 00 00 00        JZ 0x1406ef15a
1406ef09a:  48 8B CD                 MOV RCX,RBP
1406ef09d:  0F 1F 00                 NOP dword ptr [RAX]
1406ef0a0:  49 8B 16                 MOV RDX,qword ptr [R14]
1406ef0a3:  48 8B 83 C8 05 00 00     MOV RAX,qword ptr [RBX + 0x5c8]
1406ef0aa:  48 39 04 CA              CMP qword ptr [RDX + RCX*0x8],RAX
1406ef0ae:  0F 84 83 00 00 00        JZ 0x1406ef137
1406ef0b4:  48 8B 14 CA              MOV RDX,qword ptr [RDX + RCX*0x8]
1406ef0b8:  80 BA F0 04 00 00 00     CMP byte ptr [RDX + 0x4f0],0x0
1406ef0bf:  75 76                    JNZ 0x1406ef137
1406ef0c1:  0F 28 C7                 MOVAPS XMM0,XMM7
1406ef0c4:  F3 41 0F 59 C2           MULSS XMM0,XMM10
1406ef0c9:  F3 0F 10 73 04           MOVSS XMM6,dword ptr [RBX + 0x4]
```

```asm
; test15_creator.asm lines 801-1000 (partial excerpt)
1406ef0ce:  0F 28 D8                 MOVAPS XMM3,XMM0
1406ef0d1:  F3 0F 58 5B 08           ADDSS XMM3,dword ptr [RBX + 0x8]
1406ef0d6:  0F 28 D6                 MOVAPS XMM2,XMM6
1406ef0d9:  F3 0F 58 D0              ADDSS XMM2,XMM0
1406ef0dd:  E8 DE 57 00 00           CALL 0x1406f48c0
1406ef0e2:  84 C0                    TEST AL,AL
1406ef0e4:  74 51                    JZ 0x1406ef137
1406ef0e6:  45 32 C9                 XOR R9B,R9B
1406ef0e9:  0F 28 CF                 MOVAPS XMM1,XMM7
1406ef0ec:  F3 41 0F 59 C8           MULSS XMM1,XMM8
1406ef0f1:  F3 0F 58 F1              ADDSS XMM6,XMM1
1406ef0f5:  F3 0F 11 73 04           MOVSS dword ptr [RBX + 0x4],XMM6
1406ef0fa:  66 41 0F 6E C3           MOVD XMM0,R11D
1406ef0ff:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ef102:  F3 41 0F 59 C1           MULSS XMM0,XMM9
1406ef107:  0F 2F F0                 COMISS XMM6,XMM0
1406ef10a:  76 10                    JBe 0x1406ef11c
1406ef10c:  F3 44 0F 11 5B 04        MOVSS dword ptr [RBX + 0x4],XMM11
1406ef112:  F3 0F 58 4B 08           ADDSS XMM1,dword ptr [RBX + 0x8]
1406ef117:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef11c:  66 41 0F 6E CA           MOVD XMM1,R10D
1406ef121:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef124:  F3 41 0F 59 C9           MULSS XMM1,XMM9
1406ef129:  F3 0F 10 43 08           MOVSS XMM0,dword ptr [RBX + 0x8]
1406ef12e:  0F 2F C1                 COMISS XMM0,XMM1
1406ef131:  0F 87 49 FF FF FF        JA 0x1406ef080
1406ef137:  41 FF C0                 INC R8D
1406ef13a:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef13e:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef141:  48 C1 F8 03              SAR RAX,0x3
1406ef145:  41 8B C8                 MOV ECX,R8D
1406ef148:  48 3B C8                 CMP RCX,RAX
1406ef14b:  0F 82 4F FF FF FF        JC 0x1406ef0a0
1406ef151:  45 84 C9                 TEST R9B,R9B
1406ef154:  0F 84 26 FF FF FF        JZ 0x1406ef080
1406ef15a:  48 8D 15 AF 81 20 00     LEA RDX,[0x1408f7310]
1406ef161:  48 8D 0D 58 EE 2C 00     LEA RCX,[0x1409bdfc0]
1406ef168:  FF 15 52 CF 17 00        CALL qword ptr [0x14086c0c0]
1406ef16e:  85 C0                    TEST EAX,EAX
1406ef170:  75 1E                    JNZ 0x1406ef190
1406ef172:  F3 0F 10 43 04           MOVSS XMM0,dword ptr [RBX + 0x4]
1406ef177:  F3 0F 5C 43 28           SUBSS XMM0,dword ptr [RBX + 0x28]
1406ef17c:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef181:  F3 0F 10 4B 08           MOVSS XMM1,dword ptr [RBX + 0x8]
1406ef186:  F3 0F 5C 4B 2C           SUBSS XMM1,dword ptr [RBX + 0x2c]
1406ef18b:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef190:  44 8B 15 DD 60 2A 00     MOV R10D,dword ptr [0x140995274]
1406ef197:  44 8B 1D EE 60 2A 00     MOV R11D,dword ptr [0x14099528c]
1406ef19e:  F3 0F 10 15 E2 2E 2A 00  MOVSS XMM2,dword ptr [0x140992088]
1406ef1a6:  83 BB 4C 02 00 00 01     CMP dword ptr [RBX + 0x24c],0x1
1406ef1ad:  0F 85 82 00 00 00        JNZ 0x1406ef235
1406ef1b3:  66 41 0F 6E C3           MOVD XMM0,R11D
1406ef1b8:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ef1bb:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406ef1c0:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef1c5:  66 41 0F 6E CA           MOVD XMM1,R10D
1406ef1ca:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef1cd:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1406ef1d2:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef1d7:  44 8B C5                 MOV R8D,EBP
1406ef1da:  49 8B 56 08              MOV RDX,qword ptr [R14 + 0x8]
1406ef1de:  49 2B 16                 SUB RDX,qword ptr [R14]
1406ef1e1:  48 C1 FA 03              SAR RDX,0x3
1406ef1e5:  85 D2                    TEST EDX,EDX
1406ef1e7:  74 27                    JZ 0x1406ef210
1406ef1e9:  8B CA                    MOV ECX,EDX
1406ef1eb:  49 8B 06                 MOV RAX,qword ptr [R14]
1406ef1ee:  4C 8D 0C C8              LEA R9,[RAX + RCX*0x8]
1406ef1f2:  4D 8D 49 F8              LEA R9,[R9 + -0x8]
1406ef1f6:  49 8B 01                 MOV RAX,qword ptr [R9]
1406ef1f9:  41 8D 48 01              LEA ECX,[R8 + 0x1]
1406ef1fd:  83 B8 4C 02 00 00 01     CMP dword ptr [RAX + 0x24c],0x1
1406ef204:  41 0F 45 C8              CMOVNZ ECX,R8D
1406ef208:  44 8B C1                 MOV R8D,ECX
1406ef20b:  83 C2 FF                 ADD EDX,-0x1
1406ef20e:  75 E2                    JNZ 0x1406ef1f2
1406ef210:  43 8D 04 80              LEA EAX,[R8 + R8*0x4]
1406ef214:  C1 E0 03                 SHL EAX,0x3
1406ef217:  66 0F 6E C8              MOVD XMM1,EAX
1406ef21b:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef21e:  F3 0F 59 CA              MULSS XMM1,XMM2
1406ef222:  F3 0F 5C C1              SUBSS XMM0,XMM1
1406ef226:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef22b:  F3 0F 58 4B 08           ADDSS XMM1,dword ptr [RBX + 0x8]
1406ef230:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef235:  41 8D 44 24 E9           LEA EAX,[R12 + -0x17]
1406ef23a:  A9 EF FF FF FF           TEST EAX,0xffffffef
1406ef23f:  74 0B                    JZ 0x1406ef24c
1406ef241:  8B 43 34                 MOV EAX,dword ptr [RBX + 0x34]
1406ef244:  83 E8 29                 SUB EAX,0x29
1406ef247:  83 F8 02                 CMP EAX,0x2
1406ef24a:  77 53                    JA 0x1406ef29f
1406ef24c:  66 41 0F 6E C3           MOVD XMM0,R11D
1406ef251:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ef254:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406ef259:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef25e:  66 41 0F 6E CA           MOVD XMM1,R10D
1406ef263:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef266:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1406ef26b:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef270:  83 7B 34 2A              CMP dword ptr [RBX + 0x34],0x2a
1406ef274:  75 11                    JNZ 0x1406ef287
1406ef276:  F3 0F 59 15 9A BA 21 00  MULSS XMM2,dword ptr [0x14090ad18]
1406ef27e:  F3 0F 5C CA              SUBSS XMM1,XMM2
1406ef282:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef287:  41 83 FC 27              CMP R12D,0x27
1406ef28b:  74 0B                    JZ 0x1406ef298
1406ef28d:  8B 43 34                 MOV EAX,dword ptr [RBX + 0x34]
1406ef290:  83 E8 29                 SUB EAX,0x29
1406ef293:  83 F8 01                 CMP EAX,0x1
1406ef296:  77 07                    JA 0x1406ef29f
1406ef298:  C6 83 F0 04 00 00 00     MOV byte ptr [RBX + 0x4f0],0x0
1406ef29f:  41 83 FC 28              CMP R12D,0x28
1406ef2a3:  75 39                    JNZ 0x1406ef2de
1406ef2a5:  C6 83 F0 04 00 00 00     MOV byte ptr [RBX + 0x4f0],0x0
1406ef2ac:  66 41 0F 6E C3           MOVD XMM0,R11D
1406ef2b1:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1406ef2b4:  F3 41 0F 59 C4           MULSS XMM0,XMM12
1406ef2b9:  F3 0F 11 43 04           MOVSS dword ptr [RBX + 0x4],XMM0
1406ef2be:  66 41 0F 6E CA           MOVD XMM1,R10D
1406ef2c3:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1406ef2c6:  F3 41 0F 59 CC           MULSS XMM1,XMM12
1406ef2cb:  F3 0F 11 4B 08           MOVSS dword ptr [RBX + 0x8],XMM1
1406ef2d0:  45 33 C0                 XOR R8D,R8D
1406ef2d3:  48 8B D3                 MOV RDX,RBX
1406ef2d6:  49 8B CD                 MOV RCX,R13
1406ef2d9:  E8 52 4C 11 00           CALL 0x140803f30
1406ef2de:  C7 83 F8 02 00 00 00 00 00 00 MOV dword ptr [RBX + 0x2f8],0x0
1406ef2e8:  C7 83 FC 02 00 00 00 01 01 01 MOV dword ptr [RBX + 0x2fc],0x1010100
1406ef2f2:  C7 83 00 03 00 00 01 01 01 01 MOV dword ptr [RBX + 0x300],0x1010101
1406ef2fc:  66 C7 83 04 03 00 00 01 01 MOV word ptr [RBX + 0x304],0x101
1406ef305:  48 8D 83 10 03 00 00     LEA RAX,[RBX + 0x310]
1406ef30c:  48 BA 02 00 00 00 02 00 00 00 MOV RDX,0x200000002
1406ef316:  B9 06 00 00 00           MOV ECX,0x6
1406ef31b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
1406ef320:  48 89 10                 MOV qword ptr [RAX],RDX
1406ef323:  48 89 50 08              MOV qword ptr [RAX + 0x8],RDX
1406ef327:  48 89 50 10              MOV qword ptr [RAX + 0x10],RDX
1406ef32b:  48 8D 40 40              LEA RAX,[RAX + 0x40]
1406ef32f:  48 89 50 D8              MOV qword ptr [RAX + -0x28],RDX
1406ef333:  48 89 50 E0              MOV qword ptr [RAX + -0x20],RDX
1406ef337:  48 89 50 E8              MOV qword ptr [RAX + -0x18],RDX
1406ef33b:  48 89 50 F0              MOV qword ptr [RAX + -0x10],RDX
1406ef33f:  48 89 50 F8              MOV qword ptr [RAX + -0x8],RDX
1406ef343:  48 83 E9 01              SUB RCX,0x1
1406ef347:  75 D7                    JNZ 0x1406ef320
1406ef349:  48 89 10                 MOV qword ptr [RAX],RDX
1406ef34c:  48 89 50 08              MOV qword ptr [RAX + 0x8],RDX
1406ef350:  89 8B 06 03 00 00        MOV dword ptr [RBX + 0x306],ECX
1406ef356:  C7 83 0A 03 00 00 01 00 00 00 MOV dword ptr [RBX + 0x30a],0x1
1406ef360:  66 89 8B 0E 03 00 00     MOV word ptr [RBX + 0x30e],CX
1406ef367:  C7 83 A0 04 00 00 00 00 20 41 MOV dword ptr [RBX + 0x4a0],0x41200000
1406ef371:  48 39 8B 40 02 00 00     CMP qword ptr [RBX + 0x240],RCX
1406ef378:  0F 84 04 01 00 00        JZ 0x1406ef482
1406ef37e:  39 8B 48 02 00 00        CMP dword ptr [RBX + 0x248],ECX
1406ef384:  0F 85 78 02 00 00        JNZ 0x1406ef602
1406ef38a:  48 8B 93 40 02 00 00     MOV RDX,qword ptr [RBX + 0x240]
1406ef391:  48 8B 82 18 03 00 00     MOV RAX,qword ptr [RDX + 0x318]
1406ef398:  48 85 C0                 TEST RAX,RAX
1406ef39b:  74 66                    JZ 0x1406ef403
1406ef39d:  8B 88 60 03 00 00        MOV ECX,dword ptr [RAX + 0x360]
1406ef3a3:  83 F9 0C                 CMP ECX,0xc
1406ef3a6:  74 05                    JZ 0x1406ef3ad
1406ef3a8:  83 F9 6A                 CMP ECX,0x6a
1406ef3ab:  75 56                    JNZ 0x1406ef403
1406ef3ad:  45 32 D2                 XOR R10B,R10B
1406ef3b0:  44 8B C5                 MOV R8D,EBP
1406ef3b3:  48 8B 82 80 0F 00 00     MOV RAX,qword ptr [RDX + 0xf80]
1406ef3ba:  48 2B 82 78 0F 00 00     SUB RAX,qword ptr [RDX + 0xf78]
1406ef3c1:  48 C1 F8 04              SAR RAX,0x4
1406ef3c5:  48 85 C0                 TEST RAX,RAX
1406ef3c8:  74 32                    JZ 0x1406ef3fc
1406ef3ca:  48 8B CD                 MOV RCX,RBP
1406ef3cd:  4C 8B 8A 78 0F 00 00     MOV R9,qword ptr [RDX + 0xf78]
1406ef3d4:  48 03 C9                 ADD RCX,RCX
1406ef3d7:  49 83 3C C9 00           CMP qword ptr [R9 + RCX*0x8],0x0
1406ef3dc:  75 1B                    JNZ 0x1406ef3f9
1406ef3de:  41 FF C0                 INC R8D
1406ef3e1:  48 8B 82 80 0F 00 00     MOV RAX,qword ptr [RDX + 0xf80]
1406ef3e8:  49 2B C1                 SUB RAX,R9
1406ef3eb:  48 C1 F8 04              SAR RAX,0x4
1406ef3ef:  41 8B C8                 MOV ECX,R8D
1406ef3f2:  48 3B C8                 CMP RCX,RAX
1406ef3f5:  72 DD                    JC 0x1406ef3d4
1406ef3f7:  EB 03                    JMP 0x1406ef3fc
1406ef3f9:  41 B2 01                 MOV R10B,0x1
1406ef3fc:  44 88 93 F9 02 00 00     MOV byte ptr [RBX + 0x2f9],R10B
1406ef403:  48 8B BA 18 03 00 00     MOV RDI,qword ptr [RDX + 0x318]
1406ef40a:  48 85 FF                 TEST RDI,RDI
1406ef40d:  74 73                    JZ 0x1406ef482
1406ef40f:  44 8B C5                 MOV R8D,EBP
1406ef412:  44 8B CD                 MOV R9D,EBP
1406ef415:  48 8B 82 78 0C 00 00     MOV RAX,qword ptr [RDX + 0xc78]
1406ef41c:  48 2B 82 70 0C 00 00     SUB RAX,qword ptr [RDX + 0xc70]
1406ef423:  48 C1 F8 03              SAR RAX,0x3
1406ef427:  48 85 C0                 TEST RAX,RAX
1406ef42a:  74 46                    JZ 0x1406ef472
1406ef42c:  48 8B C5                 MOV RAX,RBP
1406ef42f:  4C 8B 9A 70 0C 00 00     MOV R11,qword ptr [RDX + 0xc70]
1406ef436:  4C 8B 92 78 0C 00 00     MOV R10,qword ptr [RDX + 0xc78]
```

```asm
; test15_creator.asm lines 1001-1211 (partial excerpt, end of function)
1406ef43d:  4D 2B D3                 SUB R10,R11
1406ef440:  49 C1 FA 03              SAR R10,0x3
1406ef444:  0F 1F 40 00              NOP dword ptr [RAX]
1406ef448:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
1406ef450:  49 8B 04 C3              MOV RAX,qword ptr [R11 + RAX*0x8]
1406ef454:  41 8D 48 01              LEA ECX,[R8 + 0x1]
1406ef458:  48 83 B8 98 03 00 00 00  CMP qword ptr [RAX + 0x398],0x0
1406ef460:  41 0F 45 C8              CMOVNZ ECX,R8D
1406ef464:  44 8B C1                 MOV R8D,ECX
1406ef467:  41 FF C1                 INC R9D
1406ef46a:  41 8B C1                 MOV EAX,R9D
1406ef46d:  49 3B C2                 CMP RAX,R10
1406ef470:  72 DE                    JC 0x1406ef450
1406ef472:  44 3B 87 20 08 00 00     CMP R8D,dword ptr [RDI + 0x820]
1406ef479:  7C 07                    JL 0x1406ef482
1406ef47b:  C6 83 F8 02 00 00 01     MOV byte ptr [RBX + 0x2f8],0x1
1406ef482:  83 BB 48 02 00 00 00     CMP dword ptr [RBX + 0x248],0x0
1406ef489:  0F 85 73 01 00 00        JNZ 0x1406ef602
1406ef48f:  4C 8B BB 40 02 00 00     MOV R15,qword ptr [RBX + 0x240]
1406ef496:  4D 85 FF                 TEST R15,R15
1406ef499:  0F 84 63 01 00 00        JZ 0x1406ef602
1406ef49f:  44 8B ED                 MOV R13D,EBP
1406ef4a2:  49 8B 8F 78 09 00 00     MOV RCX,qword ptr [R15 + 0x978]
1406ef4a9:  49 2B 8F 70 09 00 00     SUB RCX,qword ptr [R15 + 0x970]
1406ef4b0:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
1406ef4ba:  48 F7 E9                 IMUL RCX
1406ef4bd:  48 C1 FA 06              SAR RDX,0x6
1406ef4c1:  48 8B C2                 MOV RAX,RDX
1406ef4c4:  48 C1 E8 3F              SHR RAX,0x3f
1406ef4c8:  48 03 D0                 ADD RDX,RAX
1406ef4cb:  0F 84 29 01 00 00        JZ 0x1406ef5fa
1406ef4d1:  4C 8B C5                 MOV R8,RBP
1406ef4d4:  4C 8D A3 A8 05 00 00     LEA R12,[RBX + 0x5a8]
1406ef4db:  48 BB 25 49 92 24 49 92 24 49 MOV RBX,0x4924924924924925
1406ef4e5:  49 BE AB AA AA AA AA AA AA 2A MOV R14,0x2aaaaaaaaaaaaaab
1406ef4ef:  EB 11                    JMP 0x1406ef502
1406ef500:  33 ED                    XOR EBP,EBP
1406ef502:  49 69 F0 E0 00 00 00     IMUL RSI,R8,0xe0
1406ef509:  49 03 B7 70 09 00 00     ADD RSI,qword ptr [R15 + 0x970]
1406ef510:  0F 57 C0                 XORPS XMM0,XMM0
1406ef513:  F3 0F 7F 44 24 50        MOVDQU xmmword ptr [RSP + 0x50],XMM0
1406ef519:  48 89 6C 24 60           MOV qword ptr [RSP + 0x60],RBP
1406ef51e:  48 8D 54 24 50           LEA RDX,[RSP + 0x50]
1406ef523:  49 8B CC                 MOV RCX,R12
1406ef526:  E8 E5 EC 12 00           CALL 0x14081e210
1406ef52b:  90                       NOP
1406ef52c:  48 8B 4C 24 50           MOV RCX,qword ptr [RSP + 0x50]
1406ef531:  48 85 C9                 TEST RCX,RCX
1406ef534:  74 17                    JZ 0x1406ef54d
1406ef536:  48 8B 54 24 60           MOV RDX,qword ptr [RSP + 0x60]
1406ef53b:  48 2B D1                 SUB RDX,RCX
1406ef53e:  48 C1 FA 02              SAR RDX,0x2
1406ef542:  41 B8 04 00 00 00        MOV R8D,0x4
1406ef548:  E8 03 25 91 FF           CALL 0x140001a50
1406ef54d:  49 8B 4C 24 08           MOV RCX,qword ptr [R12 + 0x8]
1406ef552:  49 2B 0C 24              SUB RCX,qword ptr [R12]
1406ef556:  49 8B C6                 MOV RAX,R14
1406ef559:  48 F7 E9                 IMUL RCX
1406ef55c:  48 C1 FA 02              SAR RDX,0x2
1406ef560:  48 8B C2                 MOV RAX,RDX
1406ef563:  48 C1 E8 3F              SHR RAX,0x3f
1406ef567:  48 03 D0                 ADD RDX,RAX
1406ef56a:  FF CA                    DEC EDX
1406ef56c:  48 8D 0C 52              LEA RCX,[RDX + RDX*0x2]
1406ef570:  49 8B 04 24              MOV RAX,qword ptr [R12]
1406ef574:  48 8D 2C C8              LEA RBP,[RAX + RCX*0x8]
1406ef578:  33 C0                    XOR EAX,EAX
1406ef57a:  8B F8                    MOV EDI,EAX
1406ef57c:  48 8B 46 08              MOV RAX,qword ptr [RSI + 0x8]
1406ef580:  48 2B 06                 SUB RAX,qword ptr [RSI]
1406ef583:  48 C1 F8 04              SAR RAX,0x4
1406ef587:  48 85 C0                 TEST RAX,RAX
1406ef58a:  74 33                    JZ 0x1406ef5bf
1406ef58c:  0F 1F 40 00              NOP dword ptr [RAX]
1406ef590:  C7 84 24 38 01 00 00 00 00 00 00 MOV dword ptr [RSP + 0x138],0x0
1406ef59b:  48 8D 94 24 38 01 00 00  LEA RDX,[RSP + 0x138]
1406ef5a3:  48 8B CD                 MOV RCX,RBP
1406ef5a6:  E8 35 38 95 FF           CALL 0x140042de0
1406ef5ab:  FF C7                    INC EDI
1406ef5ad:  48 8B 4E 08              MOV RCX,qword ptr [RSI + 0x8]
1406ef5b1:  48 2B 0E                 SUB RCX,qword ptr [RSI]
1406ef5b4:  48 C1 F9 04              SAR RCX,0x4
1406ef5b8:  8B C7                    MOV EAX,EDI
1406ef5ba:  48 3B C1                 CMP RAX,RCX
1406ef5bd:  72 D1                    JC 0x1406ef590
1406ef5bf:  41 FF C5                 INC R13D
1406ef5c2:  45 8B C5                 MOV R8D,R13D
1406ef5c5:  49 8B 8F 78 09 00 00     MOV RCX,qword ptr [R15 + 0x978]
1406ef5cc:  49 2B 8F 70 09 00 00     SUB RCX,qword ptr [R15 + 0x970]
1406ef5d3:  48 8B C3                 MOV RAX,RBX
1406ef5d6:  48 F7 E9                 IMUL RCX
1406ef5d9:  48 C1 FA 06              SAR RDX,0x6
1406ef5dd:  48 8B C2                 MOV RAX,RDX
1406ef5e0:  48 C1 E8 3F              SHR RAX,0x3f
1406ef5e4:  48 03 D0                 ADD RDX,RAX
1406ef5e7:  4C 3B C2                 CMP R8,RDX
1406ef5ea:  0F 82 10 FF FF FF        JC 0x1406ef500
1406ef5f0:  48 8B 5C 24 30           MOV RBX,qword ptr [RSP + 0x30]
1406ef5f5:  4C 8B 74 24 38           MOV R14,qword ptr [RSP + 0x38]
1406ef5fa:  4C 8B AC 24 20 01 00 00  MOV R13,qword ptr [RSP + 0x120]
1406ef602:  C6 83 14 05 00 00 00     MOV byte ptr [RBX + 0x514],0x0
1406ef609:  33 FF                    XOR EDI,EDI
1406ef60b:  89 BB 18 05 00 00        MOV dword ptr [RBX + 0x518],EDI
1406ef611:  89 BB 08 05 00 00        MOV dword ptr [RBX + 0x508],EDI
1406ef617:  40 88 BB F8 04 00 00     MOV byte ptr [RBX + 0x4f8],DIL
1406ef61e:  83 7B 34 08              CMP dword ptr [RBX + 0x34],0x8
1406ef622:  75 46                    JNZ 0x1406ef66a
1406ef624:  44 8B C7                 MOV R8D,EDI
1406ef627:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef62b:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef62e:  48 C1 F8 03              SAR RAX,0x3
1406ef632:  48 85 C0                 TEST RAX,RAX
1406ef635:  74 33                    JZ 0x1406ef66a
1406ef637:  8B D7                    MOV EDX,EDI
1406ef639:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
1406ef640:  49 8B 06                 MOV RAX,qword ptr [R14]
1406ef643:  48 8B 0C 02              MOV RCX,qword ptr [RDX + RAX*0x1]
1406ef647:  83 79 34 08              CMP dword ptr [RCX + 0x34],0x8
1406ef64b:  75 03                    JNZ 0x1406ef650
1406ef64d:  C6 01 01                 MOV byte ptr [RCX],0x1
1406ef650:  41 FF C0                 INC R8D
1406ef653:  48 83 C2 08              ADD RDX,0x8
1406ef657:  49 8B 4E 08              MOV RCX,qword ptr [R14 + 0x8]
1406ef65b:  49 2B 0E                 SUB RCX,qword ptr [R14]
1406ef65e:  48 C1 F9 03              SAR RCX,0x3
1406ef662:  49 63 C0                 MOVSXD RAX,R8D
1406ef665:  48 3B C1                 CMP RAX,RCX
1406ef668:  72 D6                    JC 0x1406ef640
1406ef66a:  83 7B 34 09              CMP dword ptr [RBX + 0x34],0x9
1406ef66e:  75 1A                    JNZ 0x1406ef68a
1406ef670:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef674:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef677:  48 C1 F8 03              SAR RAX,0x3
1406ef67b:  48 83 F8 01              CMP RAX,0x1
1406ef67f:  72 09                    JC 0x1406ef68a
1406ef681:  4D 8B 06                 MOV R8,qword ptr [R14]
1406ef684:  49 83 C0 08              ADD R8,0x8
1406ef688:  EB 03                    JMP 0x1406ef68d
1406ef68a:  4D 8B 06                 MOV R8,qword ptr [R14]
1406ef68d:  48 8D 44 24 30           LEA RAX,[RSP + 0x30]
1406ef692:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1406ef697:  48 8D 94 24 20 01 00 00  LEA RDX,[RSP + 0x120]
1406ef69f:  49 8B CE                 MOV RCX,R14
1406ef6a2:  E8 99 EA 92 FF           CALL 0x14001e140
1406ef6a7:  41 B8 01 00 00 00        MOV R8D,0x1
1406ef6ad:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef6b1:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef6b4:  48 C1 F8 03              SAR RAX,0x3
1406ef6b8:  41 3B C0                 CMP EAX,R8D
1406ef6bb:  7E 26                    JLE 0x1406ef6e3
1406ef6bd:  BA 08 00 00 00           MOV EDX,0x8
1406ef6c2:  49 8B 06                 MOV RAX,qword ptr [R14]
1406ef6c5:  48 8B 0C 10              MOV RCX,qword ptr [RAX + RDX*0x1]
1406ef6c9:  89 79 18                 MOV dword ptr [RCX + 0x18],EDI
1406ef6cc:  41 FF C0                 INC R8D
1406ef6cf:  48 8D 52 08              LEA RDX,[RDX + 0x8]
1406ef6d3:  49 8B 46 08              MOV RAX,qword ptr [R14 + 0x8]
1406ef6d7:  49 2B 06                 SUB RAX,qword ptr [R14]
1406ef6da:  48 C1 F8 03              SAR RAX,0x3
1406ef6de:  44 3B C0                 CMP R8D,EAX
1406ef6e1:  7C DF                    JL 0x1406ef6c2
1406ef6e3:  49 89 BD 48 CB 00 00     MOV qword ptr [R13 + 0xcb48],RDI
1406ef6ea:  48 85 DB                 TEST RBX,RBX
1406ef6ed:  0F 84 BF 00 00 00        JZ 0x1406ef7b2
1406ef6f3:  C7 43 18 9A 99 19 3F     MOV dword ptr [RBX + 0x18],0x3f19999a
1406ef6fa:  49 C7 85 C8 AD 00 00 01 00 00 00 MOV qword ptr [R13 + 0xadc8],0x1
1406ef705:  41 C7 85 F0 AD 00 00 05 00 00 00 MOV dword ptr [R13 + 0xadf0],0x5
1406ef710:  48 8D 15 D9 24 1E 00     LEA RDX,[0x1408d1bf0]
1406ef717:  48 8D 0D 72 54 36 00     LEA RCX,[0x140a54b90]
1406ef71e:  FF 15 B4 D0 17 00        CALL qword ptr [0x14086c7d8]
1406ef724:  84 C0                    TEST AL,AL
1406ef726:  74 0B                    JZ 0x1406ef733
1406ef728:  41 C7 85 F4 AD 00 00 03 00 00 00 MOV dword ptr [R13 + 0xadf4],0x3
1406ef733:  F2 0F 10 43 04           MOVSD XMM0,qword ptr [RBX + 0x4]
1406ef738:  F2 41 0F 11 85 34 AE 00 00 MOVSD qword ptr [R13 + 0xae34],XMM0
1406ef741:  8B 43 0C                 MOV EAX,dword ptr [RBX + 0xc]
1406ef744:  41 89 85 3C AE 00 00     MOV dword ptr [R13 + 0xae3c],EAX
1406ef74b:  41 39 BD E8 AD 00 00     CMP dword ptr [R13 + 0xade8],EDI
1406ef752:  74 1D                    JZ 0x1406ef771
1406ef754:  8B 4B 34                 MOV ECX,dword ptr [RBX + 0x34]
1406ef757:  8D 41 D9                 LEA EAX,[RCX + -0x27]
1406ef75a:  A9 FC FF FF FF           TEST EAX,0xfffffffc
1406ef75f:  75 10                    JNZ 0x1406ef771
1406ef761:  83 F9 28                 CMP ECX,0x28
1406ef764:  74 0B                    JZ 0x1406ef771
1406ef766:  41 C7 85 F4 AD 00 00 03 00 00 00 MOV dword ptr [R13 + 0xadf4],0x3
1406ef771:  8B 83 3C 02 00 00        MOV EAX,dword ptr [RBX + 0x23c]
1406ef777:  83 F8 0A                 CMP EAX,0xa
1406ef77a:  77 15                    JA 0x1406ef791
1406ef77c:  B9 E6 06 00 00           MOV ECX,0x6e6
1406ef781:  0F A3 C1                 BT ECX,EAX
1406ef784:  73 0B                    JNC 0x1406ef791
1406ef786:  41 C7 85 C8 AD 00 00 06 00 00 00 MOV dword ptr [R13 + 0xadc8],0x6
1406ef791:  83 BB 48 02 00 00 07     CMP dword ptr [RBX + 0x248],0x7
1406ef798:  75 07                    JNZ 0x1406ef7a1
1406ef79a:  41 89 BD C8 AD 00 00     MOV dword ptr [R13 + 0xadc8],EDI
1406ef7a1:  83 7B 34 27              CMP dword ptr [RBX + 0x34],0x27
1406ef7a5:  75 0B                    JNZ 0x1406ef7b2
1406ef7a7:  41 C7 85 C8 AD 00 00 01 00 00 00 MOV dword ptr [R13 + 0xadc8],0x1
1406ef7b2:  48 8B C3                 MOV RAX,RBX
1406ef7b5:  4C 8D 9C 24 E0 00 00 00  LEA R11,[RSP + 0xe0]
1406ef7bd:  49 8B 5B 50              MOV RBX,qword ptr [R11 + 0x50]
1406ef7c1:  41 0F 28 73 F0           MOVAPS XMM6,xmmword ptr [R11 + -0x10]
1406ef7c6:  41 0F 28 7B E0           MOVAPS XMM7,xmmword ptr [R11 + -0x20]
1406ef7cb:  45 0F 28 43 D0           MOVAPS XMM8,xmmword ptr [R11 + -0x30]
1406ef7d0:  45 0F 28 4B C0           MOVAPS XMM9,xmmword ptr [R11 + -0x40]
1406ef7d5:  45 0F 28 53 B0           MOVAPS XMM10,xmmword ptr [R11 + -0x50]
1406ef7da:  45 0F 28 5B A0           MOVAPS XMM11,xmmword ptr [R11 + -0x60]
1406ef7df:  45 0F 28 63 90           MOVAPS XMM12,xmmword ptr [R11 + -0x70]
1406ef7e4:  49 8B E3                 MOV RSP,R11
```

---

## Raw Artifact: scratch/disasm_25c500.txt

Source path: `scratch/disasm_25c500.txt` | Bytes: 12453 | SHA-256: `B15A9B9F82E4252E242318E39AA390C885F344057B0A72D207DFD8322A9C87B7`
Lines: 161 | DisasmMany VA: `0x14025C500` (ctor `FUN_14025C500`)

```asm

; Range: 0x14025C500 to 0x14025C800  (containing fn: FUN_14025c500@0x14025C500)
14025c500:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
14025c505:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
14025c50a:  57                                        PUSH RDI
14025c50b:  48 83 EC 40                               SUB RSP,0x40
14025c50f:  33 F6                                     XOR ESI,ESI
14025c511:  48 8D B9 A8 05 00 00                      LEA RDI,[RCX + 0x5a8]
14025c518:  48 89 B1 C0 04 00 00                      MOV qword ptr [RCX + 0x4c0],RSI
14025c51f:  48 8D 44 24 20                            LEA RAX,[RSP + 0x20]
14025c524:  48 89 B1 C8 04 00 00                      MOV qword ptr [RCX + 0x4c8],RSI
14025c52b:  0F 57 C0                                  XORPS XMM0,XMM0
14025c52e:  48 89 B1 D0 04 00 00                      MOV qword ptr [RCX + 0x4d0],RSI
14025c535:  48 8B D9                                  MOV RBX,RCX
14025c538:  48 89 B1 D8 04 00 00                      MOV qword ptr [RCX + 0x4d8],RSI
14025c53f:  48 89 B1 E0 04 00 00                      MOV qword ptr [RCX + 0x4e0],RSI
14025c546:  48 89 B1 E8 04 00 00                      MOV qword ptr [RCX + 0x4e8],RSI
14025c54d:  48 89 B1 80 05 00 00                      MOV qword ptr [RCX + 0x580],RSI
14025c554:  48 89 B1 88 05 00 00                      MOV qword ptr [RCX + 0x588],RSI
14025c55b:  48 89 B1 90 05 00 00                      MOV qword ptr [RCX + 0x590],RSI
14025c562:  48 89 37                                  MOV qword ptr [RDI],RSI
14025c565:  48 89 77 08                               MOV qword ptr [RDI + 0x8],RSI
14025c569:  48 89 77 10                               MOV qword ptr [RDI + 0x10],RSI
14025c56d:  48 89 74 24 30                            MOV qword ptr [RSP + 0x30],RSI
14025c572:  F3 0F 7F 44 24 20                         MOVDQU xmmword ptr [RSP + 0x20],XMM0
14025c578:  48 3B F8                                  CMP RDI,RAX
14025c57b:  74 21                                     JZ 0x14025c59e
14025c57d:  48 8B CF                                  MOV RCX,RDI
14025c580:  E8 CB 22 1F 00                            CALL 0x14044e850
14025c585:  48 89 37                                  MOV qword ptr [RDI],RSI
14025c588:  0F 57 C0                                  XORPS XMM0,XMM0
14025c58b:  48 89 67 08                               MOV qword ptr [RDI + 0x8],RSI
14025c58f:  F3 0F 7F 44 24 20                         MOVDQU xmmword ptr [RSP + 0x20],XMM0
14025c595:  48 89 67 10                               MOV qword ptr [RDI + 0x10],RSI
14025c599:  48 89 74 24 30                            MOV qword ptr [RSP + 0x30],RSI
14025c59e:  48 8D 4C 24 20                            LEA RCX,[RSP + 0x20]
14025c5a3:  E8 A8 22 1F 00                            CALL 0x14044e850
14025c5a8:  89 B3 7C 05 00 00                         MOV dword ptr [RBX + 0x57c],ESI
14025c5ae:  48 8B C3                                  MOV RAX,RBX
14025c5b1:  40 88 73 11                               MOV byte ptr [RBX + 0x11],SIL
14025c5b5:  89 73 1C                                  MOV dword ptr [RBX + 0x1c],ESI
14025c5b8:  89 B3 BC 02 00 00                         MOV dword ptr [RBX + 0x2bc],ESI
14025c5be:  48 89 B3 C0 02 00 00                      MOV qword ptr [RBX + 0x2c0],RSI
14025c5c5:  48 89 B3 C8 02 00 00                      MOV qword ptr [RBX + 0x2c8],RSI
14025c5cc:  48 89 B3 98 05 00 00                      MOV qword ptr [RBX + 0x598],RSI
14025c5d3:  48 89 B3 D0 05 00 00                      MOV qword ptr [RBX + 0x5d0],RSI
14025c5da:  89 B3 A0 05 00 00                         MOV dword ptr [RBX + 0x5a0],ESI
14025c5e0:  48 8B 74 24 58                            MOV RSI,qword ptr [RSP + 0x58]
14025c5e5:  C6 83 B8 02 00 00 01                      MOV byte ptr [RBX + 0x2b8],0x1
14025c5ec:  48 8B 5C 24 50                            MOV RBX,qword ptr [RSP + 0x50]
14025c5f1:  48 83 C4 40                               ADD RSP,0x40
14025c5f5:  5F                                        POP RDI
14025c5f6:  C3                                        RET
14025c600:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
14025c605:  48 89 74 24 10                            MOV qword ptr [RSP + 0x10],RSI
14025c60a:  57                                        PUSH RDI
14025c60b:  48 83 EC 20                               SUB RSP,0x20
14025c60f:  48 8B F9                                  MOV RDI,RCX
14025c612:  33 F6                                     XOR ESI,ESI
14025c614:  48 8B 89 D0 05 00 00                      MOV RCX,qword ptr [RCX + 0x5d0]
14025c61b:  48 85 C9                                  TEST RCX,RCX
14025c61e:  74 0C                                     JZ 0x14025c62c
14025c620:  E8 BB 19 5F 00                            CALL 0x14084dfe0
14025c625:  48 89 B7 D0 05 00 00                      MOV qword ptr [RDI + 0x5d0],RSI
14025c62c:  48 8D 8F A8 05 00 00                      LEA RCX,[RDI + 0x5a8]
14025c633:  E8 18 22 1F 00                            CALL 0x14044e850
14025c638:  48 8D 8F A8 05 00 00                      LEA RCX,[RDI + 0x5a8]
14025c63f:  E8 0C 22 1F 00                            CALL 0x14044e850
14025c644:  48 8B 8F 80 05 00 00                      MOV RCX,qword ptr [RDI + 0x580]
14025c64b:  48 85 C9                                  TEST RCX,RCX
14025c64e:  74 2E                                     JZ 0x14025c67e
14025c650:  48 8B 97 90 05 00 00                      MOV RDX,qword ptr [RDI + 0x590]
14025c657:  41 B8 04 00 00 00                         MOV R8D,0x4
14025c65d:  48 2B D1                                  SUB RDX,RCX
14025c660:  48 C1 FA 02                               SAR RDX,0x2
14025c664:  E8 E7 53 DA FF                            CALL 0x140001a50
14025c669:  48 89 B7 80 05 00 00                      MOV qword ptr [RDI + 0x580],RSI
14025c670:  48 89 B7 88 05 00 00                      MOV qword ptr [RDI + 0x588],RSI
14025c677:  48 89 B7 90 05 00 00                      MOV qword ptr [RDI + 0x590],RSI
14025c67e:  48 8B 8F D8 04 00 00                      MOV RCX,qword ptr [RDI + 0x4d8]
14025c685:  48 85 C9                                  TEST RCX,RCX
14025c688:  74 2E                                     JZ 0x14025c6b8
14025c68a:  48 8B 97 E8 04 00 00                      MOV RDX,qword ptr [RDI + 0x4e8]
14025c691:  41 B8 04 00 00 00                         MOV R8D,0x4
14025c697:  48 2B D1                                  SUB RDX,RCX
14025c69a:  48 C1 FA 02                               SAR RDX,0x2
14025c69e:  E8 AD 53 DA FF                            CALL 0x140001a50
14025c6a3:  48 89 B7 D8 04 00 00                      MOV qword ptr [RDI + 0x4d8],RSI
14025c6aa:  48 89 B7 E0 04 00 00                      MOV qword ptr [RDI + 0x4e0],RSI
14025c6b1:  48 89 B7 E8 04 00 00                      MOV qword ptr [RDI + 0x4e8],RSI
14025c6b8:  48 8B 8F C0 04 00 00                      MOV RCX,qword ptr [RDI + 0x4c0]
14025c6bf:  48 85 C9                                  TEST RCX,RCX
14025c6c2:  74 2E                                     JZ 0x14025c6f2
14025c6c4:  48 8B 97 D0 04 00 00                      MOV RDX,qword ptr [RDI + 0x4d0]
14025c6cb:  41 B8 10 00 00 00                         MOV R8D,0x10
14025c6d1:  48 2B D1                                  SUB RDX,RCX
14025c6d4:  48 C1 FA 04                               SAR RDX,0x4
14025c6d8:  E8 73 53 DA FF                            CALL 0x140001a50
14025c6dd:  48 89 B7 C0 04 00 00                      MOV qword ptr [RDI + 0x4c0],RSI
14025c6e4:  48 89 B7 C8 04 00 00                      MOV qword ptr [RDI + 0x4c8],RSI
14025c6eb:  48 89 B7 D0 04 00 00                      MOV qword ptr [RDI + 0x4d0],RSI
14025c6f2:  48 8B 5C 24 30                            MOV RBX,qword ptr [RSP + 0x30]
14025c6f7:  48 8B 74 24 38                            MOV RSI,qword ptr [RSP + 0x38]
14025c6fc:  48 83 C4 20                               ADD RSP,0x20
14025c700:  5F                                        POP RDI
14025c701:  C3                                        RET
14025c710:  48 83 EC 78                               SUB RSP,0x78
14025c714:  33 C0                                     XOR EAX,EAX
14025c716:  C6 01 00                                  MOV byte ptr [RCX],0x0
14025c719:  48 89 41 04                               MOV qword ptr [RCX + 0x4],RAX
14025c71d:  48 8D 54 24 50                            LEA RDX,[RSP + 0x50]
14025c722:  66 89 41 10                               MOV word ptr [RCX + 0x10],AX
14025c726:  0F 57 C0                                  XORPS XMM0,XMM0
14025c729:  C7 41 14 FF FF FF FF                      MOV dword ptr [RCX + 0x14],0xffffffff
14025c730:  0F 57 C9                                  XORPS XMM1,XMM1
14025c733:  48 89 41 18                               MOV qword ptr [RCX + 0x18],RAX
14025c737:  48 89 41 20                               MOV qword ptr [RCX + 0x20],RAX
14025c73b:  48 89 41 28                               MOV qword ptr [RCX + 0x28],RAX
14025c73f:  48 89 41 30                               MOV qword ptr [RCX + 0x30],RAX
14025c743:  48 89 41 38                               MOV qword ptr [RCX + 0x38],RAX
14025c747:  48 83 C1 40                               ADD RCX,0x40
14025c74b:  F2 0F 11 44 24 38                         MOVSD qword ptr [RSP + 0x38],XMM0
14025c751:  48 89 44 24 50                            MOV qword ptr [RSP + 0x50],RAX
14025c756:  48 89 44 24 60                            MOV qword ptr [RSP + 0x60],RAX
14025c75b:  66 89 44 24 20                            MOV word ptr [RSP + 0x20],AX
14025c760:  48 89 44 24 28                            MOV qword ptr [RSP + 0x28],RAX
14025c765:  C7 44 24 24 FF FF FF FF                   MOV dword ptr [RSP + 0x24],0xffffffff
14025c76d:  48 89 44 24 30                            MOV qword ptr [RSP + 0x30],RAX
14025c772:  F3 0F 7F 4C 24 40                         MOVDQU xmmword ptr [RSP + 0x40],XMM1
14025c778:  48 89 44 24 58                            MOV qword ptr [RSP + 0x58],RAX
14025c77d:  E8 DE 5B FA FF                            CALL 0x140202360
14025c782:  48 8B 4C 24 50                            MOV RCX,qword ptr [RSP + 0x50]
14025c787:  48 85 C9                                  TEST RCX,RCX
14025c78a:  74 1B                                     JZ 0x14025c7a7
14025c78c:  48 8B 54 24 60                            MOV RDX,qword ptr [RSP + 0x60]
14025c791:  41 B8 10 00 00 00                         MOV R8D,0x10
14025c797:  48 2B D1                                  SUB RDX,RCX
14025c79a:  48 C1 FA 04                               SAR RDX,0x4
14025c79e:  48 83 C4 78                               ADD RSP,0x78
14025c7a2:  E9 A9 52 DA FF                            JMP 0x140001a50
14025c7a7:  48 83 C4 78                               ADD RSP,0x78
14025c7ab:  C3                                        RET
14025c7b0:  48 89 5C 24 08                            MOV qword ptr [RSP + 0x8],RBX
14025c7b5:  48 89 7C 24 10                            MOV qword ptr [RSP + 0x10],RDI
14025c7ba:  4C 8B 81 F0 27 01 00                      MOV R8,qword ptr [RCX + 0x127f0]
14025c7c1:  48 8B DA                                  MOV RBX,RDX
14025c7c4:  4C 2B 81 E8 27 01 00                      SUB R8,qword ptr [RCX + 0x127e8]
14025c7cb:  48 BF 79 78 78 78 78 78 78 78             MOV RDI,0x7878787878787879
14025c7d5:  48 8B C7                                  MOV RAX,RDI
14025c7d8:  45 33 C9                                  XOR R9D,R9D
14025c7db:  49 F7 E8                                  IMUL R8
14025c7e1:  4C 8B D1                                  MOV R10,RCX
14025c7e8:  48 C1 FA 05                               SAR RDX,0x5
14025c7e5:  48 8B C2                                  MOV RAX,RDX
14025c7e8:  48 C1 E8 3F                               SHR RAX,0x3f
14025c7ec:  48 03 D0                                  ADD RDX,RAX
14025c7ef:  74 5F                                     JZ 0x14025c850
14025c7f1:  4C 8B 99 E8 27 01 00                      MOV R11,qword ptr [RCX + 0x127e8]
14025c7f8:  45 8B C1                                  MOV R8D,R9D
14025c7fb:  0F 1F 44 00 00                            NOP dword ptr [RAX + RAX*0x1]
14025c800:  4D 6B C0 44                               IMUL R8,R8,0x44
; 159 instruction(s)
```

---

## Raw Artifact: scratch/decomp_6e9b80.c

Source path: `scratch/decomp_6e9b80.c` | Bytes: 67072 | SHA-256: `43F384885C890B0F59D7D81D0C7D3821C47DE50BEEA551D38B086AB800B896F3`
Lines: 1521 | `FUN_1406E9B80` dispatcher (VA 0x1406E9B80)

```c
// ============================================================
// Function: FUN_1406e9b80 at 1406e9b80
// Target address checked: 0x1406E9B80
// ============================================================

/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1406e9b80(longlong param_1)

{
  void *_Src;
  void *_Dst;
  uint uVar1;
  undefined1 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  char cVar7;
  bool bVar8;
  int iVar9;
  C3DAPI_TEXTURE *pCVar10;
  float *pfVar11;
  C3DMATRIX *pCVar12;
  uint *puVar13;
  undefined8 *puVar14;
  char *pcVar15;
  longlong lVar16;
  longlong lVar17;
  uint uVar18;
  ulonglong uVar19;
  C3D_NODE *pCVar20;
  int iVar21;
  longlong lVar22;
  ulonglong uVar23;
  longlong *plVar24;
  longlong lVar25;
  ulonglong uVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  float fVar31;
  float fVar32;
  float fVar33;
  float fVar34;
  float fVar35;
  undefined1 auStackY_1528 [32];
  undefined8 local_14d8;
  float local_14d0;
  char local_14c8;
  undefined8 local_14b8;
  undefined8 uStack_14b0;
  ulonglong local_14a8;
  uint local_14a0;
  int local_1498;
  int local_1494;
  int local_1490;
  int local_148c;
  longlong local_1480;
  undefined4 local_1478;
  undefined4 local_1474;
  undefined4 local_1470;
  undefined4 local_1468;
  undefined4 local_1464;
  undefined4 local_1460;
  undefined4 local_1458;
  undefined4 local_1454;
  undefined4 local_1450;
  undefined4 local_1448;
  undefined4 local_1444;
  undefined4 local_1440;
  undefined4 local_1438;
  undefined4 local_1434;
  undefined4 local_1430;
  undefined4 local_1428;
  undefined4 local_1424;
  undefined4 local_1420;
  undefined4 local_1418;
  undefined4 local_1414;
  undefined4 local_1410;
  undefined4 local_1408;
  undefined4 local_1404;
  undefined4 local_1400;
  undefined4 local_13f8;
  undefined4 local_13f4;
  undefined4 local_13f0;
  undefined4 local_13e8;
  undefined4 local_13e4;
  undefined4 local_13e0;
  undefined4 local_13d8;
  undefined4 local_13d4;
  undefined4 local_13d0;
  undefined4 local_13c8;
  undefined4 local_13c4;
  undefined4 local_13c0;
  undefined4 local_13b8;
  undefined4 local_13b4;
  undefined4 local_13b0;
  undefined4 local_13a8;
  undefined4 local_13a4;
  undefined4 local_13a0;
  undefined4 local_1398;
  undefined4 local_1394;
  undefined4 local_1390;
  undefined4 local_1388;
  undefined4 local_1384;
  undefined4 local_1380;
  undefined4 local_1378;
  undefined4 local_1374;
  undefined4 local_1370;
  undefined4 local_1368;
  undefined4 local_1364;
  undefined4 local_1360;
  undefined4 local_1358;
  undefined4 uStack_1354;
  undefined4 uStack_1350;
  float fStack_134c;
  undefined8 local_1348;
  float local_1340;
  undefined8 local_1338;
  float local_1330;
  undefined8 local_1328;
  float local_1320;
  undefined8 local_1318;
  undefined4 local_1310;
  undefined8 local_1308;
  float local_1300;
  undefined8 local_12f8;
  float local_12f0;
  undefined8 local_12e8;
  float local_12e0;
  undefined8 local_12d8;
  undefined4 local_12d0;
  undefined8 local_12c8;
  float local_12c0;
  undefined8 local_12b8;
  float local_12b0;
  undefined8 local_12a8;
  float local_12a0;
  longlong local_1298;
  undefined8 local_1290;
  float local_1288;
  undefined8 local_1280;
  float local_1278;
  float local_1228;
  float local_121c;
  float local_1210;
  float local_1204;
  float local_11f8;
  float local_11ec;
  float local_11e0;
  float local_11d4;
  float local_11c8;
  undefined4 local_11bc;
  undefined4 local_11b0;
  undefined4 local_11a4;
  float local_1198;
  float local_118c;
  undefined8 local_1110;
  undefined8 uStack_1108;
  undefined8 local_10f8;
  int iStack_10f0;
  int iStack_10ec;
  undefined8 local_10e8;
  undefined8 uStack_10e0;
  wchar_t local_10d8 [2048];
  ulonglong local_d8;
  undefined8 uStack_30;
  
  uStack_30 = 0x1406e9b99;
  local_d8 = DAT_140992048 ^ (ulonglong)auStackY_1528;
  FUN_14025c710(param_1 + 0xcaf0);
  uVar19 = 0;
  *(undefined1 *)(param_1 + 0xcae0) = 0;
  *(undefined8 *)(param_1 + 0xcad8) = 0;
  *(undefined8 *)(param_1 + 0xcae8) = 0;
  if (*(char *)(param_1 + 0xcb78) != '\0') {
    if (DAT_140a54b91 == '\0') {
      *(undefined1 *)(param_1 + 0xcb78) = 0;
    }
    if (*(char *)(param_1 + 0xcb78) != '\0') goto LAB_1406e9d22;
  }
  if ((*(longlong *)(param_1 + 0xd428) == 0) && (DAT_140a54c91 != '\0')) {
    if (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0) {
      plVar24 = *(longlong **)(param_1 + 0xcac0);
      uVar26 = uVar19;
      do {
        iVar21 = (int)uVar26;
        lVar22 = *plVar24;
        if (((*(char *)(lVar22 + 1) == '\0') && (*(longlong *)(lVar22 + 0x5c8) != 0)) &&
           (*(int *)(lVar22 + 0x34) != 0x21)) goto LAB_1406e9ceb;
        uVar26 = (ulonglong)(iVar21 + 1U);
        plVar24 = plVar24 + 1;
      } while ((ulonglong)(longlong)(int)(iVar21 + 1U) <
               (ulonglong)(*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
    }
    if (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0) {
      plVar24 = *(longlong **)(param_1 + 0xcac0);
      uVar26 = uVar19;
      do {
        iVar21 = (int)uVar26;
        if (*(char *)(*plVar24 + 1) == '\0') goto LAB_1406e9ceb;
        uVar26 = (ulonglong)(iVar21 + 1U);
        plVar24 = plVar24 + 1;
      } while ((ulonglong)(longlong)(int)(iVar21 + 1U) <
               (ulonglong)(*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
    }
  }
  goto LAB_1406e9d22;
LAB_1406e9ceb:
  *(undefined1 *)(param_1 + 0xcb78) = 1;
  **(undefined1 **)(*(longlong *)(param_1 + 0xcac0) + (longlong)iVar21 * 8) = 1;
  if (*(int *)(*(longlong *)(*(longlong *)(param_1 + 0xcac0) + (longlong)iVar21 * 8) + 0x34) == 0x28
     ) {
    *(undefined4 *)(param_1 + 0x13934) = 0;
  }
  DAT_140a54c91 = '\0';
LAB_1406e9d22:
  uVar26 = uVar19;
  if (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0) {
    do {
      pcVar15 = *(char **)(*(longlong *)(param_1 + 0xcac0) + uVar26 * 8);
      if (*pcVar15 != '\0') {
        if ((*(longlong *)(pcVar15 + 0x5c8) != 0) && (*(int *)(param_1 + 0xade8) == 1)) {
          *(undefined4 *)(param_1 + 0xadf0) = 5;
          *(undefined4 *)(param_1 + 0xadf4) = 3;
          *(undefined8 *)(param_1 + 0xadc8) = 1;
        }
        uVar23 = uVar19;
        if (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0) {
          do {
            puVar2 = *(undefined1 **)(*(longlong *)(param_1 + 0xcac0) + uVar23 * 8);
            lVar22 = *(longlong *)(*(longlong *)(param_1 + 0xcac0) + uVar26 * 8);
            if (*(longlong *)(puVar2 + 0x5c8) == lVar22) {
              if (((*(longlong *)(puVar2 + 0x240) == *(longlong *)(lVar22 + 0x240)) &&
                  (*(int *)(puVar2 + 0x34) != 0x21)) ||
                 ((*(int *)(puVar2 + 0x23c) - 0xcU < 2 || (puVar2[0x4f0] != '\0')))) {
                *puVar2 = 1;
              }
              else {
                *(undefined8 *)(puVar2 + 0x5c8) = 0;
              }
            }
            uVar23 = (ulonglong)((int)uVar23 + 1);
          } while (uVar23 < (ulonglong)
                          (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3)
                );
        }
      }
      uVar26 = (ulonglong)((int)uVar26 + 1);
    } while (uVar26 < (ulonglong)
                      (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
  }
  fVar35 = DAT_140909f58;
  uVar26 = uVar19;
  if ((int)(*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3) != 0) {
    do {
      iVar21 = (int)uVar19;
      pcVar15 = *(char **)(uVar26 + *(longlong *)(param_1 + 0xcac0));
      lVar22 = *(longlong *)(pcVar15 + 0x240);
      if ((((lVar22 == 0) || (*(int *)(pcVar15 + 0x248) != 0)) || (*(int *)(pcVar15 + 0x34) == 3))
         || ((*(longlong *)(lVar22 + 0x5b0) == 0 || (*(float *)(lVar22 + 0x604) < fVar35)))) {
        if (*pcVar15 != '\0') goto LAB_1406e9edb;
      }
      else {
        *pcVar15 = '\x01';
LAB_1406e9edb:
        if (((*(int *)(pcVar15 + 0x34) == 0x1d) && (DAT_1409e2338 != (char *)0x0)) &&
           (iVar9 = strcmp(DAT_1409e2338,"measurement"), iVar9 == 0)) {
          *(undefined8 *)(param_1 + 0xd428) = 0;
        }
        if (((((*(longlong *)(pcVar15 + 0x240) == 0) || (*(int *)(pcVar15 + 0x248) != 8)) ||
             (-1 < *(int *)(pcVar15 + 0x288))) ||
            (((*(longlong *)(pcVar15 + 0x270) != 0 && (cVar7 = FUN_140493980(), cVar7 != '\0')) ||
             (*(int *)(pcVar15 + 0x24c) == 1)))) &&
           (((*(longlong *)(pcVar15 + 0x240) == 0 || (*(int *)(pcVar15 + 0x248) != 10)) ||
            ((-1 < *(int *)(pcVar15 + 0x288) ||
             ((*(longlong *)(pcVar15 + 0x270) != 0 && (cVar7 = FUN_140224fa0(), cVar7 != '\0')))))))
           ) {
          if (*(char **)(param_1 + 0xcb88) == pcVar15) {
            *(undefined8 *)(param_1 + 0xcb88) = 0;
            if (*(longlong *)(param_1 + 0xcb98) - *(longlong *)(param_1 + 0xcb90) >> 3 != 0) {
              *(undefined8 *)(param_1 + 0xcb98) = *(undefined8 *)(param_1 + 0xcb90);
            }
            if (*(longlong *)(param_1 + 0xcbb0) - *(longlong *)(param_1 + 0xcba8) >> 3 != 0) {
              *(undefined8 *)(param_1 + 0xcbb0) = *(undefined8 *)(param_1 + 0xcba8);
            }
            lVar25 = *(longlong *)(param_1 + 0xcbc8) - *(longlong *)(param_1 + 0xcbc0);
            lVar22 = lVar25 >> 0x3f;
            if (lVar25 / 0xc + lVar22 != lVar22) {
              *(undefined8 *)(param_1 + 0xcbc8) = *(undefined8 *)(param_1 + 0xcbc0);
            }
          }
          lVar22 = *(longlong *)(param_1 + 0x13bf0);
          if ((((lVar22 != 0) && (*(char *)(lVar22 + 0x14) != '\0')) &&
              (pcVar15 == *(char **)(lVar22 + 0x68))) && (*(int *)(lVar22 + 0x48) < 1)) {
            *(undefined4 *)(lVar22 + 0x48) = 2;
          }
          FUN_1406ed310();
          if ((*(longlong *)(pcVar15 + 0x240) == *(longlong *)(param_1 + 0x13030)) ||
             (*(longlong *)(pcVar15 + 0x240) == *(longlong *)(param_1 + 0x13040))) {
            FUN_140442b40(param_1 + 0x13018);
          }
          FUN_14025c600(pcVar15);
          free(pcVar15);
          _Dst = (void *)(*(longlong *)(param_1 + 0xcac0) + (longlong)iVar21 * 8);
          _Src = (void *)((longlong)_Dst + 8);
          memmove(_Dst,_Src,*(longlong *)(param_1 + 0xcac8) - (longlong)_Src);
          *(longlong *)(param_1 + 0xcac8) = *(longlong *)(param_1 + 0xcac8) + -8;
          iVar21 = iVar21 + -1;
          uVar26 = uVar26 - 8;
        }
        else {
          *pcVar15 = '\0';
          FUN_1406ee2a0(param_1,1,pcVar15);
        }
      }
      uVar19 = (ulonglong)(iVar21 + 1U);
      uVar26 = uVar26 + 8;
    } while (iVar21 + 1U <
             (uint)(*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
  }
  *(undefined1 *)(param_1 + 0xfc4) = *(undefined1 *)(param_1 + 0xfc3);
  *(undefined1 *)(param_1 + 0xfc3) = 0;
  *(undefined8 *)(param_1 + 0xcb80) = 0;
  uVar19 = *(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3;
  if ((int)uVar19 != 0) {
    lVar22 = (uVar19 & 0xffffffff) << 3;
    do {
      lVar22 = lVar22 + -8;
      uVar18 = (int)uVar19 - 1;
      uVar19 = (ulonglong)uVar18;
      cVar7 = FUN_1406ef830(param_1,*(undefined8 *)(lVar22 + *(longlong *)(param_1 + 0xcac0)));
      *(char *)(param_1 + 0xfc3) = cVar7;
      if (cVar7 != '\0') {
        lVar25 = *(longlong *)(lVar22 + *(longlong *)(param_1 + 0xcac0));
        *(longlong *)(param_1 + 0xcb80) = lVar25;
        *(undefined1 *)(lVar25 + 0x10) = 1;
      }
    } while (uVar18 != 0);
  }
  DAT_140a55248 = 0;
  C3D_PANEL2D::SetClipping((C3D_PANEL2D *)&DAT_1409be060,0.0,(float)DAT_140995274,0.0,0.0);
  wcscpy_s(local_10d8,0x800,(wchar_t *)(param_1 + 0xd5a0));
  *(undefined8 *)(param_1 + 0xcb50) = 0;
  if ((*(longlong *)(param_1 + 0x13bf0) == 0) ||
     (*(char *)(*(longlong *)(param_1 + 0x13bf0) + 0x14) == '\0')) {
    local_14c8 = '\0';
  }
  else {
    local_14c8 = '\x01';
  }
  *(undefined8 *)(param_1 + 0x117a8) = 0;
  fVar32 = DAT_140909ddc;
  uVar18 = (int)(*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3) - 1;
  local_14a8 = (ulonglong)uVar18;
  if (-1 < (int)uVar18) {
    lVar22 = (longlong)(int)uVar18;
    do {
      *(undefined2 *)(param_1 + 0xd5a0) = 0;
      puVar2 = *(undefined1 **)(*(longlong *)(param_1 + 0xcac0) + lVar22 * 8);
      iVar21 = (int)local_14a8;
      if ((iVar21 == 0) && (local_14c8 == '\0')) {
        *(undefined4 *)(param_1 + 0xada8) = 5;
      }
      else {
        *(undefined4 *)(param_1 + 0xada8) = 9;
      }
      if ((*(int *)(puVar2 + 0x4f4) == 0) || (*(int *)(puVar2 + 0x34) == 9)) {
        bVar8 = true;
      }
      else {
        bVar8 = false;
      }
      local_1480 = lVar22;
      C3D_FONTMANAGER::SetReallyRender((C3D_FONTMANAGER *)&DAT_140996fb0,bVar8);
      if ((*(int *)(puVar2 + 0x4f4) == 0) || (*(int *)(puVar2 + 0x34) == 9)) {
        bVar8 = true;
      }
      else {
        bVar8 = false;
      }
      C3D_PANEL2D::SetReallyRender((C3D_PANEL2D *)&DAT_1409be060,bVar8);
      FUN_1406f0070(param_1,puVar2);
      C3D_PANEL2D::EndDraw((C3D_PANEL2D *)&DAT_1409be060);
      C3D_FONTMANAGER::PrintAllTexts((C3D_FONTMANAGER *)&DAT_140996fb0,"Basic");
      C3D_PANEL2D::BeginDraw((C3D_PANEL2D *)&DAT_1409be060,(char *)0x0,false);
      if (-1 < *(int *)(param_1 + 0xad74)) {
        puVar2[0x20] = 0;
      }
      if ((((*(int *)(puVar2 + 0x34) == 0) && (*(int *)(puVar2 + 0x248) == 0)) &&
          (lVar25 = *(longlong *)(puVar2 + 0x240), lVar25 != 0)) &&
         ((fVar28 = *(float *)(puVar2 + 0x2c), fVar29 = *(float *)(puVar2 + 8),
          fVar27 = DAT_140992088 * DAT_14090a8cc,
          *(float *)(lVar25 + 0x604) <= fVar35 && fVar35 != *(float *)(lVar25 + 0x604) ||
          (*(longlong *)(lVar25 + 0x5d0) != 0)))) {
        if (*(char *)(lVar25 + 0xea8) == '\0') {
          plVar24 = *(longlong **)(param_1 + 0xafc0);
        }
        else {
          plVar24 = *(longlong **)(param_1 + 45000);
        }
        (**(code **)(*plVar24 + 0x70))();
        _DAT_1409be30c = _DAT_14090c4c0;
        uRam00000001409be314 = _UNK_14090c4c8;
        local_11a4 = 0;
        _DAT_1409be2ec = DAT_140992088 * DAT_14090a95c;
        DAT_1409be2f8 = 0;
        _DAT_1409be2f0 =
             CONCAT44((fVar28 + fVar29 + fVar27) - DAT_140992088 * DAT_14090aac8,
                      *(undefined4 *)(puVar2 + 4));
        _DAT_1409be2e8 = *(float *)(puVar2 + 0x520);
        C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar35,fVar35,0.0,true);
      }
      if (((*(int *)(puVar2 + 0x24c) == 2) ||
          ((*(uint *)(puVar2 + 0x34) < 0x2a &&
           ((0x29400000000U >> ((longlong)(int)*(uint *)(puVar2 + 0x34) & 0x3fU) & 1) != 0)))) &&
         (puVar2[0x4f8] != '\0')) {
        C3D_PANEL2D::SetClipping
                  ((C3D_PANEL2D *)&DAT_1409be060,
                   (*(float *)(puVar2 + 0x2c) + *(float *)(puVar2 + 8)) -
                   DAT_140992088 * DAT_14090a828,
                   (*(float *)(puVar2 + 0x2c) + *(float *)(puVar2 + 8)) + *(float *)(puVar2 + 0x51c) +
                   DAT_140992088 * DAT_14090a828,*(float *)(puVar2 + 0x504),(float)DAT_140995274);
      }
      if (**(char **)(*(longlong *)(param_1 + 0xcac0) + lVar22 * 8) == '\0') {
        if (*(int *)(puVar2 + 0x34) == 0) {
          iVar9 = *(int *)(puVar2 + 0x248);
          if (iVar9 == 7) {
            FUN_14048d600(param_1,puVar2);
          }
          else if (iVar9 == 5) {
            FUN_1404cabd0(param_1,puVar2);
          }
          else if (iVar9 == 3) {
            lVar22 = *(longlong *)(puVar2 + 0x240);
            bVar8 = false;
            uVar19 = 0;
            if (*(longlong *)(lVar22 + 0x1a8) - *(longlong *)(lVar22 + 0x1a0) >> 3 != 0) {
              lVar25 = *(longlong *)(lVar22 + 0x1a0);
              do {
                lVar16 = *(longlong *)(lVar25 + uVar19 * 8);
                if ((*(longlong *)(lVar16 + 0x318) != 0) &&
                   (*(int *)(*(longlong *)(lVar16 + 0x318) + 0x360) == 0x27)) {
                  bVar8 = true;
                  cVar7 = FUN_1401bbfe0(param_1);
                  if ((cVar7 == '\0') && (fVar32 < *(float *)(lVar16 + 0xde0))) {
                    lVar25 = *(longlong *)(lVar25 + uVar19 * 8);
                    if (lVar25 == 0) goto LAB_1406ea8a4;
                    FUN_1407cef60(param_1,puVar2,0xffffffff);
                    *(undefined4 *)(puVar2 + 0x248) = 0;
                    *(longlong *)(puVar2 + 0x240) = lVar25;
                    FUN_14072dbf0(param_1,puVar2);
                    *(undefined4 *)(puVar2 + 0x248) = 3;
                    *(longlong *)(puVar2 + 0x240) = lVar22;
                    goto LAB_1406eb642;
                  }
                }
                uVar19 = (ulonglong)((int)uVar19 + 1);
              } while (uVar19 < (ulonglong)(*(longlong *)(lVar22 + 0x1a8) - lVar25 >> 3));
              if (bVar8) {
LAB_1406ea8a4:
                FUN_1407cef60(param_1,puVar2);
                goto LAB_1406eb642;
              }
            }
            FUN_1407cef60(param_1,puVar2);
          }
          else if (iVar9 == 2) {
            FUN_1407ceeb0(param_1,puVar2);
          }
          else if (iVar9 == 1) {
            if (*(int *)(puVar2 + 0x23c) - 9U < 2) {
LAB_1406eb63d:
              FUN_14080cc60(param_1,puVar2);
            }
            else {
              FUN_1407d5460();
            }
          }
          else if (iVar9 == 8) {
            FUN_1407cff50(param_1,puVar2);
          }
          else if (iVar9 == 10) {
            FUN_140758420(param_1,puVar2);
          }
          else if (iVar9 == 0xb) {
            FUN_14075c470(param_1,puVar2);
          }
          else {
            if ((iVar9 - 0xcU < 2) || (iVar9 == 0xe)) goto LAB_1406eb63d;
            if (iVar9 == 0) {
              lVar22 = *(longlong *)(puVar2 + 0x240);
              if ((((*(char *)(param_1 + 9) != '\0') && (lVar22 != 0)) && (DAT_140a54bad != '\0'))
                 && ((DAT_140a54bbe != '\0' &&
                     (lVar16 = *(longlong *)(lVar22 + 0xa18) - *(longlong *)(lVar22 + 0xa10),
                     uVar18 = 0, lVar25 = lVar16 >> 0x3f, lVar16 / 0x60 + lVar25 != lVar25)))) {
                local_1468 = 0x3f800000;
                uVar19 = 0;
                local_1464 = 0x3f800000;
                local_1460 = 0x3f800000;
                do {
                  lVar25 = *(longlong *)(lVar22 + 0xa10);
                  lVar16 = uVar19 * 0x60;
                  local_14b8 = _DAT_140908e28;
                  uStack_14b0 = _UNK_140908e30;
                  local_1458 = *(undefined4 *)(lVar16 + 8 + lVar25);
                  local_1454 = *(undefined4 *)(lVar16 + 0xc + lVar25);
                  local_1450 = *(undefined4 *)(lVar16 + 0x10 + lVar25);
                  FUN_140409330(param_1,&local_1458,uVar19,fVar32);
                  lVar25 = *(longlong *)(lVar16 + 0x30 + *(longlong *)(lVar22 + 0xa10));
                  if (lVar25 != 0) {
                    local_1448 = 0x3f800000;
                    local_1434 = *(undefined4 *)(lVar25 + 8);
                    local_14b8 = _DAT_140908ee8;
                    uStack_14b0 = _UNK_140908ef0;
                    local_1438 = *(undefined4 *)(lVar25 + 4);
                    local_1430 = *(undefined4 *)(lVar25 + 0xc);
                    local_1444 = 0x3f800000;
                    local_1440 = 0x3f800000;
                    FUN_140409330(param_1,&local_1438);
                  }
                  uVar18 = uVar18 + 1;
                  uVar19 = (ulonglong)uVar18;
                } while (uVar19 < (ulonglong)
                                  ((*(longlong *)(lVar22 + 0xa18) - *(longlong *)(lVar22 + 0xa10)) /
                                  0x60));
              }
              lVar25 = *(longlong *)(lVar22 + 0x318);
              if ((lVar25 != 0) && (*(longlong *)(lVar25 + 0x290) == 0)) {
                pCVar10 = C3D_MIDDLEPOINT::CreateManagedTexture
                                    ((C3D_MIDDLEPOINT *)&DAT_1409eacd0,(char *)(lVar25 + 0x80));
                *(C3DAPI_TEXTURE **)(*(longlong *)(lVar25 + 0x318) + 0x290) = pCVar10;
                plVar24 = *(longlong **)(*(longlong *)(lVar25 + 0x318) + 0x290);
                (**(code **)(*plVar24 + 0x10))(plVar24,*(longlong *)(lVar25 + 0x318) + 0x80,0,0);
              }
              uVar18 = *(uint *)(puVar2 + 0x23c);
              if ((uVar18 < 0xb) && ((0x6e6U >> (uVar18 & 0x1f) & 1) != 0)) {
                FUN_14080cc60(param_1,puVar2);
              }
              else if (uVar18 - 3 < 2) {
                FUN_14070fee0(param_1,puVar2);
              }
              else if (uVar18 == 8) {
                FUN_140711d50(param_1,puVar2);
              }
              else if (uVar18 == 0xe) {
                FUN_140715a60(param_1,puVar2);
              }
              else if ((*(float *)(lVar22 + 0x604) <= fVar35 && fVar35 != *(float *)(lVar22 + 0x604)
                       ) || (*(longlong *)(lVar22 + 0x5d0) != 0)) {
                FUN_1406f8cc0(param_1,puVar2);
                if (*(char *)(lVar22 + 0xea8) == '\0') {
                  FUN_1407838e0(param_1,puVar2);
                }
                else {
                  FUN_14077c940();
                }
              }
              else if (*(char *)(lVar22 + 0x1198) == '\0') {
                uVar1 = *(uint *)(*(longlong *)(lVar22 + 0x318) + 0x360);
                if (uVar1 == 8) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14079e020(param_1,puVar2);
                }
                else if ((uVar1 == 0x2a) && (uVar18 == 0)) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407a0c90(param_1,puVar2);
                }
                else if ((uVar1 == 0xe) && (uVar18 == 0)) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407a9b20(param_1,puVar2);
                }
                else if ((uVar1 == 0xf) && (uVar18 == 0)) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407c6d70(param_1,puVar2);
                }
                else if (uVar1 == 0x2e) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14072c360(param_1,puVar2);
                }
                else if (uVar1 == 0x21) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14072c460(param_1,puVar2);
                }
                else if (uVar1 == 0xc) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407787b0(param_1,puVar2);
                }
                else if (uVar1 == 0x1c) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14077a6f0(param_1,puVar2);
                }
                else if (uVar1 == 0x6a) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14077f3c0(param_1,puVar2);
                }
                else if (uVar1 == 0x6d) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407815d0(param_1,puVar2);
                }
                else if (uVar1 == 5) {
                  FUN_1406f8cc0(param_1,puVar2);
                  if (*(int *)(*(longlong *)(lVar22 + 0x318) + 0x364) == 0x1d) {
                    FUN_14071c9b0();
                  }
                  else {
                    FUN_14071d290(param_1,puVar2);
                  }
                }
                else if (uVar1 == 0x22) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14071e800(param_1,puVar2);
                }
                else if (uVar1 == 0x60) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14071edd0(param_1,puVar2);
                }
                else if (uVar1 == 0x14) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14071b410(param_1,puVar2);
                }
                else if (uVar1 - 0x1f < 2) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140718fc0(param_1,puVar2);
                }
                else if (uVar1 == 0x6b) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14071a540(param_1,puVar2);
                }
                else if (uVar1 == 0x16) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140728a10(param_1,puVar2);
                }
                else if (uVar1 == 0x18) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140729c30(param_1,puVar2);
                }
                else if (uVar1 == 0x33) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14072b230(param_1,puVar2);
                }
                else if (uVar1 == 0x17) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140723950(param_1,puVar2);
                }
                else if (uVar1 == 0x15) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14075d310(param_1,puVar2);
                }
                else if (uVar1 == 3) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407272f0(param_1,puVar2);
                }
                else if (uVar1 == 4) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14072f2d0(param_1,puVar2);
                }
                else if (uVar1 == 0x27) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14072dbf0(param_1,puVar2);
                }
                else if (uVar1 == 0x1b) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407326c0(param_1,puVar2);
                }
                else if (uVar1 == 0x25) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140730cf0(param_1,puVar2);
                }
                else if (uVar1 == 0x19) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140734430(param_1,puVar2);
                }
                else if (uVar1 == 0x1a) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140755960(param_1,puVar2);
                }
                else if (uVar1 == 0x49) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14073c5a0(param_1,puVar2);
                }
                else if (uVar1 == 0x4b) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_1407362b0(param_1,puVar2);
                }
                else if (uVar1 == 0x4d) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14073aec0(param_1,puVar2);
                }
                else if (uVar1 == 0x4a) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140737f00(param_1,puVar2);
                }
                else if (uVar1 == 0x4c) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_140739700(param_1,puVar2);
                }
                else if (uVar1 == 10) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14075dcb0(param_1,puVar2);
                }
                else if (uVar1 == 0x5f) {
                  FUN_1406f8cc0(param_1,puVar2);
                  FUN_14078c1a0(param_1,puVar2);
                }
                else {
                  if ((uVar1 == 6) || (uVar1 - 0x61 < 2)) goto LAB_1406eb25c;
                  if (uVar1 == 0x28) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_140794fb0(param_1,puVar2);
                  }
                  else if (uVar1 == 0x29) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_140793130(param_1,puVar2);
                  }
                  else if (uVar1 == 0x24) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_14071fed0(param_1,puVar2);
                  }
                  else if (uVar1 == 0x26) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_1407a22c0(param_1,puVar2);
                  }
                  else if (uVar1 == 0x3c) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_1407217d0(param_1,puVar2);
                  }
                  else if (((uVar1 - 0x12 & 0xffffffee) == 0) && (uVar1 != 0x22)) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_140721c80(param_1,puVar2);
                  }
                  else if (uVar1 == 0x11) {
LAB_1406eb25c:
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_140799630(param_1,puVar2);
                  }
                  else if (uVar1 - 0x47 < 2) {
                    FUN_1406f8cc0(param_1,puVar2);
                    FUN_140723190(param_1,puVar2);
                  }
                  else {
                    if (uVar1 == 0x46) goto LAB_1406eb25c;
                    if (uVar1 == 0xb) {
                      FUN_1406f8cc0(param_1,puVar2);
                      if (*(int *)(*(longlong *)(lVar22 + 0x318) + 0x364) == 0x20) {
                        FUN_140792c50(param_1,puVar2);
                      }
                      else {
                        FUN_14078edc0();
                      }
                    }
                    else if (uVar1 == 7) {
LAB_1406eb371:
                      FUN_1406f8cc0(param_1,puVar2);
                      FUN_140786be0(param_1,puVar2);
                    }
                    else if (uVar1 == 0x5a) {
                      FUN_1406f8cc0(param_1,puVar2);
                      if (*(int *)(*(longlong *)(lVar22 + 0x318) + 0x364) == 0x23) {
                        FUN_140789f30();
                      }
                      else {
                        FUN_140789320(param_1,puVar2);
                      }
                    }
                    else if (uVar1 == 0x5b) {
                      FUN_1406f8cc0(param_1,puVar2);
                      FUN_14078a780(param_1,puVar2);
                    }
                    else if (uVar1 == 0x5e) {
                      FUN_1406f8cc0(param_1,puVar2);
                      FUN_14078ac90(param_1,puVar2);
                    }
                    else if (uVar1 == 0x5d) {
                      FUN_1406f8cc0(param_1,puVar2);
                      FUN_14078b4c0(param_1,puVar2);
                    }
                    else {
                      if (uVar1 == 0x5c) goto LAB_1406eb371;
                      if (uVar1 == 9) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_14079f550(param_1,puVar2);
                      }
                      else if (uVar1 == 2) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_140761300(param_1,puVar2);
                      }
                      else if (uVar1 == 0x50) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407214a0(param_1,puVar2);
                      }
                      else if (uVar1 == 0x32) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_140725130(param_1,puVar2);
                      }
                      else if ((uVar1 < 2) &&
                               ((*(int *)(*(longlong *)(lVar22 + 0x318) + 0x364) == 0x20)) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407c92a0(param_1,puVar2);
                      }
                      else if (uVar1 == 0) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407cdc40(param_1,puVar2);
                      }
                      else if (uVar1 == 1) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407cbb30(param_1,puVar2);
                      }
                      else if (uVar1 == 0x2d) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407496c0(param_1,puVar2);
                      }
                      else if (uVar1 == 0x2f) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_140754fd0(param_1,puVar2);
                      }
                      else if (uVar1 == 0x30) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1407534f0(param_1,puVar2);
                      }
                      else if (uVar1 == 0x2b) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_140741170(param_1,puVar2);
                      }
                      else if (uVar1 == 0x34) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_140745350(param_1,puVar2);
                      }
                      else if (uVar1 == 0x2c) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_14073fe90(param_1,puVar2);
                      }
                      else if (uVar1 == 0x31) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_14074c500(param_1,puVar2);
                      }
                      else if (uVar1 == 0x6c) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_14074f910(param_1,puVar2);
                      }
                      else if (uVar1 == 0x69) {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_14078cc00(param_1,puVar2);
                      }
                      else {
                        FUN_1406f8cc0(param_1,puVar2);
                        FUN_1406f4740(param_1,puVar2);
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else {
          if (*(int *)(puVar2 + 0x34) == 1) {
            FUN_1407f2660(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 9) {
            FUN_1407f2bd0();
          }
          if (*(int *)(puVar2 + 0x34) == 0x13) {
            FUN_1407fc3f0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 2) {
            FUN_1407f2d50(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x14) {
            FUN_1407fd2b0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x12) {
            FUN_1407f3490(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x19) {
            FUN_1407f4160(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1a) {
            FUN_1407fac60(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1b) {
            FUN_1407fa6c0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1c) {
            FUN_1407fb3d0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x25) {
            FUN_1407fba50(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x26) {
            FUN_1407fbf50(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x22) {
            FUN_1407f6b50(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x24) {
            FUN_1407f7bb0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 3) {
            FUN_1407fc880(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 4) {
            FUN_1407fe200(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1f) {
            FUN_1407fd810(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x20) {
            FUN_1407fdcd0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0xf) {
            FUN_1407fe6d0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x10) {
            FUN_1407fe6d0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0xe) {
            FUN_1407fece0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x11) {
            FUN_1407ff180(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 6) {
            FUN_1407ff780(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0xb) {
            FUN_1407ffda0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x15) {
            FUN_140800280(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x16) {
            FUN_14080b550(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1e) {
            FUN_14080bc80(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x18) {
            FUN_14080c510(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 7) {
            FUN_140800880(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 8) {
            FUN_140801300(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0xc) {
            FUN_140806fc0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x1d) {
            FUN_140807ae0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0xd) {
            FUN_1408075b0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 10) {
            FUN_140808060(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x23) {
            FUN_140807bf0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x17) {
            FUN_140808830(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x27) {
            FUN_1408030f0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x28) {
            FUN_140804740(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x29) {
            FUN_1408054a0(param_1,puVar2);
          }
          if (*(int *)(puVar2 + 0x34) == 0x2a) {
            FUN_140804e80();
          }
          if (*(int *)(puVar2 + 0x34) == 0x2b) {
            FUN_140803ac0(param_1,puVar2);
          }
          if ((*(int *)(puVar2 + 0x34) == 0x21) && (*(int *)(puVar2 + 0x248) == 0)) {
            FUN_1406f8cc0(param_1,puVar2);
            FUN_1407838e0(param_1,puVar2);
          }
        }
      }
LAB_1406eb642:
      plVar24 = (longlong *)0x0;
      C3D_PANEL2D::EndDraw((C3D_PANEL2D *)&DAT_1409be060);
      C3D_FONTMANAGER::PrintAllTexts((C3D_FONTMANAGER *)&DAT_140996fb0,"Basic");
      C3D_PANEL2D::BeginDraw((C3D_PANEL2D *)&DAT_1409be060,(char *)0x0,false);
      C3D_PANEL2D::SetClipping((C3D_PANEL2D *)&DAT_1409be060,0.0,(float)DAT_140995274,0.0,0.0);
      if (*(longlong *)(param_1 + 52000) != 0) {
        cVar7 = FUN_1407b7fa0(param_1);
        if (cVar7 != '\0') {
          fVar28 = 0.0;
          fVar29 = 0.0;
          if (DAT_14099528c < local_1490) {
            iVar9 = DAT_14099528c - local_1490;
LAB_1406eb709:
            fVar28 = (float)iVar9;
          }
          else if (local_1498 < 0) {
            iVar9 = -local_1498;
            goto LAB_1406eb709;
          }
          if (DAT_140995274 < local_148c) {
            iVar9 = DAT_140995274 - local_148c;
LAB_1406eb72f:
            fVar29 = (float)iVar9;
          }
          else if (local_1494 < 0) {
            iVar9 = -local_1494;
            goto LAB_1406eb72f;
          }
          if ((fVar28 != 0.0) || (fVar29 != 0.0)) {
            local_1498 = local_1498 + (int)fVar28;
            local_1490 = local_1490 + (int)fVar28;
            local_1494 = local_1494 + (int)fVar29;
            local_148c = local_148c + (int)fVar29;
          }
          *(float *)(param_1 + 0xcaf4) = (float)local_1498;
          *(float *)(param_1 + 0xcaf8) = (float)local_1494;
        }
        FUN_1407b7fa0(param_1);
      }
      if (*(longlong *)(param_1 + 0xcae8) != 0) {
        FUN_1407b7960(param_1);
      }
      if ((((*(longlong *)(param_1 + 0xd428) == 0) && (*(int *)(puVar2 + 0x4f4) == 0)) &&
          (puVar2[0x4f0] != '\0')) &&
         (((*(char *)(param_1 + 0xfbb) == '\0' &&
            (((((DAT_140a54e91 != '\0' || (*(char *)(param_1 + 0xcc18) != '\0')) ||
               (*(char *)(param_1 + 0xcc19) != '\0')) &&
              (((*(char *)(param_1 + 0xfbc) == '\0' &&
                ((*(char *)(param_1 + 0xfc1) == '\0' || (*(char *)(param_1 + 0xcc19) != '\0')))) &&
               (*(char *)(param_1 + 0xfbe) == '\0')))) &&
             (((*(char *)(param_1 + 0xfc2) == '\0' && (*(char *)(param_1 + 0xfc3) == '\0')) &&
              (*(char *)(param_1 + 0xfc4) == '\0')))))) &&
           (((*(longlong *)(puVar2 + 0x240) == 0 || (*(int *)(puVar2 + 0x248) != 8)) ||
            ((-1 < *(int *)(puVar2 + 0x288) ||
             ((*(longlong *)(puVar2 + 0x270) != 0 && (cVar7 = FUN_140493980(), cVar7 != '\0')))))))))
         ) {
        *puVar2 = 1;
      }
      if ((((puVar2[0x10] != '\0') && (DAT_140a54e93 != '\0')) && (puVar2[0x20] != '\0')) &&
         (1 < *(int *)(puVar2 + 0x34) - 0x27U)) {
        if (*(float *)(puVar2 + 0x24) <= DAT_140909d64 && DAT_140909d64 != *(float *)(puVar2 + 0x24)
           ) {
          *puVar2 = 1;
        }
        *(undefined4 *)(puVar2 + 0x24) = 0;
      }
      fVar28 = C3D_TIMER::PowerTime((C3D_TIMER *)&DAT_1409d4ee0,DAT_140909afc,true,true);
      *(float *)(puVar2 + 0x24) = fVar28 + *(float *)(puVar2 + 0x24);
      if (*(int *)(puVar2 + 0x4f4) != 0) {
        *(int *)(puVar2 + 0x4f4) = *(int *)(puVar2 + 0x4f4) + -1;
      }
      C3D_FONTMANAGER::SetReallyRender((C3D_FONTMANAGER *)&DAT_140996fb0,true);
      C3D_PANEL2D::SetReallyRender((C3D_PANEL2D *)&DAT_1409be060,true);
      uVar6 = _UNK_14090c468;
      uVar5 = _DAT_14090c460;
      if ((*(char *)(param_1 + 9) != '\0') && (*(int *)(puVar2 + 0x248) == 0)) {
        lVar22 = *(longlong *)(puVar2 + 0x240);
        local_1298 = lVar22;
        if (((lVar22 != 0) && ((DAT_140a54bad != '\0' && (DAT_140a54bbf != '\0')))) &&
           (lVar16 = *(longlong *)(lVar22 + 0x978) - *(longlong *)(lVar22 + 0x970), local_14a0 = 0,
           lVar25 = lVar16 >> 0x3f,            lVar16 / 0xe0 + lVar25 != lVar25)) {
          local_1474 = 0x3f800000;
          local_1470 = 0x3f800000;
          do {
            uVar4 = _UNK_14090c388;
            uVar3 = _DAT_14090c380;
            uVar19 = 0;
            lVar25 = (longlong)plVar24 * 0xe0 + *(longlong *)(lVar22 + 0x970);
            if (*(longlong *)(lVar25 + 0xc0) != 0) {
              lVar17 = *(longlong *)(lVar25 + 0xd0) - *(longlong *)(lVar25 + 200);
              lVar16 = lVar17 >> 0x3f;
              if (lVar17 / 0xbc + lVar16 != lVar16) {
                local_1428 = 0x3f800000;
                local_1424 = 0x3f800000;
                local_1420 = 0x3f800000;
                local_1418 = 0x3f800000;
                local_1414 = 0x3f800000;
                local_1410 = 0x3f800000;
                local_1408 = 0x3f800000;
                local_1404 = 0x3f800000;
                local_1400 = 0x3f800000;
                local_13f8 = 0x3f800000;
                local_13f4 = 0x3f800000;
                local_13f0 = 0x3f800000;
                do {
                  pCVar20 = (C3D_NODE *)(uVar19 * 0xbc + *(longlong *)(lVar25 + 200));
                  lVar16 = C3D_NODE::GetScale(pCVar20);
                  fVar35 = *(float *)(lVar16 + 8);
                  pfVar11 = (float *)C3D_NODE::GetScale(pCVar20);
                  fVar28 = *pfVar11;
                  pfVar11 = (float *)C3D_NODE::GetPosition(pCVar20);
                  local_14d0 = pfVar11[2] - fVar35;
                  local_14d8 = CONCAT44(pfVar11[1] - 0.0,*pfVar11 - fVar28);
                  local_1198 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix((C3D_NODE *)(lVar22 + 800));
                  C3DVec3TransformCoord((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12)
                  ;
                  local_1288 = local_14d0;
                  local_14b8 = uVar3;
                  uStack_14b0 = uVar4;
                  local_1290 = local_14d8;
                  FUN_140409330(param_1,&local_1290);
                  lVar16 = C3D_NODE::GetScale(pCVar20);
                  fVar35 = *(float *)(lVar16 + 8);
                  pfVar11 = (float *)C3D_NODE::GetScale(pCVar20);
                  fVar28 = *pfVar11;
                  pfVar11 = (float *)C3D_NODE::GetPosition(pCVar20);
                  local_14d0 = fVar35 + pfVar11[2];
                  local_14d8 = CONCAT44(pfVar11[1] + 0.0,fVar28 + *pfVar11);
                  local_118c = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix((C3D_NODE *)(lVar22 + 800));
                  C3DVec3TransformCoord((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12)
                  ;
                  local_1278 = local_14d0;
                  local_14b8 = uVar3;
                  uStack_14b0 = uVar4;
                  local_1280 = local_14d8;
                  FUN_140409330(param_1,&local_1280);
                  lVar16 = C3D_NODE::GetScale(pCVar20);
                  fVar28 = (float)(*(uint *)(lVar16 + 8) ^ DAT_14090c720);
                  pfVar11 = (float *)C3D_NODE::GetScale(pCVar20);
                  fVar35 = *pfVar11;
                  pfVar11 = (float *)C3D_NODE::GetPosition(pCVar20);
                  local_14d0 = fVar28 + pfVar11[2];
                  local_14d8 = CONCAT44(pfVar11[1] + 0.0,fVar28 + *pfVar11);
                  local_1228 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix((C3D_NODE *)(lVar22 + 800));
                  C3DVec3TransformCoord((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12)
                  ;
                  local_1340 = local_14d0;
                  local_14b8 = uVar3;
                  uStack_14b0 = uVar4;
                  local_1348 = local_14d8;
                  FUN_140409330(param_1,&local_1348);
                  lVar16 = C3D_NODE::GetScale(pCVar20);
                  fVar35 = *(float *)(lVar16 + 8);
                  puVar13 = (uint *)C3D_NODE::GetScale(pCVar20);
                  fVar28 = (float)(*puVar13 ^ DAT_14090c720);
                  pfVar11 = (float *)C3D_NODE::GetPosition(pCVar20);
                  local_14d0 = fVar35 + pfVar11[2];
                  local_14d8 = CONCAT44(pfVar11[1] + 0.0,fVar28 + *pfVar11);
                  local_121c = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix((C3D_NODE *)(lVar22 + 800));
                  C3DVec3TransformCoord((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12)
                  ;
                  local_14b8 = uVar3;
                  uStack_14b0 = uVar4;
                  local_1330 = local_14d0;
                  local_1338 = local_14d8;
                  FUN_140409330(param_1,&local_1338);
                  uVar19 = (ulonglong)((int)uVar19 + 1);
                } while (uVar19 < (ulonglong)
                                  ((*(longlong *)(lVar25 + 0xd0) - *(longlong *)(lVar25 + 200)) /
                                  0xbc));
              }
              uVar4 = _UNK_14090c2d8;
              uVar3 = _DAT_14090c2d0;
              uVar18 = 0;
              plVar24 = (longlong *)(*(longlong *)(lVar25 + 0xc0) + 0x48);
              lVar16 = *(longlong *)(*(longlong *)(lVar25 + 0xc0) + 0x50) - *plVar24;
              iVar21 = (int)(lVar16 >> 0x3f);
              if ((int)(lVar16 / 0x28) + iVar21 != iVar21) {
                pCVar20 = (C3D_NODE *)(lVar22 + 800);
                local_13e8 = 0x3f800000;
                local_13e4 = 0x3f800000;
                local_13e0 = 0x3f800000;
                do {
                  lVar22 = *plVar24;
                  lVar16 = (longlong)(int)uVar18;
                  local_14d0 = *(float *)(lVar22 + 8 + lVar16 * 0x28) * fVar32 +
                               *(float *)(lVar22 + 0x1c + lVar16 * 0x28);
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0xc + lVar16 * 0x28) * fVar32 +
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28));
                  local_1210 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12)
                  ;
                  local_1320 = local_14d0;
                  local_14b8 = uVar3;
                  uStack_14b0 = uVar4;
                  local_1328 = local_14d8;
                  FUN_140409330(param_1,&local_1328);
                  local_14d8 = *(undefined8 *)(lVar22 + 0x14 + lVar16 * 0x28);
                  local_14d0 = *(float *)(lVar22 + 0x1c + lVar16 * 0x28);
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0xc + lVar16 * 0x28) +
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28));
                  local_11e0 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  local_12c0 = local_14d0;
                  local_14b8 = _DAT_14090c460;
                  uStack_14b0 = _UNK_14090c468;
                  local_1388 = 0x3f800000;
                  local_1384 = 0x3f800000;
                  local_1380 = 0x3f800000;
                  local_12c8 = local_14d8;
                  FUN_140409330(param_1,&local_12c8);
                  local_14d0 = *(float *)(lVar22 + 0x1c + lVar16 * 0x28) + 0.0;
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 8 + lVar16 * 0x28) +
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28));
                  local_11d4 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  local_14b8 = _DAT_14090c460;
                  uStack_14b0 = _UNK_14090c468;
                  local_12b0 = local_14d0;
                  local_1378 = 0x3f800000;
                  local_1374 = 0x3f800000;
                  local_1370 = 0x3f800000;
                  local_12b8 = local_14d8;
                  FUN_140409330(param_1,&local_12b8);
                  local_14d0 = *(float *)(lVar22 + 0xc + lVar16 * 0x28) +
                               *(float *)(lVar22 + 0x1c + lVar16 * 0x28);
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28) + 0.0);
                  local_11c8 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  puVar14 = &local_12a8;
                  local_12a0 = local_14d0;
                  local_12a8 = local_14d8;
                  local_1368 = 0x3f800000;
                  local_1364 = 0x3f800000;
                  local_1360 = 0x3f800000;
                }
                else {
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  local_1310 = local_14d0;
                  local_14b8 = _DAT_14090c460;
                  uStack_14b0 = _UNK_14090c468;
                  local_13d8 = 0x3f800000;
                  local_13d4 = 0x3f800000;
                  local_13d0 = 0x3f800000;
                  local_1318 = local_14d8;
                  FUN_140409330(param_1,&local_1318);
                  local_14d0 = *(float *)(lVar22 + 8 + lVar16 * 0x28) +
                               *(float *)(lVar22 + 0x1c + lVar16 * 0x28);
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0xc + lVar16 * 0x28) +
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28));
                  local_1204 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  local_1300 = local_14d0;
                  local_14b8 = _DAT_14090c460;
                  uStack_14b0 = _UNK_14090c468;
                  local_13c8 = 0x3f800000;
                  local_13c4 = 0x3f800000;
                  local_13c0 = 0x3f800000;
                  local_1308 = local_14d8;
                  FUN_140409330(param_1,&local_1308);
                  local_14d0 = *(float *)(lVar22 + 0x1c + lVar16 * 0x28) + 0.0;
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0xc + lVar16 * 0x28) +
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28));
                  local_11f8 = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  local_14b8 = _DAT_14090c460;
                  uStack_14b0 = _UNK_14090c468;
                  local_12f0 = local_14d0;
                  local_13b8 = 0x3f800000;
                  local_13b4 = 0x3f800000;
                  local_13b0 = 0x3f800000;
                  local_12f8 = local_14d8;
                  FUN_140409330(param_1,&local_12f8);
                  local_14d0 = *(float *)(lVar22 + 8 + lVar16 * 0x28) +
                               *(float *)(lVar22 + 0x1c + lVar16 * 0x28);
                  local_14d8 = CONCAT44(*(float *)(lVar22 + 0x18 + lVar16 * 0x28) + 0.0,
                                        *(float *)(lVar22 + 0x14 + lVar16 * 0x28) + 0.0);
                  local_11ec = local_14d0;
                  pCVar12 = (C3DMATRIX *)C3D_NODE::GetMatrix(pCVar20);
                  C3DVec3TransformCoord
                            ((C3DVECTOR3 *)&local_14d8,(C3DVECTOR3 *)&local_14d8,pCVar12);
                  puVar14 = &local_12e8;
                  local_12e0 = local_14d0;
                  local_12e8 = local_14d8;
                  local_13a8 = 0x3f800000;
                  local_13a4 = 0x3f800000;
                  local_13a0 = 0x3f800000;
                }
                local_14b8 = _DAT_14090c460;
                uStack_14b0 = _UNK_14090c468;
                FUN_140409330(param_1,puVar14);
                plVar24 = (longlong *)(*(longlong *)(lVar25 + 0xc0) + 0x48);
                uVar18 = uVar18 + 1;
                lVar22 = local_1298;
              } while (uVar18 < (uint)((*(longlong *)(*(longlong *)(lVar25 + 0xc0) + 0x50) -
                                       *plVar24) / 0x28));
            }
          }
          local_10e8 = uVar5;
          uStack_10e0 = uVar6;
          FUN_140409330(param_1,&local_14b8,plVar24,fVar32);
          local_14a0 = local_14a0 + 1;
          plVar24 = (longlong *)(ulonglong)local_14a0;
          fVar35 = DAT_140909f58;
          } while (plVar24 < (longlong *)
                               ((*(longlong *)(lVar22 + 0x978) - *(longlong *)(lVar22 + 0x970)) / 0xe0
                               ));
        }
        iVar21 = (int)local_14a8;
      }
      lVar22 = local_1480;
      lVar25 = *(longlong *)(*(longlong *)(param_1 + 0xcac0) + local_1480 * 8);
      if (((*(longlong *)(param_1 + 0xcb48) == lVar25) ||
          ((*(longlong *)(param_1 + 0xcb50) == lVar25 && (*(int *)(lVar25 + 0x34) != 0x28)))) &&
         (*(int *)(param_1 + 0xade8) != 0)) {
        fStack_134c = DAT_140909e10;
        if (*(longlong *)(param_1 + 0xcb48) == lVar25) {
          fStack_134c = fVar35;
        }
        local_1358 = 0x3f000000;
        uStack_1354 = 0x3ee66666;
        uStack_1350 = 0x3e800000;
        _DAT_1409be30c = 0x3ee666663f000000;
        uRam00000001409be314 = CONCAT44(fStack_134c,0x3e800000);
        (**(code **)(**(longlong **)(param_1 + 0xb790) + 0x70))
                  (*(longlong **)(param_1 + 0xb790),0,DAT_1409ead08);
        local_11bc = 0;
        _DAT_1409be2ec =
             *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xcac0) + lVar22 * 8) + 0x520) -
             DAT_140992088 * DAT_14090a9cc;
        _DAT_1409be308 = 0x4096cbe4;
        _DAT_1409be2e8 = DAT_140992088 * DAT_14090a928;
        lVar25 = *(longlong *)(*(longlong *)(param_1 + 0xcac0) + lVar22 * 8);
        DAT_1409be2f8 = 0;
        _DAT_1409be2f0 =
             CONCAT44((*(float *)(lVar25 + 0x2c) + *(float *)(lVar25 + 8)) -
                      DAT_140992088 * DAT_14090aa88,*(float *)(lVar25 + 4) - DAT_140992088 * 0.0);
        C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar35,fVar35,0.0,true);
        _DAT_1409be308 = 0x3fc90fdb;
        local_11b0 = 0;
        lVar25 = *(longlong *)(*(longlong *)(param_1 + 0xcac0) + lVar22 * 8);
        DAT_1409be2f8 = 0;
        _DAT_1409be2f0 =
             CONCAT44((*(float *)(lVar25 + 0x2c) + *(float *)(lVar25 + 8)) - DAT_140992088,
                      *(float *)(lVar25 + 4) - DAT_140992088 * 0.0);
        C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar35,fVar35,0.0,true);
        _DAT_1409be308 = 0;
      }
      local_14a8 = (ulonglong)(iVar21 - 1U);
      lVar22 = lVar22 + -1;
      local_1480 = lVar22;
    } while (-1 < (int)(iVar21 - 1U));
  }
  uVar19 = 0;
  if ((*(wchar_t *)(param_1 + 0xd5a0) == L'\0') && (*(char *)(param_1 + 0xfc3) == '\0')) {
    wcscpy_s((wchar_t *)(param_1 + 0xd5a0),0x800,local_10d8);
  }
  fVar28 = DAT_14090aa44;
  fVar32 = DAT_14090a968;
  if ((*(longlong *)(param_1 + 0x117a8) != 0) &&
     (*(char *)(*(longlong *)(param_1 + 0x13bf0) + 0x14) == '\0')) {
    local_10f8 = 0;
    iStack_10f0 = (int)(DAT_140992088 * DAT_14090aa44 + 0.0);
    iStack_10ec = (int)(DAT_140992088 * DAT_14090a968 + 0.0);
    lVar22 = *DAT_140994200;
    C3D_LANGUAGE::GetString
              ((C3D_LANGUAGE *)&DAT_140997590,*(int *)(*(longlong *)(param_1 + 0x117a8) + 0x58));
    puVar14 = (undefined8 *)(**(code **)(lVar22 + 0x28))(DAT_140994200,&local_10e8,0,0);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    fVar29 = DAT_14090a9a0;
    fVar27 = DAT_140992088 * DAT_14090a9a0;
    lVar22 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x2b00);
    puVar14 = (undefined8 *)(**(code **)(lVar22 + 0x28))(DAT_140995220,&local_10e8,0,0);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    lVar22 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x209);
    puVar14 = (undefined8 *)
              (**(code **)(lVar22 + 0x28))
                        (DAT_140995220,&local_10e8,(float)(int)uStack_1108 + 0.0,fVar27 + 0.0);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    C3D_LANGUAGE::GetString
              ((C3D_LANGUAGE *)&DAT_140997590,*(int *)(*(longlong *)(param_1 + 0x117a8) + 0x5c));
    FUN_1407ab9b0();
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    pfVar11 = (float *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    fVar34 = DAT_140992088 * fVar29 + *pfVar11;
    lVar22 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    fVar31 = 0.0;
    fVar30 = 0.0;
    fVar33 = DAT_140992088 * fVar29 + *(float *)(lVar22 + 4);
    fVar27 = DAT_140992088 * DAT_14090a828 + fVar34 + (float)iStack_10f0;
    if ((float)DAT_14099528c < fVar27) {
      fVar31 = fVar27 - (float)DAT_14099528c;
    }
    fVar27 = DAT_140992088 * DAT_14090a828 + fVar33 + (float)iStack_10ec;
    if ((float)DAT_140995274 < fVar27) {
      fVar30 = fVar27 - (float)DAT_140995274;
    }
    fVar34 = fVar34 - fVar31;
    fVar33 = fVar33 - fVar30;
    local_10f8 = CONCAT44((int)fVar33,(int)fVar34);
    fVar27 = DAT_140992088 * 0.0;
    iStack_10f0 = (int)(DAT_140992088 * fVar28 + (float)(int)fVar34);
    iStack_10ec = (int)(DAT_140992088 * fVar32 + (float)(int)fVar33);
    C3D_LANGUAGE::GetString
              ((C3D_LANGUAGE *)&DAT_140997590,*(int *)(*(longlong *)(param_1 + 0x117a8) + 0x58));
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994200,fVar27 + fVar33,
               -1.7014118e+38,0x1408f7018,(wchar_t *)(ulonglong)(uint)(fVar27 + fVar34));
    lVar22 = *DAT_140994200;
    C3D_LANGUAGE::GetString
              ((C3D_LANGUAGE *)&DAT_140997590,*(int *)(*(longlong *)(param_1 + 0x117a8) + 0x58));
    puVar14 = (undefined8 *)(**(code **)(lVar22 + 0x28))(DAT_140994200,&local_10e8,fVar34,fVar33);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    fVar32 = DAT_140992088 * 0.0;
    fVar33 = fVar33 + DAT_140992088 * fVar29;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x2b00);
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,fVar32 + fVar33,-NAN,
               0x1408f6fb8,(wchar_t *)(ulonglong)(uint)(fVar32 + fVar34));
    lVar22 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x2b00);
    puVar14 = (undefined8 *)(**(code **)(lVar22 + 0x28))(DAT_140995220,&local_10e8,0,0);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x209);
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
               DAT_140992088 * 0.0 + fVar33,-1.7014118e+38,0x1408f6fd8,
               (wchar_t *)(ulonglong)(uint)((float)(int)uStack_1108 + fVar34));
    lVar22 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x209);
    puVar14 = (undefined8 *)
              (**(code **)(lVar22 + 0x28))
                        (DAT_140995220,&local_10e8,(float)(int)uStack_1108 + fVar34,fVar33);
    local_1110 = *puVar14;
    uStack_1108 = puVar14[1];
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    C3D_LANGUAGE::GetString
              ((C3D_LANGUAGE *)&DAT_140997590,*(int *)(*(longlong *)(param_1 + 0x117a8) + 0x5c));
    FUN_1407aae80();
    C3DRectExtend((C3DRECT *)&local_10f8,(C3DRECT *)&local_1110);
    (**(code **)(**(longlong **)(param_1 + 0xb038) + 0x70))
              (*(longlong **)(param_1 + 0xb038),0,DAT_1409ead08);
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    uStack_10e0 = CONCAT44(iStack_10ec,iStack_10f0);
    local_10e8 = local_10f8;
    C3D_PANEL2D::DrawFrom9Patch
              ((C3D_PANEL2D *)&DAT_1409be060,&local_10e8,DAT_140992088 * fVar29,
               DAT_140992088 * fVar29);
  }
  if (((*(char *)(param_1 + 0xfc3) != '\0') &&
      (((lVar22 = *(longlong *)(param_1 + 0x13bf0), lVar22 == 0 ||
        (*(char *)(lVar22 + 0x15) == '\0')) || (*(char *)(lVar22 + 0x14) == '\0')))) &&
     ((DAT_140a54e91 != '\0' &&
      (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0)))) {
    do {
      lVar22 = *(longlong *)(*(longlong *)(param_1 + 0xcac0) + uVar19 * 8);
      if ((*(char *)(lVar22 + 0x10) != '\0') && (DAT_140909d64 < *(float *)(lVar22 + 0x18))) {
        if ((*(char *)(param_1 + 0x14) == '\0') &&
           (bVar8 = C3D_CONFIGURATOR::GetValueExist
                                 ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_window_change"), bVar8))
        {
          pcVar15 = C3D_CONFIGURATOR::GetValueString
                                ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_window_change");
          C3D_SOUNDMANAGER::PlaySound2D
                    (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar15,true,fVar35,(char *)0x0);
        }
        FUN_1406ed120(param_1,uVar19);
        return;
      }
      uVar19 = (ulonglong)((int)uVar19 + 1);
    } while (uVar19 < (ulonglong)
                          (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
  }
  return;
}

---

## Raw Artifact: scratch/panel_745350_final.c

Source path: `scratch/panel_745350_final.c` | Bytes: 79895 | SHA-256: `6DB7AD084C686263AB04653769947BA6756DC95C3E7FED2BA0CFDD042B12DEBE`
Lines: 1864 | `FUN_140745350` rail builder (VA 0x140745350)

```c

// ============================================================
// Function: FUN_140745350 at 140745350
// Target address checked: 0x140745350

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140745350(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  wchar_t wVar2;
  undefined4 *puVar3;
  longlong lVar4;
  longlong lVar5;
  float fVar6;
  double dVar7;
  double dVar8;
  float fVar9;
  void *_Memory;
  uint uVar10;
  char cVar11;
  bool bVar12;
  char cVar13;
  undefined1 uVar14;
  short sVar15;
  int iVar16;
  ulonglong uVar17;
  wchar_t *pwVar18;
  ulonglong *puVar19;
  longlong lVar20;
  undefined8 uVar21;
  char *pcVar22;
  undefined4 *puVar23;
  float *pfVar24;
  undefined8 uVar25;
  longlong lVar26;
  short *psVar27;
  short *psVar28;
  longlong *plVar29;
  longlong lVar30;
  int iVar31;
  undefined1 *puVar32;
  longlong *plVar33;
  void **ppvVar34;
  longlong lVar35;
  uint uVar36;
  ulonglong uVar37;
  uint uVar38;
  ulonglong uVar39;
  uint *puVar40;
  ulonglong uVar41;
  uint *puVar42;
  ulonglong uVar43;
  float fVar44;
  float fVar45;
  float fVar46;
  double dVar47;
  float fVar48;
  float fVar49;
  float fVar50;
  float fVar51;
  undefined4 uVar52;
  undefined4 uVar53;
  undefined4 uVar54;
  undefined4 uVar55;
  float fVar56;
  undefined4 uVar57;
  undefined4 uVar58;
  undefined4 uVar59;
  undefined4 uVar60;
  undefined4 uVar61;
  undefined4 uVar62;
  undefined4 uVar63;
  undefined4 uVar64;
  undefined1 auStackY_f78 [32];
  float local_ee8;
  float local_ee4;
  char local_ee0;
  uint local_edc;
  char local_ed8;
  uint local_ed4;
  uint local_ed0 [2];
  ulonglong local_ec8;
  undefined4 local_ec0;
  float local_eb8;
  ulonglong local_eb0;
  uint *local_ea8;
  uint *local_ea0;
  uint *puStack_e98;
  uint *local_e90;
  longlong local_e88;
  double local_e80;
  undefined4 local_e70;
  ulonglong local_e68;
  ulonglong uStack_e60;
  longlong *local_e58;
  longlong *plStack_e50;
  longlong *local_e48;
  ulonglong local_e40;
  undefined4 local_e30;
  void *local_e28;
  ulonglong uStack_e20;
  longlong local_e18;
  longlong lStack_e10;
  ulonglong local_e08;
  longlong local_e00;
  longlong lStack_df8;
  longlong local_df0;
  void *local_de8;
  undefined8 uStack_de0;
  longlong local_dd8;
  ulonglong local_dd0;
  longlong local_dc8;
  longlong local_dc0;
  longlong local_db8;
  longlong lStack_db0;
  longlong local_da8;
  void *local_d98;
  ulonglong uStack_d90;
  undefined4 local_d80;
  undefined4 local_d74;
  undefined4 local_d68;
  undefined1 local_d60 [16];
  undefined1 local_d50 [16];
  undefined8 local_d40;
  void *local_d08;
  undefined8 uStack_d00;
  undefined1 local_cf8 [14];
  short sStack_cea;
  wchar_t local_ce8 [2];
  short local_ce4 [1022];
  undefined1 local_4e8 [512];
  undefined1 local_2e8 [512];
  ulonglong local_e8;
  
  local_d40 = 0xfffffffffffffffe;
  local_e8 = DAT_140992048 ^ (ulonglong)auStackY_f78;
  if (*(char *)(param_2 + 1) != '\0') {
    return;
  }
  local_e00 = 0;
  lStack_df8 = 0;
  uVar43 = 0;
  local_dc0 = 0;
  local_df0 = 0;
  local_ea0 = (uint *)0x0;
  puStack_e98 = (uint *)0x0;
  local_ea8 = (uint *)0x0;
  local_e90 = (uint *)0x0;
  local_ed0[0] = 0;
  local_de8 = (void *)0x0;
  uStack_de0 = 0;
  local_dd8 = 0;
  local_e88 = param_2;
  FUN_1400cce00(&local_de8,0,local_ed0);
  fVar9 = DAT_14090ab84;
  fVar45 = DAT_14090a9f0;
  fVar46 = DAT_14090a9cc;
  local_ec8 = 0;
  local_dd0 = 0;
  local_db8 = 0;
  lStack_db0 = 0;
  local_da8 = 0;
  fVar51 = (*(float *)(param_2 + 4) - DAT_140992088 * DAT_14090a6a8) + *(float *)(param_2 + 0x28) +
           DAT_140992088 * DAT_14090a9cc;
  fVar48 = *(float *)(param_2 + 8) + *(float *)(param_2 + 0x2c);
  local_ee8 = DAT_140992088 * DAT_14090a9f0 + fVar48;
  uVar17 = (ulonglong)(uint)local_ee8;
  fVar48 = DAT_140992088 * DAT_14090ab84 + fVar48;
  lVar35 = *(longlong *)(param_2 + 0x240);
  local_ee4 = fVar51;
  local_eb8 = fVar51;
  local_dc8 = lVar35;
  if (((*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0) &&
      (param_2 == **(longlong **)(param_1 + 0xcac0))) &&
     (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 != 0)) {
    uVar37 = uVar43;
    uVar41 = uVar43;
    do {
      lVar20 = *(longlong *)(uVar41 + *(longlong *)(lVar35 + 0xd38));
      lVar26 = *(longlong *)(lVar20 + 8);
      if (((longlong *)(param_1 + 0x11dc0) != (longlong *)0x0) &&
         (lVar30 = *(longlong *)(param_1 + 0x11dc0),
         *(longlong *)(param_1 + 0x11dc8) - lVar30 >> 3 != 0)) {
        uVar39 = uVar43;
        do {
          if (lVar26 == *(longlong *)(lVar30 + uVar39 * 8)) {
            pcVar22 = *(char **)(lVar20 + 0x10);
            if (((*pcVar22 == '\0') ||
                ((local_d08 = _DAT_140908e68, uStack_d00 = _UNK_140908e70, pcVar22[200] == '\0' &&
                 (local_d08 = _DAT_140908e08, uStack_d00 = _UNK_140908e10, *pcVar22 == '\0')))) &&
               (local_d08 = _DAT_140908de8, uStack_d00 = _DAT_140908df0, pcVar22[200] != '\0')) {
              local_d08 = _DAT_140908df8;
              uStack_d00 = _UNK_140908e00;
            }
            uVar17 = FUN_14040c550(param_1,lVar26,0,&local_d08);
            break;
          }
          uVar39 = (ulonglong)((int)uVar39 + 1);
        } while (uVar39 < (ulonglong)(*(longlong *)(param_1 + 0x11dc8) - lVar30 >> 3));
      }
      uVar38 = (int)uVar37 + 1;
      uVar37 = (ulonglong)uVar38;
      uVar41 = uVar41 + 8;
    } while ((ulonglong)(longlong)(int)uVar38 <
             (ulonglong)(*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3));
  }
  local_ed8 = '\0';
  uVar37 = uVar43;
  if (*(longlong *)(lVar35 + 0xc78) - *(longlong *)(lVar35 + 0xc70) >> 3 != 0) {
    do {
      lVar20 = *(longlong *)(*(longlong *)(lVar35 + 0xc70) + uVar37 * 8);
      if ((((lVar20 != 0) && (lVar20 = *(longlong *)(lVar20 + 0x1708), lVar20 != 0)) &&
          (*(int *)(lVar20 + 0x294) == 4)) && (*(float *)(lVar20 + 0x8684) <= 0.0)) {
        local_ed8 = '\x01';
        break;
      }
      uVar37 = (ulonglong)((int)uVar37 + 1);
    } while (uVar37 < (ulonglong)
                      (*(longlong *)(lVar35 + 0xc78) - *(longlong *)(lVar35 + 0xc70) >> 3));
  }
  uVar38 = 0;
  local_ed0[0] = 0;
  plVar29 = &local_db8;
  FUN_1403f8bb0(uVar17,lVar35);
  lVar20 = local_db8;
  uVar37 = 0;
  local_edc = 0;
  local_e40 = lStack_db0 - local_db8;
  uVar17 = (longlong)local_e40 >> 3;
  local_eb0 = uVar17;
  if (uVar17 != 0) {
    do {
      plVar33 = (longlong *)0x0;
      uVar36 = (uint)uVar37;
      lVar35 = *(longlong *)(lVar20 + uVar37 * 8);
      local_ee0 = '\x01';
      if (((*(longlong *)(lVar35 + 0x1708) != 0) &&
          (*(int *)(*(longlong *)(lVar35 + 0x1708) + 0x8600) == 0x11)) &&
         (local_ee0 = '\0',
         *(longlong *)(param_1 + 0x12068) - *(longlong *)(param_1 + 0x12060) >> 3 != 0)) {
        do {
          FUN_1400f9a00(*(undefined8 *)(*(longlong *)(param_1 + 0x12060) + (longlong)plVar33 * 8),
                        &local_e00);
          plVar33 = (longlong *)(ulonglong)((int)plVar33 + 1);
          plVar29 = plVar33;
          uVar36 = local_edc;
        } while (plVar33 < (longlong *)
                           (*(longlong *)(param_1 + 0x12068) - *(longlong *)(param_1 + 0x12060) >> 3
                           ));
      }
      uVar43 = 0;
      cVar13 = local_ee0;
      uVar10 = local_ed0[0];
      if (*(longlong *)(lVar35 + 0x3c0) - *(longlong *)(lVar35 + 0x3b8) >> 4 != 0) {
        do {
          plVar33 = (longlong *)(uVar43 * 0x10 + *(longlong *)(lVar35 + 0x3b8));
          if ((cVar13 != '\0') &&
             (lVar26 = *plVar33, cVar13 = local_ee0, -1 < *(int *)(lVar26 + 0x44))) {
            FUN_1400f9a00(lVar26,&local_e00);
            cVar13 = local_ee0;
          }
          if ((*(longlong *)(lVar35 + 0x398) == 0) && (0.0 < *(float *)(plVar33 + 1))) {
            uVar38 = 1;
          }
          uVar43 = (ulonglong)((int)uVar43 + 1);
          uVar17 = local_eb0;
          uVar36 = local_edc;
          uVar10 = uVar38;
        } while (uVar43 < (ulonglong)
                          (*(longlong *)(lVar35 + 0x3c0) - *(longlong *)(lVar35 + 0x3b8) >> 4));
      }
      local_ed0[0] = uVar10;
      local_edc = uVar36 + 1;
      uVar37 = (ulonglong)local_edc;
    } while (local_edc < uVar17);
    local_dc0 = local_df0;
    uVar43 = local_ec8;
    lVar35 = local_dc8;
  }
  uVar57 = DAT_140909b90;
  uVar37 = 0;
  cVar13 = (char)uVar38;
  uVar41 = (ulonglong)plVar29 & 0xffffffffffffff00;
  local_edc = (uint)uVar41;
  local_ed4 = 0;
  uVar17 = uVar37;
  puVar40 = local_ea0;
  puVar42 = puStack_e98;
  if (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 != 0) {
    do {
      uVar38 = (uint)uVar37;
      local_e80 = *(double *)(*(longlong *)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) + 8);
      if ((*(longlong *)((longlong)local_e80 + 0x318) != 0) &&
         (*(int *)(*(longlong *)((longlong)local_e80 + 0x318) + 0x360) == 0)) {
        if ((**(char **)(*(longlong *)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) + 0x10) != '\0')
           && (0.0 < *(float *)(*(longlong *)
                                 (*(longlong *)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) + 0x10)
                               + 4))) {
          FUN_140424430(param_1,local_e80,0,0);
          uVar41 = 0;
          lVar26 = *(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8);
          lVar20 = lVar26 >> 0x3f;
          uVar37 = uVar41;
          if (lVar26 / 0x68 + lVar20 != lVar20) {
            do {
              if (*(int *)(*(longlong *)
                            (*(longlong *)(*(longlong *)(param_1 + 0x144a8) + 0x30 + uVar41) + 0x318
                           ) + 0x360) == 0xb) {
                if ((&local_ed4 < puVar42) && (puVar40 <= &local_ed4)) {
                  lVar20 = -4 - (longlong)puVar40;
                  if (puVar42 == local_ea8) {
                    FUN_14001e440(&local_ea0,1);
                    local_ea8 = local_e90;
                    puVar40 = local_ea0;
                    puVar42 = puStack_e98;
                  }
                  uVar38 = local_ed4;
                  if (puVar42 != (uint *)0x0) {
                    *puVar42 = puVar40[(longlong)local_ed0 + lVar20 >> 2];
                  }
                }
                else {
                  if (puVar42 == local_ea8) {
                    FUN_14001e440(&local_ea0,1);
                    local_ea8 = local_e90;
                    puVar40 = local_ea0;
                    puVar42 = puStack_e98;
                  }
                  if (puVar42 != (uint *)0x0) {
                    *puVar42 = uVar38;
                  }
                }
                local_ee0 = '\x01';
                local_d08 = local_de8;
                uStack_d00 = 0;
                if (uVar43 != 0) {
                  if (((longlong)uVar43 < 0) && (uVar43 != 0)) {
                    lVar20 = -((-uVar43 - 1 >> 5) * 4 + 4);
                  }
                  else {
                    lVar20 = (uVar43 >> 5) * 4;
                  }
                  local_d08 = (void *)((longlong)local_de8 + lVar20);
                  uStack_d00 = (ulonglong)((uint)uVar43 & 0x1f);
                }
                puVar32 = local_d50;
                local_d98 = local_d08;
                uStack_d90 = uStack_d00;
                goto LAB_140745c1f;
              }
              uVar36 = (int)uVar37 + 1;
              uVar41 = uVar41 + 0x68;
              uVar37 = (ulonglong)uVar36;
            } while ((ulonglong)(longlong)(int)uVar36 <
                     (ulonglong)
                     ((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68))
            ;
          }
          uVar41 = (ulonglong)local_edc;
        }
        if (*(char *)(*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) + 0x10) + 200) !=
                '\0') {
          FUN_140424430(param_1,local_e80,0,uVar57);
          uVar41 = 0;
          lVar26 = *(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8);
          lVar20 = lVar26 >> 0x3f;
          uVar37 = uVar41;
          if (lVar26 / 0x68 + lVar20 != lVar20) {
LAB_140745ae1:
            if (*(int *)(*(longlong *)
                          (*(longlong *)(*(longlong *)(param_1 + 0x144a8) + 0x30 + uVar41) + 0x318)
                        + 0x360) != 0xb) goto code_r0x000140745afd;
            if ((&local_ed4 < puVar42) && (puVar40 <= &local_ed4)) {
              lVar20 = -4 - (longlong)puVar40;
              if (puVar42 == local_ea8) {
                FUN_14001e440(&local_ea0,1);
                local_ea8 = local_e90;
                puVar40 = local_ea0;
                puVar42 = puStack_e98;
              }
              uVar38 = local_ed4;
              if (puVar42 != (uint *)0x0) {
                *puVar42 = puVar40[(longlong)local_ed0 + lVar20 >> 2];
              }
            }
            else {
              if (puVar42 == local_ea8) {
                FUN_14001e440(&local_ea0,1);
                local_ea8 = local_e90;
                puVar40 = local_ea0;
                puVar42 = puStack_e98;
              }
              if (puVar42 != (uint *)0x0) {
                *puVar42 = uVar38;
              }
            }
            local_ee0 = '\0';
            local_e28 = local_de8;
            uStack_e20 = 0;
            if (uVar43 != 0) {
              if (((longlong)uVar43 < 0) && (uVar43 != 0)) {
                lVar20 = -((-uVar43 - 1 >> 5) * 4 + 4);
              }
              else {
                lVar20 = (uVar43 >> 5) * 4;
              }
              local_e28 = (void *)((longlong)local_de8 + lVar20);
              uStack_e20 = (ulonglong)((uint)uVar43 & 0x1f);
            }
            puVar32 = local_d60;
            local_d98 = local_e28;
            uStack_d90 = uStack_e20;
LAB_140745c1f:
            puVar42 = puVar42 + 1;
            puStack_e98 = puVar42;
            FUN_140204c70(&local_de8,puVar32,&local_d98,1);
            uVar43 = local_dd0;
          }
LAB_140745c50:
          uVar41 = (ulonglong)local_edc;
        }
      }
      uVar41 = uVar41 & 0xff;
      if (**(char **)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) == '\0') {
        uVar41 = 1;
      }
      local_edc = (uint)uVar41;
      local_ed4 = uVar38 + 1;
      uVar37 = (ulonglong)local_ed4;
      uVar17 = (longlong)(int)local_ed4;
    } while ((ulonglong)(longlong)(int)local_ed4 <
             (ulonglong)(*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3));
    cVar13 = (char)local_ed0[0];
  }
  FUN_1406f36b0(param_1,lVar35,local_ee4,local_ee8);
  local_ee4 = DAT_140992088 * DAT_14090ab98 + fVar51;
  uVar17 = 0;
  local_ce8[0] = L'\0';
  bVar12 = false;
  cVar11 = FUN_1401bbfe0(param_1,lVar35);
  if (cVar11 == '\0') {
    cVar11 = FUN_1401bbd50();
    if (cVar11 != '\0') {
      psVar27 = &sStack_cea;
      do {
        psVar27 = psVar27 + 1;
      } while (*psVar27 != 0);
      *(undefined4 *)psVar27 = DAT_1408864b4;
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x954);
      psVar27 = &sStack_cea;
      do {
        psVar27 = psVar27 + 1;
      } while (*psVar27 != 0);
      lVar20 = 0;
      do {
        wVar2 = pwVar18[lVar20];
        psVar27[lVar20] = wVar2;
        lVar20 = lVar20 + 1;
      } while (wVar2 != L'\0');
      psVar27 = &sStack_cea;
      do {
        psVar27 = psVar27 + 1;
      } while (*psVar27 != 0);
      *(undefined4 *)psVar27 = DAT_1408fc224;
      sVar15 = DAT_1408fc228;
      goto LAB_140745e18;
    }
  }
  else {
    psVar27 = &sStack_cea;
    do {
      psVar28 = psVar27;
      psVar27 = psVar28 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc214;
    psVar28[3] = DAT_1408fc218;
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x95a);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc21c;
    sVar15 = DAT_1408fc220;
  LAB_140745e18:
    bVar12 = true;
    psVar27[2] = sVar15;
    uVar53 = DAT_1408fc234;
    uVar54 = DAT_1408fc238;
    uVar55 = DAT_1408fc23c;
    uVar56 = DAT_1408fc240;
    uVar57 = DAT_1408fc244;
    uVar58 = DAT_1408fc248;
    uVar59 = DAT_1408fc24c;
    uVar60 = DAT_1408fc250;
    uVar61 = DAT_1408fc254;
    uVar62 = DAT_1408fc258;
    uVar63 = DAT_1408fc25c;
    uVar64 = DAT_1408fc260;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3e8);
    puVar23 = (undefined4 *)0x0;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc22c,DAT_1408fc228);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3e9);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc230;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc230,DAT_1408fc234);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3ea);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc238;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc238,DAT_1408fc23c);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3eb);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc240;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc240,DAT_1408fc244);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3ec);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc248;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc248,DAT_1408fc24c);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3ed);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc250;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc250,DAT_1408fc254);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3ee);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc258;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined8 *)psVar27 = CONCAT44(DAT_1408fc258,DAT_1408fc25c);
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x3ef);
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    lVar20 = 0;
    do {
      wVar2 = pwVar18[lVar20];
      psVar27[lVar20] = wVar2;
      lVar20 = lVar20 + 1;
    } while (wVar2 != L'\0');
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc260;
    psVar27 = &sStack_cea;
    do {
      psVar27 = psVar27 + 1;
    } while (*psVar27 != 0);
    *(undefined4 *)psVar27 = DAT_1408fc260;
    uVar53 = 0x49;
    uVar54 = 0x1388;
    uVar55 = 0x1388;
    uVar56 = 0x1388;
    uVar57 = 0x1388;
    uVar58 = 0x1388;
    uVar59 = 0x1388;
    uVar60 = 0x1388;
    uVar61 = 0x1388;
    uVar62 = 0x1388;
    uVar63 = 0x1388;
    uVar64 = 0x1388;
    puVar23 = (undefined4 *)(longlong)(int)uVar53;

```


## Raw Artifact: scratch/panel_745350_final.c (continued, lines 951-1864)

```c
  uVar21 = FUN_14003aaa0(param_1,"building_addstation_dist");
  *(undefined8 *)(param_1 + 0xd428) = uVar21;
  *(longlong *)(param_1 + 0xd298) = lVar35;
  *(longlong *)(param_1 + 0xd2a0) = lVar20;
  *(undefined1 *)(param_1 + 0xf5b0) = 0;
  *(undefined4 *)(param_1 + 0xf5c0) = 0xbf800000;
  *(undefined8 *)(param_1 + 0xf5b8) = 0;
  FUN_140201cc0(param_1 + 0xf5c8);
  *(undefined8 *)(param_1 + 0xf5f0) = *(undefined8 *)(param_1 + 0xf5e8);
  uVar21 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar12 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar21,0,0);
  if (bVar12) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x809);
    FUN_140015900(param_1 + 0xd5a0,pwVar18);
    if (((*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 != 0) &&
        (DAT_140a54e91 != '\0')) && (*(char *)(lVar20 + 0x20) != '\0')) {
      if (*(char *)(param_1 + 0x14) == '\0') {
        bVar12 = C3D_CONFIGURATOR::GetValueExist
                          ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        if (bVar12) {
          pcVar22 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
          C3D_SOUNDMANAGER::PlaySound2D
                    (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar22,true,fVar6,(char *)0x0);
        }
      }
      FUN_14003c1d0(*(undefined8 *)(param_1 + 0x13bf0));
      *(undefined1 *)(*(longlong *)(param_1 + 0x13bf0) + 0x17) = 1;
      lVar26 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      uVar57 = *(undefined4 *)(lVar26 + 4);
      puVar23 = (undefined4 *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      puVar3 = *(undefined4 **)(param_1 + 0x13bf0);
      *puVar3 = *puVar23;
      puVar3[1] = uVar57;
      fVar49 = DAT_140992088;
      fVar50 = DAT_140992088 * fVar51;
      lVar26 = *(longlong *)(param_1 + 0x13bf0);
      *(undefined1 *)(lVar26 + 0x16) = 0;
      *(float *)(lVar26 + 8) = fVar50;
      if (fVar50 < fVar49 * fVar48) {
        *(float *)(lVar26 + 8) = fVar49 * fVar48;
      }
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88c1);
      FUN_140600350(*(undefined8 *)(param_1 + 0x13bf0),pwVar18);
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x809);
      FUN_1406e6fd0(*(undefined8 *)(param_1 + 0x13bf0),pwVar18);
      lVar26 = *(longlong *)(param_1 + 0x13bf0);
      *(code **)(lVar26 + 0x78) = FUN_140745140;
      *(longlong *)(lVar26 + 0x70) = lVar35;
      FUN_14003be20(*(undefined8 *)(param_1 + 0x13bf0),lVar20);
    }
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar6,fVar6,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xbd80) + 0x70))
            (*(longlong **)(param_1 + 0xbd80),0,DAT_1409ead08);
  lVar30 = *(longlong *)(param_1 + 0xd288) - *(longlong *)(param_1 + 0xd280);
  lVar26 = lVar30 >> 0x3f;
  _DAT_1409be2ec = _DAT_1409be2e8;
  if (lVar30 / 0x2d0 + lVar26 != lVar26) {
    do {
      iVar16 = strcmp((char *)(*(longlong *)(param_1 + 0xd280) + uVar37 * 0x2d0),
                      "building_copy_dist");
      if (iVar16 == 0) {
        lVar26 = *(longlong *)(param_1 + 0xd280) + uVar37 * 0x2d0;
        goto LAB_140747938;
      }
      uVar37 = (ulonglong)((int)uVar37 + 1);
    } while (uVar37 < (ulonglong)
                            ((*(longlong *)(param_1 + 0xd288) - *(longlong *)(param_1 + 0xd280)) / 0x2d0
                            ));
  }
  lVar26 = 0;
LAB_140747938:
  if (*(longlong *)(param_1 + 0xd428) == lVar26) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
  }
  uVar21 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar12 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar21,0,0);
  if (bVar12) {
    if (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 == 0) {
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x822);
      FUN_140015900(param_1 + 0xd5a0,pwVar18);
    }
    else {
      _DAT_1409be30c = _DAT_14090c4c0;
      uRam00000001409be314 = _UNK_14090c4c8;
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x815);
      FUN_140015900(param_1 + 0xd5a0,pwVar18);
      if ((DAT_140a54e91 != '\0') && (*(char *)(lVar20 + 0x20) != '\0')) {
        if ((*(char *)(param_1 + 0x14) == '\0') &&
           (bVar12 = C3D_CONFIGURATOR::GetValueExist
                              ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar12)) {
          pcVar22 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
          C3D_SOUNDMANAGER::PlaySound2D
                    (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar22,true,fVar6,(char *)0x0);
        }
        if (*(longlong *)(param_1 + 0xd428) == lVar26) {
          *(undefined8 *)(param_1 + 0xd428) = 0;
        }
        else {
          *(longlong *)(param_1 + 0xd428) = lVar26;
          *(longlong *)(param_1 + 0xd298) = lVar35;
          *(longlong *)(param_1 + 0xd2a0) = lVar20;
          *(undefined1 *)(param_1 + 0xf5b0) = 0;
          *(undefined4 *)(param_1 + 0xf5c0) = 0xbf800000;
          *(undefined8 *)(param_1 + 0xf5b8) = 0;
          FUN_140201cc0(param_1 + 0xf5c8);
          *(undefined8 *)(param_1 + 0xf5f0) = *(undefined8 *)(param_1 + 0xf5e8);
        }
      }
    }
  }
  uVar37 = 0;
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar6,fVar6,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb400) + 0x70))
            (*(longlong **)(param_1 + 0xb400),0,DAT_1409ead08);
  local_ec0 = 0;
  _DAT_1409be2f0 = CONCAT44(local_ee8,fVar44 + local_ee4 + DAT_140992088 * _DAT_14090acfc);
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * fVar45;
  _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
  uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
  _DAT_1409be2ec = _DAT_1409be2e8;
  cVar13 = FUN_1401e9f10();
  uVar21 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar12 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar21,0,0);
  if (bVar12) {
    if (cVar13 == '\0') {
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110f);
      FUN_140015900(param_1 + 0xd5a0,pwVar18);
    }
    else {
      _DAT_1409be30c = _DAT_14090c4c0;
      uRam00000001409be314 = _UNK_14090c4c8;
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110c);
      FUN_140015900(param_1 + 0xd5a0,pwVar18);
      if ((DAT_140a54e91 != '\0') && (*(char *)(lVar20 + 0x20) != '\0')) {
        if ((*(char *)(param_1 + 0x14) == '\0') &&
           (bVar12 = C3D_CONFIGURATOR::GetValueExist
                              ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar12)) {
          pcVar22 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
          C3D_SOUNDMANAGER::PlaySound2D
                    (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar22,true,fVar6,(char *)0x0);
        }
        FUN_1401ea2a0(param_1,lVar35);
      }
    }
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar6,fVar6,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xaff8) + 0x70))
            (*(longlong **)(param_1 + 0xaff8),0,DAT_1409ead08);
  local_ec0 = 0;
  _DAT_1409be2f0 = CONCAT44(local_ee8,fVar44 + local_ee4 + DAT_140992088 * DAT_14090ada8);
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * fVar45;
  _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
  uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
  _DAT_1409be2ec = _DAT_1409be2e8;
  uVar21 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar12 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar21,0,0);
  fVar45 = DAT_14090a928;
  if (bVar12) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88bb);
    FUN_140015900(param_1 + 0xd5a0,pwVar18);
    if ((DAT_140a54e91 != '\0') && (*(char *)(lVar20 + 0x20) != '\0')) {
      fVar45 = 0.0;
      if ((lVar35 != 0) &&
         (lVar30 = *(longlong *)(lVar35 + 0xa18) - *(longlong *)(lVar35 + 0xa10),
         lVar26 = lVar30 >> 0x3f, lVar30 / 0x60 + lVar26 != lVar26)) {
        do {
          lVar26 = *(longlong *)(uVar37 * 0x60 + *(longlong *)(lVar35 + 0xa10) + 0x20);
          if (((lVar26 != 0) && (*(int *)(lVar26 + 300) == 2)) &&
             ((*(float *)(lVar26 + 0x11c) < fVar45 || (fVar45 <= 0.0)))) {
            fVar45 = *(float *)(lVar26 + 0x11c);
          }
          uVar37 = (ulonglong)((int)uVar37 + 1);
        } while (uVar37 < (ulonglong)
                            ((*(longlong *)(lVar35 + 0xa18) - *(longlong *)(lVar35 + 0xa10)) / 0x60)
                      );
      }
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar12 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar12)) {
        pcVar22 = C3D_CONFIGURATOR::GetValueString
                          ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar22,true,fVar6,(char *)0x0);
      }
      FUN_14003c1d0(*(undefined8 *)(param_1 + 0x13bf0));
      *(undefined1 *)(*(longlong *)(param_1 + 0x13bf0) + 0x17) = 1;
      iVar16 = *(int *)(lVar35 + 0xd74);
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88b9);
      FUN_14003c4b0(*(undefined8 *)(param_1 + 0x13bf0),pwVar18,(float)iVar16,fVar6);
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88ba);
      fVar45 = DAT_14090a928;
      FUN_14003c4b0(*(undefined8 *)(param_1 + 0x13bf0),pwVar18,*(undefined4 *)(lVar35 + 0xd70),
                    DAT_14090a928);
      lVar26 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      uVar57 = *(undefined4 *)(lVar26 + 4);
      puVar23 = (undefined4 *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      puVar3 = *(undefined4 **)(param_1 + 0x13bf0);
      *puVar3 = *puVar23;
      puVar3[1] = uVar57;
      fVar49 = (float)FUN_14003c6d0(*(undefined8 *)(param_1 + 0x13bf0));
      fVar9 = DAT_140992088;
      fVar49 = fVar49 + DAT_140992088 * DAT_14090a828;
      fVar44 = DAT_140992088 * fVar51;
      if (fVar49 <= DAT_140992088 * fVar51) {
        fVar44 = fVar49;
      }
      fVar51 = DAT_140992088 * DAT_14090aafc;
      fVar44 = fVar44 + fVar51;
      lVar26 = *(longlong *)(param_1 + 0x13bf0);
      *(undefined1 *)(lVar26 + 0x16) = 0;
      *(float *)(lVar26 + 8) = fVar44;
      if (fVar44 < fVar9 * fVar48) {
        *(float *)(lVar26 + 8) = fVar9 * fVar48;
      }
      *(float *)(*(longlong *)(param_1 + 0x13bf0) + 0x10) = fVar51;
      pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,35000);
      FUN_140600350(*(undefined8 *)(param_1 + 0x13bf0),pwVar18);
      lVar26 = *(longlong *)(param_1 + 0x13bf0);
      *(undefined1 **)(lVar26 + 0x78) = &LAB_140749690;
      *(longlong *)(lVar26 + 0x70) = lVar35;
      FUN_14003be20(*(undefined8 *)(param_1 + 0x13bf0),lVar20);
    }
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar6,fVar6,0.0,true);
  local_ee8 = local_ee8 + DAT_140992088 * DAT_14090a9a0;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5d7);
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,local_ee4,local_ee8,
             0xff000000,L"%ls: %d");
  local_ee8 = local_ee8 + DAT_140992088 * DAT_14090a968;
  iVar16 = C3D_CONFIGURATOR::GetValueInteger
                   ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"UNITS_METRIC_IMPERIAL");
  iVar31 = 0x3607;
  if (iVar16 == 0) {
    iVar31 = 0x254;
  }
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar31);
  C3D_CONFIGURATOR::GetValueInteger((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"UNITS_METRIC_IMPERIAL");
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5d6);
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,local_ee4,local_ee8,
             0xff000000,L"%ls: %d %ls");
  local_ee8 = local_ee8 + DAT_140992088 * DAT_14090a9cc;
  fVar48 = (float)FUN_140769300(param_1);
  fVar9 = DAT_14090a86c;
  dVar8 = DAT_14090a1f0;
  dVar7 = DAT_14090a1c0;
  local_ee8 = local_ee8 + fVar48;
  local_ee4 = local_eb8;
  local_ed4 = 0;
  if (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 == 0) {
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x80c);
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
               local_ee4 - DAT_140992088 * DAT_14090a730,DAT_140992088 * fVar46 + local_ee8,
               0xff990000,L"%ls");
    local_ee8 = local_ee8 + DAT_140992088 * fVar45;
  }
  else {
    local_edc = 0;
    if (*(longlong *)(lVar35 + 0xd40) - *(longlong *)(lVar35 + 0xd38) >> 3 != 0) {
      local_eb0 = 0;
      local_ec8 = 0;
      local_d68 = 0;
      local_e30 = 0;
      local_e70 = 0;
      uVar57 = _DAT_14090c240;
      uVar58 = _UNK_14090c244;
      uVar59 = _UNK_14090c248;
      uVar60 = _UNK_14090c24c;
      uVar61 = _DAT_14090c240;
      uVar62 = _UNK_14090c244;
      uVar63 = _UNK_14090c248;
      uVar64 = _UNK_14090c24c;
      uVar52 = _DAT_14090c240;
      uVar53 = _UNK_14090c244;
      uVar54 = _UNK_14090c248;
      uVar55 = _UNK_14090c24c;
      do {
        uVar17 = local_ec8;
        uVar38 = local_ed4;
        uVar21 = *(undefined8 *)(*(longlong *)(*(longlong *)(lVar35 + 0xd38) + local_ec8 * 8) + 8)
        ;
        FUN_1403329e0(param_1,uVar21,0x100,1);
        cVar13 = FUN_14042a760(DAT_140992088 * DAT_14090aeb8,local_4e8,param_1 + 0x117bc,
                               DAT_140995220);
        if ((cVar13 != '\0') &&
           (lVar26 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90),
           fVar46 = *(float *)(lVar20 + 8) + *(float *)(lVar20 + 0x2c) +
                    *(float *)(lVar20 + 0x51c),
           *(float *)(lVar26 + 4) <= fVar46 && fVar46 != *(float *)(lVar26 + 4))) {
          fVar56 = DAT_140992088 * DAT_14090a8a0 + local_ee8;
          fVar48 = DAT_140992088 * DAT_14090aeb8 + local_ee4;
          fVar44 = local_ee8 - DAT_140992088 * DAT_14090a730;
          fVar51 = DAT_140992088 * DAT_14090a8a0 + local_ee4;
          lVar26 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          fVar46 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060);
          fVar46 = *(float *)(lVar26 + 4) - fVar46;
          pfVar24 = (float *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          uVar57 = _DAT_14090c240;
          uVar58 = _UNK_14090c244;
          uVar59 = _UNK_14090c248;
          uVar60 = _UNK_14090c24c;
          uVar61 = _DAT_14090c240;
          uVar62 = _UNK_14090c244;
          uVar63 = _UNK_14090c248;
          uVar64 = _UNK_14090c24c;
          uVar38 = local_ed4;
          uVar52 = _DAT_14090c240;
          uVar53 = _UNK_14090c244;
          uVar54 = _UNK_14090c248;
          uVar55 = _UNK_14090c24c;
          fVar45 = DAT_14090a928;
          if (((fVar51 - *pfVar24) * (fVar48 - *pfVar24) <= 0.0) &&
             ((fVar44 - fVar46) * (fVar56 - fVar46) <= 0.0)) {
            FUN_140015900(param_1 + 0xd5a0,param_1 + 0x117bc);
            uVar57 = _DAT_14090c240;
            uVar58 = _UNK_14090c244;
            uVar59 = _UNK_14090c248;
            uVar60 = _UNK_14090c24c;
            uVar61 = _DAT_14090c240;
            uVar62 = _UNK_14090c244;
            uVar63 = _UNK_14090c248;
            uVar64 = _UNK_14090c24c;
            uVar38 = local_ed4;
            uVar52 = _DAT_14090c240;
            uVar53 = _UNK_14090c244;
            uVar54 = _UNK_14090c248;
            uVar55 = _UNK_14090c24c;
            fVar45 = DAT_14090a928;
          }
        }
        C3D_FONTMANAGER::PrintLeftUnicode
                  ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
                   DAT_140992088 * DAT_14090a8a0 + local_ee4,
                   local_ee8 - DAT_140992088 * DAT_14090a444,0xff990000,L"%ls");
        puVar19 = (ulonglong *)(**(code **)(*DAT_140995220 + 0x28))(DAT_140995220,local_cf8,0,0);
        local_e68 = *puVar19;
        uStack_e60 = puVar19[1];
        local_ee8 = local_ee8 + DAT_140992088 * fVar45;
        if (**(char **)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0xb500) + 0x70))
                    (*(longlong **)(param_1 + 0xb500),0,DAT_1409ead08);
          fVar46 = (float)(int)uStack_e60;
          local_d80 = 0;
          _DAT_1409be2f0 =
               CONCAT44(local_ee8 - DAT_140992088 * fVar9,
                        fVar46 + local_ee4 + DAT_140992088 * DAT_14090a9f0);
          DAT_1409be2f8 = 0;
          _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
          _DAT_1409be30c = _DAT_14090c4c0;
          uRam00000001409be314 = _UNK_14090c4c8;
          _DAT_1409be2ec = _DAT_1409be2e8;
          uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          bVar12 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
          if (bVar12) {
            pwVar18 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b7);
            uVar25 = FUN_140015900(param_1 + 0xd5a0,pwVar18);
            if ((DAT_140a54e91 != '\0') &&
               (DAT_140a54e91 = '\0', *(char *)(lVar20 + 0x20) != '\0')) {
              lVar26 = *(longlong *)(lVar35 + 0xd38);
              uVar14 = FUN_1401309a0(uVar25,lVar35,
                                     *(undefined8 *)(*(longlong *)(lVar26 + uVar17 * 8) + 8));
              **(undefined1 **)(lVar26 + uVar17 * 8) = uVar14;
              if (**(char **)(*(longlong *)(lVar35 + 0xd38) + uVar17 * 8) != '\0') {
                *(undefined1 *)(lVar35 + 0xd50) = 1;
              }
            }
          }
LAB_1407485c7:
          C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar6,fVar6,0.0,true);
          local_ed4 = local_ed4 + 1;
          local_eb0 = local_eb0 + 1;
          iVar16 = (int)(fVar46 + DAT_14090a9f0);
        }
        else {
          if (((uVar43 != 0) && ((ulonglong)(longlong)(int)uVar38 < uVar43)) &&
             (local_ea0[local_eb0] == local_edc)) {
            (**(code **)(**(longlong **)(param_1 + 0xb500) + 0x70))
                      (*(longlong **)(param_1 + 0xb500),0,DAT_1409ead08);
            fVar46 = (float)(int)uStack_e60;
            local_d74 = 0;
            _DAT_1409be2f0 =
                 CONCAT44(local_ee8 - DAT_140992088 * fVar9,
                          fVar46 + local_ee4 + DAT_140992088 * DAT_14090a9f0);
            DAT_1409be2f8 = 0;
            _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
            _DAT_1409be30c = _DAT_14090c4c0;
  }
  else {
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x814);
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
               local_ee4 - DAT_140992088 * DAT_14090a730,DAT_140992088 * fVar46 + local_ee8,
               0xff990000,L"%ls: %d / 20");
    local_ee8 = local_ee8 + DAT_140992088 * DAT_14090a9f0;
  }
  fVar46 = DAT_14090aa0c;
  uVar17 = 0;
  local_ee4 = local_ee4 - DAT_140992088 * DAT_14090a800;
  if ((*(int *)(param_1 + 0x5b0) == 2) &&
     (local_ee8 = local_ee8 + DAT_140992088 * DAT_14090a828,
     lVar30 = *(longlong *)(lVar35 + 0x978) - *(longlong *)(lVar35 + 0x970), lVar26 = lVar30 >> 0x3f
     , lVar30 / 0xe0 + lVar26 != lVar26)) {
    do {
      lVar30 = uVar17 * 0xe0;
      lVar26 = *(longlong *)(lVar35 + 0x970) + lVar30;
      iVar16 = *(int *)(lVar26 + 0x90);
      iVar31 = (int)uVar17;
      if ((((iVar16 != 9) &&
           (((iVar16 != 0xe && (1 < *(int *)(lVar26 + 0x88) - 5U)) &&
            (iVar31 != *(int *)(*(longlong *)(lVar35 + 0x318) + 0x838))))) &&
          (iVar31 != *(int *)(*(longlong *)(lVar35 + 0x318) + 0x83c))) &&
         ((*(int *)(param_1 + 0x5b4) != 0 || (1 < iVar16 - 0xfU)))) {
        fVar45 = DAT_140992088 * 0.0;
        iVar16 = FUN_1402a1b20(*(undefined4 *)(lVar30 + 0x90 + *(longlong *)(lVar35 + 0x970)),
                               *(undefined4 *)(lVar30 + 0x88 + *(longlong *)(lVar35 + 0x970)));
        C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar16);
        C3D_FONTMANAGER::PrintLeftUnicode
                  ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,fVar45 + local_ee4,
                   fVar45 + local_ee8,0xff990000,L"%ls:");
        local_ee8 = local_ee8 + DAT_140992088 * fVar46;
        FUN_140706c00(param_1,lVar20,lVar26,&local_ee4);
      }
      uVar17 = (ulonglong)(iVar31 + 1);
      puVar40 = local_ea0;
    } while (uVar17 < (ulonglong)
                      ((*(longlong *)(lVar35 + 0x978) - *(longlong *)(lVar35 + 0x970)) / 0xe0));
  }
  local_ee4 = local_eb8 - DAT_140992088 * fVar56;
  FUN_1406f4b40(param_1,lVar20,&local_ee4,&local_ee8);
  *(float *)(lVar20 + 0x250) = local_ee8;
  if (local_db8 != 0) {
    FUN_140001a50(local_db8,local_da8 - local_db8 >> 3,8);
  }
  if (local_de8 != (void *)0x0) {
    FUN_140001a50(local_de8,local_dd8 - (longlong)local_de8 >> 2,4);
  }
  if (puVar40 != (uint *)0x0) {
    FUN_140001a50(puVar40,(longlong)local_ea8 - (longlong)puVar40 >> 2,4);
  }
  if (local_e00 != 0) {
    FUN_140001a50(local_e00,local_dc0 - local_e00 >> 3,8);
  }
  return;
code_r0x000140745afd:
  uVar36 = (int)uVar37 + 1;
  uVar41 = uVar41 + 0x68;
  uVar37 = (ulonglong)uVar36;
  if ((ulonglong)((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68) <=
      (ulonglong)(longlong)(int)uVar36) goto LAB_140745c50;
  goto LAB_140745ae1;
}

```

## Raw Artifact: scratch/agent1_install_connected_decomp.c (excerpt, lines 86-101)

Source path: `scratch/agent1_install_connected_decomp.c` | Bytes: 5439 | SHA-256: `5A8DD879472BC1E430C9E5CD5AE5ABC56090D4DC71BD1628C5F107ADBE9B09FD`
Lines: 86-101 (excerpt; full file is 162 lines)
Partial excerpt: FUN_14001e140 corroboration — shows FUN_14001e140 called with local_res8 by value and table slot written directly.

```c
  if (cVar3 == '\0') {
    *(longlong *)(*plVar1 + -8 + (*(longlong *)(param_1 + 0x688) - *plVar1 >> 3) * 8) = param_2;
  }
  else {
    if (param_3 < 0) {
      FUN_14001e140(plVar1,local_res8,*plVar1);
      FUN_1402030f0(param_1 + 0x638,local_res8,*(undefined8 *)(param_1 + 0x638));
      FUN_14067cd20(param_1);
      goto LAB_14067d85b;
    }
    FUN_14001e140(plVar1,local_res8,*plVar1 + (longlong)param_3 * 8);
    FUN_1402030f0((longlong *)(param_1 + 0x638),local_res8,
                  *(longlong *)(param_1 + 0x638) + (longlong)param_3 * 0x198);
    FUN_14067cd20(param_1);
    cVar3 = local_res20;
  }
  *(undefined1 *)(param_1 + 0xd3a) = 1;
LAB_14067d85b:
  *(undefined4 *)(param_1 + 0x698) = 0xffffffff;
  uVar5 = 0;
```
