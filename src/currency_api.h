// currency_api.h - the money contract.
//
// currency is a Level 0 service: it depends on nothing but the persistence
// contract, and it exists so that every other plugin has one place to move
// money and one answer to "how much does this citizen have".
//
// It owns two things:
//
//   - the per-citizen wallet map, keyed by a stable citizen ID derived
//     from the person object. The base game has no money field on a
//     Person - that has been confirmed - so this map is the money.
//   - the state treasury, at game+0x580 (USD) and +0x588 (RUB), both read
//     and written as doubles. Confirmed.
//
// It does NOT own:
//
//   - resource prices. Price tables, per-resource retail figures and the
//     price-pass hook are a Level 1 concern (resource_pricing) because
//     they require knowing which resource is which and reading the
//     engine's own price fields, neither of which is money.
//   - wage schedules or payday. That is citizen_labor.
//   - retail checkout, demands, or the customer walk. That is
//     citizen_retail.
//   - the monthly state ledger, the CSV report, or the HUD. A Level 1
//     state_ledger consumes the flow observers this service publishes;
//     the HUD is ui_overlay's job.
//
// The one thing currency knows how to do beyond raw balance arithmetic
// is *label* a money movement. moveTreasury, citizenToState and
// stateToCitizen take a category string and fire every registered flow
// observer. A state ledger consumes those events and aggregates them; a
// plugin that wants its own per-flow statistics registers the same callback.
// The string is opaque to currency and shared by convention among consumers:
//
//   "wage"               citizen_labor paying a state-employed worker
//   "state_store_sale"   citizen_retail taking payment at a state shop
//   "wholesale_b2b"      enterprise_base buying stock from the state
//   "vat", "profit_tax"  enterprise_base remitting accrued taxes
//   "property_tax"       enterprise_base remitting flat property tax
//   "pension", "welfare" citizen_labor's non-wage transfers
//   "construction"       private_construction draining the pool
//   "procurement"        private_shops paying a private producer for
//                        output state logistics picked up
//
// Nothing in the API validates these. A misspelt category produces a
// ledger line nobody can name and nothing else.

#ifndef TSM_CURRENCY_API_H
#define TSM_CURRENCY_API_H

#include <string.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_CURRENCY "currency"
#define TSM_CURRENCY_VERSION 4u
#define TSM_CURRENCY_VERSION_V3 3u

// Currency codes for treasury access. The mapping to the game's own
// fields is a private detail of the service - see currency.cpp for
// which one is which, and do not rely on it.
#define TSM_CURRENCY_RUB 0
#define TSM_CURRENCY_USD 1

// v4: unified global currency. `primary_currency` in currency.ini selects
// which treasury (game+0x580 USD vs game+0x588 RUB) the labelled
// operations (moveTreasury / citizenToState / stateToCitizen) route to.
// getActiveCurrency / getCurrencySymbol are appended at the end so a v3
// consumer reading only the prefix sees exactly the bytes it expects.
// Consumers must guard with structSize before calling (see TsmCurrencyActive
// below); the provider still honours the explicit-currency get/modify
// calls for any caller that needs the other leg.

// Fired every time the treasury moves through a labelled operation.
//
// `signedAmount` is the change to the treasury, in the requested
// currency: positive means the treasury received, negative means it
// paid out. A caller that fires this directly passes the same signed
// delta it just applied to the treasury.
//
// Runs on the caller's thread after the balance changes are committed and
// currency's transaction lock has been released. Must not block. A faulting
// observer is disabled for the session and the others carry on. No event is
// emitted for rejected, zero, or failed native-memory transactions.
typedef void (*TsmMoneyFlowCallback)(void* userData,
                                     const char* category,
                                     double signedAmount,
                                     int currency);

// The citizen ID used to key wallets.
//
// Derived from two fields of the Person object that have been stable
// across every build tested so far:
//
//   person + 0x08   uint32   the citizen's entity ID
//   person + 0xA4   float    the speed multiplier, read as a bit pattern
//
// The two together are unique in practice and survive a save/load,
// because the game itself persists both fields.
//
// Inline and header-only so every consumer uses the same ID and the
// wallet map cannot end up keyed on a slightly different value. The
// caller is responsible for passing a valid pointer - this is a raw
// read and does not guard.
static inline uint64_t TsmCitizenID(const void* person)
{
    if (!person) return 0;
    uint32_t eid, seed;
    memcpy(&eid,  (const char*)person + 0x08, 4);
    memcpy(&seed, (const char*)person + 0xA4, 4);
    return ((uint64_t)eid << 32) | (uint64_t)seed;
}

// Iterator callback for forEachWallet. `citizenId` is the key, `wallet`
// the balance. Return non-zero to continue, zero to stop early.
typedef int (*TsmWalletVisitor)(uint64_t citizenId, float wallet,
                                void* userData);

typedef struct TsmCurrencyApi {
    unsigned structSize;

    // --- per-citizen wallets -----------------------------------------------
    //
    // A person pointer that is null, out of range, or unreadable is a
    // no-op for the mutators and 0.0f for the getter. The ID is derived
    // from fields the caller has already had to validate to read the
    // person at all, so a bad pointer here is the caller's bug and not
    // a case worth throwing over.
    //
    // All mutators reject NaN/infinite amounts and nonfinite/overflowing
    // results without moving money or emitting flow events. Positive paired
    // wallet/treasury transfers require both native legs to be available.
    // Finite non-positive amounts remain no-ops (not reverse transfers).

    float (*getWalletBalance)(void* person);

    // Add to the wallet without firing any observer. Returns 1 on
    // success, 0 if `person` is invalid. A non-positive amount is
    // accepted and simply does nothing.
    int   (*addWalletBalance)(void* person, float amount);

    // Deduct `amount` from the wallet only if the wallet can cover it.
    // Returns 1 if the charge happened in full, 0 if the balance was
    // short. Also adds to the citizen's own "money spent" running total
    // at person+0x734, because a charge is a purchase by definition.
    int   (*chargeCitizen)(void* person, float amount);

    // Deduct as much as possible, up to `amount`, and return how much
    // was actually taken. Never overdraws. Also updates person+0x734
    // for whatever was taken. Use this when a partial payment beats no
    // payment - a shop sale to a near-broke customer, or the last
    // rouble of an outstanding debt.
    float (*chargeCitizenPartial)(void* person, float maxAmount);

    // Walk every wallet in the map. The visitor sees a consistent
    // snapshot taken under the lock; the map itself is not iterated
    // live, so a callback that modifies wallets is safe.
    void  (*forEachWallet)(TsmWalletVisitor cb, void* userData);

    // Total of every wallet, for a quick economy-wide figure. Computed
    // under the lock, no visitor call.
    double (*getTotalWalletBalance)(void);

    // --- state treasury ----------------------------------------------------
    //
    // Both read and write the game's own treasury fields directly, and
    // both are safe to call from any thread.

    double (*getTreasuryBalance)(int currency);
    void   (*modifyTreasury)(double delta, int currency);

    // Move the treasury by `signedAmount` and fire every flow observer
    // with the same signed delta and the given category.
    //
    // Positive means the treasury received, negative means it paid out.
    // The citizen wallets are not touched.
    //
    // This is what a plugin calls when it manages its own treasury
    // operation but wants the ledger to see the movement. A plugin that
    // needs to move money between a wallet and the treasury uses
    // citizenToState / stateToCitizen / citizenToStatePartial instead,
    // which call this internally after moving the wallet.
    void   (*moveTreasury)(double signedAmount, const char* category);

    // --- labelled money movement -------------------------------------------
    //
    // Operations that move money between citizens and state. Both fire
    // every registered flow observer, in registration order, after the
    // balance changes have been committed. citizenToState is all-or-nothing;
    // failure leaves the wallet, spent counter and treasury unchanged.
    // Only citizenToStatePartial may collect less than the requested amount.
    // stateToCitizen never debits the treasury for an invalid recipient.
    int   (*citizenToState)(void* person, float amount, const char* category);
    float (*citizenToStatePartial)(void* person, float maxAmount,
                                   const char* category);
    void  (*stateToCitizen)(void* person, float amount, const char* category);

    // --- flow observers ----------------------------------------------------

    int  (*registerFlowObserver)(TsmMoneyFlowCallback cb, void* userData);
    void (*unregisterFlowObserver)(int handle);

    // --- v4: unified global currency (append-only) ---------------------------
    //
    // primary_currency in currency.ini selects the active treasury:
    // TSM_CURRENCY_RUB (game+0x588) or TSM_CURRENCY_USD (game+0x580).
    // moveTreasury / citizenToState / stateToCitizen route to it; the
    // explicit-currency get/modify calls still honour either leg.
    // Added at the very end so a v3 consumer (structSize == v3 size)
    // reads exactly the prefix it was compiled against.
    int         (*getActiveCurrency)(void);
    const char* (*getCurrencySymbol)(void);
} TsmCurrencyApi;

// --- v4 consumer guards ------------------------------------------------------
//
// The service registry matches versions exactly, so a v4 consumer asks for
// v4 and a v3 consumer keeps asking for v3. These helpers let one binary
// tolerate whichever iface it was handed: they check structSize before
// touching the appended tail and fall back to RUB (the pre-v4 behaviour).
static inline int TsmCurrencyHasActive(const TsmCurrencyApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmCurrencyApi, getActiveCurrency) +
                              sizeof(api->getActiveCurrency) &&
        api->getActiveCurrency && api->getCurrencySymbol;
}
static inline int TsmCurrencyActive(const TsmCurrencyApi* api)
{
    if (!TsmCurrencyHasActive(api)) return TSM_CURRENCY_RUB;
    int c = api->getActiveCurrency();
    return (c == TSM_CURRENCY_USD) ? TSM_CURRENCY_USD : TSM_CURRENCY_RUB;
}
static inline const char* TsmCurrencySymbol(const TsmCurrencyApi* api)
{
    if (!TsmCurrencyHasActive(api)) return "RUB";
    const char* s = api->getCurrencySymbol();
    return (s && s[0]) ? s : "RUB";
}

#ifdef __cplusplus
}
#endif

#endif // TSM_CURRENCY_API_H