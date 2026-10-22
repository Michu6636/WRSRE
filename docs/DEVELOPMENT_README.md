# TesmioLoader Plugin Ecosystem: Architectural Blueprint & Developer Manual

**Target Binary:** `SOVIET64.exe` v1.1.1.9 (64-bit DX11.1, base address `0x140000000`)  
**Mod Framework:** TesmioLoader C++17 Modular Plugin Architecture  
**Document Purpose:** Master technical reference for AI agents and developers implementing new features, maintaining existing modules, and ensuring seamless cross-plugin integration.

---

> **Update 2026-10-22 (Rev 4):** Added `private_logistics` L3, corrected `TSM_GO_BUILDINGS_VEC` to `0x11B08/0x11B10`, added DO task/storage/connection offsets. Previous staging docs in `docs/README.md` and `docs/ghidra-do-storage-findings.md` supersede `docs/Logistics_Investigation.md` for DO task details. See evidence labels in `docs/README.md`.

## 1. Architectural Hierarchy & Dependency Rules

The ecosystem is organized into a strict **4-Tier Level Hierarchy**. Higher levels may consume services from lower levels. **Cyclic dependencies between plugins are strictly prohibited.**

```
┌────────────────────────────────────────────────────────────────────────┐
│ LEVEL 3: AUTONOMOUS APPLICATION SYSTEMS                                │
│ - private_construction (Autonomous building, native phase pricing)      │
│ - private_shops (Wholesale deliveries, procurement, service fees)       │
│ - private_logistics (Transfer accounting, freight/fuel, opt-in DO targets)│
│ - state_ledger (Macro financial balance sheet, CSV exports, HUD stats) │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ consumes
┌───────────────────────────────────▼────────────────────────────────────┐
│ LEVEL 2: INSTITUTIONAL & DISPLAY FRAMEWORKS                            │
│ - enterprise_base (Capital register, pool, rent, wage bidding, [P] tag)│
│ - taxes (VAT, corporate profit tax, flat property tax)                 │
│ - ui_panels (In-window custom row injection at window+0x250)           │
│ - ui_overlay (HUD text renderer hooked on terrain render pass)         │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ consumes
┌───────────────────────────────────▼────────────────────────────────────┐
│ LEVEL 1: DOMAIN SERVICES (GOODS & CITIZENS)                            │
│ - citizen_labor (Employment register, wages, minimum floor, sectors)  │
│ - citizen_retail (Consumer shopping walks, demand fulfillment)         │
│ - resource_pricing (Live commodity pricing, border import multipliers) │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ consumes
┌───────────────────────────────────▼────────────────────────────────────┐
│ LEVEL 0: CORE OS & ENGINE FOUNDATION                                   │
│ - sim_clock (Calendar ticks, scheduled events, pause & speed tracking) │
│ - currency (Citizen wallets, treasury doubles, flow observer bus)      │
│ - persistence (Atomic save/load file system, CRT file hooks)           │
└────────────────────────────────────────────────────────────────────────┘
```

### Dependency Rules:

1. **Level $N$ $\rightarrow$ Level $\le N-1$:** A plugin at Level 2 may consume Level 1 and Level 0 services. It may **never** consume Level 3 services.
2. **Intra-Level Communication:** Allowed only through formal decoupled callbacks/interceptors (e.g., `citizen_labor` calling an optional payroll interceptor provided by `enterprise_base`).
3. **Graceful Degradation (Soft Dependencies):** If an optional service is missing, the plugin must log a `WARN` and fall back to safe internal defaults rather than refusing to load or crashing.

---

## 2. Complete Plugin Catalog

| Plugin                     | Level | Version | Consumes                                                                                                      | Publishes                 | Primary Responsibility                                                                                                                                                            |
| :------------------------- | :---: | :-----: | :------------------------------------------------------------------------------------------------------------ | :------------------------ | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`sim_clock`**            |   0   |   1.1   | _None_                                                                                                        | `sim_clock`               | Provides in-game day/month/year calendar, scheduled callbacks (`TSM_FREQ_*`), and game speed/pause telemetry.                                                                     |
| **`currency`**             |   0   |   4.0   | `sim_clock`, `persistence`                                                                                    | `currency` (v3 & v4)      | Tracks citizen wallets, direct read/write to state treasuries (`game+0x580` USD / `+0x588` RUB), and publishes labelled flow observer events.                                     |
| **`persistence`**          |   0   |   1.0   | _None_                                                                                                        | `persistence`             | Hooks CRT file operations (`fopen`, `_wfopen`) on save triggers (`stats.ini` / `workers.bin`); provides atomic file writing.                                                      |
| **`resource_pricing`**     |   1   |   1.1   | _None_                                                                                                        | `resource_pricing`        | Linear scan of native resource vector (`game+0xC2B0`); reads live border commodity prices (`+0x58` RUB / `+0x5C` USD) and fallbacks.                                              |
| **`citizen_labor`**        |   1   |   2.0   | `sim_clock`, `currency`, `persistence`                                                                        | `citizen_labor` (v2 & v3) | Tracks citizen employment, executes daily paydays, enforces the **Statutory Minimum Wage**, maps 6 industrial sectors, persists coordinate wage overrides.                        |
| **`citizen_retail`**       |   1   |   1.2   | `sim_clock`, `currency`, `resource_pricing`, `persistence`                                                    | `citizen_retail`          | Iterates active shops, validates physical store inventory, charges citizen wallets for multi-item consumer baskets.                                                               |
| **`enterprise_base`**      |   2   |   7.0   | `sim_clock`, `currency`, `persistence`, `ui_overlay`, `ui_panels`, `citizen_labor`, `citizen_retail`, `taxes` | `enterprise_base`         | Commercial firm registry for `[P]` buildings, company capital balances, residential rent collection, investment pool siphon, dynamic wage bidding.                                |
| **`taxes`**                |   2   |   1.1   | `sim_clock`, `currency`, `enterprise_base`                                                                    | `taxes`                   | Calculates and settles VAT (sales), Corporate Profit Tax (net operating margins), and flat Property Tax on scheduled calendar boundaries.                                         |
| **`ui_overlay`**           |   2   |   1.0   | _None_                                                                                                        | `ui_overlay`              | Hooks `C3D_TERRAIN::Render` to draw DPI-scaled outlined HUD text; provides F8 toggleable drawer bus.                                                                              |
| **`ui_panels`**            |   2   |   1.0   | _None_                                                                                                        | `ui_panels`               | Hooks native panel builders (shop, residence, pub, factory) to append custom telemetry rows at `window+0x250`.                                                                    |
| **`private_shops`**        |   3   |   3.0   | `sim_clock`, `currency`, `resource_pricing`, `enterprise_base`, `taxes`, `persistence`                        | _None_                    | Auto-imports foreign restock, bills state wholesale delivery, credits state procurement revenue on export-flagged storages (`+0x85`), collects pub service fees.                  |
| **`private_logistics`**    |   3   |  0.17   | `sim_clock`, `currency`, `resource_pricing`, `enterprise_base`, `ui_panels`, `persistence`                                                    | _None_                    | Transfer accounting, producer settlement (90% retail), freight/fuel billing, and bounded opt-in state-storage targets (load-only `TaskCfg+0x04=1.0/+0xC8=0/+0xCC=0`, cap 5, `auto_targets_dry_run=1`). See `docs/ghidra-do-storage-findings.md` and `docs/current-task-state-storage.md`. |
| **`private_construction`** |   3   |  3.5.2  | `sim_clock`, `enterprise_base`, `ui_overlay`, `citizen_labor`, `currency`, `resource_pricing`, `persistence`  | _None_                    | Autonomously builds `[P]` sites using investment pool funds; prices natively via type descriptor phases (`typedesc+0x370`); implements State Acceptance Commission (99% handoff). |
| **`state_ledger`**         |   3   |   1.4   | `sim_clock`, `currency`, `persistence`, `ui_overlay`                                                          | _None_                    | Observes all macro cash movements; records monthly double-entry statements to `tesmio_ledger.csv`; renders financial HUD card.                                                    |

---

## 3. Engine Surface & Memory Layout Reference (`game_world.h`)

All addresses are relative to `SOVIET64.exe` image base (`0x140000000`).

### 3.1 Global Game Object & Core Pointers

- **`RVA_GAME_OBJECT` = `0x9D4F10`** $\rightarrow$ Pointer to root `C3D_GAME` instance.
- **`TSM_GO_TREASURY_USD` = `0x580`** (`double`) $\rightarrow$ State US Dollar treasury balance.
- **`TSM_GO_TREASURY_RUB` = `0x588`** (`double`) $\rightarrow$ State Soviet Ruble treasury balance.
- **`TSM_GO_RES_VEC` = `0xC2B0`** $\rightarrow$ Vector (`begin/end/cap`) of all resource records (Stride = `832` / `0x340`).
- **`TSM_GO_BUILDINGS_VEC` = `0x11B08` / `0x11B10`** (`TSM_GO_BLD_VEC_BEGIN/END`) $\rightarrow$ Vector of all building pointers on the active map. *Correction 2026-10-22: `src/game_world.h` authoritative; `0xC2C8` was stale.*
- **`TSM_GO_POLLUTION_GRID` = `0x120A8`** $\rightarrow$ Bilinear environmental/pollution grid pointer.

### 3.2 Building Struct (`bld`)

- **`+0x008` / `+0x010`**: Building coordinates `posX`, `posZ` (Floats).
- **`+0x318` (`TSM_BLD_TYPEDESC`)**: Pointer to Building Type Descriptor (`TypeDesc`).
- **`+0x5D0` (`TSM_BLD_BBOX`)**: Pointer to Bounding Box struct (`bb + 0x20` contains `mn[1]` / `mn[3]` min-coords).
- **`+0x604` (`TSM_BLD_BUILT`)** (`float`, `0.0f` to `1.0f`): Current construction progress.
- **`+0x698` (`TSM_BLD_DEMOLISHED`)** (`BYTE`): $0 =$ Active, non-zero $=$ Marked for demolition.
- **`+0xBD8` / `+0xBE0` (`TSM_BLD_PEOPLE_BEGIN / END`)**: Pointer vector (`void**`) of visitors/customers inside the building.
- **`+0xBC0` / `+0xBC8` (`TSM_BLD_RESIDENTS_BEGIN / END` / `TSM_BLD_WORKERS_BEGIN / END`)**: Pointer vector of permanent tenants (Residential) or active workers/staff (workplaces).
- **`+0xDDC` (`TSM_BLD_CUSTOMER_CAP`)** (`float`): Customer service capacity — max customers attended at once. NOT the worker limit.
- **`+0xDEC` (`TSM_BLD_MAX_WORKERS`)** (`int`): Maximum worker limit, initialized from type descriptor `$WORKERS_NEEDED` at construction.
- **`+0x970` / `+0x978` (`TSM_BLD_STORAGES_BEGIN/END`)**: Storage vector (stride `0xE0`, import `+0x84`/export `+0x85`, capacity `+0x8C`).
- **`+0xC70` / `+0xC78`**: Vehicle vector of owning DO (stride 8).
- **`+0xD38` / `+0xD40`**: DO task vector (stride `0x18` Entry `{+0x00 enabled byte, +0x08 target*, +0x10 TaskCfg*}`; `TaskCfg` `0x198` via `0x1512A0`; dirty `+0xD50=1`, schedule `+0xFC0=3.0`). See `docs/ghidra-do-storage-findings.md`.
- **`+0xA10`**: Connection records (stride `0x60`, type 3 road / 6 rail).
- **`+0x108` (`TSM_BLD_CITY`)**: City/NamePoint*.
- **Correction:** `+0xC38` is idle counter, not residents vector. Use `+0xBC0` per `Building_Vectors_Investigation` and `Citizen_Housing_Relocation`.
- **`+0x11B0`** (`float`, `0.0f` to `1.0f`): Daily sampled environmental attractiveness score.

### 3.3 Building Type Descriptor (`typedesc`)

- **`+0x000` (`TSM_TD_NAME`)**: Null-terminated ASCII object identifier (e.g. `"fabric_factory"`, `"blok_2seg_2p"`).
- **`+0x360` (`TSM_TD_TYPE_NUM`)** (`int`): Canonical building type number (Matches `SOVIETInstructions.txt`).
- **`+0x370` / `+0x378` (`TSM_TD_PHASES_BEGIN / END`)**: Vector of construction phase structs (Phase Stride = `0x21B8`).
  - Inside each phase at **`+0x008` / `+0x010` (`TSM_PHASE_COSTS`)**: Vector of material requirement entries (Entry Stride = `0x10`).
  - Entry **`+0x00`**: Pointer to resource record.
  - Entry **`+0x08`**: Amount required (`float`, tons or worker days).
- **`+0x920` / `+0x938`**: Visual mesh node and ruined particle emitter state matrices.

### 3.4 Resource Record Struct (`res`)

- **`+0x000`**: ASCII name string (e.g. `"concrete"`, `"steel"`, `"workers"`).
- **`+0x58` (`TSM_RES_PRICE_RUB`)** (`float`): Live dynamic market price per ton in Rubles.
- **`+0x5C` (`TSM_RES_PRICE_USD`)** (`float`): Live dynamic market price per ton in US Dollars.
- **`+0x78` / `+0x7C`**: 1960 Baseline starting prices (RUB / USD).
- **`+0x8C` (`TSM_RES_BUY_MULT_RUB`)** (`float`): Live import buy multiplier for Rubles.
- **`+0xAC` (`TSM_RES_BUY_MULT_USD`)** (`float`): Live import buy multiplier for US Dollars.

### 3.5 Citizen Person Struct (`person`)

- **`+0x028` (`TSM_PERSON_AGE`)** (`float`): Age in in-game years.
- **`+0x074` (`TSM_PERSON_FOREIGN`)** (`int`): Non-zero if foreign worker.
- **`+0x078` (`TSM_PERSON_TOURIST`)** (`int`): Non-zero if foreign tourist.
- **`+0x0E0` (`TSM_PERSON_DEMAND_N`)** (`int`): Number of active consumer demands (Capped at 8).
- **`+0x0E8` (`TSM_PERSON_DEMANDS`)**: Array of demand entries (Stride = `0x80` / `128` bytes).
  - Demand **`+0x00`** (`float`): Amount still needed.
  - Demand **`+0x08`** (`int`): Demand kind ($1 =$ Food, $2 =$ Goods/Clothes/Electronics, $3 =$ Alcohol, $4 =$ Religion, $5 =$ Culture, $6 =$ Sport, $10 =$ Health).
  - Demand **`+0x10`** (`void*`): Pointer to specific resource record.

---

## 4. Core Cross-Cutting Subsystems

### 4.1 The Currency Architecture (v4 Contract)

The mod features a unified **Dual-Currency Regime** governed by `currency.ini`:

```ini
primary_currency = rub   ; (or "usd")
```

- **Routing Contract:**
  - When set to `rub`, all labelled state flows (`moveTreasury`) write to `game+0x588` (RUB Treasury).
  - When set to `usd`, all labelled state flows write to `game+0x580` (USD Treasury).
- **Dynamic Currency Access:**
  Plugins must consume `currency_api.h` and check:
  ```cpp
  int cur = TsmCurrencyActive(g_currency); // TSM_CURRENCY_RUB (0) or TSM_CURRENCY_USD (1)
  const char* sym = TsmCurrencySymbol(g_currency); // "RUB" or "USD"
  ```
- **USD Divisor Auto-Scaling:**
  To prevent hardcoded Rouble numbers from inflating the economy when running in USD mode, all plugins implement the **Divisor Pattern**:
  $$\text{Effective USD Value} = \frac{\text{Configured Rouble Base}}{\text{usd\_divisor}} \quad (\text{Default divisor: } 50.0)$$
  _Explicit `_\_usd` keys in INI files always take precedence over the auto-divisor.\*

### 4.2 Building Identity Across Saves (The Coordinate Rule)

> **CRITICAL RULE:** In _Workers & Resources_, raw memory pointers (`void* building`) are randomized every time a save is loaded. **Never persist raw memory pointers to disk.**

To persist building-specific settings (such as private enterprise capital, construction tracking, or custom salary overrides), use the **Permanent Coordinate Pattern**:

1. **In-Memory (Running Game):** Use fast pointer lookups: `std::unordered_map<void*, Data> g_map;`.
2. **On Save (`TSM_SAVE_WRITING`):** Read the building's permanent bounding box coordinates via `bld + 0x5D0`:
   ```cpp
   float px = 0.0f, pz = 0.0f;
   ReadBuildingPosition(bld, &px, &pz); // mn[1] and mn[3] from bbox+0x20
   record.posX = px;
   record.posZ = pz;
   ```
3. **On Load (`TSM_SAVE_LOADING`):** Stage pending records from disk. On the next building walk, match pending records against spawned building coordinates within a **$2.0$-meter tolerance** (`fabsf(liveX - savedX) < 2.0f`).

### 4.3 In-Window UI Panel Architecture (`ui_panels`)

`ui_panels` injects custom telemetry rows into native building windows without modifying game files:

- **Target Post-Hooks:** Shop (`0x7272F0`), Residence (`0x761300`), Pub (`0x723950`), Factory (`0x799630`).
- **The `window+0x250` Contract:**
  1. The game lays out its native window and leaves its current vertical bottom coordinate at `window+0x250` (`float`).
  2. Our registered row provider appends rows at that Y coordinate.
  3. Our hook updates `window+0x250` to the new bottom, causing the window chrome to expand downwards without clipping.
- **Render-Thread Rules:** Panel row providers run on the **Render Thread** at 60+ FPS.
  - Hold locks for microseconds only.
  - Format strings in stack buffers (`wchar_t buf[128]`).
  - **NEVER allocate large dynamic memory, perform file I/O, or execute heavy loops inside a panel row provider.**

### 4.4 The Wage Hierarchy & Labor System (`citizen_labor`)

When calculating worker pay at building $B$ of type $T$:
$$\text{Workplace Override} \;\longrightarrow\; \text{Sector Wage} \;\longrightarrow\; \text{Legacy Type Wage} \;\longrightarrow\; \text{Default Wage} \;\longrightarrow\; \mathbf{Clamped\;UP\;to\;Minimum\;Wage}$$

1. **Specific Workplace Override:** Configured per-building via coordinates.
2. **Industry Sectors:** Types are categorized into 6 sectors (`Heavy Industry`, `Infrastructure`, `Care/Education`, `Light Industry`, `Services/Retail`, `Agriculture`).
3. **Private Sector Multiplier:** Applied dynamically by `enterprise_base`:
   $$\text{Final Wage} = \text{Resolved Wage} \times \text{wageBonusMult}$$
4. **Statutory Floor:** The final daily wage is strictly clamped:
   $$\text{Pay} = \max(\text{Final Wage}, \; \text{minimum\_wage})$$

---

## 5. Engineering Standards & Defensive Coding Conventions

All code must adhere to these non-negotiable stability standards:

### 5.1 SEH Fault Containment

Any function accessing game memory pointers, walking engine arrays, or calling inter-plugin function pointers must be guarded by Microsoft Structured Exception Handling (`__try / __except`):

```cpp
__try {
    // Game memory read / write operations
} __except (FaultFilter("plugin_name operation", GetExceptionInformation())) {
    // Log once, isolate fault, and degrade gracefully
}
```

_Never let an unhandled memory violation escape a plugin and crash `SOVIET64.exe`._

### 5.2 Pointer Validation Discipline

Before dereferencing any game pointer:

1. Verify it lies within plausible game address space: `TsmIsValidGamePtr(ptr)`.
2. Verify the memory region is readable for the required byte width: `ReadablePtr(ptr, size)`.

### 5.3 Threading & Synchronization

- **Render Thread vs. Simulation Thread:**
  - UI Panels and Overlay drawers run on the **Render Thread**.
  - Sim Clock ticks and daily paydays run on the **Simulation Thread**.
- **Locking Policy:** Always synchronize cross-thread shared maps using `EnterCriticalSection(&g_lock)` / `LeaveCriticalSection(&g_lock)`. Keep critical sections scoped to simple copies.

### 5.4 ABI Backwards Compatibility

- **Append-Only Structs:** Never insert or reorder function pointers in public API header files (`*_api.h`). Always append new function pointers to the very end of the struct.
- **Struct Size Guards:** Consumers must always check `api->structSize` before invoking tail functions to guarantee compatibility across mixed plugin versions.

### 5.5 INI File Encoding (The No-BOM Rule)

- **Strict UTF-8 without BOM:** All `.ini` configuration files must be saved strictly without Byte Order Marks (`59 32 99` header / ASCII `[`).
- _Why:_ Windows API `GetPrivateProfileStringA` treats a UTF-8 BOM (`EF BB BF`) as corrupt characters, breaking the first section header in the file.

### 5.6 Clean-Room Legal Integrity

- Mod code is built from reverse-engineered game facts, offset structures, and engine contracts.
- **Do NOT copy third-party source code** (e.g. GPL-licensed plugins). Discoveries and layout offsets are borrowed as architectural facts; all implementations must be written clean-room.

---

## 6. Build & Deployment Pipeline

- **Compiler:** Microsoft Visual C++ (MSVC x64), C++17 Standard, Static Runtime (`/MT`).
- **Build Script:** `build.bat <plugin_name>` or `build.bat` (full 13-plugin suite).
- **Target Deployment Directory:**  
  `C:\Workers Resources Soviet Republic\build\plugins\`
- **Log File Inspection:**  
  All boot diagnostics, registration telemetry, and memory warnings write directly to:  
  `C:\Workers Resources Soviet Republic\build\tesmioloader.log`

---

## 7. Golden Rules for Future Feature Implementation

When designing and coding future features (such as **Task 1: Dynamic Rent**, **Task 2: Macro Supply & Demand**, or **Task 3: Insolvency & Physical Abandonment**):

1. **Income Precedes Expense:** Ensure citizen salaries and wallet balances are balanced before introducing new citizen fees or dynamic rent escalations.
2. **Physical Over Abstract:** Leverage the game's physical simulation (actual tons in export storages, actual people walking into shops) rather than relying on abstract spreadsheet counters.
3. **Persist Through Coordinates:** Any game entity that holds mod state across savegames must be indexed by world coordinates or custom name, never by temporary runtime memory address.
4. **Log State Changes Explicitly:** Use conditional probe flags (`if (g_probe) Logf(...)`) so gameplay events (wage hikes, store sales, pool siphons, site claims) leave clear audit trails in `tesmioloader.log`.
