# 17 - Integration findings (UI, purchase lists, saves) - 2026-10-03

Findings gathered with the headless Ghidra project while adding the private
economy plugins. Everything here is off **SOVIET64.exe v1.1.1.9, build stamp
`0x6A3EB6AD`**, and every offset marked *verified* was confirmed against
disassembly or a running game, not inferred from a neighbouring field.

Addresses are **RVAs**. The Ghidra project for this build is loaded at a
file-based base of **`0x140000000`**, so a query address is
`0x140` + rva (`0x997590` -> `0x140997590`). See "Running the tools" at the end.

Sources read for this page: `reputation.cpp` v1.38 (a third-party plugin on
this same loader - the most complete UI-integrating mod available for
comparison), `plugins/depletion/depletion.cpp` (this project), the
`docs/02-findings.md` panel section, and fresh Ghidra runs recorded below.

---

## 1. Three proven ways into the interface, and they are not equivalent

| Route | What it is | Proven by | Cost |
|---|---|---|---|
| **HUD overlay** | `ui_overlay`'s drawer chain - text drawn over the world by `C3D_TERRAIN::Render` | every plugin in this repo | none; no engine state touched |
| **Panel row append** | post-hook a panel builder, draw after it, write the new bottom edge back | `depletion` (mine panel), `cities` (city window) | needs the panel's builder RVA; breaks on game update until re-found |
| **Import/name hook** | chain `C3D_LANGUAGE::GetString` and rewrite the string a widget is about to draw | `reputation` (every row of the vehicle purchase list) | one IAT swap; string lifetime rules apply (section 6) |

The `reputation` plugin is often described as "integrating into the side
panel". Read against its source, it does **not** append rows to a panel
builder. Its in-game face is the **overlay route** (`DrawOverlay` /
`DrawTextW` / `PollHotkey`, its own F9/F10/F11 windows). What it does that no
plugin in this repo does yet is the **import-hook route**: it lengthens the
country label on every vehicle purchase-list row so the row reads
`"USA - west, licence 20"`, without touching the list builder at all. That is
the cheapest "feels native" integration available, and section 3 has the
offsets it keys on.

For the economy plugins, the panel-row route reaches the place players
actually look (a building's own window), and its mechanism is documented and
in use here - section 2 restates the contract.


---

## 2. Panel-row append - the contract, re-verified

From `docs/02-findings.md`, confirmed again by disassembly of the mine panel
builder this session:

| Window offset | Contents |
|---|---|
| `+0x01` | non-zero while the panel is *not* drawn - builder returns immediately |
| `+0x04`, `+0x08` | panel position (float x, y) |
| `+0x28`, `+0x2C` | panel scroll/offset |
| `+0x240` | **the building the window is about** |
| `+0x250` | **the running Y at the end - the panel's bottom edge** |

Verified at `0x786BE0` (the panel builder this build uses for the mine):

```
140786be0: MOV RAX,RSP / PUSH RBP / ... (register-only frame setup)
140786c4a: MOV RSI,[RDX+0x240]      ; window -> the building
140786c55: MOV R15,[RSI+0x318]      ; building -> its type descriptor
140786c60: CMP byte ptr [RDX+0x1],0x0 / JNZ +0x2663   ; not drawn -> out
140786c6a: MOVSS XMM8,[0x140992088] ; the DPI scalar
140786c85: MOVSS XMM10,[RDX+0x4] ... SUBSS/ADDSS [RDX+0x28]  ; x from the window
140786cb4: MOVSS XMM0,[RDX+0x2c] ... ADDSS [RDX+0x8]         ; y from the window
```

The recipe (the only one in the repo that works):

1. Inline-hook the builder entry. It must have >= 14 position-independent
   stolen bytes. **This builder is a /GS function**, but its *leading* bytes
   are register-only (`48 8B C4 55 56 57 41 54 41 55 41 56 41 57`), so a
   14-byte steal of exactly those bytes is safe. Do not extend the steal past
   the frame setup - the `__security_cookie` load is further in.
2. After the original runs, read `window[0x250]` - the bottom edge.
3. Draw with the game's own font:
   `C3D_FONTMANAGER::PrintLeftUnicodeNoArg(&[0x996FB0], [0x995220], x, y, 0xFF990000, L"...")`.
4. Write `window[0x250] = y + DPI*35` back, or the frame clips the row.

Row geometry (`docs/02-findings.md`): first-row x/y come from
`window[0x04]/[0x08]` plus `window[0x28]/[0x2C]`, each term scaled by DPI
(`0x992088`); one row is `DPI * [0x90A9E4]` = 35.

**What this buys the economy plugins:** a `[P]` building's own window could
show its enterprise card - capital, this month's wages/taxes, pool share -
drawn by `enterprise_base` as a post-hook, in the game's own font and colour,
instead of an overlay column the player has to find. The overlay stays as the
whole-map view; the panel row is the per-building view.

---

## 3. The vehicle purchase list - bloc/licence tags and what is already there

`reputation` tags purchase-list rows by chaining `C3D_LANGUAGE::GetString`
(`?GetString@C3D_LANGUAGE@@QEAAPEA_WH@Z`, object inline at `0x997590`):

- Every vehicle's country is a language id in its record at `+0x9B60`; ids
  `38990..39100` are the vanilla countries, **negative** ids are runtime
  custom strings (workshop authors), so a hook that returns `base` for
  anything else is safe.
- Strings returned by the engine normally live in its own table and are
  stable. A hook returning its **own** buffer must not rely on that: the
  reputation plugin keeps a ring of 64 x 80 `wchar_t` (the game ships 47
  countries) and its comment records that a smaller ring began overwriting
  rows still on screen. Treat any synthetic `GetString` result as valid for
  one frame.
- The tag it appends is `" - west, licence 20"` - the *cheapest unbought tier
  for that bloc*, not the vehicle's own threshold, because the hook only sees
  the country id, not the row.

**New, from disassembly of the world saver `FUN_1402ff840` this session -
the vehicle blueprint flag is a real field:**

```
; FUN_1402ff840, saving:  $BLUEPRINT_OWNED "<object name>"
DAT_1409e7750                     ; = world+0x12840, the vehicle vector
DAT_1409e7758                     ; its end
stride  0x9f08
record + 0x200                    ; the bare object name (compared here)
record + 0x9c88                   ; <- OWNERSHIP FLAG, set to 1 on match
```

So `record + 0x9C88` is a byte the engine itself saves and restores per
vehicle ("owned as an individual blueprint"). A plugin that wants to gate,
grant or price individual vehicles has an engine-persisted bit to write
instead of a sidecar - and it round-trips through a save because the game
writes it. The same function reads `$VEHICLE_SOLD_*` the same way, so sold
history is also in the save format.

Also confirmed in the same function: `world+0x580`/`+0x588` are written with
`movsd` - the two treasury balances are **doubles**, matching the reputation
ini note.

**Why section 1's import hook matters here:** the list builder at `0x814836`
holds live row state in `xmm9`/`xmm11` and an honest hook there needs
hand-written assembly. The cheap route the reputation plugin took - lengthen
the country label on the way out of `GetString` - is copyable. The analogous
move for the economy plugins: tag a *resource* label so a private shop's
stock row reads its wholesale price, or tag a *building type* label in the
construction menu with its private-build cost.


---

## 4. Save-format finding - the game's own saves are plain-text key/value

`FUN_1402ff840` is the world saver, and it is a `strcmp` chain over
`"$TOKEN"`-prefixed lines written with an `fprintf`-shaped helper
(`FUN_1400052f0`). Tokens seen in this session's decompile:

```
$BLUEPRINT_OWNED <object name>
$VEHICLE_SOLD_SOVIET %s %d
$CameraEarthquakeRadius / CD / Duration / Strength   (floats off world+0x148..)
$Tourism_ToursitGeneratedUSD / RUB                    (world+0xBB floats)
```

Two consequences worth knowing:

1. What a save contains is wider than the `.bin` files this repo's plugins
   use. A new per-world value can ride the game's own save file if a matching
   `$TOKEN` write *and read* can be found - but the read side has to exist in
   the loader path, and that is not established, so **do not switch the
   economy sidecars to this without a test session**. The safe statement is:
   the game persists per-vehicle flags and per-world floats this way, and
   `$BLUEPRINT_OWNED` is proof the mechanism round-trips.
2. The loader's `persistence` service (sidecar `.dat` files, as the economy
   plugins use) remains the right tool for plugin-owned state: independent of
   the game's format and its version changes. This finding is an argument for
   *reading* engine state instead of duplicating it, wherever the engine
   already stores it.

---

## 5. Performance rules this project has not written down yet

From the reputation plugin's source comments - all bugs it paid for, and the
economy plugins are one frame of bad code away from the same:

- **Do not scan or `VirtualQuery` per frame.** Resolving a pointer to
  readable memory is a loop of `VirtualQuery` calls that gets slower as
  `.data` fragments under copy-on-write. Resolve when the world opens or on
  demand, cache, revalidate.
- **Compose text when a number moves, not per frame.** The reputation overlay
  went from rebuilding ~40 lines (40 `_snprintf_s` + ~200 font calls) every
  frame to composing into a cache only when dirty, then drawing the cache.
  Same rule for `ui_overlay` drawers: a drawer is called every frame; guard
  the work with a dirty check.
- **Widen once per rebuild, not per frame.** The font call is UTF-16; caching
  narrow text and converting every frame was measurable. Convert at
  composition time.
- **Clamp cached scroll positions at use, not at key-read.** A window's line
  count changes with game state; a scroll value valid yesterday can be past
  the end today.

---

## 6. C3D_LANGUAGE::GetString - the cheapest UI surface, and its two rules

`wchar_t* GetString(C3D_LANGUAGE *self, int id)`, imported; the object is
inline in `.data` at `0x997590` (5656 of 6539 call sites `lea` it
rip-relative - the same inline-object shape as the world at `0x9D4F10`).

Rules when chaining it:

1. **A null from the engine stays null.** The engine returns null for an id
   it cannot resolve; a hook that instead returns a synthetic string makes
   callers take the wrong branch - a label drawn with no name.
2. **Negative ids are runtime custom strings.** `~id` indexes the runtime
   string vector at `lang+0x26720` (see `docs/02-findings.md`). Workshop
   content arrives here. A hook that wants "all mods included" must handle
   `id < 0`.

Keying on ids: label ids resolve through `tools/assets/btf.py` (e.g. `0xC82`
"Quality of source"). A `GetString` hook can therefore rewrite *any* label in
the game by id - including transient ones the panel builders never name -
the cheapest possible way to surface a plugin value inside stock UI.


---

## 7. Running the tools (recorded because it cost a session to get working)

The intended command from `.clinerules` is `$ghidraRunner <Script> <out> [args]`.
There is no `ghidraRunner` on PATH on this machine. What works:

- **Java**: `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot\bin`
  (Temurin 21). Ghidra 12.1.4 wants a JDK on `PATH` or `JAVA_HOME`; without
  one the launcher prints `The 'java' command could not be found` and then
  hangs waiting for a keypress.
- **Ghidra**: `C:\tools\ghidra_12.1.4_PUBLIC\ghidra_12.1.4_PUBLIC`.
- **Project**: `C:\Users\EricM\Downloads\TesmioLoader-master\TesmioLoader-master\ghidra\proj`, program `SOVIET64.exe`.
- **Scripts**: `...\TesmioLoader-master\tools\ghidra\` (`DecompAt`, `Xrefs`,
  `DisasmMany`). The **`.java` versions are the ones that run**: the `.py`
  copies need PyGhidra and fail with `Ghidra was not started with PyGhidra`.
- **One run at a time.** `analyzeHeadless` takes a lock on the project; two
  concurrent invocations die with `LockException: Unable to lock project!`.
  Sequence the calls.

A working wrapper lives at `scratch/ghidra_run.bat`:

```bat
@echo off
setlocal
set "PATH=C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot\bin;%PATH%"
set "GHIDRA_DIR=C:\tools\ghidra_12.1.4_PUBLIC\ghidra_12.1.4_PUBLIC"
call "%GHIDRA_DIR%\support\analyzeHeadless.bat" "...\ghidra\proj" soviet ^
     -process SOVIET64.exe -noanalysis ^
     -scriptPath "...\tools\ghidra" -postScript %*
```

Usage: `ghidra_run.bat DecompAt.java out.c 0x1409d4f10` - note the `0x140`
prefix: `DecompAt` takes the *file-based* address, not the rva.

**Decompiler timeouts at a known cause:** `FUN_14028c770` (a world writer this
build uses) and `FUN_14030d1a0` (the per-frame update) both exceed the
script's 120-second decompile budget. Use `DisasmMany` on a range for those;
the pieces needed above were read that way.

---

## 8. Housekeeping notes

- `scratch/` now holds the working artifacts: `ghidra_run.bat` plus `d_*.c`
  decompiles, `asm_*.txt` disassembles and `x_*.txt` cross-reference dumps.
  They are scratch, but they are also the provenance for this page - keep
  them, or move them under `docs/` if they are to survive.
- `Xrefs.java` on the **world** (`0x1409d4f10`) returns ~2750 references; it
  is not a useful index. Xref a specific field or function address instead.

---

## 9. The window dispatcher - one choke point for every building window (2026-10-03)

Found by xreffing the mine panel builder (`0x786BE0`): exactly one call,
from `FUN_1406e9b80` at rva **`0x6E9B80`** - the game's **window
dispatcher**, decompiled to `scratch/d_dispatcher.txt`. It walks the open
windows out of an array of window pointers at `obj + 0xCAC0` (whether `obj`
is the world singleton needs one confirmation before use), and for each one:

```c
iVar9 = *(int*)(window + 0x248);            // window KIND (0 = building window)
...
uVar1 = *(uint*)(*(window+0x240) + 0x318 + 0x360);   // building -> typeDesc -> BUILDINGTYPE
if      (uVar1 == 7)  { F(game, window); FUN_140786be0(game, window); }   // mine
else if (uVar1 == 2)  { F(game, window); FUN_140761300(game, window); }   // living
else if (uVar1 == 3)  { F(game, window); FUN_1407272f0(game, window); }   // shop
...
else                  { F(game, window); FUN_1406f4740(game, window); }   // fallback
```

`F` = `FUN_1406f8cc0` (rva `0x6F8CC0`), a shared preamble/chrome call the
dispatcher runs before *every* type's builder. Every builder in the chain
takes the uniform signature `(game, window)`.

### The extracted type -> builder map (economy-relevant subset)

| BUILDINGTYPE | What | Panel builder rva |
|---|---|---|
| 2 | living | `0x761300` |
| 3 | shop | `0x7272F0` |
| 4 | (office/other) | `0x72F2D0` |
| 6, 97, 98, 17, 70 | factory / powerplant / generic family - **share one builder** | `0x799630` |
| 7 | mine | `0x786BE0` (depletion's, re-verified) |
| 23 (`0x17`) | pub (runtime-confirmed type on this build) | `0x723950` |
| 24 (`0x18`) | | `0x728A10` |
| anything else | fallback builder | `0x6F4740` |

~70 types are dispatched in total (`scratch/d_dispatcher.txt` has the whole
chain; `scratch/funcs_panels.txt` enumerates the 20 big builders in the
`0x78xxxx-0x7Axxxx` family).

### What this changes for the UI plan

1. **Per-building economy rows stop being hypothetical.** To put an
   enterprise card into a shop window, post-hook `0x7272F0`; a residence,
   `0x761300`; a pub, `0x723950`; a factory, `0x799630`. The append-row
   contract (section 2) is identical for all of them, so ONE shared
   `AppendEnterpriseRow(game, window)` helper can be hooked onto several
   builders.
2. **Or one hook for the entire family:** post-hook the dispatcher and walk
   the same `obj+0xCAC0` window array - every open building window whose
   `+0x248 == 0` gets its row appended after its own builder has written
   `window+0x250`. Verify `obj` first (see caveat above) and confirm the
   dispatcher runs on the render thread before drawing text there.
3. **"Cloning a record" is not a real mechanism here.** Panels are built
   imperatively - `GetString` for the label, `PrintLeftUnicode` for the
   value, `y += DPI*35`, final store to `+0x250`. There is no row/widget
   object to duplicate. The equivalent is *appending* a row in the game's
   own style (font, colour, spacing) - which is precisely what the
   `depletion` mine row does, and it reads as native. For list widgets (e.g. the
   vehicle purchase list), the cheap route remains the `GetString` retag
   (section 3); which `+0x248` kind the purchase list is has not been
   identified yet.

---

## 10. M0 - panel-hook reverse engineering: shop + residence (2026-10-03)

Five sequential Ghidra runs (`scratch/p_shop.asm`, `c_shop.c`, `p_res.asm`,
`c_res.c`, `p_mine.asm`). The mine control reproduced depletion's shipped
21-byte prologue **exactly**, so the method is validated against a hook that
works in game.

### The hook table

| hook | RVA | stolen | expect bytes (verbatim hex) |
|---|---|---|---|
| shop | `0x7272F0` | **21** | `48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 58 F5 FF FF` |
| residence | `0x761300` | **19** | `40 55 56 57 41 56 48 8D AC 24 A8 DB FF FF B8 68 25 00 00` |
| mine (control) | `0x786BE0` | 21 | `48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 F8 F4 FF FF` = depletion's, byte for byte |

- **shop**: register-only prologue, every byte position-independent. The
  `__security_cookie` load sits at `+0x5A`, far past the steal.
- **residence**: the steal ends at `+0x13`, immediately before
  `CALL 0x14084f450` (chkstk, a `rel32`) and the cookie load at `+0x1B`.
  Reaching the CALL would put a relocated-relative into the trampoline; 19
  bytes is the exact safe window (>= 14 required by `.clinerules` rule 2).
- **both**: `(game=RCX, window=RDX)`; gate `*(char*)(window+1)`; building
  `*(window+0x240)`; and they **write `*(float*)(window+0x250)` on every
  return** - shop: single exit (decompile `c_shop.c:588`); residence: both
  exits (`c_res.c:1504`, `c_res.c:2001`). The append contract therefore holds
  for both.
- **shared constant pool** (read at runtime, LayoutF-style): DPI `0x992088`;
  row x = `window[4] + window[0x28] + DPI*(rowStep + indent - back)` where
  rowStep `0x90A9CC`=35, indent `0x90AB98`=110, back `0x90A8CC`=15 - i.e.
  `window[4] + window[0x28] + DPI*130`, the same expression depletion uses
  for the mine. First-row y = `window[8] + window[0x2C] + DPI*0x90A9F0`.
- **fonts**: shop and residence print most rows with the `C3D_FONT*` at
  `0x994200` (a few with `0x995220`/`0x994208`); font manager `0x996FB0`.
  The mine used `0x995220` (depletion's `P_PANEL_FONT`), so the font is
  per-panel - default new hooks to `0x994200`, override per hook if a visual
  test disagrees. Label colour `0xFF990000` (`PANEL_LABEL_COLOUR`).

Remaining for M3: pub `0x723950`, factory `0x799630`, fallback `0x6F4740` -
same recipe, one DisasmMany + one DecompAt each.
