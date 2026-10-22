# Citizen Standard-of-Living Investigation

Target `SOVIET64.exe` v1.1.1.9, base `0x140000000`. File-based addrs.
Investigation only: no source/INI/build changes. Raw evidence:
`scratch/citizen_sol_investigation.txt` plus `scratch/sol_*.c`,
`scratch/sol_finddisp_*.txt`, `scratch/sol_funcs_*.txt`.

## 0. Mission-brief correction (read first)

The brief's citizen/demand layout is stale. Ghidra ground truth
(`FUN_140836B40`, sol_836b40.c) + `src/game_world.h`:

| Item | Brief | Engine truth |
|---|---|---|
| person size | `0x738` | `0x750` |
| demand count | `+0x0E0` | `+0x110` int |
| demands | `+0x0E8` vector | `+0x118` INLINE array, 7 x `0x80` |
| entry amount/kind/res | `+0x00/+0x08/+0x10` | same, base `person+0x118+n*0x80` |
| money spent | `+0x734` | `+0x734` float confirmed |

`person+0x0E8` is a happiness float, not a pointer. `bld+0xC38` is the
idle counter; residents `bld+0xBC0..0xBC8`, customers `bld+0xBD8..0xBE0`.
`game+0x126A8` citizen vector confirmed (120 hits; walkers incl.
overview `FUN_1402FD2E0`).

## 1. Demand generation: FUN_140836B40 (RVA 0x836B40)

1875-line decompile (sol_836b40.c), `(game, person)`. Called from kick
`FUN_140830820` when `person+0x90` hits day length 60.0 and `+0x30==0`.
Order: flush negatives to `person+0x4f0` carry list (cap 10); home/type
gate (`0x4C` prison, `0x4A` special); append entries at `n=count++`
writing `+0x118/+0x120/+0x128`: food 15/30 (kind 15->10), drink 15/45
(kind 15->1), clothes via `FUN_1402aa830(game,"clothes")` kind 15->2,
electronics via `"eletronics"` (engine spelling) kind 2, then lottery
culture-5 / sport-6 / religion-4. Every amount =
base[age/health] * fVar39 * home-chain(res+0xd4/fVar10) + carryover.
Multipliers: `FUN_140836960`, `FUN_140836A90`, home-chain terms. SoL
hook: scale fresh `entry+0x00` at exit. Kinds: 1 drink, 2 clothes/elec,
4 religion, 5 culture, 6 sport, 10 drink-alt, 15 food/raw.

## 2. Retail checkout: shop-tick family

Ticks `FUN_140171E10/140183580/140183D30/140184930/1401856F0` plus
router `FUN_1401D2DD0` (sol_171e10.c, sol_183580.c, sol_183d30.c,
sol_1856f0.c, sol_1d2dd0.c; 61 funcs in sol_funcs file). Per
`(game, bld)`: prologue (`bld+0xDDC=0; bld+0xC38=0; +0xFC0+=dt`;
`FUN_14042BEA0` refresh); Phase A over `bld+0xBC0..0xBC8`
(`person+0x90+=dt*836960`; kick `830820` past day length); Phase B over
customers `bld+0xBD8..0xBE0`, per demand `d < person+0x110`, match
`bld+0x970..0x978` (stride 0xE0, slots 0x10) + `bld+0x121C[needIdx]`
routing (`person+0x720-1`) and `"alcohol"` scan.

Core (sol_171e10.c:280-357): when need met, demand AND storage
decrement in one branch; fee only if `person+0x71c==1` (USD,
`game+0x580`) or `==2` (RUB, `game+0x588`); then `+0x721++` and
`+0x738 += min(rand*score, frac)`. YES, scaling amountNeeded
pre-branch scales goods + fee linearly. Hook at 836B40 exit or
Phase-B entry. No hook-safety analysis; 30D1A0 needs DisasmMany.

## 3. Status and wealth fields

`+0x08` id; `+0x20` home; `+0x30/+0x90` timers; `+0xC8` foreign;
`+0xD4/+0xDC/+0xF8` age; `+0xE8..+0xF4/+0xE0` happiness; `+0x110/+0x118`
demands; `+0x4F0` carry; `+0x71C` 0/1/2 resident/USD/RUB; `+0x720` need
idx; `+0x721` visits; `+0x734` spent; `+0x738` satisfaction. No native
wealth/QoL/ownership field: derive strata from wallet map + `+0x734` +
housing type + `+0x738` + happiness. Sweep +0x738 readers first.

## 4. Open items

DAT_* to decimals; 1d2dd0:130-238 prefilter; panel readers of
+0x734/+0x738; frame-vs-daily rate; 185100 is decree helper.

