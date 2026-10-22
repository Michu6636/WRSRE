# Ghidra Findings: Distribution Offices and State Storage

**Updated:** 2026-10-22  
**Target:** `SOVIET64.exe` v1.1.1.9, PE timestamp `0x6A3EB6AD`, ASLR enabled. Addresses below are RVAs unless written as absolute loaded addresses; image base `0x140000000`.

## Scope and confidence

This is a synthesis of Ghidra findings supplied in conversation, not an assertion that this Arena session independently ran Ghidra. Findings are static analysis of the target executable. They do not establish the live asset configuration, a save's actual road graph, successful plugin compilation, or behavior in a running game.

The latest supplied report is authoritative where earlier reports conflict. The external Windows workspace and `scratch\\ghidra_run.bat` were not available in this session. If further analysis is delegated, follow the workspace's investigation-only rule: use its wrapper, run Java scripts serially, save outputs under scratch, report exact errors, and do not edit code/config/build/docs as part of the Ghidra investigation.

## Executive conclusions

1. A distribution office (DO) stores a vector of native task entries. The relevant entry has an enabled byte, a target-building pointer, and a `TaskCfg*`.
2. Native target creation is more than writing a pointer: it allocates/constructs TaskCfg, appends a task entry, marks the DO dirty, and schedules processing. The observed UI frame handler is not a safe general-purpose API.
3. Task entries and configs are serialized and reconstructed on load. A later connection-rebuild function is not the task restore function.
4. Storage is reachable only when the connection graph satisfies the planner's connection-type requirements. The latest report identifies type 3 as road and type 6 as rail; a rail DO may still use a road connection.
5. Reachability disabling is distinct from removing/freeing a task. Automation must retain and merge manual targets rather than pruning them because they are currently unreachable.
6. Five entries appeared in one observed native add flow, but there is no proven global five-target engine cap. The checkbox-storage bounds above five remain unknown. The staged plugin therefore uses a conservative **policy** cap of five total existing-plus-automatic entries per DO.
7. The actual `warehouse_covered_combined` asset mode and live building connections have not been verified. Do not deploy automatic state-storage sourcing until both mode/direction and road reachability are checked in-game.

## DO task data

| Item | Finding |
|---|---|
| DO task vector | `do + 0xD38` (begin) and `do + 0xD40` (end) |
| Entry stride | `0x18` bytes |
| `TaskEntry + 0x00` | Enabled/active byte |
| `TaskEntry + 0x08` | Target building pointer |
| `TaskEntry + 0x10` | `TaskCfg*` |
| TaskCfg size | `0x198` bytes |
| TaskCfg constructor | `FUN_1401512A0` (RVA `0x1512A0`) |
| DO dirty marker | `do + 0xD50` |
| Processing/scheduling field | `do + 0xFC0` |

The vector helper observed for native append is `FUN_14008E050`. The native creation path initializes a `TaskCfg`, constructs the entry, pushes it into the vector, marks the DO dirty and schedules processing. Exact object lifetime and allocator assumptions remain important: a plugin should not copy only the visible pointer/entry bytes and omit native initialization.

### TaskCfg direction fields

| Offset | Meaning reported | Use in current design |
|---:|---|---|
| `+0x04` | Load amount/flag area | Load behavior is configured here with the game's native representation. |
| `+0xC8` | Unload flag | Set to zero for load-only state-storage targets. |
| `+0xCC` | Unload percentage | Set to zero for load-only state-storage targets. |

The staged native-target setup uses empty load/unload resource lists plus wildcard/default behavior. The exact meaning of each serialized resource-list combination should be confirmed against the native UI and an in-game task before relying on it as a universal recipe. Do not reinterpret these fields based solely on names.

## Native UI and target creation

- `FUN_1407496C0` (RVA `0x7496C0`) is the native per-frame DO UI handler. It includes button/selection logic; it is **not** a standalone plugin-facing target-add API.
- The observed construction flow allocates/configures a target task, calls the TaskCfg constructor, pushes through `FUN_14008E050`, sets the DO dirty marker at `+0xD50`, and schedules work via `+0xFC0`.
- Native direction controls allow a player to select loading and unloading. Do not state that a per-target direction control is absent.
- The native UI callback is not an appropriate function to invoke as a substitute for understanding/replicating the native append lifecycle.

## Save/load

The later report traced task persistence and supersedes the earlier statement that a serializer had not been located.

- `FUN_1406C5350` serializes TaskCfg data.
- `FUN_1406C6040` deserializes TaskCfg data.
- `FUN_1401F0BE0` reconstructs the DO task vector, including entries and resolved targets, during load.
- The vector is resized with the native helper `FUN_1401FEF00`; TaskCfg instances are constructed with `FUN_1401512A0`.
- `FUN_140191950` rebuilds connections; it is not the function that reconstructs DO task entries.

**Implication:** save/load persistence is part of the native task lifecycle, but a plugin-created entry still needs the correct native structure and must be tested through an actual save/reload. Static serialization evidence is not an in-game persistence test.

## Removal, reachability, and manual task preservation

- `FUN_140162B40` was identified as a vector-removal path and `FUN_140162B00` as freeing/removal support. Explicit removal is not the same as a target being unreachable.
- The reachability/upkeep path can change the entry's enabled state without deleting the target/task. A disabled-but-retained entry may be re-enabled when reachable again.
- The entry's enabled byte is therefore not an ownership marker. Do not delete a manual task just because it is disabled, temporarily unreachable, or absent from the automation's current desired set.
- Automation should merge: preserve native/manual entries, avoid adding duplicates, and only alter tasks it can identify as its own or whose behavior the user explicitly selected.

## Reachability and road/rail

The planner uses connection records near building `+0xA10`, with stride `0x60`. The reported path requires connection type 3 for the relevant road-connected planner case. The latest report identifies:

- Type 3: road connection.
- Type 6: rail-specific connection.
- DO building types observed: road DO type 43; rail DO type 52.
- State-storage building type observed: type 5.
- Customs type 20 is separate and is not the state-storage target type.

A rail DO is not automatically unreachable from a type-5 state-storage building: it can qualify through a road connection. A rail-specific type-6 route is distinct and should not be assumed to satisfy the type-3 planner condition. Confirm actual live connections and the target's route in a test save.

The storage parser's import/export flags are asset/configuration inputs, not proof of live DO reachability. The reported code checks connection type for planner reachability; it does not establish that every storage asset or every mode is usable as a source.

## Asset mode and direction: still unresolved

The storage mode parser was identified at `FUN_1400E40F0` (RVA `0x0E40F0`). The reported mode/flag table includes the following examples:

| Mode | Reported label/behavior | Import/source flag (`+0x84`) | Export/sink flag (`+0x85`) |
|---:|---|---:|---:|
| `0` | `$STORAGE`, neutral | 0 | 0 |
| `1` | Special, neutral | 0 | 0 |
| `2` | Basic demand, bidirectional | 1 | 1 |
| `3–5` | Demand variants, bidirectional | 1 | 1 |
| `6` | Import/source | 1 | 0 |
| `7` | Export/sink | 0 | 1 |
| `8–9` | Special import/export variants | 1 | 1 |
| `0xA` | Hotel demand | 1 | 1 |
| `0xB` | Fuel import/source | 1 | 0 |
| `0xC` | Car-plant import/source | 1 | 0 |
| `0xE–0xF` | Special import variants | 1 | 1 |
| `0x10–0x11` | Trash variants, neutral | 0 | 0 |

This table is a reverse-engineering aid, not a substitute for examining the exact target asset. The actual mode of `warehouse_covered_combined` remains **unverified**. Inspect its asset/config data and confirm native loading behavior before enabling it as an automatic source. Do not change inventory or force transfers as a test.

## Counts and UI checkbox storage

- One observed native add flow handles at most five targets in that session. This is **not** proof of a hard native vector limit.
- Other observed UI loops/rows do not establish safe storage for an arbitrary count.
- Per-target checkbox state is accessed at `panel + 0x310 + idx * 4`. The bounds/allocation size for indices above five have not been proven safe.
- Current staged policy: do not generate additions that would take a DO above five total entries, counting preserved existing entries. This is a conservative safety ceiling, not a claim about the engine's maximum.
- Investigate allocation size and every producer/consumer of the checkbox array before raising the cap. In particular, test road and rail panels separately.

## Multi-DO selection risk

No safe, global reservation or exclusive-target mechanism was established. Multiple DOs could potentially select the same state-storage source. Start testing with one DO, one source building, and one known reachable road route. If the game permits another DO to select the same source, decide whether to implement advisory logging, coordinated assignment, or leave the behavior to native logistics; do not claim exclusive sourcing without evidence.

## Superseded conclusions

| Earlier conclusion | Latest conclusion to use |
|---|---|
| Task serialization could not be located. | Save/load reconstruction is reported in `FUN_1401F0BE0`; config serializer/deserializer are `FUN_1406C5350` / `FUN_1406C6040`. |
| Type 3 meant rail. | Latest report identifies type 3 as road and type 6 as rail. |
| `FUN_1407496C0` is a callable writer. | It is a native UI frame handler, not a safe standalone API. |
| Disabling/pruning and explicit removal are equivalent. | Reachability may disable while retaining; explicit remove/free is a separate flow. |
| Five targets is a proven engine cap. | Five is an observed flow count; panel array bounds and hard engine maximum are unresolved. |

## Next static-analysis questions

1. Prove the allocated/initialized size and maximum valid index of `panel + 0x310 + idx * 4` for both road and rail DO panels.
2. Trace all checkbox array writers/readers, including panel rebuild and teardown.
3. Inspect the exact `warehouse_covered_combined` asset mode and resource-list defaults in the user's Windows checkout.
4. Confirm whether duplicated targets are tolerated by native UI and whether duplicate checkboxes share state.
5. If Ghidra is run again, use the Windows workspace wrapper, keep scripts serial, and report exact output/errors. Investigation must not modify project source, config, docs, or builds.

## Runtime validation still required

- Build with the project's actual Windows/MSVC toolchain.
- In a disposable save, verify storage opt-in defaults off and load-only direction is visible in the native panel.
- Verify road DO and rail DO reachability through road connections; separately test type-6-only rail reachability.
- Save/reload and verify manually added and automation-added tasks remain intact.
- Confirm disabled/unreachable tasks are retained, not removed, then re-enabled when reachable.
- Test one DO first; observe duplicate-source contention before expanding to multiple offices.
- Confirm no inventory manipulation occurs and no target writes happen while `auto_targets_dry_run = 1`.
