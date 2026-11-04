# Test16 — bounded road TOTAL-target policy

Status (2026-11-04): IMPLEMENTED / HOST REGRESSIONS PASS; native acceptance PENDING.
Working source: `0.18.19-road-targets`. This is not a deployment/publication instruction.

## Decision and evidence

- Road building type 43: **20 TOTAL target entries**, including every manual entry.
- Rail type 52: retain **5 TOTAL**. Unknown/unreadable office type: deny append.
- Physical panel capacity: 100 DWORD controls at `[window+0x310, window+0x4A0)`; not an engine maximum or permission to add 100 targets.
- The complete road raw renderer in `docs/private-logistics-runtime/RAW_EXPORTS_741170_1E440.md` has BOTH `/49` (line 1017) and `/20` (line 1900), selected through the panel+0x30C branch (lines 982–1003). Twenty respects both without adding unverified live-mode reads. Last row 19 uses `[+0x35C,+0x360)`, inside the initialized controls.
- The V2/insertion/helper reviews close the creator → window local → count-one vector insertion → stored panel → dispatcher → road receiver chain. See `6EE2A0`, `1E140`, `1E8C0` function indices and the cumulative report. No extraction rerun is needed.
- Existing native TaskEntry/TaskCfg append, configuration, notification, removal and serialization paths remain in use; no new object layout, allocator, resize, lifecycle hook or save format. Historical save/load synthesis is NOT newly decoded native evidence. New >5 gameplay/panel/save-load behavior still requires testing.

## Implementation invariant

`plugins/private_logistics/auto_target_policy.h` is the shared policy. Reconcile existing targets before applying its limit; never delete over-limit/manual rows to create space. Count successful or dry-run planned additions, not failed appends. Recheck actual type and live count BEFORE TaskCfg allocation/native calls. Use subtraction-based checks to avoid overflow for adversarial counts/projections. Existing identity deduplication, directions, permissions, defaults, 10/30 thresholds, timer/dirty semantics and billing remain unchanged. Payments OFF.

## Actual verification

`python -m unittest discover -s tests/private_logistics -p 'test_*.py' -v`: **147 PASS, 19.509 s** on the final 20/5 policy. The three focused tests also pass (3.181 s). The C++ harness extracts the ACTUAL production daily reconciliation body and live append preflight, compiling with C++17/warnings-as-errors in optimized and ASan+UBSan modes. Native services/vector/append are stubs, so this proves host logic, NOT native allocation/calls, linkage, UI or save-load behavior.

Coverage: sixth entry; duplicate/no-dirty stability; manual identity; reconstructed existing entries and allowed recreation after removal; 19→20, existing20/21/49/101 retention; dry-run projections; rail/unknown/unreadable types; permission/customs/delivery-only directions; failed append not spending budget; full live count/type preflight; overflow and all in-range helper combinations. Running the original HEAD daily body against the same new fixture fails as expected at the sixth-target assertion (return -6); the final body passes.

## Pending native acceptance (only after separately authorized build/deployment)

Use a disposable copy, not the sole save. With at least six eligible private storage targets and a private road office, observe a real daily reconciliation: target six is present once with correct directions. Open/scroll/collapse/reopen the native panel; exercise entries 6–20 and both available display modes. Confirm no UI corruption, no duplicate/config reset after another day, and no added target beyond total20. An existing manual row counts; existing >20 manual rows survive. Rail must remain at five automatic total; no access-policy change. Validate named save/reload specifically for the >5 targets and panel reconstruction (not another basic saving tutorial). Native failures block acceptance; retain logs and binary/source hashes if available. Do not infer deliveries merely from a newly appearing target or transfers merely from a row count.

Commodity admission is a separate unresolved investigation: cached resource row+0x28/+0x60 producers/denominators remain unproved. The cached 7496C0 clamp excerpt proves only normalized TaskCfg threshold ranges, not those runtime writers. No new raw writer packet is present in this checkout; do not guess them or change 10/30 globally. Existing `TEST15_NATIVE_CLOSURE.md` retains that separate scope.

No commit, push, PR, Windows build/deployment, game execution, CI, agent-session operation or payment enablement was performed. Evidence, source and actual test output are archived in `docs/private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md`, section “Test16 bounded road TOTAL-target implementation”.
