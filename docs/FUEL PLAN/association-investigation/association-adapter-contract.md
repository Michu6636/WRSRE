# Transport Association Adapter Contract

**Status:** proposed contract, read-only. No hooks enabled, no money moved, no game memory written.
**Source of truth:** `docs/FUEL PLAN/association-investigation/report.md` (evidence-backed findings, incl. contradiction matrix §2).
**Rule:** no guessed offsets. Every field listed is either proven in the report or explicitly marked `unverified` / `deferred` / `unknown`.
**Critical:** this contract does **not** establish a verified pickup-to-delivery association. A durable operation ID linking a pickup call to its matching delivery call has not been established (report §17). Events describe individual transfer calls only; chaining them into an operation is heuristic and unproven.

## 1. Purpose

Define the event schema and identity/lifetime rules for a read-only transport-association observer that can identify, for each individual transfer call:

- the specific live vehicle,
- its carrier/home building,
- the actual pickup building (where observable),
- the actual delivery building (where observable),
- the resource and actual transferred quantity,
- the transport call site.

It separates **physical endpoints**, **enterprise ownership**, **goods-sale eligibility**, and **freight-payment eligibility**. Current freight funding comes from the treasury; this contract does not change the payer.

## 2. What is and is not verified

| Item | Status | Basis |
|---|---|---|
| Per-call observation of vehicle, resource, tons, sign, site | supported | proven (report §2–§4) |
| Home building `veh+0x4F8` | supported | proven (report Q1) |
| Current building `*(veh+0x4F0)` as field semantics | likely | not proven readable by transfer at call sites; NULL in transit (report C3) |
| Explicit destination `param_9` at unload sites | supported where non-NULL | proven (report R7); absent at 0x1B2898 (bld=NULL) |
| `param_9`/RSI at 0x694F40 / 0x6953C4 | **deferred** | H10 not run (Ghidra unavailable) |
| Completed-delivery log entry `Building*` | **deferred** | H7 not run |
| Durable operation ID (pickup↔delivery link) | **not established** | report C8, §13, §17 |
| Duplicate-event identification | **not established** | report §13 |
| Operation interleaving / threads | **unknown** | report §10 |

## 3. Supported observation points (call sites)

The transfer function `FUN_140420350` (RVA 0x420350) is the single chokepoint. All 16 invocations are direct `call rel32`; patching call sites observes 100% of invocations. **[proven, report H14]**

Observation point = the transfer call site. At each site the observer reads:

| Read | Source | Proven? | Notes |
|---|---|---|---|
| vehicle pointer | transfer param_2 (RDX) | proven inside callee | callee dereferences `+0x3B8`/`+0x3C0` (cargo vec). Caller provenance uncertain at refuel 0x16F831 (report C1) |
| home building | `veh+0x4F8` | proven | carrier/home building |
| current building | `*(veh+0x4F0)` | likely (field semantics) | not proven readable by transfer at call sites; NULL in transit (report C3) |
| resource | transfer param_4 (R9) | proven | `Resource*` |
| nominal tons | transfer param_5 (stack+0x20) | proven | sign = direction |
| actual moved tons | transfer param_6 out (stack+0x28) | proven | valid only when AL==1 |
| return success | transfer AL | proven | AL==1 mutation committed; AL==0 early exit |
| explicit destination | transfer param_9 (stack+0x40, bld) | supported where non-NULL | NULL = order-scan/line mode; destination unavailable to caller at bld==NULL sites (report C9) |

Supported site set (proven, report §4):

| RVA | Mode | tons sign | param_9 (bld) | Role |
|---|---|---|---|---|
| 0x16F831 | 0 | − | NULL | internal refuel (fuel draw from home bld) |
| 0x1B2898 | 1 | + | NULL | road delivery unload (destination resolved internally — unavailable to caller) |
| 0x1CB7FF | 0 | + | NULL | loader per-cargo-slot unload (loop: multiple calls per servicing) |
| 0x68EECA | 0 | + | `veh+0x4F8` | sea/rail leg A |
| 0x68F792 | 0 | + | `veh+0x4F8` | sea/rail leg B |
| 0x68F7F9 | 1 | − | `veh+0x4F8` | dock return / transship credit (paired with 0x68F792) |
| 0x6945B3 | 0 | − | `veh+0x4F8` | dock return leg |
| 0x694F40 | 0 | − | RSI | dock return leg (param_9 deferred — H10) |
| 0x6953C4 | 1 | + | RSI | sea/rail delivery (param_9 deferred — H10) |
| 0x141F33, 0x1B484B, 0x1B5EC0, 0x1B653B, 0x1B6833, 0x1B75ED, 0x1B7981 | 0/reg | varies | varies | road/freight load sites |

## 4. Event schema

One event record per transfer call that passes the gate (site-specific AND tons > 0 for delivery identification). Flat, append-only log record; no nesting; no operation object allocated.

| Field | Type | Source | Populated when | Confidence |
|---|---|---|---|---|
| `event_id` | `uint64` | observer-assigned monotonic | always | observer |
| `tick` | `uint64` | game clock (read-only) | always | observer |
| `call_site_rva` | `uint32` | transfer call site | always | proven |
| `mode` | `uint32` | param_3 | always | proven |
| `tons_sign` | `int8` | sign of param_5 | always | proven |
| `tons_nominal` | `float` | param_5 | always | proven |
| `out_moved` | `float` | param_6 out, after call | when param_6 != NULL | proven |
| `al_success` | `uint8` | AL return | always | proven |
| `resource_id` | opaque handle | param_4 (Resource*) | always | proven |
| `vehicle_id` | opaque handle | param_2 (Vehicle*) | always | proven inside callee (C1 caller caveat) |
| `home_building` | opaque handle | `*(veh+0x4F8)` | always | proven |
| `current_building` | opaque handle | `*(veh+0x4F0)` | when non-NULL | likely (C3 caveat) |
| `explicit_destination` | opaque handle | param_9 (bld) | when non-NULL | supported where non-NULL |
| `is_refuel` | `uint8` | derived (§5.1) | always | proven |
| `is_delivery` | `uint8` | derived (§5.2) | when site+tons>0 | derived |
| `operation_id` | `uint64` | — | **never** | not established (report §17) |

`vehicle_id`, `home_building`, `current_building`, `explicit_destination` are **opaque handles** — copy the pointer value into the log record for correlation only; do not dereference or cache raw pointers across DLL boundaries or frames. **[plugin isolation rule, src/tesmio_api.h]**

**`operation_id` is intentionally absent.** No durable operation ID exists (report C8). Do not synthesize one from an order pointer, vehicle pointer, or any in-game pointer that can be reused or freed.

## 5. Resolution rules (per-call only)

### 5.1 Internal refuelling (distinguished from fuel commodity transport)

Condition: call site `0x16F831` AND mode == 0 AND tons < 0 AND resource == fuel (`*(game+0xC330)`).
- Supply building = home building (`veh+0x4F8`).
- `is_refuel = 1`, `is_delivery = 0`.
- Payer: none today (engine books statistics only; no treasury movement). **[proven, report G2]**

Internal refuelling is **not** a freight delivery and must not be billed as one. **[proven]**

### 5.2 Delivery / freight unload

Condition: **specific site AND tons > 0** (mode alone is insufficient — report C2).
- `is_delivery = 1` only when site + tons > 0.
- `resolved_delivery` = `param_9` when non-NULL (explicit destination). Unavailable at `bld==NULL` sites (e.g. 0x1B2898).
- `resolved_pickup` = `*(veh+0x4F0)` (current building) when non-NULL — but not proven readable by the transfer at call sites and NULL in transit (report C3).
- Negative-tons legs (0x68F7F9, 0x6945B3, 0x694F40) are **not** deliveries: `is_delivery = 0`. **[proven, report C2/H15]**

### 5.3 Load (mode == 0, tons > 0, not refuel)
- `resolved_pickup` = `*(veh+0x4F0)` (current building) when non-NULL (C3 caveat).
- `resolved_delivery` = unavailable from transfer arguments at load sites (param_9 == NULL; source resolved internally via order scan, destination = current building). Not observable at this call site without order-scan re-implementation.
- `is_refuel = 0`, `is_delivery = 0` (load-side observation only).

## 6. Identity / lifetime rules

### 6.1 No durable operation ID exists

An order pointer (`game+0x144A8` vector entry) is **not** a durable operation ID: order slots are written/cleared by the planner (vector-clear prologue, destructor loop `ADD RBX,0x68`). **[proven, report C8]**

**Consequence:** the observer must not synthesize a "transport operation ID" or link pickup and delivery calls into a verified operation. Any grouping of events into a candidate operation is heuristic and must be tagged as such (report §13).

### 6.2 Event identity

Each observed transfer call passing the gate receives an observer-assigned monotonic `event_id`. Events are appended only; never updated in place.

### 6.3 Lifetime

- Events written once at observation time, never mutated.
- Raw `Vehicle*`/`Building*` handles copied into the record at observation time, not dereferenced afterward.
- No cross-DLL pointer sharing; opaque handles only. **[plugin isolation rule]**
- No global state bleeding across DLL boundaries. **[plugin isolation rule]**

### 6.4 Save/load

DO task entries and TaskCfg are serialized/deserialized by `FUN_1406C5350`/`FUN_1406C6040` and reconstructed by `FUN_1401F0BE0` during load. **[proven, ghidra-do-storage-findings]** The observer's own event log is **not** persisted by the engine; save/load persistence of the observer log is a runtime-validation item, not a contract guarantee.

## 7. Supported paths vs unknown

| Item | Status |
|---|---|
| Observe vehicle + home + current building at transfer call sites | supported (current-bld: likely, C3 caveat) |
| Read explicit destination `param_9` at unload sites | supported where non-NULL |
| Distinguish internal refuel from fuel commodity transport | supported (proven) |
| Gate delivery on site AND tons > 0 | supported (proven, C2) |
| Read actual moved tons via param_6 (valid when AL==1) | supported (proven) |
| Resolve destination at road site 0x1B2898 (param_9 == NULL) | not supported from transfer args (proven limitation, C9) |
| Resolve `param_9` (RSI) at 0x694F40 / 0x6953C4 | deferred (H10) |
| Completed-delivery log entry `Building*` offset | deferred (H7) |
| Order destination field | not found / may not exist (contradicted, C5–C7) |
| Durable operation ID / duplicate-event identification | not supported (contradicted, C8; unknown, §13) |
| Operation interleaving / threads | unknown (§10) |

## 8. What the contract explicitly does NOT do

- Does not enable hooks or modify game memory.
- Does not move money or change the payer (treasury remains current freight funder).
- Does not infer goods-sale eligibility from transfer alone (storage direction byte `+0x84`/`+0x85`, separate concern).
- Does not infer enterprise ownership from vehicle alone (building owner field not found, report Q6).
- Does **not** synthesize a durable operation ID or verify pickup-to-delivery association.
- Does not bill on mode alone or on tons < 0 legs.
- Does not treat internal refuelling as freight delivery.

## 9. Open items requiring Ghidra (unavailable in this environment)

1. DisasmMany `0x140694F80:0x14069538E` to trace the last write to RSI before `0x6953C4` (H10).
2. DisasmMany `0x1404209A0:0x1404209E0` for completed-delivery log entry `Building*` offset (H7).
3. DecompAt `0x1401DE2B0` for completion tonnage equality (report Q7/G8).
4. Confirm caller-side provenance of `param_2` at refuel site 0x16F831 (report C1).
5. Confirm whether any order-field or log-record durably links a pickup call to its delivery call.

These cannot be executed here because `scratch\ghidra_run.bat`, the Ghidra project, and `TesmioLoader` are absent (report §0).
