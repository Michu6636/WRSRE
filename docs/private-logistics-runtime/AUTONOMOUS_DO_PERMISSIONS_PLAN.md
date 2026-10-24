# Autonomous private DOs — step 1 implementation and step 2 test

## Current status

Implemented locally: autonomous reconnection, role-aware state-site controls, independent directions, permission migration and TaskCfg reconciliation. `private_logistics` now reports **0.18.1-permissions**. This is ready for a controlled permissions experiment, NOT a claim of validated native planner enforcement. Probe, fuel lookup and freight billing algorithms were deliberately left unchanged.

## Ownership and direction policy

Directions are always from the truck's perspective: LOAD takes goods FROM a site; UNLOAD delivers goods INTO a site.

| Building | Controls on state-owned site | Private-company behaviour |
|---|---|---|
| Retail/service consumer | Deliveries only | Unload only; no player-access controls |
| Factory | Loading and deliveries independently | Both directions, subject to native input/output roles |
| Warehouse/storage | Loading and deliveries independently | Both directions, subject to native resource acceptance |
| Unknown/infrastructure/customs/DO | No access widgets | No guessed cargo endpoint role |

State directions default OFF. Private endpoints ignore saved public-access permissions while private. Returning a site to state ownership reuses its persisted public permissions, or default deny if none. Ownership is checked on each scan. Missing eligible connections are re-added at the existing once-per-game-day reconciliation; manual removal is not a permanent veto. Reachability, pointer checks, deduplication and the conservative five-total-target safety budget remain intact.

## Role classifier and limits

The classifier is shared production code in `plugins/private_logistics/site_permissions.h`:

- Type 5: storage; type 6: factory (the factory identity is also documented in enterprise_base.cpp).
- Known consumer families 3, 4, 10, 23, 24, 25, 26, 28: delivery-only. This follows existing private_shops consumer family coverage; native resource acceptance still determines usable cargo. It is not a general classifier for every Workshop asset.
- Exact asset basename exceptions `warehouse_covered_combined` and `warehouse_open`: storage, even if a known warehouse reports runtime type 3. No substring-based guess that any object containing "store" is a warehouse.
- Unrecognized type/asset: no actionable widgets and automatic directions denied. A bounded-on-panel-change diagnostic records type, asset and capabilities so missing classifications can be investigated rather than silently granting loading access to arbitrary storage-bearing buildings.

Storage presence is necessary but no longer sufficient to offer loading. This corrects the previous broad `hasNative && !private` widget gate. Additional production types (e.g. specialized mines/industry) need evidence-based classification before enabling them; no claim of universal asset coverage.

## Native TaskCfg application

Evidence consulted: `docs/engine/logistics.md` sections 5–6, `docs/ghidra-do-storage-findings.md`, `docs/private-logistics-runtime/evidence/ROAD_PATH_EVIDENCE_COMPLETE.md`, and warehouse discriminator reports. These are repository RE evidence, not fresh Ghidra execution here.

- LOAD wildcard/percentage/list at cfg+0x00/+0x04/+0x08..0x10.
- UNLOAD wildcard/percentage/list at cfg+0xC8/+0xCC/+0xD0..0xD8.
- Denial now sets wildcard=0, percentage=0 and makes that resource selector list empty (end=begin). Previously setting only the load threshold to zero left loading selectors enabled.
- Allowed direction uses wildcard=1 and percentage=1. Native storage modes, accepted resources and connection reachability remain the planner's responsibility. This is per-target direction permission, not a replacement for resource/input/output validation in the engine.
- Reconciliation applies to ALL entries of a private DO, including existing manually added public entries, unknown/unindexed targets and default-OFF sites. It does not skip revocation when no permitted candidates remain.
- Native vector spans are checked before existing config writes. Invalid configuration logs a failure to enforce; do not continue a test with such a failure. The v0.18 test exposed that leaving the entry enabled byte at zero never requested activation. v0.18.1 requests entry activation when any direction is allowed, and disables it when neither is allowed, then uses native dirty/replanning. Existing active/validated config fields are not forced. Native route validation is still required; activation alone is not proof of reachability.
- Only list ends are shortened; allocation begin/capacity and backing items are not freed or overwritten. This private company owns its DO task selectors: old user per-resource selections can be replaced by company wildcard directions, and a denied selector list is not restored on re-allow. Native storage/resource compatibility is not changed. Back up the save before this policy migration.
- Changes mark the office dirty and schedule the existing native rebuild path. In-flight native orders may outlive a permission change. Immediate cancellation or exact revocation boundaries are NOT proven; no goods are deleted or teleported by this permission code.
- Existing `state_storage_always_on` values are ignored and logged. They no longer bypass state access choices.
- Manually assigned customs (type 20) tasks are preserved byte-for-byte: they are border-trade tasks, not state-enterprise permission targets. They are not auto-discovered as assignable sources by this change; nearest-customs diagnostics remain geometric only.

## Save migration

The sidecar filename remains `private_logistics_state.bin`; its header is versioned from PLSTOR17 to PLSTOR18. Record shape remains stable building key + one permission byte.

- PLSTOR17 mode 1 -> LOAD only.
- PLSTOR17 mode 2 (explicit OFF) -> neither direction, NEVER UNLOAD.
- PLSTOR18 stores LOAD=1, UNLOAD=2, BOTH=3; zero means no permission.
- Capabilities mask saved directions at use, so an old shop LOAD opt-in cannot turn a shop into a source.
- Reconnection memos are session-local; the removed suppression cannot be revived by a saved state-access preference.

## Checks executed

- 11 logistics tests (updated after the failed v0.18 gameplay test): source guards for reconnection/enforcement plus native compilation/execution of the ACTUAL production classifier/config writer and two-direction widget handler. Cases cover independent toggles, private/public effective masks, known consumer vs type-3 warehouse, unknown sites, all 256 possible migration byte values, all four direction masks, existing explicit resource lists, repeated idempotent reconciliation and preservation of unrelated config bytes.
- 4 private-shop tests: foreign-restock defaults and delivery-debt settlement regression tests.
- 93 probe tests unchanged. **108 tests total** across these three suites.
- Production classifier/config harness also passed GCC AddressSanitizer and UndefinedBehaviorSanitizer.
- Full updated plugin compiled and linked as Windows x64 with Zig Windows-GNU. This is not an MSVC build and was not executed in Windows or the game. Generated compiler/object/DLL files remain ignored under .cache, not Git deliverables.
- `git diff --check` passed.

These checks do not establish native reachability, cancellation of already dispatched jobs, memory-write synchronization with the game UI, or whether the native planner interprets all selector combinations exactly as inferred from static evidence. Step 2 below exists to establish that boundary explicitly.

## Step 2 — controlled game test, before fuel work

After integrating the source, build from the project Windows build environment:

```bat
cmd /c "build.bat private_logistics"
cmd /c "build.bat private_logistics_probe"
```

Deploy both rebuilt DLLs and their shipped INIs to the existing active loader. The probe default cadence/budget changed in v0.18.1's test profile, not its measurement algorithm. Back up the loader/plugin INIs, save, and permission sidecar. Do not overwrite the original save with an experimental one. Keep the unchanged probe's existing executable identity and single-save/process restrictions. Fully exit between experiments; do not reload worlds with the enabled pinned probe.

Use a copied save with one private road DO and one truck. For testing the state controls, prepare reachable state warehouse, state shop, and supported state factory (with identifiable input/output goods). Keep the total target count within the existing conservative budget. Also check a private shop's panel. Confirm all listed buildings actually carry the intended ownership.

The shipped INI AND C++ missing-key fallbacks now already contain the following test profile; do not rely on a checklist asking the player to fix defaults. Deploy the updated INI so an older explicit setting does not override it:

```ini
[private_logistics]
enabled=1
billing=0
dry_run=1
auto_targets_enabled=1
auto_targets_dry_run=0
```

Keep `private_logistics=1` in the loader: this plugin must run to manage permission tasks. Legacy freight payments are disabled above, NOT the target manager. Ordinary purchases, private-shop wholesale accounting and other mods can still move money. Keep `[private_shops] foreign_restock_enabled=0`. The probe may log cargo, but its fuel measurements are not the acceptance criterion for this stage. Its capture defaults are now interval 30 / budget 1000; its fuel lookup remains unchanged and still needs the later correction.

### A. Startup and UI

1. Confirm `0.18.1-permissions`, billing=0, auto-target write mode, and private_shops `foreign off` in the log.
2. State shop: only `Allow private DO deliveries (receive goods)`. No loading control.
3. Factory/warehouse: independent `Allow private DO loading (take goods)` and deliveries controls.
4. Private endpoint: neither state-access checkbox. Unsupported infrastructure: no misleading loading control. Capture each panel; `state panel ... capabilities=...` identifies the classifier result.
5. Native widget raw 0 is visually CHECKED; raw 1 is UNCHECKED. Only the UI boundary converts this representation; saved direction bits retain their meaning. This also matches the existing citizen_labor sector widget. Fresh public permissions: both OFF and visibly unchecked. Migrated warehouse LOAD opt-in stays load-only. Migrated retail LOAD must NOT survive as a loading authorization.

### B. Permission matrix (wait for daily reconciliation)

For each state warehouse/factory, test neither, LOAD only, UNLOAD only, BOTH. Arrange eligible source cargo and destination demand so absence of movement is informative, rather than caused by lack of goods or route. Observe task directions and actual transfers, not just a checkbox changing. For factory LOAD check output goods; for UNLOAD check accepted inputs. Verify state retail is never used as a source, even when it has stock. Open a private retail site and confirm it remains unload-only without state permission controls.

Existing entries matter: repeat OFF on a site ALREADY in the private DO list; check log `task directions ... load=0 unload=0`. Distinguish a previously dispatched truck finishing work from new scheduling after reconciliation. Stop on config/enforcement faults; preserve the full log before relaunch.

### C. Reconnection and persistence

With access still allowed, remove an eligible target entry. It should be recreated on the next daily scan (not permanently suppressed). Do not expect a missing road to be bypassed: actual disconnection/reconnection is a separate native reachability check. Watch logs for the conservative capacity limit rather than interpreting that as a permission veto.

Check persistence with a save, FULL game exit and fresh launch. Verify directions and ownership changes on copied saves. Do not perform in-process reloads with the current enabled probe.

Return full tesmioloader.log, panel screenshots, timestamps of toggles/removal and observed source/destination transfers. A failed direction test needs diagnosis before fuel/trip work, not a claim that config tests proved gameplay enforcement.

## Remaining stages — deliberately not included

After permission tests pass, correct the probe's resource identity lookup and demonstrate actual fuel deductions against the truck UI in a no-refuel journey. Current fuel_slot_valid=0 / constant mirror is not a valid burn measurement. Then implement one loaded per-vehicle interval, actual deductions counted once, complete unloading, empty legs excluded, settlement-time price, and missing price=inconclusive. Validate two consecutive loaded journeys before refuel/partial-unload/save-load cases. Replace legacy per-fragment freight settlement only after that evidence. Automatic foreign restock stays OFF; delivery debt stays in place.


## Failed v0.18 gameplay run and corrective patch — 2026-10-24

### Attachment provenance and observed failures

Input: 8,129-line `tesmioloader.log`, SHA-256 `d4ba6d01c3979f65b90a6495c8aa85cf7a8370db5c1dd3a45ca918928b9ad6bb`. Raw log is not committed.

- Startup at 19:52:52 reports v0.18-permissions, **billing LIVE / billing=1**, foreign restock off. The previous shipped INI/default still enabled legacy billing despite the requested permission-only experiment: this was a source/default failure, not something to blame on the player.
- State grocery (key 3C0DBB6431F9650D) at 19:56:14.171 logs unload=1 when the player reports visually unchecking the box. Day 5 then adds a third connection. With logical permission zero, the v0.18 handler passed raw zero to a native widget that renders it checked. The test stub incorrectly assumed raw 1 was checked and therefore ratified the error. Existing `citizen_labor.cpp` sector widget already documents and handles raw 0=checked; that evidence should have been used.
- New eligible targets were appended OFF (entry byte zero) and existing reconciliation never activated them. `docs/engine/logistics.md` Part 2 states that native UI-created entries await a separate activation checkbox. An autonomous company cannot rely on the native UI user to turn its generated rows on. This is a concrete code defect and a strong explanation for connected-but-idle tasks; full proof that all idle behaviour is resolved still requires the next native test.
- After a manual customs assignment, an unclassified target receives load=0/unload=0 repeatedly around 20:01:47 and later. All type-20 customs are excluded by the role classifier, and v0.18 applied default deny even to existing customs. This is consistent with the player's one-trip-then-stops observation. No explicit TaskEntry deletion code exists in reconciliation; direction disabling and actual native pruning must not be conflated. The old log lacks target type/name/activation audit fields needed to reconstruct every internal change.
- 33 committed destination callbacks delivered a daily aggregate 5.500 t of food at roughly 20:01:57; legacy billing charged each callback. The destination grocery was privatized during this run (20:00:47), so these records are not proof of a completed delivery to a state-owned grocery.
- Probe hit its old 100-snapshot cap: 1 reset + 1 selection + 98 measurements. Last snapshot 20:01:21.636, BEFORE the delivery at 20:01:57. All measured cargo totals were zero; all fuel slots invalid; mirror constant 0.092560. This run contains no probe fuel measurement of that delivery, and does not validate fuel burn.

### Corrective implementation

1. Convert logical permission to the native checkbox boundary with raw 0=checked, raw 1=unchecked; convert back before changing permission bits. Fresh OFF now draws unchecked. Saved preferences are preserved, not silently reset or globally inverted.
2. Activate eligible managed entries separately from their direction selectors. Denied entries request activation OFF. New and present targets use the same shared activation helper and native replan notification. Existing resources are not moved or synthesized.
3. Preserve manually assigned customs tasks without rewriting their activation, resource selectors or directions. Customs remain excluded from state-enterprise checkboxes and automatic candidate assignment. This does not implement automatic customs routing/reservation.
4. Ship logistics billing=0, enabled=1, auto targets=1, auto dry-run=0 in both INI and fallback/static defaults. Probe stays enabled with interval=30, max_snapshots=1000, max_queue=256, selftest=0 in INI/fallbacks. Private-shop foreign restock remains OFF. A startup profile line and mismatch warning identify an old deployed override.
5. Add bounded daily native task audit lines showing type, policy (managed or preserve-customs), requested direction mask, entry activation, actual wildcard bytes, percentages and selector counts. If dispatch still fails, this is diagnostic evidence rather than another uninstrumented checkbox experiment. Threshold interpretation, route validity and native input/output acceptance remain separate acceptance checks; no unsupported offset changes were made to those mechanisms.

### Validation and next run

108 tests passed (11 logistics, 4 private-shop, 93 probe). The production policy/config/activation harness passed GCC ASan/UBSan. Both changed DLL sources cross-compiled and linked for Windows x64 with Zig Windows-GNU, not MSVC, and have NOT been executed on Windows or in game here. Host tests use the native widget polarity reported by gameplay, but are not a substitute for rendering in the game.

Build/deploy both DLLs AND shipped INIs; keep saved preferences unless deliberately starting a clean copied save. There should be no manual INI repair needed for a fresh deployment. Existing explicit old settings still override fallback defaults and are clearly warned about; they are not silently overwritten by the DLL. Required startup line: `permission-test profile: billing=0 auto_targets=1 apply=1`.

First verify a fresh/denied state grocery is visibly unchecked, checking it logs unload=1, and unchecking logs unload=0. Checkbox means authorization, not instantaneous connection status: reconciliation still waits for the daily callback and needs a private DO. Next check warehouse LOAD and observe `entry_enabled=1`, actual goods pickup and delivery to an authorized destination. Finally verify a manually assigned customs task continues after multiple daily reconciliations and is logged `policy=preserve-customs`. Stop and return the full log on mismatched profile, invalid config or continued idle behaviour. Do not treat current fuel data as validated or change tariffs based on zero/invalid fuel.


**Existing-save caveat:** v0.18 may already have saved a customs task with both directions zeroed. Preserving it now cannot reconstruct the prior resource selections or original intent. Prefer a copied save from before that corruption; otherwise reassign/configure the customs task once through the native UI and then verify it survives daily reconciliation. Do not silently force-enable arbitrary old customs configurations or mistake preservation for automatic customs sourcing.
