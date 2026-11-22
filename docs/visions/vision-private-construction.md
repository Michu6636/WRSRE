# Vision: private construction offices

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

> Allow **construction offices to be privatised**. They can **purchase
> vehicles** and use **the same system we use for private logistics** to find
> buildings to construct: **private ones, or buildings that allow private
> construction**. They grab goods from the storage facilities they are
> allowed to access, using **the same checkbox as "allow private logistics to
> take"**, and they construct by transporting the goods. **What to do about
> workers will be decided later.** This is a tentative vision.

---

## 3. How the native construction office works

Summary from the community and official wikis and player reports, for
design only. Native layouts still need Ghidra confirmation (§7).

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

## 4. Proposed model

### 4.1 Privatising a construction office (Proposed)

- The PRIV control already used for other buildings also appears on a
  construction office. The office becomes a company in `enterprise_base`.
- Price and starting capital follow the existing decisions:
  - the fund pays the state per D24 when the office was state-built;
  - starting capital and legal reserve per D29 (45 days of wages and inputs;
    the office's inputs are fuel and vehicle upkeep);
  - nationalisation per D27/D30.
- Its vehicles stay in the office's own vehicle vector, as today.

### 4.2 Buying vehicles (Owner: "can purchase vehicles")

- **Proposed:** a private office's vehicle purchases are paid from **its own
  capital** at the game's native vehicle price, never by the treasury.
  Selling a vehicle credits the company. This follows D20 (no money from
  nothing) and D21 (dollar mode uses the dollar price, no exchange rate).
- **Open:** where the native buy-vehicle button takes its money (§7, C4).
  Until that is known, the mod must not let a private office buy vehicles
  out of the treasury.
- **Proposed:** the office's fuel tank is filled by normal deliveries, which
  `private_logistics` settles as a B2B purchase like any other input.

### 4.3 Finding sites: the private logistics system (Owner)

Reuse the `[AUTO DO]` pattern: a once-a-day reconcile that writes eligible
targets into the native list and leaves movement to the game.

- **Eligible sites (Owner):**
  1. sites owned privately (tagged for private construction, the existing
     CON tile);
  2. sites whose owner **allows private construction**.
- **Proposed:** for (2), a new per-building checkbox **"Allow private
  construction"** on state construction sites. It is **default OFF**, like
  the state-storage loading/delivery permissions, persisted per building and
  drawn through `ui_panels`.
- **Proposed:** a private office never takes a state site without that
  permission, and keeps the native range limit.
- **Open:** should **state** offices still be allowed to build private sites?
  Today they must finish the last percent (§1.4). Under this model they would
  no longer have to.

### 4.4 Materials: the same "take goods" checkbox (Owner)

- The office's **sources of materials** are written from the storages that
  the private logistics rules already allow:
  - **state** storages with **"Allow private DO loading (take goods)"**
    checked;
  - **private** storages under the same rule private DOs use today.
- Goods move only in native vehicles: no stock writes and no teleport.

### 4.5 Construction progress (Proposed)

- On sites built by a private office, progress comes **only** from the
  native mechanism (materials and workers delivered). The mod stops writing
  progress there.
- This removes the 99 % hand-off to a state office and the invisible-finish
  risk.
- `private_construction`'s money-to-progress path stays as it is (D13) until
  the private office path is proven in game. Then it is retired.

### 4.6 Money flows (Proposed, every line needs owner approval)

Built on the existing rules: the native price (D2/D19), the buyer pays the
seller on delivery (§4 of the economy direction), and no money from nothing
(D20).

| Flow | Payer → payee | Amount |
|---|---|---|
| Materials loaded at a source | site owner → storage owner (state treasury or private company) | tons × native price, locked at loading (the same lot rule as goods) |
| Transport | site owner → construction company | observed fuel × fuel price (the same rule as a DO trip) |
| Construction service | site owner → construction company | **Open:** for example the `"workers"` (workdays) part of the native construction cost (`NativeCostForBuilding`) × the wage rate, or a contract price |
| Wages, fuel and vehicles of the office | construction company → workers / sellers | the company's own costs |

- **Site owner:** the private company or the investment fund for a private
  site, and the state treasury for a state site built by a private office.
- **Proposed:** an unaffordable bill is never silently skipped. It goes into
  debt with a visible log, like the delivery-debt rule in `private_shops`.

### 4.7 Workers (Owner: decide later)

**Open.** Options to discuss, none chosen:

- **(a) Native as-is:** the office bus fetches workers from a designated stop,
  and the mod treats them like any other workplace's workers.
- **(b) Workers employed by the construction company:** payroll comes from
  its capital through `citizen_labor`.
- **(c) Workdays billed through the service charge in §4.6:** the site owner
  pays and the company pays its workers.

---

## 5. Relation to existing decisions and code

- **D13:** private construction stays as it is for now. This vision is the
  later step.
- **D12:** the fund "unlocks" high-demand building types for the player to
  place. A private construction office is what would build those sites.
- **D24:** no fund payment for a privately built building. This still holds.
  A building completed by a private office for a private owner is privately
  built.
- **`private_logistics` `[AUTO DO]`:** the same discovery, daily reconcile,
  permission table and per-building checkbox code, extended to construction
  office lists.
- **`private_construction`:** costs (`NativeCostForBuilding`), the site
  registry and persistence are reusable. The progress-write path is what this
  replaces.

---

## 6. Phases (Proposed)

0. **Fix the §1 bug.** Done (§1.3).
1. **Observe only:** log every construction office's sites, sources,
   vehicles and auto-search range, and each site's progress source. No
   writes.
2. **Privatise offices**, with company-paid vehicle purchases.
3. **Automatic sites and sources**, dry-run first, then applied. Same
   switches as `[AUTO DO]`.
4. **Money settlement** for materials, transport and service, dry-run first.
5. **Native-only progress** for private-office sites; retire the
   money-to-progress path once proven in game.
6. **Workers**, after the §4.7 decision.

---

## 7. Native research needed (proposed Ghidra targets)

Not added to the economy direction G queue until the owner schedules this
work.

| ID | Question | Needed for |
|---|---|---|
| C1 | Construction office building type number(s) and type-descriptor fields | Recognising offices; PRIV tile |
| C2 | Layout of the office's site assignment list, auto-search flag and range | §4.3 |
| C3 | Layout of the office's "sources of materials" list, including the workers source | §4.4, §4.7 |
| C4 | Native buy-vehicle and sell-vehicle path: who is debited and credited | §4.2 |
| C5 | Where a construction vehicle's unload at a site is committed. DO trucks at a site logged `commit=0` at the B2B site RVA `0x68F792`; the construction path is probably different | §4.6 settlement |
| C6 | Whether a site can be restricted to specific offices | §4.3 state-office question |
| C7 | How native construction progress (`building+0x604`) advances per delivery and per workday | §4.5, §4.6 service price |

---

## 8. Open owner questions

1. Is the **"Allow private construction"** checkbox (default OFF) the right
   way for state sites to opt in?
2. Should state offices keep building private sites?
3. How is the construction service priced (§4.6)?
4. Workers: (a), (b) or (c) in §4.7, or something else?
5. Does the site owner pay materials at loading (proposed) or on completion?
