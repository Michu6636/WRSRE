# Remaining Logic Bugs Audit — 2026-10-22 (read-only, no in-game test)

**Branch:** `arena/3e9728cf-wrsre` at `7fb0a60` (merge PR #12 — warehouse `hasNative` fix)  
**Scope:** full `plugins/*/*.cpp` static review (~22k LOC). No exe run; `ReadablePtr`/`TsmIsValidGamePtr` guards assumed correct.  
**Method:** grep for `divide|/0`, `EnterCriticalSection`, `isBuildingPrivate`, `AutoAppendNativeTarget`, `wholesaleRatio`, `getDaysPerMonth`, `stolen bytes`, manual read of hot paths.  
**Result:** 1 CRITICAL (DoS via INF store), 5 HIGH (silent economy break), 7 MEDIUM (edge/bypass), 4 LOW. Warehouse checkbox itself is **fixed** (`!priv && hasNative → willDraw=1`, `state_storage_always_on=0` deprecated).

---

## CRITICAL — crash / game-memory corruption

### C1 — `private_shops` wholesale affordable path divides by zero when `wholesale_ratio=0`
- **Files:** `plugins/private_shops/private_shops.cpp:909` + `1639`
- **Code:**
  ```cpp
  if (f >= 0.0f && f <= 5.0f) g_wholesaleRatio = f; // 0 allowed
  // ...
  float affordableTons = (float)(affordable / ((double)perTon * (double)g_wholesaleRatio));
  *(float*)(slotBytes + TSM_SLOT_CONTENT) = snap.content + affordableTons; // INF
  ```
- **Why:** ini `wholesale_ratio` is documented 0.5–1.0 but guard accepts `0.0`. With `allowDebt=0` and capital < wholesale, the `else if (!allowDebt)` branch divides by `perTon * 0` → `INF`, writes `INF` to `TSM_SLOT_CONTENT`. Game later reads that slot for dispatch/planner → NaN/infinite tonnage, likely AV or economy blowup.
- **Repro:** set `private_shops.ini wholesale_ratio=0`, run a private shop with `capital 1 RUB`, trigger wholesale billing (daily close after shop sold). Check `tesmioloader.log` for `inf` or `slot` corruption; game may freeze on save.
- **Fix:** clamp `g_wholesaleRatio` to `>=0.01f` on load (or `if (g_wholesaleRatio < 0.01f) g_wholesaleRatio=0.01f;`) and guard division `if (perTon * g_wholesaleRatio < 1e-6) return;`. Same risk at `private_shops.cpp:897` (`wholesale = delta*perTon*ratio` is safe, but affordable path not).
- **Priority:** **P0** — one ini typo corrupts world.

---

## HIGH — silent economy / persistence breaks

### H1 — `private_logistics` state-storage key collision (quantized position)
- **File:** `plugins/private_logistics/private_logistics.cpp:2038-2055` `AutoBuildingKey`
- **Code:** `qx = (int)(x*10 + 0.5)`, `qz` same, `h = FNV(objectName, type, qx, qz)`
- **Why:** 0.1m grid. Two warehouses with same `objectName` (e.g. two `warehouse_covered_combined` cloned) within 0.1m collide → same `key`, permissions alias. User enables one checkbox, both get `LOAD-ON`. Uncommon but possible on dense saves.
- **Fix:** include raw `building` pointer high bits or full `float` bits, or at least `1.0f` quantization. Or use `b+0x320` node pointer as salt. Low risk, keep as **P1** diagnostic: log on collision `key already present with different building`.

### H2 — `private_logistics` disabled-but-present dedup hides re-enable
- **File:** `plugins/private_logistics/private_logistics.cpp:2646-2665`
- **Code:** `if (target.targetPresent) { ++existing; continue; }` — `targetPresent` is set only from *live* `entry` scan (`entry.targetPresent=1`). Native upkeep *disables* unreachable entries in place (entry stays, flag `entry[0]=0`) — they remain `targetPresent=1`, so `existing` counts them and no re-add is planned. State-storage permission change (`AutoSetStateTargetTaskMode`) only toggles `cfg+0xC8/0xCC` if `permission !=0`; it never re-enables a disabled entry (`entry[0]=0` stays). A warehouse disabled for reachability then re-enabled via checkbox may stay disabled forever.
- **Repro:** checkbox OFF → DO target removed (via `hasExplicitStateOff` path), then ON while DO still unreachable (road disconnected). Native disables entry; user reconnects road, checkbox stays ON — expected re-enable never happens because `existing` hit.
- **Fix:** when `target.isStateStorage && permission==1`, treat disabled entry as *not* present for dedup, or explicitly set `*(BYTE*)entry=1` in `AutoSetStateTargetTaskMode` when `permission==1`.

### H3 — `enterprise_base` wage cap uses stale `g_enterprise` without null check in tick
- **File:** `plugins/enterprise_base/enterprise_base.cpp:2833-2845`
- **Code:** `if (g_wageCapEnabled && g_labor && TsmLaborHasV3(g_labor) && g_simClock)` then `GetWageForTypeImpl` → `ResolveWageForBuildingImpl` → `g_labor->getEffectiveWageForBuilding` without re-checking `g_labor` null after `EnterCriticalSection` (lock released, another thread could unload `citizen_labor`).
- **Why:** `citizen_labor` can be `H->consume` null on failed init, but `g_labor` is global not atomically updated. Race is theoretical (plugins init serially) but wage cap runs on sim_clock tick which may fire during unload.
- **Fix:** snapshot `g_labor` to local `auto* labor = g_labor; if (!labor) return;` before lock.

### H4 — `citizen_labor` wage mobility hook AllocNear failure silently disables wage bidding
- **File:** `plugins/citizen_labor/citizen_labor.cpp:2267` `AllocNear(g_exeBase + TSM_HOOK_JBE_RVA, ...)`
- **Code:** `if (!g_laborMobilityStub) { Logf("hook FAILED ..."); return false; }` — failure is logged but `enterprise_base` still advertises `private_market_wages` as active. Wages never move, premium cap never bites, log shows `WAGE CAP 0` forever.
- **Why:** ASLR + 2GB AllocNear limit on large exe + plugins; rare but user sees no error besides one log line.
- **Fix:** surface to `enterprise_base` via API (`TsmLaborMobilityActive()`), or fail `private_market_wages` startup if hook fails.

### H5 — `taxes` poverty floor uses stale CPI when `resource_pricing` disabled
- **File:** `plugins/taxes/taxes.cpp:240-250` `DailyToMonthly` + `SnapshotMacroStats`
- **Code:** `cpi = (cpi == cpi && cpi>0 ? cpi : 0)` → `floor = 7 * 0 =0` → `belowFloor` always 0. Poverty rate reported 0% even with destitute wallets if `resource_pricing` fails to start (then `g_pricing=null`).
- **Why:** `taxes` depends on `resource_pricing` for CPI but does not gate poverty calc. Save with `resource_pricing disabled` hides poverty.
- **Fix:** if `cpi==0` mark `MacroStats.cpi=NaN` and skip poverty calc, log `[TAXES] CPI unavailable — poverty rate n/a`.

---

## MEDIUM — edge / bypass / persistence

### M1 — `private_logistics` `state_storage_always_on` still honored (deprecated bypass)
- **File:** `plugins/private_logistics/private_logistics.cpp:2613` + `private_logistics.ini:210`
- **Status:** Fixed doc, but code still `forceOn = g_stateStorageForceOn !=0` → counts as `candidateCount` and logs `force-enabled`. User toggling checkbox to OFF then setting ini `1` silently overrides UI. Keep for debug but add `Logf WARN deprecated` when `forceOn` and `permission==0`.

### M2 — `private_logistics` `AutoHasNativeStorage` stride check allows 4096 storages (64KiB read)
- **File:** `plugins/private_logistics/private_logistics.cpp:2182` `count <=4096 && ReadablePtr(begin, count*0xE0)`
- **Why:** A corrupted save with `end-begin = 0xE0*4096 = 0xE0000` (917KB) is considered readable and iterated. Probe will walk 4096 blocks per building, 512 buildings → 2M iterations per day, tick stall.
- **Fix:** cap to game-realistic `count <= 32` (max storages per building seen: factory 3, shop 2, warehouse 1) and log `[AUTO DO] storage count 4096 unrealistic — truncated`.

### M3 — `persistence` `SaveDirOf` “save” substring false positive
- **File:** `plugins/persistence/persistence.cpp:74-95`
- **Code:** `if (_strnicmp(out + sepPos -4, "save",4)!=0) return false;` — matches any folder ending in `save`, e.g. `autosave_backup`. Dispatch fires on wrong `fopen` (e.g. `stats.ini` in `autosave_backup/stats.ini`).
- **Fix:** require `save` is a path component: check `sepPos>=5` and `out[sepPos-5]=='\\' or '/'` and `sepPos-4` is start of component (already checks `before` but only for dir, not full path). Edge, low.

### M4 — `persistence` debounce uses `GetTickCount` wrap-safe but 3s window may drop rapid save/load cycles
- **File:** `plugins/persistence/persistence.cpp:185-195` `if ((int)(now - g_lastLoadTick) <3000 && same dir) skip`
- **Why:** Quick save → load → save within 3s (e.g. user spams F5) second load is debounced, permissions not reloaded. Native loader *does* reconstruct `TaskEntry` objects (noted in `AutoTargetsOnDay` `afterLoad` path) — missing dispatch means stale memo tombstones.
- **Fix:** debounce only duplicate *loads* for same tick, not saves. Or store `lastEvent` type.

### M5 — `enterprise_base` park-cap uses unreadable capacity fallback 0.50 (silent)
- **File:** `plugins/enterprise_base/enterprise_base.cpp:1573` `cap = -1.0f` → if unreadable → `cap=-1` → not used, but later `1939` keeps `0.50` default. User never sees log.
- **Impact:** tiny — park rent slightly off. Add `Logf once` on unreadable.

### M6 — `private_construction` 39 LOC — no private-tag check before spend?
- **File:** `plugins/private_construction/private_construction.cpp:28-39` — only `g_enabled` guard, no `isBuildingPrivate` gate in snippet. If `enterprise_base` is missing, it may spend treasury without enterprise. Check `TsmPluginInit` returns 1 if `g_enterprise==null`?
- **Fix:** already returns 1 on missing `enterprise_base` (line 1711 in private_shops, similar pattern). Verify same in `private_construction.cpp:34`.

### M7 — `ui_panels` stolen-bytes mismatch stale comment
- **File:** `plugins/ui_panels/ui_panels.cpp:1004` `kStorageExpect` comment says “22 bytes” but `plugins/private_logistics` still references “21-byte prologue” in older docs. Now fixed in `ui_panels` (v6.15), but `private_logistics.cpp:134` comment still says “21-byte”. Keep consistent.

---

## LOW — diagnostics / style

- **L1** `citizen_retail` checkout hook stub 256-byte limit: `if (stub >256) Logf FAILED` — correct, but stub size is 18+`push` etc. ~40 bytes, never hits. Keep.
- **L2** `sim_clock` `TsmPluginStart` registers `OnTick` before `g_simClock` validated — `ReadSettings` already checks `g_simClock` presence, so safe.
- **L3** `currency` `GO()` returns `g_exeBase + RVA_GAME_OBJECT` without null check — caller checks `TsmIsValidGamePtr`.
- **L4** `resource_pricing` `getPrice` divides by `usd_divisor` with `>0` guard (`270: d = (divisor>0?divisor:50)`) — safe.

---

## What is *not* a bug (already fixed)

- **Warehouse checkbox:** `StateStorageWidgetProvider` + `AutoProbeStateInfrastructure` now both `!priv && hasNative` (commit `f7f1372`). Shop-path panel `type=3` draws checkbox; logs `willDraw=1`. Verified 22B hook `40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00` at `0x4CABD0`, dispatcher `CMP ECX,5 → 0x4CABD0`, `b+0x970/0x978` stride `0xE0` (Ghidra Blocks A-D). `state_storage_always_on=0` is correct.
- **Stolen-bytes safety:** cookie at offset 29 (`0x970..0x978` readers) preserved, `sizeof kStorageExpect=22`.
- **Persistence atomic write:** `MoveFileEx|WRITE_THROUGH` + short-write delete — correct.

## Recommended fix order (no game test)

1. **C1** wholesale_ratio guard — 1-line, prevents world corruption.
2. **H2** disabled-entry re-enable — 5-line, fixes warehouse checkbox toggle.
3. **H5/M2** caps (CPI, storage count) — 2-line each.
4. **M1** deprecated ini warn — 1-line.
5. **M4** debounce event-type — 3-line.

All fixes are static, no `git mv`, LF UTF-8, push only to `arena/*` (current branch `arena/3e9728cf-wrsre` at `7fb0a60`).
