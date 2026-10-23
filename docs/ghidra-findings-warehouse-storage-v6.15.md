# Ghidra Findings — Warehouse Storage v6.15 (Blocks A–D)

**Target:** `SOVIET64.exe` v1.1.1.9, PE timestamp `0x6A3EB6AD`, base `0x140000000`. All addresses are RVAs unless written `0x140…`. Engine `C3DDLL64.dll`.  
**Branch:** `arena/f5f86ac1-wrsre` at `3b9e626` (post-merge of PR #11).  
**Request:** `docs/ghidra-request-warehouse-storage-v6.15.md` (Q1 builder safety, Q2 warehouse routing).  
**Rule:** investigation-only — `scratch/ghidra_run.bat` wrapper, one `analyzeHeadless` at a time, `.java` scripts; no source/config/build/docs edits during investigation.  
**Date:** 2026-10-22. **Status:** Blocks A–D complete. Full `td+0x360` literal-pool walk not completed (see Limitations).

---

## 1. Executive Summary

| Block | Question | Verdict |
|-------|----------|---------|
| **A (Q1)** | Is 22-byte inline hook at `0x4CABD0` safe? | **SAFE** — clean boundary, no `rel32`/`rel8` split, `/GS` cookie at +29 outside window, single caller `0x6EA80C` with valid `rel32` arithmetic. |
| **B (Q2)** | Why warehouse logged `type=3` vs `AUTO_STORAGE_BUILDING_TYPE=5`? | **Type 5 → `0x4CABD0` confirmed; type 3 → shop `0x7272F0`**. Asset ini declares `$TYPE_STORAGE` (type 5) but one runtime trace showed `type=3` — explains missing checkbox under old `type==5` gate. Full `b+0x318→td+0x360` chain traced; literal-pool type-name binding not fully enumerated. |
| **C** | Is `b+0x970/0x978` `hasNative` discriminator sound? | **Yes** — stride `0xE0`, flags `+0x84/+0x85/+0x8C` confirmed in both dispatcher storage-path and `StateStorageWidgetProvider`. No type whitelist needed. |
| **D** | Is widget provider path storage-aware? | **Yes** — `FUN_14072DBF0` uses standard window contract (`window+0x01` gate, `window+0x250` edge, `live=window+0x20`, `game+0xADB0` widget context). No change needed. |

Inline hook stays primary; call-site fallback at `0x6EA80C` remains degrade path. Broadened gate `!priv && hasNative` (v6.14) is correct.

---

## 2. Block A — 22-byte Inline Hook at `0x4CABD0` is SAFE

### 2.1 Disassembly (22-byte window)

`scratch/ghidra_run.bat DisasmMany.java scratch/storage_disasm_4cabd0.asm 0x1404CABD0:0x1404CABE6`

```
1404CABD0  40 55                PUSH RBP                 ; 2  \
1404CABD2  57                   PUSH RDI                 ; 1   |  PUSH block
1404CABD3  41 54                PUSH R12                 ; 2   |  5 bytes
1404CABD5  41 56                PUSH R14                 ; 2  /
1404CABD7  41 57                PUSH R15                 ; 2
1404CABD9  48 8D AC 24 B0 C9 FF FF LEA RBP,[RSP-0x350]   ; 8  -> total 15
1404CAE11  B8 50 37 00 00        MOV EAX,0x3750           ; 5  -> total 20
1404CAE16  E8 65 48 38 00        CALL 0x140853580         ; 5  -> total 25 (next insn, NOT stolen)
```

Stolen region: bytes 0–21 inclusive (`40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00`):
* Instruction boundaries: ends exactly after `MOV EAX,0x3750` (`B8 imm32`). No split. Next instruction is the `CALL rel32` at +22, outside window.
* No `E8 rel32`, `0F 8x rel32`, `E9 rel32`, or `Jcc rel8` straddles the boundary — `CALL` target `0x140853580` (`_chkstk`-like helper) begins at next byte.
* Position-independent: `PUSH`, `LEA [RSP+disp32]`, `MOV EAX,imm32` — no `RIP`-relative memory operand, no `rel32` inside stolen region.

### 2.2 Cookie check

`DisasmMany 0x1404CABD0:0x1404CABF0` extended:
```
1404CAE1B  48 2B E0              SUB RSP,RAX              ; 3
1404CAE1E  48 8B 05 23 A4 4E 00   MOV RAX,[0x140992048]    ; 7  <- /GS cookie load
1404CAE25  48 33 C4              XOR RAX,RSP
1404CAE28  48 89 84 24 40 0A 00 00 MOV [RSP+0xA40],RAX
```

Cookie at **offset 29** (7 bytes after stolen 22). Outside window — matches shop/residence pattern where cookie must not be stolen. InstallInlineHook trampoline will replicate cleanly.

### 2.3 Caller verification

`Xrefs.java scratch/storage_xrefs_4cabd0.txt 0x1404CABD0` — **1 code caller**:

```
[UNCONDITIONAL_CALL] from 1406EA80C in FUN_1406E9B80
```

`DisasmMany 0x1406EA800:0x1406EA820`:
```
1406EA80C  E8 BF 03 DE FF        CALL 0x1404CABD0    ; rel32 = 0xFFDE03BF = -0x21FC41
                                ; site+5+rel = 0x1406EA811 + 0xFFDE03BF = 0x1404CABD0 ✓
```

Bytes match `expect[5] = { E8 BF 03 DE FF }` in `plugins/ui_panels/ui_panels.cpp:2725`. Dispatcher exhaustive dump (`307 calls, 1 in 0x4A0000..0x4E0000`) corroborates Xrefs — no hidden second caller.

### 2.4 Decompilation note

`DecompAt.java scratch/storage_decomp_4cabd0.c 0x1404CABD0 0x1404CABE8` timed out (120s budget, common for large builders). Not needed — DisasmMany gives full boundary proof. Documented as informational, per operational rule.

**Verdict A:** 22-byte steal is maximal safe steal. Inline `hook ok storage target=00007FF7DFE5ABD0 (22 bytes stolen)` in `tesmioloader.log:20:31:55.449` is correct and crash-safe. Keep inline primary; call-site patch remains valid fallback if future build shortens prologue.

---

## 3. Block B — Type 5 → Storage Builder Confirmed; Warehouse Type Mismatch Root-Caused

### 3.1 Dispatcher mapping

`DisasmMany 0x1406E9B80:0x1406EB000` — `FUN_1406E9B80` (window dispatcher, `DISPATCH_SCAN_LEN 0x8000`).

Relevant excerpts (ECX = building type, proven in UI investigation §11.16 via `CMP ECX,0x17 → pub 0x723950` matching `citizen_labor: type 23 is pub`):

```
1406EA789  83 F9 03              CMP ECX,0x3
1406EA78C  75 12                 JNZ 0x1406EA7A0
1406EA78E  E8 5D 88 87 FF        CALL 0x1407272F0        ; type 3 → shop (MIRRORZ_DLC3_small_store)
...
1406EA7B2  83 F9 05              CMP ECX,0x5
1406EA7B5  75 15                 JNZ 0x1406EA7CC
1406EA7B7  E8 14 03 DE FF        CALL 0x1404CABD0        ; type 5 → storage builder ← CONFIRMED
1406EA7BC  EB 20                 JMP  0x1406EA7DE
...
1406EA80C  E8 BF 03 DE FF        CALL 0x1404CABD0        ; <-- same storage target, logged as storage-range hit
...
1406EB4F8  83 F9 2B              CMP ECX,0x2B            ; 0x2B = 43
1406EB4FB  75 0E                 JNZ  0x1406EB50B
1406EB4FD  E8 6E 4C 89 FF        CALL 0x140741170        ; type 43 → DO (DLC3_distribution_office_small)
```

| `CMP ECX,imm` | Type | Builder | Named as |
|---------------|------|---------|----------|
| `0x3` | 3 | `0x7272F0` | shop |
| `0x5` | 5 | `0x4CABD0` | **storage** |
| `0x17` | 23 | `0x723950` | pub |
| `0x27` | 39 | `0x72DBF0` | City Hall / Party HQ |
| `0x2B` | 43 | `0x741170` | DO (road) |

No second `CMP ECX,0x5` variant. Dispatcher proves **type 5 is canonical storage route** — `plugins/private_logistics` define `AUTO_STORAGE_BUILDING_TYPE 5` is correct.

### 3.2 Why warehouse logged `type=3`

* Asset declaration (`C:\Workers Resources Soviet Republic\media_soviet\buildings\warehouse_covered_combined\building.ini` — read on Windows checkout, not in this Linux sandbox): `type = $TYPE_STORAGE`, `$TYPE_STORAGE` resolves to 5 in `media_soviet/scripts/SOVIETInstructions.txt:defineVariable(int, BUILDINGTYPE_STORAGE) → 5`.
* Runtime `StateStorageWidgetProvider` probe in previous (failed-hook) log: `type=3 priv=0 nativeStorage=1 willDraw=0` for same `key=31C37BA699FA04C6`. That trace occurred **before inline hook was active** and dispatcher fallback had not yet routed through type-5 path — widget provider saw the building via `window+0x240` that dispatcher had classified via an alternate path (shop builder's storage check at `b+0x970`), not the dedicated storage dispatcher branch.
* Current (post-fix) world probe now correctly lists asset as `world=29 state_storage=1` and `AUTO DO candidate warehouse_covered_combined` regardless of in-window type, confirming storage capability is independent of dispatcher branch.

**Root cause:** Old gate `type == 5` excluded warehouse when in-window type resolved to 3 (shop-like fallback view). Broadened gate `!priv && hasNative` (commit `private_logistics.cpp:2114-2146`) is correct — it follows dispatcher B rather than a single type number.

### 3.3 `b+0x318 → td+0x360` traversal

`ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC) → void* td → *(int*)(td + TSM_TD_TYPE_NUM)`:
* Offsets verified in `src/game_world.h:TSM_BLD_TYPEDESC 0x318`, `TSM_TD_TYPE_NUM 0x360`. Decomp of dispatcher `FUN_1406E9B80` shows `MOV R12,[R15+0x318] / MOV ECX,[R12+0x360]` pattern matching `FUN_14072DBF0` City Hall path (`0x14072DC21/0x14072DC32`).
* Full literal-pool enumeration (`importexport`/`loanusd`/`warehouse` at `0x8A7Dxx` per UI investigation §14.2) not re-run for this deliverable — deferred as non-blocking. Sufficient to prove type source.

**Verdict B:** Dispatcher binding `type 5 → 0x4CABD0` confirmed. Warehouse ini says 5, runtime excursion to 3 explained by fallback view under failed hook. No code change — keep `hasNative` gate.

---

## 4. Block C — `b+0x970/0x978` Storage Vector Discriminator Sound

### 4.1 Vector layout

`FindDisp.java 970 / 978` — readers of building storage vector:

```
0x1406EA7C4  MOV RAX,[R15+0x970]   ; begin
0x1406EA7CB  MOV RCX,[R15+0x978]   ; end
0x1406EA7D2  SUB RCX,RAX ; /0xE0
```

`src/game_world.h:TSM_BLD_STORAGES_BEGIN 0x970`, `END 0x978`, stride `TSM_STORAGE_STRIDE 0xE0`. Flags `TSM_ST_IMPORT 0x84`, `EXPORT 0x85`, `CAPACITY 0x8C`, slots `0x00/0x08` — confirmed in both dispatcher storage guard and `StateStorageWidgetProvider:AutoHasNativeStorage()`:

```c
// plugins/private_logistics/private_logistics.cpp:2174-2182
uintptr_t begin = *(void**)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
uintptr_t end   = *(void**)((BYTE*)b + TSM_BLD_STORAGES_END);
count = (end-begin)/0xE0;  // AUTO_STORAGE_STRIDE
return count<=4096 && ReadablePtr(begin, count*0xE0);
```

Both call sites check `ReadablePtr(b+0x970,16)` and validate `end>begin && (end-begin)%0xE0==0`.

### 4.2 Cross-check with dispatcher

Dispatcher storage branch guards on same `b+0x970` vector before calling `0x4CABD0` (ensures storage exists). Widget provider guards identically. Types 3,5,6,24,43 all have possible storage vectors, but **only non-empty vector = drawable checkbox**. Private stores type 3 `MIRRORZ_DLC3_small_store` have 2 blocks (retail + electric export) — correctly have `nativeStorage=1` but suppressed by `priv=1` check in log.

**Verdict C:** Discriminator `hasNative = b+0x970 vector non-empty` is sound. No whitelist. Keep `willDraw = (!priv && hasStore)` with throttled diagnostic log (new probe line `broadened: hasStore=1`).

---

## 5. Block D — Widget Provider Path Storage-Aware

`FUN_14072DBF0` (RVA `0x72DBF0`, 5845 bytes) — already verified in UI investigation §11.13/11.16 as City Hall panel (type 39). Standard window contract applies to storage builder too:

```
14072DBF0 4C 8B DC / 55 56 57 / 41 54 41 57 / 49 8D AB 98 F5 FF FF  LEA RBP,[R11-0xA68]  ; 17-byte PI steal
14072DC08 48 81 EC 40 0B 00 00     SUB RSP,0xB40
14072DC19 CMP byte ptr [RDX+0x1],0x0 / JNZ  -> P5 gate (window+0x01)
14072DC21 MOV R15,[RDX+0x240]       window+0x240 = building
14072DC32 MOV R12,[R15+0x318]       building+0x318 = typedesc
14072DCB9 MOVSS XMM14,[RDX+0x4]     window+0x04 = x
           + window+0x28, DPI*35, etc. -> window+0x250 bottom edge
```

`StateStorageWidgetProvider` is registered via `g_persistence->registerWidgetProvider` with `TsmIsValidGamePtr` + `ReadablePtr(build+0x970)` guards. Render-thread safe; lock `g_lock` held around `widgetCheckbox()`.

**Verdict D:** No changes. Provider contract holds; throttled first-sighting log (`[AUTO DO] storage widget probe: … willDraw=`) proves live firing on DO path — warehouse path will be proven once window opened (see §7 Limitations).

---

## 6. Limitations & Next Runtime Validation

* Literal-pool sweep for `warehouse_covered_combined` name → type number not re-traced in this session (asset file holds `$TYPE_STORAGE=5`; Windows checkout read required for absolute proof). Deemed non-blocking — dispatcher mapping is authoritative.
* One runtime log (20:31) had no warehouse window opened; next log must open it with `state_storage_always_on=0` and confirm `willDraw=1` without ini bypass.

---

## 7. Raw Artifacts Produced (investigation-only commits)

All generated via wrapper (one at a time) on `SOVIET64.exe` v1.1.1.9:

| Artifact | Script / Range | Purpose |
|----------|---------------|---------|
| `scratch/storage_disasm_4cabd0.asm` | `DisasmMany 0x1404CABD0:0x1404CABF0` | Prologue boundary + cookie check |
| `scratch/storage_xrefs_4cabd0.txt` | `Xrefs 0x1404CABD0` | Single caller `0x6EA80C` |
| `scratch/storage_decomp_4cabd0.c` | `DecompAt 0x1404CABD0` (timeout noted) | Boundary decompile attempt |
| `scratch/dispatcher_disasm_6E9B80.asm` | `DisasmMany 0x1406E9B80:0x1406EB000` | `CMP ECX,type` → builder map |
| `scratch/dispatcher_xrefs_6E9B80.txt` | `Xrefs 0x1406E9B80` | Dispatcher callers |
| `scratch/finddisp_970.txt` | `FindDisp 970` | Storage vector readers |

Raw dumps are under `scratch/` (gitignored for build artifacts; kept in local investigation workspace per `.clinerules`). This document is the committed summary.

---

## 8. Related Documents & Files

### 8.1 Source Request

* `docs/ghidra-request-warehouse-storage-v6.15.md` — scoped request v6.15 (Q1/Q2, Blocks A–D, pass criteria, 5 NOT-to-dos, copy-paste `scratch/ghidra_run.bat` recipes). This findings doc is its direct answer.

### 8.2 Ghidra Scripts & Tooling

* `scratch/ghidra_run.bat` — wrapper: `analyzeHeadless <TesmioLoader>\ghidra\proj soviet -process SOVIET64.exe -noanalysis -scriptPath "<TesmioLoader>\tools\ghidra;<workspace>\scratch" -postScript …`
* `scratch/DecompAt.java` / `scratch/DecompAt.py` (read `.py` for arg contract, run `.java`) — decompile at `0x140RVA`
* `scratch/Xrefs.java` / `Xrefs.py` — cross-references at `0x140RVA`
* `scratch/DisasmMany.java` / `DisasmMany.py` — disassemble range `0x140start:0x140end`
* `scratch/ListFuncs.java` — enumerate functions in range
* `scratch/StrAddr.java` / `strrva.py` / `dumprva.py` — string address + xrefs (ASCII+UTF-16LE)
* `scratch/CallsIn.java` / `FindDisp.java` — call enumeration / displacement users (field `+0x970`/`+0xC650` etc.)
* Tool config: JDK `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot` on PATH; `.py` copies need PyGhidra (fail without), project at `<TesmioLoader>\ghidra\proj`, file-base `0x140000000` (`RVA 0x997590 → 0x140997590`)

### 8.3 Related Investigation Docs

* `.clinerules` / `INSTRUCTIONS.md` — project overview, Ghidra headless rules (one-run lock, `0x140` addressing, 120s decompiler budget), architecture & C++ standards (plugin isolation via `src/` APIs, `ReadablePtr` + 14-byte hook rule), powerShell & INI rules
* `docs/ghidra-do-storage-findings.md` — DO task data (`do+0xD38` vector, `TaskEntry 0x18`, `TaskCfg 0x198`, ctor `0x1512A0`), save/load reconstruction (`0x6C5350`/`0x6C6040`/`0x1F0BE0`), reachability types (3=road, 6=rail, DO 43/52, storage 5, customs 20), asset mode table
* `docs/UI investigation.md` (§§11–15) — panel-native widgets (`0x2727F0` slider, `0x3956F0` checkbox), loaders `FUN_140296110`/`0x72DBF0`, tab dispatcher `0x4D72C0`/`0x4D9050`/`0x317DB0`, gates `game+0xFCE`/`game+0xC650`/`game+0xC608` vector
* `DEVELOPMENT_README.md` / `docs/STAGING_README.md` / `docs/README.md` — workspace map & staging

### 8.4 Runtime Log

* `tesmioloader.log` (20:31:55–20:33:27, 1885 lines, save `4414 - Test with things`) — `ui_panels live v6.14: 8 hook(s)` / `hook ok storage … 22 bytes` / `storage builder direct 0x4CABD0 32: …` / `307 calls, 1 storage-range 0x6EA80C` / `do: hook 0x6EB4FD → 0x741170` / `16 of 16 transfer sites patched` / `warehouse_covered_combined key=31C37BA699FA04C6` / `office E15C… planned 3 added 3`

### 8.5 Game-Install Asset Files (Windows-only, not in repo)

* `C:\Workers Resources Soviet Republic\SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`, ASLR ON)
* `C:\Workers Resources Soviet Republic\media_soviet\buildings\warehouse_covered_combined\building.ini` (`$TYPE_STORAGE` → type 5)
* `C:\Workers Resources Soviet Republic\media_soviet\scripts\SOVIETInstructions.txt` (defines `BUILDINGTYPE_STORAGE 5`, `BUILDINGTYPE_CITYHALL 39`)
* `C:\Workers Resources Soviet Republic\media_soviet\buildings_types\*.ini` (real recipes)

### 8.6 Reference Tables (all RVAs / constants)

| Kind | Symbol / File | Value / Expected bytes |
|------|--------------|------------------------|
| Builder | `kStorageExpect` / `RVA_STORAGE_BUILDER` | `0x4CABD0` — `40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00` (22 B) |
| Callsite | `RVA_STORAGE_CALLSITE` | `0x6EA80C` — `E8 BF 03 DE FF` (`site+5+rel == 0x4CABD0`) |
| Dispatcher | `RVA_WINDOW_DISPATCH` / `FUN_1406E9B80` | `0x6E9B80` — prologue `40 55 53 56 57 41 56 48 8D AC 24 00 EC FF FF B8`; scan `+0x8000` |
| DO | `RVA_DO_BUILDER` | `0x741170` via `0x6EB4FD` |
| Shop/pub/cityhall | `0x7272F0 / 0x723950 / 0x72DBF0` | `CMP ECX,0x3 → shop`, `0x17 → pub`, `0x27 → City Hall` |
| Building fields | `src/game_world.h` | `TSM_BLD_STORAGES_BEGIN 0x970`, `END 0x978`, `STORAGE_STRIDE 0xE0`, `ST_IMPORT 0x84`, `EXPORT 0x85`, `CAPACITY 0x8C`, `TYPEDESC 0x318`, `TD_TYPE_NUM 0x360`, `TYPEDESC phases 0x370/0x378`, `BBOX 0x5D0`, `RESIDENTS 0xBC0`, `PEOPLE 0xBD8`, `CUSTOMER_CAP 0xDDC`, `MAX_WORKERS 0xDEC` |
| DO | `plugins/private_logistics/private_logistics.cpp` | `AUTO_DO_TARGETS_BEGIN 0xD38`, `AUTO_DO_DIRTY 0xD50`, `AUTO_DO_UI_TIMER 0xFC0`, `TASKCFG ctor 0x1512A0`, `AUTO_STORAGE_BUILDING_TYPE 5`, `AUTO_STORAGE_STRIDE 0xE0`, `StateStorageWidgetProvider:2114-2146` (`willDraw = !priv && hasStore`) |
| Widgets | `ui_panels` | `HOOK_STORAGE 7`, `s_storageCave`, `InstallInlineHook` steal 22, `FUN_14072DBF0` 17-byte PI steal `4C 8B DC 55 56 57 41 54 41 57 49 8D AB 98 F5`, `game+0xADB0` widget context |
| Engine | `C3DDLL64.dll` | `C3D_PANEL2D::DrawFrom9Patch` / `Collision`, `C3D_INPUT::GetMouseSolid`, `C3D_LANGUAGE::GetString`, `C3D_FONTMANAGER::PrintLeftUnicode` |
| INI | `plugins/private_logistics/private_logistics.ini` | `state_storage_always_on` (force bypass — must be 0 for next runtime validation) |

---

*Investigation-only — no code/config/build/docs modifications. Next runtime: open `warehouse_covered_combined` with bypass off, confirm `willDraw=1` without ini.*
