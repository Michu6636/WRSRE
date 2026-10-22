# AGENT REPORT — private industry

Target: `SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`), base `0x140000000`, ASLR on. Date 2026-10-19.
Investigation only — no memory written, no binary patched, no plugin touched. RVAs in text; file-based `0x140+RVA` in dumps.
Confidence per `30_VERIFICATION/AGENT_FINDINGS_STANDARD.md`: **proven** = two independent sources; **likely** = one source; **speculative** = impression; **negative** = searched and not found.

## 0. Summary for the reader in a hurry

1. **A1 settles the factory gate.** `fabric_factory` declares `$STORAGE_EXPORT` (and a `$PRODUCTION fabric` line). The engine stamps `+0x85`, so the direction-byte gate **opens**. The procurement bug is a **snapshot/delta logging problem in the mod**, not an INI or flag problem. (A1)
2. **G1: the payer hook is ONE patched `call rel32`.** `0x1B2898`'s `R15` is a NULL sentinel, not a building — but the site is still a real third-party delivery: with `bld=NULL` the transfer resolves the destination itself. Ships/trains also deliver through this path (their return-to-depot unload is separate). (G1)
3. **G2: refuelling IS the ordinary transfer.** `0x16F831` calls `FUN_140420350` with `res = *(game+0xC330)` (fuel), `veh-arg = *(veh+0x4F8)` (home building), mode 0, negative tons. Private-fuel billing is a two-line change at that one site. (G2)
4. **G7: the `0x0C`/`0x6A` branch is a line-stop selector, not a money guard.** Construction/demolition deliveries still transfer (and bill) once a line-stop matches. Do **not** mirror it as "not a sale". (G7)
5. **G5/G6 are clean negatives that prune work.** `order+0x00/+0x08` is NOT source buildings (it is `{Resource*, tons}` records) — the zero-hook wholesale poll via order sources is dead. And `[RSP+0xD8]` **is** the flag argument, but `flag=0` does **not** mean "never bill" (0x1B2898 passes `flag=0`). (G5, G6)

**Still unknown after this round:** G3 (vehicle purchase — not done), G8 completion *tonnage* (partially done), G9 wear fields (not done), `RSI`'s exact identity at `0x6953C4` (likely a building).

## 1. Consolidated offset table

| object | offset | meaning | confidence | evidence |
|---|---|---|---|---|
| Vehicle | +0x4F8 | home Building* (depot/garage/DO) | proven (banked) | round 1 Q1 |
| Vehicle | +0x3B8/+0x3C0 | cargo vec stride 0x10 (+0x00 Res*, +0x08 tons) | proven (banked) | round 1 Q2 |
| Vehicle | +0x610/+0x618 | line stops stride 0x21B8 | likely (banked) | round 1 Q4 |
| DO bld | +0xC70/+0xC78 | vehicle vec stride 8 — **also read by construction offices and the upkeep fn** | proven | d_q1_vehicle_owner.c:48, Codebase.txt:28352, d_g2_upkeep.c:443 |
| Game | +0x144A8/+0x144B0 | order vec stride 0x68 | proven (banked) | round 1 Q3 |
| Order | +0x30 | assigned Vehicle* | proven (banked) | round 3 R6 |
| Order | +0x00/+0x08 | vec of 8-byte ptrs → **`{Resource* +0x00, float +0x08}` records**, NOT Building* | proven (2 src) | asm_r6_scan.txt:57-73, d_q4_line.c:186 |
| Order | +0x38 | parallel vec; element +0x80 = done-flag (float 1.0) | likely | asm_r6_scan.txt:99-103 |
| Order | +0x50 | completion records (elem +0x28 → +0x188/+0x198) | likely (banked) | round 3 R7 |
| Game | +0xC330 | fuel `Resource*` (transfer res arg) | likely | d_g2_upkeep.c:657, asm_g_sites.txt:100 |
| Game | +0xC3F0 | fuel `Resource*` (burn slot match) | likely | d_g2_upkeep.c:599 |
| Building | +0xE30, +0xE4C | upkeep divisor, fuel-accum float | likely | d_g2_upkeep.c:494,501 |
| Transfer param_9 (stack+0x40) | dest Building*, NULL = order-scan mode | likely (banked) | round 3 R7 |
| Transfer `[RSP+0xD8]` | **is the flag argument** (frame offset) | proven (2 src) | asm_r8_early.txt prologue + caller stores |
| Window | +0x248 | kind; 0 = building window | likely | Codebase.txt:11082 |

**Struck this round (was wrong):**
- ~~`order+0x00/+0x08` = vector of source `Building*`~~ — corrected in G5: it is `{Resource*, tons}` records.
- ~~`flag=0` sites transfer freight without consulting an order → never bill~~ — corrected in G6: `flag=0` only skips the `+0x30==veh` assignment test; the order/slot walk still runs, and 0x1B2898 (a delivery) passes `flag=0`.


## 2. Part A — file findings

### A1 — the `fabric_factory` storage block
**Answer:** `fabric_factory` declares `$STORAGE_IMPORT` (×2) and `$STORAGE_EXPORT` (×2), plus a `$PRODUCTION fabric` recipe — so the engine stamps `+0x85` on the output storage and the direction gate **opens**; the procurement block is a mod snapshot/delta problem (outcome #1 in the design table).

**Evidence (`media_soviet\buildings_types\fabric_factory.ini`):**
```
$TYPE_FACTORY
$PRODUCTION fabric 0.05
$CONSUMPTION plants 0.2
$CONSUMPTION chemicals 0.005
$CONSUMPTION water 0.11
$STORAGE_IMPORT RESOURCE_TRANSPORT_COVERED 40
$STORAGE_IMPORT RESOURCE_TRANSPORT_WATER 20
$STORAGE_EXPORT RESOURCE_TRANSPORT_COVERED 15
$STORAGE_EXPORT RESOURCE_TRANSPORT_SEWAGE 20
```
**Tokens in order:** IMPORT, IMPORT, EXPORT, EXPORT. The `$PRODUCTION fabric 0.05` line names the output resource directly — the mod can classify the output slot by economics (`fabric`) instead of "last storage".

**Consequence:** stop hunting the flag. The `+0x85` export byte is stamped; fix the mod's stock-delta logging. Confidence: proven (INI quoted).

### A2 — do factory types declare `$STORAGE_EXPORT*`?
**Answer:** Yes — **114 of 488** `buildings_types\*.ini` contain `$STORAGE_EXPORT`, and production buildings (type 6/7/8/9) are the norm, not a special case.

| file | class | storage tokens |
|---|---|---|
| fabric_factory | factory | IMPORT, IMPORT, EXPORT, EXPORT |
| clothing_factory / food_factory / chemical_plant | factory | …EXPORT (COVERED) |
| steel_mill / aluminium_plant / brick_factory | factory | EXPORT (OPEN) |
| cement_plant / concrete_plant | factory | EXPORT (CEMENT/CONCRETE) |
| coal_mine / bauxite_mine / uranium_mine / gravelmine | mine | EXPORT (GRAVEL) |
| farm / farm_small / farm_medium / farm_large / animal_farm | farm | EXPORT (COVERED/LIVESTOCK) |
| oil_rafinery | factory | IMPORT, EXPORT_SPECIAL (fuel), EXPORT_SPECIAL (bitumen) |

**Verdict:** the export flag is **common — the normal case** for production buildings. The mod's "last storage on a production building" fallback is therefore a *correct general rule*, not a patch for a rare case. Confidence: proven (file census).

### A3 — DO storages and fuel
**Answer:** Every distribution office declares its own **fuel** storage (`$STORAGE_FUEL RESOURCE_TRANSPORT_OIL`), so its trucks refuel from the company's own depot — fuel cost is naturally attributable to the DO owner.

**Evidence:**
```
distribution_office_small.ini:  $TYPE_DISTRIBUTION_OFFICE
                                $STORAGE_FUEL RESOURCE_TRANSPORT_OIL 20
                                $WORKING_VEHICLES_NEEDED 6
                                $VEHICLE_PARKING … (6 slots)
distribution_office_medium.ini: $STORAGE_FUEL RESOURCE_TRANSPORT_OIL 20
                                $WORKING_VEHICLES_NEEDED 12
distribution_office_rail.ini:   $TYPE_DISTRIBUTION_OFFICE_RAIL
                                $STORAGE RESOURCE_TRANSPORT_VEHICLES 150.00
                                $STORAGE_FUEL RESOURCE_TRANSPORT_OIL 50
```
(2) **Yes**, a fuel storage on all three DO variants. Vehicle slots: `$VEHICLE_PARKING` lines (6 small / 12 medium), `$WORKING_VEHICLES_NEEDED`. Confidence: proven (INI quoted).

### A4 — the fuel resource
**Answer:** The vehicle-fuel resource is **`fuel`**, produced by the oil refinery and stored via `$STORAGE_FUEL` / `$STORAGE_SPECIAL … fuel`; no INI declares a base price (it is priced like every other resource, live).

**Evidence:**
```
oil_rafinery.ini:        $PRODUCTION fuel 0.25
                         $PRODUCTION bitumen 0.15
                         $STORAGE_EXPORT_SPECIAL RESOURCE_TRANSPORT_OIL 340 fuel

## 3. Part B — Ghidra findings

### G1 — is `0x1B2898` a delivery to a third party?
**Answer:** Yes — it is the road-truck delivery unload; `R15` is a NULL sentinel meaning "no explicit building", and the transfer resolves the destination itself, so the payer hook is a single patched `call rel32`. Ships/trains deliver through the same transfer; the return-to-depot unload (`0x68F7F9`, `bld=*(veh+0x4F8)`) is a separate staging site.

**Offset(s) / call sites:**
| site | function | mode | `bld` (param_9) | flag |
|---|---|---|---|---|
| `0x1B2898` | `0x1B1290` | 1 | `R15`=NULL | `R15B`=0 |
| `0x1B484B` | `0x1B3CE0` | 1 (`LEA R8D,[RAX+1]`) | `RAX`=NULL | 0 |
| `0x68F7F9` | `0x68B330` | 1 | `*(veh+0x4F8)` (own depot) | 0 |
| `0x6953C4` | `0x68B330` | 1 (`R15D`=1) | `RSI` (register) | 0 |

**Function(s):** `FUN_1401B1290` (RVA `0x1B1290`), road-freight delivery AI, game-tick thread; caller `FUN_140189030` (`x_r8_callers.txt`, single UNCONDITIONAL_CALL + 2 DATA vtable refs).

**Evidence (literal instructions):**
- `R15` provenance: `1401b1318: XOR R15D,R15D` — the only R15 write in `0x1401B2000-0x1401B2900` (asm_r8_early.txt:30, asm_r8_r9gap.txt). It is never reloaded from an order, storage walk, or parameter.
- The site: `1401b2874: MOV qword ptr [RSP+0x40],R15` (bld=NULL), `1401b2879: MOV byte ptr [RSP+0x38],R15B` (flag=0), `1401b288c: MOV R8D,0x1` (mode 1), `1401b2892: MOV RDX,RDI` (veh), `1401b2895: MOV RCX,R14` (game), `1401b2898: CALL 0x140420350` (asm_r1_r2_sites.txt).
- Ships/trains: `0x6953C4` passes a register-loaded `bld=RSI` with `R15D=1` — a third-party delivery that does **not** route through `0x1B2898`'s function, but through the same transfer.

**Provenance sentence:** *R15 comes from `XOR R15D,R15D` at function entry — it is a NULL sentinel ("no explicit destination"), not any part of the vehicle.*

**Verdict on "does every third-party delivery pass through `0x1B2898`":** **No.** The generic mechanism is the transfer `0x420350` with `bld=NULL` (destination resolved from the order) or `bld` set explicitly (`0x6953C4`). `0x1B2898` is the road-delivery instance; ship/line deliveries have their own mode-1 site (`0x6953C4`). Both are among the already-mapped 16 — no hidden 17th entry point found.

**Confidence:** proven (NULL sentinel, two dumps) / likely (function role) / likely (`RSI` at 0x6953C4 is a building).

**Changes:** unblocks S2 — implement the payer hook as one patched `call rel32` at `0x1B2898` (and `0x6953C4` for water/line freight); both pass `mode=1`. For `bld=NULL` sites, read the destination from the resolved order (`+0x50` records), not from `param_9`.

### G2 — where does fuel burn, and who pays today?
**Answer:** Fuel burn is a per-tick decrement of a `{fuel, tons}` slot on the vehicle's order record; refuelling goes through the ordinary transfer `0x420350` (mode 0, negative tons) drawn from the vehicle's **home building** (`veh+0x4F8`); today **nobody is charged** — the engine only decrements the slot and books statistics.

```
fuel resource   = fuel  (game+0xC330 for transfer, game+0xC3F0 for burn-slot match; via veh+0x4F8 → home building fuel storage)
burn            = FUN_14016E3B0 (RVA 0x16E3B0), slot+0x08, per tick (rate-scaled: line-rate / bld+0xE30 * fVar27)
refuel path     = 0x420350 with res=fuel, mode 0, veh-arg = *(veh+0x4F8), tons NEGATIVE   [0x16F831]
today's payer   = nobody (storage slot decremented; FUN_1402fdb00 books stats to game+0x11408 + city ledger, no money)
price source    = resource record (live; save stats "fuel 118.997177 1.050000")
```

**Offset(s) / function(s):** `FUN_14016E3B0` (RVA `0x16E3B0`), vehicle upkeep, game-tick thread. Fuel globals `game+0xC330` (transfer res), `game+0xC3F0` (burn slot match). Building accumulators `+0xE30` (divisor), `+0xE4C` (float).

**Evidence (literal):**
- Burn: `d_g2_upkeep.c:599-605` `if ((*plVar15 == *(longlong *)(param_1 + 0xc3f0)) && (fVar28 = *(float *)(plVar15 + 1), 0.0 < fVar28)) { … *(float *)(plVar15 + 1) = fVar28 - fVar32; }` — decrements the `+0x08` tons of the slot whose resource is `*(game+0xC3F0)`.
- Rate: `d_g2_upkeep.c:493-501` `fVar27 = (slot_float / *(param_2 + 0xe30)) * fVar27; … *(float *)(param_2 + 0xe4c) = fVar27 + *(float *)(param_2 + 0xe4c);`
- Refuel: `d_g2_upkeep.c:656` `cVar11 = FUN_140420350(param_1, *(lVar13 + 0x4f8), 0, *(param_1 + 0xc330), -fVar28, local_148);` and asm `14016f824: XOR R8D,R8D` (mode 0), `14016f827: MOV RDX,qword ptr [RBX+0x4f8]` (veh-arg = home bld), `14016f81d: MOV R9,qword ptr [R15+0xc330]` (res=fuel), `14016f831: CALL 0x140420350` (asm_g_sites.txt:95-104).
- No payer: after the call, `d_g2_upkeep.c:659-681` books any shortfall via `FUN_1402fdb00` to `game+0x11408` and the city ledger `(*( *(param_2+0x108) + 0x1b8 )) + 0xe0` — statistics, no treasury/currency call.

**Two sources:** (1) decompiled burn-slot match + tons decrement (`d_g2_upkeep.c:599-605`); (2) the refuel call's literal register setup (`asm_g_sites.txt`) matching `res=fuel`, `veh-arg=home building`, negative tons.

**Refuel sub-answer (the valuable one):** refuelling **does** go through `0x420350`. **`mode == 0`** and the **supplying building is the `veh` argument** (`*(veh+0x4F8)`), *not* `param_9` (which is NULL). The private-fuel charge is therefore a **two-line change**: at the `0x16F831` site, read the home building from `veh+0x4F8` and bill its owner for `fuel`.

**Confidence:** likely (globals/offsets, single decompile each) / proven (refuel-via-0x420350, two sources).

**Changes:** unblocks the private-logistics cost model — fuel is billable at the existing transfer, sourced from the company's own DO fuel tank (A3). No new hook.

### G3 — where is a vehicle purchased, and what does it cost?
**Answer:** **Not done** — budget spent on G1/G2 per the stop rule.

**Changes:** state-vs-private purchase question remains open; design keeps the "engine does not know `[P]` tags" assumption (all purchases read as state).

### G4 — enumerate every vehicle a building owns
**Answer:** `bld+0xC70/+0xC78` (stride 8, `Vehicle*`) is the per-building vehicle vector, and it is **generic** — read by DOs, construction offices, and the upkeep function alike. No separate global vehicle vector found (negative).

**Offset(s):** `bld+0xC70` begin, `+0xC78` end, stride 8.

**Evidence (three readers):**
- DO upkeep: `d_q1_vehicle_owner.c:48` `if (*(longlong *)(param_2 + 0xc78) - *(longlong *)(param_2 + 0xc70) >> 3 != 0)`, `:51` `lVar15 = *(local_res18 + *(param_2 + 0xc70))` — reads a `Vehicle*` out of it.
- Construction toolbar (non-DO): `Codebase.txt:28352-28354` `if ((bVar31) && (7 < *(lVar23+0xc78) - *(lVar23+0xc70))) { FUN_14066bd60(**(lVar23 + 0xc70)); }`.
- Upkeep/fuel loop: `d_g2_upkeep.c:443` `lVar13 = *( *(param_2 + 0xc70) + lVar20 * 8 );`.

**Confidence:** proven (three independent readers, incl. one non-DO).

**Changes:** unblocks charging a fleet — walk `bld+0xC70`, filter by `veh+0x4F8 == bld`, once per payroll tick. No global-vector walk needed.

### G5 — is `order+0x00`/`+0x08` a vector of source buildings?
**Answer:** **No** (clean negative). `+0x00/+0x08` is a vector of 8-byte pointers to `{Resource* +0x00, float +0x08}` records — the same shape as the mod's cost/slot records, not `Building*`.

**Offset(s):** order `+0x00` begin, `+0x08` end; elements are `Resource*`+tons records (8-byte ptr → 16-byte record).

**Evidence (literal):**
- `asm_r6_scan.txt:57-58` `MOV RAX,qword ptr [R8 + RDX + 0x8]` / `SUB RAX,qword ptr [R8 + RDX]` / `SAR RAX,0x3` — count = (end−begin)/8, an 8-byte-pointer vector.
- `asm_r6_scan.txt:73` `CMP qword ptr [RAX],R12` — element derefs to a `Resource*` compared against `R12` (= `param_4`).
- `asm_r6_scan.txt:94` `ADDSS XMM1,dword ptr [R14]` where `R14 = element+8` — a float tons field.
- Decompiler match: `d_q4_line.c:186` `if (**(longlong **)(lVar3 + lVar1) == param_4)` (element → `Resource*`) and `:190-191` `pfVar19 = (float *)(… + 8)`.

**Confidence:** proven (disasm + decompiler, two sources).

**Changes:** forecloses the zero-hook wholesale poll via "order names its source building". The `+0x30==veh` scan + `veh+0x3B8` cargo watch remains the attribution path; the hook route is unaffected.

### G6 — what does the `[RSP+0xD8]` gate actually gate?
**Answer:** `[RSP+0xD8]` **is** the `flag` argument (param_8) after the frame — and `flag=0` only skips the `+0x30==veh` assignment test, not the order walk, so it does **not** mean "never bill".

**Offset(s):** callee frame `[RSP+0xD8]` ↔ caller `[rsp+0x38]` = flag.

**Evidence (frame math):** prologue is `PUSH RBX / PUSH RDI / PUSH R12 / SUB RSP,0x80` = `0x98` bytes of frame (asm_r8_early.txt:920-923). Caller `[rsp+0x38]` (where every site stores the flag byte: `MOV byte ptr [RSP+0x38],…` at 0x141F10, 0x16F809, 0x1B2879, 0x68EEAC, … — asm_r1_r2_sites.txt) maps to callee `[rsp+0x38+0x98+8] = [rsp+0xD8]`. Cross-check: cap (caller `[rsp+0x20]`) maps to callee `[rsp+0xC0]`, and the transfer reads `MOVSS XMM7,dword ptr [RSP+0xC0]` (asm_r8_early.txt:944) — the same +0xA0 shift. Consistent for both.

**Consequence correction (G6/G9 of round 4):** `flag=0` does **not** prune billing sites. The 0x1B2898 delivery unload passes `flag=0`; billing must key on `mode==1`, never on `flag`.

**Confidence:** proven (frame math + cross-check on cap slot).

**Changes:** removes a wrong pruning rule; keeps the `mode==1` payer rule intact.

### G7 — what does the `0x0C`/`0x6A` type branch do?
**Answer:** It is a **line-stop storage-selection rule** for construction/demolition — if the type is 0x0C/0x6A and a matching line-stop exists, the transfer uses the line-stop's building; it is not an early-out and not a money guard.

**Evidence (literal, asm_r8_early.txt:995-1018):**
```
1404204ab: MOV EAX,dword ptr [RCX + 0x360]   ; td+0x360 building type
1404204b1: CMP EAX,0xc                        ; construction
1404204b4: JZ 0x1404204bf
1404204b6: CMP EAX,0x6a                       ; demolition
1404204b9: JNZ 0x14042040a                    ; other types → direct path (0x14042040a)
1404204bf: CMP byte ptr [RBX + 0xea8],R14B    ; veh+0xEA8 gate
1404204c6: JZ 0x1404204d1
1404204c8: COMISS XMM7,XMM6
1404204cb: JBE 0x14042040a                    ; else fall to direct path
1404204d1: MOV RCX,qword ptr [RBX + 0x618]    ; line-stop vec end
1404204e2: SUB RCX,qword ptr [RBX + 0x610]    ; − begin
140420500: JZ 0x14042054b                     ; no stops → return 0
140420510: IMUL RAX,R9,0x21b8                 ; stride 0x21B8 walk
```
On match it walks `veh+0x610/+0x618` (stride 0x21B8) line stops; no stop → return 0. Other types take `0x14042040a` (direct path).

**Consequence:** the design's hypothesis "deliveries to construction/demolition are not sales" is **wrong** — the engine still transfers (and would bill) once a line-stop matches. Do not add that guard. Confidence: proven (disasm).

### G8 — completion tonnage
**Answer:** Partially — the completion path (`FUN_1401E5350` → `FUN_1401E5480`) works off **DO records** (`DO+0xD58/+0xD60`, stride 0x80) and the TaskCfg (`*( *(DO+0xD38) + 0x10)`), **not** the order struct (`game+0x144A8` never appears). Whether target-vs-actual tonnage is compared is unverified (needs `FUN_1401DE2B0`).

**Evidence:** `d_g8_complete2.c` (267 lines) — iterates `param_3+0xD58/+0xD60` stride 0x80 (`:41,52`), calls `FUN_1401DE2B0(entry, veh, DO)` (`:55`), success path reads TaskCfg via `**(param_3+0xD38)+0x10` (`:73`) and manipulates `veh+0x638..0x640` (0x198-stride records). `Select-String '144a8|144b0'` → **no matches** (order struct untouched).

**Confidence:** likely (path mapped) / not-done (tonnage comparison).

**Changes:** freight billed per ton delivered still needs `FUN_1401DE2B0`; a partial delivery's leave-open behaviour is unknown. Fallback (bill at unload `bld`, `mode==1`) stands.

### G9 — wear / maintenance fields
**Answer:** **Not done** — parked by design, no offset recorded this round.

### G10 — can we append to a distribution-office window?
**Answer:** The DO window is a **building window (kind 0)**; the DO panel builder is `FUN_140741170` (RVA `0x7411170`), dispatched from `FUN_1406E9B80` on `type==0x2B`. Rail DO (`0x34`) kind not separately verified. Storage-strip (`FUN_1406F8CC0`) usage not verified.

**Evidence:** `Codebase.txt:11082` `if (*(const int*)(w + 0x248) != 0) return false; // 0 == building window`; `Codebase.txt:40142` `*(undefined4 *)(puVar2 + 0x248) = 0;` (dispatcher sets kind 0); round 1 banked the `0x2B → 0x7411170` dispatch. DOs are buildings → kind 0.

**Confidence:** likely (kind 0 for 0x2B) / not-verified (0x34, storage strip).

**Changes:** future DO books panel can reuse the existing kind-0 append path the citizen window already uses.

## 4. Corrections

- **`order+0x00/+0x08` is not source `Building*`** — struck from the table; it is `{Resource*, tons}` records (G5).
- **`flag=0` does not mean "no order / never bill"** — struck from round-4 G6 reasoning; `flag=0` only bypasses the `+0x30==veh` test, and the main delivery site 0x1B2898 passes `flag=0` (G6).
- **Round 4's "0x1B2898 is not a third-party delivery"** — refined: it *is* the road-delivery unload; `bld=NULL` means the destination is resolved internally, not that it isn't a sale (G1).

## 5. Not done

- **G3** (vehicle purchase / price / `+0x4F8` timing) — budget exhausted after G1/G2 per the brief's stop rule.
- **G8** (completion *tonnage* comparison) — path mapped, `FUN_1401DE2B0` not decompiled.
- **G9** (wear fields) — parked by design.
- **G10** rail-DO (`0x34`) kind + storage-strip usage — not verified.
- `RSI` identity at `0x6953C4` — likely a building (register-loaded `bld`), not traced to its load.

## 6. Surprises

The fuel system turned out to be almost exactly what the design hoped: refuelling is not a bespoke mechanism but the *ordinary* freight transfer with a negative tonnage, sourced from the vehicle's own home building and priced through the normal live resource record. That collapses the private-logistics fuel cost from "new hook" to "read one more argument at a site we already patch". The second surprise was the frame arithmetic on `[RSP+0xD8]` — a gate that looked like an unexplained local is simply the flag argument once the `0x98` frame and the return-address slot are added, and it turned a plausible-sounding pruning rule ("flag=0 ⇒ don't bill") into a bug that would have deleted the single most important delivery site from the payer set. Finally, the type-descriptor recipe hunt (round-1 Q5) is moot for output classification: the INI publishes `$PRODUCTION <resource>` directly, so the mod can read the recipe from the building file rather than reverse it from storage order.

## 7. Method

Ghidra runs this round, **sequential** (one `analyzeHeadless` at a time, exclusive lock respected), `.java` only, JDK 21, single `-scriptPath` via `scratch\ghidra_run.bat`, file-based `0x140+RVA`:

1. `DisasmMany.java` → `scratch/asm_g_sites.txt` (ranges `0x1401B47D0:0x1401B4870`, `0x140695330:0x1406953E0`, `0x14016F7B0:0x14016F850`) — G1 register-mode sites + G2 refuel context.
2. `DecompAt.java` → `scratch/d_g2_upkeep.c` (`0x14016E3B0`) — G2 fuel burn + refuel.
3. `DecompAt.java` → `scratch/d_g8_complete2.c` (`0x1401E5480`) — G8 completion body.

Reused prior dumps: `asm_r6_scan.txt`, `asm_r8_early.txt`, `asm_r8_site.txt`, `asm_r8_r9gap.txt`, `asm_r1_r2_sites.txt`, `d_q4_line.c`, `d_q1_vehicle_owner.c`, `x_r8_callers.txt`. File investigation read only `media_soviet\buildings_types\*.ini`, `media_soviet\vehicles\*\script.ini`, and a save `stats.ini`. No `DecompAt` attempted on `FUN_14028C770`/`FUN_14030D1A0`. Threads: every function examined (`0x1B1290`, `0x16E3B0`, `0x1E5350/0x1E5480`, `0x420350`) sits on the game-tick/sim path; no worker-thread evidence.
