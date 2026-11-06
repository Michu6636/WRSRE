# TEST15 Element Construction — FUN_14001E8C0 Read-Only Native Evidence (2026-11-04)

Purpose: bounded native proof of the element-copy helper used by `FUN_14001E140` (VA `0x14001E140`) to insert `window*` into the `context+0xCAC0` table. Read-only static analysis (Ghidra DecompAt + DisasmMany). No runtime/config/build/deployment/commit/push. Does NOT by itself approve 5→49/100 or any cap change.
Repository root: `C:\WRSRE` | Source binary: `SOVIET64.exe` v1.1.1.9, image base `0x140000000`, ASLR ON.

---

## Artifacts packaged (new export only)

| # | Artifact | Source path | Bytes | SHA-256 |
|---|---|---|---|---|
| 1 | Decompilation (DecompAt, VA 0x14001E8C0) | `scratch/out/14001e8c0_decomp.c` | 475 | `5EEEEB871A97A91B9FE6DEF33714D43AF709CA8EBF822E53F4A3609EB8FA9A32` |
| 2 | Disassembly (0x14001E8C0-0x14001E8DA) | `scratch/out/14001e8c0_disasm.asm` | 3266 | `FB3086BB4435BA87EAD6CE88D8C916E2A1740BE50D5C817EE36C7AD90A6C8B83` |
| 3 | Index entry (updated) | `docs/ghidra-index/1E8C0.md` | 2336 | `926B68CDE5F9DCDDD260915FE8D872C0FE608FBA86A4A7263188848FF1F5E921` |

---

## Bounded question and verdict

**Does FUN_14001E8C0 load from a source pointer and store to a destination pointer for count=1?**
**Verdict (instruction-level):** Yes. Signature recovered by Ghidra: `void FUN_14001e8c0(undefined8 *param_1, longlong param_2, undefined8 *param_3)`. `param_1` (`RCX`) = destination pointer; `param_2` (`RDX`) = element count; `param_3` (`R8`) = source pointer. For count=1: `TEST RDX,RDX` (`0x14001E8C0`) is non-zero; `TEST RCX,RCX` (`0x14001E8C5`) is non-null; `MOV RAX,qword ptr [R8]` (`0x14001E8CA`, bytes `49 8B 00`) loads `*param_3` = source value (`window*`); `MOV qword ptr [RCX],RAX` (`0x14001E8CD`, bytes `48 89 01`) stores `*param_1` = destination element. If `param_1 == NULL`, `TEST RCX,RCX` jumps (`JZ 0x14001E8D0`) skipping the store, but still advances/loop-decrements; if `param_2 == 0`, `JZ 0x14001E8DA` returns immediately (no load/store).

## Deciding excerpts — explicitly PARTIAL, line-ending-normalized views

Excerpts are extracted by 1-indexed line range from original files. They are **NOT** byte-identical to original lines (CRLF→LF normalization on display); original bytes preserved losslessly in Base64 blocks below. Each excerpt hash covers the **normalized display text**; full-file hash covers **exact original bytes**.

### Excerpt: Count==0 guard: TEST RDX,RDX + JZ

Source: `scratch/out/14001e8c0_disasm.asm` lines 1-2 | Original bytes: 88 | Normalized display bytes: 86

Note: PARTIAL EXCERPT — byte-level proof of count guard at VA 0x14001E8C0.

```text
; Range: 0x14001E8C0 to 0x14001E980
14001e8c0:  48 85 D2                 TEST RDX,RDX
```

### Excerpt: Source load: *param_3 (R8) -> RAX

Source: `scratch/out/14001e8c0_disasm.asm` lines 5-5 | Original bytes: 53 | Normalized display bytes: 52

Note: PARTIAL EXCERPT — `MOV RAX,qword ptr [R8]` (`49 8B 00` at `0x14001E8CA`) loads *source pointer* param_3.

```text
14001e8c8:  74 06                    JZ 0x14001e8d0
```

### Excerpt: Destination store: *param_1 (RCX) = RAX

Source: `scratch/out/14001e8c0_disasm.asm` lines 6-6 | Original bytes: 61 | Normalized display bytes: 60

Note: PARTIAL EXCERPT — `MOV qword ptr [RCX],RAX` (`48 89 01` at `0x14001E8CD`) stores *dest pointer* param_1.

```text
14001e8ca:  49 8B 00                 MOV RAX,qword ptr [R8]
```

### Excerpt: Advance dest +8; decrement count; loop

Source: `scratch/out/14001e8c0_disasm.asm` lines 7-10 | Original bytes: 216 | Normalized display bytes: 212

Note: PARTIAL EXCERPT — `ADD RCX,0x8` / `SUB RDX,1` / `JNZ` to `0x14001E8C5`. Only iterates when count != 0.

```text
14001e8cd:  48 89 01                 MOV qword ptr [RCX],RAX
14001e8d0:  48 83 C1 08              ADD RCX,0x8
14001e8d4:  48 83 EA 01              SUB RDX,0x1
14001e8d8:  75 EB                    JNZ 0x14001e8c5
```

---

## Evidence — source load and destination store (verified bytes)

**Signature:** `void FUN_14001e8c0(undefined8 *param_1, longlong param_2, undefined8 *param_3)`
- `param_1` (`RCX`): destination pointer.
- `param_2` (`RDX`): element count.
- `param_3` (`R8`): source pointer.

**Source load (`*param_3`):**
- `MOV RAX,qword ptr [R8]` at `0x14001E8CA` (bytes `49 8B 00`).

**Destination store (`*param_1 = *param_3`):**
- `MOV qword ptr [RCX],RAX` at `0x14001E8CD` (bytes `48 89 01`).

**Loop/advance:**
- `ADD RCX,0x8` (`48 83 C1 08`) at `0x14001E8D0` — advance destination by one QWORD.
- `SUB RDX,0x1` (`48 83 EA 01`) at `0x14001E8D4` — decrement count.
- `JNZ 0x14001E8C5` (`75 EB`) at `0x14001E8D8` — repeat while count != 0.
- Entry guard `TEST RDX,RDX` (`48 85 D2`) at `0x14001E8C0`; null-dest guard `TEST RCX,RCX` (`48 85 C9`) at `0x14001E8C5`; `JZ 0x14001E8DA` (`C3 RET`) early exit when count==0.

**Corroborating caller** (`FUN_14001E140`): passes count=1 (`EDX`/`MOV EDX,1`) with source pointer at `R8` (the inserted `window*`) and destination at `RCX` (new buffer offset or end), matching this helper exactly.

---

## Raw artifacts (lossless gzip+Base64 — original bytes preserved)

### Artifact 1: `scratch/out/14001e8c0_decomp.c`

Role: Decompilation (DecompAt, VA 0x14001E8C0) | Bytes: 475 | SHA-256: `5EEEEB871A97A91B9FE6DEF33714D43AF709CA8EBF822E53F4A3609EB8FA9A32`

```base64
H4sIAAAAAAAC/6WOMQ+CMBCFdxL+w3MD0bRVB4JhMjo66UyatiBRi6lgTIz/XVoEYxxt09y93nd3
jxCkfxzfIwSbRou6rHSCzX6bsQWlTMWCgtcYhAN33BSqBpfSqOsV4qDEUckE9O64dbzquP8c+d6t
KuW3l6DRUuWlVjLG+MINP2dscqp0YR+6j9nkF5qHdt7D94C8MgiWPYtRCvpR6ZBFmLIQrgMocwTv
bbbhy0VI73QAMbhqR/W7l13t2YVPvc8iMIc4wKi6MbrVT+vY3hdBXnk22wEAAA==
```

### Artifact 2: `scratch/out/14001e8c0_disasm.asm`

Role: Disassembly (0x14001E8C0-0x14001E8DA) | Bytes: 3266 | SHA-256: `FB3086BB4435BA87EAD6CE88D8C916E2A1740BE50D5C817EE36C7AD90A6C8B83`

```base64
H4sIAAAAAAAC/51Wy27bMBC8B8g/7Ln1YSlREjc9SZSNpkAekNPCaOGDY8u9NW0QoPn8Ug9zKYmM
nAo2YAMaLnd2ZshPUO1+/ayvAF+FRBRLpRFenuxfUnh50f6s1R6vAKQClUAZwfh5WK4foCo3C/Nl
SGwgmQSRgOf58v1Up1aHHYMSW0dToI42dbRTR3V1MJ2t4/Sza+oQqAIQJ6Cbu29Q5ZvFn79Pzwf4
/fIMPyq1ZfCh3yQBCi/YBerNdmEWs+jDicoYtABUQ3Relm2H+KoYIS1imU9Krr8WLfemTUa0nCSw
LLyc3Dqk7BNGNaToGPxPtXywb9ZtDwnEsW9Mm7sKKioX5suIRg1SN4R7BNQSLnCgnzrtEJFBTFlu
u24QjhJq6odqdhUBZh5mDQJfM0YcOoQZRMPsqPX156pHxBZxFN0s4gJ0PtmVvrnv9On0cWynVwKu
QGaTVrRpPe86IYaoDvKW2xqEYEEfH3u3FTMuIMHCOvbtG4q18lPcNGMBhGg3pjGwMTUA9CEwHsZ0
W7hnUBsCeWtOOdG7z5x6Ax9N5Q9mtS0vc5IDhTw+9LfrUrMd1hL6Xapck5LADrBaNWoaP9e3GnjA
JOKOxzdURO54SaS9odWcoc2MGKVsyulwyjnjEo9nJgBFaDMwMR6VQ4rG5K7vzXwMW4bjgjmOErtG
1q4hcH4NgWaR8poX2fWSNDGhAsEi3B6jU3obQO4HFAMHU4wWsSI/orx2k4hiTjsdqEHDEinHowjH
o1uC3hlEFO87L0Y458WkZpC5H0CaAnlBt3f39k2J1jFLmjimDRMaOEZKawJPz20Hg1gkmZ15o3Ab
kGT1gYGDRw2yxAiMwXu+JoizrgnkgI+cP9Fs/rS6dgIoiQaHmQodZsxnkvadnnUtoa1rioTeOYtk
3wdSNBtIkgMpqS2fZRxwkhGtsDcZSgU7KQ6Nzw2VVHYlmk4KfyfFqEQvq2hWVopllTaEmSNdrEBh
c77YT+8LODhk5xuWRYaD25wK3eZ4OJnsOGisHvlbUgOnZ2cbxekoI9Z6IEh16EpM2d5GZMhlI6OU
LiXH/zmpLy/+AWsp5prCDAAA
```

### Artifact 3: `docs/ghidra-index/1E8C0.md`

Role: Index entry (updated) | Bytes: 2336 | SHA-256: `926B68CDE5F9DCDDD260915FE8D872C0FE608FBA86A4A7263188848FF1F5E921`

```base64
H4sIAAAAAAAC/3VW227jNhB991cMkIfaji+yY6dKAqNwrGzRYuNsnUvTFosVLY1topSokpQdA3no
Uz+g6Bful3Soi60o7YMYixoOz5wzl5zAh8f5l8HIcQY37syBr3/+Aygwwth0A5nsYYMiQQWpxhCW
ezBsKRB4rFEZLuNGo91ePE0v223wnZfMhQ/NROEKlaITT9Nsv3Tvt3p04J6vY2ZShdD8fsNDxUBh
ILdIB1qZp63k4REXuoHTTOMQVzzG0IV2whSLvgw6IGS8tg/kO8MOvDc7a/k9mF7/ALE0hDvEBGmJ
jaBYUBsMr4CpdWoDBiUFaliikDvaRFgpGdlYjUoDG21X4BYFhIQ2RBvJp1QlUqMFTWRxOuwHMo2N
Dz/9fLfwdO7BL6H4oGWqAoRE8tgQq7RKOklsBqnhWzyYDny6RRtOPNG9oIU0Gpr+4SMLtywOLNg9
uIAs2AAnh5l1xwJUmYQHPJYrmWjix3BRbE4mjn8F+ndO+9pIipevoLxiMpk/fvxI3xWSUrEGHkUY
cmaQiLN2BxckaePkBKZvSWy+o63VaLzCJ+sdXmGBa64tA/ST7OnPIyXYd/DaeO12u28eOlVgssaz
Z1qr1JRUvsIvdO3RemitPWtdpDNkiN/ZnVk7l5aaNKWdje0+/yQkC4HF4VtpMuaalL18xbMaobSi
WLswKxiCdcpUeAn+w839g8XUoYeqxB+54I7BG5KcplYmV+D/+Csctryptf92BIOx3/pPSZr+4uah
5ohOkThdmKdCdC3mGpIZIZlVkMwu6kjG75E4BRLnnJBUk2e3wbiePvZ2qvcjfVl53949wWL63Plj
J1UIiVHw28L9nOG4APcaHKeOgwKBSbWO6GVHxSx3bTpmNmVHIv63TKRF5frH1kar/40uJB+3clxe
XcYDuiqy2fPnDqEtaboAZ1CH572BN7Dw8j5ZpJ69bppXbN/WISkw9bxMAOfFLT2fwWwAjlsXMc+G
+8frLHVo/2h/M30Pxhvlms0ropGOVrQx3FzXrd2yfmdMCEp7akYGX0yjUSMPAvquq5RSnjZtUk1s
GHlxTW685wn15byUJgt3UohUVlXLB2p4uVYVBbM2yCDGXcGb7XfQzK+s4h0OhtTKjq/eh+rr2fnM
+qrr3urBLTPBhsrZbLguJxq+sICmwGUBfQC0uZIq0uUXkHGetKdZcuQ03RuaXJroKQs+I7TS7CCf
EU3NaTYhFAPOo5ERJVMDp+BxzXR0y+I9AZsJGh8WF8JS2uEVVqZvxSmGa+yBJ8l2fvdguz43GsUK
WJIoGp0w/vrX36OL/sBxriDZ7DUn8sqxQZutDthEtxOmP7og0ow0ZGBo9GHRGTuwtCyB4BH57mSN
jkBHMuRmD76Su1PnZej6/eLnOSXFTtmpo6kjRYyI12grwGA/jRVqKbYY9mAuKTx63UgR6j6RHHGt
KSbdX3IhiKV+wBIINixeoy5nieEr0kDbNppTx0VWp1Q5OlAWZ1+mpn/4/+BLmFn1Akr0ESV63oVt
3WV0a4yWVur/O5xJ0qOHjp8Nz88P5xv/Ag2lYHggCQAA
```

---

# End of element construction packet

This closes the bounded element-construction follow-up only. It does not approve 5→49/100, does not change thresholds/permissions/billing, and does not resolve commodity `row+0x28`/`row+0x60` writers. Original `TEST15_NATIVE_CLOSURE_PACKET.md` preserved untouched. Scratch artifacts (`scratch/out/14001e8c0_decomp.c`, `scratch/out/14001e8c0_disasm.asm`) are read-only query results; `docs/ghidra-index/1E8C0.md` is the durable index entry; no runtime edit, build, commit or push performed.
