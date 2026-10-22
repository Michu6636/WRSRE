# Superseded Conclusions Index

Updated 2026-10-22. Table of earlier findings now replaced.

| Earlier conclusion | Latest conclusion to use | Source |
|---|---|---|
| Task serialization could not be located. | Save/load reconstruction in `FUN_1401F0BE0`; config serializer/deserializer `FUN_1406C5350`/`FUN_1406C6040`, builder `FUN_1401EDC40`, helper `FUN_1401FEF00`, ctor `FUN_1401512A0`. | `docs/ghidra-do-storage-findings.md` |
| Type 3 = rail | Type 3 = road, Type 6 = rail-specific. Rail DO can use road connection. | `ghidra-do-storage-findings.md` |
| `FUN_1407496C0` callable add API | Per-frame DO UI handler, not safe to call from day callback; reproduce allocation/vector path (`0x18` Entry + `0x198` TaskCfg via `0x08E050`, `+0xD50=1`/`+0xFC0=3.0`). | `ghidra-do-storage-findings.md` |
| DO vector stride 8 pointer vector | Vector stride `0x18` Entry `{+0x00 byte enabled 0=off, +0x08 target*, +0x10 TaskCfg*}`, TaskCfg `0x198`. | `ghidra-do-storage-findings.md` |
| `TSM_GO_BUILDINGS_VEC = 0xC2C8` | `0x11B08/0x11B10` per `src/game_world.h`. | `DEVELOPMENT_README` Rev4 |
| Task 6 Dropped | Task 6b Bounded Source-Only Staged v0.17, Task 6 broad fleet remains dropped. | `WRSRE DESIGN DOC` Rev4, `current-task-state-storage.md` |
| Panel safe above 5 | `panel+0x310+idx*4` allocation unproven above 5; conservative cap 5 policy, not engine max. | `ghidra-do-storage-findings.md` / `NEXT_PHASE.md` |
| Native add_flow 5 = engine cap | 5 observed in one add session, not proven global; vector/planner/save iterate all. | `ghidra-do-storage-findings.md` |

See `docs/ghidra-do-storage-findings.md#superseded-conclusions` for full narrative.
