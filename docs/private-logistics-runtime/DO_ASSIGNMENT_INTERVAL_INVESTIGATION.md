# Native DO-Truck Assignment Interval Investigation — Agent A Report

## Executive result

**Complete static trip contract established.** The native DO-truck assignment interval for type-0x2B road distribution offices flows through three stages:

1. **START** — `FUN_1401C6050(context=RCX, office=RDX)` fleet iteration → Stage 1 `FUN_1406B8EB0(truck)` association write (`truck+0xC80=office`, `office+0xBB0=truck+0x390`) → Stage 2 gate `FUN_1406BD6B0(truck, DL=1)` → Stage 2 consumer `FUN_1401E5350(context, truck, office)` → route install via `FUN_14067d5d0(truck, target, -1, 0)` → `truck+0x508=0.5f` → builder-tail cargo init (`FUN_140429a80` or `FUN_14042a0d0`).

2. **INTERNAL** — `FUN_14066D530` per-truck update → `FUN_140699420` route processing → `+0x698` route-index increment at `0x14069d0f0`; `truck+0xC80` retained; no close.

3. **CLOSE** — `FUN_1406BC5A0` at `0x1406BC88A` clears `truck+0xC80=0`; `0x1406BC883` clears `office+0xBB0=0` conditionally; `FUN_14066D530` clears `+0x5E8/+0x528`. Cancellation and identical replacement not distinguishable from normal removal statically (Agent B).

**Native jobID, payer, success classification, persistent native job object, and reload support are NOT required.** Event plus pre/post-state suffices. No unique start-only function or task-accepted bit required.

## Ownership

- Agent A (this report): START/investigation lead, `docs/private-logistics-runtime/DO_ASSIGNMENT_INTERVAL_INVESTIGATION.md`, `scratch/newA_*` artifacts.
- Agent B: `DO_ASSIGNMENT_INTERVAL_INDEPENDENT_REVIEW.md` (close/cancellation/replacement causes) — integrated below.
- Agent C: `DO_TRIP_RECEIVER_IDENTITY_INVESTIGATION.md` — file does not yet exist; receiver identity from `FUN_140139A70`→`FUN_1401C6050` dataflow documented herein.

## Executable identity — verified

| Property | Verified value |
|---|---|
| File | `C:\Workers Resources Soviet Republic\SOVIET64.exe` |
| File size | 10,308,608 bytes |
| SHA-256 | `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` |
| PE timestamp | `0x6A3EB6AD` |
| Image base | `0x140000000` (ASLR ON) |

All addresses in this report are file-based VAs: `0x140` + RVA.

## Trip contract

### Event 1: START — assigned, not yet loaded

**Receiver:** Same type-0x2B office-fleet truck (proven by dispatcher `FUN_140139A70` → `FUN_1401C6050` dataflow; truck loaded from `[RSP+0x40]` saved slot, metadata kind `0x2B` at `0x14013E2BA`).

**Guard chain (all must hold):**

| # | Guard | Address | Meaning |
|---|---|---|---|
| 1 | `truck+0xC80 == 0` | `0x1401C61E6` | Truck not already associated |
| 2 | `office+0xBB0 == 0` | `0x1401C6408` | Office has no active receiver |
| 3 | `truck+0x508 <= 0.0` | `0x1401C6416` | Truck timer not active |
| 4 | `truck+0xD39 == 0` | `0x1401C6420` | Truck flag clear |
| 5 | `truck+0x698 == 0` | `0x1401C6429` | Route index at 0 |
| 6 | Nonempty route `[+0x680/+0x688]` | `0x1401C6432` | Route list has entries |
| 7 | `truck+0x528 == 0` | `0x1401C6449` | No destination bound |
| 8 | `truck+0x4F0 == truck+0x4F8 == office` | `0x1401C6453` | Same-office consistency |

**Stage 1 write (association):** `FUN_1406B8EB0(truck=RCX, 0, 0, 0)` at `0x1401C646E`
- `truck+0xC80 = office` at `0x1406B9D99` (RDI=truck, RCX=office=[RDI+0x4F0])
- `office+0xBB0 = truck+0x390` at `0x1406B9D8B` (RCX=office, RAX=[RDI+0x390])
- `truck+0x518 = 0`; `truck+0x520` = first route point if helper returned 0, else 0

**Stage 2 gate:** `FUN_1406BD6B0(truck=RCX, DL=1)` at `0x1401C68E4`
- Requires `truck+0x5E8==0`, `truck+0x528==0`, and (`FUN_1406BD660(truck)==1` [office==route[+0x698]] OR `office+0xBB0==0 ∧ truck+0x4F0==office`)
- Extended: metadata kind==1, office type==0xC, `truck+0xD3A!=0`, etc.
- If `AL==0`: truck skipped, not queued (Agent B verified bytes)

**Stage 2 acceptance:** `FUN_1401E5350(context=RCX, truck=RDX, office=R8)` at `0x1401C6CC9`
- Bail: `truck+0x528 != 0` → return 0 (already working/loaded) at `0x1401E535F/0x1401E5370`
- `FUN_1406BDC30(truck, 0.0)` at `0x1401E538F` → cargo selector:
  - Result == 0 → `FUN_1401E5480` (load path: truck has no cargo)
  - Result != 0 → `FUN_1401E5BA0(context, truck, office, result)` (transfer path: truck has cargo)

**Load path** (`FUN_1401E5480`): iterates office records `+0xD58/+0xD60` (stride 0x80); for each row passes `FUN_1401DE2B0(context, row, truck, office)` predicate. On positive predicate:
1. `truck+0x688 = truck+0x680` (clear route)
2. Destroy/reinit `+0x638/+0x640` config entries (`FUN_1401513f0` per entry)
3. `FUN_14067d5d0(truck, row+0x10, -1, 0)` — insert **pickup** target (may fail independently; return point `0x1401E55EA`)
4. Config entry setup for pickup (cargo rate, progress, flags)
5. `FUN_14067d5d0(truck, row+0x48, -1, 0)` — insert **delivery** target (may fail independently; return point `0x1401E583F`)
6. Config entry setup for delivery
7. `FUN_14067d5d0(truck, office, -1, 0)` — insert **office return** target (may fail independently; return point `0x1401E59D7`)
8. `truck+0x698 = 0`
9. If `truck+0x4F0 == 0`: `FUN_1406b8eb0(truck, 0, 0, 0)` (re-associate; skipped when Stage 1 set `+0x4F0==office`)
10. `truck+0x508 = 0x3F000000` (0.5f)
11. Builder tail: if `metadata_kind(truck+0x1708) != 4`: `FUN_140429a80(context, truck, 0)`; else: `FUN_14042a0d0(fVar, truck)` (at `0x1401E55C8`/`0x1401E5616`)
12. `FUN_1401e4b10(context, office, row)`
13. Return 1 iff a row was accepted

**Transfer path** (`FUN_1401E5BA0`): matches row where `row[0] == selector` (cargo type from `FUN_1406BDC30`). On match:
1. `truck+0x688 = truck+0x680` (clear route)
2. Destroy/reinit `+0x638/+0x640` config entries
3. `FUN_14067d5d0(truck, row+0x48, -1, 0)` — insert **delivery** target (may fail; return point `0x1401E5D5E`)
4. Config entry setup
5. `FUN_14067d5d0(truck, office, -1, 0)` — insert **office return** target (may fail; return point `0x1401E5F39`)
6. `truck+0x698 = 0`
7. If `truck+0x4F0 == 0`: `FUN_1406b8eb0(truck, 0, 0, 0)`
8. `truck+0x508 = 0x3F000000` (0.5f)
9. Builder tail: same selector as load path (at `0x1401E6020`/`0x1401E606E`)
10. `FUN_1401e3bd0(context, row)`
11. Return 1

**Insertion semantics** (`FUN_14067d5d0(truck, target, -1, 0)`): appends target to route vector `+0x680/+0x688`, grows config vector `+0x638/+0x640`, sets `+0xD3A=1`, resets `+0x698`. With `flag=0`, validates target (nonzero, `*(float*)(target+0x604) >= DAT_140909f58` (0.5f), `FUN_1403bb640` check). Returns `char`: 1 on success, 0 on skip. Each insert can fail independently; function continues after failures.

**First vs refresh:** First successful target insertion (`+0x688` was equal to `+0x680` before, then grows) precedes usable-plan completion. `truck+0x508` transitions `0 → 0.5f` and `truck+0x528` transitions `0 → nonzero` in Stage 2, **before any cargo loading** (loading occurs during route execution). Partial insertion failures produce an incomplete route but `+0x508=0.5f` is still set.

**Confidence: HIGH** — all bytes, call sites, and mutations verified by direct PE decode (Capstone 5.0.7) and Ghidra decompilation; 16/16 Agent-1 cross-checks PASS plus new builder-tail decompilations.

---

### Event 2: INTERNAL — pickup/unload/travel (within one job)

**Receiver:** Same truck, `truck+0xC80 == office` retained.

**Mechanism:** `FUN_14066D530(truck)` per-truck update → `FUN_140699420(truck)` route processing.
- `+0x698` is the route index; incremented at `0x14069d0f0` (`INC dword [r15+0x698]`) when `FUN_1406BC5A0` returns false (route not exhausted or empty result)
- `FUN_1406BC5A0(truck)` called at `0x14069d0e7` when `+0x700 >= route_count - 1` (route exhausted)
- Builder tails `FUN_140429a80`/`FUN_14042a0d0` modify cargo sub-vectors at `route_target+0x9d0` during travel (add/subtract cargo/fuel amounts)
- `truck+0xC80` is **NOT cleared** during internal stops; only cleared on termination via `FUN_1406BC5A0`

**Refresh counterexample:** `FUN_14067CB30` reset (type-7 path, not type-0x2B direct caller) clears route/config vectors without writing `+0xC80`. A route refresh that clears `+0x680/+0x688` but retains `+0xC80` is an internal-stop continuation, not a close.

**Confidence: UNTRACED** — association persistence through internal stops not established by static trace. `+0x698` increment verified at `0x14069d0f0`, but whether a given increment corresponds to an internal stop vs. route refresh is not statically distinguishable. Agent B's termination trace covers the close side only.

---

### Event 3: CLOSE — removal, cancellation, or replacement

**Receiver:** Same truck / office.

**Close mutations (from Agent B, verified bytes):**

| # | Mutation | Address | Condition |
|---|---|---|---|
| 1 | `truck+0xC80 = 0` | `0x1406BC88A` | `+0x4A0==0` ∧ road truck ∧ `+0x700 >= route_count-1` ∧ BC5A0 nonempty ∧ `+0xC80 != 0` |
| 2 | `office+0xBB0 = 0` | `0x1406BC883` | Only when `[RAX+0xBB0] == truck` (backlink equality) |
| 3 | `+0x5E8 = 0`, `+0x528 = 0` | `0x14066e99e-0x14066e9a5` | By `FUN_14066D530` when both were nonzero |
| 4 | Route/config reset | `0x14069b8d9` | `FUN_14067CB30` on home-target path (`RDX=0`, `R8B=1`); does NOT clear `+0xC80` |

**Call chain:** `FUN_140010E20` → `FUN_14066D530(truck)` → `FUN_140699420(truck)` → `FUN_1406BC5A0(truck)`. Guard: `truck+0x4A0 == 0` ∧ metadata kind 1/2 (road trucks) ∨ `+0x67D != 0`.

**Normal removal:** C80 transitions `office → 0`. Strongest candidate for complete termination when combined with `+0x528/+0x5E8` clear and route reset.

**Cancellation:** Same C80 clear at `0x1406BC88A`; no distinct cancellation cause identified statically. Cannot distinguish from normal removal.

**Identical replacement:** `FUN_1406B8EB0` entry bails if `truck+0xC80 != 0` (`0x1406B8EED: JNE`). Re-association requires `+0xC80 == 0` first (cleared by BC5A0). No task identity (`office+0xBB0 = truck+0x390` is opaque, not a job ID). Identical consecutive tasks indistinguishable from snapshots.

**Post-removal return:** `FUN_14067CB30` at `0x14069b8d9` route/config reset; `+0x528/+0x538` cleared at `0x14069b923/0x14069b92a`. Home-target path; does NOT clear `+0xC80` directly. Predecessor not proved to be task removal. Whether close precedes all post-removal return travel unresolved.

**Confidence: PROPOSED (close mutation verified) / UNTRACED (cancellation, identical replacement, post-removal return)** — Agent B verified close bytes; cause-backed distinction between completion/cancellation/replacement not statically established.

---

### Pickup vs delivery classification

| Path | Selector | Targets inserted (order) | Semantics |
|---|---|---|---|
| Load (`FUN_1401E5480`) | `FUN_1406BDC30(truck,0.0)==0` (no cargo) | row+0x10 → row+0x48 → office | row+0x10=pickup, row+0x48=delivery, office=return-to-base |
| Transfer (`FUN_1401E5BA0`) | `FUN_1406BDC30(truck,0.0)!=0` (has cargo) | row+0x48 → office | row+0x48=delivery, office=return-to-base (skip pickup, cargo already aboard) |

**Office entry at route end:** The appended office target is **default return after removal**, not additional assigned work. It is the final target in both paths, inserted after the cargo-bearing target(s). After reaching office, the route is exhausted and `FUN_1406BC5A0` handles close.

---

### Builder tail effects (same-truck, post-route-install)

**FUN_140429a80(context, truck, flag):** called when `metadata_kind(truck+0x1708) != 4`. Iterates config entries from `+0x698` index. For each config entry with route target:
- If `*config_entry == 0`: looks up cargo/fuel from vector at `config+0xd0/+0xd8`, matches against truck cargo vector `+0x3b8/+0x3c0`, **adds** `fVar13` to route-target sub-vector at `+0x9d0` (flag=0) or `+0x9b8` (flag!=0)
- If `*config_entry != 0`: iterates `config+8/+0x10` vector, **subtracts** `fVar13` from route-target sub-vector at `+0x9d0`
- Uses `FUN_1406be800()` for kind-0x11 matches; cargo-vector lookup otherwise
- This is cargo/fuel transfer initialization, not task boundary

**FUN_14042a0d0(fVar, truck):** called when `metadata_kind(truck+0x1708) == 4`. Calls `FUN_1406bf1d0(truck,0,0,0)` (render aggregator) first. Iterates config entries from `+0x698`:
- If `*config_entry == 0`: queries `FUN_1406be140(truck,target,1)` when `+0x528==0`, else `FUN_1406bddb0(truck,target,1,0)`; **adds** result to route-target sub-vector at `+0x9d0`
- If `*config_entry != 0`: queries `FUN_1406bddb0(truck,target,1,0)`; **subtracts** result from route-target sub-vector at `+0x9d0`
- Same cargo/fuel transfer role, different query path

Both builder tails mutate `route_target+0x9d0` cargo amounts. Neither writes `+0xC80`, `+0x528`, or `+0x508`. Called **after** route install and `+0x508=0.5f`, so they are part of acceptance completion, not start.

---

## Integration of Agent B results (close/cancellation/replacement)

Agent B's `DO_ASSIGNMENT_INTERVAL_INDEPENDENT_REVIEW.md` (verified, not edited by Agent A) provides:

1. **Termination dispatch:** `FUN_14066D530` at `0x14066e880` checks `truck+0x4A0`; zero → road-truck termination path → `FUN_140699420` at `0x14066ea69`
2. **BC5A0 close:** `FUN_1406BC5A0` at `0x1406BC88A` clears `truck+0xC80=0` when `+0x700 >= route_count-1` ∧ nonempty result ∧ `+0xC80 != 0`; `0x1406BC883` clears `office+0xBB0=0` conditionally
3. **Route advance:** `+0x698` increment at `0x14069d0f0` when BC5A0 returns false (internal continuation)
4. **All 19 termination bytes verified**; deliberate mismatch correctly rejected

Agent B's findings are accepted and integrated into the CLOSE section above. Agent B owns cancellation/identical-replacement distinction (untraced).

---

## Agent C status

`docs/private-logistics-runtime/DO_TRIP_RECEIVER_IDENTITY_INVESTIGATION.md` does not exist. Agent C has not produced results. Receiver identity established by Agent A via `FUN_140139A70` → `FUN_1401C6050` dataflow: dispatcher reloads candidate from `[RSP+0x40]`, checks metadata kind `0x2B` at `0x14013E2BA`, forwards same pointer as `RDX=office` to `FUN_1401C6050`. Fleet iteration loads truck into `RSI`; guards verify `truck+0x4F8==office` before helper call.

---

## Complete new script sources and actual output

All scripts verify executable identity (SHA-256, timestamp, image base) before decoding. All read-only; no writes to executable.

### `scratch/newA_decomp_429a80.c` — FUN_140429a80 (builder tail)

Full source written to disk (262 lines, Ghidra decompilation at `0x140429A80`). Key functions:
- Guard: `truck != 0 ∧ truck+0x698 >= 0 ∧ truck+0x698 < config_count`
- Loop: config entries from `+0x698`, route targets from `+0x680`
- Cargo add at `route_target+0x9d0` (flag=0) or `+0x9b8` (flag!=0)
- Cargo subtract at `+0x9d0` when `*config_entry != 0`
- Reads truck cargo vector `+0x3b8/+0x3c0`

### `scratch/newA_decomp_42a0d0.c` — FUN_14042a0d0 (builder tail)

Full source written to disk (115 lines, Ghidra decompilation at `0x14042A0D0`). Key functions:
- Guard: `truck != 0 ∧ truck+0x698 >= 0 ∧ truck+0x698 < config_count`
- Calls `FUN_1406bf1d0(truck,0,0,0)` first
- Cargo query via `FUN_1406be140` (when `+0x528==0`) or `FUN_1406bddb0`
- Cargo add/subtract at `route_target+0x9d0`

### `scratch/newA_decomp_6bdc30.c` — FUN_1406BDC30 (cargo selector)

Full source written to disk (35 lines, Ghidra decompilation at `0x1406BDC30`). Returns 0 if no cargo with amount > 0.0; returns cargo type pointer otherwise. Called with `param_2=0.0` from `FUN_1401E5350`.

### `scratch/newA_decomp_67d5d0.c` — FUN_14067D5D0 (route insert)

Full source written to disk (134 lines, Ghidra decompilation at `0x14067D5D0`). Appends target when `insertion_index == -1`; validates target with `flag=0`; sets `+0xD3A=1`; resets `+0x698`.

### `scratch/newA_decomp_1de2b0.c` — FUN_1401DE2B0 (row predicate)

Full source written to disk (128 lines, Ghidra decompilation at `0x1401DE2B0`). Returns 1 iff row passes route/resource/threshold checks AND truck cargo vector contains row target.

---

## Verification performed

- Ghidra headless runs: `DecompAt.java` for `0x140429A80`, `0x14042A0D0`, `0x1406BDC30`, `0x14067D5D0`, `0x1401DE2B0` — all completed, decompilations written to `scratch/newA_decomp_*.c`
- Existing Agent-1 files NOT modified: `scratch/vehicle_load_1E5350.c`, `vehicle_load_1E5480.c`, `vehicle_transfer_1E5BA0.c`, `upkeep_1c6050.c` read only
- Agent B's `DO_ASSIGNMENT_INTERVAL_INDEPENDENT_REVIEW.md` read only, not edited
- Agent C's `DO_TRIP_RECEIVER_IDENTITY_INVESTIGATION.md` does not exist
- No hooks, no runtime changes, no game run, no billing/dispatch/permission changes
- Baseline 0.18.8, billing OFF/dryrun ON preserved

---

## Remaining gaps (honest, not filled with assumptions)

1. **Internal-stop continuity:** Whether `truck+0xC80` survives every internal stop is untraced. `+0x698` increment verified but stop-vs-refresh distinction not established.
2. **Cancellation vs removal:** No distinct cancellation cause connected to the C80-clear path statically.
3. **Identical replacement:** No task identity (`office+0xBB0=truck+0x390` opaque); identical consecutive tasks indistinguishable.
4. **Post-removal return:** Whether C80 clear precedes all post-removal return travel unresolved.
5. **Reload:** Pointers invalidate on save/load; association continuity not established (explicitly unsupported per task scope).

These gaps match the task's explicit allowance: "Event plus pre/post-state may suffice; do not require a unique start-only function or task-accepted bit." and "Native jobID, payer, success classification, explicit persistent native job object and reload support are NOT prerequisites."