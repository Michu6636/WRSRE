# Almanac Investigation Report

**Project:** Workers & Resources: Soviet Republic mod suite (WRSRE)  
**Requested artifact:** `docs/almanac-investigation.md`  
**Scope:** Investigation and planning only. No implementation, gameplay change, hook, runtime diagnostic, build, deployment, settings change, or save modification.  
**Date prepared:** 2026-11-07 (session investigation)  
**Target binary:** `C:\Workers Resources Soviet Republic\SOVIET64.exe`  
**Measured binary identity:** SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8`; file size `10,308,608` bytes; PE timestamp `0x6A3EB6AD` (matches documented baseline v1.1.1.9). Preferred Ghidra image base `0x140000000`; ASLR enabled, so all native addresses below are RVAs/preferred VAs, not runtime or file offsets.

## 1. Plain-language executive summary

Clarified priority from the project owner: the first acceptable Almanac release must provide **adjustable tax controls on a global page**. A read-only overlay or City Hall panel can be an interim experiment or development milestone, but it does not satisfy the first acceptable release. The broader economy, interest rates, subsidies, wages, trends, and graphs remain later-release work. The Almanac must still be a consumer/explainer, not a second authority for economic values.

The Almanac is best understood as a **global fiscal policy page first**, with the three existing tax controls migrated from City Hall while preserving their current behavior and saved settings. The mod already has most of the machinery a player would want to inspect: dual-currency wallets and treasury, private enterprises and an investment pool, taxes and rent, a survival basket/CPI path, retail standard-of-living logic, market multipliers, and a monthly reconciliation ledger. What is missing is a **single, player-facing, global place** where these values are explained, compared over time, and connected causally.

The native UI investigation has found a real global sidebar host (`FUN_140317DB0`, RVA `0x317DB0`; preferred VA `0x140317DB0`) and its visibility/selection structures. That makes a future **sidebar-hosted Almanac page** plausible, but it is **not yet proven safe**. The function is 84,735 bytes, its selected-panel pointer is runtime data, and adding a genuine new top-level tab would mean writing into a native `std::vector` whose ownership and capacity are not yet traced. The safest near-term path is therefore:

1. Keep the **City Hall fiscal panel** as the proven interactive control surface while the global page is investigated.
2. Investigate, but do not yet implement, a **sidebar-hosted Almanac page** that appears only on the selected Economy & Trade entry, using the proven row/widget calls with `game` as their first argument.
3. A read-only global overlay may be used as an interim experiment, but it is not the accepted first release.
4. Keep all economic values owned by their responsible subsystems. The Almanac should be a consumer and explainer, never a second authority for prices, taxes, rent, wages, or ledger balances.

The biggest design risk is not drawing the UI. It is **definition drift**: the mod already has two different poverty concepts. City Hall's poverty rate counts wallets below a 7-day survival-bundle floor using real residential rent. Retail standard-of-living uses a 14-day "Low" threshold, promotes citizens by income, and uses a default rent of 0.33 RUB/day. These must be reconciled before the Almanac presents either as "poverty."

## 2. What the mod already provides versus what is missing

### Already provided

- **Money authority:** `currency` owns citizen wallets and treasury; labelled treasury movements and flow observers.
- **Private economy:** `enterprise_base` owns enterprise register, capital, P&L period totals, solvency, monthly economy summary, investment pool, evictions, and residential rent base.
- **Tax policy:** `taxes` owns VAT, profit tax, and flat property tax; rates, enabled flags, schedules, and interactive setters; City Hall macro rows.
- **Prices and basket:** `resource_pricing` owns live resource prices, fallback prices, market multipliers, local scarcity quotes, and the v3 CPI basket.
- **Labor and wages:** `citizen_labor` owns wage hierarchy, sector wages, overrides, and mobility hook status.
- **Retail SoL:** `citizen_retail` owns absolute CPI-based standard-of-living classification and demand scaling.
- **Housing/rent:** rent formula, rent slider, eviction staging, and relocation machinery.
- **Public finance tracking:** `state_ledger` owns monthly flow aggregation, treasury reconciliation, CSV export, and HUD card.
- **Logistics and markets:** private shops, private logistics, DO permissions, dry fuel/cost accounting, and B2B settlement proposal.
- **UI services:** `ui_overlay` (global HUD drawers), `ui_panels` (building-window rows and native widgets), and a mapped global sidebar host.

### Missing for an Almanac

- A **global Almanac surface** that is not tied to one building.
- A **unified trend/history store** with defined sampling boundaries and save/load behavior.
- A **shared indicator API** for poverty strata, enterprise aggregates, and policy state.
- A **causal explanation layer** that connects observed values to supported relationships.
- Native **graph/chart** reuse evidence; no reusable chart widget has been identified.
- Reconciliation between City Hall poverty and retail SoL.
- Tests for definitions, missing services, UI/input, scaling, and history.

## 3. Recommended first version and major alternatives

### Recommended first version: "Almanac v1 — global fiscal policy page"

First release should be a **global fiscal policy page**, not a full economy dashboard. Its essential feature is to let players inspect and adjust the existing tax controls currently associated with City Hall, while preserving their current behavior and saved settings.

Minimum v1 content:

1. VAT rate and enabled state.
2. Corporate profit tax rate and enabled state.
3. Property tax flat amount (or land-value-scaled amount) and enabled state.
4. Plain-language explanation of what each tax does and when it is applied.
5. Current treasury/public-finance context: last closed monthly net, revenue, wages/pensions/welfare, sales, wholesale, taxes, and unexplained divergence where available.
6. Links/pointers to deeper economy pages in later releases.

Migration rule: migrate the controls, do not redefine them. The Almanac should call the same `taxes` service setters/getters and use the same persistence file/version. City Hall controls should be kept temporarily as a compatibility fallback during migration, then removed only after a separate verification pass. Removing them immediately is riskier because older saves, mixed plugin versions, and players' muscle memory may depend on the City Hall surface.

### First-release UI placement

- Preferred: a dedicated Almanac section in the global sidebar.
- Acceptable fallback: an Almanac page inside the existing Economy & Trade section.
- Not acceptable as the final answer: another City Hall building panel, because the request is specifically global.

The global sidebar host is mapped (`FUN_140317DB0`), but a new top-level tab or vector entry is not proven safe. Therefore the first release can be implemented more safely as an Almanac page inside the existing Economy & Trade selected sub-tab, with a future dedicated section only after selection/capacity evidence is obtained. If even that is unsafe, a read-only global overlay is the fallback.

### Major alternatives

| Option | Player experience | Engineering risk | Evidence status | Recommendation |
|---|---|---|---|---|
| A. Read-only global overlay (`ui_overlay`) | Always visible, no native hook, F8 toggle | Low | Proven service | **Recommended for v0** |
| B. City Hall panel rows/widgets (`ui_panels`) | Familiar building popup; interactive taxes already there | Low-medium | Proven widgets in that context | **Recommended for first controls** |
| C. Sidebar-hosted rows on selected Economy & Trade sub-tab | Feels like a true global Almanac | Medium-high | Host and geometry mapped; selection and clipping still unproven | **Recommended investigation target for v1** |
| D. New top-level sidebar tab by writing panel vector | First-class global section | High | 10-slot array and ownership not traced; unsafe without proof | **Do not do without new evidence** |
| E. Native `C3D_GUI` menu | Richest native controls | High | Zero exe imports of `C3D_GUI`; only engine DLL exports | **Stretch goal only** |
| F. Custom chart/overlay graph | Visual trends | Medium | No native chart reuse identified; custom drawing needs its own safety review | **Later, after data model exists** |

### Recommended sequencing

- **Phase 0:** Reconcile poverty and rent definitions; define indicator contract.
- **Phase 1:** Read-only global overlay with 10–15 indicators and explicit unavailable states.
- **Phase 2:** City Hall interactive policy controls (already largely present).
- **Phase 3:** Sidebar-hosted Almanac rows on selected Economy & Trade sub-tab, read-only first.
- **Phase 4:** Trend history and causal explanations.
- **Phase 5:** Optional charts and deeper policy controls, subject to separate approval.

## 4. Prioritized decisions for the game designer

First-release decisions:

1. **Migration safety:** Keep City Hall tax controls as a temporary compatibility fallback during migration, then remove them in a later verified release. Recommended: yes.
2. **First-release scope:** Limit v1 to tax inspection/adjustment plus treasury/public-finance context. Recommended: yes.
3. **Placement:** Prefer dedicated global sidebar section; accept Economy & Trade page if dedicated section is unproven. Recommended: Economy & Trade page for first implementation, dedicated section later.
4. **Control preservation:** Preserve exact tax behavior and `tesmio_taxes.dat` settings. Recommended: yes, no semantic changes.
5. **Unavailable data:** Show "not available" rather than zero. Recommended: yes.

Later-release decisions:

6. **Poverty definition:** Should "poverty" mean City Hall's 7-day wallet floor, retail's 14-day Low stratum, or a single agreed threshold? Recommended: one shared threshold with documented mapping, not two silent constants.
7. **Rent in CPI:** Should citizen standard-of-living use default rent (0.33/day) or real residential rent? Recommended: real rent, but only through a Level-1-safe channel.
8. **Income in poverty:** Should employment status affect poverty? City Hall currently ignores wages; retail promotes by income. Recommended: include income, but expose both "cash-poor" and "income-poor" if they differ.
9. **Population scope:** Should poverty be all wallets (City Hall) or only citizens who shop (retail)? Recommended: all citizen wallets, with coverage stated.
10. **History retention:** How many months/quarters/years of history? Recommended: start with 12 monthly points per indicator, configurable, with explicit new-save behavior.
11. **Chart ambition:** Approve text tables/sparklines first, or require full graphs? Recommended: text and small bars first; native graph investigation is a separate later task.
12. **Performance budget:** Approve monthly aggregation and bounded history, not per-frame economy scans. Recommended: yes.

## 5. Explicit limitations

- No runtime game testing, live diagnostics, builds, deployments, settings changes, or save modifications were performed for this report.
- Native sidebar claims are based on static Ghidra exports and prior investigation docs. They are not runtime-proven.
- No native graph/chart reuse has been established.
- No new shared APIs were implemented.
- Economic "status" labels come from design/investigation docs and code inspection; they are not independently runtime-verified.
- Historical values are not invented. Where no history exists, the report says so.
- The Almanac must not become a second authority for economic values or policy.
- The global sidebar's selected-panel pointer is runtime data; its index-to-name mapping is not statically settled.
- The 10-slot toolbar array must not be widened by guesswork.
- Large-save performance is unmeasured.
- Rail fuel accounting is unvalidated.
- Private logistics payments remain OFF; dry accounting is not live settlement.

## 6. Investigation 1: global sidebar and interaction

### 6.1 What is mapped

- Global sidebar host: `FUN_140317DB0`, RVA `0x317DB0` (preferred VA `0x140317DB0`), size 84,735 bytes.
- Signature: `(void* game)`; `RCX` saved to `R14`; `RBX = game + 0xADB0`, the widget draw context.
- Prologue: 18 clean position-independent bytes: `40 55 41 54 41 55 41 56 41 57 48 8D AC 24 D0 D4 FF FF`; 23 bytes are also safe if stopping before `CALL _alloca`. `/GS` cookie at `+0x9A`.
- Visibility gate: `game + 0xFCE` (verified branch in the function). Sidebar closed should cause immediate return in any hook.
- Selected panel pointer: `game + 0xC650`.
- Panel vector: `game + 0xC608` begin, `game + 0xC610` end, stride `0x68` (104 bytes), entry `+0x00` active flag, `+0x01` NUL-terminated ASCII name.
- Tab state object: 10-slot panel pointer array at `obj + 0x38`; current tab index at `obj + 0x68`.
- Tab bar function: `FUN_1404D72C0`, RVA `0x04D72C0` (preferred VA `0x1404D72C0`).
- Hotkey-to-tab mapper: `FUN_1404D6DF0`, RVA `0x04D6DF0` (preferred VA `0x1404D6DF0`).
- Sub-tab widget: `FUN_14007C400`, RVA `0x7C400` (preferred VA `0x14007C400`), called from the master function.
- Tab-name literals include `importexport`, `trade`, `loanusd`, `loanrub`, `productionconsumption`, and others. `importexport` is unique in the binary and is the likely Economy & Trade identifier.

### 6.2 Feasible host strategies

| Strategy | Feasibility | Assessment |
|---|---|---|
| Dedicated new top-level tab by writing `game+0xC608` vector | Unproven/unsafe | Do not do. Vector owner, capacity, and initialization are not traced. |
| Almanac page inside selected Economy & Trade sub-tab | Plausible | Validate `game+0xC650` lies in `[begin,end)` and its name is `importexport`; then append rows. |
| City Hall building panel | Proven but wrong scope | Good for controls, but not global. |
| Separate overlay | Proven | Safest first release if sidebar placement is deferred. |

### 6.3 Input, scrolling, clipping, resizing, localization, scaling, closing

- The master function uses the same widget draw context as `ui_panels`, so the existing `ui_panels` geometry helpers likely apply.
- Visibility is gated by `game+0xFCE`; a hook must test it because setup runs even when closed.
- Selected panel should be validated against the vector bounds; `game+0xC650` is never observed being nulled.
- Clipping, scrolling, resizing, and exact row capacity inside the global host are **not yet traced**. Treat them as open.
- Localization likely uses `C3D_LANGUAGE::GetString`; the widget calls use wide strings directly for labels, avoiding private text IDs for widget labels.
- DPI scaling uses `0x992088`; multiply pixel values.
- Closing is represented by tab index `-1` in the tab state object, but the hand-off to `game+0xC650` is not fully traced.

### 6.4 Native graphs and charts

Graphs are a later bonus. No reusable native chart/graph function has been identified in the evidence used for this report. The master function uses text printing, drawing, collision, and texture loading, but no graph control with known inputs, ownership, scaling, labels, or lifecycle. A custom chart is possible but must be designed separately; it is not a first-release blocker. Detailed native graph investigation should be a separate later task.

### 6.5 Existing widgets in the global context

- Slider `0x2727F0` (RVA `0x02727F0` (preferred VA `0x1402727F0`)) and labelled checkbox `0x3956F0` (RVA `0x03956F0` (preferred VA `0x1403956F0`)) are verified in panel builders and take `game` as their first argument.
- Because the global host sets `game+0xADB0`, these calls are likely usable there, but this is **not runtime-verified**.
- The labelled checkbox has inverted raw rendering semantics in some contexts: native raw 0 can draw checked and raw 1 unchecked. Logical values must be converted at the caller boundary.
- There is no panel-native textbox; typed entry would require overlay digit capture or a future `C3D_GUI` path.
- All widget commits must be commit-on-change, with no expensive work while the mouse button is held.

### 6.6 Required verification before any hook

- Authenticate the exact PE build and RVAs.
- Verify 18/23-byte prologue bytes and instruction boundaries.
- Check incoming edges and no relative control flow in the stolen span.
- Confirm `game+0xFCE` semantics at runtime.
- Confirm selected panel validation and exact Economy & Trade identifier.
- Confirm widget click handling does not collide with global input.
- Confirm no clipping/scroll corruption.
- Confirm compatibility with other UI hooks.
- Obtain separate approval before any implementation or runtime test.

### 6.7 Recommendation

For the first fiscal release, prefer an **Almanac page inside the existing Economy & Trade selected sub-tab** if it can be done without corrupting native state. If selection validation or clipping cannot be proven, fall back to a read-only global overlay. Do not attempt a new top-level tab yet.

## 7. Investigation 2: economic information inventory

### 7.1 Core public-finance indicators

| Indicator | Plain-language meaning | Source/API/code path | Status | Unit/coverage/update | History | Limitations/API needed |
|---|---|---|---|---|---|---|
| Treasury balance | Money currently held by the state | `currency.getTreasuryBalance(active currency)`; `game+0x580/+0x588` | Implemented | Active currency; whole Republic; live | Current only | Unified currency leg depends on currency config |
| Monthly net treasury change | Whether the Republic gained or lost money last month | `state_ledger` month boundary; `g_flowSum` | Implemented | Active currency; whole Republic; monthly | CSV + HUD snapshot | Depends on flow observers |
| Wages/pensions/welfare | State spending on people | Flow category `wage`, `pension`, `welfare` | Implemented | Active currency; state payments; monthly | CSV + HUD | Private wages are separate |
| State store sales | Money citizens paid to state shops | `state_store_sale` flow | Implemented | Active currency; state retail; monthly | CSV + HUD | Does not include private shop sales |
| Wholesale B2B | Money from private firms buying state stock | `wholesale_b2b` flow | Implemented | Active currency; private-state purchases; monthly | CSV + HUD | B2B direct settlement still draft |
| VAT | Consumption tax collected | `vat` flow | Implemented if taxes installed | Active currency; private retail; monthly/quarterly schedule | CSV + HUD | Depends on tax schedules |
| Profit tax | Tax on private enterprise profit | `profit_tax` flow | Implemented if taxes installed | Active currency; private enterprises; quarterly schedule | CSV + HUD | Profit formula depends on accruals |
| Property tax | Flat/land-scaled tax per enterprise | `property_tax` flow | Implemented if taxes installed | Active currency; enterprises; yearly schedule | CSV + HUD | Flat amount, not a rate |
| Money supply reconciliation | Treasury + wallets + enterprise capital + pool | `state_ledger` open/close totals | Implemented | Active currency; whole tracked money; monthly | CSV | Enterprise capital scan cost; no historical series beyond CSV |
| Unexplained divergence | Treasury movement not explained by labelled flows | `observedDelta - flowDelta` | Implemented diagnostic | Active currency; monthly | CSV | Can be legitimate unlabelled movement |

### 7.2 Fiscal policy controls

| Control | Meaning | Type | Source/API | Persistence | Status |
|---|---|---|---|---|---|
| VAT rate | Fraction of retail sale accrued as VAT | Percentage 0–1 | `taxes.getVatRate/setTaxRate(VAT)` | `tesmio_taxes.dat` v1 | Implemented |
| VAT enabled | Whether VAT is levied | Boolean | `taxes.isTaxEnabled/setTaxEnabled(VAT)` | `tesmio_taxes.dat` v1 | Implemented |
| Profit tax rate | Fraction of net profit taxed | Percentage 0–1 | `taxes.getProfitTaxRate/setTaxRate(PROFIT)` | `tesmio_taxes.dat` v1 | Implemented |
| Profit tax enabled | Whether profit tax is levied | Boolean | `taxes.isTaxEnabled/setTaxEnabled(PROFIT)` | `tesmio_taxes.dat` v1 | Implemented |
| Property tax amount | Flat amount per enterprise, land-value scaled if enabled | Flat active-currency amount >=0 | `taxes.setTaxRate(PROPERTY)` | `tesmio_taxes.dat` v1 | Implemented |
| Property tax enabled | Whether property tax is levied | Boolean | `taxes.isTaxEnabled/setTaxEnabled(PROPERTY)` | `tesmio_taxes.dat` v1 | Implemented |
| VAT schedule | Monthly | Calendar frequency | `registerScheduled(MONTHLY)` | INI only | Implemented |
| Profit schedule | Quarterly | Calendar frequency | `registerScheduled(QUARTERLY)` | INI only | Implemented |
| Property schedule | Yearly | Calendar frequency | `registerScheduled(YEARLY)` | INI only | Implemented |
| Land-value scaling | Property tax scaled by building attractiveness | Boolean + formula | `g_propertyLandValue` | INI only | Implemented |

Important: tax rates are fractions for VAT/profit and a flat amount for property tax. Tax revenue goes to treasury, never to the investment pool. A suspended tax stops new accrual and settlement but holds prior liability.

### 7.3 Private economy indicators

| Indicator | Meaning | Source/API | Status | Unit/coverage/update | History | Limitations |
|---|---|---|---|---|---|---|
| Enterprise count | Number of private enterprises | `enterprise_base.getEnterpriseCount` | Implemented | Count; private register; live | Current only | Order not guaranteed across save/load |
| Enterprise capital | Money held by a firm | `getCompanyCapital` | Implemented | Active currency; per firm; live | Current only | Negative balances allowed |
| Solvency | Whether firm is solvent | `isCompanySolvent` | Implemented | Boolean; per firm; live | Current only | Threshold details in code |
| Markup | Private retail markup multiplier | `getCompanyMarkup` | Implemented | Multiplier; per shop; live | Current only | Player-adjustable |
| Investment pool | Capital available for private construction | `getPrivateInvestmentPool` | Implemented | Active currency; global; live | Current only | Refuses below zero |
| Monthly pool inflow | Pool additions this month | `getMonthlyPoolInflow` | Implemented | Active currency; global; monthly | Current month only | Resets at boundary |
| Monthly private sales | Gross private sales last closed month | `getMonthlyEconomySummary` | Implemented | Active currency; all private firms; monthly | Last closed month only | Zero before first boundary |
| Monthly private wages | Private wages last closed month | `getMonthlyEconomySummary` | Implemented | Active currency; all private firms; monthly | Last closed month only | Zero before first boundary |
| Monthly private taxes | Private taxes last closed month | `getMonthlyEconomySummary` | Implemented | Active currency; all private firms; monthly | Last closed month only | Zero before first boundary |
| Monthly evictions | Successful evictions this month | `getMonthlyEvictionCount` | Implemented | Count; current month | Current month only | Resets at boundary |

### 7.4 People, living standards, and housing indicators

| Indicator | Meaning | Source/API | Status | Unit/coverage/update | History | Limitations |
|---|---|---|---|---|---|---|
| Wallet balance | Citizen money | `currency.getWalletBalance`, `forEachWallet` | Implemented | Active currency; citizens with wallets; live | Current only | ID derived from person fields |
| Total wallet balance | All citizen money combined | `currency.getTotalWalletBalance` | Implemented | Active currency; all wallets; live | Current only | No individual history |
| City Hall poverty rate | Citizens below 7-day survival floor | `taxes.SnapshotMacroStats` | Implemented | Percent of wallets; live-ish 500 ms cache | Current only | Uses real rent; ignores income |
| Retail standard-of-living strata | Destitute/Affluent/Middle/Low classification | `citizen_retail.ApplySolScaling` | Implemented | Count/stratum; shoppers; daily | Current day only | Different threshold and rent |
| CPI daily basket | Cost of survival for one day | `resource_pricing.getCpiDailyBasket` | Implemented | Active currency; per day; market multipliers | Current only | Rent differs by caller |
| Survival basket prices | Food/meat/clothes/rent components | `resource_pricing.getPrice`, market multipliers | Implemented | Active currency per ton/day; live | Current only | Fallbacks when engine price zero |
| Residential rent base | Neutral monthly rent per tenant | `enterprise_base.getResidentialRentMonthly` | Implemented | Active currency per tenant per month | Current only | Before attractiveness/slider multipliers |
| Evictions this month | Successful evictions | `getMonthlyEvictionCount` | Implemented | Count; monthly | Current month only | Resets at boundary |
| Housing burden threshold | Rent > 35% of wallet triggers distress | `g_rentBurdenThreshold` | Implemented | Percent of wallet; monthly rent | Current only | Configurable |
| Destitute bread cost | One day of staple bread | `DestituteBreadCost` | Implemented in retail | Active currency; daily | Current only | Food price × market mult |

### 7.5 Prices, markets, trade, logistics

| Indicator | Meaning | Source/API | Status | Unit/coverage/update | History | Limitations |
|---|---|---|---|---|---|---|
| Resource live price | Engine price of one ton | `resource_pricing.getPrice` | Implemented | Active currency per ton; live | Current only | Fallback if engine price <=0.01 |
| Market multiplier | Republic supply/demand balance | `getMarketMultiplier`, `getMarketBalance` | Implemented | Multiplier 0.35–2.0; per resource; monthly rebalance | Current period only | Accumulators reset at boundary |
| Local scarcity quote | Building-level scarcity/glut | `getLocalScarcityMultiplier`, `getLocalPriceQuote` | Implemented v4 | Multiplier; per building/resource; live | Current only | Neutral deadband |
| Private shop counts | Retail/service type counts | `private_shops` config | Implemented | Count; live config | Current only | No aggregate API |
| Foreign restock ratio | Share of foreign stock target | `g_foreignTargetRatio` | Implemented | Percent; per shop | Current only | Configurable |
| DO target count | Eligible private DO targets | `private_logistics` diagnostics | Implemented diagnostics | Count; road offices; daily reconciliation | Current only | No public API |
| DO fuel estimate | Approximate fuel cost of closed assignments | `private_logistics` cost book | Dry accounting only | Active currency; closed road assignments | Closed records persist | Payments OFF; sampling/tonne assumptions |
| B2B settlement | Direct buyer/seller settlement | Proposal only | Draft RFC | Not implemented | None | No live settlement |
| Local town market scarcity | Municipality-level scarcity pricing | Design doc | Planned | Not implemented | None | Next priority |

### 7.6 Poverty reconciliation: City Hall vs retail SoL

City Hall poverty and retail standard-of-living are **not the same measure** and must not be presented as such.

| Dimension | City Hall poverty display | Retail SoL classification |
|---|---|---|
| Threshold | Wallet below 7-day survival floor | Low if runway below 14 days (and not destitute/affluent/middle) |
| Income | Ignored | Daily wage can promote to Middle at PPR >= 1.0 |
| Savings | Wallet balance only | Wallet balance as runway |
| Basket | CPI with real residential rent | CPI with default rent 0.33/day |
| Population | All wallets via `forEachWallet` | Citizens encountered in shop walk |
| Cadence | 500 ms cache plus day/currency/save epoch | Once per citizen per absolute day |
| Stratum lock | Live wallet changes within cache | Locked for the day |

Definitions:

- City Hall: `povertyFloor = g_povertyRunwayDays (default 7) * cpi`, where `cpi = getCpiDailyBasket(realRent)`.
- Retail: `runway = wallet / cpi`, `ppr = dailyWage / cpi`; classification first-match Destitute, Affluent, Middle, Low.

Consequences:

- A citizen with 7–14 days of wallet runway is Low in retail but not poor in City Hall.
- An employed low-savings citizen can be poor in City Hall but Middle in retail.
- A jobless high-savings citizen can be Low in retail but not poor in City Hall.
- Real rent vs default rent can make the same wallet yield different runway values.

Configuration disagreement: `poverty_runway_days=7` in `plugins/taxes.ini` versus `runway_low_days=14` in `plugins/citizen_retail.ini`. No shared threshold or shared API exists.

### 7.7 Additional disagreements found

1. Retail consumes `citizen_labor` directly despite the documented Level-1 dependency constraint.
2. Retail uses default rent because it cannot safely consume enterprise_base.
3. Republic bank computes economic scale with injected rent 0, while taxes injects real rent.
4. Tax persistence saves rates and enabled flags only; schedules and land-value scaling are INI-only.
5. City Hall poverty is a wallet survey; retail SoL is a demand-scaling classification. They answer different questions.
6. No API exposes retail stratum counts; City Hall recomputes poverty independently.

## 8. Investigation 3: trends and explanations

### 8.1 Proposed information architecture

- **Overview / Government Finances (first release):** treasury, monthly net, revenue/spending, taxes, unexplained divergence, current tax settings.
- **People & Living Standards (later):** wallets, CPI, poverty/strata, rent burden, evictions.
- **Jobs & Wages (later):** sector wages, overrides, mobility hook status, employment effects.
- **Prices & Goods (later):** resource prices, market multipliers, local scarcity, basket.
- **Businesses & Credit (later):** enterprise counts, capital, solvency, pool, markup, loans.
- **Trade & Logistics (later):** B2B, DO targets, fuel estimates, market scarcity.

### 8.2 Sampling and history

- **Fiscal sampling boundary:** month boundary in `state_ledger.OnTick`; already captures treasury, flows, wallets, enterprise capital, pool, and private monthly summary.
- **Tax sampling:** tax settlement boundaries (monthly VAT, quarterly profit, yearly property); rates are read live.
- **History retention proposal:** 12 monthly points per indicator, stored per save, with explicit handling:
  - New save: empty history, clearly marked "no history yet."
  - Load: restore history from the save's Almanac file; validate version/checksum; reject corrupt files without overwriting.
  - Pause/speed: month boundaries are calendar-driven; no per-frame aggregation.
  - Currency change: store currency symbol/tag with each point; do not convert old points silently.
  - Missing/incomplete data: mark "partial" or "unavailable," never zero.
- **Storage:** use `persistence` save/load events and atomic writes; store primitive snapshots and derived indicators, never transient pointers.
- **Performance:** aggregate at month boundaries or on explicit Almanac open; cache results; avoid scanning all enterprises every frame.

### 8.3 Causal-chain examples

Supported by code:

- Higher staple prices → higher CPI basket → lower purchasing-power ratio → more citizens classified Low/Destitute (retail path).
- Real rent above default rent → higher CPI → lower runway for the same wallet.
- Tax suspension → `getVatRate`/`getProfitTaxRate` report 0 → no new accrual; prior liabilities held.
- Property tax with land-value scaling → higher-attractiveness enterprise pays more.
- Private sales/wages/taxes → monthly summary → treasury flow categories → state ledger reconciliation.

Plausible but not yet supported as measured causation:

- A tax rate change caused a specific change in firm solvency or investment pool. The code shows the mechanism, but attributing an outcome requires time-series data not currently retained.
- Higher wages caused higher prices. There is no automatic price-wage feedback loop documented as implemented.
- Fuel cost estimates caused any financial outcome. Payments are OFF.

### 8.4 Displaying unavailable or partial data

- Use labels: "not available," "insufficient history," "disabled," "estimated," "partial."
- Show coverage and update period next to each indicator.
- For dry accounting, label it "estimated, payments disabled."
- For City Hall vs retail poverty, show both definitions until reconciled.

## 9. Investigation 4: implementation plan and validation

### 9.1 Phased plan

- **Phase A — Fiscal Almanac read-only page:** global page showing treasury, monthly flows, tax rates/enabled states, schedules, and explanations. No control changes.
- **Phase B — Fiscal controls migration:** add the three tax controls to the Almanac using existing `taxes` setters; preserve City Hall controls as compatibility fallback.
- **Phase C — Compatibility removal:** after verification, consider removing or de-emphasizing City Hall tax controls. Recommend keeping them until a full release cycle has passed.
- **Phase D — Data and definitions:** reconcile poverty/rent/income thresholds; add shared getters.
- **Phase E — Trend history:** monthly history store and save/load.
- **Phase F — Broader economy:** wages, prices, businesses, credit, trade, logistics.
- **Phase G — Charts:** separate native/custom chart investigation.

### 9.2 Service boundaries and data flow

- `taxes` remains the sole authority for tax rates/enabled flags and settlement.
- `currency` remains the sole authority for wallets/treasury.
- `state_ledger` remains the sole monthly reconciliation authority.
- `enterprise_base` remains the sole authority for enterprises, pool, rent base, evictions.
- `resource_pricing` remains the sole price authority.
- The Almanac consumes these services and renders explanations. It must not duplicate financial authority.
- Missing services must degrade gracefully: hide rows or show "not available."

### 9.3 Sampling, thread safety, memory, performance

- No `std::string`, `std::vector`, `new`, or `malloc` in render/tick loops.
- Use pre-allocated buffers and bounded history arrays.
- Aggregate on month boundaries or when the Almanac is opened, not per frame.
- Take service locks only briefly; never hold a lock while calling native rendering.
- Commit widget changes only on the changed frame.
- Preserve existing hook safety: verify bytes, boundaries, incoming edges, ABI, state/lifetime.

### 9.4 Read-only display first vs later controls

- First release can be read-only or include controls; the owner's priority is controls for taxes.
- Even with controls, keep them commit-on-change and use existing validation.
- Risks of changing tax/interest settings while simulation runs: mid-period rate changes affect future accrual/settlement; prior liabilities are held; property tax land-value scaling changes the amount; persistence only saves rates/enabled flags, not schedules or land-value.
- Interest rates are not yet a supported control in the inspected tax service; do not imply they are implemented. They remain later-release work.

### 9.5 Validation plan

Static investigation:
- Re-verify all RVAs against the measured PE.
- Reconcile poverty definitions and rent.
- Document indicator sources and missing APIs.

Automated tests:
- Tax setter validation ranges and NaN rejection.
- Poverty threshold mapping.
- CPI calculation with injected rent.
- Month-boundary aggregation.
- History save/load validation and corrupt-file handling.
- Missing-service fallback.

Windows build:
- Build all plugins after any future implementation.
- Verify DLL timestamps.

Separate in-game testing (requires approval):
- Open global Almanac and City Hall simultaneously.
- Change VAT/profit/property settings and verify behavior unchanged.
- Switch saves/currencies and verify settings/history.
- Large population and long play sessions.
- Verify no vanilla UI interference.

### 9.6 Dependencies, risks, effort

- Dependencies: `taxes` v2, `currency` v4, `state_ledger`, `ui_panels` or overlay, `persistence`.
- Risks: native hook safety, selected-panel uncertainty, definition drift, performance, mixed plugin versions.
- Relative effort: read-only fiscal page low-medium; controls migration medium; history medium-high; charts separate.

## 10. Claim ledger

| Claim ID | Claim | Raw evidence | Status | Limit |
|---|---|---|---|---|
| C1 | Global sidebar host is `FUN_140317DB0` | `docs/UI investigation.md` §§13.4, 14.1, 15.1; exports | Supported (static) | Not runtime-proven |
| C2 | Prologue has 18 safe PI bytes | Disassembly in UI doc §13.4 | Supported for supplied bytes | Does not prove hook safety alone |
| C3 | Visibility gate is `game+0xFCE` | Disassembly `ui_probe_gate.asm` | Supported (static) | May mean toolbar visible, not exactly sidebar open |
| C4 | Selected panel is `game+0xC650` | Disassembly and vector evidence | Supported (static) | Never observed nulled; runtime validation needed |
| C5 | Panel vector stride is `0x68` | `ui_probe_vec.asm` | Supported (static) | Owner/capacity untraced |
| C6 | Slider/checkbox calls are usable with `game` | `ui_panels` references and widget docs | Supported as call signature | Not runtime-verified in global host |
| C7 | No reusable native chart identified | Absence in reviewed exports/docs | Unresolved | Separate graph task needed |
| C8 | City Hall poverty uses 7-day floor and real rent | `plugins/taxes/taxes.cpp` | Supported by code | Not runtime-tested |
| C9 | Retail SoL uses 14-day Low threshold and default rent | `plugins/citizen_retail/citizen_retail.cpp` | Supported by code | Not runtime-tested |
| C10 | Tax controls are implemented and persisted as rates/enabled flags | `plugins/taxes/taxes.cpp` | Supported by code | Schedules/land-value are INI-only |

## 11. Evidence and artifacts

- `docs/UI investigation.md` §§11–15 (native UI evidence).
- `docs/ghidra-index/` (function entries; no new Ghidra queries were run for this report).
- `docs/WRSRE DESIGN DOC.md` §§4, 6–8, 10, 12.
- `docs/visions/vision-whole-mod.md`.
- `docs/CPI_Survival_Basket_Investigation.md`.
- `docs/Citizen_SoL_Investigation.md`.
- `docs/Labor_and_Salary_Overhaul_Specification.md`.
- `docs/Rent_Housing_Task1_Investigation.md`.
- `docs/Citizen_Housing_Relocation.md`.
- `docs/proposal-b2b-wholesale-counterparty-settlement.md`.
- `docs/private-logistics-runtime/DO_OFFICES.md`.
- `src/*_api.h` and plugin implementations listed in section 7.

No files other than this report were modified. No code, settings, saves, builds, or runtime tests were performed.

## 12. Open questions and approval request

Open questions:

1. Approve fiscal-first Almanac scope.
2. Approve temporary City Hall compatibility fallback during migration.
3. Approve Economy & Trade page placement as first implementation if dedicated sidebar section is unproven.
4. Approve read-only overlay fallback if needed.
5. Approve separate native graph investigation later.

Approval requested before any implementation: yes. This report is investigation/planning only.

## 13. First-release global tax page: detailed findings

### 13.1 Plain-language answer

A safe, first-release global tax page is **not yet implementation-ready**, but it is **plausibly feasible** as an Almanac page inside the existing Economy & Trade section. A dedicated new top-level sidebar section is **not yet safe** because the native vector that holds sections has not been traced for ownership, capacity, and initialization. The existing `ui_panels` tax controls prove that the tax service can be operated from a UI, but they are bound to City Hall and do not by themselves provide a global page.

Poverty reconciliation and a read-only overlay are **not prerequisites** for the first acceptable tax release. They remain later work. The critical path is the Economy & Trade selection, drawing, and input path.

Recommended next technical step: trace and validate the selected Economy & Trade panel path, then design a bounded global hook or page host. This requires the owner's approval before any implementation or runtime test.

### 13.2 How the global sidebar selects a page

- The global sidebar is drawn by `FUN_140317DB0` (RVA `0x317DB0` (preferred VA `0x140317DB0`)).
- It owns a `std::vector`-like panel array at `game+0xC608` (begin) / `game+0xC610` (end), stride `0x68`.
- Each entry has an active flag at `+0x00` and a NUL-terminated ASCII name at `+0x01`.
- The selected entry is stored at `game+0xC650`.
- The tab-bar function `FUN_1404D72C0` (RVA `0x4D72C0` (preferred VA `0x1404D72C0`)) handles tab clicks and calls `FUN_1404D9050` to set the current tab index in a 10-slot tab object.
- The master function later reads `game+0xC650` and draws the selected page.
- The exact hand-off from the tab object/index to `game+0xC650` is **not fully traced**. `FUN_14007C400` (RVA `0x7C400` (preferred VA `0x14007C400`)) is only a candidate: an earlier `FindDisp` hit suggested it might write `game+0xC650`, but the latest disassembly ruled out that reported store. Its full role and conditions remain unresolved.
- Economy & Trade is likely identified by the unique string `importexport` in the selected entry, with `loanusd`/`loanrub` confirming the economy family. The index of that entry is runtime data.

Implication: an Almanac page can be conditionally drawn when `game+0xC650` validates as an `importexport` entry. It should not replace or remove that entry.

### 13.3 Drawing location and behavior

- The master function sets up `game+0xADB0` as the widget draw context, the same context used by `ui_panels`.
- It has a visibility branch on `game+0xFCE`; the function does not early-return, so setup always runs and a hook must test the flag.
- The selected page is drawn after tab/sub-tab handling; row origin and clipping behavior in this global host are **not fully traced**.
- The vector's capacity and owner are **not traced**; writing a new entry is unsafe.
- Closing is represented by tab index `-1` in the tab state object, but its hand-off to `game+0xC650` is unresolved.

### 13.4 Can the slider and checkbox be used globally?

The slider (`0x002727F0`, RVA `0x2727F0` (preferred VA `0x1402727F0`)) and labelled checkbox (`0x003956F0`, RVA `0x3956F0` (preferred VA `0x1403956F0`)) are verified in building-panel builders and take `game` as their first argument. That makes them candidates, not proven global widgets.

Required conditions from the verified code:

- `game` pointer.
- Widget draw context at `game+0xADB0`.
- Hit-test through `C3D_INPUT::GetMouseSolid` and `C3D_PANEL2D::Collision`.
- Click latch `0xA54E91`, consumed per click.
- Live gate `game+0xAD74 == -1`.
- Input/live state `live = window+0x20` in panel context.
- Commit-on-change only.
- Checkbox raw rendering can be inverted: logical state must be converted at the caller boundary.

Unknown in the global host:

- Whether `game+0xAD74` and `window+0x20` have the same meaning and safe values there.
- Whether collision rectangles and clipping are correct.
- Whether global input handling collides with the latch.
- Whether widget calls affect scroll or close behavior.

Conclusion: likely usable, but this must be treated as unproven until a bounded live test or deeper static trace is approved.

### 13.5 Placement comparison

| Option | Feasibility | Assessment |
|---|---|---|
| Dedicated new top-level section by writing panel vector | Unsafe/unproven | Do not do; owner/capacity untraced |
| Almanac page inside Economy & Trade selected sub-tab | Plausible | Preferred if selected-entry validation and drawing can be proven |
| City Hall panel | Proven but wrong scope | Good fallback for controls, not global |
| Read-only overlay | Proven | Useful interim experiment only, not an accepted release |

### 13.6 Candidate hook (investigation only)

- Candidate: post-hook `FUN_140317DB0` (RVA `0x317DB0` (preferred VA `0x140317DB0`)).
- Prologue has 18 clean position-independent bytes; 23 bytes are also safe if stopping before `CALL _alloca`.
- `/GS` cookie is at `+0x9A`, outside the stolen span.
- The function runs every frame while the sidebar is up; any hook must gate on `game+0xFCE` and validate the selected entry.
- This is not permission to implement. Incoming edges, exact function identity across builds, widget input safety, and runtime behavior remain unverified.

## 14. Tax migration and shared API gaps

### 14.1 Current City Hall tax controls

The City Hall fiscal panel already provides:

- VAT slider: 0–50% in 1% steps (`RateToSlider`/`SliderToRate`).
- Profit tax slider: 0–50% in 1% steps.
- Property tax slider: 0–`property_slider_max` (default 500) flat units.
- Enable/disable checkbox for each tax.
- Read-only macro dashboard: cost of living, poverty rate, evictions, commercial solvency.

Behavior:

- Widget commits call `SetTaxRateImpl`/`SetTaxEnabledImpl` immediately.
- `SetTaxRate` validates VAT/profit `[0,1]`, property `>=0`, and rejects NaN.
- A disabled tax reports an effective rate of 0 for VAT/profit, so new accrual stops.
- Prior liabilities accrued before disabling are held and can settle when re-enabled.
- Settlement occurs on monthly/quarterly/yearly boundaries.
- `settleNow` forces immediate settlement of all outstanding accruals.

### 14.2 Shared API gaps

1. **No property-tax getter.** `taxes_api.h` exposes `setTaxRate(PROPERTY)` but no `getPropertyTaxRate`/`getPropertyTaxFlat`. The City Hall UI works around this by snapshotting `g_propertyTaxFlat` directly inside the same plugin, but a global Almanac in another plugin cannot authoritatively read the configured amount.
2. **VAT/profit getters return effective zero when disabled.** `GetVatRateImpl` and `GetProfitTaxRateImpl` return 0 if the tax is disabled. This is correct for accrual callers, but a policy UI needs the configured rate to display and edit.
3. **No policy snapshot struct.** There is no single authoritative read of configured rates, enabled flags, and schedules.
4. **Tax persistence is partial.** `tesmio_taxes.dat` v1 saves rates and enabled flags only. Schedules and land-value scaling come from INI and are not saved.
5. **Property tax is a flat amount, not a rate.** Any UI must distinguish percentage taxes from flat amounts.

Recommended smallest compatible service change:

- Add an append-only policy snapshot/getter to `taxes` v2 or a new v3, following existing struct-size/version conventions.
- Expose configured rates independent of enabled state: for example `getConfiguredVatRate`, `getConfiguredProfitTaxRate`, and `getPropertyTaxFlat`.
- Optionally expose a single `getTaxPolicySnapshot` that returns configured rate, enabled flag, schedule, and land-value setting.
- Preserve existing getters' effective-zero behavior for accrual consumers.
- Do not create a second tax authority or duplicate saved settings.

### 14.3 Migration plan

1. Keep City Hall controls as the authority during migration.
2. Add the same three tax controls to the global Almanac page using the existing `taxes` setters.
3. Both surfaces read from and write to the same tax service; no duplicated state.
4. Preserve `tesmio_taxes.dat` v1; do not change its meaning.
5. Keep City Hall controls temporarily as a compatibility fallback.
6. Synchronize by reading service state on each render and committing only on change.
7. Remove or de-emphasize City Hall controls only after a full release cycle and evidence that the global page works across save/load, currencies, and missing services.

### 14.4 City Hall fallback assessment

Keeping City Hall controls temporarily is the safer approach. It preserves existing player behavior and mixed-version compatibility. The main cost is duplicated UI code, which must be kept thin and call the same service functions. The global page should not maintain its own tax state.

## 15. First-release acceptance checklist

- Global Almanac page is reachable without opening a building.
- Page clearly identifies itself as Almanac, not City Hall.
- VAT, profit tax, and property tax values and enabled states are displayed.
- Values match City Hall values.
- Sliders/checkboxes change the same tax service values.
- Disabled tax shows configured value separately from effective zero where applicable.
- Property tax is shown as a flat amount, not a percentage.
- Save/load preserves rates and enabled flags.
- Missing `taxes` service degrades gracefully.
- No vanilla UI corruption or input deadlock.
- No change to settlement timing or liability handling.

## 16. Future validation plan

- Verify RVAs against the measured PE before implementation.
- Verify hook prologue bytes and boundaries.
- Validate selected-entry detection and Economy & Trade identification.
- Test widget input in the global host.
- Test clipping, scrolling, and close behavior.
- Test with large populations and long sessions.
- Test save/load, currency switching, and backward compatibility.
- Test disabled taxes and held liabilities.
- Test synchronization between City Hall and Almanac.
- Test missing services and mixed plugin versions.
- Test vanilla UI compatibility.

## 17. Ghidra queries actually run in this follow-up

- `DisasmMany 0x14007C3C0:0x14007C400` and `0x14007C3E0:0x14007C430` to inspect the reported `game+0xC650` write site. Result: the reported address is not a store; the displacement hit was a false positive/overlapping instruction. This removes one apparent selection-write lead.
- No decompilation, Xrefs, or ListFuncs queries were run in this follow-up.
- No live game tests, builds, or deployments were run.

## 19. Focused static investigation of global-page blockers

### 19.1 Hand-off from tab selection to selected entry

Supported:

- `FUN_140317DB0` (RVA `0x317DB0`) reads the selected entry from `game+0xC650` and branches on its name.
- At RVA `0x31EACB`, if the selected entry name is "system", it calls `FUN_140337450` (RVA `0x337450`).
- At RVA `0x338770`–`0x33877B`, `FUN_140337450` computes `RCX = [game+0xC608] + RBX` and stores it to `game+0xC650`. This is a confirmed write to the selected-entry pointer, but only in the "system" page's click-handling path.
- `FUN_140337450` also checks the click latch at `0xA54E91`, calls a configurator lookup for "gui_click", and plays a sound. This indicates it is a click handler, not a passive renderer.
- If the selected entry is "storage", the master function takes a different branch and does not call `FUN_140337450`.
- If the selected entry is "infrastructure", it branches to another path.
- Otherwise it calls `FUN_140395C90` (RVA `0x395C90`), size 55,231 bytes, as a fallback page handler.

Unresolved:

- The top-level hand-off from the 10-slot tab object/index to `game+0xC650` is still not fully traced. The tab bar sets the tab object and index; the selected entry is shared state used by many functions.
- `FUN_140337450` is not the general Economy & Trade handler. It is specifically reached when the selected name is "system".
- The RBX offset used in the write is not traced to its source. It is likely a loop index times the vector stride, but that is not yet proven.
- Whether an Almanac page can be represented as a new sub-tab or only as conditional content inside an existing page remains unresolved.

### 19.2 Drawing and input context

Supported:

- The master function sets `RBX = game+0xADB0`, the same widget draw context used by `ui_panels`.
- It has a visibility branch on `game+0xFCE`; setup always runs, so a hook must test the flag itself.
- The selected page is drawn after tab/sub-tab handling; text and widget calls occur in that path.
- The panel vector begin/end and stride are established; the selected entry can be validated against `[begin,end)`.

Unresolved:

- Exact row origin, clipping, scrolling, and close behavior in the global host are not traced.
- The selected entry is shared by many functions; an Almanac page must not disturb their assumptions.
- The vector owner and capacity remain untraced; no write to the vector is safe.
- Whether the global host calls widget functions with the same input/live state as building panels is unproven.

### 19.3 Slider and checkbox requirements

Static evidence:

- Slider `0x2727F0` (RVA `0x2727F0`) and checkbox `0x3956F0` (RVA `0x3956F0`) are verified in building-panel builders.
- Both take `game` as their first argument and use `game+0xADB0` as the draw context.
- Both use `C3D_INPUT::GetMouseSolid` and `C3D_PANEL2D::Collision`.
- The slider uses click latch `0xA54E91`, `game+0xAD74 == -1`, and `live` handling.
- The checkbox uses `live`, hit-test, and can toggle on click.
- The checkbox's raw rendering can be inverted; logical state must be converted at the caller boundary.

Questions requiring a later live test:

- Whether `game+0xAD74` and the panel `live` equivalent have the same meaning in the global host.
- Whether collision rectangles and clipping are valid there.
- Whether global input handling collides with the latch.
- Whether widget calls affect scroll, close, or tab switching.
- Whether the checkbox's inverted raw semantics occur in this context.

### 19.4 Verdict

Supported: the global sidebar host, its visibility branch, panel vector, selected-entry pointer, and widget call signatures are statically established. A conditional Almanac page on a validated Economy & Trade entry is plausible.

Unresolved: top-level selection hand-off, global widget input semantics, clipping/scrolling/close behavior, and vector ownership/capacity.

Smallest next action: obtain approval for a bounded live/UI test or deeper static trace of the selected Economy & Trade path. Do not implement or hook anything until that evidence is available.

## 20. Economy & Trade path and widget assessment

### 20.1 Selection path (superseded by section 24)

Correction: an Economy & Trade/importexport entry does not reach `FUN_140395C90`. The dispatch chain reaches `FUN_140358A20` (RVA `0x358A20`). The list below is retained as historical evidence for `FUN_140395C90` only.

Supported:

- The master function compares the selected entry name in this order:
  1. "system" -> `FUN_140337450` (RVA `0x337450`).
  2. "storage" -> a branch that sets local state and calls RVA `0x33F9D0`.
  3. "infrastructure" -> a branch that calls `FUN_140389F10` (RVA `0x389F10`).
  4. "building" -> a branch that calls `FUN_140388140` (RVA `0x388140`).
  5. "workshopbuildings" -> a branch that calls `FUN_140388140` (RVA `0x388140`).
  6. "editorbuildings" -> a branch that calls `FUN_140384560` (RVA `0x384560`).
  7. "timelapse" -> a branch that calls `FUN_140385210` (RVA `0x385210`).
  8. "notifi_tab" -> a branch that calls `FUN_140383E50` (RVA `0x383E50`).
  9. "objectives" -> a branch that calls `FUN_140387320` (RVA `0x387320`).
  10. "scripts" -> a branch that calls `FUN_140387320` (RVA `0x387320`).
  11. Otherwise -> `FUN_140395C90` (RVA `0x395C90`).

- The strings were verified in `.rdata`: "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "timelapse", "notifi_tab", "objectives", and "scripts".
- "importexport" is not in this explicit list. Therefore an Economy & Trade entry named "importexport" reaches the fallback handler `FUN_140395C90` unless another name or state changes the path.
- This does **not** prove that `FUN_140395C90` is a safe or suitable host. It only identifies the likely static path.

Unresolved:

- Whether "importexport" is the only Economy & Trade entry name, or whether sub-tabs use other names.
- What `FUN_140395C90` does beyond its prologue. Its size is 55,231 bytes and it was not decompiled.
- Whether the fallback path is the correct place for an Almanac page, or whether a separate native page host would be safer.

### 20.2 Content area, clipping, scrolling, and close behavior

Supported:

- The master function sets up `game+0xADB0` as the widget draw context.
- It has a visibility branch on `game+0xFCE`.
- The selected page is drawn after tab and sub-tab handling.
- The panel vector can be validated against `[game+0xC608, game+0xC610)`.

Unresolved:

- Exact row origin, clipping rectangle, scroll state, and close behavior in the global host are not traced.
- The fallback handler's use of the draw context and input state is unknown.
- Whether adding rows or widgets there would obscure vanilla content cannot be determined statically.
- The relationship between tab index `-1` and `game+0xC650` on close is unresolved.

### 20.3 Slider and checkbox assessment

Static evidence:

- The slider (RVA `0x2727F0`) and checkbox (RVA `0x3956F0`) are verified in building-panel builders.
- Both take `game` as their first argument and use `game+0xADB0`.
- Both use `C3D_INPUT::GetMouseSolid` and `C3D_PANEL2D::Collision`.
- The slider uses click latch `0xA54E91`, `game+0xAD74 == -1`, and commit-on-change.
- The checkbox uses hit-test, sound playback, and toggles on click.
- The checkbox's raw rendering can be inverted; logical state must be converted at the caller boundary.

Missing evidence:

- Whether `game+0xAD74` and the panel `live` equivalent have the same meaning in the global host.
- Whether collision rectangles and clipping are valid there.
- Whether global input handling collides with the latch.
- Whether widget calls affect scroll, close, or tab switching.
- Whether the checkbox's inverted raw semantics occur in this context.

A later live test would need to establish at minimum:

- That the slider and checkbox render in the expected location.
- That clicks are recognized and do not steal or duplicate input.
- That tab switching and closing still work.
- That no vanilla content is obscured or corrupted.

### 20.4 Verdict

Correction: an Economy & Trade/importexport entry does not reach `FUN_140395C90`. The corrected handler is `FUN_140358A20` (RVA `0x358A20`). The statement above is retained only as historical evidence for `FUN_140395C90`.

Established: the selected entry name is not one of the explicitly handled names before the final fallback, and `FUN_140358A20` is the likely handler. `FUN_140395C90` can call native widgets in at least one construction/building path, but that path is not the Economy & Trade path.

Unknown: whether `FUN_140358A20` is a safe host, where its content area is, how clipping/scrolling/close work, and whether widgets can receive input there.

Smallest next action: use the corrected diagnostic plan in section 26. No implementation or hook is authorized.

## 21. Economy & Trade host assessment

### 21.1 Path verification (superseded by section 24)

Correction: an Economy & Trade/importexport entry does not reach `FUN_140395C90`. The dispatch chain reaches `FUN_140358A20` (RVA `0x358A20`). The notes below are retained as historical evidence for `FUN_140395C90` only.

Supported:

- In `FUN_140317DB0` (RVA `0x317DB0`), the selected entry name is compared to "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "timelapse", "notifi_tab", "objectives", and "scripts".
- If none of those names match, the function calls `FUN_140395C90` (RVA `0x395C90`).
- The Economy & Trade entry is commonly identified by the unique string "importexport", which is not in that explicit comparison list. Therefore it is likely that an "importexport" entry reaches `FUN_140395C90`.

Not established:

- `FUN_140395C90`'s string table contains construction and tool names, not "importexport", "trade", "population", or "resources". This means either the function handles Economy & Trade through a different sub-field, or it is not a suitable host for that page.
- The function uses `[RSI+0xD428]` and `[selected entry+0x48]`. The meaning of these fields and their values for an Economy & Trade entry are not established by static evidence.
- The function may early-exit or render unrelated content for non-construction entries.

### 21.2 Content area, clipping, scrolling, and close behavior

Supported:

- `FUN_140395C90` calls `PrintLeftUnicode`, `GetMouseSolid`, `Collision`, `Draw`, `SetPosition`, and `SetDimensions`.
- It does not call an obvious BeginDraw/EndDraw or scroll function in the reviewed call list.
- The master function sets up `game+0xADB0` as the draw context and branches on `game+0xFCE`.

Unresolved:

- Exact content rectangle, clipping, and scroll state are not traced.
- Close behavior is not established in this function; it may be handled elsewhere.
- Whether an Almanac page could appear without obscuring vanilla content is not established.

### 21.3 Slider and checkbox coexistence

Static evidence:

- `FUN_140395C90` calls both the slider (RVA `0x2727F0`) and checkbox (RVA `0x3956F0`) at multiple sites.
- The slider call at RVA `0x396AB5` is gated by `[RSI+0x1090] == 0`, `[RSI+0xD428] != 0`, `[[RSI+0xD428]+0x48] != 0`, and building type/subtype conditions (`0x03`, `0x22`, `0x2D`, `0x00` with subtype `0x13`/`0x21`, `0x01` with subtype `0x13`/`0x21`, or `0x17`). These are construction/building conditions, not Economy & Trade/importexport.

Missing evidence:

- Whether those widget calls occur on the Economy & Trade path.
- Whether their input state and collision rectangles are valid there.
- Whether they collide with vanilla controls.
- Whether the checkbox's inverted raw rendering occurs there.

A later live test would need to establish:

- That the page renders in the expected location.
- That sliders and checkboxes are visible and clickable.
- That clicks do not duplicate or steal input.
- That tab switching and closing still work.
- That no vanilla content is obscured.

### 21.4 Verdict

Correction: an Economy & Trade/importexport entry does not reach `FUN_140395C90`. The dispatch chain reaches `FUN_140358A20` (RVA `0x358A20`). The notes above are retained as historical evidence for `FUN_140395C90` only.

Established: `FUN_140395C90` can call native widgets in at least one construction/building path. That evidence does not apply to the Economy & Trade path.

Unknown: whether `FUN_140358A20` is a safe host, where its content area is, how clipping/scrolling/close work, and whether widgets can receive input there.

Smallest next action: use the corrected diagnostic plan in section 26. No implementation or hook is authorized.

## 22. Bounded widget-path evidence

### 22.1 Exact call sites

Slider call site:
- RVA `0x396AB5`: `CALL 0x1402727F0` (FUN_1402727F0).
- Supporting disassembly artifact: `scratch/out/140396a80_140396ae0_disasm.asm`.

Checkbox call sites in the same function include RVA `0x395F7E`, `0x396089`, `0x3961DA`, `0x396337`, `0x3964CC`, `0x396813`, `0x396A11`, and `0x396BCA` (from `scratch/out/140395c90_calls.txt`).

### 22.2 Conditions for the slider call

Exact disassembly from `scratch/out/140396900_140396ac0_disasm.asm`:

```asm
140396909: CMP byte ptr [RSI + 0x1090], R15B
140396910: JNZ 0x140396d2e
140396916: MOV RCX, qword ptr [RSI + 0xd428]
14039691d: TEST RCX, RCX
140396920: JZ 0x140396d2e
140396926: MOV RCX, qword ptr [RCX + 0x48]
14039692a: TEST RCX, RCX
14039692d: JZ 0x140396d2e
140396933: MOV EAX, dword ptr [RCX + 0x360]
140396939: CMP EAX, 0x3
14039693c: JZ 0x14039699e
14039693e: CMP EAX, 0x22
140396941: JZ 0x14039699e
140396943: CMP EAX, 0x2d
140396946: JNZ 0x14039695a
...
14039699e: ...
140396ab5: CALL 0x1402727f0
```

Interpretation: the slider call is reachable only when `[RSI+0x1090] == 0`, `[RSI+0xD428] != 0`, `[[RSI+0xD428]+0x48] != 0`, and the value at `+0x360` is one of the construction/building types listed above. This is not established for an Economy & Trade/importexport entry.

### 22.3 Conclusion

Supported: `FUN_140395C90` can call the native slider and checkbox in at least one construction/building path.

Unresolved: whether those calls are reachable for Economy & Trade, and whether their input/layout state is valid there.

Smallest next action: a separately approved live test or a deeper static trace of `FUN_140395C90`'s use of `[RSI+0xD428]` and `[selected entry+0x48]`. No implementation or hook is authorized.

## 24. Correction: actual Economy & Trade handler

The earlier inference that Economy & Trade reaches `FUN_140395C90` was incomplete. `FUN_140395C90` is reached for "infrastructure" and possibly other names, but the fallback chain continues through "const_offi", "veh_replace", "trade", "mtlist", and "list". An entry named "importexport" does not match any of those names and reaches `FUN_140358A20` (RVA `0x358A20`), size 10,143 bytes.

Consequences:

- `FUN_140395C90` is not the correct handler for an Economy & Trade/importexport entry.
- Its slider and checkbox calls do not establish that widgets are available on the Economy & Trade page.
- `FUN_140358A20`'s CallsIn list does not include `FUN_1402727F0` (slider) or `FUN_1403956F0` (checkbox). It calls `PrintLeftUnicode`, `PrintCenterUnicode`, `Draw`, `SetPosition`, `SetDimensions`, `GetMouseSolid`, and `Collision`, but no native tax slider or checkbox call was found.
- Therefore, a global Almanac page cannot rely on native sliders or checkboxes being present on the Economy & Trade path.
- The content area, clipping, scrolling, close behavior, and input safety of `FUN_140358A20` remain unresolved.

This correction does not change the first-release scope: government finances and tax controls remain the priority. It does mean the UI host question is still open and must not be treated as solved.

### 23.1 Handoff and changed files

This report is the single authoritative investigation artifact. The related native evidence is preserved in `docs/almanac-native-evidence.md` so the deciding disassembly is accessible without relying on ignored scratch files.

Files changed in this investigation:

- `docs/almanac-investigation.md`
- `docs/ghidra-index/007C400.md`
- `docs/ghidra-index/317DB0.md`
- `docs/ghidra-index/337450.md`
- `docs/ghidra-index/395C90.md`
- `docs/almanac-native-evidence.md`

No production code, configuration, saves, builds, hooks, or settings were changed.

### 23.2 Claim review

| Claim | Status | Evidence and limitation |
|---|---|---|
| `FUN_140395C90` can call the native slider and checkbox | Supported | CallsIn list and disassembly show calls at multiple sites, including RVA `0x396AB5` for the slider. |
| The slider call at RVA `0x396AB5` requires `[RSI+0x1090] == 0` | Supported, with register caveat | The disassembly compares to `R15B`; R15 is 0 on the observed master path to `FUN_140395C90`. This is not independently runtime-verified. |
| The slider call requires `[RSI+0xD428] != 0` and `[[RSI+0xD428]+0x48] != 0` | Supported | Direct disassembly. |
| The slider call requires building type/subtype values `0x03`, `0x22`, `0x2D`, `0x00` with subtype `0x13`/`0x21`, `0x01` with subtype `0x13`/`0x21`, or `0x17` | Supported | Direct disassembly. |
| The slider path is not reachable for Economy & Trade/importexport | Unresolved | The gates are construction/building-oriented, but the field values for an Economy & Trade entry are not established. |
| `FUN_140395C90` is a safe host for an Almanac page | Unresolved | No content rectangle, clipping, scrolling, close behavior, or input safety has been established. |
| Native widgets can coexist with vanilla controls in this host | Unresolved | Static signatures match, but live input state and collision behavior are unproven. |

### 23.3 Smallest useful live investigation

Passive observation of the unmodified game can establish a user-facing baseline: which sidebar pages scroll, where content appears, and whether Economy & Trade has visible rows or controls. It cannot establish memory offsets, selected-entry identity, clipping rectangles, hook safety, or whether a diagnostic write would be safe. Therefore passive observation alone cannot answer the technical host question.

A diagnostic plugin or hook could answer more, but it is not harmless and is not implementation-ready. It would require:

- A separately approved, logging-only diagnostic build or runtime patch.
- Verified prologue bytes and instruction boundaries for any hooked function.
- A rollback plan and no persistent game or save changes.
- No change to tax behavior or economic authority.

Comparison of approaches:

1. Passive observation of the unmodified game: least invasive and no code changes. It can document visible layout, scrolling, and tab behavior, but it cannot identify selected-entry pointers, handler addresses, content rectangles, or hook safety.
2. Read-only observation with an existing debugger, if available: more informative without modifying game files, but still live and intrusive. It may be blocked by anti-debugging or game stability concerns, and it requires a separate approval. It can inspect memory and registers but cannot by itself establish safe code modification.
3. Temporary diagnostic plugin or hook: most informative, but highest risk. It should be used only if passive observation and debugger inspection cannot answer the question. It requires safety evidence that is not yet complete.

Recommended least invasive capable approach: begin with passive observation for the UX baseline. If that cannot answer the technical host question, use a separately approved read-only debugger session before considering any hook. A diagnostic hook is not authorized by this plan.

1. Open the global sidebar and select Economy & Trade.
2. Record the selected entry pointer, its name, `[RSI+0xD428]`, and `[[RSI+0xD428]+0x48]`.
3. Record values used for content position, dimensions, clipping, and mouse collision if they can be read without modifying game state.
4. Record whether any native slider or checkbox call occurs on that path. The current static evidence says `FUN_140358A20` has no such calls, so a positive widget observation would contradict the static evidence and require re-analysis.

What it would not do:

- It would not change tax values, settings, saves, or native vectors.
- It would not install a permanent hook or deploy a mod without separate approval.

Risks:

- Any hook can crash the game or corrupt UI state if the prologue or calling convention is wrong.
- Logging in a render path can affect performance.
- A diagnostic build must be clearly separated from release code.

Safeguards:

- Use the already verified 18-byte prologue only as a candidate, with full edge and ABI review first.
- Keep the diagnostic read-only and temporary.
- Test in a disposable session with no save changes.
- Provide an immediate rollback to the unmodified executable.

Undo:

- Remove the diagnostic hook or build and restore the original executable. No save or settings changes should remain.

Expected result:

- If Economy & Trade selects `FUN_140358A20` and exposes usable content coordinates, a bounded Almanac page may be feasible there.
- If it does not, `FUN_140358A20` is not a suitable host for an Almanac page through that path.
- Either result would determine whether to pursue a deeper static trace or a different UI surface.

### 23.4 Recommendation

Do not implement the Almanac yet. The first acceptable release remains a global fiscal page for the existing tax controls, but the Economy & Trade host path is not proven. The smallest useful next step is either a deeper static trace of `FUN_140358A20`'s use of the selected entry and its fields, or a separately approved read-only diagnostic investigation. Passive observation alone cannot resolve the technical blocker.

## 30. Global button-row placement constraints

### 30.1 What is known

- `FUN_140317DB0` (RVA `0x317DB0`) owns the global sidebar and its selected entry.
- `FUN_14007C400` (RVA `0x07C400`) is called after panel dimensions are set and reads/writes `game+0xC650`.
- The global draw context is `game+0xADB0`.
- Native geometry uses DPI scaling from `0x992088`.
- The City Hall TAX button uses content-flow placement, not a proven tab-row dock.

### 30.2 What is not known

- Which structure represents the Economy & Trade button row.
- How many buttons exist and how their positions are calculated.
- Whether the row has spare capacity.
- Whether a new button can be added without overwriting native data.
- How the selected state is represented for each button.
- How close behavior and section switching interact with a mod-owned selection.

### 30.3 Design constraints

- The Almanac button must match the existing buttons in size, spacing, and selected appearance.
- It must not overlap another button or occupy a separate bottom row.
- Its position must be derived from the global window's actual button-row layout, not a fixed screen coordinate.
- The Almanac must open a distinct page, not paint tax controls over the trade page.
- Vanilla buttons and close behavior must remain usable.

### 30.4 If the row cannot accommodate another button

If the row cannot safely accommodate another button, this is a design blocker, not a silent fallback. Alternatives to present to the owner:

- A dedicated top-level Almanac section, if its vector ownership and capacity can be proven.
- A mod-owned button row drawn in the same visual location but with mod-owned hit-testing, if it can coexist safely.
- A separate global page launched from a different safe location, if the button row cannot be extended.

### 30.5 Smallest next step

The smallest next step is a targeted static trace of `FUN_140317DB0`'s button-row drawing and `FUN_14007C400`'s use of the selected entry, followed by a separately approved live test if needed. No implementation, build, hook, configuration change, or live test is authorized.

### 29.1 Design goal

The owner requires a clearly identifiable Almanac button beside the existing Economy & Trade buttons, opening a distinct Almanac page. The recommended first release should not embed tax controls in the import/export page.

### 29.2 Reusable City Hall TAX button precedent (corrected)

The City Hall TAX tile is a useful click-handling precedent, but its current widgetButton implementation is content-flow, not tab-row docking:

- `WidgetButtonImplEx` reads `g_frame.y` (window+0x250, the running bottom edge) and `g_frame.left` for placement. It advances `window+0x250` by two pitches.
- Earlier comments describe tab-row docking, but the live code places the button below native content in the panel's content flow.
- It uses `widgetButton` (ui_panels v3): engine 9-patch chrome from `game+0xB028`, `GetMouseSolid` + `Collision`, click latch `0xA54E91`, `gui_click` audio, hover/active tint, and sink.
- It is INI-tunable for geometry and consumes rows only when a widget is actually drawn.
- It coexists with native buttons by calibrating position, size, and hit-test to the native rect.
- Taxes remains the sole authority; City Hall calls the same tax service.

This precedent supports native-looking button drawing and click handling, but it does not support placing the Almanac button in a top button row without global-window-specific layout evidence.

### 29.3 Mod-owned button approach

A mod-owned button can be drawn in the global host without extending a native vector:

1. Draw the button in `FUN_140317DB0` (RVA `0x317DB0`) after the Economy & Trade row, using the same native draw/hit-test primitives as ui_panels.
2. Keep a mod-owned `almanacOpen` / `selectedPage` state. Do not rely on `game+0xC650` for Almanac selection.
3. When the mod button is clicked, set `almanacOpen = true` and store the previous selected entry so it can be restored.
4. While `almanacOpen` is true, draw the Almanac page instead of, or after, the vanilla page. The exact mechanism must be chosen to avoid obscuring vanilla content.
5. When a vanilla button is selected or the window closes, clear `almanacOpen` and restore the previous selected entry.
6. Reuse the existing tax service getters/setters. City Hall remains a temporary fallback.

### 29.4 Required verification for the global window

- The global draw context `game+0xADB0` must be valid for the button draw and hit-test.
- `GetMouseSolid` and `Collision` must behave correctly in the global host.
- The click latch `0xA54E91` must be available and not collide with vanilla input.
- The window close path must restore state and not leave a dangling selection.
- The button must not obscure vanilla content at different resolutions and DPI scales.
- The button must appear selected and unselected correctly.

### 29.5 Evidence status

Supported:
- Native button drawing and hit-test primitives are well understood in building panels.
- The tax service and City Hall controls are implemented.
- `FUN_140317DB0` is the global sidebar host.

Unresolved:
- Whether the mod-owned button can be drawn safely in the global host.
- Whether the global input state matches the building-panel assumptions.
- Whether the mod can avoid obscuring vanilla content.
- Whether the mod can restore state cleanly on close or section switch.

### 29.6 Smallest next step

The smallest next step is a read-only diagnostic or static trace of `FUN_140317DB0`'s button-row drawing and input handling, followed by a separately approved live test if needed. No implementation, build, hook, configuration change, or live test is authorized.

### 28.1 Exact instruction evidence

Source artifact: `scratch/out/14007c400_140084754_disasm.asm` (lines ~6758–6812).

```
140084307:  33 FF                    XOR EDI,EDI          ; RDI = 0
...
140084367:  MOV qword ptr [R13],RDI
1400843f3:  49 89 BE 50 C6 00 00     MOV qword ptr [R14 + 0xc650],RDI   ; RDI = 0
1400843fa:  41 C7 86 54 AE 00 00 00 00 7A C4 MOV dword ptr [R14 + 0xae54],0xc47a0000
```

- `R14 = game` pointer (set at function entry `0x14007c47e: MOV R14,RCX`).
- `RDI = 0` because `XOR EDI,EDI` at RVA `0x84307` (preferred VA `0x140084307`) zero-extends into RDI and nothing reassigns EDI/RDI before the write.
- Therefore the write stores **NULL (0)** into `game+0xC650` — the selected entry pointer — i.e. it **deselects/clears** the entry. It does not write a new button/entry pointer.
- The same block also writes `[R13] = 0` where `R13 = game + 0xd428`, and sets `[game+0xae54] = 0xc47a0000`.

### 28.2 Conditions (static, unresolved where marked)

Reachable only through this branch chain (all static, not runtime-proven):
1. `[R14+0xd428]` non-null (object pointer at `game+0xd428`).
2. Configuration read at `0x140a54b90` returns non-zero (AL != 0).
3. `R15B != 0` at `0x14008434c`.
4. `[game+0xC650] != 0` (an entry is currently selected).
5. `[R13] != 0` and the object at `game+0xd428` has a 7-character name equal to **"system"** (compare against `0x1408736c4`).
6. Selected entry name (`[game+0xC650] + 1`) matches **"makecity"** (strcmp against `0x14087c048`), **"objectives"** (strcmp against `0x14087bfd8`), or starts with **"qa"** (3-byte compare against `0x140871344`).

If all conditions hold: `[game+0xd428] = 0`, `game+0xC650 = 0`, `[game+0xae54] = 0xc47a0000`, then jump to `0x140084641`.

### 28.3 What this proves and what it does not

- Proven: `FUN_14007C400` (RVA `0x07C400`) is called once by the sidebar host `FUN_140317DB0` at RVA `0x31BB22` during drawing, after setting panel dimensions. It reads and conditionally clears `game+0xC650` under specific entry-name conditions.
- Proven: the reported write at RVA `0x0843F3` writes NULL, not a new entry pointer, and its conditions reference "system"/"makecity"/"objectives"/"qa" — not Economy & Trade sub-tabs ("trade", "loanusd", "loanrub", "productionconsumption", "importexport").
- Not proven: how the Economy & Trade button row is created, how many buttons exist, how a click maps to a selected entry, whether `game+0xC650`/`game+0xC608` vector has spare capacity, whether a new Almanac button/entry can be added safely.
- Not proven: `FUN_14007C400` iterates buttons or handles click→selection for Economy & Trade; its only confirmed caller is the master sidebar draw function.

### 28.4 Missing evidence

Smallest missing evidence:
- The structure/array behind `game+0xC608` (panel/vector begin) and `game+0xC610` (end): element size, owner, capacity, initialization, and whether an Economy & Trade button entry occupies a slot there.
- The click handler: which code reads mouse input over the button row and writes a chosen entry pointer to `game+0xC650` (or sub-tab index at `obj+0x68`).
- Whether any spare slot exists for an additional Almanac entry without writing past the verified `0x68`-stride vector bounds.

Until these are traced, adding a button "alongside the Economy & Trade buttons" cannot be established safe. A viable alternative that still gives the Almanac its own global page is conditional content inside the selected Economy & Trade/"importexport" page (handled by `FUN_140358A20`, RVA `0x358A20`), which requires no new vector entry — but its content area, clipping, scrolling, close behavior, and input state are also unresolved (see section 24/25).


## 29. Focused check: button-row selected-entry write

### 29.1 Exact evidence

Source artifact: `scratch/out/14007c400_140084754_disasm.asm`.

The write at RVA `0x0843F3` is:

```
140084307: 33 FF        XOR EDI,EDI                 ; RDI = 0
...
140084367: MOV qword ptr [R13], RDI                ; [game+0xd428] = 0
1400843f3: MOV qword ptr [R14 + 0xc650], RDI       ; [game+0xC650] = 0 (RDI = 0)
1400843fa: MOV dword ptr [R14 + 0xae54], 0xc47a0000
```

`R14 = game` (set at `0x14007c47e: MOV R14,RCX`). The write therefore clears the selected entry pointer; it does not store a new button or entry pointer.

### 29.2 Conditions

Reachable only when:
1. `[game+0xd428] != 0`.
2. A configuration check at `0x140a54b90` returns non-zero.
3. `R15B != 0`.
4. `[game+0xC650] != 0`.
5. `[game+0xd428]` points to an object whose name is "system".
6. The selected entry name matches "makecity", "objectives", or starts with "qa".

### 29.3 Conclusion

- Supported: the write clears `game+0xC650` under narrow "system"/"makecity"/"objectives"/"qa" conditions.
- Not supported: any connection to Economy & Trade button selection, button count, layout, capacity, or click handling.
- Missing evidence: the structure behind `game+0xC608`/`game+0xC610`, how a button click maps to a selected entry, and whether a spare slot exists for an additional Almanac button.

### 27.1 Visual context

No screenshot artifact was found in the workspace or recent picture directories. The following is based on native code and existing UI evidence, not on the screenshot.

### 27.2 What the existing evidence shows

Supported:

- `FUN_140317DB0` (RVA `0x317DB0`) owns the global sidebar and its selected entry.
- The selected entry name determines which page handler runs.
- Known selected-entry names include "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "const_offi", "veh_replace", "trade", "mtlist", "list", and likely "importexport".
- "trade" has its own handler, `FUN_140373EA0` (RVA `0x373EA0`).
- An "importexport" entry is expected to reach `FUN_140358A20` (RVA `0x358A20`).
- `FUN_14007C400` (RVA `0x07C400`) is called by `FUN_140317DB0` at RVA `0x31BB22` after setting panel dimensions. It is 33,620 bytes and reads `game+0xC650`. It can write `game+0xC650` at RVA `0x843F3`.
- `FUN_14007C400` calls `BeginDraw`, `EndDraw`, `Draw`, `SetPosition`, `SetDimensions`, `PrintLeftUnicode`, `GetMouseSolid`, and `Collision`, plus native slider and checkbox functions. It is therefore a likely sub-tab/button-row renderer, but its exact button count and layout are not established.

Unresolved:

- Whether the visible Economy & Trade buttons correspond exactly to selected-entry names.
- Whether a new button can be added without modifying the native panel vector or tab object.
- Whether the button row has spare capacity, clipping, or input handling for an additional button.
- Whether adding a button would require a new selected entry name and corresponding handler.
- Whether `FUN_140358A20` can host a separate Almanac page without covering vanilla content.

### 27.3 Assessment of the fallback

A new Almanac button alongside the existing Economy & Trade buttons is preferred by the owner if a dedicated top-level section is unsafe. Static evidence does not yet establish that this is safe.

Possible approaches, in increasing risk:

1. Conditional content inside the existing "importexport" page:
   - Lowest additional UI risk.
   - Does not add a button or vector entry.
   - Requires proving the content area and input state.
2. New button with a new selected-entry name and handler:
   - Matches the owner's preferred fallback.
   - Requires a new selected-entry name, handler, and possibly vector or tab-object changes.
   - Not established as safe.
3. Dedicated top-level section:
   - Still preferred if proven safe.
   - Requires vector ownership and capacity evidence that is not yet available.

### 27.4 What must be verified before this fallback

- Which native structure represents the Economy & Trade button row.
- How button clicks map to selected-entry names.
- Whether button capacity is fixed or dynamic.
- Whether a new button would require a new handler or can reuse `FUN_140358A20`.
- Whether content clipping and input remain valid with an additional page.

### 27.5 Diagnostic implication

The diagnostic plan should first identify the Economy & Trade button handler and selected-entry mapping. If that cannot be established statically, the live diagnostic should observe which handler runs for each button and whether a new button name can be represented safely.

No implementation, build, hook, or live test is authorized.

### 26.1 Preserved deciding evidence

The deciding evidence is preserved in `docs/almanac-native-evidence.md`, section "Dispatch evidence for the corrected Economy & Trade path." It includes the exact master-dispatch disassembly and the strings used for comparison.

What the evidence establishes:
- `FUN_140317DB0` compares the selected entry name to "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "const_offi", "veh_replace", "mtlist", "trade", and "list".
- If none match, it calls `FUN_140358A20` at RVA `0x358A20`.
- "importexport" is not in the explicit comparison list. Therefore an Economy & Trade/importexport entry is expected to reach `FUN_140358A20`.

What is missing:
- The runtime value of the selected entry when Economy & Trade is selected.
- The values of `[game+0xD428]` and `[[game+0xD428]+0x48]` for that entry.
- The content rectangle, clipping, scrolling, close behavior, and collision data for `FUN_140358A20`.
- Whether `FUN_140358A20` is a safe host.

No reconstruction should be treated as proof.

### 26.2 Debugger method

A debugger could identify the selected page without modifying game files:

1. Launch the game under the debugger or attach to it.
2. Set a breakpoint at RVA `0x31EE76` (the `CALL 0x140358A20` instruction in `FUN_140317DB0`), or at the entry of `FUN_140358A20` (RVA `0x358A20`).
3. Open the global sidebar and select Economy & Trade.
4. When the breakpoint hits, inspect:
   - `R14` / `RCX`: game pointer.
   - `RBX`: selected entry pointer plus one (name pointer).
   - `game+0xC650`: selected entry pointer.
   - `game+0xD428`: page-related pointer.
   - `[[game+0xD428]+0x48]`: sub-field used by `FUN_140395C90`-style gates.
   - Any coordinates or dimensions used by `FUN_140358A20`.

Breakpoint behavior:
- A software breakpoint temporarily replaces an instruction byte with `INT3` in memory. It does not change the executable file, but it does alter memory and may be detected by anti-debugging or stability checks.
- A hardware breakpoint uses CPU debug registers and does not modify code, but it is more limited and may also be detected.
- Either method pauses the game while the debugger is stopped.
- This is not non-invasive merely because it is read-only.

### 26.3 Question and result interpretation

Question: Does an Economy & Trade/importexport entry select `FUN_140358A20`, and does that handler expose usable content coordinates and input state for a small Almanac page?

Positive result:
- The handler is `FUN_140358A20`.
- The handler exposes a content rectangle and collision data.
- This would make a bounded Almanac page plausible on that path.

Negative result:
- The handler is not `FUN_140358A20`, or it lacks usable content coordinates or input state.
- This would rule out that host path.

Inconclusive result:
- The observed values do not clearly identify the handler or its layout.
- This would require a deeper static trace or a more targeted diagnostic.

This would not prove:
- That drawing an Almanac page there is safe.
- That clicks can be accepted without interfering with vanilla UI.
- That the same behavior holds across saves, currencies, or game versions.

### 26.4 Safeguards and rollback

Safeguards:
- Use a disposable save or session.
- Do not change tax values, settings, or saves.
- Prefer a hardware breakpoint if available.
- If a software breakpoint is used, remove it immediately after observation.
- Record only memory values and layout observations; do not patch code.
- Keep the debugger session short.

Game autosaves:
- Disable autosaves or use a disposable save before testing.
- Do not use the primary save.

Debugger attachment:
- Attaching a debugger may pause the game and may trigger anti-debugging or instability.
- No debugger attachment is authorized by this plan.

Logs or files:
- Passive phase: none.
- Debugger phase: only debugger notes or screenshots, if created manually.
- No game files, saves, or settings should be modified.

Rollback:
- Detach the debugger.
- Remove any software breakpoint.
- Restart the game if behavior is altered.
- No executable, configuration, or save rollback should be needed if the debugger was used strictly read-only.

### 26.5 Static evidence still required before any hook

- Verify exact prologue bytes and instruction boundaries for any candidate hook.
- Trace incoming edges and calling convention.
- Establish state and lifetime for the draw context and selected entry.
- Confirm compatibility with existing UI hooks.
- Resolve `FUN_140358A20`'s use of the selected entry and its fields.

No hook, build, or live test is authorized by this plan.

### 25.1 Evidence corrections applied

- R15 is 0 on the observed master path to the fallback handlers. The master function executes `XOR R15D, R15D` at RVA `0x31EAD5` before dispatch. This is static evidence, not a runtime measurement.
- The slider-path gates in `FUN_140395C90` are construction/building-oriented. They do not establish availability on the Economy & Trade path.
- `FUN_140395C90` is not the handler for an Economy & Trade/importexport entry. The dispatch chain reaches `FUN_140358A20` for that name.
- `FUN_140358A20` has no native slider or checkbox calls in its reviewed call list. This removes the widget-reuse claim for the Economy & Trade path.
- The suitability of the entire Economy & Trade page remains unresolved. The slider-path evidence only resolves one narrow path.

### 25.2 Comparison of approaches

1. Passive observation of the unmodified game:
   - Can establish: visible tab names, whether pages scroll, approximate content placement, and whether widgets are visible.
   - Cannot establish: selected-entry pointers, handler identity, content rectangles, clipping, collision, or hook safety.
   - Changes: none.
2. Read-only observation with an existing debugger:
   - Can establish: selected entry, handler identity, field values, and possibly content coordinates without modifying game files.
   - Cannot establish: safe code modification or long-term compatibility.
   - Changes: none to game files if used read-only, but debugger attachment is intrusive and may affect stability.
3. Temporary diagnostic plugin or hook:
   - Can establish: runtime field values, handler identity, layout coordinates, input state, and widget reachability.
   - Cannot establish: that modifying the host is safe, or that the same behavior holds across saves, currencies, or versions.
   - Changes: runtime code or a temporary build; requires separate approval and rollback.

### 25.3 Recommended approach

Recommended sequence:

1. Passive observation first, to establish the UX baseline.
2. If passive observation cannot answer the host question, use a separately approved read-only debugger session.
3. Use a temporary diagnostic plugin or hook only if the debugger cannot obtain the needed values.

This is the least invasive sequence capable of progressing. Passive observation alone cannot resolve the technical host question.

### 25.4 Exact diagnostic question

Does an Economy & Trade/importexport entry select `FUN_140358A20`, and if so, what are its content rectangle, clipping, scrolling, close behavior, and input state? Can a small Almanac page be drawn there without obscuring vanilla content or stealing input?

Observations needed:

- Selected entry pointer and name.
- Handler address reached.
- Values of `[game+0xD428]` and `[[game+0xD428]+0x48]`.
- Content position and dimensions.
- Collision rectangle used by `GetMouseSolid` and `C3D_PANEL2D::Collision`.
- Whether scroll state exists.
- Whether any native widget call occurs on that path.
- Whether tab switching and closing still work after adding only observation.

### 25.5 Plain-language tester steps

1. Start the game normally with no mod changes.
2. Open the global sidebar.
3. Select Economy & Trade.
4. Observe and record visible rows, scrollbars, clipping, and content boundaries.
5. Switch between Economy & Trade sub-tabs if present.
6. Close and reopen the sidebar.
7. If approved later, attach a debugger read-only and inspect the selected entry and handler fields without changing them.

### 25.6 Tools, permissions, and changes

- Passive phase: no tools, code, build, hook, configuration, or save changes.
- Debugger phase: requires an existing debugger, separate approval, and a disposable session.
- Diagnostic hook phase: requires verified prologue bytes, ABI and incoming-edge review, a temporary build, and separate approval.

Files or game state that could change:

- Passive phase: none.
- Debugger phase: none if used strictly read-only.
- Diagnostic phase: temporary runtime patch or build, log files, and possibly a disposable save if the tester chooses to test save/load.

### 25.7 Risks and safeguards

- A wrong assumption about the handler can make the diagnostic target the wrong code path.
- Any hook can crash the game or corrupt UI state.
- Logging in a render path can affect performance.
- Use a disposable save or session.
- Keep the diagnostic read-only.
- Do not change tax values, settings, or saves.

### 25.8 Rollback

- Passive phase: no rollback needed.
- Debugger phase: detach the debugger; no game files should change.
- Diagnostic phase: remove the temporary hook or build and restore the original executable. No save or settings changes should remain.

### 25.9 Result interpretation

- Positive: Economy & Trade selects `FUN_140358A20` and exposes usable content coordinates and collision data. This would make a bounded Almanac page plausible, but would not prove hook safety or compatibility.
- Negative: the handler lacks usable content coordinates or input state. This would rule out that host path.
- Inconclusive: the observed values do not clearly answer the question. This would require a deeper static trace or a more targeted diagnostic.

This would not establish that implementing an Almanac page is safe, nor that native widgets can be reused there.

### 25.10 Static evidence still required before any hook

- Verify the exact prologue bytes and instruction boundaries for any candidate hook.
- Trace incoming edges and calling convention.
- Establish state and lifetime for the draw context and selected entry.
- Confirm compatibility with existing UI hooks.
- Resolve `FUN_140358A20`'s use of the selected entry and its fields.

No hook, build, or live test is authorized by this plan.

## 34. Resolution of the FUN_1404D72C0 link

## 34. Resolution of the FUN_1404D72C0 link

### 34.1 Outcome: supported match for the top-level section button row

Existing exports support that `FUN_1404D72C0` (RVA `0x4D72C0`) draws the top-level section button row used by the global sidebar. It is not the City Hall TAX tile. It is not established as an in-page Economy & Trade sub-tab row, and the exact four visible buttons remain a runtime/visual mapping.

Deciding evidence:
- Caller: `FUN_140317DB0` calls it at RVA `0x319E42` with `RCX = [game+0xD408]`.
- It loops over `min(count+1, 10)` tabs from the array at `param_1+0x20`.
- Each tab uses a panel pointer from that array.
- Its texture comes from `panel+0x58` or is created from the string at `panel+0xB4`.
- Tab X is derived from `param_1+0x74 + index * pitch`, where pitch is derived from the panel count and DPI.
- It uses `GetMouseSolid` + `Collision` and the latch `0xA54E91`.
- Clicking sets `param_1+0x40`, `param_1+0x68`, and `param_1+1`.
- Selected state is tracked in `param_1+0x40`, `param_1+0x68`, `param_1+1`, and `DAT_1409E2338`.
- The master function's selected entry names include `system`, `storage`, `infrastructure`, `building`, `workshopbuildings`, `editorbuildings`, `trade`, `population`, `objectives`, and `scripts`, matching a top-level section row.
- The master tab-name loop at RVA `0x14031FF40` formats `maintab_<name>` from the same class of panel entries and highlights the selected panel.

The screenshot's four visible buttons may be a clipped subset of this row. A visual mapping such as Trade/Population/Objectives/Scripts is a hypothesis only and is not confirmed by the available exports.

### 34.2 Smallest next evidence

The smallest next evidence is a runtime mapping from the panel array at `game+0xD408` to visible button identities and their on-screen rectangle. No implementation, build, hook, configuration change, or live test is authorized.

### 34.2 Smallest missing evidence

The smallest missing evidence is a static or runtime mapping from the panel array at `game+0xD408` to visible button identities and their on-screen rectangle, including the initializer for `game+0xD408`. The deciding evidence should be preserved in an accessible artifact. No implementation, build, hook, configuration change, or live test is authorized.

### 33.1 What the existing evidence supports

- `FUN_1404D72C0` (RVA `0x4D72C0`) is called by `FUN_140317DB0` at RVA `0x319E42` with `RCX = [game+0xD408]`.
- It loops over `min(count+1, 10)` tabs from the array at `param_1+0x20`.
- Each tab uses a panel pointer and draws a texture from `panel+0x58` or from the string at `panel+0xB4`.
- It handles clicks with `GetMouseSolid` + `Collision` and the latch `0xA54E91`.
- It updates selected state in `param_1+0x40`, `param_1+0x68`, `param_1+1`, and `DAT_1409E2338`.

### 33.2 What is not established

- Whether this function draws the row shown directly below the Economy & Trade title.
- Whether its tabs are the four visible section buttons.
- Whether the UI investigation's "bottom toolbar" label applies to the screenshot row.
- Whether a mod-owned button can be added without overlap or capacity problems.

### 33.3 Conclusion

`FUN_1404D72C0` is a candidate section-button row, not a proven match for the screenshot row. Sections 31 and 32 are now aligned: the row's native owner, layout, identities, and click mapping remain unresolved.

### 33.4 Smallest missing evidence

The smallest missing evidence is a static or runtime mapping from the panel array at `game+0xD408` to the visible button identities and their on-screen rectangle. No implementation, build, hook, or live test is authorized.

### 32.1 Evidence

`FUN_1404D72C0` (RVA `0x4D72C0`, size 7,553) is a candidate tab bar for the global sidebar. It is called by `FUN_140317DB0` at RVA `0x319E42` with `RCX = [game+0xD408]`. The UI investigation labels it a bottom toolbar tab bar; whether it is the same row shown in the Economy & Trade screenshot is unresolved.

Supported:
- It loops over `min(count+1, 10)` tabs, where `count` comes from `param_1+0x38` at offset `+0x50`.
- Each tab uses a panel pointer from the array at `param_1+0x20`.
- Its texture comes from `panel+0x58` or is created from the string at `panel+0xB4`.
- Tab X is derived from `param_1+0x74 + index * pitch`, where pitch is derived from the panel count and DPI.
- It uses `GetMouseSolid` + `Collision` and the latch `0xA54E91`.
- Clicking a tab sets `param_1+0x40` to the selected panel, `param_1+0x68` to the selected index, and `param_1+1` to 1.
- Selected appearance and state are tracked in `param_1+0x40`, `param_1+0x68`, `param_1+1`, and `DAT_1409E2338`.

Not established:
- Whether this tab bar is the row directly below the Economy & Trade title.
- Which panel names or textures correspond to the four visible buttons.
- Whether the row's geometry matches the screenshot.
- Whether a mod-owned button can be added safely.

### 32.2 Mod-owned button assessment

A mod-owned Almanac button could be drawn beside the last tab without adding a native vector entry, provided the row has enough width. However, this is not proven safe.

Required verification:
- The actual row width and available space at the target resolution and DPI.
- The tab pitch and tab size.
- Whether the mod-owned button's hit rectangle collides with vanilla tabs.
- Whether the mod can track selected state without changing `game+0xC650`.
- Whether the mod can show a distinct Almanac page while the vanilla page is not drawn.
- Whether close and section switching restore state without overriding a newer player choice.

### 32.3 Page replacement

The game's selected page is driven by `game+0xC650`. A mod-owned Almanac button would need a mod-owned `almanacSelected` flag. While that flag is set, the mod would need to prevent the vanilla page from drawing or replace it with the Almanac page, then restore the previous selected entry when the user selects a vanilla button or closes the window. This requires a hook or equivalent interception and is not established as safe.

### 32.4 Smallest next step

The smallest next step is a targeted static trace of the tab bar's geometry and click handling, or a separately approved live diagnostic. No implementation, build, hook, configuration change, or live test is authorized.

### 31.1 Evidence status

Supported:
- `FUN_14007C400` (RVA `0x07C400`) is called once by `FUN_140317DB0` at RVA `0x31BB22`, after panel dimensions are set. It reads and conditionally clears `game+0xC650`.
- It calls `BeginDraw`, `EndDraw`, `Draw`, `SetPosition`, `SetDimensions`, `PrintLeftUnicode`, `GetMouseSolid`, and `Collision`.
- It calls native slider and checkbox functions, but its string comparisons are construction-oriented ("system", "makecity", "objectives", "qa"). No Economy & Trade sub-tab strings were found in its string table.
- The City Hall TAX button's current `widgetButton` implementation is content-flow: it uses `g_frame.y` (`window+0x250`) and `g_frame.left`, then advances `window+0x250`.
- `FUN_1404D72C0` (RVA `0x4D72C0`) is a candidate tab bar for the same sidebar. It is called by `FUN_140317DB0` at RVA `0x319E42` and draws tabs from `param_1+0x20`, but it is not yet proven to be the row shown in the screenshot.

Unresolved:
- Whether `FUN_1404D72C0` is the row directly below the Economy & Trade title.
- Which panel names or textures correspond to the four visible buttons.
- How button positions, sizes, spacing, scaling, and selected state are calculated.
- How a click maps to a page and how close/section-switch behavior works.
- Whether a mod-owned button can be inserted without overlap or obscuring vanilla content.
- Whether `FUN_140358A20` can host a distinct Almanac page while preserving vanilla pages.

### 31.2 Design assessment

A mod-owned Almanac button is a candidate design, not a proven implementation.

Required behavior:
- The button must match the existing buttons in size, spacing, and selected appearance.
- It must not overlap another button or occupy a separate bottom row.
- Its position must be derived from the global window's actual button-row layout, not a fixed screen coordinate.
- Clicking it must open a distinct Almanac page.
- Selecting a vanilla button must return to that vanilla page.
- Closing the window must not retain an unsafe pointer or override a newer player selection.

Candidate approaches:
1. Mod-owned button in the same row:
   - Preserves all vanilla buttons and their behavior.
   - Keeps mod-owned selection state.
   - Requires proving the button-row layout, input handling, and non-overlap.
2. Conditional content in the selected Economy & Trade page:
   - Not acceptable as the first release per the owner's design decision.
3. Dedicated top-level section:
   - Still preferred if safe, but its vector ownership and capacity are unproven.

### 31.3 If the row cannot safely accommodate another button

This is a design blocker, not a silent fallback. The alternatives are:
- A dedicated top-level Almanac section, if its native vector ownership and capacity can be proven.
- A mod-owned button row drawn in the same visual location, if it can coexist safely with vanilla drawing and input.
- A separately approved diagnostic to establish the button-row layout and input model.

The button must not be moved to a bottom row or embedded in the trade page to satisfy this requirement.

### 31.4 Smallest next step

The smallest next step is a targeted static trace of the Economy & Trade button-row drawing and click handling, or a separately approved live diagnostic if static evidence cannot establish the layout. No implementation, build, hook, configuration change, or live test is authorized.

## 35. Economy & Trade section-button row: identified (2026-11-07 refresh)

> SUPERSEDED (2026-11-07 later pass): The handler identified here as drawing the row is FUN_140358A20, reached only for panel names not in the explicit dispatch list. The ACTUAL handler for the Economy & Trade row (panel name "trade") is FUN_140373EA0 (RVA 0x373EA0). Sections 35-37 geometry (92x92 backdrop, 85 pitch, 7px overlap, wrap-at-8) applies only to the unreachable FUN_140358A20 fallback. See sections 38-40 for the corrected analysis. Underlying evidence (dispatch, texture names, sub-entry vector) is preserved.

### 35.1 Plain-language verdict

The row of section buttons directly below the Economy & Trade window title is drawn by **`FUN_140358A20` (RVA `0x358A20`)**, the Economy & Trade page handler. Its first loop iterates a vector of sub-entries attached to the selected sidebar panel (`game+0xC650` → `+0x48` begin / `+0x50` end, stride `0x50` = 80 bytes). Each sub-entry contributes one button: its text ID at `+0x40`, a drawable object pointer at `+0x48`, and its texture (resolved from that drawable or created from a name string). The binary contains the corresponding texture names `editor/trade_importexport.png`, `editor/trade_productionconsumption.png`, `editor/trade_tourism.png`, and `editor/trade_crime.png`.

This is **not** `FUN_1404D72C0` (RVA `0x4D72C0`), which is the top-level sidebar tab bar drawn by `FUN_140317DB0` at RVA `0x319E42` from `game+0xD408`; it is a different collection and a different visual row. It is also **not** `FUN_14007C400` (RVA `0x7C400`), which is the sub-tab/button-row widget whose only confirmed write to `game+0xC650` clears the selected entry under narrow "system"/"makecity"/"objectives"/"qa" conditions and is unrelated to Economy & Trade selection. It is **not** `FUN_140395C90` (RVA `0x395C90`), which is not reached for an "importexport" entry at all.

Button click → selection: clicking a sub-tab button sets `game+0xC65C` to its index, plays the `gui_subtab_click` sound, and updates `game+0xCCC8 = -1`, `game+0xCCD0 = 0`, `game+0xC6B8 = game+0xC6B0`. The main content area then maps the selected index `0..4` to values `1 / 4 / 6 / 8 / 10` and drives `FUN_140496DE0` / `FUN_140494F60`, switching the displayed sub-page. Thus each section button opens a distinct sub-page within Economy & Trade.

Geometry (from the decompiled handler): horizontal pitch `fVar41 = DPI * DAT_0x14090ab14`; Y origin `fVar38 = DPI * DAT_0x14090aa0c + (game+0xAE50)`; buttons wrap to a new row every 8. The exact pixel width/height of an individual button and the content-area rectangle are **not** yet extracted from the decompile and remain the smallest missing geometry evidence.

### 35.2 Evidence linking the row to Economy & Trade, not another toolbar

1. Dispatch (master function `FUN_140317DB0`, RVA `0x317DB0`): the selected entry name is compared against "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "const_offi", "veh_replace", "trade", "mtlist", "list"; if none match, it calls `FUN_140358A20` at RVA `0x31EE76`. "importexport" is not in that list, so an Economy & Trade entry reaches `FUN_140358A20`. Preserved disassembly: `docs/almanac-native-evidence.md` ("Dispatch evidence for the corrected Economy & Trade path") and `scratch/out/14031eb5f_14031ec80_disasm.asm` etc.
2. Texture names present in the binary (file offsets `0x899E07`, `0x899E27`): `editor/trade_importexport.png`, `editor/trade_productionconsumption.png`, `editor/trade_tourism.png`, `editor/trade_crime.png` — the four section icons. These are the same names referenced by the UI investigation; they are section icons, not the top-level sidebar tabs.
3. `FUN_140358A20` sub-tab loop: reads `*(game+0xC650)+0x48` (vector begin) and `+0x50` (end), stride `0x50`; per sub-entry text ID at `+0x40` and drawable at `+0x48`; click sets `game+0xC65C`; selected overlay drawn when `game+0xC65C == index`. This matches a section-button row with selected appearance.
4. `FUN_1404D72C0` is a different collection: it is driven by `game+0xD408` (a tab-state object with a pointer array at `+0x20` and count at `+0x38/+0x50`), iterates up to 10 top-level tabs, and its selected state is `param_1+0x40/+0x68/+1` and `DAT_0x1409E2338` — not `game+0xC65C`. It is the sidebar's top-level section bar, not the in-page row.
5. `FUN_14007C400`'s only confirmed write to `game+0xC650` stores NULL under "system"/"makecity"/"objectives"/"qa" conditions (see section 28/29); it does not enumerate or select Economy & Trade sub-tabs.

### 35.3 Native approach: extend the native button/page system

Supported:
- The row is data-driven: button count = size of the sub-entry vector attached to the selected sidebar panel. Adding a button means adding an entry to that vector (or to whatever initializer builds it).
- The click→sub-page mapping (index → `1/4/6/8/10` via `FUN_140496DE0`) shows there is a native dispatch for sub-pages; a new index would need a new case or a reused value.
- Text labels come from `C3D_LANGUAGE::GetString(id)` using the sub-entry's `+0x40` ID, so a new button needs a new string ID (or reuse an existing one).

Unproved / blockers:
- Who owns and initializes the sub-entry vector (`*(game+0xC650)+0x48/+0x50`). Capacity, allocator, and init timing are not traced. Writing into it without ownership proof risks heap corruption.
- Whether the vector is a `std::vector` (realloc on push) or a fixed array; whether there is spare capacity.
- Whether the game's content dispatcher has a slot for a 5th/6th sub-page value, or whether `FUN_140496DE0`/`FUN_140494F60` accept an arbitrary new value.
- Whether adding an entry changes vanilla sub-tab count/positions/selected indices in a way that breaks existing saves or muscle memory.
- The exact button rect (width, height, X/Y origin, pitch) is not yet extracted, so an "aligned" native addition cannot be verified against vanilla geometry.

Smallest missing evidence for the native approach: the initializer/owner of the sub-entry vector (who creates the 4 entries, when, and with what capacity), plus the exact button rect. Without that, extending the native collection is not safe.

### 35.4 Mod-owned approach: draw a button without extending a native collection

Supported:
- `FUN_140358A20` is a per-frame render function called from the sidebar host; a mod can hook or post-render after it returns and draw an extra button using the same native primitives (`Draw`, `SetPosition`, `SetDimensions`, `GetMouseSolid`, `Collision`, texture from `C3D_MIDDLEPOINT::CreateManagedTexture` or a mod texture), matching the City Hall `widgetButton` precedent (9-patch chrome, hover/active tint, click latch `0xA54E91`, `gui_click` audio).
- The mod can keep its own `almanacSelected` state and render a distinct Almanac page in the content area, without touching `game+0xC65C` or the native vector.
- The mod can detect clicks via `GetMouseSolid` + `Collision` on its own button rect, exactly as native buttons do.
- The mod can reuse the existing `taxes` service getters/setters; City Hall stays a temporary fallback.

Unproved / blockers:
- The exact button rect (X, Y, width, height) and pitch in DPI units is not yet extracted from the decompile; the mod cannot align without it (must derive from the same `fVar41`/`fVar38` expressions, which reference `DAT_0x14090ab14`, `DAT_0x14090aa0c`, `game+0xAE50`).
- Whether the mod can draw in the same pass after `FUN_140358A20` returns without the native content area overwriting or clipping it (Z/order, draw context state `game+0xADB0`).
- Whether the click latch `0xA54E91` coexists with the native sub-tab click handling (both read the same latch; consumption order matters).
- Whether the mod's content area (the Almanac page) avoids overlapping native content when a vanilla sub-tab is selected, or whether the mod must suppress native content rendering while `almanacSelected` is true (which requires intercepting the native content path — not yet proven safe).
- Close/section-switch restoration: the mod must clear `almanacSelected` and restore prior state when a vanilla button is selected or the window closes; the close path (`game+0xC650` hand-off, tab index `-1`) is unresolved.
- Whether drawing a 5th button overflows the row width at the target resolution/DPI; native wraps at 8, but visual row width is unmeasured.

Smallest missing evidence for the mod-owned approach: the exact button rect and pitch (pixels/DPI) and the native content-area bounds, plus confirmation that a post-return draw call can land on top without being clipped. The click-latch coexistence and close-path restoration also need a separately approved live diagnostic.

### 35.5 Comparison summary

| Aspect | Native extend | Mod-owned button |
|---|---|---|
| Button count / capacity | Must extend native vector; owner/capacity untraced (high risk) | No native vector change (lower structural risk) |
| Alignment / appearance | Native geometry guaranteed if vector entry is valid | Must reverse-engineer pitch/rect; alignment risk |
| Selection state | Native `game+0xC65C` | Mod-owned flag; no native state mutation |
| Page content | Native sub-page dispatch; needs new case | Mod renders Almanac; must avoid native content overlap |
| Click handling | Native latch already consumed | Must coexist with native latch consumption order |
| Vanilla preservation | Risk of corrupting vector/capacity | Risk of overlap/clipping; vanilla untouched structurally |
| Smallest next evidence | Vector initializer/owner + button rect | Button rect + content-area bounds + latch coexistence |

Neither approach is implementation-ready. Both require the same first artifact: an exact, measured button rect (X, Y, width, height, pitch) for the native section buttons, derived from the decompiled geometry expressions. That is the single smallest next step common to both.

### 35.6 Remaining blockers (exact)

1. Button geometry: exact width/height/pitch/Y-origin in DPI-scaled pixels (from `fVar41`, `fVar38`, `fVar36`, `DAT_0x14090ab14`, `DAT_0x14090aa0c`, `game+0xAE50`). Not yet extracted.
2. Content area: the rectangle `FUN_140358A20` uses for the sub-page body, its clipping, and scroll state. Not traced.
3. Vector ownership/capacity (native approach only): who builds `*(game+0xC650)+0x48/+0x50`. Not traced.
4. Sub-page dispatch values: whether `FUN_140496DE0` accepts a new value or needs a new case. Not traced.
5. Latch coexistence: order of `0xA54E91` consumption between native sub-tab click and a mod button. Not verified.
6. Close/section-switch path: how `game+0xC650` is cleared/restored and whether a mod flag can be cleared safely. Not traced.
7. Text IDs: a new string ID (>= 1,000,000 per project rule) for "Almanac", or reuse of an existing ID. Not allocated (investigation only; no implementation).

### 35.7 Smallest next step

Derive the exact button rect and pitch from the decompiled `FUN_140358A20` geometry expressions (a pure static read of `scratch/out/140358a20_decomp.c` already in hand), recording the constants `DAT_0x14090ab14` and `DAT_0x14090aa0c` and the role of `game+0xAE50`. Then, if the rect is established, a separately approved read-only live diagnostic can confirm on-screen placement and latch coexistence. No implementation, build, hook, configuration change, or live test is authorized by this report.

### 35.8 Files touched in this refresh

- `docs/almanac-investigation.md` (this section)
- `docs/almanac-native-evidence.md` (appended deciding dispatch + sub-tab loop excerpts)
- `docs/ghidra-index/358A20.md` (refined with sub-tab row findings)
- `scratch/out/140358a20_decomp.c` (new cached export, RVA `0x358A20`)

No production code, configuration, saves, builds, hooks, or settings were changed.

## 36. FUN_140358A20 button-row geometry and content path (static)

> SUPERSEDED (see section 38-40): geometry here (92x92, 85 pitch, 7px overlap) applies to the unreachable FUN_140358A20 fallback, not the actual Economy & Trade row (FUN_140373EA0: 64x64, 64 pitch, no overlap).

### 36.1 Constants resolved from the authenticated executable

Image base `0x140000000`; PE timestamp `0x6A3EB6AD`; SHA-256 `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` (matches). Values read directly from the mapped sections (RVA → file offset via section headers; see `INSTRUCTIONS.md` §1):

| Symbol | RVA | Value | Role |
|---|---|---|---|
| `DAT_140992088` | `0x992088` | `1.0` | DPI scalar (default; may differ at runtime) |
| `DAT_14090ab14` | `0x90AB14` | `85.0` | horizontal pitch (button stride) |
| `DAT_14090aa0c` | `0x90AA0C` | `45.0` | Y origin offset / row height |
| `DAT_14090aa44` | `0x90AA44` | `55.0` | X origin extra offset |
| `DAT_14090ab40` | `0x90AB40` | `92.0` | button backdrop width and height |
| `DAT_140909ddc` | `0x909DDC` | `0.5` | half-scale (used for `game+0xAE54`) |
| `DAT_140909f58` | `0x909F58` | `1.0` | sound/print scale |
| `DAT_14090a9f0` | `0x90A9F0` | `40.0` | (content block, see 36.4) |
| `DAT_14090aa28` | `0x90AA28` | `50.0` | (content block) |
| `DAT_14090ab8c` | `0x90AB8C` | `107.0` | (content block) |
| `DAT_14090a9dc` | `0x90A9DC` | `37.0` | (content block) |
| `DAT_14090ab70` | `0x90AB70` | `100.0` | (content block) |
| `DAT_14090a9f0`-adjacent `DAT_14090aa10` | `0x90AA10` | not read here | — |

Note: `DAT_14090aa10` was not in the read list; if needed, re-read. Runtime values of `game+0xAE50` and `game+0xAE54` are not statically resolvable (they are fields of the live game object).

### 36.2 Button-row geometry (verbatim excerpts)

From `scratch/out/140358a20_decomp.c` lines 93–106 and 127–131:

```c
fVar39 = DAT_140992088;          // DPI = 1.0
fVar7  = DAT_14090ab14;          // 85.0
fVar37 = DAT_14090aa0c;          // 45.0
...
fVar41 = DAT_140992088 * DAT_14090ab14;   // 85.0  (pitch)
fVar45 = fVar41 + DAT_140992088 * DAT_14090aa0c;  // 85+45 = 130.0  (X reset on wrap)
local_39c = *(float *)(param_1 + 0xae54) * DAT_140909ddc * DAT_140992088 + fVar41;  // 0.5*game+0xAE54 + 85
fVar38 = DAT_140992088 * DAT_14090aa0c + *(float *)(param_1 + 0xae50);  // 45 + game+0xAE50  (Y origin)
...
fVar43 = fVar39 * DAT_14090ab40;   // 92.0  (button width and height)
fVar36 = fVar39 * DAT_14090aa44 + fVar41;  // 55 + 85 = 140.0  (X origin, first button)
```

From lines 137–199 (the button loop):

```c
lVar25 = *(longlong *)(*(longlong *)(param_1 + 0xc650) + 0x48);   // vector begin
FUN_1404461f0(param_1 + 0xadb0, lVar16, fVar38, 6);             // panel setup (count, Y, mode 6)
plVar5 = *(longlong **)(lVar33 + 0x48 + lVar25);                  // drawable at sub_entry+0x48
(**(code **)(*plVar5 + 0x70))(plVar5, 0, DAT_1409ead08);          // draw icon
...
_DAT_1409be2f0 = CONCAT44(fVar38, fVar36);   // position (Y, X)
_DAT_1409be2e8 = fVar43;                     // width  = 92
_DAT_1409be2ec = fVar43;                     // height = 92
uVar17 = C3D_INPUT::GetMouseSolid(...);
bVar12 = C3D_PANEL2D::Collision(...);         // hit-test against the 92x92 rect
if (bVar12) {
  if (DAT_140a54e91 != '\0') {               // click latch
    *(uint *)(param_1 + 0xc65c) = uVar26;     // selected sub-tab index
    *(undefined4 *)(param_1 + 0xccc8) = 0xffffffff;
    *(undefined4 *)(param_1 + 0xccd0) = 0;
    *(undefined8 *)(param_1 + 0xc6b8) = *(undefined8 *)(param_1 + 0xc6b0);
    // play gui_subtab_click sound
  }
  // copy C3D_LANGUAGE::GetString(sub_entry+0x40) into game+0xD5A0  (label)
}
if (*(uint *)(param_1 + 0xc65c) == uVar26) {
  _DAT_1409be30c = _DAT_14090c4c0;          // selected overlay
  uRam00000001409be314 = _UNK_14090c4c8;
}
C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060, 0.0, 0.0, fVar8, fVar8, 0.0, true);
uVar14 = uVar26 & 0x80000007;                 // wrap every 8 buttons
if ((int)uVar14 < 0) { uVar14 = (uVar14 - 1 | 0xfffffff8) + 1; }
if (uVar14 == 7) {
  fVar38 = fVar38 + fVar41;                   // next row: Y += 85
  fVar36 = fVar45;                            // X resets to 130
} else {
  fVar36 = fVar36 + fVar41;                   // next column: X += 85
}
uVar26 = uVar26 + 1;
lVar33 = lVar33 + 0x50;                      // stride 0x50 (80 bytes)
```

Derived row geometry (at DPI = 1.0; scales linearly with `DAT_140992088`):
- Button backdrop: 92 × 92 (collision/hover rect).
- Origin of button 0: X = 140, Y = 45 + `game+0xAE50`.
- Horizontal stride (pitch): 85. Vertical stride (row height): 85.
- Wrap: after every 8 buttons (indices 0..7 in row 0; 8..15 in row 1, ...).
- X reset on wrap: 130 (10 less than the first button's X of 140 — a minor asymmetry in the native code).
- Button count: `(end - begin) / 0x50` where begin = `*(game+0xC650)+0x48`, end = `*(game+0xC650)+0x50`. With 4 sections, indices 0–3 occupy X = 140, 225, 310, 395 in row 0.
- A 5th button (Almanac) would land at index 4: X = 140 + 4×85 = 480, Y = 45 + `game+0xAE50` — still in row 0, before the wrap at index 7→8. So by the native wrap rule it fits in the existing first row. However, the backdrop rects are 92 wide on an 85 pitch, so adjacent backdrops overlap by 7 px in every existing case as well; the mod button would reproduce the same overlap pattern, not avoid it. The actual on-screen icon size comes from the unresolved drawable object at `sub_entry+0x48` (drawn first via its vtable `+0x70`), so the visible icon may be smaller than 92. Whether the container is wide enough for X≈480+92 at the user's resolution/DPI is not established.
- `game+0xAE50` (Y base) and `game+0xAE54` (used as `0.5*value+85` in `local_39c`) are runtime fields of the live game object; their values are not statically resolvable. `local_39c` is computed but not used in the button loop shown (it is used later, line 564+), so it does not affect button placement in this loop.

### 36.3 Count and available width

The count is not bounded by an available-width check in this function. It is purely the size of the sub-entry vector, and the layout wraps at a fixed 8-per-row. There is no "does it fit" test. Therefore:
- The native code already assumes up to 8 buttons per row and wraps beyond that.
- A mod-owned button does not need a spare native vector slot if it is drawn after the native loop at the next grid position (index 4 → X 480), using the same pitch/origin expressions and the same backdrop dimensions.
- Whether that position is visible (not clipped by the window) and whether it collides with existing buttons at the user's resolution/DPI is not established. The native wrap at 8 implies the designer expected up to 8, but the container's actual pixel width is unknown.

### 36.4 Content area that must be suppressed or replaced while the Almanac is open

After the button row, the function renders a content area driven by `game+0xC65C` (the selected sub-tab index). Verbatim (lines 202–292):

```c
fVar38 = fVar39 * fVar37 + *(float *)(param_1 + 0xae50);   // 45 + game+0xAE50 (Y)
fVar37 = fVar39 * DAT_14090a9f0 + fVar39 * fVar7;         // 40 + 85 = 125 (X)
iVar4 = *(int *)(param_1 + 0xc65c);                         // selected sub-tab
if ((-1 < iVar4) && (iVar4 < (end-begin)/0x50)) {
  C3D_LANGUAGE::GetString(..., *(int *)(iVar4*0x50 + begin + 0x40));   // label
  C3D_FONTMANAGER::PrintLeftUnicode(..., fVar37, ..., (fVar38 - DPI*37) + 85);  // print label
}
fVar38 = fVar38 + DPI*107;                                 // 107
fVar41 = DPI*50;                                            // 50
fVar39 = DPI*0.5 + fVar41;                                  // 50.5
FUN_1404461f0(param_1 + 0xadb0);                            // panel setup
... Draw panel at (fVar37=125, fVar38) size 50 ...
// hit-test panel; on click with latch:
//   map game+0xC65C (0..4) -> uVar30 (1,4,6,8,10)
//   lVar16 = FUN_140496de0(game+0x12800, uVar30);   // find content entry
//   if found: FUN_1406ee2a0(param_1, 2, lVar16);    // open content
//   else: play cancel sound
```

Then (lines 353–477) a second, larger content block is rendered unconditionally:
- A panel at `(fVar37, fVar38 + 3*fVar39 + ...)` (size `fVar41 = 50`) with its own hit-test and "line_addstop_copy2" toggle handling (lines 303–352).
- `FUN_140497050` / `FUN_140494F60` (called twice) populate `game+0x12800` with `iVar32` (the mapped value `1/4/6/8/10`) and a constant `0x12`.
- `FUN_140815760(...)` and further text/panel drawing (lines 527–614) render the body.

Implication for a mod-owned Almanac page: the native content area is not gated by anything a mod can set without modifying `game+0xC65C` (which would change the vanilla selection and be visible to vanilla logic). To show an Almanac page without obscuring/changing vanilla pages, the mod must either (a) suppress the native content draw calls in this function while `almanacSelected` is true, or (b) draw the Almanac over them and accept overlap. Option (a) requires hooking inside this function or replacing its content path; option (b) requires knowing the content rectangle (X≈125, Y≈45+game+0xAE50+107+..., size ≈50 for the header panel, plus the body below) so the Almanac can be sized/placed without collision. The content rectangle's full extent (body height, clipping) is not traced. The function also writes `game+0xC6B8 = game+0xC6B0` and toggles `game+0x12800` state on vanilla clicks, so a mod that only sets a flag (and does not touch these) leaves vanilla state consistent, but the native content will still draw unless suppressed.

### 36.5 Return of control to vanilla

- Vanilla button click: the native loop (section 36.2) sets `game+0xC65C = index` on latch consumption. A mod-owned Almanac button must NOT write `game+0xC65C`; it sets only a mod-owned flag. To detect "user returned to vanilla", the mod compares the current `game+0xC65C` against the value it recorded when it opened the Almanac: if it changed, a vanilla button was clicked, so the mod clears its flag. This avoids restoring a stale selection over a newer player choice (no raw-pointer restore).
- Close: this function does not contain the close path. Close is handled elsewhere (likely in `FUN_140317DB0` via visibility `game+0xFCE` and tab index `-1`). The mod must clear its flag when visibility drops or when the selected entry changes away from the Economy & Trade entry. The exact close hand-off (`game+0xC650` vs tab index `-1`) is unresolved (see `docs/almanac-investigation.md` §6.3 / §13.3).
- Because the mod keeps its own flag and never mutates `game+0xC650`/`game+0xC65C` or the native vector, vanilla selection and close behavior remain authoritative; the mod only adds rendering and a flag.

### 36.6 Unresolved questions

1. Runtime identity: whether the selected entry is actually "importexport" at runtime is not verified (static inference only). The binary strings and dispatch make it likely, but no runtime observation exists.
2. `game+0xAE50` / `game+0xAE54` runtime values and their ownership (who writes them, when) are untraced.
3. The drawable object at `sub_entry+0x48` (vtable `+0x70` draw) — its type, size, and how its texture is resolved — is untraced; the visible icon dimensions are therefore unknown, and the 92×92 backdrop overlap cannot be reconciled with icon size.
4. Container width / clipping: no available-width test exists; whether X≈480+92 is visible is unverified.
5. Content rectangle full extent (body height, scroll, clipping) for the sub-page area — untraced.
6. Whether `FUN_140496DE0` accepts a value not in {1,4,6,8,10} or whether a new sub-page value requires a new case — untraced.
7. Close path and how to detect it reliably from a mod hook — untraced.
8. Latch coexistence: `0xA54E91` is read (not necessarily consumed) in this function; whether a mod button's click handling races the native sub-tab click handling is unverified.
9. `DAT_14090aa10` and several `DAT_14090a*` constants used in the content block were not read; if exact content geometry is needed, re-read them.

### 36.7 Smallest next step

Re-read the remaining unread constants (`DAT_14090aa10`, `DAT_14090aa70`-adjacent, etc.) and trace the drawable object at `sub_entry+0x48` (its constructor/owner) to resolve the visible icon size and the backdrop-vs-icon relationship. Then, if geometry is established, a separately approved read-only live diagnostic can confirm on-screen placement and latch coexistence. No implementation, build, hook, configuration change, or live test is authorized by this report.

## 37. FUN_140358A20 deeper static findings (drawable, constants, return path)

> SUPERSEDED (see section 38-40): applies to the unreachable FUN_140358A20 fallback only.

### 37.1 The drawable at sub_entry+0x48 is a texture, not a separate icon widget

The sidebar panel initializer `FUN_140296110` (RVA `0x296110`) builds the sub-entry vector and creates each sub-entry's drawable. Verbatim (scratch/out/140296110_decomp.c, lines 95-106):

```c
lVar11 = uVar20 * 0x50;
sprintf(local_498, "editor/%s_%s.png",
        lVar8 + 1 + lVar19,                          // panel name (at panel_entry+1)
        *(longlong *)(lVar8 + 0x48 + lVar19) + lVar11); // sub-entry name (at sub_entry+0)
lVar8 = *(longlong *)(*(longlong *)(param_1 + 0xc608) + 0x48 + lVar19);
pCVar7 = C3D_MIDDLEPOINT::CreateManagedTexture((C3D_MIDDLEPOINT *)&DAT_1409eacd0, local_498);
*(C3DAPI_TEXTURE **)(lVar11 + 0x48 + lVar8) = pCVar7;   // sub_entry+0x48 = texture
plVar2 = *(longlong **)(... + 0x48 + lVar11);
(**(code **)(*plVar2 + 0x10))(plVar2, local_498, 0, 0);   // vtable+0x10 = Load(filename)
```

So `sub_entry+0x48` is a `C3DAPI_TEXTURE*` whose filename is `"editor/<panel>_<sub>.png"` (e.g. `"editor/trade_importexport.png"`). In `FUN_140358A20` (lines 138-141) it is drawn first via its vtable `+0x70`:

```c
plVar5 = *(longlong **)(lVar33 + 0x48 + lVar25);   // sub_entry+0x48
(**(code **)(*plVar5 + 0x70))(plVar5, 0, DAT_1409ead08);  // draw texture
```

The four section PNGs on disk (`media_soviet/editor/`) are 96x96. The backdrop panel drawn afterwards (line 181) is 92x92. Therefore the visible icon is 96x96 and the mouse-hit/selection backdrop is 92x92 - a 4 px total (2 px per side) mismatch, with the icon slightly larger than the hit rect. The PNG dimensions are runtime/asset data, not encoded in the exe; they were measured from the installed media files, not inferred.

### 37.3 Section count and container width (static limits)

- Count is `(end - begin) / 0x50` with begin = `*(game+0xC650)+0x48`, end = `*(game+0xC650)+0x50`. There is no available-width test and no clamp in this function. The screenshot's four buttons are visual context only; the runtime collection size is not proven to be four and could differ (e.g. if a panel has more or fewer sub-entries).
- The wrap at 8 implies the original designer expected up to 8 buttons per row, but that is a layout convention, not a capacity guarantee. The container's pixel width and clipping rectangle are not in this function (they belong to the sidebar host `FUN_140317DB0` and the draw context `game+0xADB0`, untraced here).
- Therefore a mod button at index 4 (X = 140 + 4x85 = 480) is arithmetically inside row 0, but whether X~480+92 is visible (not clipped) cannot be established statically. This is a geometry observation, not a safety proof.

### 37.4 Return to vanilla: the already-selected button case

The native loop sets `game+0xC65C = index` only when a collision occurs AND the click latch `DAT_140a54e91` is set (lines 148-163). Consequences for a mod-owned flag:

- If the user clicks a *different* vanilla button, `game+0xC65C` changes - the mod can detect the change and clear its flag. This works.
- If the user clicks the *same* button that was selected when the Almanac opened, `game+0xC65C` does not change (it is already that value). The native code still consumes the latch and runs the click handler (lines 153-166: sound, state updates, label copy), but `game+0xC65C` is unchanged. Therefore comparing `game+0xC65C` against the recorded value is insufficient for this case.
- To detect "any vanilla button clicked, including the already-selected one", the mod must observe a signal that fires on every native click regardless of selection change. Candidates: (a) consumption of the latch `DAT_140a54e91` (transitions 1->0) - but this function does not visibly clear it; the latch's owner/clear site is untraced, so relying on it is unverified; (b) the side effects that always occur on click (sound playback, `game+0xCCC8 = -1`, `game+0xCCD0 = 0`, `game+0xC6B8 = game+0xC6B0`, label copy into `game+0xD5A0`) - these are observable but require knowing their baseline values; (c) a pre-hook recording the click position (mouse coordinates) before native handling, then checking whether the point falls inside any native button rect. Option (c) is the most robust but requires a hook, which is not authorized.
- None of these are verified; the exact missing evidence is who owns and clears `DAT_140a54e91` and whether the native click path always executes the side effects above even when the latch is already 0. This must be traced before a mod can reliably detect "return to vanilla."

### 37.6 Body extent (partially traced)

The body block (lines 458-614) renders further content below the sub-page: `FUN_140815760` at (DPI*5 + fVar45 = 5+130 = 135, Y-5), then loops drawing text/panels using 450/80/20/110/15/5/10/46/400/128/135/72/70/66/63/59. The full body height, clipping rectangle, and scroll state are not traced; only the header panel (50x50 at 125, 152+game+0xAE50) is fully resolved. A mod Almanac page that replaces the body must either suppress this entire block or draw over it; neither is safe without the body extent and clip rect. The body uses `game+0x12800` (a list/grid object) and `game+0xCCC8` (a counter, compared to 0x12) to decide refresh; these are vanilla state the mod should not corrupt.

### 38.3 Corrected geometry for FUN_140373EA0 (actual handler)

From the same disassembly (scratch/out/140373ea0_1403829fd_disasm.asm):

- Backdrop: 64 x 64 (DPI * DAT_0x90aaa0 = 1.0 * 64.0). (RVA 0x90AAA0 = 64.0, verified.)
- X origin (button 0): XMM6 = DPI * (DAT_0x90aa0c + XMM11) = 1.0 * (45 + 85) = 130. (XMM11 = DAT_0x90ab14 = 85.0, set at line 36 of the disasm.)
- Pitch (X increment per button): XMM7 = DPI * DAT_0x90aaa0 = 1.0 * 64.0 = 64.0 (line 139: MULSS XMM7, XMM12 where XMM12 = 64.0). So adjacent buttons are spaced 64 px apart and the backdrop is 64x64: NO overlap (contrast with the erroneous FUN_140358A20 analysis which had 92x92 on 85 pitch).
- Y origin: not fully resolved in the extracted window; the backdrop position uses XMM6 for X and a separate value for Y. The panel setup call at 0x140445eb0 takes (RSI+0xADB0, count, XMM6, 6). The exact Y base is a runtime field.
- Backdrop-vs-icon: section PNGs are 96x96 on disk; backdrop is 64x64. So the icon (96) is larger than the hit rect (64) — opposite of the FUN_140358A20 case (92 backdrop vs 96 icon). The icon draw (vtable+0x70) happens first; the 64x64 panel (Collision/Draw) is drawn after. Whether the icon is clipped or scaled by the texture's own draw is not in this window.
- Layout: the loop has special index mapping (EDI 0/1 -> EBX=EDI; EDI 2 -> EBX=count-1; otherwise EBX=EDI-1). This reorders sub-entries; the row is NOT a simple linear 0,1,2,... sequence. The full layout (which sub-entry lands at which position) is not decoded here. A mod button cannot be placed by simple index arithmetic without decoding this mapping.
- Count: (end-begin)/0x50, same as before. No available-width test in this function.

### 38.4 Implications for the mod-owned button approach

- The target handler is `FUN_140373EA0`, not `FUN_140358A20`. Any hook/diagnostic must target `FUN_140373EA0`.
- Geometry is 64x64 backdrop, 64 px pitch (no overlap), X origin 130. But the reordering logic means "next free slot" is not simply "index N"; the layout must be decoded first.
- The overlap-click-winner claim from the FUN_140358A20 analysis does NOT apply here (64 backdrop on 64 pitch = no overlap). It is conditional on the handler and must be dropped for the actual handler.
- The PNG source dimensions (96x96) are distinct from rendered dimensions (64x64 backdrop); the texture's own draw scale is untraced.

### 38.5 What remains unverified

1. Runtime selected entry identity: whether the live selected entry is "trade" (and whether "importexport" ever occurs as a top-level name) is inferred from texture filenames and dispatch, not observed.
2. The reordering logic in FUN_140373EA0 (EBX mapping) and the resulting button positions.
3. Y origin and container width/clipping.
4. Close hand-off and latch owner (unchanged from section 37.5).
5. Whether FUN_140358A20 is ever reached (likely dead for this page).

### 39.7 Files touched in this refresh

- `docs/almanac-investigation.md` (this section)
- `docs/ghidra-index/373EA0.md` (already created; no change this pass)

No production code, configuration, saves, builds, hooks, or settings were changed.

## 38. Handler identity correction: FUN_140373EA0 draws the trade_*.png buttons

### 38.2 Decisive evidence: FUN_140373EA0 uses the sub-entry vector and trade textures

`FUN_140373EA0` (RVA `0x373EA0`, size 55,231 bytes) is the handler reached when the selected entry name is "trade" (dispatch at `FUN_140317DB0` RVA `0x31EB49`-`0x31EB64`, preserved in `docs/almanac-native-evidence.md`). It reads the sub-entry vector and draws each sub-entry's texture, exactly like the row renderer we need.

`FUN_140373EA0` button-row evidence (scratch/out/140373ea0_1403829fd_disasm.asm):

```asm
140374167: MOV R8, qword ptr [RSI + 0xc650]     ; selected entry
14037416e: ADD R8, 0x48                          ; -> sub-entry vector begin
140374172: MOV RCX, qword ptr [R8 + 0x8]         ; end
140374176: SUB RCX, qword ptr [R8]               ; end - begin
... divide by 0x50 -> count (EDX)
14037419a: TEST EDX, EDX
14037419c: JLE 0x1403745b6                       ; no sub-entries -> skip
...
1403741f6: MOVSXD RAX, EBX
1403741f9: LEA R13, [RAX + RAX*4]
1403741fd: SHL R13, 0x4                          ; R13 = EBX * 0x50
140374201: ADD R13, qword ptr [R8]               ; R13 = sub_entry base
140374204: MOV RCX, qword ptr [R13 + 0x48]       ; texture at sub_entry+0x48
140374208: MOV RAX, qword ptr [RCX]
14037420b: MOV R8, qword ptr [0x1409ead08]
140374212: XOR EDX, EDX
140374214: CALL qword ptr [RAX + 0x70]           ; draw texture
140374217: MOVSS XMM0, dword ptr [0x140992088]   ; DPI
14037421f: MULSS XMM0, dword ptr [0x14090aaa0]   ; 64.0
140374224: MOVSS dword ptr [0x1409be2e8], XMM0    ; backdrop width  = 64
14037422c: MOVSS dword ptr [0x1409be2ec], XMM0    ; backdrop height = 64
...
140374243: MOVSD qword ptr [0x1409be2f0], XMM0    ; position (X, Y)
...
140374255: MOV R8D, 0x6
14037425b: MOVAPS XMM1, XMM6
14037425e: LEA RCX, [RSI + 0xadb0]
140374265: CALL 0x140445eb0                       ; panel setup
...
1403742a4: CALL qword ptr [0x14086c850]           ; Collision (hit test)
1403742aa: TEST AL, AL
1403742ac: JZ 0x140374366
...
1403742f2: CMP byte ptr [0x140a54e91], CL          ; latch
1403742f8: JZ 0x140374366
...
140374347: CMP dword ptr [RSI + 0xc65c], EBX       ; if selected != this
14037434d: JZ 0x140374356
14037434f: MOV byte ptr [RSI + 0x1039a], 0x0       ; dirty flag
140374356: MOV dword ptr [RSI + 0xc65c], EBX       ; selected = this
14037435c: MOV dword ptr [RSI + 0x10be8], -1
140374366: CMP dword ptr [RSI + 0xc65c], EBX       ; if selected == this
14037436c: JNZ 0x14037437c
14037436e: MOVAPS XMM0, xmmword ptr [0x14090c4c0]
140374375: MOVUPS xmmword ptr [0x1409be30c], XMM0  ; selected overlay
```

Conclusion: `FUN_140373EA0` is the handler that actually draws the trade_*.png buttons. It reads `game+0xC650` (selected panel entry), walks its sub-entry vector (stride 0x50), draws each sub-entry's texture (at `sub_entry+0x48`) with a 64x64 backdrop, and tracks the selected sub-tab in `game+0xC65C`. `FUN_140358A20` is only reached for panel names not in the explicit list; since no panel is named "importexport" (no `importexport_*.png`), `FUN_140358A20` is effectively dead/unreachable for the Economy & Trade page. The earlier geometry analysis (92x92, pitch 85, 7px overlap) applied to the wrong handler and must not be reused.

### 37.5 Close path: exact missing evidence

This function (`FUN_140358A20`) contains no close handling. The close behavior lives in `FUN_140317DB0` (the sidebar host): visibility is gated by `game+0xFCE`, and closing is represented by tab index `-1` in the 10-slot tab object at `obj+0x68` (see `docs/almanac-investigation.md` section 6.3 / 13.3). The exact hand-off from that tab index to `game+0xC650` (the selected entry pointer) is not traced, nor is it known whether `game+0xC65C` (the sub-tab index) is cleared on close. A mod flag must be cleared on close, but the reliable trigger (visibility drop vs. selected-entry change vs. tab index -1) cannot be chosen without tracing that hand-off. This is the exact missing close-path evidence.

### 37.7 Unresolved questions (cumulative)

1. Runtime selected entry identity: whether the live selected entry is "importexport" is inferred, not observed.
2. `game+0xAE50` / `game+0xAE54` owners and runtime values.
3. Visible icon size is 96x96 (from media), but the draw position of the texture relative to the 92x92 backdrop (centered? offset?) is not in the decompile (the texture's own draw uses its internal origin).
4. Container width, clipping, and scroll state.
5. Body extent and clip rect.
6. `FUN_140496DE0` accepted values (whether a new value beyond {1,4,6,8,10} works).
7. Latch owner/clear site and whether click side effects always fire.
8. Close hand-off (tab index -1 -> game+0xC650/game+0xC65C).
9. Whether the runtime collection always has four entries.

### 37.8 Smallest next step

Trace (a) the latch `DAT_140a54e91` owner/clear site to settle click detection for the already-selected button, and (b) the close hand-off in `FUN_140317DB0` (visibility `game+0xFCE` -> tab index -1 -> `game+0xC650`). Both are pure static traces of existing exports/callers; no implementation, build, hook, configuration change, or live test is authorized. Only after those are traced can a mod reliably detect "return to vanilla" and "window closed."

### 37.9 Files touched in this refresh

- `docs/almanac-investigation.md` (this section)
- `docs/ghidra-index/358A20.md` (updated with 96x96 icon / 92x92 backdrop / overlap finding)

No production code, configuration, saves, builds, hooks, or settings were changed.

### 37.2 Constants: panel-relative vs. runtime

All geometry below is in the panel-relative coordinate space of `game+0xADB0` (the widget draw context set by `FUN_140317DB0`). It is not screen space; the absolute screen origin depends on `game+0xAE50` (a runtime field of the live game object) and on the sidebar's own layout, which is not traced in this function. Static binary defaults (read from the mapped `.rdata`/`.data` sections) vs. runtime values:

| Symbol | RVA | Static value | Kind |
|---|---|---|---|
| `DAT_140992088` | `0x992088` | `1.0` | DPI scalar; default in file, runtime value depends on display/DPI settings |
| `DAT_14090ab14` | `0x90AB14` | `85.0` | static pitch (both axes) |
| `DAT_14090aa0c` | `0x90AA0C` | `45.0` | static Y origin offset |
| `DAT_14090aa44` | `0x90AA44` | `55.0` | static X origin extra offset |
| `DAT_14090ab40` | `0x90AB40` | `92.0` | static backdrop width/height |
| `DAT_140909ddc` | `0x909DDC` | `0.5` | static half-scale |
| `DAT_140909f58` | `0x909F58` | `1.0` | static sound/print scale |
| `DAT_14090a9f0` | `0x90A9F0` | `40.0` | static (content block X) |
| `DAT_14090aa28` | `0x90AA28` | `50.0` | static (content block size) |
| `DAT_14090ab8c` | `0x90AB8C` | `107.0` | static (content block Y delta) |
| `DAT_14090a9dc` | `0x90A9DC` | `37.0` | static (content block label offset) |
| `DAT_14090ab70` | `0x90AB70` | `100.0` | static (content block Y delta) |
| `DAT_14090aa10` | `0x90AA10` | `46.0` | static (content block) |
| `DAT_14090af08` | `0x90AF08` | `450.0` | static (body layout) |
| `DAT_14090aafc` | `0x90AAFC` | `80.0` | static (body layout) |
| `DAT_14090a928` | `0x90A928` | `20.0` | static (body layout) |
| `DAT_14090ab98` | `0x90AB98` | `110.0` | static (body layout) |
| `DAT_14090a8cc` | `0x90A8CC` | `15.0` | static (body layout) |
| `DAT_14090a6a8` | `0x90A6A8` | `5.0` | static (body layout) |
| `DAT_14090a828` | `0x90A828` | `10.0` | static (body layout) |
| `game+0xAE50` | field | unknown | runtime field: Y base for the row |
| `game+0xAE54` | field | unknown | runtime field: used as `0.5*value+85` in `local_39c` (not used in the button loop) |

The formulae:
- Button backdrop: 92 x 92 (static, DPI-scaled at runtime).
- Button 0 origin: X = 55 + 85 = 140; Y = 45 + `game+0xAE50`.
- Pitch: 85 (X and Y).
- Wrap: after every 8 buttons (indices 0..7 row 0), Y += 85, X resets to 130.
- Content header panel: 50 x 50 at X = 40 + 85 = 125; Y = (45 + `game+0xAE50`) + 107 = 152 + `game+0xAE50`. Label printed at (125, Y-37+85).
- The body block (lines 458-614) uses 450/80/20/110/15/5/10/46 etc. for further layout; full extent not traced (see 37.6).

The sidebar panel initializer `FUN_140296110` (RVA `0x296110`) builds the sub-entry vector and creates each sub-entry's drawable. Verbatim (scratch/out/140296110_decomp.c, lines 95-106):

```c
lVar11 = uVar20 * 0x50;
sprintf(local_498, "editor/%s_%s.png",
        lVar8 + 1 + lVar19,                          // panel name (at panel_entry+1)
        *(longlong *)(lVar8 + 0x48 + lVar19) + lVar11); // sub-entry name (at sub_entry+0)
lVar8 = *(longlong *)(*(longlong *)(param_1 + 0xc608) + 0x48 + lVar19);
pCVar7 = C3D_MIDDLEPOINT::CreateManagedTexture((C3D_MIDDLEPOINT *)&DAT_1409eacd0, local_498);
*(C3DAPI_TEXTURE **)(lVar11 + 0x48 + lVar8) = pCVar7;   // sub_entry+0x48 = texture
plVar2 = *(longlong **)(... + 0x48 + lVar11);
(**(code **)(*plVar2 + 0x10))(plVar2, local_498, 0, 0);   // vtable+0x10 = Load(filename)
```

So `sub_entry+0x48` is a `C3DAPI_TEXTURE*` whose filename is `"editor/<panel>_<sub>.png"` (e.g. `"editor/trade_importexport.png"`). In `FUN_140358A20` (lines 138-141) it is drawn first via its vtable `+0x70`:

```c
plVar5 = *(longlong **)(lVar33 + 0x48 + lVar25);   // sub_entry+0x48
(**(code **)(*plVar5 + 0x70))(plVar5, 0, DAT_1409ead08);  // draw texture
```

The four section PNGs on disk (`media_soviet/editor/`) are 96x96. The backdrop panel drawn afterwards (line 181) is 92x92. Therefore the visible icon is 96x96 and the mouse-hit/selection backdrop is 92x92 - a 4 px total (2 px per side) mismatch, with the icon slightly larger than the hit rect. The PNG dimensions are runtime/asset data, not encoded in the exe; they were measured from the installed media files, not inferred.

Mouse hit area: the `C3D_PANEL2D::Collision` at line 149 tests the 92x92 rect at `(fVar36, fVar38)` (position `_DAT_1409be2f0 = CONCAT44(fVar38, fVar36)`). Adjacent buttons' hit rects are 92 wide on an 85 pitch, so they overlap by 7 px on the shared edge. Click resolution: the loop processes indices 0,1,2,... in order and, on collision, writes `game+0xC65C = uVar26` (line 163), overwriting any earlier hit. Hence in a 7 px overlap zone the higher-indexed button wins. This is the native resolution rule and is not a mod-specific risk, but a mod button inserted at index 4 would participate in the same overlap rule with index 3 (and 5, if present).
## 39. FUN_140373EA0 dispatch condition, row ordering, and content path

### 39.1 Dispatch condition at game+0x10398 (both branches)

When the selected entry name is "trade", FUN_140317DB0 (RVA 0x317DB0) reaches (scratch/out/14031eddf_14031edf5_disasm.asm, 0x14031ede3-0x14031ee18):

    14031ede3: MOV RCX, R14                          ; game
    14031ede6: CMP byte ptr [R14 + 0x10398], R15B     ; R15B = 0
    14031eded: JZ 0x14031ee10                          ; if (game+0x10398 == 0) -> alternative
    14031edef: CALL 0x140373ea0                        ; else -> FUN_140373EA0 (button-row handler)
    14031edf4: MOVSS XMM0, dword ptr [RBP + -0x10]     ; (RBP-0x10 was 0 before call)
    14031edf9: SUBSS XMM0, dword ptr [R14 + 0xae50]    ; XMM0 -= game+0xAE50
    14031ee02: MOVSS dword ptr [R14 + 0xae58], XMM0     ; game+0xAE58 = (RBP-0x10) - game+0xAE50
    14031ee0b: JMP 0x14031f018
    14031ee10: XOR R9D, R9D
    14031ee13: CALL 0x140361bb0                        ; alternative handler (FUN_140361BB0)
    14031ee18: JMP 0x14031f018

- game+0x10398 is a byte flag. When 0, the alternative handler FUN_140361BB0 (RVA 0x361BB0, a large function with a 0x1B38 stack frame) runs. When non-zero, FUN_140373EA0 runs (the button-row handler) and then computes game+0xAE58.
- What sets game+0x10398 is not traced. It is likely a runtime state (possibly a "trade UI enabled" or "advanced view" flag). The alternative handler FUN_140361BB0 may be a simpler/read-only view. This is the exact missing edge: the condition under which the button-row handler actually runs is not established.
- FUN_140361BB0 was not decompiled (DecompAt timed out; only a 64-byte disasm prologue is cached). Its purpose is unresolved.

### 39.2 Row ordering (special index mapping)

In FUN_140373EA0, the loop counter is EDI (0..count-1) but the sub-entry index drawn is EBX, computed as (scratch/out/140373ea0_1403829fd_disasm.asm, lines 159-174):

    1403741a2: CMP EDI, 0x2
    1403741a5: JGE 0x1403741ab
    1403741a7: MOV EBX, EDI          ; if EDI < 2: EBX = EDI  (positions 0,1 -> sub-entries 0,1)
    1403741a9: JMP 0x1403741cf
    1403741ab: JNZ 0x1403741cc
    1403741ad: ...                    ; if EDI == 2: EBX = count-1 (last sub-entry)
    1403741c8: DEC EBX                ; EBX = count-1
    1403741ca: JMP 0x1403741cf
    1403741cc: LEA EBX, [RDI - 0x1]  ; if EDI >= 3: EBX = EDI - 1  (positions 3.. -> sub-entries 2..)

So the rendered order is: [sub0, sub1, sub_{count-1}, sub2, sub3, ..., sub_{count-2}]. The last sub-entry is pulled into position 2 (index 2). The purpose is not established (possibly a "back"/"summary" button pinned to a fixed slot).

Consequences for a mod button:
- There is no simple "append at the end" position. Adding a sub-entry changes count and shifts every EBX >= 3 mapping.
- A mod-owned button at a fixed screen position would need to know count, the reordering, and the resulting X. The X increment per button is 64 px (pitch = backdrop = 64), X origin 130, so position n (in rendered order) is X = 130 + n*64. But "n" depends on the reordering and count.
- Whether an additional button would remain visible and aligned cannot be established without decoding the full layout and the container width. The reordering means even the count of visible buttons per row is not a simple function of count.
### 39.3 Normal page-content path

After the button row, FUN_140373EA0 renders content driven by game+0xC65C (selected sub-tab). Evidence:
- At line 0x140374347: if game+0xC65C != EBX (selection changed), sets game+0x1039a = 0 (dirty flag).
- At line 0x140374356: game+0xC65C = EBX (selected = this button).
- At line 0x14037435c: game+0x10be8 = -1.
- At line 0x140374366: if game+0xC65C == EBX, draw selected overlay.
- The body then uses game+0x12800 (a list/grid object) and game+0x10be8 (compared to values later) for content.

Missing evidence for replacing content with an Almanac page:
- The full content rectangle, clipping, and scroll state are not traced.
- Whether game+0xC65C can be left untouched (mod flag only) while suppressing the body draw is unverified — the body is not gated by any mod-settable flag.
- Whether the body's use of game+0x12800/game+0x10be8 would be corrupted if the mod skips the draw is unverified.
- The alternative path (game+0x10398 == 0 -> FUN_140361BB0) is unresolved; if the mod targets only FUN_140373EA0, it may miss the alternative view entirely.

### 39.4 Corrected/marked superseded claims

- docs/almanac-investigation.md sections 35-37 and docs/ghidra-index/358A20.md described the row as FUN_140358A20 with 92x92 backdrop, 85 pitch, 7px overlap, and wrap-at-8. These are SUPERSEDED for the Economy & Trade row: the actual handler is FUN_140373EA0 with 64x64 backdrop, 64 pitch, no overlap, and a special reordering. The FUN_140358A20 analysis remains valid only for the (likely unreachable) "importexport" fallback path.
- docs/ghidra-index/358A20.md status remains partial and now notes the supersession. docs/ghidra-index/373EA0.md (new) records the actual handler.
- The "trade_importexport.png" texture is a sub-entry of the "trade" panel, not evidence of a top-level "importexport" panel. However, the non-existence of importexport_*.png files alone does not prove "importexport" cannot be a panel name; it is supporting evidence only. The dispatch and texture-naming convention together make "trade" the panel name.
- The overlap-click-winner claim is conditional on the handler: it does not apply to FUN_140373EA0 (64 on 64 = no overlap).

### 39.5 Remaining runtime uncertainties

1. What sets game+0x10398 and whether FUN_140373EA0 is the normal path.
2. The reordering logic and resulting button positions (statically decodable but not yet decoded).
3. Y origin, container width, clipping, scroll.
4. Body extent and clip rect.
5. Close hand-off and latch owner.
6. Whether the runtime collection always has the expected entries.

### 39.6 Smallest next step

Decode the reordering loop in FUN_140373EA0 (the EBX mapping and any position clamping) from the existing disassembly, and trace what sets game+0x10398. Both are pure static reads; no implementation, build, hook, configuration change, or live test is authorized.

### 39.7 Files touched in this refresh

- docs/almanac-investigation.md (this section)
- docs/ghidra-index/373EA0.md (already created; no change this pass)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 40. game+0x10398 dispatch and FUN_140361BB0 row path

### 40.1 What sets game+0x10398

`game+0x10398` is toggled by `FUN_140309D80` (RVA 0x309D80; cached export scratch/ms_cam_309d80.c, lines 135-138):

    if (((*(char *)(param_1 + 9) != '\0') && (DAT_0x140a54bad != '\0')) &&
       (DAT_0x140a54cc0 != '\0')) {
      *(bool *)(param_1 + 0x10398) = *(char *)(param_1 + 0x10398) == '\0';
    }

So it toggles when param_1+9 (a "UI active" byte) is set and two input flags (DAT_0x140a54bad, DAT_0x140a54cc0) are set. These are likely key-press/input flags. The exact key or menu that triggers the toggle is not identified; the precise missing evidence is which input binding sets DAT_0x140a54bad / DAT_0x140a54cc0. This is a user-toggled "trade view mode": when game+0x10398 is 0, FUN_140361BB0 runs; when non-zero, FUN_140373EA0 runs.

### 40.2 FUN_140361BB0 row-drawing path

FUN_140361BB0 (RVA 0x361BB0) also reads the sub-entry vector (scratch/ch_tab23_full.asm, lines 183-218):

    140361f70: MOV R8, qword ptr [R14 + 0xc650]     ; selected entry
    140361f77: ADD R8, 0x48                          ; sub-entry vector begin
    140361f7b: MOV RCX, qword ptr [R8 + 0x8]         ; end
    140361f7f: SUB RCX, qword ptr [R8]               ; end - begin
    ... divide by 0x50 -> count-1 (EAX)
    140361ff0: MOV RBX, qword ptr [R15 + RAX*1 + 0x48]  ; sub_entry+0x48 (texture)
    140361ff5: MOV RDI, qword ptr [RBX]               ; vtable
    140361fff: CALL qword ptr [RDI + 0x70]             ; draw texture
    140362010: MOVSS XMM1, dword ptr [0x140992088]    ; DPI
    140362018: MULSS XMM1, XMM10                      ; XMM10 = DAT_0x90aaa0 = 64
    ... backdrop size = 64 (same as FUN_140373EA0)
    ... panel setup at 0x140445eb0, Collision at 0x14086c850

So FUN_140361BB0 draws the same sub-entry textures with the same 64x64 backdrop and uses the same sub-entry vector and game+0xC65C. The two handlers share the data model; they differ in overall page layout (FUN_140361BB0 is the "simple" view, FUN_140373EA0 the "advanced"/button-row view). The precise layout differences (whether FUN_140361BB0 also reorders) are not decoded here; only the shared sub-entry draw is confirmed.

### 40.3 Supersession notices

- Sections 35-37 of docs/almanac-investigation.md and docs/ghidra-index/358A20.md describe the row as FUN_140358A20 with 92x92 backdrop, 85 pitch, 7px overlap, wrap-at-8. These are SUPERSEDED for the Economy & Trade row. The actual handler is FUN_140373EA0 with 64x64 backdrop, 64 pitch, no overlap, special reordering. The FUN_140358A20 analysis applies only to the (likely unreachable) "importexport" fallback path.
- docs/ghidra-index/358A20.md: status remains partial; notes now state supersession.
- docs/ghidra-index/373EA0.md: call-site corrected to 0x14031EDEF (gated by game+0x10398 != 0). The alternative (game+0x10398 == 0) calls FUN_140361BB0 at 0x14031EE13.

### 40.4 Corrected claims about image filenames

The non-existence of editor/importexport_*.png files is supporting evidence that "importexport" is a sub-entry of "trade", not proof that "importexport" cannot be a panel name. The dispatch (game+0x10398 gating) and the texture-naming convention together make "trade" the panel name. Both facts (filename absence + dispatch) are needed; neither alone is sufficient.

### 40.5 Files touched

- docs/almanac-investigation.md (this section)
- docs/ghidra-index/373EA0.md (call-site address corrected)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 41. Comparison of the two trade handlers for the Almanac button

### 41.1 Observed facts (both handlers)

Both FUN_140373EA0 (RVA 0x373EA0) and FUN_140361BB0 (RVA 0x361BB0) are reached from FUN_140317DB0 when the selected entry name is "trade". The selection between them is gated by the byte at game+0x10398 (0 -> FUN_140361BB0; non-zero -> FUN_140373EA0). Both handlers:

- Read the same sub-entry vector: begin = *(game+0xC650)+0x48, end = *(game+0xC650)+0x50, stride 0x50.
- Draw each sub-entry's texture (C3DAPI_TEXTURE* at sub_entry+0x48) via vtable+0x70.
- Use a 64x64 backdrop (DPI * DAT_0x90aaa0 = 64.0), drawn after the texture.
- Track the selected sub-tab in game+0xC65C.
- Draw a selected overlay when game+0xC65C == the current button's index.
- Consume the click latch DAT_0x140a54e91 and play gui_subtab_click.
- Use FUN_140445eb0 for panel setup and C3D_PANEL2D::Collision for hit-testing.

### 41.2 Differences observed

FUN_140373EA0:
- Special index mapping: rendered order = [sub0, sub1, sub_{count-1}, sub2, sub3, ..., sub_{count-2}]. Pitch 64 px, X origin 130. Backdrop 64x64. No overlap (64 on 64).
- Click handling: if game+0xC65C != EBX, sets game+0x1039a = 0 (dirty). Then game+0xC65C = EBX, game+0x10be8 = -1.
- Computes game+0xAE58 = (RBP-0x10) - game+0xAE50 after the call (RBP-0x10 was 0 before call, possibly modified inside).
- Content path uses game+0xC65C, game+0x12800, game+0x10be8.

FUN_140361BB0:
- Also reads the sub-entry vector and draws textures with 64x64 backdrop.
- Uses ESI as the loop counter (not EDI). The index mapping is different: ESI goes 0..count-1, but the sub-entry selection uses R15 (incremented) and a complex condition (game+0x578, game+0x5e0). The exact mapping is not decoded.
- Has additional branches: CMP game+0x578,0; CMP ESI,6; CMP game+0x5e0,0. These suggest conditional skipping or reordering based on game state.
- Content path compares game+0xC65C to 1, 2 (multiple CMP instructions) suggesting a different sub-page dispatch.
- Larger (12,304 lines of disasm) — likely a more feature-rich view.

### 41.3 What is unknown for a mod-owned button in each path

FUN_140373EA0:
- Reordering logic fully decoded? No — the EBX mapping is known but the reason (why last sub-entry goes to position 2) and any clamping are not established.
- Container width / clipping: unknown (not in this function).
- Y origin: runtime field (game+0xAE50), not statically resolvable.
- Whether an extra button at a fixed position would collide with existing buttons: pitch 64 = backdrop 64, so no overlap in the simple linear case, but the reordering means "position n" depends on count. An extra button would need its own position calculation that accounts for the reordering.

FUN_140361BB0:
- Row layout: partially traced (64x64 backdrop, sub-entry textures drawn). The full position calculation, pitch, ordering, and whether all sub-entries are drawn in one row are not decoded.
- Whether the hit-test (Collision) uses the same 64x64 rect: yes (FUN_140445eb0 + Collision).
- Available width / clipping: unknown.
- Whether FUN_140361BB0 draws ALL sub-entries or a subset: the loop runs ESI 0..count-1, but with conditional skips (CMP game+0x578, game+0x5e0). So some buttons may be conditionally hidden. This makes position prediction harder.

### 41.4 Hypotheses (labeled, not facts)

- "Simple" vs "advanced" view: HYPOTHESIS. The two handlers share the same data model and button draw. FUN_140373EA0 has a special reordering and a dirty flag; FUN_140361BB0 is larger with more branches. Whether one is "simpler" is an interpretation, not established.
- "User-toggled" view mode: HYPOTHESIS. game+0x10398 is toggled by input flags, but the specific input binding (key, menu item) is not identified. The toggle is real (observed in FUN_140309D80); the user-facing trigger is unknown.
- "Unreachable" for FUN_140358A20: HYPOTHESIS. No importexport_*.png files exist, and "importexport" is not in the dispatch list. But the absence of image files alone does not prove the panel cannot exist (it could use a different texture naming or a runtime-generated name). The dispatch and texture convention together make it unlikely, but not impossible.

### 41.5 Concise comparison

| Aspect | FUN_140373EA0 (game+0x10398 != 0) | FUN_140361BB0 (game+0x10398 == 0) |
|---|---|---|
| Sub-entry draw | Yes, 64x64, pitch 64, X origin 130 | Yes, 64x64, layout not fully decoded |
| Ordering | [sub0, sub1, sub_last, sub2, ...] | Different; conditional skips possible |
| Selection | game+0xC65C | game+0xC65C |
| Click latch | DAT_0x140a54e91 consumed | DAT_0x140a54e91 consumed |
| Content | game+0x12800, game+0x10be8 | game+0x12800 (more branches) |
| Mod button placement | Needs reordering decode + container width | Needs full layout decode + conditional logic |

### 41.6 Smallest genuine blocker

The two handlers draw the same buttons from the same data, but their layouts differ and neither's container width, clipping, or full position calculation is established. Before a mod-owned button can be placed in either path, the layout of the applicable handler must be decoded (including the reordering in FUN_140373EA0 and the conditional logic in FUN_140361BB0) and the container's visible width must be identified. The input binding for game+0x10398 does not block this; both handlers can be assessed independently.

No implementation, build, hook, configuration change, or live test is authorized.
## 42. Final layout trace for both trade handlers

### 42.1 FUN_140373EA0 (game+0x10398 != 0) — complete button layout

Verbatim geometry instructions (scratch/out/140373ea0_1403829fd_disasm.asm):

    140374114: MOVSS XMM7, dword ptr [0x140992088]          ; DPI = 1.0
    14037411c: MOVAPS XMM6, XMM7                             ; XMM6 = 1.0
    14037411f: MULSS XMM6, dword ptr [0x14090aa0c]           ; XMM6 *= 45
    140374127: MOVAPS XMM0, XMM7                             ; XMM0 = 1.0
    14037412a: MULSS XMM0, XMM11                            ; XMM0 *= 85 (XMM11 = DAT_0x90ab14)
    14037412f: ADDSS XMM6, XMM0                             ; XMM6 = 45 + 85 = 130  (X origin)
    140374138: ... (CMP game+0xc7d4, -1; JNZ skip)
    140374140: MOVSS XMM12, dword ptr [0x14090aaa0]          ; XMM12 = 64.0
    140374149: MULSS XMM7, XMM12                            ; XMM7 = 1.0 * 64 = 64  (pitch)
    14037414e: LEA RCX, [RSI + 0xadb0]                      ; panel context
    140374155: MOV R8D, 0x6                                  ; mode 6
    14037415b: MOVAPS XMM1, XMM10                            ; XMM10 = 15 + game+0xAE50 (Y origin)
    14037415f: CALL 0x1404460a0                             ; panel setup
    14037419a: TEST EDX, EDX                                ; count == 0?
    14037419c: JLE 0x1403745b6                              ; skip if no sub-entries
    1403741a2-1403741cc: EBX mapping (see section 39.2)
    1403741f6: MOVSXD RAX, EBX
    1403741f9: LEA R13, [RAX + RAX*4]
    1403741fd: SHL R13, 0x4                                 ; R13 = EBX * 0x50
    140374201: ADD R13, qword ptr [R8]                      ; R13 = sub_entry base
    140374204: MOV RCX, qword ptr [R13 + 0x48]              ; texture
    140374214: CALL qword ptr [RAX + 0x70]                   ; draw texture
    140374217: MOVSS XMM0, dword ptr [0x140992088]          ; DPI
    14037421f: MULSS XMM0, dword ptr [0x14090aaa0]          ; 64.0
    140374224: MOVSS dword ptr [0x1409be2e8], XMM0          ; backdrop width = 64
    14037422c: MOVSS dword ptr [0x1409be2ec], XMM0          ; backdrop height = 64
    140374243: MOVSD qword ptr [0x1409be2f0], XMM0          ; position (X,Y)
    1403742a4: CALL qword ptr [0x14086c850]                 ; Collision (hit test on 64x64)
    1403742f2: CMP byte ptr [0x140a54e91], CL                ; latch
    1403742f8: JZ 0x140374366                               ; skip click if no latch
    140374347: CMP dword ptr [RSI + 0xc65c], EBX             ; selection changed?
    14037434f: MOV byte ptr [RSI + 0x1039a], 0x0             ; dirty flag
    140374356: MOV dword ptr [RSI + 0xc65c], EBX             ; selected = this
    14037435c: MOV dword ptr [RSI + 0x10be8], -1
    140374366: CMP dword ptr [RSI + 0xc65c], EBX             ; selected == this?
    14037436e: MOVUPS xmmword ptr [0x1409be30c], XMM0        ; selected overlay
    1403743a8: ADDSS XMM6, XMM7                             ; X += 64 (pitch)
    1403743b6: INC EDI                                      ; next button
    1403743b8: ... (loop condition: EDI < count)

For a 4-entry collection (count=4), rendered order = [sub0, sub1, sub3, sub2]:
- Button 0 (EDI=0, EBX=0): X=130, Y=15+game+0xAE50, rect [130,194) x [Y,Y+64)
- Button 1 (EDI=1, EBX=1): X=194, Y=same, rect [194,258)
- Button 2 (EDI=2, EBX=3): X=258, Y=same, rect [258,322)
- Button 3 (EDI=3, EBX=2): X=322, Y=same, rect [322,386)

No overlap (64 backdrop on 64 pitch). Total row width: 4 * 64 = 256 px, from X=130 to X=386.

### 42.2 FUN_140361BB0 (game+0x10398 == 0) — complete button layout

Verbatim geometry instructions (scratch/ch_tab23_full.asm):

    140361f1b: MOVSS XMM7, dword ptr [0x140992088]          ; DPI = 1.0
    140361f23: MOVAPS XMM6, XMM7                             ; XMM6 = 1.0
    140361f26: MULSS XMM6, dword ptr [0x14090aa0c]           ; XMM6 *= 45
    140361f2e: MOVAPS XMM0, XMM7                             ; XMM0 = 1.0
    140361f31: MULSS XMM0, XMM9                             ; XMM0 *= 85 (XMM9 = DAT_0x90ab14)
    140361f36: ADDSS XMM6, XMM0                             ; XMM6 = 45 + 85 = 130  (X origin)
    140361f48: MOVSS XMM10, dword ptr [0x14090aaa0]          ; XMM10 = 64.0
    140361f51: MULSS XMM7, XMM10                            ; XMM7 = 1.0 * 64 = 64  (pitch)
    140361f56: LEA RCX, [R14 + 0xadb0]                      ; panel context
    140361f5d: MOV R8D, dword ptr [R14 + 0xada8]             ; count (from game+0xADA8)
    140361f64: MOVAPS XMM1, XMM15                            ; XMM15 = 15 + game+0xAE50 (Y origin)
    140361f68: CALL 0x1404460a0                             ; panel setup
    140361f70: MOV R8, qword ptr [R14 + 0xc650]              ; selected entry
    140361f77: ADD R8, 0x48                                 ; sub-entry vector begin
    140361f7b: MOV RCX, qword ptr [R8 + 0x8]                ; end
    140361f7f: SUB RCX, qword ptr [R8]                      ; end - begin
    ... divide by 0x50 -> count-1 (EAX)
    140361f9f: MOVSS XMM9, dword ptr [0x140909f58]          ; 1.0 (scale)
    140361fb0: (loop start) CMP game+0x578, 0; JZ skip      ; conditional skip
    140361fc8: CMP ESI, 6; JNZ ...                          ; conditional (ESI != 6?)
    140361fd6: (if skip conditions met, jump to 0x14036220e)
    140361fe9: MOV RAX, qword ptr [R8]                      ; sub_entry vector begin
    140361ff0: MOV RBX, qword ptr [R15 + RAX + 0x48]        ; texture at sub_entry+0x48
    140361ff5: MOV RDI, qword ptr [RBX]                     ; vtable
    140361fff: CALL qword ptr [RDI + 0x70]                  ; draw texture
    140362005: MOVSS XMM1, dword ptr [0x140992088]          ; DPI
    140362010: MULSS XMM1, XMM10                            ; 64.0 (backdrop size)
    14036201d: MOVAPS XMM2, XMM1
    140362020: LEA RCX, [0x1409be060]
    140362027: CALL 0x14086c8a8                             ; SetDimensions(64, 64)
    140362031: MOVAPS XMM3, XMM8
    140362035: MOVAPS XMM2, XMM15
    140362038: MOVAPS XMM1, XMM6                            ; X position
    14036203f: CALL 0x14086c008                             ; SetPosition
    14036204f: CALL 0x14086c8a0                             ; Draw
    140362066: CALL 0x140445eb0                             ; panel setup
    14036206b-2095: Collision, latch, click handling (same pattern as FUN_140373EA0)
    14036209b: LEA RDX, [RBP + 0xaa0]                      ; (collision rect)
    1403620a2: LEA RCX, [0x140a54b90]                      ; GetMouseSolid
    ...
    14036220e: (skip target) INC ESI                         ; next iteration
    140362214: MOV R8, qword ptr [R14 + 0xc650]             ; re-read sub-entry vector
    14036221c-225d: loop condition (ESI < count-1)

Key difference: the loop uses ESI as counter and R15 as the sub-entry pointer (incremented by 0x50 each iteration). The sub-entry index is R15/0x50 (linear, no reordering). But there are conditional skips (game+0x578, ESI==6, game+0x5e0). So some buttons may be skipped. The pitch (XMM7 = 64) is the same, but whether X is incremented by 64 per drawn button (or per iteration) depends on whether skipped buttons still advance X. From the code, XMM6 is incremented at line 14036231a (ADDSS XMM6, XMM15?) — actually let me check. Line 471: `ADDSS XMM15, XMM0` and 472: `MOVSS [RBP-0x20], XMM15`. And line 225: `MOVAPS XMM2, XMM15`. Hmm, XMM15 is Y, not X. Let me check where X (XMM6) is incremented.

Actually, looking at the loop, XMM6 starts at 130 and is likely incremented by 64 per button. But the conditional skips (JZ 0x14036220e) jump to INC ESI without drawing, so if a button is skipped, X is NOT incremented (the ADDSS XMM6, XMM7 happens only after a successful draw). So skipped buttons don't consume a slot. This means the visible buttons are packed (no gaps for skipped entries).

For a 4-entry collection with no skips: positions X = 130, 194, 258, 322 (same as FUN_140373EA0, since no reordering in this handler).

### 42.3 Window available width and clipping

Neither handler contains a container-width test or clipping rectangle. The panel context is game+0xADB0 (set by FUN_140317DB0). The absolute screen origin depends on game+0xAE50 (runtime field) and the sidebar's own layout, which is not in either handler. The clipping is handled by the D3D/render layer (C3D_PANEL2D::Draw with the panel context), not by these functions. Therefore:

- Whether X=130+4*64=386 is visible cannot be established from these handlers.
- Whether a 5th button (X=386+64=450) would fit is unknown.
- The sidebar's actual pixel width and the panel's clip rect are the exact missing evidence for container width.

### 42.4 Four-entry collection (runtime count unverified)

The runtime collection count is not statically established. The screenshot shows 4 buttons, but the code reads count from the sub-entry vector at runtime. For a 4-entry collection:

FUN_140373EA0: rendered order [sub0, sub1, sub3, sub2], positions X=130,194,258,322. No overlap.
FUN_140361BB0: rendered order [sub0, sub1, sub2, sub3] (linear, no reordering observed), positions X=130,194,258,322. No overlap. (Conditional skips may remove some.)

Runtime count and visibility are UNVERIFIED. The vector size depends on the "trade" panel's sub-entries, which are built by FUN_140296110 from a data table. The data table (game+0xC2B0, stride 0x340) contents are not traced.

### 42.5 Mod-owned button placement (no native collection extension needed)

A mod-owned button does NOT need to follow native insertion rules (reordering, conditional skips). It can be drawn at any position after the native loop, using the same coordinate origin (X=130, Y=15+game+0xAE50) and pitch (64). The mod must:

1. Read count and compute the last native button's X (requires knowing which handler ran and its ordering).
2. Place the Almanac button at the next 64-aligned slot (or at a fixed position if the layout is stable).
3. Handle clicks via GetMouseSolid + Collision on its own 64x64 rect.
4. Not touch game+0xC65C or the native vector.

The blocker is not the insertion rule but: (a) which handler runs (game+0x10398), (b) the container width/clipping, and (c) the reordering/conditional logic that determines where the last native button lands.

### 42.6 Files touched

- docs/almanac-investigation.md (this section)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 43. Final layout resolution for both trade handlers

### 43.1 FUN_140361BB0 button loop — exact X advancement

Verbatim loop (scratch/ch_tab23_full.asm, lines 199-333):

    140361fb0: (loop body start)
    140361fc0: CMP dword ptr [R14 + 0x578], 0x0          ; skip condition 1
    140361fc8: JZ 0x140361fd6
    140361fca: LEA EAX, [RSI - 0x3]
    140361fcd: CMP EAX, 0x1
    140361fd0: JBE 0x14036220e                          ; skip (jump to INC ESI)
    140361fd6: CMP ESI, 0x6                               ; skip condition 2
    140361fd9: JNZ 0x140361fe9
    140361fdb: CMP dword ptr [R14 + 0x5e0], 0x0
    140361fe3: JZ 0x14036220e                           ; skip (jump to INC ESI)
    140361fe9: (draw button: texture, backdrop 64x64, position XMM6)
    ...
    14036220a: ADDSS XMM6, XMM7                           ; X += 64 (ONLY on draw path)
    14036220e: INC ESI                                    ; skip target: advance counter only
    140362210: ADD R15, 0x50                              ; advance sub-entry pointer
    140362214: ... recompute count
    140362243: CMP ESI, EAX                               ; EAX = count-1
    140362245: JL 0x140361fc0                             ; loop if ESI < count-1

Key findings:
- Skipped entries (JZ 0x14036220e) do NOT advance X (ADDSS is before the skip target). Drawn buttons are packed (no gaps).
- Loop runs ESI = 0..count-2 (count-1 iterations). The last sub-entry (index count-1) is NOT drawn as a button in the row.
- After the loop (0x14036225d), a separate 35x35 panel is drawn (dimensions = DPI * DAT_0x90a9cc = 35). This is a different UI element, not part of the section-button row.
- No button is drawn outside the loop for the row itself.

### 43.2 FUN_140373EA0 button loop — exact X advancement (already traced)

Verbatim (scratch/out/140373ea0_1403829fd_disasm.asm, lines 159-199):

    14037419a: TEST EDX, EDX                              ; count == 0?
    14037419c: JLE 0x1403745b6                            ; skip entirely
    1403741a2: CMP EDI, 0x2                               ; EDI = loop counter
    1403741a5: JGE 0x1403741ab
    1403741a7: MOV EBX, EDI                              ; if EDI < 2: EBX = EDI
    1403741a9: JMP 0x1403741cf
    1403741ab: JNZ 0x1403741cc
    1403741ad-1403741c8: EBX = count-1                  ; if EDI == 2: EBX = count-1
    1403741cc: LEA EBX, [RDI - 0x1]                     ; if EDI >= 3: EBX = EDI - 1
    1403741cf: (draw button at XMM6, advance X by 64)
    1403743a8: ADDSS XMM6, XMM7                           ; X += 64 (after every drawn button)
    1403743b6: INC EDI                                    ; next button
    1403743b8: ... (loop condition: EDI < count)

No skip conditions — every iteration draws a button. X advances by 64 after each. Rendered order: [sub0, sub1, sub_{count-1}, sub2, sub3, ..., sub_{count-2}].

### 43.3 Window available width and clipping

Neither handler contains a container-width test, clipping rectangle, or visible-bounds field. The panel context is game+0xADB0 (a C3D_PANEL2D or similar widget). The absolute screen origin depends on game+0xAE50 (runtime field) and the sidebar's own layout, which is set during window creation in FUN_140317DB0 or earlier — not in these handlers. Clipping is performed by C3D_PANEL2D::Draw / Collision using the panel context's internal dimensions, which are not set in these functions.

Exact missing evidence: the field in game+0xADB0 (or the sidebar host) that holds the panel width/clip rect, and the instruction that initializes it. Without this, whether X=386 (or X=450 for a 5th button) is visible cannot be established.

The sidebar host (FUN_140317DB0) at lines 29, 1874, 2052, 2458, 3349, 3495, 3766, 5987, 6264, 6270 references game+0xADB0 (LEA RCX/RBX/R15, [game+0xadb0]) but only passes it to FUN_1404460a0 / FUN_140445eb0 / FUN_1404461f0. No width field is visible in these references.

### 43.4 Fifth button assessment (four-entry, scale-1)

Under the stated assumptions (4 entries, DPI=1, no skips, no reordering):
- FUN_140373EA0: last native button at X=322, right edge 386. Fifth button at X=386, right edge 450.
- FUN_140361BB0: last drawn button at X=322 (sub2, since sub3 is not drawn in the row), right edge 386. Fifth button at X=386, right edge 450.

Both handlers use the same X origin (130) and pitch (64), so the arithmetic is identical. However:
- In FUN_140373EA0, the reordering means "sub3" is at position 2 (X=258), not position 3. A mod button at X=386 would be after all 4 native buttons regardless.
- In FUN_140361BB0, if entries are skipped, the last drawn button may be at a lower X than 322 (packed layout). The 5th button position would then be lower than 386.
- The container width is unknown, so whether X=450 is visible is unverified in both handlers.

### 43.5 Smallest genuine blocker

The container's visible width and clip rect (the field in game+0xADB0 or the sidebar host that sets it). This is the single missing piece that prevents assessing whether a 5th button (X≈450) fits. It can be traced by finding the instruction that initializes game+0xADB0's dimensions (likely in FUN_140317DB0 or an earlier window-creation function). No live test is authorized.

### 43.6 Files touched

- docs/almanac-investigation.md (this section)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 44. Corrected four-entry tables and window-bounds status

### 44.1 Corrected four-entry tables

FUN_140373EA0 (game+0x10398 != 0): draws ALL count buttons with reordering [sub0, sub1, sub_{count-1}, sub2, ...]. Pitch 64, X origin 130.

| Entries (count=4) | Rendered order | Positions (X) | Rects [X, X+64) |
|---|---|---|---|
| sub0, sub1, sub2, sub3 | sub0, sub1, sub3, sub2 | 130, 194, 258, 322 | [130,194), [194,258), [258,322), [322,386) |

FUN_140361BB0 (game+0x10398 == 0): draws count-1 buttons (sub-entries 0..count-2) in linear order, skipping conditionally. Pitch 64, X origin 130. Last sub-entry (count-1) is NOT a button.

| Entries (count=4) | Loop iterations (ESI=0..2) | Drawn if no skips | Positions (X) | Rects [X, X+64) |
|---|---|---|---|---|
| sub0, sub1, sub2, sub3 | ESI=0,1,2 | sub0, sub1, sub2 | 130, 194, 258 | [130,194), [194,258), [258,322) |

If entries are skipped (game+0x578 != 0 and ESI not in {0,1,2} with ESI-3 in {0,1}; or ESI==6 and game+0x5e0==0), those buttons are omitted and X does not advance (packed layout). The 4th sub-entry (sub3) is never drawn as a button; a separate 35x35 panel is drawn after the loop.

### 44.2 Window bounds and clipping — status

The panel context (game+0xADB0) is initialized by FUN_1404465a0 (RVA 0x4465A0) during FUN_140317DB0 prologue. That function sets game+0xAE30 = 1.0 (a scale factor, written only if a condition is zero) and performs additional setup (not fully disassembled here). No width, height, or clip-rectangle field is written in the portions examined. The panel context dimensions are likely set during object construction (before FUN_140317DB0) or by a separate initialization not in the cached exports.

Clipping is performed by C3D_PANEL2D::Draw and C3D_PANEL2D::Collision using the panel context's internal dimensions, which are not set in these handlers. The exact field and instruction that set the visible bounds are NOT identified.

Exact missing evidence: the instruction(s) that write the panel width/height/clip fields of game+0xADB0 (or the sidebar host's equivalent). Candidate locations: (a) the constructor of the sidebar/window object (before FUN_140317DB0), (b) an initialization function called from FUN_140317DB0 before the trade handlers, (c) FUN_1404465a0 (partially disassembled, needs full decomp).

Proposed diagnostic (not performed): read game+0xADB0's width/height fields at runtime during a trade page render and compare with the on-screen sidebar width. This would establish the visible bounds. No live test is authorized.

### 44.3 Fifth button assessment (corrected)

Under four-entry, scale-1, no-skip assumptions:
- FUN_140373EA0: last button right edge = 386. Fifth button at X=386, right edge 450.
- FUN_140361BB0: last loop button right edge = 322 (only 3 buttons). Fifth button at X=322, right edge 386. (If a 4th button were needed, it would be at X=386.)

In both cases, whether the right edge (386 or 450) is visible is unverified because the container width is unknown.

### 44.4 Files touched

- docs/almanac-investigation.md (this section)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 45. Panel setup, window bounds, and four-vs-visible distinction

### 45.1 FUN_1404465A0 (panel setup at FUN_140317DB0 prologue)

Called at 0x140317e68: `CALL 0x1404465a0` with RCX = game+0xADB0. Verbatim (scratch/out/1404465a0_full_disasm.asm):

    1404465a0: PUSH RBX
    1404465a2: SUB RSP, 0x60
    1404465a6: MOVSS XMM1, dword ptr [0x140909afc]       ; some constant
    1404465ae: MOV R9B, 0x1
    1404465b1: MOV RBX, RCX                              ; RBX = game+0xADB0
    1404465b4: MOV qword ptr [RSP + 0x70], RDI
    1404465b9: MOVZX R8D, R9B
    1404465bd: MOVAPS xmmword ptr [RSP + 0x50], XMM6
    1404465c2: LEA RCX, [0x1409d4ee0]
    1404465c9: CALL qword ptr [0x14086c028]              ; (some init)
    1404465cf: LEA RDX, [0x1408d1f28]
    1404465d6: MOVAPS XMM6, XMM0
    1404465d9: LEA RCX, [0x1409bdfc0]
    1404465e0: CALL qword ptr [0x14086c0c0]
    1404465e6: MOV EDI, EAX
    1404465e8: TEST EAX, EAX
    1404465ea: JNZ 0x1404465f6
    1404465ec: MOV dword ptr [RBX + 0x80], 0x3f800000    ; game+0xAE30 = 1.0 (if EAX==0)
    1404465f6: ... (continues with more setup, reads game+0x80, game+0x84, game+0x8c)

This function only writes game+0xAE30 = 1.0 (a scale factor). It does not set width, height, or clip fields of game+0xADB0. The panel context dimensions are not established by this function.

### 45.2 Window bounds and clipping — final status

No instruction in the cached exports (FUN_140317DB0, FUN_140373EA0, FUN_140361BB0, FUN_1404465A0) sets a width, height, or clip rectangle for game+0xADB0. The panel context is passed to FUN_1404460a0 / FUN_140445eb0 / FUN_1404461f0, but those are setup/draw helpers whose full behavior is not cached. Clipping is performed by C3D_PANEL2D::Draw and C3D_PANEL2D::Collision using internal dimensions, but the field and instruction that initialize those dimensions are not identified.

Exact missing evidence: the constructor or initialization of the sidebar/window object that creates game+0xADB0 and sets its width/height/clip fields. Candidate locations: (a) object constructor before FUN_140317DB0, (b) a separate init function called from FUN_140317DB0 not yet identified, (c) FUN_1404460a0 / FUN_140445eb0 (not cached).

Proposed diagnostic (not executed): during a trade page render, read the panel context's width/height fields (likely at game+0xADB0+0x00 and +0x04, or +0x80/+0x84) and compare with the on-screen sidebar width. Also read game+0xAE50 to get the Y origin and verify the row position. This would establish the visible bounds. No live test is authorized.

### 45.3 Four entries vs four visible buttons

| Handler | Total sub-entries (count) | Buttons drawn in row | Last sub-entry disposition |
|---|---|---|---|
| FUN_140373EA0 | count (all) | count (no skips) | Drawn as button (reordered to position 2) |
| FUN_140361BB0 | count | count-1 (loop ESI=0..count-2) | NOT drawn as button; separate 35x35 panel after loop |

For count=4:
- FUN_140373EA0: 4 buttons at X=130,194,258,322. All 4 sub-entries visible.
- FUN_140361BB0: 3 buttons at X=130,194,258 (if no skips). Sub-entry 3 is not a button. A 35x35 panel follows at an undetermined position.

### 45.4 Corrected blockers (not just container width)

The following remain unverified and must be resolved before a mod-owned button can be placed:
1. Container width and clip rect (exact missing evidence: panel dimension initialization, see 45.2).
2. Which handler runs (game+0x10398 setter — input binding unknown, but both handlers can be assessed independently).
3. Row layout decode: reordering in FUN_140373EA0 (partially decoded), conditional skips in FUN_140361BB0 (partially decoded).
4. Content replacement: whether the body draw can be suppressed without corrupting game+0x12800 / game+0x10be8.
5. Return-to-vanilla: click detection for the already-selected button (latch ownership unknown).

Container width is ONE of several blockers, not the only one.

### 45.5 Files touched

- docs/almanac-investigation.md (this section)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 46. Panel setup helpers: no width/clip fields set

### 46.1 FUN_1404460A0 (RVA 0x4460A0)

Decompiled (scratch/out/1404460a0_decomp.c, 64 lines). Signature: `FUN_1404460a0(param_1*, float param_2, float param_3, undefined8 param_4)` with in_R8D as a mode selector.

- If `in_R8D != -1` and `in_R8D != param_1[8]`, returns immediately (mode mismatch).
- If `param_1[8] == 5` (special mode), adjusts param_2 and param_3 based on DAT_0x90c479c/0x90c47a0/0x90c47a4/0x90c47ac, with bounds checks that may return early.
- Otherwise: if `param_1[1] - *param_1 < 0x20` (small collection), calls FUN_140449c00 to append, then FUN_140001a50 to free.
- Writes `*(float *)(param_1[1] + -8) = param_2` (stores param_2 as a float in the collection's tail).
- Does NOT read or write any width, height, or clip field of game+0xADB0. It operates on param_1's internal sorted collection.

Conclusion: FUN_1404460A0 is a vertical-position/scroll helper. It does not set the row's width or clipping bounds.

### 46.2 FUN_140445EB0 (RVA 0x445EB0)

Decompiled (scratch/out/140445eb0_decomp.c, 84 lines). Signature: `FUN_140445eb0(param_1*, float param_2, undefined8 param_3, undefined8 param_4)`.

- If `param_3 == -1` or `param_3 == param_1[8]` (mode match):
  - If collection is small (< 0x20 bytes): rebuilds collection, inserts param_2 via FUN_140042de0.
  - If collection is large: searches for param_2 in a sorted float array (param_1[1]-0x20..param_1[1]-0x18), deduplicates within tolerance (DAT_0x90992088 * DAT_0x90a6a8 = 5.0), and inserts param_2 if not present. Then sorts via FUN_140468180.
- Does NOT read or write any width, height, or clip field. It is a sorted-insert/dedup helper for float arrays.

Conclusion: FUN_140445EB0 is a sorted-collection helper. It does not set the row's width or clipping bounds.

### 46.3 FUN_1404461F0 (RVA 0x4461F0)

Decompiled (scratch/out/1404461f0_decomp.c, 145 lines). Signature: `FUN_1404461f0(param_1*, float param_2, float param_3, int param_4)`.

- If `param_4 != -1` and `param_4 != param_1[8]`, returns (mode mismatch).
- If `param_1[8] == 5` (special mode), adjusts param_3 with bounds checks.
- Otherwise: searches a nested sorted collection (param_1[0] -> array of 0x20-byte structs, each with a sorted float sub-array) for param_3, and inserts param_2 into the matching sub-array (dedup within tolerance 5.0). Then sorts via FUN_140468180.
- Writes `*(float *)(param_1[1] + -8) = param_3` (stores param_3 in the collection tail).
- Does NOT read or write any width, height, or clip field. It is a nested sorted-collection insert helper.

Conclusion: FUN_1404461F0 is a nested sorted-collection helper. It does not set the row's width or clipping bounds.

### 46.4 FUN_1404465A0 (RVA 0x4465A0)

Decompiled in part (scratch/out/1404465a0_full_disasm.asm, 0x1404465A0-0x1404467A0). Writes `game+0xAE30 = 1.0` (scale factor) if a condition is zero. Reads game+0x80, game+0x84, game+0x8c. Does not write width, height, or clip fields.

Conclusion: FUN_1404465A0 sets a scale factor (game+0xAE30). It does not set the row's width or clipping bounds.

### 46.5 Summary: no width/clip field identified

None of the four helpers (FUN_1404465A0, FUN_1404460A0, FUN_140445EB0, FUN_1404461F0) set or pass a width, height, or clip rectangle for the panel context (game+0xADB0). They are all collection-management or scale helpers. The panel context dimensions are set elsewhere — likely during object construction (before FUN_140317DB0) or by a function not yet examined.

The window bounds remain untraced. The exact missing evidence is the constructor or initialization of the sidebar/window object that creates game+0xADB0 and sets its dimension fields. No live diagnostic is proposed beyond reading those fields at runtime (which requires knowing their offsets, which are not established).

### 46.6 Four vs visible (final)

| Handler | Total sub-entries | Loop iterations | Buttons drawn | Last sub-entry |
|---|---|---|---|---|
| FUN_140373EA0 | count | count | count | Drawn (reordered to position 2) |
| FUN_140361BB0 | count | count-1 (ESI=0..count-2) | count-1 (if no skips) | Not drawn as button; 35x35 panel after loop |

### 46.7 Files touched

- docs/almanac-investigation.md (this section)
- scratch/out/1404460a0_decomp.c (new cached export)
- scratch/out/140445eb0_decomp.c (new cached export)
- scratch/out/1404461f0_decomp.c (new cached export)
- scratch/out/1404465a0_full_disasm.asm (new cached export)

No production code, configuration, saves, builds, hooks, or settings were changed.
## 47. Button drawing object vs panel context, and bounds status

### 47.1 Button drawing object vs game+0xADB0

The button drawing uses `_DAT_1409be060` (a C3D_PANEL2D global widget) for Collision, Draw, SetPosition, and SetDimensions. The backdrop dimensions are stored in `_DAT_1409be2e8` (width) and `_DAT_1409be2ec` (height), both set to 64 in both handlers. The position is `_DAT_1409be2f0` (X,Y packed). The hit-test is `C3D_PANEL2D::Collision(&_DAT_1409be060, mouse, 0, 0)`.

`game+0xADB0` is a SEPARATE object passed to the setup helpers (FUN_1404460a0, FUN_140445eb0, FUN_1404461f0, FUN_1404465a0). These helpers operate on `game+0xADB0`'s internal sorted collections and scale factor (game+0xAE30). They do NOT set the C3D_PANEL2D dimensions or the button backdrop. The two objects are distinct: game+0xADB0 is a layout/scroll context, _DAT_0x1409be060 is the render/hit-test widget.

### 47.2 Container bounds: not traceable from cached exports

No instruction in the cached exports (FUN_140317DB0, FUN_140373EA0, FUN_140361BB0, FUN_1404465A0, FUN_1404460A0, FUN_140445EB0, FUN_1404461F0) sets a width, height, or clip rectangle for the panel or the sidebar window. The panel context dimensions are likely set during object construction (before FUN_140317DB0) or by a function not in the cached exports.

Clipping is performed by C3D_PANEL2D::Draw and C3D_PANEL2D::Collision using the widget's internal dimensions, but those dimensions are not set in the examined functions. The evidence does not show which function sets them.

### 47.3 What is static vs runtime

Static (from binary):
- Button backdrop: 64x64 (both handlers).
- Pitch: 64 px (both handlers).
- X origin: 130 (both handlers).
- Y origin: 15 + game+0xAE50 (runtime field).
- Reordering: FUN_140373EA0 only (static instruction-level).
- Conditional skips: FUN_140361BB0 only (static instruction-level).

Runtime (not statically resolvable):
- game+0xAE50 (Y base).
- game+0x10398 (handler selector).
- Collection count (sub-entry vector size).
- Panel/sidebar dimensions and clip rect.
- Texture dimensions (96x96 PNGs, but render scale unknown).

### 47.4 Concise result

The button drawing object (C3D_PANEL2D at 0x1409be060) is distinct from the panel context (game+0xADB0). The container's visible bounds and clip rect are not set in any examined function. The exact missing evidence is the instruction(s) that initialize the panel/sidebar dimensions. Without this, whether a 5th button (X≈386 or 450 depending on handler) is visible cannot be established. No live test was performed.

### 47.5 Files touched

- docs/almanac-investigation.md (this section)

No production code, configuration, saves, builds, hooks, or settings were changed.
