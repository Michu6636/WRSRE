# Exploratory Windows prototype — v0.2.3

## What changed in Arena

This session imported the diagnostic plugin and offline tests from Agent D commit `0f2cbfe` without switching/merging branches, then fixed the code locally on `arena/83c93542-wrsre`.

- Snapshot payload owns bounded record-type, failure-reason and cargo text; queue copies cannot retain producer stack pointers.
- Production DLL and native test use the SAME `snapshot_record.h` serializer. The test overwrites/exits producer scope before consumption and parses the real serializer output with Python.
- Enabled probe, loader logger and sim_clock modules are pinned before starting asynchronous work. `DllMain` is a no-op: no waits, callbacks into other modules, frees or lock destruction. Resources are process-lifetime; hot unload is intentionally unavailable.
- Producer uses try-lock and counts contention/full-queue drops; worker reports loss. Snapshot log sequence is assigned on emission; separate `observation_seq` preserves capture identity. Worker performs no game-pointer dereferences.
- Expected x64 PE timestamp/magic checked before subscription. Full file hash must still be checked by the operator.
- Candidate lists are not silently truncated. Selection requires exactly one vehicle with matching home in road-DO fleets; raw kind/state/mode remain observations, not trusted eligibility meanings.
- Invalid/missing fuel is not silently converted to a valid zero reading.
- Existing production `private_logistics` and shared loader APIs are unchanged.

## Checks actually executed here

1. `python3 -B -m unittest discover -s tests/private_logistics_probe -t . -q`: **91 tests passed**, including host compilation and execution of shared production serialization.
2. Portable C++ harness compiled/run using GCC with address/undefined-behaviour sanitizers and stack-use-after-return detection: passed.
3. Windows x64 DLL cross-compiled and linked with Zig's Windows-GNU target. Exports/imports inspected with objdump. This is NOT an MSVC build and NOT Windows/game execution.

Commands (toolchain is local/ignored, not a repository dependency):

```sh
.cache/probe-toolchain/ziglang/zig c++ -target x86_64-windows-gnu -std=c++17 -fms-extensions -D_CRT_SECURE_NO_WARNINGS -c plugins/private_logistics_probe/private_logistics_probe.cpp -o build/probe-tests/probe.obj
.cache/probe-toolchain/ziglang/zig c++ -target x86_64-windows-gnu -shared build/probe-tests/probe.obj -o build/probe-tests/private_logistics_probe.dll
```

DLL SHA-256:
`186FE1A692F3491642CE08E1E4DF5F2408E57960BF751E484A72E6718EEDBBE0`

Dependencies reported by PE inspection: Windows UCRT API-set libraries, KERNEL32 and ntdll; no libstdc++/libgcc DLL dependency. Windows loader compatibility remains untested. Alternative supported project build: copy the complete probe source directory (including the new header) into the matching checkout and run `cmd /c "build.bat private_logistics_probe"`; that creates a DIFFERENT DLL/hash.

## Purpose and limits

Read-only sampled cargo/fuel/state observations for ONE truck. No new executable patches, treasury service or reimbursement. NOT actual accumulated fuel burn, complete trip-boundary verification, trip-cost validation, or proof of offset correctness. Sampling may miss transitions/refills. Existing offline checker trip/billing verdicts may be INCONCLUSIVE; that is expected for this experiment.

The cargo vector/total can include fuel. Do not interpret the total as delivered goods without inspecting the per-resource listing. Listing is bounded/truncated; it is exploratory, not an invoice.

## Controlled first run

1. Completely exit the game. Back up the current loader INI, probe files (if any) and test save. Use a COPY of a save with exactly one truck across all road DO fleets. Do not demolish/change fleets in the original save to satisfy selection.
2. Confirm target executable SHA-256 with PowerShell:

```powershell
Get-FileHash 'C:\Workers Resources Soviet Republic\SOVIET64.exe' -Algorithm SHA256
```

Expected: `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`. If different, do not run this build; report the identity first.

3. Find the EXISTING directory containing your active `tesmioloader.ini`. Its `plugins` subdirectory is the deployment location, not necessarily the game executable directory. Do not create a second loader installation. Copy only the supplied `private_logistics_probe.dll` and `.ini` into that existing plugins directory. Do not replace the loader or other DLLs.
4. Edit EXISTING keys in `tesmioloader.ini` (do not append a second `[plugins]` section):

```ini
[plugins]
sim_clock=1
private_logistics_probe=1
private_logistics=0
```

This disables the old private-logistics plugin for an isolated experiment. Preserve all unrelated keys. Normal game fuel/resource purchases and other mods may still affect treasury; this probe issues no payments. If disabling the old plugin is unacceptable for your save, ask before proceeding; use its verified billing-off configuration instead.

5. Edit the deployed `plugins/private_logistics_probe.ini`:

```ini
[private_logistics_probe]
enabled=1
tick_interval=30
max_snapshots=1000
max_queue=256
selftest=0
```

Keep ASCII/UTF-8 without BOM. Current source/INI enable the probe by default; set `enabled=0` to opt out. An existing deployed INI with `enabled=0` still overrides the DLL fallback. The original v0.2.3 download ZIP predates this default change and remains disabled by default; rebuild and deploy the current DLL/INI. The suggested sampling settings above are for the controlled test.
6. Start one game process, load one copied save. Do NOT reload, switch worlds, reassign/sell/demolish the selected truck/DO, or hot-unload DLLs during this run. Full game exit between experiments. World-load invalidation is not claimed verified.
7. Unpause at normal speed for 20–30 seconds. Check `tesmioloader.log` for version `0.2.3-exploratory`, successful tick registration and `[PROBE SNAPSHOT]` records. If no selection, fault, identity mismatch or load failure occurs, stop and send the log; do not change offsets or try old DLLs blindly.
8. If measurement records appear, take a screenshot of the truck/DO UI showing cargo/fuel, record the wall time, then let it perform a simple source-to-destination journey (and a second if the sample budget allows). Note loading, departure, arrival/unloading and refuel times. Do not force expected raw state values; discovering them is part of the experiment.
9. Exit the game normally. Copy the FULL `tesmioloader.log` elsewhere before another launch overwrites it. Return it, the screenshots/observations, exact DLL hash/config and any crash/fault messages. A truncated final record on process exit is possible; do not interpret it as complete evidence.

Stop on crash, subscriber fault, invalid measurements, unexpected performance degradation or gameplay changes. Do not continue using the test build for normal play.

## Rollback

With the game FULLY EXITED, remove/disable `private_logistics_probe` and restore the backed-up loader/plugin configurations. Restore the original save copy rather than overwriting it with the experimental one. Do not attempt to unload the pinned module in-process. The loader, original plugin and save formats were not modified by this prototype.

## Artifact provenance

The download includes DLL/config, these instructions, hashes and the complete changed probe sources. It does not include or redistribute the game executable/save. Generated toolchains, binaries and packages remain under ignored paths and are not intended as Git source changes. The DLL hash above belongs only to this exact cross-build, not Agent D's earlier MSVC build.
