# Almanac geometry snapshot (read-only) — Economy & Trade row

Read-only observation only. No debugger, no hooks, no process memory writes, no
simulated input, no game changes. All addresses are runtime (ASLR-relocated)
unless noted as preferred-image VA. Process/PID reacquired live; no stale PID or
base reused.

## 1. Process and module identity

| Field | Value |
|---|---|
| Timestamp (UTC) | 2026-11-08T20:47:45Z |
| Target PID | 8140 (`SOVIET64.exe`) |
| Process start time | 2026-11-08 20:40:40 |
| Exe path | `C:\Workers Resources Soviet Republic\SOVIET64.exe` |
| Exe size | 10,308,608 bytes |
| Exe SHA-256 | `296644a9f207d609031fc2ae73fed2dcb34619a1d55a35d1c7b51965ce6841b8` |
| Previously reported hash | `296644a9f...6841b8` |
| Hash matches previous report | Yes |
| Module base (runtime, reacquired) | `0x7FF7E7F90000` |
| Process handle | opened `PROCESS_VM_READ \| PROCESS_QUERY_INFORMATION` (read-only) |

The module base was reacquired from the live process (`MainModule.BaseAddress`);
no previously recorded base was reused.

## 2. Host call-site verification (previously checked bytes)

Read at runtime base + offset; both are `E8 rel32` direct calls matching the
static disassembly (`FUN_140317DB0` dispatch at `0x14031edef`/`0x14031ee13`).

| Site | Runtime VA | Raw bytes | Decoded target | Matches static export |
|---|---|---|---|---|
| `base + 0x31EDEF` | `0x7FF7E82A EDEF` | `E8 AC 50 05 00` | `CALL 0x140373EA0` (button-row handler) | Yes |
| `base + 0x31EE13` | `0x7FF7E82B EE13` | `E8 98 2D 04 00` | `CALL 0x140361BB0` (alternative handler) | Yes |

## 3. `game` object base

`game` is passed as `RCX` to `FUN_140317DB0` by its sole caller
(`0x140482b52` in `FUN_140482790`, `LEA RCX,[0x1409d4f10]` at `0x140482b4b`),
so `game = module_base + 0x9d4f10`.

| Field | Value |
|---|---|
| Computed `game` base | `0x7FF7E8964F10` (= base + 0x9d4f10) |
| Validation | All `game+...` reads below returned sensible, self-consistent values |

Reads were performed at `[game + offset]` via `ReadProcessMemory`; each read's
success is recorded separately from its value (see §4).

## 4. Revalidated fields (read success recorded separately from zero)

| Offset | Raw bytes | Decoded | Read OK? | Meaning |
|---|---|---|---|---|
| `game+0x10398` | `01` | `1` (byte) | Yes | Handler selector: non-zero → `FUN_140373EA0` (button row) |
| `game+0xC7D4` | `FF FF FF FF` | `-1` (i32) | Yes | Normal row path active (`!= -1` would take alternate) |
| `game+0x578` | `00 00 00 00` | `0` | Yes | No skip of mapped indices 3 and 4 |
| `game+0x5E0` | `01 00 00 00` | `1` | Yes | No skip of mapped index 6 |
| `game+0xC65C` | `00 00 00 00` | `0` | Yes | Selected sub-entry index = 0 |
| `game+0xAE50` | `00 00 38 43` | `184.0` (f32) | Yes | Y-base offset |
| `game+0xAE54` | `00 40 44 44` | `785.0` (f32) | Yes | Panel X/width branch input |

All seven field reads returned `READ_OK`. None failed. Values are consistent with
the snapshot context (selector=1, normal row, eight entries, no skips).

### Trade subvector count

`game+0xC650` → object pointer `P = 0x000001D8103C6550` (heap).
`[P + 0x48]` = subvector begin = `0x000001D802C77000`
`[P + 0x50]` = subvector end   = `0x000001D802C77280`
Span = `0x280` = 640 bytes; stride = `0x50` = 80 bytes.

| Count | Value | Read OK? |
|---|---|---|
| Subvector count (span / 0x50) | `8` | Yes |

This independently revalidates the eight visible tiles (count = 8).

## 5. Float constants at module-base offsets

| Offset | Raw bytes | Decoded (f32) | Read OK? | Role (per handler `FUN_140373EA0`) |
|---|---|---|---|---|
| `base+0x992088` | `CD CC 4C 3F` | `0.8` | Yes | DPI/scale |
| `base+0x90AA0C` | `00 00 34 42` | `45` | Yes | X-origin component A |
| `base+0x90AB14` | `00 00 AA 42` | `85` | Yes | X-origin component B |
| `base+0x90AAA0` | `00 00 80 42` | `64` | Yes | Pitch constant (= backdrop size) |
| `base+0x90A8CC` | `00 00 70 41` | `15` | Yes | Y-base constant |
| `base+0x90ADC4` | `00 00 80 43` | `256` | Yes | (reported panel height constant) |
| `base+0x90A9CC` | `00 00 0C 42` | `35` | Yes | Post-loop Y step constant |

All seven float reads returned `READ_OK`.

## 6. Window bounds (read-only Windows API)

| Field | Value |
|---|---|
| Main window handle | `0x00020934` |
| Visible | Yes |
| Title | `Workers & Resources: Soviet Republic` |
| WindowRect | L=0 T=0 R=1536 B=864 → W=1536 H=864 |
| ClientRect | L=0 T=0 R=1536 B=864 → W=1536 H=864 |

WindowRect == ClientRect (borderless/fullscreen). Read via `GetWindowRect` /
`GetClientRect` on the live process's main window handle; no window state
changed.

## 7. Hypothetical ninth rectangle (labeled assumptions)

Computed in the handler's scaled coordinate units (scale = 0.8 already applied).
Assumptions: handler is `FUN_140373EA0` (confirmed, selector=1), normal row
(confirmed, `game+0xC7D4 == -1`), no skips (confirmed, `0x578 == 0`, `0x5E0 == 1`),
eight native tiles drawn (confirmed, count = 8).

| Quantity | Formula | Value |
|---|---|---|
| X origin | scale × (45 + 85) | 104.0 |
| Pitch | scale × 64 | 51.2 |
| Tile width / height | scale × 64 | 51.2 |
| Y base | scale × 15 + 184 | 196.0 |
| Ninth tile X | 104.0 + 8 × 51.2 | 513.6 |
| Ninth tile rect X range | [513.6, 564.8] | — |
| Ninth tile rect Y range | [196.0, 247.2] | — |

This X result ([513.6, 564.8]) matches the earlier conditional estimate in the
merged review and is therefore reproduced here from live reads, not re-derived
guesses.

**This is a hypothetical rectangle only.** It is NOT established that it is
visible, clickable, unclipped, or even reached. Reasons:
- The tile X origin (104.0) and the host's panel X position
  `(game+0xAE54 − 512 or 600) × scale` are apparently misaligned (panel X
  ≈ 218.4 with `game+0xAE54 = 785`), so the relationship between tile
  coordinates and panel coordinates is not established.
- Panel nominal width is 1024×scale or 1200×scale (819.2 or 960) and height
  256×scale (204.8), but the exact branch and constants are unverified.
- Clipping (if any) is performed inside `C3D_PANEL2D::Draw` /
  `C3D_PANEL2D::Collision`, reached through indirect call slots
  `[0x14086c850]` / `[0x14086c860]` whose DLL bodies are not available. No
  explicit bounds check exists in the examined executable code.

## 8. Limitations

- Snapshot is non-atomic: fields were read at separate instants; the handler may
  not have executed between reads, so values are a point-in-time sample, not a
  single consistent frame.
- `game` base (`base + 0x9d4f10`) is inferred from the host caller's `LEA RCX`;
  it is validated empirically by the sensible field reads above, but remains an
  inference from one call path.
- `game+0xC65C = 0` means sub-entry 0 is selected; this does not prove a click
  occurred, only the current selection state.
- Window rect was read via the process's main window handle; no input was
  simulated and no window was moved/resized.
- Coordinate alignment between the handler row and the host panel, and any
  clipping, remain unresolved (DLL bodies unavailable).

## Supporting raw files

- `C:\Users\EricM\AppData\Local\Temp\kilo\snapshot_raw.txt` — primary read output
- `C:\Users\EricM\AppData\Local\Temp\kilo\snapshot_followup_raw.txt` — follow-up
  reads (Y constant, subvector begin/end, count)
- `C:\Users\EricM\AppData\Local\Temp\kilo\read_window.ps1` output — window rect

No commit or push was performed for this snapshot step (per instructions).
