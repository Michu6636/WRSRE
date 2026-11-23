# Private export markets: plan for a later step

**Status:** planned feature, **not scheduled for the current work**. The owner
decided on 2026-11-23 that private exports and imports through customs houses
are a **separate, later step on their own branch** (D41 in the
[economy direction](economy-direction-2026-11-22.md)). This document records
the investigation and the owner's decisions so that the later branch can start
without repeating the research. **For the ordered plan, read
[`ROADMAP.md`](../ROADMAP.md).**

**Evidence labels:** *code* = read in source at `main` `73a9d1d`; *log* = the
owner's in-game logs of 2026-11-23 (old build `0.19.0`, 17:11, save with a
manual customs task; step-3 build `0.19.1-live-billing`, 18:56–19:26, save
"2209 - Testville"); *static* = Ghidra results already recorded in `docs/`;
*external* = public game documentation or community measurement (linked);
*proposal* = suggested by an agent, **not decided by the owner**;
*hypothesis* = needs verification.

---

## 1. What happens today (do not rely on private exports yet)

*code, log*

- `private_logistics` finds customs houses (building type 20) but **never
  assigns them**. The daily log only prints `nearest customs (geometric, not
  assigned)`. `AutoSiteCapabilities` returns 0 for type 20, the
  infrastructure probe skips them as storage candidates, and there are no
  customs checkboxes.
- A customs target that the player added **by hand** to a private DO is kept
  exactly as it is (`AutoPreserveManualCustoms`, log `policy=preserve-customs`).
- When a private seller's goods are delivered into a customs house,
  `planSettlement` marks the delivery `customsExport` and moves **no money**.
  The native customs sale credits the **treasury**. The company loses the
  goods and receives nothing. A private DO with a manual customs task
  therefore gives the state its company's exports for free. **Until this
  step lands, do not route private goods to customs.**
- A private company that collects goods at a customs house pays
  `b2b_state_sell` at the native price on delivery. The treasury natively
  paid the customs buy price, so the state carries the difference (the
  "customs spread", a documented known limit).
- A private DO's trip fuel for a trip that ends at a customs house is not paid
  (it is not a private-to-private trip).

## 2. How exports and imports work in the game

*external*

1. **Exports are automatic sales on delivery.** A vehicle unloads at a customs
   house and the goods are sold. Nothing has to be set up at the border
   ([Steam: how do I export?](https://steamcommunity.com/app/784150/discussions/0/4415298540305772163/),
   [Steam: HOW TO SELL](https://steamcommunity.com/app/784150/discussions/0/3362406825541815875/?l=english&ctp=1)).
2. **Currency depends on the border.** The Soviet border pays roubles and the
   Western (NATO) border pays dollars. Clicking a customs house shows its
   prices ([official wiki, Q&A](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Question_and_answers),
   [official wiki, Trade](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Trade#Customs_house)).
   Dollar prices are usually 10–30 % higher than rouble prices; there is no
   exchange rate (D21).
3. **Customs houses are placed by the map**, not built by the player. All
   resources except electricity go through them; electricity uses border
   connections ([official wiki, Trade](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Trade#Customs_house)).
4. **Imports are bought when a vehicle loads at customs** (a "manual
   purchase"), or by auto-purchase, which adds a delivery fee by distance from
   the border ([fandom, Economy](https://workers-resources.fandom.com/wiki/Economy)).
   Community measurements put that fee at about 4 RUB or 7 USD per ton-km at
   the start of 1960 ([Steam: auto-building vs importing](https://steamcommunity.com/app/784150/discussions/0/3823032780418157984/)).
5. **Trade moves the price.** The game counts each resource's imports and
   exports over the last period (75 days) against a fixed threshold. A
   simplified formula from the developers: export price factor
   `1 − (exported / threshold) × 0.05`, import factor
   `1 + (imported / threshold) × 0.05`, applied to the base price, with
   inflation every 5 days ([report #57](https://workers-resources.fandom.com/wiki/Report_for_the_community_57),
   [report #56](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Report_for_the_community_56)).
   **Consequence:** private exports lower the export price for the state too,
   and the customs price can differ from the mod's global native price.
6. **Customs capacity is limited** by loading bays and vehicle movement
   ([Steam: export-based state](https://steamcommunity.com/app/784150/discussions/0/6117591738154113509/)).
7. "Brand new" vehicles can be exported too, with a bloc price rule
   ([fandom, Vehicles](https://workers-resources.fandom.com/wiki/Vehicles)).
   **Out of scope** for this step.

## 3. What the mod already sees

- **The physical unload into customs passes through the patched transfer.**
  *log (old build):* 190 unload fragments into the customs house `zoll_sahy`
  at site `0x68F792`, mode 0, sign `+`, about 0.03–0.2 t each: food 7.58 t,
  plants 5.00 t, alcohol 6.26 t. The mod valued them at 3381.27 RUB at the
  global native price. These were state goods, so no money moved.
  **Unknown:** whether the native code credits the treasury inside that same
  call or later in a customs tick. That is Ghidra target **G1**.
- **The native DO planner rewrites customs task thresholds.** *static:* the
  re-plan routine `FUN_1401E2380` sets a customs entry's load threshold to 0
  and its unload threshold to about 0.99
  ([DO percentage findings](../private-logistics-runtime/DO_PERCENTAGE_WAKEUP_FINDINGS_V2.md)).
  The mod's daily `AutoApplyTaskDirections` writes thresholds and the
  "all resources" flag. On a customs entry the two would fight, re-dirtying
  the office every day.
- **History to avoid repeating.** *static/log:* `private_logistics` v0.18
  applied default-deny to customs entries, and a manual customs task did one
  trip and then stopped. That is why manual customs tasks are preserved today
  ([permissions plan](../private-logistics-runtime/AUTONOMOUS_DO_PERMISSIONS_PLAN.md)).
- **Company money has one currency.** *code:* `enterprise_base` keeps one
  `capital` double per company in the active currency (`src/enterprise_base_api.h`).
  A dollar customs house in rouble mode would pay money a company cannot hold.
- **Trip-fuel payer identity is not yet reliable.** *code, log:* the trip
  record calls its target the "buyer" even when the truck loaded there. The
  step-3 log shows unpaid trips "for" the state `fabric_factory` that were
  probably fabric pickups for the private clothing factory. Step 3b fixes
  this before this step builds on it.

## 4. Owner decisions for this step (2026-11-23)

These are recorded in the [decision register](economy-direction-2026-11-22.md#1-decision-register).

- **D36: Customs checkboxes, default OFF.** Each customs house gets two
  checkboxes in its panel, drawn like the state-storage ones:
  "Allow private exports (private DOs deliver goods here)" = truck unload,
  and "Allow private imports (private DOs take goods from here)" = truck load.
  New and existing customs houses start with both OFF.
- **D37: Manual customs targets stay untouched.** A customs target that the
  player added by hand to a private DO keeps working exactly as today. The
  checkboxes control only the customs targets the mod adds and tracks.
- **D38: Export revenue is measured, then checked against the price.** The
  exporting company receives the **exact** native proceeds: the treasury
  increase the native sale caused, moved from the treasury to the company as
  `company_export`. The mod also logs `quantity × native price locked at
  load`; a difference beyond a small tolerance is a visible `WARN`. If no
  proceeds can be measured, it is a loud `ERROR` and nothing is paid. The
  price estimate is never paid instead.
- **D39: The foreign market pays the freight of an export trip.** A private
  DO's trip fuel for a trip that delivers to a customs house is credited to
  the DO as money from abroad, on top of the export proceeds. Nothing is
  taken from the company or the treasury. This is a deliberate, named
  exception to D20 ("no money from nothing") and must be fully visible: its
  own label (`foreign_freight`), its own daily total and its own ledger line.
- **D41: Later step.** This whole feature is built later on its own branch.

## 5. Design outline (for the later branch)

### 5.1 Flows

| Flow | Payer → payee | Amount | Label |
|---|---|---|---|
| Private goods delivered to customs (export) | treasury → exporting company | the measured native proceeds (D38) | `company_export` |
| State goods delivered to customs | native | native | none (D6) |
| Export trip fuel of a private DO | foreign market → DO (new money, D39) | observed trip fuel | `foreign_freight` |
| Private DO loads at customs (import) | the treasury pays natively at loading | native | native |
| Imported goods delivered to a private company | company → treasury | today the native price; target the measured import cost (§6 Q3) | `b2b_state_sell` today, `company_import` target |
| Import trip fuel | receiving company → DO | observed trip fuel | existing rule |

### 5.2 Export attribution (D38)

1. Find the money event. Either Ghidra G1 finds where the native code credits
   `TSM_GO_TREASURY_RUB` (`0x588`) / `TSM_GO_TREASURY_USD` (`0x580`) for a
   customs delivery, or a runtime probe reads both treasuries immediately
   before and after the native transfer call for a matched customs delivery.
   If the probe sees no increase there, the credit happens elsewhere: report
   it and fall back to G1. Never guess.
2. Pay the measured increase in the currency it arrived in. An increase in
   both currencies, or a failed read: `ERROR EXPORT UNATTRIBUTED`, counted,
   nothing paid.
3. Several seller lots in one delivery: one private seller gets the whole
   amount. *Proposal:* split between several sellers by each lot's share of
   `quantity × locked price`, logged `allocation=value_share`; a state lot's
   share stays in the treasury.
4. Unloads arrive in fragments (§3). Settle per fragment, or per delivery once
   the matched delivery closes. Either way, prove there is exactly one
   payout per tonne.

### 5.3 Customs targets (D36, D37)

- `AutoSiteCapabilities(20)` = load and unload. Customs houses join the
  candidate list only when a checkbox is ticked. Permissions are saved in the
  existing state-permission table; old saves load with both OFF.
- Mod-added customs entries are tracked by memo membership. Manual entries
  stay preserved byte for byte (D37).
- **Do not write thresholds on customs entries.** The native re-plan sets them
  (§3). The mod only turns a forbidden direction off.
- *Proposal (not an owner decision):* a mod-added customs entry starts with
  "all resources" for the ticked directions. After that the mod never
  rewrites the resource list, so the player can narrow what is exported or
  collected on the native DO screen.
- Customs entries count toward the road limit of 20 and rail limit of 5
  targets. Existing entries are never deleted to make room.

### 5.4 Visibility

- `[B2B GOODS] export:` shows seller, quantity, locked price, measured amount,
  currency and payout.
- The day close adds `exports_paid`, `exports_unattributed`,
  `export_price_mismatch` and `foreign_freight`.
- `foreign_freight` is not a treasury movement. `state_ledger` needs an
  external-inflow line for it, or the monthly reconciliation (I5) cannot
  explain the new money.
- Almanac: private export revenue and foreign freight per month.

## 6. Open questions for the owner (before the branch starts)

1. **Currency mismatch.** A dollar customs house pays dollars, but a
   company's capital is in the active currency only, and there is no exchange
   rate (D21). Options: refuse the payout loudly; give companies a second
   currency account; or allow private exports only through customs houses of
   the active currency.
2. **Resource selection.** Accept the §5.3 proposal ("all resources" first,
   then the player narrows it), or something else.
3. **Import cost.** Charge the private company the measured treasury purchase
   at loading (`company_import`, closing the customs spread)? And if the
   company cannot pay when the goods arrive, who keeps them? The treasury has
   already paid natively by then.
4. **Price impact.** Private exports lower the native export price for the
   state too (§2 point 5). Accept that as the native market, or add a decree
   later (for example an export licence or quota)?

## 7. Prerequisites and order

1. **Step 3b** (roadmap): reliable "who received the goods" for DO trips,
   with treasury-paid trip fuel for state receivers (D40). The foreign freight
   of D39 reuses it.
2. **G1, or the runtime measurement probe**, for the export money event.
3. The owner's answers to §6.
4. A `state_ledger` external-inflow channel for `foreign_freight`.

## 8. Acceptance for the later step (host tests, then in game)

- Host tests: customs capability and labels, OFF default, save round trip and
  old-save load; manual customs entries untouched; no threshold writes on
  customs entries; exact measured payout; mismatch `WARN`; unmeasured or
  two-currency `ERROR` with no payout; multi-seller split; one payout per
  tonne; `foreign_freight` labelled and counted.
- In game: tick "Allow private exports" on one customs house. A private
  factory's goods reach it through a private DO. `[B2B GOODS] export` shows
  measured ≈ price, and the company's capital rises by the measured amount.
  The treasury does not keep it. The export trip shows `foreign_freight`.
  The month close in `state_ledger` has no unexplained delta.
