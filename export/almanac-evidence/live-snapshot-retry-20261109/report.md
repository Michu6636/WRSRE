=== SOVIET64.exe Live Read-Only Memory Snapshot (RETRY) ===
Timestamp (UTC): 2026-11-09T21:38:16Z
Process ID: 10224
Executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
Module base B: 0x00007FF6186D0000
Build verification: SHA-256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8, TimeDateStamp 0x6A3EB6AD (1.1.1.9)

--- Module Globals (B + RVA) ---
DPI scalar (B+0x992088): 0.8
game object address = B + 0x9D4F10 = 0x00007FF6190A4F10
NOTE: game address is treated as a raw address for field reads; bytes at game are NOT dereferenced as a pointer

--- Fields at game + offset (game = B + 0x9D4F10) ---
Handler selector (game+0x10398): 1
Selected sub-tab index (game+0xC65C): 0
Dirty counter (game+0x10BE8): -1
Dirty flag byte (game+0x1039A): 00
Panel top pixels (game+0xAE50): 184
Panel right edge (game+0xAE54): 785
Panel bottom pixels (game+0xAE58): 916.7998
Panel geometry raw (game+0xADB0, 16 bytes): 60 AE 95 E2 F2 01 00 00 60 AE 95 E2 F2 01 00 00

--- Trade Vector (P = read_u64(game+0xC650), then P+0x48/0x50/0x58) ---
P (game+0xC650): 0x000001F3103F26D0
  Vector begin (P+0x48): 0x000001F2C917D050
  Vector end (P+0x50): 0x000001F2C917D2D0
  Vector capacity (P+0x58): 0x000001F2C917D320
  Trade item count (end-begin)/0x50: 8
  Vector element stride: 0x50 bytes (80)
  Decoding first 8 trade entries (stride 0x50, text_id at +0x40, drawable at +0x48):
    Entry[0] at 0x000001F2C917D050: text_id=1501 drawable_ptr=0x000001F361595760
    Entry[1] at 0x000001F2C917D0A0: text_id=1511 drawable_ptr=0x000001F3615973E0
    Entry[2] at 0x000001F2C917D0F0: text_id=1664 drawable_ptr=0x000001F361597E60
    Entry[3] at 0x000001F2C917D140: text_id=1683 drawable_ptr=0x000001F3615976E0
    Entry[4] at 0x000001F2C917D190: text_id=1684 drawable_ptr=0x000001F361598A60
    Entry[5] at 0x000001F2C917D1E0: text_id=37554 drawable_ptr=0x000001F361596660
    Entry[6] at 0x000001F2C917D230: text_id=67066 drawable_ptr=0x000001F361595EE0
    Entry[7] at 0x000001F2C917D280: text_id=1530 drawable_ptr=0x000001F361598D60

--- Raw Memory Dumps ---
game+0x10398 (handler selector): 01 00 00 00
game+0x1039A (dirty flag): 00
game+0x10BE8 (dirty counter): FF FF FF FF
game+0xC650 (selected panel pointer P): D0 26 3F 10 F3 01 00 00
game+0xC65C (selected sub-tab index): 00 00 00 00
game+0xAE50 (panel top/right/bottom): 00 00 38 43 00 40 44 44 30 33 65 44 00 00 00 00
game+0xADB0 (panel geometry): 60 AE 95 E2 F2 01 00 00 60 AE 95 E2 F2 01 00 00

--- Shared Panel Candidate (B + 0x9BE060) ---
Raw bytes at B+0x9BE060 (32 bytes, treated as object not auto-pointer): D0 AC 0B 19 F6 7F 00 00 70 FF 94 E2 F2 01 00 00 30 06 95 E2 F2 01 00 00 00 00 00 00 01 01 01 01
NOTE: B+0x9BE060 is recorded as a candidate shared-panel object address; its contents are raw bytes, not automatically dereferenced

=== End of Snapshot ===
Assumptions:
  - game = B + 0x9D4F10 directly (address itself, not dereferenced), per caller evidence using LEA
  - Trade vector stride = 0x50 bytes (80), per independently verified entry stride
  - Vector begin at P+0x48, end at P+0x50, capacity at P+0x58
  - Trade entry text_id at +0x40, drawable ptr at +0x48
  - Handler selector at game+0x10398
  - B+0x9BE060 treated as candidate shared-panel object address, not auto-pointer slot
Read failures indicate the address was not readable at snapshot time.
A selector value alone does not prove a handler executed.
