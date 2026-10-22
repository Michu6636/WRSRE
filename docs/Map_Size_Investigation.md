# Map Size Investigation — Feasibility of Maps Larger Than 20 km

**Scope:** SOVIET64.exe v1.1.1.9 (base `0x140000000`) + engine `C3DDLL64.dll`.
Investigation only — no game or plugin files were modified. Raw evidence, RVAs,
constants and decompile excerpts: `scratch/map_size_investigation.txt` (plus the
`scratch/ms_*` artifacts it references).

---

## 1. TL;DR Verdict

**Feasible, and mostly data-driven — but not by editing `script.ini` alone.**

The engine (C3DDLL64.dll) reads map dimensions from each map's `script.ini`
(`$WORLDSIZE`, `$HEIGHTMAPSIZE`, `$GRIDSIZE`, `$QUADTREESTARTSIZE`) and the
camera clamp follows the terrain dynamically. However, the **game exe hardcodes
the vanilla 20 km world size in a small set of shared `.rdata` floats**, and
several subsystems depend on them. A memory patch that (a) edits the map's
`script.ini` `$WORLDSIZE` and (b) patches the handful of shared constants
(20000.0, 10000.0, ±9900, ±8000) is the realistic path. The single copy of each
constant means very few byte patches; the risk is in *semantic verification* of
the ~100 reference sites, not in the number of patches.

---

## 2. Where map size actually lives

### Data-driven core (no patch needed)

- Every vanilla map ships `script.ini` with `$WORLDSIZE 20000 20000`,
  `$HEIGHTMAPSIZE 2048 2048`, `$GRIDSIZE 32`, `$QUADTREESTARTSIZE 2048`.
  The map editor's own terrain (`media_soviet\terrain_editor`) uses
  **1000×1000 / 512×512**, proving the engine accepts other values.
- The parser (`%s/script.ini`, `$WORLDSIZE %f %f` strings) lives in
  **C3DDLL64.dll**; the exe reaches it through
  `C3D_TERRAIN::InitializeFromFolder` (called from world loaders
  `FUN_140005920` RVA 0x5920 and `FUN_140094290` RVA 0x94290, folder path in
  `RDX`). `GetTerrainSize`/`GetOffset`/`GetTerrainBoundary*`/`SetTerrainSize`
  are all engine exports — the terrain object (`world+0xED8`) holds the size.
- The **save writer** `FUN_14028C770` converts coordinates using
  `GetTerrainBoundary{X,Z}{Min,Max}` and `GetTerrainSize` (only caller of the
  boundary imports), so saves are boundary-relative and scale with the map.

### Camera & bounding walls — auto-adapting (no patch needed)

- The scroll clamp is `FUN_140015260` (RVA 0x15260): it clamps the camera
  target X/Z to **±0.5 × `C3D_TERRAIN::GetTerrainSize`** (constants −0.5/+0.5
  at `0x14090B2D0`/`0x140909DDC`). It is invoked from the camera controller
  `FUN_1403D85A0` (RVA 0x3D85A0) right before `ViewSetPosition`.
- A legacy fallback clamps against globals `0x1409E4B88..B9C`, which are
  initialized to ±1e33 sentinels (= no clamp) — effectively dead.
- The mode flag `DAT_1409D5FA0` is written by the save loader (`FUN_140430FC0`).
- **Consequence:** change `$WORLDSIZE` and the camera wall moves with it.

### What actually blocks bigger maps (hardcoded constants)

All in `.rdata`, each a single shared float:

| Address | Value | Role (verified refs) |
|---|---|---|
| `0x14090B1A4` | 20000.0 | World size; 23 refs in 18 fns: camera projections (`FUN_1403D85A0/3D7AB0`), terrain-grid scatter `FUN_14024CA10` (`worldX = col·20000/gridW − 10000`), load-time terrain pass `FUN_1404E7440`, map editor `FUN_1404FCAF0` (uses `GetHeightmapSizeX` too), +13 others |
| `0x14090B180` | 10000.0 | Half-world; ~95 refs in ~85 fns (editor `FUN_1404FCAF0` ×17, `FUN_14051D160` ×8, …) |
| `0x14090B178` / `0x14090BD2C` | ±9900.0 | **Pollution emitter gate** (`FUN_1404D57A0`): emission refused outside ±9900 |
| `0x14090B164` / `0x14090BD20` | ±8000.0 | Terrain-processing margins (`FUN_1404E7440`, `FUN_1404DC070`, `FUN_1404DD6C0`, `FUN_1404DDCC0`, `FUN_1402F0F10`) |

Projection far planes (25000.0 / 15000.0) and the camera Y clamp (4544.0,
inline immediate at `0x1403DBB6B`) may also want attention on very large maps.

---

## 3. Environmental grids (pollution) — path 3 answer

Grid block at `world+0x120A8` (12-byte cells), extents at `+0x120B0/B4`
(float, world spans ±½·extent around the origin), width/height in cells at
`+0x120B8/BC`, count at `+0x120C0`.

- **Sampler** `FUN_1404D5C30` (RVA 0x4D5C30): reads every dimension from the
  variables; cell size is the constant **200.0 m** (`0x14090AD18`). Bilinear
  interpolation; index `col + row·width`, stride 12. **Fully variable-driven.**
- **Emitter** `FUN_1404D57A0` (RVA 0x4D57A0): also variable-driven for index
  math, **but gates emission to ±9900** — the one hardcoded assumption. On a
  30 km map the outer rings would silently receive no pollution unless the two
  ±9900 constants are patched (they are shared with `FUN_1404E7440` and
  `FUN_1401A4F80`, so patching them patches those too).
- **Daily decay** `FUN_1404D5F50` (RVA 0x4D5F50): iterates `count` at `+0x120C0`
  — variable-driven.
- **Can we safely overwrite `+0x120B8/+0x120BC`?** Not by themselves. The cell
  array's allocation size is *computed at runtime* (no immediate 120 000 =
  100×100×12 found; `FindImm 0x1D4C0` = 0 hits), and **no `.text` instruction
  writes any of `+0x120A8..+0x120C0` via struct displacement** (exhaustive
  `FindDisp` scans: only reads). The initializer was not located — it is either
  a bulk/serialized write in the save-loader chain or in a Ghidra
  disassembly-gap. Overwriting the dims without growing the allocation would
  desynchronize `row·width` indexing against the allocated array → heap
  corruption. **Recommendation:** find the initializer (or the allocation) in a
  follow-up before touching these fields; patching the ±9900 emitter gate is
  safe independently.

## 4. Pathfinding / graph builders

No evidence of a separate hard coordinate limit in pathfinding: road/walking
graph nodes store world floats, and the placement-time BFS cost (see
`Tesmio Mods\docs/12-walking.md`) is bounded by *work*, not by coordinates.
The 20000.0 in `FUN_14024CA10` is a terrain-grid scatter (index→world
conversion), not a pathfinding boundary. The graph systems inherit whatever
coordinate range the world has; a larger world means larger graphs → memory
and BFS-time growth (the known "placement takes seconds" scaling).

## 5. Recommended approach (for a future implementation mission)

1. **Map data:** set `$WORLDSIZE` (and optionally `$HEIGHTMAPSIZE` +
   `heightmap.dds`, `$QUADTREESTARTSIZE`) in the target map's `script.ini`.
   The engine side (terrain, quadtree, camera clamp, save coordinates) follows.
   Keep `$HEIGHTMAPSIZE` a power of two ≤ 4096 initially; texel density drops
   proportionally (200 m/cell for pollution is derived, not stored).
2. **Binary patch targets** (loader-side, i.e. a TesmioLoader plugin patching
   the exe's `.rdata` at runtime, RVA = address − 0x140000000):
   - `0x90B1A4` 20000.0 → new world size W
   - `0x90B180` 10000.0 → W/2
   - `0x90B178`/`0x90BD2C` ±9900 → ±(W/2 − 100)
   - `0x90B164`/`0x90BD20` ±8000 → ±(W/2 − margin TBD)
   - Verify each consumer of `0x90B180` (~85 fns) before shipping; if any is
     not "half world", patch call sites instead of the shared constant.
3. **Do NOT touch** the pollution dims (`world+0x120B8..`) until the grid
   initializer is identified; the ±9900 gate patch is sufficient to let
   pollution *emit* on larger maps, and the sampler clamps gracefully to the
   (unchanged 100×100) grid — outer areas simply share edge cells.
4. **Test matrix:** editor map create/save, new game on a patched map, save →
   reload round-trip (boundary-relative save coords), camera to all four map
   edges, pollution plume at >9900 from origin, minimap render, road/pipe
   placement at extreme coordinates.

## 6. Confidence & gaps

- Camera clamp, script.ini data-drivenness, ±9900 emitter gate, world-writer
  boundary math, float constant values: **verified in this build** (decompiled
  or disassembled, addresses in the raw file).
- Roles of the other 20000-ref functions (`FUN_1402F0F10`, `1403C4850`,
  `1403D4B40`, `14082F890`, …) are inferred from neighborhoods — verify before
  patching.
- Pollution-grid initializer: **not found** (see §3).
- Engine max for `$WORLDSIZE` inside `InitializeFromFolder`: unknown (DLL not in
  the Ghidra project); the editor's 1000×1000 map proves non-20000 values load,
  but an upper clamp has not been ruled out.

*New Ghidra scripts added for this work: `scratch/FindSymbol.java`,
`scratch/ScanFloat.java`, `scratch/PrintFloats.java`.*

