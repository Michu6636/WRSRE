=== SOVIET64.exe Economy & Trade Row Snapshot ===
Timestamp (UTC): 2026-11-09T22:15:38Z
Process ID: 10224
Executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
Module base B: 0x00007FF6186D0000
Build: SHA-256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8, TimeDateStamp 0x6A3EB6AD (1.1.1.9)

game = B + 0x9D4F10 = 0x00007FF6190A4F10 (address itself, not dereferenced)

--- Fields at game + offset ---
Handler selector (game+0x10398): 1
Selected sub-tab index (game+0xC65C): 4294967295
Dirty flag byte (game+0x1039A): 00
Panel top pixels (game+0xAE50): 184
Panel right edge (game+0xAE54): 785
Panel bottom pixels (game+0xAE58): 904
Panel geometry raw (game+0xADB0, 16 bytes): A0 14 0A 96 F3 01 00 00 A0 14 0A 96 F3 01 00 00
Pointer game+0x10BD0: 0x000001F395E65F00
Pointer game+0x10BD8: 0x000001F395E65F50

--- Trade Vector (P = read_u64(game+0xC650)) ---
P = selected panel pointer (game+0xC650): 0x000001F3103F2598
  Vector begin (P+0x48): 0x0000000000000000
  Vector end (P+0x50): 0x0000000000000000
  Vector capacity (P+0x58): 0x0000000000000000
  Trade item count (end-begin)/0x50: 0
  Vector element stride: 0x50 bytes (80)
  Decoding all 0 trade entries (stride 0x50, text_id at +0x40, drawable at +0x48):

--- Raw Memory Dumps ---
game+0x10398 (handler selector): 01 00 00 00
game+0x1039A (dirty flag): 00
game+0xC650 (selected panel pointer P): 98 25 3F 10 F3 01 00 00
game+0xC65C (selected sub-tab index): FF FF FF FF
game+0x10BD0 (pointer): 00 5F E6 95 F3 01 00 00
game+0x10BD8 (pointer): 50 5F E6 95 F3 01 00 00
game+0xAE50 (panel top/right/bottom): 00 00 38 43 00 40 44 44 00 00 62 44 00 00 00 00
game+0xADB0 (panel geometry): A0 14 0A 96 F3 01 00 00 A0 14 0A 96 F3 01 00 00

=== End of Snapshot ===
Assumptions:
  - game = B + 0x9D4F10 directly (address itself, not dereferenced)
  - Trade vector stride = 0x50 bytes (80)
  - Vector begin at P+0x48, end at P+0x50, capacity at P+0x58
  - Trade entry text_id at +0x40, drawable ptr at +0x48
  - Handler selector at game+0x10398
Read failures indicate the address was not readable at snapshot time.
A selector value alone does not prove a handler executed.
