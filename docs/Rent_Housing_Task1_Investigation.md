# Rent & Housing — Task 1 Implementation Investigation

> **Status:** investigation only. No plugin/header/source/INI modified, no
> builds run. Target `SOVIET64.exe` v1.1.1.9 (stamp `0x6A3EB6AD`), Ghidra
> file-base `0x140000000` (query = `0x140` + RVA).
>
> Driven by **`docs/WRSRE MASTER DESIGN DOC.md` Section 2** (Task 1 — Urban
> Land Value, Rent & Economic Relocation) and its three "Further
> Investigation Needed" asks.
>
> Raw evidence: `scratch/rent_housing_investigation.txt` plus
> `scratch/rent_13e0.txt`, `rent_80c.txt`, `rent_11b0.txt`,
> `rent_field.c`, `rent_ui.c`, `rent_kick.c`, `rent_attr.c`,
> `rent_tick.asm`, `rent_tick2.asm`, `rent_floats.txt`, `rent_xrefs.txt`.
> Companion (housing transfer primitives): `docs/Citizen_Housing_Relocation.md`.

## 0. Requirements vs. current state

| Master doc Task 1 element | Status |
|---|---|
| 2.1 Rent = BaseRent × (MinMult + (MaxMult−MinMult) × Attractiveness) | **Built** in `enterprise_base` v8.0 (`CollectResidentialRent`, `chargeCitizenPartial` → company capital) |
| "Park Exploitation Cap" via `typedesc+0x80C` | **Not built**; and the field is mislabeled — see §4 |
| 2.2 Eviction trigger: rent > 35% of income/savings | **Not built** — no engine eviction exists outside dorms (§1) |
| 2.2 Relocation via `FUN_14083CA50` | Primitive fully mapped (prior investigation); unused by the plugin |
| 2.2 Demographic sorting (poor→slums, wealthy→luxury) | **Not built** |
| Ask 1: flat-search scoring in `FUN_140834DE0:362-501` | **Answered** (§2) |
| Ask 2: eviction unhappiness penalty | **Answered: none exists natively** (§2) |
| Ask 3: homelessness edge case | **Answered: emigration despawn** (§2) |

## 1. What the engine already provides (the Task-1 toolbox)

**`bld+0x13E0` — the native rent level (0..1 fraction).** Per-building,
player-adjustable in the building panel (`FUN_14073E950`: `GetString(0x995)`
slider, `"%ls > %d %%"`, writes `bld+0x13E0 = slider/100` via the shared
slider primitive `FUN_1402727F0`), refreshed each building tick by
`FUN_1401F0BE0` from a per-type config block (stack `[RBP+0x1550]`, slot
`+0xA0` → `+0x15F0`), and save-serialized (`FUN_1401EDC40`). The housing
stat cluster `bld+0x13A0..0x1417` holds `+0x13D8/+0x13DC/+0x13E0` (rent
family), `+0x13F8` counter, `+0x140C` independent-tenant count, `+0x1410`
settled-tenant count.

**The native affordability test (dorms only)** — `FUN_140834DE0`
(`house_trans.c:567-663`), for homes with `typedesc+0x364 == 1`:

```c
fVar28 = bld+0x13E0;                 // rent level
fVar31 = person+0xE4 + bonus;        // living standard
if ((2.0 <= person+0xA8) || (fVar31 < fVar28))   // adult OR cannot pay
    -> town-wide search for type-2 subtype!=1 -> FUN_14083ca50(person, best)
else person+0xD8 -= 0.05 (afford) / 0.35 (strain)
```

**`person+0xE4` — the living-standard/wealth float (0..1).** Continuously
re-derived by `FUN_14083A6D0`: grows toward a city-policy target
(`game+0x9D54D8` selector → 0.085/0.08/0.075 base × person+0x14) at rate
0.0145 scaled by city stats `(city+0x1708)+0x8678/35 + +0x8624/95 + +0x867C`
squared; unmet needs (fulfilment ratio ≤ 0.40) divert to the carry list.
This is the engine's own income proxy — the 35% test can read it or the
currency wallet.

**`FUN_1401BCE70` — native per-resident upkeep.** Called from the
per-citizen frame loop `FUN_140832E90` (only caller) for every settled
resident: `amt = dt×0.0004×person+0xFC×mult + dt×0.0005×mult` (mult 0.15,
0.20/0.08 by work-timer bands 9..51, halved when settled), debited from the
funds pointer `bld+0x10E0`, credited to `bld+0x10F0`, accumulated in
`bld+0x112C`, shortfall timers `bld+0x110C/+0x1104`. `person+0xFC` is
confirmed as the income/savings scalar.

**`bld+0x11B0` — attractiveness (0..1).** Written daily by `FUN_1404D5F50`
(smoothing, clamp 1.0; called once from `FUN_140334780`, the daily updater
documented in `Map_Size_Investigation.md`), zeroed at placement. Consumed
in the housing cluster by `FUN_14083A6D0` and `FUN_1408243E0` — the latter
mitigates a burden/health score (`person+0xD8×25 + 35 + person+0xE0×30`) by
`(1−attr)×rate + bonus` (rate by `game+0x5C8`: 0.15/0.25/0.35).

**Searching/settled flags.** `person+0x2C = 1` = *searching for home*
(set by the kick `FUN_140830820` with a random 0..10 timer at `person+0x28`);
`person+0x2D` = *settled* (set when the timer expires in `FUN_140832E90`,
counted into `bld+0x1410`). There is no dedicated "homeless" field —
## 2. The three investigation asks — answered

**Ask 1 — flat-search scoring (`FUN_140834DE0:362-501`).**
The engine's scorer is occupancy + tenure + rent gate + neighbour bonus,
*not* attractiveness:

- candidate gates: `built (≥1)`, `!demolished (+0xEA8)`, `type == 2`,
  subtype rules, `bld+0x13E0 <= person+0xE4` (rent gate), `FUN_14083c590`
  policy gate;
- count: residents with `person+0xC8 == 0` (independent) vs capacity
  `*(bld+0x970)+0x8C` (storage0 = `$STORAGE RESOURCE_TRANSPORT_PASSANGER N`
  — `civ_dedina1a.ini` declares 3);
- score (line 490): occupancy ratio weighted; **neighbour bonus** =
  `DAT_140909C78` (0.10) per walking-connection (`bld+0xCA8`, stride 0xF0)
  whose typedesc+0x810 > 0 (lines 617-632);
- first-better-wins (`>`), no RNG at pick time, no distance term.

For demographic sorting the mod reuses the same filters: poor = low
`bld+0x13E0` / low typedesc tier; wealthy = high `bld+0x11B0` attractiveness.
Attractiveness enters the *flat choice* nowhere — only through
`FUN_1408243E0`'s burden mitigation.

**Ask 2 — eviction unhappiness penalty.** **None exists.** The kick
(`FUN_140830820`), the move executor (`FUN_14083CA50`) and
`FUN_14082F890` never write `person+0xE4/+0xE8/+0xE0`. The only native
"penalty" writes live in `FUN_14082fa50` (PRISON: `person+0xE4 +=
random(0..0.5)×(200..2000) + offset`, clamped `[0,1]`) — that is prison
unhappiness, not eviction. The mod applies its own penalty (multiply
`person+0xE4` by a factor ≤1, or nudge `person+0xE8..0xF4` happiness).

**Ask 3 — homelessness edge case.** **Emigration.** A homeless resident who
is kicked (`person+0x20 == 0`) is destroyed by `FUN_140835D70`
(reason = 1 if resident-and-unspecial else 0): `free(person)`,
erase from the four citizen vectors `game+0x126C0/126D8/126F0/12708`,
statistics `game+0x11574/0x11578` and town counters. Homelessness is
transient/deadly in vanilla. **An eviction mod must always guarantee a
candidate home before calling `FUN_14083CA50`**, or it will delete its own
evictees.

## 3. What Task 1 needs — implementation roadmap

**A. The eviction path (the missing 2.2 core).** On the day boundary, for
each settled resident of an affordability-gated `TYPEDESC` (private
`[P]`-tagged residential, `g_companies.isPrivate` registry):

```c
// 35% income/savings test (mod-side)
if (income /*wallet balance via currency API or person+0xFC*/
    < 0.35 * monthlyRent /*g_residentialRent * (min+(max-min)*attr)*/) {
    // 1) find candidate home (population-sorted reuse of the engine scorer)
    bld* best = findCandidate(person, poor ? LOW_RENT_TIER : HIGH_ATTRICTUDE);
    if (best) FUN_14083ca50(game, person, best);       // 2) move
    else { evictFails: ... }                            // 3) else: accept
}
```

Composing the candidate search from proven engine pieces:
- **Scoring:** reuse `FUN_140834DE0:362-501` (occupancy, tenure `bld+0x1410`,
  rent gate, neighbour bonus) — already the engine's proven model;
- **Filtering:** `bld+0x13E0` rent level (player slider, 0..1),
  `typedesc+0x360/0x364` tier, `bld+0x11B0` attractiveness;
- **Call:** `FUN_14083CA50` is the single safe executor (parent unlinked at
  21, family follows, `bld+0x140C/+0x1410` recomputed, old routes cleared).

**B. The monthly rent charge.** Existing `CollectResidentialRent`
(674-744) already computes and charges. Extend to the `0.35×income` test
before charging: skip charges for unaffordable tenants so they can be
evicted instead; route overdue/unaffordable charges into the same eviction
pass so the state never accumulates uncollectable debt.

**C. Land value as policy lever.** `bld+0x11B0` is updated daily by
`FUN_1404D5F50` (called from `FUN_140334780`, samples pollution grid
`game+0x120A8`). The mod can bias the daily update or clamp `bld+0x11B0`
per building (a cheap "amenity/zoning" lever), and every `enterprise_base`
panel row already shows `Attr XX%` via `TsmAttractiveness()`.

**D. The dwelling-quality "park exploitation cap" (2.1).** `typedesc+0x80C`
is actually the **generic per-type capacity int**, not quality (proven:
housing cluster never reads it; workers/prisoners type 0x5C/0x11 use it as a
divisor). Cap the attractiveness multiplier by the **dwelling count**
`*(bld+0x970)+0x8C` (from the INI `RESOURCE_TRANSPORT_PASSANGER` storage)
or by a per-type quality table read from the typedesc. Optionally scale the
charged rent by the same cap when a state flat's occupant count approaches
capacity — this is also the "slum" discriminator (near-capacity low-tier
homes).

**E. Eviction penalty.** Apply to `person+0xE4` (living standard) or
`person+0xE8..0xF4` happiness for ≤30 days post-eviction (repayment grace),
making evicted tenants selectively buy cheap low-rent state flats rather
than defaulting to homelessness. Note: no native penalty exists (§2) — the
mod owns this.

**F. Escaping homelessness.** Since an evicted citizen with zero vacant
flats is deleted, the eviction pass must be pre-validated: find a candidate
for every evicted citizen, OR grant a short grace period (rent waiver /
## 4. Correcting the master design doc

1. **§2.1's `"physical dwelling quality (typedesc + 0x80C)"` is wrong** — that
   field is the **per-type capacity int** (proof: the housing cluster never
   reads it; type 0x5C divides worker production by it, type 0x11 shelters
   use it as worker capacity, `FUN_14072D120` city-hall reads it). Use
   `*(bld+0x970)+0x8C` (the storage capacity = flats from the INI) or a real
   quality table for the park-exploitation cap.
2. **§2.2's eviction trigger:** the engine's native affordability gate is
   `person+0xE4 vs bld+0x13E0` (dorms only). The 35% income test is a
   mod-side policy — no engine constant equals 0.35 (the value 0.35 lives
   in `DAT_140909D64`, the dorm *unaffordable decay* rate, not a threshold).
3. **§8.1's demand/scaling note:** `person+0xE4` (living standard) and
   `person+0xFC` (income) are the fields the demand/living-standard system
   actually maintains; `bld+0x13E0` is the rent-level slider — these are the
   natural places for rent-linked mechanics.

## 5. Open items

- `[RBP+0x15F0]` rent-config producer inside `FUN_1401F0BE0`'s timed-out
  middle; DisasmMany 0x1401F0C80:0x1401F0D80 (block-fetch CALL) plus the
  type-config table layout.
- `GetString(0x995)` (rent-slider label) — localization table, not in the
  exe's ASCII strings; runtime check.
- `bld+0x10E0/0x10E8/0x10F0` fund/rate/spent pointers: caller/owner
  (company vs treasury) — FindDisp 10e0 + placement path.
- `FUN_140334780` timing (daily vs twice-daily; whether it also
  re-evaluates `bld+0x13E0`).
- `person+0x80/+0x88` secondary-home links (kick removes via 822FD0 flag=1);
  possibly parents' buildings — runtime verify.
- Eviction-pass bookkeeping: migration-leaderboard fields already written by
  `FUN_140835D70` (`game+0x11574/0x11578`) and town counters
  (`+0x108→+0x1B8+0x24C/0x250`) — the mod reads them like any stats flow.



