#ifndef TSM_ALMANAC_STATISTICS_HISTORY_H
#define TSM_ALMANAC_STATISTICS_HISTORY_H

#include <cstdint>
#include <cstddef>
#include <mutex>

namespace almanac {

// ---------------------------------------------------------------------------
// Capacity constants. Fixed at compile time so the render-read path never
// allocates. Documented in docs/almanac/02-statistics-history.md.
// ---------------------------------------------------------------------------
constexpr size_t SH_MAX_METRICS = 96;
constexpr size_t SH_HISTORY_CAPACITY = 120;   // bounded ring of closed periods
constexpr size_t SH_ID_LEN = 64;
constexpr size_t SH_SOURCE_LEN = 64;
constexpr size_t SH_UNIT_LEN = 16;
constexpr size_t SH_SYMBOL_LEN = 8;

// ---------------------------------------------------------------------------
// Value class: structural distinction. Never aggregate across classes.
// ---------------------------------------------------------------------------
enum ValueClass : int {
    VC_ACTUAL = 0,
    VC_DRY_RUN = 1,
    VC_ESTIMATED = 2
};

enum MetricKind : int {
    MK_STOCK = 0,
    MK_FLOW = 1
};

enum ResetReason : int {
    RR_MANUAL = 0,
    RR_SAVE_LOAD = 1,
    RR_WORLD_CHANGE = 2,
    RR_BACKWARD_TIME = 3,
    RR_CURRENCY_CHANGE = 4
};

struct SimDate {
    int day = -1;
    int month = -1;
    int year = -1;
    bool valid() const { return year >= 0 && month >= 1 && month <= 12 && day >= 0; }
};

enum MetricId : int {
    MID_UNKNOWN = 0,
    // Overview
    MID_OVERVIEW_TREASURY = 1,
    MID_OVERVIEW_WALLETS_TOTAL,
    MID_OVERVIEW_MONEY_SUPPLY,
    MID_OVERVIEW_INVESTMENT_POOL,
    MID_OVERVIEW_ENTERPRISE_CAPITAL_TOTAL,
    MID_OVERVIEW_ENTERPRISE_COUNT,
    // Money Flow (monthly, closed-period flows)
    MID_FLOW_WAGES,
    MID_FLOW_PENSIONS,
    MID_FLOW_WELFARE,
    MID_FLOW_STATE_STORE_SALES,
    MID_FLOW_WHOLESALE_B2B,
    MID_FLOW_VAT,
    MID_FLOW_PROFIT_TAX,
    MID_FLOW_PROPERTY_TAX,
    MID_FLOW_NET_TREASURY,
    MID_FLOW_UNEXPLAINED,
    // Private Economy
    MID_PRIVATE_SALES,
    MID_PRIVATE_WAGES,
    MID_PRIVATE_TAXES,
    MID_PRIVATE_POOL_INFLOW,
    MID_PRIVATE_ENTERPRISE_COUNT,
    MID_PRIVATE_EVICTIONS,
    MID_PRIVATE_RENT_MONTHLY,
    // Prices & Citizens
    MID_PRICE_FOOD,
    MID_PRICE_MEAT,
    MID_PRICE_CLOTHES,
    MID_PRICE_ELECTRONICS,
    MID_CPI_DAILY_BASKET,
    MID_MARKET_MULT_FOOD,
    MID_CITIZEN_WALLETS_TOTAL,
    MID_CITIZEN_COUNT,
    // Logistics & Production
    MID_LOGISTICS_FUEL_CONSUMED,
    MID_LOGISTICS_TRIP_COUNT,
    MID_LOGISTICS_TONS_MOVED,
    MID_LOGISTICS_DRY_RUN_FLAG,
    MID_PRODUCTION_OUTPUT,
    MID_COUNT
};

struct Metric {
    uint32_t schemaVersion = 1;
    MetricId id = MID_UNKNOWN;
    char idString[SH_ID_LEN] = {0};
    int hasValue = 0;
    double value = 0.0;
    int valueFinite = 0;
    char unit[SH_UNIT_LEN] = {0};
    int currency = -1;          // -1 = not a currency; 0 = RUB, 1 = USD
    int kind = MK_STOCK;
    SimDate periodStart;
    SimDate periodEnd;
    SimDate asOf;
    char source[SH_SOURCE_LEN] = {0};
    int valueClass = VC_ACTUAL;
    int available = 0;
    int stale = 0;
};

struct Snapshot {
    uint32_t schemaVersion = 1;
    SimDate asOf;
    int currency = -1;
    char currencySymbol[SH_SYMBOL_LEN] = {0};
    Metric metrics[SH_MAX_METRICS];
    size_t metricCount = 0;
    uint64_t sequence = 0;
    int coherent = 1;
};

// ---------------------------------------------------------------------------
// Injected read-only source adapters. The portable core never reads game
// memory or services; integration wires these to public services.
// Every function pointer is optional (may be null). userData is passed through.
// ---------------------------------------------------------------------------
struct ClockSource {
    void* userData = nullptr;
    int (*getDay)(void*) = nullptr;
    int (*getMonth)(void*) = nullptr;
    int (*getYear)(void*) = nullptr;
    int (*getDaysPerMonth)(void*) = nullptr;
};

struct CurrencySource {
    void* userData = nullptr;
    int (*getActiveCurrency)(void*) = nullptr;   // 0 RUB, 1 USD
    const char* (*getSymbol)(void*) = nullptr;
    int (*getTreasury)(void*, int currency, double* out) = nullptr;
    int (*getTotalWallets)(void*, double* out) = nullptr;
};

struct EnterpriseSource {
    void* userData = nullptr;
    int (*getCount)(void*, size_t* out) = nullptr;
    int (*getCapitalSum)(void*, double* out) = nullptr;
    int (*getMonthlySummary)(void*, double* sales, double* wages,
                             double* taxes, double* pool) = nullptr;
    int (*getPool)(void*, double* out) = nullptr;
    int (*getMonthlyEvictions)(void*, size_t* out) = nullptr;
    int (*getRentMonthly)(void*, double* out) = nullptr;
};

struct PricingSource {
    void* userData = nullptr;
    int (*getFoodPrice)(void*, int currency, double* out) = nullptr;
    int (*getMeatPrice)(void*, int currency, double* out) = nullptr;
    int (*getClothesPrice)(void*, int currency, double* out) = nullptr;
    int (*getElectronicsPrice)(void*, int currency, double* out) = nullptr;
    int (*getCpiBasket)(void*, double injectedRent, double* out) = nullptr;
    int (*getMarketMultFood)(void*, double* out) = nullptr;
};

struct FlowSource {
    void* userData = nullptr;
    int (*getWages)(void*, double* out) = nullptr;
    int (*getPensions)(void*, double* out) = nullptr;
    int (*getWelfare)(void*, double* out) = nullptr;
    int (*getStateStoreSales)(void*, double* out) = nullptr;
    int (*getWholesale)(void*, double* out) = nullptr;
    int (*getVat)(void*, double* out) = nullptr;
    int (*getProfitTax)(void*, double* out) = nullptr;
    int (*getPropertyTax)(void*, double* out) = nullptr;
    int (*getNetTreasury)(void*, double* out) = nullptr;
    int (*getUnexplained)(void*, double* out) = nullptr;
};

struct LogisticsSource {
    void* userData = nullptr;
    int (*getFuelConsumed)(void*, double* out) = nullptr;   // dry-run semantics
    int (*getTripCount)(void*, double* out) = nullptr;
    int (*getTonsMoved)(void*, double* out) = nullptr;
    int (*isDryRun)(void*, int* out) = nullptr;
};

// Optional source of exact period metadata for flow metrics. A missing or
// rejected period leaves periodStart/periodEnd invalid; the core never invents
// a month boundary from a day-of-year value.
struct PeriodSource {
    void* userData = nullptr;
    int (*getMetricPeriod)(void*, MetricId id, SimDate* start, SimDate* end) = nullptr;
};

struct Sources {
    ClockSource clock;
    CurrencySource currency;
    EnterpriseSource enterprise;
    PricingSource pricing;
    FlowSource flow;
    LogisticsSource logistics;
    PeriodSource periods;
};

class StatisticsHistory {
public:
    StatisticsHistory();
    ~StatisticsHistory();

    void setSources(const Sources& s);

    // Reset derived state. Clears current metrics (marked unavailable) and
    // bounded history. Derived history may be reset; no economic state is
    // persisted by this module.
    void reset(ResetReason reason);

    // Rebuild the current snapshot from injected sources using `asOf`.
    // Returns false (and performs a backward-time reset) when `asOf` is
    // earlier than the last observed date. Also resets on currency change.
    bool update(const SimDate& asOf);

    // Copy the current snapshot into the bounded history ring under `asOf`.
    void recordHistory(const SimDate& asOf);

    // Coherent copied snapshot for the render thread. No live pointers.
    Snapshot getSnapshot() const;

    size_t historyCount() const;
    // Copy up to destCapacity oldest->newest samples. Returns count copied.
    size_t readHistory(Snapshot* dest, size_t destCapacity) const;

    static const char* metricIdString(MetricId id);
    static constexpr size_t maxMetrics() { return SH_MAX_METRICS; }
    static constexpr size_t historyCapacity() { return SH_HISTORY_CAPACITY; }

private:
    mutable std::mutex mutex_;
    Sources sources_;
    Snapshot current_;
    SimDate lastAsOf_;
    int lastCurrency_ = -1;
    uint64_t sequence_ = 0;
    bool hasLastAsOf_ = false;

    // bounded ring
    Snapshot ring_[SH_HISTORY_CAPACITY];
    size_t ringCount_ = 0;
    size_t ringWrite_ = 0;

    void clearCurrentUnavail_();
    void buildCurrent_(const SimDate& asOf, int currency);
    static int cmpDate_(const SimDate& a, const SimDate& b);
    bool isBackward_(const SimDate& asOf) const;
};

} // namespace almanac

#endif // TSM_ALMANAC_STATISTICS_HISTORY_H
