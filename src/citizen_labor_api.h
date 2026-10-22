// citizen_labor_api.h - the wage, employment and payday contract.
//
// citizen_labor is a Level 1 service. It depends on:
//
//   sim_clock       for the calendar (day, month, year) and the day event
//   currency        for the actual movement of money
//   persistence     for saving employment state across a save/load
//   game_world.h    compile-time offsets only, no runtime dependency
//
// It owns:
//
//   - the per-building-type daily wage table, read from its .ini;
//   - the non-wage transfers: unemployed welfare, student stipend,
//     retiree pension, each with its own age threshold;
//   - the per-citizen employment registry, keyed on TsmCitizenID, which
//     records where a citizen works, what type of building that is, and
//     when they were last paid;
//   - the payroll loop, which runs on the day boundary and pays every
//     citizen whose payday is due;
//   - a single payroll interceptor, which a higher plugin can install to
//     pay wages from its own capital instead of from the state treasury.
//
// It does NOT own:
//
//   - the wallet map or the treasury. That is currency.
//   - retail checkout, demands, or the customer walk. That is
//     citizen_retail.
//   - any ledger or report. Whoever wants a wage total registers a flow
//     observer with currency and sees every "wage" movement as it
//     happens.

#ifndef TSM_CITIZEN_LABOR_API_H
#define TSM_CITIZEN_LABOR_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>          // offsetof for the v3 structSize guard
#include "sim_clock_api.h"    // TsmScheduleFrequency

#define TSM_SERVICE_CITIZEN_LABOR "citizen_labor"

// Bumped from 1u when the payroll interceptor's return values gained
// their current meaning. A version-1 consumer returns 1 for "handled"
// including the case where a private employer defaulted; version 2
// distinguishes PAID (1) from DEFAULTED (2). A version-1 consumer
// running against a version-2 provider would have its defaults
// misread as payments, so the version bump is the contract.
//
// v3 (Labor & Salary Overhaul, Milestone 1) appends the minimum-wage
// floor, the sector-wage table and the per-workplace override
// accessors at the very end. A v2 consumer reads exactly the prefix
// it was compiled against; it must check structSize before touching
// the tail (same pattern as currency v3/v4 and sim_clock getDaysPerMonth).
// The provider publishes BOTH the v2 prefix view and the full v3 iface
// (see citizen_labor.cpp), so enterprise_base (which consumes 2u) and
// older private_construction builds keep loading unchanged.
//
// v4 (Milestone 3) appends ONE more tail accessor, setSectorWage, so
// the in-window Tropico scope control can rewrite a sector base wage
// at runtime. v3 consumers are unaffected: the provider also publishes
// a truncated v3 view (structSize stops before setSectorWage). Both
// v3 and v4 views carry the M1 tail, so TsmLaborHasV3 stays true on
// every current provider.
#define TSM_CITIZEN_LABOR_VERSION 4u
#define TSM_CITIZEN_LABOR_VERSION_V3 3u
#define TSM_CITIZEN_LABOR_VERSION_V2 2u

// Return codes for TsmPayrollInterceptor:
#define TSM_PAYROLL_IGNORED   0  // Not my employee; state pays as normal
#define TSM_PAYROLL_PAID      1  // Employer paid in full from private capital
#define TSM_PAYROLL_DEFAULTED 2  // Employer is insolvent; defaulted on this wage

// Fired for every due wage before it is paid, when a private employer is
// registered as the interceptor.
//
//   workplace     the building the citizen was last seen working at, or
//                 null if they are not employed. Never null on a fired
//                 callback: an unemployed citizen is paid by the state
//                 and the interceptor is not consulted.
//   person        the Person object.
//   grossWage     what the citizen is due, for this payday, in roubles.
//                 Already scaled by the payday frequency and by
//                 `scale_pay_by_shifts` if that is on.
//   buildingType  the type number of the workplace, so the interceptor
//                 does not have to re-look it up.
//
// Return TSM_PAYROLL_PAID (1) to mean "I have paid this wage from private
// capital". citizen_labor will clear the default streak for this citizen
// and lift the workplace's poison, if it had one.
//
// Return TSM_PAYROLL_DEFAULTED (2) to mean "This is my employee, but the
// enterprise is insolvent and cannot pay". citizen_labor increments the
// citizen's default streak, marks the workplace as poisoned so tomorrow's
// workplace pass does not re-assign anyone to it, and after two
// consecutive defaults severs the citizen to unemployment.
//
// Return TSM_PAYROLL_IGNORED (0) to mean "this is not one of my employees";
// the state pays as usual. Also returned for a workplace that is no longer
// private, in which case the state pays and any poison on it is left for
// the monthly clear.
//
// Only one interceptor is supported. Registering a second replaces the
// first without warning; a plugin that wants to arbitrate between two
// payers does it inside its own single callback.
typedef int (*TsmPayrollInterceptor)(void* workplace, void* person,
                                     float grossWage, int buildingType,
                                     void* userData);

typedef struct TsmCitizenLaborApi {
    unsigned structSize;

    // --- configuration queries ---------------------------------------------
    float (*getDefaultDailyWage)(void);
    float (*getUnemployedWelfare)(void);
    float (*getStudentStipend)(void);
    float (*getRetireePension)(void);
    float (*getWageForType)(int buildingType);

    // --- per-person queries ------------------------------------------------
    float (*getEffectiveDailyWage)(void* person);
    int   (*isEmployed)(void* person);
    void* (*getWorkplace)(void* person);
    int   (*getEmployedBuildingType)(void* person);

    // --- payroll interception ----------------------------------------------
    int  (*registerPayrollInterceptor)(TsmPayrollInterceptor cb, void* userData);
    void (*clearPayrollInterceptor)(void);

    // --- manual payday -----------------------------------------------------
    int (*payAllNow)(void);

    // --- v3: minimum wage, sector hierarchy, workplace overrides ---------
    //
    // Appended for the Labor & Salary Overhaul (Milestone 1). A v2
    // consumer sees exactly the bytes above; it MUST check
    // `structSize >= offsetof(TsmCitizenLaborApi, getMinimumWage) +
    // sizeof(getMinimumWage)` before touching this tail.
    //
    // All wages are EFFECTIVE daily figures in the ACTIVE currency
    // (TsmCurrencyActive): explicit *_usd keys win, otherwise the
    // RUB base divided by usd_divisor. Overrides are stored RUB-base
    // and rescaled, so toggling currency rescales them.
    //
    //   getMinimumWage            the statutory floor (never below this).
    //   getSectorWage             effective base for a sector id
    //                             (TSM_SECTOR_* below); <= 0 when unconfigured.
    //   setWorkplaceWageOverride  per-building rate in ACTIVE currency units;
    //                             clamped to >= minimum. Returns 1 on success,
    //                             0 when the building pointer is unusable.
    //   clearWorkplaceWageOverride drops the override (live + pending).
    //   getEffectiveWageForBuilding the full M1 hierarchy for one building:
    //                             override -> sector -> legacy type/default
    //                             -> clamped to the minimum. <= 0 when unknown.
    float (*getMinimumWage)(void);
    float (*getSectorWage)(int sectorId);
    int   (*setWorkplaceWageOverride)(void* building, float dailyWage);
    void  (*clearWorkplaceWageOverride)(void* building);
    float (*getEffectiveWageForBuilding)(void* building);

    // --- v4 (M3): runtime sector wage write -------------------------------
    //
    // Rewrites one sector base wage (effective, active-currency units)
    // at runtime, clamped to >= the statutory minimum. Used by the
    // in-window "Whole Sector" scope control. Returns 1 on success,
    // 0 on a bad sector id or non-positive/NaN wage. Thread-safe.
    // Runtime-only by design: sector wages reset from the INI on the
    // next world load (workplace overrides persist - see the v3 tail).
    int   (*setSectorWage)(int sectorId, float dailyWage);
} TsmCitizenLaborApi;

// --- v3 sector ids ---------------------------------------------------------
//
// Canonical industrial sectors of the overhaul. The type->sector table
// lives in citizen_labor.cpp and follows SOVIETInstructions.txt
// BUILDINGTYPE numbers: factory is light industry, mine is heavy;
// unmapped types (incl. living=2, unknown=30) fall back to default.
#define TSM_SECTOR_HEAVY     0  // mining, heavy plants
#define TSM_SECTOR_INFRA     1  // energy, transport, storage, construction
#define TSM_SECTOR_CARE      2  // healthcare + education
#define TSM_SECTOR_LIGHT     3  // factories, production lines
#define TSM_SECTOR_SERVICES  4  // retail, culture, hospitality, civic
#define TSM_SECTOR_AGRI      5  // fields + farms
#define TSM_SECTOR_COUNT     6

// Guard: does this iface carry the v3 tail?
static inline int TsmLaborHasV3(const TsmCitizenLaborApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmCitizenLaborApi, getMinimumWage) +
                              sizeof(api->getMinimumWage) &&
        api->getMinimumWage && api->getSectorWage &&
        api->setWorkplaceWageOverride && api->clearWorkplaceWageOverride &&
        api->getEffectiveWageForBuilding;
}
// Guard: does this iface carry the v4 setSectorWage tail?
static inline int TsmLaborHasV4(const TsmCitizenLaborApi* api)
{
    return TsmLaborHasV3(api) &&
        api->structSize >= offsetof(TsmCitizenLaborApi, setSectorWage) +
                              sizeof(api->setSectorWage) &&
        api->setSectorWage;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_CITIZEN_LABOR_API_H