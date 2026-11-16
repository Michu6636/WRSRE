// taxes_api.h - the tax contract.
//
// taxes is a Level 2 service. It depends on:
//
//   sim_clock          for the calendar and the tax schedules
//   currency           to move money to the treasury
//   enterprise_base    to read accruals and period totals, and to
//                      debit capital via modifyCompanyCapital
//   game_world.h       compile-time offsets only, via enterprise_base
//
// It owns:
//
//   - the tax rates: VAT, corporate profit tax, property tax;
//   - the tax schedules: each of the three runs on its own calendar
//     frequency, independent of the others;
//   - the profit computation: net revenue minus business costs, where
//     net revenue is gross sales less accrued VAT and business costs
//     are wholesale purchases, foreign imports, and wages;
//   - the settlement loop: for each private enterprise, at the
//     scheduled boundary, debit the tax due from capital, credit the
//     treasury under a named flow category, and clear the accrual.
//
// It does NOT own:
//
//   - the accrual fields, the period fields, or the register of
//     enterprises. Those belong to enterprise_base. This service reads
//     and writes them through the public API.
//   - the retail sale itself. enterprise_base's retail interceptor
//     decides when a sale happens and what it is worth; this service
//     decides what tax is owed on it.
//   - the pool. Tax revenue goes to the treasury, never to the pool.
//
// WHEN THIS PLUGIN IS ABSENT
//
// enterprise_base treats a missing taxes service as "no tax". No
// accruals are recorded, no settlements fire, no money moves. A save
// that was written with taxes enabled and is loaded with it disabled
// will keep the accrual fields it had, but nothing will settle them.
// That is the intended degraded mode: a user who removed the tax plugin
// has chosen a world with no tax.

#ifndef TSM_TAXES_API_H
#define TSM_TAXES_API_H

#include <stddef.h>
#include "sim_clock_api.h"      // TsmScheduleFrequency

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_TAXES "taxes"
// v2 appends the interactive fiscal-policy tail: isTaxEnabled, setTaxRate and
// setTaxEnabled. The registry matches versions EXACTLY, so the provider
// publishes BOTH the v1 view (prefix only, for any consumer built against the
// old header - e.g. a third-party private-shop variant) and the v2 view (full
// struct). This mirrors the currency (v3/v4), citizen_labor (v2/v3) and
// resource_pricing (v1/v2) dual-provide precedent. build.bat rebuilds every
// in-tree consumer with this header. v3 retains both prefix views and appends
// a coherent read-only configured-policy snapshot.
#define TSM_TAXES_VERSION    3u
#define TSM_TAXES_VERSION_V2 2u
#define TSM_TAXES_VERSION_V1 1u

// Tax categories addressed by the v2 setters/getters. A future tax on a new
// base adds a value here, not a new API function.
#define TSM_TAX_VAT      0
#define TSM_TAX_PROFIT   1
#define TSM_TAX_PROPERTY 2

// v3: one coherent copy of CONFIGURED policy, including suspended taxes.
// Indices are TSM_TAX_*. VAT/profit are fractions [0,1]; property is the
// live base amount per enterprise in the provider's active-currency units,
// not the raw pre-USD-conversion INI value or the final scaled bill.
// propertyLandValue: 0 = flat base, 1 = base * (0.50 + 1.50 * attractiveness).
// Does not sample the currency service, schedules, arrears or settlements.
typedef struct TsmTaxPolicySnapshot {
    unsigned structSize; // provider sets to sizeof(TsmTaxPolicySnapshot)
    float configured[3];
    int enabled[3];      // normalized 0/1
    int propertyLandValue; // normalized 0/1
} TsmTaxPolicySnapshot;

typedef struct TsmTaxesApi {
    unsigned structSize;

    // --- rates -------------------------------------------------------------
    //
    // The rates this service is currently configured with, as fractions
    // (0.20 = 20%). A consumer that produces taxable income through its
    // own path - private_shops' pub drinks - calls getVatRate to accrue
    // at the same rate the service will settle at. Returns 0 when the
    // tax is disabled.
    //
    // The property tax is a flat amount per enterprise, not a rate, and
    // is not exposed in this prefix; v3 exposes the configured base.

    float (*getVatRate)(void);
    float (*getProfitTaxRate)(void);

    // --- accrual -----------------------------------------------------------
    //
    // Report a VAT accrual against `entity`. The entity is opaque - it
    // is an enterprise building pointer today, and will be a citizen
    // pointer when income tax arrives. The service decides, from its
    // own schedule, whether to accrue (and hold the liability on the
    // entity) or to settle immediately.
    //
    // A negative or non-finite amount is ignored.

    void (*recordVatAccrual)(void* entity, double amount);

    // --- settlement --------------------------------------------------------
    //
    // Force a settlement of every outstanding accrual immediately,
    // outside the schedule. A "pay now" button on a future UI would
    // call this. Settles whatever is currently outstanding, using the
    // current rates, then clears the accruals and the period totals.
    //
    // Does not depend on or consult the schedules: it settles
    // everything, right now.

    void (*settleNow)(void);

    // --- v2: interactive fiscal policy -------------------------------------
    //
    // Read and write the live tax policy, so a City Hall panel can toggle and
    // tune the three taxes without this plugin owning any UI. All three are
    // safe from any thread (the provider brackets its state with its own
    // lock). A v1 provider leaves these pointers null - guard with
    // TsmTaxesHasPolicy (below) before touching them.
    //
    // `taxKind` is one of TSM_TAX_VAT / TSM_TAX_PROFIT / TSM_TAX_PROPERTY.

    // 1 when the named tax is currently levied, 0 when suspended. A suspended
    // tax stops all NEW accrual (getVatRate / getProfitTaxRate report 0) and
    // does not settle while off; any liability accrued before the switch is
    // held and settles when the tax is re-enabled.
    int (*isTaxEnabled)(int taxKind);

    // Set the named tax's rate. For VAT and profit the rate is a fraction in
    // [0, 1] (0.20 = 20%); for property it is the flat per-enterprise amount
    // in the active currency (>= 0). Returns 1 on acceptance, 0 when the kind
    // is unknown or the value is out of range (the current value is left
    // untouched). Takes effect on the next accrual / settlement boundary.
    int (*setTaxRate)(int taxKind, float rate);

    // Enable (1) or suspend (0) the named tax. Returns 1 on acceptance, 0 for
    // an unknown kind.
    int (*setTaxEnabled)(int taxKind, int enabled);

    // --- v3: read-only policy snapshot (append-only) -------------------------
    // Copy all fields under the provider's policy lock. Returns 1 on success;
    // 0 before successful provider Start, for null output or insufficient size; no
    // output writes on failure. Caller owns writable storage for outSize bytes.
    // No allocation, settlement, policy changes or callbacks. This is coherent
    // tax policy only, not an atomic tax/treasury/currency snapshot.
    int (*getPolicySnapshot)(TsmTaxPolicySnapshot* out, unsigned outSize);

} TsmTaxesApi;

// --- v2 consumer guard ------------------------------------------------------
//
// The registry matches versions exactly, so a v2 consumer asks for v2 and a
// v1 consumer keeps asking for v1. This helper lets one binary tolerate
// whichever iface it was handed: it checks structSize before touching the
// appended fiscal-policy tail and reports 0 for a v1 provider.
static inline int TsmTaxesHasPolicy(const TsmTaxesApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmTaxesApi, setTaxEnabled) +
                              sizeof(api->setTaxEnabled) &&
        api->isTaxEnabled && api->setTaxRate && api->setTaxEnabled;
}

// v1/v2 prefixes remain valid; never read the new pointer before size guard.
static inline int TsmTaxesHasPolicySnapshot(const TsmTaxesApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmTaxesApi, getPolicySnapshot) +
                              sizeof(api->getPolicySnapshot) &&
        api->getPolicySnapshot;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_TAXES_API_H