# Superseded Conclusions Index

Updated 2026-11-22. Table of earlier findings now replaced.

| Earlier conclusion | Latest conclusion to use | Source |
|---|---|---|
| Task serialization could not be located. | Save/load reconstruction in `FUN_1401F0BE0`; config serializer/deserializer `FUN_1406C5350`/`FUN_1406C6040`, builder `FUN_1401EDC40`, helper `FUN_1401FEF00`, ctor `FUN_1401512A0`. | `docs/ghidra-do-storage-findings.md` |
| Type 3 = rail | Type 3 = road, Type 6 = rail-specific. Rail DO can use road connection. | `ghidra-do-storage-findings.md` |
| `FUN_1407496C0` callable add API | Per-frame DO UI handler, not safe to call from day callback; reproduce allocation/vector path (`0x18` Entry + `0x198` TaskCfg via `0x08E050`, `+0xD50=1`/`+0xFC0=3.0`). | `ghidra-do-storage-findings.md` |
| DO vector stride 8 pointer vector | Vector stride `0x18` Entry `{+0x00 byte enabled 0=off, +0x08 target*, +0x10 TaskCfg*}`, TaskCfg `0x198`. | `ghidra-do-storage-findings.md` |
| `TSM_GO_BUILDINGS_VEC = 0xC2C8` | `0x11B08/0x11B10` per `src/game_world.h`. | `DEVELOPMENT_README` Rev4 |
| Task 6 Dropped | Task 6b Bounded Source-Only Staged v0.17, Task 6 broad fleet remains dropped. | `WRSRE DESIGN DOC` Rev4, `current-task-state-storage.md` |
| Panel safe above 5 | `panel+0x310+idx*4` allocation unproven above 5; conservative cap 5 policy, not engine max. | `ghidra-do-storage-findings.md` / `NEXT_PHASE.md` |
| Native add_flow 5 = engine cap | 5 observed in one add session, not proven global; vector/planner/save iterate all. | `ghidra-do-storage-findings.md` |

| Local town markets / scarcity pricing is the next priority (Task 2). | **Cancelled 2026-11-22.** There is one domestic market at native prices. | `visions/economy-direction-2026-11-22.md` D3 |
| Prices = native price × Republic market multiplier (supply/demand book). | **Native price only.** Code removed 2026-11-22: `resource_pricing` v5 has no market book, multiplier or local scarcity quote. | economy direction D2, §3; [task record](tasks/active/2026-11-22-remove-market-multiplier-and-procurement.md) |
| Producer paid 90 % of retail on dispatch; shops procure at 70 %. | **Withdrawn.** The buyer pays the native price on delivery; companies receive their own customs export proceeds. The 90 % dispatch procurement was removed from code on 2026-11-22 (`private_shops` v3.6); the 70 % delivery bill (`wholesale_ratio`) is still in code until a later step. | economy direction D4/D5, §4 |
| B2B payments remain OFF / dry run is the safe default. | **Live billing by default** (`billing = 1`, `dry_run = 0`), after the §4.4 prerequisites. | economy direction D16 |
| City Hall TAX tile and fiscal panel are the fiscal policy UI. | **Obsolete.** The Almanac is the only policy surface. | economy direction D7 |
| Missing service: hide the UI row, or show synthetic/sample values. | **Never.** Show `UNAVAILABLE` / `ERROR <reason>` visibly; no invented data. | economy direction D8 |
| A resource the engine prices at 0 gets `[fallback_prices]`, `default`, `default_usd` or `usd_divisor`. | **UNPRICED.** `resource_pricing` v6 answers `0.0f`; a shop gives the good for free as vanilla does and logs `ERROR UNPRICED`; a business bill is refused whole. | economy direction D8, D32, D33, step 2 |
| Missing CPI falls back to a 2.15 RUB/day basket. | **UNAVAILABLE.** No default basket; demand scaling is skipped that day, the runway/CPI readouts show UNAVAILABLE and the bank refuses new credit. | economy direction D34, step 2 |
| The Almanac shows synthetic sample rows until live services connect. | **NO DATA.** Each absent service shows `NO DATA: <service> unavailable` in warning colour; no sample value may appear. | economy direction D8, §11 item 9 |
| Skill or education wage premium. | **Withdrawn.** Only a premium on high-education *slots* in two-tier buildings. | economy direction D15, §6.2 |
| USD mode divides RUB amounts by `usd_divisor` (50) or uses `*_usd` overrides. | **No exchange rate.** Resources use the native USD price; every other amount is a ratio of native-priced bases. | economy direction D21, §3.2 |
| Investment fund `starting_pool`, per-building `starting_capital` and citizen `starting_wallet` are credited from nothing. | **No money from nothing.** Fund founding capital once per map only; privatisation pays the building price to the state and the starting capital from the fund; wallets start at 0; the fund is fed by company profit above the reserve and rich citizens' surplus savings. | economy direction D20, D24–D26, §4.5 |
| Un-privatising keeps the company's capital parked on the building; one flat `starting_capital` for every building. | **Nationalisation is paid by the treasury**, capital returns to the fund, a sim-clock cooldown locks the toggle; starting capital is computed per building from its staff and inputs. | economy direction D27–D29, §4.5.6–4.5.7 |
| Mod-side relocation search (`FindRelocationCandidate`) and rent evictions in private housing only. | **Native flat search triggered for all established households**, up and down, without an eviction penalty. | economy direction D23, §8 |
| Separate welfare, pension and stipend ratio keys. | **One Welfare decree** with four tiers (none/low/medium/high). | economy direction D18, §6.4 |

See `docs/ghidra-do-storage-findings.md#superseded-conclusions` for full narrative.
