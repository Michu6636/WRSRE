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
//   - it owns the answer to "what is one ton of this worth in the active
//     currency". The engine keeps a per-resource price field (+0x58 RUB,
//     +0x5C USD) and recomputes it on its own schedule. That field is the
//     truth. There is no fallback table and no default price: a resource the
//     engine has not priced is UNPRICED and getPrice answers 0.0f.
//
//   - it looks up a resource record by name. The engine has its own
//     resolver, but it is hooked by plugins/resources and a Level 1
//     service may not depend on a higher one. So resource_pricing scans
//     the vector itself, which finds both base-game and mod resources,
//     and does not care what other plugins are installed.
//
// UNPRICED IS NOT ZERO MONEY
//
// Economy direction D2/D8: all money uses the game's native prices. A
// missing price must never be replaced by a made-up value. Consumers must
// test the answer with TsmNativePriceUsable (src/price_guards.h) and then:
//
//   - D32 shop sale: give the good as vanilla, move no money, log
//     "ERROR UNPRICED" and count it for the Almanac;
//   - D33 business bill (construction estimate, wholesale delivery, B2B
//     freight): refuse the whole transaction, loudly;
//   - D34 CPI: treat the basket as unavailable, skip wealth-class demand
//     scaling, show UNAVAILABLE and refuse new bank credit.
//
// There is no separate price-override mechanism here. A resource that IS
// produced can already be forced through [price] in plugins/resources.ini;
// a resource the engine has never priced stays UNPRICED until something
// produces it.

#ifndef TSM_RESOURCE_PRICING_API_H
#define TSM_RESOURCE_PRICING_API_H

#include <stddef.h>            // offsetof, for the structSize guard

#ifdef __cplusplus
extern "C" {
#endif

#include "currency_api.h"      // TSM_CURRENCY_RUB, TSM_CURRENCY_USD

#define TSM_SERVICE_RESOURCE_PRICING "resource_pricing"
// v6 (2026-11-22 economy direction step 2, D8/D32/D33/D34): the fallback
// price table, the `default` price, `default_usd` and `usd_divisor` are
// removed. getPrice returns 0.0f for a resource with no native price
// (UNPRICED) and getCpiDailyBasket returns 0.0f when the basket cannot be
// priced. A v5 consumer would misinterpret 0.0f as a real price, so v6 is
// the only full view published; the v1 prefix remains available for callers
// that only need the record lookups and getPrice.
//
// Removing fields changes the layout after getPrice, so v6 is a NEW layout,
// not an append: getCpiDailyBasket sits directly after getPrice.
// The provider publishes two views:
//   v1  prefix through getPrice (record lookups + getPrice with the same
//       UNPRICED contract);
//   v6  the full struct below.
// The old v2/v3/v4/v5 layouts are no longer published. A stale consumer built
// against them fails to resolve the service and must degrade as it already
// does when resource_pricing is absent. build.bat rebuilds every in-tree
// consumer against this header.
#define TSM_RESOURCE_PRICING_VERSION    6u
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
    // The live native price of one ton of the given resource, in the
    // requested currency: the engine's own +0x58 / +0x5C field.
    //
    // Returns 0.0f when there is no native price. That means UNPRICED:
    // it is not "worth nothing", and callers must not charge it as zero.
    // The same 0.0f comes back for a NULL record, an unreadable record, a
    // non-finite field, a negative field and a field at or below 0.01.
    // Never returns NaN, infinity or a negative number.
    //
    // The provider logs "ERROR UNPRICED" once per resource and currency
    // when it answers 0.0f, so a missing price is visible in the log even
    // when a caller forgets to report it.

    float (*getPrice)(void* resRecord, int currency);

    // --- v6: CPI survival basket (was the v3 tail) --------------------------
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
    // or the documented [cpi_basket] default_daily_rent when
    // injectedDailyRent <= 0.
    //
    // Returns 0.0f when the basket is UNAVAILABLE: no world, an unreadable
    // record, or any of the three goods without a native price. There is no
    // default basket (D34). Callers must show UNAVAILABLE and pause what
    // depends on CPI rather than substituting a number.
    //
    // Pure query: reads records and config. Safe from any thread.
    float (*getCpiDailyBasket)(float injectedDailyRent);
} TsmResourcePricingApi;

// --- CPI consumer guard -----------------------------------------------------
//
// structSize-prefix guard: true when the provider carries getCpiDailyBasket
// (the v6 view). A v1 view ends at getPrice, so this is false for it.
static inline int TsmPricingHasCpi(const TsmResourcePricingApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmResourcePricingApi, getCpiDailyBasket) +
                              sizeof(api->getCpiDailyBasket) &&
        api->getCpiDailyBasket;
}
// Returns 0.0f when the basket is UNAVAILABLE. It never invents a default:
// callers must treat 0.0f as "no CPI today" (D34).
static inline float TsmPricingCpiBasket(const TsmResourcePricingApi* api,
                                        float injectedDailyRent)
{
    if (!TsmPricingHasCpi(api)) return 0.0f;
    float c = api->getCpiDailyBasket(injectedDailyRent);
    return (c == c && c > 0.0f) ? c : 0.0f;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_RESOURCE_PRICING_API_H
