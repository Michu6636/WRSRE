# Master Design & Feature Implementation Specification

## Realistic Hybrid Economics Engine for _Workers & Resources: Soviet Republic_

> **Target Executable:** `SOVIET64.exe` v1.1.1.9 (64-bit DX11.1, base image `0x140000000`).  
> **Architecture:** TesmioLoader C++17 Modular Plugin Ecosystem.  
> **Design Philosophy:** _Victoria 3_ mixed-economy simulation (laws, markets, interest groups) built on top of _Workers & Resources_ physical logistics.  
> **Status:** Living Master Specification & Engineering Reference, **Revision 3** (Vector Grounding & Native Municipal Hierarchy Pass).

> **Revision 5 — owner decisions of 2026-11-22 (read first).** The economic direction changed. [`visions/economy-direction-2026-11-22.md`](visions/economy-direction-2026-11-22.md) **overrides this document wherever they conflict**. In short:
> - The state/private balance is entirely the player's choice.
> - Only **native game prices** are used. The market book and multiplier are obsolete.
> - **Local town markets (Task 2) are cancelled**: there is one domestic market.
> - The state no longer buys private output on dispatch. Companies keep their own customs export revenue, and state exports stay native.
> - The City Hall TAX tile and fiscal panel are obsolete; the **Almanac** replaces them.
> - **No sample, synthetic or invented fallback data, ever.** Missing data must be obvious.
> - Private-logistics B2B billing is to be **live by default**.
> - Decrees and housing choice by wealth are agreed.
> - Politics comes later, as a separate module.
>
> **The code has not been changed yet.** Sections below describe the code as built, with Rev-5 notes where it is now obsolete.

---

```
                              MASTER IMPLEMENTATION PIPELINE
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 1: Urban Land Value, Rent & Economic Eviction (enterprise_base v9.4)   │ [COMPLETE]
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 4: Interactive Fiscal Policy & Native UI Panels (taxes/ui_panels v6.5) │ [COMPLETE → City Hall UI OBSOLETE, Almanac replaces it (Rev 5)]
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 3 & 8: Insolvency, Buyout & Ruins Cleanup (enterprise_base v9.4)       │ [COMPLETE]
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 7: Standard of Living: Flow + Stock Matrix & Poverty Grounding         │ [IN PROGRESS]
  │         (citizen_retail v2.3, citizen_labor v4.3, resource_pricing v3.0)     │
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 2: Local Town Markets & Physical Scarcity Pricing                      │ [CANCELLED 2026-11-22 — single domestic market]
  │         (resource_pricing v4.0, private_shops v3.2, 14-cities.md grounding) │
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Information Layer: Macro Economic Readouts & Native Warning Indicators     │ [PLANNED — in the Almanac]
  │                    (almanac_overlay_dev, state_ledger; NO sample data)      │
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Physical Logistics Grounding for Private Construction (private_construction)│ [DEFERRED until core loop polished]
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  │ Task 6b: Bounded Opt-In State-Storage Source (private_logistics v0.17) [STAGED] │
  │ [DEFERRED] Task 5: Sovereign Currency & Central Bank FX Reserves            │
  └─────────────────────────────────────────────────────────────────────────────┘
```

> **Update 2026-10-22 (Rev 4):** Task 6b state-storage is now staged (v0.17/v6.11) with native DO task lifecycle (`0xD38/0xD40` stride `0x18`, `0x198` TaskCfg, save `1EDC40/6C5350` load `1F0BE0`). See `docs/ghidra-do-storage-findings.md`, `docs/current-task-state-storage.md`, `docs/NEXT_PHASE.md`. Task 6 original autonomous fleet remains deferred; Task 6b is bounded source-only.

---

# SECTION 1: VISION, SYSTEM DYNAMICS & GAMEPLAY LOOPS

### 1.1 Philosophy & The Mixed-Economy Sandbox

Vanilla _Workers & Resources_ operates as a pure command economy where the player micro-manages every kilogram of gravel, every ruble, and every citizen assignment.

This mod transforms the game into a **Realistic Mixed Economy**:

- **Rev 5 (2026-11-22): player choice of system.** How much of the economy is state-run and how much is private is **entirely the player's choice**, from a purely planned public economy to a capitalist-leaning hybrid. Nothing forces privatisation; privatisation is a manual player action. See [economy direction D1](visions/economy-direction-2026-11-22.md).
- **The Sovereign State (The Player):** Controls whatever it keeps in state hands, and sets regulatory levers through Almanac controls and **decrees**: minimum wage, welfare/pension/stipend levels, wage indexation, state retail price level, taxes, and possible price ceilings or rent control (decree list: economy direction §7).
- **The Private Sector (when the player allows it):** Private enterprises accumulate profits, bid on wages (indexed to CPI automatically), contribute to the Private Investment Pool, and face bankruptcy when insolvent. Dividends to rich citizens and fund-driven unlocking of building types are planned (D12). Automatic building placement is not.
- **The Interaction:** The player does not micro-manage private shops or apartments; instead, the player guides, taxes, and competes with private capital through state policy.

### 1.2 Victoria 3 Alignment & Physical Logistics Grounding

_Victoria 3_ serves as our macroeconomic inspiration, but _Workers & Resources_ adds what grand strategy abstracts away: **physical goods, physical logistics, and individual citizens**.

| _Victoria 3_ Concept               | WRSRE Engine Equivalent                                                                       | Source File / Implementation              | Status            |
| :--------------------------------- | :-------------------------------------------------------------------------------------------- | :---------------------------------------- | :---------------- |
| **Pops & Strata**                  | Individual citizens (`TSM_PERSON_SIZE`), classified into 4 Strata; aggregated into Pop Groups | `citizen_retail.cpp`, `citizen_labor.cpp` | `[IN PROGRESS]`   |
| **Standard of Living (SoL)**       | 4-Tier Hierarchy evaluated via Purchasing Power Ratio (Flow) and Liquidity Runway (Stock)     | `citizen_retail.cpp`                      | `[IN PROGRESS]`   |
| **Goods Markets**                  | **Rev 5:** one domestic market at **native game prices**; market book/multiplier obsolete; local town markets cancelled | `resource_pricing.cpp`                    | `[REDESIGN]` (see economy direction §3) |
| **Market Access / Infrastructure** | Native physical logistics; private-DO access/target assistance and approximate road-assignment cost records (not live settlement) | `private_shops.cpp`, `private_logistics.cpp`; see section 12 | `[IMPLEMENTED / PARTIALLY VALIDATED]` (DO scope; payments OFF) |
| **Corporate Balance Sheets**       | `[P]` enterprises with operating capital, wage bidding, and the 3-month distress ladder       | `enterprise_base.cpp`                     | `[COMPLETE]`      |
| **Investment Pool**                | Capitalist Investment Pool accumulated from net operating profits; Rev 5: dividends to rich citizens and building-type unlocks planned | `enterprise_base.cpp`                     | `[COMPLETE]` / extensions `[DEFERRED]` |
| **Construction Capacity**          | Turnkey private construction competing with state mega-projects for physical materials        | `private_construction.cpp`                | `[DEFERRED]` (Rev 5: current simple system stays for now) |
| **Fiscal Policy & Taxation**       | VAT, Corporate Profit Tax, Land-Value Property Tax; Rev 5: controls live in the **Almanac** only, City Hall UI to be removed; decrees planned | `taxes.cpp`, `almanac_overlay_dev`        | `[COMPLETE]` / City Hall UI `[OBSOLETE]` |
| **Sovereign Debt**                 | State borrowing against the Private Investment Pool with credit rating spreads                | `enterprise_base.cpp`, `state_ledger.cpp` | `[PLANNED]`       |

---

# SECTION 2: REVERSE-ENGINEERED ENGINE GROUND TRUTH

All plugins operate directly on the memory image of `SOVIET64.exe` v1.1.1.9 (ASLR dynamic base `0x140000000`). The following structures represent audited, verified ground truth:

### 2.1 Uniform Building Occupancy Vectors

The engine does **not** alter vector layouts across building types. Factories, shops, pubs, and residences share identical occupancy offsets:

```
  Building (base)
    ├── +0x108 ──► NamePoint* (Native City / Town)
    ├── +0x318 ──► TypeDescriptor*
    ├── +0xBC0 ──► vector<Person*> Workers / Residents BEGIN (TSM_BLD_WORKERS_BEGIN)
    ├── +0xBC8 ──► vector<Person*> Workers / Residents END   (TSM_BLD_WORKERS_END)
    ├── +0xBD8 ──► vector<Person*> Visitors / Customers BEGIN (TSM_BLD_VISITORS_BEGIN)
    ├── +0xBE0 ──► vector<Person*> Visitors / Customers END   (TSM_BLD_VISITORS_END)
    ├── +0xDDC ──► float Customer Service Throughput (TSM_BLD_CUSTOMER_CAP)
    └── +0xDEC ──► int   Blueprint Maximum Worker Cap (TSM_BLD_MAX_WORKERS)
```

1. **Stationary Occupants (`+0xBC0` / `+0xBC8`):**
   - **Workplaces (Factories, Shops, Pubs, Clinics):** Holds active employees / workers on shift. Audited in factory tick `FUN_1401cf590` and panel `FUN_140799630`.
   - **Residential Apartments:** Holds registered citizen households (tenants).
   - **Mod Consumers:** `citizen_labor.cpp` (workplace pass) and `enterprise_base.cpp` (`StaffingRatioImpl`).
2. **Transient Occupants (`+0xBD8` / `+0xBE0`):**
   - Holds visiting customers, shoppers, pub patrons, and clinic patients.
   - The native UI panel (`FUN_1407272f0`) calculates customers present as:
     $$\text{Customers Present} = \text{count}(0xBD8) - \text{workforce\_cap}(0xDDC)$$
   - **Mod Consumers:** `citizen_retail.cpp` (customer sales pass) and `private_shops.cpp` (`RunServiceCharge`).
3. **Staffing Denominator Fix (`+0xDEC` vs `+0xDDC`):**
   - `+0xDDC` is a dynamic `float` computed every tick representing how many customers currently present staff can serve based on productivity.
   - `+0xDEC` is the true blueprint maximum workforce capacity (`int`), loaded directly from `$WORKERS_NEEDED` in building `.ini` files.

### 2.2 Native Municipal Structure (`NamePoint` & `14-cities.md`)

Towns are not abstract spatial clusters; they are first-class engine entities:

- Every building holds a direct pointer to its assigned municipality at **`bld + 0x108`** (`NamePoint*`).
- **`city + 0x04`:** City Center Coordinates (`float X, Y, Z`).
- **`city + 0x10`:** City Name (`wchar_t[128]`).
- **`city + 0x1A0 .. 0x1B0`:** `std::vector<Building*>` containing all municipal structures.
- **City Hall (`type 39` / `0x27`):** Does not define city boundaries; it is an administrative structure assigned to a `NamePoint` that writes statistics to `*(city + 0x1B8) + 0xE0`.

---

# SECTION 3: TASK 1 — URBAN LAND VALUE, RENT & ECONOMIC RELOCATION [COMPLETE]

### 3.1 Rent Formation Formula

Private residential apartments (`[P]`) collect dynamic monthly rent based on environmental attractiveness (`bld + 0x11B0`), flat capacity, and the player's native rent slider (`bld + 0x13E0`):

$$\text{Tenant Rent} = \text{Base Rent} \times \Big(\text{MinMult} + (\text{MaxMult} - \text{MinMult}) \times \text{Attractiveness} \times \text{ParkCap}\Big) \times \text{RentSliderFactor}$$

- **Attractiveness Heightfield:** Sampled daily by `FUN_1404D5F50` from the pollution/environmental grid ($0.0\text{--}1.0$).
- **Dwelling Quality & Park Cap:** Prevents rural shacks from planting trees to charge luxury rents:
  $$\text{ParkCap} = \text{clamp}\left(\frac{\text{flatCap}}{30.0f},\ 0.20,\ 1.00\right)$$
- **Rent Slider Factor:** Grounded in the player's native building slider (`bld + 0x13E0`, $0.0\text{--}1.0$):
  $$\text{RentSliderFactor} = 0.75f + 0.50f \times \text{sliderLevel}$$

### 3.2 Household Relocation & Eviction Engine

1. **Eviction Staging:**
   - Evaluated monthly during `CollectResidentialRent()` in `enterprise_base.cpp`.
   - **Economic Burden Trigger:** Flagged if $\text{Tenant Rent} > 0.35 \times \text{Tenant Wallet}$ or if wallet covers $< 95\%$ of due rent.
   - **Destitution Trigger:** Flagged if tenant wallet cannot cover 1 day of staple bread, evaluated prior to rent collection.
2. **Relocation Search (`FindRelocationCandidate`):**
   - Scans Republic residential structures (`game + 0x11B08..0x11B10`, `type == 2`, finished, undecayed, `residents < flatCap`).
   - **Demographic Sorting:** Impoverished households ($\text{wealth} < 0.40$) route to zero-rent state housing or low-attractiveness tenements. Affluent households ($\text{wealth} \ge 0.70$) bid for top-tier private housing.
3. **Atomic Native Relocation (`FUN_14083CA50`):**
   - Executed strictly outside `g_lock` to prevent simulation stalls:
     $$\text{FUN\_14083CA50}(game, person, candidateBuilding)$$
   - Relocates the entire family unit, unlinks previous route vectors, and updates occupancy counters.
   - Applies an eviction happiness penalty (`person + 0xE8 -= 0.15f`).
4. **Homelessness Backstop (Anti-Despawn Guard):**
   - If no vacant housing exists in the Republic, **relocation is aborted**.
   - Calling `FUN_14083CA50` with `nullptr` causes native `FUN_140835D70` to permanently delete/free the citizen. Aborted tenants remain in place under rent arrears with a continuous $-0.05/\text{month}$ happiness penalty.

---

# SECTION 4: TASK 4 — FISCAL POLICY & NATIVE UI PANELS [COMPLETE — CITY HALL UI OBSOLETE]

> **Rev 5 (2026-11-22):** The City Hall TAX tile, the fiscal panel, the City Hall macro rows and `republic_bank`'s City Hall rate slider are **obsolete**. The Almanac (`plugins/almanac_overlay_dev`) is the single policy surface. Any readout that exists only on City Hall moves to the Almanac before removal. The description below is kept as a record of the built code. See [economy direction D7](visions/economy-direction-2026-11-22.md).

### 4.1 Native Hooking Architecture (`ui_panels.cpp` v6.5)

Fiscal controls are injected directly into native building windows (City Hall `type 39`, Accounting Office, and production structures) via verified prologue detours:

- **The Bevelled "TAX" Button Tile:**
  - Rendered via `C3D_PANEL2D::Draw` as a native 70-unit bevelled tile matching the tab button strip (Slot 4, alongside Overview, Storage, Exchange, and Chart).
  - Uses centered-draw compensation ($X + \text{size}/2, Y + \text{size}/2$), 5-slot fallback texture binding, and high-contrast dark charcoal text (`0xFF202020`).
- **Interaction Engine:**
  - Employs direct client-space AABB hover detection combined with engine latch `0xA54E91` and a physical `GetAsyncKeyState(VK_LBUTTON)` edge detector to prevent tab starving.
- **Collapsible Layout & Budget:**
  - Clicking `TAX` toggles `s_showTaxPanel`.
  - **Collapsed:** Consumes zero additional vertical layout height.
  - **Expanded:** Injects 3 native rate sliders and 3 live status badges at `window + 0x250`:
    - VAT Rate Slider ($0\%\text{--}50\%$) + `[X] VAT: Active` Badge.
    - Corporate Profit Tax Slider ($0\%\text{--}50\%$) + `[X] Profit: Active` Badge.
    - Property Tax Slider ($0\text{--}500\text{ RUB}$) + `[X] Property: Active` Badge.

---

# SECTION 5: TASK 3 & 8 — UNIFIED CREDIT MARKETS, INSOLVENCY & RUINS [COMPLETE]

### 5.1 The 3-Month Distress Ladder

Private enterprises tagged `[P]` tracking $\le 0.0$ operating capital advance through a deterministic distress lifecycle in `enterprise_base.cpp`:

```
  [ Capital <= 0.0 ] ──► Month 1: [DISTRESSED] Hiring Freeze (Workplace Poisoned)
                                      │
                                      ▼
                         Month 2: [AUCTION] Pool Buyout Offer (1.5x Capital)
                                      │
                                      ▼
                         Month 3: [PRE-RUIN EVICTION] Evacuate Tenants via 0x83CA50
                                      │
                                      ▼
                         Month 3: [ABANDONED] 3D Physical Ruins via 0x13FE70
```

1. **Month 1 (Distressed):** Hiring freeze enacted. The workplace defaults on payroll, poisoning the building in `citizen_labor` so no new staff are assigned.
2. **Month 2 (Insolvent Auction):** The Private Investment Pool evaluates an acquisition at $\text{Starting Capital} \times 1.5$. If pool balance permits, funds are transferred and working capital is restored to baseline.
3. **Month 3 (Pre-Ruin Eviction Guard):**
   - Before executing physical collapse, all residential tenants are evacuated to candidate housing via `FUN_14083CA50` outside `g_lock`.
   - Prevents citizen pointer corruption when the native mesh collapses.
4. **Month 3 (Physical Abandonment):** Native function `FUN_14013FE70` (RVA `0x13FE70`) collapses the structure into physical 3D ruins outside `g_lock`. Enterprise flags are permanently latched to `isAbandoned = true`, blocking auto-reprivatization.

---

# SECTION 6: TASK 7 — STANDARD OF LIVING & WEALTH STRATA [IN PROGRESS]

### 6.1 Absolute Survival Basket (CPI)

Living standards are anchored to an absolute physical basket calculated in the active currency via `resource_pricing.cpp` v3.0:

$$\text{Daily Basket Cost (CPI)} = (P_{\text{food}} \cdot \text{mult}_{\text{food}} \cdot Q_{\text{food}}) + (P_{\text{meat}} \cdot \text{mult}_{\text{meat}} \cdot Q_{\text{meat}}) + (P_{\text{clothes}} \cdot \text{mult}_{\text{clothes}} \cdot Q_{\text{clothes}}) + \text{Daily Rent}$$

> **Rev 5:** The `mult` terms (market multipliers) are obsolete. The basket uses plain native prices at the actual shelf price level (economy direction §3). The basket also becomes the CPI reference for wage indexation (§6.3 there).

- **Calibrated Physical Intakes:**
  - $Q_{\text{food}} = 0.0020\text{ t}$ (Midpoint of native consumption ladder).
  - $Q_{\text{meat}} = 0.0006\text{ t}$ (Engine baseline demand).
  - $Q_{\text{clothes}} = 0.0005\text{ t}$ (Clothing wear rate).
  - $\text{Daily Rent} = \text{Monthly Base Rent} / \text{DaysPerMonth()}$ (Injected from `enterprise_base.cpp`).

### 6.2 The Top-Down Flow + Stock Economic Class Matrix

To eliminate classification gaps and volatility from cash drawdowns, citizens are classified top-down (first match wins) in `citizen_retail.cpp` v2.3:

$$\text{Purchasing Power Ratio (PPR)} = \frac{\text{Daily Wage / Social Transfer}}{\text{Daily Basket Cost (CPI)}}$$

$$\text{Liquidity Runway (Days)} = \frac{\text{Citizen Wallet Balance}}{\text{Daily Basket Cost (CPI)}}$$

```
                                  THE ECONOMIC CLASS MATRIX

   SoL TIER               QUALIFYING CRITERIA                     INDUCED CONSUMPTION BEHAVIOR
┌──────────────┐     ┌─────────────────────────────┐     ┌─────────────────────────────────────────┐
│ 0. Destitute │ ──► │ Wallet < 1 Day Bread (CPI)  │ ──► │ Meat -> 0, Clothes -> 0, Elec -> 0.     │
│ (Crisis)     │     │ AND Daily Income <= 0.01    │     │ Eviction candidate; rapid health decay. │
└──────────────┘     └─────────────────────────────┘     └─────────────────────────────────────────┘
┌──────────────┐     ┌─────────────────────────────┐     ┌─────────────────────────────────────────┐
│ 3. Affluent  │ ──► │ PPR >= 2.0x                 │ ──► │ Meat x1.5, Clothes x1.5, Elec x2.0.     │
│ (Prosperous) │     │ AND Runway >= 30 Days       │     │ Bids for high-attractiveness housing.   │
└──────────────┘     └─────────────────────────────┘     └─────────────────────────────────────────┘
┌──────────────┐     ┌─────────────────────────────┐     ┌─────────────────────────────────────────┐
│ 2. Middle    │ ──► │ PPR >= 1.0x                 │ ──► │ Standard baseline consumption (x1.0).   │
│ (Stable)     │     │ OR Runway >= 14 Days        │     │ Early return without demand write.      │
└──────────────┘     └─────────────────────────────┘     └─────────────────────────────────────────┘
┌──────────────┐     ┌─────────────────────────────┐     ┌─────────────────────────────────────────┐
│ 1. Low       │ ──► │ All Other Citizens          │ ──► │ Meat x0.2, Clothes x0.5, Elec -> 0.     │
│ (Struggling) │     │ (PPR < 1.0x & Runway < 14d) │     │ Alcohol Escapism x1.8 (kind == 3 only). │
└──────────────┘     └─────────────────────────────┘     └─────────────────────────────────────────┘
```

- **Poverty Escapism Fix:** The $1.8\times$ demand multiplier applies strictly to `kind == 3` (Alcohol) when happiness $< 0.40$. It is strictly barred from scaling `kind == 1` (Food).

---

# SECTION 7: TASK 2 — LOCAL TOWN MARKETS & SCARCITY PRICING [CANCELLED 2026-11-22]

> **Rev 5:** Cancelled by the owner. The whole map is **one domestic market** until the core loop is perfected. Prices are the **native game prices** with no scarcity, freight-surcharge or market multiplier. A future market model may adjust the native prices themselves (economy direction D2/D3). The text below is kept only as a historical design record. The reverse-engineering in [`engine/local-markets.md`](engine/local-markets.md) stays valid as engine reference.

### 7.1 Architecture: Grounded Local Catchment

Pricing will transition from static national numbers to localized town markets. By reading `bld + 0x108` (`TSM_BLD_CITY`), every shop, warehouse, and factory is instantly linked to its native municipality (`NamePoint*`):

$$\text{DaysOfCover}_{\text{local}} = \frac{\text{Municipal Physical Stock in Storages (Tons)}}{\max(\text{Daily Municipal Consumption Rate}, 0.05)}$$

$$\text{Scarcity Mult} = \text{clamp}\Big(0.70 \cdot \text{CoverMult}(\text{local}) + 0.30 \cdot \text{CoverMult}(\text{national}),\ 0.70,\ 1.50\Big)$$

$$\text{Shelf Price} = \text{Base Price} \times \text{Scarcity Mult} \times (1 + \text{FreightSurcharge}) \times (1 + \text{VAT}) - \text{Subsidy}$$

- **Cover Curve:**
  - $\text{DaysOfCover} \ge 25.0\text{ days} \implies \text{CoverMult} = 0.70$ (Surplus discount).
  - $\text{DaysOfCover} \approx 10.0\text{ days} \implies \text{CoverMult} = 1.00$ (Equilibrium).
  - $\text{DaysOfCover} \le 3.0\text{ days} \implies \text{CoverMult} = 1.50$ (Critical shortage).
- **Physical Logistics Grounding:** A town connected to domestic distribution or rail hubs receives freight parity. Isolated towns with starving warehouses face local scarcity spikes, driving private investment signals.

---

# SECTION 8: INFORMATION LAYER & DIAGNOSTIC LEGIBILITY [PLANNED — ALMANAC]

> **Rev 5:** These readouts belong in the **Almanac**, not City Hall. **No sample, synthetic or invented value may ever be displayed.** A missing service or field shows `UNAVAILABLE` / `ERROR <reason>` in warning colour (economy direction D8). The example figures below illustrate the format only and must never appear in the mod.

### 8.1 Macroeconomic Readouts

Originally specified for the City Hall (`type 39`) and Accounting Office dashboard via `taxes.cpp` (superseded by the Almanac):

1. **Cost of Living Index (CPI):** Live daily survival basket cost in active currency (`Cost of Living: 4.80 RUB/day`).
2. **Poverty Rate:** Percentage of population currently in Low or Destitute SoL (`Poverty Rate: 8.4%`).
3. **Housing Stress:** Count of economic evictions executed during the current month (`Evictions: 3 this month`).
4. **Commercial Solvency:** Health of the private sector (`Solvent Firms: 85% | Pool: 42,500 RUB`).

### 8.2 Native Building Warning Text

Surfacing commercial distress directly onto native building window banners:

- **`-Empty Shelves`**: Injected on private shops when retail inventory reads $0.0$.
- **`-Rent Arrears`**: Injected on residential buildings where tenants fail monthly affordability checks.
- **`-Hiring Frozen`**: Injected on private enterprises in Month 1 of financial distress.

---

# SECTION 9: PHYSICAL LOGISTICS FOR PRIVATE CONSTRUCTION [DEFERRED]

> **Rev 5:** Still the goal, but deferred. The current simple private-construction system stays until the central economic loop is improved and polished (economy direction D13).

Private construction must not conjure structures out of currency balances:

1. **Physical Material Crowding-Out:**
   - When private construction advances, calculate fractional requirements (steel, concrete, gravel, bricks) from the native phase descriptor (`typedesc + 0x370`).
   - Draw down physical inventory directly from Republic warehouses at market prices, paid from the Private Investment Pool into the State Treasury.
   - If Republic physical inventories are exhausted, private construction projects stall, establishing authentic competition between private capital and state mega-projects.

---

# SECTION 10: DEFERRED SYSTEMS & ENGINEERING AUDIT

### 10.1 Rescoped Systems

- **Task 6: Autonomous Private Logistics:** The original replacement fleet engine remains dropped; native logistics still owns physical work. Its bounded target-assistance successor now supports independent state-site directions, role-aware private connections and the working 10/30 stock policy. `private_logistics` **0.18.17-dry-ledger** additionally observes private road assignments and persists approximate, closure-frozen dry costs; **payments remain OFF**. The former v0.17 load-only/state-storage and target-dry-run snapshot is superseded, not a current configuration instruction. See **section 12** and [the current DO implementation guide](private-logistics-runtime/DO_OFFICES.md); historical layout evidence remains in [the storage findings](ghidra-do-storage-findings.md).
- **Task 5: Sovereign Currency & Central Bank:** Deferred indefinitely. Simulating a virtual third currency desynchronizes `state_ledger`. The unified Dual-Currency Regime (`RUB` / `USD` via `currency_api` v4.0) remains the active standard.

### 10.2 Recent Bug Fixes (Revision 3 Audit)

| File                  | Bug Addressed                         | Resolution                                                                                                |
| :-------------------- | :------------------------------------ | :-------------------------------------------------------------------------------------------------------- |
| `enterprise_base.cpp` | `DrainSampleWallets` lock corruption  | Removed premature inner `LeaveCriticalSection`, preventing `RTL_CRITICAL_SECTION` count underflow.        |
| `citizen_retail.cpp`  | Bread categorized as alcohol escapism | Excluded `kind == 1` (Food) from escapism branch; restricted strictly to `kind == 3` and `10`.            |
| `enterprise_base.cpp` | Missing pre-ruin eviction guard       | Injected `FindRelocationCandidate` evacuation loop prior to `CallNativeAbandon` on residential buildings. |
| `citizen_labor.cpp`   | False hiring of customers             | Switched Pass 1 worker detection from `0xBD8` to `0xBC0` (`TSM_BLD_WORKERS_BEGIN`).                       |
| `enterprise_base.cpp` | False staffing capacity numbers       | Replaced dynamic customer capacity `0xDDC` (`float`) with true worker limit `0xDEC` (`int`).              |

---

# SECTION 11: CRITICAL TECHNICAL RULES FOR DEVELOPMENT

1. **Architecture & ASLR:** Target binary is `SOVIET64.exe` v1.1.1.9. ASLR is ON. Never hardcode absolute virtual addresses. All engine patches and hooks must resolve via `g_exeBase + RVA`.
2. **Hooking Safety:** Inline hooks require $\ge 14$ stolen bytes. Never place inline hooks over relative call sites (`call rel32`).
3. **Critical Section Discipline:**
   - Never invoke native engine relocation (`FUN_14083CA50`) or physical abandonment (`FUN_14013FE70`) while holding `g_lock`.
   - Never iterate world structures while holding cross-plugin locks.
4. **File Encoding:** All `.ini` configuration files must strictly maintain **UTF-8 WITHOUT BOM** encoding.
5. **Execution Environment:** Commands must be executed via PowerShell 5.1 using single quotes and semicolon separators (never `&&`). Compile exclusively via `cmd /c build.bat <plugin>`.

---

# SECTION 12: PRIVATE DISTRIBUTION OFFICES — IMPLEMENTED DRY ACCOUNTING, NOT LIVE SETTLEMENT

> **Rev 5 (2026-11-22):** The owner decided that B2B billing must be **live by default** (`billing = 1`, `dry_run = 0`). The source is now `private_logistics` `0.19.0-simplified-settlement` (buyer pays verified seller; buyer pays private DO trip fuel). Before the defaults flip:
> 1. Remove the `private_shops` dispatch procurement and 0.70 delivery billing, to avoid double settlement.
> 2. Remove the market multiplier from the B2B price.
> 3. Exclude customs houses from `b2b_state_buy`.
>
> See economy direction §4.4. The "payments remain OFF" statements below describe the 0.18.x evidence period and are historical once that work lands.

**Source update 2026-11-02:** **0.18.18-census-guard** is a bounded legacy-census safety follow-up to **0.18.17-dry-ledger** from [PR #42](https://github.com/Michu6636/WRSRE/pull/42). It guards exhausted diagnostic slots and reports omitted visits, without changing caps, dispatch or billing. The follow-up passes 144 host tests and is now merged in PR #43 (`2d83fdc`). The user reports a Windows build; Test15 (2026-11-03) logs native 0.18.18 execution through 30 daily summaries, not an exhaustion test. That run exposes the existing five-total-target connection cap and unresolved secondary-goods dispatch behaviour; it is not full dispatch acceptance or a large-save benchmark. No further runtime changes are made by the Test15 investigation. This section supersedes earlier v0.17 source-only/dry-run snapshots for DO behavior; it does not change unrelated system status labels in this document. The detailed, plain-language implementation contract is [How distribution offices work in WRSRE](private-logistics-runtime/DO_OFFICES.md). That guide includes settings, examples, source links, limits and validation gates. Complete evidence stays in the [single cumulative report](private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md).

### 12.1 Physical logistics and access

The game owns truck movement, loading/unloading, road/resource compatibility and dispatch. The mod does not teleport stock or replace the native planner. Private DOs maintain eligible targets at daily reconciliation; state-owned DOs are not taken over. Manual removal of an eligible target is not a lasting veto: the company can reconnect it. State-site access, rather than per-company task micromanagement, is the player's control. Existing entries are retained, but company directions/resource selectors and thresholds are enforced on managed tasks, including manual ones. Manually configured customs tasks are preserved; automatic customs selection is not implemented. Automatic additions respect a conservative five-total-target budget, not a claimed native maximum.

LOAD means taking goods **from** a site; UNLOAD means delivering **into** it. Recognized state storage/factories have independent, default-OFF permissions; retail/service consumers allow deliveries only. Private endpoints follow their classified role without public-access widgets; unknown roles are denied rather than guessed. Native resource/input/output rules still apply. Current company policy is source stock above **10%**, destination below **30%**; these are dispatch stock triggers, not spending limits or final-stock guarantees. A checked permission authorizes access, not immediate dispatch. Ownership and permissions are rechecked during reconciliation.

### 12.2 Observed assignments and approximate costs

The panel-independent observer tracks private **road** vehicles; automatic target assistance includes road/rail offices, but this does not establish rail fuel accounting. It samples task identity and signed native quantity changes, not verified job-completion events. Observed removal/cancellation/replacement closes the original candidate regardless of success; scalar/index-only changes do not alone create a new assignment. Lost/unsupported continuity remains interrupted, not successful completion. Original-target responsibility is retained on cancellation and replacement; outside-assignment activity remains unallocated overhead.

For continuous valid pairs, accumulate positive quantity decreases and track increases separately: `0.80 → 0.70 → 0.95 → 0.85` contributes `0.20` decrease, not a negative/net fuel bill. Refills cannot erase earlier decreases. Sampling can miss activity or include non-burn changes; units and exact consumption remain provisional, with gaps/unknown starts/rises marked partial. At observed closure freeze **sampled decrease × available cached fuel price per tonne**. Later price changes do not rewrite closed amounts. Missing price is unavailable, not zero. This is a fuel-only dry estimate, not the legacy per-transfer tariff, producer payout, margin, depreciation or wage calculation.

### 12.3 Persistent evidence is not financial identity

Save the original target's asset/type/position snapshot, not raw pointers. Unavailable identities remain unresolved; a rebuilt identical site is not distinguished by this tuple. Closed records restore their frozen quotes. Saving an active assignment preserves its totals/target as an interrupted, unpriced copy; the live assignment continues, but loading never guesses a continuation onto current vehicles. Cost records live in `private_logistics_costs.bin`, separate from state permissions in `private_logistics_state.bin`. Existing corrupt/unreadable cost files block overwriting; validated loads reject stale-world observations. The ledger is limited to 4,096 append-only records, never silently evicted; a full book warns/refuses new records.

**Payments remain OFF.** A snapshot-linked “would owe” is not a verified company/wallet, an actual debt or a transaction. There is no new deduction, carrier payment, unpaid-debt settlement or invoice UI. Original-customer/carrier financial identities, duplicate-proof settlement, insufficient funds and cross-save debt persistence still need explicit implementation/testing. Do not enable the legacy billing path as a shortcut. Checked-in target assistance applies writes (`auto_targets_dry_run=0`) independently of payments being OFF (`billing=0`); these are different switches.

### 12.4 Timing, performance and the weekly-price idea

Simulation callbacks run a cheap elapsed-time gate; the full observer pass runs at most approximately once per **real-world second**, not every tick and not on an independent timer thread. This is observation, not DO dispatch. Faster game speed means more simulated activity between samples; ordinary pause duration alone is not a continuity break. Discovery examines up to 32 enterprise-register entries and tracks up to 64 road vehicles globally, with bounded readers and throttled logs. Fixed-size observation structures avoid routine tick heap allocation, but membership reads, occasional world identity scans, daily reconciliation, logs and save locking still cost time. **Large-save native performance is unmeasured; bounds do not prove negligible overhead.**

Current quote caching uses approximately **30 real-world seconds**, refreshed during eligible observer passes; ledger files are written on save events, not on that interval. The user proposes **future** slower price changes, perhaps once per game week. If the pricing authority later provides that schedule, cache invalidation could follow a defined week boundary plus initialization/load and supported exceptional price events. This requires a separate design for the calendar/event and ordering. It is **not implemented** here and would not change vehicle sampling to weekly or reprice closed records. Price-update cadence, quote-cache refresh and observation cadence are distinct concerns.

### 12.5 Validation status and next work

Earlier working-office tests support dispatch/permissions and that office's save/reload; Test11/Test12 support the accepted initial fuel approximation, refill-like rises and pause continuity. Test12 named saving worked; expected exit-without-save errors do not invalidate it. The new ledger has host-test support: 141 Python tests and 29 fleet + 12 dry-ledger scenarios in optimized/sanitized runs, archived in the cumulative report. These are not native lifecycle, verified payer, exact-burn or large-save timing proof. The preserved Windows build evidence is for 0.18.15, not a new 0.18.17 build.

**Test13 native update (2026-11-02):** the user ran 0.18.17-dry-ledger with payments OFF. Reported factory demolition closed once with the original factory snapshot and **1.8025194 RUB estimated cost**; subsequent return fuel was not allocated to the closed assignment. Two earlier autosaves logged successful 3-record/616-byte writes. Both preceded demolition, so persistence of that cancelled record is not established. Initial load was an empty book, not closed-record restoration. Full original evidence and checks are preserved in the cumulative report; this supports native dry-runtime paths, not a matching compiler transcript/source-binary parity or exact burn.

**Test14 update:** a native load from `autosave5` accepted **3 existing records**, zero interrupted/missing/blocked. Two autosaves rewrote 3 records / 616 bytes, and the named **`26741 - My republic`** save wrote **4 records / 812 bytes**. One assignment opened before that save; none closed. The fourth row is consistent with an interrupted/unpriced saved copy of the still-active assignment, not a new completed cost. Its exact stage/fields and a reload of the four-row book are not logged. This is genuine nonempty-load evidence, but does not identify Test13's later demolition record in the pre-demolition autosave or compare restored frozen quotes row-by-row.

Next: retain the successful cancellation, named-save and nonempty-load evidence; do not require repeated simple saves, demolition, blanket rebuild or baseline/pause research. Further row-level/interrupted-save loading checks can wait by agreement. Benchmark representative large saves, address long-term ledger retention/coverage, and only then develop verified financial linkage and idempotent debt/settlement under separate approval. No speculative native hook, payment activation or weekly-pricing implementation is authorized by this evidence update.
