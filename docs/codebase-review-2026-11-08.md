# WRSRE codebase review — 2026-11-08

## Implementation follow-up

The subsequent currency correctness patch addresses finding A and changes state
checkout to use all-or-nothing payment. See `tests/currency/README.md` for the
implementation boundaries and regression coverage. The review below describes
the original baseline, not the patched currency implementation. The owner has
clarified that Almanac is the immediate milestone, functioning economy is later,
and save-switching support is deferred (restart-only is acceptable for now).

## Scope and verdict

Reviewed checkout baseline `518aac5b98960019b340139af371ec78d8eda5d9` on `arena/cfbbf26f-wrsre`. The working tree was initially clean. This review changes documentation only; no game behavior, configuration, Almanac implementation, deployment, or GitHub state was changed.

**Verdict:** this is a substantial working economic simulation prototype, with useful service boundaries and particularly strong recent investment in logistics host tests. It is not yet a uniformly hardened or release-validated platform. The highest-value next work is transaction correctness, world-load isolation, deterministic scheduling, and a reproducible Windows validation pipeline—not another broad economic feature.

This is a source-level architecture and risk review, not a claim that every line or native offset is verified. No MSVC, Windows game process, executable, or Ghidra project was available here. Existing reverse-engineering reports are evidence leads, not fresh runtime verification. Almanac belongs to another contributor; its implementation was deliberately left alone.

## 1. How the mod works

### Host, launcher, and ABI

- `src/tesmiolauncher.cpp` is the Windows launch/content-management front end; `src/tesmioloader.cpp` is the injected native host, with hook helpers, rendering support, plugin discovery, service registry, and save-content manifest support.
- Plugins are separate C++17 x64 DLLs, statically linking the CRT (`/MT`). They share POD service tables rather than C++ objects. `src/tesmio_plugin.h` binds each DLL to the host and initializes that DLL's lock and address state.
- The loader calls `TsmPluginInit` for each discovered DLL, then `TsmPluginStart` in a second pass. Init publishes services; Start resolves other services and registers callbacks. This solves service *discovery* ordering, but does not establish dependency readiness or economic callback order.
- Services are versioned by name/version in the loader noticeboard. API headers use append-only fields and, in many places, `structSize` capability checks and old-version fallbacks. Preserve this compatibility discipline during refactoring.
- Hooks combine expected-byte verification, IAT patching, inline detours, and hand-generated machine-code stubs. The generic inline helper assumes displaced instructions are position-independent; it is not a general instruction-relocating detour library.
- `src/game_addresses.h` centralizes much of the version/RVA handling. Plugins still contain native offsets and custom hook installation. Timestamp checks and expected prologue bytes reduce risk; neither is complete semantic verification of a changed game build.

### Economic layers

| Layer | Main owners | Behavior and coupling |
|---|---|---|
| Time | `sim_clock` | Hooks the native simulation timer, identifies the day-tick caller, emits tick/day/week/month/quarter/year callbacks, detects some calendar discontinuities. |
| Save/load | `persistence` | Hooks CRT file-open paths, infers save/load from configured trigger filenames, dispatches sidecar handlers, provides bounded file reads and atomic replacement writes. |
| Money | `currency` | Citizen-ID-keyed lazy wallets; RUB/USD treasury reads/writes; active-currency routing; labelled flow observers. This is a foundational conservation boundary. |
| Prices | `resource_pricing` | Native resource quotes plus configured fallbacks, supply/demand multipliers, survival-basket CPI, and local stock-scarcity quotes. |
| Firms/housing | `enterprise_base` | Private ownership/tag detection and UI controls, enterprise registry, capital/P&L, payroll and retail interception, rents, eviction, bidding, insolvency/abandonment, investment pool. |
| Labor | `citizen_labor` | Wage configuration and persisted overrides, worker/payroll processing, minimum wage and skill rules, native labor-mobility hook. Private payroll is delegated to the enterprise interceptor. |
| Retail | `citizen_retail` | Native checkout affordability/fulfillment gate, price/cost calculation, private interceptor or state settlement. It must align goods fulfillment with actual payment. |
| Shops/industry | `private_shops` | Samples physical storage changes, handles wholesale/procurement accounting and service fees; optional foreign stock injection is disabled in the tracked INI. |
| Logistics | `private_logistics` | Native transfer observations, seller/resource/price cargo provenance, goods counterparty settlement, observed-assignment fuel accounting, independent automatic DO-target assistance. |
| Credit | `republic_bank` | Pure loan-book core, CPI/currency-indexed limits, treasury-funded draws, monthly interest/principal settlement, optional daily overdraft rescue, City Hall rate UI. |
| Investment | `private_construction` | Captures native panel cost data, tracks private construction sites, debits the enterprise investment pool, advances native progress, then performs a staged native completion handoff. |
| Fiscal/accounting | `taxes`, `state_ledger` | VAT/profit/property and citizen fiscal logic; labelled treasury flow aggregation and monthly stock/flow reconciliation. |
| UI/diagnostics | `ui_panels`, `ui_overlay`, `private_logistics_probe` | Native building-window rows/widgets, HUD cards, exploratory truck snapshots. Diagnostics are not proof of exact physical fuel consumption. |
| Map utilities | `map_expander` | Separate native-boundary expansion feature; needs its own game-version and save-compatibility validation. |

### Representative transaction paths

1. **Private wage:** labor computes gross wage → enterprise payroll interceptor resolves premium/floor/cap → enterprise capital and worker wallet move → taxes/period totals are recorded. State payroll instead uses treasury-to-wallet movement.
2. **Private checkout:** native checkout gate computes cost → enterprise interceptor applies markup/VAT → wallet affordability check and debit → capital credit, with explicit refund on refused capital credit → VAT accrual → native shelf/demand fulfillment continues. There are already valuable local conservation fixes here; the lower-level currency API still needs repair.
3. **State checkout:** gate checks wallet → `citizenToStatePartial` → native dispense allowed or aborted. The partial-payment shortfall path does not have the private interceptor's explicit refund logic.
4. **Physical delivery:** logistics observes native movement, matches vehicle cargo to seller/resource/locked-price lots, and derives settlement. Separately, shops observe storage deltas. This makes exact ownership of each charge essential: one delivery must not be billed once by provenance settlement and again by storage polling.
5. **Monthly economy:** callbacks collect rent, siphon profits to the pool, adjust wage bids, evaluate insolvency, settle banking/taxes, and close ledger periods. Their ordering affects the accounting period and needs an explicit contract.
6. **Save/load:** persistence emits an event while native files are opening; each plugin serializes or stages sidecars. Enterprise records rebind to live buildings through deferred identity/coordinate matching, rather than persisting pointers. Logistics observers reset load epochs to avoid charging ambiguous pre-load journeys.

### Scale and organization

The inspected plugin sources/headers/configs and shared `src` files total roughly 39,765 lines. The largest gameplay owners are `enterprise_base.cpp` (4,690 lines), `ui_panels.cpp` (3,346), `citizen_labor.cpp` (2,840), `private_logistics.cpp` (2,723), and `private_construction.cpp` (2,361). Large files alone are not defects, but enterprise policy, native access, serialization, diagnostics, and UI are too entangled to make isolated changes cheap.

Logistics shows the preferable direction: pure, bounded state-machine helpers such as `b2b_settlement.h`, `schedule_assignment_ledger.h`, and observer helpers, with Windows/native adapters around them. Extract other subsystems incrementally in the same way rather than rewriting the whole suite.

## 2. Prioritized findings

Priorities below describe recommended work order, not measured crash frequency. **Confirmed** means visible in the current source; runtime consequences remain unmeasured unless explicitly stated.

### A. High — full-charge currency API violates its contract (confirmed)

**Evidence:** `src/currency_api.h:139–145`; `plugins/currency/currency.cpp`, `Api_ChargeCitizen` and `WalletTake` (roughly lines 181–230).

The API promises to deduct only when the entire amount is affordable. The implementation calls the partial-taking helper and only then returns whether enough was collected. With a balance of 5 and a charge of 10, it takes 5 and returns failure. `enterprise_base::OnRetail` explicitly documents this defect and works around it with precheck/partial-charge/refund logic; that does not fix the public contract for other consumers.

Related current weaknesses:

- `Api_StateToCitizen` ignores failure from `WalletAdd`, then debits the treasury even if the recipient is invalid.
- `Api_CitizenToState` restores wallet money on insufficient funds, but the `person+PERSON_SPENT_OFFSET` increment from `WalletTake` is not rolled back there.
- Several money entry points reject NaN but accept infinity. `WalletAdd` can create an infinite balance; `Api_MoveTreasury` can apply infinity. `enterprise_base::ApplyCapital` similarly rejects NaN but not infinity.
- `Api_MoveTreasury` fires a flow event even if its internal treasury write could not occur because the game object was unavailable.

**Next:** implement atomic full debit under the wallet lock, central finite-value checks, and explicit internal success propagation for treasury/wallet legs. Keep the published ABI stable initially. Test insufficient funds, invalid recipient, nonfinite inputs, failed treasury access, and spent-counter rollback. Do not solve this by adding more caller-side prechecks.

### B. High — invalid sidecars can retain another world's state (confirmed)

**Evidence:** `currency::LoadWallets` at lines 437–500 and `enterprise_base::LoadRegister` at lines 4024–4120.

Both clear state when the sidecar is missing, but return early without clearing it when a present sidecar has an invalid header/version/length. Loading world A, then world B with a corrupt sidecar, can therefore leave A's wallet map or enterprise registry/pool/pending state in memory. Enterprise runtime pointers are particularly unsuitable for crossing a world boundary.

**Next:** define one explicit per-world reset routine per plugin. Reset at the load boundary independently of successful deserialization; parse into temporary validated state and install only on success. Specify whether corruption disables the subsystem or uses clean defaults, and warn visibly. Test A→B with missing, truncated, bad-magic, unsupported-version, excessive-count, and nonfinite records. Audit labor, shop, construction, tax, and bank loading under the same matrix instead of assuming they all share this defect.

### C. High — construction loses simulation time when throttled (confirmed)

**Evidence:** `private_construction::OnTick` at lines 1893–1905 and `RunConstructionPass` at lines 1621–1636, 1766, and 1824 onward.

`OnTick` returns until `g_tickIntervalMs` wall-clock milliseconds have passed. On the accepted tick it passes only that tick's `dayDelta`; skipped tick deltas are never accumulated. Both construction progress and handoff waiting time use this reduced delta. For example, accepting one of ten equal ticks advances roughly one tenth of the intended simulation-time progress, before the per-pass clamps. Tiny accepted advances may also fall below the `0.00001f` step threshold.

**Next:** accumulate validated simulation deltas on every tick, consume them on the work cadence, and specify how progress clamps retain or deliberately discard backlog. Reset accumulation on world load and handle pause explicitly. Test equal elapsed simulation time at multiple tick rates, speeds, and throttle intervals; the result should be invariant except for documented clamps.

### D. High — callback ordering is an implicit economic policy (confirmed design gap)

**Evidence:** loader `LoadPlugins`/`StartPlugins`; `sim_clock::Register`/`FireScheduled`; `enterprise_base::OnMonth`; bank monthly callback; taxes' scheduled registrations; ledger tick closure.

The loader relies on filesystem enumeration (its comment assumes NTFS alphabetical ordering) rather than explicitly sorting or dependency scheduling. The clock invokes callbacks in registration order. Publishing all APIs before Start does not guarantee that providers have completed Start, nor that taxes/bank interest occur before enterprise totals are snapshotted/reset and profits siphoned. A later tick-based ledger snapshot sees the effects of this implicit ordering.

**Next:** first write a month-close contract, including which month boundary-day payroll/rent/taxes belong to. Then enforce explicit phases/order and test a complete month with rent, wages, VAT, profit tax, bank interest, pool transfer, and ledger closure. Sorting filenames alone makes order reproducible, but is not an adequate financial phase model.

### E. Medium/high — bank identity failure aliases loans (confirmed fallback; frequency unknown)

**Evidence:** `republic_bank::keyForBuilding`, lines 40–70; `LoanBook::makeKey`, lines 128–134.

The bank tries inline and pointer interpretations of `building+0x320`. If neither yields accepted coordinates, it silently uses `(0,0)` with the object name. Two same-type buildings whose reads fail share a loan key. The test for coordinates also rejects valid positions where both absolute coordinates are at most 1.5. The bank's coordinate logic differs from the enterprise persistence path.

**Next:** introduce a shared validated building identity adapter, returning failure rather than a fabricated origin. Refuse new debt operations when identity is uncertain; display/log why. Plan migration for existing loan files before changing key semantics. Test failed reads, origin-near buildings, demolition/rebuild, and save reload.

### F. Medium — loader build target points at a nonexistent source (confirmed)

**Evidence:** `build.bat` loader branch checks/builds root `tesmioloader.cpp`; actual source is `src/tesmioloader.cpp`.

An explicit `build.bat loader` skips the requested build and falls through to plugin matching with `WHICH=loader`. There is no matching plugin, so this does not produce a loader. Also, an unknown plugin name has no explicit zero-match failure. The common flags contain the same include directory twice and do not quote the include path for a repository path containing spaces.

**Next:** repair the explicit loader target and linker requirements, initialize counters before all branches, reject unknown targets, and quote include arguments. Verify exit codes and actual output artifacts on Windows; add a documented launcher build target if it is intended to be maintained here.

### G. Medium — persistence ownership crosses the static CRT boundary (confirmed contract conflict)

**Evidence:** `/MT` in `build.bat`; host ABI's no-cross-boundary allocation principle; `persistence::ReadFileImpl` allocates with `malloc`; `persistence_api.h` tells consumers to `free` that buffer in their own DLL.

This contradicts the repository's memory-ownership rule. It should not be described as a reproduced heap crash: modern runtime/allocator behavior can mask it. It is nevertheless an unsafe ABI contract to preserve as the suite grows or build settings diverge.

**Next:** append a provider-owned buffer-release function with size/capability checks, or introduce a caller-owned-buffer read contract. Update consumers and compatibility handling together. Never insert fields into an existing table or simply switch all plugins to a different CRT to hide the ownership issue.

### H. Medium — fault isolation does not ensure lock recovery (source-confirmed pattern; fault injection needed)

**Evidence:** `currency::WalletTake` enters its lock before a raw person-memory write; `private_construction::RunConstructionPass` and `enterprise_base::RunRegistrationPass` hold locks across native-memory processing. Their outer callers catch faults and continue/skip.

If an access violation occurs after a lock is entered and before the corresponding leave, outer SEH recovery does not execute that leave automatically. Recursive Windows critical sections can hide this on the same thread while other threads subsequently block. Disabling a callback is not sufficient recovery from a stranded lock or half-completed transaction.

**Next:** isolate native reads/writes from locked policy/state mutation, use compatible guaranteed cleanup (`__finally` or carefully designed RAII outside the MSVC C2712 restrictions), document lock order, and fault-inject at native-access boundaries. Avoid blanket wrapping whole STL-heavy functions in `__try`.

### I. Medium — failed plugin lifecycle cleanup is incomplete (latent loader defect)

**Evidence:** `LoadOnePlugin` frees a DLL after nonzero Init but does not remove services already published by that Init. `StartPlugins` merely logs a nonzero Start result; published APIs remain discoverable.

Current normal Init paths generally publish near successful return, so this is not proof of a present normal-start crash. However, a fault/late failure after publication can leave dangling service pointers, and a failed Start can leave a discoverable but unready service.

**Next:** track publications by provider, roll them back on failed Init, and distinguish published/ready/degraded services. Once hooks exist, do not unload the DLL without a proven unhook protocol. The existing loader correctly recognizes that successful hooked plugins need to remain pinned; preserve that rule.

## 3. What is already good, and what remains unproven

### Retain these foundations

- Separate domain services and versioned POD APIs are a good fit for independent native DLLs.
- Native hook expectations, address gating, bounded observer tables, and fault-containment callbacks demonstrate real attention to failure modes.
- Enterprise private retail already refunds on a partial-payment race or failed company credit; do not regress those safeguards during cleanup.
- `private_shops::RunWholesaleBilling` switches to `WholesaleMarketOnlySlot` when live B2B counterparty settlement is advertised. The duplicate-billing concern is therefore an **integration regression requirement**, not a newly proven unconditional double charge.
- Logistics retains multiple seller/resource/price lots and explicitly refuses uncertain assignment-fuel charges. Preserve unknown-start/gap/identity/discovery rejection rather than loosening rules to increase billing coverage.
- Persistence atomic replacement bounds file sizes and checks short writes; building persistence generally avoids raw pointer serialization.
- Bank and ledger have portable pure accounting cores. Logistics tests include runtime adapters/stubs as well as pure state machines, not only text matching.

### Logistics status must not be oversold

The tracked `private_logistics.ini` is a **permission-test profile**, not a general all-dry-run release preset:

- `billing=0`, `dry_run=1`: settlement does not move money.
- `auto_targets_enabled=1`, `auto_targets_dry_run=0`: automatic native target assistance is live and independently controlled.
- Source/destination thresholds are 10/30. Several diagnostic switches and the separate truck probe are on.
- The INI and startup code explicitly say native Test16 event coverage, physical fuel units, and exact fuel burn remain unverified.

Older `NEXT_PHASE.md` describes v0.17 and dry-run target additions, whereas current code/config and tests reflect later permission/reconnection work. Treat that file as historical staging context, not the current executable contract. Do not blindly flip today's flags based on it.

Before enabling live logistics payments, validate one native load/delivery, residual/mixed lots, same-firm transfers, storage-polling suppression, receiver changes, refueling/observed rises, observation gaps, save/load during transport, and missing prices/identities. Then expand to multiple DOs. Use controlled disposable saves and capture transaction-level evidence. A host-test pass cannot establish native call-site coverage or physical fuel units.

### Other investigations to schedule, not assume complete

- **Clock/world epoch:** date-jump heuristics cannot identify all loads (same date or exactly one day later). Add an explicit load epoch from persistence, coordinated with safe native-world readiness, and test same-date/mid-month reloads. Current logistics epoch resets are useful precedent.
- **Wallet population:** unmaterialized citizens receive a starting balance on lookup, but total-wallet statistics sum only map entries. Pruning idle wallets can also forget living-but-unobserved people. Define whether starting grants and inactivity eviction are policy or bookkeeping approximations; then test money-supply reports against that definition.
- **Native safety:** audit actual hook instruction boundaries, preserved registers/stack alignment, custom code cache flushing, and RVA coverage on the deployed executable. The generic helper allocates a fixed 64-byte trampoline without a corresponding maximum-stolen-length check; current call sites need individual validation before claiming an overflow is reachable.
- **Version gating:** `TsmAddrInit` deliberately accepts missing/synthetic or unparseable PE images for tests. Separate that test convenience from production fail-closed policy. A timestamp match alone is not binary authentication.
- **Performance/threading:** registration walks all buildings roughly every 300 wall-clock ms while holding the enterprise lock. Construction also walks the building span under its lock, and persistence dispatch holds its own lock during handlers/I/O. Measure frame/tick latency and lock wait before introducing caches. Confirm actual native callback threads instead of assuming documentation describes every hook.
- **Recovery UX:** a faulted clock subscriber can silently stop wages, tax collection, or another economic process for the rest of the session. Add a compact degraded-system health state rather than relying on a log line. Coordinate presentation contracts with the Almanac contributor; do not develop a competing dashboard.

### Older audits are not a ready-made backlog

`docs/remaining-logic-bugs-audit-2026-10-22.md` contains findings, suggested patches, old line numbers, and speculative claims mixed together. Several are no longer current:

- The wholesale ratio clamp is present; moreover, today's native wholesale path bills debt without erasing delivered inventory.
- Persistence path-component matching and load/save debounce invalidation now live in tested `save_path_match.h` helpers.
- Current permission/reconnection tests exercise entry activation, not just the old disabled-entry dedup assumption.
- The old suggestion to salt a persisted building key with a raw pointer would violate save stability. Do not implement it.
- Copying a service pointer to a local does not make unloading its DLL safe. The loader pins installed plugins; lifecycle safety needs a real protocol, not a pointer snapshot comment.

Archive/supersede claims only after checking current source and recording their disposition. Do not delete raw native evidence as part of cosmetic cleanup.

## 4. Validation actually performed

### Portable tests

1. Ran all 27 `test_*.py` scripts using the README's direct-execution pattern: all exited successfully; unittest summaries reported **296 tests**.
2. Noticed that some files define test classes but have no direct-execution entry point. Re-ran all 27 as dotted `unittest` modules: **312 tests passed**. The difference includes tests that the documented command otherwise silently skips; module loading also collects additional imported test cases in one suite.
3. Independently compiled and ran `tests/resource_pricing/scarcity_curve_test.cpp` with C++17 and `-Iplugins/resource_pricing`: PASS.
4. Independently compiled and ran `tests/mock_host/test_persistence_debounce.cpp` with C++17 and `-Iplugins/persistence`: OK.
5. Ran `python3 tools/audit_economy_csv.py selftest`: PASS.

A first attempt at unittest invocation mistakenly used slash-separated module names without `.py`; it failed import resolution. It was corrected to dotted module names, and the complete corrected pass above succeeded. Those import errors are not codebase failures.

Corrected shell invocation, from the repository root:

```sh
for f in $(find tests -name 'test_*.py' | sort); do
    module=$(printf '%s' "${f%.py}" | tr / .)
    PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 -m unittest "$module" || exit 1
done
```

Scratch logs are in ignored `scratch/audit/` on this workspace, not published evidence artifacts. The summary above is retained here. No Windows DLL or launcher compilation, in-game test, fresh disassembly, or benchmark was performed. The source-confirmed defects above are not all covered by the existing tests.

### Coverage assessment

There is no tracked `.github` CI directory in this checkout. Tests are heavily concentrated in logistics/probe/model behavior, with smaller bank, ledger, pricing, shop, and workflow checks. There are no comparable dedicated wallet, complete payroll, rent/eviction, construction cadence, full month-close, or cross-world corruption suites. `tests/mock_host` is useful scaffolding, but its main mock header includes Windows-dependent production headers; it is not evidence that the complete host/plugin suite compiles portably on Linux.

The documented test command also omits standalone C++ cases unless a Python wrapper invokes them. A single supported runner should inventory and execute every intended suite, report counts, and fail when required tools or binaries are missing. Distinguish source-text assertions, pure policy tests, stubbed native adapters, Windows builds, and actual game tests in its output.

## 5. Recommended execution plan outside Almanac

### Phase 1 — establish a trustworthy safety baseline

1. Add one test runner, including the currently unwrapped C++ tests; fix README invocation and add Linux portable-test CI plus Windows x64 MSVC build CI.
2. Repair `build.bat` target resolution and output checks. Keep release configuration deployment separate from compilation: today's build overwrites destination INIs with tracked test-profile INIs.
3. Repair the currency contract with a small portable wallet/transaction core and regression tests. Avoid changing economic prices or taxes in the same patch.
4. Fix corrupt-sidecar world isolation and construction elapsed-time accumulation as separate bounded changes, each with its own tests.

**Exit gate:** every declared test runs, full intended DLL builds are reproducible on Windows, and insufficient/invalid transactions and corrupted world transitions do not alter unrelated balances/state.

### Phase 2 — make integrations deterministic

1. Specify economic phases and month-close attribution; test and enforce ordering.
2. Add a shared validated building-identity adapter and bank failure policy, with save migration tests.
3. Migrate persistence buffer ownership safely; validate malformed record lengths/counts/strings/finite values throughout the suite.
4. Add explicit service readiness/degradation and failure-publication cleanup.
5. Review lock boundaries, lock ordering, and fault cleanup. Separate game-memory sampling from locked calculations; then benchmark large worlds.

**Exit gate:** a synthetic complete month conserves money according to declared sources/sinks, and A→B→A loads do not leak state or retain stale native identities.

### Phase 3 — targeted game investigation, not broad speculative reverse engineering

1. Confirm the deployed executable identity, loader version, actual INI paths, and artifact/source correspondence.
2. Run the controlled logistics matrix in section 3, first observation-only settlement, then deliberately authorized live billing on disposable saves.
3. Validate construction rate invariance and native completion handoff; verify save/reload during construction.
4. Validate housing eviction, insolvency abandonment, wages, and month-close accounting over several months in both RUB and USD configurations.
5. Capture large-world timing/lock/log-volume data. Turn proven diagnostics into opt-in probes and retain a quiet release preset.

**Exit gate:** native evidence matches policy tests; no unpriced/ambiguous movement is silently presented as verified accounting. Money conservation and goods conservation are tested separately.

### Phase 4 — organization and polish

- Extract from enterprise code in this order: validated state codec/reset; transaction helpers; pure wage/rent/insolvency policy; UI snapshot formatting. Keep existing API façades and native adapters in place.
- Keep native hook bytes and offset provenance close to the adapter, with cached evidence links. Do not scatter magic numbers further.
- Create a concise current-status matrix per subsystem: implemented, host-tested, Windows-built, game-tested, enabled by default, blocker, evidence, owner. Separate test profiles from release profiles.
- Mark historical staged reports superseded without destroying their evidence. Repair stale developer claims such as the old plugin count, root loader source, and persistence trigger description (`header.bin` in the API comment versus current `workers.bin` default).
- Add a short release validation checklist and rollback/save-backup instructions. Preserve player INIs during ordinary rebuilds unless replacement is explicitly requested.

### Features worth considering only after those gates

1. **Explicit unpaid-invoice/debt policy:** goods physically delivered but unpaid should produce an auditable liability or another consciously chosen outcome. Tie this to the existing bank/default model rather than inventing a second invisible overdraft system.
2. **Carrier cost model:** decide whether fuel-only reimbursement is the intended economy. Maintenance, wages, leases, depreciation, and margin need design approval and evidence—not inferred native units.
3. **Credit/default lifecycle:** define debt treatment after abandonment, demolition, nationalization, and rebuilding. Stable identity is a prerequisite.
4. **Ownership-aware pricing/policy controls:** extend only where the existing UI/service contracts and Almanac data ownership agree.

Do not begin multi-DO reservations, a new carrier-cost model, dynamic household fees, or a second economy dashboard merely because they sound like natural extensions. Current correctness and the intended gameplay model must determine that backlog.

## 6. Questions for the project owner

1. **What is the next milestone?** A stable playable economy, completion/live validation of logistics, or a feature-complete research prototype? My default recommendation is stable playable economy, with targeted logistics validation alongside it.
2. **What is actually deployed and broken today?** Which executable build, plugin source/artifact versions, enabled INIs, and repeatable symptoms are the current ground truth? Can the current Windows operator supply a clean build and disposable-save runtime results?
3. **Is switching saves without restarting supported?** Current probe guidance says one launch/one save, but other systems attempt load resets. If restart-only is an intentional temporary restriction, document and enforce it; do not silently promise broader support.
4. **Which economic choices are intentional?** In particular: lazy starting-wallet grants, idle-wallet pruning, billing delivered goods into debt, fuel-only carrier compensation, and the ordering of tax/interest versus profit siphoning.
5. **What does the Almanac contributor own beyond the view itself?** Confirm shared API/data-snapshot, telemetry schema, and integration-test ownership before either person changes those contracts.

## Suggested first coding task

Start with **currency transaction correctness plus dedicated tests**, followed by **construction time accumulation** and **cross-world reset tests**. These are bounded, high-value changes mostly independent of the Almanac implementation. Do not combine them into a large refactor. Before live logistics billing or a release, establish the Windows build and controlled-game validation gates above.
