# Master function write trace — Almanac evidence (corrected)

**Source:** SOVIET64.exe disassembly, `C:\WRSRE\scratch\out/140317db0_full_disasm.asm`
**Method:** Static only. No game execution, no debugger, no hooks, no writes. All evidence from Ghidra disassembly artifacts.

## 1. Correction of earlier claim

The earlier report incorrectly stated that `FUN_14007C400` and `FUN_140373EA0` were the **only** writers of `game+0xC650` and `game+0xC65C`, and that the branch at `0x14037434D` "skips the index store." Both are wrong:

1. **`FUN_140317DB0` (master dispatch) contains additional writes** to both `game+0xC650` and `game+0xC65C`, including paths that write `-1` and `0`.
2. **At `0x14037434D`**: `CMP [RSI+0xc65c],EBX` / `JZ 0x140374356` — when the current and new index are **equal** (Z=1), it **jumps to** the store at `0x140374356`. When they differ, it falls through to clear the dirty flag (`0x14037434F`) and **also** reaches the store at `0x140374356`. The branch does not skip the store; it jumps to it on equality.

## 2. Panel pointer writes to game+0xC650

| VA | Instruction | Context | Register provenance |
|----|------------|---------|-------------------|
| `0x1400843f3` | `MOV [R14+0xc650],RDI` | `FUN_14007C400` (section-switch handler) | RDI = panel pointer from array scan |
| `0x14031acd9` | `MOV [R14+0xc650],RCX` | Master function `FUN_140317DB0` | RCX = from prior load |
| `0x14031f9a4` | `MOV [R14+0xc650],RCX` | Master function, after collision hit test | RCX = `[RAX + RSI*0x8]` (panel from array at index RSI) |
| `0x1403282f9` | `MOV [R14+0xc650],R15` | Master function `FUN_140317DB0` | R15 = panel pointer |

## 3. Index writes to game+0xC65C

| VA | Instruction | Value written | Context |
|----|------------|---------------|---------|
| `0x14031fb7b` | `MOV [R14+0xc65c],EDI` | -1 (`0xFFFFFFFF`) | After `OR RDI,-0x1` at `0x14031fb77` |
| `0x14031fc76` | `MOV [R14+0xc65c],R9D` | 0 | R9D zeroed by `XOR R9D,R9D` at `0x14031fb21` or `0x14031fb6d` |
| `0x14031fde9` | `MOV [R14+0xc65c],R8D` | 0 | R8D zeroed by `XOR R8D,R8D` at `0x14031fdb4` |
| `0x14031fe38` | `MOV [R14+0xc65c],R8D` | 0 | R8D zeroed by `XOR R8D,R8D` at `0x14031fdb4` |
| `0x14031ff62` | `MOV [R14+0xc65c],EDI` | -1 | EDI = -1 from earlier `OR RDI,-0x1` |
| `0x140374356` | `MOV [RSI+0xc65c],EBX` | New item index | `FUN_140373EA0` (trade row drawer) |

## 4. Control flow from 0x14031f8a3 through the writes

### 4a. Entry: collision hit test (0x14031f8a3)

```
14031f8a3:  FF 15 A7 CF 54 00  CALL qword ptr [0x14086c850]   ; Collision(mouse, button_rect)
14031f8a9:  84 C0              TEST AL,AL
14031f8ab:  0F 84 15 07 00 00  JZ 0x14031ffc6                ; no hit -> skip to end
```
If no collision, jump to `0x14031ffc6` (bypass all writes in this block).

### 4b. State gate (0x14031f8b1)

```
14031f8b1:  41 83 BE 74 AD 00 00 FF  CMP dword ptr [R14 + 0xad74],-0x1
14031f8b9:  0F 85 07 07 00 00        JNZ 0x14031ffc6          ; state != -1 -> skip
```
If `game+0xad74 != -1`, jump to `0x14031ffc6` (bypass all writes).

### 4c. String copy and panel rect setup (0x14031f8bf–0x14031f931)

Copies a null-terminated string to `game+0xd5a0`, computes panel rectangle coordinates. At `0x14031f931`:
```
14031f931:  84 C0              TEST AL,AL
14031f933:  0F 84 8D 06 00 00  JZ 0x14031ffc6                ; condition -> skip
```

### 4d. Flag check (0x14031f939)

```
14031f939:  41 80 BE C3 0F 00 00 00  CMP byte ptr [R14 + 0xfc3],0x0
14031f941:  0F 85 7F 06 00 00      JNZ 0x14031ffc6            ; flag set -> skip
```

### 4e. Panel pointer write path (0x14031f947–0x14031f9a4)

```
14031f947:  49 83 BE 50 C6 00 00 00  CMP qword ptr [R14 + 0xc650],0x0   ; is current panel null?
14031f94f:  74 4B                    JZ 0x14031f99c              ; null -> go directly to write
```
If `game+0xC650 == 0`, jump directly to the write at `0x14031f99c`–`0x14031f9a4`.

If not null (fall through), iterate comparing existing panel name with candidate (lines 6360–6377):
```
14031f951:  49 8B 96 50 C6 00 00     MOV RDX,qword ptr [R14 + 0xc650]
14031f960:  0F B6 44 0A 01           MOVZX EAX,byte ptr [RDX + RCX*0x1 + 0x1]
14031f968:  41 3A 44 0C FF           CMP AL,byte ptr [R12 + RCX*0x1 + -0x1]
14031f96d:  75 0E                    JNZ 0x14031f97d              ; byte mismatch -> no match
14031f96f:  48 83 F9 07              CMP RCX,0x7                  ; 7-char match?
14031f973:  75 EB                    JNZ 0x14031f960
14031f975:  49 8B CE                 MOV RCX,R14
14031f978:  E8 D3 74 01 00           CALL 0x140336e50
14031f97d:  ...                                                   ; no match, fall through
```
If the 7-char name matches, call `0x140336e50` and jump to `0x14031ff70` (skip the panel pointer write). If no match, fall through:

```
14031f99c:  48 8B 45 30              MOV RAX,qword ptr [RBP + 0x30]
14031f9a0:  48 8B 0C F0              MOV RCX,qword ptr [RAX + RSI*0x8]   ; load panel from array[index RSI]
14031f9a4:  49 89 8E 50 C6 00 00     MOV qword ptr [R14 + 0xc650],RCX   ; WRITE panel pointer
```

### 4f. Index -1 write (0x14031fb77–0x14031fb7b)

After the panel pointer write, more logic leads to:
```
14031fb6d:  45 33 C9                 XOR R9D,R9D              ; R9D = 0
14031fb70:  4D 89 8E 28 D4 00 00     MOV qword ptr [R14 + 0xd428],R9
14031fb77:  48 83 CF FF              OR RDI,-0x1                ; RDI = 0xFFFFFFFF (-1)
14031fb7b:  41 89 BE 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],EDI   ; WRITE -1 to index
```

### 4g. String compare with "trade" (0x14031fb9f)

```
14031fb9f:  48 8D 15 3E 26 59 00     LEA RDX,[0x1408b21e4]     ; "trade"
14031fbb0:  0F B6 04 0B              MOVZX EAX,byte ptr [RBX + RCX*0x1]
14031fbb4:  48 FF C1                 INC RCX
14031fbb7:  3A 44 0A FF              CMP AL,byte ptr [RDX + RCX*0x1 + -0x1]
14031fbbb:  0F 85 FD 00 00 00        JNZ 0x14031fcbe            ; byte mismatch
14031fbc1:  48 83 F9 06              CMP RCX,0x6                ; 6-char match?
14031fbc5:  75 E9                    JNZ 0x14031fbb0
14031fbc7:  45 8B C1                 MOV R8D,R9D                ; R8D = R9D = 0
...
14031fbdf:  0F 84 91 00 00 00        JZ 0x14031fc76              ; "trade" matched -> jump to zero index write
```

The string at `0x1408B21E4` was resolved from the executable as **"trade"** (verified via PE section mapping, image base `0x140000000`, RVA `0x08B21E4`).

### 4h. Zero index write (0x14031fc76)

If "trade" matched (JZ taken at `0x14031fbdf`):
```
14031fc76:  45 89 8E 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],R9D   ; WRITE 0 to index
14031fc7d:  45 89 8E D0 C7 00 00     MOV dword ptr [R14 + 0xc7d0],R9D
14031fc84:  41 89 BE D4 C7 00 00     MOV dword ptr [R14 + 0xc7d4],EDI
14031fc8b:  45 88 8E D8 C7 00 00     MOV byte ptr [R14 + 0xc7d8],R9B
14031fc92:  45 89 8E 20 0F 01 00     MOV dword ptr [R14 + 0x10f20],R9D
...
14031fcb9:  E9 B2 02 00 00           JMP 0x14031ff70
```

If "trade" did NOT match (JNZ at `0x14031fbdf` not taken, fall through to `0x14031fcbe`):
```
14031fcbe:  48 8D 15 13 25 59 00     LEA RDX,[0x1408b21d8]      ; compare with another string
...
14031fdb4:  ...                                                   ; array scan block
14031fde9:  45 89 86 5C C6 00 00     MOV dword ptr [R14 + 0xc65c],R8D   ; WRITE 0 to index (R8D=0)
```

### 4i. Other index write paths

Additional index writes in the same master function:
- `0x14031fe38`: `MOV [R14+0xc65c],R8D` (0) — after comparing with string at `0x1408b2250`
- `0x14031ff62`: `MOV [R14+0xc65c],EDI` (-1) — fallback when no string match (JZ targets from `0x14031fedd`, `0x14031fef4`, `0x14031ff16`)

## 5. Indirect writes and search limits

**Direct writes to game+0xC650 found (8 total):**
1. `0x1400843f3` — `FUN_14007C400` (section-switch handler)
2. `0x14031acd9` — `FUN_140317DB0` (master function)
3. `0x14031f9a4` — `FUN_140317DB0` (after collision hit test)
4. `0x1403282f9` — `FUN_140317DB0` (master function)

**Direct writes to game+0xC65C found (7 total):**
1. `0x14031fb7b` — `FUN_140317DB0` (writes -1)
2. `0x14031fc76` — `FUN_140317DB0` (writes 0)
3. `0x14031fde9` — `FUN_140317DB0` (writes 0)
4. `0x14031fe38` — `FUN_140317DB0` (writes 0)
5. `0x14031ff62` — `FUN_140317DB0` (writes -1)
6. `0x140374356` — `FUN_140373EA0` (trade row drawer, writes new index)

**Indirect writes:** None identified in this trace. Writes through pointers (e.g., `[RCX+0x...]` where RCX is loaded from `game+0xC650`) are reads of the field followed by writes to the pointed-to object, not writes to `game+0xC650`/`0xC65C` themselves.

**Search limits:** The disassembly was searched for `c650`/`c65c` string patterns. There may be additional writes via different register encodings or through intermediate pointers not captured by this string search. The search was limited to the master function `FUN_140317DB0` and `FUN_14007C400`/`FUN_140373EA0`.

## 6. Register provenance for key writes

**Panel pointer write at `0x14031f9a4`:**
- `0x14031f8bf`: `MOV RAX,qword ptr [RBP + 0x30]` — load array base from stack
- `0x14031f8c3`: `MOV RCX,qword ptr [RAX + RSI*0x8]` — load panel pointer at index RSI
- `0x14031f99c`: `MOV RAX,qword ptr [RBP + 0x30]` — reload array base
- `0x14031f9a0`: `MOV RCX,qword ptr [RAX + RSI*0x8]` — reload panel pointer
- `0x14031f9a4`: `MOV [R14+0xc650],RCX` — store to game object

**Index -1 write at `0x14031fb7b`:**
- `0x14031fb6d`: `XOR R9D,R9D` — zero R9D
- `0x14031fb77`: `OR RDI,-0x1` — RDI = 0xFFFFFFFF
- `0x14031fb7b`: `MOV [R14+0xc65c],EDI` — store -1

**Index 0 write at `0x14031fc76`:**
- `0x14031fb21` or `0x14031fb6d`: `XOR R9D,R9D` — R9D = 0
- `0x14031fc76`: `MOV [R14+0xc65c],R9D` — store 0

## 7. Evidence files preserved

Folder: `C:\WRSRE\export\almanac-evidence\master-write-trace-20261109/`
- `report.md` — this report
- `excerpt_panel_write.asm` — panel pointer write context (lines 6282–6383)
- `excerpt_neg1_index_write.asm` — -1 index write (lines 6477–6488)
- `excerpt_string_compare.asm` — "trade" string compare and zero index write (lines 6487–6548)
- `excerpt_zero_index_write.asm` — zero index write continuation (lines 6537–6576)
- `excerpt_zero_index_write2.asm` — additional zero index write (lines 6611–6627)
- `excerpt_neg1_index_write2.asm` — fallback -1 index write (lines 6682–6693)

**Static paths vs executed behavior:** All writes are statically identified in the disassembly. Whether any given path executes at runtime depends on the collision result, state flags, string comparisons, and array contents — none of which can be determined statically. The observed live-snapshot behavior (selector=1, sub-tab changes 0→1 on icon click, panel pointer restored on return) is consistent with but not proven by these static paths.
