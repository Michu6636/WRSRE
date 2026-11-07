# Almanac: independent review of the two trade-handler exports

**Scope:** static review only. No game run, debugger, database query, hook, or implementation. The exports were fetched from commit `14c9a07fba1f09f2d8f61fef70bb08100173c77b` of the existing investigation. They are Ghidra disassembly output, not the executable; its hash and the runtime path have not been independently verified here. Addresses below are preferred-image VAs (image base `0x140000000`), not runtime addresses.

Sources: [`FUN_140373EA0` disassembly](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm) and [`FUN_140361BB0` disassembly](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm). The older `docs/almanac-investigation.md` in that commit contains conflicting claims; the review below is scoped to the cited instructions and does not validate that entire report.

## 1. Both row loops can conditionally skip entries

The earlier claim that `FUN_140373EA0` draws every sub-entry without skips is **incorrect**. In its row loop, the mapped sub-entry index is in `EBX`:

```asm
1403741cf  CMP dword ptr [RSI + 0x578],0
1403741d6  JZ  0x1403741e4
1403741d8  LEA EAX,[RBX - 3]
1403741db  CMP EAX,1
1403741de  JBE 0x1403743b6
1403741e4  CMP EBX,6
1403741e7  JNZ 0x1403741f6
1403741e9  CMP dword ptr [RSI + 0x5e0],0
1403741f0  JZ  0x1403743b6
```

A nonzero `game+0x578` skips mapped indices 3 and 4; index 6 is also skipped if `game+0x5e0` is zero. Both jumps land at the iteration increment, past the X advance at `0x1403743a8`, so skipped entries do not consume a horizontal slot. See [the mapping and skip checks](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L159-L183) and [the draw/advance/loop tail](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L260-L282).

For the conditional example of **four sub-entries**, the mapped order is `[0, 1, 3, 2]`. If `game+0x578 != 0`, index 3 is skipped, giving drawn order `[0, 1, 2]`. If the relevant skip conditions are absent, all four are drawn. This does not establish the actual runtime count or flag values.

`FUN_140361BB0` also has conditional skip checks, applied to its linear iteration index; its loop bound is the calculated sub-entry count minus one. Its X advance is likewise bypassed on a skip. See [its count and skip checks](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm#L183-L208) and [its advance and loop tail](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm#L310-L333). Thus neither handler supports a fixed button position without considering its active flags, count, scale and row state.

Additionally, both handlers test `game+0xc7d4` and branch away from their regular vector-row loops when it is not `-1`: [373EA0](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L130-L143), [361BB0](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm#L165-L181). The meaning and runtime frequency of that condition remain unknown.

## 2. A change to `game+0x10be8` is not a click event

On the path into the normal row in `FUN_140373EA0`, the handler writes `-1` to `game+0x10be8` at `0x14037403a` **before the row loop**, independent of a row click. A different early path calls another function and branches away. See [the initial comparison and reset](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L60-L82). `FUN_140361BB0` also resets the field before its normal row at `0x140361cbb`: [initial path](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm#L46-L54).

Consequences for a proposed before/after comparison: a change to `-1` can happen without a row click, and a click can write `-1` when it is already `-1`. This is a concrete false-positive and false-negative risk; the transition must not be used as the row-click signal.

## 3. Click paths differ between the two handlers

In `FUN_140373EA0`, the button collision result is tested at `0x1403742a4`–`0x1403742ac`. On a hit, a UTF-16 string-copy loop at `0x1403742d0`–`0x1403742e2` exits when `CX` is zero; there is no intervening change to `CL` before `CMP [0x140a54e91], CL` at `0x1403742f2`. Thus, on this path, the comparison is against zero. The selected-index write and `-1` write at `0x140374356` and `0x14037435c` are reached even if that index was already selected; the `game+0x1039a` write at `0x14037434f` is conditional on a change. Sound is conditional, not a dependable unique click signal. See [the complete bounded sequence](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L216-L255).

In `FUN_140361BB0`, after a successful collision check, an indirect call through `0x14086c8b8` at `0x14036213b` returns a value tested at `0x140362141`; zero branches around the selection and `-1` writes. The provided disassembly does not identify that imported function or establish whether it is an edge, a held state, or consumes input. It must not be assumed to be equivalent to the direct byte check in `373EA0`. See [the collision, gate and writes](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/ch_tab23_full.asm#L247-L298).

A future design needs an actual click signal **plus** the correct hit test, active-handler behavior, window/input gates and same-frame ordering. Cursor position alone proves only hover.

## 4. The proposed body interception remains unproven

At `0x1403745b6` the first two instructions occupy 9 and 8 bytes and use RIP-relative addresses. `InstallInlineHook` in `src/tesmioloader.cpp` supports a 17-byte stolen region, but copies those bytes to a trampoline without relocating their operands. Therefore that existing helper cannot blindly trampoline those instructions. The export shows both the `JLE` at `0x14037419c` and the `JMP` at `0x1403743ed` targeting the **start** of the site; the alternate path also falls through after `0x1403745b0`. Entry at the start is not inherently a jump into the middle of a patch, but every incoming route and any interior targets must be assessed. See [the loop exits](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L155-L158), [the regular-row exit](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L280-L285), and [the site](https://github.com/Michu6636/WRSRE/blob/14c9a07fba1f09f2d8f61fef70bb08100173c77b/export/140373ea0_1403829fd_disasm.asm#L362-L380).

A different relocation mechanism might be possible, but safe live-register preservation, stack/cleanup and output handling, skipped state effects, and the alternate handler are unverified. No hook is approved or specified here.

## Decision and next evidence

**Not ready for implementation.** The active runtime handler and state, visible bounds/clipping, native content suppression, return-to-vanilla interaction and close handoff remain unverified. The disassembly does not establish that a new same-row button fits, nor that the proposed suppression preserves vanilla behavior.

The next bounded static question is the identity and semantics of the import at `0x14086c8b8` in the existing binary's import table, plus the input ordering needed in both handlers. If the already-cached import mapping is unavailable, a targeted serialized headless query can obtain it. Any proposed runtime observation requires separate approval; there is no basis to assume it is the only way to establish bounds.
