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
// v5 (2026-11-22 economy direction, D2/D3): the Republic market order book
// (v2: recordSupply/recordDemand/getMarketMultiplier/getMarketBalance) and
// the local per-building scarcity quotes (v4) are REMOVED. All money uses the
// game's native price and nothing else; a future market model may change the
// native prices themselves, but never adds a second price layer. See
// docs/visions/economy-direction-2026-11-22.md section 3.
//
// Removing fields changes the layout after getPrice, so v5 is a NEW layout,
// not an append: getCpiDailyBasket moved up to sit directly after getPrice.
// The provider publishes two views:
//   v1  prefix through getPrice (unchanged layout - private_construction and
//       any other price-only consumer keeps working);
//   v5  the full struct below.
// The old v2/v3/v4 layouts are no longer published. A stale consumer built
// against them fails to resolve the service and must degrade as it already
// does when resource_pricing is absent. build.bat rebuilds every in-tree
// consumer against this header.
#define TSM_RESOURCE_PRICING_VERSION    5u
#define TSM_RESOURCE_PRICING_VERSION_V5 5u
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

    // --- v5: CPI survival basket (was the v3 tail) --------------------------
    //
    // Daily Basket Cost (CPI), in the ACTIVE currency units the provider
    // resolves through TsmCurrencyActive (same convention as getPrice):
    //
    //   CPI = (Pfood x Qfood) + (Pmeat x Qmeat) + (Pclothes x Qclothes)
    //       + dailyRent
    //
    // P* are the native getPrice answers for the provider's own
    // getFood/getMeat/getClothes records; Q* are the [cpi_basket]
    // quantities (tons/day); dailyRent is the injected per-day rent average,
    // or the [cpi_basket] default_daily_rent fallback (USD-scaled in USD
    // mode) when injectedDailyRent <= 0.
    //
    // Pure query: reads records and config. Safe from any thread. Always
    // returns a finite value > 0 (falls back to the configured default
    // basket, ~2.15 RUB/day at shipped fallbacks, when no world/prices
    // are readable).
    float (*getCpiDailyBasket)(float injectedDailyRent);
} TsmResourcePricingApi;

// --- CPI consumer guard -----------------------------------------------------
//
// structSize-prefix guard: true when the provider carries getCpiDailyBasket
// (the v5 view). A v1 view ends at getPrice, so this is false for it.
// TsmPricingCpiBasket wraps the call with the null/finite guards so a
// missing tail degrades to the documented ~2.15 RUB/day fallback instead of
// faulting.
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
