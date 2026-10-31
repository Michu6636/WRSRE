# FUN_1403FBDC0 — Generic Detach Helper (Metadata-Kind Check)

**RVA:** `0x3FBDC0`
**Signature:** `void FUN_1403fbdc0(longlong vehicle)` (RCX=vehicle)
**Purpose:** Generic detach helper. Loads vehicle+0xC80; if null, jumps to clear. If vehicle+0x1708 exists and [+0x294]==10, calls FUN_1406DCF20. Otherwise (ordinary): clears office+0xBB0 unconditionally, then clears vehicle+0xC80. Called by type-0x2B updater during config processing, but call alone is not proven task removal.
**Callers:** Type-0x2B updater (config processing), other metadata paths
**Data Refs:** `vehicle+0xC80`, `vehicle+0x1708`, `vehicle+0x294`, `vehicle+0x4F0` (office), `office+0xBB0`
**Artifact:** `scratch/disasm_3fbdc0.txt`, `scratch/xrefs_3fbdc0.txt`
**Notes:** Metadata-kind 10 path calls FUN_1406DCF20 (token-compare clear). Ordinary path clears office+0xBB0 unconditionally then vehicle+0xC80. Not unique to type-0x2B; call alone ≠ task removal boundary.
**Status:** `verified`