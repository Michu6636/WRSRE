# almanac_button (prototype, disabled by default)

A draw-only prototype. It draws one extra game-style backdrop tile after the
eighth tile of the Economy & Trade row. It is not clickable and handles no
input. It is off by default (`almanac_button.ini`: `enabled = 0`).

Status: the Windows glue is **not compiled or run** in the session that wrote
it. Only `almanac_button_geometry.h` is host-tested, with g++ on Linux.

## What the extra draw changes

This is **not** a change-free draw. "Draw only" means no input is handled.

- **Rect pair, RVA 0x9BE2F0..0x9BE2F7 (X and Y, 8 bytes).** The ninth X and Y are
  written with one 8-byte store, and the saved pair is written back with one
  8-byte store. The native `Draw` may also write this pair during the call. The
  restore sets the pre-call pair back, so such a write is reverted. Whether the
  native `Draw` writes the pair is **not verified**.
- **Not written by this plugin:** W and H at `0x9BE2E8..0x9BE2EF`, and
  `0x9BE2F8` onward.
- **Engine `C3D_PANEL2D::Draw` internals.** The ninth call runs the engine's own
  draw code. What that code writes internally is **not identified**. It may
  include per-widget or global draw state, hover or press state, or GPU state.
  Unverified.
- **Plugin counters and log lines.**

Known limitation: the ninth tile reuses the eighth tile's hover state.

Not written by this plugin: the click latch `0xA54E91`, native selection, the
City Hall TAX button and its layout, tax policy, saves, or game files.

## Build guard: what `TsmCheckExeBuild` accepts

`TsmCheckExeBuild` (in `src/game_addresses.h`) returns true when any of these
holds:

- the PE **TimeDateStamp** equals the repo baseline `0x6A3EB6AD`;
- `addresses.ini` `[build] target_stamp` equals the loaded stamp. The operator
  chooses that value. The loader then applies `addresses.ini` RVA overrides to
  its own table;
- `tesmioloader.ini` `[tesmioloader] allow_unverified_exe = 1`, for **any** build;
- no PE image is mapped (test harness).

The stamp is one header field. It is **not** a whole-file check.

This plugin reads its RVAs from compile-time macros, not from the loader's
runtime table. So `addresses.ini` RVA overrides do **not** change this plugin's
addresses. The byte and value checks below are therefore the only layout guard,
and they run in every case above.

## Static checks at start

- **Two call-site byte sequences**: `E8 AC 50 05 00` at RVA `0x31EDEF` (CALL
  `FUN_140373EA0`), and `FF 15 B8 84 4F 00` at RVA `0x3743A2` (CALL `[0x86C860]`).
- The import slot is at RVA `0x86C860` and holds the engine's exported `Draw`.
- **Three constants** read 45 (origin A), 85 (origin B) and 64 (pitch).

These checks do **not** authenticate the whole executable. They do **not** prove
the row layout, what the rect store means, or that the ninth slot is on screen,
unclipped, or clickable.

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

So once a swap has been written, the DLL stays loaded for the session. Start
returns 0 when armed and 1 when inactive. The detour forwards valid calls in
both states.

## Slot ownership and protection

- The swap and the restore are each **one interlocked compare-exchange** on the
  import slot.
- **The swap's expected value is the pointer validated at Start**: the engine
  export, which must equal the slot at that point. The swap does not re-read the
  slot and adopt whatever it holds.
- Three steps are reported as **separate outcomes**: making the page writable,
  the exchange, and restoring the page protection.
  - Page not writable: no exchange is tried, and nothing is written.
  - Exchange finds another value: nothing is written, and that value is left
    alone. Protection is still restored, and its outcome is logged.
  - Exchange succeeds but protection is not restored: the plugin **does not
    arm**. It tries to undo its own write, and logs whether the undo succeeded.
  - Arming requires **both** the exchange and the protection restore to succeed.
- **This does not stop another writer.** A plain store from another plugin, from
  the engine, or from another thread can replace this plugin's detour at any
  time. Nothing here guarantees that another hook is never overwritten. The code
  logs what it observes.
- **Chains.** `ui_panels` copies the slot at its own Start. If this plugin's swap
  came first, that copy is this detour. Its calls come from a different return
  address, so they are forwarded unchanged.
- The detour always forwards valid calls. Once the slot has been written,
  `g_drawOrig` is never cleared, so the detour stays correct while any copy of its
  pointer may be reachable. The DLL is never unloaded by this plugin.
- No unload entry point exists. An installed swap is undone only by
  `RestoreOriginal`, or by restarting the game with `enabled = 0`.

## Rect pair: failure policy

The pair is 8 bytes at an 8-byte-aligned address. The plugin checks the alignment
and readability at runtime. It then does these steps:

| Step | If it fails | Result |
|---|---|---|
| a) 8-byte load of the pair | faults | nothing written; drawing stops; ninth tile not drawn |
| b) 8-byte store of the ninth pair | faults | a faulting aligned store writes nothing; drawing stops; ninth tile not drawn |
| c) native ninth `Draw` | engine fault | propagates, as it would without the plugin |
| d) 8-byte store of the saved pair | faults | **RECT DIRTY** is logged. The pair still holds the ninth-slot values. The plugin cannot undo this. |

Step (b) relies on the processor's behaviour for an aligned 8-byte store to a
non-writable page. That behaviour is **not verified on this build**.

Step (d) is an **unresolved residual**, not a safe failure. The plugin stops
drawing and logs it, but the game continues with the ninth-slot values in the
store. Treat such a session as suspect.

## Faults

- **Faults in this plugin's own code are caught.** The protected blocks contain
  only the snapshot reads and tracker calls (`PlanRowTile`), and the 8-byte pair
  load and store (`LoadRectPair`, `StoreRectPair`). Each handler only returns a
  status or sets interlocked flags. Ordinary code does the cleanup.
- **Faults inside the native `Draw` are not caught.** That covers both the eighth
  tile (forwarded as before) and the ninth tile. They propagate as they would
  without this plugin. Continuing after a fault in engine code can leave engine
  state inconsistent, so this plugin does not swallow those faults.

## Concurrency (assumptions, not verified)

- `Draw` runs on one render thread. `Start` runs on the loader thread before the
  first row draw. The loader runs `StartPlugins` as a sequential loop (verified in
  `src/tesmioloader.cpp`). That the render thread does not patch the slot is an
  assumption.
- The compare-exchange removes the window between this plugin's validation and
  its write. It does not remove a concurrent writer that bypasses the check.

## Build checks

`build.bat` compiles `plugins\almanac_button\almanac_button.cpp` with MSVC, and
copies the `.ini`. A successful build shows only that the file compiles. It does
**not** show that the swap is safe, that the row layout is right, or that the
executable is the expected one. The host tests show only that the row logic
behaves as written.

## Still needs Windows testing (not authorized in this session)

- Build with MSVC. Confirm the SEH blocks, `_ReturnAddress`, the compare-exchange
  swap, `VirtualProtect`, and the import swap compile and link.
- Enable in a disposable environment.
- Confirm the ninth tile renders where expected, that all eight native buttons
  still work, that City Hall TAX is unchanged, and that there is no save
  corruption.
- Confirm the restore path by a forced fault in the plugin's own code, and check
  that the log records the restore result.
- Decide whether the RECT DIRTY residual is acceptable for a supervised test.
- Document what the extra draw changes in the engine (see above). This is not
  established.
