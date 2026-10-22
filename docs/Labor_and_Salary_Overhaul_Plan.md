# Labor & Salary Overhaul — Implementation Plan (M1: Minimum Wage, Sector Hierarchy & Labor API v3)

**Target:** `SOVIET64.exe` v1.1.1.9 · **Module:** `citizen_labor` (consumes `sim_clock`, `currency`, `persistence`; publishing to `enterprise_base`, `private_construction`) · **Arch:** Level 1 Core Simulation Service · **Currency:** Unified Dual-Currency (RUB/USD via `currency_api` v4)

> Saved from the plan-only investigation (Ghidra + code + game files). M1 executes the foundation; M2–M3 follow in order.

---

## 1. Baseline (verified)

- `citizen_labor` v1.8: flat `unordered_map<int,float> g_wageTable` from `[wages]`, parallel `[wages_usd]`; `GetWageForTypeImpl` is the single chokepoint; `GetEffectiveDailyWageImpl` age-gates then type-wage; private path is the single `g_interceptor` (`enterprise_base::OnPayroll`). Payroll daily `OnDay`: Pass 1 workplace sightings (`bld+0xBD8/0xBE0`, age gate, service-patron filter via demand kind at `demand+0x08`), Pass 2 residential payday with daily/weekly/monthly multiplier + `scale_pay_by_shifts`. Persistence v2 `tesmio_labor.dat` (`TSMLABR1`): citizen records only, workplace pointers never persisted, `structSize` guards, `g_lock` throughout. Transfers are independent scalars (6/12/18), not ratios.
- `enterprise_base` v6.2: `Enterprise` capital/markup/month+period totals/accruals; `OnPayroll` = `grossWage * private_wage_bonus` (1.00), `IsSolvent = capital>0`; siphon `netProfit = gross-(wages+wholesale+taxes)` over `legal_reserve_flat`. Staffing numerator readable today (people vector length at `bld+0xBD8/0xBE0`).
- `ui_panels` v1.0: 4 verified post-hooks (shop `0x7272F0`, residence `0x761300`, pub `0x723950`, factory `0x799630`), `window+0x240` = building, `+0x250` = bottom edge, DPI `0x992088`, font `0x994200`. Display-only; no input path yet.

## 2. Ghidra findings (plan session)

- Window chrome `0x6F8CC0`: `ListFuncs 0x1406F8000–0x1406FA000` → `FUN_1406f88d0` (998 B) + `FUN_1406f8cc0` (39534 B). Prologue `48 8B C4 55 56 57 41 54 41 55 41 56 41 57 / LEA RBP,[RAX-0xDB8] / SUB RSP,0xE80` — same 14-byte register-only prefix as the panel family, cookie XOR far past the steal (hookable shape, but 39 KB fn exceeds ~120 s decompile budget — sample with `DisasmMany`, never `DecompAt` the big fn).
- `DecompAt 0x1406F88D0` → chrome/progress-bar painter (`DrawFrom9Patch`, `PrintRightUnicode L"%.2f %%"`), not an editbox.
- Dispatcher `0x6E9B80` (`scratch/d_dispatcher.txt`): kind dispatch on `+0x34`/`+0x248`, `(kind==0x21 && +0x248==0)` → `FUN_1406f8cc0 + FUN_1407838e0`; confirms `+0x240` convention.
- Worker limit `building+0xDDC` **confirmed as staffing denominator**: `c_shop.c:260` `(people_end-people_begin)>>3 - (int)*(float*)(bld+0xDDC)`; same in `c_shop.c:470`, `g_pub_723950.c:339,527`. `staffing% = liveCount / max(1, *(float*)(bld+0xDDC))`. Not a control — a float the game reads.
- Sector/type mapping: **no Ghidra needed**. `media_soviet/scripts/SOVIETInstructions.txt:2105+` is authoritative (2 living, 3 shop, 4 school, 5 storage, 6 factory, 7 mine, 8 field, 9 farm, 10 kindergarten, 11 engine, 12/28 construction, 14/15 depots, 17 powerplant, 18/19 substation/transformer, 20 customhouse, 21 church, 22 kino, 23 pub, 24 sport, 25 hospital, 26 firestation, 27 university, 30 unknown, 31/32 electric, 33 monument, 34 gas, 37 broadcast, 38 parking, 39 cityhall, 40 production line, 41 car dealer, 42 ship dock, 43/44 distribution/container, 45 forklift, 46–48 airplane, 49 technical, 50 hotel, 51 attraction, 60 cooling, 70/71/72 heating, 73–77 police/orphanage/court/prison/secret-police, 80 bridge, 90–98 water/sewage, 105 trash, 106 demolition, 107 pipeline export, 108 repair, 109 scrapyard, 300 tram gate).
- Input widget (`C3D_GUI_EDITBOX` guess): **not found**. `x_getstring.txt` = no xrefs to guessed `0x997590`; rename is a memory scan, not a widget hook. M3 must start with `Xrefs` to `GetString` + string-ID harvest, or fall back to overlay/hotkey input. Scope-toggle radios: not located; panels are imperative (`GetString` + `PrintLeftUnicode`, `y += DPI*35`), no row object to clone.

## 3. Risks

Native text input is the critical unknown (M3 redesign risk if no editbox); `+0xDDC` is capacity not control; workplace identity is pointer-session (cross-save needs coord/type matching like `enterprise_base::FindMatchForRecord`); single interceptor slot; render/sim thread split (`g_lock`); append-only ABI (`structSize`); decompiler budget (DisasmMany/Xrefs only for big fns).
## 4. Milestone 1 — Minimum wage + sector hierarchy + API v3 (THIS TASK)

- **API** `src/citizen_labor_api.h` v2→v3, append-only: `getMinimumWage`, `getSectorWage(int)`, `setWorkplaceWageOverride`, `clearWorkplaceWageOverride`, `getEffectiveWageForBuilding`. Dual-provide v2 (prefix, truncated `structSize`) + v3 (full) like `currency` does, so `enterprise_base` (hardcoded `consume(...,2u)`) and old `private_construction.dll` keep working.
- **Sectors** (static `type→sector`, `SOVIETInstructions.txt` numbers): 0 heavy (7,60,107,109), 1 infrastructure (0,1,5,11,12,28,13,14,15,17,18,19,31,32,34,35,36,42,43,44,45,46,47,48,52,70,71,72,80,90,91,92,93,94,95,96,97,98,105,106,300 + 20), 2 healthcare/education (4,10,25,27,74), 3 light (6,40,49,108), 4 services/retail (3,21,22,23,24,26,33,37,38,39,41,50,51,73,75,76,77), 5 agriculture (8,9). Unmapped (incl. 2 living, 30 unknown) → default. Factory=light / mine=heavy split documented; per-factory heavy/light refinement is future work.
- **Resolver** `Override → Sector → Legacy [wages] fallback → Global Default → Clamped to Minimum`. Sector `0` = unconfigured (old INIs fall through to legacy, safe migration); new INI configures all six so sectors win and the M1 gate passes. Legacy `[wages]` kept as deprecated fallback only.
- **Transfers**: `welfare/stipend/pension_ratio_to_min` > 0 → `ratio x minimum_effective`; else legacy scalars (+ explicit `_usd` / divisor path). New INI uses ratios (0.40/0.60/0.80); old INIs without ratios keep legacy numbers.
- **Overrides**: runtime map keyed by live pointer with coords+type (`TSM_BLD_BBOX +0x5D0` → `bb+0x20` → `mn[1],mn[3]`, same as `enterprise_base::ReadBuildingPosition`, 2.0 m tolerance). Stored RUB-base (USD→RUB on set, RUB→effective on get) so toggling currency rescales.
- **Persistence** v2→v3: same header size, `reserved` repurposed as override count (v2 loaders ignore it → graceful). v3 = header + citizen records + `OverrideSaveRecord{posX,posZ,type,wageRub}[]`. Load accepts 1/2/3; v3 overrides go to pending and match on next `OnDay` by type+coords.
- **Currency**: every effective figure resolves at `Start` via `TsmCurrencyActive` — explicit `*_usd` wins, else RUB/divisor. Overrides currency-neutral as above.
- **Verify M1**: no pay below floor in log; sector INI edits apply republic-wide; v2 saves load clean; startup log shows currency + floor + sector table.

## 5. Milestone 2 — Private bidding

**STATUS: IMPLEMENTED + STARTUP-VERIFIED 2026-10-10 (enterprise_base v7.0, service v6 unchanged;
refined spec pass: wage_bidding_step key, Wage/Labor panel rows, max_rows 8).**
`Enterprise.wageBonusMult` (1.0..max_private_premium) + `inAusterity`, persisted as save v3
(v1/v2 records load with the 1.0 default, sanitised + clamped on apply). Monthly
`EvaluateWageBidding` runs in OnMonth after the siphon, BEFORE the accumulator clear loop
(same trap as the v6.1 dividend fix), fault-bracketed: profit>0 && capital>legal reserve &&
staffing<0.75 → +step/month (cap 1.50) with `[WAGE HIKE]` probe log; profit<=0 ||
capital<=0 → -step/month (mult floor 1.0) with `[AUSTERITY]` - both steps come from
`[private_market_wages] wage_bidding_step` (default 0.05, clamped 0.01..0.50). Staffing = workers vector (bld+0xBC0..0xBC8) / max(1, int at bld+0xDEC = `TSM_BLD_MAX_WORKERS` in game_world.h). OnPayroll pays
`grossWage x wageBonusMult` (legacy static `private_wage_bonus` only when
`dynamic_bidding_enabled = 0`) then floors to `getMinimumWage()` - labor consume now prefers
v3 with a v2 fallback (logged). Panel gains a "Wage" row: effective rate +
(+XX% Market Bonus | Market Baseline | Austerity Mode) and a "Labor" row
`XX/XX Staff (XX% Capacity)`, both in the active currency; ui_panels
max_rows 6 → 8. INI `[private_market_wages]` (dynamic_bidding_enabled/max_private_premium/
understaffed_threshold/wage_bidding_step), UTF-8 no BOM. Startup log confirmed:
`M2 wage bidding: on, premium cap +50%, understaffed < 75%, step +/-5.0%, wage floor active (labor v3)`,
`ui_panels live v1.0: 4 hook(s), max_rows 8, ...`, zero WARNs, service v6 intact. OPEN: in-world
playtest of the 3.00→3.45→3.90 float and the austerity descent (enterprise_base probe=1 during
the observation window).

## 6. Milestone 3 — In-window scope control + UI (needs Ghidra P0–P4)

(superseded by the implemented status below.)

**STATUS: IMPLEMENTED + STARTUP-VERIFIED 2026-10-10 (citizen_labor v3.0, API v4).**
The plan's "fallback (recommended default)" route was taken: no engine editbox was hooked —
the Ghidra P0–P4 probes (below) remain queued for a future native-widget pass. Instead
citizen_labor registers its own `ui_panels` row provider (`LaborPanelRows`) for
state-operated workplaces, and hotkeys are polled inside the provider (render thread, one
poll per ~16 ms bucket, game-foreground-gated via `GetForegroundWindow` + pid match):
- `[` / numpad `-` → wage −`wage_adjust_step` (default 0.10, auto-repeats while held);
  `]` / numpad `+` → +step; `T` → scope toggle (LSB edge, exactly one flip per press).
- Scope routing: Workplace Only → `setWorkplaceWageOverride` (persisted by coords since
  M1); Whole Sector → `setSectorWage` (new v4 accessor, `Api_SetSectorWage`, lock-guarded,
  runtime-only — resets from the INI on world load) + `clearWorkplaceWageOverride` so the
  building inherits cleanly. Both setters clamp up to `minimum_wage`.
- Panel rows: `Wage` (value suffix `(Workplace Override)` / `(Sector: <name>)` /
  `(Global Default)`), `Scope` (`[Workplace Only] (T: Whole Sector)` /
  `[Whole Sector - X] (T)`), `Tune` (step legend); active currency symbol inline. Private
  `[P]` buildings are skipped when enterprise_base is present (optional consume; layering
  note in code) so they keep the M2 rows. Sector display names via `kSectorNames`.
- Provider triple-publishes the API (v2 prefix / v3 prefix / v4 full); `TsmLaborHasV4`
  guard added. INI: `in_window_rows = 1`, `wage_adjust_step = 0.10`. Startup log confirmed:
  `service "citizen_labor" v2/v3/v4`, `in-window wage tuning registered (step 0.10 RUB)`,
  ui_panels max_rows 8, zero WARNs; enterprise_base v7.0 unaffected. OPEN: in-game
  interactive test (open a state building window, press T then `]`, confirm the Wage row
  moves, the Scope row flips, and the sector/override values change). Hotkey layout is
  VK_OEM_4/OEM_6 + numpad add/subtract — non-US keyboard layouts may need remapping.

## 7. Ghidra probe queue (sequential, file-based `0x140`+RVA, one run at a time)

P0 Xrefs GetString slot → rename/limit owners; P1 DisasmMany `0x1406F8CC0` windows; P2 ListFuncs Replace-Vehicles/construction-assignment; P3 Xrefs `bld+0xDDC` writers; P4 limit-row data flow. Keep raws in `scratch/labor_*`.

## 8. Acceptance gates

M1: no citizen below floor; sector edits republic-wide per sector; v2 saves load. M2: 3.00→3.45→3.90 float; austerity to floor; default→sever after 2 months at floor. M3: scope semantics correct; rows show wage/sector/min/bonus; no input crashes (`__try/__except` like `ui_panels`).

*Session Ghidra log: `ListFuncs 0x1406F8000–0x1406FA000` → `labor_list_chrome.txt`; `DisasmMany 0x1406F8CC0:0x1406F8D40` → `labor_chrome_6F8CC0.asm`; `DecompAt 0x1406F88D0` → `labor_chrome_decomp.c`; `Xrefs 0x140000DDC` → none (expected, struct offset).*

