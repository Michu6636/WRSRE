// private_logistics - what a private company earns, and what it pays.
//
// ---------------------------------------------------------------------------
// v0.18.6 updates: bounded, read-only panel-object/fleet observer.
// Verified 7D5460 prologue, scale and format fingerprints; one sample/second,
// at most 240 per load epoch. No exact burn/tracker/billing integration.
//
// v0.18-permissions: independent state-site directions; consumer sites never
// supply goods; state default deny applies to existing/manual targets too.
// Missing eligible connections are recreated by the company. No fuel/billing
// policy changes in this version. Native direction semantics need game testing.
//
// Historical v0.17 updates (state-storage source opt-in):
//   - Register a storage-panel checkbox through ui_panels' widget API; each
//     public type-5 storage defaults OFF and its permission is persisted by a
//     stable type/name/position key, never by retaining a Building pointer.
//   - When opted in, append a native DO target with load enabled and unload
//     enable/percentage cleared. Turning the checkbox off disables both task
//     directions on that state-storage target; unrelated DO targets are untouched.
//   - The storage builder is hooked by the verified dispatcher call site in
//     ui_panels. Customs remains diagnostic-only; no inventory is manipulated.
// ---------------------------------------------------------------------------
// v0.16 updates (coordinate probe correction):
//   - v0.15 logged a state storage position of (0,0) and customs distance 0.0m.
//     The position helper had read bld+0x08/+0x10 instead of the C3D_NODE
//     translation at bld+0x320+0x30/+0x38 (with placement-bbox fallback).
//     Stable identity keys and nearest-customs diagnostics now use the actual
//     position helper. No customs/state-storage target writes were added.
//
// v0.15 updates (state-storage/customs discovery probe; no public-target writes):
//   - Walk the game-wide building vector to count state storage buildings and
//     report each private road/rail DO's geometrically nearest customs house.
//     This is diagnostic only; neither is appended as a target in this version.
//   - Use game_world.h's canonical demolished flag rather than an unverified
//     raw offset in private-building discovery.
//
// v0.14 updates (automatic private DO targets; staged and disabled by default):
//   1. Once per in-game day, discover registered private DOs (road 43 / rail 52)
//      and private buildings with native storage, using enterprise_base.
//   2. Preserve existing native target entries. Missing eligible targets are
//      reported in dry-run mode by default; optional write mode appends a
//      TaskEntry and mirrors the native add-path TaskCfg defaults (wildcard,
//      100% thresholds, active/validated flags). The owner reports separate
//      load/unload checkboxes in the DO panel; their backing fields and dataflow
//      still need verification before assigning state storage as load-only.
//   3. Generated entries are enabled, deduplicated, and trigger the verified DO
//      dirty flag. Existing entries/configs are never rewritten or removed.
//   4. A session-local identity ledger tracks generated targets by stable
//      keys. Missing eligible targets are recreated; manual removal is not a veto.
//   5. This is a daily callback, not a per-frame hook. Save/load and manual-removal
//      UI behavior remain runtime validation items; write mode ships off.
// ---------------------------------------------------------------------------
// Level 3. Consumes enterprise_base (v8), currency (v4) and sim_clock.
// Optionally consumes resource_pricing for the unit price.
//
// ---------------------------------------------------------------------------
// v0.10 - THE TANK WAS FOUND, AND THE TARIFF WAS FOUR TIMES TOO CHEAP.
//
// veh+0x5F0 is fuel in tonnes. v0.9's field scanner found it by watching which
// words move, and the v0.9 run confirmed it five ways: it falls while the truck
// works, it runs to zero and jumps at a refill, it tops out at 0.46 (the
// player's own 456 LITRE tank at 1 kg/L), the refills reconcile to the diesel
// the company paid for, and nothing else in the vehicle moves that way.
//
// With the tank readable, the burn stops being a guess:
//
//     2.05 t over 51 days, two active vehicles, 139 deliveries
//     = 0.01475 t per delivery = 14.75 L at 1 kg/L
//
// v0.9 shipped 3.30 L/trip, derived from PURCHASES over trips. The company was
// being paid 1.06 per delivery against a real fuel cost of 1.96 - it lost money
// on every trip and the tariff, not the economy, was doing it.
//
// v0.10 reads every vehicle's tank at every close and reports
//
//     [TANK] day N   v=... fuel 0.4300 t | burned 0.0220 t | refills 0
//     [TANK] run: burned 2.0500 t = 2050.0 L over 139 delivery(ies) = 14.75 L/trip
//
// Refills are counted, never subtracted from burn. This is the first figure in
// the whole feature that is neither a purchase nor a model.
//
// ALSO: diesel_density 0.00084 -> 0.00100. The game stores 1 kg per litre; the
// old constant was real-world diesel and made every litre 16 % too cheap.
//
// ---------------------------------------------------------------------------
// v0.9 - FIND THE TANK.
//
// The player asked the right question: how do we know how much fuel a trip
// burnt? We do not. Two runs measured it by dividing the company's diesel
// PURCHASES by its trips and got 3.27 L/trip over 1,100 trips and 7.14 L/trip
// over 214 trips - a factor of two, because fuel sits in tanks between
// purchases. Purchases are not burn, and no amount of averaging makes them so.
//
// What is needed is the fuel going INTO a vehicle, or the level inside it. The
// vehicle record is reachable (bld+0xC70, proven) and the transfer hook never
// shows a fuel movement, so the tank is a FIELD inside the vehicle, not an
// event we can already see.
//
// This build finds it empirically instead of guessing:
//   - the first sighting of a vehicle is stored as a baseline and not logged;
//   - every day after, the same 512 dwords at veh+0x000..0x800 are compared,
//     and ONLY the words that changed are logged, as offset, old, new.
//
// A tank shows up as a word that falls while the truck drives and jumps when
// it refuels. Position, speed and timers will show up too - the offsets tell
// them apart. Nothing is priced from this; it is how the next tariff stops
// being a fleet average.
//
// ALSO IN v0.9:
//   - the [FUEL] run line now says PURCHASES and not burn, because that is
//     what it measures.
//   - the ini learned its section header back. v0.7's rewrite dropped
//     [private_logistics], so the host read nothing and every key fell back to
//     its compiled default - which is why the last two runs were dry runs.
//
// ---------------------------------------------------------------------------
// v0.8 - SEE THE VEHICLES BEFORE PRICING THEM.
//
// v0.7's trip tariff charged every delivery the same litres. That is one
// arbitrary vehicle imposed on a whole fleet, and it cannot survive: vehicles
// differ in capacity, speed, price and consumption, and a tariff that ignores
// it will overpay a scooter and bankrupt a heavy hauler. H41.
//
// The v0.7 run shows the transfer hook cannot supply the difference: 3,065
// site lines resolve to exactly TWO vehicle-side pointers, one of them NULL.
// The fleet has to be read where the game keeps it - bld+0xC70/+0xC78, proven
// by three independent readers (Part 12) - and each vehicle's own record read
// from there:
//
//     veh+0x4F8  Building*  home building                      proven
//     veh+0x3B8  vec        cargo, stride 0x10 (Res*, float t)  proven
//     veh+0x510  ?          candidate catalogue entry           UNPROVEN
//
// This build adds the census: once a day, every private enterprise's fleet is
// walked, each vehicle is logged with what it carries and what it can carry at
// peak, and the first sighting of each vehicle dumps 32 bytes at the pointer
// the game gives for its catalogue entry. That is the lead for capacity,
// speed, price and consumption - the four numbers the final model needs and
// the four this build refuses to invent.
//
// The tariff is unchanged from v0.7. It stays a fallback until the census
// says what the fleet actually is.
//
// ALSO IN v0.8:
//   - litres_per_trip 5.00 -> 3.30. The v0.7 meter measured the DO's diesel
//     bill against its own deliveries: 406.40 RUB over 1,100 trips =
//     3.267 L/trip at ~128/RUB. Not a guess, and not 8.63 either - that
//     figure counted tank fill, exactly as the player said it would.
//   - the [FUEL] run line no longer claims "model and bill agree" when the
//     bill is zero and there is nothing to compare.
//
// ---------------------------------------------------------------------------
// v0.6 - THE TRIP IS PAID FOR BY THE TRIP, AND THE SHARE IS ONLY A SHARE.
//
// Session 7 ran v0.5 with fuel switched on and produced the number the whole
// feature needed: the private DO's account reconciles exactly to
//
//     10,000.00 + 738.99 freight - 445.37 fuel - 0.02 = 10,293.60
//
// so the game DOES charge a private distribution office for diesel, and it
// arrives as a cat=2 (FOREIGN) debit on its own company capital. H37 closed.
// H33 closed too: 444 sampled payments, all moving the treasury to the kopeck.
//
// The same log indicted the payment model. Per delivery, against a measured
// fuel cost of 0.34 RUB:
//
//     plants     282 deliveries   0.29 each   0.9x the fuel bill
//     food       127 deliveries   2.13        6.3x
//     meat        16 deliveries   2.37        7.0x
//     fabric      19 deliveries   3.80       11.2x
//     chemicals   45 deliveries   6.13       18.0x
//
// 58 % of the DO's work was selling at a LOSS and 9 % was paying for it. A
// percentage of cargo value is not a tariff, it is a lottery. Real freight is
// cost recovery plus a margin; only the handling of valuable cargo is priced
// ad valorem, and it is a small part of the bill.
//
// v0.6 therefore models the tariff as what it has to be:
//
//     tariff = fuel_cost x (1 + margin) + tariff_share x cargo_value
//
// Cost recovery pays the trip, the share pays for the cargo being worth
// carrying. litre counts come from session 7 (see litres_per_trip in the ini).
//
// Still probe only. Nothing here is charged. The 8 % share keeps running so
// the two can be read side by side for one more session.
//
// Also fixed: the index and the treasury were read at plugin start, before the
// world is loaded, so every log opened with "fuel NOT FOUND" and "treasury
// opens at 0.00" that were both false a second later. Both are now re-read and
// printed at every day close.
//
// ---------------------------------------------------------------------------
// v0.5 - MEASURE THE COST BEFORE PRICING IT.
//
// v0.4 moved money and the money arrived (RE_DIGEST Part 19). What the same
// session also showed is that the private DO has NO COST SIDE: its company
// account took 646 credits over seven days and paid out nothing at all. Fuel,
// upkeep and vehicles never touched its books. So the 8 % freight is not a
// tariff, it is a subsidy, and "is 8 % enough to cover fuel" cannot be asked
// until a cost exists to compare it against.
//
// This build therefore charges nothing new and measures two things:
//
//   1. THE TARIFF MODEL. A price built the way real freight is built - cost
//      recovery plus a margin - and indexed to a live price, never to a
//      rouble constant. A constant is exactly what inflation eats:
//
//          litres  = litres_per_trip + litres_per_tonne x tonnes
//          cost    = litres x diesel_density x fuel_price      (live)
//          tariff  = cost x (1 + margin)
//
//      If no fuel price resolves (fuel_resource unknown, resource_pricing
//      absent), the same cost is expressed in daily survival baskets via the
//      CPI tail, so the fallback is indexed too. If neither resolves the
//      probe reports "unindexed" and prices nothing - it never guesses a
//      figure in currency units.
//
//   2. THE MONEY PATH, both sides of it. [BAL] prints the treasury and every
//      carrier's company capital at each day close. v0.4 proved the company
//      side to the kopeck but never read the payer (H33); the delta between
//      two closes is that proof, and the difference between a carrier's
//      capital change and the freight it was paid is its fuel bill (H37).
//
// Nothing here charges the probe. `tariff_probe = 1` logs, it does not pay.
// The 8 % share keeps running exactly as v0.4 left it, so a session produces
// three numbers side by side: modelled tariff, freight actually paid, and the
// vehicle's real fuel consumption as the game charged it.
//
// v0.4 also carried a stale banner - it opened every log with "v0.3" - which
// cost the last reader a minute. Version now comes from PL_VERSION once.
//
// ---------------------------------------------------------------------------
// v0.4 - FREIGHT IS A SHARE OF CARGO VALUE, AND THE MONEY MOVES.
//
// v0.3 paid a flat per-tonne rate (1.50 RUB/t). Measured against the live
// session that is 0.31 % of what the cargo is worth - a private distribution
// office could not cover a truck, and FreightRateFor() ignored the resource
// anyway, so the liquid and goods rates were dead keys.
//
// v0.4 replaces the rate with freight_share, a fraction of the cargo's own
// value:
//
//     freight = tons x unitPrice x freight_share          (default 0.08)
//
// A typical load is ~0.12 t of fabric worth ~58 RUB, so this pays ~4.70 RUB
// instead of ~0.18. It scales with what is actually moved, it needs no
// commodity-class table, and when Ship 2 makes the BUYER pay instead of the
// state, the same formula becomes the wholesaler's margin with no rewrite.
//
// v0.4 also:
//   - refuses a payment the treasury cannot afford (getTreasuryBalance),
//     because moveTreasury takes a signed amount and will happily go negative;
//   - splits `skipped` into the four things it was conflating;
//   - labels the daily line with the day it actually covers (it was one
//     behind - the callback fires at the START of the day).
//
// ---------------------------------------------------------------------------
// v0.3 - THE FIRST MONEY.
//
// `dry_run = 1` by default: this build COMPUTES every payment and logs it as
// "[PAY] would", but moves nothing. Set dry_run = 0 to actually pay.
// Nothing else in the plugin changes when you flip it.
//
// The money model (PLAN_TECHNICAL.md sections 6 and 7):
//
//   H1 - A BUILDING IS PAID FOR GOODS THAT LEAVE IT
//        at a mode-0 call with tons < 0, param_2 is the source Building.
//        If that building is a private company, it is credited
//        |tons| x price x factory_share, and the treasury pays.
//
//   H2 - A CARRIER IS PAID FREIGHT FOR A DELIVERY
//        at a mode-0 call with tons > 0, param_2 is the destination Building
//        and param_9 is the fleet owner. If the owner is a private company
//        and is not the destination, it is credited
//        tons x price x freight_share.
//
//   Nothing is billed at a mode-1 site: those are the vehicle side of the
//   same movement and would double-charge it.
//
// ---------------------------------------------------------------------------
// What the two live sessions established (RE_DIGEST Parts 10, 12 and 13)
// ---------------------------------------------------------------------------
//
//   - THE COMMIT TEST IS `(unsigned char)r == 1`. The transfer returns a bool
//     in AL; the upper 24 bits of EAX are register garbage. v0.1 tested the
//     whole int and discarded exactly half the committed calls.
//
//   - `param_2` IS A Building* AT MODE 0 AND A Vehicle* AT MODE 1. That is
//     the whole of attribution. At mode 0 the value also appears in
//     enterprise_base's "[CAP] ... by=wholesale" lines, which only ever take
//     a building; vehicle offsets read off it return garbage.
//
//   - `param_9` IS THE FLEET OWNER, not an endpoint: constant across two
//     different destination buildings, and equal to the mode-1 vehicle's
//     `home` field in every call.
//
//   - `(veh+0x4F0)` IS NOT AN ENDPOINT. It was NULL in all 342 calls v0.1
//     measured and all 373 of v0.2's, because at mode 0 we were reading
//     vehicle fields off a building. Nothing here uses it.
//
//   - A MOVEMENT IS TWO CALLS 30-70 ms APART, same resource, same tonnage:
//       0x694F40  mode 0  -tons   the BUILDING the goods leave
//       0x6953C4  mode 1  +tons   the VEHICLE they enter
//       0x68F792  mode 0  +tons   the BUILDING they enter
//       0x68F7F9  mode 1  -tons   the VEHICLE they leave
//     The load leg's mode-0 call has param_9 == NULL, so its carrier is
//     resolved from the paired mode-1 call through a short time ring.
//
// ---------------------------------------------------------------------------
// The hook is a call-site patch, NOT an inline hook
// ---------------------------------------------------------------------------
//
// FUN_140420350 has a five-byte prologue, so the loader's installInlineHook
// (which requires >= 14 position-independent bytes) cannot take it. All
// sixteen invocations are direct `call rel32` with no vtable, thunk or
// indirect path, so retargeting the call sites sees every invocation.
//
//   site before:  E8 <rel32>          call FUN_140420350
//   site after:   E8 <rel32>          call cave          <- SAME opcode
//   cave:         FF 25 00 00 00 00   jmp qword ptr [rip+0]
//                 <8-byte detour address>
//
// The opcode deliberately stays E8. A jmp does not push a return address, so
// a detour entered that way both reads garbage from _ReturnAddress() and
// executes `ret` into the enclosing function's caller. Verified on x86-64.
//
// Safety properties:
//   - Nothing is written unless the site verifies: byte 0 must be E8 AND its
//     rel32 must resolve to exeBase+0x420350.
//   - Every game-memory read is behind ReadablePtr.
//   - The ledger is a fixed ring; nothing allocates in the detour.
//   - The lock covers counters and the rings only. Game-memory reads, calls
//     into other plugins and logging all happen outside it.
//   - A payment needs BOTH services. If enterprise_base or currency is
//     missing, billing is skipped entirely and says so once.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "site_permissions.h"
#include "task_audit.h"
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"

#include <stdint.h>
#include <math.h>
#include <new>

// ---------------------------------------------------------------------------
// the transfer being instrumented
// ---------------------------------------------------------------------------

#define TSM_RVA_TRANSFER 0x420350

typedef int (*tTransfer)(void* game, void* veh, unsigned mode, void* res,
                         float tons, float* outMoved, float cap,
                         unsigned char flag, void* bld);

static tTransfer g_realTransfer = nullptr;

// Vehicle fields. See PLAN_TECHNICAL.md section 3.1.
#define TSM_VEH_HOME    0x4F8   // Building* - the owning depot (mode 1 only)

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static int   g_enabled         = 1;
static int   g_billing         = 0; // Permissions-test default: no freight payments.
static int   g_dryRun          = 1;      // SAFE DEFAULT: compute, log, do not pay
static int   g_payFactory      = 1;
static int   g_payCarrier      = 1;
static int   g_logCapPerDay    = 400;
static int   g_ledgerCapacity  = 512;
static int   g_siteLog         = 1;
// v0.4: ONE number, a share of the cargo's value. The three per-tonne rates
// are gone: the class table never existed (FreightRateFor ignored its
// argument) and a player cannot reason about "RUB per tonne of coal".
static float g_freightShareRetired = 0.0f; // set when the ini still carries it
static float g_freightShare    = 0.08f;  // v0.7: RETIRED. Kept only so the
                                         // ini key can be recognised and warned about.
static float g_factoryShare    = 0.90f;  // w - what a buyer pays a factory

// v0.7: THE TRIP TARIFF REPLACES THE VALUE SHARE.
//
// v0.6 measured the value share against the DO's real fuel bill and lost:
// 14,038.20 RUB of freight against 511.92 of diesel over 82 days. The share
// is 27x the cost it was supposed to cover, and it is value-blind - a truck
// hauling plants was paid 1.46 for the trip, one hauling chemicals 71.12.
//
// v0.7 charges the model that was only probed in v0.6; v0.6's behaviour is
// one key away (`trip_tariff = 0`), so both can be compared in one session.
static int    g_tripTariff      = 1;
static double gWearLitres       = 5.0;   // maintenance reserve, in litres of diesel
static int    g_litresAuto      = 0;     // re-tune litres_per_trip from the fuel bill

// v0.7: the measured fuel bill. This is the only figure in the plugin that
// comes from the game instead of from arithmetic - the [TARIFF] model is an
// assumption, the DO's monthly foreign-cost total is the bill.
//
// Fuel is bought in bulk and stored, so a day's purchases are NOT a day's
// burn. Over a run the tank delta washes out; the average is the burn.
// v0.8: the vehicle census. A republic has tens of vehicles, not thousands;
// 64 slots is four times the largest fleet seen so far and keeps the table in
// static storage, as the render/tick rules require.
#define VEH_MAX 64
static void*    g_vehPtr[VEH_MAX];
static double   g_vehPeakTons[VEH_MAX];
static unsigned g_vehSamples[VEH_MAX];
static int      g_vehCount   = 0;
static int      g_censusLog  = 1;
// v0.9: the field scanner. 512 dwords (0x800 bytes) per vehicle, compared once
// a day; only changes are logged. Static storage, no allocation.
#define VEH_SCAN_WORDS 512
static unsigned g_vehSnap[VEH_MAX][VEH_SCAN_WORDS];
static bool     g_vehSnapValid[VEH_MAX];
// v0.10: THE TANK. veh+0x5F0 is fuel, in TONNES, and the v0.9 field scanner
// found it the honest way - by watching which words move.
//
// The evidence, from the v0.9 run (51 days, two active vehicles):
//   - the word falls every day the truck works: 0.4527 -> 0.4307 -> 0.4073 ...
//   - it runs to ~0 and then JUMPS: 0.0024 -> 0.4527  (+0.4503, a refill)
//   - three refills per vehicle, each +0.26 to +0.48, and the tanks top out at
//     ~0.46 - which is the player's own reported 456 LITRE tank at 1 kg/L.
//   - the refills over the run (2.66 t) match the diesel the company PAID for
//     (389.23 RUB = 2.99 t at 133/RUB), less what was left in the tanks.
//
// That is five independent agreements, so the field is fuel and the unit is
// tonnes. Nothing else in the vehicle moved like that.
static double   g_vehFuel[VEH_MAX];          // last reading, tonnes
static bool     g_vehFuelValid[VEH_MAX];
static double   g_burnTonsRun   = 0.0;       // burned this run, tonnes
static unsigned g_refillsRun    = 0;
// v0.11: the denominator the physics actually has. Burn is per TICK, so the
// honest rate is litres per fleet-day, not litres per delivery - and it scales
// with HOW MANY TRUCKS and HOW LONG, never with how much they carry.
static unsigned g_tankDaysRun    = 0;        // closes with at least one tank read
static unsigned g_tankVehDaysRun = 0;        // (vehicle, close) pairs
static int      g_cargoDump      = 1;        // [CARGO] - settle where fuel lives
static int      g_tankCrossLog   = 40;       // v0.13: [TANK] slot-vs-mirror lines
static unsigned char g_vehNoSlotWarned[VEH_MAX];
static unsigned char g_vehCapValid[VEH_MAX];
static double        g_vehCap[VEH_MAX];      // per-vehicle tank capacity, tonnes

// v0.13: THE ACTIVITY GATE, straight out of the follow-up report.
//
// The engine skips a vehicle entirely unless veh+0x448 is 1 or 2 (and +0x440
// is 9): no burn, no refuel, nothing. That means the honest denominator for a
// fuel rate is not "fleet-day" but "TRUCK-DAY ACTIVE", and it means the ~40 L
// this fleet burned on days with no private deliveries was burned doing work
// that was not ours.
//
static unsigned g_activeVehDaysRun = 0;
static unsigned g_idleVehDaysRun   = 0;

// v0.11: THE PER-TRIP FUEL PROBE.
//
// The player's question was exact: "You were able to see the fuel used for
// every trip?" No. Every number this plugin has ever printed about fuel came
// from PURCHASES divided by trips, and purchases are a stock movement, not a
// burn. With 4,558 L bought and 4,157 L pumped into tanks in the v0.7 run, the
// ratio could have been anywhere between 7.0 and 8.7 L/trip.
//
// The fix costs one pointer walk: at every committed delivery, total the tanks
// of the fleet that did it. The difference from the previous delivery is the
// fuel burned between the two - no purchases, no storage, no assumptions. A
// refill (tank goes UP) or a change in fleet size invalidates that one
// interval, and those are counted separately rather than averaged in.
static const int VEH_FUEL_OFF = 0x5F0;     // the tank, in tonnes (see ReadTank)

static void*    g_probeBld      = nullptr;
static double   g_probeFuelTons = -1.0;
static int      g_probeVehCount = -1;
static double   g_probeBurnRun  = 0.0;
static unsigned g_probeStepsRun = 0;
static unsigned g_probeSkipRun  = 0;
static double   FleetFuelTons(void* b, int* outCount);
static void     BurnProbe(void* b);
static int      g_vehScan          = 1;
static int      g_vehScanLogPerDay = 40;
static void     CensusFleets(int closedDay);   // v0.8, defined with the census
static void     ScanVehicle(int slot, void* v, int closedDay);   // v0.9
static void     ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills);   // v0.10

static double             g_fuelPaidRun  = 0.0;
static double             g_fuelLitresRun = 0.0;
static unsigned long long g_fuelTripsRun = 0;

// v0.11 updates:
//   1. litres_per_tonne ships at 0. Load does not enter the burn formula -
//      Ghidra, FUN_14016E3B0 - and the v0.10 log agrees (r = -0.03).
//   2. [TANK] run prints LITRES PER FLEET-DAY and per truck-day beside L/trip.
//      L/trip is a ratio of two unrelated things; L/day is the physics.
//   3. [CARGO] dumps the raw cargo vector, uncapped, to settle where fuel
//      actually lives. The Ghidra report says veh+0x3B8; our own census says
//      the trucks carry 0 slots while holding 0.29 t in the tank. One of those
//      is wrong and a dump costs nothing.
//   4. A NEGATIVE tank reading is read as empty, not as extra burn. Day 13 of
//      the v0.10 run read -0.0302 t; +0x5F0 is derived, not the level.
//   5. BURN PROBE: the fleet's tanks are totalled at every committed delivery,
//      so the next run reports diesel that actually left the tanks BETWEEN two
//      deliveries. That is the player's question answered with a measurement
//      instead of purchases divided by trips.
//
// v0.12 updates:
//   1. The cargo census no longer hides vectors longer than eight slots. That
//      cap is what made this plugin report "0 slot(s)" on trucks holding fuel,
//      and I used that as evidence. It was not evidence - it was a bug here.
//   2. [CARGO] dumps BOTH storage vectors, 0x3A0 and 0x3B8, every slot.
//   3. [VEH] now prints kind=, the int at +0x50C that the report calls the
//      fuel-consumer flag. +0x510 was never a pointer; that is why it printed
//      nonsense.
//
// v0.13 updates (follow-up disassembly):
//   1. THE TANK IS THE CARGO SLOT, not +0x5F0. The slot at veh+0x3B8 whose
//      resource pointer is diesel's, +0x08 tonnes, clamped at zero.
//   2. +0x5F0 is an UNCLAMPED MIRROR written by the refuel and drive paths.
//      That is why it went negative; it is now a cross-check, not the source.
//   3. The fleet's tank can be read per vehicle in two places - the slot for
//      the level, and *(veh+0x1708)+0x8684 for that vehicle's CAPACITY. The
//      capacity is logged once per vehicle, so no arbitrary vehicle stands in
//      for the fleet.
//   4. [VEH] prints state (veh+0x448) and mode (veh+0x440). The engine skips a
//      vehicle entirely unless the state is 1 or 2, so "litres per ACTIVE
//      truck-day" is the only fuel rate that means anything; it is now on the
//      [TANK] run line.
//
// v0.5: the cost side, in physical units. No rouble constant may appear here:
// prices move, litres and ratios do not.
static int    g_tariffProbe     = 1;       // compute + log the tariff, never charge
static int    g_balanceLog      = 1;       // [BAL] treasury + carrier capital per day
static int    g_tariffLogPerDay = 60;      // per-delivery [TARIFF] lines, then totals only
static double gLitresPerTrip    = 14.75;   // v0.10: measured from the TANKS, not purchases
// v0.11: ZERO - and this retires a term I invented.
//
// Ghidra on FUN_14016E3B0 (the per-tick upkeep function) shows the burn is
//
//   consumption = (slot_rate / *(float*)(bld+0xE30)) * base_rate
//                 * (tick_delta / DAT_14090AA78) * fVar24
//   fVar24      = DAT_140909F58 / ((bld+0xE68 - bld+0xE60) / 0xC)
//
// fVar24 is a BUILDING-level constant and cargo weight never enters it. A full
// truck burns what an empty one burns. `litres_per_tonne` was my invention and
// it is falsified by the code as well as by the v0.10 log (daily burn vs daily
// deliveries: correlation -0.03 over 51 days).
//
// Kept as a key so the claim can be re-tested, but it ships at zero.
static double gLitresPerTonne   = 0.0;     // FALSIFIED - load does not burn
static double gTariffMargin     = 0.35;    // dimensionless: 35 % over fuel cost
static double gDieselDensity    = 0.00100; // t/L - v0.10: the game uses 1 kg/L
                                            // (a 456 L tank reads 0.456 t), not
                                            // real diesel's 0.84
static char   gFuelResource[64] = "fuel";  // resolved through resource_pricing
static double gBasketsPerTrip   = 0.80;    // CPI fallback, in daily survival baskets
static double gBasketsPerTonne  = 0.02;

#define PL_VERSION "0.18.6-panel-observer"
static int g_panelObserverEnabled = 1;
static void PanelObserverLoading(void);
static float g_minPayment      = 0.01f;
static double g_dailyPayCap    = 0.0;    // 0 = no cap

static const TsmEnterpriseBaseApi*  g_enterprise = nullptr;
static const TsmSimClockApi*        g_simClock   = nullptr;
static const TsmCurrencyApi*        g_currency   = nullptr;
static const TsmResourcePricingApi* g_pricing    = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persistenceHandle = 0;
static const TsmUiPanelsApi*         g_uiPanels = nullptr;
static int                           g_stateStorageWidgetHandle = 0;

// Automatic DO-target generation. Fixed buffers only; discovery runs from the
// once-per-day sim_clock callback, never from a per-frame/render hook.
static int g_autoTargetsEnabled = 1;
static int g_autoTargetsDryRun  = 0;
static int g_autoSourcePercent = 10;
static int g_autoDestinationPercent = 30;
static int g_autoTargetLogLimit = 80;
static volatile LONG g_autoResetActiveMemoAfterLoad = 0;
#define AUTO_ENTITY_MAX 8704
#define AUTO_MEMO_MAX   262144
#define AUTO_MEMO_HASH_SIZE 524288
#define AUTO_POINTER_HASH_SIZE 32768
#define AUTO_OFFICE_HASH_SIZE 16384
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char isStateStorage; unsigned char statePermission; unsigned char targetPresent; };
struct AutoMemo { uint64_t officeKey; uint64_t targetKey; unsigned nextOffice; unsigned char state; };
struct AutoPointerSlot { void* building; unsigned entityIndex; };
struct AutoOfficeMemoHead { uint64_t officeKey; unsigned headPlusOne; };
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static AutoMemo   g_autoMemo[AUTO_MEMO_MAX];
static unsigned   g_autoMemoHash[AUTO_MEMO_HASH_SIZE]; // index + 1; zero means empty
static AutoPointerSlot g_autoPointerHash[AUTO_POINTER_HASH_SIZE];
static AutoOfficeMemoHead g_autoOfficeMemoHeads[AUTO_OFFICE_HASH_SIZE];
static unsigned   g_autoMemoCount = 0;
static unsigned   g_autoLogLines = 0;
static int        g_autoLastDay = -1;
static void AutoTargetsOnDay(int day, int closedDay);
static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void* userData);

// ---------------------------------------------------------------------------
// the sixteen call sites
// ---------------------------------------------------------------------------
//
// `known` records what the RE rounds established. The instrument does not act
// on the label - every site is logged the same way - but billing only ever
// fires at a MODE 0 site, and `known` is what makes the log readable.
//
// After the first two sessions the FUN_14068B330 family is no longer
// described as "sea/rail": it carries ordinary factory traffic.

enum SiteClass {
    SITE_UNKNOWN = 0,
    SITE_REFUEL,
    SITE_LOAD,          // draws goods out of a building
    SITE_DELIVERY,      // puts goods into a building
    SITE_TRANSSHIP      // intra-vehicle / return leg
};

struct SiteSpec {
    unsigned    rva;
    SiteClass   known;
    const char* note;
};

#define MAX_SITES 16

static SiteSpec g_sites[MAX_SITES] = {
    { 0x00141F33, SITE_UNKNOWN,   "never observed" },
    { 0x0016F831, SITE_REFUEL,    "refuel call - telemetry, never billed; never observed" },
    { 0x001B2898, SITE_UNKNOWN,   "CONTESTED: r8-9 'road delivery', r10 'road refuel'; never observed" },
    { 0x001B484B, SITE_UNKNOWN,   "never observed" },
    { 0x001B5EC0, SITE_UNKNOWN,   "never observed" },
    { 0x001B653B, SITE_UNKNOWN,   "never observed" },
    { 0x001B6833, SITE_UNKNOWN,   "never observed" },
    { 0x001B75ED, SITE_UNKNOWN,   "never observed" },
    { 0x001B7981, SITE_UNKNOWN,   "never observed" },
    { 0x001CB7FF, SITE_LOAD,      "loader: mode 0, +tons, then bld+0x970 storage-add; never observed" },
    { 0x0068EECA, SITE_LOAD,      "family A: mode 0, +tons, cargo-add follows; never observed" },
    { 0x0068F792, SITE_DELIVERY,  "family A DELIVERY: mode 0, +tons, param_2 = building, param_9 = fleet owner" },
    { 0x0068F7F9, SITE_TRANSSHIP, "family A delivery, vehicle side: mode 1, -tons. NEVER BILLED" },
    { 0x006945B3, SITE_TRANSSHIP, "family A: mode 0, -tons; never observed" },
    { 0x00694F40, SITE_LOAD,      "family A LOAD: mode 0, -tons, param_2 = the building the goods leave" },
    { 0x006953C4, SITE_TRANSSHIP, "family A load, vehicle side: mode 1, +tons. NEVER BILLED" }
};

static bool g_sitePatched[MAX_SITES] = { false };
static bool g_siteWanted[MAX_SITES]  = { false };   // filled in by ReadSettings
static bool g_siteFilterActive       = false;       // false = every site

static int  g_siteCount = MAX_SITES;

// ---------------------------------------------------------------------------
// the ledger
// ---------------------------------------------------------------------------

enum ShipKind { SHIP_LOAD = 0, SHIP_DELIVERY, SHIP_FREIGHT, SHIP_FUEL, SHIP_PURCHASE };

struct Shipment {
    unsigned id;
    unsigned seq;
    void*    building;
    void*    counterparty;
    void*    vehicle;
    void*    resource;
    float    tons;
    double   unitPrice;
    double   value;
    unsigned short site;
    unsigned char  kind;
    unsigned char  mode;
    double   t;
};

#define LEDGER_MAX 2048
static Shipment g_ledger[LEDGER_MAX];
static int      g_ledgerHead  = 0;
static unsigned g_ledgerSeq   = 0;
static unsigned g_ledgerTotal = 0;

// ---------------------------------------------------------------------------
// the carrier ring
// ---------------------------------------------------------------------------
//
// The load leg's mode-0 call does not carry the carrier: param_9 is NULL
// there (91 of 91 calls). The vehicle is named 30-70 ms later by the paired
// mode-1 call. This ring is how the two are joined.
//
// A match needs the same resource, the same tonnage within 0.5 %, and an age
// under 2 s. If nothing matches, the payment is skipped and counted - never
// guessed.

#define CARRIER_RING 32

struct CarrierNote {
    void*  res;
    float  tons;      // absolute
    void*  home;      // the fleet owner
    double t;
};

static CarrierNote g_carrier[CARRIER_RING];
static int         g_carrierHead = 0;

static double NowSeconds(void)
{
    // GetTickCount64 is monotonic and needs no CRT.
    return (double)GetTickCount64() / 1000.0;
}

// ---------------------------------------------------------------------------
// counters
// ---------------------------------------------------------------------------

static unsigned g_calls            = 0;
static unsigned g_callsCommitted   = 0;
static unsigned g_callsNoop        = 0;
static unsigned g_unknownBuilding  = 0;
static unsigned g_staging          = 0;
static unsigned g_nonPrivate       = 0;
static unsigned g_siteLoggedToday  = 0;
static unsigned g_attrLoggedToday  = 0;
static unsigned g_payLoggedToday   = 0;
static unsigned g_siteSuppressed   = 0;
static unsigned g_attrSuppressed   = 0;
static unsigned g_paySuppressed    = 0;

static unsigned g_payCount         = 0;
// v0.4: `skipped` used to be one number covering four unrelated causes. In
// the fourth session it read 159 in a day purely because the distribution
// office was state-owned - alarming to read, and meaningless.
static unsigned g_skipNoPayee      = 0;   // no private owner on the leg
static unsigned g_skipNoPrice      = 0;   // resource_pricing gave no price
static unsigned g_skipSmall        = 0;   // below min_payment
static unsigned g_skipTreasury     = 0;   // the state could not afford it

// v0.5: the tariff probe's own tallies, and which index it managed to use.
// An unindexed delivery is NOT a delivery worth 0.00 - it is one this build
// could not price, and the daily line says so rather than folding it in.
static unsigned g_tariffCount      = 0;
static unsigned g_tariffNoIndex    = 0;
static unsigned g_tariffFuelIndex  = 0;
static unsigned g_tariffCpiIndex   = 0;
static double   g_tariffCostSum    = 0.0; // the fuel-recovery half
static double   g_tariffShareSum   = 0.0; // the ad valorem half
static unsigned g_tariffLoggedToday= 0;
static double   g_tariffSum        = 0.0;

// v0.5 (H33): the payer leg, sampled per payment. A daily treasury delta is
// useless as proof - wages, taxes and store sales move the same balance all
// day long. Sampling either side of OUR OWN move isolates it completely: the
// difference must be exactly minus the amount, or the books do not close.
// v0.5: the refuel site (0x16F831) has never fired in any session. If the
// game is going to start charging for fuel, this is where we see it first.
static unsigned g_fuelEvents       = 0;
static unsigned g_treasuryChecked  = 0;
static unsigned g_treasuryExact    = 0;
static unsigned g_treasuryOff      = 0;
static double   g_treasuryMaxGap   = 0.0;

// v0.5: the money path, both ends. The treasury is sampled at each close; the
// carriers we paid are remembered so their capital can be printed beside it.
#define BAL_CARRIERS 4
static double g_treasuryAtClose    = 0.0;
static bool   g_treasuryHaveClose  = false;
static void*  g_balCarrier[BAL_CARRIERS]     = { nullptr, nullptr, nullptr, nullptr };
static double g_balCarrierPrev[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 };
static double g_balCarrierIn[BAL_CARRIERS]   = { 0.0, 0.0, 0.0, 0.0 };
static double g_balCarrierFuel[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 }; // v0.7 meter,
                                                                    // -1 = no baseline yet
static int    g_balCarrierCount    = 0;
static void*  g_fuelRes            = nullptr;   // cached lookup, re-resolved daily

// v0.4: which day the accumulated tallies belong to. OnDay fires at the START
// of `day`, so the window it reports is the day that just ENDED.
static int g_summaryDay            = -1;
static unsigned g_payCapped        = 0;
static double   g_paidFactory      = 0.0;
static double   g_paidCarrier      = 0.0;
static double   g_paidToday        = 0.0;

static double   g_wouldPayFreight  = 0.0;
static double   g_wouldPayFuel     = 0.0;

static int      g_lastMonth        = -1;

// Per-site tallies, so "never fired" is distinguishable from "never logged".
struct SiteTally {
    unsigned calls;
    unsigned committed;
    double   tonsIn;      // sum of +tons
    double   tonsOut;     // sum of |−tons|
    double   value;
    unsigned sawBld;      // param_9 non-NULL
    unsigned sawCur;      // *(veh+0x4F0) non-NULL
    unsigned sawHome;     // *(veh+0x4F8) non-NULL
};

static SiteTally g_tally[MAX_SITES];

static int SiteIndex(unsigned rva)
{
    for (int i = 0; i < g_siteCount; i++)
        if (g_sites[i].rva == rva) return i;
    return -1;
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static bool IsPrivate(void* b)
{
    if (!b || !g_enterprise) return false;
    if (!TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return false;
    return g_enterprise->isBuildingPrivate(b) != 0;
}

static const char* KindName(unsigned char k)
{
    switch (k) {
        case SHIP_LOAD:     return "load";
        case SHIP_DELIVERY: return "unload";
        case SHIP_FREIGHT:  return "freight";
        case SHIP_FUEL:     return "fuel";
        case SHIP_PURCHASE: return "buy";
        default:            return "?";
    }
}

static const char* ClassName(SiteClass c)
{
    switch (c) {
        case SITE_REFUEL:    return "refuel";
        case SITE_LOAD:      return "load";
        case SITE_DELIVERY:  return "delivery";
        case SITE_TRANSSHIP: return "transship";
        default:             return "unknown";
    }
}

// The resource record's name lives at its head. A bad pointer yields "?".
static void ResourceName(void* res, char* out, size_t n)
{
    out[0] = 0;
    if (!res || !TsmIsValidGamePtr(res) || !ReadablePtr(res, 128)) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    const char* s = (const char*)res;
    size_t i = 0;
    for (; i + 1 < n; i++) {
        const char c = s[i];
        if (!c) break;
        if ((unsigned char)c < 32 || (unsigned char)c > 126) {
            _snprintf_s(out, n, _TRUNCATE, "?");
            return;
        }
        out[i] = c;
    }
    out[i] = 0;
    if (!out[0]) _snprintf_s(out, n, _TRUNCATE, "?");
}

// The building's type-descriptor name, so the log never again asks the reader
// to guess which object a pointer is.
static void BuildingName(void* b, char* out, size_t n)
{
    out[0] = 0;
    if (!b || !g_enterprise || !TsmIsValidGamePtr(b)) {
        _snprintf_s(out, n, _TRUNCATE, "-");
        return;
    }
    const char* nm = nullptr;
    __try { nm = g_enterprise->getBuildingObjectName(b); }
    __except (EXCEPTION_EXECUTE_HANDLER) { nm = nullptr; }
    if (!nm || !nm[0]) {
        _snprintf_s(out, n, _TRUNCATE, "?");
        return;
    }
    size_t i = 0;
    for (; i + 1 < n && nm[i]; i++) {
        const char c = nm[i];
        out[i] = ((unsigned char)c < 32 || (unsigned char)c > 126) ? '?' : c;
    }
    out[i] = 0;
}

// The suite's single price function. Everything this plugin charges is
// expressed against THIS number, and every pay line names it, so a change of
// price source can never silently halve or double a payment.
//
// NOTE: resource_pricing itself may be returning its shipped fallbacks
// (RE_DIGEST Part 12 section 3). That is fine and consistent - what must not
// happen is this plugin mixing a raw Res+0x58 read with a priced figure.
static double UnitPrice(void* res)
{
    if (!res || !TsmIsValidGamePtr(res)) return 0.0;

    if (g_pricing) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        const float p = g_pricing->getPrice(res, cur);
        if (p == p && p > 0.01f && p < 1e9f) return (double)p;
    }

    // Fallback: the raw record field, only if resource_pricing is absent.
    if (!ReadablePtr((const BYTE*)res + TSM_RES_PRICE_RUB, 4)) return 0.0;
    const float f = *(const float*)((const BYTE*)res + TSM_RES_PRICE_RUB);
    if (!(f == f) || f < 0.0f || f > 1e9f) return 0.0;
    return (double)f;
}

// v0.4: freight as a share of the cargo's value.
//
// `tons` is positive on this leg (the delivery). A zero or nonsense unit
// price yields zero rather than a guess - the caller counts it as a skip.
static double FreightFor(double tons, double unitPrice)
{
    const double value = tons * unitPrice;
    if (!(value > 0.0)) return 0.0;
    const double share = (double)g_freightShare;
    if (!(share > 0.0)) return 0.0;
    return value * share;
}

// One guard, used everywhere the treasury is read: an older currency host may
// carry a shorter struct, and getTreasuryBalance is past the end of it.
static bool TreasuryReadable(void)
{
    return g_currency && g_currency->getTreasuryBalance &&
           g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                     sizeof(g_currency->getTreasuryBalance);
}

// v0.5: the fuel price, resolved by NAME through resource_pricing so the
// index moves with the game's own economy. Cached for the day and dropped at
// each close, because a save load can replace the resource table under us.
static double FuelUnitPrice(void)
{
    if (!g_pricing || !g_pricing->getByName) return 0.0;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    if (!g_fuelRes) return 0.0;

    const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
    const float p = g_pricing->getPrice(g_fuelRes, cur);
    if (!(p == p) || !(p > 0.0f) || !(p < 1e9f)) return 0.0;
    return (double)p;
}

// v0.7: THE TARIFF. Fuel plus a maintenance reserve, both denominated in
// litres and both priced at the live fuel price:
//
//   work    = litres_per_trip + litres_per_tonne x tonnes
//   fuel    = work x diesel_density x fuel_price        <- the index
//   wear    = wear_litres x diesel_density x fuel_price <- sinking fund
//   tariff  = fuel x (1 + margin) + wear
//
// Two rules are enforced here, not in prose. Nothing is a rouble constant, so
// inflation cannot eat it; and nothing depends on what the cargo is WORTH, so
// a truck of plants pays for its trip the same as a truck of chemicals.
// The wear term exists because fuel x 1.35 can never buy a truck: a carrier
// has to recover vehicles over their life, and that is a per-trip cost.
//
// `*outIndex` is 1 for the fuel index, 2 for the CPI fallback, 0 for neither.
// A 0 return means "this build cannot price this delivery", not "it is free".
static double TariffFor(double tons, double* outFuel, double* outWear,
                        int* outIndex)
{
    if (outFuel)  *outFuel  = 0.0;
    if (outWear)  *outWear  = 0.0;
    if (outIndex) *outIndex = 0;
    if (!(tons > 0.0)) return 0.0;

    double fuel = 0.0, wear = 0.0;
    const double work = gLitresPerTrip + gLitresPerTonne * tons;
    const double fp   = FuelUnitPrice();

    if (fp > 0.0) {
        fuel = work * gDieselDensity * fp;
        wear = gWearLitres * gDieselDensity * fp;
        if (outIndex) *outIndex = 1;
    } else if (g_pricing) {
        const float basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        if (basket == basket && basket > 0.0f && basket < 1e9f) {
            fuel = (gBasketsPerTrip + gBasketsPerTonne * tons) * (double)basket;
            // The reserve expressed in the fallback unit: the litres ratio is
            // dimensionless, so it survives the switch unchanged.
            const double ratio =
                (gLitresPerTrip > 0.0) ? (gWearLitres / gLitresPerTrip) : 0.0;
            wear = fuel * ratio;
            if (outIndex) *outIndex = 2;
        }
    }

    if (outFuel) *outFuel = fuel;
    if (outWear) *outWear = wear;

    const double total = fuel * (1.0 + gTariffMargin) + wear;
    return (total > 0.0) ? total : 0.0;
}

// v0.5: remember who was paid, so the close can show their cash position.
// Four slots: a republic has a handful of private logistics companies, not a
// hundred, and the balance line is about seeing money arrive and leave.
// v0.6: how much of the day's freight went to whom, so the close can say
// "freight in X, spent Y" instead of making the reader subtract.
static void NoteCarrierIncome(void* b, double amount)
{
    if (!b || !(amount > 0.0)) return;
    EnterCriticalSection(&g_lock);
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { g_balCarrierIn[i] += amount; break; }
    LeaveCriticalSection(&g_lock);
}

static void TrackCarrier(void* b)
{
    if (!b) return;
    EnterCriticalSection(&g_lock);
    bool known = false;
    for (int i = 0; i < g_balCarrierCount; i++)
        if (g_balCarrier[i] == b) { known = true; break; }
    if (!known && g_balCarrierCount < BAL_CARRIERS) {
        g_balCarrier[g_balCarrierCount]     = b;
        g_balCarrierPrev[g_balCarrierCount] = -1.0;   // -1 = no baseline yet
        g_balCarrierFuel[g_balCarrierCount] = -1.0;   // first read is a baseline
        g_balCarrierCount++;
    }
    LeaveCriticalSection(&g_lock);
}

static void AppendShipment(const Shipment& s)
{
    int cap = g_ledgerCapacity;
    if (cap < 1)          cap = 1;
    if (cap > LEDGER_MAX) cap = LEDGER_MAX;

    g_ledger[g_ledgerHead % cap] = s;
    g_ledgerHead = (g_ledgerHead + 1) % cap;
    g_ledgerTotal++;
}

// ---------------------------------------------------------------------------
// paying
// ---------------------------------------------------------------------------
//
// One place that moves money, so there is exactly one place to look when a
// balance changes. dry_run decides whether the call lands.
//
//   payee  - the building whose company is credited
//   amount - positive RUB
//   cat    - what the movement is for (enterprise_base's own classification)
//   label  - appears verbatim in currency's flow ledger

static bool Pay(void* payee, double amount, TsmCapitalCategory cat,
                const char* label, const char* kind, void* res, float tons)
{
    // v0.4: every refusal is counted HERE, where the reason is known, so the
    // daily line can say which one it was instead of lumping them together.
    if (!(amount >= (double)g_minPayment)) {
        EnterCriticalSection(&g_lock); g_skipSmall++;    LeaveCriticalSection(&g_lock);
        return false;
    }
    if (!g_enterprise || !g_currency) {
        EnterCriticalSection(&g_lock); g_skipNoPayee++;  LeaveCriticalSection(&g_lock);
        return false;
    }

    char bname[64], rname[64];
    BuildingName(payee, bname, sizeof(bname));
    ResourceName(res, rname, sizeof(rname));

    if (g_dailyPayCap > 0.0 && g_paidToday + amount > g_dailyPayCap) {
        EnterCriticalSection(&g_lock);
        g_payCapped++;
        LeaveCriticalSection(&g_lock);
        return false;
    }

    // v0.4 AFFORDABILITY GATE.
    //
    // moveTreasury takes a signed amount and has no floor: crediting a
    // private company and debiting the treasury by more than it holds would
    // simply drive the balance negative. The state is the payer until Ship 2,
    // so it has to be able to say no. Reads the ACTIVE currency's treasury
    // (RUB by default), the same one moveTreasury writes.
    {
        double have = 0.0;
        if (g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance))
            have = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        if (have < amount) {
            EnterCriticalSection(&g_lock); g_skipTreasury++; LeaveCriticalSection(&g_lock);
            return false;
        }
    }

    if (g_dryRun) {
        Logf("[PAY] would %s %s %+.2f (%s %.3ft) - dry_run",
             kind, bname, amount, rname, (double)tons);
    } else {
        // v0.5 (H33): the treasury is read on both sides of the move. This is
        // the only isolated proof of the payer leg that exists - it cannot be
        // contaminated by wages or taxes, because nothing else runs between
        // the two reads.
        const int cur   = TsmCurrencyActive(g_currency);
        const bool sample = TreasuryReadable();
        double before = 0.0, after = 0.0;
        if (sample) before = g_currency->getTreasuryBalance(cur);

        g_enterprise->modifyCompanyCapital(payee, amount, cat);
        g_currency->moveTreasury(-amount, label);

        if (sample) {
            after = g_currency->getTreasuryBalance(cur);
            const double gap = (after - before) + amount;   // 0 = exact
            const double mag = (gap < 0.0) ? -gap : gap;
            EnterCriticalSection(&g_lock);
            g_treasuryChecked++;
            if (mag <= 0.005) {
                g_treasuryExact++;
            } else {
                g_treasuryOff++;
                if (mag > g_treasuryMaxGap) g_treasuryMaxGap = mag;
            }
            LeaveCriticalSection(&g_lock);
        }

        Logf("[PAY] %s %s %+.2f (%s %.3ft) paid by the state",
             kind, bname, amount, rname, (double)tons);
    }

    EnterCriticalSection(&g_lock);
    g_paidToday += amount;
    g_payCount++;
    if (cat == TSM_CAP_PRODUCTION) g_paidFactory += amount;
    else                          g_paidCarrier += amount;
    LeaveCriticalSection(&g_lock);
    return true;
}

// ---------------------------------------------------------------------------
// the detour
// ---------------------------------------------------------------------------

static int DetourTransfer(void* game, void* p2, unsigned mode, void* res,
                          float tons, float* outMoved, float cap,
                          unsigned char flag, void* p9)
{
    // ARGUMENT NAMES THAT MATTER:
    //
    //   p2 = param_2. At mode 0 this is the BUILDING; at mode 1 the VEHICLE.
    //        Proven in two sessions (RE_DIGEST Part 13 §4).
    //   p9 = param_9. The FLEET OWNER on the delivery family, NULL on the
    //        load family. NOT an endpoint.
    //
    // Getting these two the wrong way round is the single easiest way to
    // build a payment that pays nobody, so they are named for their
    // parameter position and nothing else.

    // Which site called us. The patched instruction is still a `call`, so the
    // CPU pushed site+5 and that is what _ReturnAddress reads.
    const BYTE* retAddr = (const BYTE*)_ReturnAddress();
    unsigned    siteRva = 0;
    SiteClass   known   = SITE_UNKNOWN;
    int         siteIdx = -1;

    if (retAddr && g_exeBase) {
        const intptr_t off = (intptr_t)retAddr - (intptr_t)(g_exeBase + 5);
        if (off > 0 && off < (intptr_t)g_exeSize) {
            siteRva = (unsigned)off;
            siteIdx = SiteIndex(siteRva);
            if (siteIdx >= 0) known = g_sites[siteIdx].known;
        }
    }

    // At mode 0 p2 is the building. At mode 1 it is the vehicle and its home
    // depot is the fleet owner - read it BEFORE the call, because the callee
    // clobbers registers and the vehicle can be re-homed later in the tick.
    void* building = (mode == 0 && p2 && TsmIsValidGamePtr(p2) &&
                      ReadablePtr(p2, TSM_BLD_MIN_READABLE)) ? p2 : nullptr;
    void* owner    = p9;

    void* vehicleHome = nullptr;
    if (mode != 0 && p2 && TsmIsValidGamePtr(p2) &&
        ReadablePtr((const BYTE*)p2 + TSM_VEH_HOME, 8)) {
        vehicleHome = *(void**)((BYTE*)p2 + TSM_VEH_HOME);
    }

    EnterCriticalSection(&g_lock);
    g_calls++;
    LeaveCriticalSection(&g_lock);

    // Forward. The transfer returns a bool, so only the low byte of the return
    // carries the answer.
    const int  r         = g_realTransfer
                         ? g_realTransfer(game, p2, mode, res, tons,
                                          outMoved, cap, flag, p9)
                         : 0;
    const bool committed = ((unsigned char)r == 1u);

    // ---- classify, outside the lock --------------------------------------
    const bool   isStaging = (mode != 0 && vehicleHome && vehicleHome == p9);
    const double unitPrice = UnitPrice(res);

    Shipment s;
    memset(&s, 0, sizeof(s));
    s.vehicle  = (mode != 0) ? p2 : nullptr;
    s.resource = res;
    s.tons     = tons;
    s.site     = (unsigned short)(siteRva & 0xFFFF);
    s.mode     = (unsigned char)mode;
    s.building = building;

    if (committed) {
        if (isStaging) {
            s.kind = SHIP_LOAD;
        } else if (known == SITE_REFUEL) {
            s.kind      = SHIP_FUEL;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        } else if (tons > 0.0f) {
            s.kind      = SHIP_DELIVERY;
            s.unitPrice = unitPrice;
            s.value     = (double)tons * unitPrice;
        } else {
            s.kind      = SHIP_LOAD;
            s.unitPrice = unitPrice;
            s.value     = fabs((double)tons) * unitPrice;
        }
    }

    // ---- carrier resolution (mode 1 only feeds the ring) -----------------
    bool     emitSite   = false;
    bool     emitAttrib = false;
    unsigned shipId     = 0;
    void*    carrier    = nullptr;
    bool     carrierMatched = false;

    EnterCriticalSection(&g_lock);

    if (committed && mode != 0 && vehicleHome) {
        CarrierNote& n = g_carrier[g_carrierHead % CARRIER_RING];
        n.res  = res;
        n.tons = fabsf(tons);
        n.home = vehicleHome;
        n.t  = NowSeconds();
        g_carrierHead = (g_carrierHead + 1) % CARRIER_RING;
    }

    if (committed && mode == 0 && tons < 0.0f && !owner) {
        // The load leg: param_9 is NULL, so find the vehicle that took these
        // goods from the mode-1 call that follows within a few milliseconds.
        const double now = NowSeconds();
        for (int i = 1; i <= CARRIER_RING; i++) {
            const int k = ((g_carrierHead - i) % CARRIER_RING + CARRIER_RING) % CARRIER_RING;
            const CarrierNote& n = g_carrier[k];
            if (!n.home || n.res != res) continue;
            if (now - n.t > 2.0) continue;
            const float tol = (fabsf(tons) > 0.01f) ? fabsf(tons) * 0.005f : 0.0005f;
            if (fabsf(n.tons - fabsf(tons)) > tol) continue;
            carrier = n.home;
            carrierMatched = true;
            break;
        }
    }

    if (committed) g_callsCommitted++;
    else           g_callsNoop++;

    if (siteIdx >= 0) {
        SiteTally& t = g_tally[siteIdx];
        t.calls++;
        if (committed) {
            t.committed++;
            if (tons >= 0.0f) t.tonsIn  += (double)tons;
            else              t.tonsOut += fabs((double)tons);
            t.value += s.value;
        }
        if (p9)           t.sawBld++;
        if (vehicleHome)  t.sawHome++;
    }

    if (committed) {
        if (mode == 0 && !building)                g_unknownBuilding++;
        if (isStaging)                             g_staging++;
        if (mode == 0 && !IsPrivate(building))     g_nonPrivate++;

        if (s.kind == SHIP_FUEL)                    { g_wouldPayFuel    += s.value;
                                                      g_fuelEvents++; }
        if (s.kind == SHIP_DELIVERY && s.value > 0.0) g_wouldPayFreight += s.value;

        s.id  = g_ledgerSeq++;
        s.seq = g_ledgerTotal;
        AppendShipment(s);
        shipId = s.id;
    }

    // Separate budgets: a busy day can never hide a site behind its own
    // attribution output, and payments get a third budget of their own.
    if (g_siteLog) {
        if (g_siteLoggedToday < (unsigned)g_logCapPerDay) {
            g_siteLoggedToday++;
            emitSite = true;
        } else {
            g_siteSuppressed++;
        }
    }
    if (committed) {
        if (g_attrLoggedToday < (unsigned)g_logCapPerDay) {
            g_attrLoggedToday++;
            emitAttrib = true;
        } else {
            g_attrSuppressed++;
        }
    }

    LeaveCriticalSection(&g_lock);

    // ---- logging, outside the lock ---------------------------------------
    if (emitSite) {
        char bname[64];
        BuildingName(building, bname, sizeof(bname));
        Logf("[B2B SITE] rva=0x%06X mode=%u sign=%c tons=%.3f commit=%d flag=%u "
             "p2=%p \"%s\" priv=%d p9=%p vhome=%p class=%s",
             siteRva, mode, (tons < 0.0f ? '-' : '+'), (double)tons,
             committed ? 1 : 0, (unsigned)flag,
             p2, bname, IsPrivate(building) ? 1 : 0, p9, vehicleHome,
             ClassName(known));
    }

    // ---- billing ---------------------------------------------------------
    //
    // MODE 0 ONLY. A mode-1 call is the vehicle side of a movement a mode-0
    // call has already described; billing both would charge it twice.
    // v0.5: the probe runs even with billing off. It is a measurement, and
    // measuring a cost must not depend on whether money is being moved.
    if ((g_billing || g_tariffProbe || g_tripTariff) && committed && mode == 0 && building) {

        char rname[64];
        ResourceName(res, rname, sizeof(rname));

        double freightPaid   = 0.0;
        double probeTariff   = 0.0;
        double probeCost     = 0.0;
        double probeShare    = 0.0;
        int    probeIndex    = 0;
        bool   probeLine     = false;

        if (tons > 0.0f) {

            // ---- v0.7 THE TRIP TARIFF (this IS the money) -----------------
            // v0.6 computed this and paid the value share anyway. v0.7 pays
            // it. `trip_tariff = 0` puts the value share back untouched.
            if (g_tariffProbe || g_tripTariff) {
                probeTariff = TariffFor((double)tons, &probeCost, &probeShare,
                                        &probeIndex);

                EnterCriticalSection(&g_lock);
                if (probeTariff > 0.0) {
                    g_tariffCount++;
                    g_tariffSum       += probeTariff;
                    g_tariffCostSum   += probeCost * (1.0 + gTariffMargin);
                    g_tariffShareSum  += probeShare;
                    if (probeIndex == 1)      g_tariffFuelIndex++;
                    else if (probeIndex == 2) g_tariffCpiIndex++;
                    if (g_tariffLoggedToday < (unsigned)g_tariffLogPerDay) {
                        g_tariffLoggedToday++;
                        probeLine = true;
                    }
                } else {
                    g_tariffNoIndex++;
                }
                LeaveCriticalSection(&g_lock);
            }

            if (g_billing && g_payCarrier) {
                // H2 - freight on a delivery into `building`.
                // The fleet owner is param_9 on this family; if the site does
                // not carry one, fall back to the ring.
                void* paid = nullptr;
                if (owner && owner != building && IsPrivate(owner))      paid = owner;
                else if (!owner && carrier && carrier != building &&
                         IsPrivate(carrier))                             paid = carrier;

                if (paid) {
                    // v0.7: the trip price. The value share survives only for
                    // the comparison path (trip_tariff = 0).
                    const double freight = g_tripTariff
                        ? probeTariff
                        : FreightFor((double)tons, unitPrice);
                    if (freight <= 0.0) {
                        EnterCriticalSection(&g_lock); g_skipNoPrice++; LeaveCriticalSection(&g_lock);
                    } else if (Pay(paid, freight, TSM_CAP_WHOLESALE,
                                   "b2b_freight", "freight", res, tons)) {
                        freightPaid = freight;
                        TrackCarrier(paid);
                        NoteCarrierIncome(paid, freight);
                        BurnProbe(paid);              // v0.11: per-delivery tank sample
                    }
                } else {
                    EnterCriticalSection(&g_lock); g_skipNoPayee++; LeaveCriticalSection(&g_lock);
                }
            }

            // One line carries both numbers, so the calibration is a
            // subtraction the reader can do in their head.
            if (probeLine) {
                char bname[64];
                BuildingName(building, bname, sizeof(bname));
                Logf("[TARIFF] %s %.3ft @%.2f = %.2f -> %s: %.2f = fuel %.2f x%.2f "
                     "+ wear %.2f (index %s) | charged %.2f by the state",
                     rname, (double)tons, unitPrice, (double)tons * unitPrice, bname,
                     probeTariff, probeCost, 1.0 + (double)gTariffMargin, probeShare,
                     probeIndex == 1 ? "fuel" : (probeIndex == 2 ? "cpi" : "none"),
                     freightPaid);
            }
        }

        if (g_billing && tons < 0.0f && g_payFactory) {
            // H1 - the private building the goods leave is paid for them.
            if (IsPrivate(building)) {
                if (unitPrice > 0.0) {
                    const double value =
                        fabs((double)tons) * unitPrice * (double)g_factoryShare;
                    Pay(building, value, TSM_CAP_PRODUCTION,
                        "b2b_factory", "factory", res, tons);
                } else {
                    // A private building shipping goods this build cannot
                    // price. Count it and say so once per day-cap window:
                    // silence here would look like "nothing happened".
                    char bname[64];
                    BuildingName(building, bname, sizeof(bname));
                    EnterCriticalSection(&g_lock);
                    g_skipNoPrice++;
                    const bool say = (g_payLoggedToday < (unsigned)g_logCapPerDay);
                    if (say) g_payLoggedToday++;
                    LeaveCriticalSection(&g_lock);
                    if (say)
                        Logf("[PAY] skipped %s: no price for %s (unitPrice 0.00) - "
                             "resource_pricing absent or unpriced", bname, rname);
                }
            }
        }

        (void)carrierMatched;
    }

    if (emitAttrib) {
        char bname[64], rname[64];
        BuildingName(building, bname, sizeof(bname));
        ResourceName(res, rname, sizeof(rname));
        Logf("[B2B] #%u %-6s bld=%p \"%s\" priv=%d res=%s %.2ft @%.2f = %.2f "
             "(site 0x%06X, mode %u)",
             shipId, KindName(s.kind), building, bname, IsPrivate(building) ? 1 : 0,
             rname, (double)s.tons, s.unitPrice, s.value, siteRva, mode);
    }

    return r;
}

// ---------------------------------------------------------------------------
// call-site patching
// ---------------------------------------------------------------------------

static bool PatchSite(int index)
{
    if (index < 0 || index >= g_siteCount) return false;
    if (g_sitePatched[index]) return true;

    const unsigned rva = g_sites[index].rva;

    if (rva < 0x1000 || (size_t)rva + 5 > g_exeSize) {
        Logf("private_logistics  site 0x%06X is outside the image (%llu bytes) - skipped",
             rva, (unsigned long long)g_exeSize);
        return false;
    }

    BYTE* site = g_exeBase + rva;
    if (!ReadablePtr(site, 5)) {
        Logf("private_logistics  site 0x%06X not readable - skipped", rva);
        return false;
    }

    // Verify: a direct call, landing on the transfer.
    if (site[0] != 0xE8) {
        Logf("private_logistics  site 0x%06X is not a call (0x%02X) - skipped",
             rva, site[0]);
        return false;
    }
    const int32_t rel    = *(const int32_t*)(site + 1);
    BYTE*         target = site + 5 + rel;
    if (target != g_exeBase + TSM_RVA_TRANSFER) {
        Logf("private_logistics  site 0x%06X calls %p, not the transfer - skipped",
             rva, target);
        return false;
    }

    // The cave: `jmp qword ptr [rip+0]` plus the absolute detour address.
    BYTE* cave = AllocNear(site, 32);
    if (!cave) {
        Logf("private_logistics  no code cave near 0x%06X - skipped", rva);
        return false;
    }

    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&DetourTransfer;
    memcpy(cave + 6, &detour, 8);
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("private_logistics  cave out of rel32 range at 0x%06X - skipped", rva);
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("private_logistics  VirtualProtect failed at 0x%06X - nothing written", rva);
        return false;
    }

    // Re-verify under the protection.
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("private_logistics  site 0x%06X changed under us - nothing written", rva);
        return false;
    }

    site[0] = 0xE8;                              // same opcode: still a call
    *(int32_t*)(site + 1) = (int32_t)toCave;     // new destination
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    g_sitePatched[index] = true;
    Logf("private_logistics  hook 0x%06X -> transfer ok (cave %p)", rva, (void*)cave);
    return true;
}

static void PatchAllSites(void)
{
    if (!g_siteFilterActive)
        for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = true;

    int ok = 0, wanted = 0;
    for (int i = 0; i < g_siteCount; i++) {
        if (!g_siteWanted[i]) continue;
        wanted++;
        if (PatchSite(i)) ok++;
    }

    if (g_siteFilterActive)
        Logf("private_logistics  site_list active: %d of %d sites selected",
             wanted, g_siteCount);

    Logf("private_logistics  patched %d of %d transfer call sites", ok, wanted);

    if (ok == 0) {
        Logf("private_logistics  ERROR  no sites patched. Either this is not "
             "SOVIET64.exe v1.1.1.9, or the transfer is not at RVA 0x420350. "
             "Nothing was written; the game is unaffected.");
    } else if (ok < wanted) {
        Logf("private_logistics  WARN  %d site(s) refused - see the lines above. "
             "A refusal is safe: that call still reaches the engine normally.",
             wanted - ok);
    }
}

// ---------------------------------------------------------------------------
// the daily roll-up
// ---------------------------------------------------------------------------

static void OnDay(void*, int day, int month, int year)
{
    (void)year;

    // Snapshot under the lock, clear everything, print outside it.
    //
    // v0.3 BUGFIX: v0.2 snapshotted these counters without zeroing them, so
    // its "[B2B] day N" line was cumulative since load while the per-site
    // table was genuinely per-day. Both are per-day now.
    static SiteTally snap[MAX_SITES];
    unsigned calls, committed, noop, unknown, stag, nonPriv;
    unsigned siteSupp, attrSupp, paySupp, payCount, payCap, total;
    unsigned skNoPayee, skNoPrice, skSmall, skTreasury;
    unsigned tCount, tNoIndex, tFuelIdx, tCpiIdx;
    unsigned trChecked, trExact, trOff, fuelEvents;
    double   paidF, paidC, freight, fuel, tSum, tCostSum, tShareSum, trMaxGap;
    static double cIn[BAL_CARRIERS];
    static void*  cPt[BAL_CARRIERS];
    int      cCount;
    bool     newMonth;

    EnterCriticalSection(&g_lock);
    memcpy(snap, g_tally, sizeof(snap));
    memset(g_tally, 0, sizeof(g_tally));

    calls      = g_calls;
    committed  = g_callsCommitted;
    noop       = g_callsNoop;
    unknown    = g_unknownBuilding;
    stag       = g_staging;
    nonPriv    = g_nonPrivate;
    siteSupp   = g_siteSuppressed;
    attrSupp   = g_attrSuppressed;
    paySupp    = g_paySuppressed;
    payCount   = g_payCount;
    payCap     = g_payCapped;
    skNoPayee  = g_skipNoPayee;
    skNoPrice  = g_skipNoPrice;
    skSmall    = g_skipSmall;
    skTreasury = g_skipTreasury;
    tCount     = g_tariffCount;
    tNoIndex   = g_tariffNoIndex;
    tFuelIdx   = g_tariffFuelIndex;
    tCpiIdx    = g_tariffCpiIndex;
    tSum       = g_tariffSum;
    trChecked  = g_treasuryChecked;
    trExact    = g_treasuryExact;
    trOff      = g_treasuryOff;
    trMaxGap   = g_treasuryMaxGap;
    fuelEvents = g_fuelEvents;
    tCostSum   = g_tariffCostSum;
    tShareSum  = g_tariffShareSum;
    cCount     = g_balCarrierCount;
    for (int i = 0; i < BAL_CARRIERS; i++) {
        cPt[i] = g_balCarrier[i];
        cIn[i] = g_balCarrierIn[i];
        g_balCarrierIn[i] = 0.0;      // this day's income, spent by the close
    }
    total      = g_ledgerTotal;
    paidF      = g_paidFactory;
    paidC      = g_paidCarrier;
    freight    = g_wouldPayFreight;
    fuel       = g_wouldPayFuel;

    g_calls = g_callsCommitted = g_callsNoop = 0;
    g_unknownBuilding = g_staging = g_nonPrivate = 0;
    g_siteSuppressed = g_attrSuppressed = g_paySuppressed = 0;
    g_payCount = g_payCapped = 0;
    g_skipNoPayee = g_skipNoPrice = g_skipSmall = g_skipTreasury = 0;
    g_tariffCount = g_tariffNoIndex = g_tariffFuelIndex = g_tariffCpiIndex = 0;
    g_tariffCostSum = g_tariffShareSum = 0.0;
    g_tariffSum = 0.0;
    g_treasuryChecked = g_treasuryExact = g_treasuryOff = 0;
    g_treasuryMaxGap = 0.0;
    g_fuelEvents = 0;
    g_fuelRes = nullptr;          // re-resolve the index next day
    g_paidFactory = g_paidCarrier = 0.0;
    g_wouldPayFreight = g_wouldPayFuel = 0.0;
    g_paidToday = 0.0;

    g_siteLoggedToday = 0;
    g_attrLoggedToday = 0;
    g_payLoggedToday  = 0;
    g_tariffLoggedToday = 0;

    newMonth = (month != g_lastMonth);
    g_lastMonth = month;

    // v0.4: this callback fires at the START of `day`, so everything above is
    // the day that just ENDED. Report that day, not this one - the old label
    // was one ahead of its own numbers.
    // A loaded save can rewind the clock; do not report a day in the future.
    if (g_summaryDay > day) g_summaryDay = -1;
    const int closedDay = (g_summaryDay >= 0) ? g_summaryDay : (day - 1);
    g_summaryDay = day;
    LeaveCriticalSection(&g_lock);

    Logf("[B2B] day %d closed: %u calls, %u committed, %u no-op | unknown %u, "
         "staging %u, non-private %u | suppressed site %u attrib %u pay %u",
         closedDay, calls, committed, noop, unknown, stag, nonPriv, siteSupp,
         attrSupp, paySupp);

    // Per-site table. A site that never fired is simply absent - absence is
    // evidence rather than a logging artefact.
    for (int i = 0; i < g_siteCount; i++) {
        const SiteTally& t = snap[i];
        if (t.calls == 0) continue;
        Logf("[B2B SITE] day %d  0x%06X %-9s calls=%u commit=%u in=%.3f out=%.3f "
             "value=%.2f | arg9!=0 %u home!=0 %u",
             closedDay, g_sites[i].rva, ClassName(g_sites[i].known), t.calls,
             t.committed, t.tonsIn, t.tonsOut, t.value, t.sawBld, t.sawHome);
    }

    if (g_billing) {
        Logf("[PAY] day %d closed: %u payment(s), %u capped | skipped: no payee %u, "
             "no price %u, too small %u, treasury %u | factory %.2f, carrier %.2f | %s",
             closedDay, payCount, payCap, skNoPayee, skNoPrice, skSmall,
             skTreasury, paidF, paidC,
             g_dryRun ? "DRY RUN - nothing moved"
                      : "PAID by the state treasury");
    } else {
        Logf("[PAY] day %d closed: billing off", closedDay);
    }

    // v0.5: the model's own total, beside the index it managed to use.
    // `unindexed` is the honest failure: neither a fuel price nor a CPI
    // basket resolved, so the probe refused to invent a figure.
    if (g_tariffProbe || g_tripTariff) {
        Logf("[TARIFF] day %d closed: %u delivery(ies), %s %.2f = fuel %.2f x%.2f + "
             "wear %.2f, mean %.2f | indexed by fuel %u, by cpi %u, unindexed %u",
             closedDay, tCount, g_tripTariff ? "charged" : "model",
             tSum, tCostSum, 1.0 + (double)gTariffMargin, tShareSum,
             tCount ? tSum / (double)tCount : 0.0,
             tFuelIdx, tCpiIdx, tNoIndex);
    }

    // v0.5: THE MONEY PATH, BOTH ENDS (H33, H37).
    //
    // v0.4 proved the payee's account to the kopeck; it never read the payer.
    // Here the treasury is sampled at every close, so consecutive closes give
    // the movement, and `unexplained` is what the [PAY] lines cannot account
    // for - zero when the books close, non-zero when something else touched
    // the treasury in the same day.
    //
    // The carriers' capital is printed beside it. Income minus everything the
    // company spent, fuel included: if the game charges a private DO for
    // diesel, the difference between this delta and the freight it was paid
    // IS that bill - the number Ship 2 has to price against.
    if (g_balanceLog) {
        const int cur = g_currency ? TsmCurrencyActive(g_currency) : 0;
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(cur);
            have  = true;
        }

        const double paid = paidF + paidC;

        if (!have) {
            Logf("[BAL] day %d closed: treasury unavailable (no currency service)", closedDay);
        } else if (!g_treasuryHaveClose) {
            Logf("[BAL] day %d closed: treasury %.2f (baseline - the next close gives the "
                 "movement) | paid %.2f", closedDay, treas, paid);
        } else {
            const double moved = treas - g_treasuryAtClose;
            Logf("[BAL] day %d closed: treasury %.2f -> %.2f (%+.2f) | this plugin paid "
                 "%.2f | the rest of the movement is the rest of the state's day%s",
                 closedDay, g_treasuryAtClose, treas, moved, paid,
                 g_dryRun ? " | DRY RUN: nothing should have moved" : "");
        }
        if (have) { g_treasuryAtClose = treas; g_treasuryHaveClose = true; }

        // v0.5 (H33): the proof, per payment rather than per day.
        if (trChecked > 0 || trOff > 0) {
            Logf("[BAL] day %d treasury leg: %u payment(s) sampled either side, "
                 "%u moved exactly the amount, %u off by any margin (largest %.2f)",
                 closedDay, trChecked, trExact, trOff, trMaxGap);
        }

        // v0.6: the carrier's day in one line. `spent` is the arithmetic
        // difference - freight in, minus the change in capital - and in this
        // save it is the fuel bill, because fuel is the only thing a private
        // DO buys (it arrives as a cat=2 FOREIGN debit on the same account).
        for (int i = 0; i < cCount; i++) {
            void* b = cPt[i];
            if (!b || !g_enterprise || !g_enterprise->getCompanyCapital) continue;
            const double cap = g_enterprise->getCompanyCapital(b);
            char bname[64];
            BuildingName(b, bname, sizeof(bname));
            if (g_balCarrierPrev[i] < 0.0) {
                Logf("[BAL] day %d   %s capital %.2f (baseline) | freight in %.2f",
                     closedDay, bname, cap, cIn[i]);
            } else {
                const double delta = cap - g_balCarrierPrev[i];
                Logf("[BAL] day %d   %s capital %.2f (%+.2f) | freight in %.2f | spent %.2f",
                     closedDay, bname, cap, delta, cIn[i], cIn[i] - delta);
            }

            // v0.7: THE FUEL METER.
            //
            // monthWholesale is where enterprise_base books cat=2 FOREIGN
            // debits, and diesel is the only foreign purchase a distribution
            // office makes - so this is the carrier's real fuel bill for the
            // calendar month, read back from the game rather than assumed.
            //
            // Fuel is bought in bulk and stored, so a day's purchases are not
            // a day's burn. The run average is, because the tank delta washes
            // out of a long run. That is the number the [TARIFF] model is
            // scored against, and the number litres_per_trip should hold.
            if (g_enterprise->describeEnterprise) {
                TsmEnterpriseInfo info;
                memset(&info, 0, sizeof(info));
                info.structSize = sizeof(TsmEnterpriseInfo);
                if (g_enterprise->describeEnterprise(b, &info) == 1) {
                    const double bill = info.monthWholesale;
                    double spent = 0.0;
                    if (g_balCarrierFuel[i] < 0.0) {
                        // First read after a load: monthWholesale already holds
                        // whatever the save carried. Take it as the baseline and
                        // say so - counting it would bill one day for a month.
                        g_balCarrierFuel[i] = bill;
                        Logf("[FUEL] day %d   %s meter baseline: month wholesale %.2f "
                             "(not counted as this day's burn)", closedDay, bname, bill);
                    } else {
                        spent = (bill < g_balCarrierFuel[i])
                              ? bill                      // month boundary: accumulators cleared
                              : (bill - g_balCarrierFuel[i]);
                        g_balCarrierFuel[i] = bill;
                    }
                    g_fuelPaidRun += (spent > 0.0) ? spent : 0.0;
                    const double fprice = FuelUnitPrice();
                    double litres = 0.0;
                    if (fprice > 0.0 && gDieselDensity > 0.0)
                        litres = ((spent > 0.0) ? spent : 0.0) / (gDieselDensity * fprice);
                    g_fuelLitresRun += litres;
                    Logf("[FUEL] day %d   %s diesel %.2f (measured, cat=2) = %.2f L at %.2f/%s",
                         closedDay, bname, (spent > 0.0) ? spent : 0.0, litres, fprice,
                         g_currency ? TsmCurrencySymbol(g_currency) : "RUB");
                }
            }
            g_balCarrierPrev[i] = cap;
        }

        // v0.7: the run total, and the one number to copy into the ini.
        g_fuelTripsRun += tCount;
        if (g_fuelTripsRun > 0) {
            const double perTrip = g_fuelLitresRun / (double)g_fuelTripsRun;
            // v0.9: this counts PURCHASES, not burn. Fuel is bought in bulk
            // and sits in tanks, so a short run and a long run give different
            // answers from the same fleet - 3.27 L/trip over 1,100 trips,
            // 7.14 over 214. Do not treat it as a measurement of the burn.
            Logf("[FUEL] run: %.2f RUB = %.2f L BOUGHT over %llu delivery(ies) = %.3f L/trip "
                 "| configured %.2f L | fuel sits in tanks: this is purchases, not burn",
                 g_fuelPaidRun, g_fuelLitresRun, g_fuelTripsRun, perTrip, gLitresPerTrip);
        }

        // v0.10: THE BURN, from the tanks.
        //
        // This is the number the tariff has been missing since v0.5. It is not
        // purchases (which include tank fill), it is not a model (which was
        // wrong by 4x): it is what actually left the vehicles' tanks while
        // they worked. Refills are counted separately and never subtract.
        if (g_fuelTripsRun > 0 && g_burnTonsRun > 0.0 && gDieselDensity > 0.0) {
            const double litres = g_burnTonsRun / gDieselDensity;
            const double lpt    = litres / (double)g_fuelTripsRun;
            // v0.11: L/DAY is printed beside L/trip because the physics is
            // per-tick. L/trip moves whenever the shops order differently -
            // the same fleet gave 3.27 and 7.14 L/trip in two runs - while
            // L per fleet-day is the same quantity in both. Read this one.
            const double lpd = g_tankDaysRun ? litres / (double)g_tankDaysRun : 0.0;
            const double lvd = g_tankVehDaysRun ? litres / (double)g_tankVehDaysRun : 0.0;
            const double lva = g_activeVehDaysRun ? litres / (double)g_activeVehDaysRun : 0.0;
            Logf("[TANK] run: burned %.4f t = %.1f L | %.2f L/trip over %llu delivery(ies) "
                 "| %.2f L per fleet-day over %u close(s) | %.2f L per truck-day over %u "
                 "| %.2f L per ACTIVE truck-day over %u active / %u idle "
                 "| %u refill(s) | configured %.2f L/trip",
                 g_burnTonsRun, litres, lpt, g_fuelTripsRun, lpd, g_tankDaysRun, lvd,
                 g_tankVehDaysRun, lva, g_activeVehDaysRun, g_idleVehDaysRun,
                 g_refillsRun, gLitresPerTrip);
            if (g_litresAuto) {
                const double before = gLitresPerTrip;
                gLitresPerTrip = before * 0.70 + lpt * 0.30;
                if (gLitresPerTrip < 0.5)   gLitresPerTrip = 0.5;
                if (gLitresPerTrip > 500.0) gLitresPerTrip = 500.0;
                if (fabs(gLitresPerTrip - before) > 0.01)
                    Logf("[TANK] auto-tune: litres_per_trip %.3f -> %.3f (tank says %.3f)",
                         before, gLitresPerTrip, lpt);
            }
        }
    }

    // v0.11: THE MEASURED BURN, per delivery rather than per day.
    //
    // This is the number the player asked for: diesel that actually left the
    // tanks between one delivery and the next. It needs no purchases, no
    // storage, and no guess about how far a truck drives. Once it has enough
    // intervals it replaces the configured figure - one fleet, one republic,
    // measured where it is used.
    if (g_probeStepsRun > 0 && gDieselDensity > 0.0) {
        const double litres = g_probeBurnRun / gDieselDensity;
        const double lpt    = litres / (double)g_probeStepsRun;
        Logf("[BURN] run: measured %.4f t = %.1f L between %u delivery pair(s) "
             "= %.2f L/trip | %u interval(s) skipped (refill or fleet change) "
             "| configured %.2f L/trip | %s",
             g_probeBurnRun, litres, g_probeStepsRun, lpt, g_probeSkipRun,
             gLitresPerTrip,
             (g_probeStepsRun >= 20 && fabs(lpt - gLitresPerTrip) > 0.5)
                 ? "SET litres_per_trip TO THE MEASURED FIGURE"
                 : "no action");
    }

    // v0.8: who does this company actually own, and what can they carry?
    CensusFleets(closedDay);

    // v0.6: re-read the index at every close. At plugin start the world has
    // not loaded, so v0.5's "fuel NOT FOUND" and "treasury opens at 0.00" were
    // both true for about a second and then false forever. Printing them here
    // is the honest place.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);
        Logf("[IDX] day %d closed: fuel '%s' = %.2f/%s (%s) | cpi basket %.2f/day | "
             "margin %.0f%%, wear %.2f L",
             closedDay, gFuelResource, fp,
             g_currency ? TsmCurrencySymbol(g_currency) : "RUB",
             fp > 0.0 ? "resolved" : "NOT RESOLVED",
             (double)basket, gTariffMargin * 100.0, gWearLitres);
    }

    // v0.5: fuel telemetry. The refuel site has been silent in every session
    // so far; a count of 0 here after fuel is switched on is itself a result.
    Logf("[FUEL] day %d refuel sites: %u call(s), %.2f RUB of fuel moved through "
         "the hooked sites", closedDay, fuelEvents, fuel);

    if (newMonth) {
        Logf("[B2B] month %d: %u shipments recorded, %.2f RUB would have moved "
             "(freight %.2f, fuel %.2f)",
             month, total, freight + fuel, freight, fuel);
    }

    AutoTargetsOnDay(day, closedDay);
}

// ---------------------------------------------------------------------------
// automatic private DO targets and infrastructure discovery (v0.16)
// ---------------------------------------------------------------------------

#define AUTO_RVA_VECTOR_PUSH_BACK 0x08E050
#define AUTO_RVA_TASKCFG_CTOR     0x1512A0
#define AUTO_DO_TARGETS_BEGIN     0x0D38
#define AUTO_DO_TARGETS_END       0x0D40
#define AUTO_DO_DIRTY             0x0D50
#define AUTO_DO_UI_TIMER          0x0FC0
// Conservative plugin-side bound: native add-flow supports five selections,
// but panel+0x310 per-target control-array capacity above five is unverified.
// This limits new auto targets without deleting/purging any manual entries.
#define AUTO_DO_SAFE_TARGET_COUNT 5
#define AUTO_DO_ROAD_TYPE         43
#define AUTO_DO_RAIL_TYPE         52
#define AUTO_STORAGE_BUILDING_TYPE 5
#define AUTO_CUSTOMS_TYPE         20
#define AUTO_CUSTOMS_MAX          512
#define AUTO_STATE_STORAGE_MAX    512
#define AUTO_STATE_PERMISSION_MAX 512
#define AUTO_STATE_PREF_FILE "private_logistics_state.bin"
struct AutoCustomSite { void* building; float x; float z; char name[64]; };
// permission bits: 1=load FROM site, 2=deliver INTO site; zero=deny both.
struct AutoStateStorageSite { void* building; uint64_t key; float x; float z; char name[64]; unsigned char permission; };
struct AutoStatePermission { uint64_t key; unsigned char mode; };
static AutoCustomSite g_autoCustomSites[AUTO_CUSTOMS_MAX];
static AutoStateStorageSite g_autoStateStorageSites[AUTO_STATE_STORAGE_MAX];
static unsigned g_autoStateStorageCount = 0;
static AutoStatePermission g_statePermissions[AUTO_STATE_PERMISSION_MAX];
static unsigned g_statePermissionCount = 0;
#define AUTO_TARGET_ENTRY_SIZE    0x18
#define AUTO_TASKCFG_SIZE         0x198
#define AUTO_STORAGE_STRIDE       0xE0
#define AUTO_MAX_TARGETS_READ     65536

static uint64_t AutoHashMix(uint64_t h, uint8_t byte)
{
    return (h ^ byte) * 1099511628211ull;
}

static bool AutoReadBuildingType(void* b, int* outType)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return false;
    void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td) || !ReadablePtr((BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
        return false;
    *outType = *(int*)((BYTE*)td + TSM_TD_TYPE_NUM);
    return true;
}

static bool AutoBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // The building's position is the translation row of its embedded
    // C3D_NODE, not bld+0x08/+0x10 (those yielded zeros in the v0.15 probe).
    const BYTE* node = (const BYTE*)b + TSM_BLD_NODE;
    if (ReadablePtr(node, 0x40)) {
        const float x = *(const float*)(node + 0x30);
        const float z = *(const float*)(node + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            if (outX) *outX = x;
            if (outZ) *outZ = z;
            return true;
        }
    }

    // Some unfinished sites can lack a usable node transform; use the
    // verified placement-bbox fallback used by enterprise_base.
    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                const float x = mn[1], z = mn[3];
                if (x == x && z == z && fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
                    if (outX) *outX = x;
                    if (outZ) *outZ = z;
                    return true;
                }
            }
        }
    }
    return false;
}

// Stable session/save identity: native object name + type + quantized position.
// No raw Building* is retained in the retry ledger.
static bool AutoBuildingKey(void* b, uint64_t* outKey, int* outType)
{
    int type = -1;
    float x = 0.0f, z = 0.0f;
    if (!AutoReadBuildingType(b, &type) || !AutoBuildingPosition(b, &x, &z))
        return false;

    uint64_t h = 14695981039346656037ull;
    const char* objectName = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { objectName = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { objectName = nullptr; }
    }
    if (objectName) {
        for (size_t i = 0; i < 128; ++i) {
            if (!ReadablePtr(objectName + i, 1)) return false;
            const uint8_t c = (uint8_t)objectName[i];
            h = AutoHashMix(h, c);
            if (c == 0) break;
        }
    }
    const int32_t qx = (int32_t)(x * 10.0f + (x >= 0.0f ? 0.5f : -0.5f));
    const int32_t qz = (int32_t)(z * 10.0f + (z >= 0.0f ? 0.5f : -0.5f));
    const uint32_t values[3] = { (uint32_t)type, (uint32_t)qx, (uint32_t)qz };
    const uint8_t* bytes = (const uint8_t*)values;
    for (size_t i = 0; i < sizeof(values); ++i) h = AutoHashMix(h, bytes[i]);
    // H1 FIX: mix raw float bits to break 0.1m quantized collisions (two clones within 10cm)
    // keeps qx/qz for backward compat but adds sub-decimeter entropy; one-time permission reset for old saves
    uint32_t fx = 0, fz = 0;
    memcpy(&fx, &x, sizeof(fx));
    memcpy(&fz, &z, sizeof(fz));
    const uint8_t* fxB = (const uint8_t*)&fx; for (size_t i = 0; i < sizeof(fx); ++i) h = AutoHashMix(h, fxB[i]);
    const uint8_t* fzB = (const uint8_t*)&fz; for (size_t i = 0; i < sizeof(fz); ++i) h = AutoHashMix(h, fzB[i]);
    *outKey = h ? h : 1;
    if (outType) *outType = type;
    return true;
}

static void AutoBuildingName(void* b, char* out, size_t capacity)
{
    if (!out || capacity == 0) return;
    out[0] = 0;
    const char* name = nullptr;
    if (g_enterprise && g_enterprise->getBuildingObjectName) {
        __try { name = g_enterprise->getBuildingObjectName(b); }
        __except (EXCEPTION_EXECUTE_HANDLER) { name = nullptr; }
    }
    if (name) {
        size_t i = 0;
        for (; i + 1 < capacity; ++i) {
            if (!ReadablePtr(name + i, 1)) break;
            out[i] = name[i];
            if (out[i] == 0) return;
        }
        out[i] = 0;
        if (i > 0) return;
    }
    const char fallback[] = "unknown";
    size_t j = 0;
    while (j + 1 < capacity && fallback[j]) { out[j] = fallback[j]; ++j; }
    out[j] = 0;
}

static unsigned char AutoStatePermissionGet(uint64_t key)
{
    if (!key) return 0;
    unsigned char mode = 0;
    EnterCriticalSection(&g_lock);
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { mode = g_statePermissions[i].mode; break; }
    }
    LeaveCriticalSection(&g_lock);
    return mode;
}

static bool AutoStatePermissionSet(uint64_t key, unsigned char mode)
{
    if (!key || mode > AUTO_ALLOW_BOTH) return false;
    EnterCriticalSection(&g_lock);
    unsigned found = g_statePermissionCount;
    for (unsigned i = 0; i < g_statePermissionCount; ++i) {
        if (g_statePermissions[i].key == key) { found = i; break; }
    }
    if (mode == 0) {
        if (found < g_statePermissionCount) {
            g_statePermissions[found] = g_statePermissions[g_statePermissionCount - 1];
            --g_statePermissionCount;
        }
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (found < g_statePermissionCount) {
        g_statePermissions[found].mode = mode;
        LeaveCriticalSection(&g_lock);
        return true;
    }
    if (g_statePermissionCount >= AUTO_STATE_PERMISSION_MAX) {
        LeaveCriticalSection(&g_lock);
        return false;
    }
    g_statePermissions[g_statePermissionCount].key = key;
    g_statePermissions[g_statePermissionCount].mode = mode;
    ++g_statePermissionCount;
    LeaveCriticalSection(&g_lock);
    return true;
}

static bool AutoHasNativeStorage(void* b);

static unsigned AutoBuildingCapabilities(void* building)
{
    int type = -1;
    if (!AutoReadBuildingType(building, &type) || !AutoHasNativeStorage(building)) return 0;
    char asset[128];
    AutoBuildingName(building, asset, sizeof(asset));
    return AutoSiteCapabilities(type, asset);
}

static void AutoDrawStatePermissions(uint64_t key, unsigned capabilities)
{
    const unsigned char oldMode = AutoStatePermissionGet(key);
    unsigned char newMode = (unsigned char)(oldMode & capabilities);
    if (capabilities & AUTO_ALLOW_LOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_LOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO loading (take goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_LOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (capabilities & AUTO_ALLOW_UNLOAD) {
        unsigned checked = AutoNativeCheckboxValue((newMode & AUTO_ALLOW_UNLOAD) != 0);
        if (g_uiPanels->widgetCheckbox(&checked, L"Allow private DO deliveries (receive goods)", 0))
            newMode = AutoPermissionToggle(newMode, AUTO_ALLOW_UNLOAD, AutoNativeCheckboxChecked(checked), capabilities);
    }
    if (newMode != oldMode) {
        if (!AutoStatePermissionSet(key, newMode)) {
            Logf("[AUTO DO] state permission table full; change not retained");
            return;
        }
        Logf("[AUTO DO] state permissions key=%016llX load=%u unload=%u",
             (unsigned long long)key, (newMode & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (newMode & AUTO_ALLOW_UNLOAD) ? 1u : 0u);
    }
}

static void StateStorageWidgetProvider(void* building, void*)
{
    if (!g_autoTargetsEnabled || !g_uiPanels || !TsmUiPanelsHasWidgets(g_uiPanels) ||
        !building || !TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE) || !g_enterprise ||
        !g_enterprise->isBuildingPrivate || g_enterprise->isBuildingPrivate(building)) return;
    const unsigned capabilities = AutoBuildingCapabilities(building);
    uint64_t key = 0;
    if (!AutoBuildingKey(building, &key, nullptr)) return;
    static uint64_t lastPanelKey = 0;
    if (lastPanelKey != key) {
        lastPanelKey = key;
        int type = -1; AutoReadBuildingType(building, &type);
        char asset[128]; AutoBuildingName(building, asset, sizeof(asset));
        const unsigned effective = AutoStatePermissionGet(key) & capabilities;
        Logf("[AUTO DO] state panel key=%016llX type=%d asset=%s capabilities=%u permissions=%u native_load=%u native_unload=%u (native 0=checked,1=unchecked)%s",
             (unsigned long long)key, type, asset, capabilities, effective,
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_LOAD) != 0),
             AutoNativeCheckboxValue((effective & AUTO_ALLOW_UNLOAD) != 0),
             capabilities ? "" : " (unsupported role: controls hidden, automatic access denied)");
    }
    if (!capabilities) return; // unknown roles do not get a misleading loading control
    AutoDrawStatePermissions(key, capabilities);
}

static bool AutoHasNativeStorage(void* b)
{
    if (!b || !TsmIsValidGamePtr(b) ||
        !ReadablePtr((BYTE*)b + TSM_BLD_STORAGES_BEGIN, 16)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_BEGIN);
    const uintptr_t end = (uintptr_t)*(void**)((BYTE*)b + TSM_BLD_STORAGES_END);
    if (!begin || end <= begin || ((end - begin) % AUTO_STORAGE_STRIDE) != 0) return false;
    const uintptr_t count = (end - begin) / AUTO_STORAGE_STRIDE;
    return count <= 4096 && ReadablePtr((const void*)begin, count * AUTO_STORAGE_STRIDE);
}

static bool AutoReadTargetVector(void* office, void*** outBegin, size_t* outCount)
{
    if (!office || !TsmIsValidGamePtr(office) ||
        !ReadablePtr((BYTE*)office + AUTO_DO_TARGETS_BEGIN, 24)) return false;
    const uintptr_t begin = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_BEGIN);
    const uintptr_t end   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END);
    const uintptr_t cap   = (uintptr_t)*(void**)((BYTE*)office + AUTO_DO_TARGETS_END + 8);
    if (!begin && !end && !cap) { *outBegin = nullptr; *outCount = 0; return true; }
    if (!begin || !end || !cap || end < begin || cap < end || ((end - begin) & 7) != 0)
        return false;
    const size_t count = (size_t)((end - begin) / sizeof(void*));
    if (count > AUTO_MAX_TARGETS_READ ||
        !ReadablePtr((const void*)begin, count * sizeof(void*))) return false;
    *outBegin = (void**)begin; *outCount = count;
    return true;
}

static unsigned AutoOfficeHash(uint64_t officeKey)
{
    officeKey ^= officeKey >> 33; officeKey *= 0xff51afd7ed558ccdull;
    officeKey ^= officeKey >> 33; officeKey *= 0xc4ceb9fe1a85ec53ull;
    officeKey ^= officeKey >> 33;
    return (unsigned)officeKey & (AUTO_OFFICE_HASH_SIZE - 1);
}

static bool AutoBuildOfficeMemoIndex(void)
{
    memset(g_autoOfficeMemoHeads, 0, sizeof(g_autoOfficeMemoHeads));
    for (unsigned i = 0; i < g_autoMemoCount; ++i) {
        AutoMemo& memo = g_autoMemo[i];
        unsigned slot = AutoOfficeHash(memo.officeKey);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
            AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
            if (head.officeKey == 0 || head.officeKey == memo.officeKey) {
                head.officeKey = memo.officeKey;
                memo.nextOffice = head.headPlusOne ? head.headPlusOne - 1 : 0xFFFFFFFFu;
                head.headPlusOne = i + 1;
                inserted = true;
                break;
            }
            slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoOfficeMemoHeadIndex(uint64_t officeKey)
{
    unsigned slot = AutoOfficeHash(officeKey);
    for (unsigned probe = 0; probe < AUTO_OFFICE_HASH_SIZE; ++probe) {
        const AutoOfficeMemoHead& head = g_autoOfficeMemoHeads[slot];
        if (head.officeKey == 0) return -1;
        if (head.officeKey == officeKey)
            return head.headPlusOne ? (int)(head.headPlusOne - 1) : -1;
        slot = (slot + 1) & (AUTO_OFFICE_HASH_SIZE - 1);
    }
    return -1;
}

static unsigned AutoPairHash(uint64_t officeKey, uint64_t targetKey)
{
    uint64_t h = officeKey ^ (targetKey + 0x9E3779B97F4A7C15ull + (officeKey << 6) + (officeKey >> 2));
    h ^= h >> 30; h *= 0xBF58476D1CE4E5B9ull;
    h ^= h >> 27; h *= 0x94D049BB133111EBull;
    h ^= h >> 31;
    return (unsigned)h & (AUTO_MEMO_HASH_SIZE - 1);
}

static int AutoMemoFind(uint64_t officeKey, uint64_t targetKey)
{
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        const unsigned stored = g_autoMemoHash[slot];
        if (stored == 0) return -1;
        const unsigned index = stored - 1;
        if (g_autoMemo[index].officeKey == officeKey &&
            g_autoMemo[index].targetKey == targetKey) return (int)index;
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static int AutoPointerIndex(void* building)
{
    if (!building) return -1;
    uintptr_t h = (uintptr_t)building;
    h >>= 4; h ^= h >> 17; h ^= h >> 9;
    unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
    for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
        AutoPointerSlot& item = g_autoPointerHash[slot];
        if (!item.building) return -1;
        if (item.building == building) return (int)item.entityIndex;
        slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoBuildPointerIndex(unsigned entityCount)
{
    memset(g_autoPointerHash, 0, sizeof(g_autoPointerHash));
    for (unsigned i = 0; i < entityCount; ++i) {
        uintptr_t h = (uintptr_t)g_autoEntities[i].building;
        h >>= 4; h ^= h >> 17; h ^= h >> 9;
        unsigned slot = (unsigned)h & (AUTO_POINTER_HASH_SIZE - 1);
        bool inserted = false;
        for (unsigned probe = 0; probe < AUTO_POINTER_HASH_SIZE; ++probe) {
            AutoPointerSlot& item = g_autoPointerHash[slot];
            if (!item.building) {
                item.building = g_autoEntities[i].building;
                item.entityIndex = i;
                inserted = true;
                break;
            }
            if (item.building == g_autoEntities[i].building) { inserted = true; break; }
            slot = (slot + 1) & (AUTO_POINTER_HASH_SIZE - 1);
        }
        if (!inserted) return false;
    }
    return true;
}

static int AutoMemoInsert(uint64_t officeKey, uint64_t targetKey)
{
    if (g_autoMemoCount >= AUTO_MEMO_MAX) return -1;
    unsigned slot = AutoPairHash(officeKey, targetKey);
    for (unsigned probe = 0; probe < AUTO_MEMO_HASH_SIZE; ++probe) {
        if (g_autoMemoHash[slot] == 0) {
            const unsigned index = g_autoMemoCount++;
            g_autoMemo[index].officeKey = officeKey;
            g_autoMemo[index].targetKey = targetKey;
            g_autoMemo[index].nextOffice = 0xFFFFFFFFu;
            g_autoMemo[index].state = 1;
            g_autoMemoHash[slot] = index + 1;
            return (int)index;
        }
        slot = (slot + 1) & (AUTO_MEMO_HASH_SIZE - 1);
    }
    return -1;
}

static bool AutoAppendNativeTarget(void* office, void* target, unsigned directions)
{
    if (!g_exeBase || g_exeSize <= AUTO_RVA_TASKCFG_CTOR + 1 ||
        g_exeSize <= AUTO_RVA_VECTOR_PUSH_BACK + 1) return false;
    void** oldBegin = nullptr; size_t oldCount = 0;
    if (!AutoReadTargetVector(office, &oldBegin, &oldCount)) return false;
    (void)oldBegin; (void)oldCount;

    typedef void* (__fastcall *TTaskCfgCtor)(void*);
    typedef void (__fastcall *TVectorPushBack)(void*, const void*);
    TTaskCfgCtor ctor = (TTaskCfgCtor)(g_exeBase + AUTO_RVA_TASKCFG_CTOR);
    TVectorPushBack pushBack = (TVectorPushBack)(g_exeBase + AUTO_RVA_VECTOR_PUSH_BACK);
    if (!ReadablePtr((const void*)ctor, 1) || !ReadablePtr((const void*)pushBack, 1)) return false;

    void* entry = ::operator new(AUTO_TARGET_ENTRY_SIZE, std::nothrow);
    void* cfgMem = ::operator new(AUTO_TASKCFG_SIZE, std::nothrow);
    if (!entry || !cfgMem) {
        if (entry) ::operator delete(entry);
        if (cfgMem) ::operator delete(cfgMem);
        return false;
    }
    memset(entry, 0, AUTO_TARGET_ENTRY_SIZE);
    memset(cfgMem, 0, AUTO_TASKCFG_SIZE);
    void* cfg = nullptr;
    try { cfg = ctor(cfgMem); } catch (...) { cfg = nullptr; }
    if (!cfg) { ::operator delete(entry); ::operator delete(cfgMem); return false; }

    // The native UI adds entries OFF, awaiting its activation checkbox. An
    // autonomous company must request activation itself, not only add the row.
    // Native dirty/upkeep/planner paths still validate route compatibility.
    AutoApplyEntryActivation(entry, directions);
    *(void**)((BYTE*)entry + 0x08) = target;
    *(void**)((BYTE*)entry + 0x10) = cfg;
    // Ghidra reports no resource-filter field in this 0x18-byte TaskEntry.
    // The planner's target/connection index is rebuilt in its working area
    // by reachability; do not invent or write a filter at entry+0x0C.
    AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    *((BYTE*)cfg + 0x28) = 1;                    // native active flag
    *((BYTE*)cfg + 0xF0) = 1;                    // native validated flag

    try {
        // FUN_14008E050(vector, &entry) is the native vector<TaskEntry*>::push_back.
        pushBack((BYTE*)office + AUTO_DO_TARGETS_BEGIN, &entry);
    } catch (...) {
        ::operator delete(entry); ::operator delete(cfgMem);
        return false;
    }
    return true;
}

static void AutoPersistenceEvent(TsmSaveEvent event, const char* dir, void*)
{
    if (event == TSM_SAVE_LOADING) PanelObserverLoading();
    // Persist permission keys only; never retain building pointers. Save/load
    // callbacks run on the file thread, so no game-memory access is performed.
    if (!g_persistence || !dir) return;
    if (event == TSM_SAVE_WRITING) {
        if (!g_persistence->writeAtomic) return;
        BYTE data[12 + AUTO_STATE_PERMISSION_MAX * 9];
        memcpy(data, "PLSTOR18", 8);
        uint32_t count = 0;
        EnterCriticalSection(&g_lock);
        count = g_statePermissionCount;
        memcpy(data + 8, &count, sizeof(count));
        size_t at = 12;
        for (unsigned i = 0; i < g_statePermissionCount; ++i) {
            memcpy(data + at, &g_statePermissions[i].key, sizeof(uint64_t));
            at += sizeof(uint64_t);
            data[at++] = g_statePermissions[i].mode;
        }
        LeaveCriticalSection(&g_lock);
        if (!g_persistence->writeAtomic(dir, AUTO_STATE_PREF_FILE, data, at))
            Logf("[AUTO DO] state permissions were not saved");
    } else if (event == TSM_SAVE_LOADING) {
        if (g_persistence->readFile) {
            size_t len = 0;
            void* raw = g_persistence->readFile(dir, AUTO_STATE_PREF_FILE, &len);
            AutoStatePermission loaded[AUTO_STATE_PERMISSION_MAX];
            unsigned loadedCount = 0;
            const bool legacy = raw && len >= 12 && memcmp(raw, "PLSTOR17", 8) == 0;
            if (raw && len >= 12 && (legacy || memcmp(raw, "PLSTOR18", 8) == 0)) {
                uint32_t count = 0;
                memcpy(&count, (BYTE*)raw + 8, sizeof(count));
                if (count <= AUTO_STATE_PERMISSION_MAX && len == 12 + (size_t)count * 9) {
                    size_t at = 12;
                    for (uint32_t i = 0; i < count; ++i) {
                        uint64_t key = 0;
                        memcpy(&key, (BYTE*)raw + at, sizeof(key));
                        at += sizeof(key);
                        const unsigned char mode = AutoMigratePermission(*((BYTE*)raw + at++), legacy);
                        if (key && mode != 0) {
                            loaded[loadedCount].key = key;
                            loaded[loadedCount].mode = mode;
                            ++loadedCount;
                        }
                    }
                } else {
                    Logf("[AUTO DO] state permission file has invalid length/count");
                }
            } else if (raw) {
                Logf("[AUTO DO] state permission file header invalid");
            }
            if (raw) free(raw);
            EnterCriticalSection(&g_lock);
            memset(g_statePermissions, 0, sizeof(g_statePermissions));
            if (loadedCount) memcpy(g_statePermissions, loaded,
                                    loadedCount * sizeof(loaded[0]));
            g_statePermissionCount = loadedCount;
            LeaveCriticalSection(&g_lock);
        }
        InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 1);
    }
}

static void AutoProbeStateInfrastructure(int closedDay, unsigned entityCount)
{
    g_autoStateStorageCount = 0;
    BYTE* game = g_exeBase ? TsmGameObject(g_exeBase) : nullptr;
    TsmBuildingSpan span;
    if (!game || !TsmGetBuildings(game, &span) ||
        !span.begin || !ReadablePtr(span.begin, span.count * sizeof(void*))) {
        Logf("[AUTO DO] day %d infra probe: world building vector unavailable", closedDay);
        return;
    }

    unsigned stateStorages = 0, privateStorages = 0, customsCount = 0;
    unsigned stateStoragesStored = 0, customsStored = 0;
    for (size_t i = 0; i < span.count; ++i) {
        void* b = span.begin[i];
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b)) continue;
        int type = -1;
        if (!AutoReadBuildingType(b, &type)) continue;
        // Discover storage-bearing sites for enforcement; classification, not
        // mere storage presence, determines allowed directions/visible widgets.
        if (AutoHasNativeStorage(b) && type != AUTO_CUSTOMS_TYPE &&
            type != AUTO_DO_ROAD_TYPE && type != AUTO_DO_RAIL_TYPE) {
            const bool isPrivate = g_enterprise && g_enterprise->isBuildingPrivate &&
                                   g_enterprise->isBuildingPrivate(b);
            if (isPrivate) ++privateStorages;
            else {
                ++stateStorages;
                if (stateStoragesStored < AUTO_STATE_STORAGE_MAX) {
                    AutoStateStorageSite& site = g_autoStateStorageSites[stateStoragesStored++];
                    site.building = b;
                    site.key = 0;
                    AutoBuildingKey(b, &site.key, nullptr);
                    site.permission = site.key ? (unsigned char)(AutoStatePermissionGet(site.key) & AutoBuildingCapabilities(b)) : 0;
                    site.x = 0.0f; site.z = 0.0f;
                    AutoBuildingPosition(b, &site.x, &site.z);
                    AutoBuildingName(b, site.name, sizeof(site.name));
                }
            }
            continue;
        }
        if (type != AUTO_CUSTOMS_TYPE) continue;
        ++customsCount;
        float x = 0.0f, z = 0.0f;
        if (!AutoBuildingPosition(b, &x, &z) || customsStored >= AUTO_CUSTOMS_MAX) continue;
        AutoCustomSite& site = g_autoCustomSites[customsStored++];
        site.building = b; site.x = x; site.z = z;
        AutoBuildingName(b, site.name, sizeof(site.name));
    }

    g_autoStateStorageCount = stateStoragesStored;
    Logf("[AUTO DO] day %d infra probe: world=%llu state_storage=%u private_storage=%u customs=%u customs_indexed=%u",
         closedDay, (unsigned long long)span.count, stateStorages, privateStorages,
         customsCount, customsStored);
    static int previousStateStorages = -1;
    static int previousCustoms = -1;
    if ((int)stateStorages != previousStateStorages || (int)customsCount != previousCustoms) {
        previousStateStorages = (int)stateStorages;
        previousCustoms = (int)customsCount;
        const unsigned show = (stateStoragesStored < 32) ? stateStoragesStored : 32;
        for (unsigned i = 0; i < show; ++i)
            Logf("[AUTO DO] state storage candidate name=%s key=%016llX pos=(%.1f,%.1f) permission=%s",
                 g_autoStateStorageSites[i].name,
                 (unsigned long long)g_autoStateStorageSites[i].key,
                 g_autoStateStorageSites[i].x, g_autoStateStorageSites[i].z,
                 g_autoStateStorageSites[i].permission == AUTO_ALLOW_BOTH ? "LOAD+UNLOAD" :
                 (g_autoStateStorageSites[i].permission == AUTO_ALLOW_LOAD ? "LOAD" :
                  (g_autoStateStorageSites[i].permission == AUTO_ALLOW_UNLOAD ? "UNLOAD" : "DENY")));
        if (stateStoragesStored > show)
            Logf("[AUTO DO] state storage list truncated: %u of %u shown",
                 show, stateStorages);
    }
    for (unsigned i = 0; i < entityCount; ++i) {
        AutoEntity& office = g_autoEntities[i];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        float ox = 0.0f, oz = 0.0f;
        if (!AutoBuildingPosition(office.building, &ox, &oz)) continue;
        int nearest = -1;
        double bestSq = 1.0e100;
        for (unsigned c = 0; c < customsStored; ++c) {
            const double dx = (double)g_autoCustomSites[c].x - ox;
            const double dz = (double)g_autoCustomSites[c].z - oz;
            const double d2 = dx * dx + dz * dz;
            if (d2 < bestSq) { bestSq = d2; nearest = (int)c; }
        }
        if (nearest >= 0)
            Logf("[AUTO DO] day %d nearest customs (geometric, not assigned) office=%016llX customs=%s dist=%.1fm",
                 closedDay, (unsigned long long)office.key,
                 g_autoCustomSites[nearest].name, sqrt(bestSq));
        else
            Logf("[AUTO DO] day %d nearest customs unavailable office=%016llX",
                 closedDay, (unsigned long long)office.key);
    }
}

static bool AutoTaskConfigValid(void* cfg)
{
    if (!cfg || !TsmIsValidGamePtr(cfg) || !ReadablePtr(cfg, AUTO_TASKCFG_SIZE)) return false;
    for (unsigned side = 0; side < 2; ++side) {
        const BYTE* p = (const BYTE*)cfg + (side ? 0xC8 : 0);
        const uintptr_t begin = *(const uintptr_t*)(p + 8);
        const uintptr_t end = *(const uintptr_t*)(p + 16);
        if ((!begin && end) || end < begin || (end - begin) % 16 != 0 ||
            (end - begin) / 16 > 4096) return false;
        if (end != begin && (!TsmIsValidGamePtr((void*)begin) ||
                            !ReadablePtr((void*)begin, end - begin))) return false;
    }
    return true;
}

static bool AutoSetTargetDirections(void* entry, unsigned directions)
{
    if (!entry || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) return false;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) {
        Logf("[AUTO DO] invalid task config; cannot enforce direction mask=%u", directions);
        return false;
    }
    const bool audit = g_autoLogLines < (unsigned)g_autoTargetLogLimit;
    AutoTaskAudit before = {};
    if (audit) before = AutoReadTaskAudit(entry, cfg);
    const bool directionsChanged = AutoApplyTaskDirections(cfg, directions & AUTO_ALLOW_BOTH, g_autoSourcePercent, g_autoDestinationPercent);
    const bool activationChanged = AutoApplyEntryActivation(entry, directions);
    const bool changed = directionsChanged || activationChanged;
    if (changed && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        Logf("[AUTO DO] task directions target=%p load=%u unload=%u entry_enabled=%u (native route validation still required)",
             *(void**)((BYTE*)entry + 0x08), (directions & AUTO_ALLOW_LOAD) ? 1u : 0u,
             (directions & AUTO_ALLOW_UNLOAD) ? 1u : 0u, (unsigned)*(BYTE*)entry);
        ++g_autoLogLines;
    }
    if (changed && audit && g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
        const AutoTaskAudit after = AutoReadTaskAudit(entry, cfg);
        Logf("[AUTO DO AUDIT] target=%p changed_fields=0x%02X enabled=%u->%u load_all=%u->%u unload_all=%u->%u load_pct=%.9g->%.9g unload_pct=%.9g->%.9g load_bits=%08X->%08X unload_bits=%08X->%08X",
             *(void**)((BYTE*)entry + 8), AutoTaskAuditChanges(before, after),
             (unsigned)before.enabled, (unsigned)after.enabled,
             (unsigned)before.loadAll, (unsigned)after.loadAll,
             (unsigned)before.unloadAll, (unsigned)after.unloadAll,
             AutoAuditPercent(before.loadBits), AutoAuditPercent(after.loadBits),
             AutoAuditPercent(before.unloadBits), AutoAuditPercent(after.unloadBits),
             (unsigned)before.loadBits, (unsigned)after.loadBits,
             (unsigned)before.unloadBits, (unsigned)after.unloadBits);
        ++g_autoLogLines;
    }
    return changed;
}

static void AutoLogNativeTarget(void* entry, int type, unsigned requested, bool preserve)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    void* cfg = *(void**)((BYTE*)entry + 0x10);
    if (!AutoTaskConfigValid(cfg)) return; // enforcement path reports invalid configs
    const BYTE* p = (const BYTE*)cfg;
    const uintptr_t lb = *(const uintptr_t*)(p + 8), le = *(const uintptr_t*)(p + 16);
    const uintptr_t ub = *(const uintptr_t*)(p + 0xD0), ue = *(const uintptr_t*)(p + 0xD8);
    Logf("[AUTO DO] native target=%p type=%d policy=%s requested_mask=%u enabled=%u load_all=%u load_pct=%.3f load_selectors=%llu unload_all=%u unload_pct=%.3f unload_selectors=%llu",
         *(void**)((BYTE*)entry + 8), type, preserve ? "preserve-customs" : "managed",
         requested, (unsigned)*(BYTE*)entry, (unsigned)p[0], (double)*(const float*)(p + 4),
         (unsigned long long)((le - lb) / 16), (unsigned)p[0xC8], (double)*(const float*)(p + 0xCC),
         (unsigned long long)((ue - ub) / 16));
    ++g_autoLogLines;
}


// Called only from daily reconciliation; does not arm, clear or advance timers.
static void AutoLogOfficePlanState(void* office, uint64_t key, int day,
                                   const char* phase, bool notificationRequested)
{
    if (g_autoLogLines >= (unsigned)g_autoTargetLogLimit) return;
    const BYTE* p = (const BYTE*)office;
    const bool dirtyReadable = ReadablePtr(p + AUTO_DO_DIRTY, 1);
    const bool timerReadable = ReadablePtr(p + AUTO_DO_UI_TIMER, sizeof(float));
    float timer = 0.0f;
    if (timerReadable) memcpy(&timer, p + AUTO_DO_UI_TIMER, sizeof(timer));
    Logf("[AUTO DO AUDIT] day=%d office=%p key=%016llX phase=%s notification_requested=%u dirty=%d timer_readable=%u countdown=%.9g",
         day, office, (unsigned long long)key, phase, notificationRequested ? 1u : 0u,
         dirtyReadable ? (int)p[AUTO_DO_DIRTY] : -1, timerReadable ? 1u : 0u, (double)timer);
    ++g_autoLogLines;
}

static void AutoTargetsOnDay(int day, int closedDay)
{
    if (!g_autoTargetsEnabled || !g_enterprise ||
        !g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex ||
        !g_enterprise->isBuildingPrivate) return;

    const bool afterLoad = (InterlockedExchange(&g_autoResetActiveMemoAfterLoad, 0) != 0) ||
                           (g_autoLastDay >= 0 && day < g_autoLastDay);
    if (afterLoad)
        Logf("[AUTO DO] day %d save/load detected: native TaskEntry/TaskCfg were deserialized; reconciling generated targets after load", closedDay);
    g_autoLastDay = day;

    const size_t n = g_enterprise->getEnterpriseCount();
    if (n > AUTO_ENTITY_MAX) {
        Logf("[AUTO DO] day %d skipped: registered count %llu exceeds fixed discovery capacity %u",
             closedDay, (unsigned long long)n, (unsigned)AUTO_ENTITY_MAX);
        return;
    }

    unsigned entityCount = 0, officeCount = 0, candidateCount = 0;
    for (size_t i = 0; i < n; ++i) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) ||
            !ReadablePtr((BYTE*)b + TSM_BLD_DEMOLISHED, 1) || TsmIsDemolished(b) ||
            !g_enterprise->isBuildingPrivate(b)) continue;
        uint64_t key = 0; int type = -1;
        if (!AutoBuildingKey(b, &key, &type)) continue;
        const bool isOffice = (type == AUTO_DO_ROAD_TYPE || type == AUTO_DO_RAIL_TYPE);
        const unsigned capabilities = isOffice ? 0 : AutoBuildingCapabilities(b);
        const bool hasStorage = isOffice || capabilities != 0;
        if (!hasStorage) continue;
        g_autoEntities[entityCount].building = b;
        g_autoEntities[entityCount].key = key;
        g_autoEntities[entityCount].type = type;
        g_autoEntities[entityCount].hasStorage = isOffice ? 0 : 1;
        g_autoEntities[entityCount].isStateStorage = 0;
        g_autoEntities[entityCount].statePermission = (unsigned char)capabilities;
        g_autoEntities[entityCount].targetPresent = 0;
        ++entityCount;
        if (isOffice) ++officeCount; else ++candidateCount;
    }

    AutoProbeStateInfrastructure(closedDay, entityCount);

    // Public access defaults deny. Keep denied sites indexed too: existing
    // native/manual entries must obey the same state permissions as new ones.
    for (unsigned si = 0; si < g_autoStateStorageCount && entityCount < AUTO_ENTITY_MAX; ++si) {
        AutoStateStorageSite& site = g_autoStateStorageSites[si];
        if (!site.building || !site.key) continue;
        AutoEntity& e = g_autoEntities[entityCount++];
        e.building = site.building; e.key = site.key; e.type = AUTO_STORAGE_BUILDING_TYPE;
        e.hasStorage = site.permission != 0;
        e.isStateStorage = 1; e.statePermission = site.permission; e.targetPresent = 0;
        if (site.permission != 0) ++candidateCount;
    }

    if (officeCount == 0) return;
    if (!AutoBuildPointerIndex(entityCount)) {
        Logf("[AUTO DO] day %d: pointer index capacity exhausted; scan aborted", closedDay);
        return;
    }
    if (!AutoBuildOfficeMemoIndex()) {
        Logf("[AUTO DO] day %d: office memo index capacity exhausted; scan aborted", closedDay);
        return;
    }

    unsigned planned = 0, added = 0, existing = 0, suppressed = 0, invalid = 0;
    g_autoLogLines = 0;
    for (unsigned oi = 0; oi < entityCount; ++oi) {
        AutoEntity& office = g_autoEntities[oi];
        if (office.type != AUTO_DO_ROAD_TYPE && office.type != AUTO_DO_RAIL_TYPE) continue;
        void** entries = nullptr; size_t entryCount = 0;
        if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
            Logf("[AUTO DO] day %d invalid target vector; skipped office key=%016llX",
                 closedDay, (unsigned long long)office.key);
            ++invalid; continue;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-reconcile", false);
        const size_t entryCountAtStart = entryCount;
        for (unsigned ei = 0; ei < entityCount; ++ei) g_autoEntities[ei].targetPresent = 0;
        bool doConfigChanged = false;
        for (size_t ei = 0; ei < entryCount; ++ei) {
            void* entry = entries[ei];
            if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
            void* targetBuilding = *(void**)((BYTE*)entry + 0x08);
            if (!targetBuilding || !TsmIsValidGamePtr(targetBuilding) ||
                !ReadablePtr(targetBuilding, TSM_BLD_MIN_READABLE)) continue;
            int targetType = -1;
            if (AutoReadBuildingType(targetBuilding, &targetType) && AutoPreserveManualCustoms(targetType)) {
                // Border trade is not a state-enterprise permission. Preserve
                // the user's native customs task, including its resource lists
                // and activation. Customs auto-selection is not implemented.
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] preserving manual customs target=%p entry_enabled=%u",
                         targetBuilding, (unsigned)*(BYTE*)entry);
                    ++g_autoLogLines;
                }
                AutoLogNativeTarget(entry, targetType, 0, true);
                continue;
            }
            const int targetIndex = AutoPointerIndex(targetBuilding);
            if (targetIndex >= 0) g_autoEntities[targetIndex].targetPresent = 1;
            // Enforce all targets of this PRIVATE office, including manually
            // present public entries, unindexed/unknown roles and default OFF.
            const unsigned capabilities = AutoBuildingCapabilities(targetBuilding);
            const bool isPrivate = g_enterprise->isBuildingPrivate(targetBuilding) != 0;
            unsigned permissions = 0;
            if (!isPrivate) {
                uint64_t key = 0;
                permissions = AutoBuildingKey(targetBuilding, &key, nullptr) ?
                    AutoStatePermissionGet(key) : 0;
            }
            const unsigned directions = AutoEffectiveDirections(isPrivate, permissions, capabilities);
            if (!g_autoTargetsDryRun)
                doConfigChanged |= AutoSetTargetDirections(entry, directions);
            AutoLogNativeTarget(entry, targetType, directions, false);
        }

        // Native upkeep can disable an entry in place: keep deduplication for
        // entries still present. Missing generated entries are eligible for
        // recreation regardless of why they disappeared. A private company
        // owns its target list; manual removal is not a persistent veto.
        // State-building permissions remain a separate eligibility gate.
        for (int mi = AutoOfficeMemoHeadIndex(office.key); mi >= 0;
             mi = g_autoMemo[mi].nextOffice == 0xFFFFFFFFu ? -1 : (int)g_autoMemo[mi].nextOffice) {
            AutoMemo& memo = g_autoMemo[mi];
            if (memo.state != 1) continue;
            bool found = false;
            for (size_t ei = 0; ei < entryCount; ++ei) {
                void* entry = entries[ei];
                if (!entry || !TsmIsValidGamePtr(entry) || !ReadablePtr(entry, AUTO_TARGET_ENTRY_SIZE)) continue;
                void* target = *(void**)((BYTE*)entry + 0x08);
                const int targetIndex = AutoPointerIndex(target);
                if (targetIndex >= 0 && g_autoEntities[targetIndex].key == memo.targetKey) {
                    found = true; break;
                }
            }
            if (!found) {
                memo.state = 3; // preserve hash chain; allow normal eligibility/re-add
                Logf("[AUTO DO] day %d target key=%016llX absent; eligible for automatic reconnection",
                     closedDay, (unsigned long long)memo.targetKey);
            }
        }

        unsigned doPlanned = 0, doAdded = 0, doExisting = 0, doSuppressed = 0;
        size_t projectedAdds = 0;
        // State storages were appended to g_autoEntities after private sites;
        // walk backward so an explicit state-source opt-in gets first access
        // to the five-entry conservative safe budget.
        for (unsigned ti = entityCount; ti > 0; --ti) {
            AutoEntity& target = g_autoEntities[ti - 1];
            if (!target.hasStorage || target.building == office.building) continue;
            if (target.targetPresent) {
                ++existing; ++doExisting; continue;
            }
            const int memoIndex = AutoMemoFind(office.key, target.key);
            if (entryCountAtStart + projectedAdds >= AUTO_DO_SAFE_TARGET_COUNT) {
                ++suppressed; ++doSuppressed;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d target=%016llX omitted: conservative target count %u reached (existing=%llu)",
                         closedDay, (unsigned long long)target.key,
                         (unsigned)AUTO_DO_SAFE_TARGET_COUNT,
                         (unsigned long long)entryCountAtStart);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++planned; ++doPlanned;
            if (g_autoTargetsDryRun) {
                ++projectedAdds;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d DRY RUN office=%016llX target=%016llX type=%d",
                         closedDay, (unsigned long long)office.key,
                         (unsigned long long)target.key, target.type);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (memoIndex < 0 && g_autoMemoCount >= AUTO_MEMO_MAX) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d memo capacity %u reached; target omitted",
                         closedDay, (unsigned)AUTO_MEMO_MAX);
                    ++g_autoLogLines;
                }
                continue;
            }
            if (!AutoAppendNativeTarget(office.building, target.building, target.statePermission)) {
                ++invalid;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d append failed office=%016llX target=%016llX",
                         closedDay, (unsigned long long)office.key, (unsigned long long)target.key);
                    ++g_autoLogLines;
                }
                continue;
            }
            ++projectedAdds;
            if (memoIndex < 0) {
                if (AutoMemoInsert(office.key, target.key) < 0) {
                    ++invalid;
                    Logf("[AUTO DO] day %d memo hash insertion failed after native append", closedDay);
                }
            } else {
                g_autoMemo[memoIndex].state = 1;
            }
            target.targetPresent = 1;
            ++added; ++doAdded;
            entries = nullptr; entryCount = 0;
            if (!AutoReadTargetVector(office.building, &entries, &entryCount)) {
                ++invalid; break;
            }
        }

        AutoLogOfficePlanState(office.building, office.key, closedDay, "before-notify", doAdded > 0 || doConfigChanged);
        if (doAdded > 0 || doConfigChanged) {
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_DIRTY, 1))
                *((BYTE*)office.building + AUTO_DO_DIRTY) = 1;
            if (ReadablePtr((BYTE*)office.building + AUTO_DO_UI_TIMER, sizeof(float)))
                *(float*)((BYTE*)office.building + AUTO_DO_UI_TIMER) = 3.0f;
        }
        AutoLogOfficePlanState(office.building, office.key, closedDay, "after-notify", doAdded > 0 || doConfigChanged);
        if (doPlanned || doExisting || doSuppressed || doAdded || doConfigChanged)
            Logf("[AUTO DO] day %d office=%016llX type=%d existing=%u planned=%u added=%u config_changed=%u suppressed=%u",
                 closedDay, (unsigned long long)office.key, office.type, doExisting,
                 doPlanned, doAdded, doConfigChanged ? 1u : 0u, doSuppressed);
    }

    Logf("[AUTO DO] day %d summary: private offices=%u candidates=%u planned=%u added=%u existing=%u suppressed=%u invalid=%u mode=%s",
         closedDay, officeCount, candidateCount, planned, added, existing, suppressed, invalid,
         g_autoTargetsDryRun ? "DRY RUN" : "WRITE");
}

// ---------------------------------------------------------------------------
// settings
// ---------------------------------------------------------------------------

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_logistics.ini";
    char v[256];

    g_enabled        = H->configInt(ini, "private_logistics", "enabled", 1);
    g_billing        = H->configInt(ini, "private_logistics", "billing", 0);
    g_panelObserverEnabled = H->configInt(ini, "private_logistics", "panel_fuel_observer", 1);
    g_dryRun         = H->configInt(ini, "private_logistics", "dry_run", 1);
    g_payFactory     = H->configInt(ini, "private_logistics", "pay_factory", 1);
    g_payCarrier     = H->configInt(ini, "private_logistics", "pay_carrier", 1);
    g_logCapPerDay   = H->configInt(ini, "private_logistics", "log_cap_per_day", 400);
    g_ledgerCapacity = H->configInt(ini, "private_logistics", "ledger_capacity", 512);
    g_siteLog        = H->configInt(ini, "private_logistics", "site_log", 1);
    g_autoTargetsEnabled = H->configInt(ini, "private_logistics", "auto_targets_enabled", 1);
    g_autoTargetsDryRun  = H->configInt(ini, "private_logistics", "auto_targets_dry_run", 0);
    g_autoTargetLogLimit = H->configInt(ini, "private_logistics", "auto_targets_log_limit", 80);
    g_autoSourcePercent = AutoClampSourcePercent(H->configInt(ini, "private_logistics", "auto_source_percent", 10));
    g_autoDestinationPercent = AutoClampDestinationPercent(H->configInt(ini, "private_logistics", "auto_destination_percent", 30));
    Logf("[AUTO DO] company thresholds: load above %d%% / deliver below %d%% (managed tasks, reconciled daily)",
         g_autoSourcePercent, g_autoDestinationPercent);
    if (g_autoSourcePercent != 10)
        Logf("[AUTO DO] source threshold override=%d%% (this test ships 10%%); an explicit deployed INI setting overrides the fallback", g_autoSourcePercent);
    Logf("[AUTO DO] permission-test profile: billing=%d auto_targets=%d apply=%d (expected 0/1/1)",
         g_billing, g_autoTargetsEnabled, !g_autoTargetsDryRun);
    if (g_billing || !g_autoTargetsEnabled || g_autoTargetsDryRun)
        Logf("[AUTO DO] WARNING deployed INI overrides the permission-test defaults; rebuild/deploy the shipped INI for this test");
    if (H->configInt(ini, "private_logistics", "state_storage_always_on", 0))
        Logf("[AUTO DO] state_storage_always_on is obsolete and ignored; per-building permissions are required");
    if (g_autoTargetLogLimit < 0) g_autoTargetLogLimit = 0;
    if (g_autoTargetLogLimit > 1000) g_autoTargetLogLimit = 1000;

    if (g_logCapPerDay < 0)            g_logCapPerDay = 0;
    if (g_logCapPerDay > 5000)         g_logCapPerDay = 5000;
    if (g_ledgerCapacity < 1)          g_ledgerCapacity = 1;
    if (g_ledgerCapacity > LEDGER_MAX) g_ledgerCapacity = LEDGER_MAX;

    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), ""))
        g_freightShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "factory_share", v, sizeof(v), ""))
        g_factoryShare = (float)atof(v);
    if (H->configString(ini, "private_logistics", "min_payment", v, sizeof(v), ""))
        g_minPayment = (float)atof(v);
    if (H->configString(ini, "private_logistics", "daily_pay_cap", v, sizeof(v), ""))
        g_dailyPayCap = atof(v);

    // v0.5: the tariff model. Every number here is a physical quantity or a
    // ratio - litres, tonnes per litre, a percentage. Not one of them is a
    // price in currency units, which is what makes the model survive a
    // currency event.
    g_tariffProbe     = H->configInt(ini, "private_logistics", "tariff_probe", 1);
    g_balanceLog      = H->configInt(ini, "private_logistics", "balance_log", 1);
    g_tariffLogPerDay = H->configInt(ini, "private_logistics", "tariff_log_per_day", 60);

    if (H->configString(ini, "private_logistics", "litres_per_trip", v, sizeof(v), ""))
        gLitresPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "litres_per_tonne", v, sizeof(v), ""))
        gLitresPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "tariff_margin", v, sizeof(v), ""))
        gTariffMargin = atof(v);
    if (H->configString(ini, "private_logistics", "diesel_density", v, sizeof(v), ""))
        gDieselDensity = atof(v);
    g_tripTariff = H->configInt(ini, "private_logistics", "trip_tariff", 1);
    g_censusLog  = H->configInt(ini, "private_logistics", "census_log", 1);
    g_vehScan    = H->configInt(ini, "private_logistics", "veh_scan", 1);
    g_vehScanLogPerDay = H->configInt(ini, "private_logistics", "veh_scan_log_per_day", 40);
    // v0.11: raw cargo-vector dump, up to three vehicles per close.
    g_cargoDump  = H->configInt(ini, "private_logistics", "cargo_dump", 1);
    if (g_vehScanLogPerDay < 0)   g_vehScanLogPerDay = 0;
    if (g_vehScanLogPerDay > 512) g_vehScanLogPerDay = 512;
    g_litresAuto = H->configInt(ini, "private_logistics", "litres_auto", 0);
    if (H->configString(ini, "private_logistics", "wear_litres", v, sizeof(v), ""))
        gWearLitres = atof(v);

    // v0.7: freight_share is RETIRED. Honouring it silently would make every
    // delivery look mispriced against a tariff that no longer reads it, so a
    // key left in the ini is reported and zeroed rather than obeyed.
    if (H->configString(ini, "private_logistics", "freight_share", v, sizeof(v), "") && v[0]) {
        const float f = (float)atof(v);
        g_freightShareRetired = (f > 0.0f) ? f : 0.0f;
        g_freightShare = 0.0f;
    }
    if (H->configString(ini, "private_logistics", "baskets_per_trip", v, sizeof(v), ""))
        gBasketsPerTrip = atof(v);
    if (H->configString(ini, "private_logistics", "baskets_per_tonne", v, sizeof(v), ""))
        gBasketsPerTonne = atof(v);
    if (H->configString(ini, "private_logistics", "fuel_resource", v, sizeof(v), "") && v[0])
        strncpy_s(gFuelResource, sizeof(gFuelResource), v, _TRUNCATE);

    // v0.4: a share is a fraction of cargo value. 0 is legal (freight off);
    // more than half would make haulage cost more than the goods.
    if (g_freightShare < 0.0f)   g_freightShare = 0.0f;
    if (g_freightShare > 0.50f)  g_freightShare = 0.50f;
    if (g_tripTariff != 0)       g_tripTariff   = 1;
    if (gWearLitres  < 0.0)      gWearLitres    = 0.0;
    if (gWearLitres  > 500.0)    gWearLitres    = 500.0;
    if (g_litresAuto != 1)       g_litresAuto   = 0;
    if (g_factoryShare  < 0.0f) g_factoryShare  = 0.0f;
    if (g_factoryShare  > 5.0f) g_factoryShare  = 5.0f;
    if (g_minPayment    < 0.0f) g_minPayment    = 0.0f;
    if (g_dailyPayCap   < 0.0)  g_dailyPayCap   = 0.0;

    // v0.5 clamps. Density is the one hard range: a value outside it is a
    // typo, and a typo here would misprice every delivery by orders of
    // magnitude, so it falls back to diesel rather than being honoured.
    if (g_tariffLogPerDay < 0)    g_tariffLogPerDay = 0;
    if (g_tariffLogPerDay > 5000) g_tariffLogPerDay = 5000;
    if (gLitresPerTrip  < 0.0)   gLitresPerTrip  = 0.0;
    if (gLitresPerTonne < 0.0)   gLitresPerTonne = 0.0;
    if (gLitresPerTrip  > 500.0) gLitresPerTrip  = 500.0;
    if (gLitresPerTonne > 50.0)  gLitresPerTonne = 50.0;
    if (gTariffMargin   < 0.0)   gTariffMargin   = 0.0;
    if (gTariffMargin   > 5.0)   gTariffMargin   = 5.0;
    if (!(gDieselDensity > 0.0001) || gDieselDensity > 0.01)
        gDieselDensity = 0.00084;
    if (gBasketsPerTrip  < 0.0)  gBasketsPerTrip  = 0.0;
    if (gBasketsPerTonne < 0.0)  gBasketsPerTonne = 0.0;
    if (gBasketsPerTrip  > 1000.0) gBasketsPerTrip = 1000.0;
    if (gBasketsPerTonne > 100.0)  gBasketsPerTonne = 100.0;

    // site_list, e.g. "0x1CB7FF" or "1CB7FF,68F792". Absent or empty = all.
    if (H->configString(ini, "private_logistics", "site_list", v, sizeof(v), "") && v[0]) {
        bool hit[MAX_SITES] = { false };
        int  matched = 0;

        for (char* p = v; *p; ) {
            while (*p == ' ' || *p == '\t' || *p == ',') p++;
            if (!*p) break;

            char* end = p;
            while (*end && *end != ',' && *end != ' ' && *end != '\t') end++;

            const char save = *end;
            *end = 0;
            const unsigned want = (unsigned)strtoul(p, nullptr, 16);
            *end = save;

            for (int i = 0; i < g_siteCount; i++) {
                if (g_sites[i].rva == want) { hit[i] = true; matched++; break; }
            }
            p = end;
        }

        if (matched > 0) {
            for (int i = 0; i < g_siteCount; i++) g_siteWanted[i] = hit[i];
            g_siteFilterActive = true;
        } else {
            Logf("private_logistics  WARN  site_list matched no known site - patching all");
        }
    }
}

// ---------------------------------------------------------------------------
// v0.8: the vehicle census
// ---------------------------------------------------------------------------
//
// Read-only, once a day, on the day callback. Every pointer is validated
// before it is followed, every vector length is capped, and nothing here can
// fail a delivery: a bad read reports "unreadable" and moves on.
//
// The numbers this produces are the ones the final model needs and no formula
// can supply:
//   - how many vehicles a company actually owns
//   - what each one carries at its peak (its capacity, observed not assumed)
//   - which catalogue entry the game attaches to it (+0x510, unproven)

static int VehSlot(void* v)
{
    for (int i = 0; i < g_vehCount; i++)
        if (g_vehPtr[i] == v) return i;
    if (g_vehCount >= VEH_MAX) return -1;
    g_vehPtr[g_vehCount]     = v;
    g_vehPeakTons[g_vehCount] = 0.0;
    g_vehSamples[g_vehCount]  = 0;
    return g_vehCount++;
}

// veh+0x3B8 is a vector of cargo entries, stride 0x10: { Resource* , float tons }.
// Returns the total carried and the number of non-empty slots.
static double CargoTons(void* veh, int* slots)
{
    *slots = 0;
    const BYTE* p = (const BYTE*)veh;
    if (!ReadablePtr(p + 0x3B8, 16)) return 0.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end <= beg) return 0.0;

    // v0.12: the cap was 8, and over 8 it returned 0 SILENTLY - which is how
    // this census came to report "0 slot(s)" on trucks that demonstrably held
    // fuel. A storage vector with a pre-allocated slot per resource type is
    // perfectly plausible, and it would be longer than eight. Raise the cap and
    // stop hiding the case.
    size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return 0.0;

    double t = 0.0;
    for (size_t i = 0; i < n; i++) {
        const float tons = *(const float*)(beg + i * 16 + 8);
        if (tons == tons && tons > 0.0f && tons < 1.0e6f) {
            t += (double)tons;
            (*slots)++;
        }
    }
    return t;
}

// ---------------------------------------------------------------------------
// v0.13: WHERE THE FUEL ACTUALLY IS
// ---------------------------------------------------------------------------
//
// The follow-up Ghidra report settled it, and it corrected two of my readings.
//
//   AUTHORITATIVE: the cargo vector at veh+0x3B8, stride 0x10, the slot whose
//                  +0x00 Resource* equals the diesel resource. +0x08 is the
//                  level in tonnes and the engine clamps it at zero.
//   MIRROR:        veh+0x5F0. Written by the refuel function (140142430) as
//                  min(available, capacity - current) + current, and by the
//                  drive function (14014B920) as (rate + current). NOT clamped,
//                  which is why it went NEGATIVE eight times in one run. That
//                  was never a mystery - it was a mirror with no floor.
//
// So the tank we read from now on is the slot, and +0x5F0 is kept only as a
// cross-check that gets printed beside it. If the two ever disagree by more
// than a few kilos, the log says so and we stop trusting the measurement.
static void* FuelResourcePtr(void)
{
    if (!g_pricing || !g_pricing->getByName) return nullptr;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    return g_fuelRes;
}

// Returns the fuel slot's tonnage. *found = 1 when the slot exists.
// -1 when the vector cannot be read at all.
static double FuelSlotTons(void* v, int* found)
{
    *found = 0;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x3B8, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + 0x3B8);
    const BYTE* end = *(const BYTE* const*)(p + 0x3C0);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) return -1.0;
    if (n == 0) return 0.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    void* fuel = FuelResourcePtr();
    if (!fuel) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* e = beg + i * 16;
        if (*(const void* const*)e != fuel) continue;
        float t = *(const float*)(e + 8);
        if (!(t == t) || t < -1000.0f || t > 1000.0f) return -1.0;
        if (t < 0.0f) t = 0.0f;
        *found = 1;
        return (double)t;
    }
    return 0.0;
}

// The mirror at +0x5F0, and the per-vehicle capacity the refuel code clamps to
// (*(veh+0x1708) + 0x8684). Both are evidence, not the measurement.
static double MirrorTons(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_FUEL_OFF, 4)) return -1.0;
    float f = *(const float*)(p + VEH_FUEL_OFF);
    if (!(f == f) || f < -1000.0f || f > 1000.0f) return -1.0;
    return (double)f;
}

static double TankCapacity(void* v)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + 0x1708, 8)) return -1.0;
    const BYTE* meta = *(const BYTE* const*)(p + 0x1708);
    if (!meta || !TsmIsValidGamePtr(meta)) return -1.0;
    if (!ReadablePtr(meta + 0x8684, 4)) return -1.0;
    const float c = *(const float*)(meta + 0x8684);
    if (!(c == c) || c <= 0.0f || c > 1000.0f) return -1.0;
    return (double)c;
}

// v0.11: sum of one building's fleet tanks, in tonnes. -1 = not readable.
static double FleetFuelTons(void* b, int* outCount)
{
    *outCount = 0;
    if (!b || !ReadablePtr((const BYTE*)b + 0xC70, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
    const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 8;
    if (n == 0 || n > VEH_MAX) return -1.0;
    if (!ReadablePtr(beg, n * 8)) return -1.0;

    double t = 0.0;
    int c = 0;
    for (size_t i = 0; i < n; i++) {
        void* v = *(void* const*)(beg + i * 8);
        if (!v) continue;
        int found = 0;
        double lvl = FuelSlotTons(v, &found);
        if (!found) lvl = MirrorTons(v);         // fall back to +0x5F0
        if (lvl < 0.0) continue;
        t += lvl;
        c++;
    }
    *outCount = c;
    return (c > 0) ? t : -1.0;
}

// One sample per committed delivery. Cheap: one vector walk and a few floats.
static void BurnProbe(void* b)
{
    int n = 0;
    const double now = FleetFuelTons(b, &n);
    if (now < 0.0) return;

    if (g_probeBld != b || g_probeFuelTons < 0.0 || n != g_probeVehCount) {
        g_probeBld      = b;                     // first sample, or a new fleet
        g_probeFuelTons = now;
        g_probeVehCount = n;
        return;
    }

    const double d = g_probeFuelTons - now;      // positive = burned
    if (d < -0.02 || d > 0.5) {
        g_probeSkipRun++;                        // refill, or the fleet changed size
    } else {
        g_probeBurnRun  += d;
        g_probeStepsRun++;
    }
    g_probeFuelTons = now;
}

// v0.12: RAW storage-vector dump. Two vectors, every slot, nothing filtered.
//
// Why this exists: the vehicle appears to keep its fuel in a
// {Resource*, float tons} slot, the same shape the transfer function moves
// goods with. If so, one of the vectors below holds diesel and the slot will
// track the +0x5F0 reading day after day.
//
// Two things went wrong in v0.10 and are fixed here:
//   - the census capped the vector at eight slots and returned "0 slot(s)"
//     for anything longer, which is why a truck holding 0.29 t of fuel could
//     report an empty hold. That was OUR bug, not evidence about the game.
//   - only the 0x3B8 vector was read. The trailer vector at 0x3A0 was never
//     looked at, and fuel may live there.
//
// A slot whose tonnage falls every day and jumps at a refill is the tank.
static void DumpVector(void* v, int off, const char* label, int closedDay, int slotIdx)
{
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + off, 16)) return;

    const BYTE* beg = *(const BYTE* const*)(p + off);
    const BYTE* end = *(const BYTE* const*)(p + off + 8);
    if (!beg || !end || end < beg) {
        Logf("[CARGO] day %d s=%d %s unreadable beg=%p end=%p",
             closedDay, slotIdx, label, beg, end);
        return;
    }

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64) {
        Logf("[CARGO] day %d s=%d %s n=%llu (implausible)",
             closedDay, slotIdx, label, (unsigned long long)n);
        return;
    }
    if (n == 0) {
        Logf("[CARGO] day %d s=%d %s n=0", closedDay, slotIdx, label);
        return;
    }
    if (!ReadablePtr(beg, n * 16)) return;

    Logf("[CARGO] day %d s=%d %s n=%llu", closedDay, slotIdx, label,
         (unsigned long long)n);
    const size_t show = (n < 16) ? n : 16;
    for (size_t i = 0; i < show; i++) {
        const BYTE* e   = beg + i * 16;
        const void* res = *(const void* const*)e;
        const float t   = *(const float*)(e + 8);
        if (!(t > 0.0f) && !(t < 0.0f)) continue;      // skip empty slots
        Logf("[CARGO] day %d s=%d %s   %llu res=%p %.4f t", closedDay, slotIdx,
             label, (unsigned long long)i, res, (double)t);
    }
}

static void CargoDump(int slot, void* v, int closedDay)
{
    if (!g_cargoDump || slot < 0 || slot > 2) return;   // first three vehicles
    DumpVector(v, 0x3A0, "trailer", closedDay, slot);
    DumpVector(v, 0x3B8, "cargo",   closedDay, slot);
}

// ---------------------------------------------------------------------------
// v0.9: the field scanner
// ---------------------------------------------------------------------------
//
// Compare the vehicle's first 0x800 bytes against yesterday's copy and report
// the words that moved. The baseline is taken silently on first sight so that
// day one does not dump a whole struct.
//
// Known pointer/vector ranges are skipped: they are noise here and the values
// would only ever read as "some address".
//
//   +0x3A0..0x3C8  trailer vector, cargo vector
//   +0x4F0..0x520  current building, home building, the +0x510 integer
//   +0x610..0x620  line stops
static bool VehScanSkip(int off)
{
    return (off >= 0x3A0 && off < 0x3C8) ||
           (off >= 0x4F0 && off < 0x520) ||
           (off >= 0x610 && off < 0x620);
}

// ---------------------------------------------------------------------------
// v0.10: read the tank
// ---------------------------------------------------------------------------
//
// veh+0x5F0 is fuel in tonnes (see the evidence at the globals). This turns it
// into the number the whole tariff has been missing: litres actually BURNED
// per delivery, with no purchases and no tank-fill error.
//
// A refill shows up as an increase and is counted, not charged as negative
// burn. A vehicle seen for the first time only sets a baseline.
static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
{
    if (slot < 0 || slot >= VEH_MAX) return;

    // v0.13: the SLOT is the tank. +0x5F0 is the unclamped mirror, kept as a
    // cross-check - it is the thing that went negative, and now we can show why.
    int found = 0;
    double level = FuelSlotTons(v, &found);
    const double mirror = MirrorTons(v);

    if (!found) {
        if (level < 0.0 && mirror < 0.0) return;
        level = (mirror > 0.0) ? mirror : 0.0;   // no fuel slot: fall back
        if (!g_vehNoSlotWarned[slot]) {
            g_vehNoSlotWarned[slot] = 1;
            Logf("[TANK] day %d   v=%p has NO fuel slot in the cargo vector "
                 "-> falling back to +0x5F0 (%.4f t)", closedDay, v, mirror);
        }
    }

    if (!g_vehFuelValid[slot]) {
        g_vehFuel[slot] = level;
        g_vehFuelValid[slot] = true;
        return;
    }

    const double prev = g_vehFuel[slot];
    if (level > prev + 0.02) {              // a refill, not burn
        if (refills) (*refills)++;
    } else if (prev > level) {
        if (burned) *burned += (prev - level);
    }
    g_vehFuel[slot] = level;

    // v0.13: one line per vehicle per day saying WHICH source was read and how
    // far the mirror has drifted from it. If these two ever part company by
    // more than a few kilos, the measurement is suspect and this says so.
    if (g_tankCrossLog > 0) {
        const double drift = (mirror >= 0.0) ? (mirror - level) : 0.0;
        Logf("[TANK] day %d   v=%p slot %.4f t | mirror +0x5F0 %.4f t | drift %+.4f t | %s",
             closedDay, v, level, mirror, drift,
             (fabs(drift) > 0.01) ? "MIRROR HAS DRIFTED" : "agree");
        g_tankCrossLog--;
    }
}

static void ScanVehicle(int slot, void* v, int closedDay)
{
    if (!g_vehScan || slot < 0 || slot >= VEH_MAX) return;
    if (!ReadablePtr((const BYTE*)v, VEH_SCAN_WORDS * 4)) return;

    const unsigned* src = (const unsigned*)v;

    if (!g_vehSnapValid[slot]) {
        for (int i = 0; i < VEH_SCAN_WORDS; i++) g_vehSnap[slot][i] = src[i];
        g_vehSnapValid[slot] = true;
        return;                                   // baseline, not a delta
    }

    int logged = 0;
    int changed = 0;
    for (int i = 0; i < VEH_SCAN_WORDS; i++) {
        const int off = i * 4;
        if (g_vehSnap[slot][i] == src[i]) continue;
        changed++;
        if (VehScanSkip(off)) { g_vehSnap[slot][i] = src[i]; continue; }
        if (logged < g_vehScanLogPerDay) {
            const unsigned a = g_vehSnap[slot][i];
            const unsigned b = src[i];
            float fa, fb;
            memcpy(&fa, &a, 4);
            memcpy(&fb, &b, 4);
            Logf("[VEH CHG] day %d v=%p +0x%03X %08X -> %08X (f %.4f -> %.4f)",
                 closedDay, v, off, a, b, (double)fa, (double)fb);
            logged++;
        }
        g_vehSnap[slot][i] = src[i];
    }
    if (changed > logged)
        Logf("[VEH CHG] day %d v=%p %d more word(s) changed, not logged",
             closedDay, v, changed - logged);
}

static void CensusFleets(int closedDay)
{
    if (!g_censusLog || !g_enterprise) return;
    if (!g_enterprise->getEnterpriseCount || !g_enterprise->getEnterpriseByIndex) return;

    size_t n = g_enterprise->getEnterpriseCount();
    if (n == 0) return;
    if (n > 32) n = 32;                          // a republic, not a spreadsheet

    // v0.11: one close counted once, not once per building. L per fleet-day is
    // the rate the physics has; it must not double-count a fleet seen from two
    // enterprises.
    const unsigned vehDaysBefore = g_tankVehDaysRun;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b || !TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr((const BYTE*)b + 0xC70, 16)) continue;

        const BYTE* beg = *(const BYTE* const*)((const BYTE*)b + 0xC70);
        const BYTE* end = *(const BYTE* const*)((const BYTE*)b + 0xC78);
        if (!beg || !end || end < beg) continue;

        size_t cnt = (size_t)(end - beg) / sizeof(void*);
        if (cnt > 64) cnt = 64;
        if (!ReadablePtr(beg, cnt * sizeof(void*))) continue;

        char bname[64];
        BuildingName(b, bname, sizeof(bname));

        if (cnt == 0) {
            Logf("[VEH] day %d %s fleet: 0 vehicle(s)", closedDay, bname);
            continue;
        }
        Logf("[VEH] day %d %s fleet: %u vehicle(s)", closedDay, bname, (unsigned)cnt);

        for (size_t k = 0; k < cnt; k++) {
            void* v = ((void* const*)beg)[k];
            if (!v || !TsmIsValidGamePtr(v)) continue;

            void* home = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x4F8, 8))
                home = *(void* const*)((const BYTE*)v + 0x4F8);

            void* cat = nullptr;
            if (ReadablePtr((const BYTE*)v + 0x510, 8))
                cat = *(void* const*)((const BYTE*)v + 0x510);

            // v0.12: the Ghidra report says the vehicle KIND is an int at
            // +0x50C, and that 0x42 marks a fuel consumer. +0x510, which this
            // census has been printing as a pointer, is the next field along -
            // which is why it kept printing nonsense like FFFFFFFF. If every
            // truck reads 66 here, that claim is confirmed; if none does, the
            // report has the wrong offset and we say so.
            int kind = -1;
            if (ReadablePtr((const BYTE*)v + 0x50C, 4))
                kind = *(const int*)((const BYTE*)v + 0x50C);

            int slots = 0;
            const double tons = CargoTons(v, &slots);
            CargoDump(VehSlot(v), v, closedDay);

            const int s = VehSlot(v);
            const bool first = (s >= 0 && g_vehSamples[s] == 0);
            if (s >= 0) {
                g_vehSamples[s]++;
                if (tons > g_vehPeakTons[s]) g_vehPeakTons[s] = tons;
            }

            // v0.13: the state gate the report found. 1 and 2 = working;
            // anything else and the engine does not even look at the truck.
            int state = -1, mode = -1;
            if (ReadablePtr((const BYTE*)v + 0x448, 4))
                state = *(const int*)((const BYTE*)v + 0x448);
            if (ReadablePtr((const BYTE*)v + 0x440, 4))
                mode  = *(const int*)((const BYTE*)v + 0x440);
            const bool active = (state == 1 || state == 2);
            if (active) g_activeVehDaysRun++; else g_idleVehDaysRun++;

            // Per-vehicle tank capacity, from the field the REFUEL code clamps
            // to. This is the "never assume an arbitrary vehicle" requirement
            // answered with a number the game itself uses.
            const double cap = TankCapacity(v);
            if (cap > 0.0 && s >= 0 && !g_vehCapValid[s]) {
                g_vehCapValid[s] = 1;
                g_vehCap[s] = cap;
                Logf("[VEH] day %d   v=%p tank capacity %.4f t (%.0f L at 1 kg/L)",
                     closedDay, v, cap, cap * 1000.0);
            }

            Logf("[VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) kind=%d cat=%p peak %.3ft "
                 "state=%d mode=%d %s",
                 closedDay, v, (home == b) ? "this" : "other",
                 tons, slots, kind, cat, (s >= 0) ? g_vehPeakTons[s] : 0.0,
                 state, mode, active ? "ACTIVE" : "idle");

            // One dump per vehicle, ever. +0x510 is the lead for capacity,
            // speed, price and consumption; this tells us what it actually is
            // before anything is built on it.
            // v0.10: fuel on board, and what left the tank since yesterday.
            double burned = 0.0;
            unsigned refills = 0;
            ReadTank(s, v, closedDay, &burned, &refills);
            g_burnTonsRun += burned;
            g_refillsRun  += refills;
            if (g_vehFuelValid[s]) g_tankVehDaysRun++;
            if (burned > 0.0 || refills > 0)
                Logf("[TANK] day %d   v=%p fuel %.4f t | burned %.4f t | refills %u",
                     closedDay, v, g_vehFuelValid[s] ? g_vehFuel[s] : 0.0,
                     burned, refills);

            ScanVehicle(s, v, closedDay);

            if (first && cat && TsmIsValidGamePtr(cat) && ReadablePtr(cat, 32)) {
                const unsigned* d = (const unsigned*)cat;
                Logf("[VEH] day %d   cat[+0x00] %08X %08X %08X %08X | %08X %08X %08X %08X",
                     closedDay, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
            }
        }
    }

    // v0.11: this close saw at least one tank -> it counts as a fleet-day.
    if (g_tankVehDaysRun > vehDaysBefore) g_tankDaysRun++;
}

// ---------------------------------------------------------------------------
// entry points
// ---------------------------------------------------------------------------

#include "panel_fuel_runtime.h"

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_logistics";
    info->version = PL_VERSION;

    ReadSettings();
    if (!g_enabled) {
        Logf("private_logistics  disabled in the ini");
        return 1;
    }

    g_realTransfer = (tTransfer)(g_exeBase + TSM_RVA_TRANSFER);

    Logf("private_logistics  v%s - billing %s",
         PL_VERSION,
         g_dryRun ? "ARMED IN DRY RUN (computes every payment, moves nothing)"
                  : "LIVE (moves money)");
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_exeBase || g_exeSize == 0) {
        Logf("private_logistics  no executable base - refusing to start");
        return 1;
    }

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK,
                                                   TSM_SIM_CLOCK_VERSION);
    if (!g_simClock)
        Logf("private_logistics  WARN  sim_clock is not installed - "
             "the daily summary will not be printed");

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE,
                                                           TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        Logf("private_logistics  WARN  enterprise_base is not installed - "
             "no building can be identified as private and nothing will be paid");

    if (g_autoTargetsEnabled) {
        g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                       TSM_UI_PANELS_VERSION);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V5);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V4);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V3);
        if (!g_uiPanels)
            g_uiPanels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS,
                                                           TSM_UI_PANELS_VERSION_V2);
        if (g_uiPanels && TsmUiPanelsHasWidgets(g_uiPanels) &&
            g_uiPanels->registerWidgetProvider)
            g_stateStorageWidgetHandle =
                g_uiPanels->registerWidgetProvider(StateStorageWidgetProvider, nullptr);
        if (!g_stateStorageWidgetHandle)
            Logf("private_logistics  [AUTO DO] WARN state-storage checkbox unavailable; "
                 "ui_panels v2 widgets are required");
    }

    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                   TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY,
                                                       TSM_CURRENCY_VERSION_V3);
    if (!g_currency)
        Logf("private_logistics  WARN  currency is not installed - "
             "the treasury leg of every payment is unavailable");

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                         TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING,
                                                             TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing)
        Logf("private_logistics  WARN  resource_pricing is not installed - "
             "factory payments fall back to the raw Res+0x58 field, which read "
             "0.00 for fabric and plants in two sessions");

    if (g_billing && (!g_enterprise || !g_currency)) {
        g_billing = 0;
        Logf("private_logistics  billing DISABLED: it needs both enterprise_base "
             "and currency. Instrumentation continues normally.");
    }

    if (g_autoTargetsEnabled &&
        (!g_simClock || !g_enterprise || !g_enterprise->getEnterpriseCount ||
         !g_enterprise->getEnterpriseByIndex || !g_enterprise->isBuildingPrivate)) {
        g_autoTargetsEnabled = 0;
        Logf("private_logistics  [AUTO DO] DISABLED: requires sim_clock and "
             "enterprise_base discovery/private checks");
    }

    if (g_autoTargetsEnabled) {
        g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE,
                                                              TSM_PERSISTENCE_VERSION);
        if (g_persistence &&
            g_persistence->structSize >= offsetof(TsmPersistenceApi, registerHandler) +
                                         sizeof(g_persistence->registerHandler) &&
            g_persistence->registerHandler)
            g_persistenceHandle = g_persistence->registerHandler("private_logistics",
                                                                  AutoPersistenceEvent, nullptr);
        if (!g_persistenceHandle)
            Logf("private_logistics  [AUTO DO] WARN persistence load signal unavailable; "
                 "reconciliation after load may wait for date rewind or restart");
    }

    PanelObserverStart();

    PatchAllSites();

    if (g_simClock) g_simClock->registerDay(OnDay, nullptr);

    Logf("private_logistics  live v%s: billing=%d dry_run=%d factory=%d carrier=%d, "
         "%d sites, cap %.0f/day, price = %s, factory share %.2f",
         PL_VERSION, g_billing, g_dryRun, g_payFactory, g_payCarrier,
         g_siteCount, (double)g_dailyPayCap,
         g_tripTariff ? "TRIP TARIFF (fuel x margin + wear)"
                      : "value share (v0.6 path, for comparison)",
         (double)g_factoryShare);

    Logf("private_logistics  [AUTO DO] enabled=%d dry_run=%d cadence=once/day; "
         "private endpoints -> private road/rail DOs; existing entries preserved",
         g_autoTargetsEnabled, g_autoTargetsDryRun);

    if (g_freightShareRetired > 0.0f)
        Logf("private_logistics  WARN freight_share = %.3f is RETIRED in v0.7 and was "
             "ignored - the trip tariff prices the delivery now. Remove the key.",
             (double)g_freightShareRetired);

    // v0.5: name both indices before a single delivery is priced. A probe that
    // silently cannot resolve a price is worse than no probe, so the log says
    // which index is live and what it reads.
    if (g_tariffProbe) {
        const double fp = FuelUnitPrice();
        float basket = 0.0f;
        if (g_pricing) basket = TsmPricingCpiBasket(g_pricing, 0.0f);

        Logf("private_logistics  v%s tariff: %.2f L/trip + %.2f L/t, wear %.2f L, "
             "margin %.0f%%, density %.5f t/L, index '%s', auto-tune %s",
             PL_VERSION, gLitresPerTrip, gLitresPerTonne, gWearLitres,
             gTariffMargin * 100.0, gDieselDensity, gFuelResource,
             g_litresAuto ? "on" : "off");

        // The world is not loaded yet at plugin start, so both of these read
        // zero here and are re-read at every day close. Say that rather than
        // printing a scary NOT FOUND that resolves itself a second later.
        Logf("private_logistics  [IDX] at plugin start: fuel '%s' = %.2f, cpi basket %.2f - "
             "the world is still loading; both are re-read at every day close",
             gFuelResource, fp, (double)basket);

        Logf("private_logistics  the tariff IS charged: every delivery is paid the "
             "trip price, indexed to the live fuel price. Cargo value is not a basis.");
    }

    if (g_balanceLog) {
        double treas = 0.0;
        bool   have  = false;
        if (g_currency && g_currency->getTreasuryBalance &&
            g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                      sizeof(g_currency->getTreasuryBalance)) {
            treas = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
            have  = true;
        }
        if (have) {
            g_treasuryAtClose   = treas;
            g_treasuryHaveClose = false;   // first close prints a baseline
            Logf("private_logistics  [BAL] treasury opens at %.2f", treas);
        }
    }

    if (g_billing && g_dryRun)
        Logf("private_logistics  DRY RUN: every [PAY] line says 'would'. "
             "Set dry_run = 0 in the ini and restart to move the money.");
    else if (g_billing)
        Logf("private_logistics  LIVE MONEY: the state treasury pays. Every [PAY] "
             "line reports the amount, the payee and the shortfall gate.");

    Logf("private_logistics  send back every [PAY] line and the daily [PAY] totals");
    return 0;
}
