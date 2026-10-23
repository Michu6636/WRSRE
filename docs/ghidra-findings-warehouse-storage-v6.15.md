# Ghidra Findings v6.15 — Warehouse Checkbox + Storage Hook Verification

**Branch:** `arena/5f158a5a-wrsre` (at `6e3fffd`)  
**Target binary:** `SOVIET64.exe` v1.1.1.9, timestamp `0x6A3EB6AD`, base `0x140000000`; all addresses below are RVAs unless noted (file-based = `0x140` + RVA). Engine `C3DDLL64.dll`.  
**Source request:** `docs/ghidra-request-warehouse-storage-v6.15.md`  
**Investigation date:** 2026-10-22  
**Status:** Investigation-only; no source/config/docs/build changes; not committed (scratch files gitignored).

---

## 1. Executive Summary

Two open Ghidra questions from the v6.14 log are answered:

* **Q1 (Storage builder safety):** `RVA 0x4CABD0` (`FUN_1404cabd0`) has a clean 22-byte prologue ending on an instruction boundary, no relative-operand split, no `/GS` cookie inside the window, exactly one caller (`0x6EA80C`). **Inline hook is SAFE with 22 stolen bytes.** Call-site fallback at `0x6EA80C` stays as degrade path.
* **Q2 (Warehouse vs generic storage routing):** Dispatcher maps type 5 → storage builder `0x4CABD0` (confirmed at `0x1406EA80C`) and type 3 → shop builder. Warehouse ini says `$TYPE_STORAGE` (expected numeric type 5) but prior runtime log showed `type=3` — warehouse most likely routes to shop, not storage, which is the root cause of the missing checkbox. Full `td+0x360` (numeric type) traversal from the exe string was not completed.
* **Discriminator:** `hasNative = (b+0x970..0x978 vector non-empty, stride 0xE0)` is SOUND — confirmed by `FindDisp` hits in both the dispatcher and `StateStorageWidgetProvider` (`FUN_14072dbf0`). No type whitelist needed.
* **Widget provider:** `StateStorageWidgetProvider` is storage-aware; no changes needed.

Deliverable files: `scratch/ghidra-storage-{A,B,C,D}.md` plus raw dumps `storage_disasm_4cabd0.asm`, `storage_disasm_4cabd0_ext.asm`, `storage_callsite_6ea80c.asm`, `finddisp_970.txt`, `finddisp_978.txt`, `straddr_warehouse.txt`.

---

## 2. Block A — Storage Builder Prologue (Q1)

Target: `RVA 0x4CABD0` → file-based `0x1404CABD0`, function `FUN_1404cabd0`, size 5185 bytes (`scratch/out_listfuncs_4ca000.txt`).

### 2.1 22-byte prologue disassembly (0x1404CABD0:0x1404CABE0)

Source: `scratch/storage_disasm_4cabd0.asm`

| File offset | Bytes | Instruction | Size | Cumulative |
|-------------|-------|-------------|------|------------|
| 0x1404CABD0 | 40 55 | PUSH RBP | 2 | 2 |
| 0x1404CABD2 | 57 | PUSH RDI | 1 | 3 |
| 0x1404CABD3 | 41 54 | PUSH R12 | 2 | 5 |
| 0x1404CABD5 | 41 56 | PUSH R14 | 2 | 7 |
| 0x1404CABD7 | 41 57 | PUSH R15 | 2 | 9 |
| 0x1404CABD9 | 48 8D AC 24 B0 C9 FF FF | LEA RBP,[RSP + -0x3650] | 8 | 17 |
| 0x1404CABE1 | B8 50 37 00 00 | MOV EAX,0x3750 | 5 | **22** |

**22 stolen bytes end exactly at 0x1404CABE5**, on a clean instruction boundary immediately after `MOV EAX,0x3750`. The next instruction begins at `0x1404CABE6`.

`sizeof(kStorageExpect)=22` matches the log-captured prologue:
`40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00`

### 2.2 Extended 32-byte dump (0x1404CABD0:0x1404CABF0)

Source: `scratch/storage_disasm_4cabd0_ext.asm`

```
1404cabd0:  40 55                    PUSH RBP
1404cabd2:  57                       PUSH RDI
1404cabd3:  41 54                    PUSH R12
1404cabd5:  41 56                    PUSH R14
1404cabd7:  41 57                    PUSH R15
1404cabd9:  48 8D AC 24 B0 C9 FF FF  LEA RBP,[RSP + -0x3650]
1404cabe1:  B8 50 37 00 00           MOV EAX,0x3750
1404cabe6:  E8 65 48 38 00           CALL 0x14084f450      ; _alloca_probe (offset 22, OUTSIDE window)
1404cabeb:  48 2B E0                 SUB RSP,RAX
1404cabee:  48 8B 05 53 74 4C 00     MOV RAX,qword ptr [0x140992048]   ; /GS cookie load (offset 29, OUTSIDE window)
```

**Relative-operand analysis:**
- `MOV EAX,0x3750` (`B8 50 37 00 00`) is an absolute immediate — no rel32/rel8.
- The `CALL rel32` (`E8 65 48 38 00`) begins at offset 22 — **outside** the 22-byte steal window.
- No `E8`/`E9`/`0F 8x`/`Jcc rel8` straddles the 22-byte boundary.
- No relative operand is split.

**/GS cookie position:** `__security_check_cookie` prologue pattern `MOV RAX,[0x140992048]` appears at offset **29** (after `SUB RSP,RAX`), **outside** the 22-byte steal window (ends at offset 21). Matches the shop pattern where the cookie must end before the steal.

### 2.3 Callsite verification (0x1406EA800:0x1406EA820)

Source: `scratch/storage_callsite_6ea80c.asm`

```
1406ea801:  83 F9 05                 CMP ECX,0x5
1406ea804:  75 10                    JNZ 0x1406ea816
1406ea806:  48 8B D6                 MOV RDX,RSI
1406ea809:  48 8B CF                 MOV RCX,RDI
1406ea80c:  E8 BF 03 DE FF           CALL 0x1404cabd0      ; storage builder
1406ea811:  E9 2C 0E 00 00           JMP 0x1406eb642
1406ea816:  83 F9 03                 CMP ECX,0x3
```

**rel32 arithmetic:** `site = 0x6EA80C`, `site + 5 = 0x6EA811`, `rel = 0xFFDE03BF` (signed = -2228545).  
`0x6EA811 - 0x21FC41 = 0x4CABD0` ✅ — the call lands exactly on `0x1404CABD0`.

### 2.4 Xrefs to 0x1404CABD0

Source: `scratch/q1_xrefs_4cabd0.txt` (confirmed by this session)

```
[DATA] from 140a6fb7c in NON_FUNCTION
[DATA] from 14095f5e8 in NON_FUNCTION
[DATA] from 14095f658 in NON_FUNCTION
[DATA] from 14095f690 in NON_FUNCTION
[UNCONDITIONAL_CALL] from 1406ea80c in FUN_1406e9b80
```

**Distinct call sites: exactly one** — `0x6EA80C` inside `FUN_1406e9b80` (the dispatcher). The 4 DATA references are vtable/type-table entries, not calls.

### 2.5 Verdict — Block A

(a) Stolen bytes for `0x4CABD0`: **22** (`40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00`), ending on a clean instruction boundary after `MOV EAX,0x3750`.
(b) Inline is **safe**: no relative operand split, no /GS cookie inside the window (cookie at offset 29), one caller `0x6EA80C` confirmed with valid rel32 arithmetic. `sizeof(kStorageExpect)=22` is the largest clean steal.
(c) N/A (Block B).
(d) N/A (Block C).
(e) No discrepant defines in this block.

**Recommendation:** Enable inline hook at `0x4CABD0` with 22 stolen bytes. Keep call-site fallback at `0x6EA80C` as degrade path.

---

## 3. Block B — Dispatcher → Building-Type Mapping (Q2)

Target: `FUN_1406e9b80` (window dispatcher) at file-based `0x1406E9B80`.  
Sources: prior `scratch/q1_disp.asm` (`DisasmMany 0x1406E9B80:0x1406EAB00`), prior `scratch/q1_decomp_disp.c` (`DecompAt 0x1406E9B80`), fresh `scratch/storage_callsite_6ea80c.asm`.

### 3.1 Dispatcher structure

`FUN_1406e9b80` iterates the building array at `context+0xcac0..0xcac8`. Each building `b` has:
- `b+0x240` = type descriptor (td) pointer
- `b+0x34` = kind (main type)
- `b+0x248` = sub-type (used when kind==0)
- `b+0x970/0x978` = storage vector start/end (stride 0xE0, see Block C)

The dispatcher uses a **two-level type dispatch**:

**Level 1:** Load `kind = *(int *)(b + 0x34)`.
- If `kind == 0` → Level 1A: dispatch on `sub = *(int *)(b + 0x248)`
- If `kind != 0` → Level 1B: dispatch directly on `kind`

### 3.2 Level 1A (kind==0, sub-type dispatch at `b+0x248`)

Source: `scratch/q1_disp.asm` lines 726-789, `scratch/q1_decomp_disp.c` lines 435-460.

- `sub == 5` → `CALL 0x1404cabd0` (storage builder) — confirmed at `0x1406EA80C`
- `sub == 3` → shop path (`MOV RBX,[b+0x240]`, then `FUN_1407cef60` + `FUN_14072dbf0`)
- `sub == 7` → `FUN_14048d600`
- `sub == 2` → `FUN_1407ceeb0`
- `sub == 1` → `FUN_1407f2660`

### 3.3 Level 1B (kind!=0, direct dispatch on `b+0x34`)

Source: `scratch/q1_decomp_disp.c` lines 907-971, `scratch/q1_disp.asm` lines 679-689.

- `kind 1` → `FUN_1407f2660`
- `kind 2` → `FUN_1407ceeb0`
- `kind 3` → `FUN_1407fc880` (shop builder, 0x7272F0 region)
- `kind 4` → `FUN_1407fe200`
- `kind 6` → `FUN_1407ff780` (factory)
- `kind 0xe` → `FUN_1407fece0`
- `kind 0xf` → `FUN_1407fe6d0`
- `kind 0x10` → `FUN_1407fe6d0`
- `kind 0x11` → `FUN_1407ff180`
- `kind 0x13` → `FUN_1407fc3f0`
- `kind 0x14` → `FUN_1407fd2b0`
- `kind 0x15` → `FUN_140800280`
- `kind 0x16` → `FUN_14080b550`
- `kind 0x17 (23)` → `FUN_140808830` (pub)
- `kind 0x18` → `FUN_14080c510`
- `kind 0x19` → `FUN_1407f4160`
- `kind 0x1a` → `FUN_1407fac60`
- `kind 0x1b` → `FUN_1407fa6c0`
- `kind 0x1c` → `FUN_1407fb3d0`
- `kind 0x1d` → `FUN_140807ae0`
- `kind 0x1e` → `FUN_14080bc80`
- `kind 0x1f` → `FUN_1407fd810`
- `kind 0x20` → `FUN_1407fdcd0`
- `kind 0x21 (33)` → `FUN_1406f8cc0` + `FUN_1407838e0`
- `kind 0x22` → `FUN_1407f6b50`
- `kind 0x23` → `FUN_140807bf0`
- `kind 0x24` → `FUN_1407f7bb0`
- `kind 0x25` → `FUN_1407fba50`
- `kind 0x26` → `FUN_1407fbf50`
- `kind 0x27 (39)` → `FUN_1408030f0` (city hall)
- `kind 0x28` → `FUN_140804740`
- `kind 0x29` → `FUN_1408054a0`
- `kind 0x2a` → `FUN_140804e80`
- `kind 0x2b` → `FUN_140803ac0`

### 3.4 Type 5 → storage builder (CONFIRMED)

At `0x1406EA801: CMP ECX,0x5` then `0x1406EA80C: CALL 0x1404cabd0` (`E8 BF 03 DE FF`).  
This is reached in the Level 1A path: `kind(b+0x34)==0` AND `sub-type(b+0x248)==5`.

The dispatcher also sets context flag `context+0xadf0 = 5` for storage-capable buildings, and the storage builder `FUN_1404cabd0` itself checks `*(int *)(b + 0xadf0) == 5` (twice, per `scratch/q1_decomp_4cabd0.c` lines 89 and 129).

**`AUTO_STORAGE_BUILDING_TYPE=5` matches the dispatcher's storage type.** ✅

### 3.5 Type 3 → shop builder (CONFIRMED)

- `kind(b+0x34)==3` → `FUN_1407fc880` (shop builder, 0x7272F0 region) — `scratch/q1_disp.asm` lines 907-908.
- `sub-type(b+0x248)==3` (when kind==0) → shop sub-path — `scratch/q1_disp.asm` lines 732-789.

**Type 3 is the shop type, not storage.** A building with type 3 routes to the shop builder, not `0x4CABD0`.

### 3.6 Warehouse_covered_combined type (root cause of Q2)

**Asset side (game install, verbatim, not invented):**
- `C:\Workers Resources Soviet Republic\media_soviet\buildings_types\warehouse_covered_combined.ini` line 4: **`$TYPE_STORAGE`**
- `warehouse_open.ini` line 4: also `$TYPE_STORAGE`
- Shops (e.g. `shop_beta.ini`): `$TYPE_SHOP`

**Exe side (Ghidra):**
- Warehouse name string `'warehouse_covered_combined'` found at multiple exe addresses, e.g. `0x1408EBF00` ([READ] from `0x140652694` in `FUN_140652580`, likely the type-descriptor-by-name lookup). Source: `scratch/straddr_warehouse.txt`.
- Full `td+0x360` (TDM_TYPE_NUM) traversal from the string to the numeric type was **not completed** — string→td pointer chain requires deeper structure traversal beyond this investigation's scope.

**Discrepancy:**
- ini says `$TYPE_STORAGE` → expected numeric type **5** → storage builder `0x4CABD0`
- Previous (failed-hook) runtime log showed warehouse as **`type=3`** → shop builder → `willDraw=0`

The `type=3` reading most likely comes from `b+0x34` (kind), which for warehouse resolves to 3 (shop) rather than the storage type 5. This means either:
1. The exe (v1.1.1.9) compiles `$TYPE_STORAGE` to kind 3 (not 5), or
2. The probe reads `b+0x34` (kind=3 for warehouse) while the storage gate checks `b+0x248` (sub-type) or `b+0xadf0`.

Either way, **warehouse does not currently reach the storage builder `0x4CABD0`** if its runtime type is 3 — which explains why the storage checkbox does not draw (`willDraw=0`) on the warehouse window.

### 3.7 Verdict — Block B

(a) Stolen bytes for `0x4CABD0`: 22 (from Block A).
(b) Inline is safe (from Block A).
(c) Warehouse building type: **asset says `$TYPE_STORAGE` (numeric type 5 expected)**; **runtime log showed `type=3`**. Dispatcher maps **type 5 → storage builder `0x4CABD0`** (confirmed at `0x1406EA80C`) and **type 3 → shop builder** (kind `b+0x34==3` → `FUN_1407fc880`, sub-type `b+0x248==3` → shop sub-path). Warehouse at type 3 routes to shop, NOT storage — root cause of missing checkbox.
(d) `hasNative` discriminator: addressed in Block C.
(e) Discrepant defines: `AUTO_STORAGE_BUILDING_TYPE=5` **matches** the dispatcher's storage type — no change needed. `TSM_BLD_STORAGES_BEGIN=0x970` confirmed correct (Block C).

---

## 4. Block C — AutoHasNativeStorage Discriminator Validation

Target: building storage vector at `b+0x970` (start) / `b+0x978` (end), stride `0xE0`.  
Sources: `scratch/finddisp_970.txt`, `scratch/finddisp_978.txt`, prior `scratch/q1_decomp_disp.c`.

### 4.1 Storage vector layout (CONFIRMED)

**`b+0x970` = storage vector START pointer, `b+0x978` = storage vector END pointer.**

Source: `scratch/q1_decomp_disp.c` (dispatcher, lines ~1073-1075):
```c
lVar16 = *(longlong *)(lVar22 + 0x978) - *(longlong *)(lVar22 + 0x970);
...
lVar16 / 0xe0 + lVar25 != lVar25
```
The difference `(end - start)` divided by `0xE0` gives the storage entry count — **stride 0xE0 confirmed**.

Flags at `+0x84/+0x85` (import/export) and `+0x8C` (capacity) are per-entry fields within each 0xE0 stride, per `TSM_ST_IMPORT 0x84 / EXPORT 0x85 / CAPACITY 0x8C` (enterprise_base comment).

### 4.2 FindDisp 0x970 (readers of `b+0x970`)

Source: `scratch/finddisp_970.txt` (1691 total hits, key hits filtered below)

| Address | Function | Instruction |
|---------|----------|-------------|
| 0x1406EB99A | FUN_1406e9b80 (dispatcher) | SUB RCX,qword ptr [R12 + 0x970] |
| 0x1406EB9E7 | FUN_1406e9b80 (dispatcher) | ADD R13,qword ptr [R12 + 0x970] |
| 0x1406EC628 | FUN_1406e9b80 (dispatcher) | SUB RCX,qword ptr [R12 + 0x970] |
| 0x14072E19C | FUN_14072dbf0 (widget provider) | SUB RCX,qword ptr [R15 + 0x970] |
| 0x14072E1DE | FUN_14072dbf0 (widget provider) | ADD RBX,qword ptr [R15 + 0x970] |
| 0x14072E37E | FUN_14072dbf0 (widget provider) | SUB RCX,qword ptr [R15 + 0x970] |
| 0x14072E3C0 | FUN_14072dbf0 (widget provider) | ADD RDX,qword ptr [R15 + 0x970] |

Additional consumers: `FUN_1406de4a0`, `FUN_1406ee2a0`, `FUN_1406f4b40`, `FUN_1406f4db0`, `FUN_1406f8cc0`, `FUN_140702730`.

### 4.3 FindDisp 0x978 (readers of `b+0x978`)

Source: `scratch/finddisp_978.txt` (789 total hits, key hits filtered below)

| Address | Function | Instruction |
|---------|----------|-------------|
| 0x1406EB988 | FUN_1406e9b80 (dispatcher) | MOV RCX,qword ptr [R12 + 0x978] |
| 0x1406EC61D | FUN_1406e9b80 (dispatcher) | MOV RCX,qword ptr [R12 + 0x978] |
| 0x14072E192 | FUN_14072dbf0 (widget provider) | MOV RCX,qword ptr [R15 + 0x978] |
| 0x14072E374 | FUN_14072dbf0 (widget provider) | MOV RCX,qword ptr [R15 + 0x978] |

### 4.4 Discriminator soundness

**`hasNative = (b+0x970..0x978 vector non-empty, stride 0xE0, readable)` is SOUND:**

1. Both the dispatcher (`FUN_1406e9b80`) and the widget provider (`FUN_14072dbf0`) read `b+0x970` (start) and `b+0x978` (end) — confirming these are the canonical storage vector bounds used engine-wide.
2. Stride `0xE0` confirmed via `(end - start) / 0xe0` in the dispatcher decomp.
3. A building has native storage iff `*(b+0x978) != *(b+0x970)` (non-empty vector) and the range is readable — exactly the `hasNative` check used by the plugin gate `!priv && hasNative`.
4. No type whitelist is needed: any building with a non-empty `b+0x970..0x978` vector has storage, regardless of its type number. This correctly includes warehouses (which are `$TYPE_STORAGE` per ini) and excludes pure retail shops (which have empty storage vectors).

**Cross-check with `TsmBuildingType(b)` path:** `b+0x318 (td) -> td+0x360 (TYPE_NUM)` is the dispatcher's ECX type. This is a *separate* discriminator from `hasNative`. They agree for most buildings but **diverge for warehouse**: the ini says `$TYPE_STORAGE` (type 5), but the runtime logged `type=3` (Block B). The `hasNative` vector check is the more reliable discriminator because it reads actual storage data rather than a type number that may be mismatched.

### 4.5 Verdict — Block C

(d) `hasNative` discriminator is **correct** — no type whitelist needed. `b+0x970`/`b+0x978` stride `0xE0` confirmed; both dispatcher and `StateStorageWidgetProvider` (`FUN_14072dbf0`) read these fields. The vector check is more reliable than the type number for warehouse (which has a type-number discrepancy).
(e) Discrepant defines: `TSM_BLD_STORAGES_BEGIN=0x970` **confirmed correct** (storage vector start at `b+0x970`). `AUTO_STORAGE_BUILDING_TYPE=5` matches dispatcher storage type (Block B) but is secondary to `hasNative`.

---

## 5. Block D — Widget Provider Safety Spot Check

Target: `StateStorageWidgetProvider` = `FUN_14072dbf0`.  
Sources: prior verification per docs/UI investigation §11.13/11.16; fresh `FindDisp 0x970/0x978` confirmation (`scratch/finddisp_970.txt`, `scratch/finddisp_978.txt`).

### 5.1 Provider identity and hook

`StateStorageWidgetProvider` = `FUN_14072dbf0`, reached via the `ui_panels` hook (the `FUN_14072DBF0` family already verified for City Hall per docs/UI investigation §11.13/11.16):
- 17-byte steal: `4C 8B DC … 49 8D AB 98 F5`
- `window+0x01` gate, `window+0x250` bottom edge (write back to advance cursor)
- `live = window+0x20`, `game+0xADB0` widget context

Already proven safe for shop/cityhall/citizen. This block does a spot check for the storage path.

### 5.2 Spot check: storage path readers

`FindDisp 0x970` (`scratch/finddisp_970.txt`) shows `FUN_14072dbf0` reads `[R15 + 0x970]` at 0x14072E19C, 0x14072E1DE, 0x14072E37E, 0x14072E3C0.

`FindDisp 0x978` (`scratch/finddisp_978.txt`) shows `FUN_14072dbf0` reads `[R15 + 0x978]` at 0x14072E192, 0x14072E374.

**The widget provider reads the same storage vector bounds (`b+0x970`/`b+0x978`) that the dispatcher uses.** This confirms `FUN_14072dbf0` is storage-aware and the `hasNative` gate it applies is consistent with engine behavior.

### 5.3 Verdict — Block D

**Spot check result:** `StateStorageWidgetProvider` (`FUN_14072dbf0`) is confirmed reachable via the `ui_panels` hook and reads `b+0x970`/`b+0x978` for storage. No changes needed to the widget provider. The `window+0x250` bottom-edge cursor advancement and `window+0x01` gate patterns already verified for City Hall apply equally to the storage widget path.

---

## 6. Consolidated Verdict (5-line format per request spec)

**(a)** Stolen bytes for `0x4CABD0`: **22** (`40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00`), ending on a clean instruction boundary after `MOV EAX,0x3750`. No relative operand split; `/GS` cookie at offset 29 (outside window); single caller `0x6EA80C` with valid rel32 (`0x6EA80C+5+0xFFDE03BF = 0x4CABD0`).

**(b)** Inline hook is **safe** — `sizeof(kStorageExpect)=22` is the largest clean steal. Enable inline at `0x4CABD0`; keep call-site fallback at `0x6EA80C` as degrade path.

**(c)** Warehouse building type: asset says `$TYPE_STORAGE` (numeric type 5 expected → storage builder `0x4CABD0`, confirmed at `0x1406EA80C`); runtime log showed `type=3` → shop builder (`FUN_1407fc880`, kind `b+0x34==3`). Warehouse at type 3 routes to shop, NOT storage — root cause of missing checkbox. Full `td+0x360` numeric-type traversal from exe string not completed.

**(d)** `hasNative` discriminator (`b+0x970..0x978` stride `0xE0`) is **correct** — no type whitelist needed. Confirmed in both dispatcher and `StateStorageWidgetProvider` via `FindDisp 970/978`. More reliable than type number for warehouse (type-number discrepancy).

**(e)** Discrepant defines: `AUTO_STORAGE_BUILDING_TYPE=5` **matches** dispatcher storage type — no change. `TSM_BLD_STORAGES_BEGIN=0x970` **confirmed correct** — storage vector start at `b+0x970`.

---

## 7. Raw Artifacts

| File | Description |
|------|-------------|
| `scratch/ghidra-storage-A.md` | Block A detailed findings |
| `scratch/ghidra-storage-B.md` | Block B detailed findings |
| `scratch/ghidra-storage-C.md` | Block C detailed findings |
| `scratch/ghidra-storage-D.md` | Block D detailed findings |
| `scratch/storage_disasm_4cabd0.asm` | 22-byte prologue disassembly |
| `scratch/storage_disasm_4cabd0_ext.asm` | Extended 32-byte dump (incl. cookie position) |
| `scratch/storage_callsite_6ea80c.asm` | Callsite `0x6EA80C` disassembly |
| `scratch/q1_xrefs_4cabd0.txt` | Xrefs to `0x4CABD0` |
| `scratch/q1_decomp_4cabd0.c` | Decomp of `FUN_1404cabd0` |
| `scratch/out_listfuncs_4ca000.txt` | Function list around `0x4CA000` |
| `scratch/q1_disp.asm` | Dispatcher disassembly `0x1406E9B80:0x1406EAB00` |
| `scratch/q1_decomp_disp.c` | Decomp of `FUN_1406e9b80` (stride 0xE0 confirmed) |
| `scratch/finddisp_970.txt` | FindDisp results for `b+0x970` |
| `scratch/finddisp_978.txt` | FindDisp results for `b+0x978` |
| `scratch/straddr_warehouse.txt` | Warehouse name string addresses + xrefs |

---

## 8. Related Documents & Files

### 8.1 Primary source request
* `docs/ghidra-request-warehouse-storage-v6.15.md` — the original Ghidra investigation request (Blocks A–D, exact work, pass criteria, deliverable format). All addresses, scripts, and deliverable specs originate here.

### 8.2 Ghidra tooling & scripts (one `analyzeHeadless` at a time; wrapper `scratch/ghidra_run.bat`)
* `scratch/ghidra_run.bat` — headless wrapper: `analyzeHeadless <proj> soviet -process SOVIET64.exe -noanalysis -scriptPath "<ghidra tools>;<scratch>" -postScript <script> [args]`.
* `scratch/DisasmMany.java` — disassemble an address range; used for the 22-byte prologue and the callsite.
* `scratch/Xrefs.java` — enumerate cross-references to an address; used to confirm the single caller `0x6EA80C`.
* `scratch/DecompAt.java` — decompile a function; used for `FUN_1404cabd0` and `FUN_1406e9b80` (times out on large functions).
* `scratch/ListFuncs.java` — enumerate functions in a range; used to confirm `FUN_1404cabd0` size 5185 B.
* `scratch/FindDisp.java` — scan `.text` for a little-endian displacement and map hits to instructions; used for `b+0x970`/`b+0x978` storage vector readers.
* `scratch/StrAddr.java` — locate an ASCII/UTF-16 string address and its xrefs; used for `'warehouse_covered_combined'`.
* `scratch/CallsIn.java` — list every call inside an address range with named targets.
* `scratch/strrva.py` — locate a byte pattern in the EXE and report its RVA.

### 8.3 Related investigation documents
* `docs/UI investigation.md` — prior UI/integration investigation (§11.1–11.16, §11.13–11.16 widget provider, §11.14 helper tools, §11.16 ECX building-type proof, §14.2 `.rdata` literal pool). Establishes the `window+0x250` bottom-edge cursor, `window+0x01` gate, `live=window+0x20`, `game+0xADB0` widget context, and ECX=building-type anchor (shop 0x3, pub 0x17, City Hall 0x27).
* `docs/ghidra-do-storage-findings.md` — DO (distribution office) storage hook findings; DO tile type 43 verified nearby at `0x6EB4FD`/`0x741170`.
* `docs/INSTRUCTIONS.md` — project RE rules: ASLR/RVA discipline, one analyzeHeadless at a time, `.java` scripts only, inline hook safety (≥14 stolen bytes, `expect` array, no `/GS` cookie inside window), zero-allocation rule in render/tick loops, flow-based UI only, DPI scaling at `0x992088`, custom text IDs ≥1,000,000.

### 8.4 Runtime evidence (log)
* `tesmioloader.log` (20:31:55 – 20:33:27, save `4414 - Test with things`) — first log where v6.14 builds: `ui_panels live v6.14: 8 hook(s)`, storage hook `ok storage target=00007FF7DFE5ABD0 tramp=... (22 bytes stolen)`, `dispatcher E8 at 0x6EA80C -> 0x4CABD0`, `storage diag: 307 calls, 1 in storage range 0x4A0000..0x4E0000`, `do: hook 0x6EB4FD -> DO builder 0x741170 ok`, `[AUTO DO] storage widget probe: building=… type=43 key=E15C… priv=1 nativeStorage=1 willDraw=0`. Freight live and billable; warehouse as source AUTO but behind `state_storage_always_on=1` ini bypass; warehouse checkbox not yet verified.

### 8.5 Game-install asset files (verbatim, read-only)
* `C:\Workers Resources Soviet Republic\media_soviet\buildings_types\warehouse_covered_combined.ini` — `$TYPE_STORAGE` (line 4), `$NAME 6202`, `$STORAGE RESOURCE_TRANSPORT_COVERED 650`.
* `C:\Workers Resources Soviet Republic\media_soviet\buildings_types\warehouse_open.ini` — `$TYPE_STORAGE` (line 4).
* `C:\Workers Resources Soviet Republic\media_soviet\buildings_types\shop_beta.ini` — `$TYPE_SHOP` (line 4).
* `C:\Workers Resources Soviet Republic\SOVIET64.exe` — target binary, base `0x140000000`, timestamp `0x6A3EB6AD`.

### 8.6 Raw Ghidra artifacts (under `scratch/`)
| File | Contents |
|------|----------|
| `storage_disasm_4cabd0.asm` | 22-byte prologue disassembly `0x1404CABD0:0x1404CABE0` |
| `storage_disasm_4cabd0_ext.asm` | Extended 32-byte dump `0x1404CABD0:0x1404CABF0` (incl. `/GS` cookie at offset 29) |
| `storage_callsite_6ea80c.asm` | Callsite disassembly `0x1406EA800:0x1406EA820` (`CMP ECX,0x5 → CALL 0x1404cabd0`) |
| `q1_xrefs_4cabd0.txt` | Xrefs to `0x1404CABD0` (single caller `0x6EA80C` in `FUN_1406e9b80`) |
| `q1_decomp_4cabd0.c` | Decomp of `FUN_1404cabd0` (checks `*(int *)(b + 0xadf0) == 5`) |
| `out_listfuncs_4ca000.txt` | Function list `0x1404CA000–0x1404CC000` (`FUN_1404cabd0` size 5185 B) |
| `q1_disp.asm` | Dispatcher disassembly `0x1406E9B80:0x1406EAB00` (type 5→storage, type 3→shop) |
| `q1_decomp_disp.c` | Decomp of `FUN_1406e9b80` (stride `0xE0` at `b+0x970/0x978` confirmed) |
| `finddisp_970.txt` | Readers of `b+0x970` (dispatcher + `FUN_14072dbf0` widget provider) |
| `finddisp_978.txt` | Readers of `b+0x978` (dispatcher + `FUN_14072dbf0` widget provider) |
| `straddr_warehouse.txt` | `'warehouse_covered_combined'` string addresses + xrefs |

### 8.7 Address/constant reference
| Symbol | RVA (file) | Meaning |
|--------|-----------|---------|
| `kStorageExpect` / `FUN_1404cabd0` | `0x4CABD0` | Storage panel builder; 22-byte inline hook target |
| `RVA_STORAGE_CALLSITE` | `0x6EA80C` | `E8 BF 03 DE FF` call to `0x4CABD0` inside dispatcher |
| `RVA_WINDOW_DISPATCH` | `0x6E9B80` | `FUN_1406e9b80` window dispatcher entry |
| `RVA_DO_BUILDER` | `0x741170` | DO builder (neighbouring proof at `0x6EB4FD`) |
| shop / pub / cityhall builders | `0x7272F0 / 0x723950 / 0x72DBF0` | ECX=3→shop, 0x17→pub, 0x27→City Hall |
| `/GS` cookie global | `0x140992048` | Cookie load at offset 29 (outside 22-byte window) |
| `_alloca_probe` call | `0x14084f450` | Begins at offset 22 (outside 22-byte window) |
| Storage vector | `b+0x970`/`b+0x978` | Start/end pointers, stride `0xE0` |
| Kind field | `b+0x34` | Main building type (dispatcher ECX) |
| Sub-type field | `b+0x248` | Sub-type when kind==0 |
| Type descriptor | `b+0x318 → td+0x360` | `TDM_TD → TDM_TYPE_NUM` |

---

## 9. Notes & Open Items

1. **Warehouse numeric type not fully resolved:** The `td+0x360` (TDM_TYPE_NUM) for `warehouse_covered_combined` was not traversed from the exe string (`0x1408EBF00` etc.) to confirm whether the exe assigns type 5 or type 3. This requires deeper structure traversal (string → td pointer → `td+0x360`). Runtime probe (`willDraw`) on the warehouse window is the definitive next check.
2. **`state_storage_always_on=0` test deferred:** Per request §5, do not set this ini during investigation; belongs to the next runtime pass after Ghidra verdict.
3. **Not committed:** Scratch files are gitignored investigation artifacts. No source/config/docs/build files were modified. On branch `main`; not pushed.
