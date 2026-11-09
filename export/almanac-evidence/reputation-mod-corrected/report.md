# Reputation mod (v2.0) — corrected sidebar_item trace

**Source:** `C:\WRSRE\Mods for analysis\Diplomacy\tesmioloader/the build/plugins/reputation.dll`
**Method:** Static only. No installer, script, DLL, or game executed; no debugger, hook, or game-file modification. Ghidra used read-only in separate scratch projects.

**Correction from prior report:** The functions `0x180068a20`, `0x180068a80`, `0x180068ae0`, `0x180068b40`, and `0x1800303e0` are **not** part of the sidebar_item UI trace. `0x180068ba0` calls `UnhookWindowsHookEx` and is not established as a panel-close handler. This report traces from the independently identified selection check at `0x180058310` instead.

## 1. PE summary (reputation.dll)

- PE32+ x64 DLL, image base `0x180000000`, timestamp `0x6ab18b03`
- Entry point rva `0x6a4b0` (VA `0x18006a4b0`)
- Exports: `TsmPluginApiVersion`, `TsmPluginInit` (VA `0x180068be0`), `TsmPluginStart` (VA `0x180068bf0`)
- Imports: `USER32.dll`, `KERNEL32.dll` only. **No SOVIET64.exe imports** — calls game code at fixed absolute addresses via TesmioLoader API pointer table at `[0x1800c4d70]`
- 6 sections: `.text` `.rdata` `.data` `.pdata` `.fptable` `.reloc`

Full PE dump: `reputation_peinfo.txt` (in `reputation-mod/` folder, not overwritten).

## 2. Selection check: `FUN_180058310` (VA `0x180058310`–`0x180058510`, 106 instructions)

**Verified instructions** (`rep_selection_check_58310.asm`):

| VA | Instruction | Meaning |
|----|-------------|---------|
| `180058316` | `CMP dword ptr [0x1800c3c5c],0x0` / `JL 0x180058398` | If DLL global `0x1800c3c5c` < 0 → return 0 (disabled flag) |
| `18005831f` | `CMP dword ptr [0x180287b48],0x0` / `JZ 0x180058398` | If DLL global `0x180287b48` == 0 → return 0 (second gate) |
| `180058328`–`180058342` | `MOV RBX,[0x1800c4d80]` (exe base); `CALL [RAX+0x60]` with EDX=8, RCX=`RBX+0x9e1560` | TesmioLoader API ID 8: get pointer at `exe+0x9e1560` (sidebar record) |
| `180058345` | `TEST EAX,EAX` / `JZ 0x180058398` | If null → return 0 |
| `180058349`–`180058364` | Load `exe+0x9e1560`, INC RCX, `CALL [RAX+0x60]` with EDX=0x17 | API ID 0x17 (23): string compare at `exe+0x9e1560+1` |
| `180058380`–`180058396` | Loop: compare bytes `[RDX+RAX+1]` vs `[R8+RAX]` up to 0x17 (23) chars; `R8 = 0x1800c3c28` | Compare sidebar record name vs DLL-global configured name at `0x1800c3c28` |
| `18005838b` | `TEST CL,CL` / `JZ 0x1800583a0` | Null terminator found → name matches |
| `1800583a0`–`1800583ba` | `MOV EDX,0x4`; `CALL [RAX+0x60]` with RCX=`RBX+0x9e156c` | API ID 4: get DWORD at `exe+0x9e156c` (selection index field) |
| `1800583c1`–`1800583cf` | `CMP dword ptr [RBX+0x9e156c], ECX` where ECX=`[0x1800c3c5c]`; `SETZ AL` | Return 1 if selection index matches configured value |

**Purpose:** Returns 1 if the reputation sidebar item is currently selected (name matches configured name AND selection index matches configured index), 0 otherwise. Fully gated on two DLL globals (`0x1800c3c5c`, `0x180287b48`).

**String at `0x1800c3c28`:** non-ASCII data in `.data` section — the configured item name buffer compared byte-by-byte against the sidebar record name (which has a 1-byte prefix before the string, per the `+1` offset in the compare loop).

## 3. Caller: `FUN_180057230` (VA `0x180057230`–`0x180057980`, 403 instructions)

**Verified instructions** (`rep_caller_57230.asm`). This is a per-frame/tick handler. Relevant path:

### Early setup (lines 1–63)
- `CMP [0x180280d80],0x0` / `JZ 0x180057911`; `CMP [0x180280d84],0x0` / `JZ 0x180057911` — two enable flags; if either 0, zero out state globals and return
- API ID 8: get pointer at `exe+0x9e1560` (sidebar record). If null or unchanged from cached `[0x1804de978]`, branch to alternate handlers
- Builds a debug string using format strings `0x18009eb78` ("sd in the ini"), `0x18009eb98` ("exe+0x%-6X %d records at %p, stride 0x%X"), `0x18009eba8` ("cords at %p, stride 0x%X") via `CALL 0x180001cd0`
- Outputs via `CALL 0x180053ef0` with format `0x18008df1c` ("that moved:") at line 115
- Calls `0x180057930` at line 119

### Middle section (lines 128–209)
- `CALL 0x1800592c0` at line 128
- Checks globals `[0x180411ba4]`, `[0x180411ba8]`, `[0x1800c3c68]` (lines 129–134)
- `CALL 0x180057d50` at line 135
- Modulo arithmetic on `[0x1804de974]` with `[0x1800c3c70]` (lines 141–144)
- If remainder 0: reads panel-rect floats from `exe+0x9d4f08`+`index`, outputs via `CALL 0x180053ef0` (lines 145–175)
- `MOV RCX,[0x1800c31e0]; CALL [0x18008d068]` at lines 180–183

### Selection-change detection (lines 210–233) — KEY PATH
```
18005761e: CALL 0x180057e40          ; some helper function
180057623: CALL 0x180058310          ; selection check (is reputation item selected?)
180057628: CMP EAX,dword ptr [0x1804de980]   ; compare with cached selection state
18005762e: JZ 0x180057697            ; if unchanged, skip
180057630: MOV dword ptr [0x1804de980],EAX   ; cache new state
180057636: TEST EAX,EAX              ; if NOT selected (EAX==0)
180057638: JZ 0x180057697            ; skip panel-open logic
```
**When selection changes AND reputation item IS selected:**
```
18005763a–180057668: CMP seven globals ([0x1800c3158], [0x180286080], [0x1801cda18], [0x1801cda14], [0x180184eb4], [0x1801cda1c], [0x1800c4c50]) against 0; JNZ 0x18005767d if any nonzero
180057672: MOV ECX,dword ptr [0x180411bcc]   ; ECX = selected item index
180057678: CALL 0x18002f1e0                  ; state-machine handler
```
If any of the seven globals is nonzero, jumps to `0x18005767d` (logging path, skips `FUN_18002f1e0`).

### Panel open/close state change (lines 267–306)
```
1800576fa: CMP EDI,dword ptr [0x180411bc8]   ; EDI = panel open state; compare with cached
180057700: JZ 0x180057778                    ; if unchanged, skip
180057702: MOV dword ptr [0x180411bc8],EDI   ; cache new state
180057708: TEST EDI,EDX / JNZ 0x180057799    ; if EDI != 0 (panel open)
...
180057799: CALL 0x180001c80
1800577a6–1800577b0: if [0x1800c3c74]==0 and EAX==0: MOV ECX,[0x180411bcc]; CALL 0x18002f1e0
```
**When panel state changes AND panel is open:** calls `FUN_18002f1e0` with ECX = item index from `[0x180411bcc]`, then resets seven globals to 0 and sets `[0x1800c3170]=1`.

## 4. State-machine handler: `FUN_18002f1e0` (VA `0x18002f1e0`–`0x18002f420`, 111 instructions)

**Verified instructions** (`rep_branch_2f1e0.asm`). Jump table on ECX (0–7) via table at `0x18002f3f8`:

| ECX | VA | Action |
|-----|-----|--------|
| 0 | `0x18002f203` | `[0x1800c3170]=1`; if `[0x1800c3158]!=0`: `[0x1800c3158]=2` |
| 1 | `0x18002f228` | `[0x180286084]=0`; `[0x180286080]=0` |
| 2 | `0x18002f246` | If `[0x1800c33a8]==0`: `[0x1800c3170]=1`; `[0x1801cda18]=0` |
| 3 | `0x18002f274` | If `[0x1800c33a4]==0`: `[0x1800c3170]=1`; `[0x1801cda14]=0` |
| 4 | `0x18002f2a2` | If `[0x1800c89b0]==0`: toggle `[0x180184eb4]` (0→1, 1→0) |
| 5 | `0x18002f2c7` | If `[0x1800c89b0]==0`: `[0x180184eb4]=2` or `0` |
| 6 | `0x18002f2ee` | If `[0x1800c33ac]==0`: `[0x1800c3170]=1`; `[0x1801cda1c]=0` |
| 7 | `0x18002f315` | `[0x1800c4c50]=0` |

**Shared tail** (`0x18002f327`–`0x18002f3f4`): Additional global resets based on ECX value and `[0x1800c3780]`. Clears `[0x180286080]`, `[0x180286084]`, `[0x1801cda18]`, `[0x1801cda14]`, `[0x180184eb4]`, `[0x1801cda1c]`, `[0x1800c4c50]` in various combinations.

**Purpose:** State-machine that modifies seven game globals to reflect sidebar item selection/panel state. Does **not** draw UI, create buttons, or load textures directly. Called with ECX = item index when reputation item becomes selected or when panel open state changes.

## 5. How selection opens mod content / affects vanilla content

**Selection → open flow (verified):**
1. `FUN_180057230` runs each frame/tick
2. When reputation item selection changes and becomes selected: `CALL 0x180058310` returns 1
3. Seven globals all zero → `CALL 0x18002f1e0` with ECX = item index
4. `FUN_18002f1e0` modifies game-state globals (case 0 sets `[0x1800c3158]=2`, `[0x1800c3170]=1`; other cases clear various globals)
5. Panel open state (EDI) changes → `FUN_18002f1e0` called again, globals reset

**Vanilla Economy & Trade content:** The disassembly shows **no instruction that suppresses, hides, or replaces native Economy & Trade content**. The mod only writes to game globals and lets native rendering proceed. The seven globals modified by `FUN_18002f1e0` appear to be state flags that the native game reads — but which native code reads them and how it affects rendering is **not traced** in this DLL.

**Closing / returning control:** When panel state changes to closed (EDI becomes 0), `FUN_18002f1e0` is called with ECX = item index, which clears globals (cases 2, 3, 6, 7 clear `[0x1801cda18]`, `[0x1801cda14]`, `[0x1801cda1c]`, `[0x1800c4c50]`). No explicit "restore native panel" instruction is visible.

## 6. Comparison with WRSRE trade handlers

**WRSRE evidence** (`docs/almanac-investigation.md`, `docs/ghidra-index/373EA0.md`, `docs/almanac-export-review.md`):

| Aspect | WRSRE | Reputation mod |
|--------|-------|----------------|
| Row drawer | `FUN_140373EA0` reads `game+0xC650`, walks sub-entry vector (stride 0x50), draws texture at `sub_entry+0x48`, tracks selected sub-tab in `game+0xC65C` | Uses `exe+0x9e1560` (sidebar record ptr), `exe+0x9e156c` (selection index), `exe+0x9e1518` (record array, stride 0x68) |
| Icon offset | `sub_entry+0x48` (texture) | Sidebar record name compared at `exe+0x9e1560+1` (offset +1 from record ptr) |
| Handler count | **Two** trade handlers selected by game-state flag; `FUN_140358A20` (importexport) effectively dead | One state-machine `FUN_18002f1e0` |
| Drawing | `FUN_140373EA0` directly draws row buttons | `FUN_18002f1e0` only modifies globals; native renderer draws |

**Critical distinction:** WRSRE's `FUN_140373EA0` directly draws the Economy & Trade row. Reputation's `FUN_18002f1e0` does **not** draw anything — it modifies globals that native code presumably reads. The Almanac cannot assume the same drawing pattern applies.

**Handler ambiguity NOT resolved:** The user instructed not to declare either WRSRE handler dead without control-flow evidence. `FUN_140358A20` is described as "effectively unreachable because no panel is named importexport" in the docs — that is documentation-level reasoning, not control-flow evidence from this DLL. Reputation's single-state-machine approach does not inform which WRSRE handler is active.

## 7. Strings and imported functions resolved

| Address | Type | Content | Purpose in trace |
|---------|------|---------|------------------|
| `0x1800c3c28` | DLL global (`.data`) | non-ASCII (name buffer) | Compared byte-by-byte vs sidebar record name in `FUN_180058310` |
| `0x1800c3c5c` | DLL global (`.data`) | DWORD flag | Enable gate + configured selection index in `FUN_180058310` |
| `0x18009eb98` | String (`.rdata`) | `"exe+0x%-6X %d records at %p, stride 0x%X"` | Debug format string in `FUN_180057230` |
| `0x18008df1c` | String (`.rdata`) | `"that moved:"` | Log prefix passed to `0x180053ef0` |
| `0x18009f1a0` | String (`.rdata`) | `".2f  %016llX"` | Float/pointer format in `FUN_180057230` |
| `0x18009f2c4` | String (`.rdata`) | `".1f  costs %5.1f  hold %6.2f  %s"` | Format string at end of `FUN_180057230` |
| `[0x1800c4d70]` | Import table | TesmioLoader API pointer | `[RAX+0x60]` = function pointer table; IDs 4, 8, 0x17, 0x18, 0x60 used |
| `0x180001cd0` | Import (KERNEL32/USER32?) | Function at DLL VA `0x180001cd0` | String formatting (called with format strings) |
| `0x180053ef0` | Function at DLL VA `0x180053ef0` | Output/log function | Called with formatted strings |
| `0x18008d068` | Import | IAT slot | Called via `CALL qword ptr [0x18008d068]` at `1800575ab` |

## 8. Verified vs configuration claims vs unknowns

### Verified (instructions)
- `FUN_180058310` checks if reputation sidebar item is selected (name + index match)
- `FUN_180057230` calls `FUN_180058310` when selection changes, then calls `FUN_18002f1e0` if selected and seven globals are zero
- `FUN_18002f1e0` is a state machine modifying seven game globals based on ECX (0–7)
- No instruction in the traced path suppresses, hides, or replaces native Economy & Trade content
- Panel open/close state changes trigger `FUN_18002f1e0` calls

### Configuration claims (reputation.ini, not verified in binary)
- `sidebar_item = 1` would push an item into the `trade` panel vector
- Game draws/highlights the item; clicking opens the mod window
- Icon `trade_reputation.png` 96x96, loaded without null check (crash risk)
- These claims are **not** supported by the traced disassembly — the traced path modifies globals, not vectors

### Unknowns (require in-game testing, not authorized)
- Which native code reads the seven globals modified by `FUN_18002f1e0`, and how it affects Economy & Trade rendering
- Whether the reputation item actually appears in the Economy & Trade row at runtime
- Whether both WRSRE trade handlers can display/select the item, or only `FUN_140373EA0`
- Exact panel width/height/clip write location; hit-test bounds vs visual clipping
- Whether native panel state is fully restored on close
- Read-only in-game snapshot of trade row geometry and selection state

## 9. Risks for a safe WRSRE design

- Reputation's state-machine approach (globals → native renderer) differs from WRSRE's direct-draw approach (`FUN_140373EA0`). The Almanac cannot port Reputation's mechanism without knowing which native code consumes the globals.
- WRSRE's two trade handlers mean a same-row button must work under whichever handler is active. Reputation's single-state-machine does not resolve this.
- No cleanup path verified for global state on mod unload — dangling globals could affect native rendering.

## 10. Evidence files preserved (non-ignored; DLL itself NOT copied)

Folder: `C:\WRSRE\export\almanac-evidence\reputation-mod-corrected/`
- `report.md` — this report
- `rep_selection_check_58310.asm` — verified disassembly of `FUN_180058310` (selection check, 106 instructions)
- `rep_caller_57230.asm` — verified disassembly of `FUN_180057230` caller region (403 instructions)
- `rep_branch_2f1e0.asm` — verified disassembly of `FUN_18002f1e0` state machine (111 instructions)
- No DLL binary copied (per `*.dll` ignore rule); only hashes and small excerpts preserved

**Bottom line:** The traced path shows Reputation's `sidebar_item` mechanism is a state machine that modifies seven game globals when the reputation sidebar item is selected — it does not directly draw or insert into the Economy & Trade row. This differs from WRSRE's `FUN_140373EA0` which directly draws the row. The Almanac cannot assume the same-row button approach works without knowing which native code reads Reputation's globals, and must verify against WRSRE's actual trade handler(s) via a read-only in-game snapshot (not authorized to execute).
