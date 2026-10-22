# Regression triage — 2026-10-22

**Branch:** `arena/90ccf7f8-wrsre` (1623763) vs `origin/main f38de86` (merge #1 666eb65 doc reorg)  
**Log:** `tesmioloader.log` 3648 lines, 17:56:37–17:59:43, SOVIET64.exe 1.1.1.9  
**Report:** storage-unit checkbox does not work; DO stopped detecting any buildings.

## 1. Duplicate DLL — primary blocker (fixed in this branch)

`plugins/private_construction/private_construction.cpp` on `origin/main` is **byte-identical** to `plugins/private_logistics/private_logistics.cpp` (3567 lines, `PL_VERSION 0.17`, `info->name = "private_logistics"`).

`build.bat` builds every `plugins/*/*.cpp` into `build/plugins/<name>.dll`, so a local build produces:

```
plugin   private_logistics 0.17 from private_construction.dll
  patch 0x141F33..0x694F40 16/16 ok (cave 0x...)

plugin   private_logistics 0.17 from private_logistics.dll
  site 0x141F33 calls 0x7FF61B4A0000 not transfer skipped (16 lines)
  patched 0 of 16 ERROR no sites patched ... not at RVA 0x420350
```

The second instance finds call-sites already rewritten (first instance's caves), logs 0/16 and bills nothing. It also registers a second `private_logistics` persistence handler (`handler #5` twice) and a second `widget provider 4` (same `StateStorageWidgetProvider` callback) — duplicated `[AUTO DO]` infra probe + `[B2B] day N closed` (192 calls / 0 calls per day).

`private_construction.ini` (341 lines) is correct — v3.5.2 native phase pricing via `typedesc+0x370`, `daily_build_rate 0.03`, `State Acceptance Commission 99%` — only the cpp is wrong. No correct version exists in repo history; both `76917e1` and `a281562` initial commits already contain the duplicate.

**Fix in 1623763:** stub at `plugins/private_construction/private_construction.cpp` (39 lines) advertises `private_construction v3.5.2-STUB`, does no patching, logs:

```
private_construction STUB v3.5.2-STUB: original cpp was duplicate of private_logistics; stub prevents double-patch collision
```

This unblocks single-DLL logistics tests. **Before any release claiming construction functionality, restore the real file** from the Windows checkout:

```
C:\Users\EricM\Downloads\WRSRE\plugins\private_construction\private_construction.cpp
```

then `build.bat private_construction`.

## 2. Log triage — why checkbox appears dead and DO empty

### ui_panels 6.11 hooks — installed correctly
```
ui_panels storage: hook call 0x6EA80C -> builder 0x4CABD0 installed (bytes E8 BF 03 DE FF verified)
ui_panels do:      hook 0x6EB4FD -> DO builder 0x741170 ok (cave ...)
ui_panels probe do: builder bottom 0.0 -> 1686.0 (maintains +0x250)
ui_panels do: appended 10 row(s) ...  (DO type 43)
ui_panels probe factory: ... appended 11 row(s) (type 6)
```
No `probe storage` line appears in the session — the storage panel for `warehouse_covered_combined` was never opened, or its widget pass was budget-gated. The warehouse is nevertheless **detected**:

```
[AUTO DO] day 8 infra probe: world=29 state_storage=1 private_storage=0 customs=8
[AUTO DO] state storage candidate name=warehouse_covered_combined key=31C37BA699FA04C6 pos=(-5758.8,5325.1) permission=OFF-DEFAULT
[AUTO DO] day 8 nearest customs office=E15C05467B0A8DDE customs=zoll_milhost dist=90.5m
```

`permission=OFF-DEFAULT` (0) is the default-off policy; `LOAD-ON` (1) would be explicit opt-in, `OFF-EXPLICIT` (2) opt-out after opt-in. The candidate stays `OFF-DEFAULT` for the whole session (next 16 days only `infra probe` lines, no candidate re-log unless counts change). No line:

```
[AUTO DO] state storage permission key=31C37BA699FA04C6 -> load enabled
```

was emitted, meaning the warehouse-panel checkbox callback `StateStorageWidgetProvider` (private_logistics.cpp:2106, `type==5 && !isBuildingPrivate && widgetCheckbox`) was not toggled to ON during the test — either never clicked, not visible (max_rows 14 budget), or click not reaching the widget.

### DO "stopped detecting" — dry-run is working as configured

```
[AUTO DO] day 8 DRY RUN office=E15C05467B0A8DDE target=B5943471085A010E type=6  (fabric_factory)
[AUTO DO] day 8 DRY RUN office=E15C05467B0A8DDE target=BD98B4F9C35422E3 type=3  (Avryu Small store [P])
[AUTO DO] day 8 office=E15C05467B0A8DDE type=43 existing=0 planned=2 added=0 config_changed=0 suppressed=0
[AUTO DO] day 8 summary: private offices=1 candidates=2 planned=2 added=0 existing=0 suppressed=0 invalid=0 mode=DRY RUN
```

`candidates=2` are the two private buildings with native storage (`0x970` vector, stride `0xE0`): `fabric_factory` (type 6, 3 storages) and `Avryu Small store` (type 3, 2 storages) — confirmed by `private_shops` probe. `existing=0` is the DO's current native target vector length (`do+0xD38/0xD40`), so the save's DO currently has no targets. `planned=2 added=0` with `mode=DRY RUN` is **intended** while `plugins/private_logistics.ini:auto_targets_dry_run=1`:

```ini
auto_targets_enabled=1
auto_targets_dry_run=1   # log only; do not call AutoAppendNativeTarget
```

The DO's vanilla "Add station" list being empty is separate from this automation — automation counts manual entries as `existing` and preserves them, but with `existing=0` the native list is empty (no reachable candidates or no road connection in this 29-building test world).

Billing still moves money (dry_run=0 for billing):

```
[B2B] day 8 closed: 192 calls, 178 committed | [PAY] factory fabric_factory +26.56 ...
private_logistics live v0.17: billing=1 dry_run=0 ... price = TRIP TARIFF
```

So the regression is **not** a code change in panels — it is the expected dry-run behaviour plus the duplicate DLL masking the storage opt-in.

## 3. What to do next

1. **Fix build** (done in this PR for triage): `git fetch origin; git checkout arena/90ccf7f8-wrsre; git pull` — `private_construction.cpp` is now a stub; delete `build/plugins/private_construction.dll` if you tested with the duplicate, then:
   ```
   build.bat private_logistics
   rem or with restored real file:
   build.bat private_construction
   ```
   Re-test in a **disposable save** with `auto_targets_dry_run=1` (default). Open the warehouse (`warehouse_covered_combined` at -5758,5325), check `Allow private DOs to load here`. The log must show:
   ```
   [AUTO DO] state storage permission key=31C37BA699FA04C6 -> load enabled
   [AUTO DO] day N infra probe: ... permission=LOAD-ON
   ```

2. **Enable live automation only after state-storage verification** (per `ghidra-do-storage-findings.md` asset mode still unverified for `warehouse_covered_combined`):
   ```ini
   auto_targets_dry_run=0
   ```
   Then a day close should change the summary to:
   ```
   planned=2 added=2 existing=0 -> existing=2 next day
   ```
   and for the warehouse (once opted in) `candidates=3 planned=3`. Verify `panel+0x310+idx*4` checkbox array stays within the conservative cap 5 (`AUTO_DO_SAFE_TARGET_COUNT=5`) and save/reload retains entries.

3. **Restore real construction source** before next release: copy `C:\Users\EricM\Downloads\WRSRE\plugins\private_construction\private_construction.cpp` over the stub, commit, and re-verify `info->version` catalog (`private_construction 3.5.2` in `DEVELOPMENT_README.md`).

## 4. Files

- `plugins/private_construction/private_construction.cpp` — stub (this PR)
- `plugins/private_logistics/private_logistics.cpp` 0.17 unchanged (storage checkbox via `ui_panels.widgetCheckbox`, load-only TaskCfg `+0x04=1.0/+0xC8=0/+0xCC=0`)
- `plugins/ui_panels/ui_panels.cpp` 6.11 unchanged (storage hook `0x6EA80C->0x4CABD0`, DO dispatcher `0x6E9B80+0x8000`)
- `docs/ghidra-do-storage-findings.md` — DO vector `0xD38/0xD40` stride `0x18`, TaskCfg `0x198`, dirty `+0xD50`/`+0xFC0`, connection types 3=road/6=rail
