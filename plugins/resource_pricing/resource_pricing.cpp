// resource_pricing - the price and resource-lookup service.
//
// See resource_pricing_api.h for the contract. This file is the
// implementation: a linear resource-name scan, four direct reads of the
// game object's shop-good cache, and a price lookup with a two-level
// fallback.
//
// No hooks. No subscriptions. No state that survives across a world
// load. Every call re-reads what it needs from the game object or from
// the record it was handed, which is what makes the service immune to
// the one failure mode a cached price service has - a stale pointer
// from the previous world.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/persistence_api.h"
#include "../../src/resource_pricing_api.h"
#include "scarcity_curve.h"

#include <unordered_map>

// ---------------------------------------------------------------- config

#define MAX_FALLBACKS   64
#define FALLBACK_NAME   32

struct Fallback {
    char  name[FALLBACK_NAME];
    float price;      // RUB-denominated base (shipped table unit)
    float priceUsd;   // explicit USD override; < 0 when absent
};

static int    g_enabled      = 1;
static int    g_probe        = 1;
static float  g_defaultPrice = 500.0f;
static float  g_defaultPriceUsd = -1.0f; // explicit USD default; < 0 = auto

// v4: USD scale divisor for fallback values. USD numbers run far smaller
// than RUB (~1 USD = 20..60 RUB); the shipped fallback table is RUB, so
// USD fallback answers divide by this unless an explicit USD override
// (or [fallback_prices_usd] entry) exists.
static float g_usdDivisor = 50.0f;

static Fallback g_fallbacks[MAX_FALLBACKS];
static int      g_fallbackCount;

// One line per resource name that ends up on the default price, so a
// user who wonders why a resource is worth 500 without being in the
// ini has an answer. Latched per name so a per-frame caller does not
// fill the log.
#define MAX_WARNED 64
static char g_warned[MAX_WARNED][FALLBACK_NAME];
static int  g_warnedCount;

// --- v3: Task 7 absolute CPI survival basket -------------------------------
//
// [cpi_basket]: the tonnes/day each citizen's staple basket consumes,
// calibrated against the engine's own demand generator (see
// docs/CPI_Survival_Basket_Investigation.md S1/S2):
//   food     0.0020 t - midpoint of the 0.0015..0.0030 age ladder
//   meat     0.0006 t - the live probe value for the demand minted at
//                       RVA 0x83817A (v1.1.1.9)
//   clothes  0.0005 t - calibrated clothing wear
// default_daily_rent is RUB/day as written (0.33 = 10.0 RUB/month / 30,
// enterprise_base's residential_rent); USD mode divides it by
// usd_divisor at basket time. All four are INI-tunable.
static float g_cpiFoodTons    = 0.0020f;
static float g_cpiMeatTons    = 0.0006f;
static float g_cpiClothesTons = 0.0005f;
static float g_cpiDefaultRent = 0.33f;

// v4: local scarcity config.
static int   g_localScarcityEnabled = 0;
static float g_emptyMarkupMax       = 1.30f;
static float g_fullDiscountMin     = 0.82f;
static float g_neutralFillLow      = 0.35f;
static float g_neutralFillHigh     = 0.65f;
static float g_minCapacityTons     = 0.5f;

// ---------------------------------------------------------------- v2: market book
//
// The Republic-wide macro order book. Keyed on the engine's resource record
// pointer (stable within a session; a world load rebuilds the game's own
// resource vector, so OnMarketReset clears the maps). Supply and demand are
// raw tonnage accumulated between rebalances; the multiplier is the clamped
// Victoria-3 balance figure the pricing consumers apply on top of the base
// price. Everything is guarded by the plugin's own g_lock.

static const TsmSimClockApi* g_simClock = nullptr;
static const TsmPersistenceApi* g_persistence = nullptr;
static const TsmCurrencyApi* g_currency = nullptr;   // v3: CPI prices in the active currency
static int g_persHandle = -1;

static int    g_marketEnabled   = 1;      // [market_balance] dynamic_market_enabled
static float  g_marketMaxMult   = 2.00f;  // shortage ceiling  (+100%)
static float  g_marketMinMult   = 0.35f;  // glut floor        (-65%)
static float  g_marketGain      = 0.75f;  // dampening on the raw imbalance ratio
static TsmScheduleFrequency g_marketFreq = TSM_FREQ_MONTHLY;
static char   g_marketFreqStr[32] = "monthly";  // raw ini text, parsed in Start

struct MarketEntry {
    double supply = 0.0;
    double demand = 0.0;
    float  mult   = 1.0f;   // last computed multiplier, held until next rebalance
};

static std::unordered_map<void*, MarketEntry> g_market;

// Hard cap on tracked resources; the engine's resource vector is small
// (resource_pricing's own scan rejects > 256), so this never trips in play
// and only exists to bound a pathological pointer set.
#define MARKET_MAX_ENTRIES 512

static bool WarnOnce(const char* name)
{
    for (int i = 0; i < g_warnedCount; i++)
        if (strcmp(g_warned[i], name) == 0) return false;
    if (g_warnedCount >= MAX_WARNED) return false;
    strncpy_s(g_warned[g_warnedCount++], FALLBACK_NAME, name, _TRUNCATE);
    return true;
}

// ---------------------------------------------------------------- shop goods

static BYTE* GO()  { return TsmGameObject(g_exeBase); }

static void* GetShopGood(int which)
{
    return TsmShopGood(GO(), which);
}

static void* GetFoodImpl(void)       { return GetShopGood(0); }
static void* GetMeatImpl(void)       { return GetShopGood(1); }
static void* GetClothesImpl(void)    { return GetShopGood(2); }
static void* GetEletronicsImpl(void) { return GetShopGood(3); }

// ---------------------------------------------------------------- name lookup

// A record's name, without a virtual query - the caller has already
// validated the record pointer, and the name is inline at +0x00 with a
// guaranteed NUL within the 32-byte field. A byte-level comparison that
// walks no further than the name it is asked about.
static bool RecordHasName(const BYTE* rec, const char* name)
{
    for (int i = 0; i < 32; i++) {
        char c = (char)rec[i];
        if (c != name[i]) return false;
        if (!c) return true;
    }
    return false;
}

static void* GetByNameImpl(const char* name)
{
    if (!name || !name[0]) return nullptr;

    BYTE* game = GO();
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return nullptr;

    BYTE* vec   = game + TSM_GO_RES_VEC;
    if (!ReadablePtr(vec, 24)) return nullptr;

    BYTE* begin = *(BYTE**)vec;
    BYTE* end   = *(BYTE**)(vec + 8);
    if (!begin || !end || end <= begin) return nullptr;

    size_t span = (size_t)(end - begin);
    if (span % TSM_RES_STRIDE) return nullptr;
    size_t count = span / TSM_RES_STRIDE;
    if (count > 256) return nullptr;                 // implausible

    if (!ReadablePtr(begin, span)) return nullptr;

    for (size_t i = 0; i < count; i++) {
        BYTE* rec = begin + i * TSM_RES_STRIDE;
        if (RecordHasName(rec, name)) return rec;
    }
    return nullptr;
}

// ---------------------------------------------------------------- price lookup

static float LookupFallback(const char* name, int currency)
{
    for (int i = 0; i < g_fallbackCount; i++)
        if (_stricmp(g_fallbacks[i].name, name) == 0) {
            if (currency == TSM_CURRENCY_USD) {
                // Explicit USD override always wins.
                if (g_fallbacks[i].priceUsd > 0.0f)
                    return g_fallbacks[i].priceUsd;
                float d = (g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f;
                return g_fallbacks[i].price / d;
            }
            return g_fallbacks[i].price;
        }
    if (currency == TSM_CURRENCY_USD) {
        if (g_defaultPriceUsd > 0.0f) return g_defaultPriceUsd;
        float d = (g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f;
        return g_defaultPrice / d;
    }
    return g_defaultPrice;
}

// A price the engine computed. Rejected if it is zero, negative, or not
// finite - all three of which mean "the engine has nothing to say about
// this resource", not "this resource is worth nothing".
static bool LivePriceUsable(float v)
{
    if (!(v == v)) return false;                     // NaN
    if (v <= 0.01f) return false;
    if (v > 1.0e12f) return false;                   // Inf, or nonsense
    return true;
}

static float GetPriceImpl(void* resRecord, int currency)
{
    // v4: engine live fields are already native per currency
    // (+0x58 RUB / +0x5C USD) - only the fallback path needs scaling.
    if (currency != TSM_CURRENCY_USD && currency != TSM_CURRENCY_RUB)
        currency = TSM_CURRENCY_RUB;
    if (!TsmIsValidGamePtr(resRecord))
        return (currency == TSM_CURRENCY_USD)
            ? ((g_defaultPriceUsd > 0.0f) ? g_defaultPriceUsd
                : g_defaultPrice / ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f))
            : g_defaultPrice;
    if (!ReadablePtr(resRecord, TSM_RES_STRIDE))
        return (currency == TSM_CURRENCY_USD)
            ? ((g_defaultPriceUsd > 0.0f) ? g_defaultPriceUsd
                : g_defaultPrice / ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f))
            : g_defaultPrice;

    float live = TsmResourceLivePrice(resRecord, currency);
    if (LivePriceUsable(live)) return live;

    char name[FALLBACK_NAME] = {0};
    if (!TsmResourceName(resRecord, name, sizeof(name)))
        return (currency == TSM_CURRENCY_USD)
            ? ((g_defaultPriceUsd > 0.0f) ? g_defaultPriceUsd
                : g_defaultPrice / ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f))
            : g_defaultPrice;

    if (g_probe && WarnOnce(name)) {
        Logf("resource_pricing  \"%s\" has no live price; using the "
             "fallback table (see [fallback_prices] in "
             "plugins\\resource_pricing.ini)", name);
    }
    return LookupFallback(name, currency);
}

// ---------------------------------------------------------------- v2: market book

static void RecordSupplyImpl(void* resRecord, double tons)
{
    if (!g_marketEnabled || !TsmIsValidGamePtr(resRecord)) return;
    if (!(tons == tons) || tons <= 0.0) return;
    EnterCriticalSection(&g_lock);
    if (g_market.size() < MARKET_MAX_ENTRIES || g_market.count(resRecord))
        g_market[resRecord].supply += tons;
    LeaveCriticalSection(&g_lock);
}

static void RecordDemandImpl(void* resRecord, double tons)
{
    if (!g_marketEnabled || !TsmIsValidGamePtr(resRecord)) return;
    if (!(tons == tons) || tons <= 0.0) return;
    EnterCriticalSection(&g_lock);
    if (g_market.size() < MARKET_MAX_ENTRIES || g_market.count(resRecord))
        g_market[resRecord].demand += tons;
    LeaveCriticalSection(&g_lock);
}

static float GetMarketMultiplierImpl(void* resRecord)
{
    if (!g_marketEnabled || !TsmIsValidGamePtr(resRecord)) return 1.0f;
    EnterCriticalSection(&g_lock);
    auto it = g_market.find(resRecord);
    float m = (it != g_market.end()) ? it->second.mult : 1.0f;
    LeaveCriticalSection(&g_lock);
    return (m == m && m > 0.0f) ? m : 1.0f;
}

static void GetMarketBalanceImpl(void* resRecord, double* outSupply,
                                 double* outDemand, float* outMultiplier)
{
    double s = 0.0, d = 0.0;
    float  m = 1.0f;
    if (TsmIsValidGamePtr(resRecord)) {
        EnterCriticalSection(&g_lock);
        auto it = g_market.find(resRecord);
        if (it != g_market.end()) {
            s = it->second.supply;
            d = it->second.demand;
            m = it->second.mult;
        }
        LeaveCriticalSection(&g_lock);
    }
    if (outSupply)     *outSupply     = s;
    if (outDemand)     *outDemand     = d;
    if (outMultiplier) *outMultiplier = (m == m && m > 0.0f) ? m : 1.0f;
}

// Fold the accumulated supply/demand into a clamped multiplier per resource,
// then reset the accumulators for the next period. Victoria-3 balance:
//   ratio = (demand - supply) / max(1, min(demand, supply))
//   mult  = clamp(1 + gain * ratio, minMult, maxMult)
// A resource with no recorded flow keeps its prior multiplier (it did not
// change this period); a resource seen for the first time starts at 1.0.
static void RebalanceMarket(void)
{
    if (!g_marketEnabled) return;

    int repriced = 0;
    EnterCriticalSection(&g_lock);
    for (auto& kv : g_market) {
        MarketEntry& e = kv.second;
        if (e.supply <= 0.0 && e.demand <= 0.0) {
            // No flow this period: hold the last multiplier, accumulators
            // are already zero.
            continue;
        }
        double lo = (e.demand < e.supply) ? e.demand : e.supply;
        if (lo < 1.0) lo = 1.0;
        double ratio = (e.demand - e.supply) / lo;
        double m = 1.0 + (double)g_marketGain * ratio;
        if (m > (double)g_marketMaxMult) m = (double)g_marketMaxMult;
        if (m < (double)g_marketMinMult) m = (double)g_marketMinMult;
        e.mult   = (float)m;
        e.supply = 0.0;
        e.demand = 0.0;
        repriced++;
    }
    LeaveCriticalSection(&g_lock);

    if (g_probe && repriced > 0)
        Logf("resource_pricing  market rebalance: %d resource(s) repriced "
             "(clamp %.2f..%.2f, gain %.2f)", repriced,
             g_marketMinMult, g_marketMaxMult, g_marketGain);
}

static void OnMarketRebalance(void* /*ud*/, int /*day*/, int /*month*/,
                              int /*year*/)
{
    __try { RebalanceMarket(); }
    __except (FaultFilter("resource_pricing market rebalance",
                          GetExceptionInformation())) {
        Logf("resource_pricing  market rebalance faulted - skipped this period");
    }
}

static void OnMarketReset(void)
{
    EnterCriticalSection(&g_lock);
    g_market.clear();
    LeaveCriticalSection(&g_lock);
}

// The market book is keyed on the game's resource record pointers, which a
// world load rebuilds. Clear it on load so stale pointers from the previous
// world never accumulate toward the MARKET_MAX_ENTRIES budget. Nothing is
// persisted - the macro balance is a live, session-scoped figure.
static void OnSaveLoad(TsmSaveEvent ev, const char* /*dir*/, void* /*ud*/)
{
    if (ev == TSM_SAVE_LOADING) OnMarketReset();
}

// ---------------------------------------------------------------- v3: CPI basket

// Task 7 (absolute CPI): the daily cost of the staple survival basket in
// the ACTIVE currency, priced exactly the way retail charges are (live
// record price x market multiplier), plus rent:
//
//   CPI = (Pfood x multFood x Qfood) + (Pmeat x multMeat x Qmeat)
//       + (Pclothes x multClothes x Qclothes) + dailyRent
//
// Q* are the [cpi_basket] tonnes/day quantities (calibrated to the
// engine's own demand ladder - docs/CPI_Survival_Basket_Investigation.md
// S1/S2). injectedDailyRent > 0 replaces the [cpi_basket]
// default_daily_rent (0.33 RUB/day = 10.0/month / 30, USD-scaled in USD
// mode) so a Level 2 caller - taxes - can inject the true
// enterprise_base rent average instead of the neutral base.
//
// Pure query: no subscriptions, no state. The market multipliers read
// take the book's own lock inside GetMarketMultiplierImpl. POD-only so
// the __try frame stays C2712-clean (no locals with unwinding).
static float GetCpiDailyBasketImpl(float injectedDailyRent)
{
    int cur = TsmCurrencyActive(g_currency);

    float rent = injectedDailyRent;
    if (!(rent > 0.0f) || !(rent == rent)) {
        rent = g_cpiDefaultRent;
        if (cur == TSM_CURRENCY_USD) {
            float d = (g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f;
            rent /= d;
        }
    }

    float goods = -1.0f;                 // negative = fault sentinel
    __try {
        void* food    = GetFoodImpl();
        void* meat    = GetMeatImpl();
        void* clothes = GetClothesImpl();
        goods  = GetPriceImpl(food,    cur) *
                 GetMarketMultiplierImpl(food)    * g_cpiFoodTons;
        goods += GetPriceImpl(meat,    cur) *
                 GetMarketMultiplierImpl(meat)    * g_cpiMeatTons;
        goods += GetPriceImpl(clothes, cur) *
                 GetMarketMultiplierImpl(clothes) * g_cpiClothesTons;
    } __except (FaultFilter("resource_pricing cpi basket",
                            GetExceptionInformation())) {
        goods = -1.0f;
    }

    // A faulted or nonsense goods sum falls back to the documented
    // default basket (2.15 RUB = 0.70 food + 0.42 meat + 0.70 clothes +
    // 0.33 rent at shipped fallback prices), USD-scaled in USD mode.
    if (!(goods == goods) || goods < 0.0f) {
        float d = (cur == TSM_CURRENCY_USD)
                ? ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f) : 1.0f;
        return 2.15f / d;
    }

    float cpi = goods + rent;
    if (!(cpi == cpi) || !(cpi > 0.0f)) {
        float d = (cur == TSM_CURRENCY_USD)
                ? ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f) : 1.0f;
        return 2.15f / d;
    }
    return cpi;
}

// ---------------------------------------------------------------- v4: local scarcity
static float GetLocalScarcityMultiplierImpl(void* building, void* resRecord)
{
    if (!g_localScarcityEnabled) return 1.0f;
    if (!TsmCheckExeBuild(H, "resource_pricing")) return 1.0f;
    if (!TsmIsValidGamePtr(building) || !TsmIsValidGamePtr(resRecord)) return 1.0f;
    if (!TsmLooksLikeBuilding(building)) return 1.0f;
    if (!ReadablePtr(building, TSM_BLD_MIN_READABLE)) return 1.0f;
    if (!ReadablePtr(resRecord, TSM_RES_STRIDE)) return 1.0f;

    __try {
        BYTE* b = (BYTE*)building;
        BYTE* storBegin = *(BYTE**)(b + TSM_BLD_STORAGES_BEGIN);
        BYTE* storEnd   = *(BYTE**)(b + TSM_BLD_STORAGES_END);
        if (!storBegin || !storEnd || storEnd <= storBegin) return 1.0f;
        size_t storBytes = (size_t)(storEnd - storBegin);
        if (storBytes % TSM_STORAGE_STRIDE != 0) return 1.0f;
        size_t nstor = storBytes / TSM_STORAGE_STRIDE;
        if (nstor == 0 || nstor > 64) return 1.0f;
        if (!ReadablePtr(storBegin, storBytes)) return 1.0f;

        double totalCur = 0.0;
        double totalCap = 0.0;
        bool found = false;
        for (size_t i = 0; i < nstor; i++) {
            BYTE* sp = storBegin + i * TSM_STORAGE_STRIDE;
            if (!ReadablePtr(sp, TSM_STORAGE_STRIDE)) continue;
            // slots
            BYTE* slotBegin = *(BYTE**)(sp + TSM_ST_SLOTS_BEGIN);
            BYTE* slotEnd   = *(BYTE**)(sp + TSM_ST_SLOTS_END);
            if (!slotBegin || !slotEnd || slotEnd <= slotBegin) continue;
            size_t slotBytes = (size_t)(slotEnd - slotBegin);
            if (slotBytes % TSM_SLOT_STRIDE != 0) continue;
            size_t nslot = slotBytes / TSM_SLOT_STRIDE;
            if (nslot == 0 || nslot > 64) continue;
            if (!ReadablePtr(slotBegin, slotBytes)) continue;
            float cap = *(float*)(sp + TSM_ST_CAPACITY);
            if (!(cap == cap) || cap <= 0.0f) continue;
            if (cap < g_minCapacityTons) continue; // ignore tiny
            bool storageMatched = false;
            for (size_t j = 0; j < nslot; j++) {
                // slot stride 0x10: resource ptr +0, content +8
                BYTE* slot = slotBegin + j * TSM_SLOT_STRIDE;
                if (!ReadablePtr(slot, TSM_SLOT_STRIDE)) continue;
                void* r = *(void**)(slot + TSM_SLOT_RESOURCE);
                if (r != resRecord) continue;
                float content = *(float*)(slot + TSM_SLOT_CONTENT);
                if (!(content == content)) content = 0.0f;
                if (content < 0.0f) content = 0.0f;
                totalCur += (double)content;
                storageMatched = true;
            }
            if (storageMatched) {
                totalCap += (double)cap;
                found = true;
            }
        }
        if (!found || totalCap <= 0.0) return 1.0f;
        float fill = (float)(totalCur / totalCap);
        ScarcityParams p;
        p.empty_markup_max  = g_emptyMarkupMax;
        p.full_discount_min = g_fullDiscountMin;
        p.neutral_fill_low   = g_neutralFillLow;
        p.neutral_fill_high  = g_neutralFillHigh;
        p.min_capacity_tons  = g_minCapacityTons;
        return scarcityMultiplier(fill, p);
    } __except (FaultFilter("resource_pricing local scarcity",
                            GetExceptionInformation())) {
        return 1.0f;
    }
}

static float GetLocalPriceQuoteImpl(void* building, void* resRecord, int currency)
{
    if (currency != TSM_CURRENCY_USD && currency != TSM_CURRENCY_RUB)
        currency = TSM_CURRENCY_RUB;
    float base = GetPriceImpl(resRecord, currency);
    float mkt = GetMarketMultiplierImpl(resRecord);
    if (!(mkt == mkt) || mkt <= 0.0f) mkt = 1.0f;
    float m = GetLocalScarcityMultiplierImpl(building, resRecord);
    if (!(m == m) || m <= 0.0f) m = 1.0f;
    float q = base * mkt * m;
    if (!(q == q) || q <= 0.0f) q = base;
    return q;
}

static const TsmResourcePricingApi kApi = {
    sizeof(TsmResourcePricingApi),
    GetFoodImpl,
    GetMeatImpl,
    GetClothesImpl,
    GetEletronicsImpl,
    GetByNameImpl,
    GetPriceImpl,
    RecordSupplyImpl,
    RecordDemandImpl,
    GetMarketMultiplierImpl,
    GetMarketBalanceImpl,
    GetCpiDailyBasketImpl,
    GetLocalScarcityMultiplierImpl,
    GetLocalPriceQuoteImpl,
};

// ---------------------------------------------------------------- settings

// [fallback_prices] is a flat list of `name = price` under one section.
// The profile API cannot enumerate a section's keys, so the section is
// read whole with GetPrivateProfileSectionA - the same call
// plugins/resources uses for its own multi-line sections - and each
// `name=value\0` pair is parsed by hand.
// v4: [fallback_prices_usd] carries optional explicit USD overrides,
// written into priceUsd so LookupFallback can honour them verbatim.
static void LoadFallbacks(const char* iniPath)
{
    char buf[2048];
    DWORD got = GetPrivateProfileSectionA("fallback_prices", buf,
                                          sizeof(buf), iniPath);
    if (got != 0 && got < sizeof(buf) - 2) {
        for (const char* p = buf; *p; p += strlen(p) + 1) {
            const char* eq = strchr(p, '=');
            if (!eq) continue;

            char name[FALLBACK_NAME] = {0};
            size_t nlen = (size_t)(eq - p);
            if (nlen == 0 || nlen >= sizeof(name)) continue;
            memcpy(name, p, nlen);
            name[nlen] = 0;
            Trim(name);

            float price = (float)atof(eq + 1);
            if (!(price > 0.0f) || !(price == price)) continue;

            if (g_fallbackCount >= MAX_FALLBACKS) {
                Logf("resource_pricing  fallback table full (%d) - "
                     "\"%s\" ignored", MAX_FALLBACKS, name);
                continue;
            }

            Fallback* f = &g_fallbacks[g_fallbackCount++];
            strncpy_s(f->name, sizeof(f->name), name, _TRUNCATE);
            f->price = price;
            f->priceUsd = -1.0f;
        }
    }

    // Optional USD overrides: same names, USD per ton.
    got = GetPrivateProfileSectionA("fallback_prices_usd", buf,
                                    sizeof(buf), iniPath);
    if (got == 0 || got >= sizeof(buf) - 2) return;
    for (const char* p = buf; *p; p += strlen(p) + 1) {
        const char* eq = strchr(p, '=');
        if (!eq) continue;

        char name[FALLBACK_NAME] = {0};
        size_t nlen = (size_t)(eq - p);
        if (nlen == 0 || nlen >= sizeof(name)) continue;
        memcpy(name, p, nlen);
        name[nlen] = 0;
        Trim(name);

        float price = (float)atof(eq + 1);
        if (!(price > 0.0f) || !(price == price)) continue;

        for (int i = 0; i < g_fallbackCount; i++) {
            if (_stricmp(g_fallbacks[i].name, name) == 0) {
                g_fallbacks[i].priceUsd = price;
                break;
            }
        }
    }
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\resource_pricing.ini";
    char v[32];

    g_enabled = H->configInt(ini, "resource_pricing", "enabled", 1);
    g_probe   = H->configInt(ini, "resource_pricing", "probe",   1);

    if (H->configString(ini, "resource_pricing", "default",
                        v, sizeof(v), "500.0") && v[0]) {
        float d = (float)atof(v);
        if (d > 0.0f && d == d) g_defaultPrice = d;
    }
    // v4 USD scale: explicit USD default wins; otherwise USD fallbacks
    // divide the RUB figures by usd_divisor (default 50).
    if (H->configString(ini, "resource_pricing", "default_usd",
                        v, sizeof(v), "") && v[0]) {
        float d = (float)atof(v);
        if (d > 0.0f && d == d) g_defaultPriceUsd = d;
    }
    if (H->configString(ini, "resource_pricing", "usd_divisor",
                        v, sizeof(v), "") && v[0]) {
        float d = (float)atof(v);
        if (d >= 1.0f && d <= 1000.0f && d == d) g_usdDivisor = d;
    }

    // v3 (Task 7): the absolute CPI survival basket. [cpi_basket] carries
    // the tonnes/day per staple plus the neutral default daily rent;
    // every value falls back to the calibrated default when absent or
    // nonsensical (see docs/CPI_Survival_Basket_Investigation.md S1/S2).
    g_cpiFoodTons = 0.0020f;
    if (H->configString(ini, "cpi_basket", "basket_food_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_cpiFoodTons = f;
    }
    g_cpiMeatTons = 0.0006f;
    if (H->configString(ini, "cpi_basket", "basket_meat_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_cpiMeatTons = f;
    }
    g_cpiClothesTons = 0.0005f;
    if (H->configString(ini, "cpi_basket", "basket_clothes_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_cpiClothesTons = f;
    }
    g_cpiDefaultRent = 0.33f;
    if (H->configString(ini, "cpi_basket", "default_daily_rent",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10000.0f && f == f) g_cpiDefaultRent = f;
    }

    // v2: the Republic macro market order book. See [market_balance].
    g_marketEnabled = H->configInt(ini, "market_balance",
                                   "dynamic_market_enabled", 1);
    if (H->configString(ini, "market_balance", "max_shortage_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 10.0f && f == f) g_marketMaxMult = f;
    }
    if (H->configString(ini, "market_balance", "min_glut_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_marketMinMult = f;
    }
    if (H->configString(ini, "market_balance", "market_gain",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 5.0f && f == f) g_marketGain = f;
    }
    if (H->configString(ini, "market_balance", "market_rebalance_freq",
                        v, sizeof(v), "") && v[0]) {
        // Parsed lazily in Start against sim_clock; "monthly"/"weekly"/...
        strncpy_s(g_marketFreqStr, sizeof(g_marketFreqStr), v, _TRUNCATE);
    }
    // Keep the clamp sane even if both ends were pushed past each other.
    if (g_marketMinMult > g_marketMaxMult) g_marketMinMult = g_marketMaxMult;

    // v4: local per-building scarcity quote spreads.
    g_localScarcityEnabled = H->configInt(ini, "local_scarcity", "enabled", 0);
    if (H->configString(ini, "local_scarcity", "empty_markup_max",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 10.0f && f == f) g_emptyMarkupMax = f;
    }
    if (H->configString(ini, "local_scarcity", "full_discount_min",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_fullDiscountMin = f;
    }
    if (H->configString(ini, "local_scarcity", "neutral_fill_low",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_neutralFillLow = f;
    }
    if (H->configString(ini, "local_scarcity", "neutral_fill_high",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_neutralFillHigh = f;
    }
    if (H->configString(ini, "local_scarcity", "min_capacity_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 100000.0f && f == f) g_minCapacityTons = f;
    }
    if (g_neutralFillLow > g_neutralFillHigh) {
        float t = g_neutralFillLow; g_neutralFillLow = g_neutralFillHigh; g_neutralFillHigh = t;
    }
    if (g_fullDiscountMin > g_emptyMarkupMax) g_fullDiscountMin = g_emptyMarkupMax;

    // GetPrivateProfileSectionA needs the full path; the profile API in
    // TsmHost resolves a bare name against baseDir, but the section
    // enumerator does not. Build the path the same way the host would.
    char iniPath[MAX_PATH];
    _snprintf_s(iniPath, sizeof(iniPath), _TRUNCATE,
                "%s\\plugins\\resource_pricing.ini", g_baseDir);
    LoadFallbacks(iniPath);
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
    info->name    = "resource_pricing";
    info->version = "4.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("resource_pricing  disabled");
        return 1;
    }

    // v4 ABI: publish FOUR views: v1, v2, v3, v4.
    static TsmResourcePricingApi kApiV1;
    memcpy(&kApiV1, &kApi, sizeof(kApiV1));
    kApiV1.structSize = (unsigned)offsetof(TsmResourcePricingApi, recordSupply);
    H->provide(TSM_SERVICE_RESOURCE_PRICING,
               TSM_RESOURCE_PRICING_VERSION_V1, &kApiV1);

    static TsmResourcePricingApi kApiV2;
    memcpy(&kApiV2, &kApi, sizeof(kApiV2));
    kApiV2.structSize = (unsigned)offsetof(TsmResourcePricingApi,
                                           getCpiDailyBasket);
    H->provide(TSM_SERVICE_RESOURCE_PRICING,
               TSM_RESOURCE_PRICING_VERSION_V2, &kApiV2);

    static TsmResourcePricingApi kApiV3;
    memcpy(&kApiV3, &kApi, sizeof(kApiV3));
    kApiV3.structSize = (unsigned)offsetof(TsmResourcePricingApi,
                                           getLocalScarcityMultiplier);
    H->provide(TSM_SERVICE_RESOURCE_PRICING,
               TSM_RESOURCE_PRICING_VERSION_V3, &kApiV3);

    H->provide(TSM_SERVICE_RESOURCE_PRICING,
               TSM_RESOURCE_PRICING_VERSION, &kApi);
    return 0;
}

// Start is where a service subscribes to a Level 0 service. The price and
// lookup core stays a pure query service, but the v2 market book needs the
// calendar to rebalance on. sim_clock is the only dependency, and it is a
// Level 0 service, so this introduces no layering inversion. The market
// book is in-memory only and keyed on the game's resource record pointers,
// which a world load rebuilds - so it is reset when a world appears.
extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    // v3 (Task 7): the CPI basket prices in the ACTIVE currency. currency
    // is a Level 0 service, so this adds no layering inversion (the
    // header's "no runtime dependency" note predates v3). Absent it,
    // TsmCurrencyActive(null) answers RUB and the basket still resolves -
    // graceful degradation, not a load failure.
    g_currency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency)
        Logf("resource_pricing  WARN  currency not installed - the CPI "
             "basket prices in RUB regardless of primary_currency");

    if (g_marketEnabled) {
        g_simClock = (const TsmSimClockApi*)
            H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
        if (g_simClock) {
            g_marketFreq = g_simClock->parseFrequency(g_marketFreqStr,
                                                      TSM_FREQ_MONTHLY);
            TsmSimClockRegisterScheduledPhase(g_simClock, g_marketFreq,
                                              TSM_PHASE_PRE_FISCAL,
                                              OnMarketRebalance, nullptr);
        } else {
            // No calendar: the book still records, it just never rebalances,
            // so the multiplier stays at its last (or neutral 1.0) value.
            Logf("resource_pricing  WARN  sim_clock not installed - market "
                 "book will not rebalance this session");
        }

        // Clear the pointer-keyed book when a world loads.
        g_persistence = (const TsmPersistenceApi*)
            H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
        if (g_persistence)
            g_persHandle = g_persistence->registerHandler(
                "resource_pricing", OnSaveLoad, nullptr);
    }

    Logf("resource_pricing  live: default %.2f, %d fallback price(s), "
         "market %s (clamp %.2f..%.2f, gain %.2f)",
         g_defaultPrice, g_fallbackCount,
         g_marketEnabled ? "on" : "off",
         g_marketMinMult, g_marketMaxMult, g_marketGain);
    // v3 (Task 7): the CPI survival basket, priced in the active currency
    // with the neutral default rent (injectedDailyRent = 0).
    {
        int cur = TsmCurrencyActive(g_currency);
        float d = (cur == TSM_CURRENCY_USD)
                ? ((g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f) : 1.0f;
        Logf("resource_pricing    cpi basket v3: food %.4f t, meat %.4f t, "
             "clothes %.4f t, rent %.3f %s/day -> %.3f %s/day",
             g_cpiFoodTons, g_cpiMeatTons, g_cpiClothesTons,
             g_cpiDefaultRent / d, TsmCurrencySymbol(g_currency),
             (double)GetCpiDailyBasketImpl(0.0f),
             TsmCurrencySymbol(g_currency));
    }
    for (int i = 0; i < g_fallbackCount; i++)
        Logf("resource_pricing    %-20s = %.2f",
             g_fallbacks[i].name, g_fallbacks[i].price);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }