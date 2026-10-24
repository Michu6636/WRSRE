# Transport Association Investigation — report.md

**Target:** `SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`), image base `0x140000000`, ASLR ON.
All addresses are RVAs; file-based dumps use `0x140` + RVA.
**Mode:** read-only investigation. No hooks enabled, no memory written, no billing implemented.
No game source or production source modified.

## 0. Prerequisites not met (recorded, not worked around)

| Prerequisite | Status | Consequence |
|---|---|---|
| Claimed plan `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` | **ABSENT** — `Test-Path` returned `False`. No `docs/FUEL PLAN/` directory exists. | Not read; not referenced as if read. Only `docs/implementation-plan-per-trip-fuel.md` and `docs/review-per-trip-fuel-plan.md` are present (neither is the claimed v0.18 path). |
| Branch `arena/83c93542-wrsre` | **DOES NOT EXIST** locally or on `origin`. Current branch is `reverse-engineering-and-verified-transport-associa`. | Work stays on current branch; no branch created/switched per constraints. |
| Ghidra wrapper `scratch\ghidra_run.bat` | **ABSENT** at workspace root and anywhere under the workspace (`glob **/ghidra_run.bat` = no files; `glob **/scratch/**` = no files). | No new Ghidra runs possible. All findings drawn from existing committed decompiled artifacts. |
| TesmioLoader Ghidra project | Not found at `C:\WRSRE\TesmioLoader`, `C:\Users\EricM\Downloads\WRSRE\TesmioLoader`, `C:\TesmioLoader`, or workspace root. | Headless Ghidra cannot be invoked. |

JDK 21 is present but unused without the wrapper/Ghidra project.

## 1. Evidence base

Existing artifacts only:
- `docs/engine/private-industry-re.md` (816 lines, rounds Q1–Q9, R1–R2, H1–H16)
- `docs/AGENT_REPORT_private_industry.md` (276 lines, G1–G10)
- `docs/engine/vehicle-fuel-system.md` (461 lines)
- `docs/engine/logistics.md` (superseded in part by `ghidra-do-storage-findings.md`)
- `docs/ghidra-do-storage-findings.md` (DO TaskEntry/TaskCfg, save/load)
- `docs/engine/native-verification-results.md`
- `src/game_world.h`, `src/tesmio_api.h`, `src/currency_api.h`
- `plugins/private_logistics/private_logistics.cpp` (read-only scan)

Confidence labels per `30_VERIFICATION/AGENT_FINDINGS_STANDARD.md`: **proven** = two independent sources / instruction-level; **likely** = one source; **contradicted** = earlier claim overturned by instruction-level evidence; **unknown** = not established.

## 2. Contradiction matrix

Claims resolved against instruction-level evidence where available; otherwise left unknown.

| # | Claim | Prior source | Instruction-level evidence | Resolution |
|---|---|---|---|---|
| C1 | `param_2` is a `Vehicle*` at every transfer call site | H8/Q2 — callee dereferences `+0x3B8`/`+0x3C0` cargo vec | `d_q4_line.c` (transfer decompile, RVA 0x420350, 262 lines) reads `param_2+0x3B8`/`+0x3C0` — vehicle fields — proving `Vehicle*` **inside the callee**. BUT refuel caller 0x16F831 passes `[RBX+0x4F8]` where `RBX` = output of `FUN_140427340` (route planner, processes `param_2` as a Building*: reads `+0x318` type desc, `+0x970` storage vec, `+0x604` float), and H12 admits `[RBX+0x4F8]` is a building field coincidentally sharing vehicle's `+0x4F8` offset. None of the 16 transfer callers is among the ~80 `FUN_1403FBDC0` call sites. | **Proven inside callee; caller-side provenance uncertain at refuel site 0x16F831.** The value passed as `param_2` at refuel is not demonstrably a vehicle object. |
| C2 | mode 0 = load, mode 1 = unload | Q2 shorthand; H15 initial census | H15 site table: mode 0 with + tons (0x68EECA, 0x68F792) AND − tons (0x6945B3, 0x694F40); mode 1 with − tons (0x68F7F9) AND + tons (0x6953C4). Mode and sign are independent. | **CONTRADICTED.** Mode does not determine load/unload or direction. Gate on site + tons sign, not mode. |
| C3 | `*(veh+0x4F0)` = current building readable at any transfer call site | H11 — three proofs (back-ref `+0xBB0`, parking slot `+0xB90`, fleet-vec removal `+0xC70`) | H13 correction: those proofs are inside `FUN_1403FBDC0` (`d_q2_load_event.c` misattributed), NOT inside `FUN_140420350`. `d_q4_line.c` (the transfer's own decompile) contains **no `+0x4F0` reference**. `+0x4F0` is NULL while in transit. Writer `FUN_1401C6DD0` is rail-DO-only; road DO handler reads only. | **Likely as field semantics (current building when set); NOT proven readable by the transfer at call sites; NULL in transit; writer coverage incomplete for road vehicles.** |
| C4 | `order+0x00`/`+0x08` = source `Building*` vector | Round-1 guess | G5/proven: count = (end−begin)/8 (8-byte ptr vector); element derefs to `Resource*` compared vs resource (`asm_r6_scan.txt:73`); tons at `element+8` (`asm_r6_scan.txt:94`, `d_q4_line.c:186-191`). | **CONTRADICTED.** It is `{Resource* +0x00, float +0x08}` records, not `Building*`. |
| C5 | `order+0x30` = destination `Building*` | Round-1 guess | R6/proven: `order+0x30` = assigned `Vehicle*` (`1404206e2: CMP [RCX+RAX+0x30],RBX` vs param_2; `d_q4_line.c:171`). | **CONTRADICTED as destination; proven as assigned Vehicle*.** |
| C6 | `order+0x50` = completion-record vector on the order | R7 initial | H5/H7 corrections: completion stamps (`done=1.0` at `+0x28+0x188`, resource at `+0x198`) live on the DO task record (stride 0x80) / separate completed-delivery log, NOT the order. | **CONTRADICTED as order field.** |
| C7 | `order+0x10`/`+0x48`/`+0x28`/`+0x60`/`+0x68`/`+0x70` are order fields | private-industry-re table | H5: these are DO task record fields (stride 0x80), not order fields. | **CONTRADICTED as order fields.** |
| C8 | An order pointer = durable operation ID | Design assumption | R6 correction: order slots written/cleared by planner (vector-clear prologue, destructor loop `ADD RBX,0x68`). No order offset proven as durable ID. | **CONTRADICTED.** Not durable. |
| C9 | Transfer reads `+0x4F0` / resolves destination internally at `bld==NULL` | H1/H11 | `d_q4_line.c` (transfer decompile) contains **no `+0x4F0` reference**. At `bld==NULL` the transfer resolves source internally via order scan; the only `Building*` materialized is the **source** (`order+0x00[i]`); the destination is **not** available to the caller (H1: 0x1B2898 frame has no destination arg). | **Partially contradicted.** Transfer does not read `+0x4F0`. Destination unavailable to caller at `bld==NULL` sites (e.g. 0x1B2898). |

## 3. The transfer function — FUN_140420350 (RVA 0x420350)

Single chokepoint for every physical goods movement. All 16 invocations are direct `call rel32` (E8); 3 additional DATA references are x64 unwind/metadata (`.pdata` RUNTIME_FUNCTION + `.rdata` UNWIND_INFO), not invocations. No indirect calls, no tail jumps, no function-pointer-table entries. **[proven, H14 census]**

### 3.1 Signature / calling convention

| Param | Register/stack | Meaning | Evidence |
|---|---|---|---|
| param_1 | RCX | `game*` | d_q4_line.c prologue; RCX = game base used for order-vector reads |
| param_2 | RDX | `vehicle*` (inside callee) | First dereference is `param_2+0x3B8`/`+0x3C0` cargo vec — vehicle fields |
| param_3 | R8 | `mode: u32` | `XOR R8D,R8D` (=0), `MOV R8D,1`, or `MOV R8D,R15D` at call sites |
| param_4 | R9 | `resource*` | `MOV R9,[R15 + 0xc330]` at refuel; `MOV R9,[R9 + RCX*0x8]` at loader |
| param_5 | stack+0x20 | `tons: float` | sign = direction; magnitude = nominal amount |
| param_6 | stack+0x28 | `out: float*` (actual moved tons) | written only after mutation commits; NULL = don't report |
| param_7 | stack+0x30 | `cap: float` | internal load clamp uses free capacity |
| param_8 | stack+0x38 | `flag: u8` | skips `order+0x30 == veh` test when 0; does NOT gate billing |
| param_9 | stack+0x40 | `bld: building*` | destination when non-NULL; NULL = order-scan/line mode |

Convention: **RCX = game, RDX = vehicle, R8 = mode, R9 = resource.** [proven, H8/H14/H15]

### 3.2 Return value and outMoved

- **AL == 1** = a real tons mutation was committed. **[proven, H13]**
- **AL == 0** = early exit, nothing written (bad resource id, empty cargo, no matching order/stop). **[proven, H13]**
- **param_6 (outMoved)** = clamped actually-moved tons, written only after the mutation commits: `if (param_6) *param_6 = fVar22;`. `0.0` when nothing moved. **[proven, d_q4_line.c:258-260]**
- **param_6 == NULL** = caller does not request actual (e.g. 0x1B2898). **[proven, H1 caller frame]**
- **No dry-run mode.** Every AL==1 invocation mutates game state. **[proven, H13]**

### 3.3 Internal clamping

- Load caps at free capacity `fVar21 = slotCap × (1 − sum(cargo/cap))`. **[proven, d_q4_line.c:80]**
- Unload never drives a cargo slot below 0. **[proven, d_q4_line.c:240-248]**

## 4. The 16 transfer call sites (full census)

| RVA | Caller | Mode | tons sign | param_9 (bld) | out (param_6) | Role |
|---|---|---|---|---|---|---|
| 0x16F831 | vehicle upkeep | 0 | − (fuel draw) | NULL | NULL | internal refuel |
| 0x1B2898 | `FUN_1401B1290` road delivery | 1 | + | NULL | NULL | road delivery unload (destination internal) |
| 0x1CB7FF | `FUN_1401CB2A0` loader | 0 | + (slot cargo) | NULL | `&local` | per-cargo-slot unload (loop) |
| 0x68EECA | `FUN_14068B330` sea/rail | 0 | + | `veh+0x4F8` | R12 (live) | sea/rail leg A |
| 0x68F792 | `FUN_14068B330` sea/rail | 0 | + | `veh+0x4F8` | NULL | sea/rail leg B |
| 0x68F7F9 | `FUN_14068B330` sea/rail | 1 | − | `veh+0x4F8` | `&local` | dock return / transship credit |
| 0x6945B3 | `FUN_14068B330` sea/rail | 0 | − | `veh+0x4F8` | `[bld+0x368]` | dock return leg |
| 0x694F40 | `FUN_14068B330` sea/rail | 0 | − | RSI | `&local` | dock return leg (RSI deferred — H10) |
| 0x6953C4 | `FUN_14068B330` sea/rail | 1 (always) | + | RSI | RSI (dead on mode 1) | sea/rail delivery (RSI deferred — H10) |
| 0x141F33, 0x1B484B, 0x1B5EC0, 0x1B653B, 0x1B6833, 0x1B75ED, 0x1B7981 | freight AI | 0/reg | varies | varies | varies | road/freight load sites |

Known charter sites 0x68F792, 0x68F7F9, 0x694F40, 0x6953C4 — all confirmed present, all in `FUN_14068B330`. **[proven]**

## 5. Mode / sign semantics (resolved)

- **Mode selects which side the transfer touches:** mode 0 = order-source pool entry (`d_q4_line.c:179-257`); mode 1 = vehicle cargo slot (`d_q4_line.c:36-81/192`). **[proven, H15]**
- **Sign of tons gives direction:** positive = push into touched side; negative = pull out. **[proven, H15]**
- Mode and sign are independent: mode 0 carries both + and −; mode 1 carries both − (0x68F7F9) and + (0x6953C4). **[proven, H15 census]**
- **Billing rule:** gate on **specific site AND tons > 0**. A mode-only gate bills negative return legs as deliveries. **[proven, H15 design rule]**

## 6. Vehicle identity — +0x4F8 (home) and +0x4F0 (current)

| Field | Meaning | Writer | Reader | Status |
|---|---|---|---|---|
| `veh+0x4F8` | home `Building*` (depot/garage/DO) | `FUN_1401C6DD0` sets `+0x4F8 = param_2` (building) on assignment | `FUN_140420350` reads `+0x3B8/+0x3C0` off param_2; `FUN_14016C1C0` reads `+0x4F0` | **proven** (Q1: three readers; writer `MOV [RDI+0x4F8],R12`) |
| `veh+0x4F0` | current `Building*` (where vehicle stands) | `FUN_1401C6DD0` (arrival); `FUN_1403FBDC0` at 0x3FC2F4 resets `+0x4F0 = +0x4F8`; cleared to NULL at 0x1401B259A (departure) | `FUN_14016C1C0` reads `+0x4F0`; transfer (`d_q4_line.c`) has **no `+0x4F0` ref** | **Likely** as field semantics; **not proven readable by transfer at call sites**; NULL in transit; writer coverage incomplete for road vehicles |

Lifecycle of +0x4F0: set on arrival → NULL after processing → reset to home after transfer (via `FUN_1403FBDC0`, not the transfer). NULL while in transit. **[proven, H11 + H13 correction]**

## 7. How source and destination are resolved (resolved)

Resolution depends on call-site mode and whether `param_9` (bld) is NULL.

### 7.1 Unload (mode == 1, param_9 != NULL)
- **Destination = param_9** (direct `Building*`, e.g. 0x68F7F9 `veh+0x4F8`, 0x6953C4 RSI).
- **Source = `*(veh+0x4F0)`** — current building where the vehicle stands. Read directly; no order scan needed.
- Transfer validates `param_9` as a building (`param_9+0x318` type desc; type 0x0C/0x6A branch at `d_q4_line.c:87-89`). **[proven, R7]**

### 7.2 Load / bld == NULL (mode == 0, param_9 == NULL)
- **param_9 = NULL** at 0x1CB7FF and 0x1B2898; caller's frame carries no destination argument (`R15` NULL sentinel). **[proven, H1]**
- **Source resolved inside transfer** via order scan: walks `game+0x144A8..+0x144B0` (stride 0x68), testing `order+0x30 == veh` and `order+0x00[i] == resource`. Only `Building*` materialized is the **source** at `order+0x00[i]`; destination not dereferenced. **[proven, H1]**
- **Destination = `*(veh+0x4F0)`** (current building) at the call site — but `+0x4F0` is not read by the transfer itself and is NULL in transit. **[C3: likely field semantics, not proven at call sites]**

### 7.3 What is NOT a source/destination field
`order+0x00/+0x08`, `order+0x10`, `order+0x48`, `order+0x50`, `order+0x28`, `order+0x60`, `order+0x68/+0x70` — all contradicted as order fields (matrix C4–C7). No order destination field exists (R7). Order pointer not a durable operation ID (C8).

## 8. Order structure (game+0x144A8/+0x144B0, stride 0x68)

| Offset | Meaning | Status | Evidence |
|---|---|---|---|
| `+0x00`/`+0x08` | `{Resource* +0x00, float +0x08}` records (8-byte ptr → 16-byte record) | **proven** | `asm_r6_scan.txt:57-58,73,94`; `d_q4_line.c:186-191` |
| `+0x30` | assigned `Vehicle*` | **proven** | `1404206e2`; `d_q4_line.c:171` |
| `+0x38`/`+0x40` | parallel vec; element +0x80 done-flag (float 1.0) | **likely** | `asm_r6_scan.txt:99-103` |
| All other `+0xNN` | — | **contradicted as order fields** (DO task record) | H5/H7 |

## 9. Repeated / partial transfer calls

**Yes.** Evidence:
1. **Per-slot loader loop (0x1CB7FF):** `FUN_1401CB2A0` loops over `veh+0x3B8..+0x3C0`, calling transfer once per slot with `tons = slot cargo`, then storage-add path (`bld+0x970`, stride 0xE0). One logical servicing = multiple transfer calls. **[proven, H13 caller2 + H9]**
2. **Paired sea/rail dock legs:** 0x68F792 (mode 0, +) paired with 0x68F7F9 (mode 1, −). One dock op = two opposite-sign calls. **[proven, H15]**
3. **Internal clamp + outMoved:** oversized `tons` still moves at most capacity and reports true figure via param_6. **[proven, H13]**

**Implication:** one logical shipment can span multiple calls, partial amounts, and both signs. No one-call-one-shipment assumption.

## 10. Threads / interleaving / nesting

- All examined functions sit on the game-tick/sim path. **[proven, AGENT_REPORT Method]**
- No worker-thread evidence. **[proven]**
- Interleaving/nesting for the same vehicle: **unknown** — not established.

## 11. When is actual moved quantity valid?

- Only when **AL == 1**.
- Read from param_6 **after** the call returns.
- `outMoved == 0.0` with AL==1 = mutation committed, net zero (clamped).
- At out=NULL sites the caller accounts nominal tons; billing `|tons passed|` at AL==1 matches the game's ledger. **[proven, H13]**

## 12. Operation records — move, expire, reuse

- Order vector reused/cleared by planner (vector-clear prologue, destructor loop `ADD RBX,0x68`). **[proven, R6]**
- Completion stamps on DO task record / completed-delivery log, not the order. **[proven, H5/H7]**
- `FUN_1401DE2B0` (RVA 0x1DE2B0) completion check; `param_2` = DO task record (stride 0x80), fields `+0x10` source `Building*`, `+0x18` TaskEntry index, `+0x48/+0x50` numeric. **[proven, H5]**
- Save/load: `FUN_1401F0BE0` reconstructs DO task vector; `FUN_1406C5350`/`FUN_1406C6040` serialize/deserialize TaskCfg. **[proven, ghidra-do-storage-findings]**
- **No durable operation ID.** Order slots are reused. **[proven, C8]**

## 13. Duplicate-event identification

**Nothing durable proven.** Candidates and limitations:

| Candidate | Limitation |
|---|---|
| `vehicle + resource + tons + tick` | NOT proof of distinct operation (one op spans multiple calls; identical shipments repeat) |
| `site + sign + tons + endpoints` | supports call-site dedup only, not grouping into one op |
| `order pointer` | NOT durable (C8) |
| `veh+0x4F0` + `veh+0x4F8` | change as vehicle moves; NULL in transit |
| Completed-delivery log `+0x188` (done) / `+0x198` (resource) | Building* offset inside log entry **unconfirmed** (H7 deferred) |

**No duplicate-event identification mechanism is established.** **[unknown]**

## 14. Internal refuelling vs fuel commodity transport

**Yes, distinguishable.** Different site, mode, sign, supply source:

| | Internal refuelling | Fuel commodity delivery |
|---|---|---|
| Site | 0x16F831 | delivery unload sites (0x1B2898 road, 0x6953C4 sea/rail) |
| Mode | 0 | 1 |
| tons sign | − (draw from building) | + (deliver) |
| `res` | `*(game+0xC330)` = fuel | fuel resource |
| `veh`-arg (param_2) | vehicle being refuelled | vehicle carrying fuel |
| `param_9` (bld) | NULL | destination building |
| Supply building | home `veh+0x4F8` | source = `*(veh+0x4F0)` or order-resolved |
| Payer today | nobody (statistics only) | treasury |

**[proven, G2 + vehicle-fuel-system Q4 + H15]** Refuelling is **not** a freight delivery and must not be billed as one.

## 15. Supported vs unknown call paths

**Supported (proven):**
- All 16 call sites enumerated/classified (mode/sign/param_9/out).
- `veh+0x4F8` home, `veh+0x4F0` current (when set/non-NULL).
- Mode/sign polarity rule (site + tons > 0).
- AL==1 = real movement; param_6 = actual moved tons; no dry-run.
- Refuelling distinguished from fuel commodity transport.
- DO task records, completion stamps, save/load functions.
- No worker threads; all on game-tick/sim path.

**Still unknown / targeted investigation required:**
- `param_9` (RSI) at 0x694F40 / 0x6953C4 (H10 deferred — Ghidra unavailable).
- Completed-delivery log entry `Building*` offset (H7 deferred).
- Order destination field — not found; may not exist.
- Operation interleaving / nesting / threads.
- Durable operation ID / duplicate-event identification.
- Completion tonnage equality (Q7 / G8 — `FUN_1401DE2B0` not decompiled for tonnage).
- Partial-delivery leave-open behaviour.

## 16. Separation of concerns

| Concept | Status | Where it lives |
|---|---|---|
| Physical endpoints (pickup/delivery) | `*(veh+0x4F0)` at call site; `param_9` at unload | proven (unload) / likely-but-unproven-at-call-site (load current-bld) |
| Carrier/home | `veh+0x4F8` → building's owner | vehicle→home proven; building owner field **not found** (Q6 likely-absence) |
| Goods-sale eligibility | storage direction byte `+0x84`/`+0x85` | proven (Q8 stamper) |
| Freight-payment eligibility | mode + sign + site gate; treasury funds | supported per H15; payer unchanged |
| Current freight payer | treasury (game+0x580 USD / +0x588 RUB); no per-transaction money | proven |

**Do not silently change the payer.** Current freight funding comes from the treasury; this investigation establishes identification only.

## 17. Recommendation

**Additional targeted investigation required.**

A verified **pickup-to-delivery association** cannot be claimed. What is observable per transfer call is limited to: the vehicle pointer (param_2, proven inside callee but caller provenance uncertain at 0x16F831), the resource (param_4), the nominal and actual tons (param_5/param_6, valid when AL==1), the sign, the call site, the home building (`veh+0x4F8`), the current building (`*(veh+0x4F0)` — likely as field semantics but not proven readable by the transfer at call sites, NULL in transit), and the explicit destination (`param_9` — available only at unload sites where non-NULL, absent at road site 0x1B2898).

What is **not** established:
- A durable operation ID linking a pickup call to its matching delivery call (C8: order pointer not durable; no operation ID found).
- The destination at `bld==NULL` load sites (C9: not available to caller).
- `param_2` caller provenance at the refuel site (C1: offset coincidence, not a vehicle object).
- `*(veh+0x4F0)` as a reliable endpoint at every transfer call site (C3: transfer does not read it; NULL in transit; writer incomplete for road).
- Duplicate-event identification or operation chaining (unknown).

Until the operation link (pickup ↔ delivery) is established — which requires resolving H7 (log-entry `Building*` offset), H10 (`param_9`/RSI at 0x694F40/0x6953C4), confirming whether any order field or log record durably links the two legs, and verifying `param_2` provenance at all sites — the association is at best **heuristic diagnostics for individual transfer calls**, not a verified pickup-to-delivery path.

No hooks are enabled and no money is moved by this investigation.
