// citizen_retail_api.h - the shop-sale contract.
//
// citizen_retail is a Level 1 service. It depends on:
//
//   sim_clock          for the per-frame tick that drives the shop walk
//   currency           to charge the citizen and credit the treasury
//   resource_pricing   to price what is being sold
//   game_world.h       compile-time offsets only
//
// It owns:
//
//   - the customer walk: for every shop building, for every customer
//     inside, for every demand of kind 1 or 2, look up a price and
//     charge for what the base game already gave away for free.
//
//   - the per-citizen "already shopped today" marker, keyed on
//     TsmCitizenID, which is what makes a citizen who visits three
//     shops on one walk pay at exactly one of them - the same behaviour
//     the base game has, and the same behaviour the old citizen_economy
//     plugin had.
//
//   - the retail interceptor: a single callback slot that a higher
//     plugin installs to handle a sale from its own capital instead of
//     from the state treasury.
//
// It does NOT own:
//
//   - the resources themselves, or the shop's storage. The base game
//     moves goods from a shop's storage into the customer's consumed
//     state on its own; this plugin only charges money for what the
//     base game already did. Refusing the charge does not put the goods
//     back.
//
//   - pubs, hotels, restaurants, or any other building type that has a
//     "customers" vector but is not a shop. Which types count as shops
//     is a setting; adding a type widens the walk, and the resource the
//     citizen is buying determines the price.
//
//   - the wage side. That is citizen_labor.
//
// The interceptor contract: it fires for every sale before the state
// charges the citizen. Three return values (v2):
//
//   >0  "handled"  - the interceptor took payment (or gave it away);
//                    the state does not charge and the sale proceeds.
//   0   "decline"  - the state charges the citizen as usual. This keeps
//                    its v1 meaning, so an out-of-tree v1 interceptor
//                    that returns 0 still falls through to the state
//                    charge exactly as before.
//   -1  TSM_RETAIL_INSUFFICIENT - the interceptor took NOTHING and the
//                    citizen cannot pay the full price: the sale must
//                    abort, no goods may be dispensed, and the state
//                    must not charge either.
//
// v1 interceptors documented only {0, non-zero}; a v1 interceptor can
// never emit -1, so a v2 gate sees -1 only from a v2 interceptor. The
// ABI is guarded from the provider side instead: citizen_retail
// dual-publishes the v1 iface view (same struct layout, registered
// under version 1u) and the v2 iface, and a v2 enterprise_base only
// ever registers through the v2 iface it consumed.

#ifndef TSM_CITIZEN_RETAIL_API_H
#define TSM_CITIZEN_RETAIL_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_CITIZEN_RETAIL "citizen_retail"
#define TSM_CITIZEN_RETAIL_VERSION 2u
#define TSM_CITIZEN_RETAIL_VERSION_V1 1u

// Interceptor return values. chargeCitizenPartial takes partial funds
// unconditionally (WalletTake: balance - min(balance, maxAmount)), so an
// interceptor that refuses AFTER taking money commits wallet theft.
// Insufficient funds must therefore be signalled BEFORE any charge.
#define TSM_RETAIL_INSUFFICIENT (-1)

// Fired for every sale about to be charged. The `totalCost` is what the
// citizen would pay if the state charged them; a private shop is free
// to charge a different amount, or to refuse the sale entirely, or to
// give it away for free. The interceptor is not asked what it did with
// the money - only whether the state should stay out of it.
//
//   shop        the building the sale is happening in.
//   person      the customer.
//   totalCost   the state's own price for this sale, in roubles.
//   resRecord   the resource record the demand pointed at.
//   userData    whatever was passed at registration.
//
// Return values (v2 - the gate interprets all three):
//
//   >0  "handled": the interceptor took payment; the state stays out
//       and the goods are dispensed.
//   0   "not handled": the state charges the citizen as usual - the
//       same behaviour as if no interceptor were installed.
//   TSM_RETAIL_INSUFFICIENT (-1): the citizen could not pay the full
//       price and the interceptor took NOTHING. The gate aborts the
//       sale: no state charge, no dispensed goods.
//
// An interceptor that charges must pre-check the wallet BEFORE taking
// any money: chargeCitizenPartial takes partial funds unconditionally,
// so refusing afterwards would leave the citizen poorer with no goods.
typedef int (*TsmRetailInterceptor)(void* shop, void* person,
                                    float totalCost, void* resRecord,
                                    void* userData);

typedef struct TsmCitizenRetailApi {
    unsigned structSize;

    // Install or replace the interceptor. Passing null clears it -
    // identical to clearRetailInterceptor. Only one interceptor is
    // supported; registering a second replaces the first without
    // warning. A plugin that needs to arbitrate between two payers
    // does it inside its own single callback.
    int  (*registerRetailInterceptor)(TsmRetailInterceptor cb, void* userData);
    void (*clearRetailInterceptor)(void);
} TsmCitizenRetailApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_CITIZEN_RETAIL_API_H