# WRSRE — Workers & Resources: Soviet Republic Market Economy Suite

Native x64 C++17 plugin suite (`TesmioLoader` API v4) introducing a mixed state/private market economy, citizen wallets, labor mobility, wholesale & retail pricing, B2B counterparty logistics settlement, commercial banking, taxes, and money-supply reconciliation to **Workers & Resources: Soviet Republic** (`SOVIET64.exe`).

---

## Repository Layout

```text
WRSRE/
├── AGENTS.md                 # AI agent entry point & coordination rules
├── INSTRUCTIONS.md           # Project, reverse-engineering & C++ safety standards
├── KILO_AGENT_WORKFLOW.md    # Worker/Reviewer loop, delegation, evidence gates & chunking rules
├── GEMINI.md                 # Antigravity (agy) role: architect, sole Ghidra operator, Kilo delegator
├── build.bat                 # MSVC x64 plugin builder (build.bat [clean|<plugin>|loader])
├── clean.bat                 # Removes root compiler/linker temporaries
├── .kilo/
│   └── kilo.jsonc            # Kilo agents (autonomous-dev, 2 workers, 2 reviewers) & permissions
├── .github/
│   ├── instructions/wrsre.instructions.md   # Copilot repo-wide rules (applyTo: "**")
│   └── agents/               # Copilot wrsre-worker & read-only wrsre-reviewer
├── src/                      # Shared TesmioLoader host ABI, service APIs & RVA headers
├── plugins/                  # Isolated native DLL plugins (<name>/<name>.cpp + <name>.ini)
│   └── AGENTS.md             # C++ rules, loaded only when an agent touches plugins/
├── tests/                    # Standalone C++17 & Python test suites + headless mock_host
├── tools/                    # PE scanners, CSV economy auditor & Ghidra headless scripts
│   └── ghidra/               # Headless Ghidra .java query scripts (DecompAt, DisasmMany, ...)
└── docs/                     # Design docs, Ghidra index/exports, runtime evidence & tasks
    ├── AGENTS.md             # Ghidra index/export conventions, loaded only under docs/
    ├── engine/               # Reverse-engineered engine layouts & mechanics
    ├── ghidra-index/         # Function-by-RVA knowledge base (read before querying Ghidra)
    ├── ghidra-exports/       # Cached raw Ghidra decompilations, disassemblies & xrefs
    ├── private-logistics-runtime/ # Private DO logistics investigations, evidence & packets
    ├── schemas/              # Telemetry & data schemas
    ├── tasks/active/         # Active bounded engineering & verification tasks
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
| `map_expander` | Map boundary expansion utilities |
| `ui_overlay` | Top-level HUD economy & enterprise cards |
| `ui_panels` | Native building-window row injection & interactive slider/checkbox widgets |

---

## Building & Testing

### Build (Windows — Visual Studio 2022 Build Tools x64)
```bat
build.bat                     REM Build all plugins into build\plugins\
build.bat private_logistics   REM Build a single plugin
build.bat clean               REM Clean build\plugins\ and rebuild all
```

### Run Automated Tests (Cross-Platform Python 3 + C++17)
```bash
# Run all Python + C++17 plugin test suites
for f in $(find tests -name "test_*.py" | sort); do PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 "$f" || exit 1; done

# Run the standalone CSV economy ledger auditor self-test
PYTHONDONTWRITEBYTECODE=1 python3 tools/audit_economy_csv.py selftest

# Scan a new SOVIET64.exe update against baseline RVAs
python3 tools/scan_exe_update.py path/to/SOVIET64.exe
```

---

## Agent and reverse-engineering workflow

Start with [`AGENTS.md`](AGENTS.md), then the project rules in
[`INSTRUCTIONS.md`](INSTRUCTIONS.md) and the relevant sections of
[`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md). Check the actual host,
branch, tools and task authorization; Windows game and Ghidra files may not be
present in a remote checkout. Use the function index and accessible raw exports
before requesting another query. Substantive deliverables use the
Worker/Reviewer loop in `KILO_AGENT_WORKFLOW.md`: an independent child inspects
the actual diff/evidence, and the parent repairs and rechecks findings with no
round cap, stopping only for a genuine blocker. Verify
consequential claims against actual instructions or code, not a second summary,
and stop repeated report-only passes when no new evidence is being produced.

### What loads when

Only `AGENTS.md` is loaded automatically at session start. Everything else is
opt-in, deliberately: loading all of it costs roughly 16K tokens per turn.

| File | When it loads |
|---|---|
| [`AGENTS.md`](AGENTS.md) | Always, at session start |
| [`INSTRUCTIONS.md`](INSTRUCTIONS.md) | On demand — before Ghidra or C++ work |
| [`KILO_AGENT_WORKFLOW.md`](KILO_AGENT_WORKFLOW.md) | On demand — before delegating or reviewing |
| [`GEMINI.md`](GEMINI.md) | Antigravity (`agy`) sessions |
| [`.github/instructions/wrsre.instructions.md`](.github/instructions/wrsre.instructions.md) | GitHub Copilot, every file (`applyTo: "**"`) |
| [`.github/agents/`](.github/agents/) | GitHub Copilot custom agents, when selected |
| [`.kilo/kilo.jsonc`](.kilo/kilo.jsonc) | Kilo Code — agent definitions, models and permissions |
| [`docs/AGENTS.md`](docs/AGENTS.md) | Only when an agent touches `docs/` |
| [`plugins/AGENTS.md`](plugins/AGENTS.md) | Only when an agent touches `plugins/` |

The three tool layers are independent: Kilo Code reads `.kilo/kilo.jsonc`,
GitHub Copilot reads `.github/`, and Antigravity reads `GEMINI.md`. Editing one
does not change the others; all three share the same `AGENTS.md` /
`INSTRUCTIONS.md` / `KILO_AGENT_WORKFLOW.md` rules.

The existing [`tools/ghidra/`](tools/ghidra/README.md) scripts use Ghidra
headlessly; a GUI window or MCP server is not required for that workflow. MCP is
optional. For bethington GhidraMCP v7, the bridge does not load a project by
itself: it connects to a separate GUI plugin or headless backend. Other MCP
implementations can differ. Use one designated operator, protect the original
project and keep runtime tests and implementation within the user's authorization.

## Key Documentation Entry Points

- **Architecture & Design:** [`docs/WRSRE DESIGN DOC.md`](docs/WRSRE%20DESIGN%20DOC.md), [`docs/DEVELOPMENT_README.md`](docs/DEVELOPMENT_README.md), [`docs/README.md`](docs/README.md)
- **B2B Counterparty Settlement Spec:** [`docs/proposal-b2b-wholesale-counterparty-settlement.md`](docs/proposal-b2b-wholesale-counterparty-settlement.md)
- **Game Update / DLC Re-Targeting Playbook:** [`docs/DLC_UPDATE_PLAYBOOK.md`](docs/DLC_UPDATE_PLAYBOOK.md)
- **Ghidra Function Index:** [`docs/ghidra-index/README.md`](docs/ghidra-index/README.md)
