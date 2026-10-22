# Native verification results

Binary: SOVIET64.exe v1.1.1.9 (PE timestamp 0x6A3EB6AD), base 0x140000000.
Date: 10/20/2026. Method: Ghidra 12.1.4 headless (DecompAt/DisasmMany/FindDisp/PrintFloats) over the existing soviet project; every claim confirmed in disassembly (instruction addresses cited), writer claims against FindDisp/xref censuses in `out_nv_*` and `scratch/*`.

Note: `code_review_logic_bugs.md` is not present in this checkout; findings are cited by the numbers used in the verification brief (#2, #4, #5, #7, #12, #15, #22, #24, #25).

## Verdicts

| Q   | Verdict | Confidence | Review finding affected | Effect |
| --- | ------- | ---------- | ----------------------- | ------ |
| 1   | Executor is not idempotent; second call severs the +0xC8 parent link and re-applies the 0.6 wellbeing multiplier; whole family is re-pushed whenever the called person still owns populated +0xB0..0xC0 slots | proven | #4 | stands (fix = one representative per household) |
| 2   | person+0xC8 is a Person* parent link; correct non-resident test is `+0x71C != 0 \|\| +0x748 != 0` | proven | #25 | stands; rename to TSM_PERSON_PARENT, drop the foreign test |
| 3   | Type-2-subtype-1 (dorm) occupants live in +0xBC0; non-resident spawns (+0x71C 1/2) are inserted into a building's +0xBC0 at spawn | likely | #4 / #24 | housing scope confirmed; add tourist gate to rent + relocation |
| 4   | Native checkout debits no person money field; only +0x734/+0x738 accumulators, treasury +0x580/+0x588 and owner +0x3C0/+0x3C4 rise; residents (+0x71C==0) pay nothing | proven | #24 / wallet layer | wallet charge is additive, tourists-only |
| 5   | Residence storage-0 slot content is never written by the housing cluster; +0x8C is capacity-only | likely | #2 | fix stays factory-only |
| 6   | Production output is written into the building's own storage with type token +0x90==0x10 (plain `$STORAGE`, both direction flags zero), not +0x85; both-zero storages exist | likely | #2 | one-condition (+0x84) fix holds |
| 7   | FUN_1401BCE70 debits a building-owned float fund (+0x10E0/+0x10F0/+0x112C), never the game treasury; +0x13E0 readers move no money | proven | #4 note | stays a design note |
| 8   | FUN_14013FE70 touches neither +0xBC0 nor +0xBD8 and never despawns people; occupants left dangling | proven | #4 urgency | pre-evacuation necessary; fix the loop |
| 9   | ~20 write sites in ~18 functions write the treasury doubles; biggest flows are the trade crediter and the shop-tick family | likely | #12 | stands |
| 10   | Customers with +0x71C!=0 are in +0xBD8 and the first 0x71C test (0x1401726CB) is after the give at 0x14017262B | proven | #24 | ship the tourist gate |
| 11   | No VK_F11/0x7A immediate in the polled input cluster; bindings are data-driven strings | likely | #15 | collision risk low |
| 12   | Engine currency pairs are {1: res+0x64 / workers+0x5C / game+0x588} and {2: res+0x60 / workers+0x58 / game+0x580}; treasury labels likely per the vehicle-purchase trace, which makes the mod's res+0x58/+0x5C mapping (and the "1=USD" label) likely inverted | likely | #5/#7/#22 | open new C finding; do not close the family yet |


## Q1 — Is FUN_14083CA50 idempotent, and how is the household enumerated?

**Answer.** Not idempotent — no "already moved" guard; it always operates on P's *current* home (`P+0x20`). Calling it twice for two members of the same household results in the second call removing the member from wherever they now live (already the destination) and re-inserting them (membership survives), but with two harms: (a) the entry block severs the parent link — `P+0xC8` cleared, parent's `+0xB0/0xB8/0xC0` slot zeroed — so the `+0x140C` recount (house_exec.c:158-168) miscounts them; (b) `P+0xD8` is multiplied by `DAT_140909e10` again whenever the entry detach didn't run (house_exec.c:152-153). When the *head* is called (its slots still point at the family after call 1), the whole household is re-pushed — a per-person loop re-moves the family.

**Evidence.** `0x14083CA69`: `MOV RAX,[RDX+0xc8]` … `0x14083CAA2`: `CMP [RAX+0xb0],RDX` / `0x14083CAAB`: `MOV [RAX+0xb0],R12` (R12=0) … `0x14083CAE0`: `MOV [RDX+0xc8],R12`; `0x14083CA89-98`: age gate 21.0 (`COMISS` vs `[0x14090a934]`). Float: `DAT_140909E10 = 0.6` (house_floats2.txt). Slot writers (out_nv_db0/db8/dc0.txt): FUN_140823470@0x140823621/628/62F (spawn), FUN_140834A20@0x140834D41 (indexed write), FUN_140834DE0@0x140835328/343/35E + 0x140835C2B/46/61 (clears), FUN_140835D70@0x140836832/87B/8C4 (despawn), FUN_14083CA50@0x14083CAAB/C2/D9 (clears), FUN_14083E2E0@0x14083E5AA/D0/F8, FUN_140841570@0x1408415E0, FUN_140845DC8@0x14084608B/63DD.

**How this changes the review.** #4 stands as written: the fix must move one representative per household.

## Q2 — Confirm person+0xC8 is the parent link, and find the right "foreign" test

**Answer.** `person+0xC8` is a `Person*` parent/head link, proven at four sites, never a scalar flag. The correct non-resident test is `person+0x71C != 0 || person+0x748 != 0`; both are set only for non-resident spawns and both are zero-tested together on resident-only paths. No separate "foreign resident" concept was found — 0x71C is the retail payer kind (0/1/2) and 0x748 a parallel service-fee kind (1/2).

**Evidence.** `0x14083CA69`: `MOV RAX,[RDX+0xc8]` then `CMP [RAX+0xb0],RDX` (deref into another person's slots); house_exec.c:740-741 derefs `*(P+0xC8)+0x20` as the parent's home; house_exec.c:161 null-tests it to count independent tenants; writers `MOV [RAX+0xc8],RBP` (FUN_140832E90@0x1408335BD), `MOV [RCX+0xc8],RBX` (FUN_140834A20@0x140834D1C), `MOV [RSI+0xc8],RDI` (FUN_140835D70@0x140836509/536). 0x71C writers: FUN_140823470@0x140824063 (=1) / 0x140824075 (=2); 0x748 set at out_nv_dec1.txt:626 (`+0x748 = (*(bld+0x9ec)!=4)+1`); joint zero-test at sol_830820.c:372 and rent_kick.c:372 (`+0x748==0 && +0x71c==0`).

**How this changes the review.** #25 stands: rename to `TSM_PERSON_PARENT`, drop the "skip foreigners" gate, use `+0x71C || +0x748`.

## Q3 — Do tourists and hotel guests appear in a type-2 building's resident vector?

**Answer.** Mostly. Type-2 buildings push occupants into `+0xBC0`: `FUN_14083CA50` chooses the residents-vector push whenever the source typedesc type is not 0x4A/0x4C (`0x14083CB13-0x14083CB35`: `SUB EAX,0x4A; TEST EAX,0xfffffffd; CMOVZ` → flag set iff type ∈ {0x4A,0x4C}), and `typedesc+0x364==1` only sets the flag that suppresses the `+0xD8 *= 0.6` penalty (`0x14083CAF2-0x14083CB0B`); the dorm rent gate treats them as residents (house_trans.c:594: type==2 && subtype==1 → compare `bld+0x13E0` vs `person+0xE4`). On the spawn side (likely link): the spawn function inserts the new person into the building's `+0xBC0` (out_nv_dec1.txt:433) *before* the building-type branch, and for **type-0x14** buildings then assigns `+0x71C=1/2` (lines 580/584) and clears the home pointer (lines 598-599). So `+0x71C!=0` persons do occupy a `+0xBC0` vector; the dorm-guest-carries-71C combination is inferred, not directly sighted.

**Evidence.** `0x14083CAF2`: `CMP dword ptr [RCX + 0x364],0x1`; `0x14083CB13`: `MOV EAX,dword ptr [RCX + 0x360]`; insert loops target `param_3+0xbc0` (house_exec.c:80-127) vs `+0xbd8` (129-151). Dorm rent gate: house_trans.c:594 (`type==2 && subtype==1` → `bld+0x13E0` vs `person+0xE4`); spawn: out_nv_dec1.txt:433 (`puVar1 = param_2+0xbc0`), 579-586 (71C assignment), 626 (`+0x748 = (*(bld+0x9ec)!=4)+1`).

**How this changes the review.** #4's hotel-guest charge/eviction and #24's blast radius apply to housing as well as shops; add a tourist gate to rent and relocation.

## Q4 — Does the native checkout debit the buyer, or only accumulate statistics?

**Answer.** It never debits a person-side money field. The fee path (only when `+0x71C` is 1 or 2) writes: `person+0x734` (spent accumulator), `game+0x588` or `game+0x580` (treasury), global stats `DAT_1409e65fc/e65f8`, and the owner company's `+0x3C4/+0x3C0`. A resident (`+0x71C==0`) pays nothing — both fee blocks are gated on the 0x71C compare. The wallet charge at the same gate is therefore additive (a new sink), not double-charging.

**Evidence.** Table: `person+0x734` R/W (`0x140172764/0x14017276F`); `game+0x588` R/W kind-1 (`0x140172777/780`: `ADDSD/MOVSD [R13+0x588]`); `game+0x580` R/W kind-2 (`0x14017285F/868`); `DAT_1409e65fc` W (`0x140172795`); owner+0x3C4 R/W (`0x1401727AB/B3`). Fee source: `res+0x64` (`0x140172703`: `MULSS XMM6,[RAX+0x64]`) + `workers+0x5C` leg (`0x140172732`); kind-2 uses `res+0x60` (`0x1401727E7`) + `workers+0x58` (`0x140172817`). Gates: `0x1401726CB`: `CMP dword ptr [RSI+0x71c],1`; `0x1401727BB`: `CMP …,2`. Goods are given at `0x14017262B/0x14017263F` before either gate. "Resident pays nothing": yes.

**How this changes the review.** #24 stays tourist-scoped; the wallet layer adds a sink the engine lacks.


## Q5 — Does a residence's passenger storage track occupancy?

**Answer.** No — `slot+0x08` is never written by the housing cluster; `+0x8C` is used only as a capacity number. Occupancy is tracked exclusively via the `+0xBC0` vector.

**Evidence.** `*(bld+0x970)+0x8C` is only read (house_exec.c:795; house_trans.c:415/426/459/483/534/609); no `slot+8` write exists in the decompiles of the housing cluster (house_c6a0.c, house_tool.c, sol_836b40.c, sol_830820.c, house_exec.c, out_nv_dec1.txt) — the only slot+8 writers found are the shop-shelf decrement (`0x14017262B`) and the factory tick (out_factory_tick.c:569/587).

**How this changes the review.** Finding #2 gains no housing vector; fix stays factory-only. A runtime dump of a residence storage-0 after a move-in would upgrade this from likely to proven.

## Q6 — Where does factory production write its output?

**Answer.** `FUN_1401CF590` increases `slot+0x08` on the building's own storages at out_factory_tick.c:587-588 and 1207-1208 (`*(float*)(lVar11+8) = rate*dt + *(float*)(lVar11+8)`), selecting the storage whose type token `+0x90 == 0x10` — a plain `$STORAGE` class, not `$STORAGE_EXPORT`. `FUN_1400E40F0` is the only writer of the direction flags and sets `+0x85=1` solely for `$STORAGE_EXPORT`/`$STORAGE_EXPORT_SPECIAL`; the both-flags-zero case exists (plain `$STORAGE`, `$STORAGE_SPECIAL`, all `$STORAGE_DEMAND_*` — it clears the word at `param_2+0x84` first, g_storparse_E40F0.c:91) and production output lands in such a storage. No evidence that state (import) deliveries target both-zero storages — they target `+0x84` import storages.

**How this changes the review.** #2's one-condition fix ("bill only `+0x84`") is correct: it excludes factory output while still billing state deliveries.

## Q7 — Does the engine ever move money because of bld+0x13E0?

**Answer.** No. `FUN_1401BCE70(game,bld,person,dt)` is called per settled resident (single caller `FUN_140832E90@0x140833687`) and debits `**(float**)(bld+0x10E0)` — a building-owned float fund — crediting `**(bld+0x10F0)` and accumulating `bld+0x112C`; shortfalls accumulate in `bld+0x1104/0x110C`. It never reads `game+0x580/0x588`. `bld+0x13E0` readers (rent_13e0.txt, 40 hits) are scoring/UI/slider code (FUN_140168730, FUN_14073E950 slider panel, FUN_140834DE0 daily update, FUN_1401F0BE0 config init) — none overlap the treasury-writer census.

**Evidence.** out_nv_dec1.txt:150-196 (`pfVar4 = *(bld+0x10e0); *pfVar4 = fVar1 - fVar10; **(bld+0x10f0) += fVar10; bld+0x112c += fVar10`).

**How this changes the review.** The "rent is a parallel economy" note stays a note; the engine's rent never touches the treasury.

## Q8 — Does FUN_14013FE70 handle residents itself?

**Answer.** No touch: it stops the collapse sound, sets the abandoned flag (`bld+0x1198=1`, `+0x11A0=0`), rebuilds debris fragment lists, destroys the node, and optionally calls FUN_1404C8FD0/FUN_1401A9D60/FUN_140161440. `bld+0xBC0` and `+0xBD8` are never read or written; no call to FUN_140835D70/FUN_140830820/FUN_140822FD0 — occupants are left dangling.

**Evidence.** out_nv_dec1.txt:6-101 (full decompile; the body contains no 0xbc0/0xbd8 references).

**How this changes the review.** The mod's pre-evacuation is necessary; keep it, but fix the per-person loop per Q1.

## Q9 — Who writes the treasury outside the shop tick?

**Answer.** Many. The displacement census (out_nv_d580.txt / out_nv_d588.txt, `MOVSD qword ptr` writes) finds ~28 distinct writer functions for `game+0x580` and ~29 for `game+0x588` (near-identical sets): FUN_140143710, FUN_140159070, FUN_140171E10, FUN_140173320, FUN_140174E40, the shop family FUN_140183580/183D30/184930/1856F0, FUN_1401B7D50, FUN_1401B9A40, FUN_1401D2DD0, FUN_1402BADE0, FUN_1402FB1D0, FUN_140337450, FUN_1403A8A70, FUN_1403DF9B0/E0730/E55D0, FUN_1404AFA90/B79C0/B9140, FUN_1405D6FC0, FUN_1406F8CC0, FUN_14080CC60, FUN_140831120/31700/32E90 (+ FUN_1404FCAF0 for +0x588 only). A second family accesses the treasury through the absolute globals `DAT_1409d5490/5498` (= `game+0x580/0x588`) and so does not appear in the displacement scan: FUN_1401AAD70 (vehicle purchase), FUN_14068B330 (trade crediter), FUN_140430FC0 (treasury reset), FUN_1404FCAF0 (v3_xrefs_treasury2.txt, asm_h2_wide.txt:1405171A4/1D5). Biggest known flows: the trade crediter FUN_14068B330 and the retail/service-fee family.

**Evidence.** out_nv_d580.txt (28 functions), out_nv_d588.txt (29), v3_xrefs_treasury2.txt (absolute-glob xrefs), scratch/asm_h2_wide.txt:1405171A4 (`MOVSD XMM2,[0x1409d5490]`).

**How this changes the review.** #12 stands and is if anything understated: NetTreasury (sum of observed wallet flows) diverges from the true delta because the engine moves money in dozens of unlabeled places.

## Q10 — Do tourists reach the checkout fulfillment gate?

**Answer.** Yes. Phase B iterates the customer vector `bld+0xBD8..0xBE0` (`0x14017247D`: `MOV RAX,[R14+0xbe0]` / `0x140172484`: `SUB RAX,[R14+0xbd8]`; `0x1401724E9`: `MOV RAX,[R14+0xbd8]`); the three pre-gate tests are resource match `0x1401725A9` (`CMP [RDX+RBX],RAX`), stock>0 `0x1401725BA` (`COMISS XMM8,XMM11`), and demand>rate `0x1401725D5` (`COMISS XMM0,XMM1`). The hook target `0x1401725DE` follows all three; the give is `0x14017262B` (shelf) / `0x14017263F` (demand); the first `+0x71C` test is `0x1401726CB` (`CMP dword ptr [RSI+0x71c],0x1`) — strictly after the goods are given. No branch before 0x1725DE filters on 0x71C/0x748, so a customer carrying `+0x71C!=0` (tourists occupy `+0xBD8`, and Q3 shows 71C persons sit in building person-vectors) reaches the gate.

**Evidence.** scratch/shop_checkout_gate.asm: 14017247D/484/4E9 (vector iter), 1401725A9/5BA/5D5 (gates), 1401725DE (hook), 14017262B/63F (give), 1401726CB (first 71C test).

**How this changes the review.** #24 confirmed: a tourist is dispensed goods before any currency check; ship the tourist gate at 0x1725DE.

## Q11 — Is Ctrl+F11 already bound?

**Answer.** Not in code. No `0x7A` (VK_F11) immediate appears in the polled input cluster (scratch/input_poll.c, input_update.c — greps for `0x7a`/`F11`/`7A` return nothing). The engine's hotkeys are data-driven: input is resolved through C3D_INPUT name strings (`"GAMEPAD_CONFIRM"`, `"GAMEPAD_ESC"` etc. at input_poll.c:50/60/82/94) and scancode tables, not hardcoded VK immediates. No native Ctrl+F11 chord was found; binding lives in data (sovetControls/btf tables), not code.

**How this changes the review.** #15's collision risk is low (likely-unbound); no change beyond noting bindings are data-driven.

## Q12 — Which of res+0x58/0x5C is RUB, and which of game+0x580/0x588 is USD?

**Answer.** `game+0x580` is USD and `game+0x588` is RUB (proven); the engine's pairs are kind-1 = {`res+0x64`, `workers+0x5C`, `game+0x588`(RUB)} and kind-2 = {`res+0x60`, `workers+0x58`, `game+0x580`(USD)} — the *inverse* of the mod's `res+0x58=RUB / +0x5C=USD` claim, so `getPrice` is likely inverted in every mode.

**Evidence.** Kind-1: `0x140172703` `MULSS XMM6,[RAX+0x64]`, `0x140172732` `MOVSS XMM2,[RCX+0x5c]`, `0x140172777/780` `ADDSD/MOVSD [R13+0x588]`. Kind-2: `0x1401727E7` `MULSS XMM8,[RAX+0x60]`, `0x140172817` `MOVSS XMM2,[RCX+0x58]`, `0x14017285F/868` `ADDSD/MOVSD [R13+0x580]`. Vehicle purchase FUN_1401AAD70 (d_h2_purchase.c:44-88): USD deducted from `DAT_1409d5490` (= game+0x580); RUB from `DAT_1409d5498` (= game+0x588). Cross-check: the 748 fee pairs `workers+0x5C→game+0x588`, `workers+0x58→game+0x580`.

**2×2 table.**

| 0x71C value | price offset | treasury offset | currency |
| --- | --- | --- | --- |
| 1 | res+0x64 retail, workers+0x5C | game+0x588 | RUB |
| 2 | res+0x60 retail, workers+0x58 | game+0x580 | USD |

**How this changes the review.** If the mod's pair is inverted, `resource_pricing::getPrice` is wrong in every mode — **open a new C-severity finding** and treat #5/#7/#22 as its symptoms, not independent bugs. A UI formatter of `DAT_1409D5490` labelling it RUB is the one artifact that would flip this verdict. Until then: do not close #5/#7/#22.

