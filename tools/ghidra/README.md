# Headless Ghidra Scripts (`tools/ghidra/`)

Reusable Java scripts for `analyzeHeadless` against `SOVIET64.exe` (relocated from the former root `scratch_clean/` folder).

See [`INSTRUCTIONS.md`](../../INSTRUCTIONS.md) §1 for mandatory serialization rules (`ONE RUN AT A TIME`) and `docs/ghidra-index/` lookup requirements before running any query.

| Script | Purpose |
|---|---|
| `DecompAt.java` | Decompiles one or more functions at preferred VAs (`0x140000000 + RVA`) to a `.c` file |
| `DisasmMany.java` | Disassembles instruction ranges (`0x140start:0x140end`) with raw bytes to an `.asm` file |
| `FindDisp.java` | Searches functions/instructions referencing a struct displacement |
| `dump_all_strings.java` | Dumps narrow ASCII/UTF-8 string literals from program memory |
| `dump_wide_strings.java` | Dumps UTF-16LE wide string literals from program memory |
