# Vision: construction clients and builders (state and private offices)

**Status:** tentative owner vision, recorded 2026-11-22. **Not scheduled.**
[Economy direction D13](economy-direction-2026-11-22.md) still applies:
private construction stays as it is until the core economic loop is polished.
This file records where private construction should go after that, and the
bug that prompted it.

Sections marked **Owner** are the owner's stated vision. Sections marked
**Proposed** are recommendations still awaiting an owner decision. **Open**
items need a decision or native research before implementation.

---

## 1. Trigger: private DOs deliver to buildings under construction (bug)

### 1.1 What happens

Found by the owner in a test session (`tesmioloader.log`, PR #100 build,
2026-11-22, save "Test 02"):

- The pub `MIRRORZ_pub` was tagged for private construction (`[PRIV CLICK]`,
  `[CLAIMED]` total 3292.72 RUB) while still a construction site.
- `private_logistics` automatic target assistance (`[AUTO DO]`) added it to
  the private DO `muddy_distribution` as a managed delivery target:
  `[AUTO DO] native target=...FF0968A0 type=23 policy=managed requested_mask=2 ... unload_all=1 unload_pct=0.300`.
- The DO kept sending trucks there. The log has **302** logged unload
  attempts at the pub between 17:17:36 and 17:19:52 (the `[B2B SITE]` log is
  capped per day, so the real count may be higher). Every one is `commit=0`:
  the native stock-add call at RVA `0x68F792` refused it. None is
  `commit=1`, while the same call committed 566 times for finished buildings
  in the same log. A construction site has no working storage, so the
  delivery can never complete.
- Each failed trip still produced a `[DO TRIP]` fuel charge to the pub
  (for example `charge=6.09 paid=6.09 eligible=1`, then `charge=10.21`). This
  only happened because `billing=0, dry_run=1`. With live billing, the pub's
  company would pay real money for trips that delivered nothing.

### 1.2 Root cause

`private_logistics.cpp`, automatic target discovery (about line 1768): a
private building becomes a target candidate when it is valid, not demolished,
private and has storage capabilities. **It never checks that the building is
finished** (`TsmIsFinished`, `game_world.h` line 239: `+TSM_BLD_BUILT >= 1.0`).

The reconcile pass over a DO's existing entries (about line 1830) has the
same gap. It re-applies delivery directions to an entry whose target is
still under construction.

### 1.3 Fix (implemented 2026-11-22, host-tested, no in-game run)

The owner chose to implement the fix on PR #100. Task record:
[2026-11-22-private-do-under-construction.md](../tasks/active/2026-11-22-private-do-under-construction.md).

1. **Discovery:** an unfinished building is never a candidate, private or
   opted-in state storage. It stays indexed, so existing entries still match
   it, and it becomes a candidate on the first daily pass after it is
   finished. An unfinished DO is not treated as an office.
2. **Existing entries:** an entry whose target is unfinished gets directions
   0 (no load, no unload). The entry is kept and its directions return once
   the building is finished. Removing entries would fight the native planner
   and the player.
3. **Settlement:** a DO trip whose buyer is unfinished is never charged
   (`reason=target_under_construction`).
4. **Fail closed:** an unreadable build-progress float counts as unfinished.
5. **Tests:** `target_policy_runtime_test.cpp` (`UnderConstruction`),
   `dry_assignment_runtime_test.cpp`, `test_site_permissions.py`.

### 1.4 Why this points at a bigger change

Today a privately funded site gets its materials from **nowhere physical**.
`private_construction` (v3.6.1) takes the cost from the investment fund and
writes the site's build progress directly, up to `finish_handoff_frac`
(0.99). It then waits for a **state** construction office to deliver the last
percent, because a progress write by the plugin finishes the building
invisibly (see the v3.4.3 and v3.5.1 notes at the top of
`private_construction.cpp`).

This breaks the rule the logistics vision sets for everything else: *vanilla
logistics owns physical movement* ([vision-private-logistics.md](vision-private-logistics.md),
core principle 1). Private DOs then also treat the site as a shop that wants
deliveries, which it is not.

---

## 2. The vision (Owner)

### 2.1 First statement (2026-11-22)

> Allow **construction offices to be privatised**. They can **purchase
> vehicles** and use **the same system we use for private logistics** to find
> buildings to construct: **private ones, or buildings that allow private
> construction**. They grab goods from the storage facilities they are
> allowed to access, using **the same checkbox as "allow private logistics to
> take"**, and they construct by transporting the goods. **What to do about
> workers will be decided later.** This is a tentative vision.

### 2.2 Follow-up (2026-11-22)

The owner noted that real governments also hire private builders for
buildings the state will own. The mod must simulate the private sector
building for itself and the state building separately, and perhaps let the
state build with private contractors. The native instant build (USD or
roubles, outside realistic mode) already almost simulates that.

| ID | Owner statement | Status |
|---|---|---|
| O1 | **State construction offices can build private sites.** They already do with auto search on, or when the player assigns the site. The office **gets the money and operates as a normal state office does, with its income going to the treasury.** | Decided |
| O2 | **Workdays are billed to the site owner**, and the money goes **to the workers**, who are marked as **construction workers**, because construction offices have no fixed workplace for them. | Decided (the marker's form is open, §5.5) |
| O3 | **The site owner pays for materials loaded and workdays done**, as the work happens, **not when the building is finished**. | Decided |
| O4 | **Construction workdays are paid at a separate construction wage**, if workers on a site can be identified (C8). The construction wage works like any other wage: **construction is its own industry category.** The player sets it in the Almanac for state offices, and private offices set it automatically like other private firms. | Decided, conditional on C8 |
| O5 | **Private payers build only with construction offices (private or state).** The instant build is not available to them: realistic mode has no instant build for the player either. | Decided (§5.7) |
| O6 | **"Allow private construction" on state sites is OFF by default.** | Decided |
| O7 | **If the payer cannot pay, construction is stopped.** | Decided |
| O8 | **Contractor fee of 8 %** as recommended in §5.4.1. | Decided |
| O9 | **Materials already loaded are not refunded** if the site is cancelled or demolished. | Decided |

---

## 3. How real-life construction is organised

Background for the design. It is a summary, not a rule the mod must copy.

- **The client and the builder are different parties.** The client pays and
  owns the result. The builder (contractor) is paid to build it. Who builds a
  building does not decide who owns it.
- **States use both their own crews and contractors.** Public service
  provision does not mean the government also produces the asset. Most
  government assets are bought from the private sector or built by private
  firms under contract to government specifications
  ([OECD 2008, Public-Private Partnerships](https://www.oecd.org/content/dam/oecd/en/publications/reports/2008/05/public-private-partnerships_g1gh8c7d/9789264046733-en.pdf)).
  The OECD describes a range: complete government production, then
  traditional procurement, PPPs, concessions and privatisation (same source).
- **The Soviet Union moved from direct labour to contracts too.** In
  Khrushchev's reform, many small building trusts working by direct labour
  were to be replaced by specialised trusts working by contract. A general
  contractor on each site signed contracts with specialised trusts, and
  contracts between client and builder replaced direct labour
  ([Warwick PERSA working paper 61](https://warwick.ac.uk/fac/soc/economics/staff/mharrison/archive/persa/061.pdf)).
  Most construction machinery was owned by special machinery trusts and
  rented to the building trust that needed it
  ([US report, Industrialized building in the Soviet Union](https://www.govinfo.gov/content/pkg/GOVPUB-C13-7d84f08cc44cb9903107dd699b9bfa39/pdf/GOVPUB-C13-7d84f08cc44cb9903107dd699b9bfa39.pdf)).
  In game terms, Soviet state builders were already separate organisations
  that contracted with the state client.
- **Turnkey contracts.** The same report describes combines that delivered
  turnkey buildings, from precast factory to erection (same source). The
  native instant build is a turnkey contract: the player pays a price and
  receives a finished building, without managing materials or workers. The
  mod already calls the USD labour rate in `private_construction` a
  "Western turnkey contractor" rate (`contractor_wage_per_day_usd`).
- **Clients pay as the work progresses.** Contractors are paid in interim
  payments, usually monthly, for the value of work completed and materials
  on site to date
  ([interim valuation guide](https://www.surveyorsuccess.co.uk/article/construction-valuation-guide-interim-payments)).
  O3 is the same principle, settled per load instead of per month.

---

## 4. How the native construction office works

Summary from the community and official wikis and player reports, for
design only. Native layouts still need Ghidra confirmation (§9).

- **Vehicles carry everything.** The office schedules construction vehicles
  to pick up construction materials at sources and take them to the site.
  Buses fetch workers. Construction starts once workers and resources arrive
  ([Fandom wiki](https://workers-resources.fandom.com/wiki/Construction_office)).
- **Vehicle types are fixed by cargo.** Dump trucks carry gravel and asphalt.
  Open-hull trucks carry steel, bricks, prefab panels and boards. Mixers carry
  concrete, covered-hull trucks carry components, and buses carry workers.
  Cranes, excavators and bulldozers are mechanisms (same source).
- **The office has no storage of its own** apart from a 30 t fuel tank. The
  player assigns **sources of materials** in the office window (same source).
- **Sites come from an assignment list or auto search.** With auto search
  off, the player assigns each site. With it on, the office picks up
  unfinished sites within a configurable range (default 1000 m, up to about
  3000 m per players). Several offices can work on the same site
  ([official wiki](https://wiki.hoodedhorse.com/Workers_Resources_Soviet_Republic/Construction_office);
  [Steam discussion](https://steamcommunity.com/app/784150/discussions/0/2289464808482561542/)).
- **Workers are a source too.** The office's bus fetches workers from a
  designated bus stop or station. Workers living within walking distance walk
  to the site
  ([Steam discussion](https://steamcommunity.com/app/784150/discussions/0/1643178512762431179/)).
- **Offices buy vehicles** with roubles or dollars from the office window
  (official wiki).
- The repo already knows one piece of native state: the per-building vehicle
  vector `bld+0xC70/+0xC78` is generic and **is also read by construction
  offices** ([AGENT_REPORT_private_industry.md](../AGENT_REPORT_private_industry.md)).

The native construction office already does exactly what the vision needs
physically. The mod's job is ownership, permissions and money, not movement.

---

## 5. Recommended model: who pays and who builds are separate

Each line is marked **Owner** (decided), **Proposed** (my recommendation,
awaiting a decision) or **Open**.

### 5.1 Two questions per site (Proposed)

Every construction site answers two separate questions:

1. **Client: who pays, and who will own the building?** The state treasury,
   or a private payer (the investment fund or a company).
2. **Builder: who does the work?** A state construction office, a private
   construction office, or the native instant build.

**Ownership follows the client, never the builder.** A private site built
by a state office becomes private. A state site built by a private company
stays state-owned.

| Client ↓ / Builder → | State construction office | Private construction office | Native instant build (USD/RUB) |
|---|---|---|---|
| **State** | Native game, no mod bills (as today) | **Public contract:** the treasury pays the company as work is done. Needs "Allow private construction" on the site (§5.3) | Native turnkey, unchanged |
| **Private** | **The state as contractor (O1):** the owner pays as work is done (§5.4) | **Private contract:** the owner pays the company as work is done | **Not available (O5)** |

Several offices can work on the same site in the native game, so one site
can have mixed builders. Per-load billing (§5.4) handles that without
special cases, because each load is billed to the site's client whichever
office brought it.

### 5.2 What happens to the CON tile and `private_construction` (Proposed)

- The CON tile keeps one meaning: **this site has a private client**. The
  fund pays, and the building is private when finished.
- It stops meaning "the plugin builds it". Today `private_construction` takes
  the whole cost from the fund and writes progress itself, then waits for a
  state office to finish the last percent (§1.4). Under this model, native
  offices (state or private) build it, and the client pays per load.
- The money-to-progress path stays until per-load billing works in game
  (D13). Then it is retired, together with the 99 % hand-off and the
  invisible-finish risk.
- `NativeCostForBuilding`, the site registry and persistence stay. They
  provide the budget preview ("this site will cost about X") and the
  privately-built record D24 needs.

### 5.3 Does the state need a separate "build with private contractors" mode? (Proposed: no)

- **Outside realistic mode**, the instant build already is the state hiring
  a turnkey contractor, as the owner noted. Nothing more is needed.
- **In realistic mode**, the per-site checkbox **"Allow private
  construction"** (default OFF, O6) is the contract. The player ticks it on a
  state site, and private offices may then build it and bill the treasury.
  This needs no new mode or menu, and the player chooses per site, which
  matches "the state vs private balance is the player's choice".
- A state site without the checkbox is never touched by a private office
  (follows from O6).

### 5.4 Paying as work is done (Owner: O3)

Each event is billed once, at the moment it happens, to the site's client:

| Event | Payer → payee | Amount |
|---|---|---|
| Material **loaded** for the site at a storage | client → storage owner (state treasury or private company) | tons × native price at loading (the same lot rule as goods) |
| Workday **done** at the site | client → the citizens who worked it (O2) | workdays × construction wage (O4, §5.5) |
| Transport (fuel) | client → the building office | observed fuel × fuel price (the same rule as a DO trip) |
| Contractor fee | client → the building office | 8 % of the load's materials + workdays (O8, §5.4.1) |

- **State office on a private site (O1):** the office receives transport and
  the fee, and its income goes to the treasury like any state office. Payment
  for materials taken from state storages goes to the treasury as the storage
  owner. Workday money goes to the workers (O2).

#### 5.4.1 Contractor fee (Owner: O8, 8 % accepted)

**Why a fee is needed:** under O2 and O3 the client pays materials to the
storage owner and workdays to the workers directly. Without a fee, a private
construction office would earn only fuel reimbursement. It could never
recover its vehicles, electricity or upkeep, so it could never make a profit,
build a reserve or pay anything to the fund. Real contractors price overhead
and profit into every job.

**Real-world reference points:**
- Soviet and Russian construction estimates include "planned savings", the
  builder's planned profit, at about **6–8 %** of direct costs plus overheads
  ([Jarvis Group, Construction Costs](https://www.jarvisgroup.net/construction-costs)).
- A US government report on Soviet building notes that estimated costs were
  usually set about **12–15 %** above current costs. Its US comparison
  estimate (April 1969 prices) uses **15 %** for contractors' overhead and
  profit
  ([US report, Industrialized building in the Soviet Union](https://www.govinfo.gov/content/pkg/GOVPUB-C13-7d84f08cc44cb9903107dd699b9bfa39/pdf/GOVPUB-C13-7d84f08cc44cb9903107dd699b9bfa39.pdf)).

**Recommendation:**
1. One INI value, `contractor_fee_percent`, applied to the materials +
   workdays value of every load and workday billed, and paid to the office
   that did the work.
2. **The same percentage for state and private offices.** The private payer
   then chooses between offices on availability and speed, not price, and the
   state office's fee is ordinary state income (O1).
3. **Shipped value 8 %**, the top of the Soviet planned-savings norm. It
   covers vehicles and upkeep without making private building far dearer than
   state building. The player can change it, and a later decree could expose
   it.
4. If the value is missing or invalid, the fee shows `UNAVAILABLE` and
   private billing stays off. There is no hidden fallback.
- **Owner (O9):** materials loaded but not yet unloaded at the site stay
  paid. If the site is cancelled or demolished, they are not refunded.
- **Owner (O7):** if the client cannot pay, construction is **stopped**, not
  served on credit. Its status says `UNPAID — construction stopped`.
  Native research C9 checks whether the game has a suspend flag the mod can
  set.
- **Open: when exactly the site stops (§10 Q1).** O7 says it stops. What is
  left to decide is the moment:
  - **(A) Check before each step.** Before a truck loads or a workday counts,
    the mod checks whether the payer has enough for that one step. If not,
    the step does not happen and the site stops. The payer never goes below
    zero.
  - **(B) Stop after the money runs out.** Every step is billed. When a bill
    takes the payer below zero, the site stops after that step. The payer
    can go slightly negative, by at most one load or workday.

  Either way, construction **resumes automatically** once the payer has
  money again. (A) needs the mod to block a native load before it happens,
  which may not be possible (C5, C9). (B) only needs to react after a load.
- **Proposed:** a state client (public contract) pays from the treasury, and
  the same stop rule applies when the treasury cannot pay.

### 5.5 Workers (Owner: O2)

- **Owner:** workdays are billed to the client, and the money goes to the
  citizens who did the work. They are marked as **construction workers**,
  because offices have no fixed workplace for them.
- **Owner (O4):** they are paid a **separate construction wage**, if they can
  be identified (C8). Construction becomes its **own wage sector** in
  `citizen_labor`. Today it is part of "infrastructure" (energy, transport,
  storage, construction), with a 3.50 RUB/day base.
  - **State offices:** the player sets the construction sector wage in the
    Almanac, like any other sector.
  - **Private offices:** automatic, like any private firm. That uses the
    private wage mechanism, which first needs the fix described in the
    economy direction §6.3 (code check 2026-11-22). Today private wages are
    the state rate × a premium, and nothing reads the CPI.
- **Proposed:** the new sector starts at the current infrastructure rate
  (3.50 RUB/day), so nothing changes on day one, and the player tunes it.
  Reference only: in the Soviet Interview Project data, construction workers
  earned about 191 RUB/month against about 160 RUB/month for all non-farm
  workers, roughly 19 % above average
  ([Gregory and Kohlhase 1988](https://www.uh.edu/~kohlhase/GregoryKohlhase_RESTAT_1988.pdf)).
- **Proposed form of the marker:** a per-citizen, per-day flag
  "construction worker at site X". `citizen_labor` uses it for that day's
  wage and status instead of a workplace. The Almanac can count construction
  workers like any other job.
- **Proposed:** a construction workday replaces that day's normal job pay
  and is never added on top of it. A citizen is paid once per day.
- **Open, native research C8:** can the mod see *which* citizens worked at a
  site on a given day? If it cannot, the money must not be guessed onto
  random citizens (no made-up data). The workday bill is then shown as
  `UNAVAILABLE` and not charged until the data exists.

### 5.6 Private construction offices (Owner, from §2.1)

- **Privatising (Proposed):** the PRIV tile appears on construction offices.
  Price, starting capital and nationalisation follow D24, D29, D27 and D30.
  The office's inputs for D29 are fuel and vehicle upkeep.
- **Vehicles (Owner: "can purchase vehicles"):**
  - **Proposed:** purchases are paid from the company's capital at the
    native vehicle price, never by the treasury. Selling a vehicle credits
    the company (D20, D21).
  - **Open (C4):** where the native buy button takes its money. Until that
    is known, a private office must not buy vehicles from the treasury.
- **Finding sites (Owner):** the `[AUTO DO]` pattern (daily reconcile, native
  list, game moves everything). Eligible sites: private-client sites, and
  state sites with "Allow private construction" (§5.3). The native range
  limit stays.
- **Materials (Owner):** sources are written from the storages the private
  logistics rules allow:
  - state storages with "Allow private DO loading (take goods)" checked;
  - private storages under the rule private DOs use today.

  Goods move only in native vehicles.
- **Fuel (Proposed):** the office's tank is filled by normal deliveries,
  settled as a B2B purchase.
- **Progress (Proposed):** comes only from the native mechanism. The mod
  writes no progress on sites built by offices.

### 5.7 No instant build for private payers (Owner: O5)

The owner asked whether private construction should be instant instead. The
recommendation agreed with O5: **no**. The reasons:

- **Realistic mode has no instant build**, and realistic mode is where this
  economy matters most. A private feature that only exists outside it would
  behave differently in each mode.
- **An instant build skips the domestic economy.** It uses no domestic
  materials, no construction workers and no transport, so private
  investment would create no demand for factories, storages or jobs.
- **It brings back the problem §5.2 removes.** An instant private build would
  mean money in, building out, with the mod writing build progress itself,
  which is the source of the invisible-finish bug.
- **Speed already has a lever.** A payer who wants faster building uses more
  offices (state or private) on the site, as the native game allows.

---

## 6. Relation to existing decisions and code

- **D13:** private construction stays as it is for now. This vision is the
  later step.
- **D12:** the fund "unlocks" high-demand building types for the player to
  place. They would be placed as private-client sites (§5.2).
- **D24:** no fund payment for a privately built building. A private-client
  site paid per load is privately built, whichever office built it.
- **D20:** every flow in §5.4 moves existing money between parties.
- **`private_logistics` `[AUTO DO]`:** the same discovery, daily reconcile,
  permission table and checkbox code, extended to construction office lists.
  The §1 fix keeps DOs off sites, which only construction offices serve.
- **`private_construction`:** pricing, registry and persistence are reused.
  The money-to-progress path is retired (§5.2).

---

## 7. Phases (Proposed)

0. **Fix the §1 bug.** Done (§1.3).
1. **Observe only:** log every construction office's sites, sources,
   vehicles and auto-search range, plus every material load and workday at a
   site, with the client and the office. No money and no writes.
2. **Per-load billing, dry run:** compute §5.4 for every load and workday,
   with state offices on private sites first (O1). This works before any
   office is privatised.
3. **Live billing**, then retire the money-to-progress path.
4. **Worker pay** through `citizen_labor` (O2), after C8.
5. **"Allow private construction" checkbox** on state sites.
6. **Privatised offices**, company-paid vehicles, automatic sites and
   sources (dry run first).

Phases 1–3 deliver O1 and O3 with only state offices. That is the smallest
useful step, and it removes the plugin's progress writes.

---

## 8. Mapping to game modes

| Mode | State client | Private client |
|---|---|---|
| Instant build (not realistic) | native price, unchanged | not available (O5) |
| Realistic, state office | native, no bills | owner pays per load (O1, O3) |
| Realistic, private office | treasury pays per load (checkbox) | owner pays per load |

---

## 9. Native research needed (proposed Ghidra targets)

Not added to the economy direction G queue until the owner schedules this
work.

| ID | Question | Needed for |
|---|---|---|
| C1 | Construction office building type number(s) and type-descriptor fields | Recognising offices; PRIV tile |
| C2 | Layout of the office's site assignment list, auto-search flag and range | §5.6 |
| C3 | Layout of the office's "sources of materials" list, including the workers source | §5.6, §5.5 |
| C4 | Native buy-vehicle and sell-vehicle path: who is debited and credited | §5.6 |
| C5 | Where a construction vehicle's load at a storage and unload at a site are committed. DO trucks at a site logged `commit=0` at the B2B site RVA `0x68F792`; the construction path is probably different | §5.4 per-load billing |
| C6 | Whether a site can be restricted to specific offices | §5.3 |
| C7 | How native construction progress (`building+0x604`) advances per delivery and per workday | §5.4 workday count |
| C8 | Which citizens worked at a site on a given day (bus passengers and walk-ins) | §5.5 worker pay |
| C9 | A native "suspend construction" flag the mod can set and clear | §5.4 halt rule |

---

## 10. Open owner questions

1. **When the site stops** for non-payment: (A) check before each step, so
   the payer never goes negative, or (B) stop right after the step that
   empties the account, at most one step negative (§5.4)?
2. **Starting value of the construction sector wage:** the current
   infrastructure rate of 3.50 RUB/day (§5.5)?
