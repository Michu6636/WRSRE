# Almanac Button — Follow-Up Investigation (Static, v3.3 codebase)

Static investigation only. No source edits, builds, game launches, hooks,
deployments, save edits, or click tests. Ghidra queries run one at a time;
the `soviet_trade` project lock was not touched. C3DDLL64.dll was imported
into the `soviet_trade` Ghidra project to enable cross-module queries.

Source commit: `e949ff3` (the crashed build). Current worktree commit:
`09dc3e0` (crash fix — texture binding and text rendering removed).

Binary identity:
- SOVIET64.exe v1.1.1.9, PE TimeDateStamp `0x6A3EB6AD`, preferred image base
  `0x140000000`. Crash log runtime base: `0x7FF6F25D0000`.
- C3DDLL64.dll, crash log runtime base: `0x7FF956E90000`.
- Ghidra queries executed: DisasmMany on C3DDLL64.dll `0x1800D1780` and
  `0x1800B0190`; XrefsTo on C3DDLL64.dll `0x1800D1730` (FindElement);
  DumpBytes on SOVIET64.exe `0x1409DFF58` and `0x1409EACD0`/`0x1409EAD08`.
- Artifacts: `scratch/out/crash_0xD1780.asm`,
  `scratch/out/create_managed_texture.asm`,
  `scratch/out/xrefs_FindElement.txt`,
  `scratch/out/game_0x9DFF58.txt`,
  `scratch/out/dat_0x9EACD0.txt`,
  `scratch/out/dat_0x9EAD08.txt`.

---

## Item 1 — Crash attribution and texture-loader instance

### Facts

**Crash instruction** (`scratch/out/crash_0xD1780.asm`):
- Address: `C3DDLL64.dll + 0xD1780` (VA `0x1800D1780`, matches log `0x7FF956F61780`)
- Function: `FindElement@0x1800D1730`
- Instruction: `MOVZX EDX,byte ptr [RAX]` — reads a byte from RAX
- Read address: `0x7FF957082078` = `C3DDLL64.dll + 0x1F2078` (within C3DDLL64.dll itself)
- RDI at crash: `0x7FF6F2FAFF58` = `exeBase + 0x9DFF58`

**Call chain** (`scratch/out/xrefs_FindElement.txt`):
- `FindElement` is called from `CreateManagedTexture@0x1800B0190` at call site
  `0x1800B01A4` — 55 total callers, but `CreateManagedTexture` is the one on
  the plugin's path.
- `CreateManagedTexture` actual RVA = `0xB0190` (NOT `0x99`).

**`CreateManagedTexture` entry** (`scratch/out/create_managed_texture.asm`):
```
1800b019a:  48 8B 49 08  MOV RCX,qword ptr [RCX + 0x8]   ; RCX = *(middlepoint + 8)
1800b01a4:  E8 87 15 02 00 CALL 0x1800d1730                ; FindElement(RCX, ...)
```
- `RCX` (first arg, `middlepoint`) is dereferenced at offset `+8` before being
  passed to `FindElement`.
- If `FindElement` returns NULL, code at `0x1800B01B2` reads `*middlepoint`
  (`MOV RAX,qword ptr [RDI]`).

**`game + 0xB048` is not a valid address** (`scratch/out/game_0x9DFF58.txt`):
- `game + 0xB048` = RVA `0x9D4F10 + 0xB048` = `0x9DFF58`
- DumpBytes at `0x1409DFF58`-`0x1409DFFF` returned **empty** — not a mapped/
  readable address in the SOVIET64.exe image.
- Crash log RDI `0x7FF6F2FAFF58` = `exeBase + 0x9DFF58` confirms the plugin
  passed exactly this invalid address as `middlepoint`.

**`DAT_1409EACD0` is all zeros** (`scratch/out/dat_0x9EACD0.txt`):
- RVA `0x9EACD0`-`0x1409EAD00`: 48 bytes of `0x00`. Not a C3D_MIDDLEPOINT
  instance — treated as a lead, not an assumption, it is a dead end.

**`game + 0xEAD08` (panel state) is all zeros** (`scratch/out/dat_0x9EAD08.txt`):
- RVA `0x9EAD08`-`0x1409EAD18`: 16 bytes of `0x00`. Runtime-initialized global,
  zero at static analysis time.

### Inferences

**Root cause**: `DrawNinthTile` in `e949ff3` computes:
```cpp
void* middlepoint = (void*)(Base() + TSM_RVA_DEFAULT_GAME_OBJECT + 0xB048u);
```
= `exeBase + 0x9D4F10 + 0xB048` = `exeBase + 0x9DFF58`.

But `TSM_RVA_DEFAULT_GAME_OBJECT` (`0x9D4F10`) is the RVA of a **global pointer**
that holds the game object's address at runtime, not the game object itself.
The plugin adds `0xB048` to the **address of the global pointer**, not to the
game object. The correct computation would be:
```cpp
void* gameObj = *(void**)(Base() + TSM_RVA_DEFAULT_GAME_OBJECT);
void* middlepoint = (void*)((uintptr_t)gameObj + 0xB048);
```

`CreateManagedTexture` receives this invalid `middlepoint`, dereferences
`*(middlepoint + 8)` at `0x1800B01A0`, passes the result to `FindElement`,
which eventually computes `RAX = C3DDLL64.dll + 0x1F2078` and crashes on
`MOVZX EDX,byte ptr [RAX]` at `0x1800D1780`.

**RDI at crash** = `game + 0xB048` confirms `middlepoint` was the first argument
(RCX→RDI move at `0x1800B019A`) and survived into `FindElement`'s frame.

**The earlier "game was null at Start" explanation does NOT apply to `e949ff3`**:
In `e949ff3`, texture loading is done lazily in `DrawNinthTile` (not in
`Start()`), so `game` being uninitialized at `Start` is irrelevant. The crash
happens during the ninth-tile Draw pass, when `CreateManagedTexture` is called
with the bad `middlepoint`.

**Separate from the earlier Start-time crash**: The earlier crash (before
`09dc3e0`) was at `Start()` time from texture binding via `vtable[0x70]` on an
invalid texture object. This crash is different — it's inside
`CreateManagedTexture` called from `DrawNinthTile`, with `middlepoint =
game_global_addr + 0xB048`.

### Status: RESOLVED

Crash is attributed to `CreateManagedTexture` receiving an invalid
`middlepoint` (`exeBase + 0x9D4F10 + 0xB048` instead of
`*(exeBase + 0x9D4F10) + 0xB048`). The fault occurs inside `FindElement`
(called from `CreateManagedTexture` at `0x1800B01A4`) when it reads from
`RAX = C3DDLL64.dll + 0x1F2078`. `DAT_1409EACD0` is all zeros — not the
C3D_MIDDLEPOINT instance.

---

## Item 2 — Function ABI and addresses

### Facts

**`GetMouseSolid` mangled name** (`ui_panels.cpp:711-712`):
```
#define SYM_MOUSE_SOLID \
    "?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ"
```
- `?AVC3DVECTOR3@@` = return type `C3DVECTOR3` by reference (hidden sret pointer)
- `QEAA` = non-static, non-virtual member function
- `XZ` = no parameters beyond `this`

**Plugin typedef** (both `e949ff3` and `09dc3e0`):
```cpp
typedef float* (*t_GetMouseSolid)(void* input);
```
- Returns `float*` by value, takes only `input` — **WRONG ABI**.
- Missing the hidden result pointer parameter (passed in RDX under MSVC x64
  sret convention).

**Correct typedef** (`ui_panels.cpp:760`):
```cpp
typedef void  (*t_GetMouseSolid)(void* input, C3DVec3* out);
```
- Takes `(this, out*)` — `this` in RCX, hidden result pointer in RDX.
- Documented in ui_panels.cpp v6.0 as the ABI fix for WASD/mouse-camera/ESC
  corruption: "the old `typedef C3DVec3 (*t_GetMouseSolid)(void*)` returned
  the 12-byte struct BY VALUE, so MSVC passed nothing in RDX — the callee
  wrote 12 bytes of mouse vector through whatever garbage was in RDX."

**ExtSym results** (`scratch/out/extsym_texture.txt`, `extsym_font.txt`,
`extsym_input.txt`):
- Values such as `0x99` (CreateManagedTexture), `0x10E` (GetMouseSolid),
  `0x111` (PrintLeftUnicode), `0x1C0` (CreateManagedFontBitmap) are **Ghidra
  EXTERNAL symbol IDs**, NOT code RVAs.
- Actual `CreateManagedTexture` code RVA = `0xB0190` (confirmed by XrefsTo
  and DisasmMany).
- `CreateManagedFontBitmap` actual RVA = `0xB0F70` (from XrefsTo caller list).

**Texture/font creation signatures** (from C3DDLL64.dll disassembly):
- `CreateManagedTexture(middlepoint, filename)` — RCX=middlepoint, RDX=filename,
  confirmed by `CreateManagedTexture` entry disassembly.
- `CreateManagedFontBitmap(middlepoint, filename)` — same calling convention
  (calls `FindElement` at `0x1800B0F9B`).
- `PrintLeftUnicode(fontMgr, font, x, y, color, text)` — variadic, mangled
  name `?PrintLeftUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ`.

**`TexBind` vtable offset**:
- `ui_panels.cpp:598`: "vtable+0x70 bind" — confirmed by multiple sources.
- `almanac_button.cpp` (e949ff3): `#define ALM_TEX_BIND_OFFSET 0x70u`.
- The `C3DAPI_TEXTURE*` returned by `CreateManagedTexture` has an unknown
  vtable layout — offset `0x70` was assumed but caused a crash (see Item 3).

### Inferences

**GetMouseSolid ABI mismatch** is a real bug in both `e949ff3` and `09dc3e0`:
The plugin calls `g_getMouseSolid(input)` expecting a `float*` return, but the
native function expects `(this, out*)` with the output pointer in RDX. The
callee writes 12 bytes through whatever garbage is in RDX, corrupting
adjacent stack/registers. This is the same bug class fixed in ui_panels v6.0.
However, this corruption does NOT explain the `CreateManagedTexture` crash
(Item 1) — it's a separate latent defect.

**ExtSym symbol IDs vs code RVAs**: The values `0x99`, `0x10E`, `0x111`,
`0x1C0` reported in REPORT.md as "DLL RVAs" are incorrect — they are Ghidra
external symbol table indices. Actual code RVAs must be obtained via
DisasmMany/XrefsTo on the loaded program.

**`CreateManagedTexture` actual entry**: `0xB0190` (confirmed). The function
dereferences `*(middlepoint + 8)` before calling `FindElement`, which means
even a valid but wrong `middlepoint` (e.g., pointing to game data that isn't
a C3D_MIDDLEPOINT) would pass a derived pointer to `FindElement` and likely
crash there or later.

### Status: PARTIAL

- GetMouseSolid ABI: RESOLVED (WRONG in plugin, correct signature known from
  ui_panels.cpp v6.0).
- Texture/font creation signatures: RESOLVED (confirmed by disassembly).
- ExtSym vs code RVA confusion: RESOLVED (ExtSym IDs are not RVAs).
- `C3DAPI_TEXTURE` vtable layout for TexBind: UNRESOLVED — offset `0x70`
  assumed but caused crash; actual offset unknown.

---

## Item 3 — Remaining integration gates

### Facts

**Texture binding state** (`scratch/out/crash_0xD1780.asm`,
`scratch/out/create_managed_texture.asm`):
- `CreateManagedTexture` allocates `0x178` bytes per texture
  (`MOV ECX,0x178` at `0x1800B01BF`, `CALL 0x180167038`).
- `FindElement` is called to look up an existing texture by some key derived
  from `*(middlepoint + 8)`.
- `vtable[0x70]` bind was attempted in e949ff3's `DrawNinthTile` after the
  native Draw call — this caused the earlier Start-time/maintab crash and was
  removed in `09dc3e0`.
- The `C3DAPI_TEXTURE*` vtable layout is UNKNOWN — offset `0x70` is an
  assumption from `ui_panels.cpp` comment ("vtable+0x70 bind") but was never
  verified against the actual texture object type.

**Ninth-tile clipping** (from prior investigation, not re-run):
- `DrawFrom9Patch` (C3DDLL64.dll RVA `0xB99C0`) checks `byte ptr [RCX + 0x6760]`
  at entry — if 0, jumps to function return (no drawing).
- `DrawFrom3Patch` (C3DDLL64.dll RVA `0xB9680`) checks the same `+0x6760` flag.
- COMISS floating-point comparisons determine bounds clipping.
- Whether panel+0x6760 is set at runtime, and whether the ninth tile's rect
  falls within clipping bounds, is UNKNOWN statically.

**Read-only latch observation** (`game+0xA54E91` = click latch):
- The latch is a single byte set by the input decoder on confirm/click
  press-edge (`ui_panels.cpp:568-570`).
- Native code consumes the latch (`0xA54E91`) when processing clicks.
- Plugin reads `0xA54E91` read-only (does not zero it) in `CheckNinthClick`.
- Native code toggles `game+0x5E6` on certain clicks when latch is set
  (`FUN_14035f3e0` line 194 per `docs/ghidra-index/35F3E0.md`).

**Content suppression** (`docs/ghidra-index/35F3E0.md`):
- `FUN_14035f3e0` line 178-181:
  ```cpp
  if ((*(char *)(param_1 + 0x5e6) != '\0') &&
      (cVar5 = FUN_140335bc0(), cVar4 = '\0', cVar5 != '\0')) {
      cVar4 = '\x01';
  }
  if (cVar4 == '\0') { /* draw content */ }
  ```
- `game+0x5E6` is TOGGLED by native code on clicks:
  ```cpp
  *(bool *)(param_1 + 0x5e6) = *(char *)(param_1 + 0x5e6) == '\0';
  ```
  (when `cVar5 != '\0' && DAT_140a54e91 != '\0'`)
- `FUN_1403613b0` is the sub-tab click handler called by `FUN_14035f3e0`.
  Exists at `0x1403613B0` (x64 prologue confirmed via DumpBytes) but Ghidra
  has not created a proper function body (size=1, only function pointer
  references at `0x140A66D08`, `0x14094C3D0`, `0x14094C3E0`).
- `FUN_140335bc0` exists at `0x140335BC0` (x64 prologue confirmed) but Ghidra
  cannot disassemble it (size=1). Behavior unknown.

**Alternate handler path** (`FUN_140361BB0`):
- Used when `game+0x10398 == 0` (non-trade row).
- Separate from `FUN_140373EA0` (trade row, used by the Almanac button).

### Inferences

**TexBind ordering relative to Draw**: The native row handler binds texture
BEFORE Draw (`0x140374214`: `CALL [RAX+0x70]` with texture from
`[R13+0x48]`, then sets P2D rect, then calls Draw at `0x1403743A2`). If the
plugin binds a different texture before the ninth Draw, it changes the bound
texture state that subsequent native draws (overlay at `0x1403745B0`,
content at `0x140374606`) would use. The plugin would need to restore the
original binding after its Draw, but there's no verified save/restore
mechanism for texture state. The vtable offset `0x70` assumption caused a
crash — the actual `C3DAPI_TEXTURE` vtable layout is unknown.

**Ninth-tile clipping**: Cannot be established statically. `DrawFrom9Patch`/
`DrawFrom3Patch` perform clipping via `panel+0x6760` flag + COMISS bounds
checks, but whether the flag is set and whether the ninth tile's rect is
within bounds depends on runtime state. Left as UNRESOLVED.

**Read-only latch observation**: Can detect a first click (latch transitions
0→1 while mouse is over the ninth tile). May MISSED a re-click if the latch
was already set (native code may have set it) or if the plugin's read happens
before the native setter in the same frame. No clean observation point exists
at the click-write sites (no 14-byte whole-instruction boundary at
`0x140374356`/`0x14037435c`, XrefsTo total=0 for both).

**Content suppression via `game+0x5E6`**: BLOCKED. Requires writing native
state (`game+0x5E6 = 1` every frame when Almanac is open, restore to 0 when
closed). Issues:
1. Suppression requires `FUN_140335bc0() != 0` — behavior unknown (not
   decompilable with current Ghidra setup).
2. Native code toggles `game+0x5E6` on certain clicks (when latch is set),
   so the plugin's write may be immediately flipped back.
3. `cVar4` also controls tab open/close state rendering — side effects of
   writing `game+0x5E6` beyond content suppression are unknown.
4. No clean hook point at `FUN_14035f3e0` call site (`0x140374606`) — no
   14-byte whole-instruction boundary.

**`FUN_1403613B0` navigation/cleanup**: Cannot be analyzed statically — Ghidra
has not created a function body (size=1, only function pointer references).
Cannot determine if calling it with a synthetic index would cause faults.

**Alternate handler path** (`FUN_140361BB0`): Not used by the Almanac button
(trade row uses `FUN_140373EA0`). No impact on the Almanac feature.

### Status: BLOCKED

- Texture binding via `C3DAPI_TEXTURE` vtable: BLOCKED — vtable layout
  unknown, offset `0x70` assumption caused crash.
- Ninth-tile clipping: UNRESOLVED — runtime-dependent.
- Read-only latch observation: PARTIAL — can detect first click, may miss
  re-clicks, no clean observation point.
- Content suppression via `game+0x5E6`: BLOCKED — requires native state write,
  depends on unknown `FUN_140335bc0`, native toggling may flip it back.
- `FUN_1403613B0`/`FUN_140335BC0`: BLOCKED — not decompilable with current
  Ghidra setup.
- Alternate handler path: NOT APPLICABLE — trade row uses `FUN_140373EA0`.

---

## Summary

| Item | Status | Key evidence |
|------|--------|--------------|
| 1. Crash attribution | RESOLVED | `middlepoint = exeBase + 0x9D4F10 + 0xB048` is invalid (adds offset to global pointer address, not dereferenced game object). `CreateManagedTexture` at `0x1800B0190` dereferences `*(middlepoint+8)` then calls `FindElement` which crashes reading `C3DDLL64.dll + 0x1F2078`. `DAT_1409EACD0` is all zeros — not the C3D_MIDDLEPOINT. |
| 2a. GetMouseSolid ABI | RESOLVED | Plugin typedef `float* (*)(void*)` is WRONG. Native is `void (*)(void*, C3DVec3* out)` with hidden sret pointer in RDX (confirmed by mangled name and ui_panels.cpp v6.0). |
| 2b. ExtSym vs code RVAs | RESOLVED | `0x99`, `0x10E`, `0x111`, `0x1C0` are Ghidra external symbol IDs, not code RVAs. Actual `CreateManagedTexture` RVA = `0xB0190`. |
| 2c. Texture/font signatures | RESOLVED | Confirmed by disassembly: `CreateManagedTexture(middlepoint, filename)`, `CreateManagedFontBitmap(middlepoint, filename)`. |
| 3a. TexBind/Draw ordering | BLOCKED | `C3DAPI_TEXTURE` vtable layout unknown; offset `0x70` assumption crashed. No verified save/restore of texture state. |
| 3b. Ninth-tile clipping | UNRESOLVED | `DrawFrom9Patch`/`3Patch` check `panel+0x6760` + COMISS bounds; runtime-dependent. |
| 3c. Read-only latch observation | PARTIAL | Can detect first click; may miss re-clicks; no clean observation point (XrefsTo total=0 at click-write sites). |
| 3d. Content suppression | BLOCKED | Requires writing `game+0x5E6`; depends on unknown `FUN_140335bc0`; native toggling may flip it back; `cVar4` side effects unknown. |
| 3e. FUN_1403613B0/335BC0 | BLOCKED | Not decompilable with current Ghidra setup (size=1, function pointer refs only). |
| 3f. Alternate handler path | NOT APPLICABLE | Trade row uses `FUN_140373EA0`, not `FUN_140361BB0`. |

### Recommended next gate (static only)

Resolve the `middlepoint` computation: the plugin must dereference
`*(void**)(Base() + TSM_RVA_DEFAULT_GAME_OBJECT)` to get the actual game
object, then add `0xB048`. Verify what `game + 0xB048` actually contains at
runtime (it may be a valid C3D_MIDDLEPOINT or something else entirely). Until
this is resolved, any texture loading via `CreateManagedTexture` will crash.
