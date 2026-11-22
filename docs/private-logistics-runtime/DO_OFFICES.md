# How distribution offices work in WRSRE

> **2026-11-22 owner decision:** B2B billing must be **live by default** (`billing = 1`, `dry_run = 0`).
>
> **Current source:** `private_logistics` **`0.19.0-simplified-settlement`**. With both switches set that way, the plugin settles for real (`cb.dryRun = g_dryRun != 0 || g_billing == 0`):
> - The unloading buyer pays the verified loading seller.
> - The unloading buyer pays the vehicle-owning private DO's observed trip fuel.
>
> Before the defaults flip, see [economy direction §4.4](../visions/economy-direction-2026-11-22.md):
> - Remove the `private_shops` procurement and delivery billing, so the same delivery is not settled twice.
> - Remove the market multiplier.
> - Exclude customs houses from `b2b_state_buy`.
>
> The 0.18.x text below, including "billing OFF" and "`billing=1` would opt into legacy behavior", is a historical record of the dry-ledger period.

**Current working source:** `private_logistics` **0.18.19-road-targets** (bounded road target policy; host-tested, not yet Windows-built/deployed in this review). Last observed native version: **0.18.18-census-guard**, merged in [PR #43](https://github.com/Michu6636/WRSRE/pull/43) at `2d83fdc41d99f0a98ab00ea2ae2292dc6e78ba2f`, following **0.18.17-dry-ledger** from [PR #42](https://github.com/Michu6636/WRSRE/pull/42). The user reports a successful Windows build; Test15 now logs this version running natively across 30 daily summaries. It exposes the existing five-total-target automatic-connection restriction and unresolved secondary-goods dispatch symptoms; it is not a clean overall dispatch acceptance, a >64-slot stress test or a source/binary hash-parity check.
**Documentation review:** updated 2026-11-04 with the bounded target-policy implementation. All 147 private-logistics host tests pass (including extracted production reconciliation/preflight, optimized and ASan+UBSan); this is not a Windows build or native gameplay result. See `docs/tasks/active/TEST16_ROAD_TARGETS.md` for the exact policy and pending acceptance. Source behaviour and native observations remain distinguished; a logged version is not byte-for-byte compiler/source parity.

## 1. The short version

A private distribution office (DO) uses the game's trucks, roads, storage rules and planner. The mod helps maintain its permitted connections, observes private road vehicles, and keeps a saveable notebook of approximate assignment fuel costs. **It does not currently turn that notebook into real bills or payments.**

There are three separate layers:

- **Connections:** which sites the private DO is allowed to load from or deliver to; reconciled once per game day.
- **Observation:** inspect eligible road vehicles and their task/fuel readings, at most approximately once per real-world second while simulation callbacks run.
- **Dry accounting:** associate sampled decreases with observed assignments, freeze estimates at closure, and save records when the game saves.

Automatic target assistance includes road and rail DOs in source. The current panel-independent fuel/cost observer is **road-only**; rail fuel accounting is not validated by it. None of these layers replaces native dispatch or teleports goods.

## 2. Who controls connections?

The player controls access to state-owned sites, not each private company's list of customers. A private DO autonomously maintains eligible connections. Removing an eligible target manually is **not a permanent veto**: it can be added again at the next daily reconciliation. To deny access to a state site, change that site's permission instead.

The daily pass discovers eligible sites, rechecks ownership and permissions, reconciles existing tasks and adds missing eligible targets. It keeps existing entries rather than deleting the whole list. However, it **does apply company direction/resource-selection and percentage policy to managed existing tasks, including manually added ones**. This is not a promise to preserve every manual setting. Manually configured customs targets are an explicit preservation exception; automatic customs selection is not implemented. State-owned DOs are not taken over by this automation.

The working source limits automatic additions to **20 total entries per private ROAD office; RAIL remains five**, counting all existing/manual entries. These are conservative mod policies, not proven engine maxima or per-day/batch budgets. Existing entries above the limit are never deleted to make space. Road uses 20 rather than 49/100 because the reviewed road renderer has both `/49` and `/20` modes, while its closed creator/receiver chain initializes 100 per-target DWORD controls. Twenty stays inside both display policies without reading a live UI-mode field. The native entry/config allocation, append, dirty notification and timing path are retained; the append boundary rechecks the live count. **Test15's old five-total policy caused 411 omissions**, including meat storage and a shop_prior destination. The source fix removes that bottleneck up to 20, but new native UI/dispatch behavior remains to be validated on Windows. A target still present but unreachable is not treated as missing or duplicated. Native roads, cargo compatibility, storage input/output acceptance and planner decisions still determine whether a truck can actually work; a permitted connection does not guarantee a dispatch.

## 3. Loading, deliveries and stock thresholds

Directions are named from the truck's point of view: **LOAD takes goods FROM a building; UNLOAD delivers goods INTO it.** State-site directions default OFF and are stored independently. On a recognized state warehouse/storage or factory, loading and deliveries can be authorized separately. A recognized retail/service consumer is delivery-only: it must not become a source just because it has storage. Unknown roles receive no guessed permissions. The role classifier has two explicit warehouse asset exceptions; this is not universal Workshop-asset recognition.

Private eligible sites follow their role without state-access checkboxes: recognized storage/factories may support both directions; consumers remain unload-only. Saved state permissions do not restrict a site while it is private. If it returns to state ownership, its saved public permissions apply again, or default deny if none exist. A checked access control means authorization, not that a connection or a truck dispatch has already happened.

Current company policy uses **source above 10% / destination below 30%**. These are stock triggers for the native planner, not a spending limit, cargo amount, guaranteed final stock level or physical fuel constant. Allowed directions get those configured thresholds during daily reconciliation. Denied directions are disabled by their wildcard/resource selectors, not merely by writing a zero percentage; an unused disabled percentage is left alone to avoid repeatedly dirtying the office. Manual native percentage edits to managed targets can be overwritten by company policy on the next daily pass. The working 10/30 settings were accepted through the earlier dispatch tests; they are not a universal proof of native threshold behavior for every asset.

## 4. What counts as an assignment?

The observer keeps independent histories for eligible road vehicles without requiring their panels to remain open. It follows sampled task information, **not a verified native job-start/job-finish callback**. A relevant task appearance opens an assignment candidate. If it was already underway when first seen, the beginning is marked unknown. Scalar/index-only changes and internal stop advances retain identity when the observed target identity stays the same.

Observed task removal, office-only return patterns, target replacement or other supported structural changes close the old candidate regardless of delivery success. Replacement starts a separate record; it does not move the old estimate to the new target. A lost vehicle/identity or unsupported continuity break is instead interrupted/unpriced, not invented successful completion. Invalid observations are marked as gaps rather than used to manufacture fuel deltas. Sampled boundaries can miss short-lived work between observations; a transfer callback is not automatically one whole journey.

## 5. How approximate fuel cost is calculated

For valid, continuous comparisons, add **positive decreases** in the signed native quantity readings. Record increases separately; never subtract them from earlier decreases. For example, readings `0.80 → 0.70 → 0.95 → 0.85` contain decreases of `0.10 + 0.10 = 0.20` and an increase of `0.25`. A refill-like rise therefore does not erase previously observed consumption-like drops. Root/member quantities are compared separately, so a rise in one observed object does not hide a drop in another. Quiet active samples update the ledger even when no assignment-change line is logged.

This is **not an exact burn meter**. Sampling cannot reconstruct all fuel use/refilling between readings, nor prove that every change is fuel burn. Gaps, unknown starts, rises, uncertain roles/receivers and incomplete fleet discovery are disclosed as partial/uncertain. Physical units remain provisional. The accepted starting approximation is not calibrated by purchase totals or a screenshot difference. Ordinary pauses do not alone break matching vehicle/assignment history; the next valid sample can compare across the pause, but nothing between samples was continuously observed.

At observed closure, the dry ledger freezes:

**estimated amount = accumulated sampled decrease × available cached fuel price per tonne**

The native-quantity-as-tonnes assumption is provisional. With the example's `0.20` provisional quantity and an illustrative quote of `200` per tonne, the estimate is `40` currency units. That example is not measured physical consumption or a bill. The shared price cache refreshes approximately every **30 real-world seconds**, checked on eligible observation passes; closure uses the then-cached quote, not a fresh historical reconstruction of each fuel purchase. Later price changes never reprice a closed record. No usable quote or no compared samples means unavailable cost, distinct from a valid compared zero decrease producing an available zero amount.

This new estimate is fuel-only: it does not add a new profit margin, wage, depreciation reserve, vehicle purchase charge or upkeep fee. Older `litres_per_trip`, wear/margin, CPI-fallback and producer-payment settings describe separate legacy transfer-fragment models, not this ledger's formula. `billing=1` would opt into legacy behavior, **not finish or enable this new ledger's settlement**; the new observer/ledger requires billing OFF.

## 6. Original target, cancellation and who would pay

On opening/adopting a candidate with exactly one original unloading target, the adapter tries to capture a bounded asset name, building type and exact x/z position. It checks unique current-world membership before reads and rechecks the snapshot context. Stored records contain that tuple and a diagnostic key, **not raw building/vehicle pointers**. If safe identification fails, keep the record unresolved; do not guess a replacement payer later. The permission-key heuristic is not promoted into an immutable payer ID.

The approved allocation rule is: **the original target remains responsible when its observed assignment is cancelled, removed or replaced**, not automatically refunded or charged to the replacement. The dry record retains that original snapshot even if the building disappears before closure. Travel after observed closure, return-to-office activity outside an assignment, and other unassigned decreases are not allocated to that target. They are outside-assignment overhead; this ledger does not create a separate payable overhead account.

A stable saved snapshot is not verified company/wallet ownership and cannot distinguish a rebuilt identical asset at the same position. An estimated cost can exist with an unresolved target, but `would_owe_available` is false without a valid snapshot. Even when true, it means only a dry estimate associated with that snapshot. Logs retain `payer_verified=0`, `actual_debt=not_created`, `settlement=none`, `billing=off`. **No customer debit, carrier credit, unpaid-debt settlement or new money is created.** Verified economic parties and duplicate-proof settlement remain future work.

## 7. Saving and loading

There are two distinct kinds of mod data: state-site permissions in `private_logistics_state.bin`, and the new cost book in **`private_logistics_costs.bin`**. Both use the existing save service, not a new per-second disk writer. The game's native save owns its DO tasks; these files do not replace the native task/vehicle save format.

Closed cost records restore their frozen quote/amount and save-scoped record numbers. Saving an **active** assignment writes an **interrupted/unpriced copy**, preserving the original target snapshot and sampled totals. The live in-memory assignment continues unchanged. If that save is loaded, the saved interrupted record is not joined to a current vehicle: there is no sufficiently verified save-stable native assignment ID to resume it safely. This preserves evidence without inventing completion, a closing price or continued identity. A newly observed post-load candidate is separate, not proven continuation of the old one.

The explicit versioned format checks size, checksum and record semantics before publishing a loaded book. Its checksum detects accidental corruption, not malicious alteration. Missing files initialize an empty book; corrupt, inaccessible or unreadable existing files block ledger writes instead of being replaced with an empty history. Atomic-write failure is reported and relies on the existing persistence service to retain the previous committed file. Shared data is protected by the plugin lock; disk I/O and native identity reads are outside it. Load-generation checks reject stale observations. This architecture is host-tested, not proof of actual native thread scheduling or filesystem behavior.

## 8. Timing and performance: why a real-world second?

`sim_clock` calls the observer on simulation ticks. Most calls do only the early guards/time check and return: if less than 1,000 ms has elapsed, there is no full fleet pass. An eligible pass reads fleets, fuel and task snapshots. This is **not a new timer thread**, a per-tick full world scan, or a one-second replacement for native dispatch. Daily connection reconciliation, one-second observation, 30-second cached-price refresh and save-event persistence have separate purposes. The price interval refreshes a reference; it does not set how often the game's economy changes prices.

One second is a sampling/workload compromise: every tick would do more work; much slower polling would miss more short assignments and changes. At faster game speed, more simulated activity occurs between real-world samples, so coverage can worsen even with a similar number of passes per real second. After an ordinary pause there is no synthetic catch-up sample for each missed second. The next eligible callback samples current values and retains comparison continuity only when other guards allow it.

The work is bounded, **not benchmarked as negligible**. Current fleet discovery examines at most **32 enterprise-register entries** (not 32 offices), tracks at most **64 vehicles globally**, and validates office fleet vectors against a **256-root** bound. Exceeding coverage limits is reported; unobserved vehicle costs are not invented. The separate cost book holds **4,096 append-only records**, at most **802,844 serialized bytes**. When full it warns/refuses new records rather than evicting prior history; production history retention/export remains future work. Normal observation uses fixed-size structures with no routine heap allocation, and routine logs are throttled. Save/load allocate bounded buffers on the file callback, not each observation tick.

Nontrivial costs remain: guarded memory reads and repeated membership checks, task inspection, occasional target-identity scans over the world building vector, log output and lock contention. The daily connection pass separately scans registered sites/world infrastructure; its limits are not the fleet observer's 32/64 limits. File serialization also takes the shared lock while copying/encoding the book, although disk I/O is outside it. A representative large-save benchmark should measure typical and worst observation/daily-pass times, boundary bursts, game-speed coverage and save/load stalls. No such native timing result is claimed here; bounded loops alone are not a performance guarantee.

**Read-only scale review (2026-11-02):** the cumulative report now traces fixed-prefix discovery starvation, repeated fleet validation, burst-time world identity scans, daily memo/target loops, finite history, synchronous logging and save-lock/I/O costs. Most importantly, the older daily census can obtain slot `-1` after its separate 64-slot lifetime table fills and then read `g_vehFuelValid[s]` unchecked (`private_logistics.cpp:3398`). This is a source-level safety defect, not an observed crash in the supplied runs. A separately authorized guard/regression fix should precede deliberate large-fleet stress; runtime was not changed by the review. Test13/14 cover only two observed roots in one office and 35–36 world entries, not a scale benchmark. The report gives a minimal proposed phase-timing/coverage plan; instrumentation, deployment and stress testing remain unapproved.

**Authorized safety follow-up — 0.18.18 source:** the census now checks its slot before all per-vehicle diagnostics, skips only unsupported visits and emits one `[VEH COVERAGE]` summary per census when capacity omits visits. Existing slot identities and the 64-slot limit are retained; daily reconciliation, the newer fleet observer, ledger format and billing policy are unchanged. The actual pre-fix census reproduces a negative-index error under host sanitizers; the fixed extracted census passes eight scenarios in optimized and ASan/UBSan runs. The complete suite passes **144 tests**. This is host evidence, not Windows/native validation or a large-save benchmark. No timing instrumentation or deployment was added; discovery fairness/history retention remain separate gaps.

### Future idea only: game-week price refresh

The user proposes slower in-game price changes, perhaps weekly. **Neither a weekly price-update rule nor a weekly observer-cache scheduler is implemented by this documentation.** If the pricing system later publishes changes at defined game-week boundaries, the observer could synchronize cache invalidation to those boundaries, plus initialization/load and any supported exceptional price-change event. Week definition, clock API/event, callback ordering, invalidation and unavailable-price handling still need a separate design/implementation. Merely changing the observer's cache would not make the underlying prices weekly.

Fuel/task observation would remain independent and frequent enough for its coverage target; weekly prices do not imply weekly fuel sampling. Under the currently approved closure-freeze policy, an assignment spanning a price boundary would use the applicable cached quote at closure, not retroactively reprice old closed records or automatically split consumption by week. No interval or economic rule changes without separate authorization.

## 9. Current settings and what the player can see

The checked-in INI has `enabled=1`, **`billing=0`**, `dry_run=1`, `auto_targets_enabled=1`, **`auto_targets_dry_run=0`**, `auto_source_percent=10`, `auto_destination_percent=30`, `panel_fuel_observer=1` and `schedule_observer=1`. These are source defaults, not a claim about the installed INI. Target dry-run and payment dry-run are different switches: target writes are currently allowed, but payments are OFF. Setting `auto_targets_dry_run=1` would make target reconciliation report-only; that is not required to keep the cost ledger non-paying. Legacy per-leg payment settings do not override `billing=0`.

Fleet observation requires the existing diagnostic switches, matching executable fingerprints, tick/load services and enterprise discovery. The durable ledger additionally requires a working persistence API/handler. It fails unavailable rather than creating unverified hooks or guessing data. The diagnostic panel observer can coexist, but selected-panel assignment events do **not** feed the durable fleet ledger; there is no double billing through both observers. Reporting is currently logs and saved records, not a new invoice-management UI: `[AUTO DO]` describes target policy, `[FLEET FUEL]`/`[FLEET ASSIGNMENT]` describe observations, and `[DRY COST]` describes saved-accounting estimates/status. A new cost record is not a `[PAY]` transaction.

## 10. What is proven and what comes next?

Earlier player/log tests support the working 10/30 DO's connections, pickups/deliveries and dispatch save/reload. Later fuel-observer runs support the accepted initial approximation across refill-like rises and ordinary live pauses. Test12's named saving succeeded; expected `saved_last` errors on exit without saving are not a save blocker. Those checks do not prove that the new durable cost ledger reloads correctly in the game.

PR #42 recorded **141 Python tests**, plus **29 fleet + 12 dry-ledger C++ scenarios** passing in optimized and ASan/UBSan builds. Host shims cover closure/replacement allocation, unknown identities, frozen quotes, quiet samples, corrupt-file handling, epoch gates, simultaneous vehicles and capacity. They do not validate native offsets/lifetime, Windows SEH, actual payer ownership, filesystem atomicity or native performance. The preserved compiler transcript applies to **0.18.15**, not **0.18.17**; source/binary parity for the newer user-installed runtime has not been established.

**Test13 live update (2026-11-02):** the user ran **0.18.17-dry-ledger** with payments OFF. The reported factory demolition produced one office-only closure retaining the original factory snapshot and a **1.8025194 RUB estimate**, not a debt. Later return fuel was not added to that closed assignment. Two earlier autosaves reported successful ledger writes (3 records / 616 bytes each), but both preceded demolition: this cancelled record's persistence is unproven. The initial empty-book load does not prove closed-record restoration. The cumulative report preserves the complete log, screenshots, helper and evidence limits. This is actual native dry-runtime evidence, not a new compiler transcript, meaningful reload result, exact-burn validation or performance measurement.

**Test14 update:** `autosave5` now loads a **nonempty three-record book** (`loaded=1`, zero interrupted/missing/blocked). Two autosaves write 3 records / 616 bytes; the named save **`26741 - My republic`** writes **4 records / 812 bytes**. One new assignment opens before that save and none closes: the fourth row is consistent with the saved interrupted/unpriced copy of an active assignment, not a completed cost. Its stage is inferred from source, not printed. Exact restored IDs/quotes and reloading that fourth row are not yet compared. This supersedes the blanket “nonempty native ledger reload unverified” limitation, not the narrower Test13 demolition-record persistence limit: that cancellation occurred after the loaded autosave. No repeat basic saving or demolition is required.

**Test15 update (2026-11-03):** the next native load reports `records=4 interrupted_unpriced=1 missing_file=0 blocked=0 prior_active_not_rebound=1`. The four-record/interrupted classification is now observed, not merely inferred; exact restored row identities/amounts are still not compared. The log also runs 0.18.18 through 30 daily summaries, with the target-cap/secondary-goods problems discussed above. Full Test15 evidence is in the cumulative report.

Next steps, each within its own approval scope:

1. Treat Test13's observed 0.18.17 startup/closures/save callbacks as completed native evidence, not a pending blanket rebuild/redeployment requirement. A matching compiler transcript/binary hash would separately establish build provenance if needed.
2. Preserve the successful cancellation/original-target result and Test14 named save/nonempty reload. Do not require repeated simple saves, demolition or broad baseline/pause research just to reconstruct a cancelled record absent from the earlier save.
3. Further validation can compare exact restored closed-row fields and interrupted-copy contents; Test15 already observes the four-record/interrupted classification and no-rebind flag, so a basic repeat is not required. Stale-world isolation remains a separate case. These field-level loading checks can wait; the current priority is the target/secondary-goods dispatch problem.
4. Measure large-save observation/daily-pass spikes and save/load overhead. Improve coverage/retention if necessary; do not call the current bounds an unlimited production system.
5. Before real settlement, verify original-customer and carrier financial identities, prevent duplicate debits/credits across saves, and implement explicitly auditable insufficient-funds/unpaid-debt handling. No automatic charge, refund, write-off or replacement-payer substitution is authorized by a dry record. Improving exact fuel accounting can be incremental; the user has accepted the initial approximation.

## 11. Implementation and evidence map

Read this guide for current behavior; use the following for implementation details and evidence, not old normative instructions out of context:

- [Main design document](../WRSRE%20DESIGN%20DOC.md), section 12: project-level DO contract and deferred economics.
- [Permissions/dispatch history](AUTONOMOUS_DO_PERMISSIONS_PLAN.md): working-office tests and state-site policy.
- [Cumulative investigation report](VEHICLE_PANEL_FUEL_INVESTIGATION.md): complete source/results, Test11/Test12, save correction and PR #42 reconciliation. It remains the single evidence/handover report; this guide is an explanation, not a competing test report.
- [Role and direction policy](../../plugins/private_logistics/site_permissions.h): capabilities, checkbox interpretation, direction masks and thresholds.
- [Plugin implementation](../../plugins/private_logistics/private_logistics.cpp): `AutoTargetsOnDay`, `AutoPersistenceEvent`, `DryCostStart` initialization; [checked-in settings](../../plugins/private_logistics/private_logistics.ini).
- [Fleet reader](../../plugins/private_logistics/fleet_fuel_observer.h) and [runtime](../../plugins/private_logistics/fleet_fuel_runtime.h): discovery, per-vehicle comparisons, event/sample sinks, time/quote throttles and guards.
- [Assignment core](../../plugins/private_logistics/schedule_assignment_ledger.h): observed boundaries and sampled decreases, not native-job certification.
- [Cost core](../../plugins/private_logistics/dry_assignment_ledger.h), [codec](../../plugins/private_logistics/dry_assignment_codec.h), [identity adapter](../../plugins/private_logistics/dry_assignment_native.h) and [runtime/persistence adapter](../../plugins/private_logistics/dry_assignment_runtime.h): append-only records, validation and dry-only publication.
- [Production-header host scenarios](../../tests/private_logistics/dry_assignment_runtime_test.cpp) and [Python wrapper](../../tests/private_logistics/test_dry_assignment_ledger.py): implementation coverage with synthetic memory/services, not in-game proof.
