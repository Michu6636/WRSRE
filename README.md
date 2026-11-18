# WRSRE — Workers & Resources: Soviet Republic Market Economy Suite

Native x64 C++17 plugin suite (`TesmioLoader` API v4) introducing a mixed state/private market economy, citizen wallets, labor mobility, wholesale & retail pricing, B2B counterparty logistics settlement, commercial banking, taxes, and money-supply reconciliation to **Workers & Resources: Soviet Republic** (`SOVIET64.exe`).

---

## Almanac development

The in-game ImGui panel is [`plugins/almanac_overlay_dev`](plugins/almanac_overlay_dev/README.md).
Build it by name and stage it like any other plugin:

```cmd
build.bat almanac_overlay_dev
```

It draws one panel inside the game window with four tabs (Economy, Labour,
Logistics, Enterprises), a floating `Almanac` launcher button, and editable
fiscal and wage controls. Economy, Labour and Logistics are read-only telemetry;
the fiscal sliders and wage steppers **write** through the `taxes`,
`citizen_labor` and `republic_bank` service APIs, so the panel is not read-only.

`F9` shows and hides it by default (`toggle_key` in
`plugins/almanac_overlay_dev/almanac_overlay_dev.ini`; `0x77` selects `F8`,
which `ui_overlay` also uses for its HUD). The panel starts hidden.
[TESTING.md](plugins/almanac_overlay_dev/TESTING.md) lists the expected loader
lines and what each failure means. Its README explains why the earlier isolated
ImGui candidate never appeared in game.

For the wider Almanac picture — source inventory, verified evidence and
corrections, missing native integration and test limits — read the
[Almanac engineering status and handoff](docs/almanac-engineering-status.md). Do
not use the historical sidebar prototypes as the panel's launcher.

## Repository Layout

```text
WRSRE/
├── AGENTS.md                 # Lean AI entry point and lazy load map
├── INSTRUCTIONS.md           # Project, reverse-engineering & C++ safety standards
├── KILO_AGENT_WORKFLOW.md    # Compatibility router + native-evidence/chunking reference
├── GEMINI.md                 # Antigravity Architect/Ghidra Lead/Final Auditor adapter
├── .kilo/                    # Tracked Kilo config and referenced prompt bodies
├── .github/                  # Copilot instructions and worker/reviewer agents
├── build.bat                 # MSVC x64 plugin builder (build.bat [clean|<plugin>|loader|wrsre])
├── clean.bat                 # Removes root compiler/linker temporaries
├── src/                      # Shared TesmioLoader host ABI, service APIs & RVA headers
├── plugins/                  # Isolated native DLL plugins (<name>/<name>.cpp + <name>.ini)
├── tests/                    # Standalone C++17 & Python test suites + headless mock_host
├── tools/                    # PE scanners, CSV economy auditor & Ghidra headless scripts
│   └── ghidra/               # Headless Ghidra .java query scripts (DecompAt, DisasmMany, ...)
├── prototypes/               # Historical Almanac overlay prototypes (legacy, not built by default)
├── export/                   # Cached Ghidra/PE evidence bundles (almanac-evidence/, analyses)
├── "Mods for analysis"/      # Vendored upstream mod sources reviewed read-only (see its README)
├── "Autosave bin files"/     # Captured private_logistics_costs.bin save fixtures
└── docs/                     # Design docs, Ghidra index/exports, runtime evidence & tasks
    ├── engine/               # Reverse-engineered engine layouts & mechanics
    ├── ghidra-index/         # Function-by-RVA knowledge base (read before querying Ghidra)
    ├── ghidra-exports/       # Cached raw Ghidra decompilations, disassemblies & xrefs
    ├── private-logistics-runtime/ # Private DO logistics investigations, evidence & packets
    ├── schemas/              # Telemetry & data schemas
    ├── workflow/             # Canonical execution, review, Kilo and provider contracts
    ├── tasks/                # Tracked task records, Finding Ledgers and TEMPLATE.md
    │   ├── active/           # Current/historical task packets (check each status)
    │   └── archive/          # Completed/retired records with final dispositions
    └── visions/              # Whole-mod & subsystem design visions
```

---

## Plugin Suite (`plugins/`)

| Plugin | Role |
|---|---|
| `sim_clock` | In-game tick, day, month, quarter, and year scheduling service |
| `persistence` | Atomic save-directory file I/O (`writeAtomic`/`readFile`) & save/load dispatch |
| `currency` | Dual-currency (`RUB`/`USD`) citizen wallets, Treasury accessors & flow observers |
| `resource_pricing` | Resource price table, Republic supply/demand multipliers, CPI basket & v4 local scarcity quotes |
| `enterprise_base` | Private company registry (`[P]`), balance sheets, P&L categories, rent & investment pool |
| `republic_bank` | Working-capital credit lines, daily overdraft rescue, monthly interest/sweep & City Hall rate slider |
| `citizen_labor` | Wage negotiation, skill premiums, payroll remittance & labor mobility |
| `citizen_retail` | Household retail checkout gate, CPI survival basket & store revenue settlement |
| `private_shops` | Retail/service operations, foreign restock & macro supply/demand recording |
| `private_logistics` | Autonomous private DO target management, fuel/schedule observers & Ship 2 B2B counterparty settlement |
| `private_logistics_probe` | Read-only vehicle fuel/snapshot diagnostic telemetry |
| `private_construction` | Private construction financing, site progress & turnkey ownership transfer |
| `taxes` | VAT, corporate profit tax, property tax & City Hall fiscal policy sliders |
| `state_ledger` | Monthly Treasury & full Republic money-supply reconciliation (`tesmio_ledger.csv`) |
| `ui_overlay` | Top-level HUD economy & enterprise cards |
| `ui_panels` | Native building-window row injection & interactive slider/checkbox widgets |
| `almanac_overlay_dev` | In-game ImGui panel: Economy/Labour/Logistics telemetry, Enterprises directory & fiscal controls |
| `almanac_button` | Experimental ninth sidebar tile (prototype; superseded by `almanac_overlay_dev`) |

Not part of the `wrsre.dll` monolith: `almanac_overlay_dev`, `almanac_button`
and `private_logistics_probe` are standalone-only DLLs. `almanac_overlay_dev`
links Dear ImGui and is skipped by a plain `build.bat` when the pinned dependency
is unavailable. `almanac_button` is a disposable test build that ends the game
process on a failed guard — do not stage it in a normal install.

---

## Building & Testing

### Build (Windows — Visual Studio 2022 Build Tools x64)
```bat
build.bat wrsre               REM Build single-DLL wrsre.dll + all INIs (VS Code Ctrl+Shift+B default)
build.bat clean wrsre         REM Clean build\plugins\ and rebuild wrsre.dll
build.bat                     REM Build all separate plugin DLLs into build\plugins\
build.bat private_logistics   REM Build a single plugin DLL
build.bat almanac_overlay_dev REM Build the Almanac ImGui panel by name
build.bat loader              REM Build upstream tesmioloader.dll (opt-in)
build.bat clean               REM Remove build\plugins\ before building
```

> **Staging caution.** `build.bat` and `build.bat wrsre` write to the same
> `build\plugins\` directory. Stage `wrsre.dll` **or** the separate plugin DLLs,
> never both: copying `build\plugins\*` after both builds ships every plugin
> twice (once monolithic, once standalone). Run `build.bat clean` between them.

The pinned Dear ImGui version is declared **once**, as `IMGUI_TAG` and
`IMGUI_NUM` at the top of `build.bat`. `plugins/almanac_overlay_dev/almanac_version.h`
carries the same tag for the panel's version string;
`tests/agent_workflow/test_imgui_pin.py` fails if either drifts or if a literal
version reappears in `build.bat`. Bump all of it in one change.

### Run Automated Tests (Cross-Platform Python 3 + C++17)
```bash
# Run all Python + C++17 plugin test suites
for f in $(find tests -name "test_*.py" | sort); do PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 "$f" || exit 1; done

# Almanac host tests: compiles the panel's headers with g++/clang and runs them.
# Needs a host C++17 compiler; run_native_dependency_tests.py additionally needs
# --imgui-dir <checkout> and a host CMake.
PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 tests/almanac_button/run_host_tests.py
PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 tests/almanac_button/run_binary_sha256_tests.py
PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 tests/almanac_button/run_tax_policy_tests.py
PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 tests/almanac_overlay_dev/run_host_tests.py

# Tool self-tests
PYTHONDONTWRITEBYTECODE=1 python3 tools/audit_economy_csv.py selftest
PYTHONDONTWRITEBYTECODE=1 python3 tools/peinfo.py selftest

# Scan a new SOVIET64.exe update against baseline RVAs
python3 tools/scan_exe_update.py path/to/SOVIET64.exe
```

> **One test needs real Windows.** `tests/currency/test_transactions.py` contains
> `test_real_native_write_faults`, which compiles a probe with `cl /W4 /WX` and
> exercises `CommitNative`'s rollback through genuine SEH (`__try`/`__except`
> cannot be emulated on Linux). On any other platform it is **skipped**, so a
> green Linux run says nothing about it. CI runs the whole Python suite set on
> `windows-latest` as well, and fails if that test silently skips there too.

> These are host checks. None of them is a live-game test: STATIC REVIEW,
> BUILD/TEST EVIDENCE and RUNTIME VALIDATION are separate statuses, and a
> passing suite does not prove the panel or the plugins work in `SOVIET64.exe`.

### Continuous Integration

[`.github/workflows/ci.yml`](.github/workflows/ci.yml) runs the same commands on
every push and pull request:

| Job | Runner | What it proves |
|---|---|---|
| `test` | ubuntu | all Python suites, all four Almanac host C++ runners, tool self-tests. Warns if a passing suite **skipped** tests on this platform. |
| `hygiene` | ubuntu | no NUL/invalid UTF-8 in tracked text (honours `.gitattributes` `-text`), no broken local Markdown links, `build.bat` is CRLF-only and paren-balanced, the ImGui pin is consistent |
| `Windows (MSVC + SEH)` | windows | `build.bat` and `build.bat clean wrsre` compile **and** the Python suites run where `test_real_native_write_faults` can execute against real SEH |

All three gate merges. The Windows job is not optional coverage: it is the only
place `test_real_native_write_faults` runs at all, and it fails loudly if that
test silently skips there (e.g. `cl` missing from `PATH`) rather than reporting
a green run that measured nothing.

---

## Agent and reverse-engineering workflow

Start with the lean [`AGENTS.md`](AGENTS.md) load map. For an autonomous goal,
use [`docs/workflow/EXECUTION.md`](docs/workflow/EXECUTION.md),
[`docs/workflow/REVIEW.md`](docs/workflow/REVIEW.md) and a tracked record from
[`docs/tasks/TEMPLATE.md`](docs/tasks/TEMPLATE.md). Kilo setup and provider
handoff are in [`docs/workflow/KILO.md`](docs/workflow/KILO.md) and
[`docs/workflow/PROVIDERS.md`](docs/workflow/PROVIDERS.md). Load
[`INSTRUCTIONS.md`](INSTRUCTIONS.md) and the relevant native-evidence sections of
[`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md) only when needed.

The lead runs Investigate → Plan → Implement → Build/Test → Review → Repair
without a repository round cap. Both strictly read-only reviewers inspect all
three stages; every prior in-scope finding is reconciled in a Finding Ledger.
Check the actual host, branch, tools and authorization; Windows game/Ghidra
files may be absent remotely. Reuse the function index and tracked raw exports
before querying, and stop repeated report-only passes when no new evidence is
being produced. Static review, build/test evidence and live runtime validation
are separate statuses.

The existing [`tools/ghidra/`](tools/ghidra/README.md) scripts use Ghidra
headlessly; a GUI window or MCP server is not required for that workflow. MCP is
optional. For bethington GhidraMCP v7, the bridge does not load a project by
itself: it connects to a separate GUI plugin or headless backend. Other MCP
implementations can differ. Use one designated operator, protect the original
project and keep runtime tests and implementation within the user's authorization.

## Key Documentation Entry Points

- **Architecture & Design:** [`docs/SINGLE_DLL_ARCHITECTURE.md`](docs/SINGLE_DLL_ARCHITECTURE.md), [`docs/WRSRE DESIGN DOC.md`](docs/WRSRE%20DESIGN%20DOC.md), [`docs/DEVELOPMENT_README.md`](docs/DEVELOPMENT_README.md), [`docs/README.md`](docs/README.md)
- **B2B Counterparty Settlement Spec:** [`docs/proposal-b2b-wholesale-counterparty-settlement.md`](docs/proposal-b2b-wholesale-counterparty-settlement.md)
- **Game Update / DLC Re-Targeting Playbook:** [`docs/DLC_UPDATE_PLAYBOOK.md`](docs/DLC_UPDATE_PLAYBOOK.md)
- **Ghidra Function Index:** [`docs/ghidra-index/README.md`](docs/ghidra-index/README.md)
