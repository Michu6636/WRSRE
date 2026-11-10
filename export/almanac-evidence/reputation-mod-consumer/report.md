# Reputation mod (v2.0) — consumer trace: 0x18002f1e0 case 1 → 0x18001e0a0

**Source:** `C:\WRSRE\Mods for analysis\Diplomacy\tesmioloader/the build/plugins/reputation.dll`
**Method:** Static only. No installer, script, DLL, or game executed; no debugger, hook, or game-file modification. Ghidra used read-only in separate scratch projects.

**Correction acknowledged:** The seven addresses modified by `0x18002f1e0` are Reputation **DLL globals** (in `.data` section, RVAs `0xC3158`, `0xC3780`, `0x286080`, `0x286084`, `0x1CDA18`, `0x1CDA14`, `0x184EB4`, `0x1CDA1C`, `0xC4C50`), not established game globals. `0x180411bcc` is populated by a scan of plugin-state cases, not proven to be the trade-item index.

## 1. Case 1 of `FUN_18002f1e0` — verified instructions

**Disassembly** (`rep_branch_2f1e0.asm`, VA `0x18002f228`–`0x18002f23b`):

```
18002f228:  MOV EAX,R9D              ; R9D = 0 (XOR R9D,R9D at function entry)
18002f22b:  MOV dword ptr [0x180286084],R9D   ; DLL global 0x180286084 = 0
18002f232:  CMP dword ptr [0x180286080],EAX   ; compare DLL global 0x180286080 with 0
18002f238:  SETZ AL                  ; AL = 1 if [0x180286080]==0, else 0
18002f23b:  MOV dword ptr [0x180286080],EAX   ; DLL global 0x180286080 = AL (0 or 1)
```

**Behavior:** Case 1 sets DLL global `0x180286084 = 0`, then toggles DLL global `0x180286080` between 0 and 1 (if 0 → 1; if non-zero → 0). This is a **render-enable flag toggle**, not a trade-item index.

`0x180286080` and `0x180286084` are in the DLL's `.data` section (RVA `0x286080`, `0x286084`; `.data` spans `0xC3000`–`0x6A9BD8`). They are **Reputation DLL globals**.

## 2. Consumer `FUN_18001e0a0` — verified instructions

**Disassembly** (`rep_consumer_1e0a0.asm`, VA `0x18001e0a0`–`0x18001e4c0`, 222 instructions of 3464-byte function).

**Render gate** (lines 9–10):
```
18001e0c3:  CMP dword ptr [0x180286080],0x0
18001e0ca:  JZ 0x18001ee10
```
If `0x180286080 == 0`, the consumer skips all rendering and jumps to `0x18001ee10`. When non-zero (set by case 1), it proceeds to draw content.

**Counter handling** (lines 97–101):
```
18001e243:  MOV EAX,dword ptr [0x180286084]
18001e249:  CMP EAX,EDI              ; EDI = computed item count
18001e24b:  JL 0x18001e255
18001e24d:  XOR EAX,EAX
18001e24f:  MOV dword ptr [0x180286084],EAX   ; reset 0x180286084 to 0 if >= count
```
`0x180286084` is a counter/clip index, reset when it exceeds the drawn count.

**Draw calls** — `0x180022c50` called 3 times with different string pointers (lines 163, 205, 212). Strings resolved via PE scan:

| String VA | Content | Purpose |
|-----------|---------|---------|
| `0x1800a16f0` | `" CONTRACTS      %d running, %d offered      page %d of %d"` | Contracts board header with pagination |
| `0x1800a1730` | `"   Nothing running and nothing offered."` | Empty-state message |
| `0x1800a1758` | `"   The board is redrawn every few weeks."` | Board refresh info |
| `0x1800a1790` | `"   You have no missions built, and no bloc offers contracts to a..."` | No-missions message |

**Game-global checks** (lines 213–218):
```
18001e485:  CMP dword ptr [0x1800c30d0],EBX
18001e48b:  JZ 0x18001ed80
18001e491:  CMP dword ptr [0x1800c4e90],EBX
18001e497:  JNZ 0x18001ed80
18001e49d:  CMP dword ptr [0x1800c4e94],EBX
18001e4a3:  JNZ 0x18001ed80
```
Three game globals (`0x1800c30d0`, `0x1800c4e90`, `0x1800c4e94`) must all be zero for the final draw branch to proceed; otherwise jump to `0x18001ed80` (end/cleanup).

**What it draws:** The consumer renders the mod's own **contracts/missions board panel** — header with running/offered counts and page, empty-state text, board refresh info, no-missions text. These are **Reputation panel strings**, not Economy & Trade row strings.

**How state is cleared:** `0x180286080` is cleared when case 1 runs again (toggle back to 0). `0x180286084` is reset to 0 when it exceeds the drawn count. Other cases (0, 2, 3, 6, 7) clear various DLL globals. No explicit "restore native Economy & Trade panel" instruction exists in this path.

## 3. Caller `FUN_180060e20` — verified instructions

**Disassembly** (`rep_wide_60fe0.asm`, VA `0x180060FE0`–`0x180061240`, containing fn `FUN_180060e20`).

`FUN_180060e20` is a dispatch function that calls sub-functions in sequence:
- `CALL 0x18001cc80` (VA `0x18006107a`)
- `CALL 0x18001e0a0` ← **the consumer** (VA `0x1800610bb`, the only call rel32 targeting it in `.text`)
- `CALL 0x18001fe50` (VA `0x1800610f2`)
- `CALL 0x18001f1b0` (VA `0x180061129`)
- `CALL 0x18001d980` (VA `0x180061160`)
- `CALL 0x180063c80` (VA `0x180061197`)
- `CALL 0x18000e1f0` (VA `0x1800611e8`)
- `CALL 0x18002f060` (VA `0x180061221`)

Each call is followed by `JMP` to the next, forming a linear dispatch chain. `0x18001e0a0` is one panel-rendering step in this chain.

## 4. Does this establish what happens to vanilla Economy & Trade content?

**No.** The traced path establishes:

1. Case 1 toggles **DLL global** `0x180286080` (render flag) and resets `0x180286084` (counter).
2. The consumer `0x18001e0a0` reads `0x180286080`; when set, it draws **Reputation's own contracts/missions board** content (strings about "CONTRACTS", "Nothing running", "board redrawn", "no missions built").
3. The consumer is called by `FUN_180060e20`, a panel-drawing dispatch function.

**What remains unknown — exactly:**
- **No instruction in the traced path suppresses, hides, or replaces native Economy & Trade content.** The consumer only draws its own panel content; it does not remove or overlay the eight Economy & Trade icons.
- **Which native code reads the DLL globals** `0x180286080` / `0x180286084` and how they affect rendering is not established — these are Reputation's own globals, not game globals that native Economy & Trade code is known to consume.
- **How the user returns** to vanilla Economy & Trade content is not established — there is no "restore native panel" or "clear selection" instruction in this path. Case 1 can toggle `0x180286080` back to 0, which makes the consumer skip rendering, but what native code does next is unknown.
- **Whether `0x180411bcc` is the trade-item index** is unproven — it is populated by scanning plugin-state cases (set at `180057617` and `18005783a` when a match is found in a 0–8 loop), not by reading game trade state.

## 5. Evidence files preserved (non-ignored; DLL itself NOT copied)

Folder: `C:\WRSRE\export\almanac-evidence\reputation-mod-consumer/`
- `report.md` — this report
- `rep_consumer_1e0a0.asm` — verified disassembly of `FUN_18001e0a0` (222 instructions, VA `0x18001e0a0`–`0x18001e4c0`)
- `rep_caller_610bb.asm` — verified disassembly of caller context (6 instructions, VA `0x180061060`–`0x180061120`)
- `rep_wide_60fe0.asm` — verified disassembly of caller function `FUN_180060e20` context (36 instructions, VA `0x180060FE0`–`0x180061240`)
- No DLL binary copied (per `*.dll` ignore rule); only hashes and small excerpts preserved

**Bottom line:** Case 1 of `0x18002f1e0` toggles a Reputation DLL global render flag; the consumer `0x18001e0a0` draws Reputation's own contracts board panel when that flag is set. This trace **does not establish** what happens to vanilla Economy & Trade content or how the user returns — no suppression, replacement, or restore instruction exists in the path. The seven globals are DLL globals, not game globals, and `0x180411bcc` is not proven to be the trade-item index. Static analysis alone cannot resolve these gaps; an in-game read-only snapshot would be needed (not authorized to execute).
