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
