# Reputation row item vs. separate Almanac button: static analysis and validation plan

**Status:** Static investigation and planning only. Nothing was run, installed, hooked or modified. The game, the mod, the saves and the shipped configuration were not touched. Third-party material appears only as bounded, annotated excerpts in [`export/almanac-row-item-analysis-20261110/`](../export/almanac-row-item-analysis-20261110/). Nothing is redistributed and no code is copied.

**Requirement under test:** all eight native Economy & Trade tiles plus the Almanac are visible and clickable on one row ([almanac-plan.md](almanac-plan.md) §1 and §5).

**Evidence labels used below**

- **[D]** Reputation DLL bytes or disassembly. The DLL matches SHA-256 `fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a`.
- **[N]** Native game listings preserved in the repo (`export/ch_tab23_full.asm`, `export/140373ea0_1403829fd_disasm.asm`, `export/140317db0_full_disasm.asm`). The game binary is not in the workspace, so the listings are not verified against the installed game. The instruction bytes and text are verbatim from the preserved files.
- **[C]** Text in `reputation.ini`. This is the author's claim. We have not observed it.
- **[A]** Earlier agent or review claim. A lead only.
- **[I]** Inference from the labelled evidence, stated as such.

Addresses in `0x18…` are DLL virtual addresses. Addresses in `0x14…` are game image addresses (base `0x140000000`). `g_ui` is `exe+0x9D4F10`, and `exe+X` means the game base plus X. `D01`–`D16`, `N01`–`N08` and `C01` refer to the excerpt files in the evidence folder.

## 1. Summary

1. **Reputation has two separate paths.**
   - **Path A, main sidebar entry.** The DLL appends a copy of a sidebar record to the game's sidebar vector (`exe+0x9E1518`). When that entry is selected, the DLL's own body draws inside the game's sidebar panel. The DLL also resizes that panel through its `SetDimensions` and `SetPosition` detours.
   - **Path B, optional Economy & Trade row item** (`sidebar_item`, shipped as `0`). The DLL appends a 0x50-byte sub-entry to the trade record's sub-list (`record+0x48`), which is the same list the native tiles come from. The native code draws and hit-tests the new tile. On the traced path, the DLL's click reaction changes DLL-local state. It does not establish that a window opens.
2. **The native row has two handlers.** The **normal** handler `FUN_140373EA0` draws every entry, but places the **last** entry in third position (index 2). The **alternate** handler `FUN_140361BB0` draws entries `0..count-2` in order, so the **last entry is never drawn or clicked**. The host chooses the alternate when `[game+0x10398] == 0`.
3. **Consequence for one appended item (8 → 9 entries):**
   - *Normal handler:* nine tiles. The appended item is third, and native tiles 2–7 each move one slot right. All natives remain, subject to the native skip rules.
   - *Alternate handler:* eight tiles. All natives 0–7 remain, and **the appended item is the one omitted**. Without the append, the alternate already omits native index 7.
4. **Leading approach for the same-row requirement: Path B (row item).** It is the only option in which the game's own draw, highlight and hit-test place the Almanac on the same row. Its risks are handler-dependent visibility, a changed native order, a native vector append with no DLL-side capacity check, and an unresolved content path. The separate dynamic button avoids the vector change and the reorder. However, it has to build its own hit-test, drawing and input ownership, and it still has to suppress native content. Neither approach is validated.
5. **Smallest practical validation:** one bounded, read-only, non-pausing session on a disposable save copy. It must be authorised separately. It records the active handler and the nine tile rectangles, then clicks one native tile and the appended tile. Rollback is `sidebar_item = 0` followed by a restart. The DLL has no runtime removal path (§6).

## 2. Path A: main sidebar entry

### 2.1 Creation and append

- **[D, earlier pass]** The clone function `0x180056B70` is called from the terrain-render hook `0x180068180`. When it is declined, it retries every 240 frames.
- **[D]** Gates before the copy: declined latch `0x180287B38`, `sidebar` (`0x180280D80`) and the build-stamp check `0x180280B24`.
- **[D, D16]** The copy is appended with the game's record copy constructor `exe+0x459530` (`add rax,0x459530` at `0x180056ECF`). The vector end then grows by `0x68` (`add [rsi+0x9E1520],0x68` at `0x180056EE3`). Rollback of the end pointer is at `0x180057010`.
- **[D, earlier pass]** Donor selection: the last record with `+0x00 == 1`, `+0x30 == g_ui`, and the sub-list header at `+0x48/+0x50/+0x58` null. The DLL declines when the vector is full. Path A therefore has a capacity check; Path B does not (§3.2).
- **[D]** Icon: `editor/maintab_%s.png`. If the texture is null, the record is removed. Success sets `0x180280D84` ("entry present").

### 2.2 Selection and return

- **[D]** Reputation-selected test `0x180057D50`: true when `0x180280D84` is set and the record at `exe+0x9E1560` is named `reputation`.
- **[D, D05] The selection handler `0x1800575C9`–`0x180057780` and its state writes:**
  - It stores the result of `0x180057D50` into the DLL's state flag `0x180411BC8` at `0x180057702`. So `0x180411BC8` mirrors "reputation selected".
  - It sets `exe+0x9E1560` through `0x1800593B0`, called at `0x1800576C4` (re-select rule below).
  - It calls the helper `0x1800571C0` at `0x1800576ED` only on the no-window branch described below. The helper's effect was not traced.
- **[D, D05, D12] Re-select rule.** `0x1800593B0` runs only when all of these hold: effective `in_panel` (`0x1800C32EC`) is nonzero (0x18005769C); `0x180057D50` returns 0 (0x1800576A6–0x1800576A8); `0x180411BC8` is 0 (0x1800576AA); `0x180001C80` is nonzero (0x1800576B2–0x1800576B9); and the trade-row predicate `0x180058310` is 0 (0x1800576BB–0x1800576C2). `0x180001C80` returns 1 when any of the seven DLL window flags is set (D12). In effect, the DLL re-selects its entry while one of its windows is open.
- **[D, D05] Close rule.** When the state flag changes from nonzero to zero (0x180057702–0x18005770A):
  - If `sidebar_hides_panel` (`0x1800C3C74`) is 1, the close block is skipped (0x180057710–0x180057716). **This is the shipped value**, so the close block is not reached.
  - Otherwise, if `0x180001C80` is nonzero and the trade-row predicate is 0, the handler clears `0x180286080` and `0x180286084` if `0x180286080` was set (0x18005772A–0x180057738), logs "another sidebar entry chosen - panel closed" (0x18005773E), then clears the six window flags `0x1800C4C50`, `0x1801CDA1C`, `0x180184EB4`, `0x1801CDA14`, `0x1801CDA18` and `0x1800C3158`, and sets `0x1800C3170` to 1 (0x180057745–0x180057769).
  - The no-window branch (reputation still selected, `sidebar_hides_panel` 0, no window flag set, state flag previously set) calls `0x1800571C0` and clears `0x1801A3FA8` (0x1800576D0–0x1800576F8). Its effect was not traced.
  - The branch for a new nonzero state (`0x180057799`) was not traced.

### 2.3 SetDimensions and SetPosition interception, and the AE54/AE58 writes

- **[D, earlier pass]** Hooks are `SetDimensions` detour `0x180067FA0` (original saved at `0x1802860B8`) and `SetPosition` detour `0x180068090` (original at `0x1802860C0`).
- **[D, D13] Order inside the `SetDimensions` detour:**
  1. It calls the game's original `SetDimensions` first (`0x180067FB6`, saved pointer `0x1802860B8`).
  2. If `[0x1801B4860]` is nonzero (0x180067FBC–0x180067FCF), it reads the caller return address `[rsp+0x48]` and compares it with `exe+0x31E478` (0x180067FD1–0x180067FDB), then `exe+0x31E4B2` (0x180067FDD–0x180067FE7). If neither matches, it exits.
  3. **Both callers** store the raw width and height at `0x1801B4868` and `0x1801B486C` (0x180067FE9–0x180067FF1).
  4. **Caller `exe+0x31E478` only** calls the clamp `0x1800280E0` (0x180067FFE). The clamp writes AE54 and AE58.
  5. If raw `in_panel` (`0x180280D74`) and `0x180280D78` are nonzero (0x18006800A–0x180068019): caller `exe+0x31E478` stores width and height at `0x180280D5C` and `0x180280D60` (0x18006802F–0x180068037). Then, if effective `in_panel` (`0x1800C32EC`) is nonzero and `0x180057D50` is true, it tail-jumps to the sizer `0x180058930` (0x18006803F–0x18006805B). Caller `exe+0x31E4B2` stores the same two fields at `0x180068074` (and `0x18006806C`), with no effective-panel check and no tail-jump.
- **[D, earlier pass]** `SetPosition` (caller `exe+0x31E509`) stores x and y at `0x180280D64` and `0x180280D68`, and a constant from `0x1801A3FD0` at `0x180280D6C`.
- **[D] AE54/AE58 writers.**

  | Address | Writes | Context |
  |---|---|---|
  | `0x180028128` | AE54 (`exe+0x9DFD64`) | clamp `0x1800280E0` |
  | `0x180028149` | AE58 (`exe+0x9DFD68`) | clamp `0x1800280E0` |
  | `0x180058B23` | AE54 | sizer `0x180058930`; skipped when the `comiss` test at `0x180058B1E`–`0x180058B21` fails |
  | `0x180058B2B` | AE58 | sizer `0x180058930` |
  | `0x180059323` | float to `g_ui + [0x1800C3C68]` | placement path; `panel_width_at`, shipped 44628 = `0xAE54` (verified in the file bytes, `D14`/`D15` and the earlier `19_` excerpt) |

  - The sizer reads the three floats at `g_ui+0xAE50` (`exe+0x9DFD60`). It takes its right-edge input from `0x1800C32F0`, which is config `in_panel_w` (file default 1280.0, from the earlier pass). Its bottom input is `0x1800C32FC`, which is config `in_panel_bottom`. The bottom formula is still not decoded.
- **[N, N01, N08] Native AE58 writers in row mode.** These matter for any mod-owned bottom edge.
  - Host, after the normal handler: `MOVSS XMM0,[RBP-0x10]`, then `SUBSS XMM0,[R14+0xAE50]`, then `MOVSS [R14+0xAE58],XMM0`, at `0x14031EDF4`–`0x14031EE02`. The bottom edge is the handler's returned Y minus AE50.
  - Alternate handler: `MOVSS [R14+0xAE58],XMM15` at `0x140361F12`, before any skip test.
  - Other host AE58 writes exist at `0x14031ED26` and `0x14031EDA1` (preserved listing, not excerpted).
  - **[I]** Any Almanac bottom-edge logic must not fight these writes.

### 2.4 How its own content is drawn

- **[D, earlier pass, exports 09–15]** The in-panel layout `0x180058E77`–`0x18005905D` reads the captured rectangle at `0x180280D5C`–`0x180280D68`. It runs only while the reputation test is true (callers `0x180058D62` and `0x1800592E3`).
- The body guard `0x180039400` and the dispatch `0x180038F30` choose the stage from `panel_stage` (`0x180280AF8`). Stage 3 (`0x18003A260`) draws the heading and values and handles the cursor.
- **Result:** Path A's content is a DLL drawing inside the game's sidebar panel. It is active only while the reputation entry is the selection.

## 3. Path B: optional Economy & Trade row item

### 3.1 Configuration and gates

- **[C, C01]** The shipped block (`reputation.ini` lines 1586–1632) says "OFF, and the shipped answer is 0". It sets `sidebar_item_panel = trade`, `sidebar_item_name = reputation` and "Needs sidebar = 1". It also says the icon is `media_soviet/editor/<panel>_<name>.png` at 96×96, and that the option "works" and that clicking it opens the window. These are author claims. The 96×96 size and the "works" claim were not checked here.
- **[D, D01]** Insertion gates: declined latch `0x180287B4C` must be 0; `sidebar_item` (`0x180287B48`) must be set; `sidebar` (`0x180280D80`) must be set; the build check `0x180280B24` must be set.
- **Result as shipped:** `sidebar_item = 0`, so the DLL does not run Path B.

### 3.2 Insertion into the trade sub-list

- **[D, D05]** The insertion function `0x180057E40` is called at `0x18005761E` on **every** invocation of the selection handler. Both branches of the case loop join there.
- **[D, D01] Gates, in order.** Declined latch `0x180287B4C` must be 0. `sidebar_item` (`0x180287B48`) must be set. `sidebar` (`0x180280D80`) must be set. The build check `0x180280B24` must be set; if it is not, the latch is set and the log says "sidebar_item is on, but this is not the build". Nothing clears the latch, so each decline is permanent for the session.
- **[D, D01] Retry counter.** It calls `0x180064790`. If that returns zero, the counter `0x180287B50` is reset to 0 and the function exits (0x180057EB5–0x180057EC2). If it returns nonzero, the counter is used (the 240-call threshold is from the earlier pass; `0x180064790` was not decoded).
- **[D, D02] Record lookup.** The trade record is found by a lookup that was not decoded in this pass. If the lookup returns -1, the function logs "there is no sidebar panel called %s - declined" and sets the latch `0x180287B4C` (0x180057F50–0x180057F69).
- **[D, D02] Sub-list checks.** The sub-list header is at record `+0x48` (begin), `+0x50` (end) and `+0x58` (capacity). The function checks that begin is non-null (0x180057F92–0x180057F98) and that end is not below begin (0x180057F9E–0x180057FA5). It checks that the span is an exact multiple of `0x50`, using multiplication by the reciprocal constant `0xCCCCCCCCCCCCCCCD` (0x180057FB4–0x180057FD6), and that the span is at most `0x10000` (0x180057FDC–0x180057FE3). A span that fails the multiple test logs "does not divide by 80" (0x18005829A). **Nothing checks that the vector has room for one more entry.**
- **[D, D04] Existing entry.** If a sub-entry named `reputation` already exists, its index is stored in `0x1800C3C5C` (0x180058190) and no entry is added.
- **[D, D03] Build and push.** A zeroed 0x50-byte local is filled with the name at `+0x00`, and the string ID from global `0x1800C3C58` (loaded at `0x180058087`). It is pushed through the native helper at `exe+0x4492F0` (`call rax` at `0x1800580DC`).
- **[N, FUN_1404492f0 listing]** The native helper checks `end == capacity`, calls a growth routine (`0x14044C410`) when they are equal, copies 0x50 bytes to `end`, and advances `end` by 0x50. The growth routine was not decoded.
- **[D, D04] Texture.** The format `editor/%s_%s.png` (`0x18009E3C0`) is loaded through `0x1800594C0`. The pointer is stored at `+0x48` of the new sub-entry (store at `0x180058189`; the earlier pass says this is the last entry).
- **[D, D04] Texture failure.** If the texture is null, `end` is reduced by 0x50 (`0x180058212`), the decline latch `0x180287B4C` is set (`0x180058217`), and the log says the item "has no texture - taken back out". A texture failure therefore also declines permanently.
- **[D]** The only removal in the DLL is this rollback at insertion time. There is no runtime removal path.

### 3.3 Selection: the native write and the DLL's check

- **[N, N04, N07]** A click on a native tile writes the vector index to the selected-index field. Normal handler: `MOV [RSI+0xC65C],EBX` at `0x140374356`, where EBX is the **mapped** index (§4.2). Alternate handler: `MOV [R14+0xC65C],ESI` at `0x140362193`, where ESI is the vector index.
- **[I, from offset arithmetic and both sides' code]** `g_ui+0xC650` is `exe+0x9E1560`, the selected-record pointer that Path A writes. `g_ui+0xC65C` is `exe+0x9E156C`, the index that the DLL reads in `0x180058310`. The alternate handler reads its tile list from `[R14+0xC650]+0x48` (N06); the normal handler's source is in N02. The DLL and the native code therefore agree on these addresses.
- **[D, D06] Trade-row check `0x180058310`:** true when the stored item index `0x1800C3C5C` is non-negative, `sidebar_item` is set, the selected record is named `trade`, and `exe+0x9E156C == 0x1800C3C5C`.

### 3.4 What the DLL does on a row click

- **[D, D05] Trigger.** After the insertion and predicate calls, the handler compares the predicate result with the previous state in `0x1804DE980` (0x180057628–0x180057630). It acts only when the new state is nonzero (0x180057638). If all seven window flags are zero (0x18005763A–0x18005766A), it calls the dispatcher `0x18002F1E0` with `ecx = [0x180411BCC]` (0x180057672–0x180057678).
- **[D, D05] Log.** "item chosen in the trade panel - opening the reputation window" is printed at `0x18005768B` whenever the state changes to nonzero, whether or not the dispatcher ran. It is **not** evidence that a window opened.
- **[D, D05, D07] The dispatch argument is stale.** `0x180411BCC` is written only at `0x180057617`, inside the case loop (`0x180057600`–`0x180057615`). The loop runs (0x1800575C9–0x1800575F7) when **any** of six flags is nonzero: `0x180286080`, `0x1801CDA18`, `0x1801CDA14`, `0x180184EB4`, `0x1801CDA1C` or `0x1800C4C50`. It stores the first index `i` (0–7) whose case test passes. The case tests (`0x18002F120`, table `0x18002F1BC`, D09) are: 0 is `0x1800C3158 != 0`; 1 is `0x180286080 != 0`; 2 is `0x1800C33A8 != 0` and `0x1801CDA18 != 0`; 3 is `0x1800C33A4 != 0` and `0x1801CDA14 != 0`; 4 is `0x180184EB4 == 1`; 5 is `0x180184EB4 == 2`; 6 is `0x1801CDA1C != 0`; 7 is `0x1800C4C50 != 0`.
- **[D] A call that stores never dispatches.** The dispatch guard needs all seven flags to be zero, which includes the six flags that enable the loop. So on the dispatch path, `0x180411BCC` keeps the value left by an earlier store. It is 0 at load (the address lies beyond the file's raw `.data`, from the earlier pass). In a session that has never stored, the first dispatch therefore uses index 0. The clicked native index is not read by the dispatch; it enters only through the trade-row predicate.
- **[D, D09, D10, D11] Dispatch.** `cmp ecx,7; ja` (0x18002F1E3–0x18002F1E6) sends index 8 and above to the common tail `0x18002F327`, which only clears DLL flags (D11). Index 0 goes to `0x18002F203`, which runs `cmp [0x1800C3158],0; mov edx,2; mov [0x1800C3170],1; cmovne edx,0; mov [0x1800C3158],edx` (earlier pass). So on a clean state the first click sets `0x1800C3158` to 2. While that flag stays nonzero, the all-zero guard fails and no further dispatch runs.
- **[D, D12] Effect on re-select.** With `0x1800C3158` nonzero, `0x180001C80` returns 1, which enables the re-select rule (§2.2) on later selection changes. **[I]** This static chain has not been observed at runtime.
- **Conclusion.** The static trace establishes a **DLL-state change** on a row click, and the change depends on the flag history. It does **not** establish that a reputation window draws.

### 3.5 Content and return

- **[D]** The in-panel body and layout run only while the reputation test `0x180057D50` is true (callers `0x180058D62` and `0x1800592E3` per the earlier pass; `0x180068041` per D13). On a trade-row click the reputation entry is not selected, so this body does not draw.
- **Not traced.** The DLL window that the dispatch's case code enables, and the draw function for it. The case handlers beyond index 0 were not decoded. The content path for index 8 is therefore **unresolved**, and it is the largest open gap for the row item.

## 4. Native row ordering and handler behaviour [N]

### 4.1 Which handler runs

- **[N, N01]** Host section dispatch: `CMP byte [R14+0x10398],R15B` at `0x14031EDE6`. R15 is zeroed at `0x14031EAD5` (`XOR R15D,R15D`). The conditional jump leads to `0x14031EE10` (`XOR R9D,R9D`) and then `CALL 0x140361BB0` (alternate) at `0x14031EE13`. The other path calls `0x140373EA0` (normal) at `0x14031EDEF`.
- **[N]** The host also writes AE58 (`MOVSS [R14+0xAE58],XMM0`) at `0x14031ED26` and `0x14031EDA1`. The normal handler's own path writes AE58 at `0x14031EDF4`–`0x14031EE0B`. The mapping of these writes to handlers was not traced.
- **[N, unverified]** The runtime value of `[game+0x10398]` on the trade page was not established.
- **[I]** The shipped screenshot shows eight tiles for eight entries. The alternate handler would draw seven of them, so the normal handler was probably active when that screenshot was taken. This depends on the screenshot, whose capture timing is not known.

### 4.2 Normal handler `0x140373EA0` (N02, N03, N04)

- The loop `EDI` runs from 0 while `EDI < EDX` (`CMP EDI,EDX` at `0x1403743DE`, `JL 0x1403741A2` at `0x1403743E0`). **It draws every entry that is not skipped.**
- Index mapping (`0x1403741A2`–`0x1403741CC`): `CMP EDI,2`. For `EDI < 2`, `EBX = EDI`. For `EDI == 2`, `EBX = count-1`, computed from the `(end-begin)` span. For `EDI > 2`, `EBX = EDI-1`. The entry at `count-1` is therefore drawn third.
- Skips (N02): `CMP [RSI+0x578],0` at `0x1403741CF`. If it is nonzero, mapped indices 3 and 4 are skipped (0x1403741D8–0x1403741DE). `CMP EBX,6` at `0x1403741E4`. If `EBX == 6` and `[RSI+0x5E0] == 0`, the entry is skipped (0x1403741E9–0x1403741F0). A skipped entry jumps to `0x1403743B6`, past the `ADDSS` at `0x1403743A8`, so it consumes no X slot.
- X advances by a pitch after each drawn tile (`ADDSS XMM6,XMM7` at `0x1403743A8`). The origin and pitch are scale-dependent constants that were not decoded, so the nine-tile width is unknown.

### 4.3 Alternate handler `0x140361BB0` (N05, N06, N07, N08)

- Count: `(end-begin)/0x50` via the reciprocal `0x6666666666666667` and `SAR`. `DEC EAX` gives `count-1` at `0x140362241`. The loop variable `ESI` starts at 0 (`XOR R12D,R12D` at `0x140361D86`, `MOV ESI,R12D` at `0x140361F6D`).
- Skips: `CMP [R14+0x578],0` at `0x140361FC0`, and `JZ 0x14036220E` at `0x140361FE3`, which goes to the loop increment. The alternate handler has the same skip family as the normal handler (earlier pass).
- Tile texture pointer: `MOV RBX,[R15+RAX*1+0x48]` at `0x140361FF0`.
- Loop advance (N06): `INC ESI` at `0x14036220E`, `ADD R15,0x50` at `0x140362210`. It then reloads the selected record's sub-list (`MOV R8,[R14+0xC650]`, `ADD R8,0x48` at `0x140362214`–`0x14036221B`) and recomputes `count-1` (`DEC EAX` at `0x140362241`).
- Back-edge: `CMP ESI,EAX` at `0x140362243` and `JL 0x140361FC0` at `0x140362245`. **Entries `0..count-2` are drawn. Entry `count-1` is never drawn and never clicked.**
- Click write: `MOV [R14+0xC65C],ESI` at `0x140362193` (N07). AE58 write at `0x140361F12` (N08).

### 4.4 Index tables (before skips)

| Vector entries | Normal handler: visible order (left to right) | Alternate handler: visible |
|---|---|---|
| 8 (today) | N0, N1, **N7**, N2, N3, N4, N5, N6 | N0 … N6 (**N7 omitted**) |
| 9 (with Path B item = vector index 8) | N0, N1, **Almanac**, N2, N3, N4, N5, N6, N7 | N0 … N7 (**Almanac omitted**) |

- Skipped entries are removed from the sequence and do not occupy a slot.
- The config's comment says the game's own panels draw their items "GENERICALLY - count straight from the vector". That is contradicted by the index remap in the normal handler and by the `count-1` bound in the alternate handler. It holds only in the sense that no name comparison appears in these loops [C vs N, I].
- A consequence of appending at the end: under the alternate handler, the omission lands on the appended item, not on a native icon. Without the append, the alternate already omits native index 7. The baseline must therefore be measured before any change is attributed to Path B.

## 5. Comparison against the requirement

Option A is the Path B row item: the Almanac appended to the trade sub-list. Option B is a separate, dynamically positioned mod-owned button.

| Criterion | A. Row item (Path B) | B. Separate dynamic button |
|---|---|---|
| On the same row as the eight natives | Yes, by construction, as a native tile. | Only if its X and Y come from the live row origin and pitch. Those constants are not decoded, so the ninth slot may overlap or clip. |
| Visible, normal handler | Yes, third position, if the nine tiles fit the panel. Fit is unknown. | Yes, if drawn in the same pass as the row. |
| Visible, alternate handler | **No.** The last entry is omitted. | Independent of the handler, but the draw must run on the trade page in either mode. The hook for that pass is not identified. |
| Clickable | Native hit-test and highlight. A click writes vector index 8. | Must implement a hit-test. The `ui_panels` precedent is building-window-only (`AppendWidgetsImpl`). Its comments disagree on `Collision`'s arity (`plugins/ui_panels/ui_panels.cpp` line 398, one argument, against the typedef at line 761, two arguments). |
| Native order | Changed. The Almanac is third and natives 2–7 move right (normal handler). | Preserved. |
| Native vector | Mutated by the append. Growth can reallocate it, and the DLL has no capacity check. | Not touched. |
| Selection ownership | Native index 8, read by the DLL. | No native index. Needs mod-owned state. Writing a synthetic native index was not analysed here. |
| Content for the selection | Native trade content (lead) plus DLL state (not traced). | Must be built. |
| Native content suppression | Still required. | Still required. |
| Reuse of existing code | Path B exists but is config-gated, and its click reaction is unverified. | Path A is sidebar-specific. The `ui_panels` precedent is building-specific. |
| Rollback | `sidebar_item = 0` plus restart. No runtime removal. | Disable the draw and the hit-test. No game-state writes, so simpler. |

**Reading of the comparison.** Option A is the only one where the game draws and hit-tests the ninth tile itself. That is the strongest argument for the same-row requirement. It has four weaknesses, and each is independent evidence rather than speculation:

1. It fails silently under the alternate handler (§4.3).
2. It changes the native order (§4.4).
3. It mutates the native vector without a capacity check (§3.2).
4. Its content path is undefined (§3.4–3.5).

Option B avoids (2) and (3) and needs no synthetic native writes. However, it has to build input ownership, layout and suppression from scratch, using constants that are not yet decoded and a precedent that is building-specific.

**Product fit.** `docs/almanac-plan.md` prefers a distinct global sidebar section. It accepts an Almanac page inside Economy & Trade only while the Almanac keeps **its own same-row button** (line 12). If the same-row requirement cannot be met without overlap or a second row, the plan says to stop and bring the trade-off to the owner (line 15). Option A keeps the Almanac as a tile in the trade row, so it meets the plan only if the handler, fit and content checks pass. Option B meets it only if its geometry is validated against the live row.

## 6. Recommendation and smallest practical validation

### 6.1 Recommendation

- **Leading approach: Option A (Path B), as a validation candidate only.** Implementation is not authorised, and nothing here proves it.
- **Fallback: Option B,** if the validation fails for handler or fit reasons, or if the native reorder is unacceptable to the owner.
- Do not write native selection indices, native vector entries, or the DLL's state directly as part of any validation outside the steps below.

### 6.2 Validation

**V0: static, no game run.** Needs the owner's own game binary.

- Confirm the binary's hash and that its PE timestamp equals the build stamp `0x6A3EB6AD`. The DLL refuses other builds, and the listings are unverified until this is done.
- Decode the row constants (`scale × …` for origin and pitch) and the panel width path. Compute whether nine tiles fit inside the panel's right edge.
- Confirm the `N` listing bytes against the binary by address.

**V1: one bounded runtime session.** Requires separate authorisation. Use a disposable save copy and a test configuration, not the shipped file.

- Setup: `sidebar = 1`, `sidebar_item = 1`, icon present. Change nothing else.
- Open the trade page. Take the approved non-pausing read-only snapshot. Record `[game+0x10398]` (handler), `[game+0xC7D4]` (expected `-1`), the vector count (expected 9), the nine tile rectangles and `[game+0xC65C]`.
- Click native tile 0, then the appended tile, then native tile 7. Read the selected index after each click.
- Leave the tab and come back. Confirm native content and the sidebar state return cleanly. Do not save during the session.

**Pass criteria (go/no-go for Option A only, not for the Almanac):**

- (a) `[game+0x10398] != 0`, so the normal handler runs. If the alternate handler runs, Option A fails.
- (b) Nine tiles inside the panel, with no overlap. All native tiles present, except the entries the skip rules exclude.
- (c) Clicking the appended tile sets the index to 8. Native clicks set their own indices.
- (d) No fault in the game or the DLL log.
- (e) Leaving and returning leaves the native trade content and sidebar state intact.
- (f) No save was written during the session.

### 6.3 Rollback

- **Test configuration:** set `sidebar_item = 0` and restart. The DLL does not insert again after restart. The shipped `reputation.ini` is not changed.
- **Runtime:** the DLL has no removal path for an inserted sub-entry. The only rollback is the texture-failure path at insertion time (§3.2). If the game faults, restart.
- **Saves:** do not save during the test. Keep a backup. Whether the sub-entry persists in a save is unknown, so any save made with the item present must be checked on a copy before it is trusted.
- **DLL:** not modified. Removing the plugin is a separate, authorised action.

## 7. Risks

| # | Risk | Evidence | Severity | Mitigation or test |
|---|---|---|---|---|
| R1 | The alternate handler omits the appended tile, so the Almanac is invisible. | §4.3, N05–N06 | High | V1 (a). Fall back to Option B. |
| R2 | The native order changes. | §4.4, N02 | Medium | Product decision. |
| R3 | Nine tiles may not fit the row or panel. | Row constants and width not decoded. | High until V0 | V0. |
| R4 | Vector growth moves the sub-list buffer. Holders of old pointers are unknown. | D03, FUN_1404492f0 export | Medium to high | Find the holders of the sub-list pointer. V1 clicks after the append. |
| R5 | A row click changes DLL state (`0x1800C3158`), which can alter later selection. | §3.4, D05, D12 | Medium | V1 (e). |
| R6 | The content path for index 8 is undefined. | §3.5 | High | Trace the native content for selections above 1 and the DLL window states. |
| R7 | Save persistence of the sub-entry is unknown. | No evidence either way. | Medium | Test on a copy only. |
| R8 | All EXE offsets are valid only for the build stamp. | D01 build gate; listings unverified. | High if the build differs | V0. |
| R9 | Insertion can be deferred by the 240-call counter when `0x180064790` is nonzero. The handler's call frequency was not measured. | D01 | Low to medium | Observe timing in V1. |
| R10 | The icon size (96×96) and the texture's null handling are config claims. | C01 | Medium | Check the icon file. The DLL checks only for null. |
| R11 | Two `Collision` arities are documented in the `ui_panels` comments. | `ui_panels.cpp` lines 398 and 761 | Medium for Option B | Resolve before any Option B work. |
| R12 | The DLL log "opening the reputation window" can mislead a tester. | D05 | Low | Judge by observation, not by the log. |

## 8. Open items

1. The growth routine `0x14044C410`, and which code holds pointers into the sub-list.
2. The value of `[game+0x10398]` on the trade page across sessions.
3. The native content drawn for selections above 1 (not traced in this pass).
4. Which function draws each DLL window state that `0x18002F1E0` selects.
5. The row constants (origin and pitch) and the panel width.
6. The bottom-edge formula, and the host path at `0x14031ED26`.
7. Save persistence of the sub-list entry.
8. The writers of `0x1800C4C50` and `0x180411BC8`.

## 9. Corrections to earlier notes (this pass)

- **[D]** The mechanism report said the re-select write happens when "the entry is ours". The code does the opposite: it runs when the reputation entry is **not** selected, and only while a DLL window is open (§2.2). Corrected in [`reputation-sidebar-ui-mechanism.md`](reputation-sidebar-ui-mechanism.md).
- **[D]** The clamp `0x1800280E0` is reached only from the `exe+0x31E478` caller, not from both callers (§2.3). Corrected in the same report.
- **[D]** Path B has no capacity check before the native push, and its row-click reaction is a state change, not a verified window opening (§3.2, §3.4). Added as a correction in the same report.
- **[D]** The loop that stores `0x180411BCC` runs when **any** of six flags is nonzero. An earlier note said it ran only when `0x1800C4C50` was nonzero (§3.4).
- **[D]** `0x180411BCC` is not set by the trade-row click, as an earlier draft assumed. It is written only inside the case loop, which the dispatch guard excludes, so the dispatch uses a stale value (§3.4).
- **[D]** `0x180411BC8` is written by the selection handler at `0x180057702`. It is not an unresolved writer (§2.2).
- **[D]** The close block is gated by `sidebar_hides_panel` and by `0x180001C80`, not by `0x180286080` alone (§2.2).
- **[C]** C01 says the trade tab "works", that clicking it "opens our window", and that the game's panels draw items "GENERICALLY". The first two are not confirmed here, and the static trace does not show a window opening on the dispatch path (§3.4). "GENERICALLY" is partly contradicted by the index remap in the normal handler and by the `count-1` bound in the alternate (§4.4). C01's claims that `system` hardcodes a count of five and that the renderer has no null check were not checked.

## 10. Evidence index

- [`export/almanac-row-item-analysis-20261110/`](../export/almanac-row-item-analysis-20261110/): the excerpts, with an `INPUTS_sha256.txt` manifest.
  - `D01`–`D16`: DLL excerpts (insertion, selection, trade-row predicate, dispatch, window predicate, detours, AE54/AE58 writers, main entry append).
  - `N01`–`N08`: native listing excerpts (host dispatch, normal and alternate handler mapping, loop bounds, click writes, AE58 writes).
  - `C01`: configuration block for `sidebar_item`.
  - `tools/`: the scripts that generated the excerpts. They need `scratch/rep/disasm.txt`, which is regenerated with `objdump -d -M intel`.
- [`reputation-sidebar-ui-mechanism.md`](reputation-sidebar-ui-mechanism.md): the earlier mechanism report, with the corrections above.
- [`almanac-technical-findings.md`](almanac-technical-findings.md), [`almanac-export-review.md`](almanac-export-review.md), [`almanac-button-approach-review.md`](almanac-button-approach-review.md), [`almanac-reputation-review.md`](almanac-reputation-review.md): the earlier reviews this analysis builds on. Several of their statements are leads, as noted above.
