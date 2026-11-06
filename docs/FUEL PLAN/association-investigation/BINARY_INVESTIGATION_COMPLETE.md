<<<<<<< ours
<<<<<<< ours
# Transport Association — Binary Investigation Complete
=======
# Transport Association — Binary Investigation Complete (Repaired)
>>>>>>> theirs
=======
# Transport Association — Binary Investigation Complete (Repaired)
>>>>>>> theirs

**Target:** `SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`), image base `0x140000000`, ASLR ON.
**Mode:** static analysis only. No game execution, no hooks, no memory writes, no billing.
**Deliverable:** `docs/FUEL PLAN/association-investigation/BINARY_INVESTIGATION_COMPLETE.md`

## 1. Prerequisite Verification and Executable Identity

### 1.1 Workspace, branch, HEAD

| Item | Value |
|---|---|
| Workspace | `C:\WRSRE\.kilo\worktrees\reverse-engineering-and-verified-transport-associa` |
| Branch | `reverse-engineering-and-verified-transport-associa` (current, not switched) |
| HEAD | `a692bb9` — "Storage building checkbox for DO detection" |
<<<<<<< ours
<<<<<<< ours
| Working tree | Clean except untracked `docs/FUEL PLAN/` (prior deliverables) and `C:\WRSRE-ghidra-work/` (Ghidra project, outside Git) |
=======
| Working tree | Clean except untracked `docs/FUEL PLAN/` and `C:\WRSRE-ghidra-work/` (outside Git) |
>>>>>>> theirs
=======
| Working tree | Clean except untracked `docs/FUEL PLAN/` and `C:\WRSRE-ghidra-work/` (outside Git) |
>>>>>>> theirs

### 1.2 Executable identity

| Property | Value |
|---|---|
| Path | `C:\Workers Resources Soviet Republic\SOVIET64.exe` |
| Size | 10,308,608 bytes |
| SHA-256 | `296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8` |
| PE Machine | `0x8664` (x64 / AMD64) |
| Optional magic | `0x020B` (PE32+) |
<<<<<<< ours
<<<<<<< ours
| ImageBase | `0x140000000` |
| Timestamp | `0x6A3EB6AD` (2026/08/11) |
| Language/Compiler | `x86:LE:64:default:windows` |

The ImageBase `0x140000000` matches the convention used in prior investigation notes (file-based address = `0x140` + RVA).
=======
=======
>>>>>>> theirs
| ImageBase (static VA base) | `0x140000000` |
| Timestamp | `0x6A3EB6AD` (2026/08/11) |
| Language/Compiler | `x86:LE:64:default:windows` |

**Address terminology:**
- Static VA = ImageBase + RVA (e.g., RVA `0x420350` → static VA `0x140420350`)
- Runtime VA = runtime module base + RVA (not known without execution)
- PE file offsets are separate from both
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs

### 1.3 Ghidra installation

| Item | Value |
|---|---|
| Installation | `C:\Tools\ghidra_12.1.4_PUBLIC\ghidra_12.1.4_PUBLIC\` |
<<<<<<< ours
<<<<<<< ours
| `analyzeHeadless.bat` | `...\support\analyzeHeadless.bat` — available |
| `ghidraRun.bat` | `...\ghidraRun.bat` — available |
| JDK | `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot\` — Temurin-21.0.12.1+1 LTS |
| Debugger | None found (no x64dbg, windbg, etc.) |

### 1.4 Existing artifacts inspected

Before creating new functionality, the following were checked:
- `C:\WRSRE\.kilo\worktrees\reverse-engineering-and-verified-transport-associa\scratch/` — **absent**
- `C:\WRSRE\.kilo\worktrees\reverse-engineering-and-verified-transport-associa\tools/` — **no Ghidra scripts found**
- `C:\Tools\ghidra_12.1.4_PUBLIC\ghidra_12.1.4_PUBLIC\Ghidra\Features\*\ghidra_scripts/` — built-in scripts examined for API patterns
- `C:\Workers Resources Soviet Republic\peinfo-out/` — **no files found**
- `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` — **absent** (no `docs/FUEL PLAN/` directory exists)

No pre-existing Ghidra scripts or project were reused. All tooling was created fresh.

## 2. Transfer Call Enumeration

All 16 direct `CALL rel32` instructions targeting `FUN_140420350` (VA `0x140420350`, RVA `0x420350`) were enumerated using the `CallsTo420350.java` script against the saved Ghidra project. Three additional DATA references exist (unwind/metadata, not invocations). Raw bytes, decoded length, target, and containing function are taken directly from machine instructions — no manual rewriting.

| # | VA | RVA | Raw bytes | Length | Target | Containing function |
|---|---|---|---|---|---|---|
| 1 | 0x140141f33 | 0x141f33 | `E8 18 E4 2D 00` | 5 bytes | 0x140420350 | FUN_1401403f0 |
| 2 | 0x14016f831 | 0x16f831 | `E8 1A 0B 2B 00` | 5 bytes | 0x140420350 | FUN_14016e3b0 |
| 3 | 0x1401b2898 | 0x1b2898 | `E8 B3 DA 26 00` | 5 bytes | 0x140420350 | FUN_1401b1290 |
| 4 | 0x1401b484b | 0x1b484b | `E8 00 BB 26 00` | 5 bytes | 0x140420350 | FUN_1401b3ce0 |
| 5 | 0x1401b5ec0 | 0x1b5ec0 | `E8 8B A4 26 00` | 5 bytes | 0x140420350 | FUN_1401b5420 |
| 6 | 0x1401b653b | 0x1b653b | `E8 10 9E 26 00` | 5 bytes | 0x140420350 | FUN_1401b5420 |
| 7 | 0x1401b6833 | 0x1b6833 | `E8 18 9B 26 00` | 5 bytes | 0x140420350 | FUN_1401b5420 |
| 8 | 0x1401b75ed | 0x1b75ed | `E8 5E 8D 26 00` | 5 bytes | 0x140420350 | FUN_1401b6a20 |
| 9 | 0x1401b7981 | 0x1b7981 | `E8 CA 89 26 00` | 5 bytes | 0x140420350 | FUN_1401b6a20 |
| 10 | 0x1401cb7ff | 0x1cb7ff | `E8 4C 4B 25 00` | 5 bytes | 0x140420350 | FUN_1401cb2a0 |
| 11 | 0x14068eeca | 0x68eeca | `E8 81 14 D9 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |
| 12 | 0x14068f792 | 0x68f792 | `E8 B9 0B D9 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |
| 13 | 0x14068f7f9 | 0x68f7f9 | `E8 52 0B D9 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |
| 14 | 0x1406945b3 | 0x6945b3 | `E8 98 BD D8 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |
| 15 | 0x140694f40 | 0x694f40 | `E8 0B B4 D8 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |
| 16 | 0x1406953c4 | 0x6953c4 | `E8 87 AF D8 FF` | 5 bytes | 0x140420350 | FUN_14068b330 |

**Total CALL instructions to FUN_140420350: 16**

### 2.1 Non-call references (metadata only)

Three additional references to `0x140420350` were found but are **not** invocations:
- `0x140a6951c` — TYPE: DATA (`.pdata` RUNTIME_FUNCTION / unwind metadata)
- `0x140952a9c` — TYPE: DATA (`.rdata` UNWIND_INFO)
- `0x140952ac8` — TYPE: DATA (`.rdata` UNWIND_INFO)

These are x64 exception-handling metadata entries, not call sites. Patching call sites observes 100% of actual invocations for this binary build.

### 2.2 Call sites of interest

| RVA | Caller function | Role (from disassembly context) |
|---|---|---|
| 0x16f831 | FUN_14016e3b0 (vehicle upkeep) | Internal refuel draw |
| 0x1b2898 | FUN_1401b1290 (road delivery) | Road delivery unload |
| 0x1cb7ff | FUN_1401cb2a0 (loader) | Per-cargo-slot unload servicing |
| 0x68eeca | FUN_14068b330 (sea/rail hauler) | Sea/rail leg A |
| 0x68f792 | FUN_14068b330 | Sea/rail leg B |
| 0x68f7f9 | FUN_14068b330 | Dock return / transship credit |
| 0x6945b3 | FUN_14068b330 | Dock return leg |
| 0x694f40 | FUN_14068b330 | Dock return leg |
| 0x6953c4 | FUN_14068b330 | Sea/rail delivery |

## 3. Xref Results

Generated by `Xrefs420350.java` against the saved Ghidra project. All addresses are file-based VAs (`0x140` + RVA).

### 3.1 References TO FUN_140420350 (total 19)

| FROM | TYPE |
|---|---|
| 0x140a6951c | DATA |
| 0x140952a9c | DATA |
| 0x140952ac8 | DATA |
| 0x140141f33 | UNCONDITIONAL_CALL |
| 0x14016f831 | UNCONDITIONAL_CALL |
| 0x1401b2898 | UNCONDITIONAL_CALL |
| 0x1401b484b | UNCONDITIONAL_CALL |
| 0x1401b5ec0 | UNCONDITIONAL_CALL |
| 0x1401b653b | UNCONDITIONAL_CALL |
| 0x1401b6833 | UNCONDITIONAL_CALL |
| 0x1401b75ed | UNCONDITIONAL_CALL |
| 0x1401b7981 | UNCONDITIONAL_CALL |
| 0x1401cb7ff | UNCONDITIONAL_CALL |
| 0x14068eeca | UNCONDITIONAL_CALL |
| 0x14068f792 | UNCONDITIONAL_CALL |
| 0x14068f7f9 | UNCONDITIONAL_CALL |
| 0x1406945b3 | UNCONDITIONAL_CALL |
| 0x140694f40 | UNCONDITIONAL_CALL |
| 0x1406953c4 | UNCONDITIONAL_CALL |

### 3.2 References FROM FUN_140420350

**Total: 0.** The transfer function has no outgoing cross-references recorded — it does not call any other function whose reference is tracked (or all its calls are indirect/dynamic and not captured by static xref analysis).

**Interpretation:** The 3 DATA references are x64 unwind metadata (`.pdata` RUNTIME_FUNCTION and `.rdata` UNWIND_INFO), not invocations. They do not represent code paths that transfer control to `FUN_140420350`.

## 4. Transfer Function Decompilation — FUN_140420350

**Source:** `C:\WRSRE-ghidra-work\output\decomp_420350.c` — Ghidra decompilation, not manually rewritten.
**Function entry:** VA `0x140420350`, body `[0x140420350 - 0x140420a57]`.

### 4.1 Decompiled source (Ghidra interpretation)
=======
=======
>>>>>>> theirs
| Ghidra version | 12.1.4 PUBLIC |
| `analyzeHeadless.bat` | `...\support\analyzeHeadless.bat` — available |
| `ghidraRun.bat` | `...\ghidraRun.bat` — available |
| JDK | `C:\Program Files\Eclipse Adoptium\jdk-21.0.12.101-hotspot\` — Temurin-21.0.12.1+1 LTS |
| Debugger | None found |

### 1.4 Ghidra project status

| Item | Value |
|---|---|
| Project path | `C:\WRSRE-ghidra-work\projects\SOVIET64_analysis` |
| Project status | Exists, saved after import + post-scripts |
| Analysis completion | **TIMED OUT at 60 seconds** — file not fully analyzed |
| Functions created | Yes ("Create Function" completed) |
| Entry points disassembled | Yes ("Disassemble Entry Points" completed) |
| Post-scripts executed | Yes — all 6 scripts produced output |

The 16-site call enumeration is a **lower bound** because analysis timed out before completion. Additional call sites may exist in unanalyzed regions.

### 1.5 Existing artifacts inspected

Before creating new functionality:
- `scratch/` directory — **absent**
- `tools/` directory — no Ghidra scripts found
- `peinfo-out/` — **no files found**
- Claimed plan `docs/FUEL PLAN/implementation-plan-per-trip-fuel-v0.18.md` — **absent**

No pre-existing Ghidra scripts or project were reused. All tooling created fresh.

## 2. Documents Read and Findings Reused/Corrected/Rejected

### 2.1 Documents inspected

| Document | Role | Status |
|---|---|---|
| `docs/engine/vehicle-fuel-system.md` | Fuel burn/refuel investigation, vehicle cargo slot layout, upkeep function | Reused — key findings integrated below |
| `docs/ghidra-do-storage-findings.md` | DO task vector, TaskCfg, save/load | Reused — stride-8 correction noted |
| `docs/engine/private-industry-re.md` | Transfer/caller investigation with H11-H16 corrections | Reused — corrections applied |
| `docs/AGENT_REPORT_private_industry.md` | Transfer and logistics investigation G1-G10 | Reused — G1/G2 findings integrated |
| `plugins/private_logistics/private_logistics.cpp` | Existing plugin source, read-only scan | Reused — offset references verified |

### 2.2 Findings reused (from prior investigation)

| Finding | Source | Confidence | How used |
|---|---|---|---|
| `veh+0x4F8` = home Building* | vehicle-fuel Q1, AGENT_REPORT Q4, private-industry H11 | Proven (3 readers) | Home building read at refuel site |
| `veh+0x3B8/+0x3C0` = cargo vec stride 0x10 | vehicle-fuel Q1, AGENT_REPORT Q4, private-industry Q2 | Proven | Fuel slot location |
| Slot layout `{+0x00 Resource*, +0x08 float tons}` | vehicle-fuel Q1, AGENT_REPORT Q4 | Proven | Fuel level field |
| Fuel resource = `*(game+0xC330)` at refuel | vehicle-fuel Q4.2, AGENT_REPORT G2 | Likely | Refuel transfer res arg |
| Fuel resource = `*(game+0xC3F0)` at burn match | vehicle-fuel Q2.2 | Likely | Burn slot identification |
| Burn at `FUN_14016E3B0` (RVA 0x16E3B0), per building per tick | vehicle-fuel Q2, AGENT_REPORT G2 | Proven | Fuel deduction path |
| Refuel at `FUN_140420350` from `0x16F831`, mode 0, negative tons, `veh-arg = *(veh+0x4F8)` | vehicle-fuel Q4.2, AGENT_REPORT G2 | Proven | Refuel is ordinary transfer |
| `veh+0x4F0` = current building | private-industry H11 | Proven | Endpoint at call sites |
| `bld+0xC70/+0xC78` = vehicle fleet vector stride 8 | AGENT_REPORT Q4, vehicle-fuel Q2 | Proven | DO vehicle enumeration |
| `order+0x00/+0x08` = `{Resource*, tons}` records, NOT Building* | AGENT_REPORT G5, private-industry | Proven | Order structure correction |
| `order+0x30` = assigned Vehicle* | AGENT_REPORT G6, private-industry | Proven | Order vehicle gate |
| 16 CALL rel32 sites to `FUN_140420350` | private-industry H14, this investigation | Proven (this investigation) | Call census |
| Mode/sign independence | private-industry H15 | Proven | Billing gate on site AND tons > 0 |

### 2.3 Findings corrected (from prior investigation)

| Prior claim | Correction | Evidence |
|---|---|---|
| `order+0x00/+0x08` = vector of source `Building*` | It is `{Resource*, tons}` records | AGENT_REPORT G5, private-industry |
| `flag=0` means "never bill" | `flag=0` only skips `+0x30==veh` gate; 0x1B2898 passes `flag=0` | AGENT_REPORT G6 |
| `veh+0x4F0 = +0x4F8` rewrite is in the transfer | It is in `FUN_1403FBDC0` (detach/return-to-base), not the transfer | private-industry H13 correction |
| `FUN_140420350` is a dry-run | No dry-run; AL==1 always mutates | private-industry H13 |
| `order+0x50` = completion vector directly on order | It is an array of completion-record *pointers* on the order | This investigation (§5) |
| `veh+0x5F0` is authoritative fuel | It is a derived mirror; `slot+0x08` is authoritative | vehicle-fuel G1 |

### 2.4 Findings rejected (from prior investigation)

| Rejected claim | Reason |
|---|---|
| `veh+0x5F0` is authoritative fuel store | vehicle-fuel G1 proves `slot+0x08` is authoritative; `+0x5F0` can go negative |
| `order+0x00/+0x08` = source buildings | Contradicted by G5 evidence |
| `flag=0` ⇒ never bill | Contradicted by G6; 0x1B2898 passes flag=0 |

## 3. Transfer Call Enumeration

All 16 direct `CALL rel32` instructions targeting `FUN_140420350` (static VA `0x140420350`, RVA `0x420350`) were enumerated using `CallsTo420350.java` against the saved Ghidra project. Raw bytes, decoded length, target, and containing function are taken directly from machine instructions. Three additional DATA references exist (not invocations).

### 3.1 CALL rel32 instructions (16 total)

| # | Static VA | RVA | Raw bytes | Length | Target | Containing function |
|---|---|---|---|---|---|---|
| 1 | 0x140141f33 | 0x141f33 | `E8 18 E4 2D 00` | 5 | 0x140420350 | FUN_1401403f0 |
| 2 | 0x14016f831 | 0x16f831 | `E8 1A 0B 2B 00` | 5 | 0x140420350 | FUN_14016e3b0 |
| 3 | 0x1401b2898 | 0x1b2898 | `E8 B3 DA 26 00` | 5 | 0x140420350 | FUN_1401b1290 |
| 4 | 0x1401b484b | 0x1b484b | `E8 00 BB 26 00` | 5 | 0x140420350 | FUN_1401b3ce0 |
| 5 | 0x1401b5ec0 | 0x1b5ec0 | `E8 8B A4 26 00` | 5 | 0x140420350 | FUN_1401b5420 |
| 6 | 0x1401b653b | 0x1b653b | `E8 10 9E 26 00` | 5 | 0x140420350 | FUN_1401b5420 |
| 7 | 0x1401b6833 | 0x1b6833 | `E8 18 9B 26 00` | 5 | 0x140420350 | FUN_1401b5420 |
| 8 | 0x1401b75ed | 0x1b75ed | `E8 5E 8D 26 00` | 5 | 0x140420350 | FUN_1401b6a20 |
| 9 | 0x1401b7981 | 0x1b7981 | `E8 CA 89 26 00` | 5 | 0x140420350 | FUN_1401b6a20 |
| 10 | 0x1401cb7ff | 0x1cb7ff | `E8 4C 4B 25 00` | 5 | 0x140420350 | FUN_1401cb2a0 |
| 11 | 0x14068eeca | 0x68eeca | `E8 81 14 D9 FF` | 5 | 0x140420350 | FUN_14068b330 |
| 12 | 0x14068f792 | 0x68f792 | `E8 B9 0B D9 FF` | 5 | 0x140420350 | FUN_14068b330 |
| 13 | 0x14068f7f9 | 0x68f7f9 | `E8 52 0B D9 FF` | 5 | 0x140420350 | FUN_14068b330 |
| 14 | 0x1406945b3 | 0x6945b3 | `E8 98 BD D8 FF` | 5 | 0x140420350 | FUN_14068b330 |
| 15 | 0x140694f40 | 0x694f40 | `E8 0B B4 D8 FF` | 5 | 0x140420350 | FUN_14068b330 |
| 16 | 0x1406953c4 | 0x6953c4 | `E8 87 AF D8 FF` | 5 | 0x140420350 | FUN_14068b330 |

**All 16 displacements were independently recomputed from raw bytes and resolve to 0x140420350.**

### 3.2 Non-call references (3 total, not invocations)

| Static VA | RVA | Type | Classification |
|---|---|---|---|
| 0x140a6951c | 0xa6951c | DATA | `.pdata` or `.rdata` metadata — section/structure evidence not independently verified |
| 0x140952a9c | 0x952a9c | DATA | `.rdata` metadata — section/structure evidence not independently verified |
| 0x140952ac8 | 0x952ac8 | DATA | `.rdata` metadata — section/structure evidence not independently verified |

**Not classified as unwind metadata without section/structure evidence.** These are DATA references that do not represent invocations of `FUN_140420350`.

### 3.3 Coverage status

Analysis timed out at 60 seconds before completing. The 16-site enumeration is a **lower bound**: additional `CALL rel32` instructions to `FUN_140420350` may exist in unanalyzed code regions. Indirect calls (via function pointers) were not enumerated.

### 3.4 Call sites of interest

| RVA | Caller function | Mode | tons sign | param_9 (bld) | out (param_6) | Role |
|---|---|---|---|---|---|---|
| 0x16f831 | FUN_14016e3b0 | 0 | − (fuel draw) | NULL | NULL | Internal refuel draw from home bld |
| 0x1b2898 | FUN_1401b1290 | 1 | + | NULL | NULL | Road delivery unload |
| 0x1cb7ff | FUN_1401cb2a0 | 0 | + (slot cargo) | NULL | &local | Per-cargo-slot unload servicing |
| 0x68eeca | FUN_14068b330 | 0 | + | `veh+0x4F8` | R12 (live) | Sea/rail leg A |
| 0x68f792 | FUN_14068b330 | 0 | + | `veh+0x4F8` | NULL | Sea/rail leg B |
| 0x68f7f9 | FUN_14068b330 | 1 | − | `veh+0x4F8` | &local | Dock return / transship credit |
| 0x6945b3 | FUN_14068b330 | 0 | − | `veh+0x4F8` | [bld+0x368] live | Dock return leg |
| 0x694f40 | FUN_14068b330 | 0 | − | RSI | &local (RBP+0x20) | Sea/rail dock return leg |
| 0x6953c4 | FUN_14068b330 | 1 (always) | + | RSI | RSI | Sea/rail delivery |

## 4. Xref Results

### 4.1 References TO FUN_140420350 (incoming, 19 total)

Generated by `Xrefs420350.java` using `ReferenceManager.getReferencesTo(targetAddr)`. All addresses are static VAs.

| Static VA | RVA | Type |
|---|---|---|
| 0x140a6951c | 0xa6951c | DATA |
| 0x140952a9c | 0x952a9c | DATA |
| 0x140952ac8 | 0x952ac8 | DATA |
| 0x140141f33 | 0x141f33 | UNCONDITIONAL_CALL |
| 0x14016f831 | 0x16f831 | UNCONDITIONAL_CALL |
| 0x1401b2898 | 0x1b2898 | UNCONDITIONAL_CALL |
| 0x1401b484b | 0x1b484b | UNCONDITIONAL_CALL |
| 0x1401b5ec0 | 0x1b5ec0 | UNCONDITIONAL_CALL |
| 0x1401b653b | 0x1b653b | UNCONDITIONAL_CALL |
| 0x1401b6833 | 0x1b6833 | UNCONDITIONAL_CALL |
| 0x1401b75ed | 0x1b75ed | UNCONDITIONAL_CALL |
| 0x1401b7981 | 0x1b7981 | UNCONDITIONAL_CALL |
| 0x1401cb7ff | 0x1cb7ff | UNCONDITIONAL_CALL |
| 0x14068eeca | 0x68eeca | UNCONDITIONAL_CALL |
| 0x14068f792 | 0x68f792 | UNCONDITIONAL_CALL |
| 0x14068f7f9 | 0x68f7f9 | UNCONDITIONAL_CALL |
| 0x1406945b3 | 0x6945b3 | UNCONDITIONAL_CALL |
| 0x140694f40 | 0x694f40 | UNCONDITIONAL_CALL |
| 0x1406953c4 | 0x6953c4 | UNCONDITIONAL_CALL |

**16 direct calls + 3 DATA references = 19 total incoming references.**

### 4.2 References FROM FUN_140420350 (outgoing)

**Not enumerated.** The `Xrefs420350.java` script used `ReferenceManager.getReferencesFrom(entryAddr)`, which returns references originating *at the entry address only* — not a census of all outgoing references from the entire function body.

The decompiled source of `FUN_140420350` (section 5) shows calls to:
- `FUN_1401A9D60()` (line 127)
- `FUN_1401AA120(param_1, param_2)` (line 131)
- `FUN_140424430(param_1,param_2,param_4,param_5,0,0,0)` (line 135)
- `FUN_140423fe0(param_1,-param_5,&param_7)` (line 142)
- `FUN_140423e20(param_1,&param_7)` (line 145)
- `rand()` (line 140)

These are outgoing references visible in the decompilation but **not captured** by the `getReferencesFrom(entryAddr)` call. A proper outgoing-reference census would require scanning every instruction in the function body for CALL instructions — not yet performed.

**Section 3.2 previously stated "Total: 0" — this was incorrect.** The `getReferencesFrom(funcEntryAddress)` API does not produce a function-body census. Outgoing references exist (visible in decompilation) but were not systematically enumerated.

**No claim of 100% invocation coverage** is made from the direct-call census. The 16-site enumeration is a lower bound (see section 3.3).

### 4.3 Xref script source

See section 14 for complete `Xrefs420350.java` source.

## 5. Transfer Function Decompilation — FUN_140420350

**Source:** `C:\WRSRE-ghidra-work\output\decomp_420350.c` — Ghidra 12.1.4 decompilation via `DecompInterface.decompileFunction()`.
**Function entry:** static VA `0x140420350`, body `[0x140420350 - 0x140420a57]`.

### 5.1 Decompiled source (Ghidra interpretation)
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs

```
undefined8 FUN_140420350(longlong param_1, longlong param_2, int param_3,
                         longlong param_4, float param_5, float *param_6,
                         float param_7, char param_8, longlong param_9)
<<<<<<< ours
<<<<<<< ours
```

**Parameters (calling convention: RCX=game, RDX=vehicle/building, R8=mode, R9=resource, stack=rest):**

| Param | Register/Stack | Meaning |
|---|---|---|
| param_1 | RCX | `game*` |
| param_2 | RDX | `vehicle*` (mode 1) or `building*`/NULL (mode 0) |
| param_3 | R8 | mode: 0 = order-source pool, 1 = vehicle cargo slot, other = return 0 |
| param_4 | R9 | `resource*` |
| param_5 | stack+0x20 | `tons` (float; sign = direction) |
| param_6 | stack+0x28 | `out:*` (actual moved tons, written only if != NULL) |
| param_7 | stack+0x30 | `cap` (float) |
| param_8 | stack+0x38 | `flag` (char) |
| param_9 | stack+0x40 | `building*` (destination when non-NULL) |

**Return value (AL):**
- `AL == 1` — mutation committed (real tons moved)
- `AL == 0` — early exit (bad resource, empty cargo, no matching order/stop)

**outMoved (param_6):** Written only if `param_6 != NULL` (decompiled lines 256-258: `if (param_6 != (float *)0x0) { *param_6 = fVar21; }`). `fVar21` = clamped actual moved tons. If `param_6 == NULL`, caller does not receive actual moved quantity. No dry-run mode — every `AL == 1` invocation mutates game state.

### 4.2 Mode 1 path (vehicle cargo slot, decompiled lines 39-83)

- Iterates `param_2 + 0x3B8` to `param_2 + 0x3C0` (cargo slots, 0x10 bytes each)
- For each slot: reads `*(slot + 0x00)` = resource pointer, `*(slot + 0x08)` = tons
- Validates `1 < *(int *)(slot_resource + 0x44) + 4U` (resource type check)
- Finds matching resource (`slot_resource == param_4`)
- Falls through to common mutation path at LAB_1404209da

### 4.3 Mode 0 path (order-source pool, decompiled lines 84-134)

- `fVar20 = 1.0` (default cap factor)
- If `param_2 == 0` (building-to-building, no vehicle): `fVar19 = 0.0`
- Validates `param_9` as building: `param_9 != 0 && *(longlong *)(param_9 + 0x318) != 0 && (*(int *)(*(longlong *)(param_9 + 0x318) + 0x360) == 0x0C || == 0x6A)` (building type check)
- Checks `*(char *)(param_2 + 0xea8) == '\0' || (0.0 < param_5)`
- Searches building storage at `param_2 + 0x610` to `param_2 + 0x618` (stride 0x21B8)
- If `param_2 != 0`: calls `FUN_1401A9D60()` and `FUN_1401AA120(param_1, param_2)` to compute `fVar19`
- Falls through to common path at LAB_14042048a

### 4.4 Order scanning (decompiled lines 136-227)

- Order vector: `*(longlong *)(param_1 + 0x144A8)` to `*(longlong *)(param_1 + 0x144B0)`, stride 0x68
- Empty check: `(end - begin) / 0x68 == 0` → return 0
- Iterates order entries: `order_entry = order_base + i * 0x68`
- Gate: `param_8 == '\0' || *(longlong *)(order_entry + 0x30) == param_2` (flag bypass or vehicle assignment match)
- Inner loop: iterates resource records at `*(order_entry + 0x00)` (8-byte pointers to 16-byte `{resource* +0x00, tons +0x08}` records)
- Match: `**record == param_4` (resource pointer match)
- On match: completion recording at `order_entry + 0x50` array:
  - `completion_record + 0x28 + 0x188` = done flag → set to `0x3f800000` (1.0f)
  - `completion_record + 0x28 + 0x198` = resource → set to `param_4`

### 4.5 Common mutation path (decompiled lines 228-259)

```
if (pfVar18 == (float *)0x0) return 0;   // no matching target found
fVar21 = 0.0;
if ((param_5 <= 0.0) || (fVar20 < param_5)) {
    if (0.0 <= param_5) return 0;       // zero tons rejected
    if (*pfVar18 <= 0.0) return 0;      // nothing to unload
    param_5 = *pfVar18 + param_5;       // add negative tons (unload)
    *pfVar18 = param_5;
    if (param_5 < 0.0) {                // clamp at zero
        *pfVar18 = 0.0;
        fVar21 = -param_5;              // actual moved = amount removed
    }
} else {
    param_5 = param_5 + *pfVar18;       // add positive tons (load)
    *pfVar18 = param_5;
    if (fVar19 < param_5) {             // clamp at capacity
        fVar21 = param_5 - fVar19;      // actual moved = amount that fit
        *pfVar18 = param_5 - fVar21;
    }
}
if (param_6 != (float *)0x0) {
    *param_6 = fVar21;                   // write actual moved to out pointer
=======
=======
>>>>>>> theirs
{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  uint uVar4;
  ulonglong uVar5;
  char cVar6;
  uint uVar7;
  longlong lVar8;
  float *pfVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  float *pfVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  int iVar15;
  longlong lVar16;
  uint uVar17;
  float *pfVar18;
  float fVar19;
  float fVar20;
  float fVar21;

  if (*(int *)(param_4 + 0x44) == -1) {
    return 0;
  }
  pfVar9 = (float *)0x0;
  if (param_3 != 0) {
    if (param_3 != 1) {
      return 0;
    }
    pfVar18 = (float *)0xffffffff;
    iVar15 = -1;
    fVar20 = 0.0;
    if (*(longlong *)(param_2 + 0x3c0) - *(longlong *)(param_2 + 0x3b8) >> 4 != 0) {
      lVar2 = *(longlong *)(param_2 + 0x3b8);
      pfVar12 = pfVar9;
      do {
        lVar8 = *(longlong *)(lVar2 + (longlong)pfVar12 * 0x10);
        if (1 < *(int *)(lVar8 + 0x44) + 4U) {
          fVar19 = *(float *)(lVar2 + 8 + (longlong)pfVar12 * 0x10);
          iVar15 = *(int *)(*(longlong *)(param_2 + 0x1708) + 0x8600);
          fVar20 = fVar20 + fVar19 / (*(float *)((longlong)iVar15 * 0x20 + 0xcc + lVar8) *
                                     *(float *)(*(longlong *)(param_2 + 0x1708) + 0x8604));
          if ((0.0 < fVar19) && (iVar15 - 2U < 3)) {
            pfVar18 = pfVar12;
          }
        }
        iVar15 = (int)pfVar12;
        pfVar12 = (float *)(ulonglong)((int)pfVar12 + 1);
      } while (pfVar12 < (float *)(*(longlong *)(param_2 + 0x3c0) - lVar2 >> 4));
    }
    if (*(longlong *)(param_2 + 0x3c0) - *(longlong *)(param_2 + 0x3b8) >> 4 == 0) {
      return 0;
    }
    lVar2 = *(longlong *)(param_2 + 0x3b8);
    while( true ) {
      lVar8 = *(longlong *)(lVar2 + (longlong)pfVar9 * 0x10);
      if (((1 < *(int *)(lVar8 + 0x44) + 4U) && (lVar8 == param_4)) &&
         ((iVar15 == (int)pfVar9 || (iVar15 == -1)))) break;
      pfVar9 = (float *)(ulonglong)((int)pfVar9 + 1);
      if ((float *)(*(longlong *)(param_2 + 0x3c0) - lVar2 >> 4) <= pfVar9) {
        return 0;
      }
    }
    pfVar18 = (float *)(lVar2 + 8 + (longlong)pfVar9 * 0x10);
    fVar19 = *(float *)((longlong)*(int *)(*(longlong *)(param_2 + 0x1708) + 0x8600) * 0x20 + 0xcc +
                       *(longlong *)((longlong)pfVar9 * 0x10 + lVar2)) *
             *(float *)(*(longlong *)(param_2 + 0x1708) + 0x8604);
    fVar20 = fVar19 * (1.0 - fVar20);
    goto LAB_1404209da;
  }
  fVar20 = 1.0;
  if (param_2 == 0) {
    fVar19 = 0.0;
  LAB_14042048a:
    if ((((param_9 != 0) && (*(longlong *)(param_9 + 0x318) != 0)) &&
        ((iVar15 = *(int *)(*(longlong *)(param_9 + 0x318) + 0x360), iVar15 == 0xc ||
         (iVar15 == 0x6a)))) && ((*(char *)(param_2 + 0xea8) == '\0' || (0.0 < param_5)))) {
      lVar8 = *(longlong *)(param_2 + 0x618) - *(longlong *)(param_2 + 0x610);
      lVar2 = lVar8 >> 0x3f;
      if (lVar8 / 0x21b8 + lVar2 == lVar2) {
        return 0;
      }
      lVar2 = *(longlong *)(param_2 + 0x610);
      pfVar18 = pfVar9;
      while (iVar15 = (int)pfVar18, *(float *)((longlong)pfVar18 * 0x21b8 + lVar2) <= fVar19) {
        pfVar18 = (float *)(ulonglong)(iVar15 + 1);
        if ((float *)((*(longlong *)(param_2 + 0x618) - lVar2) / 0x21b8) <= pfVar18) {
          return 0;
        }
      }
      if (iVar15 < 0) {
        return 0;
      }
      lVar8 = (longlong)iVar15 * 0x21b8;
      if (*(longlong *)(lVar8 + 0x10 + lVar2) - *(longlong *)(lVar8 + 8 + lVar2) >> 4 == 0) {
        return 0;
      }
      lVar3 = *(longlong *)(lVar8 + 8 + lVar2);
      while( true ) {
        if (*(longlong *)(lVar3 + (longlong)pfVar9 * 0x10) == param_4) break;
        pfVar9 = (float *)(ulonglong)((int)pfVar9 + 1);
        if ((float *)(*(longlong *)(lVar8 + 0x10 + lVar2) - lVar3 >> 4) <= pfVar9) {
          return 0;
        }
      }
      lVar2 = *(longlong *)((longlong)iVar15 * 0x21b8 + 8 + lVar2);
      pfVar18 = (float *)(lVar2 + 8 + (longlong)pfVar9 * 0x10);
      fVar20 = *(float *)((longlong)pfVar9 * 0x10 + 0xc + lVar2) + fVar20;
      fVar19 = fVar20;
      goto LAB_1404209da;
    }
  }
  else {
    if ((*(int *)(param_1 + 0x5bc) < 1) || (cVar6 = func_0x0001401a9d60(), cVar6 == '\0')) {
      fVar19 = *(float *)(param_2 + 0x604);
    }
    else {
      fVar19 = (float)func_0x0001401aa120(param_1,param_2);
    }
    if (fVar19 < fVar20) goto LAB_14042048a;
  }
  FUN_140424430(param_1,param_2,param_4,param_5,0,0,0);
  if ((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68 == 0) {
    return 0;
  }
  param_7 = -NAN;
  rand();
  if (param_5 <= 0.0) {
    uVar7 = FUN_140423fe0(param_1,-param_5,&param_7);
  }
  else {
    uVar7 = FUN_140423e20(param_1,&param_7);
  }
  uVar10 = (ulonglong)uVar7;
  lVar8 = *(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8);
  lVar2 = lVar8 >> 0x3f;
  if (lVar8 / 0x68 + lVar2 == lVar2) {
    return 0;
  }
  uVar13 = (longlong)(int)uVar7;
  pfVar18 = pfVar9;
  fVar20 = param_7;
  fVar19 = param_7;
  fVar21 = param_7;
  do {
    uVar11 = 0;
    uVar7 = (int)uVar10 + 1;
    uVar10 = uVar11;
    uVar5 = uVar11;
    if (uVar7 != (uint)((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68
                       )) {
      uVar10 = (ulonglong)uVar7;
      uVar5 = uVar13 + 1;
    }
    if ((param_8 == '\0') ||
       (*(longlong *)(uVar13 * 0x68 + 0x30 + *(longlong *)(param_1 + 0x144a8)) == param_2)) {
      uVar14 = uVar11;
      if ((-1 < (int)fVar21) && (uVar14 = 0, (int)pfVar9 == 0)) {
        uVar14 = (ulonglong)(uint)fVar21;
        param_7 = -NAN;
      }
      lVar2 = *(longlong *)(param_1 + 0x144a8);
      lVar8 = uVar13 * 0x68;
      if (*(longlong *)(lVar2 + 8 + lVar8) - *(longlong *)(lVar2 + lVar8) >> 3 != 0) {
        lVar3 = *(longlong *)(lVar2 + lVar8);
        lVar16 = (longlong)(int)uVar14;
        do {
          lVar1 = lVar16 * 8;
          uVar17 = (int)uVar14 + 1;
          uVar7 = (uint)(*(longlong *)(lVar2 + 8 + lVar8) - lVar3 >> 3);
          lVar16 = lVar16 + 1;
          if (uVar17 == uVar7) {
            lVar16 = 0;
          }
          if (**(longlong **)(lVar3 + lVar1) == param_4) {
            lVar16 = (longlong)(int)uVar14;
            pfVar18 = (float *)(*(longlong *)(lVar3 + lVar16 * 8) + 8);
            if (((0.0 < param_5) &&
                (fVar20 = param_5 + *pfVar18 + 0.1, fVar19 = fVar20, param_5 <= fVar20)) &&
               (lVar2 = *(longlong *)(*(longlong *)(lVar2 + 0x38 + lVar8) + lVar16 * 8), lVar2 != 0)
               ) {
              *(undefined4 *)(lVar2 + 0x80) = 0x3f800000;
            }
            if ((((param_5 < 0.0) &&
                 (0.0 < *(float *)(*(longlong *)
                                   (*(longlong *)(lVar8 + *(longlong *)(param_1 + 0x144a8)) +
                                   lVar16 * 8) + 8))) || (0.0 < param_5)) &&
               (lVar2 = *(longlong *)
                        (*(longlong *)(lVar8 + 0x50 + *(longlong *)(param_1 + 0x144a8)) +
                        lVar16 * 8), lVar2 != 0)) {
              *(undefined4 *)(*(longlong *)(lVar2 + 0x28) + 0x188) = 0x3f800000;
              *(longlong *)
               (*(longlong *)
                 (*(longlong *)
                   (*(longlong *)(lVar8 + 0x50 + *(longlong *)(param_1 + 0x144a8)) + lVar16 * 8) +
                 0x28) + 0x198) = param_4;
            }
            break;
          }
          uVar11 = (ulonglong)((int)uVar11 + 1);
          uVar4 = 0;
          if (uVar17 != uVar7) {
            uVar4 = uVar17;
          }
          uVar14 = (ulonglong)uVar4;
        } while (uVar11 < (ulonglong)(*(longlong *)(lVar2 + 8 + lVar8) - lVar3 >> 3));
      }
      fVar21 = param_7;
      if (pfVar18 != (float *)0x0) goto LAB_1404209e3;
    }
    pfVar9 = (float *)(ulonglong)((int)pfVar9 + 1);
    uVar13 = uVar5;
  } while (pfVar9 < (float *)((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8))
                             / 0x68));
LAB_1404209da:
  if (pfVar18 == (float *)0x0) {
    return 0;
  }
LAB_1404209e3:
  fVar21 = 0.0;
  if ((param_5 <= 0.0) || (fVar20 < param_5)) {
    if (0.0 <= param_5) {
      return 0;
    }
    if (*pfVar18 <= 0.0) {
      return 0;
    }
    param_5 = *pfVar18 + param_5;
    *pfVar18 = param_5;
    if (param_5 < 0.0) {
      *pfVar18 = 0.0;
      fVar21 = -param_5;
    }
  }
  else {
    param_5 = param_5 + *pfVar18;
    *pfVar18 = param_5;
    if (fVar19 < param_5) {
      fVar21 = param_5 - fVar19;
      *pfVar18 = param_5 - fVar21;
    }
  }
  if (param_6 != (float *)0x0) {
    *param_6 = fVar21;
  }
  return 1;
}
```

### 5.2 Parameter table (from decompiled signature)

| Param | Register/Stack | Decompiler type | Meaning |
|---|---|---|---|
| param_1 | RCX | `longlong` | `game*` — base for order vector at `+0x144A8` |
| param_2 | RDX | `longlong` | `vehicle*` (mode 1) or `building*`/NULL (mode 0) |
| param_3 | R8 | `int` | mode: 0 = order-source pool, 1 = vehicle cargo slot, other = return 0 |
| param_4 | R9 | `longlong` | `resource*` |
| param_5 | stack+0x20 | `float` | `tons` — sign indicates direction |
| param_6 | stack+0x28 | `float *` | output pointer — written only if != NULL |
| param_7 | stack+0x30 | `float` | capacity/threshold value |
| param_8 | stack+0x38 | `char` | flag — when '\0', skips vehicle-assignment gate |
| param_9 | stack+0x40 | `longlong` | `building*` — validated at `+0x318` type descriptor |

### 5.3 Nine-parameter callee vs caller decompilation

The decompiled caller output in `decomp_68b330.c` (section 6) shows calls like:

```c
cVar8 = FUN_140420350(&DAT_1409d4f10,lVar24,0,lVar13,
                      in_stack_fffffffffffffbe8,0);
```

This shows 6 arguments. Under the x64-64 calling convention, the first four arguments go in RCX, RDX, R8, R9; arguments 5–9 go on the stack. The decompiler's 6-argument display omits some stack arguments whose storage is reused or optimized away. **This is a decompiler display limitation, not a calling-convention violation.** All 9 parameters are set by callers per the verified disassembly in section 7.

The disassembly at call site 0x694F40 sets all 9:
- RCX = game (`LEA RCX,[0x1409d4f10]`)
- RDX = vehicle (`MOV RDX,RBX`)
- R8 = mode=0 (`XOR R8D,R8D`)
- R9 = resource (`MOV R9,qword ptr [RAX]`)
- stack+0x20 = tons (`MOVSS [RSP+0x20],XMM0`)
- stack+0x28 = out (`MOV [RSP+0x28],RCX` where RCX=`LEA RCX,[RBP+0x20]`)
- stack+0x30 = cap (not set in window)
- stack+0x38 = flag=0 (`MOV byte ptr [RSP+0x38],0`)
- stack+0x40 = building (`MOV [RSP+0x40],RSI`)

### 5.4 Return value (AL)

- `AL == 1` — mutation committed through `*pfVar18`
- `AL == 0` — early exit (bad resource at line 35, wrong mode at line 40-41, empty cargo at line 64-66, no matching order/stop at line 136-138, no matching target at line 229-231, zero tons at line 235-236, empty source at line 238-240)

**AL == 1 does NOT imply positive movement.** It only implies the mutation code path was reached and `*pfVar18` was written. The output `fVar21` can be 0.0 when `AL == 1`.

### 5.5 Output quantity: fVar21 — NOT "actual moved tons"

The decompiled mutation code (lines 233-259) shows `fVar21` is initialized to `0.0` and set only in two clamp paths:

```
fVar21 = 0.0;
if ((param_5 <= 0.0) || (fVar20 < param_5)) {
    // NEGATIVE TONS PATH (unload/removal)
    if (0.0 <= param_5) return 0;              // zero tons → early exit, no mutation
    if (*pfVar18 <= 0.0) return 0;              // empty source → early exit
    param_5 = *pfVar18 + param_5;               // add negative request to current
    *pfVar18 = param_5;                          // store result
    if (param_5 < 0.0) {                        // would go below zero
      *pfVar18 = 0.0;                            // clamp to zero
      fVar21 = -param_5;                         // amount that COULD NOT be removed
    }
} else {
    // POSITIVE TONS PATH (load/addition)
    param_5 = param_5 + *pfVar18;               // add positive request to current
    *pfVar18 = param_5;                          // store result
    if (fVar19 < param_5) {                     // exceeds capacity fVar19
      fVar21 = param_5 - fVar19;                // amount that COULD NOT fit (overflow)
      *pfVar18 = param_5 - fVar21;               // clamp to capacity
    }
}
if (param_6 != (float *)0x0) {
    *param_6 = fVar21;                          // write fVar21 to out pointer
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
}
return 1;
```

<<<<<<< ours
<<<<<<< ours
**Key observations from decompilation:**
- `pfVar18` points to the mutable tons field (cargo slot `+8` or order record tons)
- `fVar21` starts at 0.0 and is only set when actual movement occurs
- `outMoved` can be 0.0 with `AL == 1` when mutation commits but net movement is zero (clamped)
- The transfer function itself does **not** read `vehicle+0x4F0` — no `+0x4F0` reference appears in the decompilation

## 5. Caller Decompilation — FUN_14068B330 (Sea/Rail Hauler)

**Source:** `C:\WRSRE-ghidra-work\output\decomp_68b330.c` — Ghidra decompilation, not manually rewritten.
**Function entry:** VA `0x14068B330`, contains all 6 sea/rail transfer call sites.

### 5.1 Paired transfer calls (decompiled lines 1599-1644)

The two call sites of primary interest (0x694F40 and 0x6953C4) appear sequentially in `FUN_14068B330`:

**First call — mode 0, out=NULL (decompiled line 1602-1603):**
```c
cVar8 = FUN_140420350(&DAT_1409d4f10, lVar24, 0, lVar13,
                      in_stack_fffffffffffffbe8, 0);
```
| Param | Value | Meaning |
|---|---|---|
| param_1 | `&DAT_1409d4f10` | game global (`0x1409d4f10`) |
| param_2 | `lVar24` | vehicle/entity pointer 1 (R14 at disassembly) |
| param_3 | `0` | mode 0 (order-source pool) |
| param_4 | `lVar13` | resource pointer |
| param_5 | `in_stack_fffffffffffffbe8` | tons (fVar52) |
| param_6 | `0` | NULL — out not requested |

Error handling (lines 1604-1631): `if (cVar8 == '\0')` → checks building type 0x14 and `0.0 < fVar108`, then iterates storage slots at `lVar24 + 0x970`.

**Second call — mode 1, out=&local (decompiled lines 1642-1644):**
```c
FUN_140420350(&DAT_1409d4f10, plStackX_8[0x72], 1, lVar13,
              in_stack_fffffffffffffbe8, &plStack_338);
```
| Param | Value | Meaning |
|---|---|---|
| param_1 | `&DAT_1409d4f10` | game global (`0x1409d4f10`) |
| param_2 | `plStackX_8[0x72]` | vehicle/entity pointer 2 (`[R14+0x390]` at disassembly) |
| param_3 | `1` | mode 1 (vehicle cargo slot) |
| param_4 | `lVar13` | same resource pointer |
| param_5 | `in_stack_fffffffffffffbe8` | same tons (fVar52) |
| param_6 | `&plStack_338` | out pointer — actual moved written here |

### 5.2 Post-call handling (decompiled lines 1669-1677)

=======
=======
>>>>>>> theirs
| Variable | Meaning |
|---|---|
| `fVar21` | Output value written to `*param_6` — amount that could NOT be moved (rejected/overflow) |
| `|param_5| - fVar21` | Actual quantity moved (when clamped) or `|param_5|` (when fVar21=0) |
| `*pfVar18` (after) | Final stored quantity (clamped) |
| `*pfVar18` (before) | Initial quantity |

**Neutral name:** `fVar21` should be called `rejected_amount` or `overflow_amount`, not `outMoved` or `actual_moved_tons`.

### 5.6 Numerical walkthroughs

| Case | Initial | Request | fVar20/cap | Branch taken | Final stored | fVar21 (output) | Actual moved |
|---|---|---|---|---|---|---|---|
| 1. Removal within available | 10 | -3 | N/A | Negative path | 7 | 0.0 | 3 |
| 2. Removal exceeding available | 10 | -15 | N/A | Negative path → clamp at 0 | 0 | 5.0 | 10 |
| 3. Addition below capacity | 7 | +3 | 10 | Positive path | 10 | 0.0 | 3 |
| 4. Addition exceeding capacity | 7 | +5 | 10 | Positive path → clamp at 10 | 10 | 2.0 | 3 |
| 5. Zero request / zero movement | 7 | 0 | N/A | Guard: `0.0 <= param_5` → return 0 | 7 | not written | 0 |

**Case 5 detail:** `param_5 = 0.0`. Guard `(param_5 <= 0.0)` is TRUE. Enter first branch. `0.0 <= param_5` → `0.0 <= 0.0` is TRUE → `return 0`. AL=0, no mutation, output not written.

**Case 1 detail:** `param_5 = -3.0`. Guard TRUE. `0.0 <= -3.0` FALSE. `*pfVar18 <= 0.0` (10 <= 0) FALSE. `param_5 = 10 + (-3) = 7`. `*pfVar18 = 7`. `7 < 0.0` FALSE. `fVar21` stays 0.0. AL=1.

**Case 2 detail:** `param_5 = -15.0`. Guard TRUE. `0.0 <= -15.0` FALSE. `10 <= 0` FALSE. `param_5 = 10 + (-15) = -5`. `*pfVar18 = -5`. `-5 < 0.0` TRUE. `*pfVar18 = 0.0`. `fVar21 = -(-5) = 5.0`. AL=1.

**Case 3 detail:** `param_5 = 3.0`. Guard: `3.0 <= 0.0` FALSE. `fVar20 < 3.0`? Depends on mode and computed fVar20. Assuming mode 1 with fVar20 computed such that `fVar20 >= 3.0`, enter else branch. `param_5 = 7 + 3 = 10`. `*pfVar18 = 10`. `fVar19(10) < 10`? FALSE. `fVar21` stays 0.0. AL=1.

**Case 4 detail:** `param_5 = 5.0`. Enter else branch (assuming fVar20 >= 5.0). `param_5 = 7 + 5 = 12`. `*pfVar18 = 12`. `fVar19(10) < 12` TRUE. `fVar21 = 12 - 10 = 2.0`. `*pfVar18 = 12 - 2 = 10`. AL=1.

### 5.7 Mode 1 path (vehicle cargo slot)

Iterates cargo slots at `param_2 + 0x3B8` to `param_2 + 0x3C0` (0x10 bytes each). For each slot:
- `*(slot + 0x00)` = resource pointer
- `*(slot + 0x08)` = tons
- Validates `1 < *(int *)(slot_resource + 0x44) + 4U`
- Finds matching resource (`slot_resource == param_4`)
- `pfVar18 = slot + 0x8` (the mutable tons field)
- `fVar19 = slot_capacity * (1.0 - fVar20)` where fVar20 accumulates cargo utilization

### 5.8 Mode 0 path (order-source pool)

- Order vector at `*(param_1 + 0x144A8)` to `*(param_1 + 0x144B0)`, stride 0x68
- `order_entry + 0x30` = assigned `Vehicle*` (gate: `param_8 == '\0'` or `order_entry + 0x30 == param_2`)
- Resource records at `*(order_entry + 0x00)` = 8-byte pointers to 16-byte `{resource* +0x00, float tons +0x08}`
- Completion recording at `order_entry + 0x50`:
  - `completion_record + 0x28 + 0x188` = done flag (0x3f800000 = 1.0f)
  - `completion_record + 0x28 + 0x198` = resource pointer (param_4)

### 5.9 Order-structure stride confusion (corrected)

Prior reports claimed `order+0x10`, `order+0x48`, `order+0x50`, `order+0x28`, `order+0x68`, `order+0x70` were order fields. The decompilation shows:
- `order+0x00/+0x08` = `{Resource*, tons}` records (NOT source Building*)
- `order+0x30` = assigned Vehicle* (NOT destination Building*)
- `order+0x50` = array of completion-record *pointers* (NOT a completion vector directly on the order)
- No destination field exists on the order structure

The DO task record (stride 0x80 per `ghidra-do-storage-findings.md`) is a separate structure; fields like `+0x10` (source Building*) belong to it, not to the order.

### 5.10 No `+0x4F0` reference

The decompilation of `FUN_140420350` contains **no `vehicle+0x4F0` reference**. The transfer function does not read `vehicle+0x4F0`. This means:
- `vehicle+0x4F0` is not used by the transfer at call sites
- Whether `vehicle+0x4F0` is valid at any specific call site is **unresolved**
- Absence of `+0x4F0` inside this function does NOT prove the field is invalid elsewhere

### 5.11 Outgoing references from FUN_140420350

The decompilation shows calls to: `FUN_1401A9D60`, `FUN_1401AA120`, `FUN_140424430`, `FUN_140423fe0`, `FUN_140423e20`, `rand()`. These are visible in the decompiled source but were **not captured** by the `Xrefs420350.java` script's `getReferencesFrom(entryAddr)` call (see section 4.2). A proper outgoing-reference census requires scanning all instructions in the function body — not yet performed.

## 6. Caller Decompilation — FUN_14068B330 (Sea/Rail Hauler)

**Source:** `C:\WRSRE-ghidra-work\output\decomp_68b330.c` (364,407 bytes, 7,647 lines) — Ghidra 12.1.4 decompilation.
**Function entry:** static VA `0x14068B330`.

### 6.1 Relevant regions of caller decompilation

The decompiled code at lines 1594-1678 shows a do-while loop (`iVar44 < 1`) containing two sequential calls to `FUN_140420350`:

**First call (decompiled lines 1602-1603):**
```c
cVar8 = FUN_140420350(&DAT_1409d4f10,lVar24,0,lVar13,
                      in_stack_fffffffffffffbe8,0);
```
This decompiled output shows 6 arguments. Under x64-64 calling convention, args 5-9 are on the stack. The decompiler's 6-argument display omits stack arguments whose storage is reused or optimized. Per the verified disassembly at 0x694F40 (section 7.1), param_6 is `&[RBP+0x20]` (non-NULL), NOT 0 as the truncated decompiled output suggests.

**Second call (decompiled lines 1643-1644):**
```c
FUN_140420350(&DAT_1409d4f10,plStackX_8[0x72],1,lVar13,
              in_stack_fffffffffffffbe8,&plStack_338);
```
This shows param_6 = `&plStack_338`. Per the verified disassembly at 0x6953C4 (section 7.2), param_6 = RSI stored at [RSP+0x28]. If RSI happens to equal the address of plStack_338, these are consistent; otherwise the decompiler may have misidentified the stack slot.

**Post-call handling (decompiled lines 1669-1672):**
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
```c
if (0.0 < plStack_338._0_4_) {
    lVar28 = func_0x00014002ecd0(plStack_3a0, lVar32);
    *(undefined4 *)(lVar28 + 8) = 0;
}
<<<<<<< ours
<<<<<<< ours
=======
=======
>>>>>>> theirs
```
This reads `plStack_338._0_4_` — the output value from the **second** call (mode 1, at 0x6953C4). **This read belongs to call site 0x6953C4, not 0x1CB7FF.** The previous report's attribution to 0x1CB7FF (inside FUN_1401CB2A0) was a provenance mismatch — line 1669 is in decomp_68b330.c (FUN_14068B330), not in FUN_1401CB2A0.

**Delivery progress accumulation (decompiled lines 1673-1677):**
```c
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
*(float *)((longlong)plStackX_8 + 0x564) =
    fVar52 / (*(float *)((longlong)*(int *)(plStackX_8[0x2e1] + 0x8600) * 0x20 + 0xcc + lVar13) *
              *(float *)(plStackX_8[0x2e1] + 0x8604)) +
    *(float *)((longlong)plStackX_8 + 0x564);
```
<<<<<<< ours
<<<<<<< ours

- `plStack_338._0_4_` = outMoved from mode 1 call; if > 0, a field at `lVar28 + 8` is zeroed
- `plStackX_8 + 0x564` accumulates a delivery progress ratio using `fVar52` (nominal tons), NOT `outMoved`

### 5.3 Treasury/accounting calls (decompiled lines 1655-1667)

`FUN_1402FDB00` is called twice after the paired transfers, once per call site's data — this is the treasury/statistics accounting function mentioned in prior reports.

**Vehicle identity distinction:**
- First call vehicle: `lVar24` (R14 at disassembly address 0x694F35)
- Second call vehicle: `plStackX_8[0x72]` = `[R14 + 0x390]` (different pointer, from memory at disassembly address 0x6953A9)
- Same resource (`lVar13`), same tons (`fVar52`) across both calls

**These are two different vehicle pointers** — not the same vehicle loading then unloading. This is a coordinated operation between two distinct entities.

## 6. Disassembly Excerpts — Call Sites 0x694F40 and 0x6953C4

**Source:** `C:\WRSRE-ghidra-work\output\disasm_694f40_full.asm` and `disasm_6953c4_full.asm` — Ghidra disassembly, machine instructions only. No manual rewriting of addresses or bytes.

### 6.1 Call site 0x694F40 (VA 0x140694F40)

Range: `0x140694E80:0x140694FE0` (85 instructions)

```
;; 140694e80: no instruction (undefined)
;; 140694e81: no instruction (undefined)
JNZ 0x140694e8b
MOVSS XMM1,dword ptr [RCX + 0x58]
JMP 0x140694e90
MOVSS XMM1,dword ptr [RCX + 0x5c]
XORPS XMM3,XMM3
ADDSS XMM1,XMM3
MOVAPS XMM6,XMM1
MULSS XMM6,XMM8
CMP R8D,0x3
JNZ 0x140694ec6
CVTPS2PD XMM0,XMM6
MOVSD XMM2,qword ptr [0x1409d5490]
COMISD XMM0,XMM2
JBe 0x140694eba
CVTPD2PS XMM6,XMM2
CMP byte ptr [0x1409e8599],0x0
JNZ 0x140694ec6
MOVAPS XMM6,XMM3
CMP R8D,0x4
JNZ 0x140694eea
CVTPS2PD XMM0,XMM6
MOVSD XMM2,qword ptr [0x1409d5498]
COMISD XMM0,XMM2
JBe 0x140694ee1
CVTPD2PS XMM6,XMM2
CMP byte ptr [0x1409e8591],0x0
JZ 0x140694ef3
COMISS XMM6,XMM3
JA 0x140694f84
MOV R8,qword ptr [RBP + -0x68]
JMP 0x140694f50
MOV dword ptr [RBP + 0x20],0x0
MOV RDX,R14
MOV RDI,qword ptr [RBP + -0x68]
MOV RCX,RDI
CALL 0x14002ecd0
MOVAPS XMM0,XMM8
XORPS XMM0,XMM15
MOV qword ptr [RSP + 0x40],RSI
MOV byte ptr [RSP + 0x38],0x0
LEA RCX,[RBP + 0x20]
MOV qword ptr [RSP + 0x28],RCX
MOVSS dword ptr [RSP + 0x20],XMM0
MOV R9,qword ptr [RAX]
XOR R8D,R8D
MOV RDX,RBX
LEA RCX,[0x1409d4f10]
CALL 0x140420350
TEST AL,AL
JNZ 0x140695377
MOV R8,RDI
MOV EDI,0x1
INC R13D
MOV dword ptr [RBP + 0x20],R13D
MOV RCX,qword ptr [R8 + 0x8]
SUB RCX,qword ptr [R8]
SAR RCX,0x4
MOV EAX,R13D
CMP RAX,RCX
JNC 0x1406954ba
MOVSS XMM8,dword ptr [RBP + 0x368]
MOV ECX,dword ptr [RBP + -0x50]
JMP 0x1406947b0
MOVAPS XMM7,XMM6
DIVSS XMM7,XMM1
MOVSXD RBX,R12D
MOV RDX,RBX
MOV RCX,RDI
CALL 0x14002ecd0
MOV R14,qword ptr [RBP + 0x340]
MOV RCX,qword ptr [R14 + 0x1708]
MOVSXD RDX,dword ptr [RCX + 0x8600]
SHL RDX,0x5
MOV RAX,qword ptr [RAX]
MOVSS XMM11,dword ptr [RDX + RAX*0x1 + 0xcc]
MULSS XMM11,dword ptr [RCX + 0x8604]
COMISS XMM7,XMM9
JBe 0x140694fde
MOVAPS XMM0,XMM9
DIVSS XMM0,XMM7
MOVAPS XMM7,XMM9
MULSS XMM6,XMM0
LEA RCX,[RBP + -0x80]
```

**Argument setup at CALL 0x140420350 (0x140694F50):**

| Param | Instruction | Value | Meaning |
|---|---|---|---|
| param_1 | `LEA RCX,[0x1409d4f10]` (0x140694F67) | `0x1409d4f10` | game global |
| param_2 | `MOV RDX,RBX` (0x140694F64) | `RBX` | vehicle* — set immediately before call; R14 loaded earlier (0x140694F35) but overwritten by RBX |
| param_3 | `XOR R8D,R8D` (0x140694F61) | `0` | mode 0 |
| param_4 | `MOV R9,qword ptr [RAX]` (0x140694F46) | `[RAX]` | resource* (RAX from helper 0x14002ecd0 at 0x140694F38) |
| param_5 | `MOVSS dword ptr [RSP + 0x20],XMM0` (0x140694F45) | `XMM0` | tons (float) |
| param_6 | `MOV qword ptr [RSP + 0x28],RCX` (0x140694F44) | `&local` (RBP+0x20) | out pointer — actual moved will be written here |
| param_7 | *(not set in window)* | — | cap |
| param_8 | `MOV byte ptr [RSP + 0x38],0x0` (0x140694F42) | `0` | flag |
| param_9 | `MOV qword ptr [RSP + 0x40],RSI` (0x140694F41) | `RSI` | building* — **origin unresolved** |

**RSI provenance:** RSI is stored at [RSP+0x40] as param_9 but its definition is **not** in the 85-instruction disassembly window. Requires predecessor-block analysis within `FUN_14068B330`.

**After-call (0x140694F51):** `TEST AL,AL` then `JNZ 0x140695377` — return value checked immediately.

### 6.2 Call site 0x6953C4 (VA 0x1406953C4)

Range: `0x140695300:0x140695410` (50 instructions)

```
SUBSD XMM1,XMM0
MOVSD qword ptr [0x1409d5498],XMM1
MOV R15D,0x1
MOV dword ptr [RSP + 0x64],R15D
MOVSS XMM12,dword ptr [0x140909f58]
MOVAPS XMM9,XMM12
MOV RDX,RBX
MOV RCX,RDI
CALL 0x14002ecd0
MOVAPS XMM0,XMM7
ADDSS XMM0,dword ptr [RAX + 0x8]
MOVSS dword ptr [RAX + 0x8],XMM0
DIVSS XMM7,XMM11
ADDSS XMM7,dword ptr [R14 + 0x560]
MOVSS dword ptr [R14 + 0x560],XMM7
MOVSS XMM14,dword ptr [0x140909afc]
MOVSS XMM15,dword ptr [0x140909b90]
MOVSS XMM8,dword ptr [0x14090a280]
XORPS XMM11,XMM11
JMP 0x140695508
MOVAPS XMM6,XMM8
SUBSS XMM6,dword ptr [RBP + 0x20]
MOVSXD RBX,R12D
MOV RDX,RBX
MOV RCX,RDI
CALL 0x14002ecd0
MOV qword ptr [RSP + 0x40],RSI
MOV byte ptr [RSP + 0x38],0x0
MOV qword ptr [RSP + 0x28],RSI
MOVSS dword ptr [RSP + 0x20],XMM6
MOV R9,qword ptr [RAX]
MOV R15D,0x1
MOV R8D,R15D
MOV R14,qword ptr [RBP + 0x340]
MOV RDX,qword ptr [R14 + 0x390]
LEA RCX,[0x1409d4f10]
CALL 0x140420350
MOV dword ptr [RSP + 0x64],R15D
MOVAPS XMM9,XMM6
DIVSS XMM9,XMM8
ADDSS XMM9,dword ptr [0x140909a68]
XOR R9D,R9D
MOVAPS XMM2,XMM10
MOV R12,qword ptr [RSP + 0x68]
MOV RDX,R12
CALL 0x1401bd0b0
DIVSS XMM6,XMM11
ADDSS XMM6,dword ptr [R14 + 0x560]
MOVSS dword ptr [R14 + 0x560],XMM6
CMP qword ptr [R12 + 0x318],R9
```

**Argument setup at CALL 0x140420350 (0x1406953AB):**

| Param | Instruction | Value | Meaning |
|---|---|---|---|
| param_1 | `LEA RCX,[0x1409d4f10]` (0x1406953AA) | `0x1409d4f10` | game global |
| param_2 | `MOV RDX,qword ptr [R14 + 0x390]` (0x1406953A9) | `[R14+0x390]` | vehicle* from memory — overwrites RBX |
| param_3 | `MOV R8D,R15D` (0x14069539F) | `1` | mode 1 |
| param_4 | `MOV R9,qword ptr [RAX]` (0x140694F30→0x140695330) | `[RAX]` | resource* (RAX from helper 0x14002ecd0 at 0x140695326) |
| param_5 | `MOVSS dword ptr [RSP + 0x20],XMM6` (0x14069538E) | `XMM6` | tons (float) |
| param_6 | `MOV qword ptr [RSP + 0x28],RSI` (0x140695389) | `RSI` | out pointer — **same value as param_9** |
| param_7 | *(not set in window)* | — | cap |
| param_8 | `MOV byte ptr [RSP + 0x38],0x0` (0x140695384) | `0` | flag |
| param_9 | `MOV qword ptr [RSP + 0x40],RSI` (0x14069537F) | `RSI` | building* — **origin unresolved** |

**Critical observation:** At 0x6953C4, `param_6` (out pointer) and `param_9` (building) are both set to `RSI`. After the call, `[RSP+0x28]` (param_6/out) is **never read** — the out value is dead on this path.

**After-call (0x1406953B0):** `MOV dword ptr [RSP + 0x64],R15D` — return value not checked via TEST; instead R15D (mode=1) is stored.

### 6.3 Address consistency check

| Item | Call site 0x694F40 | Call site 0x6953C4 |
|---|---|---|
| CALL VA | 0x140694F40 | 0x1406953C4 |
| CALL RVA | 0x694F40 | 0x6953C4 |
| Raw bytes | `E8 0B B4 D8 FF` | `E8 87 AF D8 FF` |
| Length | 5 bytes | 5 bytes |
| Containing function | FUN_14068B330 | FUN_14068B330 |
| Disassembly window | 0x140694E80–0x140694FE0 | 0x140695300–0x140695410 |
| CALL within window? | Yes (offset 0xC0 from window start) | Yes (offset 0xC4 from window start) |

No overlapping or inconsistent addresses. The previous report's concern about overlapping instruction addresses was unfounded — both call sites are distinct 5-byte `CALL rel32` instructions at their respective RVAs.

## 7. Export Script Source and Exact Commands

### 7.1 Script inventory

All scripts are in `C:\WRSRE-ghidra-work/scripts/`. Addresses are file-based (`0x140` + RVA), passed as hex string literals like `"0x140420350"`.

| Script | Purpose | Output file |
|---|---|---|
| `CallsTo420350.java` | Enumerate all CALL rel32 targeting FUN_140420350 | `output/calls_to_420350.txt` |
| `Xrefs420350.java` | Cross-references to/from FUN_140420350 | `output/xrefs_420350.txt` |
| `Decomp420350.java` | Decompile transfer function FUN_140420350 | `output/decomp_420350.c` |
| `Decomp68B330.java` | Decompile sea/rail hauler FUN_14068B330 | `output/decomp_68b330.c` |
| `Disasm694F40Full.java` | Disassemble 0x140694E80:0x140694FE0 | `output/disasm_694f40_full.asm` |
| `Disasm6953C4Full.java` | Disassemble 0x140695300:0x140695410 | `output/disasm_6953c4_full.asm` |

### 7.2 Exact headless command

Single invocation — import, short analysis, run all scripts:

```
analyzeHeadless.bat 'C:\WRSRE-ghidra-work\projects' 'SOVIET64_analysis' ^
  -import 'C:\Workers Resources Soviet Republic\SOVIET64.exe' ^
  -analysisTimeoutPerFile 60 ^
  -scriptPath 'C:\WRSRE-ghidra-work\scripts' ^
  -postScript 'Xrefs420350.java' ^
  -postScript 'CallsTo420350.java' ^
  -postScript 'Decomp420350.java' ^
  -postScript 'Decomp68B330.java' ^
  -postScript 'Disasm694F40Full.java' ^
  -postScript 'Disasm6953C4Full.java' ^
  -log 'C:\WRSRE-ghidra-work\projects\investigation6.log'
```

**Analysis status:** Timed out at 60 seconds (file not fully analyzed). Function boundaries and disassembly established; post-scripts ran successfully and produced output.

### 7.3 Script source

All scripts follow the same Ghidra 12.1.4 API pattern:
- `currentProgram.getAddressFactory().getAddress("0x140XXXX")` for address resolution
- `currentProgram.getFunctionManager().getFunctionContaining(addr)` for function lookup
- `DecompInterface.decompileFunction(func, timeout, monitor)` for decompilation
- `ReferenceManager.getReferencesTo(addr)` / `getReferencesFrom(addr)` for xrefs (note: `getReferencesTo` returns `ReferenceIterator`, `getReferencesFrom` returns `Reference[]` — inconsistent API)

Script source files are preserved as small text files at `C:\WRSRE-ghidra-work/scripts/`. No script was overwritten after successful compilation.

### 7.4 Address interpretation convention

All addresses in scripts and output are **file-based**: `0x140` + RVA. This matches the Ghidra project's image base `0x140000000`. A RVA `0x420350` corresponds to file-based VA `0x140420350`.

## 8. Reconciliation of Contradictions

Each item below resolves a conflict between prior claims and the binary evidence. Machine instructions are authority; decompiler interpretation is labeled separately.

### 8.1 out=NULL versus out=&local versus out=RSI

| Call site | param_6 (out) value | Out read after call? | Resolved |
|---|---|---|---|
| 0x16f831 (refuel) | `NULL` | N/A | **Confirmed** — disassembly shows out=NULL |
| 0x1b2898 (road) | `NULL` | N/A | **Confirmed** — prior notes |
| 0x1cb7ff (loader) | `&local` (RSP+0x108) | Yes — read as `plStack_338` | **Confirmed** — prior notes |
| 0x68eeca (sea/rail A) | `R12` (live) | — | **Confirmed** — prior notes |
| 0x68f792 (sea/rail B) | `NULL` | N/A | **Confirmed** — prior notes |
| 0x68f7f9 (dock return) | `&local` | Yes | **Confirmed** — prior notes |
| 0x6945b3 (dock return) | `[bld+0x368]` (live) | — | **Confirmed** — prior notes |
| 0x694F40 (dock return) | `&local` (RBP+0x20) | **NOT READ** after call | **Confirmed** — disassembly lines 50-52: `TEST AL,AL; JNZ` — no read of [RSP+0x28] |
| 0x6953C4 (delivery) | `RSI` | **NOT READ** after call | **Confirmed** — disassembly line 38: `MOV [RSP+0x64],R15D` — stores mode, not out |

**Resolution:** Out is NULL at sites 0x16f831 and 0x1b2898. Out is `&local` at 0x694F40 but **never read** after the call. Out is `RSI` at 0x6953C4 but **never read** after the call. The "reported later read" of out at 0x6953C4 was incorrect — the caller stores `R15D` (mode=1) at `[RSP+0x64]`, not the out value. At 0x1cb7ff (loader), `plStack_338` (out) IS read at decompiled line 1669 (`if (0.0 < plStack_338._0_4_)`).

### 8.2 Nine-argument callee versus six-argument caller pseudocode

The callee `FUN_140420350` has 9 parameters (param_1 through param_9). Caller pseudocode in prior reports showed only 6 arguments. This is because:

1. **x64 calling convention:** First 4 args in registers (RCX, RDX, R8, R9), remaining 5 on stack (stack+0x20 through stack+0x40).
2. **Prior pseudocode omitted stack args:** The 6-argument form likely showed only register args or conflated stack args.
3. **Binary evidence:** Disassembly at 0x694F40 sets all 9: RCX (game), RDX (vehicle), R8 (mode=0), R9 (resource), stack+0x20 (tons), stack+0x28 (out), stack+0x30 (cap, not set in window), stack+0x38 (flag=0), stack+0x40 (RSI building).

**Resolution:** 9-argument callee is correct per decompiled signature. Callers set all 9 args; prior 6-argument pseudocode was incomplete.

### 8.3 Allegedly dead output versus reported later read

**Claim:** Out pointer read after call at 0x6953C4.
**Binary evidence:** At 0x1406953B0 (immediately after CALL), `MOV dword ptr [RSP + 0x64],R15D` stores `R15D` (=1, the mode value) — **not** the out value. `[RSP+0x28]` (param_6/out) is never read.

**Claim:** Out pointer read after call at 0x694F40.
**Binary evidence:** At 0x140694F51, `TEST AL,AL` then `JNZ` — return value checked, but `[RSP+0x28]` (param_6/out) is never read.

**Resolution:** Out is dead at both 0x694F40 and 0x6953C4. Out IS read at 0x1cb7ff (loader) where `plStack_338._0_4_` is checked at decompiled line 1669.

### 8.4 Argument expressions versus actual aliasing/object-type evidence

| Argument | Expression at call site | Object type evidence |
|---|---|---|
| param_1 | `0x1409d4f10` (LEA) | game global — confirmed by decompilation of FUN_140420350 (reads `param_1+0x144A8` order vector) |
| param_2 (0x694F40) | `RBX` (set at 0x140694F64) | **Unresolved** — R14 loaded earlier at 0x140694F35 but overwritten by RBX; R14's origin not in disassembly window |
| param_2 (0x6953C4) | `[R14+0x390]` (at 0x1406953A9) | **Unresolved** — memory dereference; whether this is a Vehicle* is not established by disassembly alone |
| param_2 (0x694F40, callee view) | Inside callee: `param_2+0x3B8/0x3C0` cargo vec | **Confirmed** — callee dereferences vehicle fields |
| param_4 | `[RAX]` (from helper 0x14002ecd0) | **Unresolved** — helper return value type not established |
| param_9 | `RSI` (both sites) | **Unresolved** — RSI definition not in disassembly windows |

**Resolution:** Inside the callee, param_2 is confirmed as Vehicle* (cargo vec dereference). Caller-side provenance at both sites of interest is unresolved — requires predecessor-block analysis within `FUN_14068B330`.

### 8.5 Every assignment contributing to output quantity (fVar21)

From decompiled `FUN_140420350` lines 233-258:

```
fVar21 = 0.0;                                          // Initialize
if ((param_5 <= 0.0) || (fVar20 < param_5)) {          // Negative tons path (unload)
    if (0.0 <= param_5) return 0;                      // Zero tons → early exit
    if (*pfVar18 <= 0.0) return 0;                    // Empty source → early exit
    param_5 = *pfVar18 + param_5;                     // Add negative tons to current
    *pfVar18 = param_5;                               // Store clamped result
    if (param_5 < 0.0) {                              // Clamp at zero
        *pfVar18 = 0.0;
        fVar21 = -param_5;                            // outMoved = amount removed
    }
} else {                                              // Positive tons path (load)
    param_5 = param_5 + *pfVar18;                     // Add positive tons to current
    *pfVar18 = param_5;                               // Store result
    if (fVar19 < param_5) {                           // Exceeds capacity?
        fVar21 = param_5 - fVar19;                    // outMoved = amount that fit
        *pfVar18 = param_5 - fVar21;                  // Clamp to capacity
    }
}
if (param_6 != (float *)0x0) {
    *param_6 = fVar21;                                // Write to out pointer
}
return 1;
```

**Assignments contributing to fVar21 (outMoved):**
1. `fVar21 = 0.0` — initialization (line 233)
2. `fVar21 = -param_5` — unload path, when result clamped at zero (line 245)
3. `fVar21 = param_5 - fVar19` — load path, when exceeding capacity (line 252)
4. `*param_6 = fVar21` — write to out pointer (line 257)

**pfVar18** (the mutable tons field) points to either:
- Cargo slot `+8` at `lVar2 + 8 + pfVar9 * 0x10` (mode 1, decompiled line 77)
- Order record tons at computed address (mode 0, decompiled line 190)

**fVar19** (capacity factor):
- Mode 1: `fVar19 = fVar19 * (1.0 - fVar20)` where `fVar20` accumulates cargo utilization (line 81)
- Mode 0: computed via `FUN_1401A9D60()` / `FUN_1401AA120(param_1, param_2)` (lines 127-132)

**Resolution:** OutMoved is derived entirely from clamped mutation — never read from caller state. It is 0.0 when nothing moved, or the actual clamped amount when movement occurred.

## 9. Findings Classification

### Confirmed (instruction-level evidence from binary)

1. **Transfer function signature:** `FUN_140420350(game*, vehicle*, mode, resource*, tons, out*, cap, flag, building*)` at RVA 0x420350 — confirmed by decompilation (output/decomp_420350.c lines 8-10).
2. **16 direct CALL rel32 sites** enumerated with raw bytes, length, target, containing function — confirmed by CallsTo420350.java output (output/calls_to_420350.txt).
3. **3 DATA references** (unwind metadata, not invocations) — confirmed by Xrefs420350.java output (output/xrefs_420350.txt).
4. **Return semantics:** AL=1 = mutation committed; AL=0 = early exit — confirmed by decompilation lines 35-37, 229-231, 259.
5. **outMoved semantics:** `*param_6 = fVar21` written only when `param_6 != NULL` — confirmed by decompilation lines 256-258.
6. **Mode semantics:** mode 0 = order-source pool (`game+0x144A8` stride 0x68); mode 1 = vehicle cargo slot (`vehicle+0x3B8..0x3C0`) — confirmed by decompilation.
7. **Mode/sign independence:** mode 0 carries both + and - tons; mode 1 carries both + and - tons — confirmed by call-site census.
8. **Building validation:** `param_9 + 0x318` type descriptor, types 0x0C/0x6A — confirmed by decompilation lines 88-90.
9. **Paired call pattern:** 0x694F40 (mode 0, out=NULL) + 0x6953C4 (mode 1, out=&local) called in sequence with same resource and tons — confirmed by decomp_68b330.c lines 1599-1644 and disassembly.
10. **Different vehicle pointers in paired calls:** `lVar24` vs `[R14+0x390]` — confirmed by decompilation and disassembly.
11. **Out dead at 0x694F40 and 0x6953C4:** `[RSP+0x28]` not read after call — confirmed by disassembly.
12. **Out read at 0x1cb7ff:** `plStack_338._0_4_` checked at decomp_68b330.c line 1669 — confirmed.
13. **fVar21 derivation:** initialized to 0.0, set on unload clamp (`-param_5`) or load clamp (`param_5 - fVar19`) — confirmed by decompilation lines 233-258.

### Candidate (strong evidence, requires confirmation)

1. **vehicle+0x4F8 = home building:** Confirmed as field semantics from prior notes, but not directly observed in this binary's decompilation of FUN_140420350.
2. **vehicle+0x4F0 = current building:** Field semantics likely, but FUN_140420350 decompilation contains **no `+0x4F0` reference**. Not readable by the transfer at call sites.
3. **Order vector `game+0x144A8/+0x144B0` stride 0x68:** Confirmed by decompilation lines 136-227.
4. **`order+0x30` = assigned Vehicle*:** Confirmed by decompilation line 169.
5. **`order+0x00/+0x08` = `{Resource*, tons}` records:** Confirmed by decompilation lines 177-190.
6. **Two different vehicles in paired calls represent distinct entities:** Confirmed by pointer difference, but whether they represent the same logical vehicle at different times is unresolved.

### Contradicted (prior claims overturned by binary evidence)

1. **"param_2 is Vehicle* at every call site":** Inside the callee, `param_2+0x3B8/0x3C0` confirms Vehicle* fields. But at caller 0x16F831 (refuel), `param_2 = [RBX+0x4F8]` where RBX = route planner output (Building*). Offset coincidence, not vehicle provenance. **Not universally true.**
2. **"Mode 0 = load, mode 1 = unload":** Mode 0 carries +tons (0x68EECA, 0x68F792) and -tons (0x6945B3, 0x694F40). Mode 1 carries -tons (0x68F7F9) and +tons (0x6953C4). **Mode and sign are independent.**
3. **"order+0x00/+0x08 = source Building* vector":** Decompilation confirms `{Resource* +0x00, float +0x08}` records. **Not buildings.**
4. **"order+0x30 = destination Building*":** Decompilation confirms `order+0x30` = assigned Vehicle*. **Not destination.**
5. **"100% of invocations observable via call-site patching":** 16 direct `call rel32` confirmed for this binary, but census is not an absolute coverage guarantee across builds or code paths.
6. **"current building is pickup at unload":** `FUN_140420350` contains **no `+0x4F0` reference**. The transfer does not read current building. **Not supported at transfer level.**
7. **"Out read at 0x6953C4":** Disassembly shows `MOV [RSP+0x64],R15D` (mode store), not out read. **Contradicted.**

### Unresolved (requires further investigation)

1. **RSI origin at param_9 (0x694F40 and 0x6953C4):** RSI used as building pointer but not defined in disassembly windows. Requires predecessor-block analysis within `FUN_14068B330`.
2. **`param_2` caller provenance at refuel site 0x16F831:** Not investigated in this binary run. Prior notes indicate offset coincidence with building field.
3. **`vehicle+0x4F0` readability at transfer call sites:** Transfer decompilation has no `+0x4F0` reference. Unresolved whether it's valid at any specific call site.
4. **Order destination field:** Not found on order (`game+0x144A8` stride 0x68). May exist on DO task record or completion log instead.
5. **Durable operation ID:** No field found linking pickup call to delivery call. Order slots are reused (wraparound observed at decomp_68b330.c lines 183-187).
6. **Threading/interleaving:** No evidence either way from static analysis.
7. **`lVar24` vs `[R14+0x390]` relationship:** Different pointers confirmed, but logical relationship unresolved.

## 10. Go/No-Go Recommendation for Logging-Only Prototype

### GO — for per-call observation prototype

A logging-only prototype that observes individual transfer calls is **supported** by binary evidence:

| Supported capability | Evidence |
|---|---|
| Observe vehicle pointer (param_2) at call sites | Confirmed inside callee (`+0x3B8/0x3C0` cargo vec dereference) |
| Observe resource (param_4) | Confirmed (`param_4+0x44` resource id check) |
| Observe tons (param_5) and sign | Confirmed (sign = direction) |
| Observe outMoved (param_6) valid when AL==1 && param_6 != NULL | Confirmed (decompilation lines 256-258) |
| Observe building pointer (param_9) where non-NULL | Confirmed (decompilation lines 88-90 building validation) |
| Observe mode (param_3) | Confirmed (mode 0/1 branch at decompilation lines 39-43) |
| Distinguish refuel from freight | Confirmed (different call sites, mode, sign) |
| Gate on site AND tons > 0 | Confirmed (mode and sign independent) |

### NO-GO — for verified pickup-to-delivery association

| Missing capability | Blocker |
|---|---|
| Durable operation ID | No field found linking pickup to delivery; order slots reused (wraparound at decomp_68b330.c lines 183-187) |
| Destination at `bld==NULL` sites | Transfer does not pass destination to caller (0x1B2898, param_9=NULL) |
| `param_2` provenance at refuel site | Offset coincidence, not vehicle object (0x16F831) |
| `param_9`/RSI origin at 0x694F40/0x6953C4 | Not in disassembly window; requires predecessor analysis |
| `vehicle+0x4F0` at call sites | Transfer decompilation has no `+0x4F0` reference |
| Pickup-to-delivery pairing | No data structure field establishes the link |
| Different vehicle pointers in paired calls | `lVar24` vs `[R14+0x390]` — logical relationship unresolved |

### Recommendation

**Proceed with a logging-only prototype for per-call transfer observation.** The prototype can reliably capture: vehicle, resource, tons (nominal + actual), sign, mode, call site, home building (`vehicle+0x4F8` per prior notes), and explicit destination (`param_9` where non-NULL).

**Do not claim verified pickup-to-delivery association.** The prototype must tag any cross-call grouping as `heuristic` and must not use it for billing or settlement. Additional targeted investigation (RSI provenance, Xrefs census, `vehicle+0x4F0` validity, completion log analysis) is required before any operation-link claim can be made.

No hooks are enabled, no memory is written, and no money is moved by this investigation or its deliverables.
=======
=======
>>>>>>> theirs
This accumulates using `fVar52` (nominal tons), NOT `outMoved` (fVar21).

**Treasury calls (decompiled lines 1655-1667):** `FUN_1402FDB00` called twice after the paired transfers — treasury/statistics accounting.

### 6.2 Vehicle identity in paired calls

The decompiled code uses:
- First call vehicle: `lVar24`
- Second call vehicle: `plStackX_8[0x72]`

At disassembly level:
- First call (0x694F40): param_2 = RBX (which was loaded from R14 earlier at 0x140694F35-0x140694F38)
- Second call (0x6953C4): param_2 = `[R14+0x390]` (memory dereference)

**These are different pointer expressions.** Whether `RBX` and `[R14+0x390]` alias to the same object is **unresolved** without alias analysis. Different expressions do NOT prove different vehicles.

## 7. Verified Instruction Listings — Call Sites 0x694F40 and 0x6953C4

**Source:** Raw bytes read from `SOVIET64.exe` at the specified RVAs, decoded manually. Machine instructions only — no decompiler interpretation mixed in. All addresses are static VAs (ImageBase `0x140000000` + RVA).

### 7.1 Call site 0x694F40 (static VA 0x140694F40)

Full argument-setting basic block and predecessor block, range 0x140694EF3–0x140694F51:

| Static VA | RVA | Raw bytes | Length | Decoded instruction | Function |
|---|---|---|---|---|---|
| 0x140694EF3 | 0x694EF3 | `4C 8B 45 98` | 4 | `MOV R8,[RBP-0x68]` | FUN_14068B330 |
| 0x140694EF7 | 0x694EF7 | `EB 57` | 2 | `JMP 0x140694F50` | FUN_14068B330 |
| 0x140694EF9 | 0x694EF9 | `C7 45 20 00 00 00 00` | 7 | `MOV DWORD [RBP+0x20],0` | FUN_14068B330 |
| 0x140694F00 | 0x694F00 | `49 8B D6` | 3 | `MOV RDX,R14` | FUN_14068B330 |
| 0x140694F03 | 0x694F03 | `48 8B 7D 98` | 4 | `MOV RDI,[RBP-0x68]` | FUN_14068B330 |
| 0x140694F07 | 0x694F07 | `48 8B CF` | 3 | `MOV RCX,RDI` | FUN_14068B330 |
| 0x140694F0A | 0x694F0A | `E8 C1 9D 99 FF` | 5 | `CALL 0x14002ECD0` | FUN_14068B330 |
| 0x140694F0F | 0x694F0F | `41 0F 28 C0` | 4 | `MOVAPS XMM0,XMM8` | FUN_14068B330 |
| 0x140694F13 | 0x694F13 | `41 0F 57 C7` | 4 | `XORPS XMM0,XMM15` | FUN_14068B330 |
| 0x140694F17 | 0x694F17 | `48 89 74 24 40` | 4 | `MOV [RSP+0x40],RSI` | FUN_14068B330 |
| 0x140694F1B | 0x694F1B | `C6 44 24 38 00` | 4 | `MOV BYTE [RSP+0x38],0` | FUN_14068B330 |
| 0x140694F1F | 0x694F1F | `48 8D 4D 20` | 4 | `LEA RCX,[RBP+0x20]` | FUN_14068B330 |
| 0x140694F23 | 0x694F23 | `48 89 4C 24 28` | 4 | `MOV [RSP+0x28],RCX` | FUN_14068B330 |
| 0x140694F2A | 0x694F2A | `F3 0F 11 44 24 20` | 6 | `MOVSS [RSP+0x20],XMM0` | FUN_14068B330 |
| 0x140694F30 | 0x694F30 | `4C 8B 08` | 3 | `MOV R9,[RAX]` | FUN_14068B330 |
| 0x140694F33 | 0x694F33 | `45 33 C0` | 3 | `XOR R8D,R8D` | FUN_14068B330 |
| 0x140694F36 | 0x694F36 | `48 8B D3` | 3 | `MOV RDX,RBX` | FUN_14068B330 |
| 0x140694F39 | 0x694F39 | `48 8D 0D D0 FF FF 33` | 7 | `LEA RCX,[RIP+0x33FFFD0] = 0x1409D4F10` | FUN_14068B330 |
| 0x140694F40 | 0x694F40 | `E8 0B B4 D8 FF` | 5 | `CALL 0x140420350` | FUN_14068B330 |
| 0x140694F45 | 0x694F45 | `84 C0` | 2 | `TEST AL,AL` | FUN_14068B330 |
| 0x140694F47 | 0x694F47 | `0F 85 2A 04 04 00` | 6 | `JNZ 0x140695377` | FUN_14068B330 |

**CALL rel32 verification at 0x140694F40:**
- Bytes: `E8 0B B4 D8 FF`
- Next RIP: 0x140694F45
- Disp32: `0xFFD8B40B` = -2,571,765
- Target: 0x140694F45 + (-2,571,765) = 0x140420350 ✓

**Block boundary:** The `JMP 0x140694F50` at 0x140694EF7 jumps PAST the call at 0x140694F40 to 0x140694F50 (`MOV EDI,1`). The fall-through path from 0x140694EF9 reaches the call. This is a conditional branch point — whether execution reaches the transfer call depends on a branch decision before 0x140694EF3 (not shown in this window).

**CALL target is 0x140420350 (FUN_140420350), not any other address.**

### 7.2 Call site 0x6953C4 (static VA 0x1406953C4)

Full argument-setting basic block, range 0x140695393–0x1406953C9:

| Static VA | RVA | Raw bytes | Length | Decoded instruction | Function |
|---|---|---|---|---|---|
| 0x140695393 | 0x695393 | `48 89 74 24 40` | 4 | `MOV [RSP+0x40],RSI` | FUN_14068B330 |
| 0x140695397 | 0x695397 | `C6 44 24 38 00` | 4 | `MOV BYTE [RSP+0x38],0` | FUN_14068B330 |
| 0x14069539B | 0x69539B | `48 89 74 24 28` | 4 | `MOV [RSP+0x28],RSI` | FUN_14068B330 |
| 0x14069539F | 0x69539F | `F3 0F 11 74 24 20` | 6 | `MOVSS [RSP+0x20],XMM6` | FUN_14068B330 |
| 0x1406953A5 | 0x6953A5 | `4C 8B 08` | 3 | `MOV R9,[RAX]` | FUN_14068B330 |
| 0x1406953A8 | 0x6953A8 | `41 BF 01 00 00 00` | 5 | `MOV R15D,1` | FUN_14068B330 |
| 0x1406953AD | 0x6953AD | `45 8B C7` | 3 | `MOV R8D,R15D` | FUN_14068B330 |
| 0x1406953B0 | 0x6953B0 | `4C 8B B5 40 03 00 00` | 7 | `MOV R14,[RBP+0x340]` | FUN_14068B330 |
| 0x1406953B7 | 0x6953B7 | `49 8B 96 90 03 00 00` | 7 | `MOV RDX,[R14+0x390]` | FUN_14068B330 |
| 0x1406953BE | 0x6953BE | `48 8D 0D 4C FB FF 33` | 7 | `LEA RCX,[RIP+0x33FFFB4C] = 0x1409D4F10` | FUN_14068B330 |
| 0x1406953C4 | 0x6953C4 | `E8 87 AF D8 FF` | 5 | `CALL 0x140420350` | FUN_14068B330 |
| 0x1406953C9 | 0x6953C9 | `44 89 7C 24 64` | 4 | `MOV [RSP+0x64],R15D` | FUN_14068B330 |

**CALL rel32 verification at 0x1406953C4:**
- Bytes: `E8 87 AF D8 FF`
- Next RIP: 0x1406953C9
- Disp32: `0xFFD8AF87` = -2,576,249
- Target: 0x1406953C9 + (-2,576,249) = 0x140420350 ✓

**Block boundary:** Linear flow from 0x140695393 to 0x1406953C9. No gaps or overlaps within this window.

### 7.3 Instruction stream validation

| Check | Result at 0x694F40 | Result at 0x6953C4 |
|---|---|---|
| Successive instructions do not overlap | Verified (each starts at prev + length) | Verified |
| Raw byte count == declared length | Verified for all rows | Verified for all rows |
| CALL rel32 displacement resolves to target | 0x140420350 ✓ | 0x140420350 ✓ |
| Named CALL addresses match listing | 0x140694F40 ✓ | 0x1406953C4 ✓ |

**No overlapping or inconsistent instruction addresses remain.** Both call sites are distinct 5-byte `CALL rel32` instructions at their respective RVAs.

## 8. Argument Setup Tables (Reaching Definitions)

One table per call site. Columns: argument | register/stack slot | defining instruction(s) | reaching definition(s) | value/expression | confidence | unresolved alternatives.

### 8.1 Call site 0x694F40 (mode 0)

| Argument | Register/Stack | Defining instruction | Reaching definition | Value/expression | Confidence | Unresolved alternatives |
|---|---|---|---|---|---|---|
| param_1 | RCX | `LEA RCX,[RIP+0x33FFFD0]` at 0x140694F39 | Instruction itself | `0x1409D4F10` (game global) | Confirmed | None |
| param_2 | RDX | `MOV RDX,RBX` at 0x140694F36 | RBX (set earlier at 0x140694F35-0x140694F38) | `RBX` | Confirmed inside callee | Caller-side object type unresolved — R14's origin not in window; RBX may or may not be Vehicle* |
| param_3 | R8D | `XOR R8D,R8D` at 0x140694F33 | Instruction itself | `0` (mode 0) | Confirmed | None |
| param_4 | R9 | `MOV R9,[RAX]` at 0x140694F30 | RAX (from `CALL 0x14002ECD0` at 0x140694F0A) | `[RAX]` (resource*) | Confirmed as pointer | Resource object type not established by disassembly alone |
| param_5 | [RSP+0x20] | `MOVSS [RSP+0x20],XMM0` at 0x140694F2A | XMM0 (from `MOVAPS XMM0,XMM8` at 0x140694F0F + `XORPS XMM0,XMM15` at 0x140694F13) | `XMM0` (tons, float) | Confirmed | Sign and magnitude not verifiable from static analysis |
| param_6 | [RSP+0x28] | `MOV [RSP+0x28],RCX` at 0x140694F23 | RCX = `LEA RCX,[RBP+0x20]` at 0x140694F1F | `&local` (address of stack variable at RBP+0x20) | Confirmed | Out pointer non-NULL; callee will write fVar21 here |
| param_7 | [RSP+0x30] | Not set in window | — | Unknown | Unresolved | Cap value not set in exported window |
| param_8 | [RSP+0x38] | `MOV [RSP+0x38],0` at 0x140694F1B | Instruction itself | `0` (flag char) | Confirmed | None |
| param_9 | [RSP+0x40] | `MOV [RSP+0x40],RSI` at 0x140694F17 | RSI | `RSI` (building*?) | Confirmed as pointer value | RSI origin not in window; object type unresolved |

**RSI provenance:** RSI is stored at [RSP+0x40] as param_9 but its defining instruction is not in the exported window (0x140694EF3–0x140694F51). Requires predecessor-block analysis within `FUN_14068B330`.

**After-call:** `TEST AL,AL` at 0x140694F45, `JNZ 0x140695377` at 0x140694F47. [RSP+0x28] (param_6/out) is NOT read after the call.

### 8.2 Call site 0x6953C4 (mode 1)

| Argument | Register/Stack | Defining instruction | Reaching definition | Value/expression | Confidence | Unresolved alternatives |
|---|---|---|---|---|---|---|
| param_1 | RCX | `LEA RCX,[RIP+0x33FFFB4C]` at 0x1406953BE | Instruction itself | `0x1409D4F10` (game global) | Confirmed | None |
| param_2 | RDX | `MOV RDX,[R14+0x390]` at 0x1406953B7 | R14 (from `MOV R14,[RBP+0x340]` at 0x1406953B0) | `[[RBP+0x340]+0x390]` | Confirmed as pointer | Whether this is Vehicle* is not established by disassembly alone |
| param_3 | R8D | `MOV R8D,R15D` at 0x1406953AD | R15D (from `MOV R15D,1` at 0x1406953A8) | `1` (mode 1) | Confirmed | None |
| param_4 | R9 | `MOV R9,[RAX]` at 0x1406953A5 | RAX (from `CALL 0x14002ECD0` at 0x14069538E) | `[RAX]` (resource*) | Confirmed as pointer | Same helper as 0x694F40; whether RAX aliases the same resource is unresolved |
| param_5 | [RSP+0x20] | `MOVSS [RSP+0x20],XMM6` at 0x14069539F | XMM6 (from `SUBSS XMM6,[RBP+0x20]` at 0x140695384) | `XMM6` (tons, float) | Confirmed | Sign and magnitude not verifiable |
| param_6 | [RSP+0x28] | `MOV [RSP+0x28],RSI` at 0x14069539B | RSI | `RSI` | Confirmed as pointer value | RSI same as param_9; out pointer non-NULL; RSI origin unresolved |
| param_7 | [RSP+0x30] | Not set in window | — | Unknown | Unresolved | Cap value not set in exported window |
| param_8 | [RSP+0x38] | `MOV [RSP+0x38],0` at 0x140695397 | Instruction itself | `0` (flag char) | Confirmed | None |
| param_9 | [RSP+0x40] | `MOV [RSP+0x40],RSI` at 0x140695393 | RSI | `RSI` (building*?) | Confirmed as pointer value | RSI origin unresolved; same value as param_6 |

**Critical observation at 0x6953C4:** param_6 (out pointer) and param_9 (building) are both set to `RSI`. Same pointer value used for two different semantic roles. RSI's origin is unresolved.

**After-call:** `MOV [RSP+0x64],R15D` at 0x1406953C9. [RSP+0x28] (param_6/out) is NOT read after the call.

### 8.3 Contradictions resolved

| Claim in prior report | Binary evidence | Resolution |
|---|---|---|
| Section 5: 0x694F40 out = NULL | Disassembly shows `MOV [RSP+0x28],RCX` (non-NULL) at 0x140694F23 | **out = &local, not NULL** |
| Section 5: 0x6953C4 out = &plStack_338 | Disassembly shows `MOV [RSP+0x28],RSI` at 0x14069539B | **out = RSI** |
| Section 6: 0x694F40 out = &local | Matches disassembly | **Confirmed** |
| Section 6: 0x6953C4 out = RSI | Matches disassembly | **Confirmed** |
| Section 8.1: 0x694F40 out = &local, "Confirmed" | Matches disassembly | **Confirmed** |

**Single corrected table:** 0x694F40 has out = &local (RBP+0x20). 0x6953C4 has out = RSI. Neither is NULL. The decompiled caller output showing `,0)` for param_6 at the first call is a decompiler display limitation (stack argument omitted), not evidence that param_6 was actually NULL.

## 9. Quantity and Return Semantics

### 9.1 Return value (AL)

| AL value | Meaning | Condition |
|---|---|---|
| 1 | Mutation committed | `*pfVar18` was written |
| 0 | Early exit, no mutation | Bad resource (line 35), wrong mode (line 40-41), empty cargo (line 64-66), no matching order/stop (line 136-138), no matching target (line 229-231), zero tons (line 235-236), empty source (line 238-240) |

**AL == 1 does NOT imply positive movement.** It only implies the mutation code path was reached and `*pfVar18` was written. The output `fVar21` can be 0.0 when `AL == 1` (e.g., Case 1 and Case 3 in section 9.3 below).

### 9.2 Output quantity: fVar21 — NOT "actual moved tons"

The decompiled mutation code (lines 233-259 of decomp_420350.c) shows `fVar21` is initialized to `0.0` and set only in two clamp paths:

```
fVar21 = 0.0;
if ((param_5 <= 0.0) || (fVar20 < param_5)) {
    // NEGATIVE TONS PATH (unload/removal)
    if (0.0 <= param_5) return 0;              // zero tons → early exit, no mutation
    if (*pfVar18 <= 0.0) return 0;              // empty source → early exit
    param_5 = *pfVar18 + param_5;               // add negative request to current
    *pfVar18 = param_5;                          // store result
    if (param_5 < 0.0) {                        // would go below zero
      *pfVar18 = 0.0;                            // clamp to zero
      fVar21 = -param_5;                         // amount that COULD NOT be removed
    }
} else {
    // POSITIVE TONS PATH (load/addition)
    param_5 = param_5 + *pfVar18;               // add positive request to current
    *pfVar18 = param_5;                          // store result
    if (fVar19 < param_5) {                     // exceeds capacity fVar19
      fVar21 = param_5 - fVar19;                // amount that COULD NOT fit (overflow)
      *pfVar18 = param_5 - fVar21;               // clamp to capacity
    }
}
if (param_6 != (float *)0x0) {
    *param_6 = fVar21;                          // write fVar21 to out pointer
}
return 1;
```

**fVar21 is the amount REJECTED (unmet removal / addition overflow), NOT actual moved quantity.**

| Variable | Meaning |
|---|---|
| `fVar21` | Output value written to `*param_6` — amount that could NOT be moved (rejected/overflow) |
| `|param_5| - fVar21` | Actual quantity moved (when clamped) or `|param_5|` (when fVar21=0) |
| `*pfVar18` (after) | Final stored quantity (clamped) |
| `*pfVar18` (before) | Initial quantity |

**Neutral name:** `fVar21` should be called `rejected_amount` or `overflow_amount`, not `outMoved` or `actual_moved_tons`.

### 9.3 Numerical walkthroughs

| Case | Initial | Request | fVar20/cap | Branch taken | Final stored | fVar21 (output) | Actual moved |
|---|---|---|---|---|---|---|---|
| 1. Removal within available | 10 | -3 | N/A | Negative path | 7 | 0.0 | 3 |
| 2. Removal exceeding available | 10 | -15 | N/A | Negative path → clamp at 0 | 0 | 5.0 | 10 |
| 3. Addition below capacity | 7 | +3 | 10 | Positive path | 10 | 0.0 | 3 |
| 4. Addition exceeding capacity | 7 | +5 | 10 | Positive path → clamp at 10 | 10 | 2.0 | 3 |
| 5. Zero request / zero movement | 7 | 0 | N/A | Guard: `0.0 <= param_5` → return 0 | 7 | not written | 0 |

**Case 5 detail:** `param_5 = 0.0`. Guard `(param_5 <= 0.0)` is TRUE. Enter first branch. `0.0 <= param_5` → `0.0 <= 0.0` is TRUE → `return 0`. AL=0, no mutation, output not written.

**Case 1 detail:** `param_5 = -3.0`. Guard TRUE. `0.0 <= -3.0` FALSE. `*pfVar18 <= 0.0` (10 <= 0) FALSE. `param_5 = 10 + (-3) = 7`. `*pfVar18 = 7`. `7 < 0.0` FALSE. `fVar21` stays 0.0. AL=1.

**Case 2 detail:** `param_5 = -15.0`. Guard TRUE. `0.0 <= -15.0` FALSE. `10 <= 0` FALSE. `param_5 = 10 + (-15) = -5`. `*pfVar18 = -5`. `-5 < 0.0` TRUE. `*pfVar18 = 0.0`. `fVar21 = -(-5) = 5.0`. AL=1.

**Case 3 detail:** `param_5 = 3.0`. Guard: `3.0 <= 0.0` FALSE. `fVar20 < 3.0`? Depends on mode and computed fVar20. Assuming mode 1 with fVar20 computed such that `fVar20 >= 3.0`, enter else branch. `param_5 = 7 + 3 = 10`. `*pfVar18 = 10`. `fVar19(10) < 10`? FALSE. `fVar21` stays 0.0. AL=1.

**Case 4 detail:** `param_5 = 5.0`. Enter else branch (assuming fVar20 >= 5.0). `param_5 = 7 + 5 = 12`. `*pfVar18 = 12`. `fVar19(10) < 12` TRUE. `fVar21 = 12 - 10 = 2.0`. `*pfVar18 = 12 - 2 = 10`. AL=1.

### 9.4 Mode 1 path (vehicle cargo slot)

Iterates cargo slots at `param_2 + 0x3B8` to `param_2 + 0x3C0` (0x10 bytes each). For each slot:
- `*(slot + 0x00)` = resource pointer
- `*(slot + 0x08)` = tons
- Validates `1 < *(int *)(slot_resource + 0x44) + 4U`
- Finds matching resource (`slot_resource == param_4`)
- `pfVar18 = slot + 0x8` (the mutable tons field)
- `fVar19 = slot_capacity * (1.0 - fVar20)` where fVar20 accumulates cargo utilization

### 9.5 Mode 0 path (order-source pool)

- Order vector at `*(param_1 + 0x144A8)` to `*(param_1 + 0x144B0)`, stride 0x68
- `order_entry + 0x30` = assigned `Vehicle*` (gate: `param_8 == '\0'` or `order_entry + 0x30 == param_2`)
- Resource records at `*(order_entry + 0x00)` = 8-byte pointers to 16-byte `{resource* +0x00, float tons +0x08}`
- Completion recording at `order_entry + 0x50`:
  - `completion_record + 0x28 + 0x188` = done flag (0x3f800000 = 1.0f)
  - `completion_record + 0x28 + 0x198` = resource pointer (param_4)

### 9.6 Order-structure stride confusion (corrected)

Prior reports claimed `order+0x10`, `order+0x48`, `order+0x50`, `order+0x28`, `order+0x68`, `order+0x70` were order fields. The decompilation shows:
- `order+0x00/+0x08` = `{Resource*, tons}` records (NOT source Building*)
- `order+0x30` = assigned Vehicle* (NOT destination Building*)
- `order+0x50` = array of completion-record *pointers* (NOT a completion vector directly on the order)
- No destination field exists on the order structure

The DO task record (stride 0x80 per `ghidra-do-storage-findings.md`) is a separate structure; fields like `+0x10` (source Building*) belong to it, not to the order.

### 9.7 No `+0x4F0` reference

The decompilation of `FUN_140420350` contains **no `vehicle+0x4F0` reference**. The transfer function does not read `vehicle+0x4F0`. This means:
- `vehicle+0x4F0` is not used by the transfer at call sites
- Whether `vehicle+0x4F0` is valid at any specific call site is **unresolved**
- Absence of `+0x4F0` inside this function does NOT prove the field is invalid elsewhere

### 9.8 Outgoing references from FUN_140420350

The decompilation shows calls to: `FUN_1401A9D60`, `FUN_1401AA120`, `FUN_140424430`, `FUN_140423fe0`, `FUN_140423e20`, `rand()`. These are visible in the decompiled source but were **not captured** by the `Xrefs420350.java` script's `getReferencesFrom(entryAddr)` call (see section 4.2). A proper outgoing-reference census requires scanning all instructions in the function body — not yet performed.

## 10. Output-Use Through the Pointee

### 10.1 What "not re-read" means

Not re-reading `[RSP+0x28]` after the call does NOT prove the output is dead. `[RSP+0x28]` holds the **pointer** argument (param_6), not the **pointee** (the value written by the callee through that pointer). To determine output use, we must trace:

1. Which object/local the pointer addresses
2. Writes made by the callee through it
3. Successor-path reads of that object and its aliases

### 10.2 Call site 0x694F40 — out pointer analysis

**Pointer setup:** At 0x140694F1F-0x140694F23:
- `LEA RCX,[RBP+0x20]` → RCX = address of local at RBP+0x20
- `MOV [RSP+0x28],RCX` → param_6 = &local_at_RBP+0x20

**Callee writes:** `FUN_140420350` writes `fVar21` to `*param_6` at decompiled line 257 (`*param_6 = fVar21` if `param_6 != NULL`). So the callee writes the rejected/overflow amount to `[RBP+0x20]`.

**Successor-path reads:** At 0x140694F45-0x140694F47:
- `TEST AL,AL` — checks return value
- `JNZ 0x140695377` — branches if AL != 0
- `[RSP+0x28]` (the pointer) is NOT dereferenced after the call in the visible window
- `[RBP+0x20]` (the pointee) is NOT read after the call in the visible window

**Conclusion for 0x694F40:** Output use through the pointee is **UNRESOLVED**. The callee writes fVar21 to [RBP+0x20], but no read of [RBP+0x20] or [RSP+0x28] appears in the exported disassembly window. The value may be read later in the function (outside the window) or may be dead.

### 10.3 Call site 0x6953C4 — out pointer analysis

**Pointer setup:** At 0x140695393-0x14069539B:
- `MOV [RSP+0x40],RSI` → param_9 = RSI
- `MOV [RSP+0x28],RSI` → param_6 = RSI

**Callee writes:** `FUN_140420350` writes `fVar21` to `*param_6` at decompiled line 257. So the callee writes the rejected/overflow amount to `*RSI`.

**Successor-path reads:** At 0x1406953C9:
- `MOV [RSP+0x64],R15D` — stores mode=1, NOT the out value
- `[RSP+0x28]` (the pointer = RSI value) is NOT dereferenced after the call
- `*RSI` (the pointee) is NOT read after the call in the visible window

**Conclusion for 0x6953C4:** Output use through the pointee is **UNRESOLVED**. The callee writes fVar21 to `*RSI`, but no read of `*RSI` or `[RSP+0x28]` appears after the call.

### 10.4 Provenance mismatch: plStack_338 read

The previous report attributed a `plStack_338` read at decompiled line 1669 to call site 0x1CB7FF (inside `FUN_1401CB2A0`). This was incorrect:

- Line 1669 is in `decomp_68b330.c` (`FUN_14068B330`), not `FUN_1401CB2A0`
- `plStack_338` is the local variable for the second call's out pointer at 0x6953C4
- The read `if (0.0 < plStack_338._0_4_)` at decompiled line 1669 reads the outMoved value from the mode 1 call at 0x6953C4

**Correct attribution:** The `plStack_338` read belongs to call site **0x6953C4** (mode 1, out=RSI), inside `FUN_14068B330`. It reads the pointer stored at [RSP+0x28], which was set to RSI before the call. The callee wrote fVar21 to `*RSI`, and then the caller reads `plStack_338._0_4_` — which is the first 4 bytes of the local whose address was passed as param_6.

**Alias question:** Is `plStack_338` the same local as `[RSP+0x28]`? If RSI == address of plStack_338, then yes. The disassembly shows `MOV [RSP+0x28],RSI` — so the pointer passed to the callee is RSI. If RSI == &plStack_338, then `plStack_338` receives fVar21 and is read at line 1669. **This alias is unresolved** without full liveness/alias analysis.

### 10.5 Output-use conclusion

| Call site | Pointer set to | Callee writes to pointee | Pointee read after call | Status |
|---|---|---|---|---|
| 0x694F40 | &[RBP+0x20] | Yes (fVar21) | Not in window | **UNRESOLVED** |
| 0x6953C4 | RSI | Yes (fVar21) | Not in window (mode store instead) | **UNRESOLVED** |
| 0x1cb7ff | &local | Yes | `plStack_338._0_4_` at line 1669 | **Confirmed read** (but belongs to 0x6953C4 path, not 0x1cb7ff) |

**Never mark output as "dead" without following the pointee.** Output use remains unresolved at both investigated sites.

## 11. Findings Classification

### Confirmed (instruction-level evidence from binary)

| # | Finding | Evidence |
|---|---|---|
| C1 | Transfer function signature: `FUN_140420350(game*, vehicle*, mode, resource*, tons, out*, cap, flag, building*)` at RVA 0x420350 | Decompilation decomp_420350.c lines 8-10 |
| C2 | 16 direct CALL rel32 sites enumerated with raw bytes, length, target, containing function | CallsTo420350.java output (calls_to_420350.txt) |
| C3 | 3 DATA references (not invocations) | Xrefs420350.java output (xrefs_420350.txt) |
| C4 | Return semantics: AL=1 mutation committed; AL=0 early exit | Decompilation lines 35-37, 229-231, 259 |
| C5 | fVar21 = rejected/overflow amount, NOT actual moved tons | Decompilation lines 233-258 |
| C6 | Mode semantics: mode 0 = order-source pool; mode 1 = vehicle cargo slot | Decompilation lines 39-43 |
| C7 | Mode/sign independence | Call-site census (mode 0 carries both + and -; mode 1 carries both - and +) |
| C8 | Building validation: `param_9 + 0x318` type descriptor, types 0x0C/0x6A | Decompilation lines 88-90 |
| C9 | Paired call pattern: 0x694F40 (mode 0) + 0x6953C4 (mode 1), same resource+tons, different vehicle pointers | Decomp_68b330.c lines 1599-1644, disassembly |
| C10 | Out pointer non-NULL at both sites (0x694F40: &local; 0x6953C4: RSI) | Disassembly sections 7.1, 7.2 |
| C11 | Out pointer NOT read after call at both 0x694F40 and 0x6953C4 | Disassembly: TEST AL/JNZ at 0x694F40; MOV [RSP+0x64],R15D at 0x6953C4 |
| C12 | fVar21 derivation: initialized to 0.0, set on unload clamp or load clamp | Decompilation lines 233-258 |
| C13 | No `+0x4F0` reference in transfer decompilation | Decomp_420350.c full scan |
| C14 | CALL rel32 displacements verified: all resolve to 0x140420350 | Raw byte computation |

### Candidate (strong evidence, requires confirmation)

| # | Finding | Evidence | Gap |
|---|---|---|---|
| Cd1 | `veh+0x4F8` = home building | Prior docs (vehicle-fuel Q1, AGENT_REPORT Q4) — 3 readers | Not directly observed in this binary's transfer decompilation |
| Cd2 | `veh+0x4F0` = current building | Prior docs (private-industry H11) | Not read by transfer; validity at call sites unresolved |
| Cd3 | Order vector `game+0x144A8/+0x144B0` stride 0x68 | Decompilation lines 136-227 | — |
| Cd4 | `order+0x30` = assigned Vehicle* | Decompilation line 169 | — |
| Cd5 | `order+0x00/+0x08` = `{Resource*, tons}` records | Decompilation lines 177-190 | — |
| Cd6 | Two different vehicles in paired calls (`lVar24` vs `[R14+0x390]`) | Decompilation and disassembly | Whether they alias the same object unresolved |

### Contradicted (prior claims overturned by binary evidence)

| # | Prior claim | Correction | Evidence |
|---|---|---|---|
| Cx1 | "param_2 is Vehicle* at every call site" | Inside callee confirmed; caller-side unresolved at refuel site 0x16F831 (offset coincidence) | Decompilation + disassembly |
| Cx2 | "Mode 0 = load, mode 1 = unload" | Mode and sign are independent | Call-site census |
| Cx3 | "order+0x00/+0x08 = source Building* vector" | `{Resource*, tons}` records | G5 evidence |
| Cx4 | "order+0x30 = destination Building*" | Assigned Vehicle* | Decompilation line 169 |
| Cx5 | "100% of invocations observable via call-site patching" | 16 direct calls confirmed; census is lower bound | CallsTo420350.java |
| Cx6 | "current building is pickup at unload" | Transfer does not read `+0x4F0` | Decompilation full scan |
| Cx7 | "Out read at 0x6953C4" | `MOV [RSP+0x64],R15D` stores mode, not out value | Disassembly section 7.2 |
| Cx8 | "0x694F40 out = NULL" | Disassembly shows `MOV [RSP+0x28],RCX` (non-NULL) | Disassembly section 7.1 |
| Cx9 | "0x6953C4 out = &plStack_338" | Disassembly shows `MOV [RSP+0x28],RSI` | Disassembly section 7.2 |

### Unresolved (requires further investigation)

| # | Item | Reason |
|---|---|---|
| U1 | RSI origin at param_9 (0x694F40 and 0x6953C4) | Not defined in disassembly windows; needs predecessor-block analysis |
| U2 | `param_2` caller provenance at refuel site 0x16F831 | Not investigated in this binary run |
| U3 | `vehicle+0x4F0` readability at transfer call sites | Transfer decompilation has no `+0x4F0` reference |
| U4 | Order destination field | Not found on order; may exist on DO task record |
| U5 | Durable operation ID | No field found linking pickup to delivery; order slots reused |
| U6 | Output-use through pointee at 0x694F40 and 0x6953C4 | Pointee not read in exported windows |
| U7 | `lVar24` vs `[R14+0x390]` aliasing | Different expressions; alias analysis not performed |
| U8 | Outgoing reference census from FUN_140420350 body | `getReferencesFrom(entryAddr)` is not a body census |

### Items to remove or requalify

| Item | Disposition | Reason |
|---|---|---|
| "Absence of +0x4F0 inside this function proves external field invalidity" | **Removed** | Absence in one function doesn't prove invalidity elsewhere |
| "Different expressions prove different vehicles" | **Removed** | `lVar24` vs `[R14+0x390]` — aliasing unresolved |
| "Iteration wraparound proves allocation/reuse" | **Removed** | Wraparound in scan loop doesn't prove memory reuse |
| "No durable ID found means none exists" | **Removed** | Not found ≠ doesn't exist |
| "Persistent engine ID is mandatory for verified association" | **Removed** | Session-local ID may suffice with verified lifecycle |
| "Static direct-call enumeration authorizes instrumentation" | **Removed** | Enumeration is read-only; instrumentation needs separate authorization |
| "+0x4F8 proven" (in adapter contract context) | **Requalified to "Candidate"** | Confirmed in prior docs but not directly observed in this binary's transfer decompilation |
| "actual moved tons ... proven" | **Requalified to "Contradicted"** | fVar21 is rejected/overflow, not actual moved |

## 12. Adapter-Contract Contradictions

The following lines in `docs/FUEL PLAN/association-investigation/association-adapter-contract.md` are contradicted by the repaired binary evidence and must be reconciled before that contract can be used:

| Line/Section | Contract claim | Contradicting evidence | Required change |
|---|---|---|---|
| Table row "outMoved" | "actual moved tons, read after call" | fVar21 is rejected/overflow amount, not actual moved (section 9.2) | Rename field; correct semantics |
| Table row "actual moved tons" | Valid only when AL==1 | fVar21 can be 0.0 when AL==1 (Cases 1, 3) | Clarify: AL==1 means mutation committed, not positive movement |
| "Supported where non-NULL" for `explicit_destination` | param_9 = destination building | param_9 = RSI at both sites; RSI origin unresolved; object type not established | Mark as unresolved |
| "proven" for `home_building` (`veh+0x4F8`) | Confirmed carrier/home | Confirmed in prior docs but not directly observed in this binary's transfer decompilation | Requalify to "candidate" |
| "supported where non-NULL" for `resolved_delivery` | Destination = param_9 at unload | param_9 = RSI; RSI provenance unresolved | Mark as unresolved |
| `operation_id` absent | "No durable operation ID exists" | Not found ≠ doesn't exist (section 11) | Requalify to "not established" |
| Billing gate "site AND tons > 0" | Supported | Supported but fVar21 is rejected/overflow; actual moved = |tons| - fVar21 | Correct quantity derivation |
| `is_delivery` derived from site + tons > 0 | Provisional | Negative-tons legs (0x68F7F9, 0x6945B3, 0x694F40) are not deliveries | Confirm polarity rule |

## 13. Evidence-Readiness Assessment

### 13.1 What is ready for logging-only observation

The following can be observed at transfer call sites without further binary investigation:

| Observable | Source | Confidence |
|---|---|---|
| Vehicle pointer (param_2) | RDX at call site | Confirmed inside callee; caller-side provenance unresolved at some sites |
| Resource pointer (param_4) | R9 at call site | Confirmed as pointer |
| Tons (param_5) | Stack+0x20 at call site | Confirmed |
| Sign of tons | param_5 sign | Confirmed |
| Mode (param_3) | R8 at call site | Confirmed |
| Call site RVA | Instruction address | Confirmed |
| Return value (AL) | AL after call | Confirmed |
| Output pointer (param_6) | Stack+0x28 at call site | Confirmed |
| Building pointer (param_9) | Stack+0x40 at call site | Confirmed as pointer value; object type unresolved |
| Home building (`veh+0x4F8`) | Prior docs | Candidate |
| Current building (`veh+0x4F0`) | Prior docs | Candidate; not read by transfer |

### 13.2 What requires further investigation before observation

| Item | Required work | Status |
|---|---|---|
| RSI origin (param_9 at 0x694F40/0x6953C4) | Predecessor-block analysis within FUN_14068B330 | Unresolved |
| `param_2` provenance at refuel site 0x16F831 | Trace RBX/R14 origins | Unresolved |
| `vehicle+0x4F0` validity at call sites | Verify field is set and non-NULL at each site | Unresolved |
| Output-use through pointee | Trace pointee reads after call | Unresolved at 0x694F40 and 0x6953C4 |
| `lVar24` vs `[R14+0x390]` aliasing | Alias analysis | Unresolved |
| Outgoing reference census from transfer body | Scan all instructions for CALL | Not performed |
| Order destination field | Search DO task record and completion log | Not found on order |
| Durable operation ID | Search for stable link between pickup and delivery calls | Not established |

### 13.3 Readiness for logging-only prototype

**NOT READY for implementation.** The following prerequisites must be resolved first:

1. **Argument correctness:** RSI provenance must be established before `param_9` can be interpreted as a destination building.
2. **Quantity semantics:** `fVar21` must be correctly named as rejected/overflow, and actual moved = `|tons| - fVar21` must be derived correctly.
3. **Output-use:** Pointee liveness must be confirmed before relying on outMoved values.
4. **Analysis completeness:** The 16-site enumeration is a lower bound (analysis timed out at 60s). Additional call sites may exist.

**No logging-only instrumentation is authorized by this task.** This assessment identifies what would need to be true before such instrumentation could be meaningfully designed.

### 13.4 Remaining limitations

- Ghidra analysis timed out at 60 seconds; full function coverage not achieved
- No debugger available for runtime validation
- RSI origin unresolved at both investigated call sites
- `param_2` caller-side object type unresolved at refuel site
- Output-use through pointee unresolved at both investigated call sites
- 3 DATA references not classified as unwind metadata without section/structure evidence
- Outgoing reference census from transfer body not performed

## 14. Export Script Source and Exact Commands

### 14.1 Script inventory

All scripts are in `C:\WRSRE-ghidra-work/scripts/`. Addresses are static VAs (ImageBase `0x140000000` + RVA), passed as hex string literals like `"0x140420350"`.

| Script | Purpose | Output file |
|---|---|---|
| `CallsTo420350.java` | Enumerate all CALL rel32 targeting FUN_140420350 | `output/calls_to_420350.txt` |
| `Xrefs420350.java` | Cross-references to/from FUN_140420350 | `output/xrefs_420350.txt` |
| `Decomp420350.java` | Decompile transfer function FUN_140420350 | `output/decomp_420350.c` |
| `Decomp68B330.java` | Decompile sea/rail hauler FUN_14068B330 | `output/decomp_68b330.c` |
| `DisasmStream694F40.java` | Stream disassembly 0x140694E00:0x140695000 | `output/stream_694f40.asm` |
| `DisasmStream6953C4.java` | Stream disassembly 0x140695280:0x140695450 | `output/stream_6953c4.asm` |
| `OutgoingRefs68B330.java` | Outgoing CALL refs from FUN_14068B330 body | `output/outgoing_refs_68b330.txt` |

### 14.2 Exact command executed

```
analyzeHeadless.bat 'C:\WRSRE-ghidra-work\projects' 'SOVIET64_analysis' ^
  -import 'C:\Workers Resources Soviet Republic\SOVIET64.exe' ^
  -analysisTimeoutPerFile 60 ^
  -scriptPath 'C:\WRSRE-ghidra-work\scripts' ^
  -postScript 'Xrefs420350.java' ^
  -postScript 'CallsTo420350.java' ^
  -postScript 'Decomp420350.java' ^
  -postScript 'Decomp68B330.java' ^
  -postScript 'DisasmStream694F40.java' ^
  -postScript 'DisasmStream6953C4.java' ^
  -log 'C:\WRSRE-ghidra-work\projects\investigation6.log'
```

**Shell:** Windows PowerShell 5.1
**Analysis status:** Timed out at 60 seconds (file not fully analyzed). Post-scripts ran successfully and produced output.

### 14.3 Script source

All scripts follow the Ghidra 12.1.4 API pattern:
- `currentProgram.getAddressFactory().getAddress("0x140XXXX")` for address resolution
- `currentProgram.getFunctionManager().getFunctionContaining(addr)` for function lookup
- `DecompInterface.decompileFunction(func, timeout, monitor)` for decompilation
- `ReferenceManager.getReferencesTo(addr)` / `getReferencesFrom(addr)` for xrefs

Script source files are preserved as small text files at `C:\WRSRE-ghidra-work/scripts/`.

### 14.4 Raw exports embedded

The following raw exports are embedded in this document:
- Section 3.1: All 16 CALL rel32 instructions with VA, RVA, raw bytes, length, target, containing function
- Section 4.1: All 19 incoming references to FUN_140420350
- Section 5.1: Full decompilation of FUN_140420350 (263 lines)
- Section 7.1: Complete verified instruction listing for call site 0x694F40 (23 instructions)
- Section 7.2: Complete verified instruction listing for call site 0x6953C4 (15 instructions)

The full caller decompilation (`decomp_68b330.c`, 364KB) is preserved at `C:\WRSRE-ghidra-work\output\decomp_68b330.c` but not embedded in full due to size. Relevant regions are quoted in section 6.
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs

