# Test15 — bounded native evidence closure (Windows / read only)

Use one parent Kilo Code session in the existing Windows WRSRE workspace. Read AGENTS.md, INSTRUCTIONS.md and KILO_AGENT_WORKFLOW.md. This request grants read-only analysis, not implementation, Git, build, deployment, configuration, hooks or game changes. Do not start/stop other sessions. The parent owns dependencies and the single serialized Ghidra resource; use a focused child only if useful and supported. Return one complete result, not instructions for the user to relay between agents.

## Established evidence: reuse, do not repeat

The Test15 cap is five TOTAL entries, not five additions/day. Existing automatic append uses TaskEntry 0x18, TaskCfg 0x198, ctor RVA 0x1512A0 and vector helper 0x08E050. Native ordinary 0.18.18 was tested; do not repeat saving/fuel/demolition tests or request the EXE.

`docs/ghidra-exports/decomp_1406EE2A0.c` lines 155–156 allocate 0x5D8 and call ctor 0x25C500. Lines 580–600 initialize **100 DWORD values** in `[receiver+0x310, receiver+0x4A0)`: undefined2* displacement 0x188 means byte 0x310; `(6*8+2)*8=400` bytes; next separate field is byte 0x4A0. This is a real capacity lead, not implementation readiness.

`docs/private-logistics-runtime/RAW_EXPORTS_741170_1E440.md` lines 162/175 bind the road renderer's panel and building. Lines 1697–1715 read/write `panel+0x310+idx*4`, including bulk propagation over the total task count. The literal `/49` is at line 1017. `docs/ghidra-exports/xrefs_1406EE2A0.txt` names dispatcher 0x6E9B80 and both DO builders. Xref membership is not receiver-flow proof. See `docs/ghidra-index/6EE2A0.md`.

## A. Close the receiver / consumer edge

Use caches first. Recover actual argument/return dataflow from the 0x5D8 allocation through ctor 0x25C500 and world panel table insertion to the dispatcher receivers for road builder 0x741170 and rail builder 0x745350. The creator decompiles insertion as `FUN_14001e140(plVar1,&local_res8,lVar34)` while local_res8 previously held context: resolve actual registers/stack and output-vs-input arguments. Do not invent a native bug or assume the displayed argument count is complete.

Find rail target-state accesses, initialization/rebuild/teardown and any additional index/adjacent-field constraints. Distinguish initialized physical slots (100), native UI policy (49 on road), per-selection batch limit and total target count. Report road and rail conclusions separately. If only road is closed, say whether a road-only bounded fix retaining rail's conservative policy is supported.

Only if needed, use the existing Windows wrapper, one invocation at a time:

```bat
scratch\ghidra_run.bat DecompAt.java scratch\out\test15_panel_route.c 0x1406E9B80 0x140745350 0x14025C500
scratch\ghidra_run.bat DisasmMany.java scratch\out\test15_creator.asm 0x1406EE2A0:0x1406EF7E4
```

These are preferred-image VAs (`0x140000000 + RVA`), not raw file offsets. Check wrapper/project/scripts first. Reuse current cache exports; do not run these just to reproduce available evidence. On timeout use only the unresolved instruction ranges. Report exact commands/errors and target identity; no claim of execution from this prompt itself.

## B. Commodity admission: trace only the missing ratio producer

The cached predicate at preferred VAs 0x1401DE327–0x1401DE35F reads row+0x60, row+0x28, TaskCfg+0x04 and TaskCfg+0xCC. It rejects row+0x60<=0, row+0x28<=source threshold, and row+0x60<(constant at 0x14090A1F0 minus unload threshold), plus an additional ordinary-office guard. Reader evidence does not identify the ratios' denominators.

Start from office candidate vector +0xD58/+0xD60 (0x80-byte rows), rebuilt by 0x1E2380 and consumed by 0x1E5480/0x1E5BA0. Reuse the latter's full cached decompilation in the cumulative report, lines 74500 onward. Trace the actual writers of row+0x28/+0x60 and their resource/storage selection and denominator. Inspect only the relevant upkeep caller/writer as needed; stop with the exact missing edge if unavailable. No broad gauge/offset research.

`docs/private-logistics-runtime/DO_PLANNER_RAW_EVIDENCE.md` lines 300–331 has a sum of resource stock/capacity terms on one positive-param4 branch. Here plVar19 is a building storage record (`building+0x970`, stride 0xE0), **not TaskCfg** despite older prose at lines 1553 onward. Do not use that separate planner branch to assert the DO threshold ratio.

Test15 crops80/880≈9.09% is only a conditional source10 explanation; Electronics stock was cropped out, and Electro comp. is distinct. Do not change 10/30 defaults or suggest 30→100 based on aggregate fullness.

## Return once

Return a claim ledger with exact exports/addresses, typed byte arithmetic, supported/contested/unresolved status, full deciding raw excerpts and exact commands/results. State whether native capacity-safe append is ready, separately for road/rail; identify a minimal remaining blocker if not. No runtime edits. Keep new output in scratch; follow local index-recording rules for actual Ghidra calls, without rewriting unrelated documentation. No further gameplay run is required for this static closure.
