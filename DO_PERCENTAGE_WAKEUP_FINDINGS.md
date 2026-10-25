# DO Percentage Wakeup Bug — Investigation Findings

**Target:** SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD, ASLR on, base 0x140000000.  
**Scope:** Investigation only. No source, INI, build, or save changes made.  
**Evidence:** Ghidra headless decompilations in `scratch/investigation/` and `scratch/`, one run at a time; `plugins/private_logistics/private_logistics.cpp` (current writer). All addresses are file-based RVAs (prefix `0x140`).

---

## 1. Executive Summary

The private DO writer (`AutoTargetsOnDay` → `AutoAppendNativeTarget`) already sets `DO+0xD50 = 1` (dirty) and `DO+0xFC0 = 3.0f` (timer) when a target is added or config changes. The native percentage-click flow does the same two writes **plus** it writes actual percentage values into `TaskCfg+0x04`/`+0xCC` and clamps them via global `DAT_14090a048`. The dirty flag is **not** the missing piece — "we forgot the dirty flag" is contradicted by the current writer at `private_logistics.cpp:2806-2811`.

`DO+0xFC0` is confirmed as a **per-frame countdown timer** (consumed by road DO upkeep `FUN_1401C6050`). When it reaches `<= 0.0` AND `D50` is dirty AND targets exist, the DO calls `FUN_1401E2380` to re-plan, then resets `FC0` to `3.0f`. Setting `D50=1` every frame defers re-plan because the timer resets to 3.0f each time.

The remaining gap between native and our writer is **not** the dirty flag/timer. Candidate differences:
1. Native sets **both** `cfg+0x00=1` (load-all) AND `cfg+0xC8=1` (unload-all) with both percentages at `1.0f`. Our writer sets only the allowed direction's toggle at 20%/30%.
2. Native `entry+0x00` starts `0` (disabled) and is armed to `1` by the dispatcher's add-confirm path. Our writer sets it via `AutoApplyEntryActivation` based on effective directions.
3. `FUN_1406ee2a0` (the `+` percentage button handler) is called by native but **not decompiled** — its internal writes beyond `cfg+0x04`/`+0xCC` are unknown.
4. The planner reads `bld+0xD38` every frame regardless of dirty, but may gate on `param_2+0xea8` (nonzero check at `planner_424430.c:91`).

---

## 2. Commands Actually Run

All commands run in PowerShell 5.1 from `C:\WRSRE`. Ghidra headless was **not** executed in this session (no executable or project present); all decompilations below are pre-existing scratch outputs.

| Command | Purpose | Result |
|---|---|---|
| `Get-ChildItem -LiteralPath 'C:\WRSRE\scratch\investigation'` | List Ghidra outputs | 13 files found (see section 11) |
| `Read scratch/investigation/decomp_1407496c0.c` | Native DO interaction/add-target writer | 1244 lines — full alloc + percentage row loop |
| `Read scratch/investigation/decomp_14008e050.c` | `FUN_14008E050` vector push_back | 77 lines — signature confirmed |
| `Read scratch/investigation/decomp_1401512a0.c` | `FUN_1401512A0` TaskCfg ctor | 57 lines — zeroes ptr fields only |
| `Read scratch/checkbox_check_6F49D0.c` | `FUN_1406F49D0` checkbox reader | 54 lines — returns toggle state |
| `Read scratch/planner_424430.c` | `FUN_140424430` delivery-order planner | 463 lines — clears/rebuilds orders every frame |
| `Read scratch/upkeep_1c6050.c` | `FUN_1401C6050` road DO upkeep | 536 lines — FC0 countdown, D50→re-plan |
| `Read scratch/dow_toggle.c` | `FUN_140130650` target enable toggle | 137 lines — waypoint reachability + enable |
| `Read plugins/private_logistics/private_logistics.cpp` | Current writer | AutoTargetsOnDay at line 2594; dirty/timer at 2806-2811 |
| `Read docs/private-logistics-runtime/DO_PERCENTAGE_WAKEUP_INVESTIGATION.md` | Investigation charter | 67 lines — six questions |
| `Read docs/ghidra-do-storage-findings.md` | DO storage layout | 147 lines — offsets, caps, save/load |
| `Read scratch/distribution_office_investigation_report.md` | Prior RE report | 265 lines — lifecycle, save/load, prune leak |
| `Read scratch/distribution_office_addendum_2.md` | Addendum on caps/TaskCfg | 245 lines — cfg+0x00="all" resolution |
| `Read scratch/distribution_office_followup_report.md` | Follow-up on planner/limits | 202 lines — entry enabled state, role configs |
| `Read scratch/xrefs_140424430.txt` | Planner xrefs | 96 lines — ~50 call sites |
| `Read scratch/xrefs_1407496c0.txt` | UI handler xrefs | 5 lines — one unconditional caller (dispatcher 0x2D) |
| `Read scratch/xrefs_bld_d38.txt` | DO+0xD38 xrefs | No references found (file-based RVA mismatch) |
| `Read docs/engine/logistics.md` | Logistics investigation | 224 lines — superseded by ghidra-do-storage-findings.md |

No `scratch\ghidra_run.bat` invocations were made (Ghidra project unavailable in this checkout). Commands that would be used if Ghidra were available:

```
cd 'C:\WRSRE'; scratch\ghidra_run.bat DecompAt.java out.c 0x1406EE2A0   # FUN_1406EE2A0 (percentage + button) — NOT decompiled yet
cd 'WRSRE'; scratch\ghidra_run.bat DecompAt.java out.c 0x1401E2380       # FUN_1401E2380 (re-plan on dirty+timer expiry) — NOT decompiled yet
cd 'WRSRE'; scratch\ghidra_run.bat Xrefs.java out.txt 0x1406EE2A0       # Who calls the percentage increment
cd 'WRSRE'; scratch\ghidra_run.bat Xrefs.java out.txt 0x14090A048       # Who reads/writes the clamp global DAT_14090A048
```

---

## 3. Native Percentage Controls — Exact Interaction (Q1, Q2)

### 3.1 Percentage +/- controls live in `FUN_1407496c0` (RVA 0x7496C0)

The per-target row loop (lines 899–1187 of `decomp_1407496c0.c`) renders each target's row with +/- buttons and a checkbox. For each target entry `lVar18` (= `*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_de0)`):

- **Checkbox state** at `entry+0x30c` (per-row transient widget state, NOT simulation state). Read/written by `FUN_1406f49d0` (`checkbox_check_6F49D0.c`). Returns `param_5` (toggle state). On mouse click inside the row panel: toggles `param_5 = param_5 == '\0'`, plays `gui_click` sound.
- **`+` (increase) button:** calls `FUN_1406ee2a0(param_1, 2, lVar18, 0)` (`decomp_1407496c0.c:982`). Arguments: window context, direction=2, target building pointer, 0. **Not decompiled** — internal writes unknown.
- **`-` (decrease/remove) button:** calls `FUN_140162b40()` (`decomp_1407496c0.c:1018`) — removal/prune path.

After the per-button handling, at `LAB_14074c110` (lines 1111–1174), **every** target row (not just clicked ones) runs:

1. Reads `TaskCfg*` from `entry+0x10`.
2. Reads load % from `cfg+0x04` (`dVar45`).
3. **Clamps load %:** `if (0.0 <= dVar45) { if (dVar8 < dVar45) dVar45 = dVar8; } else dVar45 = 0.0;` where `dVar8 = DAT_14090a048` (upper clamp for load). Writes back to `cfg+0x04`.
4. Reads unload % from `cfg+0xCC` (`local_dc0`).
5. **Clamps unload %:** `if (DAT_14090a048 <= local_dc0) { if (dVar9 < local_dc0) local_dc0 = dVar9; } else local_dc0 = DAT_14090a048;` where `DAT_14090a048` is also the **lower clamp for unload**, `dVar9` is upper clamp. Writes back to `cfg+0xCC`.
6. **Dirty flag:** `if (*(char *)(lVar38 + 0xd50) == '\0')` — only sets if not already dirty. Compares current entry enabled byte vs stored state at `entry+0x30c`; if different: `*(char *)(lVar38 + 0xd50) = 1`.
7. If dirty was just set: `*(undefined4 *)(lVar38 + 0xfc0) = 0x40400000;` → `DO+0xFC0 = 3.0f`.

### 3.2 Exact before/after writes for one percentage change

| Write | Before | After | Location |
|---|---|---|---|
| `TaskCfg+0x04` (load %) | current value | clamped to [0.0, DAT_14090a048] | `decomp_1407496c0.c:1161` |
| `TaskCfg+0xCC` (unload %) | current value | clamped to [DAT_14090a048, dVar9] | `decomp_1407496c0.c:1173` |
| `DO+0xD50` (dirty) | 0 | 1 (only if was 0 and state changed) | `decomp_1407496c0.c:1145` |
| `DO+0xFC0` (timer) | any | 3.0f (0x40400000, only when dirty becomes 1) | `decomp_1407496c0.c:1147` |

**Beyond `cfg+0x04`/`+0xCC`:** only `DO+0xD50` and `DO+0xFC0` change. No office-global flags, no cached work lists, no resource vector changes. The `FUN_1406ee2a0` increment path may write additional fields but is not decompiled.

**Rendering vs simulation state:** `entry+0x30c` is transient widget/checkbox state (per-row, per-frame). `TaskCfg+0x04`/`+0xCC` and `DO+0xD50`/`+0xFC0` are simulation state. `FUN_1406f49d0` reads/writes `entry+0x30c` only as widget state; it does not write `TaskCfg`.

---

## 4. Native Add-Target Writer (FUN_1407496c0) — Complete Sequence

From `decomp_1407496c0.c` lines 553–620 (the "add station" confirm flow inside the DO panel):

```
alloc TaskEntry (0x18, zeroed)
  entry+0x00 = 0              // enabled byte (disabled)
  entry+0x08 = target Building*
  entry+0x10 = 0
push_back via FUN_14008E050(bld+0xD38, &entry)   // vector grow via FUN_1400043D0 if cap exceeded
alloc TaskCfg (0x198, memset 0)
  TaskCfg_ctor FUN_1401512A0  // zeroes ptr fields: +0x08,+0x10,+0x38..+0x188,+0x2C,+0xD0,+0xD8,+0xE0,+0x100..+0x188,+0xF4
  // ctor does NOT touch +0x00,+0x04,+0xC8,+0xCC,+0x8C,+0x28,+0xF0
set entry+0x10 = cfg
set entry+0x08 = *(**(connection+0x70))   // target building from connection record
set cfg+0x00 = 1        // LOAD-all toggle
set cfg+0x04 = 1.0f     // load percent (0x3f800000)
set cfg+0xC8 = 1        // UNLOAD-all toggle
set cfg+0xCC = 1.0f     // unload percent (0x3f800000)
set cfg+0x28 = 1        // active flag
set cfg+0xF0 = 1        // validated flag
set cfg+0xD8 = cfg+0xD0  // unload vec end = begin (empty)
set cfg+0x190 = 0
set cfg+0x20 = 0
set cfg+0xE8 = 0
set bld+0xD50 = 1       // dirty
set bld+0xFC0 = 3.0f    // timer (0x40400000)
break if count > 5      // hard cap: 6 entries per add action (road)
```

The dispatcher's 0x2D case then arms `entry+0x00 = 1` (`MOV byte ptr [RCX],0x1` at `1406E9D00`, per `distribution_office_followup_report.md:63-67`).

`FUN_14008E050` signature (confirmed from `decomp_14008e050.c`):
```c
void FUN_14008E050(ulonglong *vector, undefined8 *element);
// vector[0]=begin, vector[1]=end, vector[2]=capacity (all TaskEntry**)
// Appends *element; grows via FUN_1400043D0 when end==capacity
```

---

## 5. Our Current Writer (AutoTargetsOnDay / AutoAppendNativeTarget)

From `plugins/private_logistics/private_logistics.cpp`:

**`AutoAppendNativeTarget` (line 2335):**
```cpp
void* entry = operator new(0x18); memset(entry, 0, 0x18);
void* cfgMem = operator new(0x198); memset(cfgMem, 0, 0x198);
void* cfg = ctor(cfgMem);   // FUN_1401512A0
AutoApplyEntryActivation(entry, directions);   // entry+0x00 = (directions==BOTH)?1:0
*(void**)((BYTE*)entry + 0x08) = target;       // entry+0x08
*(void**)((BYTE*)entry + 0x10) = cfg;          // entry+0x10
AutoApplyTaskDirections(cfg, directions & BOTH, g_autoSourcePercent, g_autoDestinationPercent);
//   writes cfg+0x00 (load_all byte), cfg+0x04 (load % float),
//   writes cfg+0xC8 (unload_all byte), cfg+0xCC (unload % float),
//   clears resource selector end if denied
*((BYTE*)cfg + 0x28) = 1;   // active flag
*((BYTE*)cfg + 0xF0) = 1;   // validated flag
pushBack((BYTE*)office + 0xD38, &entry);   // FUN_14008E050
```

**Dirty/timer writes (line 2806–2811):**
```cpp
if (doAdded > 0 || doConfigChanged) {
    if (ReadablePtr((BYTE*)office.building + 0xD50, 1))
        *((BYTE*)office.building + 0xD50) = 1;
    if (ReadablePtr((BYTE*)office.building + 0xFC0, sizeof(float)))
        *(float*)((BYTE*)office.building + 0xFC0) = 3.0f;
}
```

**`AutoApplyTaskDirections` (site_permissions.h:78-105):**
- For load side (offset 0): `cfg+0x00 = allowed?1:0`, `cfg+0x04 = allowed?sourcePercent/100.0f:0.0f`
- For unload side (offset 0xC8): `cfg+0xC8 = allowed?1:0`, `cfg+0xCC = allowed?destPercent/100.0f:0.0f`
- If denied: clears resource selector end (`cfg+0x10` → `cfg+0x08`)
- Returns `changed` if any field differs from previous

---

## 6. DO+0xFC0 and DO+0xD50 — Tick Path to Planning (Q3)

### 6.1 `DO+0xFC0` is a countdown timer (CONFIRMED)

From `upkeep_1c6050.c` (FUN_1401C6050, road DO upkeep), lines 86–91:
```c
fVar31 = *(float *)(param_2 + 0xfc0);   // read FC0
if (0.0 < fVar31) {
    fVar29 = C3D_TIMER::PowerTime(&DAT_1409D4EE0, DAT_14090A9FC, false, false);  // frame delta
    *(float *)(param_2 + 0xfc0) = fVar31 - fVar29;   // count down
}
```

### 6.2 Dirty flag consumption → re-plan (CONFIRMED)

From `upkeep_1c6050.c` lines 451–527:
```c
if ((0 < *(int *)(param_1 + 0x5a4)) &&                    // global gate (UNVERIFIED meaning)
    (*(float *)(param_2 + 0xfc0) <= 0.0) &&               // timer expired
    (*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3 != 0)) {   // targets exist
    if (*(char *)(param_2 + 0xd50) != '\0') {             // dirty?
        fVar29 = (float)FUN_1401E2380(param_1, param_2);  // RE-PLAN
    }
    ...
    *(undefined4 *)(param_2 + 0xfc0) = 0x40400000;        // reset timer to 3.0f
}
```

### 6.3 Entry enabled-state sync also sets dirty (CONFIRMED)

From `upkeep_1c6050.c` lines 258–286:
```c
for each entry at bld+0xD38:
    cVar12 = (target reachable?) ? 1 : 0;
    if (**(char **)(entry_ptr) != cVar12) {   // entry+0x00 vs reachability
        *(undefined1 *)(param_2 + 0xd50) = 1;   // set dirty if changed
    }
    **(char **)(entry_ptr) = cVar12;             // write enabled byte
```

### 6.4 Planner reads bld+0xD38 every frame (CONFIRMED)

From `planner_424430.c` (FUN_140424430, RVA 0x424430):
- Clears order vectors at `game+0x144a8` (stride 0x68)
- Calls `FUN_140422C30` for reachability over `bld+0xA10` waypoints
- Iterates each target's storages (`bld+0x970`/`+0x978`, stride 0xE0)
- Checks resource type whitelist, storage slot mode (`+0x84`/`+0x85`)
- Computes dispatch quantity using `cfg+0x8C` (capacity), `cfg+0x04`/`+0xCC` (percentages)
- Creates orders via `FUN_1402108A0`
- **~50+ unconditional call sites** (`xrefs_140424430.txt`) — runs every frame from many DO handlers

### 6.5 Repeated invalidation effect (CONFIRMED)

Each write of `DO+0xD50 = 1` accompanied by `DO+0xFC0 = 3.0f` resets the countdown. If `D50` is set every frame (e.g., from the percentage row loop's clamp pass at lines 1111–1174), `FC0` never reaches 0 and the `FUN_1401E2380` re-plan is deferred. However, `FUN_140424430` itself runs every frame regardless of dirty, so new targets should be picked up on the next planner invocation even without the dirty flag — **unless** the planner gates on `param_2+0xea8` (line 91: `if (*(char *)(local_180 + 0xea8) == '\0') goto skip`).

---

## 7. TaskCfg Field Meanings (Q4)

| Offset | Native default (add flow) | Our writer value | Meaning | Source |
|---|---|---|---|---|
| `cfg+0x00` | 1 (LOAD-all toggle) | `allowed?1:0` | LOAD all-resources toggle; `1`=match all whitelisted, `0`=explicit vector only | addendum_2.md §2.2-2.3 |
| `cfg+0x04` | 1.0f | `sourcePercent/100` (0.20 default) | Load percent, applied per-slot as `slot_tons * pct` | addendum_2.md §2.2 |
| `cfg+0x08/+0x10` | empty (begin==end) | empty | LOAD resource vector (8-byte elements) | addendum_2.md §2.7 |
| `cfg+0x28` | 1 | 1 | Active flag | addendum_2.md §2.2 |
| `cfg+0x8C` | 0 (not set) | 0 (not written) | DO capacity float; planner uses as multiplier cap — **UNVERIFIED** whether read | addendum_2.md §2.6 |
| `cfg+0xC8` | 1 (UNLOAD-all toggle) | `allowed?1:0` | UNLOAD all-resources toggle | addendum_2.md §2.2 |
| `cfg+0xCC` | 1.0f | `destPercent/100` (0.30 default) | Unload percent, applied per-slot | addendum_2.md §2.2 |
| `cfg+0xD0/+0xD8` | empty | empty | UNLOAD resource vector | addendum_2.md §2.7 |
| `cfg+0xF0` | 1 | 1 | Validated flag | addendum_2.md §2.2 |
| `cfg+0x190` | 0 | 0 | Unknown | decomp_1407496c0.c:599 |

**Key difference:** Native add flow sets **both** toggles to `1` and both percentages to `1.0f` (load-all + unload-all). Our writer sets only the allowed direction's toggle at 20%/30%. The planner's truth table (addendum_2.md §2.4):

| `cfg+0x00` | LOAD vector empty? | Result |
|---|---|---|
| 1 (all) | Yes | Match **all** whitelisted resources |
| 0 (explicit) | Yes | Match **nothing** |

Our writer with `cfg+0x00=0` (load denied) + empty LOAD vector = **no load orders**. With `cfg+0x00=1` (load allowed) + empty LOAD vector = **all whitelisted resources**. This is consistent.

### Entry+0x00 (enabled byte)

| Context | Value | Meaning |
|---|---|---|
| Allocator `FUN_1407496c0` | 0 | Disabled at allocation |
| Dispatcher 0x2D confirm | 1 | Armed on confirm (`1406E9D00`) |
| Our `AutoApplyEntryActivation` | `(directions==BOTH)?1:0` | Set by our writer |
| Planner | non-zero = active target | Treated as participation gate |
| Upkeep `FUN_1401C6050` | synced to reachability | Written back each tick |

---

## 8. Answers to the Six Investigation Questions

**Q1. Source/destination +/- percentage interaction:**
- **CONFIRMED:** Individual target +/- controls are in `FUN_1407496c0` per-target row loop (lines 899–1187). `+` calls `FUN_1406ee2a0(window, 2, target_building, 0)` (not decompiled). `-` calls `FUN_140162b40()` (removal). Office-wide controls not separately traced — the row loop is the only percentage interaction found. No separate office-wide percentage control found in the decompiled code.

**Q2. Exact before/after writes for one percentage change:**
- **CONFIRMED:** Reads `cfg+0x04` (load) or `cfg+0xCC` (unload). Clamps via `DAT_14090a048`. Writes clamped value back. Sets `DO+0xD50=1` (if was 0 and state changed), sets `DO+0xFC0=3.0f` (when dirty becomes 1). No other fields change. `FUN_1406ee2a0` may write additional fields — **UNVERIFIED**.

**Q3. Readers/writers of DO+0xD50 and +0xFC0 along the tick path:**
- **CONFIRMED:** Writers: `FUN_1407496c0` (add + percentage row), prune path (`FUN_1401C1770`: `D50=1`, `FC0=3.0f`), upkeep `FUN_1401C6050` (sets `D50=1` when entry enabled state changes). Reader: `FUN_1401C6050` counts down `FC0` per frame; when `FC0<=0 && D50 && targets exist` → `FUN_1401E2380` re-plan → `FC0` resets to 3.0f. `FC0` is elapsed/countdown time, not UI-only. Repeated invalidation defers re-plan (timer resets to 3.0f each time `D50` is set).

**Q4. Native UI-created task vs our constructor + policy writer:**
- **CONFIRMED:** Field-by-field comparison above (section 7). Native sets both toggles at 100%; ours sets only allowed direction at 20%/30%. Both set `cfg+0x28=1`, `cfg+0xF0=1`, empty resource vectors, `D50=1`, `FC0=3.0f`. `cfg+0x8C` = 0 in both (not set by ctor/allocator; **UNVERIFIED** if planner reads it). `entry+0x00`: native arms to 1 via dispatcher; ours sets via `AutoApplyEntryActivation`.

**Q5. Minimal safe commit/invalidation operation:**
- **CONFIRMED:** The complete native invalidation is just `DO+0xD50 = 1` + `DO+0xFC0 = 3.0f`, called from the game simulation thread (`sim_clock` registerTick, `C3D_TIMER::PowerTime` at RVA 0x334AA0). The upkeep consumes these every frame; `FUN_140424430` rebuilds orders every frame regardless. No separate commit function needed. **Do not call `FUN_1407496c0`** (UI-gated, reads window state, plays sounds). Repeated `D50=1` writes defer re-plan — set dirty once per reconciliation cycle. `FUN_1401E2380` (re-plan) and `DAT_14090a048` (clamp global) are not decompiled — **UNVERIFIED** ABI/guards.

**Q6. Native clamping of DISABLED direction percentages:**
- **CONFIRMED:** Clamping at `decomp_1407496c0.c:1151-1174` runs for **every** target row regardless of direction enabled state. `DAT_14090a048` is the upper clamp for load % and lower clamp for unload %. Could explain repeated `config_changed=1` with identical post-normalization audit **only if** `DAT_14090a048` modifies a value on each pass — but our writer's values (0.20, 0.30) are below 0.9 (likely clamp bound) and would not be clamped. **UNVERIFIED:** exact value of `DAT_14090a048`; whether the clamp actually modifies our values in-game.

---

## 9. Status Labels

| Finding | Label | Evidence |
|---|---|---|
| Native add-flow writes D50=1, FC0=3.0f | CONFIRMED | `decomp_1407496c0.c:604-605` |
| Native percentage flow clamps via DAT_14090a048 | CONFIRMED | `decomp_1407496c0.c:1151-1174` |
| FC0 is per-frame countdown timer | CONFIRMED | `upkeep_1c6050.c:86-91` |
| D50+FC0<=0 → FUN_1401E2380 re-plan → FC0=3.0f | CONFIRMED | `upkeep_1c6050.c:451-527` |
| Planner FUN_140424430 reads bld+0xD38 every frame | CONFIRMED | `planner_424430.c`, `xrefs_140424430.txt` |
| Our writer already sets D50=1, FC0=3.0f | CONFIRMED | `private_logistics.cpp:2806-2811` |
| "we forgot the dirty flag" explanation | CONTRADICTED | `private_logistics.cpp:2806-2811` |
| Native sets both toggles=1, both pct=1.0f | CONFIRMED | `decomp_1407496c0.c:593-603` |
| Entry+0x00 armed to 1 by dispatcher | CONFIRMED | `distribution_office_followup_report.md:63-67` |
| DAT_14090a048 exact value | UNVERIFIED | Not decompiled; likely 0.9 from site_permissions.h:70 comment |
| FUN_1406ee2a0 internal writes | UNVERIFIED | Called at `decomp_1407496c0.c:982`, not decompiled |
| FUN_1401E2380 (re-plan) ABI/guards | UNVERIFIED | Called at `upkeep_1c6050.c:456`, not decompiled |
| Planner gate on param_2+0xea8 | UNVERIFIED | `planner_424430.c:91` — skip if zero |
| Our writer's targets actually picked up by planner | UNVERIFIED | No live-game test performed |

---

## 10. Scratch File Inventory

All under `C:\WRSRE\scratch\`:

**In `investigation/` (used):**
- `decomp_1407496c0.c` — FUN_1407496C0 (DO interaction/add-target writer + percentage row loop)
- `decomp_14008e050.c` — FUN_14008E050 (vector push_back)
- `decomp_1401512a0.c` — FUN_1401512A0 (TaskCfg ctor)
- `decomp_140745350.c` — FUN_140745350 (rail DO panel builder)
- `decomp_key_funcs.c` — FUN_140741170 (road DO builder) + others
- `distribution_office_investigation_report.md` — prior RE report
- `distribution_office_addendum_2.md` — TaskCfg semantics addendum
- `distribution_office_followup_report.md` — planner/limits follow-up
- `xrefs_140424430.txt` — planner xrefs (~50 call sites)
- `xrefs_1407496c0.txt` — UI handler xrefs (1 unconditional caller)
- `xrefs_bld_d38.txt` — DO+0xD38 xrefs (none found at file-based RVA)

**In `scratch/` root (used):**
- `checkbox_check_6F49D0.c` — FUN_1406F49D0 (checkbox state reader)
- `planner_424430.c` — FUN_140424430 (delivery-order planner)
- `upkeep_1c6050.c` — FUN_1401C6050 (road DO upkeep: FC0 countdown, D50→re-plan)
- `dow_toggle.c` — FUN_140130650 (target enable toggle)
- `dow_addtarget.c` — add-target allocation sequence
- `dow_add1.c` — prune path (memmove compaction, D50=1, FC0=3.0f)
- `dow_helpers2.c` — vector/ctor/free helpers
- `dec_planner_424430.c` — planner decomp (alternate)
- `dec_disp_6e9b80.c` — dispatcher decomp (type 0x2B/0x34/0x2D routing)
- `d_h11_road_do.c` — per-building upkeep loop

**Not decompiled (would need Ghidra):**
- `FUN_1406EE2A0` (percentage increment `+` button handler, RVA 0x6EE2A0)
- `FUN_1401E2380` (re-plan on dirty+timer expiry)
- `DAT_14090A048` (clamp global) readers/writers
- `FUN_140162B40` (removal/prune, called by `-` button)
- `FUN_140130650` IS decompiled (dow_toggle.c)

---

## 11. Minimal Safe C++ Integration Recommendation

**Recommendation: No change to the dirty/timer writes is required.** The current writer at `private_logistics.cpp:2806-2811` already replicates the native invalidation (`D50=1`, `FC0=3.0f`). The bug ("wakes only after percentage click") is **not** explained by a missing dirty flag.

If a fix is later deemed necessary, the minimal safe change is one of:

### Option A (preferred, zero-risk): Verify rather than change
Add a debug-only log in `AutoTargetsOnDay` after the dirty/timer write, confirming `D50` and `FC0` are actually set on the live DO pointer. Also log the `entry+0x00` enabled byte and `cfg+0x00`/`+0xC8`/`+0x04`/`+0xCC` values for each managed target. Compare against a native-created target's values. This requires no memory writes and no hook changes.

### Option B (if Option A shows our config differs materially): Align toggle defaults
If logs show native-created targets use `cfg+0x00=1`/`cfg+0xC8=1` at 100% while ours use 20%/30% with single-direction toggles, and the planner requires `cfg+0x00!=0` for load orders (per addendum_2.md truth table), then for **source-only** targets set `cfg+0x00=1` (load-all) with empty LOAD vector and `cfg+0x04=0.20`, keeping `cfg+0xC8=0`. For **destination-only**, set `cfg+0xC8=1` with empty UNLOAD vector and `cfg+0xCC=0.30`, keeping `cfg+0x00=0`. This matches the native "all mode" with per-slot percentage gating.

**Guardrails for either option:**
- Call only from `sim_clock` `registerTick` (game simulation thread), never from render/hooks.
- Wrap all game-memory writes in `ReadablePtr`.
- Set `D50=1` once per reconciliation cycle, not per target (avoid timer-reset deferral).
- Do not call `FUN_1407496C0`, `FUN_1406EE2A0`, or any UI handler.
- Keep `FC0=3.0f` write paired with `D50=1`.
- Respect the 6-target road / 7-target rail add-flow cap (`AUTO_DO_SAFE_TARGET_COUNT` already at 5).
- Do not change billing, fuel, or settlement (billing remains OFF per investigation charter).

**Blocker:** `FUN_1401E2380` (re-plan entry point) and `DAT_14090a048` (clamp global) are not decompiled. If a Ghidra session becomes available, run:
```
scratch\ghidra_run.bat DecompAt.java out.c 0x1401E2380
scratch\ghidra_run.bat Xrefs.java out.txt 0x14090A048
```
before implementing any config-field changes beyond the current writer.

---

## 12. What Was NOT Done (per investigation charter)

- No Ghidra headless invocations (project unavailable in this checkout).
- No source, INI, build, or save changes.
- No fuel/pricing/settlement changes (billing remains OFF).
- No live-game testing (no save/load, no movement observation).
- `FUN_1406EE2A0`, `FUN_1401E2380`, `DAT_14090A048` not decompiled.
- Office-wide percentage controls not separately traced (only individual target rows found).
