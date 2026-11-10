# Trade handler content-call comparison — Almanac design evidence

**Source:** SOVIET64.exe v1.1.1.9 disassembly, `C:\WRSRE\scratch\out/` (sections 39a–52, 46, 47, 49–51)
**Method:** Static only. No game execution, no debugger, no hooks, no writes.

## 1. Correction: 0x1403628B8 claim

The user reports a conditional `FUN_14035F3E0` call at `0x1403628B8` in `FUN_140361BB0`. This address does **not** appear in any cached export or disassembly file in `scratch/out/`. The Ghidra project for SOVIET64.exe is locked from a timed-out full analysis, so this address could not be independently verified. The claim is **not confirmed**; it is recorded as user-provided but not independently evidenced.

Verified locations of `game+0xC65C` comparisons in `FUN_140361BB0` (from section49.md): `0x14036252b` and `0x1403626d7`. These are the only confirmed comparison sites in the alternate handler.

## 2. Comparison table: content calls by native selection

| Selection | FUN_140373EA0 (game+0x10398 != 0) | FUN_140361BB0 (game+0x10398 == 0) |
|-----------|-----------------------------------|-----------------------------------|
| **0** | Calls `FUN_14035F3E0` at `0x140374606` (guard: `CMP [RSI+0xc65c],0x1` / `JA 0x14037461b` skips if >1) | Content block begins after `0x14036225d`; `CMP game+0xC65C,1` at `0x14036252b`; `CMP game+0xC65C,2` at `0x1403626d7`; multiple sub-page branches (partially traced) |
| **1** | Calls `FUN_14035F3E0` at `0x140374606` (same guard, `1 <= 1` passes) | Same content block; sub-page dispatch at `0x14036252b`/`0x1403626d7` (branches for selection 1 vs 2, details not fully decoded) |
| **>1** | `FUN_14035F3E0` **NOT called** (`JA` jumps to `0x14037461b`); content block still runs but without `FUN_14035F3E0` sub-page | Same content block; `CMP game+0xC65C,2` suggests selection 2+ takes a different branch (details not fully decoded) |

### 2a. FUN_140373EA0 — verified instruction-level content path

**Content sub-view call** (verified, `140373ea0_1403829fd_disasm.asm`):
```
1403745f1: CMP dword ptr [RSI + 0xc65c],0x1   ; selected sub-tab <= 1?
1403745f8: JA 0x14037461b                      ; skip if > 1
140374606: CALL 0x14035f3e0                    ; FUN_14035F3E0(content sub-view)
14037460b: MOVSS XMM10,[RSP + 0x70]
140374612: TEST AL,AL                          ; AL = click indicator return
140374614: MOV byte ptr [RSP + 0x40],0x0
140374619: JNZ 0x140374620                     ; if AL != 0, skip flag set
14037461b: MOV byte ptr [RSP + 0x40],0x1       ; set flag when AL == 0
```

**FUN_14035F3E0 behavior** (from `14035f3e0_decomp.c`, section49):
- Draws a panel at (X, Y) with dimensions from font metrics
- Draws 6 sub-items (indices 0–5), each with C3D_LANGUAGE string IDs 0x68B–0x690 (or 0x2B6 for index 5)
- Each sub-item: `PrintLeftUnicode` label + `DrawFrom9Patch` button + click handling
- Click handling: `C3D_INPUT::GetMouseSolid` + `Collision` + latch `DAT_0x140A54E91`; on match calls `FUN_1402FD840(param_1, iVar15, 0)` and plays `gui_click`
- Sub-item 5: calls `FUN_1403613B0(param_1, 0xFFFFFFFF, 1, local_148)` and sets `game+0x10F20 = 4`; or `FUN_1403613B0(param_1, 0xFFFFFFFE, 1, local_148)` for sub-item 4
- Return value: `AL = 0x01` if click on specific sub-item (index 4 or 5 with conditions), `0x00` otherwise

**Content block state writes** (conditional on click/latch, from section51):
- `game+0x10F20` (sub-page state), `game+0x5E8`, `game+0x5EC`, `game+0x5E6`, `game+0x5F0`, `game+0xADB8`, `game+0xADC8`
- Content block reads: `game+0xC65C`, `game+0xC7DC`, `game+0xC7D0`, `game+0xC7D4`, `game+0xC7E8`, `game+0x10F18`, `game+0x10F1C`, `game+0x11728`, `game+0x11730`

**After content block:** computes `game+0xAE58 = (RBP-0x10) - game+0xAE50` (at `0x14031ee02`, in master dispatch)

### 2b. FUN_140361BB0 — partially verified content path

**Verified structure** (from sections 49–51):
- Button loop ends at `0x14036220e`/`0x140362245` (JL back to loop)
- Content block begins after `0x14036225d` (draws a 35x35 panel first, then continues)
- `CMP game+0xC65C,1` at `0x14036252b`; `CMP game+0xC65C,2` at `0x1403626d7`
- Multiple sub-page branches (selection-dependent content dispatch)
- Uses `game+0xC65C` and `game+0x12800` (content list)
- 12,304 lines of disassembly; content path more complex than `FUN_140373EA0`

**Not verified:**
- Exact address of any `FUN_14035F3E0` call (the `0x1403628B8` claim is unconfirmed)
- Which sub-page branch corresponds to selection 0 vs 1 vs >1
- Full set of state writes in the content block
- Return value handling

## 3. What a replacement boundary must preserve

For each boundary candidate, the mod must preserve:

| Category | Items | Source |
|----------|-------|--------|
| **Input** | Mouse state (`C3D_INPUT`), latch `DAT_0x140A54E91`, GetMouseSolid, Collision results | Section 51.1: latch consumed at `0x1403742F2`, not cleared by button loop |
| **Returned values** | `FUN_14035F3E0` return `AL` (click indicator) used at `0x140374612` | Section 49: `AL = 0x01` if click on sub-item 4/5 |
| **Output locals** | `[RSP+0x40]` flag set based on `AL` at `0x140374614`–`0x14037461B` | Section 49 |
| **State writes (always)** | `game+0xC65C` (selection index), `game+0x10BE8` (= -1 counter reset) | Section 51: always on click |
| **State writes (conditional)** | `game+0x1039A` (= 0 only if selection changed) | Section 51: `0x14037434F` |
| **Content state writes** | `game+0x10F20`, `game+0x5E8`, `game+0x5EC`, `game+0x5E6`, `game+0x5F0`, `game+0xADB8`, `game+0xADC8` | Section 51.3: only on click/latch |
| **Panel height** | `game+0xAE58` (computed after content block) | Section 39.1: `0x14031EE02` |

**Skipping `FUN_14035F3E0` loses:** sub-page navigation via `FUN_1403613B0`, sub-item click handling, `gui_click` sound, `game+0x10F20` sub-page state.

## 4. Boundary assessment by selection

### Selection 0 (first sub-tab)
- `FUN_140373EA0`: `FUN_14035F3E0` **called** (0 <= 1). Content sub-view draws 6 sub-items with click handling.
- `FUN_140361BB0`: `CMP game+0xC65C,1` at `0x14036252b`; branch target unknown (not decoded).
- **Boundary 2** (replace `FUN_14035F3E0` call): Valid for `FUN_140373EA0` only. For `FUN_140361BB0`, equivalent boundary not located.

### Selection 1 (second sub-tab)
- `FUN_140373EA0`: `FUN_14035F3E0` **called** (1 <= 1). Same as selection 0.
- `FUN_140361BB0`: `CMP game+0xC65C,1` branch; may take same path as selection 0 or different (not decoded).
- **Boundary 2**: Same caveat as selection 0.

### Selection >1 (third+ sub-tab)
- `FUN_140373EA0`: `FUN_14035F3E0` **NOT called** (`JA` jumps to `0x14037461B`). Content block runs without sub-page content.
- `FUN_140361BB0`: `CMP game+0xC65C,2` at `0x1403626D7`; takes different branch (not decoded).
- **Boundary 2**: `FUN_14035F3E0` call is already skipped natively. No replacement needed for `FUN_140373EA0`; `FUN_140361BB0` behavior unknown.

## 5. go / not yet assessment

| Aspect | Assessment | Reason |
|--------|-----------|--------|
| Native row drawing intact | **go** | Button loop (before content block) is separable at instruction level (section50.1) |
| Click handling preserved | **go** | Button loop handles native clicks and latch consumption before content block |
| `game+0xC65C` selection write preserved | **go** | Written in button loop at `0x140374356`, before content block |
| `game+0x10BE8` counter reset preserved | **go** | Written at `0x14037435C`, before content block |
| `FUN_14035F3E0` replacement safe | **not yet** | Return value `AL` used at `0x140374612`; flag `[RSP+0x40]` set at `0x140374614`–`0x14037461B` — mod must replicate |
| Content state writes skipped | **not yet** | `game+0x10F20`, `game+0x5E8/0x5EC/0x5E6/0x5F0`, `game+0xADB8/0xADC8` — readers not traced (section51.3) |
| Sub-page navigation skipped | **not yet** | `FUN_1403613B0` calls in `FUN_14035F3E0` disabled — game state consistency unverified |
| Alternate handler `FUN_140361BB0` | **not yet** | Content block location (`0x14036225D+`) known but internal branches not decoded; `0x1403628B8` claim unverified |
| Clip context | **not yet** | `game+0xADB0` dimensions not set in any traced function (section47) |

## 6. Evidence files preserved

Folder: `C:\WRSRE\export\almanac-evidence\sidebar-compare-20261110/`
- `report.md` — this report
- `rep_61bb0_628b8.asm` — Ghidra disassembly attempt (timed out; project locked, no output produced)

**No new disassembly files produced.** All evidence is from existing cached exports:
- `section39a.md`, `section40.md`, `section41.md`, `section42.md`, `section43.md`, `section44.md`, `section45.md`, `section46.md`, `section47.md`, `section49.md`, `section50.md`, `section51.md`
- `140373ea0_1403829fd_disasm.asm`, `14035f3e0_decomp.c`, `140361bb0_64_disasm.asm`

**Bottom line:** Boundary 2 (replace `FUN_14035F3E0` call at `0x140374606`) is viable for `FUN_140373EA0` with selections 0 and 1, **not yet** for `FUN_140361BB0` or selections >1. The mod must preserve `FUN_14035F3E0`'s return value (`AL`) and the `[RSP+0x40]` flag, and must accept that content-block state writes are skipped. The alternate handler's content path is too lightly traced to assess.
