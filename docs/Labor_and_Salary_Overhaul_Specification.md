# Design Document: Tropico-Style Salary Engine & Labor Market Overhaul

**Target Binary:** `SOVIET64.exe` v1.1.1.9 (`0x140000000` base)  
**System Module:** `citizen_labor` (Consumes: `sim_clock`, `currency`, `persistence`; Publishing to: `enterprise_base`, `private_construction`)  
**Architecture Classification:** Level 1 Core Simulation Service  
**Currencies Supported:** Unified Dual-Currency (`RUB` / `USD` via `currency_api` v4)

---

> **Amendment 2026-11-22 (owner decisions; overrides this spec where it conflicts).** See [economy direction §6 and §7](visions/economy-direction-2026-11-22.md).
>
> 1. **No general education or skill pay.** A university-educated worker in a basic slot, for example at a mine, earns the basic wage. A premium applies only to **high-education job slots** (`$PROFESORS_NEEDED`) in buildings that also have basic slots (`$WORKERS_NEEDED`), such as doctors vs. cleaning staff in a hospital. Whether a building has two tiers is decided by its native blueprint.
>    - State buildings: the player sets the premium per sector, with a per-building override (same hierarchy as §3.1).
>    - Private firms: they bid for high-education slots separately.
>    - Until the slot occupancy is reverse-engineered (Ghidra target G4), the premium is inactive and shown as `UNAVAILABLE`.
> 2. **Welfare, pension and stipend levels are player decrees** (low / medium / high or a slider), and the player bears the consequences. §2.1's "derive transfers from the minimum wage at 40/60/80 %" becomes the mechanism behind those levels. It is not a hidden default. Today's ratio defaults (1.00 / 1.50 / 2.00 RUB/day) sit below the CPI basket.
> 3. **Indexation:**
>    - State wages and transfers follow the CPI only while the player's *wage indexation* decree is on.
>    - Private wages follow the CPI **automatically**: private reference wage = sector baseline × CPI index, then the §2.3 bidding. They must no longer freeze with an unindexed state table.
> 4. **Wages are balanced against native prices.** A default wage must lie between the household floor (CPI basket + rent) and the firm ceiling (native value added per worker-day). The candidate anchor is the native price of the `"workers"` workday pseudo-resource (hypothesis H1, target G5). See economy direction §6.1 and §9.
> 5. **No invented numbers.** The `usd_divisor` conversion and any fallback wage shown to the player must not appear as if real. Missing values show `UNAVAILABLE`.
> 6. §2.2's "Tropico-style" state wage control stays. The fixed RUB figures in the Milestone examples (§7) are illustrations, not targets.

## 1. Executive Summary & Design Vision

The objective of this overhaul is to transform the rigid, static wage table of `citizen_labor` into an interactive, reactive **Labor Market Simulation** combining:

1. **Tropico-Style State Wage Governance:** The player can inspect any state-operated building, enter a custom wage using the game's native UI input widgets, and choose whether that salary applies to **only this specific workplace** or to the **entire state industry sector across the Republic**.
2. **Victoria 3-Style Private Wage Bidding:** Private enterprises dynamically float their wages based on profitability and worker shortages. Flush, profitable firms raise wages to attract workers away from the state sector; struggling firms implement austerity pay cuts to stave off bankruptcy.
3. **Statutory Minimum Wage Floor:** The State enacts a Republic-wide legal minimum wage decree that legally floors all wages, welfare, and pensions across both public and private sectors.

---

## 2. Core Economic Pillars & Game Mechanics

```
                             WAGE RESOLUTION HIERARCHY

                             [ National Minimum Wage ]
                             (State Law / Hard Floor)
                                        │
                                        ▼
    ┌───────────────────────────────────┴───────────────────────────────────┐
    ▼                                                                       ▼
[ State Workplaces ]                                            [ Private Workplaces ]
Level 1: Global Default                                         Base: Sector/State Baseline
Level 2: Industry Sector Tier                                   + Dynamic Profitability Bonus (+10% to +50%)
Level 3: Specific Building Override                             - Austerity Downscaling (-10% to -30%)
(Controlled via Player UI)                                      (Controlled by Market Health Engine)
    │                                                                       │
    └───────────────────────────────────┬───────────────────────────────────┘
                                        ▼
                           [ Final Daily Worker Pay ]
                                        │
                                        ├────────► Citizen Wallet (Purchasing Power)
                                        ├────────► Retail Checkout & Rent Viability
                                        └────────► Private Construction Turnkey Cost
```

### 2.1 The Statutory Minimum Wage

- **The Legal Floor:** A legally enforced baseline daily wage (`minimum_wage`). No worker—whether employed by a state mine, a private shop, or a collective farm—can be paid less than this amount.
- **Social Safety Net Indexation:** Non-working social transfers automatically derive from the minimum wage:
  - **Unemployed Welfare:** Configurable fraction of minimum wage (Default: $40\%$).
  - **Student Stipend:** Configurable fraction of minimum wage (Default: $60\%$).
  - **Retiree Pension:** Configurable fraction of minimum wage (Default: $80\%$).
- **Currency Neutrality:** The minimum wage is defined in base units and scales automatically when toggling between Rubles and US Dollars.

### 2.2 State Sector Governance: Tropico-Style Scope Control

When the player alters the wage of a state-owned building in the UI, they are presented with a scope decision:

1. **Specific Workplace Scope ("This building only"):**
   - Adjusts the wage for _only_ the currently selected building instance.
   - **Gameplay Use-Case:** Creating a high-priority "Hero of Socialist Labor" flagship factory (e.g., offering double pay at an essential heating plant or nuclear power facility to guarantee it is 100% staffed at all times).
2. **Sector-Wide Scope ("All buildings of this category"):**
   - Propagates the new wage to all buildings belonging to the same industrial sector across the entire Republic.
   - **Categorical Sectors (Look for the types as the game catagorises them with ghidra):**
     - _Extraction & Heavy Industry:_ Mines, Steel Mills, Oil Wells, Refineries.
     - _Strategic Infrastructure:_ Power Plants, Heating Plants, Water & Waste Processing.
     - _Logistics & Construction:_ Construction Offices, Depots, Distribution Centers.
     - _Healthcare & Education:_ Hospitals, Clinics, Schools, Universities.
     - _Light Industry & Agriculture:_ Food Processing, Fabric, Clothing, Farms.
     - _Retail & Civic Services:_ Grocery Stores, Department Stores, Pubs, Cinemas, Sports Halls.

### 2.3 Private Sector Dynamics: Profit-Driven Wage Bidding

Private enterprises (`[P]`) do not use manual player wage sliders. Their wages are governed autonomously by their balance sheets:

- **The Solvency & Retention Loop:**
  - At the monthly boundary, each private firm evaluates its **net operating profit**, **cash reserves**, and **staffing percentage** ($\text{Active Shifts} / \text{Worker Capacity}$).
- **Competitive Wage Bidding (High Profit + Low Staff):**
  - If a private business has positive net profits, healthy capital reserves ($> \text{Legal Reserve}$), and is suffering from understaffing ($< 75\%$ worker capacity), it increments its wage bonus by $+5\%$ to $+15\%$ above the state baseline.
  - Maximum competitive wage cap: $+50\%$ above state sector equivalents.
- **Austerity Wage Cuts (Financial Distress):**
  - If a private business is operating at a net loss or has negative capital, it implements austerity wage cuts, reducing salaries in $5\%$ increments each period down to the legal **Statutory Minimum Wage**.
  - A firm cannot reduce wages below the State Minimum Wage; if it cannot afford minimum wage, it defaults on payroll and triggers the Severance & Insolvency state machine (Task 3).

### 2.4 Macroeconomic Linkage

- **Private Construction:** `private_construction` automatically queries `citizen_labor->getDefaultDailyWage()` (or the sector wage) to calculate construction labor ($2.5\times$ base wage). Raising wages in the Republic immediately increases autonomous building costs, creating natural economic feedback.
- **Retail & Rent Viability:** Citizens with higher wages possess higher disposable incomes in their wallets, allowing private shops to sell goods at higher markups and private landlords to collect higher attractiveness rents without causing citizen poverty.

---

## 3. Data Model & Architecture Specifications

### 3.1 Wage Resolution Hierarchy

When `citizen_labor` determines the daily wage for a worker at building $B$ of type $T$:

1. Check if building $B$ has a **Specific Instance Override** registered in memory. If yes $\to$ Use override.
2. Check if building type $T$ has an **Industry Sector Wage** registered. If yes $\to$ Use sector wage.
3. Fall back to the **Global Default Daily Wage**.
4. **If Private Enterprise:** Apply the firm's dynamic profit/distress bonus multiplier.
5. **Enforce Legal Floor:** Clamp final wage: $\text{Effective Wage} = \max(\text{Resolved Wage}, \; \text{Minimum Wage})$.

### 3.2 State Persistence Requirements

- **Persistence Schema Upgrade (`tesmio_labor.dat`):**
  - Current version: v2 (Records: citizen employment, payday records, shifts, defaults).
  - Target version: v3.
  - New storage sections:
    - **Specific Workplace Overrides Table:** Pairs of `{ uint64_t buildingId/coordHash, float dailyWage }`.
    - **Industry Sector Wage Table:** Array of 16 sector wage floats.
    - **Republic Minimum Wage Float.**
  - _Backward Compatibility:_ An existing save file with v2 format must load smoothly, initializing missing tables from default INI values without corruption.

---

## 4. UI Reverse-Engineering Directives (Ghidra Targets)

To achieve native in-game wage control without clumsy console commands or external text editors, the agent must reverse-engineer the engine's native dialog and window structures.

### 4.1 Native Input Box & Edit Field Discovery

- **Ghidra Investigation Target:** Examine the building window dispatcher at `0x1406E9B80` (RVA `0x6E9B80`) and the window chrome helper `0x1406F8CC0` (RVA `0x6F8CC0`).
- **Existing Input Fields in the Game:**
  1. _Building Custom Name Input Box:_ When the player renames a building, the engine creates an active text input box inside the building window.
  2. _Worker Target Limit Field:_ The field that controls citizen limit targets (`building + 0xDDC`).
  3. _Import/Export Threshold Controls:_ Sliders and numeric entry boxes in storage/customs buildings.
- **Goal for Agent:** Locate the UI control class (e.g. `C3D_GUI_EDITBOX`, `C3D_WINDOW_INPUT`, or virtual method table for text input fields). Identify how numeric inputs capture keyboard input and store floats.

### 4.2 The Scope Toggle (Building vs. Category)

- **Ghidra Investigation Target:**
  - Inspect the native "Replace Vehicles" tool or "Construction Office Assignment" window. These windows feature native radio buttons / toggle checkmarks for **"Apply to this building"** vs. **"Apply to all lines/buildings"**.
- **Integration Strategy:**
  - In the in-window panel appended by `ui_panels`:
    - Render the current daily wage.
    - Render a toggle indicator: `[Scope: Workplace Only]` $\leftrightarrow$ `[Scope: Whole Sector]`.
    - Allow cycling scope via UI interaction or hotkey when the window is focused.

### 4.3 In-Window UI Telemetry via `ui_panels`

Add clear labor lines to the native building window:

- **For State Workplaces:**
  `Daily Wage: 3.20 RUB [State Sector: Heavy Industry]`  
  `Effective Pay: 3.20 RUB (Minimum Wage: 2.50 RUB)`
- **For Private Workplaces:**
  `Daily Wage: 3.90 RUB (+30% Market Profit Bonus)`  
  `Status: Solvent (Competitive Hiring Active)`
- **For Distressed Private Workplaces:**
  `Daily Wage: 2.50 RUB (Austerity Mode: Legal Floor)`  
  `Status: Insolvent (Hiring Frozen)`

---

## 5. Private Sector Wage Dynamic State Machine

```
                            PRIVATE WAGE STATE MACHINE

                               [ Normal Solvency ]
                             (Paying Standard Base)
                                  │          ▲
                  Staff < 75%     │          │  Distress Cleared
                  & Profit > 0    │          │  or Staff Rebalanced
                                  ▼          │
                        [ Competitive Bidding ]
                     (+5% to +15% per period wage raise)
                     (Capped at +50% above State baseline)
                                  │
                                  │ Operating Loss / Insolvent
                                  ▼
                            [ Austerity Mode ]
                     (-5% to -10% per period wage cut)
                     (Clamped at National Minimum Wage)
                                  │
                                  │ Insolvent for 2 Consecutive Months
                                  ▼
                         [ Payroll Default Streak ]
                         (Severance & Poisoning Triggered)
```

### 5.1 Tick & Rollover Operations

Inside `citizen_labor.cpp` during the monthly or weekly scheduling tick:

1. **For each private enterprise in `enterprise_base`:**
   - Retrieve net profit: $\text{Gross Revenue} - (\text{Wholesale} + \text{Wages} + \text{Taxes})$.
   - Retrieve current capital balance and legal reserve threshold.
   - Retrieve current staffing count from `bld + TSM_BLD_PEOPLE_BEGIN`.
2. **If Enterprise is Solvent & Profitable:**
   - If worker count is below $75\%$ of max capacity $\to$ Increment firm's wage bonus by $+0.05\times$.
   - If worker count is fully saturated ($100\%$) $\to$ Freeze wage bonus at current level.
3. **If Enterprise is Insolvent or Loss-Making:**
   - Decrement firm's wage bonus by $-0.05\times$.
   - Ensure the resulting daily wage never drops below `minimum_wage`.
   - If capital $\le 0.0$ and wage is already at `minimum_wage`, the firm is unable to cut costs further $\to$ enter Default Streak (triggering Severance in `citizen_labor` and leading into Task 3 Dereliction).

---

## 6. Configuration Schema (`citizen_labor.ini`)

The configuration file must be updated to support the new economic concepts cleanly in UTF-8 without BOM:

```ini
[citizen_labor]
enabled = 1
probe = 1
payday_frequency = monthly
days_per_month = 30
prune_after_days = 90

; Statutory Minimum Wage floor (All wages and transfers cannot fall below this)
minimum_wage = 2.50
minimum_wage_usd = 0.50

; Global Default Wage (used if unassigned to a specific sector)
default_daily_wage = 3.00
default_daily_wage_usd = 0.60

; Statutory Social Transfers (Defined as multipliers of Minimum Wage if positive)
welfare_ratio_to_min = 0.40
stipend_ratio_to_min = 0.60
pension_ratio_to_min = 0.80

; Industry Sector Base Wages (RUB bases; auto-divided in USD mode unless overridden)
[sector_wages]
heavy_industry = 3.80
infrastructure = 3.50
healthcare_education = 3.20
light_industry = 3.00
services_retail = 2.80
agriculture = 2.60

[private_market_wages]
; Enable autonomous profit-based wage bidding for private firms
dynamic_bidding_enabled = 1
; Maximum wage premium a booming private firm will offer (+50%)
max_private_premium = 1.50
; Minimum staffing ratio that triggers competitive wage increases (75%)
understaffed_threshold = 0.75
```

---

## 7. Implementation Phasing & Verification Milestones

### Milestone 1: Minimum Wage & Sector Hierarchy (Foundation)

- **Goal:** Implement the minimum wage legal floor, social transfer indexation, and sector wage mapping.
- **Verification Gate:** Start test game; verify in log that no citizen is paid below minimum wage, and verify that changing sector wages in the INI applies to all buildings in that category.

### Milestone 2: Private Sector Dynamic Bidding (Simulation)

- **Goal:** Implement the autonomous profitability/staffing feedback loop for private businesses.
- **Verification Gate:** Build a private factory with high profits and few workers. Watch its daily wage float from $3.00\text{ RUB} \to 3.45\text{ RUB} \to 3.90\text{ RUB}$ over consecutive months. Cut its supplies; watch it enter austerity and drop back to minimum wage.

### Milestone 3: In-Window Scope Control & UI Integration

- **Goal:** Use Ghidra to locate native edit controls or hook in-window interaction for workplace wage tuning.
- **Verification Gate:** Change a state coal mine's wage to $5.00\text{ RUB}$ with scope set to "Workplace Only"; confirm other coal mines remain at standard pay, while the inspected mine displays and pays $5.00\text{ RUB}$.

---

## 8. Safety & Engineering Directives for the Agent

1. **Preserve ABI Layouts:** When extending `TsmCitizenLaborApi`, append all new accessors (`getMinimumWage`, `getSectorWage`, `setWorkplaceWageOverride`) strictly at the end of the struct, guarded by `structSize` checks.
2. **Locking & Thread Boundaries:** Wage overrides will be modified on the render thread (via UI) and read on the simulation thread (during payday). Guard all override map accesses with `EnterCriticalSection(&g_lock)`.
3. **No GPL Code Usage:** Reverse-engineer UI widgets cleanly using Ghidra inspection without copying third-party code.
4. **UTF-8 Without BOM:** Ensure all modified INIs preserve clean UTF-8 headers (`no BOM`).
5. **Active Currency Neutrality:** Ensure all calculations check `TsmCurrencyActive()` and use dynamic symbols (`RUB` vs. `USD`).
