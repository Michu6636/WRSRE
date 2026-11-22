# Economy direction: owner decisions of 2026-11-22

**Status:** Design decisions, updated the same day with the owner's follow-up
answers (D19–D31), and the step-2 answers (D32–D34). Code step 1 (§11) is implemented in PR #100. Everything
else in §11 is still to do. **For the ordered plan of what to do next, read
[`ROADMAP.md`](../ROADMAP.md).** This record shows how the code worked and
lists the changes needed to match the decisions. Code citations refer to
commit `d4d9628` unless a section says otherwise. Line numbers move, so
search for the names.

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
| D18 | **Welfare level is player policy**, enacted as **one Tropico-style decree** with four tiers (None / Low / Medium / High) covering the unemployed, students and pensioners. Kept simple. | Decided (tiers approved; §6.4) |
| D19 | **The state pays the native price for private goods delivered into state buildings**, and private buyers pay the state the native price for state goods (§4.3). | Decided (follow-up) |
| D20 | **No money is created from nothing.** Every amount comes out of a named account. Privatised buildings get their starting capital **from the private investment fund**. The only exception is the fund's **founding capital, created once per map** and never again. | Decided; design in §4.5 |
| D21 | **No exchange rate.** In dollar mode resources use the native dollar price, and everything else is derived the same way as in rubles. | Decided (follow-up); consequences in §3.2 point 4 |
| D22 | **The starting decree list (§7) is accepted** for now. | Decided (follow-up) |
| D23 | **Housing uses the game's own flat check.** The native flat search (used today for 21-year-olds leaving home; new arrivals and demolition victims are also placed natively) is also triggered for ordinary established households. No parallel mod search. | Decided; §8 |
| D24 | **The fund pays the state for a privatised building**, based on its native construction cost, a little below it. No price when the building was built privately. | Decided; §4.5.3 |
| D25 | **Citizen wallets start at 0.** A player decree may pay new citizens a grant from the treasury. | Decided; §4.5, §7 |
| D26 | **Fund income:** company profit above the legal reserve, and rich citizens' savings above the line that keeps them in the Affluent class. | Decided; §4.5 |
| D27 | **Nationalisation is a purchase by the treasury:** the state pays the fund for the building. (Replaces the owner's earlier age-rule remark, which is withdrawn.) | Decided; §4.5.6 |
| D28 | **Toggle cooldown:** after a building is privatised or nationalised, the PRIV button is locked for a countdown on the sim clock (about one month). | Decided; §4.5.6 |
| D29 | **Starting capital differs per building**, by size and kind: it is computed from the building's own staff and inputs. | Decided; formula approved (§4.5.7) |
| D30 | **On nationalisation the company's leftover money returns to the fund**, and an overdraft is covered by the fund. | Decided; §4.5.6 |
| D31 | **Founding capital must not depend on population.** A new map may have nobody living on it, so the amount comes from native prices only. | Decided; method approved (§4.5.1) |
| D32 | **Unpriced shop sale: native give, no charge, loud error.** When a shop good has no native price (in practice an unproduced mod good, or the moments after a load), the game hands out the good as in vanilla and no money moves. It is logged as `ERROR UNPRICED` and counted for the Almanac. Owner answer 2026-11-22. | Decided; code step 2 |
| D33 | **Unpriced business bill: refuse the whole transaction, loud error.** A private construction estimate, wholesale delivery bill or B2B freight bill that includes a resource with no native price is refused as a whole, never billed partly. For example, private construction does not claim the site. Owner answer 2026-11-22. | Decided; code step 2 |
| D34 | **CPI unavailable: pause what depends on it.** When the cost-of-living basket cannot be computed (food, meat or clothes has no price), wealth-class demand scaling is skipped that day, the runway and CPI show `UNAVAILABLE`, and the bank refuses new credit until CPI is available again. Owner answer 2026-11-22. | Decided; code step 2 |

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
4. **No exchange rate (D21).** `usd_divisor = 50` (in `resource_pricing`,
   `citizen_labor`, `citizen_retail`, `enterprise_base`) and every other
   RUB→USD conversion are removed. In dollar mode every resource uses the
   native dollar price field (`0x5C`).

   Every other amount must therefore be a **ratio of something native-priced**,
   so it comes out correctly in either currency with no conversion:
   - wages, transfers and the minimum wage: ratios of the CPI basket (which is
     computed from native prices in the active currency);
   - margins: ratios of landed cost;
   - shop and B2B prices: native prices.

   This is what the owner described: switching currency only changes the
   nominal numbers. Everything computed from native prices already behaves
   that way, because the game keeps a separate RUB field (`0x58`) and USD
   field (`0x5C`) per resource, each with its own inflation (RUB up to
   10 %/yr, USD up to 3 %), so no single conversion factor exists.

   The exceptions are amounts **typed into the mod's INI files as plain
   numbers**. `residential_rent = 10` means 10 rubles in ruble mode, and in
   dollar mode the same 10 would be read as 10 dollars, which is far more
   relative to dollar goods prices (the mod's own divisor of 50 implies
   dollar prices are roughly 50 times smaller). Today the mod hides this by
   dividing by 50, the exchange rate D21 rejects. Nothing in the design
   breaks; it is cleanup work: each typed amount is rewritten as a ratio of a
   native-priced base, and then it scales by itself. The typed amounts are:
   - `residential_rent`, `starting_capital`, `legal_reserve_flat`,
     `starting_pool` (`enterprise_base.ini`);
   - `property_tax_flat = 250` (`taxes`);
   - `min_cost` (`citizen_retail`);
   - the pub service fee (`private_shops`);
   - `starting_wallet` (`currency`);
   - the 13.30 USD contractor fallback (`private_construction`);
   - `credit_limit` (`republic_bank`; already scaled by the CPI basket against
     `reference_cpi_rub = 2.15`, so it only needs the USD path checked).

   Candidate bases are the CPI basket (for household-sized amounts) and the
   building's native construction cost from its phase records (for
   building-sized amounts such as starting capital and property tax). Until a
   figure has a base, it is shown and treated as `UNAVAILABLE` (D8). It is
   never converted.
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

### 4.3 Domestic sales between private and state (decided: D19)

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

### 4.5 No money from nothing (D20, D24, D25, D26)

**Today (*code*), money is created from nothing in these places:**

| Where | What |
|---|---|
| `enterprise_base` `ApplyPrivateState` (about line 3615) | A newly privatised building gets `starting_capital` (10 000). Nothing pays for it. |
| `enterprise_base` `starting_pool` | The investment fund starts a new game with 5 000 000, from nowhere. |
| `enterprise_base` load path (about line 4285) | An unreadable saved fund balance is reset to `starting_pool`, so the fund can be re-created on any bad load. |
| `currency` `starting_wallet` | Every new citizen wallet opens with 15, from nowhere. |

**Target money flows for the fund:**

```
           founding capital (once per map)
                      │
                      ▼
  company profit ──► INVESTMENT FUND ──► privatisation: building price ──► TREASURY
  above reserve       ▲        │                       + starting capital ──► NEW COMPANY
                      │        └──► (later, D12) dividends ──► rich citizens
  rich citizens' savings
  above the Affluent line
```

#### 4.5.1 Founding capital: once per map

- The fund receives its founding capital **once per map**: at the start of a
  new game, or the first time the mod sees a save that has no fund record
  (the mod installed mid-game). It is the only money creation in the mod and
  is booked in the ledger as `"fund_founding_capital"`.
- A persisted "founded" flag makes it impossible to repeat. An unreadable
  fund balance on load is shown as `ERROR` in the Almanac and the fund is
  frozen; it is **never** reset or re-founded.
- **How much (D31, approved).** It cannot depend on population (a new map
  may be empty) or on the CPI basket (which is priced per citizen). It must
  be stable, the same for the same map start, and correct in either currency
  (D21). Recommended: a **reference bundle of construction materials** at
  native prices:

  ```
  founding capital = Σ over the bundle: amount (tons) × native base price
  ```

  - The bundle is a documented INI list, for example steel, concrete,
    bricks, prefab panels, gravel, asphalt. Amounts are calibrated once from
    **measured** native construction costs (`NativeCostForBuilding`
    `[COST]` logs) so that the fund can privatise a set number of typical
    buildings, suggested about 10. They are never guessed: until the
    calibration has been captured, the INI ships empty and the Almanac shows
    the fund as `NOT FOUNDED: calibration missing`.
  - **Why construction materials:** the fund's biggest spending is building
    prices, which are native construction costs (§4.5.3). A bundle of the
    same materials tracks those costs as prices change between a 1960 start
    and a 1990 start, in either currency, with no population and no exchange
    rate.
  - **Price used:** the native *base* price, not the temporary customs trade
    price (§3.2 point 5), so a burst of exports in the first days does not
    change the amount. Which field is base is still to be verified (§3.2
    point 5).
  - Priced **once**, at the moment of founding, then stored. Later
    inflation does not top it up.
  - **Later improvement:** "N times the native construction cost of a
    reference building type" says the same thing more directly. It needs the
    mod to read the cost of a building type nobody has placed yet, which
    needs the game's building-type catalogue (G9).
  - **Rejected alternative:** a share of the game's starting treasury. It
    would scale with difficulty, but it breaks in unlimited-money games and
    when the mod is installed mid-game. And reading the original starting
    amount from the save is not mapped.

#### 4.5.2 Fund income (D26)

Two sources, both already half-present in the code. Suggested player-facing
wording in quotes:

1. **Company profit above the legal reserve.** *"Each month, private
   companies send part of their profit above their legal reserve to the
   investment fund."* This already exists (*code*): `siphon_rate = 0.50` of
   capital above the reserve, where reserve = `legal_reserve_flat` + 1.5
   months of wages + 1.5 months of purchases (`enterprise_base.ini`
   `[investment_pool]`). Only `legal_reserve_flat` (5 000) needs a
   native-priced base (D21).
2. **Rich citizens' savings.** *"Wealthy citizens invest part of the savings
   they don't need to stay wealthy."* The Affluent class already has an
   exact definition (*code*, `citizen_retail.ini`): a citizen is Affluent
   when their wallet covers at least `runway_affluent_days = 30` days of the
   CPI basket. So the rule is:

   ```
   investable   = wallet − (runway_affluent_days + safety_days) × daily basket
   contribution = invest_rate × investable      (only if investable > 0)
   ```

   With, for example, `safety_days = 10` and `invest_rate = 10 %` per month,
   an Affluent citizen always keeps at least 40 days of basket, so **the fund
   alone can never push them out of the Affluent class.** Both numbers are
   proposals for calibration. Ledger reason: `"household_investment"`.

Later (D12), dividends flow back to those citizens, which closes the loop.

#### 4.5.3 Privatisation price (D24)

A privatisation moves two amounts out of the fund:

1. **The building price, to the treasury** (`"privatisation_sale"`).
2. **The new company's starting capital, to the company**
   (`"privatisation_capital"`).

If the fund cannot afford both, the PRIV button says so and nothing happens.

**Price formula (*proposal*):**

```
price = native construction cost × condition × sale discount
```

- **Native construction cost** is already computed by the mod:
  `private_construction` `NativeCostForBuilding` (about line 914) walks the
  building type's native construction phases (materials and the "workers"
  workdays) and prices them at native prices. Its labour part still uses the
  INI `contractor_wage_per_day`; that should become the native workday price
  (G5).
- **Condition**: if the game keeps a building wear or condition value, an
  old, worn building sells for less. The field is not mapped yet (G7).
  Until it is, the factor is 1.0 and the Almanac says that condition is not
  taken into account. Nothing is guessed.
- **Sale discount**: a single documented setting, suggested **0.75**. Real
  privatisations rarely fetched full value: Soviet 1991 rules debated
  depreciated book value versus market value, large industry was sold at a
  "discounted market level", and insiders often bought at a discount
  ([World Bank, Privatization in the Soviet Union](https://www.researchgate.net/publication/23550783_Privatization_in_the_Soviet_Union_the_beginnings_of_a_transition);
  [Estonia](https://www.researchgate.net/publication/380237052_PRIVATIZATION_OF_STATE-OWNED_ENTERPRISES_IN_ESTONIA_POLICIES_AND_IMPLEMENTATION)).
  A discount also leaves the buyer a reason to buy.

**When no price is paid:** buildings built by private construction. The
fund already paid for them, so nothing goes to the state.

The earlier age rule is withdrawn by the owner. Abuse is handled by
symmetric prices and a cooldown instead (§4.5.6).

#### 4.5.4 Citizen wallets (D25)

- New citizen wallets open at **0** (`starting_wallet = 0`). Until the first
  wage or transfer they cannot buy, and native need penalties apply (D9).
- An optional **Settlement grant** decree (§7) has the treasury pay each new
  citizen a grant, sized in days of the CPI basket (`"settlement_grant"`).

#### 4.5.5 Other rules

- An insolvency buyout already works this way: the fund pays 1.5× the
  starting capital to restore a distressed company (design doc §5).
- Bank credit to the fund stays possible later. It is treasury money lent
  out (`"bank_loan_issue"`), so it is not creation.

#### 4.5.6 Nationalisation and the toggle cooldown (D27, D28)

**Nationalisation** is the mirror of privatisation:

1. **The treasury pays the fund the building price**, using the same formula
   as the sale (§4.5.3), at today's native prices and condition
   (`"nationalisation_purchase"`).
2. **The company's remaining capital goes back to the fund (D30)**, because
   it was the fund's money (`"nationalisation_capital_return"`). If the
   company is in overdraft, the fund covers the debt
   (`"nationalisation_debt_cover"`). Today the code keeps the capital parked
   on the building record and hands it back on re-privatisation
   (`enterprise_base` `ApplyPrivateState`, about line 3662), so that money
   sits outside every account.

   Why this is the right choice:
   - **Conservation (I5):** the money stays in a real account instead of
     sitting on a building nobody owns.
   - **Fair to the state:** the treasury paid for the *building* only. If
     the state also kept the cash, nationalising a cash-rich company would
     be a way to seize fund money for less than its worth.
   - **No double pay on re-privatisation:** a later privatisation gets fresh
     starting capital from the fund by the §4.5.7 formula, rather than
     stale parked money.
   - **Real-world parallel:** in a nationalisation with compensation, the
     owners are paid for the business, which includes its cash. Returning
     the cash to the owner (the fund) is the simple version of that.

**Why symmetric prices matter:** if selling and buying back use the same
formula, a round trip moves no money (apart from real changes such as wear
or inflation). The player cannot pump money from the fund to the treasury or
back by clicking.

**Cooldown (D28):** the cooldown stops rapid flipping, for example
nationalising a company only for its loss-making months.

- After every privatisation or nationalisation, the PRIV button on that
  building is locked for `toggle_cooldown_days`, suggested **one in-game
  month** (`sim_clock` `getDaysPerMonth`).
- The button shows the countdown, for example *"Can change ownership again
  in 12 days"*.
- The last toggle date is saved per building (absolute day from `sim_clock`
  year and day), so saving and loading does not reset it.
- An unreadable saved date shows `ERROR` and keeps the button locked. It is
  never treated as "cooldown over".
- Optional, owner to decide later: also start the countdown when a new
  building is completed.

#### 4.5.7 Starting capital per building (D29, approved)

A pub and a steel mill should not start with the same 10 000. The real-world
model is the Soviet **normed working capital**: the planners gave each
enterprise its "own working capital" equal to a norm in days times its
one-day need for wages and materials, set per industry and revised with the
production plan
([NBER, The Role and Structure of Credit](https://www.nber.org/system/files/chapters/c4158/c4158.pdf)).
The same rule fits the mod, using only native data:

```
daily wages   = basic slots × basic wage + university slots × university wage
daily inputs  = Σ over the recipe's inputs:
                consumption per worker-day × worker slots × native price
                (shops: one fill of their sales storage at native prices, spread
                over norm_days)
starting capital = norm_days × (daily wages + daily inputs)
```

- **Worker slots** are native: `TSM_BLD_MAX_WORKERS` (`bld+0xDEC`) is
  already read by `enterprise_base`. The split into basic and university
  slots needs G4.
- **Wages** come from `citizen_labor` (sector rate, and the two-tier premium
  from §6.2).
- **Inputs** come from the building's native recipe (`$CONSUMPTION` lines,
  for example the fabric factory's plants, chemicals and water). The units,
  believed to be tons per worker-day, and where the game keeps the recipe in
  memory need checking (G8). A building with no inputs (a farm, a mine)
  needs wages only.
- **`norm_days` = 45**, the same 1.5 months the company's legal reserve
  already uses (`payroll_buffer_months = 1.5`,
  `wholesale_buffer_months = 1.5`). Then **starting capital equals the
  legal reserve**: a new company starts with exactly what it is required to
  keep, so the monthly fund siphon does not take it straight back. The flat
  `legal_reserve_flat = 5000` is replaced by the same formula, which also
  settles its D21 re-basing.
- It scales with the currency by itself (D21), because wages and inputs are
  native-priced.
- If any part cannot be read, the Almanac shows the starting capital as
  `UNAVAILABLE` with the missing part named, and privatisation is refused
  until it is fixed. Nothing is guessed (D8).

**Illustration only** (not data): a pub with 5 workers and no recipe inputs
needs 45 days of 5 wages. A factory with 200 workers needs 45 days of 200
wages plus 45 days of its raw materials, so it starts with 40 times more
for wages alone, plus its inputs. The insolvency buyout (1.5× starting capital, design doc §5)
then also scales per building.

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
- **Code check 2026-11-22** (`enterprise_base.cpp` `OnPayroll` and
  `EvaluateWageBidding`, `citizen_labor.cpp`; owner log of the PR #100
  build):
  - **It runs.** Private payroll is debited from company capital (for
    example `[WAGE] ... type 6 gross 18.00 x 1.00 = 18.00` then
    `[CAP] ok ... cat=3 by=wage`). The wage-mobility hook is installed, so a
    higher wage does make a job more attractive to citizens
    (`0.35 × ln(wage / minimum)`, clamped −30 % … +60 %).
  - **Formula today:** private daily wage = state rate for the building (its
    sector wage, which the player's "Whole Sector" slider changes) ×
    `wageBonusMult`, floored at the minimum wage. At each month boundary
    `wageBonusMult` rises by 5 % (up to 1.50) if the firm made a profit, has
    capital above `legal_reserve_flat` (5000) and is below 75 % staffed. It
    falls by 5 % (down to 1.00) if the firm made a loss or has no capital.
  - **Flaw 1:** the private wage is tied to the *state* sector table. When
    the player changes a state sector wage, every private firm in that
    sector changes too. This contradicts D14 and "private wages are set
    automatically".
  - **Flaw 2:** no CPI indexation anywhere. With stable state rates, private
    wages never follow prices.
  - **Flaw 3:** the multiplier cannot go below 1.00. A private firm can never
    pay less than the state rate, even in distress. A high state wage forces
    every private firm in that sector into losses.
  - **Flaw 4:** the hike test uses the flat `legal_reserve_flat`, not the
    per-building reserve of D29 (not implemented yet).
  - **Cosmetic:** the `[WAGE AUDIT]` log line always prints the services
    sector (`TSM_SECTOR_SERVICES`) as "sector 4 rate 2.80", whatever the
    building's real sector is.
  - **How real economies do it** (*external*):
    - Euro-area wages are mostly set by collective agreements lasting about
      two years. Productivity and price inflation have historically been the
      main drivers of wage growth
      ([ECB Occasional Paper 338](https://www.ecb.europa.eu/pub/pdf/scpops/ecb.op338~dd97c1f69e.en.pdf)).
    - Statutory minimum wages have mostly kept pace with inflation, while
      negotiated wages lagged it
      ([OECD Employment Outlook 2023](https://www.oecd.org/content/dam/oecd/en/publications/reports/2023/07/oecd-employment-outlook-2023_904bcef3/08785bba-en.pdf)).
    - In the Soviet system, state committees set industry base rates and
      skill differentials, with coefficients for undesirable jobs and
      locations. Managers had bonuses, premiums and piece rates to respond
      to local labour markets
      ([Gregory and Kohlhase 1988](https://www.uh.edu/~kohlhase/GregoryKohlhase_RESTAT_1988.pdf)).
  - **Fix direction, already decided (D14):** private reference wage =
    sector baseline (from the INI at map start, *not* the live state rate) ×
    CPI index, then the profit/understaffing premium on top. This is §11
    item 6, step 5 of the [roadmap](../ROADMAP.md).

### 6.4 Social transfers (D18)

Welfare, pension and stipend levels are player decrees. The consequences come
from the economy itself: treasury cost on one side, poverty and native need
penalties on the other.

**How real countries do it (*external*):**

- **Two layers for the unemployed.**
  - *Unemployment insurance* replaces a share of the last wage. Across the
    OECD, a single person on the average wage keeps on average **58 %** of
    net income early in a spell, falling to **37 %** once long-term
    unemployed
    ([OECD Society at a Glance 2024](https://www.oecd.org/en/publications/society-at-a-glance-2024_918d8db3-en/full-report/unemployment-and-social-safety-net-benefits_ddfedfa8.html)).
    For low earners it is 50–80 % early on
    ([OECD 2023](https://www.oecd.org/content/dam/oecd/en/publications/reports/2023/02/income-support-for-jobseekers_a99cb15b/405ed506-en.pdf)).
  - *Minimum income* (last resort) is a flat amount. For a jobless family it
    averages **under 40 % of median income**, more than ten points below the
    OECD relative poverty line
    ([OECD benefits and wages](https://www.oecd.org/social/benefits-and-wages/data)).
- **Poverty lines** are either relative (50 % or 60 % of median income, used
  by the OECD and EU) or absolute: a cost-of-living basket, such as the
  Soviet and Russian "subsistence minimum".
- **The Soviet Union paid no unemployment benefit**, on the claim that
  unemployment did not exist. Pensions were graded by earnings, from 50 % to
  100 % of pay with a minimum and maximum; for the average retired worker
  about **65–70 %** of pay in 1958
  ([US SSA 1959](https://www.ssa.gov/policy/docs/ssb/v22n8/v22n8p3.pdf)).
  In post-Soviet Russia in 1993 the average pension was **36 %** of the
  average wage and the average unemployment benefit about **12 %**, below
  subsistence
  ([IMF](https://www.elibrary.imf.org/display/book/9781557756800/ch013.xml)).
- Tropico 6's "Social Security" edict pays retirees and students **60–70 %
  of the average wage** ([edicts](https://tropico.fandom.com/wiki/Edicts_(Tropico_6))).
- Benefits fixed in money lose value to inflation (OECD 2023), which is why
  indexation (§6.3) matters.

**Decided (D18): one Welfare decree, four tiers.** The owner approved the
system below and asked that it stay simple. Like Tropico's "Social Security"
edict, the player picks a tier in one place and sees its daily cost there.
The numbers are starting values for calibration.

- **Base welfare and stipends on the CPI basket.** It is the mod's
  subsistence minimum, it is computed from native prices in either currency
  (D21), and it decides whether a citizen can actually eat, which is what
  drives the native need penalties (D9).
- **Base pensions on the average wage, with the basket as a floor.** This is
  the Soviet and Tropico pattern, and it keeps pensions in step with wages.
- **Work must always pay:** welfare stays below the minimum wage at every
  level (new invariant I7).

| Transfer | None | Low | Medium | High |
|---|---|---|---|---|
| Welfare (unemployed) | 0 (Soviet model: no benefit) | 50 % of basket | 100 % of basket | 130 % of basket, capped below the minimum wage |
| Student stipend | n/a | 50 % of basket | 75 % of basket | 100 % of basket |
| Pension | n/a | 40 % of average wage | 60 % of average wage | 75 % of average wage |

Pensions are floored at 100 % of the basket at every level. "Basket" means
the CPI basket **including rent**. The levels describe consequences:

- **Low welfare** cannot feed the unemployed, so native hunger hits them, but
  it is cheap.
- **Medium** keeps everyone fed with nothing to spare.
- **High** is comfortable and costly.

A one-time insurance-style benefit (a share of the citizen's last wage for a
limited time, falling to the basket level) would be more realistic. It needs
per-citizen wage history and is left out to keep the system simple.

- Today (*code*) the ratio keys override the old absolutes:
  `welfare_ratio_to_min = 0.40`, `stipend = 0.60`, `pension = 0.80` of
  `minimum_wage = 2.50`, so 1.00 / 1.50 / 2.00 RUB/day. Those sit below the
  CPI basket, which makes "low welfare" the current *default*. Under D18 these
  keys are replaced by the decree tier.

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

*Initial list (accepted by the owner, D22; welfare merged into one decree per D18):*

| Decree | Levels | Effect | Cost or revenue |
|---|---|---|---|
| Welfare | none / low / medium / high | Unemployed, student and pension transfers together (§6.4) | Treasury per recipient-day |
| Settlement grant | off / small / large | One-off grant to each new citizen, in days of basket (§4.5.4) | Treasury per new citizen |
| Wage indexation (state) | off / on | §6.3 | Treasury grows with CPI |
| Minimum wage | ratio to CPI | Floor for all wages | Paid by employers |
| State retail price level | free / subsidised / native / above native | §5.3 | Treasury forgoes or earns revenue |
| Price ceiling (per staple) | off / on | Caps the private margin on food (§5.2) | Private profits |
| Rent control | off / capped | Caps private rent | Landlord income |
| High-education premium (state) | per sector | §6.2 | Treasury |

The existing fiscal sliders stay as continuous controls in the Almanac
(D7 removes the City Hall copy).

---

## 8. Housing choice by wealth (D10, D23)

### 8.1 What the game already does natively (*static*)

Answering the owner's question "isn't housing moving taken care of by the
native mechanics?": **partly.** From
[`Citizen_Housing_Relocation.md`](../Citizen_Housing_Relocation.md) and
[`Rent_Housing_Task1_Investigation.md`](../Rent_Housing_Task1_Investigation.md):

- **Young adults leaving their parents** (age 21+) search the whole republic
  for a flat (`FUN_140834DE0`). The search **already has a wealth gate**: a
  flat qualifies only if its rent level `bld+0x13E0` is at most the citizen's
  native living standard `person+0xE4`. So new households already sort
  themselves by wealth.
- **Dorm residents** move out when they become adults or when
  `person+0xE4` falls below the rent level.
- New residential buildings can pull residents from other buildings, and the
  player can move citizens by hand (player reports in the game's Steam
  discussions; not yet checked in the binary).
- Every move uses one native executor, `FUN_14083CA50`, which moves the whole
  household. A voluntary move reduces `person+0xD8` (×0.6).

**What the game does not do:** an established household in an ordinary
apartment **never moves because its income changed**. The rent test exists
only in the dorm branch. Also, `person+0xE4` is the game's own living
standard, driven by native city statistics, not by the mod's wallets.

### 8.2 What the mod already does (*code*)

The owner thought the mod already triggered moves for ordinary citizens. It
does, but only in a narrow case, and with its own search instead of the
game's (`enterprise_base` `CollectResidentialRent`, about line 2015):

- **Only private residential buildings.** Residents of state housing are
  never checked.
- **Only downward, as an eviction.** Once a month, a tenant whose rent is
  more than 35 % of their wallet (`rent_burden_threshold`), who paid less
  than 95 % of the rent, or who cannot afford a day of bread is evicted, with
  a happiness penalty (`eviction_penalty_happiness = 0.15`). Nobody ever
  moves *up* when they get richer.
- **The mod's own search** (`FindRelocationCandidate`, about line 1866) picks
  the new flat: poor (wealth < 0.40) to state or cheap flats, rich (≥ 0.70)
  to the most attractive ones. Only the final move uses the native executor
  `FUN_14083CA50`.
- It reads unreadable wealth, rent level and attractiveness as **0.5**,
  which is invented data (D8) and must become a visible error.

### 8.3 Target (D23)

Use the game's own flat check for everyone:

1. **Trigger the native search for established households.** The game
   already searches for flats natively for 21-year-olds leaving their
   parents (verified in `FUN_140834DE0`). New arrivals and residents of
   demolished buildings are also placed natively; whether they go through
   the same scorer is not mapped yet. The cleanest hook is the dorm
   affordability test in `FUN_140834DE0`
   (`2.0 <= person+0xA8 || person+0xE4 + bonus < bld+0x13E0`, hook point 1
   in `Citizen_Housing_Relocation.md` §4), widened from dorms to all
   apartments. Trigger it in two cases:
   - **Can't afford**: the household's wealth is below its building's rent
     level. It moves down.
   - **Can afford better**: the household's wealth is clearly above its
     building's rent level. The native scorer only moves on a strict
     improvement, so it moves up only if a better flat is free.
2. **Which wealth the test reads.** The native test reads the game's own
   living standard `person+0xE4`, which the game recomputes all the time. To
   make the mod's money count, either the gate compares the wallet runway
   (§4.5.2) with the rent, or `person+0xE4` is nudged from the wallet class.
   This is decided after mapping how `FUN_14083A6D0` writes `person+0xE4`
   (G6).
3. **Retire the mod's own search and the eviction penalty** once point 1
   works. A move the citizen chooses is not an eviction. Keep the
   anti-despawn guard: nobody moves when no flat is free.
4. **No ping-pong.** If testing shows households moving back and forth, add
   a minimum time between moves per household. The native tenure value
   (`person+0x2D`) may already provide this (G6).

The earlier open question about search radius and cadence (old Q5) goes away:
the native search has its own scope and timing.

### 8.4 Building blocks already in the mod (*code/static*)

- The native living-standard / wealth float is `person + 0xE4` (0..1,
  maintained by the game; `TSM_PERSON_WEALTH`).
- Attractiveness-based rent and the eviction/relocation engine use native
  `FUN_14083CA50`, with an anti-despawn guard (design doc §3).
- Living-standard strata come from `citizen_retail` (design doc §6.2).

The intended result is unchanged from the first draft: poorer households end
up in cheap or state housing and affluent ones in attractive private housing,
moves are voluntary and only into vacant flats, and rent tiers follow
attractiveness and dwelling quality as today.

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
6. **Check money creation (I8)** by grepping every capital, pool and wallet
   credit for a matching debit, and by running the `state_ledger`
   reconciliation on the reference save.

**Invariants (acceptance gates):**

| ID | Invariant |
|---|---|
| I1 | Default minimum wage ≥ CPI basket + daily rent (household floor). |
| I2 | For each producer sector at default wages: native value added per worker-day ≥ wage (firm ceiling). A private firm that is fully staffed and fully supplied, selling at native prices, is profitable before tax. |
| I3 | For each private shop type at the default category margin and measured foot traffic: gross margin ≥ wages + fixed costs. |
| I4 | At default decrees, a reference planned-economy republic's treasury is sustainable from exports + state-shop sales + taxes. The player may choose deficits, but defaults must not force one. |
| I5 | **Money conservation:** the sum of wallets, company capital, investment fund and treasury changes only by customs flows and explicitly labelled policy flows. `state_ledger` reconciliation shows zero unexplained delta. |
| I6 | **No invented values:** every number displayed or charged traces to a native field, a player policy or a documented INI constant. Anything else shows `UNAVAILABLE` / `ERROR`. |
| I7 | **Work pays:** at every welfare level, welfare is below the minimum wage. |
| I8 | **No creation:** no code path credits any account without debiting another, except native customs flows and the once-per-map fund founding capital (D20). |
| I9 | **Currency parity:** with no exchange rate (D21), the same republic gives the same *ratios* (wage to basket, margin to cost, transfer to basket) in rubles and in dollars. |

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
| G6 | How `FUN_14083A6D0` maintains `person+0xE4` (native living standard), the exact dorm affordability branch in `FUN_140834DE0`, and the tenure field `person+0x2D` | §8.3 housing by wealth | Medium |
| G7 | Native building wear/condition field, if it exists | §4.5.3 privatisation price | Medium |
| G9 | Native building-type catalogue (every type descriptor, including unplaced types), to price a reference building's construction cost | §4.5.1 founding capital (later improvement) | Low |
| G8 | Native production recipe in memory (`$PRODUCTION` / `$CONSUMPTION` per building type) and its units (believed to be per worker-day) | §4.5.7 starting capital | Medium |
| G3 | Native price recompute site (to later write market-adjusted native prices) | Future market model (D2) | Deferred |

Follow [`docs/AGENTS.md`](../AGENTS.md): one Ghidra operator, index each
result in `docs/ghidra-index/`, and keep bounded raw exports in
`docs/ghidra-exports/`.

---

## 10. Open questions for the owner

All questions answered on 2026-11-22: Q1 → D19, Q2 → D20, Q3 → D18,
Q4 → D21, Q5 → D23, Q6 → D22, Q8 → D26, Q9 → D25, Q10 → D23, Q11 → D27,
Q12 → D31, Q13 → D29, Q14 → D30. Q7 (age rule) is withdrawn and replaced by
D28.

**No owner questions are open.** What remains is verification, not decisions:

- the Ghidra and runtime queue in §9.1 (G1–G9);
- which native price field is *base* and which is *trade* (§3.2 point 5);
- calibration from captured data (§9): the founding-capital bundle, the
  welfare tier values, the sale discount and `norm_days`.

Left open for later (raised in this doc, not yet put to the owner): whether the toggle cooldown also starts
when a new building is completed (§4.5.6).

---

## 11. Change list (follow-up tasks)

Each item is a separate engineering task with its own task record
(`docs/tasks/TEMPLATE.md`).

**Status, 2026-11-22.** Code step 1 is implemented in PR #100. Host tests and
the CI Windows build pass; the in-game acceptance check is still pending
(roadmap step 0). It covers the
market-book part of item 1, all of item 2, and the procurement part of
item 3. See the
[task record](../tasks/active/2026-11-22-remove-market-multiplier-and-procurement.md).

**Status, 2026-11-22 (step 2).** The rest of item 1 (fallback prices, `default`,
`default_usd`, `usd_divisor`) and item 9 (Almanac sample rows) are implemented
on `arena/c676e273-wrsre`, together with D32–D34 in their consumers. Host tests
pass; the Windows CI build, independent review and the in-game check are still
to do. `resource_pricing` is now v6 and returns `0.0f` for `UNPRICED` and for
an unavailable CPI basket. The 0.70 `wholesale_ratio`, foreign restock and
`private_logistics` billing defaults remain open for step 3. See the
[task record](../tasks/active/2026-11-22-remove-fallbacks-and-made-up-data.md).

**Order of work:** the items below are grouped into ordered steps in
[`ROADMAP.md`](../ROADMAP.md) §3. Use the roadmap for "what next" and this
list for the detail of each item.

1. **resource_pricing:** remove the market book and multiplier *(done:
   v5)*, fallback prices and `usd_divisor` *(done: v6, step 2)*. Missing native
   price means `UNPRICED`. Bump the API version. Keep the CPI basket.
2. **Multiplier consumers:** `citizen_retail.cpp` 613 and 983,
   `enterprise_base.cpp` 1985 and 2504, `private_logistics.cpp` 455,
   `private_shops.cpp` 948. Switch them to the plain native price. *(Done.)*
3. **private_shops:** delete the dispatch procurement (`"procurement"`,
   `procurement_price_mult`, `procure_fallback`) *(done: v3.6)*, the 0.70 delivery billing
   (`wholesale_ratio`) and foreign restock. Keep pub service charges.
4. **private_logistics:** exclude customs from `b2b_state_buy`. Defaults
   `billing = 1`, `dry_run = 0`. After G1: company export revenue and company
   import cost.
5. **enterprise_base:** private shop price = landed cost × (1 + category
   margin) × (1 + VAT), with bounded monthly adaptation through
   `SetMarkupImpl`. Remove the single `default_markup`. Privatisation pays
   the building price to the treasury and the starting capital from the fund
   (§4.5.3). Founding capital once per map with a persisted flag; no load-time
   reset (§4.5.1), amount from the construction-materials bundle (D31).
   Nationalisation paid by the treasury, capital returned to the fund, and a saved per-building toggle cooldown (§4.5.6). Starting
   capital and legal reserve from the per-building formula (§4.5.7), replacing
   `starting_capital` and `legal_reserve_flat`. Set
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
   `kLogisticsRows`, about 60–95) and the `DrawTable` fallback (about 659)
   *(done: step 2)*.
   With no live data, show a full-panel **"NO DATA: <service> unavailable"**
   in warning colour. Per row, show `UNAVAILABLE` / `ERROR <reason>`, never a
   number. Update `TESTING.md`, which still expects `SYNTHETIC` values.
10. **Decrees** (§7) and **housing choice** (§8) as new features, after items
    1–9.
11. **Calibration** (§9) with captured fixtures and invariant tests I1–I9.
12. **Currency parity (D21):** remove every `usd_divisor` and `*_usd`
    conversion. Re-base each flat money constant listed in §3.2 point 4 on
    the CPI basket or the building's native construction cost.
13. **currency:** `starting_wallet = 0`; Settlement grant decree (§4.5.4).
14. **Household investment** into the fund (§4.5.2), in `currency` or
    `citizen_retail`, which already know wallets and the Affluent line.
15. **Housing (§8.3):** trigger the native flat search for established
    households; retire `FindRelocationCandidate` and the eviction penalty;
    replace the 0.5 fallbacks with visible errors.
16. **Welfare decree (§6.4):** one decree, four tiers, replacing the
    `*_ratio_to_min` keys in `citizen_labor`.
