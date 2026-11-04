# DO Percentage Wakeup Bug — Investigation Findings V2

**Target:** SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD, ASLR on, base 0x140000000.  
**Scope:** Investigation only. No source, INI, build, or save changes made.  
**Evidence:** Ghidra headless decompilations (run serially, one at a time), pre-existing scratch outputs, `plugins/private_logistics/private_logistics.cpp`. All addresses are file-based RVAs (prefix `0x140`).

---

## 1. Executive Conclusion and Unresolved Blockers

**The dirty flag is NOT missing.** Our writer at `private_logistics.cpp:2806-2811` already sets `DO+0xD50=1` and `DO+0xFC0=3.0f` once per daily reconciliation when additions or config changes occur — exactly matching the native add-target writer (`decomp_1407496c0.c:604-605`). The previous report's claim that "we forgot the dirty flag" is CONTRADICTED.

**The most likely root cause:** `DAT_14090A048` = **0.1** (as a double at file-based address `0x14090A048`, bits `0x3FB999999999999A`). This constant is used by the native percentage row loop (`FUN_1407496c0`, lines 1151-1174) as:
- **Load % upper clamp:** max 0.1 (10%) — our writer sets 0.20 (20%), which gets clamped DOWN to 0.10 when the player opens the DO panel
- **Unload % lower clamp:** min 0.1 (10%) — our writer sets 0.30, which is within range

Every time the native DO panel renders, it reads `cfg+0x04`, clamps it to `[0.0, 0.1]`, and writes back. Our writer's 0.20 becomes 0.10. On the next daily reconciliation, our writer sees 0.10 ≠ 0.20, rewrites 0.20, sets `doConfigChanged=1`, `D50=1`, `FC0=3.0f`. The audit shows 0.20 (after our write) but the in-memory value is 0.10 (after native clamp). This matches the investigation log: "Daily reconciliation records config_changed=1 repeatedly even when the resulting audit has identical visible values."

Whether 0.10 load % is sufficient for the planner to dispatch orders is UNVERIFIED — no live-game testing was performed.

**Unresolved blockers:**
1. `LAB_1407f22b0` (tooltip writeback callback at file-based `0x1407F22B0`) — Ghidra reports "No function found containing address." It is referenced as a callback pointer at `FUN_1407f2160:47` (`*(undefined1 **)(lVar5 + 0x78) = &LAB_1407f22b0;`). Its implementation is unknown; it may write back the modified percentage and/or set D50/FC0.
2. Global gate `param_1+0x5a4` in `FUN_1401C6050` (road DO upkeep) — must be `> 0` for the upkeep re-plan path to fire. Meaning and writers UNKNOWN.
3. `DO+0xEA8` building flag — read by planner `FUN_140424430` at `planner_424430.c:91` and by many other functions. Meaning and writers UNKNOWN.
4. Whether `FUN_140424430` (planner) actually creates orders for our targets with the current config — UNVERIFIED (no live-game test).
5. Whether reachability (`FUN_140422c30`) is established between our DO and target warehouses — UNVERIFIED.

---

## 2. Corrections to the Earlier Report

| # | Earlier Claim | Correction | Evidence |
|---|---|---|---|
| 1 | `FUN_1406EE2A0` called `FUN_1406ee2a0` as the `+` percentage increment button | **WRONG.** `FUN_1406EE2A0` is a generic window/popup creator (`operator_new(0x5D8)`, calls `FUN_14025c500`). It has **231 callers** across the entire UI system (`xrefs_1406EE2A0.txt`). It is NOT specific to percentage controls. The actual percentage slider is `FUN_1407EC380` (`slider_7EC380.c`). | `decomp_1406EE2A0.c`, `xrefs_1406EE2A0.txt`, `slider_7EC380.c` |
| 2 | `entry+0x30C` is a field on TaskEntry with per-row checkbox state | **WRONG.** TaskEntry is 0x18 bytes (24 bytes); +0x30C is out of bounds. Actual accesses: `TaskCfg+0xC8` via `entry+0x10+0xC8` (`rebuild_1E2380.c:154`), and `window+0x30C` for transient UI checkbox state (`decomp_1407496c0.c:396-399`). | `rebuild_1E2380.c:154`, `decomp_1407496c0.c:396-399` |
| 3 | Numerous planner call sites establish per-frame execution for this DO | **OVERSTATED.** `xrefs_140424430.txt` shows ~50 call sites, but most are from UI/panel builders, not per-frame simulation. Per-frame execution is only confirmed from `FUN_1401C6050` (road DO upkeep) and similar upkeep paths. | `xrefs_140424430.txt`, `upkeep_1c6050.c:451-527` |
| 4 | `DAT_14090A048` is both load upper bound and unload lower bound, guessed 0.9 | **PARTIALLY CORRECT on role, WRONG on value.** `DAT_14090A048` as a double = **0.1** (bits `0x3FB999999999999A`), NOT 0.9. Used as load % upper clamp (max 0.1) and unload % lower clamp (min 0.1) in `FUN_1407496c0:1151-1174`. Our writer's 0.20 load % IS clamped (to 0.10); 0.30 unload % is NOT clamped (above 0.1). | `floats_90A048.txt`, `decomp_1407496c0.c:1151-1174` |
| 5 | "Option B" was proposed as a fix | **INVALID.** Option B (setting load-all=1 with empty vector, unload-all=0) already matches the production writer (`site_permissions.h:78-105`, `private_logistics.cpp:2335-2390`). Not a new fix. | `site_permissions.h:78-105` |
| 6 | Native invalidation complete while callees unexamined | **CORRECTED.** `FUN_1401E2380` (re-plan) and `DAT_14090A048` (clamp global) were not previously examined. `FUN_1401E2380` is now decompiled (`rebuild_1E2380.c`); `DAT_14090A048` value is now known (0.1). `LAB_1407f22b0` writeback callback remains unexamined. | `rebuild_1E2380.c`, `floats_90A048.txt` |

---

## 3. Exact Commands Actually Executed and Results

All commands run in PowerShell 5.1 from `C:\WRSRE`. Ghidra headless invocations run serially (one at a time).

| # | Command | Result |
|---|---|---|
| 1 | `Test-Path` on JDK, Ghidra, project, SOVIET64.exe | All `True` — environment available |
| 2 | `scratch\ghidra_run.bat DecompAt.java 'C:\WRSRE\scratch\decomp_1406EE2A0.c' 0x1406EE2A0` | Succeeded — `FUN_1406EE2A0` decompiled: generic window/popup creator, 736 lines |
| 3 | `scratch\ghidra_run.bat Xrefs.java 'C:\WRSRE\scratch\xrefs_1406EE2A0.txt' 0x1406EE2A0` | Succeeded — 231 xrefs, not percentage-specific |
| 4 | `scratch\ghidra_run.bat Xrefs.java 'C:\WRSRE\scratch\xrefs_14090A048.txt' 0x14090A048` | Succeeded — 24 xrefs, including `FUN_1407496c0` at `14074c0b3` |
| 5 | Copied `PrintFloats.java` to `C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\tools\ghidra\` | Required because scratch path is skipped by Ghidra headless |
| 6 | `scratch\ghidra_run.bat PrintFloats.java 'C:\WRSRE\scratch\floats_90A048.txt' 0x14090A048 0x14090A044 0x14090A04C` | Succeeded — `0x14090A048` bits `0x9999999A` (float=-1.59e-23), `0x14090A044`=`0x14090A04C`=1.45f. As **double** spanning `0x14090A048-0x14090A04F`: bits `0x3FB999999999999A` = **0.1** |
| 7 | `scratch\ghidra_run.bat DecompAt.java 'C:\WRSRE\scratch\decomp_1407F2160.c' 0x1407F2160` | Succeeded — `FUN_1407f2160` shows a tooltip, does NOT write back percentage or set D50/FC0 |
| 8 | `scratch\ghidra_run.bat DecompAt.java 'C:\WRSRE\scratch\decomp_1407F22B0.c' 0x1407F22B0` | FAILED — "No function found containing address 0x1407F22B0". This is the tooltip writeback callback referenced at `decomp_1407F2160.c:47`; its implementation is unknown |

Pre-existing scratch files read (not re-decompiled):
- `rebuild_1E2380.c` — `FUN_1401E2380` re-plan routine (1001 lines)
- `slider_7EC380.c` — `FUN_1407EC380` percentage slider handler (2348+ lines)
- `slider_7EBFE0.c` — `FUN_1407EBFE0` slider evaluator (138 lines)
- `remove_162b40.c` — `FUN_140162b40` removal function (31 lines)
- `reachability_422c30.c` — `FUN_140422c30` reachability (742 lines)
- `upkeep_1c6050.c` — `FUN_1401C6050` road DO upkeep (536 lines)
- `planner_424430.c` — `FUN_140424430` planner (463 lines)
- `checkbox_check_6F49D0.c` — `FUN_1406F49D0` checkbox reader (54 lines)
- `decomp_1407496c0.c` — `FUN_1407496c0` DO interaction handler (1244 lines)
- `decomp_14008e050.c` — `FUN_14008E050` vector push_back (77 lines)
- `decomp_1401512a0.c` — `FUN_1401512a0` TaskCfg ctor (57 lines)
- `dow_toggle.c` — `FUN_140130650` target enable toggle (137 lines)
- `xrefs_140424430.txt` — planner xrefs (~50 call sites)
- `xrefs_bld_d38.txt` — DO+0xD38 xrefs (none found at file-based RVA)

Commands NOT yet executed (would need Ghidra):
- `scratch\ghidra_run.bat Xrefs.java out.txt 0x1401E2380` — who calls the re-plan routine
- `scratch\ghidra_run.bat DecompAt.java out.c 0x1401E2380` — already done as `rebuild_1E2380.c`
- `scratch\ghidra_run.bat DisasmMany.java out.asm 0x1407F22B0:0x1407F2300` — disassemble the writeback callback region
- `scratch\ghidra_run.bat Xrefs.java out.txt 0x1401C6050` — who calls road DO upkeep
- Search for writers to `DO+0xEA8` (offset from building base, not a fixed address)

---

## 4. Bounded Disassembly/Decompilation with RVAs and Object-Base Explanations

### 4.1 `FUN_1406EE2A0` (RVA 0x6EE2A0) — Generic Window/Popup Creator, NOT Percentage Increment

Signature: `undefined2 *FUN_1406ee2a0(longlong param_1, uint param_2, longlong param_3, int param_4, uint param_5, int param_6, int param_7, short *param_8, longlong param_9)`

- Allocates `operator_new(0x5D8)` window object (line 155)
- Calls `FUN_14025c500(pvVar14)` to initialize window (line 156)
- `param_2` selects window mode: 0,1,2,3,4,5,7,8,9,0x27,0x28 (lines 329-370)
- Called from `FUN_1407496c0` at `14074ba27` with `param_2=2, param_3=target_building, param_4=0` (`decomp_1407496c0.c:982`)
- **231 callers total** (`xrefs_1406EE2A0.txt`) — used throughout the UI system, not percentage-specific

### 4.2 `FUN_1407EC380` (RVA 0x7EC380) — Actual Percentage Slider Handler

Signature: `void FUN_1407ec380(ulonglong param_1, float param_2, float param_3, char *param_4, char param_5, longlong param_6, longlong *param_7, ...)`

**Key object bases:**
- `param_4` = pointer to `cfg+0x00` (load-all toggle byte) or `cfg+0xC8` (unload-all toggle byte)
- `param_4 + 4` = `cfg+0x04` (load %) or `cfg+0xCC` (unload %)
- `local_530 = (float *)(param_4 + 4)` (line 523) — pointer to percentage float
- `local_570 = (double)*local_530` (line 524) — current percentage loaded as double

**Percentage modification (click-triggered):**
- Line 314: `*param_4 = *param_4 == '\0';` — toggles the all-toggle byte (`cfg+0x00` or `cfg+0xC8`)
- Lines 532-535: `local_570 = local_570 + dVar8;` — increment with variable step (`DAT_14090b338` or `_DAT_14090b330`)
- Lines 536-538: `if (local_570 < DAT_14090a008) local_570 = 0.0;` — clamp min
- Lines 596-599: `local_570 = local_570 + DAT_14090a048; if (DAT_14090a1d8 < local_570) local_570 = dVar9;` — add 0.1, clamp max
- **Line 1814: `*local_530 = (float)local_570;`** — **writes modified percentage back to `cfg+0x04` or `cfg+0xCC`**

**Rendering-time clamping (no state change):**
- Line 558: `FUN_1407f2160(param_1,param_6,local_530)` — shows tooltip with percentage, does NOT modify memory

**Note:** `FUN_1407EC380` does NOT set `DO+0xD50` or `DO+0xFC0`. It only modifies config values.

### 4.3 `FUN_1407f2160` (RVA 0x7F2160) — Tooltip Display, Not Writeback

- Reads `*param_3` (percentage value), multiplies by `DAT_14090ab70` (=100.0 for display)
- Creates tooltip object at `param_1+0x13bf0`
- Sets callback pointer `&LAB_1407f22b0` at tooltip+0x78 (line 47)
- Stores `param_3` pointer at tooltip+0x70 (line 48)
- **Does NOT write back percentage, does NOT set D50/FC0**

### 4.4 `LAB_1407f22B0` (file-based `0x1407F22B0`) — Tooltip Writeback Callback

- Referenced as callback pointer at `decomp_1407F2160.c:47`
- Ghidra reports "No function found containing address 0x1407F22B0" — may be an inline label within `FUN_1407f2160` or adjacent code
- **UNEXAMINED** — may write back modified percentage and/or set D50/FC0

### 4.5 `FUN_1401E2380` (RVA 0x1E2380) — Re-Plan Routine

From `rebuild_1E2380.c`:

**Line 143:** `*(undefined1 *)(param_2 + 0xd50) = 0;` — **CLEARS DIRTY FLAG** at start of re-plan

**Line 144:** `*(undefined8 *)(param_2 + 0xd60) = *(undefined8 *)(param_2 + 0xd58);` — copies D58 to D60

**Lines 145-203:** Iterates over target entries (`bld+0xD38` to `bld+0xD40`):
- Line 149: checks `**(char **)(pcVar2 + 0x10) != '\0'` = `cfg+0x00` load-all toggle != 0
- Lines 152-194: inner loop matching load entries with unload entries, calls `FUN_140200280`
- **Lines 196-200:** If target building type is `0x14` (Customs):
  - `*(undefined4 *)(*(longlong *)(pcVar2 + 0x10) + 4) = 0;` → `cfg+0x04 = 0.0f`
  - `*(undefined4 *)(*(longlong *)(pcVar2 + 0x10) + 0xcc) = 0x3f7d70a4;` → `cfg+0xCC ≈ 0.99f`

**Lines 205-261:** Iterates over `param_1+0xc2b0` to `param_1+0xc2b8` (stride 0x340), calling `FUN_1400f9a00`

**Line 262:** `FUN_14019c750(param_1)` — serialize

**Lines 263-976:** Complex nested loops matching sources to destinations, building transport connections

### 4.6 `FUN_1401C6050` (RVA 0x1C6050) — Road DO Upkeep

From `upkeep_1c6050.c`:

**Lines 86-91:** FC0 countdown timer:
```c
fVar31 = *(float *)(param_2 + 0xfc0);
if (0.0 < fVar31) {
    fVar29 = C3D_TIMER::PowerTime(&DAT_1409D4EE0, DAT_14090A9FC, false, false);
    *(float *)(param_2 + 0xfc0) = fVar31 - fVar29;
}
```

**Lines 258-286:** Entry enabled-state sync:
```c
for each entry at bld+0xD38:
    cVar12 = (target reachable?) ? 1 : 0;
    if (**(char **)(entry_ptr) != cVar12) {
        *(undefined1 *)(param_2 + 0xd50) = 1;   // set dirty if changed
    }
    **(char **)(entry_ptr) = cVar12;             // write enabled byte
```

**Lines 451-527:** Re-plan gate:
```c
if ((0 < *(int *)(param_1 + 0x5a4)) &&                          // global gate (UNVERIFIED meaning)
    (*(float *)(param_2 + 0xfc0) <= 0.0) &&                      // timer expired
    (*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3 != 0)) {   // targets exist
    if (*(char *)(param_2 + 0xd50) != '\0') {                   // dirty?
        fVar29 = (float)FUN_1401E2380(param_1, param_2);         // RE-PLAN
    }
    *(undefined4 *)(param_2 + 0xfc0) = 0x40400000;               // reset timer to 3.0f
}
```

### 4.7 `FUN_140424430` (RVA 0x424430) — Delivery-Order Planner

From `planner_424430.c`:

- Clears order vectors at `game+0x144A8` (stride 0x68)
- Calls `FUN_140422c30` for reachability over `bld+0xA10` waypoints
- Iterates each target's storages (`bld+0x970`/`+0x978`, stride 0xE0)
- Checks resource type whitelist and storage slot mode
- Creates orders via `FUN_1402108A0`
- **~50+ unconditional call sites** (`xrefs_140424430.txt`)

**Line 89-91:** `param_2+0xea8` gate:
```c
if (param_5 == '\0') {
    if (*(char *)(local_180 + 0xea8) == '\0') goto LAB_14042469e;
}
```
- `param_5` = 5th char parameter of `FUN_140424430` (value depends on caller's stack setup)
- `local_180 = param_2` (DO building)
- If `param_5 == 0` AND `DO+0xEA8 == 0`, goes to alternative time-based check at LAB_14042469e
- If `param_5 != 0`, the `DO+0xEA8` check is skipped entirely

### 4.8 `FUN_140162b40` (RVA 0x162B40) — Target Removal, NOT Percentage Decrement

From `remove_162b40.c`:
```c
void FUN_140162b40(undefined8 param_1, longlong param_2, int param_3) {
    // removes entry at index param_3 from bld+0xD38 vector
    memmove(...);
    *(longlong *)(param_2 + 0xd40) -= 8;
    *(undefined1 *)(param_2 + 0xd50) = 1;      // set dirty
    *(undefined4 *)(param_2 + 0xfc0) = 0x40400000;  // set timer 3.0f
    FUN_1401c7da0();
    FUN_140162b00(uVar1);                       // free entry
}
```
Called by `-` button in `FUN_1407496c0` at line 1018. Confirmed: removal, not percentage decrement.

### 4.9 `FUN_1407496c0` (RVA 0x7496C0) — DO Interaction Handler

From `decomp_1407496c0.c`:

**Per-target row loop (lines 899-1187):**
- Checkbox state at `entry+0x30C` is actually `window+0x30C` (transient UI state) — NOT on TaskEntry
- `+` button calls `FUN_1406EE2A0(param_1, 2, lVar18, 0)` — window creator, not percentage increment
- `-` button calls `FUN_140162B40()` — removal

**Clamping pass (lines 1111-1174):** Runs for EVERY target row:
1. Reads `cfg+0x04` (load %) — `dVar45`
2. Clamps: `if (0.0 <= dVar45) { if (dVar8 < dVar45) dVar45 = dVar8; }` where `dVar8 = DAT_14090a048 = 0.1`
3. Writes back to `cfg+0x04`
4. Reads `cfg+0xCC` (unload %) — `local_dc0`
5. Clamps: `if (DAT_14090a048 <= local_dc0) { if (dVar9 < local_dc0) local_dc0 = dVar9; } else local_dc0 = DAT_14090a048;`
6. Writes back to `cfg+0xCC`
7. **Dirty flag:** Only sets `D50=1` if `entry+0x00` differs from widget state (NOT when percentage changes)
8. Sets `FC0=3.0f` only when D50 just became 1

### 4.10 `FUN_14008E050` (RVA 0x8E050) — Vector Push_Back

From `decomp_14008e050.c`:
```c
void FUN_14008E050(ulonglong *vector, undefined8 *element);
// vector[0]=begin, vector[1]=end, vector[2]=capacity
// Appends *element; grows via FUN_1400043D0 when end==capacity
```

### 4.11 `FUN_1401512a0` (RVA 0x1512A0) — TaskCfg Ctor

From `decomp_1401512a0.c`:
- Zeroes ptr fields: `+0x08,+0x10,+0x38..+0x188,+0x2C,+0xD0,+0xD8,+0xE0,+0x100..+0x188,+0xF4`
- Does NOT touch `+0x00,+0x04,+0xC8,+0xCC,+0x8C,+0x28,+0xF0`

---

## 5. Actual Constant Values and Percentage-Button Pseudocode

### 5.1 `DAT_14090A048` = 0.1

File-based address `0x14090A048`:
- Bytes at `0x14090A048`: `0x9A 0x99 0x99 0x99` (float view: -1.59e-23, int view: -1717986918)
- Bytes at `0x14090A04C`: `0x99 0x99 0xB9 0x3F` (float view: 1.45f)
- **As double (8 bytes `0x14090A048-0x14090A04F`):** bits `0x3FB999999999999A` = **0.1**

### 5.2 Related Constants (from `floats_90A048.txt` and decompiled code)

| Address | Bits | Float | Role |
|---|---|---|---|
| `0x14090A048` (double) | `0x3FB999999999999A` | **0.1** | Load % upper clamp, unload % lower clamp, slider increment step |
| `0x14090A008` | unknown | unknown | Load % minimum clamp (line 536-538 of `slider_7EC380.c`) |
| `0x14090A1D8` | unknown | unknown | Unload % maximum clamp (line 597 of `slider_7EC380.c`) |
| `0x140909F58` | unknown | unknown | Used in planner `FUN_140424430` and upkeep |
| `0x14090AB70` | unknown | unknown | =100.0 (percentage display multiplier) |

### 5.3 Native Percentage Click Pseudocode

```
// FUN_1407EC380 (slider handler), called during DO panel rendering
// param_4 = &cfg+0x00 (load-all toggle) or &cfg+0xC8 (unload-all toggle)
// param_4+4 = &cfg+0x04 (load %) or &cfg+0xCC (unload %)

// On click of increment/decrement region:
*param_4 = !*param_4;                          // line 314: toggle all-toggle byte
local_570 = *(double*)(param_4 + 4);           // line 524: read current percentage
local_570 += step;                             // lines 532-535: increment
if (local_570 < DAT_14090a008) local_570 = 0;  // lines 536-538: clamp min
local_570 += DAT_14090a048;                    // line 596: add 0.1
if (DAT_14090a1d8 < local_570) local_570 = max; // line 597-599: clamp max
*(float*)(param_4 + 4) = (float)local_570;     // line 1814: WRITE BACK
FUN_1407f2160(window, ..., &cfg+0x04);         // line 558: show tooltip (no memory write)

// D50/FC0: NOT set by FUN_1407EC380 or FUN_1407f2160
// D50/FC0: set by FUN_1407496c0 clamping pass ONLY when entry+0x00 changes
```

### 5.4 Native Clamping Pseudocode (FUN_1407496c0 lines 1151-1174)

```
// For EVERY target row, every frame the panel is open:
load_pct = *(float*)(cfg + 0x04);
if (load_pct >= 0.0) {
    if (0.1 < load_pct) load_pct = 0.1;   // DAT_14090A048 = 0.1 upper clamp
} else {
    load_pct = 0.0;
}
*(float*)(cfg + 0x04) = load_pct;

unload_pct = *(float*)(cfg + 0xCC);
if (0.1 <= unload_pct) {
    if (upper < unload_pct) unload_pct = upper;
} else {
    unload_pct = 0.1;   // DAT_14090A048 = 0.1 lower clamp
}
*(float*)(cfg + 0xCC) = unload_pct;
```

---

## 6. CONFIRMED / CONTRADICTED / UNVERIFIED Labels

| Finding | Label | Evidence |
|---|---|---|
| Native add-flow writes D50=1, FC0=3.0f | CONFIRMED | `decomp_1407496c0.c:604-605` |
| Native percentage loop clamps via DAT_14090A048 | CONFIRMED | `decomp_1407496c0.c:1151-1174` |
| DAT_14090A048 = 0.1 (as double) | CONFIRMED | `floats_90A048.txt` |
| FC0 is per-frame countdown timer | CONFIRMED | `upkeep_1c6050.c:86-91` |
| D50+FC0<=0 → FUN_1401E2380 re-plan → FC0=3.0f | CONFIRMED | `upkeep_1c6050.c:451-527` |
| Planner FUN_140424430 reads bld+0xD38 every frame | CONFIRMED | `planner_424430.c`, `xrefs_140424430.txt` |
| Our writer already sets D50=1, FC0=3.0f | CONFIRMED | `private_logistics.cpp:2806-2811` |
| "we forgot the dirty flag" explanation | CONTRADICTED | `private_logistics.cpp:2806-2811` |
| Native sets both toggles=1, both pct=1.0f | CONFIRMED | `decomp_1407496c0.c:593-603` |
| Entry+0x00 armed to 1 by dispatcher | CONFIRMED | `distribution_office_followup_report.md:63-67` |
| FUN_1406EE2A0 is percentage increment | CONTRADICTED | `decomp_1406EE2A0.c` — it's a window creator with 231 callers |
| entry+0x30C is on TaskEntry | CONTRADICTED | TaskEntry is 0x18 bytes; actual access is TaskCfg+0xC8 via entry+0x10+0xC8 |
| FUN_140162b40 is percentage decrement | CONTRADICTED | `remove_162b40.c` — it's removal, sets D50=1, FC0=3.0f |
| FUN_1407f2160 writes back percentage | CONTRADICTED | `decomp_1407F2160.c` — only shows tooltip |
| Our load % 0.20 clamped to 0.10 by native | LIKELY (not live-tested) | `decomp_1407496c0.c:1151-1174`, `floats_90A048.txt` |
| DAT_14090A048 exact value | CONFIRMED (0.1 as double) | `floats_90A048.txt` |
| FUN_1406EE2A0 internal writes | CONFIRMED (window creator) | `decomp_1406EE2A0.c` |
| FUN_1401E2380 clears D50=0 | CONFIRMED | `rebuild_1E2380.c:143` |
| FUN_1401E2380 modifies Customs percentages | CONFIRMED | `rebuild_1E2380.c:198-200` |
| Global gate param_1+0x5a4 meaning | UNVERIFIED | `upkeep_1c6050.c:451` — gate must be > 0 |
| DO+0xEA8 meaning/writers | UNVERIFIED | `planner_424430.c:91` — read by many, written by none found |
| LAB_1407f22B0 writeback callback | UNVERIFIED | Referenced at `decomp_1407F2160.c:47`, no function at address |
| Planner creates orders for our targets | UNVERIFIED | No live-game test performed |
| Reachability established for our targets | UNVERIFIED | `FUN_140422c30` not tested against our DO/targets |
| 0.10 load % sufficient for planner dispatch | UNVERIFIED | No live-game test performed |
| Repeated daily D50=1 postpones work | UNVERIFIED | Timer units: 3.0 seconds (countdown), not per-frame |

---

## 7. Complete Source of New Query Scripts Used

### PrintFloats.java (copied to Ghidra tools directory)

```java
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * PrintFloats - read float/int values at absolute file-based addresses.
 *
 * Usage:  PrintFloats.java <out_file> <addr1_hex> [addr2 ...]
 * For each address prints: float view, int view, and first 2 raw words.
 * Addresses are FILE-BASED (project base 0x140000000).
 */
public class PrintFloats extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) {
            println("Usage: PrintFloats.java <out_file> <addr1> [addr2 ...]");
            return;
        }
        Memory mem = currentProgram.getMemory();
        try (PrintWriter w = new PrintWriter(new FileWriter(a[0]))) {
            for (int i = 1; i < a.length; i++) {
                long v = Long.parseLong(a[i].trim().toLowerCase().replace("0x", ""), 16);
                Address addr = currentProgram.getAddressFactory()
                    .getDefaultAddressSpace().getAddress(v);
                try {
                    int bits = mem.getInt(addr);
                    float f = Float.intBitsToFloat(bits);
                    w.printf("0x%X : bits=0x%08X  float=%g  int=%d%n",
                        v, bits, f, bits);
                } catch (MemoryAccessException e) {
                    w.printf("0x%X : <unreadable>%n", v);
                }
            }
        }
        println("PrintFloats: written to " + a[0]);
    }
}
```

Existing scripts used (no modifications): `DecompAt.java`, `Xrefs.java`.

---

## 8. Proposed Integration — Explicit Reasons NOT to Implement

**No C++ integration is recommended at this time.** The evidence does not justify a speculative patch.

### Reasons:

1. **Dirty flag/timer already implemented.** Our writer at `private_logistics.cpp:2806-2811` already sets `DO+0xD50=1` and `DO+0xFC0=3.0f`. The previous report's "missing dirty flag" diagnosis is contradicted by the source.

2. **The likely issue is native clamping, not missing invalidation.** `DAT_14090A048 = 0.1` clamps our load % from 0.20 to 0.10 whenever the panel renders. This is a data issue, not an invalidation issue. Setting D50 more frequently won't help if the clamped value (0.10) is insufficient for the planner.

3. **Unverified root cause.** Multiple unexamined variables could explain the bug:
   - `LAB_1407f22B0` writeback callback (may set D50/FC0 after tooltip dismissal)
   - Global gate `param_1+0x5a4` (may block upkeep re-plan if 0)
   - `DO+0xEA8` flag (may gate planner processing)
   - Reachability between DO and targets
   - Whether 0.10 load % is sufficient for dispatch

4. **No live-game testing performed.** The investigation is purely static. Without observing actual game behavior (vehicle movement, order creation, D50/FC0 values in memory), any patch would be speculative.

5. **Previous "fix" (Option B) already matches production.** The direction configuration (source-only: load-all=1, load=0.20; destination-only: unload-all=1, unload=0.30) is already implemented in `site_permissions.h` and `private_logistics.cpp`. Changing it would not address the clamping issue.

### If a fix is later deemed necessary, the minimal safe approach would be:

**Option A: Verify first, then act**
Add debug-only logging in `AutoTargetsOnDay` after the D50/FC0 write, confirming the values are set on the live DO pointer. Also log `entry+0x00`, `cfg+0x00`, `cfg+0x04`, `cfg+0xC8`, `cfg+0xCC` for each managed target. Compare against native-created targets. No memory writes, no hook changes.

**Option B: Address clamping if confirmed**
If logs show native clamping reduces load % to 0.10 and this is below the planner's dispatch threshold, consider raising the source threshold above 10% (e.g., 15-20%) so that after clamping to 0.10, the effective value is still usable. This requires:
- `ReadablePtr` checks on all game memory
- Call only from `sim_clock` registerTick (game simulation thread)
- Set D50=1 once per reconciliation cycle, not per target
- Do not call `FUN_1407496C0`, `FUN_1406EE2A0`, or any UI handler
- Preserve state permissions (load-only vs unload-only)
- Respect the 6-target road cap

### Required verification before any implementation:

1. Run `Xrefs` on `FUN_1401E2380` to confirm all callers
2. Disassemble `0x1407F22B0` region to understand the tooltip writeback callback
3. Determine the value and writers of `DO+0xEA8`
4. Determine the meaning of global gate `param_1+0x5a4`
5. Live-game observation: check if vehicles move with current config, and if D50/FC0 are actually set on the live DO pointer

---

## 9. Minimal Gameplay Validation Plan

If and when implementation is authorized:

1. **Baseline:** Start game with private_logistics enabled, billing OFF, automatic targets ON, source 20%, destination 30%. Confirm targets appear with correct percentages (already verified in gameplay log).

2. **D50/FC0 verification:** Use in-game probe or memory observation to confirm `DO+0xD50=1` and `DO+0xFC0=3.0f` are set after daily reconciliation.

3. **Clamping verification:** Open the DO panel, observe if `cfg+0x04` changes from 0.20 to 0.10 (native clamp). Close panel, wait for next reconciliation, observe if our writer rewrites to 0.20.

4. **Vehicle movement:** Wait for autonomous pickup/delivery with eligible stock, demand, and usable route. Record whether vehicles move WITHOUT opening the DO panel or clicking any percentage.

5. **Acceptance criterion:** Repeated autonomous pickup/delivery cycles without any UI interaction. Correct displayed settings alone is NOT sufficient.

6. **If vehicles don't move:** Check `FUN_140424430` planner output (orders at `game+0x144A8`), verify reachability via `FUN_140422c30` (`game+0x14478`), and confirm `param_1+0x5a4` global gate state.
