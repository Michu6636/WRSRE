// enterprise_base_api.h - the private-enterprise contract.
//
// enterprise_base is a Level 2 service. It depends on:
//
//   sim_clock          for the calendar and scheduled pool events
//   currency           to move money between wallets, treasury and pool
//   persistence        to save the company register across a save/load
//   ui_overlay         runtime dependency only, for the enterprise card
//   citizen_labor      to intercept wages for private employees
//   citizen_retail     to intercept sales at private shops
//   game_world.h       compile-time offsets only
//
//   taxes              OPTIONAL. When present, enterprise_base routes
//                      its own retail VAT computation through it.
//                      When absent, enterprises pay no tax.
//
// It owns:
//
//   - the register of private enterprises, keyed on building pointer
//     within a session and on (objectName, position) across a save;
//   - the per-enterprise capital and markup;
//   - the accrual fields (accruedVatLiability, accruedProfitTaxLiability)
//     and the period fields (periodGrossSales, periodVatAccrued,
//     periodWholesaleCosts, periodWagesPaid, periodForeignCosts), as
//     raw bookkeeping that a tax engine reads and writes;
//   - the Victoria-3-style private investment pool, with a monthly
//     siphon from profitable enterprises;
//   - the payroll interceptor, which pays private employees from their
//     employer's capital instead of from the state treasury;
//   - the retail interceptor, which charges customers a marked-up price
//     at private shops and credits the shop's own capital.
//
// It does NOT own:
//
//   - any tax rate, tax schedule, or tax settlement. Those are the
//     `taxes` plugin's business. This service exposes the accrual
//     fields and the period fields as raw data; the tax engine decides
//     what they mean.
//   - the HUD or the overlay column. The enterprise card is drawn via
//     ui_overlay, and every plugin that wants to add a line under it
//     registers its own drawer with ui_overlay.
//   - the save/load event itself. persistence fires it; this plugin
//     subscribes and writes its own file.

#ifndef TSM_ENTERPRISE_BASE_API_H
#define TSM_ENTERPRISE_BASE_API_H

#include <stddef.h>
#include "sim_clock_api.h"      // TsmScheduleFrequency

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_ENTERPRISE_BASE "enterprise_base"
// v3 was the API with the built-in tax engine: getVatRate,
// recordVatAccrual, recordProfitTaxAccrual, getAccruedTaxes (which
// computed profit tax on the fly), triggerTaxSettlement. v4 split
// those into the `taxes` plugin. What remains here is the enterprise
// as bookkeeping: accrual fields and period fields, exposed as raw
// reads and writes.
//
// The old functions are gone, not deprecated. A v3 consumer that called
// getVatRate or recordVatAccrual gets a null from consume and refuses
// to load, which is correct: the API it expects no longer exists.
//
// v5 adds getMonthlyEconomySummary, a poll-based accessor for the most
// recently closed month's private-sector totals. Purely additive: no
// existing field moved, no existing meaning changed.
//
// v6 adds describeEnterprise, one-call snapshot of a single enterprise for
// the in-window panel (ui_panels). Also purely additive - every existing
// field stays at its offset. NOTE the service registry matches EXACTLY, so
// v6 means every consumer (taxes, private_shops, private_construction) must
// be rebuilt with this header; build.bat builds them all together.

// v7 adds getMonthlyEvictionCount: successful evictions executed during the
// current calendar month, incremented in the eviction dispatch of
// CollectResidentialRent and reset at each month boundary (OnMonth). Purely
// additive - appended at the end of the ABI; every consumer rebuilds with
// this header so the service registry stays version-consistent. Safe to call
// from any thread.
//
// v8 (Task 7) appends ONE more tail accessor, getResidentialRentMonthly:
// the residential rent SETTING (per tenant per month, already resolved to
// the ACTIVE currency units at Start), so taxes can derive the true
// average daily rent for the City Hall CPI readout
// (rentPerDay = monthly / sim_clock getDaysPerMonth). It is the neutral
// base BEFORE the per-building attractiveness/rent-slider multipliers -
// the same base CollectResidentialRent starts from. Same append-only rule
// as before: v7 consumers read exactly the prefix they compiled against;
// the provider publishes BOTH the v7 prefix view and the full v8 iface.
#define TSM_ENTERPRISE_BASE_VERSION 8u
#define TSM_ENTERPRISE_BASE_VERSION_V7 7u

// What a capital movement is *for*.
typedef enum TsmCapitalCategory {
    TSM_CAP_RETAIL          = 0,  // retail sale proceeds
    TSM_CAP_WHOLESALE       = 1,  // state-delivered stock
    TSM_CAP_FOREIGN         = 2,  // imported stock, money leaves
    TSM_CAP_WAGE            = 3,  // employee wage
    TSM_CAP_TAX             = 4,  // tax remittance
    TSM_CAP_POOL            = 5,  // dividend siphoned to the pool
    TSM_CAP_CONSTRUCTION    = 6,  // construction spend
    TSM_CAP_OTHER           = 7,
    TSM_CAP_RENT            = 8,  // residential tenant rent collection
    TSM_CAP_PRODUCTION      = 9   // factory/mine/farm dispatch revenue
} TsmCapitalCategory;

// Which accrual field a write refers to. Two today. A future tax on
// some other base adds a third value here, not a new API function.
typedef enum TsmAccrualKind {
    TSM_ACCRUAL_VAT         = 0,
    TSM_ACCRUAL_PROFIT      = 1
} TsmAccrualKind;

// v6: one call, one snapshot - the in-window panel's view of one enterprise.
//
// Every value is a field the accounting already keeps; monthNet is the same
// expression SiphonToPool uses (sales - wholesale - wages - taxes), so the
// panel can never disagree with the ledger about what profit was.
// poolTotal and poolMonthInflow are GLOBAL context - identical for every
// building - present so an economy card can show the investment pool without
// a second service call.
typedef struct TsmEnterpriseInfo {
    unsigned structSize;      // caller sets = sizeof(TsmEnterpriseInfo)
    int    isPrivate;         // 0 = registered but not tagged [P]
    int    solvent;
    double capital;
    float  markup;
    double monthSales;        // current calendar month; reset at the boundary
    double monthWholesale;
    double monthWages;
    double monthTaxes;
    double monthDividend;     // siphoned to the pool this month
    double monthNet;          // sales - (wholesale + wages + taxes); may be < 0
    double accruedVat;
    double accruedProfitTax;
    double poolTotal;         // GLOBAL - same for every building
    double poolMonthInflow;   // GLOBAL - this month's siphon total
} TsmEnterpriseInfo;

typedef struct TsmEnterpriseBaseApi {
    unsigned structSize;

    // --- tagging -----------------------------------------------------------
    // A building is private when the player has said so: a `[P]`, `(P)`,
    // `[Private]` or `(Private)` substring in either the custom or the
    // object name. `all_enterprises_private` overrides that.
    int (*isBuildingPrivate)(void* building);

    // --- enumeration -------------------------------------------------------
    // A stable snapshot of the private enterprise register. The order is
    // insertion order and is not guaranteed across a save/load. Only
    // *private* enterprises are enumerated.
    size_t (*getEnterpriseCount)(void);
    void*  (*getEnterpriseByIndex)(size_t index);

    // --- names -------------------------------------------------------------
    const char* (*getBuildingObjectName)(void* building);
    int         (*getBuildingCustomName)(void* building,
                                         wchar_t* outName, size_t maxChars);

    // --- capital -----------------------------------------------------------
    // getCompanyCapital returns 0.0 for a building with no entry.
    // modifyCompanyCapital applies the delta under the classification
    // above; a positive delta is a credit, a negative one is a debit.
    // The delta is applied unconditionally - a negative resulting
    // balance is allowed.
    double (*getCompanyCapital)(void* building);
    void   (*modifyCompanyCapital)(void* building, double delta,
                                   TsmCapitalCategory category);
    int    (*isCompanySolvent)(void* building);

    // The markup is a float multiplier applied on top of the base price
    // at a private shop. 1.30 means "sell at 30% above the state price".
    float  (*getCompanyMarkup)(void* building);
    void   (*setCompanyMarkup)(void* building, float markup);

    // --- wholesale ---------------------------------------------------------
    // Debits the company, credits the treasury, fires the flow observer
    // with category "wholesale_b2b".
    void (*chargeWholesaleDelivery)(void* building, double wholesaleCost);

    // --- accruals ----------------------------------------------------------
    // The enterprise owns two accrual fields, one per tax base. A tax
    // engine decides what accrues; these functions are the generic
    // interface it uses.
    //
    //   getAccruals  reads the raw accrual fields. Nothing is computed.
    //   addAccrual   increments the accrual field named by `kind`, and
    //                for VAT also increments periodVatAccrued (which is
    //                the period running total used to derive net revenue
    //                for a profit tax).
    //   clearAccrual zeros the accrual field named by `kind`, after the
    //                tax engine has settled it.
    //
    // All three are safe on a null or unregistered building: reads
    // return zero, writes are no-ops.
    void (*getAccruals)(void* building,
                        double* outVat, double* outProfit);
    void (*addAccrual)(void* building, TsmAccrualKind kind, double amount);
    void (*clearAccrual)(void* building, TsmAccrualKind kind);

    // --- period totals -----------------------------------------------------
    // The enterprise's running totals for the current profit period,
    // cleared at the boundary the profit tax engine decides on.
    //
    //   getPeriodTotals     reads them.
    //   clearPeriodTotals   zeroes them. Called by the profit tax engine
    //                       at the end of a period, whether or not the
    //                       profit was positive.
    void (*getPeriodTotals)(void* building,
                            double* outGrossSales, double* outVatAccrued,
                            double* outWholesaleCosts, double* outWagesPaid,
                            double* outForeignCosts);
    void (*clearPeriodTotals)(void* building);

    // --- investment pool ---------------------------------------------------
    // The pool is a single global double: capital siphoned out of
    // private enterprises and available for private construction. Not
    // the treasury.
    //
    // modifyPrivateInvestmentPool refuses to take the pool below zero.
    // getMonthlyPoolInflow is the total siphoned this calendar month,
    // reset at each month boundary.
    double (*getPrivateInvestmentPool)(void);
    void   (*modifyPrivateInvestmentPool)(double delta, const char* reason);
    double (*getMonthlyPoolInflow)(void);

    // --- private economy summary -------------------------------------------
    //
    // The aggregate activity of every private enterprise during the most
    // recently closed month: total gross sales, total wages paid, total
    // tax remitted, and the pool siphon triggered at the same boundary.
    //
    // This is a poll-based getter, not an event. The value is captured by
    // enterprise_base at the month boundary, before the per-enterprise
    // monthly accumulators are cleared, and stays constant until the
    // next boundary. A consumer that reads it immediately after the
    // month changes sees the month that just closed, not zeros.
    //
    // Zero before the first month boundary of a session. A null out
    // pointer is ignored. Safe to call from any thread.
    //
    // This is a *separate* figure from the treasury totals the
    // state_ledger accumulates from currency's flow observers: a
    // private sale or private wage never touches the treasury, so it
    // never appears in a treasury ledger. This getter is how a plugin
    // sees private-sector activity that the treasury cannot.
    void (*getMonthlyEconomySummary)(double* outSales, double* outWages,
                                     double* outTaxes, double* outPoolInflow);

    // --- v6: in-window panel snapshot --------------------------------------
    //
    // describeEnterprise fills `out` from one building under the register
    // lock and returns 1 when the building is in the register, 0 otherwise
    // (out is still zeroed with structSize set, so a caller can use it as a
    // "nothing to show" signal). isPrivate==0 means the building is tracked
    // but not tagged [P] - the panel draws nothing for it.
    //
    // Safe from any thread; never blocks longer than the lock.
    int (*describeEnterprise)(void* building, TsmEnterpriseInfo* out);

    // v7: successful evictions executed during the current calendar month.
    // Incremented in the eviction dispatch of CollectResidentialRent
    // (successful moves only) and reset at the start of each month
    // boundary (OnMonth). Safe to call from any thread.
    size_t (*getMonthlyEvictionCount)(void);

    // v8 (Task 7): the residential rent SETTING - the neutral per-tenant
    // monthly base CollectResidentialRent starts from (attraction /
    // park-cap / rent-slider multipliers apply per building afterwards).
    // Already resolved to the ACTIVE currency units (RUB, or USD-scaled
    // when USD is active) - the same convention every other getter uses.
    // Zero when residential rent is disabled. Safe from any thread; no
    // lock is taken (plain double read of a Start-resolved constant).
    double (*getResidentialRentMonthly)(void);

} TsmEnterpriseBaseApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_ENTERPRISE_BASE_API_H