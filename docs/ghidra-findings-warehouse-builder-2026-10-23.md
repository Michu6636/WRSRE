# Ghidra Findings — Which builder draws the warehouse window? (2026-10-23)

**Branch:** `arena/4237da81-wrsre`
**Target binary:** `SOVIET64.exe` v1.1.1.9, timestamp `0x6A3EB6AD`, base `0x140000000`; all addresses are RVAs unless noted. Engine `C3DDLL64.dll`.
**Build under test:** `ui_panels` 6.14, `private_logistics` 0.17.
**Predecessor:** `docs/ghidra-request-warehouse-storage-v6.15.md` (Blocks A–D), `docs/ghidra-findings-warehouse-storage-v6.15.md`, and `docs/ghidra-request-warehouse-builder-2026-10-23.md` (the request this answers).
**Rule:** investigation-only — `scratch/ghidra_run.bat`, one `analyzeHeadless` at a time, `.java` scripts. No source/config/build/docs edits during the investigation.

> Transcribed from the investigator's report. §8 is a code-agent addendum added afterwards; everything above it is the investigation's own text.

---

## 1. Answer

**The `warehouse_covered_combined` window is drawn by `FUN_1407fc880` at RVA `0x7FC880`, reached through the window dispatcher `FUN_1406e9b80` (RVA `0x6E9B80`) on the `kind == 3` path (`*(int *)(b + 0x34) == 3`). It is NOT one of the 8 currently hooked builders.**

The 8 hooked builders are: shop `0x7272F0`, residence `0x761300`, pub `0x723950`, factory `0x799630`, cityhall `0x72DBF0`, citizen `0x7CEEB0`, DO `0x741170`, storage `0x4CABD0`. `0x7FC880` is not in that set. Its only caller is the dispatcher's kind==3 check at call-site `0x6EA5EF` (verified by Xrefs: single `[UNCONDITIONAL_CALL] from 1406ea5ef in FUN_1406e9b80`).

**Why the v6.15 conclusion was wrong.** v6.15 concluded "warehouse at type 3 routes to shop builder `0x7272F0`". That conflated two different type fields:

- the dispatcher's primary discriminator is `kind = *(int *)(b + 0x34)` (confirmed by `CMP dword ptr [RSI + 0x34], 0x3` at `0x6EA5E3`);
- `TsmBuildingType(b)` (the plugin's reader, `src/game_world.h:291`) reads `td + 0x360` = `*(int *)(*(b + 0x318) + 0x360)` — a *different* field.

Runtime logged warehouse `type=3`; if that is `b+0x34` (kind), the warehouse takes `kind == 3` → `0x7FC880`, not `kind==0, sub==0, td+0x360==3` → `0x7272F0`. The two are distinct builders at distinct RVAs.

---

## 2. Type→builder table (Block A)

Source: `scratch/dec_disp_6e9b80.c`. The dispatcher is a **two-level** switch.

**Level 1:** `kind = *(int *)(b + 0x34)`.
- `kind == 0` → Level 1A: sub-type dispatch on `sub = *(int *)(b + 0x248)`.
- `kind != 0` → Level 1B: direct dispatch on `kind`.

### Level 1A — `kind == 0`

| sub (`b+0x248`) | CALL site | builder RVA | hooked? |
|---|---|---|---|
| 5 | `0x6EA80C` | `0x4CABD0` | **YES (idx 7 storage)** |
| 3 | — | `0x7CEF60` (+ `0x72DBF0`) | partial (cityhall idx 4) |
| 7 | — | `0x48D600` | NO |
| 2 | — | `0x7CEEB0` | **YES (idx 5 citizen)** |
| 1 | — | `0x80CC60` / `0x7D5460` | NO |
| 8 | — | `0x7CFF50` | NO |
| 10 | — | `0x758420` | NO |
| 0xb | — | `0x75C470` | NO |
| 0 | — | td+0x360 dispatch (below) | mixed |

`sub == 0` → third level on `*(int *)(*(b + 0x318) + 0x360)`:

| td+0x360 | builder RVA | hooked? |
|---|---|---|
| 0x27 (39) | `0x72DBF0` | **YES (cityhall)** |
| 0x17 (23) | `0x723950` | **YES (pub)** |
| 3 | `0x7272F0` | **YES (shop)** |
| 2 | `0x761300` | **YES (residence)** |
| 6 / 0x61 / 0x62 / 0x11 / 0x46 | `0x799630` | **YES (factory)** |
| 0x2b (43) | `0x741170` | **YES (DO)** |
| 0 / 1 / 4 / 8 / 10 / 0xc / 0xe / 0xf / 0x14 … | ~50 others (see request doc) | NO |

**Every hooked builder is on the `kind == 0` path.**

### Level 1B — `kind != 0` (entirely unhooked)

| kind | builder RVA | | kind | builder RVA |
|---|---|---|---|---|
| 1 | `0x7F2660` | | 0x1b | `0x7FA6C0` |
| 2 | `0x7F2D50` | | 0x1c | `0x7FB3D0` |
| **3** | **`0x7FC880` ← warehouse** | | 0x1d | `0x807AE0` |
| 4 | `0x7FE200` | | 0x1e | `0x80BC80` |
| 6 | `0x7FF780` | | 0x1f | `0x7FD810` |
| 7 | `0x800880` | | 0x20 | `0x7FDCD0` |
| 8 | `0x801300` | | 0x21 | `0x6F8CC0` + `0x7838E0` |
| 0xb | `0x7FFDA0` | | 0x22 | `0x7F6B50` |
| 0xc | `0x806FC0` | | 0x23 | `0x807BF0` |
| 0xd | `0x8075B0` | | 0x24 | `0x7F7BB0` |
| 0xe | `0x7FECE0` | | 0x25 | `0x7FBA50` |
| 0xf / 0x10 | `0x7FE6D0` | | 0x26 | `0x7FBF50` |
| 0x11 | `0x7FF180` | | 0x27 | `0x8030F0` |
| 0x12 | `0x7F3490` | | 0x28 | `0x804740` |
| 0x13 | `0x7FC3F0` | | 0x29 | `0x8054A0` |
| 0x14 | `0x7FD2B0` | | 0x2a | `0x804E80` |
| 0x15 | `0x800280` | | 0x2b | `0x803AC0` |
| 0x16 | `0x80B550` | | 0x2c… | (table continues in `dec_disp_6e9b80.c`) |
| 0x17 | `0x808830` | | | |
| 0x18 | `0x80C510` | | | |
| 0x19 | `0x7F4160` | | | |
| 0x1a | `0x7FAC60` | | | |

**Does the dispatcher contain a builder call site outside the 8 hooked RVAs?** YES — the whole Level 1B path plus the unhooked Level 1A sub-types and td+0x360 cases, 60+ sites.

---

## 3. `td+0x360` for `warehouse_covered_combined` (Block B)

**Not obtainable statically.** The string `'warehouse_covered_combined'` appears at several exe addresses (`0x1408EBF00` read from `0x140652694` in `FUN_140652580`; also `0x140877E00`, `0x14087A008`, `0x1408C2028`). `FUN_140652580` is a jump-table dispatch on `*(context + 0x13178) - 0x384` (17 entries, table at `0x14065411c`), not a name→descriptor lookup. The descriptor materializes at runtime from `media_soviet/buildings_types/warehouse_covered_combined.ini` (`$TYPE_STORAGE`). Same limitation as v6.15 §Open.

**Known at runtime:** the plugin's `TsmBuildingType(b)` returned `3` for the warehouse and `43` for the DO. That is `td+0x360`, **not** the dispatcher's discriminator.

---

## 4. The dispatcher's type expression (Block C)

`FUN_1406e9b80` discriminates on `kind = *(int *)(b + 0x34)`. Verified at the kind==3 site (`scratch/callsite_6ea5ef.asm`):

```
1406ea5e3:  83 7E 34 03              CMP dword ptr [RSI + 0x34], 0x3
1406ea5e7:  75 0B                    JNZ 0x1406ea5f4
1406ea5e9:  48 8B D6                 MOV RDX, RSI
1406ea5ec:  48 8B CF                 MOV RCX, RDI
1406ea5ef:  E8 8C 22 11 00           CALL 0x1407fc880
1406ea5f4:  83 7E 34 04              CMP dword ptr [RSI + 0x34], 0x4
```

`TsmBuildingType(b)` evaluates `*(int *)(*(b + 0x318) + 0x360)`. **Different fields, and they can disagree.**

---

## 5. Recommendation: call-site hook

**Call-site hook on the dispatcher's `CALL` to `0x7FC880` at RVA `0x6EA5EF`**, same pattern as the proven DO hook at `0x6EB4FD`.

**Why not hook `0x6F8CC0`.** `FUN_1406f8cc0(context, building)` is 2-arg, receives no window, is called only in selective `kind==0, sub==0` branches, and is **not** called before `FUN_1407fc880`. The "69 arms × `call 0x6F8CC0`" hypothesis from the blind byte scan is not supported by the decompilation.

**Why not an inline hook on `0x7FC880`.** It opens `MOV RAX,RSP` and reaches its first clean boundary 18 bytes in:

```
1407fc880:  48 8B C4                 MOV RAX,RSP
1407fc883:  55                       PUSH RBP
1407fc884:  56                       PUSH RSI
1407fc885:  57                       PUSH RDI
1407fc886:  41 56                    PUSH R14
1407fc888:  41 57                    PUSH R15
1407fc88a:  48 8D A8 28 FE FF FF     LEA RBP,[RAX-0x1d8]
1407fc891:  48 81 EC B0 02 00 00     SUB RSP,0x2b0
```

| Field | Value |
|---|---|
| Hook kind | call-site (`call rel32` patch) |
| Site RVA | `0x6EA5EF` |
| Bytes at site | `E8 8C 22 11 00` |
| `expect[]` | `83 7E 34 03 75 0B 48 8B D6 48 8B CF E8 8C 22 11 00` |
| rel32 arithmetic | `0x6EA5EF + 5 + 0x0011228C = 0x7FC880` ✅ |
| Stolen | 5 B (the `E8 rel32` only) |
| Precedent | DO call-site hook at `0x6EB4FD` |

**Coverage:** warehouse only (kind==3). Other `kind != 0` panels would each need their own call-site hook.

---

## 6. Evidence appendix

| Claim | Address | Evidence |
|---|---|---|
| Dispatcher entry | `0x6E9B80` | `FUN_1406e9b80`, `scratch/dec_disp_6e9b80.c` |
| kind read | `0x6EA5E3` | `CMP dword ptr [RSI + 0x34], 0x3` |
| warehouse call site | `0x6EA5EF` | `CALL 0x1407fc880`; sole caller per `scratch/7fc880_xrefs.txt` |
| builder entry | `0x7FC880` | `FUN_1407fc880`; not in the hooked list |
| builder prologue | `0x7FC880`..`0x7FC891` | `scratch/7fc880_prologue.asm` |
| rel32 check | `0x6EA5EF` | `0x6EA5EF + 5 + 0x0011228C` = `0x7FC880` |
| `0x6F8CC0` not called before `0x7FC880` | `dec_disp_6e9b80.c` 907-908 | `if (*(int *)(puVar2 + 0x34) == 3) { FUN_1407fc880(param_1, puVar2); }` |
| `0x6F8CC0` signature | `dec_disp_6e9b80.c` | `FUN_1406f8cc0(param_1, puVar2)` |
| storage builder (hooked) | `0x4CABD0` | Level 1A sub 5; `CMP ECX,0x5` at `0x6EA801` |
| shop builder (hooked) | `0x7272F0` | Level 1A sub 0, td+0x360==3 |
| DO builder (hooked) | `0x741170` | Level 1A sub 0, td+0x360==0x2b |
| `TsmBuildingType` | `src/game_world.h:291-303` | `*(int *)(*(b + 0x318) + 0x360)` |
| warehouse string | `0x1408EBF00` | read from `0x140652694` in `FUN_140652580` |
| warehouse has native storage | `tesmioloader.log:1751` | `state storage candidate …` from the non-private branch |
| warehouse not private | `tesmioloader.log:1751` | `permission=OFF-DEFAULT`; B2B lines `priv=0` |

---

## 7. Residual uncertainty (investigator's own)

Block 0 and Block E were out of scope (they need a source edit + rebuild). The warehouse's exact `b + 0x34` (kind) value was **not statically confirmed** — it is inferred from the runtime log's `type=3` plus the probes ruling out every hooked `kind == 0` path. The fix (call-site hook on `0x6EA5EF`) is the same for `kind == 3`; other `kind != 0` values would each need their own hook.

---

## 8. Addendum — code-agent review (2026-10-23)

Two points, one of which changes how the finding should be read. Neither invalidates the call-site recommendation; both are recorded because the reasoning matters more than the answer.

### 8.1 `+0x34` is a **window** field, not a building field

`ui_panels.cpp:558` already defines:

```c
#define W_KIND      0x34   // window kind (int): 0x21 construction, 0x27/0x28
```

and `EscCloseProbe` reads exactly that at `:1721`. So `CMP dword ptr [RSI + 0x34], 0x3` is comparing the **window's** kind to 3, and `RSI` is the window — which is also why `MOV RDX, RSI` hands the builder the same `(context, window)` pair as every other builder, and why the call-site cave can reuse `RunHookBody` unchanged. Read as a building field, `b + 0x34` would be an offset no plugin code has ever touched, and the builder would receive a building where the append path requires a window.

This does not change the recommended address. It does change the description: the two-level switch is **window kind → window sub → building `td+0x360`**, i.e. the dispatcher routes by *panel class* first and only consults the building's own type on one of the arms.

### 8.2 `kind == 3` is still an inference, and §7's own reasoning is circular

§7 concedes the kind was not statically confirmed, but the inference still leans on the runtime `type=3` — and `type` is `td+0x360`, which §4 of this same document proves is a *different field*. The DO demonstrates the divergence concretely: `td+0x360 == 43` while its window takes the `kind == 0` arm. So `td+0x360 == 3` cannot establish `window+0x34 == 3`.

What *does* survive the objection: the warehouse is definitely **not** on `kind==0 && sub==0`, because that arm with `td+0x360==3` would be the shop builder `0x7272F0`, which is hooked — and it never fired (four independent per-building probes, all silent). So the warehouse is either `kind != 0` or `kind == 0` with `sub != 0`. `0x7FC880` is therefore the leading candidate, not a settled one.

The v6.14 source comment "warehouse_covered_combined is type=3 in-window" is suggestive of a *window* reading but is not a measurement either.

**Resolution shipped with this addendum:** the hook is installed with the guard verified byte-for-byte, and `DEBUG hook=` now prints `wkind=<window+0x34>`. One run therefore both tests the fix and proves or refutes the model:

- warehouse panel fires, `wkind=3`, checkbox appears → model confirmed, issue closed;
- no `warehouse` line at all → the warehouse is not kind 3; the same build's log then narrows which arm it *is* (every pre-existing hook prints its own `wkind`, and the dispatch table above maps the rest);
- fires but `wkind` is not 3 → the guard constant is wrong for this build, and the installer's `memcmp` would in fact have refused before writing anything.

### 8.3 Implementation

`ui_panels` v6.15, `HOOK_WAREHOUSE` (index 8), `MAX_HOOKS` 9:

- `RVA_WAREHOUSE_BUILDER 0x7FC880`, `RVA_WAREHOUSE_GUARD 0x6EA5E3`;
- `kWarehouseExpect[17]` = the `CMP/JNZ/MOV/MOV/CALL` cluster from §4;
- `InstallWarehouseCallSiteHook()` — verifies the 17-byte cluster, checks the `E8` resolves to `0x7FC880`, `AllocNear`, writes `FF 25 00000000 <imm64 h_Warehouse>`, re-verifies under `PAGE_EXECUTE_READWRITE`, patches the rel32;
- `h_Warehouse()` → `RunHookBody(..., HOOK_WAREHOUSE, g_exeBase + 0x7FC880)`, mirroring `h_Do`;
- ini `ui_panels.enable` default gains `,warehouse`.

No change to `private_logistics`: `hasNative` and `!isPrivate` already hold for the warehouse, so the checkbox follows automatically once the append pass runs.
