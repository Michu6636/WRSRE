# `Autosave bin files` — captured save fixtures

Two `private_logistics_costs.bin` files written by the `private_logistics`
plugin during a real save cycle, captured on 2026-11-04.

| Path | Size |
|---|---|
| `autosave2/private_logistics_costs.bin` | 16 KB |
| `autosave4/private_logistics_costs.bin` | 12 KB |

They are real serialised plugin state, kept as a reference for the persisted
record layout. Nothing in `build.bat`, `plugins/` or `tests/` reads them, and no
tracked document cites them yet.

## Treat as untrusted input

These are opaque binary blobs captured from a running game. If a future test
parses them, treat the input as hostile: bound every length, validate every
count and reject trailing bytes rather than trusting the header. Do not assume
`schema_version` or record count from the file size alone.

## Not a build input

Do not stage these into a game save. They are read-only fixtures for
understanding the on-disk format, not restorable save data.
