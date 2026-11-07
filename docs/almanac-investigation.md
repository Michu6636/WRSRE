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

### 20.1 Selection path

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

Established: Economy & Trade's selected name is not handled by the "system", "storage", "infrastructure", "building", "workshopbuildings", "editorbuildings", "timelapse", "notifi_tab", "objectives", or "scripts" branches. It reaches `FUN_140395C90` as the fallback page handler. The widget call signatures and draw context are statically understood.

Unknown: whether `FUN_140395C90` is a safe host, where its content area is, how clipping/scrolling/close work, and whether widgets can receive input there.

Smallest next action: either a deeper static trace of `FUN_140395C90`'s draw/input paths or a separately approved live test of the fallback page. No implementation or hook is authorized.

## 21. Economy & Trade host assessment

### 21.1 Path verification

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
- This shows the native widgets can be called from this function in at least some paths.

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

Established: Economy & Trade likely reaches `FUN_140395C90` as a fallback handler, and that handler can call native widgets in some paths.

Unknown: whether `FUN_140395C90` is a safe host, where its content area is, how clipping/scrolling/close work, and whether widgets can receive input there.

Smallest next action: a deeper static trace of `FUN_140395C90`'s use of the selected entry and `[game+0xD428]`, or a separately approved live test. No implementation or hook is authorized.
