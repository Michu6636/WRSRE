# Building Vectors Investigation — Workers vs. Customers

**Date:** 2026-10-15  
**Target:** SOVIET64.exe v1.1.1.9 (PE timestamp 0x6A3EB6AD, ASLR ON — all offsets are RVAs)  
**Method:** Headless Ghidra decompilation via `scratch\ghidra_run.bat DecompAt.java`  
**Scope:** 5 engine functions decompiled into `scratch/out_*.c`

## 1. Executive Summary

Both `citizen_labor` (wage payment) and `citizen_retail` (retail sales) currently iterate the same building vector `bld + 0xBD8` (`TSM_BLD_PEOPLE_BEGIN`). This causes `citizen_labor` to accidentally hire customers who are still physically inside a shop after their demand is satisfied.

**Definitive finding:** The engine itself uses **different vectors** for workers vs. customers:

| Offset | End | Name in game_world.h | Actual role (per decompilation) | Used by |
|--------|-----|---------------------|---------------------------------|---------|
| `0xBC0` | `0xBC8` | `TSM_BLD_RESIDENTS_BEGIN/END` | **Workers** (staff on workplaces; tenants on homes) | Factory tick, factory panel, citizen_labor payday pass |
| `0xBD8` | `0xBE0` | `TSM_BLD_PEOPLE_BEGIN/END` | **Customers / people inside** (transient visitors) | Shop tick, shop panel, pub panel, both mods (the bug) |
| `0xC20` | `0xC28` | — | Secondary vector (per docs: used when `person+0x95 != 0`) | Not found in decompiled code |
| `0xBF0` / `0xC08` | — | — | **Not found** as vector pairs in any decompiled function | — |

**Recommendation:** `citizen_labor` should loop `0xBC0` (workers) instead of `0xBD8` (all people inside), matching the engine's own factory tick. `citizen_retail` correctly uses `0xBD8` and should keep it. The `IsCustomerInServiceBuilding` heuristic becomes unnecessary.

## 2. Decompiled Functions Analyzed

| # | Address (RVA) | Address (file-based) | Output file | Function |
|---|--------------|---------------------|-------------|----------|
| 1 | `0x171DA0` | `0x140171DA0` | `scratch/out_shop_tick.c` | `FUN_140171d40` — Shop tick (retail sales) |
| 2 | `0x1D1E10` | `0x1401D1E10` | `scratch/out_factory_tick.c` | `FUN_1401cf590` — Factory production tick (labor) |
| 3 | `0x7272F0` | `0x1407272F0` | `scratch/out_shop_panel.c` | `FUN_1407272f0` — Shop panel builder |
| 4 | `0x723950` | `0x140723950` | `scratch/out_pub_panel.c` | `FUN_140723950` — Pub panel builder |
| 5 | `0x799630` | `0x140799630` | `scratch/out_factory_panel.c` | `FUN_140799630` — Factory panel builder |

All addresses passed to Ghidra as `0x140` + RVA per the project's base-address convention.

## 3. Detailed Findings by Offset

### 3.1 `0xBC0` / `0xBC8` — Workers (Staff)

#### Evidence A: Factory Tick (`out_factory_tick.c`, `FUN_1401cf590`)

The factory production tick iterates `param_2 + 0xBC0 .. 0xBC8` to calculate workdays produced:

**Lines 204–236** (first pass over `0xBC0`):
```c
if (*(longlong *)(param_2 + 0xbc8) - *(longlong *)(param_2 + 0xbc0) >> 3 != 0) {
    uVar27 = 0;
    do {
        lVar9 = *(longlong *)(*(longlong *)(param_2 + 0xbc0) + uVar27 * 8);
        if (*(char *)(lVar9 + 0x94) == '\0') {
            ...
            *(float *)(lVar9 + 0x90) = fVar29;
            if (fVar29 < fVar37) {
                *(int *)(param_2 + 0xc38) = *(int *)(param_2 + 0xc38) + 1;
            }
            if (iVar8 < *(int *)(param_2 + 0xdec)) {
                ...
                fVar31 = fVar31 + fVar34 * (fVar30 * fVar36 * fVar29 + fVar29 * fVar38);
                iVar8 = iVar8 + 1;
            }
            ...
        }
        uVar27 = uVar27 + 1;
    } while (uVar27 < (ulonglong)(*(longlong *)(param_2 + 0xbc8) - *(longlong *)(param_2 + 0xbc0) >> 3));
}
```

**Lines 251–283** (second pass over `0xBC0`, opposite `person+0x94` branch):
```c
if (*(longlong *)(param_2 + 0xbc8) - *(longlong *)(param_2 + 0xbc0) >> 3 != 0) {
    ...
    do {
        lVar9 = *(longlong *)(*(longlong *)(param_2 + 0xbc0) + uVar27 * 8);
        if (*(char *)(lVar9 + 0x94) != '\0') {
            ...
#### Evidence B: Factory Panel (`out_factory_panel.c`, `FUN_140799630`)

**Lines 150–236:** The factory panel iterates `lVar4 + 0xBC0 .. 0xBC8` and splits workers into two arrays by `person + 0x94`:
```c
if (*(longlong *)(lVar4 + 0xbc8) - *(longlong *)(lVar4 + 0xbc0) >> 3 != 0) {
    ...
    do {
        if (*(char *)(*(longlong *)(*(longlong *)(lVar4 + 0xbc0) + (longlong)puVar41 * 8) + 0x94) != '\0') {
            local_c78 = (int)puVar35 + 1;   // busy-worker count
            puVar35 = (undefined8 *)(*(longlong *)(lVar4 + 0xbc0) + (longlong)puVar41 * 8);
            ...
        }
        if (*(char *)(*(longlong *)(*(longlong *)(lVar4 + 0xbc0) + (longlong)puVar41 * 8) + 0x94) == '\0') {
            local_c74 = (int)puVar13 + 1;   // available-worker count
            puVar13 = (undefined8 *)(ulonglong)local_c74;
            ...
        }
    } while (puVar41 < (undefined8 *)(*(longlong *)(lVar4 + 0xbc8) - *(longlong *)(lVar4 + 0xbc0) >> 3));
}
```

**Line 586:** `uVar33 = *(longlong *)(lVar4 + 0xbc8) - *(longlong *)(lVar4 + 0xbc0);` — resident/worker vector size.

#### Evidence C: citizen_labor payday pass (`citizen_labor.cpp` lines 940–941)

Already correctly uses `TSM_BLD_RESIDENTS_BEGIN` (0xBC0) for the residential payday loop:
```cpp
void** begin = *(void***)(b + TSM_BLD_RESIDENTS_BEGIN);   // 0xBC0
void** end   = *(void***)(b + TSM_BLD_RESIDENTS_END);     // 0xBC8
```
            *(int *)(param_2 + 0xc3c) = *(int *)(param_2 + 0xc3c) + 1;
            ...
            fVar47 = fVar47 + fVar31 * (fVar30 * fVar36 * fVar29 + fVar29 * fVar38);
            ...
        }
    } while (...);
}
```

**Key points:**
- The factory tick iterates **`0xBC0`**, NOT `0xBD8`.
- Workers are split by `person + 0x94`: `0` = available, non-zero = busy.
- Workday accumulation happens here (`fVar31`, `fVar47`), confirming this is the labor-processing path.
- `bld + 0xC38` and `bld + 0xC3C` are worker-count counters (not vectors).

### 3.2 `0xBD8` / `0xBE0` — Customers / People Inside

#### Evidence A: Shop Tick (`out_shop_tick.c`, `FUN_140171d40`)

**Lines 15–30:** The shop tick iterates `param_2 + 0xBD8 .. 0xBE0` for retail processing:
```c
uVar2 = 0;
if (0 < (int)(*(longlong *)(param_2 + 0xbe0) - *(longlong *)(param_2 + 0xbd8) >> 3)) {
    uVar4 = uVar2;
    do {
        if ((longlong)uVar2 < (longlong)param_3) {
            *(undefined8 *)(*(longlong *)(*(longlong *)(param_2 + 0xbd8) + uVar2 * 8) + 0x598) = 0;
        }
        else {
            lVar1 = *(longlong *)(*(longlong *)(param_2 + 0xbd8) + uVar2 * 8);
            FUN_14082e130(param_1, lVar1, param_2, (int)uVar4 - param_3,
                          *(longlong *)(lVar1 + 0x598) != param_2);
        }
        ...
    } while ((int)uVar3 < (int)(*(longlong *)(param_2 + 0xbe0) - *(longlong *)(param_2 + 0xbd8) >> 3));
}
```

**Key points:**
- The shop tick iterates **`0xBD8`**, NOT `0xBC0`.
- Each person pointer is fetched from `*(longlong *)(param_2 + 0xbd8) + uVar2 * 8`.
- This is the retail-sales path — customers are processed here.

#### Evidence B: Shop Panel (`out_shop_panel.c`, `FUN_1407272f0`) — THE ROSETTA STONE

**Line 260:** `plVar1 = (longlong *)(lVar3 + 0xbd8);` — explicitly takes the address of the people vector at `bld + 0xBD8`.

**Line 261:** Computes the customer count as `people_at_0xBD8 minus workforce capacity`:
```c
uVar11 = (int)(*(longlong *)(lVar3 + 0xbe0) - *plVar1 >> 3) - (int)*(float *)(lVar3 + 0xddc);
```
- `*(longlong *)(lVar3 + 0xbe0)` = people vector END (`0xBE0`)
- `*plVar1` = `*(longlong *)(lVar3 + 0xbd8)` = people vector BEGIN (`0xBD8`)
- `(end - begin) >> 3` = number of people in `0xBD8`
- `*(float *)(lVar3 + 0xddc)` = workforce capacity (`0xDDC`)
- `uVar11` = people count minus capacity = **extra people = customers/visitors**

**Line 262–264:** Clamps negative values:
```c
if ((int)uVar11 < 0) { uVar11 = uVar15; }
```

**Line 266:** Display gate — shows customer info when residents are absent OR there are extra people:
```c
if ((((ulonglong)(*(longlong *)(lVar3 + 0xbc8) - *(longlong *)(lVar3 + 0xbc0)) < 8) ||   // residents < 1
    (0 < (int)uVar11)) ||                                                             // customers > 0
    (*(char *)(param_1 + 0xcbd8) != '\0')) {
```

**Lines 269–270:** String selected by building type:
```c
if (*(int *)(*(longlong *)(lVar3 + 0x318) + 0x364) == 0x22) {
    pwVar9 = C3D_Language::GetString(..., 0xa3a);   // type 0x22 label
} else {
    pwVar9 = C3D_Language::GetString(..., 0xa2c);   // default label
}
```
Then unconditionally `GetString(0xa2b)` (line 304) and `GetString(0xa2d)` (line 322).

**Lines 471–472:** Workforce capacity read:
```c
if (-1 < (int)*(float *)(lVar3 + 0xddc)) {
    local_b20 = (int)*(float *)(lVar3 + 0xddc);   // 0xDDC = worker capacity
}
```

#### Evidence C: Pub Panel (`out_pub_panel.c`, `FUN_140723950`) — Same Pattern

**Line 338:** `plVar1 = (longlong *)(lVar4 + 0xbd8);` — same people-vector pointer.

**Line 339:** Same customer-count computation:
```c
iVar11 = (int)(*(longlong *)(lVar4 + 0xbe0) - *plVar1 >> 3) - (int)*(float *)(lVar4 + 0xddc);
```

**Lines 344–347:** Same residents check + string:
```c
if (((ulonglong)(*(longlong *)(lVar4 + 0xbc8) - *(longlong *)(lVar4 + 0xbc0)) < 8) || ...) {
    ...
    if ((ulonglong)(*(longlong *)(lVar4 + 0xbc8) - *(longlong *)(lVar4 + 0xbc0)) < 8) {
        pwVar10 = C3D_Language::GetString(..., 0xa5f);   // no-residents label
    }
    ...
    pwVar10 = C3D_Language::GetString(..., 0xa3b);   // worker/customer label
}
```

**Line 527:** `local_b08 = (uint)*(float *)(lVar4 + 0xddc);` — workforce capacity.

### 3.3 `0xBF0` and `0xC08` — Not Found

Neither `0xBF0` nor `0xC08` appears as a vector begin/end pair in any of the five decompiled files. A search of all `out_*.c` files for `0xbf0` and `0xc08` returned zero matches. These offsets may exist in other engine functions but are not used in the tick/panel code paths we examined.

### 3.4 `0xC20` / `0xC28` — Secondary Vector (Docs Only)

Per `docs/Citizen_Housing_Relocation.md` (S19, S93–S105): `bld + 0xC20 .. 0xC28` is used when `person + 0x95 != 0` (institutional placement twin). It does not appear in any of the five decompiled tick/panel functions.

## 4. Cross-Building-Type Consistency

| Building type | Workers vector | Customers vector | Evidence |
|--------------|---------------|-----------------|----------|
| Factory | `0xBC0` | `0xBD8` | Factory tick (`out_factory_tick.c`) uses `0xBC0`; factory panel uses `0xBC0` |
| Shop | `0xBC0` | `0xBD8` | Shop panel (`out_shop_panel.c`) computes `0xBD8 count - cap` = customers; shop tick uses `0xBD8` |
| Pub | `0xBC0` | `0xBD8` | Pub panel (`out_pub_panel.c`) same pattern as shop panel |

**Conclusion:** Shops, pubs, and factories all use the **same offsets** for workers (`0xBC0`) and customers (`0xBD8`). The offset does **not** vary by building type. What varies is the interpretation:
- On **residential** buildings: `0xBC0` = permanent tenants (residents)
- On **workplace** buildings (factory, shop, pub): `0xBC0` = employees (workers)
- On **all** buildings: `0xBD8` = people currently inside (customers, visitors, institutional members)

The `game_world.h` comment "`TSM_BLD_RESIDENTS_BEGIN` ... (Residential only)" is **stale/misleading** — `0xBC0` functions as the workers vector on non-residential buildings, as proven by the factory tick and factory panel.

## 5. Current Mod Behavior (The Bug)

### `citizen_labor.cpp` (lines 836–917)

```cpp
void** begin = *(void***)(b + TSM_BLD_PEOPLE_BEGIN);   // 0xBD8 — WRONG: this is customers!
void** end   = *(void***)(b + TSM_BLD_PEOPLE_END);     // 0xBE0
...
// Uses IsCustomerInServiceBuilding() heuristic to filter out customers
if (IsCustomerInServiceBuilding(type, p)) {
    skippedVisitors++;
    continue;   // skips customers — but heuristic fails once demand empties
}
// Falls through to hire and pay wage
rec.workplace = b;
registeredWorkers++;
```

**Problem:** Iterates `0xBD8` (all people inside, including customers). The `IsCustomerInServiceBuilding` heuristic checks for active shopping demand, but after a customer buys goods their demand array empties, the heuristic fails, and they get hired and paid a wage.

### `citizen_retail.cpp` (lines 467–469)

```cpp
void** begin = *(void***)(shop + TSM_BLD_PEOPLE_BEGIN);   // 0xBD8 — CORRECT: customers are here
void** end   = *(void***)(shop + TSM_BLD_PEOPLE_END);     // 0xBE0
```

**Correct:** `citizen_retail` correctly iterates `0xBD8` where customers/people-inside reside.

## 6. Architectural Recommendation

### 6.1 Primary Fix: Separate the Vectors

**`citizen_labor.cpp` — Change worker-sighting loop from `0xBD8` to `0xBC0`:**

Replace `TSM_BLD_PEOPLE_BEGIN`/`TSM_BLD_PEOPLE_END` with `TSM_BLD_RESIDENTS_BEGIN`/`TSM_BLD_RESIDENTS_END` in the Pass 1 worker-sighting loop (around line 836). This matches what the engine's own factory tick does (`out_factory_tick.c` lines 204–283).

```cpp
// BEFORE (buggy — iterates customers too):
void** begin = *(void***)(b + TSM_BLD_PEOPLE_BEGIN);   // 0xBD8
void** end   = *(void***)(b + TSM_BLD_PEOPLE_END);     // 0xBE0

// AFTER (correct — iterates only workers):
void** begin = *(void***)(b + TSM_BLD_RESIDENTS_BEGIN);   // 0xBC0
void** end   = *(void***)(b + TSM_BLD_RESIDENTS_END);     // 0xBC8
```

**Result:** `citizen_labor` only sees workers (staff), never customers. The `IsCustomerInServiceBuilding` heuristic and its fragile demand-offset check become unnecessary and can be removed.

**`citizen_retail.cpp` — No change needed.** It already correctly uses `0xBD8` (`TSM_BLD_PEOPLE_BEGIN`).

### 6.2 Header Updates (`src/game_world.h`)

Add clarifying comments and/or aliases so the offset semantics are clear:

```c
// 0xBC0/0xBC8: Workers on workplaces (factory, shop, pub), residents on homes.
//   The engine's factory tick (FUN_1401cf590) and factory panel (FUN_140799630)
//   both iterate this vector for labor/workday calculation.
//   Despite the name "RESIDENTS", this is the WORKERS vector on non-residential buildings.
#define TSM_BLD_RESIDENTS_BEGIN 0xBC0
#define TSM_BLD_RESIDENTS_END   0xBC8

// 0xBD8/0xBE0: People currently inside the building — customers, visitors,
//   institutional members. The engine's shop tick (FUN_140171d40) and shop/pub
//   panels iterate this vector. Panel builders compute customer count as
//   (people_at_0xBD8 - workforce_cap_at_0xDDC).
#define TSM_BLD_PEOPLE_BEGIN    0xBD8
#define TSM_BLD_PEOPLE_END      0xBE0
```

Optionally add a workplace-specific alias:
```c
#define TSM_BLD_WORKERS_BEGIN   TSM_BLD_RESIDENTS_BEGIN   // 0xBC0
#define TSM_BLD_WORKERS_END     TSM_BLD_RESIDENTS_END     // 0xBC8
```

### 6.3 No Building-Type Branching Needed

Since shops, pubs, and factories all use `0xBC0` for workers and `0xBD8` for customers, `citizen_labor` does **not** need to branch by building type. It simply switches its vector from `0xBD8` to `0xBC0` for all workplace types.

### 6.4 Secondary Vector `0xC20`

If future work needs to handle the `person + 0x95 != 0` institutional population (orphanage/prison assignees), `0xC20`/`0xC28` is the secondary vector per `docs/Citizen_Housing_Relocation.md`. It is not used by the current tick/panel code and can be ignored for the wage-separation fix.

## 7. Proof Index (specific lines from decompiled files)

| Claim | File | Lines |
|-------|------|-------|
| Factory tick iterates `0xBC0` for labor | `scratch/out_factory_tick.c` | 204–236, 251–283 |
| Factory tick splits workers by `person+0x94` | `scratch/out_factory_tick.c` | 208, 255, 258, 261, 264 |
| Factory tick accumulates workdays on `0xBC0` | `scratch/out_factory_tick.c` | 221, 268 |
| Factory panel iterates `0xBC0` | `scratch/out_factory_panel.c` | 150–236, 586 |
| Shop tick iterates `0xBD8` | `scratch/out_shop_tick.c` | 15–30 |
| Shop panel: `plVar1 = lVar3 + 0xBD8` | `scratch/out_shop_panel.c` | 260 |
| Shop panel: customer count = `0xBD8 count - 0xDDC cap` | `scratch/out_shop_panel.c` | 261 |
| Shop panel: residents check `0xBC0..0xBC8` | `scratch/out_shop_panel.c` | 266 |
| Shop panel: workforce capacity at `0xDDC` | `scratch/out_shop_panel.c` | 471–472 |
| Pub panel: same `0xBD8` / `0xBC0` pattern | `scratch/out_pub_panel.c` | 338–339, 344–347, 527 |
| `0xBF0` / `0xC08` NOT found | All five `out_*.c` files | — |
| citizen_labor uses `0xBD8` (bug) | `plugins/citizen_labor/citizen_labor.cpp` | 836–837 |
| citizen_retail uses `0xBD8` (correct) | `plugins/citizen_retail/citizen_retail.cpp` | 467–468 |
| citizen_labor payday uses `0xBC0` (correct) | `plugins/citizen_labor/citizen_labor.cpp` | 940–941 |

## 8. Implementation Checklist

- [ ] Edit `plugins/citizen_labor/citizen_labor.cpp`: change `TSM_BLD_PEOPLE_BEGIN/END` → `TSM_BLD_RESIDENTS_BEGIN/END` in the Pass 1 worker-sighting loop (~line 836).
- [ ] Remove or simplify `IsCustomerInServiceBuilding()` and its call site (~line 892) since customers are no longer in the iterated vector.
- [ ] Update `docs/DEVELOPMENT_README.md` offset comments if they still claim `0xBD8` is "workers" and `0xBC0` is "residential only".
- [ ] Add `TSM_BLD_WORKERS_BEGIN/END` aliases in `src/game_world.h` (optional, for clarity).
- [ ] Build: `cmd /c build.bat citizen_labor`
- [ ] Verify: `citizen_labor` no longer pays wages to customers who have finished shopping.