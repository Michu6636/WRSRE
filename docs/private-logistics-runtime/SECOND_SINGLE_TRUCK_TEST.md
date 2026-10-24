# Second single-truck gameplay review — 2026-10-24

## Evidence

Attachment: `tesmioloader.log`, 5,723 lines, SHA-256 `3d5bff939e9bf4b4f60aad3245e3631d1adae16b70680349a63058bccd05c4c6`. All times/line references below refer to this attachment, not the earlier log. Raw log is not committed.

**Verdict: snapshot capture now works; cargo changes are visible; fuel burn and loaded-trip costing are NOT validated. Private-shop imports and wholesale inventory rollback interfere with clean delivery observation.**

## Probe activation and measurements

- Lines 47–50: v0.2.3-exploratory armed, tick_interval=60, max_snapshots=100, max_queue=256.
- Line 159: tick subscriber registered, handle 8.
- 41 snapshot records: 1 world_reset, 1 selection, 39 measurement records. Observation sequence 1–41 is continuous in this attachment. No `[PROBE DROP]` or subscriber-fault messages found. This is bounded evidence from this run, not proof of all concurrency/lifecycle safety.
- Measurements cover 19:13:20.699–19:15:50.329. Same selected truck `0000013475357C30` and DO `00000134A2313C50`; all 39 measurements report snapshot_ok=1 and home_matches=1.
- Every measurement reports fuel_slot_valid=0, fuel_tons=-1.000000. This means unavailable/invalid, NOT zero fuel.
- All 39 mirror readings are mirror_valid=1, mirror_tons=0.092560. No observed mirror decrease at six displayed decimal places.
- All measured raw state/mode values are 0; kind changes between -1 and 0. These do not yet supply validated loading/travel/unloading labels. `complete=1` in a measurement header must not be interpreted as a completed journey.

## Cargo and real deliveries

- 19:14:36.154: committed warehouse removal of 4.136 t clothes; corresponding truck-side cargo addition.
- 19:14:36.517: probe independently samples clothes=4.136482 t on the selected truck.
- 19:14:37.197: committed store addition of 4.136 t and paired truck removal. This is strong evidence of an actual DO delivery, not evidence of how much remained on the shop shelf later.
- Electronics cargo sampled at 0.383957 t, then 0.033817 t after an unloading burst. Native destination calls commit 0.117/0.136/0.097 t (rounded). Subsequent attempts around 0.034 t have commit=0; attempted transfer is not delivery.
- Later clothes journey: selected truck samples 5.500000 t, then 2.170473 t during unloading, then 0.000000 t. A later load samples 4.534832 t and remains loaded at the final measurement; that last interval is incomplete in this log.
- Sampling misses some transitions, particularly at accelerated simulation speed. It is not exact trip-boundary instrumentation.

## Why shop inventory can behave unexpectedly

### Direct foreign restocking was enabled and actually ran

Startup explicitly reports `foreign on (target 10%, markup 1.05)`.

- 19:14:01.164 (lines 1823–1828): store capacity reported 12 t, two slots (clothes/electronics), empty before import; private_shops imports two slots for 2,240.08 RUB. Under current source target math, 12 / 2 * 0.10 = 0.6 t per slot, subject to affordability/limits. This is not a 20% ceiling.
- 19:15:22.558: store imports two slots again for 2,280.14 RUB.
- Warehouse was briefly made private at 19:14:20.370, imported four slots for its entire 10,000 RUB capital, then returned to public at 19:14:24.656. Thus the run was not strictly store-only private throughout; this transient change also altered supply. Warehouse loading permission was enabled at 19:14:26.674.

`RunForeignRestock` writes stock directly and spends capital. Its normal path only replenishes below a reorder threshold; it is not shown clamping a delivered stock level down to 10% or 20%. The attachment has no synchronized shop stock/percentage before-and-after trace to prove what the reported 20% UI value meant.

### Wholesale billing can remove goods after the truck unloads

A separate path (`WholesaleSlot`, in private_shops.cpp) sees positive shelf deltas and bills them at wholesale price. If capital is insufficient and debt is not allowed, it writes:

```cpp
float newContent = snap.content + affordableTons;
*(float*)((BYTE*)slotBytes + TSM_SLOT_CONTENT) = newContent;
```

This rolls back the unaffordable part of a delivered inventory increase after native unloading. The truck is not restored by that code. This can create apparent disappearing deliveries, and repeated unload callbacks can still receive freight payments.

The first 4.136 t clothes delivery was billed 5,163.55 RUB at 19:14:37.521, reducing store capital from 7,948.36 to 2,784.81. That first charge was affordable; the log does NOT prove that first batch was erased. Later, after another foreign import, store capital falls to 238.28; at 19:15:38.332 wholesale takes it to zero with `paid ... (0 slot(s))`. Repeated small wholesale charges follow. This is consistent with the source's insufficient-capital branch, which does not increment the fully-billed-slot counter. It is a credible explanation for later stock not accumulating, not proof of the exact 20% display.

## Billing is still the legacy tariff, not measured trip fuel

private_logistics remains LIVE, billing=1/dry_run=0. There are 71 freight payment lines: 68 for clothes and 3 for electronics. Closely spaced unloading fragments are still charged separately. The single large first clothes callback generates one tariff payment; subsequent fragmented unloads generate many. Correct trip counts cannot be inferred from those counters. No loaded-trip fuel burn drives this tariff.

## Fuel lookup discrepancy

The legacy plugin reports a fuel slot and mirror both near 0.0926 t after its initial fallback. The probe never resolves that slot. Both examine the vehicle +0x3B8 vector, but use different resource identities:

- Probe: pointer at game+0xC3F0.
- Legacy: resource_pricing getByName("fuel").

This is a concrete discrepancy requiring instrumented identity comparison/correction. Neither a constant mirror nor the legacy zero `[BURN]` result establishes correct fuel consumption. Check actual game fuel-consumption settings and UI fuel changes, but do not ask the user to repeat the same unmodified probe test and expect a valid fuel verdict. Actual loaded-journey burn, refills and settlement pricing remain unimplemented/unverified.

## Local default change requested by this test

Automatic foreign restocking is now OFF by default in both `plugins/private_shops/private_shops.ini` and the DLL's missing-key fallback. Explicit `foreign_restock_enabled=1` still opts in. This leaves service fees and wholesale billing unchanged; it does NOT fix the separate inventory rollback or fragment-based freight billing. Existing deployed INIs set to 1 override the new fallback. Set `[private_shops] foreign_restock_enabled=0` in the active INI and fully restart to apply it to an existing build.

Validation: 2 default-configuration tests, 1 compiled production-checkbox-handler test, and 93 probe tests passed; git diff --check passed. No Windows build or game test of the new default performed here. Changes are local on the active Arena branch until committed/pushed/merged and deployed.

## Next useful work

1. Deploy foreign restock OFF, retaining private retail/service behaviour.
2. Correct/instrument fuel resource identification before another fuel validation run; compare pointer/name and slot contents against UI evidence. Keep invalid readings explicitly invalid.
3. Address wholesale stock rollback as a transaction-design issue (pre-authorize a real delivery, defer debt/settlement, or reject without destroying goods). Do not silently enable debt or disable all shop accounting as a workaround.
4. Keep legacy freight payments out of fuel validation; for tests requiring its target manager, use verified billing-off configuration rather than confusing target management with measurement.
5. Retain controlled one-save/process rules, normal speed, higher bounded sample budget and UI fuel/cargo screenshots. Save-write errors under saved_last recur near shutdown; preserve the original save.

## Follow-up implementation: preserve delivered stock and book debt

After this review, the user explicitly selected debt rather than deleting unaffordable delivered goods. The local patch now removes the entire wholesale inventory-rollback branch and its `allowDebt` switch. Positive observed delivery deltas are charged in full through the existing enterprise wholesale API; that API permits capital to become negative. The stock snapshot advances to the observed content so an unchanged subsequent walk does not charge that delta again. Wholesale billing performs no stock-content writes. The existing treasury/cost accounting path is retained; this is negative company capital, not a new deferred-invoice or loan ledger. Other enterprise insolvency/activity policies are unchanged.

Diagnostics now say `charged` rather than `paid`, include the actual capital balance, and label negative balances `(debt; delivered stock retained)`.

Local validation: 4 private-shop tests passed, including native compilation/execution of the production positive-delta settlement tail with stubbed game services. Cases cover sufficient/insufficient/zero/already-negative capital, unchanged repeated walk, consumption, later delivery increments, and unavailable price. The 1 checkbox test and 93 probe tests also pass (98 tests across those three suites). No Windows/game execution of this patch yet. The native harness does not establish atomic billing or exact transfer identity: wholesale still infers delivered stock from periodic positive shelf deltas.

For deployment after integration, build `cmd /c "build.bat private_shops"`, deploy the updated DLL and INI to the active loader, and explicitly set `[private_shops] foreign_restock_enabled=0` in any older deployed INI. Fully restart. Original foreign-restock stock/capital movements from earlier runs are not undone; use a clean copied test save. Confirm startup reports `foreign off` and validate retained stock/negative capital in game. Fuel-slot lookup correction and trip-level freight settlement remain pending; this fix must not be represented as completion of those items.
