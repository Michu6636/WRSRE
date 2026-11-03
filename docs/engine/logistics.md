# Logistics Investigation — Distribution Office (Autonomous Private Logistics)

> **Current implementation entry point (2026-11-01):** [How DO offices work in this mod](../private-logistics-runtime/DO_OFFICES.md) and [design section 12](../WRSRE%20DESIGN%20DOC.md) describe 0.18.17-dry-ledger: native dispatch, daily role/permission reconciliation, sampled road-fleet estimates and persisted dry records; payments OFF. This file is historical RE evidence, **not a current implementation recipe**. Do not copy the superseded pointer-vector write instructions below, infer universal receiver/unit semantics from an offset, or treat static layout descriptions as native lifecycle/billing validation. The existing supersession notice and original exports remain preserved.
> **Status 2026-10-22 — Superseded in part:** For DO task storage, see `docs/ghidra-do-storage-findings.md` (stride `0x18` Entry `+0x00/+0x08/+0x10`, `TaskCfg 0x198`, dirty `+0xD50`/`+0xFC0`, save `1EDC40/6C5350` load `1F0BE0`, type-3 road / type-6 rail, cap 5 policy, `panel+0x310` unverified). The `Logistics_Investigation` `0xD38 stride 8` pointer-vector model is incomplete; use the new doc as source of truth. Original kept for history.

**Target:** `SOVIET64.exe` v1.1.1.9 (base `0x140000000`). Addresses are file-based
(`0x140` + RVA). **Raw evidence:** `scratch/distribution_office.txt`.
Investigation mode only — no source/INI/build changes.

## TL;DR

- **Distribution Office = `BUILDINGTYPE 43` (`0x2B`)**; the rail variant is type 52.
  The road DO panel is built by **`FUN_140741170`**, dispatched from the central
  window dispatcher `FUN_1406E9B80` on `type == 0x2B`.
- **The DO's target/source list is a `std::vector<TaskEntry*>` at `bld + 0xD38`**
  (begin) / `bld + 0xD40` (end), **stride 8**.
- **Each task entry** is a heap object:
  - `entry + 0x08` → **target `Building*`**
  - `entry + 0x10` → **`TaskCfg*` config object**
  - `entry + 0x00` → small int/state (not read by the panel; see Open Item).
- **The config object** holds the load/unload instructions as **two symmetric
  triples** `{ all-toggle byte, percent float, resource vector }`:
  - `cfg+0x00 / +0x04 / +0x08` — LOAD (pick-up from OUTPUT storages, slot mode 2)
  - `cfg+0xC8 / +0xCC / +0xD0` — UNLOAD (drop-off to INPUT storages, slot mode 1)
  - `cfg+0x8C` — capacity (float, used by the runtime sim).
- The DO's **trucks** are a separate vector at `bld + 0xC70` (stride 8). The runtime
  planner `FUN_140424430` turns these configs into delivery orders in
  `game + 0x144A8` (stride `0x68`); `FUN_140422c30` does route reachability over
  `bld + 0xA10` waypoints (stride `0x60`).

## 1. Building type

| Constant | Value |
|---|---|
| `BUILDINGTYPE_DISTRIBUTION_OFFICE` | 43 (`0x2B`) |
| `BUILDINGTYPE_DISTRIBUTION_OFFICE_RAIL` | 52 (`0x34`) |

Confirmed from `media_soviet\scripts\SOVIETInstructions.txt` and the
`distribution_office_*.ini` `$TYPE_*` tags. This doc targets the **road DO (43)**.

## 2. Panel dispatch

`FUN_1406E9B80` (dispatcher) — DO case:

```c
else if (uVar1 == 0x2b) {                 // type 43 = Distribution Office
    FUN_1406f8cc0(param_1, puVar2);       // common header/storage strip
    FUN_140741170(param_1, puVar2);       // DO panel builder (16328 bytes)
}
```

`puVar2 + 0x240` is the DO building object.

## 3. The DO task / target array — `bld + 0xD38`

```c
// DO building "bld":
std::vector<TaskEntry*>  targets;   // bld+0xD38 (begin), bld+0xD40 (end), stride 8
std::vector<Vehicle*>    vehicles;  // bld+0xC70 (begin), bld+0xC78 (end), stride 8
```

Loop head (raw):

```asm
MOV RAX,[RSI + 0xd40]      ; RSI = DO building
SUB RAX,[RSI + 0xd38]
SAR RAX,0x3                ; numTasks
...
MOV R11,[RDI + RAX]        ; R11 = targets[i]   (TaskEntry*)
MOV R10,[R11 + 0x8]        ; R10 = entry->targetBuilding
MOV RAX,[R11 + 0x10]       ; RAX = entry->config
```

## 4. Task-entry struct

| Offset | Type | Meaning |
|---|---|---|
| `+0x00` | int/state | entry id / flags (not read by the panel — **Open Item**) |
| `+0x08` | `Building*` | **target building** (source or destination) |
| `+0x10` | `TaskCfg*` | **config object** (load/unload instructions) |

The target building is read through the standard storage layout
(`target+0x970/+0x978` storages, stride `0xE0`; slot `+0x90` = resource type,
slot `+0x08` = tons).

## 5. Task-config object — load / unload instructions

Two triples; each = all-toggle byte + percent float + resource vector:

| | All-toggle | Percent | Resource vector |
|---|---|---|---|
| **LOAD** (from OUTPUT, slot mode 2) | `cfg+0x00` byte | `cfg+0x04` float | `cfg+0x08`/`+0x10` (stride `0x10`) |
| **UNLOAD** (to INPUT, slot mode 1) | `cfg+0xC8` byte | `cfg+0xCC` float | `cfg+0xD0`/`+0xD8` (stride `0x10`) |

`cfg+0x8C` = capacity (float). Resource-vector elements are
`{ Resource* +0x00, float +0x08, float +0x0C }` and are compared against the global
resource sentinels at `game+0xC3C8/0xC3D0/0xC3D8/0xC3E0` (ALL / food / …).

The all-toggle byte is an "any/all resources" switch; the percent float is the
fill/empty threshold the DO respects when dispatching. Storage-slot mode decides
direction: **mode 1 = input (DO unloads into it)**, **mode 2 = output (DO loads
from it)**.

## 6. Runtime logistics engine

- **`FUN_140424430`** — delivery-order planner. Clears/rebuilds the global order
  vector `game + 0x144A8` (stride `0x68`), walking each target's storages and
  applying the config's resource lists, percentages and capacity.
- **`FUN_140422c30`** — route reachability over the DO's waypoints
  `bld + 0xA10` (stride `0x60`); fills `game + 0x14478` (reachable sources) and
  `game + 0x14490` (their modes).
- **`FUN_14040c550`** — draws a target marker on the map.

## 7. Open item — `entry+0x00` / add-target handler

The panel builder only **reads** `bld+0xD38`. The add/remove of a `TaskEntry` (and
thus the definitive init of `entry+0x00`) is in the map-click "assign building to
DO" handler, not yet decompiled. `entry+0x00` is almost certainly a small
id/state/flags field, but confirm it before writing to it.

## 8. Implementation notes for "Autonomous Private Logistics"

To populate a DO's task list from a mod:

1. Resolve the DO building; write to `*(bld + 0xD38 .. bld + 0xD40)` a vector of
   heap-allocated `TaskEntry` (stride 8). Keep begin/end consistent.
2. For each entry, set `entry+0x08 = targetBuilding` and `entry+0x10 = &TaskCfg`.
3. In the `TaskCfg`, set the two triples: e.g. for "haul components from warehouse
   A to factory B", put A as a LOAD target (fill `cfg+0x08` resource vector, toggle
   `cfg+0x00`, percent `cfg+0x04`) and B as an UNLOAD target (`cfg+0xD0`,
   `cfg+0xC8`, `cfg+0xCC`). Respect storage slot modes (2 = pick-up, 1 = drop-off).
4. Assign at least one vehicle via `bld + 0xC70` (each vehicle exposes
   `+0x3A0`/`+0x3B8` cargo vectors).
5. The engine's own `FUN_140424430` will then generate orders in `game + 0x144A8`.

> Caveat: the exact byte meaning of the all-toggles (`cfg+0x00`, `cfg+0xC8`) and
> `entry+0x00` should be verified against the add-target handler before relying on
> them for a write path. Reads (totals, membership, resource lists) are solid.


---

# Part 2 — DO "Add-Target" Writer (TaskEntry allocation)

**Raw evidence:** `scratch/distribution_office_writer.txt`.

## TL;DR

- The write path is **`FUN_1407496c0`** (the DO panel's mouse/interaction handler),
  **not** the builder `FUN_140741170` (which only reads `bld+0xD38`).
- Flow: "Add station" button arms UI command `building_addstation` → map-pick mode
  (global command `SetDistOffSource`/`SetDistOffDestination`) → confirm ("Add")
  → allocate + `push_back`.
- **`TaskEntry = operator_new(0x18)` (24 bytes, 3 qwords).** `entry+0x00` is a
  **byte ENABLED flag**, initialized to **0** (new targets default OFF) — this
  resolves the Part-1 open item. `entry+0x08 = target Building*`, `entry+0x10 =
  TaskCfg*`.
- **`TaskCfg = operator_new(0x198)` (408 bytes), `memset` 0**, ctor `FUN_1401512a0`.
  Defaults: `cfg+0x00 = 1` (LOAD all), `cfg+0x04 = 1.0`; `cfg+0xC8 = 1` (UNLOAD
  all), `cfg+0xCC = 1.0`; both resource vectors start empty (`end = begin`);
  `cfg+0x28 = 1`, `cfg+0xF0 = 1`.
- **Safety contract:** no global registry. The handler only allocates, `push_back`s
  into `bld+0xD38`, sets **`bld+0xD50 = 1`** (dirty flag) and arms `bld+0xFC0 = 3.0`.
  The runtime planner `FUN_140424430` **blindly reads `bld+0xD38`**; the dirty flag
  is what triggers the DO's own upkeep to re-plan.

## 1. Where the write happens

`FindDisp d38`/`d40` return hundreds of hits, but almost all are *other* structs
with a coincidental `+0xD38/+0xD40`. Filtering to the DO object:

| Function | Role |
|---|---|
| `FUN_140741170` (DO builder) | reads `bld+0xD38` only |
| `FUN_140745350` (rail DO builder) | reads only |
| **`FUN_1407496c0` (DO interaction)** | **the add-target writer** |
| `FUN_1401c1770` / `FUN_1401c6050` | vehicle upkeep / prune stale targets |

The append is a `CALL FUN_14008e050(bld+0xD38, &newEntry)` (vector `push_back`),
not an inline `MOV`.

## 2. The allocation (verbatim behaviour)

Per selected building (max 6):

```c
TaskEntry* e = operator_new(0x18);   // 24 bytes
*(byte*)e = 0;                        // +0x00 enabled = 0 (OFF)
*(Building**)(e+8)  = 0;              // +0x08
*(TaskCfg**)(e+0x10) = 0;             // +0x10
vector_push_back(bld+0xD38, e);       // FUN_14008e050
TaskCfg* c = operator_new(0x198);     // 408 bytes
memset(c, 0, 0x198);
c = TaskCfg_ctor(c);                  // FUN_1401512a0
e->config = c;                        // +0x10
e->target = selectedBuilding;         // +0x08
c->loadAll = 1;      c->loadPct  = 1.0f;   // +0x00, +0x04
c->unloadAll = 1;    c->unloadPct = 1.0f;  // +0xC8, +0xCC
c->loadVec.end   = c->loadVec.begin;       // +0x10 = +0x08  (empty)
c->unloadVec.end = c->unloadVec.begin;     // +0xD8 = +0xD0  (empty)
c->f0x28 = 1;  c->f0xF0 = 1;
bld->dirty = 1;                          // +0xD50
bld->uiTimer = 3.0f;                     // +0xFC0
```

"Add" **replaces** the whole target list first (`*(bld+0xD40) = *(bld+0xD38)` resets
`end = begin`, then old selections are dropped from `bld+0xD78/+0xD80`).

## 3. `entry+0x00` resolved

It is a **byte active/enabled toggle**, default **0**. The builder's per-target
checkbox (`FUN_140130650`) writes it, and when set the DO is marked dirty
(`bld+0xD50`). It is not an id/state field.

## 4. Safety contract

No global listener list is updated. `FUN_140424430` reads `bld+0xD38` directly every
tick. A mod must: allocate a 24-byte entry (zeroed), set `+0x08`/`+0x10`, allocate a
408-byte `TaskCfg` (zeroed + the default flags/percent/empty vectors), `push_back`
into `bld+0xD38` keeping begin/end/cap consistent, and **set `bld+0xD50 = 1`** so the
DO's upkeep re-plans. Skip the dirty flag and the new target is ignored until
something else changes the DO.

> Caveat: `TaskCfg` is `0x198` bytes with additional vectors at `+0x38..+0x188` and
> flags at `+0x28`/`+0xF0` beyond the two documented triples; those are read by the
> runtime sim and should be left at their ctor defaults unless you map them fully.
