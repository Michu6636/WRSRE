# Sidebar write trace — Almanac evidence

**Source:** SOVIET64.exe disassembly, `C:\WRSRE\scratch\out/`
**Method:** Static only. No game execution, no debugger, no hooks, no writes. All evidence from Ghidra disassembly artifacts in existing scratch projects.

## 1. Two distinct write sites

The game writes `game+0xC650` (selected parent panel pointer) and `game+0xC65C` (selected row-item index) in **two separate functions**, called from the master dispatch `FUN_140317DB0`. They are **not** written by the same code path.

### 1a. `game+0xC650` — written by `FUN_14007C400` (section-switch handler)

**Call site:** `FUN_140317DB0` (master dispatch) calls `FUN_14007C400` at `0x14031BB22` (`CALL 0x14007c400`).
**Base register tracking:** `R14 = RCX` at `14007c47e` (`MOV R14,RCX`), so RCX on entry is the game object pointer. All `[R14 + 0x...]` accesses are game-object-relative.

**Write instruction** (verified, `1400843f3`):
```
1400843ef:  49 89 7D 00              MOV qword ptr [R13],RDI
1400843f3:  49 89 BE 50 C6 00 00     MOV qword ptr [R14 + 0xc650],RDI   ; game+0xC650 = RDI (panel pointer)
1400843fa:  41 C7 86 54 AE 00 00 00 00 7A C4 MOV dword ptr [R14 + 0xae54],0xc47a0000
```

**Context:** This write is reached when the current panel name is neither "trade" nor "settings" (strcmp comparisons at `1400843b0` vs `0x14087c048`/`"trade"` and `1400843e6` vs `0x14087bfd8`/`"settings"` both fail). RDI is the candidate panel pointer selected by iterating the panel array (loop `140084470`–`14008459b`) searching `R14+0xd400`..`R14+0x13fe0` for a matching entry pointer.

**Additional writes by `FUN_14007C400`:**
- `14008466e`: `MOV qword ptr [R14 + 0xc7a8],RAX` (game+0xC7A8 = panel pointer)
- `140084672`: `MOV qword ptr [R14 + 0xc7a0],RAX` (game+0xC7A0 = panel pointer)
- `140084680`–`683`: `MOV byte ptr [R14 + 0xc7b0],AL` (game+0xC7B0 = flag byte)
- `14008468a`–`691`: `MOV dword ptr [R14 + 0xc7b4],EAX` (game+0xC7B4 = dirty counter from game+0xC8F0)
- `14007c55e`–`68d`: conditional writes to `game+0xC8E8` and `game+0xC8E0` (clearing to 0)

**`FUN_14007C400` does NOT write `game+0xC65C`.**

### 1b. `game+0xC65C` — written by `FUN_140373EA0` (trade row drawer)

**Call site:** `FUN_140317DB0` calls `FUN_140373EA0` at `0x14031EDEF` (when `game+0x10398 != 0`, line 5760–5762) or `FUN_140361BB0` at `0x14031EE13` (when selector == 0).
**Base register tracking:** RSI = game object pointer (the trade panel's parent object), set up before the row loop.

**Write instruction** (verified, `140374356`):
```
140374347:  39 9E 5C C6 00 00        CMP dword ptr [RSI + 0xc65c],EBX   ; compare current vs new index
14037434d:  74 07                    JZ 0x140374356                       ; skip if unchanged
14037434f:  C6 86 9A 03 01 00 00     MOV byte ptr [RSI + 0x1039a],0x0     ; clear dirty flag if changed
140374356:  89 9E 5C C6 00 00        MOV dword ptr [RSI + 0xc65c],EBX   ; game+0xC65C = EBX (item index)
14037435c:  C7 86 E8 0B 01 00 FF FF FF FF MOV dword ptr [RSI + 0x10be8],0xffffffff  ; reset dirty counter
```

**Context:** Reached after mouse collision (`CALL qword ptr [0x14086c850]` at `1403742a4`), latch check (`CMP byte ptr [0x140a54e91],CL` at `1403742f2`), and live-mode check (`CMP byte ptr [RSI + 0x14],CL` at `1403742fa`). EBX is the mapped sub-tab index (non-linear: screen pos 0→index 0, 1→1, 2→count-1, 3→2, etc.).

**`FUN_140373EA0` does NOT write `game+0xC650`.**

## 2. Master dispatch — how handlers are chosen

**`FUN_140317DB0`** compares the section name (in RBX) against known strings via byte-by-byte compare:

| Section name compared | String address | Length | Handler called | Call site RVA | Gate |
|---|---|---|---|---|---|
| "trade" | `0x1408b2194` | 6 chars | `FUN_140373EA0` (trade row drawer) | `0x14031EDEF` | `game+0x10398 != 0` |
| "trade" (selector==0) | — | — | `FUN_140361BB0` | `0x14031EE13` | `game+0x10398 == 0` |
| section "…" (5 chars) | `0x1408b218c` | 5 chars | `FUN_14034D590` | `0x14031EE46` | — |
| "…" (6 chars) | `0x1408b2184` | 6 chars | `FUN_140358A20` (importexport fallback) | `0x14031EE76` | — |
| "importexport" (strcmp) | `0x1408b21c8` | — | `FUN_1403472A0` | `0x14031EE9F` | `game+0x10398 != 0` |
| "importexport" (selector==0) | — | — | `FUN_14034B470` | `0x14031EEA9` | `game+0x10398 == 0` |

**Section-switch handler call** (always, before content dispatch):
- `0x14031BB22`: `CALL 0x14007c400` → `FUN_14007C400` writes `game+0xC650`

**Dispatch flow** (verified instructions):
```
14031bb1f:  49 8B CE                 MOV RCX,R14              ; game object
14031bb22:  E8 D9 08 D6 FF           CALL 0x14007c400         ; section switch: writes game+0xC650
14031bb27:  41 80 BE BA 0F 00 00 00  CMP byte ptr [R14 + 0xfba],0x0
...
14031edc0:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]   ; section name char
14031edc7:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1] ; compare with "trade"
14031edcb:  75 50                    JNZ 0x14031ee1d             ; not "trade", check next
14031edcd:  48 83 F9 06              CMP RCX,0x6                 ; 6-char match
14031edd1:  75 ED                    JNZ 0x14031edc0
14031edd3:  44 89 7D 48              MOV dword ptr [RBP + 0x48],R15D
14031eddb:  4C 8D 45 F0              LEA R8,[RBP + -0x10]
14031eddf:  48 8D 55 48              LEA RDX,[RBP + 0x48]
14031ede3:  49 8B CE                 MOV RCX,R14
14031ede6:  45 38 BE 98 03 01 00     CMP byte ptr [R14 + 0x10398],R15B   ; handler selector gate
14031eded:  74 21                    JZ 0x14031ee10               ; selector==0 -> FUN_140361BB0
14031edef:  E8 AC 50 05 00           CALL 0x140373ea0             ; trade row drawer: writes game+0xC65C
```

## 3. Parent-section switching vs row-item selection — key distinction

| Aspect | Parent-section switch | Row-item selection |
|---|---|---|
| Function | `FUN_14007C400` | `FUN_140373EA0` |
| Writes | `game+0xC650` (panel pointer) | `game+0xC65C` (item index) |
| Also writes | `game+0xC7A0/A8/B0/B4`, `game+0xC8E8/E0` | `game+0x10BE8`, `game+0x1039A` |
| Trigger | Section name change (sidebar section button) | Row icon click within trade panel |
| Base register | R14 = game object | RSI = game object (trade panel parent) |
| Write address | `0x1400843f3` | `0x140374356` |

These are **separate code paths**. Switching sidebar sections does NOT write `game+0xC65C`; clicking a row icon does NOT write `game+0xC650`.

## 4. Return/default item selection

**Default when no panel selected** (`FUN_14007C400`, lines 6755–6756):
```
1400842fc:  33 FF                    XOR EDI,EDI            ; EDI = 0 (default: first panel)
1400842fe:  49 89 BE E8 C8 00 00     MOV qword ptr [R14 + 0xc8e8],RDI
```
When `game+0xC650 == 0` (no panel selected), EDI is zeroed and `game+0xC8E8 = 0`. Then at `0x1400843f3`, RDI (0 or a found panel pointer) is written to `game+0xC650`.

**Selection-change detection** (`FUN_140373EA0`): Only writes `game+0xC65C` when `CMP [RSI+0xc65c], EBX` shows the index actually changed (line 6747d `JZ` skips if unchanged). Also clears dirty flag `game+0x1039A` only on change.

## 5. What is NOT established (static limits)

- **Runtime handler selection:** The dispatch gate `game+0x10398` is a static comparison; whether it is non-zero at runtime was not confirmed in this static pass (live snapshot showed selector = 1, but that's a separate observation).
- **`FUN_140361BB0` behavior:** Called when `game+0x10398 == 0` for the trade section; not disassembled here — unknown whether it writes `game+0xC65C` or `game+0xC650`.
- **`FUN_140358A20` / `FUN_1403472A0` / `FUN_14034B470` fallback handlers:** Called for "importexport"/other sections; their writes to `game+0xC650`/`0xC65C` not traced.
- **Panel name strings:** The compared strings at `0x1408b2194` ("trade"), `0x1408b218c`, `0x1408b2184`, `0x14087c048`, `0x14087bfd8` were not resolved from the binary in this pass.
- **No proof that skipping `FUN_14035F3E0` is safe:** Not relevant to this trace (that's the content sub-view, separate from selection writes).

## 6. Evidence files preserved (non-ignored)

Folder: `C:\WRSRE\export\almanac-evidence\sidebar-write-trace-20261109/`
- `report.md` — this report
- `excerpt_panel_write.asm` — `FUN_14007C400` write to game+0xC650 (VA `0x1400843f3`)
- `excerpt_panel_write_tail.asm` — `FUN_14007C400` epilogue writes to game+0xC7A0/A8/B0/B4
- `excerpt_item_write.asm` — `FUN_140373EA0` write to game+0xC65C (VA `0x140374356`)
- `excerpt_dispatch_call.asm` — master dispatch call to `FUN_14007C400` at `0x14031BB22`
- `excerpt_trade_dispatch.asm` — trade section dispatch (name compare + gate + handler call)
- `excerpt_importexport_dispatch.asm` — importexport fallback dispatch

**Bottom line:** Parent-section switching writes `game+0xC650` via `FUN_14007C400`; row-item selection writes `game+0xC65C` via `FUN_140373EA0`. They are separate functions called from different points in the master dispatch `FUN_140317DB0`. The Almanac cannot assume a single write site controls both the panel pointer and item index.
