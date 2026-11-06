# Single-truck gameplay log review — 2026-10-24

## Evidence and verdict

Input: attached `tesmioloader.log`, 6,625 lines, SHA-256 `e1672885bc83e4ff90623ac82e60d0574414b1c9b6ff8d2ac845568be3fd7331`. Line references below are to this exact attachment. Raw log is not committed.

**Probe loaded, but was disabled. This is not a successful snapshot-probe runtime test.** The legacy plugin nevertheless captured useful real-game transfer evidence and exposes serious billing/boundary issues.

## Probe versus legacy plugin

- Lines 47–50: exactly one `[PROBE SESSION]`, `event=disabled`; explicitly no sim_clock subscription or game reads; DLL reports `0.2.3-exploratory`. No `[PROBE SNAPSHOT]` records anywhere.
- Lines 149–151: legacy private_logistics v0.17 has `billing=1 dry_run=0`, automatic targets in write mode, fixed tariff `14.75 L/trip`, wear `5 L`, margin `35%`, auto-tune off. It installed 16 transfer-site hooks (line 148). This was not an isolated read-only experiment.
- File contents/active deployed probe configuration were not supplied; default/incorrectly located config is a plausible cause, not proven. The plugin reads `plugins\\private_logistics_probe.ini`, section `[private_logistics_probe]`, key `enabled`, default 0, relative to the loader base directory.

## Fleet and automatic targets

Legacy daily census days 2–12 repeatedly sees one vehicle `000001D8F2B6CCB0`, home=this, in `muddy_distribution`. Daily auto-target summaries see one private office, key `83DDA9FCBA29D771`, type 43. This supports the user's one-DO/one-truck setup (not a complete global census of all vehicle types).

At day 2 (lines 2024–2027), warehouse `warehouse_covered_combined`, key `72808CE752AE6DFD`, is LOAD-ON; three candidates/targets are added. With the user's setup, these are consistent with two private storage-capable endpoints plus the opted-in public warehouse; summary lines alone do not individually identify all three. After OFF, the candidate count falls to two (line 4282). The state grocery is not demonstrated to be an automatically added private destination; world-discovery counts are broader than the deliberately placed test buildings and include infrastructure.

## Checkbox inversion and remaining enforcement uncertainty

- Lines 1491–1492, 18:59:15.481: checkbox `1 -> 0`, permission becomes `load enabled`.
- Lines 4169–4170, 19:01:23.494: checkbox `0 -> 1`, permission becomes `load disabled`.
- The production handler explicitly inverted the numeric checkbox value. This agrees with the user's report of visual inversion.
- Local source fix maps mode 1 to checkbox 1; a checked toggle stores mode 1 (ON), an unchecked toggle stores mode 2 (explicit OFF). Untouched default OFF remains mode 0. It also removes a per-frame misleading diagnostic: widgetCheckbox returning zero ordinarily means unchanged, not failure.
- This does **not** establish correct native enforcement: the log records committed warehouse loading at 19:01:32 and again at 19:02:36, after the permission was logged OFF. The day-7 sync at 19:01:27 reports config_changed=1 before those events. Thus OFF cannot be claimed to have prevented subsequent loading in this run. Existing dispatches/native task settings need independent examination; reason not established here.
- Existing code reconciles once per game day, preserves untouched manual targets, and has an INI `state_storage_always_on` bypass. No change to those policies in this polarity-only patch. Keep bypass 0. Strict checkbox-only authorization of every pre-existing/native dispatch is not yet verified.

## Real transfer evidence

Legacy committed transfers pair warehouse stock removal at RVA `0x694F40` with truck cargo addition at `0x6953C4`; private destination stock addition at `0x68F792` pairs with truck cargo subtraction at `0x68F7F9`. Destination and vehicle halves are not separate deliveries.

| Observed unloading burst | Destination in log | Cargo | Committed destination callbacks | Daily carrier payment |
|---|---|---:|---:|---:|
| 19:01:50.704–19:01:51.394 | MIRRORZ_DLC3_small_store | clothes, 2.384 t (daily aggregate) | 12 | 41.18 RUB |
| 19:02:20.527–19:02:21.857 | 3022562390/bar2 (private pub) | alcohol, 3.279 t (daily aggregate) | 22 | 75.49 RUB |
| 19:02:50.446–19:02:51.081 | MIRRORZ_DLC3_small_store | eletronics, 1.593 t (daily aggregate) | 13 | 44.61 RUB |

Quantity precision is limited to logged digits. This establishes three observed resource-specific unloading bursts, not exhaustive loading-to-complete-unloading trip validation. No logged committed delivery to the state grocery was established by these records.

## Serious billing issue

All 47 positive destination transfer callbacks triggered a roughly 3.43 RUB fixed trip tariff. These callbacks are small pieces of unloading, occurring milliseconds apart; **47 callbacks are not 47 journeys**. The truck-side paired callback is classified staging/transship and is not another payment, but the destination-side pieces are still incorrectly treated as complete deliveries for trip pricing.

Rounded daily carrier totals sum to 161.28 RUB; the displayed DO capital goes from its 10,000 baseline to 10,161.27. The 0.01 difference is consistent with independently rounded displays; do not sum the rounded per-event 3.43 values for exact accounting. Treasury-leg checks report exact corresponding debits for all sampled payments (12 + 22 + 13), so this is real money movement, not just a misleading display. It validates accounting execution, not tariff correctness. Other treasury movement is not attributable to logistics alone.

## Fuel and state conclusions

- Daily legacy tank readings remain 0.0926 t at displayed precision from days 2–12. Day 2 falls back to mirror +0x5F0 because no fuel slot was found; days 3–12 show slot and mirror agreement. This is limited consistency evidence, not independently validated fuel burn.
- All hooked refuel counters are zero; this does not exclude unhooked refill paths or prove consumption was enabled.
- `[BURN]` reports zero over 46 callback-to-callback intervals and recommends setting litres_per_trip to that figure. **Do not follow that recommendation.** Intervals are not the requested loaded journeys and the log has no validated fuel decrease. Whether consumption is disabled, these readings miss consumption, or the interpretation is wrong remains unresolved.
- The daily scanner reports kind=-1, state=0/mode=0/idle even for samples carrying cargo. Raw readings and old labels do not establish actual eligibility or lifecycle states. Cargo alone does not prove movement; independently compare to the game UI.
- Live legacy fuel price resolves (e.g. 137.74 in its printed units); that does not validate units or settlement-time loaded-trip costing.

## Other log warnings

The log contains missing Workshop-item/resource warnings and save failures under `media_soviet/saved_last` near shutdown. It ends with a loader shutdown line, not an explicit probe crash. These messages should not be attributed to the disabled probe. Preserve a known-good save and investigate saving separately before relying on the experimental save.

## Next controlled test

Run separate tests: (A) checkbox/native-task permission behaviour with original logistics needed for target management, billing disabled via verified legacy configuration; (B) snapshot probe isolation with `private_logistics=0`, `sim_clock=1`, `private_logistics_probe=1` in loader config and deployed probe INI `enabled=1`, tick_interval=30, max_snapshots=1000, max_queue=256, selftest=0. Disabling logistics also disables its target-management callback: arrange suitable native targets on a copied save beforehand, without assuming the probe manages targets. Fully exit between tests.

Before driving in test B, confirm version, successful tick registration and actual measurement snapshots in the log. Check game fuel-consumption settings and compare the UI fuel reading at departure/arrival. One process/one copied save; no reload or reassignment. Do not estimate loaded-trip cost from this attachment.

## Local patch validation

`python3 -B -m unittest discover -s tests/private_logistics -v`: 1 test passed, compiling/executing the actual production handler tail with a stub native checkbox across default/on/off states, toggles, unchanged frames and subsequent redraws.

`python3 -B -m unittest discover -s tests/private_logistics_probe -t . -q`: 91 tests passed.

`git diff --check`: passed. No Windows build or in-game validation of the checkbox patch performed here; changes are local to the active Arena branch, not yet deployed or merged into the fuel branch.
