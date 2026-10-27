# Autonomous private DOs — step 1 implementation and step 2 test

## Current status

**Latest status:** v0.18.4 fixes confirmed unused-percentage churn exposed by the diagnostic run. Why a reportedly full source dispatches only after lowering its threshold remains unresolved; this is NOT a claim that all dispatch failures are fixed. Do not advance to fuel work or assume direct field writes reproduce the complete native lifecycle. See `DO_PERCENTAGE_WAKEUP_INVESTIGATION.md` for the new log evidence and bounded native-handler trace request.

Implemented locally: autonomous reconnection, role-aware state-site controls, independent directions, permission migration and TaskCfg reconciliation. `private_logistics` now reports **0.18.4-denied-threshold**. The latest player test reports working connections and deliveries; its log contains state-warehouse pickups and state-store deliveries. This is not comprehensive planner validation. The remaining daily threshold-reset defect is corrected below, pending the next in-game check. Probe, fuel lookup and freight billing algorithms were deliberately left unchanged.

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
- Denial sets wildcard=0 and makes that resource selector list empty (end=begin). Since v0.18.4 it preserves the unused percentage, because native code can normalize it even while the direction is denied. Setting only a percentage to zero is not a denial mechanism; retaining a nonzero unused percentage does not grant resource access.
- Allowed directions use wildcard=1 and the configured company percentage (source 0.20 / destination 0.30 by default since v0.18.2). Native storage modes, accepted resources and connection reachability remain the planner's responsibility. This is per-target direction permission, not a replacement for resource/input/output validation in the engine.
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

1. Confirm `0.18.2-thresholds`, billing=0, auto-target write mode, and private_shops `foreign off` in the log.
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


## Working connections; correct company stock thresholds — 2026-10-24

### New evidence and cause

Latest attachment: 3,867-line `tesmioloader.log`, SHA-256 `3030810df24744528f1f46f50b57d5087de36d187a02d533eb7f86c0cad6566e`; screenshot shows native per-task and office-wide threshold controls. Startup reports v0.18.1-permissions with `billing=0 auto_targets=1 apply=1`, confirming the intended test defaults were deployed. The player reports that connections now work. The log contains 62 `class=load` fragments and 50 `class=delivery` fragments, including state warehouse pickups and state grocery deliveries; these are callbacks, NOT counts of journeys.

However the log repeatedly shows the managed warehouse rewritten to `load_pct=1.000 unload_pct=1.000`, including 20:25:31, 20:25:54 and 20:26:15. The helper wrote 1.0 for every allowed direction on every daily reconciliation, so native adjustments (including the source-side 90% ceiling reported by the player) and manual edits were overwritten. This is an implementation default, not a missing offset. Existing TaskCfg evidence and the runtime UI/log correlation establish `cfg+0x04` as source percentage and `cfg+0xCC` as destination percentage, fractions rather than whole integers. No new Ghidra work is needed for this bounded change.

### Implemented policy: v0.18.2-thresholds

- Source: request pickup when stock is **above 20%**. This uses a low source-stock trigger rather than waiting until a warehouse is almost full; it is not a newly verified guarantee about final stock after a loaded order.
- Destination: request delivery when stock is **below 30%**. This is a native dispatch trigger, not a monetary budget or exact final-stock ceiling. Vehicle cargo and outstanding orders can cause delivered stock to exceed the trigger.
- Shipped INI, static defaults, helper defaults and missing-key fallbacks agree. Optional `[private_logistics]` keys: `auto_source_percent=20`, `auto_destination_percent=30`. Config values are clamped to our policy ranges source 0..90, destination 10..100. The latter is a conservative supported operating range, not a newly reverse-engineered claim about every native boundary.
- Both newly appended and existing managed targets use the same configured fractions, so legacy 100%/90% values converge on the next daily reconciliation. Denied directions still have wildcard zero, percentage zero and empty resource selectors; their permission is not re-enabled by choosing a threshold.
- Private-company managed tasks still reconcile daily. Native per-connection or office-wide edits are NOT persisted as separate player overrides; they will be replaced by company policy. This change does not add per-DO preferences. State access checkboxes remain the independent player permission controls.
- Manually assigned customs remain preserved, including their thresholds. No automatic customs routing, stock simulation, cash/debt, tariff, fuel or settlement algorithm changes.
- Startup reports the effective `company thresholds: load above 20% / deliver below 30%`; bounded daily native task audits show actual fields (`0.200` / `0.300` for allowed sides). Historical constructor defaults of 100% are superseded by this writer; active/validated flags and native routing are untouched.

### Validation and deployment

108 tests pass: 11 logistics, 4 shops, 93 probe. The production C++ helper harness now covers legacy 100%/90% and manual 19% replacement, configured threshold combinations with boundary clamping, all direction masks, daily idempotence, resource-vector preservation/denial, and unrelated-byte invariants. Production call-site checks require both appending and reconciliation to pass configured values. The harness passes GCC ASan/UBSan. The updated logistics DLL compiled and linked successfully for Windows x64 using Zig Windows-GNU; its three plugin exports were inspected. This was not an MSVC build or native game execution. Host tests do not prove native dispatch timing or post-delivery inventory limits.

After merging, rebuild **private_logistics only** (`build.bat private_logistics`), deploy its DLL and shipped INI, and fully restart. Probe and shops do not need rebuilding for this patch. Billing remains OFF, auto targets ON/apply, foreign restock OFF, probe ON. Missing new keys also default to 20/30; no manual INI correction is needed.

On a copied save, let one daily reconciliation run. Check startup version and threshold line, source-enabled warehouse `load_pct=0.200`, destination-enabled store `unload_pct=0.300`, and disabled retail load remains off/zero. Wait through several days to confirm values no longer revert to 90/100, observe pickup at a warehouse well below 90% but above 20%, and observe delivery eligibility for a destination below 30%. Native UI global header values are not necessarily the managed per-task policy; inspect each row and the actual task audit. Do not count callback fragments as trips or treat the destination trigger as a spending cap.


## V2 report review and logging-only build — 2026-10-24

The pushed `DO_PERCENTAGE_WAKEUP_FINDINGS_V2.md` was fetched from the actual fuel branch and read in full. Its native-constant bytes support double 0.1, but the report's claim that this is the effective source upper bound is not sufficient to justify changing source defaults: the raw decompilation/assembly establishing the complete bound expression is not committed. A 0.1 constant may be used directly or in a derived bound; do not assume either without the actual use-site dataflow. Its proposed workaround of raising a value before clamping it to the same 0.1 is mathematically ineffective. Also do not collapse separate slider branches into one unconditional operation that toggles directions on every percentage click.

The actual prior attachment was re-read: SHA-256 `07872a414a0bf1ed4fb043228e39484461fad7adb63cf24724ae8929b44266ed`, 2,556 lines. Days 3–6 had config_changed=0. Repeated changes begin on day 7 when the warehouse becomes load-only (unload is denied). This is NOT evidence that a 20% source was clamped on every day. Native normalization of a denied unload percentage is another plausible explanation, not an established diagnosis. Post-write audits cannot distinguish these cases.

**Build v0.18.3-audit is diagnostic, NOT a dispatch fix.** It adds no game-memory writes, new native calls, hooks, UI callbacks, gates or scheduling changes. Existing policy writes and existing dirty/timer writes are unchanged. Read-only snapshots are taken on the existing daily simulation callback, after validating the same entry/config spans as before. They do not retain game pointers across callbacks.

- `[AUTO DO AUDIT] target=... changed_fields=...` reports incoming and outgoing activation/wildcards, nine-significant-digit percentages, and exact float bit patterns. Mask: 0x01 activation, 0x02 load wildcard, 0x04 unload wildcard, 0x08 load percentage, 0x10 unload percentage, 0x20 load selector span, 0x40 unload selector span. This identifies the actual field(s) corrected by our writer rather than guessing from identical final values. Mask zero with a reported change would be a diagnostic discrepancy, not a scheduling instruction.
- `[AUTO DO AUDIT] office=... phase=before-reconcile/before-notify/after-notify` reports guarded reads of dirty and countdown, plus whether a notification was requested. Unreadable dirty is -1; an unreadable timer has timer_readable=0 and its placeholder value must not be interpreted. Daily snapshots do NOT prove per-frame consumption, intermediate state, or root cause.
- All new records share the existing auto_targets_log_limit=80 per reconciliation; 0 suppresses them. No unbounded per-frame logging. Fresh INI and fallback defaults already turn diagnostics on without a manual setting change. Billing remains OFF; source/destination remain 20/30; auto targets remain ON/apply; customs preservation, resource permissions, foreign restocking, probe, fuel and settlement remain unchanged.

Validation: 110 tests pass (13 logistics, 4 shops, 93 probe). New native host tests execute the actual production setter and office logger with pointer/log stubs, checking all direction masks with budgets 0/1/80, idempotence, exact bit changes, memory transparency and unreadable-field markers. Read-only snapshots and field masks are tested directly. The actual setter/logger harness passed GCC ASan/UBSan. The updated DLL compiled and linked for Windows x64 with Zig Windows-GNU; not MSVC or in-game execution. Host tests do not reproduce native dispatch or verify any Ghidra claim.

For the next controlled test, build `private_logistics` only and deploy its DLL (the shipped INI keeps the existing matching test profile). No probe/shop rebuild or manual INI repair is needed. Use a copied save with eligible stock/demand and a usable road/truck. First leave the DO panel closed for two reconciliations, then open it WITHOUT clicking for two reconciliations. Finally record which control is clicked and its before/after value; for a notification-versus-eligibility comparison, pause and use + then - to restore the SAME final value before resuming. Return the full log and note those phase/click times. This audit is already enabled in the shipped defaults, but does not timestamp native UI clicks itself. No claim that movement will be repaired by this diagnostic build.

Interpretation: incoming source 0.1 -> 0.2 with mask 0x08 supports a source-value rewrite; incoming denied unload 0.1 -> 0 with mask 0x10 points to denied-side normalization instead. Other masks expose activation/wildcard/vector changes. Neither observation alone establishes why dispatch failed. If source stays 0.2 and config_changed=0 while idle, source-clamp churn does not explain that interval. Compare office pre/post notification state without changing the countdown policy based solely on daily observations.

### Complete diagnostic helper source for a single-file handoff

The existing validation/policy functions and daily notification writes remain in the repository unchanged. Below are the complete new snapshot header and updated setter/office observation functions. New observation call sites run before reconciliation and immediately before/after the existing notification block; executable regression tests are in `tests/private_logistics/test_task_audit.py`.

```cpp
#ifndef PRIVATE_LOGISTICS_TASK_AUDIT_H
#define PRIVATE_LOGISTICS_TASK_AUDIT_H
#include <stdint.h>
#include <string.h>

// Read-only value snapshots, no retained game pointers. Caller validates entry
// and cfg spans. Compare float representations, not rounded log strings.
struct AutoTaskAudit {
    unsigned char enabled, loadAll, unloadAll;
    uint32_t loadBits, unloadBits;
    uintptr_t loadBegin, loadEnd, unloadBegin, unloadEnd;
};
inline AutoTaskAudit AutoReadTaskAudit(const void* entry, const void* cfg)
{
    const unsigned char* p = (const unsigned char*)cfg;
    AutoTaskAudit a = {};
    a.enabled = *(const unsigned char*)entry;
    a.loadAll = p[0]; a.unloadAll = p[0xC8];
    memcpy(&a.loadBits, p + 4, 4); memcpy(&a.unloadBits, p + 0xCC, 4);
    memcpy(&a.loadBegin, p + 8, sizeof(uintptr_t));
    memcpy(&a.loadEnd, p + 16, sizeof(uintptr_t));
    memcpy(&a.unloadBegin, p + 0xD0, sizeof(uintptr_t));
    memcpy(&a.unloadEnd, p + 0xD8, sizeof(uintptr_t));
    return a;
}
inline double AutoAuditPercent(uint32_t bits)
{
    float f; memcpy(&f, &bits, sizeof(f)); return (double)f;
}
// 1=activation, 2/4=load/unload wildcard, 8/16=load/unload percentage,
// 32/64=load/unload selector span. Diagnostic only: never drives policy.
inline unsigned AutoTaskAuditChanges(const AutoTaskAudit& a, const AutoTaskAudit& b)
{
    return (a.enabled != b.enabled ? 1u : 0u) |
        (a.loadAll != b.loadAll ? 2u : 0u) |
        (a.unloadAll != b.unloadAll ? 4u : 0u) |
        (a.loadBits != b.loadBits ? 8u : 0u) |
        (a.unloadBits != b.unloadBits ? 16u : 0u) |
        ((a.loadBegin != b.loadBegin || a.loadEnd != b.loadEnd) ? 32u : 0u) |
        ((a.unloadBegin != b.unloadBegin || a.unloadEnd != b.unloadEnd) ? 64u : 0u);
}
#endif

static bool AutoSetTargetDirections(void* entry, unsigned directions)
{
    if (!entry || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) return false;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) {
        Logf("[AUTO DO] invalid task config; cannot enforce direction mask=%u", directions);
        return false;
    }
    const bool audit = g_autoLogLines < (unsigned)g_autoTargetLogLimit;
    AutoTaskAudit before = {};
    if (audit) before = AutoReadTaskAudit(entry, cfg);
    const bool directionsChanged = AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    const bool activationChanged = AutoApplyEntryActivation(entry, directions);
    const bool changed = directionsChanged || activationChanged;
    if (changed && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        Logf("[AUTO DO] task directions target=%p load=%u unload=%u entry_enabled=%u (native route validation still required)",
             *(void**)((BYTE*)entry + 0x08), (directions & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (directions & AUTO_ALLOW_UNLOAD) ? 1u : 0u, (unsigned)*(BYTE*)entry);
        ++g_autoLogLines;
    }
    if (changed && audit && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        const AutoTaskAudit after = AutoReadTaskAudit(entry, cfg);
        Logf("[AUTO DO AUDIT] target=%p changed_fields=0x%02X enabled=%u->%u load_all=%u->%u unload_all=%u->%u load_pct=%.9g->%.9g unload_pct=%.9g->%.9g load_bits=%08X->%08X unload_bits=%08X->%08X",
             *(void**)((BYTE*)entry + 8), AutoTaskAuditChanges(before, after),
             (unsigned)before.enabled, (unsigned)after.enabled,
             (unsigned)before.loadAll, (unsigned)after.loadAll,
             (unsigned)before.unloadAll, (unsigned)after.unloadAll,
             AutoAuditPercent(before.loadBits), AutoAuditPercent(after.loadBits),
             AutoAuditPercent(before.unloadBits), AutoAuditPercent(after.unloadBits),
             (unsigned)before.loadBits, (unsigned)after.loadBits,
             (unsigned)before.unloadBits, (unsigned)after.unloadBits);
        ++g_autoLogLines;
    }
    return changed;
}

static void AutoLogOfficePlanState(void* office, uint64_t key, int day,
                                   const char* phase, bool notificationRequested)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    const BYTE* p = (const BYTE*)office;
    const bool dirtyReadable = ReadablePtr(p + AUTO_DO_DIRTY, 1);
    const bool timerReadable = ReadablePtr(p + AUTO_DO_UI_TIMER, sizeof(float));
    float timer = 0.0f;
    if (timerReadable) memcpy(&timer, p + AUTO_DO_UI_TIMER, sizeof(timer));
    Logf("[AUTO DO AUDIT] day=%d office=%p key=%016llX phase=%s notification_requested=%u dirty=%d timer_readable=%u countdown=%.9g",
         day, office, (unsigned long long)key, phase, notificationRequested ? 1u : 0u,
         dirtyReadable ? (int)p[AUTO_DO_DIRTY] : -1, timerReadable ? 1u : 0u, (double)timer);
    ++g_autoLogLines;
}

```


## Latest native log: disabled-percentage churn confirmed — 2026-10-25

Attachment: 5,485-line `tesmioloader.log`, SHA-256 `0c51bfcba19711b65aff4d3f676fc61a02a0021092ab87d81ed9b5649fcd4d63`. This supersedes earlier attachments for current runtime conclusions. The player reports the source was filled fully; do not dismiss that observation or assert it was empty. The log does not include the planner's resource-specific stock/capacity/available-quantity calculation, so the 10%-versus-20% eligibility discrepancy is not resolved.

### Actual evidence

- Source warehouse `0000018FA1A1CB50` repeatedly arrives with load wildcard 1, load percentage 0.200000003, unload wildcard 0 and unused unload percentage 0.100000001. The old writer changes only the unused unload percentage to zero: `changed_fields=0x10`. First example 21:24:40.895; 34 audit records overall have mask 0x10 (not all necessarily this target).
- Audits also have 8 records with mask 0x08 (load percentage only) and 6 with mask 0x18 (both percentages). There are no activation/wildcard/vector changes in these recorded masks. Native or office-wide UI edits to denied retail loading thresholds also caused pointless rewrites to zero.
- At 21:26:44.299 (and other times), incoming source percentage is 0.300000012 before our writer restores configured 0.200000003. This does NOT support a universal assertion that any source value above 10% is automatically clamped to 10%. Manual trials at 30% are also not persistent under the current 20% company policy.
- Dirty/countdown notifications are real: the old policy repeatedly writes dirty=1, countdown=3 solely because an UNUSED percentage differs. Native code sometimes clears dirty between callbacks, and countdowns can be negative. Therefore there is confirmed churn, but complete planner starvation is NOT established.
- Actual loading starts 21:25:42.803; destination delivery starts 21:25:56.993. There are 22 load callbacks (21 committed) and 23 delivery callbacks (all committed), NOT counts of trips. The first source 10% -> 20% normalization is logged at 21:25:38.230; this does not tell us whether a later pickup's order had already been generated while the lower value was active.

### Bounded correction: v0.18.4-denied-threshold

`AutoApplyTaskDirections` now controls percentages ONLY for allowed directions. It leaves a denied direction's existing float bits untouched, while still switching its wildcard OFF and emptying its selector list. The established matcher truth table is wildcard off plus empty list => no resource match; percentage is not an independent access grant. New zero-initialized configs may still start with zero on a denied side, but subsequent native normalization no longer causes daily corrections.

No timer-policy change, forced replanning call, new hooks, cargo changes, blanket direction enablement or resource purchases. Permissions and re-allow still behave as before: re-authorizing a direction restores its configured company threshold, not an arbitrary retained percentage. Existing activation management and customs preservation remain unchanged. Diagnostics remain enabled by the existing 80-line budget; billing stays OFF; source/destination defaults remain 20/30 in INI/static/fallbacks. Earlier historical sections showing denied percentage zero describe older builds, not a required invariant of v0.18.4.

### Regression validation

110 tests pass (13 logistics, 4 shops, 93 probe). Expanded native tests execute the actual production setter for 100 repeated daily reconciliations with observed source-only 20%, denied unload 10%: no change return and no mutation logs, hence no config-change notification requested by that setter. Tests cover nonzero denied source percentages, explicit resource-list revocation without changing the unused float, and re-authorizing back to company 20%. All ownership/migration/wildcard/vector invariants remain tested. Both actual production policy and setter/logger harnesses pass GCC ASan/UBSan. The updated DLL compiled and linked for Windows x64 using Zig Windows-GNU and its plugin exports were inspected; this is not an MSVC build or native game execution. Host tests do not prove native dispatch.

### Deployment and remaining investigation

Rebuild `private_logistics` only, deploy DLL and matching shipped INI, fully restart on a copied save. No manual INI correction: enabled=1, billing=0, dry_run=1, auto targets ON/apply, source20/destination30, diagnostics80 are still the shipped test defaults. Probe and shops unchanged, no rebuild needed.

Check that source-only warehouse audits no longer report unload 0.1 -> 0 every day; allowed source 20% remains controlled, denied unloading remains OFF even if its UI percentage is 10%. If dispatch still needs a 10% source setting with full stock, do not call this a complete fix or reduce defaults merely to hide the discrepancy. We need the raw `planner_424430.c` and `rebuild_1E2380.c` (and relevant callees) to inspect the actual source reserve/available-quantity and destination-demand calculations, not another summary. The current attachment does not justify claiming a particular capacity denominator or minimum truck-load rule. Fuel costing remains deferred.

### Complete corrected production writer

```cpp
#ifndef PRIVATE_LOGISTICS_SITE_PERMISSIONS_H
#define PRIVATE_LOGISTICS_SITE_PERMISSIONS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Directions are from the truck's perspective. No game pointers retained here.
enum { AUTO_ALLOW_LOAD = 1, AUTO_ALLOW_UNLOAD = 2, AUTO_ALLOW_BOTH = 3 };

inline unsigned AutoSiteCapabilities(int type, const char* asset)
{
    // Explicit asset exception: observed warehouse can report shop type 3.
    // Do not use substring "store"/"warehouse" guesses for Workshop assets.
    const char* name = asset ? asset : "";
    for (const char* p = name; *p; ++p)
        if (*p == '/' || *p == '\\') name = p + 1;
    if (strcmp(name, "warehouse_covered_combined") == 0 ||
        strcmp(name, "warehouse_open") == 0) return AUTO_ALLOW_BOTH;
    if (type == 5 || type == 6) return AUTO_ALLOW_BOTH; // storage / factory
    switch (type) {
        // Known retail/service consumer families; never loading sources.
        case 3: case 4: case 10: case 23: case 24: case 25: case 26: case 28:
            return AUTO_ALLOW_UNLOAD;
        default: return 0; // unknown, infrastructure, customs, DO: no guessed role
    }
}

// Native labelled widget polarity established by the user's v0.18 game test:
// raw 0 draws CHECKED; raw 1 draws UNCHECKED. Keep it at the UI boundary;
// saved permission bits and TaskCfg directions retain their logical meaning.
inline unsigned AutoNativeCheckboxValue(bool allowed) { return allowed ? 0u : 1u; }
inline bool AutoNativeCheckboxChecked(unsigned raw) { return raw == 0; }

inline bool AutoPreserveManualCustoms(int type) { return type == 20; }

// Entry activation is separate from direction selectors. Appending an OFF
// entry is not a functioning company connection. Request native activation;
// the engine must still validate roads, vehicle compatibility and orders.
inline bool AutoApplyEntryActivation(void* entry, unsigned directions)
{
    unsigned char* enabled = (unsigned char*)entry;
    const unsigned char wanted = (directions & AUTO_ALLOW_BOTH) ? 1 : 0;
    if (*enabled == wanted) return false;
    *enabled = wanted;
    return true;
}

inline unsigned AutoEffectiveDirections(bool isPrivate, unsigned permissions,
                                        unsigned capabilities)
{
    return (isPrivate ? capabilities : (permissions & capabilities)) & AUTO_ALLOW_BOTH;
}

inline unsigned char AutoMigratePermission(unsigned char value, bool legacy)
{
    // PLSTOR17: 1=LOAD, 2=explicit OFF. Never turn old OFF into UNLOAD.
    if (legacy) return value == 1 ? AUTO_ALLOW_LOAD : 0;
    return value <= AUTO_ALLOW_BOTH ? value : 0;
}

inline unsigned char AutoPermissionToggle(unsigned mask, unsigned direction,
                                           unsigned checked, unsigned capabilities)
{
    return (unsigned char)((checked ? (mask | direction) : (mask & ~direction)) & capabilities);
}

// Company policy bounds (not permission bits): keep source thresholds below
// the native 90% ceiling and use a positive destination replenishment trigger.
inline int AutoClampSourcePercent(int value) { return value < 0 ? 0 : (value > 90 ? 90 : value); }
inline int AutoClampDestinationPercent(int value) { return value < 10 ? 10 : (value > 100 ? 100 : value); }

// The caller validates cfg and both native vector spans before calling.
// Deny = no wildcard AND empty resource selector list, not just threshold=0.
// Keep allocation begin/capacity untouched: native code still owns the vector.
// The private DO owns its task resource selectors; native storage/resource and
// reachability rules remain the planner's responsibility.
inline bool AutoApplyTaskDirections(void* cfg, unsigned directions,
                                    int sourcePercent = 20, int destinationPercent = 30)
{
    unsigned char* p = (unsigned char*)cfg;
    bool changed = false;
    for (unsigned side = 0; side != 2; ++side) {
        const size_t offset = side ? 0xC8 : 0;
        const bool allowed = (directions & (side ? AUTO_ALLOW_UNLOAD : AUTO_ALLOW_LOAD)) != 0;
        const unsigned char all = allowed ? 1 : 0;
        if (p[offset] != all) { p[offset] = all; changed = true; }
        // A denied direction is enforced by wildcard OFF plus an empty list,
        // NOT its percentage. Native UI normalizes disabled unload to 10%.
        // Rewriting that unused value to zero on every day needlessly marks
        // the office dirty and rearms its countdown. Preserve it verbatim.
        if (allowed) {
            const int threshold = side ? AutoClampDestinationPercent(destinationPercent) : AutoClampSourcePercent(sourcePercent);
            const float percentage = (float)threshold / 100.0f;
            float previous;
            memcpy(&previous, p + offset + 4, sizeof(previous));
            if (previous != percentage) {
                memcpy(p + offset + 4, &percentage, sizeof(percentage)); changed = true;
            }
        }
        if (!allowed) {
            uintptr_t begin, end;
            memcpy(&begin, p + offset + 8, sizeof(begin));
            memcpy(&end, p + offset + 16, sizeof(end));
            if (end != begin) {
                memcpy(p + offset + 16, &begin, sizeof(begin)); changed = true;
            }
        }
    }
    return changed;
}
#endif

```
