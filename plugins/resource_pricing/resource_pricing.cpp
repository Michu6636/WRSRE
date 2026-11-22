// resource_pricing - the price and resource-lookup service.
//
// See resource_pricing_api.h for the contract. This file is the
// implementation: a linear resource-name scan, four direct reads of the
// game object's shop-good cache, a native-price lookup, and the CPI basket.
//
// v6 (2026-11-22 economy direction step 2, D8/D32/D33/D34/D35): the fallback
// price table, the `default` price and every usd divisor are gone. A resource
// with no native price is UNPRICED and answers 0.0f. The CPI basket answers
// 0.0f when food, meat or clothes cannot be priced, and has separate RUB/USD
// default rents instead of one divided number. Nothing here invents a number.
//
// No hooks. No subscriptions. No state that survives across a world
// load. Every call re-reads what it needs from the game object or from
// the record it was handed, which is what makes the service immune to
// the one failure mode a cached price service has - a stale pointer
// from the previous world.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/price_guards.h"
#include "../../src/resource_pricing_api.h"
#include "cpi_rent.h"

// ---------------------------------------------------------------- config

#define PRICE_NAME_LEN 32

static int   g_enabled = 1;

// --- v6: UNPRICED logging --------------------------------------------------
//
// One ERROR line per resource and currency for the rest of the session. A
// per-frame caller therefore reports the missing price without filling the
// log; the resource is genuinely unpriced until something produces it.
#define MAX_WARNED 64
static char g_warned[MAX_WARNED][PRICE_NAME_LEN + 8];
static int  g_warnedCount;

static bool WarnOnce(const char* name, int currency)
{
    char key[PRICE_NAME_LEN + 8] = {0};
    _snprintf_s(key, sizeof(key), _TRUNCATE, "%s|%s", name,
                (currency == TSM_CURRENCY_USD) ? "USD" : "RUB");
    for (int i = 0; i < g_warnedCount; i++)
        if (strcmp(g_warned[i], key) == 0) return false;
    if (g_warnedCount >= MAX_WARNED) return false;
    strncpy_s(g_warned[g_warnedCount++], sizeof(g_warned[0]), key, _TRUNCATE);
    return true;
}

// --- v6: CPI survival basket -----------------------------------------------
//
// [cpi_basket]: the tonnes/day each citizen's staple basket consumes,
// calibrated against the engine's own demand generator (see
// docs/CPI_Survival_Basket_Investigation.md S1/S2):
//   food     0.0020 t - midpoint of the 0.0015..0.0030 age ladder
//   meat     0.0006 t - the live probe value for the demand minted at
//                       RVA 0x83817A (v1.1.1.9)
//   clothes  0.0005 t - calibrated clothing wear
// The rent term is TWO documented INI constants, never one number divided by
// a rate (D35):
//   default_daily_rent      RUB/day (shipped 0.33 = 10.0 RUB/month / 30)
//   default_daily_rent_usd  USD/day (shipped 0.33, the same size as RUB;
//                           native USD prices are usually 10-30% higher)
// Both keys are REQUIRED. There is no compiled-in default: a missing key, 0,
// a negative value, NaN or an implausible value makes the basket UNAVAILABLE
// in EITHER currency, and the log names the offending key. Neither key is
// ever replaced by the other or by a divisor. Missing goods prices do the same.
static float g_cpiFoodTons       = 0.0020f;
static float g_cpiMeatTons       = 0.0006f;
static float g_cpiClothesTons    = 0.0005f;
static float g_cpiDefaultRent    = 0.0f;   // no compiled-in default
static float g_cpiDefaultRentUsd = 0.0f;   // no compiled-in default

static bool        g_cpiWarned        = false;
static const char* g_cpiRentWarnedKey = nullptr;

static const TsmCurrencyApi* g_currency = nullptr;   // CPI prices in the active currency

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

static float GetPriceImpl(void* resRecord, int currency)
{
    if (currency != TSM_CURRENCY_USD && currency != TSM_CURRENCY_RUB)
        currency = TSM_CURRENCY_RUB;

    // D8: a missing record is UNPRICED. It is never a default price.
    if (!TsmIsValidGamePtr(resRecord)) return 0.0f;
    if (!ReadablePtr(resRecord, TSM_RES_STRIDE)) return 0.0f;

    const float live = TsmResourceLivePrice(resRecord, currency);
    if (TsmNativePriceUsable(live)) return live;

    // The engine has no price. Say so, once per resource and currency.
    char name[PRICE_NAME_LEN] = {0};
    if (!TsmResourceName(resRecord, name, sizeof(name)) || !name[0])
        _snprintf_s(name, sizeof(name), _TRUNCATE, "?");
    if (WarnOnce(name, currency)) {
        Logf("resource_pricing  ERROR UNPRICED  \"%s\" has no native %s price; "
             "callers must refuse the charge, never use a fallback",
             name, (currency == TSM_CURRENCY_USD) ? "USD" : "RUB");
    }
    return 0.0f;
}

// ---------------------------------------------------------------- CPI basket

// The daily cost of the staple survival basket in the ACTIVE currency,
// priced exactly the way retail charges are (the native record price, no
// multiplier), plus rent:
//
//   CPI = (Pfood x Qfood) + (Pmeat x Qmeat) + (Pclothes x Qclothes)
//       + dailyRent
//
// Q* are the [cpi_basket] tonnes/day quantities (calibrated to the
// engine's own demand ladder - docs/CPI_Survival_Basket_Investigation.md
// S1/S2). injectedDailyRent > 0 replaces the active currency's required
// default (default_daily_rent or default_daily_rent_usd, D35) so a Level 2
// caller - taxes - can inject the true enterprise_base rent average instead.
//
// Returns 0.0f when the basket is UNAVAILABLE (D34): no world, a fault, any
// of the three goods without a native price, or a missing/0/negative/NaN rent
// key. It never returns a default basket. Pure query: no subscriptions, no
// state. POD-only so the __try frame stays C2712-clean (no locals with
// unwinding).
static float GetCpiDailyBasketImpl(float injectedDailyRent)
{
    const int cur = TsmCurrencyActive(g_currency);

    // D35: injected rent wins; otherwise the active currency's own REQUIRED
    // key. A missing/invalid key is UNAVAILABLE in either currency, and the
    // log names that key - never the other currency's number, never 0.
    const wrsre::pricing::CpiRent rentChoice =
        wrsre::pricing::ChooseCpiRent(cur, injectedDailyRent,
                                      g_cpiDefaultRent, g_cpiDefaultRentUsd);
    if (!rentChoice.available) {
        if (rentChoice.missingKey &&
            (!g_cpiRentWarnedKey ||
             strcmp(g_cpiRentWarnedKey, rentChoice.missingKey) != 0)) {
            g_cpiRentWarnedKey = rentChoice.missingKey;
            Logf("resource_pricing  ERROR CPI UNAVAILABLE  \"%s\" is missing or "
                 "invalid (0, negative, NaN or implausible); the %s basket has "
                 "no rent and is UNAVAILABLE (D34/D35)",
                 rentChoice.missingKey,
                 (cur == TSM_CURRENCY_USD) ? "USD" : "RUB");
        }
        return 0.0f;
    }
    const float rent = rentChoice.value;

    float goods = -1.0f;                 // negative = fault sentinel
    __try {
        void* food    = GetFoodImpl();
        void* meat    = GetMeatImpl();
        void* clothes = GetClothesImpl();
        const float pFood    = GetPriceImpl(food,    cur);
        const float pMeat    = GetPriceImpl(meat,    cur);
        const float pClothes = GetPriceImpl(clothes, cur);
        if (TsmNativePriceUsable(pFood) &&
            TsmNativePriceUsable(pMeat) &&
            TsmNativePriceUsable(pClothes)) {
            goods = pFood    * g_cpiFoodTons
                  + pMeat    * g_cpiMeatTons
                  + pClothes * g_cpiClothesTons;
        }
    } __except (FaultFilter("resource_pricing cpi basket",
                            GetExceptionInformation())) {
        goods = -1.0f;
    }

    const bool available = (goods == goods) && goods >= 0.0f;
    if (!available) {
        if (!g_cpiWarned) {
            g_cpiWarned = true;
            Logf("resource_pricing  ERROR CPI UNAVAILABLE  food/meat/clothes "
                 "has no native price; the CPI basket has no value today");
        }
        return 0.0f;
    }

    g_cpiWarned = false;
    const float cpi = goods + rent;
    if (!(cpi == cpi) || !(cpi > 0.0f)) return 0.0f;
    return cpi;
}

static const TsmResourcePricingApi kApi = {
    sizeof(TsmResourcePricingApi),
    GetFoodImpl,
    GetMeatImpl,
    GetClothesImpl,
    GetEletronicsImpl,
    GetByNameImpl,
    GetPriceImpl,
    GetCpiDailyBasketImpl,
};

// ---------------------------------------------------------------- settings

static void ReadSettings(void)
{
    const char* ini = "plugins\\resource_pricing.ini";
    char v[32];

    g_enabled = H->configInt(ini, "resource_pricing", "enabled", 1);

    // v6 (Task 7): the absolute CPI survival basket. [cpi_basket] carries
    // the tonnes/day per staple plus the neutral default daily rent;
    // every value falls back to the calibrated default when absent or
    // nonsensical (see docs/CPI_Survival_Basket_Investigation.md S1/S2).
    // These are documented quantities, not prices.
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
    // D35: BOTH keys are required and behave the same way. No compiled-in
    // default: a missing key, 0, a negative value, NaN or an implausible
    // value leaves the field at 0.0f, and ChooseCpiRent makes the basket
    // UNAVAILABLE with one log line naming that key.
    g_cpiDefaultRent = 0.0f;
    if (H->configString(ini, "cpi_basket", "default_daily_rent",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 10000.0f && f == f) g_cpiDefaultRent = f;
    }
    g_cpiDefaultRentUsd = 0.0f;
    if (H->configString(ini, "cpi_basket", "default_daily_rent_usd",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 10000.0f && f == f) g_cpiDefaultRentUsd = f;
    }
    g_cpiRentWarnedKey = nullptr;
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
    info->version = "6.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("resource_pricing  disabled");
        return 1;
    }

    // v6 ABI: one full view. The v1 prefix is deliberately NOT published: its
    // old contract promised a non-zero price, so a pre-step-2 consumer would
    // read UNPRICED 0.0f as "free" and move no money. Such a consumer now
    // fails to resolve the service and must degrade as it already does when
    // resource_pricing is absent. The removed v2/v3/v4/v5 layouts are not
    // published either.
    H->provide(TSM_SERVICE_RESOURCE_PRICING,
               TSM_RESOURCE_PRICING_VERSION, &kApi);
    return 0;
}

// Start consumes currency (Level 0) so the CPI basket prices in the active
// currency. No calendar or persistence subscription: since v5 there is no
// market book to rebalance or reset.
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

    Logf("resource_pricing  live v6: native prices only; a missing price is "
         "UNPRICED (no fallback table, no default)");

    // v3 (Task 7): the CPI survival basket, priced in the active currency
    // with that currency's documented default rent (injectedDailyRent = 0).
    // This line is informational: a world may not be loaded yet, in which
    // case the basket is reported UNAVAILABLE and no number is invented.
    {
        const int cur = TsmCurrencyActive(g_currency);
        const wrsre::pricing::CpiRent rentChoice =
            wrsre::pricing::ChooseCpiRent(cur, 0.0f,
                                          g_cpiDefaultRent, g_cpiDefaultRentUsd);
        const float cpi = GetCpiDailyBasketImpl(0.0f);
        if (cpi > 0.0f) {
            Logf("resource_pricing    cpi basket: food %.4f t, meat %.4f t, "
                 "clothes %.4f t, rent %.3f %s/day -> %.3f %s/day",
                 g_cpiFoodTons, g_cpiMeatTons, g_cpiClothesTons,
                 (double)rentChoice.value, TsmCurrencySymbol(g_currency),
                 (double)cpi, TsmCurrencySymbol(g_currency));
        } else {
            Logf("resource_pricing    cpi basket: UNAVAILABLE (no world/prices "
                 "yet; no default basket)");
        }
        (void)cur;
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
