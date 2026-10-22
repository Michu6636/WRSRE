# Ghidra Request v6.15 — Warehouse Checkbox + Storage Hook Verification

**Branch:** `arena/5f158a5a-wrsre` (at `6e3fffd` — merge of #10 syntax fix `!=0))` → `!=0)`).  
**Target binary:** `SOVIET64.exe` v1.1.1.9, timestamp `0x6A3EB6AD`, base `0x140000000`; all addresses below are RVAs unless noted. Engine `C3DDLL64.dll`.  
**Author:** code agent (Arena). Date 2026-10-22.  
**Rule:** investigation-only — use `scratch/ghidra_run.bat` wrapper, one `analyzeHeadless` at a time, `.java` scripts; never edit source/config/build/docs during investigation.

---

## 1. Why this request exists (log proves fix is live, but two questions remain)

The attached `tesmioloader.log` (20:31:55 – 20:33:27, 1885 lines, save `4414 - Test with things`) is the first log where **v6.14 builds**:

* `ui_panels live v6.14: 8 hook(s)` — storage no longer `FAILED`.  `hook ok storage target=00007FF7DFE5ABD0 tramp=... (22 bytes stolen)` — the `B0 C9` prologue is now accepted.  
* `storage builder direct 0x4CABD0 32: 40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00 E8 65 48 38 00 …` — matches the patched `kStorageExpect` (22 B).  
* `dispatcher E8 at 0x6EA80C -> 0x4CABD0 head 40 55 57 41 54 41 56 41 … <- storage-range` and `storage diag: 307 calls, 1 in storage range 0x4A0000..0x4E0000` — exhaustive dispatcher dump proves **exactly one storage builder** in this build. The “warehouse variant builder” hypothesis is disproved; all buildings route through `0x4CABD0`.  
* `do: hook 0x6EB4FD -> DO builder 0x741170 ok (cave 00007FF7E0490000). The PRIV tile now draws on a distribution office window.` + repeated `[AUTO DO] storage widget probe: building=… type=43 key=E15C… priv=1 nativeStorage=1 willDraw=0` — DO tile fix (type 43 `DLC3_distribution_office_small`) verified: widget fires on every DO frame and correctly suppresses checkbox on private DOs (broadened gate `!priv && hasStore` landing in `private_logistics.cpp:2114-2146`).

Log conclusions and why Ghidra is still needed:

1. **Freight is LIVE and billable:** `private_logistics hook 0x68F792/0x68F7F9/0x694F40/0x6953C4 … patched 16 of 16`, `billing=1 dry_run=0`, `[PAY] freight … 3.48 = fuel 2.06 x1.35 + wear 0.70`, `[BAL] treasury 9970480 -> 9970531 (+50.51)` exact.  
2. **Warehouse as source is AUTO but still behind an ini bypass:** `world=29 state_storage=1 private_storage=0`, `candidate name=warehouse_covered_combined key=31C37BA699FA04C6 … permission=OFF-DEFAULT force-enabled via ini (was OFF-DEFAULT)`, `office=E15C… type=43 existing=0 planned=3 added=3`. The Auto DO engine correctly discovers the warehouse and adds 3 tasks to the private DO — **but only because `state_storage_always_on=1` forces the permission**. Acceptance requires the checkbox to draw natively on the warehouse window *without* the ini.  
3. **Warehouse checkbox not yet verified in this log:** the only `storage widget probe` in the entire log is the DO (`type=43`). The user never opened the `warehouse_covered_combined` window, so we have no `type=X willDraw=1` proof for it. We need to know *which builder* the warehouse window dispatches to, why `type 5` vs `type 3` diverges from the scan (`private_shops -- private, finished buildings --` lists `MIRRORZ_DLC3_small_store` type 3 as the only private shop type 3, plus fabric factory type 6 and DO type 43), and whether `AutoHasNativeStorage` (`b+0x970/0x978`, stride `0xE0`) is the right discriminator (it was broadened from `type==5` precisely because warehouse was `type 3` in the previous log).

So the two open Ghidra questions are:

* **Q1 — Storage builder safety:** is `RVA 0x4CABD0` 22-byte prologue `40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00` truly 22 stolen bytes that never split a `rel32`/`rel8`/cookie, ending on an instruction boundary? Needed to certify inline hook vs. keeping the call-site fallback.  
* **Q2 — Warehouse vs. generic storage routing:** why does `warehouse_covered_combined` appear as `type=3` in-window in the prior (failed-hook) log, while `AUTO_STORAGE_BUILDING_TYPE=5` is the historical “state storage” type? Is the discriminator `b+0x318 -> td+0x360` type number, `b+0x970` storage vector, or the dispatcher case table at `0x6E9B80`? This decides whether `willDraw = !priv && hasNative` is correct, or needs a type list.

---

## 2. Exact work to run (copy-pasteable, one headless session each)

All addresses are `0x140`+RVA. Use the new helper tools proven in docs/UI investigation §11.14:

* `scratch/strrva.py <exe> <needle>` → RVA
* `scratch/StrAddr.java <out> <needle>` → data address + xrefs (ASCII+UTF-16LE)
* `scratch/CallsIn.java <out> <from> <to>`
* `scratch/FindDisp.java <dispHex>`

### Block A — Storage builder prologue (answers Q1)

> Reuses the same verification recipe that certified shop `0x7272F0` / residence `0x761300` / pub `0x723950` / factory `0x799630` / cityhall `0x72DBF0` in docs/11.1-11.4.

1. `DisasmMany 0x1404CABD0:0x1404CABE0` → `scratch/storage_disasm_4cabd0.asm` — print 16–22 bytes with instruction lengths, mnemonic, and operand. Explicitly call out whether any `E8 rel32` / `0F 8x rel32` / `E9 rel32` / `Jcc rel8` straddles the 22-byte boundary.  
2. `DisasmMany 0x1404CABD0:0x1404CABF0` extended 32-byte dump — confirm next instruction after 22 B is a boundary (`E8 65 48 38 00` call at +0x15 in the log) and that bytes 22-31 in the log (`E8 65 48 38 00 48 2B E0 48 8B …`) decode cleanly.  
3. `DecompAt 0x1404CABD0 0x1404CABE8` → `scratch/storage_decomp_4cabd0.c` (if it times out, note and stay with DisasmMany — 32 B is cheap).  
4. Report `InstallInlineHook` stolen size: is `sizeof(kStorageExpect)=22` the largest clean steal, or should it be 21/23? Check for `/GS` cookie (`MOV RAX,[0x140992048]` pattern) after steal — prologue must end **before** it, like shop did.  
5. `Xrefs 0x1404CABD0` → `scratch/storage_xrefs_4cabd0.txt` — enumerate distinct call sites (expected log: one at `0x6EA80C`). Cross-check `DisasmMany 0x1406EA800:0x1406EA820` — the call `E8 BF 03 DE FF` (`expect[5]`) and its `rel32` arithmetic `site+5+rel == target`.

**Pass criteria:** 22 stolen bytes end on instruction boundary, no relative operand split, no cookie inside window, one caller `0x6EA80C` confirmed. If any relative crosses, report correct steal (14-22) and recommend staying on call-site fallback (keep inline disabled).

### Block B — Dispatcher → building-type mapping (answers Q2, root cause of warehouse checkbox)

> Same lane as UI investigation §11.16 where `CMP ECX,0x3 -> shop 0x7272F0 / CMP ECX,0x17 -> pub 0x723950 / CMP ECX,0x27 -> City Hall 0x72DBF0` proved ECX is building type.

1. `DisasmMany 0x1406E9B80:0x1406EB000` (at least `0x1406E9B80` + `DISPATCH_SCAN_LEN 0x8000`) — the full `FUN_1406E9B80` switch/jump-table dispatch that enumerates `CMP ECX,imm` / `JNZ` chains. Extract **every** `CMP ECX,0xN` where N compares to known type constants, and the `CALL` target that follows (the panel builder). Already known from log dump: `0x6EA80C -> 0x4CABD0` is inside this dispatcher.  
2. Pay special attention to:
   * `type 3` (`CMP ECX,0x3` → `0x7272F0` shop) — why did warehouse_covered_combined log as `type=3` in-window? Is warehouse using the shop builder (i.e., the shop storage path), or does dispatcher have a separate warehouse builder we missed because dump only tagged `storage-range 0x4A0000..0x4E0000`?  
   * `type 5` (`CMP ECX,0x5` expected) — find its `CALL` target; is it `0x4CABD0` or another builder? Compare to `AUTO_STORAGE_BUILDING_TYPE` define.  
   * `type 24` (`MIRRORZ_DLC3_small_store` / `obj "MIRRORZ_DLC3_small_store"` in `private_shops` storage probe), `type 3/6/43` (small_store/fabric/DO from log), type 39 City Hall (`0x27` → `0x72DBF0`+`0x6F8CC0`) — re-confirm ECX mapping.  
3. `DecompAt 0x1406E9B80 0x1406EA100` (small slice) + `Xrefs 0x1404CABD0` within dispatcher context to bind `type -> builder`. If decompile times out (large function), use `DisasmMany` chunks (`0x6EA7C0`/`0x6EA80C`/`0x6EB4FD` neighbourhoods — DO at `0x741170` sits nearby and is the only other proven dispatcher target at `0x6EB4FD`).  
4. For `warehouse_covered_combined` itself (asset-side): locate its type descriptor literal to read its `TDM_type` (offset `TDM_TD_TYPE_NUM 0x360`). The `.rdata` block for shop/storage names in §14.2 of UI investigation doc (`importexport`, `loanusd`, `warehouse`, etc. at `0x8A7Dxx`) is a candidate literal pool — check if warehouse name string sits there and which type number it carries. If not in exe, note that asset type lives in `media_soviet/buildings/…/building.ini` and must be read from Windows checkout (state verbatim, do not invent).

### Block C — `AutoHasNativeStorage` discriminator (validates the broadened gate)

The plugin gate now is `!priv && hasNative` where `hasNative = b+0x970..978 vector nonEmpty, stride 0xE0, readable`. Verify this is sound:

1. `FindDisp 970` / `FindDisp 978` → readers of `b+0x970/0x978` (storage vector). Cross with building-type filters to confirm which types have non-empty vectors (storage-capable buildings) vs. which are pure retail (shops type 3).  
2. `DecompAt` any factory/shop/warehouse storage probe that checks `*(b+0x970)` and decides import/export — confirm stride is indeed `0xE0` and flags at `+0x84/+0x85` (enterprise_base comment: `TSM_ST_IMPORT 0x84 / EXPORT 0x85 / CAPACITY 0x8C`).  
3. Compare to `TsmBuildingType(b)` path: `b+0x318 (TDM_TD) -> +0x360 (TYPE_NUM)` — confirm it is the same `ECX` the dispatcher uses, so both discriminators agree.

### Block D — Safety for the widget provider

This is already proven for shop/cityhall/citizen but do a spot check for storage path:

1. Confirm `StateStorageWidgetProvider` is reached via `ui_panels` hook (the same `FUN_14072DBF0` family already verified for City Hall — see UI investigation §11.13/11.16: 17-byte steal `4C 8B DC … 49 8D AB 98 F5`, `window+0x01` gate, `window+0x250` bottom edge, `live=window+0x20`, `game+0xADB0` widget context).  
2. Re-run `DisasmMany 0x14072DBF0:0x14072DF00` head to keep the canonical prologue in this report.

---

## 3. Deliverable format (strict, investigation-only)

* One markdown under `scratch/` per block: `ghidra-storage-A.md`, `ghidra-storage-B.md`, etc., plus raw dumps (`*.asm`, `*.txt`, `*.c` where captured). Commit them via `git add scratch/…` to `arena/5f158a5a-wrsre` — do **not** modify `plugins/`, `Tsm*.h`, `src/game_world.h`, or docs yet.  
* Each block ends with a 5-line **Verdict**: (a) stolen bytes for `0x4CABD0`, (b) whether inline is safe or call-site fallback must stay primary, (c) warehouse building type number(s) and which dispatcher builder they hit, (d) whether `hasNative` discriminator is correct vs. needing a type whitelist, (e) any discrepant defines (`AUTO_STORAGE_BUILDING_TYPE=5`, `TSM_BLD_STORAGES_BEGIN=0x970`) to update.  
* Report exact errors: if `DecompAt` times out, `FindDisp` is noisy, or `analyzeHeadless` locks, log the error line and continue — never mask failure as success.  
* Do not push to `main`; only `git push origin arena/5f158a5a-wrsre`.

---

## 4. Appendix — RVAs, byte patterns and log excerpts for copy-paste

| Symbol | RVA (file) | Log excerpt / expected bytes |
|---|---|---|
| `kStorageExpect` | `0x4CABD0` | `40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00` (22 B) — log: `storage builder direct … 40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00 E8 65 48 38 …` |
| `RVA_WINDOW_DISPATCH` | `0x6E9B80` | prologue `40 55 53 56 57 41 56 48 8D AC 24 00 EC FF FF B8` (16 B) |
| `RVA_STORAGE_CALLSITE` | `0x6EA80C` | `E8 BF 03 DE FF` — `site+5+rel == 0x4CABD0`; `Xrefs` should show single call inside dispatcher |
| `RVA_DO_BUILDER` | `0x741170` | `do: hook 0x6EB4FD -> DO builder 0x741170 ok` — neighbouring proof the dispatcher also routes DO correctly |
| `shop / pub / cityhall` | `0x7272F0 / 0x723950 / 0x72DBF0` | ECX=3→shop, 0x17=23→pub (citizen_labor: “type 23 is pub”), 0x27=39→CityHall — anchor that ECX is building type |
| `TOM_BLD_*` | `+0x970/0x978` storages, `+0x318 td, td+0x360` type, `+0x108 CITY`, `+0xBC0 workers, +0xBD8 visitors` | see `src/game_world.h` |

**Log evidence that inline hook is the correct host (not call-site forever):** “dispatcher E8 at 0x6EA80C -> 0x4CABD0 <- storage-range” and “storage diag: 307 calls, 1 in storage range” — the same builder is dispatched exactly once, so hooking the builder directly is *why* warehouse is now potentially catchable even when opened outside dispatcher warm path (v6.13 note). Fallback call-site at `0x6EA80C` stays as degrade path.

---

## 5. What NOT to do

* Do not `git mv` / reformat / push to `main`; history (`git mv`) must be preserved per repo rules, but not as part of this investigation commit.  
* Do not claim warehouse_covered_combined is definitively type 5 because `AUTO_STORAGE_BUILDING_TYPE=5`; the previous failed-hook log showed `type=3 willDraw0` for that asset — let Ghidra settle it.  
* Do not set `private_logistics.ini state_storage_always_on=0` in this investigation — that test belongs to the next runtime pass after Ghidra verdict.

*— end request —*
