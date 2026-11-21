// state_ledger_api.h - monthly state budget & flow snapshot service.
//
// Level 1. Consumes sim_clock, currency, persistence and ui_overlay;
// provides read-only closed-month and live-month treasury budget totals so
// UI panels (such as Almanac) can display state budget telemetry without
// duplicating flow-observer hooks.

#ifndef TSM_STATE_LEDGER_API_H
#define TSM_STATE_LEDGER_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_STATE_LEDGER "state_ledger"
#define TSM_STATE_LEDGER_VERSION 1u

typedef struct TsmStateBudgetSnapshot {
    unsigned structSize;        // caller sets = sizeof(TsmStateBudgetSnapshot)

    // Most recently closed calendar month (closedYear == 0 if none yet)
    int    closedYear;
    int    closedMonth;
    double closedWages;         // signed sum: wage + pension + welfare
    double closedStoreSales;    // state_store_sale
    double closedWholesale;     // wholesale_b2b
    double closedTaxes;         // vat + profit_tax + property_tax
    double closedOther;         // procurement / residual treasury flows
    double closedNet;           // total closed-month treasury flow

    // Live in-progress month accumulators
    double liveWages;           // wage + pension + welfare so far this month
    double liveStoreSales;      // state_store_sale so far this month
    double liveWholesale;       // wholesale_b2b so far this month
    double liveTaxes;           // vat + profit_tax + property_tax so far this month
    double liveOther;           // procurement / residual flows so far this month
    double liveNet;             // total flow so far this month
} TsmStateBudgetSnapshot;

typedef struct TsmStateLedgerApi {
    unsigned structSize;

    // Populates `outSnap` (up to `snapSize` bytes). Returns 1 on success,
    // 0 on invalid arguments. Safe to call from any thread.
    int (*getBudgetSnapshot)(TsmStateBudgetSnapshot* outSnap, unsigned snapSize);
} TsmStateLedgerApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_STATE_LEDGER_API_H
