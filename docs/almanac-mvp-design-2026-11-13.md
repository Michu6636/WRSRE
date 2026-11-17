# Almanac asset-free functional MVP — design comparison and recommendation

**Date:** 2026-11-13
**Branch:** `arena/42800ec6-wrsre` (base `1ecb74d`)
**Status:** Design only. No source edits, build, deployment, game test, or Ghidra
run was performed for this document. Static evidence comes from the tracked
exports under `export/` and from `objdump` reads of the tracked
`tools/C3DDLL64.dll` (SHA-256
`65fcb6845c3cb7c25a22121f25904a855ea88657e31a3b50a15adac26e231516`,
image base `0x180000000`). The game executable is **not** present on this host;
all EXE addresses below are preferred-image VAs (base `0x140000000`) taken from
the tracked disassembly exports, whose provenance is the historical
`SOVIET64.exe` v1.1.1.9 capture recorded in
`export/almanac-evidence/pe-import-table-20261109/MANIFEST.md`.

## 0. Decision

**Recommend Approach 1: separate additive button + opaque mod-owned content
overlay, bracketed by a whole-function detour of the global sidebar host
`FUN_140317DB0`.** Approach 2 (native-content suppression) is the fallback and
is **not implementable today**: no proven suppression boundary exists that
covers both trade handlers, preserves their output/cleanup contracts, and
restores vanilla state. The specific missing facts are listed in §7.

The recommendation is conditional on the verification gates in §3.7 and §8.
If the opaque-fill gate (§3.4) cannot be met asset-free, Approach 1 fails the
"hide native content" requirement and the project must either resolve the
Approach 2 suppression contract or descope the hide requirement — it must not
ship a translucent overlay that leaves native widgets visible and clickable.

## 1. Scope and acceptance criteria

In scope (design):

- Preserve all eight native Economy & Trade toolbar buttons: order, geometry,
  selection behavior, sounds, and state writes unchanged.
- Add a **separate** ninth Almanac button in the same row. No append to the
  native sub-entry vector; no synthetic native selection index; no
  `game+0x5E6` suppression; no reuse of `ui_panels.widgetButton` (building
  windows only); no inline patch at `0x1403745B6`.
- While open: show mod-owned content; hide **and** block native content
  underneath (clicks at minimum; scroll/state mutation assessed in §3.5).
- On close: restore vanilla appearance and behavior with no residual mod
  writes to native game state.
- Account for **both** trade handlers: `FUN_140373EA0`
  (`game+0x10398 != 0`) and `FUN_140361BB0` (`== 0`).
- Correct `GetMouseSolid`/`Collision` ABI (§2.3). Minimal native-state writes.
- Asset-free: no new texture/icon files, no texture-loading pipeline. Text and
  already-loaded native draw primitives only.

Out of scope: implementation, build, game test, Ghidra queries, icon/texture
loading, the global-section product variant, finance-data service contracts
(covered by `docs/almanac-implementation-spec.md`; this document assumes a
mod-owned content provider exists or renders an "unavailable" state).

## 2. Verified baseline used by both approaches

### 2.1 Dispatch and the two handlers

Host `FUN_140317DB0` reaches the `trade` branch by name compare and dispatches
at `0x14031EDE6` on byte `game+0x10398`
(`export/140317db0_full_disasm.asm`):

| Selector | Call | Output contract |
|---|---|---|
| `!= 0` | `CALL 0x140373EA0` at `0x14031EDEF`; args `RCX=game`, `RDX=&local_x` (`[RBP+0x48]`), `R8=&local_y` (`[RBP-0x10]`) | Host then computes `game+0xAE58 = local_y - game+0xAE50` at `0x14031EDF4`–`0x14031EE02` |
| `== 0` | `XOR R9D,R9D; CALL 0x140361BB0` at `0x14031EE10`–`0x14031EE13`; same first three args, `R9=0` | Handler writes `game+0xAE58` itself (`0x140361F12`); host does not recompute |

Both handlers draw the same eight-entry row (with different index mapping and
skip gates) and then dispatch content by `game+0xC65C`. Row geometry constants:
origin `DPI*(45+85)` (`0x14090AA0C` + `0x14090AB14`), pitch `DPI*64`
(`0x14090AAA0`), DPI at `0x140992088`; these are the constants the existing
`plugins/almanac_button/almanac_button_geometry.h` already tracks.

Row click gates differ and **both must be handled**:

- Normal: `Collision` at `0x1403742A4`, then direct latch byte compare
  `CMP [0x140A54E91],CL` at `0x1403742F2`; selection store
  `[RSI+0xC65C]=EBX` at `0x140374356` (reached on repeat clicks too),
  `[RSI+0x10BE8]=-1` at `0x14037435C`.
- Alternate: `GetMouseSolid` at `0x1403620A9`, `Collision` at `0x1403620C1`,
  then import call `[0x14086C8B8]` at `0x14036213B`; selection store
  `[R14+0xC65C]=ESI` at `0x140362193`, `[R14+0x10BE8]=-1` at `0x14036219A`.
- Each handler also has a single-button alternate path (`game+0xC7D4 != -1`):
  normal `0x1403743F2` (Collision at `0x1403744C3`, latch at `0x140374522`),
  alternate `0x1403622A2` (Collision at `0x14036239E`, press import at
  `0x14036241B`). A row-click observer must include these sites.

Content dispatch: normal body begins at `0x1403745B6` (rejected hook site);
`FUN_14035F3E0` is called at `0x140374606` only for unsigned selection `<= 1`;
indices 3/4, 5, 6 and default take other paths (`0x140374ACC`, `0x140374B35`,
`0x140374BBF`, `0x140374C5D`). Alternate dispatch at `0x14036252B`:
3/4 → `0x1404ACF50`, 5 → `0x1404B0E20`, 6 → `0x1404B24A0`, and
`FUN_14035F3E0` at `0x1403628B8` gated by `R15==0 && sel<=1`.
Cleanup: normal tail `0x1403826CA` calls four destructors including the
allocated local vector free `0x14000FED0([RBP+0x50])`; alternate tail
`0x140373E2C` calls `0x14000FED0([RBP+0x110])`, with an early-exit jump past it
to `0x140373E31`. Any suppression design must preserve these contracts.

### 2.2 Hook-site prologues (re-checked this session)

`src/tesmioloader.cpp:InstallInlineHook` writes a 14-byte absolute jump, requires
`stolen >= 14`, byte-compares the stolen span, and copies it **without
RIP-relative relocation**. A site is usable only if the stolen span is whole
instructions, position-independent, and contains no branch into the span.

| Function | Safe steal | Bytes | Notes |
|---|---|---|---|
| `FUN_140317DB0` (host) | **23** (`0x317DB0`–`0x317DC7`) | `40 55 41 54 41 55 41 56 41 57 48 8D AC 24 D0 D4 FF FF B8 40 2C 00 00` | 5 pushes + `LEA RBP,[RSP-0x2B30]` + `MOV EAX,0x2C40`; ends immediately before `CALL 0x14084F450` (_alloca probe). 18-byte steal also whole. Corroborated by `docs/UI investigation.md` §13.4 and the export head. |
| `FUN_140373EA0` (normal) | **14** (`0x373EA0`–`0x373EAE`) | `48 8B C4 55 56 57 41 54 41 55 41 56 41 57` | `MOV RAX,RSP` + 7 pushes. The first RIP-relative load is at `0x373F0D` (+0x6D), **outside** the steal. (Corrects the session-memory note that placed it inside the first 14 bytes.) |
| `FUN_140361BB0` (alternate) | **21** (`0x361BB0`–`0x361BC5`) | `40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 C8 E5 FF FF` | 8 pushes + `LEA RBP,[RSP-0x1A38]`. A 14-byte steal would split the LEA; 21 is the first whole-instruction boundary ≥14. Next is `MOV EAX,0x1B38` then `CALL _alloca probe`. |

All three spans are position-independent and branch-free, so the existing
loader can trampoline them without relocation support. Entry detours are
therefore *mechanically* feasible; feasibility is not the same as a proven
suppression contract (§4).

### 2.3 Engine ABI — verified against the tracked DLL this session

Export RVAs resolved from `objdump -p tools/C3DDLL64.dll` (ordinal base 1):

| Symbol | DLL VA | Verified behavior |
|---|---|---|
| `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` | `0x18007CBA0` | MSVC x64 **sret**: `RCX=this`, `RDX=hidden out*`; writes 3 floats to `[RDX]`, returns `RAX=RDX`. Correct typedef: `void (*)(void* input, C3DVec3* out)` (the `ui_panels` v6.0 form). The prototype's `float* (*)(void*)` raw-RAX trick happens to match the register layout but is not the documented ABI and must not be copied. |
| `?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z` | `0x1800BC690` | `RCX=panel`, `RDX=C3DVECTOR3*` (by-value struct passed by pointer), `XMM2=padX`, `XMM3=padY`; returns `AL` bool. **Mutates** `[RDX+4]` (subtracts panel depth `+0x674C`) and panel fields `+0x2C4/2C8/2CC/2D0…2F0` on the in-bounds path. Reads bounds `+0x6740/6744` normalized by `+0x673C`/`+0x6734`. It is a hit-test, not a clip. Do not call it speculatively on a shared mouse vector without restoring/using a copy. |
| `?GetMouseLeftPress@C3D_INPUT@@QEAA_NXZ` | `0x18004D340` | Pure getter: `MOVZX EAX,byte [RCX+0x301]; RET`. **Does not consume.** |
| `?GetMouseLeftDown@C3D_INPUT@@QEAA_NXZ` | `0x18004D330` | Pure getter of `[RCX+0x300]` (physical held). |
| `?GetMouseLeftPressAOff@C3D_INPUT@@QEAA_NXZ` | `0x18004D460` | Consuming edge: if `[RCX+0x301]!=0` clears it and returns 1, else 0. |
| `?SetReallyRender@C3D_PANEL2D@@QEAAX_N@Z` | `0x1800BC660` | Writes `DL` to `[RCX+0x6760]`; `Draw` tests this byte at `0x1800BB694` to gate a render pass. |
| `?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z` | `0x1800BAF40` | Queues a quad from rect globals; writes hit-test bounds `+0x6740/6744` (so `Collision` agrees with the **last drawn** rect); no per-pixel clipping (delegated to patch helpers). |

**Latch identity is now proven, not merely numeric:** input singleton
`0x140A54B90 + 0x301 = 0x140A54E91`, and `GetMouseLeftPress` reads exactly that
byte. The normal handler's direct `CMP [0x140A54E91]` and the alternate
handler's import call are therefore the **same signal**. Consumption is a
write of 0 to that byte (as `GetMouseLeftPressAOff` and `FUN_14035F3E0` do).
The latch can outlive the physical press, so any mod consumer must dedup
against physical release (`GetMouseLeftDown` / `GetAsyncKeyState(VK_LBUTTON)`)
exactly as `ui_panels` v3.7 does (`s_clickConsumed` re-arms only when
`!isLDown && !latch`).

IAT slots verified by PE import parsing
(`export/almanac-evidence/pe-import-table-20261109/`): `0x86C850` Collision,
`0x86C860` Draw, `0x86C868` GetMouseSolid. Slot `0x86C8B8` is called with
`RCX=0x140A54B90` and its result tested as a bool; the DLL export table proves
`GetMouseLeftPress` exists and is a getter of `+0x301`, and the historical
Ghidra excerpt names the slot `PTR_GetMouseLeftPress_14086c8b8`, but the PE
manifest does not include this slot's Hint/Name. **Installation must resolve
imports by mangled name via `FindIatSlot`, not by hardcoded slot**, and verify
the resolved slot equals the expected anchor before arming. `0x86C870`
(PrintAllTexts) and `0x86C8F8` (EndDraw candidate) are historical leads from
`docs/UI investigation.md` §13.3 and call-site shape; resolve by name likewise.

## 3. Approach 1 — separate button + opaque overlay (recommended)

### 3.1 Hook inventory

All IAT hooks resolve by **mangled name** (`FindIatSlot(g_exe, "C3DDLL64.dll",
sym)`) and verify the resolved slot against the evidence anchor before arming.
All hooks forward to the saved original unless stated; none patches game code
except H1.

| ID | Site | Mechanism | Role |
|---|---|---|---|
| H1 | `FUN_140317DB0` entry, exe+`0x317DB0`, steal 23 | `InstallInlineHook` (span verified §2.2) | Frame bracket: pre-original input arbitration; call original; post-original overlay/content/button draw and close detection. |
| H2 | `C3D_PANEL2D::Draw` import (anchor slot `0x86C860`) | IAT swap, `_ReturnAddress()` filter | **Passive observer.** At row-tile return sites (`0x3743A8` normal, `0x36220A` alternate, plus the two single-button-path Draw sites) capture the rect globals and tile order/count into a mod-owned RowTracker. At the body-background Draw **return addresses** (`0x3745B6` normal, `0x3624D3` alternate) capture the background draw recipe (rect globals + panel color/white-overlay state) for asset-free opaque replay. These addresses are used only as `_ReturnAddress()` filter values in the IAT hook — **no code is patched at `0x3745B6`** (the rejected inline site). Forward every call unchanged; **no rect-pair swap** (unlike the v2.0 prototype). |
| H3 | `C3D_PANEL2D::Collision` import (anchor `0x86C850`) | IAT swap, return-address filter | **Passive row-press observer.** At row Collision return sites (`0x3742AA`, `0x3744C9`, `0x3620C7`, `0x3623A4`): if open and result true and latch byte != 0, record `nativeRowPress=true`. Forward unchanged; never consumes, never alters the result, never calls Collision itself. |
| H4 | `C3D_FONTMANAGER::PrintAllTexts` and `C3D_PANEL2D::EndDraw` imports | Resolve by name; call originals directly post-host | Flush mod-owned text/quads after drawing. Not intercepted; used as API. Anchors `0x86C870` / `0x86C8F8` are leads to verify at install. |

No hook is installed on `GetMouseLeftPress`, `GetMouseSolid`, or either trade
handler entry in this approach. The pre-host arbitration in H1 plus the proven
latch identity (§2.3) covers both handlers' click gates, including
`FUN_14035F3E0`'s direct latch reads, because all of them read the same byte
that H1 consumes for mod-owned regions.

### 3.2 Per-frame sequence (H1 wrapper)

```text
enter wrapper(game):
  if !armed or !buildVerified:        tail-call original; return
  if section(game) != trade or !sidebarVisible(game):
                                      tail-call original; return
  rects = rowTracker.lastFrameRects() // button slot + body rect (H2)
  if rects.provisional:               use constant geometry, mark provisional
  mouse = GetMouseSolid(input, &localVec)      // sret, mod-local output
  latch = GetMouseLeftPress(input)             // pure getter
  down  = GetMouseLeftDown(input)              // physical held
  if !down && !latch: consumed = false         // ui_panels v3.7 re-arm rule
  edge = latch || (down && !wasDown)
  if edge && !consumed:
      if inside(mouse, buttonRect):
          open = !open; consumeLatch(); consumed = true
      else if open && inside(mouse, bodyRect):
          consumeLatch(); consumed = true
          routePressToModContent(mouse)        // mod widgets only
      // native row / outside: leave latch untouched
  wasDown = down
  nativeRowPress = false
  original(game)                               // ALL native drawing+state runs
  // H2 captured this frame's row rects; H3 may have set nativeRowPress
  if section(game) != trade or !sidebarVisible(game) or nativeRowPress:
      open = false
  if open:
      saveSharedDrawState()
      drawOpaqueBodyOverlay(rects.body)          // §3.4 gate
      drawModContent(rects.body)                 // text + native-style widgets
      drawAlmanacButton(rects.button)            // asset-free label/tile
      flushModDraws()                            // EndDraw + PrintAllTexts
      restoreSharedDrawState()
  rowTracker.commit()                            // this frame becomes lastFrame
```

`consumeLatch()` is the single native input write: `*(BYTE*)(input+0x301)=0`,
guarded by `ReadablePtr`. `sidebarVisible` uses the host's own gate: the traced
path exits early at `0x1403180D8`–`0x1403180DF` when `game+0xFCE != 0`, so
`FCE == 0` is the draw-visible condition (polarity to be re-confirmed at the
implementation gate, §7). `section(game)` compares the selected entry
`game+0xC650` (pointer identity captured when the trade dispatch was observed,
plus the `trade` name at `panel+0x01` as a cross-check); it never writes it.

### 3.3 Input ownership and click-release behavior

- **One press, one action.** The mod acts on the rising edge (`latch ||
  physicalEdge`) only when `!consumed`. After acting it writes the latch to 0
  and sets `consumed`; it re-arms only when the physical button is up **and**
  the latch is clear (§2.3). Holding the button never repeats; releasing does
  nothing. This is the `ui_panels` v3.7 rule, adopted because the engine latch
  can outlive the press.
- **Opening press.** Consumed before the original host runs, so neither trade
  handler nor `FUN_14035F3E0` can also see it. The button slot lies outside the
  eight native tile rects, so no native tile loses a click.
- **Body presses while open.** Consumed before the original host runs and
  routed to mod widgets. Because the normal handler's direct latch compare,
  the alternate handler's `GetMouseLeftPress` getter, and the content helper's
  direct latch reads all observe the same byte, one consumption blocks all of
  them for that press. Hover-only native code still runs (it reads
  `GetMouseSolid`, not the latch) but is visually covered by the overlay.
- **Native row presses while open.** Not consumed. H3 observes
  `Collision==true && latch!=0` at the four row return sites (both handlers,
  both single-button paths) and requests close. This catches a repeat press on
  the already-selected tile, which a selection-change detector would miss.
  The native handler then performs its own store/sound exactly as vanilla.
- **Drag semantics.** A press that starts in a mod-owned region is consumed
  there; dragging out does not release it to native code. A press that starts
  outside and drags in is not a mod edge (the latch was already consumed or
  never set for the mod), so the mod does not act mid-drag.
- **Outside presses.** Passed through untouched. Whether an outside press
  closes the Almanac is a product decision; the MVP default is **keep open**
  (close triggers are explicit, §3.5), matching
  `docs/almanac-implementation-spec.md` §2.4.
- **Scroll/keyboard.** No wheel import was found in either trade handler
  export; native body scrolling (if any) lives in the host's generic sub-tab
  widget or content helpers. If a scroll path mutates persistent native state
  while the overlay is open, it must be gated for the body region or the
  mutated field restored on close; this is an unresolved fact (§7), not an
  assumed no-op. Keyboard focus is not claimed by the MVP.

### 3.4 Draw order and the asset-free opaque fill

**Order.** H1 draws the overlay, content, and button **after the original host
returns**. The host's own text flushes (`PrintAllTexts` at `0x14031F1EE`,
`0x14032C185`, `0x14032C36C`, `0x14032C5DB`, …) and its final EndDraw-class
calls (`0x14032C6E4`, `0x14032C7A2`) all precede the return, so a post-host
quad covers native body text as well as native body quads. The mod then flushes
its own quads/text (H4) before returning to the host's caller, so nothing mod
owned is left queued for the next native flush. The row button is drawn in the
same post-host pass; it cannot be overdrawn by native body content because the
body rect starts below the row.

**Opaque fill candidates (asset-free, in priority order):**

1. **Replay the native body-background recipe.** H2 captures, at the
   body-background Draw site (`0x1403745B6` / `0x1403624D3`), the rect globals
   and the panel color/white-overlay state in effect. Post-host, the mod
   restores that state, substitutes the body rect, and calls the original
   `Draw`. This reuses the already-loaded native sidebar material — no new
   texture. **Gate:** the captured recipe must actually produce an opaque
   cover over the full body rect at all DPI/scales; verify alpha/blend and
   9-patch border behavior before relying on it.
2. **`SetWhiteOverlay(1.0f)` + `Draw`.** The DLL export writes panel+`0x2BC`,
   which `Draw` passes to the shader at `0x1800BB5F0`. Whether this forces an
   opaque white quad independent of the bound texture's alpha is **unverified**;
   it is a candidate, not a proven primitive.
3. A text-only "panel" with no fill **fails** the hide requirement and is not
   an acceptable MVP fallback.

If neither candidate is proven opaque asset-free, Approach 1 is blocked and
the project must resolve Approach 2's suppression contract (§4) or explicitly
descope the hide requirement. Shared draw state (rect globals, color, white
overlay, clipping) is saved before and restored after the mod pass;
`SaveClipping`/`RestoreClipping` exports exist and are preferred over raw
clip-field writes.

### 3.5 Close and return-to-vanilla path

| Trigger | Detection | Action |
|---|---|---|
| Almanac button re-press | H1 pre-host edge inside button rect | Toggle `open=false`; consume latch; post-host draws nothing (vanilla already drawn by original). |
| Mod close control | H1 body-press routing | Same as above. |
| Native row press (any of 8, incl. repeat) | H3 at the four row Collision return sites | `nativeRowPress=true`; post-host closes. Native selection/store/sound already executed vanilla-style inside the original. |
| Section switch | Post-host `game+0xC650` pointer/name differs from captured trade identity | Close; no overlay. |
| Sidebar close / Esc | Post-host `game+0xFCE != 0` or `game+0xC650 == null`. The host's Esc/close handoff sets a local flag at `0x14031E662` and clears `C650` at `0x1403282F9` via `0x140328264`; the wrapper observes the result, never intercepts Esc. | Close. MVP semantics: Esc closes the whole sidebar (vanilla) and the Almanac with it. "Esc closes only Almanac" would require pre-host Esc interception with unproven ordering and is out of MVP scope. |
| World/save load, plugin stop | Mod lifecycle reset | Close; clear tracker; restore IAT slots; unpatch H1 only via the loader's verified restore path (no runtime unhook is claimed for MVP; restart-with-switch-off is the rollback). |

**Return guarantee.** While open, the only native writes are the consumed
latch byte and transient shared draw state that is saved/restored in the same
frame. The original host and both trade handlers run to completion every
frame, so native selection, outputs (`game+0xAE58`), cleanup, and state
updates are never skipped. On the first frame where `open` is false, the mod
draws nothing and the vanilla frame is already complete — there is no
"restoration" step that could fail. A fault in the mod draw pass triggers
fail-stop (close + disable mod drawing for the session) after restoring shared
draw state; it never leaves the overlay half-drawn over interactive native
content.

### 3.6 Native-state write budget

| Write | When | Justification |
|---|---|---|
| `input+0x301` latch ← 0 | Mod-owned press only | Same consumption the engine's own `GetMouseLeftPressAOff` performs; required for exclusive ownership. |
| Rect globals / panel color / white overlay / clipping | Mod draw pass, saved+restored same frame | Engine draw API inputs, not game logic. |
| H1 23-byte prologue patch + H2/H3 IAT slots | Install only | Verified span; compare-exchange slot swap; fail-stop on mismatch. |

**Never written:** `game+0xC65C`, `game+0xC650`, `game+0x5E6`,
`game+0x10BE8`, `game+0xAE58`, `game+0x10398/0x1039A`, the sub-entry vector,
or any content-helper output local.

### 3.7 Unresolved evidence specific to Approach 1

1. Opaque asset-free fill (§3.4 candidates 1–2): alpha/blend and full-body
   coverage at all DPI/scales.
2. Post-host engine batch state: whether `Draw`/`PrintAllTexts` after the
   host's final EndDraw-class call renders immediately or needs a
   `BeginDraw`/`EndDraw` pair; confirm the `0x86C8F8`/`0x86C870` import names.
3. Body rect authority: the captured background rect vs. the host's content
   area (`game+0xAE50/AE54/AE58`, `game+0xADB0` context) under both handlers,
   skips, and the single-button paths.
4. Row geometry under conditional skips and the normal handler's index
   remap: the ninth slot must follow the last **drawn** tile, not `count*64`.
5. Any native body input signal other than the latch (scroll, drag, keyboard)
   that mutates persistent state while covered.
6. `game+0xFCE` polarity and the trade-section identity check (pointer vs.
   name) on the live build.
7. Tooltip/other late host draws that may overlap the body rect after the last
   text flush; confirm the post-host pass covers them or accepts them on top.

## 4. Approach 2 — separate button + proven native-content suppression

The button, row observer, and close triggers are identical to Approach 1
(H1–H3); the difference is that native body content is **not drawn** while
open, instead of being covered. This removes the opaque-fill dependency and
eliminates hidden native hover/scroll rendering, but it requires a suppression
boundary whose output, input, and cleanup contract is proven for **both**
handlers. No such boundary exists in the current evidence.

### 4.1 Candidate interception points and why each is unproven

| Candidate | Site | Status |
|---|---|---|
| C1: inline patch at normal body start | `0x1403745B6` | **Rejected by user and by evidence:** 17 bytes spanning two RIP-relative instructions; the loader copies without relocation; three incoming routes share the continuation. Do not retry. |
| C2: replace the `FUN_14035F3E0` call | `0x140374606` (normal), `0x1403628B8` (alternate) | Covers only unsigned sel `<= 1` (alternate also requires `R15==0`). Indices 3/4/5/6/default draw through other calls. The caller consumes `AL` and reloads the output local `[RSP+0x70]` / `[RBP-0x20]`; a replacement must reproduce that contract. Insufficient alone (already concluded in `docs/almanac-content-boundary-review.md`). |
| C3: whole-handler entry detours + body skip | `0x140373EA0` steal 14; `0x140361BB0` steal 21 (§2.2) | Mechanically feasible trampolines. But an entry detour cannot skip the body without an internal branch/patch (C1/C2) or per-call IAT filtering (C4). Jumping straight to the cleanup tail passes uninitialized locals to four destructors (normal) or one (alternate); the early-exit paths show exactly which locals are not yet constructed. No safe internal jump target is proven. |
| C4: entry detours + TLS-flagged IAT filtering of body calls | Filter Draw/patch/Print/Collision/press imports by `_ReturnAddress()` inside body ranges | Direct internal calls (`0x1404B3230`, `0x1404B5D20`, `0x1404B6F40`, `0x1404ACF50`, `0x1404B0E20`, `0x1404B24A0`, `FUN_14035F3E0`, navigation helpers) are **not** IAT calls and would still run. Filtering only imports leaves native state mutations and some drawing intact; filtering everything requires an exhaustive call inventory that does not exist. Fragile and unproven. |
| C5: host dispatch redirect to a mod-owned handler | Patch `CALL` at `0x14031EDEF` / `0x14031EE13` | Bypasses the native row loop, selection stores, dirty flags, `0x10BE8` reset, and the `AE58` output contract unless all are replicated exactly. More invasive than Approach 1 and violates "preserve native behavior" until replication is proven. Not recommended. |

### 4.2 What a proven suppression design would have to specify

For each handler and each selection path (0/1, 3/4, 5, 6, default, and the
single-button `0xC7D4` path):

1. **Draw suppression:** every body draw/patch/text call is skipped or
   replaced, with no residual native quad/text under the mod content.
2. **Input suppression:** every body click/hover/scroll gate is inert while
   open, without consuming presses that belong to the row or the mod button.
3. **Required vanilla state updates preserved:** the `AE58` output (host
   recompute for normal, handler write for alternate), selection dirty flag
   `0x1039A`, `0x10BE8` reset, content-helper side effects (`0x10F20`,
   `0x5E6` toggles, navigation calls), and any per-frame simulation update the
   body performs. Each must be classified as *required* (replicate or let run)
   or *purely visual* (safe to skip), with evidence.
4. **Cleanup contract:** the normal tail's four destructors and the alternate
   tail's vector free run with correctly initialized locals on every suppressed
   path; no leak, no double-free, no uninitialized free.
5. **Close/return:** on close, the next native frame runs the full body with
   state consistent with vanilla. If suppression skipped a required update,
   the design must restore it explicitly; "it will refresh next frame" is not
   evidence.

### 4.3 Input ownership and close (if suppression were proven)

Row and button ownership are unchanged from §3.3 (H1 pre-host arbitration, H3
row-press observation). Body presses need no latch consumption because the
body gates are suppressed; the mod consumes presses inside its own content
rect only. Close triggers are §3.5. The return path is stronger than Approach 1
*if* §4.2 holds: there is no hidden native frame underneath, so no overlay
removal step. Until §4.2 holds, the return path is undefined.

### 4.4 Unresolved evidence specific to Approach 2

All of §3.7 except the opaque-fill item, plus: the complete body call/draw
inventory per selection path; the output-local and `AL` contracts at C2; the
initialized-local preconditions of every cleanup tail; which body side effects
are required vanilla state; and a safe internal suppression boundary that is
not C1. These are the facts listed in §7.

## 5. Comparison

| Criterion | Approach 1: overlay | Approach 2: suppression |
|---|---|---|
| Native code patched | H1 host prologue only (23 B, verified PI) | At least one internal body boundary per handler; none proven (C1 rejected) |
| Native handlers run | Fully, every frame, both selectors | Partially; skipped body must be contract-proven |
| Eight native buttons | Unchanged; H2/H3 passive | Unchanged; same observers |
| Hide native content | Depends on opaque-fill gate (§3.4) | Intrinsic if suppression is complete |
| Block native clicks | Latch consumption pre-host (proven same signal both handlers) | Intrinsic if body gates suppressed |
| Required vanilla updates | Preserved by running originals | Must be enumerated and replicated/let-run (§4.2) |
| Cleanup/leak risk | None added (originals run) | High until local-init preconditions proven |
| Close/return | Draw nothing; vanilla frame already complete | Undefined until §4.2 holds |
| Asset-free feasibility | Conditional on opaque fill | No fill needed, but boundary unproven |
| Least-invasive today | **Yes** | No |

## 6. Recommended MVP design (summary)

Build Approach 1 as a single plugin (`almanac_mvp`) that:

1. Installs H1 (host entry, 23-byte verified steal), H2 (Draw observer), H3
   (Collision row-press observer); resolves H4 flush imports by name. Fail-stop
   on any build/span/slot mismatch; no partial arming.
2. Keeps mod-owned state only: `open`, `consumed`, `wasDown`, RowTracker,
   captured trade identity, captured background recipe. No native game-field
   writes except the latch byte and transient saved/restored draw state.
3. Renders an **asset-free** UI: the ninth button is a text label ("Almanac")
   in the captured ninth-slot rect, optionally backed by the replayed native
   tile chrome if the opaque-fill gate passes; content is text rows plus
   native-style slider/checkbox primitives driven by the existing tax/finance
   services (or an explicit "unavailable" state). No PNG/ICO loading, no new
   text IDs below 1,000,000 if a private ID is minted.
4. Closes on the §3.5 triggers; never intercepts Esc; never writes selection.
5. Is gated by §8 before any implementation is considered ready.

This satisfies the acceptance criteria with the smallest proven native
footprint. Approach 2 remains the documented fallback if the opaque-fill gate
fails and the §7 suppression facts are later resolved.

## 7. Ghidra / static facts still needed (short, precise)

Ghidra is not present on this Linux host. These are serialized, read-only
queries for the Windows operator (or `objdump`/PE-parse equivalents where
noted). Each item states the exact range and the question; none authorizes a
hook, build, or game test.

| # | Range / target | Question | Blocks |
|---|---|---|---|
| F1 | EXE `0x1403743F2`–`0x1403745B6` and `0x1403622A2`–`0x1403624D3`; DLL `Draw` shader path | Which material/color/alpha is bound at the body-background Draw, and does replaying it over the full body rect produce an **opaque** asset-free cover? Alternatively, does `SetWhiteOverlay(1.0)` force opaque output? | §3.4 gate (Approach 1) |
| F2 | EXE PE import table (PE-parse, not Ghidra); host tail `0x14032C5DB`–`0x14032C8AF` (export is truncated before RET) | Confirm import names for slots `0x86C8F8` (EndDraw?) and `0x86C870` (PrintAllTexts?); confirm the host's final flush/EndDraw order and that a post-host Draw+flush renders in-frame. | §3.4 order, H4 |
| F3 | Host `0x14031E478`, `0x14031E4B2`, `0x14031E509`, `0x14031EDF4`–`0x14031EE02`; `game+0xADB0` setup | Authoritative body/content rect under both handlers, skips, and single-button paths; relationship of `AE50/AE54/AE58` to the drawn background rect. | §3.7.3 |
| F4 | Normal row loop `0x1403741A2`–`0x1403743E0`; alternate `0x140361FC0`–`0x140362245`; both single-button paths | Enumerate **every** row-tile Draw return site (incl. `0xC7D4` paths) and the last-drawn X under conditional skips, so the ninth slot follows the last drawn tile. | H2, §3.7.4 |
| F5 | Normal body `0x1403745B6`–`0x1403826CA`; alternate `0x14036252B`–`0x140373E2C` | Inventory body calls that read wheel/keyboard or mutate persistent native state while covered; classify required-vs-visual. | §3.3 scroll, §4.2 |
| F6 | Host `0x140317F2B`, `0x1403180D8`, `0x14031E662`→`0x140328264`→`0x1403282F9` | Confirm `game+0xFCE == 0` means sidebar visible and that Esc/close clears `C650` (or sets FCE) on every close path the wrapper must observe. | §3.5, §3.7.6 |
| F7 | `0x140374606` / `0x1403628B8` callers; cleanup tails `0x1403826CA` / `0x140373E2C`; early-exit paths | Output-local and `AL` contracts; initialized-local preconditions of each destructor; a safe internal suppression boundary that is **not** `0x1403745B6`. | Approach 2 only |
| F8 | Host caller `FUN_14028C770` at `0x140294092`; DLL writer of input+`0x301` (objdump) | Confirm the engine sets the latch **before** the host call and does not re-set it between H1 pre-host consumption and the trade handlers' reads. | §3.3 consumption validity |

F1–F6 and F8 block Approach 1 implementation readiness; F7 blocks Approach 2.
Items already resolved this session (ABI, latch identity, prologue spans,
dispatch/cleanup anchors) are **not** re-queried.

## 8. Acceptance / verification gates (future; none run here)

Static gates (before implementation is called ready):

- G1: F1 opaque-fill proof asset-free; G2: F2 import-name + post-host flush
  proof; G3: F3 body-rect authority; G4: F4 complete row Draw-site inventory;
  G5: F5 scroll/state classification; G6: F6 visibility/close polarity;
  G7: F8 latch-set ordering.
- G8: byte-compare the H1 host span and every filtered return address against
  the live build at install; fail-stop on mismatch. (If Approach 2 is later
  pursued, also byte-compare both handler spans from §2.2.)
- G9: ABI unit check that `GetMouseSolid` is called with `(this, out*)` and
  `Collision` is never called by the mod on a shared vector.

Live gates (require separate authorization; not performed):

- L1: all eight native tiles keep order, geometry, selection, sound, and
  repeat-click behavior under **both** selector values, with skips active.
- L2: ninth button opens/closes; one press = one toggle; hold does not repeat;
  latch-outlives-press does not double-toggle.
- L3: while open, native body is invisible and unclickable (incl. content
  helper sub-items); mod widgets receive presses; outside presses pass through.
- L4: native row press (incl. already-selected tile) closes and performs the
  vanilla store/sound; section switch and Esc/close restore vanilla.
- L5: no residual native-field writes after close (compare `C65C`, `C650`,
  `5E6`, `10BE8`, `AE58`, `10398/1039A` against a vanilla control session).
- L6: DPI/resolution/scale matrix; no overlap of the ninth slot with native
  tiles or sidebar edge; no leak across open/close cycles.

## 9. Evidence reproduced this session (bounded)

Source DLL: `tools/C3DDLL64.dll`, SHA-256
`65fcb6845c3cb7c25a22121f25904a855ea88657e31a3b50a15adac26e231516`.
Commands: `objdump -p` (export/ordinal tables) and `objdump -d
--start-address=… --stop-address=…` (bodies). Key results:

```text
GetMouseLeftDown   0x18004D330: 0F B6 81 00 03 00 00  movzbl 0x300(%rcx),%eax; c3 ret
GetMouseLeftPress  0x18004D340: 0F B6 81 01 03 00 00  movzbl 0x301(%rcx),%eax; c3 ret
GetMouseLeftPressAOff 0x18004D460: cmpb $0,0x301(%rcx); je -> xor al,al;
                     else movb $0,0x301(%rcx); mov $1,%al; ret
GetMouseSolid      0x18007CBA0: computes 3 floats into (%rdx),4(%rdx),8(%rdx);
                     mov %rdx,%rax; ret          ; sret out* in RDX
SetReallyRender(P) 0x1800BC660: mov %dl,0x6760(%rcx); ret
SetWhiteOverlay    0x18002DA50: movss %xmm1,0x2bc(%rcx); ret
Collision          0x1800BC690: reads +0x673C/6740/6744/674C/6734;
                     at 0x1800BC758 movss %xmm0,0x4(%rdx)  ; mutates mouse Y
Draw               0x1800BAF40: at 0x1800BB694 cmp %r13b,0x6760(%rdi) ; render gate
```

EXE anchors come from the tracked exports (preferred VAs):
`export/140317db0_full_disasm.asm` (host prologue, dispatch `0x31EDE6`–
`0x31EE18`, visibility `0x317F2B`/`0x3180D8`, close handoff `0x31E662`→
`0x328264`→`0x3282F9`, text flushes, truncated tail);
`export/140373ea0_1403829fd_disasm.asm` (normal prologue, row loop/gates,
content dispatch, cleanup `0x3826CA`);
`export/ch_tab23_full.asm` (alternate prologue, row loop/gates, content
dispatch, cleanup `0x373E2C`);
`export/almanac-evidence/pe-import-table-20261109/` (verified IAT names);
`export/almanac-evidence/fun-14035f3e0-20261108/14035f3e0_decomp.c`
(helper latch reads/consumption and side effects).

Corrections to prior session notes recorded here: the normal handler's first
RIP-relative instruction is at +0x6D, **outside** a 14-byte steal (the steal is
safe); `GetMouseLeftPress` is a non-consuming getter of the same latch byte the
normal handler compares directly (identity proven, not inferred);
`Collision` mutates its mouse-vector argument and panel fields, so it must not
be called speculatively by the mod.
