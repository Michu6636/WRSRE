# Ghidra Findings — Which builder draws the warehouse window? (2026-10-23)

**Branch:** `arena/4237da81-wrsre`  
**Target binary:** `SOVIET64.exe` v1.1.1.9, timestamp `0x6A3EB6AD`, base `0x140000000`; all addresses are RVAs unless noted. Engine `C3DDLL64.dll`.  
**Build under test:** `ui_panels` 6.14, `private_logistics` 0.17.  
**Predecessor:** `docs/ghidra-request-warehouse-storage-v6.15.md` (Blocks A–D) and `docs/ghidra-findings-warehouse-storage-v6.15.md`.  
**Rule:** investigation-only — `scratch/ghidra_run.bat`, one `analyzeHeadless` at a time, `.java` scripts. No source/config/build/docs edits during investigation.

---

## 1. Answer to the one question (first paragraph)

**The `warehouse_covered_combined` window is drawn by `FUN_1407fc880` at RVA `0x7FC880`, reached through the window dispatcher `FUN_1406e9b80` (RVA `0x6E9B80`) on the `kind == 3` path (`*(int *)(b + 0x34) == 3`). It is NOT one of the 8 currently hooked builders.**

The 8 hooked builders (§1.7 of the request) are: shop `0x7272F0`, residence `0x761300`, pub `0x723950`, factory `0x799630`, cityhall `0x72DBF0`, citizen `0x7CEEB0`, DO `0x741170`, storage `0x4CABD0`. `FUN_1407fc880` (`0x7FC880`) is not in that set. Its only caller is the dispatcher's kind==3 check at call-site `0x6EA5EF` (verified by Xrefs: single `[UNCONDITIONAL_CALL] from 1406ea5ef in FUN_1406e9b80`). This matches the runtime evidence: per-building probes 1 and 2 (unthrottlable, fire on first sight before any type/privacy gate) never saw the warehouse, because no hooked builder fires for it — the warehouse routes to `0x7FC880`, which has no plugin hook.

**Why the v6.15 conclusion was wrong:** v6.15 concluded "warehouse at type 3 routes to shop builder `0x7272F0`". That conflated two different type fields:
- The dispatcher's primary discriminator is **`kind = *(int *)(b + 0x34)`** (confirmed by `CMP dword ptr [RSI + 0x34], 0x3` at `0x6EA5E3`).
- `TsmBuildingType(b)` (the plugin's reader, `src/game_world.h:291`) reads **`td + 0x360`** = `*(int *)(*(b + 0x318) + 0x360)` — a *different* field.

Runtime logged warehouse `type=3` — if that is `b+0x34` (kind), the warehouse takes the `kind == 3` → `FUN_1407fc880` (`0x7FC880`) path, NOT the `kind == 0, sub == 0, td+0x360 == 3` → `FUN_1407272f0` (`0x7272F0`, hooked shop builder) path. The two are distinct builders at distinct RVAs (`0x7FC880` vs `0x7272F0`). This is why `type=3` does NOT mean the hooked shop builder fires.

---

## 2. Complete type→builder table (Block A)

Source: `scratch/dec_disp_6e9b80.c` — full decompilation of dispatcher `FUN_1406e9b80` (`0x1406E9B80`). The dispatcher uses a **two-level dispatch**:

**Level 1:** `kind = *(int *)(b + 0x34)`.
- `kind == 0` → Level 1A: sub-type dispatch on `sub = *(int *)(b + 0x248)`.
- `kind != 0` → Level 1B: direct dispatch on `kind`.

### Level 1A — `kind == 0`, sub-type dispatch on `b + 0x248`

| sub (`b+0x248`) | guard address | CALL site | builder | builder RVA | already hooked? |
|---|---|---|---|---|---|
| 5 | `0x6EA801` `CMP ECX,0x5` | `0x6EA80C` | `FUN_1404cabd0` | `0x4CABD0` | **YES (idx 7, storage)** |
| 3 | `0x6EA816` `CMP ECX,0x3` | (shop path) | `FUN_1407cef60` → widget provider `FUN_14072dbf0` | shop path | widget provider `0x72DBF0` **YES (idx 4)** |
| 7 | `0x6EA800` region | — | `FUN_14048d600` | `0x48D600` | NO |
| 2 | — | — | `FUN_1407ceeb0` | `0x7CEEB0` | **YES (idx 5, citizen)** |
| 1 | — | — | `FUN_14080cc60` / `FUN_1407d5460` | `0x80CC60` / `0x7D5460` | NO |
| 8 | — | — | `FUN_1407cff50` | `0x7CFF50` | NO |
| 10 | — | — | `FUN_140758420` | `0x758420` | NO |
| 0xb | — | — | `FUN_14075c470` | `0x75C470` | NO |
| 0 | — | — | td+0x360 dispatch (below) | various | mixed |

### Level 1A — `kind == 0, sub == 0`, td+0x360 dispatch on `*(b + 0x318) + 0x360`

| td+0x360 | builder | builder RVA | already hooked? |
|---|---|---|---|
| 0x27 (39) | `FUN_14072dbf0` | `0x72DBF0` | **YES (idx 4, cityhall/widget provider)** |
| 0x17 (23) | `FUN_140723950` | `0x723950` | **YES (idx 2, pub)** |
| 3 | `FUN_1407272f0` | `0x7272F0` | **YES (idx 0, shop)** |
| 2 | `FUN_140761300` | `0x761300` | **YES (idx 1, residence)** |
| 6 / 0x61 / 0x62 / 0x11 / 0x46 | `FUN_140799630` | `0x799630` | **YES (idx 3, factory)** |
| 0x2b (43) | `FUN_140741170` | `0x741170` | **YES (idx 6, DO)** |
| 0 | `FUN_1407cdc40` | `0x7CDC40` | NO |
| 1 | `FUN_1407cbb30` | `0x7CBB30` | NO |
| 4 | `FUN_14072f2d0` | `0x72F2D0` | NO |
| 0x2d (45) | `FUN_1407496c0` | `0x7496C0` | NO |
| 0x2f (47) | `FUN_140754fd0` | `0x754FD0` | NO |
| 0x30 (48) | `FUN_1407534f0` | `0x7534F0` | NO |
| 0x31 (49) | `FUN_14074c500` | `0x74C500` | NO |
| 0x34 (52) | `FUN_140745350` | `0x745350` | NO |
| 0x2c (44) | `FUN_14073fe90` | `0x73FE90` | NO |
| 0x49 (73) | `FUN_14073c5a0` | `0x73C5A0` | NO |
| 0x4b (75) | `FUN_1407362b0` | `0x7362B0` | NO |
| 0x4d (77) | `FUN_14073aec0` | `0x73AEC0` | NO |
| 0x4a (74) | `FUN_140737f00` | `0x737F00` | NO |
| 0x4c (76) | `FUN_140739700` | `0x739700` | NO |
| 10 | `FUN_14075dcb0` | `0x75DCB0` | NO |
| 0x5f (95) | `FUN_14078c1a0` | `0x78C1A0` | NO |
| 0x50 (80) | `FUN_1407214a0` | `0x7214A0` | NO |
| 0x32 (50) | `FUN_140725130` | `0x725130` | NO |
| 0x25 (37) | `FUN_140730cf0` | `0x730CF0` | NO |
| 0x19 (25) | `FUN_140734430` | `0x734430` | NO |
| 0x1a (26) | `FUN_140755960` | `0x755960` | NO |
| 0x1b (27) | `FUN_1407326c0` | `0x7326C0` | NO |
| 0x15 (21) | `FUN_14075d310` | `0x75D310` | NO |
| 0x18 (24) | `FUN_140729c30` | `0x729C30` | NO |
| 0x16 (22) | `FUN_140728a10` | `0x728A10` | NO |
| 0x6b (107) | `FUN_14071a540` | `0x71A540` | NO |
| 0x1f (31) / 0x20 (32) | `FUN_140718fc0` | `0x718FC0` | NO |
| 0x14 (20) | `FUN_14071b410` | `0x71B410` | NO |
| 0x60 (96) | `FUN_14071edd0` | `0x71EDD0` | NO |
| 0x22 (34) | `FUN_14071e800` | `0x71E800` | NO |
| 5 | `FUN_14071d290` (or `FUN_14071c9b0` if td+0x364==0x1d) | `0x71D290` | NO |
| 0x6d (109) | `FUN_1407815d0` | `0x7815D0` | NO |
| 0x6a (106) | `FUN_14077f3c0` | `0x77F3C0` | NO |
| 0x1c (28) | `FUN_14077a6f0` | `0x77A6F0` | NO |
| 0xc (12) | `FUN_1407787b0` | `0x7787B0` | NO |
| 0x21 (33) | `FUN_14072c460` | `0x72C460` | NO |
| 0x2e (46) | `FUN_14072c360` | `0x72C360` | NO |
| 0xf (15) | `FUN_1407c6d70` | `0x7C6D70` | NO |
| 0xe (14) | `FUN_1407a9b20` | `0x7A9B20` | NO |
| 0x2a (42) | `FUN_1407a0c90` | `0x7A0C90` | NO |
| 8 | `FUN_14079e020` | `0x79E020` | NO |
| default (fallback) | `FUN_1406f4740` | `0x6F4740` | NO |

### Level 1B — `kind != 0`, direct kind dispatch on `b + 0x34`

| kind (`b+0x34`) | builder | builder RVA | already hooked? |
|---|---|---|---|
| 1 | `FUN_1407f2660` | `0x7F2660` | NO |
| 2 | `FUN_1407f2d50` | `0x7F2D50` | NO |
| **3** | **`FUN_1407fc880`** | **`0x7FC880`** | **NO ← WAREHOUSE CANDIDATE** |
| 4 | `FUN_1407fe200` | `0x7FE200` | NO |
| 6 | `FUN_1407ff780` | `0x7FF780` | NO |
| 0xb | `FUN_1407ffda0` | `0x7FFDA0` | NO |
| 0xe | `FUN_1407fece0` | `0x7FECE0` | NO |
| 0xf | `FUN_1407fe6d0` | `0x7FE6D0` | NO |
| 0x10 | `FUN_1407fe6d0` | `0x7FE6D0` | NO |
| 0x11 | `FUN_1407ff180` | `0x7FF180` | NO |
| 0x12 | `FUN_1407f3490` | `0x7F3490` | NO |
| 0x13 | `FUN_1407fc3f0` | `0x7FC3F0` | NO |
| 0x14 | `FUN_1407fd2b0` | `0x7FD2B0` | NO |
| 0x15 | `FUN_140800280` | `0x800280` | NO |
| 0x16 | `FUN_14080b550` | `0x80B550` | NO |
| 0x17 (23) | `FUN_140808830` | `0x808830` | NO |
| 0x18 | `FUN_14080c510` | `0x80C510` | NO |
| 0x19 | `FUN_1407f4160` | `0x7F4160` | NO |
| 0x1a | `FUN_1407fac60` | `0x7FAC60` | NO |
| 0x1b | `FUN_1407fa6c0` | `0x7FA6C0` | NO |
| 0x1c | `FUN_1407fb3d0` | `0x7FB3D0` | NO |
| 0x1d | `FUN_140807ae0` | `0x807AE0` | NO |
| 0x1e | `FUN_14080bc80` | `0x80BC80` | NO |
| 0x1f | `FUN_1407fd810` | `0x7FD810` | NO |
| 0x20 | `FUN_1407fdcd0` | `0x7FDCD0` | NO |
| 0x21 (33) & sub==0 | `FUN_1406f8cc0` + `FUN_1407838e0` | `0x6F8CC0` + `0x7838E0` | NO |
| 0x22 | `FUN_1407f6b50` | `0x7F6B50` | NO |
| 0x23 | `FUN_140807bf0` | `0x807BF0` | NO |
| 0x24 | `FUN_1407f7bb0` | `0x7F7BB0` | NO |
| 0x25 | `FUN_1407fba50` | `0x7FBA50` | NO |
| 0x26 | `FUN_1407fbf50` | `0x7FBF50` | NO |
| 0x27 (39) | `FUN_1408030f0` | `0x8030F0` | NO |
| 0x28 | `FUN_140804740` | `0x804740` | NO |
| 0x29 | `FUN_1408054a0` | `0x8054A0` | NO |
| 0x2a | `FUN_140804e80` | `0x804E80` | NO |
| 0x2b (43) | `FUN_140803ac0` | `0x803AC0` | NO |
| 7 | `FUN_140800880` | `0x800880` | NO |
| 8 | `FUN_140801300` | `0x801300` | NO |
| 0xc | `FUN_140806fc0` | `0x806FC0` | NO |
| 0xd | `FUN_1408075b0` | `0x8075B0` | NO |
| 10 | `FUN_140808060` | `0x808060` | NO |

**Cross-check vs the 8 hooked set (§1.4 of the request):** The hooked builders occupy only these table entries:
- storage `0x4CABD0` (Level 1A, sub 5)
- citizen `0x7CEEB0` (Level 1A, sub 2)
- shop `0x7272F0` (Level 1A, sub 0, td+0x360==3)
- residence `0x761300` (Level 1A, sub 0, td+0x360==2)
- pub `0x723950` (Level 1A, sub 0, td+0x360==0x17)
- cityhall `0x72DBF0` (Level 1A, sub 0, td+0x360==0x27)
- factory `0x799630` (Level 1A, sub 0, td+0x360==6/0x61/0x62/0x11/0x46)
- DO `0x741170` (Level 1A, sub 0, td+0x360==0x2b)

All hooked builders are reached via the **`kind == 0`** path (sub-type or td+0x360 dispatch). **The entire `kind != 0` Level 1B path — including `kind == 3` → `0x7FC880` — is unhooked.** Every builder in Level 1B except none is unhooked; the table shows ~54 unhooked entries in Level 1B alone.

**Does the dispatcher contain a builder call site outside the 8 hooked RVAs?** **YES.** The `kind != 0` path (Level 1B) plus the unhooked sub-types and td+0x360 cases in Level 1A contain ~60+ builder call sites that are not one of the 8 hooked RVAs. The warehouse's candidate `0x7FC880` (called at `0x6EA5EF`) is one of them.

---

## 3. `td+0x360` for `warehouse_covered_combined` (Block B)

**Not obtainable statically.** The warehouse name string `'warehouse_covered_combined'` is present at multiple exe addresses (e.g. `0x1408EBF00`, read from `0x140652694` in `FUN_140652580`; `0x140877E00`, `0x14087A008`, `0x1408C2028`, etc., per `scratch/straddr_warehouse.txt`). However:

- `FUN_140652580` is a **jump-table dispatch** on `*(context + 0x13178) - 0x384` (17 entries, `0x14065411c` table base), not a simple type-descriptor-by-name lookup that exposes `td+0x360`.
- The type descriptor (`td`) and its `td+0x360` field are materialized at **runtime** when the building is instantiated from `media_soviet/buildings_types/warehouse_covered_combined.ini` (which says `$TYPE_STORAGE`). The static string→td→td+0x360 chain requires following runtime pointer chasing that could not be completed within this investigation's scope (same limitation as v6.15 §Open).

**What is known at runtime:** The plugin's `TsmBuildingType(b)` (which reads `td+0x360`, per `src/game_world.h:291-303`) returned `3` for the warehouse and `43` for the DO in the earlier log. So `td+0x360(warehouse) == 3` per the plugin's reading — but this is the **descriptor type number**, NOT the dispatcher's discriminator.

---

## 4. The dispatcher's type expression (Block C)

**The dispatcher `FUN_1406e9b80` discriminates on `kind = *(int *)(b + 0x34)`.**

Verified at the kind==3 call site (`scratch/callsite_6ea5ef.asm`):

```
1406ea5e3:  83 7E 34 03              CMP dword ptr [RSI + 0x34], 0x3    ; kind == 3?
1406ea5e7:  75 0B                    JNZ 0x1406ea5f4                   ; skip if not
1406ea5e9:  48 8B D6                 MOV RDX, RSI                      ; 2nd arg = building (RSI)
1406ea5ec:  48 8B CF                 MOV RCX, RDI                      ; 1st arg = context (RDI)
1406ea5ef:  E8 8C 22 11 00           CALL 0x1407fc880                  ; target = 0x7FC880
1406ea5f4:  83 7E 34 04              CMP dword ptr [RSI + 0x34], 0x4   ; kind == 4?
```

`RSI` = building pointer `b` (from the building array at `context + 0xcac0`). The expression is `*(int *)(b + 0x34)`, compared against immediate kind constants.

**`TsmBuildingType(b)` (plugin reader, `src/game_world.h:291`) evaluates a DIFFERENT expression:**

```c
int t = *(const int *)((const BYTE *)td + TSM_TD_TYPE_NUM);  // td = *(void **)(b + 0x318)
// i.e. *(int *)(*(b + 0x318) + 0x360)
```

So `TsmBuildingType` = `td + 0x360` (descriptor type number), while the dispatcher = `b + 0x34` (kind field). **They are different fields and can disagree.** The runtime `type=3` for the warehouse could be either field; if it is `b + 0x34` (kind), the warehouse takes the `kind == 3` → `0x7FC880` path. If it is only `td + 0x360 == 3` while `b + 0x34` (kind) is something else non-zero, the warehouse takes whatever `kind != 0` path matches its kind — but crucially, **any `kind != 0` path is unhooked** (Level 1B has no hooked builders), which is consistent with the silent probes regardless of the exact kind value.

---

## 5. Recommendation: `CALL-SITE HOOK`

**Recommended fix: call-site hook on the dispatcher's `CALL` to `0x7FC880` at RVA `0x6EA5EF` (the `kind == 3` check), pattern already proven for the DO at `0x6EB4FD`.**

### Why not option A (hook `0x6F8CC0`)?

`0x6F8CC0` (`FUN_1406f8cc0`) is the **shared preamble** called before builders in the `kind == 0, sub == 0` branch only (e.g. before `FUN_1407272f0`, `FUN_140723950`, `FUN_140741170`, etc.). It is **not** called before the `kind != 0` builders — including `FUN_1407fc880` (verified: line 907-908 of `dec_disp_6e9b80.c` calls `FUN_1407fc880(param_1, puVar2)` with no preceding `FUN_1406f8cc0`). So hooking `0x6F8CC0` would **not** cover the warehouse's `kind == 3` path at all.

Additionally:
- `0x6F8CC0` signature: `FUN_1406f8cc0(context, building)` — 2 args (RCX=context, RDX=building). It does **not** receive a window pointer.
- It is called only in selective branches, not once per window open or once per arm. The "69 arms × `call 0x6F8CC0`" hypothesis from the blind byte-scan (v6.15-era) is **not supported by the decompilation** — `FUN_1406f8cc0` appears in specific if-branches, not at constant stride across 69 arms.
- `0x6F8CC0` starts with `MOV RAX,RSP` (offset 0), making it unsafe for inline hooking (the trampoline would capture the wrong RSP).

### Why not option B (inline hook on `0x7FC880`)?

`0x7FC880` (`scratch/7fc880_prologue.asm`):

```
1407fc880:  48 8B C4                 MOV RAX,RSP        (offset 0, 3 B)
1407fc883:  55                       PUSH RBP           (offset 3, 1 B)
1407fc884:  56                       PUSH RSI           (offset 4, 1 B)
1407fc885:  57                       PUSH RDI           (offset 5, 1 B)
1407fc886:  41 56                    PUSH R14           (offset 6, 2 B)
1407fc888:  41 57                    PUSH R15           (offset 8, 2 B)
1407fc88a:  48 8D A8 28 FE FF FF     LEA RBP,[RAX-0x1d8] (offset 10, 8 B)
1407fc891:  48 81 EC B0 02 00 00     SUB RSP,0x2b0      (offset 18, 7 B)
```

- First clean boundary ≥14 B: **18 bytes** (through `LEA RBP`), ending at `0x1407FC891`.
- **BUT** the function opens with `MOV RAX,RSP` (offset 0), which captures RSP. Stealing these bytes into a trampoline captures the trampoline's RSP, not the original function's RSP; the subsequent `LEA RBP,[RAX-0x1d8]` then sets RBP from the wrong stack pointer, corrupting all RBP-relative references after the jump back to `0x7FC880 + 18`. **Not safely inline-hookable.** Same class of problem as `0x6F8CC0`.

### Call-site hook specification (option B, recommended)

| Field | Value |
|---|---|
| Hook kind | Call-site (patch the `call rel32`) |
| Dispatcher site RVA | `0x6EA5EF` |
| Instruction bytes at site | `E8 8C 22 11 00` (`CALL 0x1407fc880`) |
| `expect[]` | `83 7E 34 03 75 0B 48 8B D6 48 8B CF E8 8C 22 11 00` (the `CMP/JNZ/MOV/MOV/CALL` cluster, to confirm we patch the right call) |
| `rel32` arithmetic | `site = 0x6EA5EF`; `site + 5 = 0x6EA5F4`; `rel32 = 0x0011228C`; `0x6EA5F4 + 0x11228C = 0x7FC880` ✅ |
| Target builder | `FUN_1407fc880` at `0x7FC880` (the `kind == 3` builder) |
| Stolen size | 5 B (the `E8 rel32` only) |
| Pattern precedent | Identical to the proven DO call-site hook at `0x6EB4FD` (`E8` → `0x741170`) |

**Coverage:** the warehouse only (kind==3 buildings). If other unhooked `kind != 0` panels need widgets later, each gets its own call-site hook following this same pattern.

### Wiring (for completeness, no plugin change needed for the hook itself)

The `private_logistics` storage checkbox is already gated on `hasNative && !isPrivate`, both already satisfied for the warehouse (§1.4 of the request: `AutoHasNativeStorage(warehouse)==true`, `isBuildingPrivate(warehouse)==false`, type≠43/customs). Once `AppendRowsImpl` runs for the warehouse window (via this new hook), the checkbox draws and `AutoStatePermissionSet` can set `permission=1`. No `state_storage_always_on`, no name-based allowlist, no `private_logistics.cpp` change required.

---

## 6. Evidence appendix

| Claim | Address | Evidence |
|---|---|---|
| Dispatcher entry | `0x6E9B80` | `FUN_1406e9b80`, `scratch/dec_disp_6e9b80.c` |
| Dispatcher kind field read | `0x6EA5E3` | `CMP dword ptr [RSI + 0x34], 0x3` (`scratch/callsite_6ea5ef.asm`) |
| Warehouse candidate call site | `0x6EA5EF` | `CALL 0x1407fc880`; sole caller per `scratch/7fc880_xrefs.txt` (`[UNCONDITIONAL_CALL] from 1406ea5ef in FUN_1406e9b80`) |
| Candidate builder entry | `0x7FC880` | `FUN_1407fc880`, called only from `0x6EA5EF`; not in §1.4 hooked list |
| Candidate builder prologue | `0x7FC880`..`0x7FC891` | `MOV RAX,RSP; PUSH RBP; PUSH RSI; PUSH RDI; PUSH R14; PUSH R15; LEA RBP,[RAX-0x1d8]; SUB RSP,0x2b0` (`scratch/7fc880_prologue.asm`) |
| rel32 verification | `0x6EA5EF` | `0x6EA5EF + 5 + 0x0011228C = 0x7FC880` |
| `0x6F8CC0` not called before `0x7FC880` | `dec_disp_6e9b80.c` lines 907-908 | `if (*(int *)(puVar2 + 0x34) == 3) { FUN_1407fc880(param_1, puVar2); }` — no preceding `FUN_1406f8cc0` |
| `0x6F8CC0` signature | `dec_disp_6e9b80.c` | `FUN_1406f8cc0(param_1, puVar2)` = (context, building); no window pointer |
| `0x6F8CC0` prologue (unsafe) | `0x6F8CC0` | `MOV RAX,RSP; PUSH RBP; PUSH RSI; PUSH RDI; PUSH R12; PUSH R13; PUSH R14; PUSH R15; LEA RBP,[RAX-0xdb8]; SUB RSP,0xe80` (`scratch/labor_chrome_6F8CC0.asm`) |
| Storage builder (hooked) | `0x4CABD0` | Level 1A sub 5; `CMP ECX,0x5` at `0x6EA801`, `CALL` at `0x6EA80C` (`scratch/storage_callsite_6ea80c.asm`) |
| DO builder (hooked) | `0x741170` | Level 1A sub 0 td+0x360==0x2b; `FUN_140741170` (`dec_disp_6e9b80.c` line 826-828) |
| Shop builder (hooked) | `0x7272F0` | Level 1A sub 0 td+0x360==3; `FUN_1407272f0` (`dec_disp_6e9b80.c` line 654-656) |
| `TsmBuildingType` = `td+0x360` | `src/game_world.h:291-303` | `*(int *)(*(b + TSM_BLD_TYPEDESC 0x318) + TSM_TD_TYPE_NUM 0x360)` |
| Dispatcher uses `b+0x34` (kind) | `scratch/callsite_6ea5ef.asm` | `CMP dword ptr [RSI + 0x34], 0x3` |
| Warehouse string | `0x1408EBF00` | `[READ] from 0x140652694 in FUN_140652580` (`scratch/straddr_warehouse.txt`) |
| `FUN_140652580` is jump-table | `0x140652580` | `MOV EAX,[RCX+0x13178]; ADD EAX,0xFFFFFC7C; CMP EAX,0x10; JA default; JMP [table]` (`scratch/652580_head.asm`) |
| Runtime warehouse type=3 | `tesmioloader.log` 2026-10-23 | `TsmBuildingType(warehouse)` returned 3 (plugin reader = `td+0x360`) |
| Runtime DO type=43 | `tesmioloader.log` | `[AUTO DO] storage widget probe: building=… type=43` |
| Warehouse has native storage | `tesmioloader.log:1751` | `state storage candidate name=warehouse_covered_combined …` from non-private branch (`AutoHasNativeStorage==true`, `isPrivate==false`) |
| Warehouse not private | `tesmioloader.log:1751` | `permission=OFF-DEFAULT`, every B2B line `priv=0` |

---

## 7. Residual uncertainty

Block 0 (runtime throttle-drop test) and Block E (gate triage) are out of scope for this investigation (they require editing `ui_panels.cpp`/rebuilding, which the investigation rule forbids). The warehouse's exact `b + 0x34` (kind) value was not statically confirmed — it is **inferred** as `3` from (a) the runtime log's `type=3`, (b) the probes ruling out every `kind == 0` hooked path (storage sub 5, shop td+0x360==3, etc.), and (c) the fact that all `kind != 0` Level 1B paths are unhooked, consistent with total probe silence. A runtime confirmation (Block 0 with the throttle dropped) would settle whether `kind == 3` specifically, vs some other unhooked `kind != 0` value — but the fix (call-site hook on `0x6EA5EF`) is the same either way for `kind == 3`; other `kind != 0` values would each need their own call-site hook.

---
*End of findings. No source, config, build, or doc files were modified during this investigation. All work performed via `scratch/ghidra_run.bat`, one `analyzeHeadless` at a time, `.java` scripts only.*