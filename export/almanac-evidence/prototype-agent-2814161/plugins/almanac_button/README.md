# almanac_button (prototype, disabled by default)

A draw-only prototype. It draws one extra game-style backdrop tile after the
eighth tile of the Economy & Trade row. It is not clickable and handles no
input. It is off by default (`almanac_button.ini`: `enabled = 0`).

Status: the Windows glue is **not compiled or run** in the session that wrote
it. Only `almanac_button_geometry.h` is host-tested, with g++ on Linux.

## What the extra draw changes

This is **not** a change-free draw. "Draw only" means no input is handled.

- **Rect store, RVA 0x9BE2E8..0x9BE2FB (20 bytes).** The ninth X and Y are
  written for one call and the saved bytes are restored afterwards. The native
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

- the PE stamp equals the repo baseline (`TsmCheckExeBuild`);
- three call-site byte patterns (`0x31EDEF`, `0x3743A2`);
- the import slot is at RVA `0x86C860` and holds the engine's exported `Draw`;
- three constants read 45, 85 and 64.

These checks **do not authenticate the whole executable**. The stamp check is
bypassed by `allow_unverified_exe = 1` in `tesmioloader.ini`. The byte and value
checks cover three call sites and three constants. **They do not prove the row
layout**, and they do not prove that the ninth slot is on screen, unclipped, or
clickable.

Row-pass consistency (`almanac_button_geometry.h`) is also limited. A pass is a
candidate run of calls: an origin tile, then tiles at the expected X steps, all
with the same row Y, count and gate values. The tracker **does not prove** that a
pass is one native invocation or one frame. It only refuses to draw when the
observed calls are inconsistent.

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

## Ownership, restore and faults

- Start records the original pointer **before** any write. It swaps only after
  every check passes. It then reads the slot back and logs the result.
- Start **never returns non-zero after a swap**. A non-zero return would unload
  the DLL while the slot still points into it.
- `RestoreOriginal` writes the original pointer back only if the slot still
  holds this plugin's detour. If another hook has taken the slot, it writes
  nothing, logs `NOT RESTORED`, and the detour forwards only. The DLL stays
  loaded, because another hook may still chain to the detour.
- No unload entry point exists. An installed swap can only be undone by
  `RestoreOriginal`, or by restarting the game with `enabled = 0`.
- Faults inside this plugin's SEH blocks disarm the plugin, set a flag, and
  restore from ordinary code in `Detour`. Faults inside the native `Draw` are
  **not** caught here.

## Concurrency (assumptions, not verified)

- `Draw` runs on one render thread. `Start` runs on the loader thread before the
  first row draw. Shared flags use `Interlocked` operations.
- The check-then-write on the import slot is **not atomic** against another
  plugin's patch. The residual race is limited by the read-back after the write,
  and by refusing to overwrite a value this plugin does not own.

## Build checks

`build.bat` compiles `plugins\almanac_button\almanac_button.cpp` with MSVC, and
copies the `.ini`. A successful build shows the file compiles. It does **not**
show that the swap is safe, that the row layout is right, or that the executable
is the expected one. The host tests show only that the row logic behaves as
written.

## Still needs Windows testing (not authorized in this session)

- Build with MSVC. Confirm the SEH blocks, `_ReturnAddress`, and the import
  swap compile and link.
- Enable in a disposable environment, with the taxes and ledger plugins off.
- Confirm the ninth tile renders where expected, that all eight native buttons
  still work, that City Hall TAX is unchanged, and that there is no save
  corruption.
- Confirm the restore path: force a fault and check that the log shows the
  restore result.
- Document what the extra draw changes in the engine (see above). This is not
  established.
