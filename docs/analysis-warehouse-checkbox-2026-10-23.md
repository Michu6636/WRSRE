# Why the storage checkbox never appears on the warehouse panel (2026-10-23)

**Branch:** `arena/453ef127-wrsre`
**Log analysed:** `tesmioloader.log`, 3,080 lines, `17:09:46.742` → `17:14:57.369`
**Build in that log:** `ui_panels` **v6.14**, 8 hooks. `private_logistics` 0.17.
**Question:** the "Allow private DOs to load here" checkbox renders on state-owned buildings and on the grocery store / DO, but never on `warehouse_covered_combined`.

---

## 0. Read this first — the log predates the fix

The uploaded log is **not** a post-fix run. Two independent proofs:

| Evidence | Line | Meaning |
|---|---|---|
| `ui_panels  live v6.14: 8 hook(s)` | `:501` | the v6.15 9th hook does not exist in this binary |
| `grep -c wkind` → **0** | — | `wkind=` is emitted by v6.15 only |
| `ui_panels  do: hook 0x6EB4FD …` then `live v6.14` | `:498`, `:501` | this is the same 17:09 run previously analysed |

So every conclusion below about **why** the checkbox is missing is derived from the *code path* plus this log's positive evidence. The log cannot test the fix; §7 says exactly what a valid post-fix log must contain.

---

## 1. What the log establishes

| Fact | Evidence |
|---|---|
| All 8 pre-v6.15 hooks installed | `:492-499` — shop, residence, pub, factory, cityhall, citizen, do, storage |
| Only the **DO** panel was ever built | 6 × `DEBUG hook=do idx=6 kind=0 object=0x2979acc9580` (`:849, 1636, 2203, 2457, 2583, 2883`) |
| The widget pass ran **once**, on the DO | 1 × `[AUTO DO] storage widget probe:` (`:855`) |
| …and the checkbox was correctly **refused** there | 590 × `hasStore but isPrivate=1, skipping checkbox (type=43)` (`:856` → `:3039`) |
| The checkbox was never instantiated anywhere | `grep -c "widgetCheckbox returned 0"` → 0; no toggle/permission lines |
| The warehouse is a discovered storage candidate | `:1751` `state storage candidate name=warehouse_covered_combined key=D0BA81CF239FFE8F pos=(-5758.8,5325.1) permission=OFF-DEFAULT` (1 of 18) |
| …but is never planned | `:1754` `candidates=2 planned=2`; `hasStorage = (forceOn \|\| permission == 1)` and permission is `OFF-DEFAULT` |
| No window-level rejection ever fired | zero `widget pass refused` lines |

The 590 refusals are **correct behaviour**, not the bug: that building is the player's own private DO (`priv=1`), and a private DO must not be offered as a state storage source.

---

## 2. The initialisation flow, stage by stage

Both cases share **every** stage after the first. That is the whole answer.

| # | Stage | Where | Working case (state-owned shop / grocery store) | Warehouse |
|---|---|---|---|---|
| 1 | Game calls the window's builder | dispatcher | shop `0x7272F0` etc. — **hooked** | `0x7FC880` (window kind 3) — **not hooked in v6.14** |
| 2 | Detour → `RunHookBody` | `:2428` | runs | **never runs** |
| 3 | Vanilla panel drawn | `real(g,w,p3)` `:2445` | yes | game draws it without us |
| 4 | `DEBUG hook=` emitted | `:2473` | yes | **never** |
| 5 | `g_currentObjectKind = OBJ_BUILDING` | `:2487` | yes | never |
| 6 | `AppendRowsImpl` | `:2489` | yes | never |
| 7 | `AppendWidgetsImpl` gates | `:2168-2193` | pass | never reached |
| 8 | Provider loop (4 providers) | `:2264` | iterate | never reached |
| 9 | `StateStorageWidgetProvider` | `:2116` | evaluates | never reached |
| 10 | `widgetCheckbox("Allow private DOs to load here")` | `:2161` | draws | never reached |

**The chain breaks at stage 1.** Nothing downstream is wrong.

### Why "same type, different panel" is possible

`TsmBuildingType()` reads `td+0x360`. The dispatcher does **not** use that field first — it routes on the *window's* kind (`window+0x34`, `W_KIND` at `ui_panels.cpp:558`) and only falls through to `td+0x360` on the `kind == 0` arm. So a grocery store and a warehouse can both report `td+0x360 == 3` and still be drawn by different functions, if their windows carry different kinds. That is exactly what the 2026-10-23 Ghidra findings describe, and it is the only mechanism that explains a checkbox appearing on one type-3 building and not another.

---

## 3. Checkbox instantiation — the exact requirements

`WidgetCheckboxImpl` (`ui_panels.cpp:1383`) returns 0 — silently, no log — unless **all** of these hold:

| # | Condition | Line | Status for the warehouse |
|---|---|---|---|
| 1 | `g_frame.active` | `:1386` | set only at `:2198`, inside the pass → **never set** |
| 2 | `value != NULL`, `label != NULL` | `:1386` | fine (literal label) |
| 3 | `o_Checkbox` resolved | `:1387` | always non-NULL — assigned unconditionally from `RVA_CHECKBOX` at `:3085`, no pattern scan |
| 4 | `WidgetBudgetLeft() > 0` | `:1388` | `rowBudget - rowsUsed` (`:1317`), `rowBudget = min(g_maxRows, TSM_PANEL_MAX_ROWS)` = 14 (`:2208`) |
| 5 | native call does not fault | `:1404-1412` | would log `append faulted` → none present |

Note condition 4: **a warehouse panel whose vanilla row count reaches `max_rows` would starve every widget even after the hook works.** The DO used 10 of 14 (`do: appended 10 row(s)`), leaving room. The warehouse's vanilla row count has never been observed — it is the one number a post-fix log will supply.

### Visibility conditions upstream of that

`StateStorageWidgetProvider` (`private_logistics.cpp:2116`) can suppress the checkbox at six points:

| Gate | Line | Warehouse status |
|---|---|---|
| `g_autoTargetsEnabled` + `g_uiPanels` + `TsmUiPanelsHasWidgets` + pointers | `:2119-2122` | ini `auto_targets_enabled = 1`; the probe fired once, so this passed at least once |
| `AutoReadBuildingType` | `:2124` | logs on failure — none |
| `AutoHasNativeStorage` | `:2142` | **true** — the warehouse reached the non-private branch of the world scan, which uses the same predicate (`:2472`) |
| `isBuildingPrivate` | `:2147` | **false** — it took the state branch; every B2B line logs `priv=0` |
| `type != 5` | `:2152` | explicitly tolerated — logs "drawing anyway (warehouse fix)" |
| `AutoBuildingKey` | `:2157` | would return silently; unverified but the key is logged in the world scan |

So of the eleven conditions in §3 and here, **nine are already known-good for the warehouse** and two (`g_frame.active`, budget) cannot be evaluated because the pass never runs.

---

## 4. Warehouse-specific UI hooks: there are none

This is the gap, not an interaction bug.

- `g_hooks[]` (`:1073-1087` in v6.14) has exactly 8 entries. All are on the dispatcher's `kind == 0` path.
- No plugin contains a name check, a type-5 check, or any `warehouse` special case on the widget path. Before v6.15 every `warehouse` mention in the tree was a **comment** (`private_logistics.cpp:2141, 2151, 2470`; `private_shops.cpp:186, 580, 710`).
- The storage provider is deliberately **not** type-gated (`:2142-2154`), specifically so that a warehouse would get the checkbox. That decision was correct and remains correct — it simply never gets the chance to act.

---

## 5. Ruled out

| Hypothesis | Why it fails |
|---|---|
| `hasNative` false for the warehouse | It is in `g_autoStateStorageSites`, emitted only from the non-private branch of `AutoHasNativeStorage(b) && …` (`:2472`) |
| The warehouse is private | Same branch; `priv=0` on every B2B line |
| Type gate excludes it | The provider tolerates `type != 5` by design (`:2152`); `enterprise_base`'s only type bail is 39 |
| `o_Checkbox` unresolved | Assigned unconditionally at `:3085` |
| Widget budget exhausted | Would require the pass to run; and it would also stop the *other* providers, which is not what is reported |
| `TsmLooksLikeBuilding` rejected it | Zero `widget pass refused` lines, and the probe log is `g_probe`-gated on |
| ESC-probe silence proves the window never opened | The ESC probe reads `g_frame.win`, set only inside the hook-driven pass (`:2197-2209`) — it is blind to unhooked windows |

---

## 6. Fix plan

### 6.1 The fix (already implemented, `ui_panels` v6.15)

Add the missing builder as a 9th hook. Shipped in PR #16 (merge `bb29b8b`) and present on this branch:

- `MAX_HOOKS` 8 → 9, `HOOK_WAREHOUSE` = 8
- call-site hook at `0x6EA5EF` → `0x7FC880`, guarded by a byte-exact check of the `kind==3` cluster at `0x6EA5E3`
- `h_Warehouse` → `RunHookBody(..., 0x7FC880)`, mirroring the proven `h_Do`
- ini `enable` gains `warehouse`
- `DEBUG hook=` now prints `wkind=<window+0x34>`

No `private_logistics` change is needed or wanted: nine of eleven checkbox conditions already hold, so the checkbox follows automatically once the pass runs.

### 6.2 Validation steps

1. `cmd /c build.bat ui_panels`; confirm a fresh DLL timestamp.
2. Confirm `build/plugins/ui_panels.ini` contains `warehouse` in `enable` (`build.bat:115-116` copies it).
3. Open the warehouse panel once, leave it 5 s, plus one shop as a control.
4. Read the log against §7.

### 6.3 Contingency matrix

| Symptom in the post-fix log | Cause | Action |
|---|---|---|
| `hook warehouse: hook 0x6EA5EF -> builder 0x7FC880 ok` + `DEBUG hook=warehouse … wkind=3` + checkbox | fixed | close |
| `hook warehouse off in ini` | stale ini in `build/plugins/` | redeploy the ini |
| `warehouse: guard … does not match` | different game build | re-derive the guard bytes |
| no `warehouse` line at all | window kind ≠ 3 | read `wkind=` off the other hooks, then pick the arm from the Level 1B table |
| fires, `wkind=3`, **no** checkbox, and `widgetCheckbox returned 0` | row budget exhausted | raise `max_rows` or reduce vanilla rows |
| fires, `object=0x0` | `window+0x240` unset for this window class | §8.5 |
| fires, `widget pass refused` | wrong argument convention | §8.1 |

---

## 7. A valid post-fix log must contain

```
hook ok      warehouse  ...  (or "hook warehouse: hook 0x6EA5EF -> builder 0x7FC880 ok")
DEBUG hook=warehouse idx=8 kind=<w+0x248> wkind=3  object=<building ptr, non-zero>
DEBUG hook=shop      idx=0 kind=0 wkind=0     <- control; all pre-existing hooks must read wkind=0
[AUTO DO] storage widget probe: building=<same ptr> type=<n> key=<k> priv=0 nativeStorage=1 willDraw=1
```

then, when the box is ticked:

```
[AUTO DO] storage widget: checkbox toggled key=<k> 0 -> 1
[AUTO DO] state storage permission key=<k> -> load enabled
```

and on the next day close, `planned` should exceed 2 with the warehouse named. If `wkind=3` never appears, every other hook's `wkind=0` still validates the model and narrows the search to the arms in the Level 1B table.

---

## 8. Ghidra investigations still needed

Ordered by how much they block the current fix.

**8.1 — `RDX` at `0x6EA5EF`: window or building? (highest value)**
The whole call-site hook rests on `RunHookBody` receiving a *window* in arg2. Confirm by decompiling the first ~20 instructions of `FUN_1407fc880` and checking whether it dereferences `[RDX + 0x240]` and `[RDX + 0x01]`/`[RDX + 0x250]` the way a window is used. If it instead treats `RDX` as a building, the fix needs a different body — and a building→window back-pointer would be the next thing to find.

**8.2 — is `0x7FC880` reached by any other window kind?**
The findings' Level 1B table shows only `kind == 3` calling it, with a single xref. Confirm the xref list is exhaustive and report which `CMP [RSI+0x34], imm` arms exist in total, so hooking one arm is known to be narrow rather than assumed.

**8.3 — what window kind do the working cases use?**
A state-owned grocery store and a state shop get the checkbox. Report the window kind their panels carry (expected `kind == 0`) and which arm they take. This converts "the warehouse is different" from an inference into a measured contrast, and predicts which other buildings are silently missing widgets.

**8.4 — enumerate the remaining `kind != 0` arms.**
If the warehouse turns out not to be `kind == 3`, the mapping from kind → builder must be complete. The existing table lists ~40 entries; finish it and mark which are storage-capable building classes.

**8.5 — where is `window+0x240` set for kind==3 windows?**
Only needed if the hook fires with `object=0x0`. Report the writer of `+0x240` on that path, and whether it runs before or after the builder call.

**Out of scope / already answered:** `0x6F8CC0` (2-arg preamble, no window — definitively not a hook point), and the inline-hook option on `0x7FC880` (`MOV RAX,RSP` + first clean boundary at 18 bytes).
