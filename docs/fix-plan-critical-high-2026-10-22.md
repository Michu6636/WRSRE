# Fix Plan — Critical + High Bugs (2026-10-22)

**Branch:** `arena/3e9728cf-wrsre` at `7fb0a60` → `f0bbc7e`  
**Source audit:** `docs/remaining-logic-bugs-audit-2026-10-22.md`  
**Goal:** fix C1 (CRITICAL) + H1-H5 (HIGH) without in-game test, preserve LF UTF-8 no BOM, `git mv` history, push only to `arena/*`.

---

## 0. Verification baseline

- Warehouse checkbox already fixed (PR #12): `StateStorageWidgetProvider` + `AutoProbeStateInfrastructure` both `!priv && hasNative` ( `b+0x970/0x978` stride `0xE0`), `state_storage_always_on=0` deprecated, 22B `40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00` at `0x4CABD0`. No regression expected.
- New changes must not alter hook RVAs, stolen-byte sizes, or persistence file magic `PLSTOR17`.
- All edits will be compile-guarded by `ReadablePtr`/`TsmIsValidGamePtr` where new reads added.

---

## 1. C1 — private_shops wholesale_ratio=0 divide-by-zero

**Location:** `plugins/private_shops/private_shops.cpp:1639` (ini load) + `909` (affordable path)

**Current:**
```cpp
if (f >= 0.0f && f <= 5.0f) g_wholesaleRatio = f; // 0 allowed
// ...
float affordableTons = (float)(affordable / ((double)perTon * (double)g_wholesaleRatio));
```

**Root cause:** `g_wholesaleRatio==0` → denominator 0 → INF tons written to `*(float*)(slotBytes+0x8C)` → game reads INF later → AV/economy blowup. `wholesale <=0` early-return at `897` is safe for full-price path, but `else if (!allowDebt)` affordable path has no guard.

**Plan:**
1. In `ReadSettings()` after `configString("wholesale_ratio")` block, clamp:
   ```cpp
   if (f >= 0.0f && f <= 5.0f) g_wholesaleRatio = f;
   if (g_wholesaleRatio < 0.01f) {
       Logf("private_shops  wholesale_ratio %.3f < 0.01 — clamped to 0.01", (double)g_wholesaleRatio);
       g_wholesaleRatio = 0.01f;
   }
   ```
   Keep upper bound 5.0 as before, floor 0.01 prevents INF and keeps 1% minimum billing (still meaningful).

2. Defensive guard at call site `WholesaleSlot` line `908-912`:
   ```cpp
   double denom = (double)perTon * (double)g_wholesaleRatio;
   if (!(denom > 1e-6)) { snap.content = content; return; } // perTon already >0, but ratio could be tiny
   float affordableTons = (float)(affordable / denom);
   if (!(affordableTons == affordableTons) || !(affordableTons >= 0.0f) || affordableTons > 1e6f) { snap.content = content; return; }
   ```
   Add `#include <cmath>` if not present (already via std headers).

3. Same guard for `private_shops.cpp:897` full wholesale path is already `if (wholesale <=0) return` — safe, no change.

4. Test expectation (log review, no exe): set ini `wholesale_ratio=0` → log must show `clamped to 0.01`, no `INF` slot, day close `paid ... for state delivery` with finite tons.

**Risk:** none — stricter floor, log added, no hook change.

---

## 2. H1 — AutoBuildingKey quantized position collision

**Location:** `plugins/private_logistics/private_logistics.cpp:2032-2050` `AutoBuildingKey`

**Current:**
```cpp
int32_t qx = (int32_t)(x*10 + (x>=0?0.5:-0.5));
int32_t qz = (int32_t)(z*10 + (z>=0?0.5:-0.5));
uint32_t values[3] = { (uint32_t)type, (uint32_t)qx, (uint32_t)qz };
h = FNV(objectName, values...)
```

**Root cause:** 0.1m grid, two `warehouse_covered_combined` clones within 0.1m → same `key`, permission alias. Uncommon but dense saves possible.

**Plan:**
1. Strengthen hash without changing persisted file format (keys are already in `PLSTOR17`):
   - Keep existing qx/qz (backward compat), add second mixing stage with full float bits for collision resistance:
   ```cpp
   // Existing qx/qz path unchanged for compat, then mix raw float bits as extra
   uint32_t fx, fz;
   memcpy(&fx, &x, sizeof(fx));
   memcpy(&fz, &z, sizeof(fz));
   h = AutoHashMix(h, (uint8_t)(fx & 0xFF)); h = AutoHashMix(h, (uint8_t)((fx>>8)&0xFF)); ... // or reuse AutoHashMix over bytes of fx/fz
   // Simpler: hash 8 bytes of fx/fz directly
   const uint8_t* fxBytes = (const uint8_t*)&fx; for (int i=0;i<4;i++) h = AutoHashMix(h, fxBytes[i]);
   const uint8_t* fzBytes = (const uint8_t*)&fz; for (int i=0;i<4;i++) h = AutoHashMix(h, fzBytes[i]);
   ```
   This preserves old keys for existing saves? Actually old keys computed without fx/fz will differ from new keys → existing `PLSTOR17` files would miss. **Must not break persistence.**

   So alternative that is *backward compatible*: keep old key computation exactly, but add *secondary* disambiguation when collision detected at permission-set time:
   - In `AutoStatePermissionSet`, when `g_statePermissions[i].key == key` found, verify building identity via `AutoBuildingName` + position distance <0.1m; if mismatch, treat as collision and allocate new slot with `key ^ (fx ^ fz)` salt.

   **Chosen approach:** keep `AutoBuildingKey` output unchanged for compat, but add collision log + salt when `AutoStatePermissionSet` sees duplicate key with different building:
   - Change `AutoStatePermissionSet` signature not needed; instead make `AutoBuildingKey` also incorporate `building` pointer low bits *only* for new saves if we bump file magic. Simpler: bump magic `PLSTOR17 → PLSTOR18` and re-hash with full float bits, and on load handle both magics (old 17 = old hash, new 18 = new hash). This preserves backward load but fixes future.

2. Implementation steps:
   - Create `AutoBuildingKeyV2` that does old FNV + extra fx/fz mix.
   - Keep `AutoBuildingKey` as alias to V2 for new code.
   - In `AutoPersistenceEvent` write path: change magic to `PLSTOR18`, bump version check on read to accept both `PLSTOR17` (old) and `PLSTOR18` (new), with `count <= MAX` and `len == 12+count*9` for both.
   - On load of `PLSTOR17`, migrate entries by re-hashing with V2? No building pointer available at load time (no game memory), so keep old keys as-is; they will be remapped on next widget toggle which recomputes V2 key — old permission will appear as OFF-DEFAULT until user re-toggles. Log `migrated 17→18`.

3. Minimal alternative (chosen for this PR, no magic bump): just add raw float bits to hash *in addition* to qx/qz, accept that old saves will see permission reset once (one-time). Log it. Since `PLSTOR17` file contains old keys, after code update the lookup `AutoStatePermissionGet(oldKey)` will miss (new key != old) → `OFF-DEFAULT`. User must re-toggle once. Acceptable vs keeping collision bug.

   **Decision:** implement minimal: `fx/fz` added to hash, keep magic `PLSTOR17` (no bump), document one-time reset in commit message.

**Risk:** one-time permission reset for existing saves with colliding or any warehouse permissions. Mitigated by log.

---

## 3. H2 — disabled-but-present dedup hides re-enable

**Location:** `plugins/private_logistics/private_logistics.cpp:2646-2665` (first loop), `2680-2730` (state loop), `AutoSetStateTargetTaskMode:2520`

**Current:** `targetPresent` set from live entries regardless of enabled flag. Disabled entries (native upkeep cleared `entry[0]=0` for unreachable) remain `targetPresent=1` → `existing` path, never re-added. `AutoSetStateTargetTaskMode` only toggles `cfg+0xC8/0xCC` (load/unload) if `permission !=0`, never touches `entry[0]` enable flag.

**Plan:**
1. Expand `AutoSetStateTargetTaskMode` to also fix `entry[0]`:
   ```cpp
   // In AutoSetStateTargetTaskMode(void* entry, unsigned char perm):
   // perm 1 = LOAD-ON, 2 = OFF-EXPLICIT, 0 = OFF-DEFAULT
   bool changed = false;
   if (perm == 1) {
       if (*(BYTE*)entry == 0) { *(BYTE*)entry = 1; changed = true; } // re-enable disabled
       // existing cfg toggles for unload already
   } else if (perm == 2) {
       if (*(BYTE*)entry != 0) { /* optional: keep disabled entry but mark off? leave as is */ }
       // ensure unload disabled? Actually OFF-EXPLICIT should not be in target list at all — removal handled elsewhere
   }
   // keep existing cfg+0xC8/0xCC logic, but also ensure `cfg+0x28` active flag =1 when perm==1
   if (perm == 1 && *((BYTE*)cfg + 0x28) == 0) { *((BYTE*)cfg + 0x28)=1; changed=true; }
   ```

2. In `AutoTargetsOnDay` first scan, when `target.isStateStorage && permission==1` and `*(BYTE*)entry==0` (disabled), treat as *not* present for dedup OR at least log:
   ```cpp
   if (target.isStateStorage && target.statePermission==1) {
       // allow re-enable through AutoSetStateTargetTaskMode, but don't count as planned
       // doConfigChanged already handles it — ensure we don't also count as existing for budget
       // So keep targetPresent=1 but doConfigChanged will re-enable
   }
   ```
   Actually current `doConfigChanged |= AutoSetStateTargetTaskMode(entry, perm)` already called for every live entry where `perm!=0`. So if we make `AutoSetStateTargetTaskMode` re-enable, the disabled entry will be fixed in place — no need to change dedup counting.

3. Add diagnostic: when `permission==1` and `entry[0]==0` log `re-enabled disabled target key=... after reachability restore`.

**Risk:** touching `entry[0]` could conflict with native upkeep's own disable; but we only set 0→1 when user explicitly enabled checkbox, which is intended.

---

## 4. H3 — enterprise_base wage cap race on g_labor

**Location:** `plugins/enterprise_base/enterprise_base.cpp:2833-2845`

**Current:** `if (g_wageCapEnabled && g_labor && TsmLaborHasV3(g_labor) && g_simClock)` then later `capDaily = g_labor->getEffectiveWageForBuilding(workplace);` without re-check.

**Plan:**
```cpp
auto* labor = g_labor; // snapshot
auto* clock = g_simClock;
if (g_wageCapEnabled && labor && TsmLaborHasV3(labor) && clock) {
    capDaily = labor->getEffectiveWageForBuilding(workplace);
    capMin = labor->getMinimumWage();
    if (clock->structSize >= offsetof(...)) { int d = clock->getDaysPerMonth(); if (d>0 && d<=400) capDpm=d; }
```
Snapshot both globals before lock, use locals throughout. No lock needed (init serial, but snapshot prevents torn read).

---

## 5. H4 — citizen_labor wage mobility hook AllocNear failure silent

**Location:** `plugins/citizen_labor/citizen_labor.cpp:2267-2285`

**Current:** logs FAILED, returns false, no API to query. `enterprise_base` continues to advertise `private_market_wages` as active.

**Plan:**
1. Add new API flag `TsmLaborMobilityActive` (or simple global `g_wageMobilityActive` exposed via `TsmLaborApi` struct — check existing `src/citizen_labor_api.h` for version field, add `int (*isMobilityHookActive)(void);`).

2. For minimal change without API version bump, add log + config fallback:
   - In `InstallLaborMobilityHook` failure, set `g_wageMobilityFailed=1`.
   - In `LaborPanelRows`/`LaborPanelWidgets`, if `g_wageMobilityFailed` show row `Wage Mobility: HOOK FAILED — using INI wages` and disable slider's dynamic effect.
   - In `enterprise_base` wage bidding, check `g_labor->isMobilityHookActive` if available else assume active.

   **Chosen minimal:** add `static int g_mobilityHookOk = 0;` set `1` on success, `0` on failure. Expose via `TsmLaborApi` new function pointer if struct size allows, else just log and make `enterprise_base` check `TsmLaborHasV3` still true but add probe row: `if (!g_mobilityHookOk) Logf("private_market_wages disabled — mobility hook not installed");`

3. Easiest: modify `enterprise_base.cpp` to detect: if `g_labor` present but `g_labor->getEffectiveWageForBuilding` returns same as `getMinimumWage` for all types (indicates mobility not active), log warn. But better to just add global and check.

Implementation: add `static bool g_wageMobilityHookInstalled = false;` in citizen_labor, set true on success, expose via new exported function `TsmLaborMobilityHookInstalled()` (add to API with version guard). In enterprise_base, consume that function if present (`if (g_labor->structSize >= offsetof(..., isMobilityHookInstalled) + sizeof(ptr) )` ).

**Risk:** API version bump needs coordination — do minimal log-only if struct too tight. Fallback: just improve log: `Logf("citizen_labor  WAGE MOBILITY hook FAILED — private_market_wages will use static INI wages, not dynamic bidding");` and in enterprise_base log similar.

---

## 6. H5 — taxes poverty floor stale CPI

**Location:** `plugins/taxes/taxes.cpp:240-260` `SnapshotMacroStats`

**Current:** `cpi = (res_pricing? getCpiDailyBasket() : 0)` → `floor = 7*0=0` → `belowFloor` always 0 → Poverty Rate 0% even if wallets empty.

**Plan:**
1. Add guard:
   ```cpp
   float cpi = g_pricing ? g_pricing->getCpiDailyBasket() : 0.0f;
   if (!(cpi > 0.0f)) {
       out->cpi = 0.0f;
       out->belowFloor = 0;
       out->avgWallet = ... // still compute
       // mark poverty unavailable
       static bool logged = false; if (!logged) { Logf("taxes  CPI unavailable (resource_pricing disabled) — poverty rate n/a"); logged=true; }
       return; // or set out->count etc. but skip belowFloor calc
   }
   const float floor = g_povertyRunwayDays * cpi;
   // existing loop
   ```

2. In panel rows `TaxPanelRows`, if `cpi==0` show `Poverty Rate: n/a (no CPI)` instead of `0.0%`.

---

## 7. Implementation order & testing

1. **C1** (1 file, 2 hunks) — highest impact, no deps.
2. **H3** (1 file, 1 hunk) — snapshot locals.
3. **H5** (1 file, 2 hunks) — CPI guard + panel.
4. **H2** (1 file, 2 hunks) — entry re-enable.
5. **H1** (1 file, 3 hunks) — key hash, magic note.
6. **H4** (2 files, 3 hunks) — mobility flag + enterprise_base check.

Each commit will be pushed to `arena/3e9728cf-wrsre` (current active). No `git mv`, LF preserved. Verification via `grep` and syntax check (`g++ -fsyntax-only` where possible, but windows.h limits — manual review).

