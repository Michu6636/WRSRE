# Task 7 — Absolute CPI Survival Basket Investigation

> **Status:** investigation only. No plugin/header/source/INI modified, no
> builds run. Target `SOVIET64.exe` v1.1.1.9 (stamp `0x6A3EB6AD`), Ghidra
> file-base `0x140000000` (query = `0x140` + RVA).
>
> Driven by **`docs/WRSRE MASTER DESIGN DOC.md` Section 4** (Task 7 —
> Standard of Living, CPI Survival Basket & Wealth Strata) and the Task 7
> mission brief (replace relative $P_{25}/P_{75}$ wallet percentiles with
> the absolute daily survival basket).
>
> Raw evidence: `scratch/cpi_floats.txt`, `scratch/cpi_floats2.txt`,
> `scratch/cpi_str.txt`, `scratch/cpi_foodgen.c` (decompile of
> `FUN_1402A1DD0`), `scratch/cpi_meat_xrefs.txt`,
> `scratch/cpi_meat_site.asm`, `scratch/cpi_food_xrefs.txt`, plus the
> pre-existing SoL evidence
> (`scratch/citizen_sol_investigation.txt`, `scratch/sol_836b40.c`,
> `scratch/sol_171e10.c`, `docs/Citizen_SoL_Investigation.md`).
> Companion (rent & eviction toolbox): `docs/Rent_Housing_Task1_Investigation.md`.
> Companion (housing transfer primitive): `docs/Citizen_Housing_Relocation.md`.

## 0. What the brief asks vs. what the code does today

| Brief element | Current state in tree |
|---|---|
| Relative $P_{25}/P_{75}$ wallet percentiles are flawed (a rich republic still labels 25% "Low") | **Confirmed as built.** `plugins/citizen_retail/citizen_retail.cpp` `UpdateSolBenchmarks()` collects every wallet via `currency->forEachWallet`, sorts, reads `n*25/100` / `n*75/100`, EMA alpha 0.20, `n>=20` else USD-scaled INI defaults (`p25_default_rub 60`, `p75_default_rub 250`). `ApplySolScaling()` gates `wallet > g_p75` (High) / `wallet < g_p25` (Low); Mid is baseline. |
| Daily Basket Cost (CPI) = Pfood x Qfood + Pmeat x Qmeat + Pclothes x Qclothes + avg daily rent | **Not built.** No CPI computation exists anywhere in `citizen_retail` or `resource_pricing`. Prices are per-sale (`getPrice(res,cur) x getMarketMultiplier(res)`); rent lives in `enterprise_base` (`CollectResidentialRent`, monthly per-tenant). |
| Qfood=0.0010 t, Qmeat=0.0003-0.0006 t, Qclothes=0.0005 t | **CORRECTED — see S1/S2.** Tesmio's live probe (`11-needs.md` S435-444, verified in this build at RVA `0x83817A`) proves meat demand exists: `kind 2 meat 0.00061 of 0.00061`. Recommended CPI defaults: Qfood ~0.0020 t, Qmeat ~0.0003-0.0006 t, Qclothes ~0.0005 t. |
| Purchasing Power = Effective Daily Wage / CPI; Runway = Wallet / CPI | **Not built.** `citizen_retail` never reads a wage (Level 1 to Level 1 `citizen_labor` consume would violate the Level hierarchy; see S5). Wallet is read per-shopper (`getWalletBalance`) for the percentile gate only. |
| Affluent >= 2.0x AND >= 30 d; Middle >= 1.0x OR >= 14 d; Low < 1.0x AND < 7 d; Destitute < 1 day of bread, eviction target | **Thresholds exist only as the old percentile + multiplier behaviour.** Current mapping: High: meat x1.5, clothes x1.5, electronics x2.0; Low: meat x0.2, clothes x0.5, electronics x0.0, alcohol x1.8 when `person+0xE8 < 0.40`. No runway, no wage ratio, no destitute tier, no eviction linkage. |
| Pull P from `g_pricing->getPrice()` (x market multiplier); rent from `enterprise_base` / residential setting | **Half-ready.** `citizen_retail` already holds `g_pricing` (prefer-v2, v1 fallback) and applies `base x mult` per sale. It does **not** consume `enterprise_base` today (Level 1 to Level 2 is **forbidden** by `docs/DEVELOPMENT_README.md` S1). Rent must arrive via a new Level 1-safe channel — see S5. |

## 1. The engine's own daily consumption (what the CPI weights must reflect)

Demand generation is `FUN_140836B40` (RVA `0x836B40`), called from the
citizen kick `FUN_140830820` when `person+0x90` reaches the day length
`DAT_14090AA78 = 60.0`. Full decompile: `scratch/sol_836b40.c` (1881
lines). Fresh `entry+0x00` bases are **float immediates** (decoded
big-endian — the decompiler prints them as `0x3a83126f`-style words;
little-endian decode gives nonsense ~1e28):

- Food slot (`param_2+0x118`, `DAT_1408FEC58` res = `food`, lines 560-563,
  xref at file `0x140837BA2` = RVA `0x837BA2`, `cpi_food_xrefs.txt`):
  `0x3ac49ba6` / `0x3b102de0` / `0x3b449ba6` by `person+0xDC` bracket =
  `0x3ac49ba6` / `0x3b102de0` / `0x3b449ba6` by `person+0xDC` bracket =
  **0.0015 / 0.0022 / 0.0030 t**. Tesmio's live probe shows 0.00150 t
  (youngest bracket with family scaling) — consistent.
- Meat slot (`lVar26+0x118`, `DAT_1408FEC60` res = `meat`, lines 742-752):
  **CORRECTION — this block IS the meat demand.**
  `0x3a83126f` / `0x3ac49ba6` / `0x3b03126f` = **0.0010 / 0.0015 / 0.0020 t
  base**, then scaled by `fVar31` (line 797: `fVar30 = fVar31 x base`) and
  the home-chain terms (lines 768-793), plus the `1/3` chance gate
  (line 716-720: `rand() % 3 == 0`).
  Verified in v1.1.1.9: `Xrefs 0x1408FEC60` lands at file `0x14083817A`
  (`LEA RDX,[0x1408FEC60]` + `CALL 0x1402AA830`, `cpi_meat_xrefs.txt`),
  i.e. RVA **`0x83817A`** — Tesmio's documented `0x837F1B` shifted by
  `+0x25F` across builds (cf. `07-pitfalls.md`: code moved `+0x70`/`+0xA0`/
  `+0x1E0` between 1.1.1.7 and 1.1.1.9; `.data`/`.rdata` did not move).
  Tesmio's live probe (`needs.cpp:1105` format, `11-needs.md:440`):
  `[1] kind 2 meat 0.00061 of 0.00061 target 1/1` — the scaled result of a
  0.0010 t base, exactly as the code predicts. The happiness ledger
  (`16-easystart.md:44`): reason #1 *Lack of meat* raised by `0x83A6D0`
  comparing demand res against `game+0xC310`.
  **CPI consequence: Qmeat 0.0003-0.0006 t brackets the observed 0.00061 t.**
  `citizen_retail`'s meat x1.5/x0.2 scaling is **functional**, not dead code.
- Clothes (`clothes`, line 937-946): `0x3a09a027` / `0x3a49d9d3` /
  `0x3a89a027` by `person+0xF8` bracket = **0.000525 / 0.00077 / 0.00105 t**.
  Design Qclothes=0.0001 is **5-10x too small**. Open item: confirm demand
  **frequency** (daily kick vs multi-day cycle) before locking Qclothes.
- Electronics (`eletronics` — engine spelling, line 1210):
  `fVar40 x 0.0002 + fVar45 x 0.0007`, year-clamped
  (`(year-1935..1940..1955..1960)/40`, clamp 0..1) = **~0.0002-0.0009 t
  per firing**. A **durables lottery**, not a daily need: pre-1935 no
  demand; 1935-1940 scaling; post-1960 full rate. Excluding it from the
  survival basket matches the engine.

Key decoded DAT constants (`scratch/cpi_floats.txt`, file-based reads
via `PrintFloats.java`):

- `0x140909AFC = 0.0010`, `0x140909B18 = 0.0015`, `0x140909B24 = 0.0020`,
  `0x140909B28 = 0.0022`, `0x140909B34 = 0.0030` — the food/drink ladder.
- `0x140909AE0 = 0.000525`, `0x140909AF4 = 0.00077`,
  `0x3a89a027 = 0.00105` — the clothes ladder.
- `0x140909AAC = 0.0002`, `0x140909AEC = 0.0007` — electronics year-scale
  pair; year bounds `0x14090B0D0/D4/DC/E0 = 1935/1940/1955/1960`,
  divisor `0x14090A9F0 = 40.0`.
- `0x14090AA78 = 60.0` (day length), `0x14090A9A0 = 30.0`,
  `0x14090A828 = 10.0`, `0x14090A8CC = 15.0`, `0x14090A934 = 21.0`.
- `0x140909DDC = 0.5`, `0x140909F58 = 1.0`, `0x14090A280 = 2.0`,
  `0x140909D88 = 0.40` (also the escapism threshold value),
  `0x140909D64 = 0.35`, `0x140909ED4 = 0.85`.

`0x1408FEC58` / `0x1408FEC60` read back as ASCII `food`/`meat` fragments
(`0x646F6F66` = `food` LE, `0x7461656D` = `meat` LE) — inline string bytes
at those addresses, i.e. the two staple resource names. `StrAddr.java`
(`scratch/cpi_str.txt`) confirms `food`, `meat`, `clothes`, `eletronics`
(engine spelling), `alcohol` are all real resource records resolved through
`FUN_1402AA830`, and the game object caches them at `game+0xC300` (food),
`+0xC308` (alcohol), `+0xC310` (meat), `+0xC318` (clothes), `+0xC320`
(eletronics) — wired in `FUN_1402A1DD0` (`scratch/cpi_foodgen.c:2919-2928`),
matching `src/game_world.h` `TSM_GO_RES_*`.

Checkout (`FUN_140171E10` family, `scratch/sol_171e10.c`) decrements
**demand amount AND storage slot together** in one branch and derives the
fee from the fulfilled delta — so scaling `entry+0x00` scales goods taken
and money charged linearly. The current once-per-day `ApplySolScaling`
hook point is therefore CPI-compatible: swapping the percentile gate for
a CPI gate changes *who* scales, not *how* scaling propagates.

## 2. Meat leg: CONFIRMED — keep it (correction of v1 error)

The v1 investigation wrongly claimed no meat demand exists because the
analyst searched for the ASCII string `meat` instead of recognising the
packed LE constant `0x7461656D` at `0x1408FEC60` — the very bytes the
analyst's own float dump had already decoded. Tesmio's evidence is
definitive and now verified in v1.1.1.9 (stamp `0x6A3EB6AD`):

- Live probe (`needs.cpp:1094-1109` format, `11-needs.md:438-443`):
  `[1] kind 2 meat 0.00061 of 0.00061 target 1/1`.
- Minting site in this build: RVA **`0x83817A`**
  (`LEA RDX,[0x1408FEC60]` + `CALL FUN_1402AA830`,
  `scratch/cpi_meat_xrefs.txt` + `scratch/cpi_meat_site.asm:27-29`).
  Tesmio's `0x837F1B` is the same site in an older build
  (`+0x25F` code shift; `.data`/`.rdata` unmoved per `07-pitfalls.md`).
- Amount path (this build): base `0x3a83126f` (0.0010 t) selected for the
  youngest `person+0xDC` bracket (`cpi_meat_site.asm:32`), scaled by
  `fVar31` + home-chain (`sol_836b40.c:768-798`), chance-gated
  `rand()%3==0` (`sol_836b40.c:716-720`) — 0.0010 t scaled to 0.00061 t
  matches the probe exactly.
- Happiness ledger (`16-easystart.md:41-44`): reason #1 *Lack of meat*
  raised by `0x83A6D0` vs `game+0xC310`; also a health-ledger reason.
  Record cache (`02-findings.md`): `game+0xC300` food, **`+0xC310` meat**,
  `+0xC318` clothes, `+0xC320` eletronics.

**Build-task consequences:** keep the meat leg in
`getCpiDailyBasket` with Qmeat default **0.0006 t** (the observed probe
value; INI-tunable 0.0003-0.0006). Keep the existing meat x1.5/x0.2
scaling — it is functional. Qfood default **0.0020 t** (midpoint of the
0.0015-0.0030 food ladder, near the 0.00150 probe), Qclothes default
**0.0005 t** (near the 0.000525 youngest bracket and the 0.00024 probe
after family scaling).

Why the v1 error matters for methodology: a packed 4-byte literal never
appears in a naive string search for `meat` — the xref of the *address
holding the bytes* (`Xrefs.java 0x1408FEC60`) is the correct primitive,
and the disassembly (`LEA RDX,[addr]` + resolver call) is the confirmation.
The v1 file's S1-table "no separate demand line" claim and S2 options
(b)/(c) are superseded by this section.

## 3. Price legs: what `resource_pricing` can already supply

- `getPrice(res, currency)` lookup order: engine live `+0x58/+0x5C` (when
  finite and > 0.01) to `[fallback_prices]` to `default` (500.0). Never
  returns <= 0/NaN/Inf; NULL record returns the default. Shipped
  fallbacks: `food 350`, `meat 700`, `clothes 1400`, `eletronics 3800`,
  `alcohol 600` (RUB/ton).
- `getFood/getMeat/getClothes/getEletronics` are the v1 prefix every
  provider carries — `citizen_retail` already resolves them with null
  function-pointer guards and no version check needed.
- `TsmPricingMarketMult(api, res)` returns the live cover multiplier
  (clamped [0.35, 2.0], gain 0.75, monthly rebalance) or neutral 1.0 on a
  v1 provider. The brief's "multiplied by market multiplier if available"
  is exactly this helper — no new API needed.
- All four price legs must be read in the **active currency**
  (`TsmCurrencyActive`): live engine fields are natively dual (`+0x58`
  RUB / `+0x5C` USD); fallback RUB values divide by `usd_divisor` (50.0)
  unless `[fallback_prices_usd]` overrides exist. CPI must do likewise or
  the USD-mode basket will be 50x off.

## 4. Rent leg: where "average daily rent" actually lives

- `enterprise_base` `CollectResidentialRent` charges **monthly**
  per-tenant rent: `tenantRent = g_residentialRent x mult x rentFactor`
  where `g_residentialRent = 10.0` RUB/resident/month (INI
  `residential_rent`), `mult = min + (max-min) x attr [x parkCap]`
  (`rent_attr_min_mult 0.50`, `rent_attr_max_mult 2.00`,
  `PARKCAP_FLAT_REF 30.0`), `rentFactor = 0.75 + 0.5 x rentLevel`
  (`bld+0x13E0`, neutral 1.0 at default 0.5).
- The brief's "Average Daily Rent" is therefore
  `monthlyTenantRent / daysPerMonth`. `sim_clock` exposes
  `getDaysPerMonth` (guarded by `structSize`). No plugin currently
  computes this quotient. Recommendation: mean actually-charged tenant
  rent per occupied private bed per day, maintained as a monthly EMA
  beside the CPI — because attractiveness and rent-slider dispersion make
  the INI base unrepresentative (slum 5 x 0.75 = 3.75 vs luxury
  20 x 1.0 = 20.0 per month).
- Eviction today (`enterprise_base` v8.2): burden test
  `tenantRent > 0.35 x wallet` OR `taken < 95%` of rent, then stage,
  then `FUN_14083CA50` relocation (never under `g_lock`), homelessness
  backstop (abort if no vacant flat; arrears + `-0.05`/month happiness).
  The brief's "Destitute to automatic eviction target upon rent
  collection" plugs into this pass as an **additional stage condition**
  (`wallet < 1 day of bread`), not a replacement.

## 5. Wage leg: the layering constraint that shapes the design

- `citizen_labor` owns the effective daily wage hierarchy
  (override to sector to legacy type to global default, clamped to the
  statutory minimum): `default_daily_wage 3.0`, `minimum_wage 2.50`
  (`minimum_wage_usd 0.50`), sector bases heavy 3.80 down to agri 2.60
  (explicit USD twins 0.76 down to 0.52), transfers as ratios of the minimum
  (welfare 0.40x, stipend 0.60x, pension 0.80x = 1.00/1.50/2.00 per day),
  monthly payday with shift scaling. Accessors: `getMinimumWage`,
  `getSectorWage`, `getEffectiveWageForBuilding` (`TsmLaborHasV3` guard),
  `setSectorWage` (`TsmLaborHasV4`).
- **Constraint:** `citizen_retail` is Level 1 and `citizen_labor` is
  Level 1 — `docs/DEVELOPMENT_README.md` S1 forbids Level 1 to Level 1
  consumption except through decoupled callbacks/interceptors, and
  `enterprise_base` is Level 2, so Level 1 to Level 2 consume is outright
  forbidden. `citizen_retail` therefore **cannot** `consume()` either
  service for the CPI inputs.
- Consequences for the build task (design only, no code here):
  1. **Wage flow** must arrive without a Level 1-1 consume: either (i) a
     new `citizen_labor` callback/interceptor the retail plugin registers
     for (mirroring the payroll/retail interceptor precedent), or (ii) a
     Level 0-mediated share (a daily-wage cache published through a Level 0
     service keeps the hierarchy clean), or (iii) derivation from wallet
     flow itself (payday credits observed via `currency` flow observers —
     no new dependency at all, but needs a payday-cycle accumulator).
     Unemployed / student / retiree citizens have no workplace wage — the
     "Effective Daily Wage (or Social Transfer)" parenthetical in the Master
     Doc means the flow source must branch on age/employment exactly as
     `GetWageForTypeImpl` does.
  2. **Rent average** must arrive without a Level 1-2 consume: either (i)
     `enterprise_base` pushes a monthly average into a Level 0-visible slot
     / persistence record `citizen_retail` already reads, or (ii) a new tiny
     Level 1 rent-reader service both plugins feed, or (iii) the CPI
     derives daily rent from the INI base + attractiveness-neutral
     assumption (zero new plumbing, but loses the lux/slum spread of S4).
  3. **Persistence + INI**: CPI state (basket cost, tier counts, poverty
     rate) needs a `persistence` handler entry and Task 7 INI section
     (`[cpi_basket]` quantities, `[sol_tiers]` thresholds) following the
     existing `// vN.N updates:` to `info->version` to live-`Logf` to build
     to read-back convention. INI files stay UTF-8 without BOM.

## 6. Worked CPI sketch (shipped fallbacks, RUB) — CORRECTED quantities

With fallback prices and the **corrected** defaults
(Qfood 0.0020, Qmeat 0.0006, Qclothes 0.0005):

- Food: 350 x 0.0020 = 0.70 RUB/day.
- Meat: 700 x 0.0006 = 0.42 RUB/day.
- Clothes: 1400 x 0.0005 = 0.70 RUB/day.
- Rent (base, attractiveness-neutral ~1.0x, 30-day month):
  10.0 / 30 = 0.33 RUB/day.
- **CPI ~2.15 RUB/day** (slum-to-luxury spread roughly 1.9-3.3 with live
  multipliers).

Sanity vs wages: `default_daily_wage 3.0 / 2.15 = ~1.4x` (Middle on flow —
runway decides Affluent); `minimum_wage 2.50 / 2.15 = ~1.2x`;
welfare `1.00 / 2.15 = ~0.47x` (Low on flow — needs >= 14 d runway for
Middle); pension `2.00 / 2.15 = ~0.93x` (borderline). This is the intended
Task 7 behaviour: the corrected basket actually bites, unlike the v1
1.03 sketch where even minimum wage looked Affluent.

Runway tiers (per the follow-up direction — wallet/CPI only, no wage
ratio): Affluent >= 30 d, Middle >= 14 d, Low < 7 d,
Destitute < 1 d of bread.

Bread-day check (Destitute): 1 day of bread = Pfood x Qfood
(350 x 0.0020 = 0.70 RUB at fallback). Any wallet below that stages
eviction review in the rent pass.

## 7. Ghidra gaps and open items for the build task

- `DAT_1408FEC58/60` full-string recovery (only the LE `food`/`meat`
  fragments were read; the complete resource-name bytes were not dumped).
  NOTE: `0x1408FEC60` is now confirmed as the meat resolver argument
  (xref at RVA `0x83817A`) — remaining work is only the full byte run.
- Tesmio version caveat (your note): Tesmio's probe values (0.00150 food,
  0.00061 meat, 0.00024 clothes, 0.00012 eletronics) predate v1.1.1.9 and
  the code has shifted (`+0x25F` at the meat site; `+0x70`/`+0xA0`/`+0x1E0`
  per `07-pitfalls.md`). Treat probe values as order-of-magnitude anchors,
  not exact constants: food/clothes probe values sit BELOW the youngest
  immediates (family scaling + chance gates explain the gap), meat 0.00061
  matches a scaled 0.0010 base. A fresh live-probe session on v1.1.1.9
  (via Tesmio's `needs` `probe = 1` path) would close this before locking Qs.
- Demand **frequency**: is `FUN_140836B40` strictly one firing per citizen
  per day (kick on `+0x90 >= 60.0`), or can hunger/age/health multipliers
  re-fire within a day? Sets whether S1 bases ARE daily quantities or
  per-visit portions of a multi-visit day.
- `person+0xDC` / `+0xF8` bracket semantics (age bands vs health bands)
  and the `FUN_140836960` / `FUN_140836A90` multiplier functions — needed
  to pick representative vs bracketed Q values.
- Panel readers of `+0x734/+0x738` (not swept) before reusing either field
  for SoL bookkeeping; the poverty-rate readout (Information Layer, Master
  Doc S5) needs a home (Accounting Office / City Hall rows via `ui_panels`,
  plus `state_ledger` observers) that is currently unassigned.
- No hook-safety analysis for any new intercept point; the known
  decompiler-budget exclusions (`FUN_14028C770`, `FUN_14030D1A0` need
  `DisasmMany`, not `DecompAt`) still apply.
- `ListFuncs` coverage gaps are real: docs-listed RVAs can answer "No
  function found containing address" on this project snapshot — fall back
  to docs + game INIs + sibling code per the workspace rules rather than
  retrying the same address.

## 8. Recommended build slices (for the build task, not this file)

1. `resource_pricing`: add pure-query `getCpiDailyBasket` (basket cost from
   live prices x market mults + injected daily-rent average, Qfood/Qmeat/
   Qclothes INI-tunable at the corrected defaults) so the math lives
   beside the prices and stays Level 1-safe for retail to consume.
2. `citizen_retail`: replace the `UpdateSolBenchmarks` percentile core with
   the CPI gate — **Wallet Runway (wallet/CPI) only** per the follow-up
   direction (Affluent >= 30 d, Middle >= 14 d, Low < 7 d,
   Destitute < 1 d of bread); **remove the P25/P75 sort entirely**. Keep the
   once-per-day `ApplySolScaling` write path and all four multipliers +
   escapism gate, add the Destitute stage flag for the rent pass.
3. Wage/rent feeders: resolve the S5 channels first (the only
   architectural risk); everything else is local arithmetic. NOTE: with the
   runway-only classification the wage leg is informational (poverty-rate
   diagnostics), not gating — which downgrades the Level 1-1 wage channel
   from blocker to nice-to-have. The rent average remains required (it is
   inside the CPI divisor).
4. Information layer: true CPI + true Poverty Rate on the City Hall panel
   (`taxes.cpp` wiring per the follow-up direction, `ui_panels` rows,
   `state_ledger` observers) so the 0%-poverty goal is legible in game.

