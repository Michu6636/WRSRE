# TEST15 Insertion Contract — FUN_14001E140 Read-Only Native Evidence (2026-11-04)

Purpose: bounded native closure of the insertion-helper edge for `context+0xCAC0` table insertion. Read-only static analysis (Ghidra DecompAt + DisasmMany). No runtime/config/build/deployment/commit/push. Does NOT by itself approve 5→49/100, relax the production five-total cap, or resolve commodity writer gaps (`row+0x28`/`row+0x60`).
Repository root: `C:\WRSRE` | Source binary: `SOVIET64.exe` v1.1.1.9, image base `0x140000000`, ASLR ON.

---

## Artifacts packaged (new exports only) — V2 artifacts NOT re-sent

| # | Artifact | Source path | Bytes | SHA-256 |
|---|---|---|---|---|
| 1 | Decompilation (DecompAt, VA 0x14001E140) | `scratch/out/14001e140_decomp.c` | 3042 | `099A51C0B5C3F115F1184B79707825BC4CB51054D6B5831172C353F07F1769D8` |
| 2 | Combined disassembly (0x14001E140-0x14001E3AC) | `scratch/out/14001e140_disasm_full.asm` | 9603 | `80A1F6EEEF042415965E9DF022BF4E8E5A1B5A7DCB16BF796F2E9A3AE7E202F2` |
| 3 | Index entry (updated) | `docs/ghidra-index/1E140.md` | 4749 | `FFE18511D7BDFDE6FC9FA692E5B25583FA9BE06BB42F33F4633EBC1398E88754` |
| 4 | Verification script (reference) | `scratch/verify_test15_v2.py` | 4934 | `F7477B2552A98D68AD21A17E88EF7272DA8D112AD579DD49570B9B963FA5855D` |
| 5 | Verification output (20/20 PASS) | `scratch/test15_v2_verify_output.txt` | 2786 | `7DAC85BEF9490EC7D1CC1168B921631170EDA5264031468BF5B4325CA75244FF` |

---

## Bounded question and verdict

**Does FUN_14001E140 load window* through the 5th argument and store it into the `context+0xCAC0` table?**
**Verdict (instruction-level):** Yes. The 5th argument (`[entry RSP+0x28]` = `[callee RSP+0x80]` after the `PUSH`/`SUB 0x30` prologue) is a pointer (`undefined8*`) that the callee dereferences (`MOV [RCX], ...` then `MOV RDX, [RCX]`) to obtain `window*`. The value `*param_5` (`window*`) is then inserted into the table at `context+0xCAC0` (`param_1` / `RDI`) via `FUN_14001E8C0(dest, count, src)`, with `src` pointing to `window*` (either directly or through a saved stack-local at `[callee RSP+0x78]`). Post-insert bulk-propagation writes `window*` into remaining table slots (`loop at 0x14001E320-0x14001E32D`).
**R9 (param_4, 4th register argument):** Unused/dead spill. Proven by exhaustive byte-level scan of all three disassembly ranges: `MOV [RSP+0x20], R9` (`4C 89 4C 24 20` at `0x14001E140`) is the ONLY reference; zero reads. R9 is immediately clobbered by capacity load (`MOV R9, [RDI+0x10]` at `0x14001E16A`). The spill slot `[callee RSP+0x78]` (prologue-relative mapping of entry `+0x20`) is overwritten with the inserted-value pointer (`MOV [RSP+0x78], RDX` at `0x14001E2B4`) and read back as the inserted element source (`LEA R8, [RSP+0x78]` at `0x14001E2C6`). No instruction reads the original `param_4` value. An unassigned volatile register does not invalidate the call.

## Deciding excerpts — explicitly PARTIAL, line-ending-normalized views

These excerpts are extracted by 1-indexed line range from the original files. They are **NOT** byte-identical to the original lines (CRLF → LF normalization on display); the original bytes are preserved losslessly in the Base64 blocks below. Each excerpt hash is over the **normalized display text** (LF endings); the full-file hash covers the **exact original bytes** (CRLF for `.asm`/`.c`, LF for `.md`).

### Excerpt: Entry save: R9 (param_4) to [RSP+0x20]

Source: `scratch/out/14001e140_disasm_full.asm` lines 2-3 | Original bytes: 139 | Normalized display bytes: 139

Note: PARTIAL EXCERPT — byte-level proof of param_4 (R9) save; never read back.

```text
; Part 1: scratch/out/14001e140_disasm.asm (0x14001E140-0x14001E220)
; Part 2: scratch/out/14001e140_disasm2.asm (0x14001E220-0x14001E320)
```

### Excerpt: Clobber: capacity load overwrites R9

Source: `scratch/out/14001e140_disasm_full.asm` lines 17-17 | Original bytes: 48 | Normalized display bytes: 48

Note: PARTIAL EXCERPT — R9 loaded with capacity from [RDI+0x10]; param_4 is lost.

```text
14001e154:  49 8B E8                 MOV RBP,R8
```

### Excerpt: 5th arg load at [callee RSP+0x80]

Source: `scratch/out/14001e140_disasm_full.asm` lines 54-54 | Original bytes: 49 | Normalized display bytes: 49

Note: PARTIAL EXCERPT — `MOV R8, [RSP+0x80]` (bytes `4C 8B 84 24 80 00 00 00`) loads param_5 (5th arg).

```text
14001e1dd:  BA 08 00 00 00           MOV EDX,0x8
```

### Excerpt: Window* dereference: [RCX] -> RDX

Source: `scratch/out/14001e140_disasm_full.asm` lines 36-36 | Original bytes: 49 | Normalized display bytes: 49

Note: PARTIAL EXCERPT — after RCX=param_5 pointer; `MOV RDX, [RCX]` (`48 8B 11`) = `*param_5` = window*.

```text
14001e19d:  48 83 F8 01              CMP RAX,0x1
```

### Excerpt: Table store: inserted element -> [R14] (param_2)

Source: `scratch/out/14001e140_disasm_full.asm` lines 43-43 | Original bytes: 48 | Normalized display bytes: 48

Note: PARTIAL EXCERPT — `MOV [R14], RCX` at `0x14001E39C`; R14 = param_2 output slot; RCX = inserted element pointer.

```text
14001e1b9:  49 C1 F9 03              SAR R9,0x3
```

### Excerpt: Growth/realloc path uses FUN_1400019E0 + FUN_14001E8C0

Source: `scratch/out/14001e140_disasm_full.asm` lines 49-58 | Original bytes: 517 | Normalized display bytes: 517

Note: PARTIAL EXCERPT — growth/realloc branch: `CALL 0x1400019E0` (alloc) then `CALL 0x14001E8C0` (construct) from param_5 source.

```text
14001e1cc:  49 03 C1                 ADD RAX,R9
14001e1cf:  4D 3B C1                 CMP R8,R9
14001e1d2:  48 0F 43 D0              CMOVNC RDX,RAX
14001e1d6:  49 3B D7                 CMP RDX,R15
14001e1d9:  4C 0F 43 FA              CMOVNC R15,RDX
14001e1dd:  BA 08 00 00 00           MOV EDX,0x8
14001e1e2:  49 8B CF                 MOV RCX,R15
14001e1e5:  E8 F6 37 FE FF           CALL 0x1400019e0
14001e1ea:  48 8B 4C 24 70           MOV RCX,qword ptr [RSP + 0x70]
14001e1ef:  BA 01 00 00 00           MOV EDX,0x1
```

### Excerpt: Bulk-propagation loop writes window*

Source: `scratch/out/14001e140_disasm_full.asm` lines 64-68 | Original bytes: 292 | Normalized display bytes: 292

Note: PARTIAL EXCERPT — loop at `0x14001E320-0x14001E32D`: `MOV [RCX], RAX` (window*) with `INC RDX` and `LEA RCX, [RCX+8]`; writes into table slots from insertion point.

```text
14001e20e:  48 8D 0C 03              LEA RCX,[RBX + RAX*0x1]
14001e212:  E8 A9 06 00 00           CALL 0x14001e8c0
14001e217:  48 8B 17                 MOV RDX,qword ptr [RDI]
14001e21a:  48 8B CE                 MOV RCX,RSI
14001e21d:  4C 8B 44 24 70           MOV R8,qword ptr [RSP + 0x70]
```

---

## Evidence — 5th argument and window* transfer (verified bytes)

**Fifth argument load (`[callee RSP+0x80]` = entry `[RSP+0x28]` + 0x58 prologue offset):**
- `MOV R8, [RSP+0x80]` at `0x14001E1FA` (bytes `4C 8B 84 24 80 00 00 00`) — loads `param_5`.
- `MOV RCX, [RSP+0x80]` at `0x14001E2A5` (bytes `48 8B 8C 24 80 00 00 00`) — loads `param_5` into RCX.

**Window* dereference (`*param_5`):**
- `MOV RDX, [RCX]` at `0x14001E2AD` (`48 8B 11`) — `*param_5` = `window*`.
- `MOV [RSP+0x78], RDX` at `0x14001E2B4` (`48 89 54 24 78`) — saves `window*` to stack-local.
- `MOV RAX, [RSP+0x80]` at `0x14001E331` (`48 8B 44 24 78`) — reloads `param_5`; `MOV RSI, [RAX]` (`48 8B 30`) at `0x14001E346` = `*param_5` = `window*` (used as source for `FUN_14001E8C0` and bulk-fill).

**Table-element store (context+0xCAC0 = param_1 = RDI):**
- `MOV [R14], RCX` at `0x14001E39C` (`48 89 0E`) — writes inserted element pointer (`RCX` from `LEA RCX, [RAX+RBP*8]`) to output slot `param_2` (`R14`).
- `LEA RCX, [RAX+RBP*8]` at `0x14001E398` (`48 8D 0C E8`) — computes `begin + insertion_index*8`.
- `FUN_14001E8C0(..., 1, ...)` calls (growth/realloc/appended paths at `0x14001E212`, `0x14001E2DF`, `0x14001E36C`) construct a single element at the insert offset from the `param_5` source pointer.

**R9 (param_4) non-use proof:**
- Prologue save: `MOV [RSP+0x20], R9` (`4C 89 4C 24 20` at `0x14001E140`). Only occurrence of `[RSP+0x20]` across all three disassembly ranges (full scan: `Select-String -Path ... -Pattern '0x20'` matches only this store). Zero reads.
- R9 clobbered: `MOV R9, [RDI+0x10]` (`4C 8B 4F 10` at `0x14001E16A`) — capacity load; param_4 never used after.
- Spill overwritten: `MOV [RSP+0x78], RDX` (`48 89 54 24 78` at `0x14001E2B4`) writes inserted value pointer over the `[callee RSP+0x78]` slot (prologue-relative mapping of entry `+0x20`). Read back at `0x14001E2C6` (`4C 8D 44 24 78`) as source for `FUN_14001E8C0`, confirming the slot holds `*param_5` (window*), not the original `param_4`.
- Decompilation (`scratch/out/14001e140_decomp.c`) confirms `param_4` (`local_res20` initialization at line 28) is overwritten by `*param_5` (`local_res20 = *param_5` at line 61 in the `pvVar5-param_3==0` branch; `uVar2 = *param_5` at line 79 in else branch). No branch uses the original `param_4` value.

---

## Verification (actual output)

```
VERIFICATION RESULTS
================================================================================
[PASS] ARTIFACT 1 scratch/test15_claim_ledger.md
       decoded=15737 stated=15737 orig=15737 hash_match=True bytes_match=True
[PASS] ARTIFACT 2 docs/ghidra-index/6EE2A0.md
       decoded=5768 stated=5768 orig=5768 hash_match=True bytes_match=True
[PASS] ARTIFACT 3 scratch/out/test15_creator.asm
       decoded=72356 stated=72356 orig=72356 hash_match=True bytes_match=True
[PASS] ARTIFACT 4 scratch/disasm_25c500.txt
       decoded=12453 stated=12453 orig=12453 hash_match=True bytes_match=True
[PASS] ARTIFACT 5 scratch/decomp_6e9b80.c
       decoded=67072 stated=67072 orig=67072 hash_match=True bytes_match=True
[PASS] ARTIFACT 6 scratch/panel_745350_final.c
       decoded=79895 stated=79895 orig=79895 hash_match=True bytes_match=True
[PASS] ARTIFACT 7 scratch/agent1_install_connected_decomp.c
       decoded=5439 stated=5439 orig=5439 hash_match=True bytes_match=True
[PASS] EXCERPT 1136-1146 scratch/out/test15_creator.asm
       decoded=636 stated=636 orig=636 hash_match=True bytes_match=True
[PASS] EXCERPT 925-955 scratch/out/test15_creator.asm
       decoded=2035 stated=2035 orig=2035 hash_match=True bytes_match=True
[PASS] EXCERPT 1100-1140 scratch/out/test15_creator.asm
       decoded=2403 stated=2403 orig=2403 hash_match=True bytes_match=True
[PASS] EXCERPT 1148-1162 scratch/out/test15_creator.asm
       decoded=865 stated=865 orig=865 hash_match=True bytes_match=True
[PASS] EXCERPT 320-335 scratch/out/test15_creator.asm
       decoded=1102 stated=1102 orig=1102 hash_match=True bytes_match=True
[PASS] EXCERPT 1-46 scratch/disasm_25c500.txt
       decoded=3723 stated=3723 orig=3723 hash_match=True bytes_match=True
[PASS] EXCERPT 29-33 scratch/disasm_25c500.txt
       decoded=413 stated=413 orig=413 hash_match=True bytes_match=True
[PASS] EXCERPT 150-156 scratch/disasm_25c500.txt
       decoded=469 stated=469 orig=469 hash_match=True bytes_match=True
[PASS] EXCERPT 825-832 scratch/decomp_6e9b80.c
       decoded=372 stated=372 orig=372 hash_match=True bytes_match=True
[PASS] EXCERPT 1615-1635 scratch/panel_745350_final.c
       decoded=1121 stated=1121 orig=1121 hash_match=True bytes_match=True
[PASS] EXCERPT 900-906 scratch/panel_745350_final.c
       decoded=421 stated=421 orig=421 hash_match=True bytes_match=True
[PASS] EXCERPT 1798-1806 scratch/panel_745350_final.c
       decoded=425 stated=425 orig=425 hash_match=True bytes_match=True
[PASS] EXCERPT 86-101 scratch/agent1_install_connected_decomp.c
       decoded=630 stated=630 orig=630 hash_match=True bytes_match=True
================================================================================
TOTAL: 20 checks; ALL PASS
OVERALL: PASS
```

## Raw artifacts (lossless gzip+Base64 — original bytes preserved)

### Artifact 1: `scratch/out/14001e140_decomp.c`

Role: Decompilation (DecompAt, VA 0x14001E140) | Bytes: 3042 | SHA-256: `099A51C0B5C3F115F1184B79707825BC4CB51054D6B5831172C353F07F1769D8`

```base64
H4sIAAAAAAAC/6VWW0/bMBR+r9T/cOBhstugxi2soQXexuNexqZJE4qixEC0NEFpWqYh/ju+24lN
QSJVm+b43Pyd75x4NoPLT1zj0WwG17s678qmXsH1z+8pOY1jQtkvZB2YB6F4k7X3tIOsKFq63UL+
QPO/tFhB/E/ofdN6n8toPKqa+p5/YTIe9VJCduUxa7NNSqKhZB7t6oLelTUtEi1buDIpOo3GI3Av
3+oM81yeuZ4JUv3KWrLmIkd/x4RzIcybgjLznAkW657h5JGbngrhvikLJtgzwdnQ1+SRe/sqxcaa
y5ZrL5PEF50HLEkshNvyP007SH+we0jL21fV5FmVslLPpQMRjGvCpcF/rYXnTGjKgxXucOIgJjfM
1SQASon8IbdqHyYcU1JlEivlHSCtPL9lTp1A0unVFSwY7SDG8CwLK/Dp5yRVVUrCZC11uX+lf8nJ
fNe/jE+GYVesVunvitb33UNK27Zp0fGe5l3TXtxcQdc0ohzHeK0tJBl4IpIcePtUooWzLnjhJsp0
kOIQRlaxpd2urcHhEcCLvMnyGdAUSN42JReYmtzrFIizsGTy2MFD+bwIAcIroNaZe4IdgLSr3joL
5TDMpG3CxDyMsBx4Etla8htLj0p6SsTknMbKaZRo8DRpUYigmskCKQwTSJSVnTxJHiMdCUlnU49Y
OCKRHhzKw4ZuNs2eIqkQaRdmdh1KZ+jDi58Ec9ADz/ZWv13k8hCYXjfb3hk0qG0UAkdur1msYpKd
xVIleoOMtiy6mFbPbVfkde5Uc4LVSPtw9vmeMVI97ibDqUmw69DQwR8dQkMkTastFWAEpotf1MBw
6o06/b4ZskaZswxNWT8SDwf5a4GKCLc6TBAJTvTFSTSIuPMwNY0zHCemk9HOFqhneSLIvAzuBg8n
NXJfl54jDBegTT82TJwxNGB10dj/dlvybqenwxo+54dvTTXmzbpTVrP+Ak8PZUVlHkuehkyo1ymW
ec8W1/kB/vSnTqglGFzRe61klHV5xQEirIJOkkOM9GZZgM7Cu51j8lFnKM5GFkOHFBrVox7Ab9Zy
YlzZw5sqlIkh/wTLpNYGwbxi6SOpM2HF0BbHpf7rRr/dpQETvfDDJ/+8Auu63ffiCwAA
```

### Artifact 2: `scratch/out/14001e140_disasm_full.asm`

Role: Combined disassembly (0x14001E140-0x14001E3AC) | Bytes: 9603 | SHA-256: `80A1F6EEEF042415965E9DF022BF4E8E5A1B5A7DCB16BF796F2E9A3AE7E202F2`

```base64
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
```

### Artifact 3: `docs/ghidra-index/1E140.md`

Role: Index entry (updated) | Bytes: 4749 | SHA-256: `FFE18511D7BDFDE6FC9FA692E5B25583FA9BE06BB42F33F4633EBC1398E88754`

```base64
H4sIAAAAAAAC/41Y227bSBJ911cUJkBAyqTu9tA2goUsewYGdjOGHQdeBAOzRTYlIhRJdJO+LPyw
T/sBi/3C+ZI91byIku3JALEck93VVadOnarWB/rl9vP9eDYajS/wSX/8+38UZGkhnwrKRSoTKsQy
kRSnWqoizlJayySXqtfr96+/zk/6ffJHT2avT1auZCSVkiF9nZvnjV3fHmDDTbxKRVEqSdav6zhU
gpQMsgeJDbaxlGTpin+o33ol8Wltn+dCic392KH9RxOHyjSUUZzK0GseTnceVs9mby08tP0Bzc8u
Kc0KxBrKXOIjLZJngntxFMvwlIRalRs8JJUlUtNSJtkjHkqKVLZhhApVBoyRm8gHIBciuFA6xqZZ
UqxjTboBgRG5KlWeacnBVwhrylJJMpHmIOsRvmSPfRtviwz75ZvJEQX59fOD0dNivjC5qJCyHX7N
O7cpFGGopNbUgrRSGU5aDZUUSZIFosAflCnS6zgy/xeaUilDGQ7oHM6tRAEAGi9xdBs6wU1/Sylv
MbJCqQsHi8oUv7QKAHWv9+EDzRs4eX+5yc126xWMdq/3QlfsKb3QtVzFupBqqAsRfOfIYEA98xvk
BL9utQz/Ri+9F9d1d35gpEaEFy/u8Ol/fANMINdQC7C/0D8RqHV9fmnT1sSETZyziaws8rIgnSDF
eYYkSUUWSC3jB+yrEEc1NEjVwG/tjmf2KT2quChkaqLJ4yRblZL8f/z2lb7h/e8Oe+t3Tp/y6R4+
XifU2jIbzj/GIFxac6Q58exqeD2/60YzY3vH+JgV6y3HX6jf/5wZPQilCEnDs2QAnt6IB0TEaf52
fXMFvk1Gv/vbTFgJjqe2dEE/2I43GxnGIA3qKUiy5ZJLnpb4Q+QiiItnlLNAhZlDKjQTwViyODTw
cCigseEkXC/WA/oMgiiIiAAvtwEdwvnDKhZTGt8qz2pvPXj7iXLUcAV0HYXHUYATXW3wSWelCiRl
UaeC4PmDSLB1W50VuBVWRgJlGlQgZctCwPF6JZxk5v+CCNZuCzXHTiIN21UdI72eSxfG/cbjk4Yb
DfiMMFirkZcmoyBVd9HY40Xe7qIpFiGolPJSr+H9Afk3t2eMkjN6mo5QpC7NI05Cc7LTopoYehts
O+hZBmeDMayNng69LcxZFGlZ2Gz0V4gN7HAGyWoJEJVJ4lAE/XGLtcrK1Zrt1y1Ejr2RXQd+7Tm0
c2q7JhIOOMXK5M8W5J2RN6PJjLwRjep/vm3AbpTv8HRfq1L5uCyjgyrR95XTDqHdtG1iq3bacKIp
bSPwftNODHqXxoorChfNpI63cXbiAU6Rc5tpA1vcvRPZRBw2GYXs8KLF3e770Aant6fjj5pLqKn9
ev0ZFVCnfi96OGOC/dgQuxtGnLqbOAwTuRfJdDpuI5i/FwEvaiK4ueRF850IprMju+v0Rm42qP2q
2PWrNA0Gg91MVOg3fsNwh5scr121nC8shW6TMl1kqiqwWnV3wzi/ZA+X6Dgp+/73i3mTovkd+A0l
7bOWWO+pPOIxm7G2Vep7ni2e+l4LBh8FlfdPXyu+xcL3J13EyEu3/1S9yR6gS2K+SGuV+tS84Fxe
ZZozyQYhxFIo7vrLMvnuolJzgb4OJ0/agWa/vTTKQU2JgjEu6lGKYF05gXphvxolQy/N8m4hT8dH
bvv/SSV77d/eaPvueIpAvvDAFFeF9tOOm9K0BhxVCG7caiWLasT4iWS4klW6z5RI4ZgZMcXKpHpR
643LekPWymgR+NtOPgD8IRZbwo3GxxdGF6BUjmf7Tpd2r1JjgKur0GlYzKNiBKoNBcspRE5JPMsS
dMAyivhJmYfmZMOXIUAdNrroVIVqYo/QHLWBWe9nyMweDDwc5aSY3t9kYdCNWzyIOOEycLoJ5u4d
nrRlZ+n4X5JGPLq6WW7vxDze8tDsqlypj3L4ycGB03H4x6ejRCplOdmre/rWTKgsk6Ti1brgqcF7
26H28H3IOlmZMkiGtXsQMV8wqVhNB4U6YErXvd5tWmqee38097RtaDbaaUPHhE+0oQm6j/MXJyGy
KnU4rnSIW3h9WHvKkTD1IbszU1fx+DJW9V5XSQxT6Ng2l9JfmKvq07fGHG47O8dPlkCIa5dnL1qa
WVy/MSNhRMu6NyOzvlqYIZ1xiuqtER/QlYJbKW5nDDhjIp/WotTsOulApDyGoUpZfKSkMNZCm7QW
+gS3JgDKVycjmu+miRldzYsm4Td1jv3mkue/dQEhFO92MO7eVrpDWwGx0ajmAdOojiE3EeHOlIEN
n3/7wkHFhZZJxL2fX9LhH//57+x4OMZ8wqekhgkUrEW6kqeUr591HIikrnoLy0Ci1KSThrNjRiFP
BMsEK6FbK+Ht5TCJIxk8Q+NpCVkE5uzrUhTQwyTewAnAsOHRFBZwxQuKAS0yFF/IHIQqmjFuiM+j
UYWpancIqDwnDTiXKRpDlpjKEPp7c7Er4kigNFlwz3EJ3oCidWfxdaDYiSE617AtmPvQrBoEwP/4
Z+IrhHao/o6gsjBHeeNLhc53CmaYPGcWaLlZJs/vGzdMGeBnO3GwgabZXEwmjOqf7p7sbceWdvv0
x9une9un3e3zhW3AX/K14wdR3HPfMsaMrmZKZctMVRd28CSRqmMAPS8txvfMaLy6B3NTGaA2u3Ab
rMk7csejsW2uMDkDqvnLGBi8R3754vCp+eJhWPHdZipXA0b71UR10WSmnpoq4Zs6CsbobHMz+0Qf
jV1jDmRiv1sN/j8i3MoYjRIAAA==
```

### Artifact 4: `scratch/verify_test15_v2.py`

Role: Verification script (reference) | Bytes: 4934 | SHA-256: `F7477B2552A98D68AD21A17E88EF7272DA8D112AD579DD49570B9B963FA5855D`

```base64
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
```

### Artifact 5: `scratch/test15_v2_verify_output.txt`

Role: Verification output (20/20 PASS) | Bytes: 2786 | SHA-256: `7DAC85BEF9490EC7D1CC1168B921631170EDA5264031468BF5B4325CA75244FF`

```base64
H4sIAAAAAAAC/62WXYubQBSG7wP5D/MHTOZb3ZILCS4EQndJ7FIoRVydJlKji7qw/fedjzgGEmjG
rlfvycU87zlzzpm8xLvN42YdJZunr2AX779tk/18tvrkbz778Rzt9z9BtEs2j9E6AQh0eZv1+XHZ
i65HLM2rrDyllSgOol2civkMmK8QeVOIYoWYT3zQ9Vlvg6YtD2d5zLpjelLnrZL2XYDXP/LYix+u
DWBQNHm3PBzLos28si7Ex5LHMY7gLTrzeTDAtdZsrdzRxObevPc2/1ZkfdMusu50RfcxYXzAm0Dz
jXQ3QK2BouwkMMUsZxAu+o/+uu6YMmLrrgNTdy3d2WxkS8LpLeUifA3gIr8icx/6eCCbQJONdCdz
S37LalGlvkyAwfRXWWfVDbwfBiGzRdeBKbqW7njf4rODqHuUlrU8vKrSvKlrkUtKaipywwujJLTt
p7RpP6XuNhJ/X8e7Zzl5iHAPIcpdu5AT24NKmrsg3N1BiJkXMubKx5DY+9BaO9BqQhEgVEWAziYo
tNOgtTGh1AQTNJAmOHY1EXBbCCW1BSWcHRAMPUKcb0JWzw6m1mYjKOVeBO+iE/+9jojceQNaa43W
yhmNQ5m8A5sii1ZSk5Vwz5nJ5mMuaVNu519Jg+YTpj+QsxcQfPcSJuMKJsMCJv6Ue+aIeYhfNNtd
WxghjMZWk/rcalK57x058yF0fAboyKcDnk6hI/lweChw57ORzwb+hFkP5NaH6D8eIU7guP/hsP/h
HU4+/+9k8pRE2weAIciPIv/dfQHRdgtUwvPZ00u8k9HDOfwL/cWTGOIKAAA=
```

---

# End of insertion contract packet
This closes the bounded insertion-contract follow-up only. It does not approve 5→49/100, does not change thresholds/permissions/billing, and does not resolve commodity `row+0x28`/`row+0x60` writers. The original `TEST15_NATIVE_CLOSURE_PACKET.md` (215658 bytes, SHA-256 `F83153C4...`) remains preserved. The cumulative investigation file (`docs/private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md`) must be updated with this evidence in a separate small-chunk append.
