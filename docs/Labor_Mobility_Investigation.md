# Labor Mobility Investigation — citizen workplace selection

> **Status:** investigation only. No plugin/header/source/INI modified. Target
> `SOVIET64.exe` v1.1.1.9 (stamp `0x6A3EB6AD`). Ghidra project file-base
> `0x140000000`; a query address is `0x140` + RVA.
>
> Split out of `UI investigation.md` on 2026-10-11 (was §15.2/§15.3 there) so
> that document stays strictly UI-focused.
>
> Raw outputs: `scratch/labor_job_selection.c` (1,229 lines),
> `scratch/labor_job_selection.txt` (83 lines), `scratch/ui_finddisp_ddc.txt`.

---

## 1. `FUN_1401b1290` — worker capacity & production update (NOT job search)

**RVA `0x1401B1290`, size 9,323 bytes.**
Signature inferred from use: `(void* game /*param_1*/, void* building /*param_2*/)`.

Found by `FindDisp ddc` → 240 hits on the `bld+0xDDC` displacement;
`FUN_1401b1290` is the heaviest user with 15 sites. It decompiled cleanly.

### 1.1 What it does

Resets, then iterates `building+0xBC0 .. +0xBC8` — the people physically
standing inside the building (`TSM_BLD_RESIDENTS_BEGIN/END`):

```
building+0xDDC = 0;  building+0xC38 = 0;            // reset
for (i = 0; i < (bld+0xBC8 - bld+0xBC0)/8; i++) {
    person = ((void**)(bld+0xBC0))[i];
    t = C3D_TIMER::PowerTime(...);
    u = FUN_140836960(t, person);
    person+0x90 = t * u + person+0x90;              // accumulate work time
    if (person+0x90 < DAT_14090A934) bld+0xC38++;   // idle count
    if (i < bld+0xDEC)                              // <-- THE OPEN-SLOT GATE
        production += FUN_14018fe20(game,bld)
                    * FUN_140824290(u,person)
                    * FUN_1401bcae0(game,bld);
    if (DAT_14090AA78 < person+0x90) FUN_140830820(game,person);
}
```

### 1.2 `bld+0xDDC` is COMPUTED, not configured

```
if (typedesc+0x360 == 0x5C)                    // BUILDINGTYPE 92
     bld+0xDDC = (prod / typedesc+0x80C) * scale * *(float*)(typedesc+0x6B8 + 8);
else
     bld+0xDDC = (x + prod) * bld+0xDF8 * *(float*)(typedesc+0x6B8 + 8);

f = typedesc+0x7B0;                             // efficiency lerp
if (f >= 0 && bld+0x1048 && game+0x5C0 && game+0x5B0)
     bld+0xDDC = ((1.0 - f) * clamp(bld+0x1084 / DAT_14090A900) + f) * bld+0xDDC;
```

Relevant to `enterprise_base`, which reads `bld+0xDEC`
(`TSM_BLD_MAX_WORKERS`) as the staffing denominator for its wage-bidding
ratio — the value is engine-derived at construction from `$WORKERS_NEEDED`,
not a static `$`-declared runtime figure.

### 1.3 Answers to the selection questions

| Question | Answer |
|---|---|
| Walking distance / transit reach? | **No such term exists here.** Eligibility is positional — a citizen counts because it is inside `bld+0xBC0..0xBC8`. |
| Open-slot gate on `bld+0xDDC`? | **Two distinct fields.** `bld+0xDDC` is the *computed* capacity; the actual loop gate is `index < bld+0xDEC`. |
| Biasable by wage? | **No wage field is read anywhere** in the 9,323 bytes. There is no score to bias. |

**Consequence for `citizen_labor`:** the engine's model is exactly what the
plugin already assumes — workers are observed by *presence*, and capacity is a
derived number. Wage can only influence labour **indirectly**, through whatever
makes a citizen choose a destination. That choice is **not** in this function.

---

## 2. Workplace selection — reachability is PRECOMPUTED, not scored

### 2.1 The decisive fact (from `docs/12-walking.md`, TesmioLoader project)

That document answers the candidate-evaluation question directly:

> A building carries a list of the buildings reachable from it on foot… **Everything
> the simulation does with walking reads that list. There is no "is this close
> enough" test in the job, shop or service code — if the pair is in the list, the
> walk happens.**

| Building offset | Contents |
|---|---|
| `+0xCA8`…`+0xCB0` | `std::vector<WalkingConnection>`, stride **`0xF0`** |
| `+0xCC0` | the same vector for parking / personal cars |
| `+0xBC0`…`+0xBC8` | the building's citizens (adjacent, unrelated) |

| Connection offset | Contents |
|---|---|
| `+0x08` | the building at the other end |
| `+0x98` | path length found, world units (metres) |

The ceiling lives in `query+0x3C`; non-positive means no limit. It is written in
**eight places across three roles**, with no shared constant (Tesmio
`docs/12-walking.md` table):

| Role | Function | Site | Value |
|---|---|---|---|
| builder, walking, batch | `0x12E1D0` | `0x12E2DD` immediate | **480** |
| builder, walking, queued | `0x12E6E0` | `0x12E7AF` → `0x90AF38` | **480** |
| builder, parking | `0x12F830` | `0x12F926` → `0x90B11C` | **2500** |
| overlay, walking | `0x43EF10` | `0x43F04A`, `0x43F835` → `0x90AF38` | **480** |
| overlay, parking | `0x43FEA0` | `0x43FFB3` → `0x90B11C` | **2500** |
| collector, walking | `0x12DE30` | `0x12DEA7` → `0x90AF70` | 530 |
| collector, parking | `0x12F480` | `0x12F502` → `0x90B120` | 2600 |

**Distance is applied once, at graph-build time**, by the builders — not per
candidate at selection time. The queued builder `0x12E6E0` (drained twenty at a
time off `game+0x11F88` by `0x12EC50`) is the path a running game takes.

### 2.2 Answers to the four questions

| Question | Answer |
|---|---|
| Where is the candidate evaluation loop? | **There is no per-candidate evaluation.** Reachability is precomputed into `bld+0xCA8`; selection consumes that list. |
| First-available slot, distance, or scoring? | **Neither a per-candidate distance score nor any scoring formula.** Distance was baked into the list (480 m). `bld+0xDEC` gates *how many standing occupants count* (§1), it is not a selection score. |
| Can wage bias be injected here? | **Not in these routines.** No wage field is read anywhere decompiled (§1, `scratch/lab_walkselect.c`). |
| When does a citizen enter "searching for work"? | **Not located** — see §2.4. |

### 2.3 What was ruled out this session

`FindDisp` sweeps — `scratch/lab_finddisp_*.txt`:

| Sweep | Hits | Conclusion |
|---|---|---|
| `c38` | 76 | `bld+0xC38` is **not** a citizen field — `citizen_labor` uses `ReadablePtr(person, 0xE0)`, so a citizen struct ends at `0xE0`. The `MOV qword [X+0xC38],ptr` + `INC dword [Y+0xC38]` idiom appears in ~15 per-type functions on *different* base registers: a shared base-struct field, not an apartment worker vector. |
| `dec` | 155 | ~9 writers. `FUN_1406f8cc0` (dispatcher's pre-builder helper) does `MOV [R12+0xDEC],0` / `ADD [R12+0xDEC],ESI` — an accumulation for display; `FUN_1401b5420` / `0x1B6A20` write literal states `0`/`1`/`2`. |
| `ca8` | 180 | Heaviest consumer is `FUN_140168730` (8 `SUB [X+0xCA8]`). |

**`FUN_1401b6a20` (RVA `0x1401B6A20`, 4,289 B, `scratch/lab_assign.c`) is wire
logic, not labor:** it iterates a stride-`0x60` vector at
`building+0xA10/0xA18` (the connection vector per `docs/02-findings.md`) and
writes `bld+0xDEC` / `0xDDC` as state values.

**`FUN_140168730` (RVA `0x140168730`, 14,189 B, `scratch/lab_walkselect.c`,
1,718 lines)** decompiled successfully, but its body is `typedesc+0x360` type
dispatch (`== 2` living, `== 8` factory, `== 0x11/0x12` powerplant/substation,
`== 0x2F/0x30/0x32`) and contains **no wage, no distance, and no resolvable
`0xCA8` access** — the `FindDisp` hits resolve to locals. I could not confirm it
as the reachability consumer and am not claiming it.

### 2.4 Still open

- **The "searching for work" transition trigger** — which function flips a
  citizen into a job-seeking / going-to-work state. Not found this session.
- Whether a specific workplace is *chosen from* the list, or a citizen simply
  heads for whichever listed building its AI picks. This is the only place a
  wage bias could act, and it is the next probe if labour mobility is the goal.
- `FUN_140168730`'s actual role, and the 7 other `0xCA8` consumers.
- Per-purpose walk limits: `docs/12-walking.md` notes the walking plugin uses
  **one** limit for work, shops and services, with a separate `0x90ABE8` (125)
  for buildings of type `0x69` and not-yet-built cases — semantics open.

### 2.5 Raw outputs

| File | Content |
|---|---|
| `scratch/labor_job_selection.c` | Decomp `FUN_1401B1290` (1,229 lines) — capacity/production |
| `scratch/labor_job_selection.txt` | working notes for §1 |
| `scratch/lab_assign.c` | Decomp `FUN_1401B6A20` (637 lines) — wire logic |
| `scratch/lab_walkselect.c` | Decomp `FUN_140168730` (1,718 lines) — type dispatch |
| `scratch/lab_finddisp_c38.txt` | 76 hits, `bld+0xC38` |
| `scratch/lab_finddisp_dec.txt` | 155 hits, `bld+0xDEC` + writers |
| `scratch/lab_finddisp_ca8.txt` | 180 hits, walking vector `bld+0xCA8` |
| `scratch/lab_funcs_assign.txt`, `lab_funcs_walk.txt` | function bounds |

---

## 3. Destination pick from `bld + 0xCA8` — SOLVED (session 2026-10-11)

> Method: `FindDisp ca8` (180 hits) → keep only `SUB`-iterators (24) → new
> tool `scratch/FindImm.java` (iterates real instructions, no byte-scan false
> positives) finds every `IMUL …,0xF0` in `.text` (68 sites, 32 functions) →
> intersect → 21 walkers → intersect with `0xDEC` readers → **5 finalists**.
> Raw: `scratch/lab_imm_f0.txt`, `scratch/lab_dest_*.c`,
> `scratch/lab_xrefs_collect.txt`, `scratch/lab_finddisp_121c.txt`,
> `scratch/lab_probe_*.asm`, `scratch/lab_sz_*.txt`.
>
> Tool-tax note: one parallel batch of five `ListFuncs` runs collided on the
> Ghidra project lock (four 300 s timeouts — `.clinerules` rule 1 in action).
> All later runs went sequentially.

### 3.1 The chain: collector → orchestrator

| Function | RVA | Size | Role |
|---|---|---|---|
| `FUN_140828530` | `0x140828530` | 7,123 B | **candidate collector** — walks `bld+0xCA8` at stride `0xF0`, pushes parallel `(path-length, building)` vectors from `connection+0x08` / `connection+0x98` |
| `FUN_1401d2dd0` | `0x1401D2DD0` | 5,400 B | **orchestrator** — per-citizen routing + score comparison + commit |
| `FUN_1401db1b0` | `0x1401DB1B0` | 2,083 B | **manual-assign evaluator** — `gui_building_assign_ok/fail` sounds, click-latch, rejection text ids. UI path, not the AI. |
| `FUN_1401d4330` | `0x1401D4330` | 271 B | the actual assign call |
| `FUN_14042bea0` | `0x14042BEA0` | — | **weight computer** — fills `bld+0x121C..0x122C` |

### 3.2 Collector `FUN_140828530` — how candidates are built

Signature `(game, citizen, building, 0, outVec, 0xFFFFFFFF)`. It iterates the
**home building's** walking list (`lVar7+0xCA8..0xCB0`), and for each entry at
`uVar6 * 0xF0` collects `connection+0x08` (target building) into a pointer
vector and `connection+0x98` (path length, `(int)`-cast) into an int vector.
Per-candidate gates: `candidate+0xDEC` (open slots, line 435),
`typedesc+0x360` type dispatch (`==1, ==2 living, ==10, ==0x26, ==4,
==0x1B/0x1C` routed differently), plus the `PowerTime` /
`FUN_140824290` work-eligibility mix. Returns a **float score** fed through
`FUN_140836a90(score, citizen)` for delivery timing.

### 3.3 Orchestrator `FUN_1401D2DD0` — NEED-driven, FIRST-BETTER-WINS

Signature `(void* game, void* building)`. Three phases:

**Phase 1 — production (lines 108–158).** Per resident in
`bld+0xBC0..0xBC8`: work-time accumulation
`person+0x90 += t * FUN_140836960(t,person)`; idle count `bld+0xC38++` when
`person+0x90 < DAT_14090A934`; slot-gated production
`if (iVar17 < bld+0xDEC)`; then `bld+0xDDC = typedesc+0x814 * fVar38` —
confirming §1 from a second independent site.

**Phase 2 — need lookup (lines 240–277).** Scans a `0xE0`-stride table at
`param_2+0x970..0x978` for the literal `"alcohol"` (8-char compare) — the
building's *need entries*. Match indices `iVar26/iVar18` select the need;
`0x10`-stride names, `plVar12 += 0x1C`.

**Phase 3 — routing per capacity slot (lines 300–355, 661–707).** Per slot
(`local_1a8 < bld+0xDDC`) one citizen is evaluated and routed by its state
and current destination: `FUN_140828530` (walk-collect) when dest is missing,
unlinked, has time, and the need is retail (`type 3/sub 0x22`) or pub
(`type 0x17`); `FUN_140824860` for type `0x4A` / sub `==1` / short time;
`FUN_1408271f0` / `FUN_14082a110` when `bld+0x660 != 0`.

**Selection rule (lines 357–367) — strict-improvement first-better-wins:**

`lVar15 = citizen+0x720 - 1` is the **need index**; each building carries a
**per-need score array at `+0x121C`** (floats, 4-byte stride):

```c
destScore = *(float*)(collectorOut + 0x121C + lVar15*4);   // candidate
homeScore = *(float*)(param_2      + 0x121C + lVar15*4);   // current bld
if (retail-3/22 or pub-0x17 && destScore > homeScore && destScore != homeScore) {
    *(int*)(citizen + 0x28) = 0x4121999a;   // commit: go (10.1f)
    bVar2 = true;
}
```

No best-tracking variable, no RNG, **no distance term at pick time** — distance
only shapes `connection+0x98`, which is *collected* but not part of the commit
comparison. Strict `>` plus a redundant `!=` NaN-guard.

### 3.4 `bld + 0x121C` — the score array, and where wage could enter

17 `FindDisp 121c` hits (`scratch/lab_finddisp_121c.txt`): 7 readers (the
`0x171E10/0x183580/0x183D30/0x184930/0x1856F0` MOVSS-compare family,
`0x82A110`, `0x832E90` MULSS), **one float writer `FUN_14042BEA0`**, and
construction/load init (`0x2BADE0`, `0x430FC0` copy `R12D`; `0x4AAA00`
writes `0xB3B0`; `0x527D20` writes `-1`; neighbours defaulted to `0.25f` —
`scratch/lab_probe_2bade0.asm`, `lab_probe_2bad_pre.asm`).

The writer (`scratch/lab_probe_weight.asm`) computes **5 indices (0–4)** from
`bld+0x1218` plus a quadratic falloff kernel around
`index / DAT_14090A730` vs `DAT_140909DDC`, clamped through MAXSS/MINSS
against `DAT_14090A444` / `DAT_14090B300`, then scaled by an **indirect CALL
target (IAT `0x14086C7B8`)** and `bld+0x320`-object data. That CALL is the only
un-inspected input to the score — a wage term would have to enter here or at
the `0x1D3797` comparison.

### 3.5 Answers to the four questions (final)

| Question | Answer |
|---|---|
| Exact function picking a destination? | **`FUN_1401D2DD0` (RVA `0x1401D2DD0`, 5,400 B, `(game, building)`)** via collector **`FUN_140828530` (`0x140828530`, 7,123 B)**. |
| First-available, random, or scoring? | **Neither.** NEED-driven routing (§3.3) + binary walking-list membership + **strict-improvement first-better-wins** on the per-need score array. |
| Open-slot gate? | **Yes** — `candidate+0xDEC` in the collector; `iVar17 < bld+0xDEC` gates production. Slots are eligibility, never score. |
| Wage bias injectable? | **Yes, and there is exactly one honest point: the `0x121C[lVar15]` comparison at `0x1401D3797`.** No wage field is read anywhere in the decompiled bytes. Bias must be *introduced* — via the `FUN_14042BEA0` input chain, or by hooking the comparison to boost high-wage `destScore` / penalise `homeScore`. The manual-assign evaluator (`0x1DB1B0`, rejection ids `0x147/0x5C8–0x5CA/0x94D–0x953/0x810`, sounds `gui_building_assign_ok/fail`) is NOT the AI path — do not touch it for this. |

### 3.6 Still open

- The IAT target at `0x14086C7B8` feeding `FUN_14042BEA0` (unresolved).
- Need-index ↔ good mapping for `citizen+0x720 - 1` (only `"alcohol"` confirmed).
- `FUN_140824860` (10,636 B, 3 collector call sites) — likely the shop/service twin.
- Whether the `+0x98` path-length vector is ever *ranked* downstream (only collected here).
- The `param_2+0x970` needs-table producer.
