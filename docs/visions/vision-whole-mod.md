# Vision: WRSRE Economic Mod

**Updated:** 2026-10-22 (product vision); **2026-11-22 owner decisions override the economics below. Read [`economy-direction-2026-11-22.md`](economy-direction-2026-11-22.md) first.**  
**Reference snapshot:** supplied WRSRE code listing with 15 plugin names, prior work-plan/design notes, and the current staged private-logistics investigation. This is an updated product/architecture vision—not a claim that every named subsystem is implemented, integrated, or working in the live game.

> **2026-11-22 corrections, in brief:**
> - The state/private balance is the player's choice.
> - Native game prices only: no market multiplier, no invented fallback prices.
> - One domestic market: local markets are cancelled.
> - The state does not buy private output on dispatch. Domestic sales settle on delivery at the native price, and companies receive their own customs export revenue.
> - The 90 %/70 % producer/procurement shares below are **withdrawn**.
> - B2B billing is to be live by default.
> - No sample or synthetic data anywhere. Missing data must be obvious.
> - Decrees and housing choice by wealth are agreed.
> - Politics comes later, in its own module.
>
> Section 7 below is a 2026-10-22 snapshot and is out of date (`private_logistics` is now `0.19.0-simplified-settlement`).

## 1. Product vision

Build a modular economic layer for Workers & Resources that makes private production, retail, labor, transport, finance, taxation, and public-sector interaction legible and playable—while leaving the game's physical simulation in charge of people, vehicles, goods, roads, buildings, and inventory.

The mod should create meaningful economic choices rather than replace the core simulation. Private firms earn revenue and pay expenses; citizens receive income and spend through shops; freight has a visible cost; prices and taxes are explicit; the public sector can participate as a wholesaler/owner/market actor without becoming an invisible source of free goods or money.

The player should be able to understand why money moved, which actor owns a building, what price was used, who paid for delivery, and how a policy affected the result. Economic flows should be auditable and reversible where possible.

## 2. Design principles

### Physical simulation stays native

- Never fabricate a transfer by writing inventory quantities or teleporting goods.
- Use native production, storage, vehicle, and logistics behavior as the physical source of truth.
- A plugin may account for a confirmed game event or configure a native task; it must not pretend the physical event occurred.
- Native vehicle/DO reachability and capacity rules remain authoritative.

### Money is explicit and attributable

- Every debit has a payer, every credit has a recipient, and every flow has an event or rule that explains it.
- Use a shared currency/transaction service rather than ad hoc plugin-local wallets.
- Preserve enough ledger detail to trace balances and resolve double-counting.
- Fail closed on ambiguous ownership, missing prices, invalid participants, or duplicate events rather than inventing a charge.
- **Never display or use sample, synthetic, placeholder or invented values.** When information is unavailable or an error occurs, make it obvious to the player (`UNAVAILABLE` / `ERROR <reason>`).

### Prices and costs are grounded

- Use a shared price reference, not multiple conflicting price tables. **2026-11-22: that reference is the native game price, with no mod multiplier.** A missing native price means *unpriced and refused*, never an invented fallback.
- Fuel expense is based on actual physical consumption and the live price.
- Freight should be a transparent service/cost model, not an arbitrary hard-coded rouble amount.
- Wage, tax, producer-share, procurement, and tariff values are explicit tunable policies; they should be documented as balancing decisions, not engine facts.

### Preserve player control

- Avoid silently rewriting player-managed targets, building permissions, price policies, or business ownership.
- Opt-ins default off for behavior that changes logistics access. *(2026-11-22 exception: B2B delivery billing defaults **on**, per the owner.)*
- The player alone decides how much of the economy is state-run or private; nothing auto-privatises.
- Automation should add missing information, not erase manual choices.
- State-run distribution offices remain player-managed unless the player later approves a separate automation feature.

### Modular and testable

- Keep each plugin's responsibilities narrow and use shared services through the project/host API.
- Avoid implicit cross-DLL coupling and duplicated authorities.
- Test a single actor and a single flow before scaling to a whole economy.
- Clearly distinguish static analysis, staged code, compiled binaries, prior runtime logs, and current runtime validation.

## 3. Intended economy and actor flows

This is the intended high-level loop. Individual links may be at different implementation stages.

### Production and wholesale supply

Private producers create goods through the game's normal production/inventory simulation. **2026-11-22:** goods leaving a factory are *not* a sale. A sale is recognized when the goods are **delivered**:
- **To a domestic buyer:** the buyer pays the seller the **native price** on delivery. If the buyer is a state building, the treasury pays only for goods it actually receives (owner to confirm, economy direction Q1).
- **At a customs house:** the company receives the exact native customs proceeds.

The former "producer paid **90%** of retail on dispatch" rule is withdrawn. The state sector can act as a buyer or seller like any other counterparty, but must not generate free stock or untraceable money. State exports and imports stay native.

### Private procurement and retail

Private shops obtain goods through native logistics and pay the **native price** on delivery. The former "procure at **70%** of retail" rule is withdrawn: it sold state goods below their native price. Shops sell at `landed cost × (1 + category margin) × (1 + VAT)`. The margin defaults per category and adapts within bounds to refusals and sell-through. State shops sell at a player-set *state retail price level* (economy direction §5). Citizens spend from wallets; a citizen who cannot pay does not receive the goods, and the native unmet-need mechanics apply.

All ratios (margins, price levels, wage and transfer ratios) are explicit balancing inputs, visible in configuration or as player decrees. They are tested against the invariants in economy direction §9, never embedded as unexplained constants.

### Freight and fuel

The receiving party pays for freight service. The intended carrier economics account for the actual trip and physical fuel expense; the fuel cost uses litres consumed, fuel density/unit conversion, and live fuel price. The current staged logistics configuration uses a density of `0.00100` tonnes per litre and enables trip tariff logic, but this has not been full-build or in-game validated.

No fixed rouble freight fee, artificial fuel price, wage model inside the logistics plugin, or invisible subsidy should be added without an explicit design decision. Whether carriers eventually own/lease vehicles and incur maintenance or labor costs is open.

### Citizens, wages, and taxes

Citizens are intended to have wallets/income and spend at shops; the labor subsystem is the intended place for employment/payroll policy. Taxes are a separate, auditable settlement applied on documented schedules. No tax or wage behavior should be inferred from a plugin name alone; exact formulas, timing, and runtime integration require code review and tests.

### State/private boundary

Public/state storage remains public inventory and is governed by native logistics. The current state-storage feature allows a player to opt a particular public storage into use as a **load-only source** for eligible private DOs. It does not create a bidirectional source/sink, does not automate state DOs, and does not move goods. The player remains in control of the opt-in.

## 4. Plugin architecture

The supplied codebase listing names 15 plugins. The role descriptions below are the intended architecture as understood from names and prior design notes; they are not evidence that each plugin is complete or currently functional.

| Plugin | Intended responsibility | Boundary / verification note |
|---|---|---|
| `citizen_labor` | Employment, labor allocation, and wage/payroll rules for citizen work. | Coordinate wallet settlement with `currency`; exact job and wage semantics require source/runtime verification. |
| `citizen_retail` | Citizen demand, shopping/retail purchase interactions, and citizen-side transactions. | Should use shared wallets/prices; do not duplicate shop procurement accounting. |
| `currency` | Shared money, wallets/treasury, debits/credits, and transaction primitives. | Should be the single monetary authority; fail safely and provide auditable transaction results. |
| `enterprise_base` | Private-enterprise identity, ownership/registry, business state, and shared firm-level services. | Other private-sector plugins should query shared business identity instead of inventing ownership rules. |
| `map_expander` | Map/placement expansion utilities. | Separate from the core economic loop; exact current responsibilities and integration need code verification. |
| `persistence` | Save/load lifecycle and mod-owned persistent data. | Persist stable identifiers/keys rather than transient pointers; test with real save/load cycles. |
| `private_construction` | Private-sector construction activity/ownership/economic treatment. | Must coordinate capital, ownership, and construction costs with shared enterprise/currency services. |
| `private_logistics` | Transfer accounting, producer settlement, freight/fuel economics, and optional native DO target assistance. | Must never manipulate inventory; state-storage targets are opt-in and source-only. See the dedicated vision. |
| `private_shops` | Private-shop procurement/restocking and shop-side commercial rules. | Intended procurement starting point is 70% of retail reference price; physical supply remains native. |
| `resource_pricing` | Shared resource price references/indexes and pricing inputs. | Must be a single consistent source for producers, shops, logistics, and ledger explanations. |
| `sim_clock` | Shared simulation time/day/month events and economic period boundaries. | Consumers should use one clock/tick authority to avoid inconsistent settlements. |
| `state_ledger` | Auditable records and summaries of public/private economic flows. | Should record actual settled transactions, avoid duplicate authority for balances, and support reconciliation. |
| `taxes` | Tax policy, accrual, assessment, and settlement. | Keep rules explicit and period-based; coordinate with `currency`, `sim_clock`, and `state_ledger`. |
| `ui_overlay` | In-world or screen overlay presentation of mod state. | Presentation only; must not become an alternate data authority. |
| `ui_panels` | Native building/panel extensions and player controls, including opt-in settings. | Must coexist with native controls and persist settings safely; UI panel bounds need validation. |

### Dependency direction

A healthy conceptual dependency direction is:

- `sim_clock` and `persistence` provide shared lifecycle/time services.
- `currency`, `enterprise_base`, and `resource_pricing` provide shared economic authorities.
- `state_ledger` records settled flows from dependent systems.
- `citizen_labor`, `citizen_retail`, `private_shops`, `private_construction`, `private_logistics`, and `taxes` implement domain behavior using those shared services.
- `ui_panels` and `ui_overlay` display/control the domain features without owning their economic truth.
- `map_expander` remains a separate map feature unless a documented integration is necessary.

This is a design direction; check actual host/plugin loading and service discovery in code before making claims about the deployed dependency graph. Avoid direct DLL-to-DLL imports unless the established project contract explicitly allows them.

## 5. Whole-mod scope and non-goals

### In scope

- A coherent private-sector economy layered onto the game's physical simulation.
- Shared business identity, prices, wallets, time, persistence, and transaction records.
- Transparent producer, retail, logistics, labor, construction, and tax flows as their subsystems mature.
- Player-facing controls and reports that explain eligibility, prices, actors, and consequences.
- Public/private interactions that use native storage and logistics and respect player decisions.

### Not a goal

- Replacing Workers & Resources with a second physical simulation.
- Teleporting goods, manufacturing stock, or treating a planned DO task as a completed transfer.
- Silent edits to manual distribution-office instructions.
- Unbounded automation based only on an asset name or building type.
- Arbitrary hidden prices, wages, tariffs, or subsidies.
- Declaring every listed plugin finished or runtime-supported based solely on its presence in a code snapshot.

## 6. Reliability, persistence, and evidence discipline

- Use stable keys for mod-owned per-building or per-business state; do not persist raw process pointers.
- Make save/load callbacks idempotent and resilient to partially initialized buildings.
- Separate simulation-time actions from frame/UI callbacks.
- Make hooks safe against duplicate callbacks, invalid pointers, missing ownership, and unknown resource prices.
- Bound logs and include enough identifiers to reconcile changes without flooding each frame.
- Test save/load, player edits, and plugin reload/restart behavior on disposable saves.
- For every feature, label status as design, staged, compiled, historical runtime-tested, or current runtime-verified.
- Never represent Ghidra static analysis as proof of live asset configuration or gameplay behavior.

## 7. Current status and immediate priorities

The supplied code snapshot lists 15 plugin folders, but the snapshot does not establish that all 15 compile, load, interoperate, or implement the complete vision. Earlier design/work-plan materials describe intended sequencing and prior status; treat those as historical context until reconciled against the Windows checkout.

The most active current task is state-storage source opt-in for `private_logistics`/`ui_panels`. The staged candidate is private-logistics v0.17 and UI v6.11. Automatic-target dry-run remains on (`auto_targets_dry_run = 1`); billing dry-run is separately off (`dry_run = 0`) in the staged INI. There has been no full Windows/MSVC build, current-game load, UI validation, transfer test, or save/load test. The Windows tree recovery/deployment state is unknown.

Priorities:

1. Reconcile staged sources with the actual Windows project and record the exact versions/hashes before editing or deploying.
2. Establish a reproducible Windows build and a small integration smoke test for host/plugin loading.
3. Validate shared wallet, ownership, price, clock, persistence, and ledger contracts before expanding economic behaviors.
4. Reconcile one end-to-end private transaction: producer transfer → payment → shop procurement/sale → freight/fuel → ledger.
5. Complete static investigation of DO panel checkbox allocation above five and inspect the actual storage asset mode.
6. Validate one opted-in source and one DO in a disposable save; preserve native/manual targets and prove no inventory manipulation.
7. Expand to labor, taxes, construction, and additional participants only after economic flows are observable and reconciled.

## 8. Roadmap and acceptance vision

### Foundation

- [ ] One documented plugin ABI/service contract, load order, and ownership model.
- [ ] Reliable shared simulation clock and persistence lifecycle.
- [ ] Shared enterprise identity, currency transaction API, resource price service, and ledger record schema.
- [ ] A clean build of each plugin from the reconciled Windows tree.

### Closed-loop market

- [ ] Private producer sale is recognized only from a real game transfer.
- [ ] Producer payment uses the configured retail share and is traceable.
- [ ] Shop procurement uses native logistics and the documented procurement share.
- [ ] Citizen purchases debit the correct wallet and credit the right business.
- [ ] Every transaction has one stable event identity to prevent duplicate settlement.

### Freight and resource cost

- [ ] Receiver/carrier attribution is valid and logged.
- [ ] Carrier revenue and fuel expense reconcile to physical trips and live prices.
- [ ] Unit conversions are verified in game and documented.
- [ ] Ambiguous events fail closed and do not silently mint or destroy money.

### Public/private logistics integration

- [ ] State-storage permission defaults off, is player-controlled, and persists.
- [ ] A generated state-storage target is load-only in the native DO panel.
- [ ] Reachability is verified for road DO and rail DO via road connection.
- [ ] Existing manual tasks survive repeated automation and save/reload.
- [ ] No inventory manipulation occurs; native logistics alone performs transfers.
- [ ] The five-entry staged policy is retained until native checkbox bounds are proven.

### Player-facing economy

- [ ] UI shows business ownership, relevant balances/prices, and why transactions occurred.
- [ ] Overlays are display-only and consistent with ledger records.
- [ ] Taxes, wages, procurement, and tariffs are documented, configurable, and test-balanced.
- [ ] The player can inspect and adjust approved policy rather than discover hidden constants.

## 9. Open design decisions

**2026-11-22 status:**
- **Decided:** whether prices respond to scarcity (not now; native prices only, and any future model adjusts native prices); which features are on by default (B2B billing live).
- **Still open:** the remaining items below, plus the questions in economy direction §10.

These are intentional decisions for the project owner, not questions that static reverse engineering can resolve:

- Long-term freight tariff: strict fuel/cost recovery or cost plus a configurable operating margin?
- Whether to model private carrier ownership, leases, maintenance, depreciation, and wages.
- How to handle unpaid receiver invoices: block transfer, defer debt, or use another explicit credit policy?
- Whether multiple DOs should compete natively for public sources or use a future reservation/allocation system.
- Which production/retail prices become player-configurable and how prices respond to scarcity.
- Tax bases, timing, rates, exemptions, and interaction with public treasury accounting.
- Which features are enabled by default in a release versus opt-in.

Until those are decided, prefer explicit settings, auditable logs, native physical behavior, and conservative defaults.
