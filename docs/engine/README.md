# Engine investigations index

Canonical subsystem pages for native reverse-engineering knowledge. Historical
target: `SOVIET64.exe` v1.1.1.9 (`0x6A3EB6AD`); verify the actual task binary and
project identity before reuse. A page's existence or an entry-wide `verified`
label is not blanket proof of semantics or runtime behavior.

## How to use and extend

1. Find the subsystem below, then read its current page, relevant
   [`../ghidra-index/`](../ghidra-index/README.md) entries and bounded
   [`../ghidra-exports/`](../ghidra-exports/README.md). Reuse accessible evidence
   before querying; one named Ghidra operator owns the project.
2. For a new/subsystem update, use [`TEMPLATE.md`](TEMPLATE.md). Separate raw
   observation, inference, contested interpretation and runtime unknowns. Link
   exact tracked artifact ranges and update the function index after successes
   and failures.
3. Correct misleading current summaries and dependent conclusions in place with
   a concise supersession note. Keep historical material only when its status is
   unambiguous. Never leave ignored `scratch/` as the sole proof.

## Existing pages

- [ghidra-do-storage-findings.md](../ghidra-do-storage-findings.md) — canonical DO TaskEntry/TaskCfg (`0xD38/0xD40` stride `0x18`, `0x198` via `0x1512A0`, dirty `+0xD50`/`+0xFC0`, save `1EDC40/6C5350` load `1F0BE0`)
- [building-vectors.md](building-vectors.md) — `0xBC0` workers vs `0xBD8` visitors (factory vs shop tick)
- [private-industry-re.md](private-industry-re.md) — attribution of goods movement (vehicle+0x4F8 home, +0x4F0 current, cargo 0x3B8)
- [logistics.md](logistics.md) — superseded DO overview; use ghidra-do-storage-findings as source of truth
- [local-markets.md](local-markets.md) — town `0x108` NamePoint, aggregator `0xC858/0xC898`
- [vehicle-fuel-system.md](vehicle-fuel-system.md) — historical fuel attribution; read its correction notice before reuse
- [almanac-ui.md](almanac-ui.md) — Almanac UI & plugin exports; check current findings/reviews for superseded conclusions
- [native-verification-results.md](native-verification-results.md) — recorded native verdicts and affected review findings

## Documentation coverage map

This is an inventory of where knowledge lives, **not** a new verification claim.
“Gap” means no canonical engine page was identified in this audit; it does not
mean the subsystem is unimplemented or safe to change.

| Subsystem | Starting documents | Coverage note |
|---|---|---|
| Buildings / workers / visitors | [building-vectors.md](building-vectors.md), [private-industry-re.md](private-industry-re.md) | Partial; check supersession and raw-artifact availability |
| Citizens / labor / housing | [native-verification-results.md](native-verification-results.md), [../Labor_Mobility_Investigation.md](../Labor_Mobility_Investigation.md), [../Citizen_Housing_Relocation.md](../Citizen_Housing_Relocation.md) | Investigation reports; promote durable contracts into an engine page when verified |
| Markets / retail / CPI | [local-markets.md](local-markets.md), [../CPI_Survival_Basket_Investigation.md](../CPI_Survival_Basket_Investigation.md), [../Retail_Purchase_Block_Investigation.md](../Retail_Purchase_Block_Investigation.md) | Partial; some older pages cite local-only scratch evidence |
| Logistics / DO / storage | [../ghidra-do-storage-findings.md](../ghidra-do-storage-findings.md), [../private-logistics-runtime/README.md](../private-logistics-runtime/README.md) | Active evidence chain; [logistics.md](logistics.md) is superseded in part |
| Vehicle fuel | [vehicle-fuel-system.md](vehicle-fuel-system.md), [../private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md](../private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md) | Historical attribution corrected; runtime applicability remains claim-specific |
| UI / panels / Almanac | [almanac-ui.md](almanac-ui.md), [../almanac-technical-findings.md](../almanac-technical-findings.md), [../UI investigation.md](../UI%20investigation.md) | Partial; use current reviews before older conclusions |
| Map expansion | [../Map_Size_Investigation.md](../Map_Size_Investigation.md), [../Map_Expander_Test.md](../Map_Expander_Test.md) | Investigation/test notes; no canonical engine page identified |
| Save/load and persistence | [../DEVELOPMENT_README.md](../DEVELOPMENT_README.md), relevant function index entries | Gap for a subsystem-level engine contract |
| Currency / taxes / macro economy | [../WRSRE DESIGN DOC.md](../WRSRE%20DESIGN%20DOC.md), relevant plugin sources | Design documentation is not native RE proof; engine-level gap |
