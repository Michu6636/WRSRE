# Engine Investigations Index

Native reverse-engineering findings for `SOVIET64.exe` v1.1.1.9 (`0x6A3EB6AD`).

- [ghidra-do-storage-findings.md](../ghidra-do-storage-findings.md) — canonical DO TaskEntry/TaskCfg (`0xD38/0xD40` stride `0x18`, `0x198` via `0x1512A0`, dirty `+0xD50`/`+0xFC0`, save `1EDC40/6C5350` load `1F0BE0`)
- [building-vectors.md](building-vectors.md) — `0xBC0` workers vs `0xBD8` visitors (factory vs shop tick)
- [private-industry-re.md](private-industry-re.md) — attribution of goods movement (vehicle+0x4F8 home, +0x4F0 current, cargo 0x3B8)
- [logistics.md](logistics.md) — superseded DO overview; use ghidra-do-storage-findings as source of truth
- [local-markets.md](local-markets.md) — town `0x108` NamePoint, aggregator `0xC858/0xC898`
- [vehicle-fuel-system.md](vehicle-fuel-system.md) — cargo slot `+0x08` tons, burn/refuel via `0x420350`
- [almanac-ui.md](almanac-ui.md) — Almanac UI & Plugin Exports (Economy & Trade handler 0x373EA0, C3DDLL64 texture/font exports, middlepoint 0x9EACD0, hook point 0x3745B6)
