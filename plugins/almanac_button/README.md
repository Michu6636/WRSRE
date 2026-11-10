# almanac_button (prototype, disabled by default)

A draw-only prototype. It draws one extra game-style backdrop tile after the
eighth tile of the Economy & Trade row. It is not clickable and handles no
input. It is off by default (`almanac_button.ini`: `enabled = 0`).

Status: the Windows glue is **not compiled or run** in the session that wrote
it. Only `almanac_button_geometry.h` is host-tested, with g++ on Linux.

## What the extra draw changes

This is **not** a change-free draw. "Draw only" means no input is handled.

- **Rect store, RVA 0x9BE2E8..0x9BE2FB (20 bytes).** The ninth X and Y are
  written for one call, and the saved bytes are restored afterwards. The native
  `Draw` may also write this store during the call, and the restore does not
  undo those writes.
- **Engine `C3D_PANEL2D::Draw` internals.** The ninth call runs the engine's own
  draw code. What that code writes internally is **not identified**. It may
  include per-widget or global draw state, hover or press state, or GPU state.
  Unverified.
- **Plugin counters and log lines.**

Known limitation: the ninth tile reuses the eighth tile's hover state.

Not written by this plugin: the click latch `0xA54E91`, native selection, the
City Hall TAX button and its layout, tax policy, saves, or game files.

## What the static checks do and do not establish

At start the plugin checks:

- the PE **TimeDateStamp** equals the repo baseline (`TsmCheckExeBuild`). This is
  one header field. It is **not** a whole-file check, and
  `allow_unverified_exe = 1` in `tesmioloader.ini` bypasses it;
- **two call-site byte sequences**: `E8 AC 50 05 00` at RVA `0x31EDEF` (CALL
  `FUN_140373EA0`), and `FF 15 B8 84 4F 00` at RVA `0x3743A2` (CALL `[0x86C860]`);
- the import slot is at RVA `0x86C860` and holds the engine's exported `Draw`;
- **three constants** read 45 (origin A), 85 (origin B) and 64 (pitch).

These checks do **not** authenticate the whole executable. They do **not** prove
the row layout, and they do not prove that the ninth slot is on screen, unclipped,
or clickable.

### Row-pass consistency

A pass is a candidate run of calls: an origin tile, then tiles at the expected X
steps. The tracker captures the **scale, origin A, origin B, pitch, row Y and
gate values** on the pass's first tile. Every later tile, including the tile
that decides the draw, must match all of them. Any change drops the pass.

The tracker **does not prove** that a pass is one native invocation or one
frame. It only refuses to draw when the observed calls are inconsistent.

## Row-state evidence (static unless noted)

- Selector: one byte. The host export compares `CMP byte ptr
  [R14+0x10398],R15B` at `0x14031EDE6`, and the call is skipped when the byte
  equals `R15B`. The export zeroes R15 with `XOR R15D,R15D`. That the compare
  always sees zero was not traced.
- Count: `(end - begin) / 0x50` over `[P+0x48]` and `[P+0x50]`, where
  `P = [game+0xC650]`. The handler's arithmetic matches (`ADD R8,0x48`, a
  multiply by `0x6666666666666667` and `SAR 5`, which is division by 80). The
  live values were read once, in a single observation.
- `game+0xC7D4`, `game+0x578` and `game+0x5E0` are dword compares in the handler.

## Loader lifecycle (verified in `src/tesmioloader.cpp`)

- **Init non-zero:** the loader calls `FreeLibrary`. This plugin returns non-zero
  from Init only when disabled, before anything is patched.
- **Start non-zero:** the loader logs `started with N - inactive` and does
  **not** unload the DLL. A swap may already be in place.

So once a swap has succeeded, the DLL stays loaded for the session. Start returns
0 when armed and 1 when inactive. The detour forwards valid calls in both states.

## Slot ownership, swap and restore

- The swap and the restore are each **one interlocked compare-exchange** on the
  import slot. The write happens only if the slot still holds the value this
  plugin observed. This plugin never overwrites a value it did not see.
- **This does not stop another writer.** A plain store from another plugin, from
  the engine, or from another thread can replace this plugin's detour at any time.
  Nothing here guarantees that another hook is never overwritten. The code logs
  what it observes.
- **Chains.** `ui_panels` copies the slot at its own Start. If this plugin's swap
  came first, that copy is this detour. Its calls come from a different return
  address, so they are forwarded unchanged.
- The detour always forwards valid calls, so it stays correct while any copy of
  its pointer may be reachable. The DLL is never unloaded by this plugin.
- `RestoreOriginal` runs from ordinary code, never inside an exception handler.
- No unload entry point exists. An installed swap is undone only by
  `RestoreOriginal`, or by restarting the game with `enabled = 0`.

## Faults

- **Faults in this plugin's own code are caught.** That covers the snapshot reads
  and tracker calls (`PlanRowTile`) and the two rect-store stores
  (`WriteNinthSlot`). Those handlers only set interlocked flags. Ordinary code
  then resets the tracker, stops drawing, and calls `RestoreOriginal`.
- **Faults inside the native `Draw` are not caught.** That covers both the eighth
  tile (forwarded as before) and the ninth tile. They propagate as they would
  without this plugin. Continuing after a fault in engine code can leave engine
  state inconsistent, so this plugin does not swallow those faults.

## Concurrency (assumptions, not verified)

- `Draw` runs on one render thread. `Start` runs on the loader thread before the
  first row draw. The loader runs `StartPlugins` as a sequential loop (verified in
  `src/tesmioloader.cpp`). That the render thread does not patch the slot is an
  assumption.
- The compare-exchange removes the window between this plugin's check and its
  write. It does not remove a concurrent writer that bypasses the check.

## Build checks

`build.bat` compiles `plugins\almanac_button\almanac_button.cpp` with MSVC, and
copies the `.ini`. A successful build shows only that the file compiles. It does
**not** show that the swap is safe, that the row layout is right, or that the
executable is the expected one. The host tests show only that the row logic
behaves as written.

## Still needs Windows testing (not authorized in this session)

- Build with MSVC. Confirm the SEH blocks, `_ReturnAddress`, the compare-exchange
  swap, and the import swap compile and link.
- Enable in a disposable environment.
- Confirm the ninth tile renders where expected, that all eight native buttons
  still work, that City Hall TAX is unchanged, and that there is no save
  corruption.
- Confirm the restore path by a forced fault in the plugin's own code, and check
  that the log records the restore result.
- Document what the extra draw changes in the engine (see above). This is not
  established.
