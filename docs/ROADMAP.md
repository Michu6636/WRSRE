# WRSRE roadmap: what to do next

**Last updated:** 2026-11-23. **Read this first** if you want to know what
the mod should do next and in which order. Each step links to the documents
that hold the decisions and details. When a step is finished, update this
file in the same pull request (see "How to run a step").

---

## 1. Where things stand

| Item | State |
|---|---|
| Owner decisions | D1–D48 in [economy direction](visions/economy-direction-2026-11-22.md) §1 (D21 and D37 amended 2026-11-23). D35's rent value is open for the owner to tune. No other owner questions are open. |
| **Code step 1**: native prices only, no state procurement | Done in [PR #100](https://github.com/Michu6636/WRSRE/pull/100). Host tests and the CI Windows build pass. The owner ran the build (save "Test 02"), but **the in-game step-1 check has not been run yet**. [Task record](tasks/active/2026-11-22-remove-market-multiplier-and-procurement.md). |
| **Bug fix**: private DOs served buildings under construction | Done in PR #100. Host tests and CI pass. **Not yet checked in game.** [Task record](tasks/active/2026-11-22-private-do-under-construction.md). |
| **Code step 2**: no fallbacks or made-up data | Merged in [PR #101](https://github.com/Michu6636/WRSRE/pull/101) (`823df769`). Host tests and the Windows CI build passed. The owner's in-game check is still pending. [Task record](tasks/active/2026-11-22-remove-fallbacks-and-made-up-data.md). |
| **Code step 3**: live B2B billing | Merged in [PR #102](https://github.com/Michu6636/WRSRE/pull/102) (`73a9d1d`): `private_shops` 3.8 has no delivery bill or foreign restock, customs buyers create no `b2b_state_buy` leg, and `private_logistics` 0.19.1 ships `billing = 1`, `dry_run = 0`. **First in-game run 2026-11-23 (10 days):** what happened worked; several paths were not exercised (see step 3). [Task record](tasks/active/2026-11-22-live-b2b-billing.md). |
| **Step 3b**: fix-ups from that run | Merged in [PR #105](https://github.com/Michu6636/WRSRE/pull/105) (supersedes #104), `private_logistics` 0.19.2-step3b: receiver-based DO fuel (D40), `ERROR NO CARGO RECORD` + `no_cargo`, honest billing labels. Host tests and the Windows CI build pass. **Not yet checked in game.** [Task record](tasks/active/2026-11-27-step-3b-state-do-fuel.md). |
| Company finances (step 4d) | **Planned.** Company health ladder: debt, credit hold, insolvency (D45). |
| Private export markets | **Later step (D41)**, own branch. Decisions D36–D39, D42–D48 recorded; [plan and investigation](visions/vision-private-export-markets.md). |
| Construction vision (state and private offices) | Owner decisions O1–O11 recorded; **not scheduled** (D13). [Vision](visions/vision-private-construction.md). |
| PR #100 | Merged into `main` on 2026-11-22 by the owner's instruction. It holds step 1, the bug fix and the docs above. The in-game checks of step 0 are still to do. |

---

## 2. Rules that apply to every step

- **No made-up data, ever (D8).** No sample, placeholder or invented fallback
  value. Missing data shows `UNAVAILABLE` or `ERROR <reason>`, very visibly.
- **No money from nothing (D20).** Every credit has a matching debit. The only
  exception is the fund's founding capital, created once per map, and later
  the foreign-market freight of export trips (D39).
- **Native prices only (D2), one domestic market (D3), no fixed or invented
  exchange rate (D21).** The only rate is the price-derived one for foreign
  trade (D42, later step).
- **The player chooses** the state/private balance (D1) and the welfare level
  (D18). Defaults must not force either.
- **Do not assume.** If a step needs a decision that no document records, ask
  the owner and record the answer in the decision register.
- The engineering loop is in [`AGENTS.md`](../AGENTS.md). Plugin rules (`/MT`,
  14-byte hooks, zero-allocation hot paths, INI encoding) are in
  [`plugins/AGENTS.md`](../plugins/AGENTS.md).

---

## 3. The plan, in order

This is the recommended order. The owner may reorder it. Item numbers
("§11 item N") refer to the change list in the
[economy direction §11](visions/economy-direction-2026-11-22.md#11-change-list-follow-up-tasks).

### Step 0: close out PR #100

- PR #100 merged by the owner's instruction on 2026-11-22 (step 1, the bug
  fix and the docs). Work continues in a new session from `main`.
- **Step 1 in-game check (pending; not run yet):** no `procured` / `[PROC]`
  log lines. Every remaining "charged … for state delivery" line must follow
  a real input unload into that building, never growth of its own output.
- **The old 0.70 check is replaced by step 3.** Step 3 removed the
  `private_shops` 0.70 delivery lines, so the step-1 check cannot look for
  them. The step-3 check covers that ground instead: one `[B2B GOODS]`
  payment per factory → shop delivery and no `private_shops` delivery
  charge.
- **In-game check of the bug fix** on a save with a private site under
  construction. Expected log lines:
  - `[AUTO DO] day N under construction: K storage/consumer site(s) excluded`;
  - `under construction: directions 0 (entry kept; restored once finished)`
    for an existing entry;
  - no `[B2B SITE] ... commit=0` lines at a site that is still `[BUILDING]`;
  - `reason=target_under_construction` on any `[DO TRIP]` to such a site.

### Step 2: remove fallbacks and made-up data

**Status 2026-11-22:** merged in PR #101 (`823df769`); all host tests and the
Windows CI build passed. `resource_pricing` v6 removes the
fallback/default/divisor settings and answers `UNPRICED`/`UNAVAILABLE`;
D32–D34 are wired into the consumers; the Almanac has no sample rows.
The in-game acceptance check is still to do.
[Task record](tasks/active/2026-11-22-remove-fallbacks-and-made-up-data.md).

**Goal:** every number comes from the game, a player policy or a documented
INI constant (invariant I6).

**Decided behaviour when data is missing** (owner answers, 2026-11-22):
- **D32, shop sale with no native price:** the game gives the good as in
  vanilla and no money moves. Log `ERROR UNPRICED` and count it for the
  Almanac.
- **D33, business bill with an unpriced resource** (construction estimate,
  wholesale delivery bill, B2B freight): refuse the whole transaction, with a
  loud error. Never bill partly.
- **D34, CPI unavailable:** skip wealth-class demand scaling that day, show
  `UNAVAILABLE` for runway and CPI, and the bank refuses new credit until CPI
  is back.

**Callers of `getPrice` / `getCpiDailyBasket` to update** (found
2026-11-22; search for the names):
- `citizen_retail.cpp`: `CheckoutAffordabilityGate` (D32; note that
  `min_cost` must not turn a missing price into a charge), `SolCpiDaily`
  and its 2.15 fallback (D34), `DestituteBreadCost`.
- `enterprise_base.cpp`: `CollectResidentialRent` destitute bread-day.
- `private_construction.cpp`: `PriceOne` and the panel-materials estimate
  loop (D33).
- `private_shops.cpp`: the foreign restock and the 0.70 delivery bill. Both
  go in step 3, but until then an unpriced delivery is refused (D33).
- `private_logistics.cpp`: `UnitPrice` (already returns 0 for unpriced;
  make the refusal per D33 and loud). `panel_fuel_runtime.h`
  `DiagnosticFuelPrice` already reports `invalid_price`.
- `republic_bank.cpp`: `CurrentEconomicScale` (D34: no CPI, no new credit).
- `taxes.cpp`: the City Hall CPI readout (removed in step 6; until then it
  shows `UNAVAILABLE`).
- `src/resource_pricing_api.h`: change the contract. `getPrice` returns
  `0.0f` for "no native price". `getCpiDailyBasket` returns `0.0f` for
  unavailable. Remove the 2.15 default from `TsmPricingCpiBasket`.

- `resource_pricing`: delete `[fallback_prices]`, `[fallback_prices_usd]` and
  `default` (`resource_pricing.cpp` about 41, 204, 285–295). A missing native
  price is **`UNPRICED`**, shown in warning colour. Check the economy
  direction §3.2 point 5 hypothesis about which native field is the base
  price before relying on it.
- Remove `usd_divisor` from the price path (D21; §11 item 1). Other plugins'
  `*_usd` keys move in step 7.
- `citizen_retail`: remove the **2.15 RUB/day default CPI basket**
  (`citizen_retail.cpp` about 16, 853–862, 1387, 1450). No CPI means the
  runway shows `UNAVAILABLE`, not a guess.
- `almanac_overlay_dev`: delete the synthetic rows `kEconomyRows`,
  `kLabourRows`, `kLogisticsRows` (`almanac_panel_view.cpp` about 63–94) and
  the `DrawTable` sample fallback (about 148). With no live service, show a
  full-panel **"NO DATA: <service> unavailable"**. Update `TESTING.md`,
  which still expects `SYNTHETIC` values (§11 item 9).
- **Acceptance:** a grep finds no fallback price, sample row or default basket.
  A host test proves a missing price shows as `UNPRICED`. The Almanac shows
  "NO DATA" with a service disabled.

### Step 3: live B2B billing

**Status 2026-11-23:** merged in PR #102 (`73a9d1d`). First in-game run
(owner, save "2209 - Testville", 10 game days, no month close): no `[PROC]`
lines, no `ERROR UNPRICED`, no buyer-insolvent or treasury skips; 1741
state-to-state deliveries settled at 0.00; the privatised clothing factory
paid the treasury the native price for state fabric (45 lots, 358.61 RUB, the
day total matches the lines); one private-to-private DO trip was paid
(1.01 RUB). **Not exercised yet:** a private seller paid by the state
(`b2b_state_buy`), private-to-private goods, a month-close `state_ledger`
reconciliation (I5), dollar mode. **Found:** 22 private DO trips to state
buildings were unpaid (`private_parties_required`, 41.33 RUB) and the
diagnostic lines print a hardcoded `billing=off`. Both go to step 3b.

**Earlier status 2026-11-22:** implemented on `arena/5a89acc8-wrsre` from `main` at
`823df769`. `private_shops` 3.8 no longer bills deliveries or restocks from
abroad and writes no shelf contents; only service fees move money there.
`private_logistics` 0.19.1 excludes a customs-house buyer from
`b2b_state_buy` and ships `billing = 1`, `dry_run = 0`. Host tests pass;
the Windows CI build passes; the owner's factory → shop check and the
`state_ledger` reconciliation are still pending.
[Task record](tasks/active/2026-11-22-live-b2b-billing.md).

**Goal:** private goods are paid for by the buyer on delivery at the native
price (D16, D19). Prerequisites and their order are in the economy direction
§4.4.

- `private_shops`: delete the 0.70 delivery bill (`wholesale_ratio`) and
  foreign restocking (§11 item 3). Keep pub service charges.
- `private_logistics`: exclude customs houses from `b2b_state_buy`
  (`b2b_settlement.h` about 430) so exports are never paid by the state
  (§11 item 4).
- Flip the defaults to `billing = 1`, `dry_run = 0` in
  `private_logistics.ini` and in the `g_billing` / `g_dryRun` initialisers.
- Update the [design doc §12](WRSRE%20DESIGN%20DOC.md) and
  [`DO_OFFICES.md`](private-logistics-runtime/DO_OFFICES.md). "Payments OFF"
  becomes historical.
- **Later step (D41):** companies receive their own export revenue and pay
  their own import cost (D5); see "Private export markets" under Later.
- **Acceptance:** one delivery equals one bill (no double settlement). The
  `state_ledger` reconciliation shows zero unexplained delta (I5). The owner
  checks factory → shop deliveries in game.
- **Known limits (documented, no extra settlement):** a customs house as the
  import **source** is charged as a native-price state sale, so the state
  carries the customs spread until G1/D5. Settlement hooks need a vehicle, so
  conveyor/pipeline transfers are probably unbilled — unverified. See
  [economy direction §4.4](visions/economy-direction-2026-11-22.md#44-live-billing-by-default-d16-prerequisites-in-order--all-done).

### Step 3b: fix-ups from the first step-3 in-game run

**Goal:** a private DO is paid for every verified trip by whoever receives
the goods, and the log tells the truth about billing.

**Status 2026-11-28:** merged in PR #105 (`0.19.2-step3b`); host tests and
the Windows CI build pass; the owner's in-game check is still pending.

**Background (2026-11-27):** The owner-provided pre-change run
recorded 661 adjacent `0x68F792` → `0x68F7F9` delivery pairs and 32/32 DO
assignments with exactly one receiver matching the logged target. That run
shows this pair is used by road DO trucks and corrects the older sea/rail-only
classification; it is investigation evidence, not validation of the changed
build. The log also reports 206 early no-cargo deliveries despite matching
loads and sufficient tonnage. Source inspection found an unverified `+0x10` /
pointer-hash cargo-slot key and destructive pointer-mismatch clearing; the log
cannot attribute each lost record to that defect. The fix uses the documented
`vehicle+0x04` index, fails closed when invalid, and adds capped errors and a
`no_cargo` day counter without guessing another cause. No post-change in-game
test has been run.

- **Trip direction.** Today the trip's target is called the "buyer" even
  when the truck *loaded* there (`dry_assignment_runtime.h`,
  `AssignmentSettlementEvent`). Use the committed, adjacent matched B2B
  building/vehicle delivery pair for the same open truck assignment. Exactly
  one distinct receiver is required; zero/multiple receivers, an invalid pair,
  or overflow pays nothing with `reason=direction_unknown`. Never use
  `originalTarget` to choose a payer. The treasury must never pay for a trip
  where the state was the seller.
- **D40.** Private receiver: that company pays (unchanged). State receiver
  (not customs): the **treasury** pays the private DO the same observed fuel
  amount through a verified transfer with payer = `nullptr` and label
  `b2b_state_do_fuel`; the existing `treasury_skips` handling applies.
  Customs receiver: not paid in this step (`customs_export_deferred`); D39
  belongs to the later export step. State-owned DOs stay untracked.
- **Honest billing labels.** The observer and diagnostic lines
  (`[FLEET FUEL]`, `[FLEET ASSIGNMENT]`, `[PANEL *]`, `[DRY COST]`, journey
  tracker) never move money, so they say `money_moved=0`. The plugin-start and
  every day-close line report the real state from `g_billing` / `g_dryRun`
  (`billing=live`, `billing=dry_run`, or `billing=off`). `[DRY COST]` runs
  only while billing is off; with billing live, emit
  `[DRY COST] inactive: billing live` once instead of repeating
  `saved=1 records=0`.
- Missing cargo and vehicle leg: log capped `[B2B GOODS] ERROR NO CARGO
  RECORD` lines with a daily `no_cargo` count for matched deliveries lacking
  cargo attribution. The `0x68F7F9` vehicle leg is displayed as a vehicle leg,
  not `UNPRICED` or `bill refused`.
- Day close: `do_fuel_private`, `do_fuel_state`, `direction_unknown`.
- **Acceptance:** host tests for the payer choice (private receiver, state
  receiver, customs, unknown direction, state seller never paid by the
  treasury) and the billing label in all three states. In game: a private DO
  trip delivering to a state factory shows `paid>0` with `b2b_state_do_fuel`,
  a pickup trip at a state building is charged to the receiving company, and
  the start and day-close lines show `billing=live`; no `billing=off` text
  appears while billing is live. Do not mark the in-game checklist complete
  based on host tests or the pre-change owner log.

### Step 4: privatisation money, welfare decree, housing trigger

Four separate tasks, each with its own task record.

- **4a. Privatisation money flows** (§11 item 5, second half; economy
  direction §4.5):
  - founding capital once per map, with a persisted flag, from the
    construction-materials bundle (D31);
  - the fund pays the state when buying a building (D24);
  - starting capital and legal reserve per building (D29);
  - nationalisation paid by the treasury, with leftover capital returned to
    the fund (D27, D30);
  - a saved toggle cooldown of about one month (D28);
  - set `all_enterprises_private = 0` and `auto_private_types` back to
    `3,6,8`.
  - Needs G7 (condition) and G8 (recipes). Without them, show `UNAVAILABLE`
    and refuse privatisation.
- **4b. Welfare decree** (D18; §11 item 16; economy direction §6.4): one decree
  with four tiers replacing the `*_ratio_to_min` keys. It goes in the Almanac
  as the first decree (§7).
- **4c. Housing trigger** (D23; §11 item 15; economy direction §8.3): trigger
  the native flat search for established households, retire
  `FindRelocationCandidate` and the eviction penalty, and replace the 0.5
  fallbacks with visible errors. Needs G6.

- **4d. Company finances** (D45; [export plan §4](visions/vision-private-export-markets.md#4-owner-decisions-2026-11-23)):
  the company health ladder for every private company. Healthy (≥ legal
  reserve) → strained (no payouts to the fund) → in debt (buys on credit,
  interest at the bank rate, a share of each income repays) → credit hold
  (debt above the limit: no new purchases, enforced by switching off its
  DOs' deliveries of bought inputs; selling never stops) → insolvent (a
  decree chooses bail out, restructure or liquidate). Credit limit = the
  company's legal reserve; the hold lifts at half the limit; a grace period
  before insolvency. Needs 4a (reserve, fund). Propose the repayment share,
  grace period and interest from captured data, and ask the owner before
  shipping them.

### Step 5: wages

- **Private wage fix** (D14; economy direction §6.3, "Code check
  2026-11-22"). Today private pay is the state sector rate × a premium.
  Instead: the sector baseline from the INI at map start × the CPI index,
  plus the profit/understaffing premium. Cut the link to the live state
  table, let the hike test use the D29 reserve, and fix the `[WAGE AUDIT]`
  log label that always prints the services sector.
- **State wage indexation decree** (D14).
- **High-education slot premium** after G4 (D15; §6.2).
- §11 item 6.

### Step 6: shop prices and the policy surface

- `enterprise_base`: private shop price = landed cost × (1 + category margin)
  × (1 + VAT), with bounded monthly adaptation (§11 item 5, first half;
  economy direction §5.2).
- `citizen_retail`: state shop price × the state retail price level decree
  (§11 item 7; §5.3).
- Remove the City Hall TAX tile, fiscal panel, macro rows and bank-rate
  slider. First move any readout that exists only there (CPI, poverty rate,
  commercial solvency) into the Almanac (D7; §11 item 8).
- The other decrees in economy direction §7 (settlement grant, minimum wage,
  price ceiling, rent control), with `starting_wallet = 0` (D25; §11 items
  10, 13).

### Step 7: household investment and currency parity

- Rich citizens' savings above the Affluent line flow into the fund (D26;
  §11 item 14).
- Remove every remaining `usd_divisor` and `*_usd` conversion, and re-base
  flat money constants on the CPI basket or native construction cost (D21;
  §11 item 12). **The divisor was an assumption, not an exchange rate:** native
  USD prices are of the same size as RUB prices (usually 10–30 % higher), so
  `usd_divisor = 50` made USD-mode amounts about 50× too small. Do not replace
  it with another rate. D35 already gives the CPI rent a separate USD constant;
  re-base `reference_cpi_rub` on captured native prices here too.

### Step 8: calibration

- Capture a reference save, then compute wage bands and break-even margins
  from **captured** data (economy direction §9; §11 item 11).
- Encode invariants I1–I9 as tests against the captured fixtures.

### Later (not scheduled)

- **Private and state construction offices**:
  [vision](visions/vision-private-construction.md). Decided: who pays and
  who builds are separate, per-load billing, 8 % fee, stop rule B, a
  construction wage sector at 3.75 RUB/day. Waits for D13 ("core loop
  polished"). Its smallest first piece is per-load billing with state
  offices only (vision §7).
- **Fund dividends and "unlocking" building types** for the player to place
  (D12).
- **Politics module**: factions, elections, approval (D17).
- **A market model that changes native prices** (D2; Ghidra G3).
- **Private export markets and foreign trade policy** (D41, own branch):
  an Almanac "Foreign trade" page where every customs house is a route,
  closed by default, with per-good bans, export duties, import tariffs and
  monthly quotas (D36, D43); trade law also binds manual customs targets
  (D37); company export revenue measured exactly (D38) and paid in the
  active currency through a price-derived exchange rate (D42); export
  freight paid by the foreign market (D39); import cost = the measured
  treasury payment plus tariff (D44); new targets export the company's own
  products (D47); one source per truckload (D46). Needs step 3b, step 4d,
  G1 or a runtime probe, and G10 (per-resource toggles; investigate first).
  [Plan and investigation](visions/vision-private-export-markets.md).

---

## 4. Verification queue (research, not decisions)

| ID | What | Needed by |
|---|---|---|
| G1 | Customs sale/purchase event and provenance | Private export markets (later, D41) |
| G10 | Turning one resource on or off in a DO task (native selector entries) | Private export markets: bans, quotas, D47 |
| G11 | A customs house's border and currency (optional) | Private export markets: trade page |
| G4 | Worker slot counts and which slot a worker holds | Step 5 |
| G5 | Native price of the `"workers"` pseudo-resource (a runtime log may do) | Step 8 |
| G6 | Native living standard and dorm affordability | Step 4c |
| G7 | Building condition field | Step 4a |
| G8 | Native production recipes in memory | Step 4a |
| G2, G9, G3 | Person health/loyalty, building-type catalogue, price recompute | Almanac readouts, later work |
| C1–C9 | Construction office internals | Construction vision (unscheduled) |
| Runtime | A broke citizen's happiness and health fall over the following days (D9) | Any step; economy direction §9 |
| Runtime | Step 1 and the under-construction fix in game | Step 0 |

Details: [economy direction §9.1](visions/economy-direction-2026-11-22.md#91-ghidra-queue)
and [construction vision §9](visions/vision-private-construction.md). One
Ghidra operator at a time; index results in `docs/ghidra-index/`.

---

## 5. How to run a step

1. **Create a task record** from [`tasks/TEMPLATE.md`](tasks/TEMPLATE.md) as
   `docs/tasks/active/YYYY-MM-DD-<slug>.md`, and list it in
   [`tasks/README.md`](tasks/README.md).
2. **Investigate** with the code citations in the economy direction. Lines
   move, so search for the names.
3. **Implement** in small edits. Keep save compatibility, or state in the
   task record why it changes.
4. **Test on the host:** CI discovers unittest cases in each file, even when
   the file has no `unittest.main()` block:
   `for f in $(find tests -name 'test_*.py' | sort); do PYTHONPATH=. python3 -m unittest discover -s "$(dirname "$f")" -p "$(basename "$f")" -v; done`.
   There is no local MSVC, so CI's `Windows (MSVC + SEH)` job is the real
   build.
5. **Open a pull request.** CI runs docs hygiene, the host tests and the
   Windows build, and uploads the `windows-plugins` artifact (7 days).
6. **Owner in-game check** with the artifact or `build.bat clean wrsre`.
   Install the single `wrsre.dll` or the separate DLLs, never both.
7. **Update the docs in the same PR:** the task record, this roadmap (§1
   and the step's status), the economy direction §11 status, and
   [`SUPERSEDED.md`](SUPERSEDED.md) when an older finding stops being true.

---

## 6. Where everything is

| Need | Document |
|---|---|
| Every economic decision (D1–D48) and the change list | [visions/economy-direction-2026-11-22.md](visions/economy-direction-2026-11-22.md) |
| Construction: state and private offices (O1–O11) | [visions/vision-private-construction.md](visions/vision-private-construction.md) |
| Private export markets and foreign trade policy (later step, D36–D48) | [visions/vision-private-export-markets.md](visions/vision-private-export-markets.md) |
| Private logistics vision (partly overridden) | [visions/vision-private-logistics.md](visions/vision-private-logistics.md) |
| Task records and their status | [tasks/README.md](tasks/README.md) |
| Architecture and subsystem boundaries | [WRSRE DESIGN DOC.md](WRSRE%20DESIGN%20DOC.md) (the economy direction wins on conflicts) |
| Older findings that are no longer true | [SUPERSEDED.md](SUPERSEDED.md) |
| Full documentation index | [README.md](README.md) |
