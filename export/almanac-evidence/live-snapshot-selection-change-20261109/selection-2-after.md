=== SOVIET64.exe Economy & Trade Row Snapshot ===
Timestamp (UTC): 2026-11-09T22:06:53Z
Process ID: 10224
Executable: C:\Workers Resources Soviet Republic\SOVIET64.exe
Module base B: 0x00007FF6186D0000
Build: SHA-256 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8, TimeDateStamp 0x6A3EB6AD (1.1.1.9)

game = B + 0x9D4F10 = 0x00007FF6190A4F10 (address itself, not dereferenced)

--- Fields at game + offset ---
Handler selector (game+0x10398): 1
Selected sub-tab index (game+0xC65C): 1
Dirty flag byte (game+0x1039A): 00
Panel top pixels (game+0xAE50): 184
Panel right edge (game+0xAE54): 785
Panel bottom pixels (game+0xAE58): 732.3999
Panel geometry raw (game+0xADB0, 16 bytes): 60 AE 95 E2 F2 01 00 00 60 AE 95 E2 F2 01 00 00
Pointer game+0x10BD0: 0x000001F395E65F00
Pointer game+0x10BD8: 0x000001F395E65F50

--- Trade Vector (P = read_u64(game+0xC650)) ---
P = selected panel pointer (game+0xC650): 0x000001F3103F26D0
  Vector begin (P+0x48): 0x000001F2C917D050
  Vector end (P+0x50): 0x000001F2C917D2D0
  Vector capacity (P+0x58): 0x000001F2C917D320
  Trade item count (end-begin)/0x50: 8
  Vector element stride: 0x50 bytes (80)
  Decoding all 8 trade entries (stride 0x50, text_id at +0x40, drawable at +0x48):
    Entry[0] at 0x000001F2C917D050: text_id=1501 drawable_ptr=0x000001F361595760
      Raw bytes (0x50): 69 6D 70 6F 72 74 65 78 70 6F 72 74 00 74 69 6F 6E 00 6E 67 73 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 DD 05 00 00 F6 7F 00 00 60 57 59 61 F3 01 00 00
    Entry[1] at 0x000001F2C917D0A0: text_id=1511 drawable_ptr=0x000001F3615973E0
      Raw bytes (0x50): 70 72 6F 64 75 63 74 69 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 E7 05 00 00 F6 7F 00 00 E0 73 59 61 F3 01 00 00
    Entry[2] at 0x000001F2C917D0F0: text_id=1664 drawable_ptr=0x000001F361597E60
      Raw bytes (0x50): 70 72 69 63 65 73 00 69 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 80 06 00 00 F6 7F 00 00 60 7E 59 61 F3 01 00 00
    Entry[3] at 0x000001F2C917D140: text_id=1683 drawable_ptr=0x000001F3615976E0
      Raw bytes (0x50): 6C 6F 61 6E 72 75 62 00 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 93 06 00 00 F6 7F 00 00 E0 76 59 61 F3 01 00 00
    Entry[4] at 0x000001F2C917D190: text_id=1684 drawable_ptr=0x000001F361598A60
      Raw bytes (0x50): 6C 6F 61 6E 75 73 64 00 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 94 06 00 00 F6 7F 00 00 60 8A 59 61 F3 01 00 00
    Entry[5] at 0x000001F2C917D1E0: text_id=37554 drawable_ptr=0x000001F361596660
      Raw bytes (0x50): 74 6F 75 72 69 73 6D 00 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 B2 92 00 00 F6 7F 00 00 60 66 59 61 F3 01 00 00
    Entry[6] at 0x000001F2C917D230: text_id=67066 drawable_ptr=0x000001F361595EE0
      Raw bytes (0x50): 63 72 69 6D 65 00 6D 00 6F 6E 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 FA 05 01 00 F6 7F 00 00 E0 5E 59 61 F3 01 00 00
    Entry[7] at 0x000001F2C917D280: text_id=1530 drawable_ptr=0x000001F361598D60
      Raw bytes (0x50): 72 65 73 6F 75 72 63 65 73 00 63 6F 6E 73 75 6D 70 74 69 6F 6E 00 00 00 D9 50 FF 7E FD 7F 00 00 00 00 00 00 F6 7F 00 00 00 08 00 00 00 08 00 00 14 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 FA 05 00 00 F6 7F 00 00 60 8D 59 61 F3 01 00 00

--- Raw Memory Dumps ---
game+0x10398 (handler selector): 01 00 00 00
game+0x1039A (dirty flag): 00
game+0xC650 (selected panel pointer P): D0 26 3F 10 F3 01 00 00
game+0xC65C (selected sub-tab index): 01 00 00 00
game+0x10BD0 (pointer): 00 5F E6 95 F3 01 00 00
game+0x10BD8 (pointer): 50 5F E6 95 F3 01 00 00
game+0xAE50 (panel top/right/bottom): 00 00 38 43 00 40 44 44 98 19 37 44 00 00 00 00
game+0xADB0 (panel geometry): 60 AE 95 E2 F2 01 00 00 60 AE 95 E2 F2 01 00 00

=== End of Snapshot ===
Assumptions:
  - game = B + 0x9D4F10 directly (address itself, not dereferenced)
  - Trade vector stride = 0x50 bytes (80)
  - Vector begin at P+0x48, end at P+0x50, capacity at P+0x58
  - Trade entry text_id at +0x40, drawable ptr at +0x48
  - Handler selector at game+0x10398
Read failures indicate the address was not readable at snapshot time.
A selector value alone does not prove a handler executed.
