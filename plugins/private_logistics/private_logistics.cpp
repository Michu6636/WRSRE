// private_logistics -- simplified observed B2B settlement.
//
// Goods: unloading buyer -> verified loading seller. Each physical cargo lot
// retains seller/resource/origin and global_base * global_market locked at
// loading; residual lots are never reassigned to another seller.
// DO trip fuel: original unloading buyer -> vehicle-owning private DO, using
// sampled positive fuel decrease and the global fuel price at observed close.
// Unknown starts, observation gaps, uncertain identities or missing prices are
// explicitly unpriced. The assignment observer does not establish exact native
// event coverage, fuel units, or exact burn. Dry-run defaults remain enabled.
// No cargo-value freight, wholesale/margin factor, fragment tariff, wear,
// upkeep, or other settlement component is applied.
//
#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/b2b_logistics_api.h"
#include "../../src/persistence_api.h"
#include "b2b_settlement.h"
#include "../../src/ui_panels_api.h"
#include "site_permissions.h"
#include "auto_target_policy.h"
#include "task_audit.h"
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"
#include "fuel_diagnostics.h"

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
static wrsre::b2b::Engine g_b2bEngine;
static int   g_billing         = 0; // Safe default: no money movement.
static int   g_dryRun          = 1;      // SAFE DEFAULT: compute, log, do not pay
static int   g_counterpartySettlement = 1;
static int   g_allowBuyerOverdraft = 1;
static double g_buyerOverdraftLimit = 5000.0;
static unsigned g_skipBuyerInsolvent = 0;
static unsigned g_b2bLoadsMode0      = 0;
static unsigned g_b2bLoadsMatched    = 0;
static unsigned g_b2bDelivMode0      = 0;
static unsigned g_b2bDelivMatched    = 0;
static unsigned g_b2bSettleCount     = 0;
static double   g_b2bPrincipalSum    = 0.0;
static double   g_b2bTripFuelSum      = 0.0;
static double   g_b2bStateNetSum     = 0.0;
static int   g_logCapPerDay    = 400;
static int   g_ledgerCapacity  = 512;
static int   g_siteLog         = 1;
// v0.7: the measured fuel bill. This is the only figure in the plugin that
// comes from the game instead of from arithmetic - the [TARIFF] model is an
// assumption, the DO's monthly foreign-cost total is the bill.
//
// Fuel is bought in bulk and stored, so a day's purchases are NOT a day's
// burn. Run averages also do not establish exact native journey burn.
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
// Legacy candidate sampling. Physical units, native deduction/refill writers
// and activity gates are NOT verified for the selected road truck. Variable
// names retain old units for compatibility; logs explicitly say raw/estimate.
static double   g_vehFuel[VEH_MAX];          // last valid raw candidate
static bool     g_vehFuelValid[VEH_MAX];
static double   g_burnTonsRun   = 0.0;       // sum of candidate sample decreases
static unsigned g_refillsRun    = 0;
// Historical denominators retained for candidate diagnostics, not physics.
static unsigned g_tankDaysRun    = 0;        // closes with at least one tank read
static unsigned g_tankVehDaysRun = 0;        // (vehicle, close) pairs
static int      g_cargoDump      = 1;        // [CARGO] - settle where fuel lives
static int      g_tankCrossLog   = 40;       // v0.13: [TANK] slot-vs-mirror lines
static private_logistics_fuel_diagnostics::Samples g_vehFuelSamples[VEH_MAX];
static volatile LONG g_fuelDiagnosticEpoch = 1;
static unsigned char g_vehCapValid[VEH_MAX];
static double        g_vehCap[VEH_MAX];      // metadata capacity candidate, raw

// Legacy state-word guesses only; never gate native events using these.
static unsigned g_activeVehDaysRun = 0;
static unsigned g_idleVehDaysRun   = 0;

// Legacy fleet endpoint sampling. It cannot prove individual fuel events.
static const int VEH_FUEL_OFF = 0x5F0;     // panel backing candidate; physical units unverified

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

static int g_balanceLog = 1;
static char gFuelResource[64] = "fuel";

#define PL_VERSION "0.19.0-simplified-settlement"
static int g_panelObserverEnabled = 1;
static int g_scheduleObserverEnabled = 1;
static void PanelObserverLoading(void);
static float g_minPayment      = 0.01f;

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

enum ShipKind { SHIP_LOAD = 0, SHIP_DELIVERY, SHIP_FUEL, SHIP_PURCHASE };

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

// Mode-0 building transfers and mode-1 vehicle transfers are joined through
// g_b2bEngine (PendingLoad: 0x694F40 -> 0x6953C4; PendingDelivery: 0x68F792 ->
// 0x68F7F9) using resource identity, tonnage within 0.5%, and dt <= 2.0 s.

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

static unsigned g_skipTreasury = 0;

static unsigned g_fuelEvents       = 0;

// v0.5: the money path, both ends. The treasury is sampled at each close; the
// carriers we paid are remembered so their capital can be printed beside it.
#define BAL_CARRIERS 4
static double g_treasuryAtClose    = 0.0;
static bool   g_treasuryHaveClose  = false;
static void*  g_balCarrier[BAL_CARRIERS]     = { nullptr, nullptr, nullptr, nullptr };
static double g_balCarrierPrev[BAL_CARRIERS] = { -1.0, -1.0, -1.0, -1.0 };
static double g_balCarrierIn[BAL_CARRIERS]   = { 0.0, 0.0, 0.0, 0.0 };
static int    g_balCarrierCount    = 0;
static void*  g_fuelRes            = nullptr;   // cached lookup, re-resolved daily

// v0.4: which day the accumulated tallies belong to. OnDay fires at the START
// of `day`, so the window it reports is the day that just ENDED.
static int g_summaryDay            = -1;

static double   g_wouldPayFuel     = 0.0;


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

static uint32_t VehSlotIdx(void* veh)
{
    if (!veh || !TsmIsValidGamePtr(veh) ||
        !ReadablePtr((const BYTE*)veh + 0x10, sizeof(uint32_t))) {
        return wrsre::b2b::Engine::hashVehiclePtr((std::uintptr_t)veh);
    }
    uint32_t idx = 0;
    __try {
        idx = *(const uint32_t*)((const BYTE*)veh + 0x10);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return wrsre::b2b::Engine::hashVehiclePtr((std::uintptr_t)veh);
    }
    if (idx < (uint32_t)wrsre::b2b::VEH_TRACK_MAX) return idx;
    return wrsre::b2b::Engine::hashVehiclePtr((std::uintptr_t)veh);
}
static bool AutoBuildingKey(void* b, uint64_t* outKey, int* outType);
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
        const double market = (double)TsmPricingMarketMult(g_pricing, res);
        const double quote = (double)p * market;
        if (std::isfinite(quote) && quote > 0.01 && quote < 1e9) return quote;
    }

    return 0.0; // no global market service: explicitly unpriced
}

// One guard, used everywhere the treasury is read: an older currency host may
// carry a shorter struct, and getTreasuryBalance is past the end of it.
static bool TreasuryReadable(void)
{
    return g_currency && g_currency->getTreasuryBalance &&
           g_currency->structSize >= offsetof(TsmCurrencyApi, getTreasuryBalance) +
                                     sizeof(g_currency->getTreasuryBalance);
}

static wrsre::b2b::FundsTransferCallbacks MakeTransferCallbacks(
    unsigned* insolvent, unsigned* treasurySkip)
{
    wrsre::b2b::FundsTransferCallbacks cb;
    cb.isLivePrivate = [](void*, void* b) -> bool {
        return IsPrivate(b) && !TsmIsDecayed(b);
    };
    cb.getCapital = [](void*, void* b) -> double {
        return g_enterprise ? g_enterprise->getCompanyCapital(b) : 0.0;
    };
    cb.modifyCapital = [](void*, void* b, double d, int cat) {
        if (g_enterprise) g_enterprise->modifyCompanyCapital(b, d, (TsmCapitalCategory)cat);
    };
    cb.getTreasury = [](void*, int) -> double {
        return TreasuryReadable()
            ? g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency)) : 0.0;
    };
    cb.moveTreasury = [](void*, double amount, const char* label) {
        if (g_currency) g_currency->moveTreasury(amount, label);
    };
    cb.checkSellerKey = [](void*, void* b, std::uint64_t key) -> bool {
        if (key == 0) return true;
        std::uint64_t current = 0; int type = 0;
        return AutoBuildingKey(b, &current, &type) && current == key;
    };
    cb.allowBuyerOverdraft = g_allowBuyerOverdraft != 0;
    cb.buyerOverdraftLimit = g_buyerOverdraftLimit;
    cb.minPayment = g_minPayment;
    cb.dryRun = g_dryRun != 0 || g_billing == 0;
    cb.skipBuyerInsolvent = insolvent;
    cb.skipTreasury = treasurySkip;
    return cb;
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

    // Pre-compute source building identity outside the lock on load legs so
    // VehCargoLot carries the exact same AutoBuildingKey verified at delivery.
    const bool loadLegMode0 = (committed && mode == 0 && tons < 0.0f &&
                               building && known != SITE_REFUEL);
    const bool loadSrcPriv  = loadLegMode0 ? IsPrivate(building) : false;
    std::uint64_t loadSellerKey = 0;
    if (loadSrcPriv) {
        int btype = 0;
        if (!AutoBuildingKey(building, &loadSellerKey, &btype))
            loadSellerKey = 0;
    }

    // p9 == vehicleHome is tautological on the verified vehicle-delivery
    // site, not a reason to discard its cargo or its only delivery join.
    // Other staging legs remain excluded, and refuelling is never cargo.
    const bool trackMode1Cargo = wrsre::b2b::ShouldTrackMode1Cargo(
        committed, mode, p2 != nullptr, isStaging,
        known == SITE_REFUEL, siteRva);
    const uint32_t mode1VehSlot = trackMode1Cargo ? VehSlotIdx(p2) : 0;

    EnterCriticalSection(&g_lock);

    // Record PendingLoad on mode-0 load leg (0x694F40).
    if (loadLegMode0) {
        g_b2bLoadsMode0++;
        g_b2bEngine.recordMode0Load((std::uintptr_t)building, (std::uintptr_t)res,
                                   fabsf(tons), NowSeconds(), loadSrcPriv, loadSellerKey, unitPrice);
    }
    // Match PendingLoad on mode-1 load leg (0x6953C4).
    if (committed && mode != 0 && tons > 0.0f && p2 && !isStaging && known != SITE_REFUEL) {
        if (g_b2bEngine.matchMode1Load((std::uintptr_t)p2, mode1VehSlot,
                                       (std::uintptr_t)res, tons, NowSeconds(), false, 0))
            ++g_b2bLoadsMatched;
    }

    // Record PendingDelivery on mode-0 delivery leg (0x68F792).
    if (committed && mode == 0 && tons > 0.0f && building && known != SITE_REFUEL) {
        g_b2bDelivMode0++;
        g_b2bEngine.recordMode0Delivery((std::uintptr_t)building,
                                        (std::uintptr_t)(owner ? owner : nullptr),
                                        (std::uintptr_t)res, tons, NowSeconds());
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

    if (emitAttrib) {
        char bname[64], rname[64];
        BuildingName(building, bname, sizeof(bname));
        ResourceName(res, rname, sizeof(rname));
        Logf("[B2B] #%u %-6s bld=%p \"%s\" priv=%d res=%s %.2ft @%.2f = %.2f "
             "(site 0x%06X, mode %u)",
             shipId, KindName(s.kind), building, bname, IsPrivate(building) ? 1 : 0,
             rname, (double)s.tons, s.unitPrice, s.value, siteRva, mode);
    }

    // ---- Ship 2 counterparty settlement (mode-1 delivery commit: 0x68F7F9) --
    // trackMode1Cargo admits that verified site even when p9 == vehicleHome;
    // the mode-0 building leg is only recorded, so it cannot settle twice.
    if (g_counterpartySettlement && trackMode1Cargo && tons < 0.0f) {
        wrsre::b2b::MatchedDelivery md;
        EnterCriticalSection(&g_lock);
        const bool mdMatched = g_b2bEngine.matchMode1Delivery(
            (std::uintptr_t)p2, mode1VehSlot,
            (std::uintptr_t)vehicleHome, (std::uintptr_t)res,
            fabsf(tons), NowSeconds(), &md);
        if (mdMatched) g_b2bDelivMatched++;
        LeaveCriticalSection(&g_lock);

        if (mdMatched && md.destBld != 0 && (g_billing || g_dryRun)) {
            const std::uintptr_t buyerBld = md.destBld;
            const bool buyerPriv = IsPrivate((void*)buyerBld);
            char buyerName[64], resourceName[64];
            BuildingName((void*)buyerBld, buyerName, sizeof(buyerName));
            ResourceName(res, resourceName, sizeof(resourceName));

            // Goods only. Each physical seller lot keeps its own quantity and
            // global market quote locked at loading. DO fuel is settled by the
            // observed assignment lifecycle, never by a transfer fragment.
            double dryBuyerAlreadySpent = 0.0;
            for (unsigned i = 0; md.cargoFound && i < md.partCount; ++i) {
                const auto& part = md.parts[i];
                const bool sellerPriv = part.origin == wrsre::b2b::ORIGIN_PRIVATE &&
                                        part.sellerBld != 0 && IsPrivate((void*)part.sellerBld);
                const double goodsValue = (double)part.tons * part.unitPrice;
                unsigned localInsolvent = 0, localTreasurySkip = 0;
                auto cb = MakeTransferCallbacks(&localInsolvent, &localTreasurySkip);
                auto plan = wrsre::b2b::planSettlement(
                    part.origin, part.sellerBld, sellerPriv, part.sellerKey,
                    buyerBld, buyerPriv, goodsValue);
                plan.allowBuyerOverdraft = g_allowBuyerOverdraft != 0;
                plan.buyerOverdraftLimit = g_buyerOverdraftLimit;
                const auto exec = wrsre::b2b::executeAtomicTransfer(plan, cb, dryBuyerAlreadySpent);
                if (cb.dryRun) dryBuyerAlreadySpent += exec.goodsPaid;

                bool emitSettleLog = false;
                EnterCriticalSection(&g_lock);
                g_skipBuyerInsolvent += localInsolvent;
                g_skipTreasury += localTreasurySkip;
                if (exec.goodsPaid > 0.0) {
                    ++g_b2bSettleCount;
                    g_b2bPrincipalSum += exec.goodsPaid;
                    g_b2bStateNetSum += exec.stateDelta;
                }
                if (g_payLoggedToday < (unsigned)g_logCapPerDay) {
                    ++g_payLoggedToday;
                    emitSettleLog = true;
                } else ++g_paySuppressed;
                LeaveCriticalSection(&g_lock);

                if (emitSettleLog) {
                    char sellerName[64];
                    BuildingName(part.sellerBld ? (void*)part.sellerBld : nullptr,
                                 sellerName, sizeof(sellerName));
                    Logf("[B2B GOODS] %s: Buyer \"%s\"%s -> Seller \"%s\"%s %.2f "
                         "(%s %.3ft @ %.2f locked_global_at_load) | State delta = %+.2f",
                         cb.dryRun ? "dry_run" : "live",
                         buyerName, buyerPriv ? " [P]" : " [S]",
                         part.sellerBld ? sellerName :
                            (part.origin == wrsre::b2b::ORIGIN_STATE ? "State" : "Unknown"),
                         sellerPriv ? " [P]" : "", exec.goodsPaid, resourceName,
                         (double)part.tons, part.unitPrice, exec.stateDelta);
                }
            }
        }
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
    (void)month;
    static SiteTally snap[MAX_SITES];
    unsigned calls,committed,noop,unknown,stag,nonPriv;
    unsigned siteSupp,attrSupp,paySupp,skTreasury,skBuyerInsolv;
    unsigned b2bLoad0,b2bLoadM,b2bDeliv0,b2bDelivM,b2bSettleN,fuelEvents;
    double b2bPrin,b2bFrt,b2bStateNet,fuelValue;
    static double cIn[BAL_CARRIERS];
    static void* cPt[BAL_CARRIERS];
    int cCount;

    EnterCriticalSection(&g_lock);
    memcpy(snap,g_tally,sizeof(snap));
    memset(g_tally,0,sizeof(g_tally));
    calls=g_calls;committed=g_callsCommitted;noop=g_callsNoop;
    unknown=g_unknownBuilding;stag=g_staging;nonPriv=g_nonPrivate;
    siteSupp=g_siteSuppressed;attrSupp=g_attrSuppressed;paySupp=g_paySuppressed;
    skTreasury=g_skipTreasury;skBuyerInsolv=g_skipBuyerInsolvent;
    b2bLoad0=g_b2bLoadsMode0;b2bLoadM=g_b2bLoadsMatched;
    b2bDeliv0=g_b2bDelivMode0;b2bDelivM=g_b2bDelivMatched;
    b2bSettleN=g_b2bSettleCount;b2bPrin=g_b2bPrincipalSum;
    b2bFrt=g_b2bTripFuelSum;b2bStateNet=g_b2bStateNetSum;
    fuelEvents=g_fuelEvents;fuelValue=g_wouldPayFuel;
    cCount=g_balCarrierCount;
    for(int i=0;i<BAL_CARRIERS;++i) {
        cPt[i]=g_balCarrier[i];cIn[i]=g_balCarrierIn[i];g_balCarrierIn[i]=0.0;
    }

    g_calls=g_callsCommitted=g_callsNoop=0;
    g_unknownBuilding=g_staging=g_nonPrivate=0;
    g_siteSuppressed=g_attrSuppressed=g_paySuppressed=0;
    g_skipTreasury=0;g_skipBuyerInsolvent=0;
    g_b2bLoadsMode0=g_b2bLoadsMatched=g_b2bDelivMode0=g_b2bDelivMatched=0;
    g_b2bSettleCount=0;g_b2bPrincipalSum=g_b2bTripFuelSum=g_b2bStateNetSum=0.0;
    g_fuelEvents=0;g_wouldPayFuel=0.0;g_fuelRes=nullptr;
    g_siteLoggedToday=g_attrLoggedToday=g_payLoggedToday=0;
    if(g_summaryDay>day) g_summaryDay=-1;
    const int closedDay=(g_summaryDay>=0)?g_summaryDay:(day-1);
    g_summaryDay=day;
    LeaveCriticalSection(&g_lock);

    Logf("[B2B] day %d closed: %u calls, %u committed, %u no-op | unknown %u, "
         "staging %u, non-private %u | suppressed site %u attrib %u pay %u",
         closedDay,calls,committed,noop,unknown,stag,nonPriv,siteSupp,attrSupp,paySupp);
    Logf("[B2B DAY] day %d joins: load %u/%u, delivery %u/%u | settlements=%u "
         "goods=%.2f observed_assignment_fuel=%.2f state_net=%+.2f "
         "buyer_insolvent=%u treasury_skips=%u (%s)",
         closedDay,b2bLoadM,b2bLoad0,b2bDelivM,b2bDeliv0,b2bSettleN,
         b2bPrin,b2bFrt,b2bStateNet,skBuyerInsolv,skTreasury,
         (g_dryRun||!g_billing)?"dry_run":"live");

    for(int i=0;i<g_siteCount;++i) {
        const SiteTally& t=snap[i];if(!t.calls) continue;
        Logf("[B2B SITE] day %d  0x%06X %-9s calls=%u commit=%u in=%.3f out=%.3f "
             "value=%.2f | arg9!=0 %u home!=0 %u",
             closedDay,g_sites[i].rva,ClassName(g_sites[i].known),t.calls,
             t.committed,t.tonsIn,t.tonsOut,t.value,t.sawBld,t.sawHome);
    }

    if(g_balanceLog) {
        const int cur=g_currency?TsmCurrencyActive(g_currency):0;
        double treas=0.0;bool have=false;
        if(TreasuryReadable()) {treas=g_currency->getTreasuryBalance(cur);have=true;}
        if(!have) Logf("[BAL] day %d treasury unavailable",closedDay);
        else if(!g_treasuryHaveClose)
            Logf("[BAL] day %d treasury %.2f baseline",closedDay,treas);
        else Logf("[B2B BAL] day %d treasury %.2f -> %.2f (%+.2f) | "
                  "goods_state_net=%+.2f | %s",
                  closedDay,g_treasuryAtClose,treas,treas-g_treasuryAtClose,b2bStateNet,
                  g_dryRun?"dry_run":"live");
        if(have) {g_treasuryAtClose=treas;g_treasuryHaveClose=true;}
        for(int i=0;i<cCount;++i) {
            void* b=cPt[i];if(!b||!g_enterprise||!g_enterprise->getCompanyCapital) continue;
            const double capital=g_enterprise->getCompanyCapital(b);
            char name[64];BuildingName(b,name,sizeof(name));
            Logf("[DO BAL] day %d %s capital %.2f (%+.2f) | trip_fuel_income=%.2f",
                 closedDay,name,capital,
                 g_balCarrierPrev[i]<0.0?0.0:capital-g_balCarrierPrev[i],cIn[i]);
            g_balCarrierPrev[i]=capital;
        }
    }

    CensusFleets(closedDay);
    Logf("[FUEL] day %d refuel sites: %u call(s), %.2f observed fuel value",
         closedDay,fuelEvents,fuelValue);
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
// Total-count policies live in auto_target_policy.h: road 20, rail still 5.
// The reviewed fixed control storage/receiver chain supports this road bound;
// it is not the native per-selection batch count or a 100-target engine policy.
// Existing/manual entries are retained even when already over the policy limit.
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
    (void)oldBegin;
    // Recheck the live vector before allocating/calling native helpers. The
    // daily snapshot is a planning budget, not permission to exceed the limit.
    int officeType = -1;
    if (!AutoReadBuildingType(office, &officeType) ||
        !AutoCanAppendTarget(officeType, oldCount)) return false;

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
    if (event == TSM_SAVE_LOADING) {
        InterlockedIncrement(&g_fuelDiagnosticEpoch);
        PanelObserverLoading();
        EnterCriticalSection(&g_lock);
        g_b2bEngine.resetAll();
        LeaveCriticalSection(&g_lock);
    }
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
        // to the office-type total-count budget (road 20, rail still 5).
        for (unsigned ti = entityCount; ti > 0; --ti) {
            AutoEntity& target = g_autoEntities[ti - 1];
            if (!target.hasStorage || target.building == office.building) continue;
            if (target.targetPresent) {
                ++existing; ++doExisting; continue;
            }
            const int memoIndex = AutoMemoFind(office.key, target.key);
            if (!AutoCanAppendTarget(office.type, entryCountAtStart, projectedAdds)) {
                ++suppressed; ++doSuppressed;
                if (g_autoLogLines < (unsigned)g_autoTargetLogLimit) {
                    Logf("[AUTO DO] day %d target=%016llX omitted: total target policy %u reached (existing=%llu)",
                         closedDay, (unsigned long long)target.key,
                         (unsigned)AutoTargetLimitForType(office.type),
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
    g_scheduleObserverEnabled = H->configInt(ini, "private_logistics", "schedule_observer", 1);
    g_dryRun         = H->configInt(ini, "private_logistics", "dry_run", 1);
    // [b2b] section
    g_counterpartySettlement = H->configInt(ini, "b2b", "counterparty_settlement", 1);
    g_allowBuyerOverdraft = H->configInt(ini, "b2b", "allow_buyer_overdraft", 1);
    if (H->configString(ini, "b2b", "buyer_overdraft_limit", v, sizeof(v), "") && v[0])
        g_buyerOverdraftLimit = atof(v);
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

    if (H->configString(ini, "private_logistics", "min_payment", v, sizeof(v), ""))
        g_minPayment = (float)atof(v);
    if (g_minPayment < 0.0f) g_minPayment = 0.0f;

    g_balanceLog = H->configInt(ini, "private_logistics", "balance_log", 1);
    g_censusLog = H->configInt(ini, "private_logistics", "census_log", 1);
    g_vehScan = H->configInt(ini, "private_logistics", "veh_scan", 1);
    g_vehScanLogPerDay = H->configInt(ini, "private_logistics", "veh_scan_log_per_day", 40);
    g_cargoDump = H->configInt(ini, "private_logistics", "cargo_dump", 1);
    if (g_vehScanLogPerDay < 0) g_vehScanLogPerDay = 0;
    if (g_vehScanLogPerDay > 512) g_vehScanLogPerDay = 512;
    if (H->configString(ini, "private_logistics", "fuel_resource", v, sizeof(v), "") && v[0])
        strncpy_s(gFuelResource, sizeof(gFuelResource), v, _TRUNCATE);

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
// Candidate resource-slot and display-field readers, NOT authoritative fuel.
// The old building/upkeep report does not prove this road truck's burn or
// refill writer, canonical store, native clamp, or physical mass conversion.
// ---------------------------------------------------------------------------
static void* FuelResourcePtr(void)
{
    if (!g_pricing || !g_pricing->getByName) return nullptr;
    if (!g_fuelRes) {
        g_fuelRes = g_pricing->getByName(gFuelResource);
        if (g_fuelRes && !TsmIsValidGamePtr(g_fuelRes)) g_fuelRes = nullptr;
    }
    return g_fuelRes;
}

// Returns raw quantity from a matching resource slot. Units/classification are
// unverified for this road truck. *found = 1 even if the quantity is invalid.
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
        // Preserve invalid/negative raw values: diagnostics must never invent
        // an empty tank by applying an unverified native clamp.
        *found = 1;
        return (double)t;
    }
    return 0.0;
}

// The panel backing quantity (+0x5F0) and metadata capacity (+0x8684).
// Display correlation alone does not establish native burn/refill semantics.
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
        const auto reading = private_logistics_fuel_diagnostics::Select(
            found != 0, lvl, MirrorTons(v));
        if (!reading.valid) continue;
        t += reading.value;
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
// Legacy daily candidate sampling, NOT a native deduction/refill adapter.
// Reading, provenance and baseline changes are explicit. A negative candidate
// is invalid, not zero; changing source, invalid reads and load reset baselines.
static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)
{
    if (slot < 0 || slot >= VEH_MAX) return;
    using namespace private_logistics_fuel_diagnostics;
    int found = 0;
    const double slotRaw = FuelSlotTons(v, &found);
    const double displayRaw = MirrorTons(v);
    const Reading reading = Select(found != 0, slotRaw, displayRaw);
    const Result result = g_vehFuelSamples[slot].Observe(reading,
        (unsigned long long)InterlockedCompareExchange(&g_fuelDiagnosticEpoch, 0, 0));
    g_vehFuelValid[slot] = reading.valid;
    if (reading.valid) g_vehFuel[slot] = reading.value;
    // Legacy accumulators are candidate sample changes ONLY. No tracker feed.
    if (burned) *burned += result.decrease;
    if (refills && result.change == Change::Increase) ++*refills;
    if (g_tankCrossLog > 0) {
        Logf("[TANK] day %d v=%p selected_source=%s source_valid=%d "
             "slot_found=%d slot_candidate_raw=%.9g display_5f0_raw=%.9g "
             "drift_valid=%d drift_raw=%+.9g change=%s decrease_raw=%.9g "
             "units=unverified exact_burn=0 verified_refill=0 billing_input=0",
             closedDay, v, SourceName(reading.source), reading.valid ? 1 : 0,
             found, slotRaw, displayRaw, reading.comparable ? 1 : 0,
             reading.drift, ChangeName(result.change), result.decrease);
        --g_tankCrossLog;
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
    unsigned omittedVisits = 0;

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

            // Legacy slots are process-lifetime: a new pointer may not fit.
            // Skip only this visit, never index a per-slot array with -1.
            const int s = VehSlot(v);
            if (s < 0 || s >= VEH_MAX) {
                ++omittedVisits;
                continue;
            }

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
            CargoDump(s, v, closedDay);

            const bool first = (s >= 0 && g_vehSamples[s] == 0);
            if (s >= 0) {
                g_vehSamples[s]++;
                if (tons > g_vehPeakTons[s]) g_vehPeakTons[s] = tons;
            }

            // Retained raw words and legacy guess; disproven as road-truck
            // activity evidence in the supplied game test. Never gate on them.
            int state = -1, mode = -1;
            if (ReadablePtr((const BYTE*)v + 0x448, 4))
                state = *(const int*)((const BYTE*)v + 0x448);
            if (ReadablePtr((const BYTE*)v + 0x440, 4))
                mode  = *(const int*)((const BYTE*)v + 0x440);
            const bool active = (state == 1 || state == 2);
            if (active) g_activeVehDaysRun++; else g_idleVehDaysRun++;

            // Metadata capacity candidate used by the panel, NOT proof of a
            // native refill clamp or physical tonnes.
            const double cap = TankCapacity(v);
            if (cap > 0.0 && s >= 0 && !g_vehCapValid[s]) {
                g_vehCapValid[s] = 1;
                g_vehCap[s] = cap;
                Logf("[VEH] day %d v=%p capacity8684_raw=%.9g display_scale1000_estimate=%.2f "
                     "physical_units=unverified refill_clamp=unverified",
                     closedDay, v, cap, cap * 1000.0);
            }

            Logf("[VEH] day %d   v=%p home=%s cargo %.3ft in %d slot(s) legacy_kind50c=%d catalogue510_candidate=%p peak %.3ft "
                 "legacy_state448=%d legacy_mode440=%d %s native_activity=unverified",
                 closedDay, v, (home == b) ? "this" : "other",
                 tons, slots, kind, cat, (s >= 0) ? g_vehPeakTons[s] : 0.0,
                 state, mode, active ? "legacy_active_guess" : "legacy_idle_guess");

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
                Logf("[TANK SAMPLE] day %d v=%p candidate_raw=%.4f | decrease_raw=%.4f "
                     "| sample_increases=%u units=unverified exact_burn=0 verified_refill=0",
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

    if (omittedVisits)
        Logf("[VEH COVERAGE] day %d legacy_census_capacity_exhausted=1 max_slots=%u "
             "omitted_visits=%u existing_slots_retained=1",
             closedDay, (unsigned)VEH_MAX, omittedVisits);

    // v0.11: this close saw at least one tank -> it counts as a fleet-day.
    if (g_tankVehDaysRun > vehDaysBefore) g_tankDaysRun++;
}

// ---------------------------------------------------------------------------
// entry points
// ---------------------------------------------------------------------------

#include "panel_fuel_runtime.h"
#include "fleet_fuel_runtime.h"
#include "dry_assignment_native.h"
#include "dry_assignment_runtime.h"

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

    Logf("private_logistics v%s - settlement %s", PL_VERSION,
         !g_billing ? "OBSERVE ONLY (moves no money)" :
         g_dryRun ? "ARMED IN DRY RUN (computes eligible transfers, moves nothing)"
                  : "LIVE (moves money)");
    static TsmB2bLogisticsApi g_b2bLogisticsApi = { sizeof(TsmB2bLogisticsApi), nullptr };
    g_b2bLogisticsApi.isCounterpartyBillingActive = []() -> int {
        return (g_enabled && g_billing && !g_dryRun && g_counterpartySettlement) ? 1 : 0;
    };
    H->provide(TSM_SERVICE_B2B_LOGISTICS, TSM_B2B_LOGISTICS_VERSION, &g_b2bLogisticsApi);

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
             "no private building identity is available and nothing will be paid");

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
        Logf("private_logistics WARN resource_pricing is not installed - "
             "global goods/fuel quotes are unavailable and settlement is unpriced");

    if (g_billing && (!g_enterprise || !g_currency || !g_pricing)) {
        g_billing = 0;
        Logf("private_logistics billing DISABLED: enterprise_base, currency and "
             "resource_pricing are required. Instrumentation continues normally.");
    }

    if (g_autoTargetsEnabled &&
        (!g_simClock || !g_enterprise || !g_enterprise->getEnterpriseCount ||
         !g_enterprise->getEnterpriseByIndex || !g_enterprise->isBuildingPrivate)) {
        g_autoTargetsEnabled = 0;
        Logf("private_logistics  [AUTO DO] DISABLED: requires sim_clock and "
             "enterprise_base discovery/private checks");
    }

    // Settlement and observed-assignment lifecycles need load epochs even when
    // automatic target assistance is disabled.
    g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE,
                                                          TSM_PERSISTENCE_VERSION);
    if (g_persistence &&
        g_persistence->structSize >= offsetof(TsmPersistenceApi, registerHandler) +
                                     sizeof(g_persistence->registerHandler) &&
        g_persistence->registerHandler)
        g_persistenceHandle = g_persistence->registerHandler("private_logistics",
                                                              AutoPersistenceEvent, nullptr);
    if (!g_persistenceHandle)
        Logf("private_logistics  WARN persistence load signal unavailable; "
             "settlement observers stay disabled and target reconciliation may wait");

    PanelObserverStart();
    FleetObserverStart();
    DryCostStart();

    PatchAllSites();

    if (g_simClock) g_simClock->registerDay(OnDay, nullptr);

    Logf("private_logistics live v%s: billing=%d dry_run=%d sites=%d | "
         "goods=locked_global_base_x_market, DO=observed_assignment_fuel_x_global_close | "
         "no wholesale, cargo-value freight, fragment tariff, margin, wear or upkeep",
         PL_VERSION,g_billing,g_dryRun,g_siteCount);
    Logf("private_logistics [AUTO DO] enabled=%d dry_run=%d cadence=once/day; "
         "private endpoints -> private road/rail DOs; existing entries preserved",
         g_autoTargetsEnabled,g_autoTargetsDryRun);
    Logf("private_logistics [IDX] fuel resource '%s'; global base and market are "
         "resolved at each eligible event, never substituted with a local building quote",
         gFuelResource);

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
        Logf("private_logistics DRY RUN: [B2B] and [DO TRIP] log planned transfers; "
             "no balances move. Set billing=1 and dry_run=0 to move money.");
    else if (g_billing)
        Logf("private_logistics LIVE MONEY: verified buyers pay sellers and private DOs. "
             "[B2B GOODS], [DO TRIP] and [B2B DAY] report attempted and paid amounts.");

    Logf("private_logistics native Test16 event coverage and exact burn remain unverified");
    return 0;
}
