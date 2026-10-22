// resource_pricing_api.h - the price contract.
//
// resource_pricing is a Level 1 service. It depends on Level 0 only (in
// practice: game_world.h for offsets, and currency_api.h for the two
// currency codes, neither of which is a runtime dependency).
//
// It does three things:
//
//   - it caches nothing. The four shop-good record pointers and every
//     price are read fresh from the game object or from the record, so a
//     world load is invisible to this service.
//
//   - it owns the answer to "what is one ton of this worth in roubles".
//     The engine keeps a per-resource price field (+0x58 RUB, +0x5C USD)
//     and recomputes it on its own schedule. That field is the truth.
//     For a resource the engine prices at zero - a mod resource that no
//     building type produces - the service falls back to a table in its
//     .ini, and then to a default.
//
//   - it looks up a resource record by name. The engine has its own
//     resolver, but it is hooked by plugins/resources and a Level 1
//     service may not depend on a higher one. So resource_pricing scans
//     the vector itself, which finds both base-game and mod resources,
//     and does not care what other plugins are installed.
//
// THE PRICE FALLBACK TABLE, AND WHY IT IS NOT A PRICE OVERRIDE
//
// The engine's own price recompute overwrites +0x58 and +0x5C for every
// resource it knows how to compute. To force a price that survives that
// recompute, a user writes [price] in plugins/resources.ini, and the
// resources plugin re-applies it after every recompute. That is the
// override mechanism, and it lives where the resource is declared.
//
// This service's fallback table is for the other case: a resource the
// engine prices at zero because nothing produces it. The engine will
// happily leave +0x58 at 0.0 forever for such a resource, and a shop
// that tries to sell it charges 0.00. The table lets a user give it a
// value without needing a production chain to be built first.
//
// The two are complementary, not redundant:
//
//   [price] in resources.ini    a resource that IS produced, wanting a
//                               different number than the engine computes
//   [fallback_prices] here      a resource that is NOT produced, so the
//                               engine has nothing to compute from

#ifndef TSM_RESOURCE_PRICING_API_H
#define TSM_RESOURCE_PRICING_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "currency_api.h"      // TSM_CURRENCY_RUB, TSM_CURRENCY_USD

#define TSM_SERVICE_RESOURCE_PRICING "resource_pricing"
// v2 appends the Republic macro market order book (Victoria-3 style
// supply/demand balance). The registry matches versions EXACTLY, so the
// provider publishes BOTH the v1 view (prefix only, for any consumer built
// against the old header - e.g. private_construction) and the v2 view
// (full struct). This mirrors the currency (v3/v4) and citizen_labor
// (v2/v3) dual-provide precedent. build.bat rebuilds every consumer with
// this header, so an in-tree consumer asking for v2 always resolves.
//
// v3 appends getCpiDailyBasket (Task 7, absolute CPI survival basket).
// Same dual-provide pattern: the provider publishes v1, v2 AND v3 views.
// A v2 consumer asking for v2 reads exactly the prefix it expects;
// a v3 consumer asks for v3 and gets the CPI tail. Purely additive:
// no existing field moved, no existing meaning changed.
#define TSM_RESOURCE_PRICING_VERSION    3u
#define TSM_RESOURCE_PRICING_VERSION_V2 2u
#define TSM_RESOURCE_PRICING_VERSION_V1 1u

typedef struct TsmResourcePricingApi {
    unsigned structSize;

    // --- the four shop goods -----------------------------------------------
    //
    // Direct reads of the game object's own cached records, so a caller
    // that wants "the food record" does not have to scan the resource
    // vector or remember the offset. Returns NULL before a world is
    // loaded.

    void* (*getFood)(void);
    void* (*getMeat)(void);
    void* (*getClothes)(void);
    void* (*getEletronics)(void);

    // --- name lookup -------------------------------------------------------
    //
    // A linear scan of the engine's resource vector, on the record's own
    // inline name. Case-sensitive, exact-match, the same rule the engine's
    // resolver uses. Finds a mod resource published by plugins/resources
    // if it is installed, and a base-game resource regardless.
    //
    // Returns NULL if the name is not a resource. No allocation, no
    // caching - the scan is 60 to 80 short string comparisons, which at
    // the frame rates this runs at costs nothing.

    void* (*getByName)(const char* name);

    // --- prices ------------------------------------------------------------
    //
    // The live price of one ton of the given resource, in the requested
    // currency. The lookup order is:
    //
    //   1. the engine's own +0x58 / +0x5C field, if it is a finite value
    //      greater than 0.01;
    //   2. the [fallback_prices] table in plugins/resource_pricing.ini,
    //      keyed on the record's name;
    //   3. the `default` from that file.
    //
    // Never returns a negative number, zero, NaN, or infinity. A caller
    // that needs to distinguish "the engine priced this" from "the
    // service supplied a fallback" reads the record's price field
    // directly through TsmResourceLivePrice.
    //
    // A NULL record is legal and returns the default.

    float (*getPrice)(void* resRecord, int currency);

    // --- v2: Republic macro market order book -----------------------------
    //
    // A Victoria-3-style supply/demand balance, aggregated Republic-wide
    // per resource. Producers call recordSupply when finished goods leave
    // an export storage or cross the border; consumers call recordDemand
    // for citizen retail wants (serviced or turned away) and factory input
    // draws. At each market_rebalance_freq boundary the service folds the
    // accumulated totals into a clamped multiplier that getMarketMultiplier
    // returns until the next boundary, then resets the accumulators.
    //
    // All four are safe from any thread and take the register's own lock.
    // A null resRecord is ignored (record*) or returns the neutral 1.0
    // multiplier / zero balance (get*).

    // Add `tons` of sell-order volume for a resource (>= 0).
    void   (*recordSupply)(void* resRecord, double tons);
    // Add `tons` of buy-order volume for a resource (>= 0).
    void   (*recordDemand)(void* resRecord, double tons);

    // The current market multiplier for a resource, in
    // [min_glut_mult, max_shortage_mult]. 1.0 when the book is balanced,
    // the market is disabled, or the record is unknown.
    float  (*getMarketMultiplier)(void* resRecord);

    // The raw accumulated supply/demand for the current period and the
    // multiplier last computed for this resource. Any out pointer may be
    // null. Used by the in-window "Market Condition" row and probes.
    void   (*getMarketBalance)(void* resRecord, double* outSupply,
                               double* outDemand, float* outMultiplier);

    // --- v3: Task 7 absolute CPI survival basket ---------------------------
    //
    // Daily Basket Cost (CPI), in the ACTIVE currency units the provider
    // resolves through TsmCurrencyActive (same convention as getPrice):
    //
    //   CPI = (Pfood x multFood x Qfood)
    //       + (Pmeat x multMeat x Qmeat)
    //       + (Pclothes x multClothes x Qclothes)
    //       + dailyRent
    //
    // P* are the live getPrice answers for the provider's own
    // getFood/getMeat/getClothes records; mult* are the live market-book
    // multipliers (neutral 1.0 when the book is absent or disabled);
    // Q* are the [cpi_basket] quantities (tons/day); dailyRent is the
    // injected per-day rent average, or the [cpi_basket] default_daily_rent
    // fallback (USD-scaled in USD mode) when injectedDailyRent <= 0.
    //
    // Pure query: reads records and config, takes the register's own lock
    // where the market book needs it. Safe from any thread. Always
    // returns a finite value > 0 (falls back to the configured default
    // basket, ~2.15 RUB/day at shipped fallbacks, when no world/prices
    // are readable).
    float (*getCpiDailyBasket)(float injectedDailyRent);
} TsmResourcePricingApi;

// --- v2 consumer guard ------------------------------------------------------
//
// The registry matches versions exactly, so a v2 consumer asks for v2 and a
// v1 consumer keeps asking for v1. These helpers let one binary tolerate
// whichever iface it was handed: they check structSize before touching the
// appended market-book tail and fall back to neutral behaviour (multiplier
// 1.0, no recording) when the provider is a v1 build.
static inline int TsmPricingHasMarket(const TsmResourcePricingApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmResourcePricingApi, getMarketBalance) +
                              sizeof(api->getMarketBalance) &&
        api->recordSupply && api->recordDemand &&
        api->getMarketMultiplier && api->getMarketBalance;
}
static inline void TsmPricingRecordSupply(const TsmResourcePricingApi* api,
                                          void* res, double tons)
{
    if (TsmPricingHasMarket(api) && res && tons > 0.0) api->recordSupply(res, tons);
}
static inline void TsmPricingRecordDemand(const TsmResourcePricingApi* api,
                                          void* res, double tons)
{
    if (TsmPricingHasMarket(api) && res && tons > 0.0) api->recordDemand(res, tons);
}
static inline float TsmPricingMarketMult(const TsmResourcePricingApi* api,
                                         void* res)
{
    if (!TsmPricingHasMarket(api) || !res) return 1.0f;
    float m = api->getMarketMultiplier(res);
    return (m == m && m > 0.0f) ? m : 1.0f;
}

// --- v3 consumer guard ------------------------------------------------------
//
// Same structSize-prefix pattern as TsmPricingHasMarket: true when the
// provider carries the getCpiDailyBasket tail. A v2 consumer never touches
// it (reads exactly its prefix); a v3 consumer asks for v3 and calls the
// tail directly. TsmPricingCpiBasket wraps the call with the null/finite
// guards so a missing tail degrades to the documented ~2.15 RUB/day
// fallback instead of faulting.
static inline int TsmPricingHasCpi(const TsmResourcePricingApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmResourcePricingApi, getCpiDailyBasket) +
                              sizeof(api->getCpiDailyBasket) &&
        api->getCpiDailyBasket;
}
static inline float TsmPricingCpiBasket(const TsmResourcePricingApi* api,
                                        float injectedDailyRent)
{
    if (!TsmPricingHasCpi(api)) return 2.15f;
    float c = api->getCpiDailyBasket(injectedDailyRent);
    return (c == c && c > 0.0f) ? c : 2.15f;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_RESOURCE_PRICING_API_H