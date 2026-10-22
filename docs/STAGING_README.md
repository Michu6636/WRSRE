# Storage source-only staging package — not a compiled release

Contains staged candidate sources/configuration for `private_logistics` v0.17 and `ui_panels` v6.11, plus incremental diffs and updated review notes. No DLLs are included.

Important: these files are staged in the Arena workspace only. The recovered `ui_panels` baseline and the external Windows tree have not been confirmed restored. Review/merge against the actual codebase before deployment. Do not treat this package as build- or runtime-verified.

Safety: `private_logistics.ini` keeps `auto_targets_dry_run = 1`, so generated target/task writes are blocked. Keep it enabled until the v6.11 UI hook, checkbox persistence, warehouse asset mode/connection, and load-only behavior have been validated. The latest Ghidra report found the building save serializer (`FUN_1401EDC40`) and confirmed that load (`FUN_1401F0BE0`) resizes the vector, allocates/reconstructs TaskEntry/TaskCfg, and resolves target pointers. Native target save/load is therefore confirmed in the reported code path. The panel's per-target array capacity above five remains unknown; the staged plugin conservatively limits new automatic additions to five total targets per DO. Keep dry-run enabled until a Windows build and disposable-world test.

Required runtime pairing: deploy the v0.17 private-logistics source/INI together with the v6.11 ui-panels source/INI. The existing project `src/ui_panels_api.h` must match the staged API usage; no shared header replacement is included.

Behavior: the public type-5 state-storage checkbox defaults off. Explicit opt-in adds/retains a load-only target (`TaskCfg+0x04=1.0f`, `+0xC8=0`, `+0xCC=0.0f`). Explicit opt-out disables both directions on that matching state-storage task. Default-off leaves existing manual state-storage tasks unchanged. Other target types and native logistics are preserved; customs remains diagnostic-only; no inventory is manipulated.

Reachability nuance from the latest Ghidra report: the type-5 planner check requires a road connection (type 3), not a rail connection. Road DOs and regular rail DOs using road connections can reach it; rail-specific DOs using type 6 cannot. The actual `warehouse_covered_combined` asset mode/connection still requires INI and runtime verification.

The Ghidra request in `plugins/private_logistics/GHIDRA_REQUEST.md` is read-only and focuses on resolving the remaining panel target-array capacity above five.
