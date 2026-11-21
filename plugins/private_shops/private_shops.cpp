// private_shops - private-sector restocking and pub service.
//
// Level 3. Consumes sim_clock, currency, persistence, resource_pricing,
// enterprise_base and taxes; provides nothing.
//
// v3.2 from v3.1:
//
//   - SLOT-IDENTITY SNAPSHOTS (money integrity). The per-building slot
//     bookkeeping was keyed by ForEachSlot's running enumeration index,
//     and nothing tied that index to the slot it described. The index is
//     not stable: ForEachSlot `continue`s past a storage that fails its
//     readable check or that holds an empty slot vector, the engine
//     reorders the storage list when a storage is built or demolished,
//     and a storage's slot vector grows when the building starts
//     stocking a new good. Every slot after the change shifts one index,
//     so `snap` described a DIFFERENT slot than the one being read, and
//     content - snap.content became a delta between two unrelated slots.
//     On an export storage a negative delta of that shape is paid out of
//     the treasury as state procurement - enterprise income with no sale,
//     no customer and no wallet movement at all.
//
//     SlotSnapshot now carries the slot pointer. A record whose identity
//     does not match the slot being visited is re-synced and never bills.
//     Both visitors write the identity, so the one shared record stays
//     valid across the daily foreign restock.
//
//   - The re-sync is logged (probe, first 20 per session) as
//     `[SLOT] ... index shifted`. A storage-layout change that used to
//     pay money now shows up as one log line instead.
//
//   - Nothing else moved. Restock targets, foreign billing, wholesale
//     billing, the service fees, the market book and every rate are
//     untouched.
//
// v3.1 from v3.0 (Macro Supply & Demand, Task 2):
//
//   - REPUBLIC SUPPLY TRACKING. WholesaleSlot (factory export dispatch)
//     and RestockSlot (cross-border foreign import) record sell-order
//     tonnage into resource_pricing's market book. The state procurement
//     payout is base price x live marketMult x 0.90, so an overproduced
//     good floods the book and earns less per batch (the anti-money-printer).
//   - FACTORY INPUT DEMAND. Stock drawn from an import/import-special/fuel
//     storage on a non-retail building (the factory feeding its production
//     line) records buy-order tonnage into the same book.
//   - The book and multiplier live in resource_pricing v2; this plugin
//     consumes it prefer-v2 with a v1 fallback. With only v1 present,
//     payouts use a neutral 1.0 (pre-v3 behaviour). No ABI change here -
//     markers are NUL-separated cookie records, never pointers.
//
// v3.0 from v2.9:
//
//   - UNIFIED GLOBAL CURRENCY (v4). Currency consume prefers the v4 iface
//     (v3 fallback). Foreign restock, wholesale delivery billing and state
//     procurement all price through TsmCurrencyActive(g_currency), so a
//     USD-mode save transacts the USD leg of each resource. service_fees
//     gains an optional 4th field (`type:kind:fee[:fee_usd]`); without it
//     USD mode divides the RUB fee by usd_divisor. Probe/startup logs
//     carry the active symbol.
//
// v2.9 from v2.8:
//
//   - PROCUREMENT CREDIT IS EXPORT-STORAGE ONLY. The v2.8 rule credited
//     the enterprise whenever ANY slot on a non-retail private building
//     lost stock - but a factory loses stock every time its production
//     tick burns input material: the clothing factory draws `fabric 0.03`
//     per worker-day out of its $STORAGE_IMPORT, a coal plant burns from
//     its $STORAGE_IMPORT_SPECIAL. Every such draw paid the enterprise
//     `procurement_price_mult` of retail from the treasury, on top of the
//     wholesale already charged for the state's delivery of those same
//     inputs - a guaranteed margin for burning inputs instead of selling
//     output. The ini documents the intent as "dispatched output"; the
//     code could not tell one storage from another. FUN_1400e40f0 can:
//     it stamps storage+0x84 when the token was $STORAGE_IMPORT* or
//     $STORAGE_FUEL, and +0x85 when it was $STORAGE_EXPORT*. ForEachSlot
//     now hands the storage block to its visitor, and WholesaleSlot
//     credits only export-flagged outflow - where finished goods wait
//     for pickup on every producing building type that ships one (72 of
//     81 $PRODUCTION types; the nine exceptions are power and heating
//     plants, whose output flows through the grid and was never
//     procurement). Import outflow - input consumption and fuel burn -
//     now just updates the snapshot, and electricity can no longer be
//     phoned in as revenue. Inflow billing is untouched.
//
//   - retail_types: RETAIL IS AN EXPLICIT SET NOW. IsRetailType was
//     `type == 3 || a service fee || (list mode && listed)`, so under the
//     shipped `restock_types = all` the list clause was dead and any
//     retail type without a service fee - a hotel (10), a Workshop shop
//     on a non-3 type, a pub whose fees were cleared - read as
//     non-retail. Its customer sales were credited once by
//     enterprise_base's OnRetail interceptor, then the same inventory
//     drop paid the shop AGAIN as state procurement from the treasury.
//     retail_types (default 3,4,10,23,24,25,26,28 - the service-
//     building set) is consulted first, unconditionally, and logged at
//     startup; type 3 stays hardcoded so an emptied list can never
//     un-retail the base shop. The export-storage gate above already
//     keeps base-game retail ($STORAGE_DEMAND_*, $STORAGE_SPECIAL) off
//     the procurement path; this closes it for Workshop retail holding
//     stock in an export storage too.
//
//   - PROCUREMENT IS LABELLED FOR THE LEDGER. The payout went through
//     modifyTreasury - the raw balance write - while every other
//     treasury operation (wholesale_b2b, vat, wages) uses
//     moveTreasury, which fires currency's flow observers.
//     state_ledger accumulates ONLY what those observers report, so
//     g_monthFlows/g_flowSum - CSV NetTreasury, HUD net, the Other
//     column - came up short by every month the state picked up
//     private output, and the observed-vs-flow cross-check had a
//     permanent hole. The payout now goes through
//     moveTreasury("procurement"); unknown categories land in the
//     CSV's Other column, so no format change and no state_ledger
//     edit. moveTreasury is RUB-only by contract, which is exactly
//     what the old call passed.
//
// v2.8 from v2.7:
//
//   - STATE PROCUREMENT DISPATCH REVENUE. WholesaleSlot now watches for
//     outgoing stock on NON-retail buildings (delta < -0.001 t after the
//     state's own tick picked output up) and credits TSM_CAP_PRODUCTION
//     at `procurement_price_mult` of live retail, paid by the treasury.
//     Retail/service (type 3, restock types, service-fee types) still
//     only gets billed on inflow - its outflow is a customer sale the
//     retail interceptor already credited. `restock_types = all` walks
//     every building; factories, mines and farms finally have revenue.
//
// v2.7 from v2.6:
//
//   - STORAGE CAPACITY IS NOW READ. The v2.6 probe answered the
//     question it was built to ask: slot+0x0C reads +0.000 on every
//     real shop and pub storage on this build, so RestockSlot's
//     `limit > 0.01f` branch never fired and the flat 8 t fallback
//     decided every restock target - 2.4 t at the default 0.30 ratio -
//     while the storage next to it declared 15.000 t. The ini has said
//     "a fraction of the storage's declared capacity" since v2.4; the
//     code did not do that. Now it does.
//
//     SlotVisitor gained a `perSlotCap` parameter. ForEachSlot reads
//     storage+0x8C once per storage and divides it by that storage's
//     slot count before handing it down. The divide is the whole
//     correctness argument: +0x8C is the capacity of the WHOLE
//     storage, shared by every slot in it, so a per-slot target of
//     `capacity * ratio` would commit N * ratio of one storage against
//     a ratio-sized shelf. RestockSlot writes slot content directly -
//     it does not ask the engine to accept a delivery - so an overfill
//     would simply happen. The observed 2-slot 15 t storage would have
//     survived it (2 x 4.5 <= 15); a 20-slot department store would
//     not.
//
//     The absolute `if (target > 8.0f) target = 8.0f` ceiling is gone
//     with it, replaced by a clamp against the slot's own share. This
//     is a real behaviour change for anything with a large storage:
//     bigger targets mean bigger imports and a bigger capital drain.
//     RunForeignRestock's partial-purchase path already handles a shop
//     that cannot afford the whole deficit, so the effect is slower
//     filling rather than debt. Turn foreign_restock_target down if
//     imports now look too easy.
//
//     WholesaleSlot takes the parameter and ignores it - the state's
//     delivery was already bounded by the engine.
//
//   - Nothing else moved. The demand-probe state machine, the storage
//     probe, the slot cap of 32, the VAT split shared with
//     enterprise_base v5.3 and the service-charge math are untouched.
//
// v2.6 from v2.5:
//
//   - STORAGE SEMANTICS PROBE (`probe_pub_storage`). On the first walk
//     of a restocked building, and for every finished type-24 building
//     at world load (state or private, so the two can be compared),
//     each storage is logged with its declared capacity (storage+0x8C)
//     and transport class (storage+0x90), and each slot with its
//     resource, content (slot+0x08) and limit/quality (slot+0x0C).
//     This is the evidence for whether pubs initialize slot limits or
//     leave them at 0.0/1.0 - RestockSlot's target math reads
//     slot+0x0C as a goods limit when it exceeds 0.01 and falls back
//     to a flat 8 t otherwise.
//
//   - DEMAND LIFETIME PROBE (`probe_pub_demand`). Every service-fee
//     building is sampled each tick for patrons still carrying the
//     configured demand kind versus patrons whose demand the vanilla
//     service tick already cleared. A per-patron, per-day state machine
//     counts with->without transitions ("cleared mid-day") and
//     without->with transitions ("reappeared"); one summary line per
//     building per in-game day is logged.
//
//   - MULTI-SLOT WALK. ForEachSlot's slot cap is 32, not 8. Large
//     department stores and warehouses exceed 8 slots per storage and
//     were silently truncated.
//
//   - SERVICE VAT ALIGNED WITH enterprise_base v5.3. RunServiceCharge
//     now splits the taken fee the way OnRetail splits a sale:
//     netToShop = taken / (1 + vatRate), vatAccrued = taken -
//     netToShop. The old `taken * vatRate` accrual treated the fee as
//     VAT-exclusive, so SettleVatAll debited too much from capital and
//     periodVatAccrued inflated past the VAT the citizen actually
//     paid. Gross `taken` still reaches capital and periodGrossSales;
//     settlement leaves exactly netToShop.
//
// v2.5 from v2.4:
//
//   - SERVICE FEES ARE CONFIGURED, NOT HARD-CODED. The old pub-only
//     service charge was gated on building type 24. A Workshop pub -
//     or a hospital, cinema, church, sports hall - declares its own
//     $TYPE_* and its number is whatever the mod author chose, so
//     pub_types = 24 silently excluded every non-base-game service
//     building. The new `service_fees` key is a list of
//     <type>:<demand-kind>:<fee> triples; any private, finished
//     building whose type appears there charges a citizen who is
//     inside and carrying the matching demand, once per in-game day.
//     The demand-kind filter is what separates a customer from a
//     worker: a pub customer has an alcohol demand, a cinema-goer a
//     culture demand, a patient a health demand.
//
//   - `restock_types = all` is now accepted. Any private, finished
//     building is walked; storages that exist get restocked and
//     billed, storages that do not exist are simply nothing to do.
//
//   - ONE-SHOT DIAGNOSTIC. On the first tick after a world loads,
//     every private, finished building is logged with its type,
//     object name and custom name. This is what tells the operator
//     which number a Workshop building actually landed on.
//
//   - Backward compatibility: if `service_fees` is absent, the old
//     `pub_types` list is expanded with `pub_drink_price` and treated
//     as kind 3 (alcohol) service fees. The old keys are otherwise
//     ignored.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/persistence_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/b2b_logistics_api.h"
#include "../../src/taxes_api.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdint.h>
#include "utility_filter.h"

// ---------------------------------------------------------------- services

static const TsmSimClockApi*        g_simClock    = nullptr;
static const TsmCurrencyApi*        g_currency    = nullptr;
static const TsmResourcePricingApi* g_pricing     = nullptr;
static const TsmEnterpriseBaseApi*  g_enterprise  = nullptr;
static const TsmB2bLogisticsApi*    g_b2bLogistics = nullptr;
static const TsmTaxesApi*           g_taxes       = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persHandle  = -1;

// ---------------------------------------------------------------- config

static int    g_enabled              = 1;
static int    g_probe                = 1;
static int    g_tickIntervalMs       = 300;

static int    g_probePubStorage      = 1;   // v2.6 storage-semantics probe
static int    g_probePubDemand       = 1;   // v2.6 demand-lifetime probe

#define MAX_TYPE_LIST 32
static int    g_restockTypes[MAX_TYPE_LIST];
static int    g_restockTypeCount;
static int    g_restockAll           = 0;   // `restock_types = all`

// v2.9: retail is an explicit set, not a side effect of the restock-list
// mode. Consulted unconditionally by IsRetailType.
static int    g_retailTypes[MAX_TYPE_LIST];
static int    g_retailTypeCount;

static float  g_wholesaleRatio       = 0.70f;
static int    g_foreignEnabled       = 0;  // Physical deliveries by default; opt in to foreign restock.
static float  g_foreignTargetRatio   = 0.30f;
// v3.4: the shelf empties to this fraction of its capacity before an order is
// placed, and is then filled back to foreign_restock_target. Two numbers, so
// the ORDER SIZE is chosen instead of being whatever gap the tick found.
static float  g_restockReorderPoint  = 0.05f;
static float  g_foreignMarkup        = 1.05f;

// Service fees: a building type charges a given per-visit fee to a
// citizen inside who carries the given demand kind. See the ini for
// what each kind means in practice.
#define MAX_SERVICE_FEES 16
struct ServiceFee {
    int   type;      // building type number
    int   kind;      // demand kind; -1 means "any demand"
    float fee;       // RUB-denominated base (shipped ini unit)
    float feeUsd;    // explicit USD override; < 0 when absent
};
static ServiceFee g_serviceFees[MAX_SERVICE_FEES];
static int        g_serviceFeeCount = 0;

// v4: USD scale divisor for fees without an explicit override.
static float g_usdDivisor = 50.0f;

static float EffectiveFee(const ServiceFee& fee)
{
    int cur = TsmCurrencyActive(g_currency);
    if (cur != TSM_CURRENCY_USD) return fee.fee;
    if (fee.feeUsd > 0.0f) return fee.feeUsd;
    float d = (g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f;
    return fee.fee / d;
}

// ---------------------------------------------------------------- per-shop state

struct SlotSnapshot {
    void* resource;
    void* slot;          // v3.2: the slot this record describes. The
                         // enumeration index is not stable (see the v3.2
                         // header note); this pointer is what makes the
                         // record trustworthy.
    float content;
};

struct ShopState {
    std::vector<SlotSnapshot> slots;
    bool   snapshotDone     = false;
    int    lastForeignDay   = -1;
    double monthlyForeign   = 0.0;
    double monthlyWholesale = 0.0;
};

static std::unordered_map<void*, ShopState> g_shops;
static std::unordered_map<uint64_t, int>    g_serviceLastVisit;

// v2.6 probes. g_storageProbed makes the storage-semantics dump one-shot
// per building per world load; g_demandProbe accumulates the per-patron
// demand-kind samples for service-fee buildings.
static std::unordered_set<void*> g_storageProbed;

struct DemandProbeState {
    long long withSamples    = 0;   // observations of a patron holding the kind
    long long withoutSamples = 0;   // observations of a patron without it
    long long cleared        = 0;   // with->without transitions within one day
    long long reappeared     = 0;   // without->with transitions within one day
    int       lastLogDay     = -1;
    // bit0 = seen with the demand today, bit1 = seen without it today.
    std::unordered_map<uint64_t, unsigned> dayFlags;
};
static std::unordered_map<void*, DemandProbeState> g_demandProbe;

// One-shot diagnostic flag. Reset on every world load, cleared after
// the log block runs. See LogPrivateBuildings.
static bool g_loggedPrivateBuildings = false;

// ---------------------------------------------------------------- types

static bool IsInList(const int* list, int n, int type)
{
    for (int i = 0; i < n; i++) if (list[i] == type) return true;
    return false;
}

static const ServiceFee* FeeForType(int type)
{
    for (int i = 0; i < g_serviceFeeCount; i++)
        if (g_serviceFees[i].type == type) return &g_serviceFees[i];
    return nullptr;
}

// v2.8: procurement dispatch applies to every private, finished building
// EXCEPT retail/service - a shop or pub losing stock is a customer sale
// (already credited by enterprise_base's retail interceptor), not a
// state procurement dispatch from a factory/mine/farm. `restock_types =
// all` walks every building for restock, so it cannot mark everything
// retail: retail is type 3, the retail_types set, any service-fee type,
// or - when a restock list is configured - any listed restock type.
// v2.9: retail_types is consulted first and unconditionally, because
// under `all` the list clause below is dead and a non-3 retail building
// without a service fee read as non-retail - its customer sales were
// credited by the retail interceptor AND paid again as procurement.
static bool IsRetailType(int type)
{
    if (type == 3) return true;
    if (IsInList(g_retailTypes, g_retailTypeCount, type)) return true;
    if (FeeForType(type) != nullptr) return true;
    if (!g_restockAll &&
        IsInList(g_restockTypes, g_restockTypeCount, type)) return true;
    return false;
}

// ---------------------------------------------------------------- storage iteration

typedef void (*SlotVisitor)(void* storage, void* slotBytes, void* resource,
                            float content, float perSlotCap, size_t globalIndex,
                            void* userData);

static bool ForEachSlot(void* building, SlotVisitor cb, void* userData)
{
    BYTE* b = (BYTE*)building;
    void** storBegin = *(void***)(b + TSM_BLD_STORAGES_BEGIN);
    void** storEnd   = *(void***)(b + TSM_BLD_STORAGES_END);
    if (!storBegin || !storEnd || storEnd <= storBegin) return false;
    if (!TsmIsValidGamePtr(storBegin)) return false;
    if (!ReadablePtr(storBegin, sizeof(void*))) return false;

    size_t storBytes = (size_t)((BYTE*)storEnd - (BYTE*)storBegin);
    if (storBytes % TSM_STORAGE_STRIDE) return false;
    size_t storCount = storBytes / TSM_STORAGE_STRIDE;
    if (storCount > 16) storCount = 16;

    if (!ReadablePtr(storBegin, storCount * TSM_STORAGE_STRIDE)) return false;

    size_t globalIndex = 0;

    for (size_t si = 0; si < storCount; si++) {
        BYTE* storage = (BYTE*)storBegin + si * TSM_STORAGE_STRIDE;
        if (!ReadablePtr(storage, 0xA0)) continue;

        void** slotBegin = *(void***)(storage + TSM_ST_SLOTS_BEGIN);
        void** slotEnd   = *(void***)(storage + TSM_ST_SLOTS_END);
        if (!slotBegin || !slotEnd || slotEnd <= slotBegin) continue;
        if (!TsmIsValidGamePtr(slotBegin)) continue;

        size_t slotBytes = (size_t)((BYTE*)slotEnd - (BYTE*)slotBegin);
        if (slotBytes % TSM_SLOT_STRIDE) continue;
        size_t slotCount = slotBytes / TSM_SLOT_STRIDE;
        if (slotCount > 32) slotCount = 32;   // v2.6: department stores exceed 8

        if (!ReadablePtr(slotBegin, slotCount * TSM_SLOT_STRIDE)) continue;

        // v2.7: storage+0x8C is the capacity of the WHOLE storage, not
        // of one slot. A visitor that wants "30% of capacity" has to be
        // told each slot's share of it, or N slots each target 30% of
        // the whole and the storage ends up at N*30%. Computed once per
        // storage, here, rather than re-derived in every visitor.
        //
        // The cap is deliberately read after the slot vector is known
        // good, and is only a hint: a storage that declares 0 leaves
        // the visitor on its own fallback.
        float storCap = *(float*)(storage + TSM_ST_CAPACITY);
        if (!(storCap > 0.0f)) storCap = 0.0f;
        float perSlot = (slotCount > 0) ? storCap / (float)slotCount : storCap;

        for (size_t sj = 0; sj < slotCount; sj++) {
            BYTE* slot = (BYTE*)slotBegin + sj * TSM_SLOT_STRIDE;
            void* res  = *(void**)(slot + TSM_SLOT_RESOURCE);
            char rname[64];
            if (TsmIsValidGamePtr(res) &&
                TsmResourceName(res, rname, sizeof(rname)) &&
                wrsre_shops::IsUtilityResourceName(rname))
            {
                continue;
            }
            float cnt  = *(float*)(slot + TSM_SLOT_CONTENT);
            cb(storage, slot, res, cnt, perSlot, globalIndex, userData);
            globalIndex++;
        }
    }
    return true;
}

// ---------------------------------------------------------------- storage semantics probe (v2.6)
//
// Dumps what the engine actually initialized on a building's storages:
// declared capacity and transport class per storage block, then
// resource, content and limit/quality per slot. RestockSlot decides
// its restock target from slot+0x0C when that value reads as a goods
// limit; a pub whose slots keep 0.0 or 1.0 there is telling us the
// field is not a goods limit on this building type. One dump per
// building per world load - this reports raw slot counts uncapped, so
// what is logged is what is really there.

static void ProbeStorageSemantics(void* building, int type)
{
    if (!g_probePubStorage) return;
    if (g_storageProbed.count(building)) return;
    g_storageProbed.insert(building);

    BYTE* b = (BYTE*)building;
    void** storBegin = *(void***)(b + TSM_BLD_STORAGES_BEGIN);
    void** storEnd   = *(void***)(b + TSM_BLD_STORAGES_END);

    wchar_t name[128] = {0};
    g_enterprise->getBuildingCustomName(building, name, 128);

    if (!storBegin || !storEnd || storEnd <= storBegin) {
        Logf("private_shops  storage probe: \"%ls\" (type %d) has no storage vector",
             name[0] ? name : L"(unnamed)", type);
        return;
    }
    if (!TsmIsValidGamePtr(storBegin)) return;
    if (!ReadablePtr(storBegin, sizeof(void*))) return;

    size_t storBytes = (size_t)((BYTE*)storEnd - (BYTE*)storBegin);
    if (storBytes % TSM_STORAGE_STRIDE) return;
    size_t storCount = storBytes / TSM_STORAGE_STRIDE;

    // The 64 bound is crash safety only; the probe reports the truth.
    if (storCount > 64) storCount = 64;
    if (!ReadablePtr(storBegin, storCount * TSM_STORAGE_STRIDE)) return;

    Logf("private_shops  storage probe: \"%ls\" (type %d) - %zu storage block(s)",
         name[0] ? name : L"(unnamed)", type, storCount);

    for (size_t si = 0; si < storCount; si++) {
        BYTE* storage = (BYTE*)storBegin + si * TSM_STORAGE_STRIDE;
        if (!ReadablePtr(storage, 0xA0)) continue;

        float capacity = *(float*)(storage + TSM_ST_CAPACITY);
        int   cls      = *(int*)(storage + TSM_ST_CLASS);

        // v3.3: +0x84/+0x85 are the ONLY thing that decides whether a
        // drain is billed as consumption or paid as dispatch. Not printing
        // them cost a session: the probe showed the contents and capacity
        // while the question was always the flag.
        BYTE impByte = *(const BYTE*)(storage + TSM_ST_IMPORT);
        BYTE expByte = *(const BYTE*)(storage + TSM_ST_EXPORT);
        Logf("private_shops    storage #%zu: declared capacity %.3f  "
             "transport class %d  import=%d export=%d",
             si, capacity, cls, (int)impByte, (int)expByte);

        void** slotBegin = *(void***)(storage + TSM_ST_SLOTS_BEGIN);
        void** slotEnd   = *(void***)(storage + TSM_ST_SLOTS_END);
        if (!slotBegin || !slotEnd || slotEnd <= slotBegin) {
            Logf("private_shops      (no slots)");
            continue;
        }
        if (!TsmIsValidGamePtr(slotBegin)) continue;

        size_t slotBytes = (size_t)((BYTE*)slotEnd - (BYTE*)slotBegin);
        if (slotBytes % TSM_SLOT_STRIDE) continue;
        size_t slotCount = slotBytes / TSM_SLOT_STRIDE;
        if (slotCount > 64) slotCount = 64;
        if (!ReadablePtr(slotBegin, slotCount * TSM_SLOT_STRIDE)) continue;

        for (size_t sj = 0; sj < slotCount; sj++) {
            BYTE* slot = (BYTE*)slotBegin + sj * TSM_SLOT_STRIDE;
            void* res     = *(void**)(slot + TSM_SLOT_RESOURCE);
            float content = *(float*)(slot + TSM_SLOT_CONTENT);
            float limit   = *(float*)(slot + 0x0C);

            char rname[64];
            if (!(TsmIsValidGamePtr(res) && TsmResourceName(res, rname, sizeof(rname))))
                _snprintf_s(rname, sizeof(rname), _TRUNCATE, "raw %p", res);

            Logf("private_shops      slot #%zu: res \"%s\"  content %.3f  limit/quality %+.3f",
                 sj, rname, content, limit);
        }
    }
}

static void GuardedStorageProbe(void* b, int type)
{
    __try { ProbeStorageSemantics(b, type); }
    __except (FaultFilter("private_shops storage probe", GetExceptionInformation())) {}
}

// ---------------------------------------------------------------- foreign restocking

struct RestockContext {
    void*      shop;
    ShopState* state;
    double     capital;
    int        bought;
    double     spent;
};

static void RestockSlot(void* /*storage*/, void* slotBytes, void* res,
                        float content, float perSlotCap, size_t globalIndex,
                        void* ud)
{
    RestockContext* ctx = (RestockContext*)ud;
    if (!ctx || !ctx->state) return;

    if (globalIndex >= ctx->state->slots.size()) {
        ctx->state->slots.resize(globalIndex + 1,
                                 SlotSnapshot{ nullptr, nullptr, 0.0f });
    }

    if (!TsmIsValidGamePtr(res)) return;

    // v2.7: three-tier target.
    //
    //   slot+0x0C reads +0.000 on every shop and pub storage the v2.6
    //   probe dumped, so the first branch is dead on this build and
    //   kept only for a modded storage that does initialize it.
    //
    //   The second branch is the one that now runs. perSlotCap is the
    //   storage's declared capacity divided by its slot count (see
    //   ForEachSlot), so summing the targets over one storage gives
    //   capacity * ratio - never more than the shelf holds.
    //
    //   The flat 8 t of the third branch used to decide every restock,
    //   and an absolute `target > 8.0f` clip used to bound the other
    //   branches too. Both are gone: they made a 15 t shop and a 200 t
    //   warehouse restock to the same 2.4 t per slot, which is not what
    //   foreign_restock_target says it does. The clamp below is against
    //   the slot's own share instead.
    float limit = *(float*)((BYTE*)slotBytes + TSM_SLOT_QUALITY);

    // v3.4: TWO NUMBERS, NOT ONE.
    //
    // The shelf used to be topped up whenever it fell below the target, so the
    // order size was whatever the gap happened to be when the tick ran - a
    // trickle nobody chose. Inventory is two decisions and real shops keep
    // them apart:
    //
    //   reorder_point - how low the shelf may fall before an order is placed
    //   target        - what it is filled to when it is
    //
    // The order is then (target - reorder_point): fixed, predictable, and big
    // enough to be worth a truck. A shelf cycles between the two levels
    // instead of hovering at one.
    //
    // reorder_point = 0 is legal - it is what "wait until the shop is empty"
    // means - but the shop really is empty until the delivery arrives.
    float base;
    if (limit > 0.01f)            base = limit;
    else if (perSlotCap > 0.01f)  base = perSlotCap;
    else                          base = 8.0f;

    float target  = base * g_foreignTargetRatio;
    float reorder = base * g_restockReorderPoint;

    if (perSlotCap > 0.01f && target > perSlotCap) target = perSlotCap;
    if (!(reorder < target)) reorder = target * 0.5f;   // misconfigured, keep sane

    if (content > reorder) return;

    float deficit = target - content;
    // v4: unified global currency - restock at the active price so the
    // capital math stays in the treasury's unit.
    float perTon  = g_pricing->getPrice(res, TsmCurrencyActive(g_currency));
    if (!(perTon > 0.0f)) return;

    double cost = (double)deficit * (double)perTon * (double)g_foreignMarkup;
    if (cost <= 0.0) return;

    if (ctx->capital < cost) {
        if (ctx->capital <= 1.0) return;
        double affordableTons = ctx->capital / ((double)perTon * (double)g_foreignMarkup);
        double partialCost = affordableTons * (double)perTon * (double)g_foreignMarkup;
        *(float*)((BYTE*)slotBytes + TSM_SLOT_CONTENT) = content + (float)affordableTons;
        ctx->spent += partialCost;
        ctx->capital = 0.0;
        // v3.0 (Task 2): a partial cross-border purchase is a Republic
        // SELL ORDER for exactly the tonnage that cleared customs.
        TsmPricingRecordSupply(g_pricing, res, affordableTons);
    } else {
        *(float*)((BYTE*)slotBytes + TSM_SLOT_CONTENT) = content + deficit;
        ctx->spent += cost;
        ctx->capital -= cost;
        // v3.0 (Task 2): a full cross-border purchase is a Republic SELL
        // ORDER for the entire deficit that cleared customs.
        TsmPricingRecordSupply(g_pricing, res, (double)deficit);
    }
    ctx->bought++;

    ctx->state->slots[globalIndex].resource = res;
    ctx->state->slots[globalIndex].slot     = slotBytes;
    ctx->state->slots[globalIndex].content  = *(float*)((BYTE*)slotBytes + TSM_SLOT_CONTENT);
}

static void RunForeignRestock(void* building, ShopState& st, int day)
{
    if (!g_foreignEnabled) return;
    if (st.lastForeignDay == day) return;
    st.lastForeignDay = day;

    double capital = g_enterprise->getCompanyCapital(building);

    RestockContext ctx = { building, &st, capital, 0, 0.0 };
    ForEachSlot(building, RestockSlot, &ctx);

    if (ctx.spent <= 0.0) return;

    g_enterprise->modifyCompanyCapital(building, -ctx.spent, TSM_CAP_FOREIGN);
    st.monthlyForeign += ctx.spent;

    if (g_probe) {
        wchar_t name[128] = {0};
        if (g_enterprise->getBuildingCustomName)
            g_enterprise->getBuildingCustomName(building, name, 128);
        if (!name[0] && g_enterprise->getBuildingObjectName) {
            const char* obj = g_enterprise->getBuildingObjectName(building);
            if (obj && obj[0]) {
                size_t i = 0;
                for (; i + 1 < _countof(name) && obj[i]; ++i)
                    name[i] = (wchar_t)(unsigned char)obj[i];
                name[i] = L'\0';
            }
        }
        Logf("private_shops  \"%ls\" imported %d slot(s) for %.2f %s",
             name[0] ? name : L"(unnamed)", ctx.bought, ctx.spent,
             TsmCurrencySymbol(g_currency));
    }
}

// ---------------------------------------------------------------- wholesale billing

// v2.8: state procurement dispatch revenue. When a non-retail building's
// EXPORT storage loses stock (delta < -0.001 t after the state's own tick
// picked it up), the state procures that output at `procurement_price_mult`
// of retail and the enterprise is credited TSM_CAP_PRODUCTION. Treasury
// pays through moveTreasury("procurement") so state_ledger's flow
// observers see it. v2.9: export storage ONLY - outflow from an import
// or fuel storage is the building consuming its own inputs and is not
// credited.

static float  g_procurementMult        = 0.90f;  // v2.8: fraction of retail

// v3.3: a storage that carries NO direction byte at +0x84/+0x85 cannot be
// classified, and until now that meant a factory's output left unpaid. With
// this on, an UNMARKED storage that is also the LAST block on a non-retail
// building is treated as dispatch when it drains. 0 restores v3.2 exactly.
static int    g_procureFallback        = 1;

struct WholesaleContext {
    void*      shop;
    ShopState* state;
    double     capital;
    bool       isRetail;      // v2.8: retail still only gets billed
    int        procured;      // v2.8: slots that dispatched output
    double     revenue;       // v2.8: procurement credited this walk
    int        billed;
    double     charged;
    int        storCount;            // non-utility storage blocks on building
    int        lastFreightStorIndex; // raw index of last non-utility block (-1 if none)
    int        fallback;             // v3.3: credits that needed the flag fallback
};

// perSlotCap is unused here: wholesale bills the delta the state
// actually delivered, and the engine already bounded that delivery by
// the storage's own capacity. The parameter exists to match
// SlotVisitor.
// v3.3: the flag fallback may only guess "this is output" on a building
// that actually MAKES something. A warehouse, a depot and a distribution
// centre are full of goods that are passing through, and paying the
// treasury out for those would be a leak, not a repair. A production
// building also declares at least two storages - inputs and output - which
// is a second, independent reason to refuse a one-storage building.
static int DispatchFallbackAllowed(int type, int storCount)
{
    if (storCount < 2) return 0;
    switch (type) {
    case 5:  case 14: case 15: case 20:      // storage, depots, customhouse
    case 43: case 44:                        // distribution / container
        return 0;
    default:
        return 1;
    }
}

// When private_logistics Ship 2 counterparty billing is live, truck transfers
// settle directly between Buyer, Seller, and Carrier at 0x68F7F9. Keep slot
// snapshots and Republic macro supply/demand recording active without
// running State-clearinghouse procurement or wholesale money transfers.
static void WholesaleMarketOnlySlot(void* storage, void* slotBytes, void* res,
                                    float content, float /*perSlotCap*/,
                                    size_t globalIndex, void* ud)
{
    WholesaleContext* ctx = (WholesaleContext*)ud;
    if (!ctx || !ctx->state) return;
    if (!TsmIsValidGamePtr(res)) return;

    if (globalIndex >= ctx->state->slots.size()) {
        ctx->state->slots.resize(globalIndex + 1,
                                 SlotSnapshot{ nullptr, nullptr, 0.0f });
    }
    if (!ctx->state->snapshotDone) {
        ctx->state->slots[globalIndex].resource = res;
        ctx->state->slots[globalIndex].slot     = slotBytes;
        ctx->state->slots[globalIndex].content  = content;
        return;
    }

    SlotSnapshot& snap = ctx->state->slots[globalIndex];
    if (snap.slot != slotBytes) {
        snap.slot     = slotBytes;
        snap.resource = res;
        snap.content  = content;
        return;
    }

    bool isExport = storage &&
        *(const BYTE*)((const BYTE*)storage + TSM_ST_EXPORT) != 0;
    bool isImport = storage &&
        *(const BYTE*)((const BYTE*)storage + TSM_ST_IMPORT) != 0;

    int storIndex = -1;
    {
        void** sb = *(void***)((BYTE*)ctx->shop + TSM_BLD_STORAGES_BEGIN);
        void** se = *(void***)((BYTE*)ctx->shop + TSM_BLD_STORAGES_END);
        if (sb && se && (BYTE*)se > (BYTE*)sb && storage &&
            (const BYTE*)storage >= (const BYTE*)sb &&
            (const BYTE*)storage <  (const BYTE*)se) {
            storIndex = (int)(((const BYTE*)storage - (const BYTE*)sb) /
                              TSM_STORAGE_STRIDE);
        }
    }

    bool flagless = storage && !isExport && !isImport;
    if (g_procureFallback && !ctx->isRetail && flagless &&
        storIndex >= 0 && storIndex == ctx->lastFreightStorIndex &&
        DispatchFallbackAllowed(TsmBuildingType(ctx->shop), ctx->storCount)) {
        isExport = true;
    }

    if (!ctx->isRetail && isExport && ctx->state->snapshotDone) {
        double delta = (double)content - (double)snap.content;
        if (delta < -0.001 && snap.resource && content >= 0.0f) {
            TsmPricingRecordSupply(g_pricing, snap.resource, -(double)delta);
        }
        snap.resource = res;
        snap.content  = content;
        return;
    }

    if (snap.resource != res) {
        snap.resource = res;
        snap.content  = content;
        return;
    }

    float delta = content - snap.content;
    if (delta < -0.001f && !isExport && isImport && snap.resource) {
        TsmPricingRecordDemand(g_pricing, snap.resource, (double)(-delta));
    }
    snap.content = content;
}

static void WholesaleSlot(void* storage, void* slotBytes, void* res,
                          float content, float /*perSlotCap*/, size_t globalIndex,
                          void* ud)
{
    WholesaleContext* ctx = (WholesaleContext*)ud;
    if (!ctx || !ctx->state) return;
    if (!TsmIsValidGamePtr(res)) return;

    if (globalIndex >= ctx->state->slots.size()) {
        ctx->state->slots.resize(globalIndex + 1,
                                 SlotSnapshot{ nullptr, nullptr, 0.0f });
    }

    if (!ctx->state->snapshotDone) {
        ctx->state->slots[globalIndex].resource = res;
        ctx->state->slots[globalIndex].slot     = slotBytes;
        ctx->state->slots[globalIndex].content  = content;
        return;
    }

    SlotSnapshot& snap = ctx->state->slots[globalIndex];

    // v3.2: identify the slot BEFORE any delta is computed.
    //
    // `globalIndex` is a running counter over storages x slots. It is not
    // a stable name for a slot: ForEachSlot `continue`s past a storage
    // that is unreadable or has an empty slot vector, the engine reorders
    // the storage vector when a storage is built or demolished, and a
    // storage's slot vector grows when the building stocks a new good.
    // When any of those happens, this index now describes a different
    // slot than the one `snap` was written from.
    //
    // The delta below is then (content of slot A) - (content of slot B).
    // A full slot compared against an empty one reads as a large outflow,
    // and on an export storage outflow is paid out of the treasury as
    // state procurement - income with no sale, no customer, no wallet
    // debit and no shelf movement.
    //
    // A record that does not describe this slot is re-synced. A re-sync
    // never bills: there is no trustworthy delta to bill.
    if (snap.slot != slotBytes) {
        if (g_probe) {
            static int resyncs;
            if (++resyncs <= 20) {
                Logf("private_shops  [SLOT] index %zu now %p (was %p) - "
                     "layout shifted, record re-synced, no delta billed",
                     globalIndex, slotBytes, snap.slot);
            }
        }
        snap.slot     = slotBytes;
        snap.resource = res;
        snap.content  = content;
        return;
    }

    // v2.9: dispatch means an EXPORT storage. Consumption draws from
    // import-flagged storages ($CONSUMPTION fabric pulls the factory's
    // $STORAGE_IMPORT; coal plants burn from $STORAGE_IMPORT_SPECIAL)
    // and must not be paid for - the parser stamped the direction byte
    // at +0x85 when it built this block (FUN_1400e40f0).
    bool isExport = storage &&
        *(const BYTE*)((const BYTE*)storage + TSM_ST_EXPORT) != 0;
    bool isImport = storage &&
        *(const BYTE*)((const BYTE*)storage + TSM_ST_IMPORT) != 0;

    // v3.3: where is this storage in the building's list? The visitor is
    // handed the block, not the array base, so re-read the base from the
    // building - it is the same two pointers ForEachSlot walked.
    int storIndex = -1;
    {
        void** sb = *(void***)((BYTE*)ctx->shop + TSM_BLD_STORAGES_BEGIN);
        void** se = *(void***)((BYTE*)ctx->shop + TSM_BLD_STORAGES_END);
        if (sb && se && (BYTE*)se > (BYTE*)sb && storage &&
            (const BYTE*)storage >= (const BYTE*)sb &&
            (const BYTE*)storage <  (const BYTE*)se) {
            storIndex = (int)(((const BYTE*)storage - (const BYTE*)sb) /
                              TSM_STORAGE_STRIDE);
        }
    }

    // v3.3: an unmarked storage on a production building. Since v2.8 the
    // only two directions this code understands are +0x84/+0x85; a block
    // with neither is invisible to both the billing path and the
    // procurement path, and a factory's output simply leaves unpaid. The
    // observed fabric_factory declares plants+chemicals in storage #0 and
    // fabric in storage #1, so the last block is the output. Logged every
    // time it decides a credit, so the next session can confirm or refute.
    bool flagless = storage && !isExport && !isImport;
    if (g_procureFallback && !ctx->isRetail && flagless &&
        storIndex >= 0 && storIndex == ctx->lastFreightStorIndex &&
        DispatchFallbackAllowed(TsmBuildingType(ctx->shop),
                                ctx->storCount)) {
        isExport = true;
    }

    // v2.8: outgoing dispatch on a non-retail building = state procurement.
    // Inflow (delta > +0.001) is still billed wholesale as before.
    if (!ctx->isRetail && isExport && ctx->state->snapshotDone) {
        double delta = (double)content - (double)snap.content;
        if (delta < -0.001 && snap.resource && content >= 0.0f) {
            double tons = -(double)delta;
            // v3.0 (Task 2): dispatched output is a Republic SELL ORDER -
            // record it in the market book before pricing it.
            TsmPricingRecordSupply(g_pricing, snap.resource, tons);
            // v4: state procurement pays the active-currency retail price.
            float priceT = g_pricing->getPrice(snap.resource, TsmCurrencyActive(g_currency));
            if (g_probe) {
                static int drains;
                if (++drains <= 40)
                    Logf("private_shops  [PROC] %p storage #%d exp=%d imp=%d "
                         "flagless=%d res %p %.3f -> %.3f (%.3f t left) "
                         "price %.2f",
                         ctx->shop, storIndex,
                         isExport ? 1 : 0, isImport ? 1 : 0,
                         flagless ? 1 : 0, snap.resource,
                         (double)snap.content, (double)content, tons,
                         (double)priceT);
            }
            if (priceT > 0.01f) {
                // v3.0 (Task 2): factory payout = base x marketMult x 0.90 -
                // the state adjusts its take for gluts and shortages, so a
                // flooded Republic automatically earns less per batch.
                float marketMult = TsmPricingMarketMult(g_pricing, snap.resource);
                double value = tons * (double)priceT * (double)marketMult
                             * (double)g_procurementMult;
                if (value > 0.0) {
                    g_currency->moveTreasury(-value, "procurement");
                    g_enterprise->modifyCompanyCapital(
                        ctx->shop, value, TSM_CAP_PRODUCTION);
                    ctx->procured++;
                    ctx->revenue += value;
                    if (flagless) ctx->fallback++;   // v3.3: audited above
                    snap.content = content;
                    return;
                }
            }
        }
        // Resource changed under the dispatch: still procurement on what left.
        if (snap.resource != res) {
            snap.resource = res;
            snap.content  = content;
            return;
        }
    }

    if (snap.resource != res) {
        snap.resource = res;
        snap.content  = content;
        return;
    }

    float delta = content - snap.content;
    // v3.0 (Task 2): stock drawn from an import/import-special/fuel
    // storage is a factory feeding its production line - a Republic BUY
    // ORDER for that input resource. Export outflow reaches here only
    // when the dispatch branch above declined it (e.g. priceT unusable),
    // and dispatch is NOT demand; the isImport gate keeps those cases
    // strictly separated, exactly mirroring the v2.9 direction rule.
    if (delta < -0.001f && !isExport && isImport && snap.resource) {
        TsmPricingRecordDemand(g_pricing, snap.resource, (double)(-delta));
    }
    if (delta <= 0.001f) {
        snap.content = content;
        return;
    }

    // v4: state wholesale delivery bills the active-currency price.
    float perTon = g_pricing->getPrice(res, TsmCurrencyActive(g_currency));
    if (!(perTon > 0.0f)) { snap.content = content; return; }

    double wholesale = (double)delta * (double)perTon * (double)g_wholesaleRatio;
    if (wholesale <= 0.0) { snap.content = content; return; }

    // A native delivery has already happened. Settle its full wholesale cost,
    // allowing company capital to become negative; never erase delivered stock
    // to enforce affordability after the vehicle has relinquished the goods.
    g_enterprise->chargeWholesaleDelivery(ctx->shop, wholesale);
    ctx->capital -= wholesale;
    ctx->charged += wholesale;
    ctx->billed++;
    snap.content = content;

}

static void RunWholesaleBilling(void* building, ShopState& st)
{
    int type = TsmBuildingType(building);
    bool isRetail = IsRetailType(type);
    // Compute the number of non-utility storage blocks and the raw index of
    // the last non-utility block so trailing electricity/water/sewage/heating
    // buffers neither inflate DispatchFallbackAllowed nor mask a factory's
    // unmarked freight output block.
    int freightStorCount = 0;
    int lastFreightStorIndex = -1;
    {
        void** sb = *(void***)((BYTE*)building + TSM_BLD_STORAGES_BEGIN);
        void** se = *(void***)((BYTE*)building + TSM_BLD_STORAGES_END);
        if (sb && se && (BYTE*)se > (BYTE*)sb &&
            TsmIsValidGamePtr(sb) && ReadablePtr(sb, sizeof(void*))) {
            size_t storBytes = (size_t)((BYTE*)se - (BYTE*)sb);
            if ((storBytes % TSM_STORAGE_STRIDE) == 0) {
                size_t rawCount = storBytes / TSM_STORAGE_STRIDE;
                if (rawCount > 16) rawCount = 16;
                if (ReadablePtr(sb, rawCount * TSM_STORAGE_STRIDE)) {
                    for (size_t si = 0; si < rawCount; si++) {
                        BYTE* storage = (BYTE*)sb + si * TSM_STORAGE_STRIDE;
                        if (!ReadablePtr(storage, 0xA0)) continue;
                        void** slotBegin = *(void***)(storage + TSM_ST_SLOTS_BEGIN);
                        void** slotEnd   = *(void***)(storage + TSM_ST_SLOTS_END);
                        if (!slotBegin || !slotEnd || slotEnd <= slotBegin) continue;
                        if (!TsmIsValidGamePtr(slotBegin)) continue;
                        size_t slotBytes = (size_t)((BYTE*)slotEnd - (BYTE*)slotBegin);
                        if ((slotBytes % TSM_SLOT_STRIDE) != 0) continue;
                        size_t slotCount = slotBytes / TSM_SLOT_STRIDE;
                        if (slotCount > 32) slotCount = 32;
                        if (!ReadablePtr(slotBegin, slotCount * TSM_SLOT_STRIDE)) continue;

                        bool hasFreight = false;
                        for (size_t sj = 0; sj < slotCount; sj++) {
                            BYTE* slot = (BYTE*)slotBegin + sj * TSM_SLOT_STRIDE;
                            void* res  = *(void**)(slot + TSM_SLOT_RESOURCE);
                            char rname[64];
                            if (TsmIsValidGamePtr(res) &&
                                TsmResourceName(res, rname, sizeof(rname)) &&
                                wrsre_shops::IsUtilityResourceName(rname))
                            {
                                continue;
                            }
                            hasFreight = true;
                            break;
                        }
                        if (hasFreight) {
                            freightStorCount++;
                            lastFreightStorIndex = (int)si;
                        }
                    }
                }
            }
        }
    }

    WholesaleContext ctx = {
        building, &st,
        g_enterprise->getCompanyCapital(building),
        isRetail,
        0, 0.0,
        0, 0.0,
        freightStorCount, lastFreightStorIndex, 0
    };
    ForEachSlot(building,
                TsmB2bCounterpartyActive(g_b2bLogistics) ? WholesaleMarketOnlySlot
                                                         : WholesaleSlot,
                &ctx);

    auto fillName = [building](wchar_t* out, size_t cap) {
        out[0] = L'\0';
        if (g_enterprise->getBuildingCustomName)
            g_enterprise->getBuildingCustomName(building, out, cap);
        if (!out[0] && g_enterprise->getBuildingObjectName) {
            const char* obj = g_enterprise->getBuildingObjectName(building);
            if (obj && obj[0]) {
                size_t i = 0;
                for (; i + 1 < cap && obj[i]; ++i)
                    out[i] = (wchar_t)(unsigned char)obj[i];
                out[i] = L'\0';
            }
        }
        if (!out[0]) wcsncpy_s(out, cap, L"(unnamed)", _TRUNCATE);
    };

    if (ctx.revenue > 0.0 && g_probe) {
        wchar_t name[128] = {0};
        fillName(name, _countof(name));
        Logf("private_shops  \"%ls\" procured %d dispatch(es) for %.2f %s "
             "(%d of them on an UNMARKED storage - the +0x85 export flag was "
             "not set; see [PROC])",
             name, ctx.procured, ctx.revenue, TsmCurrencySymbol(g_currency),
             ctx.fallback);
    }

    if (ctx.charged > 0.0) {
        st.monthlyWholesale += ctx.charged;
        if (g_probe) {
            wchar_t name[128] = {0};
            fillName(name, _countof(name));
            const double capital = g_enterprise->getCompanyCapital(building);
            Logf("private_shops  \"%ls\" charged %.2f for state delivery (%d slot(s)); capital %.2f%s",
                 name, ctx.charged, ctx.billed, capital,
                 capital < 0.0 ? " (debt; delivered stock retained)" : "");
        }
    }
}

// ---------------------------------------------------------------- service fees
//
// A service building charges a fee to a citizen who is inside and
// carrying the matching demand. That is what separates a customer
// from an employee:
//
//   a pub customer  has an alcohol demand  (kind 3)
//   a cinema-goer   has a culture demand   (kind 5)
//   a church-goer   has a religion demand  (kind 4)
//   a patient       has a health demand    (kind 10)
//   a hotel guest   has none of the above - see the ini for the
//                   `any` kind, which charges every non-foreign,
//                   non-tourist citizen inside
//
// The charge is once per citizen per in-game day, tracked by absolute
// day so a citizen who walks in, out and back in again is charged
// once.

static inline int AbsDay(int day)
{
    int year = g_simClock ? g_simClock->getYear() : -1;
    if (year < 0 || day < 0) return -1;
    return (year * 365) + day;
}

static bool PersonCarriesDemand(const BYTE* person, int kind)
{
    if (kind < 0) return true;      // caller asked for "any"

    int demandCount = *(const int*)(person + TSM_PERSON_DEMAND_N);
    if (demandCount <= 0) return false;
    if (demandCount > TSM_PERSON_DEMAND_CAP) demandCount = TSM_PERSON_DEMAND_CAP;

    const BYTE* demands = person + TSM_PERSON_DEMANDS;
    for (int d = 0; d < demandCount; d++) {
        const BYTE* dem = demands + (size_t)d * TSM_PERSON_DEMAND_STRIDE;
        if (*(const int*)(dem + 0x08) == kind) return true;
    }
    return false;
}

// ---------------------------------------------------------------- demand lifetime probe (v2.6)
//
// Answers: when our tick looks at a service building, do the patrons
// inside still carry the demand kind we bill against, or has the
// vanilla service tick already cleared it? Each patron is sampled
// every tick. Within one in-game day a patron accumulates flags:
// bit0 = seen with the demand, bit1 = seen without it. A with->without
// transition is what "cleared by the vanilla drink tick" looks like
// from outside; without->with means the demand was re-created mid-day.
// One summary line per building per in-game day.

static void ProbeDemandLifetime(void* building, int type, const ServiceFee& fee, int day)
{
    if (!g_probePubDemand) return;
    if (fee.kind < 0) return;          // `any` fees have no kind to track

    BYTE* b = (BYTE*)building;
    void** begin = *(void***)(b + TSM_BLD_PEOPLE_BEGIN);
    void** end   = *(void***)(b + TSM_BLD_PEOPLE_END);
    if (!begin || !end || end <= begin) return;
    if (!TsmIsValidGamePtr(begin)) return;
    if (!ReadablePtr(begin, sizeof(void*))) return;

    size_t n = (size_t)(end - begin);
    if (n > 200) n = 200;

    int absDay = AbsDay(day);
    if (absDay < 0) return;

    DemandProbeState* st = &g_demandProbe[building];

    // Day rollover: report the finished day and reset the per-patron
    // flags before this tick's samples start the new day's picture.
    if (st->lastLogDay != absDay) {
        if (st->lastLogDay >= 0 && !st->dayFlags.empty()) {
            size_t patrons = st->dayFlags.size();
            size_t held = 0, never = 0;
            for (const auto& kv : st->dayFlags) {
                if (kv.second & 1) held++;
                if ((kv.second & 3) == 2) never++;   // seen without, never with
            }
            wchar_t name[128] = {0};
            g_enterprise->getBuildingCustomName(building, name, 128);
            Logf("private_shops  demand probe: \"%ls\" type %d kind %d - "
                 "%zu patron(s) sampled: %zu held the demand at least once, "
                 "%zu never showed it; transitions: %lld with->without (cleared), "
                 "%lld without->with (reappeared)",
                 name[0] ? name : L"(unnamed)", type, fee.kind,
                 patrons, held, never, st->cleared, st->reappeared);
        }
        st->dayFlags.clear();
        st->cleared = 0;
        st->reappeared = 0;
        st->withSamples = 0;
        st->withoutSamples = 0;
        st->lastLogDay = absDay;
    }

    for (size_t i = 0; i < n; i++) {
        void* person = begin[i];
        if (!TsmIsValidGamePtr(person)) continue;
        if (!ReadablePtr(person, TSM_PERSON_SIZE)) continue;

        int foreign = *(int*)((BYTE*)person + TSM_PERSON_FOREIGN);
        int tourist = *(int*)((BYTE*)person + TSM_PERSON_TOURIST);
        if (foreign != 0 || tourist != 0) continue;

        uint64_t id = TsmCitizenID(person);
        if (!id) continue;

        unsigned& flags = st->dayFlags[id];
        if (PersonCarriesDemand((const BYTE*)person, fee.kind)) {
            st->withSamples++;
            if (flags & 2) st->reappeared++;
            flags |= 1;
        } else {
            st->withoutSamples++;
            if (flags & 1) st->cleared++;
            flags |= 2;
        }
    }
}

static void GuardedDemandProbe(void* b, int type, const ServiceFee& fee, int day)
{
    __try { ProbeDemandLifetime(b, type, fee, day); }
    __except (FaultFilter("private_shops demand probe", GetExceptionInformation())) {}
}

static void RunServiceCharge(void* building, int type, const ServiceFee& fee, int day)
{
    BYTE* b = (BYTE*)building;
    void** begin = *(void***)(b + TSM_BLD_PEOPLE_BEGIN);
    void** end   = *(void***)(b + TSM_BLD_PEOPLE_END);
    if (!begin || !end || end <= begin) return;
    if (!TsmIsValidGamePtr(begin)) return;
    if (!ReadablePtr(begin, sizeof(void*))) return;

    size_t n = (size_t)(end - begin);
    if (n > 200) n = 200;

    float vatRate = g_taxes ? g_taxes->getVatRate() : 0.0f;
    if (vatRate < 0.0f) vatRate = 0.0f;
    int charged = 0;
    double totalTaken = 0.0;

    int absDay = AbsDay(day);
    if (absDay < 0) return;

    for (size_t i = 0; i < n; i++) {
        void* person = begin[i];
        if (!TsmIsValidGamePtr(person)) continue;
        if (!ReadablePtr(person, TSM_PERSON_SIZE)) continue;

        int foreign = *(int*)((BYTE*)person + TSM_PERSON_FOREIGN);
        int tourist = *(int*)((BYTE*)person + TSM_PERSON_TOURIST);
        if (foreign != 0 || tourist != 0) continue;

        if (!PersonCarriesDemand((const BYTE*)person, fee.kind)) continue;

        uint64_t id = TsmCitizenID(person);
        if (!id) continue;

        EnterCriticalSection(&g_lock);
        auto it = g_serviceLastVisit.find(id);
        if (it != g_serviceLastVisit.end() && it->second == absDay) {
            LeaveCriticalSection(&g_lock);
            continue;
        }
        g_serviceLastVisit[id] = absDay;
        LeaveCriticalSection(&g_lock);

        // v4: fees are RUB-denominated in the ini; scale for USD mode.
        float taken = g_currency->chargeCitizenPartial(person, EffectiveFee(fee));
        if (taken <= 0.0f) continue;

        // v2.6: enterprise_base v5.3 split. `taken` is the VAT-inclusive
        // gross - the fee is split, never grossed up. SettleVatAll later
        // debits vatAccrued from capital, which leaves exactly netToShop;
        // periodGrossSales keeps the gross so SettleProfitAll's
        // netRevenue = gs - vatAccrued lands on the same net figure.
        double netToShop  = (double)taken / (1.0 + (double)vatRate);
        double vatAccrued = (double)taken - netToShop;

        g_enterprise->modifyCompanyCapital(building, (double)taken, TSM_CAP_RETAIL);
        if (g_taxes && vatAccrued > 0.001) {
            g_taxes->recordVatAccrual(building, vatAccrued);
        }

        charged++;
        totalTaken += (double)taken;
    }

    if (charged > 0 && g_probe) {
        wchar_t name[128] = {0};
        g_enterprise->getBuildingCustomName(building, name, 128);
        Logf("private_shops  service type %d (kind %d, %.2f %s): \"%ls\" served %d, %.2f %s",
             type, fee.kind, EffectiveFee(fee), TsmCurrencySymbol(g_currency),
             name[0] ? name : L"(unnamed)", charged, totalTaken,
             TsmCurrencySymbol(g_currency));
    }
}

// ---------------------------------------------------------------- one-shot diagnostic

static void LogPrivateBuildings(BYTE* game)
{
    TsmBuildingSpan span;
    if (!game || !TsmGetBuildings(game, &span)) return;

    int total = 0;
    Logf("private_shops  --- private, finished buildings on the map ---");
    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;
        if (!TsmIsFinished(b)) continue;

        int type = TsmBuildingType(b);

        // v2.6: the storage-semantics probe covers every finished pub,
        // state or private, so state pubs and [P] pubs can be compared
        // side by side in the same dump.
        if (type == 24) GuardedStorageProbe(b, type);

        if (!g_enterprise->isBuildingPrivate(b)) continue;
        const char* obj = g_enterprise->getBuildingObjectName(b);

        wchar_t custom[128] = {0};
        g_enterprise->getBuildingCustomName(b, custom, 128);

        const char* suffix = "";
        if (FeeForType(type))        suffix = "   [service fee]";
        else if (IsRetailType(type)) suffix = "   [retail]";
        else if (g_restockAll || IsInList(g_restockTypes, g_restockTypeCount, type))
            suffix = "   [restock]";

        Logf("private_shops    type %-4d  obj \"%s\"  name \"%ls\"%s",
             type, obj, custom[0] ? custom : L"(unnamed)", suffix);
        total++;
    }
    Logf("private_shops  %d private, finished building(s) total", total);
    Logf("private_shops  to make one of these private-tradeable, add its type above to "
         "restock_types (or use `all`) and/or service_fees in plugins\\private_shops.ini");
}

// ---------------------------------------------------------------- the tick

static void GuardedRestock(void* b, ShopState* st, int day)
{
    __try { RunForeignRestock(b, *st, day); }
    __except (FaultFilter("private_shops foreign restock", GetExceptionInformation())) {}
}

static void GuardedWholesale(void* b, ShopState* st)
{
    __try { RunWholesaleBilling(b, *st); }
    __except (FaultFilter("private_shops wholesale billing", GetExceptionInformation())) {}
}

static void GuardedService(void* b, int type, const ServiceFee& fee, int day)
{
    __try { RunServiceCharge(b, type, fee, day); }
    __except (FaultFilter("private_shops service charge", GetExceptionInformation())) {}
}

static void RunShopTick()
{
    static DWORD last = 0;
    DWORD now = GetTickCount();
    if (now - last < (DWORD)g_tickIntervalMs) return;
    last = now;

    int day = g_simClock->getDay();
    if (day < 0) return;

    // Daily in-memory pruning pass for service visit markers.
    static int lastPruneAbsDay = -1;
    int currentAbsDay = AbsDay(day);
    if (currentAbsDay > 0 && currentAbsDay != lastPruneAbsDay) {
        lastPruneAbsDay = currentAbsDay;
        EnterCriticalSection(&g_lock);
        for (auto it = g_serviceLastVisit.begin(); it != g_serviceLastVisit.end(); ) {
            if (currentAbsDay - it->second > 2) it = g_serviceLastVisit.erase(it);
            else ++it;
        }
        LeaveCriticalSection(&g_lock);
    }

    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;

    TsmBuildingSpan span;
    if (!TsmGetBuildings(game, &span)) return;

    // One-shot diagnostic, on the first tick after a world is present.
    if (!g_loggedPrivateBuildings) {
        g_loggedPrivateBuildings = true;
        LogPrivateBuildings(game);
    }

    std::unordered_set<void*> live;
    live.reserve(span.count);

    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;

        int type = TsmBuildingType(b);
        bool isRestock = g_restockAll || IsInList(g_restockTypes, g_restockTypeCount, type);
        const ServiceFee* fee = FeeForType(type);
        if (!isRestock && !fee) continue;

        if (!g_enterprise->isBuildingPrivate(b)) continue;
        if (!TsmIsFinished(b)) continue;

        live.insert(b);

        if (isRestock) {
            ShopState& st = g_shops[b];
            GuardedStorageProbe(b, type);          // v2.6: first walk dumps slot limits
            GuardedRestock(b, &st, day);
            GuardedWholesale(b, &st);
            st.snapshotDone = true;
        }

        if (fee) {
            GuardedDemandProbe(b, type, *fee, day); // v2.6: demand lifetime audit
            GuardedService(b, type, *fee, day);
        }
    }

    // Drop per-building state for anything that no longer exists.
    for (auto it = g_shops.begin(); it != g_shops.end(); ) {
        if (live.count(it->first)) ++it;
        else                        it = g_shops.erase(it);
    }
    for (auto it = g_demandProbe.begin(); it != g_demandProbe.end(); ) {
        if (live.count(it->first)) ++it;
        else                        it = g_demandProbe.erase(it);
    }
}

// ---------------------------------------------------------------- persistence

#define SHOPS_MAGIC    "TSMSHP01"
#define SHOPS_VERSION  1u
#define SHOPS_FILENAME "tesmio_shops.dat"

#pragma pack(push, 1)
struct ShopsSaveHeader {
    char     magic[8];
    uint32_t version;
    uint32_t recordSize;
    uint32_t count;
    uint32_t reserved;
};
struct ShopsSaveRecord {
    uint64_t citizenId;
    int32_t  lastVisitDay;
};
#pragma pack(pop)

static void SaveShops(const char* dir)
{
    std::vector<ShopsSaveRecord> records;
    EnterCriticalSection(&g_lock);
    records.reserve(g_serviceLastVisit.size());
    int currentAbsDay = AbsDay(g_simClock ? g_simClock->getDay() : 0);
    for (const auto& kv : g_serviceLastVisit) {
        if (currentAbsDay > 0 && currentAbsDay - kv.second > 2) continue;
        ShopsSaveRecord r;
        r.citizenId    = kv.first;
        r.lastVisitDay = kv.second;
        records.push_back(r);
    }
    LeaveCriticalSection(&g_lock);

    size_t count = records.size();
    size_t total = sizeof(ShopsSaveHeader) + count * sizeof(ShopsSaveRecord);
    std::vector<BYTE> buf(total);

    ShopsSaveHeader hdr;
    memcpy(hdr.magic, SHOPS_MAGIC, 8);
    hdr.version    = SHOPS_VERSION;
    hdr.recordSize = (uint32_t)sizeof(ShopsSaveRecord);
    hdr.count      = (uint32_t)count;
    hdr.reserved   = 0;
    memcpy(buf.data(), &hdr, sizeof(hdr));

    if (count > 0)
        memcpy(buf.data() + sizeof(hdr), records.data(), count * sizeof(ShopsSaveRecord));

    if (g_persistence->writeAtomic(dir, SHOPS_FILENAME, buf.data(), total))
        Logf("private_shops  saved %zu service visit record(s) into %s\\%s",
             count, dir, SHOPS_FILENAME);
    else
        Logf("private_shops  WARN  could not write %s\\%s", dir, SHOPS_FILENAME);
}

static void LoadShops(const char* dir)
{
    EnterCriticalSection(&g_lock);
    g_shops.clear();
    g_loggedPrivateBuildings = false;
    g_storageProbed.clear();
    g_demandProbe.clear();
    LeaveCriticalSection(&g_lock);

    size_t len = 0;
    void* raw = g_persistence->readFile(dir, SHOPS_FILENAME, &len);
    if (!raw) {
        EnterCriticalSection(&g_lock);
        g_serviceLastVisit.clear();
        LeaveCriticalSection(&g_lock);
        Logf("private_shops  no %s in this save - starting empty", SHOPS_FILENAME);
        return;
    }

    if (len < sizeof(ShopsSaveHeader)) {
        Logf("private_shops  %s too short - ignored", SHOPS_FILENAME);
        free(raw);
        return;
    }

    ShopsSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));
    if (memcmp(hdr.magic, SHOPS_MAGIC, 8) != 0 ||
        hdr.version    != SHOPS_VERSION ||
        hdr.recordSize != sizeof(ShopsSaveRecord))
    {
        Logf("private_shops  %s version %u, expected %u - ignored",
             SHOPS_FILENAME, hdr.version, SHOPS_VERSION);
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.count * sizeof(ShopsSaveRecord);
    if (want > len) {
        Logf("private_shops  %s claims %u records but is only %zu bytes",
             SHOPS_FILENAME, hdr.count, len);
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);
    g_serviceLastVisit.clear();
    g_serviceLastVisit.reserve(hdr.count);

    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.count; i++) {
        ShopsSaveRecord r;
        memcpy(&r, p, sizeof(r));
        p += sizeof(r);
        g_serviceLastVisit.emplace(r.citizenId, r.lastVisitDay);
    }
    size_t loaded = g_serviceLastVisit.size();
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("private_shops  loaded %zu service visit record(s) from %s\\%s",
         loaded, dir, SHOPS_FILENAME);
}

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir || !g_persistence) return;
    if      (ev == TSM_SAVE_WRITING) SaveShops(dir);
    else if (ev == TSM_SAVE_LOADING) LoadShops(dir);
}

// ---------------------------------------------------------------- settings

static void ParseRestockTypes(const char* s)
{
    g_restockTypeCount = 0;
    g_restockAll       = 0;

    if (!s || !s[0]) {
        g_restockTypes[g_restockTypeCount++] = 3;
        g_restockTypes[g_restockTypeCount++] = 24;
        return;
    }

    if (_stricmp(s, "all") == 0) { g_restockAll = 1; return; }
    if (_stricmp(s, "none") == 0) { g_restockAll = 0; return; }

    while (*s && g_restockTypeCount < MAX_TYPE_LIST) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        g_restockTypes[g_restockTypeCount++] = atoi(s);
        while (*s && *s != ',') s++;
    }

    if (g_restockTypeCount == 0) {
        g_restockTypes[g_restockTypeCount++] = 3;
        g_restockTypes[g_restockTypeCount++] = 24;
    }
}

// v2.9: the retail set - types whose customer sales must never be read
// as state procurement dispatch. Absent/empty key gives the default
// service-building set; a non-empty string is parsed as ints only, so
// `none` (or any non-numeric text) deliberately empties the list. Type
// 3 is hardcoded retail in IsRetailType regardless.
static void ParseRetailTypes(const char* s)
{
    g_retailTypeCount = 0;

    if (!s || !s[0]) {
        g_retailTypes[g_retailTypeCount++] = 3;
        g_retailTypes[g_retailTypeCount++] = 4;
        g_retailTypes[g_retailTypeCount++] = 10;
        g_retailTypes[g_retailTypeCount++] = 23;
        g_retailTypes[g_retailTypeCount++] = 24;
        g_retailTypes[g_retailTypeCount++] = 25;
        g_retailTypes[g_retailTypeCount++] = 26;
        g_retailTypes[g_retailTypeCount++] = 28;
        return;
    }

    while (*s && g_retailTypeCount < MAX_TYPE_LIST) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        int t = atoi(s);
        if (t > 0) g_retailTypes[g_retailTypeCount++] = t;
        while (*s && *s != ',') s++;
    }
}

// "24:3:5.0, 25:5:15.0, 30:any:20.0" -> array of ServiceFee.
//   <type>:<kind|any>:<fee>[:<fee_usd>]
// An entry that fails to parse is logged and skipped. `any` maps to
// kind -1, which PersonCarriesDemand treats as "every citizen inside".
// v4: the optional 4th field is an explicit USD fee; without it USD mode
// divides the RUB fee by usd_divisor.
static void ParseServiceFees(const char* s)
{
    g_serviceFeeCount = 0;
    if (!s || !s[0]) return;

    while (*s && g_serviceFeeCount < MAX_SERVICE_FEES) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;

        const char* end = strchr(s, ',');
        size_t len = end ? (size_t)(end - s) : strlen(s);

        char buf[64];
        if (len >= sizeof(buf)) len = sizeof(buf) - 1;
        memcpy(buf, s, len);
        buf[len] = 0;

        char* p1 = strchr(buf, ':');
        if (p1) {
            *p1 = 0;
            char* p2 = strchr(p1 + 1, ':');
            if (p2) {
                *p2 = 0;
                int   type = atoi(buf);
                const char* kindStr = p1 + 1;
                char* p3 = strchr(p2 + 1, ':');
                float feeUsd = -1.0f;
                if (p3) {
                    *p3 = 0;
                    float u = (float)atof(p3 + 1);
                    if (u > 0.0f && u == u) feeUsd = u;
                }
                float fee  = (float)atof(p2 + 1);

                int kind;
                if (_stricmp(kindStr, "any") == 0) kind = -1;
                else                               kind = atoi(kindStr);

                if (type > 0 && fee > 0.0f && (kind == -1 || (kind >= 1 && kind <= 10))) {
                    bool duplicate = false;
                    for (int i = 0; i < g_serviceFeeCount; i++) {
                        if (g_serviceFees[i].type == type) { duplicate = true; break; }
                    }
                    if (duplicate) {
                        Logf("private_shops  service_fees entry \"%s\" rejected - "
                             "type %d is already configured (first entry wins)",
                             buf, type);
                    } else {
                        ServiceFee& f = g_serviceFees[g_serviceFeeCount++];
                        f.type = type;
                        f.kind = kind;
                        f.fee  = fee;
                        f.feeUsd = feeUsd;
                    }
                } else {
                    Logf("private_shops  service_fees entry \"%s\" rejected "
                         "(type>0, kind 1..10 or `any`, fee>0 required)", buf);
                }
            } else {
                Logf("private_shops  service_fees entry \"%s\" rejected - "
                     "needs <type>:<kind>:<fee>", buf);
            }
        } else {
            Logf("private_shops  service_fees entry \"%s\" rejected - "
                 "needs <type>:<kind>:<fee>", buf);
        }

        if (!end) break;
        s = end + 1;
    }
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_shops.ini";
    char v[256];

    g_enabled = H->configInt(ini, "private_shops", "enabled", 1);
    g_probe   = H->configInt(ini, "private_shops", "probe",   1);
    g_probePubStorage = H->configInt(ini, "private_shops", "probe_pub_storage", 1);
    g_probePubDemand  = H->configInt(ini, "private_shops", "probe_pub_demand",  1);
    g_tickIntervalMs = H->configInt(ini, "private_shops",
                                    "tick_interval_ms", 300);

    if (H->configString(ini, "private_shops", "restock_types", v, sizeof(v), ""))
        ParseRestockTypes(v);
    else
        ParseRestockTypes(nullptr);

    if (H->configString(ini, "private_shops", "retail_types", v, sizeof(v), "") && v[0])
        ParseRetailTypes(v);
    else
        ParseRetailTypes(nullptr);

    // service_fees wins when present. When it is absent, the older
    // pub_types list is expanded with pub_drink_price and treated as
    // alcohol-kind fees for every listed type, which is exactly what
    // the pre-v2.5 code did.
    if (H->configString(ini, "private_shops", "service_fees", v, sizeof(v), "") && v[0])
    {
        ParseServiceFees(v);
    }
    else
    {
        float pubPrice = 5.0f;
        char  pv[64];
        if (H->configString(ini, "private_shops", "pub_drink_price", pv, sizeof(pv), "") && pv[0]) {
            float f = (float)atof(pv);
            if (f > 0.0f) pubPrice = f;
        }

        char pubList[128] = "24";
        H->configString(ini, "private_shops", "pub_types", pubList, sizeof(pubList), "24");

        char combined[256];
        _snprintf_s(combined, sizeof(combined), _TRUNCATE, "%s", "");
        const char* s = pubList;
        while (*s && g_serviceFeeCount < MAX_SERVICE_FEES) {
            while (*s == ' ' || *s == ',' || *s == '\t') s++;
            if (!*s) break;
            int t = atoi(s);
            if (t > 0) {
                ServiceFee& f = g_serviceFees[g_serviceFeeCount++];
                f.type = t;
                f.kind = 3;         // alcohol
                f.fee  = pubPrice;
                f.feeUsd = -1.0f;   // v4: auto-scaled by usd_divisor
            }
            while (*s && *s != ',') s++;
        }
        if (g_serviceFeeCount > 0) {
            Logf("private_shops  service_fees not set - derived %d entr(ies) from "
                 "pub_types (%s) x pub_drink_price (%.2f), all kind 3 (alcohol)",
                 g_serviceFeeCount, pubList, pubPrice);
        }
    }

    if (H->configString(ini, "private_shops", "wholesale_ratio", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 5.0f) g_wholesaleRatio = f;
    }
    // C1 FIX: clamp tiny ratios to 0.01 — prevents INF tons via divide-by-zero in WholesaleSlot affordable path
    if (g_wholesaleRatio < 0.01f) {
        Logf("private_shops  wholesale_ratio %.3f < 0.01 — clamped to 0.01 (C1 fix)", (double)g_wholesaleRatio);
        g_wholesaleRatio = 0.01f;
    }
    g_procureFallback = H->configInt(ini, "private_shops",
                                     "procurement_flag_fallback", 1);
    if (H->configString(ini, "private_shops", "procurement_price_mult", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 5.0f) g_procurementMult = f;
    }
    // v4: USD divisor for service_fees without an explicit :fee_usd field.
    if (H->configString(ini, "private_shops", "usd_divisor", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 1000.0f && f == f) g_usdDivisor = f;
    }

    g_foreignEnabled = H->configInt(ini, "private_shops", "foreign_restock_enabled", 0);
    if (H->configString(ini, "private_shops", "foreign_restock_target", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f) g_foreignTargetRatio = f;
    }
    if (H->configString(ini, "private_shops", "restock_reorder_point", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f) g_restockReorderPoint = f;
    }
    if (H->configString(ini, "private_shops", "foreign_restock_markup", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 10.0f) g_foreignMarkup = f;
    }

    if (g_tickIntervalMs < 50)   g_tickIntervalMs = 50;
    if (g_tickIntervalMs > 5000) g_tickIntervalMs = 5000;
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_shops";
    info->version = "3.5";

    ReadSettings();
    if (!g_enabled) {
        Logf("private_shops  disabled");
        return 1;
    }
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) { Logf("private_shops  sim_clock is not installed"); return 1; }

    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) { Logf("private_shops  currency is not installed"); return 1; }

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing) { Logf("private_shops  resource_pricing is not installed"); return 1; }
    if (TsmPricingHasMarket(g_pricing))
        Logf("private_shops  Republic market book available - dispatches and imports feed supply");

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise) { Logf("private_shops  enterprise_base is not installed"); return 1; }

    g_b2bLogistics = (const TsmB2bLogisticsApi*)H->consume(TSM_SERVICE_B2B_LOGISTICS, TSM_B2B_LOGISTICS_VERSION);

    g_taxes = (const TsmTaxesApi*)H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION);
    // v3.5: retain older taxes providers after the v3 snapshot API bump.
    if (!g_taxes)
        g_taxes = (const TsmTaxesApi*)H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V2);
    if (!g_taxes)
        g_taxes = (const TsmTaxesApi*)H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V1);
    if (!g_taxes) Logf("private_shops  WARN  taxes service not installed");

    g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence) {
        Logf("private_shops  WARN  no persistence service");
    } else {
        g_persHandle = g_persistence->registerHandler("private_shops", OnSaveLoad, nullptr);
        if (g_persHandle <= 0) Logf("private_shops  WARN  persistence refused handler");
    }

    g_simClock->registerTick([](void*, float, float) { RunShopTick(); }, nullptr);

    const char* restockSummary = g_restockAll ? "all" : "list";
    Logf("private_shops  live v3.5: interval %d ms, slot-identity snapshots, "
         "restock %s (%d type(s)), "
         "retail %d type(s), %d service fee(s), wholesale %.2f, procurement %.2f (flag fallback %s), foreign %s (target %.0f%%, markup %.2f), taxes %s, currency %s",
         g_tickIntervalMs, restockSummary, g_restockTypeCount,
         g_retailTypeCount, g_serviceFeeCount,
         g_wholesaleRatio,
         g_procurementMult,
         g_procureFallback ? "on" : "off",
         g_foreignEnabled ? "on" : "off",
         g_foreignTargetRatio * 100.0f, g_foreignMarkup,
         g_taxes ? "on" : "off", TsmCurrencySymbol(g_currency));

    if (g_retailTypeCount > 0) {
        char retailList[256] = "";
        size_t pos = 0;
        for (int i = 0; i < g_retailTypeCount; i++) {
            int n = _snprintf_s(retailList + pos, sizeof(retailList) - pos,
                                _TRUNCATE, "%s%d", i ? "," : "", g_retailTypes[i]);
            if (n <= 0) break;
            pos += (size_t)n;
        }
        Logf("private_shops    retail types: %s", retailList);
    }

    for (int i = 0; i < g_serviceFeeCount; i++) {
        const ServiceFee& f = g_serviceFees[i];
        if (f.kind < 0)
            Logf("private_shops    service: type %-4d any demand    %.2f %s/visit",
                 f.type, EffectiveFee(f), TsmCurrencySymbol(g_currency));
        else
            Logf("private_shops    service: type %-4d demand kind %-2d  %.2f %s/visit",
                 f.type, f.kind, EffectiveFee(f), TsmCurrencySymbol(g_currency));
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
