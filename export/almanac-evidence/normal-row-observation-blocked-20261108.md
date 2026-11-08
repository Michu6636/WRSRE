# Almanac: normal-row execution observation — blocked (no debugger available)

Read-only observation only. No debugger attached, no breakpoints set, no process
memory writes, no simulated input, no game changes. Live process reacquired;
module base reacquired from the live process (no stale PID or base reused).

## 1. Process and module identity

| Field | Value |
|---|---|
| Timestamp (UTC) | 2026-11-08T23:54:00Z (approx) |
| Target PID | 8140 (`SOVIET64.exe`) |
| Process state | Responding |
| Exe path | `C:\Workers Resources Soviet Republic\SOVIET64.exe` |
| Exe SHA-256 | `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` (matches previous report) |
| Module base (runtime, reacquired) | `0x7FF7E7F90000` (ASLR-relocated, not reused) |
| `game` base (inferred) | `0x7FF7E8964F10` (= base + 0x9d4f10) |

## 2. Live instruction byte verification (read-only)

Read via `ReadProcessMemory` (no writes, no breakpoints).

### Host call site at B+0x31EDEF

| Address | Raw bytes | Interpretation |
|---|---|---|
| `0x7FF7E82AEDEF` (B+0x31EDEF) | `E8 AC 50 05 00 F3 0F 10` | `E8 rel32` = direct `CALL`. First 5 bytes are a call instruction; `F3 0F 10` begins the following instruction. Matches a `CALL` at the expected host dispatch site. |

Note: the static export shows `CALL 0x140373EA0` at preferred VA `0x14031EDEF`. The runtime bytes confirm a `CALL` opcode (`E8`) at the corresponding runtime address. Exact target computation depends on the live image base and was not fully resolved; the opcode presence is confirmed.

### Normal-row exit at B+0x3743E6

| Address | Raw bytes | Interpretation |
|---|---|---|
| `0x7FF7E83043E6` (B+0x3743E6) | `4C 8D A6 F0 C7 00 00 E9` | `4C 8D A6 F0 C7 00 00` = `LEA R12,[RSI + 0xc7f0]`; `E9` = `JMP rel32` opcode. Matches the static export at `0x1403743E6` (normal-row exit block). |

## 3. Blocked: no debugging tools available

A search of the system found **no debugging tools installed**:

- WinDbg / CDB (Windows Kits): not found
- x64dbg / x32dbg: not found
- OllyDbg: not found
- GDB / LLDB: not on PATH
- Python debugging modules: not available for native-process debugging

Per the task instructions: *"if suitable debugging tools are unavailable, stop and report that rather than installing anything."*

Therefore:
- **No breakpoints were set** (neither hardware nor software).
- **No registers were captured** (RSI, EDI, XMM6, XMM7, XMM10 not observed).
- **No process was paused or detached** (the game remains running, unmodified).
- **No instruction bytes were changed** (no software breakpoint `0xCC` insertion).

## 4. What the observation would have captured (if a debugger were available)

Planned observation points:

| Site | Purpose | Registers/state to record |
|---|---|---|
| `B+0x31EDEF` | Host call to `FUN_140373EA0` (button-row handler) | Verify the call is reached; confirm `RCX` = game pointer |
| `B+0x3743E6` | Normal-row exit (after all drawn tiles, before post-loop) | `RSI` (game ptr), `EDI` (drawn_count), `XMM6` (next-slot X), `XMM7` (pitch), `XMM10` (row Y), `game+0xC65C` (selected index), `game+0x578`, `game+0x5E0` |

Distinction to record:
- **Visited entries**: loop iterations where `EDI` advanced (all `count` iterations, including skipped ones).
- **Drawn entries**: iterations that passed the skip checks and executed the draw/collision at `0x1403742a4`/`0x1403743a2` (count minus skipped entries).

## 5. Limitations

- No debugger available; breakpoint-based observation cannot proceed.
- Read-only instruction-byte verification confirms the expected instruction patterns at both sites but does not prove execution reached them.
- Register values (RSI, EDI, XMM6, XMM7, XMM10) and per-frame field values were not observed.
- Whether the normal row actually executed in any given frame remains unverified.

## 6. Artifacts

- `export/almanac-evidence/geometry-snapshot-20261108.md` — prior read-only geometry snapshot (no debugging)
- `export/almanac-evidence/geometry-snapshot-20261108-raw/` — raw read output from prior snapshot

No commit or push was performed for this step (per the no-implementation constraint), beyond the observation report itself. No patches left behind; the game process is unmodified and continues running.
