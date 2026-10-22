# Vision: `private_logistics`

**Updated:** 2026-10-22  
**Role:** private freight-market accounting and carefully bounded optional DO-target assistance.

## Purpose

`private_logistics` makes private freight economically meaningful while leaving physical logistics to Workers & Resources. It accounts for private transport based on real game movements, charges the receiving party for freight, pays private producers when their goods enter the modeled market, and prices fuel from physical consumption. Optional target assistance helps private offices find eligible sources/sinks through the native logistics system; it does not perform transfers itself.

The plugin is an accounting and logistics-integration layer, not a replacement simulation for trucks, roads, warehouses, inventory, or the native DO planner.

## Core principles

1. **Vanilla logistics owns physical movement.** Do not teleport stock, write inventory quantities, or bypass native loading, unloading, route, or capacity rules.
2. **Source of truth is the observed game event.** Billing responds to committed transfer/movement callbacks, not merely to planned work.
3. **No arbitrary fixed carrier price.** Freight cost should reflect a transparent trip/tariff model, not a hand-entered rouble value per ton or kilometer.
4. **Fuel is physical.** Convert consumed fuel to litres using the configured density and charge at the live fuel price. Do not substitute a fictional flat fuel price.
5. **Receiver pays the carrier.** The consignee is charged for the trip/service; the carrier receives the corresponding revenue through the plugin's accounting path.
6. **Private producer settlement is separate.** When private-factory goods enter the modeled market, the producer receives the configured factory share (staged value 90% of retail price).
7. **Native targets are preserved.** Automation adds eligible targets conservatively, merges with manual configuration, and never treats a reachability-disabled task as removed.
8. **State storage is source-only and opt-in.** No bidirectional state target, no automatic opt-in, and no inventory manipulation.
9. **Observable and reversible.** Logs and configuration should explain why a charge, payout, candidate, or target decision occurred. Dry-run must be available for high-risk target changes.
10. **No overclaiming.** Static findings, staged source, successful builds, and live game behavior are different evidence classes.

## Economic model

### Freight and fuel

A private carrier's service cost is associated with the physical trip and its fuel burden. Fuel expense is based on consumed litres multiplied by the live fuel price, with litres derived from game fuel units and the configured density. Current staged density is `0.00100` tonnes per litre; exact game-unit conversion and accounting must be confirmed against the live callbacks and runtime data before changing it.

The receiver pays the carrier for the freight service. The intended model is cost-recovery/transparent service billing, not a speculative wage system or arbitrary subsidy. A carrier's own revenue, expense and margin should be traceable in the ledger.

### Producer settlement

Private producers are paid when their goods are transferred into the market through observed game behavior. The staged factory share is 90% of the retail reference price. This is a design starting point, not proof of a balanced economy; test with actual resource prices and cash-flow logs.

### Pricing and parties

`resource_pricing` owns the shared price reference/market index. `currency` and `enterprise_base` provide wallet/ownership/accounting services. `private_logistics` should consume those services rather than create a competing price table or hidden wallet.

### Deliberately excluded assumptions

- No fixed freight charge per ton or kilometer unless introduced as a separately approved, data-backed tariff rule.
- No fictional fuel price when live prices are available.
- No worker wage model in this plugin; labor/payroll belongs in labor/economy systems.
- No claim that carriers own/lease vehicles in a fully modeled market until that ownership and cost model is implemented and verified.
- No customs-specific assignment or inventory-handling behavior without a separate, explicitly approved design.

## Plugin responsibilities

### In scope

- Observe the game's committed transfer/movement hooks for private-sector logistics.
- Attribute an eligible transfer to the correct private producer, receiver, and/or carrier when attribution is valid.
- Settle producer payouts and carrier freight charges using shared currency, resource-price, enterprise, and ledger interfaces.
- Account for physical fuel expense using the real fuel price and explicit unit conversion.
- Maintain auditable logs and clear configuration for billing, tariff, fuel conversion, and optional automation.
- Optionally identify candidate buildings and assist native DO target selection, while retaining existing player configuration.
- Persist only the feature's own required metadata/permissions and restore safely after load.

### Out of scope

- Moving goods or modifying any inventory/stock quantities.
- Replacing the native vehicle/road/rail planner or dispatch system.
- Turning state storage into an unload/destination/bidirectional target.
- Automatically changing player-managed state distribution offices.
- Deleting manual targets to make room for automation.
- Claiming exclusive source reservations across multiple DOs without an implemented/verified coordination mechanism.
- Hiding unexpected billing or silently creating money.

## Optional automatic target assistance

This is a separate capability from transfer billing and should have independent controls and safe defaults.

- The game's native logistics must remain responsible for task execution.
- Existing native/manual targets are retained and merged.
- State storage is considered only when the player has explicitly enabled that individual storage building.
- State-storage candidates are **load-only**. The plugin must never create a state-storage bidirectional task.
- The staged selector prioritizes opted-in state-storage candidates before ordinary private candidates, subject to reachability and target limits.
- Current staged policy caps a private DO at five total entries, including existing/manual entries. This is a conservative response to unverified native panel checkbox allocation above index four; it is not a proven game cap.
- Reconciliation must be idempotent: repeated passes should not duplicate a target or rewrite unrelated manual targets.
- If the player explicitly opts a state-storage building in, the plugin-managed task for that storage must be load-only; the feature must never generate a bidirectional state-storage task. An explicit opt-out may disable the plugin-managed load behavior without deleting the target entry.
- Manual targets unrelated to an opted-in state-storage source retain their native configuration. Default-off state-storage permissions do not turn a manually configured task into an automation-owned task.
- Reachability disablement is not removal. If the native game retains a task while unreachable, the plugin must preserve it.
- No global reservation is known. Start with one DO and make no exclusivity guarantee.
- `auto_targets_dry_run = 1` is the current staged setting and blocks target/vector and TaskCfg writes.

## Current implementation/status snapshot

Staged candidates are `private_logistics` v0.17 and `ui_panels` v6.11. Staged configuration has plugin/billing enabled, billing `dry_run = 0`, factory share `0.90`, trip tariff on, fuel density `0.00100` t/L, automatic target assistance enabled, and `auto_targets_dry_run = 1`.

These are the contents of the staged files only. This session has not built the full plugin with MSVC, confirmed the staged files are installed in the Windows game tree, loaded the plugin in-game, tested billing, verified UI behavior, or completed a save/reload cycle. The external Windows-tree recovery state remains unknown. Do not describe the candidate as released or verified.

## Interfaces and plugin boundaries

- Use the host/plugin API and the project's shared headers; do not invent direct cross-DLL linking or duplicated state ownership.
- `currency` is the wallet/transaction authority; `enterprise_base` is the business/ownership registry; `resource_pricing` supplies shared prices; `state_ledger` records auditable flows; `sim_clock` provides consistent period timing.
- `ui_panels` owns native-panel integration and persistent UI state; `private_logistics` owns the interpretation of storage permission and its effect on candidate eligibility.
- Native game functions are not safe merely because a function can be located in Ghidra. Use only understood signatures/lifecycles, preserve native initialization, and validate against the actual executable.

## Observability and failure behavior

- Log eligibility, exclusion reason, permission value, reachability result, existing-target match, cap decision, and whether an operation was suppressed by dry-run.
- Log billing inputs, party attribution, live price, physical fuel, and final debit/credit, with enough context to reconcile against `state_ledger`.
- On missing owner, invalid pointer, unresolved price, or ambiguous transfer attribution, fail closed: do not create a speculative charge or payout; emit a bounded diagnostic.
- Keep logs rate-limited and avoid per-frame noise.
- Treat duplicate callbacks, save/load re-entry, and player edits as normal conditions that need idempotent handling.

## Roadmap

1. **Source reconciliation:** establish which staged files match the Windows checkout; retain backups and record hashes/versions.
2. **Build and hook validation:** compile with the real Windows toolchain; confirm hook signatures, callback order, and no regressions.
3. **Billing verification:** test one private producer, receiver, and carrier in a disposable save; reconcile every cash flow against prices, fuel, and ledger logs.
4. **Static panel-capacity investigation:** establish the allocation and bounds for `panel + 0x310 + idx * 4` before increasing the five-entry policy.
5. **State-storage asset validation:** inspect actual asset mode and live connection graph. Confirm load-only behavior in the native panel.
6. **Single-DO automation test:** keep target dry-run on until build and safety checks pass; then test one road DO and one opted-in source in a disposable save.
7. **Save/load and manual edits:** verify persistence, manual target preservation, idempotence, and unreachable/reachable transitions.
8. **Rail and multi-DO tests:** test rail DOs using road connections first; test type-6-only routes and source contention separately.
9. **Controlled release:** document known limits and only change dry-run settings after explicit review.

## Player decisions that remain open

The plugin should not silently settle these policy questions:

- Should freight tariffs be purely fuel-cost recovery, or include a separately configured operating margin?
- Should margins/fees vary by distance, vehicle class, cargo, or only by observed trip cost?
- What should happen when transfer attribution is ambiguous or the receiver cannot afford a charge?
- Should the mod eventually support private carrier vehicle ownership and maintenance, or treat the carrier as a service provider using native vehicles?
- Should source competition across multiple DOs remain fully native/competitive, or should there be a reservation service?

Until decided, keep the economic model transparent, log all inputs, avoid hidden subsidies, and do not invent unapproved wages or fixed prices.
