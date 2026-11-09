# Almanac: call-site comparison at 0x140374606 (FUN_14035f3e0 vs no-call)

Static analysis only. No Ghidra rerun; all evidence from existing artifacts.
No hooks, debugger, game changes, commit, or push.

Sources (existing, non-ignored):
- `scratch/out/14035f3e0_decomp.c` → preserved at
  `export/almanac-evidence/fun-14035f3e0-20261108/14035f3e0_decomp.c`
- `docs/ghidra-index/35F3E0.md` → preserved at
  `export/almanac-evidence/fun-14035f3e0-20261108/35F3E0.md`
- `export/140373ea0_1403829fd_disasm.asm` (handler call site)
- `export/140317db0_full_disasm.asm` (host output consumption)

## 1. Call site in handler `FUN_140373EA0`

The call at `0x140374606` is reached only when `game+0xC65C <= 1` (selection 0 or 1):

```
0x1403745f1: CMP dword ptr [RSI + 0xc65c],0x1    ; selection vs 1
0x1403745f8: JA 0x14037461b                       ; if selection > 1, skip call entirely
0x1403745fa: LEA R8,[RSP + 0x70]                  ; param_3 = &output_Y
0x1403745ff: LEA RDX,[RBP + -0x4c]                ; param_2 = &output_X
0x140374603: MOV RCX,RSI                          ; param_1 = game pointer
0x140374606: CALL 0x14035f3e0                     ; FUN_14035f3e0(game, &output_X, &output_Y)
0x14037460b: MOVSS XMM10,dword ptr [RSP + 0x70]   ; XMM10 = *output_Y (post-call)
0x140374612: TEST AL,AL                           ; AL = cVar4 return value
0x140374614: MOV byte ptr [RSP + 0x40],0x0        ; [RSP+0x40] = 0 (tentative)
0x140374619: JNZ 0x140374620                      ; if cVar4 != 0, skip line below
0x14037461b: MOV byte ptr [RSP + 0x40],0x1        ; if cVar4 == 0, [RSP+0x40] = 1
0x140374620: MOV R13D,dword ptr [RSI + 0xc7dc]    ; converge point
```

Argument mapping:
- `param_2` = RDX = `[RBP - 0x4c]` → output-X pointer (saved at handler entry `0x140374050`)
- `param_3` = R8 = `[RSP + 0x70]` → output-Y pointer
- `param_1` = RCX = RSI = game pointer

## 2. Recovered decompilation excerpts from FUN_14035f3e0

### Draw / input conditions and writes to param_2 / param_3

**Reads of both output locals (decomp lines 53–54):**
```
  fVar24 = *param_2;          // line 53  — reads output-X, NEVER writes it
  fVar27 = *param_3;          // line 54  — reads output-Y into fVar27
```

**cVar4 computation — controls whether input handling runs (decomp lines 177–181):**
```
  cVar4 = '\0';
  if ((*(char *)(param_1 + 0x5e6) != '\0') &&
     (cVar5 = FUN_140335bc0(), fVar25 = DAT_140992088, cVar4 = '\0', cVar5 != '\0')) {
    cVar4 = '\x01';
  }
```
`cVar4 == 1` only when BOTH `game+0x5E6 != 0` AND `FUN_140335bc0()` returns nonzero.

**Alias of output-Y pointer (decomp line 63):**
```
  local_110 = param_3;
```

**Input handling gated on cVar4 (decomp lines 224–242):**
```
  if (cVar4 == '\0') {                              // line 224 — input only if cVar4==0
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14035fcfc;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                               ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                              ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xffffffff,1,local_148);   ; sub-page switch
      *(undefined4 *)(param_1 + 0x10f20) = 4;            ; game+0x10F20 = 4
    }
  }
  else { LAB_14035fcfc: ... }                       // line 244 — skips input handling
```

**Write to param_3 (output-Y) and the single return (decomp lines 63, 732–733):**
```
  local_110 = param_3;                              // line 63  — alias output-Y
  ...
  *local_110 = DAT_140992088 * DAT_14090aa28 + fVar27;  // line 732 — WRITES output-Y
  return cVar4;                                        // line 733 — ONLY return statement
```

`param_2` (output-X) is read at line 53 but **never written** anywhere in the body.
`param_3` (output-Y) is written at line 732: `*param_3 = scale * DAT_14090aa28 + original_Y`.

### All return statements in FUN_14035f3e0

Scanned the complete 737-line decompilation. **Exactly one `return` statement exists**, at line 733:

```
  return cVar4;
```

No other `return` appears. Every other control path falls through to lines 728–733
(including `LAB_1403612ee` at line 728, then line 729 draw, lines 730–733 return).
There is no early return that skips drawing: the function always executes the backdrop
draw (line 74) and the 6-sub-item drawing loop (lines 87–152) before reaching the
single return.

## 3. Path A vs Path B — immediate consequences

### Path A: existing path — CALL FUN_14035f3e0 (selection 0 or 1)

| Step | Address / decomp line | Effect |
|---|---|---|
| Call | `0x140374606` | Enters FUN_14035f3e0 |
| output-Y modified | decomp line 732 | `*param_3 = scale * DAT_14090aa28 + original_Y` |
| output-X | decomp line 53 only | Read, never written → **unchanged** |
| XMM10 reload | `0x14037460b` | `XMM10 = [RSP+0x70] = *output_Y` = NEW value |
| Return value | decomp line 733 | `AL = cVar4` |
| [RSP+0x40] flag | `0x140374614`–`0x140374619` | `(cVar4 == 0) ? 1 : 0` |

Resulting `*output_Y` (using snapshot values: scale=0.8, DAT_14090aa28=45,
pre-call Y_base+scale*35=196.0):
```
*output_Y = 0.8 * 45 + 196.0 = 36.0 + 196.0 = 232.0
```

### Path B: hypothetical — skip the call while Almanac page active

If execution jumps from `0x140374603` directly to `0x14037460b` (skipping the CALL):

| Step | Address | Effect |
|---|---|---|
| Call | `0x140374606` | **Skipped** — FUN_14035f3e0 not entered |
| output-Y | — | **NOT modified** → stays `Y_base + scale*35` = 196.0 |
| output-X | — | **Unchanged** (same as path A) |
| XMM10 reload | `0x14037460b` | `XMM10 = [RSP+0x70]` = PRE-CALL value = 196.0 |
| Return value | — | `AL` **undefined** (no call made) |
| [RSP+0x40] flag | `0x140374614`–`0x140374619` | Depends on undefined AL → **must be set manually** |

### Immediate difference (output-Y)

| Quantity | Path A | Path B | Delta |
|---|---|---|---|
| `*output_Y` | `scale * DAT_14090aa28 + Y_base + scale*35` | `Y_base + scale*35` | `scale * DAT_14090aa28` = **36.0** |
| `XMM10` at `0x14037460b` | 232.0 | 196.0 | 36.0 |
| `[RSP+0x40]` | `(cVar4==0)?1:0` | undefined (manual) | unknown |

Both paths then converge at `0x140374620` (`MOV R13D,[RSI+0xc7dc]`) and proceed identically:
- `0x140374627`: `XOR AL,AL`; `0x140374629`: `[RBP-0x54] = 0` (same both paths)
- Various locals zeroed (`0x140374634`–`0x140374646`)
- `[RBP-0x70]` zeroed at `0x14037464f` (cleanup local, initialized)
- Eventually reach content-drawing calls (`0x140374acc`, `0x140374b35`, `0x140374c25`)
- Output write at `0x140374c46` (`*output_X = [RBP-0x4c]`) and `0x140374c54` (`*output_Y = [RSP+0x70]`)
- Host consumption at `0x14031edf4–0x14031ee02`: `game+0xae58 = *output_Y - game+0xae50`
  → differs between paths by **36.0** (snapshot values)

## 4. What would need separate handling (path B)

1. **output-Y advancement.** Suppressing the call means `*output_Y` is NOT advanced by
   `scale * DAT_14090aa28` (36.0 snapshot units). If the Almanac page uses its own
   vertical layout, `*output_Y` / `[RSP+0x70]` / `XMM10` must be set explicitly at the
   skip point to match expected positioning.

2. **[RSP+0x40] flag.** In path B, `AL` is undefined (no call), so the sequence at
   `0x140374614`–`0x140374619` (`[RSP+0x40] = (cVar4==0)?1:0`) cannot produce a
   defined result. `[RSP+0x40]` gates click handling at `0x140374d6a` (`CMP byte
   [RSP+0x40],0; JZ 0x140375010`). Path B must set `[RSP+0x40]` manually:
   - `1` → native click handling proceeds (may conflict with Almanac input)
   - `0` → click handling skipped (Almanac handles its own input)

3. **XMM10.** Reloaded from `[RSP+0x70]` at `0x14037460b`; in path B it carries the
   pre-call value. Any downstream use of XMM10 before it is overwritten must account
   for this.

4. **Content-drawing calls.** Even if FUN_14035f3e0 is skipped, the vanilla content
   draws at `0x140374acc` (sub-tab 0–4), `0x140374b35` (sub-tab 5), `0x140374c25`
   (default) still execute unless separately suppressed. Full Almanac-page substitution
   likely requires skipping these too, plus suppressing the post-loop panel draw.

## 5. What remains unknown

| Unknown | Why |
|---|---|
| Correct `[RSP+0x40]` value for path B | Depends on whether native click handling should coexist with Almanac input; not derivable from static export |
| Whether skipping content-drawing calls is required for full suppression | Their bodies (`0x1404b3230`, `0x1404b5d20`, `0x140335cc0`) not examined |
| Almanac page coordinate system / expected `*output_Y` | No mod-owned drawing code defined |
| Whether output-X needs adjustment | FUN_14035f3e0 doesn't touch it, but Almanac layout may |
| `game+0xC65C` / `game+0x10be8` interaction | FUN_14035f3e0 doesn't write them, but the click block sets `game+0x10F20=4` |

## 6. Fact vs assumption

**Facts (from decompilation + handler disasm):**
- Call at `0x140374606`, gated on `game+0xC65C <= 1` (`0x1403745f1`–`0x1403745f8`)
- `param_2 = &output_X`, `param_3 = &output_Y`; `param_3` written at decomp line 732,
  `param_2` only read (line 53)
- Single `return` at decomp line 733 (`return cVar4`)
- `cVar4 == 1` iff `game+0x5E6 != 0 && FUN_140335bc0() != 0` (decomp lines 178–181)
- Input handling runs only when `cVar4 == '\0'` (decomp line 224)
- `[RSP+0x40] = (cVar4==0)?1:0` (`0x140374614`–`0x140374619`)
- Host computes `game+0xae58 = *output_Y - game+0xae50` (`0x14031edf4–0x14031ee02`)

**Assumptions (not proven):**
- Snapshot constants (scale=0.8, DAT_14090aa28=45) apply at the moment of the call
- Skipping FUN_14035f3e0 alone is sufficient for "Almanac page active" substitution

This is a planning comparison, not a patch proposal. No hook is authorized.
