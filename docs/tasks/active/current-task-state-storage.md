# Current Task: Opt-In, Load-Only State-Storage Targets

**Status date:** 2026-10-22  
**Workstream:** `private_logistics` + `ui_panels`  
**State:** staged candidate; not build- or runtime-verified.

## Goal

Allow a player to opt selected public state-storage buildings into native loading targets for private distribution offices. The feature is strictly **source-only**: it must not make state storage a bidirectional target, must not unload goods into state storage, and must not manipulate inventory. It uses the game's native DO target/task system rather than moving goods itself.

The opt-in belongs to the individual storage building and is player-managed. Automatic candidate selection must respect that opt-in and current native logistics reachability. State-operated DOs remain player-managed; this feature is about an opted-in state-storage building being available as a source to an eligible private DO, not automation of state DOs.

## User-visible behavior and invariants

1. A state-storage building is **not** an automatic source unless the player explicitly enables its state-storage permission.
2. Permission defaults off and is persisted per building by the UI plugin.
3. A generated state-storage task is **load-only**. Native UI direction must reflect loading checked and unloading unchecked.
4. Automation may add a missing target but must preserve and merge existing manual/native targets. It must not delete a task merely because the target is unreachable or the entry is currently inactive. If the player opts a state-storage building in, the feature-managed task for that source must be load-only; unrelated manual target configuration remains untouched.
5. State storage remains source-only regardless of other automation modes. Do not offer a “both directions” state-storage mode.
6. The game retains authority over reachability, route planning, loading, unloading, and physical inventory transfers.
7. No inventory writes, simulated stock changes, or teleportation are permitted.
8. New automatic targets must not exceed the staged conservative five-entry total per DO (existing plus generated); this is a plugin safety policy, not a proven game maximum.
9. With `auto_targets_dry_run = 1`, no new task entries or TaskCfg direction edits are allowed.
10. Existing state-storage permission and the target list must survive save/load.

## Staged implementation snapshot

The current staged candidate in `/home/user/implementation` contains:

- `private_logistics` v0.17 and `ui_panels` v6.11 candidate sources/configuration.
- A per-storage permission key and UI control, with permission modes designed to distinguish default/off, source-only, and explicit off behavior. The control is opt-in.
- State-storage candidate integration in automatic target selection, with state-storage candidates considered before ordinary private-industry candidates.
- Load-only TaskCfg setup for generated state-storage targets: the staged setup clears/unsets unload behavior (`+0xC8` and `+0xCC`) and uses native default/wildcard resource configuration. Exact native semantics remain subject to the native-panel/runtime check.
- Preservation/merge behavior for existing target entries; the latest staged code initializes generated task entries inactive and relies on the game's native reachability/upkeep path to activate them.
- A conservative cap of five total entries per DO, counting already-present/manual entries and prospective automatic additions.
- Detailed candidate/logging behavior and save/load notes in the staged plugin sources.

This is a staging description only. It does **not** establish that the files were copied to the Windows game checkout, that the game loaded the DLL, or that the UI/task behaves correctly at runtime. The external Windows tree restoration status is unknown.

## Configuration: two separate dry-run controls

The staged `private_logistics.ini` has distinct settings:

| Setting | Staged value | Meaning |
|---|---:|---|
| `enabled` | `1` | Plugin enabled in the staged configuration. |
| `billing` | `1` | Transfer billing feature enabled in the staged configuration. |
| `dry_run` | `0` | Billing dry-run is off in the staged configuration; if deployed and loaded, billing would not be dry-run. This has not been runtime-verified. |
| `factory_share` | `0.90` | Staged producer payment share of retail price. |
| `trip_tariff` | `1` | Staged trip-based carrier billing enabled. |
| `auto_targets_enabled` | `1` | Automatic target feature enabled in configuration. |
| `auto_targets_dry_run` | `1` | **Automatic target writes remain dry-run/blocking.** No generated targets or TaskCfg changes should be made. |
| `diesel_density` | `0.00100` t/L | Staged conversion constant for physical fuel accounting. |

Do not conflate billing dry-run with automatic-target dry-run. Billing configuration is staged as live (`dry_run = 0`), while target automation remains dry-run. Since there has been no build/deployment/runtime check, these values describe the staged INI only, not a running game.

## Why this design follows the Ghidra findings

- DO task entries are native vector records (`do + 0xD38/+0xD40`, `0x18` stride) with enabled byte, target pointer, and `TaskCfg*`.
- Native target creation also initializes config, appends through a vector helper, marks dirty, and schedules processing. Calling the UI frame handler as an add API is unsafe.
- Save/load reconstructs native tasks; the plugin must use a task layout compatible with that lifecycle and be tested through a real save/reload.
- Reachability is planner- and connection-dependent. Type 3 is road, type 6 rail-specific. A rail DO may qualify through road connections; do not assume type-6-only behavior.
- Reachability disabling does not equal explicit removal. Preserve inactive/unreachable manual tasks.
- No global reservation across DOs is established. Start with one DO.
- The panel checkbox buffer at `panel + 0x310 + idx * 4` is not proven safe above five, so do not raise the staged cap based on a displayed row count or observed vector length.

See [`ghidra-do-storage-findings.md`](../../ghidra-do-storage-findings.md) for function names, offsets, superseded conclusions, and the full analysis caveats.

## Acceptance criteria

### UI and permission

- [ ] The state-storage checkbox appears only where intended and does not affect unrelated building panels.
- [ ] Default is off for existing and newly encountered storage buildings.
- [ ] The player can opt a selected storage in and later opt it out.
- [ ] Permission survives save/load and does not reset when the panel is reopened.
- [ ] The native DO panel confirms generated state-storage targets are load-only (loading enabled; unloading disabled).
- [ ] The control does not imply that a state-operated DO is automatically managed.

### Candidate discovery and task behavior

- [ ] Only player-opted-in storage buildings qualify as state sources.
- [ ] Candidate requires native-compatible road reachability; test road DO and rail DO via road connection separately.
- [ ] Test type-6-only rail connectivity separately; do not assume it satisfies the type-3 road planner condition.
- [ ] Automatic candidates are state-first, then normal private-industry candidates, without overwriting the user's existing list.
- [ ] Existing native/manual targets remain in the vector when disabled or temporarily unreachable.
- [ ] Duplicate targets are not appended; behavior is stable across repeated reconciliation passes.
- [ ] Existing/manual targets plus generated targets never exceed the staged total-five policy.
- [ ] Multiple DOs are not assumed to reserve or exclusively claim a source.

### Safety, persistence, and release

- [ ] `auto_targets_dry_run = 1` causes zero task-vector writes and zero TaskCfg direction edits.
- [ ] No inventory, stock, or goods-transfer state is modified by the plugin.
- [ ] A target added in a disposable save survives save/reload and remains load-only.
- [ ] Manual target-list changes are preserved on the next pass; opted-in state-storage tasks remain load-only by contract, while unrelated manual TaskCfg directions are not rewritten.
- [ ] Full Windows/MSVC build succeeds against the actual game headers/runtime.
- [ ] In-game panel behavior is verified on road and rail DOs; logs corroborate candidate and write decisions.
- [ ] Only after the above checks may a deliberate deployment decision change `auto_targets_dry_run` from `1` to `0`.

## Known blockers and open questions

1. **Asset mode:** The mode of `warehouse_covered_combined` has not been inspected/verified. Do not infer that its name or building type makes it a source.
2. **Live connections:** Actual roads, path graph, and road-versus-rail connection records in the user's save are unverified.
3. **UI checkbox bounds:** Allocation and bounds for per-target checkbox state above index four remain unknown. Current five-entry policy is a safety limit, not an engine limit.
4. **Native config semantics:** Verify wildcard/default and empty load/unload resource lists in the native panel and live DO.
5. **Cross-DO contention:** No global reservation guarantee has been demonstrated.
6. **External deployment:** The staged source is not proven to be the source used by the Windows game installation.
7. **Build/test:** No full Windows plugin build, UI test, live transfer test, or save/load cycle has been performed.

## Release gate / immediate next steps

1. Reconcile staged files with the actual Windows tree; preserve backups and compare versions/hashes. Do not claim the tree is restored until verified.
2. Inspect the exact storage asset mode and connection configuration in the user's game files/save.
3. Complete static investigation of `panel + 0x310` allocation and all checkbox accessors before changing the five-entry policy.
4. Build with the real Windows toolchain; compile and inspect logs.
5. Run a disposable-save test with one state-storage source and one road DO. Verify direction in the native panel, then test save/load.
6. Add a rail DO with road connection; only later test type-6-only rail paths and multiple DOs.
7. Keep automation dry-run on until all checks pass; never test by manipulating inventory.
