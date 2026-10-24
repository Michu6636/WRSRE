# Private DO wakes only after a percentage click — investigation

## Status

2026-10-24. Native lifecycle regression remains unresolved. This report preserves the new log evidence and a narrowly scoped RE request. No runtime source, settings or offsets have been changed for this investigation. Do not advance to fuel settlement or describe percentage-field tests as proof of autonomous native scheduling.

## Latest attachment

`tesmioloader.log`: 2,556 lines, SHA-256 `07872a414a0bf1ed4fb043228e39484461fad7adb63cf24724ae8929b44266ed`.

- 20:35:16.156: configured company thresholds load above 20%, deliver below 30%; test profile billing=0, auto_targets=1, apply=1. This is not a failure to deploy the threshold defaults.
- Day 2, 20:37:58.593: private office `83DDA9FCBA29D771` adds three targets. Subsequent days see all three; no invalid target/config summary errors.
- Day 3 through day 6: native audits show enabled=1, allowed load percentage 0.200 and allowed unload percentage 0.300. Config changes are zero on those days.
- Day 7 onward: state warehouse changes from both permissions to load-only. Daily reconciliation records config_changed=1 repeatedly even when the resulting audit has identical visible values. The existing audit is AFTER normalization and cannot show which input byte/float was changed by native code or interaction. Do not infer a particular cause from identical output lines.
- Warehouse target `0000022416F97ED0`; store targets `0000022416F90070` and `0000022416F93FA0`.
- Vehicle `00000223F56EC4B0`, home DO `0000022416F993E0`: all 107 probe measurement records have state=0 and cargo total=0. No `class=load` or `class=delivery` records occur in this attachment. This does not negate the player's observed brief movement; movement may occur between samples, and state=0 has not been independently established as a complete motion indicator.
- Player reports that clicking a native percentage control causes movement. The attachment does not timestamp that click, identify which +/- control was used, capture stock/capacity at the click, or show native handler side effects.

## What source inspection establishes

`AutoApplyTaskDirections` writes the wildcard byte, percentage float and (for denial) resource-vector end. `AutoApplyEntryActivation` writes entry byte zero to 1/0 according to effective permissions. Append also constructs TaskCfg and sets the reported config flags at +0x28 and +0xF0. Both appending and existing-target reconciliation pass the configured 20/30 policy.

At the end of `AutoTargetsOnDay`, if any target was added OR any config/activation changed, code already does:

```cpp
if (doAdded > 0 || doConfigChanged) {
    if (ReadablePtr((BYTE*)office.building + AUTO_DO_DIRTY, 1))
        *((BYTE*)office.building + AUTO_DO_DIRTY) = 1;
    if (ReadablePtr((BYTE*)office.building + AUTO_DO_UI_TIMER, sizeof(float)))
        *(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;
}
```

`AUTO_DO_DIRTY=0xD50`, `AUTO_DO_UI_TIMER=0xFC0`.

Therefore **“we forgot the dirty flag” is not an established explanation**. Earlier evidence ties those writes to native target addition, not necessarily to the complete native percentage-change commit operation. Nor is it proven that the field named UI_TIMER is a countdown or that 3.0 has the same meaning for every caller. Do not blindly re-arm it every frame/day, perturb thresholds, or call a large UI function to emulate a click.

## Working hypotheses, NOT conclusions

1. A native percentage change commits additional config/resource data, invalidates cached work, or requests scheduling in a way our direct writer does not reproduce.
2. The +/- operation changes real stock eligibility (e.g. crossing a source/destination boundary), rather than waking a stalled planner. We do not have stock/capacity snapshots to exclude this.
3. The native engine/UI clamps or rewrites a denied percentage or another managed field, causing repeated policy writes and native invalidation. Current post-write audits cannot distinguish the changed field. Repeated invalidation could matter, but starvation/cancellation is not proven.

The earlier claim that we only needed the known percentage fields was too strong as a claim about complete autonomous task execution. The fields themselves are confirmed by UI/log correlation; the native lifecycle is not yet reproduced/proven.

## Narrow Ghidra handoff — investigation only

Use the existing Windows pre-analyzed project for SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD, base 0x140000000. This Arena checkout has neither the executable nor that Ghidra project; no native disassembly was executed here. Read `docs/ghidra-do-storage-findings.md`, `docs/engine/logistics.md` (respect its supersession notes), `plugins/private_logistics/site_permissions.h`, and the functions named above before investigating.

Use the Windows workspace's `scratch\\ghidra_run.bat <Script.java> <out_file> [args...]`, existing Java DecompAt/Xrefs/DisasmMany scripts, one headless invocation at a time. Keep temporary scripts and raw outputs under scratch. If unavailable, report the actual command/error rather than inventing analysis. Do not change runtime source, INIs, build scripts or saves.

### Questions to resolve

1. Find the actual source/destination +/- percentage interaction for individual targets AND office-wide controls. Starting points: road panel builder RVA 0x741170; rail builder 0x745350; interaction handler 0x7496C0. These are starting points, not declarations that all controls live in the interaction handler. Trace helpers and call sites as needed.
2. Give exact before/after writes and helper calls for one percentage change. Which TaskCfg fields, selected-resource entries, office fields, global flags or cached work lists change beyond cfg+0x04 / +0xCC? Distinguish rendering and transient widget state from simulation state.
3. Trace readers/writers of office+0xD50 and +0xFC0 along the actual DO tick path to planning. Is FC0 elapsed time, a delay, UI-only, or something else? What exact conditions consume/reset the dirty flag and schedule work? Does repeated invalidation cancel/postpone orders?
4. Compare a native UI-created working task with our constructor + policy writer. Verify the meaning of cfg+0x00, +0x28, +0x8C, +0xC8, +0xF0, and empty wildcard resource vectors only insofar as required to explain the difference. Do not blindly inherit old labels such as active/validated.
5. Starting points for consumers: planner RVA 0x424430, reachability 0x422C30, connection rebuild 0x191950. Determine the minimal safe commit/invalidation operation after percentage/direction changes, with ABI, required context, calling thread, executable guards and ordering. The entire UI handler is NOT a safe callable API; do not recommend calling it from a day callback.
6. Verify native clamping of DISABLED direction percentages: can zero be normalized to a positive minimum even when the wildcard is off and the selector vector is empty? Could this explain repeated config_changed=1 with unchanged post-normalization audit output? State evidence, not speculation.

### Required return: all findings in ONE Markdown file

Return `DO_PERCENTAGE_WAKEUP_FINDINGS.md` with CONFIRMED / CONTRADICTED / UNVERIFIED labels, exact commands actually run, RVAs and bounded relevant disassembly/decompilation, exact native percentage-change pseudocode, complete source of any query scripts used, actual results, and the minimal implementable integration recommendation (or explicit blocker). Do not report only another summary of existing documents. No fuel/pricing/settlement changes; billing remains OFF. Send the single Markdown file back for concrete C++ integration.

## Acceptance after an evidence-backed fix

Copied save, deployed test defaults unchanged: billing OFF, automatic targets ON/apply, source=20, destination=30, probe ON, foreign restock OFF. With known eligible stock, demand and a usable truck/road, generated tasks must initiate work without opening the DO panel or clicking any percentage. Confirm repeated deliveries after cargo is replenished/consumed, not merely one movement after a UI event. Test changes and save/load without resetting live orders or cargo. Existing host tests are useful for masks/memory bounds, not proof of native scheduling.
