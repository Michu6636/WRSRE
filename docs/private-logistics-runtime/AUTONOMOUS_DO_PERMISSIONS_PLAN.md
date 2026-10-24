# Autonomous private DOs — step 1 implementation and step 2 test

## Current status

Implemented locally: autonomous reconnection, role-aware state-site controls, independent directions, permission migration and TaskCfg reconciliation. `private_logistics` now reports **0.18-permissions**. This is ready for a controlled permissions experiment, NOT a claim of validated native planner enforcement. Probe, fuel lookup and freight billing algorithms were deliberately left unchanged.

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
- Native vector spans are checked before existing config writes. Invalid configuration logs a failure to enforce; do not continue a test with such a failure. The code does not force the target entry's enabled byte or existing active/validated flags to bypass native reachability.
- Only list ends are shortened; allocation begin/capacity and backing items are not freed or overwritten. This private company owns its DO task selectors: old user per-resource selections can be replaced by company wildcard directions, and a denied selector list is not restored on re-allow. Native storage/resource compatibility is not changed. Back up the save before this policy migration.
- Changes mark the office dirty and schedule the existing native rebuild path. In-flight native orders may outlive a permission change. Immediate cancellation or exact revocation boundaries are NOT proven; no goods are deleted or teleported by this permission code.
- Existing `state_storage_always_on` values are ignored and logged. They no longer bypass state access choices.

## Save migration

The sidecar filename remains `private_logistics_state.bin`; its header is versioned from PLSTOR17 to PLSTOR18. Record shape remains stable building key + one permission byte.

- PLSTOR17 mode 1 -> LOAD only.
- PLSTOR17 mode 2 (explicit OFF) -> neither direction, NEVER UNLOAD.
- PLSTOR18 stores LOAD=1, UNLOAD=2, BOTH=3; zero means no permission.
- Capabilities mask saved directions at use, so an old shop LOAD opt-in cannot turn a shop into a source.
- Reconnection memos are session-local; the removed suppression cannot be revived by a saved state-access preference.

## Checks executed

- 7 logistics tests: source guards for reconnection/enforcement plus native compilation/execution of the ACTUAL production classifier/config writer and two-direction widget handler. Cases cover independent toggles, private/public effective masks, known consumer vs type-3 warehouse, unknown sites, all 256 possible migration byte values, all four direction masks, existing explicit resource lists, repeated idempotent reconciliation and preservation of unrelated config bytes.
- 4 private-shop tests: foreign-restock defaults and delivery-debt settlement regression tests.
- 93 probe tests unchanged. **104 tests total** across these three suites.
- Production classifier/config harness also passed GCC AddressSanitizer and UndefinedBehaviorSanitizer.
- Full updated plugin compiled and linked as Windows x64 with Zig Windows-GNU. This is not an MSVC build and was not executed in Windows or the game. Generated compiler/object/DLL files remain ignored under .cache, not Git deliverables.
- `git diff --check` passed.

These checks do not establish native reachability, cancellation of already dispatched jobs, memory-write synchronization with the game UI, or whether the native planner interprets all selector combinations exactly as inferred from static evidence. Step 2 below exists to establish that boundary explicitly.

## Step 2 — controlled game test, before fuel work

After integrating the source, build from the project Windows build environment:

```bat
cmd /c "build.bat private_logistics"
```

Deploy only the updated `private_logistics.dll` and INI to the existing active loader. Back up the loader/plugin INIs, save, and permission sidecar. Do not overwrite the original save with an experimental one. Keep the unchanged probe's existing executable identity and single-save/process restrictions. Fully exit between experiments; do not reload worlds with the enabled pinned probe.

Use a copied save with one private road DO and one truck. For testing the state controls, prepare reachable state warehouse, state shop, and supported state factory (with identifiable input/output goods). Keep the total target count within the existing conservative budget. Also check a private shop's panel. Confirm all listed buildings actually carry the intended ownership.

In the active `private_logistics.ini`, preserve the section and unrelated settings, and set:

```ini
[private_logistics]
enabled=1
billing=0
auto_targets_enabled=1
auto_targets_dry_run=0
```

Keep `private_logistics=1` in the loader: this plugin must run to manage permission tasks. Legacy freight payments are disabled above, NOT the target manager. Ordinary purchases, private-shop wholesale accounting and other mods can still move money. Keep `[private_shops] foreign_restock_enabled=0`. The probe may log cargo, but its fuel measurements are not the acceptance criterion for this stage. It remains unchanged and still needs the later lookup correction.

### A. Startup and UI

1. Confirm `0.18-permissions`, billing=0, auto-target write mode, and private_shops `foreign off` in the log.
2. State shop: only `Allow private DO deliveries (receive goods)`. No loading control.
3. Factory/warehouse: independent `Allow private DO loading (take goods)` and deliveries controls.
4. Private endpoint: neither state-access checkbox. Unsupported infrastructure: no misleading loading control. Capture each panel; `state panel ... capabilities=...` identifies the classifier result.
5. Fresh public permissions: both OFF. Migrated warehouse LOAD opt-in stays load-only. Migrated retail LOAD must NOT survive as a loading authorization.

### B. Permission matrix (wait for daily reconciliation)

For each state warehouse/factory, test neither, LOAD only, UNLOAD only, BOTH. Arrange eligible source cargo and destination demand so absence of movement is informative, rather than caused by lack of goods or route. Observe task directions and actual transfers, not just a checkbox changing. For factory LOAD check output goods; for UNLOAD check accepted inputs. Verify state retail is never used as a source, even when it has stock. Open a private retail site and confirm it remains unload-only without state permission controls.

Existing entries matter: repeat OFF on a site ALREADY in the private DO list; check log `task directions ... load=0 unload=0`. Distinguish a previously dispatched truck finishing work from new scheduling after reconciliation. Stop on config/enforcement faults; preserve the full log before relaunch.

### C. Reconnection and persistence

With access still allowed, remove an eligible target entry. It should be recreated on the next daily scan (not permanently suppressed). Do not expect a missing road to be bypassed: actual disconnection/reconnection is a separate native reachability check. Watch logs for the conservative capacity limit rather than interpreting that as a permission veto.

Check persistence with a save, FULL game exit and fresh launch. Verify directions and ownership changes on copied saves. Do not perform in-process reloads with the current enabled probe.

Return full tesmioloader.log, panel screenshots, timestamps of toggles/removal and observed source/destination transfers. A failed direction test needs diagnosis before fuel/trip work, not a claim that config tests proved gameplay enforcement.

## Remaining stages — deliberately not included

After permission tests pass, correct the probe's resource identity lookup and demonstrate actual fuel deductions against the truck UI in a no-refuel journey. Current fuel_slot_valid=0 / constant mirror is not a valid burn measurement. Then implement one loaded per-vehicle interval, actual deductions counted once, complete unloading, empty legs excluded, settlement-time price, and missing price=inconclusive. Validate two consecutive loaded journeys before refuel/partial-unload/save-load cases. Replace legacy per-fragment freight settlement only after that evidence. Automatic foreign restock stays OFF; delivery debt stays in place.
