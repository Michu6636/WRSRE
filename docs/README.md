# WRSRE Mod Documentation

**Updated:** 2026-10-22  
**Game target:** Workers & Resources: Soviet Republic, `SOVIET64.exe` v1.1.1.9, PE timestamp `0x6A3EB6AD`; ASLR enabled. Record executable locations as RVAs; the loaded image base is `0x140000000`.

This folder consolidates the current design and status for the mod, the private-logistics plugin, and the state-storage source-only task. It also records the latest supplied Ghidra findings and separates static analysis from runtime proof.

## Documents

1. [`ghidra-do-storage-findings.md`](ghidra-do-storage-findings.md) — distribution-office TaskEntry/TaskCfg layout, save/load, native add/remove behavior, road/rail/type-5 reachability, count limits, and remaining unknowns.
2. [`current-task-state-storage.md`](tasks/active/current-task-state-storage.md) — the active opt-in source-only state-storage feature, staged implementation, safety limits, and validation checklist.
3. [`vision-private-logistics.md`](visions/vision-private-logistics.md) — the purpose, responsibilities, economics, invariants, and roadmap of `private_logistics`.
4. [`vision-whole-mod.md`](visions/vision-whole-mod.md) — the broader economic model and modular architecture for the WRSRE mod.

## Source material consolidated

- `/home/user/uploads/01_WORK_PLAN.md` — prior work plan and phase sequencing.
- `/home/user/uploads/02_VISION_AND_DESIGN.md` — prior economy design, player constraints, decisions, and open questions.
- `/home/user/uploads/03_ENGINE_AND_FUEL_FINDINGS.md` — prior static engine findings for transfer hooks, fuel, money, and logs.
- `/home/user/uploads/README.md` and `/home/user/uploads/WRSRE Codebase.txt` — handover context and the supplied 15-plugin codebase listing.
- `/home/user/implementation/` — staged candidate source/configuration and prior task notes. These files are not treated as compiled or deployed.
- Latest Ghidra reports supplied in conversation — summarized in the findings document; this session did not independently run Ghidra.

## Evidence labels used here

- **Ghidra-confirmed (static):** supported by the supplied decompilation/disassembly reports for the exact executable. This does not, by itself, prove the behavior in a live save.
- **Staged:** source/configuration exists in `/home/user/implementation`; it has not necessarily been built or deployed to the Windows checkout.
- **Historical runtime:** backed by an older log/run, but not evidence that the current staged version is working.
- **Unverified:** requires asset inspection, a build, or an in-game test.
- **Design decision:** a policy choice for the mod owner, not something Ghidra can settle.

## Source-of-truth and version caution

The documents draw on the supplied codebase snapshot, the staged implementation, historical design documents, and Ghidra reports shared during this conversation. The external Windows project tree has not been confirmed restored or identical to the staged files. Reconcile file hashes/commit and deployed DLL/INI versions before applying any patch.

The latest Ghidra report supersedes earlier contradictory reports where it provides newer evidence. In particular:

- The native DO save path was initially reported as not found; the later report found it and traced TaskEntry/TaskCfg reconstruction during load.
- Connection type 3 was initially called rail in one report; the later report identifies type 3 as road and type 6 as rail.
- `FUN_1407496C0` was initially described as a callable writer; the later report establishes it is a per-frame UI handler, not a standalone add API.

## Immediate status

The state-storage feature is staged, not released. `private_logistics.ini` has `auto_targets_dry_run = 1`, so target additions and TaskCfg edits remain blocked. No full Windows build or current-version in-game test has been performed. Keep that distinction visible in any handoff.
