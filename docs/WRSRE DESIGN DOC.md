# Master Design & Feature Implementation Specification

## Realistic Hybrid Economics Engine for _Workers & Resources: Soviet Republic_

> **Target Executable:** `SOVIET64.exe` v1.1.1.9 (64-bit DX11.1, base image `0x140000000`).  
> **Architecture:** TesmioLoader C++17 Modular Plugin Ecosystem.  
> **Design Philosophy:** _Victoria 3_ mixed-economy simulation (laws, markets, interest groups) built on top of _Workers & Resources_ physical logistics.  
> **Status:** Living Master Specification & Engineering Reference, **Revision 3** (Vector Grounding & Native Municipal Hierarchy Pass).

---

```
                              MASTER IMPLEMENTATION PIPELINE
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 1: Urban Land Value, Rent & Economic Eviction (enterprise_base v9.4)   │ [COMPLETE]
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Task 4: Interactive Fiscal Policy & Native UI Panels (taxes/ui_panels v6.5) │ [COMPLETE]
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
  │ Task 2: Local Town Markets & Physical Scarcity Pricing                      │ [NEXT PRIORITY]
  │         (resource_pricing v4.0, private_shops v3.2, 14-cities.md grounding) │
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Information Layer: Macro Economic Readouts & Native Warning Indicators     │ [PLANNED]
  │                    (ui_panels v6.6, state_ledger v1.5, accounting_office)   │
  └──────────────────────────────────────┬──────────────────────────────────────┘
                                         │
                                         ▼
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │ Physical Logistics Grounding for Private Construction (private_construction)│ [PLANNED]
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

- **The Sovereign State (The Player):** Retains direct control over heavy industry, infrastructure, energy, public services, and regulatory levers (minimum wages, price ceilings, subsidies, taxes, import tariffs).
- **The Autonomous Private Sector:** Manages consumer retail, residential housing, private services, and commercial investment. Private enterprises autonomously accumulate profits, bid on wages, pay dividends into the Private Investment Pool, and face bankruptcy when insolvent.
- **The Interaction:** The player does not micro-manage private shops or apartments; instead, the player guides, taxes, and competes with private capital through state policy.

### 1.2 Victoria 3 Alignment & Physical Logistics Grounding

_Victoria 3_ serves as our macroeconomic inspiration, but _Workers & Resources_ adds what grand strategy abstracts away: **physical goods, physical logistics, and individual citizens**.

| _Victoria 3_ Concept               | WRSRE Engine Equivalent                                                                       | Source File / Implementation              | Status            |
| :--------------------------------- | :-------------------------------------------------------------------------------------------- | :---------------------------------------- | :---------------- |
| **Pops & Strata**                  | Individual citizens (`TSM_PERSON_SIZE`), classified into 4 Strata; aggregated into Pop Groups | `citizen_retail.cpp`, `citizen_labor.cpp` | `[IN PROGRESS]`   |
| **Standard of Living (SoL)**       | 4-Tier Hierarchy evaluated via Purchasing Power Ratio (Flow) and Liquidity Runway (Stock)     | `citizen_retail.cpp`                      | `[IN PROGRESS]`   |
| **Goods Markets**                  | Local Town Markets grounded by physical stock cover via native `NamePoint` (`0x108`)          | `resource_pricing.cpp`, `14-cities.md`    | `[NEXT PRIORITY]` |
| **Market Access / Infrastructure** | Real road, rail, and warehouse network simulated via freight distance penalties               | `private_shops.cpp`, `private_logistics.cpp` (type-5 road `0xA10` type-3) | `[STAGED]` (6b) |
| **Corporate Balance Sheets**       | `[P]` enterprises with operating capital, wage bidding, and the 3-month distress ladder       | `enterprise_base.cpp`                     | `[COMPLETE]`      |
| **Investment Pool**                | Capitalist Investment Pool accumulated strictly from net operating profits                    | `enterprise_base.cpp`                     | `[COMPLETE]`      |
| **Construction Capacity**          | Turnkey private construction competing with state mega-projects for physical materials        | `private_construction.cpp`                | `[PLANNED]`       |
| **Fiscal Policy & Taxation**       | Direct UI sliders for VAT, Corporate Profit Tax, and Land-Value Property Tax                  | `taxes.cpp`, `ui_panels.cpp`              | `[COMPLETE]`      |
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

# SECTION 4: TASK 4 — FISCAL POLICY & NATIVE UI PANELS [COMPLETE]

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

# SECTION 7: TASK 2 — LOCAL TOWN MARKETS & SCARCITY PRICING [NEXT PRIORITY]

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

# SECTION 8: INFORMATION LAYER & DIAGNOSTIC LEGIBILITY [PLANNED]

### 8.1 Macroeconomic Readouts

Integrated into the City Hall (`type 39`) and Accounting Office dashboard via `taxes.cpp`:

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

# SECTION 9: PHYSICAL LOGISTICS FOR PRIVATE CONSTRUCTION [PLANNED]

Private construction must not conjure structures out of currency balances:

1. **Physical Material Crowding-Out:**
   - When private construction advances, calculate fractional requirements (steel, concrete, gravel, bricks) from the native phase descriptor (`typedesc + 0x370`).
   - Draw down physical inventory directly from Republic warehouses at market prices, paid from the Private Investment Pool into the State Treasury.
   - If Republic physical inventories are exhausted, private construction projects stall, establishing authentic competition between private capital and state mega-projects.

---

# SECTION 10: DEFERRED SYSTEMS & ENGINEERING AUDIT

### 10.1 Rescoped Systems

- **Task 6: Autonomous Private Logistics:** Original broad fleet engine **remains dropped**. **Task 6b state-storage source (private_logistics v0.17) is staged** — bounded opt-in, load-only (`TaskCfg+0x04=1.0/+0xC8=0/+0xCC=0`), road type-3, cap 5, `dry_run=1`. Direct DO vector manipulation via background threads is still unsafe for general fleet use; Task 6b uses validated native lifecycle (`TaskEntry 0x18/TaskCfg 0x198` via `0x08E050`, `+0xD50=1/+0xFC0=3.0`, save `1EDC40/6C5350` load `1F0BE0`). See `docs/ghidra-do-storage-findings.md`.
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
