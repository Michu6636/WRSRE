=== SOVIET64.exe Live Read-Only Memory Snapshot ===
Timestamp (UTC): 2026-11-09T21:31:02Z
Process ID: 10224
Executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
Module base address: 0x00007FF6186D0000
Build verification: SHA-256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8, TimeDateStamp 0x6A3EB6AD (1.1.1.9)

--- Module Globals (base + RVA) ---
DPI scalar (base+0x992088): 0.8
World object pointer (base+0x9D4F10): 0x00002DFE42BD5441
Shared panel 0x1409BE060 (base+0x9BE060): 0x00007FF6190BACD0

--- World Object Relative Fields (world_ptr + offset) ---
Handler selector (world+0x10398): READ_FAILED: 299
Selected sub-tab index (world+0xC65C): READ_FAILED: 299
Dirty counter (world+0x10BE8): READ_FAILED: 299
Dirty flag byte (world+0x1039A): READ_FAILED: 299
Panel top pixels (world+0xAE50): READ_FAILED: 299
Panel right edge (world+0xAE54): READ_FAILED: 299
Panel bottom pixels (world+0xAE58): READ_FAILED: 299
Panel geometry raw (world+0xADB0, 16 bytes): READ_FAILED: 299
Trade vector object pointer (world+0xC650): READ_FAILED: 299

--- Raw Memory Dumps ---
world+0xC650 (trade vector object ptr): READ_FAILED: 299
world+0xC65C (selected sub-tab): READ_FAILED: 299
world+0x10BE8 (dirty counter): READ_FAILED: 299
world+0x10398 (handler selector): READ_FAILED: 299
world+0x1039A (dirty flag): READ_FAILED: 299
world+0xAE50 (panel top/right/bottom): READ_FAILED: 299
world+0xADB0 (panel geometry): READ_FAILED: 299

=== End of Snapshot ===
Assumptions:
  - World object pointer at base+0x9D4F10 (from Reputation mod config, unverified at runtime)
  - Trade vector stride = 96 bytes (0x60), per FUN_140373EA0 disassembly
  - Vector begin at vec_obj+0x48, end at vec_obj+0x50, per FUN_140373EA0 disassembly
  - Trade entry text_id at +0x40, drawable ptr at +0x48, per FUN_140373EA0 disassembly
  - Handler selector at world+0x10398, per FUN_140373EA0 gating logic
Read failures indicate the address was not readable at snapshot time.
A selector value alone does not prove a handler executed.

=== BLOCKER ===
The world object pointer assumption (base+0x9D4F10 -> game object ptr) from the Reputation mod config was INCORRECT.
The value read at base+0x9D4F10 was 0x00002DFE42BD5441, which is not a valid readable pointer (all world-relative reads failed with Win32 error 299).
Consequently, none of the world-relative fields could be captured:
  - Handler selector (world+0x10398): unreadable
  - Selected sub-tab index (world+0xC65C): unreadable
  - Dirty counter (world+0x10BE8): unreadable
  - Panel geometry (world+0xAE50/0xAE54/0xAE58/0xADB0): unreadable
  - Trade vector object pointer (world+0xC650): unreadable
  - Trade item count and entries: unreadable

Successfully captured (read-only, verified):
  - Module base: 0x00007FF6186D0000 (ASLR'd, differs from preferred 0x140000000)
  - DPI scalar: 0.8 at base+0x992088
  - Shared panel pointer: 0x00007FF6190BACD0 at base+0x9BE060

What the read-only snapshot must check next:
  - Locate the actual game/world object pointer (base+0x9D4F10 was wrong)
  - Once found, re-read handler selector, selected sub-tab, dirty counter, panel geometry, and trade vector
  - Verify whether the trade vector contains 8 or more entries
  - Confirm whether handler selector matches FUN_140373EA0 or FUN_140358A20

A selector value alone does not prove a handler executed.
