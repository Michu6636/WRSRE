# Local Markets Investigation — City Hall Catchment & Local Resource Stocks

**Target:** `SOVIET64.exe` v1.1.1.9 (image base `0x140000000`).
**Scope:** Reverse-engineering analysis only (no source/INI/build changes).
**Raw evidence:** `scratch/cityhall_catchment.txt` (all decompiles/disassembly referenced below).

> Addresses below are **file-based** (`0x140` + RVA), matching the Ghidra project.

---

## TL;DR

- City Hall **district membership is a stored pointer**, not a radius scan: every
  building carries its Town/City entity at **`bld + 0x108`**. The City Hall
  trusts that pointer; it never measures distance to decide membership.
- The City Hall *panel* (`FUN_14072DBF0`) only **reads** pre-aggregated town
  statistics. The real "how much of resource X is available locally" summation
  lives in a separate routine, **`FUN_14033E620` ("Buy resources")**, which walks
  every building (and vehicle), filters by town (`bld+0x108`) **and optionally a
  radius**, and sums the physical `slot+0x08` stock into a live supply table.
- For localized city market prices, read the live supply table at
  **`C3D_GAME + 0xC858`** (or re-run the same building/storage walk yourself).

---

## 1. The City Hall code path (`FUN_14072DBF0`, RVA `0x72DBF0`)

`FUN_14072DBF0` is **not** a simulation tick — it is the City Hall **window/panel
builder** (a renderer), dispatched from `0x1406E9B80` on `CMP ECX, 0x27`
(`BUILDINGTYPE 39`).

```
void FUN_14072dbf0(C3D_GAME* game, WindowCtx* wnd, char measurePass)
    bld = *(Building**)(wnd + 0x240);      // the City Hall building
    town = *(void**)(bld + 0x108);         // its Town/City entity
    townStats = *(void**)(town + 0x1b8);   // the town statistics block
    switch (*(int*)(wnd + 0x260)) {        // active tab
      0 -> FUN_1402FD2E0  Overview (citizens/workers)
      1 -> FUN_14033F9D0  Production / consumption graphs
      2,3 -> FUN_140361BB0  Big statistics renderer (74,459 bytes)
      4 -> FUN_1404B24A0  Crime statistics
    }
```

The panel never recomputes the catchment or the resource totals — it reads cached
values (see §3).

---

## 2. Town / district membership (Question 1)

**Answer: buildings belong to a Town entity by pointer, not by radius.**

Every building stores a back-pointer to its town at **`bld + 0x108`**. The
overview tab (`FUN_1402FD2E0`) proves this by iterating the *global* citizen
vector and keeping only citizens whose home building's town pointer equals the
City Hall's:

```c
// ch_tab_overview.c
count = (*(longlong*)(game + 0x126b0) - *(longlong*)(game + 0x126a8)) >> 3;
for each citizen c:
    home = *(longlong*)(c + 0x20);                 // citizen's home building
    if ( (*(int*)(c + 0x71c) == 0) && home &&
         (cityHallTown == 0 || *(longlong*)(home + 0x108) == cityHallTown) )
        ...aggregate...
```

The crime tab (`FUN_1404B24A0`) and the entire labor system read the same
`(bld + 0x108) -> (+0x1b8)` chain. **There is no distance/bounding-box test in
this path.** The engine assigns `bld+0x108` when a building is placed within a
town's influence; downstream code just trusts it.

> Note: a *radius* filter **does** exist, but only inside the market aggregator
> (§3a), where it is optional and separate from town membership.

---

## 3. How local resources are aggregated & where totals live (Questions 2 & 3)

### 3a. The live market aggregator - `FUN_14033E620` ("Buy resources")

This is the function that actually sums local resource stock for market pricing.
It is called by the production tab (`FUN_14033F9D0`, ch_tab1.c:129).

```c
// ch_buyres.c  (FUN_14033e620)
for (b = *(void***)(game + 0x11B08); b != *(void***)(game + 0x11B10); b++) {
    if (typedesc(b)->type == 0x14 /*customhouse*/) continue;
    if (typedesc(b)->type == 0x0b)                 continue;
    if (radiusSq > 0.0) {                          // OPTIONAL radius filter
        if (radiusSq < C3DCalcDistanceSq2d(GetPosition(b + 0x320), center)) continue;
    }
    if (townFilter == 0 || *(void**)(b + 0x108) == townFilter) {   // TOWN filter
        for (st = *(void***)(b + 0x970); st != *(void***)(b + 0x978); st += 0xE0) {
            for (slot = *(void***)(st + 0); slot != *(void***)(st + 8); slot += 0x10) {
                content = *(float*)(slot + 0x08);                 // PHYSICAL tons
                if (threshold < content && *(void**)(slot + 0) == resource)
                    supply[resource] += content;
            }
        }
    }
}
// identical town+radius filter applied to VEHICLE cargo (game + 0x12810):
for (v = *(void***)(game + 0x12810); v != *(void***)(game + 0x12818); v++) {
    if (radiusSq > 0.0) { if (radiusSq < dist2(GetPosition(v + 0xd60))) continue; }
    for (cs = *(void***)(v + 0x3b8); ...; cs += 0x10)
        supply[res] += *(float*)(cs + 0x08);
}
```

The accumulator is a `std::vector` at **`C3D_GAME + 0xC858`**, stride `0x10`:
`{ resource* +0x00, float tons +0x08, float +0x0C }`. A richer per-resource
market table (stride `0x28`) lives at **`C3D_GAME + 0xC898`** (read by tab1).

**So local resource stock = sum of `slot+0x08` over every member building (and
every in-range vehicle) storage**, computed on demand and cached - not re-summed
by the panel every frame.

### 3b. Per-building storage layout (the walk above)

| Object | Offset | Meaning |
|---|---|---|
| building | `+0x970` / `+0x978` | storages vector begin / end (stride `0xE0`) |
| storage | `+0x00` / `+0x08` | slots vector begin / end (stride `0x10`) |
| storage | `+0x84` / `+0x85` | import / export flag (byte) |
| storage | `+0x8C` | capacity (float, whole storage, shared by slots) |
| storage | `+0x90` | transport class |
| slot | `+0x00` | `resource*` |
| slot | `+0x08` | **content** (float tons currently stored) |
| slot | `+0x0C` | quality / limit (float) |

These match `src/game_world.h` (`TSM_STORAGE_STRIDE 0xE0`, `TSM_SLOT_STRIDE 0x10`,
`TSM_SLOT_CONTENT 0x08`, `TSM_ST_CAPACITY 0x8C`).

### 3c. The Town statistics block - `*(void**)(town + 0x1b8)`

The City Hall *panel* renders from a town statistics block reached via
`town = *(void**)(bld+0x108)`, `townStats = *(void**)(town+0x1b8)`.

- Republic-wide default block is embedded in `C3D_GAME + 0x11328`.
- The array of per-town blocks is at `C3D_GAME + 0x11728` (stride `0x400`,
  end `+0x11730`) - see `ch_tab23_full.asm:1570-1582`.

Block layout (offsets relative to the block; resource vectors are stride `0x10`
= `{ resource* +0x00, float +0x08, float +0x0C }`):

| Offset | Contents |
|---|---|
| `+0x000` | int period key (`year*0x16d + day`) |
| `+0x008` | vector - production per resource |
| `+0x0b0` | vector - consumption per resource |
| `+0x0e0` | vector - labor feed (worker demand) |
| `+0x0f8` | ptr - worker series (graph) |
| `+0x128` | ptr - worker-availability series (graph) |
| `+0x1c8` | vector - import/export flow per resource |
| `+0x3e0` | float - town treasury RUB |
| `+0x3e4` | float - town treasury USD |
| `+0x0c9`,`+199`,`+200`,`+0xcc`,`+0xcd`,`+0xce` | int crime counters |

This block is **written by the simulation**, not the UI - e.g. the factory
production tick does `*(float*)(townStats + 0x3e4) += USD` and `+0x3e0 += RUB`
(`lab_dest_d2dd0.c:451/474`). These are **production/consumption rates and running
money totals - NOT the physical tons sitting in warehouses.**

---

## 4. Recommendation for localized city market prices

- The cleanest source of "local stock per resource" is the **live supply table
  `C3D_GAME + 0xC858`** maintained by `FUN_14033E620`, which already reflects the
  exact buildings + vehicles the game prices against.
- If you need strict per-town isolation (the aggregator table may be scoped to
  the currently-open trade context), reproduce the walk directly:
  1. iterate buildings `game + 0x11B08 .. +0x11B10`;
  2. keep those with `*(void**)(bld + 0x108) == cityHallTown`;
  3. for each, walk storages `bld+0x970 .. +0x978` (stride `0xE0`) -> slots
     (stride `0x10`) and sum `*(float*)(slot + 0x08)` grouped by `*(void**)(slot + 0x00)`.
- Resource names -> records via `FUN_1402AA830` (`ResourceGet`), which scans the
  resource master vector at `C3D_GAME + 0xC2B0` (stride `0x340`).

---

## 5. Correction to the mission brief

The brief listed "storages vector at `bld + 0xC78..0xC80`". On this build
**`bld + 0xC70/0xC78` is the building WORKERS list** (8-byte pointer stride),
confirmed by `labor_job_selection.c:210` and the shop decompile (`c_shop.c`
reads residents at `+0xBD8` and workforce at `+0xDDC`). The real **storages**
vector is `bld + 0x970 .. +0x978` (stride `0xE0`), as already recorded in
`src/game_world.h` (`TSM_BLD_STORAGES_BEGIN 0x970`).

---

## 6. Functions crawled

| RVA | Role |
|---|---|
| `0x14072DBF0` | City Hall panel builder / tab dispatcher |
| `0x1402FD2E0` | Overview tab (proves town-pointer membership) |
| `0x14033F9D0` | Production/consumption graph tab (calls aggregator) |
| `0x140361BB0` | Big statistics tab (disasm only - decompile times out) |
| `0x1404B24A0` | Crime tab (reads `town+0x1b8` crime ints) |
| **`0x14033E620`** | **Buy-resources market aggregator (local stock sum)** |
| `0x1402FDB00` | Graph-series accumulator |
| `0x1402AA830` | `ResourceGet(name)` -> resource record |
| `0x1400E40F0` | Storage-tooltip parser (`$STORAGE*` tokens) |
| `0x14018EEA0` / `0x14018EE50` | Special-slot (money/fuel) amount getters |
| `0x1403080B0` / `0x140308130` / `0x1403083D0` / `0x140308530` | Chart-history helpers |

---

# Part 2 — Town Names, Master Lists & Municipal Pricing

**Raw evidence:** `scratch/local_markets_pt2.txt`.  Addresses are file-based
(`0x140` + RVA).

## TL;DR (Part 2)

- **Town name** = an inline `wchar_t[128]` (256 bytes) at **`town + 0x10`** (the
  Town entity is the object at `bld + 0x108`, size `0x1C0`). Not a pointer, not a
  language ID. A second `wchar_t[64]` sits at `town + 0x11A`.
- **Wilderness rule:** an isolated building has **`bld + 0x108 == NULL`**. The save
  stores a signed town index; `< 0` or out-of-range ⇒ town pointer `0`. Isolated
  buildings are excluded from every town aggregate — never folded into a republic
  pseudo-town at the building level.
- **Master Town vector:** `C3D_GAME + 0x10348` (Town\* array, stride 8, end
  `+0x10350`). The per-town **statistics time-series** is a separate array at
  `C3D_GAME + 0x11728` (stride `0x400`, end `+0x11730`), republic default block at
  `C3D_GAME + 0x11328`.
- **Town treasury** `townStats + 0x3E0` (RUB) / `+0x3E4` (USD) is a running `+=`
  accumulator (a municipal cash pool), written by the factory production tick,
  living in the tail of the 0x400 stats block next to the Loan fields.
- **The third float at `game+0xC858 + 0x0C` is NOT a price/surcharge** — it is a
  duplicate accumulator of the same summed **tons** as `+0x08`. Local market value
  = local tons × the **global** per-resource unit price (`resource+0x64` RUB,
  `resource+0x60` USD). The engine supplies only the quantity.

## 1. Town entity & name

The Town object (0x1C0 bytes) is allocated by `FUN_140333770` and filled by
`FUN_140333870`:

| Town offset | Contents |
|---|---|
| `+0x10` | **`wchar_t[128]` display name** (e.g. "Osterburg") |
| `+0x11A` | `wchar_t[64]` secondary name/description |
| `+0x1A0`/`+0x1A8`/`+0x1B0` | std::vector (member/production feed) |
| `+0x1B8` | `TownStats*` → the 0x400 statistics block |

Read a City Hall town name as `*(wchar_t*)(*(void**)(bld + 0x108) + 0x10)`.

## 2. Wilderness / isolated buildings

Building loader `FUN_1401f0be0` (disasm `0x1401f19d4`):

```asm
townIndex = *(int*)(stack + 0x17e0);       // signed, from save
if (townIndex < 0 || townIndex >= numTowns)  // numTowns = (game+0x10350-game+0x10348)>>3
    bld+0x108 = 0;                           // NULL  -> wilderness
else
    bld+0x108 = *(game+0x10348 + townIndex*8);
```

Every consumer guards the null: overview tab `(town==0 || *(home+0x108)==town)`,
buy-resources `(townFilter==0 || *(bld+0x108)==townFilter)`. So wilderness
buildings simply never join a town aggregate.

## 3. Master lists

- **Town entities:** `C3D_GAME + 0x10348` (begin) / `+0x10350` (end), stride 8.
- **Statistics records (daily time-series):** `C3D_GAME + 0x11728` / `+0x11730`,
  stride `0x400`. The save writer `FUN_1402fe230` treats this array as a union:
  indices `< numRecords` = dated `$STAT_RECORD`; index `== numRecords` = the
  republic `$STAT_CURRENT` block at `C3D_GAME + 0x11328`; indices above = per-city
  `$STAT_CITY` blocks reached via `*(*(game+0x10348 + k*8) + 0x1b8)`.

## 4. Town treasury (`townStats + 0x3E0` / `+0x3E4`)

Written by the factory production dispatch (`lab_dest_d2dd0.c:452/475`) as
`*(float*)(townStats + 0x3e4) += USD` / `+0x3e0 += RUB`. They sit in the Economy/
Loan tail of the 0x400 block, beside `Loan_Ballance` (`+0x3F0`/`+0x3F8`) and
`Loan_Interest` (`+0x3F4`/`+0x3FC`). It is a straight running accumulator — a
per-town municipal cash pool, snapshotted daily. No interest/decay is applied to
`+0x3E0/+0x3E4` in the crawled code.

Full 0x400 stats-block layout is in `scratch/local_markets_pt2.txt` §4 (Economy
pricing scalars, per-resource import/export/production vectors at stride 0x10,
waste, citizens, crime, tourism, loan, treasury).

## 5. Pricing math (`FUN_14033E620` supply table)

`C3D_GAME + 0xC858` vector, stride 0x10 = `{ resource* +0x00, tons +0x08, tons +0x0C }`.
Both `+0x08` and `+0x0C` accumulate the same `slot+0x08` physical quantity — the
third float is a duplicate **quantity**, not a price. The displayed value is
`tons × global price`:

```c
// ch_tab1.c:1065-1066
valueRUB = tons * *(float*)(resourceRec + 0x64);   // global RUB unit price
valueUSD = tons * *(float*)(resourceRec + 0x60);   // global USD unit price
```

**For a genuine local/scarcity price**, compute it yourself from the supply tons
(`game+0xC858`) versus the town consumption vector (`townStats+0x0B0`) — the engine
only provides the local quantity and then multiplies by the fixed global price.
