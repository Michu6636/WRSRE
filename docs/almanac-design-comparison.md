# Almanac: static design comparison, Path A vs Path B

**Status:** design comparison only. No implementation, no hooks, no game or mod execution, no file deployment, no game-file or save changes. Nothing here is validated live.

**Labels used throughout**

- **[DLL]**: observed in the bytes or disassembly of `reputation.dll` (SHA-256 `fd7d9459…`), or in an exported native excerpt already in the repo. These are addresses and facts, not runtime behavior.
- **[CFG]**: a claim in `reputation.ini` or the author's comments. Not verified.
- **[DOC]**: a claim in a repo document, including the Almanac plan and earlier reviews. Treat as a lead.
- **[TAX]**: a fact from the tax service header, `src/taxes_api.h`.
- **[source]**: read from repo source or config (`plugins/…`). Not runtime evidence; no build or run was done.
- **[OPEN]**: unknown. Needs a static check or a later live test.

Reputation's H1 result is **not** assumed to apply to the Almanac. The Almanac is a new independent implementation, so its behavior must be shown by its own checks.

## 0. Status and qualifications

- **Candidates.** Both Path A and Path B remain candidates. **Path B is the initial design lead**, because it does not append to the native vector or reorder native tiles. This is a lead for checks, not a decision.
- **Input ownership is not settled.** A separate button does not necessarily require input handling before the native handler. Whether the mod's hit-test must run first, run after, or coexist depends on the trade-row dispatch order, which is not established. Input ownership and ordering remain to be validated.
- **Close/return is not one signal.** A changed native selection index alone is not a complete close/return detector. It does not cover repeated clicks on the same tile, section changes that leave the index unchanged, or Esc. Each needs its own rule and check.
- **Alternate-handler scope.** The last-item omission applies to the **examined path** (`[game+0x10398] == 0` at the examined host dispatch). It is not established for every observed game mode, and how often that path runs is unknown.
- **Tax values.** Prefer an **authoritative, version-compatible getter or snapshot** for configured VAT, profit, and property values, including suspended taxes. The current v2 contract lacks one (§2, §7).
- **Economy & Trade fallback.** The page fallback is already acceptable if a separate global section proves unsafe, provided the Almanac keeps its own same-row button (§6).
- **No live test is approved.** A later proposal must name a specific environment, backups, stop conditions, and verified restoration (§9).

---

## 1. Requirements

From `docs/almanac-plan.md` **[DOC]**:

1. A same-row button for the Almanac.
2. Dynamic placement, so the button position is computed rather than hard-coded.
3. The first release includes fiscal information and adjustable **VAT, profit tax, and flat per-enterprise property tax**. The shipped taxes code scales property tax by land value by default (see §7), so "flat" needs confirming.
4. The tax service is the authority for rates and state. Any copy the panel keeps for display must never override the service.
5. City Hall must keep working.
6. The Economy & Trade page is an acceptable fallback only if the same-row button is also present.

Requirement 3 means the tax controls cannot be deferred to a later release. This applies to both paths.

---

## 2. Shared facts

These apply whichever path is chosen.

**Native trade row [DLL / native excerpts]**

- The host dispatch at `0x14031EDE6` selects the alternate handler `0x140361BB0` when `[game+0x10398]` is zero **[DLL/native]**.
- The alternate handler never draws or clicks its **last** entry **[DLL/native]**.
- The normal handler `0x140373EA0` maps index `i < 2 → i`, `i == 2 → count−1`, `i > 2 → i−1`, and draws all entries **[DLL/native]**. A native pick at index 2 therefore reaches the last item, not the third.
- Pitch, origin, and the nine-tile fit are **unverified** **[OPEN]**. The native excerpts do not give tile coordinates.
- The native selected trade-item index is at executable offset `exe+0x9E156C`. The DLL compares it with its own global `0x1800C3C5C`, which the insertion path stores at `0x180058190` **[DLL, per the Reputation follow-up review]**. The earlier assumption that the trade-row click writes `0x180411BCC` was wrong **[DLL]**; that address is written only in the case loop at `0x180057617`.
- **Consequence for both paths:** on the **examined path** (`[game+0x10398] == 0`), the last native tile is not drawn or clicked. This is a property of that native path, not of either design. It is not established for every observed game mode. Whether other modes reach the examined path is **[OPEN]**, so "all eight native icons stay visible and clickable" is not guaranteed by either path until that is known.

**The `0x140374606` call-site [DLL/native]**

- In `FUN_140373EA0`, the comparison at `0x1403745F1–F8` is **unsigned**. Only native indices 0 and 1 reach the helper call at `0x140374606`. Any index above 1, including `0xFFFFFFFF`, skips it.
- So replacing or skipping that call does not, by itself, cover native selections of index 2 or more. Other native paths (`0x140374A78–ACC`, `0x140374AD6–B35`, `0x140374B60–BBF`) also run for other selections **[DLL/native]**. Their visual and input effects are not established.
- The helper at `0x14035F3E0` has side effects beyond drawing: it writes through its third parameter and touches native state and input **[DLL/native]**. Its output local is consumed at `0x14037460B–619`. A design that skips it must account for this. No design is established to be safe by skipping it.

**Reputation's sidebar and panel mechanism [DLL]**

- The mod's sidebar button is a copy of a native record appended through `exe+0x459530` to vector `exe+0x9E1518/1520`, stride `0x68` **[DLL]**.
- A native sub-list row record is `0x50` bytes: name at `+0x00`, string ID at `+0x40`, texture at `+0x48`. The insertion function `0x180057E40` pushes through `exe+0x4492F0` (call at `0x1800580DC`). It has **no capacity check** before the push. A null texture rolls back at `0x180058212` **[DLL]**.
- `sidebar_hides_panel` (effective value `0x1800C3C74`) is state-dependent. The load block at `0x180048CC2–0x180048D14` forces it to 0 when `in_panel` and `sidebar` are both on **[DLL]**. The config states the same **[CFG]**.
- The re-select rule `0x1800593B0` does **not** read `sidebar_hides_panel` **[DLL]**.

**Tax service [TAX]**

- Tax rates and state are owned by the `taxes` service (v2 adds the fiscal-policy tail).
- `isTaxEnabled(kind)`, `setTaxRate(kind, rate)`, and `setTaxEnabled(kind, on)` are provided for VAT, profit, and property (`TSM_TAX_VAT/PROFIT/PROPERTY`).
- Rates are fractions in [0, 1] for VAT and profit. Property is a flat amount per enterprise, `>= 0`, in the active currency.
- `getVatRate` and `getProfitTaxRate` return **0 when the tax is disabled**. The v2 tail has **no getter for the property amount**, and the header says the property amount is not exposed.
- Consequence: the panel cannot read back a configured rate from the service while a tax is suspended, and cannot read the property amount at all with the current contract. This is a real gap for the first release **[TAX]**. The **preferred** fix is an authoritative, version-compatible getter or snapshot that returns configured values, including for suspended taxes. A display-only copy is a fallback, never authority **[OPEN]**.
- If the taxes service is absent, enterprise_base treats it as "no tax". The panel must show that state rather than a rate **[TAX]**.

**Currency [TAX]**

- The treasury is `game+0x580` (USD) and `+0x588` (RUB). Money moves are labelled, and tax remittances use labels `vat`, `profit_tax`, and `property_tax` **[TAX / currency header]**. The fiscal section should read fiscal totals from the service, not recompute them.

---

## 3. Path A: native inserted row item opening a separate global panel

**Idea:** insert an Almanac record into the native trade row vector, so it renders and clicks like a native tile. Selecting it opens a separate global Almanac panel.

**Technical risks (smallest first)**

1. **Alternate handler drops the last item [DLL/native].** On the examined path (`[game+0x10398] == 0`) the last entry is never drawn or clicked. Other game modes are not established. If the Almanac is appended last, it disappears silently on that path. Placing it elsewhere moves a native tile instead.
2. **Reordering [DLL/native].** The normal handler's mapping (`i > 2 → i−1`, `i == 2 → count−1`) means a new entry changes which native tile sits in each slot. Native users see a different row.
3. **No capacity check [DLL].** The push at `0x1800580DC` has no bound check in the DLL before it runs. Whether the native push itself is safe at the vector's limit is **[OPEN]**.
4. **Content path is undefined.** Selecting the Almanac sets a native selected index. Native paths for that index are not established to be safe (see §2 and the `0x140374606` note). Suppressing vanilla content for a row item is unresolved [OPEN].
5. **Geometry [OPEN].** The nine-tile fit, pitch, and origin are not established by the native excerpts.

**Keeping all eight native icons and the Almanac visible and clickable**

- Not established. A row item adds a ninth entry. Whether nine tiles fit, and whether the native handler lays them out without clipping, is unverified **[OPEN]**.
- The reorder in risk 2 means the eight native icons may not keep their slots. If the Almanac is appended last, the alternate handler never shows it, so the requirement fails on that path. The omitted last native tile is also a native-path issue (see §2).
- Only a live layout and click test can confirm this. Static checks cannot.

**Close and return**

- Selecting a native tile after the Almanac changes the native selected index. Whether the global panel then closes depends on a mod-owned rule. The only DLL precedent is the re-select rule `0x1800593B0`, which does not read `sidebar_hides_panel` **[DLL]**. Whether that rule covers an Almanac selection is **[OPEN]**.
- The native content for a restored selection depends on the native paths in §2. Restoration after a bypass is not established.
- A changed native index is **not** a complete detector. Repeated clicks on the same tile, section changes that leave the index unchanged, and Esc each need their own rule. All are **[OPEN]**.

**Assessment:** the reorder is a defect Path A creates. The alternate-handler omission is a native-path condition that Path A does not remove (§2). Path A cannot show that "all eight plus the Almanac" holds without also controlling which handler runs, which is not established as safe.

---

## 4. Path B: separate dynamically positioned button in the same row, opening the same panel

**Idea:** leave the native vector alone. Draw and hit-test a separate Almanac button in the trade row, computed at runtime from the row's geometry, and open the same global panel as Path A.

**Technical risks (smallest first)**

1. **Anchor geometry [OPEN].** The button must be placed relative to the native row. The native tile coordinates are not established statically. The placement rule (for example, to the right of the last visible tile) needs a live measurement or a verified read of the row layout.
2. **Space [OPEN].** The row may have no free slot. If it is full, a same-row button would overlap a native tile. The plan requires the same row, so this must be measured.
3. **Input ownership and ordering [OPEN].** A separate button does not necessarily require input handling before the native handler. Whether the mod's hit-test must run first, run after, or coexist depends on dispatch order, which is not established. The button must not steal clicks on native tiles. Input ownership and ordering remain to be validated. The `ui_panels` building precedent is building-window-specific and is not a safe template for this row **[DOC]**.
4. **Clipping and drawing order [OPEN].** Whether the button is clipped by the native panel, and whether it draws above native content, is not established. The review found no proof for the clipping claims in earlier notes **[DOC]**.
5. **Native content suppression.** Path B avoids the vector append and the reorder. It does not remove the need to check native selection paths, because the panel opens while a native selection may still be active.
6. **Native selection and the open flag.** The open state is mod-owned. The panel must not drive native selection, so the native selected index stays where it was.

**Keeping all eight native icons and the Almanac visible and clickable**

- Native icons are untouched in the vector, so the native order and alternate-handler behavior are unchanged. The eight native icons keep their existing visibility, **unless** the button overlaps one (risk 2).
- The Almanac stays visible and clickable only if the anchor geometry (risk 1) and input ownership (risk 3) are correct. Both need live checks.
- Only the eight native tiles matter for the native rule. The alternate-handler omission of the last native icon is a **pre-existing** property of the native path, not caused by Path B. The design must still show that the last native tile is visible and clickable on that path. If it is not, Path B cannot fix it; the native handler would need a separate, unproven change.

**Close and return**

- Open: the button sets the mod-owned open flag and shows the global panel. The native selection is not changed.
- Close by the Almanac's own close control or Esc: clears the flag and hides the panel.
- Return to native: clicking a native tile should close the panel. The design needs a detection rule, such as comparing the native selected index (`exe+0x9E156C`) with the value stored at open. The DLL's re-select rule at `0x1800593B0` is a precedent for reacting to a changed selection, but it does not cover this case **[DLL]**. The Almanac rule must be built and tested, not assumed. It must also cover repeated clicks, section changes, and Esc (§0).
- Close on native change, including a click on the Almanac button itself, is **[OPEN]**.

**Assessment:** Path B avoids the two defects Path A creates (vector append and reorder). It is the initial design lead, not a proven safer design. Its geometry and input risks need checks, some static and some live.

---

## 5. Comparison

| Criterion | Path A (row item) | Path B (separate button) |
|---|---|---|
| Native vector mutated | Yes, with no capacity check [DLL] | No |
| Known native defect | Alternate handler drops last item (also applies to B); reorder [DLL/native] | Alternate-handler omission still applies (§2); geometry and input unverified |
| Eight native icons keep their slots | No, reorder expected [DLL/native] | Yes, unless overlapped |
| Almanac visible and clickable | Depends on nine-tile fit [OPEN] | Depends on anchor and input ownership [OPEN] |
| Content path | Undefined; native selection involved [OPEN] | Native selection not changed; checks still needed |
| Close and return | Depends on re-select rule (no H1 assumed) [OPEN] | Needs mod-owned detection rule [OPEN] |
| Dependency on native handler selection | High | Low, but the panel must not be opened during unsafe native states [OPEN] |
| Static checks possible now | Yes (native map, geometry) | Partly (vector and reorder are avoided) |
| Live tests needed | Yes | Yes |

**Position:** both remain candidates. Path B is the initial design lead, because it does not mutate the native vector or reorder native tiles. Neither is established as safer. Both need validation before a design is chosen. No live test is approved (§9), and neither path is approved for implementation.

---

## 6. Economy & Trade page fallback

The plan allows the Economy & Trade page as a fallback **only with a same-row button** **[DOC]**.

- The page fallback is a separate panel reached through the page, not the row. It does not alter the native row, so it avoids the row defects in §3 and §4.
- The same-row button is still required. The page fallback is therefore Path B plus a page entry, not a replacement for it.
- The page is an existing stats-window family, not a building window **[DOC: `docs/UI investigation.md` §6]**. Its builder has not been located in this analysis. Locating it is a static check, and must not use a Ghidra project that is locked or in use.
- The page fallback is **already acceptable** if a separate global section proves unsafe, provided the Almanac keeps its own same-row button and that button passes its checks. The fallback does not replace the same-row requirement.

---

## 7. First-release taxes

Required in the first release **[DOC]**: VAT, profit tax, and flat per-enterprise property tax, adjustable from the Almanac panel.

- **Authority:** the plan says the tax service is the authority **[DOC]**. Writes go through `setTaxRate`, `setTaxEnabled`, and reads of on/off go through `isTaxEnabled` **[TAX]**.
- **Display gap:** the service returns 0 for a disabled rate, and has no getter for the property amount **[TAX]**. The **preferred** route is an authoritative, read-only getter or snapshot of configured VAT, profit, and property values, including suspended taxes. The taxes API header says the registry matches versions exactly, and that new fields are added by appending and publishing both views (as the v1/v2 precedent does). A new getter therefore needs a new version tail and dual publication, and the Almanac must request the version that carries it **[TAX; header precedent]**. A display-only copy is a fallback that never overrides the service **[OPEN]**.
- **Property basis (source, not runtime):** the taxes source describes property tax as "a flat per-enterprise charge, on no base at all" in its header, but the code settles `g_propertyTaxFlat` and, when `property_land_value` is on, scales it by attractiveness (`plugins/taxes/taxes.cpp` around lines 498–505). The shipped default is `property_land_value = 1` (`plugins/taxes/taxes.ini` line 85). The config comment states `Tax = property_tax_flat x (0.50 + 1.50 x attractiveness)`. So the value the panel shows as "flat" is the **base**, not the amount each enterprise pays. The plan's "flat" wording needs confirming, and the panel label must match what is actually settled **[DOC/source; OPEN]**.
- **Setter units (source):** `setTaxRate` for property writes the value directly into `g_propertyTaxFlat`. The header says the value is "in the active currency". The config value is RUB-denominated and converted to USD at Start. Whether a value written through `setTaxRate` is in the active currency, with no conversion, is not established **[OPEN]**.
- **Existing City Hall controls (source):** the taxes plugin already draws VAT, profit, and property sliders and per-tax enable checkboxes through `TaxPanelWidgets` in the City Hall tax tile (`plugins/taxes/taxes.cpp` header, v3.0 and v3.3 notes). The Almanac must not create a second control surface that writes the same setters without an agreed rule. Any Almanac control must use the same authority. This matters for "City Hall must keep working" **[source; OPEN]**.
- **Finance and ledger contract (source):**
  - `state_ledger` provides **no service** and has **no header**. It consumes sim_clock, currency, persistence, and ui_overlay, and writes HUD and CSV output **[source]**.
  - Its monthly flows are kept in process memory (`g_monthFlows`), cleared on month change and rewind, and written to `tesmio_ledger.csv` (`csv = 1` in `state_ledger.ini`). Neither is a query API **[source]**.
  - The Almanac could register its own money-flow observer through the currency `registerFlowObserver`. That call sits in the body before the v4 tail, so v3 and v4 consumers can use it. Its totals would start at registration, miss earlier flows and reloads, and are not authoritative for month totals **[source]**.
  - **Assessment:** the ledger is not usable as the authority for fiscal totals. Options are a new read-only snapshot in `state_ledger` (a new provider with version rules), or session-only observer totals labelled as such. The choice is **[OPEN]**.
- **Accepted writes:** a rate setter returns 0 on out-of-range values and leaves the current value unchanged. The panel must show that refusal, not assume success **[TAX]**.
- **Timing:** changes take effect at the next accrual or settlement boundary **[TAX]**. The panel must say so; it must not imply immediate effect.
- **Absent service:** if the taxes service is missing, the panel shows "no tax" and disables the controls **[TAX]**.
- **City Hall:** must keep working. Any change to shared panel or tax code must not alter City Hall's behavior **[DOC]**. This is a static check: confirm City Hall does not share the changed entry point.

---

## 8. Static checks (can be done now, no game execution)

These are repo and DLL inspections. They need no game run and no hooks.

| ID | Check | Path |
|---|---|---|
| S1 | Re-derive the native trade-row mapping (`i<2`, `i==2`, `i>2`) and the alternate-handler omission from the tracked exports, and confirm the row count used | A, B |
| S2 | Confirm no capacity check on the append at `0x1800580DC` (already recorded; re-read the raw bytes) | A |
| S3 | Map every native path reached by a selected index of 2 or more (`0x140374A78`, `0x140374AD6`, `0x140374B60`), and their side effects | A, B |
| S4 | Establish whether the button's rectangle can overlap a native tile, using any verified layout data in the exports; if none, record as unknown | B |
| S5 | Locate the existing input-dispatch order for the trade row, to show the mod hit-test can run first | B |
| S6 | Confirm the taxes service provides the three v2 functions for the header in use; confirm no getter for property exists | A, B |
| S7 | Confirm City Hall's entry point does not share the Almanac code path | A, B |
| S8 | Check the Almanac plan's requirements against this document (no requirement omitted) | A, B |
| S9 | Confirm the property-tax basis (`property_land_value`, the attractiveness scaling) in the shipped config and code, and whether the plan's "flat" wording is intended | A, B |
| S10 | Confirm the unit of `setTaxRate` for property against the RUB/USD conversion at Start | A, B |
| S11 | Confirm which taxes API version each consumer requests, and whether a new getter needs a v3 tail with dual publication | A, B |
| S12 | Confirm there is no existing state_ledger query path and that the currency observer has the version the Almanac would request | A, B |
| S13 | Map the existing City Hall tax controls against the proposed Almanac controls, to find duplicate write paths | A, B |

Static checks cannot show clipping, visibility, click routing, or close behavior. Those are live tests.

---

## 9. Live tests (later; require explicit authorization)

**No live test is approved.** Nothing in this section may run until the user explicitly authorizes it. A later proposal must name a specific environment, backups, stop conditions, and verified restoration, and each test needs a rollback path (§10).

| ID | Test | Pass criterion |
|---|---|---|
| L1 | Measure the native trade-row layout with the mod disabled | Recorded coordinates for all eight tiles |
| L2 | Path A: append the item and verify all nine entries | All eight native icons and the Almanac visible and clickable on both handler paths |
| L3 | Path B: place the button at the computed anchor | Button visible; no native tile overlapped or clipped |
| L4 | Path B: click each native tile with the button present | Each native tile still selects; the button does not steal native clicks |
| L5 | Open the Almanac; click each native tile | Panel closes; native selection and content are restored |
| L6 | Esc and the panel's close control | Panel closes; no native state is left changed |
| L7 | Tax controls: change VAT, profit, property; toggle each | Values round-trip through the service; refusals are shown |
| L8 | Taxes service absent | Panel shows "no tax" and controls are disabled |
| L9 | City Hall after all changes | City Hall opens and works as before |
| L10 | Economy & Trade page fallback (only if used) | Page entry works, and the same-row button still passes L3–L5 |

**Note:** L2 and L3 change the game UI. They therefore require explicit user authorization and a tested rollback. They are not read-only.

---

## 10. Rollback

- The committed comparison records `sidebar_item = 0` plus a restart as rollback **[CFG / committed report]**. That key gates the existing Reputation sidebar item. It is **not** an Almanac switch, and no Almanac switch exists yet.
- There is **no runtime removal path** for the appended native record **[DLL]**. For Path A, rollback is therefore a restart with the Almanac's own switch off, plus a check that the native vector is restored. That switch must be designed.
- For Path B, the button's own switch must be designed. The native vector is not touched, so a restart with the switch off should restore the row.
- Rollback must be tested before any live test (L2–L5) runs. Until the switches are designed, no rollback is claimed.

---

## 11. Decisions needed

1. Keep both paths as candidates, with Path B as the initial design lead (§0).
2. For the configured-value gap (§7), choose a version-compatible getter or snapshot in the taxes contract (preferred), or a display-only fallback.
3. Confirm the property-tax basis: flat per enterprise, or land-value scaled as the shipped code does.
4. Choose the fiscal-totals source: a new `state_ledger` snapshot, or session-only observer totals labelled as such.
5. Decide how the Almanac's tax controls share authority with the existing City Hall controls.
6. Any live test proposal must come later, with environment, backups, stop conditions, and verified restoration. None is approved.

---

## 12. Limits of this document

- Nothing here is validated by running the game or the mod.
- Native addresses are from exported excerpts; the executable itself is not in the workspace. Its hash is unverified.
- The `0x180411BCC` index and the Reputation H1 result are not used as evidence for the Almanac.
- The tax and ledger points in §7 are read from repo source and config. They are not runtime evidence, and no build was run.
- Earlier committed reports are unchanged. Corrections in this document supersede them only where stated.
