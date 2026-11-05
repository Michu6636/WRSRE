# Almanac UX, Acceptance Matrix, and Synthetic Fixtures

Status: planning/acceptance artifact for worktree `04-almanac-ux-validation`.
Scope: documentation and synthetic test fixtures only. No production C++, plugin
configuration, host/shell/data module, build script, loader registration, or
address registration changes are made by this artifact.

## 1. Purpose and boundaries

This document defines the Almanac user experience before shell and page
implementations are integrated. It is written to be consumed by later
implementation worktrees. It owns only:

- `docs/almanac/04-ux-and-acceptance.md`
- `tests/almanac/fixtures/*`
- `tests/almanac/test_ux_fixtures.py`
- `tests/almanac/fixture_validation.py`

It deliberately does not implement or enable any runtime feature. Deferred
runtime acceptance criteria are listed as `deferred`, never as `passing`.

## 2. First integrated milestone

The first integrated milestone is intentionally small and read-only except for
the controls that already exist:

- Read-only **Overview**.
- **Fiscal Policy** containing the existing tax and bank controls already
  provided by `taxes` and `republic_bank` through City Hall.
- All other pages (Money Flow, Private Economy, Prices & Citizens, Logistics &
  Production, Causation) are specified here but may ship as read-only or
  placeholder shells in the first milestone.
- **City Hall remains an unchanged fallback** during migration. No Almanac page
  may remove, replace, or alter the existing City Hall fiscal panel.

## 3. Page set and flow-layout wireframes

All layouts are flow-based. No absolute X/Y screen coordinates are used for
Almanac content. Rows append at the current bottom edge and advance it. DPI
scaling uses the game scalar at `0x992088`.

Legend: `[ ]` text row, `{ }` control row, `---` separator, `>` selected page.

### 3.1 Overview

```
Almanac > Overview
[ Title: Overview ]
[ Treasury: 12,340 RUB  |  246.80 USD ]   (two currency columns, not merged)
[ Period: day 142 / month 5 / year 3 ]
[ Net cash flow (ACTUAL): +420 RUB ]
[ Open liabilities (ACTUAL): 1,000 RUB ]
[ Pending dry-run operations: 2 ]          (DRY_RUN count only, no value merge)
[ Estimates: 3 stale ]                     (ESTIMATED count only)
[ Empty-state: "No data source" when service missing ]
```

### 3.2 Fiscal Policy

```
Almanac > Fiscal Policy
[ Title: Fiscal Policy ]
{ VAT rate slider (ACTUAL control) }
{ Profit tax rate slider (ACTUAL control) }
{ Property tax flat slider (ACTUAL control) }
[ ] VAT enabled (ACTUAL flag)
[ ] Profit tax enabled (ACTUAL flag)
[ ] Property tax enabled (ACTUAL flag)
[ ] City Hall fallback: unchanged ]
{ Bank rate slider (ACTUAL control) }
[ ] Bank rate (authoritative stored value) ]
```

### 3.3 Money Flow

```
Almanac > Money Flow
[ Title: Money Flow ]
[ Section: ACTUAL flows ]
[ rows: source -> destination, amount, currency, unit ]
[ Section: DRY_RUN operations ]
[ rows: operation, would-credit, currency, unit, posted=false ]
[ Section: ESTIMATED projections ]
[ rows: metric, amount, currency, basis, confidence, stale-after ]
[ Totals: one per value class and currency; never merged ]
```

### 3.4 Private Economy

```
Almanac > Private Economy
[ Title: Private Economy ]
[ Enterprise count, solvency share, investment pool ]
[ Wages and tax accruals (ACTUAL) ]
[ Dry-run procurement previews (DRY_RUN) ]
[ Estimated revenue (ESTIMATED) ]
[ Empty state when enterprise service missing ]
```

### 3.5 Prices & Citizens

```
Almanac > Prices & Citizens
[ Title: Prices & Citizens ]
[ CPI basket (ACTUAL price basis) ]
[ Poverty rate (ACTUAL) ]
[ Wage levels (ACTUAL) ]
[ Estimated price drift (ESTIMATED) ]
[ Missing source: "resource_pricing unavailable" ]
```

### 3.6 Logistics & Production

```
Almanac > Logistics & Production
[ Title: Logistics & Production ]
[ Section: ACTUAL deliveries ]
[ Section: DRY_RUN targets ]
[ Section: ESTIMATED throughput ]
[ Residuals: unmatched / partial unload (unresolved) ]
[ Truncated diagnostics marked incomplete ]
```

### 3.7 Causation

```
Almanac > Causation
[ Title: Causation ]
[ Event timeline (timestamp, category, label) ]
[ Edge list with relation and provenance ]
[ Missing parents panel ]
[ Truncated history indicator ]
[ Unsupported edges panel ]
[ Rule: shared category or timestamp alone is not causation ]
```

## 4. Internal page navigation

The Almanac must not rely on native tab indices and must not mutate the native
panel vector (`game+0xC608`). Navigation is internal to the Almanac shell.

Required behavior:

- Page selection is an internal enum/state value, not a native tab index.
- Default page is Overview.
- Back behavior returns to the previous internal page; at the root it returns
  to Overview.
- Scrolling is internal to the Almanac content region and must clip to the
  visible content rectangle.
- DPI changes rescale row height, padding, and clip rectangle together.
- Long labels wrap or truncate with an ellipsis; they must not overflow the
  panel or clip adjacent controls.
- Narrow panels reduce column counts and hide non-essential columns before
  controls become unusable.
- Empty states render a single explanatory row when a source or dataset is absent.
- Unavailable sources render "unavailable" and disable dependent controls; they
  do not crash.
- Stale data renders a stale marker with the last update day/time.
- Host disappearance (panel closed or host removed) clears selection state and
  stops rendering without leaking rows.

## 5. Value-class separation

`ACTUAL`, `DRY_RUN`, and `ESTIMATED` are distinct value classes.

- They must be visibly and structurally distinct: separate sections, columns, or
  totals.
- Colour alone is insufficient.
- No combined total may silently mix value classes, currencies, or incompatible
  units.
- Totals are per value class and per currency.
- B2B dry-run output is never treated as posted revenue.
- Successful pairing is not billing evidence.

## 6. Causation page rules

- Missing parents are listed explicitly, not silently dropped.
- Truncated history is marked incomplete and never treated as complete evidence.
- Unsupported edges are listed as unsupported, not as causation.
- Unresolved provenance prevents a causal claim.
- Shared category or timestamp alone does not imply causation.
- Diagnostic truncation means incomplete evidence, not proof of phantom income.

## 7. City Hall fallback

During migration, City Hall remains an unchanged fallback. The Almanac must not
remove or alter the existing City Hall fiscal panel. If Almanac pages are
unavailable, City Hall remains usable.

## 8. Production host gating

The production host remains disabled until native geometry, selection, input,
lifecycle, and hook-safety acceptance is complete. No production host code is
enabled by this artifact.

## 9. Acceptance matrix

Columns: ID | Criterion | Setup | Expected | Owning module | Type | Status.

| ID | Criterion | Setup | Expected | Owning module | Type | Status |
|---|---|---|---|---|---|---|
| ALM-NAV-001 | Page selection is internal | Open Almanac | Selected page changes without native tab index mutation | Almanac shell | synthetic | specified |
| ALM-NAV-002 | Default page Overview | Launch Almanac | Overview is first visible page | Almanac shell | synthetic | specified |
| ALM-NAV-003 | Back returns previous page | Navigate Overview -> Fiscal Policy -> Back | Fiscal Policy then Overview | Almanac shell | synthetic | specified |
| ALM-NAV-004 | Back at root stays Overview | Press Back on Overview | No crash; remains Overview | Almanac shell | synthetic | specified |
| ALM-INPUT-001 | Input only inside visible unclipped controls | Render clipped control | Input ignored for clipped control | Almanac shell | synthetic | specified |
| ALM-SCROLL-001 | Scrolling clips content | Scroll long page | Content clipped to visible rectangle | Almanac shell | synthetic | specified |
| ALM-DPI-001 | DPI changes rescale layout | Change DPI | Row height, padding, clip scale together | Almanac shell | synthetic | specified |
| ALM-LABEL-001 | Long labels do not overflow | Render very long label | Wrap or ellipsis; no overlap | Almanac shell | synthetic | specified |
| ALM-NARROW-001 | Narrow panels degrade gracefully | Narrow panel width | Non-essential columns hidden; controls usable | Almanac shell | synthetic | specified |
| ALM-EMPTY-001 | Empty state renders | Missing dataset | Single explanatory row | Almanac page | synthetic | specified |
| ALM-SOURCE-001 | Unavailable source handled | Disable service | "unavailable" row; no crash | Almanac page | synthetic | specified |
| ALM-STALE-001 | Stale data marked | Old timestamp | Stale marker with last update | Almanac page | synthetic | specified |
| ALM-HOST-001 | Host disappearance clears state | Close host | Rendering stops; no leaked rows | Almanac shell | synthetic | specified |
| ALM-LIFE-001 | Coherent render snapshots | Repeated frames | No torn or partial snapshot | Almanac shell | integration | deferred |
| ALM-LIFE-002 | Lifecycle invalidation | Save/load | State invalidates and rebuilds | Almanac shell | integration | deferred |
| ALM-GROWTH-001 | No per-frame unbounded growth | 10k frames | Memory and row count bounded | Almanac shell | integration | deferred |
| ALM-SAVE-001 | Save/load round trip | Save then load | Displayed values restored | Almanac pages | integration | deferred |
| ALM-TIME-001 | Backward-time reset | Load older save | No stale future data | Almanac shell | integration | deferred |
| ALM-SERVICE-001 | Missing optional services | Remove plugin | Graceful degradation | Almanac pages | integration | deferred |
| ALM-TAX-001 | Tax/bank displayed and stored values agree | Change slider | Displayed equals authoritative stored | taxes / republic_bank | integration | deferred |
| ALM-BANK-001 | Legacy bank-rate normalization intact | Load old save | Rate normalized | republic_bank | integration | deferred |
| ALM-B2B-001 | B2B dry-run/actual separation | Run dry-run | No actual treasury movement | private_logistics | integration | deferred |
| ALM-B2B-002 | No billing from pairing success | Pair succeeds | Residual remains unresolved | private_logistics | integration | deferred |
| ALM-FALLBACK-001 | City Hall fallback | Disable Almanac | City Hall fiscal panel unchanged | taxes | integration | deferred |
| ALM-HOSTGATE-001 | Production host disabled | Build default | Host code not enabled | Almanac host | Windows-build | deferred |
| ALM-NATIVE-001 | Native geometry acceptance | Runtime probe | Geometry verified | Almanac host | native-runtime | deferred |
| ALM-NATIVE-002 | Native selection acceptance | Runtime probe | Selection verified | Almanac host | native-runtime | deferred |
| ALM-NATIVE-003 | Native input acceptance | Runtime probe | Input verified | Almanac host | native-runtime | deferred |
| ALM-NATIVE-004 | Native lifecycle acceptance | Runtime probe | Lifecycle verified | Almanac host | native-runtime | deferred |
| ALM-NATIVE-005 | Hook safety acceptance | Runtime probe | Hooks safe | Almanac host | native-runtime | deferred |

## 10. Synthetic fixtures

Fixtures live under `tests/almanac/fixtures/` and are validated by
`tests/almanac/test_ux_fixtures.py`.

| Fixture | Purpose |
|---|---|
| `actual_flows.json` | Actual treasury/enterprise flows |
| `dry_run_output.json` | Dry-run operations, not posted |
| `estimates.json` | Estimated projections |
| `mixed_currencies.json` | RUB and USD kept separate |
| `truncated_diagnostics.json` | Truncated diagnostic capture |
| `incomplete_residuals.json` | Unresolved residuals |
| `causation_provenance.json` | Causation events, edges, missing parents |

Diagnostic truncation is represented as incomplete evidence, never as proof of
phantom income.

## 11. Deferred criteria

The following remain deferred and are not marked passing:

- All `integration`, `Windows-build`, and `native-runtime` rows in the
  acceptance matrix.
- Production host enablement.
- Native geometry, selection, input, lifecycle, and hook-safety acceptance.

## 12. Verification command

```text
python tests/almanac/test_ux_fixtures.py
```

Expected: all tests pass.