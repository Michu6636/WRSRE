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

### 1.1 What the log shows

Log: 3,080 lines, `17:09:46.742` → `17:14:57.369` (~5 min 11 s).

The player states the warehouse panel was opened **several times** during this run. Four independent once-per-building probes agree that **no widget provider was ever invoked with the warehouse building**:

| # | Probe | Where | Throttle | Fired for | Warehouse |
|---|---|---|---|---|---|
| 1 | `[PRIV TILE] reached <ptr>` | `enterprise_base.cpp:3506-3513` via `:3552` | once per distinct building ptr (`s_seen`) | DO only, `:852` | **absent** |
| 2 | `[AUTO DO] storage widget probe:` | `private_logistics.cpp:2128-2139` | once per distinct building key (`s_lastWidgetKey`) | DO only, `:855` | **absent** |
| 3 | `ui_panels  widget pass refused:` | `ui_panels.cpp:2186-2192` | none | — | **absent** |
| 4 | `ui_panels  DEBUG hook=<name>` | `ui_panels.cpp:2460-2477` | once per distinct window ptr (`s_lastDbgWin`) | `do` ×6, all `object=0x2979acc9580` | **absent** |

Probes 1 and 2 are the strong ones: both are keyed per building and fire on **first sight** of any building that reaches the pass, before any type or privacy gate can suppress them. `TileProbeOnce` (`:3506`) logs before the `type == 39` bail; the storage probe (`:2128`) is commented "ALWAYS log first sight of any building that reaches widget pass (throttled) to prove hook fired". Neither ever saw the warehouse, although the warehouse is a live building whose pointer appears 384 times in the log as a passive B2B delivery target.

### 1.2 The ESC probe cannot see an unhooked window — correcting an earlier misreading

An earlier draft of this document concluded from "6 × `ESC probe: new window`, all paired with `DEBUG hook=do`" that **only the DO window was opened** in the run. **That inference was invalid and has been retracted.**

`EscCloseProbe` (`ui_panels.cpp:1693-1739`) reads `g_frame.win`, and `g_frame` is populated **exclusively** inside `AppendWidgetsImpl` at `:2197-2209` (`g_frame.active = 1; g_frame.win = w;`), cleared at `:2243`, and zeroed on every early return (`:2183`, `:2191`). It also bails at `:1697` when `!g_frame.game || !g_frame.win`.

So the ESC probe is **hook-driven, not a window-system hook**: it can only ever report windows that already reached one of the 8 hooked builders. A window built by an unhooked builder leaves `g_frame` at its previous/zero state and produces **no ESC line at all**. The probe's silence about the warehouse is therefore *expected* if the warehouse is unhooked — it is not evidence that the window was never opened.

The same blindness explains the other silences: `AppendWidgetsImpl` only runs from `RunHookBody`, so no hook ⇒ no provider ⇒ no `[PRIV TILE]`, no storage probe, no `refused` line.

### 1.3 What this now establishes

Given the player opened the warehouse several times and **all four** independent probes are silent for it, the conclusion is:

> **The `warehouse_covered_combined` window is not served by any of the 8 hooked builders.** Its panel is completely vanilla — no plugin rows and no plugin widgets of any kind, from any of the four providers.

This is consistent with the player's report and is *stronger* than the previous draft's conclusion, not weaker, because it rests on per-building first-sight probes rather than on the hook-driven ESC probe.

**One residual doubt, and it is cheap to kill:** probe 4 is the only one throttled on *window pointer* rather than building, and the engine reuses window allocations. If the warehouse repeatedly reused the exact window pointer that `s_lastDbgWin` already held, its `DEBUG hook=` line could be suppressed. Probes 1 and 2 are keyed per *building* and could not be suppressed this way — but they sit downstream of `AppendWidgetsImpl` being reached at all. To make the case airtight, drop the throttle for one run (see Block 0).

### 1.4 What else the log proves — the provider's preconditions already pass

The world scan at `private_logistics.cpp:2472` and the widget provider at `:2142` use **the same predicate**:

```c
// world scan (:2472)                          // widget provider (:2142-2150)
if (AutoHasNativeStorage(b) &&                 hasNative = AutoHasNativeStorage(b);
    type != AUTO_CUSTOMS_TYPE &&               if (!hasNative) return;
    type != AUTO_DO_ROAD_TYPE &&               if (isBuildingPrivate(b)) return;
    type != AUTO_DO_RAIL_TYPE) {               // else: draw the checkbox
      if (isPrivate) ... else -> state pool
```

`:1751` records `state storage candidate name=warehouse_covered_combined key=D0BA81CF239FFE8F pos=(-5758.8,5325.1) permission=OFF-DEFAULT`, emitted from the **non-private branch** of that exact predicate. Therefore, at runtime:

* `AutoHasNativeStorage(warehouse) == true` — it could not have reached that branch otherwise;
* `isBuildingPrivate(warehouse) == false` — it took the state branch (every B2B line logs `priv=0`);
* `type != 43 / customs` — it passed those guards.

**Both conditions the widget provider checks are already satisfied for the warehouse.** If the provider were ever called with the warehouse's `Building*`, it *would* draw the checkbox — `type != 5` is explicitly tolerated at `:2152-2154`.

So the defect is **not** in the provider, **not** in `AutoHasNativeStorage`, and **not** in the privacy gate. It is upstream: no hooked builder fires for that window.

The deadlock:

```
permission = 1   <-  only set by the checkbox (AutoStatePermissionSet)
checkbox drawn   <-  provider called AND hasNative AND !private   (all three already true)
provider called  <-  AppendRowsImpl runs, i.e. a hooked builder fired   <-- BREAKS HERE
```

`:2141` then explains the second symptom: `hasStorage = (forceOn || site.permission == 1) ? 1 : 0`, and the day summary reads `candidates=2 planned=2` (`:1754`) out of 18 discovered candidates — the warehouse's `OFF-DEFAULT` keeps it out of the plan.

### 1.5 The four widget providers (corrected inventory)

An earlier draft said the shop checkboxes came from one provider. That was wrong. Four providers are registered, in this order (`tesmioloader.log:70/108/126/173`):

| # | plugin | widget | type gate |
|---|---|---|---|
| 1 | `citizen_labor` | wage slider + **`"Apply to Whole Sector"`** checkbox (`:2502`, `:2521`) — workplace vs whole-category scope, exactly the pattern the player cites as the model | `LaborWindowType` (`:2364`): type in `[g_workplaceMin, g_workplaceMax]`, not excluded, not 39, not private |
| 2 | `enterprise_base` | `[ Private Enterprise ]` checkbox (`:3600`) or the wide `MAKE IT PRIVATE` bar (`:3579`) | `TsmLooksLikeBuilding`, bail on type 39 |
| 3 | `private_logistics` | **`"Allow private DOs to load here"`** checkbox (`:2161`) — the one this request is about | `hasNative && !private` only; **no type gate** |
| 4 | `taxes` | `[ Fiscal Policy & Taxes ]` (`:974`) | type 39 |

`private_shops`, `citizen_retail` and `private_construction` register no widget provider at all — their panels are text-row only, and `private_construction` is a stub. So the checkbox a player sees on a shop is provider 2, and on a wage building it is provider 1.

This is the precedent that matters: providers 1 and 3 both draw a plain scope checkbox on a **state** building. The warehouse is a state building (`priv=0`, proven). Nothing distinguishes it from a wage building except that no hooked builder fires for its window.

### 1.6 Why the v6.15 evidence does not settle it

`docs/ghidra-request-warehouse-storage-v6.15.md` §1 concluded:

> "`storage diag: 307 calls, 1 in storage range 0x4A0000..0x4E0000` — exhaustive dispatcher dump proves **exactly one storage builder** in this build. The 'warehouse variant builder' hypothesis is disproved; all buildings route through `0x4CABD0`."

**That reading is too strong and should not be relied on.** The diagnostic scans the dispatcher `0x6E9B80..+0x8000` for `E8` targets and counts how many land in `0x4A0000..0x4E0000`. It proves there is **one storage-builder call site inside the dispatcher**, not that every storage-capable building reaches it. `docs/ghidra-findings-warehouse-storage-v6.15.md` §"Open" records the same gap:

> "Warehouse numeric type not fully resolved: The `td+0x360` (TDM_TYPE_NUM) for `warehouse_covered_combined` was not traversed from the exe string to confirm whether the exe assigns type 5 or type 3."

So the question this request must answer is narrow:

> **When the player opens the `warehouse_covered_combined` window, which function draws it, and is that function one of the 8 currently hooked builders?**

If the answer is "one of the 8", the defect is in the plugin's append gates (§1.7) and no new hook is needed. If it is a 9th builder, the deliverable is a hookable RVA with verified prologue bytes.

### 1.7 The 8 hooked surfaces (for cross-reference)

| idx | name | target RVA | hook kind |
|---|---|---|---|
| 0 | shop | `0x7272F0` | inline, 21 B |
| 1 | residence | `0x761300` | inline, 19 B |
| 2 | pub | `0x723950` | inline, 21 B |
| 3 | factory | `0x799630` | inline, 21 B |
| 4 | cityhall | `0x72DBF0` | inline, 17 B |
| 5 | citizen | `0x7CEEB0` | inline, 15 B |
| 6 | DO | `0x741170` | **call-site** at `0x6EB4FD` |
| 7 | storage | `0x4CABD0` | inline, 22 B |

All 8 reported `hook ok` in the log. Widget providers run only from `AppendRowsImpl` (`ui_panels.cpp:2401`), which runs only from `RunHookBody` for these surfaces.

---

## 2. Work to run

### Block 0 — Confirm the unhooked finding (no Ghidra, ~3 minutes)

§1.1 already points hard at "no hooked builder fires for the warehouse window", but probe 4 is throttled on window pointer (`s_lastDbgWin`, `:2466`), so a reused allocation could in principle hide it. One run makes it airtight.

1. Temporarily change `ui_panels.cpp:2467` so the `DEBUG hook=` line logs **once per (hook, window) pair** rather than only on a window change — e.g. key the static on `idx` and log whenever `hk->probed` is still 0, or simply drop the `if (w != s_lastDbgWin)` guard for the test. Rebuild `ui_panels`, confirm a fresh DLL timestamp (`INSTRUCTIONS.md:122-130`).
2. Load the save, open **only** the `warehouse_covered_combined` panel, leave it open ≥5 s, close it, then open one shop and one wage building as controls.
3. Read the log:
   * `hook=<name>` for the warehouse → the builder **is** hooked; go to **Block E**.
   * nothing for the warehouse, but `hook=shop` / `hook=<wage building>` for the controls → the warehouse window is **unhooked**; go to **Block A0**.
   * `hook=<name>` for the warehouse **but no providers ran** → the pass was reached and returned silently. The only silent returns above the providers are `ui_panels.cpp:2172` (`g_widgetCount <= 0`) and **`:2173` (`!building || !game || !w`)** — i.e. `window+0x240` was NULL. That is the one alternative to "unhooked" that all four §1.1 probes cannot distinguish, because a NULL building also suppresses `[PRIV TILE]`, the storage probe and `widget pass refused` (`:2186` is only reached when `building` is non-NULL). Record the `object=` value explicitly to settle it.

4. Also record: does the warehouse panel show the `citizen_labor` wage slider? If `LaborWindowType` accepts its type (`:2372`), the warehouse panel should carry provider 1's controls as well. Their presence or absence is an independent signal about whether the pass ran at all, and which providers saw it.

**Pass criteria:** for the warehouse window, a definite statement — "these hooks fired: …" or "no hook fired" — plus the `object=` (`window+0x240`) value.

> Note: per §1.2, `ESC probe` is **not** usable as a window detector here. It reads `g_frame.win`, which only exists during a hook-driven append pass. Do not draw conclusions from ESC lines alone.

### Block A0 — The dispatcher dump is already in the log, and it has a repeating structure

`DumpDispatcherTargets` (`ui_panels.cpp:2651-2682`) does a **blind byte-wise linear sweep**: for `off` in `0..DISPATCH_SCAN_LEN` (`0x8000`, `:1066`) starting at `RVA_WINDOW_DISPATCH 0x6E9B80` (`:1065`), it treats **every `0xE8` byte** as a `call rel32`. It is therefore *not* function-scoped, and some hits are false positives (immediate/displacement bytes equal to `0xE8`). Treat all 172 "targets" as candidates, not as call sites.

The 2026-10-23 log (`tesmioloader.log:180-489`) contains **307 hits / 172 distinct targets**.

#### A0.1 — Confirmed finding: `0x6F8CC0` is a shared per-arm preamble, NOT a builder

This matters because it was the leading hypothesis and it is **wrong**. Evidence:

1. The 69 hits on `0x6F8CC0` are **not** scattered. They occupy `0x6EA7D1` … `0x6EB5FE`, a span of only `0xE2D`, at a near-constant stride (`0x23` = 35 on 39 of 68 gaps, `0x20` = 32 on 10). That is a **switch with ~69 arms of equal size**, not 69 unrelated callers.
2. Inside every arm the same idiom repeats: `call 0x6F8CC0` at `S`, then **0x0B bytes later** `call <kind-specific target>`, with a **different** target in each of the 69 arms (69 distinct).
3. Decisive: **five of the eight already-hooked builders are exactly that second call**, at delta `0x0B`:

   | arm head | +0x0B target | hooked builder |
   |---|---|---|
   | `0x6EAF2D` | `0x6EAF38` | pub (idx 2) |
   | `0x6EAF73` | `0x6EAF7E` | shop (idx 0) |
   | `0x6EB262` | `0x6EB26D` | factory (idx 3) |
   | `0x6EB3BD` | `0x6EB3C8` | residence (idx 1) |
   | `0x6EB4F2` | `0x6EB4FD` | DO (idx 6) |

   5/5 at exactly `+0x0B` is not coincidence: `0x6F8CC0` is the **common preamble each arm runs before its own builder**.

4. The other three hooked builders are in the same region but not at `+0x0B`, because their arms have extra code between: storage `0x6EA80C` is `+0x3B` from the arm head `0x6EA7D1`; cityhall `0x6EA8EF` is `+0x11E`; citizen `0x6EA931` is `+0x160`. (Those three arms therefore contain *several* calls each.)

**Consequence:** `0x6F8CC0` is very probably the shared "create/attach the panel window" helper that all ~69 window kinds call. Hooking it once could cover every window kind at a stroke — **if** it is handed the window pointer. That is now the highest-value question, because it would fix the warehouse *and* every other unhooked panel permanently. But do **not** assume it receives the window; A0.2/A0.3 decide.

**The warehouse's own builder is one of the ~64 kind-specific second-call targets that are not yet hooked.**

#### Questions

**A0.2 — What is `0x6F8CC0`'s signature?** Decompile it. Report:
   - how many arguments, and which register/stack slot carries the **window** object (the thing `AppendRowsImpl` receives as `w`), and
   - which carries the **building** (`W_BUILDING` at `w+0x240` is what the plugin needs), and
   - whether it is called exactly once per window open (69 arms × one window each ⇒ 69 window kinds).

**A0.3 — Is `0x6F8CC0` hookable inline?** Report its first **32 bytes**. The known head `48 8B C4 55 56 57 41 54` is `mov rax,rsp; push rbp; push rsi; push rdi; push r12` = 9 bytes, which is **short of the 14 bytes** needed for the abs-jmp trampoline (`INSTRUCTIONS.md:122-130`, `.clinerules` §B). Report instruction boundaries up to the first clean boundary ≥14, and flag any RIP-relative/`rel32` operand or `/GS` cookie in that range. If no clean ≥14 exists, say so (`0x6F8CC0` being a *naked-ish* short prologue makes this likely) and fall back to A0.4.

**A0.4 — Identify the warehouse arm.** For each of the ~69 arms, report the arm head `S`, the `+0x0B` target, and the guard that selects that arm (`CMP reg,imm` or jump-table index). Then answer: **which arm is reached for `warehouse_covered_combined` / a type-5-or-`$TYPE_STORAGE` building?**
   - Give the `+0x0B` target for that arm: that is the candidate 9th hook.
   - Say whether the guard keys on a **building type id**, a **window kind**, or something else. Note the live log prints `kind 0x0` for every panel and `ui_panels`'s own `PanelHook` carries a `kind`, so "type" and "kind" may be different fields — do not conflate them.
   - If the arms are *not* type-keyed, state exactly what the discriminator is.

**A0.5 — Cross-check against the already-hooked set.** Confirm that the 69 arm targets, minus {shop, pub, factory, residence, DO, cityhall, citizen, storage}, leave the ~64 candidates. If `0x6F8CC0` is the preamble, the cleanest fix may be **one hook on `0x6F8CC0`** instead of a 9th per-kind hook — report which of the two you recommend and why.

**Pass criteria:** (a) `0x6F8CC0`'s signature, naming the window and building arguments; (b) a yes/no on whether it is called once per window open; (c) the arm target for the warehouse, or a precise statement of what the arms are actually keyed on; (d) a hookability verdict for `0x6F8CC0`.

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
shop                       0x7272F0   (inline, 21 B)
residence                  0x761300   (inline, 19 B)
pub                        0x723950   (inline, 21 B)
factory                    0x799630   (inline, 21 B)
cityhall                   0x72DBF0   (inline, 17 B)
citizen                    0x7CEEB0   (inline, 15 B)
warehouse name string      ~0x8EBF00  (reader FUN_140652580 at 0x652694)

candidate preamble         0x6F8CC0   head 48 8B C4 55 56 57 41 54
```

**The 5 verified arm pairs** — each row is `call 0x6F8CC0` at `S`, then `call <builder>` exactly 0x0B later. These are already-hooked builders, so this correspondence is self-validating:

```
0x6EAF2D -> 0x6F8CC0   ...  0x6EAF38 -> 0x723950   pub        (idx 2)
0x6EAF73 -> 0x6F8CC0   ...  0x6EAF7E -> 0x7272F0   shop       (idx 0)
0x6EB262 -> 0x6F8CC0   ...  0x6EB26D -> 0x799630   factory    (idx 3)
0x6EB3BD -> 0x6F8CC0   ...  0x6EB3C8 -> 0x761300   residence  (idx 1)
0x6EB4F2 -> 0x6F8CC0   ...  0x6EB4FD -> 0x741170   DO         (idx 6)
```

The three arms with extra code between preamble and builder (so *not* at `+0x0B`):

```
0x6EA7D1 -> 0x6F8CC0   ...  0x6EA80C -> 0x4CABD0   storage    (idx 7)   delta 0x3B
0x6EA7D1 -> 0x6F8CC0   ...  0x6EA8EF -> 0x72DBF0   cityhall   (idx 4)   delta 0x11E
0x6EA7D1 -> 0x6F8CC0   ...  0x6EA931 -> 0x7CEEB0   citizen    (idx 5)   delta 0x160
```

The 69 arms span `0x6EA7D1`–`0x6EB5FE` (0xE2D bytes); stride histogram `{0x23:39, 0x20:10, 0x27:3, ...}`. Full ordered list is `tesmioloader.log:180-489`.

Log excerpts (2026-10-23 run):

```
[17:09:48.579] hook ok      storage  target=00007FF6C245ABD0 tramp=00000296C0D70000 (22 bytes stolen)
[17:09:48.579] ui_panels  live v6.14: 8 hook(s), ... probe 1 ...
[17:13:54.543] ui_panels  DEBUG hook=do idx=6 kind=0  object=0x2979acc9580
[17:13:54.555] [AUTO DO] storage widget probe: building=000002979ACC9580 type=43 key=3D976F9DD50CEB5D priv=1 nativeStorage=1 willDraw=0
[17:13:54.556] [AUTO DO] storage widget: hasStore but isPrivate=1, skipping checkbox (building=000002979ACC9580 type=43)
[17:14:11.743] [AUTO DO] state storage candidate name=warehouse_covered_combined key=D0BA81CF239FFE8F pos=(-5758.8,5325.1) permission=OFF-DEFAULT
[17:14:11.745] [AUTO DO] day 8 summary: private offices=1 candidates=2 planned=2 added=2 existing=0 suppressed=0 invalid=0 mode=WRITE
[17:13:54.548] ui_panels  ESC probe: new window | ... win 000002969E9EB760 kind 0x0 hidden 0 winClose 0
```

## 4b. Fix plan — the four outcomes and what to do in each

The investigation has exactly four possible outcomes. Each maps to one edit; pick the row that matches the findings.

| # | Finding | Fix | Effort | Coverage |
|---|---|---|---|---|
| **A** | `0x6F8CC0` is called once per window open and receives both the window and the building | **1 hook on `0x6F8CC0`** as a 9th `g_hooks[]` row | small | **all ~69 window kinds** — warehouse, construction sites, everything currently unhooked |
| **B** | `0x6F8CC0` is a preamble that does *not* receive the window, but Block A0.4 names the warehouse's arm target | **1 hook on that arm target** as a 9th row | small | the warehouse only |
| **C** | The arm target's prologue has no clean ≥14-byte steal (Block D) | **call-site hook** on the arm's `call`, pattern already proven for the DO at `0x6EB4FD` | medium | the warehouse only |
| **D** | Block 0 shows a hooked builder *does* fire for the warehouse — **unlikely** after §1.1, but not yet excluded | **no new hook** — fix `AppendRowsImpl`'s gates (`W_HIDDEN` 0x01, `W_BUILDING` 0x240, `TsmLooksLikeBuilding`) | small | n/a |

Why option **A** is worth testing first: if it holds, one hook replaces the per-type table and the "9th builder" treadmill ends permanently. Every future building type gets widgets for free. If it fails A0.2 (no window argument), fall back to **B** without further analysis.

**No plugin change is needed for any of A–C.** The storage checkbox is already gated on `hasNative && !isPrivate`, both of which the warehouse already satisfies (§1.2). Once `AppendRowsImpl` runs for the warehouse window, the checkbox appears and `AutoStatePermissionSet` can set `permission=1`, which is the only thing standing between the warehouse and `planned=`. Options A–C are `ui_panels` `g_hooks[]` edits only.

**Wiring after the hook exists** (unchanged, for completeness): `ui_panels.cpp:1073-1087` `g_hooks[]` row + `install` path; `state_storage_always_on` and the name-based allowlist are **not** the fix and should not be reintroduced.

## 5. What NOT to do

- **Do not** re-derive what is already proven: the tank offsets, the burn/refuel path, the 16 transfer call sites, `param_9`'s per-site values. None of that is in scope.
- **Do not** treat `storage diag: 307 calls, 1 in storage range` as proof that all buildings share one builder. That is the specific over-reading this request exists to correct. Note also that `DumpDispatcherTargets` (`:2651`) is a blind byte scan that treats every `0xE8` byte as a call, so some of its 172 "targets" are false positives.
- **Do not** use `ESC probe` lines as a window detector. `EscCloseProbe` reads `g_frame.win`, which is only set inside the hook-driven `AppendWidgetsImpl`; an unhooked window produces no ESC line. See §1.2.
- **Do not** conclude from an absence of `DEBUG hook=` lines alone that a window was never opened — that line is throttled on window pointer (`s_lastDbgWin`). Use the per-building probes 1 and 2 from §1.1, or Block 0.
- **Do not** change `private_logistics.cpp`, `ui_panels.cpp`, the ini, or the build during the investigation.
- **Do not** propose a name-based allowlist (`if name == "warehouse_*"`) as the fix. It is the interim workaround, not the answer.
- **Do not** run two `analyzeHeadless` sessions concurrently — the project takes an exclusive lock.
- **Do not** report a builder route without the `CALL` address that proves it.
