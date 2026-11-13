# Independent review of the shared evidence

The original files and manifest were copied from commit `7f8e65a3ab7ad44e739b4333cb9720e704b9d6ca`; the original report and manifest are retained verbatim as repository blobs, not silently corrected. The manifest identifies the executable hash as reported/verified in an earlier diagnostic, not independently demonstrated as the imported Ghidra program.

## Scope and caveats

- The constructor exports cover only `0x1402e6c80–0x1402e8200` (with overlap at the boundary), not the full function or the earlier reported zero-initialization. The trade transfer calls `0x140459530`, whose body is not included.
- The five-byte range in `raw_bytes_0x1402e7cf0-0x1402e7cf4.txt` contains only the first two bytes of the call at `0x1402e7cf3`; the constructor disassembly includes all five call bytes.
- The close report incorrectly states that the else block at `0x14031e673` runs when the input return is zero. `0x14031e64e` actually jumps to `0x14031e69e` on zero; the earlier collision failure at `0x14031e619` branches to `0x14031e673`.
- The report does not prove all routes execute `0x140326e6d`: the conditional jump at `0x14032670a` bypasses it, and `0x14032092a` bypasses the earlier zeroing at `0x14032642a`. These are not proof of a nonzero R15 at the writes; trace prior register values and all relevant branches. When the flag is nonzero, a null selected entry skips comparisons, not the final writes.
- The original manifest was made in a Windows working tree. Six raw files reproduce their listed SHA-256 values after converting the repository LF line endings back to CRLF. The close report hash matches neither its LF bytes nor a simple CRLF conversion; its original working-tree bytes cannot be reconstructed from this evidence. Do not treat the manifest alone as validation of that report.

## SHA-256 of repository-normalized files

These hashes describe the files as copied into this LF-normalized checkout; they do not authenticate the executable or establish the semantics of the disassembly.

- `FUN_1402e6bc0_0x1402e6c80-0x1402e7200.asm`: `9c0f37cf22c253ee2b1226c2f26d2e958cb4029077f2b92d4834e94102b713aa`
- `FUN_1402e6bc0_0x1402e7200-0x1402e8200.asm`: `b866ea938e377582751c5ed3b40536ea305414e7e80b18961a509c2ea2b1176e`
- `FUN_1404492f0_disasm.asm`: `166de87514d03159955e18e1d98650f4b27c3af5ccc81769cbfc3db16c7c1635`
- `MANIFEST.txt`: `2f125bf3b92547ed3daaaa891cc680d88a1c88b4b04119959ee37ec279d7da93`
- `close_trace_report.txt`: `cfe0f23bbefa2b988671e8e79de5a8cf2bef051230e70a4d2638024758633701`
- `hexdump_0x1408a7d1d-0x1408a7d29_and_0x1408a7dfc-0x1408a7e0f.txt`: `2e2aa4b4d893dbb3cbb69c137dc5119acc558a806c8a8da3afb27a145eb01d40`
- `raw_bytes_0x1402e7cf0-0x1402e7cf4.txt`: `2303c09ce4239fe3cf6ec46439cf61dfb644834b1391656ff30a21ea007950e4`
- `strings_0x1408793fc_and_0x1408b3238.txt`: `9c5c4a756a0b651238d38ebc5415ffe51af23c84b2f49993416036275514c11b`
