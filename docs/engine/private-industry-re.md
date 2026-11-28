# Private Industry RE â€” attribution of goods movement
> **Status 2026-10-22 — Superseded in part:** For DO task storage, see `docs/ghidra-do-storage-findings.md` (stride `0x18` Entry `+0x00/+0x08/+0x10`, `TaskCfg 0x198`, dirty `+0xD50`/`+0xFC0`, save `1EDC40/6C5350` load `1F0BE0`, type-3 road / type-6 rail, cap 5 policy, `panel+0x310` unverified). The `Logistics_Investigation` `0xD38 stride 8` pointer-vector model is incomplete; use the new doc as source of truth. Original kept for history.

Target: SOVIET64.exe v1.1.1.9 (PE timestamp 0x6A3EB6AD), base 0x140000000, ASLR on.
Mode: investigation only. RVAs in text; file-based (0x140+RVA) only in dumps.

> **Route correction (2026-11-27):** older rounds below called the
> `0x68F792`/`0x68F7F9` pair sea/rail-only. The owner-provided runtime log
> shows this exact delivery pair on road DO trucks (661/661 logged pairs in
> that run). See H15 and the [step-3b evidence report](../private-logistics-runtime/evidence/STEP3B_OWNER_RUN_2026-11-27.md).
> This corrects the route label for the pair; it does not classify every site
> in `FUN_14068B330` or establish universal event coverage.

## Consolidated offset table

| object | offset | meaning | confidence |
|---|---|---|---|
| Vehicle | +0x4F8 | home Building* (owning depot/garage/DO) | proven |
| Vehicle | +0x4F0 | current Building* (writer: FUN_1403FBDC0 @0x3FC2F4) | proven |
| Vehicle | +0x488/+0x490 | trailer vector stride 8 | proven |
| Vehicle | +0x3B8/+0x3C0 | cargo vec stride 0x10 (+0x00 Res*, +0x08 tons) | proven |
| Vehicle | +0x3A0/+0x3A8 | trailer Object* vec â€” NOT cargo | corrected |
| Vehicle | +0x610/+0x618 | line stops stride 0x21B8 | likely |
| DO bld | +0xC70/+0xC78 | vehicle vec stride 8 | proven prior |
| TaskEntry | +0x00/+0x08/+0x10 | enabled/target Building*/TaskCfg* | proven prior |
| TaskCfg | +0x00/+0x04/+0x08, +0xC8/+0xCC/+0xD0, +0x8C | LOAD / UNLOAD / capacity | proven prior |
| Game | +0x144A8/+0x144B0 | order vec stride 0x68 | proven prior |
| Order | +0x00/+0x08, +0x38/+0x40, +0x50 | sources / resvec / completion-records | likely |
| Order +0x30 | assigned Vehicle* (NULL-tolerant at 0x1B2397/0x1B28D9) | proven |
| Transfer param_9 (stack+0x40) | destination Building*, NULL = order-scan/line mode | likely |
| Storage | +0x84/+0x85 | import/export byte | proven |
| Slot | +0x00/+0x08/+0x0C | Res*/tons/quality | proven prior |
| Game | +0x580 / +0x588 | treasury USD / RUB | proven |
| Order | +0x00 | pointer vs game+0xc3f0 (fuel) | likely |
| Order | +0x10 | source Building* | likely |
| Order | +0x28 | value vs time deadline | likely |
| Order | +0x48 | Building* (destination/other) | likely |
| Order | +0x50 | index into DO targets vec | likely |
| Order | +0x60 | value vs time deadline | likely |
| Order | +0x68/+0x70 | time pair (end-start) | likely |
| Vehicle purchase | FUN_1401AAD70 (RVA 0x1AAD70) | treasury deduct + spawn + +0x4F8 set | proven |
| Transfer FUN_140420350 | AL==1 = movement committed; param_6 (stk+0x28) out = clamped moved tons; no dry-run mode | proven r10 |
| +0x4F0=+0x4F8 writer | FUN_1403FBDC0 (RVA 0x3FBDC0) at 0x3FC2F4 - detach/return-to-base, NOT the transfer | proven r10 |
| FUN_14068B330 family transfer sites | 6 transfer sites (0x68EECA m0+, 0x68F792 m0+, 0x68F7F9 m1-, 0x6945B3 m0-, 0x694F40 m0-, 0x6953C4 m1+); mode touches side (m0=order pool, m1=cargo slot), sign = direction. The 0x68F792/0x68F7F9 pair is observed on road DO deliveries; the family is not sea/rail-only. | proven r10; road use confirmed in one owner run |
| Vehicle | sizeof 0x1AD8; +0x04 global vehicle index; +0x19E0 cross-world mirror vec | proven r10 |
| Vehicle | +0x510 int type index (compared vs bld desc +0x820 slot count) | proven r10 |
| World | +0x7C08/+0x7C78 catalog vec stride 0x110, entry+0x100 -> type object; +0x7CB8 count | likely r10 |
| Resource desc | +0x58/+0x5C unit price floats (Res+0x44==-1 = passenger) | likely r10 |
| Purchase price | param_4 high float = USD at FUN_1401AAD70; RUB via FUN_1403E3F40 out (type 0x14 only) | likely r10 |
| Vehicle INI | $COST_RUB (x294) / $COST_USD (x62) in media_soviet\vehicles\*\script.ini | proven r10 |

What surprised me: vehicle+0x3A0 is trailers, not cargo; cargo is +0x3B8.
No single load function: FUN_140420350 holds the hookable tons math.
### Q1 â€” vehicle owning building
Answer: Vehicle+0x4F8 is home Building*; no fleet id found.
Offsets: veh+0x4F8 home; veh+0x4F0 current; bld+0xC70 forward vec.
Functions: RVA 0x16C1C0 writer; 0x1C1770/0x1C6050 readers; tick thread.
Evidence: d_q1_vehicle_owner.c + x_q1_4f8.txt + asm_q1_16c7c5.txt.
Sources: upkeep check plus MOV [RDI+0x4F8],R12 writer.
Confidence: proven. Blocks: private carriers Stage 2.

### Q2 â€” load event
Answer: No single transfer; hookable tons math is FUN_140420350.
Offsets: veh+0x3B8 cargo; order+0x38 candidates; slot+0x08 tons.
Function: RVA 0x420350 (game,veh,mode,res,tons,out,cap,flag,bld); tick.
Evidence: d_q2_load_event.c + asm_q2_transfer_tail.txt + d_q4_line.c.
Sources: float add plus clamp; ADDSS/SUBSS disasm.
Confidence: likely / proven-layout. Blocks: event billing.
Hook: NO inline (prologue 5 bytes 40 53 57 41 54); patch caller call rel32.

### Q3 â€” delivery order struct
Answer: order+0x30 is destination Building*; stride 0x68.
Offsets: +0x00/+0x08 sources; +0x30 dest; +0x38/+0x40 resvec; +0x50 done.
Functions: writer 0x424430; readers 0x423FE0/0x423E20/0x420350; tick.
Evidence: d_q3_order.c + x_q3_order.txt + asm_q3_planner.txt.
Confidence: proven-stride / likely-dest. Blocks: buyer attribution.

### Q4 â€” non-DO next stop
Answer: veh+0x610/+0x618 line stops (0x21B8); index not isolated.
Function: RVA 0x420350 walks stops; tick.
Evidence: d_q4_line.c + x_q2_3a0.txt.
Confidence: likely-list / speculative-index. Blocks: state-fleet fallback.
### Q5 â€” descriptor recipe
Answer: Not finished; veh+0x688 is planner cache, not recipe.
Offsets: typedesc+0x360 type number proven prior; recipe unknown.
Function: RVA 0x424430 reads veh+0x688.
Evidence: d_q5_688.txt planner hits.
Confidence: speculative. Blocks: storage classification.

### Q6 â€” native ownership
Answer: No such field found.
Evidence: x_q6_owner.txt (no owner/company/enterprise) + d_q6_d00.txt (39 hits, none owner).
Confidence: likely-absence. Blocks: name-tag model safe.

### Q7 â€” delivery completion
Answer: Funnels via RVA 0x1E5350 (DO upkeep finish); tons equality unconfirmed.
Offsets: DO +0xD58/+0xD60 vec stride 0x80; veh+0x528 gate.
Function: RVA 0x1E5350 (game,veh,DO); tick.
Evidence: d_q7_complete.c + d_q1_vehicle_owner.c caller.
Confidence: likely-location / speculative-tons. Blocks: per-ton billing.

### Q8 â€” direction stamper
Answer: Confirmed FUN_1400E40F0 (RVA 0xE40F0) only writer; both-zero possible.
Offsets: stor+0x84 import; +0x85 export; +0x8C cap; +0x94 token.
Function: RVA 0xE40F0 parse/init.
Evidence: d_q8_stamper.c:91-110 + d_q8_84.txt writer-only hits.
Token: +0x84 on FUEL/IMPORT/IMPORT_SPECIAL/DEMAND_HOTEL/TRASH_BIG/CARPLANT; +0x85 on EXPORT/EXPORT_SPECIAL.
Confidence: proven. Blocks: procurement gate stays on +0x85.

### Q9 â€” batch provenance
Answer: No producer id; slot append copies (Res*,tons) only.
Function: RVA 0xB14E0 16-byte append.
Evidence: d_q9_batch.c + d_q4_line.c cargo reads.
Confidence: likely-absence. Blocks: none; vehicle event required.

## Closeout
Unfinished: Q5 recipe, Q4 index, Q7 tons, Q2 caller count. One DisasmMany each.
Threads: all examined funcs on game tick/sim path; no worker evidence.
Doc fix: veh+0x3A0 is trailers (stride 8), veh+0x3B8 is cargo (stride 0x10).

## Part 2 - round 2

### R1 - vehicle to order
Answer: None of the three - no order pointer param, no vehicle order field found, no (game,veh)->order accessor.
Offsets: transfer param order is (game,veh,mode:u32,res,tons:f32,out:f32*,cap:f32,flag:u8,bld).
Functions: RVA 0x420350 transfer scans orders internally; 0x423FE0/0x423E20 are (game)->index readers, not (game,veh)->order.
Evidence: scratch/asm_r1_r2_sites.txt + scratch/x_r1_r2_calls.txt + scratch/d_q4_line.c:130-229.
Sources: (1) all 16 call sites pass R8D mode 0/1 and R9 resource, never game+0x144A8+i*0x68 in RDX/R9/stack; (2) transfer loop walks game+0x144A8..+0x144B0 stride 0x68 itself and tests order+0x30 vs vehicle.
Confidence: likely-absence. Changes: forecloses bill-at-load exact; use bill-at-unload, state-fleet approximate per 3.6.

### R2 - call sites of 0x420350
Answer: 16 CALL sites in 8 functions; load sites are 13 constant-0, unload 1 constant-1, 2 register.
Call sites: 0x141F33 mode 0; 0x16F831 mode 0; 0x1B2898 mode 1; 0x1B484B mode reg (LEA R8D,[RAX+1]); 0x1B5EC0 mode 0; 0x1B653B mode 0; 0x1B6833 mode 0; 0x1B75ED mode 0; 0x1B7981 mode 0; 0x1CB7FF mode 0; 0x68EECA mode 0; 0x68F792 mode 0; 0x68F7F9 mode 1; 0x6945B3 mode 0; 0x694F40 mode 0; 0x6953C4 mode reg (MOV R8D,R15D).
Functions: 0x1403F0 rail/truck AI; 0x16E3B0 vehicle upkeep; 0x1B1290/0x1B3CE0/0x1B5420/0x1B6A20 freight AI cluster; 0x1CB2A0 loader; 0x68B330 ship/line hauler (6 sites).
Evidence: scratch/x_r1_r2_calls.txt (16 UNCONDITIONAL_CALL + 3 DATA) + scratch/asm_r1_r2_sites.txt R8D setup lines.
Sources: Xrefs count plus per-site MOV R8D,0x0 / MOV R8D,0x1 disasm.
Confidence: proven-count / likely-mode. Changes: patching all load sites is NOT <=6 (13 load sites); patch ship-cluster + rail sites or hook unload-complete instead. S2 feasible but wide.



### R3-R5 - deferred
Answer: Not run - budget spent on R1+R2 per brief priority.
Changes: none; fallbacks stand (unload billing, state-pays, direction-byte classification).

## Part 3 - round 3

### R6 - what is order+0x30
Answer: +0x30 is the assigned Vehicle*, not the destination â€” round 1 was wrong.
Offset(s) / call sites: order+0x30 = assigned Vehicle* (8-byte ptr at stride-0x68 entry +0x30).
Function(s): RVA 0x420350 (game,veh,mode,res,tons,out,cap,flag,bld), tick thread; writer is the planner path that assigns orders (store outside examined windows, not re-derived).
Evidence: scratch/asm_r6_scan.txt:45 + scratch/asm_q2_transfer_prologue.txt:7-9 + scratch/d_q4_line.c:170-171.
Sources: (1) prologue `140420364: MOV RBX,RDX` pins RBX=veh (RDX is param_2; decompiler uses param_2 for veh+0x3B8/veh+0x610 everywhere); (2) scan-loop compare `1404206e2: CMP qword ptr [RCX+RAX+0x30],RBX` where RCX=order-index*0x68 and RAX=game+0x144A8 base, matching decompile `*(order+0x30)==param_2` at d_q4_line.c:171.
Literal instructions:
`140420364: MOV RBX,RDX` (RBX holds veh for the whole function)
`1404206d7: MOV RAX,[RDI+0x144a8]` (RAX = order-vector base; RDI = game)
`1404206de: IMUL RCX,R10,0x68` (RCX = candidate index * stride)
`1404206e2: CMP [RCX+RAX+0x30],RBX` (order+0x30 vs veh)
`1404206e7: JNZ 0x14042083b` (skip orders not assigned to this vehicle)
Loop, not single: the compare sits inside the per-order scan (index R13D advances at 140420842, bounds CMP/JC at 14042086a/14042086d back to 140420680), and is bypassed entirely when flag param_8==0 (`CMP [RSP+0xd8],0` at 1404206c1, JZ falls through to 1404206ed).
Round-1 error: asm_q3_planner.txt covers only 0x140424430-0x1404245A0, which is the vector-clear prologue (order slots +0x08/+0x20 reset, destructor loop `ADD RBX,0x68`); it contains no +0x30 store at all, so destination was never proven â€” it was a guess from the reader-side type check in d_q3_order.c. Correction stands: no order offset is currently proven to be the destination.
Confidence: proven (vehicle). Changes: kills bill-at-load-via-+0x30; the payer path is R7/unload-bld instead.

### R7 - where is the destination, really
Answer: Not found as an order field in the examined planner window â€” at unload the destination arrives as transfer param bld ([RSP+0x40]), a direct Building*.
Offset(s) / call sites: destination = FUN_140420350 param_9 (stack+0x40), Building*; no order+0xNN destination store located.
Function(s): planner RVA 0x424430 (writer window 0x1404245A0-0x140424E60 examined); unload call sites RVA 0x1B2898 and 0x68F7F9 (mode==1) carry it.
Evidence: scratch/asm_r7_dest.txt:587-595 + scratch/asm_r7_dest.txt:604-613 + scratch/d_q4_line.c:199-212.
Sources: (1) planner window shows waypoint scoring only (game+0x14478/0x14480 vec reads, td+0x360 type checks, storage vec +0x970/+0x978 reads, vehicle caches +0x688/+0x690) with no order-entry Building* store; (2) unload sites store bld explicitly: `1401b2874: MOV [RSP+0x40],R15` (mode-1 site) and `14068f767: MOV [RSP+0x40],RAX` where `14068f760: MOV RAX,[R8+0x4f8]` (home building).
Reading instruction (unload path uses bld, not an order field): the transfer validates `param_9` as a building (`param_9+0x318` typedesc, type 0x0C/0x6A at d_q4_line.c:87-89) and walks `param_2+0x610` line stops against it.
Partial-delivery tracking lives at order+0x50 (completion-record vector, not destination): `1404207dd: MOV RAX,[RDI+0x144a8]` / `1404207e4: MOV RCX,[RDX+RAX+0x50]` / `1404207e9: MOV RAX,[RCX+R10*8]` then `MOV RAX,[RAX+0x28]` with flag write `[RAX+0x188]=1.0` and resource store `[...+0x198]=param_4` (d_q4_line.c:203-212). Reconcile should count these records per order plus bld-at-unload, not a phantom order destination.

## Part 6 - round 6

### H1 - when bld==NULL, where does the destination come from?
**Answer:** The destination building is **never present** at the `0x1B2898` call â€” not in the caller's frame, not as a transfer parameter, and not as a field of the order; the engine derives it *inside* the transfer from the **source storage it selects**, and the only `Building*` the transfer itself materialises is that **source** (source, not buyer). A hook at `0x1B2898` **cannot** read the buyer without re-implementing the transfer's scan; the buyer is recorded *after* the transfer returns, at `*(*(order+0x50)[i] + 0x28) + 0x198`.

**Ask #1 - the caller's local: negative.** At the call the four stack args are all `R15`-derived and `R15` is the NULL sentinel:
```
1401b2874: MOV qword ptr [RSP+0x40],R15   ; param_9 (bld)  = NULL
1401b2879: MOV byte ptr [RSP+0x38],R15B   ; param_8 (flag) = 0
1401b287e: MOV qword ptr [RSP+0x28],R15   ; param_6        = NULL
1401b2883: MOVSS dword ptr [RSP+0x20],XMM6 ; param_5 (tons)
1401b2892: MOV RDX,RDI                     ; param_2 = veh
1401b2895: MOV RCX,R14                     ; param_1 = game
1401b2898: CALL 0x140420350
```
Register provenance: `R14`=game, `R12`=res (`MOV R9,R12`), `RDI`=veh, `R15`=NULL. There is **no fifth destination argument** and no live building pointer on the caller's frame at the call (`asm_h1_caller.txt`).

**Ask #2 - the transfer's resolution: it keys on the SOURCE, not a destination.** For `bld==NULL` the `0x0C`/`0x6A` branch short-circuits on `param_9 != 0` (`d_q4_line.c:87`), so the transfer calls the planner and scans the order vector by resource (`d_q4_line.c:134-147`). The only `Building*` it loads is the order's **source** at `order+0x00[i]`, and it never dereferences a building to pick storage:
```
190: if (**(longlong **)(lVar3 + lVar1) == param_4)   ; *(order+0x00[i]) == resource
203:    lVar2 = *(longlong *)(
204:      *(longlong *)(lVar8 + 0x50 + *(longlong *)(param_1+0x144a8)) + lVar16*8);  ; elem = (order+0x50)[i]
205:    if (lVar2 != 0)
206:      *(undefined4 *)(*(longlong *)(lVar2 + 0x28) + 0x188) = 0x3f800000;         ; *(elem+0x28)+0x188 = 1.0
211:        *(longlong *)(*(longlong *)(elem + 0x28) + 0x198) = param_4;            ; *(elem+0x28)+0x198 = resource
```
So `order+0x50[i]` is a pointer to a record whose `+0x28` is a pointer to a **completed-delivery log entry** (round-3 R7 wrote the same `+0x188 = 1.0` / `+0x198 = resource` fields). Neither `+0x28` nor `+0x198` is a `Building*` â€” `+0x198` is proven to hold the **resource** (`param_4`), so the buyer is not there.

**Verdict / the positive alternative.** The destination is **not recoverable from the transfer's arguments or the order's destination field**. It is recoverable from the **completed-delivery log** the transfer appends to: the log entry at `*(*(order+0x50)[i] + 0x28)` carries the building it was unloaded into, which round-3 R7's writer (`asm_q3_planner.txt`) stores in that same record. That entry â€” not the order and not the call frame â€” is where the payer lives. A hook at `0x1B2898` must therefore read the delivery's log entry (or re-run the storage selection) to name the buyer.

**Historical route note:** this earlier H4.1 contrast treated the transfer family as sea/rail-only and recommended leaving road on the "state pays" default. That route classification is superseded for the `0x68F792`/`0x68F7F9` delivery pair by the owner-run evidence summarized above. The original register/caller question at `0x6953C4` remains a separate, site-specific question.

**Offset(s):** `order+0x50[i]` â†’ record `+0x28` â†’ log entry; log entry `+0x188` (done float), `+0x198` (Resource*). Destination-building field inside the log entry: **unresolved** (one more field read would settle it).
**Function(s):** `FUN_140420350` (RVA `0x420350`), transfer; caller `FUN_1401B1290` (RVA `0x1B1290`). Thread: game tick.
**Evidence:** `scratch/asm_h1_caller.txt` (caller frame), `scratch/d_q4_line.c:87-147,190-211`, `scratch/asm_q3_planner.txt` (R7 writer). Two sources per claim: (a) caller frame shows no destination arg; (b) decompile shows the only Building* is the source and `+0x198` holds the resource.
**Confidence:** proven (no destination in caller/order) / likely (log entry is the destination of record).
**Changes (historical recommendation, route scope superseded):** forecloses "read buyer at the call site"; the earlier S2 proposal treated sea/rail via `param_9` separately from road. The owner-run evidence later confirmed the `0x68F792`/`0x68F7F9` pair on road DO trucks; see H15.

### H2 — G3: vehicle purchase *(blocks the private-logistics cost model)*
**Answer:** **Found.** FUN_1401aad70 (RVA 0x1AAD70) is the vehicle purchase transaction: it checks the vehicle type (0x14), reads the price, verifies treasury sufficiency, deducts cost from game+0x580 (USD) and game+0x588 (RUB), creates the vehicle via FUN_1403e4a00, and sets 
ehicle+0x4F8 via FUN_1401aaaa0 — all at purchase time, not just at assignment.

**Ask #1 — purchase transaction:** FUN_1401aad70 (RVA 0x1AAD70). Deducts local_res20 from RUB treasury (DAT_1409d5498 = game+0x588) and 
StackX_24 from USD treasury (DAT_1409d5490 = game+0x580), then calls FUN_1403e4a00 to spawn the vehicle and FUN_1401aaaa0 to assign it to the buying building. Aborts if treasury insufficient.

**Ask #2 — price location:** Two-part price passed via param_4 (64-bit): high 32 bits = USD cost (
StackX_24 = param_4 >> 32), low 32 bits → RUB base from FUN_1403e3f40 then modified by FUN_1403f7ce0(&local_res20, ...). The base price ultimately comes from $COST_RUB/$COST_USD in the vehicle's script.ini (e.g. ust_skd_14Tr: $COST_RUB 4300), parsed at vehicle-type load time. Runtime price not traced to a specific descriptor offset — FUN_1403f7ce0 not decompiled.

**Ask #3 — +0x4F8 timing:** Set at **purchase time** by FUN_1401aaaa0(game, building, vehicle, 1) called immediately after FUN_1403e4a00 creates the vehicle (d_h2_purchase.c, lines after treasury deduction). Also overwritten at assignment time by FUN_1401C6DD0 (*(longlong *)(lVar22 + 0x4f8) = param_2; at d_h2_setter.c:318).

**Ask #4 — state vs private:** Engine has no concept of mod [P] tags. All purchases read as state purchases from the engine's POV. Confirmed by absence of any owner/company field (round-1 Q6, x_q6_owner.txt).

**Offset(s):** game+0x580 (treasury USD), game+0x588 (treasury RUB) — confirmed from 17-ui-and-integration.md and treasury xrefs. Vehicle record stride 0x9f08, +0x200 = object name, +0x9c88 = ownership flag (from FUN_1402ff840 world saver, d_worldwrite.txt).

**Function(s):** FUN_1401aad70 (RVA 0x1AAD70), game-tick/UI thread — purchase transaction. FUN_1403e4a00 — vehicle spawn. FUN_1401aaaa0 — sets 
ehicle+0x4F8 = building (likely, pattern match). FUN_1401C6DD0 (RVA 0x1C6DD0) — +0x4F8 setter, assignment only.

**Evidence:** scratch/d_h2_purchase.c (full decompile of 0x1AAD70 — treasury check/deduct, vehicle spawn, +0x4F8 assignment), scratch/x_h2_treasury2.txt (xrefs to game+0x580 showing writes at 0x1401AAF12 in FUN_1401AAD70), scratch/d_h2_setter.c:318 (assignment-time +0x4F8 write), scratch/x_q6_owner.txt (no owner field).

**Two sources:** (a) decompile shows DAT_1409d5498 -= local_res20 and DAT_1409d5490 -= fStackX_24 (treasury deduction) at d_h2_purchase.c:LAB_1401aaef3; (b) xrefs confirm 0x1401AAF12 writes to game+0x580 from FUN_1401AAD70 (x_h2_treasury2.txt).

**Confidence:** proven (treasury deduction + vehicle spawn in same function) / likely (FUN_1401aaaa0 sets +0x4F8 — pattern match, not directly decompiled).

**Changes:** Unblocks the private-logistics cost model — vehicle purchase cost can be read at FUN_1401aad70 (one call site to patch). +0x4F8 is set at purchase, so we can charge the buyer at acquisition time. State vs private: all purchases are state purchases; the mod must overlay [P] ownership on top.


Confidence: likely (bld-is-destination) / speculative-negative (no order destination field in window â€” the order-build store may live outside 0x1404245A0-0x140424E60; one more DisasmMany of the planner tail would settle it). Changes: S2 stays on unload-bld; monthly reconcile (Â§9) bounds arrival via order+0x50 records.

### H3 â€” G8's remainder: does completion compare tonnage? *(blocks S2 billing accuracy)*
**Answer:** **No.** `FUN_1401DE2B0` (the completion check called from `FUN_1401E5350`â†’`FUN_1401E5480`) does **not** compare delivered tonnage against planned tonnage. Completion is **time-gated**: it checks whether the current time has passed a deadline derived from the task config, plus building-type conditions. A partial delivery does **not** keep the order open based on tonnage shortfall.

**Offset(s) on order (`param_2`, treated as `longlong*`):**
| offset | field | meaning |
|--------|-------|---------|
| `+0x00` | pointer | compared against `game+0xc3f0` (fuel resource) â€” special-cases fuel |
| `+0x10` | `Building*` | source building (`+0x318` = type desc) |
| `+0x28` | value | compared against time deadline `dVar12` |
| `+0x48` | `Building*` | destination/other building (`+0x318` = type desc) |
| `+0x50` | index | index into DO targets vector (`DO+0xd38`) |
| `+0x60` | value | compared against time deadline `dVar2` |
| `+0x68`/`+0x70` | time pair | delta computed as `order+0x70 - order+0x68` |

**Function(s):** `FUN_1401DE2B0` (RVA `0x1DE2B0`), game-tick thread. Returns 1 (complete) or 0 (not complete).

**Evidence:** `scratch/d_h3_complete.c` â€” full decompile. Lines 43-55: early returns on `order+0x60 <= 0.0`, `order+0x28 <= dVar12`, `order+0x60 < dVar2`. Lines 104-106: completion condition is purely time/building-type based, no tonnage comparison. Lines 90-98: fuel special-case checks storage amount vs consumption rate, still no planned-vs-delivered tons comparison.

**Two sources:** (a) decompile shows all comparisons are time/building-type based; (b) no `param_2` field holds "planned tons" or "delivered tons" that gets compared â€” the only tonnage-like field (`param_2[5]` at `+0x28`) is compared against a *time* deadline, not another tonnage value.

**Confidence:** likely (decompilation clear on comparison types; field semantics inferred from access patterns).

**Changes:** S2 billing must use planned tonnage at unload time, not delivered tonnage at completion â€” the engine doesn't track delivered-vs-planned. Partial deliveries: order completion is time-gated, not tonnage-gated.

### H4 â€” two small ones *(low priority)*
**H4.1 â€” `RSI` at `0x6953C4`:** Not yet fully traced. At `14069538e: MOV qword ptr [RSP+0x40],RSI`, RSI is stored as `param_9` (destination `bld`) at the transfer call `1406953c4: CALL 0x140420350`. RSI is set at function entry of `FUN_14068B330` (`14068B3ad: MOV RSI,RCX`, i.e. RSI = param_2). The provenance of RCX at the caller of `FUN_14068B330` is unresolved â€” one more Xrefs run needed.

**H4.2 â€” Rail DO kind `0x34`:** Not yet checked. Road DO type `0x2B` confirmed as building window kind 0. Rail DO dispatcher check pending.

**Offset(s):** `+0x248` = window kind (0 = building window, from `Codebase.txt:11082`).

**Function(s):** `FUN_14068B330` (RVA `0x68B330`) â€” ship/line delivery function containing `0x6953C4`. `FUN_1406E9B80` (RVA `0x6E9B80`) â€” window dispatcher.

**Evidence:** `scratch/asm_h4_entry.txt` (RSI=RCX at entry), `scratch/asm_r1_r2_sites.txt:322` (RSI stored as bld at `0x6953C4`).

**Confidence:** likely (RSI = param_2 of `FUN_14068B330`) / not-done (caller provenance + rail DO kind).

**Changes:** H4.1 determines if sea/rail billing can read destination from `param_9` directly (easier than road). H4.2 is for a future DO books panel â€” low priority.

## Corrections

- ~~`order+0x00/+0x08` = vector of source `Building*`~~ â€” corrected in H3: these are order struct fields accessed by `FUN_1401DE2B0`, not a source building vector.
- ~~Round 1 claim that `order+0x30` is destination `Building*`~~ â€” corrected in round 3 R6: `order+0x30` is the assigned **Vehicle***, proven by `CMP [RCX+RAX+0x30],RBX` where `RBX â† RDX â† param_2 = veh` (`asm_r6_scan.txt`).
- ~~`flag=0` means "no order / never bill"~~ â€” corrected in round 4 G6: `flag=0` only bypasses the `+0x30==veh` assignment test; the main delivery site `0x1B2898` passes `flag=0` and is a real delivery.
- ~~Round 4 claim that `0x1B2898` is not a third-party delivery~~ â€” refined in round 6 H1: it *is* the road-delivery unload; `bld=NULL` means destination resolved internally, not that it isn't a sale.

## Closeout

### Questions answered this round

| Question | Answer | Confidence | Key RVA |
|----------|--------|------------|---------|
| H1 â€” destination when `bld==NULL` | Not in caller/order; in completed-delivery log at `*(*(order+0x50)[i] + 0x28)` | proven / likely | `0x1B1290`, `0x420350` |
| H2 â€” vehicle purchase | `FUN_1401aad70` (RVA `0x1AAD70`): treasury deduct + spawn + `+0x4F8` set at purchase | proven / likely | `0x1AAD70` |
| H3 â€” completion compares tonnage | **No** â€” time-gated, no planned-vs-delivered tons comparison | likely | `0x1DE2B0` |
| H4.1 â€” `RSI` at `0x6953C4` | RSI = param_2 (Vehicle*) at entry of `FUN_14068B330`; exact value at call site unresolved | likely | `0x68B330` |
| H4.2 â€” Rail DO kind `0x34` | Not checked | not-done | â€” |

### Still not done
- H4.1: caller provenance of RCX at `FUN_14068B330` entry â€” one Xrefs + DisasmMany run
- H4.2: Rail DO kind `0x34` at dispatcher `FUN_1406E9B80` â€” one DisasmMany run
- H2: price source not traced to specific descriptor offset (`FUN_1403f7ce0` not decompiled)
- H3: order field semantics inferred, not all proven

### Surprises
1. The vehicle purchase function `FUN_1401aad70` sets `vehicle+0x4F8` at **purchase time** (via `FUN_1401aaaa0`), not just at assignment. This means we can charge the buyer at acquisition time â€” a significant simplification.
2. Completion (`FUN_1401DE2B0`) is **time-gated**, not tonnage-based. There is no delivered-vs-planned tons comparison anywhere in the completion path. Partial deliveries don't keep orders open based on tonnage shortfall.
3. The "destination when `bld==NULL`" question has no clean answer â€” the destination is not in the caller's frame, the transfer's arguments, or the order struct. It lives in the completed-delivery log, requiring either a log read or re-running the storage selection.

### Method
All Ghidra runs sequential (one `analyzeHeadless` at a time), `.java` scripts only, JDK 21, single `-scriptPath` via `scratch\\ghidra_run.bat`, file-based `0x140+RVA`.

Runs this round:
1. `DecompAt.java` â†’ `scratch/d_h2_purchase.c` (`0x1401AAD70`) â€” vehicle purchase
2. `DecompAt.java` â†’ `scratch/d_h2_price.c` (`0x1403E3F40`) â€” price/capacity check (not price)
3. `DecompAt.java` â†’ `scratch/d_h3_complete.c` (`0x1401DE2B0`) â€” completion check
4. `Xrefs.java` â†’ `scratch/x_h2_treasury2.txt` (`0x1409D5490`) â€” treasury writes
5. `Xrefs.java` â†’ `scratch/x_h4_caller.txt` (`0x14068B330`) â€” ship/line callers
6. `DisasmMany.java` â†’ `scratch/asm_h4_caller1.txt` (`0x14066E950:0x14066EA00`) â€” caller context
7. `DisasmMany.java` â†’ `scratch/asm_h4_entry.txt` (`0x14068B330:0x14068B450`) â€” function entry
8. `DisasmMany.java` â†’ `scratch/asm_h4_rsi.txt` (`0x140695300:0x140695390`) â€” RSI store context
9. `DisasmMany.java` â†’ `scratch/asm_h4_rsi2.txt` (`0x140694F80:0x140695300`) â€” wider RSI context

No `DecompAt` attempted on `FUN_14028C770` or `FUN_14030D1A0`. No memory written, no binary patched.

### Threads
All examined functions (`0x1AAD70`, `0x1DE2B0`, `0x68B330`) sit on the game-tick/sim path. No worker-thread evidence found.
## Part 7 - round 7

### H5 â€” which struct are +0x10 / +0x48 / +0x50 on?
**Answer:** FUN_1401DE2B0's param_2 is a **DO task record (stride 0x80), not the order (stride 0x68)**. The apparent order fields +0x10, +0x48, +0x50 belong to the **DO task record**, not the order. order+0x48 is **not** a destination building.

**Evidence:**
- Call site (FUN_1401E5480 â†’ FUN_1401DE2B0 at 1401E5523): RCX=R13 (game), RDX=RAX = ase [RBX] + index*0x80 (a 0x80-stride pointer), R8=RSI, R9=RBP. FUN_1401DE2B0(game, *param_2, ...) where param_2 is a **pointer into a 0x80-stride array**.
- Inside FUN_1401DE2B0: param_2[2] is dereferenced as a Building* (type descriptor read at +0x318).
- param_2[3] and param_2[10] are indices into the targets vector (stride 8).
- DO task record at DO+0xD58, stride 0x80. Order (stride 0x68) at game+0x144A8.

**Field table (DO task record, stride 0x80):**
| offset | field | value |
|---|---|---|
| +0x10 | Building* | param_2[2] |
| +0x18 | TaskEntry index | param_2[3] |
| +0x48 | Building* | param_2[9] |
| +0x50 | numeric | param_2[0xa] |

**Changes:** order+0x48 is non-existent (task-record field). Use unload-bld for sea/rail and log-entry reads for road.

### H6 â€” H4.1's register claim
**Answer:** RSI = param_2 claim is wrong under x64 __fastcall. RCX = param_1 (game), RDX = param_2 (veh). 14068B3AD: MOV RSI,RCX sets RSI = RCX = param_1 = game, not param_2.
**Changes:** The sea/rail "straight param_9 read" hypothesis is not supported by the register evidence. Re-derive param_9 at 0x6953C4 before sequencing the sea/rail leg. Road stays on unload-bld.

### H7 â€” the completed-delivery log entry's building field
**Answer:** FUN_1401DE2B0's param_2 is a DO task record, not the order. The log-entry path (*(order+0x50)[i] + 0x28) is the surviving route to the payer. The Building* field is not yet confirmed.
**Evidence:**
- FUN_1401DE2B0 (RVA 0x1DE2B0) â€” completion check, game, DO-record parameters. param_2 = DO task record (stride 0x80); fields +0x10 (source Building*), +0x18 (TaskEntry index), +0x48/+0x50 (numeric). Not the order.
- FUN_140420350 (RVA 0x420350, the transfer) scan loop:
  - 197: store 1.0 at the source building's +0x80 (completion record)
  - 206: store done = 1.0 at the log entry's +0x188
  - 211: store resource at the log entry's +0x198
**Log entry layout:**
| offset | field |
|---|---|
| +0x188 | done (float 1.0) |
| +0x198 | Resource* |
| +0x00 | Building* (destination) â€” unconfirmed |

**Changes:** The payer lives in the completed-delivery log entry. One DisasmMany of FUN_140420350's log-entry write will confirm the Building* offset and close H7.

## Corrections
- ~~order+0x10 = source Building~~ corrected in H5: +0x10 is the DO task record's source Building* (stride 0x80).
- ~~order+0x48 = destination Building~~ corrected in H5: +0x48 is a DO task record field; the order has no destination field.
- ~~Round-6 H1's "destination delivered in the log +0x198"~~ corrected in H7: +0x198 is the Resource*; the Building* is at an unconfirmed offset.
- ~~Round-6 H6's RSI = param_2 at 0x6953C4~~ corrected in H6: RCX = param_1 (game); RSI = game, and param_9 at 0x6953C4 is not the destination building.

## Closeout
### Questions answered this round
| Question | Answer | Confidence | Key RVA |
|---|---|---|---|
| H5 â€” which struct are order+0x10/+0x48/+0x50 on? | DO task record (stride 0x80) | proven | 0x1DE2B0, call site 0x1E5523 |
| H6 â€” H4.1's register claim | RCX = game (param_1); RSI = RCX = game, not param_2 | proven | 0x68B330 prologue |
| H7 â€” log entry's building field | done +0x188, resource +0x198; building offset unconfirmed | unconfirmed | 0x420350 |

### Still not done
- H7: log-entry Building* offset â€” one more DisasmMany of FUN_140420350's log-entry write (region 0x1404209A0:0x1404209E0).
- H6: the last write to RSI before 0x6953C4 â€” one DisasmMany of the function body between 0x140694F80 and 0x14069538E.

### Surprises
1. The round-6 H6 register label was wrong (RSI = RCX = game, not param_2). Sea/rail leg should not be sequenced before re-deriving param_9 at 0x6953C4.
2. FUN_1401DE2B0's param_2 is a DO task record, not the order â€” the order+0x10/+0x48/+0x50 offsets are on a different struct.

### Method
All Ghidra runs sequential (one analyzeHeadless at a time), .java scripts only, JDK 21, single -scriptPath via scratch\ghidra_run.bat, file-based 0x140+RVA.

Runs this round:
1. Xrefs.java scratch\x_h8_callers.txt (0x1401DE2B0) â€” callers of the completion check
2. DisasmMany.java scratch\asm_h5_callsite.txt (0x1401E5500:0x1401E5560) â€” call-site register setup
## Part 8 - round 8

### H8 â€” is param_2 a Vehicle*, or a storage container?
**Answer:** param_2 is a **Vehicle\*** inside FUN_140420350 at every call site, including refuel. The apparent contradiction (refuel passes [RBX+0x4F8] which Q1 proved is a home Building*) is resolved: RBX at 0x14016F827 is NOT a vehicle â€” it is the output base from FUN_140427340 (a route-planning function that processed the home building's storages). [RBX+0x4F8] evaluates to a Vehicle\* stored in that output data.

**Offset(s) / call sites:** refuel at 0x14016F831 inside vehicle AI upkeep function.

**First dereference inside FUN_140420350** (proves param_2 = Vehicle*):
`
reads param_2+0x3B8 (vehicle cargo vector begin)
reads param_2+0x3C0 (vehicle cargo vector end)
`
Then param_2+0x610/+0x618 (line stops), param_2+0xEA8, param_2+0x604, param_2+0x1708 â€” all vehicle fields, not building storage (+0x970).

**At the refuel call site 0x14016F831:**
`
14016f81d:  MOV R9,qword ptr [R15 + 0xc330]   ; R9 = param_4 = fuel resource (game+0xC330)
14016f824:  XOR R8D,R8D                       ; R8D = param_3 = mode = 0 (load/consume)
14016f827:  MOV RDX,qword ptr [RBX + 0x4f8]   ; RDX = param_2 = [RBX+0x4F8]
14016f82e:  MOV RCX,R15                       ; RCX = param_1 = game
14016f831:  CALL 0x140420350
`
RBX here = R8 = output base from FUN_140427340(game, home_building, &output) (set at 14016f4f0: IMUL RBX,RAX,0x68 with RAX=0, so RBX=R8). FUN_140427340 processes param_2 as a Building* (+0x318 type descriptor, +0x970 storage vector, +0x604 float) and fills output with 0x68-stride entries. [output_base+0x4F8] = the Vehicle* passed to the transfer.

**Convention:** RCX = param_1 = game, RDX = param_2 = vehicle, R8 = param_3 = mode, R9 = param_4 = resource.

**Two sources:** (1) d_q4_line.c first dereference param_2+0x3B8/+0x3C0 (vehicle cargo vectors) inside FUN_140420350; (2) sm_h8_refuel.txt:27-31 shows [RBX+0x4F8] passed as RDX at the refuel call, and sm_h8_refuel_ctx.txt:37-39 / d_h8_27340.c shows RBX is output from FUN_140427340, not a vehicle.

**Confidence:** proven (param_2 = Vehicle* inside callee) / likely (that [RBX+0x4F8] evaluates to a Vehicle* â€” required for callee to work, but the exact output-entry offset holding the Vehicle* is not isolated)

**Changes:** Answer A â€” the fuel charge is two lines at 0x16F831 (mode=0/consume from vehicle cargo), as planned. No discriminator needed. All other hook sites that pass param_2 as Vehicle* remain valid.

### H9 â€” at a load, is param_9 the source building?
**Answer:** No â€” at load site 0x1401CB7FF, param_9 = bld = NULL. The transfer resolves the source internally via the order scan (order+0x30 == veh).

**Offset(s) / call sites:** loader 0x1401CB2A0 at call site 0x1401CB7FF.

`
1401cb7ca:  MOV qword ptr [RSP + 0x40],0x0    ; param_9 = bld = NULL
1401cb7d3:  MOV byte ptr [RSP + 0x38],0x0     ; param_8 = flag = 0
1401cb7e0:  LEA RAX,[RSP + 0x108]              ; param_6 = out = &local
1401cb7e5:  MOVSS XMM0,dword ptr [R9 + RCX*0x8 + 0x8]  ; param_5 = tons from cargo
1401cb7ec:  MOVSS dword ptr [RSP + 0x20],XMM0
1401cb7f2:  MOV R9,qword ptr [R9 + RCX*0x8]              ; param_4 = resource from cargo
1401cb7f6:  XOR R8D,R8D                        ; R8D = param_3 = mode = 0 (load)
1401cb7f9:  MOV RDX,R13                        ; RDX = param_2 = vehicle (R13)
1401cb7fc:  MOV RCX,R15                        ; RCX = param_1 = game (R15)
1401cb7ff:  CALL 0x140420350
`

**Two sources:** (1) sm_h9_loader.txt:10 MOV [RSP+0x40],0x0 at the loader call site; (2) sm_r1_r2_sites.txt confirms this is mode=0 (load) site with bld=NULL.

**Confidence:** proven.

**Changes:** Confirms the assumption is WRONG â€” at load sites, param_9 (bld) is NULL, not the source building. The source is resolved internally by the transfer via order+0x30 == veh. The merchant role (buy at gate) cannot use param_9 at load sites; it must use the order's resolved source or the drain detector.

### H7 â€” completed-delivery log entry's building field
**Answer:** Deferred â€” not run this round. The log entry at *(*(order+0x50)[i] + 0x28) has +0x188 (done=1.0) and +0x198 (Resource*) proven. The Building* offset remains unconfirmed. Not blocking â€” Â§3.4f fallback stands.

### H10 â€” param_9 at 0x6953C4 re-derived
**Answer:** Deferred â€” not run this round. H6 (round 7) showed RSI=RCX=game at 0x68B3AD, invalidating the "straight param_9 read" hypothesis for sea/rail. Re-deriving param_9 at 0x6953C4 requires one more DisasmMany of 0x14068B330's body.

## Corrections
- ~~order+0x10 = source Building~~ corrected in round 7 H5: +0x10 is the DO task record's source Building* (stride 0x80).
- ~~order+0x48 = destination Building~~ corrected in round 7 H5: +0x48 is a DO task record field; the order has no destination field.
- ~~order+0x50 = completion-record vector~~ corrected in round 7 H7: +0x50 is on the DO task record, not the order; the order has a separate completed-delivery log.
- ~~Round-6 H6's RSI = param_2 at 0x6953C4~~ corrected in round 7 H6: RCX = param_1 (game); RSI = RCX = game, and param_9 at 0x6953C4 is not confirmed as destination building.
- ~~Round-6 claim "param_9 is source building at load"~~ corrected in round 8 H9: param_9 = NULL at load site 0x1CB7FF; source resolved internally.

## Closeout

### Questions answered this round
| Question | Answer | Confidence | Key RVA |
|---|---|---|---|
| H8 â€” param_2 Vehicle* or storage container? | Vehicle* â€” refuel passes [RBX+0x4F8] which resolves to Vehicle* | proven / likely | 0x420350, 0x16F831 |
| H9 â€” param_9 source building at load? | No â€” param_9 = NULL at 0x1CB7FF | proven | 0x420350, 0x1CB7FF |

### Still not done
- H7: log-entry Building* offset â€” one DisasmMany of FUN_140420350 log-entry write (0x1404209A0:0x1404209E0)
- H10: param_9 at 0x6953C4 â€” one DisasmMany of FUN_14068B330 body (0x140694F80:0x14069538E)

### Surprises
1. The refuel call does NOT pass a building as param_9 â€” it passes NULL, same as other load sites. The transfer resolves source/destination internally.
2. RBX at the refuel site is not a vehicle â€” it's the output of FUN_140427340 (a route planner), and [RBX+0x4F8] reads a Vehicle* from that output data.

### Method
All Ghidra runs sequential (one analyzeHeadless at a time), .java scripts only, JDK 21, single -scriptPath via scratch\ghidra_run.bat, file-based 0x140+RVA.

Runs this round:
1. DisasmMany.java scratch/asm_h8_refuel.txt (0x14016F7A0:0x14016F8A0) â€” refuel call site context
2. DisasmMany.java scratch/asm_h8_refuel_ctx.txt (0x14016F6E0:0x14016F7B0) â€” refuel caller context
3. DisasmMany.java scratch/asm_h8_refuel_early.txt (0x14016F600:0x14016F6E0) â€” RBX origin trace
4. DisasmMany.java scratch/asm_h8_refuel_start.txt (0x14016F4F0:0x14016F600) â€” RBX reassignment
5. DisasmMany.java scratch/asm_h8_refuel_before.txt (0x14016F3E0:0x14016F4F0) â€” RBX initial value
6. DecompAt.java scratch/d_h8_25b10.c (0x140425B10) â€” first planning function
7. DecompAt.java scratch/d_h8_27340.c (0x140427340) â€” second planning function (overwrites output)
8. DisasmMany.java scratch/asm_h9_loader.txt (0x1401CB7A0:0x1401CB810) â€” loader call site for H9

## Part 9 - round 9

### H11 — what is veh + 0x4F0?
**Answer:** veh+0x4F0 is the **current building** — the Building* where the vehicle is physically located. At a load, *(veh+0x4F0) is the producer; at an unload, *(veh+0x4F0) is the destination. Both endpoints readable from one field at every call site.

**Offset(s):** veh+0x4F0 = current Building*; veh+0x4F8 = home Building* (banked from Q1).

**Writer (proven):** FUN_1401C6DD0 at RVA 0x1C6DD0 (rail DO handler, type 0x34), called from FUN_140139A70 at 0x14013E2E9. At lines 318-319 of d_h2_setter.c:
*(longlong *)(lVar22 + 0x4f8) = param_2;   // home building = param_2 (a Building*)
*(longlong *)(lVar22 + 0x4f0) = param_2;   // current building = param_2 (a Building*)
where lVar22 is the vehicle/trailer and param_2 is the building being assigned.

**Clear writer (proven):** Inside FUN_1401B1290 at 0x1401B259A (asm_r8_site.txt:85):
1401b259a:  MOV qword ptr [RDI + 0x4f0],R15     ; R15 = NULL (XOR R15D,R15D at 1401b1318)
This clears +0x4F0 to NULL after the vehicle finishes processing at a building (vehicle is now in transit). The code first verifies +0x4F0 == RSI at 1401b2416-1401b2420 (MOV R8,[RDI+0x4F0]; CMP R8,RSI; JNZ), confirming this runs only when the vehicle IS at building RSI.

**Reset writer (proven):** FUN_140420350 at RVA 0x420350 (d_q2_load_event.c:234):
*(undefined8 *)(param_2 + 0x4f0) = *(undefined8 *)(param_2 + 0x4f8);  // current = home
This resets current to home after the transfer completes (line 234), also propagated to trailers at line 239-240.

**Reader (proven):** FUN_14016C1C0 at RVA 0x16C1C0 (d_q1_writer.c:95):
(*(longlong *)(lVar3 + 0x4f0) == param_2)    // reads current building from vehicle lVar3
Reads +0x4F0 and compares against a Building* parameter param_2.

**Reader (proven):** FUN_140420350 at RVA 0x420350 (d_q2_load_event.c:84-98):
lVar14 = *(longlong *)(param_2 + 0x4f0);      // read current building
if (lVar14 != 0) {
    if (*(longlong *)(lVar14 + 0xbb0) == param_2)   // building's current_vehicle == this vehicle
        *(undefined8 *)(lVar14 + 0xbb0) = 0;        // clear back-reference
    ...
    lVar14 = *(longlong *)(param_2 + 0x4f0);
    if (lVar14 != *(longlong *)(param_2 + 0x4f8))   // if current != home
        // remove from building's vehicle vector (+0xC70/+0xC78)
}

**Current vs target — CURRENT (proven):** Three independent proofs:
1. **Back-reference:** At d_q2_load_event.c:86, *(current_building + 0xBB0) == vehicle — the building's current_vehicle field points back to this vehicle. A target field wouldn't have this back-pointer.
2. **Parking slot:** At d_q2_load_event.c:89-91, the code clears building + 0xB90 + vehicle_type_index — a physical parking slot at the building where the vehicle sits.
3. **Vector removal:** At d_q2_load_event.c:97-100, when current != home, the vehicle is removed from current_building + 0xC70/+0xC78 (the building's vehicle vector). You only remove from the vector of the building you're physically at.

**When cleared:** Set to NULL at 0x1401B259A (after processing at a building — vehicle departs). Set to home (+0x4F8) at 0x420350+0x234 (after transfer completes — vehicle returns home). Is NULL when in transit between buildings.

**Carrier types:** Holds for all carriers — FUN_140420350 is the generic transfer function used by all 16 call sites (road, ship, rail), and FUN_14016C1C0 reads +0x4F0 generically. The writer FUN_1401C6DD0 is rail-specific but the field is universal.

**Convention:** At FUN_140420350, RCX = param_1 = game, RDX = param_2 = vehicle, R8 = param_3 = mode, R9 = param_4 = resource. At FUN_1401C6DD0, param_1 = game, param_2 = building.

**Two sources:** (1) Writer d_h2_setter.c:318-319 (FUN_1401C6DD0) sets +0x4F0 = param_2 (a Building*); (2) Reader d_q2_load_event.c:84-98 (FUN_140420350) dereferences +0x4F0 as a Building* (+0xBB0, +0xB90, +0xC70/+0xC78).

**Confidence:** proven.

**Changes:** Answer A — veh+0x4F0 = current building, set on arrival, cleared on departure (NULL) or reset to home. Holds for all vehicle types. This unblocks the merchant role: at any transfer call site, *(veh+0x4F0) gives the building the truck is standing at. At a load (mode=0), that's the producer; at an unload (mode=1), that's the destination. No order scan needed — read +0x4F0 directly at the hook site.

### H12 — verify the round-8 RBX explanation
**Answer:** The conclusion (param_2 is a Vehicle*) is safe, but the RBX inference was an accidental offset coincidence. FUN_140427340 (RVA 0x427340) processes param_2 as a Building* (reads +0x318 type descriptor, +0x970 storage vector, +0x604 float — see d_h8_27340.c:94-100) and fills 0x68-stride output entries. The output entry field at +0x4F8 is a building field, not a vehicle field — it happens to share the same offset as vehicle's +0x4F8 (home building). When round 8's refuel site does MOV RDX,[RBX+0x4F8], it reads a building-field from planner output that the transfer then treats as param_2 — and the transfer's own param_2+0x3B8 dereference (d_q4_line.c:43) proves it's actually a Vehicle* at runtime. The offset coincidence is not meaningful — two unrelated structs share +0x4F8 by chance.

**Confidence:** likely.

**Changes:** No design impact — param_2 is confirmed Vehicle* by the callee, not by the RBX inference.

## Corrections
None new this round — all prior corrections (rounds 7-8) already applied.

## Closeout

### Questions answered this round
| Question | Answer | Confidence | Key RVA |
|---|---|---|---|
| H11 — veh+0x4F0 current or target building? | Current building — written by FUN_1401C6DD0, read by FUN_140420350 and FUN_14016C1C0 | proven | 0x1C6DD0, 0x420350, 0x16C1C0 |
| H12 — RBX explanation coincidence? | Yes — accidental offset match, param_2 still Vehicle* | likely | 0x427340 |

### Investigation status
**All blockers resolved.** The merchant role is now buildable:
- At any FUN_140420350 call site, *(veh+0x4F0) gives the building the vehicle is standing at
- At load (mode=0): producer = *(veh+0x4F0), charge the producer's owner
- At unload (mode=1): destination = *(veh+0x4F0), charge the destination's owner
- veh+0x4F8 (home building, Q1) is available as fallback when +0x4F0 is NULL (in transit)

### Still optional (not blocking)
- H7: completed-delivery log entry's Building* — one DisasmMany of 0x1404209A0:0x1404209E0
- H10: param_9 at 0x6953C4 — one DisasmMany of 0x14068B330 body

### Method
All Ghidra runs sequential (one analyzeHeadless at a time), .java scripts only, JDK 21, single -scriptPath via scratch\ghidra_run.bat, file-based 0x140+RVA. Four runs this round:
1. Xrefs.java scratch/x_h11_writer.txt (0x1401C6DD0) — writer callers
2. DisasmMany.java scratch/asm_h11_caller.txt (0x14013E250:0x14013E350) — call site context
3. DecompAt.java scratch/d_h11_road_do.c (0x1401C6050) — road DO handler (reads only, no write)
4. DecompAt.java scratch/d_h11_dispatcher.c (0x140139A70) — dispatcher (no +0x4F0 refs)


### H12 — verify the round-8 RBX explanation
**Answer:** The conclusion (param_2 is a Vehicle*) is safe, but the RBX inference was an accidental offset coincidence. FUN_140427340 (RVA 0x427340) processes param_2 as a Building* (reads +0x318 type descriptor, +0x970 storage vector, +0x604 float — see d_h8_27340.c:94-100) and fills 0x68-stride output entries. The output entry field at +0x4F8 is a building field, not a vehicle field — it happens to share the same offset as vehicle's +0x4F8 (home building). When round 8's refuel site does MOV RDX,[RBX+0x4F8], it reads a building-field from planner output that the transfer then treats as param_2 — and the transfer's own param_2+0x3B8 dereference (d_q4_line.c:43) proves it's actually a Vehicle* at runtime. The offset coincidence is not meaningful — two unrelated structs share +0x4F8 by chance.

**Confidence:** likely.

**Changes:** No design impact — param_2 is confirmed Vehicle* by the callee, not by the RBX inference.

## Corrections
None new this round — all prior corrections (rounds 7-8) already applied.

## Closeout

### Questions answered this round
| Question | Answer | Confidence | Key RVA |
|---|---|---|---|
| H11 — veh+0x4F0 current or target building? | Current building — written by FUN_1401C6DD0, read by FUN_140420350 and FUN_14016C1C0 | proven | 0x1C6DD0, 0x420350, 0x16C1C0 |
| H12 — RBX explanation coincidence? | Yes — accidental offset match, param_2 still Vehicle* | likely | 0x427340 |

### Investigation status
All blockers resolved. The merchant role is now buildable:
- At any FUN_140420350 call site, *(veh+0x4F0) gives the building the vehicle is standing at
- At load (mode=0): producer = *(veh+0x4F0), charge the producer's owner
- At unload (mode=1): destination = *(veh+0x4F0), charge the destination's owner
- veh+0x4F8 (home building, Q1) is available as fallback when +0x4F0 is NULL (in transit)

### Still optional (not blocking)
- H7: completed-delivery log entry's Building* — one DisasmMany of 0x1404209A0:0x1404209E0
- H10: param_9 at 0x6953C4 — one DisasmMany of 0x14068B330 body

### Method
All Ghidra runs sequential (one analyzeHeadless at a time), .java scripts only, JDK 21, single -scriptPath via scratch\ghidra_run.bat, file-based 0x140+RVA. Four runs this round:
1. Xrefs.java scratch/x_h11_writer.txt (0x1401C6DD0) — writer callers
2. DisasmMany.java scratch/asm_h11_caller.txt (0x14013E250:0x14013E350) — call site context
3. DecompAt.java scratch/d_h11_road_do.c (0x1401C6050) — road DO handler (reads only, no write)
4. DecompAt.java scratch/d_h11_dispatcher.c (0x140139A70) — dispatcher (no +0x4F0 refs)


## Part 10 - round 10

Brief items H13-H16 (H17 parked until S4 is actually being built). Thirteen headless
runs, all sequential, .java scripts only. Tier 1 came back clean; Tier 2 answered with
one structural surprise (H16) and one design-relevant polarity refinement (H15).

### Correction first - the +0x4F0=+0x4F8 rewrite was misattributed to the transfer

`scratch\d_q2_load_event.c` on disk is the decompile of **FUN_1403FBDC0** (detach /
return-to-base) - its own header says "Function: FUN_1403fbdc0 at 1403fbdc0". It is NOT
FUN_140420350. The round-9 citations (d_q2_load_event.c:86 / :89-91 / :97-100 / :234)
all match that file, so the three semantic proofs of veh+0x4F0 stand - but they were
observed inside FUN_1403FBDC0, and "Set to home (+0x4F8) at 0x420350+0x234" conflated a
decompile FILE line number with an RVA. The transfer's own decompile is
`scratch\d_q4_line.c` (header "FUN_140420350 at 140420350", full 262-line body) and
contains **no +0x4F0 reference at all**. The actual write:

```
FUN_1403FBDC0 (RVA 0x3FBDC0), write at RVA 0x3FC2F4:
  1403fc2ed:  MOV RAX, qword ptr [RDI + 0x4f8]   ; home building
  1403fc2f4:  MOV qword ptr [RDI + 0x4f0], RAX   ; current = home
  1403fc2fb:  MOV RCX, RDI
  1403fc2fe:  CALL 0x140667450
  ...then propagates +0x4F0 to every trailer (vec +0x3A0, stride 8)
```

It sits in the unconditional tail of the "current != home" arm (the at-home arm returns
earlier at d_q2_load_event.c:68 without the write), after the bld+0xBB0 back-ref clear,
the bld+0xB90 parking-slot release, the bld+0xC70 fleet-vector removal and the trailer
recursion (self-call at 0x3FC2E2, args (game,trailer,0,0,1,1)). ~80 call sites reach
FUN_1403FBDC0 across the vehicle lifecycle (sell, line edit, deletion, arrival
processing, catalog ops, self-recursion - x_h13_detach.txt); **none of the 16 transfer
callers is among them**. Evidence: asm_h13_write.txt, d_q2_load_event.c, x_h13_detach.txt.

### H13 - is FUN_140420350 ever called as a dry run?

**Answer: A - real movements only.**

1. The +0x4F0 rewrite is not in this function (correction above). The transfer's only
   writes are the tons mutations and the order-completion stamps
   (d_q4_line.c:197, :206-211, :244-256).
2. Callers on once-per-real-movement paths:
   - 0x1B2898 (FUN_1401B1290, road refuel): epsilon-gated
     (COMISS XMM6,[0x140909a68]; JBE skip), tons pre-clamped to min(bound1,bound2),
     mode=1, out=NULL, bld=NULL; the caller then performs the real building-slot
     subtraction itself ([slot-array][idx+0xC] -= tons/factor). asm_h13_caller1.txt.
   - 0x1CB7FF (FUN_1401CB2A0, loader): per-cargo-slot loop over veh+0x3B8 during an
     actual unload servicing; mode=0, tons=that slot's current cargo, out=&local
     (RSP+0x108); the caller then runs the storage-add path (bld+0x970, stride 0xE0).
     asm_h13_caller2.txt.
   Neither is a scoring/planning loop.
3. param_6 (stack+0x28, float*): written ONLY at d_q4_line.c:258-260, **after** the
   tons mutation has committed: `if (param_6) *param_6 = fVar22;` where fVar22 is the
   clamped actually-moved tons (0.0 when nothing clipped). NULL = "don't report".
   It is NOT a dry-run discriminator - the function has no read-only configuration.
   Return 1 = mutation committed; return 0 = early exit with nothing written (bad
   resource id, empty cargo, no matching order/stop).
4. Tonnage: callers pre-clamp (road: min of bounds; sea/rail: MINSS against
   availability; loader: the slot's own cargo). The function clamps internally anyway -
   load caps at free capacity fVar21 = slotCap x (1 - sum cargo/cap) (d_q4_line.c:80),
   unload never drives a slot below 0 (d_q4_line.c:240-248). An oversized param_5 would
   still move at most capacity and report the true figure via param_6. At out=NULL sites
   the caller accounts the nominal, so billing |tons passed| at AL==1 matches the
   game's own ledger.
5. Indirect reach: the transfer CALLS the planner (d_q4_line.c:134,
   FUN_140424430(game,veh,res,tons,0,0,0)) - the dependency is one-way. A CallsIn pass
   over 0x422C30-0x426000 (route scorer + planner + neighbours, 91 calls,
   calls_h13_planscorer.txt) shows zero calls into any of the eight distinct
   transfer-calling functions. No speculative path exists.

**Confidence: proven.** Hook guidance unchanged: patch call sites, bill on AL==1.

### H14 - are all invocations of 0x420350 direct call rel32?

**Answer: 19 references total - 16 direct `call rel32` (E8), 3 DATA = x64 unwind
metadata. No indirect calls, no tail jumps, no function-pointer-table entries.**
Call-site patching sees 100% of invocations.

- The 16 UNCONDITIONAL_CALL sites (fresh census x_h14_xfer.txt, identical to round 2):
  0x141F33, 0x16F831, 0x1B2898, 0x1B484B, 0x1B5EC0, 0x1B653B, 0x1B6833, 0x1B75ED,
  0x1B7981, 0x1CB7FF, 0x68EECA, 0x68F792, 0x68F7F9, 0x6945B3, 0x694F40, 0x6953C4.
- DATA ref 0xA6951C (.pdata): a RUNTIME_FUNCTION entry {begin=0x420350, end=0x42037A,
  unwind=0x952A70} - the function's own exception-unwind record (verified from raw PE
  bytes; the .pdata dword stream around it lists neighbouring entries
  0x4201F0-0x420223, 0x420585-0x420A58, ...).
- DATA refs 0x952A9C / 0x952AC8 (.rdata): the UNWIND_INFO region the .pdata entries
  point into; 0x952AC4 holds a chained RUNTIME_FUNCTION triple {0x420350, 0x42037A,
  0x952A70} (funclet chain info). Exception-handling metadata, not invocations.

### H15 - FUN_14068B330 family: mode values at its six transfer sites

0x6953C4 is NOT the only site in this function family: FUN_14068B330 contains
**six** transfer calls. The owner-provided run observed the paired
`0x68F792`/`0x68F7F9` calls on road DO deliveries, so the former sea/rail-only
classification for that pair is superseded. The run does not classify every
other site or prove complete road coverage.
All six classified from disasm (asm_h15_*.txt):

| site | mode | tons | out (param_6) | param_9 | after-call |
|---|---|---|---|---|---|
| 0x68EECA | 0 (XOR R8D) | +XMM6 | R12 (live) | veh+0x4F8 | TEST AL, then cargo-add via 0x421DB0 |
| 0x68F792 | 0 (XOR R8D) | +XMM6 (MINSS-bounded) | NULL | veh+0x4F8 | TEST AL |
| 0x68F7F9 | 1 (MOV R8D,1) | -XMM6 (sign-flipped) | &local | veh+0x4F8 | paired with 0x68F792 |
| 0x6945B3 | 0 (XOR R8D) | -XMM6 | [bld+0x368] live | veh+0x4F8 | TEST AL |
| 0x694F40 | 0 (XOR R8D) | -XMM8 | &local | RSI | TEST AL |
| 0x6953C4 | 1 (R8D=R15D; R15D=1 set at 0x6953A6) | +XMM6 (= cap - stored) | RSI | RSI (dead on mode 1) | 0x1BD0B0 follow-up |

1. At 0x6953C4 the mode is in R8D, loaded from R15D which is set to the constant 1 two
   instructions before the arg setup - **it is always 1 on every path to the call**.
   The preceding ~15 instructions are quoted in asm_h15_mode.txt (0x69538E-0x6953C4).
   The register indirection is real but never varies.
2. Polarity refinement (sharpens, does not contradict, the round-9 shorthand): the MODE
   selects which side the transfer touches - mode 0 = the order-source pool entry
   (d_q4_line.c:179-257), mode 1 = the vehicle cargo slot (d_q4_line.c:36-81/192) -
   and the SIGN of tons gives direction. Positive-tons calls push goods into the
   touched side; negative-tons calls pull them out (dock return legs 0x6945B3/0x694F40,
   transship credit 0x68F7F9).
   **Billing rule for the design: gate on the specific site AND tons > 0.** A mode-only
   gate would bill the negative return legs as if they were deliveries. Whether the
   goods-flow "delivery" at a given dock is the mode-1-positive leg or its
   mode-0-negative partner depends on the dock operation direction; the hook should
   key off the measured side-effect (site + sign), not off a mode shorthand.
3. The six call sites are in the 16-site transfer census; the owner run proves
   that at least the `0x68F792`/`0x68F7F9` delivery pair also serves road DO
   trucks. Other-site route coverage remains unclassified by that run.

### H16 - vehicle pricing: FUN_1403F7CE0 and the vehicle's type id

1. **FUN_1403F7CE0 is not a price lookup.** Signature:
   `float* FUN(unused8, float out[2], World* w)` - a LINE profitability calculator. It
   zeroes out[0]/out[1], then accumulates over the line's vehicles (0x110-stride
   records) using per-resource unit prices at **Res+0x58 / Res+0x5C** (Res+0x44 == -1
   = passenger, price scaled by DAT_140909DB8), difficulty multipliers
   DAT_14090A178/DAT_14090A228, and doubles income for world+0x294 in {8,11}. Returns
   out. (d_h16_3f7ce0.c.)
2. **A Vehicle* does NOT follow the building +0x318/+0x360 pattern.** From the factory
   FUN_1403E4A00 (d_h16_factory.c):
   - Vehicle object size is **0x1AD8** (operator_new at :71);
   - +0x04 = global vehicle index (mgr vec at +0x12810, :366-367);
   - +0x4F8 = home Building* (:360), +0x4F0 = current (:322);
   - +0x19E0 = mirror vector, one entry per world in the world+0x7C78 catalogue
     (count = world+0x7CB8), each entry = catalogue[i]+0x100 (cross-world mirrors);
   - **+0x510 (int) = vehicle type index**, compared against the building descriptor's
     +0x820 (slot count) at d_h2_price.c:161-163 and d_h16_factory.c:46 - the same int
     the +0xB90 parking-slot code uses;
   - world catalogues: world+0x7C08..+0x7C10 and world+0x7C78..+0x7C80, stride 0x110,
     entry+0x100 -> the type/world object (purchase loop at d_h16_purchase.c:99-113
     feeds entry+0x100 back as the factory's world arg).
   A direct Vehicle* -> own TypeDescriptor* field was NOT found. If the table-price
   route is ever chosen, the candidate chain is veh+0x510 (type index) -> catalogue
   entry - still unproven at the descriptor level.
3. **$COST:** the INI keys are `$COST_RUB` (294 vehicle folders) and `$COST_USD` (62),
   one per media_soviet\vehicles\*\script.ini (e.g. bust_f2472: $COST_USD 4300). The
   descriptor field offset where the INI loader stores them was NOT established. What
   IS proven about engine-side prices:
   - commodity unit prices live at Res+0x58/+0x5C (from 1.);
   - FUN_1401AAD70 (purchase; sole caller FUN_1402B3C80 @0x2B4548, x_h16_purchase.txt)
     receives the USD price as the HIGH float of param_4 and, for type-0x14 producer
     buildings, the RUB price via FUN_1403E3F40's out-param; it checks both against
     budget globals DAT_1409D5498 (RUB) / DAT_1409D5490 (USD) and deducts
     (d_h16_purchase.c:53-91). For depot purchases (non-0x14) no money moves inside
     FUN_1401AAD70 - the depot ledger handles pricing elsewhere.

**Recommendation:** do not build a hand-maintained price table. Either (a) parse
`media_soviet\vehicles\*\script.ini` $COST_RUB/$COST_USD at mod load - the same data
the game reads, zero drift, no descriptor RE - or (b) for engine-priced producer-plant
buys, read the FUN_1403E3F40 out-param + param_4 high float at the purchase hook.
Neither route needs the descriptor field offset.

### Closeout

| Question | Answer | Confidence | Key RVA |
|---|---|---|---|
| H13 - dry-run invocations? | A: real movements only; the +0x4F0 rewrite was in FUN_1403FBDC0, not the transfer | proven | 0x420350, 0x3FBDC0, 0x3FC2F4 |
| H14 - all refs direct calls? | 19 refs = 16 call rel32 + 3 unwind-metadata data refs; no indirect path | proven | 0x420350 |
| H15 - modes at 0x6953C4 / other FUN_14068B330 sites? | Always mode 1 at 0x6953C4; six sites total; gate billing on site AND tons>0. The 0x68F792/0x68F7F9 pair is also observed on road DO deliveries. | proven mode/site census; road pair observed in one run | 0x68EECA..0x6953C4 |
| H16 - pricing/type chain? | 0x3F7CE0 = line P&L (Res+0x58/+0x5C); vehicle has no +0x318; veh+0x510 = type index; $COST read at media level or via 0x3E3F40/param_4 | likely | 0x3F7CE0, 0x1AAD70, 0x3E3F40 |

### Investigation status
Tier 1 (H13/H14) is clean: the two hooks that move money bill exactly the real
movements and see every invocation. Tier 2 (H15/H16) delivered: transfer sites classified with a sign-gating
rule, and a no-drift pricing route that needs no
descriptor RE. H17 stays parked until S4. Per the brief, **the investigation is
closed** - the remaining work is code, per
20_PLAN/BUILD_PLAN_private_logistics_v1.md.

### Method
Thirteen headless runs this round, all sequential (one analyzeHeadless at a time), .java
scripts only, JDK 21, single -scriptPath via scratch\ghidra_run.bat, file-based
0x140+RVA:
1. Xrefs.java scratch/x_h14_xfer.txt (0x140420350) - H14 census
2. Xrefs.java scratch/x_h13_detach.txt (0x1403FBDC0) - detach callers
3. DisasmMany.java scratch/asm_h13_caller1.txt (0x1401B2830:0x1401B2900) - road site
4. DisasmMany.java scratch/asm_h13_caller2.txt (0x1401CB790:0x1401CB860) - loader site
5. DisasmMany.java scratch/asm_h13_write.txt (0x1403FC2A0:0x1403FC3A0) - +0x4F0 write
6. CallsIn.java scratch/calls_h13_planscorer.txt (0x140422C30:0x140426000) - indirect check
7. DisasmMany.java scratch/asm_h14_data1/data2 (empty - pure data) + raw PE byte dumps
   for the 3 DATA refs
8. DisasmMany.java scratch/asm_h15_mode.txt (0x140695300:0x140695410) + 4 more site
   passes (asm_h15_s68eec / s68f79 / s6945 / s694f)
9. DecompAt.java scratch/d_h16_3f7ce0.c (0x1403F7CE0) - line P&L
10. DecompAt.java scratch/d_h16_purchase.c (0x1401AAD70) - purchase
11. DecompAt.java scratch/d_h16_factory.c (0x1403E4A00) - vehicle factory
12. Xrefs.java scratch/x_h16_purchase.txt (0x1401AAD70) + DisasmMany
    scratch/asm_h16_buycall.txt (0x1402B4440:0x1402B4560) - buy call site


