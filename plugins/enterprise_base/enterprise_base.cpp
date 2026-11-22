// enterprise_base - private enterprises and the investment pool.
//
// Level 2. Consumes sim_clock, currency, persistence, ui_overlay,
// ui_panels, citizen_labor, citizen_retail, taxes and resource_pricing;
// provides enterprise_base.
//
// v9.20 NO MARKET MULTIPLIER (economy-direction 2026-11-22, code step 1).
//
//   resource_pricing v5 removed the Republic market order book. The
//   destitute bread-day is now native P_food x destitute_bread_tons, and
//   the v8.1 "Market" Shortage/Glut row in the building window is gone.
//   resource_pricing is still consumed (v5 -> v1) for the food price.
//
// v9.15 PRIVATISATION BAR.
//
//   The PRIV control on a non-residential building was a 40-unit toolbar
//   tile. On a panel scaled to dpi 0.80 that is 32 pixels carrying four
//   letters - it works, but it cannot be read at a glance and it cannot
//   say what it does. v9.15 uses ui_panels v6.8's widgetWideButton
//   instead: a full-width bar captioned "MAKE THIS BUILDING PRIVATE" /
//   "PRIVATE - CLICK TO RELEASE".
//
//   Residences keep the icon-row tile (confirmed correct on screen) and a
//   construction site keeps the toolbar PC tile. If ui_panels is older
//   than v6.8 the old small tile is used unchanged - the capability is
//   checked with TsmUiPanelsHasWideButton, so an older ui_panels DLL
//   degrades to v9.14's appearance rather than to no button.
//
//   No economic behaviour changes in this version.
//
// v9.14 WAGE RATE AUDIT - fixes the defect that made v9.13 inert.
//
// v9.13 capped at `getEffectiveWageForBuilding x days x premium`, and the
// 9.13 session produced zero [WAGE CAP] lines. The two wage ladders explain
// why: the factory ladder is 90.00 x 0.30..0.70 (3.00/day), but the
// shop/pub ladder is 1816.80 x 0.20..0.70 - a 60.56/day rate. citizen_labor
// restores the six sector base wages from tesmio_labor.dat over the INI,
// after the banner printed 2.80, so the contract became 2725.20 and no
// wage could ever exceed it.
//
// v9.14 audits the rate, refuses one above `minimum x 8` as corrupt, and
// dumps the live sector table at the first payroll.
//
// v9.13 WAGE CONTRACT CAP - behaviour change.
//
// v9.12 answered its own question: `mult` was 1.00 on all 94 wages, so the
// 4x-14x inflation arrives inside `grossWage` from citizen_labor, not from
// this plugin's bidding. The 21:55 session paid 31,511.76 RUB of wages
// against 467.86 RUB of revenue - 67:1 - and that is what kills every
// private enterprise in the save.
//
// This plugin posts a daily rate and a +50% premium ceiling, so it caps the
// wage at `posted x days_per_month x max_private_premium` and logs
// [WAGE CAP] with both numbers whenever the cap bites. wage_cap_enabled = 0
// restores the old behaviour. Fails open if the labor API or clock is mute.
//
// v9.12 DIAGNOSTIC BUILD - NO BEHAVIOUR CHANGE.
//
// Two things the 21:37 session could not answer about itself:
//   - why a worker is paid 726.72 when the rate is 2.80/day and the premium
//     cap is +50% (8x the ceiling, in eight steps of 90.84). [WAGE] now
//     records grossWage, mult and the result on every wage.
//   - how much a shop actually earned. cat=0 credits were suppressed outside
//     a refusal watch, which hid every counter sale and every service fee.
//     They are logged now.
//
// v9.11 PAYROLL AFFORDABILITY FIX - first behaviour change since v9.6.
//
// A private shop paid any wage it was asked to pay as long as its balance
// was above zero. On 2026-10-18 "Avryu Small store [P]" held 407.34 and
// paid a 1090.08 wage: the worker was credited for real, the balance went
// to -682.75, and the 682.75 difference was created out of nothing. The
// enterprise was then [DISTRESSED] two milliseconds later, and five of its
// neighbours at "Avryu Pub [P]" were refused outright because that shop
// was already at or below zero - the same wrong predicate, the opposite
// wrong answer. A boolean "has some money" is not "can pay this".
//
// v9.11 asks for the amount (CanPayWage) and refuses to credit a wallet
// whose debit did not land. allow_debt = 1 remains the explicit opt-in to
// overdraft, so saves that want the old behaviour keep it.
//
// v9.10 DIAGNOSTIC BUILD (P2 attribution trace) - NO BEHAVIOUR CHANGE.
//
// v9.9 answered the question and then lied about it: the [WATCH n END]
// residual was 0.000 in every window, while [AUDIT GAP] fired seven times
// claiming capital had moved untraced. The gaps were self-inflicted - the
// refusal branch reset the audit baseline to the arm-time balance, so every
// later check compared against a stale number. v9.10 deletes that write.
// The residual is the trustworthy metric and always was.
//
// v9.9 DIAGNOSTIC BUILD (P2 attribution trace, second attempt) - NO
// BEHAVIOUR CHANGE.
//
// v9.7 logged every movement and ran out of budget in 60 seconds of busy
// shopping; all 14 refusals in that session happened after both caps had
// filled, so the window that mattered was dark. v9.9 inverts it:
//
//   - SILENT by default. No line per sale, no line per wholesale charge.
//   - A REFUSAL ARMS A WATCH on that shop for g_watchMs. Every capital
//     movement on it - credit or debit, from any caller - is logged for the
//     duration, with the payer and the running total.
//   - SELF-AUDIT. The wrapper remembers the last value it wrote for each
//     building. If the next read disagrees, capital changed behind its
//     back: one loud [AUDIT GAP] line, once per second at most. This
//     catches the three in-plugin direct writes AND anything added later.
//   - The three direct writes - dividend siphon, auction buyout,
//     auto-registration - log themselves.
//
//   A session now produces a few lines per refusal and a verdict, instead
//   of 1000 lines and silence at the wrong moment.
//
// v9.7 DIAGNOSTIC BUILD (P2 attribution trace) - NO BEHAVIOUR CHANGE.
//
//   Every enterprise capital movement now logs the function that caused it,
//   and every completed retail sale logs the buyer. Nothing about pricing,
//   charging, eligibility or accounting is touched: the arithmetic below is
//   byte-for-byte v9.6.
//
//   Why: "the shop gained 1.60 while the citizen was refused" cannot be
//   settled by reading source - it needs the two sides of the transaction on
//   the same log line. With this build, one of four things is true, and the
//   log says which:
//
//     [RETAIL] insufficient + [SALE] person A   -> a DIFFERENT citizen bought
//                                                  something. Not a bug.
//     [CAP] with no matching [SALE]             -> a phantom. `by=` names
//                                                  the exact function.
//     [SALE] for the SAME person as a refusal   -> contradiction: real bug.
//     no [CAP] line at all                      -> capital is not moving
//                                                  through this plugin.
//
//   Log cost is bounded: 600 [CAP], 400 [SALE], 200 refusal lines per
//   session. Nothing is logged when g_probe is 0.
//
//   Remove this build (ship the v9.6 file instead) once P2 is attributed.
//
// v9.6 updates (retail money conservation + the PRIV tile regression):
//   - ATOMIC CHARGE+CREDIT on the retail path. TryModifyCapital() returns
//     whether the delta actually landed; OnRetail refunds the wallet (both
//     halves - see RefundRetailCharge) and returns TSM_RETAIL_INSUFFICIENT
//     when it did not. Before this, ModifyCapitalImpl silently dropped the
//     credit on a decayed building / lost private tag and the debit stood.
//     The invariant is now structural: on every return path of OnRetail,
//     wallet-delta == capital-delta, or both are zero.
//   - RefundRetailCharge() reverses person+0x734 as well as the ledger.
//     currency's WalletTake writes that engine accumulator, so a refund that
//     only called addWalletBalance left the citizen's satisfaction (+0x738)
//     and the city's retail turnover inflated by money that was handed back.
//   - Non-finite (NaN) price is a refusal, not a free sale. A zero price
//     still falls through to the state path, which charges 0 - same visible
//     outcome, without declaring a broken price "handled".
//   - BUILDING DISCRIMINATOR on both providers. TsmLooksLikeBuilding()
//     (game_world.h v8.10) gates EnterpriseWidgetProvider, EnterprisePanelRows
//     and ApplyPrivateState. Root cause of the session-long PRIV-tile
//     outage: ui_panels ran the widget pass on the citizen panel with a
//     Person* as `building`, and TsmBuildingType() read person+0x318 - the
//     FIFTH demand record's float pair {remaining, rate} - as a type
//     descriptor. The follow-up deref faulted, and ui_panels dropped this
//     provider for the rest of the session. ui_panels v6.6 fixes the caller;
//     this is the defence that keeps a future caller honest.
//   - Note on the reported "phantom income": the credit at the old line
//     ~2561 was ALREADY behind the abort gate (see the OnRetail comment
//     block). The code could not credit a refused sale; what it could do was
//     charge a citizen and fail to credit the shop. Both directions are now
//     closed by the same change.
//   - Service ABI unchanged; save format unchanged; version -> 9.6.
//
// v9.5 updates (closing the private-shop underpayment exploit):
//   - OnRetail now PRE-CHECKS the wallet against the full private price
//     (markedUp x (1 + vatRate)) BEFORE any charge and returns the
//     citizen_retail API v2 sentinel TSM_RETAIL_INSUFFICIENT (-1) when
//     it cannot be covered - taking nothing. Rationale, re-confirmed
//     against src/currency_api.h and plugins/currency/currency.cpp:
//     Api_ChargeCitizen delegates to WalletTake, which unconditionally
//     writes balance - min(balance, maxAmount) and only THEN reports
//     success/failure (the money is destroyed on a "failed"
//     all-or-nothing charge), and chargeCitizenPartial keeps whatever
//     was available. Refusing after either call would be wallet
//     theft, so refusal must happen before the take.
//   - Post-charge shortfall guard: if a concurrent writer drained the
//     wallet between check and take (taken < finalPrice), -1 is
//     returned as well - the partial take is already done, but the
//     gate must abort the dispense rather than hand over goods for
//     partial payment.
//   - Probe-gated, rate-limited [RETAIL] abort log. Consumes the v2
//     citizen_retail iface (TSM_CITIZEN_RETAIL_VERSION); a v1-only
//     citizen_retail fails the consume and no interceptor registers,
//     so a -1-returning OnRetail can never be read by an old gate.
//   - Version -> 9.5; save format unchanged.
//
// v9.3 updates:
//   - Fixed maximum worker capacity detection: StaffingRatioImpl and
//     StaffCountsImpl now read TSM_BLD_MAX_WORKERS (0xDEC, int) instead of
//     TSM_BLD_CUSTOMER_CAP (0xDDC, float). The 0xDDC field is the dynamic
//     customer service capacity, not the maximum worker limit — using it
//     caused nonsensical UI like "3/15 Staff" where 15 was customer
//     capacity, corrupting StaffingRatioImpl and M2 wage bidding.
//     See docs/Building_Vectors_Investigation.md.
//
// v9.4 updates:
//   - Fixed DrainSampleWallets critical-section imbalance: the lock was
//     entered once but left twice (once inside the residential-building
//     loop, once after it), corrupting g_lock ownership and deadlocking
//     any later EnterCriticalSection. Lock is now held for the full
//     iteration and released exactly once.
//   - Pre-ruin eviction guard: before CallNativeAbandon collapses a
//     residential building into ruins, all remaining residents are
//     relocated via FUN_14083CA50 (FindRelocationCandidate +
//     InvokeRelocateRaw) to prevent citizen pointer corruption.
//   - Version -> 9.4; save format unchanged.
//
// v9.2 updates:
//   - StaffingRatioImpl and StaffCountsImpl switched from
//     TSM_BLD_PEOPLE_BEGIN/END (0xBD8) to TSM_BLD_WORKERS_BEGIN/END
//     (0xBC0/0xBC8). Customer presence in 0xBD8 no longer inflates
//     labor staffing ratio or UI capacity. See
//     docs/Building_Vectors_Investigation.md.
//
// v9.1 updates (Task 7 - CPI survival basket & grounded eviction):
//   - API v8: getResidentialRentMonthly appended (the neutral per-tenant
//     monthly rent SETTING in active-currency units) so taxes can derive
//     the true average daily rent for the City Hall CPI readout. The
//     provider dual-publishes the v7 prefix view and the full v8 iface
//     (same pattern as currency v3/v4, citizen_labor v2/v3/v4).
//   - DESTITUTE EVICTION STAGE in CollectResidentialRent: a resident
//     whose PRE-CHARGE wallet cannot cover ONE day of staple bread
//     (Pfood x destitute_bread_tons, live food price x market multiplier
//     from resource_pricing) is staged for relocation regardless of
//     whether the rent take succeeded - destitution, not arrears. The
//     homelessness backstop is untouched: no vacant flat anywhere keeps
//     the citizen in place with the arrears dent; never nullptr.
//   - Version -> 9.1; save format unchanged.
// v9.0 updates (construction-site privatisation tile):
//   - The widget provider now branches on the engine's own built flag:
//     finished panels keep the big PRIV tile in the icon row; a
//     construction-site window (built < 1.0) draws the smaller
//     toolbar-docked CON tile (ui_panels v4.9 widgetButtonSmall).
//   - This closes the privatise-before-built gap: private_construction
//     adopts a site the tick it appears in g_companies, but the PRIV
//     tile only ever existed on finished panels - so a building under
//     construction could never be marked, and private construction
//     could never start. Clicking CON registers the site exactly like
//     PRIV (same ApplyPrivateState, same save rebind).
//   - private_construction runs the widget pass on the construction
//     panel through ui_panels' new appendWidgetsToWindow (its own hook
//     owns that builder's prologue).
//   - Service ABI unchanged; save format unchanged; version -> 9.0.
// v8.9 updates (panel polish):
//   - Rent row value shortened: dropped "(Total: +TOTAL)" which duplicated
//     the month-sales row and overflowed the panel's right edge (user
//     screenshot); now "RATE SYM/tenant (Attr XX%, Rent YY%)".
//   - Service ABI unchanged; save format unchanged; version -> 8.9.
// v8.8 updates (fresh privatisations showing "Abandoned (Ruins)" - log
// verdict):
//   - ROOT CAUSE, proven by the v8.7 panel probe: TSM_BLD_DECAYED (+0x1198)
//     is a BYTE field (the dispatcher routes on *(char*)(bld+0x1198) == 0),
//     but TsmIsDecayed read the full INT - and the upper three bytes hold
//     unrelated adjacent data. Live, occupied dwellings probed with
//     decC=0 alongside decI garbage: 268700160, -256, -1061158912
//     (0xC0C0C0C0 = float bits), 1048576. Every one of those misfired the
//     int read, so RunRegistrationPass latched the just-privatised
//     enterprise isAbandoned=true / isPrivate=false <= 300 ms after the
//     click - the panel then rendered "Status: Abandoned (Ruins)" on a
//     full, occupied dwelling and every economic loop stood down.
//   - FIX: TsmIsDecayed now reads the byte, exactly the engine's own
//     routing view (game_world.h - single choke point, all consumers:
//     pass sweep, IsPrivateImpl, relocation, eviction).
//   - LOAD-TIME HEAL: saves written by the broken build carry
//     isAbandoned=1 for live dwellings; ProcessDeferredLoadMatches now
//     restores those to live private firms when the engine's persisted
//     ruin byte is clear. Genuine ruins (byte = 1, whether from the
//     insolvency latch's native swap or a collapse) keep the flag - the
//     "ruins never re-adopt" design is untouched.
//   - Enterprise struct was audited: all fields carry default
//     initialisers, so the panel-button create path was never the source.
//   - Service ABI unchanged; save format unchanged; version -> 8.8.
// v8.7 updates (same-type dwellings missing the tile - log verdict):
//   - VERDICT from the v8.6 probe session: the engine dispatcher routes
//     by STATE before type (d_dispatcher.txt 540-563): unbuilt/bbox ->
//     construction panel 0x7838E0, +0x1198 char != 0 -> damage/ruin
//     panels, +0x23c view modes -> info panels. Only finished, clean
//     windows reach the 5 hooked builders that carry the tile. BUT for 6
//     of 14 probed windows the row provider ran while the tile provider
//     never logged - meaning an in-plugin gate aborted AFTER the engine
//     routed the correct panel.
//   - ROOT CAUSE: TsmIsDecayed reads *(int*)(bld+0x1198) != 0 while the
//     dispatcher allows the NORMAL panel chain on the low CHAR == 0
//     (d_dispatcher.txt 563) - any state value whose low byte is 0 but
//     whose int is nonzero (or a mid damage state 1/2) passes the
//     engine's routing yet was silently refused by the tile provider.
//     TsmIsDemolished is equally redundant here: the dispatcher itself
//     routes +0xEA8 != 0 windows to the demolished panel 0x77C940, which
//     none of the 5 hooked builders serve - so those windows can never
//     reach this provider anyway, and the check can only misfire on a
//     stale read. BOTH GATES REMOVED from the tile provider: the engine's
//     own dispatcher is the authority for "is this a privatisable
//     panel", and it already ran (the provider was called from that
//     panel's builder). Simulation sweeps keep their dem/dec checks.
//   - Probes upgraded to close the loop: the one-shot tile probe now
//     logs reached / bail unreadable / bail cityhall for EVERY window
//     that reaches the provider, and the one-shot panel probe logs the
//     building state (type, dem, dec char AND int, built, bbox) so any
//     future "no tile" report is one log line, not one session.
//   - Service ABI unchanged; save format unchanged; version -> 8.7.
// v8.6 updates (privatisation-tile routing diagnosis):
//   - VERDICT on the "DescribeEnterpriseImpl catch-22" claim: FALSE on
//     both mechanisms. (1) EnterpriseWidgetProvider has NO registry gate -
//     it reads g_companies only for the tile's pressed state and calls
//     widgetButton unconditionally, and ui_panels runs widget providers
//     even when the row provider pushed 0 rows (AppendRowsImpl only bails
//     when rows AND widget count are both zero). (2) SafeReadStr reads
//     typedesc+0x00 (the mod object id), never bld+0x10, and custom-name
//     scanning was retired BY DESIGN in v8.5 - tagging is supposed to be
//     dead, that is not the regression.
//   - REAL cause of "same type, one tiles one doesn't": engine PANEL
//     ROUTING, upstream of all mod code. The dispatcher
//     (scratch/d_dispatcher.txt 540-812) picks the window builder by
//     STATE before type: built < 1.0 or a placement bbox at bld+0x5D0 ->
//     construction panel 0x7838E0 (no ui_panels hook, no tile); decayed
//     (+0x1198) skips the type chain; window+0x23c view modes {1..10,14}
//     -> info panels. Only a finished, bbox-clear, non-decayed type-2
//     window reaches the residence builder 0x761300 that carries our
//     widget pass. Two identical dwellings differing in STATE (one still
//     building / stalled on materials / ruined) tile differently.
//   - Added a one-shot [PRIV TILE] probe log per building pointer: its
//     presence/absence in tesmioloader.log instantly separates "engine
//     routed elsewhere" from "widget no-op".
//   - Service ABI unchanged; save format unchanged; version -> 8.6.
// v8.5 updates ([P] detection failure -> panel privatisation button):
//   - VERDICT on the SSO/UTF-8 claim: direction right (string scanning is
//     the wrong mechanism), mechanism wrong. The tesmio log shows 14,707
//     "enterprise_base tag scan" C0000005 faults at
//     enterprise_base.dll+0x7957: v8.4 pass 2 (heap follow) dereferenced
//     FLOAT bit-patterns read from 8-byte struct slots as pointers
//     (0x3F61C598, 0xBF800000 = -1.0f). TsmIsValidGamePtr's range check
//     passes them; the raw read hits unmapped low memory; the SEH aborts
//     the WHOLE scan - so the ASCII pass never ran and almost every
//     building came back "no tag". A real heap pointer to UTF-8 text would
//     NOT fault (it would fail LegitW) - the garbage-pointer AV is the
//     proven failure, not the encoding mismatch.
//   - FIX: privatisation is now the ui_panels widget - the TAX-style
//     native square tile ("PRIV", checkbox fallback) on every hooked
//     building window (City Hall excluded: taxes' TAX tile owns that
//     chrome). The click writes g_companies directly (create with
//     starting_capital; un-privatise keeps capital in reserve); no string
//     parsing, no span scan, no 300 ms cost. State persists through
//     OnSaveLoad/EntSaveRecord and rebinds by custom name or type+pos.
//   - CheckBuildingHasTag loses the custom-name span scan (hot path is now
//     one SafeReadStr); RunRegistrationPass loses the "!hasTag ->
//     un-privatise" branch, which would have reverted every button click
//     300 ms later. Internal_GetBuildingCustomName keeps its inline spans
//     for display/load matching, and its heap-pointer follow now
//     host-validates each candidate (ReadablePtr) - kills the AVs.
//   - Row budget: TSM_PANEL_MAX_ROWS 12 -> 14 so a fully-loaded private
//     shop (11 rows) + the 2-row tile fits (ui_panels 4.1).
//   - Service ABI unchanged (v6); save format unchanged; version -> 8.5.
// v8.4 updates (lag fix + tag perf):
//   - Tag-scan perf: v8.3 called the host ReadablePtr (a VirtualQuery
//     region walk) PER OFFSET - ~4600 UTF-16 + ~4600 heap-pointer + ~4600
//     ASCII calls per building per 300 ms registration pass. Replaced with
//     ONE header check + a single SEH-guarded raw pass: tag-anchored
//     byte-pattern search (find "[P]"/"(P)" bytes first, validate only on
//     a hit) + 8-byte aligned heap-pointer follow. Same coverage
//     (0x10..0x1200, both parities, SSO heap spill), ~4 host calls.
//   - Service ABI unchanged (v6); save format unchanged; version -> 8.4.
// v8.3 updates ([P] tag + rent + legibility task):
//   - Tag detection rewrite (Order 2): Internal_GetBuildingCustomName now
//     scans bld+0x10..0x1200 (was 0x10..0x580, cutting off larger structs),
//     follows heap wstring pointers (long/Workshop names spill to heap via
//     SSO), and tests UTF-16 at BOTH byte parities (odd-aligned names were
//     invisible to the step-2 scan). Tag-anchored first pass returns the
//     tagged string immediately instead of longest-string-wins (a 25-char
//     default prefix used to beat a 3-char "[P]" tail). CheckBuildingHasTag
//     drops the 16-char default-prefix veto (a legit "Default Name [P]"
//     rename was vetoed as "default").
//   - Residence rows (Orders 1+3): EnterprisePanelRows opens with a native-
//     style divider "[ Private Enterprise ]", then Owner + Rent on the first
//     two data lines so the rate is visible without scrolling (was 4th).
//   - Rent grounding (Order 4): RentRateFor multiplies by the verified
//     native rent slider (bld+0x13E0, 0..1, FUN_14073E950.GetString(0x995))
//     as 0.75+0.5xlevel (neutral 1.0 at 0.5); the Rent row shows both
//     Attr XX% (bld+0x11B0, FUN_1404D5F50) and Rent YY% (bld+0x13E0).
//     0x11B0 confirmed correct for residences (type-2-only writer; tourism
//     hotels/attractions use a separate star path, not 0x11B0).
//   - Service ABI unchanged (v6); save format unchanged; version -> 8.3.
// v8.2 updates (Task 1: economic eviction & household relocation):
//   - Park exploitation cap: the attractiveness multiplier is scaled by
//     the dwelling count (storage-0 capacity, the INI
//     RESOURCE_TRANSPORT_PASSANGER figure; civ_mesto* blocks declare
//     75..135, dedina shacks 3..13, so PARKCAP_FLAT_REF = 30): a
//     low-capacity shack in a park paradise now pays near the slum floor
//     instead of luxury rates. Toggle: park_cap_enabled (1).
//   - Eviction evaluation in CollectResidentialRent: ability-to-pay is
//     snapshotted BEFORE the wallet take (post-charge would over-evict);
//     a tenant is staged when tenantRent > rent_burden_threshold (0.35)
//     x wallet OR the partial take covered < 95% of the rent.
//   - Relocation search FindRelocationCandidate(game, currentBld, wealth)
//     scans the town building vector for a vacant type-2 flat: the poor
//     (wealth < 0.40) are steered to state/cheap flats, the wealthy
//     (>= 0.70) score top attractiveness first, everyone else takes the
//     cheapest fit; pointer tiebreak keeps it deterministic.
//   - Safe dispatch OUTSIDE g_lock: pairs stage lock-free, candidates are
//     re-validated, and native FUN_14083CA50 (RVA 0x83CA50) runs
//     SEH-isolated. Never evicts into nullptr: no vacancy -> the citizen
//     is retained in place with a small arrears happiness dent (the
//     homelessness backstop; vanilla would despawn them).
//   - New INI keys: eviction_enabled (1), rent_burden_threshold (0.35),
//     eviction_penalty_happiness (0.15), park_cap_enabled (1).
//   - v7.0 updates (debug hotkeys & macro readouts - Task 2/3):
//     - Internal_GetBuildingCustomName does a full-span multi-string scan of
//       bld+0x10..bld+0x580 (UTF-16 step 2, ASCII step 1); CheckBuildingHasTag
//       also tests the type-descriptor object name, so Workshop cottages,
//       modded flats and default-name-prefix buildings register as [P]
//       immediately.
//     - debug_mode INI toggle (default 1 in dev) gates hotkey polling in
//       OnTick guarded by GetForegroundWindow() == GameWindow().
//       Ctrl+F9 runs a manual rent & eviction pass (CollectResidentialRent);
//       Ctrl+F11 drains the first 30 citizen wallets to 0.50 RUB.
//     - Export getMonthlyEvictionCount() from the enterprise_base API: the
//       successful-evictions count for the current month, reset at each
//       month boundary. Included in the taxes Accounting Office macros.
//   - Service ABI unchanged from v6 (no existing field moved); only the plugin
//     version string moves to 8.x. Save format unchanged.
//
// v8.1 updates (UI clarity polish, Task 1 follow-up):
//   - Residence Rent row now shows the ACTUAL per-tenant rate
//     (g_residentialRent x (min + (max-min) x attr)) alongside the month's
//     collected total: "RATE SYM/tenant (Total: +TOTAL) (Attr XX%)". The row
//     v8.9: dropped the redundant "(Total: +TOTAL)" - it duplicated the
//     month-sales row and pushed the value past the panel's right edge
//     ("Rent: 3.75 RUB/tenant (Total: +0.00) (Attr 0%, Rent 50%)" is 56
//     chars; the shorter form is 41 and fits inside the parchment width).
//     is overwritten in place, so the panel row count is unchanged.
//   - Retail (3) / factory (6) / service (23) windows gain a Victoria-3
//     "Market" condition row (Balanced / Shortage +N% / Glut -N%), read from
//     resource_pricing's Republic macro order book via the food bellwether.
//     Optional consume; pre-v2 resource_pricing or its absence skips the row.
//   - Service ABI unchanged (TSM_ENTERPRISE_BASE_VERSION stays 6u); save
//     format unchanged. Only the plugin version string moves to 8.1.
//
// v8.0 updates (Urban Land Value + Phased Insolvency):
//   - Task 1 (attractiveness rent): CollectResidentialRent scales each
//     residence's per-tenant rent from the engine's environmental
//     attractiveness heightfield (bld+TSM_BLD_ATTRACTIVENESS, 0..1,
//     fallback 0.50): tenantRent = g_residentialRent x (g_rentMinMult +
//     (g_rentMaxMult - g_rentMinMult) x attractiveness). New INI keys
//     rent_attr_min_mult (0.50) / rent_attr_max_mult (2.00). The in-window
//     Rent row shows "+XX.XX SYM (Attr XX%)". Currency-neutral (pure
//     multipliers over the USD-scaled base).
//   - Task 3 (phased insolvency): Enterprise gains insolventMonths and
//     isAbandoned (persisted in save v4, v1..v3 records load with the
//     defaults). EvaluateInsolvency runs in OnMonth after wage bidding
//     and advances negative-capital firms: month 1 logs [DISTRESSED]
//     (hiring freeze rides the EXISTING insolvent-payroll poison path -
//     OnPayroll already returns TSM_PAYROLL_DEFAULTED when !IsSolvent,
//     which makes citizen_labor poison the workplace; no new cross-DLL
//     call is needed or safe); month 2 attempts a pool buyout auction
//     at startingCapital x auction_buyout_cost_mult (1.5x), resetting
//     capital on success; month >= months_to_abandon (3) calls native
//     FUN_14013FE70(game, bld) RVA 0x13FE70 (SEH-isolated, POD helper),
//     strips the tag and marks abandoned. New [insolvency] INI section.
//     A "Status" panel row reports Solvent/Distressed/Insolvent/Abandoned.
//     Resurrection is blocked three ways: abandoned firms keep isPrivate
//     false and are skipped by every economic loop; RunRegistrationPass,
//     ModifyCapitalImpl and IsPrivateImpl refuse engine-decayed buildings
//     (bld+TSM_BLD_DECAYED, set by the native call and engine-persisted);
//     SaveRegister never writes non-private entries.
//   - Service ABI unchanged (TSM_ENTERPRISE_BASE_VERSION stays 6u - no
//     accessor moved or added); only the plugin version string moves
//     to 8.0. Save format moves v3 -> v4 (append-only trailing fields).
//
// v7.0 updates (Labor & Salary Overhaul, Milestone 2 - private wage bidding):
//   - The static private_wage_bonus multiplier is replaced by an
//     autonomous per-enterprise state machine. Enterprise gains
//     wageBonusMult (1.0 .. [private_market_wages] max_private_premium,
//     default 1.50) and an inAusterity flag; both are persisted (save
//     v3, v1/v2 records load with the default 1.0).
//   - Monthly evaluation (OnMonth, after the siphon, BEFORE the
//     accumulator clear loop - it reads the same fields the loop
//     zeroes): profitable + above-reserve + understaffed firms bid
//     +wage_bidding_step/month; loss-making or capital-exhausted
//     firms cut -wage_bidding_step/month down to 1.0 (effective pay
//     still floored to minimum_wage at payroll). Probe logs
//     [WAGE HIKE] / [AUSTERITY].
//   - Staffing ratio = live workers (0xBC0 workers vector) / max(1, capacity
//     int at bld+TSM_BLD_MAX_WORKERS). Unreadable structure => 1.0 ("fully
//     staffed") so no firm bids up on garbage; a valid empty vector => 0.0.
//     Reads game memory - the caller holds g_lock and brackets the pass.
//   - OnPayroll pays grossWage x wageBonusMult (dynamic) or the legacy
//     static bonus (dynamic_bidding_enabled = 0), then floors the
//     result to citizen_labor's statutory minimum via the v3 API
//     (labor consume now prefers v3, falls back to v2 without a floor).
//   - In-window telemetry: a "Wage" row shows the effective rate with
//     its state suffix (+XX% Market Bonus / Market Baseline /
//     Austerity Mode) and a "Labor" row shows
//     XX/XX Staff (XX% Capacity), both in the active currency.
//     ui_panels max_rows 7 -> 8.
//   - New INI section [private_market_wages]: dynamic_bidding_enabled,
//     max_private_premium, understaffed_threshold, wage_bidding_step.
//     Service ABI
//     unchanged (TSM_ENTERPRISE_BASE_VERSION stays 6u - no accessor
//     moved or added); only the plugin version string moves to 7.0.
//
// v6.2 updates:
//   - Unified Global Currency (v4): currency consume prefers the v4 iface
//     (v3 fallback). At Start, USD mode resolves starting_capital,
//     residential_rent, starting_pool and legal_reserve_flat from their
//     RUB bases via usd_divisor - explicit *_usd keys win. Panel "Capital"
//     row, the overlay card pool line and probe logs carry the active
//     RUB/USD symbol.
//
// v6.1 updates:
//   - Dividend Field No Longer Wiped On The Spot: SiphonToPool wrote
//     e.monthlyDividendPaid and OnMonth's clearing loop - running in the
//     same call, microseconds later - zeroed it again, so
//     DescribeEnterpriseImpl and the EnterprisePanelRows "Net profit"
//     row read div +0.00 for every enterprise, every frame, forever.
//     The reset now lives inside SiphonToPool right after the
//     same-month skip guard (the enterprise's new cycle starts there),
//     and the post-siphon loop no longer touches the field - that
//     loop's position stays put because SiphonToPool consumes the very
//     accumulators it clears (clearing before the siphon would zero
//     netProfit and stop every dividend).
//
// v5.4 updates:
//   - Net-Profit Siphon: SiphonToPool takes dividends strictly from net
//     positive operating profit (gross - wages - wholesale - taxes),
//     capped by excess over the reserve. Idle firms pay nothing.
//   - Residential Rent: CollectResidentialRent charges residential_rent
//     per resident per month at the month boundary via
//     currency's partial wallet take, credited as TSM_CAP_RENT revenue.
//
// v5.3 updates:
//   - Correct VAT Capital Crediting: OnRetail credits gross `taken` to capital.
//     When taxes::SettleVatAll debits `vatAccrued`, capital retains netToShop,
//     and periodGrossSales accurately reflects gross revenue for taxes::SettleProfitAll.
//   - Foreign Restock Siphon Buffer: TSM_CAP_FOREIGN costs increment
//     monthlyWholesaleCosts so SiphonToPool's reserve protects working capital
//     used for foreign imports.
//   - Save Matching Disambiguation: FindMatchForRecord validates object name
//     and position bounds even when customName matches, preventing identical
//     tags from cross-wiring on load.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_overlay_api.h"
#include "../../src/ui_panels_api.h"
#include "../../src/citizen_labor_api.h"
#include "../../src/citizen_retail_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/taxes_api.h"
#include "../../src/resource_pricing_api.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdint.h>
#include <cmath>

#pragma comment(lib, "user32.lib")   // GetForegroundWindow / GetAsyncKeyState

// ---------------------------------------------------------------- services

static const TsmSimClockApi*       g_simClock    = nullptr;
static const TsmCurrencyApi*       g_currency    = nullptr;
static const TsmPersistenceApi*    g_persistence = nullptr;
static const TsmUiOverlayApi*      g_ui          = nullptr;
static const TsmUiPanelsApi*       g_panels      = nullptr;
static const TsmCitizenLaborApi*   g_labor       = nullptr;
static const TsmCitizenRetailApi*  g_retail      = nullptr;
static const TsmTaxesApi*          g_taxes       = nullptr;
static const TsmResourcePricingApi* g_pricing    = nullptr;   // destitute bread-day food price

static int g_persHandle = -1;

// ---------------------------------------------------------------- config

static int    g_enabled              = 1;
static int    g_probe                = 1;
static int    g_card                 = 1;   // overlay enterprise card
static int    g_panelRows            = 1;   // in-window rows via ui_panels
// v8.5: widget tail capability flags for the privatisation tile (mirror
// taxes' g_panelsWidgets/g_panelsButtons). Set in Start after the ui_panels
// consume; the provider no-ops when both stay 0 (v1 ui_panels install).
static int    g_panelsWidgets        = 0;   // v2 widget tail present
static int    g_panelsButtons        = 0;   // v3 native square tile present
static int    g_panelsSmall          = 0;   // v4.9 toolbar-docked small tile
static int    g_panelsWide           = 0;   // v6.8 full-width PRIV bar

static double g_startingCapital      = 10000.0;
static float  g_defaultMarkup        = 1.30f;
static int    g_allowDebt            = 0;

static int    g_allPrivate           = 0;
static int    g_autoTypes[16];
static int    g_autoTypeCount;

static int    g_poolEnabled          = 1;
static double g_startingPool         = 50000.0;
static double g_legalReserveFlat     = 5000.0;
static float  g_payrollBufferMonths  = 1.5f;
static float  g_wholesaleBufferMonths = 1.5f;
static float  g_siphonRate           = 0.50f;

static float  g_privateWageBonus     = 1.20f;

// v9.13: a private employer will not pay more than it advertised. The
// contract is `posted daily rate x days per month x max_private_premium`,
// and it is the same ceiling the M2 bidding system claims to honour. 1 =
// clamp and log; 0 = pay whatever arrives (the pre-v9.13 behaviour).
static int    g_wageCapEnabled       = 1;

// v9.14: a resolved daily rate above `minimum_wage x this` is treated as
// corrupt and replaced by the minimum. The 9.13 session resolved
// services_retail at 60.56/day against an INI value of 2.80 - 24x the floor -
// which multiplied the contract up to 2725.20 and made the v9.13 cap
// unreachable. No sane daily wage is 8x the statutory minimum.
static float  g_wageRateCeiling       = 8.0f;

// M2: private wage market bidding ([private_market_wages]). The static
// g_privateWageBonus above applies ONLY when dynamic_bidding is off -
// it is the off-switch fallback, not the engine.
static int   g_dynamicBidding        = 1;      // state machine on/off
static int   g_dynamicBiddingVerified = 0;     // latched post-Start

// One-shot post-Start verification of citizen_labor's wage mobility hook.
// Deferred out of TsmPluginStart so monolithic (wrsre.dll) and standalone
// builds do not depend on Start() order, and called from both simulation
// and paused-frame UI paths (EnterprisePanelRows).
static int DynamicBiddingActive()
{
    if (!g_dynamicBidding) return 0;
    if (!g_dynamicBiddingVerified) {
        g_dynamicBiddingVerified = 1;
        if (g_labor && TsmLaborHasV5(g_labor) &&
            !g_labor->isWageMobilityHookInstalled())
        {
            Logf("enterprise_base  WARN  citizen_labor wage mobility hook not installed — "
                 "private_market_wages dynamic bidding disabled, using static INI wages");
            g_dynamicBidding = 0;
            return 0;
        }
    }
    return g_dynamicBidding;
}
static float g_maxPrivatePremium     = 1.50f;  // wageBonusMult ceiling (+50%)
static float g_understaffedThreshold = 0.75f;  // staffing ratio that bids up
static float g_wageBiddingStep       = 0.05f;  // monthly +/- adjustment step
#define      M2_WAGE_STEP_DEFAULT    0.05f     // default for the key above

static double g_residentialRent        = 10.0;   // v5.4: RUB per resident per month

// v8.0 (Task 1): attractiveness-scaled rent. tenantRent = g_residentialRent
// x (min + (max - min) x attr), attr = engine land value 0..1 (fallback
// 0.50). Held as floats (multiplier math); read in the enterprise_base
// section as rent_attr_min_mult / rent_attr_max_mult. The USD-scaled base
// carries whichever currency is active through unchanged.
static float g_rentMinMult = 0.50f;   // slum / industrial floor: 50% discount
static float g_rentMaxMult = 2.00f;   // luxury / park ceiling: 200% premium

// v8.2 (Task 1): economic eviction & household relocation. The burden test
// stages a tenant when tenantRent > threshold x wallet (default 35%); the
// penalty is the happiness dent applied on a forced move.
static int   g_evictionEnabled          = 1;     // master switch
static float g_rentBurdenThreshold      = 0.35f; // 35% of wallet/income
static float g_evictionPenaltyHappiness = 0.15f; // forced-move happiness dent
static int   g_parkCapEnabled           = 1;     // dwelling-count rent cap

// v9.1 (Task 7): destitute eviction stage. A resident whose PRE-CHARGE
// wallet cannot cover ONE day of staple bread (live Pfood x
// destitute_bread_tons, market-multiplied, active currency) is staged at
// the next rent pass regardless of whether the rent take succeeded -
// destitution, not rent arrears. bread_tons mirrors resource_pricing's
// [cpi_basket] basket_food_tons (docs/CPI_Survival_Basket_Investigation.md
// S1: the engine's 0.0015..0.0030 food ladder); the price is read live
// from g_pricing so USD mode and market swings need no extra config.
// Without resource_pricing the value stays 0 and the stage is inert
// (the burden / short-pay tests still govern - graceful degradation).
static int   g_destituteEnabled   = 1;
static float g_destituteBreadTons = 0.0020f;

// v7.0 (Task 2/3): debug hotkey + monthly eviction counter.
static int   g_debugMode                = 1;     // INI-driven: 1 = poll hotkeys
static size_t g_monthlyEvictionCount    = 0;     // successful evictions this month

static void ResetMonthlyEvictionCount(void)
{
    g_monthlyEvictionCount = 0;
}

// v8.2 (Task 1): native relocation primitive FUN_14083CA50:
//   void (__fastcall *)(void* game, void* person, void* newBuilding)
// Moves the person plus their whole family household atomically (unlinks
// bld+0xBC0, clears routes, recomputes tenant counters). A move into
// nullptr despawns the citizen (FUN_140835D70 frees them) - the plugin
// therefore always confirms a candidate flat first.
#define TSM_F_RELOCATE (g_tsmAddr.relocate_person) // RVA of native FUN_14083CA50
// Person floats (docs/Citizen_Housing_Relocation.md,
// docs/Rent_Housing_Task1_Investigation.md): 0xE4 living standard /
// wealth (0..1, maintained by FUN_14083A6D0), 0xE8 happiness (0..1).
// Both sit inside TSM_PERSON_SIZE.
#define TSM_PERSON_WEALTH    0xE4
#define TSM_PERSON_HAPPINESS 0xE8
// Park-cap reference: a residence whose dwelling count (storage-0
// capacity, the INI RESOURCE_TRANSPORT_PASSANGER figure) reaches this
// many flats charges the full attractiveness multiplier; smaller homes
// scale down linearly toward the slum floor.
#define PARKCAP_FLAT_REF 30.0f

// v8.0 (Task 3): phased-insolvency tunables from [insolvency].
static int    g_abandonEnabled    = 1;    // 1 = collapse ruins after N months
static int    g_monthsToAbandon   = 3;    // negative-capital months before ruin
static float  g_auctionBuyoutMult = 1.5f; // buyout = startingCapital x mult
#define TSM_F_ABANDON (g_tsmAddr.abandon_building) // RVA of native FUN_14013FE70

// v4: RUB-denominated bases as written in the ini. USD-effective values
// resolve at Start via usd_divisor unless an explicit *_usd key exists.
static double g_startingCapitalCfg  = 10000.0;
static double g_startingPoolCfg     = 50000.0;
static double g_legalReserveFlatCfg = 5000.0;
static double g_residentialRentCfg  = 10.0;
static double g_usdDivisor = 50.0;

static double UsdScale(void)
{
    double d = g_usdDivisor;
    if (!(d >= 1.0)) d = 50.0;
    return d;
}

static int IsUsdActive(void)
{
    return TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD;
}

// ---------------------------------------------------------------- the register

struct Enterprise {
    double capital      = 0.0;
    float  markup       = 1.0f;
    bool   isPrivate    = false;

    // M2: dynamic wage bidding state. wageBonusMult is the firm's
    // current market premium, clamped to [1.0, g_maxPrivatePremium];
    // inAusterity mirrors "last monthly evaluation was distressed" and
    // drives the panel's state suffix. Persisted in save v3.
    float  wageBonusMult = 1.0f;
    bool   inAusterity   = false;

    // v8.0 (Task 3): phased-insolvency state machine. insolventMonths
    // counts consecutive month boundaries spent at capital <= 0; reaching
    // g_monthsToAbandon (default 3) triggers physical abandonment.
    // isAbandoned latches once the native ruins swap has fired and prevents
    // re-registration even if the [P] tag survives on the name. Both are
    // persisted in save v4 (v1..v3 records load with the defaults 0/false).
    int    insolventMonths  = 0;
    bool   isAbandoned      = false;
    bool   payrollDefaulted = false;
    bool   stalledOnDust    = false;

    // v9.16: the player pressed RELEASE on this building in the panel.
    // The 300 ms registration sweep re-privatises anything that
    // CheckBuildingHasTag() matches - and with all_enterprises_private = 1
    // and auto_private_types = 43,44 that is EVERY distribution office. A
    // released DO was therefore private again 150-250 ms later (six
    // release/re-tag pairs in the 2026-10-20 log). A panel click is an
    // explicit instruction and outranks the config; cleared again when the
    // player privatises. In-memory only - after a reload the INI governs,
    // which is why the shipped ini now sets all_enterprises_private = 0.
    bool   releasedByPlayer = false;

    int    lastSiphonMonth = -1;
    int    lastSiphonYear  = -1;

    double monthlyGrossSales    = 0.0;
    double monthlyWholesaleCosts= 0.0;
    double monthlyWagesPaid     = 0.0;
    double monthlyTaxesPaid     = 0.0;
    double monthlyDividendPaid  = 0.0;

    double periodGrossSales     = 0.0;
    double periodVatAccrued     = 0.0;
    double periodWholesaleCosts = 0.0;
    double periodWagesPaid      = 0.0;
    double periodForeignCosts   = 0.0;

    double accruedVatLiability        = 0.0;
    double accruedProfitTaxLiability  = 0.0;
};

static std::unordered_map<void*, Enterprise> g_companies;
static std::vector<void*> g_companyOrder;

static void RebuildOrder()
{
    g_companyOrder.clear();
    g_companyOrder.reserve(g_companies.size());
    for (auto& kv : g_companies) {
        if (kv.second.isPrivate) {
            g_companyOrder.push_back(kv.first);
            continue;
        }
        // Leaving the private register ends the company's tax period:
        // taxes enumerates only this order, so totals kept across an
        // un-privatise/abandon/re-privatise cycle would be taxed in a
        // later (possibly suspended) window.
        Enterprise& e = kv.second;
        e.periodGrossSales     = 0.0;
        e.periodVatAccrued     = 0.0;
        e.periodWholesaleCosts = 0.0;
        e.periodWagesPaid      = 0.0;
        e.periodForeignCosts   = 0.0;
    }
}

// ---------------------------------------------------------------- name helpers

static inline bool LegitW(wchar_t c)
{
    if (c >= 32 && c <= 126) return true;
    if (c >= 0x00A0 && c <= 0x024F) return true;
    if (c >= 0x0400 && c <= 0x04FF) return true;
    return false;
}

static inline bool LegitA(char c)
{
    unsigned char u = (unsigned char)c;
    return (u >= 32 && u <= 126) || u >= 128;
}

// ---------------------------------------------------------------- tag detection
//
// v8.4 perf fix: the v8.3 widened scan called the host ReadablePtr (a
// VirtualQuery region walk) PER OFFSET - ~4600 calls for the UTF-16 pass,
// ~4600 for the heap-pointer pass, ~4600 for ASCII, per building, every
// 300 ms registration pass. That is the lag. This rewrite does ONE
// ReadablePtr per building (the 0xC00 header), then a single SEH-guarded
// raw pass: tag-anchored byte-pattern search (find "[P]"/"(P)"/Private
// bytes first, validate the surrounding string only on a hit) + 8-byte
// aligned heap-pointer follow. Same coverage (0x10..0x1200 span, odd
// parity, heap SSO spill), ~4 host calls per building instead of ~14000.

static bool ContainsPrivateTagW(const wchar_t* s, size_t maxLen)
{
    if (!s) return false;
    __try {
        size_t len = 0;
        while (len < maxLen && s[len] != 0 && LegitW(s[len])) len++;
        if (len < 2) return false;

        for (size_t i = 0; i + 3 <= len; i++) {
            wchar_t c0 = s[i];
            wchar_t c1 = s[i + 1];
            wchar_t c2 = s[i + 2];

            if ((c0 == 0x5B || c0 == 0x28) &&
                (c1 == 0x50 || c1 == 0x70) &&
                (c2 == 0x5D || c2 == 0x29))
                return true;

            if (i + 9 <= len) {
                if (_wcsnicmp(&s[i], L"[Private]", 9) == 0) return true;
                if (_wcsnicmp(&s[i], L"(Private)", 9) == 0) return true;
            }
        }
    } __except (FaultFilter("enterprise_base tag W",
                            GetExceptionInformation())) {
    }
    return false;
}

static bool ContainsPrivateTagA(const char* s, size_t maxLen)
{
    if (!s) return false;
    __try {
        size_t len = 0;
        while (len < maxLen && s[len] != 0 && LegitA(s[len])) len++;
        if (len < 2) return false;

        for (size_t i = 0; i + 3 <= len; i++) {
            char c0 = s[i];
            char c1 = s[i + 1];
            char c2 = s[i + 2];

            if ((c0 == 0x5B || c0 == 0x28) &&
                (c1 == 0x50 || c1 == 0x70) &&
                (c2 == 0x5D || c2 == 0x29))
                return true;

            if (i + 9 <= len) {
                if (_strnicmp(&s[i], "[Private]", 9) == 0) return true;
                if (_strnicmp(&s[i], "(Private)", 9) == 0) return true;
            }
        }
    } __except (FaultFilter("enterprise_base tag A",
                            GetExceptionInformation())) {
    }
    return false;
}

// v8.4: raw UTF-16 tag anchor. Finds the 6-byte LE "[P]"/"(P)" pattern
// at EITHER parity first, then validates the surrounding run only on a
// hit. No host calls inside; runs under the caller's SEH guard.
static bool TagAnchorScanW(const BYTE* b, size_t scanEnd,
                           wchar_t* out, size_t maxChars)
{
    for (size_t i = 0x10; i + 6 <= scanEnd; i++) {
        BYTE c0 = b[i], c1 = b[i + 1], c2 = b[i + 2];
        BYTE c3 = b[i + 3], c4 = b[i + 4], c5 = b[i + 5];
        bool hit = ((c0 == 0x5B || c0 == 0x28) && c1 == 0x00 &&
                    (c2 == 0x50 || c2 == 0x70) && c3 == 0x00 &&
                    (c4 == 0x5D || c4 == 0x29) && c5 == 0x00);
        if (!hit) continue;
        size_t start = i;
        for (int back = 0; back < 64; back++) {
            if (start < 0x10 + 2) break;
            size_t p = start - 2;
            wchar_t c = *(const wchar_t*)(b + p);
            if (c == 0) { start = p + 2; break; }
            if (!LegitW(c)) break;
            start = p;
        }
        size_t len = 0;
        bool ok = true;
        while (len < 64) {
            if (start + len * 2 + 2 > scanEnd) { ok = false; break; }
            wchar_t c = *(const wchar_t*)(b + start + len * 2);
            if (c == 0) break;
            if (!LegitW(c)) { ok = false; break; }
            len++;
        }
        if (!ok || len < 2) continue;
        if (*(const wchar_t*)(b + start + len * 2) != 0) continue;
        if (i < start || i + 6 > start + len * 2 + 2) continue;
        if (out && maxChars >= 2) {
            size_t copy = (len < maxChars - 1) ? len : (maxChars - 1);
            const wchar_t* ws = (const wchar_t*)(b + start);
            for (size_t k = 0; k < copy; k++) out[k] = ws[k];
            out[copy] = 0;
        }
        return true;
    }
    return false;
}

static bool Internal_GetBuildingCustomName(void* building,
                                           wchar_t* out, size_t maxChars)
{
    if (!out || maxChars < 2) return false;
    out[0] = 0;
    if (!TsmIsValidGamePtr(building)) return false;
    if (!ReadablePtr(building, TSM_BLD_MIN_READABLE)) return false;

    const BYTE* b = (const BYTE*)building;

    // v8.4 perf: ONE header check above, then a single SEH-guarded raw
    // pass. Inner reads are plain memory; a torn page faults once and
    // degrades to "no tag" instead of ~14000 VirtualQuery walks.
    size_t scanEnd = TSM_BLD_MIN_READABLE;
    if (ReadablePtr(b, TSM_TAG_SCAN_END)) scanEnd = TSM_TAG_SCAN_END;
    bool tagFound = false;
    __try {
        if (TagAnchorScanW(b, scanEnd, out, maxChars)) tagFound = true;
        if (!tagFound) {
            for (size_t i = 0x10; i + 8 <= scanEnd; i += 8) {
                void* p = *(void* const*)(b + i);
                if (!TsmIsValidGamePtr(p)) continue;
                // v8.5: host-validate before deref. This8-byte slot may be
                // a float pair (0x3F61C598, 0xBF800000 = -1.0f - the v8.4
                // AV storm at enterprise_base.dll+0x7957): the range check
                // passes float bit-patterns, the raw deref then hits
                // unmapped low memory. ReadablePtr costs one VirtualQuery,
                // and this path now runs only per open window / load match
                // - never in the 300 ms registration pass.
                if (!ReadablePtr(p, 4)) continue;
                const wchar_t* ws = (const wchar_t*)p;
                if (!LegitW(ws[0])) continue;
                if (!ContainsPrivateTagW((wchar_t*)ws, 64)) continue;
                size_t len = 0;
                bool ok = true;
                while (len < 64) {
                    wchar_t c = ws[len];
                    if (c == 0) break;
                    if (!LegitW(c)) { ok = false; break; }
                    len++;
                }
                if (!ok || len < 2 || ws[len] != 0) continue;
                tagFound = true;
                size_t copy = (len < maxChars - 1) ? len : (maxChars - 1);
                for (size_t k = 0; k < copy; k++) out[k] = ws[k];
                out[copy] = 0;
                break;
            }
        }
        if (!tagFound) {
            for (size_t i = 0x10; i + 3 <= scanEnd; i += 1) {
                const char* s = (const char*)(b + i);
                if (s[0] != 0x5B && s[0] != 0x28) continue;
                if (!LegitA(s[0])) continue;
                size_t len = 0;
                bool ok = true;
                while (len < 64) {
                    if (i + len >= scanEnd) { ok = false; break; }
                    char c = s[len];
                    if (c == 0) break;
                    if (!LegitA(c)) { ok = false; break; }
                    len++;
                }
                if (!ok || len < 2 || s[len] != 0) continue;
                if (!ContainsPrivateTagA(s, len + 1)) continue;
                tagFound = true;
                if (out[0] == 0) {
                    size_t cp = (len < maxChars - 1) ? len : (maxChars-1);
                    for (size_t k = 0; k < cp; k++)
                        out[k] = (unsigned char)s[k];
                    out[cp] = 0;
                }
                break;
            }
        }
    } __except (FaultFilter("enterprise_base tag scan",
                            GetExceptionInformation())) {
    }

    return tagFound;
}

static const char* Internal_GetBuildingObjectName(void* building)
{
    static __declspec(thread) char buf[128];
    buf[0] = 0;
    if (!TsmIsValidGamePtr(building)) return buf;
    if (!ReadablePtr(building, 0x400)) return buf;

    void* td = *(void**)((BYTE*)building + TSM_BLD_TYPEDESC);
    if (!TsmIsValidGamePtr(td)) return buf;

    SafeReadStr((const BYTE*)td + TSM_TD_NAME, buf, sizeof(buf));
    return buf;
}

static void FillLogBuildingName(void* building, wchar_t* out, size_t maxChars)
{
    if (!out || maxChars == 0) return;
    out[0] = L'\0';
    if (Internal_GetBuildingCustomName(building, out, maxChars) && out[0]) {
        return;
    }
    const char* obj = Internal_GetBuildingObjectName(building);
    if (obj && obj[0]) {
        size_t i = 0;
        for (; i + 1 < maxChars && obj[i]; ++i) {
            out[i] = (wchar_t)(unsigned char)obj[i];
        }
        out[i] = L'\0';
        return;
    }
    wcsncpy_s(out, maxChars, L"(unnamed)", _TRUNCATE);
}

static bool IsAutoPrivateType(int type)
{
    for (int i = 0; i < g_autoTypeCount; i++)
        if (g_autoTypes[i] == type) return true;
    return false;
}

static bool CheckBuildingHasTag(void* building)
{
    if (!TsmIsValidGamePtr(building)) return false;
    if (!ReadablePtr(building, TSM_BLD_MIN_READABLE)) return false;

    if (g_allPrivate) {
        int t = TsmBuildingType(building);
        if (t >= 0 && IsAutoPrivateType(t)) return true;
    }

    // Object name from the type descriptor (canonical single string).
    const char* objName = Internal_GetBuildingObjectName(building);
    if (objName[0] && ContainsPrivateTagA(objName, 128)) return true;

    // v8.5: the custom-name span scan is GONE from this 300 ms hot path.
    // v8.3 scanned per-offset with host calls (lag); v8.4 followed heap
    // pointers and dereferenced float-shaped garbage (14,707 AVs in one
    // session, aborting the scan for almost every building). Privatisation
    // is now the panel PRIV tile (EnterpriseWidgetProvider). What remains
    // is the cheap auto-type test and a literal tag in the mod's own
    // object name - one SafeReadStr, no scan.
    return false;
}

// ---------------------------------------------------------------- window
// The game's main window, enumerated once per process and re-validated
// cheaply. Mirrored here so the debug hotkeys can be foreground-gated with
// GetForegroundWindow() == GameWindow() on the sim thread without racing the
// ui_panels window query. Only a top-level window in the game process with a
// sizeable client area qualifies.
static HWND GameWindow(void)
{
    static HWND s_hwnd = nullptr;
    if (s_hwnd && IsWindow(s_hwnd)) return s_hwnd;
    s_hwnd = nullptr;
    EnumWindows([](HWND h, LPARAM ud) -> BOOL {
        HWND* out = (HWND*)ud;
        DWORD pid = 0;
        GetWindowThreadProcessId(h, &pid);
        if (pid != GetCurrentProcessId() || !IsWindowVisible(h)) return TRUE;
        RECT rc;
        if (!GetClientRect(h, &rc)) return TRUE;
        if (rc.right - rc.left < 200 || rc.bottom - rc.top < 200) return TRUE;
        if (!*out) {
            *out = h;
            return TRUE;
        }
        RECT best;
        if (GetClientRect(*out, &best) &&
            (rc.right - rc.left) * (rc.bottom - rc.top) >
                (best.right - best.left) * (best.bottom - best.top))
            *out = h;
        return TRUE;
    }, (LPARAM)&s_hwnd);
    return s_hwnd;
}

#define TSM_TYPE_RESIDENTIAL 2

// Forward declaration of a fault-guarded POD read defined lower in the file;
// it must precede the debug wallet-drain helper that uses it.
static bool ReadResidentSpan(void* b, void*** outBegin, void*** outEnd);

// ---------------------------------------------------------------- wallet drain
// Drain the first 30 citizen wallets to 0.50 RUB for the eviction stress
// test. The currency service exposes per-person mutation only through a
// person pointer, and forEachWallet hands out citizenId + balance, so we
// enumerate the first 30 wallets (as the directive requires), resolve the
// matching resident persons out of private residential buildings, and drain
// each matched wallet with chargeCitizen(person, balance - 0.50f). A matched
// citizen whose wallet is already <= 0.50 is skipped.
static void DrainSampleWallets(void)
{
    if (!g_currency || !g_currency->forEachWallet) {
        Logf("enterprise_base  [DEBUG] no currency iface - skip stress test");
        return;
    }

    enum { kSample = 30 };
    struct { int n; uint64_t id[kSample]; float bal[kSample]; } snap = {0};

    g_currency->forEachWallet([](uint64_t cid, float bal, void* ud) {
        auto* s = (decltype(snap)*)ud;
        if (s->n < kSample) {
            s->id[s->n] = cid;
            s->bal[s->n] = bal;
            s->n++;
        }
        return 1;
    }, &snap);

    // Collect resident persons from private residential enterprises.
    std::vector<void*> persons;
    persons.reserve(g_companies.size() * 16);
    EnterCriticalSection(&g_lock);
    for (auto& kv : g_companies) {
        void* b = kv.first;
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
            continue;
        if (TsmBuildingType(b) != TSM_TYPE_RESIDENTIAL) continue;
        if (!kv.second.isPrivate) continue;

        void** rBegin = nullptr;
        void** rEnd = nullptr;
        if (ReadResidentSpan(b, &rBegin, &rEnd)) {
            size_t n = (size_t)(rEnd - rBegin);
            if (n > 1000) n = 1000;
            persons.insert(persons.end(), rBegin, rBegin + (size_t)n);
        }
    }
    LeaveCriticalSection(&g_lock);

    int drained = 0;
    for (void* person : persons) {
        if (drained >= kSample) break;
        uint64_t id = TsmCitizenID(person);
        for (int i = 0; i < snap.n; i++) {
            if (snap.id[i] == id && snap.bal[i] > 0.5f) {
                float drain = snap.bal[i] - 0.5f;
                g_currency->chargeCitizen(person, drain);
                drained++;
                break;
            }
        }
    }

    Logf("enterprise_base  [DEBUG] drained %d sample citizen wallets "
         "to 0.50 RUB for eviction stress test", drained);
}

// ---------------------------------------------------------------- accounting

static bool IsRevenue(TsmCapitalCategory c)
{
    // v9.18: WHOLESALE is revenue too.
    //
    // A distribution office sells TRANSPORT, not stock, and private_logistics
    // credits its freight with TSM_CAP_WHOLESALE (cat=1). Until now a positive
    // delta in that category fell through both branches of ApplyCategory - not
    // revenue, and delta > 0 so not a cost either - so the DO's freight never
    // reached monthlyGrossSales. Its fuel DID: FOREIGN arrives as a negative
    // delta and is booked as a wholesale cost.
    //
    // The panel therefore showed a net profit of -fuel while the capital rose
    // by freight - fuel. Exactly the contradiction a session-8 player reported:
    // "it only shows the negative and I can see the capital increase".
    //
    // Booking the credit closes it: net profit = freight + sales - (fuel +
    // wages + taxes), which is what the account has been doing all along.
    return c == TSM_CAP_RETAIL || c == TSM_CAP_RENT || c == TSM_CAP_PRODUCTION
        || c == TSM_CAP_WHOLESALE;
}

static void ApplyCategory(Enterprise& e, double delta,
                          TsmCapitalCategory category)
{
    if (delta > 0.0 && IsRevenue(category)) {
        e.monthlyGrossSales += delta;
        e.periodGrossSales  += delta;
    } else if (delta < 0.0) {
        double cost = -delta;
        if (category == TSM_CAP_WHOLESALE) {
            e.monthlyWholesaleCosts += cost;
            e.periodWholesaleCosts  += cost;
        } else if (category == TSM_CAP_FOREIGN) {
            // Protect operating working capital used for foreign inventory imports
            e.monthlyWholesaleCosts += cost;
            e.periodForeignCosts    += cost;
        } else if (category == TSM_CAP_WAGE) {
            e.monthlyWagesPaid += cost;
            e.periodWagesPaid  += cost;
        } else if (category == TSM_CAP_TAX) {
            e.monthlyTaxesPaid += cost;
        }
    }
}

// ---------------------------------------------------------------- capital ops

static double GetCapitalImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return 0.0;
    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    double v = (it != g_companies.end()) ? it->second.capital : 0.0;
    LeaveCriticalSection(&g_lock);
    return v;
}

// v9.6: this is the real operation. v9.7 renamed it ApplyCapital, because
// TryModifyCapital is now the tracing wrapper further down. The arithmetic
// in this function is unchanged from v9.6.
//
// ModifyCapitalImpl stays as the void façade kept for the published API
// table (TsmEnterpriseBaseApi's modifyCompanyCapital is `void (*)(...)`,
// and an ABI is not something to churn in a hotfix).
//
// Why it returns a status: every refusal below used to be a silent
// `return`. On the retail path the caller had ALREADY debited the
// citizen's wallet by the time it called this, so a refused credit
// destroyed money with no trace anywhere - the exact opposite face of the
// reported phantom income, and neither is possible once the caller can see
// the refusal and roll back.
//
// 1 = the delta landed. 0 = it did not, and the caller owes the money back.
// ---------------------------------------------------------------- v9.9 trace state
//
// g_lastCap: the last value this plugin WROTE for a building. Any divergence
// between it and what is actually in the map means somebody else wrote the
// field. That is the single invariant the whole P2 question reduces to: money
// should only exist because a traced operation put it there.
static std::unordered_map<void*, double> g_lastCap;

// The watch: armed by a refusal, live for g_watchMs, scoped to one building.
static void* g_watchShop  = nullptr;
static DWORD g_watchUntil = 0;
static double g_watchCap0 = 0.0;
static double g_watchSum  = 0.0;
static int    g_watchN    = 0;
static int    g_watchId   = 0;
static int    g_watchMs   = 4000;

static bool WatchLive(void* building)
{
    return g_watchShop && building == g_watchShop &&
           (long)(g_watchUntil - GetTickCount()) > 0;
}

// Called from the wrapper before a movement, and from the tick once a second.
// Reports, at most once a second, any capital that is not where we left it.
static void AuditCapital(void* building, const char* when)
{
    if (!g_probe || !TsmIsValidGamePtr(building)) return;

    auto it = g_lastCap.find(building);
    if (it == g_lastCap.end()) return;              // first sighting: no baseline

    double actual = GetCapitalImpl(building);
    if (actual - it->second > 0.005 || it->second - actual > 0.005) {
        static DWORD lastGap = 0;
        DWORD now = GetTickCount();
        if (now - lastGap >= 1000) {
            lastGap = now;
            Logf("enterprise_base  [AUDIT GAP] %p at %s: expected %.2f, found "
                 "%.2f (%+.2f) - capital moved outside the traced path",
                 building, when, it->second, actual, actual - it->second);
        }
        it->second = actual;                        // re-baseline
    }
}

static int ApplyCapital(void* building, double delta,
                        TsmCapitalCategory category)
{
    if (!TsmIsValidGamePtr(building)) return 0;
    // NaN would poison capital; +-inf would too (and would slip past the
    // equality test the old guard used).
    if (!std::isfinite(delta)) return 0;
    if (delta == 0.0) return 1;              // nothing to apply is success
    // v8.0 (Task 3): a ruin never takes capital movements - no auto-
    // registration, no silent re-privatisation.
    if (TsmIsDecayed(building)) return 0;

    EnterCriticalSection(&g_lock);

    auto it = g_companies.find(building);
    if (it == g_companies.end() || !it->second.isPrivate) {
        // Prevent silent privatization: only register if the building truly carries the private tag
        if (!CheckBuildingHasTag(building)) {
            LeaveCriticalSection(&g_lock);
            return 0;
        }

        Enterprise& e = g_companies[building];
        e.isPrivate = true;
        e.markup    = g_defaultMarkup;
        if (e.capital == 0.0) e.capital = g_startingCapital;
        double next = e.capital + delta;
        if (!std::isfinite(next)) {
            LeaveCriticalSection(&g_lock);
            return 0;
        }
        if (next < -1.0e12) next = -1.0e12;
        if (next >  1.0e12) next =  1.0e12;
        e.capital = next;
        ApplyCategory(e, delta, category);
        RebuildOrder();
        LeaveCriticalSection(&g_lock);
        return 1;
    }

    Enterprise& e = it->second;
    double next = e.capital + delta;
    if (!std::isfinite(next)) {
        LeaveCriticalSection(&g_lock);
        return 0;
    }
    if (next < -1.0e12) next = -1.0e12;
    if (next >  1.0e12) next =  1.0e12;
    e.capital = next;
    if (delta > 0.0) e.stalledOnDust = false;
    ApplyCategory(e, delta, category);

    LeaveCriticalSection(&g_lock);
    return 1;
}

// v9.7: the ONLY capital entry point. Reads the balance either side of the
// operation and logs the movement with the caller's name attached. Everything
// that moves enterprise money - this plugin's own rent/wage/retail paths and
// every cross-plugin call through the published table - goes through here, so
// there is exactly one place to look when a balance moves on its own.
//
// ApplyCapital is the untouched v9.6 body: this wrapper adds a read before,
// a read after, and a log line. It cannot change whether a movement lands.
static int TryModifyCapital(void* building, double delta,
                            TsmCapitalCategory category, const char* by)
{
    AuditCapital(building, "before a movement");

    double before = GetCapitalImpl(building);
    bool   watch  = WatchLive(building);

    int    ok     = ApplyCapital(building, delta, category);
    double after  = ok ? GetCapitalImpl(building) : before;

    g_lastCap[building] = after;

    if (g_probe) {
        if (watch) {
            // Everything that touches the watched shop, uncapped: this is the
            // few seconds after a citizen was refused, and the question is
            // who - if anyone - moved the shop's money.
            g_watchN++;
            g_watchSum += (after - before);
            Logf("enterprise_base  [WATCH %d CAP] %s %p %+.3f cat=%d by=%s  "
                 "%.2f -> %.2f  (running %+.3f over %d event(s))",
                 g_watchId, ok ? "ok " : "REF", building, delta, (int)category,
                 by ? by : "?", before, after, g_watchSum, g_watchN);
        } else {
            // v9.12: log retail credits too. v9.7 withheld them because they
            // were "the flood", and the cost was every rouble a shop earns:
            // counter sales AND private_shops service fees both arrive as
            // cat=0, so a session's ledger showed a shop paying wages, taxes
            // and imports and receiving nothing. Profit cannot be computed
            // from half a ledger. A busy shop writes a few hundred lines a
            // session, not a flood; the budget is raised to match.
            static int logged;
            if (++logged <= 20000)
                Logf("enterprise_base  [CAP] %s %p %+.3f cat=%d by=%s  "
                     "%.2f -> %.2f",
                     ok ? "ok " : "REF", building, delta, (int)category,
                     by ? by : "?", before, after);
        }
    }
    return ok;
}

// v9.6 façade - signature unchanged, so the published TsmEnterpriseBaseApi
// table is untouched. Calls arriving through the API are tagged api.modify.
static void ModifyCapitalImpl(void* building, double delta,
                              TsmCapitalCategory category)
{
    (void)TryModifyCapital(building, delta, category, "api.modify");
}

static int IsSolventImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return 1;
    if (g_allowDebt) return 1;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    int solvent = (it == g_companies.end()) ? 1 : (it->second.capital > 0.0 ? 1 : 0);
    LeaveCriticalSection(&g_lock);
    return solvent;
}

// v9.11: affordability, not solvency. IsSolventImpl answers "does this
// enterprise have any capital at all", which is the right question for the
// panel and for the API, and the wrong one for payroll: a shop holding
// 407.34 passed it and then paid a 1090.08 wage, landing at -682.75. The
// 682.75 was minted - the citizen's wallet credit is real, the ledger was
// allowed to go negative against its will.
//
// allow_debt is the explicit opt-in to overdraft; with it on, this returns 1
// and v9.10's behaviour is exactly preserved. POD only, no allocation.
static int CanPayWage(void* building, double wage)
{
    if (g_allowDebt) return 1;
    if (!(wage > 0.0) || !(wage == wage)) return 0;
    if (!TsmIsValidGamePtr(building)) return 0;
    return (GetCapitalImpl(building) + 1e-6 >= wage) ? 1 : 0;
}

static float GetMarkupImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return g_defaultMarkup;
    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    float m = (it != g_companies.end()) ? it->second.markup : g_defaultMarkup;
    LeaveCriticalSection(&g_lock);
    return m;
}

static void SetMarkupImpl(void* building, float m)
{
    if (!TsmIsValidGamePtr(building)) return;
    // Same bound the save/load path enforces, so a value accepted here
    // always survives a round-trip instead of reverting to the default.
    if (!(m > 0.0f && m == m)) return;
    if (m > 100.0f) m = 100.0f;
    EnterCriticalSection(&g_lock);
    g_companies[building].markup = m;
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- accrual access

static void GetAccrualsImpl(void* building, double* outVat, double* outProfit)
{
    double v = 0.0, p = 0.0;
    if (TsmIsValidGamePtr(building)) {
        EnterCriticalSection(&g_lock);
        auto it = g_companies.find(building);
        if (it != g_companies.end()) {
            v = it->second.accruedVatLiability;
            p = it->second.accruedProfitTaxLiability;
        }
        LeaveCriticalSection(&g_lock);
    }
    if (outVat)    *outVat    = v;
    if (outProfit) *outProfit = p;
}

static void AddAccrualImpl(void* building, TsmAccrualKind kind, double amount)
{
    if (!TsmIsValidGamePtr(building)) return;
    // +inf passes an equality-only NaN test; reject both non-finite ends so
    // the liability and period totals can never become non-finite.
    if (!(amount > 0.0) || !(amount <= 1.0e300)) return;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    if (it != g_companies.end()) {
        Enterprise& e = it->second;
        if (kind == TSM_ACCRUAL_VAT) {
            e.accruedVatLiability += amount;
            e.periodVatAccrued    += amount;
        } else if (kind == TSM_ACCRUAL_PROFIT) {
            e.accruedProfitTaxLiability += amount;
        }
    }
    LeaveCriticalSection(&g_lock);
}

static void ClearAccrualImpl(void* building, TsmAccrualKind kind)
{
    if (!TsmIsValidGamePtr(building)) return;
    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    if (it != g_companies.end()) {
        Enterprise& e = it->second;
        if (kind == TSM_ACCRUAL_VAT)
            e.accruedVatLiability = 0.0;
        else if (kind == TSM_ACCRUAL_PROFIT)
            e.accruedProfitTaxLiability = 0.0;
    }
    LeaveCriticalSection(&g_lock);
}

static void GetPeriodTotalsImpl(void* building,
                                double* outGS, double* outVatAccrued,
                                double* outWS, double* outWages,
                                double* outForeign)
{
    double gs = 0, va = 0, ws = 0, wg = 0, fo = 0;
    if (TsmIsValidGamePtr(building)) {
        EnterCriticalSection(&g_lock);
        auto it = g_companies.find(building);
        if (it != g_companies.end()) {
            gs = it->second.periodGrossSales;
            va = it->second.periodVatAccrued;
            ws = it->second.periodWholesaleCosts;
            wg = it->second.periodWagesPaid;
            fo = it->second.periodForeignCosts;
        }
        LeaveCriticalSection(&g_lock);
    }
    if (outGS)        *outGS        = gs;
    if (outVatAccrued) *outVatAccrued = va;
    if (outWS)        *outWS        = ws;
    if (outWages)     *outWages     = wg;
    if (outForeign)   *outForeign   = fo;
}

static void ClearPeriodTotalsImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return;
    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    if (it != g_companies.end()) {
        Enterprise& e = it->second;
        e.periodGrossSales     = 0.0;
        e.periodVatAccrued     = 0.0;
        e.periodWholesaleCosts = 0.0;
        e.periodWagesPaid      = 0.0;
        e.periodForeignCosts   = 0.0;
    }
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- residential rent (v5.4)
//
// Private residential buildings earn rent per resident per month, charged
// via currency's partial wallet take so a broke tenant pays what they can.
// Runs at the month boundary BEFORE SiphonToPool, so the rent counts as
// operating profit this same month. Snapshots the building list first so
// no lock is held across currency calls.

// SEH helpers live in their own POD-only functions: C2712 forbids
// __try in a function that needs C++ unwinding (std::vector), so the
// fault-guarded reads are isolated here where every local is a raw
// pointer or integer.
static bool ReadResidentSpan(void* b, void*** outBegin, void*** outEnd)
{
    void** rBegin = nullptr;
    void** rEnd = nullptr;
    __try {
        void** begin = *(void***)((BYTE*)b + TSM_BLD_RESIDENTS_BEGIN);
        void** end   = *(void***)((BYTE*)b + TSM_BLD_RESIDENTS_END);
        if (begin && end && end > begin && TsmIsValidGamePtr(begin) &&
            ReadablePtr(begin, sizeof(void*))) {
            rBegin = begin;
            rEnd = end;
        }
    } __except (FaultFilter("enterprise_base residential rent scan",
                            GetExceptionInformation())) {
        rBegin = nullptr;
        rEnd = nullptr;
    }
    *outBegin = rBegin;
    *outEnd = rEnd;
    return (rBegin && rEnd);
}

static size_t CopyResidentPtrs(void** rBegin, size_t n,
                               void** out, size_t cap)
{
    size_t count = 0;
    __try {
        for (size_t k = 0; k < n; k++) {
            void* person = rBegin[k];
            if (!TsmIsValidGamePtr(person)) continue;
            if (!ReadablePtr(person, TSM_PERSON_SIZE)) continue;
            if (count < cap) out[count++] = person;
        }
    } __except (FaultFilter("enterprise_base residential rent copy",
                            GetExceptionInformation())) {
    }
    return count;
}

// v8.2 (Task 1): park-cap dwelling count. Returns storage-0's declared
// capacity (the INI RESOURCE_TRANSPORT_PASSANGER figure = flats) or a
// negative sentinel when unreadable. Sibling-proven layout (private_shops
// ForEachSlot): the storage vector is INLINE structs at stride
// TSM_STORAGE_STRIDE, so storage #0 starts AT storBegin - no extra
// dereference. POD-only: C2712 forbids __try beside C++ unwinding.
static float ReadFlatCapacityRaw(void* b)
{
    float cap = -1.0f;
    __try {
        void** storBegin = *(void***)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
        void** storEnd   = *(void***)((BYTE*)b + TSM_BLD_STORAGES_END);
        if (storBegin && storEnd && storEnd > storBegin &&
            TsmIsValidGamePtr(storBegin) &&
            ReadablePtr(storBegin, TSM_STORAGE_STRIDE)) {
            size_t storBytes = (size_t)((BYTE*)storEnd - (BYTE*)storBegin);
            if (storBytes >= TSM_STORAGE_STRIDE &&
                !(storBytes % TSM_STORAGE_STRIDE)) {
                float v = *(float*)((BYTE*)storBegin + TSM_ST_CAPACITY);
                if (v == v && v > 0.0f && v < 1000000.0f) cap = v;
            }
        }
    } __except (FaultFilter("enterprise_base flat capacity",
                            GetExceptionInformation())) {
        cap = -1.0f;
    }
    return cap;
}

// v8.3 (Order 4): SEH-guarded native rent-level read (0..1 float at
// TSM_BLD_RENT_LEVEL, the player slider FUN_14073E950 writes as
// slider/100). Returns dflt (neutral 0.5) when unreadable or out of
// range. POD-only.
static float ReadRentLevelRaw(void* b, float dflt)
{
    float v = dflt;
    __try {
        if (TsmIsValidGamePtr(b) &&
            ReadablePtr((BYTE*)b + TSM_BLD_RENT_LEVEL, sizeof(float))) {
            float r = *(float*)((BYTE*)b + TSM_BLD_RENT_LEVEL);
            if (r == r && r >= 0.0f && r <= 1.0f) v = r;
        }
    } __except (FaultFilter("enterprise_base rent level",
                            GetExceptionInformation())) {
        v = dflt;
    }
    return v;
}

// v8.2 (Task 1): shared rent-rate math. The park cap scales the
// attractiveness-driven premium by the dwelling count: a flatCap >=
// PARKCAP_FLAT_REF charges the full multiplier, a tiny shack collapses
// toward the slum floor (min + (max-min) x attr x flatCap/REF). An
// unreadable flatCap (sentinel < 0) or the disabled toggle leaves the
// v8.0 rate untouched. Pure arithmetic - never faults.
// v8.3 (Order 4): multiplies by the verified native rent slider as
// 0.75+0.5xlevel (neutral 1.0 at the 0.5 default), so the player's own
// per-building rent % shapes private rent while an unread building keeps
// the v8.2 rate exactly.
static double RentRateFor(float flatCap, float attr, float rentLevel)
{
    float mult = g_rentMinMult + (g_rentMaxMult - g_rentMinMult) * attr;
    if (g_parkCapEnabled && flatCap > 0.0f && flatCap < PARKCAP_FLAT_REF) {
        float scaled = g_rentMinMult +
            (g_rentMaxMult - g_rentMinMult) * attr * (flatCap / PARKCAP_FLAT_REF);
        if (scaled < g_rentMinMult) scaled = g_rentMinMult;
        if (scaled > mult) scaled = mult;
        mult = scaled;
    }
    float rentFactor = 1.0f;
    if (rentLevel >= 0.0f && rentLevel <= 1.0f)
        rentFactor = 0.75f + 0.5f * rentLevel;
    return g_residentialRent * (double)mult * (double)rentFactor;
}

// v8.2 (Task 1): SEH-guarded person float read (wealth 0xE4 / happiness
// 0xE8). Returns dflt when unreadable or non-finite. POD-only.
static float ReadPersonFloatRaw(void* person, size_t off, float dflt)
{
    float v = dflt;
    __try {
        if (TsmIsValidGamePtr(person) &&
            ReadablePtr((BYTE*)person + off, sizeof(float))) {
            float r = *(float*)((BYTE*)person + off);
            if (r == r) v = r;
        }
    } __except (FaultFilter("enterprise_base person float",
                            GetExceptionInformation())) {
        v = dflt;
    }
    return v;
}

// v8.2 (Task 1): SEH-guarded happiness write. Clamps at zero - a forced
// move never bankrupts happiness below the floor. POD-only.
static void DentHappinessRaw(void* person, float dent)
{
    __try {
        if (TsmIsValidGamePtr(person) &&
            ReadablePtr((BYTE*)person + TSM_PERSON_HAPPINESS, sizeof(float))) {
            float h = *(float*)((BYTE*)person + TSM_PERSON_HAPPINESS);
            if (!(h == h)) h = 0.5f;
            h -= dent;
            if (h < 0.0f) h = 0.0f;
            if (h > 1.0f) h = 1.0f;
            *(float*)((BYTE*)person + TSM_PERSON_HAPPINESS) = h;
        }
    } __except (FaultFilter("enterprise_base happiness dent",
                            GetExceptionInformation())) {
    }
}

// v8.2 (Task 1): SEH-isolated native relocation call. Returns 1 on a clean
// return. POD-only: the function pointer arithmetic and the __try live
// where no C++ object needs unwinding.
static int InvokeRelocateRaw(void* game, void* person, void* dest)
{
    if (!TsmCheckExeBuild(H, "enterprise_base")) return 0;
    const unsigned char* code = TsmCallableCodePtr(H, TSM_F_RELOCATE, "relocate_person");
    if (!code) return 0;

    int ok = 0;
    __try {
        typedef void (__fastcall *t_RelocateCitizen)(void*, void*, void*);
        t_RelocateCitizen fn = (t_RelocateCitizen)code;
        if (TsmIsValidGamePtr(game) && TsmIsValidGamePtr(person) &&
            TsmIsValidGamePtr(dest))
            fn(game, person, dest);
        else
            return 0;
        ok = 1;
    } __except (FaultFilter("enterprise_base native relocate",
                            GetExceptionInformation())) {
        ok = 0;
    }
    return ok;
}

// v8.2 (Task 1): guarded resident headcount for the vacancy test. Counts
// only fully-valid person pointers so a stale slot never fakes a full
// flat. POD-only.
static size_t CountValidResidentsRaw(void** rBegin, size_t n)
{
    size_t c = 0;
    __try {
        for (size_t k = 0; k < n; k++) {
            void* p = rBegin[k];
            if (!TsmIsValidGamePtr(p)) continue;
            if (!ReadablePtr(p, TSM_PERSON_SIZE)) continue;
            c++;
        }
    } __except (FaultFilter("enterprise_base resident count",
                            GetExceptionInformation())) {
    }
    return c;
}

// v8.2 (Task 1): SEH-guarded building-type read. TsmBuildingType walks the
// typedesc without a readability probe, so the town-wide candidate scan
// uses this instead. POD-only.
static int ReadBuildingTypeRaw(void* b)
{
    int t = -1;
    __try {
        if (TsmIsValidGamePtr(b) &&
            ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*))) {
            void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
            if (td && TsmIsValidGamePtr(td) &&
                ReadablePtr((BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
                t = *(int*)((BYTE*)td + TSM_TD_TYPE_NUM);
        }
    } __except (FaultFilter("enterprise_base building type",
                            GetExceptionInformation())) {
        t = -1;
    }
    return t;
}

// v8.2 (Task 1): a staged eviction. dest == nullptr means "no vacant flat
// anywhere" - the citizen is retained in place, never moved into nullptr.
struct EvictStage {
    void*  person;
    void*  dest;
    void*  from;
    float  wealth;
    double rent;
    int    reason;   // v9.1: 0 = rent burden, 1 = short pay, 2 = destitute
};

// v8.2 (Task 1): SEH-guarded town building-vector read. POD-only: the
// caller keeps its C++ locals (unordered_set) clear of any __try frame
// (C2712), so the fault-guarded pointer walk lives here.
static int ReadBuildingArrayRaw(void* game, void*** outBegin, void*** outEnd)
{
    void** arrBegin = nullptr;
    void** arrEnd = nullptr;
    __try {
        if (!TsmIsValidGamePtr(game)) return 0;
        if (!ReadablePtr((BYTE*)game + TSM_GO_BLD_VEC_BEGIN, 16)) return 0;
        arrBegin = *(void***)((BYTE*)game + TSM_GO_BLD_VEC_BEGIN);
        arrEnd   = *(void***)((BYTE*)game + TSM_GO_BLD_VEC_END);
    } __except (FaultFilter("enterprise_base eviction scan",
                            GetExceptionInformation())) {
        return 0;
    }
    if (!arrBegin || !arrEnd || arrEnd <= arrBegin) return 0;
    size_t n = (size_t)(arrEnd - arrBegin);
    if (n == 0 || n > 25000) return 0;
    *outBegin = arrBegin;
    *outEnd = arrEnd;
    return 1;
}

// v8.2 (Task 1): SEH-guarded building-array element read. POD-only.
static void* ReadBuildingAtRaw(void** arrBegin, size_t i)
{
    void* b = nullptr;
    __try {
        b = arrBegin[i];
    } __except (FaultFilter("enterprise_base eviction walk",
                            GetExceptionInformation())) {
        b = nullptr;
    }
    return b;
}

// v8.2 (Task 1): relocation candidate search. Scans the town building
// vector (game+0x11B08..0x11B10) for a vacant type-2 flat: finished,
// standing, undecayed, not the current home, with residents < flatCap.
// The poor (wealth < 0.40) prefer state or cheap private flats
// (attr < 0.35); the wealthy (>= 0.70) prefer high-attractiveness private
// flats (attr >= 0.60); everyone else takes the cheapest fit. Pointer
// order breaks ties so the result is deterministic. Returns nullptr when
// no vacancy exists - the caller must NEVER relocate into nullptr
// (vanilla FUN_140835D70 despawns a citizen evicted without a
// destination). The register peek is a brief lock grab; callers never
// hold g_lock here. Every engine read goes through the SEH helpers.
void* FindRelocationCandidate(void* game, void* currentBld, float tenantWealth)
{
    if (!TsmIsValidGamePtr(game) || !TsmIsValidGamePtr(currentBld))
        return nullptr;
    float wealth = tenantWealth;
    if (!(wealth == wealth)) wealth = 0.5f;
    int poor = (wealth < 0.40f);
    int rich = (wealth >= 0.70f);

    void** arrBegin = nullptr;
    void** arrEnd = nullptr;
    if (!ReadBuildingArrayRaw(game, &arrBegin, &arrEnd)) return nullptr;
    size_t n = (size_t)(arrEnd - arrBegin);

    // One snapshot of the private register under a single grab: the scan
    // below then runs lock-free (per-candidate grabs would churn the lock
    // thousands of times on a large map). Callers never hold g_lock here,
    // so this cannot nest.
    std::unordered_set<void*> privateSet;
    EnterCriticalSection(&g_lock);
    privateSet.reserve(g_companies.size());
    for (std::unordered_map<void*, Enterprise>::const_iterator it =
             g_companies.begin();
         it != g_companies.end(); ++it)
        if (it->second.isPrivate) privateSet.insert(it->first);
    LeaveCriticalSection(&g_lock);

    void*  bestPref = nullptr;
    double rentPref = 0.0;
    float  attrPref = 0.0f;
    void*  bestAny  = nullptr;
    double rentAny  = 0.0;

    for (size_t i = 0; i < n; i++) {
        void* b = ReadBuildingAtRaw(arrBegin, i);
        if (!b) continue;
        if (b == currentBld) continue;
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (ReadBuildingTypeRaw(b) != TSM_TYPE_RESIDENTIAL) continue;
        if (!TsmIsFinished(b)) continue;
        if (TsmIsDemolished(b)) continue;
        if (TsmIsDecayed(b)) continue;

        float cap = ReadFlatCapacityRaw(b);
        if (!(cap > 0.0f)) continue;

        void** rb = nullptr;
        void** re = nullptr;
        if (!ReadResidentSpan(b, &rb, &re)) continue;
        size_t rn = (size_t)(re - rb);
        if (rn > 100000) continue;
        size_t occ = CountValidResidentsRaw(rb, (rn > 2000) ? 2000 : rn);
        if (!((double)occ < (double)cap)) continue;

        float attr = TsmAttractiveness(b);
        // v8.3: same native rent-level grounding as the charge path.
        float rl = ReadRentLevelRaw(b, 0.5f);
        double rent = RentRateFor(cap, attr, rl);

        int priv = (privateSet.find(b) != privateSet.end()) ? 1 : 0;

        int pref;
        if (poor)
            pref = (!priv) || (attr < 0.35f);
        else if (rich)
            pref = (priv && attr >= 0.60f);
        else
            pref = 1;

        if (pref) {
            int take = 0;
            if (!bestPref) {
                take = 1;
            } else if (rich) {
                if (attr > attrPref) take = 1;
                else if (attr == attrPref &&
                         (rent < rentPref ||
                          (rent == rentPref && b < bestPref))) take = 1;
            } else if (rent < rentPref ||
                       (rent == rentPref && b < bestPref)) {
                take = 1;
            }
            if (take) {
                bestPref = b;
                rentPref = rent;
                attrPref = attr;
            }
        }

        if (!bestAny || rent < rentAny ||
            (rent == rentAny && b < bestAny)) {
            bestAny = b;
            rentAny = rent;
        }
    }

    return bestPref ? bestPref : bestAny;
}

// Defined with the retail refund path below; declared here because a
// refused landlord credit must hand the tenants' money back.
static void RefundRetailCharge(void* person, float amount);

static void CollectResidentialRent(void)
{
    if (!(g_residentialRent > 0.0)) return;
    if (!g_currency) return;

    // v9.1 (Task 7): one day of staple bread - native Pfood x
    // destitute_bread_tons, in the ACTIVE currency (getPrice resolves the
    // currency leg). v9.20: no market multiplier. 0 disables the stage.
    float destituteDay = 0.0f;
    if (g_destituteEnabled && g_pricing &&
        g_pricing->getFood && g_pricing->getPrice) {
        int cur = TsmCurrencyActive(g_currency);
        void* food = g_pricing->getFood();
        float pFood = g_pricing->getPrice(food, cur);
        destituteDay = pFood * g_destituteBreadTons;
        if (!(destituteDay == destituteDay) || destituteDay <= 0.0f)
            destituteDay = 0.0f;
    }

    std::vector<void*> homes;
    EnterCriticalSection(&g_lock);
    homes.reserve(g_companies.size());
    for (auto& kv : g_companies)
        if (kv.second.isPrivate)
            homes.push_back(kv.first);
    LeaveCriticalSection(&g_lock);

    double totalRent = 0.0;
    int buildings = 0, tenants = 0;

    std::vector<void*> residents;
    residents.reserve(256);

    // Per-tenant amounts actually taken this building, so a refused
    // landlord credit can be reversed instead of destroying the money.
    struct RentTake { void* person; float amount; };
    std::vector<RentTake> rentTakes;
    rentTakes.reserve(64);

    // v8.2 (Task 1): staged relocations. The burden test runs inline, but
    // native FUN_14083CA50 must NEVER run under g_lock (it walks the same
    // building vectors and tenant lists the register guards), so each
    // staged household moves after the loop with the lock released.
    std::vector<EvictStage> plan;
    std::unordered_set<void*> staged;

    for (void* b : homes) {
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;
        if (TsmBuildingType(b) != TSM_TYPE_RESIDENTIAL) continue;
        EnterCriticalSection(&g_lock);
        auto it = g_companies.find(b);
        bool priv = (it != g_companies.end() && it->second.isPrivate);
        LeaveCriticalSection(&g_lock);
        if (!priv) continue;

        // v8.0 (Task 1): one attractiveness read per building (same for
        // every tenant in it), guarded inside the helper: bld+0x11B0 is
        // past the 0xC00 floor, unreadable caps at the 0.50 fallback.
        // v8.2: the dwelling count caps the premium so a shack in a park
        // cannot charge penthouse rent; unreadable capacity keeps v8.0.
        // v8.3 (Order 4): the verified native rent slider (bld+0x13E0,
        // FUN_14073E950) multiplies as 0.75+0.5xlevel, neutral at default.
        float attr = TsmAttractiveness(b);
        float flatCap = ReadFlatCapacityRaw(b);
        float rentLevel = ReadRentLevelRaw(b, 0.5f);
        double tenantRent = RentRateFor(flatCap, attr, rentLevel);

        void** rBegin = nullptr;
        void** rEnd = nullptr;
        if (!ReadResidentSpan(b, &rBegin, &rEnd)) continue;

        size_t n = (size_t)(rEnd - rBegin);
        if (n > 1000) n = 1000;
        if (residents.capacity() < n) residents.reserve(n);
        residents.resize(n);
        size_t got = CopyResidentPtrs(rBegin, n, residents.data(), n);
        residents.resize(got);

        double buildingRent = 0.0;
        rentTakes.clear();
        for (void* person : residents) {
            // v8.2 (Task 1): snapshot ability-to-pay BEFORE the wallet take.
            // A post-charge read would compare the drained balance and flag
            // nearly every full payer as burdened.
            float wallet = 0.0f;
            if (g_currency->getWalletBalance)
                wallet = g_currency->getWalletBalance(person);
            if (!(wallet == wallet) || wallet < 0.0f) wallet = 0.0f;
            float wealth =
                ReadPersonFloatRaw(person, TSM_PERSON_WEALTH, 0.5f);

            float taken = g_currency->chargeCitizenPartial(
                person, (float)tenantRent);
            if (taken > 0.0f) {
                buildingRent += (double)taken;
                tenants++;
                rentTakes.push_back({person, taken});
            }

            // v8.2 (Task 1): burden test - rent exceeds the threshold share
            // of the wallet, or the partial take covered < 95% of the rent.
            // v9.1 (Task 7): destitute stage - the PRE-CHARGE wallet cannot
            // cover one day of staple bread, so the resident is a
            // relocation candidate regardless of the rent outcome (they may
            // have paid in full and still be starving).
            if (!g_evictionEnabled) continue;
            int destitute = (destituteDay > 0.0f && wallet < destituteDay)
                          ? 1 : 0;
            if (!(tenantRent > 0.0) && !destitute) continue;
            int burdened = (wallet > 0.0f &&
                            tenantRent > (double)g_rentBurdenThreshold *
                                         (double)wallet);
            int shortPaid = (taken < (float)(tenantRent * 0.95));
            if (!(burdened || shortPaid || destitute)) continue;
            if (staged.find(person) != staged.end()) continue;
            staged.insert(person);

            // v8.2 (Task 1): wealth-band search happens INLINE (no lock is
            // held here - the homes snapshot owns this loop), while the
            // native move itself stays staged: the engine mutates the same
            // vectors the relocation walks, so it fires after the loop
            // with nothing held.
            void* game = TsmGameObject(g_exeBase);
            void* dest = nullptr;
            if (TsmIsValidGamePtr(game))
                dest = FindRelocationCandidate(game, b, wealth);
            EvictStage st;
            st.person = person;
            st.dest   = dest;
            st.from   = b;
            st.wealth = wealth;
            st.rent   = tenantRent;
            st.reason = destitute ? 2 : (burdened ? 0 : 1);
            plan.push_back(st);
        }

        if (buildingRent > 0.0) {
            if (TryModifyCapital(b, buildingRent, TSM_CAP_RENT, "rent")) {
                totalRent += buildingRent;
                buildings++;
            } else {
                // The landlord refused the credit (decayed or lost tag).
                // The tenants were already charged, so hand every take
                // back - the retail path has the same rule.
                for (const RentTake& t : rentTakes)
                    RefundRetailCharge(t.person, t.amount);
                if (g_probe) {
                    wchar_t nm[128] = {0};
                    Internal_GetBuildingCustomName(b, nm, 128);
                    Logf("enterprise_base  \"%ls\" rent credit refused for "
                         "%.2f - refunded %zu tenant(s)",
                         nm[0] ? nm : L"(unnamed)", buildingRent,
                         rentTakes.size());
                }
            }
        }
    }

    if (g_probe && (totalRent > 0.0 || buildings > 0))
        Logf("enterprise_base  residential rent: %.2f %s from %d tenant(s) "
             "in %d building(s)", totalRent, TsmCurrencySymbol(g_currency),
             tenants, buildings);

    // v8.2 (Task 1): OUT-OF-LOCK dispatch. Candidates re-validate here
    // (the world may have changed since staging): a confirmed flat moves
    // via FUN_14083CA50 with the happiness penalty; no vacancy anywhere
    // keeps the citizen in place with a small arrears dent - never
    // nullptr (homelessness backstop: vanilla would despawn them).
    int evicted = 0, retained = 0;
    for (size_t i = 0; i < plan.size(); i++) {
        void* person = plan[i].person;
        void* dest   = plan[i].dest;
        if (!TsmIsValidGamePtr(person) ||
            !ReadablePtr(person, TSM_PERSON_SIZE))
            continue;
        if (dest && (!TsmIsValidGamePtr(dest) ||
                     !ReadablePtr(dest, TSM_BLD_MIN_READABLE)))
            dest = nullptr;
        if (dest) {
            void* game = TsmGameObject(g_exeBase);
            if (!InvokeRelocateRaw(game, person, dest)) {
                DentHappinessRaw(person, 0.05f);
                retained++;
                continue;
            }
            DentHappinessRaw(person, g_evictionPenaltyHappiness);
            evicted++;
            g_monthlyEvictionCount++;
            if (g_probe)
                Logf("enterprise_base  [EVICTION] Relocated tenant %p "
                     "to %p (wealth: %.2f, %s)",
                     person, dest, plan[i].wealth,
                     (plan[i].reason == 2) ? "destitute: under one day of bread"
                     : (plan[i].reason == 1 ? "short pay"
                                            : "rent burden exceeded"));
        } else {
            DentHappinessRaw(person, 0.05f);
            retained++;
            if (g_probe)
                Logf("enterprise_base  [EVICTION FAILED] No vacant flats in "
                     "Republic for tenant %p; citizen retained in place.",
                     person);
        }
    }
    if (g_probe && (evicted > 0 || retained > 0))
        Logf("enterprise_base  eviction pass: %d relocated, %d retained "
             "(arrears), %d staged", evicted, retained, (int)plan.size());
}

// ---------------------------------------------------------------- pool siphon
//
// v5.4: dividends come strictly from net positive operating profit.
//   netProfit = grossSales - (wages + wholesale + taxes)
// A firm with no sales (idle factory, empty residence) pays nothing,
// even if its capital sits above the reserve. The reserve still caps
// the payout so working capital is never siphoned.

static void SiphonToPool(int month, int year)
{
    if (!g_poolEnabled) return;

    EnterCriticalSection(&g_lock);

    double total = 0.0;
    int n = 0;

    for (auto& kv : g_companies) {
        Enterprise& e = kv.second;
        if (!e.isPrivate) continue;

        if (e.lastSiphonMonth == month && e.lastSiphonYear == year) continue;
        e.lastSiphonMonth = month;
        e.lastSiphonYear  = year;
        // v6.1: this enterprise's new cycle - clear LAST month's reading
        // before the fresh one is written below. OnMonth's post-siphon
        // loop used to zero it microseconds after SiphonToPool set it,
        // so the panels always read div +0.00. Sits after the skip
        // guard so a double OnMonth for one month cannot wipe a live
        // reading, and before every `continue` so an idle firm shows 0.
        e.monthlyDividendPaid = 0.0;

        if (e.capital <= 0.0) continue;

        double buffer = (e.monthlyWagesPaid * (double)g_payrollBufferMonths)
                      + (e.monthlyWholesaleCosts * (double)g_wholesaleBufferMonths)
                      + g_legalReserveFlat;

        double excess = e.capital - buffer;
        if (excess <= 100.0) continue;

        // v5.4: strictly from net positive operating profit.
        double netProfit = e.monthlyGrossSales
                         - (e.monthlyWagesPaid
                            + e.monthlyWholesaleCosts
                            + e.monthlyTaxesPaid);
        if (netProfit <= 0.0) continue;

        double dividend = netProfit * (double)g_siphonRate;
        if (dividend > excess) dividend = excess;
        if (dividend <= 0.0) continue;
        double _capBefore = e.capital;
        e.capital -= dividend;
        g_lastCap[kv.first] = e.capital;
        if (g_probe)
            Logf("enterprise_base  [CAP] direct %p %+.3f by=dividend  "
                 "%.2f -> %.2f",
                 kv.first, -dividend, _capBefore, e.capital);
        e.monthlyDividendPaid = dividend;
        total += dividend;
        n++;
    }

    if (total > 0.0) {
        extern void ModifyPoolInternal(double delta);
        ModifyPoolInternal(total);
    }

    if (g_probe && n > 0)
        Logf("enterprise_base  monthly siphon: %.2f %s from %d enterprise(s)",
             total, TsmCurrencySymbol(g_currency), n);

    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- pool access

static double g_pool = 50000.0;
static double g_monthlyPoolInflow = 0.0;
static int    g_lastPoolMonth = -1;
static int    g_lastPoolYear  = -1;

static double g_lastMonthSales      = 0.0;
static double g_lastMonthWages      = 0.0;
static double g_lastMonthTaxes      = 0.0;
static double g_lastMonthPoolInflow = 0.0;

void ModifyPoolInternal(double delta)
{
    g_pool += delta;
    if (g_pool < 0.0) g_pool = 0.0;
    if (delta > 0.0) g_monthlyPoolInflow += delta;
}

static double GetPoolImpl(void)
{
    EnterCriticalSection(&g_lock);
    double v = g_pool;
    LeaveCriticalSection(&g_lock);
    return v;
}

static void ModifyPoolImpl(double delta, const char* /*reason*/)
{
    if (!(delta == delta)) return;
    EnterCriticalSection(&g_lock);
    ModifyPoolInternal(delta);
    LeaveCriticalSection(&g_lock);
}

static double GetMonthlyPoolInflowImpl(void)
{
    EnterCriticalSection(&g_lock);
    double v = g_monthlyPoolInflow;
    LeaveCriticalSection(&g_lock);
    return v;
}

static void GetMonthlyEconomySummaryImpl(double* outSales, double* outWages,
                                         double* outTaxes,
                                         double* outPoolInflow)
{
    EnterCriticalSection(&g_lock);
    if (outSales)      *outSales      = g_lastMonthSales;
    if (outWages)      *outWages      = g_lastMonthWages;
    if (outTaxes)      *outTaxes      = g_lastMonthTaxes;
    if (outPoolInflow) *outPoolInflow = g_lastMonthPoolInflow;
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- in-window panel (v6)

static int DescribeEnterpriseImpl(void* building, TsmEnterpriseInfo* out)
{
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->structSize = sizeof(TsmEnterpriseInfo);
    if (!TsmIsValidGamePtr(building)) return 0;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    if (it != g_companies.end())
    {
        const Enterprise& e = it->second;
        out->isPrivate         = e.isPrivate ? 1 : 0;
        out->solvent           = (g_allowDebt || e.capital > 0.0) ? 1 : 0;
        out->capital           = e.capital;
        out->markup            = e.markup;
        out->monthSales        = e.monthlyGrossSales;
        out->monthWholesale    = e.monthlyWholesaleCosts;
        out->monthWages        = e.monthlyWagesPaid;
        out->monthTaxes        = e.monthlyTaxesPaid;
        out->monthDividend     = e.monthlyDividendPaid;
        out->monthNet          = e.monthlyGrossSales
                               - (e.monthlyWholesaleCosts
                                  + e.monthlyWagesPaid
                                  + e.monthlyTaxesPaid);
        out->accruedVat        = e.accruedVatLiability;
        out->accruedProfitTax  = e.accruedProfitTaxLiability;
        out->poolTotal         = g_pool;
        out->poolMonthInflow   = g_monthlyPoolInflow;
    }
    int found = (it != g_companies.end());
    LeaveCriticalSection(&g_lock);
    return found ? 1 : 0;
}

// ui_panels calls this for every building whose window is open, on the
// render thread. It copies under the register lock and formats after, so the
// lock is held for microseconds. A building that is not [P] gets zero rows:
// its window stays byte-identical to vanilla.
static void PushPanelRow(TsmPanelRows* out, const wchar_t* label,
                         const wchar_t* value)
{
    if (out->count >= out->capacity || out->count >= TSM_PANEL_MAX_ROWS) return;
    TsmPanelRow* r = &out->rows[out->count++];
    wcsncpy_s(r->label, TSM_PANEL_LABEL, label, _TRUNCATE);
    wcsncpy_s(r->value, TSM_PANEL_VALUE, value, _TRUNCATE);
    r->colour = 0;   // the hook's default label colour
}

static int StaffCountsImpl(void* b, int* outCap);   // M2, defined below

static void EnterprisePanelRows(void* building, TsmPanelRows* out,
                                void* /*ud*/)
{
    if (!g_panelRows || !building || !out) return;

    // v9.6: the probe block below reads bld+0xEA8/0x1198/0x604/0x5D0 and
    // calls TsmBuildingType() for the FIRST pointer it ever sees - including
    // a Person* on the citizen panel. Refuse anything that is not a Building
    // before any of that runs.
    if (!TsmLooksLikeBuilding(building)) return;

    TsmEnterpriseInfo info;
    int foundRow = DescribeEnterpriseImpl(building, &info);
    if (g_probe) {
        // Once per building pointer per session, not per frame - the append
        // runs every rendered frame and flooded the log at ~20 lines/second.
        // v8.7: log the engine-side state so a missing [PRIV TILE] line is
        // decidable here - dem (char +0xEA8, the dispatcher's own routing
        // byte), decC (char +0x1198, what the dispatcher allows the normal
        // chain on), decI (the int TsmIsDecayed reads), built (+0x604),
        // bbox (+0x5D0). decC=0 with decI!=0 is the v8.7 mismatch case.
        static std::unordered_set<void*> s_logged;
        if (s_logged.insert(building).second) {
            size_t nRow = 0;
            EnterCriticalSection(&g_lock);
            nRow = g_companies.size();
            LeaveCriticalSection(&g_lock);
            int   dem  = 0, decC = 0, decI = 0, bbox = 0;
            float built = -1.0f;
            if (ReadablePtr((BYTE*)building + TSM_BLD_DEMOLISHED, 1))
                dem = *(BYTE*)((BYTE*)building + TSM_BLD_DEMOLISHED) ? 1 : 0;
            if (ReadablePtr((BYTE*)building + TSM_BLD_DECAYED, 1))
                decC = (int)(*(BYTE*)((BYTE*)building + TSM_BLD_DECAYED));
            if (ReadablePtr((BYTE*)building + TSM_BLD_DECAYED, sizeof(int)))
                decI = *(int*)((BYTE*)building + TSM_BLD_DECAYED);
            if (ReadablePtr((BYTE*)building + TSM_BLD_BUILT, sizeof(float)))
                built = *(float*)((BYTE*)building + TSM_BLD_BUILT);
            if (ReadablePtr((BYTE*)building + TSM_BLD_BBOX, sizeof(void*)))
                bbox = (*(void**)((BYTE*)building + TSM_BLD_BBOX)) ? 1 : 0;
            Logf("enterprise_base  panel probe: b=%p type=%d found=%d "
                 "dem=%d decC=%d decI=%d built=%.2f bbox=%d companies=%zu",
                 building, TsmBuildingType(building), foundRow,
                 dem, decC, decI, built, bbox, nRow);
        }
    }
    if (!foundRow) return;

    // v8.0 (Task 3): an abandoned firm is deliberately stripped of its
    // private tag (so no loop buys, sells or staffs it), but its window
    // should still report WHY it went quiet. State buildings - which are
    // not in the register at all - keep the vanilla window.
    {
        bool abandoned = false;
        EnterCriticalSection(&g_lock);
        auto ait = g_companies.find(building);
        if (ait != g_companies.end() && ait->second.isAbandoned)
            abandoned = true;
        LeaveCriticalSection(&g_lock);
        if (abandoned) {
            PushPanelRow(out, L"Status", L"Abandoned (Ruins)");
            return;
        }
    }

    if (!info.isPrivate) return;     // state buildings stay vanilla

    wchar_t v[TSM_PANEL_VALUE];

    // Honest label per revenue source: retail shops have genuine citizen
    // purchases (TSM_CAP_RETAIL), service buildings charge fees (pub etc),
    // residences collect rent (TSM_CAP_RENT), factories/mines record state
    // procurement of dispatched production (TSM_CAP_PRODUCTION).
    int bldType = TsmBuildingType(building);
    const wchar_t* salesLabel = L"";
    if (bldType == 2)
        salesLabel = L"Rent";           // TSM_CAP_RENT
    else if (bldType == 3)
        salesLabel = L"Sales (retail)"; // TSM_CAP_RETAIL
    else if (bldType == 23)
        salesLabel = L"Sales (service)"; // service fees
    else
        salesLabel = L"Revenue (prod.)"; // TSM_CAP_PRODUCTION - state procurement

    // v8.3 (Orders 1+3): native-style section divider first, then Owner +
    // Rent on the first two data lines so the lease rate is visible without
    // scrolling (Diagnostic C: the old Rent row sat 4th and slid under the
    // window fold on short residence panels).
    PushPanelRow(out, L"[ Private Enterprise ]", L"");

    PushPanelRow(out, L"Owner", L"Private");

    // v8.3 (Order 3): residences push the Rent row SECOND, before Capital,
    // so the per-tenant rate + environmental factors never need a scroll.
    // Non-residences keep the old Owner, Capital, Sales order.
    if (bldType == 2) {
        float attr = TsmAttractiveness(building);
        float flatCap = ReadFlatCapacityRaw((void*)building);
        float rentLevel = ReadRentLevelRaw((void*)building, 0.5f);
        const wchar_t* sym2 = (IsUsdActive() ? L"USD" : L"RUB");
        // v8.3 (Order 4): the panel shows the same park-capped,
        // rent-slider-grounded rate the rent loop charges, plus both
        // verified engine inputs: Attr (bld+0x11B0, FUN_1404D5F50) and
        // Rent (bld+0x13E0, FUN_14073E950.GetString(0x995) slider).
        double rate = RentRateFor(flatCap, attr, rentLevel);
        _snwprintf_s(v, _countof(v), _TRUNCATE,
                     L"%.2f %ls/tenant (Attr %.0f%%, Rent %.0f%%)",
                     rate, sym2,
                     attr * 100.0f, rentLevel * 100.0f);
        PushPanelRow(out, salesLabel, v);
    }

    // v4: capital row carries the active currency symbol.
    {
        const wchar_t* sym = (IsUsdActive() ? L"USD" : L"RUB");
        _snwprintf_s(v, _countof(v), _TRUNCATE, L"%.2f %ls", info.capital, sym);
    }
    PushPanelRow(out, L"Capital", v);

    if (bldType != 2) {
        _snwprintf_s(v, _countof(v), _TRUNCATE, L"%+.2f", info.monthSales);
        PushPanelRow(out, salesLabel, v);
    }

    _snwprintf_s(v, _countof(v), _TRUNCATE, L"%-+.2f / %-+.2f",
                 info.monthWages, info.monthTaxes);
    PushPanelRow(out, L"Wages / taxes", v);

    // M2: effective daily wage + market state. The base comes from
    // citizen_labor's resolver (sector/override/floor aware on v3, type
    // rate on v2); the multiplier mirrors exactly what OnPayroll will
    // pay this month. Currency symbol is the active one.
    if (g_labor) {
        float baseWage = 0.0f;
        if (TsmLaborHasV3(g_labor))
            baseWage = g_labor->getEffectiveWageForBuilding(building);
        if (!(baseWage > 0.0f))
            baseWage = g_labor->getWageForType(TsmBuildingType(building));

        float mult = 1.0f;
        int aust = 0;
        EnterCriticalSection(&g_lock);
        auto wit = g_companies.find(building);
        if (wit != g_companies.end()) {
            mult = wit->second.wageBonusMult;
            aust = wit->second.inAusterity ? 1 : 0;
        }
        LeaveCriticalSection(&g_lock);
        if (!(mult >= 1.0f)) mult = 1.0f;

        int dynBid = DynamicBiddingActive();
        float eff = baseWage * (dynBid ? mult : g_privateWageBonus);
        if (TsmLaborHasV3(g_labor)) {
            float minw = g_labor->getMinimumWage();
            if (eff < minw) eff = minw;
        }

        if (baseWage > 0.0f) {
            const wchar_t* sym = (IsUsdActive() ? L"USD" : L"RUB");
            if (aust)
                _snwprintf_s(v, _countof(v), _TRUNCATE,
                             L"%.2f %ls (Austerity Mode)", eff, sym);
            else if (dynBid && mult > 1.005f)
                _snwprintf_s(v, _countof(v), _TRUNCATE,
                             L"%.2f %ls (+%.0f%% Market Bonus)",
                             eff, sym, (mult - 1.0f) * 100.0f);
            else
                _snwprintf_s(v, _countof(v), _TRUNCATE,
                             L"%.2f %ls (Market Baseline)", eff, sym);
            PushPanelRow(out, L"Wage", v);
        }
    }

    // M2: labor status - workers present / native capacity, from the
    // same guarded reads the wage-bidding staffing ratio uses.
    {
        int cap = 0;
        int cnt = StaffCountsImpl(building, &cap);
        if (cnt >= 0 && cap > 0) {
            int pct = (int)((double)cnt * 100.0 / (double)cap + 0.5);
            _snwprintf_s(v, _countof(v), _TRUNCATE,
                         L"%d/%d Staff (%d%% Capacity)", cnt, cap, pct);
            PushPanelRow(out, L"Labor", v);
        }
    }

    _snwprintf_s(v, _countof(v), _TRUNCATE, L"%+.2f  div %+.2f",
                 info.monthNet, info.monthDividend);
    PushPanelRow(out, L"Net profit", v);

    _snwprintf_s(v, _countof(v), _TRUNCATE, L"%.2f (%+.2f)",
                 info.poolTotal, info.poolMonthInflow);
    PushPanelRow(out, L"Investment pool", v);

    // v8.0 (Task 3): solvency status, last so it lands at the panel foot
    // within the 12-row budget. Values come from the register under lock;
    // the abandoned case is reachable because we kept isPrivate true only
    // for live firms - here we re-check both fields fresh.
    {
        int months = 0;
        bool abandoned = false;
        bool priv = true;
        EnterCriticalSection(&g_lock);
        auto sit = g_companies.find(building);
        if (sit != g_companies.end()) {
            months    = sit->second.insolventMonths;
            abandoned = sit->second.isAbandoned;
            priv      = sit->second.isPrivate;
        }
        LeaveCriticalSection(&g_lock);

        const wchar_t* status;
        if (abandoned || !priv)
            status = L"Abandoned (Ruins)";
        else if (months == 1)
            status = L"Distressed (Hiring Frozen)";
        else if (months >= 2)
            status = L"Insolvent (Facing Auction)";
        else
            status = L"Solvent";
        PushPanelRow(out, L"Status", status);
    }
}

// ---------------------------------------------------------------- M2 wage market
//
// Staffing ratio = live workers (people vector) / max(1, capacity int
// at bld+TSM_BLD_MAX_WORKERS). Unreadable structure => 1.0 ("fully
// staffed") so no firm bids up on garbage; a valid empty vector => 0.0.
// Reads game memory - the caller holds g_lock and brackets the pass.
static float StaffingRatioImpl(void* b)
{
    if (!TsmIsValidGamePtr(b)) return 1.0f;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return 1.0f;
    if (!ReadablePtr((BYTE*)b + TSM_BLD_MAX_WORKERS, sizeof(int)))
        return 1.0f;

    void** begin = *(void***)((BYTE*)b + TSM_BLD_WORKERS_BEGIN);
    void** end   = *(void***)((BYTE*)b + TSM_BLD_WORKERS_END);
    if ((begin == nullptr) != (end == nullptr)) return 1.0f;   // half-built
    size_t n = (begin && end && end > begin) ? (size_t)(end - begin) : 0;
    if (n > 100000) return 1.0f;                               // nonsense span

    int cap = *(int*)((BYTE*)b + TSM_BLD_MAX_WORKERS);
    if (!(cap >= 1)) cap = 1;                                  // max(1, cap)
    float r = (float)n / (float)cap;
    if (r > 1.0f) r = 1.0f;
    return r;
}

// M2: raw staff counts for the panel's Labor row - workers present and
// the native capacity int, both guarded exactly like
// StaffingRatioImpl. Callable WITHOUT g_lock (the panel runs on the
// render thread; ui_panels brackets the append in __try and the span
// guards keep a concurrent resize from producing nonsense). Returns -1
// when the structure cannot be read; *outCap is then left 0.
static int StaffCountsImpl(void* b, int* outCap)
{
    if (outCap) *outCap = 0;
    if (!TsmIsValidGamePtr(b)) return -1;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return -1;
    if (!ReadablePtr((BYTE*)b + TSM_BLD_MAX_WORKERS, sizeof(int)))
        return -1;

    void** begin = *(void***)((BYTE*)b + TSM_BLD_WORKERS_BEGIN);
    void** end   = *(void***)((BYTE*)b + TSM_BLD_WORKERS_END);
    if ((begin == nullptr) != (end == nullptr)) return -1;
    size_t n = (begin && end && end > begin) ? (size_t)(end - begin) : 0;
    if (n > 100000) return -1;

    int capi = *(int*)((BYTE*)b + TSM_BLD_MAX_WORKERS);
    if (!(capi >= 1)) capi = 1;
    if (capi > 100000) capi = 100000;
    if (outCap) {
        *outCap = capi;
        if (*outCap < 1) *outCap = 1;
    }
    return (int)n;
}

// M2: the monthly wage-market loop. Called from OnMonth after the siphon
// and BEFORE the accumulator clear loop (it reads the very monthly
// fields that loop zeroes - same trap as the v6.1 dividend fix). Each
// private, finished, non-demolished enterprise either:
//   A) booming + understaffed: netProfit > 0 && capital > legal reserve
//      && staffing < understaffed_threshold  -> +g_wageBiddingStep (cap)
//   B) distressed: netProfit <= 0 || capital <= 0 -> -g_wageBiddingStep
//      (mult floor 1.0; the effective wage floor is minimum_wage and is
//      applied at payroll in OnPayroll)
// and mirrors B's condition into e.inAusterity for the panel row. The
// statutory wage floor itself is applied at payroll (OnPayroll), not
// here - the multiplier only scales.
static void EvaluateWageBidding(void)
{
    if (!DynamicBiddingActive()) return;

    EnterCriticalSection(&g_lock);
    for (auto& kv : g_companies) {
        Enterprise& e = kv.second;
        if (!e.isPrivate) continue;

        void* b = kv.first;
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
            continue;
        if (TsmIsDemolished(b) || !TsmIsFinished(b)) continue;

        double netProfit = e.monthlyGrossSales
                         - (e.monthlyWagesPaid
                            + e.monthlyWholesaleCosts
                            + e.monthlyTaxesPaid);
        float staff = StaffingRatioImpl(b);

        bool distressed = (netProfit <= 0.0 || e.capital <= 0.0);
        bool booming    = (netProfit > 0.0 &&
                           e.capital > g_legalReserveFlat &&
                           staff < g_understaffedThreshold);
        e.inAusterity = distressed;

        if (booming) {
            float next = e.wageBonusMult + g_wageBiddingStep;
            if (next > g_maxPrivatePremium) next = g_maxPrivatePremium;
            if (next > e.wageBonusMult) {
                e.wageBonusMult = next;
                if (g_probe) {
                    wchar_t nm[128] = {0};
                    if (!Internal_GetBuildingCustomName(b, nm, 128) || !nm[0])
                        wcsncpy_s(nm, _countof(nm), L"(unnamed)", _TRUNCATE);
                    Logf("enterprise_base  [WAGE HIKE] %ls raised wage bonus "
                         "to +%.0f%% (profit: %.2f %ls, staff: %.0f%%)",
                         nm, (e.wageBonusMult - 1.0f) * 100.0f, netProfit,
                         IsUsdActive() ? L"USD" : L"RUB", staff * 100.0f);
                }
            }
        } else if (distressed) {
            float next = e.wageBonusMult - g_wageBiddingStep;
            if (next < 1.0f) next = 1.0f;
            if (next < e.wageBonusMult) {
                e.wageBonusMult = next;
                if (g_probe) {
                    wchar_t nm[128] = {0};
                    if (!Internal_GetBuildingCustomName(b, nm, 128) || !nm[0])
                        wcsncpy_s(nm, _countof(nm), L"(unnamed)", _TRUNCATE);
                    Logf("enterprise_base  [AUSTERITY] %ls cut wage bonus "
                         "to +%.0f%%",
                         nm, (e.wageBonusMult - 1.0f) * 100.0f);
                }
            }
        }
    }
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- v8.0 (Task 3): phased insolvency
//
// Negative-capital firms advance one stage per month boundary:
//   month 1  [DISTRESSED]   hiring freeze. Stage 1 is EFFECTIVE through
//                            the existing insolvent-payroll path: capital
//                            <= 0 -> IsSolventImpl reports insolvent ->
//                            OnPayroll returns TSM_PAYROLL_DEFAULTED ->
//                            citizen_labor inserts the workplace into its
//                            poison set and tomorrow's staffing pass skips
//                            it. No cross-DLL call is made from here (the
//                            poison set is citizen_labor-private, and
//                            enterprise_base's own lock would invert the
//                            documented intra-level callback rule).
//   month 2  [AUCTION]      investment pool attempts a buyout at
//                            startingCapital x auction_buyout_cost_mult;
//                            on success the firm recapitalises and resets.
//   month N  [ABANDONED]    native FUN_14013FE70(game, bld) collapses the
//                            building into ruins; tag stripped, state
//                            latched, all economic loops skip it after.
// Recovery (capital > 0 again) resets the counter at any stage. Runs in
// OnMonth AFTER wage bidding (both read e.capital, neither depends on the
// monthly accumulators the later clear loop zeroes).

// POD-only SEH helper: __try is illegal in a frame that needs C++ unwinding
// (C2712), and EvaluateInsolvency walks g_companies (std::unordered_map),
// so the native call lives in its own function with only raw locals.
static void CallNativeAbandon(void* game, void* bld)
{
    if (!TsmCheckExeBuild(H, "enterprise_base")) return;
    const unsigned char* code = TsmCallableCodePtr(H, TSM_F_ABANDON, "abandon_building");
    if (!code) return;

    typedef void (*t_Abandon)(void*, void*);
    t_Abandon fn = (t_Abandon)code;
    __try { fn(game, bld); }
    __except (FaultFilter("enterprise_base native abandon", GetExceptionInformation())) {
        Logf("enterprise_base  ERROR calling native abandonment at 0x%X",
             (unsigned)TSM_F_ABANDON);
    }
}

static void EvaluateInsolvency(void)
{
    if (!g_abandonEnabled) return;

    // Snapshot which firms hit the ruin stage under the lock, then release
    // it: the native call touches engine-side visual lists and must never
    // run while another thread waits on g_lock.
    std::vector<void*> toAbandon;
    toAbandon.reserve(8);

    EnterCriticalSection(&g_lock);
    for (auto& kv : g_companies) {
        Enterprise& e = kv.second;
        if (!e.isPrivate || e.isAbandoned) continue;

        void* b = kv.first;
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
            continue;
        if (TsmIsDemolished(b) || !TsmIsFinished(b)) continue;

        const bool payrollFailed = e.payrollDefaulted;
        e.payrollDefaulted = false;

        if (e.capital > 0.0 && !payrollFailed && !e.stalledOnDust) {
            if (e.insolventMonths > 0) {
                if (g_probe) {
                    wchar_t nm[128] = {0};
                    FillLogBuildingName(b, nm, _countof(nm));
                    Logf("enterprise_base  [RECOVERED] %ls is solvent again "
                         "(capital %.2f)", nm, e.capital);
                }
                e.insolventMonths = 0;
            }
            continue;
        }

        // capital <= 0 or stalled after a payroll default: advance one month down the ladder.
        e.insolventMonths++;

        if (e.insolventMonths >= g_monthsToAbandon) {
            // Stage 3: physical abandonment. Latch state now (so every
            // economic loop stops this month) and strip the tag so the
            // registration pass cannot re-privatise it; the native swap
            // runs outside the lock below.
            e.isAbandoned = true;
            e.isPrivate   = false;
            toAbandon.push_back(b);
            if (g_probe) {
                wchar_t nm[128] = {0};
                FillLogBuildingName(b, nm, _countof(nm));
                Logf("enterprise_base  [ABANDONED] %ls collapsed into ruins "
                     "after %d distressed month(s)", nm, e.insolventMonths);
            }
        } else if (e.insolventMonths == 1) {
            if (g_probe) {
                wchar_t nm[128] = {0};
                FillLogBuildingName(b, nm, _countof(nm));
                Logf("enterprise_base  [DISTRESSED] %ls (capital %.2f) - "
                     "hiring frozen, no further staff assigned",
                     nm, e.capital);
            }
        } else if (e.insolventMonths == 2) {
            // Stage 2: the pool may rescue the firm outright. The pool is
            // OURS to move (same module as SiphonToPool), and the reserve
            // never fires here - an auction buyout is a full recapitalisation
            // by design, priced off starting capital, not the buffer.
            double buyoutCost = g_startingCapital * (double)g_auctionBuyoutMult;
            if (g_pool >= buyoutCost && buyoutCost > 0.0) {
                ModifyPoolInternal(-buyoutCost);
                double _capBefore = e.capital;
                e.capital = g_startingCapital;
                g_lastCap[b] = e.capital;
                if (g_probe)
                    Logf("enterprise_base  [CAP] direct %p %+.3f by=buyout  "
                         "%.2f -> %.2f",
                         b, e.capital - _capBefore, _capBefore, e.capital);
                e.insolventMonths  = 0;
                e.payrollDefaulted = false;
                e.stalledOnDust    = false;
                if (g_probe) {
                    wchar_t nm[128] = {0};
                    FillLogBuildingName(b, nm, _countof(nm));
                    Logf("enterprise_base  [AUCTION BUYOUT] investment pool "
                         "acquired %ls for %.2f %ls (capital restored to %.2f)",
                         nm, buyoutCost, IsUsdActive() ? L"USD" : L"RUB",
                         g_startingCapital);
                }
            } else if (g_probe) {
                wchar_t nm[128] = {0};
                FillLogBuildingName(b, nm, _countof(nm));
                Logf("enterprise_base  [AUCTION FAILED] pool %.2f cannot cover "
                     "%.2f - %ls faces ruin", g_pool, buyoutCost, nm);
            }
        }
    }
    if (!toAbandon.empty()) RebuildOrder();
    LeaveCriticalSection(&g_lock);

    // Native call OUTSIDE g_lock: game memory, engine visual lists, and an
    // SEH frame must not sit behind the register lock other threads hold.
    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;
    for (void* b : toAbandon) {
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
            continue;
        // Pre-ruin evacuation: relocate all residents of residential
        // buildings before native abandonment collapses the structure,
        // preventing citizen pointer corruption (Task 3 & 8, §6.1).
        if (TsmBuildingType(b) == TSM_TYPE_RESIDENTIAL) {
            void** rBegin = nullptr;
            void** rEnd = nullptr;
            if (ReadResidentSpan(b, &rBegin, &rEnd)) {
                size_t n = (size_t)(rEnd - rBegin);
                if (n > 256) n = 256;
                void* gameObj = TsmGameObject(g_exeBase);
                for (size_t k = 0; k < n; k++) {
                    void* person = rBegin[k];
                    if (!TsmIsValidGamePtr(person) || !ReadablePtr(person, TSM_PERSON_SIZE))
                        continue;
                    void* dest = FindRelocationCandidate(gameObj, b, 0.5f);
                    if (dest)
                        InvokeRelocateRaw(gameObj, person, dest);
                }
            }
        }
        CallNativeAbandon(game, b);
    }
}

// ---------------------------------------------------------------- interceptors

static int OnPayroll(void* workplace, void* person, float grossWage,
                     int buildingType, void* /*userData*/)
{
    if (!TsmIsValidGamePtr(workplace) || !TsmIsValidGamePtr(person))
        return TSM_PAYROLL_IGNORED;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(workplace);
    bool isPrivate = (it != g_companies.end() && it->second.isPrivate);
    float mult = isPrivate ? it->second.wageBonusMult : 1.0f;
    LeaveCriticalSection(&g_lock);
    if (!isPrivate) return TSM_PAYROLL_IGNORED;

    // M2: dynamic bidding multiplier replaces the static bonus when the
    // market engine is on; private_wage_bonus applies only with
    // dynamic_bidding_enabled = 0 (the off-switch). Then floor to the
    // statutory minimum (citizen_labor v3; v2 has no floor accessor and
    // citizen_labor's own resolver already floors its grossWage).
    if (!(mult >= 1.0f)) mult = 1.0f;
    float wage = grossWage * (DynamicBiddingActive() ? mult : g_privateWageBonus);
    if (TsmLaborHasV3(g_labor)) {
        float minw = g_labor->getMinimumWage();
        if (wage < minw) wage = minw;
    }

    // v9.13: the contract. v9.12 proved the 8-15x arrives inside
    // `grossWage` with this plugin's premium at 1.00, so nothing here
    // created it - but this is the party doing the paying, and it never
    // advertised 1271.76 for a 3.00/day trade. Cap at the posted rate plus
    // the premium the panel shows the player. Fails open: with no labor v3
    // or no clock, nothing is clamped.
    double contract = 0.0;
    int    capDpm   = 30;
    float  capDaily = 0.0f;
    float  capMin   = 0.0f;
    int    capRateBad = 0;
    // H3 FIX: snapshot globals to avoid race on unload (lock not held across tick)
    auto* laborSnap = g_labor;
    auto* clockSnap = g_simClock;
    if (g_wageCapEnabled && laborSnap && TsmLaborHasV3(laborSnap) && clockSnap) {
        capDaily = laborSnap->getEffectiveWageForBuilding(workplace);
        capMin   = laborSnap->getMinimumWage();
        if (clockSnap->structSize >=
            offsetof(TsmSimClockApi, getDaysPerMonth) +
                sizeof(clockSnap->getDaysPerMonth)) {
            int d = clockSnap->getDaysPerMonth();
            if (d > 0 && d <= 400) capDpm = d;
        }
        // v9.14: the rate this cap multiplies is itself persisted data.
        // citizen_labor restores all six sector base wages from
        // tesmio_labor.dat over the INI, after the banner has printed the
        // configured values. 60.56/day against an INI 2.80 made the v9.13
        // contract 2725.20 and the cap unreachable. A rate this far above
        // the statutory floor is not a wage; treat it as corrupt.
        if (capDaily > 0.0f && capMin > 0.0f &&
            capDaily > capMin * g_wageRateCeiling) {
            if (g_probe) {
                Logf("enterprise_base  [WAGE RATE] %p type %d resolved "
                     "%.2f/day vs minimum %.2f - above %.0fx, treating the "
                     "rate as corrupt and clamping at the minimum",
                     workplace, buildingType, (double)capDaily,
                     (double)capMin, (double)g_wageRateCeiling);
            }
            capDaily   = capMin;
            capRateBad = 1;
        }
        if (capDaily > 0.0f)
            contract = (double)capDaily * (double)capDpm *
                       (double)g_maxPrivatePremium;
    }

    // v9.14: once per workplace, say exactly how the wage was derived. Four
    // sessions were spent inferring these numbers from the ladders; one line
    // each removes the inference.
    if (g_probe && g_labor && TsmLaborHasV3(g_labor)) {
        static void* seen[16];
        static int   seenN = 0;
        int known = 0;
        for (int i = 0; i < seenN; i++)
            if (seen[i] == workplace) { known = 1; break; }
        if (!known && seenN < 16) {
            seen[seenN++] = workplace;
            Logf("enterprise_base  [WAGE AUDIT] %p type %d rate %.2f%s "
                 "x %d days x %.2f premium = contract %.2f | person daily "
                 "%.2f | min %.2f | sector %d rate %.2f | gross %.2f",
                 workplace, buildingType, (double)capDaily,
                 capRateBad ? " (CORRUPT, using minimum)" : "",
                 capDpm, (double)g_maxPrivatePremium, contract,
                 (double)g_labor->getEffectiveDailyWage(person),
                 (double)capMin, TSM_SECTOR_SERVICES,
                 (double)g_labor->getSectorWage(TSM_SECTOR_SERVICES),
                 (double)grossWage);
        }
        if (seenN == 1) {
            for (int s = 0; s < TSM_SECTOR_COUNT; s++)
                Logf("enterprise_base  [SECTOR RATES] sector %d = %.2f/day",
                     s, (double)g_labor->getSectorWage(s));
            Logf("enterprise_base  [SECTOR RATES] minimum = %.2f/day "
                 "(printed at the first payroll, after any save load)", (double)capMin);
        }
    }
    if (contract > 0.0 && (double)wage > contract) {
        if (g_probe) {
            Logf("enterprise_base  [WAGE CAP] %p type %d %.2f -> %.2f "
                 "(posted %.2f/day x %d days x %.2f premium; gross came in "
                 "at %.2fx the contract)",
                 workplace, buildingType, (double)wage, contract,
                 (double)capDaily, capDpm,
                 (double)g_maxPrivatePremium, (double)wage / contract);
        }
        wage = (float)contract;
    }

    // v9.12: write down the arithmetic BEFORE the affordability gate, so a
    // wage that is refused is recorded too. `grossWage` is citizen_labor's
    // number (daily rate x days_per_month, already summed); `mult` is this
    // plugin's bidding premium. Until now neither was recorded anywhere, so a
    // wage of 726.72 against a 90 RUB/month rate could not be blamed on
    // either side. With both factors on one line, one session says which.
    if (g_probe) {
        Logf("enterprise_base  [WAGE] %p type %d gross %.2f x %.2f = %.2f "
             "contract %.2f (%s, capital now %.2f)",
             workplace, buildingType, (double)grossWage, (double)mult,
             (double)wage, contract,
             DynamicBiddingActive() ? "bidding" : "legacy bonus",
             GetCapitalImpl(workplace));
    }

    // v9.11: two failures, two answers. Not solvent at all (capital <= 0)
    // is the old case; solvent but short is the one that used to slip
    // through and mint the difference. Both default - the worker is not
    // paid, which is what a wage is: an obligation the firm must be able
    // to meet. allow_debt makes CanPayWage unconditional.
    if (!IsSolventImpl(workplace) || !CanPayWage(workplace, (double)wage)) {
        EnterCriticalSection(&g_lock);
        auto it = g_companies.find(workplace);
        if (it != g_companies.end() && it->second.isPrivate) {
            it->second.payrollDefaulted = true;
            it->second.stalledOnDust    = (it->second.capital > 0.0);
        }
        LeaveCriticalSection(&g_lock);
        if (g_probe) {
            wchar_t name[128] = {0};
            FillLogBuildingName(workplace, name, _countof(name));
            double capNow = GetCapitalImpl(workplace);
            Logf("enterprise_base  \"%ls\" defaulted on a %.2f wage "
                 "(capital %.2f, short %.2f)",
                 name, wage, capNow, (double)wage - capNow);
        }
        return TSM_PAYROLL_DEFAULTED;
    }

    // v9.11: the debit must LAND before the wallet is credited. Retail has
    // enforced this since v9.6 ("no path where a sale charges one side and
    // not the other"); payroll was the last place that paid the citizen and
    // ignored a refused debit - a decayed building, a lost private tag or a
    // stale pointer all returned 0 after the wallet had already been
    // credited. TryModifyCapital logs the refusal; return DEFAULTED so
    // citizen_labor sees an unpaid worker instead of a free one.
    if (!TryModifyCapital(workplace, -(double)wage, TSM_CAP_WAGE, "wage")) {
        EnterCriticalSection(&g_lock);
        auto it = g_companies.find(workplace);
        if (it != g_companies.end() && it->second.isPrivate) {
            it->second.payrollDefaulted = true;
            it->second.stalledOnDust    = (it->second.capital > 0.0);
        }
        LeaveCriticalSection(&g_lock);
        if (g_probe) {
            wchar_t name[128] = {0};
            FillLogBuildingName(workplace, name, _countof(name));
            Logf("enterprise_base  \"%ls\" wage debit refused for %.2f - "
                 "worker not paid", name, wage);
        }
        return TSM_PAYROLL_DEFAULTED;
    }
    EnterCriticalSection(&g_lock);
    {
        auto it = g_companies.find(workplace);
        if (it != g_companies.end()) {
            it->second.payrollDefaulted = false;
            it->second.stalledOnDust    = false;
        }
    }
    LeaveCriticalSection(&g_lock);
    g_currency->addWalletBalance(person, wage);
    return TSM_PAYROLL_PAID;
}

// v9.6: reverses BOTH halves of a chargeCitizenPartial debit.
//
// currency's WalletTake (currency.cpp, PERSON_SPENT_OFFSET = 0x734) writes
// the wallet ledger AND the ENGINE's own "spent this period" accumulator on
// the Person. addWalletBalance only restores the ledger, so a refund that
// stops there leaves person+0x734 inflated - and FUN_140171E10 folds that
// field into the citizen's satisfaction (+0x738) and the city's retail
// turnover. Money handed back must not improve anybody's shopping mood.
// person+0x734 is an engine field (see TSM_PERSON_SPENT in game_world.h),
// not currency-private state, so reversing it here crosses no plugin
// boundary and needs no ABI change.
static void RefundRetailCharge(void* person, float amount)
{
    if (!(amount > 0.0f)) return;
    if (!g_currency || !person) return;

    g_currency->addWalletBalance(person, amount);

    if (!TsmIsValidGamePtr(person)) return;
    if (!ReadablePtr((BYTE*)person + TSM_PERSON_SPENT, sizeof(float))) return;
    float* spent = (float*)((BYTE*)person + TSM_PERSON_SPENT);
    float v = *spent - amount;
    if (!(v == v)) return;                 // never write a NaN back
    if (v < 0.0f) v = 0.0f;
    *spent = v;
}

static int OnRetail(void* shop, void* person, float stateCost,
                    void* /*resRecord*/, void* /*userData*/)
{
    if (!TsmIsValidGamePtr(shop) || !TsmIsValidGamePtr(person))
        return 0;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(shop);
    bool isPrivate = (it != g_companies.end() && it->second.isPrivate);
    float markup = isPrivate ? it->second.markup : 1.0f;
    LeaveCriticalSection(&g_lock);
    if (!isPrivate) return 0;

    float markedUp = stateCost * markup;
    // v9.6: `return 1` here means "handled" - the state stays out AND the
    // engine dispenses. That is the right answer for a zero-priced sale
    // (nothing to charge) but the wrong one for a NaN price, which used to
    // land in the same branch and give the goods away for free. Split the
    // two: a non-finite price is a refusal, a zero price is the state's
    // business (it charges 0 and the sale proceeds as vanilla).
    if (!(markedUp == markedUp)) return TSM_RETAIL_INSUFFICIENT;
    if (!(markedUp > 0.0f)) return 0;

    float vatRate = (g_taxes) ? g_taxes->getVatRate() : 0.0f;
    if (vatRate < 0.0f) vatRate = 0.0f;

    // Apply VAT on top of the marked-up retail price
    float finalPrice = markedUp * (1.0f + vatRate);

    // v9.5: PRE-CHECK BEFORE ANY CHARGE. The old currency implementation
    // partially debited even a failed full charge. Currency now fixes that
    // contract atomically; retain this precheck and refund path because this
    // interceptor deliberately uses chargeCitizenPartial, and because older
    // compatible currency DLLs may still have the historical behavior.
    // A partial take is real and must be refunded when dispense is refused.
    // Refusing a sale after a partial charge would leave the citizen poorer
    // with no goods - wallet theft. So verify the full private price
    // while nothing has been taken; only then charge.
    float wallet = g_currency->getWalletBalance(person);
    if (!(wallet == wallet) || !(finalPrice == finalPrice) ||
        wallet < finalPrice - 0.0001f) {
        if (g_probe) {
            // v9.9: arm the watch. Everything the shop's capital does for the
            // next few seconds gets logged with its cause, so a rise during a
            // refusal window has a name attached or is proved to have none.
            if (!WatchLive(shop)) {
                g_watchShop  = shop;
                g_watchUntil = GetTickCount() + (DWORD)g_watchMs;
                g_watchCap0  = GetCapitalImpl(shop);
                g_watchSum   = 0.0;
                g_watchN     = 0;
                ++g_watchId;
                Logf("enterprise_base  [WATCH %d] armed on shop %p at %.2f "
                     "for %d ms - a citizen was just refused",
                     g_watchId, shop, g_watchCap0, g_watchMs);
            }
            // v9.10: this line reset the audit baseline (g_lastCap) to the
            // balance captured when the watch was armed. Every refusal
            // re-applied a stale value, so the next audit compared reality
            // against it and reported [AUDIT GAP] for movements that had
            // been traced correctly. The wrapper owns g_lastCap; the
            // refusal path must not touch it.
            Logf("enterprise_base  [RETAIL] insufficient: shop %p "
                 "person %p wallet %.2f < private price %.2f - refused, "
                 "nothing taken",
                 shop, person, (double)wallet, (double)finalPrice);
        }
        return TSM_RETAIL_INSUFFICIENT;   // take NOTHING; gate aborts the dispense
    }

    float taken = g_currency->chargeCitizenPartial(person, finalPrice);
    if (taken < finalPrice - 0.0001f) {
        // The wallet covered finalPrice at check time; a concurrent
        // writer drained it between the check and the take. The partial
        // take is already done - refund it: the invariant is "if the
        // citizen cannot pay the full price, ZERO money is deducted",
        // not merely "no goods dispensed". WalletTake already created
        // the wallet entry on a miss, so addWalletBalance is a pure
        // increment on this thread. The engine must still abort rather
        // than hand over goods for partial payment, so report
        // insufficient and let the gate skip the dispense.
        RefundRetailCharge(person, taken);
        if (g_probe) {
            static int logged;
            if (++logged <= 20)
                Logf("enterprise_base  [RETAIL] race shortfall: shop %p "
                     "took %.2f of %.2f - refunded, aborting dispense",
                     shop, (double)taken, (double)finalPrice);
        }
        return TSM_RETAIL_INSUFFICIENT;
    }

    // Proportional split between net shop revenue and VAT liability
    float netToShop = taken / (1.0f + vatRate);
    double vatAccrued = (double)(taken - netToShop);

    // Credit gross `taken` to capital.
    // When taxes::SettleVatAll later debits `vatAccrued` (-vat) from capital,
    // the enterprise retains exactly netToShop, and periodGrossSales correctly
    // reflects the full gross revenue for taxes::SettleProfitAll.
    //
    // v9.6 ATOMIC CHARGE+CREDIT. The wallet is already debited at this point,
    // so the credit is not allowed to be best-effort any more. TryModifyCapital
    // returns 0 when the delta cannot land - the building decayed between the
    // privacy check and here, the private tag was lost, or the pointer went
    // stale. The old code called the void version and moved on, which is how a
    // citizen pays and the money simply ceases to exist.
    //
    // If the credit is refused: refund in full (both halves, see
    // RefundRetailCharge), report INSUFFICIENT so citizen_retail aborts the
    // dispense, and let the citizen walk out with their money. There is no
    // longer a path where a sale charges one side and not the other.
    if (!TryModifyCapital(shop, (double)taken, TSM_CAP_RETAIL, "retail")) {
        RefundRetailCharge(person, taken);
        if (g_probe) {
            static int logged;
            if (++logged <= 20)
                Logf("enterprise_base  [RETAIL] credit refused: shop %p "
                     "(%.2f charged) - refunded, aborting dispense "
                     "[charged-but-not-credited is now impossible]",
                     shop, (double)taken);
        }
        return TSM_RETAIL_INSUFFICIENT;
    }

    // v9.7: the decisive line. Debit side (person + the wallet it came from)
    // and credit side (the shop + its new balance) on one record. Pair it with
    // a [RETAIL] insufficient line for the same shop: if the person here is a
    // different citizen, the "phantom income" was somebody else's purchase.
    if (g_probe && WatchLive(shop)) {
        g_watchN++;
        Logf("enterprise_base  [WATCH %d SALE] shop %p person %p wallet %.2f "
             "took %.2f cap now %.2f - a DIFFERENT citizen was served",
             g_watchId, shop, person, (double)wallet, (double)taken,
             GetCapitalImpl(shop));
    }

    if (g_taxes && vatAccrued > 0.001) {
        g_taxes->recordVatAccrual(shop, vatAccrued);
    }

    return 1;
}

// ---------------------------------------------------------------- persistence structs

#define ENT_MAGIC         "TSMENT04"
#define ENT_VERSION       4u            // v8.0 (Task 3): + insolventMonths/isAbandoned
#define ENT_VERSION_V3    3u
#define ENT_VERSION_V2    2u
#define ENT_VERSION_LEGACY 1u
#define ENT_FILENAME      "tesmio_enterprise.dat"

#pragma pack(push, 1)
struct EntSaveHeader {
    char     magic[8];
    uint32_t version;
    double   pool;
    uint32_t recordCount;
    uint32_t reserved;
};

struct EntSaveRecord {
    char     objectName[64];
    float    posX;
    float    posZ;
    double   capital;
    float    markup;
    double   accruedVat;
    double   periodGrossSales;
    double   periodVatAccrued;
    double   periodWholesaleCosts;
    double   periodWagesPaid;
    double   periodForeignCosts;
    double   accruedProfitTax;
    wchar_t  customName[128];
    // M2 (v3 only): the bidding multiplier, 1.0 .. max_private_premium.
    // v1 records stop before customName, v2 records stop right here -
    // the loader picks recordSize by offsetof, so older files parse
    // unchanged and reach this field zeroed (sanitised to 1.0 on apply).
    float    wageBonusMult;
    // v8.0 / save v4 (Task 3): phased-insolvency state. v1..v3 records
    // stop at wageBonusMult; the loader picks recordSize by offsetof so
    // older files parse unchanged with these zeroed (0 months = solvent,
    // false = not abandoned), and the next monthly pass re-derives both.
    int32_t  insolventMonths;
    uint8_t  isAbandoned;
};
#pragma pack(pop)

static std::vector<EntSaveRecord> g_pendingLoadRecords;
static int g_deferredMatchAttempts = 0;
const int MAX_DEFERRED_ATTEMPTS = 15;

static bool ReadBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // 1. C3D_NODE is embedded at bld + 0x320 (NOT a pointer - cf.
    // 11-needs.md / 08-depletion.md: GetPosition(building + 0x320)).
    // The node wraps a 4x4 float transform matrix _11.._44:
    //   Row 0 (+0x00..0x0C)  Right / X-axis vector
    //   Row 1 (+0x10..0x1C)  Up    / Y-axis vector
    //   Row 2 (+0x20..0x2C)  Forward / Z-axis vector
    //   Row 3 (+0x30..0x3C)  Translation (Position) vector  <-- REAL WORLD COORDS
    // Float X at +0x30, Y at +0x34, Z at +0x38. Real coordinates are in
    // hundreds/thousands of metres, so require |x| > 1.5 || |z| > 1.5
    // to reject unit-vector / identity rows.
    const BYTE* nodeDirect = (const BYTE*)b + 0x320;
    if (ReadablePtr(nodeDirect, 0x40)) {
        float x = *(const float*)(nodeDirect + 0x30);
        float z = *(const float*)(nodeDirect + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            *outX = x;
            *outZ = z;
            return true;
        }
    }

    // 2. In case bld + 0x320 is a pointer to C3D_NODE:
    const BYTE* nodePtr = *(const BYTE* const*)((const BYTE*)b + 0x320);
    if (TsmIsValidGamePtr(nodePtr) && ReadablePtr(nodePtr, 0x40)) {
        float x = *(const float*)(nodePtr + 0x30);
        float z = *(const float*)(nodePtr + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            *outX = x;
            *outZ = z;
            return true;
        }
    }

    // 3. Fallback: Placement BBox at +0x5D0 (for unfinished construction sites)
    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((const BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                *outX = mn[1];
                *outZ = mn[3];
                return true;
            }
        }
    }

    return false;
}

static void* FindMatchForRecord(const EntSaveRecord& rec, TsmBuildingSpan* span)
{
    // --- 1. custom name with type/position validation
    if (rec.customName[0]) {
        for (size_t j = 0; j < span->count; j++) {
            BYTE* b = (BYTE*)span->begin[j];
            if (!TsmIsValidGamePtr(b)) continue;
            if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
            if (TsmIsDecayed(b)) continue;   // v8.0: ruins never re-adopt

            if (g_companies.count(b) && g_companies[b].isPrivate) continue;

            wchar_t custom[128] = {0};
            if (!Internal_GetBuildingCustomName(b, custom, 128)) continue;
            if (_wcsicmp(custom, rec.customName) == 0) {
                // Ensure duplicate custom names (e.g. "[P] Store") do not cross-wire:
                // require objectName match or position within generous map bounds
                const char* on = Internal_GetBuildingObjectName(b);
                float px = 0.0f, pz = 0.0f;
                bool hasPos = ReadBuildingPosition(b, &px, &pz);
                if (strcmp(on, rec.objectName) == 0 ||
                    (hasPos && fabsf(px - rec.posX) < 15.0f && fabsf(pz - rec.posZ) < 15.0f))
                {
                    return b;
                }
            }
        }
    }

    // --- 2. type name + position (unnamed buildings or unmatched custom names)
    for (size_t j = 0; j < span->count; j++) {
        BYTE* b = (BYTE*)span->begin[j];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;

        if (g_companies.count(b) && g_companies[b].isPrivate) continue;

        const char* on = Internal_GetBuildingObjectName(b);
        if (strcmp(on, rec.objectName) != 0) continue;

        float px = 0.0f, pz = 0.0f;
        if (!ReadBuildingPosition(b, &px, &pz)) continue;
        if (fabsf(px - rec.posX) < 2.0f && fabsf(pz - rec.posZ) < 2.0f)
            return b;
    }

    return nullptr;
}

// A structurally valid sidecar can still carry corrupt money fields
// (hand-edited save, damaged sector). Clamp each one into a plausible
// range before it reaches the live register and the next save; a
// non-finite value falls back to the field's clean default.
static double SanitizeLoadedMoney(double v, double lo, double hi,
                                  double fallback)
{
    if (!std::isfinite(v)) return fallback;
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void ProcessDeferredLoadMatches(TsmBuildingSpan* span)
{
    if (g_pendingLoadRecords.empty() || !span || span->count == 0) return;

    std::vector<EntSaveRecord> stillPending;
    stillPending.reserve(g_pendingLoadRecords.size());

    int matchedByCustom = 0;
    int matchedByPos    = 0;

    for (const auto& rec : g_pendingLoadRecords) {
        void* found = FindMatchForRecord(rec, span);

        if (!found) {
            stillPending.push_back(rec);
            continue;
        }

        bool byCustom = false;
        if (rec.customName[0]) {
            wchar_t live[128] = {0};
            if (Internal_GetBuildingCustomName(found, live, 128))
                byCustom = (_wcsicmp(live, rec.customName) == 0);
        }
        if (byCustom) matchedByCustom++;
        else          matchedByPos++;

        Enterprise& e = g_companies[found];
        e.isPrivate                 = true;
        e.markup                    = (std::isfinite(rec.markup) &&
                                       rec.markup > 0.0f &&
                                       rec.markup <= 100.0f)
                                        ? rec.markup : g_defaultMarkup;
        // Capital may legitimately be negative (debt/insolvency), but not
        // non-finite. Finite values are clamped into the plausible ±1e12
        // band (the configured starting-capital maximum) so a corrupt
        // figure cannot mint or forgive an unbounded amount, while growth
        // just above the band survives a save/load instead of vanishing.
        // Non-finite becomes zero - never a fresh starting grant.
        if (!std::isfinite(rec.capital))
            e.capital = 0.0;
        else if (rec.capital < -1.0e12)
            e.capital = -1.0e12;
        else if (rec.capital > 1.0e12)
            e.capital = 1.0e12;
        else
            e.capital = rec.capital;
        // Accruals and period totals are running charges: never negative.
        e.accruedVatLiability =
            SanitizeLoadedMoney(rec.accruedVat, 0.0, 1.0e12, 0.0);
        e.accruedProfitTaxLiability =
            SanitizeLoadedMoney(rec.accruedProfitTax, 0.0, 1.0e12, 0.0);
        e.periodGrossSales =
            SanitizeLoadedMoney(rec.periodGrossSales, 0.0, 1.0e12, 0.0);
        e.periodVatAccrued =
            SanitizeLoadedMoney(rec.periodVatAccrued, 0.0, 1.0e12, 0.0);
        e.periodWholesaleCosts =
            SanitizeLoadedMoney(rec.periodWholesaleCosts, 0.0, 1.0e12, 0.0);
        e.periodWagesPaid =
            SanitizeLoadedMoney(rec.periodWagesPaid, 0.0, 1.0e12, 0.0);
        e.periodForeignCosts =
            SanitizeLoadedMoney(rec.periodForeignCosts, 0.0, 1.0e12, 0.0);
        // M2: sanitise the persisted bidding multiplier. v1/v2 records
        // predate the field (zeroed by the caller's memset) and land on
        // the 1.0 default; a v3 value outside [1, max] is clamped to the
        // current INI bounds. The austerity flag is memory-only and is
        // re-derived at the next monthly evaluation.
        e.wageBonusMult = rec.wageBonusMult;
        if (!(e.wageBonusMult >= 1.0f)) e.wageBonusMult = 1.0f;
        if (e.wageBonusMult > g_maxPrivatePremium)
            e.wageBonusMult = g_maxPrivatePremium;
        e.inAusterity = false;
        // v8.0 (Task 3): restore insolvency state. v1..v3 records leave
        // these zeroed (memset), which loads as solvent and live; v4
        // records carry the real values. An abandoned firm loads with
        // isPrivate true here so its window can render the Status row,
        // but the decayed-flag guards in RunRegistrationPass/IsPrivateImpl
        // keep every economic loop off it and the engine's own persisted
        // ruin flag stops resurrection.
        e.insolventMonths = rec.insolventMonths;
        if (e.insolventMonths < 0) e.insolventMonths = 0;
        if (e.insolventMonths > g_monthsToAbandon + 1)
            e.insolventMonths = g_monthsToAbandon + 1;
        e.isAbandoned = (rec.isAbandoned != 0);
        // v8.8: a save written by the broken int-read build carries
        // isAbandoned=1 for LIVE dwellings (the pass misfired on
        // adjacent-field garbage at +0x1198 within 300 ms of the click).
        // The engine's own persisted ruin BYTE is the authority: a real
        // ruin has it set (insolvency latch's native swap, collapse) and
        // keeps the flag; a false positive is restored to the live
        // private firm the misfire stripped.
        if (e.isAbandoned && !TsmIsDecayed(found)) {
            e.isAbandoned = false;
            e.isPrivate   = true;
            Logf("enterprise_base  healed false-positive abandonment on "
                 "load: %s back as private firm (engine ruin flag clear)",
                 Internal_GetBuildingObjectName(found));
        }
    }

    if (matchedByCustom + matchedByPos > 0) {
        Logf("enterprise_base  deferred load matched %d by custom name, "
             "%d by type+position, %zu still pending",
             matchedByCustom, matchedByPos, stillPending.size());
        RebuildOrder();
    }

    g_pendingLoadRecords.swap(stillPending);
    g_deferredMatchAttempts++;

    if (g_pendingLoadRecords.empty()) {
        g_deferredMatchAttempts = 0;
    } else if (g_deferredMatchAttempts >= MAX_DEFERRED_ATTEMPTS) {
        for (const auto& rec : g_pendingLoadRecords) {
            Logf("enterprise_base  unmatched: custom=\"%ls\" type=\"%s\" "
                 "pos=(%.1f, %.1f)",
                 rec.customName[0] ? rec.customName : L"(none)",
                 rec.objectName, rec.posX, rec.posZ);
        }
        g_pendingLoadRecords.clear();
        g_deferredMatchAttempts = 0;
    }
}

// ---------------------------------------------------------------- v8.5 privatisation tile
//
// The TAX-style native square tile (ui_panels v3 widgetButton - the same
// widget taxes docks in City Hall's header) on every hooked building
// window. A click writes g_companies directly: no string parsing, no
// memory scan, no 300 ms pass cost. A v2 ui_panels build degrades to a
// "[ Private Enterprise ]" checkbox, exactly how taxes falls back from
// its TAX tile. State persists through OnSaveLoad/EntSaveRecord and
// rebinds on load by custom name or type name + position.
//
// Commit-on-change contract: read state under g_lock, draw the widget
// with NO lock held, commit the toggle under g_lock on the click edge.
static void ApplyPrivateState(void* building, int want)
{
    // v9.6: a click on a tile drawn on a non-building surface (possible
    // before ui_panels v6.6, which let the PRIV/PC tile draw on the citizen
    // panel) used to emplace a Person* into g_companies with
    // starting_capital. Never let that reach the registry.
    // v9.18: a click is the one moment the player's intent is knowable, so it
    // is logged BEFORE any early return. The v0.6 run shows why: two buildings
    // printed [PRIV TILE] reached and neither produced a registered/released
    // line, and nothing in the log could separate "the bar was never pressed"
    // from "pressed and refused". Now it can.
    Logf("enterprise_base  [PRIV CLICK] b=%p want=%d", building, want);

    if (!TsmLooksLikeBuilding(building)) {
        if (g_probe)
            Logf("enterprise_base  privatisation refused: %p is not a "
                 "Building - registry untouched", building);
        return;
    }

    EnterCriticalSection(&g_lock);

    // Save-record matches own creation right after a load; a click in
    // that window would cross-wire the deferred bind.
    if (!g_pendingLoadRecords.empty()) {
        LeaveCriticalSection(&g_lock);
        // v9.18: this used to be silent, which is the worst possible outcome
        // for a control the player has just pressed.
        Logf("enterprise_base  privatisation DEFERRED: a save record is still "
             "unbound - try again in a few seconds");
        return;
    }

    auto it = g_companies.find(building);

    if (want) {
        if (it == g_companies.end()) {
            Enterprise e;
            e.isPrivate = true;
            e.releasedByPlayer = false;
            e.capital   = g_startingCapital;
            e.markup    = g_defaultMarkup;
            g_companies.emplace(building, e);
            RebuildOrder();
            double cap = g_startingCapital;
            LeaveCriticalSection(&g_lock);

            float px = 0.0f, pz = 0.0f;
            ReadBuildingPosition(building, &px, &pz);
            wchar_t name[128] = {0};
            Internal_GetBuildingCustomName(building, name, 128);
            Logf("enterprise_base  registered \"%ls\" (%s) with %.2f "
                 "starting capital (panel button)",
                 name, Internal_GetBuildingObjectName(building), cap);
            Logf("enterprise_base  [PRIV CLICK] %s at pos=(%.1f, %.1f)",
                 Internal_GetBuildingObjectName(building), px, pz);
            return;
        }
        // Already private - nothing to do.
        if (it->second.isPrivate) {
            LeaveCriticalSection(&g_lock);
            return;
        }
        // v9.2: a previously-abandoned building that is now standing and
        // repaired can be re-privatized - clear the abandoned latch.
        if (it->second.isAbandoned && TsmIsFinished(building) &&
            !TsmIsDecayed(building)) {
            it->second.isAbandoned = false;
            if (g_probe)
                Logf("enterprise_base  cleared abandonment latch on "
                     "re-privatize %p", building);
        }
        it->second.isPrivate = true;
        it->second.releasedByPlayer = false;
        RebuildOrder();
        double cap = it->second.capital;
        LeaveCriticalSection(&g_lock);

        float px = 0.0f, pz = 0.0f;
        ReadBuildingPosition(building, &px, &pz);
        Logf("enterprise_base  re-privatised an existing enterprise "
             "(capital preserved at %.2f)", cap);
        Logf("enterprise_base  [PRIV CLICK] %s at pos=(%.1f, %.1f)",
             Internal_GetBuildingObjectName(building), px, pz);
        return;
    }

    // want == 0: un-privatise; capital held in reserve (pass language).
    if (it != g_companies.end() && it->second.isPrivate) {
        it->second.isPrivate = false;
        it->second.releasedByPlayer = true;      // v9.16: outranks the sweep
        RebuildOrder();
        double cap = it->second.capital;
        LeaveCriticalSection(&g_lock);
        Logf("enterprise_base  \"%s\" is no longer private; capital %.2f "
             "held in reserve (panel button)",
             Internal_GetBuildingObjectName(building), cap);
        return;
    }
    LeaveCriticalSection(&g_lock);
}

// v8.7: one-shot outcome probe - every window that reaches this provider
// leaves exactly one trace (reached / bail unreadable / bail cityhall), so
// "no tile on this window" is decidable from tesmioloader.log. Render
// thread only; the static set needs no lock.
static void TileProbeOnce(void* building, const char* stage, int type, int priv)
{
    if (!g_probe || !building) return;
    static std::unordered_set<void*> s_seen;
    if (s_seen.size() < 4096 && s_seen.insert(building).second)
        Logf("enterprise_base  [PRIV TILE] %s %p type=%d priv=%d",
             stage, building, type, priv);
}

static void EnterpriseWidgetProvider(void* building, void* /*ud*/)
{
    if (!g_panels || !g_panelRows || !building) return;
    if (!(g_panelsWidgets || g_panelsButtons)) return;
    // v9.6: "is this a Building at all?" The widget provider is called by
    // ui_panels for a window whose +0x240 slot may hold a Person* (the
    // citizen panel) - before ui_panels v6.6 it was called there with no
    // discriminator at all, and TsmBuildingType() on a Person reads its
    // FIFTH demand record (0x318) as a type descriptor. The ReadablePtr
    // test alone cannot catch that: range != readability. TsmLooksLikeBuilding
    // checks the descriptor AND its length AND the type number.
    if (!TsmLooksLikeBuilding(building)) {
        TileProbeOnce(building, "bail not a building", -1, -1);
        return;
    }
    // v8.7: the demolished/decayed gates are GONE from this provider. The
    // dispatcher routes +0xEA8 != 0 to the demolished panel 0x77C940 and
    // +0x1198 char != 0 to damage/ruin panels - none of which are among
    // the 5 hooked builders, so those windows can never call us; while
    // TsmIsDecayed's int read refused buildings the dispatcher's own char
    // read allowed into the normal chain (the v8.6 "same type, one tiles
    // one doesn't" bug). The engine's routing is the gate; the simulation
    // sweeps keep their own dem/dec checks.
    // City Hall: taxes' TAX tile owns that window's chrome; privatising a
    // council building is meaningless.
    int type = TsmBuildingType(building);
    if (type == 39) {
        TileProbeOnce(building, "bail cityhall", type, -1);
        return;
    }

    int priv = 0;
    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    if (it != g_companies.end()) priv = it->second.isPrivate ? 1 : 0;
    LeaveCriticalSection(&g_lock);

    TileProbeOnce(building, "reached", type, priv);

    if (g_panelsButtons) {
        // v9.17: ONE control everywhere. Every finished building - residence
        // included - gets the same full-width PRIV bar below the panel
        // content, with the same two captions. The residence used to keep a
        // 65-unit "PRIV" icon-row tile because that placement was visually
        // confirmed; the player asked for the bar instead, and uniformity is
        // worth more than the icon-row slot. A construction site still gets
        // the small PC tile (it has no privatisation semantics).
        int finished = 0;
        if (ReadablePtr((BYTE*)building + TSM_BLD_BUILT, sizeof(float)))
            finished = (*(const float*)((BYTE*)building + TSM_BLD_BUILT)
                        >= 1.0f) ? 1 : 0;
        if (finished) {
            if (g_panelsWide) {
                // v9.15: a full-width bar. The 40-unit toolbar tile is
                // unreadable on a small panel, and a square cannot say
                // "make this building private" without an abbreviation the
                // player has to decode.
                // v9.17: captions sized for the bar. The live font is 14
                // units, ui_panels charges 0.78 x fs per glyph, and the bar is
                // 224 px at the default 280-unit width - so about 19
                // characters fit. "MAKE IT PRIVATE" is 15 and
                // "MAKE STATE-OWNED" is 16. ui_panels v6.10 also elides
                // anything that still does not fit, which is the real
                // guarantee: a caption cannot leave its button.
                if (g_panels->widgetWideButton(
                        priv ? L"MAKE STATE-OWNED"
                             : L"MAKE IT PRIVATE",
                        priv))
                    ApplyPrivateState(building, !priv);
            } else if (g_panelsButtons && type == 2) {
                // v9.17 fallback for an older ui_panels: the residence keeps
                // its icon-row PRIV tile rather than losing the control.
                if (g_panels->widgetButton(L"PRIV", priv))
                    ApplyPrivateState(building, !priv);
            } else if (g_panelsSmall) {
                // v4.9 fallback: small PRIV tile on the top toolbar.
                if (g_panels->widgetButtonSmall(L"PRIV", priv))
                    ApplyPrivateState(building, !priv);
            }
        } else if (g_panelsSmall) {
            if (g_panels->widgetButtonSmall(L"PC", priv))
                ApplyPrivateState(building, !priv);
        }
    } else {
        unsigned hd = (unsigned)priv;
        if (g_panels->widgetCheckbox(&hd, L"[ Private Enterprise ]", 0))
            ApplyPrivateState(building, (hd != 0) ? 1 : 0);
    }
}

static void RunRegistrationPass()
{
    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;

    TsmBuildingSpan span;
    if (!TsmGetBuildings(game, &span)) return;

    EnterCriticalSection(&g_lock);

    if (!g_pendingLoadRecords.empty()) {
        ProcessDeferredLoadMatches(&span);
    }

    bool pendingMatches = !g_pendingLoadRecords.empty();
    std::unordered_set<void*> live;
    bool changed = false;

    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;

        // v8.0 (Task 3): a building the ENGINE has decayed (native
        // FUN_14013FE70 set bld+0x1198, engine-persisted in the save) is a
        // ruin, never a fresh enterprise - even if its name still carries
        // the [P] tag. This is what stops an abandoned firm resurrecting
        // every 300 ms or after a save/load cycle.
        if (TsmIsDecayed(b)) {
            auto dit = g_companies.find(b);
            if (dit != g_companies.end()) {
                if (dit->second.isPrivate) {
                    dit->second.isPrivate = false;
                    changed = true;
                }
                dit->second.isAbandoned = true;
            }
            live.insert(b);
            continue;
        }

        // v9.2: Rebuilt ruins latch - a private building abandoned as
        // ruins that is now standing and repaired should be un-abandoned
        // so it can be re-privatized via the PRIV button or re-tagging.
        {
            auto ait = g_companies.find(b);
            if (ait != g_companies.end() && ait->second.isAbandoned &&
                !TsmIsDecayed(b) && TsmIsFinished(b)) {
                ait->second.isAbandoned = false;
                changed = true;
                if (g_probe)
                    Logf("enterprise_base  rebuilt ruins un-latched %p "
                         "(isPrivate now %d)", b,
                         ait->second.isPrivate ? 1 : 0);
            }
        }

        live.insert(b);

        bool hasTag = CheckBuildingHasTag(b);
        auto it = g_companies.find(b);

        if (hasTag) {
            // v9.16: an explicit RELEASE from the panel wins over the config
            // route. Without this, all_enterprises_private + type 43 put a
            // released distribution office straight back to private.
            // (no log: this runs every sweep, and the single transition is
            // already reported by ApplyPrivateState)
            if (it != g_companies.end() && it->second.releasedByPlayer)
                continue;
            if (it == g_companies.end()) {
                if (pendingMatches) continue;

                Enterprise e;
                e.isPrivate = true;
                e.capital   = g_startingCapital;
                e.markup    = g_defaultMarkup;
                g_companies.emplace(b, e);
                changed = true;

                wchar_t name[128] = {0};
                Internal_GetBuildingCustomName(b, name, 128);
                Logf("enterprise_base  registered \"%ls\" (%s) with %.2f "
                     "starting capital",
                     name, Internal_GetBuildingObjectName(b), e.capital);
            } else if (!it->second.isPrivate && !it->second.isAbandoned) {
                it->second.isPrivate = true;
                changed = true;
                Logf("enterprise_base  re-tagged an existing enterprise "
                     "(capital preserved at %.2f)", it->second.capital);
            }
        }
        // v8.5: the "!hasTag -> un-privatise" branch is GONE. Tag detection
        // no longer drives the register (the panel PRIV tile does), so a
        // button-privatised building - which carries no tag - would have
        // been silently reverted 300 ms after every click. Un-privatisation
        // now happens only through the button, the decayed (ruins) guard
        // above, or the lost-building sweep below. Consequence: renaming a
        // [P]-tagged building no longer un-privatises it either.
    }

    for (auto& kv : g_companies) {
        if (live.count(kv.first)) continue;
        if (!kv.second.isPrivate) continue;
        kv.second.isPrivate = false;
        changed = true;
    }

    if (changed) RebuildOrder();

    LeaveCriticalSection(&g_lock);
}

// v9.9: the audit sweep, deliberately in its own function.
//
// OnTick contains __try (RunRegistrationPass). MSVC C2712 forbids __try in a
// function that also has a local requiring unwinding, and std::vector is
// exactly that - it was a build failure, not a warning. Keeping the vector's
// lifetime inside this helper keeps OnTick POD-only, which is the invariant
// the whole file already follows ("C2712 forbids __try beside C++
// unwinding" on every guarded reader above).
static void AuditSweep(void)
{
    std::vector<void*> keys;
    EnterCriticalSection(&g_lock);
    keys.reserve(g_companies.size());
    for (auto& kv : g_companies) keys.push_back(kv.first);
    LeaveCriticalSection(&g_lock);
    for (void* b : keys) AuditCapital(b, "tick sweep");
}

static void OnTick(void* /*ud*/, float /*dt*/, float /*dayDelta*/)
{
    // v7.0 (Task 2): debug hotkeys. Poll every sim frame while debug_mode is
    // on (NOT gated by the 300 ms registration debounce) and the game window
    // is the foreground window. Guarded by GetForegroundWindow() == GameWindow()
    // so no debug action can fire while another process owns the keyboard.
    // Ctrl+F9 / Ctrl+F11 are single-shot: a physical edge (high bit AND low
    // bit set) on the combined Ctrl+press.
    if (g_debugMode) {
        HWND fg = GetForegroundWindow();
        if (fg && fg == GameWindow()) {
            SHORT ctrl = GetAsyncKeyState(VK_CONTROL);
            SHORT f9   = GetAsyncKeyState(VK_F9);
            SHORT f11  = GetAsyncKeyState(VK_F11);
            if ((ctrl & 0x8000) && (ctrl & 0x0001) &&
                (f9 & 0x8000) && (f9 & 0x0001)) {
                Logf("enterprise_base  [DEBUG] Manual rent & eviction pass "
                     "triggered by Ctrl+F9");
                CollectResidentialRent();
            } else if ((ctrl & 0x8000) && (ctrl & 0x0001) &&
                       (f11 & 0x8000) && (f11 & 0x0001)) {
                DrainSampleWallets();
            }
        }
    }

    // v9.9: close an expired watch and report what the shop's capital did.
    //
    // NOTE (v9.9): the per-second audit sweep lives in AuditSweep(), NOT
    // here. This function contains __try (the registration pass below), and
    // MSVC C2712 forbids a __try in any function that also needs C++ object
    // unwinding - a std::vector local here cost one failed build. Every
    // local in this function must stay POD.
    if (g_probe) {
        DWORD nowMs = GetTickCount();
        if (g_watchShop && (long)(g_watchUntil - nowMs) <= 0) {
            double cap1 = GetCapitalImpl(g_watchShop);
            double delta = cap1 - g_watchCap0;
            Logf("enterprise_base  [WATCH %d END] shop %p  %.2f -> %.2f  "
                 "(%+.3f); %d logged event(s) during the watch; residual %+.3f",
                 g_watchId, g_watchShop, g_watchCap0, cap1, delta,
                 g_watchN, delta - g_watchSum);
            g_watchShop = nullptr;
        }

        // Independent of any watch: sweep every building about once a second.
        // A divergence here is a write that did not pass through the trace.
        static DWORD lastSweep = 0;
        if (nowMs - lastSweep > 1000) {
            lastSweep = nowMs;
            AuditSweep();
        }
    }

    static DWORD lastPass = 0;
    DWORD now = GetTickCount();
    if (now - lastPass < 300) return;
    lastPass = now;

    __try { RunRegistrationPass(); }
    __except (FaultFilter("enterprise_base registration",
                          GetExceptionInformation()))
    {
        Logf("enterprise_base  registration pass faulted - skipped this frame");
    }
}

// ---------------------------------------------------------------- calendar events

static void OnMonth(void* /*ud*/, int /*day*/, int month, int year)
{
    // v7 (Task 3): reset the monthly eviction counter at the month boundary
    // so it counts only successful evictions executed during the current
    // month.
    ResetMonthlyEvictionCount();

    EnterCriticalSection(&g_lock);
    if (g_lastPoolMonth != month || g_lastPoolYear != year) {
        g_lastMonthPoolInflow = g_monthlyPoolInflow;
        g_monthlyPoolInflow   = 0.0;
        g_lastPoolMonth = month;
        g_lastPoolYear  = year;
    }
    LeaveCriticalSection(&g_lock);

    // v5.4: rent first (counts as this month's revenue), then siphon
    // takes only net positive operating profit.
    CollectResidentialRent();
    SiphonToPool(month, year);

    // M2: wage bidding runs after the siphon (which may reduce capital)
    // and BEFORE the clear loop below zeroes the monthly accumulators.
    // Game memory reads (staffing, bbox) are fault-bracketed like the
    // registration pass.
    __try {
        EvaluateWageBidding();
    }
    __except (FaultFilter("enterprise_base wage bidding",
                          GetExceptionInformation()))
    {
        Logf("enterprise_base  wage bidding pass faulted - skipped this month");
    }

    // v8.0 (Task 3): phased insolvency runs after wage bidding (both read
    // e.capital; neither depends on the accumulators the clear loop below
    // zeroes). Its native abandonment call is SEH-isolated and runs after
    // EvaluateInsolvency releases g_lock.
    EvaluateInsolvency();

    static bool loggedOnce = false;
    EnterCriticalSection(&g_lock);
    {
        double closedSales = 0.0, closedWages = 0.0, closedTaxes = 0.0;
        for (auto& kv : g_companies) {
            Enterprise& e = kv.second;
            if (e.isPrivate) {
                closedSales += e.monthlyGrossSales;
                closedWages += e.monthlyWagesPaid;
                closedTaxes += e.monthlyTaxesPaid;
            }
            e.monthlyGrossSales     = 0.0;
            e.monthlyWholesaleCosts = 0.0;
            e.monthlyWagesPaid      = 0.0;
            e.monthlyTaxesPaid      = 0.0;
            // v6.1: monthlyDividendPaid is deliberately NOT cleared here -
            // this loop runs right after SiphonToPool writes it, which
            // is exactly why the panels always showed div +0.00. Its
            // reset lives in SiphonToPool, at the start of the next
            // cycle (after the same-month skip guard).
        }
        g_lastMonthSales = closedSales;
        g_lastMonthWages = closedWages;
        g_lastMonthTaxes = closedTaxes;
    }
    bool announce = (!loggedOnce && g_probe);
    if (announce) loggedOnce = true;
    LeaveCriticalSection(&g_lock);

    if (announce)
        Logf("enterprise_base  monthly accumulators cleared for %zu "
             "enterprise(s)", g_companies.size());
}

// ---------------------------------------------------------------- the card

static float DrawEnterpriseCard(void* /*ud*/, float x, float y, float step)
{
    if (!g_currency) return y;

    EnterCriticalSection(&g_lock);

    size_t count = 0;
    double totalCapital = 0.0;
    double totalSales   = 0.0;
    double totalWages   = 0.0;
    double totalTaxes   = 0.0;
    for (auto& kv : g_companies) {
        if (!kv.second.isPrivate) continue;
        count++;
        totalCapital += kv.second.capital;
        totalSales   += kv.second.monthlyGrossSales;
        totalWages   += kv.second.monthlyWagesPaid;
        totalTaxes   += kv.second.monthlyTaxesPaid;
    }
    double pool = g_pool;
    double inflow = g_monthlyPoolInflow;

    double closedSales = g_lastMonthSales;
    double closedWages = g_lastMonthWages;
    double closedTaxes = g_lastMonthTaxes;
    double closedPool  = g_lastMonthPoolInflow;

    LeaveCriticalSection(&g_lock);

    wchar_t buf[256];
    const wchar_t* sym = (IsUsdActive() ? L"USD" : L"RUB");
    wchar_t symBuf[16];
    _snwprintf_s(symBuf, _countof(symBuf), _TRUNCATE, L"%ls", sym);

    g_ui->drawTextOutlined(x, y, 0xFF70C8FF,
        L"=== PRIVATE CAPITAL & COMMERCIAL SECTOR ===");
    y += step;

    _snwprintf_s(buf, _countof(buf), _TRUNCATE,
        L" Capitalist Pool:  %.2f %ls  (this month +%.2f)  |  "
        L"Private Cash: %.2f",
        pool, symBuf, inflow, totalCapital);
    g_ui->drawTextOutlined(x, y, 0xFF90F090, buf);
    y += step;

    _snwprintf_s(buf, _countof(buf), _TRUNCATE,
        L" This month:  Sales +%.2f  Wages -%.2f  Taxes -%.2f  (%zu firms)",
        totalSales, totalWages, totalTaxes, count);
    g_ui->drawTextOutlined(x, y, 0xFFDCDCDC, buf);
    y += step;

    _snwprintf_s(buf, _countof(buf), _TRUNCATE,
        L" Prev month:  Sales +%.2f  Wages -%.2f  Taxes -%.2f  pool +%.2f",
        closedSales, closedWages, closedTaxes, closedPool);
    g_ui->drawTextOutlined(x, y, 0xFFB0B0B0, buf);
    y += step;

    g_ui->drawTextOutlined(x, y, 0xFF70C8FF,
        L"---------------------------------------------------------");
    y += step;

    return y;
}

// ---------------------------------------------------------------- persistence

static void SaveRegister(const char* dir)
{
    std::vector<EntSaveRecord> records;
    EnterCriticalSection(&g_lock);

    records.reserve(g_companies.size());

    for (auto& kv : g_companies) {
        if (!kv.second.isPrivate) continue;

        float px = 0.0f, pz = 0.0f;
        ReadBuildingPosition(kv.first, &px, &pz);
        if (g_probe)
            Logf("enterprise_base [SAVE POS] %s at pos=(%.1f, %.1f)",
                 Internal_GetBuildingObjectName(kv.first), px, pz);

        EntSaveRecord rec;
        memset(&rec, 0, sizeof(rec));
        strncpy_s(rec.objectName, sizeof(rec.objectName),
                  Internal_GetBuildingObjectName(kv.first), _TRUNCATE);
        rec.posX                 = px;
        rec.posZ                 = pz;
        if (g_probe)
            Logf("enterprise_base  [SAVE POS] %s at pos=(%.1f, %.1f)",
                 rec.objectName, rec.posX, rec.posZ);
        rec.capital              = SanitizeLoadedMoney(kv.second.capital, -1.0e12, 1.0e12, 0.0);
        rec.markup               = kv.second.markup;
        rec.accruedVat           = kv.second.accruedVatLiability;
        rec.periodGrossSales     = kv.second.periodGrossSales;
        rec.periodVatAccrued     = kv.second.periodVatAccrued;
        rec.periodWholesaleCosts = kv.second.periodWholesaleCosts;
        rec.periodWagesPaid      = kv.second.periodWagesPaid;
        rec.periodForeignCosts   = kv.second.periodForeignCosts;
        rec.accruedProfitTax     = kv.second.accruedProfitTaxLiability;
        Internal_GetBuildingCustomName(kv.first, rec.customName, 128);
        rec.wageBonusMult = kv.second.wageBonusMult;
        rec.insolventMonths = kv.second.insolventMonths;
        rec.isAbandoned     = kv.second.isAbandoned ? 1 : 0;

        records.push_back(rec);
    }

    size_t count = records.size();
    size_t total = sizeof(EntSaveHeader) + count * sizeof(EntSaveRecord);
    std::vector<BYTE> buf(total);

    EntSaveHeader hdr;
    memcpy(hdr.magic, ENT_MAGIC, 8);
    hdr.version     = ENT_VERSION;
    hdr.pool        = g_pool;
    hdr.recordCount = (uint32_t)count;
    hdr.reserved    = 0;
    memcpy(buf.data(), &hdr, sizeof(hdr));

    if (count > 0)
        memcpy(buf.data() + sizeof(hdr), records.data(),
               count * sizeof(EntSaveRecord));

    LeaveCriticalSection(&g_lock);

    if (g_persistence->writeAtomic(dir, ENT_FILENAME, buf.data(), total))
        Logf("enterprise_base  saved pool %.2f and %zu enterprise(s) to %s\\%s",
             hdr.pool, count, dir, ENT_FILENAME);
    else
        Logf("enterprise_base  WARN  could not write %s\\%s",
             dir, ENT_FILENAME);
}

// Caller holds g_lock. A missing or unusable sidecar must leave the
// register empty: the entries are keyed by raw building pointers, so
// state from the previous world is both stale and aliasing-prone.
static void ResetRegisterStateLocked(void)
{
    g_companies.clear();
    g_companyOrder.clear();
    g_pool = g_startingPool;
    g_monthlyPoolInflow   = 0.0;
    g_lastMonthPoolInflow = 0.0;
    g_lastMonthSales      = 0.0;
    g_lastMonthWages      = 0.0;
    g_lastMonthTaxes      = 0.0;
    g_monthlyEvictionCount = 0;
    g_lastPoolMonth = -1;
    g_lastPoolYear  = -1;
    g_pendingLoadRecords.clear();
    g_deferredMatchAttempts = 0;
}

static void LoadRegister(const char* dir)
{
    size_t len = 0;
    void* raw = g_persistence->readFile(dir, ENT_FILENAME, &len);
    if (!raw) {
        EnterCriticalSection(&g_lock);
        ResetRegisterStateLocked();
        LeaveCriticalSection(&g_lock);

        Logf("enterprise_base  no %s in this save - register cleared, "
             "pool at %.0f", ENT_FILENAME, g_startingPool);
        return;
    }

    if (len < sizeof(EntSaveHeader)) {
        Logf("enterprise_base  %s too short - ignored", ENT_FILENAME);
        EnterCriticalSection(&g_lock);
        ResetRegisterStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    EntSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));

    size_t recordSize = 0;
    if (hdr.version == ENT_VERSION) {
        recordSize = sizeof(EntSaveRecord);
    } else if (hdr.version == ENT_VERSION_V3) {
        // v3 stops before the Task-3 insolventMonths/isAbandoned tail.
        recordSize = offsetof(EntSaveRecord, insolventMonths);
    } else if (hdr.version == ENT_VERSION_V2) {
        // v2 stops before the M2 wageBonusMult field.
        recordSize = offsetof(EntSaveRecord, wageBonusMult);
    } else if (hdr.version == ENT_VERSION_LEGACY) {
        recordSize = offsetof(EntSaveRecord, customName);
    } else {
        Logf("enterprise_base  %s version %u, expected %u..%u - ignored",
             ENT_FILENAME, hdr.version, ENT_VERSION_LEGACY, ENT_VERSION);
        EnterCriticalSection(&g_lock);
        ResetRegisterStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    if (memcmp(hdr.magic, ENT_MAGIC, 8) != 0) {
        Logf("enterprise_base  %s has the wrong magic - ignored",
             ENT_FILENAME);
        EnterCriticalSection(&g_lock);
        ResetRegisterStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.recordCount * recordSize;
    if (want > len) {
        Logf("enterprise_base  %s claims %u records but is only %zu bytes",
             ENT_FILENAME, hdr.recordCount, len);
        EnterCriticalSection(&g_lock);
        ResetRegisterStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);

    g_companies.clear();
    g_companyOrder.clear();

    g_pool = hdr.pool;
    if (!(g_pool >= 0.0) || !(g_pool <= 1.0e15)) g_pool = g_startingPool;
    g_monthlyPoolInflow   = 0.0;
    g_lastMonthPoolInflow = 0.0;
    g_lastMonthSales      = 0.0;
    g_lastMonthWages      = 0.0;
    g_lastMonthTaxes      = 0.0;
    g_monthlyEvictionCount = 0;
    g_lastPoolMonth = -1;
    g_lastPoolYear  = -1;

    g_pendingLoadRecords.clear();
    g_pendingLoadRecords.reserve(hdr.recordCount);
    g_deferredMatchAttempts = 0;

    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.recordCount; i++) {
        EntSaveRecord rec;
        memset(&rec, 0, sizeof(rec));
        memcpy(&rec, p, recordSize);
        p += recordSize;
        g_pendingLoadRecords.push_back(rec);
    }

    Logf("enterprise_base  loaded pool %.2f, staged %u enterprise(s) "
         "(format v%u)", hdr.pool, hdr.recordCount, hdr.version);

    LeaveCriticalSection(&g_lock);
    free(raw);
}

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir || !g_persistence) return;
    if      (ev == TSM_SAVE_WRITING) SaveRegister(dir);
    else if (ev == TSM_SAVE_LOADING) LoadRegister(dir);
}

// ---------------------------------------------------------------- the API

static int IsPrivateImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return 0;
    // v8.0 (Task 3): ruins are not private enterprises, even if the [P]
    // tag lingers on the name.
    if (TsmIsDecayed(building)) return 0;

    EnterCriticalSection(&g_lock);
    auto it = g_companies.find(building);
    int in = (it != g_companies.end() && it->second.isPrivate);
    LeaveCriticalSection(&g_lock);
    if (in) return 1;

    return CheckBuildingHasTag(building) ? 1 : 0;
}

static size_t GetCountImpl(void)
{
    EnterCriticalSection(&g_lock);
    size_t n = g_companyOrder.size();
    LeaveCriticalSection(&g_lock);
    return n;
}

static void* GetByIndexImpl(size_t i)
{
    EnterCriticalSection(&g_lock);
    void* p = (i < g_companyOrder.size()) ? g_companyOrder[i] : nullptr;
    LeaveCriticalSection(&g_lock);
    return p;
}

static int GetCustomNameImpl(void* b, wchar_t* out, size_t maxChars)
{
    return Internal_GetBuildingCustomName(b, out, maxChars) ? 1 : 0;
}

static const char* GetObjectNameImpl(void* b)
{
    return Internal_GetBuildingObjectName(b);
}

static void ChargeWholesaleImpl(void* building, double cost)
{
    if (!TsmIsValidGamePtr(building)) return;
    if (!(cost > 0.0)) return;

    (void)TryModifyCapital(building, -cost, TSM_CAP_WHOLESALE,
                           "wholesale(api)");
    g_currency->moveTreasury(cost, "wholesale_b2b");
}

static size_t GetMonthlyEvictionCountImpl(void)
{
    return g_monthlyEvictionCount;
}

// v8 (Task 7): the residential rent SETTING - per tenant per month, in
// the ACTIVE currency units resolved at Start (USD-scaled when USD is
// active). taxes divides it by sim_clock getDaysPerMonth for the true
// average daily rent injected into the CPI basket. Plain read of a
// Start-resolved constant: no lock, no fault path.
static double GetResidentialRentMonthlyImpl(void)
{
    return g_residentialRent;
}

static const TsmEnterpriseBaseApi kApi = {
    sizeof(TsmEnterpriseBaseApi),
    IsPrivateImpl,
    GetCountImpl,
    GetByIndexImpl,
    GetObjectNameImpl,
    GetCustomNameImpl,
    GetCapitalImpl,
    ModifyCapitalImpl,
    IsSolventImpl,
    GetMarkupImpl,
    SetMarkupImpl,
    ChargeWholesaleImpl,
    GetAccrualsImpl,
    AddAccrualImpl,
    ClearAccrualImpl,
    GetPeriodTotalsImpl,
    ClearPeriodTotalsImpl,
    GetPoolImpl,
    ModifyPoolImpl,
    GetMonthlyPoolInflowImpl,
    GetMonthlyEconomySummaryImpl,
    DescribeEnterpriseImpl,
    GetMonthlyEvictionCountImpl,
    GetResidentialRentMonthlyImpl,
};

// ---------------------------------------------------------------- settings

static void ParseAutoTypes(const char* s)
{
    g_autoTypeCount = 0;
    if (!s || !s[0]) {
        g_autoTypes[g_autoTypeCount++] = 3;
        g_autoTypes[g_autoTypeCount++] = 6;
        g_autoTypes[g_autoTypeCount++] = 8;
        return;
    }
    while (*s && g_autoTypeCount < 16) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        g_autoTypes[g_autoTypeCount++] = atoi(s);
        while (*s && *s != ',') s++;
    }
}

// Money-valued config fields feed capital, the pool, and rent directly.
// The INI/USD parsers accept atof() output, so inf/absurd figures must be
// re-bounded here (NaN fails the lower test, inf fails the upper one).
// Called once after the base values are read and again after the USD
// overrides, which are parsed later and bypass the first pass.
static void SanitizeMoneyConfig()
{
    if (!(g_startingCapital >= 0.0) ||
        !(g_startingCapital <= 1.0e12))  g_startingCapital = 0.0;
    if (!(g_defaultMarkup > 0.0f) ||
        !(g_defaultMarkup <= 100.0f))    g_defaultMarkup   = 1.0f;
    if (!(g_startingPool >= 0.0) ||
        !(g_startingPool <= 1.0e15))     g_startingPool = 0.0;
    if (!(g_legalReserveFlat >= 0.0) ||
        !(g_legalReserveFlat <= 1.0e15)) g_legalReserveFlat = 0.0;
    if (!(g_residentialRent >= 0.0) ||
        !(g_residentialRent <= 1.0e9))   g_residentialRent = 10.0;
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\enterprise_base.ini";
    char v[64];

    g_enabled = H->configInt(ini, "enterprise_base", "enabled", 1);
    g_probe   = H->configInt(ini, "enterprise_base", "probe",   1);
    g_card    = H->configInt(ini, "enterprise_base", "card",    1);
    g_panelRows = H->configInt(ini, "enterprise_base", "panel_rows", 1);

    g_allPrivate = H->configInt(ini, "enterprise_base",
                                "all_enterprises_private", 0);
    g_allowDebt  = H->configInt(ini, "enterprise_base", "allow_debt", 0);

    // v7.0 (Task 2): developer debug hotkeys. 1 = poll Ctrl+F9 / Ctrl+F11 in
    // OnTick (guarded by GetForegroundWindow() == GameWindow()); 0 disables
    // them so a build can ship with hardkeys off.
    g_debugMode = H->configInt(ini, "enterprise_base", "debug_mode", 1);

    // v4: keep the RUB-denominated figures as written; USD-effective
    // values resolve at Start once the active currency is known.
    g_startingCapitalCfg  = 10000.0;
    g_residentialRentCfg  = 10.0;
    if (H->configString(ini, "enterprise_base", "starting_capital",
                        v, sizeof(v), "") && v[0])
        g_startingCapitalCfg = atof(v);
    if (H->configString(ini, "enterprise_base", "default_markup",
                        v, sizeof(v), "") && v[0])
        g_defaultMarkup = (float)atof(v);
    if (H->configString(ini, "enterprise_base", "private_wage_bonus",
                        v, sizeof(v), "") && v[0])
        g_privateWageBonus = (float)atof(v);

    // M2: the private wage market ([private_market_wages]). The static
    // private_wage_bonus above is only the off-switch fallback.
    g_dynamicBidding = H->configInt(ini, "private_market_wages",
                                    "dynamic_bidding_enabled", 1);
    g_maxPrivatePremium     = 1.50f;
    g_understaffedThreshold = 0.75f;
    if (H->configString(ini, "private_market_wages", "max_private_premium",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f == f) g_maxPrivatePremium = f;
    }
    if (H->configString(ini, "private_market_wages", "understaffed_threshold",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f == f) g_understaffedThreshold = f;
    }
    if (H->configString(ini, "private_market_wages", "wage_bidding_step",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f == f) g_wageBiddingStep = f;
    }
    // v9.13: 0 restores pre-v9.13 behaviour (pay whatever citizen_labor
    // computes, however far above the posted rate).
    g_wageCapEnabled = H->configInt(ini, "private_market_wages",
                                    "wage_cap_enabled", 1);
    if (H->configString(ini, "private_market_wages",
                        "wage_rate_ceiling_multiple",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f == f) g_wageRateCeiling = f;
    }
    if (H->configString(ini, "enterprise_base", "residential_rent",
                        v, sizeof(v), "") && v[0])
        g_residentialRentCfg = atof(v);
    g_startingCapital = g_startingCapitalCfg;
    g_residentialRent = g_residentialRentCfg;

    // v8.0 (Task 1): attractiveness-scaled rent. Pure multipliers over the
    // (possibly USD-scaled) base - no currency conversion needed here.
    g_rentMinMult = 0.50f;
    g_rentMaxMult = 2.00f;
    if (H->configString(ini, "enterprise_base", "rent_attr_min_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_rentMinMult = f;
    }
    if (H->configString(ini, "enterprise_base", "rent_attr_max_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_rentMaxMult = f;
    }

    // v8.2 (Task 1): economic eviction & park-cap tunables.
    g_evictionEnabled = H->configInt(ini, "enterprise_base",
                                     "eviction_enabled", 1);
    g_parkCapEnabled = H->configInt(ini, "enterprise_base",
                                    "park_cap_enabled", 1);
    g_rentBurdenThreshold = 0.35f;
    if (H->configString(ini, "enterprise_base", "rent_burden_threshold",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_rentBurdenThreshold = f;
    }
    g_evictionPenaltyHappiness = 0.15f;
    if (H->configString(ini, "enterprise_base", "eviction_penalty_happiness",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_evictionPenaltyHappiness = f;
    }

    // v9.1 (Task 7): destitute eviction stage ([destitute_eviction]).
    // bread_tons mirrors resource_pricing's [cpi_basket]
    // basket_food_tons - keep the two in step so the rent pass stages on
    // the same "one day of bread" mark retail's runway implies.
    g_destituteEnabled = H->configInt(ini, "destitute_eviction",
                                      "enabled", 1);
    g_destituteBreadTons = 0.0020f;
    if (H->configString(ini, "destitute_eviction", "bread_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_destituteBreadTons = f;
    }

    // v8.0 (Task 3): phased-insolvency tunables ([insolvency]).
    g_abandonEnabled = H->configInt(ini, "insolvency",
                                    "abandonment_enabled", 1);
    g_monthsToAbandon = H->configInt(ini, "insolvency",
                                     "months_to_abandon", 3);
    g_auctionBuyoutMult = 1.5f;
    if (H->configString(ini, "insolvency", "auction_buyout_cost_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 100.0f && f == f) g_auctionBuyoutMult = f;
    }

    g_usdDivisor = 50.0;
    if (H->configString(ini, "enterprise_base", "usd_divisor",
                        v, sizeof(v), "") && v[0]) {
        double d = atof(v);
        if (d >= 1.0 && d <= 1000.0) g_usdDivisor = d;
    }

    if (H->configString(ini, "enterprise_base", "auto_private_types",
                        v, sizeof(v), "") && v[0])
        ParseAutoTypes(v);
    else
        ParseAutoTypes(nullptr);

    g_poolEnabled = H->configInt(ini, "investment_pool", "enabled", 1);
    g_startingPoolCfg     = 50000.0;
    g_legalReserveFlatCfg = 5000.0;
    if (H->configString(ini, "investment_pool", "starting_pool",
                        v, sizeof(v), "") && v[0])
        g_startingPoolCfg = atof(v);
    if (H->configString(ini, "investment_pool", "legal_reserve_flat",
                        v, sizeof(v), "") && v[0])
        g_legalReserveFlatCfg = atof(v);
    g_startingPool     = g_startingPoolCfg;
    g_legalReserveFlat = g_legalReserveFlatCfg;
    if (H->configString(ini, "investment_pool", "siphon_rate",
                        v, sizeof(v), "") && v[0])
        g_siphonRate = (float)atof(v);
    if (H->configString(ini, "investment_pool", "payroll_buffer_months",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (std::isfinite(f) && f >= 0.0f && f <= 120.0f)
            g_payrollBufferMonths = f;
    }
    if (H->configString(ini, "investment_pool", "wholesale_buffer_months",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (std::isfinite(f) && f >= 0.0f && f <= 120.0f)
            g_wholesaleBufferMonths = f;
    }

    SanitizeMoneyConfig();
    if (!(g_privateWageBonus > 0.0f))   g_privateWageBonus= 1.0f;
    if (!(g_maxPrivatePremium >= 1.0f)) g_maxPrivatePremium = 1.50f;
    if (g_maxPrivatePremium > 3.0f)     g_maxPrivatePremium = 3.0f;
    if (!(g_understaffedThreshold > 0.0f) ||
        g_understaffedThreshold > 1.0f) g_understaffedThreshold = 0.75f;
    if (!(g_wageBiddingStep >= 0.01f))  g_wageBiddingStep = M2_WAGE_STEP_DEFAULT;
    if (g_wageBiddingStep > 0.50f)      g_wageBiddingStep = 0.50f;
    if (!(g_residentialRent >= 0.0))    g_residentialRent = 10.0;
    if (!(g_siphonRate >= 0.0f) ||
        !(g_siphonRate <= 1.0f))        g_siphonRate      = 0.5f;
    // v8.0 (Task 1): keep the rent band sane - both finite, non-negative,
    // and min never above max (swap rather than clamp so a typo preserves
    // the intent).
    if (!(g_rentMinMult >= 0.0f)) g_rentMinMult = 0.50f;
    if (!(g_rentMaxMult >= 0.0f)) g_rentMaxMult = 2.00f;
    if (g_rentMinMult > g_rentMaxMult) {
        float t = g_rentMinMult;
        g_rentMinMult = g_rentMaxMult;
        g_rentMaxMult = t;
    }
    // v8.2 (Task 1): burden threshold strictly inside (0, 1] (0 would
    // evict everyone with a positive rent; negatives are nonsense), the
    // happiness dent inside [0, 1] (it is a 0..1 float).
    if (!(g_rentBurdenThreshold > 0.0f) || g_rentBurdenThreshold > 1.0f)
        g_rentBurdenThreshold = 0.35f;
    if (!(g_evictionPenaltyHappiness >= 0.0f) ||
        g_evictionPenaltyHappiness > 1.0f)
        g_evictionPenaltyHappiness = 0.15f;
    // v8.0 (Task 3): at least one month of distress before ruin; cap the
    // auction multiplier so a fat-fingered 1000000x cannot drain the pool.
    if (g_monthsToAbandon < 1)   g_monthsToAbandon = 1;
    if (g_monthsToAbandon > 60)  g_monthsToAbandon = 60;
    if (g_auctionBuyoutMult < 1.0f) g_auctionBuyoutMult = 1.0f;
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "enterprise_base";
    info->version = "9.20";

    g_simClock = (const TsmSimClockApi*)
        H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);

    ReadSettings();
    if (!g_enabled) {
        Logf("enterprise_base  disabled");
        return 1;
    }

    g_pool = g_startingPool;

    // v8 ABI (Task 7): publish BOTH the full v8 iface and a v7-prefix
    // view, so an out-of-tree consumer built against the v7 header reads
    // exactly the prefix it expects. In-tree consumers (taxes,
    // private_shops, private_construction, citizen_labor) all rebuild
    // against this header via build.bat and ask for v8.
    static TsmEnterpriseBaseApi kApiV7;
    memcpy(&kApiV7, &kApi, sizeof(kApiV7));
    kApiV7.structSize = (unsigned)offsetof(TsmEnterpriseBaseApi,
                                           getResidentialRentMonthly);
    H->provide(TSM_SERVICE_ENTERPRISE_BASE,
               TSM_ENTERPRISE_BASE_VERSION_V7, &kApiV7);
    H->provide(TSM_SERVICE_ENTERPRISE_BASE,
               TSM_ENTERPRISE_BASE_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_simClock)
        g_simClock = (const TsmSimClockApi*)
            H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("enterprise_base  sim_clock is not installed");
        return 1;
    }

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only
    // routing) so a mixed install still runs.
    g_currency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) {
        Logf("enterprise_base  currency is not installed");
        return 1;
    }

    // v4 USD scale calibration: flat figures divide the RUB bases by
    // usd_divisor unless an explicit *_usd key exists. Runs at Start
    // (before any world load writes the pool) so a fresh USD game does
    // not start with a 5,000,000 USD investment pool.
    if (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD) {
        const char* ini = "plugins\\enterprise_base.ini";
        char v[64];
        double d = UsdScale();
        if (H->configString(ini, "enterprise_base", "starting_capital_usd",
                            v, sizeof(v), "") && v[0]) {
            double u = atof(v);
            if (u >= 0.0 && u == u) g_startingCapital = u;
        } else {
            g_startingCapital = g_startingCapitalCfg / d;
        }
        if (H->configString(ini, "enterprise_base", "residential_rent_usd",
                            v, sizeof(v), "") && v[0]) {
            double u = atof(v);
            if (u >= 0.0 && u == u) g_residentialRent = u;
        } else {
            g_residentialRent = g_residentialRentCfg / d;
        }
        if (H->configString(ini, "investment_pool", "starting_pool_usd",
                            v, sizeof(v), "") && v[0]) {
            double u = atof(v);
            if (u >= 0.0 && u == u) g_startingPool = u;
        } else {
            g_startingPool = g_startingPoolCfg / d;
        }
        if (H->configString(ini, "investment_pool", "legal_reserve_flat_usd",
                            v, sizeof(v), "") && v[0]) {
            double u = atof(v);
            if (u >= 0.0 && u == u) g_legalReserveFlat = u;
        } else {
            g_legalReserveFlat = g_legalReserveFlatCfg / d;
        }
        // The USD overrides are parsed after ReadSettings' first pass, so
        // re-bound the resolved values before they seed anything.
        SanitizeMoneyConfig();
        // No save has loaded yet at Start - re-seed the fresh-game pool.
        g_pool = g_startingPool;
        Logf("enterprise_base  v4 USD scale: capital %.2f, pool %.2f, "
             "reserve %.2f, rent %.2f (divisor %.1f)",
             g_startingCapital, g_startingPool, g_legalReserveFlat,
             g_residentialRent, d);
    }

    g_ui = (const TsmUiOverlayApi*)
        H->consume(TSM_SERVICE_UI_OVERLAY, TSM_UI_OVERLAY_VERSION);
    if (!g_ui)
        Logf("enterprise_base  ui_overlay is not installed");

    g_panels = (const TsmUiPanelsApi*)
        H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_panels)
        Logf("enterprise_base  ui_panels is not installed - no in-window rows");
    g_persistence = (const TsmPersistenceApi*)
        H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence) {
        Logf("enterprise_base  WARN  no persistence service");
    } else {
        g_persHandle = g_persistence->registerHandler(
            "enterprise_base", OnSaveLoad, nullptr);
        if (g_persHandle <= 0)
            Logf("enterprise_base  WARN  persistence refused the handler");
    }

    // M2: prefer the v3 labour iface (statutory floor + wage overrides);
    // fall back to v2 so a mixed install still intercepts wages - the
    // v2 path just has no floor accessor (citizen_labor's own resolver
    // already floors the grossWage it hands us).
    g_labor = (const TsmCitizenLaborApi*)
        H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION);
    if (!g_labor)
        g_labor = (const TsmCitizenLaborApi*)
            H->consume(TSM_SERVICE_CITIZEN_LABOR,
                       TSM_CITIZEN_LABOR_VERSION_V2);
    if (g_labor) {
        g_labor->registerPayrollInterceptor(OnPayroll, nullptr);
        if (!TsmLaborHasV3(g_labor))
            Logf("enterprise_base  WARN  citizen_labor v3 not available - "
                 "wages will not be floored to the statutory minimum");
        // Wage-mobility hook presence is verified lazily via
        // DynamicBiddingActive() after all plugins finish TsmPluginStart.
    } else {
        Logf("enterprise_base  WARN  citizen_labor not available - "
             "private wages will not be intercepted");
    }

    g_retail = (const TsmCitizenRetailApi*)
        H->consume(TSM_SERVICE_CITIZEN_RETAIL, TSM_CITIZEN_RETAIL_VERSION);
    if (g_retail)
        g_retail->registerRetailInterceptor(OnRetail, nullptr);

    g_taxes = (const TsmTaxesApi*)
        H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION);
    // v9.19: retain older taxes providers after the v3 snapshot API bump.
    if (!g_taxes)
        g_taxes = (const TsmTaxesApi*)H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V2);
    if (!g_taxes)
        g_taxes = (const TsmTaxesApi*)H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V1);

    // The food price for the destitute bread-day. Optional - when
    // resource_pricing is absent the destitute stage is disabled
    // (destituteDay stays 0) and nothing else changes.
    g_pricing = (const TsmResourcePricingApi*)
        H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)
            H->consume(TSM_SERVICE_RESOURCE_PRICING,
                       TSM_RESOURCE_PRICING_VERSION_V1);
    // v9.1 (Task 7): the destitute bread-day needs a live food price.
    // Absent pricing the stage is inert - burden/short-pay still govern.
    if (g_destituteEnabled && !g_pricing)
        Logf("enterprise_base  WARN  resource_pricing not installed - the "
             "destitute eviction stage is inert (burden/short tests only)");

    g_simClock->registerTick(OnTick, nullptr);
    TsmSimClockRegisterScheduledPhase(g_simClock, TSM_FREQ_MONTHLY,
                                      TSM_PHASE_ENTERPRISE_CLOSE,
                                      OnMonth, nullptr);

    if (g_ui && g_card) g_ui->registerDrawer(DrawEnterpriseCard, nullptr);

    if (g_panels && g_panelRows) {
        if (g_panels->registerRowProvider(EnterprisePanelRows, nullptr))
            Logf("enterprise_base  in-window enterprise rows registered");
        else
            Logf("enterprise_base  ui_panels refused the row provider");

        // v8.5: the privatisation tile - TAX-style native square button on
        // every hooked building window (checkbox on a v2 ui_panels build).
        if (TsmUiPanelsHasWidgets(g_panels)) {
            if (g_panels->registerWidgetProvider(EnterpriseWidgetProvider,
                                                 nullptr)) {
                g_panelsWidgets = 1;
                g_panelsButtons = TsmUiPanelsHasButton(g_panels) ? 1 : 0;
                g_panelsSmall   = TsmUiPanelsHasButtonSmall(g_panels) ? 1 : 0;
                g_panelsWide    = TsmUiPanelsHasWideButton(g_panels) ? 1 : 0;
                Logf("enterprise_base  privatisation tile registered (%s)",
                     g_panelsWide    ? "full-width bar on every finished "
                                       "building"
                     : g_panelsButtons ? "native square button"
                                       : "checkbox fallback");
            } else {
                Logf("enterprise_base  WARN  ui_panels refused the widget "
                     "provider - no privatisation tile");
            }
        } else {
            Logf("enterprise_base  WARN  ui_panels has no widget tail - "
                 "no privatisation tile");
        }
    }

    Logf("enterprise_base  live v9.20 (unified currency %s, privatisation bar everywhere, net-profit siphon, attractiveness rent, park-cap, economic eviction + destitute bread-day, atomic retail charge+credit, in-window rows, wage bidding, phased insolvency, native prices (no market multiplier), full-private-price retail pre-check, refusal-triggered capital watch, honest audit baseline, payroll affordability, wage provenance, visible revenue, wage contract cap %s, rate ceiling %.0fx minimum)",
         TsmCurrencySymbol(g_currency), g_wageCapEnabled ? "on" : "off",
         (double)g_wageRateCeiling);
    Logf("enterprise_base  *** BUILD v9.20 - no market multiplier, wholesale income counts as revenue, every privatise click logged ***");
    Logf("enterprise_base      [WATCH n]      armed by a refusal; logs every movement on that shop for %d ms", g_watchMs);
    Logf("enterprise_base      [WATCH n CAP]  a capital movement on the watched shop, with by=<caller>");
    Logf("enterprise_base      [WATCH n SALE] a completed sale on the watched shop, with the buyer");
    Logf("enterprise_base      [WATCH n END]  the shop's balance, the movement count, and the residual gap");
    Logf("enterprise_base      [AUDIT GAP]    capital changed without a traced operation - this is a bug");
    Logf("enterprise_base  Task 1 rent: base %.2f x attr %.2f..%.2f; Task 3 insolvency: %s, ruin after %d month(s), buyout %.2fx",
         g_residentialRent, g_rentMinMult, g_rentMaxMult,
         g_abandonEnabled ? "on" : "off", g_monthsToAbandon,
         g_auctionBuyoutMult);
    Logf("enterprise_base  Task 1 eviction: %s, burden > %.0f%% of wallet, happiness dent %.2f, park cap %s",
         g_evictionEnabled ? "on" : "off",
         (double)g_rentBurdenThreshold * 100.0,
         (double)g_evictionPenaltyHappiness,
         g_parkCapEnabled ? "on" : "off");
    Logf("enterprise_base  M2 wage bidding: %s, premium cap +%.0f%%, "
         "understaffed < %.0f%%, step +/-%.1f%%, wage floor %s",
         g_dynamicBidding ? "enabled (pending post-start hook check)" : "off",
         (g_maxPrivatePremium - 1.0f) * 100.0f,
         g_understaffedThreshold * 100.0f,
         g_wageBiddingStep * 100.0f,
         (g_labor && TsmLaborHasV3(g_labor)) ? "active (labor v3)"
                                             : "unavailable (labor v2)");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
