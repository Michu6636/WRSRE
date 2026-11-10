# Almanac button placement & click approach — static evidence

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out/`
**Method:** Static only. No game execution, no debugger, no hooks, no writes.

## 0. Verified string evidence — "trade" at 0x1408B21E4

The string compared by the master dispatch at `0x14031fb9f` (`LEA RDX,[0x1408b21e4]`) was resolved from the executable:

| Field | Value |
|-------|-------|
| Target RVA | 0x1408B21E4 |
| Image base | 0x140000000 |
| File RVA | 0x08B21E4 |
| Section | .rdata (vaddr 0x86C000, rawptr 0x86B000) |
| File offset | 0x8B11E4 |
| Raw bytes (8) | `74 72 61 64 65 00 00 00` |
| ASCII | "trade" |
| Surrounding 32 bytes | `7472616465000000 00000000 77617465726f73696f6e00000000 6d006500` |
| Executable SHA-256 | `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` |
| Executable size | 10,308,608 bytes |
| TimeDateStamp | 0x6A3EB6AD |

This confirms the master dispatch compares the section name against "trade" (6 bytes) before routing to the trade row drawer.

## 1. Native Economy & Trade row — position, hit-test, clipping (verified instructions)

### 1a. Drawing entry point: `FUN_140373EA0` (VA `0x140373EA0`–`0x1403829FD`)

Called from master dispatch `FUN_140317DB0`:
- `0x14031EDEF`: `CALL 0x140373EA0` when section == "trade" AND `game+0x10398 != 0`
- `0x14031EE13`: `CALL 0x140361BB0` when section == "trade" AND `game+0x10398 == 0`

### 1b. Position geometry (verified, VA `0x14037413D`–`0x14037414e`)

```
140374114:  MOVSS XMM7, dword ptr [0x140992088]    ; DPI scalar (live snapshot: 0.8)
14037411c:  MOVAPS XMM6, XMM7
14037411f:  MULSS XMM6, dword ptr [0x14090aa0c]    ; XMM6 = DPI * constant_A (vertical offset/base)
140374127:  MOVAPS XMM0, XMM7
14037412a:  MULSS XMM0, XMM11                       ; XMM0 = DPI * constant_B
14037412f:  ADDSS XMM6, XMM0                        ; XMM6 = DPI * (constant_A + constant_B)
140374139:  CMP dword ptr [RSI + 0xc7d4], -0x1
14037413a:  JNZ 0x1403743f2                         ; if dirty counter != -1, take alternate path
140374140:  MOVSS XMM12, dword ptr [0x14090aaa0]
140374149:  MULSS XMM7, XMM12                       ; XMM7 = DPI * constant_C (likely button size)
14037414e:  LEA RCX,[RSI + 0xadb0]
140374155:  MOV R8D, 0x6
14037415b:  MOVAPS XMM1, XMM10
14037415f:  CALL 0x1404460a0                         ; setup call (unknown purpose)
```

**Position globals written per entry:**
| Global | VA | Likely meaning |
|--------|-----|---------------|
| `[0x1409be2e8]` / `[0x1409be2ec]` | `0x1409be2e8` | DPI-scaled button dimension (width/height), stored at lines 194-196 |
| `[0x1409be2f0]` / `[0x1409be2f8]` | `0x1409be2f0` | Position origin / clip rect origin, stored at lines 201-202 |

Alternate path (dirty counter != -1) writes the same globals at lines 292-293 and 298-300.

### 1c. Per-entry layout and index reordering (verified, VA `0x140374164`–`0x1403741cf`)

```
140374167:  MOV R8, qword ptr [RSI + 0xc650]   ; R8 = selected panel entry pointer
14037416e:  ADD R8, 0x48                       ; R8 = &entry->sub_entry_vector
140374172:  MOV RCX, [R8 + 0x8]                ; RCX = vector end
140374176:  SUB RCX, [R8]                      ; RCX = vector size in bytes
140374179:  MOV RAX, R13                       ; R13 = stride constant (0x6666666666666667)
14037417c:  IMUL RCX                           ; RDX:RAX = vector_size * magic
14037417f:  SAR RDX, 0x5                       ; divide high part by 32
140374183:  MOV RAX, RDX
140374186:  SHR RAX, 0x3f                      ; sign adjustment
14037418a:  ADD RDX, RAX                       ; EDX = count = vector_size / stride
```
**Stride:** The disassembly uses magic-number division by 96 (`0x6666666666666667` then `SAR 0x5`). However, the live snapshot showed `count = (end - begin) / 0x50 = 8` (stride 0x50 = 80 bytes). This **discrepancy** (disassembly /96 vs live /80) is **unresolved** and must be verified against runtime layout.

**Index reordering (non-linear), lines 159–174:**
| Screen position (EDI) | Vector index (EBX) |
|---|---|
| 0 | 0 |
| 1 | 1 |
| 2 | count - 1 (last) |
| 3 | 2 |
| 4 | 3 |
| … | … |
| count - 1 | count - 2 |

### 1d. Per-entry address and texture (verified, VA `0x1403741e4`–`0x140374214`)

```
1403741e4:  CMP EBX, 0x6
1403741e7:  JNZ 0x1403741f6
1403741e9:  CMP dword ptr [RSI + 0x5e0], 0x0   ; entry 6 gate
1403741f0:  JZ 0x1403743b6                      ; skip entry 6 if flag clear
1403741f6:  MOVSXD RAX, EBX
1403741f9:  LEA R13, [RAX + RAX*0x4]           ; R13 = EBX * 5
1403741fd:  SHL R13, 0x4                       ; R13 = EBX * 80 (0x50 stride)
140374201:  ADD R13, qword ptr [R8]            ; R13 = &sub_entry[EBX]
140374204:  MOV RCX, [R13 + 0x48]               ; RCX = sub_entry drawable pointer
140374208:  MOV RAX, [RCX]
14037420b:  MOV R8, qword ptr [0x1409ead08]    ; interface vtable?
140374212:  XOR EDX, EDX
140374214:  CALL qword ptr [RAX + 0x70]         ; virtual draw call
```

**Trade entry record layout (inferred):**
| Offset | Field |
|--------|-------|
| `+0x40` | text ID (DWORD, read at line 220) |
| `+0x48` | drawable object pointer (read at line 204) |

### 1e. Hit-testing (verified, VA `0x14037428e`–`0x1403742aa`)

```
14037428e:  MOVAPS XMM3, XMM8
140374292:  MOVAPS XMM2, XMM8
140374296:  LEA RDX, [RBP + 0x210]              ; RDX = rect struct on stack
14037429d:  LEA RCX, [0x1409be060]             ; RCX = shared panel object
1403742a4:  CALL qword ptr [0x14086c850]        ; Collision(mouse, rect)
1403742aa:  TEST AL, AL
1403742ac:  JZ 0x140374366                      ; no hit -> skip selection
```

**Hit-test input:** rect at `[RBP + 0x210]`, built from DPI-scaled position values stored earlier. The rect is compared against mouse position via `Collision` at IAT slot `0x14086c850`.

### 1f. Click handling and state change (verified, VA `0x1403742b2`–`0x14037435c`)

```
1403742b2:  MOV EDX, dword ptr [R13 + 0x40]   ; EDX = text ID of clicked entry
1403742b6:  LEA RCX, [0x140997590]            ; GetString module
1403742bd:  CALL qword ptr [0x14086c890]      ; GetString(text ID)
...
1403742f2:  CMP byte ptr [0x140a54e91], CL    ; input latch check
1403742f8:  JZ 0x140374366                    ; latch not set -> skip
1403742fa:  CMP byte ptr [RSI + 0x14], CL     ; live mode check on game object
1403742fd:  JNZ 0x140374347                   ; not live mode -> skip state write
1403742ff:  ...                               ; play sound, get value string
140374347:  CMP dword ptr [RSI + 0xc65c], EBX ; compare current selection with new index
14037434d:  JZ 0x140374356                    ; if equal, jump TO the store (does NOT skip)
14037434f:  MOV byte ptr [RSI + 0x1039a], 0x0 ; if different, clear dirty flag
140374356:  MOV dword ptr [RSI + 0xc65c], EBX ; WRITE selected item index
14037435c:  MOV dword ptr [RSI + 0x10be8], 0xFFFFFFFF ; reset dirty counter
```

**Note:** `JZ 0x140374356` jumps TO the store when indices are equal. The dirty flag is cleared only when they differ. Both paths reach the store at `0x140374356`.

### 1g. Selected overlay and draw (verified, VA `0x140374366`–`0x1403743a2`)

```
140374366:  CMP dword ptr [RSI + 0xc65c], EBX  ; is this entry selected?
14037436c:  JNZ 0x14037437c                    ; no -> skip overlay
14037436e:  MOVAPS XMM0, [0x14090c4c0]         ; selected overlay color
140374375:  MOVUPS [0x1409be30c], XMM0         ; store to draw color global
14037437c:  ...
14037439b:  LEA RCX, [0x1409be060]            ; shared panel
1403743a2:  CALL qword ptr [0x14086c860]      ; Draw call
```

### 1h. Vertical advance (verified, VA `0x1403743a8`–`0x1403743e0`)

```
1403743a8:  ADDSS XMM6, XMM7                  ; advance vertical position by button size
1403743ac:  MOV R13, 0x6666666666666667       ; magic for stride division
1403743b6:  INC EDI                           ; next screen position
1403743b8:  ... recompute count ...
1403743de:  CMP EDI, EDX
1403743e0:  JL 0x1403741a2                    ; loop if more entries
```

**Vertical step:** `XMM6 += XMM7` where XMM7 = DPI * `[0x14090aaa0]`. This is the per-button vertical pitch.

### 1i. Alternate path: no entries / dirty counter != -1 (VA `0x1403743f2`–`0x1403745b6`)

When `game+0xc7d4 != -1` (line 136-137), a different drawing path is taken at `0x1403743f2`, which also writes `[0x1409be2e8]`/`[0x1409be2ec]` (lines 292-293) and `[0x1409be2f0]`/`[0x1409be2f8]` (lines 298-300), then does its own hit-test at `0x1403744c3`.

## 2. Two approaches for an Almanac button

### Approach A: Append a native entry to the Economy & Trade row

**Mechanism:** Add a 9th entry to the sub-entry vector at `*(game+0xC650)+0x48`..`+0x50`.

**Evidence from live snapshot:**
- Trade vector: begin=`0x000001F2C917D050`, end=`0x000001F2C917D2D0`, count=8, capacity beyond end
- Stride: 0x50 bytes (live-verified); text ID at +0x40, drawable ptr at +0x48
- Entry text_ids: 1501, 1511, 1664, 1683, 1684, 37554, 67066, 1530

**What happens when count goes 8→9 (from reordering logic):**
| Screen position | Vector index (count=8) | Vector index (count=9) |
|---|---|---|
| 0 | 0 | 0 |
| 1 | 1 | 1 |
| 2 | 7 (last) | 8 (new last) |
| 3 | 2 | 2 |
| 4 | 3 | 3 |
| 5 | 4 | 4 |
| 6 | 5 | 5 |
| 7 | 6 | 6 |
| 8 | — | 7 |

**Critical:** Appending does NOT put the new item at screen position 8. The item at logical index 2 moves from screen pos 7 to screen pos 8. The new item at index 8 appears at screen pos 2. This is **not** "alongside the eight visible icons" — it rearranges existing ones.

**Requirements:**
- Write a new sub-entry record (stride 0x50) with text ID and drawable pointer into the vector
- Ensure vector has capacity (live snapshot shows capacity pointer beyond end, but exact capacity unknown)
- Register a drawable object implementing the vtable called at `[RAX+0x70]`
- Add a GetString entry for the Almanac label (custom text ID ≥ 1,000,000 per modding rules)
- Click handling is automatic: native drawer writes `game+0xC65C` and `game+0x10BE8=-1`

**Risks:**
- Vector capacity may be exactly 8 (no room for 9th) — must verify at runtime
- Non-linear reordering breaks the "alongside" assumption
- Native-content suppression: the Almanac page would need `FUN_14035F3E0` (called when `game+0xC65C <= 1`) to draw custom content instead of native trade page
- Return to another section: handled by `FUN_14007C400` (section-switch handler writes `game+0xC650`)

### Approach B: Draw a separate dynamically positioned button

**Mechanism:** Do not touch the trade vector. Draw an independent button in the same row area using the same DPI-scaled position math, with independent hit-testing.

**Position math (from trade drawer):**
- Base Y from `[0x140992088]` (DPI) * `[0x14090aa0c]` + DPI * `[0x14090aaa0]` (XMM6 accumulator)
- Button size from DPI * `[0x14090aaa0]` (XMM7)
- Vertical step: `XMM6 += XMM7` per entry
- Position stored at globals `[0x1409be2f0]`/`[0x1409be2f8]`
- Hit-test via `Collision` at IAT `[0x14086c850]` with rect on stack

**Requirements:**
- Read DPI scalar at `0x140992088` (module global)
- Compute button rect using same constants as trade drawer (`[0x14090aa0c]`, `[0x14090aaa0]`)
- Position button at row Y + (count * step) to place after the 8th icon, or at a specific X offset
- Hit-test against mouse using `Collision`
- On click: write `game+0xC65C` (selected index) and `game+0x10BE8=-1` manually, then draw custom content via `FUN_14035F3E0`-equivalent or own renderer
- Draw button texture via virtual draw call `[drawable+0x70]` like native entries

**Risks:**
- Hit-test rect must exactly match visual button bounds (clipping vs hit-test are separate unknowns)
- DPI scaling must match native (use `0x140992088`)
- No native suppression: the 8 native icons still draw; Almanac button is purely additive
- Return to another section: must manually write `game+0xC650` via `FUN_14007C400`-equivalent logic

## 3. Custom content, native suppression, and return — requirements per approach

| Requirement | Approach A (append native) | Approach B (separate button) |
|---|---|---|
| Custom content | `FUN_14035F3E0` draws when `game+0xC65C <= 1`; must handle Almanac page | Own render code, triggered by own click detection |
| Native suppression | None — native row still draws 8 icons + Almanac item | None — purely additive |
| Replace native page | Must detect Almanac selection and skip native `FUN_14035F3E0` output | N/A (button is separate from row) |
| Return to another section | `FUN_14007C400` writes `game+0xC650` on section switch | Must manually write `game+0xC650` |
| Vector capacity risk | Yes (9th entry may not fit) | No |
| Reordering risk | Yes (index 2 moves screen pos) | No |
| Click integration | Native drawer handles it | Must replicate Collision + state writes |

## 4. Unproven assumptions

1. **Stride is 0x50:** Live snapshot shows count=8 with /0x50, but disassembly magic-number division suggests /96. **Unresolved discrepancy.**
2. **Vector capacity:** Live snapshot shows capacity pointer beyond end, but exact capacity (max entries) unknown.
3. **Hit-test rect == visual bounds:** The `Collision` rect at `[RBP+0x210]` may differ from drawn button bounds (clipping vs hit-test are separate).
4. **`FUN_14035F3E0` content suppression:** Not traced whether skipping this call is safe or how to suppress native trade page when Almanac item selected.
5. **`game+0xC65C` semantics:** Snapshot showed sub-tab index 0→1 on icon click, but whether this is "selected row item" or "selected sub-tab" within a page is unproven.
6. **DPI constants:** `[0x14090aa0c]`, `[0x14090aaa0]`, `[0x14090a9cc]` purposes not independently verified.

## 5. Future integration tests (read-only snapshot)

1. Read `game+0xC650` vector begin/end/capacity to confirm 9th entry capacity
2. Read DPI scalar and constants `[0x14090aa0c]`, `[0x14090aaa0]` to verify position math
3. Snapshot `[0x1409be2e8]`/`[0x1409be2f0]` globals during row draw to capture button rect
4. Verify whether clicking an existing icon writes `game+0xC65C` (already observed: yes, 0→1)
5. Verify whether selecting another section writes `game+0xC650` (already observed: yes, pointer changes)

## 6. Evidence files preserved

Folder: `C:\WRSRE\export\almanac-evidence\almanac-button-approach-20261109/`
- `report.md` — this report

No new disassembly files created; all evidence from existing `scratch/out/` artifacts:
- `140373ea0_1403829fd_disasm.asm` — trade drawer position/hit-test/click
- `140317db0_full_disasm.asm` — master dispatch and section-switch
- `14007c400_140084754_disasm.asm` — section switch handler

**Bottom line:** Approach B (separate dynamically positioned button) is safer for the Almanac goal — it avoids vector capacity risk and non-linear reordering, and is purely additive. Approach A (append native entry) rearranges existing icons (index 2 moves screen position when count 8→9) and requires vector capacity. Both require independent hit-test/clip verification and custom content rendering. The native drawer writes `game+0xC65C` on click; a separate button must replicate this manually.
