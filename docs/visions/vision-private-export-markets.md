# Private export markets and foreign trade policy: plan for a later step

**Status:** planned feature, **not scheduled for the current work**. The owner
decided on 2026-11-23 that private exports and imports through customs houses
are a **separate, later step on their own branch** (D41 in the
[economy direction](economy-direction-2026-11-22.md)). The design decisions
for that step are recorded (D36–D39, D42–D48; D21 and D37 amended). It depends
on step 4d (company finances, D45) and on the native verifications in §7.
**For the ordered plan, read [`ROADMAP.md`](../ROADMAP.md).**

**Evidence labels:** *code* = read in source at `main` `73a9d1d`; *log* = the
owner's in-game logs of 2026-11-23 (old build `0.19.0`, 17:11, save with a
manual customs task; step-3 build `0.19.1-live-billing`, 18:56–19:26, save
"2209 - Testville"); *static* = Ghidra results already recorded in `docs/`;
*external* = public documentation or measurement (linked); *proposal* = not
decided by the owner; *hypothesis* = needs verification.

---

## 1. What happens today (do not rely on private exports yet)

*code, log*

- `private_logistics` finds customs houses (building type 20) but **never
  assigns them**. The daily log only prints `nearest customs (geometric, not
  assigned)`. `AutoSiteCapabilities` returns 0 for type 20, the
  infrastructure probe skips them as storage candidates, and there are no
  customs controls.
- A customs target that the player added **by hand** to a private DO is kept
  exactly as it is (`AutoPreserveManualCustoms`, log `policy=preserve-customs`).
- When a private seller's goods are delivered into a customs house,
  `planSettlement` marks the delivery `customsExport` and moves **no money**.
  The native customs sale credits the **treasury**. The company loses the
  goods and receives nothing. A private DO with a manual customs task
  therefore gives the state its company's exports for free. **Until this
  step lands, do not route private goods to customs.**
- A private company that collects goods at a customs house pays
  `b2b_state_sell` at the mod's native price on delivery. The treasury
  natively paid the customs market price, so the state carries the difference
  (the "customs spread", a documented known limit).
- A private DO's trip fuel for a trip that ends at a customs house is not paid.

## 2. How trade works: the game, real life, other games

### 2.1 In the game

*external*

1. **Exports are automatic sales on delivery.** A vehicle unloads at a customs
   house and the goods are sold
   ([Steam: how do I export?](https://steamcommunity.com/app/784150/discussions/0/4415298540305772163/),
   [Steam: HOW TO SELL](https://steamcommunity.com/app/784150/discussions/0/3362406825541815875/?l=english&ctp=1)).
2. **Currency depends on the border:** Soviet border = roubles, Western (NATO)
   border = dollars. Clicking a customs house shows its prices
   ([official wiki, Q&A](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Question_and_answers),
   [official wiki, Trade](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Trade#Customs_house)).
   Dollar prices are usually 10–30 % higher; the game has no exchange rate.
3. **Customs houses are placed by the map**, not built by the player. All
   resources except electricity go through them
   ([official wiki, Trade](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Trade#Customs_house)).
   The game lets the player rename them.
4. **Imports are bought when a vehicle loads at customs**, at the customs
   market price. Auto-purchase adds a delivery fee by distance from the border;
   a truck pickup does not ([fandom, Economy](https://workers-resources.fandom.com/wiki/Economy),
   [Steam: auto-building vs importing](https://steamcommunity.com/app/784150/discussions/0/3823032780418157984/)).
5. **Trade moves the price (supply and demand).** Each resource's imports and
   exports over the last 75 days are compared with a fixed threshold.
   Simplified developer formula: export factor `1 − (exported / threshold) ×
   0.05`, import factor `1 + (imported / threshold) × 0.05`, applied to the
   base price, with inflation every 5 days
   ([report #57](https://workers-resources.fandom.com/wiki/Report_for_the_community_57),
   [report #56](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Report_for_the_community_56)).
   Private exports therefore lower the export price for the state too, and the
   customs price can differ from the mod's native price.
6. **Customs capacity is limited** by loading bays and vehicle movement
   ([Steam: export-based state](https://steamcommunity.com/app/784150/discussions/0/6117591738154113509/)).
7. "Brand new" vehicles can be exported with a bloc price rule
   ([fandom, Vehicles](https://workers-resources.fandom.com/wiki/Vehicles)).
   **Out of scope.**

### 2.2 Real life

*external*

- **Currency surrender.** In China's 1980s–1994 system, exporters handed their
  foreign currency to the state at the official rate and were paid in yuan;
  they received retention quotas (rights to buy part of it back) that could be
  traded in swap centres ([IMF](https://www.elibrary.imf.org/display/book/9781557755629/Ch08.xml),
  [Tsang](https://www.sktsang.com/ArchiveIII/Streeter93.PDF)). An official rate
  above the market rate acted as a hidden tax on exporters. The Soviet Union
  kept foreign trade as a state monopoly.
- **Trade policy tools:** bans and embargoes, import tariffs, export duties,
  quotas (tonnes per period) and licences.
- **Company credit:** suppliers give trade credit with a credit limit and put
  a non-paying customer on credit hold. A failing firm is restructured or
  liquidated. Kornai's **soft budget constraint** describes socialist firms
  that are always bailed out and so never improve
  ([Kornai, Maskin, Roland](https://www.semanticscholar.org/paper/Understanding-the-Soft-Budget-Constraint-Kornai-Maskin/e1e034c81ecbf2d70cf30efa60b6c326d1dc22dd)).

### 2.3 Other games

*external*

- **Victoria 3:** a trade-policy law caps tariffs; per-good import/export
  tariffs and subventions; embargoes; per-good market policies; trade centres
  pay the tariffs ([wiki, Market](https://vic3.paradoxwikis.com/Market),
  [dev diary #54](https://www.paradoxinteractive.com/games/victoria-3/news/dev-diary-54-trade-revisions)).
  Victoria 3 charges tariffs on the base price; this mod charges them on the
  measured price, because it uses native prices only (D2).
- **WR:SR itself** has no tariffs, quotas or bans.

## 3. What the mod already sees

- **The physical unload into customs passes through the patched transfer.**
  *log (old build):* 190 unload fragments into the customs house `zoll_sahy`
  at site `0x68F792`, mode 0, sign `+`, about 0.03–0.2 t each: food 7.58 t,
  plants 5.00 t, alcohol 6.26 t, valued by the mod at 3381.27 RUB. These were
  state goods, so no money moved. **Unknown:** whether the native code credits
  the treasury inside that call or later (Ghidra **G1**).
- **One source per truckload.** *log:* in both logs no delivery mixed goods
  from different sellers; the only multi-lot load had two lots from the same
  seller.
- **The native DO planner rewrites customs task thresholds.** *static:* the
  re-plan routine `FUN_1401E2380` sets a customs entry's load threshold to 0
  and its unload threshold to about 0.99
  ([DO percentage findings](../private-logistics-runtime/DO_PERCENTAGE_WAKEUP_FINDINGS_V2.md)).
  A customs house therefore always "wants" goods. The mod's daily
  `AutoApplyTaskDirections` writes thresholds and the "all resources" flag; on
  customs entries the two would fight.
- **History to avoid repeating.** *static/log:* v0.18 applied default-deny to
  customs entries, and a manual customs task did one trip and stopped
  ([permissions plan](../private-logistics-runtime/AUTONOMOUS_DO_PERMISSIONS_PLAN.md)).
- **Today the mod can only deny a whole direction** of a DO task (wildcard off
  and an empty resource list) or allow all resources. Switching **one
  resource** on or off needs the native per-resource selector layout (G10).
- **Building names.** *code:* the Almanac already reads a building's
  player-given name (`bld+0x114`, `enterprise_base` `getBuildingCustomName`)
  and its town (`bld+0x108` → `+0x10`) in
  `plugins/almanac_overlay_dev/graphics_hook.cpp`. The trade page reuses that
  code to name each customs house.
- **Company money has one currency.** *code:* `enterprise_base` keeps one
  `capital` double per company in the active currency
  (`src/enterprise_base_api.h`). D42 handles the other currency.
- **Trip-fuel payer identity is not yet reliable.** *code, log:* the trip
  record calls its target the "buyer" even where the truck loaded. Step 3b
  fixes this first.

## 4. Owner decisions (2026-11-23)

Recorded in the [decision register](economy-direction-2026-11-22.md#1-decision-register).

- **D36 (amended by D43): customs controls, default closed.** Private exports
  and imports are off on every customs house until the player opens them. The
  controls live on the Almanac trade page; the customs panel shows the route's
  status.
- **D37 (amended): manual customs targets.** The mod changes nothing on a
  customs target the player added by hand, **except** that trade law applies
  to it: a closed route, a banned good or a full quota is enforced there too.
- **D38: export revenue is measured, then checked.** The company receives the
  treasury increase the native sale caused (`company_export`). `quantity ×
  locked native price` is logged next to it; a difference above **1 %** is a
  visible `WARN`. Nothing measured: loud `ERROR`, nothing paid, never the
  estimate.
- **D39: the foreign market pays the freight of an export trip**, as money
  from abroad on top of the proceeds (`foreign_freight`). A deliberate, named
  exception to D20, fully visible.
- **D41: later step**, own branch.
- **D42: price-derived exchange rate and currency surrender** (amends D21).
  - Rate = the geometric mean, over every resource with both a native rouble
    and a native dollar price, of `USD price ÷ RUB price`. It moves with the
    native prices. Not enough prices: `UNAVAILABLE`, no conversion.
  - Companies keep holding only the active currency. Proceeds earned in the
    other currency stay in that treasury (the game puts them there) and the
    company is paid in the active currency at the rate from the active
    treasury (`fx_surrender`). Imports in the other currency work the other
    way round. No money is created.
  - A later decree may set an official rate above or below the computed one.
  - D21's purpose stands: no fixed or invented rate; `usd_divisor = 50` still
    goes in step 7; the rate is used **only** for currency surrender unless
    the owner decides otherwise.
- **D43: an Almanac "Foreign trade" page.** Every customs house is a trade
  route, listed by name (§3), border (East/RUB or West/USD) and mode (road,
  rail, ship).
  - Per route: open or closed for private exports and, separately, for
    private imports.
  - Per good, as a default for all routes with a per-route override: ban
    export, ban import, export duty %, import tariff %, quota in tonnes per
    game month.
  - Rules apply to **private** trade. State trade stays native (D6); the
    player controls it directly.
  - "A route is a nation" holds for **rules**. Prices stay native per border.
- **D44: import cost = the measured treasury payment plus any tariff.** What
  the treasury pays when a truck loads at customs is the market price at that
  customs house; the company pays exactly that (`company_import`), closing
  the customs spread. Nothing measured: loud `ERROR`, nothing charged.
- **D45: company health ladder** for every private company (roadmap step 4d).
  - **Healthy:** money ≥ legal reserve (D29). **Strained:** below it; no
    payouts to the fund, otherwise normal. **In debt:** money < 0 within the
    credit limit; buys on credit, pays interest at the bank rate, and a share
    of every income repays the debt automatically. **Credit hold:** debt above
    the limit; no new purchases (the mod switches off its DOs' import pickups
    and deliveries of bought inputs), but it can **always still sell and
    export**. **Insolvent:** on credit hold for a long time; a decree chooses
    bail out (treasury pays, shown as a subsidy), restructure (part of the
    debt written off, the fund recapitalises) or liquidate (the building
    returns to the state, bad debt written off visibly).
  - Credit limit = the company's own legal reserve. The hold lifts only when
    debt falls to half the limit. A grace period precedes insolvency.
  - Import debt uses this ladder.
- **D46: one source per truckload.** A load that mixes different sellers is
  refused for attribution and logged as an error, like D33. No split rule.
- **D47: a new mod-added customs target exports only the company's own
  products.** The player can widen or narrow the list on the native DO
  screen, and the mod never overwrites it, except to enforce trade law (D43).
  Buying state goods to resell abroad is not a default.
- **D48: price impact is native.** Private exports lower the native price
  through the game's supply and demand. The Almanac shows private export and
  import tonnage and value per good each month.

## 5. Design outline (for the later branch)

### 5.1 Flows

| Flow | Payer → payee | Amount | Label |
|---|---|---|---|
| Private goods delivered to an open route (export) | treasury → company | measured proceeds − export duty (D38, D43) | `company_export` (+ `export_duty` kept) |
| Proceeds in the other currency | treasury (active) → company; the foreign amount stays in the other treasury | measured × rate (D42) | `fx_surrender` |
| State goods delivered to customs | native | native | none (D6) |
| Export trip fuel of a private DO | foreign market → DO (new money, D39) | observed trip fuel | `foreign_freight` |
| Private DO loads at customs (import) | the treasury pays natively at loading | native market price | native |
| Imported goods delivered to a private company | company → treasury | measured purchase + import tariff (D44, D43) | `company_import` (+ `import_tariff`) |
| Import trip fuel | receiving company → DO | observed trip fuel | existing rule |
| Unpaid import or input bill | becomes company debt (D45) | — | `company_debt` |

### 5.2 Measuring the money (D38, D44)

1. Either Ghidra G1 finds where the native code credits or debits
   `TSM_GO_TREASURY_RUB` (`0x588`) / `TSM_GO_TREASURY_USD` (`0x580`) for a
   customs delivery or pickup, or a runtime probe reads both treasuries
   immediately before and after the native transfer call. If the probe sees no
   change there, the money moves elsewhere: report it and use G1. Never guess.
2. A change in both currencies, or a failed read: `ERROR EXPORT UNATTRIBUTED`
   / `ERROR IMPORT UNATTRIBUTED`, counted, nothing moved.
3. Unloads arrive in fragments (§3). Settle per fragment, or per delivery when
   the matched delivery closes; prove exactly one payout per tonne.
4. Mixed-seller loads are refused (D46).

### 5.3 Routes and enforcement (D43, D47, D37)

- `AutoSiteCapabilities(20)` = load and unload, but a customs house is a
  candidate only for the directions its route has open.
- **Closed route:** the direction is switched off on every private DO's entry
  for that customs house, manual ones included (D37 amended).
- **Banned good or full quota:** that one resource is removed from the
  entry's list and restored when the ban ends or the month resets. Until G10
  verifies per-resource writes, the only safe enforcement is closing the
  whole direction; the page must say so.
- **Quotas** count measured tonnes per route, per good, per game month. The
  last load can overshoot; that is logged.
- **Never write thresholds on customs entries** (the native re-plan owns them).
- Customs entries count toward the road limit of 20 and rail limit of 5
  targets; existing entries are never deleted to make room.
- Settings persist per customs house; old saves load with every route closed.

### 5.4 Visibility

- `[B2B GOODS] export:` / `import:` show seller or buyer, route, quantity,
  locked price, measured amount, currency, rate, duty or tariff and payout.
- Day close: `exports_paid`, `exports_unattributed`, `export_price_mismatch`,
  `imports_charged`, `fx_surrender`, `duties`, `tariffs`, `foreign_freight`,
  `quota_hits`.
- `foreign_freight` is not a treasury movement; `state_ledger` needs an
  external-inflow line for it, or the month close (I5) cannot explain it.
- Almanac: the trade page (§4 D43), private trade per good and route per
  month, the current exchange rate and its inputs.

## 6. Open questions

None from the owner side; all were answered on 2026-11-23 (D42–D48). What
remains is verification (§7) and calibration of D45's numbers (repayment
share, grace period, interest), which step 4d proposes from captured data.

## 7. Prerequisites and order

1. **Step 3b:** reliable "who received the goods" for DO trips and
   treasury-paid trip fuel for state receivers (D40).
2. **Step 4a** (reserves, fund) and **step 4d** (company health ladder, D45).
3. **G1, or the runtime measurement probe**, for the customs money events.
4. **G10:** the native per-resource selector entries of a DO task (layout,
   how to add or remove one resource, what the native UI does) — needed for
   bans, quotas and D47. Investigate first; until verified, enforcement is
   whole-direction only.
5. **G11 (optional):** which border and currency a customs house belongs to.
   Without it, the currency is known only after the first measured trade.
6. A `state_ledger` external-inflow channel for `foreign_freight`.

## 8. Acceptance for the later step (host tests, then in game)

- Host tests: route open/close and per-good rules, old-save load with routes
  closed; manual entries changed only by trade law; no threshold writes on
  customs entries; exact measured payout and charge; 1 % mismatch `WARN`;
  unmeasured or two-currency `ERROR`; mixed-seller refusal; one payout per
  tonne; exchange rate from prices and `UNAVAILABLE` without them;
  `fx_surrender`, duty, tariff and `foreign_freight` labelled and counted;
  quota reset at month change.
- In game: open exports on one route. A private factory's own goods reach it
  through a private DO. `[B2B GOODS] export` shows measured ≈ price; the
  company's money rises by the measured amount minus duty; the export trip
  shows `foreign_freight`. Ban one good and see it stop. Set a quota and see
  it stop and resume next month. Import through a route and see the company
  charged the measured price plus tariff. The month close in `state_ledger`
  has no unexplained delta.
