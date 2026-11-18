# Old vs New Analysis — Why the “working” build stopped detecting

**Date:** 2026-10-22  
**Branch:** `arena/900b469c-wrsre` (6dc57dd) vs uploaded “older version”  
**Files compared:** `ui_panels.cpp` 2822→3001 lines (+179), `private_logistics.cpp` 1112→3588 lines (+2476), `.ini` diffs below  
**Log:** `/tmp/old_tesmioloader.log` 208 199 lines (v0.17) vs current 1 949 lines — both show same state after our stub fix.

## Summary

The “older version” you uploaded is **v0.3-era private_logistics + pre-6.11 ui_panels**. It never had the storage checkbox or auto-targets — DO “detection” was **vanilla manual** (player presses *Add station* in the DO window, game lists buildings reachable via road). The current broken build is **v0.17 + ui_panels 6.11** with:

- `ui_panels` 7→8 hooks (`HOOK_STORAGE` 7, `RVA_STORAGE_BUILDER 0x4CABD0`, `RVA_STORAGE_CALLSITE 0x6EA80C`, `MAX_HOOKS 8`, default `enable=...,do,storage`)
- `private_logistics` v0.3→v0.17: trip tariff (`litres_per_trip 14.75L`), `trip_tariff=1`, billing LIVE, **plus `auto_targets_enabled=1` `dry_run=1`** state-storage discovery.

Both old and new logs after our `3.5.2-STUB` fix show **identical** infra probe:

```
world=29 state_storage=1 (warehouse_covered_combined -5758,5325) permission=OFF-DEFAULT
private offices=1 candidates=2 (fabric_factory type6 + Avryu Small store type3) planned=2 added=0 existing=0 mode=DRY RUN
```

That is **not a crash** — it is the *designed* dry-run behaviour: log what *would* be added, never call `AutoAppendNativeTarget`. The DO’s vanilla list being empty (`existing=0`) is therefore unrelated to our code — either the save’s road graph has no reachable candidates, or you expected automation to fill it while dry_run is on.

## What actually changed

### ui_panels.cpp 2822→3001

| Area | Old (working) | New (broken report) |
|------|---------------|---------------------|
| `MAX_HOOKS` | 7 (`HOOK_DO` last) | 8 (+ `HOOK_STORAGE`) |
| `RVA_STORAGE_*` | absent | `0x4CABD0` + `0x6EA80C` |
| `s_storageCave/CallAt` | absent | scan dispatcher for `E8 → 0x4CABD0` (fixed-site `E8 BF 03 DE FF` before 6dc57dd) |
| `h_Storage` | absent | `RunHookBody(..., HOOK_STORAGE, 0x4CABD0)` |
| `enable` default | `shop,residence,pub,factory,cityhall,citizen,do` | `...,do,storage` |
| Button label | `bx+size*0.5`, `0.70*fs`, `ty+(size-fs)/2` | `FitLabelToWidth` + `0.78*fs`, `width/height` centering, wide-bar tint fix |

The button changes are cosmetic (fix “PRIVATISE THIS BUILDING” bleed). The storage hook is the only new functional hook.

### private_logistics.cpp 1112→3588

- **v0.3:** `billing=1 dry_run=1` (instrument only), `pay_factory=1 factory_share=0.90`, `pay_carrier=1` with `freight_rate_*`, no `auto_targets`, no `TARIFF`, no `trip_tariff`, no `state_storage`.
- **v0.17:** full `v0.7 tariff` (`litres_per_trip 14.75 + wear 5.00 L, margin 35%, density 0.001`), `trip_tariff=1`, `lives=16` sites, `auto_targets_enabled=1 dry_run=1` with `AutoProbeStateInfrastructure` (`game+0x11B08` → `world=29`), `AutoHasNativeStorage` (`bld+0x970` stride `0xE0`), `AutoBuildingKey` (type+pos+name hash), permission file `private_logistics_state.bin`.

`.ini` grew 93→216 lines — old had 6 keys (`enabled,billing,dry_run,pay_factory,pay_carrier,factory_share,min_payment,daily_pay_cap,freight_rate_*`), new has 40+ (`litres_per_trip/wear/tariff_margin`, `baskets_*`, `census/cargo_dump`, `auto_targets_*`).

### .ini mismatch explains “checkbox doesn’t work”

- `ui_panels.ini` old **omits** `,storage` — with that ini on disk, new DLL’s `ListHas(v, "storage")` is false, so `g_hooks[HOOK_STORAGE].wanted=0` and **`InstallStorageCallSiteHook` is never called** → no `storage: hook ... installed` and no `probe storage` ever, even if DLL is correct.
- Your new log (1949 lines) *does* show `storage: hook call 0x6EA80C -> builder 0x4CABD0 installed`, so you updated the ini — good. But the warehouse panel still never emitted `probe storage`, meaning you hadn’t opened it in that session (only DO: `probe do: builder bottom 0.0 -> 1686.0`).

## Why DO “stopped detecting”

It didn’t — **automation is in log-only mode**:

```ini
auto_targets_enabled=1
auto_targets_dry_run=1   ; log missing pairs only; do not append
```

For every day in both logs (old 208k-line and new 1949-line):

```
[ AUTO DO] DRY RUN office=E15C... target=BD98... type=3
[ AUTO DO] DRY RUN office=E15C... target=B594... type=6
office type=43 existing=0 planned=2 added=0
summary private offices=1 candidates=2 planned=2 added=0 existing=0 mode=DRY RUN
```

`existing=0` = DO’s native `0xD38/0xD40` vector is empty in the save (no manual stations). `planned=2` are the two private storages we discovered (fabric_factory + small store). With `dry_run=1` we **never write**, so the DO stays empty. Vanilla “Add station” would still work if a road exists — the fact it shows 0 buildings suggests either no road, or you expected our automation to have filled it.

## Recommended verification

1. **Keep our stub fix** (`private_construction 3.5.2-STUB` already merged to `main db9e801`) — it fixes the `0/16 patched` duplicate. Do not revert to 1112-line v0.3 unless you want to lose tariff.
2. **Storage checkbox:** with new `ui_panels.ini` (contains `,storage`) open `warehouse_covered_combined` — with `6dc57dd` you will now get:
   ```
   [AUTO DO] storage widget probe: building=... type=5 key=31C... priv=0 nativeStorage=1
   ```
   Toggle `Allow private DOs to load here` → `state storage permission key=31C... -> load enabled` and next `infra probe permission=LOAD-ON`.
3. **DO automation:** in a **disposable save**, set:
   ```ini
   auto_targets_dry_run=0
   ```
   Restart → day close should change to `added=2` and `existing=2` (and `3` once warehouse is enabled). Keep cap `5` (`AUTO_DO_SAFE_TARGET_COUNT`).
4. If vanilla “Add station” still shows 0 buildings after that, the save’s road graph is disconnected — check road connection `bld+0xA10` stride `0x60` type 3=road (see `ghidra-do-storage-findings.md`).

Reverting fully to the 1112-line v0.3 would restore manual-only DO (no automation, no tariff), but it is not the reason the current DO is empty — `dry_run=1` is.
