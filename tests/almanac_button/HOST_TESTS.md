# Reproducible Almanac host checks

Run from any directory with Python 3 and a GNU-compatible C++17 compiler (g++ or clang++). This is not the MSVC plugin build. It never fetches dependencies, installs hooks, deploys DLLs or starts the game. Run one instance at a time: the existing tax extraction harness shares generated files under build/tax_policy_host.

```sh
python3 tests/almanac_button/run_host_tests.py
python3 tests/almanac_button/run_host_tests.py --sanitize
# Optional compiler executable, NOT a shell command with flags:
python3 tests/almanac_button/run_host_tests.py --cxx clang++
```

Default coverage: ten strict warnings-as-errors C++ suites (shared button policy, existing button geometry/open-request policy, panel lifecycle, Finance view/editor, bridge, controller, session and executor), plus the actual tax-provider/publication/consumer extraction harness. Sanitizer mode adds ASan/UBSan; it is not thread-race detection. Failures return a nonzero exit; suites after the first failure are not run. Compile/run commands are printed and each subprocess has a 180-second timeout.

Results are written to ignored build/almanac_host/normal/summary.json or build/almanac_host/sanitized/summary.json, recording compiler, per-suite outcomes, overall failure and optional dependency identity. Each invocation overwrites its mode's report. Compiler lookup errors are command-line errors before report creation. A compiler --version failure writes a failed report with zero suites; it does not claim tests passed. No code/source revision fingerprint is recorded, so retain the exact source commit/diff alongside results when handing off acceptance evidence.

## Optional headless drawing test

Supply an existing, unmodified upstream Dear ImGui checkout at commit f5befd2d29e66809cd1110a152e375a7f1981f06 (v1.91.9b). No Windows/DX11 backends are required for this headless test. Dependencies remain outside Git/source deliverables.

```sh
# Explicit dependency acquisition, NOT performed by the runner:
git -c core.autocrlf=false clone --depth 1 --branch v1.91.9b \
  https://github.com/ocornut/imgui.git .cache/almanac-imgui-host-tests
python3 tests/almanac_button/run_host_tests.py \
  --imgui-dir .cache/almanac-imgui-host-tests
```

The runner verifies HEAD and the bytes of every used ImGui source/header/license against that commit. Modified inputs fail instead of being accepted merely because HEAD matches. Disable checkout line-ending conversion to retain those bytes. Without --imgui-dir, the report explicitly marks the drawing suite skipped; a successful core run is not a claim that drawing was tested. This dependency pin is for prototype checks, not a decision on the future native backend.

## Lifecycle regression repaired in this pass

The merged bridge checked freshness when a completion was created, but did not invalidate an acknowledgement that was already waiting when SetContext changed world/panel/revision/readiness. A direct bridge consumer could consequently receive stale data with stale=false. The synthetic controller's separate identity checks already protected its display on a world change, but the transport flag was inaccurate.

A new test reproduced the failure before the fix. SetContext now marks waiting results stale, without changing their real execution/Finance/rejection outcome. Invalidation stays sticky even if a caller later restores readiness/context. Tests cover completion, refusal and queued cancellation across all four context changes (12 cases), plus controller completion -> world change -> Poll. No new native code or thread scheduling was introduced.

## Checks actually performed

- Normal core runner: all 11 core/extraction suites passed.
- ASan/UBSan runner: all 11 core/extraction suites passed; optional drawing explicitly skipped.
- Normal runner with pinned ImGui: all 12 suites passed, including headless drawing.
- Deliberately modified cached dependency was rejected and then restored.
- Deliberately failing compiler preflight returned nonzero and recorded failure with no suites falsely passed. The first manual assertion expected a suite record and failed; it was corrected to account for failure before suite execution. Final normal run replaced that intentional failure report with passing results.

Independent child review is unavailable in Arena; this is tested work, not independently approved native integration. Beam owns the parallel backend review. No graphics hooks, native launcher, live providers, Windows DLL build, deployment or game execution were performed. These new changes remain on the current Arena branch until a separate PR/handoff; they are not yet in Beam's Almanac checkout.
