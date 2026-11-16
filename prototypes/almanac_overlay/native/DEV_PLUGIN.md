# Experimental Almanac DLL — build/review candidate, not game acceptance

`dev_plugin.cpp` supplies the missing Tesmio API 4 Init/Start exports. It is outside `plugins/`, has no custom DllMain, and the CMake DLL target is **OFF by default**. Building does not copy a DLL/config into a game. Do not deploy or run it until the new source is reviewed, Windows-built and a disposable-copy test is explicitly authorized.

## Explicit build target (not yet Windows-tested)

After the isolated Windows configure described in [WINDOWS_CHECK.md](WINDOWS_CHECK.md), opt in explicitly:

```powershell
cmake -S prototypes/almanac_overlay/native -B build/almanac_native -A x64 "-DALMANAC_IMGUI_DIR=$imgui" -DALMANAC_BUILD_DEV_PLUGIN=ON
if ($LASTEXITCODE -ne 0) { throw "Almanac configure failed" }
cmake --build build/almanac_native --config Debug --target almanac_overlay_dev
if ($LASTEXITCODE -ne 0) { throw "Almanac DLL build failed" }
```

Expected output: `build/almanac_native/Debug/almanac_overlay_dev.dll`. This is the first full link target for the adapter; the standalone executable did not pull its unreferenced object out of the static library. All targets now use the static MSVC CRT to match the loader API contract. No Windows build result from an earlier source revision transfers to this target/CRT change.

## Defaults and scope

The example `almanac_overlay_dev.ini` is not copied by the build. Both controls default to zero:

- `enabled`: opts into binary validation and creation-observer installation. If absent/zero, Init declines with no hooks installed.
- `allow_synthetic_without_modal_tracking`: independent, explicit permission for the noninteractive synthetic overlay. Without it, observation can occur but F8 cannot draw. With it, F8 can toggle the synthetic panel once the chain is captured and focused. This is deliberately named: there is **no native modal-state provider**, so it may cover native dialogs. It captures no input and does not block underlying game controls. Do not click through the panel and call that safe interaction. Restrict any eventual test to a disposable setup and visual checks.

Neither setting permits live Finance access, editing, deployment, save writes or enabling the old sidebar prototype. Existing budget/HUD F8 remains unchanged; both would toggle in a game test. Final player launcher and shared hotkey ownership remain unresolved.

## Identity/startup contract

Init validates the host prefix/API/functions and current executable/engine module handles, hashes their backing files using synchronous bounded reads plus an original streaming SHA-256 implementation, and checks the loaded engine instruction `FF 15 8E 6E 15 00` at RVA `0x148BC`. Expected file identities are the S0 executable/engine hashes; failures decline before hook installation. Files above 256 MiB, path truncation, sharing/read failures or a read-loop elapsed budget above ten seconds per file also decline. A blocking OS read is not forcibly cancellable by that budget. This validates the backing-file identity and one call instruction, NOT resistance to hostile in-memory patching or arbitrary mods.

Hashing uses no CNG/crypto-provider loading, allocations or workers under the loader lock. Start installs only the early IAT observer and publishes the explicit synthetic permission. Window validation occurs on actual creation; ImGui/backend initialization occurs at Present. There is no wait for a window or worker thread in Init/Start, and late load has no fallback. Startup still requires a reviewed loader/injection path. The existing loader retains Start failures; adapter installation pins the DLL before any detour publication. No detach cleanup/unpatch or hot unload is supported.

The matching launcher timeout path was also corrected: only WAIT_OBJECT_0 plus a successful, nonzero, non-STILL_ACTIVE exit status permits ResumeThread. Failed/timed-out injection does not free the parameter buffer under a live remote thread; the unsuccessful child is terminated instead. This launcher source change needs its own Windows build/regression checks; merely rebuilding the plugin does not update an old launcher binary.

## Verification to obtain

- Review the new plugin, identity hash, CMake runtime/link settings and launcher failure path.
- Build the standalone diagnostic AND explicit DLL; build/check the updated launcher separately. Confirm API exports with dumpbin and retain compiler/dependency/source identities and artifact hashes.
- Recheck missing config/disabled behavior, hash mismatch/read-failure rejection and normal identity success. Do not weaken the gate to fit a different binary.
- Standalone F8/resize/focus/minimize visual checks are still outstanding. DLL link success does not prove actual hook installation or original rendering/input coexistence.
- Only after explicit authorization: disposable-game visual synthetic-panel test, no policy/save modifications. Keep full evidence and stop on faults.

Arena verification so far: original hash matched Python hashlib on 80 fixtures/chunk combinations, normally and with ASan/UBSan. This is host hash testing, not Windows file-read, plugin, launcher or game testing. Separate reviewer/toolchain acceptance remains outstanding for the new changes.
