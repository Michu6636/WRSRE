# Citizen Housing Relocation — Investigation Summary

> **Status:** investigation only. No plugin/header/source/INI modified, no
> builds run. Target `SOVIET64.exe` v1.1.1.9 (stamp `0x6A3EB6AD`). Ghidra
> project file-base `0x140000000`; query addresses are `0x140` + RVA.
>
> Raw evidence: `scratch/citizen_relocation.txt` plus decompiles
> `scratch/house_822e60.c`, `house_8303c0.c`, `house_822fd0.c`,
> `house_trans.c`, `house_exec.c`, `house_c6a0.c`, `house_tool.c`,
> `house_168730.c`, xrefs `house_xrefs{,2,3}.txt`, sweeps `house_bc0.txt`,
> `house_20.txt`, `reloc_str.txt`, `house_str.txt`, constants
> `house_floats{,2}.txt`, call-site `house_callsite.asm`.

## 0. Mission-brief corrections (read first)

| Item | Brief | Engine truth (Ghidra + `src/game_world.h`) |
|---|---|---|
| residents vector | `bld+0xBC0..0xBC8` (or `0xC38..0xC40`) | **`bld+0xBC0..0xBC8`** only; `0xC38` is the int idle counter, `0xC40` its qword pair. `DEVELOPMENT_README.md:93` is stale. |
| second vector | — | `bld+0xBD8..0xBE0` = people inside / institutional members (orphanage & prison assignees live HERE, not in `+0xBC0`); third vector `bld+0xC20..0xC28` used when `person+0x95 != 0` |
| age field | `person+0xD4 / 0xDC / 0xF8` | **`person+0xD4` float**, incremented by `0.164384` per daily update; `+0xDC`/`+0xF8` are separate brackets |
| rent | — | **`bld+0x13E0` float** (dorm rent); ability-to-pay is **`person+0xE4`** (NOT happiness — happiness is `+0xE0..0xF4`) |
| `relocate` strings | `.rdata` refs | only `worker_relocate` ×10 (localization table, **no code refs**); no `tool_relocate`, `flat`, `homeless`, `apartment` strings exist — UI text is all `GetString` id |

## 1. The adulthood move-out — the age-21 chain

The whole lifecycle runs from one per-citizen frame loop, and one atomic
transfer function does every move:

```
FUN_14030c790            per-frame citizen driver (5 call sites)
  └─ FUN_140832e90       per-citizen frame refresh (debug logs "Rfsh cit %d")
       └─ FUN_140834de0  DAILY PERSON LIFE UPDATE (game, person)   RVA 0x834de0
FUN_14030d1a0            per-frame world update
  └─ FUN_1402b8ce0       automatic re-home driver
       └─ FUN_14083c6a0  eligible-resident picker                  RVA 0x83c6a0
```

**`FUN_140834de0` (RVA 0x834DE0, `house_trans.c:228-738`)** does, in order:

1. `age_old = person+0xD4; person+0xD4 += 0.164384` — the birthday tick.
2. **21st birthday** (`house_trans.c:291-296`): when `age_old < 21.0 && 21.0 <=
   new_age && person+0xC8 != 0` — split the household money `+0xFC`:
   child gets `(parent+0xFC * 0.5) * 1.5`, parent keeps the other half.
3. **Leave-parents home search** (lines 362-501), gated on
   `age > 21.0 && person+0xC8 != 0` (parent-linked) and not in an orphanage:
   - young (`1.0 <= person+0xA8 < 2.0`) → walk-list pass over the home's
     `bld+0xCA8` (stride `0xF0`) for dorms;
   - otherwise town-wide scan of `game+0x11B08..0x11B10` scoring type-2
     buildings by occupancy, tenure (`person+0x2D`), rent `bld+0x13E0` vs
     `person+0xE4`, and neighbour bonuses — strict-improvement, first-better
     wins. Commit: `FUN_14083ca50(game, person, bestBld)`.
4. **Dorm rent/eviction** (lines 567-663): a dorm (`subtype==1`) resident
   leaves when `person+0xA8 >= 2.0` (adult) **or** `person+0xE4 + bonus <
   bld+0x13E0` (cannot pay the rent); otherwise stays and pays
   (`person+0xD8 -= 0.05` affordable / `-= 0.35` unaffordable).
5. **Orphanage graduation** (lines 698-720): home `0x4A` && `age > 21` →
   unlink parent slots, `FUN_1408305f0` (parent's home, else a random free
   type-2 apartment).

Institutional siblings: **`FUN_1408303c0`** (orphanage placement for
`age < 21`) and **`FUN_14082fa50`** (prison assignment, type `0x4C`, with
sentence decay on `person+0xD8`). **`FUN_140835d70`** is the *despawn* path
(`free(person)`, erase from `game+0x126C0/126D8/126F0/12708`) — it runs when
the update cannot place a citizen; it is NOT a re-home.

## 2. The relocation tool

There is exactly **one citizen-transfer primitive** — `FUN_14083ca50` — and
every path (AI, tool, save-load) funnels through it:

| Path | Entry | Notes |
|---|---|---|
| building-move tool | `FUN_1401da580` (ray-pick dispatcher, `CollisionRay`/`Collision2DBBox` at `DAT_1409941F0+0xff8`) → `FUN_140168730` → `FUN_14083ca50` | `FUN_140168730` (RVA `0x168730`) gates on typedesc type `0x2F` (47), rebuilds walking connections, then re-commits **every** resident of the moved building: `MOV RCX,RSI; MOV RDX,[RDI+0xbc0]+i*8; MOV R8,[game+0x11b08]+idx*8; CALL 0x14083ca50` (`house_callsite.asm:140169993-1401699af`) |
| automatic re-home | `FUN_1402b8ce0` → `FUN_14083c6a0` → `FUN_14083ca50` | per-frame; picker moves dorm adults (`person+0xA8 >= 2`) out, and 21+ parent-linked (`person+0xC8 != 0`) residents to the target |
| save load | `FUN_14028c770` → `FUN_140430fc0` → `FUN_140168730` + `FUN_140822fd0` | restores resident vectors |
| world gen | `FUN_1404e7440` | church/DLC placement; evicts via `168730`, places via `159070` |

**Unresolved:** the UI button that fires the citizen "relocate" command. The
`worker_relocate` localization keys (10 copies in `.rdata`, e.g. `0x1408A6D87`)
have **no code references** — they are resolved through the localization table
at runtime. The button→handler wiring was not isolated; `FUN_1401da580` is the
closest evidenced entry (generic building click dispatch).

## 3. Moving the citizen pointer — mechanics & flags

**`FUN_14083ca50 (game, person, newBld)` (RVA 0x83CA50)** is the safe transfer:

1. `age >= 21 && person+0xC8` → clear the parent's `+0xB0/+0xB8/+0xC0` slot,
   `person+0xC8 = 0` (leave the parental household).
2. cancel walk route (`person+0x660`) for person + 3 family slots;
   `FUN_140830820` demand kick (also for family when old home is normal).
3. **remove** from old home: `FUN_140822fd0` (residents `+0xBC0`) for normal
   homes, `FUN_1408231f0` (people `+0xBD8`) for orphanage/prison.
4. **insert** into new home: grow `newBld+0xBC0..0xBD0` (`FUN_1400b1700` when
   at capacity) and push person; `person+0x20 = newBld`; each family member is
   removed from the old home and pushed into the new one too (**the whole
   household moves**).
5. `person+0xD8 *= 0.6` on voluntary non-dorm moves; recompute
   `newBld+0x140C` (independent tenants) / `+0x1410` (settled tenants).

**Remove helpers:** `FUN_140822fd0 (game, person, bld, flag)` — linear scan +
`std::remove` compact + `memmove` tail on `bld+0xBC0` (or `bld+0xC20` when
`person+0x95 != 0`), then `person+0x95 = 0`; with `flag` it also fires the
building event (`FUN_1401990c0` family). `FUN_1408231f0` is the institutional
twin operating on `bld+0xBD8`. Generic push/grow: `FUN_1400b0e90` /
`FUN_1400b1700`.

**"Homeless / searching" flags** — no dedicated field exists; the state is
derived: `person+0x20 == 0` = no home; `person+0x95` switches the remove helper
to the queued list `bld+0xC20`; `person+0x2D` = settled-resident marker
(counted into `bld+0x1410`); `person+0xA8` life-stage float (`0` child,
`[1,2)` young, `>=2` adult); `person+0xE4` ability-to-pay vs `bld+0x13E0` rent.

## 4. Why citizens never vacate for rent — and where a mod hooks in

Vanilla's **only** rent-driven move-out lives in the dorm branch of
`FUN_140834de0` (lines 567-663): a `subtype==1` building's resident leaves when

```c
fVar28 = bld+0x13E0;                 // the rent
fVar31 = person+0xE4 + bonus;        // ability to pay
if ((2.0 <= person+0xA8) || (fVar31 < fVar28))   // adult OR cannot afford
    -> town-wide search for type-2 subtype!=1 -> FUN_14083ca50(person, best)
```

For ordinary apartments (`subtype!=1`) there is **no rent comparison at all** —
re-housing only happens via the 21+ parent-link rule, the per-frame
`FUN_14083c6a0` eligibility pass, and demolition/load. So the mission premise
("citizens never vacate an apartment due to high rent") is confirmed: the
mechanism exists but is gated to dorms. Honest hook points for a
rent-eviction mod, in order of cleanliness:

1. **`FUN_140834de0` dorm branch** (`0x140835669`-ish, the
   `2.0 <= person+0xA8 || fVar31 < fVar28` test) — widen the gate to
   `subtype!=1` homes, or bias `person+0xE4` downward when rent exceeds income.
2. **`FUN_14083c6a0`** — the per-frame picker already moves eligible residents
   between two named buildings; a plugin can drive it directly with
   `dryRun=0`.
3. **`FUN_14083ca50`** — the atomic executor; call it with any
   `(game, person, newBld)` to perform a complete, family-aware relocation
   (handles parent-unlink at 21, vector surgery, counter recompute). This is
   the single safe entry for a manual relocate feature.

## 5. Function index

| RVA | Name | Role |
|---|---|---|
| `0x30c790` | `FUN_14030c790` | per-frame citizen driver → `832e90` |
| `0x30d1a0` | `FUN_14030d1a0` | per-frame world update → `2b8ce0` (DisasmMany only, 120 s decompile budget) |
| `0x832e90` | `FUN_140832e90` | per-citizen frame refresh; calls `834de0` |
| `0x834de0` | `FUN_140834de0` | **daily person life update**: age++, 21st-birthday split, leave-parents search, dorm rent eviction, orphanage graduation |
| `0x83ca50` | `FUN_14083ca50` | **the move executor** (household-aware transfer) |
| `0x83c6a0` | `FUN_14083c6a0` | eligible-resident picker (dorm adults out; 21+ parent-linked out) |
| `0x822fd0` | `FUN_140822fd0` | remove person from `bld+0xBC0` / `+0xC20` |
| `0x8231f0` | `FUN_1408231f0` | institutional remove twin (`bld+0xBD8`) |
| `0x8303c0` | `FUN_1408303c0` | orphanage placement (`age < 21`) |
| `0x8305f0` | `FUN_1408305f0` | orphanage graduation (`age > 21`) |
| `0x82fa50` | `FUN_14082fa50` | prison assignment (type `0x4C`) |
| `0x835d70` | `FUN_140835d70` | citizen **despawn** (free + global-list erase) |
| `0x168730` | `FUN_140168730` | building-move handler; re-commits residents via `83ca50` |
| `0x1da580` | `FUN_1401da580` | building click/pick dispatcher (tool entry) |
| `0x159070` | `FUN_140159070` | citizen-vector walker; single-citizen assign |
| `0x430fc0` | `FUN_140430fc0` | save loader (resident restore) |
| `0x4e7440` | `FUN_1404e7440` | world/scenario generation |
| `0x822e60` | `FUN_140822e60` | portrait randomization — **not** housing |

## 6. Open items

- Citizen-relocate **UI button wiring**: `worker_relocate` keys carry no code
  refs (localization table); handler not isolated — `1da580` is the evidenced
  entry point.
- `bld+0xC20..0xC28` (`person+0x95` gated) semantics undecoded.
- `FUN_14083c590` (candidate gate in `834de0`/`8305f0`) not decompiled.
- Exact semantics of `person+0x18` (adult-at age?) and `person+0xA8` producer.


