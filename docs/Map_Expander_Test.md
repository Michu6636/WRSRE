# Map Expander - test setup (30 km map)

Pairs with `plugins/map_expander/` (world_size = 30000.0 default).
The plugin patches the six hardcoded .rdata floats in SOVIET64.exe;
the map below gives the engine the matching $WORLDSIZE.

## 1. Make a 30 km test map (copy, do not edit vanilla in place)

1. Copy a vanilla map folder, e.g.
   `media_soviet/terrainblank` -> `media_soviet/terrainblank_30km`.
2. In the COPY's `script.ini`, change ONLY this line:
   `$WORLDSIZE 20000.000000 20000.000000`
   to
   `$WORLDSIZE 30000.000000 30000.000000`
3. Leave `$HEIGHTMAPSIZE 2048 2048`, `$GRIDSIZE 32`,
   `$QUADTREESTARTSIZE 2048` untouched for the first test.
   (Texel density drops: 20000/2048 = 9.8 m/texel becomes
   30000/2048 = 14.6 m/texel. Pollution cells stay 200 m wide,
   derived from the extent, not stored.)
4. Keep the heightmap.dds as-is. The terrain stretches the same height
   field over the larger extent: hills get wider, not taller.

## 2. Deploy the plugin

- `build.bat map_expander` builds `build/plugins/map_expander.dll`.
- Copy `map_expander.dll` + `map_expander.ini` to
  `C:\Workers Resources Soviet Republic\build\plugins\`.
- Confirm `map_expander.ini` starts with `[`, not a BOM: the first
  three bytes must NOT be `EF BB BF`.

## 3. Headless verification (no game window needed)

`build\tesmiolauncher.exe --nogui` launches the game with the loader.
Watch `C:\Workers Resources Soviet Republic\build\tesmioloader.log`
for, in order:

1. `plugin   map_expander ... from map_expander.dll`
   (Init accepted: constants still vanilla, config sane.)
2. `map_expander  probe  world=30000.0 half=15000.0 gate=+-14900.0 margin=+-13000.0`
3. Six `map_expander  patch  0x...` lines (before -> after per site).
4. `map_expander  live v1.0: world 30000.0 ...`
5. NO `FAILED`, NO `refusing`, NO access-violation / fault-filter lines
   from the plugin.

Failure modes and what they mean:

| Log line | Cause | Fix |
|---|---|---|
| `rva 0x... is X, expected Y - refusing` | Game build is not v1.1.1.9; a constant moved | Do NOT force: audit the new address first |
| `FAILED  VirtualProtect ... - refusing, nothing written` | .rdata page would not unlock | Run with normal permissions; check AV hooks |
| `disabled` | `enabled = 0` in the ini | Set `enabled = 1` |
| No `map_expander` lines at all | DLL not in the game-side plugins folder, or off in tesmioloader.ini `[plugins]` | Deploy + enable |

## 4. In-game test matrix (from Map_Size_Investigation section 5)

1. New game on the 30 km copy: loads without an immediate fault.
2. Camera to all four map edges: the clamp follows GetTerrainSize,
   so the wall should sit at +-15000, not +-10000.
3. Save -> reload round-trip: save coords are boundary-relative, so a
   building placed at x = +12000 must reload at +12000.
4. Pollution plume at >9900 from origin: emission must register
   (the +-9900 gate is now +-14900). Check downwind ground values.
5. Road placement at extreme coordinates (x near +-14000): the
   terrain scatter (`col*W/gridW - halfW`) must agree with the
   visible ground.
6. Minimap render: stale 20 km bounds show as a clipped inset.

## 5. Known residuals (NOT patched by v1.0, safe to defer)

- Inline `MOV 0x461C4000` immediates in .text (FUN_140007d20/ed0
  emissive-map ortho bounds): 20 km ortho box in the emissive-map
  generator. Visual-only, gameplay-safe.
- Projection far planes 25000.0 / 15000.0 and camera Y clamp 4544.0:
  only matter past ~40 km; revisit if world_size grows further.
- Pollution grid stays 100x100 cells: outer rings share edge cells
  (sampler clamps gracefully). Do NOT touch world+0x120B8 dims:
  the allocator was never found, so resizing risks heap corruption.
