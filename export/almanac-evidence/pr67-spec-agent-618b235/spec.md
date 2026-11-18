# Almanac: implementation specification (decision draft, not code)

**Status:** specification only. D1–D6 are **proposals, not approved implementation decisions**. Neither proof of concept (§6) is authorized or read-only. No code, no game or mod execution, no hooks, no deployment, no live test. The existing tax rules are unchanged by this specification. Builds on [`almanac-design-comparison.md`](../../../docs/almanac-design-comparison.md) (committed `d0b3f05`).

**Labels**

- **[src]** read from repo source or config, with path and approximate line. Not runtime evidence.
- **[DLL]** from earlier static evidence on `reputation.dll` or exported native excerpts.
- **[assume]** not verified. Must be checked before it is relied on.
- **[open]** a decision or unknown that blocks implementation.

---

## 0. Proposed decisions (not approved)

Each row is a proposal for the operator to approve, change, or reject. None is an approved implementation decision.

**Proposed first-release scope (not approved):** fiscal information (§4) and adjustable VAT, profit, and property tax (§3.3, §5.2), shown in the Almanac panel. Both same-row approaches (§2, Path B lead and Path A alternative) remain under evaluation. The Economy & Trade page fallback remains acceptable under D6.

| ID | Decision | Proposed default |
|---|---|---|
| D1 | Initial lead for the same-row control | **Path B** (separate same-row button). Path A (native row item) stays as the alternative, used only if B's ordering or geometry check fails. |
| D2 | Tax snapshot | Add a **new, append-only taxes interface version (v3)** with a read-only snapshot. Keep the v1 and v2 contracts unchanged (own `structSize` each). |
| D3 | Property-tax input validation | An **Almanac-side pre-check** only: finite and ≥0. No upper bound is imposed. The City Hall slider ceiling is a UI mapping, not a tax-policy limit. The authoritative boundary, the tax-service setter, is unchanged in this proposal. Any finite check or cap there is a separate decision. |
| D4 | Finance snapshot | Add a **new read-only provider** in `state_ledger`. Report currency mixing and completeness only after the tracking in §4.2 exists. Until then they are `unknown`. Do not change how flows are accumulated in the first proof of concept. |
| D5 | Global panel host | **Open, unproven candidate.** The UI panel API has no global host (§2.2). A new host or the page section (D6) are the candidates. Neither is validated. |
| D6 | Economy & Trade fallback | Use the page only if the global section is unsafe. The same-row button must still exist (§2.5). |

---

## 1. Established facts and assumptions

### 1.1 Established (source or prior DLL evidence)

- **Service binding.** Providers publish in Init with `provide`. Consumers bind in Start with `consume`, which returns null when the plugin is absent [src `src/tesmio_api.h` ~202–233].
- **Version matching.** The registry matches versions exactly. Taxes publishes both v1 (prefix only) and v2 (full) [src `src/taxes_api.h` ~53–60; `plugins/taxes/taxes.cpp` ~1148–1154].
- **Setter ranges.** `setTaxRate` rejects NaN. VAT and profit accept `0..1`. Property accepts `rate >= 0`, so **+Infinity is accepted** [src `plugins/taxes/taxes.cpp` ~600–612, read by inspection, not run].
- **Setter writes.** Property writes the given value straight into `g_propertyTaxFlat`. There is no unit conversion at write time [src same].
- **Currency conversion.** Conversion from the RUB config value happens **only at Start**, and only when the active currency is USD. It divides by `usd_divisor`, or uses `property_tax_flat_usd` when set [src `plugins/taxes/taxes.cpp` ~1205–1222]. This establishes the unit of the config-derived value at that point only. It does not establish the unit of a loaded value or of a value written later.
- **Land-value basis.** When `property_land_value = 1` (the shipped default), settled property tax is `base × (0.50 + 1.50 × attractiveness)` [src `taxes.cpp` ~498–508; `taxes.ini` line 85]. The taxes header still says "flat per-enterprise charge, on no base at all" [src `taxes.cpp` ~57].
- **Persistence.** Rates and flags are saved to `tesmio_taxes.dat`. The header is fixed, with `TAXES_VERSION 1` and magic `TSMTAX01`. Loading overwrites live values. The header records no currency or unit field. A version mismatch is ignored silently [src `taxes.cpp` ~316–390].
- **Ledger role.** `state_ledger` provides no service and has no header. It consumes flows and writes HUD and CSV output [src `plugins/state_ledger/state_ledger.cpp` header, ~619].
- **Ledger flows ignore currency.** `OnFlow` receives the currency argument and discards it. All flows are summed into one `g_flowSum` [src `state_ledger.cpp` ~95–104].
- **Observer threading.** Currency observers run on the **caller's** thread, after the balance change is committed and the currency lock is released [src `currency_api.h` ~84–87].
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

- **Host (unproven candidates).** The current UI panel API has no global host (D5). Candidates: (a) a new mod-owned draw and hit-test host, unproven; (b) the Economy & Trade page section (§2.5), accepted if (a) is unsafe. Neither is validated.
- **Sections.** Fiscal policy (VAT, profit, property: values, enabled state, unit, basis) and Finance (period, completeness, recorded flows, treasury change, unexplained difference, §4).
- **Authority.** The panel reads only the snapshots (§3, §4). It keeps no tax value of its own that could override the service. Any display copy is stale-marked and never authoritative.

### 2.3 Input ownership

- **Button.** Consumes a press only if the press is inside the button's rectangle. Presses outside it must reach the native handler unchanged.
- **Panel.** Consumes presses inside the panel rectangle only. Outside presses are passed on, and the close rule in §2.4 applies.
- **Ordering (unproven design candidate).** Not established (A1). Static check S5 and live check L4 must establish whether the button runs before or after the native handler, and whether the native click latch is consumed. The spec does not assume either.
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

### 2.7 Feature rollback (unproven candidate)

No feature rollback mechanism exists in this design. Candidates, all unproven: (a) an Almanac-specific configuration switch that stops button creation at startup, with a restart; (b) for Path A, removal of the appended native record, which has no runtime removal path [DLL]. For Path B, no native state is written, so none should need restoring; this must be shown by the tests in §6. Test restoration (§6.6) is separate from feature rollback.

---

## 3. Tax policy snapshot (taxes, new v3 view)

### 3.1 Contract

- **Version.** New taxes interface **v3** = v2 with one appended function, `getPolicySnapshot`. Publish v1 (prefix), v2 (full v2 struct, unchanged), and v3 (full, plus the new function), following the existing dual-publish pattern [src `taxes.cpp` ~1148–1154]. **Each view must be published as a copy with its own `structSize`**, as the v1 view already is (`kApiV1`). Otherwise a v2 consumer would see the v3 size and read the appended function. Existing consumers keep requesting v1 or v2 and see no change.
- **Access.** Read-only. Takes the existing `g_lock`, copies values, and returns. No settlement or accrual code changes.
- **Struct.** Own `structSize` and an append-only layout. Fields:
  - `generation` (uint32), incremented on every change to any rate, flag, or stored basis, including changes from `LoadTaxes`. Used to detect torn reads and reloads.
  - `available` (int), 1 when the service is running.
  - For each tax (VAT, profit, property): `enabled` (0/1), `configuredValue` (VAT and profit: fraction `0..1`; property: the stored flat value), and `frequency` (the schedule, as in the config).
  - `propertyStoredCurrency`: `RUB`, `USD`, or `unknown`. A value is `unknown` unless its unit is tracked at every assignment point (Start, save load, setter). Startup currency alone is never used to infer it (§3.2, §3.4). Tracking is additive and does not exist today.
  - `propertyLandValueScaled` (0/1), with the scale constants `scaleFloor = 0.50` and `scaleSlope = 1.50` [src `taxes.cpp` ~504–508]. Expose them, so the panel does not hard-code them.
  - No upper bound is exposed as a policy value. The City Hall slider ceiling (`property_slider_max`, [src `taxes.ini` ~120]) is a UI mapping only.
- **Enabled-state semantics.** `configuredValue` is returned **regardless of enabled state**. The existing `getVatRate` and `getProfitTaxRate` still return 0 when disabled. This is the gap the snapshot closes [src `taxes_api.h` ~74–80, ~120–122].

### 3.2 Persistence

- **Facts (source).** Taxes persists to its own file, `tesmio_taxes.dat`, not into the game save. On load, a magic or version mismatch returns before any live value is assigned. Live values then stay at whatever Start set from `taxes.ini` [src `taxes.cpp` ~356–372].
- **Scope.** The source does not show that an incompatible tax header discards the game save, and this specification does not claim it does. Whether the game's own save loader reacts to a missing or unreadable side file is **not established**.
- **Related fact.** The taxes API header says a save written with taxes enabled and loaded with taxes disabled keeps its accrual fields, and nothing settles them [src `taxes_api.h`, "WHEN THIS PLUGIN IS ABSENT"]. Accrual fields belong to `enterprise_base`, not to the tax header.
- **Rules for this proposal.** (a) Do not change `TaxSaveHeader` or `TAXES_VERSION`. (b) The v3 snapshot is in memory only. (c) Any future header change needs its own compatibility decision, covering what an old file does (today: live values stay at config defaults for that session) and what happens to accrued liabilities.
- **Test, not assumed.** The legacy-file behavior is a consequence to test in the disposable copy (§6.2), not a fact to rely on.
- The snapshot is in-memory only. `propertyStoredCurrency` is **not** re-derived from startup currency. After a save load it is `unknown`, because the file records no unit and `LoadTaxes` overwrites the live value (§1.1).

### 3.3 Writes

- The Almanac writes only through the existing v2 setters `setTaxRate` and `setTaxEnabled`. No new setter is added in the first proof of concept.
- After every write, re-read the snapshot and display the returned value. Show `refused` when the setter returns 0. Never assume success.
- City Hall may write the same values. Both paths go through the same setters. Last writer wins. The panel re-reads on `generation` change, so it does not show a stale value.

### 3.4 Unit rules

- The Almanac shows the stored unit only when `propertyStoredCurrency` is known. When it is `unknown`, the value is shown as "unit unknown". The active currency is never substituted for the stored unit. Do not convert: no conversion rule exists for a running session [src §1.1].
- The unit of a value written through `setTaxRate` is **not established** (S10). A value written by the Almanac or by City Hall is `unknown` until S10 is answered and the assignment is tracked.

### 3.5 Consumers

- City Hall stays on v2. Its setters and slider mapping are unchanged. The proof of concept must show City Hall still writes and reads (L9).
- A consumer built against the v2 header is unaffected by v3. The v2 view must keep the same fields, the same `structSize` (v2), and the same function behavior. This is checked statically (S11).

---

## 4. Finance and ledger snapshot (state_ledger, new read-only provider)

### 4.1 Contract

- **Provider.** `state_ledger` publishes a new read-only service view from Init. This changes its role from "consumer only" to "consumer and provider". Decision D4.
- **Access.** The snapshot is **published on the sim tick** (`OnTick`) into a buffer, under lock. The render thread reads that buffer. This is intended to reduce sampling skew. It does **not** guarantee a single instant; see the consistency limits in §4.3.
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

- **Mixed currency: not detectable today.** `OnFlow` discards the currency argument and sums all flows (§1.1). Once the tracking in §4.2 exists, `currencyMixed` can be set and `unexplainedValid` set to 0 when it is 1. Until then the snapshot **cannot detect mixed currency**. It reports `unknown` and never a zero difference (S12).
- **Completeness: not detectable today.** The baseline branches set no flag (§4.2), so the snapshot cannot tell a complete period from one that started mid-session or was re-baselined by a rewind. Once tracked, those cases would set `coveredFromStart = 0` and be shown as partial. Until then, completeness is `unknown`.
- **Closed period.** Today only the HUD subset of a closed period is kept in memory. The full closed record is written only to CSV [src ~240–260]. The proof of concept adds an in-memory copy of the full record at close (additive, no CSV change).
- **Observer loss.** A faulting currency observer is disabled for the session [src currency header ~87]. The ledger cannot see this today (A4). The snapshot cannot prove completeness against it. It must state that limit rather than claim completeness.
- **Consistency limits.**
  1. Observers run on the caller's thread (§1.1), so a flow can arrive on another thread between the ledger's reads.
  2. `g_lock` protects `g_monthFlows`, but the treasury read is not under that lock (§1.1). Flow totals and the treasury value can therefore be sampled at different instants.
  3. The `tick` field records publication time only. It does not prove a single instant.
  4. A faulting observer is disabled for the session, and the ledger cannot see it (A4).
  5. A save load or rewind resets the baseline.
  Any of these can make `unexplained` non-zero or misleading. The unexplained figure is shown only as a reconciliation indicator with these limits, never as proof of completeness.

### 4.4 Authority

- The ledger is **not** the authority for tax policy. It is the authority only for recorded flows and the treasury comparison.
- The unexplained figure is a reconciliation indicator. It is not a tax computation and must not be used to change any tax rule.

---

## 5. Unavailable states and validation

### 5.0 Unavailable-service policy (single rule)

Each service is in exactly one resolution state:

- `resolved`: provider found, required version present, reads valid.
- `unresolved`: `consume` returns null.
- `version-missing`: provider present, but without the required version (for example, no v3 snapshot).
- `torn`: the generation changed between two reads.
- `stale`: no valid read after an expected change.

Rules:

1. Every state other than `resolved` shows an explicit label, "unavailable: <state>", with the reason.
2. Writing controls are enabled only in `resolved`.
3. **No tax-status claim is made from resolution.** A provider the Almanac cannot resolve means the Almanac cannot read the service. It does not mean the game charges or does not charge tax. The text "no tax is charged" is not shown for any resolution state.
4. The taxes header says enterprise_base treats an absent taxes service as "no tax" [src `taxes_api.h`, "WHEN THIS PLUGIN IS ABSENT"]. That describes enterprise_base's behavior. The Almanac does not restate it as a fact about the current game state.
5. Finance follows the same rule independently. A ledger that is `unresolved` shows finance as unavailable. Tax controls depend only on the taxes state.

### 5.1 Unavailable states

| Condition | Display | Controls | Basis |
|---|---|---|---|
| Taxes `unresolved` (`consume` returns null) | "Unavailable: taxes service not resolved. Policy status unknown." | Disabled | §5.0 |
| Taxes `version-missing` (no v3 snapshot, older build) | "Unavailable: policy snapshot not provided by this build." | Disabled | §5.0; the v2 view cannot show suspended values |
| Snapshot read torn (`generation` changed between two reads) | Retry once, then "unavailable" for that frame | Disabled for that frame | §3.1 |
| `state_ledger` `unresolved` | Finance section "unavailable: ledger not resolved"; tax section still shown | Tax controls depend only on the taxes state | §5.0; the ledger provides no service today [src] |
| `currencyMixed = 1`, `coveredFromStart = 0`, or either is `unknown` | Values marked "partial" or "unverified"; unexplained "not reconciled" | n/a | §4.3 |
| Stored property unit `unknown`, or differs from the active currency | Value shown as "unit unknown", or with its stated unit; never converted | Enabled | §3.4; S10 |
| Setter returns 0 | "Refused: value unchanged" | Enabled | Setter contract [src `taxes_api.h`] |
| Save loaded or generation changed | Refresh from the new snapshot | Enabled | `LoadTaxes` overwrites live values |

**Never** show zero for an unavailable or unreconciled figure. Show "unavailable" or "not reconciled".

### 5.2 Input validation

**Two layers, not one.**

1. **Almanac-side pre-checks** run before the Almanac calls any setter. They protect only the Almanac's own writes. They are **not authoritative**.
2. **The authoritative boundary is the tax-service setter.** It currently rejects NaN but accepts **+Infinity** for property (§1.1). City Hall and any other writer bypass the Almanac's checks. Making the boundary reject non-finite values requires a change to the tax service, which is a separate decision (D3). It is not part of this proposal.

The rules below are Almanac-side pre-checks.

- **VAT and profit:** finite, `0 ≤ x ≤ 1`. Reject NaN, ±Infinity, and out-of-range values. No setter call on rejection.
- **Property (D3):** finite and `≥ 0`. Reject NaN, **+Infinity** (accepted by the current setter [src §1.1]), and negative values before calling the setter. **No upper bound is imposed** by this proposal. The City Hall slider ceiling (`0..100` mapped onto `0..property_slider_max`, [src `taxes.cpp` header ~78–90]) is a UI mapping, not a validated tax-policy limit. Any cap is a separate, justified decision.
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
- The property basis flag matches the config. `propertyStoredCurrency` is reported as `unknown` after a save load and after a test setter call. It is reported as known only where the probe tracks the assignment. Test in both RUB and USD modes.
- `generation` changes after a test `setTaxRate` and after a save load, and not otherwise.
- **Only if the tracking in §4.2 is implemented:** a mid-month start gives `coveredFromStart = 0`, and a deliberate test flow in the non-active currency sets `currencyMixed = 1` and `unexplainedValid = 0`. Without that tracking, the expected results are `unknown`, not 0 and not "complete".
- Non-finite and out-of-range property inputs are rejected before the setter.
- City Hall still reads and writes its controls (L9).

**PoC-2**

- The button rectangle does not overlap any native tile at the measured geometry.
- Press-event logs record whether the native handler sees a press inside the button rectangle (ordering, A1). Ordering is an unproven candidate, so the result is recorded either way and is not assumed.

### 6.3 Environment

- A **disposable copy** of the game install and a **disposable copy** of a save, on a separate Windows machine or VM. Not the user's working install, and not the repo.
- The mod is loaded only in that copy.
- Do **not** run `install.ps1`, `uninstall.ps1`, or the migration scripts. The installer overwrites `dinput8.dll`, and migration can rewrite save-related files [repo doc: `docs/almanac-frontline-triage.md`].
- No test artifacts are written to the repo except logs and hashes under `export/`, with no save data and no game binaries.

### 6.4 Backups (before any test)

- SHA-256 manifest and full copies, stored outside the repo, of: the game executable and `dinput8.dll`; every plugin DLL; the loader configuration (path under `tesmioloader`, to be confirmed); `taxes.ini`, `state_ledger.ini`, `reputation.ini`, `ui_panels.ini`; `tesmio_taxes.dat`; `tesmio_ledger.csv`; and the full save folder.
- The manifest is recorded in the repo as hashes only.

### 6.5 Stop conditions

Expected events are not failures. Each observation is classified. Only unexpected events stop the test.

| Observation | Class | Action |
|---|---|---|
| Tester's native click from the checklist, with the native selection changing as a result | Expected | Log the click and the result |
| Native selection changes with no logged test click | Stop: unexplained, possibly mod-origin | Stop, then run §6.6 |
| Native tile missing, duplicated, or moved, outside an approved Path A change set | Stop | Stop, then run §6.6 |
| Native tile not clickable during the checklist | Stop | Stop, then run §6.6 |
| File hash changes that match the approved build manifest (DLL or config from the approved build) | Expected | Verify each hash against the manifest |
| File changes outside the approved build manifest | Stop | Stop, then run §6.6 |
| `tesmio_taxes.dat` or `taxes.ini` changes after a logged test setter call or save load | Expected | Log the change |
| Same files change with no logged action | Stop | Stop, then run §6.6 |
| Money movement attributable to a logged source (test write, native action, scheduled settlement) | Expected | Log the source |
| Money movement with no attributable source, or an unexplained treasury change | Stop | Stop, then run §6.6 |
| Non-finite value reaching a tax setter from the Almanac's own code path | Stop: pre-check failure | Stop, then run §6.6 |
| Deliberate non-finite probe sent to the authoritative setter | Not part of this PoC. Needs a separate approval | Do not run unless approved |
| Crash, save-load failure, or City Hall regression | Stop | Stop, then run §6.6 |
| Repo change outside the approved change set | Stop | Stop |
| Ledger reports `unexplainedValid = 1` or a zero unexplained figure while tracking is absent (`unknown`) | Stop: false completeness | Stop, then run §6.6 |

On any stop: halt input, record the observation with its timestamp and the log, then run §6.6.

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
9. **Mixed currency cannot be detected** until the tracking in §4.2 exists. Until then the unexplained figure is unreliable, and the snapshot reports `unknown`.
10. **Observer loss (A4)** can make the ledger silently incomplete. The snapshot states this limit.
11. **Unit provenance of the property value is unknown** after a save load or a setter call (§3.4). Startup currency is not used to infer it.
12. **Consistency limits** (§4.3): flows and treasury can be sampled at different instants, and observers run on the caller's thread.

---

## 8. Decisions needed before any implementation

1. Confirm D1: Path B as lead, Path A as alternative.
2. Confirm D2: a new taxes v3 snapshot view, with v1 and v2 unchanged.
3. Confirm D3: an Almanac-side pre-check that is finite and ≥0, with no upper bound imposed. Any finite check or cap in the tax service is a separate decision.
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

---

## 10. Revision 2: PR #67 review points

This revision changes only this specification. Raw evidence and earlier reports are unchanged.

1. Saved property-tax currency provenance is `unknown` when it cannot be established. Startup currency is not used to infer it (§1.1, §3.1, §3.2, §3.4).
2. The ledger snapshot does not claim to detect mixed currency or complete periods without the tracking in §4.2. Consistency limits are stated in §4.3.
3. PoC stop conditions distinguish expected native clicks and approved build changes from unexplained or mod-origin changes (§6.5).
4. One unavailable-service policy applies everywhere. No tax-status claim is made from a resolution failure (§5.0).
5. Input ordering, global hosting, and feature rollback are unproven design candidates (§2.2, §2.3, §2.7). The City Hall slider ceiling is a UI mapping, not a tax-policy limit (§3.1, §5.2).
6. Both same-row approaches, the first-release finance and adjustable-tax scope, and the Economy & Trade fallback are retained (§0, §2, §3, §4).
