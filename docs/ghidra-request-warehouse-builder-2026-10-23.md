# Ghidra Request — Which builder draws the warehouse window? (2026-10-23)

**Branch:** `arena/4237da81-wrsre`
**Target binary:** `SOVIET64.exe` v1.1.1.9, timestamp `0x6A3EB6AD`, base `0x140000000`; all addresses are RVAs unless noted. Engine `C3DDLL64.dll`.
**Author:** code agent (Arena). Date 2026-10-23.
**Rule:** investigation-only — use `scratch/ghidra_run.bat`, one `analyzeHeadless` at a time, `.java` scripts. Do not edit source, config, build files or docs during the investigation.

**Build under test:** `ui_panels` 6.14, `private_logistics` 0.17.
**Predecessor:** `docs/ghidra-request-warehouse-storage-v6.15.md` (Blocks A–D) and `docs/ghidra-findings-warehouse-storage-v6.15.md`.

---

## 1. Why this request exists

The player reports two symptoms, and the 2026-10-23 `tesmioloader.log` (17:09:46 – 17:14:43, 3080 lines) shows they share one cause:

> "The check box for the panel of the warehouse did not appear, nor did the DO detect it."

### 1.1 What the log proves

| Observation | Evidence |
|---|---|
| The widget pass ran **590 times in 4.1 s**, always on the **private DO** | `17:13:54.555 → 17:13:58.636`, every line `building=000002979ACC9580 type=43 … priv=1 nativeStorage=1 willDraw=0` |
| Every one was rejected by the private gate | `private_logistics.cpp:2147` → `"[AUTO DO] storage widget: hasStore but isPrivate=1, skipping checkbox"` ×590 |
| `type=43` is the **DO**, not the warehouse | `AUTO_DO_ROAD_TYPE 43` (`private_logistics.cpp:1929`); the same building carries office key `3D976F9DD50CEB5D` in the day-8 summary |
| **No other building ever reached a hooked builder** | All six `DEBUG hook=` lines are `hook=do idx=6 object=0x2979acc9580`; no `hook=shop`, `hook=storage`, `hook=residence`, `hook=pub`, `hook=factory`, `hook=cityhall` anywhere in the log |
| The warehouse **was** discovered by the world scan | `17:14:11.743  state storage candidate name=warehouse_covered_combined key=D0BA81CF239FFE8F pos=(-5758.8,5325.1) permission=OFF-DEFAULT` |
| …but never planned as a target | `:2633` `e.hasStorage = (forceOn \|\| site.permission == 1) ? 1 : 0;` — `state_storage_always_on=0`, so `hasStorage=0`; day-8 line reads `candidates=2 planned=2 added=2` (the two *private* storages) |

### 1.2 The deadlock

```
permission = 1   ←  only set by the checkbox (AutoStatePermissionSet)
checkbox drawn   ←  provider called AND building is not private  (:2147)
provider called  ←  AppendRowsImpl runs, i.e. one of the 8 hooked builders fired
```

The log shows the provider never ran on a warehouse, so the checked state can never be created, so the DO never plans the warehouse.

### 1.3 Why the existing evidence does not settle it

`docs/ghidra-request-warehouse-storage-v6.15.md` §1 concluded:

> "`storage diag: 307 calls, 1 in storage range 0x4A0000..0x4E0000` — exhaustive dispatcher dump proves **exactly one storage builder** in this build. The 'warehouse variant builder' hypothesis is disproved; all buildings route through `0x4CABD0`."

**That reading is too strong and should not be relied on.** The diagnostic scans the dispatcher `0x6E9B80..+0x8000` for `E8` targets and counts how many land in `0x4A0000..0x4E0000`. It proves there is **one storage-builder call site inside the dispatcher**, not that every storage-capable building reaches it. `docs/ghidra-findings-warehouse-storage-v6.15.md` §"Open" records the same gap:

> "Warehouse numeric type not fully resolved: The `td+0x360` (TDM_TYPE_NUM) for `warehouse_covered_combined` was not traversed from the exe string to confirm whether the exe assigns type 5 or type 3."

So the question this request must answer is narrow:

> **When the player opens the `warehouse_covered_combined` window, which function draws it, and is that function one of the 8 currently hooked builders?**

If the answer is "one of the 8", the defect is in the plugin's append gates (§1.4) and no new hook is needed. If it is a 9th builder, the deliverable is a hookable RVA with verified prologue bytes.

### 1.4 The 8 hooked surfaces (for cross-reference)

| idx | name | target RVA | hook kind |
|---|---|---|---|
| 0 | shop | `0x6B72F0` | inline, 21 B |
| 1 | residence | `0x6F1300` | inline, 19 B |
| 2 | pub | `0x6B3950` | inline, 21 B |
| 3 | factory | `0x729630` | inline, 21 B |
| 4 | cityhall | `0x6BDBF0` | inline, 17 B |
| 5 | citizen | `0x75EEB0` | inline, 15 B |
| 6 | DO | `0x741170` | **call-site** at `0x6EB4FD` |
| 7 | storage | `0x4CABD0` | inline, 22 B |

All 8 reported `hook ok` in the log. Widget providers run only from `AppendRowsImpl` (`ui_panels.cpp:2401`), which runs only from `RunHookBody` for these surfaces.

---

## 2. Work to run

### Block 0 — Runtime discriminator first (no Ghidra, 2 minutes, decisive)

Before any static work, settle whether the warehouse window reaches a hooked builder **at all**:

1. Start the game, load the save, open **only** the `warehouse_covered_combined` window, leave it open ≥5 s, then close it.
2. `grep "DEBUG hook=" tesmioloader.log`
3. Interpret:
   * a new line appears (e.g. `hook=shop idx=0` or `hook=storage idx=7`) → the builder **is** hooked, and the defect is downstream. Go to **Block E**.
   * no new line appears → no hooked builder draws that window. Go to **Block A**.
4. Also record whether any `ui_panels  widget pass refused:` or `TsmLooksLikeBuilding` line appears — that is Block E evidence.

**Pass criteria:** one line stating which (if any) hook fired for the warehouse window, with the `object=` pointer, plus the `w` pointer implied by the `DEBUG` throttle.

> Note: the `DEBUG hook=` line is throttled on a *changing window pointer* (`ui_panels.cpp:2466-2475`, `static void* s_lastDbgWin`), and it is emitted whenever `g_probe` is on (it is: `probe 1`). If the engine reuses one window object for all building panels, a repeat open of a *different* building may not log. If that is suspected, temporarily log unconditionally for the test, or rely on Block A.

### Block A — Dispatcher case table → builder RVA, complete (answers the routing question)

The dispatcher is `FUN_1406E9B80`. The v6.15 dump only classified `E8` targets by RVA range; this block must recover the **type → builder** mapping.

1. `DisasmMany 0x1406E9B80:0x1406EB200` → `scratch/dispatch_full.asm`
   Extract every `CMP ECX,imm32` / `JNZ` / `JE` chain and the `CALL rel32` it guards. For each, resolve `target = site + 5 + rel32`.
2. Produce the table:

   | ECX (type) | guard address | CALL site | builder RVA | already hooked? |
   |---|---|---|---|---|
   | 3 | … | `0x6EA80C` | `0x4CABD0` | **yes, idx 7** |
   | 5 | ? | ? | ? | ? |
   | 43 | `0x6EB4FD` | `0x741170` | yes, idx 6 |
   | 39 | ? | `0x72DBF0` | yes, idx 4 |

   Fill every row; do not stop at the known ones.
3. Call out explicitly: **is there any `CALL` target in this function that is not one of the 8 RVAs in §1.4?** That is the candidate 9th builder.
4. `Xrefs 0x1404CABD0` → `scratch/storage_xrefs.txt`: list every distinct caller. If the warehouse can reach `0x4CABD0` from anywhere other than `0x6EA80C`, say where.

**Pass criteria:** a complete type→builder table, and a yes/no answer to "does the dispatcher contain a builder call site outside the 8 hooked RVAs".

### Block B — `warehouse_covered_combined` → `td+0x360` TYPE_NUM (the unfinished v6.15 item)

This resolves whether the warehouse presents as type 5 (`$TYPE_STORAGE` in its ini) or something else, which decides *which* dispatcher row it takes.

1. `StrAddr.java` on `warehouse_covered_combined` → string address + xrefs (`scratch/straddr_warehouse.txt`). v6.15 found it at `0x1408EBF00`-ish, read from `0x140652694` in `FUN_140652580`.
2. Walk string → type-descriptor pointer → `td+0x360` (TDM_TYPE_NUM). Report the integer.
3. If the traversal dead-ends, state exactly where, and say whether the type descriptor is loaded from `media_soviet/buildings_types/warehouse_covered_combined.ini` at runtime (in which case the answer is `$TYPE_STORAGE` = 5 by construction and the divergence is in the *in-window* type read, not the asset).

**Pass criteria:** a numeric `td+0x360` for the warehouse, or a precise statement that it is asset-loaded and the divergence must be measured at runtime.

### Block C — Runtime type fields: which one is "the" type?

The plugin reads a type that returned `43` for the DO and `3` for the warehouse in the earlier log. Identify which field each caller uses, so the plugin can read the *dispatcher's* type instead of a guess.

1. `FindDisp 34` (hex `0x34`) and confirm the reader used by `FUN_1406E9B80` to compute `ECX` before the `CMP` chain. Is it `b+0x34`, `b+0x248`, `*(b+0x318)+0x360`, or `b+0xADF0`?
2. `FindDisp 248` / `FindDisp 318` / `FindDisp AD F0` — which of these appear inside `FUN_1406E9B80`?
3. Report the exact expression the dispatcher evaluates, in decompiler form.

**Pass criteria:** one expression, quoted, that yields the type the dispatcher switches on — and whether `TsmBuildingType(b)` (which the plugin already calls) evaluates the same expression.

### Block D — If Block A finds a 9th builder: make it hookable

Only if Block A shows the warehouse reaching a builder outside the 8.

1. `DisasmMany <builder>:<builder>+0x40` — instruction lengths, boundaries.
2. Report the largest clean steal ≥14 bytes that splits no `rel32`/`rel8`, and does not cross the `/GS` cookie.
3. Compare against `InstallInlineHook` requirements in `.clinerules` §B.
4. If no clean ≥14-byte steal exists, say so and recommend a **call-site hook** on the dispatcher's `CALL` instead (pattern already proven for the DO at `0x6EB4FD`).

**Pass criteria:** either `RVA + expect[] + stolen` ready to paste, or a call-site site RVA with its `E8`/`rel32` arithmetic verified.

### Block E — Only if Block 0 shows a hooked builder already fires

Then the window *is* visible and the defect is in the append gates. Check, in this order:

1. `ui_panels.cpp:2248-2260` early returns — is `*(char*)(w + W_HIDDEN) != 0` true for the warehouse window (`W_HIDDEN 0x01`, `:530`)?
2. `*(void**)(w + W_BUILDING)` — is it NULL, or a non-`Building` object? (`W_BUILDING 0x240`, `:536`; `TsmLooksLikeBuilding` at `:2186`). If `g_probe=1` this already logs `widget pass refused`.
3. `TsmLooksLikeBuilding` — state the exact test it performs and whether a warehouse satisfies it.
4. `private_logistics.cpp:2147` — confirm `isBuildingPrivate(warehouse)` is false at runtime (the world scan already says it is a *state* storage, so it should be).

**Pass criteria:** the single gate that rejects the warehouse window, quoted with its line number.

---

## 3. Deliverable format (strict)

Write `docs/ghidra-findings-warehouse-builder-2026-10-23.md` containing:

1. **Answer to the one question**, in the first paragraph: *which function draws the warehouse window, and is it one of the 8 hooked builders?*
2. **The complete type→builder table** from Block A, every row filled.
3. **`td+0x360` for `warehouse_covered_combined`** (a number, or the precise reason it cannot be obtained).
4. **The dispatcher's type expression**, quoted from the decompiler.
5. **A recommendation**, one of:
   * `HOOK NEW BUILDER` — RVA + `expect[]` bytes + stolen size, ready to paste into a 9th `g_hooks[]` row;
   * `CALL-SITE HOOK` — dispatcher site RVA + `E8` arithmetic;
   * `ALREADY HOOKED — DEFECT ELSEWHERE` — with the failing gate from Block E.
6. **Evidence appendix**: raw addresses and instruction text for every claim. No claim without an address.

## 4. Appendix — addresses and log excerpts for copy-paste

```
dispatcher                 0x6E9B80
dispatcher -> storage      CALL at 0x6EA80C -> 0x4CABD0
dispatcher -> DO           CALL at 0x6EB4FD -> 0x741170
storage builder            0x4CABD0   prologue 22:
  40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00
shop                       0x6B72F0   (inline, 21 B)
residence                  0x6F1300   (inline, 19 B)
pub                        0x6B3950   (inline, 21 B)
factory                    0x729630   (inline, 21 B)
cityhall                   0x6BDBF0   (inline, 17 B)
citizen                    0x75EEB0   (inline, 15 B)
warehouse name string      ~0x8EBF00  (reader FUN_140652580 at 0x652694)
```

Log excerpts (2026-10-23 run):

```
[17:09:48.579] hook ok      storage  target=00007FF6C245ABD0 tramp=00000296C0D70000 (22 bytes stolen)
[17:09:48.579] ui_panels  live v6.14: 8 hook(s), ... probe 1 ...
[17:13:54.543] ui_panels  DEBUG hook=do idx=6 kind=0  object=0x2979acc9580
[17:13:54.555] [AUTO DO] storage widget probe: building=000002979ACC9580 type=43 key=3D976F9DD50CEB5D priv=1 nativeStorage=1 willDraw=0
[17:13:54.556] [AUTO DO] storage widget: hasStore but isPrivate=1, skipping checkbox (building=000002979ACC9580 type=43)
[17:14:11.743] [AUTO DO] state storage candidate name=warehouse_covered_combined key=D0BA81CF239FFE8F pos=(-5758.8,5325.1) permission=OFF-DEFAULT
[17:14:11.745] [AUTO DO] day 8 office=3D976F9DD50CEB5D type=43 existing=0 planned=2 added=2
```

## 5. What NOT to do

- **Do not** re-derive what is already proven: the tank offsets, the burn/refuel path, the 16 transfer call sites, `param_9`'s per-site values. None of that is in scope.
- **Do not** treat `storage diag: 307 calls, 1 in storage range` as proof that all buildings share one builder. That is the specific over-reading this request exists to correct.
- **Do not** change `private_logistics.cpp`, `ui_panels.cpp`, the ini, or the build during the investigation.
- **Do not** propose a name-based allowlist (`if name == "warehouse_*"`) as the fix. It is the interim workaround, not the answer.
- **Do not** run two `analyzeHeadless` sessions concurrently — the project takes an exclusive lock.
- **Do not** report a builder route without the `CALL` address that proves it.
