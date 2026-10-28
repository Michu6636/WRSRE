# Road-vehicle panel fuel: focused investigation

## Status

Investigation specification, not completed reverse engineering or tracker implementation. Billing remains OFF; working dispatch policy stays unchanged.

## New visual evidence

User supplied `images.jpeg`, showing the native panel titled `Skd-706 RTTN open (Road vehicle)`. Beneath the E/F fuel gauge, the numeric display appears to read **549.54 l**. This is an approximate transcription from the screenshot, not a captured native memory value. The user reports this displayed fuel quantity is stable while parked and decreases while moving.

The screenshot is not established as contemporaneous with any logged sample or as the same vehicle/save as earlier logs. Do not compare its numeric value directly with those logs as a synchronized measurement.

## Why this investigation is needed

The latest inspected log reports an unavailable probe fuel slot and a constant candidate mirror. The legacy ReadTank logger can copy the mirror into its fallback level, then print that level under the label `slot` and print `agree`. That output does not establish an independent slot/mirror agreement.

Existing engine documentation supplies candidate paths, not a validated connection from this road-vehicle panel to the selected DO truck's physical tank. In particular, docs/ASSOCIATION_REVIEW_COMPLETE.md questions object provenance at the older refuel call site. Recheck vehicle versus building context rather than carrying forward names from decompiled-variable annotations.

## Investigation target

Trace **the numeric litres display underneath the selected road vehicle's E/F fuel gauge** back to its data source, then forward through its consumption and refill writers. Do not search for a literal `current fuel` label: the supplied panel does not show one.

1. Identify the road-vehicle panel update/render function and the number-formatting call for the litres display. Use the gauge, litres unit/formatting, and adjacent speed/RPM controls as navigation clues, not proof of a specific function.
2. Trace the displayed number backward through helpers, scaling, clamping and rounding. Report the underlying representation and units; do not assume tonnes, litres or the legacy configured density.
3. Establish the object provenance: selected-vehicle handle/pointer -> any nested object -> quantity field/getter. Explain how this maps to a vehicle enumerated from a DO. A matching model name alone is not sufficient identity evidence.
4. Find writes to that backing storage: actual consumption, refill, initialization/reset, and any mirror/cache synchronization. Distinguish physical quantity from a UI-only cached value.
5. Determine whether a consumption writer exposes the same vehicle and the actual deducted amount (including empty-tank clamping). Determine whether refuelling can occur between samples or in the same update as consumption. Do not propose endpoint subtraction as exact burn when refill can mask it.
6. Compare the proved source with current candidates (vehicle+0x5F0; cargo vector +0x3B8/+0x3C0; game resource globals +0xC3F0 and +0xC330). Confirm or reject each only with object provenance and instructions. Do not assume the two resource globals are equal.

## Existing entry points to inspect, not assume correct

- docs/engine/vehicle-fuel-system.md
- docs/ASSOCIATION_REVIEW_COMPLETE.md, especially M1/M2 and unresolved provenance
- docs/private-logistics-runtime/evidence/ROAD_PATH_EVIDENCE_COMPLETE.md
- plugins/private_logistics_probe/private_logistics_probe.cpp: selected-vehicle resolution and fuel readers
- plugins/private_logistics/private_logistics.cpp: FuelSlotTons, MirrorTons, ReadTank

Prior addresses are tied to SOVIET64.exe v1.1.1.9, PE timestamp 0x6A3EB6AD. Verify the analyzed binary identity and record its full hash before reusing addresses. Use module-relative addresses in integration proposals, not one process's ASLR addresses.

## Required output — all findings in this one Markdown file

Append the actual findings here, including:

- Analyzed executable identity/hash and tools actually run.
- Panel display function, format call, getter/source chain, and instruction excerpts proving each step.
- Object provenance, field offsets, units, conversion and precision.
- Burn/refill writer addresses, actual deducted amount and clamping behavior.
- Confirmed facts versus hypotheses and remaining unknowns.
- Complete proposed C++ reader/observer source, if justified, including validation and unavailable-value behavior.
- Actual build/test results, explicitly distinguishing synthetic tests from in-game evidence.

A Ghidra result alone is not runtime confirmation. The first integrated read must identify the same truck and match its live panel over a changing interval. Keep the observation bounded and nonblocking, and do not write game fuel fields or enable billing.

## Integration acceptance after the source is proved

- Same-truck native quantity follows the panel after verified unit conversion/rounding.
- Empty travel excluded; first goods loading starts one journey; additional loading and partial unloading do not reset/close it; final goods unloading closes it once.
- Tank fuel excluded from goods-empty detection.
- Refills cannot erase accumulated consumption or create negative charges. If exact deductions are not observable, ambiguity is explicit and the journey is inconclusive.
- Missing reads, dropped observations, identity changes and world reloads invalidate continuity; do not assume pointer identity persists across reloads.
- Initially loaded vehicles are incomplete observations, not fully measured journeys.
- Exactly one diagnostic completion per fully observed loaded journey. No settlement in this stage.
