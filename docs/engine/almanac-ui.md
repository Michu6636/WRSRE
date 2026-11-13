# Almanac UI — Native Evidence

Native reverse-engineering findings for the Almanac feature's use of the
Economy & Trade page handler and C3DDLL64.dll texture/font exports.
SOVIET64.exe v1.1.1.9, PE TimeDateStamp 0x6A3EB6AD, preferred image base
0x140000000. Addresses are preferred-image VAs unless labelled otherwise.
Raw excerpts (git-tracked): docs/ghidra-exports/almanac-ui/.

## 1. UI handlers
### 1.1 FUN_140373EA0 — Economy & Trade page handler

- RVA: 0x373EA0. Preferred VA 0x140373EA0. Size 55,231 bytes.
- Caller: FUN_140317DB0 at 0x14031EDEF, inside the "trade" name-match branch,
  gated by game+0x10398 != 0.
- Purpose: draws the Economy & Trade page: section-button row (8 textures:
  trade_importexport, trade_productionconsumption, trade_tourism, trade_crime,
  trade_loanrub, trade_loanusd, trade_prices, trade_resources) plus selected
  sub-page content.
- Sub-entry vector at *(game+0xC650)+0x48/+0x50, stride 0x50.
- Reads game+0xC650 (selected panel entry), game+0xC65C (selected sub-tab
  index), game+0x12800 (content list), game+0x1039a (dirty flag),
  game+0x10be8 (counter). Writes game+0xC65C=EBX, game+0x10be8=-1,
  game+0x1039a=0 if selection changed. Plays gui_subtab_click sound.
- Button backdrop 64x64 (DPI * DAT_0x90AAA0 = 64.0). X origin 130
  (DPI*(45+85)). Pitch 64 (DPI*64.0), no overlap. Texture PNGs on disk 96x96
  source vs 64x64 rendered backdrop.
- Loop has special index mapping (EDI 0/1 -> EBX=EDI; EDI 2 -> EBX=count-1;
  else EBX=EDI-1): layout is not a simple linear row.
- Conditional skips (per independent review): nonzero game+0x578 skips mapped
  indices 3 and 4; index 6 skipped if game+0x5e0 is zero. Skipped entries
  bypass the X advance at 0x1403743A8. Drawn order for 4 entries: [0,1,3,2].
- Both handlers test game+0xC7D4 and branch away from the regular vector-row
  loop when it is not -1 (JNZ at 0x14037413A for the game+0xC7D4 test; the
  JLE at 0x14037419C is the zero-count skip). The JMP at 0x1403743ED targets
  the start of the site.
- game+0x10BE8 is reset to -1 BEFORE the row loop (0x14037403A), independent
  of any click: not a click signal.
- Click path: collision tested 0x1403742A4-0x1403742AC; on hit a UTF-16
  string-copy loop 0x1403742D0-0x1403742E2 exits on CX==0; then
  CMP [0x140A54E91], CL at 0x1403742F2 (CL is 0 here). Selected-index write
  and -1 write at 0x140374356/0x14037435C occur even if already selected;
  game+0x1039A write at 0x14037434F is conditional on change. Sound conditional.
- Raw excerpt: docs/ghidra-exports/almanac-ui/373EA0_excerpt_1.asm (lines 125-290
  of scratch/out/140373ea0_1403829fd_disasm.asm); full disasm 751,146 bytes
  (local-only, not tracked).
- Status: partial (Y origin, container width/clipping, reordering, close path,
  latch owner, runtime selected-entry identity unresolved).
### 1.2 FUN_14035F3E0 — content sub-view

- RVA: 0x35F3E0. Preferred VA 0x14035F3E0.
- Signature: char FUN_14035f3e0(game, float *param_2, float *param_3) — returns
  '\\x01' if a specific sub-item was clicked, '\\x00' otherwise.
- Caller: FUN_140373EA0 at 0x140374606 when game+0xC65C <= 1.
- Purpose: draws a content sub-view with 6 clickable sub-items (C3D_LANGUAGE
  string IDs 0x68B-0x690, or 0x2B6 for index 5). Each sub-item has a label
  (PrintLeftUnicode), a 9-patch button (DrawFrom9Patch), and click handling
  (GetMouseSolid + Collision + latch). Calls FUN_1403613B0 (sub-page switch)
  and FUN_140335B10 (state setter) on click.
- Data refs: reads game+0x10F20 (sub-page state), game+0x5E6 (toggle),
  game+0x14 (live mode). Writes game+0x10F20 = 4 on click. Uses
  game+0xB028/B038 (widget pointers), game+0xB048 (font), game+0xB4A0
  (drawable).
- Raw excerpt: docs/ghidra-exports/almanac-ui/35F3E0_excerpt.c (first 80 lines
  of scratch/out/14035f3e0_decomp.c, 31,659 bytes local-only).
- Status: partial. On click calls FUN_1403613B0 (sub-page switch) and FUN_140335B10
  (state setter); the content-suppression path additionally depends on
  FUN_140335BC0 (predicate, not decompilable). FUN_1403613B0 and FUN_140335BC0
  are not decompilable with current Ghidra setup (size=1, function pointer refs
  only). FUN_140335B10 behavior also unverified here.

### 1.3 FUN_140361BB0 — alternate handler

- RVA: 0x361BB0. Preferred VA 0x140361BB0.
- Caller: FUN_140317DB0 at 0x14031EDAF when game+0x10398 == 0 (non-trade row);
  R9D is zeroed immediately before the call (host path).
- Purpose: alternate row handler with its own vector-row loop and conditional
  skips applied to the linear iteration index; loop bound is calculated
  sub-entry count minus one.
- Conditional gate: tests game+0xC7D4 and branches away from the regular loop
  when it is not -1. On the host path R9D=0 so the nonzero branch is not
  taken at this call site if reached.
- Click path: after successful collision check, indirect call through
  0x14086C8B8 (reported C3DDLL64.DLL::C3D_INPUT::GetMouseLeftPress) at
  0x14036213B returns a value tested at 0x140362141; zero branches around the
  selection and -1 writes. Direct byte at 0x140A54E91 is 0x301 bytes after the
  0x140A54B90 address passed in RCX to that indirect call (candidate
  relationship, not proof).
- Also resets game+0x10BE8 to -1 before its normal row at 0x140361CBB.
- Raw artifact: export/ch_tab23_full.asm (historical, not tracked here);
  scratch/out/140361bb0_64_disasm.asm, scratch/out/140361bb0_decomp.c (local-only).
- Status: partial.

## 2. Plugin exports used

### 2.1 CreateManagedTexture / FindElement

- IAT slot: 0x14086C4C0 imports CreateManagedTexture from C3DDLL64.dll.
  Slot bytes (static): 20 8E 98 00 00 00 00 00 = 0x140988E20 (name thunk).
  At runtime the loader replaces the name thunk with the actual function
  (C3DDLL64.dll RVA 0xB0190).
- CreateManagedTexture entry: C3DDLL64.dll RVA 0xB0190.
  ABI: RCX = middlepoint (C3D_MIDDLEPOINT*), RDX = filename (char*).
  Body: RDI = RCX (middlepoint); RSI = RDX (filename); RCX = *(middlepoint+8);
  CALL FindElement(RCX). If non-null, returns existing texture; else allocates
  0x178 bytes and initializes.
- FindElement: C3DDLL64.dll RVA 0xD1730. Receives RCX = *(middlepoint+8) which is a
  container pointer (begin/end at +0/+8 within it); RDX = filename (the key).
  Linear scan: R11 = [RCX] (container begin), RCX = [RCX+8] (container end);
  count = (end-begin)/0x88 (stride per entry); loops index 0..count-1, computing
  entry = begin + index*0x88, comparing the filename string against each entry
  (INC R9D). Crashes with MOVZX EDX, byte ptr [RAX] at 0x1800D1780 when the
  entry pointer computes past the end of C3DDLL64.dll.
- middlepoint MUST be a valid C3D_MIDDLEPOINT*. The plugin's wrong computation
  (exeBase + 0x9D4F10 + 0xB048) dereferences unrelated .data and crashes.
  Correct middlepoint is Base() + 0x9EACD0.
- Raw excerpts: docs/ghidra-exports/almanac-ui/create_managed_texture.asm,
  docs/ghidra-exports/almanac-ui/find_element_full.asm.

### 2.2 Texture vtable offsets (Load / TexBind)

- Object returned by CreateManagedTexture is C3DAPI_TEXTURE*.
- vtable+0x10 = Load(filename, 0, 0) — loads image data from file.
- vtable+0x70 = TexBind(texture, 0, panel_state) — binds texture for drawing.
  panel_state = DAT_1409EAD08 (RVA 0x9EAD08, all zeros in static image,
  runtime-initialized).
- Native sequence (from decompilation): Create -> store at sub_entry+0x48 ->
  vtable+0x10 Load -> later vtable+0x70 TexBind.
- The plugin's v3.0 called vtable+0x70 WITHOUT vtable+0x10 first; and used
  vtable offset 0x70 as an assumption that caused a crash. Offsets are inferred
  from decompilation, not verified against the actual C3DAPI_TEXTURE vtable in
  C3DDLL64.dll. Status: unresolved.

### 2.3 CreateManagedFontBitmap

- C3DDLL64.dll RVA 0xB0F70 (from XrefsTo caller list). Same ABI as
  CreateManagedTexture: RCX = middlepoint, RDX = filename. Calls FindElement at
  0x1800B0F9B.

### 2.4 GetMouseSolid ABI

- Mangled name: ?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ.
  ?AVC3DVECTOR3@@ = return by reference (hidden sret pointer).
  QEAA = non-static non-virtual member. XZ = no params beyond this.
- Correct ABI: void (*)(void* input, C3DVec3* out) — this in RCX, hidden
  result pointer in RDX (MSVC x64 sret).
- Plugin typedef float* (*)(void*) is WRONG (missing RDX). Same bug class
  fixed in ui_panels v6.0. Corrupts adjacent stack/registers. Latent defect
   independent of the CreateManagedTexture crash.

## 3. middlepoint resolution

### 3.1 IAT slot 0x14086C4C0

- Preferred VA 0x14086C4C0. Static bytes: 20 8E 98 00 00 00 00 00.
- Points to 0x140988E20 (name thunk). At 0x140988E20: ASCII "CreateManagedTexture"
  (import name table entry for C3DDLL64.dll). At runtime the loader replaces
  the name thunk with the actual function address (C3DDLL64.dll RVA 0xB0190).

### 3.2 Callsite 0x1403827E2

- Preferred VA 0x1403827E2, inside a function starting at 0x140382760.
- Bytes: 48 8D 0D E7 84 66 00  LEA RCX,[0x1409EACD0]
  FF 15 D1 9C 4E 00        CALL QWORD PTR [0x14086C4C0]
- RCX = 0x1409EACD0 passed as first arg (middlepoint) to CreateManagedTexture.
- Ghidra has not analyzed this function (FuncInfo size=0, XrefsTo total=0).
- Raw excerpt: docs/ghidra-exports/almanac-ui/callsite_0x1403827C0.txt (byte dump).
- Related index entry: docs/ghidra-index/382760.md.

### 3.3 DAT_1409EACD0

- Preferred VA 0x1409EACD0. Tracked excerpt (docs/ghidra-exports/almanac-ui/dat_0x9EACD0.txt)
  covers 0x1409EACD0-0x1409EACFF: 48 bytes all 0x00. Local-only scratch/out
  reports 256 bytes all 0x00 starting at 0x1409EAC00, but only the 48-byte
  range is tracked/verified here. (BSS/zero-initialized global.)
- Classification: global C3D_MIDDLEPOINT instance, confirmed by decompilation
  casting &DAT_1409EACD0 as C3D_MIDDLEPOINT*. Used as middlepoint in 30+
  native call sites (LEA RCX,[0x1409EACD0] before CreateManagedTexture).
- At runtime an initialization function populates this structure before
  texture-loading code runs; static image is zero. Whether 0x1409EACD0 is valid
  when the plugin's DrawNinthTile runs is not established statically.
- The native vtable offsets (0x10 Load, 0x70 TexBind) are inferred from
  decompilation, not verified against the actual C3DAPI_TEXTURE vtable.
- game+0xB048 (in FUN_14035F3E0) is a font field (C3D_FONT*), NOT a
  C3D_MIDDLEPOINT. The plugin's computation exeBase + 0x9D4F10 + 0xB048 is
  wrong on two counts: it adds 0xB048 to the global pointer's own address
  instead of dereferencing, and even dereferenced game+0xB048 is a font field.
- Correct middlepoint: Base() + 0x9EACD0 (compile-time constant, no runtime
  dereference).
- Raw excerpt: docs/ghidra-exports/almanac-ui/dat_0x9EACD0.txt.

## 4. Hook point 0x1403745B6

- Preferred VA 0x1403745B6, inside FUN_140373EA0.
- First two instructions occupy 9 and 8 bytes and use RIP-relative addresses.
  InstallInlineHook in src/tesmioloader.cpp supports a 17-byte stolen region
  but copies bytes to a trampoline without relocating operands: cannot blindly
  trampoline these instructions.
- Both the JLE at 0x14037419C and the JMP at 0x1403743ED target the START of
  the site; the alternate path falls through after 0x1403745B0. Entry at the
  start is not inherently a jump into the middle of a patch, but every incoming
  route and interior targets must be assessed.
- Safe live-register preservation, stack/cleanup and output handling, skipped
  state effects, and the alternate handler are unverified. The 373EA0 tail calls
  helpers with four stack-local addresses before restoring registers and
  returning; [RBP+0x50] is initialized before the row, [RBP-0x70], [RBP+0x510],
  [RBP+0x90] are initialized after the proposed interception point. Jumping
  from post-row interception directly to full cleanup would pass later,
  uninitialized locals to those helpers.
- Status: unsafe (per corrections: hook point 0x1403745B6 is unsafe for the
  17-byte loader). No hook approved or specified.

## 5. Claim Ledger

| Claim ID | Statement | Raw artifact | Status | Limit |
|----------|-----------|--------------|--------|-------|
| C1 | FUN_140373EA0 is the Economy & Trade page handler | docs/ghidra-index/373EA0.md, 317DB0.md | supported | gated by game+0x10398 != 0 |
| C2 | game+0x10BE8 reset to -1 is not a click signal | almanac-export-review.md sec.2 | supported | static only |
| C3 | Conditional row skips exist in both handlers | almanac-export-review.md sec.1 | supported | runtime flags unknown |
| C4 | CreateManagedTexture middlepoint must be Base()+0x9EACD0 | FOLLOW-UP.md Item 4 | supported | runtime init of 0x9EACD0 unresolved |
| C5 | Plugin middlepoint computation is wrong | FOLLOW-UP.md Item 1,5 | supported | crash repro confirmed |
| C6 | GetMouseSolid ABI is void(*)(void*,C3DVec3*) | FOLLOW-UP.md Item 2 | supported | from mangled name + ui_panels.cpp |
| C7 | vtable+0x10=Load, +0x70=TexBind | FOLLOW-UP.md Item 4 | contested | inferred, not verified vs vtable |
| C8 | 0x1403745B6 unsafe for 17-byte loader | almanac-export-review.md sec.4 | supported | no hook approved |
| C9 | FUN_140335BC0/FUN_1403613B0 not decompilable | docs/ghidra-index/335BC0.md, 3613B0.md | unresolved | not decompilable |
| C10 | ninth-tile clipping | FOLLOW-UP.md Item 3 | unresolved | runtime-dependent |
| C11 | IAT slot 0x14086C4C0 imports CreateManagedTexture | docs/ghidra-exports/almanac-ui/callsite_0x1403827C0.txt | supported | static bytes only |
| C12 | Callsite 0x1403827E2 passes &DAT_1409EACD0 | docs/ghidra-exports/almanac-ui/callsite_0x1403827C0.txt | supported | function body unanalyzed |
| C13 | DAT_1409EACD0 is 48 zero bytes in static image | docs/ghidra-exports/almanac-ui/dat_0x9EACD0.txt | supported | runtime-initialized |
| C14 | game+0xB048 is a font field, not middlepoint | FOLLOW-UP.md Item 4 | supported | decompilation cast |
| C15 | Button backdrop 64x64, X origin 130, pitch 64 | docs/ghidra-index/373EA0.md | supported | DPI-scaled |
| C16 | Conditional row skips (game+0x578, game+0x5E0) | almanac-export-review.md sec.1 | supported | runtime flags unknown |
| C17 | game+0x10BE8 reset before row loop | almanac-export-review.md sec.2 | supported | static only |
| C18 | Click path writes index/-1 even if already selected | almanac-export-review.md sec.3 | supported | static only |
| C19 | CreateManagedFontBitmap RVA 0xB0F70 | FOLLOW-UP.md Item 2 | supported | from XrefsTo caller list |
| C20 | GetMouseSolid ABI mismatch in plugin | FOLLOW-UP.md Item 2 | supported | mangled name + ui_panels.cpp |
| C21 | FUN_14035F3E0 returns click flag, 6 sub-items | docs/ghidra-index/35F3E0.md | supported | partial |
| C22 | FUN_140361BB0 alternate handler, R9D=0 host path | docs/ghidra-index/361BB0.md | supported | static call-site reasoning |