# WRSRE roadmap: what to do next

**Last updated:** 2026-11-22. **Read this first** if you want to know what
the mod should do next and in which order. Each step links to the documents
that hold the decisions and details. When a step is finished, update this
file in the same pull request (see "How to run a step").

---

## 1. Where things stand

| Item | State |
|---|---|
| Owner decisions | Complete for the economy: D1–D31 in [economy direction](visions/economy-direction-2026-11-22.md) §1. No owner questions are open there. |
| **Code step 1**: native prices only, no state procurement | Done in [PR #100](https://github.com/Michu6636/WRSRE/pull/100). Host tests and the CI Windows build pass. The owner ran it in game (save "Test 02"): no procurement lines, and state delivery bills appear only after real deliveries. [Task record](tasks/active/2026-11-22-remove-market-multiplier-and-procurement.md). |
| **Bug fix**: private DOs served buildings under construction | Done in PR #100. Host tests and CI pass. **Not yet checked in game.** [Task record](tasks/active/2026-11-22-private-do-under-construction.md). |
| Construction vision (state and private offices) | Owner decisions O1–O11 recorded; **not scheduled** (D13). [Vision](visions/vision-private-construction.md). |
| PR #100 | Open, not merged. It holds step 1, the bug fix and the docs above. |

---

## 2. Rules that apply to every step

- **No made-up data, ever (D8).** No sample, placeholder or invented fallback
  value. Missing data shows `UNAVAILABLE` or `ERROR <reason>`, very visibly.
- **No money from nothing (D20).** Every credit has a matching debit. The only
  exception is the fund's founding capital, created once per map.
- **Native prices only (D2), one domestic market (D3), no exchange rate (D21).**
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

- The owner reviews and merges PR #100.
- **In-game check of the bug fix** on a save with a private site under
  construction. Expected log lines:
  - `[AUTO DO] day N under construction: K storage/consumer site(s) excluded`;
  - `under construction: directions 0 (entry kept; restored once finished)`
    for an existing entry;
  - no `[B2B SITE] ... commit=0` lines at a site that is still `[BUILDING]`;
  - `reason=target_under_construction` on any `[DO TRIP]` to such a site.

### Step 2: remove fallbacks and made-up data

**Goal:** every number comes from the game, a player policy or a documented
INI constant (invariant I6).

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
- **After Ghidra G1:** companies receive their own export revenue and pay
  their own import cost (D5).
- **Acceptance:** one delivery equals one bill (no double settlement). The
  `state_ledger` reconciliation shows zero unexplained delta (I5). The owner
  checks factory → shop deliveries in game.

### Step 4: privatisation money, welfare decree, housing trigger

Three separate tasks, each with its own task record.

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
  §11 item 12).

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

---

## 4. Verification queue (research, not decisions)

| ID | What | Needed by |
|---|---|---|
| G1 | Customs sale/purchase event and provenance | Step 3 (company exports) |
| G4 | Worker slot counts and which slot a worker holds | Step 5 |
| G5 | Native price of the `"workers"` pseudo-resource (a runtime log may do) | Step 8 |
| G6 | Native living standard and dorm affordability | Step 4c |
| G7 | Building condition field | Step 4a |
| G8 | Native production recipes in memory | Step 4a |
| G2, G9, G3 | Person health/loyalty, building-type catalogue, price recompute | Almanac readouts, later work |
| C1–C9 | Construction office internals | Construction vision (unscheduled) |
| Runtime | A broke citizen's happiness and health fall over the following days (D9) | Any step; economy direction §9 |
| Runtime | The under-construction fix in game | Step 0 |

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
4. **Test on the host:**
   `for f in $(find tests -name 'test_*.py'); do PYTHONPATH=. python3 $f; done`.
   Every test file must end with `unittest.main()`, because CI runs each file
   directly. There is no local MSVC, so CI's `Windows (MSVC + SEH)` job is the
   real build.
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
| Every economic decision (D1–D31) and the change list | [visions/economy-direction-2026-11-22.md](visions/economy-direction-2026-11-22.md) |
| Construction: state and private offices (O1–O11) | [visions/vision-private-construction.md](visions/vision-private-construction.md) |
| Private logistics vision (partly overridden) | [visions/vision-private-logistics.md](visions/vision-private-logistics.md) |
| Task records and their status | [tasks/README.md](tasks/README.md) |
| Architecture and subsystem boundaries | [WRSRE DESIGN DOC.md](WRSRE%20DESIGN%20DOC.md) (the economy direction wins on conflicts) |
| Older findings that are no longer true | [SUPERSEDED.md](SUPERSEDED.md) |
| Full documentation index | [README.md](README.md) |
