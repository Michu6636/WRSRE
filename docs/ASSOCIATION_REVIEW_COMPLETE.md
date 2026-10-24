# Transport Association Investigation — Complete Review

**Target:** `SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`), image base `0x140000000`, ASLR ON.
**Mode:** read-only investigation. No hooks enabled, no memory written, no billing implemented, no money moved.
**Deliverable:** single self-contained file. Prior files (`report.md`, `association-adapter-contract.md`, `runtime-validation.md`) are left untouched and are superseded where they conflict with this document.

---

## 1. Provenance and limitations

### 1.1 Actual workspace, branch and HEAD

- **Workspace:** `C:\WRSRE\.kilo\worktrees\reverse-engineering-and-verified-transport-associa`
- **Branch:** `reverse-engineering-and-verified-transport-associa` (current, checked out)
- **HEAD commit:** `a692bb9` — "Storage building checkbox for DO detection"
- **Working tree:** clean except untracked `docs/FUEL PLAN/` (this investigation's output)
- No branch was created, switched, merged, or pushed; no production source modified.

### 1.2 Exact source files examined

| File | Role |
|---|---|
| `docs/engine/private-industry-re.md` | Primary consolidated offset/function findings (rounds Q1-Q9, R1-R2, H1-H16) |
| `docs/AGENT_REPORT_private_industry.md` | G1-G10 findings, transfer call-site census, offset table |
| `docs/engine/vehicle-fuel-system.md` | Cargo slot, burn/refuel via FUN_140420350, fuel globals |
| `docs/engine/logistics.md` | DO overview (superseded in part by ghidra-do-storage-findings.md) |
| `docs/ghidra-do-storage-findings.md` | DO TaskEntry/TaskCfg, save/load functions |
| `docs/engine/native-verification-results.md` | Treasury writers, checkout, rent |
| `src/game_world.h` | Offset definitions and safe-read helper conventions |
| `src/tesmio_api.h` | Plugin/loader contract, isolation rules |
| `src/currency_api.h` | Treasury/wallet service contract |
| `plugins/private_logistics/private_logistics.cpp` | Read-only scan only (no modification) |

### 1.3 Artifact availability: prose summaries only

No original Ghidra disassembly/decompilation artifacts (`.asm`, `.c` dumps from `DecompAt`/`DisasmMany`) are present in this workspace. The investigation is based exclusively on **prose summaries quoting those artifacts** (instruction addresses and line citations embedded in the markdown above). No new Ghidra run was or can be performed here.

### 1.4 Ghidra / binary / project availability

| Item | Status |
|---|---|
| JDK 21 | Present at `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot` (unused) |
| `scratch\ghidra_run.bat` | **Absent** at workspace root; `glob **/ghidra_run.bat` returned no files |
| `scratch/` directory | **Absent** |
| `TesmioLoader` Ghidra project | Not found at `C:\WRSRE\TesmioLoader`, `C:\Users\EricM\Downloads\WRSRE\TesmioLoader`, `C:\TesmioLoader`, or workspace root |
| `analyzeHeadless` | Cannot be invoked (no wrapper, no project, exclusive-lock constraints moot) |

### 1.5 Claimed plan

`docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` is **absent** — `Test-Path` returned `False`; no `docs/FUEL PLAN/` directory exists. Only `docs/implementation-plan-per-trip-fuel.md` and `docs/review-per-trip-fuel-plan.md` are present; neither is the claimed v0.18 path. The plan is not read and not referenced as if read.

### 1.6 What was not done

- No game execution, no runtime observation, no telemetry capture.
- No hooks enabled, no inline patching, no `call rel32` modification.
- No game memory read or written at runtime.
- No billing, settlement, or payer change.
- No claim of independent verification between two documents repeating the same original analysis.

---

## 2. Contradiction matrix

Each row: old claim; conflicting evidence; corrected conclusion; confidence; exact evidence location; remaining investigation.

| # | Old claim | Conflicting evidence | Corrected conclusion | Confidence | Evidence | Remaining investigation |
|---|---|---|---|---|---|---|
| M1 | `param_2` (RDX) is a `Vehicle*` at every transfer call site | Callee `FUN_140420350` dereferences `param_2+0x3B8`/`+0x3C0` (cargo vec) — vehicle fields — **inside** the callee. But refuel caller 0x16F831 passes `[RBX+0x4F8]` where `RBX` = output of `FUN_140427340` (route planner; reads `param_2` as a Building*: `+0x318` type desc, `+0x970` storage vec, `+0x604` float). H12 admits `[RBX+0x4F8]` is a building field coincidentally sharing vehicle's `+0x4F8` offset. None of the 16 transfer callers is among ~80 `FUN_1403FBDC0` call sites. | **Proven inside callee; caller-side object type unresolved at refuel site 0x16F831.** Do not universally call `param_2` a vehicle unless supported on that path. | Proven (callee) / unresolved (refuel caller) | `docs/engine/private-industry-re.md` H8, H12, Q2; `d_q4_line.c`; `d_h8_27340.c` | Trace the value passed as RDX at 0x16F831 back through `FUN_140427340` output to confirm whether it is a Vehicle* |
| M2 | Refuel at 0x16F831 passes vehicle+0x4F8 (home building) as param_2 | H12: RBX at refuel site is planner output, not a vehicle; `[RBX+0x4F8]` reads a building field; the transfer then treats it as param_2 and its own `param_2+0x3B8` dereference proves it is a Vehicle* at runtime — offset coincidence, not provenance. | **Unresolved.** The value in RDX at 0x16F831 is not demonstrably a vehicle object from caller-side evidence. | Unresolved | H8, H12 | Same as M1 |
| M3 | Mode 0 = load, mode 1 = unload; mode determines physical direction | H15 site table: mode 0 with +tons (0x68EECA, 0x68F792) AND −tons (0x6945B3, 0x694F40); mode 1 with −tons (0x68F7F9) AND +tons (0x6953C4). Mode and sign are independent. | **Contradicted.** Mode selects which side the transfer touches (mode 0 = order-source pool entry; mode 1 = vehicle cargo slot, per `d_q4_line.c:36-81/179-257`); tons sign gives direction. **Delivery classification is provisional:** site + tons > 0 is an observed heuristic for delivery-like legs, not established billing policy. Mode alone is never a delivery classifier. Complete side effects (caller-side complementary storage/cargo updates, pickup versus delivery, the physical movement each leg represents) remain to be reconciled before any billing rule is derived. | Proven | H15 census table; `d_q4_line.c:36-81,179-257` | None for polarity; H10 still needed for RSI param_9 at 0x694F40/0x6953C4 |
| M4 | `vehicle+0x4F0` = current building, readable at every transfer call site, pickup at unload | H13 correction: the three "proofs" (back-ref `+0xBB0`, parking slot `+0xB90`, fleet-vec removal `+0xC70`) are inside `FUN_1403FBDC0` (`d_q2_load_event.c` misattributed), **not** inside `FUN_140420350`. `d_q4_line.c` (transfer's own decompile, 262 lines) contains **no `+0x4F0` reference**. `+0x4F0` is NULL while in transit. Writer `FUN_1401C6DD0` is rail-DO-only; road DO handler reads only. | **Likely as field semantics; NOT proven readable by the transfer at call sites; NULL in transit; writer coverage incomplete for road vehicles.** Do not infer pickup from current location at unload; do not substitute home for unknown endpoint. | Likely (field) / unproven (call-site readability) | H11, H13 correction; `d_q4_line.c`; `FUN_1401C6DD0` at 0x1C6DD0 | Confirm whether `+0x4F0` is valid at each of the 16 sites; resolve road-vehicle writer coverage |
| M5 | `param_9` (stack+0x40, bld) = destination building at every site | H1: at road site 0x1B2898, `param_9 == NULL` (R15 sentinel); caller's frame carries no destination argument; destination resolved inside transfer, unavailable to caller. H10 deferred: `param_9` (RSI) at 0x694F40/0x6953C4 not traced to its load. | **Supported where non-NULL (unload sites); absent/unknown at bld==NULL sites (0x1B2898, 0x1CB7FF); RSI identity deferred at 0x694F40/0x6953C4.** Do not substitute home for an unknown physical endpoint. | Supported (non-NULL) / absent (NULL sites) / deferred (RSI) | H1 caller frame; H15 site table; H10 deferred | Disassemble the basic blocks and predecessor paths around **each** call (0x694F40 and 0x6953C4), extending back to the reaching definitions of RSI; a nearby instruction window is insufficient if RSI originates in another block (H10) |
| M6 | `order+0x00/+0x08` = vector of source `Building*` | G5/proven: count = (end−begin)/8 (8-byte ptr vector); element derefs to `Resource*` compared vs resource (`asm_r6_scan.txt:73`); tons at `element+8` (`asm_r6_scan.txt:94`, `d_q4_line.c:186-191`). | **Contradicted.** `order+0x00/+0x08` = `{Resource* +0x00, float +0x08}` records, not `Building*`. | Proven | `asm_r6_scan.txt:57-73,94`; `d_q4_line.c:186-191`; G5 | None |
| M7 | `order+0x30` = destination `Building*` | R6/proven: `order+0x30` = assigned `Vehicle*` (`1404206e2: CMP [RCX+RAX+0x30],RBX` vs param_2; `d_q4_line.c:171`). | **Contradicted as destination; proven as assigned Vehicle*.** | Proven | `asm_r6_scan`; `d_q4_line.c:171` | None |
| M8 | `order+0x50` = completion-record vector on the order | H5/H7 corrections: completion stamps (`done=1.0` at `+0x28+0x188`, resource at `+0x198`) live on the DO task record (stride 0x80) / separate completed-delivery log, NOT the order. | **Contradicted as order field.** | Proven | H5, H7; `d_q4_line.c:203-212` | Log-entry `Building*` offset (H7 deferred) |
| M9 | `order+0x10/+0x48/+0x28/+0x60/+0x68/+0x70` are order fields | H5: these are DO task record fields (stride 0x80), not order fields. | **Contradicted as order fields.** | Proven | H5 | None |
| M10 | Fields beyond a structure's stated stride are valid | Order stride 0x68; claims of `order+0x68/+0x70` (time pair) extend beyond/at stride boundary without separate evidence; DO task record stride 0x80 with fields beyond it unproven. | **Unproven for offsets at/beyond stated stride without independent evidence.** Treat stride as the hard boundary unless a field is separately evidenced. | Unproven | private-industry-re table vs `d_q4_line.c` scan bounds | Confirm each field against its parent structure's stride |
| M11 | AL==1 = success; param_6 outMoved always valid | H13: param_6 written only after mutation commits (`if (param_6) *param_6 = fVar22;`); `0.0` when nothing moved; param_6==NULL at sites like 0x1B2898 means caller doesn't request actual. AL==0 = early exit, nothing written. | **Supported with conditions:** AL==1 = mutation committed; outMoved valid only when AL==1 AND param_6 != NULL; at out=NULL sites caller accounts nominal tons. No dry-run mode. | Proven | H13; `d_q4_line.c:258-260`; H1 caller frame | None |
| M12 | 16-site census = 100% coverage guarantee | H14: 19 references = 16 direct `call rel32` + 3 DATA unwind metadata (`.pdata` RUNTIME_FUNCTION + `.rdata` UNWIND_INFO), no indirect calls seen. A census of references is not a proof of exhaustiveness across all code paths or future builds. | **Qualified:** 16 direct call sites enumerated; 3 DATA refs are metadata, not invocations; no indirect path observed in this binary. Not an absolute coverage guarantee. | Proven-count (this binary) | H14 census; raw PE byte dumps | Re-census on any new binary build |
| M13 | All transfer functions on game-tick thread ⇒ no concurrency | AGENT_REPORT Method: all examined functions sit on game-tick/sim path; no worker-thread evidence found. Absence of evidence is not evidence of absence. | **Supported: no worker-thread evidence in this binary.** Do not claim proven absence of concurrency; same-tick events are NOT proof of concurrency. | Likely-absence | AGENT_REPORT Method | Thread/interleaving analysis if worker threads are ever suspected |
| M14 | "No durable ID exists" | C8: order slots written/cleared by planner (vector-clear prologue, destructor loop `ADD RBX,0x68`); no order offset proven as durable ID. | **Corrected to: no durable ID established.** Absence of an established ID does not prove none exists anywhere in the binary. | Proven (not established) | R6 correction | Search for any stable operation identifier across the transfer/completion path |

---

## 3. Site-specific evidence table

Raw arguments are recorded separately from inferred roles. `param_2` is not universally called a vehicle. `param_9` semantics are per-site. Home is never substituted for an unknown physical endpoint.

**Evidence status:** All findings in this section are reported instruction-level evidence from committed investigation notes (prose summaries quoting `.asm`/`.c` dumps); they are **not independently revalidated against the binary in this task**, because Ghidra and the original dumps are unavailable (Section 1.4). Treat them as the best available record, not as a fresh confirmation.

| RVA | Caller function | param_1 (RCX) | param_2 (RDX) raw | param_2 object type | param_3 (R8) mode | param_4 (R9) resource | param_5 tons (sign) | param_6 out | param_9 (bld) raw | AL | Inferred role (not fact) |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 0x16F831 | vehicle upkeep `FUN_14016E3B0` | game | `[RBX+0x4F8]` | **unresolved** (M2) | 0 | `*(game+0xC330)` fuel | − | NULL | NULL | — | Internal refuel draw from home (G2) |
| 0x1B2898 | `FUN_1401B1290` road delivery | game (R14) | veh (RDI) | vehicle | 1 | res (R12) | + | NULL | NULL (R15) | — | Road delivery unload; destination internal, unavailable to caller (H1) |
| 0x1CB7FF | `FUN_1401CB2A0` loader | game (R15) | vehicle (R13) | vehicle | 0 | resource from cargo | + (slot cargo) | `&local` RSP+0x108 | NULL | — | Per-cargo-slot unload servicing; loop (H9/H13) |
| 0x68EECA | `FUN_14068B330` sea/rail | — | — | — | 0 | — | + | R12 (live) | `veh+0x4F8` | TEST AL | Sea/rail leg A (H15) |
| 0x68F792 | `FUN_14068B330` sea/rail | — | — | — | 0 | — | + (MINSS-bounded) | NULL | `veh+0x4F8` | TEST AL | Sea/rail leg B (H15) |
| 0x68F7F9 | `FUN_14068B330` sea/rail | — | — | — | 1 | — | − | `&local` | `veh+0x4F8` | TEST AL | Dock return / transship credit (H15) |
| 0x6945B3 | `FUN_14068B330` sea/rail | — | — | — | 0 | — | − | `[bld+0x368]` live | `veh+0x4F8` | TEST AL | Dock return leg (H15) |
| 0x694F40 | `FUN_14068B330` sea/rail | — | — | — | 0 | — | − | `&local` | RSI | TEST AL | Dock return leg; **RSI param_9 unresolved (H10)** |
| 0x6953C4 | `FUN_14068B330` sea/rail | — | — | — | 1 (R15D=1 at 0x6953A6) | — | + (= cap − stored) | RSI (dead on mode 1) | RSI | — | Sea/rail delivery; **RSI param_9 unresolved (H10)** |
| 0x141F33, 0x1B484B, 0x1B5EC0, 0x1B653B, 0x1B6833, 0x1B75ED, 0x1B7981 | freight AI cluster | — | — | — | 0 / reg | — | varies | varies | varies | — | Road/freight load sites (R2) |

**Rules applied in this table:**
- `param_2` object type recorded per row, not assumed.
- `param_9` NULL at 0x1B2898 and 0x1CB7FF means destination not passed; not "home".
- Home (`veh+0x4F8`) appears as `param_9` at sea/rail sites but is never substituted for a pickup endpoint at unload.
- Negative-tons legs (0x68F7F9, 0x6945B3, 0x694F40) are not deliveries under the site+sign rule.

---

## 4. Proposed observation contract

Each log record captures one transfer call. Raw values and interpretations are separated; no unverified delivery classifier filters the log.

### 4.1 Record schema

| Field | Kind | Source | Confidence | Notes |
|---|---|---|---|---|
| `call_id` | observer | process-wide atomic call counter | observer | Globally unique per observed call; no collision across sites |
| `thread_id` | observer | OS thread ID at call | observer | Captured, not assumed single-threaded |
| `nesting_depth` | observer | re-entrancy guard depth (increment on entry, decrement on exit) | observer | 0 unless nested calls detected |
| `tick` | observer | game clock read-only | observer | Timestamp |
| `call_site_rva` | raw | transfer call-site address | proven | One of the 16 sites |
| `param_1` | raw pointer | RCX = game | proven | |
| `param_2` | raw pointer | RDX | proven inside callee | Interpreted object type per M1/M2 (site-dependent) |
| `param_2_interpreted_as` | interpreted | observer label | unresolved at 0x16F831 | "vehicle" / "unresolved" |
| `param_3_mode` | raw u32 | R8 | proven | Not a load/unload classifier (M3) |
| `param_4` | raw pointer | R9 = Resource* | proven | |
| `param_5_tons_nominal` | raw float | stack+0x20 | proven | Sign = direction (M3) |
| `param_6_out` | raw pointer | stack+0x28 | proven | NULL = caller doesn't request actual |
| `out_moved` | actual quantity | *param_6 after call | proven only if AL==1 && param_6!=NULL | 0.0 when nothing moved |
| `al_success` | raw u8 | AL return | proven | 1 = mutation committed; 0 = early exit |
| `param_9_bld` | raw pointer | stack+0x40 | supported where non-NULL | NULL at 0x1B2898/0x1CB7FF; RSI unresolved at 0x694F40/0x6953C4 |
| `param_9_interpreted_as` | interpreted | observer label | site-dependent | "destination" / "home" / "unresolved" / NULL |
| `veh_plus_0x4F0` | raw pointer | `*(veh+0x4F0)` | likely (M4) | Current building; NULL in transit; not read by transfer |
| `veh_plus_0x4F8` | raw pointer | `*(veh+0x4F8)` | proven | Home building |
| `is_refuel` | interpreted | site==0x16F831 && mode==0 && tons<0 && resource==fuel | proven (G2) | Not a freight delivery |
| `is_delivery` | interpreted | site-specific AND tons>0 (M3) | derived | Never set on mode alone or tons<0 |
| `pickup_endpoint` | interpreted | `*(veh+0x4F0)` when valid | likely (M4 caveat) | Where observable only |
| `delivery_endpoint` | interpreted | param_9 when non-NULL | supported where non-NULL | Unavailable at bld==NULL sites |
| `captured_pre_call` | boolean | observer | observer | Whether args sampled before the call |
| `captured_post_call` | boolean | observer | observer | Whether out/AL sampled after return |

### 4.2 What is intentionally NOT in the record

- `operation_id`: **absent by design.** No durable operation ID established (M10/M14). Session-local plugin identities may safely group events for diagnostics only, tagged as heuristic, never as verified operation identity.
- Inferred "pickup" derived from "current location at unload": prohibited (M4).
- Home substituted for unknown physical endpoint: prohibited.
- Commercial eligibility, enterprise ownership, freight payer/payee: separate concerns, not in this contract.

### 4.3 Capture requirements

- Capture **unsuccessful** (AL==0), **negative** (tons<0), **zero** (out_moved==0.0), and **positive** calls; do not filter using an unverified delivery classifier.
- **Stack capture location:** sample raw arguments at the `call` instruction **before** the return address is pushed (i.e., at the call site, where stack offsets are caller-relative). Stack offsets at callee entry differ because the return address has been pushed. Sample AL and `*param_6` at return (callee exit).
- **Pointer readability:** validate `param_6` (out pointer) is readable before dereferencing. "Non-null and AL==1" establishes semantic eligibility, not memory readability; use `ReadablePtr(param_6, sizeof(float))` before reading `out_moved`.
- Entry/exit correlation by `call_id` (process-wide atomic), not by `call_seq` + `session_id` alone (which can collide across sites).

---

## 5. Feasible runtime-validation method

### 5.1 Concrete mechanism (not activated here)

The feasible mechanism is **non-billing instrumentation at the 16 known transfer call sites**, implemented as either:

- **Debugger tracepoints** (gameplay-nonmutating, but code bytes may be modified by the debugger): break on each call-site RVA, read registers/stack arguments and return value, log, continue. No gameplay data, arguments, return values, or payment behaviour are modified.
- **Authorized read-only hooking** (only if separately authorized): patch the `call rel32` at each site to a logging trampoline that reads args and return, then jumps to the original target. No gameplay state mutation; no money movement.

**Instrumentation requirement:** No modification of gameplay data, arguments, return values, or payment behaviour. Any instrumentation or code patching requires explicit authorization. If literal no-code-write observation is required, investigate suitable hardware-breakpoint or tracing facilities and their limitations.

This document **names** the mechanism but does **not** activate it. Activation requires separate authorization outside this task.

### 5.2 Why this is not self-contradictory

The prior documents restricted the *current task* to read-only investigation; this document names a concrete, authorized-in-principle mechanism for future validation without activating it now. Logging is feasible **once instrumentation is authorized**; it is not feasible as a pure static-analysis exercise because per-site argument values (e.g. RSI at 0x694F40/0x6953C4, `param_2` provenance at 0x16F831) cannot be observed without execution.

### 5.3 Same-tick events

Same-tick events for the same vehicle are **not proof of concurrency**. They may reflect serial processing within one tick. Concurrency requires worker-thread evidence (M13), which is absent but not disproven.

### 5.4 Entry/exit correlation

- Entry: capture `param_1`..`param_9`, `tick`, `thread_id`, `call_id`.
- Exit: capture `AL`, `*param_6` (if non-NULL and readable), `tick`.
- Correlate entry and exit by `call_id` (process-wide atomic). A call that exits with AL==0 has no trustworthy `out_moved`.

---

## 6. Association and identity limits

Four distinct concepts must not be conflated:

| Concept | Status | Basis |
|---|---|---|
| **Per-call identity** | Established | `call_id` = process-wide atomic call counter per observed call (contract §4); globally unique, no collision across sites |
| **Physical movement identity** | Not established | One logical shipment can span multiple transfer calls (loader loop 0x1CB7FF; paired sea/rail legs 0x68F792/0x68F7F9); partial amounts and both signs possible (M11, report §9) |
| **Pickup-to-delivery operation identity** | Not established | No durable operation ID found (M14); order slots reused by planner; no field proven to link a pickup call to its matching delivery call |
| **Commercial eligibility & enterprise ownership** | Separate concern | Goods-sale eligibility = storage direction byte `+0x84`/`+0x85`; building owner field not found (report Q6, likely-absence); freight payer/payee = treasury (current), unchanged by this investigation |

**"No durable ID established" ≠ "no durable ID exists."** The binary may contain a stable operation identifier that this investigation has not located. A durable engine ID is **not necessarily required**: what is required is a verified association and lifecycle rule. If investigation establishes an unambiguous pickup event, vehicle-specific active operation state, and a corresponding completion boundary, the plugin may assign a session-local operation ID safely. The ID need not survive world loads if all associated state is invalidated correctly. Do not block a sound session-local solution merely because the game lacks a persistent identifier.

**Session-local plugin identities** may safely be used for diagnostic grouping within one observation session: an observer may assign a provisional `candidate_operation_id` to events sharing vehicle, resource, and overlapping ticks, but must tag it `heuristic`, must not use it for billing or settlement, and must invalidate it when the operation's completion boundary or save/load occurs.

---

## 7. Targeted next investigation

Prioritized, bounded Ghidra/debugger questions. Each specifies evidence required and acceptance criterion. Uncertainties preserved where tools/artifacts are unavailable (Ghidra absent in this environment).

| Priority | Question | Evidence required | Acceptance criterion |
|---|---|---|---|
| 1 | Caller-side provenance of `param_2` at refuel site 0x16F831 (M1/M2) | Trace RDX value at 0x16F831 back through `FUN_140427340` output; confirm whether it is a Vehicle* | Decompiled/disassembled trace showing RDX origin; either "vehicle object confirmed" or "unresolved, building-field coincidence" |
| 2 | `param_9` (RSI) identity at 0x694F40 and 0x6953C4 (M5/H10) | Disassemble the basic blocks and predecessor paths around **each** call (0x694F40 and 0x6953C4), extending back to the reaching definitions of RSI; a nearby instruction window is insufficient if RSI originates in another block | RSI traced to a Building* load or explicitly "unresolved" |
| 3 | Completed-delivery log entry `Building*` offset (H7/M8) | DisasmMany `0x1404209A0:0x1404209E0` (transfer log-entry write) | Building* offset inside log entry confirmed or explicitly "unconfirmed" |
| 4 | `vehicle+0x4F0` validity at each of the 16 call sites (M4) | Per-site check whether `+0x4F0` is non-NULL and valid at call time | Table of site → `+0x4F0` valid / NULL / unknown |
| 5 | Order-structure field audit against stride 0x68 (M10) | Re-derive every claimed `order+0xNN` field; discard any at/beyond stride without separate evidence | Clean field table with stride boundary enforced |
| 6 | Re-census of transfer references on current binary (M12) | Xrefs on 0x420350; separate `call rel32` from DATA metadata | 16 direct calls + 3 DATA refs confirmed, or updated count |
| 7 | Thread/interleaving check (M13) | Scan for worker threads touching transfer or order vector | Worker-thread evidence found, or explicitly "none found, not disproven" |
| 8 | Establish a defensible operation identity/lifecycle mechanism (M14) | Scan order/completion/log records for a candidate engine identifier stable across pickup and delivery; alternatively verify execution-context boundaries (unambiguous pickup event, vehicle-specific active operation state, completion boundary) that allow a session-local plugin ID with correct invalidation on save/load | Candidate ID field found, or verified lifecycle boundaries established, or "none found" explicitly stated |

All items 1-8 require Ghidra or a debugger, both unavailable in this environment (Section 1.4). They are listed as future work, not as completed findings.

---

## 8. Final recommendation

**Additional targeted investigation required.**

### Evidence justifying the recommendation

**Supported (reported instruction-level evidence; not independently revalidated in this task):**
- `FUN_140420350` (RVA 0x420350) is the known transfer function used by the examined transport paths; 16 direct `call rel32` sites enumerated; 3 DATA refs are unwind metadata, not invocations (H14).
- Callee dereferences `param_2+0x3B8/+0x3C0` (vehicle cargo vec), proving `Vehicle*` **inside** the callee (Q2/d_q4_line.c).
- `veh+0x4F8` = home building, proven by three independent readers and a writer (Q1).
- Mode selects side touched; tons sign gives direction; they are independent (H15). Delivery-like classification (site + tons > 0) is provisional and not yet established as billing policy (M3).
- AL==1 = mutation committed; `out_moved` via param_6 valid only when AL==1 && param_6 != NULL; no dry-run mode (H13).
- `order+0x00/+0x08` = `{Resource*, tons}` records, not buildings; `order+0x30` = assigned `Vehicle*` (R6/G5).
- Internal refuelling (0x16F831, mode 0, negative tons, from home) distinguishable from fuel commodity delivery (G2).

**Not established (blocking verified association):**
- A durable operation ID linking a pickup call to its matching delivery call (M14; order slots reused).
- `param_2` caller provenance at refuel site 0x16F831 (M1/M2; offset coincidence, unresolved).
- `param_9`/RSI at 0x694F40/0x6953C4 (H10 deferred; Ghidra unavailable).
- Completed-delivery log entry `Building*` offset (H7 deferred; Ghidra unavailable).
- `vehicle+0x4F0` readability by the transfer at call sites (M4; transfer decompile has no `+0x4F0` reference; NULL in transit).
- Destination at `bld==NULL` load sites (0x1B2898; unavailable to caller, H1).
- Duplicate-event identification, operation interleaving, threads (unknown).

### What this means

Per-call observation of vehicle, resource, tons, sign, site, home building, and (where valid) current/explicit-destination building is supported. **Verified pickup-to-delivery association is not supported** until items 1-4 (and ideally 5-8) of Section 7 are resolved. Until then, the observation contract (§4) is the safe, read-only scope; session-local heuristic grouping is permitted only if explicitly tagged as non-verified.

No hooks are enabled, no memory is written, and no money is moved by this investigation or its deliverables.

---

## Final consistency audit

Superseded assertions searched and removed or qualified:

- "100% of invocations" → qualified to "16 direct call sites enumerated in this binary; census is not an absolute coverage guarantee" (M12).
- "vehicle at every site" → removed; `param_2` object type recorded per site, unresolved at 0x16F831 (M1/M2, site table).
- "current building is pickup at unload" → removed; prohibited in §3 and §4.1.
- "order resource records are buildings" → removed; contradicted as `{Resource*, tons}` (M6).
- "no destination field exists" → qualified to "no destination field found on the order; may exist elsewhere/unresolved" (M5/M7/M8).
- "no durable ID exists" → corrected to "no durable ID established" (M14, §6).
- "same tick means concurrency" → removed; explicitly prohibited (§5.3).

Two documents repeating the same original analysis are treated as one evidence base, not independent proof. Observed instructions are separated from interpretation throughout. No fabricated execution or runtime validation is claimed.
