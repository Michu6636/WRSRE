# WRSRE Documentation Index

**Game target:** Workers & Resources: Soviet Republic (`SOVIET64.exe` baseline v1.1.1.9, PE timestamp `0x6A3EB6AD`; ASLR enabled, preferred Ghidra image base `0x140000000`).

---

## Directory Map

| Path | Contents |
|---|---|
| [`WRSRE DESIGN DOC.md`](WRSRE%20DESIGN%20DOC.md) | Master economic architecture, subsystem boundaries & design invariants |
| [`DEVELOPMENT_README.md`](DEVELOPMENT_README.md) | Developer onboarding, plugin ABI conventions & debugging workflow |
| [`DLC_UPDATE_PLAYBOOK.md`](DLC_UPDATE_PLAYBOOK.md) | Re-targeting RVAs/offsets after game updates via `tools/scan_exe_update.py` |
| [`proposal-b2b-wholesale-counterparty-settlement.md`](proposal-b2b-wholesale-counterparty-settlement.md) | Ship 2 B2B vehicle-provenance counterparty settlement specification |
| [`engine/`](engine/README.md) | Reverse-engineered engine structs, building vectors, local markets & fuel mechanics |
| [`ghidra-index/`](ghidra-index/README.md) | Function-by-RVA reverse-engineering ledger (`verified` / `partial` / `contested`) |
| [`ghidra-exports/`](ghidra-exports/README.md) | Cached raw `.c`, `.asm`, and `.txt` Ghidra exports |
| [`private-logistics-runtime/`](private-logistics-runtime/README.md) | Autonomous DO logistics investigations, runtime evidence & [`packets/`](private-logistics-runtime/packets/README.md) |
| [`schemas/`](schemas/telemetry_schema.json) | JSON schemas for telemetry & diagnostics |
| [`workflow/`](workflow/README.md) | Canonical goal execution, independent review, Kilo setup and provider handoff contracts |
| [`tasks/`](tasks/README.md) | Tracked task records, Finding Ledgers, [`TEMPLATE.md`](tasks/TEMPLATE.md), active packets and archive |
| [`visions/`](visions/README.md) | Long-term design visions (`vision-whole-mod.md`, `vision-private-logistics.md`) |
| `Archive dont read/` | Historical early-phase notes superseded by current docs |

---

## Subsystem Investigations & Plans

- **Labor & Wages:** [`Labor_and_Salary_Overhaul_Plan.md`](Labor_and_Salary_Overhaul_Plan.md), [`Labor_and_Salary_Overhaul_Specification.md`](Labor_and_Salary_Overhaul_Specification.md), [`Labor_Mobility_Investigation.md`](Labor_Mobility_Investigation.md)
- **Retail, CPI & Standard of Living:** [`CPI_Survival_Basket_Investigation.md`](CPI_Survival_Basket_Investigation.md), [`Retail_Purchase_Block_Investigation.md`](Retail_Purchase_Block_Investigation.md), [`Citizen_SoL_Investigation.md`](Citizen_SoL_Investigation.md)
- **Housing & Rent:** [`Rent_Housing_Task1_Investigation.md`](Rent_Housing_Task1_Investigation.md), [`Citizen_Housing_Relocation.md`](Citizen_Housing_Relocation.md)
- **Logistics, Fuel & Storage:** [`ghidra-do-storage-findings.md`](ghidra-do-storage-findings.md), [`implementation-plan-per-trip-fuel.md`](implementation-plan-per-trip-fuel.md), [`review-per-trip-fuel-plan.md`](review-per-trip-fuel-plan.md), [`ASSOCIATION_REVIEW_COMPLETE.md`](ASSOCIATION_REVIEW_COMPLETE.md), [`FUEL PLAN/`](FUEL%20PLAN/association-investigation/report.md)
- **Warehouses & UI Panels:** [`UI investigation.md`](UI%20investigation.md), [`analysis-warehouse-checkbox-2026-10-23.md`](analysis-warehouse-checkbox-2026-10-23.md), [`ghidra-findings-warehouse-storage-v6.15.md`](ghidra-findings-warehouse-storage-v6.15.md), [`ghidra-findings-warehouse-builder-2026-10-23.md`](ghidra-findings-warehouse-builder-2026-10-23.md)
- **Almanac:** [Product and delivery plan](almanac-plan.md), [current technical findings](almanac-technical-findings.md), [third-party DLL research approach](almanac-dll-research.md), [initial Frontline triage](almanac-frontline-triage.md), [Reputation evidence review](almanac-reputation-review.md), [trade-trace review](almanac-trade-trace-review.md), [sidebar write review](almanac-sidebar-write-review.md), [button approach review](almanac-button-approach-review.md), [content-boundary review](almanac-content-boundary-review.md), [native-click review](almanac-click-close-review.md), [click-test plan review](almanac-click-test-plan-review.md), [Reputation agent comparison](almanac-agent-comparison-review.md), [design comparison review](almanac-design-comparison-review.md), [PR #67 review](almanac-pr67-review.md), [draw-only prototype review](almanac-prototype-6aa15e0-review.md), [draw-only prototype visual-check plan](almanac-prototype-b5c207d-visual-test-plan.md), [City Manager UI reference review](../export/almanac-evidence/city-manager-ui-review.md), [snapshot address correction](almanac-snapshot-review.md), the detailed [independent export review](almanac-export-review.md), and the [asset-free MVP design comparison and recommendation (2026-11-13)](almanac-mvp-design-2026-11-13.md). Older investigation documents contain superseded conclusions; use the current findings and review first.
- **Map Expansion:** [`Map_Size_Investigation.md`](Map_Size_Investigation.md), [`Map_Expander_Test.md`](Map_Expander_Test.md)
