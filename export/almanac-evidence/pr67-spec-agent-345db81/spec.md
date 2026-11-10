# Almanac: implementation specification (decision draft, not code)

**Status:** specification only. D1–D6 are **proposals, not approved implementation decisions**. Neither proof of concept (§6) is authorized or read-only. No code, no game or mod execution, no hooks, no deployment, no live test. The existing tax rules are unchanged by this specification. Builds on [`almanac-design-comparison.md`](almanac-design-comparison.md) (committed `d0b3f05`).

**Labels**

- **[src]** read from repo source or config, with path and approximate line. Not runtime evidence.
- **[DLL]** from earlier static evidence on `reputation.dll` or exported native excerpts.
- **[assume]** not verified. Must be checked before it is relied on.
- **[open]** a decision or unknown that blocks implementation.

---

## 0. Proposed decisions (not approved)

Each row is a proposal for the operator to approve, change, or reject. None is an approved implementation decision.

| ID | Decision | Proposed default |
|---|---|---|
| D1 | Initial lead for the same-row control | **Path B** (separate same-row button). Path A (native row item) stays as the alternative, used only if B's ordering or geometry check fails. |
| D2 | Tax snapshot | Add a **new, append-only taxes interface version (v3)** with a read-only snapshot. Keep the v1 and v2 contracts unchanged (own `structSize` each). |
| D3 | Property-tax input bound | An **Almanac-side pre-check** only (finite, ≥0, ≤ the City Hall slider ceiling). The authoritative boundary, the tax-service setter, is unchanged in this proposal. Whether to add a finite check there is a separate decision. |
| D4 | Finance snapshot | Add a **new read-only provider** in `state_ledger`. Report currency mixing and completeness. Do not change how flows are accumulated in the first proof of concept. |
| D5 | Global panel host | **Open.** The UI panel API has no global host (§2.2). A host must be designed or the panel becomes a page section. |
| D6 | Economy & Trade fallback | Use the page only if the global section is unsafe. The same-row button must still exist (§2.5). |

---

## 1. Established facts and assumptions

### 1.1 Established (source or prior DLL evidence)

- **Service binding.** Providers publish in Init with `provide`. Consumers bind in Start with `consume`, which returns null when the plugin is absent [src `src/tesmio_api.h` ~202–233].
- **Version matching.** The registry matches versions exactly. Taxes publishes both v1 (prefix only) and v2 (full) [src `src/taxes_api.h` ~53–60; `plugins/taxes/taxes.cpp` ~1148–1154].
- **Setter ranges.** `setTaxRate` rejects NaN. VAT and profit accept `0..1`. Property accepts `rate >= 0`, so **+Infinity is accepted** [src `plugins/taxes/taxes.cpp` ~600–612, read by inspection, not run].
- **Setter writes.** Property writes the given value straight into `g_propertyTaxFlat`. There is no unit conversion at write time [src same].
- **Currency conversion.** Conversion from the RUB config value happens **only at Start**, and only when the active currency is USD. It divides by `usd_divisor`, or uses `property_tax_flat_usd` when set [src `plugins/taxes/taxes.cpp` ~1205–1222].
- **Land-value basis.** When `property_land_value = 1` (the shipped default), settled property tax is `base × (0.50 + 1.50 × attractiveness)` [src `taxes.cpp` ~498–508; `taxes.ini` line 85]. The taxes header still says "flat per-enterprise charge, on no base at all" [src `taxes.cpp` ~57].
- **Persistence.** Rates and flags are saved to `tesmio_taxes.dat`. The header is fixed, with `TAXES_VERSION 1` and magic `TSMTAX01`. Loading overwrites live values. A version mismatch is ignored silently [src `taxes.cpp` ~316–390].
- **Ledger role.** `state_ledger` provides no service and has no header. It consumes flows and writes HUD and CSV output [src `plugins/state_ledger/state_ledger.cpp` header, ~619].
- **Ledger flows ignore currency.** `OnFlow` receives the currency argument and discards it. All flows are summed into one `g_flowSum` [src `state_ledger.cpp` ~95–104].
- **Ledger treasury.** `ReadTreasury` reads the **active** currency at read time [src `state_ledger.cpp` ~108, header ~7–10].
- **Ledger baseline.** The month baseline is set on the first observed tick and reset on a backward time jump [src `state_ledger.cpp` ~125–160]. A month that starts mid-session is partial.
- **Ledger unexplained figure.** At month close, `unexplained = (closing − open) − flowDelta` [src `state_ledger.cpp` ~200–215; `ledger_reconcile.h` ~76–79]. Only the HUD subset of closed totals is kept in memory. The full record goes to CSV [src ~240–260].
- **UI providers are per building.** Every `ui_panels` provider is for **one building** [src `src/ui_panels_api.h` ~25–52, 117–125]. The widget button uses the engine's `GetMouseSolid` + `Collision` pair [src same ~180–188].
- **City Hall already has tax controls.** Taxes registers VAT, profit, and property sliders and enable checkboxes in the City Hall tax tile [src `taxes.cpp` header ~78, ~765, ~1241–1262].
- **Native trade row (prior DLL/native evidence).** On the examined path (`[game+0x10398] == 0`), the last entry is not drawn or clicked. The native selected index is at `exe+0x9E156C` [DLL/native, per earlier reports]. No native layout coordinates are established.

### 1.2 Assumptions (unverified)

- **A1.** The trade-row dispatch order allows a separate button's hit-test to run before the native handler. Unknown [assume].
- **A2.** Row geometry can be read at runtime for anchoring. Unknown [assume].
- **A3.** A new global panel host can draw and hit-test without disturbing native input. Unknown [assume].
- **A4.** A faulting currency observer is disabled for the session and stays disabled. The ledger cannot detect this today [src currency header ~87; assume it is undetected].
- **A5.** The active currency changes only at load or by an explicit action. Not verified [assume].
- **A6.** The taxes setters are only called by City Hall and, later, the Almanac. Not verified [assume; check S13 in the design comparison].

---

## 2. Path B UI specification (lead)

### 2.1 Same-row button

- **Placement.** Anchored to the trade row's geometry at runtime (A2). The rule: place after the last native tile if there is space. If there is no space, **do not overlap a native tile**. Report "no space" and treat the path as blocked (D1 fallback to Path A, or D6).
- **Rendering.** Use the engine `GetMouseSolid` + `Collision` pair, as `ui_panels` documents for its widgets [src `ui_panels_api.h` ~180–188]. The rectangle is fixed in size and label.
- **States.** `enabled` (service ok), `unavailable` (service missing or snapshot not readable, §5), and `hidden` (native row not present). The button must never be drawn over a native tile in any state.

### 2.2 Global panel

- **Host.** Not available in the current UI panel API (D5). Options: (a) a new mod-owned draw and hit-test host; (b) the Economy & Trade page section (§2.5). Choice is open.
- **Sections.** Fiscal policy (VAT, profit, property: values, enabled state, unit, basis) and Finance (period, completeness, recorded flows, treasury change, unexplained difference, §4).
- **Authority.** The panel reads only the snapshots (§3, §4). It keeps no tax value of its own that could override the service. Any display copy is stale-marked and never authoritative.

### 2.3 Input ownership

- **Button.** Consumes a press only if the press is inside the button's rectangle. Presses outside it must reach the native handler unchanged.
- **Panel.** Consumes presses inside the panel rectangle only. Outside presses are passed on, and the close rule in §2.4 applies.
- **Ordering.** Not established (A1). Static check S5 and live check L4 must establish whether the button runs before or after the native handler, and whether the native click latch is consumed. The spec does not assume either.
- **Esc.** Handled by the panel only while it is open. Its ordering against native Esc handling is unverified and must be checked before it is relied on.

### 2.4 Close and return

**Open.** A press inside the button rectangle while closed. If the service is unavailable, open in the unavailable state (§5), not closed.

**Close triggers.** Each must be handled separately, because a changed native index is not a complete detector (design comparison §0):

1. The panel's own close control.
2. The same button pressed again.
3. Esc, if ordering allows it (§2.3).
4. A **press** anywhere in the native row rectangle, including a repeat press on the same tile. The trigger is the press edge, not an index change.
5. A native section change, detected by the section identity, not by the index.
6. Native window close, or a world or save load.
7. Service state changing to absent or unreadable.

**Return.** The Almanac never writes the native selected index or native content. On return, native state should be untouched. A PoC that shows the Almanac changes native state is a **stop condition** (§6.5). Restoration is not defined for that case.

**State machine.** `CLOSED → OPEN(generation g)`. On each snapshot generation change while open, refresh the view. Close clears only panel state.

### 2.5 Economy & Trade page fallback

- **When.** Only if the global section is unsafe (D6, operator-approved). The fallback is already acceptable under that condition.
- **Requirement.** The same-row button still exists and opens the page. The page's own section shows the same snapshot content.
- **Close.** Ordinary page navigation. No native tile is changed.
- **Limit.** The fallback does not remove the same-row requirement, and it does not fix the native alternate-handler omission (design comparison §0).

### 2.6 Alternative: native row item (Path A)

Retained as the alternative. It needs: nine entries in the native vector, the examined-path omission handled, a capacity check before the append (none exists [DLL]), and a close rule that uses the same triggers as §2.4. Use only if B fails its ordering or geometry checks.

---

## 3. Tax policy snapshot (taxes, new v3 view)

### 3.1 Contract

- **Version.** New taxes interface **v3** = v2 with one appended function, `getPolicySnapshot`. Publish v1 (prefix), v2 (full v2 struct, unchanged), and v3 (full, plus the new function), following the existing dual-publish pattern [src `taxes.cpp` ~1148–1154]. **Each view must be published as a copy with its own `structSize`**, as the v1 view already is (`kApiV1`). Otherwise a v2 consumer would see the v3 size and read the appended function. Existing consumers keep requesting v1 or v2 and see no change.
- **Access.** Read-only. Takes the existing `g_lock`, copies values, and returns. No settlement or accrual code changes.
- **Struct.** Own `structSize` and an append-only layout. Fields:
  - `generation` (uint32), incremented on every change to any rate, flag, or stored basis, including changes from `LoadTaxes`. Used to detect torn reads and reloads.
  - `available` (int), 1 when the service is running.
  - For each tax (VAT, profit, property): `enabled` (0/1), `configuredValue` (VAT and profit: fraction `0..1`; property: the stored flat value), and `frequency` (the schedule, as in the config).
  - `propertyStoredCurrency` (`TSM_CURRENCY_RUB` or `TSM_CURRENCY_USD`): the unit the stored property value is in. Record it at Start, where the conversion already happens (§1.1). This is additive.
  - `propertyLandValueScaled` (0/1), with the scale constants `scaleFloor = 0.50` and `scaleSlope = 1.50` [src `taxes.cpp` ~504–508]. Expose them, so the panel does not hard-code them.
  - `propertySliderMax` (the City Hall ceiling, from `property_slider_max`) for input mapping [src `taxes.ini` ~120].
- **Enabled-state semantics.** `configuredValue` is returned **regardless of enabled state**. The existing `getVatRate` and `getProfitTaxRate` still return 0 when disabled. This is the gap the snapshot closes [src `taxes_api.h` ~74–80, ~120–122].

### 3.2 Persistence

- **Facts (source).** Taxes persists to its own file, `tesmio_taxes.dat`, not into the game save. On load, a magic or version mismatch returns before any live value is assigned. Live values then stay at whatever Start set from `taxes.ini` [src `taxes.cpp` ~356–372].
- **Scope.** The source does not show that an incompatible tax header discards the game save, and this specification does not claim it does. Whether the game's own save loader reacts to a missing or unreadable side file is **not established**.
- **Related fact.** The taxes API header says a save written with taxes enabled and loaded with taxes disabled keeps its accrual fields, and nothing settles them [src `taxes_api.h`, "WHEN THIS PLUGIN IS ABSENT"]. Accrual fields belong to `enterprise_base`, not to the tax header.
- **Rules for this proposal.** (a) Do not change `TaxSaveHeader` or `TAXES_VERSION`. (b) The v3 snapshot is in memory only. (c) Any future header change needs its own compatibility decision, covering what an old file does (today: live values stay at config defaults for that session) and what happens to accrued liabilities.
- **Test, not assumed.** The legacy-file behavior is a consequence to test in the disposable copy (§6.2), not a fact to rely on.
- The snapshot is in-memory only. `propertyStoredCurrency` is re-derived at Start on each load.

### 3.3 Writes

- The Almanac writes only through the existing v2 setters `setTaxRate` and `setTaxEnabled`. No new setter is added in the first proof of concept.
- After every write, re-read the snapshot and display the returned value. Show `refused` when the setter returns 0. Never assume success.
- City Hall may write the same values. Both paths go through the same setters. Last writer wins. The panel re-reads on `generation` change, so it does not show a stale value.

### 3.4 Unit rules

- The Almanac shows the unit from `propertyStoredCurrency`, not the active currency. If the two differ, show both and mark the value **unit-mismatched**. Do not convert. No conversion rule exists for a running session [src §1.1].
- The unit of a value written through `setTaxRate` is the unit of the active currency at write time, which is **not established** (S10). Until S10 is answered, a value written in the Almanac is labelled with the active currency and marked "unit unverified".

### 3.5 Consumers

- City Hall stays on v2. Its setters and slider mapping are unchanged. The proof of concept must show City Hall still writes and reads (L9).
- A consumer built against the v2 header is unaffected by v3. The v2 view must keep the same fields, the same `structSize` (v2), and the same function behavior. This is checked statically (S11).

---

## 4. Finance and ledger snapshot (state_ledger, new read-only provider)

### 4.1 Contract

- **Provider.** `state_ledger` publishes a new read-only service view from Init. This changes its role from "consumer only" to "consumer and provider". Decision D4.
- **Access.** The snapshot is **published on the sim tick** (`OnTick`) into a buffer, under lock. The render thread reads the buffer. It does not read live accumulators, so the flow totals, treasury reads, and period fields come from one tick (§4.3).
- **Periods.** Two records: `current` (in progress) and `lastClosed`.

### 4.2 Fields

For `current` and `lastClosed`:

| Field | Meaning | Source today |
|---|---|---|
| `year`, `month` | The period | `g_lastYear`, `g_lastMonth` |
| `state` | `IN_PROGRESS` or `CLOSED` | new |
| `currency` | Unit of every amount in the record | **Not tracked today.** The open treasury uses the active currency at open, but that currency is not stored. Report `unknown` until tracked. |
| `currencyMixed` | 1 if any flow arrived in a currency other than `currency` | **Not tracked today.** `OnFlow` discards the currency argument. Report `unknown` until tracked. |
| `coveredFromStart` | 1 only if the baseline was set at a real month change while the ledger was running; 0 if set on first tick or by a rewind | **Not tracked today.** The baseline branches set no flag. Report `unknown` until tracked. |
| `taxVat`, `taxProfit`, `taxProperty` | Recorded flows, labels `vat`, `profit_tax`, `property_tax` | `g_monthFlows` (existing) |
| `wagesAndTransfers` | `wage`, `pension`, `welfare` | `g_monthFlows` (existing) |
| `stateStoreSales`, `wholesale` | `state_store_sale`, `wholesale_b2b` | `g_monthFlows` (existing) |
| `otherFlows` | Sum of all other categories, and `categoryCount` | `g_monthFlows` (existing) |
| `recordedNet` | Sum of all recorded flows | `g_flowSum` (existing; mixed-currency caveat) |
| `treasuryOpen`, `treasuryNow` | Treasury balance in `currency` | `g_treasuryAtMonthStart`; `ReadTreasury` |
| `treasuryChange` | `treasuryNow − treasuryOpen` | derived |
| `unexplained` | `treasuryChange − recordedNet` (same formula as close) | `TreasuryUnexplained` (existing) |
| `unexplainedValid` | 1 only if `currencyMixed = 0`, `coveredFromStart = 1`, and `currency` equals the active currency | `unknown` until the three inputs are tracked |
| `tick` | Sim tick at which the record was published | new |

### 4.3 Consistency and currency rules

- **Unknown is the default.** A field marked "not tracked today" is reported as `unknown`. It is never reported as `complete`, `single-currency`, or zero. Provenance and completeness become known only after the proposed tracking is implemented and tested.

- **Mixed currency.** `OnFlow` currently discards the currency argument and sums all flows. `treasuryNow` reads the active currency. A mid-period currency change would compare different units. Once tracked, the snapshot must set `currencyMixed` and mark `unexplainedValid = 0`. Until then, both stay `unknown`. It must **not** report a zero difference (S12).
- **Partial periods.** A period that starts mid-session, or is re-baselined by a rewind, has `coveredFromStart = 0`. Its totals are shown as partial, not as complete.
- **Closed period.** Today only the HUD subset of a closed period is kept in memory. The full closed record is written only to CSV [src ~240–260]. The proof of concept adds an in-memory copy of the full record at close (additive, no CSV change).
- **Observer loss.** A faulting currency observer is disabled for the session [src currency header ~87]. The ledger cannot see this today (A4). The snapshot cannot prove completeness against it. It must state that limit rather than claim completeness.
- **Read consistency.** Treasury is read on the sim tick, the same tick as the flow totals. A torn value is not expected, but a `tick` field is included to make that checkable.

### 4.4 Authority

- The ledger is **not** the authority for tax policy. It is the authority only for recorded flows and the treasury comparison.
- The unexplained figure is a reconciliation indicator. It is not a tax computation and must not be used to change any tax rule.

---

## 5. Unavailable states and validation

### 5.1 Unavailable states

| Condition | Display | Controls | Basis |
|---|---|---|---|
| Taxes not installed (`consume` returns null) | "No tax service: no tax is charged" | Disabled | Taxes header degraded mode [src `taxes_api.h`, "WHEN THIS PLUGIN IS ABSENT"] |
| Taxes present, v3 absent (older build) | "Policy snapshot unavailable" | Disabled | v3 is new; the v2 view cannot show suspended values |
| Snapshot read torn (`generation` changed between two reads) | Retry once, then "unavailable" for that frame | Disabled for that frame | §3.1 |
| `state_ledger` absent | Finance section "unavailable"; tax section still shown | Tax controls enabled | Ledger provides nothing today [src] |
| `currencyMixed = 1`, `coveredFromStart = 0`, or either is `unknown` | Values marked "partial" or "unverified"; unexplained "not reconciled" | n/a | §4.3 |
| Stored unit differs from active currency | Value with its own unit label; no conversion | Enabled, unit marked | §3.4; S10 |
| Setter returns 0 | "Refused: value unchanged" | Enabled | Setter contract [src `taxes_api.h`] |
| Save loaded or generation changed | Refresh from the new snapshot | Enabled | `LoadTaxes` overwrites live values |

**Never** show zero for an unavailable or unreconciled figure. Show "unavailable" or "not reconciled".

### 5.2 Input validation

**Two layers, not one.**

1. **Almanac-side pre-checks** run before the Almanac calls any setter. They protect only the Almanac's own writes. They are **not authoritative**.
2. **The authoritative boundary is the tax-service setter.** It currently rejects NaN but accepts **+Infinity** for property (§1.1). City Hall and any other writer bypass the Almanac's checks. Making the boundary reject non-finite values requires a change to the tax service, which is a separate decision (D3). It is not part of this proposal.

The rules below are Almanac-side pre-checks.

- **VAT and profit:** finite, `0 ≤ x ≤ 1`. Reject NaN, ±Infinity, and out-of-range values. No setter call on rejection.
- **Property (D3):** finite, `≥ 0`, and `≤ propertySliderMax` from the snapshot. Reject NaN, **+Infinity** (accepted by the current setter [src §1.1]), and negative values before calling the setter. The ceiling matches the City Hall slider, which maps `0..100` onto `0..property_slider_max` [src `taxes.cpp` header ~78–90].
- **Derived property display:** compute `base × (0.50 + 1.50 × a)` only with a finite base. Show the range `0.50–2.00 × base` (not per-building values). The `a` range (0..1, neutral 0.5) is taken from the taxes comments and must be confirmed (S9).
- **Ledger amounts:** a non-finite value makes that field "unavailable". It does not become zero.
- **Struct reads:** check `structSize` covers each field before reading it. On failure, use a zero-initialized copy and mark the view unavailable.
- **Rounding:** for display only. Stored and written values are not rounded.

### 5.3 Labels that must match the code

- The property amount is shown as **"base (land-value scaled)"** while `propertyLandValueScaled = 1`. The label "flat per enterprise" is used only when the flag is 0. This resolves the wording conflict in §1.1 without changing the tax rule.

---

## 6. Proof of concept (proposal only, not approved)

### 6.1 Smallest proof of concept

**Status of both PoCs.** Neither is authorized. Neither is read-only. Each requires code changes in a test copy and probe logging. The names below describe the purpose, not a read-only guarantee.

**PoC-1, data-focused (first).** Build the v3 taxes snapshot and the `state_ledger` snapshot. Add a probe that logs both at Start and once per in-game month. It also calls `setTaxRate` and injects a deliberate test flow, so it performs writes in the test copy. No button, no panel, no click hooks.

**PoC-2, geometry and ordering (only if PoC-1 passes).** Draw the Path B button rectangle, and log its rectangle against the native tile rectangles. Log press events in the row rectangle without consuming them. This establishes A1 and A2 and tests for overlap. It draws UI, so it is not read-only. No panel.

Neither PoC is intended to change tax rules, settlement, or native selection. Any test writes happen only in the disposable test copy (§6.3). That intent is not a guarantee.

### 6.2 Success criteria

**PoC-1**

- Snapshot values equal the expected configured values from `taxes.ini` (or `tesmio_taxes.dat` if a save was loaded), for an enabled tax and for a suspended tax.
- Property basis flag and `propertyStoredCurrency` match the config and the active currency at Start, in both RUB and USD modes (test each).
- `generation` changes after a test `setTaxRate` and after a save load, and not otherwise.
- A mid-month start gives `coveredFromStart = 0`.
- A deliberate test flow in the non-active currency sets `currencyMixed = 1` and `unexplainedValid = 0`.
- Non-finite and out-of-range property inputs are rejected before the setter.
- City Hall still reads and writes its controls (L9).

**PoC-2**

- The button rectangle does not overlap any native tile at the measured geometry.
- Press-event logs establish whether the native handler sees a press inside the button rectangle (ordering, A1). The result is recorded either way.

### 6.3 Environment

- A **disposable copy** of the game install and a **disposable copy** of a save, on a separate Windows machine or VM. Not the user's working install, and not the repo.
- The mod is loaded only in that copy.
- Do **not** run `install.ps1`, `uninstall.ps1`, or the migration scripts. The installer overwrites `dinput8.dll`, and migration can rewrite save-related files [repo doc: `docs/almanac-frontline-triage.md`].
- No test artifacts are written to the repo except logs and hashes under `export/`, with no save data and no game binaries.

### 6.4 Backups (before any test)

- SHA-256 manifest and full copies, stored outside the repo, of: the game executable and `dinput8.dll`; every plugin DLL; the loader configuration (path under `tesmioloader`, to be confirmed); `taxes.ini`, `state_ledger.ini`, `reputation.ini`, `ui_panels.ini`; `tesmio_taxes.dat`; `tesmio_ledger.csv`; and the full save folder.
- The manifest is recorded in the repo as hashes only.

### 6.5 Stop conditions (stop the test immediately if any occurs)

1. Any native tile is missing, duplicated, moved, or not clickable.
2. The Almanac changes the native selected index or native content.
3. `tesmio_taxes.dat` or `taxes.ini` changes without an explicit test action.
4. A crash, save-load failure, or City Hall regression.
5. A money movement not in the flow log, or a treasury change the probe cannot explain.
6. The ledger reports a zero unexplained figure while `currencyMixed = 1` or `coveredFromStart = 0`.
7. Any non-finite value reaches a setter.
8. Any repo change outside `docs/` and `export/`.

### 6.6 Restoration checks

- **R1.** Restore the test copy from the backups. Compare the SHA-256 of every backed-up file with the manifest. All must match.
- **R2.** Remove test-only DLLs and probe files. Confirm the plugin list matches the manifest.
- **R3.** Run the test checklist on the restored copy: native tiles visible and clickable, City Hall tax controls working, no Almanac button.
- **R4.** In the repo, `git status` shows only approved files.
- **R5.** Write a restoration log under `export/`, with hashes and results and no save data.

### 6.7 Live tests

No live test is approved. A PoC runs only after explicit approval of the environment, backups, stop conditions, and restoration checks above.

---

## 7. Risks

1. **Trade-row ordering (A1)** may block Path B. Path A then needs a capacity check and omission handling.
2. **Row geometry (A2)** may not be readable at runtime. Then the button cannot be placed dynamically, which the plan requires.
3. **No global UI host (D5).** A new host is engine-level work. Until it exists, the page fallback is the only option.
4. **Taxes v3 mis-publication** could break v2 consumers if the v2 prefix changes. Mitigation: the v2 view keeps its own unchanged `structSize`, plus a static check (S11).
5. **Ledger role change.** Becoming a provider adds a dependency surface. Mitigation: additive, read-only, versioned.
6. **Property `+Infinity` accepted** by the existing setter (§1.1). Not blocked at the authoritative boundary. Only Almanac-side pre-checks apply, and they do not stop City Hall or other writers. D3 decides whether the boundary changes.
7. **Writers race** (City Hall and Almanac). Last writer wins. Mitigated by re-reading the snapshot.
8. **Label confusion.** "Flat" vs "land-value scaled". Mitigated by §5.3.
9. **Mixed currency** breaks the unexplained figure until the flow accumulation is fixed. The proposal flags it, but does not fix it.
10. **Observer loss (A4)** can make the ledger silently incomplete. The snapshot states this limit.

---

## 8. Decisions needed before any implementation

1. Confirm D1: Path B as lead, Path A as alternative.
2. Confirm D2: a new taxes v3 snapshot view, with v1 and v2 unchanged.
3. Confirm D3: Almanac-side property validation, with a `+Infinity` fix to the setter as a separate decision.
4. Confirm D4: a read-only `state_ledger` provider, with flow accumulation unchanged for the PoC.
5. Choose D5: a new global UI host, or the Economy & Trade page section.
6. Confirm the property-tax basis label (§5.3).
7. Approve or decline PoC-1 and PoC-2, with the environment, backups, stop conditions, and restoration checks in §6.

---

## 9. Limits

- Nothing here is run. All code-level facts were read from source and not executed.
- No change to the existing tax rules, settlement, persistence format, or City Hall behavior is proposed.
- The native executable and its layout are not in the workspace. Native facts are prior evidence only.
- This specification does not approve any implementation, hook, game run, deployment, or live test.
