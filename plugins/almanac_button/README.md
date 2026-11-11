# almanac_button (experimental prototype, enabled by default)

A draw-only prototype. It attempts to draw one extra game-style backdrop tile
after the eighth tile of the Economy & Trade row. It is not clickable and
handles no input. The shipped INI and missing-key fallback set `enabled = 1`.
An existing INI with `enabled = 0` remains an explicit opt-out.

**Caution:** when installed, this experimental plugin attempts its guarded draw
hook automatically. It can end the game process on certain faults. It is not a
finished Almanac button. In the one user-performed visual check, the added tile
appeared in the row but the user reports crime-button artwork; it has no click
handling. The supplied log also contains 259 `game.ERROR` lines, including
save-path errors. Their cause is unknown, so this was not a clean safety pass.
Disable the plugin with `enabled = 0` in the active INI and restart.

The screenshot, loader log and result note are preserved at
[`export/almanac-evidence/almanac-button-visual-test-run1/result.md`](../../export/almanac-evidence/almanac-button-visual-test-run1/result.md).
The log reports plugin version 1.5 armed and the extra draw ran, but does not
record the DLL SHA-256 or exact source revision, or establish the disposable
status of the game profile. KiloCode's MSVC build report covers `b5c207d`, not
the default-on follow-up. No independent MSVC build for the exact tested binary
is available. The visual check did not test input, clickability, or behavior at
other scales. The one-run approval has been used; any additional game run
requires separate approval. Only `almanac_button_geometry.h` is host-tested with
g++ on Linux.

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
- **Start runs in a sequential loop** (`StartPlugins`, `src/tesmioloader.cpp`
  lines 1473–1487) inside a `__try` whose filter logs the fault and returns -1.

So once a swap has been written, the DLL stays loaded for the session. Start
returns 0 when armed and 1 when inactive. The detour forwards valid calls in
both states.

## Slot ownership and protection

- The swap and the restore are each **one interlocked compare-exchange** on the
  import slot.
- **The swap's expected value is the pointer validated at Start**: the engine
  export, which must equal the slot at that point. The swap does not re-read the
  slot and adopt whatever it holds.
- **The original protection is captured once**, by the first protect-set in
  Start. Every later restore uses that recorded value. A later `VirtualProtect`
  return value is **not** used as "the original". After a failed restore the page
  stays `PAGE_READWRITE`, so a later call would report the wrong value.
- Each restore is tried **twice**.
- Three steps are reported as **separate outcomes**: making the page writable,
  the exchange, and restoring the original protection.
  - Page not writable: no exchange is tried, and nothing is written.
  - Exchange finds another value: nothing is written, and that value is left
    alone. The original protection is still restored, and its outcome is logged.
  - Exchange succeeds, original protection **not** restored: the plugin **does
    not arm**. It tries to undo its own write, using the same recorded original
    protection. It logs whether the undo ran, what the slot held, and whether the
    **original** protection is now in place. A failed undo leaves the page
    writable, and the log says so.
  - Arming requires **both** the exchange and the original protection restore to
    succeed.
- **This does not stop another writer.** A plain store from another plugin, from
  the engine, or from another thread can replace this plugin's detour at any
  time. Nothing here guarantees that another hook is never overwritten. The code
  logs what it observes.
- **Chains.** `ui_panels` copies the slot at its own Start. If this plugin's swap
  came first, that copy is this detour. Its calls come from a different return
  address, so they are forwarded unchanged.
- The detour always forwards valid calls. Once the slot has been written,
  `g_drawOrig` is never cleared. The DLL is never unloaded by this plugin.
- No unload entry point exists. An installed swap is undone only by
  `RestoreOriginal` (a planning fault), by the Start undo, or by ending the game.

## Rect pair: failure policy (fail-stop)

The pair is 8 bytes at an 8-byte-aligned address, checked at runtime. Each step
below ends in **FailStop** (a disposable-test fail-stop; see the next section)
unless it succeeds:

| Step | If it fails | Result |
|---|---|---|
| a) address unreadable or misaligned | — | FAIL-STOP, before any write |
| b) 8-byte load of the saved pair | faults | FAIL-STOP, before any write |
| c) 8-byte store of the ninth values | faults | FAIL-STOP |
| d) native ninth `Draw` (wrapped, see below) | engine fault | the pair is restored (step e), then FAIL-STOP |
| e) 8-byte store of the saved pair | faults | `RECT DIRTY` is logged, then FAIL-STOP |

Notes:

- The plugin does **not** try to repair the pair or the import slot in a
  fail-stop. Their state is **not assumed**. The pair is never read again to
  decide anything after a failure.
- Step (c) relies on the processor writing nothing for a faulting aligned store.
  That is **not verified on this build**. The policy does not depend on it.
- Step (e) is an **unresolved residual**. If it fails, the pair holds ninth-slot
  values or an unknown mix. The plugin cannot put it back.
- A fail-stop **is not** a safe state for the game. It is a containment step for
  a disposable test.

## FAIL-STOP: what the plugin does, and what it does not do

`FailStop` (in `almanac_button.cpp`):

1. Writes `almanac_button  FAIL-STOP  <reason>` through the loader's `Logf`
   (a direct `WriteFile` to the loader log).
2. Writes a second FAIL-STOP line that says the pair and the import slot are not
   assumed to be in any state.
3. Calls `TerminateProcess(GetCurrentProcess(), 0xA1A10001)`.
4. Calls `__fastfail(7)` as a backstop, if `TerminateProcess` returns.

It does **not** restore the rect pair or the import slot. A log line alone does
**not** stop the game. Ending the process does. Whether `TerminateProcess` ends
the game in a Windows test is **not verified**.

The ninth tile's native call is wrapped by `CallNinthNative`. Its handler only
returns 1. It does not touch state. The caller then restores the pair and fails
stop. This **changes** the earlier "engine faults propagate" policy **for the
ninth tile only**. The eighth tile's native call still has no handler, and its
faults propagate as they would without the plugin. This change needs the user's
acknowledgement.

Faults in the plugin's **own planning code** (`PlanRowTile`) are **not** a
fail-stop. They disarm and call `RestoreOriginal`, unchanged from revision 3.

## Disposable visual check

One supervised, visual-only check is approved but has not been run. The plugin
can terminate or hang the game on certain faults, so use only a disposable game
copy/profile with a copied save and cloud sync off. Do not use valuable saves,
save, advance time, or change policies. A VM is optional if the test copy can be
discarded and recreated. Stay present; if the test crashes or hangs, close only
the test game process and discard the copy. Do not force faults or retry after
an error. If the game build guard refuses to arm, stop rather than bypass it.
Capture the baseline/after screenshots and loader/plugin logs.

## Concurrency (assumptions, not verified)

- `Draw` runs on one render thread. `Start` runs on the loader thread before the
  first row draw. That the render thread does not patch the slot is an assumption.
- The compare-exchange removes the window between this plugin's validation and
  its write. It does not remove a concurrent writer that bypasses the check.

## Build checks

`build.bat` compiles `plugins\almanac_button\almanac_button.cpp` with MSVC, and
copies the `.ini`. A successful build shows only that the file compiles. It does
**not** show that the swap is safe, that the row layout is right, or that the
executable is the expected one. The host tests show only that the row logic
behaves as written.

## Remaining Windows validation

- Rebuild this default-on follow-up with MSVC; the successful KiloCode report
  covers base revision `b5c207d`, not this change.
- Run the one user-approved visual-only check in a disposable environment,
  following the containment steps above. Check whether the extra backdrop appears
  beside the eight existing tiles at the tested resolution and scale.
- The current visual-test approval does **not** include forced-fault or
  protection-failure tests. Do not add those tests without separate approval.
- The visual check cannot establish clickability, general hook coexistence,
  re-entrancy safety, or all effects of the extra native Draw; these remain open.
