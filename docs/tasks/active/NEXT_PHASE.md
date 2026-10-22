# State-storage source-only targeting — staged v0.17 status

## Requirement

Give the player an opt-in checkbox on each public state-storage panel. When enabled, eligible private distribution offices may **load from** that storage; they must never **unload into** it. Preserve native logistics and existing/manual targets, do not move or edit inventory, and leave customs assignment diagnostic-only.

## Staged implementation

- `../ui_panels/ui_panels.cpp` is staged as v6.11 with a type-5 storage panel hook at builder RVA `0x4CABD0`, installed through dispatcher call site RVA `0x6EA80C`; expected call bytes are `E8 BF 03 DE FF`. The staged INI enables `storage`.
- `private_logistics.cpp` is staged as v0.17. It registers a per-storage checkbox through the `ui_panels` widget API. Permission is keyed to stable building identity and saved per game; pointers are not persisted. New state-storage targets are opt-in only.
- Load-only TaskCfg is `+0x04=1.0f`, `+0xC8=0`, `+0xCC=0.0f`. Explicit opt-out zeros both directions on the matching state-storage task. Default-off does not rewrite existing manual state-storage tasks. Other target types are untouched.
- TaskEntry initial enabled byte is `+0x00=0`, matching native add. Native upkeep sets it to 1/0 from reachability. The staged appender uses native allocation, the TaskCfg constructor, and native vector push helper; it sets the target/task config and dirty fields. It does **not** call `FUN_1407496C0`, which the latest report identifies as a per-frame UI handler rather than a callable add API.
- The plugin enforces a conservative cap of five total entries per office for **new automatic additions**. This is not claimed as an engine hard cap: it avoids exceeding the highest panel target count the report considers proven safe while the backing panel checkbox array capacity remains unknown. If manual entries already use the budget, the plugin preserves them and skips new additions. Opted-in state storages are prioritized over private-site candidates within the budget.
- Target additions/config changes set `building+0xD50=1` and `building+0xFC0=3.0f`. Explicit player removal frees an entry; reachability upkeep can also disable an entry in place. Deduplication checks target pointers so disabled-but-retained entries are not duplicated.
- Private-storage targeting keeps native bidirectional defaults. No inventory is manipulated. Customs remains diagnostic-only.
- `private_logistics.ini` keeps `auto_targets_dry_run = 1`; target additions and TaskCfg writes are blocked until testing is deliberate.

## Latest Ghidra findings

### Save/load and lifetime — reconstructed by native load path

- Save serializer `FUN_1401EDC40` (RVA `0x1EDC40`) writes the DO target count and per-entry target/config data; `FUN_1406C5350` (`0x6C5350`) serializes TaskCfg fields.
- Load `FUN_1401F0BE0` (`0x1F0BE0`) resizes the DO vector via `FUN_1401FEF00`, allocates TaskEntry (`0x18`) and TaskCfg (`0x198`), calls the TaskCfg constructor/deserializers, resolves the target from a saved index, sets TaskEntry+0 to 1, and marks the DO dirty/timer. Therefore the latest report confirms native entries are reconstructed; no separate plugin post-load reconstruction hook is needed for entries using this native format.
- `FUN_140191950` rebuilds connection lists; the DO tick/upkeep handles reachability. The staged persistence callback still stores only checkbox permissions and resets the session memo after load.

### Native add and target activation

- `FUN_1407496C0` is the DO panel's per-frame UI handler, not a simple add API; it depends on world/panel/input/gui state. Do not call it directly from the daily callback.
- Reproducing the allocation/vector operation is the reported safe approach: allocate TaskEntry/TaskCfg, run the constructor, set fields, push `TaskEntry*` with `FUN_14008E050`, then set dirty/timer. New entries start with enabled byte 0; upkeep later sets reachability status.
- Native explicit remove (`FUN_140162B40`/`FUN_140162B00`) removes/frees an entry. Reachability upkeep sets `TaskEntry+0x00` to 0/1 in place; it does not free the disabled entry.

### Target-count bound and dispatch risk

- Vector, panel-render, upkeep, planner, and save loops were reported to iterate all targets. Native add-flow stops after five targets in one add session, but that alone does not prove a global five-target engine cap.
- The per-target panel control array at `panel+0x310+idx*4` has no bounds check; panel allocation/capacity is unknown. Counts above five therefore remain **UNVERIFIED** for UI safety. The staged plugin conservatively refuses new auto-additions once the total reaches five; it never removes manual targets.
- Each DO plans independently without a global order reservation. The current policy can assign the same opted-in source to multiple private DOs, producing duplicate load orders/over-collection. The first live test should involve one private DO and one source; broader assignment needs measured behavior or narrower assignment policy.

### Type-5 reachability and asset

- Type-5 storage requires a road connection (type 3), not rail type 6. Connection records are at `building+0xA10`, stride `0x60`, type at offset 0. Road DOs and regular rail DOs using road connections can reach it; rail-specific DOs using type 6 cannot.
- `warehouse_covered_combined` mode and actual connections still require asset/runtime evidence. Inspect its INI and live-probe `building+0x790` mode, `+0xA10` road connection, and storage records near `+0x970` (import flag `record+0x84`).

## Checks completed in this staging pass

- Delimiter/balance scan passed for both modified C++ files.
- `g++ -fsyntax-only` passed on isolated extracts of the storage-callsite installer, permission/widget/persistence/TaskCfg helpers, and the new capped target-selection loop with Windows/API stubs. This is not a full plugin compile.
- Static checks confirmed source-only TaskCfg values, initial TaskEntry disabled state, the conservative five-entry cap, storage-widget registration, callsite bytes/target arithmetic, and `storage` INI enable token.
- No Windows/MSVC-compatible compiler is available here. No complete plugin build, game launch, UI interaction, save/load cycle, or delivery test has been performed.

## Remaining validation

1. Build both plugins with the project's Windows toolchain. Verify the type-5 hook calling convention, continuation, protection restore, and registration order.
2. Keep dry-run on; confirm the storage checkbox appears only for public type-5 panels, starts unchecked, and persists as a permission preference. Confirm dry-run logs candidates but adds no targets and changes no TaskCfg.
3. Verify the actual `warehouse_covered_combined` INI/mode and road-connection/import flags. Do not infer these from executable code.
4. In a backed-up disposable world, test one road-connected storage and one private DO: load from it, never unload into it, explicit opt-out disables both directions, and save/load restores the target/config. Test a regular rail DO only if it uses road connection type 3; rail-specific type-6 depots are ineligible.
5. Exercise 5 and >5 target UI counts only in disposable worlds; the plugin itself will not add beyond five, but existing/manual lists can be larger and their panel-array safety is unresolved.
6. Measure duplicate-order behavior before enabling the same source on multiple private DOs. Confirm the deployed INI path; earlier runtime logs reported dry-run off while the staged INI says `1`.

## Scope boundary

Do not enable customs assignments, alter inventory, or change existing manual targets merely to make this feature work. Preserve state DO player management. Ghidra work is investigation-only and must not edit source/config/build/documentation files.
