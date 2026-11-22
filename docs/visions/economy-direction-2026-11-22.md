# Economy direction: owner decisions of 2026-11-22

**Status:** Design decisions. **No code or INI file has been changed yet.** This
record shows how the code works today and lists the changes needed to match the
decisions. Code citations refer to commit `d4d9628`.

**Precedence:** where this document conflicts with
[`WRSRE DESIGN DOC.md`](../WRSRE%20DESIGN%20DOC.md),
[`vision-whole-mod.md`](vision-whole-mod.md),
[`Labor_and_Salary_Overhaul_Specification.md`](../Labor_and_Salary_Overhaul_Specification.md),
[`proposal-b2b-wholesale-counterparty-settlement.md`](../proposal-b2b-wholesale-counterparty-settlement.md)
or [`engine/local-markets.md`](../engine/local-markets.md), this document wins.
Those files carry a pointer here.

**Evidence labels:** *code* = read in source at `d4d9628`; *static* =
Ghidra/static analysis already recorded in `docs/`; *external* = public game
documentation or community measurement (linked); *proposal* = not decided by
the owner yet; *hypothesis* = needs verification before use.

---

## 1. Decision register

| ID | Decision | Status |
|---|---|---|
| D1 | **Player choice of system.** The balance between state and private sector is entirely the player's choice: a purely planned public economy, a hybrid, or a capitalist-leaning economy. Nothing may force privatisation. | Decided |
| D2 | **Native prices only.** All money amounts use the game's native resource prices. The mod's market book, market multiplier and supply/demand recording are obsolete and must be removed. A future market model may *change the native prices themselves*; it must never add a second price layer. | Decided |
| D3 | **One domestic market.** Local town markets / local scarcity (old Task 2) are **cancelled**. The whole map is one domestic market until the core loop is polished. | Decided |
| D4 | **No state purchase of private output on dispatch.** The state no longer buys whatever leaves a private factory. That code is obsolete and must be removed. | Decided |
| D5 | **Companies keep their export revenue.** When a private company exports through customs, it receives the native customs proceeds directly. The money must not stay in the state treasury. | Decided |
| D6 | **State exports stay native.** Goods the state exports through customs are completely vanilla. | Decided |
| D7 | **City Hall TAX tile and fiscal panel are obsolete.** The Almanac replaces them, and they must be removed. | Decided |
| D8 | **No made-up data, ever.** No sample, synthetic, placeholder or invented fallback value may appear anywhere in the mod. Missing information or an error must be *very* obvious. | Decided |
| D9 | **Native need penalties are the poverty consequence.** A citizen who cannot pay does not get the goods. The native unmet-need mechanics then lower happiness and health. The mod does not write happiness or health for this. | Decided (static support, runtime check pending: §9) |
| D10 | **Housing choice by wealth.** | Agreed: design in §8 |
| D11 | **Decrees.** | Agreed: design in §7 |
| D12 | **Private investment fund.** Later: dividends to rich citizens, and the fund "unlocks" building types in high demand for the player to place with private construction. No autonomous building placement. | Decided (deferred) |
| D13 | **Private construction stays as it is** until the core loop is polished. | Decided (deferred) |
| D14 | **Wage indexation.** State wages can be indexed to CPI by decree. Private enterprises adjust wages to CPI automatically. | Decided |
| D15 | **No general education pay.** The premium applies only to **high-education job slots** in buildings that have two job tiers. Details in §6.2. | Decided (owner asked for a sanity check: §6.2 answers it) |
| D16 | **Live B2B billing by default:** `billing = 1`, `dry_run = 0` in `private_logistics.ini`. | Decided (prerequisites in §4.4) |
| D17 | **Politics later.** Factions, elections and approval go in a separate politics module, after the economy is right. | Decided (deferred) |
| D18 | **Welfare level is player policy:** low, medium or high, with consequences either way. The same applies to pensions and student stipends. | Decided |

---

## 2. Target money loop

```
            FOREIGN BUYERS / SELLERS (customs houses, native prices, native dumping factor)
                  ▲   │                                         ▲   │
     state exports│   │state imports (native, D6)   company      │   │ company imports
                  │   ▼                             exports (D5) │   ▼ (company pays, §4.2)
   ┌──────────────────────────────┐                ┌──────────────────────────────┐
   │        STATE TREASURY        │◄─── taxes ─────│      PRIVATE COMPANIES       │
   │                              │  (VAT, profit, │  capital, P&L, insolvency    │
   │  state enterprises are cost  │   property)    │                              │
   │  centres of the treasury     │── buys goods ─►│  sells goods it DELIVERS to  │
   │                              │  it receives   │  state sites, native price   │
   │                              │  (§4.3)        │  (B2B settlement, §4.3)      │
   └───┬───────────────────▲──────┘                └───┬──────────────────▲───────┘
       │ state wages,      │ state-shop sales          │ private wages    │ private-shop
       │ welfare/pension/  │ at the state price        │ (bidding, CPI-   │ sales (cost-plus
       │ stipend (decrees) │ level (decree)            │ indexed)         │ price, §5)
       ▼                   │                           ▼                  │
   ┌──────────────────────────────────────────────────────────────────────────┐
   │                    CITIZEN WALLETS (rent to landlords too)               │
   │  cannot pay → no goods → native unmet need → native happiness/health (D9)│
   └──────────────────────────────────────────────────────────────────────────┘
```

Money enters or leaves the republic **only at customs** (plus explicit,
labelled policy flows such as bank lending). Everything else moves between
named accounts. `state_ledger` already reconciles the money supply, and that
reconciliation is the acceptance gate for every change below (§9, invariant I5).

---

## 3. Prices: native only (D2, D3)

### 3.1 What exists today (code)

- `resource_pricing` reads the engine's live price fields (`TSM_RES_PRICE_RUB`
  `0x58`, `TSM_RES_PRICE_USD` `0x5C`, `src/game_world.h`). That part stays.
- On top of it, a monthly **market book** builds a multiplier per resource:
  `ratio = (demand − supply) / max(1, min(demand, supply))`,
  `mult = clamp(1 + 0.75·ratio, 0.35, 2.0)`
  (`resource_pricing.cpp` `RebalanceMarket`, about line 303).
- **Supply** is recorded from every private factory dispatch and from foreign
  restock (`private_shops.cpp` 642, 649, 806, 929). **Demand** is recorded only
  from citizen checkouts (`citizen_retail.cpp` 586) and from private factory
  input consumption (`private_shops.cpp` 821, 985). State production and state
  consumption are invisible to it.
- The multiplier is consumed through `TsmPricingMarketMult`
  (`src/resource_pricing_api.h` 214) in `citizen_retail.cpp` 613 and 983,
  `enterprise_base.cpp` 1985 and 2504, `private_logistics.cpp` 455 and
  `private_shops.cpp` 948.

**Consequence (code, arithmetic):** take an industrial good that no citizen
buys and no private factory consumes, such as steel delivered to state
factories. Its recorded demand is 0 and its recorded supply is whatever the
private mill dispatched. From about 0.87 t dispatched in a month the ratio
reaches the floor, so the multiplier drops to **0.35** at the next
rebalance and stays there. The mill is then paid 0.90 × 0.35 ≈ **31.5 % of the
native price** (§4.1), while its inputs cost 70 % of the native price. This
plausibly explains the owner's observation that inputs are too expensive and
sales do not cover wages. *Hypothesis: confirm with the `[PROC]` and
`market` log lines of a real session before calling it the root cause.*

### 3.2 Target

1. Every price is `native price(resource, currency)`, multiplied only by an
   explicit **policy ratio** that the player sets or that is documented as
   a balancing constant: state retail price level, private margin, VAT.
2. Remove the market book and multiplier: `recordSupply`, `recordDemand`,
   `getMarketMultiplier`, `getMarketBalance`, the `[market_balance]` and
   `[local_scarcity]` sections, and every `TsmPricingMarketMult` call. Replace
   each call with the plain native price. Bump the `resource_pricing` API
   version, because consumers must not keep linking a removed function.
3. **Fallback prices are made-up data (D8)** and must go: `default = 500.0`
   and `[fallback_prices]` / `[fallback_prices_usd]` in `resource_pricing.ini`.
   If the native price is unavailable or 0, the resource is **UNPRICED**. Any
   transaction that needs it is refused and logged, and the Almanac shows the
   resource as `UNPRICED` in warning colour. *Hypothesis to check first:
   whether a native price can really be 0 for a never-produced resource; the
   INI comment claims so.*
4. `usd_divisor = 50` (in `resource_pricing`, `citizen_labor`,
   `citizen_retail`, `enterprise_base`) is an invented exchange rate. Use the
   native USD price field wherever an amount derives from a resource price.
   Where a figure has no native USD counterpart (for example a RUB wage), the
   decision is open (§10, Q4). Until it is decided, USD mode must show that
   figure as unavailable rather than divide by 50.
5. **Base price or trade price?** The game keeps a *base* price, which
   inflates (RUB on average up to 10 %/yr, USD up to 3 %, applied every 5
   days), and a temporary customs *trade* price. The trade price is the base
   price × `1 ∓ (traded / actual threshold) × 0.05` over the last 75 days, and
   it resets to the base price when trade is neutral
   ([report #57](https://workers-resources.fandom.com/wiki/Report_for_the_community_57)).
   *To verify:* which of `0x58/0x5C` (read by `resource_pricing`) and
   `0x60/0x64` (read by the City Hall valuation, per
   [`engine/local-markets.md`](../engine/local-markets.md) Part 2 §5) is base
   and which is trade. Domestic transactions should use the **base** price.
   Company exports and imports use the **actual customs proceeds or cost**
   (§4.2).
6. **Later (not now):** a market model may adjust the *native* base prices.
   The game already does this itself: export dumping lowers the trade price
   and imports raise it (point 5).
   Native prices also already include input cost, workforce and a share of
   facility cost
   ([report #6](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Report_for_the_community_6)).
   Any mod adjustment must write into that same price, so that customs, the
   native trade screens and the mod always agree.

---

## 4. Private trade, exports and B2B settlement (D4, D5, D6, D16)

### 4.1 What exists today (code)

| Flow | Where | Rule today |
|---|---|---|
| Private factory output leaves its export storage | `private_shops.cpp` about 694–960 | Treasury pays `tons × native × marketMult × 0.90` (`procurement_price_mult`) as `"procurement"`, **whoever picks the goods up and wherever they go**. |
| Goods arrive at a private building | `private_shops.cpp` `WholesaleSlot`, about 985–1000 | Company pays `tons × native × 0.70` (`g_wholesaleRatio`) to the treasury, whoever delivered. |
| Foreign restock | `private_shops.cpp` `RestockSlot`, about 575–655 | **Writes shelf content directly** (`TSM_SLOT_CONTENT`), which teleports goods. Disabled (`foreign_restock = 0`). |
| Vehicle-provenance B2B | `private_logistics` `0.19.0-simplified-settlement`, `b2b_settlement.h` | The buyer pays the verified loading seller `qty × native × marketMult`. The buyer pays a private DO's observed trip fuel. State legs are `b2b_state_buy` / `b2b_state_sell`. **Dry run:** `billing = 0`, `dry_run = 1`. |

The two `private_shops` paths and the `private_logistics` settlement overlap.
If `billing` is switched on while `private_shops` wholesale is still on, the
same delivery is settled twice. The B2B proposal (§7) already documents this.

### 4.2 Target

1. **Delete the dispatch procurement path (D4).** Output leaving a private
   factory is not a sale. It becomes a sale when it is **delivered**: to a
   buyer (§4.3) or to a customs house (point 3).
2. **Delete the `private_shops` delivery billing at 0.70.** It sells state
   goods below their native price, which is a hidden subsidy, and it
   duplicates B2B settlement. Delivery settlement belongs to `private_logistics`
   alone.
3. **Company exports (D5).** When goods sold at a customs house came from a
   private company, the company receives the **exact native proceeds**. Today
   the native code credits the treasury (`TSM_GO_TREASURY_RUB` `0x588` /
   `TSM_GO_TREASURY_USD` `0x580`). The mod must attribute that exact amount to
   the exporting company (seller identity from the vehicle cargo lot) and move
   it from the treasury to the company as `"company_export"`. This is not a
   subsidy: it corrects which account the native sale credits. Prerequisite:
   Ghidra target **G1** (§9).
   - A delivery into a customs house must **not** also trigger `b2b_state_buy`.
     Otherwise the state pays the company once and the native sale credits
     the state again.
4. **Company imports** are symmetric. Goods a private company pulls from
   customs are charged to the company at the exact native import cost
   (including the native distance/shipping fee). Prerequisite: **G1**.
5. **State exports and imports stay native (D6).** The mod does nothing.
6. **Delete foreign restock.** It writes inventory directly, which violates
   the vision's first rule ("never teleport goods").
7. B2B goods price = **native price**, with no market multiplier and no
   wholesale factor. The current 0.19 header already says "no extra
   wholesale/margin factor". Only the multiplier remains to remove.

### 4.3 Domestic sales between private and state (*proposal: owner to confirm*)

With D4 removed, a private factory's goods delivered into a **state** building
(warehouse, factory, shop) still need a payer. Otherwise private goods become
free state goods. The consistent rule, already implemented in the dry-run
settlement, is:

- **The state pays only for goods it actually receives**, at the native price,
  on delivery (`b2b_state_buy`). It never pays on dispatch, and never for goods
  that sit unsold.
- A private buyer receiving state goods pays the treasury the native price
  (`b2b_state_sell`).
- Private to private: buyer pays seller, with no treasury involvement.
  Vertical integration (same owner) moves no money.

This keeps D1 neutral. A planned-economy player never triggers these legs,
and a hybrid player sees each one in the ledger.

### 4.4 Live billing by default (D16): prerequisites, in order

1. Remove the `private_shops` dispatch procurement and delivery billing
   (§4.2, points 1–2), so that switching billing on cannot double-settle.
2. Remove the market multiplier from `private_logistics` `UnitPrice`
   (`private_logistics.cpp` about 446–457).
3. Exclude customs houses from `b2b_state_buy` (§4.2, point 3), even before G1
   lands, so exports are not paid by the state.
4. Flip the defaults to `billing = 1`, `dry_run = 0` in `private_logistics.ini`
   and in the `g_billing` / `g_dryRun` initialisers (`private_logistics.cpp`
   about 56–57).
5. Update [§12 of the design doc](../WRSRE%20DESIGN%20DOC.md) and
   [`DO_OFFICES.md`](../private-logistics-runtime/DO_OFFICES.md). The old
   "payments remain OFF" wording becomes historical.

Known limits, unchanged: observed-assignment coverage, fuel units and exact
burn are still unproven at native level. Unpriced or uncertain events stay
**refused and visible** rather than guessed (D8).

---

## 5. Retail: private and state shops

### 5.1 Is a fixed margin multiplier wrong?

**A fixed markup is not wrong in itself.** Real shops widely use cost-plus
pricing, and Soviet state retail used fixed trade markups. What is wrong in
the current code (*code*) is:

1. **One markup for every good.** `default_markup = 2.00`
   (`enterprise_base.ini` 117) means a 50 % gross margin on bread, clothes and
   electronics alike. Real gross margins differ widely by category
   ([benchmarks](https://www.retailopstoolkit.com/benchmarks)): grocery about
   20–30 %, apparel about 50–60 %, consumer electronics about 10–25 %.
2. **The base is wrong.** The private price is the *state* checkout cost
   (`native × marketMult`, with a `min_cost` floor) × 2.0 × 1.20 VAT
   (`OnRetail`, `enterprise_base.cpp` about 3128), so about 2.4× the state
   shop price. The shop's real purchase cost (0.70 × native today, plus
   freight) plays no part in the price.
3. **It never responds to anything.** `SetMarkupImpl` exists in the API but
   nothing drives it.

One important game fact (*external*, Steam community discussions for app
784150, recorded in the 2026-11-22 review): in vanilla W&R citizens choose
shops by native logic, **not by price**. Vanilla citizens don't pay at all;
the shop price level only affects tourists.
So a higher private price does not lose customers to a cheaper shop. It
**refuses the citizens who cannot afford it** (the checkout gate), who then
go hungry or unclothed (D9). Price competition in the economic sense does
not exist natively. Pricing must therefore be judged by **refusals and
sell-through**, not by market share.

### 5.2 Private shop pricing (target)

```
shelf price = landed unit cost × (1 + category margin_c) × (1 + VAT)
landed unit cost = native price at purchase + delivered freight per unit
```

- **Category default margins** come from real retail gross margins (above) as
  *starting points*, then get calibrated in game against invariant I3 (§9).
  They live in the INI as documented balancing constants, never as hidden
  code literals.
- **Bounded adaptation**, done by the company itself each month:
  - **Lower** the margin, toward a floor at landed cost, when many customers
    are refused or stock ages on the shelf.
  - **Raise** the margin, toward a ceiling, when the shelf sells out with
    few refusals.
  - Drive this through `SetMarkupImpl`. The ceiling can be capped by a
    price-ceiling decree (§7).
- The Almanac shows each shop's margin, refusals and sell-through.

**Why private shops struggle today (code):** they sell at 2.4× the state
shop price to citizens earning 2.60–3.80 RUB/day who start with a 15 RUB
wallet. Most citizens are refused, so turnover stays low, while wages and
property tax keep running.

**Break-even check per shop type** (used by I3):

```
required margin ≥ (shop wages per day + fixed costs per day) / (landed cost of goods sold per day)
```

Goods sold per day is set by native foot traffic. That figure must be
**measured** from the checkout logs, not assumed.

### 5.3 State shops (target)

- The state shop price is a **player policy**: the *state retail price
  level* decree (§7), applied to the native price. Examples: 0 % = free
  distribution as in vanilla; subsidised; 100 % = native price; above native.
- State-shop revenue goes to the treasury (already true: `"state_store_sale"`).
  State-shop wages come from the treasury.
- In a purely planned economy the loop closes: treasury → state wages → state
  shops → treasury. The price level decides how much of the wage bill comes
  back and how many citizens can afford food.
- The checkout gate is already all-or-nothing for state shops: it refuses
  rather than taking a partial payment (`citizen_retail.cpp` about 604–678,
  *code*).

---

## 6. Labour

### 6.1 Wage anchor and balancing band (*proposal*)

A wage is viable only inside this band:

```
household floor:  CPI basket per day (at real shelf prices) + rent per day
        ≤  wage per worker-day  ≤
firm ceiling:     native value added per worker-day
                  = (output × native output price − Σ inputs × native input price) / worker-days
```

- Below the floor, workers are poor and native need penalties hit (D9).
  Above the ceiling, no firm can pay, private or state, and the state can
  only pay from export income.
- Native prices already include workforce cost (*external*, report #6), so
  the ceiling should normally sit above the floor. The job is to **measure
  it** and put the defaults inside the band.
- *Hypothesis H1:* the game's construction phases carry a `"workers"`
  pseudo-resource for workdays (`private_construction.cpp` about 860). If that
  pseudo-resource has a native price, it is the natural anchor:
  `wage = policy ratio × native workday price`. Then wages inflate with native
  prices and stay consistent with them. To verify, log
  `getByName("workers")` prices (target **G5**).
- Community measurement (*external*,
  [Steam](https://steamcommunity.com/app/784150/discussions/0/3838802451901886483/)):
  in 1960s prices a worker needs about 0.21 RUB of food and meat per workday,
  and at most about 1 RUB of goods in total. Compare that with today's mod
  wages of 2.60–3.80 RUB/day and the CPI basket of `resource_pricing`. It is a
  reference point for calibration, not a value to hard-code.

### 6.2 Two-tier pay for high-education slots (D15): sanity check

**Yes, the owner's rule matches how the game works.** The game assigns
education to *job slots*, not to people:

- Building scripts declare `$WORKERS_NEEDED` (basic slots) and, where
  needed, `$PROFESORS_NEEDED` (high-education slots, meaning doctors,
  teachers, priests and so on)
  ([modder documentation](https://steamcommunity.com/sharedfiles/filedetails/?id=1885817861)).
  A hospital, for example, has 30 workers plus 30 high-education workers
  ([official wiki](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Hospital)).
- University-educated citizens can work in basic slots but prefer their own
  slots
  ([Steam](https://steamcommunity.com/app/784150/discussions/0/3812911928389549279/)).

So pay follows **the position held**, as in real life:

- A university-educated worker in a mine's basic slot earns the basic wage.
- In a building that defines **both** slot types, the high-education slot
  earns `basic wage × (1 + premium)`. A doctor earns more than the cleaner.
- Whether a given factory (vehicle production, for example) has
  high-education slots is decided **by its blueprint**, not by us. The rule
  applies automatically wherever `$PROFESORS_NEEDED > 0` together with
  `$WORKERS_NEEDED > 0`. Buildings with only high-education slots (for
  example a small secret police building) just use that wage.
- **State buildings:** the player sets the premium per sector, with a
  per-building override, like the existing wage hierarchy.
- **Private buildings:** the company bids separately for high-education slots
  when they are understaffed (the same bidding rule as basic slots).
- Prerequisite: Ghidra target **G4** (slot counts on the building and which
  slot a worker occupies). Until it is available the premium is **inactive and
  shown as `UNAVAILABLE`**. It is never guessed (D8).

The old "skill premium" wording (root README, Almanac sample row
"Skill premium (skilled) +18%") is withdrawn. There is no general education or
skill premium.

### 6.3 Indexation (D14)

- **State wages, welfare, pensions and stipends:** indexed only if the player
  enacts the *wage indexation* decree (§7). The index is the CPI basket at
  native prices, month over month. The basket already picks up native
  inflation.
- **Private wages:** firms index automatically. The private reference wage
  is `sector baseline × CPI index`, then ± profit/understaffing bidding. Today
  private wages derive from the *state* sector table, so they would freeze
  with it if the state does not index; that link must be cut.
- Today `citizen_labor` never reads the CPI (*code*).

### 6.4 Social transfers (D18)

- Welfare, pension and stipend levels are player decrees (low / medium / high,
  or a slider) as a ratio of the CPI basket or of the minimum wage. Which base
  to use is open (§10, Q3). The consequences come from the economy itself:
  treasury cost on one side, poverty and native need penalties on the other.
- Today (*code*) the ratio keys override the old absolutes:
  `welfare_ratio_to_min = 0.40`, `stipend = 0.60`, `pension = 0.80` of
  `minimum_wage = 2.50`, so 1.00 / 1.50 / 2.00 RUB/day. Those sit below the
  CPI basket, which makes "low welfare" the current *default*. Under D18 that
  must become a visible player choice.

---

## 7. Decrees (D11): economic set first

The decrees build on controls that already exist (VAT, profit tax, property
tax, bank rate, wages). Discrete policies become decrees, shown and enacted in
the Almanac. Each decree has a **visible running cost or revenue** and a
**visible consequence**. The Tropico 6 edict model is the reference for the
pattern
([Tropico 6 edicts](https://tropico.fandom.com/wiki/Edicts_(Tropico_6))):
"Food for the People", "No Free Lunch", "Free Housing", "Social Security",
"Tax Cut". Faction effects are **left out** until the politics module exists
(D17), but the decree record keeps a slot for them.

*Proposed initial list (owner to confirm or trim):*

| Decree | Levels | Effect | Cost or revenue |
|---|---|---|---|
| Welfare level | low / medium / high | Unemployed transfer (§6.4) | Treasury per recipient-day |
| Pension level | low / medium / high | Retiree transfer | Treasury |
| Student stipend | low / medium / high | Student transfer | Treasury |
| Wage indexation (state) | off / on | §6.3 | Treasury grows with CPI |
| Minimum wage | ratio to CPI | Floor for all wages | Paid by employers |
| State retail price level | free / subsidised / native / above native | §5.3 | Treasury forgoes or earns revenue |
| Price ceiling (per staple) | off / on | Caps the private margin on food (§5.2) | Private profits |
| Rent control | off / capped | Caps private rent | Landlord income |
| High-education premium (state) | per sector | §6.2 | Treasury |

The existing fiscal sliders stay as continuous controls in the Almanac
(D7 removes the City Hall copy).

---

## 8. Housing choice by wealth (D10)

The building blocks already exist (*code/static*):

- The native living-standard / wealth float is `person + 0xE4` (0..1,
  maintained by the game; `TSM_PERSON_WEALTH`).
- Attractiveness-based rent and the eviction/relocation engine use native
  `FUN_14083CA50`, with an anti-despawn guard (design doc §3).
- Living-standard strata come from `citizen_retail` (design doc §6.2).

**Target:** turn relocation into **choice**. When a household's affordability
changes (a stratum change, or rent above a share of income), it searches for
a flat priced for its stratum:

- Destitute and Low → state or cheap housing.
- Affluent → high-attractiveness private housing.

The move is voluntary, with no eviction penalty, and only into vacant flats
(the same anti-despawn rule). Rent tiers follow attractiveness and dwelling
quality as today. The relocation-search radius and cadence are open (§10, Q5).

---

## 9. How to balance: method and invariants

**Order of work:**

1. **Remove distortions:** market multiplier and book (§3), dispatch
   procurement and 0.70 delivery billing (§4), foreign restock, fallback
   prices and invented divisors (§3.2, D8).
2. **Measure:** log native prices (including the H1 workday price), shop
   foot traffic and refusals, per-firm P&L (`enterprise_base` monthly gross
   sales, wages and wholesale costs), and the `state_ledger` CSV, on a
   reference save.
3. **Compute the band** (§6.1) per sector and the break-even margin (§5.2)
   per shop type, from **captured** data. Fixtures must name their source
   log and date. They are never invented (D8).
4. **Set defaults as ratios** to native prices or the CPI: wages, transfers,
   margins, state price level. Avoid absolute RUB numbers.
5. **Encode the invariants** as portable tests against the captured fixtures,
   then iterate.

**Invariants (acceptance gates):**

| ID | Invariant |
|---|---|
| I1 | Default minimum wage ≥ CPI basket + daily rent (household floor). |
| I2 | For each producer sector at default wages: native value added per worker-day ≥ wage (firm ceiling). A private firm that is fully staffed and fully supplied, selling at native prices, is profitable before tax. |
| I3 | For each private shop type at the default category margin and measured foot traffic: gross margin ≥ wages + fixed costs. |
| I4 | At default decrees, a reference planned-economy republic's treasury is sustainable from exports + state-shop sales + taxes. The player may choose deficits, but defaults must not force one. |
| I5 | **Money conservation:** the sum of wallets, company capital, investment fund and treasury changes only by customs flows and explicitly labelled policy flows. `state_ledger` reconciliation shows zero unexplained delta. |
| I6 | **No invented values:** every number displayed or charged traces to a native field, a player policy or a documented INI constant. Anything else shows `UNAVAILABLE` / `ERROR`. |

**Runtime check for D9 (static support exists):** the checkout gate skips the
native "demand satisfied" write
(`0x14017263F`, [Retail Purchase Block Investigation](../Retail_Purchase_Block_Investigation.md)),
so the demand stays open and the native hunger/health/happiness tick runs.
Still to do: in game, confirm that a broke citizen's happiness and health
fall over the following days.

### 9.1 Ghidra queue

Owner question: "Do we need Ghidra to find loyalty, health and productivity?"

- **For the economic loop: no.** D9 relies on native mechanics. Only the
  runtime check above is needed.
- **Yes, for showing them in the Almanac and for the later politics module.**
  Today only happiness (`0xE8`; block `0xE0..0xF4`) and wealth (`0xE4`) are
  mapped (`src/game_world.h`, *static*). Loyalty is a native citizen
  property: monuments, propaganda and the secret police reveal it
  ([fandom](https://workers-resources.fandom.com/wiki/Question_and_answers)).

| ID | Target | Needed for | Priority |
|---|---|---|---|
| G1 | Customs sale/purchase event: the site where the native code credits or debits treasury `0x588`/`0x580` for a delivered/received load, the amount, and the vehicle/cargo provenance available there | D5 company exports and company imports | **High** |
| G4 | Building `$WORKERS_NEEDED` / `$PROFESORS_NEEDED` counts (`0xDEC` is the basic max) and which slot each worker in `0xBC0` occupies; the person education field | D15 two-tier pay | High |
| G5 | Native price of the `"workers"` pseudo-resource (H1). May need only a runtime log, not Ghidra | §6.1 wage anchor | High (cheap) |
| G2 | Person health, loyalty and productivity fields | Almanac readouts, housing choice, politics module | Medium |
| G3 | Native price recompute site (to later write market-adjusted native prices) | Future market model (D2) | Deferred |

Follow [`docs/AGENTS.md`](../AGENTS.md): one Ghidra operator, index each
result in `docs/ghidra-index/`, and keep bounded raw exports in
`docs/ghidra-exports/`.

---

## 10. Open questions for the owner

| ID | Question |
|---|---|
| Q1 | Confirm §4.3: the state pays the native price for private goods **delivered into state buildings**, and private buyers pay the state for state goods. |
| Q2 | Starting capital (10 000 per privatised building) and the 5 000 000 starting investment fund are created from nothing (`enterprise_base.ini`). Keep them, or fund privatisation from the treasury or a buyer? This conflicts with I5 if kept unlabelled. |
| Q3 | Base for social transfers and the minimum wage: ratio of the CPI basket or of the minimum wage? |
| Q4 | USD mode: how should RUB-only figures (wages, rent) be denominated, now that the invented `usd_divisor = 50` must go? |
| Q5 | Housing choice: search radius and how often a household may move. |
| Q6 | Confirm or trim the decree list in §7. |

---

## 11. Change list (follow-up tasks, not done yet)

Each item is a separate engineering task with its own task record
(`docs/tasks/TEMPLATE.md`).

1. **resource_pricing:** remove the market book and multiplier, fallback
   prices and `usd_divisor`. Missing native price means `UNPRICED`. Bump the
   API version. Keep the CPI basket.
2. **Multiplier consumers:** `citizen_retail.cpp` 613 and 983,
   `enterprise_base.cpp` 1985 and 2504, `private_logistics.cpp` 455,
   `private_shops.cpp` 948. Switch them to the plain native price.
3. **private_shops:** delete the dispatch procurement (`"procurement"`,
   `procurement_price_mult`, `procure_fallback`), the 0.70 delivery billing
   (`wholesale_ratio`) and foreign restock. Keep pub service charges.
4. **private_logistics:** exclude customs from `b2b_state_buy`. Defaults
   `billing = 1`, `dry_run = 0`. After G1: company export revenue and company
   import cost.
5. **enterprise_base:** private shop price = landed cost × (1 + category
   margin) × (1 + VAT), with bounded monthly adaptation through
   `SetMarkupImpl`. Remove the single `default_markup`. Set
   `all_enterprises_private = 0` and `auto_private_types` back to the
   shipped default, because auto-privatising every DO contradicts D1 now that
   `ui_panels` ≥ 6.7 provides the DO PRIV tile.
6. **citizen_labor:** CPI indexation (decree for state, automatic for
   private), transfers as player decrees, and the high-education premium
   after G4.
7. **citizen_retail:** state shop price × state retail price level decree.
8. **taxes / republic_bank / ui_panels:** remove the City Hall TAX tile,
   fiscal panel, macro rows and bank-rate slider (D7). The Almanac is the only
   policy surface. Move any readout that exists only on City Hall (CPI,
   poverty rate, commercial solvency) into the Almanac first.
9. **almanac_overlay_dev:** delete the synthetic sample rows
   (`almanac_panel_view.cpp` `kEconomyRows` / `kLabourRows` /
   `kLogisticsRows`, about 60–95) and the `DrawTable` fallback (about 659).
   With no live data, show a full-panel **"NO DATA: <service> unavailable"**
   in warning colour. Per row, show `UNAVAILABLE` / `ERROR <reason>`, never a
   number. Update `TESTING.md`, which still expects `SYNTHETIC` values.
10. **Decrees** (§7) and **housing choice** (§8) as new features, after items
    1–9.
11. **Calibration** (§9) with captured fixtures and invariant tests I1–I6.
