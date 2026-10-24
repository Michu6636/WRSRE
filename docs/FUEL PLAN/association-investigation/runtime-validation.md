# Transport Association — Runtime Validation

**Status:** proposed read-only validation plan. No hooks enabled, no memory written, no money moved.
**Source of truth:** `docs/FUEL PLAN/association-investigation/report.md` and `association-adapter-contract.md`.
**Rule:** all validation is observational/logging only. The observer must not patch, call, or modify any game function or memory.
**Scope:** validation targets **individual transfer-call observation**, not pickup-to-delivery association. No durable operation ID exists (report C8), so no test may claim to link a pickup call to its delivery call.

## 1. Feasible observation mechanism (preconditions)

Ghidra is unavailable in this environment (report §0: `scratch\ghidra_run.bat`, `TesmioLoader`, and `scratch/` absent), so no new call sites or offsets can be confirmed now. The feasible observation mechanism is therefore:

1. **Read-only observer installed at the 16 known transfer call sites** (report §4). Call-site logging is read-only with respect to game memory: it reads arguments and return values but writes nothing to game addresses.
2. **Observer enabled only when hooking is separately permitted.** This document specifies *what* to log and *what assertions to check*; it does not authorize enabling hooks now.
3. **No game memory writes:** use `ReadablePtr(ptr, size)` before dereferencing any offset; if unverified, wrap in try/heuristic and degrade gracefully (report architecture rule B). Pre-allocate a static ring buffer; use `_snprintf_s`/`_snwprintf_s`; no `std::string`/`std::vector`/`new`/`malloc` in the observation path (rule C, Zero-Allocation Rule).
4. **Graceful degradation:** missing service → hide UI row / skip event, do not crash (rule A).

### 1.1 What can be validated now vs later

| Validation | Feasible now without Ghidra? | Requires Ghidra / hooks? |
|---|---|---|
| Confirm the 16-site list against existing disassembly dumps | yes (report §4 already cites them) | no |
| Confirm no indirect calls (19-ref census) | yes (report H14) | no |
| Per-event field capture at call sites | no — needs observer installed | yes (hooks) |
| `param_9`/RSI at 0x694F40 / 0x6953C4 | no | yes (H10) |
| Log-entry `Building*` offset | no | yes (H7) |
| `param_2` caller provenance at 0x16F831 | no | yes (report C1) |

## 2. Read-only logging requirements

Log one record per transfer call passing the gate (site-specific AND tons > 0 for delivery identification), using the schema in `association-adapter-contract.md` §4. Logging is append-only; records are never updated in place; `event_id` is never reused.

### 2.1 Minimum logged fields per event

| Field | Required? | Reason |
|---|---|---|
| `event_id` (monotonic) | yes | unique per observed call |
| `tick` | yes | time correlation |
| `call_site_rva` | yes | site-specific gating |
| `mode` | yes | polarity interpretation (C2: mode alone insufficient) |
| `tons_sign` | yes | delivery vs return leg |
| `tons_nominal` | yes | caller intent |
| `out_moved` | yes (when param_6 != NULL) | actual transferred quantity |
| `al_success` | yes | valid-only-when-AL==1 gate |
| `resource_id` | yes | commodity identity |
| `vehicle_id` | yes | specific live vehicle (C1 caller caveat at 0x16F831) |
| `home_building` | yes | carrier/home |
| `current_building` | yes (C3 caveat) | physical location at call; may be NULL in transit |
| `explicit_destination` | yes (when param_9 != NULL) | delivery endpoint at unload |
| `is_refuel` | yes | separate from freight (proven) |
| `is_delivery` | yes | derived per contract §5.2 (site AND tons > 0) |

### 2.2 Log format

Append-only text or binary records; static pre-allocated buffer; on overflow, drop oldest and log a drop counter (read-only diagnostic, not game state). Never write to game memory.

## 3. Validation tests (observational, per-call only)

Each test runs on a disposable save, read-only. No game writes; no hooks enabled in this environment.

### 3.1 Test A — overlapping identical shipments

**Goal:** verify two identical shipments (same vehicle type, resource, quantity, pickup, delivery) produce distinct events and are not collapsed.

**Setup:** two vehicles of the same type hauling the same resource between the same two buildings, scheduled so transfer calls interleave/overlap.

**Logging checks:**
- Each transfer call yields a separate `event_id`.
- `vehicle_id` differs between vehicles (or same vehicle at different ticks).
- `current_building` / `explicit_destination` identical across shipments.
- **Pass:** N distinct events for N physical calls; no event merging.
- **Fail:** any two calls sharing all derived fields assigned the same `event_id` or merged.
- **Not tested:** whether the two calls belong to one "operation" — no durable operation ID exists (report C8).

### 3.2 Test B — partial transfers

**Goal:** verify `out_moved` reflects actual (clamped) quantity, not nominal `tons`.

**Setup:** vehicle attempts unload larger than cargo content; load into slot with insufficient free capacity.

**Logging checks:**
- `al_success == 1` and `out_moved < tons_nominal` (clamped down).
- `out_moved == 0.0` with `al_success == 1` when slot already empty/full (mutation committed, net zero).
- `al_success == 0` with no trusted `out_moved` on early exit.
- **Pass:** recorded quantity = `out_moved`, never `tons_nominal`. **[proven, report H13]**

### 3.3 Test C — cancellation / early exit

**Goal:** verify `al_success == 0` events logged but not billed.

**Setup:** dispatch vehicle with no cargo to unload site; invalid resource id.

**Logging checks:**
- `al_success == 0` recorded.
- `out_moved` not written / not trusted.
- No event with `al_success == 0` contributes to any aggregate.
- **Pass:** zero failed events billed. **[proven, report H13]**

### 3.4 Test D — rehoming

**Goal:** verify `home_building` tracks carrier/home; `current_building` reflects physical location incl. NULL-in-transit.

**Setup:** rehome vehicle (change depot/DO assignment); observe in transit.

**Logging checks:**
- `home_building` reads new home after rehoming.
- `current_building` non-NULL at arrival, NULL in transit, reset to home after transfer (via `FUN_1403FBDC0`, not the transfer — report H13 correction).
- **Pass:** `home_building` and `current_building` logged correctly at every call site; NULL-in-transit not misinterpreted. **[report H11 + C3 caveat]**

### 3.5 Test E — save/load

**Goal:** verify observer log handles save/load safely; in-game DO task entries persist natively.

**Setup:** log several transfer events; save; reload; continue.

**Logging checks:**
- DO task entries / TaskCfg reconstructed natively by `FUN_1401F0BE0` (serializer `FUN_1406C5350`, deserializer `FUN_1406C6040`). **[proven, ghidra-do-storage-findings]**
- Observer's event log NOT persisted by engine. After reload: fresh `event_id` sequence OR pre-reload events marked `pre_save`; **no cross-load correlation by raw pointer** (handles may be stale).
- No stale-pointer dereference after load.
- **Pass:** no crash/stale read after save/load. **[plugin isolation rule]**

### 3.6 Test F — internal refuel vs fuel commodity transport

**Goal:** verify refuelling distinguished from fuel freight, not billed as delivery.

**Setup:** observe vehicle refuelling from home DO (0x16F831, mode 0, tons < 0, fuel) and fuel delivery unload (mode 1, tons > 0).

**Logging checks:**
- `is_refuel == 1` only at refuel site; `is_delivery == 0`.
- Fuel delivery: `is_refuel == 0`, `is_delivery == 1`, `tons > 0`.
- **Pass:** refuelling excluded from freight billing; fuel deliveries tagged `is_delivery`. **[proven, report G2]**

### 3.7 Test G — sea/rail dock legs (known sites)

**Goal:** verify mode/sign classification at named sea/rail sites; negative-tons legs not billed as deliveries.

**Setup:** observe ship/train through `FUN_14068B330`.

**Logging checks:**
- 0x68F792: mode 0, tons > 0, `param_9 == veh+0x4F8`, `out == NULL`.
- 0x68F7F9: mode 1, tons < 0, `param_9 == veh+0x4F8`, paired with 0x68F792.
- 0x694F40: mode 0, tons < 0, `param_9 == RSI` (RSI deferred — H10).
- 0x6953C4: mode 1 (always), tons > 0, `param_9 == RSI` (deferred — H10).
- Negative-tons legs → `is_delivery == 0`.
- **Pass:** site + sign polarity matches report §4/H15; no negative-tons leg billed as delivery.

### 3.8 Test H — threads / interleaving (observational)

**Goal:** confirm no concurrent transfers for the same vehicle.

**Logging checks:**
- For each `vehicle_id`, transfer calls serialized (no overlapping ticks).
- All call sites on game-tick/sim thread. **[proven, report §10]**
- **Pass:** no two events for same `vehicle_id` share a tick. If observed, flag `threading_anomaly`; do not group.

## 4. Evaluation criteria (pass/fail)

| Criterion | Pass | Fail |
|---|---|---|
| Distinct events per physical call | N calls → N `event_id`s | merged/collapsed |
| `out_moved` used, not `tons_nominal` | billed = clamped actual | billed = nominal |
| `al_success == 0` excluded | zero billing from failed calls | failed calls billed |
| Refuel separated from freight | `is_refuel` excludes from billing | refuel billed as delivery |
| Negative tons excluded | tons < 0 → `is_delivery == 0` | return legs billed |
| Save/load pointer safety | no stale-pointer deref after load | crash or stale read |
| Duplicate shipments not merged | identical shipments = distinct events | identical shipments merged |

**No criterion tests pickup-to-delivery association.** No durable operation ID exists (report C8); any such claim is out of scope.

## 5. Deliverables of validation

- Event log (read-only artifact) per test run.
- Pass/fail matrix per criterion above.
- `threading_anomaly` / `unverified_field` observations, flagged but not acted on.

No implementation, no hooks, no money movement results from this validation.
